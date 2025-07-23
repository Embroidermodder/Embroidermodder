#include "object-line.h"
#include "object-data.h"

#include <QPainter>
#include <QStyleOption>
#include <QGraphicsScene>

LineObject::LineObject(qreal x1, qreal y1, qreal x2, qreal y2, QRgb rgb, QGraphicsItem* parent) : BaseObject(parent)
{
    qDebug("LineObject Constructor()");
    init(x1, y1, x2, y2, rgb, Qt::SolidLine); //TODO: getCurrentLineType
}

LineObject::LineObject(LineObject* obj, QGraphicsItem* parent) : BaseObject(parent)
{
    qDebug("LineObject Constructor()");
    if(obj)
    {
        init(obj->objectX1(), obj->objectY1(), obj->objectX2(), obj->objectY2(), obj->objectColorRGB(), Qt::SolidLine); //TODO: getCurrentLineType
    }
}

LineObject::~LineObject()
{
    qDebug("LineObject Destructor()");
}

void LineObject::init(qreal x1, qreal y1, qreal x2, qreal y2, QRgb rgb, Qt::PenStyle lineType)
{
    setData(OBJ_TYPE, type());
    setData(OBJ_NAME, OBJ_NAME_LINE);

    //WARNING: DO NOT enable QGraphicsItem::ItemIsMovable. If it is enabled,
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

void LineObject::setObjectEndPoint1(const QPointF& endPt1)
{
    setObjectEndPoint1(endPt1.x(), endPt1.y());
}

void LineObject::setObjectEndPoint1(qreal x1, qreal y1)
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

void LineObject::setObjectEndPoint2(const QPointF& endPt2)
{
    setObjectEndPoint2(endPt2.x(), endPt2.y());
}

void LineObject::setObjectEndPoint2(qreal x2, qreal y2)
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

QPointF LineObject::objectEndPoint2() const
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

QPointF LineObject::objectMidPoint() const
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

qreal LineObject::objectAngle() const
{
    qreal angle = line().angle() - rotation();
    while(angle >= 360.0) { angle -= 360.0; }
    while(angle < 0.0)    { angle += 360.0; }
    return angle;
}

void LineObject::paint(QPainter* painter, const QStyleOptionGraphicsItem* option, QWidget* /*widget*/)
{
    QGraphicsScene* objScene = scene();
    if(!objScene) return;

    QPen paintPen = pen();
    painter->setPen(paintPen);
    updateRubber(painter);
    if(option->state & QStyle::State_Selected)  { paintPen.setStyle(Qt::DashLine); }
    if(objScene->property(ENABLE_LWT).toBool()) { paintPen = lineWeightPen(); }
    painter->setPen(paintPen);

    if(objectRubberMode() != OBJ_RUBBER_LINE) painter->drawLine(line());

    if(objScene->property(ENABLE_LWT).toBool() && objScene->property(ENABLE_REAL).toBool()) { realRender(painter, path()); }
}

void LineObject::updateRubber(QPainter* painter)
{
    int rubberMode = objectRubberMode();
    if(rubberMode == OBJ_RUBBER_LINE)
    {
        QPointF sceneStartPoint = objectRubberPoint("LINE_START");
        QPointF sceneQSnapPoint = objectRubberPoint("LINE_END");

        setObjectEndPoint1(sceneStartPoint);
        setObjectEndPoint2(sceneQSnapPoint);

        drawRubberLine(line(), painter, VIEW_COLOR_CROSSHAIR);
    }
    else if(rubberMode == OBJ_RUBBER_GRIP)
    {
        if(painter)
        {
            QPointF gripPoint = objectRubberPoint("GRIP_POINT");
            if     (gripPoint == objectEndPoint1()) painter->drawLine(line().p2(), mapFromScene(objectRubberPoint(QString())));
            else if(gripPoint == objectEndPoint2()) painter->drawLine(line().p1(), mapFromScene(objectRubberPoint(QString())));
            else if(gripPoint == objectMidPoint())  painter->drawLine(line().translated(mapFromScene(objectRubberPoint(QString()))-mapFromScene(gripPoint)));

            QLineF rubLine(mapFromScene(gripPoint), mapFromScene(objectRubberPoint(QString())));
            drawRubberLine(rubLine, painter, VIEW_COLOR_CROSSHAIR);
        }
    }
}

void LineObject::vulcanize()
{
    qDebug("LineObject vulcanize()");
    updateRubber();

    setObjectRubberMode(OBJ_RUBBER_OFF);
}

// Returns the closest snap point to the mouse point
QPointF LineObject::mouseSnapPoint(const QPointF& mousePoint)
{
    QPointF endPoint1 = objectEndPoint1();
    QPointF endPoint2 = objectEndPoint2();
    QPointF midPoint  = objectMidPoint();

    qreal end1Dist = QLineF(mousePoint, endPoint1).length();
    qreal end2Dist = QLineF(mousePoint, endPoint2).length();
    qreal midDist  = QLineF(mousePoint, midPoint).length();

    qreal minDist = qMin(qMin(end1Dist, end2Dist), midDist);

    if     (minDist == end1Dist) return endPoint1;
    else if(minDist == end2Dist) return endPoint2;
    else if(minDist == midDist)  return midPoint;

    return scenePos();
}

QList<QPointF> LineObject::allGripPoints()
{
    QList<QPointF> gripPoints;
    gripPoints << objectEndPoint1() << objectEndPoint2() << objectMidPoint();
    return gripPoints;
}

void LineObject::gripEdit(const QPointF& before, const QPointF& after)
{
    if     (before == objectEndPoint1()) { setObjectEndPoint1(after); }
    else if(before == objectEndPoint2()) { setObjectEndPoint2(after); }
    else if(before == objectMidPoint())  { QPointF delta = after-before; moveBy(delta.x(), delta.y()); }
}

QPainterPath LineObject::objectSavePath() const
{
    QPainterPath path;
    path.lineTo(objectDeltaX(), objectDeltaY());
    return path;
}

/* kate: bom off; indent-mode cstyle; indent-width 4; replace-trailing-space-save on; */
