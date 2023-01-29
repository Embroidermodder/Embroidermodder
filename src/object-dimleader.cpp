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

#include "embroidermodder.h"

#if 0
DimLeaderObject::DimLeaderObject(double x1, double y1, double x2, double y2, unsigned int rgb, QGraphicsItem* parent) : BaseObject(parent)
{
    debug_message("DimLeaderObject Constructor()");
    init(x1, y1, x2, y2, rgb, SolidLine); //TODO: getCurrentLineType
}

DimLeaderObject::DimLeaderObject(DimLeaderObject* obj, QGraphicsItem* parent) : BaseObject(parent)
{
    debug_message("DimLeaderObject Constructor()");
    if (obj) {
        init(obj->objectX1(), obj->objectY1(), obj->objectX2(), obj->objectY2(), obj->objectColorRGB(), SolidLine); //TODO: getCurrentLineType
    }
}

DimLeaderObject::~DimLeaderObject()
{
    debug_message("DimLeaderObject Destructor()");
}

void DimLeaderObject::init(double x1, double y1, double x2, double y2, unsigned int rgb, Qt::PenStyle lineType)
{
    setData(OBJ_TYPE, type);
    setData(OBJ_NAME, "Leader Dimension");

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
    setPen(objPen);
}

void DimLeaderObject::setObjectEndPoint1(const EmbVector& endPt1)
{
    setObjectEndPoint1(endPt1.x(), endPt1.y());
}

void DimLeaderObject::setObjectEndPoint1(double x1, double y1)
{
    EmbVector endPt2 = objectEndPoint2();
    double x2 = endPt2.x();
    double y2 = endPt2.y();
    double dx = x2 - x1;
    double dy = y2 - y1;
    setRotation(0);
    setLine(0, 0, dx, dy);
    setPos(x1, y1);
    updateLeader();
}

void DimLeaderObject::setObjectEndPoint2(const EmbVector& endPt2)
{
    setObjectEndPoint2(endPt2.x(), endPt2.y());
}

void DimLeaderObject::setObjectEndPoint2(double x2, double y2)
{
    EmbVector endPt1 = scenePos();
    double x1 = endPt1.x();
    double y1 = endPt1.y();
    double dx = x2 - x1;
    double dy = y2 - y1;
    setRotation(0);
    setLine(0, 0, dx, dy);
    setPos(x1, y1);
    updateLeader();
}

EmbVector DimLeaderObject::objectEndPoint1() const
{
    return scenePos();
}

EmbVector DimLeaderObject::objectEndPoint2() const
{
    EmbLine lyne = line();
    double rot = radians(rotation());
    EmbVector point2;
    point2.x = lyne.x2()*scale();
    point2.y = lyne.y2()*scale();
    EmbVector rot = embVector_rotate(point2, alpha);

    return (scenePos() + rot);
}

EmbVector DimLeaderObject::objectMidPoint() const
{
    EmbVector mp = line().pointAt(0.5) * scale();
    double alpha = radians(rotation());
    EmbVector rotMid = embVector_rotate(mp, alpha);
    return scenePos() + rotMid;
}

double DimLeaderObject::objectAngle() const
{
    return fmodf(line().angle() - rotation(), 360.0);
}

void DimLeaderObject::updateLeader()
{
    int arrowStyle = Closed; //TODO: Make this customizable
    double arrowStyleAngle = 15.0; //TODO: Make this customizable
    double arrowStyleLength = 1.0; //TODO: Make this customizable
    double lineStyleAngle = 45.0; //TODO: Make this customizable
    double lineStyleLength = 1.0; //TODO: Make this customizable

    EmbLine lyne = line();
    double angle = lyne.angle();
    EmbVector ap0 = lyne.p1();
    EmbVector lp0 = lyne.p2();

    //Arrow
    EmbLine lynePerp(lyne.pointAt(arrowStyleLength/lyne.length()) ,lp0);
    lynePerp.setAngle(angle + 90);
    EmbLine lyne1(ap0, lp0);
    EmbLine lyne2(ap0, lp0);
    lyne1.setAngle(angle + arrowStyleAngle);
    lyne2.setAngle(angle - arrowStyleAngle);
    EmbVector ap1;
    EmbVector ap2;
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

    if (arrowStyle == Open)
    {
        arrowStylePath = QPainterPath();
        arrowStylePath.moveTo(ap1);
        arrowStylePath.lineTo(ap0);
        arrowStylePath.lineTo(ap2);
        arrowStylePath.lineTo(ap0);
        arrowStylePath.lineTo(ap1);
    }
    else if (arrowStyle == Closed)
    {
        arrowStylePath = QPainterPath();
        arrowStylePath.moveTo(ap1);
        arrowStylePath.lineTo(ap0);
        arrowStylePath.lineTo(ap2);
        arrowStylePath.lineTo(ap1);
    }
    else if (arrowStyle == Dot)
    {
        arrowStylePath = QPainterPath();
        arrowStylePath.addEllipse(ap0, arrowStyleLength, arrowStyleLength);
    }
    else if (arrowStyle == Box)
    {
        arrowStylePath = QPainterPath();
        double side = EmbLine(ap1, ap2).length();
        EmbRect ar0(0, 0, side, side);
        ar0.moveCenter(ap0);
        arrowStylePath.addRect(ar0);
    }
    else if (arrowStyle == Tick)
    {
    }

    lineStylePath = QPainterPath();
    lineStylePath.moveTo(ap0);
    lineStylePath.lineTo(lp0);
}

void DimLeaderObject::paint(QPainter* painter, const QStyleOptionGraphicsItem* option, QWidget* /*widget*/)
{
    QGraphicsScene* objScene = scene();
    if (!objScene) return;

    QPen paintPen = pen();
    painter->setPen(paintPen);
    updateRubber(painter);
    if (option->state & QStyle::State_Selected)  { paintPen.setStyle(Qt::DashLine); }
    if (objScene->property(ENABLE_LWT).toBool()) { paintPen = lineWeightPen(); }
    painter->setPen(paintPen);

    painter->drawPath(lineStylePath);
    painter->drawPath(arrowStylePath);

    if (filled)
        painter->fillPath(arrowStylePath, objectColor());
}

void DimLeaderObject::updateRubber(QPainter* painter)
{
    int rubberMode = objectRubberMode();
    if (rubberMode == OBJ_RUBBER_DIMLEADER_LINE) {
        EmbVector sceneStartPoint = objectRubberPoint("DIMLEADER_LINE_START");
        EmbVector sceneQSnapPoint = objectRubberPoint("DIMLEADER_LINE_END");

        setObjectEndPoint1(sceneStartPoint);
        setObjectEndPoint2(sceneQSnapPoint);
    }
    else if (rubberMode == OBJ_RUBBER_GRIP) {
        if (painter) {
            EmbVector gripPoint = objectRubberPoint("GRIP_POINT");
            if (gripPoint == objectEndPoint1())
                painter->drawLine(line().p2(), mapFromScene(objectRubberPoint(std::string())));
            else if (gripPoint == objectEndPoint2())
                painter->drawLine(line().p1(), mapFromScene(objectRubberPoint(std::string())));
            else if (gripPoint == objectMidPoint())
                painter->drawLine(line().translated(mapFromScene(objectRubberPoint(std::string()))-mapFromScene(gripPoint)));
        }
    }
}

void DimLeaderObject::vulcanize()
{
    debug_message("DimLeaderObject vulcanize()");
    updateRubber();

    setObjectRubberMode(OBJ_RUBBER_OFF);
}

// Returns the closest snap point to the mouse point
EmbVector DimLeaderObject::mouseSnapPoint(const EmbVector& mousePoint)
{
    EmbVector endPoint1 = objectEndPoint1();
    EmbVector endPoint2 = objectEndPoint2();
    EmbVector midPoint  = objectMidPoint();

    double end1Dist = EmbLine(mousePoint, endPoint1).length();
    double end2Dist = EmbLine(mousePoint, endPoint2).length();
    double midDist  = EmbLine(mousePoint, midPoint).length();

    double minDist = std::min(end1Dist, end2Dist);

    if (curved)
        minDist = std::min(minDist, midDist);

    if     (minDist == end1Dist) return endPoint1;
    else if (minDist == end2Dist) return endPoint2;
    else if (minDist == midDist)  return midPoint;

    return scenePos();
}

std::vector<EmbVector> DimLeaderObject_allGripPoints()
{
    std::vector<EmbVector> gripPoints;
    gripPoints << objectEndPoint1() << objectEndPoint2();
    if (curved) {
        gripPoints << objectMidPoint();
    }
    return gripPoints;
}

void DimLeaderObject::gripEdit(const EmbVector& before, const EmbVector& after)
{
    if (before == objectEndPoint1()) {
        setObjectEndPoint1(after);
    }
    else if (before == objectEndPoint2()) {
        setObjectEndPoint2(after);
    }
    else if (before == objectMidPoint()) {
        EmbVector delta = embVector_subtract(after, before);
        moveBy(delta);
    }
}
#endif
