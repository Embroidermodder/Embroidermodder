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

#include "core.h"

/* (double x1, double y1, double x2, double y2, double x3, double y3, int rubberMode).
 */
ScriptValue
add_arc_command(ScriptEnv *context)
{
    int32_t doc_index = activeDocument();
    if (doc_index < 0) {
        return script_false;
    }
    EmbArc arc;
    arc.start = unpack_vector(context, 0);
    arc.mid = unpack_vector(context, 2);
    arc.end = unpack_vector(context, 4);
    int rubberMode = INT(5);
    uint32_t obj = create_arc(arc, getCurrentColor());
    obj_set_rubber_mode(obj, rubberMode);
    doc_undoable_add_obj(doc_index, obj, rubberMode);
    return script_true;
}

/* (double centerX, double centerY, double radius, bool fill, int rubberMode). */
ScriptValue
add_circle_command(ScriptEnv *context)
{
    int32_t doc_index = activeDocument();
    if (doc_index < 0) {
        return script_false;
    }
    EmbCircle circle;
    circle.center = unpack_vector(context, 0);
    circle.radius = REAL(2);
    int rubberMode = INT(3);
    uint32_t obj = create_circle(circle, getCurrentColor());
    obj_set_rubber_mode(obj, rubberMode);
    /* TODO: circle fill. */
    doc_undoable_add_obj(doc_index, obj, rubberMode);
    return script_true;
}

/* double x1, double y1, double x2, double y2, double rot, int rubberMode */
ScriptValue
add_dimleader_command(ScriptEnv *context)
{
    int32_t doc_index = activeDocument();
    if (doc_index < 0) {
        return script_false;
    }
    EmbLine line;
    line.start = unpack_vector(context, 0);
    line.end = unpack_vector(context, 2);
    double rot = REAL(4);
    int rubberMode = INT(5);
    uint32_t obj = create_dimleader(line, getCurrentColor());
    obj_set_rotation(obj, -rot);
    obj_set_rubber_mode(obj, rubberMode);
    doc_undoable_add_obj(doc_index, obj, rubberMode);
    return script_true;
}

/* (double centerX, double centerY, double width, double height, double rot, bool fill, int rubberMode). */
ScriptValue
add_ellipse_command(ScriptEnv *context)
{
    int32_t doc_index = activeDocument();
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
    uint32_t obj = create_ellipse(ellipse, getCurrentColor());
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
    int32_t doc_index = activeDocument();
    if (doc_index < 0) {
        return script_false;
    }
    EmbLine line;
    line.start = unpack_vector(context, 0);
    line.end = unpack_vector(context, 2);
    double rot = REAL(4);
    int rubberMode = INT(5);
    uint32_t obj = create_line(line, getCurrentColor());
    obj_set_rotation(obj, -rot);
    obj_set_rubber_mode(obj, rubberMode);
    doc_undoable_add_obj(doc_index, obj, rubberMode);
    return script_true;
}

/* double x, double y. */
ScriptValue
add_point_command(ScriptEnv *context)
{
    int32_t doc_index = activeDocument();
    if (doc_index < 0) {
        return script_false;
    }
    EmbPoint point;
    point.position = unpack_vector(context, 0);
    int rubberMode = INT(2);
    uint32_t obj = create_point(point, getCurrentColor());
    doc_undoable_add_obj(doc_index, obj, rubberMode);
    return script_true;
}

/* (double x, double y, double w, double h, double rot, bool fill, int rubberMode). */
ScriptValue
add_rectangle_command(ScriptEnv *context)
{
    int32_t doc_index = activeDocument();
    if (doc_index < 0) {
        return script_false;
    }
    EmbRect rect = emb_rect(REAL(0), -REAL(1), REAL(2), -REAL(3));
    double rot = REAL(4);
    bool fill = BOOL(5);
    int rubberMode = INT(6);
    uint32_t obj = create_rect(rect, getCurrentColor());
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
    int32_t doc_index = activeDocument();
    if (doc_index < 0) {
        return script_false;
    }
    /*
    uint32_t obj = new Object(centerX, -centerY, diameter, length, getCurrentColor());
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
    int32_t doc_index = activeDocument();
    if (doc_index < 0) {
        return script_false;
    }
    char *str = STR(0);
    EmbVector v = unpack_vector(context, 1);
    double rot = REAL(3);
    bool fill = BOOL(4);
    int rubberMode = INT(5);
    uint32_t obj = create_text_single(str, v, getCurrentColor());
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

