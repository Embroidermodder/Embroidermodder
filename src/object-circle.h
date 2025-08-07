#ifndef OBJECT_CIRCLE_H
#define OBJECT_CIRCLE_H

#include "object-base.h"

class CircleObject : public BaseObject
{
public:
    CircleObject(qreal centerX, qreal centerY, qreal radius, QRgb rgb, QGraphicsItem* parent = 0);
    CircleObject(CircleObject* obj, QGraphicsItem* parent = 0);
    ~CircleObject();

    enum { Type = OBJ_TYPE_CIRCLE };
    virtual int type() const { return Type; }

    QPainterPath objectSavePath() const;

    QPointF objectCenter()        const { return scenePos(); }
    qreal   objectCenterX()       const { return scenePos().x(); }
    qreal   objectCenterY()       const { return scenePos().y(); }
    qreal   objectRadius()        const { return rect().width()/2.0*scale(); }
    qreal   objectDiameter()      const { return rect().width()*scale(); }
    qreal   objectArea()          const { return pi()*objectRadius()*objectRadius(); }
    qreal   objectCircumference() const { return pi()*objectDiameter(); }
    QPointF objectQuadrant0()     const { return objectCenter() + QPointF(objectRadius(), 0); }
    QPointF objectQuadrant90()    const { return objectCenter() + QPointF(0,-objectRadius()); }
    QPointF objectQuadrant180()   const { return objectCenter() + QPointF(-objectRadius(),0); }
    QPointF objectQuadrant270()   const { return objectCenter() + QPointF(0, objectRadius()); }

    void setObjectCenter(const QPointF& center);
    void setObjectCenter(qreal centerX, qreal centerY);
    void setObjectCenterX(qreal centerX);
    void setObjectCenterY(qreal centerY);
    void setObjectRadius(qreal radius);
    void setObjectDiameter(qreal diameter);
    void setObjectArea(qreal area);
    void setObjectCircumference(qreal circumference);

    void updateRubber(QPainter* painter = 0);
    virtual void vulcanize();
    virtual QPointF mouseSnapPoint(const QPointF& mousePoint);
    virtual QList<QPointF> allGripPoints();
    virtual void gripEdit(const QPointF& before, const QPointF& after);
protected:
    void paint(QPainter*, const QStyleOptionGraphicsItem*, QWidget*);
private:
    void init(qreal centerX, qreal centerY, qreal radius, QRgb rgb, Qt::PenStyle lineType);
    void updatePath();
};

#endif
