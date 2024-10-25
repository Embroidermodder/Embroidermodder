/*
 * Embroidermodder 2.
 *
 * Copyright 2011-2024 The Embroidermodder Team
 * Embroidermodder 2 is Open Source Software, see LICENSE.md for licensing terms.
 *
 * Read the reference manual (https://www.libembroidery.org/downloads/emrm.pdf)
 * for advice on altering this file.
 *
 * Geometry Commands for user interface (internal functions only are in
 * "objects.c")
 */

#include <stdio.h>

#include "core.h"

/* (double x1, double y1, double x2, double y2, double x3, double y3, int rubberMode).
 */
ScriptValue
add_arc_command(ScriptEnv *context)
{
    int32_t doc_index = active_document();
    if (doc_index < 0) {
        return script_false;
    }
    EmbArc arc;
    arc.start = unpack_vector(context, 0);
    arc.mid = unpack_vector(context, 2);
    arc.end = unpack_vector(context, 4);
    int rubberMode = INT(5);
    uint32_t obj = create_arc(arc, get_current_color());
    obj_set_rubber_mode(obj, rubberMode);
    doc_undoable_add_obj(doc_index, obj, rubberMode);
    return script_true;
}

/* (double centerX, double centerY, double radius, bool fill, int rubberMode). */
ScriptValue
add_circle_command(ScriptEnv *context)
{
    int32_t doc_index = active_document();
    if (doc_index < 0) {
        return script_false;
    }
    EmbCircle circle;
    circle.center = unpack_vector(context, 0);
    circle.radius = REAL(2);
    int rubberMode = INT(3);
    uint32_t obj = create_circle(circle, get_current_color());
    obj_set_rubber_mode(obj, rubberMode);
    /* TODO: circle fill. */
    doc_undoable_add_obj(doc_index, obj, rubberMode);
    return script_true;
}

/* double x1, double y1, double x2, double y2, double rot, int rubberMode */
ScriptValue
add_dimleader_command(ScriptEnv *context)
{
    int32_t doc_index = active_document();
    if (doc_index < 0) {
        return script_false;
    }
    EmbLine line;
    line.start = unpack_vector(context, 0);
    line.end = unpack_vector(context, 2);
    double rot = REAL(4);
    int rubberMode = INT(5);
    uint32_t obj = create_dimleader(line, get_current_color());
    obj_set_rotation(obj, -rot);
    obj_set_rubber_mode(obj, rubberMode);
    doc_undoable_add_obj(doc_index, obj, rubberMode);
    return script_true;
}

/* (double centerX, double centerY, double width, double height, double rot, bool fill, int rubberMode). */
ScriptValue
add_ellipse_command(ScriptEnv *context)
{
    int32_t doc_index = active_document();
    if (doc_index < 0) {
        return script_false;
    }
    double width = REAL(2);
    double height = REAL(3);
    double rot = REAL(4);
    bool fill = BOOL(5);
    int rubberMode = INT(6);
    EmbEllipse ellipse;
    ellipse.center = unpack_vector(context, 0);
    ellipse.radius.x = width/2.0;
    ellipse.radius.y = height/2.0;
    uint32_t obj = create_ellipse(ellipse, get_current_color());
    obj_set_rotation(obj, -rot);
    obj_set_rubber_mode(obj, rubberMode);
    /* TODO: ellipse fill */
    doc_undoable_add_obj(doc_index, obj, rubberMode);
    return script_true;
}

/* (double x1, double y1, double x2, double y2, double rot, int rubberMode). */
ScriptValue
add_line_command(ScriptEnv *context)
{
    int32_t doc_index = active_document();
    if (doc_index < 0) {
        return script_false;
    }
    EmbLine line;
    line.start = unpack_vector(context, 0);
    line.end = unpack_vector(context, 2);
    double rot = REAL(4);
    int rubberMode = INT(5);
    uint32_t obj = create_line(line, get_current_color());
    obj_set_rotation(obj, -rot);
    obj_set_rubber_mode(obj, rubberMode);
    doc_undoable_add_obj(doc_index, obj, rubberMode);
    return script_true;
}

/* Argument format: (double x1, double y1, double x2, double y2, double rot).
 */
ScriptValue
add_infinite_line_command(ScriptEnv *context)
{
    EmbVector start = unpack_vector(context, 0);
    EmbVector end = unpack_vector(context, 2);
    double rot = REAL(4);
    printf("add_infinite_line_command (%f, %f) (%f, %f) %f\n",
        start.x, start.y, end.x, end.y, rot);
    return script_false;
}

/* Argument format: (double x1, double y1, double x2, double y2, double rot).
 */
ScriptValue
add_ray_command(ScriptEnv *context)
{
    EmbVector start = unpack_vector(context, 0);
    EmbVector end = unpack_vector(context, 2);
    double rot = REAL(4);
    printf("add_ray_command (%f, %f) (%f, %f) %f\n",
        start.x, start.y, end.x, end.y, rot);
    return script_false;
}

/* Argument format: (double x1, double y1, double x2, double y2, double x3,
 * double y3, double rot, bool fill).
 */
ScriptValue
add_triangle_command(ScriptEnv *context)
{
    EmbVector point1 = unpack_vector(context, 0);
    EmbVector point2 = unpack_vector(context, 2);
    EmbVector point3 = unpack_vector(context, 4);
    double rot = REAL(6);
    bool fill = BOOL(7);
    printf("add_triangle_command (%f, %f) (%f, %f) (%f, %f) %f %d\n",
        point1.x, point1.y,
        point2.x, point2.y,
        point3.x, point3.y,
        rot,
        fill);
    return script_false;
}

/* Argument format: (double x, double y, double w, double h, double rad,
 * double rot, bool fill).
 */
ScriptValue
add_rounded_rectangle_command(ScriptEnv *context)
{
    double x = REAL(0);
    double y = REAL(1);
    double w = REAL(2);
    double h = REAL(3);
    double rad = REAL(4);
    double rot = REAL(5);
    bool fill = BOOL(6);
    printf("add_triangle_command %f, %f, %f, %f, %f, %f, %d\n",
        x, y, w, h,
        rad, rot,
        fill);
    return script_false;
}

/* Argument format: (double centerX, double centerY, uint16_t sides,
 * uint8_t mode, double rad, double rot, bool fill).
 */
ScriptValue
add_regular_polygon_command(ScriptEnv *context)
{
    EmbVector point1 = unpack_vector(context, 0);
    EmbVector point2 = unpack_vector(context, 2);
    EmbVector point3 = unpack_vector(context, 4);
    double rot = REAL(6);
    bool fill = BOOL(7);
    printf("add_triangle_command (%f, %f) (%f, %f) (%f, %f) %f %d",
        point1.x, point1.y,
        point2.x, point2.y,
        point3.x, point3.y,
        rot,
        fill);
    return script_false;
}

/* NOTE: This native is different than the rest in that the Y+ is down
 * (scripters need not worry about this). */
/*
ScriptValue
add_Path(double startX, double startY, const QPainterPath& p, int rubberMode)
{
    EmbVector point1 = unpack_vector(context, 0);
    EmbVector point2 = unpack_vector(context, 2);
    EmbVector point3 = unpack_vector(context, 4);
    double rot = REAL(6);
    bool fill = BOOL(7);
    printf("add_triangle_command (%f, %f) (%f, %f) (%f, %f) %f %d",
        point1.x, point1.y,
        point2.x, point2.y,
        point3.x, point3.y,
        rot,
        fill);
    return script_false;
}
 */

/* Argument format: (double x1, double y1, double x2, double y2,
 * double legHeight).
 */
ScriptValue
add_horizontal_dimension_command(ScriptEnv *context)
{
    EmbVector start = unpack_vector(context, 0);
    EmbVector end = unpack_vector(context, 2);
    double legHeight = REAL(4);
    printf("add_horizontal_dimension_command (%f, %f) (%f, %f) %f",
        start.x, start.y,
        end.x, end.y,
        legHeight);
    return script_false;
}

/* Argument format: (double x1, double y1, double x2, double y2,
 * double legHeight)
 */
ScriptValue
add_vertical_dimension_command(ScriptEnv *context)
{
    EmbVector start = unpack_vector(context, 0);
    EmbVector end = unpack_vector(context, 2);
    double legHeight = REAL(4);
    printf("add_vertical_dimension_command (%f, %f) (%f, %f) %f",
        start.x, start.y,
        end.x, end.y,
        legHeight);
    return script_false;
}

/* (QString img, double x, double y, double w, double h, double rot)
ScriptValue
add_image_command(ScriptEnv *context)
{
    return script_false;
}

void
add_To_selection(const QPainterPath path, Qt::ItemSelectionMode mode)
{
}
*/

/* NOTE: This native is different than the rest in that the Y+ is down
 * (scripters need not worry about this)
 */
ScriptValue
add_polygon_command(ScriptEnv *context)
{
    int32_t doc_index = active_document();
    if (doc_index < 0) {
        return script_false;
    }
    EmbPolygon polygon;
    EmbVector start = unpack_vector(context, 0);
    int rubberMode = INT(2);
    /* FIXME: pass path */
    uint32_t obj = create_polygon(NULL, get_current_color());
    obj_set_rubber_mode(obj, rubberMode);
    doc_undoable_add_obj(doc_index, obj, rubberMode);
 
    #if 0
    if (context->argumentCount != 1) {
        prompt_output("addPolygon() requires one argument");
        return script_false;
    }
    if (!context->argument[0].isArray()) {
        prompt_output("TYPE_ERROR, addPolygon(): first argument is not an array");
        return script_false;
    }

    QVariantList varList = context->argument[0].toVariant().toList();
    int varSize = varList.size();
    if (varSize < 2) {
        prompt_output("TYPE_ERROR, addPolygon(): array must contain at least two elements");
        return script_false;
    }
    if (varSize % 2) {
        prompt_output("TYPE_ERROR, addPolygon(): array cannot contain an odd number of elements");
        return script_false;
    }

    bool lineTo = false;
    bool xCoord = true;
    EmbVector v = zero_vector;
    EmbVector start = zero_vector;
    QPainterPath path;
    foreach (QVariant var, varList) {
        if (var.canConvert(QVariant::Double)) {
            if (xCoord) {
                xCoord = false;
                v.x = var.toReal();
            }
            else {
                xCoord = true;
                v.y = -var.toReal();

                if (lineTo) {
                    path.lineTo(v.x, v.y);
                }
                else {
                    path.moveTo(v.x, v.y);
                    lineTo = true;
                    start = v;
                }
            }
        }
        else {
            prompt_output("TYPE_ERROR, addPolygon(): array contains one or more invalid elements");
            return script_false;
        }
    }

    /* Close the polygon. */
    path.closeSubpath();

    path.translate(-start.x, -start.y);

    add_Polygon(start.x, start.y, path, RUBBER_OFF);
    #endif

    return script_true;
}

/* NOTE: This native is different than the rest in that the Y+ is down
 * (scripters need not worry about this)
 */
ScriptValue
add_polyline_command(ScriptEnv *context)
{
    int32_t doc_index = active_document();
    if (doc_index < 0) {
        return script_false;
    }
    EmbPath path;
    EmbVector start = unpack_vector(context, 0);
    int rubberMode = INT(2);
    /* FIXME: pass path */
    uint32_t obj = create_polygon(NULL, get_current_color());
    obj_set_rubber_mode(obj, rubberMode);
    doc_undoable_add_obj(doc_index, obj, rubberMode);
    
    #if 0
    if (context->argumentCount != 1) {
        prompt_output("addPolyline() requires one argument");
        return script_false;
    }
    if (!context->argument[0].isArray()) {
        prompt_output("TYPE_ERROR, addPolyline(): first argument is not an array");
        return script_false;
    }

    QVariantList varList = context->argument[0].toVariant().toList();
    int varSize = varList.size();
    if (varSize < 2) {
        prompt_output("TYPE_ERROR, addPolyline(): array must contain at least two elements");
        return script_false;
    }
    if (varSize % 2) {
        prompt_output("TYPE_ERROR, addPolyline(): array cannot contain an odd number of elements");
        return script_false;
    }

    bool lineTo = false;
    bool xCoord = true;
    double x = 0;
    double y = 0;
    double startX = 0;
    double startY = 0;
    QPainterPath path;
    foreach(QVariant var, varList) {
        if (var.canConvert(QVariant::Double)) {
            if (xCoord) {
                xCoord = false;
                x = var.toReal();
            }
            else {
                xCoord = true;
                y = -var.toReal();

                if (lineTo) {
                    path.lineTo(x,y);
                }
                else {
                    path.moveTo(x,y);
                    lineTo = true;
                    startX = x;
                    startY = y;
                }
            }
        }
        else {
            prompt_output("TYPE_ERROR, addPolyline(): array contains one or more invalid elements");
            return script_false;
        }
    }

    path.translate(-startX, -startY);

    add_Polyline(startX, startY, path, RUBBER_OFF);
    #endif
    return script_true;
}

/* double x, double y. */
ScriptValue
add_point_command(ScriptEnv *context)
{
    int32_t doc_index = active_document();
    if (doc_index < 0) {
        return script_false;
    }
    EmbPoint point;
    point.position = unpack_vector(context, 0);
    int rubberMode = INT(2);
    uint32_t obj = create_point(point, get_current_color());
    doc_undoable_add_obj(doc_index, obj, rubberMode);
    return script_true;
}

/* (double x, double y, double w, double h, double rot, bool fill, int rubberMode). */
ScriptValue
add_rectangle_command(ScriptEnv *context)
{
    int32_t doc_index = active_document();
    if (doc_index < 0) {
        return script_false;
    }
    EmbRect rect = emb_rect(REAL(0), -REAL(1), REAL(2), -REAL(3));
    double rot = REAL(4);
    bool fill = BOOL(5);
    int rubberMode = INT(6);
    uint32_t obj = create_rect(rect, get_current_color());
    obj_set_rotation(obj, -rot);
    obj_set_rubber_mode(obj, rubberMode);
    /* TODO: rect fill */
    doc_undoable_add_obj(doc_index, obj, rubberMode);
    return script_false;
}

/* (double centerX, double centerY, double diameter, double length, double rot, bool fill, int rubberMode). */
ScriptValue
add_slot_command(ScriptEnv *context)
{
    /* TODO: Use UndoableCommand for slots */
    int32_t doc_index = active_document();
    if (doc_index < 0) {
        return script_false;
    }
    /*
    uint32_t obj = create_object(EMB_SLOT, centerX, -centerY, diameter, length, get_current_color());
    obj_set_rotation(obj, -rot);
    obj_set_rubber_mode(obj, rubberMode);
    if (rubberMode) {
        doc_add_to_rubber_room(doc_index, slotObj);
    }
    scene->addItem(slotObj);
    */
    /* TODO: slot fill */
    doc_update(doc_index);
    return script_false;
}

/* (char *str, double x, double y, double rot, bool fill, int rubberMode). */
ScriptValue
add_textsingle_command(ScriptEnv *context)
{
    int32_t doc_index = active_document();
    if (doc_index < 0) {
        return script_false;
    }
    char *str = STR(0);
    EmbVector v = unpack_vector(context, 1);
    double rot = REAL(3);
    bool fill = BOOL(4);
    int rubberMode = INT(5);
    uint32_t obj = create_text_single(str, v, get_current_color());
    ObjectCore *core = obj_get_core(obj);
    obj_set_text_font(core, get_str(TEXT_FONT));
    obj_set_text_size(core, get_real(TEXT_SIZE));
    obj_set_text_style(core,
        get_bool(TEXT_STYLE_BOLD),
        get_bool(TEXT_STYLE_ITALIC),
        get_bool(TEXT_STYLE_UNDERLINE),
        get_bool(TEXT_STYLE_STRIKEOUT),
        get_bool(TEXT_STYLE_OVERLINE));
    obj_set_text_backward(core, false);
    obj_set_text_upside_down(core, false);
    obj_set_rotation(obj, -rot);
    /* TODO: single line text fill. */
    obj_set_rubber_mode(obj, rubberMode);
    doc_undoable_add_obj(doc_index, obj, rubberMode);
    return script_true;
}

/* add_TextMulti(char *str, double x, double y, double rot, bool fill, int rubberMode).
 */
ScriptValue
add_textmulti_command(ScriptEnv *context)
{
    return script_false;
}

