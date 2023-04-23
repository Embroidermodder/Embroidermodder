#ifndef OBJECT_ELLIPSE_H
#define OBJECT_ELLIPSE_H

#include "object-base.h"

class EllipseObject : public BaseObject
{
public:
    EllipseObject(EmbReal centerX, EmbReal centerY, EmbReal width, EmbReal height, QRgb rgb, QGraphicsItem* parent = 0);
    EllipseObject(EllipseObject* obj, QGraphicsItem* parent = 0);
    ~EllipseObject();

    enum { Type = OBJ_TYPE_ELLIPSE };
    virtual int type() const { return Type; }

    QPainterPath objectSavePath() const;

    QPointF objectCenter()        const { return scenePos(); }
    EmbReal   objectCenterX()       const { return scenePos().x(); }
    EmbReal   objectCenterY()       const { return scenePos().y(); }
    EmbReal   objectRadiusMajor()   const { return qMax(rect().width(), rect().height())/2.0*scale(); }
    EmbReal   objectRadiusMinor()   const { return qMin(rect().width(), rect().height())/2.0*scale(); }
    EmbReal   objectDiameterMajor() const { return qMax(rect().width(), rect().height())*scale(); }
    EmbReal   objectDiameterMinor() const { return qMin(rect().width(), rect().height())*scale(); }
    EmbReal   objectWidth()         const { return rect().width()*scale(); }
    EmbReal   objectHeight()        const { return rect().height()*scale(); }
    QPointF objectQuadrant0()     const;
    QPointF objectQuadrant90()    const;
    QPointF objectQuadrant180()   const;
    QPointF objectQuadrant270()   const;

    void setObjectSize(EmbReal width, EmbReal height);
    void setObjectCenter(const QPointF& center);
    void setObjectCenter(EmbReal centerX, EmbReal centerY);
    void setObjectCenterX(EmbReal centerX);
    void setObjectCenterY(EmbReal centerY);
    void setObjectRadiusMajor(EmbReal radius);
    void setObjectRadiusMinor(EmbReal radius);
    void setObjectDiameterMajor(EmbReal diameter);
    void setObjectDiameterMinor(EmbReal diameter);

    void updateRubber(QPainter* painter = 0);
    virtual void vulcanize();
    virtual QPointF mouseSnapPoint(const QPointF& mousePoint);
    virtual QList<QPointF> allGripPoints();
    virtual void gripEdit(const QPointF& before, const QPointF& after);
protected:
    void paint(QPainter*, const QStyleOptionGraphicsItem*, QWidget*);
private:
    void init(EmbReal centerX, EmbReal centerY, EmbReal width, EmbReal height, QRgb rgb, Qt::PenStyle lineType);
    void updatePath();
};

#endif

/* kate: bom off; indent-mode cstyle; indent-width 4; replace-trailing-space-save on; */
