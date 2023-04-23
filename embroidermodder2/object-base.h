#ifndef OBJECT_BASE_H
#define OBJECT_BASE_H

#include "embroidermodder.h"

#include <QHash>
#include <QPen>
#include <QGraphicsPathItem>

class BaseObject : public QGraphicsPathItem
{
public:
    BaseObject(QGraphicsItem* parent = 0);
    virtual ~BaseObject();

    enum { Type = OBJ_TYPE_BASE };
    virtual int type() const { return Type; }

    QPen objPen;
    QPen lwtPen;
    QLineF objLine;
    int objRubberMode;
    QHash<QString, QPointF> objRubberPoints;
    QHash<QString, QString> objRubberTexts;
    qint64 objID;

    qint64       objectID() const { return objID; }
    QPen         objectPen() const { return objPen; }
    QColor       objectColor() const { return objPen.color(); }
    QRgb         objectColorRGB() const { return objPen.color().rgb(); }
    Qt::PenStyle objectLineType() const { return objPen.style(); }
    EmbReal        objectLineWeight() const { return lwtPen.widthF(); }
    QPainterPath objectPath() const { return path(); }
    int          objectRubberMode() const { return objRubberMode; }
    QPointF      objectRubberPoint(const QString& key) const;
    QString      objectRubberText(const QString& key) const;

    QRectF rect() const { return path().boundingRect(); }
    void setRect(const QRectF& r) { QPainterPath p; p.addRect(r); setPath(p); }
    void setRect(EmbReal x, EmbReal y, EmbReal w, EmbReal h) { QPainterPath p; p.addRect(x,y,w,h); setPath(p); }
    QLineF line() const { return objLine; }
    void setLine(const QLineF& li) { QPainterPath p; p.moveTo(li.p1()); p.lineTo(li.p2()); setPath(p); objLine = li; }
    void setLine(EmbReal x1, EmbReal y1, EmbReal x2, EmbReal y2) { QPainterPath p; p.moveTo(x1,y1); p.lineTo(x2,y2); setPath(p); objLine.setLine(x1,y1,x2,y2); }

    void setObjectColor(const QColor& color);
    void setObjectColorRGB(QRgb rgb);
    void setObjectLineType(Qt::PenStyle lineType);
    void setObjectLineWeight(EmbReal lineWeight);
    void setObjectPath(const QPainterPath& p) { setPath(p); }
    void setObjectRubberMode(int mode) { objRubberMode = mode; }
    void setObjectRubberPoint(const QString& key, const QPointF& point) { objRubberPoints.insert(key, point); }
    void setObjectRubberText(const QString& key, const QString& txt) { objRubberTexts.insert(key, txt); }

    virtual QRectF boundingRect() const;
    virtual QPainterPath shape() const { return path(); }

    void drawRubberLine(const QLineF& rubLine, QPainter* painter = 0, const char* colorFromScene = 0);

    virtual void vulcanize() = 0;
    virtual QPointF mouseSnapPoint(const QPointF& mousePoint) = 0;
    virtual QList<QPointF> allGripPoints() = 0;
    virtual void gripEdit(const QPointF& before, const QPointF& after) = 0;
protected:
    QPen lineWeightPen() const { return lwtPen; }
    void realRender(QPainter* painter, const QPainterPath& renderPath);
};

#endif
