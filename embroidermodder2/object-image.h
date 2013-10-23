#ifndef OBJECT_IMAGE_H
#define OBJECT_IMAGE_H

#include "object-base.h"

class ImageObject : public BaseObject
{
public:
    ImageObject(qreal x, qreal y, qreal w, qreal h, QRgb rgb, QGraphicsItem* parent = 0);
    ImageObject(ImageObject* obj, QGraphicsItem* parent = 0);
    ~ImageObject();

    enum { Type = OBJ_TYPE_IMAGE };
    virtual int type() const { return Type; }

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
