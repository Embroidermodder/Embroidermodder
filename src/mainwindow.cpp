/*
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
    app.setApplicationName(_appName_);
    app.setApplicationVersion(_appVer_);

    QStringList filesToOpen;

    for (int i = 1; i < argc; i++) {
        if (!strcmp(argv[i], "-d") || !strcmp(argv[i], "--debug")  ) {
        }
        else if (!strcmp(argv[i], "-h") || !strcmp(argv[i], "--help")   ) {
            fprintf(stdout, "%s", usage_msg);
            exitApp = true;
        }
        else if (!strcmp(argv[i], "-v") || !strcmp(argv[i], "--version")) {
            fprintf(stdout, "%s %s\n", _appName_, _appVer_);
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
get_id(char *table[], const char *cmd)
{
    for (int i=0; table[i][0]!='_'; i++) {
        if (!strncmp(table[i], cmd, 50)) {
            return i;
        }
    }
    return -1;
}

/* Wrapper for C calls of the main command processor. */
void
run_cmd(const char *line)
{
    _mainWin->cmd(line);
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
 */
void
MainWindow::cmd(const char *line)
{
    prompt->appendHistory(line);

    QString qstr(line);
    QStringList list = qstr.split(" ");
    const char *cmd = qPrintable(list[0]);
    if (aliases.find(list[0]) != aliases.end()) {
        cmd = qPrintable(aliases.at(list[0]));
    }
    int id = get_id(state.command_names, cmd);
    if (id < 0) {
        debug("ERROR: unrecognised command id.");
        return;
    }

    if (state.debug > 0) {
        debug("COMMAND %s", state.command_names[id]);
    }

    switch (id) {
    /* This action intentionally does nothing. */
    case CMD_NULL:
        break;

    /* Show the about dialog. */
    case CMD_ABOUT:
        about();
        break;

    case CMD_STUB:
        QMessageBox::warning(this, tr("Testing Feature"),
            tr("<b>This feature is in testing.</b>"));
        break;

    case CMD_CUT: {
        View* gview = activeView();
        if (gview) {
            gview->cut();
        }
        break;
    }

    case CMD_COPY: {
        View* gview = activeView();
        if (gview) {
            gview->copy();
        }
        break;
    }

    case CMD_PASTE: {
        View* gview = activeView();
        if (gview) {
            gview->paste();
        }
        break;
    }

    case CMD_SELECT_ALL: {
        View* gview = activeView();
        if (gview) {
            gview->selectAll();
        }
        break;
    }

    case CMD_DETAILS: {
        QGraphicsScene* scene = activeScene();
        if (scene) {
            EmbDetailsDialog dialog(scene, this);
            dialog.exec();
        }
        break;
    }


    case CMD_UPDATES: {
        debug("TODO: Check website for new versions, commands, etc...");
        break;
    }

    case CMD_WHATS_THIS: {
        QWhatsThis::enterWhatsThisMode();
        break;
    }

    case CMD_PRINT: {
        MdiWindow* mdiWin = qobject_cast<MdiWindow*>(mdiArea->activeSubWindow());
        if (mdiWin == NULL) {
            debug("ERROR: No active window for printing.");
            break;
        }
        View *gview = activeView();

        QPrintDialog dialog(&printer, mdiWin);
        if (dialog.exec() == QDialog::Accepted) {
            QPainter painter(&printer);
            if (st[ST_PRINTING_DISABLE_BG].b) {
                //Save current bg
                QBrush brush = gview->backgroundBrush();
                //Save ink by not printing the bg at all
                gview->setBackgroundBrush(Qt::NoBrush);
                //Print, fitting the viewport contents into a full page
                gview->render(&painter);
                //Restore the bg
                gview->setBackgroundBrush(brush);
            }
            else {
                //Print, fitting the viewport contents into a full page
                gview->render(&painter);
            }
        }
        break;
    }

    case CMD_HELP: {
        // Open the HTML Help in the default browser
        QUrl helpURL("file:///" + qApp->applicationDirPath() + "/help/doc-index.html");
        QDesktopServices::openUrl(helpURL);

        /* TODO: This is how to start an external program. Use this elsewhere...
         * QString program = "firefox";
         * QStringList arguments;
         * arguments << "help/commands.html";
         * QProcess *myProcess = new QProcess(this);
         * myProcess->start(program, arguments);
         */
        break;
    }

    case CMD_CHANGELOG: {
        QUrl changelogURL("help/changelog.html");
        QDesktopServices::openUrl(changelogURL);
        break;
    }

    case CMD_UNDO: {
        QString prefix = prompt->getPrefix();
        if (dockUndoEdit->canUndo()) {
            prompt->setPrefix("Undo " + dockUndoEdit->undoText());
            prompt->appendHistory(QString());
            dockUndoEdit->undo();
            prompt->setPrefix(prefix);
        }
        else {
            prompt->alert("Nothing to undo");
            prompt->setPrefix(prefix);
        }
        break;
    }

    case CMD_REDO: {
        QString prefix = prompt->getPrefix();
        if (dockUndoEdit->canRedo()) {
            prompt->setPrefix("Redo " + dockUndoEdit->redoText());
            prompt->appendHistory(QString());
            dockUndoEdit->redo();
            prompt->setPrefix(prefix);
        }
        else {
            prompt->alert("Nothing to redo");
            prompt->setPrefix(prefix);
        }
        break;
    }

    case CMD_REPEAT: {
        break;
    }

    case CMD_ICON16: {
        iconResize(16);
        break;
    }

    case CMD_ICON24: {
        iconResize(24);
        break;
    }

    case CMD_ICON32: {
        iconResize(32);
        break;
    }

    case CMD_ICON48: {
        iconResize(48);
        break;
    }

    case CMD_ICON64: {
        iconResize(64);
        break;
    }

    case CMD_ICON128: {
        iconResize(128);
        break;
    }

    case CMD_PLAY: {
        state.play_mode = 1;
        state.simulation_start = current_time();
        break;
    }

    case CMD_STOP: {
        state.play_mode = 0;
        break;
    }

    case CMD_SLEEP: {
        std::this_thread::sleep_for(100ms);
        break;
    }

    case CMD_NEW: {
        state.docIndex++;
        state.numOfDocs++;
        MdiWindow* mdiWin = new MdiWindow(state.docIndex, _mainWin, mdiArea, Qt::SubWindow);
        connect(mdiWin, SIGNAL(sendCloseMdiWin(MdiWindow*)), this,
            SLOT(onCloseMdiWin(MdiWindow*)));
        connect(mdiArea, SIGNAL(subWindowActivated(QMdiSubWindow*)), this,
            SLOT(onWindowActivated(QMdiSubWindow*)));

        updateMenuToolbarStatusbar();
        windowMenuAboutToShow();

        View* v = mdiWin->gview;
        if (v) {
            v->recalculateLimits();
            v->zoomExtents();
        }
        break;
    }

    case CMD_OPEN: {
        if (list.size() == 1) {
            openFile();
            break;
        }
        list.remove(0);
        openFilesSelected(list);
        break;
    }

    case CMD_SAVE: {
        /* FIXME */
        break;
    }

    case CMD_SAVE_AS: {
        /* need to find the activeSubWindow before it loses focus to the FileDialog. */
        MdiWindow* mdiWin = qobject_cast<MdiWindow*>(mdiArea->activeSubWindow());
        if (!mdiWin) {
            return;
        }

        openFilesPath = st[ST_RECENT_DIRECTORY].s;
        QString file = QFileDialog::getSaveFileName(this, tr("Save As"),
            openFilesPath, formatFilterSave);

        mdiWin->saveFile(file);
        break;
    }

    /* Moves the view to the left. */
    case CMD_PAN_LEFT: {
        View* gview = activeView();
        QUndoStack* stack = gview->getUndoStack();
        if (gview && stack) {
            UndoableNavCommand* cmd = new UndoableNavCommand("PanLeft", gview, 0);
            stack->push(cmd);
        }
        break;
    }

    /* Moves the view down. */
    case CMD_PAN_DOWN: {
        View* gview = activeView();
        QUndoStack* stack = gview->getUndoStack();
        if (gview && stack) {
            UndoableNavCommand* cmd = new UndoableNavCommand("PanDown", gview, 0);
            stack->push(cmd);
        }
        break;
    }

    /* Moves the view to the right. */
    case CMD_PAN_RIGHT: {
        View* gview = activeView();
        QUndoStack* stack = gview->getUndoStack();
        if (gview && stack) {
            UndoableNavCommand* cmd = new UndoableNavCommand("PanRight", gview, 0);
            stack->push(cmd);
        }
        break;
    }

    /* Moves the view up. */
    case CMD_PAN_UP: {
        View* gview = activeView();
        QUndoStack* stack = gview->getUndoStack();
        if (gview && stack) {
            UndoableNavCommand* cmd = new UndoableNavCommand("PanUp", gview, 0);
            stack->push(cmd);
        }
        break;
    }

    /* . */
    case CMD_PAN_POINT: {
        View* gview = activeView();
        if (gview) {
            gview->panPoint();
        }
        break;
    }

    /* . */
    case CMD_PAN_REAL_TIME: {
        View* gview = activeView();
        if (gview) {
            gview->panRealTime();
        }
        break;
    }

    /* . */
    case CMD_WINDOW_CASCADE: {
        mdiArea->cascade();
        break;
    }

    /* . */
    case CMD_WINDOW_CLOSE: {
        _mainWin->onCloseWindow();
        break;
    }

    /* . */
    case CMD_WINDOW_CLOSE_ALL: {
        mdiArea->closeAllSubWindows();
        break;
    }

    /* . */
    case CMD_WINDOW_NEXT: {
        mdiArea->activateNextSubWindow();
        break;
    }

    /* . */
    case CMD_WINDOW_PREVIOUS: {
        mdiArea->activatePreviousSubWindow();
        break;
    }

    /* . */
    case CMD_WINDOW_TILE: {
        mdiArea->tile();
        break;
    }

    /* . */
    case CMD_ZOOM_ALL: {
        debug("TODO: Implement zoomAll.");
        break;
    }

    /* . */
    case CMD_ZOOM_CENTER: {
        debug("TODO: Implement zoomCenter.");
        break;
    }

    /* . */
    case CMD_ZOOM_DYNAMIC: {
        debug("TODO: Implement zoomDynamic.");
        break;
    }

    /* Zooms to display the drawing extents. */
    case CMD_ZOOM_EXTENTS: {
        View* gview = activeView();
        QUndoStack* stack = gview->getUndoStack();
        if (gview && stack) {
            UndoableNavCommand* cmd = new UndoableNavCommand("ZoomExtents", gview, 0);
            stack->push(cmd);
        }
        break;
    }

    /* Zooms to increase the apparent size of objects. */
    case CMD_ZOOM_IN: {
        View* gview = activeView();
        if (gview) {
            gview->zoomIn();
        }
        break;
    }

    /* Zooms to decrease the apparent size of objects. */
    case CMD_ZOOM_OUT: {
        View* gview = activeView();
        if (gview) {
            gview->zoomOut();
        }
        break;
    }

    /* . */
    case CMD_ZOOM_SCALE: {
        debug("TODO: Implement zoomScale.");
        break;
    }

    /* . */
    case CMD_ZOOM_PREVIOUS: {
        debug("TODO: Implement zoomPrevious.");
        break;
    }

    /* . */
    case CMD_ZOOM_REAL_TIME: {
        debug("TODO: Implement zoomRealtime.");
        break;
    }

    /* . */
    case CMD_ZOOM_SELECTED: {
        View* gview = activeView();
        QUndoStack* stack = gview->getUndoStack();
        if (gview && stack) {
            UndoableNavCommand* cmd = new UndoableNavCommand("ZoomSelected", gview, 0);
            stack->push(cmd);
        }
        break;
    }

    /* . */
    case CMD_ZOOM_WINDOW: {
        View* gview = activeView();
        if (gview) {
            gview->zoomWindow();
        }
        break;
    }

    /* TODO: Make day vision color settings. */
    case CMD_DAY: {
        View* gview = activeView();
        if (gview) {
            gview->setBackgroundColor(qRgb(255,255,255)); 
            gview->setCrossHairColor(qRgb(0,0,0));
            gview->setGridColor(qRgb(0,0,0));
        }
        break;
    }

    /* TODO: Make night vision color settings. */
    case CMD_NIGHT: {
        View* gview = activeView();
        if (gview) {
            gview->setBackgroundColor(qRgb(0,0,0));
            gview->setCrossHairColor(qRgb(255,255,255));
            gview->setGridColor(qRgb(255,255,255));
        }
        break;
    }

    case CMD_CLEAR_RUBBER: {
        View* gview = activeView();
        if (gview) {
            gview->clearRubberRoom();
        }
        break;
    }

    case CMD_CLEAR_SELECTION: {
        View* gview = activeView();
        if (gview) {
            gview->clearSelection();
        }
        break;
    }

    case CMD_END: {
        View* gview = activeView();
        if (gview) {
            gview->clearRubberRoom();
            gview->previewOff();
            gview->disableMoveRapidFire();
        }
        prompt->endCommand();
        break;
    }

    case CMD_EXIT: {
        if (st[ST_PROMPT_SAVE_HISTORY].b) {
            /* TODO: get filename from settings. */
            prompt->saveHistory("prompt.log", st[ST_PROMPT_SAVE_AS_HTML].b);
        }
        qApp->closeAllWindows();
        /* Force the MainWindow destructor to run before exiting.
         * Makes Valgrind "still reachable" happy :)
         */
        _mainWin->deleteLater();
        break;
    }

    case CMD_MACRO: {
        debug("TODO: macro support");
        break;
    }

    case CMD_SCRIPT: {
        run(qPrintable(list[1]));
        break;
    }

    case CMD_SETTINGS: {
        settingsDialog("General");
        break;
    }

    /** @todo Report the value. */
    case CMD_GET: {
        ScriptValue value = get(qPrintable(list[1]));
        break;
    }

    case CMD_SET: {
        ScriptValue value;
        strncpy(value.s, qPrintable(list[2]), 200);
        set(qPrintable(list[1]), value);
        break;
    }

    /** Add a EmbTextMulti object to the design.
     * @todo argument parsing
     */
    case CMD_TEXT_MULTI: {
        const QString& str = "Lorem ipsum\ndolor sit amet,";
        EmbVector position = emb_vector(10.0f, 10.0f);
        EmbReal rot = 0.0f;
        bool fill = false;
        int rubberMode = OBJ_RUBBER_OFF;
        add_text_multi(str, position, rot, fill, rubberMode);
        break;
    }

    /** @todo argument parsing
     */
    case CMD_TEXT_SINGLE: {
        const QString& str = "Lorem ipsum dolor sit amet,";
        EmbVector position = emb_vector(10.0f, 10.0f);
        EmbReal rot = 0.0f;
        bool fill = false;
        int rubberMode = OBJ_RUBBER_OFF;
        add_text_single(str, position, rot, fill, rubberMode);
        break;
    }

    /** @todo argument parsing
     */
    case CMD_INFINITE_LINE: {
        EmbVector point1 = emb_vector(0.0f, 0.0f);
        EmbVector point2 = emb_vector(0.0f, 10.0f);
        EmbReal rot = 0.0f;
        add_infinite_line(point1, point2, rot);
        break;
    }

    /** @todo argument parsing
     */
    case CMD_RAY: {
        EmbVector start = emb_vector(0.0f, 0.0f);
        EmbVector point = emb_vector(0.0f, 10.0f);
        EmbReal rot = 0.0f;
        add_ray(start, point, rot);
        break;
    }

    /** @todo argument parsing
     */
    case CMD_LINE: {
        EmbVector start = emb_vector(0.0f, 0.0f);
        EmbVector end = emb_vector(0.0f, 10.0f);
        EmbReal rot = 0.0f;
        int rubberMode = OBJ_RUBBER_OFF;
        add_line(start, end, rot, rubberMode);
        break;
    }

    /** @todo argument parsing
     */
    case CMD_TRIANGLE: {
        EmbVector point1 = emb_vector(0.0f, 0.0f);
        EmbVector point2 = emb_vector(0.0f, 10.0f);
        EmbVector point3 = emb_vector(0.0f, 10.0f);
        EmbReal rot = 0.0f;
        bool fill = false;
        add_triangle(point1, point2, point3, rot, fill);
        break;
    }

    /** @todo argument parsing
     */
    case CMD_RECTANGLE: {
        EmbReal x = 0.0f;
        EmbReal y = 0.0f;
        EmbReal w = 10.0f;
        EmbReal h = 20.0f;
        EmbReal rot = 0.0f;
        bool fill = false;
        int rubberMode = OBJ_RUBBER_OFF;
        add_rectangle(x, y, w, h, rot, fill, rubberMode);
        break;
    }

    /** @todo argument parsing
     */
    case CMD_ROUNDED_RECTANGLE: {
        EmbReal x = 0.0f;
        EmbReal y = 0.0f;
        EmbReal w = 10.0f;
        EmbReal h = 20.0f;
        EmbReal rad = 2.0f;
        EmbReal rot = 0.0f;
        bool fill = false;
        add_rounded_rectangle(x, y, w, h, rad, rot, fill);
        break;
    }

    /** @todo argument parsing
     */
    case CMD_ARC: {
        EmbVector start = emb_vector(0.0f, 0.0f);
        EmbVector mid = emb_vector(10.0f, 0.0f);
        EmbVector end = emb_vector(10.0f, 10.0f);
        add_arc(start, mid, end, OBJ_RUBBER_OFF);
        break;
    }

    /** @todo argument parsing
     */
    case CMD_CIRCLE: {
        EmbVector center = emb_vector(0.0f, 0.0f);
        EmbReal radius = 10.0;
        bool fill = false;
        int rubberMode = OBJ_RUBBER_OFF;
        add_circle(center, radius, fill, rubberMode);
        break;
    }

    /** @todo argument parsing
     */
    case CMD_SLOT: {
        EmbVector center = emb_vector(0.0f, 0.0f);
        EmbReal diameter = 1.0;
        EmbReal length = 10.0;
        EmbReal rot = 0.0;
        bool fill = false;
        int rubberMode = OBJ_RUBBER_OFF;
        add_slot(center, diameter, length, rot, fill, rubberMode);
        break;
    }

    /** @todo argument parsing
     */
    case CMD_ELLIPSE: {
        EmbVector center = emb_vector(0.0f, 0.0f);
        EmbReal width = 10.0;
        EmbReal height = 30.0;
        EmbReal rot = 1.0;
        bool fill = false;
        int rubberMode = OBJ_RUBBER_OFF;
        add_ellipse(center, width, height, rot, fill, rubberMode);
        break;
    }

    /** @todo argument parsing
     */
    case CMD_POINT: {
        EmbVector position = emb_vector(10.0f, 10.0f);
        add_point(position);
        break;
    }

    /** @todo argument parsing
     */
    case CMD_REGULAR_POLYGON: {
        EmbVector center = emb_vector(10.0f, 10.0f);
        int sides = 5;
        int mode = 0;
        EmbReal rad = 10.0;
        EmbReal rot = 0.0;
        bool fill = false;
        add_regular_polygon(center, sides, mode, rad, rot, fill);
        break;
    }

    /** @todo argument parsing
     */
    case CMD_POLYGON: {
        EmbVector start = emb_vector(0.0f, 0.0f);
        QPainterPath p;
        int rubberMode = OBJ_RUBBER_OFF;
        add_polygon(start, p, rubberMode);
        break;
    }

    /** @todo argument parsing
     */
    case CMD_POLYLINE: {
        EmbVector start = emb_vector(0.0f, 0.0f);
        QPainterPath p;
        int rubberMode = OBJ_RUBBER_OFF;
        add_polyline(start, p, rubberMode);
        break;
    }

    /** @todo argument parsing
     */
    case CMD_PATH: {
        EmbVector start = emb_vector(0.0f, 0.0f);
        QPainterPath p;
        int rubberMode = OBJ_RUBBER_OFF;
        add_path(start, p, rubberMode);
        break;
    }

    /** @todo argument parsing
     */
    case CMD_HORIZONTAL_DIM: {
        EmbVector start = emb_vector(8.0f, 12.0f);
        EmbVector end = emb_vector(18.0f, 11.0f);
        EmbReal legHeight = 10.0f;
        add_horizontal_dimension(start, end, legHeight);
        break;
    }

    /** @todo argument parsing
     */
    case CMD_VERTICAL_DIM: {
        EmbVector start = emb_vector(-8.0f, 10.0f);
        EmbVector end = emb_vector(1.0f, 13.0f);
        EmbReal legHeight = 10.0f;
        add_vertical_dimension(start, end, legHeight);
        break;
    }

    /** @todo argument parsing
     */
    case CMD_IMAGE: {
        QString img = "icons/default/app.png";
        double x = 10.0;
        double y = 10.0;
        double w = 30.0;
        double h = 20.0;
        double rot = 10.0;
        add_image(img, x, y, w, h, rot);
        break;
    }

    /** @todo argument parsing
     */
    case CMD_DIM_LEADER: {
        EmbVector start = emb_vector(8.0f, 1.0f);
        EmbVector end = emb_vector(1.0f, 3.0f);
        EmbReal rot = 10.0;
        int rubberMode = OBJ_RUBBER_OFF;
        add_dim_leader(start.x, start.y, end.x, end.y, rot, rubberMode);
        break;
    }

    case CMD_GENERATE: {
        generate(qPrintable(list[1]));
        break;
    }

    case CMD_FILL: {
        fill(qPrintable(list[1]));
        break;
    }

    default:
        printf("ERROR: unrecognised command id %d\n", id);
        break;
    }
}

/*!
 */
ScriptValue
MainWindow::get(const char *key)
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
        strncpy(result.s, qPrintable(platformString()), 200);
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

/*!
 */
void
MainWindow::set(const char *key, ScriptValue value)
{
    if (!strncmp(key, "text_angle", 20)) {
        st[ST_TEXT_ANGLE].r = value.r;
        return;
    }
    if (!strncmp(key, "text_size", 20)) {
        double num = value.i;

        if (std::isnan(num)) {
            debug("TypeError, setTextSize(): first argument failed isNaN check.");
            return;
        }

        st[ST_TEXT_SIZE].i = std::fabs(num);
        int index = textSizeSelector->findText("Custom", Qt::MatchContains);
        if (index != -1) {
            textSizeSelector->removeItem(index);
        }
        textSizeSelector->addItem("Custom " + QString().setNum(num, 'f', 2) + " pt", num);
        index = textSizeSelector->findText("Custom", Qt::MatchContains);
        if (index != -1) {
            textSizeSelector->setCurrentIndex(index);
        }
        return;
    }
    if (!strncmp(key, "text_bold", 20)) {
        st[ST_TEXT_BOLD].b = value.b;
        return;
    }
    if (!strncmp(key, "text_italic", 20)) {
        st[ST_TEXT_ITALIC].b = value.b;
        return;
    }
    if (!strncmp(key, "text_underline", 20)) {
        st[ST_TEXT_UNDERLINE].b = value.b;
        return;
    }
    if (!strncmp(key, "text_strikeout", 20)) {
        st[ST_TEXT_STRIKEOUT].b = value.b;
        return;
    }
    if (!strncmp(key, "text_overline", 20)) {
        st[ST_TEXT_OVERLINE].b = value.b;
        return;
    }
    if (!strncmp(key, "prefix", 20)) {
        prompt->setPrefix(value.s);
        return;
    }
}

MainWindow::MainWindow() : QMainWindow(0)
{
    _mainWin = this;

    readSettings();

    QString appDir = qApp->applicationDirPath();
    //Verify that files/directories needed are actually present.
    QStringList folders = {
        "scripts",
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

void MainWindow::recentMenuAboutToShow()
{
    debug("MainWindow::recentMenuAboutToShow()");
    menuHash["RECENT"]->clear();

    QFileInfo recentFileInfo;
    QString recentValue;
    for (int i = 0; i < st[ST_RECENT_FILES].length; ++i) {
        /* If less than the max amount of entries add to menu. */
        if (i >= st[ST_RECENT_MAX_FILES].i) {
            break;
        }
        recentFileInfo = QFileInfo(st[ST_RECENT_FILES].l[i]);
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
            const char *s = st[ST_RECENT_FILES].l[i];
            rAction->setData(s);
            menuHash["RECENT"]->addAction(rAction);
            connect(rAction, SIGNAL(triggered()), this, SLOT(openrecentfile()));
        }
    }
    /* Ensure the list only has max amount of entries. */
    if (st[ST_RECENT_FILES].length > st[ST_RECENT_MAX_FILES].i) {
        st[ST_RECENT_FILES].length = st[ST_RECENT_MAX_FILES].i;
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
        QString key_(settings_table[i].section.c_str());
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
        QString key_(settings_table[i].section.c_str());
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

