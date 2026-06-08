#ifndef OBJECT_DIMLEADER_H
#define OBJECT_DIMLEADER_H

#include "object-base.h"

class DimLeaderObject : public BaseObject
{
public:
    DimLeaderObject(qreal x1, qreal y1, qreal x2, qreal y2, QRgb rgb, QGraphicsItem* parent = 0);
    DimLeaderObject(DimLeaderObject* obj, QGraphicsItem* parent = 0);
    ~DimLeaderObject();

    enum ArrowStyle
    {
        NoArrow, //NOTE: Allow this enum to evaluate false
        Open,
        Closed,
        Dot,
        Box,
        Tick
    };

    enum lineStyle
    {
        NoLine, //NOTE: Allow this enum to evaluate false
        Flared,
        Fletching
    };

    enum { Type = OBJ_TYPE_DIMLEADER };
    virtual int type() const { return Type; }

    QPointF objectEndPoint1() const;
    QPointF objectEndPoint2() const;
    QPointF objectMidPoint()  const;
    qreal   objectX1()        const { return objectEndPoint1().x(); }
    qreal   objectY1()        const { return objectEndPoint1().y(); }
    qreal   objectX2()        const { return objectEndPoint2().x(); }
    qreal   objectY2()        const { return objectEndPoint2().y(); }
    qreal   objectDeltaX()    const { return (objectX2() - objectX1()); }
    qreal   objectDeltaY()    const { return (objectY2() - objectY1()); }
    qreal   objectAngle()     const;
    qreal   objectLength()    const { return line().length(); }

    void setObjectEndPoint1(const QPointF& endPt1);
    void setObjectEndPoint1(qreal x1, qreal y1);
    void setObjectEndPoint2(const QPointF& endPt2);
    void setObjectEndPoint2(qreal x2, qreal y2);
    void setObjectX1(qreal x) { setObjectEndPoint1(x, objectY1()); }
    void setObjectY1(qreal y) { setObjectEndPoint1(objectX1(), y); }
    void setObjectX2(qreal x) { setObjectEndPoint2(x, objectY2()); }
    void setObjectY2(qreal y) { setObjectEndPoint2(objectX2(), y); }

    void updateRubber(QPainter* painter = 0);
    virtual void vulcanize();
    virtual QPointF mouseSnapPoint(const QPointF& mousePoint);
    virtual QList<QPointF> allGripPoints();
    virtual void gripEdit(const QPointF& before, const QPointF& after);
protected:
    void paint(QPainter*, const QStyleOptionGraphicsItem*, QWidget*);
private:
    void init(qreal x1, qreal y1, qreal x2, qreal y2, QRgb rgb, Qt::PenStyle lineType);

    bool curved;
    bool filled;
    void updateLeader();
    QPainterPath lineStylePath;
    QPainterPath arrowStylePath;
    qreal arrowStyleAngle;
    qreal arrowStyleLength;
    qreal lineStyleAngle;
    qreal lineStyleLength;
};

#endif

