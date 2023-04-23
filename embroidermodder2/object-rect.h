#ifndef OBJECT_RECT_H
#define OBJECT_RECT_H

#include "object-base.h"

class RectObject : public BaseObject
{
public:
    RectObject(EmbReal x, EmbReal y, EmbReal w, EmbReal h, QRgb rgb, QGraphicsItem* parent = 0);
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
    EmbReal   objectWidth()       const { return rect().width()*scale(); }
    EmbReal   objectHeight()      const { return rect().height()*scale(); }
    EmbReal   objectArea()        const { return qAbs(objectWidth()*objectHeight()); }

    void setObjectRect(EmbReal x, EmbReal y, EmbReal w, EmbReal h);

    void updateRubber(QPainter* painter = 0);
    virtual void vulcanize();
    virtual QPointF mouseSnapPoint(const QPointF& mousePoint);
    virtual QList<QPointF> allGripPoints();
    virtual void gripEdit(const QPointF& before, const QPointF& after);
protected:
    void paint(QPainter*, const QStyleOptionGraphicsItem*, QWidget*);
private:
    void init(EmbReal x, EmbReal y, EmbReal w, EmbReal h, QRgb rgb, Qt::PenStyle lineType);
    void updatePath();
};

#endif
