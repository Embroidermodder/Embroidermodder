#ifndef OBJECT_ARC_H
#define OBJECT_ARC_H

#include "object-base.h"

class ArcObject : public BaseObject
{
public:
    ArcObject(EmbReal startX, EmbReal startY, EmbReal midX, EmbReal midY, EmbReal endX, EmbReal endY, QRgb rgb, QGraphicsItem* parent = 0);
    ArcObject(ArcObject* obj, QGraphicsItem* parent = 0);
    ~ArcObject();

    enum { Type = OBJ_TYPE_ARC };
    virtual int type() const { return Type; }

    QPointF objectCenter()        const { return scenePos(); }
    EmbReal   objectCenterX()       const { return scenePos().x(); }
    EmbReal   objectCenterY()       const { return scenePos().y(); }
    EmbReal   objectRadius()        const { return rect().width()/2.0*scale(); }
    EmbReal   objectStartAngle()    const;
    EmbReal   objectEndAngle()      const;
    QPointF objectStartPoint()    const;
    EmbReal   objectStartX()        const;
    EmbReal   objectStartY()        const;
    QPointF objectMidPoint()      const;
    EmbReal   objectMidX()          const;
    EmbReal   objectMidY()          const;
    QPointF objectEndPoint()      const;
    EmbReal   objectEndX()          const;
    EmbReal   objectEndY()          const;
    EmbReal   objectArea()          const;
    EmbReal   objectArcLength()     const;
    EmbReal   objectChord()         const;
    EmbReal   objectIncludedAngle() const;
    bool    objectClockwise()     const;

    void setObjectCenter(const QPointF& point);
    void setObjectCenter(EmbReal pointX, EmbReal pointY);
    void setObjectCenterX(EmbReal pointX);
    void setObjectCenterY(EmbReal pointY);
    void setObjectRadius(EmbReal radius);
    void setObjectStartAngle(EmbReal angle);
    void setObjectEndAngle(EmbReal angle);
    void setObjectStartPoint(const QPointF& point);
    void setObjectStartPoint(EmbReal pointX, EmbReal pointY);
    void setObjectMidPoint(const QPointF& point);
    void setObjectMidPoint(EmbReal pointX, EmbReal pointY);
    void setObjectEndPoint(const QPointF& point);
    void setObjectEndPoint(EmbReal pointX, EmbReal pointY);

    void updateRubber(QPainter* painter = 0);
    virtual void vulcanize();
    virtual QPointF mouseSnapPoint(const QPointF& mousePoint);
    virtual QList<QPointF> allGripPoints();
    virtual void gripEdit(const QPointF& before, const QPointF& after);
protected:
    void paint(QPainter*, const QStyleOptionGraphicsItem*, QWidget*);
private:
    void init(EmbReal startX, EmbReal startY, EmbReal midX, EmbReal midY, EmbReal endX, EmbReal endY, QRgb rgb, Qt::PenStyle lineType);
    void updatePath();

    void calculateArcData(EmbReal startX, EmbReal startY, EmbReal midX, EmbReal midY, EmbReal endX, EmbReal endY);
    void updateArcRect(EmbReal radius);

    QPointF arcStartPoint;
    QPointF arcMidPoint;
    QPointF arcEndPoint;
};

#endif

/* kate: bom off; indent-mode cstyle; indent-width 4; replace-trailing-space-save on; */
