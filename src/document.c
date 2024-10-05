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

#include "core.h"

double zoomInLimit = 1.0e-10;
double zoomOutLimit = 1.0e10;

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
    if (doc_num_selected(doc) == 0) {
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
    if (doc_num_selected(doc) == 0) {
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

