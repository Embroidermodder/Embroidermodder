/*
 *  Embroidermodder 2.
 *
 *  ------------------------------------------------------------
 *
 *  Copyright 2013-2022 The Embroidermodder Team
 *  Embroidermodder 2 is Open Source Software.
 *  See LICENSE for licensing terms.
 *
 *  ------------------------------------------------------------
 *
 *  Use Python's PEP7 style guide.
 *      https://peps.python.org/pep-0007/
 */

/**
 * \file object-rect.cpp
 */

#include "embroidermodder.h"

/**
 * @brief DimLeaderObject::DimLeaderObject
 * @param x1
 * @param y1
 * @param x2
 * @param y2
 * @param rgb
 * @param parent
 */
DimLeaderObject::DimLeaderObject(EmbReal x1, EmbReal y1, EmbReal x2, EmbReal y2, QRgb rgb, QGraphicsItem* parent) : BaseObject(parent)
{
    debug_message("DimLeaderObject Constructor()");
    init(x1, y1, x2, y2, rgb, Qt::SolidLine); //TODO: getCurrentLineType
}

/**
 * @brief DimLeaderObject::DimLeaderObject
 * @param obj
 * @param parent
 */
DimLeaderObject::DimLeaderObject(DimLeaderObject* obj, QGraphicsItem* parent) : BaseObject(parent)
{
    debug_message("DimLeaderObject Constructor()");
    if (obj)
    {
        init(obj->objectX1(), obj->objectY1(), obj->objectX2(), obj->objectY2(), obj->objectColorRGB(), Qt::SolidLine); //TODO: getCurrentLineType
    }
}

/**
 * @brief DimLeaderObject::~DimLeaderObject
 */
DimLeaderObject::~DimLeaderObject()
{
    debug_message("DimLeaderObject Destructor()");
}

/**
 * @brief DimLeaderObject::init
 * @param x1
 * @param y1
 * @param x2
 * @param y2
 * @param rgb
 * @param lineType
 */
void
DimLeaderObject::init(EmbReal x1, EmbReal y1, EmbReal x2, EmbReal y2, QRgb rgb, Qt::PenStyle lineType)
{
    setData(OBJ_TYPE, OBJ_TYPE_DIMLEADER);
    setData(OBJ_NAME, "Dimension Leader");

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

/**
 * @brief DimLeaderObject::setObjectEndPoint1
 * @param endPt1
 */
void
DimLeaderObject::setObjectEndPoint1(const QPointF& endPt1)
{
    setObjectEndPoint1(endPt1.x(), endPt1.y());
}

/**
 * @brief DimLeaderObject::setObjectEndPoint1
 * @param x1
 * @param y1
 */
void
DimLeaderObject::setObjectEndPoint1(EmbReal x1, EmbReal y1)
{
    QPointF endPt2 = objectEndPoint2();
    EmbReal x2 = endPt2.x();
    EmbReal y2 = endPt2.y();
    EmbReal dx = x2 - x1;
    EmbReal dy = y2 - y1;
    setRotation(0);
    setLine(0, 0, dx, dy);
    setPos(x1, y1);
    updateLeader();
}

/**
 * @brief DimLeaderObject::setObjectEndPoint2
 * @param endPt2
 */
void
DimLeaderObject::setObjectEndPoint2(const QPointF& endPt2)
{
    setObjectEndPoint2(endPt2.x(), endPt2.y());
}

/**
 * @brief DimLeaderObject::setObjectEndPoint2
 * @param x2
 * @param y2
 */
void
DimLeaderObject::setObjectEndPoint2(EmbReal x2, EmbReal y2)
{
    QPointF endPt1 = scenePos();
    EmbReal x1 = endPt1.x();
    EmbReal y1 = endPt1.y();
    EmbReal dx = x2 - x1;
    EmbReal dy = y2 - y1;
    setRotation(0);
    setLine(0, 0, dx, dy);
    setPos(x1, y1);
    updateLeader();
}

/**
 * @brief DimLeaderObject::objectEndPoint1
 * @return
 */
QPointF
DimLeaderObject::objectEndPoint1() const
{
    return scenePos();
}

/**
 * @brief DimLeaderObject::objectEndPoint2
 * @return
 */
QPointF
DimLeaderObject::objectEndPoint2() const
{
    QLineF lyne = line();
    EmbReal rot = radians(rotation());
    EmbReal cosRot = qCos(rot);
    EmbReal sinRot = qSin(rot);
    EmbReal x2 = lyne.x2()*scale();
    EmbReal y2 = lyne.y2()*scale();
    EmbReal rotEnd2X = x2*cosRot - y2*sinRot;
    EmbReal rotEnd2Y = x2*sinRot + y2*cosRot;

    return (scenePos() + QPointF(rotEnd2X, rotEnd2Y));
}

/**
 * @brief DimLeaderObject::objectMidPoint
 * @return
 */
QPointF
DimLeaderObject::objectMidPoint() const
{
    QLineF lyne = line();
    QPointF mp = lyne.pointAt(0.5);
    EmbReal rot = radians(rotation());
    EmbReal cosRot = qCos(rot);
    EmbReal sinRot = qSin(rot);
    EmbReal mx = mp.x()*scale();
    EmbReal my = mp.y()*scale();
    EmbReal rotMidX = mx*cosRot - my*sinRot;
    EmbReal rotMidY = mx*sinRot + my*cosRot;

    return (scenePos() + QPointF(rotMidX, rotMidY));
}

/**
 * @brief DimLeaderObject::objectAngle
 * @return
 */
EmbReal
DimLeaderObject::objectAngle() const
{
    EmbReal angle = line().angle() - rotation();
    return std::fmod(angle, 360.0);
}

/**
 * @brief DimLeaderObject::updateLeader
 */
void
DimLeaderObject::updateLeader()
{
    int arrowStyle = Closed; //TODO: Make this customizable
    EmbReal arrowStyleAngle = 15.0; //TODO: Make this customizable
    EmbReal arrowStyleLength = 1.0; //TODO: Make this customizable
    EmbReal lineStyleAngle = 45.0; //TODO: Make this customizable
    EmbReal lineStyleLength = 1.0; //TODO: Make this customizable

    QLineF lyne = line();
    EmbReal angle = lyne.angle();
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

/**
 * @brief DimLeaderObject::paint
 * @param painter
 * @param option
 */
void
DimLeaderObject::paint(QPainter* painter, const QStyleOptionGraphicsItem* option, QWidget* /*widget*/)
{
    QGraphicsScene* objScene = scene();
    if (!objScene) return;

    QPen paintPen = pen();
    painter->setPen(paintPen);
    updateRubber(painter);
    if (option->state & QStyle::State_Selected)  { paintPen.setStyle(Qt::DashLine); }
    if (objScene->property("ENABLE_LWT").toBool()) { paintPen = lineWeightPen(); }
    painter->setPen(paintPen);

    painter->drawPath(lineStylePath);
    painter->drawPath(arrowStylePath);

    if (filled)
        painter->fillPath(arrowStylePath, objectColor());
}
/**
 * @brief DimLeaderObject::updateRubber
 * @param painter
 */
void
DimLeaderObject::updateRubber(QPainter* painter)
{
    int rubberMode = objRubberMode;
    if (rubberMode == OBJ_RUBBER_DIMLEADER_LINE) {
        QPointF sceneStartPoint = objectRubberPoint("DIMLEADER_LINE_START");
        QPointF sceneQSnapPoint = objectRubberPoint("DIMLEADER_LINE_END");

        setObjectEndPoint1(sceneStartPoint);
        setObjectEndPoint2(sceneQSnapPoint);
    }
    else if (rubberMode == OBJ_RUBBER_GRIP) {
        if (painter) {
            QPointF gripPoint = objectRubberPoint("GRIP_POINT");
            if (gripPoint == objectEndPoint1()) {
                painter->drawLine(line().p2(), mapFromScene(objectRubberPoint(QString())));
            }
            else if (gripPoint == objectEndPoint2()) {
                painter->drawLine(line().p1(), mapFromScene(objectRubberPoint(QString())));
            }
            else if (gripPoint == objectMidPoint()) {
                painter->drawLine(line().translated(mapFromScene(objectRubberPoint(QString()))-mapFromScene(gripPoint)));
            }
        }
    }
}

/**
 * @brief DimLeaderObject::vulcanize
 */
void
DimLeaderObject::vulcanize()
{
    debug_message("DimLeaderObject vulcanize()");
    updateRubber();

    setObjectRubberMode(OBJ_RUBBER_OFF);
}

/**
 * Returns the closest snap point to the mouse point
 *
 * \todo generic closest point from list to point x.
 */
QPointF DimLeaderObject::mouseSnapPoint(const QPointF& mousePoint)
{
    QPointF endPoint1 = objectEndPoint1();
    QPointF endPoint2 = objectEndPoint2();
    QPointF midPoint  = objectMidPoint();

    EmbReal end1Dist = QLineF(mousePoint, endPoint1).length();
    EmbReal end2Dist = QLineF(mousePoint, endPoint2).length();
    EmbReal midDist  = QLineF(mousePoint, midPoint).length();

    EmbReal minDist = std::min(end1Dist, end2Dist);

    if (curved) {
        minDist = std::min(minDist, midDist);
    }

    if     (minDist == end1Dist) return endPoint1;
    else if (minDist == end2Dist) return endPoint2;
    else if (minDist == midDist)  return midPoint;

    return scenePos();
}

/**
 * @brief DimLeaderObject::allGripPoints
 * @return
 */
QList<QPointF>
DimLeaderObject::allGripPoints()
{
    QList<QPointF> gripPoints;
    gripPoints << objectEndPoint1() << objectEndPoint2();
    if (curved)
        gripPoints << objectMidPoint();
    return gripPoints;
}

/**
 * @brief DimLeaderObject::gripEdit
 * @param before
 * @param after
 */
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
        QPointF delta = after-before;
        moveBy(delta.x(), delta.y());
    }
}
