/*
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

#include "embroidermodder.h"

#if 0
MainWindow *mainWin;
std::string settings_general_language;
std::string settings_general_icon_theme;

QIcon load_icon(std::string icon)
{
    std::string appDir = qApp->applicationDirPath();
    return QIcon(appDir + "/icons/" + settings_general_icon_theme + "/" + icon + ".png");
}

void createAllActions()
{
    debug_message("Creating All Actions...");
    for (int i=0; action_list[i].action>=0; i++) {
        std::string icon = action_list[i].command;
        std::string toolTip = translate(action_list[i].menu_label);
        std::string statusTip = translate(action_list[i].description);
        QAction *ACTION = new QAction(load_icon(icon), toolTip, this);
        ACTION->setStatusTip(statusTip);
        ACTION->setObjectName(icon);
        // TODO: Set What's This Context Help to statusTip for now so there is some infos there.
        // Make custom whats this context help popup with more descriptive help than just
        // the status bar/tip one liner(short but not real long) with a hyperlink in the custom popup
        // at the bottom to open full help file description. Ex: like wxPython AGW's SuperToolTip.
        ACTION->setWhatsThis(statusTip);
        // TODO: Finish All Commands ... <.<

        if (icon == "textbold") {
            ACTION->setCheckable(true);
            connect(ACTION, SIGNAL(toggled(bool)), this, SLOT(setTextBold(bool)));
        }
        else if (icon == "textitalic") {
            ACTION->setCheckable(true);
            connect(ACTION, SIGNAL(toggled(bool)), this, SLOT(setTextItalic(bool)));
        }
        else if (icon == "textunderline") {
            ACTION->setCheckable(true);
            connect(ACTION, SIGNAL(toggled(bool)), this, SLOT(setTextUnderline(bool)));
        }
        else if (icon == "textstrikeout") {
            ACTION->setCheckable(true);
            connect(ACTION, SIGNAL(toggled(bool)), this, SLOT(setTextStrikeOut(bool)));
        }
        else if (icon == "textoverline") {
            ACTION->setCheckable(true);
            connect(ACTION, SIGNAL(toggled(bool)), this, SLOT(setTextOverline(bool)));
        }
        else {
            connect(ACTION, &QAction::triggered, this, [=](){
                actuator(ACTION->objectName().toStdString());
            });
        }

        if (icon == "new") {
            ACTION->setShortcut(QKeySequence::New);
        }
        else if (icon == "open") {
            ACTION->setShortcut(QKeySequence::Open);
        }
        else if (icon == "save") {
            ACTION->setShortcut(QKeySequence::Save);
        }
        else if (icon == "saveas") {
            ACTION->setShortcut(QKeySequence::SaveAs);
        }
        else if (icon == "print") {
            ACTION->setShortcut(QKeySequence::Print);
        }
        else if (icon == "designdetails") {
            ACTION->setShortcut(QKeySequence("Ctrl+D"));
        }
        else if (icon == "exit") {
            ACTION->setShortcut(QKeySequence("Ctrl+Q"));
        }
        else if (icon == "cut") {
            ACTION->setShortcut(QKeySequence::Cut);
        }
        else if (icon == "copy") {
            ACTION->setShortcut(QKeySequence::Copy);
        }
        else if (icon == "paste") {
            ACTION->setShortcut(QKeySequence::Paste);
        }
        else if (icon == "windowclose") {
            ACTION->setShortcut(QKeySequence::Close);
        }
        else if (icon == "windownext") {
            ACTION->setShortcut(QKeySequence::NextChild);
        }
        else if (icon == "windowprevious") {
            ACTION->setShortcut(QKeySequence::PreviousChild);
        }
        actionHash.insert(action_list[i].action, ACTION);
    }

    actionHash.value(ACTION_windowclose)->setEnabled(numOfDocs > 0);
    actionHash.value(ACTION_designdetails)->setEnabled(numOfDocs > 0);
}


int toolbar_zoom_entries[] = {
    ACTION_zoomwindow,
    ACTION_zoomdynamic,
    ACTION_zoomscale,
    -1,
    ACTION_zoomcenter,
    ACTION_zoomin,
    ACTION_zoomout,
    -1,
    ACTION_zoomselected,
    ACTION_zoomall,
    ACTION_zoomextents,
    -2
};

int toolbar_pan_entries[] = {
    ACTION_panrealtime,
    ACTION_panpoint,
    -1,
    ACTION_panleft,
    ACTION_panright,
    ACTION_panup,
    ACTION_pandown,
    -2
};

int toolbar_icon_entries[] = {
    ACTION_icon16,
    ACTION_icon24,
    ACTION_icon32,
    ACTION_icon48,
    ACTION_icon64,
    ACTION_icon128,
    -2
};

int toolbar_help_entries[] = {
    ACTION_help,
    -1,
    ACTION_changelog,
    -1,
    ACTION_about,
    -1,
    ACTION_whatsthis,
    -2
};

void add_to_toolbar(QToolBar *toolbar, std::string toolbar_description, int entries[])
{
    debug_message("MainWindow add_to_toolbar");
    debug_message(toolbar_description);

    toolbar->setObjectName(toolbar_description);
    for (int i=0; entries[i] >= -1; i++) {
        if (entries[i]>-1) {
            toolbar->addAction(actionHash.value(entries[i]));
        }
        else {
            toolbarFile->addSeparator();
        }
    }

    connect(toolbar, SIGNAL(topLevelChanged(bool)), this, SLOT(floatingChangedToolBar(bool)));
}

void createLayerToolbar()
{
    debug_message("MainWindow createLayerToolbar()");

    toolbarLayer->setObjectName("toolbarLayer");
    toolbarLayer->addAction(actionHash.value(ACTION_makelayercurrent));
    toolbarLayer->addAction(actionHash.value(ACTION_layers));

    layerSelector->setFocusProxy(prompt);
    //NOTE: Qt4.7 wont load icons without an extension...
    //TODO: Create layer pixmaps by concatenating several icons
    layerSelector->addItem(load_icon("linetypebylayer"), "0");
    layerSelector->addItem(load_icon("linetypebylayer"), "1");
    layerSelector->addItem(load_icon("linetypebylayer"), "2");
    layerSelector->addItem(load_icon("linetypebylayer"), "3");
    layerSelector->addItem(load_icon("linetypebylayer"), "4");
    layerSelector->addItem(load_icon("linetypebylayer"), "5");
    layerSelector->addItem(load_icon("linetypebylayer"), "6");
    layerSelector->addItem(load_icon("linetypebylayer"), "7");
    layerSelector->addItem(load_icon("linetypebylayer"), "8");
    layerSelector->addItem(load_icon("linetypebylayer"), "9");
    toolbarLayer->addWidget(layerSelector);
    connect(layerSelector, SIGNAL(currentIndexChanged(int)), this, SLOT(layerSelectorIndexChanged(int)));

    toolbarLayer->addAction(actionHash.value(ACTION_layerprevious));

    connect(toolbarLayer, SIGNAL(topLevelChanged(bool)), this, SLOT(floatingChangedToolBar(bool)));
}

void createPropertiesToolbar()
{
    debug_message("MainWindow createPropertiesToolbar()");

    toolbarProperties->setObjectName("toolbarProperties");

    colorSelector->setFocusProxy(prompt);
    colorSelector->addItem(load_icon("colorbylayer"), "ByLayer");
    colorSelector->addItem(load_icon("colorbyblock"), "ByBlock");
    colorSelector->addItem(load_icon("colorred"    ), translate("Red"),     qRgb(255,  0,  0));
    colorSelector->addItem(load_icon("coloryellow" ), translate("Yellow"),  qRgb(255,255,  0));
    colorSelector->addItem(load_icon("colorgreen"  ), translate("Green"),   qRgb(  0,255,  0));
    colorSelector->addItem(load_icon("colorcyan"   ), translate("Cyan"),    qRgb(  0,255,255));
    colorSelector->addItem(load_icon("colorblue"   ), translate("Blue"),    qRgb(  0,  0,255));
    colorSelector->addItem(load_icon("colormagenta"), translate("Magenta"), qRgb(255,  0,255));
    colorSelector->addItem(load_icon("colorwhite"  ), translate("White"),   qRgb(255,255,255));
    colorSelector->addItem(load_icon("colorother"  ), translate("Other..."));
    toolbarProperties->addWidget(colorSelector);
    connect(colorSelector, SIGNAL(currentIndexChanged(int)), this, SLOT(colorSelectorIndexChanged(int)));

    toolbarProperties->addSeparator();
    linetypeSelector->setFocusProxy(prompt);
    linetypeSelector->addItem(load_icon("linetypebylayer"   ), "ByLayer");
    linetypeSelector->addItem(load_icon("linetypebyblock"   ), "ByBlock");
    linetypeSelector->addItem(load_icon("linetypecontinuous"), "Continuous");
    linetypeSelector->addItem(load_icon("linetypehidden"    ), "Hidden");
    linetypeSelector->addItem(load_icon("linetypecenter"    ), "Center");
    linetypeSelector->addItem(load_icon("linetypeother"     ), "Other...");
    toolbarProperties->addWidget(linetypeSelector);
    connect(linetypeSelector, SIGNAL(currentIndexChanged(int)), this, SLOT(linetypeSelectorIndexChanged(int)));

    toolbarProperties->addSeparator();
    lineweightSelector->setFocusProxy(prompt);
    lineweightSelector->addItem(load_icon("lineweightbylayer"), "ByLayer", -2.00);
    lineweightSelector->addItem(load_icon("lineweightbyblock"), "ByBlock", -1.00);
    lineweightSelector->addItem(load_icon("lineweightdefault"), "Default",  0.00);
    //TODO: Thread weight is weird. See http://en.wikipedia.org/wiki/Thread_(yarn)#Weight
    lineweightSelector->addItem(load_icon("lineweight01"), "0.00 mm", 0.00);
    lineweightSelector->addItem(load_icon("lineweight02"), "0.05 mm", 0.05);
    lineweightSelector->addItem(load_icon("lineweight03"), "0.15 mm", 0.15);
    lineweightSelector->addItem(load_icon("lineweight04"), "0.20 mm", 0.20);
    lineweightSelector->addItem(load_icon("lineweight05"), "0.25 mm", 0.25);
    lineweightSelector->addItem(load_icon("lineweight06"), "0.30 mm", 0.30);
    lineweightSelector->addItem(load_icon("lineweight07"), "0.35 mm", 0.35);
    lineweightSelector->addItem(load_icon("lineweight08"), "0.40 mm", 0.40);
    lineweightSelector->addItem(load_icon("lineweight09"), "0.45 mm", 0.45);
    lineweightSelector->addItem(load_icon("lineweight10"), "0.50 mm", 0.50);
    lineweightSelector->addItem(load_icon("lineweight11"), "0.55 mm", 0.55);
    lineweightSelector->addItem(load_icon("lineweight12"), "0.60 mm", 0.60);
    lineweightSelector->addItem(load_icon("lineweight13"), "0.65 mm", 0.65);
    lineweightSelector->addItem(load_icon("lineweight14"), "0.70 mm", 0.70);
    lineweightSelector->addItem(load_icon("lineweight15"), "0.75 mm", 0.75);
    lineweightSelector->addItem(load_icon("lineweight16"), "0.80 mm", 0.80);
    lineweightSelector->addItem(load_icon("lineweight17"), "0.85 mm", 0.85);
    lineweightSelector->addItem(load_icon("lineweight18"), "0.90 mm", 0.90);
    lineweightSelector->addItem(load_icon("lineweight19"), "0.95 mm", 0.95);
    lineweightSelector->addItem(load_icon("lineweight20"), "1.00 mm", 1.00);
    lineweightSelector->addItem(load_icon("lineweight21"), "1.05 mm", 1.05);
    lineweightSelector->addItem(load_icon("lineweight22"), "1.10 mm", 1.10);
    lineweightSelector->addItem(load_icon("lineweight23"), "1.15 mm", 1.15);
    lineweightSelector->addItem(load_icon("lineweight24"), "1.20 mm", 1.20);
    lineweightSelector->setMinimumContentsLength(8); // Prevent dropdown text readability being squish...d.
    toolbarProperties->addWidget(lineweightSelector);
    connect(lineweightSelector, SIGNAL(currentIndexChanged(int)), this, SLOT(lineweightSelectorIndexChanged(int)));

    connect(toolbarProperties, SIGNAL(topLevelChanged(bool)), this, SLOT(floatingChangedToolBar(bool)));
}

void createTextToolbar()
{
    debug_message("MainWindow createTextToolbar()");

    toolbarText->setObjectName("toolbarText");

    toolbarText->addWidget(textFontSelector);
    textFontSelector->setCurrentFont(QFont(settings_text_font));
    connect(textFontSelector, SIGNAL(currentFontChanged(const QFont&)), this, SLOT(textFontSelectorCurrentFontChanged(const QFont&)));

    toolbarText->addAction(actionHash.value(ACTION_textbold));
    actionHash.value(ACTION_textbold)->setChecked(settings_text_style_bold);
    toolbarText->addAction(actionHash.value(ACTION_textitalic));
    actionHash.value(ACTION_textitalic)->setChecked(settings_text_style_italic);
    toolbarText->addAction(actionHash.value(ACTION_textunderline));
    actionHash.value(ACTION_textunderline)->setChecked(settings_text_style_underline);
    toolbarText->addAction(actionHash.value(ACTION_textstrikeout));
    actionHash.value(ACTION_textstrikeout)->setChecked(settings_text_style_strikeout);
    toolbarText->addAction(actionHash.value(ACTION_textoverline));
    actionHash.value(ACTION_textoverline)->setChecked(settings_text_style_overline);

    textSizeSelector->setFocusProxy(prompt);
    textSizeSelector->addItem("6 pt",   6);
    textSizeSelector->addItem("8 pt",   8);
    textSizeSelector->addItem("9 pt",   9);
    textSizeSelector->addItem("10 pt", 10);
    textSizeSelector->addItem("11 pt", 11);
    textSizeSelector->addItem("12 pt", 12);
    textSizeSelector->addItem("14 pt", 14);
    textSizeSelector->addItem("18 pt", 18);
    textSizeSelector->addItem("24 pt", 24);
    textSizeSelector->addItem("30 pt", 30);
    textSizeSelector->addItem("36 pt", 36);
    textSizeSelector->addItem("48 pt", 48);
    textSizeSelector->addItem("60 pt", 60);
    textSizeSelector->addItem("72 pt", 72);
    setTextSize(settings_text_size);
    toolbarText->addWidget(textSizeSelector);
    connect(textSizeSelector, SIGNAL(currentIndexChanged(int)), this, SLOT(textSizeSelectorIndexChanged(int)));

    connect(toolbarText, SIGNAL(topLevelChanged(bool)), this, SLOT(floatingChangedToolBar(bool)));
}

void createPromptToolbar()
{
    debug_message("MainWindow createPromptToolbar()");

    toolbarPrompt->setObjectName("toolbarPrompt");
    toolbarPrompt->addWidget(prompt);
    toolbarPrompt->setAllowedAreas(Qt::TopToolBarArea | Qt::BottomToolBarArea);
    connect(toolbarPrompt, SIGNAL(topLevelChanged(bool)), prompt, SLOT(floatingChanged(bool)));
}

void createAllToolbars()
{
    debug_message("MainWindow createAllToolbars()");

    add_to_toolbar(toolbarFile, "toolbarFile", toolbar_file_entries);
    add_to_toolbar(toolbarEdit, "toolbarEdit", toolbar_edit_entries);
    add_to_toolbar(toolbarView, "toolbarView", toolbar_view_entries);
    add_to_toolbar(toolbarZoom, "toolbarZoom", toolbar_zoom_entries);
    add_to_toolbar(toolbarPan, "toolbarPan", toolbar_pan_entries);
    add_to_toolbar(toolbarIcon, "toolbarIcon", toolbar_icon_entries);
    add_to_toolbar(toolbarHelp, "toolbarHelp", toolbar_help_entries);

    createLayerToolbar();
    createPropertiesToolbar();
    createTextToolbar();
    createPromptToolbar();

    // Horizontal
    toolbarView->setOrientation(Qt::Horizontal);
    toolbarZoom->setOrientation(Qt::Horizontal);
    toolbarLayer->setOrientation(Qt::Horizontal);
    toolbarProperties->setOrientation(Qt::Horizontal);
    toolbarText->setOrientation(Qt::Horizontal);
    toolbarPrompt->setOrientation(Qt::Horizontal);
    // Top
    addToolBarBreak(Qt::TopToolBarArea);
    addToolBar(Qt::TopToolBarArea, toolbarFile);
    addToolBar(Qt::TopToolBarArea, toolbarEdit);
    addToolBar(Qt::TopToolBarArea, toolbarHelp);
    addToolBar(Qt::TopToolBarArea, toolbarIcon);
    addToolBarBreak(Qt::TopToolBarArea);
    addToolBar(Qt::TopToolBarArea, toolbarZoom);
    addToolBar(Qt::TopToolBarArea, toolbarPan);
    addToolBar(Qt::TopToolBarArea, toolbarView);
    addToolBarBreak(Qt::TopToolBarArea);
    addToolBar(Qt::TopToolBarArea, toolbarLayer);
    addToolBar(Qt::TopToolBarArea, toolbarProperties);
    addToolBarBreak(Qt::TopToolBarArea);
    addToolBar(Qt::TopToolBarArea, toolbarText);
    // Bottom
    addToolBar(Qt::BottomToolBarArea, toolbarPrompt);

    //zoomToolBar->setToolButtonStyle(Qt::ToolButtonTextOnly);
}

void create_menu(QHash<int, QAction*> actionHash, QMenu *menu, int menu_data[], bool tear_off)
{
    for (int i=0; menu_data[i]>=-1; i++) {
        if (menu_data[i]>=0) {
            menu->addAction(actionHash.value(menu_data[i]));
        }
        else {
            menu->addSeparator();
        }
    }
    menu->setTearOffEnabled(tear_off);
}

void createAllMenus()
{
    debug_message("MainWindow createAllMenus()");

    menuBar()->addMenu(fileMenu);
    fileMenu->addAction(actionHash.value(ACTION_new));
    fileMenu->addSeparator();
    fileMenu->addAction(actionHash.value(ACTION_open));

    fileMenu->addMenu(recentMenu);
    connect(recentMenu, SIGNAL(aboutToShow()), this, SLOT(recentMenuAboutToShow()));
    //Do not allow the Recent Menu to be torn off. It's a pain in the ass to maintain.
    recentMenu->setTearOffEnabled(false);
    create_menu(actionHash, fileMenu, file_menu_data, false);

    menuBar()->addMenu(editMenu);
    create_menu(actionHash, editMenu, edit_menu_data, true);

    menuBar()->addMenu(viewMenu);
    viewMenu->addSeparator();
    viewMenu->addMenu(zoomMenu);
    zoomMenu->setIcon(load_icon("zoom"));
    create_menu(actionHash, zoomMenu, zoom_menu_data, true);
    viewMenu->addMenu(panMenu);
    panMenu->setIcon(load_icon("pan"));
    create_menu(actionHash, panMenu, pan_menu_data, true);
    create_menu(actionHash, viewMenu, view_menu_data, true);

    menuBar()->addMenu(settingsMenu);
    create_menu(actionHash, settingsMenu, settings_menu_data, true);

    menuBar()->addMenu(windowMenu);
    connect(windowMenu, SIGNAL(aboutToShow()), this, SLOT(windowMenuAboutToShow()));
    // Do not allow the Window Menu to be torn off. It's a pain in the ass to maintain.
    windowMenu->setTearOffEnabled(false);

    menuBar()->addMenu(helpMenu);
    create_menu(actionHash, helpMenu, help_menu_data, true);
}

void
check_load_file(std::string path)
{
    std::string appDir = qApp->applicationDirPath();
    QFileInfo check(appDir + std::string::fromLocal8Bit(path));
    if (!check.exists()) {
        QMessageBox::critical(this, translate("Path Error"), translate("Cannot locate: ")
                              + check.absoluteFilePath());
    }
}

MainWindow() : QMainWindow(0)
{
    readSettings();

    std::string appDir = qApp->applicationDirPath();
    // Verify that files/directories needed are actually present.
    check_load_file("/designs");
    check_load_file("/docs");
    check_load_file("/icons");
    check_load_file("/images");
    check_load_file("/samples");
    check_load_file("/translations");

    debug_message("language: " + settings_general_language.toStdString());
    if (settings_general_language == "system") {
        settings_general_language = QLocale::system().languageToString(QLocale::system().language()).toLocal8Bit().toLower();
    }

    //Load translations for the Embroidermodder 2 GUI
    QTranslator translatorEmb;
    std::string language = settings_general_language;
    translatorEmb.load(appDir + "/translations/" + language + "/embroidermodder2_" + language);
    qApp->installTranslator(&translatorEmb);

    //Load translations for the commands
    QTranslator translatorCmd;
    translatorCmd.load(appDir + "/translations/" + language + "/commands_" + language);
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

    setWindowIcon(load_icon("app"));
    setMinimumSize(800, 480); //Require Minimum WVGA

    loadFormats();

    //create the mdiArea
    QFrame* vbox = new QFrame(this);
    QVBoxLayout* layout = new QVBoxLayout(vbox);
    /* layout->setMargin(0); */
    vbox->setFrameStyle(QFrame::StyledPanel | QFrame::Sunken);
    mdiArea = new MdiArea(this, vbox);
    mdiArea->useBackgroundLogo(settings_general_mdi_bg_use_logo);
    mdiArea->useBackgroundTexture(settings_general_mdi_bg_use_texture);
    mdiArea->useBackgroundColor(settings_general_mdi_bg_use_color);
    mdiArea->setBackgroundLogo(settings_general_mdi_bg_logo);
    mdiArea->setBackgroundTexture(settings_general_mdi_bg_texture);
    mdiArea->setBackgroundColor(QColor(settings_general_mdi_bg_color));
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

    prompt->setPromptTextColor(QColor(settings_prompt_text_color));
    prompt->setPromptBackgroundColor(QColor(settings_prompt_bg_color));

    connect(prompt, SIGNAL(startCommand(const std::string&)), this, SLOT(logPromptInput(const std::string&)));

    connect(prompt, SIGNAL(startCommand(const std::string&)), this, SLOT(runCommandMain(const std::string&)));
    connect(prompt, SIGNAL(runCommand(const std::string&, const std::string&)), this, SLOT(runCommandPrompt(const std::string&, const std::string&)));

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

            connect(prompt, SIGNAL(historyAppended(const std::string&)), this, SLOT(promptHistoryAppended(const std::string&)));

    //create the Object Property Editor
    std::string s = appDir + "/icons/" + settings_general_icon_theme;
    dockPropEdit = new PropertyEditor(s, settings_selection_mode_pickadd, prompt, this);
    addDockWidget(Qt::LeftDockWidgetArea, dockPropEdit);
    connect(dockPropEdit, SIGNAL(pickAddModeToggled()), this, SLOT(pickAddModeToggled()));

    //create the Command History Undo Editor
    dockUndoEdit = new UndoEditor(s, prompt, this);
    addDockWidget(Qt::LeftDockWidgetArea, dockUndoEdit);

    //setDockOptions(QAnimatedDocks | QAllowTabbedDocks | QVerticalTabs); //TODO: Load these from settings
    //tabifyDockWidget(dockPropEdit, dockUndoEdit); //TODO: load this from settings

    statusbar = new StatusBar(this, this);
    this->setStatusBar(statusbar);

    createAllActions();
    createAllMenus();
    createAllToolbars();

    iconResize(settings_general_icon_size);
    updateMenuToolbarStatusbar();

    //Show date in statusbar after it has been updated
    QDate date = QDate::currentDate();
    std::string datestr = date.toString("MMMM d, yyyy");
    statusbar->showMessage(datestr);

    showNormal();

    //Load tips from external file
    std::vector<std::string> tips;

    QFile tipFile(appDir + "/docs/tips.txt");
    if (tipFile.open(QFile::ReadOnly)) {
        QTextStream stream(&tipFile);
        std::string tipLine;
        do {
            tipLine = stream.readLine();
            if (!tipLine.isEmpty())
                listTipOfTheDay << tipLine;
        }
        while(!tipLine.isNull());
    }
    if (settings_general_tip_of_the_day) {
        tipOfTheDay();
    }
}

~MainWindow()
{
    debug_message("Destructor()");

    //Prevent memory leaks by deleting any unpasted objects
    qDeleteAll(cutCopyObjectList.begin(), cutCopyObjectList.end());
    cutCopyObjectList.clear();
}

QAction* getAction(int actionEnum)
{
    return actionHash.value(actionEnum);
}

void recentMenuAboutToShow()
{
    debug_message("recentMenuAboutToShow()");
    recentMenu->clear();

    QFileInfo recentFileInfo;
    std::string recentValue;
    for (int i = 0; i < settings_opensave_recent_list_of_files.size(); ++i) {
        //If less than the max amount of entries add to menu
        if (i < settings_opensave_recent_max_files) {
            recentFileInfo = QFileInfo(settings_opensave_recent_list_of_files.at(i));
            if (recentFileInfo.exists() && validFileFormat(recentFileInfo.fileName())) {
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

void windowMenuAboutToShow()
{
    debug_message("windowMenuAboutToShow()");
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
    std::vector<QMdiSubWindow*> windows = mdiArea->subWindowList();
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

void windowMenuActivated(bool checked)
{
    debug_message("windowMenuActivated()");
    QAction* aSender = qobject_cast<QAction*>(sender());
    if (!aSender)
        return;
    QWidget* w = mdiArea->subWindowList().at(aSender->data().toInt());
    if (w && checked)
        w->setFocus();
}

void newFile()
{
    debug_message("newFile()");
    pattern_index++;
    n_patterns++;
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

void openFile(bool recent, const std::string& recentFile)
{
    debug_message("openFile()");

    QApplication::setOverrideCursor(Qt::ArrowCursor);

    std::stringList files;
    bool preview = settings_opensave_open_thumbnail;
    openFilesPath = settings_opensave_recent_directory;

    // Check to see if this from the recent files list
    if (recent) {
        files.append(recentFile);
        openFilesSelected(files);
    }
    else {
        if (!preview) {
            // TODO: set getOpenFileNames' selectedFilter parameter from settings_opensave_open_format
            files = QFileDialog::getOpenFileNames(this, translate("Open"), openFilesPath, formatFilterOpen);
            openFilesSelected(files);
        }
        else {
            PreviewDialog* openDialog = new PreviewDialog(this, translate("Open w/Preview"), openFilesPath, formatFilterOpen);
            //TODO: set openDialog->selectNameFilter(const std::string& filter) from settings_opensave_open_format
            connect(openDialog, SIGNAL(filesSelected(const std::stringList&)), this, SLOT(openFilesSelected(const std::stringList&)));
            openDialog->exec();
        }
    }

    QApplication::restoreOverrideCursor();
}

void openFilesSelected(const std::stringList& filesToOpen)
{
    bool doOnce = true;

    if (filesToOpen.count()) {
        for (int i = 0; i < filesToOpen.count(); i++) {
            if (!validFileFormat(filesToOpen[i])) {
                continue;
            }

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

            if (mdiWin->loadFile(filesToOpen.at(i))) {
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
            else {
                mdiWin->close();
            }
        }
    }

    windowMenuAboutToShow();
}

void openrecentfile()
{
    debug_message("openrecentfile()");

    //Check to see if this from the recent files list
    QAction* recentSender = qobject_cast<QAction*>(sender());
    if (recentSender) {
        openFile(true, recentSender->data().toString());
    }
}

void savefile()
{
    debug_message("savefile()");
}

void saveasfile()
{
    debug_message("saveasfile()");
    // need to find the activeSubWindow before it loses focus to the FileDialog
    MdiWindow* mdiWin = qobject_cast<MdiWindow*>(mdiArea->activeSubWindow());
    if (!mdiWin) {
        return;
    }

    std::string file;
    openFilesPath = settings_opensave_recent_directory;
    file = QFileDialog::getSaveFileName(this, translate("Save As"), openFilesPath, formatFilterSave);

    mdiWin->saveFile(file);
}

QMdiSubWindow* findMdiWindow(const std::string& fileName)
{
    debug_message("findMdiWindow(" + fileName.toStdString() + ")");
    std::string canonicalFilePath = QFileInfo(fileName).canonicalFilePath();

    foreach (QMdiSubWindow* subWindow, mdiArea->subWindowList()) {
        MdiWindow* mdiWin = qobject_cast<MdiWindow*>(subWindow);
        if (mdiWin) {
            if (mdiWin->getCurrentFile() == canonicalFilePath) {
                return subWindow;
            }
        }
    }
    return 0;
}

void closeEvent(QCloseEvent* event)
{
    mdiArea->closeAllSubWindows();
    writeSettings();
    event->accept();
}

void onCloseWindow()
{
    debug_message("onCloseWindow()");
    MdiWindow* mdiWin = qobject_cast<MdiWindow*>(mdiArea->activeSubWindow());
    if (mdiWin) {
        onCloseMdiWin(mdiWin);
    }
}

void onCloseMdiWin(MdiWindow* theMdiWin)
{
    debug_message("onCloseMdiWin()");
    numOfDocs--;

    bool keepMaximized;
    if (theMdiWin) { keepMaximized = theMdiWin->isMaximized(); }

    mdiArea->removeSubWindow(theMdiWin);
    theMdiWin->deleteLater();

    updateMenuToolbarStatusbar();
    windowMenuAboutToShow();

    if (keepMaximized)
    {
        MdiWindow* mdiWin = qobject_cast<MdiWindow*>(mdiArea->activeSubWindow());
        if (mdiWin) { mdiWin->showMaximized(); }
    }
}

void onWindowActivated(QMdiSubWindow* w)
{
    debug_message("onWindowActivated()");
    MdiWindow* mdiWin = qobject_cast<MdiWindow*>(w);
    if (mdiWin) {
        mdiWin->onWindowActivated();
    }
}

void resizeEvent(QResizeEvent* e)
{
    debug_message("resizeEvent()");
    QresizeEvent(e);
    statusBar()->setSizeGripEnabled(!isMaximized());
}

QAction* getFileSeparator()
{
    debug_message("getFileSeparator()");
    return myFileSeparator;
}

void updateMenuToolbarStatusbar()
{
    debug_message("updateMenuToolbarStatusbar()");

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

        foreach(QToolBar* tb, toolbarHash)
        {
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

        foreach(QMenu* menu, menuHash)
        {
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
        foreach(QToolBar* tb, toolbarHash)
        {
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

void hideUnimplemented()
{
    debug_message("hideUnimplemented()");
}

bool validFileFormat(const std::string& fileName)
{
    /* TODO: need to extract the extention */
    if (emb_identify_format(qPrintable(fileName))) {
        return true;
    }
    return false;
}

void loadFormats()
{
    char stable, unstable;
    std::string supportedReaders  = "All Supported Files (";
    std::string individualReaders = "All Files (*);;";
    std::string supportedWriters  = "All Supported Files (";
    std::string individualWriters = "All Files (*);;";
    std::string supportedStr;
    std::string individualStr;

    //TODO: Stable Only (Settings Option)
    //stable = 'S'; unstable = 'S';

    //Stable + Unstable
    stable = 'S'; unstable = 'U';

    const char* extension = 0;
    const char* description = 0;
    char readerState;
    char writerState;

    EmbFormatList* curFormat = 0;
    for (int i=0; i < numberOfFormats; i++) {
        extension = formatTable[i].extension;
        description = formatTable[i].description;
        readerState = formatTable[i].reader_state;
        writerState = formatTable[i].writer_state;

        std::string upperExt = std::string(extension).toUpper();
        supportedStr = "*" + upperExt + " ";
        individualStr = upperExt.replace(".", "") + " - " + description + " (*" + extension + ");;";
        if (readerState == stable || readerState == unstable) {
            //Exclude color file formats from open dialogs
            if (upperExt != "COL" && upperExt != "EDR" && upperExt != "INF" && upperExt != "RGB") {
                supportedReaders.append(supportedStr);
                individualReaders.append(individualStr);
            }
        }
        if (writerState == stable || writerState == unstable) {
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
    std::string custom = getSettingsCustomFilter();
    if (custom.contains("supported", Qt::CaseInsensitive))
        custom = ""; //This will hide it
    else if (!custom.contains("*", Qt::CaseInsensitive))
        custom = ""; //This will hide it
    else
        custom = "Custom Filter(" + custom + ");;";

    return translate(qPrintable(custom + supported + all));
    */
}

void closeToolBar(QAction* action)
{
    if (action->objectName() == "toolbarclose") {
        QToolBar* tb = qobject_cast<QToolBar*>(sender());
        if (tb) {
            debug_message("%s closed.", qPrintable(tb->objectName()));
            tb->hide();
        }
    }
}

void floatingChangedToolBar(bool isFloating)
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
        else
        {
            std::vector<QAction*> actList = tb->actions();
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

#endif
