/**
 *  Embroidermodder 2.
 *
 *  ------------------------------------------------------------
 *
 *  Copyright 2013-2022 The Embroidermodder Team
 *  Embroidermodder 2 is Open Source Software.
 *  See LICENSE for licensing terms.
 *
 *  ------------------------------------------------------------
 *
 *  Use Python's PEP7 style guide.
 *      https://peps.python.org/pep-0007/
 */

/**
 * \file mainwindow.cpp
 */

#include "embroidermodder.h"

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
    QFileInfo check = QFileInfo(appDir + "/help");
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
    if (command == "about") {
        about();
        return "";
    }
    if (command == "day") {
        dayVision();
        return "";
    }
    if (command == "end") {
        nativeEndCommand();
        return "";
    }
    if (command == "help") {
        help();
        return "";
    }
    if (command == "init") {
        nativeInitCommand();
        return "";
    }
    if (command == "new") {
        nativeClearSelection();
        newFile();
        return "";
    }
    if (command == "night") {
        nightVision();
        return "";
    }
    if (command == "platform") {
        return "<br/>" + platformString().toStdString();
    }
    if (command == "version") {
        return "<br/>2.0.0-alpha";
    }
    if (command == "selectall") {
        selectAll();
        return "";
    }
    if (command == "tipoftheday") {
        tipOfTheDay();
        return "";
    }
    if (command == "redo") {
        redo();
        return "";
    }
    if (command == "undo") {
        undo();
        return "";
    }
    if (command == "windowcascade") {
        nativeWindowCascade();
        return "";
    }
    if (command == "windowclose") {
        nativeWindowClose();
        return "";
    }
    if (command == "windowcloseall") {
        nativeWindowCloseAll();
        return "";
    }
    if (command == "windowtile") {
        nativeWindowTile();
        return "";
    }
    if (command == "windownext") {
        nativeWindowNext();
        return "";
    }
    if (command == "windowprevious") {
        nativeWindowPrevious();
        return "";
    }
    if (command == "zoomextents") {
        nativeZoomExtents();
        return "";
    }
    if (command == "zoomin") {
        nativeZoomIn();
        return "";
    }
    if (command == "zoomout") {
        nativeZoomOut();
        return "";
    }
    if (command == "open") {
        nativeOpenFile();
        return "";
    }
    if (command == "exit") {
        nativeExit();
        return "";
    }
    if (command == "icon16") {
        nativeIcon16();
        return "";
    }
    if (command == "icon24") {
        nativeIcon24();
        return "";
    }
    if (command == "icon32") {
        nativeIcon32();
        return "";
    }
    if (command == "icon48") {
        nativeIcon48();
        return "";
    }
    if (command == "icon64") {
        nativeIcon64();
        return "";
    }
    if (command == "icon128") {
        nativeIcon128();
        return "";
    }
    if (command == "pan left") {
        nativePanLeft();
        return "";
    }
/*
    if (command == "pan right") {
        nativePanRight();
        return "";
    }
    if (command == "pan up") {
        nativePanUp();
        return "";
    }
    if (command == "pan down") {
        nativePanDown();
        return "";
    }

    if (command == "day") {
        nativeDayVision();
        return "";
    }
    if (command == "night") {
        nativeNightVision();
        return "";
    }
    if (command == "text font") {
        output = nativeTextFont();
        return "";
    }
    if (command == "text size") {
        output = nativeTextSize();
        return "";
    }
    if (command == "text angle") {
        return nativeTextAngle();
    }
    if (command == "text bold") {
        output = nativeTextBold();
        return "";
    }
    if (command == "text italic") {
        output = nativeTextItalic();
        return "";
    }
    if (command == "text underline") {
        output = nativeTextUnderline();
        return "";
    }
    if (command == "text strikeout") {
        output = nativeTextStrikeOut();
        return "";
    }
    if (command == "text overline") {
        output = nativeTextOverline();
        return "";
    }

    if (command == "set text font", output = ""; nativeSetTextFont(args[0]);}
        return "";
    }
    if (command == "set text size", output = ""; nativeSetTextSize(atoi(args[0]));}
        return "";
    }
"set text angle",  output = ""; nativeSetTextAngle(args[0]);}
        return "";
    }
"set text bold", output = ""; nativeTextBold(args[0]);}
        return "";
    }
"set text italic", output = ""; nativeTextItalic(args[0]);}
        return "";
    }
"set text underline", output = ""; nativeTextUnderline(args[0]);}
        return "";
    }
"set text strikeout", output = ""; nativeTextStrikeOut(args[0]);}
        return "";
    }
"set text overline", output = ""; nativeTextOverline(args[0]);}
        return "";
    }

    if (command == "num selected") {
        output = nativeNumSelected();
        return "";
    }
    if (command == "select all") { nativeSelectAll();
        return "";
    }
    if (command == "add to selection") { AddToSelection();}
        return "";
    }
    if (command == "clear selection") { nativeClearSelection();}
        return "";
    }
    if (command == "delete selection") { nativeDeleteSelected(); }
        return "";
    }

    if (command == "qsnapx") {
        return nativeQSnapX();
    }
    if (command == "qsnapY") {
        return nativeQSnapY();
    }
    if (command == "mousex") {
        return nativeMouseX();
    }
    if (command == "mousey") {
        return nativeMouseY();
    }

    if (command == "debug") {
        scriptValDebug();
        return "";
    }
    if (command == "error") {
        scriptValError();
        return "";
    }
    if (command == "todo") {
        Todo();
        return "";
    }
    if (command == "alert") {
        output = "";
        Alert();
    }
    if (command == "blinkPrompt") {
        BlinkPrompt();
    }
    if (command == "setPromptPrefix") {
        SetPromptPrefix(); }
    if (command == "appendPromptHistory") {
        AppendPromptHistory(); }
    if (command == "enablePromptRapidFire") { EnablePromptRapidFire(); }
    if (command == "disablePromptRapidFire") { DisablePromptRapidFire(); }
    if (command == "enableMoveRapidFire") { EnableMoveRapidFire(); }
    if (command == "disableMoveRapidFire") { DisableMoveRapidFire(); }
    if (command == "initCommand") { InitCommand(); }
    if (command == "endCommand") { EndCommand(); }
    if (command == "newFile") { NewFile(); }
    if (command == "openFile") { OpenFile(); }
    if (command == "exit") { Exit(); }
    if (command == "help") { Help(); }
    if (command == "about") { About(); }
    if (command == "tipOfTheDay") { TipOfTheDay(); }
    if (command == "windowCascade") { WindowCascade(); }
    if (command == "windowTile") { WindowTile(); }
    if (command == "windowClose") { WindowClose(); }
    if (command == "windowCloseAll") { WindowCloseAll(); }
    if (command == "windowNext") {
        WindowNext(); }
    if (command == "windowPrevious") {
        WindowPrevious(); }
    if (command == "platformString") {
        PlatformString(); }
    if (command == "messageBox") {
        MessageBox(); }
    if (command == "isInt") {
        IsInt(); }
    if (command == "undo") { Undo(); }
    if (command == "redo") { Redo(); }
    if (command == "icon16") { Icon16(); }
    if (command == "icon24") { Icon24(); }
    if (command == "icon32") { Icon32(); }
    if (command == "icon48") { Icon48(); }
    if (command == "icon64") { Icon64(); }
    if (command == "icon128") { Icon128(); }
    if (command == "panLeft") { PanLeft(); }
    if (command == "panRight") { PanRight(); }
    if (command == "panUp") { PanUp(); }
    if (command == "panDown") { PanDown(); }
    if (command == "zoomIn") { ZoomIn(); }
    if (command == "zoomOut") { ZoomOut(); }
    if (command == "zoomExtents") { ZoomExtents(); }
    if (command == "printArea") { PrintArea(); }
    if (command == "dayVision") { DayVision(); }
    if (command == "nightVision") { NightVision(); }
    if (command == "setBackgroundColor") { SetBackgroundColor(); }
    if (command == "setCrossHairColor") { SetCrossHairColor(); }
    if (command == "setGridColor") { SetGridColor(); }
    if (command == "textFont") { TextFont(); }
    if (command == "textSize") { TextSize(); }
    if (command == "textAngle") { TextAngle(); }
    if (command == "textBold") { TextBold(); }
    if (command == "textItalic") { TextItalic(); }
    if (command == "textUnderline") { TextUnderline(); }
    if (command == "textStrikeOut") { TextStrikeOut(); }
    if (command == "textOverline") { TextOverline(); }
    if (command == "setTextFont") { SetTextFont(); }
    if (command == "setTextSize") { SetTextSize(); }
    if (command == "setTextAngle") { SetTextAngle(); }
    if (command == "setTextBold") { SetTextBold(); }
    if (command == "setTextItalic") { SetTextItalic(); }
    if (command == "setTextUnderline") { SetTextUnderline(); }
    if (command == "setTextStrikeOut") { SetTextStrikeOut(); }
    if (command == "setTextOverline") { SetTextOverline(); }
    if (command == "previewOn") { PreviewOn(); }
    if (command == "previewOff") { PreviewOff(); }
    if (command == "vulcanize") { Vulcanize(); }
    if (command == "allowRubber") { AllowRubber(); }
    if (command == "setRubberMode") { SetRubberMode(); }
    if (command == "setRubberPoint") { SetRubberPoint(); }
    if (command == "setRubberText") { SetRubberText(); }
    if (command == "addRubber") { AddRubber(); }
    if (command == "clearRubber") { ClearRubber(); }
    if (command == "spareRubber") {
        SpareRubber();
    }
    if (command == "addTextMulti") {
        AddTextMulti();
    }
    if (command == "addTextSingle") {
        AddTextSingle();
    }
    if (command == "addInfiniteLine") {
        AddInfiniteLine();
    }
    if (command == "addRay") {
        AddRay();
    }
    if (command == "addLine") {
        AddLine();
    }
    if (command == "addTriangle") { AddTriangle();
    }
    if (command == "addRectangle") { AddRectangle();
    }
    if (command == "addRoundedRectangle") { AddRoundedRectangle(); }
    if (command == "addArc") { AddArc(); }
    if (command == "addCircle") { AddCircle(); }
    if (command == "addEllipse") { AddEllipse(); }
    if (command == "addPoint") { AddPoint(); }
    if (command == "addRegularPolygon") { AddRegularPolygon(); }
    if (command == "addPolygon") { AddPolygon(); }
    if (command == "addPolyline") { AddPolyline(); }
    if (command == "addPath") { AddPath(); }
    if (command == "addHorizontalDimension") { AddHorizontalDimension(); }
    if (command == "addVerticalDimension") { AddVerticalDimension(); }
    if (command == "addImage") { AddImage(); }
    if (command == "addDimLeader") { AddDimLeader(); }
    if (command == "setCursorShape") { SetCursorShape(); }
    if (command == "calculateAngle") { CalculateAngle(); }
    if (command == "calculateDistance") { CalculateDistance(); }
    if (command == "perpendicularDistance") { PerpendicularDistance((); }
    if (command == "numSelected") { NumSelected(); }
    if (command == "selectAll") { SelectAll(); }
    if (command == "addToSelection") {
        scriptValAddToSelection();
    }
    if (command == "clearSelection") {
        scriptValClearSelection();
    }
    if (command == "deleteSelected") {
        scriptValDeleteSelected();
    }
    if (command == "cutSelected") {
        scriptValCutSelected);
    }
    if (command == "copySelected") {
        scriptValCopySelected);
    }
    if (command == "pasteSelected") {
        scriptValPasteSelected();
    }
    if (command == "moveSelected") {
        scriptValMoveSelected();
    }
    if (command == "scaleSelected") {
        scriptValScaleSelected();
    }
    if (command == "rotateSelected") {
        scriptValRotateSelected();
    }
    if (command == "mirrorSelected") {
        scriptValMirrorSelected();
    }
    if (command == "qsnapX") {
        scriptValQSnapX();
    }
    if (command == "qsnapY") {
        scriptValQSnapY();
    }
    if (command == "mouseX") {
        scriptValMouseX();
    }
    if (command == "mouseY") {
        scriptValMouseY();
    }
    if (command == "include") {
        Include();
    }
"Alert", nativeAlert(args[0]);
"BlinkPrompt", nativeBlinkPrompt();
"SetPromptPrefix", nativeSetPromptPrefix(args[0]);
"EnablePromptRapidFire", nativeEnablePromptRapidFire();
"DisablePromptRapidFire", nativeDisablePromptRapidFire();
"EnableMoveRapidFire", nativeEnableMoveRapidFire();
"DisableMoveRapidFire", nativeDisableMoveRapidFire();
    */
    return "<br/><font color=\"red\">Unknown command \"" + command
        + "\". Press F1 for help.</font>";
}

/*
void MainWindow::LoadCommand(EmbString cmdName)
{
    qDebug("LoadCommand(%s)", qPrintable(cmdName));
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
                index += findFunc.size();
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
                        charAfter = script.at(index + i + findFunc.size());
                        if (charAfter == '(')
                        {
                            found++;
                            script.replace(index, findFunc.size(), funcName);
                        }
                        i++;
                    }
                    while(validAfterChars.contains(charAfter));
                }
                index += findFunc.size();
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
    EmbString menuName = settings.value("Menu/Name", "Lost & Found").toString();
    int menuPos = settings.value("Menu/Position",             0).toInt();
    EmbString toolbarName = settings.value("ToolBar/Name", "Lost & Found").toString();
    int toolbarPos  = settings.value("ToolBar/Position",          0).toInt();
    EmbString toolTip = settings.value("Tips/ToolTip",             "").toString();
    EmbString statusTip = settings.value("Tips/StatusTip",           "").toString();
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
}
*/

/*
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

    return "";
}
    */

/**
 * "debug": qDebug("%s", qPrintable(args(0).toString()));
 */
std::string
Error(std::vector<std::string> args)
{
    /*
    if (args.size() != 2) {
        return "ERROR: error() requires two arguments");
    }
    if (!args(0).isString()) {
        return "TYPE ERROR: error(): first argument is not a string");
    }
    if (!args(1).isString()) {
        return "TYPE ERROR: error(): second argument is not a string");
    }

    EmbString strCmd = args(0).toString();
    EmbString strErr = args(1).toString();

    mainWin()->nativeSetPromptPrefix("ERROR: (" + strCmd + ") " + strErr);
    mainWin()->nativeAppendPromptHistory(QString());
    mainWin()->nativeEndCommand();
    */
    return "";
}

std::string
Todo(std::vector<std::string> args)
{
    /*
    if (args.size() != 2)
        return "ERROR: todo() requires two arguments");
    if (!args(0).isString())
        return "TYPE ERROR: todo(): first argument is not a string");
    if (!args(1).isString())
        return "TYPE ERROR: todo(): second argument is not a string");

    EmbString strCmd  = args(0).toString();
    EmbString strTodo = args(1).toString();

    mainWin()->nativeAlert("TODO: (" + strCmd + ") " + strTodo);
    mainWin()->nativeEndCommand();
    */
    return "";
}

#if 0

std::string
AppendPromptHistory(std::vector<std::string> args)
{
    int args = args.size();
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
        return "ERROR: appendPromptHistory() requires one or zero arguments");
    }
    return "";
}


std::string
MessageBox(std::vector<std::string> args)
{
    /*
    if (args.size() != 3)    return "ERROR: messageBox() requires three arguments");
    if (!args(0).isString()) return "TYPE ERROR: messageBox(): first argument is not a string");
    if (!args(1).isString()) return "TYPE ERROR: messageBox(): second argument is not a string");
    if (!args(2).isString()) return "TYPE ERROR: messageBox(): third argument is not a string");

    EmbString type  = args(0).toString().toLower();
    EmbString title = args(1).toString();
    EmbString text  = args(2).toString();

    if (type != "critical" && type != "information" && type != "question" && type != "warning")
        return context->throwError(QScriptContext::UnknownError, "messageBox(): first argument must be \"critical\", \"information\", \"question\" or \"warning\".");

    mainWin()->nativeMessageBox(type, title, text);
    */
    return "";
}

std::string
IsInt(std::vector<std::string> args)
{
    if (args.size() != 1)    return "ERROR: isInt() requires one argument");
    if (!args(0).isNumber()) return "TYPE ERROR: isInt(): first argument is not a number");

    EmbReal num = args(0).toNumber();

    //isNaN check
    if (std::isnan(num)) return "TYPE ERROR: isInt(): first argument failed isNaN check. There is an error in your code.");

    if (fmod(num, 1) == 0)
        return std::string(true);
    return std::string(false);
}

std::string
PrintArea(std::vector<std::string> args)
{
    if (args.size() != 4)    return "ERROR: printArea() requires four arguments");
    if (!args(0).isNumber()) return "TYPE ERROR: printArea(): first argument is not a number");
    if (!args(1).isNumber()) return "TYPE ERROR: printArea(): second argument is not a number");
    if (!args(2).isNumber()) return "TYPE ERROR: printArea(): third argument is not a number");
    if (!args(3).isNumber()) return "TYPE ERROR: printArea(): fourth argument is not a number");

    EmbReal x = args(0).toNumber();
    EmbReal y = args(1).toNumber();
    EmbReal w = args(2).toNumber();
    EmbReal h = args(3).toNumber();

    //isNaN check
    if (std::isnan(x)) return "TYPE ERROR: printArea(): first argument failed isNaN check. There is an error in your code.");
    if (std::isnan(y)) return "TYPE ERROR: printArea(): second argument failed isNaN check. There is an error in your code.");
    if (std::isnan(w)) return "TYPE ERROR: printArea(): third argument failed isNaN check. There is an error in your code.");
    if (std::isnan(h)) return "TYPE ERROR: printArea(): fourth argument failed isNaN check. There is an error in your code.");

    mainWin()->nativePrintArea(x, y, w, h);
    return "";
}

std::string
SetBackgroundColor(std::vector<std::string> args)
{
    if (args.size() != 3)    return "ERROR: setBackgroundColor() requires three arguments");
    if (!args(0).isNumber()) return "TYPE ERROR: setBackgroundColor(): first argument is not a number");
    if (!args(1).isNumber()) return "TYPE ERROR: setBackgroundColor(): second argument is not a number");
    if (!args(2).isNumber()) return "TYPE ERROR: setBackgroundColor(): third argument is not a number");

    EmbReal r = args(0).toNumber();
    EmbReal g = args(1).toNumber();
    EmbReal b = args(2).toNumber();

    //isNaN check
    if (std::isnan(r)) return "TYPE ERROR: setBackgroundColor(): first argument failed isNaN check. There is an error in your code.");
    if (std::isnan(g)) return "TYPE ERROR: setBackgroundColor(): second argument failed isNaN check. There is an error in your code.");
    if (std::isnan(b)) return "TYPE ERROR: setBackgroundColor(): third argument failed isNaN check. There is an error in your code.");

    if (r < 0 || r > 255) { return context->throwError(QScriptContext::UnknownError, "setBackgroundColor(): r value must be in range 0-255"); }
    if (g < 0 || g > 255) { return context->throwError(QScriptContext::UnknownError, "setBackgroundColor(): g value must be in range 0-255"); }
    if (b < 0 || b > 255) { return context->throwError(QScriptContext::UnknownError, "setBackgroundColor(): b value must be in range 0-255"); }

    mainWin()->nativeSetBackgroundColor(r, g, b);
    return "";
}

std::string
SetCrossHairColor(std::vector<std::string> args)
{
    if (args.size() != 3)    return "ERROR: setCrossHairColor() requires three arguments");
    if (!args(0).isNumber()) return "TYPE ERROR: setCrossHairColor(): first argument is not a number");
    if (!args(1).isNumber()) return "TYPE ERROR: setCrossHairColor(): second argument is not a number");
    if (!args(2).isNumber()) return "TYPE ERROR: setCrossHairColor(): third argument is not a number");

    int r = args[0].toNumber();
    int g = args[1].toNumber();
    int b = args[2].toNumber();

    if (!validRGB(r, g, b)) {
    }

    //isNaN check
    if (std::isnan(r)) return "TYPE ERROR: setCrossHairColor(): first argument failed isNaN check. There is an error in your code.");
    if (std::isnan(g)) return "TYPE ERROR: setCrossHairColor(): second argument failed isNaN check. There is an error in your code.");
    if (std::isnan(b)) return "TYPE ERROR: setCrossHairColor(): third argument failed isNaN check. There is an error in your code.");

    if (r < 0 || r > 255) { return context->throwError(QScriptContext::UnknownError, "setCrossHairColor(): r value must be in range 0-255"); }
    if (g < 0 || g > 255) { return context->throwError(QScriptContext::UnknownError, "setCrossHairColor(): g value must be in range 0-255"); }
    if (b < 0 || b > 255) { return context->throwError(QScriptContext::UnknownError, "setCrossHairColor(): b value must be in range 0-255"); }

    mainWin()->nativeSetCrossHairColor(r, g, b);
    return "";
}

std::string
SetGridColor(std::vector<std::string> args)
{
    if (args.size() != 3)    return "ERROR: setGridColor() requires three arguments");
    if (!args(0).isNumber()) return "TYPE ERROR: setGridColor(): first argument is not a number");
    if (!args(1).isNumber()) return "TYPE ERROR: setGridColor(): second argument is not a number");
    if (!args(2).isNumber()) return "TYPE ERROR: setGridColor(): third argument is not a number");

    EmbReal r = args(0).toNumber();
    EmbReal g = args(1).toNumber();
    EmbReal b = args(2).toNumber();

    //isNaN check
    if (std::isnan(r)) return "TYPE ERROR: setGridColor(): first argument failed isNaN check. There is an error in your code.");
    if (std::isnan(g)) return "TYPE ERROR: setGridColor(): second argument failed isNaN check. There is an error in your code.");
    if (std::isnan(b)) return "TYPE ERROR: setGridColor(): third argument failed isNaN check. There is an error in your code.");

    if (r < 0 || r > 255) { return context->throwError(QScriptContext::UnknownError, "setGridColor(): r value must be in range 0-255"); }
    if (g < 0 || g > 255) { return context->throwError(QScriptContext::UnknownError, "setGridColor(): g value must be in range 0-255"); }
    if (b < 0 || b > 255) { return context->throwError(QScriptContext::UnknownError, "setGridColor(): b value must be in range 0-255"); }

    mainWin()->nativeSetGridColor(r, g, b);
    return "";
}

SetTextSize(std::vector<std::string> args)
    if (args.size() != 1)    return "ERROR: setTextSize() requires one argument");
    if (!args(0).isNumber()) return "TYPE ERROR: setTextSize(): first argument is not a number");

    EmbReal num = args(0).toNumber();

    //isNaN check
    if (std::isnan(num)) {
        return "TYPE ERROR: setTextSize(): first argument failed isNaN check. There is an error in your code.");
    }

    mainWin()->nativeSetTextSize(num);

SetTextAngle(std::vector<std::string> args)
    if (args.size() != 1)    return "ERROR: setTextAngle() requires one argument");
    if (!args(0).isNumber()) return "TYPE ERROR: setTextAngle(): first argument is not a number");

    EmbReal num = args(0).toNumber();

    //isNaN check
    if (std::isnan(num))
        return "TYPE ERROR: setTextAngle(): first argument failed isNaN check. There is an error in your code.");

    mainWin()->nativeSetTextAngle(num);

SetTextBold(std::vector<std::string> args)
    if (args.size() != 1) {
        return "ERROR: setTextBold() requires one argument");
    }
    if (!args(0).isBool()) return "TYPE ERROR: setTextBold(): first argument is not a bool");

    mainWin()->nativeSetTextBold(args(0).toBool());

SetTextItalic(std::vector<std::string> args)
    if (args.size() != 1)
        return "ERROR: setTextItalic() requires one argument");
    if (!args(0).isBool())
        return "TYPE ERROR: setTextItalic(): first argument is not a bool");

    mainWin()->nativeSetTextItalic(args[0].toBool());

SetTextUnderline(std::vector<std::string> args)
    if (args.size() != 1)
        return "ERROR: setTextUnderline() requires one argument");
    if (!args(0).isBool())
        return "TYPE ERROR: setTextUnderline(): first argument is not a bool");

    mainWin()->nativeSetTextUnderline(args(0).toBool());

SetTextStrikeOut(std::vector<std::string> args)
    if (args.size() != 1)    return "ERROR: setTextStrikeOut() requires one argument");
    if (!args(0).isBool()) return "TYPE ERROR: setTextStrikeOut(): first argument is not a bool");

    mainWin()->nativeSetTextStrikeOut(args(0).toBool());

/**
 * \brief
 */
std::string
SetTextOverline(std::vector<std::string> args)
{
    if (args.size() != 1)    return "ERROR: setTextOverline() requires one argument");
    if (!args(0).isBool()) return "TYPE ERROR: setTextOverline(): first argument is not a bool");

    mainWin()->nativeSetTextOverline(args(0).toBool());
    return "";
}

std::string
PreviewOn(std::vector<std::string> args)
{
    if (args.size() != 5)    return "ERROR: previewOn() requires five arguments");
    if (!args(0).isString()) return "TYPE ERROR: previewOn(): first argument is not a string");
    if (!args(1).isString()) return "TYPE ERROR: previewOn(): second argument is not a string");
    if (!args(2).isNumber()) return "TYPE ERROR: previewOn(): third argument is not a number");
    if (!args(3).isNumber()) return "TYPE ERROR: previewOn(): fourth argument is not a number");
    if (!args(4).isNumber()) return "TYPE ERROR: previewOn(): fifth argument is not a number");

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
        return "TYPE ERROR: previewOn(): third argument failed isNaN check. There is an error in your code.");
    if (std::isnan(y))
        return "TYPE ERROR: previewOn(): fourth argument failed isNaN check. There is an error in your code.");
    if (std::isnan(data)) {
        return context->throwError(QScriptContext::TypeError,
            "previewOn(): fifth argument failed isNaN check. There is an error in your code.");
    }

    mainWin()->nativePreviewOn(clone, mode, x, y, data);
    return "";
}

"preview off", nativePreviewOff();
"vulcanize", nativeVulcanize();
"allow rubber", nativeAllowRubber();

std::string
SetRubberMode(std::vector<std::string> args)
{
    if (args.size() != 1)    return "ERROR: setRubberMode() requires one argument");
    if (!args(0).isString()) return "TYPE ERROR: setRubberMode(): first argument is not a string");

    EmbString mode = args(0).toString().toUpper();

    if (mode == "CIRCLE_1P_RAD") {
        mainWin()->nativeSetRubberMode(OBJ_RUBBER_CIRCLE_1P_RAD);
    }
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
    if (args.size() != 3) {
        return "ERROR: setRubberPoint() requires three arguments");
    }
    if (!args(0).isString()) {
        return "TYPE ERROR: setRubberPoint(): first argument is not a string");
    }
    if (!args(1).isNumber()) {
        return "TYPE ERROR: setRubberPoint(): second argument is not a number");
    }
    if (!args(2).isNumber()) {
        return "TYPE ERROR: setRubberPoint(): third argument is not a number");
    }

    EmbString key = args(0).toString().toUpper();
    EmbReal x = args(1).toNumber();
    EmbReal y = args(2).toNumber();

    //isNaN check
    if (std::isnan(x)) {
        return "TYPE ERROR: setRubberPoint(): second argument failed isNaN check. There is an error in your code.");
    }
    if (std::isnan(y)) {
        return "TYPE ERROR: setRubberPoint(): third argument failed isNaN check. There is an error in your code.");
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
    if (args.size() != 2)    return "ERROR: setRubberText() requires two arguments");
    if (!args(0).isString()) return "TYPE ERROR: setRubberText(): first argument is not a string");
    if (!args(1).isString()) return "TYPE ERROR: setRubberText(): second argument is not a string");

    EmbString key = args(0).toString().toUpper();
    EmbString txt = args(1).toString();

    mainWin()->nativeSetRubberText(key, txt);
    return "";
}

std::string
AddRubber(std::vector<std::string> args)
{
    if (args.size() != 1)    return "ERROR: addRubber() requires one argument");
    if (!args(0).isString()) return "TYPE ERROR: addRubber(): first argument is not a string");

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

"clear rubber", nativeClearRubber();

std::string
SpareRubber(std::vector<std::string> args)
{
    if (args.size() != 1)    return "ERROR: spareRubber() requires one argument");
    if (!args(0).isString()) return "TYPE ERROR: spareRubber(): first argument is not a string");

    EmbString objID = args(0).toString().toUpper();

    if     (objID == "PATH") { mainWin()->nativeSpareRubber(SPARE_RUBBER_PATH);     }
    else if (objID == "POLYGON") { mainWin()->nativeSpareRubber(SPARE_RUBBER_POLYGON);  }
    else if (objID == "POLYLINE") { mainWin()->nativeSpareRubber(SPARE_RUBBER_POLYLINE); }
    else
    {
        bool ok = false;
        qint64 id = objID.toLongLong(&ok);
        if (!ok) return "TYPE ERROR: spareRubber(): error converting object ID into an int64");
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
    if (args.size() != 5)    return "ERROR: addTextMulti() requires five arguments");
    if (!args(0).isString()) return "TYPE ERROR: addTextMulti(): first argument is not a string");
    if (!args(1).isNumber()) return "TYPE ERROR: addTextMulti(): second argument is not a number");
    if (!args(2).isNumber()) return "TYPE ERROR: addTextMulti(): third argument is not a number");
    if (!args(3).isNumber()) return "TYPE ERROR: addTextMulti(): fourth argument is not a number");
    if (!args(4).isBool())   return "TYPE ERROR: addTextMulti(): fifth argument is not a bool");

    EmbString str   = args(0).toString();
    EmbReal   x     = args(1).toNumber();
    EmbReal   y     = args(2).toNumber();
    EmbReal   rot   = args(3).toNumber();
    bool    fill  = args(4).toBool();

    //isNaN check
    if (std::isnan(x))   return "TYPE ERROR: addTextMulti(): second argument failed isNaN check. There is an error in your code.");
    if (std::isnan(y))   return "TYPE ERROR: addTextMulti(): third argument failed isNaN check. There is an error in your code.");
    if (std::isnan(rot)) return "TYPE ERROR: addTextMulti(): fourth argument failed isNaN check. There is an error in your code.");

    mainWin()->nativeAddTextMulti(str, x, y, rot, fill, OBJ_RUBBER_OFF);
    return "";
}

std::string
AddTextSingle(std::vector<std::string> args)
{
    if (args.size() != 5)    return "ERROR: addTextSingle() requires five arguments");
    if (!args(0).isString()) return "TYPE ERROR: addTextSingle(): first argument is not a string");
    if (!args(1).isNumber()) return "TYPE ERROR: addTextSingle(): second argument is not a number");
    if (!args(2).isNumber()) return "TYPE ERROR: addTextSingle(): third argument is not a number");
    if (!args(3).isNumber()) return "TYPE ERROR: addTextSingle(): fourth argument is not a number");
    if (!args(4).isBool())   return "TYPE ERROR: addTextSingle(): fifth argument is not a bool");

    EmbString str = args[0];
    EmbReal x = args(1).toNumber();
    EmbReal y = args(2).toNumber();
    EmbReal rot = args(3).toNumber();
    bool fill  = args(4).toBool();

    //isNaN check
    if (std::isnan(x))   return "TYPE ERROR: addTextSingle(): second argument failed isNaN check. There is an error in your code.");
    if (std::isnan(y))   return "TYPE ERROR: addTextSingle(): third argument failed isNaN check. There is an error in your code.");
    if (std::isnan(rot)) return "TYPE ERROR: addTextSingle(): fourth argument failed isNaN check. There is an error in your code.");

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
    if (args.size() != 5)    return "ERROR: addLine() requires five arguments");
    if (!args(0).isNumber()) return "TYPE ERROR: addLine(): first argument is not a number");
    if (!args(1).isNumber()) return "TYPE ERROR: addLine(): second argument is not a number");
    if (!args(2).isNumber()) return "TYPE ERROR: addLine(): third argument is not a number");
    if (!args(3).isNumber()) return "TYPE ERROR: addLine(): fourth argument is not a number");
    if (!args(4).isNumber()) return "TYPE ERROR: addLine(): fifth argument is not a number");

    EmbReal x1  = args(0).toNumber();
    EmbReal y1  = args(1).toNumber();
    EmbReal x2  = args(2).toNumber();
    EmbReal y2  = args(3).toNumber();
    EmbReal rot = args(4).toNumber();

    //isNaN check
    if (std::isnan(x1))  return "TYPE ERROR: addLine(): first argument failed isNaN check. There is an error in your code.");
    if (std::isnan(y1))  return "TYPE ERROR: addLine(): second argument failed isNaN check. There is an error in your code.");
    if (std::isnan(x2))  return "TYPE ERROR: addLine(): third argument failed isNaN check. There is an error in your code.");
    if (std::isnan(y2))  return "TYPE ERROR: addLine(): fourth argument failed isNaN check. There is an error in your code.");
    if (std::isnan(rot)) return "TYPE ERROR: addLine(): fifth argument failed isNaN check. There is an error in your code.");

    mainWin()->nativeAddLine(x1, y1, x2, y2, rot, OBJ_RUBBER_OFF);
    return "";
}

/**
 * \brief
 */
std::string
AddTriangle(std::vector<std::string> args)
{
    if (args.size() != 8)    return "ERROR: addTriangle() requires eight arguments");
    if (!args(0).isNumber()) return "TYPE ERROR: addTriangle(): first argument is not a number");
    if (!args(1).isNumber()) return "TYPE ERROR: addTriangle(): second argument is not a number");
    if (!args(2).isNumber()) return "TYPE ERROR: addTriangle(): third argument is not a number");
    if (!args(3).isNumber()) return "TYPE ERROR: addTriangle(): fourth argument is not a number");
    if (!args(4).isNumber()) return "TYPE ERROR: addTriangle(): fifth argument is not a number");
    if (!args(5).isNumber()) return "TYPE ERROR: addTriangle(): sixth argument is not a number");
    if (!args(6).isNumber()) return "TYPE ERROR: addTriangle(): seventh argument is not a number");
    if (!args(7).isBool())   return "TYPE ERROR: addTriangle(): eighth argument is not a bool");

    EmbReal x1     = args(0).toNumber();
    EmbReal y1     = args(1).toNumber();
    EmbReal x2     = args(2).toNumber();
    EmbReal y2     = args(3).toNumber();
    EmbReal x3     = args(4).toNumber();
    EmbReal y3     = args(5).toNumber();
    EmbReal rot    = args(6).toNumber();
    bool  fill   = args(7).toBool();

    //isNaN check
    if (std::isnan(x1))  return "TYPE ERROR: addTriangle(): first argument failed isNaN check. There is an error in your code.");
    if (std::isnan(y1))  return "TYPE ERROR: addTriangle(): second argument failed isNaN check. There is an error in your code.");
    if (std::isnan(x2))  return "TYPE ERROR: addTriangle(): third argument failed isNaN check. There is an error in your code.");
    if (std::isnan(y2))  return "TYPE ERROR: addTriangle(): fourth argument failed isNaN check. There is an error in your code.");
    if (std::isnan(x3))  return "TYPE ERROR: addTriangle(): fifth argument failed isNaN check. There is an error in your code.");
    if (std::isnan(y3))  return "TYPE ERROR: addTriangle(): sixth argument failed isNaN check. There is an error in your code.");
    if (std::isnan(rot)) return "TYPE ERROR: addTriangle(): seventh argument failed isNaN check. There is an error in your code.");

    mainWin()->nativeAddTriangle(x1, y1, x2, y2, x3, y3, rot, fill);
    return "";
}

/**
 * \brief
 */
std::string
AddRectangle(std::vector<std::string> args)
{
    if (args.size() != 6)    return "ERROR: addRectangle() requires six arguments");
    if (!args(0).isNumber()) return "TYPE ERROR: addRectangle(): first argument is not a number");
    if (!args(1).isNumber()) return "TYPE ERROR: addRectangle(): second argument is not a number");
    if (!args(2).isNumber()) return "TYPE ERROR: addRectangle(): third argument is not a number");
    if (!args(3).isNumber()) return "TYPE ERROR: addRectangle(): fourth argument is not a number");
    if (!args(4).isNumber()) return "TYPE ERROR: addRectangle(): fifth argument is not a number");
    if (!args(5).isBool())   return "TYPE ERROR: addRectangle(): sixth argument is not a bool");

    EmbReal x    = args(0).toNumber();
    EmbReal y    = args(1).toNumber();
    EmbReal w    = args(2).toNumber();
    EmbReal h    = args(3).toNumber();
    EmbReal rot  = args(4).toNumber();
    bool  fill = args(5).toBool();

    //isNaN check
    if (std::isnan(x))   return "TYPE ERROR: addRectangle(): first argument failed isNaN check. There is an error in your code.");
    if (std::isnan(y))   return "TYPE ERROR: addRectangle(): second argument failed isNaN check. There is an error in your code.");
    if (std::isnan(w))   return "TYPE ERROR: addRectangle(): third argument failed isNaN check. There is an error in your code.");
    if (std::isnan(h))   return "TYPE ERROR: addRectangle(): fourth argument failed isNaN check. There is an error in your code.");
    if (std::isnan(rot)) return "TYPE ERROR: addRectangle(): fifth argument failed isNaN check. There is an error in your code.");

    mainWin()->nativeAddRectangle(x, y, w, h, rot, fill, OBJ_RUBBER_OFF);
    return "";
}

/**
 * \brief
 */
std::string
AddRoundedRectangle(std::vector<std::string> args)
{
    if (args.size() != 7)    return "ERROR: addRoundedRectangle() requires seven arguments");
    if (!args(0).isNumber()) return "TYPE ERROR: addRoundedRectangle(): first argument is not a number");
    if (!args(1).isNumber()) return "TYPE ERROR: addRoundedRectangle(): second argument is not a number");
    if (!args(2).isNumber()) return "TYPE ERROR: addRoundedRectangle(): third argument is not a number");
    if (!args(3).isNumber()) return "TYPE ERROR: addRoundedRectangle(): fourth argument is not a number");
    if (!args(4).isNumber()) return "TYPE ERROR: addRoundedRectangle(): fifth argument is not a number");
    if (!args(5).isNumber()) return "TYPE ERROR: addRoundedRectangle(): sixth argument is not a number");
    if (!args(6).isBool())   return "TYPE ERROR: addRoundedRectangle(): seventh argument is not a bool");

    EmbReal x    = args(0).toNumber();
    EmbReal y    = args(1).toNumber();
    EmbReal w    = args(2).toNumber();
    EmbReal h    = args(3).toNumber();
    EmbReal rad  = args(4).toNumber();
    EmbReal rot  = args(5).toNumber();
    bool  fill = args(6).toBool();

    //isNaN check
    if (std::isnan(x))   return "TYPE ERROR: addRoundedRectangle(): first argument failed isNaN check. There is an error in your code.");
    if (std::isnan(y))   return "TYPE ERROR: addRoundedRectangle(): second argument failed isNaN check. There is an error in your code.");
    if (std::isnan(w))   return "TYPE ERROR: addRoundedRectangle(): third argument failed isNaN check. There is an error in your code.");
    if (std::isnan(h))   return "TYPE ERROR: addRoundedRectangle(): fourth argument failed isNaN check. There is an error in your code.");
    if (std::isnan(rad)) return "TYPE ERROR: addRoundedRectangle(): fifth argument failed isNaN check. There is an error in your code.");
    if (std::isnan(rot)) return "TYPE ERROR: addRoundedRectangle(): sixth argument failed isNaN check. There is an error in your code.");

    mainWin()->nativeAddRoundedRectangle(x, y, w, h, rad, rot, fill);
    return "";
}

/**
 * \brief
 */
std::string
AddArc(std::vector<std::string> args)
{
    if (args.size() != 6)    return "ERROR: addArc() requires six arguments");
    if (!args(0).isNumber()) return "TYPE ERROR: addArc(): first argument is not a number");
    if (!args(1).isNumber()) return "TYPE ERROR: addArc(): second argument is not a number");
    if (!args(2).isNumber()) return "TYPE ERROR: addArc(): third argument is not a number");
    if (!args(3).isNumber()) return "TYPE ERROR: addArc(): fourth argument is not a number");
    if (!args(4).isNumber()) return "TYPE ERROR: addArc(): fifth argument is not a number");
    if (!args(5).isNumber()) return "TYPE ERROR: addArc(): sixth argument is not a number");

    EmbReal startX = args(0).toNumber();
    EmbReal startY = args(1).toNumber();
    EmbReal midX   = args(2).toNumber();
    EmbReal midY   = args(3).toNumber();
    EmbReal endX   = args(4).toNumber();
    EmbReal endY   = args(5).toNumber();

    //isNaN check
    if (std::isnan(startX)) return "TYPE ERROR: addArc(): first argument failed isNaN check. There is an error in your code.");
    if (std::isnan(startY)) return "TYPE ERROR: addArc(): second argument failed isNaN check. There is an error in your code.");
    if (std::isnan(midX))   return "TYPE ERROR: addArc(): third argument failed isNaN check. There is an error in your code.");
    if (std::isnan(midY))   return "TYPE ERROR: addArc(): fourth argument failed isNaN check. There is an error in your code.");
    if (std::isnan(endX))   return "TYPE ERROR: addArc(): fifth argument failed isNaN check. There is an error in your code.");
    if (std::isnan(endY))   return "TYPE ERROR: addArc(): sixth argument failed isNaN check. There is an error in your code.");

    mainWin()->nativeAddArc(startX, startY, midX, midY, endX, endY, OBJ_RUBBER_OFF);
    return "";
}

/**
 * \brief
 */
std::string
AddCircle(std::vector<std::string> args)
{
    if (args.size() != 4)    return "ERROR: addCircle() requires four arguments");
    if (!args(0).isNumber()) return "TYPE ERROR: addCircle(): first argument is not a number");
    if (!args(1).isNumber()) return "TYPE ERROR: addCircle(): second argument is not a number");
    if (!args(2).isNumber()) return "TYPE ERROR: addCircle(): third argument is not a number");
    if (!args(3).isBool())   return "TYPE ERROR: addCircle(): fourth argument is not a bool");

    EmbReal centerX = args(0).toNumber();
    EmbReal centerY = args(1).toNumber();
    EmbReal radius  = args(2).toNumber();
    bool  fill    = args(3).toBool();

    //isNaN check
    if (std::isnan(centerX)) return "TYPE ERROR: addCircle(): first argument failed isNaN check. There is an error in your code.");
    if (std::isnan(centerY)) return "TYPE ERROR: addCircle(): second argument failed isNaN check. There is an error in your code.");
    if (std::isnan(radius))  return "TYPE ERROR: addCircle(): third argument failed isNaN check. There is an error in your code.");

    mainWin()->nativeAddCircle(centerX, centerY, radius, fill, OBJ_RUBBER_OFF);
    return "";
}

/**
 * \brief
 */
std::string
AddSlot(std::vector<std::string> args)
{
    if (args.size() != 6)    return "ERROR: addSlot() requires six arguments");
    if (!args(0).isNumber()) return "TYPE ERROR: addSlot(): first argument is not a number");
    if (!args(1).isNumber()) return "TYPE ERROR: addSlot(): second argument is not a number");
    if (!args(2).isNumber()) return "TYPE ERROR: addSlot(): third argument is not a number");
    if (!args(3).isNumber()) return "TYPE ERROR: addSlot(): fourth argument is not a number");
    if (!args(4).isNumber()) return "TYPE ERROR: addSlot(): fifth argument is not a number");
    if (!args(5).isBool())   return "TYPE ERROR: addSlot(): sixth argument is not a bool");

    EmbReal centerX  = args(0).toNumber();
    EmbReal centerY  = args(1).toNumber();
    EmbReal diameter = args(2).toNumber();
    EmbReal length   = args(3).toNumber();
    EmbReal rot      = args(4).toNumber();
    bool  fill     = args(5).toBool();

    //isNaN check
    if (std::isnan(centerX))  return "TYPE ERROR: addSlot(): first argument failed isNaN check. There is an error in your code.");
    if (std::isnan(centerY))  return "TYPE ERROR: addSlot(): second argument failed isNaN check. There is an error in your code.");
    if (std::isnan(diameter)) return "TYPE ERROR: addSlot(): third argument failed isNaN check. There is an error in your code.");
    if (std::isnan(length))   return "TYPE ERROR: addSlot(): fourth argument failed isNaN check. There is an error in your code.");
    if (std::isnan(rot))      return "TYPE ERROR: addSlot(): fifth argument failed isNaN check. There is an error in your code.");

    mainWin()->nativeAddSlot(centerX, centerY, diameter, length, rot, fill, OBJ_RUBBER_OFF);
    return "";
}

std::string
AddEllipse(std::vector<std::string> args)
{
    if (args.size() != 6)    return "ERROR: addEllipse() requires six arguments");
    if (!args(0).isNumber()) return "TYPE ERROR: addEllipse(): first argument is not a number");
    if (!args(1).isNumber()) return "TYPE ERROR: addEllipse(): second argument is not a number");
    if (!args(2).isNumber()) return "TYPE ERROR: addEllipse(): third argument is not a number");
    if (!args(3).isNumber()) return "TYPE ERROR: addEllipse(): fourth argument is not a number");
    if (!args(4).isNumber()) return "TYPE ERROR: addEllipse(): fifth argument is not a number");
    if (!args(5).isBool())   return "TYPE ERROR: addEllipse(): sixth argument is not a bool");

    EmbReal centerX = args(0).toNumber();
    EmbReal centerY = args(1).toNumber();
    EmbReal radX    = args(2).toNumber();
    EmbReal radY    = args(3).toNumber();
    EmbReal rot     = args(4).toNumber();
    bool  fill    = args(5).toBool();

    //isNaN check
    if (std::isnan(centerX)) return "TYPE ERROR: addEllipse(): first argument failed isNaN check. There is an error in your code.");
    if (std::isnan(centerY)) return "TYPE ERROR: addEllipse(): second argument failed isNaN check. There is an error in your code.");
    if (std::isnan(radX))    return "TYPE ERROR: addEllipse(): third argument failed isNaN check. There is an error in your code.");
    if (std::isnan(radY))    return "TYPE ERROR: addEllipse(): fourth argument failed isNaN check. There is an error in your code.");
    if (std::isnan(rot))     return "TYPE ERROR: addEllipse(): fifth argument failed isNaN check. There is an error in your code.");

    mainWin()->nativeAddEllipse(centerX, centerY, radX, radY, rot, fill, OBJ_RUBBER_OFF);
    return "";
}

std::string
AddPoint(std::vector<std::string> args)
{
    if (args.size() != 2)    return "ERROR: addPoint() requires two arguments");
    if (!args(0).isNumber()) return "TYPE ERROR: addPoint(): first argument is not a number");
    if (!args(1).isNumber()) return "TYPE ERROR: addPoint(): second argument is not a number");

    EmbReal x = args(0).toNumber();
    EmbReal y = args(1).toNumber();

    //isNaN check
    if (std::isnan(x)) return "TYPE ERROR: addPoint(): first argument failed isNaN check. There is an error in your code.");
    if (std::isnan(y)) return "TYPE ERROR: addPoint(): second argument failed isNaN check. There is an error in your code.");

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
    if (args.size() != 1)   return "ERROR: addPolygon() requires one argument");
    if (!args(0).isArray()) return "TYPE ERROR: addPolygon(): first argument is not an array");

    QVariantList varList = args(0).toVariant().toList();
    int varSize = varList.size();
    if (varSize < 2) return "TYPE ERROR: addPolygon(): array must contain at least two elements");
    if (varSize % 2) return "TYPE ERROR: addPolygon(): array cannot contain an odd number of elements");

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
            return "TYPE ERROR: addPolygon(): array contains one or more invalid elements");
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
    if (args.size() != 1)   return "ERROR: addPolyline() requires one argument");
    if (!args(0).isArray()) return "TYPE ERROR: addPolyline(): first argument is not an array");

    QVariantList varList = args(0).toVariant().toList();
    int varSize = varList.size();
    if (varSize < 2) return "TYPE ERROR: addPolyline(): array must contain at least two elements");
    if (varSize % 2) return "TYPE ERROR: addPolyline(): array cannot contain an odd number of elements");

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
            return "TYPE ERROR: addPolyline(): array contains one or more invalid elements");
    }

    path.translate(-startX, -startY);

    mainWin()->nativeAddPolyline(startX, startY, path, OBJ_RUBBER_OFF);
    return "";
}

AddPath(std::vector<std::string> args)
    //TODO: parameter error checking
    qDebug("TODO: finish addPath command");

AddHorizontalDimension(std::vector<std::string> args)
    //TODO: parameter error checking
    qDebug("TODO: finish addHorizontalDimension command");

AddVerticalDimension(std::vector<std::string> args)
    //TODO: parameter error checking
    qDebug("TODO: finish addVerticalDimension command");

AddImage(std::vector<std::string> args)
    //TODO: parameter error checking
    qDebug("TODO: finish addImage command");

/**
 *
 */
std::string
AddDimLeader(std::vector<std::string> args)
{
    if (args.size() != 5)    return "ERROR: addDimLeader() requires five arguments");
    if (!args(0).isNumber()) return "TYPE ERROR: addDimLeader(): first argument is not a number");
    if (!args(1).isNumber()) return "TYPE ERROR: addDimLeader(): second argument is not a number");
    if (!args(2).isNumber()) return "TYPE ERROR: addDimLeader(): third argument is not a number");
    if (!args(3).isNumber()) return "TYPE ERROR: addDimLeader(): fourth argument is not a number");
    if (!args(4).isNumber()) return "TYPE ERROR: addDimLeader(): fifth argument is not a number");

    EmbReal x1  = args(0).toNumber();
    EmbReal y1  = args(1).toNumber();
    EmbReal x2  = args(2).toNumber();
    EmbReal y2  = args(3).toNumber();
    EmbReal rot = args(4).toNumber();

    //isNaN check
    if (std::isnan(x1))  return "TYPE ERROR: addDimLeader(): first argument failed isNaN check. There is an error in your code.");
    if (std::isnan(y1))  return "TYPE ERROR: addDimLeader(): second argument failed isNaN check. There is an error in your code.");
    if (std::isnan(x2))  return "TYPE ERROR: addDimLeader(): third argument failed isNaN check. There is an error in your code.");
    if (std::isnan(y2))  return "TYPE ERROR: addDimLeader(): fourth argument failed isNaN check. There is an error in your code.");
    if (std::isnan(rot)) return "TYPE ERROR: addDimLeader(): fifth argument failed isNaN check. There is an error in your code.");

    mainWin()->nativeAddDimLeader(x1, y1, x2, y2, rot, OBJ_RUBBER_OFF);
    return "";
}

/**
 *
 */
std::string
SetCursorShape(std::vector<std::string> args)
{
    if (args.size() != 1)    return "ERROR: setCursorShape() requires one argument");
    if (!args(0).isString()) return "TYPE ERROR: setCursorShape(): first argument is not a string");

    EmbString shape = args(0).toString();
    mainWin()->nativeSetCursorShape(shape);
    return "";
}

/**
 *
 */
std::string
CalculateAngle(std::vector<std::string> args)
{
    if (args.size() != 4)    return "ERROR: calculateAngle() requires four arguments");
    if (!args(0).isNumber()) return "TYPE ERROR: calculateAngle(): first argument is not a number");
    if (!args(1).isNumber()) return "TYPE ERROR: calculateAngle(): second argument is not a number");
    if (!args(2).isNumber()) return "TYPE ERROR: calculateAngle(): third argument is not a number");
    if (!args(3).isNumber()) return "TYPE ERROR: calculateAngle(): fourth argument is not a number");

    EmbReal x1 = args(0).toNumber();
    EmbReal y1 = args(1).toNumber();
    EmbReal x2 = args(2).toNumber();
    EmbReal y2 = args(3).toNumber();

    //isNaN check
    if (std::isnan(x1))  return "TYPE ERROR: calculateAngle(): first argument failed isNaN check. There is an error in your code.");
    if (std::isnan(y1))  return "TYPE ERROR: calculateAngle(): second argument failed isNaN check. There is an error in your code.");
    if (std::isnan(x2))  return "TYPE ERROR: calculateAngle(): third argument failed isNaN check. There is an error in your code.");
    if (std::isnan(y2))  return "TYPE ERROR: calculateAngle(): fourth argument failed isNaN check. There is an error in your code.");

    return std::string(mainWin()->nativeCalculateAngle(x1, y1, x2, y2));
}

/**
 *
 */
std::string
CalculateDistance(std::vector<std::string> args)
{
    if (args.size() != 4)    return "ERROR: calculateDistance() requires four arguments");
    if (!args(0).isNumber()) return "TYPE ERROR: calculateDistance(): first argument is not a number");
    if (!args(1).isNumber()) return "TYPE ERROR: calculateDistance(): second argument is not a number");
    if (!args(2).isNumber()) return "TYPE ERROR: calculateDistance(): third argument is not a number");
    if (!args(3).isNumber()) return "TYPE ERROR: calculateDistance(): fourth argument is not a number");

    EmbReal x1 = args(0).toNumber();
    EmbReal y1 = args(1).toNumber();
    EmbReal x2 = args(2).toNumber();
    EmbReal y2 = args(3).toNumber();

    //isNaN check
    if (std::isnan(x1))  return "TYPE ERROR: calculateDistance(): first argument failed isNaN check. There is an error in your code.");
    if (std::isnan(y1))  return "TYPE ERROR: calculateDistance(): second argument failed isNaN check. There is an error in your code.");
    if (std::isnan(x2))  return "TYPE ERROR: calculateDistance(): third argument failed isNaN check. There is an error in your code.");
    if (std::isnan(y2))  return "TYPE ERROR: calculateDistance(): fourth argument failed isNaN check. There is an error in your code.");

    return std::string(mainWin()->nativeCalculateDistance(x1, y1, x2, y2));
}

/**
 *
 */
std::string
PerpendicularDistance(std::vector<std::string> args)
{
    if (args.size() != 6)    return "ERROR: perpendicularDistance() requires six arguments");
    if (!args(0).isNumber()) return "TYPE ERROR: perpendicularDistance(): first argument is not a number");
    if (!args(1).isNumber()) return "TYPE ERROR: perpendicularDistance(): second argument is not a number");
    if (!args(2).isNumber()) return "TYPE ERROR: perpendicularDistance(): third argument is not a number");
    if (!args(3).isNumber()) return "TYPE ERROR: perpendicularDistance(): fourth argument is not a number");
    if (!args(4).isNumber()) return "TYPE ERROR: perpendicularDistance(): fifth argument is not a number");
    if (!args(5).isNumber()) return "TYPE ERROR: perpendicularDistance(): sixth argument is not a number");

    EmbReal px = args(0).toNumber();
    EmbReal py = args(1).toNumber();
    EmbReal x1 = args(2).toNumber();
    EmbReal y1 = args(3).toNumber();
    EmbReal x2 = args(4).toNumber();
    EmbReal y2 = args(5).toNumber();

    //isNaN check
    if (std::isnan(px))  return "TYPE ERROR: perpendicularDistance(): first argument failed isNaN check. There is an error in your code.");
    if (std::isnan(py))  return "TYPE ERROR: perpendicularDistance(): second argument failed isNaN check. There is an error in your code.");
    if (std::isnan(x1))  return "TYPE ERROR: perpendicularDistance(): third argument failed isNaN check. There is an error in your code.");
    if (std::isnan(y1))  return "TYPE ERROR: perpendicularDistance(): fourth argument failed isNaN check. There is an error in your code.");
    if (std::isnan(x2))  return "TYPE ERROR: perpendicularDistance(): fifth argument failed isNaN check. There is an error in your code.");
    if (std::isnan(y2))  return "TYPE ERROR: perpendicularDistance(): sixth argument failed isNaN check. There is an error in your code.");

    return std::string(mainWin()->nativePerpendicularDistance(px, py, x1, y1, x2, y2));
}

/**
 *
 */
std::string
CutSelected(std::vector<std::string> args)
{
    if (args.size() != 2)    return "ERROR: cutSelected() requires two arguments");
    if (!args(0).isNumber()) return "TYPE ERROR: cutSelected(): first argument is not a number");
    if (!args(1).isNumber()) return "TYPE ERROR: cutSelected(): second argument is not a number");

    EmbReal x = args(0).toNumber();
    EmbReal y = args(1).toNumber();

    //isNaN check
    if (std::isnan(x)) return "TYPE ERROR: cutSelected(): first argument failed isNaN check. There is an error in your code.");
    if (std::isnan(y)) return "TYPE ERROR: cutSelected(): second argument failed isNaN check. There is an error in your code.");

    mainWin()->nativeCutSelected(x, y);
    return "";
}

/**
 *
 */
std::string
CopySelected(std::vector<std::string> args)
{
    if (args.size() != 2)    return "ERROR: copySelected() requires two arguments");
    if (!args(0).isNumber()) return "TYPE ERROR: copySelected(): first argument is not a number");
    if (!args(1).isNumber()) return "TYPE ERROR: copySelected(): second argument is not a number");

    EmbReal x = args(0).toNumber();
    EmbReal y = args(1).toNumber();

    //isNaN check
    if (std::isnan(x)) return "TYPE ERROR: copySelected(): first argument failed isNaN check. There is an error in your code.");
    if (std::isnan(y)) return "TYPE ERROR: copySelected(): second argument failed isNaN check. There is an error in your code.");

    mainWin()->nativeCopySelected(x, y);
    return "";
}

/**
 *
 */
std::string
PasteSelected(std::vector<std::string> args)
{
    if (args.size() != 2)    return "ERROR: pasteSelected() requires two arguments");
    if (!args(0).isNumber()) return "TYPE ERROR: pasteSelected(): first argument is not a number");
    if (!args(1).isNumber()) return "TYPE ERROR: pasteSelected(): second argument is not a number");

    EmbReal x = args(0).toNumber();
    EmbReal y = args(1).toNumber();

    //isNaN check
    if (std::isnan(x)) return "TYPE ERROR: pasteSelected(): first argument failed isNaN check. There is an error in your code.");
    if (std::isnan(y)) return "TYPE ERROR: pasteSelected(): second argument failed isNaN check. There is an error in your code.");

    mainWin()->nativePasteSelected(x, y);
    return "";
}

/**
 *
 */
std::string
MoveSelected(std::vector<std::string> args)
{
    if (args.size() != 2)    return "ERROR: moveSelected() requires two arguments");
    if (!args(0).isNumber()) return "TYPE ERROR: moveSelected(): first argument is not a number");
    if (!args(1).isNumber()) return "TYPE ERROR: moveSelected(): second argument is not a number");

    EmbReal dx = args(0).toNumber();
    EmbReal dy = args(1).toNumber();

    //isNaN check
    if (std::isnan(dx)) return "TYPE ERROR: moveSelected(): first argument failed isNaN check. There is an error in your code.");
    if (std::isnan(dy)) return "TYPE ERROR: moveSelected(): second argument failed isNaN check. There is an error in your code.");

    mainWin()->nativeMoveSelected(dx, dy);
    return "";
}

/**
 *
 */
std::string
ScaleSelected(std::vector<std::string> args)
{
    if (args.size() != 3)    return "ERROR: scaleSelected() requires three arguments");
    if (!args(0).isNumber()) return "TYPE ERROR: scaleSelected(): first argument is not a number");
    if (!args(1).isNumber()) return "TYPE ERROR: scaleSelected(): second argument is not a number");
    if (!args(2).isNumber()) return "TYPE ERROR: scaleSelected(): third argument is not a number");

    EmbReal x      = args(0).toNumber();
    EmbReal y      = args(1).toNumber();
    EmbReal factor = args(2).toNumber();

    //isNaN check
    if (std::isnan(x))      return "TYPE ERROR: scaleSelected(): first argument failed isNaN check. There is an error in your code.");
    if (std::isnan(y))      return "TYPE ERROR: scaleSelected(): second argument failed isNaN check. There is an error in your code.");
    if (std::isnan(factor)) return "TYPE ERROR: scaleSelected(): third argument failed isNaN check. There is an error in your code.");

    if (factor <= 0.0) return context->throwError(QScriptContext::UnknownError, "scaleSelected(): scale factor must be greater than zero");

    mainWin()->nativeScaleSelected(x, y, factor);
    return "";
}

std::string
RotateSelected(std::vector<std::string> args)
{
    if (args.size() != 3)    return "ERROR: rotateSelected() requires three arguments");
    if (!args(0).isNumber()) return "TYPE ERROR: rotateSelected(): first argument is not a number");
    if (!args(1).isNumber()) return "TYPE ERROR: rotateSelected(): second argument is not a number");
    if (!args(2).isNumber()) return "TYPE ERROR: rotateSelected(): third argument is not a number");

    EmbReal x   = args(0).toNumber();
    EmbReal y   = args(1).toNumber();
    EmbReal rot = args(2).toNumber();

    //isNaN check
    if (std::isnan(x))   return "TYPE ERROR: rotateSelected(): first argument failed isNaN check. There is an error in your code.");
    if (std::isnan(y))   return "TYPE ERROR: rotateSelected(): second argument failed isNaN check. There is an error in your code.");
    if (std::isnan(rot)) return "TYPE ERROR: rotateSelected(): third argument failed isNaN check. There is an error in your code.");

    mainWin()->nativeRotateSelected(x, y, rot);
    return "";
}

/**
 * \brief
 */
std::string
MirrorSelected(std::vector<std::string> args)
{
    if (args.size() != 4)
        return "ERROR: mirrorSelected() requires four arguments";
    if (!args(0).isNumber())
        return "TYPE ERROR: mirrorSelected(): first argument is not a number";
    if (!args(1).isNumber())
        return "TYPE ERROR: mirrorSelected(): second argument is not a number";
    if (!args(2).isNumber())
        return "TYPE ERROR: mirrorSelected(): third argument is not a number";
    if (!args(3).isNumber())
        return "TYPE ERROR: mirrorSelected(): fourth argument is not a number";

    EmbReal x1 = args(0).toNumber();
    EmbReal y1 = args(1).toNumber();
    EmbReal x2 = args(2).toNumber();
    EmbReal y2 = args(3).toNumber();

    //isNaN check
    if (std::isnan(x1))
        return "TYPE ERROR: mirrorSelected(): first argument failed isNaN check. There is an error in your code.";
    if (std::isnan(y1))
        return "TYPE ERROR: mirrorSelected(): second argument failed isNaN check. There is an error in your code.";
    if (std::isnan(x2))
        return "TYPE ERROR: mirrorSelected(): third argument failed isNaN check. There is an error in your code.";
    if (std::isnan(y2))
        return "TYPE ERROR: mirrorSelected(): fourth argument failed isNaN check. There is an error in your code.";

    mainWin()->nativeMirrorSelected(x1, y1, x2, y2);
    return "";
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
    for (int i = 0; i < settings_opensave_recent_list_of_files.size(); ++i) {
        //If less than the max amount of entries add to menu
        if (i < settings_opensave_recent_max_files) {
            recentFileInfo = QFileInfo(settings_opensave_recent_list_of_files.at(i));
            if (recentFileInfo.exists() && validFileFormat(recentFileInfo.fileName())) {
                recentValue.setNum(i+1);
                QAction* rAction;
                if     (recentValue.toInt() >= 1 && recentValue.toInt() <= 9)
                    rAction = new QAction("&" + recentValue + " " + recentFileInfo.fileName(), this);
                else if (recentValue.toInt() == 10)
                    rAction = new QAction("1&0 " + recentFileInfo.fileName(), this);
                else
                    rAction = new QAction(recentValue + " " + recentFileInfo.fileName(), this);
                rAction->setCheckable(false);
                rAction->setData(settings_opensave_recent_list_of_files.at(i));
                recentMenu->addAction(rAction);
                connect(rAction, SIGNAL(triggered()), this, SLOT(openrecentfile()));
            }
        }
    }
    //Ensure the list only has max amount of entries
    while (settings_opensave_recent_list_of_files.size() > settings_opensave_recent_max_files) {
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
    if (recent) {
        files.append(recentFile);
        openFilesSelected(files);
    }
    else {
        if (!preview) {
            //TODO: set getOpenFileNames' selectedFilter parameter from settings_opensave_open_format
            files = QFileDialog::getOpenFileNames(this, tr("Open"), openFilesPath, formatFilterOpen);
            openFilesSelected(files);
        }
        else {
            PreviewDialog* openDialog = new PreviewDialog(this, tr("Open w/Preview"), openFilesPath, formatFilterOpen);
            //TODO: set openDialog->selectNameFilter(const QString& filter) from settings_opensave_open_format
            connect(openDialog, SIGNAL(filesSelected(const QStringList&)), this, SLOT(openFilesSelected(const QStringList&)));
            openDialog->exec();
        }
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

#if 0

//Command: Circle

var global = {}; //Required
global.x1;
global.y1;
global.x2;
global.y2;
global.x3;
global.y3;
global.rad;
global.dia;
global.cx;
global.cy;
global.mode;

//enums
global.mode_1P_RAD = 0;
global.mode_1P_DIA = 1;
global.mode_2P     = 2;
global.mode_3P     = 3;
global.mode_TTR    = 4;

/**
 *
 */
void
circle_main(void)
{
    initCommand();
    clearSelection();
    global.mode = global.mode_1P_RAD;
    global.x1 = NaN;
    global.y1 = NaN;
    global.x2 = NaN;
    global.y2 = NaN;
    global.x3 = NaN;
    global.y3 = NaN;
    setPromptPrefix(qsTr("Specify center point for circle or [3P/2P/Ttr (tan tan radius)]: "));
}

/**
 * .
 */
void circle_click(float x, float y)
{
    if (global.mode == global.mode_1P_RAD) {
        if (std::isnan(global.x1)) {
            global.x1 = x;
            global.y1 = y;
            global.cx = x;
            global.cy = y;
            addRubber("CIRCLE");
            setRubberMode("CIRCLE_1P_RAD");
            setRubberPoint("CIRCLE_CENTER", global.cx, global.cy);
            appendPromptHistory();
            setPromptPrefix(qsTr("Specify radius of circle or [Diameter]: "));
        }
        else {
            global.x2 = x;
            global.y2 = y;
            setRubberPoint("CIRCLE_RADIUS", global.x2, global.y2);
            vulcanize();
            appendPromptHistory();
            endCommand();
        }
    }
    else if (global.mode == global.mode_1P_DIA) {
        if (std::isnan(global.x1)) {
            error("CIRCLE", qsTr("This should never happen."));
        }
        else {
            global.x2 = x;
            global.y2 = y;
            setRubberPoint("CIRCLE_DIAMETER", global.x2, global.y2);
            vulcanize();
            appendPromptHistory();
            endCommand();
        }
    }
    else if (global.mode == global.mode_2P) {
        if (std::isnan(global.x1)) {
            global.x1 = x;
            global.y1 = y;
            addRubber("CIRCLE");
            setRubberMode("CIRCLE_2P");
            setRubberPoint("CIRCLE_TAN1", global.x1, global.y1);
            appendPromptHistory();
            setPromptPrefix(qsTr("Specify second end point of circle's diameter: "));
        }
        else if (std::isnan(global.x2)) {
            global.x2 = x;
            global.y2 = y;
            setRubberPoint("CIRCLE_TAN2", global.x2, global.y2);
            vulcanize();
            appendPromptHistory();
            endCommand();
        }
        else
        {
            error("CIRCLE", qsTr("This should never happen."));
        }
    }
    else if (global.mode == global.mode_3P)
    {
        if (std::isnan(global.x1))
        {
            global.x1 = x;
            global.y1 = y;
            appendPromptHistory();
            setPromptPrefix(qsTr("Specify second point on circle: "));
        }
        else if (std::isnan(global.x2))
        {
            global.x2 = x;
            global.y2 = y;
            addRubber("CIRCLE");
            setRubberMode("CIRCLE_3P");
            setRubberPoint("CIRCLE_TAN1", global.x1, global.y1);
            setRubberPoint("CIRCLE_TAN2", global.x2, global.y2);
            appendPromptHistory();
            setPromptPrefix(qsTr("Specify third point on circle: "));
        }
        else if (std::isnan(global.x3))
        {
            global.x3 = x;
            global.y3 = y;
            setRubberPoint("CIRCLE_TAN3", global.x3, global.y3);
            vulcanize();
            appendPromptHistory();
            endCommand();
        }
        else
        {
            error("CIRCLE", qsTr("This should never happen."));
        }
    }
    else if (global.mode == global.mode_TTR)
    {
        if (std::isnan(global.x1))
        {
            global.x1 = x;
            global.y1 = y;
            appendPromptHistory();
            setPromptPrefix(qsTr("Specify point on object for second tangent of circle: "));
        }
        else if (std::isnan(global.x2))
        {
            global.x2 = x;
            global.y2 = y;
            appendPromptHistory();
            setPromptPrefix(qsTr("Specify radius of circle: "));
        }
        else if (std::isnan(global.x3))
        {
            global.x3 = x;
            global.y3 = y;
            appendPromptHistory();
            setPromptPrefix(qsTr("Specify second point: "));
        }
        else
        {
            todo("CIRCLE", "click() for TTR");
        }
    }
}

/**
 * .
 */
void
circle_context(std::string str)
{
    todo("CIRCLE", "context()");
}

/**
 * .
 */
std::string
circle_prompt(std::string str)
{
    if (global.mode == global.mode_1P_RAD) {
        if (std::isnan(global.x1)) {
            if (str == "2P") {
                // TODO: Probably should add additional qsTr calls here.
                global.mode = global.mode_2P;
                setPromptPrefix(qsTr("Specify first end point of circle's diameter: "));
            }
            else if (str == "3P") {
                //TODO: Probably should add additional qsTr calls here.
                global.mode = global.mode_3P;
                setPromptPrefix(qsTr("Specify first point of circle: "));
            }
            else if (str == "T" || str == "TTR") //TODO: Probably should add additional qsTr calls here.
            {
                global.mode = global.mode_TTR;
                setPromptPrefix(qsTr("Specify point on object for first tangent of circle: "));
            }
            else
            {
                var strList = str.split(",");
                if (std::isnan(strList[0]) || isNaN(strList[1]))
                {
                    alert(qsTr("Point or option keyword required."));
                    setPromptPrefix(qsTr("Specify center point for circle or [3P/2P/Ttr (tan tan radius)]: "));
                }
                else
                {
                    global.x1 = Number(strList[0]);
                    global.y1 = Number(strList[1]);
                    global.cx = global.x1;
                    global.cy = global.y1;
                    addRubber("CIRCLE");
                    setRubberMode("CIRCLE_1P_RAD");
                    setRubberPoint("CIRCLE_CENTER", global.cx, global.cy);
                    setPromptPrefix(qsTr("Specify radius of circle or [Diameter]: "));
                }
            }
        }
        else
        {
            if (str == "D" || str == "DIAMETER") //TODO: Probably should add additional qsTr calls here.
            {
                global.mode = global.mode_1P_DIA;
                setRubberMode("CIRCLE_1P_DIA");
                setPromptPrefix(qsTr("Specify diameter of circle: "));
            }
            else {
                var num = Number(str);
                if (std::isnan(num)) {
                    alert(qsTr("Requires numeric radius, point on circumference, or \"D\"."));
                    setPromptPrefix(qsTr("Specify radius of circle or [Diameter]: "));
                }
                else {
                    global.rad = num;
                    global.x2 = global.x1 + global.rad;
                    global.y2 = global.y1;
                    setRubberPoint("CIRCLE_RADIUS", global.x2, global.y2);
                    vulcanize();
                    endCommand();
                }
            }
        }
    }
    else if (global.mode == global.mode_1P_DIA)
    {
        if (std::isnan(global.x1)) {
            error("CIRCLE", qsTr("This should never happen."));
        }
        if (std::isnan(global.x2)) {
            var num = Number(str);
            if (std::isnan(num)) {
                alert(qsTr("Requires numeric distance or second point."));
                setPromptPrefix(qsTr("Specify diameter of circle: "));
            }
            else {
                global.dia = num;
                global.x2 = global.x1 + global.dia;
                global.y2 = global.y1;
                setRubberPoint("CIRCLE_DIAMETER", global.x2, global.y2);
                vulcanize();
                endCommand();
            }
        }
        else {
            error("CIRCLE", qsTr("This should never happen."));
        }
    }
    else if (global.mode == global.mode_2P) {
        if (std::isnan(global.x1))
        {
            var strList = str.split(",");
            if (std::isnan(strList[0]) || isNaN(strList[1]))
            {
                alert(qsTr("Invalid point."));
                setPromptPrefix(qsTr("Specify first end point of circle's diameter: "));
            }
            else
            {
                global.x1 = Number(strList[0]);
                global.y1 = Number(strList[1]);
                addRubber("CIRCLE");
                setRubberMode("CIRCLE_2P");
                setRubberPoint("CIRCLE_TAN1", global.x1, global.y1);
                setPromptPrefix(qsTr("Specify second end point of circle's diameter: "));
            }
        }
        else if (std::isnan(global.x2)) {
            var strList = str.split(",");
            if (std::isnan(strList[0]) || isNaN(strList[1])) {
                alert(qsTr("Invalid point."));
                setPromptPrefix(qsTr("Specify second end point of circle's diameter: "));
            }
            else {
                global.x2 = Number(strList[0]);
                global.y2 = Number(strList[1]);
                setRubberPoint("CIRCLE_TAN2", global.x2, global.y2);
                vulcanize();
                endCommand();
            }
        }
        else {
            error("CIRCLE", qsTr("This should never happen."));
        }
    }
    else if (global.mode == global.mode_3P) {
        if (std::isnan(global.x1)) {
            var strList = str.split(",");
            if (std::isnan(strList[0]) || isNaN(strList[1]))
            {
                alert(qsTr("Invalid point."));
                setPromptPrefix(qsTr("Specify first point of circle: "));
            }
            else
            {
                global.x1 = Number(strList[0]);
                global.y1 = Number(strList[1]);
                setPromptPrefix(qsTr("Specify second point of circle: "));
            }
        }
        else if (std::isnan(global.x2)) {
            var strList = str.split(",");
            if (std::isnan(strList[0]) || isNaN(strList[1])) {
                alert(qsTr("Invalid point."));
                setPromptPrefix(qsTr("Specify second point of circle: "));
            }
            else {
                global.x2 = Number(strList[0]);
                global.y2 = Number(strList[1]);
                addRubber("CIRCLE");
                setRubberMode("CIRCLE_3P");
                setRubberPoint("CIRCLE_TAN1", global.x1, global.y1);
                setRubberPoint("CIRCLE_TAN2", global.x2, global.y2);
                setPromptPrefix(qsTr("Specify third point of circle: "));
            }
        }
        else if (std::isnan(global.x3)) {
            var strList = str.split(",");
            if (std::isnan(strList[0]) || isNaN(strList[1])) {
                alert(qsTr("Invalid point."));
                setPromptPrefix(qsTr("Specify third point of circle: "));
            }
            else {
                global.x3 = Number(strList[0]);
                global.y3 = Number(strList[1]);
                setRubberPoint("CIRCLE_TAN3", global.x3, global.y3);
                vulcanize();
                endCommand();
            }
        }
        else {
            error("CIRCLE", qsTr("This should never happen."));
        }
    }
    else if (global.mode == global.mode_TTR) {
        todo("CIRCLE", "prompt() for TTR");
    }
}

//Command: Distance

var global = {}; //Required
global.x1;
global.y1;
global.x2;
global.y2;

void distance_main()
{
    initCommand();
    clearSelection();
    global.x1 = NaN;
    global.y1 = NaN;
    global.x2 = NaN;
    global.y2 = NaN;
    setPromptPrefix(qsTr("Specify first point: "));
}


void distance_click(x, y)
{
    if (std::isnan(global.x1)) {
        global.x1 = x;
        global.y1 = y;
        addRubber("LINE");
        setRubberMode("LINE");
        setRubberPoint("LINE_START", global.x1, global.y1);
        appendPromptHistory();
        setPromptPrefix(qsTr("Specify second point: "));
    }
    else {
        appendPromptHistory();
        global.x2 = x;
        global.y2 = y;
        reportDistance();
        endCommand();
    }
}

void distance_context(str)
{
    todo("DISTANCE", "context()");
}

void prompt(str)
{
    var strList = str.split(",");
    if (std::isnan(global.x1)) {
        if (std::isnan(strList[0]) || isNaN(strList[1])) {
            alert(qsTr("Requires numeric distance or two points."));
            setPromptPrefix(qsTr("Specify first point: "));
        }
        else {
            global.x1 = Number(strList[0]);
            global.y1 = Number(strList[1]);
            addRubber("LINE");
            setRubberMode("LINE");
            setRubberPoint("LINE_START", global.x1, global.y1);
            setPromptPrefix(qsTr("Specify second point: "));
        }
    }
    else {
        if (std::isnan(strList[0]) || isNaN(strList[1])) {
            alert(qsTr("Requires numeric distance or two points."));
            setPromptPrefix(qsTr("Specify second point: "));
        }
        else {
            global.x2 = Number(strList[0]);
            global.y2 = Number(strList[1]);
            reportDistance();
            endCommand();
        }
    }
}

/**
 * Cartesian Coordinate System reported:
 *
 *               (+)
 *               90
 *               |
 *      (-) 180__|__0 (+)
 *               |
 *              270
 *              (-)
 */
void reportDistance()
{
    var dx = global.x2 - global.x1;
    var dy = global.y2 - global.y1;

    var dist = calculateDistance(global.x1,global.y1,global.x2, global.y2);
    var angle = calculateAngle(global.x1,global.y1,global.x2, global.y2);

    setPromptPrefix(qsTr("Distance") + " = " + dist.toString() + ", " + qsTr("Angle") + " = " + angle.toString());
    appendPromptHistory();
    setPromptPrefix(qsTr("Delta X") + " = " + dx.toString() + ", " + qsTr("Delta Y") + " = " + dy.toString());
    appendPromptHistory();
}

//Command: Dolphin

var global = {}; //Required
global.numPoints = 512; //Default //TODO: min:64 max:8192
global.cx;
global.cy;
global.sx = 0.04; //Default
global.sy = 0.04; //Default
global.numPoints;
global.mode;

//enums
global.mode_NUM_POINTS = 0;
global.mode_XSCALE     = 1;
global.mode_YSCALE     = 2;

/**
 *
 */
void
dolphin_main(void)
{
    initCommand();
    clearSelection();
    global.cx = NaN;
    global.cy = NaN;
    global.mode = global.mode_NUM_POINTS;

    addRubber("POLYGON");
    setRubberMode("POLYGON");
    updateDolphin(global.numPoints, global.sx, global.sy);
    spareRubber("POLYGON");
    endCommand();
}

/**
 * .
 */
void
updateDolphin(int numPts, EmbReal xScale, EmbReal yScale)
{
    var i;
    var t;
    var xx = NaN;
    var yy = NaN;
    var two_pi = 2*PI;

    for(i = 0; i <= numPts; i++)
    {
        t = two_pi/numPts*i; 

        xx = 4/23*sin(62/33-58*t)+
        8/11*sin(10/9-56*t)+
        17/24*sin(38/35-55*t)+
        30/89*sin(81/23-54*t)+
        3/17*sin(53/18-53*t)+
        21/38*sin(29/19-52*t)+
        11/35*sin(103/40-51*t)+
        7/16*sin(79/18-50*t)+
        4/15*sin(270/77-49*t)+
        19/35*sin(59/27-48*t)+
        37/43*sin(71/17-47*t)+
        sin(18/43-45*t)+
        21/26*sin(37/26-44*t)+
        27/19*sin(111/32-42*t)+
        8/39*sin(13/25-41*t)+
        23/30*sin(27/8-40*t)+
        23/21*sin(32/35-37*t)+
        18/37*sin(91/31-36*t)+
        45/22*sin(29/37-35*t)+
        56/45*sin(11/8-33*t)+
        4/7*sin(32/19-32*t)+
        54/23*sin(74/29-31*t)+
        28/19*sin(125/33-30*t)+
        19/9*sin(73/27-29*t)+
        16/17*sin(737/736-28*t)+
        52/33*sin(130/29-27*t)+
        41/23*sin(43/30-25*t)+
        29/20*sin(67/26-24*t)+
        64/25*sin(136/29-23*t)+
        162/37*sin(59/34-21*t)+
        871/435*sin(199/51-20*t)+
        61/42*sin(58/17-19*t)+
        159/25*sin(77/31-17*t)+
        241/15*sin(94/31-13*t)+
        259/18*sin(114/91-12*t)+
        356/57*sin(23/25-11*t)+
        2283/137*sin(23/25-10*t)+
        1267/45*sin(139/42-9*t)+
        613/26*sin(41/23-8*t)+
        189/16*sin(122/47-6*t)+
        385/6*sin(151/41-5*t)+
        2551/38*sin(106/35-4*t)+
        1997/18*sin(6/5-2*t)+
        43357/47*sin(81/26-t)-
        4699/35*sin(3*t+25/31)-
        1029/34*sin(7*t+20/21)-
        250/17*sin(14*t+7/40)-
        140/17*sin(15*t+14/25)-
        194/29*sin(16*t+29/44)-
        277/52*sin(18*t+37/53)-
        94/41*sin(22*t+33/31)-
        57/28*sin(26*t+44/45)-
        128/61*sin(34*t+11/14)-
        111/95*sin(38*t+55/37)-
        85/71*sin(39*t+4/45)-
        25/29*sin(43*t+129/103)-
        7/37*sin(46*t+9/20)-
        17/32*sin(57*t+11/28)-
        5/16*sin(59*t+32/39);

        yy = 5/11*sin(163/37-59*t)+
        7/22*sin(19/41-58*t)+
        30/41*sin(1-57*t)+
        37/29*sin(137/57-56*t)+
        5/7*sin(17/6-55*t)+
        11/39*sin(46/45-52*t)+
        25/28*sin(116/83-51*t)+
        25/34*sin(11/20-47*t)+
        8/27*sin(81/41-46*t)+
        44/39*sin(78/37-45*t)+
        11/25*sin(107/37-44*t)+
        7/20*sin(7/16-41*t)+
        30/31*sin(19/5-40*t)+
        37/27*sin(148/59-39*t)+
        44/39*sin(17/27-38*t)+
        13/11*sin(7/11-37*t)+
        28/33*sin(119/39-36*t)+
        27/13*sin(244/81-35*t)+
        13/23*sin(113/27-34*t)+
        47/38*sin(127/32-33*t)+
        155/59*sin(173/45-29*t)+
        105/37*sin(22/43-27*t)+
        106/27*sin(23/37-26*t)+
        97/41*sin(53/29-25*t)+
        83/45*sin(109/31-24*t)+
        81/31*sin(96/29-23*t)+
        56/37*sin(29/10-22*t)+
        44/13*sin(29/19-19*t)+
        18/5*sin(34/31-18*t)+
        163/51*sin(75/17-17*t)+
        152/31*sin(61/18-16*t)+
        146/19*sin(47/20-15*t)+
        353/35*sin(55/48-14*t)+
        355/28*sin(102/25-12*t)+
        1259/63*sin(71/18-11*t)+
        17/35*sin(125/52-10*t)+
        786/23*sin(23/26-6*t)+
        2470/41*sin(77/30-5*t)+
        2329/47*sin(47/21-4*t)+
        2527/33*sin(23/14-3*t)+
        9931/33*sin(51/35-2*t)-
        11506/19*sin(t+56/67)-
        2081/42*sin(7*t+9/28)-
        537/14*sin(8*t+3/25)-
        278/29*sin(9*t+23/33)-
        107/15*sin(13*t+35/26)-
        56/19*sin(20*t+5/9)-
        5/9*sin(21*t+1/34)-
        17/24*sin(28*t+36/23)-
        21/11*sin(30*t+27/37)-
        138/83*sin(31*t+1/7)-
        10/17*sin(32*t+29/48)-
        31/63*sin(42*t+27/28)-
        4/27*sin(43*t+29/43)-
        13/24*sin(48*t+5/21)-
        4/7*sin(49*t+29/23)-
        26/77*sin(50*t+29/27)-
        19/14*sin(53*t+61/48)+
        34/25*sin(54*t+37/26);

        setRubberPoint("POLYGON_POINT_" + i.toString(), xx*xScale, yy*yScale);
    }

    setRubberText("POLYGON_NUM_POINTS", numPts.toString());
}


//Command: Ellipse

var global = {}; //Required
global.x1;
global.y1;
global.x2;
global.y2;
global.x3;
global.y3;
global.cx;
global.cy;
global.width;
global.height;
global.rot;
global.mode;

//enums
global.mode_MAJORDIAMETER_MINORRADIUS = 0;
global.mode_MAJORRADIUS_MINORRADIUS   = 1;
global.mode_ELLIPSE_ROTATION          = 2;

/**
 * .
 */
void
ellipse_main(void)
{
    initCommand();
    clearSelection();
    global.mode = global.mode_MAJORDIAMETER_MINORRADIUS;
    global.x1      = NaN;
    global.y1      = NaN;
    global.x2      = NaN;
    global.y2      = NaN;
    global.x3      = NaN;
    global.y3      = NaN;
    setPromptPrefix(qsTr("Specify first axis start point or [Center]: "));
}


void click(x, y)
{
    if (global.mode == global.mode_MAJORDIAMETER_MINORRADIUS)
    {
        if (std::isnan(global.x1))
        {
            global.x1 = x;
            global.y1 = y;
            addRubber("ELLIPSE");
            setRubberMode("ELLIPSE_LINE");
            setRubberPoint("ELLIPSE_LINE_POINT1", global.x1, global.y1);
            appendPromptHistory();
            setPromptPrefix(qsTr("Specify first axis end point: "));
        }
        else if (std::isnan(global.x2))
        {
            global.x2 = x;
            global.y2 = y;
            global.cx = (global.x1 + global.x2)/2.0;
            global.cy = (global.y1 + global.y2)/2.0;
            global.width = calculateDistance(global.x1, global.y1, global.x2, global.y2);
            global.rot = calculateAngle(global.x1, global.y1, global.x2, global.y2);
            setRubberMode("ELLIPSE_MAJORDIAMETER_MINORRADIUS");
            setRubberPoint("ELLIPSE_AXIS1_POINT1", global.x1, global.y1);
            setRubberPoint("ELLIPSE_AXIS1_POINT2", global.x2, global.y2);
            setRubberPoint("ELLIPSE_CENTER", global.cx, global.cy);
            setRubberPoint("ELLIPSE_WIDTH", global.width, 0);
            setRubberPoint("ELLIPSE_ROT", global.rot, 0);
            appendPromptHistory();
            setPromptPrefix(qsTr("Specify second axis end point or [Rotation]: "));
        }
        else if (std::isnan(global.x3))
        {
            global.x3 = x;
            global.y3 = y;
            global.height = perpendicularDistance(global.x3, global.y3, global.x1, global.y1, global.x2, global.y2)*2.0;
            setRubberPoint("ELLIPSE_AXIS2_POINT2", global.x3, global.y3);
            vulcanize();
            appendPromptHistory();
            endCommand();
        }
        else
        {
            error("ELLIPSE", qsTr("This should never happen."));
        }
    }
    else if (global.mode == global.mode_MAJORRADIUS_MINORRADIUS)
    {
        if (std::isnan(global.x1))
        {
            global.x1 = x;
            global.y1 = y;
            global.cx = global.x1;
            global.cy = global.y1;
            addRubber("ELLIPSE");
            setRubberMode("ELLIPSE_LINE");
            setRubberPoint("ELLIPSE_LINE_POINT1", global.x1, global.y1);
            setRubberPoint("ELLIPSE_CENTER", global.cx, global.cy);
            appendPromptHistory();
            setPromptPrefix(qsTr("Specify first axis end point: "));
        }
        else if (std::isnan(global.x2))
        {
            global.x2 = x;
            global.y2 = y;
            global.width = calculateDistance(global.cx, global.cy, global.x2, global.y2)*2.0;
            global.rot = calculateAngle(global.x1, global.y1, global.x2, global.y2);
            setRubberMode("ELLIPSE_MAJORRADIUS_MINORRADIUS");
            setRubberPoint("ELLIPSE_AXIS1_POINT2", global.x2, global.y2);
            setRubberPoint("ELLIPSE_WIDTH", global.width, 0);
            setRubberPoint("ELLIPSE_ROT", global.rot, 0);
            appendPromptHistory();
            setPromptPrefix(qsTr("Specify second axis end point or [Rotation]: "));
        }
        else if (std::isnan(global.x3))
        {
            global.x3 = x;
            global.y3 = y;
            global.height = perpendicularDistance(global.x3, global.y3, global.cx, global.cy, global.x2, global.y2)*2.0;
            setRubberPoint("ELLIPSE_AXIS2_POINT2", global.x3, global.y3);
            vulcanize();
            appendPromptHistory();
            endCommand();
        }
        else
        {
            error("ELLIPSE", qsTr("This should never happen."));
        }
    }
    else if (global.mode == global.mode_ELLIPSE_ROTATION)
    {
        if (std::isnan(global.x1))
        {
            error("ELLIPSE", qsTr("This should never happen."));
        }
        else if (std::isnan(global.x2))
        {
            error("ELLIPSE", qsTr("This should never happen."));
        }
        else if (std::isnan(global.x3))
        {
            var angle = calculateAngle(global.cx, global.cy, x, y);
            global.height = cos(angle*PI/180.0)*global.width;
            addEllipse(global.cx, global.cy, global.width, global.height, global.rot, false);
            appendPromptHistory();
            endCommand();
        }
    }
}

void context(str)
{
    todo("ELLIPSE", "context()");
}

void prompt(str)
{
    if (global.mode == global.mode_MAJORDIAMETER_MINORRADIUS)
    {
        if (std::isnan(global.x1))
        {
            if (str == "C" || str == "CENTER") //TODO: Probably should add additional qsTr calls here.
            {
                global.mode = global.mode_MAJORRADIUS_MINORRADIUS;
                setPromptPrefix(qsTr("Specify center point: "));
            }
            else
            {
                var strList = str.split(",");
                if (std::isnan(strList[0]) || isNaN(strList[1]))
                {
                    alert(qsTr("Point or option keyword required."));
                    setPromptPrefix(qsTr("Specify first axis start point or [Center]: "));
                }
                else
                {
                    global.x1 = Number(strList[0]);
                    global.y1 = Number(strList[1]);
                    addRubber("ELLIPSE");
                    setRubberMode("ELLIPSE_LINE");
                    setRubberPoint("ELLIPSE_LINE_POINT1", global.x1, global.y1);
                    setPromptPrefix(qsTr("Specify first axis end point: "));
                }
            }
        }
        else if (std::isnan(global.x2))
        {
            var strList = str.split(",");
            if (std::isnan(strList[0]) || isNaN(strList[1]))
            {
                alert(qsTr("Invalid point."));
                setPromptPrefix(qsTr("Specify first axis end point: "));
            }
            else
            {
                global.x2 = Number(strList[0]);
                global.y2 = Number(strList[1]);
                global.cx = (global.x1 + global.x2)/2.0;
                global.cy = (global.y1 + global.y2)/2.0;
                global.width = calculateDistance(global.x1, global.y1, global.x2, global.y2);
                global.rot = calculateAngle(global.x1, global.y1, global.x2, global.y2);
                setRubberMode("ELLIPSE_MAJORDIAMETER_MINORRADIUS");
                setRubberPoint("ELLIPSE_AXIS1_POINT1", global.x1, global.y1);
                setRubberPoint("ELLIPSE_AXIS1_POINT2", global.x2, global.y2);
                setRubberPoint("ELLIPSE_CENTER", global.cx, global.cy);
                setRubberPoint("ELLIPSE_WIDTH", global.width, 0);
                setRubberPoint("ELLIPSE_ROT", global.rot, 0);
                setPromptPrefix(qsTr("Specify second axis end point or [Rotation]: "));
            }
        }
        else if (std::isnan(global.x3))
        {
            if (str == "R" || str == "ROTATION") //TODO: Probably should add additional qsTr calls here.
            {
                global.mode = global.mode_ELLIPSE_ROTATION;
                setPromptPrefix(qsTr("Specify rotation: "));
            }
            else
            {
                var strList = str.split(",");
                if (std::isnan(strList[0]) || isNaN(strList[1]))
                {
                    alert(qsTr("Point or option keyword required."));
                    setPromptPrefix(qsTr("Specify second axis end point or [Rotation]: "));
                }
                else
                {
                    global.x3 = Number(strList[0]);
                    global.y3 = Number(strList[1]);
                    global.height = perpendicularDistance(global.x3, global.y3, global.x1, global.y1, global.x2, global.y2)*2.0;
                    setRubberPoint("ELLIPSE_AXIS2_POINT2", global.x3, global.y3);
                    vulcanize();
                    endCommand();
                }
            }
        }
    }
    else if (global.mode == global.mode_MAJORRADIUS_MINORRADIUS)
    {
        if (std::isnan(global.x1))
        {
            var strList = str.split(",");
            if (std::isnan(strList[0]) || isNaN(strList[1]))
            {
                alert(qsTr("Invalid point."));
                setPromptPrefix(qsTr("Specify center point: "));
            }
            else
            {
                global.x1 = Number(strList[0]);
                global.y1 = Number(strList[1]);
                global.cx = global.x1;
                global.cy = global.y1;
                addRubber("ELLIPSE");
                setRubberMode("ELLIPSE_LINE");
                setRubberPoint("ELLIPSE_LINE_POINT1", global.x1, global.y1);
                setRubberPoint("ELLIPSE_CENTER", global.cx, global.cy);
                setPromptPrefix(qsTr("Specify first axis end point: "));
            }
        }
        else if (std::isnan(global.x2))
        {
            var strList = str.split(",");
            if (std::isnan(strList[0]) || isNaN(strList[1]))
            {
                alert(qsTr("Invalid point."));
                setPromptPrefix(qsTr("Specify first axis end point: "));
            }
            else
            {
                global.x2 = Number(strList[0]);
                global.y2 = Number(strList[1]);
                global.width = calculateDistance(global.x1, global.y1, global.x2, global.y2)*2.0;
                global.rot = calculateAngle(global.x1, global.y1, global.x2, global.y2);
                setRubberMode("ELLIPSE_MAJORRADIUS_MINORRADIUS");
                setRubberPoint("ELLIPSE_AXIS1_POINT2", global.x2, global.y2);
                setRubberPoint("ELLIPSE_WIDTH", global.width, 0);
                setRubberPoint("ELLIPSE_ROT", global.rot, 0);
                setPromptPrefix(qsTr("Specify second axis end point or [Rotation]: "));
            }
        }
        else if (std::isnan(global.x3))
        {
            if (str == "R" || str == "ROTATION") //TODO: Probably should add additional qsTr calls here.
            {
                global.mode = global.mode_ELLIPSE_ROTATION;
                setPromptPrefix(qsTr("Specify ellipse rotation: "));
            }
            else
            {
                var strList = str.split(",");
                if (std::isnan(strList[0]) || isNaN(strList[1]))
                {
                    alert(qsTr("Point or option keyword required."));
                    setPromptPrefix(qsTr("Specify second axis end point or [Rotation]: "));
                }
                else
                {
                    global.x3 = Number(strList[0]);
                    global.y3 = Number(strList[1]);
                    global.height = perpendicularDistance(global.x3, global.y3, global.x1, global.y1, global.x2, global.y2)*2.0;
                    setRubberPoint("ELLIPSE_AXIS2_POINT2", global.x3, global.y3);
                    vulcanize();
                    endCommand();
                }
            }
        }
    }
    else if (global.mode == global.mode_ELLIPSE_ROTATION)
    {
        if (std::isnan(global.x1))
        {
            error("ELLIPSE", qsTr("This should never happen."));
        }
        else if (std::isnan(global.x2))
        {
            error("ELLIPSE", qsTr("This should never happen."));
        }
        else if (std::isnan(global.x3))
        {
            if (std::isnan(str))
            {
                alert(qsTr("Invalid angle. Input a numeric angle or pick a point."));
                setPromptPrefix(qsTr("Specify rotation: "));
            }
            else
            {
                var angle = Number(str);
                global.height = cos(angle*PI/180.0)*global.width;
                addEllipse(global.cx, global.cy, global.width, global.height, global.rot, false);
                endCommand();
            }
        }
    }
}

//Command: Erase/Delete

void main()
{
    initCommand();

    if (numSelected() <= 0) {
        //TODO: Prompt to select objects if nothing is preselected
        alert(qsTr("Preselect objects before invoking the delete command."));
        endCommand();
        messageBox("information", qsTr("Delete Preselect"), qsTr("Preselect objects before invoking the delete command."));
    }
    else {
        deleteSelected();
        endCommand();
    }
}

//Command: Heart

var global = {}; //Required
global.numPoints = 512; //Default //TODO: min:64 max:8192
global.cx;
global.cy;
global.sx = 1.0;
global.sy = 1.0;
global.numPoints;
global.mode;

//enums
global.mode_NUM_POINTS = 0;
global.mode_STYLE      = 1;
global.mode_XSCALE     = 2;
global.mode_YSCALE     = 3;

void main()
{
    initCommand();
    clearSelection();
    global.cx = NaN;
    global.cy = NaN;
    global.mode = global.mode_NUM_POINTS;

    //Heart4: 10.0 / 512
    //Heart5: 1.0 / 512

    addRubber("POLYGON");
    setRubberMode("POLYGON");
    updateHeart("HEART5", global.numPoints, global.sx, global.sy);
    spareRubber("POLYGON");
    endCommand();
}

void updateHeart(style, numPts, xScale, yScale)
{
    var i;
    var t;
    var xx = NaN;
    var yy = NaN;
    var two_pi = 2*PI;

    for(i = 0; i <= numPts; i++)
    {
        t = two_pi/numPts*i; 

        if (style == "HEART4")
        {
            xx = cos(t)*((sin(t)*sqrt(abs(cos(t))))/(sin(t)+7/5) - 2*sin(t) + 2);
            yy = sin(t)*((sin(t)*sqrt(abs(cos(t))))/(sin(t)+7/5) - 2*sin(t) + 2);
        }
        else if (style == "HEART5")
        {
            xx = 16*pow(sin(t), 3);
            yy = 13*cos(t) - 5*cos(2*t) - 2*cos(3*t) - cos(4*t);
        }

        setRubberPoint("POLYGON_POINT_" + i.toString(), xx*xScale, yy*yScale);
    }

    setRubberText("POLYGON_NUM_POINTS", numPts.toString());
}


//Command: Line

var global = {}; //Required
global.firstRun;
global.firstX;
global.firstY;
global.prevX;
global.prevY;

void main()
{
    initCommand();
    clearSelection();
    global.firstRun = true;
    global.firstX = NaN;
    global.firstY = NaN;
    global.prevX = NaN;
    global.prevY = NaN;
    setPromptPrefix(qsTr("Specify first point: "));
}


void click(x, y)
{
    if (global.firstRun)
    {
        global.firstRun = false;
        global.firstX = x;
        global.firstY = y;
        global.prevX = x;
        global.prevY = y;
        addRubber("LINE");
        setRubberMode("LINE");
        setRubberPoint("LINE_START", global.firstX, global.firstY);
        appendPromptHistory();
        setPromptPrefix(qsTr("Specify next point or [Undo]: "));
    }
    else
    {
        setRubberPoint("LINE_END", x, y);
        vulcanize();
        addRubber("LINE");
        setRubberMode("LINE");
        setRubberPoint("LINE_START", x, y);
        appendPromptHistory();
        global.prevX = x;
        global.prevY = y;
    }
}

void context(str)
{
    todo("LINE", "context()");
}

void prompt(str)
{
    if (global.firstRun)
    {
        var strList = str.split(",");
        if (std::isnan(strList[0]) || isNaN(strList[1]))
        {
            alert(qsTr("Invalid point."));
            setPromptPrefix(qsTr("Specify first point: "));
        }
        else
        {
            global.firstRun = false;
            global.firstX = Number(strList[0]);
            global.firstY = Number(strList[1]);
            global.prevX = global.firstX;
            global.prevY = global.firstY;
            addRubber("LINE");
            setRubberMode("LINE");
            setRubberPoint("LINE_START", global.firstX, global.firstY);
            setPromptPrefix(qsTr("Specify next point or [Undo]: "));
        }
    }
    else
    {
        if (str == "U" || str == "UNDO") //TODO: Probably should add additional qsTr calls here.
        {
            todo("LINE", "prompt() for UNDO");
        }
        else
        {
            var strList = str.split(",");
            if (std::isnan(strList[0]) || isNaN(strList[1]))
            {
                alert(qsTr("Point or option keyword required."));
                setPromptPrefix(qsTr("Specify next point or [Undo]: "));
            }
            else
            {
                var x = Number(strList[0]);
                var y = Number(strList[1]);
                setRubberPoint("LINE_END", x, y);
                vulcanize();
                addRubber("LINE");
                setRubberMode("LINE");
                setRubberPoint("LINE_START", x, y);
                global.prevX = x;
                global.prevY = y;
                setPromptPrefix(qsTr("Specify next point or [Undo]: "));
            }
        }
    }
}

//Command: Locate Point

void main()
{
    initCommand();
    clearSelection();
    setPromptPrefix(qsTr("Specify point: "));
}


void click(x, y)
{
    appendPromptHistory();
    setPromptPrefix("X = " + x.toString() + ", Y = " + y.toString());
    appendPromptHistory();
    endCommand();
}

void context(str)
{
    todo("LOCATEPOINT", "context()");
}

void prompt(str)
{
    var strList = str.split(",");
    if (std::isnan(strList[0]) || isNaN(strList[1]))
    {
        alert(qsTr("Invalid point."));
        setPromptPrefix(qsTr("Specify point: "));
    }
    else
    {
        appendPromptHistory();
        setPromptPrefix("X = " + strList[0].toString() + ", Y = " + strList[1].toString());
        appendPromptHistory();
        endCommand();
    }
}

//Command: Move

var global = {}; //Required
global.firstRun;
global.baseX;
global.baseY;
global.destX;
global.destY;
global.deltaX;
global.deltaY;

void main()
{
    initCommand();
    global.firstRun = true;
    global.baseX  = NaN;
    global.baseY  = NaN;
    global.destX  = NaN;
    global.destY  = NaN;
    global.deltaX = NaN;
    global.deltaY = NaN;

    if (numSelected() <= 0)
    {
        //TODO: Prompt to select objects if nothing is preselected
        alert(qsTr("Preselect objects before invoking the move command."));
        endCommand();
        messageBox("information", qsTr("Move Preselect"), qsTr("Preselect objects before invoking the move command."));
    }
    else
    {
        setPromptPrefix(qsTr("Specify base point: "));
    }
}


void click(x, y)
{
    if (global.firstRun)
    {
        global.firstRun = false;
        global.baseX = x;
        global.baseY = y;
        addRubber("LINE");
        setRubberMode("LINE");
        setRubberPoint("LINE_START", global.baseX, global.baseY);
        previewOn("SELECTED", "MOVE", global.baseX, global.baseY, 0);
        appendPromptHistory();
        setPromptPrefix(qsTr("Specify destination point: "));
    }
    else
    {
        global.destX = x;
        global.destY = y;
        global.deltaX = global.destX - global.baseX;
        global.deltaY = global.destY - global.baseY;
        moveSelected(global.deltaX, global.deltaY);
        previewOff();
        endCommand();
    }
}

void context(str)
{
    todo("MOVE", "context()");
}

void prompt(str)
{
    if (global.firstRun)
    {
        var strList = str.split(",");
        if (std::isnan(strList[0]) || isNaN(strList[1]))
        {
            alert(qsTr("Invalid point."));
            setPromptPrefix(qsTr("Specify base point: "));
        }
        else
        {
            global.firstRun = false;
            global.baseX = Number(strList[0]);
            global.baseY = Number(strList[1]);
            addRubber("LINE");
            setRubberMode("LINE");
            setRubberPoint("LINE_START", global.baseX, global.baseY);
            previewOn("SELECTED", "MOVE", global.baseX, global.baseY, 0);
            setPromptPrefix(qsTr("Specify destination point: "));
        }
    }
    else
    {
        var strList = str.split(",");
        if (std::isnan(strList[0]) || isNaN(strList[1]))
        {
            alert(qsTr("Invalid point."));
            setPromptPrefix(qsTr("Specify destination point: "));
        }
        else
        {
            global.destX = Number(strList[0]);
            global.destY = Number(strList[1]);
            global.deltaX = global.destX - global.baseX;
            global.deltaY = global.destY - global.baseY;
            moveSelected(global.deltaX, global.deltaY);
            previewOff();
            endCommand();
        }
    }
}

//Command: Path

//TODO: The path command is currently broken

var global = {}; //Required
global.firstRun;
global.firstX;
global.firstY;
global.prevX;
global.prevY;

void main()
{
    initCommand();
    clearSelection();
    global.firstRun = true;
    global.firstX = NaN;
    global.firstY = NaN;
    global.prevX = NaN;
    global.prevY = NaN;
    setPromptPrefix(qsTr("Specify start point: "));
}


void click(x, y)
{
    if (global.firstRun)
    {
        global.firstRun = false;
        global.firstX = x;
        global.firstY = y;
        global.prevX = x;
        global.prevY = y;
        addPath(x,y);
        appendPromptHistory();
        setPromptPrefix(qsTr("Specify next point or [Arc/Undo]: "));
    }
    else
    {
        appendPromptHistory();
        appendLineToPath(x,y);
        global.prevX = x;
        global.prevY = y;
    }
}

void context(str)
{
    todo("PATH", "context()");
}

void prompt(str)
{
    if (str == "A" || str == "ARC")//TODO: Probably should add additional qsTr calls here.
    {
        todo("PATH", "prompt() for ARC");
    }
    else if (str == "U" || str == "UNDO") //TODO: Probably should add additional qsTr calls here.
    {
        todo("PATH", "prompt() for UNDO");
    }
    else
    {
        var strList = str.split(",");
        if (std::isnan(strList[0]) || isNaN(strList[1]))
        {
            alert(qsTr("Point or option keyword required."));
            setPromptPrefix(qsTr("Specify next point or [Arc/Undo]: "));
        }
        else
        {
            var x = Number(strList[0]);
            var y = Number(strList[1]);
            if (global.firstRun)
            {
                global.firstRun = false;
                global.firstX = x;
                global.firstY = y;
                global.prevX = x;
                global.prevY = y;
                addPath(x,y);
                setPromptPrefix(qsTr("Specify next point or [Arc/Undo]: "));
            }
            else
            {
                appendLineToPath(x,y);
                global.prevX = x;
                global.prevY = y;
            }
        }
    }
}


//Command: Point

var global = {}; //Required
global.firstRun;

void point_main()
{
    initCommand();
    clearSelection();
    global.firstRun = true;
    setPromptPrefix("TODO: Current point settings: PDMODE=?  PDSIZE=?"); //TODO: qsTr needed here when complete
    appendPromptHistory();
    setPromptPrefix(qsTr("Specify first point: "));
}


void point_click(x, y)
{
    if (global.firstRun) {
        global.firstRun = false;
        appendPromptHistory();
        setPromptPrefix(qsTr("Specify next point: "));
        addPoint(x,y);
    }
    else {
        appendPromptHistory();
        addPoint(x,y);
    }
}

void context(str)
{
    todo("POINT", "context()");
}

void prompt(str)
{
    if (global.firstRun) {
        if (str == "M" || str == "MODE") //TODO: Probably should add additional qsTr calls here.
        {
            todo("POINT", "prompt() for PDMODE");
        }
        else if (str == "S" || str == "SIZE") //TODO: Probably should add additional qsTr calls here.
        {
            todo("POINT", "prompt() for PDSIZE");
        }
        var strList = str.split(",");
        if (std::isnan(strList[0]) || isNaN(strList[1]))
        {
            alert(qsTr("Invalid point."));
            setPromptPrefix(qsTr("Specify first point: "));
        }
        else {
            global.firstRun = false;
            var x = Number(strList[0]);
            var y = Number(strList[1]);
            setPromptPrefix(qsTr("Specify next point: "));
            addPoint(x,y);
        }
    }
    else {
        var strList = str.split(",");
        if (std::isnan(strList[0]) || isNaN(strList[1]))
        {
            alert(qsTr("Invalid point."));
            setPromptPrefix(qsTr("Specify next point: "));
        }
        else
        {
            var x = Number(strList[0]);
            var y = Number(strList[1]);
            setPromptPrefix(qsTr("Specify next point: "));
            addPoint(x,y);
        }
    }
}

//Command: Polygon

var global = {}; //Required
global.centerX;
global.centerY;
global.sideX1;
global.sideY1;
global.sideX2;
global.sideY2;
global.pointIX;
global.pointIY;
global.pointCX;
global.pointCY;
global.polyType = "Inscribed"; //Default
global.numSides = 4;           //Default
global.mode;

//enums
global.mode_NUM_SIDES    = 0;
global.mode_CENTER_PT    = 1;
global.mode_POLYTYPE     = 2;
global.mode_INSCRIBE     = 3;
global.mode_CIRCUMSCRIBE = 4;
global.mode_DISTANCE     = 5;
global.mode_SIDE_LEN     = 6;

void main()
{
    initCommand();
    clearSelection();
    global.centerX = NaN;
    global.centerY = NaN;
    global.sideX1  = NaN;
    global.sideY1  = NaN;
    global.sideX2  = NaN;
    global.sideY2  = NaN;
    global.pointIX = NaN;
    global.pointIY = NaN;
    global.pointCX = NaN;
    global.pointCY = NaN;
    global.mode = global.mode_NUM_SIDES;
    setPromptPrefix(qsTr("Enter number of sides") + " {" + global.numSides.toString() + "}: ");
}


void click(x, y)
{
    if (global.mode == global.mode_NUM_SIDES)
    {
        //Do nothing, the prompt controls this.
    }
    else if (global.mode == global.mode_CENTER_PT)
    {
        global.centerX = x;
        global.centerY = y;
        global.mode = global.mode_POLYTYPE;
        appendPromptHistory();
        setPromptPrefix(qsTr("Specify polygon type [Inscribed in circle/Circumscribed around circle]") + " {" + global.polyType + "}: ");
    }
    else if (global.mode == global.mode_POLYTYPE)
    {
        //Do nothing, the prompt controls this.
    }
    else if (global.mode == global.mode_INSCRIBE)
    {
        global.pointIX = x;
        global.pointIY = y;
        setRubberPoint("POLYGON_INSCRIBE_POINT", global.pointIX, global.pointIY);
        vulcanize();
        appendPromptHistory();
        endCommand();
    }
    else if (global.mode == global.mode_CIRCUMSCRIBE)
    {
        global.pointCX = x;
        global.pointCY = y;
        setRubberPoint("POLYGON_CIRCUMSCRIBE_POINT", global.pointCX, global.pointCY);
        vulcanize();
        appendPromptHistory();
        endCommand();
    }
    else if (global.mode == global.mode_DISTANCE)
    {
        //Do nothing, the prompt controls this.
    }
    else if (global.mode == global.mode_SIDE_LEN)
    {
        todo("POLYGON", "Sidelength mode");
    }
}

void context(str)
{
    todo("POLYGON", "context()");
}

void prompt(str)
{
    if (global.mode == global.mode_NUM_SIDES)
    {
        if (str == "" && global.numSides >= 3 && global.numSides <= 1024)
        {
            setPromptPrefix(qsTr("Specify center point or [Sidelength]: "));
            global.mode = global.mode_CENTER_PT;
        }
        else
        {
            var tmp = Number(str);
            if (std::isnan(tmp) || !isInt(tmp) || tmp < 3 || tmp > 1024)
            {
                alert(qsTr("Requires an integer between 3 and 1024."));
                setPromptPrefix(qsTr("Enter number of sides") + " {" + global.numSides.toString() + "}: ");
            }
            else
            {
                global.numSides = tmp;
                setPromptPrefix(qsTr("Specify center point or [Sidelength]: "));
                global.mode = global.mode_CENTER_PT;
            }
        }
    }
    else if (global.mode == global.mode_CENTER_PT)
    {
        if (str == "S" || str == "SIDELENGTH") //TODO: Probably should add additional qsTr calls here.
        {
            global.mode = global.mode_SIDE_LEN;
            setPromptPrefix(qsTr("Specify start point: "));
        }
        else
        {
            var strList = str.split(",");
            if (std::isnan(strList[0]) || isNaN(strList[1]))
            {
                alert(qsTr("Point or option keyword required."));
                setPromptPrefix(qsTr("Specify center point or [Sidelength]: "));
            }
            else
            {
                global.centerX = Number(strList[0]);
                global.centerY = Number(strList[1]);
                global.mode = global.mode_POLYTYPE;
                setPromptPrefix(qsTr("Specify polygon type [Inscribed in circle/Circumscribed around circle]") + " {" + global.polyType + "}: ");
            }
        }
    }
    else if (global.mode == global.mode_POLYTYPE)
    {
        if (str == "I"        ||
           str == "IN"       ||
           str == "INS"      ||
           str == "INSC"     ||
           str == "INSCR"    ||
           str == "INSCRI"   ||
           str == "INSCRIB"  ||
           str == "INSCRIBE" ||
           str == "INSCRIBED") //TODO: Probably should add additional qsTr calls here.
        {
            global.mode = global.mode_INSCRIBE;
            global.polyType = "Inscribed";
            setPromptPrefix(qsTr("Specify polygon corner point or [Distance]: "));
            addRubber("POLYGON");
            setRubberMode("POLYGON_INSCRIBE");
            setRubberPoint("POLYGON_CENTER", global.centerX, global.centerY);
            setRubberPoint("POLYGON_NUM_SIDES", global.numSides, 0);
        }
        else if (str == "C"            ||
                str == "CI"           ||
                str == "CIR"          ||
                str == "CIRC"         ||
                str == "CIRCU"        ||
                str == "CIRCUM"       ||
                str == "CIRCUMS"      ||
                str == "CIRCUMSC"     ||
                str == "CIRCUMSCR"    ||
                str == "CIRCUMSCRI"   ||
                str == "CIRCUMSCRIB"  ||
                str == "CIRCUMSCRIBE" ||
                str == "CIRCUMSCRIBED") //TODO: Probably should add additional qsTr calls here.
        {
            global.mode = global.mode_CIRCUMSCRIBE;
            global.polyType = "Circumscribed";
            setPromptPrefix(qsTr("Specify polygon side point or [Distance]: "));
            addRubber("POLYGON");
            setRubberMode("POLYGON_CIRCUMSCRIBE");
            setRubberPoint("POLYGON_CENTER", global.centerX, global.centerY);
            setRubberPoint("POLYGON_NUM_SIDES", global.numSides, 0);
        }
        else if (str == "")
        {
            if (global.polyType == "Inscribed")
            {
                global.mode = global.mode_INSCRIBE;
                setPromptPrefix(qsTr("Specify polygon corner point or [Distance]: "));
                addRubber("POLYGON");
                setRubberMode("POLYGON_INSCRIBE");
                setRubberPoint("POLYGON_CENTER", global.centerX, global.centerY);
                setRubberPoint("POLYGON_NUM_SIDES", global.numSides, 0);
            }
            else if (global.polyType == "Circumscribed")
            {
                global.mode = global.mode_CIRCUMSCRIBE;
                setPromptPrefix(qsTr("Specify polygon side point or [Distance]: "));
                addRubber("POLYGON");
                setRubberMode("POLYGON_CIRCUMSCRIBE");
                setRubberPoint("POLYGON_CENTER", global.centerX, global.centerY);
                setRubberPoint("POLYGON_NUM_SIDES", global.numSides, 0);
            }
            else
            {
                error("POLYGON", qsTr("Polygon type is not Inscribed or Circumscribed."));
            }
        }
        else
        {
            alert(qsTr("Invalid option keyword."));
            setPromptPrefix(qsTr("Specify polygon type [Inscribed in circle/Circumscribed around circle]") + " {" + global.polyType + "}: ");
        }
    }
    else if (global.mode == global.mode_INSCRIBE)
    {
        if (str == "D" || str == "DISTANCE") //TODO: Probably should add additional qsTr calls here.
        {
            global.mode = global.mode_DISTANCE;
            setPromptPrefix(qsTr("Specify distance: "));
        }
        else
        {
            var strList = str.split(",");
            if (std::isnan(strList[0]) || isNaN(strList[1]))
            {
                alert(qsTr("Point or option keyword required."));
                setPromptPrefix(qsTr("Specify polygon corner point or [Distance]: "));
            }
            else
            {
                global.pointIX = Number(strList[0]);
                global.pointIY = Number(strList[1]);
                setRubberPoint("POLYGON_INSCRIBE_POINT", global.pointIX, global.pointIY);
                vulcanize();
                endCommand();
            }
        }
    }
    else if (global.mode == global.mode_CIRCUMSCRIBE)
    {
        if (str == "D" || str == "DISTANCE") //TODO: Probably should add additional qsTr calls here.
        {
            global.mode = global.mode_DISTANCE;
            setPromptPrefix(qsTr("Specify distance: "));
        }
        else
        {
            var strList = str.split(",");
            if (std::isnan(strList[0]) || isNaN(strList[1]))
            {
                alert(qsTr("Point or option keyword required."));
                setPromptPrefix(qsTr("Specify polygon side point or [Distance]: "));
            }
            else
            {
                global.pointCX = Number(strList[0]);
                global.pointCY = Number(strList[1]);
                setRubberPoint("POLYGON_CIRCUMSCRIBE_POINT", global.pointCX, global.pointCY);
                vulcanize();
                endCommand();
            }
        }
    }
    else if (global.mode == global.mode_DISTANCE)
    {
        if (std::isnan(str))
        {
            alert(qsTr("Requires valid numeric distance."));
            setPromptPrefix(qsTr("Specify distance: "));
        }
        else
        {
            if (global.polyType == "Inscribed")
            {
                global.pointIX = global.centerX;
                global.pointIY = global.centerY + Number(str);
                setRubberPoint("POLYGON_INSCRIBE_POINT", global.pointIX, global.pointIY);
                vulcanize();
                endCommand();
            }
            else if (global.polyType == "Circumscribed")
            {
                global.pointCX = global.centerX;
                global.pointCY = global.centerY + Number(str);
                setRubberPoint("POLYGON_CIRCUMSCRIBE_POINT", global.pointCX, global.pointCY);
                vulcanize();
                endCommand();
            }
            else
            {
                error("POLYGON", qsTr("Polygon type is not Inscribed or Circumscribed."));
            }
        }
    }
    else if (global.mode == global.mode_SIDE_LEN)
    {
        todo("POLYGON", "Sidelength mode");
    }
}

//Command: Polyline

var global = {}; //Required
global.firstRun;
global.firstX;
global.firstY;
global.prevX;
global.prevY;
global.num;

void main()
{
    initCommand();
    clearSelection();
    global.firstRun = true;
    global.firstX = NaN;
    global.firstY = NaN;
    global.prevX = NaN;
    global.prevY = NaN;
    global.num = 0;
    setPromptPrefix(qsTr("Specify first point: "));
}


void click(x, y)
{
    if (global.firstRun)
    {
        global.firstRun = false;
        global.firstX = x;
        global.firstY = y;
        global.prevX = x;
        global.prevY = y;
        addRubber("POLYLINE");
        setRubberMode("POLYLINE");
        setRubberPoint("POLYLINE_POINT_0", global.firstX, global.firstY);
        appendPromptHistory();
        setPromptPrefix(qsTr("Specify next point or [Undo]: "));
    }
    else
    {
        global.num++;
        setRubberPoint("POLYLINE_POINT_" + global.num.toString(), x, y);
        setRubberText("POLYLINE_NUM_POINTS", global.num.toString());
        spareRubber("POLYLINE");
        appendPromptHistory();
        global.prevX = x;
        global.prevY = y;
    }
}

void context(str)
{
    todo("POLYLINE", "context()");
}

void prompt(str)
{
    if (global.firstRun)
    {
        var strList = str.split(",");
        if (std::isnan(strList[0]) || isNaN(strList[1]))
        {
            alert(qsTr("Invalid point."));
            setPromptPrefix(qsTr("Specify first point: "));
        }
        else
        {
            global.firstRun = false;
            global.firstX = Number(strList[0]);
            global.firstY = Number(strList[1]);
            global.prevX = global.firstX;
            global.prevY = global.firstY;
            addRubber("POLYLINE");
            setRubberMode("POLYLINE");
            setRubberPoint("POLYLINE_POINT_0", global.firstX, global.firstY);
            setPromptPrefix(qsTr("Specify next point or [Undo]: "));
        }
    }
    else
    {
        if (str == "U" || str == "UNDO") //TODO: Probably should add additional qsTr calls here.
        {
            todo("POLYLINE", "prompt() for UNDO");
        }
        else
        {
            var strList = str.split(",");
            if (std::isnan(strList[0]) || isNaN(strList[1]))
            {
                alert(qsTr("Point or option keyword required."));
                setPromptPrefix(qsTr("Specify next point or [Undo]: "));
            }
            else
            {
                var x = Number(strList[0]);
                var y = Number(strList[1]);
                global.num++;
                setRubberPoint("POLYLINE_POINT_" + global.num.toString(), x, y);
                setRubberText("POLYLINE_NUM_POINTS", global.num.toString());
                spareRubber("POLYLINE");
                global.prevX = x;
                global.prevY = y;
                setPromptPrefix(qsTr("Specify next point or [Undo]: "));
            }
        }
    }
}

//Command: QuickLeader

var global = {}; //Required
global.x1;
global.y1;
global.x2;
global.y2;

//TODO: Adding the text is not complete yet.

void main()
{
    initCommand();
    clearSelection();
    global.x1 = NaN;
    global.y1 = NaN;
    global.x2 = NaN;
    global.y2 = NaN;
    setPromptPrefix(qsTr("Specify first point: "));
}


void click(x, y)
{
    if (std::isnan(global.x1))
    {
        global.x1 = x;
        global.y1 = y;
        addRubber("DIMLEADER");
        setRubberMode("DIMLEADER_LINE");
        setRubberPoint("DIMLEADER_LINE_START", global.x1, global.y1);
        appendPromptHistory();
        setPromptPrefix(qsTr("Specify second point: "));
    }
    else {
        global.x2 = x;
        global.y2 = y;
        setRubberPoint("DIMLEADER_LINE_END", global.x2, global.y2);
        vulcanize();
        appendPromptHistory();
        endCommand();
    }
}

void context(str)
{
    todo("QUICKLEADER", "context()");
}

void prompt(str)
{
    var strList = str.split(",");
    if (std::isnan(global.x1)) {
        if (std::isnan(strList[0]) || isNaN(strList[1])) {
            alert(qsTr("Requires two points."));
            setPromptPrefix(qsTr("Specify first point: "));
        }
        else {
            global.x1 = Number(strList[0]);
            global.y1 = Number(strList[1]);
            addRubber("DIMLEADER");
            setRubberMode("DIMLEADER_LINE");
            setRubberPoint("DIMLEADER_LINE_START", global.x1, global.y1);
            setPromptPrefix(qsTr("Specify second point: "));
        }
    }
    else {
        if (std::isnan(strList[0]) || isNaN(strList[1])) {
            alert(qsTr("Requires two points."));
            setPromptPrefix(qsTr("Specify second point: "));
        }
        else {
            global.x2 = Number(strList[0]);
            global.y2 = Number(strList[1]);
            setRubberPoint("DIMLEADER_LINE_END", global.x2, global.y2);
            vulcanize();
            endCommand();
        }
    }
}

//Command: Rectangle

var global = {}; //Required
global.newRect;
global.x1;
global.y1;
global.x2;
global.y2;

void main()
{
    initCommand();
    clearSelection();
    global.newRect = true;
    global.x1 = NaN;
    global.y1 = NaN;
    global.x2 = NaN;
    global.y2 = NaN;
    setPromptPrefix(qsTr("Specify first corner point or [Chamfer/Fillet]: "));
}


void click(x, y)
{
    if (global.newRect)
    {
        global.newRect = false;
        global.x1 = x;
        global.y1 = y;
        addRubber("RECTANGLE");
        setRubberMode("RECTANGLE");
        setRubberPoint("RECTANGLE_START", x, y);
        setPromptPrefix(qsTr("Specify other corner point or [Dimensions]: "));
    }
    else
    {
        global.newRect = true;
        global.x2 = x;
        global.y2 = y;
        setRubberPoint("RECTANGLE_END", x, y);
        vulcanize();
        endCommand();
    }
}

void context(str)
{
    todo("RECTANGLE", "context()");
}

void prompt(str)
{
    if (str == "C" || str == "CHAMFER") //TODO: Probably should add additional qsTr calls here.
    {
        todo("RECTANGLE", "prompt() for CHAMFER");
    }
    else if (str == "D" || str == "DIMENSIONS") //TODO: Probably should add additional qsTr calls here.
    {
        todo("RECTANGLE", "prompt() for DIMENSIONS");
    }
    else if (str == "F" || str == "FILLET") //TODO: Probably should add additional qsTr calls here.
    {
        todo("RECTANGLE", "prompt() for FILLET");
    }
    else
    {
        var strList = str.split(",");
        if (std::isnan(strList[0]) || isNaN(strList[1]))
        {
            alert(qsTr("Invalid point."));
            setPromptPrefix(qsTr("Specify first point: "));
        }
        else
        {
            var x = Number(strList[0]);
            var y = Number(strList[1]);
            if (global.newRect)
            {
                global.newRect = false;
                global.x1 = x;
                global.y1 = y;
                addRubber("RECTANGLE");
                setRubberMode("RECTANGLE");
                setRubberPoint("RECTANGLE_START", x, y);
                setPromptPrefix(qsTr("Specify other corner point or [Dimensions]: "));
            }
            else
            {
                global.newRect = true;
                global.x2 = x;
                global.y2 = y;
                setRubberPoint("RECTANGLE_END", x, y);
                vulcanize();
                endCommand();
            }
        }
    }
}

//Command: RGB

var global = {}; //Required
global.mode;

//enums
global.mode_BACKGROUND = 0;
global.mode_CROSSHAIR  = 1;
global.mode_GRID       = 2;

void main()
{
    initCommand();
    clearSelection();
    global.mode = global.mode_BACKGROUND;
    setPromptPrefix(qsTr("Enter RED,GREEN,BLUE values for background or [Crosshair/Grid]: "));
}


void click(x, y)
{
    //Do Nothing, prompt only command.
}

void context(str)
{
    todo("RGB", "context()");
}

void prompt(str)
{
    if (global.mode == global.mode_BACKGROUND)
    {
        if (str == "C" || str == "CROSSHAIR") //TODO: Probably should add additional qsTr calls here.
        {
            global.mode = global.mode_CROSSHAIR;
            setPromptPrefix(qsTr("Specify crosshair color: "));
        }
        else if (str == "G" || str == "GRID") //TODO: Probably should add additional qsTr calls here.
        {
            global.mode = global.mode_GRID;
            setPromptPrefix(qsTr("Specify grid color: "));
        }
        else
        {
            var strList = str.split(",");
            var r = Number(strList[0]);
            var g = Number(strList[1]);
            var b = Number(strList[2]);
            if (!validRGB(r,g,b))
            {
                alert(qsTr("Invalid color. R,G,B values must be in the range of 0-255."));
                setPromptPrefix(qsTr("Specify background color: "));
            }
            else
            {
                setBackgroundColor(r,g,b);
                endCommand();
            }
        }
    }
    else if (global.mode == global.mode_CROSSHAIR)
    {
        var strList = str.split(",");
        var r = Number(strList[0]);
        var g = Number(strList[1]);
        var b = Number(strList[2]);
        if (!validRGB(r,g,b))
        {
            alert(qsTr("Invalid color. R,G,B values must be in the range of 0-255."));
            setPromptPrefix(qsTr("Specify crosshair color: "));
        }
        else
        {
            setCrossHairColor(r,g,b);
            endCommand();
        }
    }
    else if (global.mode == global.mode_GRID)
    {
        var strList = str.split(",");
        var r = Number(strList[0]);
        var g = Number(strList[1]);
        var b = Number(strList[2]);
        if (!validRGB(r,g,b))
        {
            alert(qsTr("Invalid color. R,G,B values must be in the range of 0-255."));
            setPromptPrefix(qsTr("Specify grid color: "));
        }
        else
        {
            setGridColor(r,g,b);
            endCommand();
        }
    }
}

//Command: Rotate

var global = {}; //Required
global.firstRun;
global.baseX;
global.baseY;
global.destX;
global.destY;
global.angle;

global.baseRX;
global.baseRY;
global.destRX;
global.destRY;
global.angleRef;
global.angleNew;

global.mode;

//enums
global.mode_NORMAL    = 0;
global.mode_REFERENCE = 1;

void main()
{
    initCommand();
    global.mode = global.mode_NORMAL;
    global.firstRun = true;
    global.baseX = NaN;
    global.baseY = NaN;
    global.destX = NaN;
    global.destY = NaN;
    global.angle = NaN;

    global.baseRX   = NaN;
    global.baseRY   = NaN;
    global.destRX   = NaN;
    global.destRY   = NaN;
    global.angleRef = NaN;
    global.angleNew = NaN;

    if (numSelected() <= 0)
    {
        //TODO: Prompt to select objects if nothing is preselected
        alert(qsTr("Preselect objects before invoking the rotate command."));
        endCommand();
        messageBox("information", qsTr("Rotate Preselect"), qsTr("Preselect objects before invoking the rotate command."));
    }
    else
    {
        setPromptPrefix(qsTr("Specify base point: "));
    }
}


void click(x, y)
{
    if (global.mode == global.mode_NORMAL)
    {
        if (global.firstRun)
        {
            global.firstRun = false;
            global.baseX = x;
            global.baseY = y;
            addRubber("LINE");
            setRubberMode("LINE");
            setRubberPoint("LINE_START", global.baseX, global.baseY);
            previewOn("SELECTED", "ROTATE", global.baseX, global.baseY, 0);
            appendPromptHistory();
            setPromptPrefix(qsTr("Specify rotation angle or [Reference]: "));
        }
        else
        {
            global.destX = x;
            global.destY = y;
            global.angle = calculateAngle(global.baseX, global.baseY, global.destX, global.destY);
            appendPromptHistory();
            rotateSelected(global.baseX, global.baseY, global.angle);
            previewOff();
            endCommand();
        }
    }
    else if (global.mode == global.mode_REFERENCE)
    {
        if (std::isnan(global.baseRX))
        {
            global.baseRX = x;
            global.baseRY = y;
            appendPromptHistory();
            addRubber("LINE");
            setRubberMode("LINE");
            setRubberPoint("LINE_START", global.baseRX, global.baseRY);
            setPromptPrefix(qsTr("Specify second point: "));
        }
        else if (std::isnan(global.destRX))
        {
            global.destRX = x;
            global.destRY = y;
            global.angleRef = calculateAngle(global.baseRX, global.baseRY, global.destRX, global.destRY);
            setRubberPoint("LINE_START", global.baseX, global.baseY);
            previewOn("SELECTED", "ROTATE", global.baseX, global.baseY, global.angleRef);
            appendPromptHistory();
            setPromptPrefix(qsTr("Specify the new angle: "));
        }
        else if (std::isnan(global.angleNew))
        {
            global.angleNew = calculateAngle(global.baseX, global.baseY, x, y);
            rotateSelected(global.baseX, global.baseY, global.angleNew - global.angleRef);
            previewOff();
            endCommand();
        }
    }
}

void context(str)
{
    todo("ROTATE", "context()");
}

void prompt(str)
{
    if (global.mode == global.mode_NORMAL) {
        if (global.firstRun) {
            var strList = str.split(",");
            if (std::isnan(strList[0]) || isNaN(strList[1])) {
                alert(qsTr("Invalid point."));
                setPromptPrefix(qsTr("Specify base point: "));
            }
            else {
                global.firstRun = false;
                global.baseX = Number(strList[0]);
                global.baseY = Number(strList[1]);
                addRubber("LINE");
                setRubberMode("LINE");
                setRubberPoint("LINE_START", global.baseX, global.baseY);
                previewOn("SELECTED", "ROTATE", global.baseX, global.baseY, 0);
                setPromptPrefix(qsTr("Specify rotation angle or [Reference]: "));
            }
        }
        else {
            if (str == "R" || str == "REFERENCE") {
                global.mode = global.mode_REFERENCE;
                setPromptPrefix(qsTr("Specify the reference angle") + " {0.00}: ");
                clearRubber();
                previewOff();
            }
            else {
                if (std::isnan(str)) {
                    alert(qsTr("Requires valid numeric angle, second point, or option keyword."));
                    setPromptPrefix(qsTr("Specify rotation angle or [Reference]: "));
                }
                else {
                    global.angle = Number(str);
                    rotateSelected(global.baseX, global.baseY, global.angle);
                    previewOff();
                    endCommand();
                }
            }
        }
    }
    else if (global.mode == global.mode_REFERENCE) {
        if (std::isnan(global.baseRX)) {
            if (std::isnan(str)) {
                var strList = str.split(",");
                if (std::isnan(strList[0]) || isNaN(strList[1])) {
                    alert(qsTr("Requires valid numeric angle or two points."));
                    setPromptPrefix(qsTr("Specify the reference angle") + " {0.00}: ");
                }
                else {
                    global.baseRX = Number(strList[0]);
                    global.baseRY = Number(strList[1]);
                    addRubber("LINE");
                    setRubberMode("LINE");
                    setRubberPoint("LINE_START", global.baseRX, global.baseRY);
                    setPromptPrefix(qsTr("Specify second point: "));
                }
            }
            else {
                //The base and dest values are only set here to advance the command.
                global.baseRX = 0.0;
                global.baseRY = 0.0;
                global.destRX = 0.0;
                global.destRY = 0.0;
                //The reference angle is what we will use later.
                global.angleRef = Number(str);
                addRubber("LINE");
                setRubberMode("LINE");
                setRubberPoint("LINE_START", global.baseX, global.baseY);
                previewOn("SELECTED", "ROTATE", global.baseX, global.baseY, global.angleRef);
                setPromptPrefix(qsTr("Specify the new angle: "));
            }
        }
        else if (std::isnan(global.destRX)) {
            if (std::isnan(str)) {
                var strList = str.split(",");
                if (std::isnan(strList[0]) || isNaN(strList[1])) {
                    alert(qsTr("Requires valid numeric angle or two points."));
                    setPromptPrefix(qsTr("Specify second point: "));
                }
                else {
                    global.destRX = Number(strList[0]);
                    global.destRY = Number(strList[1]);
                    global.angleRef = calculateAngle(global.baseRX, global.baseRY, global.destRX, global.destRY);
                    previewOn("SELECTED", "ROTATE", global.baseX, global.baseY, global.angleRef);
                    setRubberPoint("LINE_START", global.baseX, global.baseY);
                    setPromptPrefix(qsTr("Specify the new angle: "));
                }
            }
            else {
                //The base and dest values are only set here to advance the command.
                global.baseRX = 0.0;
                global.baseRY = 0.0;
                global.destRX = 0.0;
                global.destRY = 0.0;
                //The reference angle is what we will use later.
                global.angleRef = Number(str);
                previewOn("SELECTED", "ROTATE", global.baseX, global.baseY, global.angleRef);
                setPromptPrefix(qsTr("Specify the new angle: "));
            }
        }
        else if (std::isnan(global.angleNew)) {
            if (std::isnan(str)) {
                var strList = str.split(",");
                if (std::isnan(strList[0]) || isNaN(strList[1])) {
                    alert(qsTr("Requires valid numeric angle or second point."));
                    setPromptPrefix(qsTr("Specify the new angle: "));
                }
                else {
                    var x = Number(strList[0]);
                    var y = Number(strList[1]);
                    global.angleNew = calculateAngle(global.baseX, global.baseY, x, y);
                    rotateSelected(global.baseX, global.baseY, global.angleNew - global.angleRef);
                    previewOff();
                    endCommand();
                }
            }
            else {
                global.angleNew = Number(str);
                rotateSelected(global.baseX, global.baseY, global.angleNew - global.angleRef);
                previewOff();
                endCommand();
            }
        }
    }
}

//Command: Sandbox

var global = {}; //Required
global.test1;
global.test2;

void main()
{
    initCommand();
    
    //Report number of pre-selected objects
    setPromptPrefix("Number of Objects Selected: " + numSelected().toString());
    appendPromptHistory();
    
    mirrorSelected(0,0,0,1);
    
    //selectAll();
    //rotateSelected(0,0,90);
    
    //Polyline & Polygon Testing
    
    var offsetX = 0.0;
    var offsetY = 0.0;
    
    var polylineArray = [];
    polylineArray.push(1.0 + offsetX);
    polylineArray.push(1.0 + offsetY);
    polylineArray.push(1.0 + offsetX);
    polylineArray.push(2.0 + offsetY);
    polylineArray.push(2.0 + offsetX);
    polylineArray.push(2.0 + offsetY);
    polylineArray.push(2.0 + offsetX);
    polylineArray.push(3.0 + offsetY);
    polylineArray.push(3.0 + offsetX);
    polylineArray.push(3.0 + offsetY);
    polylineArray.push(3.0 + offsetX);
    polylineArray.push(2.0 + offsetY);
    polylineArray.push(4.0 + offsetX);
    polylineArray.push(2.0 + offsetY);
    polylineArray.push(4.0 + offsetX);
    polylineArray.push(1.0 + offsetY);
    addPolyline(polylineArray);
    
    offsetX = 5.0;
    offsetY = 0.0;
    
    var polygonArray = [];
    polygonArray.push(1.0 + offsetX);
    polygonArray.push(1.0 + offsetY);
    polygonArray.push(1.0 + offsetX);
    polygonArray.push(2.0 + offsetY);
    polygonArray.push(2.0 + offsetX);
    polygonArray.push(2.0 + offsetY);
    polygonArray.push(2.0 + offsetX);
    polygonArray.push(3.0 + offsetY);
    polygonArray.push(3.0 + offsetX);
    polygonArray.push(3.0 + offsetY);
    polygonArray.push(3.0 + offsetX);
    polygonArray.push(2.0 + offsetY);
    polygonArray.push(4.0 + offsetX);
    polygonArray.push(2.0 + offsetY);
    polygonArray.push(4.0 + offsetX);
    polygonArray.push(1.0 + offsetY);
    addPolygon(polygonArray);
    

    endCommand();
}


//Command: Scale

var global = {}; //Required
global.firstRun;
global.baseX;
global.baseY;
global.destX;
global.destY;
global.factor;

global.baseRX;
global.baseRY;
global.destRX;
global.destRY;
global.factorRef;
global.factorNew;

global.mode;

//enums
global.mode_NORMAL    = 0;
global.mode_REFERENCE = 1;

void main()
{
    initCommand();
    global.mode = global.mode_NORMAL;
    global.firstRun = true;
    global.baseX  = NaN;
    global.baseY  = NaN;
    global.destX  = NaN;
    global.destY  = NaN;
    global.factor = NaN;

    global.baseRX    = NaN;
    global.baseRY    = NaN;
    global.destRX    = NaN;
    global.destRY    = NaN;
    global.factorRef = NaN;
    global.factorNew = NaN;

    if (numSelected() <= 0) {
        //TODO: Prompt to select objects if nothing is preselected
        alert(qsTr("Preselect objects before invoking the scale command."));
        endCommand();
        messageBox("information", qsTr("Scale Preselect"), qsTr("Preselect objects before invoking the scale command."));
    }
    else {
        setPromptPrefix(qsTr("Specify base point: "));
    }
}


void click(x, y)
{
    if (global.mode == global.mode_NORMAL) {
        if (global.firstRun) {
            global.firstRun = false;
            global.baseX = x;
            global.baseY = y;
            addRubber("LINE");
            setRubberMode("LINE");
            setRubberPoint("LINE_START", global.baseX, global.baseY);
            previewOn("SELECTED", "SCALE", global.baseX, global.baseY, 1);
            appendPromptHistory();
            setPromptPrefix(qsTr("Specify scale factor or [Reference]: "));
        }
        else {
            global.destX = x;
            global.destY = y;
            global.factor = calculateDistance(global.baseX, global.baseY, global.destX, global.destY);
            appendPromptHistory();
            scaleSelected(global.baseX, global.baseY, global.factor);
            previewOff();
            endCommand();
        }
    }
    else if (global.mode == global.mode_REFERENCE)
    {
        if (std::isnan(global.baseRX))
        {
            global.baseRX = x;
            global.baseRY = y;
            appendPromptHistory();
            addRubber("LINE");
            setRubberMode("LINE");
            setRubberPoint("LINE_START", global.baseRX, global.baseRY);
            setPromptPrefix(qsTr("Specify second point: "));
        }
        else if (std::isnan(global.destRX))
        {
            global.destRX = x;
            global.destRY = y;
            global.factorRef = calculateDistance(global.baseRX, global.baseRY, global.destRX, global.destRY);
            if (global.factorRef <= 0.0)
            {
                global.destRX    = NaN;
                global.destRY    = NaN;
                global.factorRef = NaN;
                alert(qsTr("Value must be positive and nonzero."));
                setPromptPrefix(qsTr("Specify second point: "));
            }
            else
            {
                appendPromptHistory();
                setRubberPoint("LINE_START", global.baseX, global.baseY);
                previewOn("SELECTED", "SCALE", global.baseX, global.baseY, global.factorRef);
                setPromptPrefix(qsTr("Specify new length: "));
            }
        }
        else if (std::isnan(global.factorNew))
        {
            global.factorNew = calculateDistance(global.baseX, global.baseY, x, y);
            if (global.factorNew <= 0.0)
            {
                global.factorNew = NaN;
                alert(qsTr("Value must be positive and nonzero."));
                setPromptPrefix(qsTr("Specify new length: "));
            }
            else
            {
                appendPromptHistory();
                scaleSelected(global.baseX, global.baseY, global.factorNew/global.factorRef);
                previewOff();
                endCommand();
            }
        }
    }
}

void context(str)
{
    todo("SCALE", "context()");
}

void prompt(str)
{
    if (global.mode == global.mode_NORMAL) {
        if (global.firstRun) {
            var strList = str.split(",");
            if (std::isnan(strList[0]) || isNaN(strList[1])) {
                alert(qsTr("Invalid point."));
                setPromptPrefix(qsTr("Specify base point: "));
            }
            else {
                global.firstRun = false;
                global.baseX = Number(strList[0]);
                global.baseY = Number(strList[1]);
                addRubber("LINE");
                setRubberMode("LINE");
                setRubberPoint("LINE_START", global.baseX, global.baseY);
                previewOn("SELECTED", "SCALE", global.baseX, global.baseY, 1);
                setPromptPrefix(qsTr("Specify scale factor or [Reference]: "));
            }
        }
        else
        {
            if (str == "R" || str == "REFERENCE") //TODO: Probably should add additional qsTr calls here.
            {
                global.mode = global.mode_REFERENCE;
                setPromptPrefix(qsTr("Specify reference length") + " {1}: ");
                clearRubber();
                previewOff();
            }
            else
            {
                if (std::isnan(str))
                {
                    alert(qsTr("Requires valid numeric distance, second point, or option keyword."));
                    setPromptPrefix(qsTr("Specify scale factor or [Reference]: "));
                }
                else
                {
                    global.factor = Number(str);
                    scaleSelected(global.baseX, global.baseY, global.factor);
                    previewOff();
                    endCommand();
                }
            }
        }
    }
    else if (global.mode == global.mode_REFERENCE)
    {
        if (std::isnan(global.baseRX))
        {
            if (std::isnan(str))
            {
                var strList = str.split(",");
                if (std::isnan(strList[0]) || isNaN(strList[1]))
                {
                    alert(qsTr("Requires valid numeric distance or two points."));
                    setPromptPrefix(qsTr("Specify reference length") + " {1}: ");
                }
                else
                {
                    global.baseRX = Number(strList[0]);
                    global.baseRY = Number(strList[1]);
                    addRubber("LINE");
                    setRubberMode("LINE");
                    setRubberPoint("LINE_START", global.baseRX, global.baseRY);
                    setPromptPrefix(qsTr("Specify second point: "));
                }
            }
            else
            {
                //The base and dest values are only set here to advance the command.
                global.baseRX = 0.0;
                global.baseRY = 0.0;
                global.destRX = 0.0;
                global.destRY = 0.0;
                //The reference length is what we will use later.
                global.factorRef = Number(str);
                if (global.factorRef <= 0.0)
                {
                    global.baseRX    = NaN;
                    global.baseRY    = NaN;
                    global.destRX    = NaN;
                    global.destRY    = NaN;
                    global.factorRef = NaN;
                    alert(qsTr("Value must be positive and nonzero."));
                    setPromptPrefix(qsTr("Specify reference length") + " {1}: ");
                }
                else
                {
                    addRubber("LINE");
                    setRubberMode("LINE");
                    setRubberPoint("LINE_START", global.baseX, global.baseY);
                    previewOn("SELECTED", "SCALE", global.baseX, global.baseY, global.factorRef);
                    setPromptPrefix(qsTr("Specify new length: "));
                }
            }
        }
        else if (std::isnan(global.destRX))
        {
            if (std::isnan(str))
            {
                var strList = str.split(",");
                if (std::isnan(strList[0]) || isNaN(strList[1]))
                {
                    alert(qsTr("Requires valid numeric distance or two points."));
                    setPromptPrefix(qsTr("Specify second point: "));
                }
                else
                {
                    global.destRX = Number(strList[0]);
                    global.destRY = Number(strList[1]);
                    global.factorRef = calculateDistance(global.baseRX, global.baseRY, global.destRX, global.destRY);
                    if (global.factorRef <= 0.0)
                    {
                        global.destRX    = NaN;
                        global.destRY    = NaN;
                        global.factorRef = NaN;
                        alert(qsTr("Value must be positive and nonzero."));
                        setPromptPrefix(qsTr("Specify second point: "));
                    }
                    else
                    {
                        setRubberPoint("LINE_START", global.baseX, global.baseY);
                        previewOn("SELECTED", "SCALE", global.baseX, global.baseY, global.factorRef);
                        setPromptPrefix(qsTr("Specify new length: "));
                    }
                }
            }
            else
            {
                //The base and dest values are only set here to advance the command.
                global.baseRX = 0.0;
                global.baseRY = 0.0;
                global.destRX = 0.0;
                global.destRY = 0.0;
                //The reference length is what we will use later.
                global.factorRef = Number(str);
                if (global.factorRef <= 0.0)
                {
                    global.destRX    = NaN;
                    global.destRY    = NaN;
                    global.factorRef = NaN;
                    alert(qsTr("Value must be positive and nonzero."));
                    setPromptPrefix(qsTr("Specify second point: "));
                }
                else
                {
                    setRubberPoint("LINE_START", global.baseX, global.baseY);
                    previewOn("SELECTED", "SCALE", global.baseX, global.baseY, global.factorRef);
                    setPromptPrefix(qsTr("Specify new length: "));
                }
            }
        }
        else if (std::isnan(global.factorNew))
        {
            if (std::isnan(str))
            {
                var strList = str.split(",");
                if (std::isnan(strList[0]) || isNaN(strList[1]))
                {
                    alert(qsTr("Requires valid numeric distance or second point."));
                    setPromptPrefix(qsTr("Specify new length: "));
                }
                else
                {
                    var x = Number(strList[0]);
                    var y = Number(strList[1]);
                    global.factorNew = calculateDistance(global.baseX, global.baseY, x, y);
                    if (global.factorNew <= 0.0)
                    {
                        global.factorNew = NaN;
                        alert(qsTr("Value must be positive and nonzero."));
                        setPromptPrefix(qsTr("Specify new length: "));
                    }
                    else
                    {
                        scaleSelected(global.baseX, global.baseY, global.factorNew/global.factorRef);
                        previewOff();
                        endCommand();
                    }
                }
            }
            else
            {
                global.factorNew = Number(str);
                if (global.factorNew <= 0.0)
                {
                    global.factorNew = NaN;
                    alert(qsTr("Value must be positive and nonzero."));
                    setPromptPrefix(qsTr("Specify new length: "));
                }
                else
                {
                    scaleSelected(global.baseX, global.baseY, global.factorNew/global.factorRef);
                    previewOff();
                    endCommand();
                }
            }
        }
    }
}


//Command: Single Line Text

var global = {}; //Required
global.text;
global.textX;
global.textY;
global.textJustify;
global.textFont;
global.textHeight;
global.textRotation;
global.mode;

//enums
global.mode_JUSTIFY = 0;
global.mode_SETFONT = 1;
global.mode_SETGEOM = 2;
global.mode_RAPID   = 3;

void main()
{
    initCommand();
    clearSelection();
    global.text = "";
    global.textX = NaN;
    global.textY = NaN;
    global.textJustify = "Left";
    global.textFont = textFont();
    global.textHeight = NaN;
    global.textRotation = NaN;
    global.mode = global.mode_SETGEOM;
    setPromptPrefix(qsTr("Current font: ") + "{" + global.textFont + "} " + qsTr("Text height: ") + "{" +  textSize() + "}");
    appendPromptHistory();
    setPromptPrefix(qsTr("Specify start point of text or [Justify/Setfont]: "));
}


void click(x, y)
{
    if (global.mode == global.mode_SETGEOM)
    {
        if (std::isnan(global.textX))
        {
            global.textX = x;
            global.textY = y;
            addRubber("LINE");
            setRubberMode("LINE");
            setRubberPoint("LINE_START", global.textX, global.textY);
            appendPromptHistory();
            setPromptPrefix(qsTr("Specify text height") + " {" + textSize() + "}: ");
        }
        else if (std::isnan(global.textHeight))
        {
            global.textHeight = calculateDistance(global.textX, global.textY, x, y);
            setTextSize(global.textHeight);
            appendPromptHistory();
            setPromptPrefix(qsTr("Specify text angle") + " {" + textAngle() + "}: ");
        }
        else if (std::isnan(global.textRotation))
        {
            global.textRotation = calculateAngle(global.textX, global.textY, x, y);
            setTextAngle(global.textRotation);
            appendPromptHistory();
            setPromptPrefix(qsTr("Enter text: "));
            global.mode = global.mode_RAPID;
            enablePromptRapidFire();
            clearRubber();
            addRubber("TEXTSINGLE");
            setRubberMode("TEXTSINGLE");
            setRubberPoint("TEXT_POINT", global.textX, global.textY);
            setRubberPoint("TEXT_HEIGHT_ROTATION", global.textHeight, global.textRotation);
            setRubberText("TEXT_FONT", global.textFont);
            setRubberText("TEXT_JUSTIFY", global.textJustify);
            setRubberText("TEXT_RAPID", global.text);
        }
        else
        {
            //Do nothing, as we are in rapidFire mode now.
        }
    }
}

void context(str)
{
    todo("SINGLELINETEXT", "context()");
}

void prompt(str)
{
    if (global.mode == global.mode_JUSTIFY)
    {
        if (str == "C" || str == "CENTER") {
            global.mode = global.mode_SETGEOM;
            global.textJustify = "Center";
            setRubberText("TEXT_JUSTIFY", global.textJustify);
            setPromptPrefix(qsTr("Specify center point of text or [Justify/Setfont]: "));
        }
        else if (str == "R" || str == "RIGHT") {
            global.mode = global.mode_SETGEOM;
            global.textJustify = "Right";
            setRubberText("TEXT_JUSTIFY", global.textJustify);
            setPromptPrefix(qsTr("Specify right-end point of text or [Justify/Setfont]: "));
        }
        else if (str == "A" || str == "ALIGN") {
            global.mode = global.mode_SETGEOM;
            global.textJustify = "Aligned";
            setRubberText("TEXT_JUSTIFY", global.textJustify);
            setPromptPrefix(qsTr("Specify start point of text or [Justify/Setfont]: "));
        }
        else if (str == "M" || str == "MIDDLE") {
            global.mode = global.mode_SETGEOM;
            global.textJustify = "Middle";
            setRubberText("TEXT_JUSTIFY", global.textJustify);
            setPromptPrefix(qsTr("Specify middle point of text or [Justify/Setfont]: "));
        }
        else if (str == "F" || str == "FIT") {
            global.mode = global.mode_SETGEOM;
            global.textJustify = "Fit";
            setRubberText("TEXT_JUSTIFY", global.textJustify);
            setPromptPrefix(qsTr("Specify start point of text or [Justify/Setfont]: "));
        }
        else if (str == "TL" || str == "TOPLEFT") {
            global.mode = global.mode_SETGEOM;
            global.textJustify = "Top Left";
            setRubberText("TEXT_JUSTIFY", global.textJustify);
            setPromptPrefix(qsTr("Specify top-left point of text or [Justify/Setfont]: "));
        }
        else if (str == "TC" || str == "TOPCENTER") {
            global.mode = global.mode_SETGEOM;
            global.textJustify = "Top Center";
            setRubberText("TEXT_JUSTIFY", global.textJustify);
            setPromptPrefix(qsTr("Specify top-center point of text or [Justify/Setfont]: "));
        }
        else if (str == "TR" || str == "TOPRIGHT") {
            global.mode = global.mode_SETGEOM;
            global.textJustify = "Top Right";
            setRubberText("TEXT_JUSTIFY", global.textJustify);
            setPromptPrefix(qsTr("Specify top-right point of text or [Justify/Setfont]: "));
        }
        else if (str == "ML" || str == "MIDDLELEFT") {
            global.mode = global.mode_SETGEOM;
            global.textJustify = "Middle Left";
            setRubberText("TEXT_JUSTIFY", global.textJustify);
            setPromptPrefix(qsTr("Specify middle-left point of text or [Justify/Setfont]: "));
        }
        else if (str == "MC" || str == "MIDDLECENTER") {
            global.mode = global.mode_SETGEOM;
            global.textJustify = "Middle Center";
            setRubberText("TEXT_JUSTIFY", global.textJustify);
            setPromptPrefix(qsTr("Specify middle-center point of text or [Justify/Setfont]: "));
        }
        else if (str == "MR" || str == "MIDDLERIGHT") {
            global.mode = global.mode_SETGEOM;
            global.textJustify = "Middle Right";
            setRubberText("TEXT_JUSTIFY", global.textJustify);
            setPromptPrefix(qsTr("Specify middle-right point of text or [Justify/Setfont]: "));
        }
        else if (str == "BL" || str == "BOTTOMLEFT") {
            global.mode = global.mode_SETGEOM;
            global.textJustify = "Bottom Left";
            setRubberText("TEXT_JUSTIFY", global.textJustify);
            setPromptPrefix(qsTr("Specify bottom-left point of text or [Justify/Setfont]: "));
        }
        else if (str == "BC" || str == "BOTTOMCENTER") {
            global.mode = global.mode_SETGEOM;
            global.textJustify = "Bottom Center";
            setRubberText("TEXT_JUSTIFY", global.textJustify);
            setPromptPrefix(qsTr("Specify bottom-center point of text or [Justify/Setfont]: "));
        }
        else if (str == "BR" || str == "BOTTOMRIGHT") {
            global.mode = global.mode_SETGEOM;
            global.textJustify = "Bottom Right";
            setRubberText("TEXT_JUSTIFY", global.textJustify);
            setPromptPrefix(qsTr("Specify bottom-right point of text or [Justify/Setfont]: "));
        }
        else
        {
            alert(qsTr("Invalid option keyword."));
            setPromptPrefix(qsTr("Text Justification Options [Center/Right/Align/Middle/Fit/TL/TC/TR/ML/MC/MR/BL/BC/BR]: "));
        }
    }
    else if (global.mode == global.mode_SETFONT) {
        global.mode = global.mode_SETGEOM;
        global.textFont = str;
        setRubberText("TEXT_FONT", global.textFont);
        setTextFont(global.textFont);
        setPromptPrefix(qsTr("Specify start point of text or [Justify/Setfont]: "));
    }
    else if (global.mode == global.mode_SETGEOM) {
        if (std::isnan(global.textX)) {
            if (str == "J" || str == "JUSTIFY") {
                global.mode = global.mode_JUSTIFY;
                setPromptPrefix(qsTr("Text Justification Options [Center/Right/Align/Middle/Fit/TL/TC/TR/ML/MC/MR/BL/BC/BR]: "));
            }
            else if (str == "S" || str == "SETFONT") {
                global.mode = global.mode_SETFONT;
                setPromptPrefix(qsTr("Specify font name: "));
            }
            else {
                var strList = str.split(",");
                if (std::isnan(strList[0]) || isNaN(strList[1]))
                {
                    alert(qsTr("Point or option keyword required."));
                    setPromptPrefix(qsTr("Specify start point of text or [Justify/Setfont]: "));
                }
                else
                {
                    global.textX = Number(strList[0]);
                    global.textY = Number(strList[1]);
                    addRubber("LINE");
                    setRubberMode("LINE");
                    setRubberPoint("LINE_START", global.textX, global.textY);
                    setPromptPrefix(qsTr("Specify text height") + " {" + textSize() + "}: ");
                }
            }
        }
        else if (std::isnan(global.textHeight)) {
            if (str == "") {
                global.textHeight = textSize();
                setPromptPrefix(qsTr("Specify text angle") + " {" + textAngle() + "}: ");
            }
            else if (std::isnan(str)) {
                alert(qsTr("Requires valid numeric distance or second point."));
                setPromptPrefix(qsTr("Specify text height") + " {" + textSize() + "}: ");
            }
            else {
                global.textHeight = Number(str);
                setTextSize(global.textHeight);
                setPromptPrefix(qsTr("Specify text angle") + " {" + textAngle() + "}: ");
            }
        }
        else if (std::isnan(global.textRotation))
        {
            if (str == "")
            {
                global.textRotation = textAngle();
                setPromptPrefix(qsTr("Enter text: "));
                global.mode = global.mode_RAPID;
                enablePromptRapidFire();
                clearRubber();
                addRubber("TEXTSINGLE");
                setRubberMode("TEXTSINGLE");
                setRubberPoint("TEXT_POINT", global.textX, global.textY);
                setRubberPoint("TEXT_HEIGHT_ROTATION", global.textHeight, global.textRotation);
                setRubberText("TEXT_FONT", global.textFont);
                setRubberText("TEXT_JUSTIFY", global.textJustify);
                setRubberText("TEXT_RAPID", global.text);
            }
            else if (std::isnan(str))
            {
                alert(qsTr("Requires valid numeric angle or second point."));
                setPromptPrefix(qsTr("Specify text angle") + " {" + textAngle() + "}: ");
            }
            else
            {
                global.textRotation = Number(str);
                setTextAngle(global.textRotation);
                setPromptPrefix(qsTr("Enter text: "));
                global.mode = global.mode_RAPID;
                enablePromptRapidFire();
                clearRubber();
                addRubber("TEXTSINGLE");
                setRubberMode("TEXTSINGLE");
                setRubberPoint("TEXT_POINT", global.textX, global.textY);
                setRubberPoint("TEXT_HEIGHT_ROTATION", global.textHeight, global.textRotation);
                setRubberText("TEXT_FONT", global.textFont);
                setRubberText("TEXT_JUSTIFY", global.textJustify);
                setRubberText("TEXT_RAPID", global.text);
            }
        }
        else {
            //Do nothing, as we are in rapidFire mode now.
        }
    }
    else if (global.mode == global.mode_RAPID) {
        if (str == "RAPID_ENTER") {
            if (global.text == "") {
                endCommand();
            }
            else {
                vulcanize();
                endCommand(); //TODO: Rather than ending the command, calculate where the next line would be and modify the x/y to the new point
            }
        }
        else {
            global.text = str;
            setRubberText("TEXT_RAPID", global.text);
        }
    }
}

//Command: Snowflake

var global = {}; //Required
global.numPoints = 2048; //Default //TODO: min:64 max:8192
global.cx;
global.cy;
global.sx = 0.04; //Default
global.sy = 0.04; //Default
global.numPoints;
global.mode;

//enums
global.mode_NUM_POINTS = 0;
global.mode_XSCALE     = 1;
global.mode_YSCALE     = 2;

void main()
{
    initCommand();
    clearSelection();
    global.cx = NaN;
    global.cy = NaN;
    global.mode = global.mode_NUM_POINTS;

    addRubber("POLYGON");
    setRubberMode("POLYGON");
    updateSnowflake(global.numPoints, global.sx, global.sy);
    spareRubber("POLYGON");
    endCommand();
}

void updateSnowflake(numPts, xScale, yScale)
{
    var i;
    var t;
    var xx = NaN;
    var yy = NaN;
    var two_pi = 2*PI;

    for(i = 0; i <= numPts; i++)
    {
        t = two_pi/numPts*i; 

//Snowflake Curve with t [0,2pi]

xx = 4/7*sin(20/11-318*t)+
3/13*sin(19/11-317*t)+
3/5*sin(21/16-316*t)+
1/6*sin(17/5-315*t)+
2/9*sin(20/19-314*t)+
5/9*sin(35/9-313*t)+
7/12*sin(9/8-310*t)+
5/16*sin(33/8-309*t)+
5/11*sin(31/11-308*t)+
4/7*sin(3/8-307*t)+
4/11*sin(9/8-306*t)+
7/8*sin(21/11-305*t)+
2/3*sin(55/13-304*t)+
5/9*sin(17/7-303*t)+
3/10*sin(3/13-302*t)+
4/11*sin(60/17-301*t)+
6/11*sin(48/11-300*t)+
9/19*sin(1/6-299*t)+
4/5*sin(19/11-298*t)+
7/13*sin(25/8-297*t)+
7/11*sin(19/7-296*t)+
1/2*sin(1-295*t)+
4/9*sin(24/11-294*t)+
1/3*sin(7/2-291*t)+
6/17*sin(15/13-290*t)+
11/17*sin(32/7-288*t)+
3/8*sin(33/8-287*t)+
4/7*sin(15/7-286*t)+
4/5*sin(48/11-284*t)+
6/7*sin(10/7-283*t)+
6/7*sin(20/11-282*t)+
3/8*sin(11/7-281*t)+
5/7*sin(23/6-280*t)+
1/21*sin(19/12-279*t)+
4/9*sin(1/5-278*t)+
5/8*sin(5/9-276*t)+
9/10*sin(2/3-274*t)+
5/8*sin(5/11-273*t)+
1/6*sin(9/2-272*t)+
12/25*sin(29/12-271*t)+
7/13*sin(59/15-270*t)+
5/7*sin(23/9-269*t)+
3/4*sin(9/2-268*t)+
5/11*sin(37/9-267*t)+
10/11*sin(11/7-266*t)+
1/3*sin(3/7-264*t)+
7/9*sin(33/17-262*t)+
5/8*sin(9/8-261*t)+
5/8*sin(38/13-260*t)+
11/21*sin(36/13-259*t)+
3/11*sin(1/29-258*t)+
8/15*sin(31/8-257*t)+
2/5*sin(3/13-256*t)+
1/2*sin(47/10-255*t)+
1/10*sin(33/10-254*t)+
2/5*sin(1/2-253*t)+
4/7*sin(33/7-252*t)+
6/17*sin(3/8-250*t)+
5/7*sin(25/9-249*t)+
7/9*sin(35/8-248*t)+
2/7*sin(81/20-247*t)+
5/8*sin(25/6-244*t)+
5/16*sin(11/21-243*t)+
11/13*sin(167/42-242*t)+
11/15*sin(18/5-241*t)+
13/14*sin(37/11-240*t)+
1/4*sin(20/9-239*t)+
9/14*sin(52/15-238*t)+
9/14*sin(17/14-237*t)+
6/13*sin(69/17-236*t)+
5/8*sin(74/21-235*t)+
7/15*sin(76/25-234*t)+
10/11*sin(15/8-232*t)+
5/11*sin(5/9-230*t)+
1/8*sin(8/3-229*t)+
5/9*sin(2/7-227*t)+
4/13*sin(32/9-226*t)+
2/3*sin(45/11-225*t)+
1/30*sin(53/15-223*t)+
7/11*sin(4/11-222*t)+
10/19*sin(31/13-221*t)+
sin(13/7-219*t)+
9/14*sin(33/7-216*t)+
2/3*sin(19/9-215*t)+
3/5*sin(27/11-214*t)+
9/11*sin(43/10-210*t)+
5/7*sin(13/8-209*t)+
5/9*sin(21/5-208*t)+
2/7*sin(14/9-206*t)+
9/8*sin(23/7-205*t)+
18/13*sin(11/9-203*t)+
7/4*sin(47/12-201*t)+
10/7*sin(8/9-200*t)+
7/10*sin(6/11-199*t)+
5/3*sin(7/6-198*t)+
19/11*sin(11/6-196*t)+
15/8*sin(9/8-195*t)+
8/17*sin(9/7-192*t)+
8/3*sin(39/10-191*t)+
23/10*sin(2/7-188*t)+
3/4*sin(3/5-187*t)+
7/12*sin(50/11-185*t)+
57/29*sin(4-184*t)+
9/8*sin(6/7-183*t)+
9/7*sin(15/13-182*t)+
5/13*sin(16/7-181*t)+
18/7*sin(5/14-180*t)+
17/9*sin(35/12-179*t)+
5/4*sin(5/7-178*t)+
22/23*sin(3/4-176*t)+
3/8*sin(48/13-175*t)+
15/11*sin(13/11-174*t)+
25/17*sin(23/5-173*t)+
18/11*sin(19/8-172*t)+
11/16*sin(5/3-170*t)+
39/38*sin(15/7-169*t)+
7/6*sin(36/11-166*t)+
15/11*sin(11/6-163*t)+
17/13*sin(3-162*t)+
11/9*sin(20/7-161*t)+
9/7*sin(35/9-160*t)+
7/6*sin(3/2-159*t)+
8/7*sin(9/10-158*t)+
12/25*sin(13/5-156*t)+
6/13*sin(25/13-154*t)+
9/13*sin(7/8-152*t)+
23/10*sin(33/14-151*t)+
8/11*sin(36/11-150*t)+
15/7*sin(26/7-149*t)+
6/5*sin(53/12-148*t)+
14/11*sin(3/2-147*t)+
9/8*sin(4/3-146*t)+
5/8*sin(18/13-145*t)+
15/7*sin(3/8-143*t)+
5/8*sin(5/6-142*t)+
6/7*sin(35/9-139*t)+
16/13*sin(1/2-138*t)+
9/4*sin(7/2-137*t)+
20/9*sin(15/8-135*t)+
11/8*sin(9/4-134*t)+
sin(19/10-133*t)+
22/7*sin(48/11-132*t)+
23/14*sin(1-131*t)+
19/9*sin(27/8-130*t)+
19/5*sin(20/7-129*t)+
18/5*sin(76/25-128*t)+
27/8*sin(4/5-126*t)+
37/8*sin(3/8-125*t)+
62/11*sin(11/3-124*t)+
49/11*sin(7/6-123*t)+
21/22*sin(23/12-122*t)+
223/74*sin(11/3-121*t)+
11/5*sin(19/5-120*t)+
13/4*sin(33/13-119*t)+
27/8*sin(22/5-117*t)+
24/7*sin(13/7-114*t)+
69/17*sin(18/17-113*t)+
10/9*sin(2/7-112*t)+
133/66*sin(12/7-111*t)+
2/5*sin(47/24-110*t)+
13/5*sin(11/6-108*t)+
16/7*sin(39/11-105*t)+
11/5*sin(25/9-104*t)+
151/50*sin(19/7-103*t)+
19/7*sin(12/5-101*t)+
26/7*sin(101/25-99*t)+
43/21*sin(41/14-98*t)+
13/3*sin(31/9-97*t)+
10/13*sin(1-95*t)+
17/7*sin(39/10-93*t)+
145/48*sin(3-92*t)+
37/6*sin(47/13-91*t)+
5/6*sin(36/13-89*t)+
9/4*sin(3/7-87*t)+
48/13*sin(26/17-86*t)+
7/3*sin(28/19-82*t)+
31/6*sin(8/7-81*t)+
36/7*sin(12/7-80*t)+
38/9*sin(25/9-79*t)+
17/2*sin(37/14-76*t)+
16/3*sin(19/20-75*t)+
81/16*sin(4/5-74*t)+
67/10*sin(19/15-73*t)+
40/11*sin(32/11-72*t)+
71/13*sin(21/20-71*t)+
68/15*sin(46/15-70*t)+
52/15*sin(27/10-69*t)+
57/14*sin(7/8-67*t)+
7/4*sin(42/13-66*t)+
39/11*sin(43/21-65*t)+
30/11*sin(33/8-64*t)+
7/5*sin(20/7-63*t)+
4/7*sin(13/14-62*t)+
39/10*sin(16/9-61*t)+
7/6*sin(137/34-59*t)+
16/13*sin(107/27-58*t)+
26/27*sin(17/5-57*t)+
4/3*sin(9/14-56*t)+
46/11*sin(5/3-55*t)+
11/6*sin(13/4-54*t)+
19/4*sin(17/5-53*t)+
19/7*sin(43/11-52*t)+
25/12*sin(30/7-51*t)+
15/7*sin(5/11-50*t)+
53/5*sin(21/13-49*t)+
62/13*sin(67/15-48*t)+
122/9*sin(48/13-47*t)+
20/13*sin(1-46*t)+
7/6*sin(32/7-43*t)+
12/7*sin(13/25-42*t)+
11/17*sin(9/10-40*t)+
11/9*sin(2-39*t)+
4/3*sin(19/7-38*t)+
12/5*sin(47/11-37*t)+
10/7*sin(12/7-36*t)+
108/17*sin(3/4-35*t)+
25/9*sin(19/5-34*t)+
7/13*sin(22/5-33*t)+
9/4*sin(13/11-32*t)+
181/15*sin(25/11-31*t)+
202/11*sin(57/13-29*t)+
2/11*sin(26/7-28*t)+
129/13*sin(38/15-25*t)+
13/6*sin(1/8-24*t)+
77/13*sin(11/8-23*t)+
19/6*sin(15/7-22*t)+
18/7*sin(29/10-21*t)+
9*sin(13/5-18*t)+
342/7*sin(11/6-17*t)+
3/5*sin(49/11-15*t)+
38/3*sin(19/7-14*t)+
994/9*sin(25/8-13*t)+
22/9*sin(49/12-10*t)+
97/9*sin(1/14-8*t)+
559/7*sin(47/14-7*t)+
19/13*sin(5/6-6*t)+
3*sin(57/17-4*t)+
28/5*sin(1-3*t)+
10/3*sin(22/7-2*t)+
1507/3*sin(29/8-t)-
1407/13*sin(5*t+8/11)-
15/2*sin(9*t+2/5)-
1193/9*sin(11*t+28/27)-
209/15*sin(12*t+2/5)-
116/15*sin(16*t+40/39)-
1105/33*sin(19*t+1/3)-
45/13*sin(20*t+7/6)-
91/46*sin(26*t+4/7)-
43/16*sin(27*t+12/11)-
46/13*sin(30*t+14/9)-
29/10*sin(41*t+3/14)-
31/11*sin(44*t+15/14)-
22/7*sin(45*t+10/7)-
7/8*sin(60*t+22/15)-
54/53*sin(68*t+5/4)-
214/15*sin(77*t+5/9)-
54/11*sin(78*t+1/13)-
47/6*sin(83*t+5/11)-
1/2*sin(84*t+8/7)-
2/3*sin(85*t+4/9)-
7/3*sin(88*t+7/6)-
15/4*sin(90*t+1/6)-
35/6*sin(94*t+17/18)-
77/26*sin(96*t+2/7)-
64/11*sin(100*t+34/23)-
13/6*sin(102*t+14/11)-
19/7*sin(106*t+5/6)-
13/6*sin(107*t+10/11)-
42/13*sin(109*t+8/7)-
69/35*sin(115*t+10/21)-
12/7*sin(116*t+17/16)-
8/3*sin(118*t+5/9)-
1/6*sin(127*t+17/12)-
13/7*sin(136*t+8/7)-
7/10*sin(140*t+7/5)-
15/7*sin(141*t+19/14)-
6/11*sin(144*t+5/16)-
3/2*sin(153*t+9/14)-
6/5*sin(155*t+3/10)-
3/8*sin(157*t+10/11)-
20/11*sin(164*t+19/14)-
7/5*sin(165*t+7/6)-
8/13*sin(167*t+20/13)-
7/8*sin(168*t+3/7)-
5/14*sin(171*t+16/13)-
22/7*sin(177*t+3/13)-
23/8*sin(186*t+7/8)-
13/7*sin(189*t+11/9)-
9/5*sin(190*t+32/21)-
27/28*sin(193*t+1)-
5/12*sin(194*t+1/2)-
44/43*sin(197*t+6/5)-
5/11*sin(202*t+1/5)-
8/7*sin(204*t+1/23)-
16/15*sin(207*t+7/10)-
1/2*sin(211*t+2/5)-
5/8*sin(212*t+3/5)-
10/13*sin(213*t+6/5)-
21/16*sin(217*t+4/3)-
11/5*sin(218*t+24/25)-
2/3*sin(220*t+5/9)-
13/10*sin(224*t+7/8)-
17/8*sin(228*t+1/9)-
3/7*sin(231*t+14/9)-
5/12*sin(233*t+9/11)-
3/5*sin(245*t+4/7)-
2/3*sin(246*t+15/11)-
3/8*sin(251*t+4/7)-
2/9*sin(263*t+19/20)-
1/2*sin(265*t+13/11)-
3/8*sin(275*t+3/2)-
17/35*sin(277*t+9/13)-
3/7*sin(285*t+3/11)-
9/10*sin(289*t+25/19)-
4/9*sin(292*t+20/13)-
12/25*sin(293*t+5/4)-
3/5*sin(311*t+9/8)-
33/32*sin(312*t+1/2);

yy = 3/7*sin(24/11-318*t)+
5/12*sin(3-317*t)+
5/14*sin(21/16-316*t)+
9/19*sin(31/9-315*t)+
2/9*sin(13/6-314*t)+
3/5*sin(9/7-312*t)+
2/5*sin(49/12-311*t)+
1/13*sin(30/7-310*t)+
4/13*sin(19/12-309*t)+
1/3*sin(32/7-307*t)+
5/8*sin(22/5-306*t)+
4/11*sin(25/11-305*t)+
8/15*sin(9/8-304*t)+
1/8*sin(35/9-303*t)+
3/5*sin(51/25-302*t)+
2/5*sin(9/8-301*t)+
4/7*sin(2/7-300*t)+
2/7*sin(50/11-299*t)+
3/13*sin(35/8-297*t)+
5/14*sin(14/5-295*t)+
8/13*sin(47/14-294*t)+
2/9*sin(25/8-293*t)+
8/17*sin(136/45-291*t)+
2/7*sin(17/7-290*t)+
3/5*sin(8/7-288*t)+
3/13*sin(19/8-286*t)+
6/11*sin(10/19-285*t)+
9/10*sin(121/40-283*t)+
8/5*sin(21/5-282*t)+
1/10*sin(87/25-281*t)+
7/13*sin(22/7-279*t)+
3/7*sin(8/5-278*t)+
4/5*sin(3/14-277*t)+
7/10*sin(19/13-276*t)+
1/5*sin(6/13-274*t)+
7/10*sin(20/9-273*t)+
1/3*sin(9/4-272*t)+
4/13*sin(47/11-271*t)+
18/17*sin(22/7-269*t)+
1/7*sin(31/9-268*t)+
7/10*sin(43/17-267*t)+
8/11*sin(24/7-266*t)+
5/8*sin(13/6-264*t)+
9/10*sin(17/13-262*t)+
4/11*sin(31/8-261*t)+
1/5*sin(66/19-260*t)+
1/10*sin(23/5-259*t)+
3/10*sin(66/19-255*t)+
1/8*sin(6/7-253*t)+
9/13*sin(16/5-252*t)+
3/7*sin(8/9-251*t)+
4/11*sin(30/13-250*t)+
7/11*sin(66/19-247*t)+
1/19*sin(2-246*t)+
1/4*sin(16/7-245*t)+
8/17*sin(41/10-244*t)+
15/16*sin(2/11-240*t)+
5/7*sin(19/18-239*t)+
1/6*sin(5/12-238*t)+
5/11*sin(16/17-236*t)+
3/10*sin(25/12-235*t)+
8/17*sin(16/7-233*t)+
5/8*sin(47/12-231*t)+
9/11*sin(11/8-230*t)+
3/11*sin(33/7-229*t)+
9/10*sin(20/7-226*t)+
4/9*sin(39/14-225*t)+
4/9*sin(10/9-224*t)+
6/7*sin(19/13-222*t)+
7/9*sin(29/7-221*t)+
8/11*sin(33/8-220*t)+
16/9*sin(2/7-219*t)+
25/14*sin(1/8-218*t)+
8/11*sin(5/9-217*t)+
9/11*sin(11/10-216*t)+
21/13*sin(27/7-215*t)+
3/7*sin(1/12-213*t)+
13/9*sin(15/16-212*t)+
23/8*sin(1/8-210*t)+
sin(32/11-209*t)+
9/13*sin(1/9-208*t)+
7/9*sin(33/10-206*t)+
2/3*sin(9/4-205*t)+
3/4*sin(1/2-204*t)+
3/13*sin(11/17-203*t)+
3/7*sin(31/12-202*t)+
19/12*sin(17/8-201*t)+
7/8*sin(75/19-200*t)+
6/5*sin(21/10-198*t)+
3/2*sin(7/5-194*t)+
28/27*sin(3/2-193*t)+
4/9*sin(16/5-192*t)+
22/13*sin(13/6-189*t)+
18/11*sin(19/10-188*t)+
sin(7/6-187*t)+
16/7*sin(13/11-186*t)+
9/5*sin(11/9-184*t)+
16/11*sin(2/5-183*t)+
10/13*sin(10/3-182*t)+
9/7*sin(38/9-181*t)+
45/13*sin(8/9-180*t)+
7/9*sin(35/8-179*t)+
2/3*sin(35/8-176*t)+
10/7*sin(6/19-175*t)+
40/13*sin(15/7-174*t)+
20/13*sin(1/2-173*t)+
3/11*sin(20/7-171*t)+
17/16*sin(50/11-169*t)+
2/9*sin(1/31-168*t)+
4/9*sin(7/2-165*t)+
1/12*sin(26/17-164*t)+
21/22*sin(27/26-163*t)+
13/12*sin(17/8-162*t)+
19/14*sin(39/10-160*t)+
18/11*sin(5/7-159*t)+
3/5*sin(15/14-158*t)+
11/9*sin(35/8-157*t)+
5/8*sin(30/7-156*t)+
3/2*sin(28/11-155*t)+
4/5*sin(5/11-151*t)+
25/19*sin(11/10-150*t)+
10/11*sin(11/14-148*t)+
13/9*sin(7/4-147*t)+
7/13*sin(19/6-146*t)+
1/5*sin(37/14-145*t)+
11/8*sin(42/13-144*t)+
20/11*sin(32/9-143*t)+
2/3*sin(22/5-141*t)+
10/11*sin(9/7-140*t)+
8/7*sin(23/9-138*t)+
5/2*sin(9/19-137*t)+
7/5*sin(193/48-136*t)+
5/8*sin(67/66-135*t)+
8/7*sin(7/15-134*t)+
13/6*sin(13/7-133*t)+
19/7*sin(16/5-132*t)+
16/7*sin(39/11-131*t)+
28/17*sin(69/35-130*t)+
84/17*sin(7/8-129*t)+
114/23*sin(10/9-128*t)+
29/11*sin(1/7-127*t)+
63/10*sin(65/32-124*t)+
74/17*sin(37/16-121*t)+
31/16*sin(35/11-120*t)+
19/5*sin(23/12-119*t)+
82/27*sin(27/7-118*t)+
49/11*sin(8/3-117*t)+
29/14*sin(63/16-116*t)+
9/13*sin(35/8-114*t)+
29/19*sin(5/4-113*t)+
13/7*sin(20/7-112*t)+
9/7*sin(11/23-111*t)+
19/8*sin(27/26-110*t)+
sin(4/7-109*t)+
119/40*sin(22/5-108*t)+
7/5*sin(47/46-107*t)+
5/3*sin(1/6-106*t)+
2*sin(14/5-105*t)+
7/3*sin(10/3-104*t)+
3/2*sin(15/4-103*t)+
19/11*sin(3/4-102*t)+
74/17*sin(13/10-99*t)+
98/33*sin(26/11-98*t)+
36/11*sin(13/3-97*t)+
43/12*sin(26/25-96*t)+
13/2*sin(3/13-95*t)+
6/7*sin(24/7-94*t)+
16/5*sin(6/5-93*t)+
5/7*sin(9/14-92*t)+
55/12*sin(27/14-90*t)+
15/11*sin(14/3-88*t)+
7/3*sin(7/10-87*t)+
11/4*sin(2/9-86*t)+
13/4*sin(35/12-84*t)+
26/9*sin(38/9-83*t)+
7/2*sin(5/7-82*t)+
31/8*sin(27/8-78*t)+
91/6*sin(35/8-77*t)+
37/5*sin(7/10-76*t)+
70/13*sin(17/11-73*t)+
76/25*sin(56/19-70*t)+
19/8*sin(17/8-68*t)+
59/13*sin(42/17-67*t)+
28/17*sin(49/13-64*t)+
9/7*sin(79/17-63*t)+
1/8*sin(7/11-62*t)+
39/8*sin(49/15-61*t)+
53/18*sin(33/8-59*t)+
9/7*sin(41/9-58*t)+
8/7*sin(65/14-57*t)+
10/11*sin(16/7-56*t)+
68/13*sin(42/13-55*t)+
21/10*sin(7/8-54*t)+
6/7*sin(41/14-53*t)+
31/11*sin(55/12-51*t)+
59/17*sin(27/7-50*t)+
124/9*sin(37/11-49*t)+
24/11*sin(3/5-48*t)+
65/6*sin(12/5-47*t)+
11/7*sin(49/11-45*t)+
13/25*sin(11/13-42*t)+
7/4*sin(5/8-40*t)+
43/42*sin(2/5-39*t)+
20/9*sin(4/7-38*t)+
19/8*sin(4/11-37*t)+
5/4*sin(15/4-36*t)+
1/5*sin(11/13-34*t)+
12/7*sin(23/5-32*t)+
409/34*sin(39/10-31*t)+
10/7*sin(5/2-30*t)+
180/11*sin(3-29*t)+
23/8*sin(53/12-26*t)+
71/8*sin(56/13-25*t)+
12/5*sin(10/21-24*t)+
10/3*sin(34/9-22*t)+
27/16*sin(12/11-21*t)+
49/6*sin(13/7-20*t)+
69/2*sin(19/14-19*t)+
475/9*sin(3/10-17*t)+
68/13*sin(57/28-16*t)+
40/17*sin(1/6-15*t)+
77/13*sin(29/11-12*t)+
4954/39*sin(15/4-11*t)+
1075/11*sin(4-5*t)+
191/24*sin(5/4-4*t)+
84/17*sin(2/7-3*t)-
12/5*sin(74*t)-
4/5*sin(166*t)-
1523/3*sin(t+12/11)-
25/3*sin(2*t+17/18)-
13/8*sin(6*t+1/9)-
5333/62*sin(7*t+9/7)-
56/9*sin(8*t+5/12)-
65/8*sin(9*t+2/5)-
106/9*sin(10*t+1/8)-
1006/9*sin(13*t+11/7)-
67/8*sin(14*t+6/5)-
25/8*sin(18*t+15/11)-
40/11*sin(23*t+1/16)-
4/7*sin(27*t+6/5)-
41/8*sin(28*t+7/12)-
8/5*sin(33*t+5/6)-
137/17*sin(35*t+4/5)-
29/12*sin(41*t+22/15)-
25/9*sin(43*t+6/7)-
12/25*sin(44*t+16/11)-
31/6*sin(46*t+4/3)-
19/5*sin(52*t+16/13)-
19/11*sin(60*t+8/17)-
16/7*sin(65*t+6/13)-
25/12*sin(66*t+11/13)-
8/9*sin(69*t+4/11)-
25/7*sin(71*t+7/5)-
11/10*sin(72*t+3/2)-
14/5*sin(75*t+7/9)-
107/14*sin(79*t+3/4)-
67/8*sin(80*t+2/11)-
161/27*sin(81*t+5/11)-
55/18*sin(85*t+3/7)-
161/40*sin(89*t+1/21)-
32/7*sin(91*t+38/25)-
sin(100*t+19/20)-
27/5*sin(101*t+2/13)-
26/9*sin(115*t+1/44)-
17/11*sin(122*t+1/16)-
87/22*sin(123*t+2/3)-
37/8*sin(125*t+9/11)-
10/7*sin(126*t+8/7)-
7/8*sin(139*t+3/5)-
3/7*sin(142*t+5/6)-
71/36*sin(149*t+5/16)-
7/6*sin(152*t+1/9)-
63/25*sin(153*t+29/19)-
27/20*sin(154*t+8/15)-
8/15*sin(161*t+12/13)-
5/3*sin(167*t+13/10)-
17/25*sin(170*t+3/5)-
10/9*sin(172*t+3/8)-
5/7*sin(177*t+5/8)-
1/2*sin(178*t+7/6)-
34/13*sin(185*t+5/8)-
11/13*sin(190*t+38/39)-
25/19*sin(191*t+11/8)-
11/12*sin(195*t+18/19)-
51/26*sin(196*t+2/7)-
14/9*sin(197*t+4/11)-
19/12*sin(199*t+1)-
19/11*sin(207*t+11/8)-
6/11*sin(211*t+1/20)-
11/7*sin(214*t+1/14)-
7/13*sin(223*t+8/11)-
3/5*sin(227*t+12/13)-
4/5*sin(228*t+29/19)-
11/10*sin(232*t+2/7)-
1/6*sin(234*t+7/11)-
sin(237*t+60/59)-
5/11*sin(241*t+7/8)-
1/2*sin(242*t+8/7)-
7/15*sin(243*t+15/16)-
5/8*sin(248*t+2/3)-
1/3*sin(249*t+4/11)-
2/3*sin(254*t+8/7)-
10/19*sin(256*t+14/11)-
4/9*sin(257*t+8/11)-
3/4*sin(258*t+3/7)-
sin(263*t+2/7)-
3/10*sin(265*t+1/28)-
1/2*sin(270*t+1)-
12/13*sin(275*t+5/8)-
1/4*sin(280*t+16/13)-
1/10*sin(284*t+5/8)-
13/25*sin(287*t+3/7)-
9/13*sin(289*t+3/5)-
22/23*sin(292*t+17/13)-
9/11*sin(296*t+17/11)-
3/7*sin(298*t+12/11)-
5/6*sin(308*t+1/2)-
7/15*sin(313*t+1/3);

        setRubberPoint("POLYGON_POINT_" + i.toString(), xx*xScale, yy*yScale);
    }

    setRubberText("POLYGON_NUM_POINTS", numPts.toString());
}

//Command: Star

var global = {}; //Required
global.numPoints = 5; //Default
global.cx;
global.cy;
global.x1;
global.y1;
global.x2;
global.y2;
global.mode;

//enums
global.mode_NUM_POINTS = 0;
global.mode_CENTER_PT  = 1;
global.mode_RAD_OUTER  = 2;
global.mode_RAD_INNER  = 3;

void main()
{
    initCommand();
    clearSelection();
    global.cx       = NaN;
    global.cy       = NaN;
    global.x1       = NaN;
    global.y1       = NaN;
    global.x2       = NaN;
    global.y2       = NaN;
    global.mode = global.mode_NUM_POINTS;
    setPromptPrefix(qsTr("Enter number of star points") + " {" + global.numPoints.toString() + "}: ");
}


void click(x, y)
{
    if (global.mode == global.mode_NUM_POINTS)
    {
        //Do nothing, the prompt controls this.
    }
    else if (global.mode == global.mode_CENTER_PT)
    {
        global.cx = x;
        global.cy = y;
        global.mode = global.mode_RAD_OUTER;
        setPromptPrefix(qsTr("Specify outer radius of star: "));
        addRubber("POLYGON");
        setRubberMode("POLYGON");
        updateStar(global.cx, global.cy);
        enableMoveRapidFire();
    }
    else if (global.mode == global.mode_RAD_OUTER)
    {
        global.x1 = x;
        global.y1 = y;
        global.mode = global.mode_RAD_INNER;
        setPromptPrefix(qsTr("Specify inner radius of star: "));
        updateStar(global.x1, global.y1);
    }
    else if (global.mode == global.mode_RAD_INNER)
    {
        global.x2 = x;
        global.y2 = y;
        disableMoveRapidFire();
        updateStar(global.x2, global.y2);
        spareRubber("POLYGON");
        endCommand();
    }
}

void move(x, y)
{
    if (global.mode == global.mode_NUM_POINTS)
    {
        //Do nothing, the prompt controls this.
    }
    else if (global.mode == global.mode_CENTER_PT)
    {
        //Do nothing, prompt and click controls this.
    }
    else if (global.mode == global.mode_RAD_OUTER)
    {
        updateStar(x, y);
    }
    else if (global.mode == global.mode_RAD_INNER)
    {
        updateStar(x, y);
    }
}

void context(str)
{
    todo("STAR", "context()");
}

void prompt(str)
{
    if (global.mode == global.mode_NUM_POINTS)
    {
        if (str == "" && global.numPoints >= 3 && global.numPoints <= 1024)
        {
            setPromptPrefix(qsTr("Specify center point: "));
            global.mode = global.mode_CENTER_PT;
        }
        else
        {
            var tmp = Number(str);
            if (std::isnan(tmp) || !isInt(tmp) || tmp < 3 || tmp > 1024)
            {
                alert(qsTr("Requires an integer between 3 and 1024."));
                setPromptPrefix(qsTr("Enter number of star points") + " {" + global.numPoints.toString() + "}: ");
            }
            else
            {
                global.numPoints = tmp;
                setPromptPrefix(qsTr("Specify center point: "));
                global.mode = global.mode_CENTER_PT;
            }
        }
    }
    else if (global.mode == global.mode_CENTER_PT)
    {
        var strList = str.split(",");
        if (std::isnan(strList[0]) || isNaN(strList[1]))
        {
            alert(qsTr("Invalid point."));
            setPromptPrefix(qsTr("Specify center point: "));
        }
        else
        {
            global.cx = Number(strList[0]);
            global.cy = Number(strList[1]);
            global.mode = global.mode_RAD_OUTER;
            setPromptPrefix(qsTr("Specify outer radius of star: "));
            addRubber("POLYGON");
            setRubberMode("POLYGON");
            updateStar(qsnapX(), qsnapY());
            enableMoveRapidFire();
        }
    }
    else if (global.mode == global.mode_RAD_OUTER)
    {
        var strList = str.split(",");
        if (std::isnan(strList[0]) || isNaN(strList[1]))
        {
            alert(qsTr("Invalid point."));
            setPromptPrefix(qsTr("Specify outer radius of star: "));
        }
        else
        {
            global.x1 = Number(strList[0]);
            global.y1 = Number(strList[1]);
            global.mode = global.mode_RAD_INNER;
            setPromptPrefix(qsTr("Specify inner radius of star: "));
            updateStar(qsnapX(), qsnapY());
        }
    }
    else if (global.mode == global.mode_RAD_INNER)
    {
        var strList = str.split(",");
        if (std::isnan(strList[0]) || isNaN(strList[1]))
        {
            alert(qsTr("Invalid point."));
            setPromptPrefix(qsTr("Specify inner radius of star: "));
        }
        else
        {
            global.x2 = Number(strList[0]);
            global.y2 = Number(strList[1]);
            disableMoveRapidFire();
            updateStar(global.x2, global.y2);
            spareRubber("POLYGON");
            endCommand();
        }
    }
}

void
updateStar(x, y)
{
    var distOuter;
    var distInner;
    var angOuter;

    if (global.mode == global.mode_RAD_OUTER)
    {
        angOuter = calculateAngle(global.cx, global.cy, x, y);
        distOuter = calculateDistance(global.cx, global.cy, x, y);
        distInner = distOuter/2.0;
    }
    else if (global.mode == global.mode_RAD_INNER)
    {
        angOuter = calculateAngle(global.cx, global.cy, global.x1, global.y1);
        distOuter = calculateDistance(global.cx, global.cy, global.x1, global.y1);
        distInner = calculateDistance(global.cx, global.cy, x, y);
    }

    //Calculate the Star Points
    var angInc = 360.0/(global.numPoints*2);
    var odd = true;
    for(var i = 0; i < global.numPoints*2; i++)
    {
        var xx;
        var yy;
        if (odd)
        {
            xx = distOuter*cos((angOuter+(angInc*i))*PI/180.0);
            yy = distOuter*sin((angOuter+(angInc*i))*PI/180.0);
        }
        else
        {
            xx = distInner*cos((angOuter+(angInc*i))*PI/180.0);
            yy = distInner*sin((angOuter+(angInc*i))*PI/180.0);
        }
        odd = !odd;
        setRubberPoint("POLYGON_POINT_" + i.toString(), global.cx + xx, global.cy + yy);
    }
    setRubberText("POLYGON_NUM_POINTS", (global.numPoints*2 - 1).toString());
}

void
syswindows_main(void)
{
    initCommand();
    clearSelection();
    setPromptPrefix(qsTr("Enter an option [Cascade/Tile]: "));
}

void
syswindows_prompt(std::string str)
{
    if (str == "C" || str == "CASCADE") {
        windowCascade();
        endCommand();
    }
    else if (str == "T" || str == "TILE") {
        windowTile();
        endCommand();
    }
    else {
        alert(qsTr("Invalid option keyword."));
        setPromptPrefix(qsTr("Enter an option [Cascade/Tile]: "));
    }
}

#endif
