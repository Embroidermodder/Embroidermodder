/*
 * Embroidermodder 2
 * Copyright 2011-2025 The Embroidermodder Team
 *
 * Embroidermodder 2 is free and open software under the zlib license:
 * see LICENSE.md for details.
 *
 * ----------------------------------------------------------------------------
 */

#include "embroidermodder.h"

#include <QDebug>
#include <QPainter>
#include <QStyleOption>
#include <QGraphicsScene>
#include <QMessageBox>
#include <QDateTime>

/*
 * Tools for drawing and manipulating EmbArcs via the GUI.
 */

ArcObject::ArcObject(qreal startX, qreal startY, qreal midX, qreal midY, qreal endX, qreal endY, QRgb rgb, QGraphicsItem* parent) : BaseObject(parent)
{
    debug("ArcObject Constructor()");
    init(startX, startY, midX, midY, endX, endY, rgb, Qt::SolidLine); /* TODO: getCurrentLineType */
}

ArcObject::ArcObject(ArcObject* obj, QGraphicsItem* parent) : BaseObject(parent)
{
    debug("ArcObject Constructor()");
    if (obj) {
        QPointF start = obj->objectStartPoint();
        QPointF mid = obj->objectMidPoint();
        QPointF end = obj->objectEndPoint();
        init(start.x(), start.y(), mid.x(), mid.y(), end.x(), end.y(),
            obj->objectColorRGB(), Qt::SolidLine); /* TODO: getCurrentLineType */
        setRotation(obj->rotation());
    }
}

ArcObject::~ArcObject()
{
    debug("ArcObject Destructor()");
}

void
ArcObject::init(qreal startX, qreal startY, qreal midX, qreal midY, qreal endX, qreal endY, QRgb rgb, Qt::PenStyle lineType)
{
    setData(OBJ_TYPE, type());
    setData(OBJ_NAME, OBJ_NAME_ARC);

    /* WARNING: DO NOT enable QGraphicsItem::ItemIsMovable. If it is enabled, */
    //WARNING: and the item is double clicked, the scene will erratically move the item while zooming.
    //WARNING: All movement has to be handled explicitly by us, not by the scene.
    setFlag(QGraphicsItem::ItemIsSelectable, true);

    calculateArcData(startX, startY, midX, midY, endX, endY);

    setObjectColor(rgb);
    setObjectLineType(lineType);
    setObjectLineWeight(0.35); //TODO: pass in proper lineweight
    setPen(objectPen());
}

void
ArcObject::calculateArcData(qreal startX, qreal startY, qreal midX, qreal midY, qreal endX, qreal endY)
{
    EmbError error = EMB_NO_ERR;
    EmbGeometry arc = emb_arc(startX, startY, midX, midY, endX, endY);
    EmbVector center = emb_center(&arc, &error);
    if (error) {
        debug("Failed to find center.");
    }

    arcStartPoint = QPointF(startX - center.x, startY - center.y);
    arcMidPoint = QPointF(midX - center.x, midY - center.y);
    arcEndPoint = QPointF(endX - center.x, endY - center.y);

    setPos(center.x, center.y);

    qreal radius = QLineF(center.x, center.y, midX, midY).length();
    updateArcRect(radius);
    updatePath();
    setRotation(0);
    setScale(1);
}

void
ArcObject::updateArcRect(qreal radius)
{
    QRectF arcRect;
    arcRect.setWidth(radius*2.0);
    arcRect.setHeight(radius*2.0);
    arcRect.moveCenter(QPointF(0,0));
    setRect(arcRect);
}

void
ArcObject::setObjectCenter(const QPointF& point)
{
    setObjectCenter(point.x(), point.y());
}

void
ArcObject::setObjectCenter(qreal pointX, qreal pointY)
{
    setPos(pointX, pointY);
}

void
ArcObject::setObjectCenterX(qreal pointX)
{
    setX(pointX);
}

void
ArcObject::setObjectCenterY(qreal pointY)
{
    setY(pointY);
}

void
ArcObject::setObjectRadius(qreal radius)
{
    qreal rad;
    if (radius <= 0) {
        rad = 0.0000001;
    }
    else
        rad = radius;

    QPointF center = scenePos();
    QLineF startLine = QLineF(center, objectStartPoint());
    QLineF midLine = QLineF(center, objectMidPoint());
    QLineF endLine = QLineF(center, objectEndPoint());
    startLine.setLength(rad);
    midLine.setLength(rad);
    endLine.setLength(rad);
    arcStartPoint = startLine.p2();
    arcMidPoint = midLine.p2();
    arcEndPoint = endLine.p2();

    calculateArcData(arcStartPoint.x(), arcStartPoint.y(), arcMidPoint.x(), arcMidPoint.y(), arcEndPoint.x(), arcEndPoint.y());
}

void
ArcObject::setObjectStartAngle(qreal angle)
{
    //TODO: ArcObject setObjectStartAngle
}

void
ArcObject::setObjectEndAngle(qreal angle)
{
    //TODO: ArcObject setObjectEndAngle
}

void
ArcObject::setObjectStartPoint(const QPointF& point)
{
    setObjectStartPoint(point.x(), point.y());
}

void
ArcObject::setObjectStartPoint(qreal pointX, qreal pointY)
{
    calculateArcData(pointX, pointY, arcMidPoint.x(), arcMidPoint.y(), arcEndPoint.x(), arcEndPoint.y());
}

void
ArcObject::setObjectMidPoint(const QPointF& point)
{
    setObjectMidPoint(point.x(), point.y());
}

void
ArcObject::setObjectMidPoint(qreal pointX, qreal pointY)
{
    calculateArcData(arcStartPoint.x(), arcStartPoint.y(), pointX, pointY, arcEndPoint.x(), arcEndPoint.y());
}

void
ArcObject::setObjectEndPoint(const QPointF& point)
{
    setObjectEndPoint(point.x(), point.y());
}

void
ArcObject::setObjectEndPoint(qreal pointX, qreal pointY)
{
    calculateArcData(arcStartPoint.x(), arcStartPoint.y(), arcMidPoint.x(), arcMidPoint.y(), pointX, pointY);
}

qreal ArcObject::objectStartAngle() const
{
    return line_angle(QLineF(scenePos(), objectStartPoint()));
}

qreal ArcObject::objectEndAngle() const
{
    return line_angle(QLineF(scenePos(), objectEndPoint()));
}

QPointF
ArcObject::objectStartPoint() const
{
    return scenePos() + scale_and_rotate(arcStartPoint, scale(), rotation());
}

QPointF
ArcObject::objectMidPoint() const
{
    return scenePos() + scale_and_rotate(arcMidPoint, scale(), rotation());
}

QPointF
ArcObject::objectEndPoint() const
{
    return scenePos() + scale_and_rotate(arcEndPoint, scale(), rotation());
}

qreal ArcObject::objectArea() const
{
    //Area of a circular segment
    qreal r = objectRadius();
    qreal theta = radians(objectIncludedAngle());
    return ((r*r)/2)*(theta - qSin(theta));
}

qreal ArcObject::objectArcLength() const
{
    return radians(objectIncludedAngle())*objectRadius();
}

qreal ArcObject::objectChord() const
{
    return QLineF(objectStartPoint(), objectEndPoint()).length();
}

qreal ArcObject::objectIncludedAngle() const
{
    qreal chord = objectChord();
    qreal rad = objectRadius();
    if (chord <= 0 || rad <= 0) return 0; //Prevents division by zero and non-existant circles

    //NOTE: Due to floating point rounding errors, we need to clamp the quotient so it is in the range [-1, 1]
    //      If the quotient is out of that range, then the result of asin() will be NaN.
    qreal quotient = chord/(2.0*rad);
    if (quotient > 1.0) quotient = 1.0;
    if (quotient < 0.0) quotient = 0.0; //NOTE: 0 rather than -1 since we are enforcing a positive chord and radius
    return degrees(2.0*asin(quotient)); //Properties of a Circle - Get the Included Angle - Reference: ASD9
}

bool ArcObject::objectClockwise() const
{
    // NOTE: Y values are inverted here on purpose
    EmbGeometry arc = emb_arc(objectStartPoint().x(), -objectStartPoint().y(),
        objectMidPoint().x(), -objectMidPoint().y(),
        objectEndPoint().x(), -objectEndPoint().y());
    EmbError error = EMB_NO_ERR;
    bool clockwise = emb_clockwise(&arc, &error);
    return clockwise;
}

void
ArcObject::updatePath()
{
    qreal startAngle = (objectStartAngle() + rotation());
    qreal spanAngle = objectIncludedAngle();

    if (objectClockwise())
        spanAngle = -spanAngle;

    QPainterPath path;
    path.arcMoveTo(rect(), startAngle);
    path.arcTo(rect(), startAngle, spanAngle);
    //NOTE: Reverse the path so that the inside area isn't considered part of the arc
    path.arcTo(rect(), startAngle+spanAngle, -spanAngle);
    setObjectPath(path);
}

void
ArcObject::paint(QPainter* painter, const QStyleOptionGraphicsItem* option, QWidget* /*widget*/)
{
    QGraphicsScene* objScene = scene();
    if (!objScene) return;

    QPen paintPen = pen();
    painter->setPen(paintPen);
    updateRubber(painter);
    if (option->state & QStyle::State_Selected) { paintPen.setStyle(Qt::DashLine); }
    if (objScene->property(ENABLE_LWT).toBool()) { paintPen = lineWeightPen(); }
    painter->setPen(paintPen);

    qreal startAngle = (objectStartAngle() + rotation())*16;
    qreal spanAngle = objectIncludedAngle()*16;

    if (objectClockwise())
        spanAngle = -spanAngle;

    qreal rad = objectRadius();
    QRectF paintRect(-rad, -rad, rad*2.0, rad*2.0);
    painter->drawArc(paintRect, startAngle, spanAngle);
}

void
ArcObject::updateRubber(QPainter* painter)
{
    //TODO: Arc Rubber Modes

    //TODO: updateRubber() gripping for ArcObject

}

void
ArcObject::vulcanize()
{
    debug("ArcObject vulcanize()");
    updateRubber();

    setObjectRubberMode(OBJ_RUBBER_OFF);
}

QList<QPointF> ArcObject::allGripPoints()
{
    QList<QPointF> gripPoints;
    gripPoints << objectCenter() << objectStartPoint() << objectMidPoint() << objectEndPoint();
    return gripPoints;
}

void
ArcObject::gripEdit(const QPointF& before, const QPointF& after)
{
    //TODO: gripEdit() for ArcObject
}

/*
 * Tools for drawing and manipulating geometric objects via the GUI.
 */

BaseObject::BaseObject(QGraphicsItem* parent) : QGraphicsPathItem(parent)
{
    debug("BaseObject Constructor()");

    objPen.setCapStyle(Qt::RoundCap);
    objPen.setJoinStyle(Qt::RoundJoin);
    lwtPen.setCapStyle(Qt::RoundCap);
    lwtPen.setJoinStyle(Qt::RoundJoin);

    objID = QDateTime::currentMSecsSinceEpoch();
}

BaseObject::~BaseObject()
{
    debug("BaseObject Destructor()");
}

void
BaseObject::setObjectColor(const QColor& color)
{
    objPen.setColor(color);
    lwtPen.setColor(color);
}

void
BaseObject::setObjectColorRGB(QRgb rgb)
{
    objPen.setColor(QColor(rgb));
    lwtPen.setColor(QColor(rgb));
}

void
BaseObject::setObjectLineType(Qt::PenStyle lineType)
{
    objPen.setStyle(lineType);
    lwtPen.setStyle(lineType);
}

void
BaseObject::setObjectLineWeight(qreal lineWeight)
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

QPointF
BaseObject::objectRubberPoint(const QString& key) const
{
    if (objRubberPoints.contains(key))
        return objRubberPoints.value(key);

    QGraphicsScene* gscene = scene();
    if (gscene)
        return scene()->property(SCENE_QSNAP_POINT).toPointF();
    return QPointF();
}

QString BaseObject::objectRubberText(const QString& key) const
{
    if (objRubberTexts.contains(key))
        return objRubberTexts.value(key);
    return QString();
}

QRectF BaseObject::boundingRect() const
{
    //If gripped, force this object to be drawn even if it is offscreen
    if (objectRubberMode() == OBJ_RUBBER_GRIP)
        return scene()->sceneRect();
    return path().boundingRect();
}

void
BaseObject::drawRubberLine(const QLineF& rubLine, QPainter* painter, const char* colorFromScene)
{
    if (painter) {
        QGraphicsScene* objScene = scene();
        if (!objScene) return;
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
    QColor color1 = objectColor();       //lighter color
    QColor color2  = color1.darker(150); //darker color

    //If we have a dark color, lighten it
    int darkness = color1.lightness();
    int threshold = 32; //TODO: This number may need adjusted or maybe just add it to settings.
    if (darkness < threshold) {
        color2 = color1;
        if (!darkness) { color1 = QColor(threshold, threshold, threshold); } //lighter() does not affect pure black
        else { color1 = color2.lighter(100 + threshold); }
    }

    int count = renderPath.elementCount();
    for (int i = 0; i < count-1; ++i) {
        QPainterPath::Element elem = renderPath.elementAt(i);
        QPainterPath::Element next = renderPath.elementAt(i+1);

        if (next.isMoveTo()) continue;

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

/*
 * Tools for drawing and manipulating EmbCircles via the GUI.
 */

CircleObject::CircleObject(qreal centerX, qreal centerY, qreal radius, QRgb rgb, QGraphicsItem* parent) : BaseObject(parent)
{
    debug("CircleObject Constructor()");
    init(centerX, centerY, radius, rgb, Qt::SolidLine); /* TODO: getCurrentLineType */
}

CircleObject::CircleObject(CircleObject* obj, QGraphicsItem* parent) : BaseObject(parent)
{
    debug("CircleObject Constructor()");
    if (obj) {
        init(obj->objectCenterX(), obj->objectCenterY(), obj->objectRadius(), obj->objectColorRGB(), Qt::SolidLine); /* TODO: getCurrentLineType */
        setRotation(obj->rotation());
    }
}

CircleObject::~CircleObject()
{
    debug("CircleObject Destructor()");
}

void
CircleObject::init(qreal centerX, qreal centerY, qreal radius, QRgb rgb, Qt::PenStyle lineType)
{
    setData(OBJ_TYPE, type());
    setData(OBJ_NAME, OBJ_NAME_CIRCLE);

    /* WARNING: DO NOT enable QGraphicsItem::ItemIsMovable. If it is enabled, */
    //WARNING: and the item is double clicked, the scene will erratically move the item while zooming.
    //WARNING: All movement has to be handled explicitly by us, not by the scene.
    setFlag(QGraphicsItem::ItemIsSelectable, true);

    setObjectRadius(radius);
    setObjectCenter(centerX, centerY);
    setObjectColor(rgb);
    setObjectLineType(lineType);
    setObjectLineWeight(0.35); //TODO: pass in proper lineweight
    setPen(objectPen());
    updatePath();
}

void
CircleObject::setObjectCenter(const QPointF& center)
{
    setObjectCenter(center.x(), center.y());
}

void
CircleObject::setObjectCenter(qreal centerX, qreal centerY)
{
    setPos(centerX, centerY);
}

void
CircleObject::setObjectCenterX(qreal centerX)
{
    setX(centerX);
}

void
CircleObject::setObjectCenterY(qreal centerY)
{
    setY(centerY);
}

void
CircleObject::setObjectRadius(qreal radius)
{
    setObjectDiameter(radius*2.0);
}

void
CircleObject::setObjectDiameter(qreal diameter)
{
    QRectF circRect;
    circRect.setWidth(diameter);
    circRect.setHeight(diameter);
    circRect.moveCenter(QPointF(0,0));
    setRect(circRect);
    updatePath();
}

void
CircleObject::setObjectArea(qreal area)
{
    qreal radius = qSqrt(area/pi());
    setObjectRadius(radius);
}

void
CircleObject::setObjectCircumference(qreal circumference)
{
    qreal diameter = circumference/pi();
    setObjectDiameter(diameter);
}

void
CircleObject::updatePath()
{
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
}

void
CircleObject::paint(QPainter* painter, const QStyleOptionGraphicsItem* option, QWidget* /*widget*/)
{
    QGraphicsScene* objScene = scene();
    if (!objScene) return;

    QPen paintPen = pen();
    painter->setPen(paintPen);
    updateRubber(painter);
    if (option->state & QStyle::State_Selected) { paintPen.setStyle(Qt::DashLine); }
    if (objScene->property(ENABLE_LWT).toBool()) { paintPen = lineWeightPen(); }
    painter->setPen(paintPen);

    painter->drawEllipse(rect());
}

void
CircleObject::updateRubber(QPainter* painter)
{
    int rubberMode = objectRubberMode();
    if (rubberMode == OBJ_RUBBER_CIRCLE_1P_RAD) {
        QPointF sceneCenterPoint = objectRubberPoint("CIRCLE_CENTER");
        QPointF sceneQSnapPoint = objectRubberPoint("CIRCLE_RADIUS");
        QPointF itemCenterPoint = mapFromScene(sceneCenterPoint);
        QPointF itemQSnapPoint = mapFromScene(sceneQSnapPoint);
        QLineF itemLine(itemCenterPoint, itemQSnapPoint);
        setObjectCenter(sceneCenterPoint);
        QLineF sceneLine(sceneCenterPoint, sceneQSnapPoint);
        qreal radius = sceneLine.length();
        setObjectRadius(radius);
        if (painter) drawRubberLine(itemLine, painter, VIEW_COLOR_CROSSHAIR);
        updatePath();
    }
    else if (rubberMode == OBJ_RUBBER_CIRCLE_1P_DIA) {
        QPointF sceneCenterPoint = objectRubberPoint("CIRCLE_CENTER");
        QPointF sceneQSnapPoint = objectRubberPoint("CIRCLE_DIAMETER");
        QPointF itemCenterPoint = mapFromScene(sceneCenterPoint);
        QPointF itemQSnapPoint = mapFromScene(sceneQSnapPoint);
        QLineF itemLine(itemCenterPoint, itemQSnapPoint);
        setObjectCenter(sceneCenterPoint);
        QLineF sceneLine(sceneCenterPoint, sceneQSnapPoint);
        qreal diameter = sceneLine.length();
        setObjectDiameter(diameter);
        if (painter) drawRubberLine(itemLine, painter, VIEW_COLOR_CROSSHAIR);
        updatePath();
    }
    else if (rubberMode == OBJ_RUBBER_CIRCLE_2P) {
        QPointF sceneTan1Point = objectRubberPoint("CIRCLE_TAN1");
        QPointF sceneQSnapPoint = objectRubberPoint("CIRCLE_TAN2");
        QLineF sceneLine(sceneTan1Point, sceneQSnapPoint);
        setObjectCenter(sceneLine.pointAt(0.5));
        qreal diameter = sceneLine.length();
        setObjectDiameter(diameter);
        updatePath();
    }
    else if (rubberMode == OBJ_RUBBER_CIRCLE_3P) {
        QPointF sceneTan1Point = objectRubberPoint("CIRCLE_TAN1");
        QPointF sceneTan2Point = objectRubberPoint("CIRCLE_TAN2");
        QPointF sceneTan3Point = objectRubberPoint("CIRCLE_TAN3");

        EmbGeometry arc = emb_arc(
            sceneTan1Point.x(), sceneTan1Point.y(),
            sceneTan2Point.x(), sceneTan2Point.y(),
            sceneTan3Point.x(), sceneTan3Point.y());
        EmbError error = EMB_NO_ERR;
        EmbVector sceneCenter = emb_center(&arc, &error);
        QPointF sceneCenterPoint(sceneCenter.x, sceneCenter.y);
        QLineF sceneLine(sceneCenterPoint, sceneTan3Point);
        setObjectCenter(sceneCenterPoint);
        qreal radius = sceneLine.length();
        setObjectRadius(radius);
        updatePath();
    }
    else if (rubberMode == OBJ_RUBBER_GRIP) {
        if (painter) {
            QPointF gripPoint = objectRubberPoint("GRIP_POINT");
            if (gripPoint == objectCenter()) {
                painter->drawEllipse(rect().translated(mapFromScene(objectRubberPoint(QString()))-mapFromScene(gripPoint)));
            }
            else {
                qreal gripRadius = QLineF(objectCenter(), objectRubberPoint(QString())).length();
                painter->drawEllipse(QPointF(), gripRadius, gripRadius);
            }

            QLineF rubLine(mapFromScene(gripPoint), mapFromScene(objectRubberPoint(QString())));
            drawRubberLine(rubLine, painter, VIEW_COLOR_CROSSHAIR);
        }
    }
}

void
CircleObject::vulcanize()
{
    qDebug("CircleObject vulcanize()");
    updateRubber();

    setObjectRubberMode(OBJ_RUBBER_OFF);
}

QList<QPointF>
CircleObject::allGripPoints(void)
{
    QList<QPointF> gripPoints;
    gripPoints << objectCenter() << objectQuadrant0() << objectQuadrant90()
        << objectQuadrant180() << objectQuadrant270();
    return gripPoints;
}

void
CircleObject::gripEdit(const QPointF& before, const QPointF& after)
{
    if (before == objectCenter()) {
        QPointF delta = after-before;
        moveBy(delta.x(), delta.y());
    }
    else {
        setObjectRadius(QLineF(objectCenter(), after).length());
    }
}

QPainterPath CircleObject::objectSavePath() const
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

/*
 * Tools for drawing and manipulating EmbDimLeaders via the GUI.
 */

DimLeaderObject::DimLeaderObject(qreal x1, qreal y1, qreal x2, qreal y2, QRgb rgb, QGraphicsItem* parent) : BaseObject(parent)
{
    debug("DimLeaderObject Constructor()");
    init(x1, y1, x2, y2, rgb, Qt::SolidLine); /* TODO: getCurrentLineType */
}

DimLeaderObject::DimLeaderObject(DimLeaderObject* obj, QGraphicsItem* parent) : BaseObject(parent)
{
    debug("DimLeaderObject Constructor()");
    if (obj) {
        init(obj->objectX1(), obj->objectY1(), obj->objectX2(), obj->objectY2(), obj->objectColorRGB(), Qt::SolidLine); /* TODO: getCurrentLineType */
    }
}

DimLeaderObject::~DimLeaderObject()
{
    debug("DimLeaderObject Destructor()");
}

void
DimLeaderObject::init(qreal x1, qreal y1, qreal x2, qreal y2, QRgb rgb, Qt::PenStyle lineType)
{
    setData(OBJ_TYPE, type());
    setData(OBJ_NAME, OBJ_NAME_DIMLEADER);

    /* WARNING: DO NOT enable QGraphicsItem::ItemIsMovable. If it is enabled, */
    //WARNING: and the item is double clicked, the scene will erratically move the item while zooming.
    //WARNING: All movement has to be handled explicitly by us, not by the scene.
    setFlag(QGraphicsItem::ItemIsSelectable, true);

    curved = false;
    filled = true;
    setObjectEndPoint1(x1, y1);
    setObjectEndPoint2(x2, y2);
    setObjectColor(rgb);
    setObjectLineType(lineType);
    setObjectLineWeight(0.35); //TODO: pass in proper lineweight
    setPen(objectPen());
}

void
DimLeaderObject::setObjectEndPoint1(const QPointF& endPt1)
{
    setObjectEndPoint1(endPt1.x(), endPt1.y());
}

void
DimLeaderObject::setObjectEndPoint1(qreal x1, qreal y1)
{
    QPointF endPt2 = objectEndPoint2();
    qreal x2 = endPt2.x();
    qreal y2 = endPt2.y();
    qreal dx = x2 - x1;
    qreal dy = y2 - y1;
    setRotation(0);
    setLine(0, 0, dx, dy);
    setPos(x1, y1);
    updateLeader();
}

void
DimLeaderObject::setObjectEndPoint2(const QPointF& endPt2)
{
    setObjectEndPoint2(endPt2.x(), endPt2.y());
}

void
DimLeaderObject::setObjectEndPoint2(qreal x2, qreal y2)
{
    QPointF endPt1 = scenePos();
    qreal x1 = endPt1.x();
    qreal y1 = endPt1.y();
    qreal dx = x2 - x1;
    qreal dy = y2 - y1;
    setRotation(0);
    setLine(0, 0, dx, dy);
    setPos(x1, y1);
    updateLeader();
}

QPointF
DimLeaderObject::objectEndPoint1() const
{
    return scenePos();
}

QPointF
DimLeaderObject::objectEndPoint2() const
{
    QLineF lyne = line();
    QPointF point(lyne.x2(), lyne.y2());
    return scenePos() + scale_and_rotate(point, scale(), rotation());
}

QPointF
DimLeaderObject::objectMidPoint() const
{
    QPointF mp = line().pointAt(0.5);
    return scenePos() + scale_and_rotate(mp, scale(), rotation());
}

qreal DimLeaderObject::objectAngle() const
{
    return line_angle(line());
}

void
DimLeaderObject::updateLeader()
{
    int arrowStyle = Closed; //TODO: Make this customizable
    qreal arrowStyleAngle = 15.0; //TODO: Make this customizable
    qreal arrowStyleLength = 1.0; //TODO: Make this customizable
    qreal lineStyleAngle = 45.0; //TODO: Make this customizable
    qreal lineStyleLength = 1.0; //TODO: Make this customizable

    QLineF lyne = line();
    qreal angle = lyne.angle();
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
        qreal side = QLineF(ap1, ap2).length();
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
DimLeaderObject::paint(QPainter* painter, const QStyleOptionGraphicsItem* option, QWidget* /*widget*/)
{
    QGraphicsScene* objScene = scene();
    if (!objScene) return;

    QPen paintPen = pen();
    painter->setPen(paintPen);
    updateRubber(painter);
    if (option->state & QStyle::State_Selected) { paintPen.setStyle(Qt::DashLine); }
    if (objScene->property(ENABLE_LWT).toBool()) { paintPen = lineWeightPen(); }
    painter->setPen(paintPen);

    painter->drawPath(lineStylePath);
    painter->drawPath(arrowStylePath);

    if (filled)
        painter->fillPath(arrowStylePath, objectColor());
}

void
DimLeaderObject::updateRubber(QPainter* painter)
{
    int rubberMode = objectRubberMode();
    if (rubberMode == OBJ_RUBBER_DIMLEADER_LINE) {
        QPointF sceneStartPoint = objectRubberPoint("DIMLEADER_LINE_START");
        QPointF sceneQSnapPoint = objectRubberPoint("DIMLEADER_LINE_END");

        setObjectEndPoint1(sceneStartPoint);
        setObjectEndPoint2(sceneQSnapPoint);
    }
    else if (rubberMode == OBJ_RUBBER_GRIP) {
        if (painter) {
            QPointF gripPoint = objectRubberPoint("GRIP_POINT");
            if     (gripPoint == objectEndPoint1()) painter->drawLine(line().p2(), mapFromScene(objectRubberPoint(QString())));
            else if (gripPoint == objectEndPoint2()) painter->drawLine(line().p1(), mapFromScene(objectRubberPoint(QString())));
            else if (gripPoint == objectMidPoint())  painter->drawLine(line().translated(mapFromScene(objectRubberPoint(QString()))-mapFromScene(gripPoint)));
        }
    }
}

void
DimLeaderObject::vulcanize()
{
    qDebug("DimLeaderObject vulcanize()");
    updateRubber();

    setObjectRubberMode(OBJ_RUBBER_OFF);
}

QList<QPointF> DimLeaderObject::allGripPoints()
{
    QList<QPointF> gripPoints;
    gripPoints << objectEndPoint1() << objectEndPoint2();
    if (curved) {
        gripPoints << objectMidPoint();
    }
    return gripPoints;
}

void
DimLeaderObject::gripEdit(const QPointF& before, const QPointF& after)
{
    if (before == objectEndPoint1()) {
        setObjectEndPoint1(after);
    }
    else if (before == objectEndPoint2()) {
        setObjectEndPoint2(after);
    }
    else if (before == objectMidPoint()) {
        QPointF delta = after - before;
        moveBy(delta.x(), delta.y());
    }
}

/*
 * Tools for drawing and manipulating EmbEllipses via the GUI.
 */
EllipseObject::EllipseObject(qreal centerX, qreal centerY, qreal width, qreal height, QRgb rgb, QGraphicsItem* parent) : BaseObject(parent)
{
    debug("EllipseObject Constructor()");
    init(centerX, centerY, width, height, rgb, Qt::SolidLine); /* TODO: getCurrentLineType */
}

EllipseObject::EllipseObject(EllipseObject* obj, QGraphicsItem* parent) : BaseObject(parent)
{
    debug("EllipseObject Constructor()");
    if (obj) {
        init(obj->objectCenterX(), obj->objectCenterY(), obj->objectWidth(), obj->objectHeight(), obj->objectColorRGB(), Qt::SolidLine); /* TODO: getCurrentLineType */
        setRotation(obj->rotation());
    }
}

EllipseObject::~EllipseObject()
{
    debug("EllipseObject Destructor()");
}

void
EllipseObject::init(qreal centerX, qreal centerY, qreal width, qreal height, QRgb rgb, Qt::PenStyle lineType)
{
    setData(OBJ_TYPE, type());
    setData(OBJ_NAME, OBJ_NAME_ELLIPSE);

    /* WARNING: DO NOT enable QGraphicsItem::ItemIsMovable. If it is enabled, */
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

void
EllipseObject::setObjectSize(qreal width, qreal height)
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
EllipseObject::setObjectCenter(qreal centerX, qreal centerY)
{
    setPos(centerX, centerY);
}

void
EllipseObject::setObjectCenterX(qreal centerX)
{
    setX(centerX);
}

void
EllipseObject::setObjectCenterY(qreal centerY)
{
    setY(centerY);
}

void
EllipseObject::setObjectRadiusMajor(qreal radius)
{
    setObjectDiameterMajor(radius*2.0);
}

void
EllipseObject::setObjectRadiusMinor(qreal radius)
{
    setObjectDiameterMinor(radius*2.0);
}

void
EllipseObject::setObjectDiameterMajor(qreal diameter)
{
    QRectF elRect = rect();
    if (elRect.width() > elRect.height())
        elRect.setWidth(diameter);
    else
        elRect.setHeight(diameter);
    elRect.moveCenter(QPointF(0,0));
    setRect(elRect);
}

void
EllipseObject::setObjectDiameterMinor(qreal diameter)
{
    QRectF elRect = rect();
    if (elRect.width() < elRect.height())
        elRect.setWidth(diameter);
    else
        elRect.setHeight(diameter);
    elRect.moveCenter(QPointF(0,0));
    setRect(elRect);
}

QPointF
EllipseObject::objectQuadrant0() const
{
    qreal halfW = objectWidth()/2.0;
    qreal rot = radians(rotation());
    qreal x = halfW*qCos(rot);
    qreal y = halfW*qSin(rot);
    return objectCenter() + QPointF(x,y);
}

QPointF
EllipseObject::objectQuadrant90() const
{
    qreal halfH = objectHeight()/2.0;
    qreal rot = radians(rotation()+90.0);
    qreal x = halfH*qCos(rot);
    qreal y = halfH*qSin(rot);
    return objectCenter() + QPointF(x,y);
}

QPointF
EllipseObject::objectQuadrant180() const
{
    qreal halfW = objectWidth()/2.0;
    qreal rot = radians(rotation()+180.0);
    qreal x = halfW*qCos(rot);
    qreal y = halfW*qSin(rot);
    return objectCenter() + QPointF(x,y);
}

QPointF
EllipseObject::objectQuadrant270() const
{
    qreal halfH = objectHeight()/2.0;
    qreal rot = radians(rotation()+270.0);
    qreal x = halfH*qCos(rot);
    qreal y = halfH*qSin(rot);
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
EllipseObject::paint(QPainter* painter, const QStyleOptionGraphicsItem* option, QWidget* /*widget*/)
{
    QGraphicsScene* objScene = scene();
    if (!objScene) return;

    QPen paintPen = pen();
    painter->setPen(paintPen);
    updateRubber(painter);
    if (option->state & QStyle::State_Selected) { paintPen.setStyle(Qt::DashLine); }
    if (objScene->property(ENABLE_LWT).toBool()) { paintPen = lineWeightPen(); }
    painter->setPen(paintPen);

    painter->drawEllipse(rect());
}

void
EllipseObject::updateRubber(QPainter* painter)
{
    int rubberMode = objectRubberMode();
    if (rubberMode == OBJ_RUBBER_ELLIPSE_LINE) {
        QPointF sceneLinePoint1 = objectRubberPoint("ELLIPSE_LINE_POINT1");
        QPointF sceneLinePoint2 = objectRubberPoint("ELLIPSE_LINE_POINT2");
        QPointF itemLinePoint1  = mapFromScene(sceneLinePoint1);
        QPointF itemLinePoint2  = mapFromScene(sceneLinePoint2);
        QLineF itemLine(itemLinePoint1, itemLinePoint2);
        if (painter) drawRubberLine(itemLine, painter, VIEW_COLOR_CROSSHAIR);
        updatePath();
    }
    else if (rubberMode == OBJ_RUBBER_ELLIPSE_MAJORDIAMETER_MINORRADIUS) {
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
        if (painter) drawRubberLine(itemLine, painter, VIEW_COLOR_CROSSHAIR);
        updatePath();
    }
    else if (rubberMode == OBJ_RUBBER_ELLIPSE_MAJORRADIUS_MINORRADIUS) {
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
        if (painter) drawRubberLine(itemLine, painter, VIEW_COLOR_CROSSHAIR);
        updatePath();
    }
    else if (rubberMode == OBJ_RUBBER_GRIP) {
        //TODO: updateRubber() gripping for EllipseObject
    }
}

void
EllipseObject::vulcanize()
{
    qDebug("EllipseObject vulcanize()");
    updateRubber();

    setObjectRubberMode(OBJ_RUBBER_OFF);
}

QList<QPointF> EllipseObject::allGripPoints()
{
    QList<QPointF> gripPoints;
    gripPoints << objectCenter() << objectQuadrant0() << objectQuadrant90()
        << objectQuadrant180() << objectQuadrant270();
    return gripPoints;
}

void
EllipseObject::gripEdit(const QPointF& before, const QPointF& after)
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

/*
 * Tools for drawing and manipulating EmbImages via the GUI.
 */

ImageObject::ImageObject(qreal x, qreal y, qreal w, qreal h, QRgb rgb, QGraphicsItem* parent) : BaseObject(parent)
{
    debug("ImageObject Constructor()");
    init(x, y, w, h, rgb, Qt::SolidLine); /* TODO: getCurrentLineType */
}

ImageObject::ImageObject(ImageObject* obj, QGraphicsItem* parent) : BaseObject(parent)
{
    debug("ImageObject Constructor()");
    if (obj) {
        QPointF ptl = obj->objectTopLeft();
        init(ptl.x(), ptl.y(), obj->objectWidth(), obj->objectHeight(), obj->objectColorRGB(), Qt::SolidLine); /* TODO: getCurrentLineType */
        setRotation(obj->rotation());
    }
}

ImageObject::~ImageObject()
{
    debug("ImageObject Destructor()");
}

void
ImageObject::init(qreal x, qreal y, qreal w, qreal h, QRgb rgb, Qt::PenStyle lineType)
{
    setData(OBJ_TYPE, type());
    setData(OBJ_NAME, OBJ_NAME_IMAGE);

    /* WARNING: DO NOT enable QGraphicsItem::ItemIsMovable. If it is enabled, */
    //WARNING: and the item is double clicked, the scene will erratically move the item while zooming.
    //WARNING: All movement has to be handled explicitly by us, not by the scene.
    setFlag(QGraphicsItem::ItemIsSelectable, true);

    setObjectRect(x, y, w, h);
    setObjectColor(rgb);
    setObjectLineType(lineType);
    setObjectLineWeight(0.35); //TODO: pass in proper lineweight
    setPen(objectPen());
}

void
ImageObject::setObjectRect(qreal x, qreal y, qreal w, qreal h)
{
    setPos(x, y);
    setRect(0, 0, w, h);
    updatePath();
}

QPointF
ImageObject::objectTopLeft() const
{
    return scenePos() + scale_and_rotate(rect().topLeft(), scale(), rotation());
}

QPointF
ImageObject::objectTopRight() const
{
    return scenePos() + scale_and_rotate(rect().topRight(), scale(), rotation());
}

QPointF
ImageObject::objectBottomLeft() const
{
    return scenePos() + scale_and_rotate(rect().bottomLeft(), scale(), rotation());
}

QPointF
ImageObject::objectBottomRight() const
{
    return scenePos() + scale_and_rotate(rect().bottomRight(), scale(), rotation());
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
ImageObject::paint(QPainter* painter, const QStyleOptionGraphicsItem* option, QWidget* /*widget*/)
{
    QGraphicsScene* objScene = scene();
    if (!objScene) return;

    QPen paintPen = pen();
    painter->setPen(paintPen);
    updateRubber(painter);
    if (option->state & QStyle::State_Selected) { paintPen.setStyle(Qt::DashLine); }
    if (objScene->property(ENABLE_LWT).toBool()) { paintPen = lineWeightPen(); }
    painter->setPen(paintPen);

    painter->drawRect(rect());
}

void
ImageObject::updateRubber(QPainter* painter)
{
    int rubberMode = objectRubberMode();
    if (rubberMode == OBJ_RUBBER_IMAGE) {
        QPointF sceneStartPoint = objectRubberPoint("IMAGE_START");
        QPointF sceneEndPoint = objectRubberPoint("IMAGE_END");
        qreal x = sceneStartPoint.x();
        qreal y = sceneStartPoint.y();
        qreal w = sceneEndPoint.x() - sceneStartPoint.x();
        qreal h = sceneEndPoint.y() - sceneStartPoint.y();
        setObjectRect(x,y,w,h);
        updatePath();
    }
    else if (rubberMode == OBJ_RUBBER_GRIP) {
        //TODO: updateRubber() gripping for ImageObject
    }
}

void
ImageObject::vulcanize()
{
    qDebug("ImageObject vulcanize()");
    updateRubber();

    setObjectRubberMode(OBJ_RUBBER_OFF);
}

QList<QPointF>
ImageObject::allGripPoints()
{
    QList<QPointF> gripPoints;
    gripPoints << objectTopLeft() << objectTopRight() << objectBottomLeft()
        << objectBottomRight();
    return gripPoints;
}

void
ImageObject::gripEdit(const QPointF& before, const QPointF& after)
{
    //TODO: gripEdit() for ImageObject
}

/*
 * Tools for drawing and manipulating EmbLines via the GUI.
 */

LineObject::LineObject(qreal x1, qreal y1, qreal x2, qreal y2, QRgb rgb, QGraphicsItem* parent) : BaseObject(parent)
{
    debug("LineObject Constructor()");
    init(x1, y1, x2, y2, rgb, Qt::SolidLine); /* TODO: getCurrentLineType */
}

LineObject::LineObject(LineObject* obj, QGraphicsItem* parent) : BaseObject(parent)
{
    debug("LineObject Constructor()");
    if (obj) {
        init(obj->objectX1(), obj->objectY1(), obj->objectX2(), obj->objectY2(), obj->objectColorRGB(), Qt::SolidLine); /* TODO: getCurrentLineType */
    }
}

LineObject::~LineObject()
{
    debug("LineObject Destructor()");
}

void
LineObject::init(qreal x1, qreal y1, qreal x2, qreal y2, QRgb rgb, Qt::PenStyle lineType)
{
    setData(OBJ_TYPE, type());
    setData(OBJ_NAME, OBJ_NAME_LINE);

    /* WARNING: DO NOT enable QGraphicsItem::ItemIsMovable. If it is enabled, */
    //WARNING: and the item is double clicked, the scene will erratically move the item while zooming.
    //WARNING: All movement has to be handled explicitly by us, not by the scene.
    setFlag(QGraphicsItem::ItemIsSelectable, true);

    setObjectEndPoint1(x1, y1);
    setObjectEndPoint2(x2, y2);
    setObjectColor(rgb);
    setObjectLineType(lineType);
    setObjectLineWeight(0.35); //TODO: pass in proper lineweight
    setPen(objectPen());
}

void
LineObject::setObjectEndPoint1(const QPointF& endPt1)
{
    setObjectEndPoint1(endPt1.x(), endPt1.y());
}

void
LineObject::setObjectEndPoint1(qreal x1, qreal y1)
{
    QPointF endPt2 = objectEndPoint2();
    qreal x2 = endPt2.x();
    qreal y2 = endPt2.y();
    qreal dx = x2 - x1;
    qreal dy = y2 - y1;
    setRotation(0);
    setScale(1);
    setLine(0, 0, dx, dy);
    setPos(x1, y1);
}

void
LineObject::setObjectEndPoint2(const QPointF& endPt2)
{
    setObjectEndPoint2(endPt2.x(), endPt2.y());
}

void
LineObject::setObjectEndPoint2(qreal x2, qreal y2)
{
    QPointF endPt1 = scenePos();
    qreal x1 = endPt1.x();
    qreal y1 = endPt1.y();
    qreal dx = x2 - x1;
    qreal dy = y2 - y1;
    setRotation(0);
    setScale(1);
    setLine(0, 0, dx, dy);
    setPos(x1, y1);
}

QPointF
LineObject::objectEndPoint2() const
{
    QLineF lyne = line();
    QPointF point(lyne.x2(), lyne.y2());
    return scenePos() + scale_and_rotate(point, scale(), rotation());
}

QPointF
LineObject::objectMidPoint() const
{
    QPointF mp = line().pointAt(0.5);
    return scenePos() + scale_and_rotate(mp, scale(), rotation());
}

qreal LineObject::objectAngle() const
{
    return line_angle(line());
}

void
LineObject::paint(QPainter* painter, const QStyleOptionGraphicsItem* option, QWidget* /*widget*/)
{
    QGraphicsScene* objScene = scene();
    if (!objScene) return;

    QPen paintPen = pen();
    painter->setPen(paintPen);
    updateRubber(painter);
    if (option->state & QStyle::State_Selected) { paintPen.setStyle(Qt::DashLine); }
    if (objScene->property(ENABLE_LWT).toBool()) { paintPen = lineWeightPen(); }
    painter->setPen(paintPen);

    if (objectRubberMode() != OBJ_RUBBER_LINE) painter->drawLine(line());

    if (objScene->property(ENABLE_LWT).toBool() && objScene->property(ENABLE_REAL).toBool()) { realRender(painter, path()); }
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

        drawRubberLine(line(), painter, VIEW_COLOR_CROSSHAIR);
    }
    else if (rubberMode == OBJ_RUBBER_GRIP) {
        if (painter) {
            QPointF gripPoint = objectRubberPoint("GRIP_POINT");
            if     (gripPoint == objectEndPoint1()) painter->drawLine(line().p2(), mapFromScene(objectRubberPoint(QString())));
            else if (gripPoint == objectEndPoint2()) painter->drawLine(line().p1(), mapFromScene(objectRubberPoint(QString())));
            else if (gripPoint == objectMidPoint())  painter->drawLine(line().translated(mapFromScene(objectRubberPoint(QString()))-mapFromScene(gripPoint)));

            QLineF rubLine(mapFromScene(gripPoint), mapFromScene(objectRubberPoint(QString())));
            drawRubberLine(rubLine, painter, VIEW_COLOR_CROSSHAIR);
        }
    }
}

void
LineObject::vulcanize()
{
    qDebug("LineObject vulcanize()");
    updateRubber();

    setObjectRubberMode(OBJ_RUBBER_OFF);
}

QList<QPointF> LineObject::allGripPoints()
{
    QList<QPointF> gripPoints;
    gripPoints << objectEndPoint1() << objectEndPoint2() << objectMidPoint();
    return gripPoints;
}

void
LineObject::gripEdit(const QPointF& before, const QPointF& after)
{
    if     (before == objectEndPoint1()) { setObjectEndPoint1(after); }
    else if (before == objectEndPoint2()) { setObjectEndPoint2(after); }
    else if (before == objectMidPoint()) { QPointF delta = after-before; moveBy(delta.x(), delta.y()); }
}

QPainterPath LineObject::objectSavePath() const
{
    QPainterPath path;
    path.lineTo(objectDeltaX(), objectDeltaY());
    return path;
}

/*
 * Tools for drawing and manipulating EmbPaths via the GUI.
 */

PathObject::PathObject(qreal x, qreal y, const QPainterPath p, QRgb rgb, QGraphicsItem* parent) : BaseObject(parent)
{
    debug("PathObject Constructor()");
    init(x, y, p, rgb, Qt::SolidLine); /* TODO: getCurrentLineType */
}

PathObject::PathObject(PathObject* obj, QGraphicsItem* parent) : BaseObject(parent)
{
    debug("PathObject Constructor()");
    if (obj) {
        init(obj->objectX(), obj->objectY(), obj->objectCopyPath(), obj->objectColorRGB(), Qt::SolidLine); /* TODO: getCurrentLineType */
        setRotation(obj->rotation());
        setScale(obj->scale());
    }
}

PathObject::~PathObject()
{
    debug("PathObject Destructor()");
}

void
PathObject::init(qreal x, qreal y, const QPainterPath& p, QRgb rgb, Qt::PenStyle lineType)
{
    setData(OBJ_TYPE, type());
    setData(OBJ_NAME, OBJ_NAME_PATH);

    /* WARNING: DO NOT enable QGraphicsItem::ItemIsMovable. If it is enabled, */
    //WARNING: and the item is double clicked, the scene will erratically move the item while zooming.
    //WARNING: All movement has to be handled explicitly by us, not by the scene.
    setFlag(QGraphicsItem::ItemIsSelectable, true);

    updatePath(p);
    setObjectPos(x,y);
    setObjectColor(rgb);
    setObjectLineType(lineType);
    setObjectLineWeight(0.35); //TODO: pass in proper lineweight
    setPen(objectPen());
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
PathObject::paint(QPainter* painter, const QStyleOptionGraphicsItem* option, QWidget* /*widget*/)
{
    QGraphicsScene* objScene = scene();
    if (!objScene) return;

    QPen paintPen = pen();
    painter->setPen(paintPen);
    updateRubber(painter);
    if (option->state & QStyle::State_Selected) { paintPen.setStyle(Qt::DashLine); }
    if (objScene->property(ENABLE_LWT).toBool()) { paintPen = lineWeightPen(); }
    painter->setPen(paintPen);

    painter->drawPath(objectPath());
}

void
PathObject::updateRubber(QPainter* painter)
{
    //TODO: Path Rubber Modes

    //TODO: updateRubber() gripping for PathObject

}

void
PathObject::vulcanize()
{
    qDebug("PathObject vulcanize()");
    updateRubber();

    setObjectRubberMode(OBJ_RUBBER_OFF);

    if (!normalPath.elementCount())
        QMessageBox::critical(0, QObject::tr("Empty Path Error"), QObject::tr("The path added contains no points. The command that created this object has flawed logic."));
}

QList<QPointF> PathObject::allGripPoints()
{
    QList<QPointF> gripPoints;
    gripPoints << scenePos(); //TODO: loop thru all path Elements and return their points
    return gripPoints;
}

void
PathObject::gripEdit(const QPointF& before, const QPointF& after)
{
    //TODO: gripEdit() for PathObject
}

QPainterPath PathObject::objectCopyPath() const
{
    return normalPath;
}

QPainterPath PathObject::objectSavePath() const
{
    qreal s = scale();
    QTransform trans;
    trans.rotate(rotation());
    trans.scale(s,s);
    return trans.map(normalPath);
}

/*
 * Tools for drawing and manipulating EmbPoints via the GUI.
 */

PointObject::PointObject(qreal x, qreal y, QRgb rgb, QGraphicsItem* parent) : BaseObject(parent)
{
    debug("PointObject Constructor()");
    init(x, y, rgb, Qt::SolidLine); /* TODO: getCurrentLineType */
}

PointObject::PointObject(PointObject* obj, QGraphicsItem* parent) : BaseObject(parent)
{
    debug("PointObject Constructor()");
    if (obj) {
        init(obj->objectX(), obj->objectY(), obj->objectColorRGB(), Qt::SolidLine); /* TODO: getCurrentLineType */
        setRotation(obj->rotation());
    }
}

PointObject::~PointObject()
{
    debug("PointObject Destructor()");
}

void
PointObject::init(qreal x, qreal y, QRgb rgb, Qt::PenStyle lineType)
{
    setData(OBJ_TYPE, type());
    setData(OBJ_NAME, OBJ_NAME_POINT);

    /* WARNING: DO NOT enable QGraphicsItem::ItemIsMovable. If it is enabled, */
    //WARNING: and the item is double clicked, the scene will erratically move the item while zooming.
    //WARNING: All movement has to be handled explicitly by us, not by the scene.
    setFlag(QGraphicsItem::ItemIsSelectable, true);

    setRect(-0.00000001, -0.00000001, 0.00000002, 0.00000002);
    setObjectPos(x,y);
    setObjectColor(rgb);
    setObjectLineType(lineType);
    setObjectLineWeight(0.35); //TODO: pass in proper lineweight
    setPen(objectPen());
}

void
PointObject::paint(QPainter* painter, const QStyleOptionGraphicsItem* option, QWidget* /*widget*/)
{
    QGraphicsScene* objScene = scene();
    if (!objScene) return;

    QPen paintPen = pen();
    painter->setPen(paintPen);
    updateRubber(painter);
    if (option->state & QStyle::State_Selected) { paintPen.setStyle(Qt::DashLine); }
    if (objScene->property(ENABLE_LWT).toBool()) { paintPen = lineWeightPen(); }
    painter->setPen(paintPen);

    painter->drawPoint(0,0);
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
                drawRubberLine(rubLine, painter, VIEW_COLOR_CROSSHAIR);
            }
        }
    }
}

void
PointObject::vulcanize()
{
    qDebug("PointObject vulcanize()");
    updateRubber();

    setObjectRubberMode(OBJ_RUBBER_OFF);
}

QList<QPointF> PointObject::allGripPoints()
{
    QList<QPointF> gripPoints;
    gripPoints << scenePos();
    return gripPoints;
}

void
PointObject::gripEdit(const QPointF& before, const QPointF& after)
{
    if (before == scenePos()) { QPointF delta = after-before; moveBy(delta.x(), delta.y()); }
}

QPainterPath PointObject::objectSavePath() const
{
    QPainterPath path;
    path.addRect(-0.00000001, -0.00000001, 0.00000002, 0.00000002);
    return path;
}

/*
 * Tools for drawing and manipulating EmbPolygons via the GUI.
 */

PolygonObject::PolygonObject(qreal x, qreal y, const QPainterPath& p, QRgb rgb, QGraphicsItem* parent) : BaseObject(parent)
{
    debug("PolygonObject Constructor()");
    init(x, y, p, rgb, Qt::SolidLine); /* TODO: getCurrentLineType */
}

PolygonObject::PolygonObject(PolygonObject* obj, QGraphicsItem* parent) : BaseObject(parent)
{
    debug("PolygonObject Constructor()");
    if (obj) {
        init(obj->objectX(), obj->objectY(), obj->objectCopyPath(), obj->objectColorRGB(), Qt::SolidLine); /* TODO: getCurrentLineType */
        setRotation(obj->rotation());
        setScale(obj->scale());
    }
}

PolygonObject::~PolygonObject()
{
    debug("PolygonObject Destructor()");
}

void
PolygonObject::init(qreal x, qreal y, const QPainterPath& p, QRgb rgb, Qt::PenStyle lineType)
{
    setData(OBJ_TYPE, type());
    setData(OBJ_NAME, OBJ_NAME_POLYGON);

    /* WARNING: DO NOT enable QGraphicsItem::ItemIsMovable. If it is enabled, */
    //WARNING: and the item is double clicked, the scene will erratically move the item while zooming.
    //WARNING: All movement has to be handled explicitly by us, not by the scene.
    setFlag(QGraphicsItem::ItemIsSelectable, true);

    gripIndex = -1;
    updatePath(p);
    setObjectPos(x,y);
    setObjectColor(rgb);
    setObjectLineType(lineType);
    setObjectLineWeight(0.35); //TODO: pass in proper lineweight
    setPen(objectPen());
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
PolygonObject::paint(QPainter* painter, const QStyleOptionGraphicsItem* option, QWidget* /*widget*/)
{
    QGraphicsScene* objScene = scene();
    if (!objScene) return;

    QPen paintPen = pen();
    painter->setPen(paintPen);
    updateRubber(painter);
    if (option->state & QStyle::State_Selected) { paintPen.setStyle(Qt::DashLine); }
    if (objScene->property(ENABLE_LWT).toBool()) { paintPen = lineWeightPen(); }
    painter->setPen(paintPen);

    if (normalPath.elementCount()) {
        painter->drawPath(normalPath);
        QPainterPath::Element zero = normalPath.elementAt(0);
        QPainterPath::Element last = normalPath.elementAt(normalPath.elementCount()-1);
        painter->drawLine(QPointF(zero.x, zero.y), QPointF(last.x, last.y));
    }
}

void
PolygonObject::updateRubber(QPainter* painter)
{
    int rubberMode = objectRubberMode();
    if (rubberMode == OBJ_RUBBER_POLYGON) {
        setObjectPos(objectRubberPoint("POLYGON_POINT_0"));

        bool ok = false;
        QString numStr = objectRubberText("POLYGON_NUM_POINTS");
        if (numStr.isNull()) return;
        int num = numStr.toInt(&ok);
        if (!ok) return;

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
        qreal inscribeAngle = inscribeLine.angle();
        qreal inscribeInc = 360.0/numSides;

        if (painter) drawRubberLine(inscribeLine, painter, VIEW_COLOR_CROSSHAIR);

        QPainterPath inscribePath;
        /* First Point */
        inscribePath.moveTo(inscribePoint);
        /* Remaining Points */
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
        qreal circumscribeAngle = circumscribeLine.angle();
        qreal circumscribeInc = 360.0/numSides;

        if (painter) drawRubberLine(circumscribeLine, painter, VIEW_COLOR_CROSSHAIR);

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
    }
    else if (rubberMode == OBJ_RUBBER_GRIP) {
        if (painter) {
            int elemCount = normalPath.elementCount();
            QPointF gripPoint = objectRubberPoint("GRIP_POINT");
            if (gripIndex == -1) gripIndex = findIndex(gripPoint);
            if (gripIndex == -1) return;

            int m = 0;
            int n = 0;

            if (!gripIndex) { m = elemCount-1; n = 1; }
            else if (gripIndex == elemCount-1) { m = elemCount-2; n = 0; }
            else          { m = gripIndex-1; n = gripIndex+1; }
            QPainterPath::Element em = normalPath.elementAt(m);
            QPainterPath::Element en = normalPath.elementAt(n);
            QPointF emPoint = QPointF(em.x, em.y);
            QPointF enPoint = QPointF(en.x, en.y);
            painter->drawLine(emPoint, mapFromScene(objectRubberPoint(QString())));
            painter->drawLine(enPoint, mapFromScene(objectRubberPoint(QString())));

            QLineF rubLine(mapFromScene(gripPoint), mapFromScene(objectRubberPoint(QString())));
            drawRubberLine(rubLine, painter, VIEW_COLOR_CROSSHAIR);
        }
    }
}

void
PolygonObject::vulcanize()
{
    qDebug("PolygonObject vulcanize()");
    updateRubber();

    setObjectRubberMode(OBJ_RUBBER_OFF);

    if (!normalPath.elementCount())
        QMessageBox::critical(0, QObject::tr("Empty Polygon Error"), QObject::tr("The polygon added contains no points. The command that created this object has flawed logic."));
}

QList<QPointF> PolygonObject::allGripPoints()
{
    QList<QPointF> gripPoints;
    QPainterPath::Element element;
    for (int i = 0; i < normalPath.elementCount(); ++i) {
        element = normalPath.elementAt(i);
        gripPoints << mapToScene(element.x, element.y);
    }
    return gripPoints;
}

int PolygonObject::findIndex(const QPointF& point)
{
    int i = 0;
    int elemCount = normalPath.elementCount();
    /* NOTE: Points here are in item coordinates */
    QPointF itemPoint = mapFromScene(point);
    for (i = 0; i < elemCount; i++) {
        QPainterPath::Element e = normalPath.elementAt(i);
        QPointF elemPoint = QPointF(e.x, e.y);
        if (itemPoint == elemPoint) return i;
    }
    return -1;
}

void
PolygonObject::gripEdit(const QPointF& before, const QPointF& after)
{
    gripIndex = findIndex(before);
    if (gripIndex == -1) return;
    QPointF a = mapFromScene(after);
    normalPath.setElementPositionAt(gripIndex, a.x(), a.y());
    updatePath(normalPath);
    gripIndex = -1;
}

QPainterPath PolygonObject::objectCopyPath() const
{
    return normalPath;
}

QPainterPath PolygonObject::objectSavePath() const
{
    QPainterPath closedPath = normalPath;
    closedPath.closeSubpath();
    qreal s = scale();
    QTransform trans;
    trans.rotate(rotation());
    trans.scale(s,s);
    return trans.map(closedPath);
}


/*
 * Tools for drawing and manipulating EmbPolylines via the GUI.
 */

/*! Calculate what position the embroidery machine is at given the current time.
 *
 * HACK: this is very inefficient because there is no storage of data from previous
 *       renders.
 *
 * FIXME: this is not per-design, so all loaded designs will simulate together.
 */
QPainterPath
simulation_step(QPainterPath normalPath)
{
    double simulation_time = (current_time() - state.simulation_start)/1000.0;
    char msg[200];
    sprintf(msg, "Simulation time: %lu", simulation_time);
    debug(msg);

    QPainterPath animatePath;
    QPointF start_pt = normalPath.elementAt(0);
    double time = 0.0f;
    animatePath.moveTo(start_pt);
    for (int i=1; i<normalPath.elementCount(); i++) {
        QPointF pt = normalPath.elementAt(i) - start_pt;
        double travelled = std::sqrt(pt.x()*pt.x() + pt.y()*pt.y());
        time += (travelled / state.machine_speed) + state.stitch_time;
        if (time > simulation_time) {
            break;
        }
        animatePath.lineTo(pt);
    }
    return animatePath;
}

PolylineObject::PolylineObject(qreal x, qreal y, const QPainterPath& p, QRgb rgb, QGraphicsItem* parent) : BaseObject(parent)
{
    debug("PolylineObject Constructor()");
    /* TODO: getCurrentLineType */
    init(x, y, p, rgb, Qt::SolidLine);
}

PolylineObject::PolylineObject(PolylineObject* obj, QGraphicsItem* parent) : BaseObject(parent)
{
    debug("PolylineObject Constructor()");
    if (obj) {
        init(obj->objectX(), obj->objectY(), obj->objectCopyPath(), obj->objectColorRGB(), Qt::SolidLine);
        /* TODO: getCurrentLineType */
        setRotation(obj->rotation());
        setScale(obj->scale());
    }
}

PolylineObject::~PolylineObject()
{
    debug("PolylineObject Destructor()");
}

void
PolylineObject::init(qreal x, qreal y, const QPainterPath& p, QRgb rgb, Qt::PenStyle lineType)
{
    setData(OBJ_TYPE, type());
    setData(OBJ_NAME, OBJ_NAME_POLYLINE);

    /* WARNING: DO NOT enable QGraphicsItem::ItemIsMovable. If it is enabled,
     * WARNING: and the item is double clicked, the scene will erratically move the item while zooming.
     * WARNING: All movement has to be handled explicitly by us, not by the scene.
     */
    setFlag(QGraphicsItem::ItemIsSelectable, true);

    gripIndex = -1;
    updatePath(p);
    setObjectPos(x,y);
    setObjectColor(rgb);
    setObjectLineType(lineType);
    setObjectLineWeight(0.35); /* TODO: pass in proper lineweight */
    setPen(objectPen());
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
PolylineObject::paint(QPainter* painter, const QStyleOptionGraphicsItem* option, QWidget* /*widget*/)
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
    if (objScene->property(ENABLE_LWT).toBool()) {
        paintPen = lineWeightPen();
    }
    painter->setPen(paintPen);

    if (state.play_mode) {
        painter->drawPath(simulation_step(normalPath));
        update();
    }
    else {
        painter->drawPath(normalPath);
    }

    if (objScene->property(ENABLE_LWT).toBool() && objScene->property(ENABLE_REAL).toBool()) {
        realRender(painter, normalPath);
    }
}

void
PolylineObject::updateRubber(QPainter* painter)
{
    int rubberMode = objectRubberMode();
    if (rubberMode == OBJ_RUBBER_POLYLINE) {
        setObjectPos(objectRubberPoint("POLYLINE_POINT_0"));

        QLineF rubberLine(normalPath.currentPosition(), mapFromScene(objectRubberPoint(QString())));
        if (painter) drawRubberLine(rubberLine, painter, VIEW_COLOR_CROSSHAIR);

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

        /* Ensure the path isn't updated until the number of points is changed again. */
        setObjectRubberText("POLYLINE_NUM_POINTS", QString());
    }
    else if (rubberMode == OBJ_RUBBER_GRIP) {
        if (painter) {
            int elemCount = normalPath.elementCount();
            QPointF gripPoint = objectRubberPoint("GRIP_POINT");
            if (gripIndex == -1) gripIndex = findIndex(gripPoint);
            if (gripIndex == -1) return;

            if (!gripIndex) {
                /* First */
                QPainterPath::Element ef = normalPath.elementAt(1);
                QPointF efPoint = QPointF(ef.x, ef.y);
                painter->drawLine(efPoint, mapFromScene(objectRubberPoint(QString())));
            }
            else if (gripIndex == elemCount-1) {
                /* Last */
                QPainterPath::Element el = normalPath.elementAt(gripIndex-1);
                QPointF elPoint = QPointF(el.x, el.y);
                painter->drawLine(elPoint, mapFromScene(objectRubberPoint(QString())));
            }
            else {
                /* Middle */
                QPainterPath::Element em = normalPath.elementAt(gripIndex-1);
                QPainterPath::Element en = normalPath.elementAt(gripIndex+1);
                QPointF emPoint = QPointF(em.x, em.y);
                QPointF enPoint = QPointF(en.x, en.y);
                painter->drawLine(emPoint, mapFromScene(objectRubberPoint(QString())));
                painter->drawLine(enPoint, mapFromScene(objectRubberPoint(QString())));
            }

            QLineF rubLine(mapFromScene(gripPoint), mapFromScene(objectRubberPoint(QString())));
            drawRubberLine(rubLine, painter, VIEW_COLOR_CROSSHAIR);
        }
    }
}

void
PolylineObject::vulcanize()
{
    qDebug("PolylineObject vulcanize()");
    updateRubber();

    setObjectRubberMode(OBJ_RUBBER_OFF);

    if (!normalPath.elementCount())
        QMessageBox::critical(0, QObject::tr("Empty Polyline Error"), QObject::tr("The polyline added contains no points. The command that created this object has flawed logic."));
}

QList<QPointF> PolylineObject::allGripPoints()
{
    QList<QPointF> gripPoints;
    QPainterPath::Element element;
    for (int i = 0; i < normalPath.elementCount(); ++i) {
        element = normalPath.elementAt(i);
        gripPoints << mapToScene(element.x, element.y);
    }
    return gripPoints;
}

int PolylineObject::findIndex(const QPointF& point)
{
    int elemCount = normalPath.elementCount();
    /* NOTE: Points here are in item coordinates */
    QPointF itemPoint = mapFromScene(point);
    for (int i = 0; i < elemCount; i++) {
        QPainterPath::Element e = normalPath.elementAt(i);
        QPointF elemPoint = QPointF(e.x, e.y);
        if (itemPoint == elemPoint) return i;
    }
    return -1;
}

void
PolylineObject::gripEdit(const QPointF& before, const QPointF& after)
{
    gripIndex = findIndex(before);
    if (gripIndex == -1) return;
    QPointF a = mapFromScene(after);
    normalPath.setElementPositionAt(gripIndex, a.x(), a.y());
    updatePath(normalPath);
    gripIndex = -1;
}

QPainterPath PolylineObject::objectCopyPath() const
{
    return normalPath;
}

QPainterPath PolylineObject::objectSavePath() const
{
    qreal s = scale();
    QTransform trans;
    trans.rotate(rotation());
    trans.scale(s,s);
    return trans.map(normalPath);
}

/*
 * Tools for drawing and manipulating EmbRects via the GUI.
 */

RectObject::RectObject(qreal x, qreal y, qreal w, qreal h, QRgb rgb, QGraphicsItem* parent) : BaseObject(parent)
{
    debug("RectObject Constructor()");
    init(x, y, w, h, rgb, Qt::SolidLine); /* TODO: getCurrentLineType */
}

RectObject::RectObject(RectObject* obj, QGraphicsItem* parent) : BaseObject(parent)
{
    debug("RectObject Constructor()");
    if (obj) {
        QPointF ptl = obj->objectTopLeft();
        init(ptl.x(), ptl.y(), obj->objectWidth(), obj->objectHeight(), obj->objectColorRGB(), Qt::SolidLine); /* TODO: getCurrentLineType */
        setRotation(obj->rotation());
    }
}

RectObject::~RectObject()
{
    debug("RectObject Destructor()");
}

void
RectObject::init(qreal x, qreal y, qreal w, qreal h, QRgb rgb, Qt::PenStyle lineType)
{
    setData(OBJ_TYPE, type());
    setData(OBJ_NAME, OBJ_NAME_RECTANGLE);

    /* WARNING: DO NOT enable QGraphicsItem::ItemIsMovable. If it is enabled, */
    //WARNING: and the item is double clicked, the scene will erratically move the item while zooming.
    //WARNING: All movement has to be handled explicitly by us, not by the scene.
    setFlag(QGraphicsItem::ItemIsSelectable, true);

    setObjectRect(x, y, w, h);
    setObjectColor(rgb);
    setObjectLineType(lineType);
    setObjectLineWeight(0.35); //TODO: pass in proper lineweight
    setPen(objectPen());
}

void
RectObject::setObjectRect(qreal x, qreal y, qreal w, qreal h)
{
    setPos(x, y);
    setRect(0, 0, w, h);
    updatePath();
}

QPointF
RectObject::objectTopLeft() const
{
    return scenePos() + scale_and_rotate(rect().topLeft(), scale(), rotation());
}

QPointF
RectObject::objectTopRight() const
{
    return scenePos() + scale_and_rotate(rect().topRight(), scale(), rotation());
}

QPointF
RectObject::objectBottomLeft() const
{
    return scenePos() + scale_and_rotate(rect().bottomLeft(), scale(), rotation());
}

QPointF
RectObject::objectBottomRight() const
{
    return scenePos() + scale_and_rotate(rect().bottomRight(), scale(), rotation());
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
    /* NOTE: Reverse the path so that the inside area isn't considered part of the rectangle. */
    path.lineTo(r.topLeft());
    path.lineTo(r.topRight());
    path.lineTo(r.bottomRight());
    path.moveTo(r.bottomLeft());
    setObjectPath(path);
}

void
RectObject::paint(QPainter* painter, const QStyleOptionGraphicsItem* option, QWidget* /*widget*/)
{
    QGraphicsScene* objScene = scene();
    if (!objScene) return;

    QPen paintPen = pen();
    painter->setPen(paintPen);
    updateRubber(painter);
    if (option->state & QStyle::State_Selected) { paintPen.setStyle(Qt::DashLine); }
    if (objScene->property(ENABLE_LWT).toBool()) { paintPen = lineWeightPen(); }
    painter->setPen(paintPen);

    painter->drawRect(rect());
}

void
RectObject::updateRubber(QPainter* painter)
{
    int rubberMode = objectRubberMode();
    if (rubberMode == OBJ_RUBBER_RECTANGLE) {
        QPointF sceneStartPoint = objectRubberPoint("RECTANGLE_START");
        QPointF sceneEndPoint = objectRubberPoint("RECTANGLE_END");
        qreal x = sceneStartPoint.x();
        qreal y = sceneStartPoint.y();
        qreal w = sceneEndPoint.x() - sceneStartPoint.x();
        qreal h = sceneEndPoint.y() - sceneStartPoint.y();
        setObjectRect(x,y,w,h);
        updatePath();
    }
    else if (rubberMode == OBJ_RUBBER_GRIP) {
        if (painter) {
            /* TODO: Make this work with rotation & scaling */
            /*
            QPointF gripPoint = objectRubberPoint("GRIP_POINT");
            QPointF after = objectRubberPoint(QString());
            QPointF delta = after-gripPoint;
            if     (gripPoint == objectTopLeft()) { painter->drawPolygon(mapFromScene(QRectF(after.x(), after.y(), objectWidth()-delta.x(), objectHeight()-delta.y()))); }
            else if (gripPoint == objectTopRight()) { painter->drawPolygon(mapFromScene(QRectF(objectTopLeft().x(), objectTopLeft().y()+delta.y(), objectWidth()+delta.x(), objectHeight()-delta.y()))); }
            else if (gripPoint == objectBottomLeft()) { painter->drawPolygon(mapFromScene(QRectF(objectTopLeft().x()+delta.x(), objectTopLeft().y(), objectWidth()-delta.x(), objectHeight()+delta.y()))); }
            else if (gripPoint == objectBottomRight()) { painter->drawPolygon(mapFromScene(QRectF(objectTopLeft().x(), objectTopLeft().y(), objectWidth()+delta.x(), objectHeight()+delta.y()))); }

            QLineF rubLine(mapFromScene(gripPoint), mapFromScene(objectRubberPoint(QString())));
            drawRubberLine(rubLine, painter, VIEW_COLOR_CROSSHAIR);
            */

            QPointF gripPoint = objectRubberPoint("GRIP_POINT");
            QPointF after = objectRubberPoint(QString());
            QPointF delta = after-gripPoint;

            QLineF rubLine(mapFromScene(gripPoint), mapFromScene(objectRubberPoint(QString())));
            drawRubberLine(rubLine, painter, VIEW_COLOR_CROSSHAIR);
        }
    }
}

void
RectObject::vulcanize()
{
    qDebug("RectObject vulcanize()");
    updateRubber();

    setObjectRubberMode(OBJ_RUBBER_OFF);
}

QList<QPointF> RectObject::allGripPoints()
{
    QList<QPointF> gripPoints;
    gripPoints << objectTopLeft() << objectTopRight() << objectBottomLeft() << objectBottomRight();
    return gripPoints;
}

void
RectObject::gripEdit(const QPointF& before, const QPointF& after)
{
    QPointF delta = after-before;
    if     (before == objectTopLeft()) { setObjectRect(after.x(), after.y(), objectWidth()-delta.x(), objectHeight()-delta.y()); }
    else if (before == objectTopRight()) { setObjectRect(objectTopLeft().x(), objectTopLeft().y()+delta.y(), objectWidth()+delta.x(), objectHeight()-delta.y()); }
    else if (before == objectBottomLeft()) { setObjectRect(objectTopLeft().x()+delta.x(), objectTopLeft().y(), objectWidth()-delta.x(), objectHeight()+delta.y()); }
    else if (before == objectBottomRight()) { setObjectRect(objectTopLeft().x(), objectTopLeft().y(), objectWidth()+delta.x(), objectHeight()+delta.y()); }
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

    qreal s = scale();
    QTransform trans;
    trans.rotate(rotation());
    trans.scale(s,s);
    return trans.map(path);
}

/*
 * Tools for writing and manipulating EmbTextSingles via the GUI.
 */

TextSingleObject::TextSingleObject(const QString& str, qreal x, qreal y, QRgb rgb, QGraphicsItem* parent) : BaseObject(parent)
{
    debug("TextSingleObject Constructor()");
    init(str, x, y, rgb, Qt::SolidLine); /* TODO: getCurrentLineType */
}

TextSingleObject::TextSingleObject(TextSingleObject* obj, QGraphicsItem* parent) : BaseObject(parent)
{
    debug("TextSingleObject Constructor()");
    if (obj) {
        setObjectTextFont(obj->objectTextFont());
        setObjectTextSize(obj->objectTextSize());
        setRotation(obj->rotation());
        setObjectTextBackward(obj->objectTextBackward());
        setObjectTextUpsideDown(obj->objectTextUpsideDown());
        setObjectTextStyle(obj->objectTextBold(), obj->objectTextItalic(), obj->objectTextUnderline(), obj->objectTextStrikeOut(), obj->objectTextOverline());
        init(obj->objectText(), obj->objectX(), obj->objectY(), obj->objectColorRGB(), Qt::SolidLine); /* TODO: getCurrentLineType */
        setScale(obj->scale());
    }
}

TextSingleObject::~TextSingleObject()
{
    debug("TextSingleObject Destructor()");
}

void
TextSingleObject::init(const QString& str, qreal x, qreal y, QRgb rgb, Qt::PenStyle lineType)
{
    setData(OBJ_TYPE, type());
    setData(OBJ_NAME, OBJ_NAME_TEXTSINGLE);

    /* WARNING: DO NOT enable QGraphicsItem::ItemIsMovable. If it is enabled,
     * WARNING: and the item is double clicked, the scene will erratically move the item while zooming.
     * WARNING: All movement has to be handled explicitly by us, not by the scene.
     */
    setFlag(QGraphicsItem::ItemIsSelectable, true);

    objTextJustify = "Left"; //TODO: set the justification properly

    setObjectText(str);
    setObjectPos(x,y);
    setObjectColor(rgb);
    setObjectLineType(lineType);
    setObjectLineWeight(0.35); //TODO: pass in proper lineweight
    setPen(objectPen());
}

QStringList TextSingleObject::objectTextJustifyList() const
{
    QStringList justifyList;
    justifyList << "Left" << "Center" << "Right" /* TODO: << "Aligned" */ << "Middle" /* TODO: << "Fit" */ ;
    justifyList << "Top Left" << "Top Center" << "Top Right";
    justifyList << "Middle Left" << "Middle Center" << "Middle Right";
    justifyList << "Bottom Left" << "Bottom Center" << "Bottom Right";
    return justifyList;
}

void
TextSingleObject::setObjectText(const QString& str)
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
    if     (objTextJustify == "Left") { textPath.translate(-jRect.left(), 0); }
    else if (objTextJustify == "Center") { textPath.translate(-jRect.center().x(), 0); }
    else if (objTextJustify == "Right") { textPath.translate(-jRect.right(), 0); }
    else if (objTextJustify == "Aligned") { } //TODO: TextSingleObject Aligned Justification
    else if (objTextJustify == "Middle") { textPath.translate(-jRect.center()); }
    else if (objTextJustify == "Fit") { } //TODO: TextSingleObject Fit Justification
    else if (objTextJustify == "Top Left") { textPath.translate(-jRect.topLeft()); }
    else if (objTextJustify == "Top Center") { textPath.translate(-jRect.center().x(), -jRect.top()); }
    else if (objTextJustify == "Top Right") { textPath.translate(-jRect.topRight()); }
    else if (objTextJustify == "Middle Left") { textPath.translate(-jRect.left(), -jRect.top()/2.0); }
    else if (objTextJustify == "Middle Center") { textPath.translate(-jRect.center().x(), -jRect.top()/2.0); }
    else if (objTextJustify == "Middle Right") { textPath.translate(-jRect.right(), -jRect.top()/2.0); }
    else if (objTextJustify == "Bottom Left") { textPath.translate(-jRect.bottomLeft()); }
    else if (objTextJustify == "Bottom Center") { textPath.translate(-jRect.center().x(), -jRect.bottom()); }
    else if (objTextJustify == "Bottom Right") { textPath.translate(-jRect.bottomRight()); }

    //Backward or Upside Down
    if (objTextBackward || objTextUpsideDown) {
        qreal horiz = 1.0;
        qreal vert = 1.0;
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

void
TextSingleObject::setObjectTextFont(const QString& font)
{
    objTextFont = font;
    setObjectText(objText);
}

void
TextSingleObject::setObjectTextJustify(const QString& justify)
{
    //Verify the string is a valid option
    if     (justify == "Left") { objTextJustify = justify; }
    else if (justify == "Center") { objTextJustify = justify; }
    else if (justify == "Right") { objTextJustify = justify; }
    else if (justify == "Aligned") { objTextJustify = justify; }
    else if (justify == "Middle") { objTextJustify = justify; }
    else if (justify == "Fit") { objTextJustify = justify; }
    else if (justify == "Top Left") { objTextJustify = justify; }
    else if (justify == "Top Center") { objTextJustify = justify; }
    else if (justify == "Top Right") { objTextJustify = justify; }
    else if (justify == "Middle Left") { objTextJustify = justify; }
    else if (justify == "Middle Center") { objTextJustify = justify; }
    else if (justify == "Middle Right") { objTextJustify = justify; }
    else if (justify == "Bottom Left") { objTextJustify = justify; }
    else if (justify == "Bottom Center") { objTextJustify = justify; }
    else if (justify == "Bottom Right") { objTextJustify = justify; }
    else            { objTextJustify = "Left";  } //Default
    setObjectText(objText);
}

void
TextSingleObject::setObjectTextSize(qreal size)
{
    objTextSize = size;
    setObjectText(objText);
}

void
TextSingleObject::setObjectTextStyle(bool bold, bool italic, bool under, bool strike, bool over)
{
    objTextBold = bold;
    objTextItalic = italic;
    objTextUnderline = under;
    objTextStrikeOut = strike;
    objTextOverline = over;
    setObjectText(objText);
}

void
TextSingleObject::setObjectTextBold(bool val)
{
    objTextBold = val;
    setObjectText(objText);
}

void
TextSingleObject::setObjectTextItalic(bool val)
{
    objTextItalic = val;
    setObjectText(objText);
}

void
TextSingleObject::setObjectTextUnderline(bool val)
{
    objTextUnderline = val;
    setObjectText(objText);
}

void
TextSingleObject::setObjectTextStrikeOut(bool val)
{
    objTextStrikeOut = val;
    setObjectText(objText);
}

void
TextSingleObject::setObjectTextOverline(bool val)
{
    objTextOverline = val;
    setObjectText(objText);
}

void
TextSingleObject::setObjectTextBackward(bool val)
{
    objTextBackward = val;
    setObjectText(objText);
}

void
TextSingleObject::setObjectTextUpsideDown(bool val)
{
    objTextUpsideDown = val;
    setObjectText(objText);
}

void
TextSingleObject::paint(QPainter* painter, const QStyleOptionGraphicsItem* option, QWidget* /*widget*/)
{
    QGraphicsScene* objScene = scene();
    if (!objScene) return;

    QPen paintPen = pen();
    painter->setPen(paintPen);
    updateRubber(painter);
    if (option->state & QStyle::State_Selected) { paintPen.setStyle(Qt::DashLine); }
    if (objScene->property(ENABLE_LWT).toBool()) { paintPen = lineWeightPen(); }
    painter->setPen(paintPen);

    painter->drawPath(objTextPath);
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
            drawRubberLine(rubLine, painter, VIEW_COLOR_CROSSHAIR);
        }
    }
}

void
TextSingleObject::vulcanize()
{
    debug("TextSingleObject vulcanize()");
    updateRubber();

    setObjectRubberMode(OBJ_RUBBER_OFF);
}

QList<QPointF> TextSingleObject::allGripPoints()
{
    QList<QPointF> gripPoints;
    gripPoints << scenePos();
    return gripPoints;
}

void
TextSingleObject::gripEdit(const QPointF& before, const QPointF& after)
{
    if (before == scenePos()) { QPointF delta = after-before; moveBy(delta.x(), delta.y()); }
}

QList<QPainterPath> TextSingleObject::subPathList() const
{
    qreal s = scale();
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

