/*
 * Embroidermodder 2
 * Copyright 2011-2025 The Embroidermodder Team
 *
 * Embroidermodder 2 is free and open software under the zlib license:
 * see LICENSE.md for details.
 *
 * ----------------------------------------------------------------------------
 *
 * Definitions of the command functions used by "cmd": all user actions are
 * described here.
 *
 * Please keep the parts of this file not commented out in alphabetical
 * order by function name.
 *
 * All functions should be of the form:
 *
 *     int
 *     *_command(int argc, char *argv[])
 *
 * If the function takes arguments then a table is supplied to describe the
 * arguments. Otherwise the statement "This command takes no arguments." is
 * written at the bottom of the description.
 */

#if 0
    switch (id) {
    case CMD_DETAILS: {
        QGraphicsScene* scene = activeScene();
        if (scene) {
            EmbDetailsDialog dialog(scene, _mainWin);
            dialog.exec();
        }
        break;
    }

    case CMD_PRINT: {
        MdiWindow* mdiWin = qobject_cast<MdiWindow*>(mdiArea->activeSubWindow());
        if (mdiWin == NULL) {
            debug("ERROR: No active window for printing.");
            break;
        }
        View *gview = activeView();

        QPrintDialog dialog(&_mainWin->printer, mdiWin);
        if (dialog.exec() == QDialog::Accepted) {
            QPainter painter(&_mainWin->printer);
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
        _mainWin->iconResize(16);
        break;
    }

    case CMD_ICON24: {
        _mainWin->iconResize(24);
        break;
    }

    case CMD_ICON32: {
        _mainWin->iconResize(32);
        break;
    }

    case CMD_ICON48: {
        _mainWin->iconResize(48);
        break;
    }

    case CMD_ICON64: {
        _mainWin->iconResize(64);
        break;
    }

    case CMD_ICON128: {
        _mainWin->iconResize(128);
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
        new_file_command(argc-1, (char**)argv);
        break;
    }

    case CMD_OPEN: {
        if (list.size() == 1) {
            _mainWin->openFile();
            break;
        }
        list.remove(0);
        _mainWin->openFilesSelected(list);
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

    case CMD_MACRO: {
        debug("TODO: macro support");
        break;
    }

    case CMD_SCRIPT:
        run_command(argc, (char**)argv);
        break;

    case CMD_SETTINGS: {
        _mainWin->settingsDialog("General");
        break;
    }

    /** @todo Report the value. */
    case CMD_GET: {
        ScriptValue value = get(qPrintable(list[1]));
        break;
    }

    case CMD_SET:
        set_command(argc, (char**)argv);
        break;

    /** @todo argument parsing
     */
    case CMD_INFINITE_LINE: {
        EmbVector point1 = emb_vector(0.0f, 0.0f);
        EmbVector point2 = emb_vector(0.0f, 10.0f);
        EmbReal rot = 0.0f;
        _mainWin->add_infinite_line(point1, point2, rot);
        break;
    }

    /** @todo argument parsing
     */
    case CMD_RAY: {
        EmbVector start = emb_vector(0.0f, 0.0f);
        EmbVector point = emb_vector(0.0f, 10.0f);
        EmbReal rot = 0.0f;
        _mainWin->add_ray(start, point, rot);
        break;
    }

    /** @todo argument parsing
     */
    case CMD_LINE: {
        EmbVector start = emb_vector(0.0f, 0.0f);
        EmbVector end = emb_vector(0.0f, 10.0f);
        EmbReal rot = 0.0f;
        int rubberMode = OBJ_RUBBER_OFF;
        _mainWin->add_line(start, end, rot, rubberMode);
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
        _mainWin->add_triangle(point1, point2, point3, rot, fill);
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
        _mainWin->add_rectangle(x, y, w, h, rot, fill, rubberMode);
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
        _mainWin->add_rounded_rectangle(x, y, w, h, rad, rot, fill);
        break;
    }

    /** @todo argument parsing
     */
    case CMD_ARC: {
        EmbVector start = emb_vector(0.0f, 0.0f);
        EmbVector mid = emb_vector(10.0f, 0.0f);
        EmbVector end = emb_vector(10.0f, 10.0f);
        _mainWin->add_arc(start, mid, end, OBJ_RUBBER_OFF);
        break;
    }

    /** @todo argument parsing
     */
    case CMD_CIRCLE: {
        EmbVector center = emb_vector(0.0f, 0.0f);
        EmbReal radius = 10.0;
        bool fill = false;
        int rubberMode = OBJ_RUBBER_OFF;
        _mainWin->add_circle(center, radius, fill, rubberMode);
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
        _mainWin->add_slot(center, diameter, length, rot, fill, rubberMode);
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
        _mainWin->add_ellipse(center, width, height, rot, fill, rubberMode);
        break;
    }

    /** @todo argument parsing
     */
    case CMD_POINT: {
        EmbVector position = emb_vector(10.0f, 10.0f);
        _mainWin->add_point(position);
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
        _mainWin->add_regular_polygon(center, sides, mode, rad, rot, fill);
        break;
    }

    /** @todo argument parsing
     */
    case CMD_POLYGON: {
        EmbVector start = emb_vector(0.0f, 0.0f);
        QPainterPath p;
        int rubberMode = OBJ_RUBBER_OFF;
        _mainWin->add_polygon(start, p, rubberMode);
        break;
    }

    /** @todo argument parsing
     */
    case CMD_POLYLINE: {
        EmbVector start = emb_vector(0.0f, 0.0f);
        QPainterPath p;
        int rubberMode = OBJ_RUBBER_OFF;
        _mainWin->add_polyline(start, p, rubberMode);
        break;
    }

    /** @todo argument parsing
     */
    case CMD_PATH: {
        EmbVector start = emb_vector(0.0f, 0.0f);
        QPainterPath p;
        int rubberMode = OBJ_RUBBER_OFF;
        _mainWin->add_path(start, p, rubberMode);
        break;
    }

    /** @todo argument parsing
     */
    case CMD_HORIZONTAL_DIM: {
        EmbVector start = emb_vector(8.0f, 12.0f);
        EmbVector end = emb_vector(18.0f, 11.0f);
        EmbReal legHeight = 10.0f;
        _mainWin->add_horizontal_dimension(start, end, legHeight);
        break;
    }

    /** @todo argument parsing
     */
    case CMD_VERTICAL_DIM: {
        EmbVector start = emb_vector(-8.0f, 10.0f);
        EmbVector end = emb_vector(1.0f, 13.0f);
        EmbReal legHeight = 10.0f;
        _mainWin->add_vertical_dimension(start, end, legHeight);
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
        _mainWin->add_image(img, x, y, w, h, rot);
        break;
    }

    /** @todo argument parsing
     */
    case CMD_DIM_LEADER: {
        EmbVector start = emb_vector(8.0f, 1.0f);
        EmbVector end = emb_vector(1.0f, 3.0f);
        EmbReal rot = 10.0;
        int rubberMode = OBJ_RUBBER_OFF;
        _mainWin->add_dim_leader(start.x, start.y, end.x, end.y, rot, rubberMode);
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

#endif

#include <math.h>

#include "embroidermodder.h"

/*
 * Shows the about dialog.
 *
 * This command takes no arguments.
 */
int
about_command(int argc, char *argv[])
{
    no_arguments("about_command", argc, argv);
    _mainWin->about();
    return 0;
}

/*
 */
int
arc_command(int argc, char *argv[])
{
    excess_arguments("", 0, argc, argv);
    return 0;
}

/*
 */
int
changelog_command(int argc, char *argv[])
{
    excess_arguments("", 0, argc, argv);
    return 0;
}
/*
 */
int
circle_command(int argc, char *argv[])
{
    excess_arguments("", 0, argc, argv);
    return 0;
}
/*
 */
int
clear_command(int argc, char *argv[])
{
    excess_arguments("", 0, argc, argv);
    return 0;
}
/*
 */
int
clear_rubber_command(int argc, char *argv[])
{
    excess_arguments("", 0, argc, argv);
    return 0;
}

/*
 * This command takes no arguments.
 */
int
copy_command(int argc, char *argv[])
{
    no_arguments("copy_command", argc, argv);
    View* gview = activeView();
    if (gview) {
        gview->copy();
    }
    return 0;
}

/*
 * This command takes no arguments.
 */
int
cut_command(int argc, char *argv[])
{
    no_arguments("cut_command", argc, argv);
    View* gview = activeView();
    if (gview) {
        gview->cut();
    }
    return 0;
}

/*
 */
int
day_command(int argc, char *argv[])
{
    excess_arguments("", 0, argc, argv);
    return 0;
}
/*
 */
int
details_command(int argc, char *argv[])
{
    excess_arguments("", 0, argc, argv);
    return 0;
}

/*
 */
int
dim_leader_command(int argc, char *argv[])
{
    excess_arguments("", 0, argc, argv);
    return 0;
}

/*
 */
int
ellipse_command(int argc, char *argv[])
{
    excess_arguments("", 0, argc, argv);
    return 0;
}

/*
 * This command takes no arguments.
 */
int
end_command(int argc, char *argv[])
{
    no_arguments("", argc, argv);
    View* gview = activeView();
    if (gview) {
        gview->clearRubberRoom();
        gview->previewOff();
        gview->disableMoveRapidFire();
    }
    prompt->endCommand();
    return 0;
}

/*
 * This command takes no arguments.
 */
int
exit_command(int argc, char *argv[])
{
    no_arguments("", argc, argv);
    if (st[ST_PROMPT_SAVE_HISTORY].b) {
        /* TODO: get filename from settings. */
        prompt->saveHistory("prompt.log", st[ST_PROMPT_SAVE_AS_HTML].b);
    }
    qApp->closeAllWindows();
    /* Force the MainWindow destructor to run before exiting.
     * Makes Valgrind "still reachable" happy :)
     */
    _mainWin->deleteLater();
    return 0;
}

/*
 */
int
fill_command(int argc, char *argv[])
{
    excess_arguments("", 0, argc, argv);
    return 0;
}

/*
 */
int
generate_command(int argc, char *argv[])
{
    excess_arguments("", 0, argc, argv);
    return 0;
}

/*
 */
int
help_command(int argc, char *argv[])
{
    excess_arguments("", 0, argc, argv);
    return 0;
}

/*
 */
int
horizontal_dimension_command(int argc, char *argv[])
{
    excess_arguments("", 0, argc, argv);
    return 0;
}

/*
 */
int
icon_command(int argc, char *argv[])
{
    excess_arguments("", 0, argc, argv);
    return 0;
}
/*
 */
int
image_command(int argc, char *argv[])
{
    excess_arguments("", 0, argc, argv);
    return 0;
}
/*
 */
int
infinite_line_command(int argc, char *argv[])
{
    excess_arguments("", 0, argc, argv);
    return 0;
}
/*
 */
int
line_command(int argc, char *argv[])
{
    excess_arguments("", 0, argc, argv);
    return 0;
}

/*
 */
int
macro_command(int argc, char *argv[])
{
    excess_arguments("", 0, argc, argv);
    return 0;
}

/*
 * Open a new file and establish a view for it.
 *
 * This command takes no arguments.
 */
int
new_command(int argc, char *argv[])
{
    no_arguments("new_file_command", argc, argv);
    state.docIndex++;
    state.numOfDocs++;
    MdiWindow* mdiWin = new MdiWindow(state.docIndex, _mainWin, mdiArea, Qt::SubWindow);
    _mainWin->connect(mdiWin, SIGNAL(sendCloseMdiWin(MdiWindow*)), _mainWin,
        SLOT(onCloseMdiWin(MdiWindow*)));
    _mainWin->connect(mdiArea, SIGNAL(subWindowActivated(QMdiSubWindow*)), _mainWin,
        SLOT(onWindowActivated(QMdiSubWindow*)));

    _mainWin->updateMenuToolbarStatusbar();
    _mainWin->windowMenuAboutToShow();

    View* v = mdiWin->gview;
    if (v) {
        v->recalculateLimits();
        v->zoomExtents();
    }
    return 0;
}

/*
 */
int
night_command(int argc, char *argv[])
{
    excess_arguments("", 0, argc, argv);
    return 0;
}

/*
 * The null command intensionally does nothing.
 * 
 * This command takes no arguments.
 */
int
null_command(int argc, char *argv[])
{
    no_arguments("null_command", argc, argv);
    return 0;
}

/*
 */
int
open_command(int argc, char *argv[])
{
    excess_arguments("", 0, argc, argv);
    return 0;
}

/*
 * Moves the view: the subcommand determines the direction.
 *
 * This command takes 1 argument and it is the subcommand.
 */
int
pan_command(int argc, char *argv[])
{
    excess_arguments("pan_command", 1, argc, argv);

    View* gview = activeView();
    if (!gview) {
        return 3;
    }
    /* Commands that only use the view. */
    if (strcmp(argv[0], "point")) {
        gview->panPoint();
        return 0;
    }
    if (strcmp(argv[0], "real-time")) {
        gview->panRealTime();
        return 0;
    }

    QUndoStack* stack = gview->getUndoStack();
    if (!stack) {
        return 4;
    }
    /* Commands that use the view and the undo stack. */
    if (strcmp(argv[0], "left")) {
        UndoableNavCommand* cmd = new UndoableNavCommand("PanLeft", gview, 0);
        stack->push(cmd);
        return 0;
    }
    if (strcmp(argv[0], "right")) {
        UndoableNavCommand* cmd = new UndoableNavCommand("PanRight", gview, 0);
        stack->push(cmd);
        return 0;
    }
    if (strcmp(argv[0], "down")) {
        UndoableNavCommand* cmd = new UndoableNavCommand("PanDown", gview, 0);
        stack->push(cmd);
        return 0;
    }
    if (strcmp(argv[0], "up")) {
        UndoableNavCommand* cmd = new UndoableNavCommand("PanUp", gview, 0);
        stack->push(cmd);
        return 0;
    }
    return 2;
}

/*
 */
int
paste_command(int argc, char *argv[])
{
    no_arguments("paste_command", argc, argv);
    View* gview = activeView();
    if (gview) {
        gview->paste();
    }
    return 0;
}

/*
 */
int
path_command(int argc, char *argv[])
{
    excess_arguments("", 0, argc, argv);
    return 0;
}

/*
 */
int
play_command(int argc, char *argv[])
{
    excess_arguments("", 0, argc, argv);
    return 0;
}
/*
 */
int
point_command(int argc, char *argv[])
{
    excess_arguments("", 0, argc, argv);
    return 0;
}
/*
 */
int
polygon_command(int argc, char *argv[])
{
    excess_arguments("", 0, argc, argv);
    return 0;
}
/*
 */
int
polyline_command(int argc, char *argv[])
{
    excess_arguments("", 0, argc, argv);
    return 0;
}
/*
 */
int
print_command(int argc, char *argv[])
{
    excess_arguments("", 0, argc, argv);
    return 0;
}
/*
 */
int
ray_command(int argc, char *argv[])
{
    excess_arguments("", 0, argc, argv);
    return 0;
}
/*
 */
int
rectangle_command(int argc, char *argv[])
{
    excess_arguments("", 0, argc, argv);
    return 0;
}
/*
 */
int
redo_command(int argc, char *argv[])
{
    excess_arguments("", 0, argc, argv);
    return 0;
}
/*
 */
int
regular_polygon_command(int argc, char *argv[])
{
    excess_arguments("", 0, argc, argv);
    return 0;
}
/*
 */
int
repeat_command(int argc, char *argv[])
{
    excess_arguments("", 0, argc, argv);
    return 0;
}
/*
 */
int
rounded_rectangle_command(int argc, char *argv[])
{
    excess_arguments("", 0, argc, argv);
    return 0;
}

/*
 * Run the macro in the file supplied.
 *
 * This command take one argument, the file name, and discards the rest.
 *
 * @todo warning box
 */
int
run_command(int argc, char *argv[])
{
    /*
    int status = luaL_dofile(Lua, filename);
    if (status) {
        printf("ERROR: %d\n", status);
        debug("Failed to boot scripting environment.");
    }
    */
    return 0;
}

/*
 */
int
save_as_command(int argc, char *argv[])
{
    excess_arguments("save_as_command", 0, argc, argv);

    /* need to find the activeSubWindow before it loses focus to the FileDialog. */
    MdiWindow* mdiWin = qobject_cast<MdiWindow*>(mdiArea->activeSubWindow());
    if (!mdiWin) {
        return 5;
    }

    _mainWin->openFilesPath = st[ST_RECENT_DIRECTORY].s;
    QString file = QFileDialog::getSaveFileName(_mainWin, _mainWin->tr("Save As"),
        _mainWin->openFilesPath, formatFilterSave);

    mdiWin->saveFile(file);
    return 0;
}

/*
 * FIXME
 *
 * This command takes no arguments.
 */
int
save_command(int argc, char *argv[])
{
    no_arguments("save_command", argc, argv);
    return 0;
}

/*
 * This command takes no arguments.
 */
int
select_all_command(int argc, char *argv[])
{
    no_arguments("select_all_command", argc, argv);
    View* gview = activeView();
    if (gview) {
        gview->selectAll();
    }
    return 0;
}

/*
 * Set a value that is stored in a state variable within a macro.
 *
 * This command takes two arguments.
 *
 * ARGUMENTS
 * |-------|--------|-----------------------------------------------|
 * | index | type   | description                                   |
 * |-------|--------|-----------------------------------------------|
 * | 0     | char * | the variable name                             |
 * | 1     | any    | its value, the type is determined by the name |
 * |-------|--------|-----------------------------------------------|
 *
 * ERROR CODES
 * |-------|-------------------------------------------------------|
 * | Code  | Description                                           |
 * |-------|-------------------------------------------------------|
 * | 0     | No error.                                             |
 * | 1     | Failed to convert types from char array.              |
 * | 2     | Did not recognise the variable name.                  |
 * |-------|-------------------------------------------------------|
 */
int
set_command(int argc, char *argv[])
{
    char *key = argv[0];
    char *value = argv[1];
    if (!strncmp(key, "text_angle", 20)) {
        st[ST_TEXT_ANGLE].r = atof(value);
        return 0;
    }
    if (!strncmp(key, "text_size", 20)) {
        double num = atof(value);

        if (isnan(num)) {
            debug("TypeError, setTextSize(): first argument failed isNaN check.");
            return 1;
        }

        st[ST_TEXT_SIZE].i = fabs(num);
        update_text_size();
        return 0;
    }
    if (!strncmp(key, "text_bold", 20)) {
        st[ST_TEXT_BOLD].b = to_boolean(value);
        return 0;
    }
    if (!strncmp(key, "text_italic", 20)) {
        st[ST_TEXT_ITALIC].b = to_boolean(value);
        return 0;
    }
    if (!strncmp(key, "text_underline", 20)) {
        st[ST_TEXT_UNDERLINE].b = to_boolean(value);
        return 0;
    }
    if (!strncmp(key, "text_strikeout", 20)) {
        st[ST_TEXT_STRIKEOUT].b = to_boolean(value);
        return 0;
    }
    if (!strncmp(key, "text_overline", 20)) {
        st[ST_TEXT_OVERLINE].b = to_boolean(value);
        return 0;
    }
    if (!strncmp(key, "prefix", 20)) {
        update_prompt_prefix(value);
        return 0;
    }
    return 2;
}

/*
 */
int
settings_command(int argc, char *argv[])
{
    excess_arguments("", 0, argc, argv);
    return 0;
}
/*
 */
int
sleep_command(int argc, char *argv[])
{
    excess_arguments("", 0, argc, argv);
    return 0;
}
/*
 */
int
slot_command(int argc, char *argv[])
{
    excess_arguments("", 0, argc, argv);
    return 0;
}
/*
 */
int
stop_command(int argc, char *argv[])
{
    excess_arguments("", 0, argc, argv);
    return 0;
}
/*
 */
int
stub_command(int argc, char *argv[])
{
    no_arguments("stub_command", argc, argv);
    QMessageBox::warning(_mainWin, _mainWin->tr("Testing Feature"),
        _mainWin->tr("<b>This feature is in testing.</b>"));
    return 0;
}

/*
 * Add a EmbTextMulti object to the design.
 * TODO: argument parsing
 */
int
text_multi_command(int argc, char *argv[])
{
    excess_arguments("", 0, argc, argv);
    const QString& str = "Lorem ipsum\ndolor sit amet,";
    EmbVector position = emb_vector(10.0f, 10.0f);
    EmbReal rot = 0.0f;
    bool fill = false;
    int rubberMode = OBJ_RUBBER_OFF;
    _mainWin->add_text_multi(str, position, rot, fill, rubberMode);
    return 0;
}

/* TODO: argument parsing
 */
int
text_single_command(int argc, char *argv[])
{
    excess_arguments("", 0, argc, argv);
    const QString& str = "Lorem ipsum dolor sit amet,";
    EmbVector position = emb_vector(10.0f, 10.0f);
    EmbReal rot = 0.0f;
    bool fill = false;
    int rubberMode = OBJ_RUBBER_OFF;
    _mainWin->add_text_single(str, position, rot, fill, rubberMode);
    return 0;
}

/*
 */
int
triangle_command(int argc, char *argv[])
{
    excess_arguments("", 0, argc, argv);
    return 0;
}

/*
 */
int
undo_command(int argc, char *argv[])
{
    excess_arguments("", 0, argc, argv);
    return 0;
}

/*
 */
int
update_command(int argc, char *argv[])
{
    no_arguments("update_command", argc, argv);
    debug("TODO: Check website for new versions, commands, etc...");
    return 0;
}

/*
 */
int
vertical_dimension_command(int argc, char *argv[])
{
    excess_arguments("vertical_dimension_command", 0, argc, argv);
    return 0;
}

/*
 */
int
whats_this_command(int argc, char *argv[])
{
    no_arguments("whats_this_command", argc, argv);
    QWhatsThis::enterWhatsThisMode();
    return 0;
}

/*
 * Alters the MDI windows, the subcommand determines the behaviour.
 *
 * This command always expects exactly one argument.
 *
 * ARGUMENTS
 * |-------|--------|-----------------------------------------------|
 * | index | type   | description                                   |
 * |-------|--------|-----------------------------------------------|
 * | 0     | char * | the subcommand                                |
 * |-------|--------|-----------------------------------------------|
 */
int
window_command(int argc, char *argv[])
{
    excess_arguments("zoom_command", 1, argc, argv);
    char *key = argv[1];
    if (!strncmp(key, "cascade", 20)) {
        mdiArea->cascade();
        return 0;
    }
    if (!strncmp(key, "close_all", 20)) {
        _mainWin->onCloseWindow();
        return 0;
    }
    if (!strncmp(key, "close", 20)) {
        mdiArea->closeAllSubWindows();
        return 0;
    }
    if (!strncmp(key, "next", 20)) {
        mdiArea->activateNextSubWindow();
        return 0;
    }
    if (!strncmp(key, "previous", 20)) {
        mdiArea->activatePreviousSubWindow();
        return 0;
    }
    if (!strncmp(key, "tile", 20)) {
        mdiArea->tile();
        return 0;
    }
    return 2;
}

/*
 * This command always expects exactly one argument.
 *
 * ARGUMENTS
 * |-------|--------|-----------------------------------------------|
 * | index | type   | description                                   |
 * |-------|--------|-----------------------------------------------|
 * | 0     | char * | the subcommand                                |
 * |-------|--------|-----------------------------------------------|
 */
int
zoom_command(int argc, char *argv[])
{
    excess_arguments("window_command", 1, argc, argv);
    char *key = argv[1];
    if (!strncmp(key, "all", 20)) {
        debug("TODO: Implement zoomAll.");
        return 0;
    }
    if (!strncmp(key, "center", 20)) {
        debug("TODO: Implement zoomCenter.");
        return 0;
    }
    if (!strncmp(key, "dynamic", 20)) {
        debug("TODO: Implement zoomDynamic.");
        return 0;
    }
    if (!strncmp(key, "extents", 20)) {
        View* gview = activeView();
        QUndoStack* stack = gview->getUndoStack();
        if (gview && stack) {
            UndoableNavCommand* cmd = new UndoableNavCommand("ZoomExtents", gview, 0);
            stack->push(cmd);
        }
        return 0;
    }
    if (!strncmp(key, "in", 20)) {
        View* gview = activeView();
        if (gview) {
            gview->zoomIn();
        }
        return 0;
    }
    if (!strncmp(key, "out", 20)) {
        View* gview = activeView();
        if (gview) {
            gview->zoomOut();
        }
        return 0;
    }
    if (!strncmp(key, "previous", 20)) {
        debug("TODO: Implement zoomPrevious.");
        return 0;
    }
    if (!strncmp(key, "real_time", 20)) {
        debug("TODO: Implement zoomRealtime.");
        return 0;
    }
    if (!strncmp(key, "scale", 20)) {
        debug("TODO: Implement zoomScale.");
        return 0;
    }
    if (!strncmp(key, "selected", 20)) {
        View* gview = activeView();
        QUndoStack* stack = gview->getUndoStack();
        if (gview && stack) {
            UndoableNavCommand* cmd = new UndoableNavCommand("ZoomSelected", gview, 0);
            stack->push(cmd);
        }
        return 0;
    }
    if (!strncmp(key, "window", 20)) {
        View* gview = activeView();
        if (gview) {
            gview->zoomWindow();
        }
        return 0;
    }
    return 2;
}

#if 0
-- Command: Distance

var global = {}-- Required
global.x1
global.y1
global.x2
global.y2

-- NOTE: main() is run every time the command is started.
--       Use it to reset variables so they are ready to go.
function main()
{
    initCommand()
    clearSelection()
    global.x1 = NaN
    global.y1 = NaN
    global.x2 = NaN
    global.y2 = NaN
    setPromptPrefix(qsTr("Specify first point: "))
}

-- NOTE: click() is run only for left clicks.
--       Middle clicks are used for panning.
--       Right clicks bring up the context menu.
function click(x, y)
{
    if(isNaN(global.x1))
    {
        global.x1 = x
        global.y1 = y
        addRubber("LINE")
        setRubberMode("LINE")
        setRubberPoint("LINE_START", global.x1, global.y1)
        appendPromptHistory()
        setPromptPrefix(qsTr("Specify second point: "))
    }
    else
    {
        appendPromptHistory()
        global.x2 = x
        global.y2 = y
        reportDistance()
        endCommand()
    }
}

-- NOTE: context() is run when a context menu entry is chosen.
function context(str)
{
    todo("DISTANCE", "context()")
}

-- NOTE: prompt() is run when Enter is pressed.
--       appendPromptHistory is automatically called before prompt()
--       is called so calling it is only needed for erroneous input.
--       Any text in the command prompt is sent as an uppercase string.
function prompt(str)
{
    var strList = str.split(",")
    if(isNaN(global.x1))
    {
        if(isNaN(strList[0]) || isNaN(strList[1]))
        {
            alert(qsTr("Requires numeric distance or two points."))
            setPromptPrefix(qsTr("Specify first point: "))
        }
        else
        {
            global.x1 = Number(strList[0])
            global.y1 = Number(strList[1])
            addRubber("LINE")
            setRubberMode("LINE")
            setRubberPoint("LINE_START", global.x1, global.y1)
            setPromptPrefix(qsTr("Specify second point: "))
        }
    }
    else
    {
        if(isNaN(strList[0]) || isNaN(strList[1]))
        {
            alert(qsTr("Requires numeric distance or two points."))
            setPromptPrefix(qsTr("Specify second point: "))
        }
        else
        {
            global.x2 = Number(strList[0])
            global.y2 = Number(strList[1])
            reportDistance()
            endCommand()
        }
    }
}

-- Cartesian Coordinate System reported:
-- 
--                (+)
--                90
--                |
--       (-) 180__|__0 (+)
--                |
--               270
--               (-)

function reportDistance()
{
    var dx = global.x2 - global.x1
    var dy = global.y2 - global.y1

    var dist = calculateDistance(global.x1,global.y1,global.x2, global.y2)
    var angle = calculateAngle(global.x1,global.y1,global.x2, global.y2)

    setPromptPrefix(qsTr("Distance") + " = " + dist.toString() + ", " + qsTr("Angle") + " = " + angle.toString())
    appendPromptHistory()
    setPromptPrefix(qsTr("Delta X") + " = " + dx.toString() + ", " + qsTr("Delta Y") + " = " + dy.toString())
    appendPromptHistory()
}

-- Command: Erase/Delete

-- NOTE: main() is run every time the command is started.
--       Use it to reset variables so they are ready to go.
function main()
{
    initCommand();

    if(numSelected() <= 0)
    {
        -- TODO: Prompt to select objects if nothing is preselected
        alert(qsTr("Preselect objects before invoking the delete command."));
        endCommand();
        messageBox("information", qsTr("Delete Preselect"), qsTr("Preselect objects before invoking the delete command."));
    }
    else
    {
        deleteSelected();
        endCommand();
    }
}

-- NOTE: click() is run only for left clicks.
--       Middle clicks are used for panning.
--       Right clicks bring up the context menu.
function click(x, y)
{
    deleteSelected();
    endCommand();
}

-- NOTE: context() is run when a context menu entry is chosen.
function context(str)
{
    deleteSelected();
    endCommand();
}

-- NOTE: prompt() is run when Enter is pressed.
--       appendPromptHistory is automatically called before prompt()
--       is called so calling it is only needed for erroneous input.
--       Any text is in the command prompt is sent as an uppercase string.
function prompt(str)
{
    deleteSelected();
    endCommand();
}
-- Command: Exit

-- NOTE: main() is run every time the command is started.
--       Use it to reset variables so they are ready to go.
function main()
{
    initCommand();
    clearSelection();
    exit();
    endCommand();
}

-- NOTE: click() is run only for left clicks.
--       Middle clicks are used for panning.
--       Right clicks bring up the context menu.
function click(x, y)
{
    exit();
    endCommand();
}

-- NOTE: context() is run when a context menu entry is chosen.
function context(str)
{
    exit();
    endCommand();
}

-- NOTE: prompt() is run when Enter is pressed.
--       appendPromptHistory is automatically called before prompt()
--       is called so calling it is only needed for erroneous input.
--       Any text is in the command prompt is sent as an uppercase string.
function prompt(str)
{
    exit();
    endCommand();
}

-- Command: Locate Point

-- NOTE: main() is run every time the command is started.
--       Use it to reset variables so they are ready to go.
function main()
{
    initCommand();
    clearSelection();
    setPromptPrefix(qsTr("Specify point: "));
}

-- NOTE: click() is run only for left clicks.
--       Middle clicks are used for panning.
--       Right clicks bring up the context menu.
function click(x, y)
{
    appendPromptHistory();
    setPromptPrefix("X = " + x.toString() + ", Y = " + y.toString());
    appendPromptHistory();
    endCommand();
}

-- NOTE: context() is run when a context menu entry is chosen.
function context(str)
{
    todo("LOCATEPOINT", "context()");
}

-- NOTE: prompt() is run when Enter is pressed.
--       appendPromptHistory is automatically called before prompt()
--       is called so calling it is only needed for erroneous input.
--       Any text in the command prompt is sent as an uppercase string.
function prompt(str)
{
    var strList = str.split(",");
    if(isNaN(strList[0]) || isNaN(strList[1]))
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
-- Command: Move

var global = {}-- -- Required
global.firstRun;
global.baseX;
global.baseY;
global.destX;
global.destY;
global.deltaX;
global.deltaY;

-- NOTE: main() is run every time the command is started.
--       Use it to reset variables so they are ready to go.
function main()
{
    initCommand();
    global.firstRun = true;
    global.baseX  = NaN;
    global.baseY  = NaN;
    global.destX  = NaN;
    global.destY  = NaN;
    global.deltaX = NaN;
    global.deltaY = NaN;

    if(numSelected() <= 0)
    {
        -- TODO: Prompt to select objects if nothing is preselected
        alert(qsTr("Preselect objects before invoking the move command."));
        endCommand();
        messageBox("information", qsTr("Move Preselect"), qsTr("Preselect objects before invoking the move command."));
    }
    else
    {
        setPromptPrefix(qsTr("Specify base point: "));
    }
}

-- NOTE: click() is run only for left clicks.
--       Middle clicks are used for panning.
--       Right clicks bring up the context menu.
function click(x, y)
{
    if(global.firstRun)
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

-- NOTE: context() is run when a context menu entry is chosen.
function context(str)
{
    todo("MOVE", "context()");
}

-- NOTE: prompt() is run when Enter is pressed.
--       appendPromptHistory is automatically called before prompt()
--       is called so calling it is only needed for erroneous input.
--       Any text in the command prompt is sent as an uppercase string.
function prompt(str)
{
    if(global.firstRun)
    {
        var strList = str.split(",");
        if(isNaN(strList[0]) || isNaN(strList[1]))
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
        if(isNaN(strList[0]) || isNaN(strList[1]))
        {
            alert(qsTr("Invalid point."));
            setPromptPrefix(qsTr("Specify destination point: "));
        }
        else
            global.destX = Number(strList[0]);
            global.destY = Number(strList[1]);
            global.deltaX = global.destX - global.baseX;
            global.deltaY = global.destY - global.baseY;
            moveSelected(global.deltaX, global.deltaY);
            previewOff();
            endCommand();
        end
    end
end
-- Command: Platform

function platform_command ()
  -- NOTE: main() is run every time the command is started.
  --       Use it to reset variables so they are ready to go.
  if context == CONTEXT_MAIN then
    init_command()
    clear_selection()
  end
  setPromptPrefix(qsTr("Platform") + " = " + platformString())
  appendPromptHistory()
  end_command()
end

-- Command: RGB

var global = {}-- -- Required
global.mode;

-- enums
global.mode_BACKGROUND = 0;
global.mode_CROSSHAIR  = 1;
global.mode_GRID       = 2;

-- NOTE: main() is run every time the command is started.
--       Use it to reset variables so they are ready to go.
function main()
{
    initCommand();
    clearSelection();
    global.mode = global.mode_BACKGROUND;
    setPromptPrefix(qsTr("Enter RED,GREEN,BLUE values for background or [Crosshair/Grid]: "));
}

-- NOTE: click() is run only for left clicks.
--       Middle clicks are used for panning.
--       Right clicks bring up the context menu.
function click(x, y)
{
    -- Do Nothing, prompt only command.
}

-- NOTE: context() is run when a context menu entry is chosen.
function context(str)
{
    todo("RGB", "context()");
}

-- NOTE: prompt() is run when Enter is pressed.
--       appendPromptHistory is automatically called before prompt()
--       is called so calling it is only needed for erroneous input.
--       Any text is in the command prompt is sent as an uppercase string.
function prompt(str)
{
    if(global.mode == global.mode_BACKGROUND)
    {
        if(str == "C" || str == "CROSSHAIR") -- TODO: Probably should add additional qsTr calls here.
        {
            global.mode = global.mode_CROSSHAIR;
            setPromptPrefix(qsTr("Specify crosshair color: "));
        }
        else if(str == "G" || str == "GRID") -- TODO: Probably should add additional qsTr calls here.
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
            if(!validRGB(r,g,b))
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
    else if(global.mode == global.mode_CROSSHAIR)
    {
        var strList = str.split(",");
        var r = Number(strList[0]);
        var g = Number(strList[1]);
        var b = Number(strList[2]);
        if(!validRGB(r,g,b))
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
    else if(global.mode == global.mode_GRID)
    {
        var strList = str.split(",");
        var r = Number(strList[0]);
        var g = Number(strList[1]);
        var b = Number(strList[2]);
        if(!validRGB(r,g,b)) then
            alert(qsTr("Invalid color. R,G,B values must be in the range of 0-255."))
            setPromptPrefix(qsTr("Specify grid color: "));
        else
            setGridColor(r,g,b)
            endCommand()
        end
    end
end

function validRGB(r, g, b)
    if(isNaN(r)) return false;
    if(isNaN(g)) return false;
    if(isNaN(b)) return false;
    if(r < 0 || r > 255) return false;
    if(g < 0 || g > 255) return false;
    if(b < 0 || b > 255) return false;
    return true;
end
-- Command: Rotate

var global = {}-- -- Required
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

-- enums
global.mode_NORMAL    = 0;
global.mode_REFERENCE = 1;

-- NOTE: main() is run every time the command is started.
--       Use it to reset variables so they are ready to go.
function main()
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

    if(numSelected() <= 0)
    {
        -- TODO: Prompt to select objects if nothing is preselected
        alert(qsTr("Preselect objects before invoking the rotate command."));
        endCommand();
        messageBox("information", qsTr("Rotate Preselect"), qsTr("Preselect objects before invoking the rotate command."));
    }
    else
    {
        setPromptPrefix(qsTr("Specify base point: "));
    }
}

-- NOTE: click() is run only for left clicks.
--       Middle clicks are used for panning.
--       Right clicks bring up the context menu.
function click(x, y)
{
    if(global.mode == global.mode_NORMAL)
    {
        if(global.firstRun)
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
    else if(global.mode == global.mode_REFERENCE)
    {
        if(isNaN(global.baseRX))
        {
            global.baseRX = x;
            global.baseRY = y;
            appendPromptHistory();
            addRubber("LINE");
            setRubberMode("LINE");
            setRubberPoint("LINE_START", global.baseRX, global.baseRY);
            setPromptPrefix(qsTr("Specify second point: "));
        }
        else if(isNaN(global.destRX))
        {
            global.destRX = x;
            global.destRY = y;
            global.angleRef = calculateAngle(global.baseRX, global.baseRY, global.destRX, global.destRY);
            setRubberPoint("LINE_START", global.baseX, global.baseY);
            previewOn("SELECTED", "ROTATE", global.baseX, global.baseY, global.angleRef);
            appendPromptHistory();
            setPromptPrefix(qsTr("Specify the new angle: "));
        }
        else if(isNaN(global.angleNew))
        {
            global.angleNew = calculateAngle(global.baseX, global.baseY, x, y);
            rotateSelected(global.baseX, global.baseY, global.angleNew - global.angleRef);
            previewOff();
            endCommand();
        }
    }
}

-- NOTE: context() is run when a context menu entry is chosen.
function context(str)
{
    todo("ROTATE", "context()");
}

-- NOTE: prompt() is run when Enter is pressed.
--       appendPromptHistory is automatically called before prompt()
--       is called so calling it is only needed for erroneous input.
--       Any text in the command prompt is sent as an uppercase string.
function prompt(str)
{
    if(global.mode == global.mode_NORMAL)
    {
        if(global.firstRun)
        {
            var strList = str.split(",");
            if(isNaN(strList[0]) || isNaN(strList[1]))
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
                previewOn("SELECTED", "ROTATE", global.baseX, global.baseY, 0);
                setPromptPrefix(qsTr("Specify rotation angle or [Reference]: "));
            }
        }
        else
        {
            if(str == "R" || str == "REFERENCE") -- TODO: Probably should add additional qsTr calls here.
            {
                global.mode = global.mode_REFERENCE;
                setPromptPrefix(qsTr("Specify the reference angle") + " {0.00}: ");
                clearRubber();
                previewOff();
            }
            else
            {
                if(isNaN(str))
                {
                    alert(qsTr("Requires valid numeric angle, second point, or option keyword."));
                    setPromptPrefix(qsTr("Specify rotation angle or [Reference]: "));
                }
                else
                {
                    global.angle = Number(str);
                    rotateSelected(global.baseX, global.baseY, global.angle);
                    previewOff();
                    endCommand();
                }
            }
        }
    }
    else if(global.mode == global.mode_REFERENCE)
    {
        if(isNaN(global.baseRX))
        {
            if(isNaN(str))
            {
                var strList = str.split(",");
                if(isNaN(strList[0]) || isNaN(strList[1]))
                {
                    alert(qsTr("Requires valid numeric angle or two points."));
                    setPromptPrefix(qsTr("Specify the reference angle") + " {0.00}: ");
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
                -- The base and dest values are only set here to advance the command.
                global.baseRX = 0.0;
                global.baseRY = 0.0;
                global.destRX = 0.0;
                global.destRY = 0.0;
                -- The reference angle is what we will use later.
                global.angleRef = Number(str);
                addRubber("LINE");
                setRubberMode("LINE");
                setRubberPoint("LINE_START", global.baseX, global.baseY);
                previewOn("SELECTED", "ROTATE", global.baseX, global.baseY, global.angleRef);
                setPromptPrefix(qsTr("Specify the new angle: "));
            }
        }
        else if(isNaN(global.destRX))
        {
            if(isNaN(str))
            {
                var strList = str.split(",");
                if(isNaN(strList[0]) || isNaN(strList[1]))
                {
                    alert(qsTr("Requires valid numeric angle or two points."));
                    setPromptPrefix(qsTr("Specify second point: "));
                }
                else
                {
                    global.destRX = Number(strList[0]);
                    global.destRY = Number(strList[1]);
                    global.angleRef = calculateAngle(global.baseRX, global.baseRY, global.destRX, global.destRY);
                    previewOn("SELECTED", "ROTATE", global.baseX, global.baseY, global.angleRef);
                    setRubberPoint("LINE_START", global.baseX, global.baseY);
                    setPromptPrefix(qsTr("Specify the new angle: "));
                }
            }
            else
            {
                -- The base and dest values are only set here to advance the command.
                global.baseRX = 0.0;
                global.baseRY = 0.0;
                global.destRX = 0.0;
                global.destRY = 0.0;
                -- The reference angle is what we will use later.
                global.angleRef = Number(str);
                previewOn("SELECTED", "ROTATE", global.baseX, global.baseY, global.angleRef);
                setPromptPrefix(qsTr("Specify the new angle: "));
            }
        }
        else if(isNaN(global.angleNew))
        {
            if(isNaN(str))
            {
                var strList = str.split(",");
                if(isNaN(strList[0]) || isNaN(strList[1]))
                {
                    alert(qsTr("Requires valid numeric angle or second point."));
                    setPromptPrefix(qsTr("Specify the new angle: "));
                }
                else
                {
                    var x = Number(strList[0]);
                    var y = Number(strList[1]);
                    global.angleNew = calculateAngle(global.baseX, global.baseY, x, y);
                    rotateSelected(global.baseX, global.baseY, global.angleNew - global.angleRef);
                    previewOff()
                    endCommand()
                end
            else
                global.angleNew = Number(str)
                rotateSelected(global.baseX, global.baseY, global.angleNew - global.angleRef)
                previewOff()
                endCommand()
            end
        end
    end
end

-- Command: Scale

var global = {}-- -- Required
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

-- enums
global.mode_NORMAL    = 0;
global.mode_REFERENCE = 1;

-- NOTE: main() is run every time the command is started.
--       Use it to reset variables so they are ready to go.
function main()
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

    if(numSelected() <= 0)
    {
        -- TODO: Prompt to select objects if nothing is preselected
        alert(qsTr("Preselect objects before invoking the scale command."));
        endCommand();
        messageBox("information", qsTr("Scale Preselect"), qsTr("Preselect objects before invoking the scale command."));
    }
    else
    {
        setPromptPrefix(qsTr("Specify base point: "));
    }
}

-- NOTE: click() is run only for left clicks.
--       Middle clicks are used for panning.
--       Right clicks bring up the context menu.
function click(x, y)
{
    if(global.mode == global.mode_NORMAL)
    {
        if(global.firstRun)
        {
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
        else
        {
            global.destX = x;
            global.destY = y;
            global.factor = calculateDistance(global.baseX, global.baseY, global.destX, global.destY);
            appendPromptHistory();
            scaleSelected(global.baseX, global.baseY, global.factor);
            previewOff();
            endCommand();
        }
    }
    else if(global.mode == global.mode_REFERENCE)
    {
        if(isNaN(global.baseRX))
        {
            global.baseRX = x;
            global.baseRY = y;
            appendPromptHistory();
            addRubber("LINE");
            setRubberMode("LINE");
            setRubberPoint("LINE_START", global.baseRX, global.baseRY);
            setPromptPrefix(qsTr("Specify second point: "));
        }
        else if(isNaN(global.destRX))
        {
            global.destRX = x;
            global.destRY = y;
            global.factorRef = calculateDistance(global.baseRX, global.baseRY, global.destRX, global.destRY);
            if(global.factorRef <= 0.0)
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
        else if(isNaN(global.factorNew))
        {
            global.factorNew = calculateDistance(global.baseX, global.baseY, x, y);
            if(global.factorNew <= 0.0)
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

-- NOTE: context() is run when a context menu entry is chosen.
function context(str)
{
    todo("SCALE", "context()");
}

-- NOTE: prompt() is run when Enter is pressed.
--       appendPromptHistory is automatically called before prompt()
--       is called so calling it is only needed for erroneous input.
--       Any text in the command prompt is sent as an uppercase string.
function prompt(str)
{
    if(global.mode == global.mode_NORMAL)
    {
        if(global.firstRun)
        {
            var strList = str.split(",");
            if(isNaN(strList[0]) || isNaN(strList[1]))
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
                previewOn("SELECTED", "SCALE", global.baseX, global.baseY, 1);
                setPromptPrefix(qsTr("Specify scale factor or [Reference]: "));
            }
        }
        else
        {
            if(str == "R" || str == "REFERENCE") -- TODO: Probably should add additional qsTr calls here.
            {
                global.mode = global.mode_REFERENCE;
                setPromptPrefix(qsTr("Specify reference length") + " {1}: ");
                clearRubber();
                previewOff();
            }
            else
            {
                if(isNaN(str))
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
    else if(global.mode == global.mode_REFERENCE)
    {
        if(isNaN(global.baseRX))
        {
            if(isNaN(str))
            {
                var strList = str.split(",");
                if(isNaN(strList[0]) || isNaN(strList[1]))
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
                -- The base and dest values are only set here to advance the command.
                global.baseRX = 0.0;
                global.baseRY = 0.0;
                global.destRX = 0.0;
                global.destRY = 0.0;
                -- The reference length is what we will use later.
                global.factorRef = Number(str);
                if(global.factorRef <= 0.0)
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
        else if(isNaN(global.destRX))
        {
            if(isNaN(str))
            {
                var strList = str.split(",");
                if(isNaN(strList[0]) || isNaN(strList[1]))
                {
                    alert(qsTr("Requires valid numeric distance or two points."));
                    setPromptPrefix(qsTr("Specify second point: "));
                }
                else
                {
                    global.destRX = Number(strList[0]);
                    global.destRY = Number(strList[1]);
                    global.factorRef = calculateDistance(global.baseRX, global.baseRY, global.destRX, global.destRY);
                    if(global.factorRef <= 0.0)
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
                -- The base and dest values are only set here to advance the command.
                global.baseRX = 0.0;
                global.baseRY = 0.0;
                global.destRX = 0.0;
                global.destRY = 0.0;
                -- The reference length is what we will use later.
                global.factorRef = Number(str);
                if(global.factorRef <= 0.0)
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
        else if(isNaN(global.factorNew))
        {
            if(isNaN(str))
            {
                var strList = str.split(",");
                if(isNaN(strList[0]) || isNaN(strList[1]))
                {
                    alert(qsTr("Requires valid numeric distance or second point."));
                    setPromptPrefix(qsTr("Specify new length: "));
                }
                else
                {
                    var x = Number(strList[0]);
                    var y = Number(strList[1]);
                    global.factorNew = calculateDistance(global.baseX, global.baseY, x, y);
                    if(global.factorNew <= 0.0)
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
                if(global.factorNew <= 0.0)
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
                end
            end
        end
    end
end
-- Command: SelectAll

-- NOTE: main() is run every time the command is started.
--       Use it to reset variables so they are ready to go.
function main()
    initCommand();
    selectAll();
    endCommand();
end

-- NOTE: click() is run only for left clicks.
--       Middle clicks are used for panning.
--       Right clicks bring up the context menu.
function click(x, y)
    selectAll();
    endCommand();
end

-- NOTE: context() is run when a context menu entry is chosen.
function context(str)
    selectAll();
    endCommand();
end

-- NOTE: prompt() is run when Enter is pressed.
--       appendPromptHistory is automatically called before prompt()
--       is called so calling it is only needed for erroneous input.
--       Any text is in the command prompt is sent as an uppercase string.
function prompt(str)
    selectAll();
    endCommand();
end
-- Command: SysWindows

-- NOTE: main() is run every time the command is started.
--       Use it to reset variables so they are ready to go.
function main()
{
    initCommand();
    clearSelection();
    setPromptPrefix(qsTr("Enter an option [Cascade/Tile]: "));
}

-- NOTE: click() is run only for left clicks.
--       Middle clicks are used for panning.
--       Right clicks bring up the context menu.
function click(x, y)
{
    -- Do Nothing
}

-- NOTE: context() is run when a context menu entry is chosen.
function context(str)
{
    -- Do Nothing
}

-- NOTE: prompt() is run when Enter is pressed.
--       appendPromptHistory is automatically called before prompt()
--       is called so calling it is only needed for erroneous input.
--       Any text in the command prompt is sent as an uppercase string.
function prompt(str)
{
    if(str == "C" || str == "CASCADE") -- TODO: Probably should add additional qsTr calls here.
    {
        windowCascade();
        endCommand();
    }
    else if(str == "T" || str == "TILE") -- TODO: Probably should add additional qsTr calls here.
    {
        windowTile();
        endCommand();
    }
    else
        alert(qsTr("Invalid option keyword."));
        setPromptPrefix(qsTr("Enter an option [Cascade/Tile]: "));
    end
end
#endif

