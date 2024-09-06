/*
 * Embroidermodder 2.
 *
 * Copyright 2011-2024 The Embroidermodder Team
 * Embroidermodder 2 is Open Source Software, see LICENSE.md for licensing terms.
 *
 * Read the reference manual (https://www.libembroidery.org/downloads/emrm.pdf)
 * for advice on altering this file.
 *
 * Object: attempting to combine the geometry objects into one that changes
 * behaviour depending on its mode.
 */

#include "embroidermodder.h"

EmbVector obj_map_rubber(Object *obj, const char *key);
EmbVector map_from_scene(Object *obj, EmbVector v);

EmbVector
operator+(EmbVector a, EmbVector b)
{
    return emb_vector_add(a, b);
}

EmbVector
operator-(EmbVector a, EmbVector b)
{
    return emb_vector_subtract(a, b);
}

EmbVector
map_from_scene(Object *obj, EmbVector v)
{
    return to_emb_vector(obj->mapFromScene(to_qpointf(v)));
}

/* WARNING: DO NOT enable QGraphicsItem::ItemIsMovable. If it is enabled,
 * WARNING: and the item is double clicked, the scene will erratically move the item while zooming.
 * WARNING: All movement has to be handled explicitly by us, not by the scene.
 */
Object::Object(int type_, QRgb rgb, Qt::PenStyle lineType, QGraphicsItem* item)// : QGraphicsItem(item)
{
    debug_message("BaseObject Constructor()");

    obj = this;

    obj->setData(OBJ_TYPE, type_);
    if (type_ < 30) {
        obj->data.OBJ_NAME = object_names[type_];
    }
    else {
        obj->data.OBJ_NAME = "Unknown";
    }

    obj->data.objPen.setCapStyle(Qt::RoundCap);
    obj->data.objPen.setJoinStyle(Qt::RoundJoin);
    obj->data.lwtPen.setCapStyle(Qt::RoundCap);
    obj->data.lwtPen.setJoinStyle(Qt::RoundJoin);

    obj->data.objID = QDateTime::currentMSecsSinceEpoch();

    obj->data.gripIndex = -1;
    obj->data.curved = 0;

    obj->setFlag(QGraphicsItem::ItemIsSelectable, true);

    obj_set_color(obj, rgb);
    obj_set_line_type(obj, lineType);
    obj_set_line_weight(obj, 0.35);
    todo("pass in proper lineweight");
    obj->setPen(obj->data.objPen);

    geometry = (EmbGeometry*)malloc(sizeof(EmbGeometry));
    geometry->type = type_;
    geometry->object.color.r = qRed(rgb);
    geometry->object.color.g = qGreen(rgb);
    geometry->object.color.b = qBlue(rgb);
    geometry->lineType = lineType;
}

/* . */
Object::~Object()
{
    debug_message("ArcObject Destructor()");
    free(geometry);
}

/* . */
Object *
create_arc(EmbArc arc, QRgb rgb, QGraphicsItem *item)
{
    debug_message("ArcObject Constructor()");
    Object *obj = new Object(EMB_ARC, rgb, Qt::SolidLine, item);
    obj->geometry->object.arc = arc;
    todo("getCurrentLineType");
    obj_calculate_data(obj);
    obj_set_pos(obj, arc.start);
    return obj;
}

/* . */
Object *
create_circle(EmbCircle circle, QRgb rgb, QGraphicsItem *item)
{
    debug_message("CircleObject Constructor()");
    Object *obj = new Object(EMB_CIRCLE, rgb, Qt::SolidLine, item);
    todo("getCurrentLineType");
    obj->geometry->object.circle = circle;

    /*
    update_path();
    */
    return obj;
}

/* . */
Object *
create_ellipse(EmbEllipse ellipse, QRgb rgb, QGraphicsItem *item)
{
    debug_message("EllipseObject Constructor()");
    todo("getCurrentLineType");
    Object *obj = new Object(EMB_ELLIPSE, rgb, Qt::SolidLine, item);
    obj->geometry->object.ellipse = ellipse;

    /*
    setObjectSize(width, height);
    obj_update_path(obj);
    */
    return obj;
}

/* . */
Object *
create_polyline(EmbPath path, const QPainterPath& p, QRgb rgb, QGraphicsItem* parent)
{
    debug_message("PolylineObject Constructor()");
    todo("getCurrentLineType");
    Object *obj = new Object(EMB_POLYLINE, rgb, Qt::SolidLine);
    obj_update_path_r(obj, p);
    /* EmbVector v; obj_set_pos(obj, v); */
    return obj;
}

/* . */
Object *
create_path(double x, double y, const QPainterPath p, QRgb rgb, QGraphicsItem* parent)
{
    debug_message("PathObject Constructor()");
    Object *obj = new Object(EMB_PATH, rgb, Qt::SolidLine);
    todo("getCurrentLineType");
    obj_update_path_r(obj, p);
    EmbVector v;
    v.x = x;
    v.y = y;
    obj_set_pos(obj, v);
    return obj;
}

/* . */
Object *
create_polygon(double x, double y, const QPainterPath& p, QRgb rgb, QGraphicsItem* parent)
{
    debug_message("PolygonObject Constructor()");
    Object *obj = new Object(EMB_POLYGON, rgb, Qt::SolidLine);
    todo("getCurrentLineType");
    obj_update_path_r(obj, p);
    EmbVector v;
    v.x = x;
    v.y = y;
    obj_set_pos(obj, v);
    return obj;
}

/* . */
Object *
create_text_single(QString str, double x, double y, QRgb rgb, QGraphicsItem* parent)
{
    debug_message("TextSingleObject Constructor()");
    todo("getCurrentLineType");
    Object *obj = new Object(EMB_TEXT_SINGLE, rgb, Qt::SolidLine);
    obj->data.objTextJustify = "Left"; /* TODO: set the justification properly */

    obj_set_text(obj, str);
    EmbVector v;
    v.x = x;
    v.y = y;
    obj_set_pos(obj, v);
    return obj;
}

/* . */
Object *
create_dim_leader(double x1, double y1, double x2, double y2, QRgb rgb, QGraphicsItem* parent)
{
    debug_message("DimLeaderObject Constructor()");
    todo("getCurrentLineType");
    Object *obj = new Object(EMB_DIM_LEADER, rgb, Qt::SolidLine);

    obj->data.curved = false;
    obj->data.filled = true;
    EmbVector point1, point2;
    point1.x = x1;
    point1.y = y1;
    point2.x = x2;
    point2.y = y2;
    obj_set_end_point_1(obj, point1);
    obj_set_end_point_2(obj, point2);
    return obj;
}

/* . */
Object *
create_image(double x, double y, double w, double h, QRgb rgb, QGraphicsItem* parent)
{
    debug_message("ImageObject Constructor()");
    todo("getCurrentLineType");
    Object *obj = new Object(EMB_IMAGE, rgb, Qt::SolidLine);
    obj_set_rect(obj, x, y, w, h);
    return obj;
}

/* . */
Object *
create_rect(double x, double y, double w, double h, QRgb rgb, QGraphicsItem* parent)
{
    debug_message("RectObject Constructor()");
    todo("getCurrentLineType");
    Object *obj = new Object(EMB_RECT, rgb, Qt::SolidLine);
    obj_set_rect(obj, x, y, w, h);
    return obj;
}

/* . */
Object *
create_line(double x1, double y1, double x2, double y2, QRgb rgb, QGraphicsItem* parent)
{
    debug_message("LineObject Constructor()");
    Object *obj = new Object(EMB_LINE, rgb, Qt::SolidLine);
    todo("getCurrentLineType");
    EmbVector point1, point2;
    point1.x = x1;
    point1.y = y1;
    point2.x = x2;
    point2.y = y2;
    obj_set_end_point_1(obj, point1);
    obj_set_end_point_2(obj, point2);
    return obj;
}

/* . */
Object *
create_point(EmbPoint_ point, QRgb rgb, QGraphicsItem* parent)
{
    Object *obj = new Object(EMB_POINT, rgb, Qt::SolidLine);
    return obj;
}

/* . */
Object *
copy_object(Object* obj)
{
    debug_message("ArcObject Constructor()");
    if (!obj) {
        return NULL;
    }
    Object *copy = new Object(obj->geometry->type, obj_color_rgb(obj), Qt::SolidLine);
    switch (obj->geometry->type) {
    case EMB_ARC:
        copy->geometry->object.arc = obj->geometry->object.arc;
        todo("getCurrentLineType");
        copy->setRotation(obj->rotation());
        break;
    case EMB_CIRCLE:
        copy->geometry->object.circle = obj->geometry->object.circle;
        todo("getCurrentLineType");
        copy->setRotation(obj->rotation());
        break;
    case EMB_DIM_LEADER:
        copy->geometry->object.line = obj->geometry->object.line;
        /* init(obj_x1(obj), obj_y1(obj), obj_x2(obj), obj_y2(obj), obj_color_rgb(obj), Qt::SolidLine); */
        todo("getCurrentLineType");
        break;
    case EMB_ELLIPSE:
        copy->geometry->object.ellipse = obj->geometry->object.ellipse;
        /* init(obj->obj_centerX(), obj->obj_centerY(), obj->objectWidth(), obj->objectHeight(), obj_color_rgb(obj), Qt::SolidLine); */
        todo("getCurrentLineType");
        copy->setRotation(obj->rotation());
        break;
    case EMB_IMAGE: {
        copy->geometry->object.ellipse = obj->geometry->object.ellipse;
        /* EmbVector ptl = obj_top_left(obj); */
        /* init(ptl.x, ptl.y, obj->objectWidth(), obj->objectHeight(), obj_color_rgb(obj), Qt::SolidLine); */
        todo("getCurrentLineType");
        copy->setRotation(obj->rotation());
        break;
    }
    case EMB_LINE: {
        copy->geometry->object.line = obj->geometry->object.line;
        /* init(obj->objectX1(), obj->objectY1(), obj->objectX2(), obj->objectY2(), obj_color_rgb(obj), Qt::SolidLine); */
        todo("getCurrentLineType");
        break;
    }
    case EMB_PATH: {
        copy->geometry->object.path = obj->geometry->object.path;
        /* init(obj->objectX(), obj->objectY(), obj->objectCopyPath(), obj_color_rgb(obj), Qt::SolidLine);
         * obj->setRotation(obj->rotation());
         * obj->setScale(obj->scale());
         */
        todo("getCurrentLineType");
        break;
    }
    case EMB_POINT: {
        copy->geometry->object.point = obj->geometry->object.point;
        /* init(obj->objectX(), obj->objectY(), obj_color_rgb(obj), Qt::SolidLine);
         */
        todo("getCurrentLineType");
        copy->setRotation(obj->rotation());
        break;
    }
    case EMB_POLYGON: {
        copy->geometry->object.polygon = obj->geometry->object.polygon;
        /* init(obj->objectX(), obj->objectY(), obj->objectCopyPath(), obj_color_rgb(obj), Qt::SolidLine);
         * obj->setRotation(obj->rotation());
         * obj->setScale(obj->scale());
         */
        todo("getCurrentLineType");
        break;
    }
    case EMB_POLYLINE: {
        copy->geometry->object.polyline = obj->geometry->object.polyline;
        /* init(obj->objectX(), obj->objectY(), obj->objectCopyPath(), obj_color_rgb(obj), Qt::SolidLine);
         */
        todo("getCurrentLineType");
        copy->setRotation(obj->rotation());
        copy->setScale(obj->scale());
        break;
    }
    case EMB_RECT: {
        obj->geometry->object.rect = obj->geometry->object.rect;
        /* EmbVector ptl = obj->objectTopLeft();
         * init(ptl.x, ptl.y, obj->objectWidth(), obj->objectHeight(), obj_color_rgb(obj), Qt::SolidLine);
         */
        todo("getCurrentLineType");
        copy->setRotation(obj->rotation());
        break;
    }
    case EMB_TEXT_SINGLE: {
        obj_set_text_font(copy, obj->data.objTextFont);
        obj_set_text_size(copy, obj->data.objTextSize);
        copy->setRotation(obj->rotation());
        obj_set_text_backward(copy, obj->data.objTextBackward);
        obj_set_text_upside_down(copy, obj->data.objTextUpsideDown);
        obj_set_text_style(copy, obj->data.objTextBold, obj->data.objTextItalic,
            obj->data.objTextUnderline, obj->data.objTextStrikeOut, obj->data.objTextOverline);
        /* init(obj->objText, obj->objectX(), obj->objectY(), obj_color_rgb(obj), Qt::SolidLine);
         */
        todo("getCurrentLineType");
        copy->setScale(obj->scale());
        break;
    }
    default:
        break;
    }
    return copy;
}

QColor
obj_color(Object* obj)
{
    return obj->data.objPen.color();
}

QRgb
obj_color_rgb(Object* obj)
{
    return obj->data.objPen.color().rgb();
}

Qt::PenStyle
obj_line_type(Object* obj)
{
    return obj->data.objPen.style();
}

double
obj_line_weight(Object* obj)
{
    return obj->data.lwtPen.widthF();
}

QPainterPath
obj_path(Object* obj)
{
    return obj->path();
}

/* . */
ScriptValue
arc_command(ScriptEnv *context)
{

    return script_null;
}

/* . */
void
obj_draw_rubber_line(Object *obj, const QLineF& rubLine, QPainter* painter, const char* colorFromScene)
{
    if (painter) {
        QGraphicsScene* objScene = obj->scene();
        if (!objScene) {
            return;
        }
        QPen colorPen = obj->data.objPen;
        colorPen.setColor(QColor(objScene->property(colorFromScene).toUInt()));
        painter->setPen(colorPen);
        painter->drawLine(rubLine);
        painter->setPen(obj->data.objPen);
    }
}

/* . */
void
obj_real_render(Object *obj, QPainter* painter, QPainterPath renderPath)
{
    QColor color1 = obj->data.objPen.color(); /* lighter color */
    QColor color2 = color1.darker(150); /* darker color */

    /* If we have a dark color, lighten it. */
    int darkness = color1.lightness();
    int threshold = 32;
    todo("This number may need adjusted or maybe just add it to settings.");
    if (darkness < threshold) {
        color2 = color1;
        if (!darkness) {
            color1 = QColor(threshold, threshold, threshold);
        } /* lighter() does not affect pure black */
        else {
            color1 = color2.lighter(100 + threshold);
        }
    }

    int count = renderPath.elementCount();
    for (int i = 0; i < count-1; ++i) {
        QPainterPath::Element elem = renderPath.elementAt(i);
        QPainterPath::Element next = renderPath.elementAt(i+1);

        if (next.isMoveTo()) {
            continue;
        }

        QPainterPath elemPath;
        elemPath.moveTo(elem.x, elem.y);
        elemPath.lineTo(next.x, next.y);

        QPen renderPen(QColor(0,0,0,0));
        renderPen.setWidthF(0);
        painter->setPen(renderPen);
        QPainterPathStroker stroker;
        stroker.setWidth(0.35);
        stroker.setCapStyle(Qt::RoundCap);
        stroker.setJoinStyle(Qt::RoundJoin);
        QPainterPath realPath = stroker.createStroke(elemPath);
        painter->drawPath(realPath);

        QLinearGradient grad(elemPath.pointAtPercent(0.5), elemPath.pointAtPercent(0.0));
        grad.setColorAt(0, color1);
        grad.setColorAt(1, color2);
        grad.setSpread(QGradient::ReflectSpread);

        painter->fillPath(realPath, QBrush(grad));
    }
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
    context->point1 = zero_vector;
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
    MdiWindow *window = activeMdiWindow();
    if (window) {
        emb_array_add_circle(window->pattern->geometry, c);
    }
    /* _main->nativeAddCircle(0.0, 0.0, 10.0, true, OBJ_RUBBER_CIRCLE_1P_DIA); */
   
    end_command();
    return script_null;
}

ScriptValue
circle_click(ScriptEnv *context)
{
    switch (context->mode) {
    case CIRCLE_MODE_1P_RAD_ONE: {
    #if 0
        context->point1 = zero_vector;
        context->center = zero_vector;
        obj_add_rubber(obj, "CIRCLE");
        obj_set_rubber_mode(obj, "CIRCLE_1P_RAD");
        obj_set_rubber_point(obj, "CIRCLE_CENTER", v);
        prompt_output(translate("Specify radius of circle or [Diameter]: "));
        break;
    }
    case CIRCLE_MODE_1P_RAD_TWO: {
        context->point2 = v;
        obj_set_rubber_point(obj, "CIRCLE_RADIUS", context->point2);
        vulcanize();
        end_command();
        break;
    }
    case CIRCLE_MODE_1P_DIA_ONE: {
        error("CIRCLE", translate("This should never happen."));
        break;
    }
    case CIRCLE_MODE_1P_DIA_TWO: {
        context->point2 = v;
        obj_set_rubber_point(obj, "CIRCLE_DIAMETER", context->x2, context->y2);
        vulcanize();
        end_command();
        break;
    }
    case MODE_2P: {
        if (isnan(context->x1)) {
            context->point1 = v;
            obj_add_rubber(obj, "CIRCLE");
            obj_set_rubber_mode(obj, "CIRCLE_2P");
            obj_set_rubber_point(obj, "CIRCLE_TAN1", x, y);
            prompt_output(translate("Specify second end point of circle's diameter: "));
        }
        else if (isnan(context->x2)) {
            context->point2 = v;
            obj_set_rubber_point(obj, "CIRCLE_TAN2", x, y);
            vulcanize();
            end_command();
        }
        else {
            error("CIRCLE", translate("This should never happen."));
        }
        break;
    }
    case CIRCLE_MODE_3P: {
        if (isnan(context->x1)) {
            context->point1 = v;
            prompt_output(translate("Specify second point on circle: "));
        }
        else if (isnan(context->x2)) {
            context->point2 = v;
            obj_add_rubber(obj, "CIRCLE");
            obj_set_rubber_mode(obj, "CIRCLE_3P");
            obj_set_rubber_point(obj, "CIRCLE_TAN1", context->x1, context->y1);
            obj_set_rubber_point(obj, "CIRCLE_TAN2", context->x2, context->y2);
            prompt_output(translate("Specify third point on circle: "));
        }
        else if (isnan(context->x3)) {
            context->point3 = v;
            obj_set_rubber_point(obj, "CIRCLE_TAN3", context->x3, context->y3);
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
    case CIRCLE_MODE_1P_RAD_ONE: {
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
                var strList = str.split(",");
                if (isnan(strList[0]) || isnan(strList[1])) {
                    alert(translate("Point or option keyword required."));
                    prompt_output(translate("Specify center point for circle or [3P/2P/Ttr (tan tan radius)]: "));
                }
                else {
                    context->x1 = Number(strList[0]);
                    context->y1 = Number(strList[1]);
                    context->cx = context->x1;
                    context->cy = context->y1;
                    obj_add_rubber(obj, "CIRCLE");
                    obj_set_rubber_mode(obj, "CIRCLE_1P_RAD");
                    obj_set_rubber_point(obj, "CIRCLE_CENTER", context->cx, context->cy);
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
                    context->x2 = context->x1 + context->rad;
                    context->y2 = context->y1;
                    obj_set_rubber_point(obj, "CIRCLE_RADIUS", context->point2);
                    vulcanize();
                    endCommand();
                }
            }
        }
    }
    case CIRCLE_MODE_1P_DIA_ONE: {
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
                context->x2 = context->x1 + context->dia;
                context->y2 = context->y1;
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
    case CIRCLE_MODE_2P: {
        if (isnan(context->x1)) {
            if (!parse_vector(str, &v)) {
                alert(translate("Invalid point."));
                prompt_output(translate("Specify first end point of circle's diameter: "));
            }
            else {
                context->point1 = v;
                obj_add_rubber(obj, "CIRCLE");
                obj_set_rubber_mode(obj, "CIRCLE_2P");
                obj_set_rubber_point(obj, "CIRCLE_TAN1", context->x1, context->y1);
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
                obj_set_rubber_point(obj, "CIRCLE_TAN2", context->x2, context->y2);
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
                obj_set_rubber_point(obj, "CIRCLE_TAN1", context->x1, context->y1);
                obj_set_rubber_point(obj, "CIRCLE_TAN2", context->x2, context->y2);
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
                obj_set_rubber_point(obj, "CIRCLE_TAN3", context->x3, context->y3);
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
    double arrowStyleAngle = 15.0;
    double arrowStyleLength = 1.0;
    double lineStyleAngle = 45.0;
    double lineStyleLength = 1.0;

    QLineF lyne = obj_line(obj);
    double angle = lyne.angle();
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
        double side = QLineF(ap1, ap2).length();
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
        context->mode = ELLIPSE_MODE_MAJDIA_MINRAD_ONE;
        context->point1 = zero_vector;
        context->point2 = zero_vector;
        context->point3 = zero_vector;
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
    case ELLIPSE_MODE_MAJDIA_MINRAD: {
        if (isnan(context->x1)) {
            context->point1 = v;
            obj_add_rubber(obj, "ELLIPSE");
            obj_set_rubber_mode(obj, "ELLIPSE_LINE");
            obj_set_rubber_point(obj, "ELLIPSE_LINE_POINT1", context->x1, context->y1);
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
            context->height = perpendicularDistance(context->x3, context->y3, context->x1, context->y1, context->x2, context->y2)*2.0;
            obj_set_rubber_point(obj, "ELLIPSE_AXIS2_POINT2", context->x3, context->y3);
            vulcanize();
            end_command();
        }
        else {
            error("ELLIPSE", translate("This should never happen."));
        }
        break;
    }
    case ELLIPSE_MODE_MAJRAD_MINRAD: {
        if (isnan(context->x1)) {
            context->point1 = v;
            context->center = context->point1;
            obj_add_rubber(obj, "ELLIPSE");
            obj_set_rubber_mode(obj, "ELLIPSE_LINE");
            obj_set_rubber_point(obj, "ELLIPSE_LINE_POINT1", context->x1, context->y1);
            obj_set_rubber_point(obj, "ELLIPSE_CENTER", context->cx, context->cy);
            prompt_output(translate("Specify first axis end point: "));
        }
        else if (isnan(context->x2)) {
            context->point2 = v;
            context->width = calculateDistance(context->cx, context->cy, context->x2, context->y2)*2.0;
            context->rot = calculateAngle(context->x1, context->y1, context->x2, context->y2);
            obj_set_rubber_mode(obj, "ELLIPSE_MAJRAD_MINRAD");
            obj_set_rubber_point(obj, "ELLIPSE_AXIS1_POINT2", context->x2, context->y2);
            obj_set_rubber_point(obj, "ELLIPSE_WIDTH", context->width, 0);
            obj_set_rubber_point(obj, "ELLIPSE_ROT", context->rot, 0);
            prompt_output(translate("Specify second axis end point or [Rotation]: "));
        }
        else if (isnan(context->x3)) {
            context->point3 = v;
            context->height = perpendicularDistance(context->x3, context->y3, context->cx, context->cy, context->x2, context->y2)*2.0;
            obj_set_rubber_point(obj, "ELLIPSE_AXIS2_POINT2", context->x3, context->y3);
            vulcanize();
            end_command();
        }
        else {
            error("ELLIPSE", translate("This should never happen."));
        }
        break;
    }
    case ELLIPSE_MODE_ROTATION: {
        if (isnan(context->x1)) {
            error("ELLIPSE", translate("This should never happen."));
        }
        else if (isnan(context->x2)) {
            error("ELLIPSE", translate("This should never happen."));
        }
        else if (isnan(context->x3)) {
            var angle = calculateAngle(context->cx, context->cy, x, y);
            context->height = Math.cos(angle*Math.PI/180.0)*context->width;
            addEllipse(context->cx, context->cy, context->width, context->height, context->rot, false);
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
    case ELLIPSE_MODE_MAJDIA_MINRAD_ONE: {
        if (str == "C" || str == "CENTER") {
            context->mode = ELLIPSE_MODE_MAJRAD_MINRAD;
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
            obj_set_rubber_point(obj, "ELLIPSE_LINE_POINT1", context->x1, context->y1);
            prompt_output(translate("Specify first axis end point: "));
            mode = ELLIPSE_MODE_MAJDIA_MINRAD_TWO;
        }
        break;
    }
    case ELLIPSE_MODE_MAJDIA_MINRAD_TWO: {
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
            mode = ELLIPSE_MODE_MAJDIA_MINRAD_THREE;
        }
        break;
    }
    case ELLIPSE_MODE_MAJDIA_MINRAD_THREE: {
        if (str == "R" || str == "ROTATION") {
            context->mode = ELLIPSE_MODE_ROTATION;
            prompt_output(translate("Specify rotation: "));
            break;
        }

        if (parse_vector(str, &v)) {
            alert(translate("Point or option keyword required."));
            prompt_output(translate("Specify second axis end point or [Rotation]: "));
        }
        else {
            context->point3 = v;
            context->height = perpendicularDistance(context->x3, context->y3, context->x1, context->y1, context->x2, context->y2)*2.0;
            obj_set_rubber_point(obj, "ELLIPSE_AXIS2_POINT2", context->x3, context->y3);
            vulcanize();
            end_command();
            context->mode = ELLIPSE_MODE_NEUTRAL;
        }
        break;
    }
    case ELLIPSE_MODE_MAJRAD_MINRAD_ONE: {
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
    case ELLIPSE_MODE_MAJRAD_MINRAD_TWO: {
        if (parse_vector(str, &v)) {
            alert(translate("Invalid point."));
            prompt_output(translate("Specify first axis end point: "));
        }
        else {
            context->point2 = v;
            context->width = calculateDistance(context->x1, context->y1, context->x2, context->y2)*2.0;
            context->rot = calculateAngle(context->x1, context->y1, context->x2, context->y2);
            obj_set_rubber_mode(obj, "ELLIPSE_MAJRAD_MINRAD");
            obj_set_rubber_point(obj, "ELLIPSE_AXIS1_POINT2", context->x2, context->y2);
            obj_set_rubber_point(obj, "ELLIPSE_WIDTH", context->width, 0);
            obj_set_rubber_point(obj, "ELLIPSE_ROT", context->rot, 0);
            prompt_output(translate("Specify second axis end point or [Rotation]: "));
        }
        break;
    }
    case ELLIPSE_MODE_MAJRAD_MINRAD_THREE: {
        if (str == "R" || str == "ROTATION") {
            context->mode = ELLIPSE_MODE_ROTATION;
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
    case ELLIPSE_MODE_ROTATION: {
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

void
Object::setObjectSize(double width, double height)
{
    QRectF elRect = obj_rect(obj);
    elRect.setWidth(width);
    elRect.setHeight(height);
    elRect.moveCenter(QPointF(0, 0));
    // FIXME: obj->setRect(elRect);
}

/* LINE */
ScriptValue
line_command(ScriptEnv *context)
{
    switch (context->context) {
    case CONTEXT_MAIN: {
        /*
        context->firstRun = true;
        context->first = zero_vector;
        context->prev = zero_vector;
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
        context->first = zero_vector;
        context->prev = zero_vector;
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
Object::Object(double x, double y, QRgb rgb, QGraphicsItem* parent) : BaseObject(parent)
{
    debug_message("PointObject Constructor()");
    init(x, y, rgb, Qt::SolidLine);
    todo("getCurrentLineType");
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
    context->center = zero_vector;
    context->side1 = zero_vector;
    context->side2 = zero_vector;
    context->pointI = zero_vector;
    context->pointC = zero_vector;
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
            obj_set_rubber_point(obj, "POLYGON_CENTER", context->centerX, context->centerY);
            obj_set_rubber_point(obj, "POLYGON_NUM_SIDES", context->numSides, 0);
        }
        else if (str == "C" || str == "CIRCUMSCRIBED") {
            context->mode = POLYGON_MODE_CIRCUMSCRIBE;
            context->polyType = "Circumscribed";
            prompt_output(translate("Specify polygon side point or [Distance]: "));
            obj_add_rubber(obj, "POLYGON");
            obj_set_rubber_mode(obj, "POLYGON_CIRCUMSCRIBE");
            obj_set_rubber_point(obj, "POLYGON_CENTER", context->centerX, context->centerY);
            obj_set_rubber_point(obj, "POLYGON_NUM_SIDES", context->numSides, 0);
        }
        else if (str == "") {
            if (context->polyType == "Inscribed") {
                context->mode = POLYGON_MODE_INSCRIBE;
                prompt_output(translate("Specify polygon corner point or [Distance]: "));
                obj_add_rubber(obj, "POLYGON");
                obj_set_rubber_mode(obj, "POLYGON_INSCRIBE");
                obj_set_rubber_point(obj, "POLYGON_CENTER", context->centerX, context->centerY);
                obj_set_rubber_point(obj, "POLYGON_NUM_SIDES", context->numSides, 0);
            }
            else if (context->polyType == "Circumscribed") {
                context->mode = POLYGON_MODE_CIRCUMSCRIBE;
                prompt_output(translate("Specify polygon side point or [Distance]: "));
                obj_add_rubber(obj, "POLYGON");
                obj_set_rubber_mode(obj, "POLYGON_CIRCUMSCRIBE");
                obj_set_rubber_point(obj, "POLYGON_CENTER", context->centerX, context->centerY);
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
                obj_set_rubber_point(obj, "POLYGON_INSCRIBE_POINT", context->pointIX, context->pointIY);
                vulcanize();
                end_command();
            }
            else if (context->polyType == "Circumscribed") {
                context->pointC.x = context->center.x;
                context->pointC.y = context->center.y + Number(str);
                obj_set_rubber_point(obj, "POLYGON_CIRCUMSCRIBE_POINT", context->pointCX, context->pointCY);
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

void
obj_draw_rubber_grip(Object *obj, QPainter *painter)
{
    QPointF point = to_qpointf(obj_map_rubber(obj, ""))
        - to_qpointf(obj_map_rubber(obj, "GRIP_POINT"));
    painter->drawLine(obj_line(obj).translated(point));
}

/* . */
void
obj_update_rubber_grip(Object *obj, QPainter *painter)
{
    if (!painter) {
        return;
    }
    EmbVector gripPoint = obj_rubber_point(obj, "GRIP_POINT");
    switch (obj->geometry->type) {
    case EMB_ARC: {
        todo("rubber grip arc");
        break;
    }
    case EMB_ELLIPSE: {
        todo("rubber grip ellipse");
        break;
    }
    case EMB_IMAGE: {
        todo("rubber grip image");
        break;
    }
    case EMB_PATH: {
        todo("rubber grip path");
        break;
    }
    case EMB_LINE: {
        QPointF p = to_qpointf(obj_map_rubber(obj, ""));
        if (emb_approx(gripPoint, obj_end_point_1(obj))) {
            painter->drawLine(obj_line(obj).p2(), p);
        }
        else if (emb_approx(gripPoint, obj_end_point_2(obj))) {
            painter->drawLine(obj_line(obj).p1(), p);
        }
        else if (emb_approx(gripPoint, obj_mid_point(obj))) {
            QPointF point = p - to_qpointf(obj_map_rubber(obj, "GRIP_POINT"));
            QLineF line = obj_line(obj).translated(point);
            painter->drawLine(line);
        }

        obj_draw_rubber_grip(obj, painter);
        break;
    }
    case EMB_CIRCLE: {
        if (emb_approx(gripPoint, obj_center(obj))) {
            QPointF point = to_qpointf(obj_map_rubber(obj, ""))
                - to_qpointf(obj_map_rubber(obj, "GRIP_POINT"));
            painter->drawEllipse(obj_rect(obj).translated(point));
        }
        else {
            double gripRadius = emb_vector_distance(obj_center(obj), obj_rubber_point(obj, ""));
            painter->drawEllipse(QPointF(), gripRadius, gripRadius);
        }

        obj_draw_rubber_grip(obj, painter);
        break;
    }
    case EMB_DIM_LEADER: {
        QPointF p = to_qpointf(obj_map_rubber(obj, ""));
        if (emb_approx(gripPoint, obj_end_point_1(obj))) {
            painter->drawLine(obj_line(obj).p2(), p);
        }
        else if (emb_approx(gripPoint, obj_end_point_2(obj))) {
            painter->drawLine(obj_line(obj).p1(), p);
        }
        else if (emb_approx(gripPoint, obj_mid_point(obj))) {
            obj_draw_rubber_grip(obj, painter);
        }
        break;
    }
    case EMB_POINT: {
        if (emb_approx(gripPoint, obj_pos(obj))) {
            obj_draw_rubber_grip(obj, painter);
        }
        break;
    }
    case EMB_POLYGON: {
        int elemCount = obj->data.normalPath.elementCount();
        EmbVector gripPoint = obj_rubber_point(obj, "GRIP_POINT");
        if (obj->data.gripIndex == -1) {
            obj->data.gripIndex = obj_find_index(obj, gripPoint);
                if (obj->data.gripIndex == -1) {
                    return;
                }
            }

            int m = 0;
            int n = 0;

            if (!obj->data.gripIndex) {
                m = elemCount - 1;
                n = 1;
            }
            else if (obj->data.gripIndex == elemCount-1) {
                m = elemCount - 2;
                n = 0;
            }
            else {
                m = obj->data.gripIndex - 1;
                n = obj->data.gripIndex + 1;
            }

            QPointF p = to_qpointf(obj_map_rubber(obj, ""));
            QPainterPath::Element em = obj->data.normalPath.elementAt(m);
            QPainterPath::Element en = obj->data.normalPath.elementAt(n);
            painter->drawLine(QPointF(em.x, em.y), p);
            painter->drawLine(QPointF(en.x, en.y), p);

            obj_draw_rubber_grip(obj, painter);
            break;
        }
        case EMB_POLYLINE: {
            int elemCount = obj->data.normalPath.elementCount();
            EmbVector gripPoint = obj_rubber_point(obj, "GRIP_POINT");
            if (obj->data.gripIndex == -1) {
                obj->data.gripIndex = obj_find_index(obj, gripPoint);
            }
            if (obj->data.gripIndex == -1) {
                return;
            }

            QPointF p = to_qpointf(obj_map_rubber(obj, ""));
            if (!obj->data.gripIndex) {
                /* First */
                QPainterPath::Element ef = obj->data.normalPath.elementAt(1);
                painter->drawLine(QPointF(ef.x, ef.y), p);
            }
            else if (obj->data.gripIndex == elemCount-1) {
                /* Last */
                QPainterPath::Element el = obj->data.normalPath.elementAt(obj->data.gripIndex-1);
                painter->drawLine(QPointF(el.x, el.y), p);
            }
            else {
                /* Middle */
                QPainterPath::Element em = obj->data.normalPath.elementAt(obj->data.gripIndex-1);
                QPainterPath::Element en = obj->data.normalPath.elementAt(obj->data.gripIndex+1);
                painter->drawLine(QPointF(em.x, em.y), p);
                painter->drawLine(QPointF(en.x, en.y), p);
            }

        obj_draw_rubber_grip(obj, painter);
        break;
    }
    case EMB_RECT: {
        todo("Make this work with rotation & scaling.");
        /*
        EmbVector after = obj_rubber_point(obj, "");
        EmbVector delta = after-gripPoint;
        if (emb_approx(gripPoint, obj_top_left(obj))) {
            painter->drawPolygon(obj->mapFromScene(QRectF(after.x, after.y,
                objectWidth()-delta.x, objectHeight()-delta.y)));
        }
        else if (emb_approx(gripPoint, obj_top_right(obj))) {
            painter->drawPolygon(obj->mapFromScene(QRectF(objectTopLeft().x,
                objectTopLeft().y+delta.y,
                objectWidth()+delta.x,
                objectHeight()-delta.y)));
        }
        else if (emb_approx(gripPoint, obj_bottom_left(obj))) {
            painter->drawPolygon(obj->mapFromScene(QRectF(objectTopLeft().x+delta.x,
                objectTopLeft().y,
                objectWidth()-delta.x,
                objectHeight()+delta.y)));
        }
        else if (emb_approx(gripPoint, obj_bottom_right(obj))) {
            painter->drawPolygon(obj->mapFromScene(QRectF(objectTopLeft().x,
                objectTopLeft().y,
                objectWidth() + delta.x,
                objectHeight() + delta.y)));
        }
        */

        obj_draw_rubber_grip(obj, painter);
        break;
    }
    case EMB_TEXT_SINGLE: {
        if (emb_approx(gripPoint, obj_pos(obj))) {
           // painter->drawPath(obj_path(obj).translated(obj->mapFromScene(obj_rubber_point(obj, ""))- obj_map_rubber(obj, "GRIP_POINT"));
        }

        obj_draw_rubber_grip(obj, painter);
        break;
    }
    default:
        break;
    }
}

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
    context->first = zero_vector;
    context->prev = zero_vector;
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
    context->point1 = zero_vector;
    context->point2 = zero_vector;
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

    TextSingleObject* obj = static_cast<TextSingleObject*>(item);
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
    context->position = zero_vector;
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
            context->textX = x;
            context->textY = y;
            obj_add_rubber(obj, "LINE");
            obj_set_rubber_mode(obj, "LINE");
            obj_set_rubber_point(obj, "LINE_START", context->textX, context->textY);
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
            context->textJustify = "Center";
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
        setTextFont(context->textFont);
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
                var strList = str.split(",");
                if (isnan(strList[0]) || isnan(strList[1])) {
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
    double s = obj->scale();
    QTransform trans;
    trans.rotate(rotation());
    trans.scale(s,s);

    QList<QPainterPath> pathList;

    QPainterPath path = objTextPath;

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

/* FIXME: */
QList<QPainterPath>
Object::subPathList() const {
    QList<QPainterPath> p;
    return p;
}

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
    context->point1 = zero_vector;
    context->point2 = zero_vector;
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
obj_start_point(Object *obj)
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
obj_mid_point(Object *obj)
{
    switch (obj->geometry->type) {
    case EMB_ARC: {
        return obj->geometry->object.arc.mid;
    }
    case EMB_LINE:
    case EMB_DIM_LEADER: {
        QLineF lyne = obj_line(obj);
        EmbVector mp = to_emb_vector(lyne.pointAt(0.5));
        return obj_pos(obj) + scale_and_rotate(mp, obj->scale(), obj->rotation());
    }
    default:
        break;
    }
    return obj_pos(obj);
}

/* . */
EmbVector
obj_end_point(Object *obj)
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
obj_top_left(Object *obj)
{
    EmbVector p = to_emb_vector(obj_rect(obj).topLeft());
    return obj_pos(obj) + scale_and_rotate(p, obj->scale(), obj->rotation());
}

/* . */
EmbVector
obj_top_right(Object *obj)
{
    EmbVector p = to_emb_vector(obj_rect(obj).topRight());
    return obj_pos(obj) + scale_and_rotate(p, obj->scale(), obj->rotation());
}

/* . */
EmbVector
obj_bottom_left(Object *obj)
{
    EmbVector p = to_emb_vector(obj_rect(obj).bottomLeft());
    return obj_pos(obj) + scale_and_rotate(p, obj->scale(), obj->rotation());
}

/* . */
EmbVector
obj_bottom_right(Object *obj)
{
    EmbVector p = to_emb_vector(obj_rect(obj).bottomRight());
    return obj_pos(obj) + scale_and_rotate(p, obj->scale(), obj->rotation());
}

/* . */
EmbVector
obj_center(Object *obj)
{
    return obj_pos(obj);
}

/* TODO: << "Aligned" << "Fit" */
QStringList objectTextJustifyList = {
    "Left",
    "Center",
    "Right",
    "Middle",
    "Top Left",
    "Top Center",
    "Top Right",
    "Middle Left",
    "Middle Center",
    "Middle Right",
    "Bottom Left",
    "Bottom Center",
    "Bottom Right"
};

/* . */
void
obj_set_text(Object* obj, QString str)
{
    obj->data.objText = str;
    QPainterPath textPath;
    QFont font;
    font.setFamily(obj->data.objTextFont);
    font.setPointSizeF(obj->data.objTextSize);
    font.setBold(obj->data.objTextBold);
    font.setItalic(obj->data.objTextItalic);
    font.setUnderline(obj->data.objTextUnderline);
    font.setStrikeOut(obj->data.objTextStrikeOut);
    font.setOverline(obj->data.objTextOverline);
    textPath.addText(0, 0, font, str);

    /* Translate the path based on the justification. */
    QRectF jRect = textPath.boundingRect();
    if (obj->data.objTextJustify == "Left") {
        textPath.translate(-jRect.left(), 0);
    }
    else if (obj->data.objTextJustify == "Center") {
        textPath.translate(-jRect.center().x(), 0);
    }
    else if (obj->data.objTextJustify == "Right") {
        textPath.translate(-jRect.right(), 0);
    }
    else if (obj->data.objTextJustify == "Aligned") {
        todo("TextSingleObject Aligned Justification.");
    }
    else if (obj->data.objTextJustify == "Middle") {
        textPath.translate(-jRect.center());
    }
    else if (obj->data.objTextJustify == "Fit") {
        todo("TextSingleObject Fit Justification.");
    }
    else if (obj->data.objTextJustify == "Top Left") {
        textPath.translate(-jRect.topLeft());
    }
    else if (obj->data.objTextJustify == "Top Center") {
        textPath.translate(-jRect.center().x(), -jRect.top());
    }
    else if (obj->data.objTextJustify == "Top Right") {
        textPath.translate(-jRect.topRight());
    }
    else if (obj->data.objTextJustify == "Middle Left") {
        textPath.translate(-jRect.left(), -jRect.top()/2.0);
    }
    else if (obj->data.objTextJustify == "Middle Center") {
        textPath.translate(-jRect.center().x(), -jRect.top()/2.0);
    }
    else if (obj->data.objTextJustify == "Middle Right") {
        textPath.translate(-jRect.right(), -jRect.top()/2.0);
    }
    else if (obj->data.objTextJustify == "Bottom Left") {
        textPath.translate(-jRect.bottomLeft());
    }
    else if (obj->data.objTextJustify == "Bottom Center") {
        textPath.translate(-jRect.center().x(), -jRect.bottom());
    }
    else if (obj->data.objTextJustify == "Bottom Right") {
        textPath.translate(-jRect.bottomRight());
    }

    /* Backward or Upside Down. */
    if (obj->data.objTextBackward || obj->data.objTextUpsideDown) {
        double horiz = 1.0;
        double vert = 1.0;
        if (obj->data.objTextBackward) {
            horiz = -1.0;
        }
        if (obj->data.objTextUpsideDown) {
            vert = -1.0;
        }

        QPainterPath flippedPath;

        QPainterPath::Element element;
        QPainterPath::Element P2;
        QPainterPath::Element P3;
        QPainterPath::Element P4;
        for (int i = 0; i < textPath.elementCount(); ++i) {
            element = textPath.elementAt(i);
            if (element.isMoveTo()) {
                flippedPath.moveTo(horiz * element.x, vert * element.y);
            }
            else if (element.isLineTo()) {
                flippedPath.lineTo(horiz * element.x, vert * element.y);
            }
            else if (element.isCurveTo()) {
                                              /* start point P1 is not needed. */
                P2 = textPath.elementAt(i);   /* control point. */
                P3 = textPath.elementAt(i+1); /* control point. */
                P4 = textPath.elementAt(i+2); /* end point. */

                flippedPath.cubicTo(horiz * P2.x, vert * P2.y,
                                    horiz * P3.x, vert * P3.y,
                                    horiz * P4.x, vert * P4.y);
            }
        }
        obj->data.objTextPath = flippedPath;
    }
    else {
        obj->data.objTextPath = textPath;
    }

    /* Add the grip point to the shape path. */
    QPainterPath gripPath = obj->data.objTextPath;
    gripPath.connectPath(obj->data.objTextPath);
    gripPath.addRect(-0.00000001, -0.00000001, 0.00000002, 0.00000002);
    obj_set_path(obj, gripPath);
}

/* . */
void
obj_set_text_font(Object* obj, QString font)
{
    obj->data.objTextFont = font;
    obj_set_text(obj, obj->data.objText);
}

/* Verify the string is a valid option. */
void
obj_set_text_justify(Object* obj, QString justify)
{
    if (justify == "Left") {
        obj->data.objTextJustify = justify;
    }
    else if (justify == "Center") {
        obj->data.objTextJustify = justify;
    }
    else if (justify == "Right") {
        obj->data.objTextJustify = justify;
    }
    else if (justify == "Aligned") {
        obj->data.objTextJustify = justify;
    }
    else if (justify == "Middle") {
        obj->data.objTextJustify = justify;
    }
    else if (justify == "Fit") {
        obj->data.objTextJustify = justify;
    }
    else if (justify == "Top Left") {
        obj->data.objTextJustify = justify;
    }
    else if (justify == "Top Center") {
        obj->data.objTextJustify = justify;
    }
    else if (justify == "Top Right") {
        obj->data.objTextJustify = justify;
    }
    else if (justify == "Middle Left") {
        obj->data.objTextJustify = justify;
    }
    else if (justify == "Middle Center") {
        obj->data.objTextJustify = justify;
    }
    else if (justify == "Middle Right") {
        obj->data.objTextJustify = justify;
    }
    else if (justify == "Bottom Left") {
        obj->data.objTextJustify = justify;
    }
    else if (justify == "Bottom Center") {
        obj->data.objTextJustify = justify;
    }
    else if (justify == "Bottom Right") {
        obj->data.objTextJustify = justify;
    }
    else {
        /* Default */
        obj->data.objTextJustify = "Left";
    }
    obj_set_text(obj, obj->data.objText);
}

/* . */
void
obj_set_text_size(Object* obj, double size)
{
    obj->data.objTextSize = size;
    obj_set_text(obj, obj->data.objText);
}

/* . */
void
obj_set_text_style(Object* obj, bool bold, bool italic, bool under, bool strike, bool over)
{
    obj->data.objTextBold = bold;
    obj->data.objTextItalic = italic;
    obj->data.objTextUnderline = under;
    obj->data.objTextStrikeOut = strike;
    obj->data.objTextOverline = over;
    obj_set_text(obj, obj->data.objText);
}

/* . */
void
obj_set_text_bold(Object* obj, bool val)
{
    obj->data.objTextBold = val;
    obj_set_text(obj, obj->data.objText);
}

/* . */
void
obj_set_text_italic(Object* obj, bool val)
{
    obj->data.objTextItalic = val;
    obj_set_text(obj, obj->data.objText);
}

/* . */
void
obj_set_text_underline(Object* obj, bool val)
{
    obj->data.objTextUnderline = val;
    obj_set_text(obj, obj->data.objText);
}

/* . */
void
obj_set_text_strikeout(Object* obj, bool val)
{
    obj->data.objTextStrikeOut = val;
    obj_set_text(obj, obj->data.objText);
}

/* . */
void
obj_set_text_overline(Object* obj, bool val)
{
    obj->data.objTextOverline = val;
    obj_set_text(obj, obj->data.objText);
}

/* . */
void
obj_set_text_backward(Object* obj, bool val)
{
    obj->data.objTextBackward = val;
    obj_set_text(obj, obj->data.objText);
}

/* . */
void
obj_set_text_upside_down(Object* obj, bool val)
{
    obj->data.objTextUpsideDown = val;
    obj_set_text(obj, obj->data.objText);
}

/* . */
EmbVector
obj_end_point_1(Object *obj)
{
    return obj_pos(obj);
}

/* . */
void
obj_set_end_point_1(Object *obj, EmbVector endPt1)
{
    EmbVector endPt2 = obj_end_point_2(obj);
    EmbVector delta = endPt2 - endPt1;
    obj->setRotation(0);
    obj->setScale(1);
    obj_set_line(obj, 0, 0, delta.x, delta.y);
    obj->setPos(endPt1.x, endPt1.y);
}

/* . */
void
obj_update_path(Object *obj)
{
    switch (obj->geometry->type) {
    case EMB_ARC: {
        double startAngle = (emb_start_angle(obj->geometry) + obj->rotation());
        double spanAngle = emb_included_angle(obj->geometry);

        if (emb_clockwise(obj->geometry)) {
            spanAngle = -spanAngle;
        }

        QPainterPath path;
        path.arcMoveTo(obj_rect(obj), startAngle);
        path.arcTo(obj_rect(obj), startAngle, spanAngle);
        /* Reverse the path so that the inside area isn't considered part of the arc. */
        path.arcTo(obj_rect(obj), startAngle+spanAngle, -spanAngle);
        obj_set_path(obj, path);
        break;
    }
    case EMB_CIRCLE: {
        QPainterPath path;
        QRectF r = obj_rect(obj);
        /* Add the center point. */
        path.addRect(-0.00000001, -0.00000001, 0.00000002, 0.00000002);
        /* Add the circle */
        path.arcMoveTo(r, 0);
        path.arcTo(r, 0, 360);
        /* Reverse the path so that the inside area isn't considered part of the circle. */
        path.arcTo(r, 0, -360);
        obj_set_path(obj, path);
        break;
    }
    case EMB_ELLIPSE: {
        QPainterPath path;
        QRectF r = obj_rect(obj);
        path.arcMoveTo(r, 0);
        path.arcTo(r, 0, 360);
        /* Reverse the path so that the inside area isn't considered part of the ellipse. */
        path.arcTo(r, 0, -360);
        obj_set_path(obj, path);
        break;
    }
    case EMB_RECT:
    case EMB_IMAGE: {
        QPainterPath path;
        QRectF r = obj_rect(obj);
        path.moveTo(r.bottomLeft());
        path.lineTo(r.bottomRight());
        path.lineTo(r.topRight());
        path.lineTo(r.topLeft());
        path.lineTo(r.bottomLeft());
        /* NOTE: Reverse the path so that the inside area isn't considered part of the rectangle. */
        path.lineTo(r.topLeft());
        path.lineTo(r.topRight());
        path.lineTo(r.bottomRight());
        path.moveTo(r.bottomLeft());
        obj_set_path(obj, path);
        break;
    }
    default:
        break;
    }
}

/* . */
void
obj_calculate_data(Object *obj)
{
    EmbVector center = emb_arc_center(*(obj->geometry));

    double radius = emb_vector_distance(center, obj->geometry->object.arc.mid);
    obj_update_arc_rect(obj, radius);
    obj_update_path(obj);
    obj->setRotation(0);
    obj->setScale(1);
}

/* . */
void
obj_update_arc_rect(Object *obj, double radius)
{
    QRectF arcRect;
    arcRect.setWidth(radius*2.0);
    arcRect.setHeight(radius*2.0);
    arcRect.moveCenter(QPointF(0, 0));
    obj_set_rect(obj, arcRect);
}

/* . */
void
obj_set_line_weight(Object *obj, double lineWeight)
{
    obj->data.objPen.setWidthF(0); /* NOTE: The objPen will always be cosmetic. */

    if (lineWeight < 0) {
        if (lineWeight == OBJ_LWT_BYLAYER) {
            obj->data.lwtPen.setWidthF(0.35);
            todo("getLayerLineWeight");
        }
        else if (lineWeight == OBJ_LWT_BYBLOCK) {
            obj->data.lwtPen.setWidthF(0.35);
            todo("getBlockLineWeight");
        }
        else {
            char msg[MAX_STRING_LENGTH];
            sprintf(msg, "Lineweight: %f", lineWeight);
            warning_box(translate("Error - Negative Lineweight"), msg);
            debug_message("Lineweight cannot be negative! Inverting sign.");
            obj->data.lwtPen.setWidthF(-lineWeight);
        }
    }
    else {
        obj->data.lwtPen.setWidthF(lineWeight);
    }
}

/* . */
void
obj_update_path_r(Object *obj, QPainterPath p)
{
    switch (obj->geometry->type) {
    case EMB_POLYGON: {
        obj->data.normalPath = p;
        QPainterPath closedPath = obj->data.normalPath;
        closedPath.closeSubpath();
        QPainterPath reversePath = closedPath.toReversed();
        reversePath.connectPath(closedPath);
        obj_set_path(obj, reversePath);
        break;
    }
    case EMB_PATH:
    case EMB_POLYLINE: {
        obj->data.normalPath = p;
        QPainterPath reversePath = obj->data.normalPath.toReversed();
        reversePath.connectPath(obj->data.normalPath);
        obj_set_path(obj, reversePath);
        break;
    }
    default:
        break;
    }
}

/* . */
void
obj_set_color(Object *obj, const QColor& color)
{
    obj->data.objPen.setColor(color);
    obj->data.lwtPen.setColor(color);
}

/* . */
void
obj_set_color_rgb(Object *obj, QRgb rgb)
{
    obj->data.objPen.setColor(QColor(rgb));
    obj->data.lwtPen.setColor(QColor(rgb));
}

/* . */
void
obj_set_line_type(Object *obj, Qt::PenStyle lineType)
{
    obj->data.objPen.setStyle(lineType);
    obj->data.lwtPen.setStyle(lineType);
}

/* . */
EmbVector
obj_rubber_point(Object *obj, QString key)
{
    if (obj->data.objRubberPoints.contains(key)) {
        return obj->data.objRubberPoints.value(key);
    }

    QGraphicsScene* gscene = obj->scene();
    if (gscene) {
        QPointF p = obj->scene()->property("SCENE_QSNAP_POINT").toPointF();
        return to_emb_vector(p);
    }
    EmbVector v;
    v.x = 0.0;
    v.y = 0.0;
    return v;
}

/* . */
QString
obj_rubber_text(Object *obj, QString key)
{
    if (obj->data.objRubberTexts.contains(key)) {
        return obj->data.objRubberTexts.value(key);
    }
    return "";
}

/* . */
QRectF
obj_bounding_rect(Object *obj)
{
    /* If gripped, force this object to be drawn even if it is offscreen. */
    if (obj->data.objRubberMode == OBJ_RUBBER_GRIP) {
        return obj->scene()->sceneRect();
    }
    return obj->path().boundingRect();
}

/* . */
EmbVector
obj_pos(Object *obj)
{
    return obj_pos(obj);
}

/* . */
double
obj_x(Object *obj)
{
    return obj_pos(obj).x;
}

/* . */
double
obj_y(Object *obj)
{
    return obj_pos(obj).y;
}

/* . */
double
obj_center_x(Object *obj)
{
    return obj_pos(obj).x;
}

/* . */
double
obj_center_y(Object *obj)
{
    return obj_pos(obj).y;
}

/* . */
double
obj_radius(Object *obj)
{
    return obj_rect(obj).width()/2.0 * obj->scale();
}

/* . */
double
obj_diameter(Object *obj)
{
    return obj_rect(obj).width() * obj->scale();
}

/* . */
double
obj_circumference(Object *obj)
{
    return embConstantPi * obj_diameter(obj);
}

/* . */
EmbVector
obj_delta(Object *obj)
{
    return obj_end_point_2(obj) - obj_end_point_1(obj);
}

/* . */
void
obj_set_center(Object *obj, EmbVector center)
{
    obj->setPos(center.x, center.y);
}

/* . */
void
obj_set_center_x(Object *obj, double centerX)
{
    obj->setX(centerX);
}

/* . */
void
obj_set_center_y(Object *obj, double centerY)
{
    obj->setY(centerY);
}

/* . */
double
obj_length(Object *obj)
{
    return obj_line(obj).length() * obj->scale();
}

/* . */
void
obj_set_x1(Object *obj, double x)
{
    EmbVector v = obj_end_point_1(obj);
    v.x = x;
    obj_set_end_point_1(obj, v);
}

/* . */
void
obj_set_y1(Object *obj, double y)
{
    EmbVector v = obj_end_point_1(obj);
    v.y = y;
    obj_set_end_point_1(obj, v);
}

/* . */
void
obj_set_x2(Object *obj, double x)
{
    EmbVector v = obj_end_point_2(obj);
    v.x = x;
    obj_set_end_point_2(obj, v);
}

/* . */
void
obj_set_y2(Object *obj, double y)
{
    EmbVector v = obj_end_point_2(obj);
    v.y = y;
    obj_set_end_point_2(obj, v);
}

/* . */
QRectF
obj_rect(Object *obj)
{
    return obj_bounding_rect(obj);
}

/* . */
void
obj_set_rect(Object *obj, QRectF r)
{
    QPainterPath p;
    p.addRect(r);
    obj->setPath(p);
}

/* . */
void
obj_set_rect(Object *obj, double x, double y, double w, double h)
{
    // obj->setPos(x, y); ?
    QPainterPath p;
    p.addRect(x,y,w,h);
    obj->setPath(p);
}

/* . */
QLineF
obj_line(Object *obj)
{
    return obj->data.objLine;
}

/* . */
void
obj_set_line(Object *obj, QLineF li)
{
    QPainterPath p;
    p.moveTo(li.p1());
    p.lineTo(li.p2());
    obj->setPath(p);
    obj->data.objLine = li;
}

/* . */
void
obj_set_line(Object *obj, double x1, double y1, double x2, double y2)
{
    QPainterPath p;
    p.moveTo(x1, y1);
    p.lineTo(x2, y2);
    obj->setPath(p);
    obj->data.objLine.setLine(x1, y1, x2, y2);
}

/* . */
void
obj_set_pos(Object *obj, EmbVector point)
{
    obj->setPos(point.x, point.y);
}

/* . */
void
obj_set_pos(Object *obj, double x, double y)
{
    obj->setPos(x, y);
}

/* . */
void
obj_set_x(Object *obj, double x)
{
    obj_set_pos(obj, x, obj_y(obj));
}

/* . */
void
obj_set_y(Object *obj, double y)
{
    obj_set_pos(obj, obj_x(obj), y);
}

/* . */
QPainterPath
obj_shape(Object *obj)
{
    return obj->path();
}

/* . */
void
obj_set_path(Object *obj, QPainterPath p)
{
    obj->setPath(p);
}

/* . */
void
obj_set_rubber_mode(Object *obj, int mode)
{
    obj->data.objRubberMode = mode;
}

/* . */
void
obj_set_rubber_point(Object *obj, QString key, const EmbVector& point)
{
    obj->data.objRubberPoints.insert(key, point);
}

/* . */
void
obj_set_rubber_text(Object *obj, QString key, QString txt)
{
    obj->data.objRubberTexts.insert(key, txt);
}

/* . */
QList<QPainterPath>
obj_save_path_list(Object *obj)
{
    return obj->subPathList();
}

/* . */
int
obj_find_index(Object *obj, EmbVector point)
{
    int elemCount = obj->data.normalPath.elementCount();
    /* NOTE: Points here are in item coordinates */
    EmbVector itemPoint = map_from_scene(obj, point);
    for (int i = 0; i < elemCount; i++) {
        QPainterPath::Element e = obj->data.normalPath.elementAt(i);
        EmbVector elemPoint;
        elemPoint.x = e.x;
        elemPoint.y = e.y;
        if (emb_approx(itemPoint, elemPoint)) {
            return i;
        }
    }
    return -1;
}

/* Map rubber point from scene. */
EmbVector
obj_map_rubber(Object *obj, const char *key)
{
    EmbVector point = obj_rubber_point(obj, key);
    return map_from_scene(obj, point);
}

/* . */
void
obj_update_rubber(Object *obj, QPainter* painter)
{
    todo("Arc,Path Rubber Modes");
    switch (obj->data.objRubberMode) {
    case OBJ_RUBBER_CIRCLE_1P_RAD: {
        EmbVector sceneCenterPoint = obj_rubber_point(obj, "CIRCLE_CENTER");
        EmbVector sceneQSnapPoint = obj_rubber_point(obj, "CIRCLE_RADIUS");
        EmbVector itemCenterPoint = obj_map_rubber(obj, "CIRCLE_CENTER");
        EmbVector itemQSnapPoint = obj_map_rubber(obj, "CIRCLE_RADIUS");
        QLineF itemLine(to_qpointf(itemCenterPoint), to_qpointf(itemQSnapPoint));
        obj_set_center(obj, sceneCenterPoint);
        QLineF sceneLine(to_qpointf(sceneCenterPoint), to_qpointf(sceneQSnapPoint));
        double radius = sceneLine.length();
        emb_set_radius(obj->geometry, radius);
        if (painter) {
            obj_draw_rubber_line(obj, itemLine, painter, "VIEW_COLOR_CROSSHAIR");
        }
        obj_update_path(obj);
        break;
    }
    case OBJ_RUBBER_CIRCLE_1P_DIA: {
        EmbVector sceneCenterPoint = obj_rubber_point(obj, "CIRCLE_CENTER");
        EmbVector sceneQSnapPoint = obj_rubber_point(obj, "CIRCLE_DIAMETER");
        EmbVector itemCenterPoint = obj_map_rubber(obj, "CIRCLE_CENTER");
        EmbVector itemQSnapPoint = obj_map_rubber(obj, "CIRCLE_DIAMETER");
        QLineF itemLine(to_qpointf(itemCenterPoint), to_qpointf(itemQSnapPoint));
        obj_set_center(obj, sceneCenterPoint);
        QLineF sceneLine(to_qpointf(sceneCenterPoint), to_qpointf(sceneQSnapPoint));
        double diameter = sceneLine.length();
        emb_set_diameter(obj->geometry, diameter);
        if (painter) {
            obj_draw_rubber_line(obj, itemLine, painter, "VIEW_COLOR_CROSSHAIR");
        }
        obj_update_path(obj);
        break;
    }
    case OBJ_RUBBER_CIRCLE_2P: {
        EmbVector sceneTan1Point = obj_rubber_point(obj, "CIRCLE_TAN1");
        EmbVector sceneQSnapPoint = obj_rubber_point(obj, "CIRCLE_TAN2");
        QLineF sceneLine(to_qpointf(sceneTan1Point), to_qpointf(sceneQSnapPoint));
        obj_set_center(obj, to_emb_vector(sceneLine.pointAt(0.5)));
        double diameter = sceneLine.length();
        emb_set_diameter(obj->geometry, diameter);
        obj_update_path(obj);
        break;
    }
    case OBJ_RUBBER_CIRCLE_3P: {
        EmbGeometry g;
        g.object.arc.start = obj_rubber_point(obj, "CIRCLE_TAN1");
        g.object.arc.mid = obj_rubber_point(obj, "CIRCLE_TAN2");
        g.object.arc.end = obj_rubber_point(obj, "CIRCLE_TAN3");
        g.type = EMB_ARC;
        EmbVector center = emb_arc_center(g);
        obj_set_center(obj, center);
        double radius = emb_vector_distance(center, g.object.arc.end);
        emb_set_radius(obj->geometry, radius);
        obj_update_path(obj);
        break;
    }
    case OBJ_RUBBER_DIMLEADER_LINE: {
        EmbVector sceneStartPoint = obj_rubber_point(obj, "DIMLEADER_LINE_START");
        EmbVector sceneQSnapPoint = obj_rubber_point(obj, "DIMLEADER_LINE_END");

        obj_set_end_point_1(obj, sceneStartPoint);
        obj_set_end_point_2(obj, sceneQSnapPoint);
        break;
    }
    case OBJ_RUBBER_ELLIPSE_LINE: {
        EmbVector itemLinePoint1  = obj_map_rubber(obj, "ELLIPSE_LINE_POINT1");
        EmbVector itemLinePoint2  = obj_map_rubber(obj, "ELLIPSE_LINE_POINT2");
        QLineF itemLine(to_qpointf(itemLinePoint1), to_qpointf(itemLinePoint2));
        if (painter) {
            obj_draw_rubber_line(obj, itemLine, painter, "VIEW_COLOR_CROSSHAIR");
        }
        obj_update_path(obj);
        break;
    }
    case OBJ_RUBBER_ELLIPSE_MAJORDIAMETER_MINORRADIUS: {
        EmbVector sceneAxis1Point1 = obj_rubber_point(obj, "ELLIPSE_AXIS1_POINT1");
        EmbVector sceneAxis1Point2 = obj_rubber_point(obj, "ELLIPSE_AXIS1_POINT2");
        EmbVector sceneCenterPoint = obj_rubber_point(obj, "ELLIPSE_CENTER");
        EmbVector sceneAxis2Point2 = obj_rubber_point(obj, "ELLIPSE_AXIS2_POINT2");
        double ellipseWidth = obj_rubber_point(obj, "ELLIPSE_WIDTH").x;
        double ellipseRot = obj_rubber_point(obj, "ELLIPSE_ROT").x;

        todo("incorporate perpendicularDistance() into libembroidery.");
        QLineF line(to_qpointf(sceneAxis1Point1), to_qpointf(sceneAxis1Point2));
        QLineF norm = line.normalVector();
        EmbVector delta = sceneAxis2Point2 - sceneAxis1Point1;
        norm.translate(delta.x, delta.y);
        QPointF iPoint;
        norm.intersects(line, &iPoint);
        double ellipseHeight = emb_vector_distance(sceneAxis2Point2, to_emb_vector(iPoint))*2.0;

        obj_set_center(obj, sceneCenterPoint);
        // FIXME: obj->setSize(ellipseWidth, ellipseHeight);
        obj->setRotation(-ellipseRot);

        EmbVector itemCenterPoint = obj_map_rubber(obj, "ELLIPSE_CENTER");
        EmbVector itemAxis2Point2 = obj_map_rubber(obj, "ELLIPSE_AXIS2_POINT2");
        QLineF itemLine(to_qpointf(itemCenterPoint), to_qpointf(itemAxis2Point2));
        if (painter) {
            obj_draw_rubber_line(obj, itemLine, painter, "VIEW_COLOR_CROSSHAIR");
        }
        obj_update_path(obj);
        break;
    }
    case OBJ_RUBBER_ELLIPSE_MAJORRADIUS_MINORRADIUS: {
        EmbVector sceneAxis1Point2 = obj_rubber_point(obj, "ELLIPSE_AXIS1_POINT2");
        EmbVector sceneCenterPoint = obj_rubber_point(obj, "ELLIPSE_CENTER");
        EmbVector sceneAxis2Point2 = obj_rubber_point(obj, "ELLIPSE_AXIS2_POINT2");
        double ellipseWidth = obj_rubber_point(obj, "ELLIPSE_WIDTH").x;
        double ellipseRot = obj_rubber_point(obj, "ELLIPSE_ROT").x;

        todo("incorporate perpendicularDistance() into libcgeometry.");
        QLineF line(to_qpointf(sceneCenterPoint), to_qpointf(sceneAxis1Point2));
        QLineF norm = line.normalVector();
        EmbVector delta = sceneAxis2Point2 - sceneCenterPoint;
        norm.translate(delta.x, delta.y);
        QPointF iPoint;
        norm.intersects(line, &iPoint);
        double ellipseHeight = emb_vector_distance(sceneAxis2Point2, to_emb_vector(iPoint)) * 2.0;

        obj_set_center(obj, sceneCenterPoint);
        obj->setObjectSize(ellipseWidth, ellipseHeight);
        obj->setRotation(-ellipseRot);

        EmbVector itemCenterPoint = obj_map_rubber(obj, "ELLISPE_CENTER");
        EmbVector itemAxis2Point2 = obj_map_rubber(obj, "ELLIPSE_AXIS2_POINT2");
        QLineF itemLine(to_qpointf(itemCenterPoint), to_qpointf(itemAxis2Point2));
        if (painter) {
            obj_draw_rubber_line(obj, itemLine, painter, "VIEW_COLOR_CROSSHAIR");
        }
        obj_update_path(obj);
        break;
    }
    case OBJ_RUBBER_IMAGE: {
        EmbVector start = obj_rubber_point(obj, "IMAGE_START");
        EmbVector end = obj_rubber_point(obj, "IMAGE_END");
        EmbVector delta = emb_vector_subtract(end, start);
        obj_set_rect(obj, start.x, start.y, delta.x, delta.y);
        obj_update_path(obj);
        break;
    }
    case OBJ_RUBBER_LINE: {
        EmbVector sceneStartPoint = obj_rubber_point(obj, "LINE_START");
        EmbVector sceneQSnapPoint = obj_rubber_point(obj, "LINE_END");

        obj_set_end_point_1(obj, sceneStartPoint);
        obj_set_end_point_2(obj, sceneQSnapPoint);

        obj_draw_rubber_line(obj, obj_line(obj), painter, "VIEW_COLOR_CROSSHAIR");
        break;
    }
    case OBJ_RUBBER_POLYGON: {
        obj_set_pos(obj, obj_rubber_point(obj, "POLYGON_POINT_0"));

        bool ok = false;
        QString numStr = obj_rubber_text(obj, "POLYGON_NUM_POINTS");
        if (numStr.isNull()) {
            return;
        }
        int num = numStr.toInt(&ok);
        if (!ok) {
            return;
        }

        QPainterPath rubberPath;
        rubberPath.moveTo(to_qpointf(obj_map_rubber(obj, "POLYGON_POINT_0")));
        for (int i = 1; i <= num; i++) {
            char key[MAX_STRING_LENGTH];
            sprintf(key, "POLYGON_POINT_%d", i);
            EmbVector appendPoint = obj_map_rubber(obj, key);
            rubberPath.lineTo(to_qpointf(appendPoint));
        }
        /* rubberPath.lineTo(0,0); */
        obj_update_path_r(obj, rubberPath);

        /* Ensure the path isn't updated until the number of points is changed again. */
        obj_set_rubber_text(obj, "POLYGON_NUM_POINTS", "");
        break;
    }
    case OBJ_RUBBER_POLYGON_INSCRIBE: {
        obj_set_pos(obj, obj_rubber_point(obj, "POLYGON_CENTER"));

        quint16 numSides = obj_rubber_point(obj, "POLYGON_NUM_SIDES").x;

        EmbVector inscribePoint = obj_map_rubber(obj, "POLYGON_INSCRIBE_POINT");
        QLineF inscribeLine = QLineF(QPointF(0, 0), to_qpointf(inscribePoint));
        double inscribeAngle = inscribeLine.angle();
        double inscribeInc = 360.0/numSides;

        if (painter) {
            obj_draw_rubber_line(obj, inscribeLine, painter, "VIEW_COLOR_CROSSHAIR");
        }

        QPainterPath inscribePath;
        /* First Point */
        inscribePath.moveTo(to_qpointf(inscribePoint));
        /* Remaining Points */
        for (int i = 1; i < numSides; i++) {
            inscribeLine.setAngle(inscribeAngle + inscribeInc*i);
            inscribePath.lineTo(inscribeLine.p2());
        }
        obj_update_path_r(obj, inscribePath);
        break;
    }
    case OBJ_RUBBER_POLYGON_CIRCUMSCRIBE: {
        obj_set_pos(obj, obj_rubber_point(obj, "POLYGON_CENTER"));

        quint16 numSides = obj_rubber_point(obj, "POLYGON_NUM_SIDES").x;

        EmbVector circumscribePoint = obj_map_rubber(obj, "POLYGON_CIRCUMSCRIBE_POINT");
        QLineF circumscribeLine = QLineF(QPointF(0, 0), to_qpointf(circumscribePoint));
        double circumscribeAngle = circumscribeLine.angle();
        double circumscribeInc = 360.0/numSides;

        if (painter) {
            obj_draw_rubber_line(obj, circumscribeLine, painter, "VIEW_COLOR_CROSSHAIR");
        }

        QPainterPath circumscribePath;
        /* First Point */
        QLineF prev(circumscribeLine.p2(), QPointF(0,0));
        prev = prev.normalVector();
        circumscribeLine.setAngle(circumscribeAngle + circumscribeInc);
        QLineF perp(circumscribeLine.p2(), QPointF(0,0));
        perp = perp.normalVector();
        QPointF iPoint;
        perp.intersects(prev, &iPoint);
        circumscribePath.moveTo(iPoint);
        /* Remaining Points */
        for (int i = 2; i <= numSides; i++) {
            prev = perp;
            circumscribeLine.setAngle(circumscribeAngle + circumscribeInc*i);
            perp = QLineF(circumscribeLine.p2(), QPointF(0,0));
            perp = perp.normalVector();
            perp.intersects(prev, &iPoint);
            circumscribePath.lineTo(iPoint);
        }
        obj_update_path_r(obj, circumscribePath);
        break;
    }
    case OBJ_RUBBER_POLYLINE: {
        obj_set_pos(obj, obj_rubber_point(obj, "POLYLINE_POINT_0"));

        QPointF p = to_qpointf(obj_map_rubber(obj, ""));
        QLineF rubberLine(obj->data.normalPath.currentPosition(), p);
        if (painter) {
            obj_draw_rubber_line(obj, rubberLine, painter, "VIEW_COLOR_CROSSHAIR");
        }

        bool ok = false;
        QString numStr = obj_rubber_text(obj, "POLYLINE_NUM_POINTS");
        if (numStr.isNull()) return;
        int num = numStr.toInt(&ok);
        if (!ok) {
            return;
        }

        QPainterPath rubberPath;
        for (int i = 1; i <= num; i++) {
            char msg[MAX_STRING_LENGTH];
            sprintf(msg, "POLYLINE_POINT_%d", i);
            EmbVector appendPoint = obj_map_rubber(obj, msg);
            rubberPath.lineTo(to_qpointf(appendPoint));
        }
        obj_update_path_r(obj, rubberPath);

        /* Ensure the path isn't updated until the number of points is changed again. */
        obj_set_rubber_text(obj, "POLYLINE_NUM_POINTS", "");
        break;
    }
    case OBJ_RUBBER_RECTANGLE: {
        EmbVector start = obj_rubber_point(obj, "RECTANGLE_START");
        EmbVector end = obj_rubber_point(obj, "RECTANGLE_END");
        EmbVector delta = emb_vector_subtract(end, start);
        obj_set_rect(obj, start.x, start.y, delta.x, delta.y);
        obj_update_path(obj);
        break;
    }
    case OBJ_RUBBER_TEXTSINGLE: {
        obj_set_text_font(obj, obj_rubber_text(obj, "TEXT_FONT"));
        obj_set_text_justify(obj, obj_rubber_text(obj, "TEXT_JUSTIFY"));
        obj_set_pos(obj, obj_rubber_point(obj, "TEXT_POINT"));
        EmbVector hr = obj_rubber_point(obj, "TEXT_HEIGHT_ROTATION");
        obj_set_text_size(obj, hr.x);
        obj->setRotation(hr.y);
        obj_set_text(obj, obj_rubber_text(obj, "TEXT_RAPID"));
        break;
    }
    case OBJ_RUBBER_GRIP: {
        obj_update_rubber_grip(obj, painter);
        break;
    }
    default:
        break;
    }
}

/* . */
EmbVector
obj_end_point_2(Object *obj)
{
    QLineF lyne = obj_line(obj);
    EmbVector endPoint2;
    endPoint2.x = lyne.x2();
    endPoint2.y = lyne.y2();
    return obj_pos(obj) + scale_and_rotate(endPoint2, obj->scale(), obj->rotation());
}

/* . */
void
obj_set_end_point_2(Object *obj, EmbVector endPt2)
{
    EmbVector endPt1 = obj_pos(obj);
    EmbVector delta = endPt2 - endPt1;
    obj->setRotation(0);
    obj->setScale(1);
    obj_set_line(obj, 0, 0, delta.x, delta.y);
    obj->setPos(endPt1.x, endPt1.y);
}

/* . */
void
Object::vulcanize()
{
    debug_message("vulcanize()");
    /* FIXME: obj_update_rubber(painter); */

    obj->data.objRubberMode = OBJ_RUBBER_OFF;

    switch (obj->geometry->type) {
    case EMB_POLYLINE:
        if (!data.normalPath.elementCount()) {
            critical_box(
                translate("Empty Polyline Error"),
                translate("The polyline added contains no points. The command that created this object has flawed logic."));
        }
        break;
    case EMB_POLYGON:
        if (!data.normalPath.elementCount()) {
            critical_box(
                translate("Empty Polygon Error"),
                translate("The polygon added contains no points. The command that created this object has flawed logic."));
        }
        break;
    case EMB_PATH:
        if (!data.normalPath.elementCount()) {
            critical_box(
                translate("Empty Path Error"),
                translate("The path added contains no points. The command that created this object has flawed logic."));
        }
        break;
    default:
        break;
    }
}

/* . */
QList<EmbVector>
Object::allGripPoints()
{
    QList<EmbVector> gripPoints;
    switch (obj->geometry->type) {
    case EMB_ARC: {
        gripPoints.append(obj_center(obj));
        gripPoints.append(obj_start_point(obj));
        gripPoints.append(obj_mid_point(obj));
        gripPoints.append(obj_end_point(obj));
        break;
    }
    case EMB_CIRCLE:
    case EMB_ELLIPSE: {
        gripPoints.append(obj_center(obj));
        gripPoints.append(emb_quadrant(obj->geometry, 0));
        gripPoints.append(emb_quadrant(obj->geometry, 90));
        gripPoints.append(emb_quadrant(obj->geometry, 180));
        gripPoints.append(emb_quadrant(obj->geometry, 270));
        break;
    }
    case EMB_DIM_LEADER: {
        gripPoints.append(obj_end_point_1(obj));
        gripPoints.append(obj_end_point_2(obj));
        if (obj->data.curved) {
            gripPoints.append(obj_mid_point(obj));
        }
        break;
    }
    case EMB_IMAGE: {
        gripPoints.append(obj_top_left(obj));
        gripPoints.append(obj_top_right(obj));
        gripPoints.append(obj_bottom_left(obj));
        gripPoints.append(obj_bottom_right(obj));
        break;
    }
    case EMB_LINE: {
        gripPoints.append(obj_end_point_1(obj));
        gripPoints.append(obj_end_point_2(obj));
        gripPoints.append(obj_mid_point(obj));
        break;
    }
    case EMB_PATH: {
        gripPoints.append(obj_pos(obj));
        todo("loop thru all path Elements and return their points.");
        break;
    }
    case EMB_POLYGON:
    case EMB_POLYLINE: {
        QPainterPath::Element element;
        for (int i = 0; i < obj->data.normalPath.elementCount(); ++i) {
            element = obj->data.normalPath.elementAt(i);
            gripPoints.append(to_emb_vector(obj->mapToScene(element.x, element.y)));
        }
        break;
    }
    case EMB_TEXT_SINGLE:
    case EMB_POINT:
    default:
        gripPoints.append(obj_pos(obj));
        break;
    }
    return gripPoints;
}

/* Returns the closest snap point to the mouse point */
EmbVector
Object::mouseSnapPoint(EmbVector mousePoint)
{
    return find_mouse_snap_point(allGripPoints(), mousePoint);
}

/* . */
void
obj_grip_edit(Object *obj, EmbVector before, EmbVector after)
{
    EmbVector delta = emb_vector_subtract(after, before);
    switch (obj->geometry->type) {
    case EMB_ARC: {
        todo("gripEdit() for ArcObject.");
        break;
    }
    case EMB_CIRCLE: {
        if (emb_approx(before, obj_center(obj))) {
            obj->moveBy(delta.x, delta.y);
        }
        else {
            double length = emb_vector_distance(obj_center(obj), after);
            emb_set_radius(obj->geometry, length);
        }
        break;
    }
    case EMB_DIM_LEADER:
    case EMB_LINE: {
        if (emb_approx(before, obj_end_point_1(obj))) {
            obj_set_end_point_1(obj, after);
        }
        else if (emb_approx(before, obj_end_point_2(obj))) {
            obj_set_end_point_2(obj, after);
        }
        else if (emb_approx(before, obj_mid_point(obj))) {
            obj->moveBy(delta.x, delta.y);
        }
        break;
    }
    case EMB_ELLIPSE: {
        todo("gripEdit() for EllipseObject");
        break;
    }
    case EMB_IMAGE:
    case EMB_RECT: {
        double height = emb_height(obj->geometry);
        double width = emb_width(obj->geometry);
        EmbVector tl = obj_top_left(obj);
        if (emb_approx(before, tl)) {
            obj_set_rect(obj, after.x, after.y,
                width - delta.x, height - delta.y);
        }
        else if (emb_approx(before, obj_top_right(obj))) {
            obj_set_rect(obj, tl.x, tl.y+delta.y,
                width + delta.x, height - delta.y);
        }
        else if (emb_approx(before, obj_bottom_left(obj))) {
            obj_set_rect(obj, tl.x+delta.x, tl.y,
                width - delta.x, height + delta.y);
        }
        else if (emb_approx(before, obj_bottom_right(obj))) {
            obj_set_rect(obj, tl.x, tl.y,
                width + delta.x, height + delta.y);
        }
        break;
    }
    case EMB_PATH: {
        todo("gripEdit() for PathObject");
        break;
    }
    case EMB_POLYGON:
    case EMB_POLYLINE: {
        obj->data.gripIndex = obj_find_index(obj, before);
        if (obj->data.gripIndex == -1) {
            return;
        }
        EmbVector a = map_from_scene(obj, after);
        obj->data.normalPath.setElementPositionAt(obj->data.gripIndex, a.x, a.y);
        obj_update_path_r(obj, obj->data.normalPath);
        obj->data.gripIndex = -1;
        break;
    }
    case EMB_TEXT_SINGLE:
    case EMB_POINT:
    default: {
        if (emb_approx(before, obj_pos(obj))) {
            obj->moveBy(delta.x, delta.y);
        }
        break;
    }
    }
}

void
Object::paint(QPainter* painter, const QStyleOptionGraphicsItem *option, QWidget *)
{
    QGraphicsScene* objScene = obj->scene();
    if (!objScene) {
        return;
    }

    QPen paintPen = pen();
    painter->setPen(paintPen);
    obj_update_rubber(obj, painter);
    if (option->state & QStyle::State_Selected) {
        paintPen.setStyle(Qt::DashLine);
    }
    if (objScene->property("ENABLE_LWT").toBool()) {
        paintPen = obj->data.lwtPen;
    }
    painter->setPen(paintPen);

    switch (obj->geometry->type) {
    case EMB_ARC: {
        double startAngle = (emb_start_angle(obj->geometry) + obj->rotation())*16;
        double spanAngle = emb_included_angle(obj->geometry) * 16;

        if (emb_clockwise(obj->geometry)) {
            spanAngle = -spanAngle;
        }

        double rad = obj_radius(obj);
        QRectF paintRect(-rad, -rad, rad*2.0, rad*2.0);
        painter->drawArc(paintRect, startAngle, spanAngle);
        break;
    }
    case EMB_CIRCLE:
    case EMB_ELLIPSE: {
        painter->drawEllipse(obj_rect(obj));
        break;
    }
    case EMB_DIM_LEADER: {
        painter->drawPath(obj->data.lineStylePath);
        painter->drawPath(obj->data.arrowStylePath);

        if (obj->data.filled) {
            painter->fillPath(obj->data.arrowStylePath, obj->data.objPen.color());
        }
        break;
    }
    case EMB_LINE: {
        if (obj->data.objRubberMode != OBJ_RUBBER_LINE) {
            painter->drawLine(obj_line(obj));
        }

        if (objScene->property("ENABLE_LWT").toBool()
            && objScene->property("ENABLE_REAL").toBool()) {
            obj_real_render(obj, painter, path());
        }
        break;
    }
    case EMB_IMAGE: {
        painter->drawRect(obj_rect(obj));
        break;
    }
    case EMB_RECT: {
        painter->drawRect(obj_rect(obj));
        break;
    }
    case EMB_PATH: {
        painter->drawPath(obj_path(obj));
        break;
    }
    case EMB_POLYGON: {
        if (obj->data.normalPath.elementCount()) {
            painter->drawPath(obj->data.normalPath);
            QPainterPath::Element zero = obj->data.normalPath.elementAt(0);
            QPainterPath::Element last = obj->data.normalPath.elementAt(obj->data.normalPath.elementCount()-1);
            painter->drawLine(QPointF(zero.x, zero.y), QPointF(last.x, last.y));
        }
        break;
    }
    case EMB_POLYLINE: {
        painter->drawPath(obj->data.normalPath);

        if (objScene->property("ENABLE_LWT").toBool()
            && objScene->property("ENABLE_REAL").toBool()) {
            obj_real_render(obj, painter, obj->data.normalPath);
        }
        break;
    }
    case EMB_TEXT_SINGLE: {
        painter->drawPath(obj->data.objTextPath);
        break;
    }
    default:
    case EMB_POINT: {
        painter->drawPoint(0,0);
        break;
    }
    }
}

QPainterPath
Object::objectCopyPath() const
{
    return obj->data.normalPath;
}

QPainterPath
Object::objectSavePath() const
{
    switch (obj->geometry->type) {
    case EMB_CIRCLE:
    case EMB_ELLIPSE: {
        QPainterPath path;
        QRectF r = obj_rect(obj);
        path.arcMoveTo(r, 0);
        path.arcTo(r, 0, 360);

        double s = obj->scale();
        QTransform trans;
        trans.rotate(rotation());
        trans.scale(s,s);
        return trans.map(path);
    }
    case EMB_DIM_LEADER:
    case EMB_LINE: {
        QPainterPath path;
        EmbVector delta = obj_delta(obj);
        path.lineTo(delta.x, delta.y);
        return path;
    }
    case EMB_POINT: {
        QPainterPath path;
        path.addRect(-0.00000001, -0.00000001, 0.00000002, 0.00000002);
        return path;
    }
    case EMB_RECT: {
        QPainterPath path;
        QRectF r = obj_rect(obj);
        path.moveTo(r.bottomLeft());
        path.lineTo(r.bottomRight());
        path.lineTo(r.topRight());
        path.lineTo(r.topLeft());
        path.lineTo(r.bottomLeft());

        double s = obj->scale();
        QTransform trans;
        trans.rotate(rotation());
        trans.scale(s,s);
        return trans.map(path);
    }
    case EMB_PATH:
    case EMB_POLYLINE: {
        double s = obj->scale();
        QTransform trans;
        trans.rotate(rotation());
        trans.scale(s,s);
        return trans.map(obj->data.normalPath);
    }
    case EMB_POLYGON: {
        QPainterPath closedPath = obj->data.normalPath;
        closedPath.closeSubpath();
        double s = obj->scale();
        QTransform trans;
        trans.rotate(rotation());
        trans.scale(s,s);
        return trans.map(closedPath);
    }
    default:
        break;
    }
    return obj->data.normalPath;
}
