#ifndef OBJECT_POLYLINE_H
#define OBJECT_POLYLINE_H

#include "object-base.h"

class PolylineObject : public BaseObject
{
public:
    PolylineObject(qreal x, qreal y, const QPainterPath& p, QRgb rgb, QGraphicsItem* parent = 0);
    PolylineObject(PolylineObject* obj, QGraphicsItem* parent = 0);
    ~PolylineObject();

    enum { Type = OBJ_TYPE_POLYLINE };
    virtual int type() const { return Type; }

    QPointF objectPos() const { return scenePos(); }
    qreal   objectX()   const { return scenePos().x(); }
    qreal   objectY()   const { return scenePos().y(); }

    void setObjectPos(const QPointF& point) { setPos(point.x(), point.y()); }
    void setObjectPos(qreal x, qreal y) { setPos(x, y); }
    void setObjectX(qreal x) { setObjectPos(x, objectY()); }
    void setObjectY(qreal y) { setObjectPos(objectX(), y); }

    QPointF mouseSnapPoint(const QPointF& mousePoint);
    QList<QPointF> allGripPoints();
    void updateRubber(QPainter* painter = 0);
    virtual void vulcanize();
protected:
    void paint(QPainter*, const QStyleOptionGraphicsItem*, QWidget*);
private:
    void init(qreal x, qreal y, const QPainterPath& p, QRgb rgb, Qt::PenStyle lineType);
    void updatePath(const QPainterPath& p);
    QPainterPath normalPath;
};

#endif

/* kate: bom off; indent-mode cstyle; indent-width 4; replace-trailing-space-save on; */
