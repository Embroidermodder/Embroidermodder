#ifndef OBJECT_DIMLEADER_H
#define OBJECT_DIMLEADER_H

#include "object-base.h"

class DimLeaderObject : public BaseObject
{
public:
    DimLeaderObject(EmbReal x1, EmbReal y1, EmbReal x2, EmbReal y2, QRgb rgb, QGraphicsItem* parent = 0);
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
    EmbReal   objectX1()        const { return objectEndPoint1().x(); }
    EmbReal   objectY1()        const { return objectEndPoint1().y(); }
    EmbReal   objectX2()        const { return objectEndPoint2().x(); }
    EmbReal   objectY2()        const { return objectEndPoint2().y(); }
    EmbReal   objectDeltaX()    const { return (objectX2() - objectX1()); }
    EmbReal   objectDeltaY()    const { return (objectY2() - objectY1()); }
    EmbReal   objectAngle()     const;
    EmbReal   objectLength()    const { return line().length(); }

    void setObjectEndPoint1(const QPointF& endPt1);
    void setObjectEndPoint1(EmbReal x1, EmbReal y1);
    void setObjectEndPoint2(const QPointF& endPt2);
    void setObjectEndPoint2(EmbReal x2, EmbReal y2);
    void setObjectX1(EmbReal x) { setObjectEndPoint1(x, objectY1()); }
    void setObjectY1(EmbReal y) { setObjectEndPoint1(objectX1(), y); }
    void setObjectX2(EmbReal x) { setObjectEndPoint2(x, objectY2()); }
    void setObjectY2(EmbReal y) { setObjectEndPoint2(objectX2(), y); }

    void updateRubber(QPainter* painter = 0);
    virtual void vulcanize();
    virtual QPointF mouseSnapPoint(const QPointF& mousePoint);
    virtual QList<QPointF> allGripPoints();
    virtual void gripEdit(const QPointF& before, const QPointF& after);
protected:
    void paint(QPainter*, const QStyleOptionGraphicsItem*, QWidget*);
private:
    void init(EmbReal x1, EmbReal y1, EmbReal x2, EmbReal y2, QRgb rgb, Qt::PenStyle lineType);

    bool curved;
    bool filled;
    void updateLeader();
    QPainterPath lineStylePath;
    QPainterPath arrowStylePath;
    EmbReal arrowStyleAngle;
    EmbReal arrowStyleLength;
    EmbReal lineStyleAngle;
    EmbReal lineStyleLength;
};

#endif

/* kate: bom off; indent-mode cstyle; indent-width 4; replace-trailing-space-save on; */
