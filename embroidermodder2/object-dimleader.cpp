#include "object-dimleader.h"
#include "object-data.h"

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
    if(obj)
    {
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
    qreal rot = radians(rotation());
    qreal cosRot = qCos(rot);
    qreal sinRot = qSin(rot);
    qreal x2 = lyne.x2()*scale();
    qreal y2 = lyne.y2()*scale();
    qreal rotEnd2X = x2*cosRot - y2*sinRot;
    qreal rotEnd2Y = x2*sinRot + y2*cosRot;

    return (scenePos() + QPointF(rotEnd2X, rotEnd2Y));
}

QPointF DimLeaderObject::objectMidPoint() const
{
    QLineF lyne = line();
    QPointF mp = lyne.pointAt(0.5);
    qreal rot = radians(rotation());
    qreal cosRot = qCos(rot);
    qreal sinRot = qSin(rot);
    qreal mx = mp.x()*scale();
    qreal my = mp.y()*scale();
    qreal rotMidX = mx*cosRot - my*sinRot;
    qreal rotMidY = mx*sinRot + my*cosRot;

    return (scenePos() + QPointF(rotMidX, rotMidY));
}

qreal DimLeaderObject::objectAngle() const
{
    qreal angle = line().angle() - rotation();
    while(angle >= 360.0) { angle -= 360.0; }
    while(angle < 0.0)    { angle += 360.0; }
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
    lynePerp.intersect(lyne1, &ap1);
    lynePerp.intersect(lyne2, &ap2);

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

    if(arrowStyle == Open)
    {
        arrowStylePath = QPainterPath();
        arrowStylePath.moveTo(ap1);
        arrowStylePath.lineTo(ap0);
        arrowStylePath.lineTo(ap2);
        arrowStylePath.lineTo(ap0);
        arrowStylePath.lineTo(ap1);
    }
    else if(arrowStyle == Closed)
    {
        arrowStylePath = QPainterPath();
        arrowStylePath.moveTo(ap1);
        arrowStylePath.lineTo(ap0);
        arrowStylePath.lineTo(ap2);
        arrowStylePath.lineTo(ap1);
    }
    else if(arrowStyle == Dot)
    {
        arrowStylePath = QPainterPath();
        arrowStylePath.addEllipse(ap0, arrowStyleLength, arrowStyleLength);
    }
    else if(arrowStyle == Box)
    {
        arrowStylePath = QPainterPath();
        qreal side = QLineF(ap1, ap2).length();
        QRectF ar0(0, 0, side, side);
        ar0.moveCenter(ap0);
        arrowStylePath.addRect(ar0);
    }
    else if(arrowStyle == Tick)
    {
    }

    lineStylePath = QPainterPath();
    lineStylePath.moveTo(ap0);
    lineStylePath.lineTo(lp0);
}

void DimLeaderObject::paint(QPainter* painter, const QStyleOptionGraphicsItem* option, QWidget* /*widget*/)
{
    QGraphicsScene* objScene = scene();
    if(!objScene) return;

    QPen paintPen = pen();
    if(option->state & QStyle::State_Selected)  { paintPen.setStyle(Qt::DashLine); }
    if(objScene->property(ENABLE_LWT).toBool()) { paintPen = lineWeightPen(); }
    painter->setPen(paintPen);

    updateRubber(painter);

    painter->drawPath(lineStylePath);
    painter->drawPath(arrowStylePath);

    if(filled)
        painter->fillPath(arrowStylePath, objectColor());
}

void DimLeaderObject::updateRubber(QPainter* painter)
{
    int rubberMode = objectRubberMode();
    if(rubberMode == OBJ_RUBBER_DIMLEADER_LINE)
    {
        QPointF sceneStartPoint = objectRubberPoint("DIMLEADER_LINE_START");
        QPointF sceneQSnapPoint = objectRubberPoint("DIMLEADER_LINE_END");

        setObjectEndPoint1(sceneStartPoint);
        setObjectEndPoint2(sceneQSnapPoint);
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
    QPointF endPoint1 = objectEndPoint1();
    QPointF endPoint2 = objectEndPoint2();
    QPointF midPoint  = objectMidPoint();

    qreal end1Dist = QLineF(mousePoint, endPoint1).length();
    qreal end2Dist = QLineF(mousePoint, endPoint2).length();
    qreal midDist  = QLineF(mousePoint, midPoint).length();

    qreal minDist = qMin(end1Dist, end2Dist);

    if(curved)
        minDist = qMin(minDist, midDist);

    if     (minDist == end1Dist) return endPoint1;
    else if(minDist == end2Dist) return endPoint2;
    else if(minDist == midDist)  return midPoint;

    return scenePos();
}

QList<QPointF> DimLeaderObject::allGripPoints()
{
    QList<QPointF> gripPoints;
    gripPoints << objectEndPoint1() << objectEndPoint2();
    if(curved)
        gripPoints << objectMidPoint();
    return gripPoints;
}

/* kate: bom off; indent-mode cstyle; indent-width 4; replace-trailing-space-save on; */
