/*
 * Embroidermodder 2.
 *
 * Copyright 2011-2024 The Embroidermodder Team
 * Embroidermodder 2 is Open Source Software, see LICENSE.md for licensing terms.
 *
 * Read the reference manual (https://www.libembroidery.org/downloads/emrm.pdf)
 * for advice on altering this file.
 *
 * Rubber Point and Grip Point Management
 */

#include "core.h"

/* . */
void
obj_grip_edit(int32_t obj_id, EmbVector before, EmbVector after)
{
    EmbVector delta = emb_vector_subtract(after, before);
    ObjectCore *core = obj_get_core(obj_id);
    switch (core->geometry->type) {
    case EMB_ARC: {
        debug_message("TODO: gripEdit() for ArcObject.");
        break;
    }
    case EMB_CIRCLE: {
        if (emb_approx(before, obj_center(core))) {
            obj_move_by(obj_id, delta);
        }
        else {
            EmbReal length = emb_vector_distance(obj_center(core), after);
            emb_set_radius(core->geometry, length);
        }
        break;
    }
    case EMB_DIM_LEADER:
    case EMB_LINE: {
        if (emb_approx(before, obj_end_point_1(core))) {
            obj_set_end_point_1(core, after);
        }
        else if (emb_approx(before, obj_end_point_2(core))) {
            obj_set_end_point_2(core, after);
        }
        else if (emb_approx(before, obj_mid_point(core))) {
            obj_move_by(obj_id, delta);
        }
        break;
    }
    case EMB_ELLIPSE: {
        debug_message("TODO: gripEdit() for EllipseObject");
        break;
    }
    case EMB_IMAGE:
    case EMB_RECT: {
        EmbReal height = emb_height(core->geometry);
        EmbReal width = emb_width(core->geometry);
        EmbVector tl = obj_top_left(core);
        int obj_id = core->objID;
        if (emb_approx(before, tl)) {
            obj_set_rect(obj_id, after.x, after.y,
                width - delta.x, height - delta.y);
        }
        else if (emb_approx(before, obj_top_right(core))) {
            obj_set_rect(obj_id, tl.x, tl.y+delta.y,
                width + delta.x, height - delta.y);
        }
        else if (emb_approx(before, obj_bottom_left(core))) {
            obj_set_rect(obj_id, tl.x+delta.x, tl.y,
                width - delta.x, height + delta.y);
        }
        else if (emb_approx(before, obj_bottom_right(core))) {
            obj_set_rect(obj_id, tl.x, tl.y,
                width + delta.x, height + delta.y);
        }
        break;
    }
    case EMB_PATH: {
        debug_message("TODO: gripEdit() for PathObject");
        break;
    }
    case EMB_POLYGON:
    case EMB_POLYLINE: {
        core->gripIndex = obj_find_index(obj_id, before);
        if (core->gripIndex == -1) {
            return;
        }
        /* FIXME:
        EmbVector a = map_from_scene(obj, after);
        obj->normalPath.setElementPositionAt(core->gripIndex, a.x, a.y);
        obj_update_path_r(obj, obj->normalPath);
        */
        core->gripIndex = -1;
        break;
    }
    case EMB_TEXT_SINGLE:
    case EMB_POINT:
    default: {
        if (emb_approx(before, obj_pos(core))) {
            obj_move_by(obj_id, delta);
        }
        break;
    }
    }
}

/* The caller is responsible for freeing this memory, currently. */
EmbVectorList *
all_grip_points(int32_t obj_id)
{
    ObjectCore *core = obj_get_core(obj_id);
    EmbVectorList *gripPoints = create_vector_list();
    switch (core->geometry->type) {
    case EMB_ARC: {
        append_vector_to_list(gripPoints, obj_center(core));
        append_vector_to_list(gripPoints, obj_start_point(core));
        append_vector_to_list(gripPoints, obj_mid_point(core));
        append_vector_to_list(gripPoints, obj_end_point(core));
        break;
    }
    case EMB_CIRCLE:
    case EMB_ELLIPSE: {
        append_vector_to_list(gripPoints, obj_center(core));
        append_vector_to_list(gripPoints, emb_quadrant(core->geometry, 0));
        append_vector_to_list(gripPoints, emb_quadrant(core->geometry, 90));
        append_vector_to_list(gripPoints, emb_quadrant(core->geometry, 180));
        append_vector_to_list(gripPoints, emb_quadrant(core->geometry, 270));
        break;
    }
    case EMB_DIM_LEADER: {
        append_vector_to_list(gripPoints, obj_end_point_1(core));
        append_vector_to_list(gripPoints, obj_end_point_2(core));
        if (core->curved) {
            append_vector_to_list(gripPoints, obj_mid_point(core));
        }
        break;
    }
    case EMB_IMAGE: {
        append_vector_to_list(gripPoints, obj_top_left(core));
        append_vector_to_list(gripPoints, obj_top_right(core));
        append_vector_to_list(gripPoints, obj_bottom_left(core));
        append_vector_to_list(gripPoints, obj_bottom_right(core));
        break;
    }
    case EMB_LINE: {
        append_vector_to_list(gripPoints, obj_end_point_1(core));
        append_vector_to_list(gripPoints, obj_end_point_2(core));
        append_vector_to_list(gripPoints, obj_mid_point(core));
        break;
    }
    case EMB_PATH: {
        append_vector_to_list(gripPoints, obj_pos(core));
        debug_message("TODO: loop thru all path Elements and return their points.");
        break;
    }
    case EMB_POLYGON:
    case EMB_POLYLINE: {
        /* FIXME:
        QPainterPath::Element element;
        for (int i = 0; i < normalPath.elementCount(); ++i) {
            element = normalPath.elementAt(i);
            append_vector_to_list(gripPoints, to_emb_vector(mapToScene(element.x, element.y)));
        }
        */
        break;
    }
    case EMB_TEXT_SINGLE:
    case EMB_POINT:
    default:
        append_vector_to_list(gripPoints, obj_pos(core));
        break;
    }
    return gripPoints;
}

/* Returns the closest snap point to the mouse point */
EmbVector
mouse_snap_point(int32_t obj_id, EmbVector mousePoint)
{
    return find_mouse_snap_point(all_grip_points(obj_id), mousePoint);
}

