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
void
allow_zoom_in(void)
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
}

/* .
 */
void
allow_zoom_out(void)
{
	/*
    origin = map_to_scene(0, 0);
    corner = map_to_scene(width(), height())
    max_size = corner.subtract(origin);

    if max(max_size.x, max_size.y) > zoom_out_limit:
        debug_message("zoom_out limit reached. (limit=%.1f)" % zoom_out_limit);
        return 0;

    return 1; */
}

/* .
 */
void
zoom_in(void)
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
}

/* .
 */
void
zoom_out(void)
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
}

/* . */
void
zoom_selected(void)
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
}

/* .
 */
void
zoom_scale(void)
{
    debug_message("zoom_scale()");
    debug_message("Implement zoom_scale.");
}

/* .
 */
void
zoom_center(void)
{
    debug_message("zoom_center()");
    debug_message("Implement zoom_center.");
}

/* .
 */
void
zoom_all(void)
{
    debug_message("zoom_all()");
    debug_message("Implement zoom_all.");
}

/* .
 */
void
zoom_extents(void)
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
}

void
zoom_real_time(void)
{
    debug_message("zoomRealtime()");
    debug_message("Implement zoomRealtime.");
}

void
zoom_previous(void)
{
    debug_message("zoomPrevious()");
    debug_message("Implement zoomPrevious.");
}

void
zoom_window(void)
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
}

void
zoom_dynamic(void)
{
    debug_message("zoom_dynamic()");
    debug_message("Implement zoom_dynamic.");
}


void
zoom_to_point(EmbVector mouse_point, float zoom_dir)
{
    double s;
    /*
    point-before-scale(map_to_scene(mouse-point)) */

    /* Do The zoom */
    s = 1.0;
    /*
    if (zoom-dir > 0) {
        if (!allow-zoom-in()) {
            return;
        }
        s = display-zoom-scale-action-in;
    }
    else {
        if (!allow-zoom-out()) {
            return;
        }
        s = display-zoom-scale-action-out;
    }

    scale(s, s);
    align-scene-point-with-view-point(point-before-scale, mouse-point);
    recalculate-limits();
    align-scene-point-with-view-point(point-before-scale, mouse-point);

    update-mouse-coords(mouse-point.x, mouse-point.y);
    if pasting-active:
        v = scene-mouse-point.subtract(paste-delta);
        paste-object-item-group.set_pos(v);

    if (selecting-active) {
        rect = make_rectangle(map-from-scene(scenePressPoint), mousePoint).normalized();
        select-box.set_geometry(rect);
    }
    */

    scene_update();
}

