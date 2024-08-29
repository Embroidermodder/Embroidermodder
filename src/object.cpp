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

/* . */
Object::Object(EmbArc arc, QRgb rgb, QGraphicsItem *item)
{
    debug_message("ArcObject Constructor()");
    obj_init_geometry(this, EMB_ARC, rgb, Qt::SolidLine);
    geometry->object.arc = arc;
    /* TODO: getCurrentLineType */
    calculateData();
    setPos(arc.start.x, arc.start.y);
}

/* . */
Object::Object(EmbCircle circle, QRgb rgb, QGraphicsItem *item)
{
    debug_message("CircleObject Constructor()");
    /* TODO: getCurrentLineType */
    obj_init_geometry(this, EMB_CIRCLE, rgb, Qt::SolidLine);
    geometry->object.circle = circle;

    /*
    EmbVector center;
    center.x = centerX;
    center.y = centerY;
    set_radius(geometry, radius);
    set_center(geometry, center);
    updatePath();
    */
}

/* . */
Object::Object(EmbEllipse ellipse, QRgb rgb, QGraphicsItem *item)
{
    debug_message("EllipseObject Constructor()");
    /* TODO: getCurrentLineType */
    obj_init_geometry(this, EMB_ELLIPSE, rgb, Qt::SolidLine);
    geometry->object.ellipse = ellipse;

    /*
    setObjectSize(width, height);
    EmbVector center;
    center.x = centerX;
    center.y = centerY;
    setObjectCenter(center);
    updatePath();
    */
}

/* . */
Object::Object(EmbPath path, int type_, const QPainterPath& p, QRgb rgb, QGraphicsItem* parent)
{
    debug_message("PolylineObject Constructor()");
    /* TODO: getCurrentLineType */
    obj_init_geometry(this, type_, rgb, Qt::SolidLine);
    updatePath(p);
    /* setObjectPos(x,y); */
}

/*
Object::Object(double x, double y, const QPainterPath p, QRgb rgb, QGraphicsItem* parent)
{
    debug_message("PathObject Constructor()");
    init(x, y, p, rgb, Qt::SolidLine); */ /* TODO: getCurrentLineType */ /*
    obj_init_geometry(this, EMB_PATH, rgb, lineType);
    updatePath(p);
    setObjectPos(x,y);
}

Object::Object(double x, double y, const QPainterPath& p, QRgb rgb, QGraphicsItem* parent) : BaseObject(parent)
{
    debug_message("PolygonObject Constructor()");
    init(x, y, p, rgb, Qt::SolidLine); */ /* TODO: getCurrentLineType */ /*
    obj_init_geometry(this, EMB_POLYGON, rgb, lineType);
    updatePath(p);
    setObjectPos(x,y);
}
*/

/* . */
Object::Object(const QString& str, double x, double y, QRgb rgb, QGraphicsItem* parent)
{
    debug_message("TextSingleObject Constructor()");
    /* TODO: getCurrentLineType */
    obj_init_geometry(this, EMB_TEXT_SINGLE, rgb, Qt::SolidLine);
    data.objTextJustify = "Left"; /* TODO: set the justification properly */

    obj_set_text(obj, str);
    setObjectPos(x,y);
}

/* . */
Object::Object(double x1, double y1, double x2, double y2, QRgb rgb, QGraphicsItem* parent)
{
    debug_message("DimLeaderObject Constructor()");
    /* TODO: getCurrentLineType */
    obj_init_geometry(this, EMB_DIM_LEADER, rgb, Qt::SolidLine);

    data.curved = false;
    data.filled = true;
    setObjectEndPoint1(x1, y1);
    setObjectEndPoint2(x2, y2);
}

/*
Object::Object(double x, double y, double w, double h, QRgb rgb, QGraphicsItem* parent)
{
    debug_message("ImageObject Constructor()"); */
    /* TODO: getCurrentLineType */ /*
    obj_init_geometry(this, EMB_IMAGE, rgb, Qt::SolidLine);
    setObjectRect(x, y, w, h);
}

Object::Object(double x, double y, double w, double h, QRgb rgb, QGraphicsItem* parent)
{
    debug_message("RectObject Constructor()"); */
    /* TODO: getCurrentLineType */ /*
    obj_init_geometry(this, EMB_RECT, rgb, lineType);
    setObjectRect(x, y, w, h);
}
*/

/*
Object::Object(double x1, double y1, double x2, double y2, QRgb rgb, QGraphicsItem* parent)
{
    debug_message("LineObject Constructor()"); */
    /* TODO: getCurrentLineType */ /*

    obj_init_geometry(this, EMB_LINE, rgb, Qt::SolidLine);
    setObjectEndPoint1(x1, y1);
    setObjectEndPoint2(x2, y2);
}
*/

/* . */
Object::Object(EmbPoint_, unsigned int, QGraphicsItem*)
{
}

/* . */
Object::Object(Object* obj, QGraphicsItem* parent)
{
    debug_message("ArcObject Constructor()");
    if (!obj) {
        return;
    }
    obj_init_geometry(this, obj->geometry->type, obj->objectColorRGB(obj), Qt::SolidLine);
    switch (obj->geometry->type) {
    case EMB_ARC:
        geometry->object.arc = obj->geometry->object.arc;
        /* TODO: getCurrentLineType */
        setRotation(obj->rotation());
        break;
    case EMB_CIRCLE:
        geometry->object.circle = obj->geometry->object.circle;
        /* TODO: getCurrentLineType. */
        setRotation(obj->rotation());
        break;
    case EMB_DIM_LEADER:
        geometry->object.line = obj->geometry->object.line;
        /* init(obj->objectX1(), obj->objectY1(), obj->objectX2(), obj->objectY2(), obj->objectColorRGB(obj), Qt::SolidLine); */ /* TODO: getCurrentLineType */
        break;
    case EMB_ELLIPSE:
        geometry->object.ellipse = obj->geometry->object.ellipse;
        /* init(obj->objectCenterX(), obj->objectCenterY(), obj->objectWidth(), obj->objectHeight(), obj->objectColorRGB(obj), Qt::SolidLine); */
        /* TODO: getCurrentLineType */
        setRotation(obj->rotation());
        break;
    case EMB_IMAGE: {
        geometry->object.ellipse = obj->geometry->object.ellipse;
        /* QPointF ptl = obj->objectTopLeft(); */
        /* init(ptl.x(), ptl.y(), obj->objectWidth(), obj->objectHeight(), obj->objectColorRGB(), Qt::SolidLine); */
        /* TODO: getCurrentLineType */
        setRotation(obj->rotation());
        break;
    }
    case EMB_LINE: {
        geometry->object.line = obj->geometry->object.line;
        /* init(obj->objectX1(), obj->objectY1(), obj->objectX2(), obj->objectY2(), obj->objectColorRGB(), Qt::SolidLine); */
        /* TODO: getCurrentLineType */
        break;
    }
    case EMB_PATH: {
        geometry->object.path = obj->geometry->object.path;
        /* init(obj->objectX(), obj->objectY(), obj->objectCopyPath(), obj->objectColorRGB(), Qt::SolidLine);
         * TODO: getCurrentLineType
         * setRotation(obj->rotation());
         * setScale(obj->scale());
         */
        break;
    }
    case EMB_POINT: {
        geometry->object.point = obj->geometry->object.point;
        /* init(obj->objectX(), obj->objectY(), obj->objectColorRGB(), Qt::SolidLine);
         * TODO: getCurrentLineType
         */
        setRotation(obj->rotation());
        break;
    }
    case EMB_POLYGON: {
        geometry->object.polygon = obj->geometry->object.polygon;
        /* init(obj->objectX(), obj->objectY(), obj->objectCopyPath(), obj->objectColorRGB(), Qt::SolidLine);
         * TODO: getCurrentLineType
         * setRotation(obj->rotation());
         * setScale(obj->scale());
         */
        break;
    }
    case EMB_POLYLINE: {
        geometry->object.polyline = obj->geometry->object.polyline;
        /* init(obj->objectX(), obj->objectY(), obj->objectCopyPath(), obj->objectColorRGB(), Qt::SolidLine);
         * TODO: getCurrentLineType
         */
        setRotation(obj->rotation());
        setScale(obj->scale());
        break;
    }
    case EMB_RECT: {
        geometry->object.rect = obj->geometry->object.rect;
        /* QPointF ptl = obj->objectTopLeft();
         * init(ptl.x(), ptl.y(), obj->objectWidth(), obj->objectHeight(), obj->objectColorRGB(), Qt::SolidLine);
         * TODO: getCurrentLineType
         */
        setRotation(obj->rotation());
        break;
    }
    case EMB_TEXT_SINGLE: {
        obj_set_text_font(obj, obj->data.objTextFont);
        obj_set_text_size(obj, obj->data.objTextSize);
        setRotation(obj->rotation());
        obj_set_text_backward(obj, obj->data.objTextBackward);
        obj_set_text_upside_down(obj, obj->data.objTextUpsideDown);
        obj_set_text_style(obj, obj->data.objTextBold, obj->data.objTextItalic,
            obj->data.objTextUnderline, obj->data.objTextStrikeOut, obj->data.objTextOverline);
        /* init(obj->objText, obj->objectX(), obj->objectY(), obj->objectColorRGB(), Qt::SolidLine);
         * TODO: getCurrentLineType
         */
        setScale(obj->scale());
        break;
    }
    default:
        break;
    }
}

/* . */
Object::~Object()
{
    debug_message("ArcObject Destructor()");
    free(geometry);
}

/* WARNING: DO NOT enable QGraphicsItem::ItemIsMovable. If it is enabled,
 * WARNING: and the item is double clicked, the scene will erratically move the item while zooming.
 * WARNING: All movement has to be handled explicitly by us, not by the scene.
 */
void
obj_init_geometry(Object* obj, int type_, QRgb rgb, Qt::PenStyle lineType)
{
    debug_message("BaseObject Constructor()");

    obj->obj = obj;

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

    obj->setObjectColor(rgb);
    obj->setObjectLineType(lineType);
    obj->setObjectLineWeight(0.35); /* TODO: pass in proper lineweight */
    obj->setPen(obj->data.objPen);

    obj->geometry = (EmbGeometry*)malloc(sizeof(EmbGeometry));
    obj->geometry->type = type_;
    obj->geometry->object.color.r = qRed(rgb);
    obj->geometry->object.color.g = qGreen(rgb);
    obj->geometry->object.color.b = qBlue(rgb);
    obj->geometry->lineType = lineType;
}

/* . */
ScriptValue
arc_command(ScriptEnv *context)
{

    return script_null;
}

/* . */
void
Object::drawRubberLine(const QLineF& rubLine, QPainter* painter, const char* colorFromScene)
{
    if (painter) {
        QGraphicsScene* objScene = scene();
        if (!objScene) {
            return;
        }
        QPen colorPen = data.objPen;
        colorPen.setColor(QColor(objScene->property(colorFromScene).toUInt()));
        painter->setPen(colorPen);
        painter->drawLine(rubLine);
        painter->setPen(data.objPen);
    }
}

/* . */
void
Object::realRender(QPainter* painter, const QPainterPath& renderPath)
{
    QColor color1 = data.objPen.color(); /* lighter color */
    QColor color2 = color1.darker(150); /* darker color */

    /* If we have a dark color, lighten it. */
    int darkness = color1.lightness();
    int threshold = 32; /* TODO: This number may need adjusted or maybe just add it to settings. */
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
        /* todo("CIRCLE", "context()"); */
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
            todo("CIRCLE", "click() for TTR");
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
                    obj_set_rubber_point(obj, "CIRCLE_RADIUS", context->x2, context->y2);
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
                obj_set_rubber_point(obj, "CIRCLE_DIAMETER", context->x2, context->y2);
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
        todo("CIRCLE", "prompt() for TTR");
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
/* TODO: Make arrowStyle, arrowStyleAngle, arrowStyleLength, lineStyleAngle,
 * lineStyleLength customizable
 */
void
DimLeaderObject::updateLeader()
{
    int arrowStyle = Closed;
    double arrowStyleAngle = 15.0;
    double arrowStyleLength = 1.0;
    double lineStyleAngle = 45.0;
    double lineStyleLength = 1.0;

    QLineF lyne = line();
    double angle = lyne.angle();
    QPointF ap0 = lyne.p1();
    QPointF lp0 = lyne.p2();

    /* Arrow */
    QLineF lynePerp(lyne.pointAt(arrowStyleLength/lyne.length()) ,lp0);
    lynePerp.setAngle(angle + 90);
    QLineF lyne1(ap0, lp0);
    QLineF lyne2(ap0, lp0);
    lyne1.setAngle(angle + arrowStyleAngle);
    lyne2.setAngle(angle - arrowStyleAngle);
    QPointF ap1;
    QPointF ap2;
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
    todo("ELLIPSE", "context()");
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
    QRectF elRect = rect();
    elRect.setWidth(width);
    elRect.setHeight(height);
    elRect.moveCenter(QPointF(0,0));
    setRect(elRect);
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
        /* todo("LINE", "context()"); */
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
            todo("LINE", "prompt() for UNDO");
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
 * TODO: The path command is currently broken
 */
ScriptValue
path_command(ScriptEnv *context)
{
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
        /* todo("PATH", "context()"); */
        break;
    case CONTEXT_PROMPT:
/*
    if (str == "A" || str == "ARC") {
        todo("PATH", "prompt() for ARC");
    }
    else if (str == "U" || str == "UNDO") {
        todo("PATH", "prompt() for UNDO");
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
    /* TODO: translate needed here when complete */
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
    todo("POINT", "context()");
}

function prompt(str)
{
    EmbVector v;
    if (global->firstRun) {
        if (str == "M" || str == "MODE") {
            todo("POINT", "prompt() for PDMODE");
        }
        else if (str == "S" || str == "SIZE") {
            todo("POINT", "prompt() for PDSIZE");
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
    init(x, y, rgb, Qt::SolidLine); /* TODO: getCurrentLineType */
    init_geometry(EMB_POINT, rgb, lineType);
    setRect(-0.00000001, -0.00000001, 0.00000002, 0.00000002);
    setObjectPos(x,y);
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
        todo("POLYGON", "Sidelength mode");
        break;
    }
    }
}

function
context(char *str)
{
    todo("POLYGON", "context()");
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
        todo("POLYGON", "Sidelength mode");
        break;
    }
    default:
        break;
    }
}
#endif

void
Object::updateRubberGrip(QPainter *painter)
{
    if (!painter) {
        return;
    }
    switch (geometry->type) {
    case EMB_ARC: {
        /* TODO: */
        break;
    }
    case EMB_ELLIPSE: {
        /* TODO: */
        break;
    }
    case EMB_IMAGE: {
        /* TODO: */
        break;
    }
    case EMB_PATH: {
        /* TODO: */
        break;
    }
    case EMB_LINE: {
        QPointF gripPoint = objectRubberPoint(obj, "GRIP_POINT");
        if (gripPoint == objectEndPoint1())
            painter->drawLine(line().p2(), mapFromScene(objectRubberPoint(obj, "")));
        else if (gripPoint == objectEndPoint2())
            painter->drawLine(line().p1(), mapFromScene(objectRubberPoint(obj, "")));
        else if (gripPoint == objectMidPoint())
            painter->drawLine(line().translated(mapFromScene(objectRubberPoint(obj, ""))-mapFromScene(gripPoint)));

        QLineF rubLine(mapFromScene(gripPoint), mapFromScene(objectRubberPoint(obj, "")));
        drawRubberLine(rubLine, painter, "VIEW_COLOR_CROSSHAIR");
        break;
    }
    case EMB_CIRCLE: {
        QPointF gripPoint = objectRubberPoint(obj, "GRIP_POINT");
        if (gripPoint == objectCenter()) {
            painter->drawEllipse(rect().translated(mapFromScene(objectRubberPoint(obj, ""))-mapFromScene(gripPoint)));
            }
        else {
            double gripRadius = QLineF(objectCenter(), objectRubberPoint(obj, "")).length();
            painter->drawEllipse(QPointF(), gripRadius, gripRadius);
        }

        QLineF rubLine(mapFromScene(gripPoint), mapFromScene(objectRubberPoint(obj, "")));
        drawRubberLine(rubLine, painter, "VIEW_COLOR_CROSSHAIR");
        break;
    }
    case EMB_DIM_LEADER: {
        QPointF gripPoint = objectRubberPoint(obj, "GRIP_POINT");
        if (gripPoint == objectEndPoint1()) {
            painter->drawLine(line().p2(), mapFromScene(objectRubberPoint(obj, "")));
        }
        else if (gripPoint == objectEndPoint2()) {
            painter->drawLine(line().p1(), mapFromScene(objectRubberPoint(obj, "")));
        }
        else if (gripPoint == objectMidPoint()) {
            painter->drawLine(line().translated(mapFromScene(objectRubberPoint(obj, ""))-mapFromScene(gripPoint)));
        }
        break;
    }
    case EMB_POINT: {
        QPointF gripPoint = objectRubberPoint(obj, "GRIP_POINT");
        if (gripPoint == scenePos()) {
            QLineF rubLine(mapFromScene(gripPoint), mapFromScene(objectRubberPoint(obj, "")));
            drawRubberLine(rubLine, painter, "VIEW_COLOR_CROSSHAIR");
        }
        break;
    }
    case EMB_POLYGON: {
        int elemCount = data.normalPath.elementCount();
        QPointF gripPoint = objectRubberPoint(obj, "GRIP_POINT");
        if (data.gripIndex == -1) {
            data.gripIndex = findIndex(gripPoint);
                if (data.gripIndex == -1) {
                    return;
                }
            }

            int m = 0;
            int n = 0;

            if (!data.gripIndex) {
                m = elemCount - 1;
                n = 1;
            }
            else if (data.gripIndex == elemCount-1) {
                m = elemCount - 2;
                n = 0;
            }
            else {
                m = data.gripIndex - 1;
                n = data.gripIndex + 1;
            }
            QPainterPath::Element em = data.normalPath.elementAt(m);
            QPainterPath::Element en = data.normalPath.elementAt(n);
            QPointF emPoint = QPointF(em.x, em.y);
            QPointF enPoint = QPointF(en.x, en.y);
            painter->drawLine(emPoint, mapFromScene(objectRubberPoint(obj, "")));
            painter->drawLine(enPoint, mapFromScene(objectRubberPoint(obj, "")));

            QLineF rubLine(mapFromScene(gripPoint), mapFromScene(objectRubberPoint(obj, "")));
            drawRubberLine(rubLine, painter, "VIEW_COLOR_CROSSHAIR");
            break;
        }
        case EMB_POLYLINE: {
            int elemCount = data.normalPath.elementCount();
            QPointF gripPoint = objectRubberPoint(obj, "GRIP_POINT");
            if (data.gripIndex == -1) {
                data.gripIndex = findIndex(gripPoint);
            }
            if (data.gripIndex == -1) {
                return;
            }

            if (!data.gripIndex) {
                /* First */
                QPainterPath::Element ef = data.normalPath.elementAt(1);
                QPointF efPoint = QPointF(ef.x, ef.y);
                painter->drawLine(efPoint, mapFromScene(objectRubberPoint(obj, "")));
            }
            else if (data.gripIndex == elemCount-1) {
                /* Last */
                QPainterPath::Element el = data.normalPath.elementAt(data.gripIndex-1);
                QPointF elPoint = QPointF(el.x, el.y);
                painter->drawLine(elPoint, mapFromScene(objectRubberPoint(obj, "")));
            }
            else {
                /* Middle */
                QPainterPath::Element em = data.normalPath.elementAt(data.gripIndex-1);
                QPainterPath::Element en = data.normalPath.elementAt(data.gripIndex+1);
                QPointF emPoint = QPointF(em.x, em.y);
                QPointF enPoint = QPointF(en.x, en.y);
                painter->drawLine(emPoint, mapFromScene(objectRubberPoint(obj, "")));
                painter->drawLine(enPoint, mapFromScene(objectRubberPoint(obj, "")));
            }

            QLineF rubLine(mapFromScene(gripPoint), mapFromScene(objectRubberPoint(obj, "")));
            drawRubberLine(rubLine, painter, "VIEW_COLOR_CROSSHAIR");
            break;
    }
    case EMB_RECT: {
        /* TODO: Make this work with rotation & scaling */
        /*
        QPointF gripPoint = objectRubberPoint(obj, "GRIP_POINT");
        QPointF after = objectRubberPoint(obj, "");
        QPointF delta = after-gripPoint;
            if (gripPoint == topLeft()) {
                painter->drawPolygon(mapFromScene(QRectF(after.x(), after.y(), objectWidth()-delta.x(), objectHeight()-delta.y())));
            }
            else if (gripPoint == topRight()) {
                painter->drawPolygon(mapFromScene(QRectF(objectTopLeft().x(), objectTopLeft().y()+delta.y(), objectWidth()+delta.x(), objectHeight()-delta.y())));
            }
            else if (gripPoint == bottomLeft()) {
                painter->drawPolygon(mapFromScene(QRectF(objectTopLeft().x()+delta.x(), objectTopLeft().y(), objectWidth()-delta.x(), objectHeight()+delta.y())));
            }
            else if (gripPoint == bottomRight()) {
                painter->drawPolygon(mapFromScene(QRectF(objectTopLeft().x(), objectTopLeft().y(), objectWidth()+delta.x(), objectHeight()+delta.y())));
            }

            QLineF rubLine(mapFromScene(gripPoint), mapFromScene(objectRubberPoint(obj, "")));
            drawRubberLine(rubLine, painter, "VIEW_COLOR_CROSSHAIR");
            */

            QPointF gripPoint = objectRubberPoint(obj, "GRIP_POINT");
            QPointF after = objectRubberPoint(obj, "");
            QPointF delta = after-gripPoint;

            QLineF rubLine(mapFromScene(gripPoint), mapFromScene(objectRubberPoint(obj, "")));
            drawRubberLine(rubLine, painter, "VIEW_COLOR_CROSSHAIR");
        break;
    }
    case EMB_TEXT_SINGLE: {
        QPointF gripPoint = objectRubberPoint(obj, "GRIP_POINT");
        if (gripPoint == scenePos()) {
           painter->drawPath(objectPath(obj).translated(mapFromScene(objectRubberPoint(obj, ""))-mapFromScene(gripPoint)));
        }

        QLineF rubLine(mapFromScene(gripPoint), mapFromScene(objectRubberPoint(obj, "")));
        drawRubberLine(rubLine, painter, "VIEW_COLOR_CROSSHAIR");
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
    todo("POLYLINE", "context()");
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
            todo("POLYLINE", "prompt() for UNDO");
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
    todo("RECTANGLE", "context()");
}

function prompt(str)
{
    if (str == "C" || str == "CHAMFER") {
        todo("RECTANGLE", "prompt() for CHAMFER");
    }
    else if (str == "D" || str == "DIMENSIONS") {
        todo("RECTANGLE", "prompt() for DIMENSIONS");
    }
    else if (str == "F" || str == "FILLET") {
        todo("RECTANGLE", "prompt() for FILLET");
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
    */
    /* TODO: saving polygons, polylines and paths must be stable before we go here. */

    /* TODO: This needs to work like a path, not a polyline. Improve this */
    /*
    TextSingleObject* obj = static_cast<TextSingleObject*>(item);
    if (obj) {
        if (formatType == EMBFORMAT_STITCHONLY) {
            QList<QPainterPath> pathList = obj->objectSavePathList();
            foreach(QPainterPath path, pathList) {
                toPolyline(pattern, obj->objectPos(), path.simplified(), "0", obj->objectColor(), "CONTINUOUS", "BYLAYER"); /* TODO: proper layer/lineType/lineWeight */
                /* TODO: Improve precision, replace simplified */
/*            }
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
    todo("SINGLELINETEXT", "context()");
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
                /* TODO: Rather than ending the command, calculate where the next line would be and modify the x/y to the new point. */
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
    double s = scale();
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

/* QUICKLEADER */
ScriptValue
quickleader_command(ScriptEnv *context)
{

    end_command();
    return script_null;
}

/* TODO: Adding the text is not complete yet. */
#if 0
function main()
{
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
    todo("QUICKLEADER", "context()");
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
QPointF
Object::objectStartPoint() const
{
    switch (geometry->type) {
    case EMB_ARC: {
        return to_qpointf(geometry->object.arc.start);
    }
    default:
        break;
    }
    return scenePos();
}

/* . */
QPointF
Object::objectMidPoint() const
{
    switch (geometry->type) {
    case EMB_ARC: {
        return to_qpointf(geometry->object.arc.mid);
    }
    case EMB_LINE:
    case EMB_DIM_LEADER: {
        QLineF lyne = line();
        QPointF mp = lyne.pointAt(0.5);
        return scenePos() + scale_and_rotate(mp, scale(), rotation());
    }
    default:
        break;
    }
    return scenePos();
}

/* . */
QPointF
Object::objectEndPoint() const
{
    switch (geometry->type) {
    case EMB_ARC: {
        return to_qpointf(geometry->object.arc.end);
    }
    default:
        break;
    }
    return scenePos();
}

/* . */
QPointF
Object::topLeft() const
{
    return scenePos() + scale_and_rotate(rect().topLeft(), scale(), rotation());
}

/* . */
QPointF
Object::topRight() const
{
    return scenePos() + scale_and_rotate(rect().topRight(), scale(), rotation());
}

/* . */
QPointF
Object::bottomLeft() const
{
    return scenePos() + scale_and_rotate(rect().bottomLeft(), scale(), rotation());
}

/* . */
QPointF
Object::bottomRight() const
{
    return scenePos() + scale_and_rotate(rect().bottomRight(), scale(), rotation());
}

/* . */
QPointF
Object::objectCenter() const
{
    return scenePos();
}

/* . */
QStringList objectTextJustifyList = {
    "Left",
    "Center",
    "Right",
    /* TODO: << "Aligned" */
    "Middle",
    /* TODO: << "Fit" */
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
obj_set_text(Object* obj, const QString& str)
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
        /* TODO: TextSingleObject Aligned Justification. */
    }
    else if (obj->data.objTextJustify == "Middle") {
        textPath.translate(-jRect.center());
    }
    else if (obj->data.objTextJustify == "Fit") {
        /* TODO: TextSingleObject Fit Justification. */
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
    obj->setObjectPath(gripPath);
}

/* . */
void
obj_set_text_font(Object* obj, const QString& font)
{
    obj->data.objTextFont = font;
    obj_set_text(obj, obj->data.objText);
}

/* Verify the string is a valid option. */
void
obj_set_text_justify(Object* obj, const QString& justify)
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
QPointF
Object::objectEndPoint1() const
{
    return scenePos();
}

/* . */
void
Object::setObjectEndPoint1(const QPointF& endPt1)
{
    setObjectEndPoint1(endPt1.x(), endPt1.y());
}

/* . */
void
Object::setObjectEndPoint1(double x1, double y1)
{
    QPointF endPt2 = objectEndPoint2();
    double x2 = endPt2.x();
    double y2 = endPt2.y();
    double dx = x2 - x1;
    double dy = y2 - y1;
    setRotation(0);
    setScale(1);
    setLine(0, 0, dx, dy);
    setPos(x1, y1);
}

/* . */
void
Object::updatePath()
{
    switch (geometry->type) {
    case EMB_ARC: {
        double startAngle = (get_start_angle(*geometry) + rotation());
        double spanAngle = get_included_angle(*geometry);

        if (get_clockwise(*geometry)) {
            spanAngle = -spanAngle;
        }

        QPainterPath path;
        path.arcMoveTo(rect(), startAngle);
        path.arcTo(rect(), startAngle, spanAngle);
        /* NOTE: Reverse the path so that the inside area isn't considered part of the arc. */
        path.arcTo(rect(), startAngle+spanAngle, -spanAngle);
        setObjectPath(path);
        break;
    }
    case EMB_CIRCLE: {
        QPainterPath path;
        QRectF r = rect();
        /* Add the center point. */
        path.addRect(-0.00000001, -0.00000001, 0.00000002, 0.00000002);
        /* Add the circle */
        path.arcMoveTo(r, 0);
        path.arcTo(r, 0, 360);
        /* NOTE: Reverse the path so that the inside area isn't considered part of the circle. */
        path.arcTo(r, 0, -360);
        setObjectPath(path);
        break;
    }
    case EMB_ELLIPSE: {
        QPainterPath path;
        QRectF r = rect();
        path.arcMoveTo(r, 0);
        path.arcTo(r, 0, 360);
        /* NOTE: Reverse the path so that the inside area isn't considered part of the ellipse. */
        path.arcTo(r, 0, -360);
        setObjectPath(path);
        break;
    }
    case EMB_RECT:
    case EMB_IMAGE: {
        QPainterPath path;
        QRectF r = rect();
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
        setObjectPath(path);
        break;
    }
    default:
        break;
    }
}

/* . */
void
Object::calculateData(void)
{
    EmbVector center = emb_arc_center(*geometry);

    double radius = emb_vector_distance(center, geometry->object.arc.mid);
    updateArcRect(radius);
    updatePath();
    setRotation(0);
    setScale(1);
}

/* . */
void
Object::updateArcRect(double radius)
{
    QRectF arcRect;
    arcRect.setWidth(radius*2.0);
    arcRect.setHeight(radius*2.0);
    arcRect.moveCenter(QPointF(0,0));
    setRect(arcRect);
}

/* . */
void
Object::setObjectLineWeight(double lineWeight)
{
    data.objPen.setWidthF(0); /* NOTE: The objPen will always be cosmetic. */

    if (lineWeight < 0) {
        if (lineWeight == OBJ_LWT_BYLAYER) {
            data.lwtPen.setWidthF(0.35); /* TODO: getLayerLineWeight */
        }
        else if (lineWeight == OBJ_LWT_BYBLOCK) {
            data.lwtPen.setWidthF(0.35); /* TODO: getBlockLineWeight */
        }
        else {
            QMessageBox::warning(0, QObject::tr("Error - Negative Lineweight"),
                                    QObject::tr("Lineweight: %1")
                                    .arg(QString().setNum(lineWeight)));
            debug_message("Lineweight cannot be negative! Inverting sign.");
            data.lwtPen.setWidthF(-lineWeight);
        }
    }
    else {
        data.lwtPen.setWidthF(lineWeight);
    }
}

/* . */
void
Object::updatePath(QPainterPath const& p)
{
    switch (geometry->type) {
    case EMB_POLYGON: {
        data.normalPath = p;
        QPainterPath closedPath = data.normalPath;
        closedPath.closeSubpath();
        QPainterPath reversePath = closedPath.toReversed();
        reversePath.connectPath(closedPath);
        setObjectPath(reversePath);
        break;
    }
    case EMB_PATH:
    case EMB_POLYLINE: {
        data.normalPath = p;
        QPainterPath reversePath = data.normalPath.toReversed();
        reversePath.connectPath(data.normalPath);
        setObjectPath(reversePath);
        break;
    }
    default:
        break;
    }
}

/* . */
void
Object::setObjectColor(const QColor& color)
{
    data.objPen.setColor(color);
    data.lwtPen.setColor(color);
}

/* . */
void
Object::setObjectColorRGB(QRgb rgb)
{
    data.objPen.setColor(QColor(rgb));
    data.lwtPen.setColor(QColor(rgb));
}

/* . */
void
Object::setObjectLineType(Qt::PenStyle lineType)
{
    data.objPen.setStyle(lineType);
    data.lwtPen.setStyle(lineType);
}

/* . */
QPointF
Object::objectRubberPoint(Object *obj, const QString& key) const
{
    if (data.objRubberPoints.contains(key)) {
        return data.objRubberPoints.value(key);
    }

    QGraphicsScene* gscene = scene();
    if (gscene) {
        return scene()->property("SCENE_QSNAP_POINT").toPointF();
    }
    return QPointF();
}

/* . */
QString
Object::objectRubberText(Object *obj, const QString& key) const
{
    if (data.objRubberTexts.contains(key)) {
        return data.objRubberTexts.value(key);
    }
    return "";
}

/* . */
QRectF
Object::boundingRect() const
{
    /* If gripped, force this object to be drawn even if it is offscreen. */
    if (data.objRubberMode == OBJ_RUBBER_GRIP) {
        return scene()->sceneRect();
    }
    return path().boundingRect();
}

/* . */
void
Object::setObjectCenter(EmbVector center)
{
    setPos(center.x, center.y);
}

void
Object::setObjectCenter(const QPointF& center)
{
    setObjectCenter(to_emb_vector(center));
}

/* . */
void
Object::setObjectCenterX(double centerX)
{
    setX(centerX);
}

/* . */
void
Object::setObjectCenterY(double centerY)
{
    setY(centerY);
}

/* . */
int
Object::findIndex(const QPointF& point)
{
    int elemCount = data.normalPath.elementCount();
    /* NOTE: Points here are in item coordinates */
    QPointF itemPoint = mapFromScene(point);
    for (int i = 0; i < elemCount; i++) {
        QPainterPath::Element e = data.normalPath.elementAt(i);
        QPointF elemPoint = QPointF(e.x, e.y);
        if (itemPoint == elemPoint) return i;
    }
    return -1;
}

/* TODO: Arc,Path Rubber Modes */
void
Object::updateRubber(QPainter* painter)
{
    switch (data.objRubberMode) {
    case OBJ_RUBBER_CIRCLE_1P_RAD: {
        QPointF sceneCenterPoint = objectRubberPoint(obj, "CIRCLE_CENTER");
        QPointF sceneQSnapPoint = objectRubberPoint(obj, "CIRCLE_RADIUS");
        QPointF itemCenterPoint = mapFromScene(sceneCenterPoint);
        QPointF itemQSnapPoint = mapFromScene(sceneQSnapPoint);
        QLineF itemLine(itemCenterPoint, itemQSnapPoint);
        setObjectCenter(to_emb_vector(sceneCenterPoint));
        QLineF sceneLine(sceneCenterPoint, sceneQSnapPoint);
        double radius = sceneLine.length();
        set_radius(geometry, radius);
        if (painter) {
            drawRubberLine(itemLine, painter, "VIEW_COLOR_CROSSHAIR");
        }
        updatePath();
        break;
    }
    case OBJ_RUBBER_CIRCLE_1P_DIA: {
        QPointF sceneCenterPoint = objectRubberPoint(obj, "CIRCLE_CENTER");
        QPointF sceneQSnapPoint = objectRubberPoint(obj, "CIRCLE_DIAMETER");
        QPointF itemCenterPoint = mapFromScene(sceneCenterPoint);
        QPointF itemQSnapPoint = mapFromScene(sceneQSnapPoint);
        QLineF itemLine(itemCenterPoint, itemQSnapPoint);
        setObjectCenter(to_emb_vector(sceneCenterPoint));
        QLineF sceneLine(sceneCenterPoint, sceneQSnapPoint);
        double diameter = sceneLine.length();
        set_diameter(geometry, diameter);
        if (painter) {
            drawRubberLine(itemLine, painter, "VIEW_COLOR_CROSSHAIR");
        }
        updatePath();
        break;
    }
    case OBJ_RUBBER_CIRCLE_2P: {
        QPointF sceneTan1Point = objectRubberPoint(obj, "CIRCLE_TAN1");
        QPointF sceneQSnapPoint = objectRubberPoint(obj, "CIRCLE_TAN2");
        QLineF sceneLine(sceneTan1Point, sceneQSnapPoint);
        setObjectCenter(to_emb_vector(sceneLine.pointAt(0.5)));
        double diameter = sceneLine.length();
        set_diameter(geometry, diameter);
        updatePath();
        break;
    }
    case OBJ_RUBBER_CIRCLE_3P: {
        QPointF sceneTan1Point = objectRubberPoint(obj, "CIRCLE_TAN1");
        QPointF sceneTan2Point = objectRubberPoint(obj, "CIRCLE_TAN2");
        QPointF sceneTan3Point = objectRubberPoint(obj, "CIRCLE_TAN3");

        EmbGeometry g;
        g.object.arc.start = to_emb_vector(sceneTan1Point);
        g.object.arc.mid = to_emb_vector(sceneTan2Point);
        g.object.arc.end = to_emb_vector(sceneTan3Point);
        g.type = EMB_ARC;
        EmbVector center = emb_arc_center(g);
        setObjectCenter(center);
        double radius = emb_vector_distance(center, to_emb_vector(sceneTan3Point));
        set_radius(geometry, radius);
        updatePath();
        break;
    }
    case OBJ_RUBBER_DIMLEADER_LINE: {
        QPointF sceneStartPoint = objectRubberPoint(obj, "DIMLEADER_LINE_START");
        QPointF sceneQSnapPoint = objectRubberPoint(obj, "DIMLEADER_LINE_END");

        setObjectEndPoint1(sceneStartPoint);
        setObjectEndPoint2(sceneQSnapPoint);
        break;
    }
    case OBJ_RUBBER_ELLIPSE_LINE: {
        QPointF sceneLinePoint1 = objectRubberPoint(obj, "ELLIPSE_LINE_POINT1");
        QPointF sceneLinePoint2 = objectRubberPoint(obj, "ELLIPSE_LINE_POINT2");
        QPointF itemLinePoint1  = mapFromScene(sceneLinePoint1);
        QPointF itemLinePoint2  = mapFromScene(sceneLinePoint2);
        QLineF itemLine(itemLinePoint1, itemLinePoint2);
        if (painter) {
            drawRubberLine(itemLine, painter, "VIEW_COLOR_CROSSHAIR");
        }
        updatePath();
        break;
    }
    case OBJ_RUBBER_ELLIPSE_MAJORDIAMETER_MINORRADIUS: {
        QPointF sceneAxis1Point1 = objectRubberPoint(obj, "ELLIPSE_AXIS1_POINT1");
        QPointF sceneAxis1Point2 = objectRubberPoint(obj, "ELLIPSE_AXIS1_POINT2");
        QPointF sceneCenterPoint = objectRubberPoint(obj, "ELLIPSE_CENTER");
        QPointF sceneAxis2Point2 = objectRubberPoint(obj, "ELLIPSE_AXIS2_POINT2");
        double ellipseWidth = objectRubberPoint(obj, "ELLIPSE_WIDTH").x();
        double ellipseRot = objectRubberPoint(obj, "ELLIPSE_ROT").x();

        /* TODO: incorporate perpendicularDistance() into libcgeometry */
        double px = sceneAxis2Point2.x();
        double py = sceneAxis2Point2.y();
        double x1 = sceneAxis1Point1.x();
        double y1 = sceneAxis1Point1.y();
        QLineF line(sceneAxis1Point1, sceneAxis1Point2);
        QLineF norm = line.normalVector();
        double dx = px-x1;
        double dy = py-y1;
        norm.translate(dx, dy);
        QPointF iPoint;
        norm.intersects(line, &iPoint);
        double ellipseHeight = QLineF(px, py, iPoint.x(), iPoint.y()).length()*2.0;

        setObjectCenter(sceneCenterPoint);
        setObjectSize(ellipseWidth, ellipseHeight);
        setRotation(-ellipseRot);

        QPointF itemCenterPoint = mapFromScene(sceneCenterPoint);
        QPointF itemAxis2Point2 = mapFromScene(sceneAxis2Point2);
        QLineF itemLine(itemCenterPoint, itemAxis2Point2);
        if (painter) {
            drawRubberLine(itemLine, painter, "VIEW_COLOR_CROSSHAIR");
        }
        updatePath();
        break;
    }
    case OBJ_RUBBER_ELLIPSE_MAJORRADIUS_MINORRADIUS: {
        QPointF sceneAxis1Point2 = objectRubberPoint(obj, "ELLIPSE_AXIS1_POINT2");
        QPointF sceneCenterPoint = objectRubberPoint(obj, "ELLIPSE_CENTER");
        QPointF sceneAxis2Point2 = objectRubberPoint(obj, "ELLIPSE_AXIS2_POINT2");
        double ellipseWidth = objectRubberPoint(obj, "ELLIPSE_WIDTH").x();
        double ellipseRot = objectRubberPoint(obj, "ELLIPSE_ROT").x();

        /* TODO: incorporate perpendicularDistance() into libcgeometry */
        double px = sceneAxis2Point2.x();
        double py = sceneAxis2Point2.y();
        double x1 = sceneCenterPoint.x();
        double y1 = sceneCenterPoint.y();
        QLineF line(sceneCenterPoint, sceneAxis1Point2);
        QLineF norm = line.normalVector();
        double dx = px-x1;
        double dy = py-y1;
        norm.translate(dx, dy);
        QPointF iPoint;
        norm.intersects(line, &iPoint);
        double ellipseHeight = QLineF(px, py, iPoint.x(), iPoint.y()).length()*2.0;

        setObjectCenter(sceneCenterPoint);
        setObjectSize(ellipseWidth, ellipseHeight);
        setRotation(-ellipseRot);

        QPointF itemCenterPoint = mapFromScene(sceneCenterPoint);
        QPointF itemAxis2Point2 = mapFromScene(sceneAxis2Point2);
        QLineF itemLine(itemCenterPoint, itemAxis2Point2);
        if (painter) {
            drawRubberLine(itemLine, painter, "VIEW_COLOR_CROSSHAIR");
        }
        updatePath();
        break;
    }
    case OBJ_RUBBER_IMAGE: {
        QPointF sceneStartPoint = objectRubberPoint(obj, "IMAGE_START");
        QPointF sceneEndPoint = objectRubberPoint(obj, "IMAGE_END");
        double x = sceneStartPoint.x();
        double y = sceneStartPoint.y();
        double w = sceneEndPoint.x() - sceneStartPoint.x();
        double h = sceneEndPoint.y() - sceneStartPoint.y();
        setObjectRect(x,y,w,h);
        updatePath();
        break;
    }
    case OBJ_RUBBER_LINE: {
        QPointF sceneStartPoint = objectRubberPoint(obj, "LINE_START");
        QPointF sceneQSnapPoint = objectRubberPoint(obj, "LINE_END");

        setObjectEndPoint1(sceneStartPoint);
        setObjectEndPoint2(sceneQSnapPoint);

        drawRubberLine(line(), painter, "VIEW_COLOR_CROSSHAIR");
        break;
    }
    case OBJ_RUBBER_POLYGON: {
        setObjectPos(objectRubberPoint(obj, "POLYGON_POINT_0"));

        bool ok = false;
        QString numStr = objectRubberText(obj, "POLYGON_NUM_POINTS");
        if (numStr.isNull()) {
            return;
        }
        int num = numStr.toInt(&ok);
        if (!ok) {
            return;
        }

        QString appendStr;
        QPainterPath rubberPath;
        rubberPath.moveTo(mapFromScene(objectRubberPoint(obj, "POLYGON_POINT_0")));
        for (int i = 1; i <= num; i++) {
            appendStr = "POLYGON_POINT_" + QString().setNum(i);
            QPointF appendPoint = mapFromScene(objectRubberPoint(obj, appendStr));
            rubberPath.lineTo(appendPoint);
        }
        /* rubberPath.lineTo(0,0); */
        updatePath(rubberPath);

        /* Ensure the path isn't updated until the number of points is changed again. */
        setObjectRubberText("POLYGON_NUM_POINTS", QString());
        break;
    }
    case OBJ_RUBBER_POLYGON_INSCRIBE: {
        setObjectPos(objectRubberPoint(obj, "POLYGON_CENTER"));

        quint16 numSides = objectRubberPoint(obj, "POLYGON_NUM_SIDES").x();

        QPointF inscribePoint = mapFromScene(objectRubberPoint(obj, "POLYGON_INSCRIBE_POINT"));
        QLineF inscribeLine = QLineF(QPointF(0,0), inscribePoint);
        double inscribeAngle = inscribeLine.angle();
        double inscribeInc = 360.0/numSides;

        if (painter) {
            drawRubberLine(inscribeLine, painter, "VIEW_COLOR_CROSSHAIR");
        }

        QPainterPath inscribePath;
        /* First Point */
        inscribePath.moveTo(inscribePoint);
        /* Remaining Points */
        for (int i = 1; i < numSides; i++) {
            inscribeLine.setAngle(inscribeAngle + inscribeInc*i);
            inscribePath.lineTo(inscribeLine.p2());
        }
        updatePath(inscribePath);
        break;
    }
    case OBJ_RUBBER_POLYGON_CIRCUMSCRIBE: {
        setObjectPos(objectRubberPoint(obj, "POLYGON_CENTER"));

        quint16 numSides = objectRubberPoint(obj, "POLYGON_NUM_SIDES").x();

        QPointF circumscribePoint = mapFromScene(objectRubberPoint(obj, "POLYGON_CIRCUMSCRIBE_POINT"));
        QLineF circumscribeLine = QLineF(QPointF(0,0), circumscribePoint);
        double circumscribeAngle = circumscribeLine.angle();
        double circumscribeInc = 360.0/numSides;

        if (painter) {
            drawRubberLine(circumscribeLine, painter, "VIEW_COLOR_CROSSHAIR");
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
        updatePath(circumscribePath);
        break;
    }
    case OBJ_RUBBER_POLYLINE: {
        setObjectPos(objectRubberPoint(obj, "POLYLINE_POINT_0"));

        QLineF rubberLine(data.normalPath.currentPosition(), mapFromScene(objectRubberPoint(obj, "")));
        if (painter) drawRubberLine(rubberLine, painter, "VIEW_COLOR_CROSSHAIR");

        bool ok = false;
        QString numStr = objectRubberText(obj, "POLYLINE_NUM_POINTS");
        if (numStr.isNull()) return;
        int num = numStr.toInt(&ok);
        if (!ok) {
            return;
        }

        QString appendStr;
        QPainterPath rubberPath;
        for (int i = 1; i <= num; i++) {
            appendStr = "POLYLINE_POINT_" + QString().setNum(i);
            QPointF appendPoint = mapFromScene(objectRubberPoint(obj, appendStr));
            rubberPath.lineTo(appendPoint);
        }
        updatePath(rubberPath);

        /* Ensure the path isn't updated until the number of points is changed again. */
        setObjectRubberText("POLYLINE_NUM_POINTS", QString());
        break;
    }
    case OBJ_RUBBER_RECTANGLE: {
        QPointF sceneStartPoint = objectRubberPoint(obj, "RECTANGLE_START");
        QPointF sceneEndPoint = objectRubberPoint(obj, "RECTANGLE_END");
        double x = sceneStartPoint.x();
        double y = sceneStartPoint.y();
        double w = sceneEndPoint.x() - sceneStartPoint.x();
        double h = sceneEndPoint.y() - sceneStartPoint.y();
        setObjectRect(x,y,w,h);
        updatePath();
        break;
    }
    case OBJ_RUBBER_TEXTSINGLE: {
        obj_set_text_font(obj, objectRubberText(obj, "TEXT_FONT"));
        obj_set_text_justify(obj, objectRubberText(obj, "TEXT_JUSTIFY"));
        setObjectPos(objectRubberPoint(obj, "TEXT_POINT"));
        QPointF hr = objectRubberPoint(obj, "TEXT_HEIGHT_ROTATION");
        obj_set_text_size(obj, hr.x());
        setRotation(hr.y());
        obj_set_text(obj, objectRubberText(obj, "TEXT_RAPID"));
        break;
    }
    case OBJ_RUBBER_GRIP: {
        updateRubberGrip(painter);
        break;
    }
    default:
        break;
    }
}

/* . */
void
Object::setObjectRect(double x, double y, double w, double h)
{
    setPos(x, y);
    setRect(0, 0, w, h);
    updatePath();
}

/* . */
QPointF
Object::objectEndPoint2() const
{
    QLineF lyne = line();
    QPointF endPoint2(lyne.x2(), lyne.y2());
    return scenePos() + scale_and_rotate(endPoint2, scale(), rotation());
}

/* . */
void
Object::setObjectEndPoint2(const QPointF& endPt2)
{
    setObjectEndPoint2(endPt2.x(), endPt2.y());
}

/* . */
void
Object::setObjectEndPoint2(double x2, double y2)
{
    QPointF endPt1 = scenePos();
    double x1 = endPt1.x();
    double y1 = endPt1.y();
    double dx = x2 - x1;
    double dy = y2 - y1;
    setRotation(0);
    setScale(1);
    setLine(0, 0, dx, dy);
    setPos(x1, y1);
}

/* . */
void
Object::vulcanize()
{
    debug_message("vulcanize()");
    /* FIXME: updateRubber(painter); */

    data.objRubberMode = OBJ_RUBBER_OFF;

    switch (geometry->type) {
    case EMB_POLYLINE:
        if (!data.normalPath.elementCount()) {
            QMessageBox::critical(0,
                QObject::tr("Empty Polyline Error"),
                QObject::tr("The polyline added contains no points. The command that created this object has flawed logic."));
        }
        break;
    case EMB_POLYGON:
        if (!data.normalPath.elementCount()) {
            QMessageBox::critical(0,
                QObject::tr("Empty Polygon Error"),
                QObject::tr("The polygon added contains no points. The command that created this object has flawed logic."));
        }
        break;
    case EMB_PATH:
        if (!data.normalPath.elementCount()) {
            QMessageBox::critical(0,
                QObject::tr("Empty Path Error"),
                QObject::tr("The path added contains no points. The command that created this object has flawed logic."));
        }
        break;
    default:
        break;
    }
}

QList<QPointF>
Object::allGripPoints()
{
    QList<QPointF> gripPoints;
    switch (geometry->type) {
    case EMB_ARC: {
        gripPoints << objectCenter()
            << objectStartPoint()
            << objectMidPoint()
            << objectEndPoint();
        break;
    }
    case EMB_CIRCLE:
    case EMB_ELLIPSE: {
        gripPoints << objectCenter()
            << to_qpointf(get_quadrant(*geometry, 0))
            << to_qpointf(get_quadrant(*geometry, 90))
            << to_qpointf(get_quadrant(*geometry, 180))
            << to_qpointf(get_quadrant(*geometry, 270));
        break;
    }
    case EMB_DIM_LEADER: {
        gripPoints << objectEndPoint1() << objectEndPoint2();
        if (data.curved) {
            gripPoints << objectMidPoint();
        }
        break;
    }
    case EMB_IMAGE: {
        gripPoints << topLeft()
             << topRight()
             << bottomLeft()
             << bottomRight();
        break;
    }
    case EMB_LINE: {
        gripPoints << objectEndPoint1() << objectEndPoint2() << objectMidPoint();
        break;
    }
    case EMB_PATH: {
        gripPoints << scenePos(); /* TODO: loop thru all path Elements and return their points */
        break;
    }
    case EMB_POLYGON:
    case EMB_POLYLINE: {
        QPainterPath::Element element;
        for (int i = 0; i < data.normalPath.elementCount(); ++i) {
            element = data.normalPath.elementAt(i);
            gripPoints << mapToScene(element.x, element.y);
        }
        break;
    }
    case EMB_TEXT_SINGLE:
    case EMB_POINT:
    default:
        gripPoints << scenePos();
        break;
    }
    return gripPoints;
}

/* Returns the closest snap point to the mouse point */
QPointF
Object::mouseSnapPoint(const QPointF& mousePoint)
{
    return find_mouse_snap_point(allGripPoints(), mousePoint);
}

void
Object::gripEdit(const QPointF& before, const QPointF& after)
{
    switch (geometry->type) {
    case EMB_ARC: {
        /* TODO: gripEdit() for ArcObject */
        break;
    }
    case EMB_CIRCLE: {
        if (before == objectCenter()) {
            QPointF delta = after-before;
            moveBy(delta.x(), delta.y());
        }
        else {
            set_radius(geometry, QLineF(objectCenter(), after).length());
        }
        break;
    }
    case EMB_DIM_LEADER:
    case EMB_LINE: {
        if (before == objectEndPoint1()) {
            setObjectEndPoint1(after);
        }
        else if (before == objectEndPoint2()) {
            setObjectEndPoint2(after);
        }
        else if (before == objectMidPoint()) {
            QPointF delta = after-before;
            moveBy(delta.x(), delta.y());
        }
        break;
    }
    case EMB_ELLIPSE: {
        /* TODO: gripEdit() for EllipseObject */
        break;
    }
    case EMB_IMAGE:
    case EMB_RECT: {
        QPointF delta = after-before;
        double height = get_height(*geometry);
        double width = get_width(*geometry);
        if (before == topLeft()) {
            setObjectRect(after.x(), after.y(),
                width - delta.x(), height - delta.y());
        }
        else if (before == topRight()) {
            setObjectRect(topLeft().x(), topLeft().y()+delta.y(),
                width + delta.x(), height - delta.y());
        }
        else if (before == bottomLeft()) {
            setObjectRect(topLeft().x()+delta.x(), topLeft().y(),
                width - delta.x(), height + delta.y());
        }
        else if (before == bottomRight()) {
            setObjectRect(topLeft().x(), topLeft().y(),
                width + delta.x(), height + delta.y());
        }
        break;
    }
    case EMB_PATH: {
        /* TODO: gripEdit() for PathObject */
        break;
    }
    case EMB_POLYGON:
    case EMB_POLYLINE: {
        data.gripIndex = findIndex(before);
        if (data.gripIndex == -1) {
            return;
        }
        QPointF a = mapFromScene(after);
        data.normalPath.setElementPositionAt(data.gripIndex, a.x(), a.y());
        updatePath(data.normalPath);
        data.gripIndex = -1;
        break;
    }
    case EMB_TEXT_SINGLE:
    case EMB_POINT:
    default: {
        if (before == scenePos()) {
            QPointF delta = after-before;
            moveBy(delta.x(), delta.y());
        }
        break;
    }
    }
}

void
Object::paint(QPainter* painter, const QStyleOptionGraphicsItem *option, QWidget *)
{
    QGraphicsScene* objScene = scene();
    if (!objScene) {
        return;
    }

    QPen paintPen = pen();
    painter->setPen(paintPen);
    updateRubber(painter);
    if (option->state & QStyle::State_Selected) {
        paintPen.setStyle(Qt::DashLine);
    }
    if (objScene->property("ENABLE_LWT").toBool()) {
        paintPen = lineWeightPen();
    }
    painter->setPen(paintPen);

    switch (geometry->type) {
    case EMB_ARC: {
        double startAngle = (get_start_angle(*geometry) + rotation())*16;
        double spanAngle = get_included_angle(*geometry)*16;

        if (get_clockwise(*geometry)) {
            spanAngle = -spanAngle;
        }

        double rad = objectRadius();
        QRectF paintRect(-rad, -rad, rad*2.0, rad*2.0);
        painter->drawArc(paintRect, startAngle, spanAngle);
        break;
    }
    case EMB_CIRCLE:
    case EMB_ELLIPSE: {
        painter->drawEllipse(rect());
        break;
    }
    case EMB_DIM_LEADER: {
        painter->drawPath(data.lineStylePath);
        painter->drawPath(data.arrowStylePath);

        if (data.filled) {
            painter->fillPath(data.arrowStylePath, data.objPen.color());
        }
        break;
    }
    case EMB_LINE: {
        if (data.objRubberMode != OBJ_RUBBER_LINE) {
            painter->drawLine(line());
        }

        if (objScene->property("ENABLE_LWT").toBool()
            && objScene->property("ENABLE_REAL").toBool()) {
            realRender(painter, path());
        }
        break;
    }
    case EMB_IMAGE: {
        painter->drawRect(rect());
        break;
    }
    case EMB_RECT: {
        painter->drawRect(rect());
        break;
    }
    case EMB_PATH: {
        painter->drawPath(objectPath(obj));
        break;
    }
    case EMB_POLYGON: {
        if (data.normalPath.elementCount()) {
            painter->drawPath(data.normalPath);
            QPainterPath::Element zero = data.normalPath.elementAt(0);
            QPainterPath::Element last = data.normalPath.elementAt(data.normalPath.elementCount()-1);
            painter->drawLine(QPointF(zero.x, zero.y), QPointF(last.x, last.y));
        }
        break;
    }
    case EMB_POLYLINE: {
        painter->drawPath(data.normalPath);

        if (objScene->property("ENABLE_LWT").toBool()
            && objScene->property("ENABLE_REAL").toBool()) {
            realRender(painter, data.normalPath);
        }
        break;
    }
    case EMB_TEXT_SINGLE: {
        painter->drawPath(data.objTextPath);
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
    return data.normalPath;
}

QPainterPath
Object::objectSavePath() const
{
    switch (geometry->type) {
    case EMB_CIRCLE:
    case EMB_ELLIPSE: {
        QPainterPath path;
        QRectF r = rect();
        path.arcMoveTo(r, 0);
        path.arcTo(r, 0, 360);

        double s = scale();
        QTransform trans;
        trans.rotate(rotation());
        trans.scale(s,s);
        return trans.map(path);
    }
    case EMB_DIM_LEADER:
    case EMB_LINE: {
        QPainterPath path;
        path.lineTo(objectDelta().x(), objectDelta().y());
        return path;
    }
    case EMB_POINT: {
        QPainterPath path;
        path.addRect(-0.00000001, -0.00000001, 0.00000002, 0.00000002);
        return path;
    }
    case EMB_RECT: {
        QPainterPath path;
        QRectF r = rect();
        path.moveTo(r.bottomLeft());
        path.lineTo(r.bottomRight());
        path.lineTo(r.topRight());
        path.lineTo(r.topLeft());
        path.lineTo(r.bottomLeft());

        double s = scale();
        QTransform trans;
        trans.rotate(rotation());
        trans.scale(s,s);
        return trans.map(path);
    }
    case EMB_PATH:
    case EMB_POLYLINE: {
        double s = scale();
        QTransform trans;
        trans.rotate(rotation());
        trans.scale(s,s);
        return trans.map(data.normalPath);
    }
    case EMB_POLYGON: {
        QPainterPath closedPath = data.normalPath;
        closedPath.closeSubpath();
        double s = scale();
        QTransform trans;
        trans.rotate(rotation());
        trans.scale(s,s);
        return trans.map(closedPath);
    }
    default:
        break;
    }
    return data.normalPath;
}
