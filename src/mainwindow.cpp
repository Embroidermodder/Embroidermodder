/*
 * Embroidermodder 2
 * Copyright 2011-2025 The Embroidermodder Team
 *
 * Embroidermodder 2 is free and open software under the zlib license:
 * see LICENSE.md for details.
 *
 * ----------------------------------------------------------------------------
 *
 * The main starting point for loading the application and main window.
 *
 * This is the most frequently updated part of the source: new core developers would
 * benefit most from understanding the rough approach of this file and making
 * small changes here.
 *
 * NOTE: the run function is the part of the source treated with the most
 * concern for optimization and bug traceability.
 *
 * Core Commands and User Interaction
 * ==================================
 *
 * All user interaction takes place via commands which can be:
 *
 * 1. Core commands defined for internal use that uses the switch table below
 *    for fast calling with logging for debugging purposes and the undo history.
 * 2. Functions written in lua scripts in the `scripts/` folder that call the
 *    wrapper `cmd_f` to run this switch table.
 * 3. C++ functions designed to take the same arguments called lua registerables
 *    in src/script.cpp: intended the most complex commands. These call `run`
 *    directly using the appropriate constant.
 *
 * For example, when the user issues a "day" command we have 3 internal calls to make
 * which the day command is a wrapper for: change the grid color, change the background
 * color and change the crosshair color. Each of these is issued as a core command,
 * with day being a macro encoding these as a list of core commands.
 *
 * It is likely (FIXME: decide on this) that all core commands will not be modal and
 * only the scripted commands deal with the mode variable.
 */

#include "embroidermodder.h"

#include <math.h>

#define KEY_SHORTCUT(F, CMD) \
    connect(prompt, SIGNAL(F()), this, SLOT([=]() { cmd(CMD); }))

using namespace std::chrono_literals;

QList<QGraphicsItem*> clipboard;

QHash<QString, QAction*> actionHash;
QHash<QString, QToolBar*> toolbarHash;
QHash<QString, QMenu*> menuHash;

QString formatFilterOpen;
QString formatFilterSave;

bool exitApp = false;

/* Pointer access */
MainWindow* _mainWin = NULL;
MdiArea* mdiArea = NULL;
CmdPrompt* prompt = NULL;
PropertyEditor* dockPropEdit = NULL;
UndoEditor* dockUndoEdit = NULL;
StatusBar* statusbar = NULL;

/* Selectors */
QComboBox* layerSelector;
QComboBox* colorSelector;
QComboBox* linetypeSelector;
QComboBox* lineweightSelector;
QFontComboBox* textFontSelector;
QComboBox* textSizeSelector;

/* Tables */
std::vector<Command> command_map;
std::unordered_map<std::string, ToolbarData> toolbar_table;
std::unordered_map<std::string, MenuData> menu_table;
std::unordered_map<std::string, StringList> string_tables;
std::unordered_map<std::string, std::vector<PropertiesData>> properties_table;
std::unordered_map<QString, QString> aliases;
SettingsData settings_table[N_SETTINGS];

int
main(int argc, char* argv[])
{
    load_data();

#if defined(Q_OS_MAC)
    Application app(argc, argv);
#else
    QApplication app(argc, argv);
#endif
    app.setApplicationName(state.name);
    app.setApplicationVersion(state.version);

    QStringList filesToOpen;

    for (int i = 1; i < argc; i++) {
        if (!strcmp(argv[i], "-d") || !strcmp(argv[i], "--debug")  ) {
        }
        else if (!strcmp(argv[i], "-s") || !strcmp(argv[i], "--sdl")  ) {
            sdl_version(argc, argv);
            exitApp = true;
        }
        else if (!strcmp(argv[i], "-h") || !strcmp(argv[i], "--help")   ) {
            fprintf(stdout, "%s", usage_msg);
            exitApp = true;
        }
        else if (!strcmp(argv[i], "-v") || !strcmp(argv[i], "--version")) {
            fprintf(stdout, "%s %s\n", state.name, state.version);
            exitApp = true;
        }
        else if (!strcmp(argv[i], "-t") || !strcmp(argv[i], "--test")) {
            state.testing = true;
        }
        else if(QFile::exists(argv[i]) && MainWindow::validFileFormat(argv[i])) {
            filesToOpen << argv[i];
        }
        else {
            fprintf(stdout, "%s", usage_msg);
            exitApp = true;
        }
    }

    if (exitApp) {
        return 1;
    }

    MainWindow* mainWin = new MainWindow();
#if defined(Q_OS_MAC)
    app.setMainWin(mainWin);
#endif

    QObject::connect(&app, SIGNAL(lastWindowClosed()), mainWin, SLOT(quit()));

    mainWin->setWindowTitle(app.applicationName() + " " + app.applicationVersion());
    mainWin->show();

    //NOTE: If openFilesSelected() is called from within the mainWin constructor, slot commands wont work and the window menu will be screwed
    if(!filesToOpen.isEmpty())
        mainWin->openFilesSelected(filesToOpen);

    return app.exec();
}

Application::Application(int argc, char **argv) : QApplication(argc, argv), _mainWin(NULL)
{
}

bool Application::event(QEvent *event)
{
    switch (event->type()) {
    case QEvent::FileOpen:
        if (_mainWin) {
            _mainWin->openFilesSelected(QStringList(static_cast<QFileOpenEvent *>(event)->file()));
            return true;
        }
        // Fall through
    default:
        return QApplication::event(event);
    }
}

/*
 * Identify command id by its name in the supplied table.
 */
int
get_index(char *table[], const char *cmd)
{
    for (int i=0; table[i][0]!='_'; i++) {
        if (!strncmp(table[i], cmd, 50)) {
            return i;
        }
    }
    return -1;
}

/*
 * The main command processor.
 *
 * It is important that there is as little as possible before the switch: this
 * overhead would be present for every core command issued and individual commands
 * may consist of thousands of core command calls.
 *
 * Wrappers like runCommandMain and cmd_f can add manage the interface state with before
 * calling this function so we don't check that, for example, the clipboard is
 * empty before entering the switch table.
 *
 * If the body of the entry for a case is more than a dozen lines or so, then
 * it is worth making it a seperate function just to reduce indentation.
 *
 * WARNING: note that this does not perform type checking. This is performed
 * on lua style functions, but we either assume that the runCommand* family of functions
 * has vetted the data before it gets to this point or it is the caller's
 * responsibility to get it right within a C++ function.
 *
 * TODO: Appropriate state changes for user issued commands like this:
 *
 *     if (state.context_flag == CONTEXT_MAIN) {
 *         cmd("clear_rubber");
 *         // Some selection based commands need to override this.
 *         cmd("clear");
 *     }
 *     cmd(args[0].s);
 *     // TODO: conditional on if the command is open ended or not.
 *     cmd("end");
 *     return 0;
 */
void
cmd(const char *line)
{
    int argc = 0;
    char argv[20][200];

    prompt->appendHistory(line);

    QString qstr(line);
    QStringList list = qstr.split(" ");
    const char *cmd = qPrintable(list[0]);
    if (aliases.find(list[0]) != aliases.end()) {
        cmd = qPrintable(aliases.at(list[0]));
    }
    int id = -1;
    for (int i=0; i<MAX_COMMANDS; i++) {
        if (!strncmp(state.commands[i].name, cmd, 20)) {
            id = i;
            break;
        }
    }
    if (id < 0) {
        debug("ERROR: unrecognised command id.");
        return;
    }

    if (argc > 20) {
        debug("ERROR: too many arguments supplied.");
    }
    for (int i=1; i<list.size(); i++) {
        strncpy(argv[i], qPrintable(list.at(i)), 200-1);
    }
    argc = list.size() - 1;

    if (state.debug > 0) {
        debug("COMMAND %s", state.commands[id].name);
    }

    int error = state.commands[id].function(argc, (char**)argv);
}

/*
 * Get a value that is stored in a state variable within a macro.
 *
 * Note that this is a C-linked function.
 */
ScriptValue
get(const char *key)
{
    ScriptValue result;
    result.r = 0.0;
    result.i = 0;
    result.b = false;
    if (!strncmp(key, "text_angle", 20)) {
        result.r = st[ST_TEXT_ANGLE].r;
        return result;
    }
    if (!strncmp(key, "text_size", 20)) {
        result.i = st[ST_TEXT_SIZE].i;
        return result;
    }
    if (!strncmp(key, "text_bold", 20)) {
        result.b = st[ST_TEXT_BOLD].b;
        return result;
    }
    if (!strncmp(key, "text_italic", 20)) {
        result.b = st[ST_TEXT_ITALIC].b;
        return result;
    }
    if (!strncmp(key, "text_underline", 20)) {
        result.b = st[ST_TEXT_UNDERLINE].b;
        return result;
    }
    if (!strncmp(key, "text_strikeout", 20)) {
        result.b = st[ST_TEXT_STRIKEOUT].b;
        return result;
    }
    if (!strncmp(key, "text_overline", 20)) {
        result.b = st[ST_TEXT_OVERLINE].b;
        return result;
    }
    if (!strncmp(key, "platform", 20)) {
        strncpy(result.s, qPrintable(_mainWin->platformString()), 200);
        return result;
    }
    if (!strncmp(key, "prefix", 20)) {
        strncpy(result.s, qPrintable(prompt->promptInput->prefix), 200);
        return result;
    }
    /* Report the current x-position of the mouse and return it. */
    if (!strncmp(key, "mousex", 20)) {
        QGraphicsScene* scene = activeScene();
        if (scene) {
            result.r = scene->property(SCENE_MOUSE_POINT).toPointF().x();
        }
        return result;
    }
    /* Report the current y-position of the mouse and return it. */
    if (!strncmp(key, "mousey", 20)) {
        QGraphicsScene* scene = activeScene();
        if (scene) {
            result.r = -scene->property(SCENE_MOUSE_POINT).toPointF().y();
        }
        return result;
    }
    /* Return the current x-position of the quicksnap position. */
    if (!strncmp(key, "qsnapx", 20)) {
        QGraphicsScene* scene = activeScene();
        if (scene) {
            result.r = scene->property(SCENE_QSNAP_POINT).toPointF().x();
        }
        return result;
    }
    /* Return the current y-position of the quicksnap position. */
    if (!strncmp(key, "qsnapy", 20)) {
        QGraphicsScene* scene = activeScene();
        if (scene) {
            result.r = -scene->property(SCENE_QSNAP_POINT).toPointF().y();
        }
        return result;
    }
    /* Return the current y-position of the quicksnap position. */
    if (!strncmp(key, "num_selected", 20)) {
        View* gview = activeView();
        if (gview) {
            result.i = gview->numSelected();
        }
        return result;
    }
    return result;
}

/* . */
int
set_text_font_f(const char *str)
{
    debug("TODO: add_to_menu");
    textFontSelector->setCurrentFont(QFont(str));
    strncpy(st[ST_TEXT_FONT].s, str, 200);
    return 0;
}

void
update_text_size(void)
{
    int index = textSizeSelector->findText("Custom", Qt::MatchContains);
    if (index != -1) {
        textSizeSelector->removeItem(index);
    }
    qreal num = st[ST_TEXT_SIZE].i;
    textSizeSelector->addItem("Custom " + QString().setNum(num, 'f', 2) + " pt", num);
    index = textSizeSelector->findText("Custom", Qt::MatchContains);
    if (index != -1) {
        textSizeSelector->setCurrentIndex(index);
    }
}

void
update_prompt_prefix(const char *prefix)
{
    prompt->setPrefix(prefix);
}

// Icons
void
MainWindow::iconResize(int iconSize)
{
    this->setIconSize(QSize(iconSize, iconSize));
    layerSelector->     setIconSize(QSize(iconSize*4, iconSize));
    colorSelector->     setIconSize(QSize(iconSize,   iconSize));
    linetypeSelector->  setIconSize(QSize(iconSize*4, iconSize));
    lineweightSelector->setIconSize(QSize(iconSize*4, iconSize));
    //set the minimum combobox width so the text is always readable
    layerSelector->     setMinimumWidth(iconSize*4);
    colorSelector->     setMinimumWidth(iconSize*2);
    linetypeSelector->  setMinimumWidth(iconSize*4);
    lineweightSelector->setMinimumWidth(iconSize*4);

    //TODO: low-priority: open app with iconSize set to 128. resize the icons to a smaller size.

    st[ST_ICON_SIZE].i = iconSize;
}

// TODO: check that the toReal() conversion is ok
void
MainWindow::textSizeSelectorIndexChanged(int index)
{
    debug("textSizeSelectorIndexChanged(%d)", index);
    st[ST_TEXT_SIZE].i = qFabs(textSizeSelector->itemData(index).toReal());
}

MainWindow::MainWindow() : QMainWindow(0)
{
    _mainWin = this;

    readSettings();

    QString appDir = qApp->applicationDirPath();
    //Verify that files/directories needed are actually present.
    QStringList folders = {
        "docs",
        "icons",
        "images",
        "samples",
        "translations",
        "data"
    };
    for (QString folder: folders) {
        QFileInfo check(appDir + "/" + folder);
        if (!check.exists()) {
            QMessageBox::critical(this, tr("Path Error"),
                tr("Cannot locate: ") + check.absoluteFilePath());
        }
    }

    QString lang = st[ST_LANGUAGE].s;
    debug("language: %s", qPrintable(lang));
    if(lang == "system")
        lang = QLocale::system().languageToString(QLocale::system().language()).toLower();

    //Load translations for the Embroidermodder 2 GUI
    QTranslator translatorEmb;
    translatorEmb.load(appDir + "/translations/" + lang + "/" + lang);
    qApp->installTranslator(&translatorEmb);

    //Load translations provided by Qt - this covers dialog buttons and other common things.
    QTranslator translatorQt;
    translatorQt.load("qt_" + QLocale::system().name(), QLibraryInfo::location(QLibraryInfo::TranslationsPath)); //TODO: ensure this always loads, ship a copy of this with the app
    qApp->installTranslator(&translatorQt);

    //Toolbars
    toolbarHash["FILE"] = addToolBar(tr("File"));
    toolbarHash["EDIT"] = addToolBar(tr("Edit"));
    toolbarHash["VIEW"] = addToolBar(tr("View"));
    toolbarHash["ZOOM"] = addToolBar(tr("Zoom"));
    toolbarHash["PAN"] = addToolBar(tr("Pan"));
    toolbarHash["ICON"] = addToolBar(tr("Icon"));
    toolbarHash["HELP"] = addToolBar(tr("Help"));
    toolbarHash["LAYER"] = addToolBar(tr("Layer"));
    toolbarHash["PROPERTIES"] = addToolBar(tr("Properties"));
    toolbarHash["TEXT"] = addToolBar(tr("Text"));
    toolbarHash["PROMPT"] = addToolBar(tr("Command Prompt"));
    toolbarHash["DRAW"] = addToolBar(tr("Draw"));
    toolbarHash["INQUIRY"] = addToolBar(tr("Inquiry"));
    toolbarHash["MODIFY"] = addToolBar(tr("Modify"));
    toolbarHash["DIMENSION"] = addToolBar(tr("Dimension"));
    //Selectors
    layerSelector      = new QComboBox(this);
    colorSelector      = new QComboBox(this);
    linetypeSelector   = new QComboBox(this);
    lineweightSelector = new QComboBox(this);
    textFontSelector   = new QFontComboBox(this);
    textSizeSelector   = new QComboBox(this);

    state.numOfDocs = 0;
    state.docIndex = 0;

    state.shift = false;

    setWindowIcon(createIcon("app"));
    setMinimumSize(800, 480); //Require Minimum WVGA

    loadFormats();

    //create the mdiArea
    QFrame* vbox = new QFrame(this);
    QVBoxLayout* layout = new QVBoxLayout(vbox);
    //layout->setMargin(0);
    vbox->setFrameStyle(QFrame::StyledPanel | QFrame::Sunken);
    mdiArea = new MdiArea(this, vbox);
    mdiArea->useBackgroundLogo(st[ST_MDI_BG_USE_LOGO].b);
    mdiArea->useBackgroundTexture(st[ST_MDI_BG_USE_TEXTURE].b);
    mdiArea->useBackgroundColor(st[ST_MDI_BG_USE_COLOR].b);
    mdiArea->setBackgroundLogo(st[ST_MDI_BG_LOGO].s);
    mdiArea->setBackgroundTexture(st[ST_MDI_BG_TEXTURE].s);
    mdiArea->setBackgroundColor(QColor(st[ST_MDI_BG_COLOR].u));
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

    prompt->setPromptTextColor(QColor(st[ST_PROMPT_TEXT_COLOR].u));
    prompt->setPromptBackgroundColor(QColor(st[ST_PROMPT_BG_COLOR].u));

    connect(prompt, SIGNAL(startCommand(const QString&)), this, SLOT(logPromptInput(const QString&)));

    connect(prompt, SIGNAL(startCommand(const QString&)), this, SLOT(runCommandMain(const QString&)));

    connect(prompt, SIGNAL(deletePressed()),    this, SLOT(deletePressed()));
    //TODO: connect(prompt, SIGNAL(tabPressed()),       this, SLOT(someUnknownSlot()));
    connect(prompt, SIGNAL(escapePressed()),    this, SLOT(escapePressed()));
    connect(prompt, SIGNAL(upPressed()),        this, SLOT(promptInputPrevious()));
    connect(prompt, SIGNAL(downPressed()),      this, SLOT(promptInputNext()));
    KEY_SHORTCUT(F1Pressed, CMD_HELP);
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

    connect(prompt, &CmdPrompt::shiftPressed, this, [=]() { state.shift = true; });
    connect(prompt, &CmdPrompt::shiftReleased, this, [=]() { state.shift = false; });

    connect(prompt, SIGNAL(showSettings()),     this, SLOT(settingsPrompt()));

    connect(prompt, SIGNAL(historyAppended(const QString&)), this, SLOT(promptHistoryAppended(const QString&)));

    //create the Object Property Editor
    dockPropEdit = new PropertyEditor(appDir + "/icons/" + QString(st[ST_ICON_THEME].s),
        st[ST_SELECTION_MODE_PICKADD].b, prompt, this);
    addDockWidget(Qt::LeftDockWidgetArea, dockPropEdit);
    connect(dockPropEdit, SIGNAL(pickAddModeToggled()), this, SLOT(pickAddModeToggled()));

    //create the Command History Undo Editor
    dockUndoEdit = new UndoEditor(appDir + "/icons/" + QString(st[ST_ICON_THEME].s),
        prompt, this);
    addDockWidget(Qt::LeftDockWidgetArea, dockUndoEdit);

    //setDockOptions(QMainWindow::AnimatedDocks | QMainWindow::AllowTabbedDocks | QMainWindow::VerticalTabs); //TODO: Load these from settings
    //tabifyDockWidget(dockPropEdit, dockUndoEdit); //TODO: load this from settings

    statusbar = new StatusBar(this, this);
    this->setStatusBar(statusbar);

    createAllActions();
    createAllMenus();
    createAllToolbars();

    iconResize(st[ST_ICON_SIZE].i);
    updateMenuToolbarStatusbar();

    /* Show date in statusbar after it has been updated. */
    QDate date = QDate::currentDate();
    QString datestr = date.toString("MMMM d, yyyy");
    statusbar->showMessage(datestr);

    showNormal();

    /* Run updates at around 60fps. */
    timer.start(12, this);

    if (st[ST_TIP_OF_THE_DAY].b && (!state.testing)) {
        tipOfTheDay();
    }
}

MainWindow::~MainWindow()
{
    debug("MainWindow::Destructor()");

    /* Prevent memory leaks by deleting any unpasted objects. */
    qDeleteAll(clipboard.begin(), clipboard.end());
    clipboard.clear();
}

/* Allows us to run test scripts and animations. */
void
MainWindow::timerEvent(QTimerEvent * /* event */)
{
    if (state.testing) {
        StringList test_script = string_tables["test_script"];
        if (state.test_script_pos < test_script.size()) {
            const char *line = test_script[state.test_script_pos].c_str();
            cmd(line);
            cmd("sleep");
            state.test_script_pos++;
        }
    }

    update();
}

void
MainWindow::recentMenuAboutToShow()
{
    debug("MainWindow::recentMenuAboutToShow()");
    menuHash["RECENT"]->clear();

    QFileInfo recentFileInfo;
    QString recentValue;
    for (int i = 0; state.recent_files[i][0] != '_'; ++i) {
        /* If less than the max amount of entries add to menu. */
        if (i >= st[ST_RECENT_MAX_FILES].i) {
            break;
        }
        recentFileInfo = QFileInfo(state.recent_files[i]);
        if (recentFileInfo.exists() && validFileFormat(recentFileInfo.fileName())) {
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
            const char *s = state.recent_files[i];
            rAction->setData(s);
            menuHash["RECENT"]->addAction(rAction);
            connect(rAction, SIGNAL(triggered()), this, SLOT(openrecentfile()));
        }
    }

    /* Check that the maximum entries in table do not exceed the maximum table length. */
    if (st[ST_RECENT_MAX_FILES].i >= MAX_TABLE) {
        st[ST_RECENT_MAX_FILES].i = MAX_TABLE;
    }

    /* Ensure the list only has max amount of entries. */
    int length = table_length((char**)state.recent_files);
    int max_length = st[ST_RECENT_MAX_FILES].i;
    if (length > max_length) {
        strcpy(state.recent_files[max_length-1], END_SYMBOL);
    }
}

void MainWindow::windowMenuAboutToShow()
{
    debug("MainWindow::windowMenuAboutToShow()");
    menuHash["WINDOW"]->clear();
    menuHash["WINDOW"]->addAction(actionHash.value("windowclose"));
    menuHash["WINDOW"]->addAction(actionHash.value("windowcloseall"));
    menuHash["WINDOW"]->addSeparator();
    menuHash["WINDOW"]->addAction(actionHash.value("windowcascade"));
    menuHash["WINDOW"]->addAction(actionHash.value("windowtile"));
    menuHash["WINDOW"]->addSeparator();
    menuHash["WINDOW"]->addAction(actionHash.value("windownext"));
    menuHash["WINDOW"]->addAction(actionHash.value("windowprevious"));

    menuHash["WINDOW"]->addSeparator();
    QList<QMdiSubWindow*> windows = mdiArea->subWindowList();
    for(int i = 0; i < windows.count(); ++i) {
        QAction* aAction = new QAction(windows.at(i)->windowTitle(), this);
        aAction->setCheckable(true);
        aAction->setData(i);
        menuHash["WINDOW"]->addAction(aAction);
        connect(aAction, SIGNAL(toggled(bool)), this, SLOT(windowMenuActivated(bool)));
        aAction->setChecked(mdiArea->activeSubWindow() == windows.at(i));
    }
}

void MainWindow::windowMenuActivated(bool checked)
{
    debug("MainWindow::windowMenuActivated()");
    QAction* aSender = qobject_cast<QAction*>(sender());
    if (!aSender)
        return;
    QWidget* w = mdiArea->subWindowList().at(aSender->data().toInt());
    if (w && checked)
        w->setFocus();
}

void MainWindow::openFile(bool recent, const QString& recentFile)
{
    debug("MainWindow::openFile()");

    QApplication::setOverrideCursor(Qt::ArrowCursor);

    QStringList files;
    bool preview = st[ST_OPEN_THUMBNAIL].b;
    openFilesPath = st[ST_RECENT_DIRECTORY].s;

    //Check to see if this from the recent files list
    if(recent) {
        files.append(recentFile);
        openFilesSelected(files);
    }
    else if (!preview) {
        /* TODO: set getOpenFileNames' selectedFilter parameter from st[ST_OPEN_FORMAT].s */
        files = QFileDialog::getOpenFileNames(this, tr("Open"), openFilesPath, formatFilterOpen);
        openFilesSelected(files);
    }
    else if (preview) {
        PreviewDialog* openDialog = new PreviewDialog(this, tr("Open w/Preview"), openFilesPath, formatFilterOpen);
        /* TODO: set openDialog->selectNameFilter(const QString& filter) from st[ST_OPEN_FORMAT].s */
        connect(openDialog, SIGNAL(filesSelected(const QStringList&)), this, SLOT(openFilesSelected(const QStringList&)));
        openDialog->exec();
    }

    QApplication::restoreOverrideCursor();
}

void MainWindow::openFilesSelected(const QStringList& filesToOpen)
{
    bool doOnce = true;

    if(filesToOpen.count()) {
        for(int i = 0; i < filesToOpen.count(); i++) {
            if(!validFileFormat(filesToOpen[i]))
                continue;

            QMdiSubWindow* existing = findMdiWindow(filesToOpen[i]);
            if(existing) {
                mdiArea->setActiveSubWindow(existing);
                continue;
            }

            //The docIndex doesn't need increased as it is only used for unnamed files
            state.numOfDocs++;
            MdiWindow* mdiWin = new MdiWindow(state.docIndex, _mainWin, mdiArea, Qt::SubWindow);
            connect(mdiWin, SIGNAL(sendCloseMdiWin(MdiWindow*)), this, SLOT(onCloseMdiWin(MdiWindow*)));
            connect(mdiArea, SIGNAL(subWindowActivated(QMdiSubWindow*)), this, SLOT(onWindowActivated(QMdiSubWindow*)));

            //Make sure the toolbars/etc... are shown before doing their zoomExtents
            if (doOnce) {
                updateMenuToolbarStatusbar();
                doOnce = false;
            }

            if (mdiWin->loadFile(filesToOpen.at(i))) {
                statusbar->showMessage(tr("File(s) loaded"), 2000);
                mdiWin->show();
                mdiWin->showMaximized();
                /* FIXME:
                //Prevent duplicate entries in the recent files list
                if (!st[ST_RECENT_FILES].l.contains(filesToOpen.at(i), Qt::CaseInsensitive)) {
                    st[ST_RECENT_FILES].l.prepend(filesToOpen.at(i));
                }
                //Move the recent file to the top of the list
                else {
                    st[ST_RECENT_FILES].l.removeAll(filesToOpen.at(i));
                    st[ST_RECENT_FILES].l.prepend(filesToOpen.at(i));
                }
                */
                QString recent_dir = QFileInfo(filesToOpen.at(i)).absolutePath();
                strncpy(st[ST_RECENT_DIRECTORY].s, qPrintable(recent_dir), 200);

                View* v = mdiWin->gview;
                if (v) {
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

void MainWindow::openrecentfile()
{
    debug("MainWindow::openrecentfile()");

    //Check to see if this from the recent files list
    QAction* recentSender = qobject_cast<QAction*>(sender());
    if(recentSender) {
        openFile(true, recentSender->data().toString());
    }
}

QMdiSubWindow* MainWindow::findMdiWindow(const QString& fileName)
{
    debug("MainWindow::findMdiWindow(%s)", qPrintable(fileName));
    QString canonicalFilePath = QFileInfo(fileName).canonicalFilePath();

    foreach(QMdiSubWindow* subWindow, mdiArea->subWindowList()) {
        MdiWindow* mdiWin = qobject_cast<MdiWindow*>(subWindow);
        if(mdiWin) {
            if (mdiWin->curFile == canonicalFilePath) {
                return subWindow;
            }
        }
    }
    return 0;
}

void MainWindow::closeEvent(QCloseEvent* event)
{
    mdiArea->closeAllSubWindows();
    writeSettings();
    event->accept();
}

void
MainWindow::onCloseWindow()
{
    debug("MainWindow::onCloseWindow()");
    MdiWindow* mdiWin = qobject_cast<MdiWindow*>(mdiArea->activeSubWindow());
    if(mdiWin) {
        onCloseMdiWin(mdiWin);
    }
}

void
MainWindow::onCloseMdiWin(MdiWindow* theMdiWin)
{
    debug("MainWindow::onCloseMdiWin()");
    state.numOfDocs--;

    bool keepMaximized;
    if (theMdiWin) {
        keepMaximized = theMdiWin->isMaximized();
    }

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

/*
 */
void
MainWindow::onWindowActivated(QMdiSubWindow* w)
{
    debug("MainWindow::onWindowActivated()");
    MdiWindow* mdiWin = qobject_cast<MdiWindow*>(w);
    if (mdiWin) {
        mdiWin->onWindowActivated();
    }
}

/*
 */
void
MainWindow::resizeEvent(QResizeEvent* e)
{
    debug("MainWindow::resizeEvent()");
    QMainWindow::resizeEvent(e);
    statusBar()->setSizeGripEnabled(!isMaximized());
}

/* TODO: remove this.
 */
QAction* MainWindow::getFileSeparator()
{
    debug("MainWindow::getFileSeparator()");
    return myFileSeparator;
}

/*
 */
void
MainWindow::updateMenuToolbarStatusbar()
{
    debug("MainWindow::updateMenuToolbarStatusbar()");

    actionHash.value("print")->setEnabled(state.numOfDocs > 0);
    actionHash.value("windowclose")->setEnabled(state.numOfDocs > 0);
    actionHash.value("designdetails")->setEnabled(state.numOfDocs > 0);

    if (state.numOfDocs) {
        //Toolbars
        foreach(QToolBar* tb, toolbarHash) {
            tb->show();
        }

        //DockWidgets
        dockPropEdit->show();
        dockUndoEdit->show();

        //Menus
        menuBar()->clear();
        StringList order = string_tables["menubar_order"];
        for (int i=0; i<order.size(); i++) {
            const char *key = order[i].c_str();
            menuBar()->addMenu(menuHash[key]);
            menuHash[key]->setEnabled(true);
        }

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
        /* Toolbars */
        foreach(QToolBar* tb, toolbarHash) {
            tb->hide();
        }
        toolbarHash["FILE"]->show();
        toolbarHash["EDIT"]->show();

        //DockWidgets
        dockPropEdit->hide();
        dockUndoEdit->hide();

        //Menus
        menuBar()->clear();
        StringList order = string_tables["menubar_order"];
        for (int i=0; i<order.size(); i++) {
            const char *key = order[i].c_str();
            menuBar()->addMenu(menuHash[key]);
            if (menu_table[key].mdi_only) {
                menuHash[key]->setEnabled(false);
            }
            else {
                menuHash[key]->setEnabled(true);
            }
        }

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

void MainWindow::hideUnimplemented()
{
    debug("MainWindow::hideUnimplemented()");
}

bool MainWindow::validFileFormat(const QString& fileName)
{
    /* FIXME
    if (embFormat_typeFromName(qPrintable(fileName))) {
    */
        return true;
    //}
    return false;
}

void MainWindow::loadFormats()
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

    /* FIXME:
    EmbFormatList* curFormat = 0;
    EmbFormatList* formatList = embFormatList_create();
    if(!formatList) { QMessageBox::critical(this, tr("Format Loading Error"), tr("Unable to load formats from libembroidery.")); return; }
    curFormat = formatList;
    while(curFormat) {
        extension = embFormat_extension(curFormat);
        description = embFormat_description(curFormat);
        readerState = embFormat_readerState(curFormat);
        writerState = embFormat_writerState(curFormat);

        QString upperExt = QString(extension).toUpper();
        supportedStr = "*" + upperExt + " ";
        individualStr = upperExt.replace(".", "") + " - " + description + " (*" + extension + ");;";
        if(readerState == stable || readerState == unstable) {
            //Exclude color file formats from open dialogs
            if(upperExt != "COL" && upperExt != "EDR" && upperExt != "INF" && upperExt != "RGB") {
                supportedReaders.append(supportedStr);
                individualReaders.append(individualStr);
            }
        }
        if(writerState == stable || writerState == unstable) {
            supportedWriters.append(supportedStr);
            individualWriters.append(individualStr);
        }

        curFormat = curFormat->next;
    }
    embFormatList_free(formatList);
    formatList = 0;

    supportedReaders.append(");;");
    supportedWriters.append(");;");

    formatFilterOpen = supportedReaders + individualReaders;
    formatFilterSave = supportedWriters + individualWriters;

    //TODO: Fixup custom filter
    QString custom = st[ST_CUSTOM_FILTER].s;
    if (custom.contains("supported", Qt::CaseInsensitive)) {
        custom = ""; //This will hide it
    }
    else if (!custom.contains("*", Qt::CaseInsensitive)) {
        custom = ""; //This will hide it
    }
    else
        custom = "Custom Filter(" + custom + ");;";

    return tr(qPrintable(custom + supported + all));
    */
}

void MainWindow::closeToolBar(QAction* action)
{
    if(action->objectName() == "toolbarclose") {
        QToolBar* tb = qobject_cast<QToolBar*>(sender());
        if(tb) {
            debug("%s closed.", qPrintable(tb->objectName()));
            tb->hide();
        }
    }
}

void MainWindow::floatingChangedToolBar(bool isFloating)
{
    QToolBar* tb = qobject_cast<QToolBar*>(sender());
    if(tb) {
        if(isFloating) {
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
            for(int i = 0; i < actList.size(); ++i) {
                QAction* ACTION = actList.value(i);
                if(ACTION->objectName() == "toolbarclose") {
                    tb->removeAction(ACTION);
                    disconnect(tb, SIGNAL(actionTriggered(QAction*)), this, SLOT(closeToolBar(QAction*)));
                    delete ACTION;
                }
            }
        }
    }
}

// Do not allow the menus to be torn off. It's a pain in the ass to maintain.
void
MainWindow::addToMenu(QMenu *menu, StringList data)
{
    for (int i=0; i<data.size(); i++) {
        if (data[i] == "submenu") {
            i += 1;
            menu->addMenu(menuHash[data[i].c_str()]);
            continue;
        }
        if (data[i] == "icon") {
            i += 1;
            menu->setIcon(createIcon(data[i].c_str()));
            continue;
        }
        if (data[i] == "---") {
            menu->addSeparator();
            continue;
        }
        menu->addAction(actionHash.value(data[i].c_str()));
    }
    /* Do not allow the Window Menu to be torn off.
     * It's a pain in the ass to maintain.
     */
    menu->setTearOffEnabled(false);
}

/**
 *
 */
void
MainWindow::createAllMenus(void)
{
    debug("MainWindow createAllMenus()");

    /* We loop twice through the menu data so all the submenus exist before
     * they are attached the leaf above.
     */
    for (auto iter=menu_table.begin(); iter!=menu_table.end(); iter++) {
        const char *key = iter->first.c_str();
        const char *label = qPrintable(iter->second.label);
        menuHash[key] = new QMenu(tr(label), this);
    }

    /* Populate the menus. */
    for (auto iter=menu_table.begin(); iter!=menu_table.end(); iter++) {
        const char *key = iter->first.c_str();
        if (iter->second.top_level) {
            menuBar()->addMenu(menuHash[key]);
        }
        addToMenu(menuHash[key], iter->second.entries);
    }

    /* Deal with dynamic menus with call backs. */
    connect(menuHash["RECENT"], SIGNAL(aboutToShow()), this, SLOT(recentMenuAboutToShow()));
    connect(menuHash["WINDOW"], SIGNAL(aboutToShow()), this, SLOT(windowMenuAboutToShow()));
}

/**
 * @brief Construct a new toolbar.
 */
void
MainWindow::addToToolbar(const char *toolbar, const char *name, const char *list)
{
    toolbarHash[toolbar]->setObjectName(name);
    for (int i=0; i<string_tables[list].size(); i++) {
        std::string value = string_tables[list][i];
        if (value == "---") {
            toolbarHash[toolbar]->addSeparator();
        }
        else {
            toolbarHash[toolbar]->addAction(actionHash.value(value.c_str()));
        }
    }
    connect(toolbarHash[toolbar], SIGNAL(topLevelChanged(bool)), this,
        SLOT(floatingChangedToolBar(bool)));
}

//NOTE: Qt4.7 wont load icons without an extension...
QIcon
MainWindow::createIcon(QString label)
{
    QString appDir = qApp->applicationDirPath();
    QString icontheme = st[ST_ICON_THEME].s;
    return QIcon(appDir + "/icons/" + icontheme + "/" + label + ".png");
}

/**
 * @todo switch prompt to some other focus proxy
 * @todo some kind of type awareness of data column
 */
void
MainWindow::addToComboBox(QComboBox *box, StringList data)
{
    box->setFocusProxy(prompt);
    for (int i=0; i<data.size()/3; i++) {
        if (data[3*i+2].c_str() != "") {
            box->addItem(createIcon(data[3*i].c_str()),
                data[3*i+1].c_str(),
                atof(data[3*i+2].c_str()));
        }
        else {
            box->addItem(createIcon(data[3*i].c_str()), data[3*i+1].c_str());
        }
    }
}

void
MainWindow::place_toolbars(Qt::ToolBarArea toolbar_area, const char *toolbar_list)
{
    for (int i=0; i<string_tables[toolbar_list].size(); i++) {
        std::string key = string_tables[toolbar_list][i];
        if (key == "---") {
            addToolBarBreak(toolbar_area);
        }
        else {
            addToolBar(toolbar_area, toolbarHash[key.c_str()]);
        }
    }
}

void
MainWindow::createAllToolbars()
{
    debug("MainWindow createAllToolbars()");

    addToToolbar("FILE", "toolbarFile", "file_toolbar_list");
    addToToolbar("EDIT", "toolbarEdit", "edit_toolbar_list");
    addToToolbar("VIEW", "toolbarView", "view_toolbar_list");
    addToToolbar("ZOOM", "toolbarZoom", "zoom_toolbar_list");
    addToToolbar("PAN", "toolbarPan", "pan_toolbar_list");
    addToToolbar("ICON", "toolbarIcon", "icon_toolbar_list");
    addToToolbar("HELP", "toolbarHelp", "help_toolbar_list");
    addToToolbar("DRAW", "toolbarDraw", "draw_toolbar_list");
    addToToolbar("INQUIRY", "toolbarInquiry", "inquiry_toolbar_list");
    addToToolbar("MODIFY", "toolbarModify", "modify_toolbar_list");
    addToToolbar("DIMENSION", "toolbarDimension",  "dimension_toolbar_list");

    toolbarHash["LAYER"]->setObjectName("toolbarLayer");
    toolbarHash["LAYER"]->addAction(actionHash.value("makelayercurrent"));
    toolbarHash["LAYER"]->addAction(actionHash.value("layers"));

    addToComboBox(layerSelector, string_tables["layer_selector_list"]);
    toolbarHash["LAYER"]->addWidget(layerSelector);
    connect(layerSelector, SIGNAL(currentIndexChanged(int)), this, SLOT(layerSelectorIndexChanged(int)));

    toolbarHash["LAYER"]->addAction(actionHash.value("layerprevious"));
    connect(toolbarHash["LAYER"], SIGNAL(topLevelChanged(bool)), this, SLOT(floatingChangedToolBar(bool)));

    toolbarHash["PROPERTIES"]->setObjectName("toolbarProperties");
    addToComboBox(colorSelector, string_tables["color_selector_list"]);
    toolbarHash["PROPERTIES"]->addWidget(colorSelector);
    connect(colorSelector, SIGNAL(currentIndexChanged(int)), this, SLOT(colorSelectorIndexChanged(int)));

    toolbarHash["PROPERTIES"]->addSeparator();
    addToComboBox(linetypeSelector, string_tables["linetype_selector_list"]);
    toolbarHash["PROPERTIES"]->addWidget(linetypeSelector);
    connect(linetypeSelector, SIGNAL(currentIndexChanged(int)), this, SLOT(linetypeSelectorIndexChanged(int)));

    toolbarHash["PROPERTIES"]->addSeparator();
    addToComboBox(lineweightSelector, string_tables["lineweight_selector_list"]);
    lineweightSelector->setMinimumContentsLength(8); // Prevent dropdown text readability being squish...d.
    toolbarHash["PROPERTIES"]->addWidget(lineweightSelector);
    connect(lineweightSelector, SIGNAL(currentIndexChanged(int)), this, SLOT(lineweightSelectorIndexChanged(int)));
    connect(toolbarHash["PROPERTIES"], SIGNAL(topLevelChanged(bool)), this, SLOT(floatingChangedToolBar(bool)));

    toolbarHash["TEXT"]->setObjectName("toolbarText");
    toolbarHash["TEXT"]->addWidget(textFontSelector);
    textFontSelector->setCurrentFont(QFont(st[ST_TEXT_FONT].s));
    connect(textFontSelector, SIGNAL(currentFontChanged(const QFont&)), this, SLOT(textFontSelectorCurrentFontChanged(const QFont&)));

    toolbarHash["TEXT"]->addAction(actionHash.value("textbold"));
    actionHash.value("textbold")->setChecked(st[ST_TEXT_BOLD].b);
    toolbarHash["TEXT"]->addAction(actionHash.value("textitalic"));
    actionHash.value("textitalic")->setChecked(st[ST_TEXT_ITALIC].b);
    toolbarHash["TEXT"]->addAction(actionHash.value("textunderline"));
    actionHash.value("textunderline")->setChecked(st[ST_TEXT_UNDERLINE].b);
    toolbarHash["TEXT"]->addAction(actionHash.value("textstrikeout"));
    actionHash.value("textstrikeout")->setChecked(st[ST_TEXT_STRIKEOUT].b);
    toolbarHash["TEXT"]->addAction(actionHash.value("textoverline"));
    actionHash.value("textoverline")->setChecked(st[ST_TEXT_OVERLINE].b);

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
    char command[200];
    sprintf(command, "set_text_size(%d)", st[ST_TEXT_SIZE].i);
    cmd(command);
    toolbarHash["TEXT"]->addWidget(textSizeSelector);
    connect(textSizeSelector, SIGNAL(currentIndexChanged(int)), this, SLOT(textSizeSelectorIndexChanged(int)));

    connect(toolbarHash["TEXT"], SIGNAL(topLevelChanged(bool)), this, SLOT(floatingChangedToolBar(bool)));

    toolbarHash["PROMPT"]->setObjectName("toolbarPrompt");
    toolbarHash["PROMPT"]->addWidget(prompt);
    toolbarHash["PROMPT"]->setAllowedAreas(Qt::TopToolBarArea | Qt::BottomToolBarArea);
    connect(toolbarHash["PROMPT"], SIGNAL(topLevelChanged(bool)), prompt, SLOT(floatingChanged(bool)));

    // Horizontal
    toolbarHash["VIEW"]->setOrientation(Qt::Horizontal);
    toolbarHash["ZOOM"]->setOrientation(Qt::Horizontal);
    toolbarHash["LAYER"]->setOrientation(Qt::Horizontal);
    toolbarHash["PROPERTIES"]->setOrientation(Qt::Horizontal);
    toolbarHash["TEXT"]->setOrientation(Qt::Horizontal);
    toolbarHash["PROMPT"]->setOrientation(Qt::Horizontal);

    toolbarHash["DRAW"]->setOrientation(Qt::Vertical);
    toolbarHash["MODIFY"]->setOrientation(Qt::Vertical);
    toolbarHash["DIMENSION"]->setOrientation(Qt::Vertical);
    toolbarHash["INQUIRY"]->setOrientation(Qt::Vertical);

    addToolBarBreak(Qt::TopToolBarArea);
    place_toolbars(Qt::TopToolBarArea, "top_toolbar_order");
    place_toolbars(Qt::BottomToolBarArea, "bottom_toolbar_order");
    place_toolbars(Qt::LeftToolBarArea, "left_toolbar_order");

    //zoomToolBar->setToolButtonStyle(Qt::ToolButtonTextOnly);
}

// Note: on Unix we include the trailing separator. For Windows compatibility we omit it.
QString SettingsDir()
{
#if defined(Q_OS_UNIX) || defined(Q_OS_MAC)
    QString homePath = QDir::homePath();
    return homePath + "/.embroidermodder2/";
#else
    return "";
#endif
}

QString SettingsPath()
{
    QString settingsPath = SettingsDir() + "settings.ini";
    return settingsPath;
}

void MainWindow::readSettings()
{
    debug("Reading Settings...");
    // This file needs to be read from the users home directory to ensure it is writable
    QString settingsPath = SettingsPath();
    QString settingsDir = SettingsDir();
    QString appDir = qApp->applicationDirPath();
    QSettings settings_file(settingsPath, QSettings::IniFormat);
    QPoint pos = settings_file.value("Window/Position", QPoint(0, 0)).toPoint();
    QSize size = settings_file.value("Window/Size", QSize(800, 600)).toSize();

    layoutState = settings_file.value("LayoutState").toByteArray();
    if (!restoreState(layoutState)) {
        debug("LayoutState NOT restored! Setting Default Layout...");
        //someToolBar->setVisible(true);
    }

    for (int i=0; i<N_SETTINGS; i++) {
        QString key_(settings_table[i].section);
        key_ += QString("/") + QString(settings_table[i].key.c_str());
        switch (settings_table[i].type) {
        case 's': {
            const char *s = st[i].s;
            strncpy(st[i].s,
                qPrintable(settings_file.value(key_, s).toString()),
                200);
            break;
        }
        case 'r':
            st[i].r = settings_file.value(key_, st[i].r).toFloat();
            break;
        case 'i':
            st[i].i = settings_file.value(key_, st[i].i).toInt();
            break;
        case 'u':
        case 'c':
            st[i].u = settings_file.value(key_, st[i].u).toInt();
            break;
        case 'b':
            st[i].b = settings_file.value(key_, st[i].b).toBool();
            break;
        default:
            debug("ERROR: unknown settings type starting with the character %c.",
                settings_table[i].type);
            break;
        }
    }

    move(pos);
    resize(size);
}

void MainWindow::writeSettings()
{
    debug("Writing Settings...");
    QString settingsPath = SettingsPath();
    // This file needs to be read from the users home directory to ensure it is writable
    QSettings settings_file(settingsPath, QSettings::IniFormat);
    QString tmp;
    settings_file.setValue("Window/Position", pos());
    settings_file.setValue("Window/Size", size());

    for (int i=0; i<N_SETTINGS; i++) {
        QString key_(settings_table[i].section);
        key_ += QString("/") + QString(settings_table[i].key.c_str());
        switch (settings_table[i].type) {
        case 's': {
            const char *s = st[i].s;
            settings_file.setValue(key_, s);
            break;
        }
        case 'i':
            settings_file.setValue(key_, tmp.setNum(st[i].i));
            break;
        case 'u':
        case 'c':
            settings_file.setValue(key_, tmp.setNum(st[i].u));
            break;
        case 'r':
            settings_file.setValue(key_, tmp.setNum(st[i].r));
            break;
        case 'b':
            settings_file.setValue(key_, st[i].b);
            break;
        default:
            debug("ERROR: unknown settings type starting with the character %c.",
                settings_table[i].type);
            break;
        }
    }
}

void MainWindow::settingsPrompt()
{
    settingsDialog("Prompt");
}

void MainWindow::settingsDialog(const QString& showTab)
{
    Settings_Dialog dialog(showTab, this);
    dialog.exec();
}

