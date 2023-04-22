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
    if (!check.exists())
        QMessageBox::critical(this, tr("Path Error"), tr("Cannot locate: ") + check.absoluteFilePath());
    check = QFileInfo(appDir + "/help");
    if (!check.exists())
        QMessageBox::critical(this, tr("Path Error"), tr("Cannot locate: ") + check.absoluteFilePath());
    check = QFileInfo(appDir + "/icons");
    if (!check.exists())
        QMessageBox::critical(this, tr("Path Error"), tr("Cannot locate: ") + check.absoluteFilePath());
    check = QFileInfo(appDir + "/images");
    if (!check.exists())
        QMessageBox::critical(this, tr("Path Error"), tr("Cannot locate: ") + check.absoluteFilePath());
    check = QFileInfo(appDir + "/samples");
    if (!check.exists())
        QMessageBox::critical(this, tr("Path Error"), tr("Cannot locate: ") + check.absoluteFilePath());
    check = QFileInfo(appDir + "/translations");
    if (!check.exists())
        QMessageBox::critical(this, tr("Path Error"), tr("Cannot locate: ") + check.absoluteFilePath());

    QString lang = getSettingsGeneralLanguage();
    qDebug("language: %s", qPrintable(lang));
    if (lang == "system")
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

    //script
    _mainWin = this;

    /*
    engine = new QScriptEngine(this);
    engine->installTranslatorFunctions();
    debugger = new QScriptEngineDebugger(this);
    debugger->attachTo(engine);
    InitNatives(engine);

    //Load all commands in a loop
    QDir commandDir(appDir + "/commands");
    QStringList cmdList = commandDir.entryList(QDir::Dirs | QDir::NoDotAndDotDot);
    foreach(QString cmdName, cmdList)
    {
        LoadCommand(cmdName);
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
    if (tipFile.open(QFile::ReadOnly))
    {
        QTextStream stream(&tipFile);
        QString tipLine;
        do
        {
            tipLine = stream.readLine();
            if (!tipLine.isEmpty())
                listTipOfTheDay << tipLine;
        }
        while(!tipLine.isNull());
    }
    if (getSettingsGeneralTipOfTheDay())
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
/*
    if (cmd == "debug") scriptValDebug();
    if (cmd == "error") scriptValError();
    "todo", scriptValTodo();
    "alert", scriptValAlert();
    "blinkPrompt", scriptValBlinkPrompt);
    "setPromptPrefix", scriptValSetPromptPrefix);
    "appendPromptHistory", scriptValAppendPromptHistory);
    "enablePromptRapidFire", scriptValEnablePromptRapidFire);
    "disablePromptRapidFire", scriptValDisablePromptRapidFire);
    "enableMoveRapidFire", scriptValEnableMoveRapidFire);
    "disableMoveRapidFire", scriptValDisableMoveRapidFire);
    "initCommand", scriptValInitCommand);
    "endCommand", scriptValEndCommand);
    "newFile", scriptValNewFile);
    "openFile", scriptValOpenFile);
    "exit", scriptValExit);
    "help", scriptValHelp);
    "about", scriptValAbout);
    "tipOfTheDay", scriptValTipOfTheDay);
    "windowCascade", scriptValWindowCascade);
    "windowTile", scriptValWindowTile);
    "windowClose", scriptValWindowClose);
    "windowCloseAll", scriptValWindowCloseAll);
    "windowNext", scriptValWindowNext);
    "windowPrevious", scriptValWindowPrevious);
    "platformString", scriptValPlatformString);
    "messageBox", scriptValMessageBox);
    "isInt", scriptValIsInt);
    "undo", scriptValUndo);
    "redo", scriptValRedo);
    "icon16", scriptValIcon16);
    "icon24", scriptValIcon24);
    "icon32", scriptValIcon32);
    "icon48", scriptValIcon48);
    "icon64", scriptValIcon64);
    "icon128", scriptValIcon128);
    "panLeft", scriptValPanLeft);
    "panRight", scriptValPanRight);
    "panUp", scriptValPanUp);
    "panDown", scriptValPanDown);
    "zoomIn", scriptValZoomIn);
    "zoomOut", scriptValZoomOut);
    "zoomExtents", scriptValZoomExtents);
    "printArea", scriptValPrintArea);
    "dayVision", scriptValDayVision);
    "nightVision", scriptValNightVision);
    "setBackgroundColor", scriptValSetBackgroundColor);
    "setCrossHairColor", scriptValSetCrossHairColor);
    "setGridColor", scriptValSetGridColor);
    "textFont", scriptValTextFont);
    "textSize", scriptValTextSize);
    "textAngle", scriptValTextAngle);
    "textBold", scriptValTextBold);
    "textItalic", scriptValTextItalic);
    "textUnderline", scriptValTextUnderline);
    "textStrikeOut", scriptValTextStrikeOut);
    "textOverline", scriptValTextOverline);
    "setTextFont", scriptValSetTextFont);
    "setTextSize", scriptValSetTextSize);
    "setTextAngle", scriptValSetTextAngle);
    "setTextBold", scriptValSetTextBold);
    "setTextItalic", scriptValSetTextItalic);
    "setTextUnderline", scriptValSetTextUnderline);
    "setTextStrikeOut", scriptValSetTextStrikeOut);
    "setTextOverline", scriptValSetTextOverline);
    "previewOn", scriptValPreviewOn);
    "previewOff", scriptValPreviewOff);
    "vulcanize", scriptValVulcanize);
    "allowRubber", scriptValAllowRubber);
    "setRubberMode", scriptValSetRubberMode);
    "setRubberPoint", scriptValSetRubberPoint);
    "setRubberText", scriptValSetRubberText);
    "addRubber", scriptValAddRubber);
    "clearRubber", scriptValClearRubber);
    "spareRubber", scriptValSpareRubber);
    "addTextMulti", scriptValAddTextMulti);
    "addTextSingle", scriptValAddTextSingle);
    "addInfiniteLine", scriptValAddInfiniteLine);
    "addRay", scriptValAddRay);
    "addLine", scriptValAddLine);
    "addTriangle", scriptValAddTriangle);
    "addRectangle", scriptValAddRectangle);
    "addRoundedRectangle", scriptValAddRoundedRectangle);
    "addArc", scriptValAddArc);
    "addCircle", scriptValAddCircle);
    "addEllipse", scriptValAddEllipse);
    "addPoint", scriptValAddPoint);
    "addRegularPolygon", scriptValAddRegularPolygon);
    "addPolygon", scriptValAddPolygon);
    "addPolyline", scriptValAddPolyline);
    "addPath", scriptValAddPath);
    "addHorizontalDimension", scriptValAddHorizontalDimension);
    "addVerticalDimension", scriptValAddVerticalDimension);
    "addImage", scriptValAddImage);
    "addDimLeader", scriptValAddDimLeader);
    "setCursorShape", scriptValSetCursorShape);
    "calculateAngle", scriptValCalculateAngle);
    if (cmd == "calculateDistance", scriptValCalculateDistance);
    cmd == "perpendicularDistance", scriptValPerpendicularDistance();
    cmd == "numSelected", scriptValNumSelected);
    cmd == "selectAll", scriptValSelectAll);
    if (cmd == "addToSelection", scriptValAddToSelection();
    if (cmd == "clearSelection") scriptValClearSelection();
    if (cmd == "deleteSelected") scriptValDeleteSelected();
    if (cmd == "cutSelected") scriptValCutSelected);
    if (cmd == "copySelected") scriptValCopySelected);
    if (cmd == "pasteSelected") scriptValPasteSelected);
    if (cmd == "moveSelected", scriptValMoveSelected);
    if (cmd == "scaleSelected", scriptValScaleSelected);
    if (cmd == "rotateSelected", scriptValRotateSelected);
    if (cmd == "mirrorSelected", scriptValMirrorSelected);
    "qsnapX", scriptValQSnapX);
    "qsnapY", scriptValQSnapY);
    "mouseX", scriptValMouseX);
    "mouseY", scriptValMouseY);

    std::string scriptValInclude = engine->newFunction(Include);
    "include", scriptValInclude);
*/
    return output;
}

#if 0
void MainWindow::LoadCommand(EmbString cmdName)
{
    qDebug("LoadCommand(%s)", qPrintable(cmdName));
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
                if (validBeforeChars.contains(charBefore))
                {
                    int i = 0;
                    QChar charAfter;
                    do {
                        charAfter = script.at(index + i + findFunc.length());
                        if (charAfter == '(')
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

    if (toolbarName.toUpper() != "NONE")
    {
        //If the toolbar doesn't exist, create it.
        if (!toolbarHash.value(toolbarName))
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

    if (menuName.toUpper() != "NONE")
    {
        //If the menu doesn't exist, create it.
        if (!menuHash.value(menuName))
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

/**
 * \brief
 */
std::string
Include(std::vector<std::string> args, QScriptEngine* engine)
{
    EmbString fileName = args(0).toString();
    QFile scriptFile("commands/" + fileName);

    if (!scriptFile.open(QIODevice::ReadOnly))
        return -1;

    QTextStream stream(&scriptFile);
    EmbString s=stream.readAll();
    scriptFile.close();

    QScriptContext* parent=context->parentContext();

    if (parent!=0)
    {
        context->setActivationObject(context->parentContext()->activationObject());
        context->setThisObject(context->parentContext()->thisObject());
    }

    std::string result = engine->evaluate(s);

    return 0;
}

std::string
Debug(std::vector<std::string> args)
{
    if (argsCount() != 1)    return context->throwError("debug() requires one argument");
    if (!args(0).isString()) return context->throwError(QScriptContext::TypeError, "debug(): first argument is not a string");

    qDebug("%s", qPrintable(args(0).toString()));
    return "";
}

std::string
Error(std::vector<std::string> args)
{
    if (argsCount() != 2)    return context->throwError("error() requires two arguments");
    if (!args(0).isString()) return context->throwError(QScriptContext::TypeError, "error(): first argument is not a string");
    if (!args(1).isString()) return context->throwError(QScriptContext::TypeError, "error(): second argument is not a string");

    EmbString strCmd = args(0).toString();
    EmbString strErr = args(1).toString();

    mainWin()->nativeSetPromptPrefix("ERROR: (" + strCmd + ") " + strErr);
    mainWin()->nativeAppendPromptHistory(QString());
    mainWin()->nativeEndCommand();
    return "";
}

std::string
Todo(std::vector<std::string> args)
{
    if (argsCount() != 2)    return context->throwError("todo() requires two arguments");
    if (!args(0).isString()) return context->throwError(QScriptContext::TypeError, "todo(): first argument is not a string");
    if (!args(1).isString()) return context->throwError(QScriptContext::TypeError, "todo(): second argument is not a string");

    EmbString strCmd  = args(0).toString();
    EmbString strTodo = args(1).toString();

    mainWin()->nativeAlert("TODO: (" + strCmd + ") " + strTodo);
    mainWin()->nativeEndCommand();
    return "";
}

std::string
Alert(std::vector<std::string> args)
{
    if (argsCount() != 1)    return context->throwError("alert() requires one argument");
    if (!args(0).isString()) return context->throwError(QScriptContext::TypeError, "alert(): first argument is not a string");

    mainWin()->nativeAlert(args(0).toString());
    return "";
}

std::string
BlinkPrompt(std::vector<std::string> args)
{
    if (argsCount() != 0) return context->throwError("blinkPrompt() requires zero arguments");

    mainWin()->nativeBlinkPrompt();
    return "";
}

std::string
SetPromptPrefix(std::vector<std::string> args)
{
    if (argsCount() != 1)    return context->throwError("setPromptPrefix() requires one argument");
    if (!args(0).isString()) return context->throwError(QScriptContext::TypeError, "setPromptPrefix(): first argument is not a string");

    mainWin()->nativeSetPromptPrefix(args(0).toString());
    return "";
}

/**
 *
 */
std::string
AppendPromptHistory(std::vector<std::string> args)
{
    int args = argsCount();
    if (args == 0)
    {
        mainWin()->nativeAppendPromptHistory(QString());
    }
    else if (args == 1)
    {
        mainWin()->nativeAppendPromptHistory(args(0).toString());
    }
    else
    {
        return context->throwError("appendPromptHistory() requires one or zero arguments");
    }
    return "";
}

/**
 * @brief EnablePromptRapidFire
 * @param context
 * @return
 */
std::string
EnablePromptRapidFire(std::vector<std::string> args)
{
    if (argsCount() != 0) return context->throwError("enablePromptRapidFire() requires zero arguments");

    mainWin()->nativeEnablePromptRapidFire();
    return "";
}

/**
 * @brief DisablePromptRapidFire
 * @param context
 * @return
 */
std::string
DisablePromptRapidFire(std::vector<std::string> args)
{
    if (argsCount() != 0) return context->throwError("disablePromptRapidFire() requires zero arguments");

    mainWin()->nativeDisablePromptRapidFire();
    return "";
}

std::string
EnableMoveRapidFire(std::vector<std::string> args)
{
    if (argsCount() != 0) return context->throwError("enableMoveRapidFire() requires zero arguments");

    mainWin()->nativeEnableMoveRapidFire();
    return "";
}

std::string
DisableMoveRapidFire(std::vector<std::string> args)
{
    if (argsCount() != 0) return context->throwError("disableMoveRapidFire() requires zero arguments");

    mainWin()->nativeDisableMoveRapidFire();
    return "";
}

std::string
InitCommand(std::vector<std::string> args)
{
    if (argsCount() != 0) return context->throwError("initCommand() requires zero arguments");

    mainWin()->nativeInitCommand();
    return "";
}

/**
 * @brief EndCommand
 * @param context
 * @return
 */
std::string
EndCommand(std::vector<std::string> args)
{
    if (argsCount() != 0) return context->throwError("endCommand() requires zero arguments");

    mainWin()->nativeEndCommand();
    return "";
}

/**
 * @brief NewFile
 * @param context
 * @return
 */
std::string
NewFile(std::vector<std::string> args)
{
    if (argsCount() != 0) return context->throwError("newFile() requires zero arguments");

    mainWin()->nativeNewFile();
    return "";
}

/**
 * @brief OpenFile
 * @param context
 * @return
 */
std::string
OpenFile(std::vector<std::string> args)
{
    if (argsCount() != 0) {
        return context->throwError("openFile() requires zero arguments");
    }

    mainWin()->nativeOpenFile();
    return "";
}

/**
 * @brief Exit
 * @param context
 * @return
 */
std::string
Exit(std::vector<std::string> args)
{
    if (argsCount() != 0) return context->throwError("exit() requires zero arguments");

    mainWin()->nativeExit();
    return "";
}

std::string
Help(std::vector<std::string> args)
{
    if (argsCount() != 0) return context->throwError("help() requires zero arguments");

    mainWin()->nativeHelp();
    return "";
}

std::string
About(std::vector<std::string> args)
{
    if (argsCount() != 0) return context->throwError("about() requires zero arguments");

    mainWin()->nativeAbout();
    return "";
}

std::string
TipOfTheDay(std::vector<std::string> args)
{
    if (argsCount() != 0) return context->throwError("tipOfTheDay() requires zero arguments");

    mainWin()->nativeTipOfTheDay();
    return "";
}

std::string
WindowCascade(std::vector<std::string> args)
{
    if (argsCount() != 0) return context->throwError("windowCascade() requires zero arguments");

    mainWin()->nativeWindowCascade();
    return "";
}

std::string
WindowTile(std::vector<std::string> args)
{
    if (argsCount() != 0) return context->throwError("windowTile() requires zero arguments");

    mainWin()->nativeWindowTile();
    return "";
}

std::string
WindowClose(std::vector<std::string> args)
{
    if (argsCount() != 0) return context->throwError("windowClose() requires zero arguments");

    mainWin()->nativeWindowClose();
    return "";
}

std::string
WindowCloseAll(std::vector<std::string> args)
{
    if (argsCount() != 0) return context->throwError("windowCloseAll() requires zero arguments");

    mainWin()->nativeWindowCloseAll();
    return "";
}

std::string
WindowNext(std::vector<std::string> args)
{
    if (argsCount() != 0) return context->throwError("windowNext() requires zero arguments");

    mainWin()->nativeWindowNext();
    return "";
}

std::string
WindowPrevious(std::vector<std::string> args)
{
    if (argsCount() != 0) return context->throwError("windowPrevious() requires zero arguments");

    mainWin()->nativeWindowPrevious();
    return "";
}

std::string
PlatformString(std::vector<std::string> args)
{
    if (argsCount() != 0) return context->throwError("platformString() requires zero arguments");
    return std::string(mainWin()->nativePlatformString());
}

std::string
MessageBox(std::vector<std::string> args)
{
    if (argsCount() != 3)    return context->throwError("messageBox() requires three arguments");
    if (!args(0).isString()) return context->throwError(QScriptContext::TypeError, "messageBox(): first argument is not a string");
    if (!args(1).isString()) return context->throwError(QScriptContext::TypeError, "messageBox(): second argument is not a string");
    if (!args(2).isString()) return context->throwError(QScriptContext::TypeError, "messageBox(): third argument is not a string");

    EmbString type  = args(0).toString().toLower();
    EmbString title = args(1).toString();
    EmbString text  = args(2).toString();

    if (type != "critical" && type != "information" && type != "question" && type != "warning")
        return context->throwError(QScriptContext::UnknownError, "messageBox(): first argument must be \"critical\", \"information\", \"question\" or \"warning\".");

    mainWin()->nativeMessageBox(type, title, text);
    return "";
}

std::string
IsInt(std::vector<std::string> args)
{
    if (argsCount() != 1)    return context->throwError("isInt() requires one argument");
    if (!args(0).isNumber()) return context->throwError(QScriptContext::TypeError, "isInt(): first argument is not a number");

    EmbReal num = args(0).toNumber();

    //isNaN check
    if (std::isnan(num)) return context->throwError(QScriptContext::TypeError, "isInt(): first argument failed isNaN check. There is an error in your code.");

    if (fmod(num, 1) == 0)
        return std::string(true);
    return std::string(false);
}

std::string
Undo(std::vector<std::string> args)
{
    if (argsCount() != 0) return context->throwError("undo() requires zero arguments");

    mainWin()->nativeUndo();
    return "";
}

std::string
Redo(std::vector<std::string> args)
{
    if (argsCount() != 0) return context->throwError("redo() requires zero arguments");

    mainWin()->nativeRedo();
    return "";
}

std::string
Icon16(std::vector<std::string> args)
{
    if (argsCount() != 0) return context->throwError("icon16() requires zero arguments");

    mainWin()->nativeIcon16();
    return "";
}

std::string
Icon24(std::vector<std::string> args)
{
    if (argsCount() != 0) return context->throwError("icon24() requires zero arguments");

    mainWin()->nativeIcon24();
    return "";
}

std::string
Icon32(std::vector<std::string> args)
{
    if (argsCount() != 0) return context->throwError("icon32() requires zero arguments");

    mainWin()->nativeIcon32();
    return "";
}

std::string
Icon48(std::vector<std::string> args)
{
    if (argsCount() != 0) return context->throwError("icon48() requires zero arguments");

    mainWin()->nativeIcon48();
    return "";
}

std::string
Icon64(std::vector<std::string> args)
{
    if (argsCount() != 0) return context->throwError("icon64() requires zero arguments");

    mainWin()->nativeIcon64();
    return "";
}

std::string
Icon128(std::vector<std::string> args)
{
    if (argsCount() != 0) return context->throwError("icon128() requires zero arguments");

    mainWin()->nativeIcon128();
    return "";
}

std::string
PanLeft(std::vector<std::string> args)
{
    if (argsCount() != 0) return context->throwError("panLeft() requires zero arguments");

    mainWin()->nativePanLeft();
    return "";
}

std::string
PanRight(std::vector<std::string> args)
{
    if (argsCount() != 0) return context->throwError("panRight() requires zero arguments");

    mainWin()->nativePanRight();
    return "";
}

std::string
PanUp(std::vector<std::string> args)
{
    if (argsCount() != 0) return context->throwError("panUp() requires zero arguments");

    mainWin()->nativePanUp();
    return "";
}

std::string
PanDown(std::vector<std::string> args)
{
    if (argsCount() != 0) return context->throwError("panDown() requires zero arguments");

    mainWin()->nativePanDown();
    return "";
}

std::string
ZoomIn(std::vector<std::string> args)
{
    if (argsCount() != 0) return context->throwError("zoomIn() requires zero arguments");

    mainWin()->nativeZoomIn();
    return "";
}

std::string
ZoomOut(std::vector<std::string> args)
{
    if (argsCount() != 0) return context->throwError("zoomOut() requires zero arguments");

    mainWin()->nativeZoomOut();
    return "";
}

std::string
ZoomExtents(std::vector<std::string> args)
{
    if (argsCount() != 0) return context->throwError("zoomExtents() requires zero arguments");

    mainWin()->nativeZoomExtents();
    return "";
}

std::string
PrintArea(std::vector<std::string> args)
{
    if (argsCount() != 4)    return context->throwError("printArea() requires four arguments");
    if (!args(0).isNumber()) return context->throwError(QScriptContext::TypeError, "printArea(): first argument is not a number");
    if (!args(1).isNumber()) return context->throwError(QScriptContext::TypeError, "printArea(): second argument is not a number");
    if (!args(2).isNumber()) return context->throwError(QScriptContext::TypeError, "printArea(): third argument is not a number");
    if (!args(3).isNumber()) return context->throwError(QScriptContext::TypeError, "printArea(): fourth argument is not a number");

    EmbReal x = args(0).toNumber();
    EmbReal y = args(1).toNumber();
    EmbReal w = args(2).toNumber();
    EmbReal h = args(3).toNumber();

    //isNaN check
    if (std::isnan(x)) return context->throwError(QScriptContext::TypeError, "printArea(): first argument failed isNaN check. There is an error in your code.");
    if (std::isnan(y)) return context->throwError(QScriptContext::TypeError, "printArea(): second argument failed isNaN check. There is an error in your code.");
    if (std::isnan(w)) return context->throwError(QScriptContext::TypeError, "printArea(): third argument failed isNaN check. There is an error in your code.");
    if (std::isnan(h)) return context->throwError(QScriptContext::TypeError, "printArea(): fourth argument failed isNaN check. There is an error in your code.");

    mainWin()->nativePrintArea(x, y, w, h);
    return "";
}

std::string
DayVision(std::vector<std::string> args)
{
    if (argsCount() != 0) return context->throwError("dayVision() requires zero arguments");

    mainWin()->nativeDayVision();
    return "";
}

std::string
NightVision(std::vector<std::string> args)
{
    if (argsCount() != 0) return context->throwError("nightVision() requires zero arguments");

    mainWin()->nativeNightVision();
    return "";
}

std::string
SetBackgroundColor(std::vector<std::string> args)
{
    if (argsCount() != 3)    return context->throwError("setBackgroundColor() requires three arguments");
    if (!args(0).isNumber()) return context->throwError(QScriptContext::TypeError, "setBackgroundColor(): first argument is not a number");
    if (!args(1).isNumber()) return context->throwError(QScriptContext::TypeError, "setBackgroundColor(): second argument is not a number");
    if (!args(2).isNumber()) return context->throwError(QScriptContext::TypeError, "setBackgroundColor(): third argument is not a number");

    EmbReal r = args(0).toNumber();
    EmbReal g = args(1).toNumber();
    EmbReal b = args(2).toNumber();

    //isNaN check
    if (std::isnan(r)) return context->throwError(QScriptContext::TypeError, "setBackgroundColor(): first argument failed isNaN check. There is an error in your code.");
    if (std::isnan(g)) return context->throwError(QScriptContext::TypeError, "setBackgroundColor(): second argument failed isNaN check. There is an error in your code.");
    if (std::isnan(b)) return context->throwError(QScriptContext::TypeError, "setBackgroundColor(): third argument failed isNaN check. There is an error in your code.");

    if (r < 0 || r > 255) { return context->throwError(QScriptContext::UnknownError, "setBackgroundColor(): r value must be in range 0-255"); }
    if (g < 0 || g > 255) { return context->throwError(QScriptContext::UnknownError, "setBackgroundColor(): g value must be in range 0-255"); }
    if (b < 0 || b > 255) { return context->throwError(QScriptContext::UnknownError, "setBackgroundColor(): b value must be in range 0-255"); }

    mainWin()->nativeSetBackgroundColor(r, g, b);
    return "";
}

std::string
SetCrossHairColor(std::vector<std::string> args)
{
    if (argsCount() != 3)    return context->throwError("setCrossHairColor() requires three arguments");
    if (!args(0).isNumber()) return context->throwError(QScriptContext::TypeError, "setCrossHairColor(): first argument is not a number");
    if (!args(1).isNumber()) return context->throwError(QScriptContext::TypeError, "setCrossHairColor(): second argument is not a number");
    if (!args(2).isNumber()) return context->throwError(QScriptContext::TypeError, "setCrossHairColor(): third argument is not a number");

    EmbReal r = args(0).toNumber();
    EmbReal g = args(1).toNumber();
    EmbReal b = args(2).toNumber();

    //isNaN check
    if (std::isnan(r)) return context->throwError(QScriptContext::TypeError, "setCrossHairColor(): first argument failed isNaN check. There is an error in your code.");
    if (std::isnan(g)) return context->throwError(QScriptContext::TypeError, "setCrossHairColor(): second argument failed isNaN check. There is an error in your code.");
    if (std::isnan(b)) return context->throwError(QScriptContext::TypeError, "setCrossHairColor(): third argument failed isNaN check. There is an error in your code.");

    if (r < 0 || r > 255) { return context->throwError(QScriptContext::UnknownError, "setCrossHairColor(): r value must be in range 0-255"); }
    if (g < 0 || g > 255) { return context->throwError(QScriptContext::UnknownError, "setCrossHairColor(): g value must be in range 0-255"); }
    if (b < 0 || b > 255) { return context->throwError(QScriptContext::UnknownError, "setCrossHairColor(): b value must be in range 0-255"); }

    mainWin()->nativeSetCrossHairColor(r, g, b);
    return "";
}

std::string
SetGridColor(std::vector<std::string> args)
{
    if (argsCount() != 3)    return context->throwError("setGridColor() requires three arguments");
    if (!args(0).isNumber()) return context->throwError(QScriptContext::TypeError, "setGridColor(): first argument is not a number");
    if (!args(1).isNumber()) return context->throwError(QScriptContext::TypeError, "setGridColor(): second argument is not a number");
    if (!args(2).isNumber()) return context->throwError(QScriptContext::TypeError, "setGridColor(): third argument is not a number");

    EmbReal r = args(0).toNumber();
    EmbReal g = args(1).toNumber();
    EmbReal b = args(2).toNumber();

    //isNaN check
    if (std::isnan(r)) return context->throwError(QScriptContext::TypeError, "setGridColor(): first argument failed isNaN check. There is an error in your code.");
    if (std::isnan(g)) return context->throwError(QScriptContext::TypeError, "setGridColor(): second argument failed isNaN check. There is an error in your code.");
    if (std::isnan(b)) return context->throwError(QScriptContext::TypeError, "setGridColor(): third argument failed isNaN check. There is an error in your code.");

    if (r < 0 || r > 255) { return context->throwError(QScriptContext::UnknownError, "setGridColor(): r value must be in range 0-255"); }
    if (g < 0 || g > 255) { return context->throwError(QScriptContext::UnknownError, "setGridColor(): g value must be in range 0-255"); }
    if (b < 0 || b > 255) { return context->throwError(QScriptContext::UnknownError, "setGridColor(): b value must be in range 0-255"); }

    mainWin()->nativeSetGridColor(r, g, b);
    return "";
}

std::string
TextFont(std::vector<std::string> args)
{
    if (argsCount() != 0) return context->throwError("textFont() requires zero arguments");
    return std::string(mainWin()->nativeTextFont());
}

std::string
TextSize(std::vector<std::string> args)
{
    if (argsCount() != 0) return context->throwError("textSize() requires zero arguments");
    return std::string(mainWin()->nativeTextSize());
}

std::string
TextAngle(std::vector<std::string> args)
{
    if (argsCount() != 0) return context->throwError("textAngle() requires zero arguments");
    return std::string(mainWin()->nativeTextAngle());
}

std::string
TextBold(std::vector<std::string> args)
{
    if (argsCount() != 0) return context->throwError("textBold() requires zero arguments");
    return std::string(mainWin()->nativeTextBold());
}

std::string
TextItalic(std::vector<std::string> args)
{
    if (argsCount() != 0) return context->throwError("textItalic() requires zero arguments");
    return std::string(mainWin()->nativeTextItalic());
}

std::string
TextUnderline(std::vector<std::string> args)
{
    if (argsCount() != 0) return context->throwError("textUnderline() requires zero arguments");
    return std::string(mainWin()->nativeTextUnderline());
}

std::string
TextStrikeOut(std::vector<std::string> args)
{
    if (argsCount() != 0) return context->throwError("textStrikeOut() requires zero arguments");
    return std::string(mainWin()->nativeTextStrikeOut());
}

std::string
TextOverline(std::vector<std::string> args)
{
    if (argsCount() != 0) return context->throwError("textOverline() requires zero arguments");
    return std::string(mainWin()->nativeTextOverline());
}

std::string
SetTextFont(std::vector<std::string> args)
{
    if (argsCount() != 1)    return context->throwError("setTextFont() requires one argument");
    if (!args(0).isString()) return context->throwError(QScriptContext::TypeError, "setTextFont(): first argument is not a string");

    mainWin()->nativeSetTextFont(args(0).toString());
    return "";
}

/**
 * \brief
 */
std::string
SetTextSize(std::vector<std::string> args)
{
    if (argsCount() != 1)    return context->throwError("setTextSize() requires one argument");
    if (!args(0).isNumber()) return context->throwError(QScriptContext::TypeError, "setTextSize(): first argument is not a number");

    EmbReal num = args(0).toNumber();

    //isNaN check
    if (std::isnan(num)) {
        return context->throwError(QScriptContext::TypeError, "setTextSize(): first argument failed isNaN check. There is an error in your code.");
    }

    mainWin()->nativeSetTextSize(num);
    return "";
}

/**
 * \brief
 */
std::string
SetTextAngle(std::vector<std::string> args)
{
    if (argsCount() != 1)    return context->throwError("setTextAngle() requires one argument");
    if (!args(0).isNumber()) return context->throwError(QScriptContext::TypeError, "setTextAngle(): first argument is not a number");

    EmbReal num = args(0).toNumber();

    //isNaN check
    if (std::isnan(num))
        return context->throwError(QScriptContext::TypeError, "setTextAngle(): first argument failed isNaN check. There is an error in your code.");

    mainWin()->nativeSetTextAngle(num);
    return "";
}

/**
 * \brief
 */
std::string
SetTextBold(std::vector<std::string> args)
{
    if (args.length() != 1) {
        return context->throwError("setTextBold() requires one argument");
    }
    if (!args(0).isBool()) return context->throwError(QScriptContext::TypeError, "setTextBold(): first argument is not a bool");

    mainWin()->nativeSetTextBold(args(0).toBool());
    return "";
}

/**
 * \brief
 */
std::string
SetTextItalic(std::vector<std::string> args)
{
    if (argsCount() != 1)    return context->throwError("setTextItalic() requires one argument");
    if (!args(0).isBool()) return context->throwError(QScriptContext::TypeError, "setTextItalic(): first argument is not a bool");

    mainWin()->nativeSetTextItalic(args(0).toBool());
    return "";
}

/**
 * \brief
 */
std::string
SetTextUnderline(std::vector<std::string> args)
{
    if (argsCount() != 1)    return context->throwError("setTextUnderline() requires one argument");
    if (!args(0).isBool()) return context->throwError(QScriptContext::TypeError, "setTextUnderline(): first argument is not a bool");

    mainWin()->nativeSetTextUnderline(args(0).toBool());
    return "";
}

/**
 * \brief
 */
std::string
SetTextStrikeOut(std::vector<std::string> args)
{
    if (argsCount() != 1)    return context->throwError("setTextStrikeOut() requires one argument");
    if (!args(0).isBool()) return context->throwError(QScriptContext::TypeError, "setTextStrikeOut(): first argument is not a bool");

    mainWin()->nativeSetTextStrikeOut(args(0).toBool());
    return "";
}

/**
 * \brief
 */
std::string
SetTextOverline(std::vector<std::string> args)
{
    if (argsCount() != 1)    return context->throwError("setTextOverline() requires one argument");
    if (!args(0).isBool()) return context->throwError(QScriptContext::TypeError, "setTextOverline(): first argument is not a bool");

    mainWin()->nativeSetTextOverline(args(0).toBool());
    return "";
}

std::string
PreviewOn(std::vector<std::string> args)
{
    if (argsCount() != 5)    return context->throwError("previewOn() requires five arguments");
    if (!args(0).isString()) return context->throwError(QScriptContext::TypeError, "previewOn(): first argument is not a string");
    if (!args(1).isString()) return context->throwError(QScriptContext::TypeError, "previewOn(): second argument is not a string");
    if (!args(2).isNumber()) return context->throwError(QScriptContext::TypeError, "previewOn(): third argument is not a number");
    if (!args(3).isNumber()) return context->throwError(QScriptContext::TypeError, "previewOn(): fourth argument is not a number");
    if (!args(4).isNumber()) return context->throwError(QScriptContext::TypeError, "previewOn(): fifth argument is not a number");

    EmbString cloneStr = args(0).toString().toUpper();
    EmbString modeStr  = args(1).toString().toUpper();
    EmbReal x          = args(2).toNumber();
    EmbReal y          = args(3).toNumber();
    EmbReal data       = args(4).toNumber();

    int clone = PREVIEW_CLONE_NULL;
    int mode = PREVIEW_MODE_NULL;
    if     (cloneStr == "SELECTED") { clone = PREVIEW_CLONE_SELECTED; }
    else if (cloneStr == "RUBBER") { clone = PREVIEW_CLONE_RUBBER;   }
    else                            { return context->throwError(QScriptContext::UnknownError, "previewOn(): first argument must be \"SELECTED\" or \"RUBBER\"."); }

    if     (modeStr == "MOVE") { mode = PREVIEW_MODE_MOVE;   }
    else if (modeStr == "ROTATE") { mode = PREVIEW_MODE_ROTATE; }
    else if (modeStr == "SCALE") { mode = PREVIEW_MODE_SCALE;  }
    else                         { return context->throwError(QScriptContext::UnknownError, "previewOn(): second argument must be \"MOVE\", \"ROTATE\" or \"SCALE\"."); }

    // isNaN check
    if (std::isnan(x))
        return context->throwError(QScriptContext::TypeError, "previewOn(): third argument failed isNaN check. There is an error in your code.");
    if (std::isnan(y))
        return context->throwError(QScriptContext::TypeError, "previewOn(): fourth argument failed isNaN check. There is an error in your code.");
    if (std::isnan(data)) {
        return context->throwError(QScriptContext::TypeError,
            "previewOn(): fifth argument failed isNaN check. There is an error in your code.");
    }

    mainWin()->nativePreviewOn(clone, mode, x, y, data);
    return "";
}

/**
 * \brief
 */
std::string
PreviewOff(std::vector<std::string> args)
{
    /*
    if (argsCount() != 0) {
        return context->throwError("previewOff() requires zero arguments");
    }

    mainWin()->nativePreviewOff();
    */
    return "";
}

std::string
Vulcanize(std::vector<std::string> args)
{
    if (argsCount() != 0) return context->throwError("vulcanize() requires zero arguments");

    mainWin()->nativeVulcanize();
    return "";
}

std::string
AllowRubber(std::vector<std::string> args)
{
    if (argsCount() != 0) return context->throwError("allowRubber() requires zero arguments");

    return std::string(mainWin()->nativeAllowRubber());
}

std::string
SetRubberMode(std::vector<std::string> args)
{
    if (argsCount() != 1)    return context->throwError("setRubberMode() requires one argument");
    if (!args(0).isString()) return context->throwError(QScriptContext::TypeError, "setRubberMode(): first argument is not a string");

    EmbString mode = args(0).toString().toUpper();

    if     (mode == "CIRCLE_1P_RAD") { mainWin()->nativeSetRubberMode(OBJ_RUBBER_CIRCLE_1P_RAD); }
    else if (mode == "CIRCLE_1P_DIA") { mainWin()->nativeSetRubberMode(OBJ_RUBBER_CIRCLE_1P_DIA); }
    else if (mode == "CIRCLE_2P") { mainWin()->nativeSetRubberMode(OBJ_RUBBER_CIRCLE_2P); }
    else if (mode == "CIRCLE_3P") { mainWin()->nativeSetRubberMode(OBJ_RUBBER_CIRCLE_3P); }
    else if (mode == "CIRCLE_TTR") { mainWin()->nativeSetRubberMode(OBJ_RUBBER_CIRCLE_TTR); }
    else if (mode == "CIRCLE_TTR") { mainWin()->nativeSetRubberMode(OBJ_RUBBER_CIRCLE_TTT); }

    else if (mode == "DIMLEADER_LINE") { mainWin()->nativeSetRubberMode(OBJ_RUBBER_DIMLEADER_LINE); }

    else if (mode == "ELLIPSE_LINE") { mainWin()->nativeSetRubberMode(OBJ_RUBBER_ELLIPSE_LINE); }
    else if (mode == "ELLIPSE_MAJORDIAMETER_MINORRADIUS") { mainWin()->nativeSetRubberMode(OBJ_RUBBER_ELLIPSE_MAJORDIAMETER_MINORRADIUS); }
    else if (mode == "ELLIPSE_MAJORRADIUS_MINORRADIUS") { mainWin()->nativeSetRubberMode(OBJ_RUBBER_ELLIPSE_MAJORRADIUS_MINORRADIUS); }
    else if (mode == "ELLIPSE_ROTATION") { mainWin()->nativeSetRubberMode(OBJ_RUBBER_ELLIPSE_ROTATION); }

    else if (mode == "LINE") { mainWin()->nativeSetRubberMode(OBJ_RUBBER_LINE); }

    else if (mode == "POLYGON") { mainWin()->nativeSetRubberMode(OBJ_RUBBER_POLYGON); }
    else if (mode == "POLYGON_INSCRIBE") { mainWin()->nativeSetRubberMode(OBJ_RUBBER_POLYGON_INSCRIBE); }
    else if (mode == "POLYGON_CIRCUMSCRIBE") { mainWin()->nativeSetRubberMode(OBJ_RUBBER_POLYGON_CIRCUMSCRIBE); }

    else if (mode == "POLYLINE") { mainWin()->nativeSetRubberMode(OBJ_RUBBER_POLYLINE); }

    else if (mode == "RECTANGLE") { mainWin()->nativeSetRubberMode(OBJ_RUBBER_RECTANGLE); }

    else if (mode == "TEXTSINGLE") { mainWin()->nativeSetRubberMode(OBJ_RUBBER_TEXTSINGLE); }

    else                                                 { return context->throwError(QScriptContext::UnknownError, "setRubberMode(): unknown rubberMode value"); }

    return "";
}

/**
 * \brief
 */
std::string
SetRubberPoint(std::vector<std::string> args)
{
    if (argsCount() != 3) {
        return context->throwError("setRubberPoint() requires three arguments");
    }
    if (!args(0).isString()) {
        return context->throwError(QScriptContext::TypeError, "setRubberPoint(): first argument is not a string");
    }
    if (!args(1).isNumber()) {
        return context->throwError(QScriptContext::TypeError, "setRubberPoint(): second argument is not a number");
    }
    if (!args(2).isNumber()) {
        return context->throwError(QScriptContext::TypeError, "setRubberPoint(): third argument is not a number");
    }

    EmbString key = args(0).toString().toUpper();
    EmbReal x = args(1).toNumber();
    EmbReal y = args(2).toNumber();

    //isNaN check
    if (std::isnan(x)) {
        return context->throwError(QScriptContext::TypeError, "setRubberPoint(): second argument failed isNaN check. There is an error in your code.");
    }
    if (std::isnan(y)) {
        return context->throwError(QScriptContext::TypeError, "setRubberPoint(): third argument failed isNaN check. There is an error in your code.");
    }

    mainWin()->nativeSetRubberPoint(key, x, y);
    return "";
}

/**
 * \brief
 */
std::string
SetRubberText(std::vector<std::string> args)
{
    if (argsCount() != 2)    return context->throwError("setRubberText() requires two arguments");
    if (!args(0).isString()) return context->throwError(QScriptContext::TypeError, "setRubberText(): first argument is not a string");
    if (!args(1).isString()) return context->throwError(QScriptContext::TypeError, "setRubberText(): second argument is not a string");

    EmbString key = args(0).toString().toUpper();
    EmbString txt = args(1).toString();

    mainWin()->nativeSetRubberText(key, txt);
    return "";
}

std::string
AddRubber(std::vector<std::string> args)
{
    if (argsCount() != 1)    return context->throwError("addRubber() requires one argument");
    if (!args(0).isString()) return context->throwError(QScriptContext::TypeError, "addRubber(): first argument is not a string");

    EmbString objType = args(0).toString().toUpper();

    if (!mainWin()->nativeAllowRubber())
        return context->throwError(QScriptContext::UnknownError, "addRubber(): You must use vulcanize() before you can add another rubber object.");

    EmbReal mx = mainWin()->nativeMouseX();
    EmbReal my = mainWin()->nativeMouseY();

    if     (objType == "ARC") {} //TODO: handle this type
    else if (objType == "BLOCK") {} //TODO: handle this type
    else if (objType == "CIRCLE") { mainWin()->nativeAddCircle(mx, my, 0, false, OBJ_RUBBER_ON); }
    else if (objType == "DIMALIGNED") {} //TODO: handle this type
    else if (objType == "DIMANGULAR") {} //TODO: handle this type
    else if (objType == "DIMARCLENGTH") {} //TODO: handle this type
    else if (objType == "DIMDIAMETER") {} //TODO: handle this type
    else if (objType == "DIMLEADER") { mainWin()->nativeAddDimLeader(mx, my, mx, my, 0, OBJ_RUBBER_ON); }
    else if (objType == "DIMLINEAR") {} //TODO: handle this type
    else if (objType == "DIMORDINATE") {} //TODO: handle this type
    else if (objType == "DIMRADIUS") {} //TODO: handle this type
    else if (objType == "ELLIPSE") { mainWin()->nativeAddEllipse(mx, my, 0, 0, 0, 0, OBJ_RUBBER_ON); }
    else if (objType == "ELLIPSEARC") {} //TODO: handle this type
    else if (objType == "HATCH") {} //TODO: handle this type
    else if (objType == "IMAGE") {} //TODO: handle this type
    else if (objType == "INFINITELINE") {} //TODO: handle this type
    else if (objType == "LINE") { mainWin()->nativeAddLine(mx, my, mx, my, 0, OBJ_RUBBER_ON); }
    else if (objType == "PATH") {} //TODO: handle this type
    else if (objType == "POINT") {} //TODO: handle this type
    else if (objType == "POLYGON") { mainWin()->nativeAddPolygon(mx, my, QPainterPath(), OBJ_RUBBER_ON); }
    else if (objType == "POLYLINE") { mainWin()->nativeAddPolyline(mx, my, QPainterPath(), OBJ_RUBBER_ON); }
    else if (objType == "RAY") {} //TODO: handle this type
    else if (objType == "RECTANGLE") { mainWin()->nativeAddRectangle(mx, my, mx, my, 0, 0, OBJ_RUBBER_ON); }
    else if (objType == "SPLINE") {} //TODO: handle this type
    else if (objType == "TEXTMULTI") {} //TODO: handle this type
    else if (objType == "TEXTSINGLE") { mainWin()->nativeAddTextSingle("", mx, my, 0, false, OBJ_RUBBER_ON); }

    return "";
}

std::string
ClearRubber(std::vector<std::string> args)
{
    if (argsCount() != 0) return context->throwError("clearRubber() requires zero arguments");

    mainWin()->nativeClearRubber();
    return "";
}

std::string
SpareRubber(std::vector<std::string> args)
{
    if (argsCount() != 1)    return context->throwError("spareRubber() requires one argument");
    if (!args(0).isString()) return context->throwError(QScriptContext::TypeError, "spareRubber(): first argument is not a string");

    EmbString objID = args(0).toString().toUpper();

    if     (objID == "PATH") { mainWin()->nativeSpareRubber(SPARE_RUBBER_PATH);     }
    else if (objID == "POLYGON") { mainWin()->nativeSpareRubber(SPARE_RUBBER_POLYGON);  }
    else if (objID == "POLYLINE") { mainWin()->nativeSpareRubber(SPARE_RUBBER_POLYLINE); }
    else
    {
        bool ok = false;
        qint64 id = objID.toLongLong(&ok);
        if (!ok) return context->throwError(QScriptContext::TypeError, "spareRubber(): error converting object ID into an int64");
        mainWin()->nativeSpareRubber(id);
    }

    return "";
}

/**
 * \brief
 */
std::string
AddTextMulti(std::vector<std::string> args)
{
    if (argsCount() != 5)    return context->throwError("addTextMulti() requires five arguments");
    if (!args(0).isString()) return context->throwError(QScriptContext::TypeError, "addTextMulti(): first argument is not a string");
    if (!args(1).isNumber()) return context->throwError(QScriptContext::TypeError, "addTextMulti(): second argument is not a number");
    if (!args(2).isNumber()) return context->throwError(QScriptContext::TypeError, "addTextMulti(): third argument is not a number");
    if (!args(3).isNumber()) return context->throwError(QScriptContext::TypeError, "addTextMulti(): fourth argument is not a number");
    if (!args(4).isBool())   return context->throwError(QScriptContext::TypeError, "addTextMulti(): fifth argument is not a bool");

    EmbString str   = args(0).toString();
    EmbReal   x     = args(1).toNumber();
    EmbReal   y     = args(2).toNumber();
    EmbReal   rot   = args(3).toNumber();
    bool    fill  = args(4).toBool();

    //isNaN check
    if (std::isnan(x))   return context->throwError(QScriptContext::TypeError, "addTextMulti(): second argument failed isNaN check. There is an error in your code.");
    if (std::isnan(y))   return context->throwError(QScriptContext::TypeError, "addTextMulti(): third argument failed isNaN check. There is an error in your code.");
    if (std::isnan(rot)) return context->throwError(QScriptContext::TypeError, "addTextMulti(): fourth argument failed isNaN check. There is an error in your code.");

    mainWin()->nativeAddTextMulti(str, x, y, rot, fill, OBJ_RUBBER_OFF);
    return "";
}

std::string
AddTextSingle(std::vector<std::string> args)
{
    if (argsCount() != 5)    return context->throwError("addTextSingle() requires five arguments");
    if (!args(0).isString()) return context->throwError(QScriptContext::TypeError, "addTextSingle(): first argument is not a string");
    if (!args(1).isNumber()) return context->throwError(QScriptContext::TypeError, "addTextSingle(): second argument is not a number");
    if (!args(2).isNumber()) return context->throwError(QScriptContext::TypeError, "addTextSingle(): third argument is not a number");
    if (!args(3).isNumber()) return context->throwError(QScriptContext::TypeError, "addTextSingle(): fourth argument is not a number");
    if (!args(4).isBool())   return context->throwError(QScriptContext::TypeError, "addTextSingle(): fifth argument is not a bool");

    EmbString str   = args(0).toString();
    EmbReal   x     = args(1).toNumber();
    EmbReal   y     = args(2).toNumber();
    EmbReal   rot   = args(3).toNumber();
    bool    fill  = args(4).toBool();

    //isNaN check
    if (std::isnan(x))   return context->throwError(QScriptContext::TypeError, "addTextSingle(): second argument failed isNaN check. There is an error in your code.");
    if (std::isnan(y))   return context->throwError(QScriptContext::TypeError, "addTextSingle(): third argument failed isNaN check. There is an error in your code.");
    if (std::isnan(rot)) return context->throwError(QScriptContext::TypeError, "addTextSingle(): fourth argument failed isNaN check. There is an error in your code.");

    mainWin()->nativeAddTextSingle(str, x, y, rot, fill, OBJ_RUBBER_OFF);
    return "";
}

/**
 * \brief
 */
std::string
AddInfiniteLine(std::vector<std::string> args)
{
    //TODO: parameter error checking
    qDebug("TODO: finish addInfiniteLine command");
    return "";
}

/**
 * \brief
 */
std::string
AddRay(std::vector<std::string> args)
{
    //TODO: parameter error checking
    qDebug("TODO: finish addRay command");
    return "";
}

/**
 * \brief
 */
std::string
AddLine(std::vector<std::string> args)
{
    if (argsCount() != 5)    return context->throwError("addLine() requires five arguments");
    if (!args(0).isNumber()) return context->throwError(QScriptContext::TypeError, "addLine(): first argument is not a number");
    if (!args(1).isNumber()) return context->throwError(QScriptContext::TypeError, "addLine(): second argument is not a number");
    if (!args(2).isNumber()) return context->throwError(QScriptContext::TypeError, "addLine(): third argument is not a number");
    if (!args(3).isNumber()) return context->throwError(QScriptContext::TypeError, "addLine(): fourth argument is not a number");
    if (!args(4).isNumber()) return context->throwError(QScriptContext::TypeError, "addLine(): fifth argument is not a number");

    EmbReal x1  = args(0).toNumber();
    EmbReal y1  = args(1).toNumber();
    EmbReal x2  = args(2).toNumber();
    EmbReal y2  = args(3).toNumber();
    EmbReal rot = args(4).toNumber();

    //isNaN check
    if (std::isnan(x1))  return context->throwError(QScriptContext::TypeError, "addLine(): first argument failed isNaN check. There is an error in your code.");
    if (std::isnan(y1))  return context->throwError(QScriptContext::TypeError, "addLine(): second argument failed isNaN check. There is an error in your code.");
    if (std::isnan(x2))  return context->throwError(QScriptContext::TypeError, "addLine(): third argument failed isNaN check. There is an error in your code.");
    if (std::isnan(y2))  return context->throwError(QScriptContext::TypeError, "addLine(): fourth argument failed isNaN check. There is an error in your code.");
    if (std::isnan(rot)) return context->throwError(QScriptContext::TypeError, "addLine(): fifth argument failed isNaN check. There is an error in your code.");

    mainWin()->nativeAddLine(x1, y1, x2, y2, rot, OBJ_RUBBER_OFF);
    return "";
}

/**
 * \brief
 */
std::string
AddTriangle(std::vector<std::string> args)
{
    if (argsCount() != 8)    return context->throwError("addTriangle() requires eight arguments");
    if (!args(0).isNumber()) return context->throwError(QScriptContext::TypeError, "addTriangle(): first argument is not a number");
    if (!args(1).isNumber()) return context->throwError(QScriptContext::TypeError, "addTriangle(): second argument is not a number");
    if (!args(2).isNumber()) return context->throwError(QScriptContext::TypeError, "addTriangle(): third argument is not a number");
    if (!args(3).isNumber()) return context->throwError(QScriptContext::TypeError, "addTriangle(): fourth argument is not a number");
    if (!args(4).isNumber()) return context->throwError(QScriptContext::TypeError, "addTriangle(): fifth argument is not a number");
    if (!args(5).isNumber()) return context->throwError(QScriptContext::TypeError, "addTriangle(): sixth argument is not a number");
    if (!args(6).isNumber()) return context->throwError(QScriptContext::TypeError, "addTriangle(): seventh argument is not a number");
    if (!args(7).isBool())   return context->throwError(QScriptContext::TypeError, "addTriangle(): eighth argument is not a bool");

    EmbReal x1     = args(0).toNumber();
    EmbReal y1     = args(1).toNumber();
    EmbReal x2     = args(2).toNumber();
    EmbReal y2     = args(3).toNumber();
    EmbReal x3     = args(4).toNumber();
    EmbReal y3     = args(5).toNumber();
    EmbReal rot    = args(6).toNumber();
    bool  fill   = args(7).toBool();

    //isNaN check
    if (std::isnan(x1))  return context->throwError(QScriptContext::TypeError, "addTriangle(): first argument failed isNaN check. There is an error in your code.");
    if (std::isnan(y1))  return context->throwError(QScriptContext::TypeError, "addTriangle(): second argument failed isNaN check. There is an error in your code.");
    if (std::isnan(x2))  return context->throwError(QScriptContext::TypeError, "addTriangle(): third argument failed isNaN check. There is an error in your code.");
    if (std::isnan(y2))  return context->throwError(QScriptContext::TypeError, "addTriangle(): fourth argument failed isNaN check. There is an error in your code.");
    if (std::isnan(x3))  return context->throwError(QScriptContext::TypeError, "addTriangle(): fifth argument failed isNaN check. There is an error in your code.");
    if (std::isnan(y3))  return context->throwError(QScriptContext::TypeError, "addTriangle(): sixth argument failed isNaN check. There is an error in your code.");
    if (std::isnan(rot)) return context->throwError(QScriptContext::TypeError, "addTriangle(): seventh argument failed isNaN check. There is an error in your code.");

    mainWin()->nativeAddTriangle(x1, y1, x2, y2, x3, y3, rot, fill);
    return "";
}

/**
 * \brief
 */
std::string
AddRectangle(std::vector<std::string> args)
{
    if (argsCount() != 6)    return context->throwError("addRectangle() requires six arguments");
    if (!args(0).isNumber()) return context->throwError(QScriptContext::TypeError, "addRectangle(): first argument is not a number");
    if (!args(1).isNumber()) return context->throwError(QScriptContext::TypeError, "addRectangle(): second argument is not a number");
    if (!args(2).isNumber()) return context->throwError(QScriptContext::TypeError, "addRectangle(): third argument is not a number");
    if (!args(3).isNumber()) return context->throwError(QScriptContext::TypeError, "addRectangle(): fourth argument is not a number");
    if (!args(4).isNumber()) return context->throwError(QScriptContext::TypeError, "addRectangle(): fifth argument is not a number");
    if (!args(5).isBool())   return context->throwError(QScriptContext::TypeError, "addRectangle(): sixth argument is not a bool");

    EmbReal x    = args(0).toNumber();
    EmbReal y    = args(1).toNumber();
    EmbReal w    = args(2).toNumber();
    EmbReal h    = args(3).toNumber();
    EmbReal rot  = args(4).toNumber();
    bool  fill = args(5).toBool();

    //isNaN check
    if (std::isnan(x))   return context->throwError(QScriptContext::TypeError, "addRectangle(): first argument failed isNaN check. There is an error in your code.");
    if (std::isnan(y))   return context->throwError(QScriptContext::TypeError, "addRectangle(): second argument failed isNaN check. There is an error in your code.");
    if (std::isnan(w))   return context->throwError(QScriptContext::TypeError, "addRectangle(): third argument failed isNaN check. There is an error in your code.");
    if (std::isnan(h))   return context->throwError(QScriptContext::TypeError, "addRectangle(): fourth argument failed isNaN check. There is an error in your code.");
    if (std::isnan(rot)) return context->throwError(QScriptContext::TypeError, "addRectangle(): fifth argument failed isNaN check. There is an error in your code.");

    mainWin()->nativeAddRectangle(x, y, w, h, rot, fill, OBJ_RUBBER_OFF);
    return "";
}

/**
 * \brief
 */
std::string
AddRoundedRectangle(std::vector<std::string> args)
{
    if (argsCount() != 7)    return context->throwError("addRoundedRectangle() requires seven arguments");
    if (!args(0).isNumber()) return context->throwError(QScriptContext::TypeError, "addRoundedRectangle(): first argument is not a number");
    if (!args(1).isNumber()) return context->throwError(QScriptContext::TypeError, "addRoundedRectangle(): second argument is not a number");
    if (!args(2).isNumber()) return context->throwError(QScriptContext::TypeError, "addRoundedRectangle(): third argument is not a number");
    if (!args(3).isNumber()) return context->throwError(QScriptContext::TypeError, "addRoundedRectangle(): fourth argument is not a number");
    if (!args(4).isNumber()) return context->throwError(QScriptContext::TypeError, "addRoundedRectangle(): fifth argument is not a number");
    if (!args(5).isNumber()) return context->throwError(QScriptContext::TypeError, "addRoundedRectangle(): sixth argument is not a number");
    if (!args(6).isBool())   return context->throwError(QScriptContext::TypeError, "addRoundedRectangle(): seventh argument is not a bool");

    EmbReal x    = args(0).toNumber();
    EmbReal y    = args(1).toNumber();
    EmbReal w    = args(2).toNumber();
    EmbReal h    = args(3).toNumber();
    EmbReal rad  = args(4).toNumber();
    EmbReal rot  = args(5).toNumber();
    bool  fill = args(6).toBool();

    //isNaN check
    if (std::isnan(x))   return context->throwError(QScriptContext::TypeError, "addRoundedRectangle(): first argument failed isNaN check. There is an error in your code.");
    if (std::isnan(y))   return context->throwError(QScriptContext::TypeError, "addRoundedRectangle(): second argument failed isNaN check. There is an error in your code.");
    if (std::isnan(w))   return context->throwError(QScriptContext::TypeError, "addRoundedRectangle(): third argument failed isNaN check. There is an error in your code.");
    if (std::isnan(h))   return context->throwError(QScriptContext::TypeError, "addRoundedRectangle(): fourth argument failed isNaN check. There is an error in your code.");
    if (std::isnan(rad)) return context->throwError(QScriptContext::TypeError, "addRoundedRectangle(): fifth argument failed isNaN check. There is an error in your code.");
    if (std::isnan(rot)) return context->throwError(QScriptContext::TypeError, "addRoundedRectangle(): sixth argument failed isNaN check. There is an error in your code.");

    mainWin()->nativeAddRoundedRectangle(x, y, w, h, rad, rot, fill);
    return "";
}

/**
 * \brief
 */
std::string
AddArc(std::vector<std::string> args)
{
    if (argsCount() != 6)    return context->throwError("addArc() requires six arguments");
    if (!args(0).isNumber()) return context->throwError(QScriptContext::TypeError, "addArc(): first argument is not a number");
    if (!args(1).isNumber()) return context->throwError(QScriptContext::TypeError, "addArc(): second argument is not a number");
    if (!args(2).isNumber()) return context->throwError(QScriptContext::TypeError, "addArc(): third argument is not a number");
    if (!args(3).isNumber()) return context->throwError(QScriptContext::TypeError, "addArc(): fourth argument is not a number");
    if (!args(4).isNumber()) return context->throwError(QScriptContext::TypeError, "addArc(): fifth argument is not a number");
    if (!args(5).isNumber()) return context->throwError(QScriptContext::TypeError, "addArc(): sixth argument is not a number");

    EmbReal startX = args(0).toNumber();
    EmbReal startY = args(1).toNumber();
    EmbReal midX   = args(2).toNumber();
    EmbReal midY   = args(3).toNumber();
    EmbReal endX   = args(4).toNumber();
    EmbReal endY   = args(5).toNumber();

    //isNaN check
    if (std::isnan(startX)) return context->throwError(QScriptContext::TypeError, "addArc(): first argument failed isNaN check. There is an error in your code.");
    if (std::isnan(startY)) return context->throwError(QScriptContext::TypeError, "addArc(): second argument failed isNaN check. There is an error in your code.");
    if (std::isnan(midX))   return context->throwError(QScriptContext::TypeError, "addArc(): third argument failed isNaN check. There is an error in your code.");
    if (std::isnan(midY))   return context->throwError(QScriptContext::TypeError, "addArc(): fourth argument failed isNaN check. There is an error in your code.");
    if (std::isnan(endX))   return context->throwError(QScriptContext::TypeError, "addArc(): fifth argument failed isNaN check. There is an error in your code.");
    if (std::isnan(endY))   return context->throwError(QScriptContext::TypeError, "addArc(): sixth argument failed isNaN check. There is an error in your code.");

    mainWin()->nativeAddArc(startX, startY, midX, midY, endX, endY, OBJ_RUBBER_OFF);
    return "";
}

/**
 * \brief
 */
std::string
AddCircle(std::vector<std::string> args)
{
    if (argsCount() != 4)    return context->throwError("addCircle() requires four arguments");
    if (!args(0).isNumber()) return context->throwError(QScriptContext::TypeError, "addCircle(): first argument is not a number");
    if (!args(1).isNumber()) return context->throwError(QScriptContext::TypeError, "addCircle(): second argument is not a number");
    if (!args(2).isNumber()) return context->throwError(QScriptContext::TypeError, "addCircle(): third argument is not a number");
    if (!args(3).isBool())   return context->throwError(QScriptContext::TypeError, "addCircle(): fourth argument is not a bool");

    EmbReal centerX = args(0).toNumber();
    EmbReal centerY = args(1).toNumber();
    EmbReal radius  = args(2).toNumber();
    bool  fill    = args(3).toBool();

    //isNaN check
    if (std::isnan(centerX)) return context->throwError(QScriptContext::TypeError, "addCircle(): first argument failed isNaN check. There is an error in your code.");
    if (std::isnan(centerY)) return context->throwError(QScriptContext::TypeError, "addCircle(): second argument failed isNaN check. There is an error in your code.");
    if (std::isnan(radius))  return context->throwError(QScriptContext::TypeError, "addCircle(): third argument failed isNaN check. There is an error in your code.");

    mainWin()->nativeAddCircle(centerX, centerY, radius, fill, OBJ_RUBBER_OFF);
    return "";
}

/**
 * \brief
 */
std::string
AddSlot(std::vector<std::string> args)
{
    if (argsCount() != 6)    return context->throwError("addSlot() requires six arguments");
    if (!args(0).isNumber()) return context->throwError(QScriptContext::TypeError, "addSlot(): first argument is not a number");
    if (!args(1).isNumber()) return context->throwError(QScriptContext::TypeError, "addSlot(): second argument is not a number");
    if (!args(2).isNumber()) return context->throwError(QScriptContext::TypeError, "addSlot(): third argument is not a number");
    if (!args(3).isNumber()) return context->throwError(QScriptContext::TypeError, "addSlot(): fourth argument is not a number");
    if (!args(4).isNumber()) return context->throwError(QScriptContext::TypeError, "addSlot(): fifth argument is not a number");
    if (!args(5).isBool())   return context->throwError(QScriptContext::TypeError, "addSlot(): sixth argument is not a bool");

    EmbReal centerX  = args(0).toNumber();
    EmbReal centerY  = args(1).toNumber();
    EmbReal diameter = args(2).toNumber();
    EmbReal length   = args(3).toNumber();
    EmbReal rot      = args(4).toNumber();
    bool  fill     = args(5).toBool();

    //isNaN check
    if (std::isnan(centerX))  return context->throwError(QScriptContext::TypeError, "addSlot(): first argument failed isNaN check. There is an error in your code.");
    if (std::isnan(centerY))  return context->throwError(QScriptContext::TypeError, "addSlot(): second argument failed isNaN check. There is an error in your code.");
    if (std::isnan(diameter)) return context->throwError(QScriptContext::TypeError, "addSlot(): third argument failed isNaN check. There is an error in your code.");
    if (std::isnan(length))   return context->throwError(QScriptContext::TypeError, "addSlot(): fourth argument failed isNaN check. There is an error in your code.");
    if (std::isnan(rot))      return context->throwError(QScriptContext::TypeError, "addSlot(): fifth argument failed isNaN check. There is an error in your code.");

    mainWin()->nativeAddSlot(centerX, centerY, diameter, length, rot, fill, OBJ_RUBBER_OFF);
    return "";
}

std::string
AddEllipse(std::vector<std::string> args)
{
    if (argsCount() != 6)    return context->throwError("addEllipse() requires six arguments");
    if (!args(0).isNumber()) return context->throwError(QScriptContext::TypeError, "addEllipse(): first argument is not a number");
    if (!args(1).isNumber()) return context->throwError(QScriptContext::TypeError, "addEllipse(): second argument is not a number");
    if (!args(2).isNumber()) return context->throwError(QScriptContext::TypeError, "addEllipse(): third argument is not a number");
    if (!args(3).isNumber()) return context->throwError(QScriptContext::TypeError, "addEllipse(): fourth argument is not a number");
    if (!args(4).isNumber()) return context->throwError(QScriptContext::TypeError, "addEllipse(): fifth argument is not a number");
    if (!args(5).isBool())   return context->throwError(QScriptContext::TypeError, "addEllipse(): sixth argument is not a bool");

    EmbReal centerX = args(0).toNumber();
    EmbReal centerY = args(1).toNumber();
    EmbReal radX    = args(2).toNumber();
    EmbReal radY    = args(3).toNumber();
    EmbReal rot     = args(4).toNumber();
    bool  fill    = args(5).toBool();

    //isNaN check
    if (std::isnan(centerX)) return context->throwError(QScriptContext::TypeError, "addEllipse(): first argument failed isNaN check. There is an error in your code.");
    if (std::isnan(centerY)) return context->throwError(QScriptContext::TypeError, "addEllipse(): second argument failed isNaN check. There is an error in your code.");
    if (std::isnan(radX))    return context->throwError(QScriptContext::TypeError, "addEllipse(): third argument failed isNaN check. There is an error in your code.");
    if (std::isnan(radY))    return context->throwError(QScriptContext::TypeError, "addEllipse(): fourth argument failed isNaN check. There is an error in your code.");
    if (std::isnan(rot))     return context->throwError(QScriptContext::TypeError, "addEllipse(): fifth argument failed isNaN check. There is an error in your code.");

    mainWin()->nativeAddEllipse(centerX, centerY, radX, radY, rot, fill, OBJ_RUBBER_OFF);
    return "";
}

std::string
AddPoint(std::vector<std::string> args)
{
    if (argsCount() != 2)    return context->throwError("addPoint() requires two arguments");
    if (!args(0).isNumber()) return context->throwError(QScriptContext::TypeError, "addPoint(): first argument is not a number");
    if (!args(1).isNumber()) return context->throwError(QScriptContext::TypeError, "addPoint(): second argument is not a number");

    EmbReal x = args(0).toNumber();
    EmbReal y = args(1).toNumber();

    //isNaN check
    if (std::isnan(x)) return context->throwError(QScriptContext::TypeError, "addPoint(): first argument failed isNaN check. There is an error in your code.");
    if (std::isnan(y)) return context->throwError(QScriptContext::TypeError, "addPoint(): second argument failed isNaN check. There is an error in your code.");

    mainWin()->nativeAddPoint(x,y);
    return "";
}

std::string
AddRegularPolygon(std::vector<std::string> args)
{
    //TODO: parameter error checking
    qDebug("TODO: finish addRegularPolygon command");
    return "";
}

std::string
AddPolygon(std::vector<std::string> args)
{
    if (argsCount() != 1)   return context->throwError("addPolygon() requires one argument");
    if (!args(0).isArray()) return context->throwError(QScriptContext::TypeError, "addPolygon(): first argument is not an array");

    QVariantList varList = args(0).toVariant().toList();
    int varSize = varList.size();
    if (varSize < 2) return context->throwError(QScriptContext::TypeError, "addPolygon(): array must contain at least two elements");
    if (varSize % 2) return context->throwError(QScriptContext::TypeError, "addPolygon(): array cannot contain an odd number of elements");

    bool lineTo = false;
    bool xCoord = true;
    EmbReal x = 0;
    EmbReal y = 0;
    EmbReal startX = 0;
    EmbReal startY = 0;
    QPainterPath path;
    foreach(QVariant var, varList)
    {
        if (var.canConvert(QVariant::Double))
        {
            if (xCoord)
            {
                xCoord = false;
                x = var.toReal();
            }
            else
            {
                xCoord = true;
                y = -var.toReal();

                if (lineTo) { path.lineTo(x,y); }
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
    return "";
}

std::string
AddPolyline(std::vector<std::string> args)
{
    if (argsCount() != 1)   return context->throwError("addPolyline() requires one argument");
    if (!args(0).isArray()) return context->throwError(QScriptContext::TypeError, "addPolyline(): first argument is not an array");

    QVariantList varList = args(0).toVariant().toList();
    int varSize = varList.size();
    if (varSize < 2) return context->throwError(QScriptContext::TypeError, "addPolyline(): array must contain at least two elements");
    if (varSize % 2) return context->throwError(QScriptContext::TypeError, "addPolyline(): array cannot contain an odd number of elements");

    bool lineTo = false;
    bool xCoord = true;
    EmbReal x = 0;
    EmbReal y = 0;
    EmbReal startX = 0;
    EmbReal startY = 0;
    QPainterPath path;
    foreach (QVariant var, varList) {
        if (var.canConvert(QVariant::Double)) {
            if (xCoord) {
                xCoord = false;
                x = var.toReal();
            }
            else {
                xCoord = true;
                y = -var.toReal();

                if (lineTo) { path.lineTo(x,y); }
                else       { path.moveTo(x,y); lineTo = true; startX = x; startY = y; }
            }
        }
        else
            return context->throwError(QScriptContext::TypeError, "addPolyline(): array contains one or more invalid elements");
    }

    path.translate(-startX, -startY);

    mainWin()->nativeAddPolyline(startX, startY, path, OBJ_RUBBER_OFF);
    return "";
}

std::string
AddPath(std::vector<std::string> args)
{
    //TODO: parameter error checking
    qDebug("TODO: finish addPath command");
    return "";
}

std::string
AddHorizontalDimension(std::vector<std::string> args)
{
    //TODO: parameter error checking
    qDebug("TODO: finish addHorizontalDimension command");
    return "";
}

std::string
AddVerticalDimension(std::vector<std::string> args)
{
    //TODO: parameter error checking
    qDebug("TODO: finish addVerticalDimension command");
    return "";
}

std::string
AddImage(std::vector<std::string> args)
{
    //TODO: parameter error checking
    qDebug("TODO: finish addImage command");
    return "";
}

std::string
AddDimLeader(std::vector<std::string> args)
{
    if (argsCount() != 5)    return context->throwError("addDimLeader() requires five arguments");
    if (!args(0).isNumber()) return context->throwError(QScriptContext::TypeError, "addDimLeader(): first argument is not a number");
    if (!args(1).isNumber()) return context->throwError(QScriptContext::TypeError, "addDimLeader(): second argument is not a number");
    if (!args(2).isNumber()) return context->throwError(QScriptContext::TypeError, "addDimLeader(): third argument is not a number");
    if (!args(3).isNumber()) return context->throwError(QScriptContext::TypeError, "addDimLeader(): fourth argument is not a number");
    if (!args(4).isNumber()) return context->throwError(QScriptContext::TypeError, "addDimLeader(): fifth argument is not a number");

    EmbReal x1  = args(0).toNumber();
    EmbReal y1  = args(1).toNumber();
    EmbReal x2  = args(2).toNumber();
    EmbReal y2  = args(3).toNumber();
    EmbReal rot = args(4).toNumber();

    //isNaN check
    if (std::isnan(x1))  return context->throwError(QScriptContext::TypeError, "addDimLeader(): first argument failed isNaN check. There is an error in your code.");
    if (std::isnan(y1))  return context->throwError(QScriptContext::TypeError, "addDimLeader(): second argument failed isNaN check. There is an error in your code.");
    if (std::isnan(x2))  return context->throwError(QScriptContext::TypeError, "addDimLeader(): third argument failed isNaN check. There is an error in your code.");
    if (std::isnan(y2))  return context->throwError(QScriptContext::TypeError, "addDimLeader(): fourth argument failed isNaN check. There is an error in your code.");
    if (std::isnan(rot)) return context->throwError(QScriptContext::TypeError, "addDimLeader(): fifth argument failed isNaN check. There is an error in your code.");

    mainWin()->nativeAddDimLeader(x1, y1, x2, y2, rot, OBJ_RUBBER_OFF);
    return "";
}

std::string
SetCursorShape(std::vector<std::string> args)
{
    if (argsCount() != 1)    return context->throwError("setCursorShape() requires one argument");
    if (!args(0).isString()) return context->throwError(QScriptContext::TypeError, "setCursorShape(): first argument is not a string");

    EmbString shape = args(0).toString();
    mainWin()->nativeSetCursorShape(shape);
    return "";
}

std::string
CalculateAngle(std::vector<std::string> args)
{
    if (argsCount() != 4)    return context->throwError("calculateAngle() requires four arguments");
    if (!args(0).isNumber()) return context->throwError(QScriptContext::TypeError, "calculateAngle(): first argument is not a number");
    if (!args(1).isNumber()) return context->throwError(QScriptContext::TypeError, "calculateAngle(): second argument is not a number");
    if (!args(2).isNumber()) return context->throwError(QScriptContext::TypeError, "calculateAngle(): third argument is not a number");
    if (!args(3).isNumber()) return context->throwError(QScriptContext::TypeError, "calculateAngle(): fourth argument is not a number");

    EmbReal x1 = args(0).toNumber();
    EmbReal y1 = args(1).toNumber();
    EmbReal x2 = args(2).toNumber();
    EmbReal y2 = args(3).toNumber();

    //isNaN check
    if (std::isnan(x1))  return context->throwError(QScriptContext::TypeError, "calculateAngle(): first argument failed isNaN check. There is an error in your code.");
    if (std::isnan(y1))  return context->throwError(QScriptContext::TypeError, "calculateAngle(): second argument failed isNaN check. There is an error in your code.");
    if (std::isnan(x2))  return context->throwError(QScriptContext::TypeError, "calculateAngle(): third argument failed isNaN check. There is an error in your code.");
    if (std::isnan(y2))  return context->throwError(QScriptContext::TypeError, "calculateAngle(): fourth argument failed isNaN check. There is an error in your code.");

    return std::string(mainWin()->nativeCalculateAngle(x1, y1, x2, y2));
}

std::string
CalculateDistance(std::vector<std::string> args)
{
    if (argsCount() != 4)    return context->throwError("calculateDistance() requires four arguments");
    if (!args(0).isNumber()) return context->throwError(QScriptContext::TypeError, "calculateDistance(): first argument is not a number");
    if (!args(1).isNumber()) return context->throwError(QScriptContext::TypeError, "calculateDistance(): second argument is not a number");
    if (!args(2).isNumber()) return context->throwError(QScriptContext::TypeError, "calculateDistance(): third argument is not a number");
    if (!args(3).isNumber()) return context->throwError(QScriptContext::TypeError, "calculateDistance(): fourth argument is not a number");

    EmbReal x1 = args(0).toNumber();
    EmbReal y1 = args(1).toNumber();
    EmbReal x2 = args(2).toNumber();
    EmbReal y2 = args(3).toNumber();

    //isNaN check
    if (std::isnan(x1))  return context->throwError(QScriptContext::TypeError, "calculateDistance(): first argument failed isNaN check. There is an error in your code.");
    if (std::isnan(y1))  return context->throwError(QScriptContext::TypeError, "calculateDistance(): second argument failed isNaN check. There is an error in your code.");
    if (std::isnan(x2))  return context->throwError(QScriptContext::TypeError, "calculateDistance(): third argument failed isNaN check. There is an error in your code.");
    if (std::isnan(y2))  return context->throwError(QScriptContext::TypeError, "calculateDistance(): fourth argument failed isNaN check. There is an error in your code.");

    return std::string(mainWin()->nativeCalculateDistance(x1, y1, x2, y2));
}

std::string
PerpendicularDistance(std::vector<std::string> args)
{
    if (argsCount() != 6)    return context->throwError("perpendicularDistance() requires six arguments");
    if (!args(0).isNumber()) return context->throwError(QScriptContext::TypeError, "perpendicularDistance(): first argument is not a number");
    if (!args(1).isNumber()) return context->throwError(QScriptContext::TypeError, "perpendicularDistance(): second argument is not a number");
    if (!args(2).isNumber()) return context->throwError(QScriptContext::TypeError, "perpendicularDistance(): third argument is not a number");
    if (!args(3).isNumber()) return context->throwError(QScriptContext::TypeError, "perpendicularDistance(): fourth argument is not a number");
    if (!args(4).isNumber()) return context->throwError(QScriptContext::TypeError, "perpendicularDistance(): fifth argument is not a number");
    if (!args(5).isNumber()) return context->throwError(QScriptContext::TypeError, "perpendicularDistance(): sixth argument is not a number");

    EmbReal px = args(0).toNumber();
    EmbReal py = args(1).toNumber();
    EmbReal x1 = args(2).toNumber();
    EmbReal y1 = args(3).toNumber();
    EmbReal x2 = args(4).toNumber();
    EmbReal y2 = args(5).toNumber();

    //isNaN check
    if (std::isnan(px))  return context->throwError(QScriptContext::TypeError, "perpendicularDistance(): first argument failed isNaN check. There is an error in your code.");
    if (std::isnan(py))  return context->throwError(QScriptContext::TypeError, "perpendicularDistance(): second argument failed isNaN check. There is an error in your code.");
    if (std::isnan(x1))  return context->throwError(QScriptContext::TypeError, "perpendicularDistance(): third argument failed isNaN check. There is an error in your code.");
    if (std::isnan(y1))  return context->throwError(QScriptContext::TypeError, "perpendicularDistance(): fourth argument failed isNaN check. There is an error in your code.");
    if (std::isnan(x2))  return context->throwError(QScriptContext::TypeError, "perpendicularDistance(): fifth argument failed isNaN check. There is an error in your code.");
    if (std::isnan(y2))  return context->throwError(QScriptContext::TypeError, "perpendicularDistance(): sixth argument failed isNaN check. There is an error in your code.");

    return std::string(mainWin()->nativePerpendicularDistance(px, py, x1, y1, x2, y2));
}

std::string
NumSelected(std::vector<std::string> args)
{
    if (argsCount() != 0) return context->throwError("numSelected() requires zero arguments");
    return std::string(mainWin()->nativeNumSelected());
}

std::string
SelectAll(std::vector<std::string> args)
{
    if (argsCount() != 0) return context->throwError("selectAll() requires zero arguments");

    mainWin()->nativeSelectAll();
    return "";
}

std::string
AddToSelection(std::vector<std::string> args)
{
    //TODO: finish
    return "";
}

std::string
ClearSelection(std::vector<std::string> args)
{
    if (argsCount() != 0) return context->throwError("clearSelection() requires zero arguments");

    mainWin()->nativeClearSelection();
    return "";
}

std::string
DeleteSelected(std::vector<std::string> args)
{
    if (argsCount() != 0) return context->throwError("deleteSelected() requires zero arguments");

    mainWin()->nativeDeleteSelected();
    return "";
}

std::string
CutSelected(std::vector<std::string> args)
{
    if (argsCount() != 2)    return context->throwError("cutSelected() requires two arguments");
    if (!args(0).isNumber()) return context->throwError(QScriptContext::TypeError, "cutSelected(): first argument is not a number");
    if (!args(1).isNumber()) return context->throwError(QScriptContext::TypeError, "cutSelected(): second argument is not a number");

    EmbReal x = args(0).toNumber();
    EmbReal y = args(1).toNumber();

    //isNaN check
    if (std::isnan(x)) return context->throwError(QScriptContext::TypeError, "cutSelected(): first argument failed isNaN check. There is an error in your code.");
    if (std::isnan(y)) return context->throwError(QScriptContext::TypeError, "cutSelected(): second argument failed isNaN check. There is an error in your code.");

    mainWin()->nativeCutSelected(x, y);
    return "";
}

std::string
CopySelected(std::vector<std::string> args)
{
    if (argsCount() != 2)    return context->throwError("copySelected() requires two arguments");
    if (!args(0).isNumber()) return context->throwError(QScriptContext::TypeError, "copySelected(): first argument is not a number");
    if (!args(1).isNumber()) return context->throwError(QScriptContext::TypeError, "copySelected(): second argument is not a number");

    EmbReal x = args(0).toNumber();
    EmbReal y = args(1).toNumber();

    //isNaN check
    if (std::isnan(x)) return context->throwError(QScriptContext::TypeError, "copySelected(): first argument failed isNaN check. There is an error in your code.");
    if (std::isnan(y)) return context->throwError(QScriptContext::TypeError, "copySelected(): second argument failed isNaN check. There is an error in your code.");

    mainWin()->nativeCopySelected(x, y);
    return "";
}

std::string
PasteSelected(std::vector<std::string> args)
{
    if (argsCount() != 2)    return context->throwError("pasteSelected() requires two arguments");
    if (!args(0).isNumber()) return context->throwError(QScriptContext::TypeError, "pasteSelected(): first argument is not a number");
    if (!args(1).isNumber()) return context->throwError(QScriptContext::TypeError, "pasteSelected(): second argument is not a number");

    EmbReal x = args(0).toNumber();
    EmbReal y = args(1).toNumber();

    //isNaN check
    if (std::isnan(x)) return context->throwError(QScriptContext::TypeError, "pasteSelected(): first argument failed isNaN check. There is an error in your code.");
    if (std::isnan(y)) return context->throwError(QScriptContext::TypeError, "pasteSelected(): second argument failed isNaN check. There is an error in your code.");

    mainWin()->nativePasteSelected(x, y);
    return "";
}

std::string
MoveSelected(std::vector<std::string> args)
{
    if (argsCount() != 2)    return context->throwError("moveSelected() requires two arguments");
    if (!args(0).isNumber()) return context->throwError(QScriptContext::TypeError, "moveSelected(): first argument is not a number");
    if (!args(1).isNumber()) return context->throwError(QScriptContext::TypeError, "moveSelected(): second argument is not a number");

    EmbReal dx = args(0).toNumber();
    EmbReal dy = args(1).toNumber();

    //isNaN check
    if (std::isnan(dx)) return context->throwError(QScriptContext::TypeError, "moveSelected(): first argument failed isNaN check. There is an error in your code.");
    if (std::isnan(dy)) return context->throwError(QScriptContext::TypeError, "moveSelected(): second argument failed isNaN check. There is an error in your code.");

    mainWin()->nativeMoveSelected(dx, dy);
    return "";
}

std::string
ScaleSelected(std::vector<std::string> args)
{
    if (argsCount() != 3)    return context->throwError("scaleSelected() requires three arguments");
    if (!args(0).isNumber()) return context->throwError(QScriptContext::TypeError, "scaleSelected(): first argument is not a number");
    if (!args(1).isNumber()) return context->throwError(QScriptContext::TypeError, "scaleSelected(): second argument is not a number");
    if (!args(2).isNumber()) return context->throwError(QScriptContext::TypeError, "scaleSelected(): third argument is not a number");

    EmbReal x      = args(0).toNumber();
    EmbReal y      = args(1).toNumber();
    EmbReal factor = args(2).toNumber();

    //isNaN check
    if (std::isnan(x))      return context->throwError(QScriptContext::TypeError, "scaleSelected(): first argument failed isNaN check. There is an error in your code.");
    if (std::isnan(y))      return context->throwError(QScriptContext::TypeError, "scaleSelected(): second argument failed isNaN check. There is an error in your code.");
    if (std::isnan(factor)) return context->throwError(QScriptContext::TypeError, "scaleSelected(): third argument failed isNaN check. There is an error in your code.");

    if (factor <= 0.0) return context->throwError(QScriptContext::UnknownError, "scaleSelected(): scale factor must be greater than zero");

    mainWin()->nativeScaleSelected(x, y, factor);
    return "";
}

std::string
RotateSelected(std::vector<std::string> args)
{
    if (argsCount() != 3)    return context->throwError("rotateSelected() requires three arguments");
    if (!args(0).isNumber()) return context->throwError(QScriptContext::TypeError, "rotateSelected(): first argument is not a number");
    if (!args(1).isNumber()) return context->throwError(QScriptContext::TypeError, "rotateSelected(): second argument is not a number");
    if (!args(2).isNumber()) return context->throwError(QScriptContext::TypeError, "rotateSelected(): third argument is not a number");

    EmbReal x   = args(0).toNumber();
    EmbReal y   = args(1).toNumber();
    EmbReal rot = args(2).toNumber();

    //isNaN check
    if (std::isnan(x))   return context->throwError(QScriptContext::TypeError, "rotateSelected(): first argument failed isNaN check. There is an error in your code.");
    if (std::isnan(y))   return context->throwError(QScriptContext::TypeError, "rotateSelected(): second argument failed isNaN check. There is an error in your code.");
    if (std::isnan(rot)) return context->throwError(QScriptContext::TypeError, "rotateSelected(): third argument failed isNaN check. There is an error in your code.");

    mainWin()->nativeRotateSelected(x, y, rot);
    return "";
}

std::string
MirrorSelected(std::vector<std::string> args)
{
    if (argsCount() != 4)    return context->throwError("mirrorSelected() requires four arguments");
    if (!args(0).isNumber()) return context->throwError(QScriptContext::TypeError, "mirrorSelected(): first argument is not a number");
    if (!args(1).isNumber()) return context->throwError(QScriptContext::TypeError, "mirrorSelected(): second argument is not a number");
    if (!args(2).isNumber()) return context->throwError(QScriptContext::TypeError, "mirrorSelected(): third argument is not a number");
    if (!args(3).isNumber()) return context->throwError(QScriptContext::TypeError, "mirrorSelected(): fourth argument is not a number");

    EmbReal x1 = args(0).toNumber();
    EmbReal y1 = args(1).toNumber();
    EmbReal x2 = args(2).toNumber();
    EmbReal y2 = args(3).toNumber();

    //isNaN check
    if (std::isnan(x1))  return context->throwError(QScriptContext::TypeError, "mirrorSelected(): first argument failed isNaN check. There is an error in your code.");
    if (std::isnan(y1))  return context->throwError(QScriptContext::TypeError, "mirrorSelected(): second argument failed isNaN check. There is an error in your code.");
    if (std::isnan(x2))  return context->throwError(QScriptContext::TypeError, "mirrorSelected(): third argument failed isNaN check. There is an error in your code.");
    if (std::isnan(y2))  return context->throwError(QScriptContext::TypeError, "mirrorSelected(): fourth argument failed isNaN check. There is an error in your code.");

    mainWin()->nativeMirrorSelected(x1, y1, x2, y2);
    return "";
}

std::string
QSnapX(std::vector<std::string> args)
{
    if (argsCount() != 0) {
        return context->throwError("qsnapX() requires zero arguments");
    }
    return std::string(mainWin()->nativeQSnapX());
}

std::string
QSnapY(std::vector<std::string> args)
{
    if (argsCount() != 0) {
        return context->throwError("qsnapY() requires zero arguments");
    }
    return std::string(mainWin()->nativeQSnapY());
}

std::string
MouseX(std::vector<std::string> args)
{
    if (argsCount() != 0) {
        return context->throwError("mouseX() requires zero arguments");
    }
    return std::string(mainWin()->nativeMouseX());
}

std::string
MouseY(std::vector<std::string> args)
{
    if (argsCount() != 0) {
        return context->throwError("mouseY() requires zero arguments");
    }
    return std::string(mainWin()->nativeMouseY());
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
        if (i < settings_opensave_recent_max_files)
        {
            recentFileInfo = QFileInfo(settings_opensave_recent_list_of_files.at(i));
            if (recentFileInfo.exists() && validFileFormat(recentFileInfo.fileName()))
            {
                recentValue.setNum(i+1);
                QAction* rAction;
                if     (recentValue.toInt() >= 1 && recentValue.toInt() <= 9) rAction = new QAction("&" + recentValue + " " + recentFileInfo.fileName(), this);
                else if (recentValue.toInt() == 10)                            rAction = new QAction("1&0 "                  + recentFileInfo.fileName(), this);
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
    if (!aSender)
        return;
    QWidget* w = mdiArea->subWindowList().at(aSender->data().toInt());
    if (w && checked)
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
    if (recent)
    {
        files.append(recentFile);
        openFilesSelected(files);
    }
    else if (!preview)
    {
        //TODO: set getOpenFileNames' selectedFilter parameter from settings_opensave_open_format
        files = QFileDialog::getOpenFileNames(this, tr("Open"), openFilesPath, formatFilterOpen);
        openFilesSelected(files);
    }
    else if (preview)
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
            if (doOnce) { updateMenuToolbarStatusbar(); doOnce = false; }

            if (mdiWin->loadFile(filesToOpen.at(i)))
            {
                statusbar->showMessage(tr("File(s) loaded"), 2000);
                mdiWin->show();
                mdiWin->showMaximized();
                //Prevent duplicate entries in the recent files list
                if (!settings_opensave_recent_list_of_files.contains(filesToOpen.at(i), Qt::CaseInsensitive))
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
                if (v)
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
    if (!mdiWin)
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
        if (mdiWin)
        {
            if (mdiWin->getCurrentFile() == canonicalFilePath)
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
    if (theMdiWin) { keepMaximized = theMdiWin->isMaximized(); }

    mdiArea->removeSubWindow(theMdiWin);
    theMdiWin->deleteLater();

    updateMenuToolbarStatusbar();
    windowMenuAboutToShow();

    if (keepMaximized) {
        MdiWindow* mdiWin = qobject_cast<MdiWindow*>(mdiArea->activeSubWindow());
        if (mdiWin) { mdiWin->showMaximized(); }
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
        if (readerState == stable || readerState == unstable)
        {
            //Exclude color file formats from open dialogs
            if (upperExt != "COL" && upperExt != "EDR" && upperExt != "INF" && upperExt != "RGB")
            {
                supportedReaders.append(supportedStr);
                individualReaders.append(individualStr);
            }
        }
        if (writerState == stable || writerState == unstable)
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
    if (custom.contains("supported", Qt::CaseInsensitive))
        custom = ""; //This will hide it
    else if (!custom.contains("*", Qt::CaseInsensitive))
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
                if (ACTION->objectName() == "toolbarclose")
                {
                    tb->removeAction(ACTION);
                    disconnect(tb, SIGNAL(actionTriggered(QAction*)), this, SLOT(closeToolBar(QAction*)));
                    delete ACTION;
                }
            }
        }
    }
}
