/*
 *  Embroidermodder 2.
 *
 *  ------------------------------------------------------------
 *
 *  Copyright 2013-2023 The Embroidermodder Team
 *  Embroidermodder 2 is Open Source Software.
 *  See LICENSE for licensing terms.
 *
 *  ------------------------------------------------------------
 *
 *  Use Python's PEP7 style guide.
 *      https://peps.python.org/pep-0007/
 */

#include "embroidermodder.h"

int pop_command(const char *line);

extern bool test_program;

MainWindow* _mainWin = 0;
MdiArea* mdiArea = 0;
CmdPrompt* prompt = 0;
PropertyEditor* dockPropEdit = 0;
UndoEditor* dockUndoEdit = 0;
StatusBar* statusbar = 0;

QWizard* wizardTipOfTheDay;
QLabel* labelTipOfTheDay;
QCheckBox* checkBoxTipOfTheDay;

QToolBar* toolbarHash[MAX_TOOLBARS];
QMenu* menuHash[MAX_MENUS];
QAction* actionHash[MAX_ACTIONS];
QIcon icon_list[MAX_ICONS];

std::vector<std::string> coverage_test_script = {
    "new",
    "icon 16",
    "icon 32",
    "icon 64",
    "icon 128",
    "icon 24",
    "zoom in",
    "zoom extents",
    "pan up",
    "pan down",
    "pan right",
    "pan left",
    "quit"
};

#if defined(WIN32)
void
emb_sleep(int seconds)
{
    sleep(1);
}
#else
void
emb_sleep(int seconds)
{
    usleep(1000000);
}
#endif

/* Create menu. */
void
create_menu(int32_t menu, const int32_t *def, bool topLevel)
{
    if (topLevel) {
        _mainWin->menuBar()->addMenu(menuHash[menu]);
    }
    for (int i=0; def[i] != MENU_END; i++) {
        switch (def[i]) {
        case MENU_SEPERATOR: {
            menuHash[menu]->addSeparator();
            break;
        }
        case MENU_SUBMENU: {
            menuHash[menu]->addMenu(menuHash[def[i+1]]);
            i++;
            break;
        }
        case MENU_ICON: {
            menuHash[menu]->setIcon(_mainWin->create_icon(def[i+1]));
            i++;
            break;
        }
        default: {
            menuHash[menu]->addAction(actionHash[def[i]]);
            break;
        }
        }
    }

    /* Do not allow the Menus to be torn off. It's a pain in the ass to maintain. */
    menuHash[menu]->setTearOffEnabled(false);
}

/* Create all menus. */
void
MainWindow::createAllMenus()
{
    debug_message("MainWindow createAllMenus()");

    /* Populate menus. */
    create_menu(MENU_FILE, file_menu, true);
    create_menu(MENU_EDIT, edit_menu, true);
    create_menu(MENU_VIEW, view_menu, true);
    create_menu(MENU_SETTINGS, settings_menu, true);
    create_menu(MENU_WINDOW, window_menu, true);
    create_menu(MENU_HELP, help_menu, true);
    create_menu(MENU_DRAW, draw_menu, true);

    /* Populate submenus. */
    create_menu(MENU_ZOOM, zoom_menu, false);
    create_menu(MENU_PAN, pan_menu, false);

    /* Connect dynamic menus. */
    connect(menuHash[MENU_RECENT], SIGNAL(aboutToShow()), this, SLOT(recentMenuAboutToShow()));
    connect(menuHash[MENU_WINDOW], SIGNAL(aboutToShow()), this, SLOT(windowMenuAboutToShow()));
}

/* Create toolbar. */
void
MainWindow::create_toolbar(ToolbarData t)
{
    QString label_ = QString::fromStdString(t.key);
    toolbarHash[t.id]->setObjectName(label_);
    for (int i=0; t.entries[i] != TOOLBAR_END; i++) {
        if (t.entries[i] == TOOLBAR_SEPERATOR) {
            toolbarHash[t.id]->addSeparator();
        }
        else {
            toolbarHash[t.id]->addAction(actionHash[t.entries[i]]);
        }
    }

    connect(toolbarHash[t.id],
        SIGNAL(topLevelChanged(bool)),
        this,
        SLOT(floatingChangedToolBar(bool)));

    if (t.horizontal) {
        toolbarHash[t.id]->setOrientation(Qt::Horizontal);
    }
    else {
        toolbarHash[t.id]->setOrientation(Qt::Vertical);
    }
}

/* Create icon using stub. */
QIcon
MainWindow::create_icon(QString stub)
{
    QString appDir = qApp->applicationDirPath();
    QString icontheme(settings[ST_ICON_THEME].s);
    return QIcon(appDir + "/icons/" + icontheme + "/" + stub + ".png");
}

/* Create icon using action table. */
QIcon
MainWindow::create_icon(int32_t stub)
{
    int i;
    QString appDir = qApp->applicationDirPath();
    QString icontheme(settings[ST_ICON_THEME].s);
    for (i=0; i<ACTION_END; i++) {
        if (action_table[i].id == stub) {
            break;
        }
    }
    QString stub_s(action_table[i].icon);
    return QIcon(appDir + "/icons/" + icontheme + "/" + stub_s + ".png");
}

void
MainWindow::populate_toolbars(Qt::ToolBarArea area, int32_t toolbar_layout[])
{
    for (int i=0; toolbar_layout[i] != TOOLBAR_END; i++) {
        if (toolbar_layout[i] == TOOLBAR_SEPERATOR) {
            addToolBarBreak(area);
        }
        else {
            addToolBar(area, toolbarHash[toolbar_layout[i]]);
        }
    }
}

/* Create all toolbars. */
void
MainWindow::createAllToolbars()
{
    debug_message("MainWindow createAllToolbars()");

    for (int i=0; i<TOTAL_TOOLBARS; i++) {
        create_toolbar(toolbar_data[i]);
    }

    layerSelector->setFocusProxy(prompt);
    //TODO: Create layer pixmaps by concatenating several icons
    layerSelector->addItem(create_icon("linetypebylayer"), "0");
    layerSelector->addItem(create_icon("linetypebylayer"), "1");
    layerSelector->addItem(create_icon("linetypebylayer"), "2");
    layerSelector->addItem(create_icon("linetypebylayer"), "3");
    layerSelector->addItem(create_icon("linetypebylayer"), "4");
    layerSelector->addItem(create_icon("linetypebylayer"), "5");
    layerSelector->addItem(create_icon("linetypebylayer"), "6");
    layerSelector->addItem(create_icon("linetypebylayer"), "7");
    layerSelector->addItem(create_icon("linetypebylayer"), "8");
    layerSelector->addItem(create_icon("linetypebylayer"), "9");
    toolbarHash[TOOLBAR_LAYER]->addWidget(layerSelector);
    connect(layerSelector, SIGNAL(currentIndexChanged(int)), this, SLOT(layerSelectorIndexChanged(int)));

    toolbarHash[TOOLBAR_LAYER]->addAction(actionHash[ACTION_LAYER_PREVIOUS]);

    connect(toolbarHash[TOOLBAR_LAYER], SIGNAL(topLevelChanged(bool)), this, SLOT(floatingChangedToolBar(bool)));

    colorSelector->setFocusProxy(prompt);
    //NOTE: Qt4.7 wont load icons without an extension...
    colorSelector->addItem(create_icon("colorbylayer"), "ByLayer");
    colorSelector->addItem(create_icon("colorbyblock"), "ByBlock");
    colorSelector->addItem(create_icon("colorred"), tr("Red"),     qRgb(255,  0,  0));
    colorSelector->addItem(create_icon("coloryellow"), tr("Yellow"),  qRgb(255,255,  0));
    colorSelector->addItem(create_icon("colorgreen"), tr("Green"),   qRgb(  0,255,  0));
    colorSelector->addItem(create_icon("colorcyan"), tr("Cyan"),    qRgb(  0,255,255));
    colorSelector->addItem(create_icon("colorblue"), tr("Blue"),    qRgb(  0,  0,255));
    colorSelector->addItem(create_icon("colormagenta"), tr("Magenta"), qRgb(255,  0,255));
    colorSelector->addItem(create_icon("colorwhite"), tr("White"),   qRgb(255,255,255));
    colorSelector->addItem(create_icon("colorother"), tr("Other..."));
    toolbarHash[TOOLBAR_PROPERTIES]->addWidget(colorSelector);
    connect(colorSelector, SIGNAL(currentIndexChanged(int)), this, SLOT(colorSelectorIndexChanged(int)));

    toolbarHash[TOOLBAR_PROPERTIES]->addSeparator();
    linetypeSelector->setFocusProxy(prompt);
    //NOTE: Qt4.7 wont load icons without an extension...
    linetypeSelector->addItem(create_icon("linetypebylayer"), "ByLayer");
    linetypeSelector->addItem(create_icon("linetypebyblock"), "ByBlock");
    linetypeSelector->addItem(create_icon("linetypecontinuous"), "Continuous");
    linetypeSelector->addItem(create_icon("linetypehidden"), "Hidden");
    linetypeSelector->addItem(create_icon("linetypecenter"), "Center");
    linetypeSelector->addItem(create_icon("linetypeother"), "Other...");
    toolbarHash[TOOLBAR_PROPERTIES]->addWidget(linetypeSelector);
    connect(linetypeSelector, SIGNAL(currentIndexChanged(int)), this, SLOT(linetypeSelectorIndexChanged(int)));

    toolbarHash[TOOLBAR_PROPERTIES]->addSeparator();
    lineweightSelector->setFocusProxy(prompt);
    //NOTE: Qt4.7 wont load icons without an extension...
    lineweightSelector->addItem(create_icon("lineweightbylayer"), "ByLayer", -2.00);
    lineweightSelector->addItem(create_icon("lineweightbyblock"), "ByBlock", -1.00);
    lineweightSelector->addItem(create_icon("lineweightdefault"), "Default",  0.00);
    //TODO: Thread weight is weird. See http://en.wikipedia.org/wiki/Thread_(yarn)#Weight
    lineweightSelector->addItem(create_icon("lineweight01"), "0.00 mm", 0.00);
    lineweightSelector->addItem(create_icon("lineweight02"), "0.05 mm", 0.05);
    lineweightSelector->addItem(create_icon("lineweight03"), "0.15 mm", 0.15);
    lineweightSelector->addItem(create_icon("lineweight04"), "0.20 mm", 0.20);
    lineweightSelector->addItem(create_icon("lineweight05"), "0.25 mm", 0.25);
    lineweightSelector->addItem(create_icon("lineweight06"), "0.30 mm", 0.30);
    lineweightSelector->addItem(create_icon("lineweight07"), "0.35 mm", 0.35);
    lineweightSelector->addItem(create_icon("lineweight08"), "0.40 mm", 0.40);
    lineweightSelector->addItem(create_icon("lineweight09"), "0.45 mm", 0.45);
    lineweightSelector->addItem(create_icon("lineweight10"), "0.50 mm", 0.50);
    lineweightSelector->addItem(create_icon("lineweight11"), "0.55 mm", 0.55);
    lineweightSelector->addItem(create_icon("lineweight12"), "0.60 mm", 0.60);
    lineweightSelector->addItem(create_icon("lineweight13"), "0.65 mm", 0.65);
    lineweightSelector->addItem(create_icon("lineweight14"), "0.70 mm", 0.70);
    lineweightSelector->addItem(create_icon("lineweight15"), "0.75 mm", 0.75);
    lineweightSelector->addItem(create_icon("lineweight16"), "0.80 mm", 0.80);
    lineweightSelector->addItem(create_icon("lineweight17"), "0.85 mm", 0.85);
    lineweightSelector->addItem(create_icon("lineweight18"), "0.90 mm", 0.90);
    lineweightSelector->addItem(create_icon("lineweight19"), "0.95 mm", 0.95);
    lineweightSelector->addItem(create_icon("lineweight20"), "1.00 mm", 1.00);
    lineweightSelector->addItem(create_icon("lineweight21"), "1.05 mm", 1.05);
    lineweightSelector->addItem(create_icon("lineweight22"), "1.10 mm", 1.10);
    lineweightSelector->addItem(create_icon("lineweight23"), "1.15 mm", 1.15);
    lineweightSelector->addItem(create_icon("lineweight24"), "1.20 mm", 1.20);
    lineweightSelector->setMinimumContentsLength(8); // Prevent dropdown text readability being squish...d.
    toolbarHash[TOOLBAR_PROPERTIES]->addWidget(lineweightSelector);
    connect(lineweightSelector, SIGNAL(currentIndexChanged(int)), this, SLOT(lineweightSelectorIndexChanged(int)));

    connect(toolbarHash[TOOLBAR_PROPERTIES], SIGNAL(topLevelChanged(bool)), this, SLOT(floatingChangedToolBar(bool)));

    toolbarHash[TOOLBAR_TEXT]->addWidget(textFontSelector);
    QString font_(settings[ST_TEXT_FONT].s);
    textFontSelector->setCurrentFont(QFont(font_));
    connect(textFontSelector, SIGNAL(currentFontChanged(QFont)), this, SLOT(textFontSelectorCurrentFontChanged(QFont)));

    toolbarHash[TOOLBAR_TEXT]->addAction(actionHash[ACTION_TEXT_BOLD]);
    actionHash[ACTION_TEXT_BOLD]->setChecked(settings[ST_TEXT_BOLD].i);

    toolbarHash[TOOLBAR_TEXT]->addAction(actionHash[ACTION_TEXT_ITALIC]);
    actionHash[ACTION_TEXT_ITALIC]->setChecked(settings[ST_TEXT_ITALIC].i);

    toolbarHash[TOOLBAR_TEXT]->addAction(actionHash[ACTION_TEXT_UNDERLINE]);
    actionHash[ACTION_TEXT_UNDERLINE]->setChecked(settings[ST_TEXT_UNDERLINE].i);

    toolbarHash[TOOLBAR_TEXT]->addAction(actionHash[ACTION_TEXT_STRIKEOUT]);
    actionHash[ACTION_TEXT_STRIKEOUT]->setChecked(settings[ST_TEXT_STRIKEOUT].i);

    toolbarHash[TOOLBAR_TEXT]->addAction(actionHash[ACTION_TEXT_OVERLINE]);
    actionHash[ACTION_TEXT_OVERLINE]->setChecked(settings[ST_TEXT_OVERLINE].i);

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
    setTextSize(settings[ST_TEXT_SIZE].r);
    toolbarHash[TOOLBAR_TEXT]->addWidget(textSizeSelector);
    connect(textSizeSelector, SIGNAL(currentIndexChanged(int)), this, SLOT(textSizeSelectorIndexChanged(int)));

    connect(toolbarHash[TOOLBAR_TEXT], SIGNAL(topLevelChanged(bool)), this, SLOT(floatingChangedToolBar(bool)));

    toolbarHash[TOOLBAR_PROMPT]->addWidget(prompt);
    toolbarHash[TOOLBAR_PROMPT]->setAllowedAreas(Qt::TopToolBarArea | Qt::BottomToolBarArea);
    connect(toolbarHash[TOOLBAR_PROMPT], SIGNAL(topLevelChanged(bool)),
        prompt, SLOT(floatingChanged(bool)));

    // Horizontal
    toolbarHash[TOOLBAR_VIEW]->setOrientation(Qt::Horizontal);
    toolbarHash[TOOLBAR_ZOOM]->setOrientation(Qt::Horizontal);
    toolbarHash[TOOLBAR_LAYER]->setOrientation(Qt::Horizontal);
    toolbarHash[TOOLBAR_PROPERTIES]->setOrientation(Qt::Horizontal);
    toolbarHash[TOOLBAR_TEXT]->setOrientation(Qt::Horizontal);
    toolbarHash[TOOLBAR_PROMPT]->setOrientation(Qt::Horizontal);
    toolbarHash[TOOLBAR_DRAW]->setOrientation(Qt::Vertical);

    populate_toolbars(Qt::TopToolBarArea, top_toolbar_layout);
    populate_toolbars(Qt::BottomToolBarArea, bottom_toolbar_layout);
    populate_toolbars(Qt::LeftToolBarArea, side_toolbar_layout);

    //zoomToolBar->setToolButtonStyle(Qt::ToolButtonTextOnly);
}


/* settingsPrompt */
void
MainWindow::settingsPrompt()
{
    actuator("settings-dialog Prompt");
}

/* stub_testing */
void
MainWindow::stub_testing()
{
    QMessageBox::warning(_mainWin,
        translate_str("Testing Feature"),
        translate_str("<b>This feature is in testing.</b>"));
}

/* quit */
void
MainWindow::quit()
{
    debug_message("quit()");
    if (settings[ST_SAVE_HISTORY].i) {
        prompt->saveHistory("prompt.log", settings[ST_HTML_OUTPUT].i);
        //TODO: get filename from settings
    }
    qApp->closeAllWindows();
    this->deleteLater();
    // Force the MainWindow destructor to run before exiting. Makes Valgrind "still reachable" happy :)
}

/* checkForUpdates */
void
MainWindow::checkForUpdates()
{
    debug_message("checkForUpdates()");
    //TODO: Check website for new versions, commands, etc...
}

/* platformString.
 * TODO: Append QSysInfo to string where applicable.
 */
std::string
platformString(void)
{
    std::string os;
#if defined(WIN32)
    return "Windows";
#else
    struct utsname platform;
    uname(&platform);
    std::string ret(platform.sysname);
    return ret;
#endif
}

/* The about dialog. */
void
MainWindow::about(void)
{
    //TODO: QTabWidget for about dialog
    QApplication::setOverrideCursor(Qt::ArrowCursor);
    QString appDir = qApp->applicationDirPath();
    QString appName = QApplication::applicationName();
    QString title = "About " + appName;

    QDialog dialog(_mainWin);
    ImageWidget img(appDir + "/images/logo-small");
    QLabel text(appName + translate_str("\n\n") +
                          translate_str("https://www.libembroidery.org") +
                          translate_str("\n\n") +
                          translate_str("Available Platforms: GNU/Linux, Windows, Mac OSX, Raspberry Pi") +
                          translate_str("\n\n") +
                          translate_str("Embroidery formats by Josh Varga.") +
                          translate_str("\n") +
                          translate_str("User Interface by Jonathan Greig.") +
                          translate_str("\n\n") +
                          translate_str("Free under the zlib/libpng license.")
                          #if defined(BUILD_GIT_HASH)
                          + translate_str("\n\n") +
                          translate_str("Build Hash: ") + qPrintable(BUILD_GIT_HASH)
                          #endif
                          );
    text.setWordWrap(true);

    QDialogButtonBox buttonbox(Qt::Horizontal, &dialog);
    QPushButton button(&dialog);
    button.setText("Oh, Yeah!");
    buttonbox.addButton(&button, QDialogButtonBox::AcceptRole);
    buttonbox.setCenterButtons(true);
    connect(&buttonbox, SIGNAL(accepted()), &dialog, SLOT(accept()));

    QVBoxLayout layout;
    layout.setAlignment(Qt::AlignCenter);
    layout.addWidget(&img);
    layout.addWidget(&text);
    layout.addWidget(&buttonbox);

    dialog.setWindowTitle(title);
    dialog.setMinimumWidth(img.minimumWidth()+30);
    dialog.setMinimumHeight(img.minimumHeight()+50);
    dialog.setLayout(&layout);
    dialog.exec();
    QApplication::restoreOverrideCursor();
}

/* The Tip of the Day dialog. */
void
MainWindow::tipOfTheDay(void)
{
    QString appDir = qApp->applicationDirPath();

    wizardTipOfTheDay = new QWizard(_mainWin);
    wizardTipOfTheDay->setAttribute(Qt::WA_DeleteOnClose);
    wizardTipOfTheDay->setWizardStyle(QWizard::ModernStyle);
    wizardTipOfTheDay->setMinimumSize(550, 400);

    QWizardPage* page = new QWizardPage(wizardTipOfTheDay);

    ImageWidget* imgBanner = new ImageWidget(appDir + "/images/did-you-know.png", wizardTipOfTheDay);

    int n = string_array_length(tips);
    if (settings[ST_CURRENT_TIP].i >= n) {
        settings[ST_CURRENT_TIP].i = 0;
    }
    int general_current_tip = settings[ST_CURRENT_TIP].i;
    labelTipOfTheDay = new QLabel(tips[general_current_tip], wizardTipOfTheDay);
    labelTipOfTheDay->setWordWrap(true);

    QCheckBox* checkBoxTipOfTheDay = new QCheckBox(translate_str("&Show tips on startup"), wizardTipOfTheDay);
    checkBoxTipOfTheDay->setChecked(settings[ST_TIP_OF_THE_DAY].i);
    connect(checkBoxTipOfTheDay, SIGNAL(stateChanged(int)), _mainWin, SLOT(checkBoxTipOfTheDayStateChanged(int)));

    QVBoxLayout* layout = new QVBoxLayout(wizardTipOfTheDay);
    layout->addWidget(imgBanner);
    layout->addStrut(1);
    layout->addWidget(labelTipOfTheDay);
    layout->addStretch(1);
    layout->addWidget(checkBoxTipOfTheDay);
    page->setLayout(layout);
    wizardTipOfTheDay->addPage(page);

    wizardTipOfTheDay->setWindowTitle("Tip of the Day");

    //TODO: Add icons to buttons by using wizardTipOfTheDay->setButton(QWizard::CustomButton1, buttonPrevious)
    //TODO: Add icons to buttons by using wizardTipOfTheDay->setButton(QWizard::CustomButton1, buttonNext)
    //TODO: Add icons to buttons by using wizardTipOfTheDay->setButton(QWizard::CustomButton1, buttonClose)
    wizardTipOfTheDay->setButtonText(QWizard::CustomButton1, translate_str("&Previous"));
    wizardTipOfTheDay->setButtonText(QWizard::CustomButton2, translate_str("&Next"));
    wizardTipOfTheDay->setButtonText(QWizard::CustomButton3, translate_str("&Close"));
    wizardTipOfTheDay->setOption(QWizard::HaveCustomButton1, true);
    wizardTipOfTheDay->setOption(QWizard::HaveCustomButton2, true);
    wizardTipOfTheDay->setOption(QWizard::HaveCustomButton3, true);
    connect(wizardTipOfTheDay, SIGNAL(customButtonClicked(int)), _mainWin, SLOT(buttonTipOfTheDayClicked(int)));

    QList<QWizard::WizardButton> listTipOfTheDayButtons;
    listTipOfTheDayButtons << QWizard::Stretch << QWizard::CustomButton1 << QWizard::CustomButton2 << QWizard::CustomButton3;
    wizardTipOfTheDay->setButtonLayout(listTipOfTheDayButtons);

    wizardTipOfTheDay->exec();
}

/* . */
void
MainWindow::buttonTipOfTheDayClicked(int button)
{
    debug_message("buttonTipOfTheDayClicked(%d)" + std::to_string(button));
    int n_tips = string_array_length(tips);
    if (button == QWizard::CustomButton1) {
        if (settings[ST_CURRENT_TIP].i > 0) {
            settings[ST_CURRENT_TIP].i--;
        }
        else {
            settings[ST_CURRENT_TIP].i = n_tips-1;
        }
        labelTipOfTheDay->setText(tips[settings[ST_CURRENT_TIP].i]);
    }
    else if (button == QWizard::CustomButton2) {
        settings[ST_CURRENT_TIP].i++;
        if (settings[ST_CURRENT_TIP].i >= n_tips) {
            settings[ST_CURRENT_TIP].i = 0;
        }
        labelTipOfTheDay->setText(tips[settings[ST_CURRENT_TIP].i]);
    }
    else if (button == QWizard::CustomButton3) {
        wizardTipOfTheDay->close();
    }
}

/* . */
bool
MainWindow::isShiftPressed()
{
    return shiftKeyPressedState;
}

/* . */
void
MainWindow::setShiftPressed()
{
    shiftKeyPressedState = true;
}

/* . */
void
MainWindow::setShiftReleased()
{
    shiftKeyPressedState = false;
}

// Icons
void
MainWindow::iconResize(int iconSize)
{
    this->setIconSize(QSize(iconSize, iconSize));
    layerSelector->setIconSize(QSize(iconSize*4, iconSize));
    colorSelector->setIconSize(QSize(iconSize, iconSize));
    linetypeSelector->setIconSize(QSize(iconSize*4, iconSize));
    lineweightSelector->setIconSize(QSize(iconSize*4, iconSize));
    //set the minimum combobox width so the text is always readable
    layerSelector->setMinimumWidth(iconSize*4);
    colorSelector->setMinimumWidth(iconSize*2);
    linetypeSelector->setMinimumWidth(iconSize*4);
    lineweightSelector->setMinimumWidth(iconSize*4);

    //TODO: low-priority: open app with iconSize set to 128. resize the icons to a smaller size.

    settings[ST_ICON_SIZE].i = iconSize;
}

/* Active mdi window. */
MdiWindow *
MainWindow::activeMdiWindow()
{
    debug_message("activeMdiWindow()");
    MdiWindow* mdiWin = qobject_cast<MdiWindow*>(mdiArea->activeSubWindow());
    return mdiWin;
}

/* Active view. */
View *
activeView(void)
{
    debug_message("activeView()");
    MdiWindow* mdiWin = _mainWin->activeMdiWindow();
    if (mdiWin) {
        return mdiWin->gview;
    }
    return 0;
}

/* Active scene. */
QGraphicsScene *
activeScene(void)
{
    debug_message("activeScene()");
    MdiWindow* mdiWin = qobject_cast<MdiWindow*>(mdiArea->activeSubWindow());
    if (mdiWin) {
        return mdiWin->gscene;
    }
    return 0;
}

/* Active undo stack. */
QUndoStack *
MainWindow::activeUndoStack(void)
{
    debug_message("activeUndoStack()");
    View* v = activeView();
    if (v) {
        QUndoStack* u = v->undoStack;
        return u;
    }
    return 0;
}

/* Set undo clean icon. */
void
MainWindow::setUndoCleanIcon(bool opened)
{
    dockUndoEdit->updateCleanIcon(opened);
}

/* Update all view scrollbars. */
void
MainWindow::updateAllViewScrollBars(bool val)
{
    QList<QMdiSubWindow*> windowList = mdiArea->subWindowList();
    for (int i = 0; i < windowList.count(); ++i) {
        MdiWindow* mdiWin = qobject_cast<MdiWindow*>(windowList.at(i));
        if (mdiWin) {
            mdiWin->showViewScrollBars(val);
        }
    }
}

/* Update all view crosshair colors. */
void
MainWindow::updateAllViewCrossHairColors(QRgb color)
{
    QList<QMdiSubWindow*> windowList = mdiArea->subWindowList();
    for (int i = 0; i < windowList.count(); ++i) {
        MdiWindow* mdiWin = qobject_cast<MdiWindow*>(windowList.at(i));
        if (mdiWin) { mdiWin->setViewCrossHairColor(color); }
    }
}

/* Update all view background colors. */
void
MainWindow::updateAllViewBackgroundColors(QRgb color)
{
    QList<QMdiSubWindow*> windowList = mdiArea->subWindowList();
    for (int i = 0; i < windowList.count(); ++i) {
        MdiWindow* mdiWin = qobject_cast<MdiWindow*>(windowList.at(i));
        if (mdiWin) {
            mdiWin->setViewBackgroundColor(color);
        }
    }
}

/* UpdateAllViewSelectBoxColors.
 * colorL, fillL:
 * colorR, fillR:
 * alpha
 */
void
MainWindow::updateAllViewSelectBoxColors(
    QRgb colorL, QRgb fillL, QRgb colorR, QRgb fillR, int alpha)
{
    QList<QMdiSubWindow*> windowList = mdiArea->subWindowList();
    for (int i = 0; i < windowList.count(); ++i) {
        MdiWindow* mdiWin = qobject_cast<MdiWindow*>(windowList.at(i));
        if (mdiWin) {
            mdiWin->setViewSelectBoxColors(colorL, fillL, colorR, fillR, alpha);
        }
    }
}

/* Update all view grid colors to QRgb color. */
void
MainWindow::updateAllViewGridColors(QRgb color)
{
    QList<QMdiSubWindow*> windowList = mdiArea->subWindowList();
    for (int i = 0; i < windowList.count(); ++i) {
        MdiWindow* mdiWin = qobject_cast<MdiWindow*>(windowList.at(i));
        if (mdiWin) {
            mdiWin->setViewGridColor(color);
        }
    }
}

/* MainWindow::updateAllViewRulerColors color */
void
MainWindow::updateAllViewRulerColors(QRgb color)
{
    QList<QMdiSubWindow*> windowList = mdiArea->subWindowList();
    for (int i = 0; i < windowList.count(); ++i) {
        MdiWindow* mdiWin = qobject_cast<MdiWindow*>(windowList.at(i));
        if (mdiWin) {
            mdiWin->setViewRulerColor(color);
        }
    }
}

/* Update pickAdd mode. */
void
MainWindow::updatePickAddMode(bool val)
{
    settings[ST_SELECTION_PICK_ADD].i = val;
    dockPropEdit->updatePickAddModeButton(val);
}

/* PickAdd mode toggled. */
void
MainWindow::pickAddModeToggled()
{
    updatePickAddMode(!settings[ST_SELECTION_PICK_ADD].i);
}

/* MainWindow::makeLayerActive */
std::string
make_layer_active_action(std::string args)
{
    debug_message("TODO: Implement makeLayerActive.");
    return "";
}

/* layer_manager_action  args */
std::string
layer_manager_action(std::string args)
{
    debug_message("TODO: Implement layerManager.");
    LayerManager layman(_mainWin);
    layman.exec();
    return "";
}

/* Using the undo stack for data, return to the last layer displayed. */
std::string
layer_previous_action(std::string args)
{
    debug_message("TODO: Implement layerPrevious.");
    return "";
}

/* Zoom the current view using behaviour described by "mode". */
std::string
zoom_action(std::string mode)
{
    View* gview = activeView();
    if (!gview) {
        return "ERROR: no active view found.";
    }
    QUndoStack* stack = gview->undoStack;
    if (!stack) {
        return "ERROR: no undo stack found.";
    }
    if (mode == "realtime") {
        debug_message("zoomRealtime()");
        debug_message("TODO: Implement zoomRealtime.");
        return "";
    }
    if (mode == "previous") {
        debug_message("zoomPrevious()");
        debug_message("TODO: Implement zoomPrevious.");
        return "";
    }
    if (mode == "window") {
        debug_message("zoomWindow()");
        gview->zoomWindow();
        return "";
    }
    if (mode == "dynamic") {
        debug_message("zoomDynamic()");
        debug_message("TODO: Implement zoomDynamic.");
        return "";
    }
    if (mode == "scale") {
        debug_message("zoomScale()");
        debug_message("TODO: Implement zoomScale.");
        return "";
    }
    if (mode == "center") {
        debug_message("zoomCenter()");
        debug_message("TODO: Implement zoomCenter.");
        return "";
    }
    if (mode == "in") {
        debug_message("zoomIn()");
        gview->zoomIn();
        return "";
    }
    if (mode == "out") {
        debug_message("zoomOut()");
        gview->zoomOut();
        return "";
    }
    if (mode == "selected") {
        debug_message("zoomSelected()");
        UndoableCommand* cmd = new UndoableCommand("ZoomSelected", gview, 0);
        stack->push(cmd);
        return "";
    }
    if (mode == "all") {
        debug_message("zoomAll()");
        debug_message("TODO: Implement zoomAll.");
        return "";
    }
    if (mode == "extents") {
        debug_message("zoomExtents()");
        UndoableCommand* cmd = new UndoableCommand("ZoomExtents", gview, 0);
        stack->push(cmd);
        return "";
    }
    return "ERROR: zoom subcommand not recognised.";
}

/* LayerSelectorIndexChanged index */
void
MainWindow::layerSelectorIndexChanged(int index)
{
    debug_message("layerSelectorIndexChanged(%d)" + std::to_string(index));
}

/* MainWindow::colorSelectorIndexChanged index */
void
MainWindow::colorSelectorIndexChanged(int index)
{
    debug_message("colorSelectorIndexChanged(%d)" + std::to_string(index));

    QComboBox* comboBox = qobject_cast<QComboBox*>(sender());
    QRgb newColor;
    if (comboBox) {
        bool ok = 0;
        //TODO: Handle ByLayer and ByBlock and Other...
        newColor = comboBox->itemData(index).toUInt(&ok);
        if (!ok)
            QMessageBox::warning(this, translate_str("Color Selector Conversion Error"), translate_str("<b>An error has occured while changing colors.</b>"));
    }
    else
        QMessageBox::warning(this, translate_str("Color Selector Pointer Error"), translate_str("<b>An error has occured while changing colors.</b>"));

    MdiWindow* mdiWin = qobject_cast<MdiWindow*>(mdiArea->activeSubWindow());
    if (mdiWin) { mdiWin->currentColorChanged(newColor); }
}

/* . */
void
MainWindow::linetypeSelectorIndexChanged(int index)
{
    debug_message("linetypeSelectorIndexChanged(%d)" + std::to_string(index));
}

/* lineweightSelectorIndexChanged index */
void
MainWindow::lineweightSelectorIndexChanged(int index)
{
    debug_message("lineweightSelectorIndexChanged(%d)" + std::to_string(index));
}

/* textFontSelectorCurrentFontChanged font */
void
MainWindow::textFontSelectorCurrentFontChanged(const QFont& font)
{
    debug_message("textFontSelectorCurrentFontChanged()");
    strcpy(settings[ST_TEXT_FONT].s, font.family().toStdString().c_str());
}

/* textSizeSelectorIndexChanged index */
void
MainWindow::textSizeSelectorIndexChanged(int index)
{
    debug_message("textSizeSelectorIndexChanged(%d)" + std::to_string(index));
    settings[ST_TEXT_SIZE].r = (EmbReal)fabs(textSizeSelector->itemData(index).toReal()); //TODO: check that the toReal() conversion is ok
}

/* Set text font to the one with the key given by "str". */
void
MainWindow::setTextFont(QString str)
{
    textFontSelector->setCurrentFont(QFont(str));
    strcpy(settings[ST_TEXT_FONT].s, str.toStdString().c_str());
}

/* Set text size to "num". */
void
MainWindow::setTextSize(EmbReal num)
{
    settings[ST_TEXT_SIZE].r = (EmbReal)fabs(num);
    int index = textSizeSelector->findText("Custom", Qt::MatchContains);
    if (index != -1) {
        textSizeSelector->removeItem(index);
    }
    textSizeSelector->addItem("Custom " + QString().setNum(num, 'f', 2) + " pt", num);
    index = textSizeSelector->findText("Custom", Qt::MatchContains);
    if (index != -1) {
        textSizeSelector->setCurrentIndex(index);
    }
}

/* Get current layer. */
QString
MainWindow::getCurrentLayer()
{
    MdiWindow* mdiWin = qobject_cast<MdiWindow*>(mdiArea->activeSubWindow());
    if (mdiWin) {
        return mdiWin->curLayer;
    }
    return "0";
}

/* Get current color. */
QRgb
MainWindow::getCurrentColor()
{
    MdiWindow* mdiWin = qobject_cast<MdiWindow*>(mdiArea->activeSubWindow());
    if (mdiWin) {
        return mdiWin->curColor;
    }
    return 0; //TODO: return color ByLayer
}

/* Get current line type. */
QString
MainWindow::getCurrentLineType()
{
    MdiWindow* mdiWin = qobject_cast<MdiWindow*>(mdiArea->activeSubWindow());
    if (mdiWin) {
        return mdiWin->curLineType;
    }
    return "ByLayer";
}

/* Get current line weight */
QString
MainWindow::getCurrentLineWeight()
{
    MdiWindow* mdiWin = qobject_cast<MdiWindow*>(mdiArea->activeSubWindow());
    if (mdiWin) {
        return mdiWin->curLineWeight;
    }
    return "ByLayer";
}

/* Delete pressed. */
void
MainWindow::deletePressed()
{
    debug_message("deletePressed()");
    QApplication::setOverrideCursor(Qt::WaitCursor);
    MdiWindow* mdiWin = qobject_cast<MdiWindow*>(mdiArea->activeSubWindow());
    if (mdiWin) {
        mdiWin->deletePressed();
    }
    QApplication::restoreOverrideCursor();
}

/* Escape pressed. */
void
MainWindow::escapePressed()
{
    debug_message("escapePressed()");
    QApplication::setOverrideCursor(Qt::WaitCursor);
    MdiWindow* mdiWin = qobject_cast<MdiWindow*>(mdiArea->activeSubWindow());
    if (mdiWin) {
        mdiWin->escapePressed();
    }
    QApplication::restoreOverrideCursor();

    actuator("end");
}

/* MainWindow::toggleGrid */
void
MainWindow::toggleGrid()
{
    debug_message("toggleGrid()");
    statusbar->buttons["GRID"]->toggle();
}

/* MainWindow::toggleRuler */
void
MainWindow::toggleRuler()
{
    debug_message("toggleRuler()");
    statusbar->buttons["RULER"]->toggle();
}

/* MainWindow::toggleLwt */
void
MainWindow::toggleLwt()
{
    debug_message("toggleLwt()");
    statusbar->buttons["LWT"]->toggle();
}

/* MainWindow::promptHistoryAppended(txt) */
void
MainWindow::promptHistoryAppended(QString txt)
{
    MdiWindow* mdiWin = qobject_cast<MdiWindow*>(mdiArea->activeSubWindow());
    if (mdiWin) {
        mdiWin->promptHistoryAppended(txt);
    }
}

/* MainWindow::logPromptInput  txt */
void
MainWindow::logPromptInput(QString txt)
{
    MdiWindow* mdiWin = qobject_cast<MdiWindow*>(mdiArea->activeSubWindow());
    if (mdiWin) {
        mdiWin->logPromptInput(txt);
    }
}

/* MainWindow::promptInputPrevious */
void
MainWindow::promptInputPrevious()
{
    MdiWindow* mdiWin = qobject_cast<MdiWindow*>(mdiArea->activeSubWindow());
    if (mdiWin) {
        mdiWin->promptInputPrevious();
    }
}

/* . */
void
MainWindow::promptInputNext()
{
    MdiWindow* mdiWin = qobject_cast<MdiWindow*>(mdiArea->activeSubWindow());
    if (mdiWin) {
        mdiWin->promptInputNext();
    }
}

/* SetCrossHairColor uint8_t r, uint8_t g, uint8_t b
 */
std::string
set_crosshair_color_action(uint8_t r, uint8_t g, uint8_t b)
{
    settings[ST_CROSSHAIR_COLOR].i = qRgb(r,g,b);
    _mainWin->updateAllViewCrossHairColors(qRgb(r,g,b));
    return "";
}

/* set_grid_color
 * uint8_t r, uint8_t g, uint8_t b
 */
std::string
set_grid_color_action(uint8_t r, uint8_t g, uint8_t b)
{
    settings[ST_GRID_COLOR].i = qRgb(r,g,b);
    _mainWin->updateAllViewGridColors(qRgb(r,g,b));
    return "";
}

/* PreviewOn
 * clone, mode, x, y, data
 */
std::string
preview_on_action(std::string args)
{
    View* gview = activeView();
    if (gview) {
        //gview->previewOn(clone, mode, x, -y, data);

        /*
        QString cloneStr = QString::toStdString(a[0].s).toUpper();
        QString modeStr  = QString::toStdString(a[1].s).toUpper();
        EmbReal x = a[2].r;
        EmbReal y = a[3].r;
        EmbReal data = a[4].r;

        int clone = PREVIEW_CLONE_NULL;
        int mode = PREVIEW_MODE_NULL;
        if     (cloneStr == "SELECTED") { clone = PREVIEW_CLONE_SELECTED; }
        else if (cloneStr == "RUBBER") { clone = PREVIEW_CLONE_RUBBER;   }
        else { return context->throwError(QScriptContext::UnknownError, "preview_on_action(): first argument must be \"SELECTED\" or \"RUBBER\"."); }

        if     (modeStr == "MOVE") { mode = PREVIEW_MODE_MOVE;   }
        else if (modeStr == "ROTATE") { mode = PREVIEW_MODE_ROTATE; }
        else if (modeStr == "SCALE") { mode = PREVIEW_MODE_SCALE;  }
        else { return context->throwError(QScriptContext::UnknownError, "preview_on_action(): second argument must be \"MOVE\", \"ROTATE\" or \"SCALE\"."); }

        _mainWin->nativepreview_on_action(clone, mode, x, y, data);
        */
    }
    return "";
}

/*
 * NOTE: This native is different than the rest in that the Y+ is down
 * (scripters need not worry about this)
 * EmbReal startX, EmbReal startY, const QPainterPath& p, std::string rubberMode
 */
std::string
add_polyline_action(std::string args)
{
    /*
    QVariantList varList = a[0].toVariant().toList();
    int varSize = varList.size();
    if (varSize < 2) {
        return "TYPE ERROR: addPolyline(): array must contain at least two elements";
    }
    if (varSize % 2) {
        return "TYPE ERROR: addPolyline(): array cannot contain an odd number of elements";
    }

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

                if (lineTo) {
                    path.lineTo(x,y);
                }
                else {
                    path.moveTo(x,y);
                    lineTo = true;
                    startX = x;
                    startY = y;
                }
            }
        }
        else {
            return "TYPE ERROR: addPolyline(): array contains one or more invalid elements";
        }
    }

    path.translate(-startX, -startY);

    add_polyline(startX, startY, path, OBJ_RUBBER_OFF);
    */
    return "";
}

/* . */
std::string
delete_selected_action(std::string args)
{
    View* gview = activeView();
    if (gview) {
        gview->deleteSelected();
    }
    return "";
}

/* . */
std::string
paste_selected_action(std::string args)
{
    /*
    _mainWin->nativePasteSelected(a[0].r, a[1].r);
    */
    return "";
}

/* disable_action
 * variable
 */
std::string
disable_action(std::string variable)
{
    if (variable == "text-angle") {
        settings[ST_TEXT_ANGLE].i = 0;
        return "";
    }
    if (variable == "text-bold") {
        settings[ST_TEXT_BOLD].i = 0;
        return "";
    }
    if (variable == "text-italic") {
        settings[ST_TEXT_ITALIC].i = 0;
        return "";
    }
    if (variable == "text-underline") {
        settings[ST_TEXT_UNDERLINE].i = 0;
        return "";
    }
    if (variable == "text-strikeout") {
        settings[ST_TEXT_STRIKEOUT].i = 0;
        return "";
    }
    if (variable == "text-overline") {
        settings[ST_TEXT_OVERLINE].i = 0;
        return "";
    }
    if (variable == "prompt-rapid-fire") {
        prompt->promptInput->rapidFireEnabled = 0;
        return "";
    }
    if (variable == "move-rapid-fire") {
        View* gview = activeView();
        if (gview) {
            gview->rapidMoveActive = 0;
        }
        return "";
    }
    return "";
}

/* MainWindow::MainWindow
 */
MainWindow::MainWindow() : QMainWindow(0)
{
    QString appDir = qApp->applicationDirPath();
    read_settings();

    QString icon_theme(settings[ST_ICON_THEME].s);
    //Verify that files/directories needed are actually present.
    QFileInfo check = QFileInfo(appDir + "/icons");
    if (!check.exists()) {
        QMessageBox::critical(_mainWin, translate_str("Path Error"),
            translate_str("Cannot locate: ") + check.absoluteFilePath());
    }
    check = QFileInfo(appDir + "/images");
    if (!check.exists()) {
        QMessageBox::critical(_mainWin, translate_str("Path Error"),
            translate_str("Cannot locate: ") + check.absoluteFilePath());
    }
    check = QFileInfo(appDir + "/translations");
    if (!check.exists()) {
        QMessageBox::critical(_mainWin, translate_str("Path Error"),
            translate_str("Cannot locate: ") + check.absoluteFilePath());
    }

    QString lang(settings[ST_LANGUAGE].s);
    debug_message("language: " + lang.toStdString());
    if (lang == "system") {
        lang = QLocale::system().languageToString(QLocale::system().language()).toLower();
    }

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
    _mainWin = this;

    QPoint pos = QPoint(settings[ST_WINDOW_POS_X].i, settings[ST_WINDOW_POS_Y].i);
    QSize size = QSize(settings[ST_WINDOW_SIZE_X].i, settings[ST_WINDOW_SIZE_Y].i);

    move(pos);
    resize(size);

    //Menus
    menuHash[MENU_FILE] = new QMenu(translate_str("&File"), this);
    menuHash[MENU_EDIT] = new QMenu(translate_str("&Edit"), this);
    menuHash[MENU_VIEW] = new QMenu(translate_str("&View"), this);
    menuHash[MENU_SETTINGS] = new QMenu(translate_str("&Settings"), this);
    menuHash[MENU_WINDOW] = new QMenu(translate_str("&Window"), this);
    menuHash[MENU_HELP] = new QMenu(translate_str("&Help"), this);
    menuHash[MENU_DRAW] = new QMenu(translate_str("&Draw"), this);

    //SubMenus
    menuHash[MENU_RECENT] = new QMenu(translate_str("Open &Recent"), this);
    menuHash[MENU_ZOOM] = new QMenu(translate_str("&Zoom"), this);
    menuHash[MENU_PAN] = new QMenu(translate_str("&Pan"), this);

    //Toolbars
    for (int i=0; i<TOTAL_TOOLBARS; i++) {
        toolbarHash[i] = addToolBar(translate_str(toolbar_data[i].key));
    }

    //Selectors
    layerSelector = new QComboBox(this);
    colorSelector = new QComboBox(this);
    linetypeSelector = new QComboBox(this);
    lineweightSelector = new QComboBox(this);
    textFontSelector = new QFontComboBox(this);
    textSizeSelector = new QComboBox(this);

    numOfDocs = 0;
    docIndex = 0;

    shiftKeyPressedState = false;

    setWindowIcon(_mainWin->create_icon("app"));
    setMinimumSize(800, 480); //Require Minimum WVGA

    loadFormats();

    //create the mdiArea
    QFrame* vbox = new QFrame(this);
    QVBoxLayout* layout = new QVBoxLayout(vbox);
    //layout->setMargin(0);
    vbox->setFrameStyle(QFrame::StyledPanel | QFrame::Sunken);
    mdiArea = new MdiArea(vbox);
    mdiArea->useBackgroundLogo(settings[ST_MDI_USE_LOGO].i);
    mdiArea->useBackgroundTexture(settings[ST_MDI_USE_TEXTURE].i);
    mdiArea->useBackgroundColor(settings[ST_MDI_USE_COLOR].i);
    mdiArea->setBackgroundLogo(QString(settings[ST_MDI_LOGO].s));
    mdiArea->setBackgroundTexture(QString(settings[ST_MDI_TEXTURE].s));
    mdiArea->setBackgroundColor(QColor(settings[ST_MDI_COLOR].i));
    mdiArea->setViewMode(QMdiArea::TabbedView);
    mdiArea->setHorizontalScrollBarPolicy(Qt::ScrollBarAsNeeded);
    mdiArea->setVerticalScrollBarPolicy(Qt::ScrollBarAsNeeded);
    mdiArea->setActivationOrder(QMdiArea::ActivationHistoryOrder);
    layout->addWidget(mdiArea);
    setCentralWidget(vbox);

    //create the Command Prompt
    prompt = new CmdPrompt();
    prompt->setFocus(Qt::OtherFocusReason);
    this->setFocusProxy(prompt);
    mdiArea->setFocusProxy(prompt);

    prompt->setPromptTextColor(QColor(settings[ST_PROMPT_TEXT_COLOR].i));
    uint32_t c = (uint32_t)settings[ST_PROMPT_BG_COLOR].i;
    prompt->setPromptBackgroundColor(QColor(c));

    connect(prompt, SIGNAL(startCommand(QString)), this, SLOT(logPromptInput(QString)));
    connect(prompt, SIGNAL(startCommand(QString)), this, SLOT(runCommandMain(QString)));
    connect(prompt, SIGNAL(runCommand(QString,QString)), this, SLOT(runCommandPrompt(QString,QString)));

    connect(prompt, SIGNAL(deletePressed()), this, SLOT(deletePressed()));
    //TODO: connect(prompt, SIGNAL(tabPressed()), this, SLOT(someUnknownSlot()));
    connect(prompt, SIGNAL(escapePressed()), this, SLOT(escapePressed()));
    connect(prompt, SIGNAL(upPressed()), this, SLOT(promptInputPrevious()));
    connect(prompt, SIGNAL(downPressed()), this, SLOT(promptInputNext()));
    connect(prompt, SIGNAL(F1Pressed()), this, SLOT(help()));
    //TODO: connect(prompt, SIGNAL(F2Pressed()), this, SLOT(floatHistory()));
    //TODO: connect(prompt, SIGNAL(F3Pressed()), this, SLOT(toggleQSNAP()));
    connect(prompt, SIGNAL(F4Pressed()), this, SLOT(toggleLwt())); //TODO: typically this is toggleTablet(), make F-Keys customizable thru settings
    //TODO: connect(prompt, SIGNAL(F5Pressed()), this, SLOT(toggleISO()));
    //TODO: connect(prompt, SIGNAL(F6Pressed()), this, SLOT(toggleCoords()));
    connect(prompt, SIGNAL(F7Pressed()), this, SLOT(toggleGrid()));
    //TODO: connect(prompt, SIGNAL(F8Pressed()), this, SLOT(toggleORTHO()));
    //TODO: connect(prompt, SIGNAL(F9Pressed()), this, SLOT(toggleSNAP()));
    //TODO: connect(prompt, SIGNAL(F10Pressed()), this, SLOT(togglePOLAR()));
    //TODO: connect(prompt, SIGNAL(F11Pressed()), this, SLOT(toggleQTRACK()));
    connect(prompt, SIGNAL(F12Pressed()), this, SLOT(toggleRuler()));
    connect(prompt, SIGNAL(cutPressed()), this, SLOT(cut_action()));
    connect(prompt, SIGNAL(copyPressed()), this, SLOT(copy_action()));
    connect(prompt, SIGNAL(pastePressed()), this, SLOT(paste_action()));
    connect(prompt, SIGNAL(selectAllPressed()), this, SLOT(selectAll()));
    connect(prompt, SIGNAL(undoPressed()), this, SLOT(undo()));
    connect(prompt, SIGNAL(redoPressed()), this, SLOT(redo()));

    connect(prompt, SIGNAL(shiftPressed()), this, SLOT(setShiftPressed()));
    connect(prompt, SIGNAL(shiftReleased()), this, SLOT(setShiftReleased()));

    connect(prompt, SIGNAL(showSettings()), this, SLOT(settingsPrompt()));

    connect(prompt, SIGNAL(historyAppended(QString)), this, SLOT(promptHistoryAppended(QString)));

    /* create the Object Property Editor */
    dockPropEdit = new PropertyEditor(
        appDir + "/icons/" + icon_theme,
        settings[ST_SELECTION_PICK_ADD].i, prompt, this);
    addDockWidget(Qt::LeftDockWidgetArea, dockPropEdit);
    connect(dockPropEdit, SIGNAL(pickAddModeToggled()), this, SLOT(pickAddModeToggled()));

    /* create the Command History Undo Editor */
    dockUndoEdit = new UndoEditor(appDir + "/icons/" + icon_theme, prompt, this);
    addDockWidget(Qt::LeftDockWidgetArea, dockUndoEdit);

    //setDockOptions(QMainWindow::AnimatedDocks | QMainWindow::AllowTabbedDocks | QMainWindow::VerticalTabs); //TODO: Load these from settings
    //tabifyDockWidget(dockPropEdit, dockUndoEdit); //TODO: load this from settings

    statusbar = new StatusBar(this);
    this->setStatusBar(statusbar);

    createAllActions();
    createAllMenus();
    createAllToolbars();

    iconResize(settings[ST_ICON_SIZE].i);
    updateMenuToolbarStatusbar();

    //Show date in statusbar after it has been updated
    QDate date = QDate::currentDate();
    QString datestr = date.toString("MMMM d, yyyy");
    statusbar->showMessage(datestr);

    showNormal();

    if (settings[ST_TIP_OF_THE_DAY].i) {
        actuator_core(ACTION_TIP_OF_THE_DAY, "");
    }
}

/* MainWindow::~MainWindow
 */
MainWindow::~MainWindow()
{
    debug_message("MainWindow::Destructor()");

    //Prevent memory leaks by deleting any unpasted objects
    qDeleteAll(cutCopyObjectList.begin(), cutCopyObjectList.end());
    cutCopyObjectList.clear();
}

/* MainWindow::createAllActions
 *
 * \todo Set What's This Context Help to statusTip for now so there is some infos there.
 * Make custom whats this context help popup with more descriptive help than just
 * the status bar/tip one liner(short but not real long) with a hyperlink in the custom popup
 * at the bottom to open full help file description. Ex: like wxPython AGW's SuperToolTip.
ACTION->setWhatsThis(statusTip);
 * \todo Finish All Commands ... <.<
 * .
 * If an action calls a script then there will be an entry in
 * config that is a std::vector<std::string> to be interpreted as a script.
 *
 * An alias is another entry in config that is also a std::vector<std::string>
 * containing just the name of the command it aliases.
 *
 * icon: The stub used for the icon and the basic command.
 * command:
 * tooltip: The label in the menus and the message that appears when
 *                       you hover over an icon.
 * statustip: The message that appears at the bottom of the .
 * shortcut: The keyboard shortcut for this action.
 */
void
MainWindow::createAllActions()
{
    debug_message("Creating All Actions...");

    for (int i=0; i<N_ACTIONS; i++) {
        ActionData a = action_table[i];

        std::string icon_s(a.icon);
        QString tooltip(a.tooltip);
        QString statustip(a.statustip);
        std::string shortcut(a.shortcut);
        QIcon icon = create_icon(a.icon);

        QAction *ACTION = new QAction(icon, tooltip, this);
        ACTION->setStatusTip(statustip);
        ACTION->setObjectName(icon_s);
        if (shortcut != "") {
            ACTION->setShortcut(
                QKeySequence(QString::fromStdString(shortcut))
            );
        }

        if (   (icon_s == "textbold")
            || (icon_s == "textitalic")
            || (icon_s == "textunderline")
            || (icon_s == "textstrikeout")
            || (icon_s == "textoverline")) {
            ACTION->setCheckable(true);
        }

        connect(ACTION, &QAction::triggered, this, [=](){ actuator(a.command); });
        actionHash[a.id] = ACTION;
    }

    actionHash[ACTION_WINDOW_CLOSE]->setEnabled(numOfDocs > 0);
    actionHash[ACTION_DESIGN_DETAILS]->setEnabled(numOfDocs > 0);
}

/* A basic line-by-line script processor to allow for extensions to the program.
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
run_script(std::vector<std::string> script)
{
    std::string output = "";
    for (int i=0; i<(int)script.size(); i++) {
        debug_message(script[i]);
        output += actuator(script[i]);
    }
    return output;
}

#define MAX_ARGS                         10

/* .
 */
const char *
actuator(char string[MAX_STRING_LENGTH])
{
    std::string s(string);
    s = actuator(s);
    return s.c_str();
}

/* actuator(command)
 *
 * RUN COMMAND
 * -----------
 * QAction* act = qobject_cast<QAction*>(sender());
 * if (act) {
 *     prompt->endCommand();
 *     prompt->setCurrentText(act->objectName());
 *     prompt->processInput();
 * }
 *
 * INIT
 * ----
 * QString fileName = "commands/" + cmd + "/" + cmd + ".js";
 * if (!getSettingsSelectionModePickFirst()) { actuator("clear-selection"); }
 * TODO: Uncomment this line when post-selection is available
 * engine->evaluate(cmd + "_main(void)", fileName);
 *
 * PROMPT
 * ------
 * QString fileName = "commands/" + cmd + "/" + cmd + ".js";
 * NOTE: Replace any special characters that will cause a syntax error
 * QString safeStr = str;
 * safeStr.replace("\\", "\\\\");
 * safeStr.replace("\'", "\\\'");
 *
 * if (prompt->promptInput->rapidFireEnabled) {
 *     engine->evaluate(cmd + "_prompt('" + safeStr + "')", fileName);
 * }
 * else {
 *     engine->evaluate(cmd + "_prompt('" + safeStr.toUpper() + "')", fileName);
 * }
 */
std::string
actuator(std::string line)
{
    char args[MAX_STRING_LENGTH];
    char *argv[100];
    char error_str[MAX_STRING_LENGTH];
    int i;
    int found_space = 0;

    for (i=0; i<(int)line.size(); i++) {
        if (line[i] == ' ') {
            found_space = 1;
            break;
        }
    }

    strcpy(args, line.c_str());
    int argc = tokenize(argv, args, ' ');
    int action_id = -1;

    if (found_space) {
        for (i=0; i<N_ACTIONS; i++) {
            if (!strncmp(argv[0], command_labels[i], strlen(command_labels[i]))) {
                action_id = i;
                break;
            }
        }
    }
    else {
        for (i=0; i<N_ACTIONS; i++) {
            if (!strncmp((char*)line.c_str(), command_labels[i], strlen(command_labels[i]))) {
                action_id = i;
                break;
            }
        }
    }

    /* This could produce silly amounts of output, so watch this line. */
    sprintf(error_str, "action: %d\n", action_id);
    debug_message(error_str);

    if (action_id < 0) {
        char out[2*MAX_STRING_LENGTH];
        sprintf(out, "<br/><font color=\"red\">Unknown command \"%s\". Press F1 for help.</font>", line.c_str());
        std::string output(out);
        return output;
    }

    strcpy(args, line.c_str() + strlen(command_labels[action_id]) + 1);
    std::string args_(args);
    return actuator_core(action_id, args_);
}

/* To speed up the majority of actuator calls, rather than using the command
 * hash, jump to the command directly with a jump table.
 *
 * Most calls should use this version directly rather than the CLI style version.
 */
std::string
actuator_core(int32_t action_id, std::string args_)
{
    std::vector<Node> result;
    char args[MAX_STRING_LENGTH];
    char *argv[MAX_STRING_LENGTH];
    View* gview = activeView();
    debug_message("actuator_core " + std::to_string(action_id));
    strcpy(args, args_.c_str());
    int argc = tokenize(argv, args, ' ');

    switch (action_id) {

    /* Open the about dialog. */
    case ACTION_ABOUT: {
        _mainWin->about();
        return "";
    }

    /* add_arc_action.
     *
     * EmbReal startX, EmbReal startY, EmbReal midX, EmbReal midY, EmbReal endX, EmbReal endY, std::string rubberMode
     */
    case ACTION_ADD_ARC: {
        QGraphicsScene* scene = activeScene();
        if (gview && scene) {
            /*
            EmbArc arc;
            arc.start.x = startX;
            arc.start.x = -startY;
            arc.mid.x = midX;
            arc.mid.x = -midY;
            arc.end.x = endX;
            arc.end.x = -endY;
            ArcObject* arcObj = new ArcObject(arc,_mainWin->getCurrentColor());
            arcObj->setObjectRubberMode(rubberMode);
            if (rubberMode != "OBJ_RUBBER_OFF") {
                gview->addToRubberRoom(arcObj);
            }
            scene->addItem(arcObj);
            scene->update();
            */
        }
        return "";
    }

    /* add_circle_action.
     *
     * EmbReal centerX, EmbReal centerY, EmbReal radius, bool fill, std::string rubberMode
     */
    case ACTION_ADD_CIRCLE: {
        QGraphicsScene* gscene = gview->scene();
        QUndoStack* stack = gview->undoStack;
        if (gview && gscene && stack) {
            EmbCircle circle;
            circle.center.x = 0.0;
            circle.center.y = -0.0;
            circle.radius = 10.0;
            bool fill = false;
            std::string rubberMode = "OBJ_RUBBER_OFF";

            /*
            Geometry* obj = new Geometry(circle, _mainWin->getCurrentColor(), Qt::SolidLine);
            obj->objRubberMode = rubberMode;
            //TODO: circle fill
            if (rubberMode != "OBJ_RUBBER_OFF") {
                gview->addToRubberRoom(obj);
                gscene->addItem(obj);
                gscene->update();
            }
            else {
                UndoableAddCommand* cmd = new UndoableAddCommand(obj->data(OBJ_NAME).toString(), obj, gview, 0);
                stack->push(cmd);
            }
            */
        }
        return "";
    }

    /* EmbReal x1, EmbReal y1, EmbReal x2, EmbReal y2, EmbReal rot, std::string rubberMode
     */
    case ACTION_ADD_DIM_LEADER: {
        /*
        AddDimLeader(std::vector<Node> a)
        _mainWin->nativeAddDimLeader(a[0].r, a[1].r, a[2].r, a[3].r, a[4].r, OBJ_RUBBER_OFF);
        */
        QGraphicsScene* gscene = gview->scene();
        QUndoStack* stack = gview->undoStack;
        if (gview && gscene && stack) {
            /*
            Geometry* obj = new Geometry(x1, -y1, x2, -y2,_mainWin->getCurrentColor());
            obj->setRotation(-rot);
            obj->setObjectRubberMode(rubberMode);
            if (rubberMode != "OBJ_RUBBER_OFF") {
                gview->addToRubberRoom(obj);
                gscene->addItem(obj);
                gscene->update();
            }
            else {
                UndoableAddCommand* cmd = new UndoableAddCommand(obj->data(OBJ_NAME).toString(), obj, gview, 0);
                stack->push(cmd);
            }
            */
        }
        return "";
    }

    /* Add an ellipse to the scene.
     *
     * EmbReal centerX, EmbReal centerY, EmbReal width, EmbReal height, EmbReal rot, bool fill, std::string rubberMode
     */
    case ACTION_ADD_ELLIPSE: {

        QGraphicsScene* gscene = gview->scene();
        QUndoStack* stack = gview->undoStack;
        if (gview && gscene && stack) {
            /*
            Geometry* obj = new Geometry(centerX, -centerY, width, height,_mainWin->getCurrentColor());
            obj->setRotation(-rot);
            obj->setObjectRubberMode(rubberMode);
            //TODO: ellipse fill
            if (rubberMode != "OBJ_RUBBER_OFF") {
                gview->addToRubberRoom(obj);
                gscene->addItem(obj);
                gscene->update();
            }
            else {
                UndoableAddCommand* cmd = new UndoableAddCommand(obj->data(OBJ_NAME).toString(), obj, gview, 0);
                stack->push(cmd);
            }
            */
        }

        return "";
    }

    /* add_geometry_action args
     */
    case ACTION_ADD_GEOMETRY: {
        std::string command(argv[0]);
        /*
        args_ = args_.substr(std::min(command.size()+1, argc));
        std::vector<std::string> subcommands = {
            "arc",
            "circle",
            "ellipse",
            "horizontal_dimension",
            "image",
            "path",
            "point",
            "polygon",
            "polyline",
            "rectangle",
            "regular_polygon",
            "vertical_dimension",
            "dim_leader",
            "infinite_line",
            "ray",
            "line",
            "triangle",
            "text_multi",
            "text_single",
            "rounded-rectangle"
        };
        if (contains(subcommands, command)) {
            return actuator("add_" + command + " " + args_);
        }
        */

        return "The add subcommand is not recognised.";
    }

    /*
     * EmbReal x1, EmbReal y1, EmbReal x2, EmbReal y2, EmbReal legHeight
     */
    case ACTION_ADD_HORIZONTAL_DIMENSION: {
        /*
        AddHorizontalDimension(std::vector<Node> a)
        //TODO: Node error checking
        debug_message("TODO: finish addHorizontalDimension command");
        */
        return "";
    }

    /*
     * QString img, EmbReal x, EmbReal y, EmbReal w, EmbReal h, EmbReal rot
     */
    case ACTION_ADD_IMAGE: {
        /*
        AddImage(std::vector<Node> a)
        //TODO: Node error checking
        debug_message("TODO: finish addImage command");
        */
        return "";
    }

    /* .
     * EmbReal x1, EmbReal y1, EmbReal x2, EmbReal y2, EmbReal rot
     */
    case ACTION_ADD_INFINITE_LINE: {
        /*
        //TODO: Node error checking
        debug_message("TODO: finish addInfiniteLine command");
        */
        return "";
    }

    /*
     * .
     * EmbReal x1, EmbReal y1, EmbReal x2, EmbReal y2, EmbReal rot, std::string rubberMode
     */
    case ACTION_ADD_LINE: {
        /*
        _mainWin->nativeadd_line_action(a[0].r, a[1].r, a[2].r, a[3].r, a[4].r, OBJ_RUBBER_OFF);
        QGraphicsScene* gscene = gview->scene();
        QUndoStack* stack = gview->undoStack;
        if (gview && gscene && stack) {
            EmbLine line;
            line.start.x = x1;
            line.start.y = -y1;
            line.end.x = x2;
            line.end.y = -y2;
            LineObject* obj = new LineObject(line,_mainWin->getCurrentColor());
            obj->setRotation(-rot);
            obj->setObjectRubberMode(rubberMode);
            if (rubberMode != "OBJ_RUBBER_OFF") {
                gview->addToRubberRoom(obj);
                gscene->addItem(obj);
                gscene->update();
            }
            else {
                UndoableAddCommand* cmd = new UndoableAddCommand(obj->data(OBJ_NAME).toString(), obj, gview, 0);
                stack->push(cmd);
            }
        }
        */
        return "";
    }

    /*
     * NOTE: This native is different than the rest in that
     * the Y+ is down (scripters need not worry about this).
     *
     * EmbReal startX, EmbReal startY, const QPainterPath& p, std::string rubberMode
     */
    case ACTION_ADD_PATH: {
        /*
        AddPath(std::vector<Node> a)
        // TODO: Node error checking
        debug_message("TODO: finish addPath command");
        */
        return "";
    }

    /* add_point_action
     * args
     */
    case ACTION_ADD_POINT: {
        QUndoStack* stack = gview->undoStack;
        if (gview && stack) {
            /*
            EmbVector v;
            v.x = x;
            v.y = -y;
            */
            /*
            Geometry* obj = new Geometry(v, _mainWin->getCurrentColor(), Qt::SolidLine);
            UndoableAddCommand* cmd = new UndoableAddCommand(obj->data(OBJ_NAME).toString(), obj, gview, 0);
            stack->push(cmd);
            */
        }
        return "";
    }

    /* add_polygon_action
     * args
     *
     * NOTE: This native is different than the rest in that the Y+ is down (scripters need not worry about this)
     * EmbReal startX, EmbReal startY, const QPainterPath& p, std::string rubberMode
     */
    case ACTION_ADD_POLYGON: {
        /*
        QGraphicsScene* gscene = gview->scene();
        QUndoStack* stack = gview->undoStack;
        if (gview && gscene && stack) {
            PolygonObject* obj = new PolygonObject(startX, startY, p,_mainWin->getCurrentColor());
            obj->setObjectRubberMode(rubberMode);
            if (rubberMode != "OBJ_RUBBER_OFF") {
                gview->addToRubberRoom(obj);
                gscene->addItem(obj);
                gscene->update();
            }
            else {
                UndoableAddCommand* cmd = new UndoableAddCommand(obj->data(OBJ_NAME).toString(), obj, gview, 0);
                stack->push(cmd);
            }
        }

        QVariantList varList = a[0].toVariant().toList();
        int varSize = varList.size();
        if (varSize < 2) {
            return "TYPE ERROR: add_polygon_action(): array must contain at least two elements";
        }
        if (varSize % 2) {
            return "TYPE ERROR: add_polygon_action(): array cannot contain an odd number of elements";
        }

        bool lineTo = false;
        bool xCoord = true;
        EmbReal x = 0;
        EmbReal y = 0;
        EmbReal startX = 0;
        EmbReal startY = 0;
        QPainterPath path;
        foreach(QVariant var, varList) {
            if (var.canConvert(QVariant::Double)) {
                if (xCoord) {
                    xCoord = false;
                    x = var.toReal();
                }
                else {
                    xCoord = true;
                    y = -var.toReal();

                    if (lineTo) { path.lineTo(x,y); }
                    else { path.moveTo(x,y); lineTo = true; startX = x; startY = y; }
                }
            }
            else {
                return "TYPE ERROR: add_polygon_action(): array contains one or more invalid elements");
            }
        }

        //Close the polygon
        path.closeSubpath();

        path.translate(-startX, -startY);

        _mainWin->nativeadd_polygon_action(startX, startY, path, OBJ_RUBBER_OFF);
        */
        return "";
    }

    case ACTION_ADD_POLYLINE: {
        return "";
    }

    /*
     * .
     * EmbReal x1, EmbReal y1, EmbReal x2, EmbReal y2, EmbReal rot
     */
    case ACTION_ADD_RAY: {
        /*
        //TODO: Node error checking
        debug_message("TODO: finish addRay command");
        */
        return "";
    }

    /* . */
    case ACTION_ADD_RECTANGLE: {
        if (!gview) {
            return "ERROR: no active view found.";
        }
        QGraphicsScene* gscene = gview->scene();
        if (!gscene) {
            return "ERROR: no graphics scene view found.";
        }
        QUndoStack* stack = gview->undoStack;
        if (stack) {
            EmbRect rect;
            rect.left = atof(argv[0]);
            rect.right = -atof(argv[1]);
            rect.top = atof(argv[2]);
            rect.bottom = -atof(argv[3]);
            EmbReal rot = atof(argv[4]);
            bool fill = (argv[5] == "1");
            std::string rubberMode = argv[6];

            /*
            Geometry* obj = new Geometry(rect, _mainWin->getCurrentColor(), Qt::SolidLine);
            obj->setRotation(-rot);
            obj->objRubberMode = rubberMode;
            //TODO: rect fill
            if (rubberMode != "OBJ_RUBBER_OFF") {
                gview->addToRubberRoom(obj);
                gscene->addItem(obj);
                gscene->update();
            }
            else {
                UndoableAddCommand* cmd = new UndoableAddCommand(obj->data(OBJ_NAME).toString(), obj, gview, 0);
                stack->push(cmd);
            }
            */
        }
        return "";
    }

    /* AddRegularPolygon
     *
     * EmbReal centerX, EmbReal centerY, quint16 sides, uint8_t mode,
     * EmbReal rad, EmbReal rot, bool fill
     */
    case ACTION_ADD_REGULAR_POLYGON: {
        return "";
    }

    /* add_rounded_rectangle_action.
     *
     * EmbReal x, EmbReal y, EmbReal w, EmbReal h, EmbReal rad, EmbReal rot, bool fill
     */
    case ACTION_ADD_ROUNDED_RECTANGLE: {
        /*
        _mainWin->nativeadd_rounded_rectangle_action(
            a[0].r, a[1].r, a[2].r, a[3].r, a[4].r, a[5].r, a[6].b);
        */
        return "";
    }

    /* add_rubber_action
     * args
     */
    case ACTION_ADD_RUBBER: {
        //QString objType = QString::fromStdString(a[0].s).toUpper();

        /*
        if (!_mainWin->nativeallow_rubber_action()) {
            return context->throwError(QScriptContext::UnknownError, "addRubber(): You must use actuator("vulcanize") before you can add another rubber object.");
        }

        EmbReal mx = _mainWin->nativemouse_x_action();
        EmbReal my = _mainWin->nativemouse_y_action();

        if (objType == "ARC") {
             //TODO: handle this type
        }
        else if (objType == "BLOCK") {

        } //TODO: handle this type
        else if (objType == "CIRCLE") {
            _mainWin->nativeAddCircle(mx, my, 0, false, OBJ_RUBBER_ON);
        }
        else if (objType == "DIMALIGNED") {

        } //TODO: handle this type
        else if (objType == "DIMANGULAR") {

        } //TODO: handle this type
        else if (objType == "DIMARCLENGTH") {

        } //TODO: handle this type
        else if (objType == "DIMDIAMETER") {

        } //TODO: handle this type
        else if (objType == "DIMLEADER") {
            _mainWin->nativeAddDimLeader(mx, my, mx, my, 0, OBJ_RUBBER_ON);
        }
        else if (objType == "DIMLINEAR") {

        } //TODO: handle this type
        else if (objType == "DIMORDINATE") {

        } //TODO: handle this type
        else if (objType == "DIMRADIUS") {

        } //TODO: handle this type
        else if (objType == "ELLIPSE") {
            _mainWin->nativeAddEllipse(mx, my, 0, 0, 0, 0, OBJ_RUBBER_ON);
        }
        else if (objType == "ELLIPSEARC") {

        } //TODO: handle this type
        else if (objType == "HATCH") {
        } //TODO: handle this type
        else if (objType == "IMAGE") {
        } //TODO: handle this type
        else if (objType == "INFINITELINE") {
        } //TODO: handle this type
        else if (objType == "LINE") {
            _mainWin->nativeadd_line_action(mx, my, mx, my, 0, OBJ_RUBBER_ON);
        }
        else if (objType == "PATH") {
            actuator("todo handle path type in add_rubber_action.);
        }
        else if (objType == "POINT") {
            actuator("todo handle point type in add_rubber_action.);
        }
        else if (objType == "POLYGON") {
            _mainWin->nativeadd_polygon_action(mx, my, QPainterPath(), OBJ_RUBBER_ON);
        }
        else if (objType == "POLYLINE") {
            _mainWin->nativeAddPolyline(mx, my, QPainterPath(), OBJ_RUBBER_ON);
        }
        else if (objType == "RAY") {
            actuator("todo handle ray type in add_rubber_action.);
        }
        else if (objType == "RECTANGLE") {
            _mainWin->nativeadd_rectangle_action(mx, my, mx, my, 0, 0, OBJ_RUBBER_ON);
        }
        else if (objType == "SPLINE") {
            actuator("todo handle spline type in add_rubber_action.);
        }
        else if (objType == "TEXTMULTI") {
            actuator("todo handle text multi type in add_rubber_action.);
        }
        else if (objType == "TEXTSINGLE") {
            _mainWin->nativeadd_text_single_action("", mx, my, 0, false, OBJ_RUBBER_ON);
        }
        */
        return "";
    }

    /* add_slot_action
     * args
     *
     * EmbReal centerX, EmbReal centerY, EmbReal diameter, EmbReal length, EmbReal rot, bool fill, std::string rubberMode
     */
    case ACTION_ADD_SLOT: {
        //TODO: Use UndoableAddCommand for slots
        /*
        SlotObject* slotObj = new SlotObject(centerX, -centerY, diameter, length,_mainWin->getCurrentColor());
        slotObj->setRotation(-rot);
        slotObj->setObjectRubberMode(rubberMode);
        if (rubberMode) gview->addToRubberRoom(slotObj);
        scene->addItem(slotObj);
        //TODO: slot fill
        scene->update();
        */
        //_mainWin->nativeAddSlot(a[0].r, a[1].r, a[2].r, a[3].r, a[4].r, a[5].b, OBJ_RUBBER_OFF);

        return "";
    }

    /* add_text_multi_action
     *
     * QString str, EmbReal x, EmbReal y, EmbReal rot, bool fill, std::string rubberMode
     */
    case ACTION_ADD_TEXT_MULTI: {
        /*
        _mainWin->nativeadd_text_multi_action(a[0].s, a[1].r, a[2].r, a[3].r, a[4].b, OBJ_RUBBER_OFF);
        */
        return "";
    }

    /* add_text_single_action
     *
     * QString str, EmbReal x, EmbReal y, EmbReal rot, bool fill, std::string rubberMode
     */
    case ACTION_ADD_TEXT_SINGLE: {
        /*
        _mainWin->nativeadd_text_single_action(a[0].s, a[1].r, a[2].r, a[3].r, a[4].b, OBJ_RUBBER_OFF);
        */
        QGraphicsScene* gscene = gview->scene();
        QUndoStack* stack = gview->undoStack;
        if (gview && gscene && stack) {
            /*
            TextSingleObject* obj = new TextSingleObject(str, x, -y, _mainWin->getCurrentColor());
            obj->setObjectTextFont(QString::fromStdString(settings.text_font));
            obj->setObjectTextSize(settings.text_size);
            obj->setObjectTextStyle(settings.text_style_bold,
                                    settings.text_style_italic,
                                    settings.text_style_underline,
                                    settings.text_style_strikeout,
                                    settings.text_style_overline);
            obj->setObjectTextBackward(false);
            obj->setObjectTextUpsideDown(false);
            obj->setRotation(-rot);
            //TODO: single line text fill
            obj->setObjectRubberMode(rubberMode);
            if (rubberMode != "OBJ_RUBBER_OFF") {
                gview->addToRubberRoom(obj);
                gscene->addItem(obj);
                gscene->update();
            }
            else {
                UndoableAddCommand* cmd = new UndoableAddCommand(obj->data(OBJ_NAME).toString(), obj, gview, 0);
                stack->push(cmd);
            }
            */
        }
        return "";
    }

    /* . */
    case ACTION_ADD_TO_SELECTION: {
        return "";
    }

    /* . */
    case ACTION_ADD_TRIANGLE: {
        /*
        _mainWin->nativeadd_triangle_action(a[0].r, a[1].r, a[2].r, a[3].r, a[4].r, a[5].r, a[6].r, a[7].b);
        */
        return "";
    }

    /* TODO: Node error checking
     *
     * EmbReal x1, EmbReal y1, EmbReal x2, EmbReal y2, EmbReal legHeight
     */
    case ACTION_ADD_VERTICAL_DIMENSION: {
        /*
        debug_message("TODO: finish addVerticalDimension command");
        */
        return "";
    }

    /* Alert the user with a message box. */
    case ACTION_ALERT: {
        prompt->alert(QString::fromStdString(args));
        return "";
    }

    /* AllowRubber. */
    case ACTION_ALLOW_RUBBER: {
        if (gview) {
            return std::to_string(gview->allowRubber());
        }
        return "false";
    }

    /* Can accept no argument. */
    case ACTION_APPEND_HISTORY: {
        prompt->appendHistory(QString::fromStdString(args));
        return "";
    }

    /* . */
    case ACTION_CALCULATE_ANGLE: {
        EmbReal x1 = atof(argv[0]);
        EmbReal y1 = atof(argv[1]);
        EmbReal x2 = atof(argv[2]);
        EmbReal y2 = atof(argv[3]);
        return std::to_string(QLineF(x1, -y1, x2, -y2).angle());
    }

    /* . */
    case ACTION_CALCULATE_DISTANCE: {
        EmbReal x1 = atof(argv[0]);
        EmbReal y1 = atof(argv[1]);
        EmbReal x2 = atof(argv[2]);
        EmbReal y2 = atof(argv[3]);
        return std::to_string(QLineF(x1, y1, x2, y2).length());
    }

    /* Open the changelog dialog. */
    case ACTION_CHANGELOG: {
        QUrl changelogURL("help/changelog.html");
        QDesktopServices::openUrl(changelogURL);
        return "";
    }

    /* ClearRubber. */
    case ACTION_CLEAR_RUBBER: {
        if (gview) {
            gview->clearRubberRoom();
        }
        return "";
    }

    /* . */
    case ACTION_CLEAR_SELECTION: {
        if (gview) {
            gview->clearSelection();
        }
        return "";
    }

    /* Copy the selected objects to this activeView's clipboard. */
    case ACTION_COPY: {
        if (gview) {
            gview->copy();
        }
        return "";
    }

    case ACTION_COPY_SELECTED: {
        /* _mainWin->nativeCopySelected(a[0].r, a[1].r); */
        return "";
    }

    /* Cut the selected objects, placing them into the clipboard variable
     * for this gview.
     */
    case ACTION_CUT: {
        if (gview) {
            gview->cut();
        }
        return "";
    }

    case ACTION_CUT_SELECTED: {
        /* _mainWin->nativeCutSelected(a[0].r, a[1].r); */
        return "";
    }

    /* Activate day vision.
     * TODO: Make day vision color settings.
     */
    case ACTION_DAY: {
        if (gview) {
            gview->setBackgroundColor(qRgb(255,255,255));
            gview->setCrossHairColor(qRgb(0,0,0));
            gview->setGridColor(qRgb(0,0,0));
        }
        return "";
    }

    /* Allows scripts to produce debug output similar to "echo". */
    case ACTION_DEBUG: {
        return "DEBUG: " + args_;
    }

    case ACTION_DELETE_SELECTED: {
        return "";
    }

    /* Open the design details dialog. */
    case ACTION_DESIGN_DETAILS: {
        QGraphicsScene* scene = activeScene();
        if (scene) {
            EmbDetailsDialog dialog(scene, _mainWin);
            dialog.exec();
        }
        return "";
    }

    /* This action intensionally does nothing. */
    case ACTION_DO_NOTHING: {
        return "";
    }

    /* For scripts: ensure that the script is tidied up. */
    case ACTION_END: {
        if (gview) {
            gview->clearRubberRoom();
            gview->previewOff();
            gview->rapidMoveActive = false;
        }
        prompt->promptInput->endCommand();
        return "";
    }

    /* Bring up the error messagebox. */
    case ACTION_ERROR: {
        /*
        _mainWin->setPromptPrefix("ERROR: (" + a[0].s + ") " + a[1].s);
        */
        actuator("append-prompt-history");
        actuator("end");
        return "";
    }

    /* Open the help dialog. */
    case ACTION_HELP: {
        // Open the HTML Help in the default browser
        QUrl helpURL("file:///" + qApp->applicationDirPath() + "/help/doc-index.html");
        QDesktopServices::openUrl(helpURL);

        //TODO: This is how to start an external program. Use this elsewhere...
        //QString program = "firefox";
        //QStringList arguments;
        //arguments << "help/commands.html";
        //QProcess *myProcess = new QProcess(this);
        //myProcess->start(program, arguments);
        return "";
    }

    case ACTION_ICON: {
        if (args_ == "16") {
            _mainWin->iconResize(16);
            return "";
        }
        if (args_ == "24") {
            _mainWin->iconResize(24);
            return "";
        }
        if (args_ == "32") {
            _mainWin->iconResize(32);
            return "";
        }
        if (args_ == "48") {
            _mainWin->iconResize(48);
            return "";
        }
        if (args_ == "64") {
            _mainWin->iconResize(64);
            return "";
        }
        if (args_ == "128") {
            _mainWin->iconResize(128);
            return "";
        }
        return "ERROR: this icon size is not supported.";
    }

    /* For scripts: clear out any current variables before running. */
    case ACTION_INIT: {
        if (gview) {
            gview->clearRubberRoom();
        }
        return "";
    }

    /* MessageBox
     * type, title, text
     */
    case ACTION_MESSAGEBOX: {
        QString type, title, text;
        /*
        QString msgType = type.toLower();
        if (msgType == "critical") {
            QMessageBox::critical(_mainWin, tr(qPrintable(title)), tr(qPrintable(text)));
        }
        else if (msgType == "information") {
            QMessageBox::information(_mainWin, tr(qPrintable(title)), tr(qPrintable(text)));
        }
        else if (msgType == "question") {
            QMessageBox::question(_mainWin, tr(qPrintable(title)), tr(qPrintable(text)));
        }
        else if (msgType == "warning") {
            QMessageBox::warning(_mainWin, tr(qPrintable(title)), tr(qPrintable(text)));
        }
        else {
            QMessageBox::critical(_mainWin, translate_str("Native MessageBox Error"), translate_str("Incorrect use of the native messageBox function."));
        }
        */

        return "";
    }

    /* MirrorSelected(x1, y1, x2, y2)
     */
    case ACTION_MIRROR_SELECTED: {
        if (gview) {
            EmbReal x1 = atof(argv[0]);
            EmbReal y1 = atof(argv[1]);
            EmbReal x2 = atof(argv[2]);
            EmbReal y2 = atof(argv[3]);
            gview->mirrorSelected(x1, -y1, x2, -y2);
        }
        return "";
    }

    /* . */
    case ACTION_MOUSE_X: {
        QGraphicsScene* scene = activeScene();
        if (scene) {
            return std::to_string(scene->property("SCENE_MOUSE_POINT").toPointF().x());
        }
        return "0.0";
    }

    /* . */
    case ACTION_MOUSE_Y: {
        QGraphicsScene* scene = activeScene();
        if (scene) {
            return std::to_string(scene->property("SCENE_MOUSE_POINT").toPointF().y());
        }
        return "0.0";
    }

    /* . */
    case ACTION_MOVE_SELECTED: {
        EmbVector delta;
        delta.x = atof(argv[0]);
        delta.y = -atof(argv[1]);
        View* gview = activeView();
        if (gview) {
            gview->moveSelected(delta);
        }
        return "";
    }

    /* . */
    case ACTION_NEW: {
        _mainWin->newFile();
        return "";
    }

    /* Activate night vision.
     * TODO: Make night vision color settings.
     */
    case ACTION_NIGHT: {
        if (gview) {
            gview->setBackgroundColor(qRgb(0,0,0));
            gview->setCrossHairColor(qRgb(255,255,255));
            gview->setGridColor(qRgb(255,255,255));
        }
        return "0";
    }

    /* Return the number of objects selected currently,
     * if no view is present returns 0.
     */
    case ACTION_NUM_SELECTED: {
        if (gview) {
            return std::to_string(gview->numSelected());
        }
        return "0";
    }

    case ACTION_OPEN: {
        _mainWin->openFile();
        return "";
    }

    /* Pan the current view using behaviour described by "mode". */
    case ACTION_PAN: {
        if (!gview) {
            return "ERROR: no active view found.";
        }
        QUndoStack* stack = gview->undoStack;
        if (!stack) {
            return "ERROR: no undo stack found.";
        }
        if (args_ == "realtime") {
            gview->panRealTime();
            return "";
        }
        if (args_ == "point") {
            gview->panPoint();
            return "";
        }
        if (args_ == "left") {
            UndoableCommand* cmd = new UndoableCommand("PanLeft", gview, 0);
            stack->push(cmd);
            return "";
        }
        if (args_ == "right") {
            UndoableCommand* cmd = new UndoableCommand("PanRight", gview, 0);
            stack->push(cmd);
            return "";
        }
        if (args_ == "up") {
            UndoableCommand* cmd = new UndoableCommand("PanUp", gview, 0);
            stack->push(cmd);
            return "";
        }
        if (args_ == "down") {
            UndoableCommand* cmd = new UndoableCommand("PanDown", gview, 0);
            stack->push(cmd);
            return "";
        }
        return "ERROR: pan subcommand not recognised.";
    }

    /* Copy the current clipboard's objects to the scene. */
    case ACTION_PASTE: {
        if (gview) {
            gview->paste();
        }
        return "";
    }

    case ACTION_PASTE_SELECTED: {
        return "";
    }

    case ACTION_PERPENDICULAR_DISTANCE: {
        EmbReal px = atof(argv[0]);
        EmbReal py = atof(argv[1]);
        EmbReal x1 = atof(argv[2]);
        EmbReal y1 = atof(argv[3]);
        EmbReal x2 = atof(argv[4]);
        EmbReal y2 = atof(argv[5]);
        QLineF line(x1, y1, x2, y2);
        QLineF norm = line.normalVector();
        EmbReal dx = px-x1;
        EmbReal dy = py-y1;
        norm.translate(dx, dy);
        QPointF iPoint;
        norm.intersects(line, &iPoint);
        return std::to_string(QLineF(px, py, iPoint.x(), iPoint.y()).length());
    }

    /* Return the platform running the software (for the CLI). */
    case ACTION_PLATFORM: {
        return platformString();
    }

    /* Turn preview off for this view. */
    case ACTION_PREVIEW_OFF: {
        if (gview) {
            gview->previewOff();
        }
        return "";
    }

    case ACTION_PREVIEW_ON: {
        return "";
    }

    /* Open the system's print dialog. */
    case ACTION_PRINT: {
        MdiWindow* mdiWin = qobject_cast<MdiWindow*>(mdiArea->activeSubWindow());
        if (mdiWin) {
            mdiWin->print();
        }
        return "";
    }

    /* PrintArea
     * EmbReal x, EmbReal y, EmbReal w, EmbReal h
     */
    case ACTION_PRINT_AREA: {
        // qDebug("nativeprint_area_action(%.2f, %.2f, %.2f, %.2f)", x, y, w, h);
        // TODO: Print Setup Stuff
        actuator("print " + args_);
        return "";
    }

    /* QSnapX
     */
    case ACTION_QSNAP_X: {
        QGraphicsScene* scene = activeScene();
        if (scene) {
            float x = scene->property("SCENE_QSNAP_POINT").toPointF().x();
            return std::to_string(x);
        }
        return "0.0";
    }

    /* QSnapX
     */
    case ACTION_QSNAP_Y: {
        QGraphicsScene* scene = activeScene();
        if (scene) {
            float y = scene->property("SCENE_QSNAP_POINT").toPointF().y();
            return std::to_string(y);
        }
        return "0.0";
    }

    /* Close the program. */
    case ACTION_EXIT: {
        _mainWin->quit();
        return "";
    }

    /* Redo the next action as defined on the undo stack.
     * This is described in detail in the documentation.
     * TODO: reference section.
     */
    case ACTION_REDO: {
        QString prefix = prompt->promptInput->prefix;
        if (dockUndoEdit->canRedo()) {
            actuator("set-prompt-prefix Redo " + dockUndoEdit->redoText().toStdString());
            actuator("append-history ");
            dockUndoEdit->redo();
            actuator("set-prompt-prefix " + prefix.toStdString());
        }
        else {
            prompt->alert("Nothing to redo");
            actuator("set-prompt-prefix " + prefix.toStdString());
        }
        return "";
    }

    /* RotateSelected(v, rot)
     */
    case ACTION_ROTATE_SELECTED: {
        if (gview) {
            EmbVector v;
            v.x = atof(argv[0]);
            v.y = -atof(argv[1]);
            EmbReal rot = atof(argv[2]);
            gview->rotateSelected(v, -rot);
        }
        return "";
    }

    /* . */
    case ACTION_RUBBER: {
        /*
        if (command == "allow") {
            allow_rubber_action();
            return "";
        }

        if (command == "set-mode") {
            SetRubberMode();
            return "";
        }

        if (command == "set-point") {
            SetRubberPoint();
            return "";
        }
        if (command == "set-text") {
            SetRubberText();
            return "";
        }

        if (command == "add") {
            AddRubber();
            return "";
        }
        if (command == "clear") {
            clear_rubber_action();
            return "";
        }
        if (command == "spare") {
            spare_rubber_action();
            return "";
        }
        */
        return "";
    }

    case ACTION_SCALE_SELECTED: {
        EmbVector v;
        char *argv[5];
        v.x = atof(argv[0]);
        v.y = -atof(argv[1]);
        EmbReal factor = atof(argv[2]);

        if (factor <= 0.0) {
            QMessageBox::critical(_mainWin,
                translate_str("ScaleFactor Error"),
                translate_str("Hi there. If you are not a developer, report this as a bug. "
                "If you are a developer, your code needs examined, and possibly your head too."));
        }

        if (gview) {
            gview->scaleSelected(v, factor);
        }
        return "";
    }

    /* For every object in the scene, add it to the selected array. */
    case ACTION_SELECT_ALL: {
        if (gview) {
            gview->selectAll();
        }
        return "";
    }

    /* settings_dialog showTab */
    case ACTION_SETTINGS_DIALOG: {
        Settings_Dialog dialog(QString::fromStdString(args_));
        dialog.exec();
        return "";
    }

    /* set_background_color_action
     * uint8_t r, uint8_t g, uint8_t b
     */
    case ACTION_SET_BACKGROUND_COLOR: {
        /*
        EmbReal r = a[0].r;
        EmbReal g = a[1].r;
        EmbReal b = a[2].r;

        if (r < 0 || r > 255) { return context->throwError(QScriptContext::UnknownError, "set_background_color_action(): r value must be in range 0-255"); }
        if (g < 0 || g > 255) { return context->throwError(QScriptContext::UnknownError, "set_background_color_action(): g value must be in range 0-255"); }
        if (b < 0 || b > 255) { return context->throwError(QScriptContext::UnknownError, "set_background_color_action(): b value must be in range 0-255"); }

        settings.display_bg_color = qRgb(r,g,b);
        _mainWin->updateAllViewBackgroundColors(qRgb(r,g,b));
        */
        return "";
    }

    /*
     * .
     * argument string "iii"
     */
    case ACTION_SET_CROSSHAIR_COLOR: {
        /*
        int r = args[0].r;
        int g = args[1].r;
        int b = args[2].r;

        if (!validRGB(r, g, b)) {
        }

        if (r < 0 || r > 255) {
            return "ERROR setCrossHairColor(): r value must be in range 0-255";
        }
        if (g < 0 || g > 255) {
            return "ERROR setCrossHairColor(): g value must be in range 0-255";
        }
        if (b < 0 || b > 255) {
            return "ERROR setCrossHairColor(): b value must be in range 0-255";
        }

        _mainWin->setCrossHairColor(r, g, b);
        */
        return "";
    }

    /* . */
    case ACTION_SET_CURSOR_SHAPE: {
        if (gview) {
            QString shape = QString::fromStdString(args_).toLower();
            if (shape == "arrow") {
                gview->setCursor(QCursor(Qt::ArrowCursor));
            }
            else if (shape == "uparrow") {
                gview->setCursor(QCursor(Qt::UpArrowCursor));
            }
            else if (shape == "cross") {
                gview->setCursor(QCursor(Qt::CrossCursor));
            }
            else if (shape == "wait") {
                gview->setCursor(QCursor(Qt::WaitCursor));
            }
            else if (shape == "ibeam") {
                gview->setCursor(QCursor(Qt::IBeamCursor));
            }
            else if (shape == "resizevert") {
                gview->setCursor(QCursor(Qt::SizeVerCursor));
            }
            else if (shape == "resizehoriz") {
                gview->setCursor(QCursor(Qt::SizeHorCursor));
            }
            else if (shape == "resizediagleft") {
                gview->setCursor(QCursor(Qt::SizeBDiagCursor));
            }
            else if (shape == "resizediagright") {
                gview->setCursor(QCursor(Qt::SizeFDiagCursor));
            }
            else if (shape == "move") {
                gview->setCursor(QCursor(Qt::SizeAllCursor));
            }
            else if (shape == "blank") {
                gview->setCursor(QCursor(Qt::BlankCursor));
            }
            else if (shape == "splitvert") {
                gview->setCursor(QCursor(Qt::SplitVCursor));
            }
            else if (shape == "splithoriz") {
                gview->setCursor(QCursor(Qt::SplitHCursor));
            }
            else if (shape == "handpointing") {
                gview->setCursor(QCursor(Qt::PointingHandCursor));
            }
            else if (shape == "forbidden") {
                gview->setCursor(QCursor(Qt::ForbiddenCursor));
            }
            else if (shape == "handopen") {
                gview->setCursor(QCursor(Qt::OpenHandCursor));
            }
            else if (shape == "handclosed") {
                gview->setCursor(QCursor(Qt::ClosedHandCursor));
            }
            else if (shape == "whatsthis") {
                gview->setCursor(QCursor(Qt::WhatsThisCursor));
            }
            else if (shape == "busy") {
                gview->setCursor(QCursor(Qt::BusyCursor));
            }
            else if (shape == "dragmove") {
                gview->setCursor(QCursor(Qt::DragMoveCursor));
            }
            else if (shape == "dragcopy") {
                gview->setCursor(QCursor(Qt::DragCopyCursor));
            }
            else if (shape == "draglink") {
                gview->setCursor(QCursor(Qt::DragLinkCursor));
            }
        }
        return "";
    }

    /*
     * .
     * argument string "iii"
     */
    case ACTION_SET_GRID_COLOR: {
        /*
        int r = a[0].r;
        int g = a[1].r;
        int b = a[2].r;

        if (r < 0 || r > 255) {
            return "ERROR set_grid_color_action(): r value must be in range 0-255";
        }
        if (g < 0 || g > 255) {
            return "ERROR set_grid_color_action(): g value must be in range 0-255";
        }
        if (b < 0 || b > 255) {
            return "ERROR set_grid_color_action(): b value must be in range 0-255";
        }

        _mainWin->set_grid_color_action(r, g, b);
        */
        return "";
    }

    /* TODO: This setting should override a config variable used to set the prefix. */
    case ACTION_SET_PROMPT_PREFIX: {
        prompt->setPrefix(QString::fromStdString(args_));
        return "";
    }

    /* \todo This is so more than 1 rubber object can exist at one time
     * without updating all rubber objects at once
     */
    case ACTION_SET_RUBBER_FILTER: {
        return args_;
    }

    case ACTION_SET_RUBBER_MODE: {
        /*
        if (gview) {
            std::string mode = QString::fromStdString(a[0].s).toUpper().toStdString();

            if (contains(rubber_modes, mode)) {
                gview->setRubberMode("OBJ_RUBBER_" + mode);
            }
            else {
                return "ERROR: setRubberMode(): unknown rubberMode value";
                //return context->throwError(QScriptContext::UnknownError, "setRubberMode(): unknown rubberMode value");
            }
        }
        */
        return "";
    }

    /* QString key, EmbReal x, EmbReal y
     */
    case ACTION_SET_RUBBER_POINT: {
        /*
        _mainWin->setRubberPoint(a[0].s.toUpper(), a[1].r, a[2].r);
        */
        if (gview) {
            //gview->setRubberPoint(key, QPointF(x, -y));
        }
        return "";
    }

    /* set_rubber_text_action
     * args
     */
    case ACTION_SET_RUBBER_TEXT: {
        if (gview) {
            // if (!contains(args, " ")) { return "requires 2 arguments." }
            // QString key = get_first_word(args);
            // QString txt = args + key[len(args)] + 1;
            // _mainWin->setRubberText(key, a[1].s);
            // gview->setRubberText(key, txt);
        }
        return "";
    }

    /* SpareRubber
     * id
     */
    case ACTION_SPARE_RUBBER: {
        if (gview) {
            int64_t id = std::stoi(args_);
            gview->spareRubber(id);
        }

        //QString objID = QString::fromStdString(a[0].s).toUpper();

        /*
        if (objID == "PATH") {
            _mainWin->nativespare_rubber_action(SPARE_RUBBER_PATH);
        }
        else if (objID == "POLYGON") {
            _mainWin->nativespare_rubber_action(SPARE_RUBBER_POLYGON);
        }
        else if (objID == "POLYLINE") {
            _mainWin->nativespare_rubber_action(SPARE_RUBBER_POLYLINE);
        }
        else {
            bool ok = false;
            qint64 id = objID.toLongLong(&ok);
            if (!ok) return "TYPE ERROR: spare_rubber_action(): error converting object ID into an int64");
            _mainWin->nativespare_rubber_action(id);
        }
        */
        return "";
    }

    case ACTION_SLEEP: {
        emb_sleep(1);
        return "";
    }

    case ACTION_TEST: {
        if (test_program) {
            return run_script(coverage_test_script);
        }
        return "";
    }

    /* Open the Tip of the Day dialog. */
    case ACTION_TIP_OF_THE_DAY: {
        _mainWin->tipOfTheDay();
        return "";
    }

    /* TODO reminders for the developers. */
    case ACTION_TODO: {
        return "TODO: " + args_;
    }

    /* Undo the previous action as defined on the undo stack.
     * This is described in detail in the documentation.
     * TODO: reference section.
     */
    case ACTION_UNDO: {
        QString prefix = prompt->promptInput->prefix;
        if (dockUndoEdit->canUndo()) {
            actuator("set-prompt-prefix Undo "
                + dockUndoEdit->undoText().toStdString());
            /* \bug this won't append history because there's no mechanism
               for and empty string */
            actuator("append-history ");
            dockUndoEdit->undo();
            actuator("set-prompt-prefix " + prefix.toStdString());
        }
        else {
            prompt->alert("Nothing to undo");
            actuator("set-prompt-prefix " + prefix.toStdString());
        }
        return "";
    }

    /* Return the version string to the user (for the CLI). */
    case ACTION_VERSION: {
        std::string v(version);
        return v;
    }

    case ACTION_VULCANIZE: {
        if (gview) {
            gview->vulcanizeRubberRoom();
        }
        return "";
    }

    /* Enter "whats this" mode. */
    case ACTION_WHATS_THIS: {
        QWhatsThis::enterWhatsThisMode();
        return "";
    }

    /* . */
    case ACTION_WINDOW_CASCADE:{
        mdiArea->cascade();
        return "";
    }

    case ACTION_WINDOW_CLOSE:{
        _mainWin->onCloseWindow();
        return "";
    }

    case ACTION_WINDOW_CLOSE_ALL:{
        mdiArea->closeAllSubWindows();
        return "";
    }

    case ACTION_WINDOW_TILE:{
        mdiArea->tile();
        return "";
    }

    case ACTION_WINDOW_NEXT:{
        mdiArea->activateNextSubWindow();
        return "";
    }

    case ACTION_WINDOW_PREVIOUS:{
        mdiArea->activatePreviousSubWindow();
        return "";
    }

    case ACTION_ZOOM: {
        return "";
    }

    /*
    case ACTION_SCRIPT:
        auto script = scripts.find(command);
        if (script != scripts.end()) {
            std::string result = run_script(script->second);
            if (result != "") {
                return "<br/>" + result;
            }
            return "";
        }
        break;
    */

    default: {
        break;
    }

    }

    return "";
}

/* . */
std::string
text_action(std::string args)
{
    /*
    if (list.size() < 1) {
        return "text requires an argument.";
    }
    command = list[0];
    if (command == "font") {
        return std::string(settings[ST_TEXT_FONT].s);
    }
    if (command == "size") {
        return std::to_string(settings[ST_TEXT_SIZE].r);
    }
    if (command == "angle") {
        return std::to_string(settings[ST_TEXT_ANGLE].r);
    }
    if (command == "bold") {
        return std::to_string(settings[ST_TEXT_BOLD].i);
    }
    if (command == "italic") {
        return std::to_string(settings[ST_TEXT_ITALIC].i);
    }
    if (command == "underline") {
        return std::to_string(settings[ST_TEXT_UNDERLINE].i);
    }
    if (command == "strikeout") {
        return std::to_string(settings[ST_TEXT_STRIKEOUT].i);
    }
    if (command == "overline") {
        return std::to_string(settings[ST_TEXT_OVERLINE].i);
    }
    */
    return "";
}

/* Create or alter variables in the script environment. */
std::string
set_action(std::string args)
{
    /*
    if (list.size() < 2) {
        return "The command 'set' requires 2 arguments.";
    }
    int value = list[1] == "true"
        || list[1] == "True"
        || list[1] == "TRUE"
        || list[1] == "on"
        || list[1] == "ON"
        || list[1] == "T"
        || list[1] == "t"
        || list[1] == "1"
    );
    if (list[0] == "text_font") {
        settings[ST_TEXT_FONT].s = list[1];
        return "";
    }
    if (list[0] == "text_size") {
        settings[ST_TEXT_SIZE].r = atof(list[1]);
        return "";
    }
    if (command == "text_angle") {
        settings["text_angle"].i = atof(list[1]);
        return "";
    }
    if (command == "text_style_bold") {
        settings["text_style_bold"] = value;
        return "";
    }
    if (command == "text_style_italic") {
        settings["text_style_italic = value;
        return "";
    }
    if (command == "text_style_underline") {
        settings["text_style_underline = value;
        return "";
    }
    if (command == "text_style_strikeout") {
        settings["text_style_strikeout = value;
        return "";
    }
    if (command == "text_style_overline") {
        settings["text_style_overline = value;
        return "";
    }
    */
    return "";
}

/* . */
std::string
enable_action(std::string args)
{
    /*
    if (list.size() < 1) {
        return "The command 'enable' requires an argument.";
    }
    if (command == "text-angle") {
        settings[ST_TEXT_ANGLE] = 1;
        return "";
    }
    if (command == "text-bold") {
        settings[ST_TEXT_BOLD] = 1;
        return "";
    }
    if (command == "text-italic") {
        settings[ST_TEXT_ITALIC] = 1;
        return "";
    }
    if (command == "text-underline") {
        settings[ST_TEXT_UNDERLINE] = 1;
        return "";
    }
    if (command == "text-strikeout") {
        settings[ST_TEXT_STRIKEOUT] = 1;
        return "";
    }
    if (command == "text-overline") {
        settings[ST_TEXT_OVERLINE] = 1;
        return "";
    }
    if (command == "prompt-rapid-fire") {
        prompt->promptInput->rapidFireEnabled = true;
        return "";
    }
    if (command == "move-rapid-fire") {
        View* gview = activeView();
        if (gview) {
            gview->rapidMoveActive = true;
        }
        return "";
    }
    */
    return "";
}

/* . */
std::string
blink_prompt_action(std::string args)
{
    prompt->startBlinking();
    return "";
}

/* Convert argument list into types needed, inspired by
 * PyArg_ParseTupleAndKeywords allowing a uniform argument parsing framework.
 *
 * * label: The caller's name.
 * * args: The list of strings passed from the user.
 * * args_template: The string of characters describing the types of the output.
 * * result: The fixed length array of results.
 *
 * Returns an error message if an error occured or an empty string if it passes.
 */
std::string
convert_args_to_type(
    std::string label, char *argv[], int argc, const char *args_template, std::vector<Node> a)
{
    int required_args = strlen(args_template);
    if (argc < required_args) {
        std::string required = std::to_string(required_args);
        return "ERROR: " + label + "requires" + required + "arguments";
    }
    for (int i=0; i<argc; i++) {
        switch (args_template[i]) {
        case 'i': {
            Node entry;
            entry.i = atoi(argv[i]);
            a.push_back(entry);
            if (errno == EINVAL) {
                return "TYPE ERROR: failed to convert argument " + std::to_string(i) + " to int.";
            }
            if (errno == ERANGE) {
                return "RANGE ERROR: argument " + std::to_string(i) + " out of range.";
            }
            break;
        }
        case 'r': {
            Node entry;
            entry.r = atof(argv[i]);
            a.push_back(entry);
            if (errno == EINVAL) {
                return "TYPE ERROR: failed to convert argument " + std::to_string(i) + " to floating point.";
            }
            if (std::isnan(a[i].r)) {
                return "NaN ERROR: argument " + std::to_string(i) + " is not a number.";
            }
            break;
        }
        case 's': {
            Node entry;
            strcpy(entry.s, argv[i]);
            a.push_back(entry);
            break;
        }
        default:
            break;
        }
    }
    /* Empty string meaning "passes". */
    return "";
}

/* Run the lines in another script before continuing. */
std::string
include_action(std::string a)
{
/*
    std::string file("commands/");
    file += a[0].s;
    return run_script_file(file);
*/
    return "";
}

std::string
SetTextAngle_action(std::string args)
{
    /*
    _mainWin->setTextAngle(a[0].r);
    */
    return "";
}

/* MainWindow::recentMenuAboutToShow
 */
void
MainWindow::recentMenuAboutToShow()
{
    debug_message("MainWindow::recentMenuAboutToShow()");
    menuHash[MENU_RECENT]->clear();

    QFileInfo recentFileInfo;
    QString recentValue;
    /* doesn't account for quoting/escaped quotes */
    char *argv[100];
    char str[MAX_STRING_LENGTH];
    strcpy(str, settings[ST_RECENT_FILES].s);
    int argc = tokenize(argv, str, ' ');
    for (int i = 0; i < argc; ++i) {
        //If less than the max amount of entries add to menu
        QString file(argv[i]);
        if (i < settings[ST_RECENT_MAX].i) {
            recentFileInfo = QFileInfo(file);
            if (recentFileInfo.exists() && validFileFormat(recentFileInfo.fileName().toStdString())) {
                recentValue.setNum(i+1);
                QAction* rAction;
                if (recentValue.toInt() >= 1 && recentValue.toInt() <= 9) {
                    rAction = new QAction("&" + recentValue + " " + recentFileInfo.fileName(), this);
                }
                else if (recentValue.toInt() == 10) {
                    rAction = new QAction("1&0 " + recentFileInfo.fileName(), this);
                }
                else {
                    rAction = new QAction(recentValue + " " + recentFileInfo.fileName(), this);
                }
                rAction->setCheckable(false);
                rAction->setData(file);
                menuHash[MENU_RECENT]->addAction(rAction);
                connect(rAction, SIGNAL(triggered()), this, SLOT(openrecentfile()));
            }
        }
    }

    /* Ensure the list only has max amount of entries. */
    int max_files = settings[ST_RECENT_MAX].i;
    str[0] = 0;
    for (int i=argc-1; i > argc-1-max_files && i >= 0; i--) {
        strcat(str, argv[i]);
        strcat(str, " ");
    }
    strcpy(settings[ST_RECENT_FILES].s, str);
}

/* MainWindow::windowMenuAboutToShow
 */
void
MainWindow::windowMenuAboutToShow()
{
    debug_message("MainWindow::windowMenuAboutToShow()");
    menuHash[MENU_WINDOW]->clear();
    menuHash[MENU_WINDOW]->addAction(actionHash[ACTION_WINDOW_CLOSE]);
    menuHash[MENU_WINDOW]->addAction(actionHash[ACTION_WINDOW_CLOSE_ALL]);
    menuHash[MENU_WINDOW]->addSeparator();
    menuHash[MENU_WINDOW]->addAction(actionHash[ACTION_WINDOW_CASCADE]);
    menuHash[MENU_WINDOW]->addAction(actionHash[ACTION_WINDOW_TILE]);
    menuHash[MENU_WINDOW]->addSeparator();
    menuHash[MENU_WINDOW]->addAction(actionHash[ACTION_WINDOW_NEXT]);
    menuHash[MENU_WINDOW]->addAction(actionHash[ACTION_WINDOW_PREVIOUS]);

    menuHash[MENU_WINDOW]->addSeparator();
    QList<QMdiSubWindow*> windows = mdiArea->subWindowList();
    for (int i = 0; i < windows.count(); ++i) {
        QAction* aAction = new QAction(windows.at(i)->windowTitle(), this);
        aAction->setCheckable(true);
        aAction->setData(i);
        menuHash[MENU_WINDOW]->addAction(aAction);
        connect(aAction, SIGNAL(toggled(bool)), this, SLOT(windowMenuActivated(bool)));
        aAction->setChecked(mdiArea->activeSubWindow() == windows.at(i));
    }
}

/* MainWindow::windowMenuActivated
 * checked
 */
void
MainWindow::windowMenuActivated(bool checked)
{
    debug_message("MainWindow::windowMenuActivated()");
    QAction* aSender = qobject_cast<QAction*>(sender());
    if (!aSender)
        return;
    QWidget* w = mdiArea->subWindowList().at(aSender->data().toInt());
    if (w && checked)
        w->setFocus();
}

/* MainWindow::newFile
 */
void
MainWindow::newFile()
{
    debug_message("MainWindow::newFile()");
    docIndex++;
    numOfDocs++;
    MdiWindow* mdiWin = new MdiWindow(docIndex, mdiArea, Qt::SubWindow);
    connect(mdiWin, SIGNAL(sendCloseMdiWin(MdiWindow*)), this, SLOT(onCloseMdiWin(MdiWindow*)));
    connect(mdiArea, SIGNAL(subWindowActivated(QMdiSubWindow*)), this, SLOT(onWindowActivated(QMdiSubWindow*)));

    updateMenuToolbarStatusbar();
    windowMenuAboutToShow();

    if (mdiWin->gview) {
        mdiWin->gview->recalculateLimits();
        mdiWin->gview->zoomExtents();
    }
}

/* MainWindow::openFile
 * recent
 * recentFile
 */
void
MainWindow::openFile(bool recent, std::string recentFile)
{
    debug_message("MainWindow::openFile()");

    QApplication::setOverrideCursor(Qt::ArrowCursor);

    bool preview = settings[ST_OPEN_THUMBNAIL].i;
    std::string openFilesPath(settings[ST_RECENT_DIRECTORY].s);

    //Check to see if this from the recent files list
    if (recent) {
        QStringList files;
        files.push_back(QString::fromStdString(recentFile));
        openFilesSelected(files);
    }
    else {
        if (!preview) {
            //TODO: set getOpenFileNames' selectedFilter Node from settings[ST_OPEN_FORMAT]
            QStringList sl = QFileDialog::getOpenFileNames(this,
                translate_str("Open"), QString::fromStdString(openFilesPath), formatFilterOpen);
            /*for (int i=0; i<(int)sl.size(); i++) {
                files.push_back(sl[i].toStdString());
            }
            */
            openFilesSelected(sl);
        }
        else {
            PreviewDialog* openDialog = new PreviewDialog(this, translate_str("Open w/Preview"), QString::fromStdString(openFilesPath), formatFilterOpen);
            //TODO: set openDialog->selectNameFilter(QString  filter) from settings[ST_OPEN_FORMAT]
            connect(openDialog, SIGNAL(filesSelected(QStringList)), this, SLOT(openFilesSelected(QStringList)));
            openDialog->exec();
        }
    }

    QApplication::restoreOverrideCursor();
}

/* MainWindow::openFilesSelected
 * filesToOpen
 */
void
MainWindow::openFilesSelected(QStringList filesToOpen)
{
    bool doOnce = true;

    if (filesToOpen.size() == 0) {
        return;
    }
    for (int i = 0; i < (int)filesToOpen.size(); i++) {
        if (!validFileFormat(filesToOpen[i].toStdString())) {
            continue;
        }

        QMdiSubWindow* existing = findMdiWindow(filesToOpen[i].toStdString());
        if (existing) {
            mdiArea->setActiveSubWindow(existing);
            continue;
        }

        //The docIndex doesn't need increased as it is only used for unnamed files
        numOfDocs++;
        MdiWindow* mdiWin = new MdiWindow(docIndex, mdiArea, Qt::SubWindow);
        connect(mdiWin, SIGNAL(sendCloseMdiWin(MdiWindow*)), this, SLOT(onCloseMdiWin(MdiWindow*)));
        connect(mdiArea, SIGNAL(subWindowActivated(QMdiSubWindow*)), this, SLOT(onWindowActivated(QMdiSubWindow*)));

        //Make sure the toolbars/etc... are shown before doing their zoomExtents
        if (doOnce) { updateMenuToolbarStatusbar(); doOnce = false; }

        if (mdiWin->loadFile(filesToOpen[i].toStdString())) {
            statusbar->showMessage(translate_str("File(s) loaded"), 2000);
            mdiWin->show();
            mdiWin->showMaximized();
            //Prevent duplicate entries in the recent files list
            /* \todo fix this
            std::vector<std::string> list = tokenize(settings[ST_RECENT_FILES].s, ' ');
            if (!list.contains(filesToOpen[i], Qt::CaseInsensitive)) {
                list.prepend(filesToOpen[i]);
            }
            //Move the recent file to the top of the list
            else {
                list.removeAll(filesToOpen[i]);
                list.prepend(filesToOpen[i]));
            }
            settings[ST_RECENT_FILES] = list;
            */
            strcpy(settings[ST_RECENT_DIRECTORY].s,
                QFileInfo(filesToOpen[i]).absolutePath().toStdString().c_str());

            if (mdiWin->gview) {
                mdiWin->gview->recalculateLimits();
                mdiWin->gview->zoomExtents();
            }
        }
        else {
            mdiWin->close();
        }
    }

    windowMenuAboutToShow();
}

/* MainWindow::openrecentfile
 */
void
MainWindow::openrecentfile()
{
    debug_message("MainWindow::openrecentfile()");

    //Check to see if this from the recent files list
    QAction* recentSender = qobject_cast<QAction*>(sender());
    if (recentSender) {
        openFile(true, recentSender->data().toString().toStdString());
    }
}

/* MainWindow::savefile
 */
void
MainWindow::savefile()
{
    debug_message("MainWindow::savefile()");
}

/* MainWindow::saveasfile
 */
void
MainWindow::saveasfile()
{
    debug_message("MainWindow::saveasfile()");
    // need to find the activeSubWindow before it loses focus to the FileDialog
    MdiWindow* mdiWin = qobject_cast<MdiWindow*>(mdiArea->activeSubWindow());
    if (!mdiWin) {
        return;
    }

    QString openFilesPath = QString::fromStdString(settings[ST_RECENT_DIRECTORY].s);
    QString file = QFileDialog::getSaveFileName(this, translate_str("Save As"), openFilesPath, formatFilterSave);

    mdiWin->saveFile(file.toStdString());
}

/* MainWindow::findMdiWindow
 * fileName
 */
QMdiSubWindow *
MainWindow::findMdiWindow(std::string fileName)
{
    debug_message("MainWindow::findMdiWindow(" + fileName + ")");
    QString canonicalFilePath = QFileInfo(QString::fromStdString(fileName)).canonicalFilePath();

    foreach(QMdiSubWindow* subWindow, mdiArea->subWindowList()) {
        MdiWindow* mdiWin = qobject_cast<MdiWindow*>(subWindow);
        if (mdiWin) {
            if (mdiWin->curFile == canonicalFilePath) {
                return subWindow;
            }
        }
    }
    return 0;
}

/* MainWindow::closeEvent
 * event
 */
void
MainWindow::closeEvent(QCloseEvent* event)
{
    mdiArea->closeAllSubWindows();
    write_settings();
    event->accept();
}

/* MainWindow::onCloseWindow
 */
void
MainWindow::onCloseWindow()
{
    debug_message("MainWindow::onCloseWindow()");
    MdiWindow* mdiWin = qobject_cast<MdiWindow*>(mdiArea->activeSubWindow());
    if (mdiWin) {
        onCloseMdiWin(mdiWin);
    }
}

/* MainWindow::onCloseMdiWin
 * theMdiWin
 */
void
MainWindow::onCloseMdiWin(MdiWindow* theMdiWin)
{
    debug_message("MainWindow::onCloseMdiWin()");
    numOfDocs--;

    bool keepMaximized;
    if (theMdiWin) { keepMaximized = theMdiWin->isMaximized(); }

    mdiArea->removeSubWindow(theMdiWin);
    theMdiWin->deleteLater();

    updateMenuToolbarStatusbar();
    windowMenuAboutToShow();

    if (keepMaximized) {
        MdiWindow* mdiWin = qobject_cast<MdiWindow*>(mdiArea->activeSubWindow());
        if (mdiWin) {
            mdiWin->showMaximized();
        }
    }
}

/* MainWindow::onWindowActivated
 * w
 */
void
MainWindow::onWindowActivated(QMdiSubWindow* w)
{
    debug_message("MainWindow::onWindowActivated()");
    MdiWindow* mdiWin = qobject_cast<MdiWindow*>(w);
    if (mdiWin) {
        mdiWin->onWindowActivated();
    }
}

/* MainWindow::resizeEvent
 * e
 */
void
MainWindow::resizeEvent(QResizeEvent* e)
{
    debug_message("MainWindow::resizeEvent()");
    QMainWindow::resizeEvent(e);
    statusBar()->setSizeGripEnabled(!isMaximized());
}

/* MainWindow::updateMenuToolbarStatusbar
 */
void
MainWindow::updateMenuToolbarStatusbar()
{
    debug_message("MainWindow::updateMenuToolbarStatusbar()");

    actionHash[ACTION_PRINT]->setEnabled(numOfDocs > 0);
    actionHash[ACTION_WINDOW_CLOSE]->setEnabled(numOfDocs > 0);
    actionHash[ACTION_DESIGN_DETAILS]->setEnabled(numOfDocs > 0);

    if (numOfDocs) {
        for (int i=0; i<TOTAL_TOOLBARS; i++) {
            toolbarHash[i]->show();
        }

        //DockWidgets
        dockPropEdit->show();
        dockUndoEdit->show();

        //Menus
        menuBar()->clear();
        for (int i=0; menubar_order[i] >= 0; i++) {
            menuBar()->addMenu(menuHash[menubar_order[i]]);
        }

        menuHash[MENU_WINDOW]->setEnabled(true);

        //Statusbar
        statusbar->clearMessage();
        statusbar->statusBarMouseCoord->show();
        int n = string_array_length(button_list);
        for (int i=0; i<n; i++) {
            statusbar->buttons[button_list[i]]->show();
        }
    }
    else {
        for (int i=0; i<TOTAL_TOOLBARS; i++) {
            toolbarHash[i]->hide();
        }

        toolbarHash[TOOLBAR_FILE]->show();
        toolbarHash[TOOLBAR_EDIT]->show();

        //DockWidgets
        dockPropEdit->hide();
        dockUndoEdit->hide();

        //Menus
        menuBar()->clear();
        menuBar()->addMenu(menuHash[MENU_FILE]);
        menuBar()->addMenu(menuHash[MENU_EDIT]);
        menuBar()->addMenu(menuHash[MENU_SETTINGS]);
        menuBar()->addMenu(menuHash[MENU_WINDOW]);
        menuBar()->addMenu(menuHash[MENU_HELP]);

        menuHash[MENU_WINDOW]->setEnabled(false);

        //Statusbar
        statusbar->clearMessage();
        statusbar->statusBarMouseCoord->hide();
        int n = string_array_length(button_list);
        for (int i=0; i<n; i++) {
            statusbar->buttons[button_list[i]]->hide();
        }
    }
    /* TODO: decide why this call was here */
    debug_message("MainWindow::hideUnimplemented()");
}

/* MainWindow::validFileFormat
 * fileName
 *
 * TODO: check the file exists on the system, rename to validFile?
 */
bool
validFileFormat(std::string fileName)
{
    if (fileName == "") {
        return false;
    }
    if (emb_identify_format(fileName.c_str()) >= 0) {
        return true;
    }
    return false;
}

/* MainWindow::loadFormats
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
    QString custom = settings.custom_filter;
    if (custom.contains("supported", Qt::CaseInsensitive)) {
        custom = ""; //This will hide it
    }
    else if (!custom.contains("*", Qt::CaseInsensitive)) {
        custom = ""; //This will hide it
    }
    else {
        custom = "Custom Filter(" + custom + ");;";
    }

    return tr(qPrintable(custom + supported + all));
    */
}

/* MainWindow::closeToolBar
 * action
 */
void
MainWindow::closeToolBar(QAction* action)
{
    if (action->objectName() == "toolbarclose") {
        QToolBar* tb = qobject_cast<QToolBar*>(sender());
        if (tb) {
            debug_message(tb->objectName().toStdString() + "%s closed.");
            tb->hide();
        }
    }
}

/* MainWindow::floatingChangedToolBar
 * isFloating
 */
void
MainWindow::floatingChangedToolBar(bool isFloating)
{
    QToolBar* tb = qobject_cast<QToolBar*>(sender());
    if (!tb) {
        return;
    }
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
        for (int i = 0; i < actList.size(); ++i) {
            QAction* ACTION = actList.value(i);
            if (ACTION->objectName() == "toolbarclose") {
                tb->removeAction(ACTION);
                disconnect(tb, SIGNAL(actionTriggered(QAction*)), this, SLOT(closeToolBar(QAction*)));
                delete ACTION;
            }
        }
    }
}


/* . */
UndoableCommand::UndoableCommand(std::string command_, QString text, Geometry* obj, View* v, QUndoCommand* parent) : QUndoCommand(parent)
{
    gview = v;
    object = obj;
    command = command_;
    setText(text);
}

/* . */
UndoableCommand::UndoableCommand(EmbVector delta_, QString text, Geometry* obj, View* v, QUndoCommand* parent) : QUndoCommand(parent)
{
    gview = v;
    object = obj;
    command = "move";
    setText(text);
    delta = delta_;
}

/* . */
UndoableCommand::UndoableCommand(
    std::string command, EmbVector point, EmbReal value, QString text, Geometry* obj, View* v, QUndoCommand* parent) : QUndoCommand(parent)
{
    gview = v;
    object = obj;
    setText(text);
    command = command;
    if (command == "scale") {
        pivot = point;
        angle = value;
    }
    else {
        //Prevent division by zero and other wacky behavior
        if (value <= 0.0) {
            delta.x = 0.0;
            delta.y = 0.0;
            factor = 1.0;
            QMessageBox::critical(0,
                QObject::tr("ScaleFactor Error"),
                QObject::tr("Hi there. If you are not a developer, report this as a bug. "
            "If you are a developer, your code needs examined, and possibly your head too."));
        }
        else {
            //Calculate the offset
            EmbVector old;
            old.x = object->x();
            old.y = object->y();
            factor = value;
            QLineF scaleLine(point.x, point.y, old.x, old.y);
            scaleLine.setLength(scaleLine.length() * factor);
            EmbVector new_;
            new_.x = scaleLine.x2();
            new_.y = scaleLine.y2();

            delta = embVector_subtract(new_, old);
        }
    }
}

/* . */
void
UndoableCommand::undo()
{
    if (command == "add") {
        gview->deleteObject(object);
    }
    else if (command == "delete") {
        gview->addObject(object);
    }
    else if (command == "move") {
        object->moveBy(-delta.x, -delta.y);
    }
    else if (command == "rotate") {
        rotate(pivot, -angle);
    }
    else if (command == "scale") {
        object->setScale(object->scale()*(1/factor));
        object->moveBy(-delta.x, -delta.y);
    }
    else if (command == "gripedit") {
        object->gripEdit(after, before);
    }
    else if (command == "mirror") {
        mirror();
    }
    else if (command == "nav") {
        if (!done) {
            toTransform = gview->transform();
            toCenter = gview->center();
        }
        done = true;

        gview->setTransform(fromTransform);
        gview->centerAt(fromCenter);
    }
}

/* . */
void
UndoableCommand::redo()
{
    if (command == "add") {
        gview->addObject(object);
    }
    else if (command == "delete") {
        gview->deleteObject(object);
    }
    else if (command == "move") {
        object->moveBy(delta.x, delta.y);
    }
    else if (command == "rotate") {
        rotate(pivot, angle);
    }
    else if (command == "scale") {
        object->setScale(object->scale()*factor);
        object->moveBy(delta.x, delta.y);
    }
    else if (command == "gripedit") {
        object->gripEdit(before, after);
    }
    else if (command == "mirror") {
        mirror();
    }
    else if (command == "nav") {
        if (!done) {
            if (navType == "ZoomInToPoint")  {
                gview->zoomToPoint(gview->scene()->property("VIEW_MOUSE_POINT").toPoint(), +1);
            }
            else if (navType == "ZoomOutToPoint") {
                gview->zoomToPoint(gview->scene()->property("VIEW_MOUSE_POINT").toPoint(), -1);
            }
            else if (navType == "ZoomExtents") {
                gview->zoomExtents();
            }
            else if (navType == "ZoomSelected") {
                gview->zoomSelected();
            }
            else if (navType == "PanStart") {
                /* Do Nothing. We are just recording the spot where the pan started. */
            }
            else if (navType == "PanStop") {
                /* Do Nothing. We are just recording the spot where the pan stopped. */
            }
            else if (navType == "PanLeft") {
                gview->panLeft();
            }
            else if (navType == "PanRight") {
                gview->panRight();
            }
            else if (navType == "PanUp") {
                gview->panUp();
            }
            else if (navType == "PanDown") {
                gview->panDown();
            }
            toTransform = gview->transform();
            toCenter = gview->center();
        }
        else {
            gview->setTransform(toTransform);
            gview->centerAt(toCenter);
        }
    }
}

/* . */
void
UndoableCommand::rotate(EmbVector pivot, EmbReal rot)
{
    EmbReal rad = radians(rot);
    EmbVector p = embVector_subtract(to_EmbVector(object->scenePos()), pivot);
    EmbVector rotv = embVector_add(rotate_vector(p, rad), pivot);

    object->setPos(rotv.x, rotv.y);
    object->setRotation(object->rotation() + rot);
}

/* . */
UndoableCommand::UndoableCommand(QString type, View* v, QUndoCommand* parent) : QUndoCommand(parent)
{
    gview = v;
    navType = type;
    setText(QObject::tr("Navigation"));
    command = "nav";
    done = false;
    fromTransform = gview->transform();
    fromCenter = gview->center();
}

/* . */
bool
UndoableCommand::mergeWith(const QUndoCommand* newest)
{
    if (newest->id() != id()) // make sure other is also an UndoableNavCommand
         return false;

    const UndoableCommand* cmd = static_cast<const UndoableCommand*>(newest);
    toTransform = cmd->toTransform;
    toCenter = cmd->toCenter;

    return true;
}

/* . */
UndoableCommand::UndoableCommand(const QPointF beforePoint, const QPointF afterPoint, QString  text, Geometry* obj, View* v, QUndoCommand* parent) : QUndoCommand(parent)
{
    gview = v;
    object = obj;
    setText(text);
    command = "gripedit";
    before = beforePoint;
    after = afterPoint;
}

/* . */
UndoableCommand::UndoableCommand(EmbReal x1, EmbReal y1, EmbReal x2, EmbReal y2, QString  text, Geometry* obj, View* v, QUndoCommand* parent) : QUndoCommand(parent)
{
    gview = v;
    object = obj;
    setText(text);
    command = "mirror";
    mirrorLine = QLineF(x1, y1, x2, y2);
}

/* . */
void
UndoableCommand::mirror()
{
    //TODO: finish undoable mirror
}
