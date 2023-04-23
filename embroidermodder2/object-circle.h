#ifndef OBJECT_CIRCLE_H
#define OBJECT_CIRCLE_H

#include "object-base.h"

class CircleObject : public BaseObject
{
public:
    CircleObject(EmbReal centerX, EmbReal centerY, EmbReal radius, QRgb rgb, QGraphicsItem* parent = 0);
    CircleObject(CircleObject* obj, QGraphicsItem* parent = 0);
    ~CircleObject();

    enum { Type = OBJ_TYPE_CIRCLE };
    virtual int type() const { return Type; }

    QPainterPath objectSavePath() const;

    QPointF objectCenter() const { return scenePos(); }
    EmbReal objectCenterX() const { return scenePos().x(); }
    EmbReal objectCenterY() const { return scenePos().y(); }
    EmbReal objectRadius() const { return rect().width()/2.0*scale(); }
    EmbReal objectDiameter() const { return rect().width()*scale(); }
    EmbReal objectArea() const { return emb_constant_pi*objectRadius()*objectRadius(); }
    EmbReal objectCircumference() const { return emb_constant_pi*objectDiameter(); }
    QPointF objectQuadrant0() const { return objectCenter() + QPointF(objectRadius(), 0); }
    QPointF objectQuadrant90() const { return objectCenter() + QPointF(0,-objectRadius()); }
    QPointF objectQuadrant180() const { return objectCenter() + QPointF(-objectRadius(),0); }
    QPointF objectQuadrant270() const { return objectCenter() + QPointF(0, objectRadius()); }

    void setObjectCenter(const QPointF& center);
    void setObjectCenter(EmbReal centerX, EmbReal centerY);
    void setObjectCenterX(EmbReal centerX);
    void setObjectCenterY(EmbReal centerY);
    void setObjectRadius(EmbReal radius);
    void setObjectDiameter(EmbReal diameter);
    void setObjectArea(EmbReal area);
    void setObjectCircumference(EmbReal circumference);

    void updateRubber(QPainter* painter = 0);
    virtual void vulcanize();
    virtual QPointF mouseSnapPoint(const QPointF& mousePoint);
    virtual QList<QPointF> allGripPoints();
    virtual void gripEdit(const QPointF& before, const QPointF& after);
protected:
    void paint(QPainter*, const QStyleOptionGraphicsItem*, QWidget*);
private:
    void init(EmbReal centerX, EmbReal centerY, EmbReal radius, QRgb rgb, Qt::PenStyle lineType);
    void updatePath();
};

#endif
