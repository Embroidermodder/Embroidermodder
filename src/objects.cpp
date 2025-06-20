/*!
 * \file objects.c
 *
 * Embroidermodder 2.
 *
 * Copyright 2011-2025 The Embroidermodder Team
 * Embroidermodder 2 is Open Source Software, see LICENSE.md for licensing terms.
 *
 * Read the reference manual (https://www.libembroidery.org/downloads/emrm.pdf)
 * for advice on altering this file.
 *
 * ObjectCore: the additional information and tools on top of EmbGeometry
 * required for the user interface.
 *
 * Geometry Commands for user interface (internal functions only are in
 * "objects.c")
 *
 * Rubber Point and Grip Point Management
 */

#include <stdio.h>
#include <string.h>

#include "core.h"

extern const char *justify_options[];

/* . */
uint32_t
copy_object(uint32_t obj)
{
    debug_message("Object Copy()");
    ObjectCore *core = obj_get_core(obj);
    uint32_t copy = 0xFFFFFFFF; /* error symbol */
    debug_message("TODO: get_current_line_type");
    switch (core->geometry->type) {
    case EMB_ARC: {
        copy = create_arc(core->geometry->object.arc, core->rgb);
        break;
    }
    case EMB_CIRCLE: {
        copy = create_circle(core->geometry->object.circle, core->rgb);
        break;
    }
    case EMB_DIM_LEADER: {
        // FIXME: copy = create_dimleader(core->geometry->object.dimleader, core->rgb);
        break;
    }
    case EMB_ELLIPSE: {
        copy = create_ellipse(core->geometry->object.ellipse, core->rgb);
        break;
    }
    case EMB_IMAGE: {
        // FIXME: copy = create_image(core->geometry->object.image, core->rgb);
        break;
    }
    case EMB_LINE: {
        copy = create_line(core->geometry->object.line, core->rgb);
        break;
    }
    case EMB_PATH: {
        copy = create_path(&(core->geometry->object.path), core->rgb);
        break;
    }
    case EMB_POINT: {
        copy = create_point(core->geometry->object.point, core->rgb);
        break;
    }
    case EMB_POLYGON: {
        copy = create_polygon(&(core->geometry->object.polygon), core->rgb);
        break;
    }
    case EMB_POLYLINE: {
        copy = create_polyline(&(core->geometry->object.polyline), core->rgb);
        break;
    }
    case EMB_RECT: {
        copy = create_rect(core->geometry->object.rect, core->rgb);
        break;
    }
    case EMB_TEXT_SINGLE: {
        copy = create_text_single(core->text, core->position, core->rgb);
        ObjectCore *copy_core = obj_get_core(copy);
        obj_set_text_font(copy_core, core->textFont);
        obj_set_text_size(copy_core, core->textSize);
        /* TODO: fix this */
        obj_set_rotation(0, core->rotation);
        obj_set_text_backward(copy_core, core->textBackward);
        obj_set_text_upside_down(copy_core, core->textUpsideDown);
        obj_set_text_style(copy_core, core->textBold, core->textItalic,
            core->textUnderline, core->textStrikeOut, core->textOverline);
//        object_list[copy]->setScale(core->scale);
        break;
    }
    default:
        /* TODO: error report */
        break;
    }
    return copy;
}

/* . */
void
free_object(ObjectCore *core)
{
    debug_message("Object Destructor()");
    free(core->geometry);
    free(core);
}

/* . */
uint32_t
create_arc(EmbArc arc, uint32_t rgb)
{
    debug_message("ArcObject Constructor()");
    uint32_t obj = create_object(EMB_ARC, rgb);
    ObjectCore *core = obj_get_core(obj);
    core->geometry->object.arc = arc;
    debug_message("TODO: get_current_line_type");
    obj_calculate_data(obj);
    obj_set_pos(core, arc.start);
    return obj;
}

/* . */
uint32_t
create_circle(EmbCircle circle, uint32_t rgb)
{
    debug_message("CircleObject Constructor()");
    uint32_t obj = create_object(EMB_CIRCLE, rgb);
    debug_message("TODO: get_current_line_type");
    ObjectCore *core = obj_get_core(obj);
    core->geometry->object.circle = circle;
    /* update_path(); */
    return obj;
}

/* . */
uint32_t
create_dimleader(EmbLine line, uint32_t rgb)
{
    debug_message("DimLeaderObject Constructor()");
    debug_message("TODO: get_current_line_type");
    uint32_t obj = create_object(EMB_DIM_LEADER, rgb);
    ObjectCore *core = obj_get_core(obj);

    core->curved = false;
    core->filled = true;
    emb_gset(core->geometry, EMB_START, script_vector(line.start));
    emb_gset(core->geometry, EMB_END, script_vector(line.end));
    return obj;
}

/* . */
uint32_t
create_ellipse(EmbEllipse ellipse, uint32_t rgb)
{
    debug_message("EllipseObject Constructor()");
    debug_message("TODO: get_current_line_type");
    uint32_t obj = create_object(EMB_ELLIPSE, rgb);
    ObjectCore *core = obj_get_core(obj);
    core->geometry->object.ellipse = ellipse;

    /*
    setObjectSize(width, height);
    obj_update_path(obj);
    */
    return obj;
}

/* . */
uint32_t
create_image(EmbRect rect, uint32_t rgb)
{
    debug_message("ImageObject Constructor()");
    debug_message("TODO: get_current_line_type");
    uint32_t obj = create_object(EMB_IMAGE, rgb);
    obj_set_rect(obj, rect.x, rect.y, rect.w, rect.h);
    return obj;
}

/* . */
uint32_t
create_line(EmbLine line, uint32_t rgb)
{
    debug_message("LineObject Constructor()");
    uint32_t obj = create_object(EMB_LINE, rgb);
    ObjectCore *core = obj_get_core(obj);
    debug_message("TODO: get_current_line_type");
    emb_gset(core->geometry, EMB_START, script_vector(line.start));
    emb_gset(core->geometry, EMB_END, script_vector(line.end));
    return obj;
}

/* . */
uint32_t
create_path(EmbPath *p, uint32_t rgb)
{
    debug_message("PathObject Constructor()");
    uint32_t obj = create_object(EMB_PATH, rgb);
    debug_message("TODO: get_current_line_type");
    // FIXME: obj_update_path_r(obj, p);
    //obj_set_pos(obj->core, v);
    return obj;
}

/* . */
uint32_t
create_polygon(EmbPath *p, uint32_t rgb)
{
    debug_message("PolygonObject Constructor()");
    uint32_t obj = create_object(EMB_POLYGON, rgb);
    debug_message("TODO: get_current_line_type");
    // FIXME: obj_update_path_r(obj, p);
    //obj_set_pos(obj->core, v);
    return obj;
}

/* . */
uint32_t
create_polyline(EmbPath *path, uint32_t rgb)
{
    debug_message("PolylineObject Constructor()");
    debug_message("TODO: get_current_line_type");
    uint32_t obj = create_object(EMB_POLYLINE, rgb);
    // FIXME: obj_update_path_r(obj, p);
    /* EmbVector v; obj_set_pos(obj->core, v); */
    return obj;
}

/* . */
uint32_t
create_point(EmbPoint point, uint32_t rgb)
{
    uint32_t obj = create_object(EMB_POINT, rgb);
    return obj;
}

/* . */
uint32_t
create_rect(EmbRect rect, uint32_t rgb)
{
    debug_message("RectObject Constructor()");
    debug_message("TODO: get_current_line_type");
    uint32_t obj = create_object(EMB_RECT, rgb);
    obj_set_rect(obj, rect.x, rect.y, rect.w, rect.h);
    return obj;
}

/* . */
uint32_t
create_text_single(EmbString str, EmbVector v, uint32_t rgb)
{
    debug_message("TextSingleObject Constructor()");
    debug_message("TODO: get_current_line_type");
    uint32_t obj = create_object(EMB_TEXT_SINGLE, rgb);
    ObjectCore *core = obj_get_core(obj);

    strcpy(core->textJustify, "Left");
    /* TODO: set the justification properly */

    obj_set_text(core, str);
    obj_set_pos(core, v);
    return obj;
}

/* . */
EmbVector
find_mouse_snap_point(std::vector<EmbVector> snap_points, EmbVector mouse_point)
{
    float closest = 1.0e10;
    EmbVector result = snap_points[0];
    int i;
    for (i=0; i<snap_points.size(); i++) {
        float distance = emb_vector_distance(snap_points[i], mouse_point);
        if (distance < closest) {
            closest = distance;
            result = snap_points[i];
        }
    }
    return result;
}

/* . */
EmbVector
obj_delta(ObjectCore *obj)
{
    return emb_vector_subtract(emb_gget(obj->geometry, EMB_END).v,
        emb_gget(obj->geometry, EMB_START).v);
}

/* . */
void
obj_set_pos(ObjectCore *obj, EmbVector point)
{
    obj->position = point;
}

/* . */
void
obj_set_rubber_mode(uint32_t id, int mode)
{
    ObjectCore *core = obj_get_core(id);
    core->rubber_mode = mode;
}

/* . */
void
obj_set_x(ObjectCore *obj, EmbReal x)
{
    obj->position.x = x;
}

/* . */
void
obj_set_y(ObjectCore *obj, EmbReal y)
{
    obj->position.y = y;
}

/* FIXME. */
void
obj_set_center(ObjectCore *obj, EmbVector center)
{
    obj_set_pos(obj, center);
}

/* . */
void
obj_set_center_x(ObjectCore *obj, EmbReal centerX)
{
    EmbVector center = emb_gget(obj->geometry, EMB_CENTER).v;
    center.x = centerX;
    obj_set_center(obj, center);
}

/* . */
void
obj_set_center_y(ObjectCore *obj, EmbReal centerY)
{
    EmbVector center = emb_gget(obj->geometry, EMB_CENTER).v;
    center.y = centerY;
    obj_set_center(obj, center);
}

/* . */
double
obj_length(ObjectCore *obj)
{
    /* FIXME: return obj_line(obj).length() * obj->scale(); */
    return 1.0;
}

/* . */
void
obj_set_x1(ObjectCore *obj, EmbReal x)
{
    EmbVector v = emb_gget(obj->geometry, EMB_START).v;
    v.x = x;
    emb_gset(obj->geometry, EMB_START, script_vector(v));
}

/* . */
void
obj_set_y1(ObjectCore *obj, EmbReal y)
{
    EmbVector v = emb_gget(obj->geometry, EMB_START).v;
    v.y = y;
    emb_gset(obj->geometry, EMB_START, script_vector(v));
}

/* . */
void
obj_set_x2(ObjectCore *obj, EmbReal x)
{
    EmbVector v = emb_gget(obj->geometry, EMB_END).v;
    v.x = x;
    emb_gset(obj->geometry, EMB_END, script_vector(v));
}

/* . */
void
obj_set_y2(ObjectCore *obj, EmbReal y)
{
    EmbVector v = emb_gget(obj->geometry, EMB_END).v;
    v.y = y;
    emb_gset(obj->geometry, EMB_END, script_vector(v));
}

/* . */
ScriptValue
arc_command(ScriptEnv *context)
{

    return script_null;
}

/* . */
ScriptValue
circle_command(ScriptEnv *context)
{
    debug_message("ADDING CIRCLE");

    switch (context->mode) {
    case CONTEXT_CONTEXT:
        debug_message("TODO: CIRCLE context()");
        break;
    default:
        break;
    }

    /*
    context->point1 = emb_vector(0.0, 0.0);
    context->point2 = context->point1;
    context->point3 = context->point1;
    add_real_variable(context, "rad", 0.0f);
    add_real_variable(context, "dia", 0.0f);
    add_real_variable(context, "cx", 0.0f);
    add_real_variable(context, "cy", 0.0f);
    context->mode = MODE_1P_RAD;

    _main->nativeAppendPromptHistory(translate("Specify center point for circle or [3P/2P/Ttr (tan tan radius)]: "));
    */
    EmbCircle c;
    c.center.x = 0.0;
    c.center.y = 0.0;
    c.radius = 10.0;
    /*
    MdiWindow *window = activeMdiWindow();
    if (window) {
        emb_array_add_circle(window->pattern->geometry, c);
    }
    _main->nativeAddCircle(0.0, 0.0, 10.0, true, RUBBER_CIRCLE_1P_DIA); */
   
    end_command();
    return script_null;
}

ScriptValue
circle_click(ScriptEnv *context)
{
    switch (context->mode) {
    case CIRCLE_1P_RAD_ONE: {
    #if 0
        context->point1 = emb_vector(0.0, 0.0);
        context->center = emb_vector(0.0, 0.0);
        obj_add_rubber(obj, "CIRCLE");
        obj_set_rubber_mode(obj, "CIRCLE_1P_RAD");
        obj_set_rubber_point(obj, "CIRCLE_CENTER", v);
        prompt_output(translate("Specify radius of circle or [Diameter]: "));
        break;
    }
    case CIRCLE_1P_RAD_TWO: {
        context->point2 = v;
        obj_set_rubber_point(obj, "CIRCLE_RADIUS", context->point2);
        vulcanize();
        end_command();
        break;
    }
    case CIRCLE_1P_DIA_ONE: {
        error("CIRCLE", translate("This should never happen."));
        break;
    }
    case CIRCLE_1P_DIA_TWO: {
        context->point2 = v;
        obj_set_rubber_point(obj, "CIRCLE_DIAMETER", context->point2);
        vulcanize();
        end_command();
        break;
    }
    case MODE_2P: {
        if (isnan(context->x1)) {
            context->point1 = v;
            obj_add_rubber(obj, "CIRCLE");
            obj_set_rubber_mode(obj, "CIRCLE_2P");
            obj_set_rubber_point(obj, "CIRCLE_TAN1", v);
            prompt_output(translate("Specify second end point of circle's diameter: "));
        }
        else if (isnan(context->x2)) {
            context->point2 = v;
            obj_set_rubber_point(obj, "CIRCLE_TAN2", v);
            vulcanize();
            end_command();
        }
        else {
            error("CIRCLE", translate("This should never happen."));
        }
        break;
    }
    case CIRCLE_3P: {
        if (isnan(context->x1)) {
            context->point1 = v;
            prompt_output(translate("Specify second point on circle: "));
        }
        else if (isnan(context->x2)) {
            context->point2 = v;
            obj_add_rubber(obj, "CIRCLE");
            obj_set_rubber_mode(obj, "CIRCLE_3P");
            obj_set_rubber_point(obj, "CIRCLE_TAN1", context->point1);
            obj_set_rubber_point(obj, "CIRCLE_TAN2", context->point2);
            prompt_output(translate("Specify third point on circle: "));
        }
        else if (isnan(context->x3)) {
            context->point3 = v;
            obj_set_rubber_point(obj, "CIRCLE_TAN3", context->point3);
            vulcanize();
            end_command();
        }
        else {
            error("CIRCLE", translate("This should never happen."));
        }
        break;
    }
    case MODE_TTR: {
        if (isnan(context->x1)) {
            context->point1 = v;
            prompt_output(translate("Specify point on object for second tangent of circle: "));
        }
        else if (isnan(context->x2)) {
            context->point2 = v;
            prompt_output(translate("Specify radius of circle: "));
        }
        else if (isnan(context->x3)) {
            context->point3 = v;
            prompt_output(translate("Specify second point: "));
        }
        else {
            debug_message("TODO: CIRCLE click() for TTR");
        }
    #endif
        break;
    }
    default:
        break;
    }
    end_command();
    return script_null;
}

ScriptValue
circle_prompt(ScriptEnv *context)
{
    switch (context->mode) {
    case CIRCLE_1P_RAD_ONE: {
    #if 0
        if (isnan(context->x1)) {
            if (str == "2P") {
                context->mode = context->mode_2P;
                prompt_output(translate("Specify first end point of circle's diameter: "));
            }
            else if (str == "3P") {
                context->mode = context->mode_3P;
                prompt_output(translate("Specify first point of circle: "));
            }
            else if (str == "T" || str == "TTR") {
                context->mode = context->mode_TTR;
                prompt_output(translate("Specify point on object for first tangent of circle: "));
            }
            else {
                if (!parse_vector(str, &v)) {
                    alert(translate("Point or option keyword required."));
                    prompt_output(translate("Specify center point for circle or [3P/2P/Ttr (tan tan radius)]: "));
                }
                else {
                    context->point1 = v;
                    context->center = context->point1;
                    obj_add_rubber(obj, "CIRCLE");
                    obj_set_rubber_mode(obj, "CIRCLE_1P_RAD");
                    obj_set_rubber_point(obj, "CIRCLE_CENTER", context->center);
                    prompt_output(translate("Specify radius of circle or [Diameter]: "));
                }
            }
        }
        else {
            if (str == "D" || str == "DIAMETER") {
                context->mode = context->mode_1P_DIA;
                obj_set_rubber_mode(obj, "CIRCLE_1P_DIA");
                prompt_output(translate("Specify diameter of circle: "));
            }
            else {
                var num = Number(str);
                if (isnan(num)) {
                    alert(translate("Requires numeric radius, point on circumference, or \"D\"."));
                    prompt_output(translate("Specify radius of circle or [Diameter]: "));
                }
                else {
                    context->rad = num;
                    context->point2 = context->point1;
                    context->x2 += context->rad;
                    obj_set_rubber_point(obj, "CIRCLE_RADIUS", context->point2);
                    vulcanize();
                    endCommand();
                }
            }
        }
    }
    case CIRCLE_1P_DIA_ONE: {
        if (isnan(context->x1)) {
            error("CIRCLE", translate("This should never happen."));
        }
        if (isnan(context->x2)) {
            var num = Number(str);
            if (isnan(num)) {
                alert(translate("Requires numeric distance or second point."));
                prompt_output(translate("Specify diameter of circle: "));
            }
            else {
                context->dia = num;
                context->point2 = context->point1;
                context->x2 += context->dia;
                obj_set_rubber_point(obj, "CIRCLE_DIAMETER", context->point2);
                vulcanize();
                endCommand();
            }
        }
        else {
            error("CIRCLE", translate("This should never happen."));
        }
        break;
    }
    case CIRCLE_2P: {
        if (isnan(context->x1)) {
            if (!parse_vector(str, &v)) {
                alert(translate("Invalid point."));
                prompt_output(translate("Specify first end point of circle's diameter: "));
            }
            else {
                context->point1 = v;
                obj_add_rubber(obj, "CIRCLE");
                obj_set_rubber_mode(obj, "CIRCLE_2P");
                obj_set_rubber_point(obj, "CIRCLE_TAN1", context->point1);
                prompt_output(translate("Specify second end point of circle's diameter: "));
            }
        }
        else if (isnan(context->x2)) {
            if (!parse_vector(str, &v)) {
                alert(translate("Invalid point."));
                prompt_output(translate("Specify second end point of circle's diameter: "));
            }
            else {
                context->point2 = v;
                obj_set_rubber_point(obj, "CIRCLE_TAN2", context->point2);
                vulcanize();
                end_command();
            }
        }
        else {
            error("CIRCLE", translate("This should never happen."));
        }
    }
    case MODE_3P: {
        if (isnan(context->x1)) {
            if (!parse_vector(str, &v)) {
                alert(translate("Invalid point."));
                prompt_output(translate("Specify first point of circle: "));
            }
            else {
                context->point1 = v;
                prompt_output(translate("Specify second point of circle: "));
            }
        }
        else if (isnan(context->x2)) {
            if (!parse_vector(str, &v)) {
                alert(translate("Invalid point."));
                prompt_output(translate("Specify second point of circle: "));
            }
            else {
                context->point2 = v;
                obj_add_rubber(obj, "CIRCLE");
                obj_set_rubber_mode(obj, "CIRCLE_3P");
                obj_set_rubber_point(obj, "CIRCLE_TAN1", context->point1);
                obj_set_rubber_point(obj, "CIRCLE_TAN2", context->point2);
                prompt_output(translate("Specify third point of circle: "));
            }
        }
        else if (isnan(context->x3)) {
            if (!parse_vector(str, &v)) {
                alert(translate("Invalid point."));
                prompt_output(translate("Specify third point of circle: "));
            }
            else {
                context->point3 = v;
                obj_set_rubber_point(obj, "CIRCLE_TAN3", context->point3);
                vulcanize();
                endCommand();
            }
        }
        else {
            error("CIRCLE", translate("This should never happen."));
        }
        
    }
    else if (context->mode == context->mode_TTR) {
        debug_message("TODO: CIRCLE prompt() for TTR");
    }
    #endif
        break;
    }
    default:
        break;
    }

    end_command();
    return script_null;
}


/* Math Diagram
 *                 .(ap1)                     .(lp1)
 *                /|                         /|
 *               / |                        / |
 *              /  |                       /  |
 *             /   |                      /   |
 *            /    |                     /    |
 *           /     |                    /     |
 *          /      |                   /      |
 *         /       |                  /       |
 *        /+(aSA)  |                 /+(lSA)  |
 * (ap0)./__(aSL)__|__________(lp0)./__(lSL)__|
 *       \ -(aSA)  |                \ -(lSA)  |
 *        \        |                 \        |
 *         \       |                  \       |
 *          \      |                   \      |
 *           \     |                    \     |
 *            \    |                     \    |
 *             \   |                      \   |
 *              \  |                       \  |
 *               \ |                        \ |
 *                \|                         \|
 *                 .(ap2)                     .(lp2)
 */
#if 0
void
DimLeaderobj_updateLeader()
{
    debug_message("TODO: Make arrowStyle, arrowStyleAngle, arrowStyleLength, lineStyleAngle, lineStyleLength customizable");
    int arrowStyle = Closed;
    EmbReal arrowStyleAngle = 15.0;
    EmbReal arrowStyleLength = 1.0;
    EmbReal lineStyleAngle = 45.0;
    EmbReal lineStyleLength = 1.0;

    QLineF lyne = obj_line(obj);
    EmbReal angle = lyne.angle();
    EmbVector ap0 = lyne.p1();
    EmbVector lp0 = lyne.p2();

    /* Arrow */
    QLineF lynePerp(lyne.pointAt(arrowStyleLength/lyne.length()) ,lp0);
    lynePerp.setAngle(angle + 90);
    QLineF lyne1(ap0, lp0);
    QLineF lyne2(ap0, lp0);
    lyne1.setAngle(angle + arrowStyleAngle);
    lyne2.setAngle(angle - arrowStyleAngle);
    EmbVector ap1;
    EmbVector ap2;
    lynePerp.intersects(lyne1, &ap1);
    lynePerp.intersects(lyne2, &ap2);

    if (arrowStyle == Open) {
        arrowStylePath = QPainterPath();
        arrowStylePath.moveTo(ap1);
        arrowStylePath.lineTo(ap0);
        arrowStylePath.lineTo(ap2);
        arrowStylePath.lineTo(ap0);
        arrowStylePath.lineTo(ap1);
    }
    else if (arrowStyle == Closed) {
        arrowStylePath = QPainterPath();
        arrowStylePath.moveTo(ap1);
        arrowStylePath.lineTo(ap0);
        arrowStylePath.lineTo(ap2);
        arrowStylePath.lineTo(ap1);
    }
    else if (arrowStyle == Dot) {
        arrowStylePath = QPainterPath();
        arrowStylePath.addEllipse(ap0, arrowStyleLength, arrowStyleLength);
    }
    else if (arrowStyle == Box) {
        arrowStylePath = QPainterPath();
        EmbReal side = QLineF(ap1, ap2).length();
        QRectF ar0(0, 0, side, side);
        ar0.moveCenter(ap0);
        arrowStylePath.addRect(ar0);
    }
    else if (arrowStyle == Tick) {
    }

    lineStylePath = QPainterPath();
    lineStylePath.moveTo(ap0);
    lineStylePath.lineTo(lp0);
}
#endif

/* ELLIPSE */
ScriptValue
ellipse_command(ScriptEnv *context)
{
    switch (context->mode) {
    default:
    case CONTEXT_MAIN:
        /*
        context->mode = ELLIPSE_MAJDIA_MINRAD_ONE;
        context->point1 = emb_vector(0.0, 0.0);
        context->point2 = emb_vector(0.0, 0.0);
        context->point3 = emb_vector(0.0, 0.0);
        prompt_output(translate("Specify first axis start point or [Center]: "));
        */
        end_command();
        break;
    case CONTEXT_PROMPT:
        end_command();
        break;
    case CONTEXT_CONTEXT:
        end_command();
        break;
    case CONTEXT_CLICK:
        end_command();
        break;
    }
    return script_null;
}

#if 0
context->ellipse;
context->rot;

function click(EmbVector v)
{
    switch (context->mode) {
    case ELLIPSE_MAJDIA_MINRAD: {
        if (isnan(context->x1)) {
            context->point1 = v;
            obj_add_rubber(obj, "ELLIPSE");
            obj_set_rubber_mode(obj, "ELLIPSE_LINE");
            obj_set_rubber_point(obj, "ELLIPSE_LINE_POINT1", context->point1);
            prompt_output(translate("Specify first axis end point: "));
        }
        else if (isnan(context->x2)) {
            context->point2 = v;
            context->center = emb_vector_average(context->point1, context->point2);
            context->width = calculateDistance(context->point1, context->point2);
            context->rot = calculateAngle(context->point1, context->point2);
            obj_set_rubber_mode(obj, "ELLIPSE_MAJDIA_MINRAD");
            obj_set_rubber_point(obj, "ELLIPSE_AXIS1_POINT1", context->point1);
            obj_set_rubber_point(obj, "ELLIPSE_AXIS1_POINT2", context->point2);
            obj_set_rubber_point(obj, "ELLIPSE_CENTER", context->center);
            obj_set_rubber_point(obj, "ELLIPSE_WIDTH", context->width, 0);
            obj_set_rubber_point(obj, "ELLIPSE_ROT", context->rot, 0);
            prompt_output(translate("Specify second axis end point or [Rotation]: "));
        }
        else if (isnan(context->x3)) {
            context->point3 = v;
            context->height = perpendicularDistance(context->point3, context->point1, context->point2)*2.0;
            obj_set_rubber_point(obj, "ELLIPSE_AXIS2_POINT2", context->point3);
            vulcanize();
            end_command();
        }
        else {
            error("ELLIPSE", translate("This should never happen."));
        }
        break;
    }
    case ELLIPSE_MAJRAD_MINRAD: {
        if (isnan(context->x1)) {
            context->point1 = v;
            context->center = context->point1;
            obj_add_rubber(obj, "ELLIPSE");
            obj_set_rubber_mode(obj, "ELLIPSE_LINE");
            obj_set_rubber_point(obj, "ELLIPSE_LINE_POINT1", context->point1);
            obj_set_rubber_point(obj, "ELLIPSE_CENTER", context->center);
            prompt_output(translate("Specify first axis end point: "));
        }
        else if (isnan(context->x2)) {
            context->point2 = v;
            context->width = calculateDistance(context->center, context->point2)*2.0;
            context->rot = calculateAngle(context->point1, context->point2);
            obj_set_rubber_mode(obj, "ELLIPSE_MAJRAD_MINRAD");
            obj_set_rubber_point(obj, "ELLIPSE_AXIS1_POINT2", context->point2);
            obj_set_rubber_point(obj, "ELLIPSE_WIDTH", context->width, 0);
            obj_set_rubber_point(obj, "ELLIPSE_ROT", context->rot, 0);
            prompt_output(translate("Specify second axis end point or [Rotation]: "));
        }
        else if (isnan(context->x3)) {
            context->point3 = v;
            context->height = perpendicularDistance(context->point3, context->center, context->point2)*2.0;
            obj_set_rubber_point(obj, "ELLIPSE_AXIS2_POINT2", context->point3);
            vulcanize();
            end_command();
        }
        else {
            error("ELLIPSE", translate("This should never happen."));
        }
        break;
    }
    case ELLIPSE_ROTATION: {
        if (isnan(context->x1)) {
            error("ELLIPSE", translate("This should never happen."));
        }
        else if (isnan(context->x2)) {
            error("ELLIPSE", translate("This should never happen."));
        }
        else if (isnan(context->x3)) {
            var angle = calculateAngle(context->center, x, y);
            context->height = Math.cos(angle*Math.PI/180.0)*context->width;
            addEllipse(context->center, context->width, context->height, context->rot, false);
            end_command();
        }
        break;
    }
    default:
        break;
    }
}

function context(str)
{
    debug_message("TODO: ELLIPSE context()");
}

function prompt(str)
{
    EmbVector v;
    switch (context->mode) {
    case ELLIPSE_MAJDIA_MINRAD_ONE: {
        if (str == "C" || str == "CENTER") {
            context->mode = ELLIPSE_MAJRAD_MINRAD;
            prompt_output(translate("Specify center point: "));
            break;
        }

        if (parse_vector(str, &v)) {
            alert(translate("Point or option keyword required."));
            prompt_output(translate("Specify first axis start point or [Center]: "));
        }
        else {
            context->point1 = v;
            obj_add_rubber(obj, "ELLIPSE");
            obj_set_rubber_mode(obj, "ELLIPSE_LINE");
            obj_set_rubber_point(obj, "ELLIPSE_LINE_POINT1", context->point1);
            prompt_output(translate("Specify first axis end point: "));
            mode = ELLIPSE_MAJDIA_MINRAD_TWO;
        }
        break;
    }
    case ELLIPSE_MAJDIA_MINRAD_TWO: {
        if (parse_vector(str, &v)) {
            alert(translate("Invalid point."));
            prompt_output(translate("Specify first axis end point: "));
        }
        else {
            context->point2 = v;
            context->center = emb_vector_average(context->point1, context->point2);
            context->width = calculateDistance(context->point1, context->point2);
            context->rot = calculateAngle(context->point1, context->point2);
            obj_set_rubber_mode(obj, "ELLIPSE_MAJDIA_MINRAD");
            obj_set_rubber_point(obj, "ELLIPSE_AXIS1_POINT1", context->point1);
            obj_set_rubber_point(obj, "ELLIPSE_AXIS1_POINT2", context->point2);
            obj_set_rubber_point(obj, "ELLIPSE_CENTER", context->center);
            obj_set_rubber_point(obj, "ELLIPSE_WIDTH", context->width, 0);
            obj_set_rubber_point(obj, "ELLIPSE_ROT", context->rot, 0);
            prompt_output(translate("Specify second axis end point or [Rotation]: "));
            mode = ELLIPSE_MAJDIA_MINRAD_THREE;
        }
        break;
    }
    case ELLIPSE_MAJDIA_MINRAD_THREE: {
        if (str == "R" || str == "ROTATION") {
            context->mode = ELLIPSE_ROTATION;
            prompt_output(translate("Specify rotation: "));
            break;
        }

        if (parse_vector(str, &v)) {
            alert(translate("Point or option keyword required."));
            prompt_output(translate("Specify second axis end point or [Rotation]: "));
        }
        else {
            context->point3 = v;
            context->height = perpendicularDistance(context->point3, context->point1, context->point2)*2.0;
            obj_set_rubber_point(obj, "ELLIPSE_AXIS2_POINT2", context->point3);
            vulcanize();
            end_command();
            context->mode = ELLIPSE_NEUTRAL;
        }
        break;
    }
    case ELLIPSE_MAJRAD_MINRAD_ONE: {
        if (parse_vector(str, &v)) {
            alert(translate("Invalid point."));
            prompt_output(translate("Specify center point: "));
        }
        else {
            context->point1 = v;
            context->center = context->point1;
            obj_add_rubber(obj, "ELLIPSE");
            obj_set_rubber_mode(obj, "ELLIPSE_LINE");
            obj_set_rubber_point(obj, "ELLIPSE_LINE_POINT1", context->point1);
            obj_set_rubber_point(obj, "ELLIPSE_CENTER", context->center);
            prompt_output(translate("Specify first axis end point: "));
        }
        break;
    }
    case ELLIPSE_MAJRAD_MINRAD_TWO: {
        if (parse_vector(str, &v)) {
            alert(translate("Invalid point."));
            prompt_output(translate("Specify first axis end point: "));
        }
        else {
            context->point2 = v;
            context->width = calculateDistance(context->point1, context->point2)*2.0;
            context->rot = calculateAngle(context->point1, context->point2);
            obj_set_rubber_mode(obj, "ELLIPSE_MAJRAD_MINRAD");
            obj_set_rubber_point(obj, "ELLIPSE_AXIS1_POINT2", context->point2);
            obj_set_rubber_point(obj, "ELLIPSE_WIDTH", context->width, 0);
            obj_set_rubber_point(obj, "ELLIPSE_ROT", context->rot, 0);
            prompt_output(translate("Specify second axis end point or [Rotation]: "));
        }
        break;
    }
    case ELLIPSE_MAJRAD_MINRAD_THREE: {
        if (str == "R" || str == "ROTATION") {
            context->mode = ELLIPSE_ROTATION;
            prompt_output(translate("Specify ellipse rotation: "));
        }
        else {
            if (parse_vector(str, &v)) {
                alert(translate("Point or option keyword required."));
                prompt_output(translate("Specify second axis end point or [Rotation]: "));
            }
            else {
                context->point3 = v;
                context->height = perpendicularDistance(context->point3, context->point1, context->point2)*2.0;
                obj_set_rubber_point(obj, "ELLIPSE_AXIS2_POINT2", context->point3);
                vulcanize();
                end_command();
            }
        }
        break;
    }
    case ELLIPSE_ROTATION: {
        if (isnan(str)) {
            alert(translate("Invalid angle. Input a numeric angle or pick a point."));
            prompt_output(translate("Specify rotation: "));
        }
        else {
            var angle = Number(str);
            context->height = cos(angle * embConstantPi / 180.0) * context->width;
            addEllipse(context->center, context->width, context->height, context->rot, false);
            end_command();
        }
        break;
    }
    default:
        break;
    }
}
#endif

/* LINE */
ScriptValue
line_command(ScriptEnv *context)
{
    switch (context->context) {
    case CONTEXT_MAIN: {
        /*
        context->firstRun = true;
        context->first = emb_vector(0.0, 0.0);
        context->prev = emb_vector(0.0, 0.0);
        */
        prompt_output(translate("Specify first point: "));
        break;
    }
    case CONTEXT_CLICK: {
    /*
    if (context->firstRun) {
        context->firstRun = false;
        context->first = v;
        context->prev = v;
        obj_add_rubber(obj, "LINE");
        obj_set_rubber_mode(obj, "LINE");
        obj_set_rubber_point(obj, "LINE_START", context->first);
        prompt_output(translate("Specify next point or [Undo]: "));
    }
    else {
        obj_set_rubber_point(obj, "LINE_END", v);
        vulcanize();
        obj_add_rubber(obj, "LINE");
        obj_set_rubber_mode(obj, "LINE");
        obj_set_rubber_point(obj, "LINE_START", v);
        context->prev = v;
    }
    */
        break;
    }
    case CONTEXT_CONTEXT: {
        debug_message("TODO: LINE context()");
        break;
    }
    default:
        break;
    }
    return script_null;
}

#if 0
function prompt(str)
{
    EmbVector v;
    if (context->firstRun) {
        if (!parse_vector(str, &v)) {
            alert(translate("Invalid point."));
            prompt_output(translate("Specify first point: "));
        }
        else {
            context->firstRun = false;
            context->first = v;
            context->prev = context->first;
            obj_add_rubber(obj, "LINE");
            obj_set_rubber_mode(obj, "LINE");
            obj_set_rubber_point(obj, "LINE_START", context->first.x, context->first.y);
            prompt_output(translate("Specify next point or [Undo]: "));
        }
    }
    else {
        if (str == "U" || str == "UNDO") {
            debug_message("TODO: LINE prompt() for UNDO");
        }
        else {
            if (!parse_vector(str, &v)) {
                alert(translate("Point or option keyword required."));
                prompt_output(translate("Specify next point or [Undo]: "));
            }
            else {
                obj_set_rubber_point(obj, "LINE_END", v);
                obj_vulcanize(obj);
                obj_add_rubber(obj, "LINE");
                obj_set_rubber_mode(obj, "LINE");
                obj_set_rubber_point(obj, "LINE_START", v);
                context->prev = v;
                prompt_output(translate("Specify next point or [Undo]: "));
            }
        }
    }
}

#endif

/* PATH
 */
ScriptValue
path_command(ScriptEnv *context)
{
    debug_message("TODO: Path command is currently broken.");
    switch (context->mode) {
    case CONTEXT_MAIN:
        /*
        init_command();
        clear_selection();
        context->firstRun = true;
        context->first = emb_vector(0.0, 0.0);
        context->prev = emb_vector(0.0, 0.0);
        prompt_output(translate("Specify start point: "));
        */
        break;
    case CONTEXT_CLICK:
        /*
        if (context->firstRun) {
            context->firstRun = false;
            context->first = v;
            context->prev = v;
            addPath(v);
            prompt_output(translate("Specify next point or [Arc/Undo]: "));
        }
        else {
            appendLineToPath(x,y);
            context->prev = v;
        }
        */
        break;
    case CONTEXT_CONTEXT:
        debug_message("TODO: PATH context()");
        break;
    case CONTEXT_PROMPT:
        /*
        if (str == "A" || str == "ARC") {
            debug_message("TODO: PATH prompt() for ARC");
        }
        else if (str == "U" || str == "UNDO") {
            debug_message("TODO: PATH prompt() for UNDO");
        }
        else {
            if (!parse_vector(str, &v)) {
                alert(translate("Point or option keyword required."));
                prompt_output(translate("Specify next point or [Arc/Undo]: "));
            }
            else {
                if (context->firstRun) {
                    context->firstRun = false;
                    context->first = v;
                    context->prev = v;
                    addPath(v);
                    prompt_output(translate("Specify next point or [Arc/Undo]: "));
                }
                else {
                    appendLineToPath(v);
                    context->prev = v;
                }
            }
        }
        */
        break;
    default:
        break;
    }
    return script_null;
}

/* POINT */
ScriptValue
point_command(ScriptEnv *context)
{
    return script_null;
}

#if 0
function main()
{
    global->firstRun = true;
    prompt_output("TODO: Current point settings: PDMODE=?  PDSIZE=?");
    debug_message("TODO: translate needed here when complete");
    prompt_output(translate("Specify first point: "));
}

void
click(EmbVector v)
{
    if (context->firstRun) {
        context->firstRun = false;
        prompt_output(translate("Specify next point: "));
        addPoint(x,y);
    }
    else {
        addPoint(x,y);
    }
}

function context(str)
{
    debug_message("TODO: POINT context()");
}

function prompt(str)
{
    EmbVector v;
    if (global->firstRun) {
        if (str == "M" || str == "MODE") {
            debug_message("TODO: POINT prompt() for PDMODE");
        }
        else if (str == "S" || str == "SIZE") {
            debug_message("TODO: POINT prompt() for PDSIZE");
        }
        if (!parse_vector(str, v)) {
            alert(translate("Invalid point."));
            prompt_output(translate("Specify first point: "));
        }
        else {
            context->firstRun = false;
            prompt_output(translate("Specify next point: "));
            addPoint(v);
        }
    }
    else {
        if (!parse_vector(str, v)) {
            alert(translate("Invalid point."));
            prompt_output(translate("Specify next point: "));
        }
        else {
            prompt_output(translate("Specify next point: "));
            addPoint(v);
        }
    }
}

/* . */
Object::Object(double x, EmbReal y, QRgb rgb, QGraphicsItem* parent) : BaseObject(parent)
{
    debug_message("PointObject Constructor()");
    init(x, y, rgb, Qt::SolidLine);
    debug_message("TODO: get_current_line_type");
    init_geometry(EMB_POINT, rgb, lineType);
    obj_set_rect(obj, -0.00000001, -0.00000001, 0.00000002, 0.00000002);
    obj_set_pos(obj, x,y);
}

#endif

/* POLYGON */
ScriptValue
polygon_command(ScriptEnv *context)
{

    end_command();
    return script_null;
}

#if 0
function
main()
{
    context->center = emb_vector(0.0, 0.0);
    context->side1 = emb_vector(0.0, 0.0);
    context->side2 = emb_vector(0.0, 0.0);
    context->pointI = emb_vector(0.0, 0.0);
    context->pointC = emb_vector(0.0, 0.0);
    context->polyType = "Inscribed";
    context->numSides = 4;
    context->mode = POLYGON_MODE_NUM_SIDES;
    prompt_output(translate("Enter number of sides") + " {" + context->numSides.toString() + "}: ");
}

function
click(EmbVector v)
{
    switch (context->mode) {
    default:
    case POLYGON_MODE_NUM_SIDES:
    case POLYGON_MODE_POLYTYPE:
    case POLYGON_MODE_DISTANCE: {
        /* Do nothing, the prompt controls these. */
        break;
    }
    case POLYGON_MODE_CENTER_PT: {
        context->center = v;
        context->mode = POLYGON_MODE_POLYTYPE;
        prompt_output(translate("Specify polygon type [Inscribed in circle/Circumscribed around circle]") + " {" + context->polyType + "}: ");
        break;
    }
    case POLYGON_MODE_INSCRIBE: {
        context->pointI = v;
        obj_set_rubber_point(obj, "POLYGON_INSCRIBE_POINT", context->pointIX, context->pointIY);
        vulcanize();
        end_command();
        break;
    }
    case POLYGON_MODE_CIRCUMSCRIBE: {
        context->pointC = v;
        obj_set_rubber_point(obj, "POLYGON_CIRCUMSCRIBE_POINT", context->pointCX, context->pointCY);
        vulcanize();
        end_command();
        break;
    }
    case POLYGON_MODE_SIDE_LEN: {
        debug_message("TODO: POLYGON Sidelength mode");
        break;
    }
    }
}

function
context(char *str)
{
    debug_message("TODO: POLYGON context()");
}

function
prompt(char *str)
{
    EmbVector v;
    switch (global->mode) {
    case POLYGON_MODE_NUM_SIDES: {
        if (str == "" && context->numSides >= 3 && context->numSides <= 1024) {
            prompt_output(translate("Specify center point or [Sidelength]: "));
            context->mode = POLYGON_MODE_CENTER_PT;
        }
        else {
            var tmp = Number(str);
            if (isnan(tmp) || !isInt(tmp) || tmp < 3 || tmp > 1024) {
                alert(translate("Requires an integer between 3 and 1024."));
                prompt_output(translate("Enter number of sides") + " {" + context->numSides.toString() + "}: ");
            }
            else {
                context->numSides = tmp;
                prompt_output(translate("Specify center point or [Sidelength]: "));
                context->mode = POLYGON_MODE_CENTER_PT;
            }
        }
        break;
    }
    case POLYGON_MODE_CENTER_PT: {
        if (str == "S" || str == "SIDELENGTH") {
            context->mode = POLYGON_MODE_SIDE_LEN;
            prompt_output(translate("Specify start point: "));
        }
        else {
            if (!parse_vector(str, &v)) {
                alert(translate("Point or option keyword required."));
                prompt_output(translate("Specify center point or [Sidelength]: "));
            }
            else {
                context->center = v;
                context->mode = POLYGON_MODE_POLYTYPE;
                prompt_output(translate("Specify polygon type [Inscribed in circle/Circumscribed around circle]") + " {" + context->polyType + "}: ");
            }
        }
        break;
    }
    case POLYGON_MODE_POLYTYPE: {
        if (str == "I" || str == "INSCRIBED") {
            context->mode = POLYGON_MODE_INSCRIBE;
            context->polyType = "Inscribed";
            prompt_output(translate("Specify polygon corner point or [Distance]: "));
            obj_add_rubber(obj, "POLYGON");
            obj_set_rubber_mode(obj, "POLYGON_INSCRIBE");
            obj_set_rubber_point(obj, "POLYGON_CENTER", context->center);
            obj_set_rubber_point(obj, "POLYGON_NUM_SIDES", context->numSides, 0);
        }
        else if (str == "C" || str == "CIRCUMSCRIBED") {
            context->mode = POLYGON_MODE_CIRCUMSCRIBE;
            context->polyType = "Circumscribed";
            prompt_output(translate("Specify polygon side point or [Distance]: "));
            obj_add_rubber(obj, "POLYGON");
            obj_set_rubber_mode(obj, "POLYGON_CIRCUMSCRIBE");
            obj_set_rubber_point(obj, "POLYGON_CENTER", context->center);
            obj_set_rubber_point(obj, "POLYGON_NUM_SIDES", context->numSides, 0);
        }
        else if (str == "") {
            if (context->polyType == "Inscribed") {
                context->mode = POLYGON_MODE_INSCRIBE;
                prompt_output(translate("Specify polygon corner point or [Distance]: "));
                obj_add_rubber(obj, "POLYGON");
                obj_set_rubber_mode(obj, "POLYGON_INSCRIBE");
                obj_set_rubber_point(obj, "POLYGON_CENTER", context->center);
                obj_set_rubber_point(obj, "POLYGON_NUM_SIDES", context->numSides, 0);
            }
            else if (context->polyType == "Circumscribed") {
                context->mode = POLYGON_MODE_CIRCUMSCRIBE;
                prompt_output(translate("Specify polygon side point or [Distance]: "));
                obj_add_rubber(obj, "POLYGON");
                obj_set_rubber_mode(obj, "POLYGON_CIRCUMSCRIBE");
                obj_set_rubber_point(obj, "POLYGON_CENTER", context->center);
                obj_set_rubber_point(obj, "POLYGON_NUM_SIDES", context->numSides, 0);
            }
            else {
                error("POLYGON", translate("Polygon type is not Inscribed or Circumscribed."));
            }
        }
        else {
            alert(translate("Invalid option keyword."));
            prompt_output(translate("Specify polygon type [Inscribed in circle/Circumscribed around circle]") + " {" + context->polyType + "}: ");
        }
        break;
    }
    case POLYGON_MODE_INSCRIBE: {
        if (str == "D" || str == "DISTANCE") {
            context->mode = POLYGON_MODE_DISTANCE;
            prompt_output(translate("Specify distance: "));
        }
        else {
            if (!parse_vector(str, &v)) {
                alert(translate("Point or option keyword required."));
                prompt_output(translate("Specify polygon corner point or [Distance]: "));
            }
            else {
                context->pointI = v;
                obj_set_rubber_point(obj, "POLYGON_INSCRIBE_POINT", context->pointIX, context->pointIY);
                vulcanize();
                end_command();
            }
        }
        break;
    }
    case POLYGON_MODE_CIRCUMSCRIBE: {
        if (str == "D" || str == "DISTANCE") {
            context->mode = POLYGON_MODE_DISTANCE;
            prompt_output(translate("Specify distance: "));
        }
        else {
            if (!parse_vector(str, &v)) {
                alert(translate("Point or option keyword required."));
                prompt_output(translate("Specify polygon side point or [Distance]: "));
            }
            else {
                context->pointC = v;
                obj_set_rubber_point(obj, "POLYGON_CIRCUMSCRIBE_POINT", context->pointCX, context->pointCY);
                vulcanize();
                end_command();
            }
        }
        break;
    }
    case POLYGON_MODE_DISTANCE: {
        if (isnan(str)) {
            alert(translate("Requires valid numeric distance."));
            prompt_output(translate("Specify distance: "));
        }
        else {
            if (context->polyType == "Inscribed") {
                context->pointI.x = context->center.x;
                context->pointI.y = context->center.y + Number(str);
                obj_set_rubber_point(obj, "POLYGON_INSCRIBE_POINT", context->pointI);
                vulcanize();
                end_command();
            }
            else if (context->polyType == "Circumscribed") {
                context->pointC.x = context->center.x;
                context->pointC.y = context->center.y + Number(str);
                obj_set_rubber_point(obj, "POLYGON_CIRCUMSCRIBE_POINT", context->pointC);
                vulcanize();
                end_command();
            }
            else {
                error("POLYGON", translate("Polygon type is not Inscribed or Circumscribed."));
            }
        }
        break;
    }
    case POLYGON_MODE_SIDE_LEN: {
        debug_message("TODO: POLYGON Sidelength mode");
        break;
    }
    default:
        break;
    }
}
#endif


/* POLYLINE. */
ScriptValue
polyline_command(ScriptEnv *context)
{

    end_command();
    return script_null;
}

#if 0
function main()
{
    context->firstRun = true;
    context->first = emb_vector(0.0, 0.0);
    context->prev = emb_vector(0.0, 0.0);
    context->num = 0;
    prompt_output(translate("Specify first point: "));
}

function click(EmbVector v)
{
    if (context->firstRun) {
        context->firstRun = false;
        context->first = v;
        context->prev = v;
        obj_add_rubber(obj, "POLYLINE");
        obj_set_rubber_mode(obj, "POLYLINE");
        obj_set_rubber_point(obj, "POLYLINE_POINT_0", context->first);
        prompt_output(translate("Specify next point or [Undo]: "));
    }
    else {
        context->num++;
        obj_set_rubber_point(obj, "POLYLINE_POINT_" + context->num.toString(), v);
        obj_add_rubber_text(obj, "POLYLINE_NUM_POINTS", context->num.toString());
        obj_spare_rubber(obj, "POLYLINE");
        context->prev = v;
    }
}

function context(str)
{
    debug_message("TODO: POLYLINE context()");
}

function prompt(str)
{
    if (context->firstRun) {
        if (!parse_vector(str, v)) {
            alert(translate("Invalid point."));
            prompt_output(translate("Specify first point: "));
        }
        else {
            context->firstRun = false;
            context->first = v;
            context->prev = context->first;
            obj_add_rubber(obj, "POLYLINE");
            obj_set_rubber_mode(obj, "POLYLINE");
            obj_set_rubber_point(obj, "POLYLINE_POINT_0", context->first);
            prompt_output(translate("Specify next point or [Undo]: "));
        }
    }
    else {
        if (str == "U" || str == "UNDO") {
            debug_message("TODO: POLYLINE prompt() for UNDO");
        }
        else {
            if (!parse_vector(str, v)) {
                alert(translate("Point or option keyword required."));
                prompt_output(translate("Specify next point or [Undo]: "));
            }
            else {
                context->num++;
                obj_set_rubber_point(obj, "POLYLINE_POINT_" + context->num.toString(), v);
                obj_set_rubber_text(obj, "POLYLINE_NUM_POINTS", context->num.toString());
                obj_spare_rubber(obj, "POLYLINE");
                context->prev = v;
                prompt_output(translate("Specify next point or [Undo]: "));
            }
        }
    }
}
#endif

/* RECTANGLE */
ScriptValue
rectangle_command(ScriptEnv *context)
{

    end_command();
    return script_null;
}

#if 0
function main()
{
    context->newRect = true;
    context->point1 = emb_vector(0.0, 0.0);
    context->point2 = emb_vector(0.0, 0.0);
    prompt_output(translate("Specify first corner point or [Chamfer/Fillet]: "));
}

void
click(EmbVector v)
{
    if (context->newRect) {
        context->newRect = false;
        context->point1 = v;
        obj_add_rubber(obj, "RECTANGLE");
        obj_set_rubber_mode(obj, "RECTANGLE");
        obj_set_rubber_point(obj, "RECTANGLE_START", v);
        prompt_output(translate("Specify other corner point or [Dimensions]: "));
    }
    else {
        context->newRect = true;
        context->point2 = v;
        obj_set_rubber_point(obj, "RECTANGLE_END", v);
        vulcanize();
        end_command();
    }
}

function context(str)
{
    debug_message("TODO: RECTANGLE context()");
}

function prompt(str)
{
    if (str == "C" || str == "CHAMFER") {
        debug_message("TODO: RECTANGLE prompt() for CHAMFER");
    }
    else if (str == "D" || str == "DIMENSIONS") {
        debug_message("TODO: RECTANGLE prompt() for DIMENSIONS");
    }
    else if (str == "F" || str == "FILLET") {
        debug_message("TODO: RECTANGLE prompt() for FILLET");
    }
    else {
        if (!parse_vector(str, &v)) {
            alert(translate("Invalid point."));
            prompt_output(translate("Specify first point: "));
        }
        else {
            if (context->newRect) {
                context->newRect = false;
                context->point1 = v;
                obj_add_rubber(obj, "RECTANGLE");
                obj_set_rubber_mode(obj, "RECTANGLE");
                obj_set_rubber_point(obj, "RECTANGLE_START", v);
                prompt_output(translate("Specify other corner point or [Dimensions]: "));
            }
            else {
                context->newRect = true;
                context->point2 = v;
                obj_set_rubber_point(obj, "RECTANGLE_END", v);
                vulcanize();
                end_command();
            }
        }
    }
}
#endif

/*
void
SaveObject::addTextSingle(EmbPattern* pattern, QGraphicsItem* item)
{
    debug_message("TODO: saving polygons, polylines and paths must be stable before we go here.");

    debug_message("TODO: This needs to work like a path, not a polyline. Improve this");

    TextSingleObjectCore* obj = static_cast<TextSingleObjectCore*>(item);
    if (obj) {
        if (formatType == EMBFORMAT_STITCHONLY) {
            QList<QPainterPath> pathList = obj->objectSavePathList();
            foreach(QPainterPath path, pathList) {
                toPolyline(pattern, obj->objectPos(), path.simplified(), "0", obj_color(obj), "CONTINUOUS", "BYLAYER");
                debug_message("TODO: proper layer/lineType/lineWeight");
                debug_message("TODO: Improve precision, replace simplified");
            }
        }
    }
}
*/

/* SINGLELINETEXT */
ScriptValue
singlelinetext_command(ScriptEnv * /* context */)
{
    return script_null;
}

#if 0
function
main()
{
    context->text = "";
    context->position = emb_vector(0.0, 0.0);
    context->justify = "Left";
    context->font = textFont();
    context->height = 10.0;
    context->rotation = 0.0;
    context->mode = TEXTSINGLE_MODE_SETGEOM;
    prompt_output(translate("Current font: ") + "{" + context->textFont + "} " + translate("Text height: ") + "{" +  textSize() + "}");
    prompt_output(translate("Specify start point of text or [Justify/Setfont]: "));
}

function
click(EmbVector v)
{
    if (context->mode == TEXTSINGLE_MODE_SETGEOM) {
        if (isnan(context->textX)) {
            context->text_position = v;
            obj_add_rubber(obj, "LINE");
            obj_set_rubber_mode(obj, "LINE");
            obj_set_rubber_point(obj, "LINE_START", context->text_position);
            prompt_output(translate("Specify text height") + " {" + textSize() + "}: ");
        }
        else if (isnan(context->textHeight)) {
            context->textHeight = calculateDistance(context->textX, context->textY, x, y);
            obj_set_text_size(context->textHeight);
            prompt_output(translate("Specify text angle") + " {" + textAngle() + "}: ");
        }
        else if (isnan(context->textRotation)) {
            context->textRotation = calculateAngle(context->textX, context->textY, x, y);
            setSettingsTextAngle(context->textRotation);
            prompt_output(translate("Enter text: "));
            context->mode = TEXTSINGLE_MODE_RAPID;
            command_prompt("enable rapidfire");
            clearRubber();
            obj_add_rubber(obj, "TEXTSINGLE");
            obj_set_rubber_mode(obj, "TEXTSINGLE");
            obj_set_rubber_point(obj, "TEXT_POINT", context->textX, context->textY);
            obj_set_rubber_point(obj, "TEXT_HEIGHT_ROTATION", context->textHeight, context->textRotation);
            obj_set_rubber_text(obj, "TEXT_FONT", context->textFont);
            obj_set_rubber_text(obj, "TEXT_JUSTIFY", context->textJustify);
            obj_set_rubber_text(obj, "TEXT_RAPID", context->text);
        }
        else {
            /* Do nothing, as we are in rapidFire mode now. */
        }
    }
}

function context(str)
{
    debug_message("TODO: SINGLELINETEXT context()");
}

function prompt(str)
{
    if (context->mode == TEXTSINGLE_MODE_JUSTIFY) {
        if (str == "C" || str == "CENTER") {
            context->mode = TEXTSINGLE_MODE_SETGEOM;
            strcpy(context->textJustify, "Center");
            obj_add_rubber_text(obj, "TEXT_JUSTIFY", context->textJustify);
            prompt_output(translate("Specify center point of text or [Justify/Setfont]: "));
        }
        else if (str == "R" || str == "RIGHT") {
            context->mode = TEXTSINGLE_MODE_SETGEOM;
            context->textJustify = "Right";
            obj_add_rubber_text(obj, "TEXT_JUSTIFY", context->textJustify);
            prompt_output(translate("Specify right-end point of text or [Justify/Setfont]: "));
        }
        else if (str == "A" || str == "ALIGN") {
            context->mode = TEXTSINGLE_MODE_SETGEOM;
            context->textJustify = "Aligned";
            obj_add_rubber_text(obj, "TEXT_JUSTIFY", context->textJustify);
            prompt_output(translate("Specify start point of text or [Justify/Setfont]: "));
        }
        else if (str == "M" || str == "MIDDLE") {
            context->mode = TEXTSINGLE_MODE_SETGEOM;
            context->textJustify = "Middle";
            obj_add_rubber_text(obj, "TEXT_JUSTIFY", context->textJustify);
            prompt_output(translate("Specify middle point of text or [Justify/Setfont]: "));
        }
        else if (str == "F" || str == "FIT") {
            context->mode = TEXTSINGLE_MODE_SETGEOM;
            context->textJustify = "Fit";
            obj_add_rubber_text(obj, "TEXT_JUSTIFY", context->textJustify);
            prompt_output(translate("Specify start point of text or [Justify/Setfont]: "));
        }
        else if (str == "TL" || str == "TOPLEFT") {
            context->mode = TEXTSINGLE_MODE_SETGEOM;
            context->textJustify = "Top Left";
            obj_add_rubber_text(obj, "TEXT_JUSTIFY", context->textJustify);
            prompt_output(translate("Specify top-left point of text or [Justify/Setfont]: "));
        }
        else if (str == "TC" || str == "TOPCENTER") {
            context->mode = TEXTSINGLE_MODE_SETGEOM;
            context->textJustify = "Top Center";
            obj_add_rubber_text(obj, "TEXT_JUSTIFY", context->textJustify);
            prompt_output(translate("Specify top-center point of text or [Justify/Setfont]: "));
        }
        else if (str == "TR" || str == "TOPRIGHT") {
            context->mode = TEXTSINGLE_MODE_SETGEOM;
            context->textJustify = "Top Right";
            obj_add_rubber_text(obj, "TEXT_JUSTIFY", context->textJustify);
            prompt_output(translate("Specify top-right point of text or [Justify/Setfont]: "));
        }
        else if (str == "ML" || str == "MIDDLELEFT") {
            context->mode = TEXTSINGLE_MODE_SETGEOM;
            context->textJustify = "Middle Left";
            obj_add_rubber_text(obj, "TEXT_JUSTIFY", context->textJustify);
            prompt_output(translate("Specify middle-left point of text or [Justify/Setfont]: "));
        }
        else if (str == "MC" || str == "MIDDLECENTER") {
            context->mode = TEXTSINGLE_MODE_SETGEOM;
            context->textJustify = "Middle Center";
            obj_add_rubber_text(obj, "TEXT_JUSTIFY", context->textJustify);
            prompt_output(translate("Specify middle-center point of text or [Justify/Setfont]: "));
        }
        else if (str == "MR" || str == "MIDDLERIGHT") {
            context->mode = TEXTSINGLE_MODE_SETGEOM;
            context->textJustify = "Middle Right";
            obj_add_rubber_text(obj, "TEXT_JUSTIFY", context->textJustify);
            prompt_output(translate("Specify middle-right point of text or [Justify/Setfont]: "));
        }
        else if (str == "BL" || str == "BOTTOMLEFT") {
            context->mode = TEXTSINGLE_MODE_SETGEOM;
            context->textJustify = "Bottom Left";
            obj_add_rubber_text(obj, "TEXT_JUSTIFY", context->textJustify);
            prompt_output(translate("Specify bottom-left point of text or [Justify/Setfont]: "));
        }
        else if (str == "BC" || str == "BOTTOMCENTER") {
            context->mode = TEXTSINGLE_MODE_SETGEOM;
            context->textJustify = "Bottom Center";
            obj_add_rubber_text(obj, "TEXT_JUSTIFY", context->textJustify);
            prompt_output(translate("Specify bottom-center point of text or [Justify/Setfont]: "));
        }
        else if (str == "BR" || str == "BOTTOMRIGHT") {
            context->mode = TEXTSINGLE_MODE_SETGEOM;
            context->textJustify = "Bottom Right";
            obj_add_rubber_text(obj, "TEXT_JUSTIFY", context->textJustify);
            prompt_output(translate("Specify bottom-right point of text or [Justify/Setfont]: "));
        }
        else {
            alert(translate("Invalid option keyword."));
            prompt_output(translate("Text Justification Options [Center/Right/Align/Middle/Fit/TL/TC/TR/ML/MC/MR/BL/BC/BR]: "));
        }
    }
    else if (context->mode == TEXTSINGLE_MODE_SETFONT) {
        context->mode = TEXTSINGLE_MODE_SETGEOM;
        context->textFont = str;
        obj_add_rubber_text(obj, "TEXT_FONT", context->textFont);
        set_text_font(context->textFont);
        prompt_output(translate("Specify start point of text or [Justify/Setfont]: "));
    }
    else if (context->mode == TEXTSINGLE_MODE_SETGEOM) {
        if (isnan(context->textX)) {
            if (str == "J" || str == "JUSTIFY") {
                context->mode = TEXTSINGLE_MODE_JUSTIFY;
                prompt_output(translate("Text Justification Options [Center/Right/Align/Middle/Fit/TL/TC/TR/ML/MC/MR/BL/BC/BR]: "));
            }
            else if (str == "S" || str == "SETFONT") {
                context->mode = TEXTSINGLE_MODE_SETFONT;
                prompt_output(translate("Specify font name: "));
            }
            else {
                if (!parse_vector(str, &v)) {
                    alert(translate("Point or option keyword required."));
                    prompt_output(translate("Specify start point of text or [Justify/Setfont]: "));
                }
                else {
                    context->textX = Number(strList[0]);
                    context->textY = Number(strList[1]);
                    obj_add_rubber(obj, "LINE");
                    obj_set_rubber_mode(obj, "LINE");
                    obj_set_rubber_point(obj, "LINE_START", context->textX, context->textY);
                    prompt_output(translate("Specify text height") + " {" + textSize() + "}: ");
                }
            }
        }
        else if (isnan(context->textHeight)) {
            if (str == "") {
                context->textHeight = textSize();
                prompt_output(translate("Specify text angle") + " {" + textAngle() + "}: ");
            }
            else if (isnan(str)) {
                alert(translate("Requires valid numeric distance or second point."));
                prompt_output(translate("Specify text height") + " {" + textSize() + "}: ");
            }
            else {
                context->textHeight = Number(str);
                obj_set_text_size(obj, context->textHeight);
                prompt_output(translate("Specify text angle") + " {" + textAngle() + "}: ");
            }
        }
        else if (isnan(context->textRotation)) {
            if (str == "") {
                context->textRotation = textAngle();
                prompt_output(translate("Enter text: "));
                context->mode = TEXTSINGLE_MODE_RAPID;
                enablePromptRapidFire();
                clearRubber();
                obj_add_rubber(obj, "TEXTSINGLE");
                obj_set_rubber_mode(obj, "TEXTSINGLE");
                obj_set_rubber_point(obj, "TEXT_POINT", context->textX, context->textY);
                obj_set_rubber_point(obj, "TEXT_HEIGHT_ROTATION", context->textHeight, context->textRotation);
                obj_set_rubber_text(obj, "TEXT_FONT", context->textFont);
                obj_set_rubber_text(obj, "TEXT_JUSTIFY", context->textJustify);
                obj_set_rubber_text(obj, "TEXT_RAPID", context->text);
            }
            else if (isnan(str)) {
                alert(translate("Requires valid numeric angle or second point."));
                prompt_output(translate("Specify text angle") + " {" + textAngle() + "}: ");
            }
            else {
                context->textRotation = Number(str);
                setSettingsTextAngle(context->textRotation);
                prompt_output(translate("Enter text: "));
                context->mode = TEXTSINGLE_MODE_RAPID;
                command_prompt("enable rapidfire");
                clearRubber();
                obj_add_rubber(obj, "TEXTSINGLE");
                obj_set_rubber_mode(obj, "TEXTSINGLE");
                obj_set_rubber_point(obj, "TEXT_POINT", context->textX, context->textY);
                obj_set_rubber_point(obj, "TEXT_HEIGHT_ROTATION", context->textHeight, context->textRotation);
                obj_add_rubber_text(obj, "TEXT_FONT", context->textFont);
                obj_add_rubber_text(obj, "TEXT_JUSTIFY", context->textJustify);
                obj_add_rubber_text(obj, "TEXT_RAPID", context->text);
            }
        }
        else {
            /* Do nothing, as we are in rapidFire mode now. */
        }
    }
    else if (context->mode == TEXTSINGLE_MODE_RAPID) {
        if (str == "RAPID_ENTER") {
            if (context->text == "") {
                endCommand();
            }
            else {
                vulcanize();
                endCommand();
                debug_message("TODO: Rather than ending the command, calculate where the next line would be and modify the x/y to the new point.");
            }
        }
        else {
            context->text = str;
            obj_add_rubber_text(obj, "TEXT_RAPID", context->text);
        }
    }
}

QList<QPainterPath>
TextSingleObject::subPathList() const
{
    EmbReal s = obj->scale();
    QTransform trans;
    trans.rotate(rotation());
    trans.scale(s,s);

    QList<QPainterPath> pathList;

    QPainterPath path = textPath;

    QPainterPath::Element element;
    QList<int> pathMoves;
    int numMoves = 0;

    for (int i = 0; i < path.elementCount(); i++) {
        element = path.elementAt(i);
        if (element.isMoveTo()) {
            pathMoves << i;
            numMoves++;
        }
    }

    pathMoves << path.elementCount();

    for (int p = 0; p < pathMoves.size()-1 && p < numMoves; p++) {
        QPainterPath subPath;
        for (int i = pathMoves.value(p); i < pathMoves.value(p+1); i++) {
            element = path.elementAt(i);
            if (element.isMoveTo()) {
                subPath.moveTo(element.x, element.y);
            }
            else if (element.isLineTo()) {
                subPath.lineTo(element.x, element.y);
            }
            else if (element.isCurveTo()) {
                subPath.cubicTo(path.elementAt(i  ).x, path.elementAt(i  ).y, /* control point 1 */
                                path.elementAt(i+1).x, path.elementAt(i+1).y, /* control point 2 */
                                path.elementAt(i+2).x, path.elementAt(i+2).y); /* end point */
            }
        }
        pathList.append(trans.map(subPath));
    }

    return pathList;
}
#endif

/* QUICKLEADER */
ScriptValue
quickleader_command(ScriptEnv *context)
{

    end_command();
    return script_null;
}

#if 0
function main()
{
    debug_message("TODO: Adding the text is not complete yet.");
    context->point1 = emb_vector(0.0, 0.0);
    context->point2 = emb_vector(0.0, 0.0);
    prompt_output(translate("Specify first point: "));
}

void
click(EmbVector v)
{
    if (isnan(context->x1)) {
        context->point1 = v;
        obj_add_rubber(obj, "DIMLEADER");
        obj_set_rubber_mode(obj, "DIMLEADER_LINE");
        obj_set_rubber_point(obj, "DIMLEADER_LINE_START", context->point1);
        prompt_output(translate("Specify second point: "));
    }
    else {
        context->point2 = v;
        obj_set_rubber_point(obj, "DIMLEADER_LINE_END", context->point2);
        vulcanize();
        end_command();
    }
}

void
context(str)
{
    debug_message("TODO: QUICKLEADER context()");
}

void
prompt(str)
{
    EmbVector v;
    if (isnan(context->x1)) {
        if (!parse_vector(str, &v)) {
            alert(translate("Requires two points."));
            prompt_output(translate("Specify first point: "));
        }
        else {
            context->point1 = v;
            obj_add_rubber(obj, "DIMLEADER");
            obj_set_rubber_mode(obj, "DIMLEADER_LINE");
            obj_set_rubber_point(obj, "DIMLEADER_LINE_START", context->point1);
            prompt_output(translate("Specify second point: "));
        }
    }
    else {
        if (!parse_vector(str, &v)) {
            alert(translate("Requires two points."));
            prompt_output(translate("Specify second point: "));
        }
        else {
            context->point2 = v;
            obj_set_rubber_point(obj, "DIMLEADER_LINE_END", context->point2);
            vulcanize();
            end_command();
        }
    }
}
#endif


/* . */
EmbVector
obj_top_left(ObjectCore *obj)
{
    EmbRect rect = obj_rect(obj);
    EmbVector p = emb_vector(rect.x, rect.y);
    p = scale_and_rotate(p, obj->scale, obj->rotation);
    /* EMB_POS? */
    EmbVector v = emb_gget(obj->geometry, EMB_CENTER).v;
    return emb_vector_add(v, p);
}

/* . */
EmbVector
obj_top_right(ObjectCore *obj)
{
    EmbRect rect = obj_rect(obj);
    EmbVector p = emb_vector(rect.x + rect.w, rect.y);
    p = scale_and_rotate(p, obj->scale, obj->rotation);
    /* EMB_POS? */
    EmbVector v = emb_gget(obj->geometry, EMB_CENTER).v;
    return emb_vector_add(v, p);
}

/* . */
EmbVector
obj_bottom_left(ObjectCore *obj)
{
    EmbRect rect = obj_rect(obj);
    EmbVector p = emb_vector(rect.x, rect.y + rect.h);
    p = scale_and_rotate(p, obj->scale, obj->rotation);
    /* EMB_POS? */
    EmbVector v = emb_gget(obj->geometry, EMB_CENTER).v;
    return emb_vector_add(v, p);
}

/* . */
EmbVector
obj_bottom_right(ObjectCore *obj)
{
    EmbRect rect = obj_rect(obj);
    EmbVector p = emb_vector(rect.x + rect.w, rect.y + rect.h);
    p = scale_and_rotate(p, obj->scale, obj->rotation);
    /* EMB_POS? */
    EmbVector v = emb_gget(obj->geometry, EMB_CENTER).v;
    return emb_vector_add(v, p);
}

/* . */
void
obj_set_text_font(ObjectCore* obj, const char *font)
{
    strcpy(obj->textFont, font);
    obj_set_text(obj, obj->text);
}

/* Verify the string is a valid option. */
void
obj_set_text_justify(ObjectCore* obj, const char *justify)
{
    /* Default */
    strcpy(obj->textJustify, "Left");
    /* See if a valid justify option has been passed in, then set it if it is. */
    int n = table_length((char**)justify_options);
    for (int i=0; i<n; i++) {
        if (string_equal((char*)justify, justify_options[i])) {
            strcpy(obj->textJustify, "Left");
            break;
        }
    }
    obj_set_text(obj, obj->text);
}

/* . */
void
obj_set_text_size(ObjectCore* obj, EmbReal size)
{
    obj->textSize = size;
    obj_set_text(obj, obj->text);
}

/* . */
void
obj_set_text_style(ObjectCore* obj, bool bold, bool italic, bool under, bool strike, bool over)
{
    obj->textBold = bold;
    obj->textItalic = italic;
    obj->textUnderline = under;
    obj->textStrikeOut = strike;
    obj->textOverline = over;
    obj_set_text(obj, obj->text);
}

/* . */
void
obj_set_text_bold(ObjectCore* obj, bool val)
{
    obj->textBold = val;
    obj_set_text(obj, obj->text);
}

/* . */
void
obj_set_text_italic(ObjectCore* obj, bool val)
{
    obj->textItalic = val;
    obj_set_text(obj, obj->text);
}

/* . */
void
obj_set_text_underline(ObjectCore* obj, bool val)
{
    obj->textUnderline = val;
    obj_set_text(obj, obj->text);
}

/* . */
void
obj_set_text_strikeout(ObjectCore* obj, bool val)
{
    obj->textStrikeOut = val;
    obj_set_text(obj, obj->text);
}

/* . */
void
obj_set_text_overline(ObjectCore* obj, bool val)
{
    obj->textOverline = val;
    obj_set_text(obj, obj->text);
}

/* . */
void
obj_set_text_backward(ObjectCore* obj, bool val)
{
    obj->textBackward = val;
    obj_set_text(obj, obj->text);
}

/* . */
void
obj_set_text_upside_down(ObjectCore* obj, bool val)
{
    obj->textUpsideDown = val;
    obj_set_text(obj, obj->text);
}

/* . */
void
obj_vulcanize(int32_t obj)
{
    debug_message("vulcanize()");
    /* FIXME: obj_update_rubber(painter); */

    ObjectCore *core = obj_get_core(obj);
    core->rubber_mode = RUBBER_OFF;
    EmbGeometry *g = core->geometry;

    switch (g->type) {
    case EMB_POLYLINE:
        if (g->object.polyline.pointList->length == 0) {
            messagebox("critical",
                translate("Empty Polyline Error"),
                translate("The polyline added contains no points. The command that created this object has flawed logic."));
        }
        break;
    case EMB_POLYGON:
        if (g->object.polygon.pointList->length == 0) {
            messagebox("critical",
                translate("Empty Polygon Error"),
                translate("The polygon added contains no points. The command that created this object has flawed logic."));
        }
        break;
    case EMB_PATH:
        if (g->object.path.pointList->length == 0) {
            messagebox("critical",
                translate("Empty Path Error"),
                translate("The path added contains no points. The command that created this object has flawed logic."));
        }
        break;
    default:
        break;
    }
}

/* . */
void
obj_set_rubber_point(uint32_t id, EmbString key, EmbVector point)
{
    ObjectCore *core = obj_get_core(id);
    strcpy(core->rubber_points[core->n_rubber_points].key, key);
    core->rubber_points[core->n_rubber_points].vector = point;
    core->n_rubber_points++;
}

/* . */
void
obj_set_rubber_text(uint32_t id, EmbString key, EmbString txt)
{
    ObjectCore *core = obj_get_core(id);
    strcpy(core->rubber_texts[core->n_rubber_texts].key, key);
    strcpy(core->rubber_texts[core->n_rubber_texts].value, txt);
    core->n_rubber_texts++;
}

/* . */
EmbVector
obj_rubber_point(int32_t id, const char *key)
{
    ObjectCore *core = obj_get_core(id);
    for (int i=0; i<(int)core->n_rubber_points; i++) {
        if (string_equal(core->rubber_points[i].key, key)) {
            return core->rubber_points[i].vector;
        }
    }

    /* TODO: object's scene rather than current. */
    int doc = active_document();
    DocumentData *data = doc_data(doc);
    return data->sceneQSnapPoint;
}

/* . */
const char *
obj_rubber_text(int32_t id, const char *key)
{
    ObjectCore *core = obj_get_core(id);
    for (int i=0; i<(int)core->n_rubber_texts; i++) {
        if (string_equal(core->rubber_texts[i].key, key)) {
            return core->rubber_texts[i].value;
        }
    }
    return "";
}

/* . */
EmbArc
emb_arc_set_radius(EmbArc arc, EmbReal radius)
{
    EmbGeometry geometry;
    geometry.object.arc = arc;
    radius = EMB_MAX(radius, 0.0000001);
    EmbVector center = emb_gget(&geometry, EMB_CENTER).v;

    EmbVector start = emb_vector_subtract(center, arc.start);
    start = emb_vector_scale(start, radius/emb_vector_length(start));
    arc.start = emb_vector_add(center, start);

    EmbVector mid = emb_vector_subtract(center, arc.mid);
    mid = emb_vector_scale(mid, radius/emb_vector_length(mid));
    arc.mid = emb_vector_add(center, mid);

    EmbVector end = emb_vector_subtract(center, arc.end);
    end = emb_vector_scale(start, radius/emb_vector_length(end));
    arc.end = emb_vector_add(center, end);

    return arc;
}

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
        if (emb_approx(before, emb_gget(core->geometry, EMB_CENTER).v)) {
            obj_move_by(obj_id, delta);
        }
        else {
            EmbReal length = emb_vector_distance(emb_gget(core->geometry, EMB_CENTER).v, after);
            emb_set_radius(core->geometry, length);
        }
        break;
    }
    case EMB_DIM_LEADER:
    case EMB_LINE: {
        if (emb_approx(before, emb_gget(core->geometry, EMB_START).v)) {
            emb_gset(core->geometry, EMB_START, script_vector(after));
        }
        else if (emb_approx(before, emb_gget(core->geometry, EMB_END).v)) {
            emb_gset(core->geometry, EMB_END, script_vector(after));
        }
        else if (emb_approx(before, emb_gget(core->geometry, EMB_MID).v)) {
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
        EmbVector v = emb_gget(core->geometry, EMB_CENTER).v;
        if (emb_approx(before, v)) {
            obj_move_by(obj_id, delta);
        }
        break;
    }
    }
}

/* The caller is responsible for freeing this memory, currently. */
std::vector<EmbVector>
all_grip_points(int32_t obj_id)
{
    ObjectCore *core = obj_get_core(obj_id);
    EmbGeometry *g = core->geometry;
    std::vector<EmbVector> gripPoints;
    switch (core->geometry->type) {
    case EMB_ARC: {
        gripPoints.push_back(emb_gget(g, EMB_CENTER).v);
        gripPoints.push_back(emb_gget(g, EMB_START).v);
        gripPoints.push_back(emb_gget(g, EMB_MID).v);
        gripPoints.push_back(emb_gget(g, EMB_END).v);
        break;
    }
    case EMB_CIRCLE:
    case EMB_ELLIPSE: {
        gripPoints.push_back(emb_gget(core->geometry, EMB_CENTER).v);
        gripPoints.push_back(emb_quadrant(core->geometry, 0));
        gripPoints.push_back(emb_quadrant(core->geometry, 90));
        gripPoints.push_back(emb_quadrant(core->geometry, 180));
        gripPoints.push_back(emb_quadrant(core->geometry, 270));
        break;
    }
    case EMB_DIM_LEADER: {
        gripPoints.push_back(emb_gget(g, EMB_START).v);
        gripPoints.push_back(emb_gget(g, EMB_END).v);
        if (core->curved) {
            gripPoints.push_back(emb_gget(g, EMB_MID).v);
        }
        break;
    }
    case EMB_IMAGE: {
        gripPoints.push_back(obj_top_left(core));
        gripPoints.push_back(obj_top_right(core));
        gripPoints.push_back(obj_bottom_left(core));
        gripPoints.push_back(obj_bottom_right(core));
        break;
    }
    case EMB_LINE: {
        gripPoints.push_back(emb_gget(g, EMB_START).v);
        gripPoints.push_back(emb_gget(g, EMB_END).v);
        gripPoints.push_back(emb_gget(g, EMB_MID).v);
        break;
    }
    case EMB_PATH: {
        /* EMB_POS? */
        gripPoints.push_back(emb_gget(g, EMB_CENTER).v);
        debug_message("TODO: loop thru all path Elements and return their points.");
        break;
    }
    case EMB_POLYGON:
    case EMB_POLYLINE: {
        /* FIXME:
        QPainterPath::Element element;
        for (int i = 0; i < normalPath.elementCount(); ++i) {
            element = normalPath.elementAt(i);
            gripPoints.push_back(to_emb_vector(mapToScene(element.x, element.y)));
        }
        */
        break;
    }
    case EMB_TEXT_SINGLE:
    case EMB_POINT:
    default:
        gripPoints.push_back(emb_gget(g, EMB_CENTER).v);
        break;
    }
    return gripPoints;
}

/* Returns the closest snap point to the mouse point */
EmbVector
mouse_snap_point(int32_t obj_id, EmbVector mousePoint)
{
    std::vector<EmbVector> points = all_grip_points(obj_id);
    
    return find_mouse_snap_point(all_grip_points(obj_id), mousePoint);
}

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


