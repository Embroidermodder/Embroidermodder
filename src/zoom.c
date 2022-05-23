/* 
 *  Embroidermodder 2.
 *
 *  ------------------------------------------------------------
 *
 *  Copyright 2013-2022 The Embroidermodder Team
 *  Embroidermodder 2 is Open Source Software.
 *  See LICENSE for licensing terms.
 *
 *  ------------------------------------------------------------
 *
 *  Use Python's PEP7 style guide.
 *      https://peps.python.org/pep-0007/
 *
 *  Zoom and Pan functions.
 */

#include "em2.h"

/* .
 */
pointer
scm_allow_zoom_in(scheme *sc, pointer args)
{
/*
    origin = map_to_scene(0, 0);
    corner = map_to_scene(width(), height());
    max_size = corner.subtract(origin);

    if (emb_min(max_size.x, max_size.y) < zoom_in_limit) {
        debug_message("zoom_in limit reached. (limit=%.10f)" % zoom_in_limit)
        return 0;
    }
    return 1;*/
    return sc->NIL;
}

/* .
 */
pointer
scm_allow_zoom_out(scheme *sc, pointer args)
{
	/*
    origin = map_to_scene(0, 0);
    corner = map_to_scene(width(), height())
    max_size = corner.subtract(origin);

    if max(max_size.x, max_size.y) > zoom_out_limit:
        debug_message("zoom_out limit reached. (limit=%.1f)" % zoom_out_limit);
        return 0;

    return 1; */
    return sc->NIL;
}

/* .
 */
pointer
scm_zoom_in(scheme *sc, pointer args)
{
    debug_message("zoom_in()");
    debug_message("View zoom_in()");
	/*
    if (!allow_zoom_in()) {
        return;
    }

    set_override_cursor("Wait Cursor");
    cntr = map_to_scene(Vector(width()/2, height()/2));
    s = display_zoom_scale_in;
    scale(s, s);

    center_on(cntr);
    restore_override_cursor();
    */
    return sc->NIL;
}

/* .
 */
pointer
scm_zoom_out(scheme *sc, pointer args)
{
    debug_message("zoom_out()");
    debug_message("View zoom_out()");
	/*
    if (!allow_zoom_out()
        return;

    set_override_cursor("Wait Cursor");
    cntr = map_to_scene(Vector(width()/2, height()/2));
    s = display_zoom_scale_out;
    scale(s, s);

    center_on(cntr);
    restore_override_cursor();
    */
    return sc->NIL;
}

/* . */
pointer
scm_zoom_selected(scheme *sc, pointer args)
{
    debug_message("zoom_selected()");
	/*
    set_override_cursor("Wait Cursor");
    item_list = gscene.selected_items();
    selected_rect_path = Path();
    for (item in item_list) {
        selected_rect_path.add_polygon(item.map_to_scene(item.bounding_rect()));
    }

    selected_rect = selected_rectPath.bounding_rect()
    if (selected_rect) {
        message = translate("Preselect objects before invoking the zoom_selected command.")
        information(translate("zoom_selected Preselect"), message) */
        /* TODO: Support Post selection of objects */ /*
    }

    fit_in_view(selected_rect, "KeepAspectRatio")
    restore_override_cursor() */
    return sc->NIL;
}

/* .
 */
pointer
scm_zoom_scale(scheme *sc, pointer args)
{
    debug_message("zoom_scale()");
    debug_message("Implement zoom_scale.");
    return sc->NIL;
}

/* .
 */
pointer
scm_zoom_center(scheme *sc, pointer args)
{
    debug_message("zoom_center()");
    debug_message("Implement zoom_center.");
    return sc->NIL;
}

/* .
 */
pointer
scm_zoom_all(scheme *sc, pointer args)
{
    debug_message("zoom_all()");
    debug_message("Implement zoom_all.");
    return sc->NIL;
}

/* .
 */
pointer
scm_zoom_extents(scheme *sc, pointer args)
{
    debug_message("zoom_extents()");
	/*
    set_override_cursor("WaitCursor")
    extents = gscene.items_bounding_rect()
    if extents:
        extents.set_width(grid_size_x)
        extents.set_height(grid_size_y)
        extents.move_center(Vector(0, 0))

    fit_in_view(extents, "KeepAspectRatio")
    restore_override_cursor()
    */
    return sc->NIL;
}

pointer
scm_zoom_real_time(scheme *sc, pointer args)
{
    debug_message("zoomRealtime()");
    debug_message("Implement zoomRealtime.");
    return sc->NIL;
}

pointer
scm_zoom_previous(scheme *sc, pointer args)
{
    debug_message("zoomPrevious()");
    debug_message("Implement zoomPrevious.");
    return sc->NIL;
}

pointer
scm_zoom_window(scheme *sc, pointer args)
{
    debug_message("zoom_window()");
    /*
    gview = active_view();
    if gview:
        gview->zoom_window(); */

	/*
    zoom_window_active = 1;
    selecting_active = 0;
    clear_selection();
    */
    return sc->NIL;
}

pointer
scm_zoom_dynamic(scheme *sc, pointer args)
{
    debug_message("zoom_dynamic()");
    debug_message("Implement zoom_dynamic.");
    return sc->NIL;
}

