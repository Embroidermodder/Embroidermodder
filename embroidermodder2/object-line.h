#ifndef OBJECT_LINE_H
#define OBJECT_LINE_H

#include "object-base.h"

class LineObject : public BaseObject
{
public:
    LineObject(qreal x1, qreal y1, qreal x2, qreal y2, QRgb rgb, QGraphicsItem* parent = 0);
    LineObject(LineObject* obj, QGraphicsItem* parent = 0);
    ~LineObject();

    enum { Type = OBJ_TYPE_LINE };
    virtual int type() const { return Type; }

    QPointF objectEndPoint1() const { return scenePos(); }
    QPointF objectEndPoint2() const;
    QPointF objectMidPoint()  const;
    qreal   objectX1()        const { return objectEndPoint1().x(); }
    qreal   objectY1()        const { return objectEndPoint1().y(); }
    qreal   objectX2()        const { return objectEndPoint2().x(); }
    qreal   objectY2()        const { return objectEndPoint2().y(); }
    qreal   objectDeltaX()    const { return (objectX2() - objectX1()); }
    qreal   objectDeltaY()    const { return (objectY2() - objectY1()); }
    qreal   objectAngle()     const;
    qreal   objectLength()    const { return line().length()*scale(); }

    void setObjectEndPoint1(const QPointF& endPt1);
    void setObjectEndPoint1(qreal x1, qreal y1);
    void setObjectEndPoint2(const QPointF& endPt2);
    void setObjectEndPoint2(qreal x2, qreal y2);
    void setObjectX1(qreal x) { setObjectEndPoint1(x, objectY1()); }
    void setObjectY1(qreal y) { setObjectEndPoint1(objectX1(), y); }
    void setObjectX2(qreal x) { setObjectEndPoint2(x, objectY2()); }
    void setObjectY2(qreal y) { setObjectEndPoint2(objectX2(), y); }

    QPointF mouseSnapPoint(const QPointF& mousePoint);
    QList<QPointF> allGripPoints();
    void updateRubber(QPainter* painter = 0);
    virtual void vulcanize();
protected:
    void paint(QPainter*, const QStyleOptionGraphicsItem*, QWidget*);
private:
    void init(qreal x1, qreal y1, qreal x2, qreal y2, QRgb rgb, Qt::PenStyle lineType);
};

#endif

/* kate: bom off; indent-mode cstyle; indent-width 4; replace-trailing-space-save on; */
