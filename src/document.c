/*
 * Embroidermodder 2.
 *
 * Copyright 2011-2024 The Embroidermodder Team
 * Embroidermodder 2 is Open Source Software, see LICENSE.md for licensing terms.
 *
 * Read the reference manual (https://www.libembroidery.org/downloads/emrm.pdf)
 * for advice on altering this file.
 *
 * Main Embroidermodder Header
 */

#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#include "core.h"

double zoomInLimit = 1.0e-10;
double zoomOutLimit = 1.0e10;

/* . */
void
doc_init(int32_t doc)
{
    DocumentData *data = doc_data(doc);

    data->selected_memory = 1000;
    data->selected_items = (int*)malloc(data->selected_memory*sizeof(int));
    data->n_selected = 0;

    /* NOTE: This has to be done before setting mouse tracking.
     * TODO: Review OpenGL for Qt5 later
     * if (get_bool(DISPLAY_USE_OPENGL)) {
     *     debug_message("Using OpenGL...");
     *     setViewport(new QGLWidget(QGLFormat(QGL::DoubleBuffer)));
     * }

     * TODO: Review RenderHints later
     * setRenderHint(QPainter::Antialiasing, get_bool(DISPLAY_RENDERHINT_AA));
     * setRenderHint(QPainter::TextAntialiasing, get_bool(DISPLAY_RENDERHINT_TEXT_AA));
     * setRenderHint(QPainter::SmoothPixmapTransform, get_bool(DISPLAY_RENDERHINT_SMOOTHPIX));
     * setRenderHint(QPainter::HighQualityAntialiasing, get_bool(DISPLAY_RENDERHINT_HIGH_AA));
     * setRenderHint(QPainter::NonCosmeticDefaultPen, get_bool(DISPLAY_RENDERHINT_NONCOSMETIC));

     * NOTE: FullViewportUpdate MUST be used for both the GL and Qt renderers.
     * NOTE: Qt renderer will not draw the foreground properly if it isnt set.
     */

    data->panDistance = 10; /* TODO: should there be a setting for this??? */

    data->grippingActive = false;
    data->rapidMoveActive = false;
    data->previewMode = PREVIEW_NULL;
    data->previewData = 0;
    data->previewActive = false;
    data->pastingActive = false;
    data->movingActive = false;
    data->selectingActive = false;
    data->zoomWindowActive = false;
    data->panningRealTimeActive = false;
    data->panningPointActive = false;
    data->panningActive = false;
    data->qSnapActive = false;
    data->qSnapToggle = false;

    data->qsnapLocatorColor = get_int(QSNAP_LOCATOR_COLOR);
    data->qsnapLocatorSize = get_int(QSNAP_LOCATOR_SIZE);
    data->qsnapApertureSize = get_int(QSNAP_APERTURE_SIZE);
    data->gripColorCool = get_int(SELECTION_COOLGRIP_COLOR);
    data->gripColorHot = get_int(SELECTION_HOTGRIP_COLOR);
    data->gripSize = get_int(SELECTION_GRIP_SIZE);
    data->pickBoxSize = get_int(SELECTION_PICKBOX_SIZE);

    /* TODO: set up config */
    data->enableRuler = true;
    data->enableGrid = true;
    data->enableOrtho = false;
    data->enablePolar = false;
    data->enableLwt = false;
    data->enableRuler = true;

    /* Randomize the hot grip location initially so it's not located at (0,0). */
    srand(time(NULL));
    data->sceneGripPoint = emb_vector(rand()*1000, rand()*1000);

    strcpy(data->curLayer, "0");
    data->curColor = 0; /* TODO: color ByLayer */
    strcpy(data->curLineType, "ByLayer");
    strcpy(data->curLineWeight, "ByLayer");

    data->pattern = emb_pattern_create();
    if (!data->pattern) {
        printf("Could not allocate memory for embroidery pattern\n");
        exit(1);
    }
}

/* . */
void
doc_clear_selection(int32_t doc)
{
    if (doc >= numOfDocs) {
        return;
    }
    DocumentData *data = doc_data(doc);
    data->n_selected = 0;
}

/* TODO: finish this */
void
doc_toggle_snap(int32_t doc, bool on)
{
    debug_message("View toggleSnap()");
    wait_cursor();
    DocumentData *data = doc_data(doc);
    data->enableSnap = on;
    doc_update(doc);
    restore_cursor();
}

/* TODO: finish this */
void
doc_toggle_polar(int32_t doc, bool on)
{
    debug_message("View togglePolar()");
    wait_cursor();
    DocumentData *data = doc_data(doc);
    data->enablePolar = on;
    doc_update(doc);
    restore_cursor();
}

/* TODO: finish this */
void
doc_toggle_ortho(int32_t doc, bool on)
{
    debug_message("View toggleOrtho()");
    wait_cursor();
    DocumentData *data = doc_data(doc);
    data->enableOrtho = on;
    doc_update(doc);
    restore_cursor();
}

/* TODO: finish this */
void
doc_toggle_qtrack(int32_t doc, bool on)
{
    debug_message("View toggleQTrack()");
    wait_cursor();
    DocumentData *data = doc_data(doc);
    data->enableQTrack = on;
    doc_update(doc);
    restore_cursor();
}

/* . */
void
doc_toggle_lwt(int32_t doc, bool on)
{
    debug_message("View toggleLwt()");
    wait_cursor();
    DocumentData *data = doc_data(doc);
    data->enableLwt = on;
    doc_update(doc);
    restore_cursor();
}

/* . */
void
doc_toggle_real(int32_t doc, bool on)
{
    debug_message("View toggleReal()");
    wait_cursor();
    DocumentData *data = doc_data(doc);
    data->enableReal = on;
    doc_update(doc);
    restore_cursor();
}

/* . */
void
doc_toggle_qsnap(int32_t doc, bool on)
{
    debug_message("View toggleQSnap()");
    wait_cursor();
    DocumentData *data = doc_data(doc);
    data->qSnapToggle = on;
    data->enableQSnap = on;
    doc_update(doc);
    restore_cursor();
}

/* . */
void
doc_zoom_in(int32_t doc)
{
    debug_message("View zoomIn()");
    if (!doc_allow_zoom_in(doc)) {
        return;
    }
    wait_cursor();
    EmbVector cntr = doc_map_to_scene(doc, doc_center(doc));
    double s = get_real(DISPLAY_ZOOMSCALE_IN);
    doc_scale(s, s);

    doc_center_on(doc, cntr);
    restore_cursor();
}

/* . */
void
doc_zoom_out(int32_t doc)
{
    debug_message("View zoomOut()");
    if (!doc_allow_zoom_out(doc)) {
        return;
    }
    wait_cursor();
    EmbVector cntr = doc_map_to_scene(doc, doc_center(doc));
    double s = get_real(DISPLAY_ZOOMSCALE_OUT);
    doc_scale(doc, s);

    doc_center_on(doc, cntr);
    restore_cursor();
}

/* . */
void
doc_zoom_window(int32_t doc)
{
    DocumentData *data = doc_data(doc);
    data->zoomWindowActive = true;
    data->selectingActive = false;
    doc_clear_selection(doc);
}

/* . */
void
appendHistory(EmbString s)
{
}

/* . */
void
updateColorLinetypeLineweight(void)
{
}

/* . */
void
doc_cut(int32_t doc)
{
    DocumentData *data = doc_data(doc);
    if (data->n_selected <= 0) {
        information_box(translate("Cut Preselect"),
            translate("Preselect objects before invoking the cut command."));
        return; /* TODO: Prompt to select objects if nothing is preselected */
    }

    doc_begin_macro(doc, "Cut");
    doc_copy_selected(doc);
    doc_delete_selected(doc);
    doc_end_macro(doc);
}

/* . */
void
doc_create_grid(int32_t doc, EmbString gridType)
{
    DocumentData *data = doc_data(doc);
    if (string_equal(gridType, "Rectangular")) {
        doc_create_grid_rect(doc);
        data->enableGrid = true;
    }
    else if (string_equal(gridType, "Circular")) {
        doc_create_grid_polar(doc);
        data->enableGrid = true;
    }
    else if (string_equal(gridType, "Isometric")) {
        doc_create_grid_iso(doc);
        data->enableGrid = true;
    }
    else {
        doc_empty_grid(doc);
        data->enableGrid = false;
    }

    doc_create_origin(doc);

    doc_update(doc);
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
statusbar_toggle(EmbString key, bool on)
{
    debug_message("StatusBarButton toggleSnap()");
    int32_t doc = activeDocument();
    if (doc < 0) {
        return;
    }
    if (string_equal(key, "SNAP")) {
        doc_toggle_snap(doc, on);
    }
    else if (string_equal(key, "GRID")) {
        doc_toggle_grid(doc, on);
    }
    else if (string_equal(key, "RULER")) {
        doc_toggle_ruler(doc, on);
    }
    else if (string_equal(key, "ORTHO")) {
        doc_toggle_ortho(doc, on);
    }
    else if (string_equal(key, "POLAR")) {
        doc_toggle_polar(doc, on);
    }
    else if (string_equal(key, "QSNAP")) {
        doc_toggle_qsnap(doc, on);
    }
    else if (string_equal(key, "LWT")) {
        doc_toggle_lwt(doc, on);
    }
}

/* . */
void
doc_copy(int32_t doc)
{
    DocumentData *data = doc_data(doc);
    if (data->n_selected <= 0) {
        information_box(translate("Copy Preselect"),
            translate("Preselect objects before invoking the copy command."));
        return; /* TODO: Prompt to select objects if nothing is preselected */
    }

    doc_copy_selected(doc);
    doc_clear_selection(doc);
}

/* . */
void
doc_center_at(int32_t doc, EmbVector centerPoint)
{
    /* centerOn also updates the scrollbars, which shifts things out of wack o_O */
    doc_center_on(doc, centerPoint);
    /* Reshift to the new center */
    EmbVector offset = emb_vector_subtract(centerPoint, doc_center(doc));
    EmbVector newCenter = emb_vector_add(centerPoint, offset);
    doc_center_on(doc, newCenter);
}

/* . */
void
doc_pan_real_time(int32_t doc)
{
    DocumentData *data = doc_data(doc);
    data->panningRealTimeActive = true;
}

/* . */
void
doc_pan_point(int32_t doc)
{
    DocumentData *data = doc_data(doc);
    data->panningPointActive = true;
}

/* . */
bool
doc_allow_zoom_in(int32_t doc)
{
    EmbVector origin = doc_map_to_scene(doc, emb_vector(0.0, 0.0));
    EmbVector corner = doc_map_to_scene(doc,
        emb_vector(doc_width(doc), doc_height(doc)));
    double maxWidth = corner.x - origin.x;
    double maxHeight = corner.y - origin.y;

    if (EMB_MIN(maxWidth, maxHeight) < zoomInLimit) {
        char message[MAX_STRING_LENGTH];
        sprintf(message, "ZoomIn limit reached. (limit=%.10f)", zoomInLimit);
        debug_message(message);
        return false;
    }

    return true;
}

/* . */
bool
doc_allow_zoom_out(int32_t doc)
{
    EmbVector origin = doc_map_to_scene(doc, emb_vector(0.0, 0.0));
    EmbVector corner = doc_map_to_scene(doc,
        emb_vector(doc_width(doc), doc_height(doc)));
    double maxWidth = corner.x - origin.x;
    double maxHeight = corner.y - origin.y;

    if (EMB_MAX(maxWidth, maxHeight) > zoomOutLimit) {
        char message[MAX_STRING_LENGTH];
        sprintf(message, "ZoomOut limit reached. (limit=%.1f)", zoomOutLimit);
        debug_message(message);
        return false;
    }

    return true;
}

/* . */
void
doc_enable_move_rapid_fire(int32_t doc)
{
    DocumentData *data = doc_data(doc);
    data->rapidMoveActive = true;
}

/* . */
void
doc_disable_move_rapid_fire(int32_t doc)
{
    DocumentData *data = doc_data(doc);
    data->rapidMoveActive = false;
}

/* . */
bool
doc_allow_rubber(int32_t doc)
{
    /* if (!data->rubberRoomList.size()) */ /* TODO: this check should be removed later */
        return true;
    return false;
}

/* . */
void
doc_set_grid_color(int32_t doc, uint32_t color)
{
    DocumentData *data = doc_data(doc);
    data->gridColor = color;
    doc_update(doc);
}

/* . */
void
doc_set_ruler_color(int32_t doc, uint32_t color)
{
    DocumentData *data = doc_data(doc);
    data->rulerColor = color;
    doc_update(doc);
}

/* . */
void
doc_set_cross_hair_color(int32_t doc, uint32_t color)
{
    DocumentData *data = doc_data(doc);
    data->crosshairColor = color;
    doc_update(doc);
}

/* . */
void
doc_toggle_grid(int32_t doc, bool on)
{
    debug_message("View toggleGrid()");
    wait_cursor();
    if (on) {
        doc_create_grid(doc, get_str(GRID_TYPE));
    }
    else {
        doc_create_grid(doc, "");
    }
    restore_cursor();
}

/* . */
void
doc_toggle_ruler(int32_t doc, bool on)
{
    DocumentData *data = doc_data(doc);
    debug_message("View toggleRuler()");
    wait_cursor();
    data->enableRuler = on;
    data->rulerMetric = get_bool(RULER_METRIC);
    data->rulerColor = get_int(RULER_COLOR);
    data->rulerPixelSize = get_int(RULER_PIXEL_SIZE);
    doc_update(doc);
    restore_cursor();
}

/* . */
void
doc_deletePressed(int32_t doc)
{
    debug_message("View deletePressed()");
    DocumentData *data = doc_data(doc);
    if (data->pastingActive) {
        remove_paste_object_item_group(doc);
    }
    data->pastingActive = false;
    data->zoomWindowActive = false;
    data->selectingActive = false;
    hide_selectbox(doc);
    doc_stop_gripping(doc, false);
    doc_delete_selected(doc);
}

/* . */
void
doc_escapePressed(int32_t doc)
{
    debug_message("View escapePressed()");
    DocumentData *data = doc_data(doc);
    if (data->pastingActive) {
        remove_paste_object_item_group(doc);
    }
    data->pastingActive = false;
    data->zoomWindowActive = false;
    data->selectingActive = false;
    hide_selectbox(doc);
    if (data->grippingActive) {
        doc_stop_gripping(doc, false);
    }
    else {
        doc_clear_selection(doc);
    }
}

/* . */
void
doc_delete_selected(int32_t doc)
{
    DocumentData *data = doc_data(doc);
    if (data->n_selected > 1) {
        EmbString macro_name;
        sprintf(macro_name, "%s %d", translate("Delete"), data->n_selected);
        doc_begin_macro(doc, macro_name);
    }
    for (int i = 0; i < data->n_selected; i++) {
        ObjectCore* core = obj_get_core(data->selected_items[i]);
        if (core->geometry->type != OBJ_NULL) {
            EmbString label;
            sprintf(label, "%s%s", translate("Delete 1 "), core->OBJ_NAME);
            undoable_delete(doc, data->selected_items[i], label);
        }
    }
    if (data->n_selected > 1) {
        doc_end_macro(doc);
    }
}

/* . */
void
doc_move_selected(int32_t doc, EmbVector delta)
{
    DocumentData *data = doc_data(doc);
    if (data->n_selected > 1) {
        EmbString macro_name;
        sprintf(macro_name, "%s %d", translate("Move"), data->n_selected);
        doc_begin_macro(doc, macro_name);
    }
    for (int i = 0; i < data->n_selected; i++) {
        ObjectCore* core = obj_get_core(data->selected_items[i]);
        EmbString msg;
        sprintf(msg, "%s 1 %s", translate("Move"), core->OBJ_NAME);
        undoable_move(doc, data->selected_items[i], delta, msg);
    }
    if (data->n_selected > 1) {
        doc_end_macro(doc);
    }

    /* Always clear the selection after a move. */
    doc_clear_selection(doc);
}

/* . */
void
doc_rotate_selected(int32_t doc, double x, double y, double rot)
{
    DocumentData *data = doc_data(doc);
    if (data->n_selected > 1) {
        EmbString macro_name;
        sprintf(macro_name, "%s %d", translate("Rotate"), data->n_selected);
        doc_begin_macro(doc, macro_name);
    }
    for (int i = 0; i < data->n_selected; i++) {
        ObjectCore* core = obj_get_core(data->selected_items[i]);
        EmbString msg;
        sprintf(msg, "%s 1 %s", translate("Rotate"), core->OBJ_NAME);
        EmbVector v = emb_vector(x, y);
        undoable_rotate(doc, data->selected_items[i], v, msg);
    }
    if (data->n_selected > 1) {
        doc_end_macro(doc);
    }

    /* Always clear the selection after a rotate. */
    doc_clear_selection(doc);
}

/* . */
void
doc_mirror_selected(int32_t doc, double x1, double y1, double x2, double y2)
{
    DocumentData *data = doc_data(doc);
    if (data->n_selected > 1) {
        EmbString macro_name;
        sprintf(macro_name, "%s %d", translate("Mirror"), data->n_selected);
        doc_begin_macro(doc, macro_name);
    }
    for (int i = 0; i < data->n_selected; i++) {
        ObjectCore* core = obj_get_core(data->selected_items[i]);
        EmbString msg;
        sprintf(msg, "%s 1 %s", translate("Mirror"), core->OBJ_NAME);
        EmbVector start, end;
        start = emb_vector(x1, y1);
        end = emb_vector(x2, y2);
        undoable_mirror(doc, data->selected_items[i], start, end, msg);
    }
    if (data->n_selected > 1) {
        doc_end_macro(doc);
    }

    /* Always clear the selection after a mirror. */
    doc_clear_selection(doc);
}

/* . */
void
doc_scale_selected(int32_t doc, double x, double y, double factor)
{
    DocumentData *data = doc_data(doc);
    if (data->n_selected > 1) {
        EmbString macro_name;
        sprintf(macro_name, "%s %d", translate("Scale"), data->n_selected);
        doc_begin_macro(doc, macro_name);
    }
    for (int i = 0; i < data->n_selected; i++) {
        ObjectCore* core = obj_get_core(data->selected_items[i]);
        EmbVector v = emb_vector(x, y);
        EmbString msg;
        sprintf(msg, "%s%s", translate("Scale 1 "), core->OBJ_NAME);
        undoable_scale(doc, data->selected_items[i], v, factor, msg);
    }
    if (data->n_selected > 1) {
        doc_end_macro(doc);
    }

    /* Always clear the selection after a scale. */
    doc_clear_selection(doc);
}

