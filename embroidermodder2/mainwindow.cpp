/**
 * \file mainwindow.cpp
 */

#include "mainwindow.h"
#include "embroidermodder.h"
#include "statusbar.h"
#include "statusbar-button.h"
#include "view.h"
#include "cmdprompt.h"
#include "property-editor.h"
#include "undo-editor.h"
#include "preview-dialog.h"
#include "object-data.h"

#include <vector>
#include <stdlib.h>

#include <QDebug>
#include <QFrame>
#include <QVBoxLayout>
#include <QMenu>
#include <QMenuBar>
#include <QStatusBar>
#include <QMdiArea>
#include <QWidget>
#include <QMdiSubWindow>
#include <QMessageBox>
#include <QToolBar>
#include <QFileDialog>
#include <QApplication>
#include <QDate>
#include <QFileInfo>
#include <QLabel>
#include <QComboBox>
#include <QCloseEvent>
#include <QMetaObject>
#include <QLocale>

MainWindow* _mainWin = 0;

/**
 * @brief mainWin
 * @return
 */
MainWindow* mainWin()
{
    return _mainWin;
}

/**
 * @brief MainWindow::MainWindow
 */
MainWindow::MainWindow() : QMainWindow(0)
{
    readSettings();

    QString appDir = qApp->applicationDirPath();
    //Verify that files/directories needed are actually present.
    QFileInfo check(appDir + "/commands");
    if(!check.exists())
        QMessageBox::critical(this, tr("Path Error"), tr("Cannot locate: ") + check.absoluteFilePath());
    check = QFileInfo(appDir + "/help");
    if(!check.exists())
        QMessageBox::critical(this, tr("Path Error"), tr("Cannot locate: ") + check.absoluteFilePath());
    check = QFileInfo(appDir + "/icons");
    if(!check.exists())
        QMessageBox::critical(this, tr("Path Error"), tr("Cannot locate: ") + check.absoluteFilePath());
    check = QFileInfo(appDir + "/images");
    if(!check.exists())
        QMessageBox::critical(this, tr("Path Error"), tr("Cannot locate: ") + check.absoluteFilePath());
    check = QFileInfo(appDir + "/samples");
    if(!check.exists())
        QMessageBox::critical(this, tr("Path Error"), tr("Cannot locate: ") + check.absoluteFilePath());
    check = QFileInfo(appDir + "/translations");
    if(!check.exists())
        QMessageBox::critical(this, tr("Path Error"), tr("Cannot locate: ") + check.absoluteFilePath());

    QString lang = getSettingsGeneralLanguage();
    qDebug("language: %s", qPrintable(lang));
    if(lang == "system")
        lang = QLocale::system().languageToString(QLocale::system().language()).toLower();

    //Load translations for the Embroidermodder 2 GUI
    QTranslator translatorEmb;
    translatorEmb.load(appDir + "/translations/" + lang + "/embroidermodder2_" + lang);
    qApp->installTranslator(&translatorEmb);

    //Load translations for the commands
    QTranslator translatorCmd;
    translatorCmd.load(appDir + "/translations/" + lang + "/commands_" + lang);
    qApp->installTranslator(&translatorCmd);

    //Load translations provided by Qt - this covers dialog buttons and other common things.
    QTranslator translatorQt;
    translatorQt.load("qt_" + QLocale::system().name(), QLibraryInfo::location(QLibraryInfo::TranslationsPath)); //TODO: ensure this always loads, ship a copy of this with the app
    qApp->installTranslator(&translatorQt);

    //Init
    mainWin = this;
    //Menus
    fileMenu     = new QMenu(tr("&File"),     this);
    editMenu     = new QMenu(tr("&Edit"),     this);
    viewMenu     = new QMenu(tr("&View"),     this);
    settingsMenu = new QMenu(tr("&Settings"), this);
    windowMenu   = new QMenu(tr("&Window"),   this);
    helpMenu     = new QMenu(tr("&Help"),     this);
    //SubMenus
    recentMenu   = new QMenu(tr("Open &Recent"), this);
    zoomMenu     = new QMenu(tr("&Zoom"),        this);
    panMenu      = new QMenu(tr("&Pan"),         this);
    //Toolbars
    toolbarFile       = addToolBar(tr("File"));
    toolbarEdit       = addToolBar(tr("Edit"));
    toolbarView       = addToolBar(tr("View"));
    toolbarZoom       = addToolBar(tr("Zoom"));
    toolbarPan        = addToolBar(tr("Pan"));
    toolbarIcon       = addToolBar(tr("Icon"));
    toolbarHelp       = addToolBar(tr("Help"));
    toolbarLayer      = addToolBar(tr("Layer"));
    toolbarProperties = addToolBar(tr("Properties"));
    toolbarText       = addToolBar(tr("Text"));
    toolbarPrompt     = addToolBar(tr("Command Prompt"));
    //Selectors
    layerSelector      = new QComboBox(this);
    colorSelector      = new QComboBox(this);
    linetypeSelector   = new QComboBox(this);
    lineweightSelector = new QComboBox(this);
    textFontSelector   = new QFontComboBox(this);
    textSizeSelector   = new QComboBox(this);

    numOfDocs = 0;
    docIndex = 0;

    shiftKeyPressedState = false;

    setWindowIcon(QIcon(appDir + "/icons/" + getSettingsGeneralIconTheme() + "/" + "app" + ".png"));
    setMinimumSize(800, 480); //Require Minimum WVGA

    loadFormats();

    //create the mdiArea
    QFrame* vbox = new QFrame(this);
    QVBoxLayout* layout = new QVBoxLayout(vbox);
    //layout->setMargin(0);
    vbox->setFrameStyle(QFrame::StyledPanel | QFrame::Sunken);
    mdiArea = new MdiArea(this, vbox);
    mdiArea->useBackgroundLogo(getSettingsGeneralMdiBGUseLogo());
    mdiArea->useBackgroundTexture(getSettingsGeneralMdiBGUseTexture());
    mdiArea->useBackgroundColor(getSettingsGeneralMdiBGUseColor());
    mdiArea->setBackgroundLogo(getSettingsGeneralMdiBGLogo());
    mdiArea->setBackgroundTexture(getSettingsGeneralMdiBGTexture());
    mdiArea->setBackgroundColor(QColor(getSettingsGeneralMdiBGColor()));
    mdiArea->setViewMode(QMdiArea::TabbedView);
    mdiArea->setHorizontalScrollBarPolicy(Qt::ScrollBarAsNeeded);
    mdiArea->setVerticalScrollBarPolicy(Qt::ScrollBarAsNeeded);
    mdiArea->setActivationOrder(QMdiArea::ActivationHistoryOrder);
    layout->addWidget(mdiArea);
    setCentralWidget(vbox);

    //create the Command Prompt
    prompt = new CmdPrompt(this);
    prompt->setFocus(Qt::OtherFocusReason);
    this->setFocusProxy(prompt);
    mdiArea->setFocusProxy(prompt);

    prompt->setPromptTextColor(QColor(getSettingsPromptTextColor()));
    prompt->setPromptBackgroundColor(QColor(getSettingsPromptBGColor()));

    connect(prompt, SIGNAL(startCommand(const QString&)), this, SLOT(logPromptInput(const QString&)));

    connect(prompt, SIGNAL(startCommand(const QString&)), this, SLOT(runCommandMain(const QString&)));
    connect(prompt, SIGNAL(runCommand(const QString&, const QString&)), this, SLOT(runCommandPrompt(const QString&, const QString&)));

            connect(prompt, SIGNAL(deletePressed()),    this, SLOT(deletePressed()));
    //TODO: connect(prompt, SIGNAL(tabPressed()),       this, SLOT(someUnknownSlot()));
            connect(prompt, SIGNAL(escapePressed()),    this, SLOT(escapePressed()));
            connect(prompt, SIGNAL(upPressed()),        this, SLOT(promptInputPrevious()));
            connect(prompt, SIGNAL(downPressed()),      this, SLOT(promptInputNext()));
            connect(prompt, SIGNAL(F1Pressed()),        this, SLOT(help()));
    //TODO: connect(prompt, SIGNAL(F2Pressed()),        this, SLOT(floatHistory()));
    //TODO: connect(prompt, SIGNAL(F3Pressed()),        this, SLOT(toggleQSNAP()));
            connect(prompt, SIGNAL(F4Pressed()),        this, SLOT(toggleLwt())); //TODO: typically this is toggleTablet(), make F-Keys customizable thru settings
    //TODO: connect(prompt, SIGNAL(F5Pressed()),        this, SLOT(toggleISO()));
    //TODO: connect(prompt, SIGNAL(F6Pressed()),        this, SLOT(toggleCoords()));
            connect(prompt, SIGNAL(F7Pressed()),        this, SLOT(toggleGrid()));
    //TODO: connect(prompt, SIGNAL(F8Pressed()),        this, SLOT(toggleORTHO()));
    //TODO: connect(prompt, SIGNAL(F9Pressed()),        this, SLOT(toggleSNAP()));
    //TODO: connect(prompt, SIGNAL(F10Pressed()),       this, SLOT(togglePOLAR()));
    //TODO: connect(prompt, SIGNAL(F11Pressed()),       this, SLOT(toggleQTRACK()));
            connect(prompt, SIGNAL(F12Pressed()),       this, SLOT(toggleRuler()));
            connect(prompt, SIGNAL(cutPressed()),       this, SLOT(cut()));
            connect(prompt, SIGNAL(copyPressed()),      this, SLOT(copy()));
            connect(prompt, SIGNAL(pastePressed()),     this, SLOT(paste()));
            connect(prompt, SIGNAL(selectAllPressed()), this, SLOT(selectAll()));
            connect(prompt, SIGNAL(undoPressed()),      this, SLOT(undo()));
            connect(prompt, SIGNAL(redoPressed()),      this, SLOT(redo()));

            connect(prompt, SIGNAL(shiftPressed()),     this, SLOT(setShiftPressed()));
            connect(prompt, SIGNAL(shiftReleased()),    this, SLOT(setShiftReleased()));

            connect(prompt, SIGNAL(showSettings()),     this, SLOT(settingsPrompt()));

            connect(prompt, SIGNAL(historyAppended(const QString&)), this, SLOT(promptHistoryAppended(const QString&)));

    //create the Object Property Editor
    dockPropEdit = new PropertyEditor(appDir + "/icons/" + getSettingsGeneralIconTheme(), getSettingsSelectionModePickAdd(), prompt, this);
    addDockWidget(Qt::LeftDockWidgetArea, dockPropEdit);
    connect(dockPropEdit, SIGNAL(pickAddModeToggled()), this, SLOT(pickAddModeToggled()));

    //create the Command History Undo Editor
    dockUndoEdit = new UndoEditor(appDir + "/icons/" + getSettingsGeneralIconTheme(), prompt, this);
    addDockWidget(Qt::LeftDockWidgetArea, dockUndoEdit);

    //setDockOptions(QMainWindow::AnimatedDocks | QMainWindow::AllowTabbedDocks | QMainWindow::VerticalTabs); //TODO: Load these from settings
    //tabifyDockWidget(dockPropEdit, dockUndoEdit); //TODO: load this from settings

    //Javascript
    _mainWin = this;

    /*
    engine = new QScriptEngine(this);
    engine->installTranslatorFunctions();
    debugger = new QScriptEngineDebugger(this);
    debugger->attachTo(engine);
    javaInitNatives(engine);

    //Load all commands in a loop
    QDir commandDir(appDir + "/commands");
    QStringList cmdList = commandDir.entryList(QDir::Dirs | QDir::NoDotAndDotDot);
    foreach(QString cmdName, cmdList)
    {
        javaLoadCommand(cmdName);
    }
    */

    statusbar = new StatusBar(this, this);
    this->setStatusBar(statusbar);

    createAllActions();
    createAllMenus();
    createAllToolbars();

    iconResize(getSettingsGeneralIconSize());
    updateMenuToolbarStatusbar();

    //Show date in statusbar after it has been updated
    QDate date = QDate::currentDate();
    QString datestr = date.toString("MMMM d, yyyy");
    statusbar->showMessage(datestr);

    showNormal();

    //Load tips from external file
    QFile tipFile(appDir + "/tips.txt");
    if(tipFile.open(QFile::ReadOnly))
    {
        QTextStream stream(&tipFile);
        QString tipLine;
        do
        {
            tipLine = stream.readLine();
            if(!tipLine.isEmpty())
                listTipOfTheDay << tipLine;
        }
        while(!tipLine.isNull());
    }
    if(getSettingsGeneralTipOfTheDay())
        tipOfTheDay();
}

/**
 * @brief MainWindow::~MainWindow
 */
MainWindow::~MainWindow()
{
    qDebug("MainWindow::Destructor()");

    //Prevent memory leaks by deleting any unpasted objects
    qDeleteAll(cutCopyObjectList.begin(), cutCopyObjectList.end());
    cutCopyObjectList.clear();
}

/**
 * @brief MainWindow::run_script_file
 * @param fname The path of the script to run.
 */
std::string
MainWindow::run_script_file(std::string fname)
{
    std::string output = "";
    /*
    char line[200];
    FILE *f;
    int line_pos = 0;
    f = fopen(filename, "r");
    char c;
    while (fread(&c, 1, 1, f)) {
        if (c != '\n') {
            line[line_pos] = c;
            line_pos++;
        }
        else {
            line[line_pos+1] = 0;
            actuator(line);
            line_pos = 0;
        }
    }
    fclose(f);
    */
    return output;
}

/**
 * @brief A basic line-by-line script processor to allow for extensions to the program.
 *
 * Since the actuator uses command line style parsing, a script is just a text
 * file with each line a compatible command.
 *
 * It should be stressed that this has no control flow on purpose. We don't want
 * this to be hacked into a full scripting language that could cause havoc on
 * the user's system.
 *
 * However, it may be useful to set and get variables and define macros:
 * neither of these will allow for endless loops, stack overflow or other
 * problems that third-party scripts could introduce.
 *
 *     example.sh
 *     ------------------------------------------------------------------
 *     # Save characters by defining functions.
 *     # The syntax features
 *     # Semi-colon ';' seperates out lines like in bash.
 *     # The line ending is the end of the function, but the style
 *     # is a shell function, so we need to write the end brace.
 *
 *     donut() { circle $1 $2 $3 $5 ; circle $1 $2 $4 $5 }
 *
 *     donut 10 20 20 black
 *     donut 20 40 20 black
 *     ------------------------------------------------------------------
 */
std::string
MainWindow::run_script(std::vector<std::string> script)
{
    std::string output = "";
    for (std::string s : script) {
        output += actuator(s);
    }
    return output;
}

/**
 * @brief MainWindow::actuator
 * @param command
 */
std::string
MainWindow::actuator(std::string command)
{
    std::string output = "<br/><font color=\"red\">Unknown command \"" + command + "\". Press F1 for help.</font>";
    if (command == "about") {
        output = "";
        about();
    }
    if (command == "help") {
        output = "";
        help();
    }
    if (command == "platform") {
        output = "<br/>" + platformString().toStdString();
    }
    if (command == "version") {
        output = "<br/>2.0.0-alpha";
    }
    return output;
}

/**
 * \brief .
 */
typedef struct EmbScriptValue_ {
    int int_value;
    EmbReal real_value;
    EmbString string_value;
    int type;
} EmbScriptValue;

/**
 * \brief .
 */
typedef struct EmbFunction_ {
    EmbString label;
    int arguments;
    std::vector<EmbString> body;
} EmbFunction;

/**
 * \brief .
 */
typedef struct EmbScriptContext_ {
    std::vector<EmbScriptValue> arguments;
    std::vector<EmbFunction> functions;
} EmbScriptContext;

/**
 * \brief .
 */
typedef struct EmbAction_ {
    EmbString init;
    EmbString main;
    EmbString mouse;
    EmbString click;
    EmbString prompt;
} EmbAction;

EmbScriptContext engine;

#if 0
void
MainWindow::javaInitNatives(EmbScriptContext* engine)
{
    /*
    EmbScriptValue scriptValDebug = engine->newFunction(javaDebug);
    engine->globalObject().setProperty("debug", scriptValDebug);
    EmbScriptValue scriptValError = engine->newFunction(javaError);
    engine->globalObject().setProperty("error", scriptValError);
    EmbScriptValue scriptValTodo = engine->newFunction(javaTodo);
    engine->globalObject().setProperty("todo", scriptValTodo);
    EmbScriptValue scriptValAlert = engine->newFunction(javaAlert);
    engine->globalObject().setProperty("alert", scriptValAlert);
    EmbScriptValue scriptValBlinkPrompt = engine->newFunction(javaBlinkPrompt);
    engine->globalObject().setProperty("blinkPrompt", scriptValBlinkPrompt);
    EmbScriptValue scriptValSetPromptPrefix = engine->newFunction(javaSetPromptPrefix);
    engine->globalObject().setProperty("setPromptPrefix", scriptValSetPromptPrefix);
    EmbScriptValue scriptValAppendPromptHistory = engine->newFunction(javaAppendPromptHistory);
    engine->globalObject().setProperty("appendPromptHistory", scriptValAppendPromptHistory);
    EmbScriptValue scriptValEnablePromptRapidFire = engine->newFunction(javaEnablePromptRapidFire);
    engine->globalObject().setProperty("enablePromptRapidFire", scriptValEnablePromptRapidFire);
    EmbScriptValue scriptValDisablePromptRapidFire = engine->newFunction(javaDisablePromptRapidFire);
    engine->globalObject().setProperty("disablePromptRapidFire", scriptValDisablePromptRapidFire);
    EmbScriptValue scriptValEnableMoveRapidFire = engine->newFunction(javaEnableMoveRapidFire);
    engine->globalObject().setProperty("enableMoveRapidFire", scriptValEnableMoveRapidFire);
    EmbScriptValue scriptValDisableMoveRapidFire = engine->newFunction(javaDisableMoveRapidFire);
    engine->globalObject().setProperty("disableMoveRapidFire", scriptValDisableMoveRapidFire);
    EmbScriptValue scriptValInitCommand = engine->newFunction(javaInitCommand);
    engine->globalObject().setProperty("initCommand", scriptValInitCommand);
    EmbScriptValue scriptValEndCommand = engine->newFunction(javaEndCommand);
    engine->globalObject().setProperty("endCommand", scriptValEndCommand);
    EmbScriptValue scriptValNewFile = engine->newFunction(javaNewFile);
    engine->globalObject().setProperty("newFile", scriptValNewFile);
    EmbScriptValue scriptValOpenFile = engine->newFunction(javaOpenFile);
    engine->globalObject().setProperty("openFile", scriptValOpenFile);
    EmbScriptValue scriptValExit = engine->newFunction(javaExit);
    engine->globalObject().setProperty("exit", scriptValExit);
    EmbScriptValue scriptValHelp = engine->newFunction(javaHelp);
    engine->globalObject().setProperty("help", scriptValHelp);
    EmbScriptValue scriptValAbout = engine->newFunction(javaAbout);
    engine->globalObject().setProperty("about", scriptValAbout);
    EmbScriptValue scriptValTipOfTheDay = engine->newFunction(javaTipOfTheDay);
    engine->globalObject().setProperty("tipOfTheDay", scriptValTipOfTheDay);
    EmbScriptValue scriptValWindowCascade = engine->newFunction(javaWindowCascade);
    engine->globalObject().setProperty("windowCascade", scriptValWindowCascade);
    EmbScriptValue scriptValWindowTile = engine->newFunction(javaWindowTile);
    engine->globalObject().setProperty("windowTile", scriptValWindowTile);
    EmbScriptValue scriptValWindowClose = engine->newFunction(javaWindowClose);
    engine->globalObject().setProperty("windowClose", scriptValWindowClose);
    EmbScriptValue scriptValWindowCloseAll = engine->newFunction(javaWindowCloseAll);
    engine->globalObject().setProperty("windowCloseAll", scriptValWindowCloseAll);
    EmbScriptValue scriptValWindowNext = engine->newFunction(javaWindowNext);
    engine->globalObject().setProperty("windowNext", scriptValWindowNext);
    EmbScriptValue scriptValWindowPrevious = engine->newFunction(javaWindowPrevious);
    engine->globalObject().setProperty("windowPrevious", scriptValWindowPrevious);
    EmbScriptValue scriptValPlatformString = engine->newFunction(javaPlatformString);
    engine->globalObject().setProperty("platformString", scriptValPlatformString);
    EmbScriptValue scriptValMessageBox = engine->newFunction(javaMessageBox);
    engine->globalObject().setProperty("messageBox", scriptValMessageBox);
    EmbScriptValue scriptValIsInt = engine->newFunction(javaIsInt);
    engine->globalObject().setProperty("isInt", scriptValIsInt);
    EmbScriptValue scriptValUndo = engine->newFunction(javaUndo);
    engine->globalObject().setProperty("undo", scriptValUndo);
    EmbScriptValue scriptValRedo = engine->newFunction(javaRedo);
    engine->globalObject().setProperty("redo", scriptValRedo);
    EmbScriptValue scriptValIcon16 = engine->newFunction(javaIcon16);
    engine->globalObject().setProperty("icon16", scriptValIcon16);
    EmbScriptValue scriptValIcon24 = engine->newFunction(javaIcon24);
    engine->globalObject().setProperty("icon24", scriptValIcon24);
    EmbScriptValue scriptValIcon32 = engine->newFunction(javaIcon32);
    engine->globalObject().setProperty("icon32", scriptValIcon32);
    EmbScriptValue scriptValIcon48 = engine->newFunction(javaIcon48);
    engine->globalObject().setProperty("icon48", scriptValIcon48);
    EmbScriptValue scriptValIcon64 = engine->newFunction(javaIcon64);
    engine->globalObject().setProperty("icon64", scriptValIcon64);
    EmbScriptValue scriptValIcon128 = engine->newFunction(javaIcon128);
    engine->globalObject().setProperty("icon128", scriptValIcon128);
    EmbScriptValue scriptValPanLeft = engine->newFunction(javaPanLeft);
    engine->globalObject().setProperty("panLeft", scriptValPanLeft);
    EmbScriptValue scriptValPanRight = engine->newFunction(javaPanRight);
    engine->globalObject().setProperty("panRight", scriptValPanRight);
    EmbScriptValue scriptValPanUp = engine->newFunction(javaPanUp);
    engine->globalObject().setProperty("panUp", scriptValPanUp);
    EmbScriptValue scriptValPanDown = engine->newFunction(javaPanDown);
    engine->globalObject().setProperty("panDown", scriptValPanDown);
    EmbScriptValue scriptValZoomIn = engine->newFunction(javaZoomIn);
    engine->globalObject().setProperty("zoomIn", scriptValZoomIn);
    EmbScriptValue scriptValZoomOut = engine->newFunction(javaZoomOut);
    engine->globalObject().setProperty("zoomOut", scriptValZoomOut);
    EmbScriptValue scriptValZoomExtents = engine->newFunction(javaZoomExtents);
    engine->globalObject().setProperty("zoomExtents", scriptValZoomExtents);
    EmbScriptValue scriptValPrintArea = engine->newFunction(javaPrintArea);
    engine->globalObject().setProperty("printArea", scriptValPrintArea);
    EmbScriptValue scriptValDayVision = engine->newFunction(javaDayVision);
    engine->globalObject().setProperty("dayVision", scriptValDayVision);
    EmbScriptValue scriptValNightVision = engine->newFunction(javaNightVision);
    engine->globalObject().setProperty("nightVision", scriptValNightVision);
    EmbScriptValue scriptValSetBackgroundColor = engine->newFunction(javaSetBackgroundColor);
    engine->globalObject().setProperty("setBackgroundColor", scriptValSetBackgroundColor);
    EmbScriptValue scriptValSetCrossHairColor = engine->newFunction(javaSetCrossHairColor);
    engine->globalObject().setProperty("setCrossHairColor", scriptValSetCrossHairColor);
    EmbScriptValue scriptValSetGridColor = engine->newFunction(javaSetGridColor);
    engine->globalObject().setProperty("setGridColor", scriptValSetGridColor);
    EmbScriptValue scriptValTextFont = engine->newFunction(javaTextFont);
    engine->globalObject().setProperty("textFont", scriptValTextFont);
    EmbScriptValue scriptValTextSize = engine->newFunction(javaTextSize);
    engine->globalObject().setProperty("textSize", scriptValTextSize);
    EmbScriptValue scriptValTextAngle = engine->newFunction(javaTextAngle);
    engine->globalObject().setProperty("textAngle", scriptValTextAngle);
    EmbScriptValue scriptValTextBold = engine->newFunction(javaTextBold);
    engine->globalObject().setProperty("textBold", scriptValTextBold);
    EmbScriptValue scriptValTextItalic = engine->newFunction(javaTextItalic);
    engine->globalObject().setProperty("textItalic", scriptValTextItalic);
    EmbScriptValue scriptValTextUnderline = engine->newFunction(javaTextUnderline);
    engine->globalObject().setProperty("textUnderline", scriptValTextUnderline);
    EmbScriptValue scriptValTextStrikeOut = engine->newFunction(javaTextStrikeOut);
    engine->globalObject().setProperty("textStrikeOut", scriptValTextStrikeOut);
    EmbScriptValue scriptValTextOverline = engine->newFunction(javaTextOverline);
    engine->globalObject().setProperty("textOverline", scriptValTextOverline);
    EmbScriptValue scriptValSetTextFont = engine->newFunction(javaSetTextFont);
    engine->globalObject().setProperty("setTextFont", scriptValSetTextFont);
    EmbScriptValue scriptValSetTextSize = engine->newFunction(javaSetTextSize);
    engine->globalObject().setProperty("setTextSize", scriptValSetTextSize);
    EmbScriptValue scriptValSetTextAngle = engine->newFunction(javaSetTextAngle);
    engine->globalObject().setProperty("setTextAngle", scriptValSetTextAngle);
    EmbScriptValue scriptValSetTextBold = engine->newFunction(javaSetTextBold);
    engine->globalObject().setProperty("setTextBold", scriptValSetTextBold);
    EmbScriptValue scriptValSetTextItalic = engine->newFunction(javaSetTextItalic);
    engine->globalObject().setProperty("setTextItalic", scriptValSetTextItalic);
    EmbScriptValue scriptValSetTextUnderline = engine->newFunction(javaSetTextUnderline);
    engine->globalObject().setProperty("setTextUnderline", scriptValSetTextUnderline);
    EmbScriptValue scriptValSetTextStrikeOut = engine->newFunction(javaSetTextStrikeOut);
    engine->globalObject().setProperty("setTextStrikeOut", scriptValSetTextStrikeOut);
    EmbScriptValue scriptValSetTextOverline = engine->newFunction(javaSetTextOverline);
    engine->globalObject().setProperty("setTextOverline", scriptValSetTextOverline);
    EmbScriptValue scriptValPreviewOn = engine->newFunction(javaPreviewOn);
    engine->globalObject().setProperty("previewOn", scriptValPreviewOn);
    EmbScriptValue scriptValPreviewOff = engine->newFunction(javaPreviewOff);
    engine->globalObject().setProperty("previewOff", scriptValPreviewOff);
    EmbScriptValue scriptValVulcanize = engine->newFunction(javaVulcanize);
    engine->globalObject().setProperty("vulcanize", scriptValVulcanize);
    EmbScriptValue scriptValAllowRubber = engine->newFunction(javaAllowRubber);
    engine->globalObject().setProperty("allowRubber", scriptValAllowRubber);
    EmbScriptValue scriptValSetRubberMode = engine->newFunction(javaSetRubberMode);
    engine->globalObject().setProperty("setRubberMode", scriptValSetRubberMode);
    EmbScriptValue scriptValSetRubberPoint = engine->newFunction(javaSetRubberPoint);
    engine->globalObject().setProperty("setRubberPoint", scriptValSetRubberPoint);
    EmbScriptValue scriptValSetRubberText = engine->newFunction(javaSetRubberText);
    engine->globalObject().setProperty("setRubberText", scriptValSetRubberText);
    EmbScriptValue scriptValAddRubber = engine->newFunction(javaAddRubber);
    engine->globalObject().setProperty("addRubber", scriptValAddRubber);
    EmbScriptValue scriptValClearRubber = engine->newFunction(javaClearRubber);
    engine->globalObject().setProperty("clearRubber", scriptValClearRubber);
    EmbScriptValue scriptValSpareRubber = engine->newFunction(javaSpareRubber);
    engine->globalObject().setProperty("spareRubber", scriptValSpareRubber);
    EmbScriptValue scriptValAddTextMulti = engine->newFunction(javaAddTextMulti);
    engine->globalObject().setProperty("addTextMulti", scriptValAddTextMulti);
    EmbScriptValue scriptValAddTextSingle = engine->newFunction(javaAddTextSingle);
    engine->globalObject().setProperty("addTextSingle", scriptValAddTextSingle);
    EmbScriptValue scriptValAddInfiniteLine = engine->newFunction(javaAddInfiniteLine);
    engine->globalObject().setProperty("addInfiniteLine", scriptValAddInfiniteLine);
    EmbScriptValue scriptValAddRay = engine->newFunction(javaAddRay);
    engine->globalObject().setProperty("addRay", scriptValAddRay);
    EmbScriptValue scriptValAddLine = engine->newFunction(javaAddLine);
    engine->globalObject().setProperty("addLine", scriptValAddLine);
    EmbScriptValue scriptValAddTriangle = engine->newFunction(javaAddTriangle);
    engine->globalObject().setProperty("addTriangle", scriptValAddTriangle);
    EmbScriptValue scriptValAddRectangle = engine->newFunction(javaAddRectangle);
    engine->globalObject().setProperty("addRectangle", scriptValAddRectangle);
    EmbScriptValue scriptValAddRoundedRectangle = engine->newFunction(javaAddRoundedRectangle);
    engine->globalObject().setProperty("addRoundedRectangle", scriptValAddRoundedRectangle);
    EmbScriptValue scriptValAddArc = engine->newFunction(javaAddArc);
    engine->globalObject().setProperty("addArc", scriptValAddArc);
    EmbScriptValue scriptValAddCircle = engine->newFunction(javaAddCircle);
    engine->globalObject().setProperty("addCircle", scriptValAddCircle);
    EmbScriptValue scriptValAddEllipse = engine->newFunction(javaAddEllipse);
    engine->globalObject().setProperty("addEllipse", scriptValAddEllipse);
    EmbScriptValue scriptValAddPoint = engine->newFunction(javaAddPoint);
    engine->globalObject().setProperty("addPoint", scriptValAddPoint);
    EmbScriptValue scriptValAddRegularPolygon = engine->newFunction(javaAddRegularPolygon);
    engine->globalObject().setProperty("addRegularPolygon", scriptValAddRegularPolygon);
    EmbScriptValue scriptValAddPolygon = engine->newFunction(javaAddPolygon);
    engine->globalObject().setProperty("addPolygon", scriptValAddPolygon);
    EmbScriptValue scriptValAddPolyline = engine->newFunction(javaAddPolyline);
    engine->globalObject().setProperty("addPolyline", scriptValAddPolyline);
    EmbScriptValue scriptValAddPath = engine->newFunction(javaAddPath);
    engine->globalObject().setProperty("addPath", scriptValAddPath);
    EmbScriptValue scriptValAddHorizontalDimension = engine->newFunction(javaAddHorizontalDimension);
    engine->globalObject().setProperty("addHorizontalDimension", scriptValAddHorizontalDimension);
    EmbScriptValue scriptValAddVerticalDimension = engine->newFunction(javaAddVerticalDimension);
    engine->globalObject().setProperty("addVerticalDimension", scriptValAddVerticalDimension);
    EmbScriptValue scriptValAddImage = engine->newFunction(javaAddImage);
    engine->globalObject().setProperty("addImage", scriptValAddImage);
    EmbScriptValue scriptValAddDimLeader = engine->newFunction(javaAddDimLeader);
    engine->globalObject().setProperty("addDimLeader", scriptValAddDimLeader);
    EmbScriptValue scriptValSetCursorShape = engine->newFunction(javaSetCursorShape);
    engine->globalObject().setProperty("setCursorShape", scriptValSetCursorShape);
    EmbScriptValue scriptValCalculateAngle = engine->newFunction(javaCalculateAngle);
    engine->globalObject().setProperty("calculateAngle", scriptValCalculateAngle);
    EmbScriptValue scriptValCalculateDistance = engine->newFunction(javaCalculateDistance);
    engine->globalObject().setProperty("calculateDistance", scriptValCalculateDistance);
    EmbScriptValue scriptValPerpendicularDistance = engine->newFunction(javaPerpendicularDistance);
    engine->globalObject().setProperty("perpendicularDistance", scriptValPerpendicularDistance);
    EmbScriptValue scriptValNumSelected = engine->newFunction(javaNumSelected);
    engine->globalObject().setProperty("numSelected", scriptValNumSelected);
    EmbScriptValue scriptValSelectAll = engine->newFunction(javaSelectAll);
    engine->globalObject().setProperty("selectAll", scriptValSelectAll);
    EmbScriptValue scriptValAddToSelection = engine->newFunction(javaAddToSelection);
    engine->globalObject().setProperty("addToSelection", scriptValAddToSelection);
    EmbScriptValue scriptValClearSelection = engine->newFunction(javaClearSelection);
    engine->globalObject().setProperty("clearSelection", scriptValClearSelection);
    EmbScriptValue scriptValDeleteSelected = engine->newFunction(javaDeleteSelected);
    engine->globalObject().setProperty("deleteSelected", scriptValDeleteSelected);
    EmbScriptValue scriptValCutSelected = engine->newFunction(javaCutSelected);
    engine->globalObject().setProperty("cutSelected", scriptValCutSelected);
    EmbScriptValue scriptValCopySelected = engine->newFunction(javaCopySelected);
    engine->globalObject().setProperty("copySelected", scriptValCopySelected);
    EmbScriptValue scriptValPasteSelected = engine->newFunction(javaPasteSelected);
    engine->globalObject().setProperty("pasteSelected", scriptValPasteSelected);
    EmbScriptValue scriptValMoveSelected = engine->newFunction(javaMoveSelected);
    engine->globalObject().setProperty("moveSelected", scriptValMoveSelected);
    EmbScriptValue scriptValScaleSelected = engine->newFunction(javaScaleSelected);
    engine->globalObject().setProperty("scaleSelected", scriptValScaleSelected);
    EmbScriptValue scriptValRotateSelected = engine->newFunction(javaRotateSelected);
    engine->globalObject().setProperty("rotateSelected", scriptValRotateSelected);
    EmbScriptValue scriptValMirrorSelected = engine->newFunction(javaMirrorSelected);
    engine->globalObject().setProperty("mirrorSelected", scriptValMirrorSelected);
    EmbScriptValue scriptValQSnapX = engine->newFunction(javaQSnapX);
    engine->globalObject().setProperty("qsnapX", scriptValQSnapX);
    EmbScriptValue scriptValQSnapY = engine->newFunction(javaQSnapY);
    engine->globalObject().setProperty("qsnapY", scriptValQSnapY);
    EmbScriptValue scriptValMouseX = engine->newFunction(javaMouseX);
    engine->globalObject().setProperty("mouseX", scriptValMouseX);
    EmbScriptValue scriptValMouseY = engine->newFunction(javaMouseY);
    engine->globalObject().setProperty("mouseY", scriptValMouseY);

    EmbScriptValue scriptValInclude = engine->newFunction(javaInclude);
    engine->globalObject().setProperty("include", scriptValInclude);
    */
}

/**
 * @brief MainWindow::javaLoadCommand
 * @param cmdName
 */
void MainWindow::javaLoadCommand(EmbString cmdName)
{
    qDebug("javaLoadCommand(%s)", qPrintable(cmdName));
    /*
    //NOTE: Every QScriptProgram must have a unique function name to call. If every function was called main(), then
    //      the QScriptEngine would only call the last script evaluated (which happens to be main() in another script).
    //      Thus, by adding the cmdName before main(), it becomes line_main(), circle_main(), etc...
    //      Do not change this code unless you really know what you are doing. I mean it.
    EmbString appDir = qApp->applicationDirPath();
    QFile file(appDir + "/commands/" + cmdName + "/" + cmdName + ".js");
    file.open(QIODevice::ReadOnly);
    EmbString script(file.readAll());
    file.close();

    bool done = false;
    bool findNextIndex = true;
    EmbString findFunc = "function";
    EmbString funcName = "";
    QStringList funcList;
    int index = 0;
    while (!done) {
        if (findNextIndex) {
            index = script.indexOf(findFunc, index);
            if (index == -1) {
                done = true;
            }
            else {
                index += findFunc.length();
                findNextIndex = false;
            }
        }
        else {
            QChar ch = script.at(index);
            if (ch == '(') {
                funcName = funcName.simplified();
                funcList.append(funcName);
                funcName.clear();
                findNextIndex = true;
            }
            else
            {
                funcName.append(ch);
                index++;
            }
        }
    }

    QList<QChar> validBeforeChars;
    validBeforeChars << '\t' << '\n' << '\v' << '\f' << '\r' << ' ' << ';' << '(' << ')'
               << '{' << '}' << '!' << '=' << '+' << '-' << '/' << '*' << '%'
               << '<' << '>' << '&' << '|' << '?' << ':' << '^' << '~';
    QList<QChar> validAfterChars;
    validAfterChars << '\t' << '\n' << '\v' << '\f' << '\r' << ' ' << '(';
    foreach (EmbString functionName, funcList) {
        findFunc = functionName;
        funcName = cmdName + "_" + functionName;

        int found = 0;
        done = false;
        index = 0;
        findNextIndex = true;
        while (!done) {
            if (findNextIndex) {
                index = script.indexOf(findFunc, index);
                if (index == -1) {
                    done = true;
                }
                else {
                    findNextIndex = false;
                }
            }
            else {
                QChar charBefore = script.at(index - 1);
                if(validBeforeChars.contains(charBefore))
                {
                    int i = 0;
                    QChar charAfter;
                    do {
                        charAfter = script.at(index + i + findFunc.length());
                        if(charAfter == '(')
                        {
                            found++;
                            script.replace(index, findFunc.length(), funcName);
                        }
                        i++;
                    }
                    while(validAfterChars.contains(charAfter));
                }
                index += findFunc.length();
                findNextIndex = true;
            }
        }
        qDebug("%s found: %d", qPrintable(findFunc), found);
    }
    //TODO: low priority caveat: If a function name is within a string, it is still replaced.

    script.replace("var global = {};", "var " + cmdName + "_global = {};");
    script.replace("global.", cmdName + "_global.");

    engine->evaluate(script);

    QSettings settings(appDir + "/commands/" + cmdName + "/" + cmdName + ".ini", QSettings::IniFormat);
    EmbString menuName    = settings.value("Menu/Name",    "Lost & Found").toString();
    int     menuPos     = settings.value("Menu/Position",             0).toInt();
    EmbString toolbarName = settings.value("ToolBar/Name", "Lost & Found").toString();
    int     toolbarPos  = settings.value("ToolBar/Position",          0).toInt();
    EmbString toolTip     = settings.value("Tips/ToolTip",             "").toString();
    EmbString statusTip   = settings.value("Tips/StatusTip",           "").toString();
    QStringList aliases = settings.value("Prompt/Alias")                .toStringList();

    QAction* ACTION = createAction(cmdName, toolTip, statusTip, true);

    if(toolbarName.toUpper() != "NONE")
    {
        //If the toolbar doesn't exist, create it.
        if(!toolbarHash.value(toolbarName))
        {
            QToolBar* tb = new QToolBar(toolbarName, this);
            tb->setObjectName("toolbar" + toolbarName);
            connect(tb, SIGNAL(topLevelChanged(bool)), this, SLOT(floatingChangedToolBar(bool)));
            addToolBar(Qt::LeftToolBarArea, tb);
            addToolBarBreak(Qt::LeftToolBarArea);
            toolbarHash.insert(toolbarName, tb);
        }

        //TODO: order actions position in toolbar based on .ini setting
        toolbarHash.value(toolbarName)->addAction(ACTION);
    }

    if(menuName.toUpper() != "NONE")
    {
        //If the menu doesn't exist, create it.
        if(!menuHash.value(menuName))
        {
            QMenu* menu = new QMenu(menuName, this);
            menu->setTearOffEnabled(true);
            menuBar()->addMenu(menu);
            menuHash.insert(menuName, menu);
        }

        //TODO: order actions position in menu based on .ini setting
        menuHash.value(menuName)->addAction(ACTION);
    }

    foreach(EmbString alias, aliases)
    {
        prompt->addCommand(alias, cmdName);
    }
    */
}

//NOTE: http://www.qtcentre.org/threads/20432-Can-I-include-a-script-from-script

/**
 * @brief javaInclude
 * @param context
 * @param engine
 * @return
 */
EmbScriptValue javaInclude(QScriptContext* context, QScriptEngine* engine)
{
    EmbString fileName = context->argument(0).toString();
    QFile scriptFile("commands/" + fileName);

    if(!scriptFile.open(QIODevice::ReadOnly))
        return -1;

    QTextStream stream(&scriptFile);
    EmbString s=stream.readAll();
    scriptFile.close();

    QScriptContext* parent=context->parentContext();

    if(parent!=0)
    {
        context->setActivationObject(context->parentContext()->activationObject());
        context->setThisObject(context->parentContext()->thisObject());
    }

    EmbScriptValue result = engine->evaluate(s);

    return 0;
}

/**
 * @brief javaDebug
 * @param context
 * @return
 */
EmbScriptValue javaDebug(QScriptContext* context)
{
    if(context->argumentCount() != 1)    return context->throwError("debug() requires one argument");
    if(!context->argument(0).isString()) return context->throwError(QScriptContext::TypeError, "debug(): first argument is not a string");

    qDebug("%s", qPrintable(context->argument(0).toString()));
    return EmbScriptValue();
}

/**
 * @brief javaError
 * @param context
 * @return
 */
EmbScriptValue javaError(QScriptContext* context)
{
    if(context->argumentCount() != 2)    return context->throwError("error() requires two arguments");
    if(!context->argument(0).isString()) return context->throwError(QScriptContext::TypeError, "error(): first argument is not a string");
    if(!context->argument(1).isString()) return context->throwError(QScriptContext::TypeError, "error(): second argument is not a string");

    EmbString strCmd = context->argument(0).toString();
    EmbString strErr = context->argument(1).toString();

    mainWin()->nativeSetPromptPrefix("ERROR: (" + strCmd + ") " + strErr);
    mainWin()->nativeAppendPromptHistory(QString());
    mainWin()->nativeEndCommand();
    return EmbScriptValue();
}

/**
 * @brief javaTodo
 * @param context
 * @return
 */
EmbScriptValue javaTodo(QScriptContext* context)
{
    if(context->argumentCount() != 2)    return context->throwError("todo() requires two arguments");
    if(!context->argument(0).isString()) return context->throwError(QScriptContext::TypeError, "todo(): first argument is not a string");
    if(!context->argument(1).isString()) return context->throwError(QScriptContext::TypeError, "todo(): second argument is not a string");

    EmbString strCmd  = context->argument(0).toString();
    EmbString strTodo = context->argument(1).toString();

    mainWin()->nativeAlert("TODO: (" + strCmd + ") " + strTodo);
    mainWin()->nativeEndCommand();
    return EmbScriptValue();
}

/**
 * @brief javaAlert
 * @param context
 * @return
 */
EmbScriptValue
javaAlert(QScriptContext* context)
{
    if(context->argumentCount() != 1)    return context->throwError("alert() requires one argument");
    if(!context->argument(0).isString()) return context->throwError(QScriptContext::TypeError, "alert(): first argument is not a string");

    mainWin()->nativeAlert(context->argument(0).toString());
    return EmbScriptValue();
}

/**
 * @brief javaBlinkPrompt
 * @param context
 * @return
 */
EmbScriptValue
javaBlinkPrompt(QScriptContext* context)
{
    if(context->argumentCount() != 0) return context->throwError("blinkPrompt() requires zero arguments");

    mainWin()->nativeBlinkPrompt();
    return EmbScriptValue();
}

/**
 * @brief javaSetPromptPrefix
 * @param context
 * @return
 */
EmbScriptValue
javaSetPromptPrefix(QScriptContext* context)
{
    if(context->argumentCount() != 1)    return context->throwError("setPromptPrefix() requires one argument");
    if(!context->argument(0).isString()) return context->throwError(QScriptContext::TypeError, "setPromptPrefix(): first argument is not a string");

    mainWin()->nativeSetPromptPrefix(context->argument(0).toString());
    return EmbScriptValue();
}

/**
 * @brief javaAppendPromptHistory
 * @param context
 * @return
 */
EmbScriptValue
javaAppendPromptHistory(QScriptContext* context)
{
    int args = context->argumentCount();
    if(args == 0)
    {
        mainWin()->nativeAppendPromptHistory(QString());
    }
    else if(args == 1)
    {
        mainWin()->nativeAppendPromptHistory(context->argument(0).toString());
    }
    else
    {
        return context->throwError("appendPromptHistory() requires one or zero arguments");
    }
    return EmbScriptValue();
}

/**
 * @brief javaEnablePromptRapidFire
 * @param context
 * @return
 */
EmbScriptValue
javaEnablePromptRapidFire(QScriptContext* context)
{
    if(context->argumentCount() != 0) return context->throwError("enablePromptRapidFire() requires zero arguments");

    mainWin()->nativeEnablePromptRapidFire();
    return EmbScriptValue();
}

/**
 * @brief javaDisablePromptRapidFire
 * @param context
 * @return
 */
EmbScriptValue
javaDisablePromptRapidFire(QScriptContext* context)
{
    if(context->argumentCount() != 0) return context->throwError("disablePromptRapidFire() requires zero arguments");

    mainWin()->nativeDisablePromptRapidFire();
    return EmbScriptValue();
}

EmbScriptValue javaEnableMoveRapidFire(QScriptContext* context)
{
    if(context->argumentCount() != 0) return context->throwError("enableMoveRapidFire() requires zero arguments");

    mainWin()->nativeEnableMoveRapidFire();
    return EmbScriptValue();
}

EmbScriptValue javaDisableMoveRapidFire(QScriptContext* context)
{
    if(context->argumentCount() != 0) return context->throwError("disableMoveRapidFire() requires zero arguments");

    mainWin()->nativeDisableMoveRapidFire();
    return EmbScriptValue();
}

EmbScriptValue javaInitCommand(QScriptContext* context)
{
    if(context->argumentCount() != 0) return context->throwError("initCommand() requires zero arguments");

    mainWin()->nativeInitCommand();
    return EmbScriptValue();
}

/**
 * @brief javaEndCommand
 * @param context
 * @return
 */
EmbScriptValue javaEndCommand(QScriptContext* context)
{
    if(context->argumentCount() != 0) return context->throwError("endCommand() requires zero arguments");

    mainWin()->nativeEndCommand();
    return EmbScriptValue();
}

/**
 * @brief javaNewFile
 * @param context
 * @return
 */
EmbScriptValue javaNewFile(QScriptContext* context)
{
    if(context->argumentCount() != 0) return context->throwError("newFile() requires zero arguments");

    mainWin()->nativeNewFile();
    return EmbScriptValue();
}

/**
 * @brief javaOpenFile
 * @param context
 * @return
 */
EmbScriptValue javaOpenFile(QScriptContext* context)
{
    if (context->argumentCount() != 0) {
        return context->throwError("openFile() requires zero arguments");
    }

    mainWin()->nativeOpenFile();
    return EmbScriptValue();
}

/**
 * @brief javaExit
 * @param context
 * @return
 */
EmbScriptValue javaExit(QScriptContext* context)
{
    if(context->argumentCount() != 0) return context->throwError("exit() requires zero arguments");

    mainWin()->nativeExit();
    return EmbScriptValue();
}

EmbScriptValue javaHelp(QScriptContext* context)
{
    if(context->argumentCount() != 0) return context->throwError("help() requires zero arguments");

    mainWin()->nativeHelp();
    return EmbScriptValue();
}

EmbScriptValue javaAbout(QScriptContext* context)
{
    if(context->argumentCount() != 0) return context->throwError("about() requires zero arguments");

    mainWin()->nativeAbout();
    return EmbScriptValue();
}

EmbScriptValue javaTipOfTheDay(QScriptContext* context)
{
    if(context->argumentCount() != 0) return context->throwError("tipOfTheDay() requires zero arguments");

    mainWin()->nativeTipOfTheDay();
    return EmbScriptValue();
}

EmbScriptValue javaWindowCascade(QScriptContext* context)
{
    if(context->argumentCount() != 0) return context->throwError("windowCascade() requires zero arguments");

    mainWin()->nativeWindowCascade();
    return EmbScriptValue();
}

EmbScriptValue javaWindowTile(QScriptContext* context)
{
    if(context->argumentCount() != 0) return context->throwError("windowTile() requires zero arguments");

    mainWin()->nativeWindowTile();
    return EmbScriptValue();
}

EmbScriptValue javaWindowClose(QScriptContext* context)
{
    if(context->argumentCount() != 0) return context->throwError("windowClose() requires zero arguments");

    mainWin()->nativeWindowClose();
    return EmbScriptValue();
}

EmbScriptValue javaWindowCloseAll(QScriptContext* context)
{
    if(context->argumentCount() != 0) return context->throwError("windowCloseAll() requires zero arguments");

    mainWin()->nativeWindowCloseAll();
    return EmbScriptValue();
}

EmbScriptValue javaWindowNext(QScriptContext* context)
{
    if(context->argumentCount() != 0) return context->throwError("windowNext() requires zero arguments");

    mainWin()->nativeWindowNext();
    return EmbScriptValue();
}

EmbScriptValue javaWindowPrevious(QScriptContext* context)
{
    if(context->argumentCount() != 0) return context->throwError("windowPrevious() requires zero arguments");

    mainWin()->nativeWindowPrevious();
    return EmbScriptValue();
}

EmbScriptValue javaPlatformString(QScriptContext* context)
{
    if(context->argumentCount() != 0) return context->throwError("platformString() requires zero arguments");
    return EmbScriptValue(mainWin()->nativePlatformString());
}

EmbScriptValue javaMessageBox(QScriptContext* context)
{
    if(context->argumentCount() != 3)    return context->throwError("messageBox() requires three arguments");
    if(!context->argument(0).isString()) return context->throwError(QScriptContext::TypeError, "messageBox(): first argument is not a string");
    if(!context->argument(1).isString()) return context->throwError(QScriptContext::TypeError, "messageBox(): second argument is not a string");
    if(!context->argument(2).isString()) return context->throwError(QScriptContext::TypeError, "messageBox(): third argument is not a string");

    EmbString type  = context->argument(0).toString().toLower();
    EmbString title = context->argument(1).toString();
    EmbString text  = context->argument(2).toString();

    if(type != "critical" && type != "information" && type != "question" && type != "warning")
        return context->throwError(QScriptContext::UnknownError, "messageBox(): first argument must be \"critical\", \"information\", \"question\" or \"warning\".");

    mainWin()->nativeMessageBox(type, title, text);
    return EmbScriptValue();
}

EmbScriptValue javaIsInt(QScriptContext* context)
{
    if(context->argumentCount() != 1)    return context->throwError("isInt() requires one argument");
    if(!context->argument(0).isNumber()) return context->throwError(QScriptContext::TypeError, "isInt(): first argument is not a number");

    EmbReal num = context->argument(0).toNumber();

    //isNaN check
    if(qIsNaN(num)) return context->throwError(QScriptContext::TypeError, "isInt(): first argument failed isNaN check. There is an error in your code.");

    if(fmod(num, 1) == 0)
        return EmbScriptValue(true);
    return EmbScriptValue(false);
}

EmbScriptValue javaUndo(QScriptContext* context)
{
    if(context->argumentCount() != 0) return context->throwError("undo() requires zero arguments");

    mainWin()->nativeUndo();
    return EmbScriptValue();
}

EmbScriptValue javaRedo(QScriptContext* context)
{
    if(context->argumentCount() != 0) return context->throwError("redo() requires zero arguments");

    mainWin()->nativeRedo();
    return EmbScriptValue();
}

EmbScriptValue javaIcon16(QScriptContext* context)
{
    if(context->argumentCount() != 0) return context->throwError("icon16() requires zero arguments");

    mainWin()->nativeIcon16();
    return EmbScriptValue();
}

EmbScriptValue javaIcon24(QScriptContext* context)
{
    if(context->argumentCount() != 0) return context->throwError("icon24() requires zero arguments");

    mainWin()->nativeIcon24();
    return EmbScriptValue();
}

EmbScriptValue javaIcon32(QScriptContext* context)
{
    if(context->argumentCount() != 0) return context->throwError("icon32() requires zero arguments");

    mainWin()->nativeIcon32();
    return EmbScriptValue();
}

EmbScriptValue javaIcon48(QScriptContext* context)
{
    if(context->argumentCount() != 0) return context->throwError("icon48() requires zero arguments");

    mainWin()->nativeIcon48();
    return EmbScriptValue();
}

EmbScriptValue javaIcon64(QScriptContext* context)
{
    if(context->argumentCount() != 0) return context->throwError("icon64() requires zero arguments");

    mainWin()->nativeIcon64();
    return EmbScriptValue();
}

EmbScriptValue javaIcon128(QScriptContext* context)
{
    if(context->argumentCount() != 0) return context->throwError("icon128() requires zero arguments");

    mainWin()->nativeIcon128();
    return EmbScriptValue();
}

EmbScriptValue javaPanLeft(QScriptContext* context)
{
    if(context->argumentCount() != 0) return context->throwError("panLeft() requires zero arguments");

    mainWin()->nativePanLeft();
    return EmbScriptValue();
}

EmbScriptValue javaPanRight(QScriptContext* context)
{
    if(context->argumentCount() != 0) return context->throwError("panRight() requires zero arguments");

    mainWin()->nativePanRight();
    return EmbScriptValue();
}

EmbScriptValue javaPanUp(QScriptContext* context)
{
    if(context->argumentCount() != 0) return context->throwError("panUp() requires zero arguments");

    mainWin()->nativePanUp();
    return EmbScriptValue();
}

EmbScriptValue javaPanDown(QScriptContext* context)
{
    if(context->argumentCount() != 0) return context->throwError("panDown() requires zero arguments");

    mainWin()->nativePanDown();
    return EmbScriptValue();
}

EmbScriptValue javaZoomIn(QScriptContext* context)
{
    if(context->argumentCount() != 0) return context->throwError("zoomIn() requires zero arguments");

    mainWin()->nativeZoomIn();
    return EmbScriptValue();
}

EmbScriptValue javaZoomOut(QScriptContext* context)
{
    if(context->argumentCount() != 0) return context->throwError("zoomOut() requires zero arguments");

    mainWin()->nativeZoomOut();
    return EmbScriptValue();
}

EmbScriptValue javaZoomExtents(QScriptContext* context)
{
    if(context->argumentCount() != 0) return context->throwError("zoomExtents() requires zero arguments");

    mainWin()->nativeZoomExtents();
    return EmbScriptValue();
}

EmbScriptValue javaPrintArea(QScriptContext* context)
{
    if(context->argumentCount() != 4)    return context->throwError("printArea() requires four arguments");
    if(!context->argument(0).isNumber()) return context->throwError(QScriptContext::TypeError, "printArea(): first argument is not a number");
    if(!context->argument(1).isNumber()) return context->throwError(QScriptContext::TypeError, "printArea(): second argument is not a number");
    if(!context->argument(2).isNumber()) return context->throwError(QScriptContext::TypeError, "printArea(): third argument is not a number");
    if(!context->argument(3).isNumber()) return context->throwError(QScriptContext::TypeError, "printArea(): fourth argument is not a number");

    EmbReal x = context->argument(0).toNumber();
    EmbReal y = context->argument(1).toNumber();
    EmbReal w = context->argument(2).toNumber();
    EmbReal h = context->argument(3).toNumber();

    //isNaN check
    if(qIsNaN(x)) return context->throwError(QScriptContext::TypeError, "printArea(): first argument failed isNaN check. There is an error in your code.");
    if(qIsNaN(y)) return context->throwError(QScriptContext::TypeError, "printArea(): second argument failed isNaN check. There is an error in your code.");
    if(qIsNaN(w)) return context->throwError(QScriptContext::TypeError, "printArea(): third argument failed isNaN check. There is an error in your code.");
    if(qIsNaN(h)) return context->throwError(QScriptContext::TypeError, "printArea(): fourth argument failed isNaN check. There is an error in your code.");

    mainWin()->nativePrintArea(x, y, w, h);
    return EmbScriptValue();
}

EmbScriptValue javaDayVision(QScriptContext* context)
{
    if(context->argumentCount() != 0) return context->throwError("dayVision() requires zero arguments");

    mainWin()->nativeDayVision();
    return EmbScriptValue();
}

EmbScriptValue javaNightVision(QScriptContext* context)
{
    if(context->argumentCount() != 0) return context->throwError("nightVision() requires zero arguments");

    mainWin()->nativeNightVision();
    return EmbScriptValue();
}

EmbScriptValue javaSetBackgroundColor(QScriptContext* context)
{
    if(context->argumentCount() != 3)    return context->throwError("setBackgroundColor() requires three arguments");
    if(!context->argument(0).isNumber()) return context->throwError(QScriptContext::TypeError, "setBackgroundColor(): first argument is not a number");
    if(!context->argument(1).isNumber()) return context->throwError(QScriptContext::TypeError, "setBackgroundColor(): second argument is not a number");
    if(!context->argument(2).isNumber()) return context->throwError(QScriptContext::TypeError, "setBackgroundColor(): third argument is not a number");

    EmbReal r = context->argument(0).toNumber();
    EmbReal g = context->argument(1).toNumber();
    EmbReal b = context->argument(2).toNumber();

    //isNaN check
    if(qIsNaN(r)) return context->throwError(QScriptContext::TypeError, "setBackgroundColor(): first argument failed isNaN check. There is an error in your code.");
    if(qIsNaN(g)) return context->throwError(QScriptContext::TypeError, "setBackgroundColor(): second argument failed isNaN check. There is an error in your code.");
    if(qIsNaN(b)) return context->throwError(QScriptContext::TypeError, "setBackgroundColor(): third argument failed isNaN check. There is an error in your code.");

    if(r < 0 || r > 255) { return context->throwError(QScriptContext::UnknownError, "setBackgroundColor(): r value must be in range 0-255"); }
    if(g < 0 || g > 255) { return context->throwError(QScriptContext::UnknownError, "setBackgroundColor(): g value must be in range 0-255"); }
    if(b < 0 || b > 255) { return context->throwError(QScriptContext::UnknownError, "setBackgroundColor(): b value must be in range 0-255"); }

    mainWin()->nativeSetBackgroundColor(r, g, b);
    return EmbScriptValue();
}

EmbScriptValue javaSetCrossHairColor(QScriptContext* context)
{
    if(context->argumentCount() != 3)    return context->throwError("setCrossHairColor() requires three arguments");
    if(!context->argument(0).isNumber()) return context->throwError(QScriptContext::TypeError, "setCrossHairColor(): first argument is not a number");
    if(!context->argument(1).isNumber()) return context->throwError(QScriptContext::TypeError, "setCrossHairColor(): second argument is not a number");
    if(!context->argument(2).isNumber()) return context->throwError(QScriptContext::TypeError, "setCrossHairColor(): third argument is not a number");

    EmbReal r = context->argument(0).toNumber();
    EmbReal g = context->argument(1).toNumber();
    EmbReal b = context->argument(2).toNumber();

    //isNaN check
    if(qIsNaN(r)) return context->throwError(QScriptContext::TypeError, "setCrossHairColor(): first argument failed isNaN check. There is an error in your code.");
    if(qIsNaN(g)) return context->throwError(QScriptContext::TypeError, "setCrossHairColor(): second argument failed isNaN check. There is an error in your code.");
    if(qIsNaN(b)) return context->throwError(QScriptContext::TypeError, "setCrossHairColor(): third argument failed isNaN check. There is an error in your code.");

    if(r < 0 || r > 255) { return context->throwError(QScriptContext::UnknownError, "setCrossHairColor(): r value must be in range 0-255"); }
    if(g < 0 || g > 255) { return context->throwError(QScriptContext::UnknownError, "setCrossHairColor(): g value must be in range 0-255"); }
    if(b < 0 || b > 255) { return context->throwError(QScriptContext::UnknownError, "setCrossHairColor(): b value must be in range 0-255"); }

    mainWin()->nativeSetCrossHairColor(r, g, b);
    return EmbScriptValue();
}

EmbScriptValue javaSetGridColor(QScriptContext* context)
{
    if(context->argumentCount() != 3)    return context->throwError("setGridColor() requires three arguments");
    if(!context->argument(0).isNumber()) return context->throwError(QScriptContext::TypeError, "setGridColor(): first argument is not a number");
    if(!context->argument(1).isNumber()) return context->throwError(QScriptContext::TypeError, "setGridColor(): second argument is not a number");
    if(!context->argument(2).isNumber()) return context->throwError(QScriptContext::TypeError, "setGridColor(): third argument is not a number");

    EmbReal r = context->argument(0).toNumber();
    EmbReal g = context->argument(1).toNumber();
    EmbReal b = context->argument(2).toNumber();

    //isNaN check
    if(qIsNaN(r)) return context->throwError(QScriptContext::TypeError, "setGridColor(): first argument failed isNaN check. There is an error in your code.");
    if(qIsNaN(g)) return context->throwError(QScriptContext::TypeError, "setGridColor(): second argument failed isNaN check. There is an error in your code.");
    if(qIsNaN(b)) return context->throwError(QScriptContext::TypeError, "setGridColor(): third argument failed isNaN check. There is an error in your code.");

    if(r < 0 || r > 255) { return context->throwError(QScriptContext::UnknownError, "setGridColor(): r value must be in range 0-255"); }
    if(g < 0 || g > 255) { return context->throwError(QScriptContext::UnknownError, "setGridColor(): g value must be in range 0-255"); }
    if(b < 0 || b > 255) { return context->throwError(QScriptContext::UnknownError, "setGridColor(): b value must be in range 0-255"); }

    mainWin()->nativeSetGridColor(r, g, b);
    return EmbScriptValue();
}

EmbScriptValue javaTextFont(QScriptContext* context)
{
    if(context->argumentCount() != 0) return context->throwError("textFont() requires zero arguments");
    return EmbScriptValue(mainWin()->nativeTextFont());
}

EmbScriptValue javaTextSize(QScriptContext* context)
{
    if(context->argumentCount() != 0) return context->throwError("textSize() requires zero arguments");
    return EmbScriptValue(mainWin()->nativeTextSize());
}

EmbScriptValue javaTextAngle(QScriptContext* context)
{
    if(context->argumentCount() != 0) return context->throwError("textAngle() requires zero arguments");
    return EmbScriptValue(mainWin()->nativeTextAngle());
}

EmbScriptValue javaTextBold(QScriptContext* context)
{
    if(context->argumentCount() != 0) return context->throwError("textBold() requires zero arguments");
    return EmbScriptValue(mainWin()->nativeTextBold());
}

EmbScriptValue javaTextItalic(QScriptContext* context)
{
    if(context->argumentCount() != 0) return context->throwError("textItalic() requires zero arguments");
    return EmbScriptValue(mainWin()->nativeTextItalic());
}

EmbScriptValue javaTextUnderline(QScriptContext* context)
{
    if(context->argumentCount() != 0) return context->throwError("textUnderline() requires zero arguments");
    return EmbScriptValue(mainWin()->nativeTextUnderline());
}

EmbScriptValue javaTextStrikeOut(QScriptContext* context)
{
    if(context->argumentCount() != 0) return context->throwError("textStrikeOut() requires zero arguments");
    return EmbScriptValue(mainWin()->nativeTextStrikeOut());
}

EmbScriptValue javaTextOverline(QScriptContext* context)
{
    if(context->argumentCount() != 0) return context->throwError("textOverline() requires zero arguments");
    return EmbScriptValue(mainWin()->nativeTextOverline());
}

EmbScriptValue javaSetTextFont(QScriptContext* context)
{
    if(context->argumentCount() != 1)    return context->throwError("setTextFont() requires one argument");
    if(!context->argument(0).isString()) return context->throwError(QScriptContext::TypeError, "setTextFont(): first argument is not a string");

    mainWin()->nativeSetTextFont(context->argument(0).toString());
    return EmbScriptValue();
}

EmbScriptValue javaSetTextSize(QScriptContext* context)
{
    if(context->argumentCount() != 1)    return context->throwError("setTextSize() requires one argument");
    if(!context->argument(0).isNumber()) return context->throwError(QScriptContext::TypeError, "setTextSize(): first argument is not a number");

    EmbReal num = context->argument(0).toNumber();

    //isNaN check
    if(qIsNaN(num)) return context->throwError(QScriptContext::TypeError, "setTextSize(): first argument failed isNaN check. There is an error in your code.");

    mainWin()->nativeSetTextSize(num);
    return EmbScriptValue();
}

EmbScriptValue javaSetTextAngle(QScriptContext* context)
{
    if(context->argumentCount() != 1)    return context->throwError("setTextAngle() requires one argument");
    if(!context->argument(0).isNumber()) return context->throwError(QScriptContext::TypeError, "setTextAngle(): first argument is not a number");

    EmbReal num = context->argument(0).toNumber();

    //isNaN check
    if(qIsNaN(num)) return context->throwError(QScriptContext::TypeError, "setTextAngle(): first argument failed isNaN check. There is an error in your code.");

    mainWin()->nativeSetTextAngle(num);
    return EmbScriptValue();
}

EmbScriptValue javaSetTextBold(QScriptContext* context)
{
    if(context->argumentCount() != 1)    return context->throwError("setTextBold() requires one argument");
    if(!context->argument(0).isBool()) return context->throwError(QScriptContext::TypeError, "setTextBold(): first argument is not a bool");

    mainWin()->nativeSetTextBold(context->argument(0).toBool());
    return EmbScriptValue();
}

EmbScriptValue javaSetTextItalic(QScriptContext* context)
{
    if(context->argumentCount() != 1)    return context->throwError("setTextItalic() requires one argument");
    if(!context->argument(0).isBool()) return context->throwError(QScriptContext::TypeError, "setTextItalic(): first argument is not a bool");

    mainWin()->nativeSetTextItalic(context->argument(0).toBool());
    return EmbScriptValue();
}

EmbScriptValue javaSetTextUnderline(QScriptContext* context)
{
    if(context->argumentCount() != 1)    return context->throwError("setTextUnderline() requires one argument");
    if(!context->argument(0).isBool()) return context->throwError(QScriptContext::TypeError, "setTextUnderline(): first argument is not a bool");

    mainWin()->nativeSetTextUnderline(context->argument(0).toBool());
    return EmbScriptValue();
}

EmbScriptValue javaSetTextStrikeOut(QScriptContext* context)
{
    if(context->argumentCount() != 1)    return context->throwError("setTextStrikeOut() requires one argument");
    if(!context->argument(0).isBool()) return context->throwError(QScriptContext::TypeError, "setTextStrikeOut(): first argument is not a bool");

    mainWin()->nativeSetTextStrikeOut(context->argument(0).toBool());
    return EmbScriptValue();
}

EmbScriptValue javaSetTextOverline(QScriptContext* context)
{
    if(context->argumentCount() != 1)    return context->throwError("setTextOverline() requires one argument");
    if(!context->argument(0).isBool()) return context->throwError(QScriptContext::TypeError, "setTextOverline(): first argument is not a bool");

    mainWin()->nativeSetTextOverline(context->argument(0).toBool());
    return EmbScriptValue();
}

EmbScriptValue javaPreviewOn(QScriptContext* context)
{
    if(context->argumentCount() != 5)    return context->throwError("previewOn() requires five arguments");
    if(!context->argument(0).isString()) return context->throwError(QScriptContext::TypeError, "previewOn(): first argument is not a string");
    if(!context->argument(1).isString()) return context->throwError(QScriptContext::TypeError, "previewOn(): second argument is not a string");
    if(!context->argument(2).isNumber()) return context->throwError(QScriptContext::TypeError, "previewOn(): third argument is not a number");
    if(!context->argument(3).isNumber()) return context->throwError(QScriptContext::TypeError, "previewOn(): fourth argument is not a number");
    if(!context->argument(4).isNumber()) return context->throwError(QScriptContext::TypeError, "previewOn(): fifth argument is not a number");

    EmbString cloneStr = context->argument(0).toString().toUpper();
    EmbString modeStr  = context->argument(1).toString().toUpper();
    EmbReal x          = context->argument(2).toNumber();
    EmbReal y          = context->argument(3).toNumber();
    EmbReal data       = context->argument(4).toNumber();

    int clone = PREVIEW_CLONE_NULL;
    int mode = PREVIEW_MODE_NULL;
    if     (cloneStr == "SELECTED") { clone = PREVIEW_CLONE_SELECTED; }
    else if(cloneStr == "RUBBER")   { clone = PREVIEW_CLONE_RUBBER;   }
    else                            { return context->throwError(QScriptContext::UnknownError, "previewOn(): first argument must be \"SELECTED\" or \"RUBBER\"."); }

    if     (modeStr == "MOVE")   { mode = PREVIEW_MODE_MOVE;   }
    else if(modeStr == "ROTATE") { mode = PREVIEW_MODE_ROTATE; }
    else if(modeStr == "SCALE")  { mode = PREVIEW_MODE_SCALE;  }
    else                         { return context->throwError(QScriptContext::UnknownError, "previewOn(): second argument must be \"MOVE\", \"ROTATE\" or \"SCALE\"."); }

    //isNaN check
    if(qIsNaN(x))    return context->throwError(QScriptContext::TypeError, "previewOn(): third argument failed isNaN check. There is an error in your code.");
    if(qIsNaN(y))    return context->throwError(QScriptContext::TypeError, "previewOn(): fourth argument failed isNaN check. There is an error in your code.");
    if(qIsNaN(data)) return context->throwError(QScriptContext::TypeError, "previewOn(): fifth argument failed isNaN check. There is an error in your code.");

    mainWin()->nativePreviewOn(clone, mode, x, y, data);
    return EmbScriptValue();
}

EmbScriptValue javaPreviewOff(QScriptContext* context)
{
    if(context->argumentCount() != 0) return context->throwError("previewOff() requires zero arguments");

    mainWin()->nativePreviewOff();
    return EmbScriptValue();
}

EmbScriptValue javaVulcanize(QScriptContext* context)
{
    if(context->argumentCount() != 0) return context->throwError("vulcanize() requires zero arguments");

    mainWin()->nativeVulcanize();
    return EmbScriptValue();
}

EmbScriptValue javaAllowRubber(QScriptContext* context)
{
    if(context->argumentCount() != 0) return context->throwError("allowRubber() requires zero arguments");

    return EmbScriptValue(mainWin()->nativeAllowRubber());
}

EmbScriptValue javaSetRubberMode(QScriptContext* context)
{
    if(context->argumentCount() != 1)    return context->throwError("setRubberMode() requires one argument");
    if(!context->argument(0).isString()) return context->throwError(QScriptContext::TypeError, "setRubberMode(): first argument is not a string");

    EmbString mode = context->argument(0).toString().toUpper();

    if     (mode == "CIRCLE_1P_RAD")                     { mainWin()->nativeSetRubberMode(OBJ_RUBBER_CIRCLE_1P_RAD); }
    else if(mode == "CIRCLE_1P_DIA")                     { mainWin()->nativeSetRubberMode(OBJ_RUBBER_CIRCLE_1P_DIA); }
    else if(mode == "CIRCLE_2P")                         { mainWin()->nativeSetRubberMode(OBJ_RUBBER_CIRCLE_2P); }
    else if(mode == "CIRCLE_3P")                         { mainWin()->nativeSetRubberMode(OBJ_RUBBER_CIRCLE_3P); }
    else if(mode == "CIRCLE_TTR")                        { mainWin()->nativeSetRubberMode(OBJ_RUBBER_CIRCLE_TTR); }
    else if(mode == "CIRCLE_TTR")                        { mainWin()->nativeSetRubberMode(OBJ_RUBBER_CIRCLE_TTT); }

    else if(mode == "DIMLEADER_LINE")                    { mainWin()->nativeSetRubberMode(OBJ_RUBBER_DIMLEADER_LINE); }

    else if(mode == "ELLIPSE_LINE")                      { mainWin()->nativeSetRubberMode(OBJ_RUBBER_ELLIPSE_LINE); }
    else if(mode == "ELLIPSE_MAJORDIAMETER_MINORRADIUS") { mainWin()->nativeSetRubberMode(OBJ_RUBBER_ELLIPSE_MAJORDIAMETER_MINORRADIUS); }
    else if(mode == "ELLIPSE_MAJORRADIUS_MINORRADIUS")   { mainWin()->nativeSetRubberMode(OBJ_RUBBER_ELLIPSE_MAJORRADIUS_MINORRADIUS); }
    else if(mode == "ELLIPSE_ROTATION")                  { mainWin()->nativeSetRubberMode(OBJ_RUBBER_ELLIPSE_ROTATION); }

    else if(mode == "LINE")                              { mainWin()->nativeSetRubberMode(OBJ_RUBBER_LINE); }

    else if(mode == "POLYGON")                           { mainWin()->nativeSetRubberMode(OBJ_RUBBER_POLYGON); }
    else if(mode == "POLYGON_INSCRIBE")                  { mainWin()->nativeSetRubberMode(OBJ_RUBBER_POLYGON_INSCRIBE); }
    else if(mode == "POLYGON_CIRCUMSCRIBE")              { mainWin()->nativeSetRubberMode(OBJ_RUBBER_POLYGON_CIRCUMSCRIBE); }

    else if(mode == "POLYLINE")                          { mainWin()->nativeSetRubberMode(OBJ_RUBBER_POLYLINE); }

    else if(mode == "RECTANGLE")                         { mainWin()->nativeSetRubberMode(OBJ_RUBBER_RECTANGLE); }

    else if(mode == "TEXTSINGLE")                        { mainWin()->nativeSetRubberMode(OBJ_RUBBER_TEXTSINGLE); }

    else                                                 { return context->throwError(QScriptContext::UnknownError, "setRubberMode(): unknown rubberMode value"); }

    return EmbScriptValue();
}

EmbScriptValue javaSetRubberPoint(QScriptContext* context)
{
    if(context->argumentCount() != 3)    return context->throwError("setRubberPoint() requires three arguments");
    if(!context->argument(0).isString()) return context->throwError(QScriptContext::TypeError, "setRubberPoint(): first argument is not a string");
    if(!context->argument(1).isNumber()) return context->throwError(QScriptContext::TypeError, "setRubberPoint(): second argument is not a number");
    if(!context->argument(2).isNumber()) return context->throwError(QScriptContext::TypeError, "setRubberPoint(): third argument is not a number");

    EmbString key = context->argument(0).toString().toUpper();
    EmbReal x     = context->argument(1).toNumber();
    EmbReal y     = context->argument(2).toNumber();

    //isNaN check
    if(qIsNaN(x)) return context->throwError(QScriptContext::TypeError, "setRubberPoint(): second argument failed isNaN check. There is an error in your code.");
    if(qIsNaN(y)) return context->throwError(QScriptContext::TypeError, "setRubberPoint(): third argument failed isNaN check. There is an error in your code.");

    mainWin()->nativeSetRubberPoint(key, x, y);
    return EmbScriptValue();
}

EmbScriptValue javaSetRubberText(QScriptContext* context)
{
    if(context->argumentCount() != 2)    return context->throwError("setRubberText() requires two arguments");
    if(!context->argument(0).isString()) return context->throwError(QScriptContext::TypeError, "setRubberText(): first argument is not a string");
    if(!context->argument(1).isString()) return context->throwError(QScriptContext::TypeError, "setRubberText(): second argument is not a string");

    EmbString key = context->argument(0).toString().toUpper();
    EmbString txt = context->argument(1).toString();

    mainWin()->nativeSetRubberText(key, txt);
    return EmbScriptValue();
}

EmbScriptValue javaAddRubber(QScriptContext* context)
{
    if(context->argumentCount() != 1)    return context->throwError("addRubber() requires one argument");
    if(!context->argument(0).isString()) return context->throwError(QScriptContext::TypeError, "addRubber(): first argument is not a string");

    EmbString objType = context->argument(0).toString().toUpper();

    if(!mainWin()->nativeAllowRubber())
        return context->throwError(QScriptContext::UnknownError, "addRubber(): You must use vulcanize() before you can add another rubber object.");

    EmbReal mx = mainWin()->nativeMouseX();
    EmbReal my = mainWin()->nativeMouseY();

    if     (objType == "ARC")          {} //TODO: handle this type
    else if(objType == "BLOCK")        {} //TODO: handle this type
    else if(objType == "CIRCLE")       { mainWin()->nativeAddCircle(mx, my, 0, false, OBJ_RUBBER_ON); }
    else if(objType == "DIMALIGNED")   {} //TODO: handle this type
    else if(objType == "DIMANGULAR")   {} //TODO: handle this type
    else if(objType == "DIMARCLENGTH") {} //TODO: handle this type
    else if(objType == "DIMDIAMETER")  {} //TODO: handle this type
    else if(objType == "DIMLEADER")    { mainWin()->nativeAddDimLeader(mx, my, mx, my, 0, OBJ_RUBBER_ON); }
    else if(objType == "DIMLINEAR")    {} //TODO: handle this type
    else if(objType == "DIMORDINATE")  {} //TODO: handle this type
    else if(objType == "DIMRADIUS")    {} //TODO: handle this type
    else if(objType == "ELLIPSE")      { mainWin()->nativeAddEllipse(mx, my, 0, 0, 0, 0, OBJ_RUBBER_ON); }
    else if(objType == "ELLIPSEARC")   {} //TODO: handle this type
    else if(objType == "HATCH")        {} //TODO: handle this type
    else if(objType == "IMAGE")        {} //TODO: handle this type
    else if(objType == "INFINITELINE") {} //TODO: handle this type
    else if(objType == "LINE")         { mainWin()->nativeAddLine(mx, my, mx, my, 0, OBJ_RUBBER_ON); }
    else if(objType == "PATH")         {} //TODO: handle this type
    else if(objType == "POINT")        {} //TODO: handle this type
    else if(objType == "POLYGON")      { mainWin()->nativeAddPolygon(mx, my, QPainterPath(), OBJ_RUBBER_ON); }
    else if(objType == "POLYLINE")     { mainWin()->nativeAddPolyline(mx, my, QPainterPath(), OBJ_RUBBER_ON); }
    else if(objType == "RAY")          {} //TODO: handle this type
    else if(objType == "RECTANGLE")    { mainWin()->nativeAddRectangle(mx, my, mx, my, 0, 0, OBJ_RUBBER_ON); }
    else if(objType == "SPLINE")       {} //TODO: handle this type
    else if(objType == "TEXTMULTI")    {} //TODO: handle this type
    else if(objType == "TEXTSINGLE")   { mainWin()->nativeAddTextSingle("", mx, my, 0, false, OBJ_RUBBER_ON); }

    return EmbScriptValue();
}

EmbScriptValue javaClearRubber(QScriptContext* context)
{
    if(context->argumentCount() != 0) return context->throwError("clearRubber() requires zero arguments");

    mainWin()->nativeClearRubber();
    return EmbScriptValue();
}

EmbScriptValue javaSpareRubber(QScriptContext* context)
{
    if(context->argumentCount() != 1)    return context->throwError("spareRubber() requires one argument");
    if(!context->argument(0).isString()) return context->throwError(QScriptContext::TypeError, "spareRubber(): first argument is not a string");

    EmbString objID = context->argument(0).toString().toUpper();

    if     (objID == "PATH")     { mainWin()->nativeSpareRubber(SPARE_RUBBER_PATH);     }
    else if(objID == "POLYGON")  { mainWin()->nativeSpareRubber(SPARE_RUBBER_POLYGON);  }
    else if(objID == "POLYLINE") { mainWin()->nativeSpareRubber(SPARE_RUBBER_POLYLINE); }
    else
    {
        bool ok = false;
        qint64 id = objID.toLongLong(&ok);
        if(!ok) return context->throwError(QScriptContext::TypeError, "spareRubber(): error converting object ID into an int64");
        mainWin()->nativeSpareRubber(id);
    }

    return EmbScriptValue();
}

EmbScriptValue javaAddTextMulti(QScriptContext* context)
{
    if(context->argumentCount() != 5)    return context->throwError("addTextMulti() requires five arguments");
    if(!context->argument(0).isString()) return context->throwError(QScriptContext::TypeError, "addTextMulti(): first argument is not a string");
    if(!context->argument(1).isNumber()) return context->throwError(QScriptContext::TypeError, "addTextMulti(): second argument is not a number");
    if(!context->argument(2).isNumber()) return context->throwError(QScriptContext::TypeError, "addTextMulti(): third argument is not a number");
    if(!context->argument(3).isNumber()) return context->throwError(QScriptContext::TypeError, "addTextMulti(): fourth argument is not a number");
    if(!context->argument(4).isBool())   return context->throwError(QScriptContext::TypeError, "addTextMulti(): fifth argument is not a bool");

    EmbString str   = context->argument(0).toString();
    EmbReal   x     = context->argument(1).toNumber();
    EmbReal   y     = context->argument(2).toNumber();
    EmbReal   rot   = context->argument(3).toNumber();
    bool    fill  = context->argument(4).toBool();

    //isNaN check
    if(qIsNaN(x))   return context->throwError(QScriptContext::TypeError, "addTextMulti(): second argument failed isNaN check. There is an error in your code.");
    if(qIsNaN(y))   return context->throwError(QScriptContext::TypeError, "addTextMulti(): third argument failed isNaN check. There is an error in your code.");
    if(qIsNaN(rot)) return context->throwError(QScriptContext::TypeError, "addTextMulti(): fourth argument failed isNaN check. There is an error in your code.");

    mainWin()->nativeAddTextMulti(str, x, y, rot, fill, OBJ_RUBBER_OFF);
    return EmbScriptValue();
}

EmbScriptValue javaAddTextSingle(QScriptContext* context)
{
    if(context->argumentCount() != 5)    return context->throwError("addTextSingle() requires five arguments");
    if(!context->argument(0).isString()) return context->throwError(QScriptContext::TypeError, "addTextSingle(): first argument is not a string");
    if(!context->argument(1).isNumber()) return context->throwError(QScriptContext::TypeError, "addTextSingle(): second argument is not a number");
    if(!context->argument(2).isNumber()) return context->throwError(QScriptContext::TypeError, "addTextSingle(): third argument is not a number");
    if(!context->argument(3).isNumber()) return context->throwError(QScriptContext::TypeError, "addTextSingle(): fourth argument is not a number");
    if(!context->argument(4).isBool())   return context->throwError(QScriptContext::TypeError, "addTextSingle(): fifth argument is not a bool");

    EmbString str   = context->argument(0).toString();
    EmbReal   x     = context->argument(1).toNumber();
    EmbReal   y     = context->argument(2).toNumber();
    EmbReal   rot   = context->argument(3).toNumber();
    bool    fill  = context->argument(4).toBool();

    //isNaN check
    if(qIsNaN(x))   return context->throwError(QScriptContext::TypeError, "addTextSingle(): second argument failed isNaN check. There is an error in your code.");
    if(qIsNaN(y))   return context->throwError(QScriptContext::TypeError, "addTextSingle(): third argument failed isNaN check. There is an error in your code.");
    if(qIsNaN(rot)) return context->throwError(QScriptContext::TypeError, "addTextSingle(): fourth argument failed isNaN check. There is an error in your code.");

    mainWin()->nativeAddTextSingle(str, x, y, rot, fill, OBJ_RUBBER_OFF);
    return EmbScriptValue();
}

EmbScriptValue javaAddInfiniteLine(QScriptContext* context)
{
    //TODO: parameter error checking
    qDebug("TODO: finish addInfiniteLine command");
    return EmbScriptValue();
}

EmbScriptValue javaAddRay(QScriptContext* context)
{
    //TODO: parameter error checking
    qDebug("TODO: finish addRay command");
    return EmbScriptValue();
}

EmbScriptValue javaAddLine(QScriptContext* context)
{
    if(context->argumentCount() != 5)    return context->throwError("addLine() requires five arguments");
    if(!context->argument(0).isNumber()) return context->throwError(QScriptContext::TypeError, "addLine(): first argument is not a number");
    if(!context->argument(1).isNumber()) return context->throwError(QScriptContext::TypeError, "addLine(): second argument is not a number");
    if(!context->argument(2).isNumber()) return context->throwError(QScriptContext::TypeError, "addLine(): third argument is not a number");
    if(!context->argument(3).isNumber()) return context->throwError(QScriptContext::TypeError, "addLine(): fourth argument is not a number");
    if(!context->argument(4).isNumber()) return context->throwError(QScriptContext::TypeError, "addLine(): fifth argument is not a number");

    EmbReal x1  = context->argument(0).toNumber();
    EmbReal y1  = context->argument(1).toNumber();
    EmbReal x2  = context->argument(2).toNumber();
    EmbReal y2  = context->argument(3).toNumber();
    EmbReal rot = context->argument(4).toNumber();

    //isNaN check
    if(qIsNaN(x1))  return context->throwError(QScriptContext::TypeError, "addLine(): first argument failed isNaN check. There is an error in your code.");
    if(qIsNaN(y1))  return context->throwError(QScriptContext::TypeError, "addLine(): second argument failed isNaN check. There is an error in your code.");
    if(qIsNaN(x2))  return context->throwError(QScriptContext::TypeError, "addLine(): third argument failed isNaN check. There is an error in your code.");
    if(qIsNaN(y2))  return context->throwError(QScriptContext::TypeError, "addLine(): fourth argument failed isNaN check. There is an error in your code.");
    if(qIsNaN(rot)) return context->throwError(QScriptContext::TypeError, "addLine(): fifth argument failed isNaN check. There is an error in your code.");

    mainWin()->nativeAddLine(x1, y1, x2, y2, rot, OBJ_RUBBER_OFF);
    return EmbScriptValue();
}

EmbScriptValue javaAddTriangle(QScriptContext* context)
{
    if(context->argumentCount() != 8)    return context->throwError("addTriangle() requires eight arguments");
    if(!context->argument(0).isNumber()) return context->throwError(QScriptContext::TypeError, "addTriangle(): first argument is not a number");
    if(!context->argument(1).isNumber()) return context->throwError(QScriptContext::TypeError, "addTriangle(): second argument is not a number");
    if(!context->argument(2).isNumber()) return context->throwError(QScriptContext::TypeError, "addTriangle(): third argument is not a number");
    if(!context->argument(3).isNumber()) return context->throwError(QScriptContext::TypeError, "addTriangle(): fourth argument is not a number");
    if(!context->argument(4).isNumber()) return context->throwError(QScriptContext::TypeError, "addTriangle(): fifth argument is not a number");
    if(!context->argument(5).isNumber()) return context->throwError(QScriptContext::TypeError, "addTriangle(): sixth argument is not a number");
    if(!context->argument(6).isNumber()) return context->throwError(QScriptContext::TypeError, "addTriangle(): seventh argument is not a number");
    if(!context->argument(7).isBool())   return context->throwError(QScriptContext::TypeError, "addTriangle(): eighth argument is not a bool");

    EmbReal x1     = context->argument(0).toNumber();
    EmbReal y1     = context->argument(1).toNumber();
    EmbReal x2     = context->argument(2).toNumber();
    EmbReal y2     = context->argument(3).toNumber();
    EmbReal x3     = context->argument(4).toNumber();
    EmbReal y3     = context->argument(5).toNumber();
    EmbReal rot    = context->argument(6).toNumber();
    bool  fill   = context->argument(7).toBool();

    //isNaN check
    if(qIsNaN(x1))  return context->throwError(QScriptContext::TypeError, "addTriangle(): first argument failed isNaN check. There is an error in your code.");
    if(qIsNaN(y1))  return context->throwError(QScriptContext::TypeError, "addTriangle(): second argument failed isNaN check. There is an error in your code.");
    if(qIsNaN(x2))  return context->throwError(QScriptContext::TypeError, "addTriangle(): third argument failed isNaN check. There is an error in your code.");
    if(qIsNaN(y2))  return context->throwError(QScriptContext::TypeError, "addTriangle(): fourth argument failed isNaN check. There is an error in your code.");
    if(qIsNaN(x3))  return context->throwError(QScriptContext::TypeError, "addTriangle(): fifth argument failed isNaN check. There is an error in your code.");
    if(qIsNaN(y3))  return context->throwError(QScriptContext::TypeError, "addTriangle(): sixth argument failed isNaN check. There is an error in your code.");
    if(qIsNaN(rot)) return context->throwError(QScriptContext::TypeError, "addTriangle(): seventh argument failed isNaN check. There is an error in your code.");

    mainWin()->nativeAddTriangle(x1, y1, x2, y2, x3, y3, rot, fill);
    return EmbScriptValue();
}

EmbScriptValue javaAddRectangle(QScriptContext* context)
{
    if(context->argumentCount() != 6)    return context->throwError("addRectangle() requires six arguments");
    if(!context->argument(0).isNumber()) return context->throwError(QScriptContext::TypeError, "addRectangle(): first argument is not a number");
    if(!context->argument(1).isNumber()) return context->throwError(QScriptContext::TypeError, "addRectangle(): second argument is not a number");
    if(!context->argument(2).isNumber()) return context->throwError(QScriptContext::TypeError, "addRectangle(): third argument is not a number");
    if(!context->argument(3).isNumber()) return context->throwError(QScriptContext::TypeError, "addRectangle(): fourth argument is not a number");
    if(!context->argument(4).isNumber()) return context->throwError(QScriptContext::TypeError, "addRectangle(): fifth argument is not a number");
    if(!context->argument(5).isBool())   return context->throwError(QScriptContext::TypeError, "addRectangle(): sixth argument is not a bool");

    EmbReal x    = context->argument(0).toNumber();
    EmbReal y    = context->argument(1).toNumber();
    EmbReal w    = context->argument(2).toNumber();
    EmbReal h    = context->argument(3).toNumber();
    EmbReal rot  = context->argument(4).toNumber();
    bool  fill = context->argument(5).toBool();

    //isNaN check
    if(qIsNaN(x))   return context->throwError(QScriptContext::TypeError, "addRectangle(): first argument failed isNaN check. There is an error in your code.");
    if(qIsNaN(y))   return context->throwError(QScriptContext::TypeError, "addRectangle(): second argument failed isNaN check. There is an error in your code.");
    if(qIsNaN(w))   return context->throwError(QScriptContext::TypeError, "addRectangle(): third argument failed isNaN check. There is an error in your code.");
    if(qIsNaN(h))   return context->throwError(QScriptContext::TypeError, "addRectangle(): fourth argument failed isNaN check. There is an error in your code.");
    if(qIsNaN(rot)) return context->throwError(QScriptContext::TypeError, "addRectangle(): fifth argument failed isNaN check. There is an error in your code.");

    mainWin()->nativeAddRectangle(x, y, w, h, rot, fill, OBJ_RUBBER_OFF);
    return EmbScriptValue();
}

EmbScriptValue javaAddRoundedRectangle(QScriptContext* context)
{
    if(context->argumentCount() != 7)    return context->throwError("addRoundedRectangle() requires seven arguments");
    if(!context->argument(0).isNumber()) return context->throwError(QScriptContext::TypeError, "addRoundedRectangle(): first argument is not a number");
    if(!context->argument(1).isNumber()) return context->throwError(QScriptContext::TypeError, "addRoundedRectangle(): second argument is not a number");
    if(!context->argument(2).isNumber()) return context->throwError(QScriptContext::TypeError, "addRoundedRectangle(): third argument is not a number");
    if(!context->argument(3).isNumber()) return context->throwError(QScriptContext::TypeError, "addRoundedRectangle(): fourth argument is not a number");
    if(!context->argument(4).isNumber()) return context->throwError(QScriptContext::TypeError, "addRoundedRectangle(): fifth argument is not a number");
    if(!context->argument(5).isNumber()) return context->throwError(QScriptContext::TypeError, "addRoundedRectangle(): sixth argument is not a number");
    if(!context->argument(6).isBool())   return context->throwError(QScriptContext::TypeError, "addRoundedRectangle(): seventh argument is not a bool");

    EmbReal x    = context->argument(0).toNumber();
    EmbReal y    = context->argument(1).toNumber();
    EmbReal w    = context->argument(2).toNumber();
    EmbReal h    = context->argument(3).toNumber();
    EmbReal rad  = context->argument(4).toNumber();
    EmbReal rot  = context->argument(5).toNumber();
    bool  fill = context->argument(6).toBool();

    //isNaN check
    if(qIsNaN(x))   return context->throwError(QScriptContext::TypeError, "addRoundedRectangle(): first argument failed isNaN check. There is an error in your code.");
    if(qIsNaN(y))   return context->throwError(QScriptContext::TypeError, "addRoundedRectangle(): second argument failed isNaN check. There is an error in your code.");
    if(qIsNaN(w))   return context->throwError(QScriptContext::TypeError, "addRoundedRectangle(): third argument failed isNaN check. There is an error in your code.");
    if(qIsNaN(h))   return context->throwError(QScriptContext::TypeError, "addRoundedRectangle(): fourth argument failed isNaN check. There is an error in your code.");
    if(qIsNaN(rad)) return context->throwError(QScriptContext::TypeError, "addRoundedRectangle(): fifth argument failed isNaN check. There is an error in your code.");
    if(qIsNaN(rot)) return context->throwError(QScriptContext::TypeError, "addRoundedRectangle(): sixth argument failed isNaN check. There is an error in your code.");

    mainWin()->nativeAddRoundedRectangle(x, y, w, h, rad, rot, fill);
    return EmbScriptValue();
}

EmbScriptValue javaAddArc(QScriptContext* context)
{
    if(context->argumentCount() != 6)    return context->throwError("addArc() requires six arguments");
    if(!context->argument(0).isNumber()) return context->throwError(QScriptContext::TypeError, "addArc(): first argument is not a number");
    if(!context->argument(1).isNumber()) return context->throwError(QScriptContext::TypeError, "addArc(): second argument is not a number");
    if(!context->argument(2).isNumber()) return context->throwError(QScriptContext::TypeError, "addArc(): third argument is not a number");
    if(!context->argument(3).isNumber()) return context->throwError(QScriptContext::TypeError, "addArc(): fourth argument is not a number");
    if(!context->argument(4).isNumber()) return context->throwError(QScriptContext::TypeError, "addArc(): fifth argument is not a number");
    if(!context->argument(5).isNumber()) return context->throwError(QScriptContext::TypeError, "addArc(): sixth argument is not a number");

    EmbReal startX = context->argument(0).toNumber();
    EmbReal startY = context->argument(1).toNumber();
    EmbReal midX   = context->argument(2).toNumber();
    EmbReal midY   = context->argument(3).toNumber();
    EmbReal endX   = context->argument(4).toNumber();
    EmbReal endY   = context->argument(5).toNumber();

    //isNaN check
    if(qIsNaN(startX)) return context->throwError(QScriptContext::TypeError, "addArc(): first argument failed isNaN check. There is an error in your code.");
    if(qIsNaN(startY)) return context->throwError(QScriptContext::TypeError, "addArc(): second argument failed isNaN check. There is an error in your code.");
    if(qIsNaN(midX))   return context->throwError(QScriptContext::TypeError, "addArc(): third argument failed isNaN check. There is an error in your code.");
    if(qIsNaN(midY))   return context->throwError(QScriptContext::TypeError, "addArc(): fourth argument failed isNaN check. There is an error in your code.");
    if(qIsNaN(endX))   return context->throwError(QScriptContext::TypeError, "addArc(): fifth argument failed isNaN check. There is an error in your code.");
    if(qIsNaN(endY))   return context->throwError(QScriptContext::TypeError, "addArc(): sixth argument failed isNaN check. There is an error in your code.");

    mainWin()->nativeAddArc(startX, startY, midX, midY, endX, endY, OBJ_RUBBER_OFF);
    return EmbScriptValue();
}

EmbScriptValue javaAddCircle(QScriptContext* context)
{
    if(context->argumentCount() != 4)    return context->throwError("addCircle() requires four arguments");
    if(!context->argument(0).isNumber()) return context->throwError(QScriptContext::TypeError, "addCircle(): first argument is not a number");
    if(!context->argument(1).isNumber()) return context->throwError(QScriptContext::TypeError, "addCircle(): second argument is not a number");
    if(!context->argument(2).isNumber()) return context->throwError(QScriptContext::TypeError, "addCircle(): third argument is not a number");
    if(!context->argument(3).isBool())   return context->throwError(QScriptContext::TypeError, "addCircle(): fourth argument is not a bool");

    EmbReal centerX = context->argument(0).toNumber();
    EmbReal centerY = context->argument(1).toNumber();
    EmbReal radius  = context->argument(2).toNumber();
    bool  fill    = context->argument(3).toBool();

    //isNaN check
    if(qIsNaN(centerX)) return context->throwError(QScriptContext::TypeError, "addCircle(): first argument failed isNaN check. There is an error in your code.");
    if(qIsNaN(centerY)) return context->throwError(QScriptContext::TypeError, "addCircle(): second argument failed isNaN check. There is an error in your code.");
    if(qIsNaN(radius))  return context->throwError(QScriptContext::TypeError, "addCircle(): third argument failed isNaN check. There is an error in your code.");

    mainWin()->nativeAddCircle(centerX, centerY, radius, fill, OBJ_RUBBER_OFF);
    return EmbScriptValue();
}

EmbScriptValue javaAddSlot(QScriptContext* context)
{
    if(context->argumentCount() != 6)    return context->throwError("addSlot() requires six arguments");
    if(!context->argument(0).isNumber()) return context->throwError(QScriptContext::TypeError, "addSlot(): first argument is not a number");
    if(!context->argument(1).isNumber()) return context->throwError(QScriptContext::TypeError, "addSlot(): second argument is not a number");
    if(!context->argument(2).isNumber()) return context->throwError(QScriptContext::TypeError, "addSlot(): third argument is not a number");
    if(!context->argument(3).isNumber()) return context->throwError(QScriptContext::TypeError, "addSlot(): fourth argument is not a number");
    if(!context->argument(4).isNumber()) return context->throwError(QScriptContext::TypeError, "addSlot(): fifth argument is not a number");
    if(!context->argument(5).isBool())   return context->throwError(QScriptContext::TypeError, "addSlot(): sixth argument is not a bool");

    EmbReal centerX  = context->argument(0).toNumber();
    EmbReal centerY  = context->argument(1).toNumber();
    EmbReal diameter = context->argument(2).toNumber();
    EmbReal length   = context->argument(3).toNumber();
    EmbReal rot      = context->argument(4).toNumber();
    bool  fill     = context->argument(5).toBool();

    //isNaN check
    if(qIsNaN(centerX))  return context->throwError(QScriptContext::TypeError, "addSlot(): first argument failed isNaN check. There is an error in your code.");
    if(qIsNaN(centerY))  return context->throwError(QScriptContext::TypeError, "addSlot(): second argument failed isNaN check. There is an error in your code.");
    if(qIsNaN(diameter)) return context->throwError(QScriptContext::TypeError, "addSlot(): third argument failed isNaN check. There is an error in your code.");
    if(qIsNaN(length))   return context->throwError(QScriptContext::TypeError, "addSlot(): fourth argument failed isNaN check. There is an error in your code.");
    if(qIsNaN(rot))      return context->throwError(QScriptContext::TypeError, "addSlot(): fifth argument failed isNaN check. There is an error in your code.");

    mainWin()->nativeAddSlot(centerX, centerY, diameter, length, rot, fill, OBJ_RUBBER_OFF);
    return EmbScriptValue();
}

EmbScriptValue javaAddEllipse(QScriptContext* context)
{
    if(context->argumentCount() != 6)    return context->throwError("addEllipse() requires six arguments");
    if(!context->argument(0).isNumber()) return context->throwError(QScriptContext::TypeError, "addEllipse(): first argument is not a number");
    if(!context->argument(1).isNumber()) return context->throwError(QScriptContext::TypeError, "addEllipse(): second argument is not a number");
    if(!context->argument(2).isNumber()) return context->throwError(QScriptContext::TypeError, "addEllipse(): third argument is not a number");
    if(!context->argument(3).isNumber()) return context->throwError(QScriptContext::TypeError, "addEllipse(): fourth argument is not a number");
    if(!context->argument(4).isNumber()) return context->throwError(QScriptContext::TypeError, "addEllipse(): fifth argument is not a number");
    if(!context->argument(5).isBool())   return context->throwError(QScriptContext::TypeError, "addEllipse(): sixth argument is not a bool");

    EmbReal centerX = context->argument(0).toNumber();
    EmbReal centerY = context->argument(1).toNumber();
    EmbReal radX    = context->argument(2).toNumber();
    EmbReal radY    = context->argument(3).toNumber();
    EmbReal rot     = context->argument(4).toNumber();
    bool  fill    = context->argument(5).toBool();

    //isNaN check
    if(qIsNaN(centerX)) return context->throwError(QScriptContext::TypeError, "addEllipse(): first argument failed isNaN check. There is an error in your code.");
    if(qIsNaN(centerY)) return context->throwError(QScriptContext::TypeError, "addEllipse(): second argument failed isNaN check. There is an error in your code.");
    if(qIsNaN(radX))    return context->throwError(QScriptContext::TypeError, "addEllipse(): third argument failed isNaN check. There is an error in your code.");
    if(qIsNaN(radY))    return context->throwError(QScriptContext::TypeError, "addEllipse(): fourth argument failed isNaN check. There is an error in your code.");
    if(qIsNaN(rot))     return context->throwError(QScriptContext::TypeError, "addEllipse(): fifth argument failed isNaN check. There is an error in your code.");

    mainWin()->nativeAddEllipse(centerX, centerY, radX, radY, rot, fill, OBJ_RUBBER_OFF);
    return EmbScriptValue();
}

EmbScriptValue javaAddPoint(QScriptContext* context)
{
    if(context->argumentCount() != 2)    return context->throwError("addPoint() requires two arguments");
    if(!context->argument(0).isNumber()) return context->throwError(QScriptContext::TypeError, "addPoint(): first argument is not a number");
    if(!context->argument(1).isNumber()) return context->throwError(QScriptContext::TypeError, "addPoint(): second argument is not a number");

    EmbReal x = context->argument(0).toNumber();
    EmbReal y = context->argument(1).toNumber();

    //isNaN check
    if(qIsNaN(x)) return context->throwError(QScriptContext::TypeError, "addPoint(): first argument failed isNaN check. There is an error in your code.");
    if(qIsNaN(y)) return context->throwError(QScriptContext::TypeError, "addPoint(): second argument failed isNaN check. There is an error in your code.");

    mainWin()->nativeAddPoint(x,y);
    return EmbScriptValue();
}

EmbScriptValue javaAddRegularPolygon(QScriptContext* context)
{
    //TODO: parameter error checking
    qDebug("TODO: finish addRegularPolygon command");
    return EmbScriptValue();
}

EmbScriptValue javaAddPolygon(QScriptContext* context)
{
    if(context->argumentCount() != 1)   return context->throwError("addPolygon() requires one argument");
    if(!context->argument(0).isArray()) return context->throwError(QScriptContext::TypeError, "addPolygon(): first argument is not an array");

    QVariantList varList = context->argument(0).toVariant().toList();
    int varSize = varList.size();
    if(varSize < 2) return context->throwError(QScriptContext::TypeError, "addPolygon(): array must contain at least two elements");
    if(varSize % 2) return context->throwError(QScriptContext::TypeError, "addPolygon(): array cannot contain an odd number of elements");

    bool lineTo = false;
    bool xCoord = true;
    EmbReal x = 0;
    EmbReal y = 0;
    EmbReal startX = 0;
    EmbReal startY = 0;
    QPainterPath path;
    foreach(QVariant var, varList)
    {
        if(var.canConvert(QVariant::Double))
        {
            if(xCoord)
            {
                xCoord = false;
                x = var.toReal();
            }
            else
            {
                xCoord = true;
                y = -var.toReal();

                if(lineTo) { path.lineTo(x,y); }
                else       { path.moveTo(x,y); lineTo = true; startX = x; startY = y; }
            }
        }
        else
            return context->throwError(QScriptContext::TypeError, "addPolygon(): array contains one or more invalid elements");
    }

    //Close the polygon
    path.closeSubpath();

    path.translate(-startX, -startY);

    mainWin()->nativeAddPolygon(startX, startY, path, OBJ_RUBBER_OFF);
    return EmbScriptValue();
}

EmbScriptValue javaAddPolyline(QScriptContext* context)
{
    if(context->argumentCount() != 1)   return context->throwError("addPolyline() requires one argument");
    if(!context->argument(0).isArray()) return context->throwError(QScriptContext::TypeError, "addPolyline(): first argument is not an array");

    QVariantList varList = context->argument(0).toVariant().toList();
    int varSize = varList.size();
    if(varSize < 2) return context->throwError(QScriptContext::TypeError, "addPolyline(): array must contain at least two elements");
    if(varSize % 2) return context->throwError(QScriptContext::TypeError, "addPolyline(): array cannot contain an odd number of elements");

    bool lineTo = false;
    bool xCoord = true;
    EmbReal x = 0;
    EmbReal y = 0;
    EmbReal startX = 0;
    EmbReal startY = 0;
    QPainterPath path;
    foreach (QVariant var, varList) {
        if(var.canConvert(QVariant::Double)) {
            if(xCoord) {
                xCoord = false;
                x = var.toReal();
            }
            else {
                xCoord = true;
                y = -var.toReal();

                if(lineTo) { path.lineTo(x,y); }
                else       { path.moveTo(x,y); lineTo = true; startX = x; startY = y; }
            }
        }
        else
            return context->throwError(QScriptContext::TypeError, "addPolyline(): array contains one or more invalid elements");
    }

    path.translate(-startX, -startY);

    mainWin()->nativeAddPolyline(startX, startY, path, OBJ_RUBBER_OFF);
    return EmbScriptValue();
}

EmbScriptValue javaAddPath(QScriptContext* context)
{
    //TODO: parameter error checking
    qDebug("TODO: finish addPath command");
    return EmbScriptValue();
}

EmbScriptValue javaAddHorizontalDimension(QScriptContext* context)
{
    //TODO: parameter error checking
    qDebug("TODO: finish addHorizontalDimension command");
    return EmbScriptValue();
}

EmbScriptValue javaAddVerticalDimension(QScriptContext* context)
{
    //TODO: parameter error checking
    qDebug("TODO: finish addVerticalDimension command");
    return EmbScriptValue();
}

EmbScriptValue javaAddImage(QScriptContext* context)
{
    //TODO: parameter error checking
    qDebug("TODO: finish addImage command");
    return EmbScriptValue();
}

EmbScriptValue javaAddDimLeader(QScriptContext* context)
{
    if(context->argumentCount() != 5)    return context->throwError("addDimLeader() requires five arguments");
    if(!context->argument(0).isNumber()) return context->throwError(QScriptContext::TypeError, "addDimLeader(): first argument is not a number");
    if(!context->argument(1).isNumber()) return context->throwError(QScriptContext::TypeError, "addDimLeader(): second argument is not a number");
    if(!context->argument(2).isNumber()) return context->throwError(QScriptContext::TypeError, "addDimLeader(): third argument is not a number");
    if(!context->argument(3).isNumber()) return context->throwError(QScriptContext::TypeError, "addDimLeader(): fourth argument is not a number");
    if(!context->argument(4).isNumber()) return context->throwError(QScriptContext::TypeError, "addDimLeader(): fifth argument is not a number");

    EmbReal x1  = context->argument(0).toNumber();
    EmbReal y1  = context->argument(1).toNumber();
    EmbReal x2  = context->argument(2).toNumber();
    EmbReal y2  = context->argument(3).toNumber();
    EmbReal rot = context->argument(4).toNumber();

    //isNaN check
    if(qIsNaN(x1))  return context->throwError(QScriptContext::TypeError, "addDimLeader(): first argument failed isNaN check. There is an error in your code.");
    if(qIsNaN(y1))  return context->throwError(QScriptContext::TypeError, "addDimLeader(): second argument failed isNaN check. There is an error in your code.");
    if(qIsNaN(x2))  return context->throwError(QScriptContext::TypeError, "addDimLeader(): third argument failed isNaN check. There is an error in your code.");
    if(qIsNaN(y2))  return context->throwError(QScriptContext::TypeError, "addDimLeader(): fourth argument failed isNaN check. There is an error in your code.");
    if(qIsNaN(rot)) return context->throwError(QScriptContext::TypeError, "addDimLeader(): fifth argument failed isNaN check. There is an error in your code.");

    mainWin()->nativeAddDimLeader(x1, y1, x2, y2, rot, OBJ_RUBBER_OFF);
    return EmbScriptValue();
}

EmbScriptValue javaSetCursorShape(QScriptContext* context)
{
    if(context->argumentCount() != 1)    return context->throwError("setCursorShape() requires one argument");
    if(!context->argument(0).isString()) return context->throwError(QScriptContext::TypeError, "setCursorShape(): first argument is not a string");

    EmbString shape = context->argument(0).toString();
    mainWin()->nativeSetCursorShape(shape);
    return EmbScriptValue();
}

EmbScriptValue javaCalculateAngle(QScriptContext* context)
{
    if(context->argumentCount() != 4)    return context->throwError("calculateAngle() requires four arguments");
    if(!context->argument(0).isNumber()) return context->throwError(QScriptContext::TypeError, "calculateAngle(): first argument is not a number");
    if(!context->argument(1).isNumber()) return context->throwError(QScriptContext::TypeError, "calculateAngle(): second argument is not a number");
    if(!context->argument(2).isNumber()) return context->throwError(QScriptContext::TypeError, "calculateAngle(): third argument is not a number");
    if(!context->argument(3).isNumber()) return context->throwError(QScriptContext::TypeError, "calculateAngle(): fourth argument is not a number");

    EmbReal x1 = context->argument(0).toNumber();
    EmbReal y1 = context->argument(1).toNumber();
    EmbReal x2 = context->argument(2).toNumber();
    EmbReal y2 = context->argument(3).toNumber();

    //isNaN check
    if(qIsNaN(x1))  return context->throwError(QScriptContext::TypeError, "calculateAngle(): first argument failed isNaN check. There is an error in your code.");
    if(qIsNaN(y1))  return context->throwError(QScriptContext::TypeError, "calculateAngle(): second argument failed isNaN check. There is an error in your code.");
    if(qIsNaN(x2))  return context->throwError(QScriptContext::TypeError, "calculateAngle(): third argument failed isNaN check. There is an error in your code.");
    if(qIsNaN(y2))  return context->throwError(QScriptContext::TypeError, "calculateAngle(): fourth argument failed isNaN check. There is an error in your code.");

    return EmbScriptValue(mainWin()->nativeCalculateAngle(x1, y1, x2, y2));
}

EmbScriptValue javaCalculateDistance(QScriptContext* context)
{
    if(context->argumentCount() != 4)    return context->throwError("calculateDistance() requires four arguments");
    if(!context->argument(0).isNumber()) return context->throwError(QScriptContext::TypeError, "calculateDistance(): first argument is not a number");
    if(!context->argument(1).isNumber()) return context->throwError(QScriptContext::TypeError, "calculateDistance(): second argument is not a number");
    if(!context->argument(2).isNumber()) return context->throwError(QScriptContext::TypeError, "calculateDistance(): third argument is not a number");
    if(!context->argument(3).isNumber()) return context->throwError(QScriptContext::TypeError, "calculateDistance(): fourth argument is not a number");

    EmbReal x1 = context->argument(0).toNumber();
    EmbReal y1 = context->argument(1).toNumber();
    EmbReal x2 = context->argument(2).toNumber();
    EmbReal y2 = context->argument(3).toNumber();

    //isNaN check
    if(qIsNaN(x1))  return context->throwError(QScriptContext::TypeError, "calculateDistance(): first argument failed isNaN check. There is an error in your code.");
    if(qIsNaN(y1))  return context->throwError(QScriptContext::TypeError, "calculateDistance(): second argument failed isNaN check. There is an error in your code.");
    if(qIsNaN(x2))  return context->throwError(QScriptContext::TypeError, "calculateDistance(): third argument failed isNaN check. There is an error in your code.");
    if(qIsNaN(y2))  return context->throwError(QScriptContext::TypeError, "calculateDistance(): fourth argument failed isNaN check. There is an error in your code.");

    return EmbScriptValue(mainWin()->nativeCalculateDistance(x1, y1, x2, y2));
}

EmbScriptValue javaPerpendicularDistance(QScriptContext* context)
{
    if(context->argumentCount() != 6)    return context->throwError("perpendicularDistance() requires six arguments");
    if(!context->argument(0).isNumber()) return context->throwError(QScriptContext::TypeError, "perpendicularDistance(): first argument is not a number");
    if(!context->argument(1).isNumber()) return context->throwError(QScriptContext::TypeError, "perpendicularDistance(): second argument is not a number");
    if(!context->argument(2).isNumber()) return context->throwError(QScriptContext::TypeError, "perpendicularDistance(): third argument is not a number");
    if(!context->argument(3).isNumber()) return context->throwError(QScriptContext::TypeError, "perpendicularDistance(): fourth argument is not a number");
    if(!context->argument(4).isNumber()) return context->throwError(QScriptContext::TypeError, "perpendicularDistance(): fifth argument is not a number");
    if(!context->argument(5).isNumber()) return context->throwError(QScriptContext::TypeError, "perpendicularDistance(): sixth argument is not a number");

    EmbReal px = context->argument(0).toNumber();
    EmbReal py = context->argument(1).toNumber();
    EmbReal x1 = context->argument(2).toNumber();
    EmbReal y1 = context->argument(3).toNumber();
    EmbReal x2 = context->argument(4).toNumber();
    EmbReal y2 = context->argument(5).toNumber();

    //isNaN check
    if(qIsNaN(px))  return context->throwError(QScriptContext::TypeError, "perpendicularDistance(): first argument failed isNaN check. There is an error in your code.");
    if(qIsNaN(py))  return context->throwError(QScriptContext::TypeError, "perpendicularDistance(): second argument failed isNaN check. There is an error in your code.");
    if(qIsNaN(x1))  return context->throwError(QScriptContext::TypeError, "perpendicularDistance(): third argument failed isNaN check. There is an error in your code.");
    if(qIsNaN(y1))  return context->throwError(QScriptContext::TypeError, "perpendicularDistance(): fourth argument failed isNaN check. There is an error in your code.");
    if(qIsNaN(x2))  return context->throwError(QScriptContext::TypeError, "perpendicularDistance(): fifth argument failed isNaN check. There is an error in your code.");
    if(qIsNaN(y2))  return context->throwError(QScriptContext::TypeError, "perpendicularDistance(): sixth argument failed isNaN check. There is an error in your code.");

    return EmbScriptValue(mainWin()->nativePerpendicularDistance(px, py, x1, y1, x2, y2));
}

EmbScriptValue javaNumSelected(QScriptContext* context)
{
    if(context->argumentCount() != 0) return context->throwError("numSelected() requires zero arguments");
    return EmbScriptValue(mainWin()->nativeNumSelected());
}

EmbScriptValue javaSelectAll(QScriptContext* context)
{
    if(context->argumentCount() != 0) return context->throwError("selectAll() requires zero arguments");

    mainWin()->nativeSelectAll();
    return EmbScriptValue();
}

EmbScriptValue javaAddToSelection(QScriptContext* context)
{
    //TODO: finish
    return EmbScriptValue();
}

EmbScriptValue javaClearSelection(QScriptContext* context)
{
    if(context->argumentCount() != 0) return context->throwError("clearSelection() requires zero arguments");

    mainWin()->nativeClearSelection();
    return EmbScriptValue();
}

EmbScriptValue javaDeleteSelected(QScriptContext* context)
{
    if(context->argumentCount() != 0) return context->throwError("deleteSelected() requires zero arguments");

    mainWin()->nativeDeleteSelected();
    return EmbScriptValue();
}

EmbScriptValue javaCutSelected(QScriptContext* context)
{
    if(context->argumentCount() != 2)    return context->throwError("cutSelected() requires two arguments");
    if(!context->argument(0).isNumber()) return context->throwError(QScriptContext::TypeError, "cutSelected(): first argument is not a number");
    if(!context->argument(1).isNumber()) return context->throwError(QScriptContext::TypeError, "cutSelected(): second argument is not a number");

    EmbReal x = context->argument(0).toNumber();
    EmbReal y = context->argument(1).toNumber();

    //isNaN check
    if(qIsNaN(x)) return context->throwError(QScriptContext::TypeError, "cutSelected(): first argument failed isNaN check. There is an error in your code.");
    if(qIsNaN(y)) return context->throwError(QScriptContext::TypeError, "cutSelected(): second argument failed isNaN check. There is an error in your code.");

    mainWin()->nativeCutSelected(x, y);
    return EmbScriptValue();
}

EmbScriptValue javaCopySelected(QScriptContext* context)
{
    if(context->argumentCount() != 2)    return context->throwError("copySelected() requires two arguments");
    if(!context->argument(0).isNumber()) return context->throwError(QScriptContext::TypeError, "copySelected(): first argument is not a number");
    if(!context->argument(1).isNumber()) return context->throwError(QScriptContext::TypeError, "copySelected(): second argument is not a number");

    EmbReal x = context->argument(0).toNumber();
    EmbReal y = context->argument(1).toNumber();

    //isNaN check
    if(qIsNaN(x)) return context->throwError(QScriptContext::TypeError, "copySelected(): first argument failed isNaN check. There is an error in your code.");
    if(qIsNaN(y)) return context->throwError(QScriptContext::TypeError, "copySelected(): second argument failed isNaN check. There is an error in your code.");

    mainWin()->nativeCopySelected(x, y);
    return EmbScriptValue();
}

EmbScriptValue javaPasteSelected(QScriptContext* context)
{
    if(context->argumentCount() != 2)    return context->throwError("pasteSelected() requires two arguments");
    if(!context->argument(0).isNumber()) return context->throwError(QScriptContext::TypeError, "pasteSelected(): first argument is not a number");
    if(!context->argument(1).isNumber()) return context->throwError(QScriptContext::TypeError, "pasteSelected(): second argument is not a number");

    EmbReal x = context->argument(0).toNumber();
    EmbReal y = context->argument(1).toNumber();

    //isNaN check
    if(qIsNaN(x)) return context->throwError(QScriptContext::TypeError, "pasteSelected(): first argument failed isNaN check. There is an error in your code.");
    if(qIsNaN(y)) return context->throwError(QScriptContext::TypeError, "pasteSelected(): second argument failed isNaN check. There is an error in your code.");

    mainWin()->nativePasteSelected(x, y);
    return EmbScriptValue();
}

EmbScriptValue javaMoveSelected(QScriptContext* context)
{
    if(context->argumentCount() != 2)    return context->throwError("moveSelected() requires two arguments");
    if(!context->argument(0).isNumber()) return context->throwError(QScriptContext::TypeError, "moveSelected(): first argument is not a number");
    if(!context->argument(1).isNumber()) return context->throwError(QScriptContext::TypeError, "moveSelected(): second argument is not a number");

    EmbReal dx = context->argument(0).toNumber();
    EmbReal dy = context->argument(1).toNumber();

    //isNaN check
    if(qIsNaN(dx)) return context->throwError(QScriptContext::TypeError, "moveSelected(): first argument failed isNaN check. There is an error in your code.");
    if(qIsNaN(dy)) return context->throwError(QScriptContext::TypeError, "moveSelected(): second argument failed isNaN check. There is an error in your code.");

    mainWin()->nativeMoveSelected(dx, dy);
    return EmbScriptValue();
}

EmbScriptValue javaScaleSelected(QScriptContext* context)
{
    if(context->argumentCount() != 3)    return context->throwError("scaleSelected() requires three arguments");
    if(!context->argument(0).isNumber()) return context->throwError(QScriptContext::TypeError, "scaleSelected(): first argument is not a number");
    if(!context->argument(1).isNumber()) return context->throwError(QScriptContext::TypeError, "scaleSelected(): second argument is not a number");
    if(!context->argument(2).isNumber()) return context->throwError(QScriptContext::TypeError, "scaleSelected(): third argument is not a number");

    EmbReal x      = context->argument(0).toNumber();
    EmbReal y      = context->argument(1).toNumber();
    EmbReal factor = context->argument(2).toNumber();

    //isNaN check
    if(qIsNaN(x))      return context->throwError(QScriptContext::TypeError, "scaleSelected(): first argument failed isNaN check. There is an error in your code.");
    if(qIsNaN(y))      return context->throwError(QScriptContext::TypeError, "scaleSelected(): second argument failed isNaN check. There is an error in your code.");
    if(qIsNaN(factor)) return context->throwError(QScriptContext::TypeError, "scaleSelected(): third argument failed isNaN check. There is an error in your code.");

    if(factor <= 0.0) return context->throwError(QScriptContext::UnknownError, "scaleSelected(): scale factor must be greater than zero");

    mainWin()->nativeScaleSelected(x, y, factor);
    return EmbScriptValue();
}

EmbScriptValue javaRotateSelected(QScriptContext* context)
{
    if(context->argumentCount() != 3)    return context->throwError("rotateSelected() requires three arguments");
    if(!context->argument(0).isNumber()) return context->throwError(QScriptContext::TypeError, "rotateSelected(): first argument is not a number");
    if(!context->argument(1).isNumber()) return context->throwError(QScriptContext::TypeError, "rotateSelected(): second argument is not a number");
    if(!context->argument(2).isNumber()) return context->throwError(QScriptContext::TypeError, "rotateSelected(): third argument is not a number");

    EmbReal x   = context->argument(0).toNumber();
    EmbReal y   = context->argument(1).toNumber();
    EmbReal rot = context->argument(2).toNumber();

    //isNaN check
    if(qIsNaN(x))   return context->throwError(QScriptContext::TypeError, "rotateSelected(): first argument failed isNaN check. There is an error in your code.");
    if(qIsNaN(y))   return context->throwError(QScriptContext::TypeError, "rotateSelected(): second argument failed isNaN check. There is an error in your code.");
    if(qIsNaN(rot)) return context->throwError(QScriptContext::TypeError, "rotateSelected(): third argument failed isNaN check. There is an error in your code.");

    mainWin()->nativeRotateSelected(x, y, rot);
    return EmbScriptValue();
}

EmbScriptValue javaMirrorSelected(QScriptContext* context)
{
    if(context->argumentCount() != 4)    return context->throwError("mirrorSelected() requires four arguments");
    if(!context->argument(0).isNumber()) return context->throwError(QScriptContext::TypeError, "mirrorSelected(): first argument is not a number");
    if(!context->argument(1).isNumber()) return context->throwError(QScriptContext::TypeError, "mirrorSelected(): second argument is not a number");
    if(!context->argument(2).isNumber()) return context->throwError(QScriptContext::TypeError, "mirrorSelected(): third argument is not a number");
    if(!context->argument(3).isNumber()) return context->throwError(QScriptContext::TypeError, "mirrorSelected(): fourth argument is not a number");

    EmbReal x1 = context->argument(0).toNumber();
    EmbReal y1 = context->argument(1).toNumber();
    EmbReal x2 = context->argument(2).toNumber();
    EmbReal y2 = context->argument(3).toNumber();

    //isNaN check
    if(qIsNaN(x1))  return context->throwError(QScriptContext::TypeError, "mirrorSelected(): first argument failed isNaN check. There is an error in your code.");
    if(qIsNaN(y1))  return context->throwError(QScriptContext::TypeError, "mirrorSelected(): second argument failed isNaN check. There is an error in your code.");
    if(qIsNaN(x2))  return context->throwError(QScriptContext::TypeError, "mirrorSelected(): third argument failed isNaN check. There is an error in your code.");
    if(qIsNaN(y2))  return context->throwError(QScriptContext::TypeError, "mirrorSelected(): fourth argument failed isNaN check. There is an error in your code.");

    mainWin()->nativeMirrorSelected(x1, y1, x2, y2);
    return EmbScriptValue();
}

EmbScriptValue javaQSnapX(QScriptContext* context)
{
    if (context->argumentCount() != 0) {
        return context->throwError("qsnapX() requires zero arguments");
    }
    return EmbScriptValue(mainWin()->nativeQSnapX());
}

EmbScriptValue javaQSnapY(QScriptContext* context)
{
    if (context->argumentCount() != 0) {
        return context->throwError("qsnapY() requires zero arguments");
    }
    return EmbScriptValue(mainWin()->nativeQSnapY());
}

EmbScriptValue javaMouseX(QScriptContext* context)
{
    if (context->argumentCount() != 0) {
        return context->throwError("mouseX() requires zero arguments");
    }
    return EmbScriptValue(mainWin()->nativeMouseX());
}

EmbScriptValue javaMouseY(QScriptContext* context)
{
    if (context->argumentCount() != 0) {
        return context->throwError("mouseY() requires zero arguments");
    }
    return EmbScriptValue(mainWin()->nativeMouseY());
}

#endif

/**
 * @brief MainWindow::getAction
 * @param actionEnum
 * @return
 */
QAction*
MainWindow::getAction(int actionEnum)
{
    return actionHash.value(actionEnum);
}

/**
 * @brief MainWindow::recentMenuAboutToShow
 */
void
MainWindow::recentMenuAboutToShow()
{
    qDebug("MainWindow::recentMenuAboutToShow()");
    recentMenu->clear();

    QFileInfo recentFileInfo;
    QString recentValue;
    for(int i = 0; i < settings_opensave_recent_list_of_files.size(); ++i)
    {
        //If less than the max amount of entries add to menu
        if(i < settings_opensave_recent_max_files)
        {
            recentFileInfo = QFileInfo(settings_opensave_recent_list_of_files.at(i));
            if(recentFileInfo.exists() && validFileFormat(recentFileInfo.fileName()))
            {
                recentValue.setNum(i+1);
                QAction* rAction;
                if     (recentValue.toInt() >= 1 && recentValue.toInt() <= 9) rAction = new QAction("&" + recentValue + " " + recentFileInfo.fileName(), this);
                else if(recentValue.toInt() == 10)                            rAction = new QAction("1&0 "                  + recentFileInfo.fileName(), this);
                else                                                          rAction = new QAction(      recentValue + " " + recentFileInfo.fileName(), this);
                rAction->setCheckable(false);
                rAction->setData(settings_opensave_recent_list_of_files.at(i));
                recentMenu->addAction(rAction);
                connect(rAction, SIGNAL(triggered()), this, SLOT(openrecentfile()));
            }
        }
    }
    //Ensure the list only has max amount of entries
    while(settings_opensave_recent_list_of_files.size() > settings_opensave_recent_max_files)
    {
        settings_opensave_recent_list_of_files.removeLast();
    }
}

/**
 * @brief MainWindow::windowMenuAboutToShow
 */
void
MainWindow::windowMenuAboutToShow()
{
    qDebug("MainWindow::windowMenuAboutToShow()");
    windowMenu->clear();
    windowMenu->addAction(actionHash.value(ACTION_windowclose));
    windowMenu->addAction(actionHash.value(ACTION_windowcloseall));
    windowMenu->addSeparator();
    windowMenu->addAction(actionHash.value(ACTION_windowcascade));
    windowMenu->addAction(actionHash.value(ACTION_windowtile));
    windowMenu->addSeparator();
    windowMenu->addAction(actionHash.value(ACTION_windownext));
    windowMenu->addAction(actionHash.value(ACTION_windowprevious));

    windowMenu->addSeparator();
    QList<QMdiSubWindow*> windows = mdiArea->subWindowList();
    for(int i = 0; i < windows.count(); ++i)
    {
        QAction* aAction = new QAction(windows.at(i)->windowTitle(), this);
        aAction->setCheckable(true);
        aAction->setData(i);
        windowMenu->addAction(aAction);
        connect(aAction, SIGNAL(toggled(bool)), this, SLOT(windowMenuActivated(bool)));
        aAction->setChecked(mdiArea->activeSubWindow() == windows.at(i));
    }
}

/**
 * @brief MainWindow::windowMenuActivated
 * @param checked
 */
void
MainWindow::windowMenuActivated(bool checked)
{
    qDebug("MainWindow::windowMenuActivated()");
    QAction* aSender = qobject_cast<QAction*>(sender());
    if(!aSender)
        return;
    QWidget* w = mdiArea->subWindowList().at(aSender->data().toInt());
    if(w && checked)
        w->setFocus();
}

/**
 * @brief MainWindow::getMdiArea
 * @return
 */
MdiArea*
MainWindow::getMdiArea()
{
    qDebug("MainWindow::getMdiArea()");
    return mdiArea;
}

/**
 * @brief MainWindow::getApplication
 * @return
 */
MainWindow*
MainWindow::getApplication()
{
    qDebug("MainWindow::getApplication()");
    return mainWin;
}

/**
 * @brief MainWindow::newFile
 */
void
MainWindow::newFile()
{
    qDebug("MainWindow::newFile()");
    docIndex++;
    numOfDocs++;
    MdiWindow* mdiWin = new MdiWindow(docIndex, mainWin, mdiArea, Qt::SubWindow);
    connect(mdiWin, SIGNAL(sendCloseMdiWin(MdiWindow*)), this, SLOT(onCloseMdiWin(MdiWindow*)));
    connect(mdiArea, SIGNAL(subWindowActivated(QMdiSubWindow*)), this, SLOT(onWindowActivated(QMdiSubWindow*)));

    updateMenuToolbarStatusbar();
    windowMenuAboutToShow();

    View* v = mdiWin->getView();
    if (v) {
        v->recalculateLimits();
        v->zoomExtents();
    }
}

/**
 * @brief MainWindow::openFile
 * @param recent
 * @param recentFile
 */
void
MainWindow::openFile(bool recent, const QString& recentFile)
{
    qDebug("MainWindow::openFile()");

    QApplication::setOverrideCursor(Qt::ArrowCursor);

    QStringList files;
    bool preview = getSettingsOpenThumbnail();
    openFilesPath = settings_opensave_recent_directory;

    //Check to see if this from the recent files list
    if(recent)
    {
        files.append(recentFile);
        openFilesSelected(files);
    }
    else if(!preview)
    {
        //TODO: set getOpenFileNames' selectedFilter parameter from settings_opensave_open_format
        files = QFileDialog::getOpenFileNames(this, tr("Open"), openFilesPath, formatFilterOpen);
        openFilesSelected(files);
    }
    else if(preview)
    {
        PreviewDialog* openDialog = new PreviewDialog(this, tr("Open w/Preview"), openFilesPath, formatFilterOpen);
        //TODO: set openDialog->selectNameFilter(const QString& filter) from settings_opensave_open_format
        connect(openDialog, SIGNAL(filesSelected(const QStringList&)), this, SLOT(openFilesSelected(const QStringList&)));
        openDialog->exec();
    }

    QApplication::restoreOverrideCursor();
}

/**
 * @brief MainWindow::openFilesSelected
 * @param filesToOpen
 */
void
MainWindow::openFilesSelected(const QStringList& filesToOpen)
{
    bool doOnce = true;

    if (filesToOpen.count()) {
        for (int i = 0; i < filesToOpen.count(); i++) {
            if (!validFileFormat(filesToOpen[i]))
                continue;

            QMdiSubWindow* existing = findMdiWindow(filesToOpen[i]);
            if (existing) {
                mdiArea->setActiveSubWindow(existing);
                continue;
            }

            //The docIndex doesn't need increased as it is only used for unnamed files
            numOfDocs++;
            MdiWindow* mdiWin = new MdiWindow(docIndex, mainWin, mdiArea, Qt::SubWindow);
            connect(mdiWin, SIGNAL(sendCloseMdiWin(MdiWindow*)), this, SLOT(onCloseMdiWin(MdiWindow*)));
            connect(mdiArea, SIGNAL(subWindowActivated(QMdiSubWindow*)), this, SLOT(onWindowActivated(QMdiSubWindow*)));

            //Make sure the toolbars/etc... are shown before doing their zoomExtents
            if(doOnce) { updateMenuToolbarStatusbar(); doOnce = false; }

            if(mdiWin->loadFile(filesToOpen.at(i)))
            {
                statusbar->showMessage(tr("File(s) loaded"), 2000);
                mdiWin->show();
                mdiWin->showMaximized();
                //Prevent duplicate entries in the recent files list
                if(!settings_opensave_recent_list_of_files.contains(filesToOpen.at(i), Qt::CaseInsensitive))
                {
                    settings_opensave_recent_list_of_files.prepend(filesToOpen.at(i));
                }
                //Move the recent file to the top of the list
                else
                {
                    settings_opensave_recent_list_of_files.removeAll(filesToOpen.at(i));
                    settings_opensave_recent_list_of_files.prepend(filesToOpen.at(i));
                }
                settings_opensave_recent_directory = QFileInfo(filesToOpen.at(i)).absolutePath();

                View* v = mdiWin->getView();
                if(v)
                {
                    v->recalculateLimits();
                    v->zoomExtents();
                }
            }
            else
            {
                mdiWin->close();
            }
        }
    }

    windowMenuAboutToShow();
}

/**
 * @brief MainWindow::openrecentfile
 */
void
MainWindow::openrecentfile()
{
    qDebug("MainWindow::openrecentfile()");

    //Check to see if this from the recent files list
    QAction* recentSender = qobject_cast<QAction*>(sender());
    if (recentSender) {
        openFile(true, recentSender->data().toString());
    }
}

/**
 * @brief MainWindow::savefile
 */
void
MainWindow::savefile()
{
    qDebug("MainWindow::savefile()");
}

/**
 * @brief MainWindow::saveasfile
 */
void
MainWindow::saveasfile()
{
    qDebug("MainWindow::saveasfile()");
    // need to find the activeSubWindow before it loses focus to the FileDialog
    MdiWindow* mdiWin = qobject_cast<MdiWindow*>(mdiArea->activeSubWindow());
    if(!mdiWin)
        return;

    QString file;
    openFilesPath = settings_opensave_recent_directory;
    file = QFileDialog::getSaveFileName(this, tr("Save As"), openFilesPath, formatFilterSave);

    mdiWin->saveFile(file);
}

/**
 * @brief MainWindow::findMdiWindow
 * @param fileName
 * @return
 */
QMdiSubWindow* MainWindow::findMdiWindow(const QString& fileName)
{
    qDebug("MainWindow::findMdiWindow(%s)", qPrintable(fileName));
    QString canonicalFilePath = QFileInfo(fileName).canonicalFilePath();

    foreach(QMdiSubWindow* subWindow, mdiArea->subWindowList())
    {
        MdiWindow* mdiWin = qobject_cast<MdiWindow*>(subWindow);
        if(mdiWin)
        {
            if(mdiWin->getCurrentFile() == canonicalFilePath)
            {
                return subWindow;
            }
        }
    }
    return 0;
}

/**
 * @brief MainWindow::closeEvent
 * @param event
 */
void MainWindow::closeEvent(QCloseEvent* event)
{
    mdiArea->closeAllSubWindows();
    writeSettings();
    event->accept();
}

/**
 * @brief MainWindow::onCloseWindow
 */
void MainWindow::onCloseWindow()
{
    qDebug("MainWindow::onCloseWindow()");
    MdiWindow* mdiWin = qobject_cast<MdiWindow*>(mdiArea->activeSubWindow());
    if (mdiWin) {
        onCloseMdiWin(mdiWin);
    }
}

/**
 * @brief MainWindow::onCloseMdiWin
 * @param theMdiWin
 */
void
MainWindow::onCloseMdiWin(MdiWindow* theMdiWin)
{
    qDebug("MainWindow::onCloseMdiWin()");
    numOfDocs--;

    bool keepMaximized;
    if(theMdiWin) { keepMaximized = theMdiWin->isMaximized(); }

    mdiArea->removeSubWindow(theMdiWin);
    theMdiWin->deleteLater();

    updateMenuToolbarStatusbar();
    windowMenuAboutToShow();

    if (keepMaximized) {
        MdiWindow* mdiWin = qobject_cast<MdiWindow*>(mdiArea->activeSubWindow());
        if(mdiWin) { mdiWin->showMaximized(); }
    }
}

/**
 * @brief MainWindow::onWindowActivated
 * @param w
 */
void
MainWindow::onWindowActivated(QMdiSubWindow* w)
{
    qDebug("MainWindow::onWindowActivated()");
    MdiWindow* mdiWin = qobject_cast<MdiWindow*>(w);
    if (mdiWin) {
        mdiWin->onWindowActivated();
    }
}

/**
 * @brief MainWindow::resizeEvent
 * @param e
 */
void MainWindow::resizeEvent(QResizeEvent* e)
{
    qDebug("MainWindow::resizeEvent()");
    QMainWindow::resizeEvent(e);
    statusBar()->setSizeGripEnabled(!isMaximized());
}

/**
 * @brief MainWindow::getFileSeparator
 * @return
 */
QAction*
MainWindow::getFileSeparator()
{
    qDebug("MainWindow::getFileSeparator()");
    return myFileSeparator;
}

/**
 * @brief MainWindow::updateMenuToolbarStatusbar
 */
void
MainWindow::updateMenuToolbarStatusbar()
{
    qDebug("MainWindow::updateMenuToolbarStatusbar()");

    actionHash.value(ACTION_print)->setEnabled(numOfDocs > 0);
    actionHash.value(ACTION_windowclose)->setEnabled(numOfDocs > 0);
    actionHash.value(ACTION_designdetails)->setEnabled(numOfDocs > 0);

    if (numOfDocs) {
        //Toolbars
        toolbarView->show();
        toolbarZoom->show();
        toolbarPan->show();
        toolbarIcon->show();
        toolbarHelp->show();
        toolbarLayer->show();
        toolbarText->show();
        toolbarProperties->show();
        toolbarPrompt->show();

        foreach (QToolBar* tb, toolbarHash) {
            tb->show();
        }

        //DockWidgets
        dockPropEdit->show();
        dockUndoEdit->show();

        //Menus
        menuBar()->clear();
        menuBar()->addMenu(fileMenu);
        menuBar()->addMenu(editMenu);
        menuBar()->addMenu(viewMenu);

        foreach (QMenu* menu, menuHash) {
            menuBar()->addMenu(menu);
        }

        menuBar()->addMenu(settingsMenu);
        menuBar()->addMenu(windowMenu);
        menuBar()->addMenu(helpMenu);

        windowMenu->setEnabled(true);

        //Statusbar
        statusbar->clearMessage();
        statusbar->statusBarMouseCoord->show();
        statusbar->statusBarSnapButton->show();
        statusbar->statusBarGridButton->show();
        statusbar->statusBarRulerButton->show();
        statusbar->statusBarOrthoButton->show();
        statusbar->statusBarPolarButton->show();
        statusbar->statusBarQSnapButton->show();
        statusbar->statusBarQTrackButton->show();
        statusbar->statusBarLwtButton->show();
    }
    else {
        //Toolbars
        toolbarView->hide();
        toolbarZoom->hide();
        toolbarPan->hide();
        toolbarIcon->hide();
        toolbarHelp->hide();
        toolbarLayer->hide();
        toolbarText->hide();
        toolbarProperties->hide();
        toolbarPrompt->hide();
        foreach (QToolBar* tb, toolbarHash) {
            tb->hide();
        }

        //DockWidgets
        dockPropEdit->hide();
        dockUndoEdit->hide();

        //Menus
        menuBar()->clear();
        menuBar()->addMenu(fileMenu);
        menuBar()->addMenu(editMenu);
        menuBar()->addMenu(settingsMenu);
        menuBar()->addMenu(windowMenu);
        menuBar()->addMenu(helpMenu);

        windowMenu->setEnabled(false);

        //Statusbar
        statusbar->clearMessage();
        statusbar->statusBarMouseCoord->hide();
        statusbar->statusBarSnapButton->hide();
        statusbar->statusBarGridButton->hide();
        statusbar->statusBarRulerButton->hide();
        statusbar->statusBarOrthoButton->hide();
        statusbar->statusBarPolarButton->hide();
        statusbar->statusBarQSnapButton->hide();
        statusbar->statusBarQTrackButton->hide();
        statusbar->statusBarLwtButton->hide();
    }
    hideUnimplemented();
}

/**
 * @brief MainWindow::hideUnimplemented
 */
void
MainWindow::hideUnimplemented()
{
    qDebug("MainWindow::hideUnimplemented()");
}

/**
 * @brief MainWindow::validFileFormat
 * @param fileName
 * @return
 */
bool
MainWindow::validFileFormat(const QString& fileName)
{
    if (emb_identify_format(qPrintable(fileName)) >= 0) {
        return true;
    }
    return false;
}

/**
 * @brief MainWindow::loadFormats
 */
void
MainWindow::loadFormats()
{
    char stable, unstable;
    QString supportedReaders  = "All Supported Files (";
    QString individualReaders = "All Files (*);;";
    QString supportedWriters  = "All Supported Files (";
    QString individualWriters = "All Files (*);;";
    QString supportedStr;
    QString individualStr;

    //TODO: Stable Only (Settings Option)
    //stable = 'S'; unstable = 'S';

    //Stable + Unstable
    stable = 'S'; unstable = 'U';

    const char* extension = 0;
    const char* description = 0;
    char readerState;
    char writerState;

    for (int i=0; i<numberOfFormats; i++) {
        extension = formatTable[i].extension;
        description = formatTable[i].description;
        readerState = formatTable[i].reader_state;
        writerState = formatTable[i].writer_state;

        QString upperExt = QString(extension).toUpper();
        supportedStr = "*" + upperExt + " ";
        individualStr = upperExt.replace(".", "") + " - " + description + " (*" + extension + ");;";
        if(readerState == stable || readerState == unstable)
        {
            //Exclude color file formats from open dialogs
            if(upperExt != "COL" && upperExt != "EDR" && upperExt != "INF" && upperExt != "RGB")
            {
                supportedReaders.append(supportedStr);
                individualReaders.append(individualStr);
            }
        }
        if(writerState == stable || writerState == unstable)
        {
            supportedWriters.append(supportedStr);
            individualWriters.append(individualStr);
        }
    }

    supportedReaders.append(");;");
    supportedWriters.append(");;");

    formatFilterOpen = supportedReaders + individualReaders;
    formatFilterSave = supportedWriters + individualWriters;

    //TODO: Fixup custom filter
    /*
    QString custom = getSettingsCustomFilter();
    if(custom.contains("supported", Qt::CaseInsensitive))
        custom = ""; //This will hide it
    else if(!custom.contains("*", Qt::CaseInsensitive))
        custom = ""; //This will hide it
    else
        custom = "Custom Filter(" + custom + ");;";

    return tr(qPrintable(custom + supported + all));
    */
}

/**
 * @brief MainWindow::closeToolBar
 * @param action
 */
void
MainWindow::closeToolBar(QAction* action)
{
    if (action->objectName() == "toolbarclose") {
        QToolBar* tb = qobject_cast<QToolBar*>(sender());
        if (tb) {
            qDebug("%s closed.", qPrintable(tb->objectName()));
            tb->hide();
        }
    }
}

/**
 * @brief MainWindow::floatingChangedToolBar
 * @param isFloating
 */
void
MainWindow::floatingChangedToolBar(bool isFloating)
{
    QToolBar* tb = qobject_cast<QToolBar*>(sender());
    if (tb) {
        if (isFloating) {
            /*
            //TODO: Determine best suited close button on various platforms.
            QStyle::SP_DockWidgetCloseButton
            QStyle::SP_TitleBarCloseButton
            QStyle::SP_DialogCloseButton
            */
            QAction *ACTION = new QAction(tb->style()->standardIcon(QStyle::SP_DialogCloseButton), "Close", this);
            ACTION->setStatusTip("Close the " + tb->windowTitle() + " Toolbar");
            ACTION->setObjectName("toolbarclose");
            tb->addAction(ACTION);
            connect(tb, SIGNAL(actionTriggered(QAction*)), this, SLOT(closeToolBar(QAction*)));
        }
        else {
            QList<QAction*> actList = tb->actions();
            for(int i = 0; i < actList.size(); ++i)
            {
                QAction* ACTION = actList.value(i);
                if(ACTION->objectName() == "toolbarclose")
                {
                    tb->removeAction(ACTION);
                    disconnect(tb, SIGNAL(actionTriggered(QAction*)), this, SLOT(closeToolBar(QAction*)));
                    delete ACTION;
                }
            }
        }
    }
}
