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

    data->selectedItems = create_id_list();
    data->rubberRoomList = create_id_list();
    data->previewObjectList = create_id_list();
    data->spareRubberList = create_id_list();
    data->previewObjectItemGroup = create_id_list();
    data->pasteObjectItemGroup = create_id_list();
    data->hashDeletedObjects = create_id_list();

    data->selectedGripPoints = create_vector_list();

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

/* TODO: review for memory leaks. */
void
free_doc(int32_t doc)
{
    DocumentData *data = doc_data(doc);

    document_memory[doc] = false;

    /* Prevent memory leaks by deleting any objects that were removed from the scene */
    free_objects(data->hashDeletedObjects);

    /* Prevent memory leaks by deleting any unused instances. */
    free_objects(data->previewObjectList);

    free_id_list(data->selectedItems);
    free_id_list(data->rubberRoomList);
    free_id_list(data->previewObjectList);
    free_id_list(data->spareRubberList);
    free_id_list(data->hashDeletedObjects);

    free_vector_list(data->selectedGripPoints);
}

/* . */
void
doc_clear_selection(int32_t doc)
{
    if (doc >= numOfDocs) {
        return;
    }
    DocumentData *data = doc_data(doc);
    data->selectedItems->count = 0;
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
    EmbReal s = get_real(DISPLAY_ZOOMSCALE_IN);
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
    EmbReal s = get_real(DISPLAY_ZOOMSCALE_OUT);
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
doc_preview_on(int32_t doc, int clone, int mode, EmbReal x, EmbReal y, EmbReal data_)
{
    debug_message("View previewOn()");
    DocumentData *data = doc_data(doc);
    doc_preview_off(doc); /* Free the old objects before creating new ones */

    data->previewMode = mode;

    /* Create new objects and add them to the scene in an item group. */
    if (clone == PREVIEW_CLONE_SELECTED) {
        copy_object_list(data->previewObjectList, data->selectedItems);
    }
    else if (clone == PREVIEW_CLONE_RUBBER) {
        copy_object_list(data->previewObjectList, data->rubberRoomList);
    }
    else {
        return;
    }
    //FIXME: data->previewObjectItemGroup = documents[doc]->gscene->createItemGroup(documents[doc]->previewObjectList);

    if (data->previewMode == PREVIEW_MOVE ||
       data->previewMode == PREVIEW_ROTATE ||
       data->previewMode == PREVIEW_SCALE) {
        data->previewPoint.x = x;
        data->previewPoint.y = y; /* NOTE: Move: basePt; Rotate: basePt; Scale: basePt; */
        data->previewData = data_;           /* NOTE: Move: unused; Rotate: refAngle; Scale: refFactor; */
        data->previewActive = true;
    }
    else {
        data->previewMode = PREVIEW_NULL;
        data->previewPoint = emb_vector(0.0, 0.0);
        data->previewData = 0;
        data->previewActive = false;
    }

    doc_update(doc);
}

/* . */
void
doc_preview_off(int32_t doc)
{
    /* Prevent memory leaks by deleting any unused instances */
    DocumentData *data = doc_data(doc);
    free_objects(data->previewObjectList);
    data->previewObjectList->count = 0;

    if (data->previewObjectItemGroup->count > 0) {
        free_objects(data->previewObjectItemGroup);
        data->previewObjectList->count = 0;
    }

    data->previewActive = false;

    doc_update(doc);
}

/* . */
void
doc_spare_rubber(int32_t doc, int64_t id)
{
    DocumentData *data = doc_data(doc);
    append_id_to_list(data->spareRubberList, id);
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
    if (data->selectedItems->count <= 0) {
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
    if (data->selectedItems->count <= 0) {
        information_box(translate("Copy Preselect"),
            translate("Preselect objects before invoking the copy command."));
        return; /* TODO: Prompt to select objects if nothing is preselected */
    }

    doc_copy_selected(doc);
    doc_clear_selection(doc);
}
/* . */
void
doc_set_rubber_mode(int32_t doc, int mode)
{
    DocumentData *data = doc_data(doc);
    for (int i=0; i<data->rubberRoomList->count; i++) {
        int32_t item = data->rubberRoomList->data[i];
        obj_set_rubber_mode(item, mode);
    }
    doc_update(doc);
}

/* . */
void
doc_set_rubber_point(int32_t doc, EmbString key, EmbVector point)
{
    DocumentData *data = doc_data(doc);
    for (int i=0; i<data->rubberRoomList->count; i++) {
        int32_t item = data->rubberRoomList->data[i];
        obj_set_rubber_point(item, key, point);
    }
    doc_update(doc);
}

/* . */
void
doc_set_rubber_text(int32_t doc, EmbString key, EmbString txt)
{
    DocumentData *data = doc_data(doc);
    for (int i=0; i<data->rubberRoomList->count; i++) {
        int32_t item = data->rubberRoomList->data[i];
        obj_set_rubber_text(item, key, txt);
    }
    doc_update(doc);
}

/* The caller is responsible for allocating the memory. */
void
copy_object_list(EmbIdList *dst, EmbIdList *src)
{
    dst->count = 0;
    for (int i = 0; i < src->count; i++) {
        uint32_t copyObj = copy_object(src->data[i]);
        append_id_to_list(dst, copyObj);
    }
}

/* . */
void
doc_copy_selected(int32_t doc)
{
    DocumentData *data = doc_data(doc);

    /* Prevent memory leaks by deleting any unpasted instances */
    free_objects(data->cutCopyObjectList);

    /* Create new objects but do not add them to the scene just yet.
     * By creating them now, ensures that pasting will still work
     * if the original objects are deleted before the paste occurs.
     */
    copy_object_list(data->cutCopyObjectList, data->selectedItems);
}

/* . */
void
doc_vulcanize_rubber_room(int32_t doc)
{
    DocumentData *data = doc_data(doc);
    for (int i=0; i<data->rubberRoomList->count; i++) {
        int32_t item = data->rubberRoomList->data[i];
        doc_vulcanize_object(doc, item);
    }
    data->rubberRoomList->count = 0;
    doc_update(doc);
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
    EmbReal maxWidth = corner.x - origin.x;
    EmbReal maxHeight = corner.y - origin.y;

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
    EmbReal maxWidth = corner.x - origin.x;
    EmbReal maxHeight = corner.y - origin.y;

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
    if (data->selectedItems->count > 1) {
        EmbString macro_name;
        sprintf(macro_name, "%s %d", translate("Delete"), data->selectedItems->count);
        doc_begin_macro(doc, macro_name);
    }
    for (int i = 0; i < data->selectedItems->count; i++) {
        ObjectCore* core = obj_get_core(data->selectedItems->data[i]);
        if (core->geometry->type != OBJ_NULL) {
            EmbString label;
            sprintf(label, "%s%s", translate("Delete 1 "), core->OBJ_NAME);
            undoable_delete(doc, data->selectedItems->data[i], label);
        }
    }
    if (data->selectedItems->count > 1) {
        doc_end_macro(doc);
    }
}

/* . */
void
doc_move_selected(int32_t doc, EmbVector delta)
{
    DocumentData *data = doc_data(doc);
    if (data->selectedItems->count > 1) {
        EmbString macro_name;
        sprintf(macro_name, "%s %d", translate("Move"), data->selectedItems->count);
        doc_begin_macro(doc, macro_name);
    }
    for (int i = 0; i < data->selectedItems->count; i++) {
        ObjectCore* core = obj_get_core(data->selectedItems->data[i]);
        EmbString msg;
        sprintf(msg, "%s 1 %s", translate("Move"), core->OBJ_NAME);
        undoable_move(doc, data->selectedItems->data[i], delta, msg);
    }
    if (data->selectedItems->count > 1) {
        doc_end_macro(doc);
    }

    /* Always clear the selection after a move. */
    doc_clear_selection(doc);
}

/* . */
void
doc_rotate_selected(int32_t doc, EmbReal x, EmbReal y, EmbReal rot)
{
    DocumentData *data = doc_data(doc);
    if (data->selectedItems->count > 1) {
        EmbString macro_name;
        sprintf(macro_name, "%s %d", translate("Rotate"), data->selectedItems->count);
        doc_begin_macro(doc, macro_name);
    }
    for (int i = 0; i < data->selectedItems->count; i++) {
        ObjectCore* core = obj_get_core(data->selectedItems->data[i]);
        EmbString msg;
        sprintf(msg, "%s 1 %s", translate("Rotate"), core->OBJ_NAME);
        EmbVector v = emb_vector(x, y);
        undoable_rotate(doc, data->selectedItems->data[i], v, msg);
    }
    if (data->selectedItems->count > 1) {
        doc_end_macro(doc);
    }

    /* Always clear the selection after a rotate. */
    doc_clear_selection(doc);
}

/* . */
void
doc_mirror_selected(int32_t doc, EmbReal x1, EmbReal y1, EmbReal x2, EmbReal y2)
{
    DocumentData *data = doc_data(doc);
    if (data->selectedItems->count > 1) {
        EmbString macro_name;
        sprintf(macro_name, "%s %d", translate("Mirror"), data->selectedItems->count);
        doc_begin_macro(doc, macro_name);
    }
    for (int i = 0; i < data->selectedItems->count; i++) {
        ObjectCore* core = obj_get_core(data->selectedItems->data[i]);
        EmbString msg;
        sprintf(msg, "%s 1 %s", translate("Mirror"), core->OBJ_NAME);
        EmbVector start, end;
        start = emb_vector(x1, y1);
        end = emb_vector(x2, y2);
        undoable_mirror(doc, data->selectedItems->data[i], start, end, msg);
    }
    if (data->selectedItems->count > 1) {
        doc_end_macro(doc);
    }

    /* Always clear the selection after a mirror. */
    doc_clear_selection(doc);
}

/* . */
void
doc_scale_selected(int32_t doc, EmbReal x, EmbReal y, EmbReal factor)
{
    DocumentData *data = doc_data(doc);
    if (data->selectedItems->count > 1) {
        EmbString macro_name;
        sprintf(macro_name, "%s %d", translate("Scale"), data->selectedItems->count);
        doc_begin_macro(doc, macro_name);
    }
    for (int i = 0; i < data->selectedItems->count; i++) {
        ObjectCore* core = obj_get_core(data->selectedItems->data[i]);
        EmbVector v = emb_vector(x, y);
        EmbString msg;
        sprintf(msg, "%s%s", translate("Scale 1 "), core->OBJ_NAME);
        undoable_scale(doc, data->selectedItems->data[i], v, factor, msg);
    }
    if (data->selectedItems->count > 1) {
        doc_end_macro(doc);
    }

    /* Always clear the selection after a scale. */
    doc_clear_selection(doc);
}

