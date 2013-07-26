#ifndef OBJECT_TEXTSINGLE_H
#define OBJECT_TEXTSINGLE_H

#include "object-base.h"

class TextSingleObject : public BaseObject
{
public:
    TextSingleObject(const QString& str, qreal x, qreal y, QRgb rgb, QGraphicsItem* parent = 0);
    TextSingleObject(TextSingleObject* obj, QGraphicsItem* parent = 0);
    ~TextSingleObject();

    enum { Type = OBJ_TYPE_TEXTSINGLE };
    virtual int type() const { return Type; }

    QString objectText()          const { return objText; }
    QString objectTextFont()      const { return objTextFont; }
    qreal   objectTextSize()      const { return objTextSize; }
    bool    objectTextBold()      const { return objTextBold; }
    bool    objectTextItalic()    const { return objTextItalic; }
    bool    objectTextUnderline() const { return objTextUnderline; }
    bool    objectTextStrikeOut() const { return objTextStrikeOut; }
    bool    objectTextOverline()  const { return objTextOverline; }
    QPointF objectPos()           const { return scenePos(); }
    qreal   objectX()             const { return scenePos().x(); }
    qreal   objectY()             const { return scenePos().y(); }

    void setObjectText(const QString& str);
    void setObjectTextFont(const QString& font);
    void setObjectTextSize(qreal size);
    void setObjectTextStyle(bool bold, bool italic, bool under, bool strike, bool over);
    void setObjectTextBold(bool val);
    void setObjectTextItalic(bool val);
    void setObjectTextUnderline(bool val);
    void setObjectTextStrikeOut(bool val);
    void setObjectTextOverline(bool val);
    void setObjectPos(const QPointF& point) { setPos(point.x(), point.y()); }
    void setObjectPos(qreal x, qreal y) { setPos(x, y); }
    void setObjectX(qreal x) { setObjectPos(x, objectY()); }
    void setObjectY(qreal y) { setObjectPos(objectX(), y); }

    QPointF mouseSnapPoint(const QPointF& mousePoint);
    QList<QPointF> allGripPoints();
    void updateRubber(QPainter* painter = 0);
    virtual void vulcanize();
protected:
    void paint(QPainter*, const QStyleOptionGraphicsItem*, QWidget*);
private:
    void init(const QString& str, qreal x, qreal y, QRgb rgb, Qt::PenStyle lineType);

    QString objText;
    QString objTextFont;
    qreal   objTextSize;
    bool    objTextBold;
    bool    objTextItalic;
    bool    objTextUnderline;
    bool    objTextStrikeOut;
    bool    objTextOverline;
};

#endif

/* kate: bom off; indent-mode cstyle; indent-width 4; replace-trailing-space-save on; */
