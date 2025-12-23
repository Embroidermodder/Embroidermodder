/*
 * Embroidermodder 2
 * Copyright 2011-2025 The Embroidermodder Team
 *
 * Embroidermodder 2 is free and open software under the zlib license:
 * see LICENSE.md for details.
 *
 * ----------------------------------------------------------------------------
 *
 * Wrappers for commands that use C++ or Qt specific features.
 *
 * Try to keep this section of the file alphabetized in the function name to
 * match the declarations above.
 *
 * All function names should end in `_c`.
 */

#include "embroidermodder.h"

using namespace std::chrono_literals;

extern QString formatFilterSave;

/* . */
int
clear_selection_c(void)
{
    View* gview = activeView();
    if (gview) {
        gview->clearSelection();
    }
    return 0;
}

/* . */
int
copy_c(void)
{
    View* gview = activeView();
    if (gview) {
        gview->copy();
    }
    return 0;
}

/* . */
int
cut_c(void)
{
    View* gview = activeView();
    if (gview) {
        gview->cut();
    }
    return 0;
}

/* TODO: Make day vision color settings. */
int
day_c(void)
{
    View* gview = activeView();
    if (gview) {
        gview->setBackgroundColor(qRgb(255,255,255)); 
        gview->setCrossHairColor(qRgb(0,0,0));
        gview->setGridColor(qRgb(0,0,0));
    }
    return 0;
}

int
end_c(void)
{
    View* gview = activeView();
    if (gview) {
        gview->clearRubberRoom();
        gview->previewOff();
        gview->disableMoveRapidFire();
    }
    prompt->endCommand();
    return 0;
}

int
exit_program_c(void)
{
    if (st[ST_PROMPT_SAVE_HISTORY].b) {
        /* TODO: get filename from settings. */
        prompt->saveHistory("prompt.log", st[ST_PROMPT_SAVE_AS_HTML].b);
    }
    if (state.debug) {
        if (!report_state(state)) {
            debug("State reporting failed because \"state.toml\" could not be created.");
        }
    }

    qApp->closeAllWindows();
    /* Force the MainWindow destructor to run before exiting.
     * Makes Valgrind "still reachable" happy :)
     */
    _mainWin->deleteLater();
    return 0;
}

/* TODO: Report the value.
 */
ScriptValue
get_c(const char *key)
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

int
icon_resize_c(int size)
{
    _mainWin->iconResize(size);
    return 0;
}

/* . */
int
messagebox_c(const char *type, const char *title, const char *text)
{
    QString msgType = QString(type).toLower();
    QString title_ = _mainWin->tr(title);
    QString text_ = _mainWin->tr(text);
    if (msgType == "critical") {
        QMessageBox::critical(_mainWin, title_, text_);
    }
    else if (msgType == "information") {
        QMessageBox::information(_mainWin, title_, text_);
    }
    else if (msgType == "question") {
        QMessageBox::question(_mainWin, title_, text_);
    }
    else if (msgType == "warning") {
        QMessageBox::warning(_mainWin, title_, text_);
    }
    else {
        QMessageBox::critical(_mainWin,
            _mainWin->tr("Native MessageBox Error"),
            _mainWin->tr("Incorrect use of the native messageBox function."));
    }
    return 0;
}

int
new_file_c(void)
{
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

/* TODO: Make night vision color settings. */
int
night_c(void)
{
    View* gview = activeView();
    if (gview) {
        gview->setBackgroundColor(qRgb(0,0,0));
        gview->setCrossHairColor(qRgb(255,255,255));
        gview->setGridColor(qRgb(255,255,255));
    }
    return 0;
}

int
open_c(const char *filename)
{
    if (filename[0] == 0) {
        _mainWin->openFile();
    }
    QStringList list = {filename};
    _mainWin->openFilesSelected(list);
    return 0;
}

int
pan_c(const char *subcommand)
{
    /* Moves the view to the left. */
    if (!strcmp(subcommand, "left")) {
        View* gview = activeView();
        QUndoStack* stack = gview->getUndoStack();
        if (gview && stack) {
            UndoableNavCommand* cmd = new UndoableNavCommand("PanLeft", gview, 0);
            stack->push(cmd);
        }
        return 0;
    }
    /* Moves the view down. */
    if (!strcmp(subcommand, "down")) {
        View* gview = activeView();
        QUndoStack* stack = gview->getUndoStack();
        if (gview && stack) {
            UndoableNavCommand* cmd = new UndoableNavCommand("PanDown", gview, 0);
            stack->push(cmd);
        }
        return 0;
    }
    /* Moves the view to the right. */
    if (!strcmp(subcommand, "right")) {
        View* gview = activeView();
        QUndoStack* stack = gview->getUndoStack();
        if (gview && stack) {
            UndoableNavCommand* cmd = new UndoableNavCommand("PanRight", gview, 0);
            stack->push(cmd);
        }
        return 0;
    }
    /* Moves the view up. */
    if (!strcmp(subcommand, "up")) {
        View* gview = activeView();
        QUndoStack* stack = gview->getUndoStack();
        if (gview && stack) {
            UndoableNavCommand* cmd = new UndoableNavCommand("PanUp", gview, 0);
            stack->push(cmd);
        }
        return 0;
    }
    if (!strcmp(subcommand, "point")) {
        View* gview = activeView();
        if (gview) {
            gview->panPoint();
        }
        return 0;
    }
    if (!strcmp(subcommand, "real-time")) {
        View* gview = activeView();
        if (gview) {
            gview->panRealTime();
        }
        return 0;
    }
    return 1;
}

int
paste_c(void)
{
    View* gview = activeView();
    if (gview) {
        gview->paste();
    }
    return 0;
}

int
print_file_c(void)
{
    MdiWindow* mdiWin = qobject_cast<MdiWindow*>(mdiArea->activeSubWindow());
    if (mdiWin == NULL) {
        debug("ERROR: No active window for printing.");
        return 1;
    }
    View *gview = activeView();

    /*
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
    */
    return 0;
}

int
redo_c(void)
{
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
    return 0;
}

int
repeat_c(void)
{
    return 0;
}

int
save_c(void)
{
    /* FIXME */
    return 0;
}

int
save_as_c(void)
{
    /* We need to find the activeSubWindow before it loses focus to the FileDialog. */
    MdiWindow* mdiWin = qobject_cast<MdiWindow*>(mdiArea->activeSubWindow());
    if (!mdiWin) {
        return 1;
    }

    _mainWin->openFilesPath = st[ST_RECENT_DIRECTORY].s;
    QString file = QFileDialog::getSaveFileName(_mainWin, _mainWin->tr("Save As"),
        _mainWin->openFilesPath, formatFilterSave);

    mdiWin->saveFile(file);
    return 0;
}

/**
 * @brief Run the script in the file supplied.
 *
 * @todo warning box
 */
int
script_c(const char *filename)
{
    /* FIXME: */
    return 0;
}

int
select_all_c(void)
{
    View* gview = activeView();
    if (gview) {
        gview->selectAll();
    }
    return 0;
}

/*
 */
int
set_c(const char *key, ScriptValue value)
{
    if (!strncmp(key, "text_angle", 20)) {
        st[ST_TEXT_ANGLE].r = value.r;
        return 0;
    }
    if (!strncmp(key, "text_size", 20)) {
        double num = value.i;

        if (std::isnan(num)) {
            debug("TypeError, setTextSize(): first argument failed isNaN check.");
            return 1;
        }

        st[ST_TEXT_SIZE].i = std::fabs(num);
        int index = _mainWin->textSizeSelector->findText("Custom", Qt::MatchContains);
        if (index != -1) {
            _mainWin->textSizeSelector->removeItem(index);
        }
        _mainWin->textSizeSelector->addItem("Custom " + QString().setNum(num, 'f', 2) + " pt", num);
        index = _mainWin->textSizeSelector->findText("Custom", Qt::MatchContains);
        if (index != -1) {
            _mainWin->textSizeSelector->setCurrentIndex(index);
        }
        return 0;
    }
    if (!strncmp(key, "text_bold", 20)) {
        st[ST_TEXT_BOLD].b = value.b;
        return 0;
    }
    if (!strncmp(key, "text_italic", 20)) {
        st[ST_TEXT_ITALIC].b = value.b;
        return 0;
    }
    if (!strncmp(key, "text_underline", 20)) {
        st[ST_TEXT_UNDERLINE].b = value.b;
        return 0;
    }
    if (!strncmp(key, "text_strikeout", 20)) {
        st[ST_TEXT_STRIKEOUT].b = value.b;
        return 0;
    }
    if (!strncmp(key, "text_overline", 20)) {
        st[ST_TEXT_OVERLINE].b = value.b;
        return 0;
    }
    if (!strncmp(key, "prefix", 20)) {
        prompt->setPrefix(value.s);
        return 0;
    }
    return 1;
}

int
sleep_c(void)
{
    std::this_thread::sleep_for(100ms);
    return 0;
}

int
stub_testing_c(void)
{
    QMessageBox::warning(_mainWin,
        _mainWin->tr("Testing Feature"),
        _mainWin->tr("<b>This feature is in testing.</b>"));
    return 0;
}

int
undo_c(void)
{
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
    return 0;
}

int
whats_this_c(void)
{
    QWhatsThis::enterWhatsThisMode();
    return 0;
}

/* . */
int
window_c(const char *subcommand)
{
    if (!strcmp(subcommand, "cascade")) {
        mdiArea->cascade();
        return 0;
    }
    if (!strcmp(subcommand, "close")) {
        _mainWin->onCloseWindow();
        return 0;
    }
    if (!strcmp(subcommand, "close-all")) {
        mdiArea->closeAllSubWindows();
        return 0;
    }
    if (!strcmp(subcommand, "next")) {
        mdiArea->activateNextSubWindow();
        return 0;
    }
    if (!strcmp(subcommand, "previous")) {
        mdiArea->activatePreviousSubWindow();
        return 0;
    }
    if (!strcmp(subcommand, "tile")) {
        mdiArea->tile();
        return 0;
    }
    debug("ERROR: window subcommand not recognised.");
    return 1;
}

/* . */
int
zoom_c(char *subcommand)
{
    if (!strcmp(subcommand, "all")) {
        debug("TODO: Implement zoomAll.");
        return 0;
    }
    if (!strcmp(subcommand, "center")) {
        debug("TODO: Implement zoomCenter.");
        return 0;
    }
    if (!strcmp(subcommand, "dynamic")) {
        debug("TODO: Implement zoomDynamic.");
        return 0;
    }
    /* Zooms to display the drawing extents. */
    if (!strcmp(subcommand, "extents")) {
        View* gview = activeView();
        QUndoStack* stack = gview->getUndoStack();
        if (gview && stack) {
            UndoableNavCommand* cmd = new UndoableNavCommand("ZoomExtents", gview, 0);
            stack->push(cmd);
        }
        return 0;
    }
    /* Zooms to increase the apparent size of objects. */
    if (!strcmp(subcommand, "in")) {
        View* gview = activeView();
        if (gview) {
            gview->zoomIn();
        }
        return 0;
    }
    /* Zooms to decrease the apparent size of objects. */
    if (!strcmp(subcommand, "out")) {
        View* gview = activeView();
        if (gview) {
            gview->zoomOut();
        }
        return 0;
    }
    if (!strcmp(subcommand, "scale")) {
        debug("TODO: Implement zoom scale.");
        return 0;
    }
    if (!strcmp(subcommand, "previous")) {
        debug("TODO: Implement zoom previous.");
        return 0;
    }
    if (!strcmp(subcommand, "real_time")) {
        debug("TODO: Implement zoom real_time.");
        return 0;
    }
    if (!strcmp(subcommand, "selected")) {
        View* gview = activeView();
        QUndoStack* stack = gview->getUndoStack();
        if (gview && stack) {
            UndoableNavCommand* cmd = new UndoableNavCommand("ZoomSelected", gview, 0);
            stack->push(cmd);
        }
        return 0;
    }
    if (!strcmp(subcommand, "window")) {
        View* gview = activeView();
        if (gview) {
            gview->zoomWindow();
        }
        return 0;
    }
    debug("ERROR: zoom subcommand not recognised.");
    return 1;
}

/* --------------------------------------------------------------------------------- */

/* . */
int
delete_selected_c(void)
{
    View* gview = activeView();
    if (gview) {
        gview->deleteSelected();
    }
    clear_selection_c();
    return 0;
}

/* . */
int
preview_off_c(void)
{
    debug("TODO: preview_off_f");
    View* gview = activeView();
    if (gview) {
        gview->previewOff();
    }
    return 0;
}

/* . */
int
preview_on_c(const char *clone_str, const char *mode_str,
    double x, double y, double data)
{
    QString cloneStr = QString(clone_str).toUpper();
    QString modeStr = QString(mode_str).toUpper();

    int clone = PREVIEW_CLONE_NULL;
    int mode = PREVIEW_MODE_NULL;
    if (cloneStr == "SELECTED") {
        clone = PREVIEW_CLONE_SELECTED;
    }
    else if (cloneStr == "RUBBER") {
        clone = PREVIEW_CLONE_RUBBER;
    }
    else {
        debug("ERROR, preview_on_c(): first argument must be \"SELECTED\" or \"RUBBER\".");
        return 0;
    }

    if (modeStr == "MOVE") {
        mode = PREVIEW_MODE_MOVE;
    }
    else if (modeStr == "ROTATE") {
        mode = PREVIEW_MODE_ROTATE;
    }
    else if (modeStr == "SCALE") {
        mode = PREVIEW_MODE_SCALE;
    }
    else {
        debug("ERROR preview_on_c(): second argument must be \"MOVE\", \"ROTATE\" or \"SCALE\".");
        return 0;
    }

    View* gview = activeView();
    if (gview) {
        gview->previewOn(clone, mode, x, -y, data);
    }
    return 0;
}

/* . */
int
print_area_c(double x, double y, double w, double h)
{
    debug("print_area_c(%.2f, %.2f, %.2f, %.2f)", x, y, w, h);
    //TODO: Print Setup Stuff
    _mainWin->cmd("print");
    return 0;
}

/* . */
int
text_font_c(void)
{
    debug("TODO: text_font_f");
    /* return QScriptValue(st[ST_TEXT_FONT].s); */
    return 0;
}

/* . */
int
set_color_c(const char *key_, double r, double g, double b)
{
    QString key = key_;

    if ((r < 0) || (r > 255)) {
        debug("ERROR set_color(): r value must be in range 0-255");
        return 0;
    }
    if ((g < 0) || (g > 255)) {
        debug("ERROR set_color(): g value must be in range 0-255");
        return 0;
    }
    if ((b < 0) || (b > 255)) {
        debug("ERROR set_color(): b value must be in range 0-255");
        return 0;
    }

    if (key == "background") {
        st[ST_BG_COLOR].u = qRgb(r,g,b);
        _mainWin->updateAllViewBackgroundColors(qRgb(r,g,b));
    }
    else if (key == "crosshair") {
        st[ST_CROSSHAIR_COLOR].u = qRgb(r,g,b);
        _mainWin->updateAllViewCrossHairColors(qRgb(r,g,b));
    }
    else if (key == "grid") {
        st[ST_GRID_COLOR].u = qRgb(r,g,b);
        _mainWin->updateAllViewGridColors(qRgb(r,g,b));
    }
    else {
        debug("ERROR set_color(): key does not correspond to a color setting variable");
    }
    return 0;
}

/* . */
int
set_text_font_c(const char *str)
{
    debug("TODO: add_to_menu");
    _mainWin->textFontSelector->setCurrentFont(QFont(str));
    strncpy(st[ST_TEXT_FONT].s, str, 200);
    return 0;
}

/* Adds the command:
 *
 * > alert "EXAMPLE ALERT"
 */
int
alert_c(const char *msg)
{
    prompt->alert(msg);
    return 0;
}

/*
 */
int
append_prompt_history_c(const char *msg)
{
    prompt->appendHistory(msg);
    return 0;
}

/* Adds the function (blink-prompt).
 */
int
blink_c(void)
{
    prompt->startBlinking();
    return 0;
}

/* Adds the command:
 *
 * > error "EXAMPLE ERROR"
 */
int
error_c(const char *context, const char *msg)
{
    char msg_[1000];
    sprintf(msg_, "ERROR: (%s) %s", context, msg);
    prompt->setPrefix(QString(msg_));
    prompt->appendHistory(QString());
    return 0;
}

/* . */
int
color_selector_c(void)
{
    return 0;
}

/* . */
int
disable_move_rapid_fire_c(void)
{
    View* gview = activeView();
    if (gview) {
        gview->disableMoveRapidFire();
    }
    return 0;
}

/* . */
int
disable_prompt_rapid_fire_c(void)
{
    prompt->disableRapidFire();
    return 0;
}

/* . */
int
distance_c(void)
{
    return 0;
}

/* . */
int
enable_move_rapid_fire_c(void)
{
    View* gview = activeView();
    if (gview) {
        gview->enableMoveRapidFire();
    }
    return 0;
}

/* . */
int
enable_prompt_rapid_fire_c(void)
{
    prompt->enableRapidFire();
    return 0;
}

/* . */
int
erase_c(void)
{
    return 0;
}

/* . */
int
freeze_all_layers_c(void)
{
    return 0;
}

/* . */
int
hide_all_layers_c(void)
{
    return 0;
}

/* . */
int
layers_c(void)
{
    return 0;
}

/* . */
int
layer_previous_c(void)
{
    return 0;
}

/* . */
int
layer_selector_c(void)
{
    return 0;
}

int
line_type_selector_c(void)
{
    return 0;
}

int
line_weight_selector_c(void)
{
    return 0;
}

int
locate_point_c(void)
{
    return 0;
}

/* . */
int
lock_all_layers_c(void)
{
    return 0;
}

/* . */
int
make_layer_current_c(void)
{
    return 0;
}

/* . */
int
move_c(void)
{
    return 0;
}

int
quickleader_c(void)
{
    return 0;
}

int
rgb_c(void)
{
    return 0;
}

int
rotate_c(void)
{
    return 0;
}

/* . */
int
print_c(void)
{
    return 0;
}

int
scale_c(void)
{
    return 0;
}

int
show_all_layers_c(void)
{
    return 0;
}

int
syswindows_c(void)
{
    return 0;
}

/* . */
int
thaw_all_layers_c(void)
{
    return 0;
}

/* Adds the function:
 *     (todo "Missing feature description")
 */
int
todo(const char *strCmd, const char *strTodo)
{
    char msg[200];
    sprintf(msg, "TODO: (%s) %s", strCmd, strTodo);
    prompt->alert(msg);
    return 0;
}

int
unlock_all_layers_c(void)
{
    return 0;
}

int
set_cursor_shape_c(const char *shape_)
{
    View* gview = activeView();
    if (!gview) {
        return 1;
    }
    QString shape = QString(shape_).toLower();
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
    else if (shape == "resizevert"){
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
    else {
        debug("ERROR: unrecognised cursor shape.");
    }
    return 0;
}

int
calculate_angle_c(double x1, double y1, double x2, double y2)
{
    EmbVector start = emb_vector(x1, y1);
    EmbVector end = emb_vector(x2, y2);
    // QLineF(x1, -y1, x2, -y2).angle();
    return 0;
}

/* . */
int
calculate_distance_c(double x1, double y1, double x2, double y2)
{
    EmbVector start = emb_vector(x1, y1);
    EmbVector end = emb_vector(x2, y2);
    EmbVector line = emb_vector_subtract(end, start);
    // QLineF(x1, y1, x2, y2).length();
    return 0;
}

/* . */
int
perpendicular_distance_c(double px, double py, double x1, double y1, double x2, double y2)
{
    QLineF line(x1, y1, x2, y2);
    QLineF norm = line.normalVector();
    double dx = px-x1;
    double dy = py-y1;
    norm.translate(dx, dy);
    QPointF iPoint;
    norm.intersects(line, &iPoint);
//    return QLineF(px, py, iPoint.x(), iPoint.y()).length();
    return 0;
}

/* . */
int
add_to_selection_c(void)
{
    //TODO: finish, needs to take arguments:
    // add_to_selection(const QPainterPath path, Qt::ItemSelectionMode mode)
    return 0;
}

/* . */
int
cut_selected_c(double x1, double y1)
{
    EmbVector position = emb_vector(x1, -y1);
    // TODO: cut selected at position
    return 0;
}

/* . */
int
copy_selected_c(double x1, double y1)
{
    EmbVector position = emb_vector(x1, -y1);
    // TODO: copy selected at position
    return 0;
}

/* . */
int
paste_selected_c(double x1, double y1)
{
    EmbVector position = emb_vector(x1, -y1);
    // TODO: paste selected at position
    return 0;
}

/* . */
int
move_selected_c(double x1, double y1)
{
    EmbVector delta = emb_vector(x1, -y1);
    View* gview = activeView();
    if (gview) {
        gview->moveSelected(delta.x, delta.y);
    }
    return 0;
}

/* . */
int
scale_selected_c(double x, double y, double factor)
{
    if (factor <= 0.0) {
        messagebox_c("critical", "ScaleFactor Error",
            "Hi there. If you are not a developer, report this as a bug. "
            "If you are a developer, your code needs examined, and possibly your head too.");
        return 1;
    }

    View* gview = activeView();
    if (!gview) {
        return 2;
    }
    gview->scaleSelected(x, -y, factor);
    return 0;
}

/* . */
int
rotate_selected_c(double x, double y, double angle)
{
    View* gview = activeView();
    if (!gview) {
        return 2;
    }
    gview->rotateSelected(x, -y, -angle);
    return 0;
}

/* . */
int
mirror_selected_c(double x1, double y1, double x2, double y2)
{
    View* gview = activeView();
    if (!gview) {
        return 2;
    }
    gview->mirrorSelected(x1, -y1, x2, -y2);
    return 0;
}

/*
 * ----------------------------------------------------------------------------
 */

/*
 * Our preferred current time format: milliseconds since epoch as an uint64_t.
 *
 * TODO: Check for failure due to porting or the 2038 problem.
 */
uint64_t
current_time(void)
{
    const std::chrono::time_point now = std::chrono::system_clock::now();
    const auto now_t = now.time_since_epoch();
    const auto time_s = std::chrono::duration_cast<std::chrono::milliseconds>(now_t);
    return time_s.count();
}

/* TODO: Append QSysInfo to string where applicable. */
QString
MainWindow::platformString()
{
    debug("Platform: %s", OS_STR);
    return OS_STR;
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

void
MainWindow::setUndoCleanIcon(bool opened)
{
    dockUndoEdit->updateCleanIcon(opened);
}

void
MainWindow::updateAllViewScrollBars(bool val)
{
    QList<QMdiSubWindow*> windowList = mdiArea->subWindowList();
    for (int i = 0; i < windowList.count(); i++) {
        MdiWindow* mdiWin = qobject_cast<MdiWindow*>(windowList.at(i));
        if (mdiWin) {
            mdiWin->showViewScrollBars(val);
        }
    }
}

void
MainWindow::updateAllViewCrossHairColors(QRgb color)
{
    QList<QMdiSubWindow*> windowList = mdiArea->subWindowList();
    for (int i = 0; i < windowList.count(); i++) {
        MdiWindow* mdiWin = qobject_cast<MdiWindow*>(windowList.at(i));
        if (mdiWin) {
            mdiWin->setViewCrossHairColor(color);
        }
    }
}

void
MainWindow::updateAllViewBackgroundColors(QRgb color)
{
    QList<QMdiSubWindow*> windowList = mdiArea->subWindowList();
    for(int i = 0; i < windowList.count(); ++i)
    {
        MdiWindow* mdiWin = qobject_cast<MdiWindow*>(windowList.at(i));
        if(mdiWin) { mdiWin->setViewBackgroundColor(color); }
    }
}

void
MainWindow::updateAllViewSelectBoxColors(QRgb colorL, QRgb fillL, QRgb colorR, QRgb fillR, int alpha)
{
    QList<QMdiSubWindow*> windowList = mdiArea->subWindowList();
    for(int i = 0; i < windowList.count(); ++i)
    {
        MdiWindow* mdiWin = qobject_cast<MdiWindow*>(windowList.at(i));
        if(mdiWin) { mdiWin->setViewSelectBoxColors(colorL, fillL, colorR, fillR, alpha); }
    }
}

void
MainWindow::updateAllViewGridColors(QRgb color)
{
    QList<QMdiSubWindow*> windowList = mdiArea->subWindowList();
    for(int i = 0; i < windowList.count(); ++i)
    {
        MdiWindow* mdiWin = qobject_cast<MdiWindow*>(windowList.at(i));
        if(mdiWin) { mdiWin->setViewGridColor(color); }
    }
}

void
MainWindow::updateAllViewRulerColors(QRgb color)
{
    QList<QMdiSubWindow*> windowList = mdiArea->subWindowList();
    for(int i = 0; i < windowList.count(); ++i)
    {
        MdiWindow* mdiWin = qobject_cast<MdiWindow*>(windowList.at(i));
        if(mdiWin) { mdiWin->setViewRulerColor(color); }
    }
}

void
MainWindow::updatePickAddMode(bool val)
{
    st[ST_SELECTION_MODE_PICKADD].b = val;
    dockPropEdit->updatePickAddModeButton(val);
}

void
MainWindow::pickAddModeToggled()
{
    bool val = !st[ST_SELECTION_MODE_PICKADD].b;
    updatePickAddMode(val);
}

/* Layer ToolBar */
void
MainWindow::makeLayerActive()
{
    debug("makeLayerActive()");
    debug("TODO: Implement makeLayerActive.");
}

void
MainWindow::layerManager()
{
    debug("layerManager()");
    debug("TODO: Implement layerManager.");
    LayerManager layman(_mainWin, _mainWin);
    layman.exec();
}

void
MainWindow::layerPrevious()
{
    debug("layerPrevious()");
    debug("TODO: Implement layerPrevious.");
}

void
MainWindow::layerSelectorIndexChanged(int index)
{
    debug("layerSelectorIndexChanged(%d)", index);
}

void
MainWindow::colorSelectorIndexChanged(int index)
{
    debug("colorSelectorIndexChanged(%d)", index);

    QComboBox* comboBox = qobject_cast<QComboBox*>(sender());
    QRgb newColor;
    if (comboBox) {
        bool ok = 0;
        //TODO: Handle ByLayer and ByBlock and Other...
        newColor = comboBox->itemData(index).toUInt(&ok);
        if (!ok) {
            QMessageBox::warning(this, tr("Color Selector Conversion Error"), tr("<b>An error has occured while changing colors.</b>"));
        }
    }
    else {
        QMessageBox::warning(this, tr("Color Selector Pointer Error"), tr("<b>An error has occured while changing colors.</b>"));
    }

    MdiWindow* mdiWin = qobject_cast<MdiWindow*>(mdiArea->activeSubWindow());
    if (mdiWin) {
        mdiWin->currentColorChanged(newColor);
    }
}

void
MainWindow::linetypeSelectorIndexChanged(int index)
{
    debug("linetypeSelectorIndexChanged(%d)", index);
}

void
MainWindow::lineweightSelectorIndexChanged(int index)
{
    debug("lineweightSelectorIndexChanged(%d)", index);
}

void
MainWindow::textFontSelectorCurrentFontChanged(const QFont& font)
{
    debug("textFontSelectorCurrentFontChanged()");
    char command[200];
    sprintf(command, "set_text_font(%s)", qPrintable(font.family()));
    cmd(command);
}

// TODO: check that the toReal() conversion is ok
void
MainWindow::textSizeSelectorIndexChanged(int index)
{
    debug("textSizeSelectorIndexChanged(%d)", index);
    st[ST_TEXT_SIZE].i = qFabs(textSizeSelector->itemData(index).toReal());
}

QString
MainWindow::getCurrentLayer()
{
    MdiWindow* mdiWin = qobject_cast<MdiWindow*>(mdiArea->activeSubWindow());
    if (mdiWin) {
        return mdiWin->curLayer;
    }
    return "0";
}

QString MainWindow::getCurrentLineType()
{
    MdiWindow* mdiWin = qobject_cast<MdiWindow*>(mdiArea->activeSubWindow());
    if (mdiWin) {
        return mdiWin->curLineType;
    }
    return "ByLayer";
}

QString MainWindow::getCurrentLineWeight()
{
    MdiWindow* mdiWin = qobject_cast<MdiWindow*>(mdiArea->activeSubWindow());
    if (mdiWin) {
        return mdiWin->curLineWeight;
    }
    return "ByLayer";
}

void
delete_pressed(void)
{
    debug("deletePressed()");
    QApplication::setOverrideCursor(Qt::WaitCursor);
    View *gview = activeView();
    if (gview) {
        gview->deletePressed();
    }
    QApplication::restoreOverrideCursor();
}

void
escape_pressed(void)
{
    debug("escapePressed()");
    QApplication::setOverrideCursor(Qt::WaitCursor);
    View *gview = activeView();
    if (gview) {
        gview->escapePressed();
    }
    QApplication::restoreOverrideCursor();

    end_c();
}

void
MainWindow::toggleGrid()
{
    debug("toggleGrid()");
    statusbar->statusBarGridButton->toggle();
}

void
MainWindow::toggleRuler()
{
    debug("toggleRuler()");
    statusbar->statusBarRulerButton->toggle();
}

void
MainWindow::toggleLwt()
{
    debug("toggleLwt()");
    statusbar->statusBarLwtButton->toggle();
}

void
MainWindow::promptHistoryAppended(const QString& txt)
{
    MdiWindow* mdiWin = activeMdiWindow();
    if (mdiWin) {
        mdiWin->promptHistoryAppended(txt);
    }
}

void
MainWindow::logPromptInput(const QString& txt)
{
    MdiWindow* mdiWin = activeMdiWindow();
    if (mdiWin) {
        mdiWin->logPromptInput(txt);
    }
}

void
MainWindow::promptInputPrevious()
{
    MdiWindow* mdiWin = activeMdiWindow();
    if (mdiWin) {
        mdiWin->promptInputPrevious();
    }
}

void
MainWindow::promptInputNext()
{
    MdiWindow* mdiWin = activeMdiWindow();
    if (mdiWin) {
        mdiWin->promptInputNext();
    }
}

/*
 * The callback for buttons to call `cmd` via the prompt.
 */
void
MainWindow::runCommand(void)
{
    QAction* act = qobject_cast<QAction*>(sender());
    if (act) {
        debug("runCommand(%s)", qPrintable(act->objectName()));
        prompt->endCommand();
        prompt->setCurrentText(act->objectName());
        prompt->processInput();
    }
}

/*
 * The standard way to call a command.
 */
void
MainWindow::runCommandMain(const QString& line)
{
    char msg[200];
    sprintf(msg, "runCommandMain(%s)", qPrintable(line));
    debug(msg);
    // TODO: Uncomment this line when post-selection is available
    // if (!st[ST_SELECTION_MODE_PICKFIRST].b) {
    //     clear_selection();
    // }
    state.context = CONTEXT_MAIN;
    cmd(qPrintable(line));
}

void
MainWindow::runCommandClick(const QString& line, qreal x, qreal y)
{
    char msg[200];
    sprintf(msg, "runCommandClick(%s, %.2f, %.2f)", qPrintable(line), x, y);
    debug(msg);

    char mouse_pos[100];
    sprintf(mouse_pos, "mouse = {%f, %f}", x, y);
    state.context = CONTEXT_CLICK;
    cmd(qPrintable(line));
}

void
MainWindow::runCommandMove(const QString& line, qreal x, qreal y)
{
    char msg[200];
    sprintf(msg, "runCommandMove(%s, %.2f, %.2f)", qPrintable(line), x, y);
    debug(msg);

    char mouse_pos[100];
    sprintf(mouse_pos, "mouse = {%f, %f}", x, y);
    state.context = CONTEXT_MOVE;
    cmd(qPrintable(line));
}

/*
 */
void
MainWindow::runCommandContext(const QString& line, const QString& str)
{
    char msg[200];
    sprintf(msg, "runCommandContext(%s, %s)", qPrintable(line), qPrintable(str));
    debug(msg);

    char context_str[100];
    sprintf(context_str, "prompt = \"%s\"", qPrintable(str));
    state.context = CONTEXT_MENU;
    cmd(qPrintable(line));
}

/*
 */
void
MainWindow::runCommandPrompt(const QString& line, const QString& str)
{
    char msg[200];
    sprintf(msg, "runCommandPrompt(%s, %s)", qPrintable(line), qPrintable(str));
    debug(msg);

    char prompt[100];
    sprintf(prompt, "(define prompt \"%s\")", qPrintable(str));
    QString safeStr = str;
    safeStr.replace("\\", "\\\\");
    safeStr.replace("\'", "\\\'");
    /*
    if (prompt->isRapidFireEnabled()) {
        engine->evaluate(cmd + "_prompt('" + safeStr + "')", fileName);
    }
    else {
        engine->evaluate(cmd + "_prompt('" + safeStr.toUpper() + "')", fileName);
    }
    */
    state.context = CONTEXT_PROMPT;
    cmd(qPrintable(line));
}

