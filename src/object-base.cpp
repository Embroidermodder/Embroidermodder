/*
 *  Embroidermodder 2.
 *
 *  ------------------------------------------------------------
 *
 *  Copyright 2013-2022 The Embroidermodder Team
 *  Embroidermodder 2 is Open Source Software.
 *  See LICENSE for licensing terms.
 *
 *  ------------------------------------------------------------
 *
 *  Use Python's PEP7 style guide.
 *      https://peps.python.org/pep-0007/
 */

#include "embroidermodder.h"

BaseObject::BaseObject(QGraphicsItem* parent) : QGraphicsPathItem(parent)
{
    debug_message("BaseObject Constructor()");

    objPen.setCapStyle(Qt::RoundCap);
    objPen.setJoinStyle(Qt::RoundJoin);
    lwtPen.setCapStyle(Qt::RoundCap);
    lwtPen.setJoinStyle(Qt::RoundJoin);

    objID = QDateTime::currentMSecsSinceEpoch();
}

BaseObject::~BaseObject()
{
    debug_message("BaseObject Destructor()");
}

void BaseObject::setObjectColor(const QColor& color)
{
    objPen.setColor(color);
    lwtPen.setColor(color);
}

void BaseObject::setObjectColorRGB(unsigned int rgb)
{
    objPen.setColor(QColor(rgb));
    lwtPen.setColor(QColor(rgb));
}

void BaseObject::setObjectLineType(Qt::PenStyle lineType)
{
    objPen.setStyle(lineType);
    lwtPen.setStyle(lineType);
}

void BaseObject::setObjectLineWeight(qreal lineWeight)
{
    objPen.setWidthF(0); //NOTE: The objPen will always be cosmetic

    if(lineWeight < 0)
    {
        if(lineWeight == OBJ_LWT_BYLAYER)
        {
            lwtPen.setWidthF(0.35); //TODO: getLayerLineWeight
        }
        else if(lineWeight == OBJ_LWT_BYBLOCK)
        {
            lwtPen.setWidthF(0.35); //TODO: getBlockLineWeight
        }
        else
        {
            QMessageBox::warning(0, QObject::tr("Error - Negative Lineweight"),
                                    QObject::tr("Lineweight: %1")
                                    .arg(QString().setNum(lineWeight)));
            debug_message("Lineweight cannot be negative! Inverting sign.");
            lwtPen.setWidthF(-lineWeight);
        }
    }
    else
    {
        lwtPen.setWidthF(lineWeight);
    }
}

QPointF BaseObject::objectRubberPoint(const QString& key) const
{
    if(objRubberPoints.contains(key))
        return objRubberPoints.value(key);

    QGraphicsScene* gscene = scene();
    if(gscene)
        return scene()->property(SCENE_QSNAP_POINT).toPointF();
    return QPointF();
}

QString BaseObject::objectRubberText(const QString& key) const
{
    if(objRubberTexts.contains(key))
        return objRubberTexts.value(key);
    return QString();
}

QRectF BaseObject::boundingRect() const
{
    //If gripped, force this object to be drawn even if it is offscreen
    if(objectRubberMode() == OBJ_RUBBER_GRIP)
        return scene()->sceneRect();
    return path().boundingRect();
}

void BaseObject::drawRubberLine(const QLineF& rubLine, QPainter* painter, const char* colorFromScene)
{
    if(painter)
    {
        QGraphicsScene* objScene = scene();
        if(!objScene) return;
        QPen colorPen = objPen;
        colorPen.setColor(QColor(objScene->property(colorFromScene).toUInt()));
        painter->setPen(colorPen);
        painter->drawLine(rubLine);
        painter->setPen(objPen);
    }
}

void BaseObject::realRender(QPainter* painter, const QPainterPath& renderPath)
{
    QColor color1 = objectColor();       //lighter color
    QColor color2  = color1.darker(150); //darker color

    //If we have a dark color, lighten it
    int darkness = color1.lightness();
    int threshold = 32; //TODO: This number may need adjusted or maybe just add it to settings.
    if(darkness < threshold)
    {
        color2 = color1;
        if(!darkness) { color1 = QColor(threshold, threshold, threshold); } //lighter() does not affect pure black
        else          { color1 = color2.lighter(100 + threshold); }
    }

    int count = renderPath.elementCount();
    for(int i = 0; i < count-1; ++i)
    {
        QPainterPath::Element elem = renderPath.elementAt(i);
        QPainterPath::Element next = renderPath.elementAt(i+1);

        if(next.isMoveTo()) continue;

        QPainterPath elemPath;
        elemPath.moveTo(elem.x, elem.y);
        elemPath.lineTo(next.x, next.y);

        QPen renderPen(QColor(0,0,0,0));
        renderPen.setWidthF(0);
        painter->setPen(renderPen);
        QPainterPathStroker stroker;
        stroker.setWidth(0.35);
        stroker.setCapStyle(Qt::RoundCap);
        stroker.setJoinStyle(Qt::RoundJoin);
        QPainterPath realPath = stroker.createStroke(elemPath);
        painter->drawPath(realPath);

        QLinearGradient grad(elemPath.pointAtPercent(0.5), elemPath.pointAtPercent(0.0));
        grad.setColorAt(0, color1);
        grad.setColorAt(1, color2);
        grad.setSpread(QGradient::ReflectSpread);

        painter->fillPath(realPath, QBrush(grad));
    }
}
