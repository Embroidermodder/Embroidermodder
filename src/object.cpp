/*
 * Embroidermodder 2.
 *
 * Copyright 2011-2024 The Embroidermodder Team
 * Embroidermodder 2 is Open Source Software, see LICENSE.md for licensing terms.
 *
 * Download the docs at https://www.libembroidery.org/downloads/emrm_current.pdf
 * for advice on altering this file.
 *
 * Object: attempting to combine the geometry objects into one that changes
 * behaviour depending on its mode.
 */

#include "embroidermodder.h"

EmbVector
to_emb_vector(QPointF p)
{
    EmbVector v;
    v.x = p.x();
    v.y = p.y();
    return v;
}

QPointF
to_qpointf(EmbVector v)
{
    QPointF p(v.x, v.y);
    return p;
}

QPointF
scale_and_rotate(QPointF v, double scale, double angle)
{
    double rot = radians(angle);
    double cosRot = cos(rot);
    double sinRot = sin(rot);
    double x = v.x() * scale;
    double y = v.y() * scale;
    double rotX = x*cosRot - y*sinRot;
    double rotY = x*sinRot + y*cosRot;
    return QPointF(rotX, rotY);    
}

QPointF
find_mouse_snap_point(QList<QPointF> snap_points, const QPointF& mouse_point)
{
    float closest = 1.0e10;
    QPointF result = snap_points[0];
    int i;
    for (i=0; i<snap_points.count(); i++) {
        float distance = QLineF(snap_points[i], mouse_point).length();
        if (distance < closest) {
            closest = distance;
            result = snap_points[i];
        }
    }
    return result;
}

EmbArc
emb_arc_set_radius(EmbArc arc, EmbReal radius)
{
    radius = EMB_MAX(radius, 0.0000001);
    EmbVector center = emb_arc_center(arc);

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

QIcon
create_icon(QString icon)
{
    QString fname = qApp->applicationDirPath() + "/icons/";
    fname += general_icon_theme.setting;
    fname += "/" + icon + ".png";
    return QIcon(fname);
}

/* . */
ScriptValue
arc_command(ScriptEnv *context)
{
    init_command();
    clear_selection();

    end_command();
    return script_null;
}

#if 0
BaseObject::BaseObject(QGraphicsItem* parent) : QGraphicsPathItem(parent)
{
    qDebug("BaseObject Constructor()");

    objPen.setCapStyle(Qt::RoundCap);
    objPen.setJoinStyle(Qt::RoundJoin);
    lwtPen.setCapStyle(Qt::RoundCap);
    lwtPen.setJoinStyle(Qt::RoundJoin);

    objID = QDateTime::currentMSecsSinceEpoch();

    geometry = (EmbGeometry*)malloc(sizeof(EmbGeometry));
}

void
BaseObject::drawRubberLine(const QLineF& rubLine, QPainter* painter, const char* colorFromScene)
{
    if (painter) {
        QGraphicsScene* objScene = scene();
        if (!objScene) {
            return;
        }
        QPen colorPen = objPen;
        colorPen.setColor(QColor(objScene->property(colorFromScene).toUInt()));
        painter->setPen(colorPen);
        painter->drawLine(rubLine);
        painter->setPen(objPen);
    }
}

void
BaseObject::realRender(QPainter* painter, const QPainterPath& renderPath)
{
    QColor color1 = objPen.color(); /* lighter color */
    QColor color2  = color1.darker(150); /* darker color */

    /* If we have a dark color, lighten it. */
    int darkness = color1.lightness();
    int threshold = 32; //TODO: This number may need adjusted or maybe just add it to settings.
    if (darkness < threshold) {
        color2 = color1;
        if (!darkness) {
            color1 = QColor(threshold, threshold, threshold);
        } //lighter() does not affect pure black
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
#endif

/* . */
ScriptValue
circle_command(ScriptEnv *context)
{
    _main->debug_message("ADDING CIRCLE");
    
    init_command();
    clear_selection();

    /*
    context->point1.x = 0.0f;
    context->point1.y = 0.0f;
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
        emb_array_addCircle(window->pattern->geometry, c);
    }
    // _main->nativeAddCircle(0.0, 0.0, 10.0, true, OBJ_RUBBER_CIRCLE_1P_DIA);
   
    end_command();
    return script_null;
}

ScriptValue
circle_click(ScriptEnv *context)
{
    switch (context->mode) {
    case CIRCLE_MODE_1P_RAD_ONE: {
    #if 0
        context->point1.x = x;
        context->point1.y = y;
        context->center.x = x;
        context->center.y = y;
        addRubber("CIRCLE");
        setRubberMode("CIRCLE_1P_RAD");
        setRubberPoint("CIRCLE_CENTER", x, y);
        prompt_output(translate("Specify radius of circle or [Diameter]: "));
        break;
    }
    case CIRCLE_MODE_1P_RAD_TWO: {
        context->point2 = v;
        setRubberPoint("CIRCLE_RADIUS", context->x2, context->y2);
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
        setRubberPoint("CIRCLE_DIAMETER", context->x2, context->y2);
        vulcanize();
        end_command();
        break;
    }
    case MODE_2P: {
        if (isNaN(context->x1)) {
            context->point1 = v;
            addRubber("CIRCLE");
            setRubberMode("CIRCLE_2P");
            setRubberPoint("CIRCLE_TAN1", x, y);
            prompt_output(translate("Specify second end point of circle's diameter: "));
        }
        else if (isNaN(context->x2)) {
            context->point2 = v;
            setRubberPoint("CIRCLE_TAN2", x, y);
            vulcanize();
            end_command();
        }
        else {
            error("CIRCLE", translate("This should never happen."));
        }
        break;
    }
    case CIRCLE_MODE_3P: {
        if (isNaN(context->x1)) {
            context->point1 = v;
            prompt_output(translate("Specify second point on circle: "));
        }
        else if (isNaN(context->x2)) {
            context->point2 = v;
            addRubber("CIRCLE");
            setRubberMode("CIRCLE_3P");
            setRubberPoint("CIRCLE_TAN1", context->x1, context->y1);
            setRubberPoint("CIRCLE_TAN2", context->x2, context->y2);
            prompt_output(translate("Specify third point on circle: "));
        }
        else if (isNaN(context->x3)) {
            context->point3 = v;
            setRubberPoint("CIRCLE_TAN3", context->x3, context->y3);
            vulcanize();
            end_command();
        }
        else {
            error("CIRCLE", translate("This should never happen."));
        }
        break;
    }
    case MODE_TTR: {
        if (isNaN(context->x1)) {
            context->point1 = v;
            prompt_output(translate("Specify point on object for second tangent of circle: "));
        }
        else if (isNaN(context->x2)) {
            context->point2 = v;
            prompt_output(translate("Specify radius of circle: "));
        }
        else if (isNaN(context->x3)) {
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
circle_context(ScriptEnv *context)
{
    // todo("CIRCLE", "context()");
    end_command();
    return script_null;
}

ScriptValue
circle_prompt(ScriptEnv *context)
{
    switch (context->mode) {
    case CIRCLE_MODE_1P_RAD_ONE: {
    #if 0
        if (isNaN(context->x1)) {
            if (str == "2P") //TODO: Probably should add additional translate calls here.
            {
                context->mode = context->mode_2P;
                prompt_output(translate("Specify first end point of circle's diameter: "));
            }
            else if (str == "3P") //TODO: Probably should add additional translate calls here.
            {
                context->mode = context->mode_3P;
                prompt_output(translate("Specify first point of circle: "));
            }
            else if (str == "T" || str == "TTR") //TODO: Probably should add additional translate calls here.
            {
                context->mode = context->mode_TTR;
                prompt_output(translate("Specify point on object for first tangent of circle: "));
            }
            else {
                var strList = str.split(",");
                if (isNaN(strList[0]) || isNaN(strList[1])) {
                    alert(translate("Point or option keyword required."));
                    prompt_output(translate("Specify center point for circle or [3P/2P/Ttr (tan tan radius)]: "));
                }
                else {
                    context->x1 = Number(strList[0]);
                    context->y1 = Number(strList[1]);
                    context->cx = context->x1;
                    context->cy = context->y1;
                    addRubber("CIRCLE");
                    setRubberMode("CIRCLE_1P_RAD");
                    setRubberPoint("CIRCLE_CENTER", context->cx, context->cy);
                    prompt_output(translate("Specify radius of circle or [Diameter]: "));
                }
            }
        }
        else {
            if (str == "D" || str == "DIAMETER") //TODO: Probably should add additional translate calls here.
            {
                context->mode = context->mode_1P_DIA;
                setRubberMode("CIRCLE_1P_DIA");
                prompt_output(translate("Specify diameter of circle: "));
            }
            else {
                var num = Number(str);
                if (isNaN(num)) {
                    alert(translate("Requires numeric radius, point on circumference, or \"D\"."));
                    prompt_output(translate("Specify radius of circle or [Diameter]: "));
                }
                else {
                    context->rad = num;
                    context->x2 = context->x1 + context->rad;
                    context->y2 = context->y1;
                    setRubberPoint("CIRCLE_RADIUS", context->x2, context->y2);
                    vulcanize();
                    endCommand();
                }
            }
        }
    }
    case CIRCLE_MODE_1P_DIA_ONE: {
        if (isNaN(context->x1)) {
            error("CIRCLE", translate("This should never happen."));
        }
        if (isNaN(context->x2)) {
            var num = Number(str);
            if (isNaN(num)) {
                alert(translate("Requires numeric distance or second point."));
                prompt_output(translate("Specify diameter of circle: "));
            }
            else {
                context->dia = num;
                context->x2 = context->x1 + context->dia;
                context->y2 = context->y1;
                setRubberPoint("CIRCLE_DIAMETER", context->x2, context->y2);
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
        if (isNaN(context->x1)) {
            if (!parse_vector(str, &v)) {
                alert(translate("Invalid point."));
                prompt_output(translate("Specify first end point of circle's diameter: "));
            }
            else {
                context->point1 = v;
                addRubber("CIRCLE");
                setRubberMode("CIRCLE_2P");
                setRubberPoint("CIRCLE_TAN1", context->x1, context->y1);
                prompt_output(translate("Specify second end point of circle's diameter: "));
            }
        }
        else if (isNaN(context->x2)) {
            if (!parse_vector(str, &v)) {
                alert(translate("Invalid point."));
                prompt_output(translate("Specify second end point of circle's diameter: "));
            }
            else {
                context->point2 = v;
                setRubberPoint("CIRCLE_TAN2", context->x2, context->y2);
                vulcanize();
                end_command();
            }
        }
        else {
            error("CIRCLE", translate("This should never happen."));
        }
    }
    case MODE_3P: {
        if (isNaN(context->x1)) {
            if (!parse_vector(str, &v)) {
                alert(translate("Invalid point."));
                prompt_output(translate("Specify first point of circle: "));
            }
            else {
                context->point1 = v;
                prompt_output(translate("Specify second point of circle: "));
            }
        }
        else if (isNaN(context->x2)) {
            if (!parse_vector(str, &v)) {
                alert(translate("Invalid point."));
                prompt_output(translate("Specify second point of circle: "));
            }
            else {
                context->point2 = v;
                addRubber("CIRCLE");
                setRubberMode("CIRCLE_3P");
                setRubberPoint("CIRCLE_TAN1", context->x1, context->y1);
                setRubberPoint("CIRCLE_TAN2", context->x2, context->y2);
                prompt_output(translate("Specify third point of circle: "));
            }
        }
        else if (isNaN(context->x3)) {
            if (!parse_vector(str, &v)) {
                alert(translate("Invalid point."));
                prompt_output(translate("Specify third point of circle: "));
            }
            else {
                context->point3 = v;
                setRubberPoint("CIRCLE_TAN3", context->x3, context->y3);
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

#if 0
/* . */
Object::Object(double centerX, double centerY, double radius, QRgb rgb, QGraphicsItem* parent)
{
    qDebug("CircleObject Constructor()");
    init(centerX, centerY, radius, rgb, Qt::SolidLine); //TODO: getCurrentLineType
}

/* . */
Object::Object(CircleObject* obj, QGraphicsItem* parent)
{
    qDebug("CircleObject Constructor()");
    if (obj) {
        init(obj->objectCenter().x(), obj->objectCenter().y(), obj->objectRadius(), obj->objectColorRGB(), Qt::SolidLine);
        /* TODO: getCurrentLineType. */
        setRotation(obj->rotation());
    }
}

/* . */
//WARNING: DO NOT enable QGraphicsItem::ItemIsMovable. If it is enabled,
//WARNING: and the item is double clicked, the scene will erratically move the item while zooming.
//WARNING: All movement has to be handled explicitly by us, not by the scene.
void
Object::init(double centerX, double centerY, double radius, QRgb rgb, Qt::PenStyle lineType)
{
    setData(OBJ_TYPE, type());
    setData(OBJ_NAME, OBJ_NAME_CIRCLE);

    setFlag(QGraphicsItem::ItemIsSelectable, true);

    EmbVector center;
    center.x = centerX;
    center.y = centerY;
    setObjectRadius(radius);
    setObjectCenter(center);
    setObjectColor(rgb);
    setObjectLineType(lineType);
    setObjectLineWeight(0.35); //TODO: pass in proper lineweight
    setPen(objPen);
    updatePath();
}

void
Object::updatePath()
{
    switch (geometry->type) {
    case EMB_CIRCLE: {
        QPainterPath path;
        QRectF r = rect();
        //Add the center point
        path.addRect(-0.00000001, -0.00000001, 0.00000002, 0.00000002);
        //Add the circle
        path.arcMoveTo(r, 0);
        path.arcTo(r, 0, 360);
        //NOTE: Reverse the path so that the inside area isn't considered part of the circle
        path.arcTo(r, 0, -360);
        setObjectPath(path);
        break;
    }
    default:
        break;
    }
}

void
CircleObject::updateRubber(QPainter* painter)
{
    int rubberMode = objectRubberMode();
    switch (rubberMode) {
    case OBJ_RUBBER_CIRCLE_1P_RAD: {
        QPointF sceneCenterPoint = objectRubberPoint("CIRCLE_CENTER");
        QPointF sceneQSnapPoint = objectRubberPoint("CIRCLE_RADIUS");
        QPointF itemCenterPoint = mapFromScene(sceneCenterPoint);
        QPointF itemQSnapPoint = mapFromScene(sceneQSnapPoint);
        QLineF itemLine(itemCenterPoint, itemQSnapPoint);
        setObjectCenter(to_emb_vector(sceneCenterPoint));
        QLineF sceneLine(sceneCenterPoint, sceneQSnapPoint);
        double radius = sceneLine.length();
        setObjectRadius(radius);
        if (painter) {
            drawRubberLine(itemLine, painter, "VIEW_COLOR_CROSSHAIR");
        }
        updatePath();
        break;
    }
    case OBJ_RUBBER_CIRCLE_1P_DIA: {
        QPointF sceneCenterPoint = objectRubberPoint("CIRCLE_CENTER");
        QPointF sceneQSnapPoint = objectRubberPoint("CIRCLE_DIAMETER");
        QPointF itemCenterPoint = mapFromScene(sceneCenterPoint);
        QPointF itemQSnapPoint = mapFromScene(sceneQSnapPoint);
        QLineF itemLine(itemCenterPoint, itemQSnapPoint);
        setObjectCenter(to_emb_vector(sceneCenterPoint));
        QLineF sceneLine(sceneCenterPoint, sceneQSnapPoint);
        double diameter = sceneLine.length();
        setObjectDiameter(diameter);
        if (painter) {
            drawRubberLine(itemLine, painter, "VIEW_COLOR_CROSSHAIR");
        }
        updatePath();
        break;
    }
    case OBJ_RUBBER_CIRCLE_2P: {
        QPointF sceneTan1Point = objectRubberPoint("CIRCLE_TAN1");
        QPointF sceneQSnapPoint = objectRubberPoint("CIRCLE_TAN2");
        QLineF sceneLine(sceneTan1Point, sceneQSnapPoint);
        setObjectCenter(to_emb_vector(sceneLine.pointAt(0.5)));
        double diameter = sceneLine.length();
        setObjectDiameter(diameter);
        updatePath();
        break;
    }
    case OBJ_RUBBER_CIRCLE_3P: {
        QPointF sceneTan1Point = objectRubberPoint("CIRCLE_TAN1");
        QPointF sceneTan2Point = objectRubberPoint("CIRCLE_TAN2");
        QPointF sceneTan3Point = objectRubberPoint("CIRCLE_TAN3");

        EmbArc arc;
        arc.start = to_emb_vector(sceneTan1Point);
        arc.mid = to_emb_vector(sceneTan2Point);
        arc.end = to_emb_vector(sceneTan3Point);
        EmbVector center = emb_arc_center(arc);
        setObjectCenter(center);
        double radius = emb_vector_distance(center, to_emb_vector(sceneTan3Point));
        setObjectRadius(radius);
        updatePath();
        break;
    }
    case OBJ_RUBBER_GRIP: {
        if (painter) {
            QPointF gripPoint = objectRubberPoint("GRIP_POINT");
            if (gripPoint == objectCenter()) {
                painter->drawEllipse(rect().translated(mapFromScene(objectRubberPoint(QString()))-mapFromScene(gripPoint)));
            }
            else {
                double gripRadius = QLineF(objectCenter(), objectRubberPoint(QString())).length();
                painter->drawEllipse(QPointF(), gripRadius, gripRadius);
            }

            QLineF rubLine(mapFromScene(gripPoint), mapFromScene(objectRubberPoint(QString())));
            drawRubberLine(rubLine, painter, "VIEW_COLOR_CROSSHAIR");
        }
        break;
    }
    default:
        break;
    }
}

QPainterPath
CircleObject::objectSavePath() const
{
    switch (geometry->type) {
    case EMB_CIRCLE: {
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
    default:
        break;
    }
    QPainterPath path;
    return path;
}

DimLeaderObject::DimLeaderObject(double x1, double y1, double x2, double y2, QRgb rgb, QGraphicsItem* parent) : BaseObject(parent)
{
    qDebug("DimLeaderObject Constructor()");
    init(x1, y1, x2, y2, rgb, Qt::SolidLine); //TODO: getCurrentLineType
}

DimLeaderObject::DimLeaderObject(DimLeaderObject* obj, QGraphicsItem* parent) : BaseObject(parent)
{
    qDebug("DimLeaderObject Constructor()");
    if (obj) {
        init(obj->objectX1(), obj->objectY1(), obj->objectX2(), obj->objectY2(), obj->objectColorRGB(), Qt::SolidLine); //TODO: getCurrentLineType
    }
}

// WARNING: DO NOT enable QGraphicsItem::ItemIsMovable. If it is enabled,
// WARNING: and the item is double clicked, the scene will erratically move the item while zooming.
// WARNING: All movement has to be handled explicitly by us, not by the scene.
void
DimLeaderObject::init(double x1, double y1, double x2, double y2, QRgb rgb, Qt::PenStyle lineType)
{
    setData(OBJ_TYPE, type());
    setData(OBJ_NAME, OBJ_NAME_DIMLEADER);

    setFlag(QGraphicsItem::ItemIsSelectable, true);

    curved = false;
    filled = true;
    setObjectEndPoint1(x1, y1);
    setObjectEndPoint2(x2, y2);
    setObjectColor(rgb);
    setObjectLineType(lineType);
    setObjectLineWeight(0.35); //TODO: pass in proper lineweight
    setPen(objPen);
}

QPointF DimLeaderObject::objectEndPoint1() const
{
    return scenePos();
}

QPointF DimLeaderObject::objectMidPoint() const
{
    QLineF lyne = line();
    QPointF mp = lyne.pointAt(0.5);
    return scenePos() + scale_and_rotate(mp, scale(), rotation());
}

void
DimLeaderObject::updateLeader()
{
    int arrowStyle = Closed; //TODO: Make this customizable
    double arrowStyleAngle = 15.0; //TODO: Make this customizable
    double arrowStyleLength = 1.0; //TODO: Make this customizable
    double lineStyleAngle = 45.0; //TODO: Make this customizable
    double lineStyleLength = 1.0; //TODO: Make this customizable

    QLineF lyne = line();
    double angle = lyne.angle();
    QPointF ap0 = lyne.p1();
    QPointF lp0 = lyne.p2();

    //Arrow
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

    //Math Diagram
    //                 .(ap1)                     .(lp1)
    //                /|                         /|
    //               / |                        / |
    //              /  |                       /  |
    //             /   |                      /   |
    //            /    |                     /    |
    //           /     |                    /     |
    //          /      |                   /      |
    //         /       |                  /       |
    //        /+(aSA)  |                 /+(lSA)  |
    // (ap0)./__(aSL)__|__________(lp0)./__(lSL)__|
    //       \ -(aSA)  |                \ -(lSA)  |
    //        \        |                 \        |
    //         \       |                  \       |
    //          \      |                   \      |
    //           \     |                    \     |
    //            \    |                     \    |
    //             \   |                      \   |
    //              \  |                       \  |
    //               \ |                        \ |
    //                \|                         \|
    //                 .(ap2)                     .(lp2)

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

void
DimLeaderObject::updateRubber(QPainter* painter)
{
    int rubberMode = objectRubberMode();
    switch (rubberMode) {
    case OBJ_RUBBER_DIMLEADER_LINE: {
        QPointF sceneStartPoint = objectRubberPoint("DIMLEADER_LINE_START");
        QPointF sceneQSnapPoint = objectRubberPoint("DIMLEADER_LINE_END");

        setObjectEndPoint1(sceneStartPoint);
        setObjectEndPoint2(sceneQSnapPoint);
        break;
    }
    case OBJ_RUBBER_GRIP: {
        if (painter) {
            QPointF gripPoint = objectRubberPoint("GRIP_POINT");
            if (gripPoint == objectEndPoint1())
                painter->drawLine(line().p2(), mapFromScene(objectRubberPoint(QString())));
            else if (gripPoint == objectEndPoint2())
                painter->drawLine(line().p1(), mapFromScene(objectRubberPoint(QString())));
            else if (gripPoint == objectMidPoint())
                painter->drawLine(line().translated(mapFromScene(objectRubberPoint(QString()))-mapFromScene(gripPoint)));
        }
        break;
    }
    default:
        break;
    }
}
#endif

/* ELLIPSE */
ScriptValue
ellipse_command(ScriptEnv *context)
{
    switch (context->mode) {
    default:
    case CONTEXT_MAIN:
        init_command();
        clear_selection();

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
context->mode;

function main()
{
    init_command();
    clear_selection();
    context->mode = ELLIPSE_MODE_MAJDIA_MINRAD_ONE;
    context->point1 = zero_vector;
    context->point2 = zero_vector;
    context->point3 = zero_vector;
    prompt_output(translate("Specify first axis start point or [Center]: "));
}

function click(EmbVector v)
{
    switch (context->mode) {
    case ELLIPSE_MODE_MAJDIA_MINRAD: {
        if (isNaN(context->x1)) {
            context->point1 = v;
            addRubber("ELLIPSE");
            setRubberMode("ELLIPSE_LINE");
            setRubberPoint("ELLIPSE_LINE_POINT1", context->x1, context->y1);
            prompt_output(translate("Specify first axis end point: "));
        }
        else if (isNaN(context->x2)) {
            context->point2 = v;
            context->cx = (context->x1 + context->x2)/2.0;
            context->cy = (context->y1 + context->y2)/2.0;
            context->width = calculateDistance(context->x1, context->y1, context->x2, context->y2);
            context->rot = calculateAngle(context->x1, context->y1, context->x2, context->y2);
            setRubberMode("ELLIPSE_MAJDIA_MINRAD");
            setRubberPoint("ELLIPSE_AXIS1_POINT1", context->x1, context->y1);
            setRubberPoint("ELLIPSE_AXIS1_POINT2", context->x2, context->y2);
            setRubberPoint("ELLIPSE_CENTER", context->cx, context->cy);
            setRubberPoint("ELLIPSE_WIDTH", context->width, 0);
            setRubberPoint("ELLIPSE_ROT", context->rot, 0);
            prompt_output(translate("Specify second axis end point or [Rotation]: "));
        }
        else if (isNaN(context->x3)) {
            context->x3 = x;
            context->y3 = y;
            context->height = perpendicularDistance(context->x3, context->y3, context->x1, context->y1, context->x2, context->y2)*2.0;
            setRubberPoint("ELLIPSE_AXIS2_POINT2", context->x3, context->y3);
            vulcanize();
            end_command();
        }
        else {
            error("ELLIPSE", translate("This should never happen."));
        }
        break;
    }
    case ELLIPSE_MODE_MAJRAD_MINRAD: {
        if (isNaN(context->x1)) {
            context->point1 = v;
            context->center = context->point1;
            addRubber("ELLIPSE");
            setRubberMode("ELLIPSE_LINE");
            setRubberPoint("ELLIPSE_LINE_POINT1", context->x1, context->y1);
            setRubberPoint("ELLIPSE_CENTER", context->cx, context->cy);
            prompt_output(translate("Specify first axis end point: "));
        }
        else if (isNaN(context->x2)) {
            context->point2 = v;
            context->width = calculateDistance(context->cx, context->cy, context->x2, context->y2)*2.0;
            context->rot = calculateAngle(context->x1, context->y1, context->x2, context->y2);
            setRubberMode("ELLIPSE_MAJRAD_MINRAD");
            setRubberPoint("ELLIPSE_AXIS1_POINT2", context->x2, context->y2);
            setRubberPoint("ELLIPSE_WIDTH", context->width, 0);
            setRubberPoint("ELLIPSE_ROT", context->rot, 0);
            prompt_output(translate("Specify second axis end point or [Rotation]: "));
        }
        else if (isNaN(context->x3)) {
            context->point3 = v;
            context->height = perpendicularDistance(context->x3, context->y3, context->cx, context->cy, context->x2, context->y2)*2.0;
            setRubberPoint("ELLIPSE_AXIS2_POINT2", context->x3, context->y3);
            vulcanize();
            end_command();
        }
        else {
            error("ELLIPSE", translate("This should never happen."));
        }
        break;
    }
    case ELLIPSE_MODE_ROTATION: {
        if (isNaN(context->x1)) {
            error("ELLIPSE", translate("This should never happen."));
        }
        else if (isNaN(context->x2)) {
            error("ELLIPSE", translate("This should never happen."));
        }
        else if (isNaN(context->x3)) {
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
            addRubber("ELLIPSE");
            setRubberMode("ELLIPSE_LINE");
            setRubberPoint("ELLIPSE_LINE_POINT1", context->x1, context->y1);
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
            setRubberMode("ELLIPSE_MAJDIA_MINRAD");
            setRubberPoint("ELLIPSE_AXIS1_POINT1", context->x1, context->y1);
            setRubberPoint("ELLIPSE_AXIS1_POINT2", context->x2, context->y2);
            setRubberPoint("ELLIPSE_CENTER", context->cx, context->cy);
            setRubberPoint("ELLIPSE_WIDTH", context->width, 0);
            setRubberPoint("ELLIPSE_ROT", context->rot, 0);
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
            setRubberPoint("ELLIPSE_AXIS2_POINT2", context->x3, context->y3);
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
            addRubber("ELLIPSE");
            setRubberMode("ELLIPSE_LINE");
            setRubberPoint("ELLIPSE_LINE_POINT1", context->x1, context->y1);
            setRubberPoint("ELLIPSE_CENTER", context->cx, context->cy);
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
            setRubberMode("ELLIPSE_MAJRAD_MINRAD");
            setRubberPoint("ELLIPSE_AXIS1_POINT2", context->x2, context->y2);
            setRubberPoint("ELLIPSE_WIDTH", context->width, 0);
            setRubberPoint("ELLIPSE_ROT", context->rot, 0);
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
                context->height = perpendicularDistance(context->x3, context->y3, context->x1, context->y1, context->x2, context->y2)*2.0;
                setRubberPoint("ELLIPSE_AXIS2_POINT2", context->x3, context->y3);
                vulcanize();
                end_command();
            }
        }
        break;
    }
    case ELLIPSE_MODE_ROTATION: {
        if (isNaN(str)) {
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

EllipseObject::EllipseObject(double centerX, double centerY, double width, double height, QRgb rgb, QGraphicsItem* parent) : BaseObject(parent)
{
    qDebug("EllipseObject Constructor()");
    init(centerX, centerY, width, height, rgb, Qt::SolidLine); //TODO: getCurrentLineType
}

EllipseObject::EllipseObject(EllipseObject* obj, QGraphicsItem* parent) : BaseObject(parent)
{
    qDebug("EllipseObject Constructor()");
    if (obj) {
        init(obj->objectCenterX(), obj->objectCenterY(), obj->objectWidth(), obj->objectHeight(), obj->objectColorRGB(), Qt::SolidLine); //TODO: getCurrentLineType
        setRotation(obj->rotation());
    }
}

void
EllipseObject::init(double centerX, double centerY, double width, double height, QRgb rgb, Qt::PenStyle lineType)
{
    setData(OBJ_TYPE, type());
    setData(OBJ_NAME, OBJ_NAME_ELLIPSE);

    //WARNING: DO NOT enable QGraphicsItem::ItemIsMovable. If it is enabled,
    //WARNING: and the item is double clicked, the scene will erratically move the item while zooming.
    //WARNING: All movement has to be handled explicitly by us, not by the scene.
    setFlag(QGraphicsItem::ItemIsSelectable, true);

    setObjectSize(width, height);
    setObjectCenter(centerX, centerY);
    setObjectColor(rgb);
    setObjectLineType(lineType);
    setObjectLineWeight(0.35); //TODO: pass in proper lineweight
    setPen(objPen);
    updatePath();
}

void
EllipseObject::setObjectSize(double width, double height)
{
    QRectF elRect = rect();
    elRect.setWidth(width);
    elRect.setHeight(height);
    elRect.moveCenter(QPointF(0,0));
    setRect(elRect);
}

void
EllipseObject::setObjectCenter(const QPointF& center)
{
    setObjectCenter(center.x(), center.y());
}

void
EllipseObject::setObjectCenter(double centerX, double centerY)
{
    setPos(centerX, centerY);
}

void
EllipseObject::setObjectCenterX(double centerX)
{
    setX(centerX);
}

void
EllipseObject::setObjectCenterY(double centerY)
{
    setY(centerY);
}
#endif

void
Object::setObjectRadiusMajor(double radius)
{
    setObjectDiameterMajor(radius*2.0);
}

void
Object::setObjectRadiusMinor(double radius)
{
    setObjectDiameterMinor(radius*2.0);
}

void
Object::setObjectDiameterMajor(double diameter)
{
    QRectF elRect = rect();
    if (elRect.width() > elRect.height()) {
        elRect.setWidth(diameter);
    }
    else {
        elRect.setHeight(diameter);
    }
    elRect.moveCenter(QPointF(0,0));
    setRect(elRect);
}

void
Object::setObjectDiameterMinor(double diameter)
{
    QRectF elRect = rect();
    if (elRect.width() < elRect.height())
        elRect.setWidth(diameter);
    else
        elRect.setHeight(diameter);
    elRect.moveCenter(QPointF(0,0));
    setRect(elRect);
}

#if 0
QPointF EllipseObject::objectQuadrant0() const
{
    double halfW = objectWidth()/2.0;
    double rot = radians(rotation());
    double x = halfW * cos(rot);
    double y = halfW * sin(rot);
    return objectCenter() + QPointF(x,y);
}

QPointF EllipseObject::objectQuadrant90() const
{
    double halfH = objectHeight()/2.0;
    double rot = radians(rotation()+90.0);
    double x = halfH * cos(rot);
    double y = halfH * sin(rot);
    return objectCenter() + QPointF(x,y);
}

QPointF EllipseObject::objectQuadrant180() const
{
    double halfW = objectWidth()/2.0;
    double rot = radians(rotation()+180.0);
    double x = halfW * cos(rot);
    double y = halfW * sin(rot);
    return objectCenter() + QPointF(x,y);
}

QPointF EllipseObject::objectQuadrant270() const
{
    double halfH = objectHeight()/2.0;
    double rot = radians(rotation()+270.0);
    double x = halfH * cos(rot);
    double y = halfH * sin(rot);
    return objectCenter() + QPointF(x,y);
}

void
EllipseObject::updatePath()
{
    QPainterPath path;
    QRectF r = rect();
    path.arcMoveTo(r, 0);
    path.arcTo(r, 0, 360);
    //NOTE: Reverse the path so that the inside area isn't considered part of the ellipse
    path.arcTo(r, 0, -360);
    setObjectPath(path);
}

void
EllipseObject::updateRubber(QPainter* painter)
{
    int rubberMode = objectRubberMode();
    switch (rubberMode) {
    case OBJ_RUBBER_ELLIPSE_LINE: {
        QPointF sceneLinePoint1 = objectRubberPoint("ELLIPSE_LINE_POINT1");
        QPointF sceneLinePoint2 = objectRubberPoint("ELLIPSE_LINE_POINT2");
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
        QPointF sceneAxis1Point1 = objectRubberPoint("ELLIPSE_AXIS1_POINT1");
        QPointF sceneAxis1Point2 = objectRubberPoint("ELLIPSE_AXIS1_POINT2");
        QPointF sceneCenterPoint = objectRubberPoint("ELLIPSE_CENTER");
        QPointF sceneAxis2Point2 = objectRubberPoint("ELLIPSE_AXIS2_POINT2");
        double ellipseWidth = objectRubberPoint("ELLIPSE_WIDTH").x();
        double ellipseRot = objectRubberPoint("ELLIPSE_ROT").x();

        //TODO: incorporate perpendicularDistance() into libcgeometry
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
        QPointF sceneAxis1Point2 = objectRubberPoint("ELLIPSE_AXIS1_POINT2");
        QPointF sceneCenterPoint = objectRubberPoint("ELLIPSE_CENTER");
        QPointF sceneAxis2Point2 = objectRubberPoint("ELLIPSE_AXIS2_POINT2");
        double ellipseWidth = objectRubberPoint("ELLIPSE_WIDTH").x();
        double ellipseRot = objectRubberPoint("ELLIPSE_ROT").x();

        //TODO: incorporate perpendicularDistance() into libcgeometry
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
    case OBJ_RUBBER_GRIP: {
        //TODO: updateRubber() gripping for EllipseObject
        break;
    }
    default:
        break;
    }
}

QPainterPath EllipseObject::objectSavePath() const
{
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

ImageObject::ImageObject(double x, double y, double w, double h, QRgb rgb, QGraphicsItem* parent) : BaseObject(parent)
{
    qDebug("ImageObject Constructor()");
    init(x, y, w, h, rgb, Qt::SolidLine); //TODO: getCurrentLineType
}

Object::Object(Object* obj, QGraphicsItem* parent)
{
    qDebug("Object Constructor()");
    if (!obj) {
        return;
    }
    switch (geometry->type) {
    case EMB_IMAGE: {
        QPointF ptl = obj->objectTopLeft();
        init(ptl.x(), ptl.y(), obj->objectWidth(), obj->objectHeight(), obj->objectColorRGB(), Qt::SolidLine); //TODO: getCurrentLineType
        setRotation(obj->rotation());
        break;
    }
    case EMB_LINE: {
        init(obj->objectX1(), obj->objectY1(), obj->objectX2(), obj->objectY2(), obj->objectColorRGB(), Qt::SolidLine); //TODO: getCurrentLineType
        break;
    }
    case EMB_PATH: {
        init(obj->objectX(), obj->objectY(), obj->objectCopyPath(), obj->objectColorRGB(), Qt::SolidLine); //TODO: getCurrentLineType
        setRotation(obj->rotation());
        setScale(obj->scale());
        break;
    }
    case EMB_POINT: {
        init(obj->objectX(), obj->objectY(), obj->objectColorRGB(), Qt::SolidLine); //TODO: getCurrentLineType
        setRotation(obj->rotation());
        break;
    }
    case EMB_POLYGON: {
        init(obj->objectX(), obj->objectY(), obj->objectCopyPath(), obj->objectColorRGB(), Qt::SolidLine); //TODO: getCurrentLineType
        setRotation(obj->rotation());
        setScale(obj->scale());
        break;
    }
    case EMB_POLYLINE: {
        init(obj->objectX(), obj->objectY(), obj->objectCopyPath(), obj->objectColorRGB(), Qt::SolidLine); //TODO: getCurrentLineType
        setRotation(obj->rotation());
        setScale(obj->scale());
        break;
    }
    case EMB_RECT: {
        QPointF ptl = obj->objectTopLeft();
        init(ptl.x(), ptl.y(), obj->objectWidth(), obj->objectHeight(), obj->objectColorRGB(), Qt::SolidLine); //TODO: getCurrentLineType
        setRotation(obj->rotation());
        break;
    }
    default:
        break;
    }
}

//WARNING: DO NOT enable QGraphicsItem::ItemIsMovable. If it is enabled,
//WARNING: and the item is double clicked, the scene will erratically move the item while zooming.
//WARNING: All movement has to be handled explicitly by us, not by the scene.
void
ImageObject::init(double x, double y, double w, double h, QRgb rgb, Qt::PenStyle lineType)
{
    setData(OBJ_TYPE, type());
    setData(OBJ_NAME, OBJ_NAME_IMAGE);

    setFlag(QGraphicsItem::ItemIsSelectable, true);

    setObjectRect(x, y, w, h);
    setObjectColor(rgb);
    setObjectLineType(lineType);
    setObjectLineWeight(0.35); //TODO: pass in proper lineweight
    setPen(objPen);
}

void
ImageObject::updatePath()
{
    QPainterPath path;
    QRectF r = rect();
    path.moveTo(r.bottomLeft());
    path.lineTo(r.bottomRight());
    path.lineTo(r.topRight());
    path.lineTo(r.topLeft());
    path.lineTo(r.bottomLeft());
    //NOTE: Reverse the path so that the inside area isn't considered part of the rectangle
    path.lineTo(r.topLeft());
    path.lineTo(r.topRight());
    path.lineTo(r.bottomRight());
    path.moveTo(r.bottomLeft());
    setObjectPath(path);
}

void
ImageObject::updateRubber(QPainter* painter)
{
    int rubberMode = objectRubberMode();
    if (rubberMode == OBJ_RUBBER_IMAGE) {
        QPointF sceneStartPoint = objectRubberPoint("IMAGE_START");
        QPointF sceneEndPoint = objectRubberPoint("IMAGE_END");
        double x = sceneStartPoint.x();
        double y = sceneStartPoint.y();
        double w = sceneEndPoint.x() - sceneStartPoint.x();
        double h = sceneEndPoint.y() - sceneStartPoint.y();
        setObjectRect(x,y,w,h);
        updatePath();
    }
    else if (rubberMode == OBJ_RUBBER_GRIP) {
        //TODO: updateRubber() gripping for ImageObject
    }
}
#endif

/* LINE */
ScriptValue
line_command(ScriptEnv *context)
{
    switch (context->context) {
    case CONTEXT_MAIN: {
        init_command();
        clear_selection();
        /*
        context->firstRun = true;
        context->first = zero_vector;
        context->prev = zero_vector;
        */
        prompt_output(translate("Specify first point: "));
        break;
    }
    default:
        break;
    }
    return script_null;
}

#if 0
//Command: Line

var global = {}; //Required
context->firstRun;
context->first;
context->prev;

function main()
{
}

function
click(EmbVector v)
{
    if (context->firstRun) {
        context->firstRun = false;
        context->first = v;
        context->prev = v;
        addRubber("LINE");
        setRubberMode("LINE");
        setRubberPoint("LINE_START", context->first.x, context->first.y);
        prompt_output(translate("Specify next point or [Undo]: "));
    }
    else {
        setRubberPoint("LINE_END", v.x, v.y);
        vulcanize();
        addRubber("LINE");
        setRubberMode("LINE");
        setRubberPoint("LINE_START", v.x, v.y);
        context->prev = v;
    }
}

function context(str)
{
    todo("LINE", "context()");
}

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
            addRubber("LINE");
            setRubberMode("LINE");
            setRubberPoint("LINE_START", context->first.x, context->first.y);
            prompt_output(translate("Specify next point or [Undo]: "));
        }
    }
    else {
        if (str == "U" || str == "UNDO") {
            /* TODO: Probably should add additional translate calls here. */
            todo("LINE", "prompt() for UNDO");
        }
        else {
            if (!parse_vector(str, &v)) {
                alert(translate("Point or option keyword required."));
                prompt_output(translate("Specify next point or [Undo]: "));
            }
            else {
                setRubberPoint("LINE_END", v.x, v.y);
                vulcanize();
                addRubber("LINE");
                setRubberMode("LINE");
                setRubberPoint("LINE_START", v.x, v.y);
                context->prev = v;
                prompt_output(translate("Specify next point or [Undo]: "));
            }
        }
    }
}

LineObject::LineObject(double x1, double y1, double x2, double y2, QRgb rgb, QGraphicsItem* parent) : BaseObject(parent)
{
    qDebug("LineObject Constructor()");
    init(x1, y1, x2, y2, rgb, Qt::SolidLine); //TODO: getCurrentLineType
}

//WARNING: DO NOT enable QGraphicsItem::ItemIsMovable. If it is enabled,
//WARNING: and the item is double clicked, the scene will erratically move the item while zooming.
//WARNING: All movement has to be handled explicitly by us, not by the scene.
void
LineObject::init(double x1, double y1, double x2, double y2, QRgb rgb, Qt::PenStyle lineType)
{
    setData(OBJ_TYPE, type());
    setData(OBJ_NAME, OBJ_NAME_LINE);

    setFlag(QGraphicsItem::ItemIsSelectable, true);

    setObjectEndPoint1(x1, y1);
    setObjectEndPoint2(x2, y2);
    setObjectColor(rgb);
    setObjectLineType(lineType);
    setObjectLineWeight(0.35); //TODO: pass in proper lineweight
    setPen(objPen);
}

void
LineObject::updateRubber(QPainter* painter)
{
    int rubberMode = objectRubberMode();
    if (rubberMode == OBJ_RUBBER_LINE) {
        QPointF sceneStartPoint = objectRubberPoint("LINE_START");
        QPointF sceneQSnapPoint = objectRubberPoint("LINE_END");

        setObjectEndPoint1(sceneStartPoint);
        setObjectEndPoint2(sceneQSnapPoint);

        drawRubberLine(line(), painter, "VIEW_COLOR_CROSSHAIR");
    }
    else if (rubberMode == OBJ_RUBBER_GRIP) {
        if (painter) {
            QPointF gripPoint = objectRubberPoint("GRIP_POINT");
            if (gripPoint == objectEndPoint1())
                painter->drawLine(line().p2(), mapFromScene(objectRubberPoint(QString())));
            else if (gripPoint == objectEndPoint2())
                painter->drawLine(line().p1(), mapFromScene(objectRubberPoint(QString())));
            else if (gripPoint == objectMidPoint())
                painter->drawLine(line().translated(mapFromScene(objectRubberPoint(QString()))-mapFromScene(gripPoint)));

            QLineF rubLine(mapFromScene(gripPoint), mapFromScene(objectRubberPoint(QString())));
            drawRubberLine(rubLine, painter, "VIEW_COLOR_CROSSHAIR");
        }
    }
}

QPainterPath
LineObject::objectSavePath() const
{
    QPainterPath path;
    path.lineTo(objectDeltaX(), objectDeltaY());
    return path;
}
#endif

/* PATH */
ScriptValue
path_command(ScriptEnv * /* context */)
{
    return script_null;
}

#if 0
//Command: Path

//TODO: The path command is currently broken
var global = {}; //Required
bool firstRun;
EmbVector first;
EmbVector prev;

function main()
{
    initCommand();
    clearSelection();
    context->firstRun = true;
    context->firstX = NaN;
    context->firstY = NaN;
    context->prevX = NaN;
    context->prevY = NaN;
    prompt_output(translate("Specify start point: "));
}

function click(EmbVector v)
{
    if (context->firstRun) {
        context->firstRun = false;
        context->first = v;
        context->prev = v;
        addPath(v.x, v.y);
        prompt_output(translate("Specify next point or [Arc/Undo]: "));
    }
    else {
        appendLineToPath(x,y);
        context->prev = v;
    }
}

function
context(str)
{
    todo("PATH", "context()");
}

function
prompt(str)
{
    if (str == "A" || str == "ARC") {
        //TODO: Probably should add additional translate calls here.
        todo("PATH", "prompt() for ARC");
    }
    else if (str == "U" || str == "UNDO") {
        //TODO: Probably should add additional translate calls here.
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
                addPath(v.x, v.y);
                prompt_output(translate("Specify next point or [Arc/Undo]: "));
            }
            else {
                appendLineToPath(v.x, v.y);
                context->prev = v;
            }
        }
    }
}

PathObject::PathObject(double x, double y, const QPainterPath p, QRgb rgb, QGraphicsItem* parent) : BaseObject(parent)
{
    qDebug("PathObject Constructor()");
    init(x, y, p, rgb, Qt::SolidLine); //TODO: getCurrentLineType
}

//WARNING: DO NOT enable QGraphicsItem::ItemIsMovable. If it is enabled,
//WARNING: and the item is double clicked, the scene will erratically move the item while zooming.
//WARNING: All movement has to be handled explicitly by us, not by the scene.
void
PathObject::init(double x, double y, const QPainterPath& p, QRgb rgb, Qt::PenStyle lineType)
{
    setData(OBJ_TYPE, type());
    setData(OBJ_NAME, OBJ_NAME_PATH);

    setFlag(QGraphicsItem::ItemIsSelectable, true);

    updatePath(p);
    setObjectPos(x,y);
    setObjectColor(rgb);
    setObjectLineType(lineType);
    setObjectLineWeight(0.35); //TODO: pass in proper lineweight
    setPen(objPen);
}

void
PathObject::updatePath(const QPainterPath& p)
{
    normalPath = p;
    QPainterPath reversePath = normalPath.toReversed();
    reversePath.connectPath(normalPath);
    setObjectPath(reversePath);
}

void
PathObject::updateRubber(QPainter* painter)
{
    //TODO: Path Rubber Modes

    //TODO: updateRubber() gripping for PathObject

}

#endif

/* POINT */
ScriptValue
point_command(ScriptEnv * context)
{
    return script_null;
}

#if 0
function main()
{
    init_command();
    clear_selection();
    global->firstRun = true;
    prompt_output("TODO: Current point settings: PDMODE=?  PDSIZE=?"); //TODO: translate needed here when complete
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
            //TODO: Probably should add additional translate calls here.
            todo("POINT", "prompt() for PDMODE");
        }
        else if (str == "S" || str == "SIZE") {
            //TODO: Probably should add additional translate calls here.
            todo("POINT", "prompt() for PDSIZE");
        }
        if (!parse_vector(str, v)) {
            alert(translate("Invalid point."));
            prompt_output(translate("Specify first point: "));
        }
        else {
            context->firstRun = false;
            prompt_output(translate("Specify next point: "));
            addPoint(v.x, v.y);
        }
    }
    else {
        if (!parse_vector(str, v)) {
            alert(translate("Invalid point."));
            prompt_output(translate("Specify next point: "));
        }
        else {
            prompt_output(translate("Specify next point: "));
            addPoint(v.x, v.y);
        }
    }
}

PointObject::PointObject(double x, double y, QRgb rgb, QGraphicsItem* parent) : BaseObject(parent)
{
    qDebug("PointObject Constructor()");
    init(x, y, rgb, Qt::SolidLine); //TODO: getCurrentLineType
}

void
PointObject::init(double x, double y, QRgb rgb, Qt::PenStyle lineType)
{
    setData(OBJ_TYPE, type());
    setData(OBJ_NAME, OBJ_NAME_POINT);

    //WARNING: DO NOT enable QGraphicsItem::ItemIsMovable. If it is enabled,
    //WARNING: and the item is double clicked, the scene will erratically move the item while zooming.
    //WARNING: All movement has to be handled explicitly by us, not by the scene.
    setFlag(QGraphicsItem::ItemIsSelectable, true);

    setRect(-0.00000001, -0.00000001, 0.00000002, 0.00000002);
    setObjectPos(x,y);
    setObjectColor(rgb);
    setObjectLineType(lineType);
    setObjectLineWeight(0.35); //TODO: pass in proper lineweight
    setPen(objPen);
}

void
PointObject::updateRubber(QPainter* painter)
{
    int rubberMode = objectRubberMode();
    if (rubberMode == OBJ_RUBBER_GRIP) {
        if (painter) {
            QPointF gripPoint = objectRubberPoint("GRIP_POINT");
            if (gripPoint == scenePos()) {
                QLineF rubLine(mapFromScene(gripPoint), mapFromScene(objectRubberPoint(QString())));
                drawRubberLine(rubLine, painter, "VIEW_COLOR_CROSSHAIR");
            }
        }
    }
}

QPainterPath PointObject::objectSavePath() const
{
    QPainterPath path;
    path.addRect(-0.00000001, -0.00000001, 0.00000002, 0.00000002);
    return path;
}
#endif

/* POLYGON */
ScriptValue
polygon_command(ScriptEnv * context)
{
    init_command();
    clear_selection();

    end_command();
    return script_null;
}

#if 0
//Command: Polygon

var global = {}; //Required
EmbVector center;
EmbVector side1;
EmbVector side2;
EmbVector pointI;
EmbVector pointC;
char *polyType = "Inscribed"; //Default
int numSides = 4;           //Default
int mode;

function
main()
{
    init_command();
    clear_selection();
    context->centerX = NaN;
    context->centerY = NaN;
    context->sideX1  = NaN;
    context->sideY1  = NaN;
    context->sideX2  = NaN;
    context->sideY2  = NaN;
    context->pointIX = NaN;
    context->pointIY = NaN;
    context->pointCX = NaN;
    context->pointCY = NaN;
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
        //Do nothing, the prompt controls these.
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
        setRubberPoint("POLYGON_INSCRIBE_POINT", context->pointIX, context->pointIY);
        vulcanize();
        end_command();
        break;
    }
    case POLYGON_MODE_CIRCUMSCRIBE: {
        context->pointC = v;
        setRubberPoint("POLYGON_CIRCUMSCRIBE_POINT", context->pointCX, context->pointCY);
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
            if (isNaN(tmp) || !isInt(tmp) || tmp < 3 || tmp > 1024) {
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
            /* TODO: Probably should add additional translate calls here. */
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
            /* TODO: Probably should add additional translate calls here. */
            context->mode = POLYGON_MODE_INSCRIBE;
            context->polyType = "Inscribed";
            prompt_output(translate("Specify polygon corner point or [Distance]: "));
            addRubber("POLYGON");
            setRubberMode("POLYGON_INSCRIBE");
            setRubberPoint("POLYGON_CENTER", context->centerX, context->centerY);
            setRubberPoint("POLYGON_NUM_SIDES", context->numSides, 0);
        }
        else if (str == "C" || str == "CIRCUMSCRIBED") {
            /* TODO: Probably should add additional translate calls here. */
            context->mode = POLYGON_MODE_CIRCUMSCRIBE;
            context->polyType = "Circumscribed";
            prompt_output(translate("Specify polygon side point or [Distance]: "));
            addRubber("POLYGON");
            setRubberMode("POLYGON_CIRCUMSCRIBE");
            setRubberPoint("POLYGON_CENTER", context->centerX, context->centerY);
            setRubberPoint("POLYGON_NUM_SIDES", context->numSides, 0);
        }
        else if (str == "") {
            if (context->polyType == "Inscribed") {
                context->mode = POLYGON_MODE_INSCRIBE;
                prompt_output(translate("Specify polygon corner point or [Distance]: "));
                addRubber("POLYGON");
                setRubberMode("POLYGON_INSCRIBE");
                setRubberPoint("POLYGON_CENTER", context->centerX, context->centerY);
                setRubberPoint("POLYGON_NUM_SIDES", context->numSides, 0);
            }
            else if (context->polyType == "Circumscribed") {
                context->mode = POLYGON_MODE_CIRCUMSCRIBE;
                prompt_output(translate("Specify polygon side point or [Distance]: "));
                addRubber("POLYGON");
                setRubberMode("POLYGON_CIRCUMSCRIBE");
                setRubberPoint("POLYGON_CENTER", context->centerX, context->centerY);
                setRubberPoint("POLYGON_NUM_SIDES", context->numSides, 0);
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
                setRubberPoint("POLYGON_INSCRIBE_POINT", context->pointIX, context->pointIY);
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
                setRubberPoint("POLYGON_CIRCUMSCRIBE_POINT", context->pointCX, context->pointCY);
                vulcanize();
                end_command();
            }
        }
        break;
    }
    case POLYGON_MODE_DISTANCE: {
        if (isNaN(str)) {
            alert(translate("Requires valid numeric distance."));
            prompt_output(translate("Specify distance: "));
        }
        else {
            if (context->polyType == "Inscribed") {
                context->pointI.x = context->center.x;
                context->pointI.y = context->center.y + Number(str);
                setRubberPoint("POLYGON_INSCRIBE_POINT", context->pointIX, context->pointIY);
                vulcanize();
                end_command();
            }
            else if (context->polyType == "Circumscribed") {
                context->pointC.x = context->center.x;
                context->pointC.y = context->center.y + Number(str);
                setRubberPoint("POLYGON_CIRCUMSCRIBE_POINT", context->pointCX, context->pointCY);
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

PolygonObject::PolygonObject(double x, double y, const QPainterPath& p, QRgb rgb, QGraphicsItem* parent) : BaseObject(parent)
{
    qDebug("PolygonObject Constructor()");
    init(x, y, p, rgb, Qt::SolidLine); //TODO: getCurrentLineType
}

void
PolygonObject::init(double x, double y, const QPainterPath& p, QRgb rgb, Qt::PenStyle lineType)
{
    setData(OBJ_TYPE, type());
    setData(OBJ_NAME, OBJ_NAME_POLYGON);

    //WARNING: DO NOT enable QGraphicsItem::ItemIsMovable. If it is enabled,
    //WARNING: and the item is double clicked, the scene will erratically move the item while zooming.
    //WARNING: All movement has to be handled explicitly by us, not by the scene.
    setFlag(QGraphicsItem::ItemIsSelectable, true);

    gripIndex = -1;
    updatePath(p);
    setObjectPos(x,y);
    setObjectColor(rgb);
    setObjectLineType(lineType);
    setObjectLineWeight(0.35); //TODO: pass in proper lineweight
    setPen(objPen);
}

void
PolygonObject::updatePath(const QPainterPath& p)
{
    normalPath = p;
    QPainterPath closedPath = normalPath;
    closedPath.closeSubpath();
    QPainterPath reversePath = closedPath.toReversed();
    reversePath.connectPath(closedPath);
    setObjectPath(reversePath);
}

void
PolygonObject::updateRubber(QPainter* painter)
{
    int rubberMode = objectRubberMode();
    if (rubberMode == OBJ_RUBBER_POLYGON) {
        setObjectPos(objectRubberPoint("POLYGON_POINT_0"));

        bool ok = false;
        QString numStr = objectRubberText("POLYGON_NUM_POINTS");
        if (numStr.isNull()) {
            return;
        }
        int num = numStr.toInt(&ok);
        if (!ok) {
            return;
        }

        QString appendStr;
        QPainterPath rubberPath;
        rubberPath.moveTo(mapFromScene(objectRubberPoint("POLYGON_POINT_0")));
        for (int i = 1; i <= num; i++) {
            appendStr = "POLYGON_POINT_" + QString().setNum(i);
            QPointF appendPoint = mapFromScene(objectRubberPoint(appendStr));
            rubberPath.lineTo(appendPoint);
        }
        //rubberPath.lineTo(0,0);
        updatePath(rubberPath);

        //Ensure the path isn't updated until the number of points is changed again
        setObjectRubberText("POLYGON_NUM_POINTS", QString());
    }
    else if (rubberMode == OBJ_RUBBER_POLYGON_INSCRIBE) {
        setObjectPos(objectRubberPoint("POLYGON_CENTER"));

        quint16 numSides = objectRubberPoint("POLYGON_NUM_SIDES").x();

        QPointF inscribePoint = mapFromScene(objectRubberPoint("POLYGON_INSCRIBE_POINT"));
        QLineF inscribeLine = QLineF(QPointF(0,0), inscribePoint);
        double inscribeAngle = inscribeLine.angle();
        double inscribeInc = 360.0/numSides;

        if (painter) {
            drawRubberLine(inscribeLine, painter, "VIEW_COLOR_CROSSHAIR");
        }

        QPainterPath inscribePath;
        //First Point
        inscribePath.moveTo(inscribePoint);
        //Remaining Points
        for (int i = 1; i < numSides; i++) {
            inscribeLine.setAngle(inscribeAngle + inscribeInc*i);
            inscribePath.lineTo(inscribeLine.p2());
        }
        updatePath(inscribePath);
    }
    else if (rubberMode == OBJ_RUBBER_POLYGON_CIRCUMSCRIBE) {
        setObjectPos(objectRubberPoint("POLYGON_CENTER"));

        quint16 numSides = objectRubberPoint("POLYGON_NUM_SIDES").x();

        QPointF circumscribePoint = mapFromScene(objectRubberPoint("POLYGON_CIRCUMSCRIBE_POINT"));
        QLineF circumscribeLine = QLineF(QPointF(0,0), circumscribePoint);
        double circumscribeAngle = circumscribeLine.angle();
        double circumscribeInc = 360.0/numSides;

        if (painter) {
            drawRubberLine(circumscribeLine, painter, "VIEW_COLOR_CROSSHAIR");
        }

        QPainterPath circumscribePath;
        //First Point
        QLineF prev(circumscribeLine.p2(), QPointF(0,0));
        prev = prev.normalVector();
        circumscribeLine.setAngle(circumscribeAngle + circumscribeInc);
        QLineF perp(circumscribeLine.p2(), QPointF(0,0));
        perp = perp.normalVector();
        QPointF iPoint;
        perp.intersects(prev, &iPoint);
        circumscribePath.moveTo(iPoint);
        //Remaining Points
        for (int i = 2; i <= numSides; i++) {
            prev = perp;
            circumscribeLine.setAngle(circumscribeAngle + circumscribeInc*i);
            perp = QLineF(circumscribeLine.p2(), QPointF(0,0));
            perp = perp.normalVector();
            perp.intersects(prev, &iPoint);
            circumscribePath.lineTo(iPoint);
        }
        updatePath(circumscribePath);
    }
    else if (rubberMode == OBJ_RUBBER_GRIP) {
        if (painter) {
            int elemCount = normalPath.elementCount();
            QPointF gripPoint = objectRubberPoint("GRIP_POINT");
            if (gripIndex == -1) {
                gripIndex = findIndex(gripPoint);
                if (gripIndex == -1) {
                    return;
                }
            }

            int m = 0;
            int n = 0;

            if (!gripIndex) {
                m = elemCount - 1;
                n = 1;
            }
            else if (gripIndex == elemCount-1) {
                m = elemCount - 2;
                n = 0;
            }
            else {
                m = gripIndex - 1;
                n = gripIndex + 1;
            }
            QPainterPath::Element em = normalPath.elementAt(m);
            QPainterPath::Element en = normalPath.elementAt(n);
            QPointF emPoint = QPointF(em.x, em.y);
            QPointF enPoint = QPointF(en.x, en.y);
            painter->drawLine(emPoint, mapFromScene(objectRubberPoint(QString())));
            painter->drawLine(enPoint, mapFromScene(objectRubberPoint(QString())));

            QLineF rubLine(mapFromScene(gripPoint), mapFromScene(objectRubberPoint(QString())));
            drawRubberLine(rubLine, painter, "VIEW_COLOR_CROSSHAIR");
        }
    }
}
#endif

/* POLYLINE. */
ScriptValue
polyline_command(ScriptEnv * context)
{
    init_command();
    clear_selection();

    end_command();
    return script_null;
}

#if 0
//Command: Polyline

var global = {}; //Required
context->firstRun;
context->first;
context->prev;
context->num;

function main()
{
    init_command();
    clear_selection();
    context->firstRun = true;
    context->firstX = NaN;
    context->firstY = NaN;
    context->prevX = NaN;
    context->prevY = NaN;
    context->num = 0;
    prompt_output(translate("Specify first point: "));
}

function click(EmbVector v)
{
    if (context->firstRun) {
        context->firstRun = false;
        context->first = v;
        context->prev = v;
        addRubber("POLYLINE");
        setRubberMode("POLYLINE");
        setRubberPoint("POLYLINE_POINT_0", context->first.x, context->first.y);
        prompt_output(translate("Specify next point or [Undo]: "));
    }
    else {
        context->num++;
        setRubberPoint("POLYLINE_POINT_" + context->num.toString(), v.x, v.y);
        setRubberText("POLYLINE_NUM_POINTS", context->num.toString());
        spareRubber("POLYLINE");
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
            addRubber("POLYLINE");
            setRubberMode("POLYLINE");
            setRubberPoint("POLYLINE_POINT_0", context->first.x, context->first.y);
            prompt_output(translate("Specify next point or [Undo]: "));
        }
    }
    else {
        if (str == "U" || str == "UNDO") {
            /* TODO: Probably should add additional translate calls here. */
            todo("POLYLINE", "prompt() for UNDO");
        }
        else {
            if (!parse_vector(str, v)) {
                alert(translate("Point or option keyword required."));
                prompt_output(translate("Specify next point or [Undo]: "));
            }
            else {
                context->num++;
                setRubberPoint("POLYLINE_POINT_" + context->num.toString(), v.x, v.y);
                setRubberText("POLYLINE_NUM_POINTS", context->num.toString());
                spareRubber("POLYLINE");
                context->prev = v;
                prompt_output(translate("Specify next point or [Undo]: "));
            }
        }
    }
}

PolylineObject::PolylineObject(double x, double y, const QPainterPath& p, QRgb rgb, QGraphicsItem* parent) : BaseObject(parent)
{
    qDebug("PolylineObject Constructor()");
    init(x, y, p, rgb, Qt::SolidLine); //TODO: getCurrentLineType
}

//WARNING: DO NOT enable QGraphicsItem::ItemIsMovable. If it is enabled,
//WARNING: and the item is double clicked, the scene will erratically move the item while zooming.
//WARNING: All movement has to be handled explicitly by us, not by the scene.
void
PolylineObject::init(double x, double y, const QPainterPath& p, QRgb rgb, Qt::PenStyle lineType)
{
    setData(OBJ_TYPE, type());
    setData(OBJ_NAME, OBJ_NAME_POLYLINE);

    setFlag(QGraphicsItem::ItemIsSelectable, true);

    gripIndex = -1;
    updatePath(p);
    setObjectPos(x,y);
    setObjectColor(rgb);
    setObjectLineType(lineType);
    setObjectLineWeight(0.35); //TODO: pass in proper lineweight
    setPen(objPen);
}

void
PolylineObject::updatePath(const QPainterPath& p)
{
    normalPath = p;
    QPainterPath reversePath = normalPath.toReversed();
    reversePath.connectPath(normalPath);
    setObjectPath(reversePath);
}

void
PolylineObject::updateRubber(QPainter* painter)
{
    int rubberMode = objectRubberMode();
    if (rubberMode == OBJ_RUBBER_POLYLINE) {
        setObjectPos(objectRubberPoint("POLYLINE_POINT_0"));

        QLineF rubberLine(normalPath.currentPosition(), mapFromScene(objectRubberPoint(QString())));
        if (painter) drawRubberLine(rubberLine, painter, "VIEW_COLOR_CROSSHAIR");

        bool ok = false;
        QString numStr = objectRubberText("POLYLINE_NUM_POINTS");
        if (numStr.isNull()) return;
        int num = numStr.toInt(&ok);
        if (!ok) return;

        QString appendStr;
        QPainterPath rubberPath;
        for (int i = 1; i <= num; i++) {
            appendStr = "POLYLINE_POINT_" + QString().setNum(i);
            QPointF appendPoint = mapFromScene(objectRubberPoint(appendStr));
            rubberPath.lineTo(appendPoint);
        }
        updatePath(rubberPath);

        //Ensure the path isn't updated until the number of points is changed again
        setObjectRubberText("POLYLINE_NUM_POINTS", QString());
    }
    else if (rubberMode == OBJ_RUBBER_GRIP) {
        if (painter) {
            int elemCount = normalPath.elementCount();
            QPointF gripPoint = objectRubberPoint("GRIP_POINT");
            if (gripIndex == -1) gripIndex = findIndex(gripPoint);
            if (gripIndex == -1) return;

            if (!gripIndex) {
                //First
                QPainterPath::Element ef = normalPath.elementAt(1);
                QPointF efPoint = QPointF(ef.x, ef.y);
                painter->drawLine(efPoint, mapFromScene(objectRubberPoint(QString())));
            }
            else if (gripIndex == elemCount-1) {
                //Last
                QPainterPath::Element el = normalPath.elementAt(gripIndex-1);
                QPointF elPoint = QPointF(el.x, el.y);
                painter->drawLine(elPoint, mapFromScene(objectRubberPoint(QString())));
            }
            else {
                //Middle
                QPainterPath::Element em = normalPath.elementAt(gripIndex-1);
                QPainterPath::Element en = normalPath.elementAt(gripIndex+1);
                QPointF emPoint = QPointF(em.x, em.y);
                QPointF enPoint = QPointF(en.x, en.y);
                painter->drawLine(emPoint, mapFromScene(objectRubberPoint(QString())));
                painter->drawLine(enPoint, mapFromScene(objectRubberPoint(QString())));
            }

            QLineF rubLine(mapFromScene(gripPoint), mapFromScene(objectRubberPoint(QString())));
            drawRubberLine(rubLine, painter, "VIEW_COLOR_CROSSHAIR");
        }
    }
}
#endif

/* RECTANGLE */
ScriptValue
rectangle_command(ScriptEnv * context)
{
    init_command();
    clear_selection();

    end_command();
    return script_null;
}

#if 0
//Command: Rectangle

var global = {}; //Required
context->newRect;
context->x1;
context->y1;
context->x2;
context->y2;

function main()
{
    init_command();
    clear_selection();
    context->newRect = true;
    context->point1.x = NaN;
    context->point1.y = NaN;
    context->point2.x = NaN;
    context->point2.y = NaN;
    prompt_output(translate("Specify first corner point or [Chamfer/Fillet]: "));
}

void
click(EmbVector v)
{
    if (context->newRect) {
        context->newRect = false;
        context->point1 = v;
        addRubber("RECTANGLE");
        setRubberMode("RECTANGLE");
        setRubberPoint("RECTANGLE_START", v.x, v.y);
        prompt_output(translate("Specify other corner point or [Dimensions]: "));
    }
    else {
        context->newRect = true;
        context->point2 = v;
        setRubberPoint("RECTANGLE_END", v.x, v.y);
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
        //TODO: Probably should add additional translate calls here.
        todo("RECTANGLE", "prompt() for CHAMFER");
    }
    else if (str == "D" || str == "DIMENSIONS") {
        //TODO: Probably should add additional translate calls here.
        todo("RECTANGLE", "prompt() for DIMENSIONS");
    }
    else if (str == "F" || str == "FILLET") {
        //TODO: Probably should add additional translate calls here.
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
                addRubber("RECTANGLE");
                setRubberMode("RECTANGLE");
                setRubberPoint("RECTANGLE_START", v.x, v.y);
                prompt_output(translate("Specify other corner point or [Dimensions]: "));
            }
            else {
                context->newRect = true;
                context->point2 = v;
                setRubberPoint("RECTANGLE_END", v.x, v.y);
                vulcanize();
                end_command();
            }
        }
    }
}

RectObject::RectObject(double x, double y, double w, double h, QRgb rgb, QGraphicsItem* parent) : BaseObject(parent)
{
    qDebug("RectObject Constructor()");
    init(x, y, w, h, rgb, Qt::SolidLine); //TODO: getCurrentLineType
}

void
RectObject::init(double x, double y, double w, double h, QRgb rgb, Qt::PenStyle lineType)
{
    setData(OBJ_TYPE, type());
    setData(OBJ_NAME, OBJ_NAME_RECTANGLE);

    //WARNING: DO NOT enable QGraphicsItem::ItemIsMovable. If it is enabled,
    //WARNING: and the item is double clicked, the scene will erratically move the item while zooming.
    //WARNING: All movement has to be handled explicitly by us, not by the scene.
    setFlag(QGraphicsItem::ItemIsSelectable, true);

    setObjectRect(x, y, w, h);
    setObjectColor(rgb);
    setObjectLineType(lineType);
    setObjectLineWeight(0.35); //TODO: pass in proper lineweight
    setPen(objPen);
}


void
RectObject::updatePath()
{
    QPainterPath path;
    QRectF r = rect();
    path.moveTo(r.bottomLeft());
    path.lineTo(r.bottomRight());
    path.lineTo(r.topRight());
    path.lineTo(r.topLeft());
    path.lineTo(r.bottomLeft());
    //NOTE: Reverse the path so that the inside area isn't considered part of the rectangle
    path.lineTo(r.topLeft());
    path.lineTo(r.topRight());
    path.lineTo(r.bottomRight());
    path.moveTo(r.bottomLeft());
    setObjectPath(path);
}

void
RectObject::updateRubber(QPainter* painter)
{
    int rubberMode = objectRubberMode();
    if (rubberMode == OBJ_RUBBER_RECTANGLE) {
        QPointF sceneStartPoint = objectRubberPoint("RECTANGLE_START");
        QPointF sceneEndPoint = objectRubberPoint("RECTANGLE_END");
        double x = sceneStartPoint.x();
        double y = sceneStartPoint.y();
        double w = sceneEndPoint.x() - sceneStartPoint.x();
        double h = sceneEndPoint.y() - sceneStartPoint.y();
        setObjectRect(x,y,w,h);
        updatePath();
    }
    else if (rubberMode == OBJ_RUBBER_GRIP) {
        if (painter) {
            //TODO: Make this work with rotation & scaling
            /*
            QPointF gripPoint = objectRubberPoint("GRIP_POINT");
            QPointF after = objectRubberPoint(QString());
            QPointF delta = after-gripPoint;
            if     (gripPoint == objectTopLeft())     { painter->drawPolygon(mapFromScene(QRectF(after.x(), after.y(), objectWidth()-delta.x(), objectHeight()-delta.y()))); }
            else if (gripPoint == objectTopRight())    { painter->drawPolygon(mapFromScene(QRectF(objectTopLeft().x(), objectTopLeft().y()+delta.y(), objectWidth()+delta.x(), objectHeight()-delta.y()))); }
            else if (gripPoint == objectBottomLeft())  { painter->drawPolygon(mapFromScene(QRectF(objectTopLeft().x()+delta.x(), objectTopLeft().y(), objectWidth()-delta.x(), objectHeight()+delta.y()))); }
            else if (gripPoint == objectBottomRight()) { painter->drawPolygon(mapFromScene(QRectF(objectTopLeft().x(), objectTopLeft().y(), objectWidth()+delta.x(), objectHeight()+delta.y()))); }

            QLineF rubLine(mapFromScene(gripPoint), mapFromScene(objectRubberPoint(QString())));
            drawRubberLine(rubLine, painter, "VIEW_COLOR_CROSSHAIR");
            */

            QPointF gripPoint = objectRubberPoint("GRIP_POINT");
            QPointF after = objectRubberPoint(QString());
            QPointF delta = after-gripPoint;

            QLineF rubLine(mapFromScene(gripPoint), mapFromScene(objectRubberPoint(QString())));
            drawRubberLine(rubLine, painter, "VIEW_COLOR_CROSSHAIR");
        }
    }
}

QPainterPath RectObject::objectSavePath() const
{
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
#endif

/*
void
SaveObject::addTextSingle(EmbPattern* pattern, QGraphicsItem* item)
{
    //TODO: saving polygons, polylines and paths must be stable before we go here.

    //TODO: This needs to work like a path, not a polyline. Improve this
    TextSingleObject* obj = static_cast<TextSingleObject*>(item);
    if (obj) {
        if (formatType == EMBFORMAT_STITCHONLY) {
            QList<QPainterPath> pathList = obj->objectSavePathList();
            foreach(QPainterPath path, pathList) {
                toPolyline(pattern, obj->objectPos(), path.simplified(), "0", obj->objectColor(), "CONTINUOUS", "BYLAYER"); //TODO: proper layer/lineType/lineWeight
                // TODO: Improve precision, replace simplified
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
var global = {}; //Required
context->text;
context->textX;
context->textY;
context->textJustify;
context->textFont;
context->textHeight;
context->textRotation;
context->mode;

function
main()
{
    initCommand();
    clearSelection();
    context->text = "";
    context->textX = NaN;
    context->textY = NaN;
    context->textJustify = "Left";
    context->textFont = textFont();
    context->textHeight = NaN;
    context->textRotation = NaN;
    context->mode = TEXTSINGLE_MODE_SETGEOM;
    prompt_output(translate("Current font: ") + "{" + context->textFont + "} " + translate("Text height: ") + "{" +  textSize() + "}");
    prompt_output(translate("Specify start point of text or [Justify/Setfont]: "));
}

function
click(EmbVector v)
{
    if (context->mode == TEXTSINGLE_MODE_SETGEOM) {
        if (isNaN(context->textX)) {
            context->textX = x;
            context->textY = y;
            addRubber("LINE");
            setRubberMode("LINE");
            setRubberPoint("LINE_START", context->textX, context->textY);
            prompt_output(translate("Specify text height") + " {" + textSize() + "}: ");
        }
        else if (isNaN(context->textHeight)) {
            context->textHeight = calculateDistance(context->textX, context->textY, x, y);
            setTextSize(context->textHeight);
            prompt_output(translate("Specify text angle") + " {" + textAngle() + "}: ");
        }
        else if (isNaN(context->textRotation)) {
            context->textRotation = calculateAngle(context->textX, context->textY, x, y);
            setSettingsTextAngle(context->textRotation);
            prompt_output(translate("Enter text: "));
            context->mode = TEXTSINGLE_MODE_RAPID;
            command_prompt("enable rapidfire");
            clearRubber();
            addRubber("TEXTSINGLE");
            setRubberMode("TEXTSINGLE");
            setRubberPoint("TEXT_POINT", context->textX, context->textY);
            setRubberPoint("TEXT_HEIGHT_ROTATION", context->textHeight, context->textRotation);
            setRubberText("TEXT_FONT", context->textFont);
            setRubberText("TEXT_JUSTIFY", context->textJustify);
            setRubberText("TEXT_RAPID", context->text);
        }
        else {
            //Do nothing, as we are in rapidFire mode now.
        }
    }
}

function context(str)
{
    todo("SINGLELINETEXT", "context()");
}

//TODO: Probably should add additional translate calls here.
function prompt(str)
{
    if (context->mode == TEXTSINGLE_MODE_JUSTIFY) {
        if (str == "C" || str == "CENTER") {
            context->mode = TEXTSINGLE_MODE_SETGEOM;
            context->textJustify = "Center";
            setRubberText("TEXT_JUSTIFY", context->textJustify);
            prompt_output(translate("Specify center point of text or [Justify/Setfont]: "));
        }
        else if (str == "R" || str == "RIGHT") {
            context->mode = TEXTSINGLE_MODE_SETGEOM;
            context->textJustify = "Right";
            setRubberText("TEXT_JUSTIFY", context->textJustify);
            prompt_output(translate("Specify right-end point of text or [Justify/Setfont]: "));
        }
        else if (str == "A" || str == "ALIGN") {
            context->mode = TEXTSINGLE_MODE_SETGEOM;
            context->textJustify = "Aligned";
            setRubberText("TEXT_JUSTIFY", context->textJustify);
            prompt_output(translate("Specify start point of text or [Justify/Setfont]: "));
        }
        else if (str == "M" || str == "MIDDLE") {
            context->mode = TEXTSINGLE_MODE_SETGEOM;
            context->textJustify = "Middle";
            setRubberText("TEXT_JUSTIFY", context->textJustify);
            prompt_output(translate("Specify middle point of text or [Justify/Setfont]: "));
        }
        else if (str == "F" || str == "FIT") {
            context->mode = TEXTSINGLE_MODE_SETGEOM;
            context->textJustify = "Fit";
            setRubberText("TEXT_JUSTIFY", context->textJustify);
            prompt_output(translate("Specify start point of text or [Justify/Setfont]: "));
        }
        else if (str == "TL" || str == "TOPLEFT") {
            context->mode = TEXTSINGLE_MODE_SETGEOM;
            context->textJustify = "Top Left";
            setRubberText("TEXT_JUSTIFY", context->textJustify);
            prompt_output(translate("Specify top-left point of text or [Justify/Setfont]: "));
        }
        else if (str == "TC" || str == "TOPCENTER") {
            context->mode = TEXTSINGLE_MODE_SETGEOM;
            context->textJustify = "Top Center";
            setRubberText("TEXT_JUSTIFY", context->textJustify);
            prompt_output(translate("Specify top-center point of text or [Justify/Setfont]: "));
        }
        else if (str == "TR" || str == "TOPRIGHT") {
            context->mode = TEXTSINGLE_MODE_SETGEOM;
            context->textJustify = "Top Right";
            setRubberText("TEXT_JUSTIFY", context->textJustify);
            prompt_output(translate("Specify top-right point of text or [Justify/Setfont]: "));
        }
        else if (str == "ML" || str == "MIDDLELEFT") {
            context->mode = TEXTSINGLE_MODE_SETGEOM;
            context->textJustify = "Middle Left";
            setRubberText("TEXT_JUSTIFY", context->textJustify);
            prompt_output(translate("Specify middle-left point of text or [Justify/Setfont]: "));
        }
        else if (str == "MC" || str == "MIDDLECENTER") {
            context->mode = TEXTSINGLE_MODE_SETGEOM;
            context->textJustify = "Middle Center";
            setRubberText("TEXT_JUSTIFY", context->textJustify);
            prompt_output(translate("Specify middle-center point of text or [Justify/Setfont]: "));
        }
        else if (str == "MR" || str == "MIDDLERIGHT") {
            context->mode = TEXTSINGLE_MODE_SETGEOM;
            context->textJustify = "Middle Right";
            setRubberText("TEXT_JUSTIFY", context->textJustify);
            prompt_output(translate("Specify middle-right point of text or [Justify/Setfont]: "));
        }
        else if (str == "BL" || str == "BOTTOMLEFT") {
            context->mode = TEXTSINGLE_MODE_SETGEOM;
            context->textJustify = "Bottom Left";
            setRubberText("TEXT_JUSTIFY", context->textJustify);
            prompt_output(translate("Specify bottom-left point of text or [Justify/Setfont]: "));
        }
        else if (str == "BC" || str == "BOTTOMCENTER") {
            context->mode = TEXTSINGLE_MODE_SETGEOM;
            context->textJustify = "Bottom Center";
            setRubberText("TEXT_JUSTIFY", context->textJustify);
            prompt_output(translate("Specify bottom-center point of text or [Justify/Setfont]: "));
        }
        else if (str == "BR" || str == "BOTTOMRIGHT") {
            context->mode = TEXTSINGLE_MODE_SETGEOM;
            context->textJustify = "Bottom Right";
            setRubberText("TEXT_JUSTIFY", context->textJustify);
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
        setRubberText("TEXT_FONT", context->textFont);
        setTextFont(context->textFont);
        prompt_output(translate("Specify start point of text or [Justify/Setfont]: "));
    }
    else if (context->mode == TEXTSINGLE_MODE_SETGEOM) {
        if (isNaN(context->textX)) {
            if (str == "J" || str == "JUSTIFY") //TODO: Probably should add additional translate calls here.
            {
                context->mode = TEXTSINGLE_MODE_JUSTIFY;
                prompt_output(translate("Text Justification Options [Center/Right/Align/Middle/Fit/TL/TC/TR/ML/MC/MR/BL/BC/BR]: "));
            }
            else if (str == "S" || str == "SETFONT") //TODO: Probably should add additional translate calls here.
            {
                context->mode = TEXTSINGLE_MODE_SETFONT;
                prompt_output(translate("Specify font name: "));
            }
            else {
                var strList = str.split(",");
                if (isNaN(strList[0]) || isNaN(strList[1])) {
                    alert(translate("Point or option keyword required."));
                    prompt_output(translate("Specify start point of text or [Justify/Setfont]: "));
                }
                else {
                    context->textX = Number(strList[0]);
                    context->textY = Number(strList[1]);
                    addRubber("LINE");
                    setRubberMode("LINE");
                    setRubberPoint("LINE_START", context->textX, context->textY);
                    prompt_output(translate("Specify text height") + " {" + textSize() + "}: ");
                }
            }
        }
        else if (isNaN(context->textHeight)) {
            if (str == "") {
                context->textHeight = textSize();
                prompt_output(translate("Specify text angle") + " {" + textAngle() + "}: ");
            }
            else if (isNaN(str)) {
                alert(translate("Requires valid numeric distance or second point."));
                prompt_output(translate("Specify text height") + " {" + textSize() + "}: ");
            }
            else {
                context->textHeight = Number(str);
                setTextSize(context->textHeight);
                prompt_output(translate("Specify text angle") + " {" + textAngle() + "}: ");
            }
        }
        else if (isNaN(context->textRotation)) {
            if (str == "") {
                context->textRotation = textAngle();
                prompt_output(translate("Enter text: "));
                context->mode = TEXTSINGLE_MODE_RAPID;
                enablePromptRapidFire();
                clearRubber();
                addRubber("TEXTSINGLE");
                setRubberMode("TEXTSINGLE");
                setRubberPoint("TEXT_POINT", context->textX, context->textY);
                setRubberPoint("TEXT_HEIGHT_ROTATION", context->textHeight, context->textRotation);
                setRubberText("TEXT_FONT", context->textFont);
                setRubberText("TEXT_JUSTIFY", context->textJustify);
                setRubberText("TEXT_RAPID", context->text);
            }
            else if (isNaN(str)) {
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
                addRubber("TEXTSINGLE");
                setRubberMode("TEXTSINGLE");
                setRubberPoint("TEXT_POINT", context->textX, context->textY);
                setRubberPoint("TEXT_HEIGHT_ROTATION", context->textHeight, context->textRotation);
                setRubberText("TEXT_FONT", context->textFont);
                setRubberText("TEXT_JUSTIFY", context->textJustify);
                setRubberText("TEXT_RAPID", context->text);
            }
        }
        else {
            //Do nothing, as we are in rapidFire mode now.
        }
    }
    else if (context->mode == TEXTSINGLE_MODE_RAPID)
    {
        if (str == "RAPID_ENTER")
        {
            if (context->text == "")
            {
                endCommand();
            }
            else
            {
                vulcanize();
                endCommand(); //TODO: Rather than ending the command, calculate where the next line would be and modify the x/y to the new point
            }
        }
        else
        {
            context->text = str;
            setRubberText("TEXT_RAPID", context->text);
        }
    }
}

TextSingleObject::TextSingleObject(const QString& str, double x, double y, QRgb rgb, QGraphicsItem* parent) : BaseObject(parent)
{
    qDebug("TextSingleObject Constructor()");
    init(str, x, y, rgb, Qt::SolidLine); //TODO: getCurrentLineType
}

TextSingleObject::TextSingleObject(TextSingleObject* obj, QGraphicsItem* parent) : BaseObject(parent)
{
    qDebug("TextSingleObject Constructor()");
    if (obj) {
        setObjectTextFont(obj->objTextFont);
        setObjectTextSize(obj->objTextSize);
        setRotation(obj->rotation());
        setObjectTextBackward(obj->objTextBackward);
        setObjectTextUpsideDown(obj->objTextUpsideDown);
        setObjectTextStyle(obj->objTextBold, obj->objTextItalic,
            obj->objTextUnderline, obj->objTextStrikeOut, obj->objTextOverline);
        init(obj->objText, obj->objectX(), obj->objectY(), obj->objectColorRGB(), Qt::SolidLine); //TODO: getCurrentLineType
        setScale(obj->scale());
    }
}

void
TextSingleObject::init(const QString& str, double x, double y, QRgb rgb, Qt::PenStyle lineType)
{
    setData(OBJ_TYPE, type());
    setData(OBJ_NAME, OBJ_NAME_TEXTSINGLE);

    //WARNING: DO NOT enable QGraphicsItem::ItemIsMovable. If it is enabled,
    //WARNING: and the item is double clicked, the scene will erratically move the item while zooming.
    //WARNING: All movement has to be handled explicitly by us, not by the scene.
    setFlag(QGraphicsItem::ItemIsSelectable, true);

    objTextJustify = "Left"; //TODO: set the justification properly

    setObjectText(str);
    setObjectPos(x,y);
    setObjectColor(rgb);
    setObjectLineType(lineType);
    setObjectLineWeight(0.35); //TODO: pass in proper lineweight
    setPen(objPen);
}

void
TextSingleObject::updateRubber(QPainter* painter)
{
    int rubberMode = objectRubberMode();
    if (rubberMode == OBJ_RUBBER_TEXTSINGLE) {
        setObjectTextFont(objectRubberText("TEXT_FONT"));
        setObjectTextJustify(objectRubberText("TEXT_JUSTIFY"));
        setObjectPos(objectRubberPoint("TEXT_POINT"));
        QPointF hr = objectRubberPoint("TEXT_HEIGHT_ROTATION");
        setObjectTextSize(hr.x());
        setRotation(hr.y());
        setObjectText(objectRubberText("TEXT_RAPID"));
    }
    else if (rubberMode == OBJ_RUBBER_GRIP) {
        if (painter) {
            QPointF gripPoint = objectRubberPoint("GRIP_POINT");
            if (gripPoint == scenePos()) {
                painter->drawPath(objectPath().translated(mapFromScene(objectRubberPoint(QString()))-mapFromScene(gripPoint)));
            }

            QLineF rubLine(mapFromScene(gripPoint), mapFromScene(objectRubberPoint(QString())));
            drawRubberLine(rubLine, painter, "VIEW_COLOR_CROSSHAIR");
        }
    }
}

QList<QPainterPath> TextSingleObject::subPathList() const
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
                subPath.cubicTo(path.elementAt(i  ).x, path.elementAt(i  ).y,  //control point 1
                                path.elementAt(i+1).x, path.elementAt(i+1).y,  //control point 2
                                path.elementAt(i+2).x, path.elementAt(i+2).y); //end point
            }
        }
        pathList.append(trans.map(subPath));
    }

    return pathList;
}
#endif

/* QUICKLEADER */
ScriptValue
quickleader_command(ScriptEnv * context)
{
    init_command();
    clear_selection();

    end_command();
    return script_null;
}

#if 0
//TODO: Adding the text is not complete yet.

function main()
{
    init_command();
    clear_selection();
    global.point1 = zero_vector;
    global.point2 = zero_vector;
    prompt_output(translate("Specify first point: "));
}

void
click(EmbVector v)
{
    if (isNaN(global.x1)) {
        global.point1 = v;
        addRubber("DIMLEADER");
        setRubberMode("DIMLEADER_LINE");
        setRubberPoint("DIMLEADER_LINE_START", global.x1, global.y1);
        prompt_output(translate("Specify second point: "));
    }
    else {
        global.point2 = v;
        setRubberPoint("DIMLEADER_LINE_END", global.x2, global.y2);
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
    if (isNaN(global.x1)) {
        if (!parse_vector(str, &v)) {
            alert(translate("Requires two points."));
            prompt_output(translate("Specify first point: "));
        }
        else {
            global.point1 = v;
            addRubber("DIMLEADER");
            setRubberMode("DIMLEADER_LINE");
            setRubberPoint("DIMLEADER_LINE_START", global.x1, global.y1);
            prompt_output(translate("Specify second point: "));
        }
    }
    else {
        if (!parse_vector(str, &v)) {
            alert(translate("Requires two points."));
            prompt_output(translate("Specify second point: "));
        }
        else {
            global.point2 = v;
            setRubberPoint("DIMLEADER_LINE_END", global.x2, global.y2);
            vulcanize();
            end_command();
        }
    }
}
#endif

/* . */
Object::Object(EmbArc arc, QRgb rgb, QGraphicsItem *item)
{
    qDebug("ArcObject Constructor()");
    TYPE = OBJ_TYPE_ARC;
    curved = 0;
    geometry->object.arc = arc;
    init(arc, rgb, Qt::SolidLine); //TODO: getCurrentLineType
}

/* . */
Object::Object(EmbCircle circle, QRgb rgb, QGraphicsItem *item)
{
    TYPE = OBJ_TYPE_CIRCLE;
    geometry->object.circle = circle;
}

/* . */
Object::Object(EmbEllipse ellipse, QRgb rgb, QGraphicsItem *item)
{
    TYPE = OBJ_TYPE_ELLIPSE;
    geometry->object.ellipse = ellipse;
}

/* . */
Object::Object(Object* obj, QGraphicsItem* parent)
{
    qDebug("ArcObject Constructor()");
    if (obj) {
        init(obj->geometry->object.arc, obj->objectColorRGB(), Qt::SolidLine);
        //TODO: getCurrentLineType
        setRotation(obj->rotation());
    }
}

/* . */
Object::~Object()
{
    qDebug("ArcObject Destructor()");
    free(geometry);
}

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
Object::objectQuadrant0()
{
    switch (geometry->type) {
    case OBJ_TYPE_CIRCLE:
        return objectCenter() + QPointF(objectRadius(), 0);
    default:
        break;
    }
    return objectCenter();
}

QPointF
Object::objectQuadrant90()
{
    switch (geometry->type) {
    case OBJ_TYPE_CIRCLE:
        return objectCenter() + QPointF(0, -objectRadius());
    default:
        break;
    }
    return objectCenter();
}

QPointF
Object::objectQuadrant180()
{
    switch (geometry->type) {
    case EMB_CIRCLE:
        return objectCenter() + QPointF(-objectRadius(),0);
    default:
        break;
    }
    return objectCenter();
}

QPointF
Object::objectQuadrant270()
{
    switch (geometry->type) {
    case EMB_CIRCLE:
        return objectCenter() + QPointF(0, objectRadius());
    default:
        break;
    }
    return objectCenter();
}

/* . */
QPointF
Object::objectTopLeft() const
{
    return scenePos() + scale_and_rotate(rect().topLeft(), scale(), rotation());
}

/* . */
QPointF
Object::objectTopRight() const
{
    return scenePos() + scale_and_rotate(rect().topRight(), scale(), rotation());
}

/* . */
QPointF
Object::objectBottomLeft() const
{
    return scenePos() + scale_and_rotate(rect().bottomLeft(), scale(), rotation());
}

/* . */
QPointF
Object::objectBottomRight() const
{
    return scenePos() + scale_and_rotate(rect().bottomRight(), scale(), rotation());
}

/*
QPointF
Object::objectCenter()
{
    return QPointF(0.0f, 0.0f);
}

double
Object::objectWidth()
{
    return 0.0f;
}

double
Object::objectHeight()
{
    return 0.0f;
}

Object::Object(QString const&, double, double, unsigned int, QGraphicsItem*)
{
}
*/

/* . */
QStringList
Object::objectTextJustifyList() const
{
    QStringList justifyList;
    justifyList << "Left" << "Center" << "Right" /* TODO: << "Aligned" */ << "Middle" /* TODO: << "Fit" */ ;
    justifyList << "Top Left" << "Top Center" << "Top Right";
    justifyList << "Middle Left" << "Middle Center" << "Middle Right";
    justifyList << "Bottom Left" << "Bottom Center" << "Bottom Right";
    return justifyList;
}

/* . */
void
Object::setObjectText(const QString& str)
{
    objText = str;
    QPainterPath textPath;
    QFont font;
    font.setFamily(objTextFont);
    font.setPointSizeF(objTextSize);
    font.setBold(objTextBold);
    font.setItalic(objTextItalic);
    font.setUnderline(objTextUnderline);
    font.setStrikeOut(objTextStrikeOut);
    font.setOverline(objTextOverline);
    textPath.addText(0, 0, font, str);

    //Translate the path based on the justification
    QRectF jRect = textPath.boundingRect();
    if     (objTextJustify == "Left") {
        textPath.translate(-jRect.left(), 0);
    }
    else if (objTextJustify == "Center") {
        textPath.translate(-jRect.center().x(), 0);
    }
    else if (objTextJustify == "Right") {
        textPath.translate(-jRect.right(), 0);
    }
    else if (objTextJustify == "Aligned") {
        // TODO: TextSingleObject Aligned Justification
    }
    else if (objTextJustify == "Middle") {
        textPath.translate(-jRect.center());
    }
    else if (objTextJustify == "Fit") {
        // TODO: TextSingleObject Fit Justification
    }
    else if (objTextJustify == "Top Left") {
        textPath.translate(-jRect.topLeft());
    }
    else if (objTextJustify == "Top Center") {
        textPath.translate(-jRect.center().x(), -jRect.top());
    }
    else if (objTextJustify == "Top Right") {
        textPath.translate(-jRect.topRight());
    }
    else if (objTextJustify == "Middle Left") {
        textPath.translate(-jRect.left(), -jRect.top()/2.0);
    }
    else if (objTextJustify == "Middle Center") {
        textPath.translate(-jRect.center().x(), -jRect.top()/2.0);
    }
    else if (objTextJustify == "Middle Right") {
        textPath.translate(-jRect.right(), -jRect.top()/2.0);
    }
    else if (objTextJustify == "Bottom Left") {
        textPath.translate(-jRect.bottomLeft());
    }
    else if (objTextJustify == "Bottom Center") {
        textPath.translate(-jRect.center().x(), -jRect.bottom());
    }
    else if (objTextJustify == "Bottom Right") {
        textPath.translate(-jRect.bottomRight());
    }

    //Backward or Upside Down
    if (objTextBackward || objTextUpsideDown) {
        double horiz = 1.0;
        double vert = 1.0;
        if (objTextBackward) horiz = -1.0;
        if (objTextUpsideDown) vert = -1.0;

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
                                              // start point P1 is not needed
                P2 = textPath.elementAt(i);   // control point
                P3 = textPath.elementAt(i+1); // control point
                P4 = textPath.elementAt(i+2); // end point

                flippedPath.cubicTo(horiz * P2.x, vert * P2.y,
                                    horiz * P3.x, vert * P3.y,
                                    horiz * P4.x, vert * P4.y);
            }
        }
        objTextPath = flippedPath;
    }
    else
        objTextPath = textPath;

    //Add the grip point to the shape path
    QPainterPath gripPath = objTextPath;
    gripPath.connectPath(objTextPath);
    gripPath.addRect(-0.00000001, -0.00000001, 0.00000002, 0.00000002);
    setObjectPath(gripPath);
}

/* . */
void
Object::setObjectTextFont(const QString& font)
{
    objTextFont = font;
    setObjectText(objText);
}

/* . */
void
Object::setObjectTextJustify(const QString& justify)
{
    //Verify the string is a valid option
    if (justify == "Left") {
        objTextJustify = justify;
    }
    else if (justify == "Center") {
        objTextJustify = justify;
    }
    else if (justify == "Right") {
        objTextJustify = justify;
    }
    else if (justify == "Aligned") {
        objTextJustify = justify;
    }
    else if (justify == "Middle") {
        objTextJustify = justify;
    }
    else if (justify == "Fit") {
        objTextJustify = justify;
    }
    else if (justify == "Top Left") {
        objTextJustify = justify;
    }
    else if (justify == "Top Center") {
        objTextJustify = justify;
    }
    else if (justify == "Top Right") {
        objTextJustify = justify;
    }
    else if (justify == "Middle Left") {
        objTextJustify = justify;
    }
    else if (justify == "Middle Center") {
        objTextJustify = justify;
    }
    else if (justify == "Middle Right") {
        objTextJustify = justify;
    }
    else if (justify == "Bottom Left") {
        objTextJustify = justify;
    }
    else if (justify == "Bottom Center") {
        objTextJustify = justify;
    }
    else if (justify == "Bottom Right") {
        objTextJustify = justify;
    }
    else {
        // Default
        objTextJustify = "Left";
    }
    setObjectText(objText);
}

/* . */
void
Object::setObjectTextSize(double size)
{
    objTextSize = size;
    setObjectText(objText);
}

/* . */
void
Object::setObjectTextStyle(bool bold, bool italic, bool under, bool strike, bool over)
{
    objTextBold = bold;
    objTextItalic = italic;
    objTextUnderline = under;
    objTextStrikeOut = strike;
    objTextOverline = over;
    setObjectText(objText);
}

void
Object::setObjectTextBold(bool val)
{
    objTextBold = val;
    setObjectText(objText);
}

/* . */
void
Object::setObjectTextItalic(bool val)
{
    objTextItalic = val;
    setObjectText(objText);
}

/* . */
void
Object::setObjectTextUnderline(bool val)
{
    objTextUnderline = val;
    setObjectText(objText);
}

/* . */
void
Object::setObjectTextStrikeOut(bool val)
{
    objTextStrikeOut = val;
    setObjectText(objText);
}

/* . */
void
Object::setObjectTextOverline(bool val)
{
    objTextOverline = val;
    setObjectText(objText);
}

/* . */
void
Object::setObjectTextBackward(bool val)
{
    objTextBackward = val;
    setObjectText(objText);
}

/* . */
void
Object::setObjectTextUpsideDown(bool val)
{
    objTextUpsideDown = val;
    setObjectText(objText);
}

/*
Object::Object(double, double, double, double, unsigned int, QGraphicsItem*)
{
}

Object::Object(double, double, double, double, unsigned int, QGraphicsItem*)'
{
}

Object::Object(EmbPoint_, unsigned int, QGraphicsItem*)'
{
}

Object::Object(EmbPath_, int, QPainterPath const&, unsigned int, QGraphicsItem*)'
{
}

Object::Object(EmbPath_, int, QPainterPath const&, unsigned int, QGraphicsItem*)'
{
}

Object::objectIncludedAngle() const
{
}

Object::objectClockwise() const
{
}

Object::realRender(QPainter*, QPainterPath const&)
{
}

Object::gripEdit(QPointF const&, QPointF const&)
{
}

Object::updatePath(QPainterPath const&)
{
}
*/

QPointF
Object::objectEndPoint1()
{
    return QPointF(0.0, 0.0);
}

QPointF
Object::objectEndPoint2()
{
    return QPointF(0.0, 0.0);
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
double
Object::objectAngle() const
{
    double angle = line().angle() - rotation();
    while (angle >= 360.0) {
        angle -= 360.0;
    }
    while (angle < 0.0) {
        angle += 360.0;
    }
    return angle;
}

/* . */
double
Object::objectStartAngle() const
{
    switch (geometry->type) {
    case EMB_ARC: {
        double angle = QLineF(scenePos(), objectStartPoint()).angle();
        return fmod(angle+360.0, 360.0);
    }
    default:
        break;
    }
    return 0.0f;
}

/* . */
double
Object::objectEndAngle() const
{
    switch (geometry->type) {
    case EMB_ARC: {
        double angle = QLineF(scenePos(), objectEndPoint()).angle();
        return fmod(angle+360.0, 360.0);
    }
    default:
        break;
    }
    return 0.0f;
}

/* . */
double
Object::objectArcLength() const
{
    switch (geometry->type) {
    case EMB_ARC: {
        return radians(objectIncludedAngle()) * objectRadius();
    }
    default:
        break;
    }
    return 0.0;
}


/* . */
void
Object::setObjectStartAngle(double angle)
{
    switch (geometry->type) {
    case EMB_ARC: {
        //TODO: ArcObject setObjectStartAngle
        break;
    }
    default:
        break;
    }
}

/* . */
void
Object::setObjectEndAngle(double angle)
{
    switch (geometry->type) {
    case EMB_ARC: {
        //TODO: ArcObject setObjectEndAngle
        break;
    }
    default:
        break;
    }
}

/* . */
void
Object::setObjectStartPoint(EmbVector point)
{
    switch (geometry->type) {
    case EMB_ARC: {
        geometry->object.arc.start = point;
        calculateArcData(geometry->object.arc);
        break;
    }
    default:
        break;
    }
}

/* . */
void
Object::setObjectMidPoint(EmbVector point)
{
    switch (geometry->type) {
    case EMB_ARC: {
        geometry->object.arc.mid = point;
        calculateArcData(geometry->object.arc);
        break;
    }
    default:
        break;
    }
}

/* . */
void
Object::setObjectEndPoint(EmbVector point)
{
    switch (geometry->type) {
    case EMB_ARC: {
        geometry->object.arc.end = point;
        calculateArcData(geometry->object.arc);
        break;
    }
    default:
        break;
    }
}

/*
double
Object::objectArea() const
{
    switch (geometry->type) {
    case EMB_ARC: {
        //Area of a circular segment
        double r = objectRadius();
        double theta = radians(objectIncludedAngle());
        return ((r*r)/2)*(theta - sin(theta));
    }
    default:
        break;
    }
    return 0.0;
}
*/

/* . */
double
Object::objectChord() const
{
    switch (geometry->type) {
    case EMB_ARC: {
        return QLineF(objectStartPoint(), objectEndPoint()).length();
    }
    default:
        break;
    }
    return 0.0;
}

/* . */
double
Object::objectIncludedAngle() const
{
    switch (geometry->type) {
    case EMB_ARC: {
        double chord = objectChord();
        double rad = objectRadius();
        if (chord <= 0 || rad <= 0) {
            /* Prevents division by zero and non-existant circles. */
            return 0;
        }

        //NOTE: Due to floating point rounding errors, we need to clamp the quotient so it is in the range [-1, 1]
        //      If the quotient is out of that range, then the result of asin() will be NaN.
        double quotient = chord/(2.0*rad);
        quotient = EMB_MIN(1.0, quotient);
        quotient = EMB_MAX(0.0, quotient); //NOTE: 0 rather than -1 since we are enforcing a positive chord and radius
        return degrees(2.0*asin(quotient)); //Properties of a Circle - Get the Included Angle - Reference: ASD9
    }
    default:
        break;
    }
    return 0.0;
}

/* . */
bool
Object::objectClockwise() const
{
    switch (geometry->type) {
    case EMB_ARC: {
        //NOTE: Y values are inverted here on purpose
        EmbArc arc;
        geometry->object.arc.start.y = -geometry->object.arc.start.y;
        geometry->object.arc.mid.y = -geometry->object.arc.start.y;
        geometry->object.arc.end.y = -geometry->object.arc.end.y;
        if (emb_arc_clockwise(arc)) {
            return true;
        }
        break;
    }
    default:
        break;
    }
    return false;
}

/* . */
void
Object::updatePath()
{
    switch (geometry->type) {
    case EMB_ARC: {
        double startAngle = (objectStartAngle() + rotation());
        double spanAngle = objectIncludedAngle();

        if (objectClockwise()) {
            spanAngle = -spanAngle;
        }

        QPainterPath path;
        path.arcMoveTo(rect(), startAngle);
        path.arcTo(rect(), startAngle, spanAngle);
        //NOTE: Reverse the path so that the inside area isn't considered part of the arc
        path.arcTo(rect(), startAngle+spanAngle, -spanAngle);
        setObjectPath(path);
        break;
    }
    default:
        break;
    }
}

/* . */
Object::Object(QString const&, double, double, unsigned int, QGraphicsItem*)
{
}

/* . */
Object::Object(double, double, double, double, unsigned int, QGraphicsItem*)
{
}

/* . */
Object::Object(EmbPoint_, unsigned int, QGraphicsItem*)
{
}

/* . */
Object::Object(EmbPath_, int, QPainterPath const&, unsigned int, QGraphicsItem*)
{
}

/* WARNING: DO NOT enable QGraphicsItem::ItemIsMovable. If it is enabled,
 * and the item is double clicked, the scene will erratically move the item while zooming.
 * All movement has to be handled explicitly by us, not by the scene.
 */
void
Object::init(EmbArc arc, QRgb rgb, Qt::PenStyle lineType)
{
    setData(OBJ_TYPE, type());
    setData(OBJ_NAME, OBJ_NAME_ARC);

    setFlag(QGraphicsItem::ItemIsSelectable, true);

    geometry->object.arc = arc;
    geometry->object.color.r = qRed(rgb);
    geometry->object.color.g = qGreen(rgb);
    geometry->object.color.b = qBlue(rgb);
    geometry->lineType = lineType;
    geometry->type = EMB_ARC;

    calculateArcData(geometry->object.arc);

    setPos(arc.start.x, arc.start.y);
    setObjectLineWeight(0.35); /* TODO: pass in proper lineweight */
    setPen(objPen);
}

/* . */
void
Object::calculateArcData(EmbArc arc)
{
    EmbVector center = emb_arc_center(arc);

    double radius = emb_vector_distance(center, arc.mid);
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
    objPen.setWidthF(0); //NOTE: The objPen will always be cosmetic

    if (lineWeight < 0) {
        if (lineWeight == OBJ_LWT_BYLAYER) {
            lwtPen.setWidthF(0.35); //TODO: getLayerLineWeight
        }
        else if (lineWeight == OBJ_LWT_BYBLOCK) {
            lwtPen.setWidthF(0.35); //TODO: getBlockLineWeight
        }
        else {
            QMessageBox::warning(0, QObject::tr("Error - Negative Lineweight"),
                                    QObject::tr("Lineweight: %1")
                                    .arg(QString().setNum(lineWeight)));
            qDebug("Lineweight cannot be negative! Inverting sign.");
            lwtPen.setWidthF(-lineWeight);
        }
    }
    else {
        lwtPen.setWidthF(lineWeight);
    }
}

/* . */
void
Object::realRender(QPainter*, QPainterPath const&)
{

}

/* . */
void
Object::updatePath(QPainterPath const& p)
{
}

/* . */
void
Object::setObjectColor(const QColor& color)
{
    objPen.setColor(color);
    lwtPen.setColor(color);
}

/* . */
void
Object::setObjectColorRGB(QRgb rgb)
{
    objPen.setColor(QColor(rgb));
    lwtPen.setColor(QColor(rgb));
}

/* . */
void
Object::setObjectLineType(Qt::PenStyle lineType)
{
    objPen.setStyle(lineType);
    lwtPen.setStyle(lineType);
}

/* . */
QPointF
Object::objectRubberPoint(const QString& key) const
{
    if (objRubberPoints.contains(key)) {
        return objRubberPoints.value(key);
    }

    QGraphicsScene* gscene = scene();
    if (gscene) {
        return scene()->property("SCENE_QSNAP_POINT").toPointF();
    }
    return QPointF();
}

/* . */
QString
Object::objectRubberText(const QString& key) const
{
    if (objRubberTexts.contains(key)) {
        return objRubberTexts.value(key);
    }
    return QString();
}

/* . */
QRectF
Object::boundingRect() const
{
    /* If gripped, force this object to be drawn even if it is offscreen. */
    if (objectRubberMode() == OBJ_RUBBER_GRIP) {
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
void
Object::setObjectRadius(double radius)
{
    switch (geometry->type) {
    case EMB_ARC: {
        geometry->object.arc = emb_arc_set_radius(geometry->object.arc, radius);
        calculateArcData(geometry->object.arc);
        break;
    }
    case EMB_CIRCLE:
        setObjectDiameter(radius*2.0);
        break;
    default:
        break;
    }
}

/* . */
void
Object::setObjectDiameter(double diameter)
{
    switch (geometry->type) {
    case EMB_CIRCLE: {
        QRectF circRect;
        circRect.setWidth(diameter);
        circRect.setHeight(diameter);
        circRect.moveCenter(QPointF(0,0));
        setRect(circRect);
        //FIXME: updatePath();
        break;
    }
    default:
        break;
    }
}

/* . */
void
Object::setObjectArea(double area)
{
    switch (geometry->type) {
    case EMB_CIRCLE: {
        double radius = sqrt(area / embConstantPi);
        setObjectRadius(radius);
        break;
    }
    default:
        break;
    }
}

/* . */
void
Object::setObjectCircumference(double circumference)
{
    switch (geometry->type) {
    case EMB_CIRCLE: {
        double diameter = circumference / embConstantPi;
        setObjectDiameter(diameter);
        break;
    }
    default:
        break;
    }
}

/* . */
int
Object::findIndex(const QPointF& point)
{
    int elemCount = normalPath.elementCount();
    //NOTE: Points here are in item coordinates
    QPointF itemPoint = mapFromScene(point);
    for (int i = 0; i < elemCount; i++) {
        QPainterPath::Element e = normalPath.elementAt(i);
        QPointF elemPoint = QPointF(e.x, e.y);
        if (itemPoint == elemPoint) return i;
    }
    return -1;
}

void
Object::updateRubber(QPainter* painter)
{
    switch (geometry->type) {
    case EMB_ARC:
        //TODO: Arc Rubber Modes

        //TODO: updateRubber() gripping for ArcObject
        break;
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
    qDebug("vulcanize()");
    // FIXME: updateRubber(painter);

    setObjectRubberMode(OBJ_RUBBER_OFF);

    switch (geometry->type) {
    case EMB_POLYLINE:
        if (!normalPath.elementCount()) {
            QMessageBox::critical(0,
                QObject::tr("Empty Polyline Error"),
                QObject::tr("The polyline added contains no points. The command that created this object has flawed logic."));
        }
        break;
    case EMB_POLYGON:
        if (!normalPath.elementCount()) {
            QMessageBox::critical(0,
                QObject::tr("Empty Polygon Error"),
                QObject::tr("The polygon added contains no points. The command that created this object has flawed logic."));
        }
        break;
    case EMB_PATH:
        if (!normalPath.elementCount()) {
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
            << objectQuadrant0()
            << objectQuadrant90()
            << objectQuadrant180()
            << objectQuadrant270();
        break;
    }
    case EMB_DIM_LEADER: {
        gripPoints << objectEndPoint1() << objectEndPoint2();
        if (curved) {
            gripPoints << objectMidPoint();
        }
        break;
    }
    case EMB_IMAGE: {
        gripPoints << objectTopLeft()
             << objectTopRight()
             << objectBottomLeft()
             << objectBottomRight();
        break;
    }
    case EMB_LINE: {
        gripPoints << objectEndPoint1() << objectEndPoint2() << objectMidPoint();
        break;
    }
    case EMB_PATH: {
        gripPoints << scenePos(); //TODO: loop thru all path Elements and return their points
        break;
    }
    case EMB_POLYGON:
    case EMB_POLYLINE: {
        QPainterPath::Element element;
        for (int i = 0; i < normalPath.elementCount(); ++i) {
            element = normalPath.elementAt(i);
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
        //TODO: gripEdit() for ArcObject
        break;
    }
    case EMB_CIRCLE: {
        if (before == objectCenter()) {
            QPointF delta = after-before;
            moveBy(delta.x(), delta.y());
        }
        else {
            setObjectRadius(QLineF(objectCenter(), after).length());
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
        //TODO: gripEdit() for EllipseObject
        break;
    }
    case EMB_IMAGE:
    case EMB_RECT: {
        QPointF delta = after-before;
        if (before == objectTopLeft()) {
            setObjectRect(after.x(), after.y(), objectWidth()-delta.x(),
                objectHeight()-delta.y());
        }
        else if (before == objectTopRight()) {
            setObjectRect(objectTopLeft().x(), objectTopLeft().y()+delta.y(),
                objectWidth()+delta.x(), objectHeight()-delta.y());
        }
        else if (before == objectBottomLeft()) {
            setObjectRect(objectTopLeft().x()+delta.x(), objectTopLeft().y(),
                objectWidth()-delta.x(), objectHeight()+delta.y());
        }
        else if (before == objectBottomRight()) {
            setObjectRect(objectTopLeft().x(), objectTopLeft().y(),
                objectWidth()+delta.x(), objectHeight()+delta.y());
        }
        break;
    }
    case EMB_PATH: {
        //TODO: gripEdit() for PathObject
        break;
    }
    case EMB_POLYGON:
    case EMB_POLYLINE: {
        gripIndex = findIndex(before);
        if (gripIndex == -1) {
            return;
        }
        QPointF a = mapFromScene(after);
        normalPath.setElementPositionAt(gripIndex, a.x(), a.y());
        updatePath(normalPath);
        gripIndex = -1;
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
        double startAngle = (objectStartAngle() + rotation())*16;
        double spanAngle = objectIncludedAngle()*16;

        if (objectClockwise()) {
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
        painter->drawPath(lineStylePath);
        painter->drawPath(arrowStylePath);

        if (filled) {
            painter->fillPath(arrowStylePath, objPen.color());
        }
        break;
    }
    case EMB_LINE: {
        if (objectRubberMode() != OBJ_RUBBER_LINE) {
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
        painter->drawPath(objectPath());
        break;
    }
    case EMB_POLYGON: {
        if (normalPath.elementCount()) {
            painter->drawPath(normalPath);
            QPainterPath::Element zero = normalPath.elementAt(0);
            QPainterPath::Element last = normalPath.elementAt(normalPath.elementCount()-1);
            painter->drawLine(QPointF(zero.x, zero.y), QPointF(last.x, last.y));
        }
        break;
    }
    case EMB_POLYLINE: {
        painter->drawPath(normalPath);

        if (objScene->property("ENABLE_LWT").toBool()
            && objScene->property("ENABLE_REAL").toBool()) {
            realRender(painter, normalPath);
        }
        break;
    }
    case EMB_TEXT_SINGLE: {
        painter->drawPath(objTextPath);
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
    return normalPath;
}

QPainterPath
Object::objectSavePath() const
{
    switch (geometry->type) {
    case EMB_PATH:
    case EMB_POLYLINE: {
        double s = scale();
        QTransform trans;
        trans.rotate(rotation());
        trans.scale(s,s);
        return trans.map(normalPath);
    }
    case EMB_POLYGON: {
        QPainterPath closedPath = normalPath;
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
    return normalPath;
}
