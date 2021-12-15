#include "object-polygon.h"
#include "object-data.h"

#include <QPainter>
#include <QStyleOption>
#include <QGraphicsScene>

PolygonObject::PolygonObject(qreal x, qreal y, const QPainterPath p, QRgb rgb, QGraphicsItem* parent) : BaseObject(parent)
{
    qDebug("PolygonObject Constructor()");
    init(x, y, p, rgb, Qt::SolidLine); //TODO: getCurrentLineType
}

PolygonObject::PolygonObject(PolygonObject* obj, QGraphicsItem* parent) : BaseObject(parent)
{
    qDebug("PolygonObject Constructor()");
    if(obj)
    {
        init(obj->objectX(), obj->objectY(), obj->objectPath(), obj->objectColorRGB(), Qt::SolidLine); //TODO: getCurrentLineType
    }
}

PolygonObject::~PolygonObject()
{
    qDebug("PolygonObject Destructor()");
}

void PolygonObject::init(qreal x, qreal y, const QPainterPath p, QRgb rgb, Qt::PenStyle lineType)
{
    setData(OBJ_TYPE, type());
    setData(OBJ_NAME, OBJ_NAME_POLYGON);

    //WARNING: DO NOT enable QGraphicsItem::ItemIsMovable. If it is enabled,
    //WARNING: and the item is double clicked, the scene will erratically move the item while zooming.
    //WARNING: All movement has to be handled explicitly by us, not by the scene.
    setFlag(QGraphicsItem::ItemIsSelectable, true);

    setObjectPath(p);
    setObjectPos(x,y);
    setObjectColor(rgb);
    setObjectLineType(lineType);
    setObjectLineWeight(0.35); //TODO: pass in proper lineweight
    setPen(objectPen());
}

void PolygonObject::paint(QPainter* painter, const QStyleOptionGraphicsItem* option, QWidget* /*widget*/)
{
    QGraphicsScene* objScene = scene();
    if(!objScene) return;

    QPen paintPen = pen();
    if(option->state & QStyle::State_Selected)  { paintPen.setStyle(Qt::DashLine); }
    if(objScene->property(ENABLE_LWT).toBool()) { paintPen = lineWeightPen(); }
    painter->setPen(paintPen);

    updateRubber(painter);

    painter->drawPath(objectPath());
}

void PolygonObject::updateRubber(QPainter* painter)
{
    //TODO: Polygon Rubber Modes
}

void PolygonObject::vulcanize()
{
    qDebug("PolygonObject vulcanize()");
    updateRubber();

    setObjectRubberMode(OBJ_RUBBER_OFF);
}

// Returns the closest snap point to the mouse point
QPointF PolygonObject::mouseSnapPoint(const QPointF& mousePoint)
{
    return scenePos();
}

QList<QPointF> PolygonObject::allGripPoints()
{
    QList<QPointF> gripPoints;
    gripPoints << scenePos(); //TODO: loop thru all path Elements and return their points
    return gripPoints;
}

/* kate: bom off; indent-mode cstyle; indent-width 4; replace-trailing-space-save on; */
