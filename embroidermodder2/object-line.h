#ifndef OBJECT_LINE_H
#define OBJECT_LINE_H

#include "object-base.h"

class LineObject : public BaseObject
{
public:
    LineObject(EmbReal x1, EmbReal y1, EmbReal x2, EmbReal y2, QRgb rgb, QGraphicsItem* parent = 0);
    LineObject(LineObject* obj, QGraphicsItem* parent = 0);
    ~LineObject();

    enum { Type = OBJ_TYPE_LINE };
    virtual int type() const { return Type; }

    QPainterPath objectSavePath() const;

    QPointF objectEndPoint1() const { return scenePos(); }
    QPointF objectEndPoint2() const;
    QPointF objectMidPoint()  const;
    EmbReal   objectX1()        const { return objectEndPoint1().x(); }
    EmbReal   objectY1()        const { return objectEndPoint1().y(); }
    EmbReal   objectX2()        const { return objectEndPoint2().x(); }
    EmbReal   objectY2()        const { return objectEndPoint2().y(); }
    EmbReal   objectDeltaX()    const { return (objectX2() - objectX1()); }
    EmbReal   objectDeltaY()    const { return (objectY2() - objectY1()); }
    EmbReal   objectAngle()     const;
    EmbReal   objectLength()    const { return line().length()*scale(); }

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
};

#endif

/* kate: bom off; indent-mode cstyle; indent-width 4; replace-trailing-space-save on; */
