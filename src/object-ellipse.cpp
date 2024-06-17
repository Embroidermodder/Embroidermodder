/*
 * Embroidermodder 2.
 *
 * Copyright 2013-2024 The Embroidermodder Team
 * Embroidermodder 2 is Open Source Software.
 * See LICENSE for licensing terms.
 *
 * Command: about
 */

#include "embroidermodder.h"

#include <QPainter>
#include <QStyleOption>
#include <QGraphicsScene>

#define ELLIPSE_MODE_MAJDIA_MINRAD_ONE     0
#define ELLIPSE_MODE_MAJDIA_MINRAD_TWO     1
#define ELLIPSE_MODE_MAJDIA_MINRAD_THREE   2
#define ELLIPSE_MODE_MAJRAD_MINRAD_ONE     3
#define ELLIPSE_MODE_MAJRAD_MINRAD_TWO     4
#define ELLIPSE_MODE_MAJRAD_MINRAD_THREE   5
#define ELLIPSE_MODE_ROTATION              6
#define ELLIPSE_MODE_NEUTRAL               7

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
global.ellipse;
global.rot;
global.mode;

function main()
{
    init_command();
    clear_selection();
    global.mode = ELLIPSE_MODE_MAJDIA_MINRAD_ONE;
    global.point1 = zero_vector;
    global.point2 = zero_vector;
    global.point3 = zero_vector;
    prompt_output(translate("Specify first axis start point or [Center]: "));
}

function click(EmbVector v)
{
    switch (global.mode) {
    case ELLIPSE_MODE_MAJDIA_MINRAD: {
        if (isNaN(global.x1)) {
            global.point1 = v;
            addRubber("ELLIPSE");
            setRubberMode("ELLIPSE_LINE");
            setRubberPoint("ELLIPSE_LINE_POINT1", global.x1, global.y1);
            prompt_output(translate("Specify first axis end point: "));
        }
        else if (isNaN(global.x2)) {
            global.point2 = v;
            global.cx = (global.x1 + global.x2)/2.0;
            global.cy = (global.y1 + global.y2)/2.0;
            global.width = calculateDistance(global.x1, global.y1, global.x2, global.y2);
            global.rot = calculateAngle(global.x1, global.y1, global.x2, global.y2);
            setRubberMode("ELLIPSE_MAJDIA_MINRAD");
            setRubberPoint("ELLIPSE_AXIS1_POINT1", global.x1, global.y1);
            setRubberPoint("ELLIPSE_AXIS1_POINT2", global.x2, global.y2);
            setRubberPoint("ELLIPSE_CENTER", global.cx, global.cy);
            setRubberPoint("ELLIPSE_WIDTH", global.width, 0);
            setRubberPoint("ELLIPSE_ROT", global.rot, 0);
            prompt_output(translate("Specify second axis end point or [Rotation]: "));
        }
        else if (isNaN(global.x3)) {
            global.x3 = x;
            global.y3 = y;
            global.height = perpendicularDistance(global.x3, global.y3, global.x1, global.y1, global.x2, global.y2)*2.0;
            setRubberPoint("ELLIPSE_AXIS2_POINT2", global.x3, global.y3);
            vulcanize();
            end_command();
        }
        else {
            error("ELLIPSE", translate("This should never happen."));
        }
        break;
    }
    case ELLIPSE_MODE_MAJRAD_MINRAD: {
        if (isNaN(global.x1)) {
            global.point1 = v;
            global.center = global.point1;
            addRubber("ELLIPSE");
            setRubberMode("ELLIPSE_LINE");
            setRubberPoint("ELLIPSE_LINE_POINT1", global.x1, global.y1);
            setRubberPoint("ELLIPSE_CENTER", global.cx, global.cy);
            prompt_output(translate("Specify first axis end point: "));
        }
        else if (isNaN(global.x2)) {
            global.point2 = v;
            global.width = calculateDistance(global.cx, global.cy, global.x2, global.y2)*2.0;
            global.rot = calculateAngle(global.x1, global.y1, global.x2, global.y2);
            setRubberMode("ELLIPSE_MAJRAD_MINRAD");
            setRubberPoint("ELLIPSE_AXIS1_POINT2", global.x2, global.y2);
            setRubberPoint("ELLIPSE_WIDTH", global.width, 0);
            setRubberPoint("ELLIPSE_ROT", global.rot, 0);
            prompt_output(translate("Specify second axis end point or [Rotation]: "));
        }
        else if (isNaN(global.x3)) {
            global.point3 = v;
            global.height = perpendicularDistance(global.x3, global.y3, global.cx, global.cy, global.x2, global.y2)*2.0;
            setRubberPoint("ELLIPSE_AXIS2_POINT2", global.x3, global.y3);
            vulcanize();
            end_command();
        }
        else {
            error("ELLIPSE", translate("This should never happen."));
        }
        break;
    }
    case ELLIPSE_MODE_ROTATION: {
        if (isNaN(global.x1)) {
            error("ELLIPSE", translate("This should never happen."));
        }
        else if (isNaN(global.x2)) {
            error("ELLIPSE", translate("This should never happen."));
        }
        else if (isNaN(global.x3)) {
            var angle = calculateAngle(global.cx, global.cy, x, y);
            global.height = Math.cos(angle*Math.PI/180.0)*global.width;
            addEllipse(global.cx, global.cy, global.width, global.height, global.rot, false);
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
    switch (global.mode) {
    case ELLIPSE_MODE_MAJDIA_MINRAD_ONE: {
        if (str == "C" || str == "CENTER") {
            global.mode = ELLIPSE_MODE_MAJRAD_MINRAD;
            prompt_output(translate("Specify center point: "));
            break;
        }

        if (parse_vector(str, &v)) {
            alert(translate("Point or option keyword required."));
            prompt_output(translate("Specify first axis start point or [Center]: "));
        }
        else {
            global.point1 = v;
            addRubber("ELLIPSE");
            setRubberMode("ELLIPSE_LINE");
            setRubberPoint("ELLIPSE_LINE_POINT1", global.x1, global.y1);
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
            global.point2 = v;
            global.center = emb_vector_average(global.point1, global.point2);
            global.width = calculateDistance(global.point1, global.point2);
            global.rot = calculateAngle(global.point1, global.point2);
            setRubberMode("ELLIPSE_MAJDIA_MINRAD");
            setRubberPoint("ELLIPSE_AXIS1_POINT1", global.x1, global.y1);
            setRubberPoint("ELLIPSE_AXIS1_POINT2", global.x2, global.y2);
            setRubberPoint("ELLIPSE_CENTER", global.cx, global.cy);
            setRubberPoint("ELLIPSE_WIDTH", global.width, 0);
            setRubberPoint("ELLIPSE_ROT", global.rot, 0);
            prompt_output(translate("Specify second axis end point or [Rotation]: "));
            mode = ELLIPSE_MODE_MAJDIA_MINRAD_THREE;
        }
        break;
    }
    case ELLIPSE_MODE_MAJDIA_MINRAD_THREE: {
        if (str == "R" || str == "ROTATION") {
            global.mode = ELLIPSE_MODE_ROTATION;
            prompt_output(translate("Specify rotation: "));
            break;
        }

        if (parse_vector(str, &v)) {
            alert(translate("Point or option keyword required."));
            prompt_output(translate("Specify second axis end point or [Rotation]: "));
        }
        else {
            global.point3 = v;
            global.height = perpendicularDistance(global.x3, global.y3, global.x1, global.y1, global.x2, global.y2)*2.0;
            setRubberPoint("ELLIPSE_AXIS2_POINT2", global.x3, global.y3);
            vulcanize();
            end_command();
            global.mode = ELLIPSE_MODE_NEUTRAL;
        }
        break;
    }
    case ELLIPSE_MODE_MAJRAD_MINRAD_ONE: {
        if (parse_vector(str, &v)) {
            alert(translate("Invalid point."));
            prompt_output(translate("Specify center point: "));
        }
        else {
            global.point1 = v;
            global.center = global.point1;
            addRubber("ELLIPSE");
            setRubberMode("ELLIPSE_LINE");
            setRubberPoint("ELLIPSE_LINE_POINT1", global.x1, global.y1);
            setRubberPoint("ELLIPSE_CENTER", global.cx, global.cy);
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
            global.point2 = v;
            global.width = calculateDistance(global.x1, global.y1, global.x2, global.y2)*2.0;
            global.rot = calculateAngle(global.x1, global.y1, global.x2, global.y2);
            setRubberMode("ELLIPSE_MAJRAD_MINRAD");
            setRubberPoint("ELLIPSE_AXIS1_POINT2", global.x2, global.y2);
            setRubberPoint("ELLIPSE_WIDTH", global.width, 0);
            setRubberPoint("ELLIPSE_ROT", global.rot, 0);
            prompt_output(translate("Specify second axis end point or [Rotation]: "));
        }
        break;
    }
    case ELLIPSE_MODE_MAJRAD_MINRAD_THREE: {
        if (str == "R" || str == "ROTATION") {
            global.mode = ELLIPSE_MODE_ROTATION;
            prompt_output(translate("Specify ellipse rotation: "));
        }
        else {
            if (parse_vector(str, &v)) {
                alert(translate("Point or option keyword required."));
                prompt_output(translate("Specify second axis end point or [Rotation]: "));
            }
            else {
                global.point3 = v;
                global.height = perpendicularDistance(global.x3, global.y3, global.x1, global.y1, global.x2, global.y2)*2.0;
                setRubberPoint("ELLIPSE_AXIS2_POINT2", global.x3, global.y3);
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
            global.height = cos(angle * embConstantPi / 180.0) * global.width;
            addEllipse(global.center, global.width, global.height, global.rot, false);
            end_command();
        }
        break;
    }
    default:
        break;
    }
}
#endif

EllipseObject::EllipseObject(qreal centerX, qreal centerY, qreal width, qreal height, QRgb rgb, QGraphicsItem* parent) : BaseObject(parent)
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

EllipseObject::~EllipseObject()
{
    qDebug("EllipseObject Destructor()");
}

void EllipseObject::init(qreal centerX, qreal centerY, qreal width, qreal height, QRgb rgb, Qt::PenStyle lineType)
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
    setPen(objectPen());
    updatePath();
}

void EllipseObject::setObjectSize(qreal width, qreal height)
{
    QRectF elRect = rect();
    elRect.setWidth(width);
    elRect.setHeight(height);
    elRect.moveCenter(QPointF(0,0));
    setRect(elRect);
}

void EllipseObject::setObjectCenter(const QPointF& center)
{
    setObjectCenter(center.x(), center.y());
}

void EllipseObject::setObjectCenter(qreal centerX, qreal centerY)
{
    setPos(centerX, centerY);
}

void EllipseObject::setObjectCenterX(qreal centerX)
{
    setX(centerX);
}

void EllipseObject::setObjectCenterY(qreal centerY)
{
    setY(centerY);
}

void EllipseObject::setObjectRadiusMajor(qreal radius)
{
    setObjectDiameterMajor(radius*2.0);
}

void EllipseObject::setObjectRadiusMinor(qreal radius)
{
    setObjectDiameterMinor(radius*2.0);
}

void EllipseObject::setObjectDiameterMajor(qreal diameter)
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

void EllipseObject::setObjectDiameterMinor(qreal diameter)
{
    QRectF elRect = rect();
    if (elRect.width() < elRect.height())
        elRect.setWidth(diameter);
    else
        elRect.setHeight(diameter);
    elRect.moveCenter(QPointF(0,0));
    setRect(elRect);
}

QPointF EllipseObject::objectQuadrant0() const
{
    qreal halfW = objectWidth()/2.0;
    qreal rot = radians(rotation());
    qreal x = halfW * cos(rot);
    qreal y = halfW * sin(rot);
    return objectCenter() + QPointF(x,y);
}

QPointF EllipseObject::objectQuadrant90() const
{
    qreal halfH = objectHeight()/2.0;
    qreal rot = radians(rotation()+90.0);
    qreal x = halfH * cos(rot);
    qreal y = halfH * sin(rot);
    return objectCenter() + QPointF(x,y);
}

QPointF EllipseObject::objectQuadrant180() const
{
    qreal halfW = objectWidth()/2.0;
    qreal rot = radians(rotation()+180.0);
    qreal x = halfW * cos(rot);
    qreal y = halfW * sin(rot);
    return objectCenter() + QPointF(x,y);
}

QPointF EllipseObject::objectQuadrant270() const
{
    qreal halfH = objectHeight()/2.0;
    qreal rot = radians(rotation()+270.0);
    qreal x = halfH * cos(rot);
    qreal y = halfH * sin(rot);
    return objectCenter() + QPointF(x,y);
}

void EllipseObject::updatePath()
{
    QPainterPath path;
    QRectF r = rect();
    path.arcMoveTo(r, 0);
    path.arcTo(r, 0, 360);
    //NOTE: Reverse the path so that the inside area isn't considered part of the ellipse
    path.arcTo(r, 0, -360);
    setObjectPath(path);
}

void EllipseObject::paint(QPainter* painter, const QStyleOptionGraphicsItem* option, QWidget* /*widget*/)
{
    QGraphicsScene* objScene = scene();
    if (!objScene) return;

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

    painter->drawEllipse(rect());
}

void EllipseObject::updateRubber(QPainter* painter)
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
            drawRubberLine(itemLine, painter, VIEW_COLOR_CROSSHAIR);
        }
        updatePath();
        break;
    }
    case OBJ_RUBBER_ELLIPSE_MAJORDIAMETER_MINORRADIUS: {
        QPointF sceneAxis1Point1 = objectRubberPoint("ELLIPSE_AXIS1_POINT1");
        QPointF sceneAxis1Point2 = objectRubberPoint("ELLIPSE_AXIS1_POINT2");
        QPointF sceneCenterPoint = objectRubberPoint("ELLIPSE_CENTER");
        QPointF sceneAxis2Point2 = objectRubberPoint("ELLIPSE_AXIS2_POINT2");
        qreal ellipseWidth = objectRubberPoint("ELLIPSE_WIDTH").x();
        qreal ellipseRot = objectRubberPoint("ELLIPSE_ROT").x();

        //TODO: incorporate perpendicularDistance() into libcgeometry
        qreal px = sceneAxis2Point2.x();
        qreal py = sceneAxis2Point2.y();
        qreal x1 = sceneAxis1Point1.x();
        qreal y1 = sceneAxis1Point1.y();
        QLineF line(sceneAxis1Point1, sceneAxis1Point2);
        QLineF norm = line.normalVector();
        qreal dx = px-x1;
        qreal dy = py-y1;
        norm.translate(dx, dy);
        QPointF iPoint;
        norm.intersects(line, &iPoint);
        qreal ellipseHeight = QLineF(px, py, iPoint.x(), iPoint.y()).length()*2.0;

        setObjectCenter(sceneCenterPoint);
        setObjectSize(ellipseWidth, ellipseHeight);
        setRotation(-ellipseRot);

        QPointF itemCenterPoint = mapFromScene(sceneCenterPoint);
        QPointF itemAxis2Point2 = mapFromScene(sceneAxis2Point2);
        QLineF itemLine(itemCenterPoint, itemAxis2Point2);
        if (painter) {
            drawRubberLine(itemLine, painter, VIEW_COLOR_CROSSHAIR);
        }
        updatePath();
        break;
    }
    case OBJ_RUBBER_ELLIPSE_MAJORRADIUS_MINORRADIUS: {
        QPointF sceneAxis1Point2 = objectRubberPoint("ELLIPSE_AXIS1_POINT2");
        QPointF sceneCenterPoint = objectRubberPoint("ELLIPSE_CENTER");
        QPointF sceneAxis2Point2 = objectRubberPoint("ELLIPSE_AXIS2_POINT2");
        qreal ellipseWidth = objectRubberPoint("ELLIPSE_WIDTH").x();
        qreal ellipseRot = objectRubberPoint("ELLIPSE_ROT").x();

        //TODO: incorporate perpendicularDistance() into libcgeometry
        qreal px = sceneAxis2Point2.x();
        qreal py = sceneAxis2Point2.y();
        qreal x1 = sceneCenterPoint.x();
        qreal y1 = sceneCenterPoint.y();
        QLineF line(sceneCenterPoint, sceneAxis1Point2);
        QLineF norm = line.normalVector();
        qreal dx = px-x1;
        qreal dy = py-y1;
        norm.translate(dx, dy);
        QPointF iPoint;
        norm.intersects(line, &iPoint);
        qreal ellipseHeight = QLineF(px, py, iPoint.x(), iPoint.y()).length()*2.0;

        setObjectCenter(sceneCenterPoint);
        setObjectSize(ellipseWidth, ellipseHeight);
        setRotation(-ellipseRot);

        QPointF itemCenterPoint = mapFromScene(sceneCenterPoint);
        QPointF itemAxis2Point2 = mapFromScene(sceneAxis2Point2);
        QLineF itemLine(itemCenterPoint, itemAxis2Point2);
        if (painter) {
            drawRubberLine(itemLine, painter, VIEW_COLOR_CROSSHAIR);
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

void EllipseObject::vulcanize()
{
    qDebug("EllipseObject vulcanize()");
    updateRubber();

    setObjectRubberMode(OBJ_RUBBER_OFF);
}

// Returns the closest snap point to the mouse point
QPointF EllipseObject::mouseSnapPoint(const QPointF& mousePoint)
{
    return find_mouse_snap_point(allGripPoints(), mousePoint);
}

QList<QPointF> EllipseObject::allGripPoints()
{
    QList<QPointF> gripPoints;
    gripPoints << objectCenter() << objectQuadrant0() << objectQuadrant90() << objectQuadrant180() << objectQuadrant270();
    return gripPoints;
}

void EllipseObject::gripEdit(const QPointF& before, const QPointF& after)
{
    //TODO: gripEdit() for EllipseObject
}

QPainterPath EllipseObject::objectSavePath() const
{
    QPainterPath path;
    QRectF r = rect();
    path.arcMoveTo(r, 0);
    path.arcTo(r, 0, 360);

    qreal s = scale();
    QTransform trans;
    trans.rotate(rotation());
    trans.scale(s,s);
    return trans.map(path);
}
