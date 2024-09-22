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

#include "core.h"

/* TODO: finish this */
void
doc_toggle_snap(int32_t doc, bool on)
{
    debug_message("View toggleSnap()");
    wait_cursor();
    doc_set_property(doc, "ENABLE_SNAP", on);
    doc_update(doc);
    restore_cursor();
}

/* TODO: finish this */
void
doc_toggle_polar(int32_t doc, bool on)
{
    debug_message("View togglePolar()");
    wait_cursor();
    doc_set_property(doc, "ENABLE_POLAR", on);
    doc_update(doc);
    restore_cursor();
}

/* TODO: finish this */
void
doc_toggle_ortho(int32_t doc, bool on)
{
    debug_message("View toggleOrtho()");
    wait_cursor();
    doc_set_property(doc, "ENABLE_ORTHO", on);
    doc_update(doc);
    restore_cursor();
}

/* TODO: finish this */
void
doc_toggle_qtrack(int32_t doc, bool on)
{
    debug_message("View toggleQTrack()");
    wait_cursor();
    doc_set_property(doc, "ENABLE_QTRACK", on);
    doc_update(doc);
    restore_cursor();
}

/* . */
void
doc_toggle_lwt(int32_t doc, bool on)
{
    debug_message("View toggleLwt()");
    wait_cursor();
    doc_set_property(doc, "ENABLE_LWT", on);
    doc_update(doc);
    restore_cursor();
}

/* . */
void
doc_toggle_real(int32_t doc, bool on)
{
    debug_message("View toggleReal()");
    wait_cursor();
    doc_set_property(doc, "ENABLE_REAL", on);
    doc_update(doc);
    restore_cursor();
}

/* . */
bool
doc_is_lwt_enabled(int32_t doc)
{
    return doc_get_property(doc, "ENABLE_LWT");
}

/* . */
bool
doc_is_real_enabled(int32_t doc)
{
    return doc_get_property(doc, "ENABLE_REAL");
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
doc_enable_move_rapid_fire(int32_t doc)
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

