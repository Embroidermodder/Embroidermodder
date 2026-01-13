#ifndef OBJECT_RECT_H
#define OBJECT_RECT_H

#include "object-base.h"

class RectObject : public BaseObject
{
public:
    RectObject(qreal x, qreal y, qreal w, qreal h, QRgb rgb, QGraphicsItem* parent = 0);
    RectObject(RectObject* obj, QGraphicsItem* parent = 0);
    ~RectObject();

    enum { Type = OBJ_TYPE_RECTANGLE };
    virtual int type() const { return Type; }

    QPainterPath objectSavePath() const;

    QPointF objectPos() const { return scenePos(); }

    QPointF objectTopLeft()     const;
    QPointF objectTopRight()    const;
    QPointF objectBottomLeft()  const;
    QPointF objectBottomRight() const;
    qreal   objectWidth()       const { return rect().width()*scale(); }
    qreal   objectHeight()      const { return rect().height()*scale(); }
    qreal   objectArea()        const { return qAbs(objectWidth()*objectHeight()); }

    void setObjectRect(qreal x, qreal y, qreal w, qreal h);

    void updateRubber(QPainter* painter = 0);
    virtual void vulcanize();
    virtual QPointF mouseSnapPoint(const QPointF& mousePoint);
    virtual QList<QPointF> allGripPoints();
    virtual void gripEdit(const QPointF& before, const QPointF& after);
protected:
    void paint(QPainter*, const QStyleOptionGraphicsItem*, QWidget*);
private:
    void init(qreal x, qreal y, qreal w, qreal h, QRgb rgb, Qt::PenStyle lineType);
    void updatePath();
};

#endif

/* kate: bom off; indent-mode cstyle; indent-width 4; replace-trailing-space-save on; */
