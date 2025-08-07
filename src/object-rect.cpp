#include "object-rect.h"
#include "object-data.h"

#include <QPainter>
#include <QStyleOption>
#include <QGraphicsScene>

RectObject::RectObject(qreal x, qreal y, qreal w, qreal h, QRgb rgb, QGraphicsItem* parent) : BaseObject(parent)
{
    qDebug("RectObject Constructor()");
    init(x, y, w, h, rgb, Qt::SolidLine); //TODO: getCurrentLineType
}

RectObject::RectObject(RectObject* obj, QGraphicsItem* parent) : BaseObject(parent)
{
    qDebug("RectObject Constructor()");
    if(obj)
    {
        QPointF ptl = obj->objectTopLeft();
        init(ptl.x(), ptl.y(), obj->objectWidth(), obj->objectHeight(), obj->objectColorRGB(), Qt::SolidLine); //TODO: getCurrentLineType
        setRotation(obj->rotation());
    }
}

RectObject::~RectObject()
{
    qDebug("RectObject Destructor()");
}

void RectObject::init(qreal x, qreal y, qreal w, qreal h, QRgb rgb, Qt::PenStyle lineType)
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
    setPen(objectPen());
}

void RectObject::setObjectRect(qreal x, qreal y, qreal w, qreal h)
{
    setPos(x, y);
    setRect(0, 0, w, h);
    updatePath();
}

QPointF RectObject::objectTopLeft() const
{
    qreal rot = radians(rotation());
    qreal cosRot = qCos(rot);
    qreal sinRot = qSin(rot);

    QPointF tl = rect().topLeft();
    qreal ptlX = tl.x()*scale();
    qreal ptlY = tl.y()*scale();
    qreal ptlXrot = ptlX*cosRot - ptlY*sinRot;
    qreal ptlYrot = ptlX*sinRot + ptlY*cosRot;

    return (scenePos() + QPointF(ptlXrot, ptlYrot));
}

QPointF RectObject::objectTopRight() const
{
    qreal rot = radians(rotation());
    qreal cosRot = qCos(rot);
    qreal sinRot = qSin(rot);

    QPointF tr = rect().topRight();
    qreal ptrX = tr.x()*scale();
    qreal ptrY = tr.y()*scale();
    qreal ptrXrot = ptrX*cosRot - ptrY*sinRot;
    qreal ptrYrot = ptrX*sinRot + ptrY*cosRot;

    return (scenePos() + QPointF(ptrXrot, ptrYrot));
}

QPointF RectObject::objectBottomLeft() const
{
    qreal rot = radians(rotation());
    qreal cosRot = qCos(rot);
    qreal sinRot = qSin(rot);

    QPointF bl = rect().bottomLeft();
    qreal pblX = bl.x()*scale();
    qreal pblY = bl.y()*scale();
    qreal pblXrot = pblX*cosRot - pblY*sinRot;
    qreal pblYrot = pblX*sinRot + pblY*cosRot;

    return (scenePos() + QPointF(pblXrot, pblYrot));
}

QPointF RectObject::objectBottomRight() const
{
    qreal rot = radians(rotation());
    qreal cosRot = qCos(rot);
    qreal sinRot = qSin(rot);

    QPointF br = rect().bottomRight();
    qreal pbrX = br.x()*scale();
    qreal pbrY = br.y()*scale();
    qreal pbrXrot = pbrX*cosRot - pbrY*sinRot;
    qreal pbrYrot = pbrX*sinRot + pbrY*cosRot;

    return (scenePos() + QPointF(pbrXrot, pbrYrot));
}

void RectObject::updatePath()
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

void RectObject::paint(QPainter* painter, const QStyleOptionGraphicsItem* option, QWidget* /*widget*/)
{
    QGraphicsScene* objScene = scene();
    if(!objScene) return;

    QPen paintPen = pen();
    painter->setPen(paintPen);
    updateRubber(painter);
    if(option->state & QStyle::State_Selected)  { paintPen.setStyle(Qt::DashLine); }
    if(objScene->property(ENABLE_LWT).toBool()) { paintPen = lineWeightPen(); }
    painter->setPen(paintPen);

    painter->drawRect(rect());
}

void RectObject::updateRubber(QPainter* painter)
{
    int rubberMode = objectRubberMode();
    if(rubberMode == OBJ_RUBBER_RECTANGLE)
    {
        QPointF sceneStartPoint = objectRubberPoint("RECTANGLE_START");
        QPointF sceneEndPoint = objectRubberPoint("RECTANGLE_END");
        qreal x = sceneStartPoint.x();
        qreal y = sceneStartPoint.y();
        qreal w = sceneEndPoint.x() - sceneStartPoint.x();
        qreal h = sceneEndPoint.y() - sceneStartPoint.y();
        setObjectRect(x,y,w,h);
        updatePath();
    }
    else if(rubberMode == OBJ_RUBBER_GRIP)
    {
        if(painter)
        {
            //TODO: Make this work with rotation & scaling
            /*
            QPointF gripPoint = objectRubberPoint("GRIP_POINT");
            QPointF after = objectRubberPoint(QString());
            QPointF delta = after-gripPoint;
            if     (gripPoint == objectTopLeft())     { painter->drawPolygon(mapFromScene(QRectF(after.x(), after.y(), objectWidth()-delta.x(), objectHeight()-delta.y()))); }
            else if(gripPoint == objectTopRight())    { painter->drawPolygon(mapFromScene(QRectF(objectTopLeft().x(), objectTopLeft().y()+delta.y(), objectWidth()+delta.x(), objectHeight()-delta.y()))); }
            else if(gripPoint == objectBottomLeft())  { painter->drawPolygon(mapFromScene(QRectF(objectTopLeft().x()+delta.x(), objectTopLeft().y(), objectWidth()-delta.x(), objectHeight()+delta.y()))); }
            else if(gripPoint == objectBottomRight()) { painter->drawPolygon(mapFromScene(QRectF(objectTopLeft().x(), objectTopLeft().y(), objectWidth()+delta.x(), objectHeight()+delta.y()))); }

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

void RectObject::vulcanize()
{
    qDebug("RectObject vulcanize()");
    updateRubber();

    setObjectRubberMode(OBJ_RUBBER_OFF);
}

// Returns the closest snap point to the mouse point
QPointF RectObject::mouseSnapPoint(const QPointF& mousePoint)
{
    QPointF ptl = objectTopLeft();     //Top Left Corner QSnap
    QPointF ptr = objectTopRight();    //Top Right Corner QSnap
    QPointF pbl = objectBottomLeft();  //Bottom Left Corner QSnap
    QPointF pbr = objectBottomRight(); //Bottom Right Corner QSnap

    qreal ptlDist = QLineF(mousePoint, ptl).length();
    qreal ptrDist = QLineF(mousePoint, ptr).length();
    qreal pblDist = QLineF(mousePoint, pbl).length();
    qreal pbrDist = QLineF(mousePoint, pbr).length();

    qreal minDist = qMin(qMin(ptlDist, ptrDist), qMin(pblDist, pbrDist));

    if     (minDist == ptlDist) return ptl;
    else if(minDist == ptrDist) return ptr;
    else if(minDist == pblDist) return pbl;
    else if(minDist == pbrDist) return pbr;

    return scenePos();
}

QList<QPointF> RectObject::allGripPoints()
{
    QList<QPointF> gripPoints;
    gripPoints << objectTopLeft() << objectTopRight() << objectBottomLeft() << objectBottomRight();
    return gripPoints;
}

void RectObject::gripEdit(const QPointF& before, const QPointF& after)
{
    QPointF delta = after-before;
    if     (before == objectTopLeft())     { setObjectRect(after.x(), after.y(), objectWidth()-delta.x(), objectHeight()-delta.y()); }
    else if(before == objectTopRight())    { setObjectRect(objectTopLeft().x(), objectTopLeft().y()+delta.y(), objectWidth()+delta.x(), objectHeight()-delta.y()); }
    else if(before == objectBottomLeft())  { setObjectRect(objectTopLeft().x()+delta.x(), objectTopLeft().y(), objectWidth()-delta.x(), objectHeight()+delta.y()); }
    else if(before == objectBottomRight()) { setObjectRect(objectTopLeft().x(), objectTopLeft().y(), objectWidth()+delta.x(), objectHeight()+delta.y()); }
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
