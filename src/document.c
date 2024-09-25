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
    doc_set_bool(doc, "ENABLE_SNAP", on);
    doc_update(doc);
    restore_cursor();
}

/* TODO: finish this */
void
doc_toggle_polar(int32_t doc, bool on)
{
    debug_message("View togglePolar()");
    wait_cursor();
    doc_set_bool(doc, "ENABLE_POLAR", on);
    doc_update(doc);
    restore_cursor();
}

/* TODO: finish this */
void
doc_toggle_ortho(int32_t doc, bool on)
{
    debug_message("View toggleOrtho()");
    wait_cursor();
    doc_set_bool(doc, "ENABLE_ORTHO", on);
    doc_update(doc);
    restore_cursor();
}

/* TODO: finish this */
void
doc_toggle_qtrack(int32_t doc, bool on)
{
    debug_message("View toggleQTrack()");
    wait_cursor();
    doc_set_bool(doc, "ENABLE_QTRACK", on);
    doc_update(doc);
    restore_cursor();
}

/* . */
void
doc_toggle_lwt(int32_t doc, bool on)
{
    debug_message("View toggleLwt()");
    wait_cursor();
    doc_set_bool(doc, "ENABLE_LWT", on);
    doc_update(doc);
    restore_cursor();
}

/* . */
void
doc_toggle_real(int32_t doc, bool on)
{
    debug_message("View toggleReal()");
    wait_cursor();
    doc_set_bool(doc, "ENABLE_REAL", on);
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
    doc_set_bool(doc, "ENABLE_QSNAP", on);
    doc_update(doc);
    restore_cursor();
}

/* . */
bool
doc_is_lwt_enabled(int32_t doc)
{
    return doc_get_bool(doc, "ENABLE_LWT");
}

/* . */
bool
doc_is_real_enabled(int32_t doc)
{
    return doc_get_bool(doc, "ENABLE_REAL");
}


/* . */
void
doc_zoom_window(int32_t doc)
{
}

/* . */
void
doc_zoom_in(int32_t doc)
{
}

/* . */
void
doc_zoom_out(int32_t doc)
{
}

/* . */
void
doc_zoom_selected(int32_t doc)
{
}

/* . */
void
appendHistory(EmbString s)
{
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

