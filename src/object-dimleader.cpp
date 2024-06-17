/*
 * Embroidermodder 2.
 *
 * Copyright 2011-2024 The Embroidermodder Team
 * Embroidermodder 2 is Open Source Software, see LICENSE.md for licensing terms.
 * Visit https://www.libembroidery.org/refman for advice on altering this file,
 * or read the markdown version in embroidermodder2/docs/refman.
 *
 * Dimension Leader
 */

#include "embroidermodder.h"

#include <QPainter>
#include <QStyleOption>
#include <QGraphicsScene>

DimLeaderObject::DimLeaderObject(qreal x1, qreal y1, qreal x2, qreal y2, QRgb rgb, QGraphicsItem* parent) : BaseObject(parent)
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

DimLeaderObject::~DimLeaderObject()
{
    qDebug("DimLeaderObject Destructor()");
}

void DimLeaderObject::init(qreal x1, qreal y1, qreal x2, qreal y2, QRgb rgb, Qt::PenStyle lineType)
{
    setData(OBJ_TYPE, type());
    setData(OBJ_NAME, OBJ_NAME_DIMLEADER);

    //WARNING: DO NOT enable QGraphicsItem::ItemIsMovable. If it is enabled,
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

void DimLeaderObject::setObjectEndPoint1(const QPointF& endPt1)
{
    setObjectEndPoint1(endPt1.x(), endPt1.y());
}

void DimLeaderObject::setObjectEndPoint1(qreal x1, qreal y1)
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

void DimLeaderObject::setObjectEndPoint2(const QPointF& endPt2)
{
    setObjectEndPoint2(endPt2.x(), endPt2.y());
}

void DimLeaderObject::setObjectEndPoint2(qreal x2, qreal y2)
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

QPointF DimLeaderObject::objectEndPoint1() const
{
    return scenePos();
}

QPointF DimLeaderObject::objectEndPoint2() const
{
    QLineF lyne = line();
    QPointF endPoint2(lyne.x2(), lyne.y2());
    return scenePos() + scale_and_rotate(endPoint2, scale(), rotation());
}

QPointF DimLeaderObject::objectMidPoint() const
{
    QLineF lyne = line();
    QPointF mp = lyne.pointAt(0.5);
    return scenePos() + scale_and_rotate(mp, scale(), rotation());
}

qreal DimLeaderObject::objectAngle() const
{
    qreal angle = line().angle() - rotation();
    while (angle >= 360.0) {
        angle -= 360.0;
    }
    while (angle < 0.0) {
        angle += 360.0;
    }
    return angle;
}

void DimLeaderObject::updateLeader()
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

void DimLeaderObject::paint(QPainter* painter, const QStyleOptionGraphicsItem* option, QWidget* /*widget*/)
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

    painter->drawPath(lineStylePath);
    painter->drawPath(arrowStylePath);

    if (filled) {
        painter->fillPath(arrowStylePath, objectColor());
    }
}

void DimLeaderObject::updateRubber(QPainter* painter)
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

void DimLeaderObject::vulcanize()
{
    qDebug("DimLeaderObject vulcanize()");
    updateRubber();

    setObjectRubberMode(OBJ_RUBBER_OFF);
}

// Returns the closest snap point to the mouse point
QPointF DimLeaderObject::mouseSnapPoint(const QPointF& mousePoint)
{
    return find_mouse_snap_point(allGripPoints(), mousePoint);
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

void DimLeaderObject::gripEdit(const QPointF& before, const QPointF& after)
{
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
}
