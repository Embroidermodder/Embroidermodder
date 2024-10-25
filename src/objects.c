/*
 * Embroidermodder 2.
 *
 * Copyright 2011-2024 The Embroidermodder Team
 * Embroidermodder 2 is Open Source Software, see LICENSE.md for licensing terms.
 *
 * Read the reference manual (https://www.libembroidery.org/downloads/emrm.pdf)
 * for advice on altering this file.
 *
 * ObjectCore: the additional information and tools on top of EmbGeometry
 * required for the user interface.
 */

#include "core.h"

/* . */
uint32_t
copy_object(uint32_t obj)
{
    debug_message("Object Copy()");
    ObjectCore *core = obj_get_core(obj);
    uint32_t copy = 0xFFFFFFFF; /* error symbol */
    todo("get_current_line_type");
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
        obj_set_rotation(copy_core, core->rotation);
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
    todo("get_current_line_type");
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
    todo("get_current_line_type");
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
    todo("get_current_line_type");
    uint32_t obj = create_object(EMB_DIM_LEADER, rgb);
    ObjectCore *core = obj_get_core(obj);

    core->curved = false;
    core->filled = true;
    obj_set_end_point_1(core, line.start);
    obj_set_end_point_2(core, line.end);
    return obj;
}

/* . */
uint32_t
create_ellipse(EmbEllipse ellipse, uint32_t rgb)
{
    debug_message("EllipseObject Constructor()");
    todo("get_current_line_type");
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
    todo("get_current_line_type");
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
    todo("get_current_line_type");
    obj_set_end_point_1(core, line.start);
    obj_set_end_point_2(core, line.end);
    return obj;
}

/* . */
uint32_t
create_path(EmbPath *p, uint32_t rgb)
{
    debug_message("PathObject Constructor()");
    uint32_t obj = create_object(EMB_PATH, rgb);
    todo("get_current_line_type");
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
    todo("get_current_line_type");
    // FIXME: obj_update_path_r(obj, p);
    //obj_set_pos(obj->core, v);
    return obj;
}

/* . */
uint32_t
create_polyline(EmbPath *path, uint32_t rgb)
{
    debug_message("PolylineObject Constructor()");
    todo("get_current_line_type");
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
    todo("get_current_line_type");
    uint32_t obj = create_object(EMB_RECT, rgb);
    obj_set_rect(obj, rect.x, rect.y, rect.w, rect.h);
    return obj;
}

/* . */
uint32_t
create_text_single(EmbString str, EmbVector v, uint32_t rgb)
{
    debug_message("TextSingleObject Constructor()");
    todo("get_current_line_type");
    uint32_t obj = create_object(EMB_TEXT_SINGLE, rgb);
    ObjectCore *core = obj_get_core(obj);

    string_copy(core->textJustify, "Left");
    /* TODO: set the justification properly */

    obj_set_text(core, str);
    obj_set_pos(core, v);
    return obj;
}

/* . */
EmbVector
find_mouse_snap_point(EmbVectorList *snap_points, EmbVector mouse_point)
{
    float closest = 1.0e10;
    EmbVector result = snap_points->data[0];
    int i;
    for (i=0; i<snap_points->count; i++) {
        float distance = emb_vector_distance(snap_points->data[i], mouse_point);
        if (distance < closest) {
            closest = distance;
            result = snap_points->data[i];
        }
    }
    return result;
}

/* . */
EmbVector
obj_pos(ObjectCore *obj)
{
    return obj->position;
}

/* . */
double
obj_x(ObjectCore *obj)
{
    return obj->position.x;
}

/* . */
double
obj_y(ObjectCore *obj)
{
    return obj->position.y;
}

/* . */
double
obj_center_x(ObjectCore *obj)
{
    return obj_pos(obj).x;
}

/* . */
double
obj_center_y(ObjectCore *obj)
{
    return obj_pos(obj).y;
}

/* . */
double
obj_radius(ObjectCore *obj)
{
    return emb_radius(obj->geometry);
}

/* . */
double
obj_diameter(ObjectCore *obj)
{
    return emb_diameter(obj->geometry);
}

/* . */
double
obj_circumference(ObjectCore *obj)
{
    return embConstantPi * emb_diameter(obj->geometry);
}

/* . */
EmbVector
obj_delta(ObjectCore *obj)
{
    return emb_vector_subtract(obj_end_point_2(obj), obj_end_point_1(obj));
}

/* . */
EmbVector
obj_center(ObjectCore *obj)
{
    return obj_pos(obj);
}

/* . */
EmbVector
obj_start_point(ObjectCore *obj)
{
    switch (obj->geometry->type) {
    case EMB_ARC: {
        return obj->geometry->object.arc.start;
    }
    default:
        break;
    }
    return obj_pos(obj);
}

/* . */
EmbVector
obj_end_point(ObjectCore *obj)
{
    switch (obj->geometry->type) {
    case EMB_ARC: {
        return obj->geometry->object.arc.end;
    }
    default:
        break;
    }
    return obj_pos(obj);
}

/* . */
EmbVector
obj_end_point_1(ObjectCore *obj)
{
    return obj_pos(obj);
}

/* . */
EmbVector
obj_end_point_2(ObjectCore *obj)
{
    /* FIXME:
    QLineF lyne = obj_line(obj);
    EmbVector endPoint2;
    endPoint2.x = lyne.x2();
    endPoint2.y = lyne.y2();
    return emb_vector_add(obj_pos(obj->core), scale_and_rotate(endPoint2, obj->scale(), obj->rotation()));
    */
    return emb_vector(0.0, 0.0);
}

/* . */
EmbVector
obj_mid_point(ObjectCore *obj)
{
    switch (obj->geometry->type) {
    case EMB_ARC: {
        return obj->geometry->object.arc.mid;
    }
    case EMB_LINE:
    case EMB_DIM_LEADER: {
        /* FIXME:
        QLineF lyne = obj_line(obj);
        EmbVector mp = to_emb_vector(lyne.pointAt(0.5));
        return emb_vector_add(obj_pos(obj->core), scale_and_rotate(mp, obj->scale(), obj->rotation()));
        */
    }
    default:
        break;
    }
    return obj_pos(obj);
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
    EmbVector center = obj_center(obj);
    center.x = centerX;
    obj_set_center(obj, center);
}

/* . */
void
obj_set_center_y(ObjectCore *obj, EmbReal centerY)
{
    EmbVector center = obj_center(obj);
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
    EmbVector v = obj_end_point_1(obj);
    v.x = x;
    obj_set_end_point_1(obj, v);
}

/* . */
void
obj_set_y1(ObjectCore *obj, EmbReal y)
{
    EmbVector v = obj_end_point_1(obj);
    v.y = y;
    obj_set_end_point_1(obj, v);
}

/* . */
void
obj_set_x2(ObjectCore *obj, EmbReal x)
{
    EmbVector v = obj_end_point_2(obj);
    v.x = x;
    obj_set_end_point_2(obj, v);
}

/* . */
void
obj_set_y2(ObjectCore *obj, EmbReal y)
{
    EmbVector v = obj_end_point_2(obj);
    v.y = y;
    obj_set_end_point_2(obj, v);
}

/* . */
void
obj_set_end_point_2(ObjectCore *obj, EmbVector endPt2)
{
    EmbVector endPt1 = obj_pos(obj);
    EmbVector delta = emb_vector_subtract(endPt2, endPt1);
    /*
    obj->setRotation(0);
    obj->setScale(1);
    obj_set_line(obj, 0, 0, delta.x, delta.y);
    obj->setPos(endPt1.x, endPt1.y);
    */
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
        todo("CIRCLE context()");
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
            todo("CIRCLE click() for TTR");
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
        todo("CIRCLE prompt() for TTR");
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
    todo("Make arrowStyle, arrowStyleAngle, arrowStyleLength, lineStyleAngle, lineStyleLength customizable");
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
    todo("ELLIPSE context()");
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
        todo("LINE context()");
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
            todo("LINE prompt() for UNDO");
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
    todo("Path command is currently broken.");
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
        todo("PATH context()");
        break;
    case CONTEXT_PROMPT:
        /*
        if (str == "A" || str == "ARC") {
            todo("PATH prompt() for ARC");
        }
        else if (str == "U" || str == "UNDO") {
            todo("PATH prompt() for UNDO");
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
    todo("translate needed here when complete");
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
    todo("POINT context()");
}

function prompt(str)
{
    EmbVector v;
    if (global->firstRun) {
        if (str == "M" || str == "MODE") {
            todo("POINT prompt() for PDMODE");
        }
        else if (str == "S" || str == "SIZE") {
            todo("POINT prompt() for PDSIZE");
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
    todo("get_current_line_type");
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
        todo("POLYGON Sidelength mode");
        break;
    }
    }
}

function
context(char *str)
{
    todo("POLYGON context()");
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
        todo("POLYGON Sidelength mode");
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
    todo("POLYLINE context()");
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
            todo("POLYLINE prompt() for UNDO");
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
    todo("RECTANGLE context()");
}

function prompt(str)
{
    if (str == "C" || str == "CHAMFER") {
        todo("RECTANGLE prompt() for CHAMFER");
    }
    else if (str == "D" || str == "DIMENSIONS") {
        todo("RECTANGLE prompt() for DIMENSIONS");
    }
    else if (str == "F" || str == "FILLET") {
        todo("RECTANGLE prompt() for FILLET");
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
    todo("saving polygons, polylines and paths must be stable before we go here.");

    todo("This needs to work like a path, not a polyline. Improve this");

    TextSingleObjectCore* obj = static_cast<TextSingleObjectCore*>(item);
    if (obj) {
        if (formatType == EMBFORMAT_STITCHONLY) {
            QList<QPainterPath> pathList = obj->objectSavePathList();
            foreach(QPainterPath path, pathList) {
                toPolyline(pattern, obj->objectPos(), path.simplified(), "0", obj_color(obj), "CONTINUOUS", "BYLAYER");
                todo("proper layer/lineType/lineWeight");
                todo("Improve precision, replace simplified");
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
    todo("SINGLELINETEXT context()");
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
                todo("Rather than ending the command, calculate where the next line would be and modify the x/y to the new point.");
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
    todo("Adding the text is not complete yet.");
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
    todo("QUICKLEADER context()");
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
    return emb_vector_add(obj_pos(obj), p);
}

/* . */
EmbVector
obj_top_right(ObjectCore *obj)
{
    EmbRect rect = obj_rect(obj);
    EmbVector p = emb_vector(rect.x + rect.w, rect.y);
    p = scale_and_rotate(p, obj->scale, obj->rotation);
    return emb_vector_add(obj_pos(obj), p);
}

/* . */
EmbVector
obj_bottom_left(ObjectCore *obj)
{
    EmbRect rect = obj_rect(obj);
    EmbVector p = emb_vector(rect.x, rect.y + rect.h);
    p = scale_and_rotate(p, obj->scale, obj->rotation);
    return emb_vector_add(obj_pos(obj), p);
}

/* . */
EmbVector
obj_bottom_right(ObjectCore *obj)
{
    EmbRect rect = obj_rect(obj);
    EmbVector p = emb_vector(rect.x + rect.w, rect.y + rect.h);
    p = scale_and_rotate(p, obj->scale, obj->rotation);
    return emb_vector_add(obj_pos(obj), p);
}

/* . */
void
obj_set_text_font(ObjectCore* obj, const char *font)
{
    string_copy(obj->textFont, font);
    obj_set_text(obj, obj->text);
}

EmbStringTable justify_options = {
    "Left",
    "Center",
    "Right",
    "Aligned",
    "Middle",
    "Fit",
    "Top Left",
    "Top Center",
    "Top Right",
    "Middle Left",
    "Middle Center",
    "Middle Right",
    "Bottom Left",
    "Bottom Center",
    "Bottom Right",
    "END"
};

/* Verify the string is a valid option. */
void
obj_set_text_justify(ObjectCore* obj, const char *justify)
{
    /* Default */
    string_copy(obj->textJustify, "Left");
    /* See if a valid justify option has been passed in, then set it if it is. */
    int n = string_array_length(justify_options);
    for (int i=0; i<n; i++) {
        if (string_equal(justify, justify_options[i])) {
            string_copy(obj->textJustify, "Left");
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
obj_set_end_point_1(ObjectCore *obj, EmbVector endPt1)
{
    EmbVector endPt2 = obj_end_point_2(obj);
    EmbVector delta = emb_vector_subtract(endPt2, endPt1);
    /*
    obj->setRotation(0);
    obj->setScale(1);
    obj_set_line(obj, 0, 0, delta.x, delta.y);
    obj->setPos(endPt1.x, endPt1.y);
    */
}

/* . */
EmbArc
emb_arc_set_radius(EmbArc arc, EmbReal radius)
{
    EmbGeometry geometry;
    geometry.object.arc = arc;
    radius = EMB_MAX(radius, 0.0000001);
    EmbVector center = emb_arc_center(geometry);

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

