#ifndef OBJECT_ARC_H
#define OBJECT_ARC_H

#include "object-base.h"

class ArcObject : public BaseObject
{
public:
    ArcObject(qreal startX, qreal startY, qreal midX, qreal midY, qreal endX, qreal endY, QRgb rgb, QGraphicsItem* parent = 0);
    ArcObject(ArcObject* obj, QGraphicsItem* parent = 0);
    ~ArcObject();

    enum { Type = OBJ_TYPE_ARC };
    virtual int type() const { return Type; }

    QPointF objectCenter()        const { return scenePos(); }
    qreal   objectCenterX()       const { return scenePos().x(); }
    qreal   objectCenterY()       const { return scenePos().y(); }
    qreal   objectRadius()        const { return rect().width()/2.0*scale(); }
    qreal   objectStartAngle()    const;
    qreal   objectEndAngle()      const;
    QPointF objectStartPoint()    const;
    qreal   objectStartX()        const;
    qreal   objectStartY()        const;
    QPointF objectMidPoint()      const;
    qreal   objectMidX()          const;
    qreal   objectMidY()          const;
    QPointF objectEndPoint()      const;
    qreal   objectEndX()          const;
    qreal   objectEndY()          const;
    qreal   objectArea()          const;
    qreal   objectArcLength()     const;
    qreal   objectChord()         const;
    qreal   objectIncludedAngle() const;
    bool    objectClockwise()     const;

    void setObjectCenter(const QPointF& point);
    void setObjectCenter(qreal pointX, qreal pointY);
    void setObjectCenterX(qreal pointX);
    void setObjectCenterY(qreal pointY);
    void setObjectRadius(qreal radius);
    void setObjectStartAngle(qreal angle);
    void setObjectEndAngle(qreal angle);
    void setObjectStartPoint(const QPointF& point);
    void setObjectStartPoint(qreal pointX, qreal pointY);
    void setObjectMidPoint(const QPointF& point);
    void setObjectMidPoint(qreal pointX, qreal pointY);
    void setObjectEndPoint(const QPointF& point);
    void setObjectEndPoint(qreal pointX, qreal pointY);

    QPointF mouseSnapPoint(const QPointF& mousePoint);
    QList<QPointF> allGripPoints();
    void updateRubber(QPainter* painter = 0);
    virtual void vulcanize();
protected:
    void paint(QPainter*, const QStyleOptionGraphicsItem*, QWidget*);
private:
    void init(qreal startX, qreal startY, qreal midX, qreal midY, qreal endX, qreal endY, QRgb rgb, Qt::PenStyle lineType);
    void updatePath();

    void calculateArcData(qreal startX, qreal startY, qreal midX, qreal midY, qreal endX, qreal endY);
    void updateArcRect(qreal radius);

    QPointF arcStartPoint;
    QPointF arcMidPoint;
    QPointF arcEndPoint;
};

#endif

/* kate: bom off; indent-mode cstyle; indent-width 4; replace-trailing-space-save on; */
