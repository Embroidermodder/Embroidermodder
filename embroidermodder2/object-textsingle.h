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

    QPainterPath objectSavePath() const { return objTextPath; }

    QString objectText()           const { return objText; }
    QString objectTextFont()       const { return objTextFont; }
    QString objectTextJustify()    const { return objTextJustify; }
    qreal   objectTextSize()       const { return objTextSize; }
    bool    objectTextBold()       const { return objTextBold; }
    bool    objectTextItalic()     const { return objTextItalic; }
    bool    objectTextUnderline()  const { return objTextUnderline; }
    bool    objectTextStrikeOut()  const { return objTextStrikeOut; }
    bool    objectTextOverline()   const { return objTextOverline; }
    bool    objectTextBackward()   const { return objTextBackward; }
    bool    objectTextUpsideDown() const { return objTextUpsideDown; }
    QPointF objectPos()            const { return scenePos(); }
    qreal   objectX()              const { return scenePos().x(); }
    qreal   objectY()              const { return scenePos().y(); }

    QStringList objectTextJustifyList() const;

    void setObjectText(const QString& str);
    void setObjectTextFont(const QString& font);
    void setObjectTextJustify(const QString& justify);
    void setObjectTextSize(qreal size);
    void setObjectTextStyle(bool bold, bool italic, bool under, bool strike, bool over);
    void setObjectTextBold(bool val);
    void setObjectTextItalic(bool val);
    void setObjectTextUnderline(bool val);
    void setObjectTextStrikeOut(bool val);
    void setObjectTextOverline(bool val);
    void setObjectTextBackward(bool val);
    void setObjectTextUpsideDown(bool val);
    void setObjectPos(const QPointF& point) { setPos(point.x(), point.y()); }
    void setObjectPos(qreal x, qreal y) { setPos(x, y); }
    void setObjectX(qreal x) { setObjectPos(x, objectY()); }
    void setObjectY(qreal y) { setObjectPos(objectX(), y); }

    void updateRubber(QPainter* painter = 0);
    virtual void vulcanize();
    virtual QPointF mouseSnapPoint(const QPointF& mousePoint);
    virtual QList<QPointF> allGripPoints();
    virtual void gripEdit(const QPointF& before, const QPointF& after);
protected:
    void paint(QPainter*, const QStyleOptionGraphicsItem*, QWidget*);
private:
    void init(const QString& str, qreal x, qreal y, QRgb rgb, Qt::PenStyle lineType);

    QString objText;
    QString objTextFont;
    QString objTextJustify;
    qreal   objTextSize;
    bool    objTextBold;
    bool    objTextItalic;
    bool    objTextUnderline;
    bool    objTextStrikeOut;
    bool    objTextOverline;
    bool    objTextBackward;
    bool    objTextUpsideDown;
    QPainterPath objTextPath;
};

#endif

/* kate: bom off; indent-mode cstyle; indent-width 4; replace-trailing-space-save on; */
