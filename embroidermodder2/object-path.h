#ifndef OBJECT_PATH_H
#define OBJECT_PATH_H

#include "object-base.h"

class PathObject : public BaseObject
{
public:
    PathObject(qreal x, qreal y, const QPainterPath p, QRgb rgb, QGraphicsItem* parent = 0);
    PathObject(PathObject* obj, QGraphicsItem* parent = 0);
    ~PathObject();

    enum { Type = OBJ_TYPE_PATH };
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
    void init(qreal x, qreal y, const QPainterPath p, QRgb rgb, Qt::PenStyle lineType);
};

#endif

/* kate: bom off; indent-mode cstyle; indent-width 4; replace-trailing-space-save on; */
