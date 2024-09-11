/*
 * Embroidermodder 2.
 *
 * Copyright 2011-2024 The Embroidermodder Team
 * Embroidermodder 2 is Open Source Software, see LICENSE.md for licensing terms.
 *
 * Read the reference manual (https://www.libembroidery.org/downloads/emrm.pdf)
 * for advice on altering this file.
 *
 * MainWindow
 */

#include "embroidermodder.h"

std::unordered_map<int, QAction*> actionHash;
QToolBar* toolbar[N_TOOLBARS];
QMenu* menu[N_MENUS];
const char *settings_file = "settings.toml";

QList<QGraphicsItem*> cutCopyObjectList;

QString formatFilterOpen;
QString formatFilterSave;

QStatusBar* statusbar;
MdiArea* mdiArea;
CmdPrompt* prompt;
PropertyEditor* dockPropEdit;
UndoEditor* dockUndoEdit;
QTimer* testing_timer;

QList<MdiWindow*> listMdiWin;
QString openFilesPath;

QAction* myFileSeparator;

QWizard* wizardTipOfTheDay;
QLabel* labelTipOfTheDay;
QCheckBox* checkBoxTipOfTheDay;

/* Selectors */
QComboBox* layerSelector;
QComboBox* colorSelector;
QComboBox* linetypeSelector;
QComboBox* lineweightSelector;
QFontComboBox* textFontSelector;
QComboBox* textSizeSelector;

QByteArray layoutState;

MainWindow *_main;

StringMap aliases[MAX_ALIASES];
std::unordered_map<std::string, std::string> aliasHash;

QString curText;
QString defaultPrefix;
QString prefix;

QString lastCmd;
QString curCmd;

QString prompt_color_;
QString prompt_selection_bg_color_;
QString prompt_bg_color_;
QString prompt_selection_color_;

QTextBrowser* promptHistory;
CmdPromptInput* promptInput;

QTimer* blinkTimer;
bool blinkState;
/* NOTE: These shortcuts need to be caught since QLineEdit uses them. */

std::unordered_map<int, int> key_map = {
    {QKeySequence::Cut, CUT_SEQUENCE},
    {QKeySequence::Copy, COPY_SEQUENCE},
    {QKeySequence::Paste, PASTE_SEQUENCE},
    {QKeySequence::SelectAll, SELECT_ALL_SEQUENCE},
    {QKeySequence::Undo, UNDO_SEQUENCE},
    {QKeySequence::Redo, REDO_SEQUENCE},
    {Qt::Key_Delete, DELETE_KEY},
    {Qt::Key_Tab, TAB_KEY},
    {Qt::Key_Escape, ESCAPE_KEY},
    {Qt::Key_Up, UP_KEY},
    {Qt::Key_Down, DOWN_KEY},
    {Qt::Key_F1, F1_KEY},
    {Qt::Key_F2, F2_KEY},
    {Qt::Key_F3, F3_KEY},
    {Qt::Key_F4, F4_KEY},
    {Qt::Key_F5, F5_KEY},
    {Qt::Key_F6, F6_KEY},
    {Qt::Key_F7, F7_KEY},
    {Qt::Key_F8, F8_KEY},
    {Qt::Key_F9, F9_KEY},
    {Qt::Key_F10, F10_KEY},
    {Qt::Key_F11, F11_KEY},
    {Qt::Key_F12, F12_KEY},
    {Qt::Key_Shift, SHIFT_KEY}
};

QToolButton* statusBarSnapButton;
QToolButton* statusBarGridButton;
QToolButton* statusBarRulerButton;
QToolButton* statusBarOrthoButton;
QToolButton* statusBarPolarButton;
QToolButton* statusBarQSnapButton;
QToolButton* statusBarQTrackButton;
QToolButton* statusBarLwtButton;
QLabel* statusBarMouseCoord;

QStringList button_list = {
    "SNAP",
    "GRID",
    "RULER",
    "ORTHO",
    "POLAR",
    "QSNAP",
    "QTRACK",
    "LWT"
};
/* . */
EmbVector
to_emb_vector(QPointF p)
{
    EmbVector v;
    v.x = p.x();
    v.y = p.y();
    return v;
}

/* . */
QPointF
to_qpointf(EmbVector v)
{
    QPointF p(v.x, v.y);
    return p;
}

/* . */
EmbVector
scale_and_rotate(EmbVector v, double scale, double angle)
{
    EmbVector w;
    double rot = radians(angle);
    double cosRot = cos(rot);
    double sinRot = sin(rot);
    w.x = v.x * scale;
    w.y = v.y * scale;
    w.x = w.x * cosRot - w.y * sinRot;
    w.y = w.x * sinRot + w.y * cosRot;
    return w;    
}

/* . */
EmbVector
find_mouse_snap_point(QList<EmbVector> snap_points, EmbVector mouse_point)
{
    float closest = 1.0e10;
    EmbVector result = snap_points[0];
    int i;
    for (i=0; i<snap_points.count(); i++) {
        float distance = emb_vector_distance(snap_points[i], mouse_point);
        if (distance < closest) {
            closest = distance;
            result = snap_points[i];
        }
    }
    return result;
}

/* . */
EmbArc
emb_arc_set_radius(EmbArc arc, EmbReal radius)
{
    EmbGeometry geometry;
    geometry.object.arc = arc;
    radius = EMB_MAX(radius, 0.0000001);
    EmbVector center = emb_arc_center(geometry);

    EmbVector start = emb_vector_subtract(center, arc.start);
    start = emb_vector_scale(start, radius/emb_vector_length(start));
    arc.start = emb_vector_add(center, start);

    EmbVector mid = emb_vector_subtract(center, arc.mid);
    mid = emb_vector_scale(mid, radius/emb_vector_length(mid));
    arc.mid = emb_vector_add(center, mid);

    EmbVector end = emb_vector_subtract(center, arc.end);
    end = emb_vector_scale(start, radius/emb_vector_length(end));
    arc.end = emb_vector_add(center, end);

    return arc;
}

/* . */
QPixmap
create_pixmap(QString icon)
{
    int id = 0;
    for (int i=0; ; i++) {
        if (!xpm_icon_labels[i]) {
            break;
        }
        if (!strcmp(qPrintable(icon), xpm_icon_labels[i])) {
            id = i;
            break;
        }
    }
    QPixmap pixmap(xpm_icons[id]);
    return pixmap;
}

/* . */
QIcon
create_icon(QString icon)
{
    return QIcon(create_pixmap(icon));
}

/* . */
QIcon
create_swatch(int32_t color)
{
    QPixmap pixmap(16, 16);
    pixmap.fill(QColor(color));
    return QIcon(pixmap); 
}


/* . */
void
wait_cursor(void)
{
    QApplication::setOverrideCursor(Qt::WaitCursor);
}

/* . */
void
arrow_cursor(void)
{
    QApplication::setOverrideCursor(Qt::ArrowCursor);
}

/* . */
void
restore_cursor(void)
{
    QApplication::restoreOverrideCursor();
}

QMenuBar *menuBar()
{
    return _main->menuBar();
}

Application::Application(int argc, char **argv) : QApplication(argc, argv), _mainWin(NULL)
{
}

bool
Application::event(QEvent *event)
{
    switch (event->type()) {
    case QEvent::FileOpen:
        if (_main) {
            openFilesSelected(QStringList(static_cast<QFileOpenEvent *>(event)->file()));
            return true;
        }
        /* Fall through */
    default:
        return QApplication::event(event);
    }
}

int
make_application(int argc, char* argv[])
{
#if defined(Q_OS_MAC)
    Application app(argc, argv);
#else
    QApplication app(argc, argv);
#endif
    app.setApplicationName(_appName_);
    app.setApplicationVersion(_appVer_);

    QStringList filesToOpen;
    for (int i=0; i<argc; i++) {
        filesToOpen << QString(argv[i]);
    }
    
    _main = new MainWindow();
#if defined(Q_OS_MAC)
    app.setMainWin(_main);
#endif

    QObject::connect(&app, SIGNAL(lastWindowClosed()), _main, SLOT(quit()));

    _main->setWindowTitle(app.applicationName() + " " + app.applicationVersion());
    _main->show();

    /* NOTE: If openFilesSelected() is called from within the _main constructor,
     * slot commands wont work and the window menu will be screwed
     */
    if (!filesToOpen.isEmpty()) {
        openFilesSelected(filesToOpen);
    }

    return app.exec();
}

void
nanosleep(int time)
{
    std::this_thread::sleep_for(std::chrono::milliseconds(time));
}

void
run_testing(void)
{
    int i;
    nanosleep(2000);
    int n = string_array_length(coverage_test);
    for (i=0; i<n; i++) {
        QString cmd(coverage_test[i]);
        runCommandMain(cmd);
        nanosleep(1000);
    }        
}

void
add_to_selector(QComboBox* box, char *list[], char *type, int use_icon)
{
    int n = string_array_length(list) / 3;
    for (int i=0; i<n; i++) {
        if (!use_icon) {
            if (!strcmp(type, "real")) {
                box->addItem(list[3*i+0], atof(list[3*i+1]));
                continue;
            }
            if (!strcmp(type, "int")) {
                box->addItem(list[3*i+0], atoi(list[3*i+1]));
            }
            continue;
        }
        if (!strcmp(type, "string")) {
            box->addItem(create_icon(list[3*i+0]), list[3*i+1]);
            continue;
        }
        if (!strcmp(type, "int")) {
            if (strlen(list[3*i+2]) > 0) {
                box->addItem(create_icon(list[3*i+0]), list[3*i+1],
                    atoi(list[3*i+2]));
            }
            else {
                box->addItem(create_icon(list[3*i+0]), list[3*i+1]);
            }
            continue;
        }
        if (!strcmp(type, "real")) {
            if (strlen(list[3*i+2]) > 0) {
                box->addItem(create_icon(list[3*i+0]), list[3*i+1],
                    atof(list[3*i+2]));
            }
            else {
                box->addItem(create_icon(list[3*i+0]), list[3*i+1]);
            }
        }
    }
}

MainWindow::MainWindow() : QMainWindow(0)
{
    readSettings();

    QString lang = get_str(GENERAL_LANGUAGE);
    debug_message(qPrintable("language: " + lang));
    if (lang == "system") {
        lang = QLocale::system().languageToString(QLocale::system().language()).toLower();
    }

    /* Init */
    _main = this;

    /* Menus */
    for (int i=0; i<N_MENUS; i++) {
        menu[i] = new QMenu(translate(menu_list[i]), this);
    }

    /* Toolbars */
    for (int i=0; i<N_TOOLBARS; i++) {
        toolbar[i] = new QToolBar(translate(toolbar_list[i]), this);
    }

    /* Selectors */
    layerSelector = new QComboBox(this);
    colorSelector = new QComboBox(this);
    linetypeSelector = new QComboBox(this);
    lineweightSelector = new QComboBox(this);
    textFontSelector = new QFontComboBox(this);
    textSizeSelector = new QComboBox(this);

    numOfDocs = 0;
    docIndex = 0;

    shiftKeyPressedState = false;

    setWindowIcon(create_icon("app"));
    setMinimumSize(800, 480); /* Require Minimum WVGA */

    loadFormats();

    /* create the mdiArea */
    QFrame* vbox = new QFrame(this);
    QVBoxLayout* layout = new QVBoxLayout(vbox);
    /* layout->setMargin(0); */
    vbox->setFrameStyle(QFrame::StyledPanel | QFrame::Sunken);
    mdiArea = new MdiArea(this, vbox);
    mdiArea->useBackgroundLogo(get_bool(GENERAL_MDI_BG_USE_LOGO));
    mdiArea->useBackgroundTexture(get_bool(GENERAL_MDI_BG_USE_TEXTURE));
    mdiArea->useBackgroundColor(get_bool(GENERAL_MDI_BG_USE_COLOR));
    mdiArea->setBackgroundLogo(get_str(GENERAL_MDI_BG_LOGO));
    mdiArea->setBackgroundTexture(get_str(GENERAL_MDI_BG_TEXTURE));
    mdiArea->setBackgroundColor(QColor(get_int(GENERAL_MDI_BG_COLOR)));
    mdiArea->setViewMode(QMdiArea::TabbedView);
    mdiArea->setHorizontalScrollBarPolicy(Qt::ScrollBarAsNeeded);
    mdiArea->setVerticalScrollBarPolicy(Qt::ScrollBarAsNeeded);
    mdiArea->setActivationOrder(QMdiArea::ActivationHistoryOrder);

    /*
    ExperimentalView *openGL = new ExperimentalView(this);
    QLabel *example = new QLabel(translate("OpenGL"));
    layout->addWidget(openGL);
    */

    layout->addWidget(mdiArea);
    setCentralWidget(vbox);

    /* create the Command Prompt */
    prompt = new CmdPrompt(this);
    prompt->setFocus(Qt::OtherFocusReason);
    this->setFocusProxy(prompt);
    mdiArea->setFocusProxy(prompt);

    setPromptTextColor(QColor(get_int(PROMPT_TEXT_COLOR)));
    setPromptBackgroundColor(QColor(get_int(PROMPT_BG_COLOR)));

    connect(prompt, SIGNAL(startCommand(QString)), this, SLOT(logPromptInput(QString)));

    connect(prompt, SIGNAL(startCommand(QString)), this, SLOT(runCommandMain(QString)));
    connect(prompt, SIGNAL(runCommand(QString, QString)), this, SLOT(runCommandPrompt(QString, QString)));

    connect(prompt, SIGNAL(deletePressed()), this, SLOT(deletePressed()));
    /* TODO: connect(prompt, SIGNAL(tabPressed()), this, SLOT(someUnknownSlot())); */
    connect(prompt, SIGNAL(escapePressed()), this, SLOT(escapePressed()));
    connect(prompt, SIGNAL(upPressed()), this, SLOT(promptInputPrevious()));
    connect(prompt, SIGNAL(downPressed()), this, SLOT(promptInputNext()));
    connect(prompt, SIGNAL(F1Pressed()), this, SLOT(help()));
    /* TODO: connect(prompt, SIGNAL(F2Pressed()), this, SLOT(floatHistory())); */
    /* TODO: connect(prompt, SIGNAL(F3Pressed()), this, SLOT(toggleQSNAP())); */
    connect(prompt, SIGNAL(F4Pressed()), this, SLOT(toggleLwt())); /* TODO: typically this is toggleTablet(), make F-Keys customizable thru settings */
    /* TODO: connect(prompt, SIGNAL(F5Pressed()), this, SLOT(toggleISO())); */
    /* TODO: connect(prompt, SIGNAL(F6Pressed()), this, SLOT(toggleCoords())); */
    connect(prompt, SIGNAL(F7Pressed()), this, SLOT(toggleGrid()));
    /* TODO: connect(prompt, SIGNAL(F8Pressed()), this, SLOT(toggleORTHO())); */
    /* TODO: connect(prompt, SIGNAL(F9Pressed()), this, SLOT(toggleSNAP())); */
    /* TODO: connect(prompt, SIGNAL(F10Pressed()), this, SLOT(togglePOLAR())); */
    /* TODO: connect(prompt, SIGNAL(F11Pressed()), this, SLOT(toggleQTRACK())); */
    connect(prompt, SIGNAL(F12Pressed()), this, SLOT(toggleRuler()));
    connect(prompt, SIGNAL(cutPressed()), this, SLOT(cut()));
    connect(prompt, SIGNAL(copyPressed()), this, SLOT(copy()));
    connect(prompt, SIGNAL(pastePressed()), this, SLOT(paste()));
    connect(prompt, SIGNAL(selectAllPressed()), this, SLOT(selectAll()));
    connect(prompt, SIGNAL(undoPressed()), this, SLOT(undo()));
    connect(prompt, SIGNAL(redoPressed()), this, SLOT(redo()));

    connect(prompt, SIGNAL(shiftPressed()), this, SLOT(setShiftPressed()));
    connect(prompt, SIGNAL(shiftReleased()), this, SLOT(setShiftReleased()));

    connect(prompt, SIGNAL(showSettings()), this, SLOT(settingsPrompt()));

    connect(prompt, SIGNAL(historyAppended(QString)), this, SLOT(promptHistoryAppended(QString)));

    /* create the Object Property Editor */
    dockPropEdit = new PropertyEditor(
        get_str(GENERAL_ICON_THEME),
        get_bool(SELECTION_MODE_PICKADD),
        prompt,
        this);
    addDockWidget(Qt::LeftDockWidgetArea, dockPropEdit);
    connect(dockPropEdit, SIGNAL(pickAddModeToggled()), this, SLOT(pickAddModeToggled()));

    /* create the Command History Undo Editor */
    dockUndoEdit = new UndoEditor(get_str(GENERAL_ICON_THEME), prompt, this);
    addDockWidget(Qt::LeftDockWidgetArea, dockUndoEdit);

    /* setDockOptions(QMainWindow::AnimatedDocks | QMainWindow::AllowTabbedDocks | QMainWindow::VerticalTabs); */ /* TODO: Load these from settings */
    /* tabifyDockWidget(dockPropEdit, dockUndoEdit); */ /* TODO: load this from settings */

    create_statusbar(this);
    this->setStatusBar(statusbar);

    createAllActions();
    createAllMenus();

    debug_message("createAllToolbars()");

    add_to_toolbar(TOOLBAR_FILE, file_toolbar);
    add_to_toolbar(TOOLBAR_EDIT, edit_toolbar);
    add_to_toolbar(TOOLBAR_ZOOM, zoom_toolbar);
    add_to_toolbar(TOOLBAR_PAN, pan_toolbar);
    add_to_toolbar(TOOLBAR_VIEW, view_toolbar);
    add_to_toolbar(TOOLBAR_ICON, icon_toolbar);
    add_to_toolbar(TOOLBAR_HELP, help_toolbar);

    debug_message("createLayerToolbar()");

    toolbar[TOOLBAR_LAYER]->setObjectName("toolbarLayer");
    toolbar[TOOLBAR_LAYER]->addAction(actionHash[ACTION_MAKE_LAYER_CURRENT]);
    toolbar[TOOLBAR_LAYER]->addAction(actionHash[ACTION_LAYERS]);

    QString icontheme = get_str(GENERAL_ICON_THEME);

    layerSelector->setFocusProxy(prompt);
    /* TODO: Create layer pixmaps by concatenating several icons. */
    add_to_selector(layerSelector, layer_list, "string", true);
    toolbar[TOOLBAR_LAYER]->addWidget(layerSelector);
    connect(layerSelector, SIGNAL(currentIndexChanged(int)), this, SLOT(layerSelectorIndexChanged(int)));

    toolbar[TOOLBAR_LAYER]->addAction(actionHash[ACTION_LAYER_PREVIOUS]);

    connect(toolbar[TOOLBAR_LAYER], SIGNAL(topLevelChanged(bool)), this, SLOT(floatingChangedToolBar(bool)));

    debug_message("createPropertiesToolbar()");

    toolbar[TOOLBAR_PROPERTIES]->setObjectName("toolbarProperties");

    colorSelector->setFocusProxy(prompt);
    colorSelector->addItem(create_icon("colorbylayer"), "ByLayer");
    colorSelector->addItem(create_icon("colorbyblock"), "ByBlock");
    colorSelector->addItem(create_icon("colorred"), translate("Red"), qRgb(255, 0, 0));
    colorSelector->addItem(create_icon("coloryellow"), translate("Yellow"), qRgb(255, 255, 0));
    colorSelector->addItem(create_icon("colorgreen"), translate("Green"), qRgb(  0,255, 0));
    colorSelector->addItem(create_icon("colorcyan"), translate("Cyan"), qRgb(  0,255,255));
    colorSelector->addItem(create_icon("colorblue"), translate("Blue"), qRgb(  0, 0,255));
    colorSelector->addItem(create_icon("colormagenta"), translate("Magenta"), qRgb(255, 0,255));
    colorSelector->addItem(create_icon("colorwhite"), translate("White"), qRgb(255,255,255));
    colorSelector->addItem(create_icon("colorother"), translate("Other..."));
    toolbar[TOOLBAR_PROPERTIES]->addWidget(colorSelector);
    connect(colorSelector, SIGNAL(currentIndexChanged(int)), this, SLOT(colorSelectorIndexChanged(int)));

    toolbar[TOOLBAR_PROPERTIES]->addSeparator();
    linetypeSelector->setFocusProxy(prompt);
    add_to_selector(linetypeSelector, line_type_list, "string", true);
    toolbar[TOOLBAR_PROPERTIES]->addWidget(linetypeSelector);
    connect(linetypeSelector, SIGNAL(currentIndexChanged(int)), this, SLOT(linetypeSelectorIndexChanged(int)));

    toolbar[TOOLBAR_PROPERTIES]->addSeparator();
    lineweightSelector->setFocusProxy(prompt);
    add_to_selector(lineweightSelector, line_weight_list, "real", true);
    /* Prevent dropdown text readability being squish...d. */
    lineweightSelector->setMinimumContentsLength(8);
    toolbar[TOOLBAR_PROPERTIES]->addWidget(lineweightSelector);
    connect(lineweightSelector, SIGNAL(currentIndexChanged(int)), this, SLOT(lineweightSelectorIndexChanged(int)));

    connect(toolbar[TOOLBAR_PROPERTIES], SIGNAL(topLevelChanged(bool)), this, SLOT(floatingChangedToolBar(bool)));

    debug_message("createTextToolbar()");

    toolbar[TOOLBAR_TEXT]->setObjectName("toolbarText");

    toolbar[TOOLBAR_TEXT]->addWidget(textFontSelector);
    textFontSelector->setCurrentFont(QFont(get_str(TEXT_FONT)));
    connect(textFontSelector, SIGNAL(currentFontChanged(const QFont&)), this, SLOT(textFontSelectorCurrentFontChanged(const QFont&)));

    toolbar[TOOLBAR_TEXT]->addAction(actionHash[ACTION_TEXT_BOLD]);
    actionHash[ACTION_TEXT_BOLD]->setChecked(get_bool(TEXT_STYLE_BOLD));
    toolbar[TOOLBAR_TEXT]->addAction(actionHash[ACTION_TEXT_ITALIC]);
    actionHash[ACTION_TEXT_ITALIC]->setChecked(get_bool(TEXT_STYLE_ITALIC));
    toolbar[TOOLBAR_TEXT]->addAction(actionHash[ACTION_TEXT_UNDERLINE]);
    actionHash[ACTION_TEXT_UNDERLINE]->setChecked(get_bool(TEXT_STYLE_UNDERLINE));
    toolbar[TOOLBAR_TEXT]->addAction(actionHash[ACTION_TEXT_STRIKEOUT]);
    actionHash[ACTION_TEXT_STRIKEOUT]->setChecked(get_bool(TEXT_STYLE_STRIKEOUT));
    toolbar[TOOLBAR_TEXT]->addAction(actionHash[ACTION_TEXT_OVERLINE]);
    actionHash[ACTION_TEXT_OVERLINE]->setChecked(get_bool(TEXT_STYLE_OVERLINE));

    textSizeSelector->setFocusProxy(prompt);
    add_to_selector(textSizeSelector, text_size_list, "int", false);
    setTextSize(get_int(TEXT_SIZE));
    toolbar[TOOLBAR_TEXT]->addWidget(textSizeSelector);
    connect(textSizeSelector, SIGNAL(currentIndexChanged(int)), this, SLOT(textSizeSelectorIndexChanged(int)));

    connect(toolbar[TOOLBAR_TEXT], SIGNAL(topLevelChanged(bool)), this, SLOT(floatingChangedToolBar(bool)));

    debug_message("createPromptToolbar()");

    toolbar[TOOLBAR_PROMPT]->setObjectName("toolbarPrompt");
    toolbar[TOOLBAR_PROMPT]->addWidget(prompt);
    toolbar[TOOLBAR_PROMPT]->setAllowedAreas(Qt::TopToolBarArea | Qt::BottomToolBarArea);
    connect(toolbar[TOOLBAR_PROMPT], SIGNAL(topLevelChanged(bool)), prompt, SLOT(floatingChanged(bool)));

    add_to_toolbar(TOOLBAR_DRAW, draw_toolbar);
    add_to_toolbar(TOOLBAR_MODIFY, modify_toolbar);

    /* Horizontal */
    toolbar[TOOLBAR_VIEW]->setOrientation(Qt::Horizontal);
    toolbar[TOOLBAR_ZOOM]->setOrientation(Qt::Horizontal);
    toolbar[TOOLBAR_LAYER]->setOrientation(Qt::Horizontal);
    toolbar[TOOLBAR_PROPERTIES]->setOrientation(Qt::Horizontal);
    toolbar[TOOLBAR_TEXT]->setOrientation(Qt::Horizontal);
    toolbar[TOOLBAR_PROMPT]->setOrientation(Qt::Horizontal);

    /* Top */
    addToolBarBreak(Qt::TopToolBarArea);
    addToolBar(Qt::TopToolBarArea, toolbar[TOOLBAR_FILE]);
    addToolBar(Qt::TopToolBarArea, toolbar[TOOLBAR_EDIT]);
    addToolBar(Qt::TopToolBarArea, toolbar[TOOLBAR_HELP]);
    addToolBar(Qt::TopToolBarArea, toolbar[TOOLBAR_ICON]);
    addToolBarBreak(Qt::TopToolBarArea);
    addToolBar(Qt::TopToolBarArea, toolbar[TOOLBAR_ZOOM]);
    addToolBar(Qt::TopToolBarArea, toolbar[TOOLBAR_PAN]);
    addToolBar(Qt::TopToolBarArea, toolbar[TOOLBAR_VIEW]);
    addToolBarBreak(Qt::TopToolBarArea);
    addToolBar(Qt::TopToolBarArea, toolbar[TOOLBAR_LAYER]);
    addToolBar(Qt::TopToolBarArea, toolbar[TOOLBAR_PROPERTIES]);
    addToolBarBreak(Qt::TopToolBarArea);
    addToolBar(Qt::TopToolBarArea, toolbar[TOOLBAR_TEXT]);
    /* Bottom */
    addToolBar(Qt::BottomToolBarArea, toolbar[TOOLBAR_PROMPT]);

    /* Left */
    addToolBar(Qt::LeftToolBarArea, toolbar[TOOLBAR_DRAW]);
    addToolBar(Qt::LeftToolBarArea, toolbar[TOOLBAR_MODIFY]);
    addToolBarBreak(Qt::LeftToolBarArea);

    /* zoomToolBar->setToolButtonStyle(Qt::ToolButtonTextOnly); */

    iconResize(get_int(GENERAL_ICON_SIZE));
    update_interface();

    /* Show date in statusbar after it has been updated. */
    QDate date = QDate::currentDate();
    QString datestr = date.toString("MMMM d, yyyy");
    statusbar->showMessage(datestr);

    showNormal();

    toolbar[TOOLBAR_PROMPT]->show();

    if (get_bool(GENERAL_TIP_OF_THE_DAY) && (!testing_mode)) {
        tipOfTheDay();
    }

    debug_message("Finished creating window.");
}

MainWindow::~MainWindow()
{
    debug_message("Destructor()");

    /* Prevent memory leaks by deleting any unpasted objects. */
    qDeleteAll(cutCopyObjectList.begin(), cutCopyObjectList.end());
    cutCopyObjectList.clear();
}

/* . */
void
MainWindow::recentMenuAboutToShow(void)
{
    debug_message("recentMenuAboutToShow()");
    menu[MENU_RECENT]->clear();

    QFileInfo recentFileInfo;
    QString recentValue;
    for (int i = 0; ; ++i) {
        if (!strcmp(recent_files[i], "END")) {
            break;
        }
        /* If less than the max amount of entries add to menu. */
        if (i < get_int(OPENSAVE_RECENT_MAX_FILES)) {
            recentFileInfo = QFileInfo(recent_files[i]);
            if (recentFileInfo.exists() && validFileFormat(recentFileInfo.fileName())) {
                recentValue.setNum(i+1);
                QAction* rAction;
                if (recentValue.toInt() >= 1 && recentValue.toInt() <= 9) {
                    rAction = new QAction("&" + recentValue + " " + recentFileInfo.fileName(), _main);
                }
                else if (recentValue.toInt() == 10) {
                    rAction = new QAction("1&0 " + recentFileInfo.fileName(), _main);
                }
                else {
                    rAction = new QAction(recentValue + " " + recentFileInfo.fileName(), _main);
                }
                rAction->setCheckable(false);
                QString fname(recent_files[i]);
                rAction->setData(fname);
                menu[MENU_RECENT]->addAction(rAction);
                QObject::connect(rAction, SIGNAL(triggered()), _main,
                    SLOT(openrecentfile()));
            }
        }
        else {
            break;
        }
    }
    /* Ensure the list only has max amount of entries. */
    if (get_int(OPENSAVE_RECENT_MAX_FILES) < MAX_FILES) {
        strcpy(recent_files[get_int(OPENSAVE_RECENT_MAX_FILES)], "END");
    }
    else {
        set_int(OPENSAVE_RECENT_MAX_FILES, MAX_FILES - 1);
        strcpy(recent_files[get_int(OPENSAVE_RECENT_MAX_FILES)], "END");
    }
}

/* . */
void
windowMenuAboutToShow(void)
{
    debug_message("windowMenuAboutToShow()");
    menu[MENU_WINDOW]->clear();
    menu[MENU_WINDOW]->addAction(actionHash[ACTION_WINDOW_CLOSE]);
    menu[MENU_WINDOW]->addAction(actionHash[ACTION_WINDOW_CLOSE_ALL]);
    menu[MENU_WINDOW]->addSeparator();
    menu[MENU_WINDOW]->addAction(actionHash[ACTION_WINDOW_CASCADE]);
    menu[MENU_WINDOW]->addAction(actionHash[ACTION_WINDOW_TILE]);
    menu[MENU_WINDOW]->addSeparator();
    menu[MENU_WINDOW]->addAction(actionHash[ACTION_WINDOW_NEXT]);
    menu[MENU_WINDOW]->addAction(actionHash[ACTION_WINDOW_PREVIOUS]);

    menu[MENU_WINDOW]->addSeparator();
    QList<QMdiSubWindow*> windows = mdiArea->subWindowList();
    for (int i = 0; i < windows.count(); ++i) {
        QAction* aAction = new QAction(windows.at(i)->windowTitle(), _main);
        aAction->setCheckable(true);
        aAction->setData(i);
        menu[MENU_WINDOW]->addAction(aAction);
        QObject::connect(aAction, SIGNAL(toggled(bool)), _main, SLOT(menu[MENU_WINDOW]Activated(bool)));
        aAction->setChecked(mdiArea->activeSubWindow() == windows.at(i));
    }
}

/* . */
void
MainWindow::windowMenuActivated(bool checked)
{
    debug_message("windowMenuActivated()");
    QAction* aSender = qobject_cast<QAction*>(sender());
    if (!aSender) {
        return;
    }
    QWidget* w = mdiArea->subWindowList().at(aSender->data().toInt());
    if (w && checked) {
        w->setFocus();
    }
}

/* . */
void
new_file(void)
{
    debug_message("new_file()");
    docIndex++;
    numOfDocs++;
    MdiWindow* mdiWin = new MdiWindow(docIndex, _main, mdiArea, Qt::SubWindow);
    QObject::connect(mdiWin, SIGNAL(sendCloseMdiWin(MdiWindow*)), _main,
        SLOT(onCloseMdiWin(MdiWindow*)));
    QObject::connect(mdiArea, SIGNAL(subWindowActivated(QMdiSubWindow*)), _main,
        SLOT(onWindowActivated(QMdiSubWindow*)));

    update_interface();
    windowMenuAboutToShow();

    Document* doc = mdiWin->gview;
    if (doc) {
        doc_recalculate_limits(doc);
        doc_zoom_extents(doc);
    }
}

/* . */
void
openFile(bool recent, QString recentFile)
{
    debug_message("openFile()");

    QApplication::setOverrideCursor(Qt::ArrowCursor);

    QStringList files;
    bool preview = get_bool(OPENSAVE_OPEN_THUMBNAIL);
    openFilesPath = get_str(OPENSAVE_RECENT_DIRECTORY);

    /* Check to see if this from the recent files list. */
    if (recent) {
        files.append(recentFile);
        openFilesSelected(files);
    }
    else if (!preview) {
        /* TODO: set getOpenFileNames' selectedFilter parameter from opensave_open_format.setting */
        files = QFileDialog::getOpenFileNames(_main, translate("Open"), openFilesPath, formatFilterOpen);
        openFilesSelected(files);
    }
    else if (preview) {
        PreviewDialog* openDialog = new PreviewDialog(_main, translate("Open w/Preview"),
            openFilesPath, formatFilterOpen);
        /* TODO: set openDialog->selectNameFilter(QString filter) from opensave_open_format.setting */
        QObject::connect(openDialog, SIGNAL(filesSelected(const QStringList&)), _main,
            SLOT(openFilesSelected(const QStringList&)));
        openDialog->exec();
    }

    restore_cursor();
}

/* . */
int
string_list_contains(char list[MAX_FILES][MAX_STRING_LENGTH], const char *entry)
{
    for (int i=0; ; i++) {
        if (!strncmp(list[i], "END", MAX_STRING_LENGTH)) {
            break;
        }
        if (!strncmp(list[i], entry, MAX_STRING_LENGTH)) {
            return 1;
        }
    }
    return 0;
}

/* . */
void
openFilesSelected(const QStringList& filesToOpen)
{
    debug_message("openFileSelected()");
    bool doOnce = true;

    if (filesToOpen.count()) {
        for (int i = 0; i < filesToOpen.count(); i++) {
            char message[MAX_STRING_LENGTH];
            sprintf(message, "opening %s...", qPrintable(filesToOpen[i]));
            debug_message(message);

            QMdiSubWindow* existing = findMdiWindow(filesToOpen[i]);
            if (existing) {
                debug_message("File already exists, switching to it.");
                mdiArea->setActiveSubWindow(existing);
                continue;
            }

            /* The docIndex doesn't need increased as it is only used for unnamed files. */
            numOfDocs++;
            MdiWindow* mdiWin = new MdiWindow(docIndex, _main, mdiArea, Qt::SubWindow);
            QObject::connect(mdiWin, SIGNAL(sendCloseMdiWin(MdiWindow*)), _main,
                SLOT(onCloseMdiWin(MdiWindow*)));
            QObject::connect(mdiArea, SIGNAL(subWindowActivated(QMdiSubWindow*)), _main,
                SLOT(onWindowActivated(QMdiSubWindow*)));

            /* Make sure the toolbars/etc... are shown before doing their zoomExtents. */
            if (doOnce) {
                update_interface();
                doOnce = false;
            }

            if (mdiWin->loadFile(filesToOpen.at(i))) {
                statusbar->showMessage(translate("File(s) loaded"), 2000);
                mdiWin->show();
                mdiWin->showMaximized();
                /* Prevent duplicate entries in the recent files list. */
                if (!string_list_contains(recent_files, qPrintable(filesToOpen.at(i)))) {
                    for (int j=0; j<MAX_FILES-1; j++) {
                        strcpy(recent_files[j], recent_files[j+1]);
                    }
                    strcpy(recent_files[0], qPrintable(filesToOpen.at(i)));
                }
                /* Move the recent file to the top of the list */
                else {
                    strcpy(recent_files[0], qPrintable(filesToOpen.at(i)));
                    strcpy(recent_files[1], "END");
                }
                set_str(OPENSAVE_RECENT_DIRECTORY, (char*)qPrintable(QFileInfo(filesToOpen.at(i)).absolutePath()));

                Document* doc = mdiWin->gview;
                if (doc) {
                    doc_recalculate_limits(doc);
                    doc_zoom_extents(doc);
                }
            }
            else {
                critical_box(translate("Failed to load file"),
                    translate("Failed to load file."));
                debug_message("Failed to load file.");
                mdiWin->close();
            }
        }
    }

    windowMenuAboutToShow();
}

/* . */
void
open_recent_file(void)
{
    debug_message("open_recent_file()");

    /* Check to see if this from the recent files list. */
    /* FIXME: QAction* recentSender = qobject_cast<QAction*>(sender());
    if (recentSender) {
        openFile(true, recentSender->data().toString());
    }
    */
}

/* . */
void
save_as_file(void)
{
    debug_message("save_as_file()");
    /* need to find the activeSubWindow before it loses focus to the FileDialog. */
    MdiWindow* mdiWin = qobject_cast<MdiWindow*>(mdiArea->activeSubWindow());
    if (!mdiWin) {
        return;
    }

    openFilesPath = get_str(OPENSAVE_RECENT_DIRECTORY);
    QString file = QFileDialog::getSaveFileName(_main, translate("Save As"), openFilesPath, formatFilterSave);

    mdiWin->saveFile(file);
}

/* . */
QMdiSubWindow*
findMdiWindow(QString fileName)
{
    char message[MAX_STRING_LENGTH];
    sprintf(message, "findMdiWindow(%s)", qPrintable(fileName));
    debug_message(message);
    QString canonicalFilePath = QFileInfo(fileName).canonicalFilePath();

    foreach (QMdiSubWindow* subWindow, mdiArea->subWindowList()) {
        MdiWindow* mdiWin = qobject_cast<MdiWindow*>(subWindow);
        if (mdiWin) {
            if (mdiWin->curFile == canonicalFilePath) {
                return subWindow;
            }
        }
    }
    return 0;
}

/* . */
void
MainWindow::closeEvent(QCloseEvent* event)
{
    mdiArea->closeAllSubWindows();
    writeSettings();
    event->accept();
}

/* . */
void
onCloseWindow(void)
{
    debug_message("onCloseWindow()");
    MdiWindow* mdiWin = qobject_cast<MdiWindow*>(mdiArea->activeSubWindow());
    if (mdiWin) {
        onCloseMdiWin(mdiWin);
    }
}

/* . */
void
onCloseMdiWin(MdiWindow* theMdiWin)
{
    debug_message("onCloseMdiWin()");
    numOfDocs--;

    bool keepMaximized;
    if (theMdiWin) {
        keepMaximized = theMdiWin->isMaximized();
    }

    mdiArea->removeSubWindow(theMdiWin);
    theMdiWin->deleteLater();

    update_interface();
    windowMenuAboutToShow();

    if (keepMaximized) {
        MdiWindow* mdiWin = qobject_cast<MdiWindow*>(mdiArea->activeSubWindow());
        if (mdiWin) {
            mdiWin->showMaximized();
        }
    }
}

/* . */
void
onWindowActivated(QMdiSubWindow* w)
{
    debug_message("onWindowActivated()");
    MdiWindow* mdiWin = qobject_cast<MdiWindow*>(w);
    if (mdiWin) {
        mdiWin->onWindowActivated();
    }
}

/* . */
void
MainWindow::resizeEvent(QResizeEvent* e)
{
    debug_message("resizeEvent()");
    QMainWindow::resizeEvent(e);
    statusBar()->setSizeGripEnabled(!isMaximized());
}

/* . */
QAction*
getFileSeparator()
{
    debug_message("getFileSeparator()");
    return myFileSeparator;
}

/* . */
void
update_interface()
{
    debug_message("update_interface()");

    actionHash[ACTION_PRINT]->setEnabled(numOfDocs > 0);
    actionHash[ACTION_WINDOW_CLOSE]->setEnabled(numOfDocs > 0);
    actionHash[ACTION_DESIGN_DETAILS]->setEnabled(numOfDocs > 0);

    if (numOfDocs) {
        /* Toolbars */
        for (int i=0; toolbars_when_docs[i] != TERMINATOR_SYMBOL; i++) {
            toolbar[toolbars_when_docs[i]]->show();
        }

        /* DockWidgets */
        dockPropEdit->show();
        dockUndoEdit->show();

        /* Menus */
        menuBar()->clear();
        for (int i=0; menubar_full_list[i] != TERMINATOR_SYMBOL; i++) {
            menuBar()->addMenu(menu[menubar_full_list[i]]);
        }
        menu[MENU_WINDOW]->setEnabled(true);

        /* Statusbar */
        statusbar->clearMessage();
        statusBarMouseCoord->show();
        statusBarSnapButton->show();
        statusBarGridButton->show();
        statusBarRulerButton->show();
        statusBarOrthoButton->show();
        statusBarPolarButton->show();
        statusBarQSnapButton->show();
        statusBarQTrackButton->show();
        statusBarLwtButton->show();
    }
    else {
        /* Toolbars */
        for (int i=0; toolbars_when_docs[i] != TERMINATOR_SYMBOL; i++) {
            toolbar[toolbars_when_docs[i]]->hide();
        }

        /* DockWidgets */
        dockPropEdit->hide();
        dockUndoEdit->hide();

        /* Menus */
        menuBar()->clear();
        for (int i=0; menubar_no_docs[i] != TERMINATOR_SYMBOL; i++) {
            menuBar()->addMenu(menu[menubar_no_docs[i]]);
        }
        menu[MENU_WINDOW]->setEnabled(false);

        /* Statusbar */
        statusbar->clearMessage();
        statusBarMouseCoord->hide();
        statusBarSnapButton->hide();
        statusBarGridButton->hide();
        statusBarRulerButton->hide();
        statusBarOrthoButton->hide();
        statusBarPolarButton->hide();
        statusBarQSnapButton->hide();
        statusBarQTrackButton->hide();
        statusBarLwtButton->hide();
    }
    hide_unimplemented();
}

/* . */
void
hide_unimplemented(void)
{
    debug_message("hide_unimplemented()");
}

/* . */
bool
validFileFormat(QString fileName)
{
    if (emb_identify_format(qPrintable(fileName)) >= 0) {
        return true;
    }
    return false;
}

/* . */
void
loadFormats()
{
    char stable, unstable;
    QString supportedReaders  = "All Supported Files (";
    QString individualReaders = "All Files (*);;";
    QString supportedWriters  = "All Supported Files (";
    QString individualWriters = "All Files (*);;";
    QString supportedStr;
    QString individualStr;

    /* TODO: Stable Only (Settings Option) */
    /* stable = 'S'; unstable = 'S'; */

    /* Stable + Unstable */
    stable = 'S'; unstable = 'U';


    for (int i=0; i<numberOfFormats; i++) {
        const char* extension = formatTable[i].extension;
        const char* description = formatTable[i].description;
        char readerState = formatTable[i].reader_state;
        char writerState = formatTable[i].writer_state;

        QString upperExt = QString(extension).toUpper();
        supportedStr = "*" + upperExt + " ";
        individualStr = upperExt.replace(".", "") + " - " + description + " (*" + extension + ");;";
        if (readerState == stable || readerState == unstable) {
            /* Exclude color file formats from open dialogs. */
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

    /* TODO: Fixup custom filter. */
    /*
    QString custom = custom_filter.setting;
    if (custom.contains("supported", Qt::CaseInsensitive)) {
        custom = ""; */ /* This will hide it */ /*
    }
    else if (!custom.contains("*", Qt::CaseInsensitive)) {
        custom = ""; */ /* This will hide it */ /*
    }
    else {
        custom = "Custom Filter(" + custom + ");;";
    }

    return tr(qPrintable(custom + supported + all));
    */
}

/* . */
void
MainWindow::closeToolBar(QAction* action)
{
    if (action->objectName() == "toolbarclose") {
        QToolBar* tb = qobject_cast<QToolBar*>(sender());
        if (tb) {
            debug_message(qPrintable(tb->objectName() + " closed."));
            tb->hide();
        }
    }
}

/* . */
void
MainWindow::floatingChangedToolBar(bool isFloating)
{
    QToolBar* tb = qobject_cast<QToolBar*>(sender());
    if (tb) {
        if (isFloating) {
            /* TODO: Determine best suited close button on various platforms. */
            /*
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
                QAction* ACTION = actList[i];
                if (ACTION->objectName() == "toolbarclose") {
                    tb->removeAction(ACTION);
                    disconnect(tb, SIGNAL(actionTriggered(QAction*)), this, SLOT(closeToolBar(QAction*)));
                    delete ACTION;
                }
            }
        }
    }
}

/* . */
QAction*
get_action_by_icon(const char *icon)
{
    int i;
    for (i=0; command_data[i].id != -2; i++) {
        if (!strcmp(command_data[i].icon, icon)) {
            return actionHash[command_data[i].id];
        }
    }
    return actionHash[ACTION_DO_NOTHING];
}

int
get_id(char *data[], char *label)
{
    int id;
    int n = string_array_length(data);
    for (id=0; id<n; id++) {
        if (!strcmp(data[id], label)) {
            return id;
        }
    }
    return -1;
}

/* . */
void
add_to_menu(int index, char *menu_data[])
{
    int n = string_array_length(menu_data);
    for (int i=0; i<n; i++) {
        char *s = menu_data[i];
        if (s[0] == '-') {
            menu[index]->addSeparator();
        }
        else if (s[0] == '>') {
            int id = get_id(menu_list, s+1);
            if (id < 0) {
                debug_message("Failed to identify submenu.");
                continue;
            }
            menu[index]->addMenu(menu[id]);
        }
        else if (s[0] == '+') {
            QString icontheme = get_str(GENERAL_ICON_THEME);
            menu[index]->setIcon(create_icon(s+1));
        }
        else {
            menu[index]->addAction(get_action_by_icon(s));
        }
    }
    menu[index]->setTearOffEnabled(false);
}

/* Do not allow the menus to be torn off.
 * It's a pain in the ass to maintain.
 */
void
createAllMenus(void)
{
    debug_message("createAllMenus()");

    menuBar()->addMenu(menu[MENU_FILE]);
    QObject::connect(menu[MENU_RECENT], SIGNAL(aboutToShow()), _main,
        SLOT(recentMenuAboutToShow()));
    menu[MENU_RECENT]->setTearOffEnabled(false);
    add_to_menu(MENU_FILE, file_menu);

    menuBar()->addMenu(menu[MENU_EDIT]);
    add_to_menu(MENU_EDIT, edit_menu);

    menuBar()->addMenu(menu[MENU_VIEW]);
    add_to_menu(MENU_VIEW, view_menu);
    add_to_menu(MENU_ZOOM, zoom_menu);
    add_to_menu(MENU_PAN, pan_menu);

    menuBar()->addMenu(menu[MENU_TOOLS]);
    add_to_menu(MENU_TOOLS, tools_menu);

    menuBar()->addMenu(menu[MENU_DRAW]);
    add_to_menu(MENU_DRAW, draw_menu);

    menuBar()->addMenu(menu[MENU_DIMENSION]);
    add_to_menu(MENU_DIMENSION, dimension_menu);

    menuBar()->addMenu(menu[MENU_MODIFY]);
    add_to_menu(MENU_MODIFY, modify_menu);

    menuBar()->addMenu(menu[MENU_SANDBOX]);
    add_to_menu(MENU_SANDBOX, sandbox_menu);

    menuBar()->addMenu(menu[MENU_WINDOW]);
    QObject::connect(menu[MENU_WINDOW], SIGNAL(aboutToShow()), _main,
        SLOT(windowMenuAboutToShow()));
    menu[MENU_WINDOW]->setTearOffEnabled(false);

    menuBar()->addMenu(menu[MENU_HELP]);
    add_to_menu(MENU_HELP, help_menu);
}

/* Note: on Unix we include the trailing separator. For Windows compatibility we
 * omit it.
 */
QString
SettingsDir()
{
#if defined(Q_OS_UNIX) || defined(Q_OS_MAC)
    return QDir::homePath() + "/.embroidermodder2/";
#else
    return "";
#endif
}

/* . */
void
readSettings(void)
{
    debug_message("Reading Settings...");
    /* This file needs to be read from the users home directory to ensure it is writable. */
    QString settingsDir = SettingsDir();
    /* load_settings(qPrintable(SettingsDir())); */

    QSettings settings(SettingsDir() + settings_file, QSettings::IniFormat);
    QPoint pos = settings.value("Window/Position", QPoint(0, 0)).toPoint();
    QSize size = settings.value("Window/Size", QSize(800, 600)).toSize();

    layoutState = settings.value("LayoutState").toByteArray();
    if (!_main->restoreState(layoutState)) {
        debug_message("LayoutState NOT restored! Setting Default Layout...");
        /* someToolBar->setVisible(true); */
    }

    for (int i=0; i<N_SETTINGS; i++) {
        char *label = settings_data[i].label;
        char *value = settings_data[i].value;
        switch (settings_data[i].type) {
        case SCRIPT_INT: {
            int x = settings.value(label, atoi(value)).toInt();
            printf("%d %s %d %d\n", i, label, atoi(value), x);
            set_int(i, x);
            break;
        }
        case SCRIPT_BOOL: {
            bool x = settings.value(label, (bool)atoi(value)).toBool();
            printf("%d %s %d %d\n", i, label, (bool)atoi(value), x);
            set_bool(i, x);
            break;
        }
        case SCRIPT_STRING: {
            char *x = (char *)qPrintable(settings.value(label, QString(value)).toString());
            printf("%d %s %s %s\n", i, label, value, x);
            set_str(i, x);
            break;
        }
        case SCRIPT_REAL: {
            float x = settings.value(label, atof(value)).toFloat();
            printf("%d %s %f %f\n", i, label, atof(value), x);
            set_real(i, x);
            break;
        }
        default:
            // TODO: ERROR
            break;
        }
    }

    /* FIXME:
    _main->move(pos);
    _main->resize(size);
    */
}

/* . */
void
writeSettings(void)
{
    debug_message("Writing Settings...");
    save_settings("", qPrintable(SettingsDir() + settings_file));
}

/* . */
void
settingsPrompt(void)
{
    settingsDialog("Prompt");
}

/* . */
void
settingsDialog(QString showTab)
{
    Settings_Dialog dialog(_main, showTab, _main);
    dialog.exec();
}

/* . */
void
add_to_toolbar(int id, char *toolbar_data[])
{
    toolbar[id]->setObjectName(QString("toolbar") + toolbar_list[id]);

    int n = string_array_length(toolbar_data);
    for (int i=0; i<n; i++) {
        if (toolbar_data[i][0] == '-') {
            toolbar[id]->addSeparator();
        }
        else {
            QAction *action = get_action_by_icon(toolbar_data[i]);
            toolbar[id]->addAction(action);
        }
    }

    QAction::connect(toolbar[id], SIGNAL(topLevelChanged(bool)), _main,
        SLOT(floatingChangedToolBar(bool)));
}

/* For each Command in command_data, for each alias set up a map from
 * alias to the Command. Then for a given context the call doesn't have to loop?
 *
 * NOTE: Every QScriptProgram must have a unique function name to call. If every function was called main(), then
 *       the ScriptArgs would only call the last script evaluated (which happens to be main() in another script).
 *       Thus, by adding the cmdName before main(), it becomes line_main(), circle_main(), etc...
 *       Do not change this code unless you really know what you are doing. I mean it.
 *
 * Position currently comes from the order of the command_data.
 *
 * TODO: Set What's This Context Help to statusTip for now so there is some infos there.
 *       Make custom whats this context help popup with more descriptive help than just
 *       the status bar/tip one liner(short but not real long) with a hyperlink in the custom popup
 *       at the bottom to open full help file description. Ex: like wxPython AGW's SuperToolTip.
 */
void
createAllActions(void)
{
    debug_message("Creating All Actions...");
    for (int i=0; command_data[i].id != -2; i++) {
        QString icon(command_data[i].icon);
        QString toolTip(command_data[i].tooltip);
        QString statusTip(command_data[i].statustip);
        QString alias_string(command_data[i].alias);
        QStringList aliases = alias_string.split(", ");

        debug_message(qPrintable("COMMAND: " + icon));

        QAction *ACTION = new QAction(create_icon(icon), toolTip, _main);
        ACTION->setStatusTip(statusTip);
        ACTION->setObjectName(icon);
        ACTION->setWhatsThis(statusTip);

        if (strcmp(command_data[i].shortcut, "")) {
            ACTION->setShortcut(QKeySequence(command_data[i].shortcut));
        }

        if (icon == "textbold" || icon == "textitalic" || icon == "textunderline"
            || icon == "textstrikeout" || icon == "textoverline") {
            ACTION->setCheckable(true);
        }

        QObject::connect(ACTION, SIGNAL(triggered()), _main, SLOT(runCommand()));

        aliasHash[icon.toStdString()] = icon.toStdString();
        actionHash[command_data[i].id] = ACTION;

        foreach (QString alias, aliases) {
            char msg[200];
            aliasHash[alias.toStdString()] = icon.toStdString();
            sprintf(msg, "Command Added: %s, %s", qPrintable(icon), qPrintable(icon));
            debug_message(msg);
        }
    }

    actionHash[ACTION_WINDOW_CLOSE]->setEnabled(numOfDocs > 0);
    actionHash[ACTION_DESIGN_DETAILS]->setEnabled(numOfDocs > 0);
}
