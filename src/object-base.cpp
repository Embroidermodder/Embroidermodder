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

#if 0
Base_BaseObject(QGraphicsItem* parent)
{
    debug_message("BaseObject Constructor()");

    objPen.setCapStyle(RoundCap);
    objPen.setJoinStyle(RoundJoin);
    lwtPen.setCapStyle(RoundCap);
    lwtPen.setJoinStyle(RoundJoin);

    objID = QDateTime::currentMSecsSinceEpoch();
}

Base_~BaseObject()
{
    debug_message("BaseObject Destructor()");
}

void Base_setObjectColor(const QColor& color)
{
    objPen.setColor(color);
    lwtPen.setColor(color);
}

void Base_setObjectColorRGB(unsigned int rgb)
{
    objPen.setColor(QColor(rgb));
    lwtPen.setColor(QColor(rgb));
}

void Base_setObjectLineType(PenStyle lineType)
{
    objPen.setStyle(lineType);
    lwtPen.setStyle(lineType);
}

void Base_setObjectLineWeight(double lineWeight)
{
    objPen.setWidthF(0); //NOTE: The objPen will always be cosmetic

    if (lineWeight < 0)
    {
        if (lineWeight == OBJ_LWT_BYLAYER)
        {
            lwtPen.setWidthF(0.35); //TODO: getLayerLineWeight
        }
        else if (lineWeight == OBJ_LWT_BYBLOCK)
        {
            lwtPen.setWidthF(0.35); //TODO: getBlockLineWeight
        }
        else
        {
            QMessageBox::warning(0, translate("Error - Negative Lineweight"),
                                    translate("Lineweight: %1")
                                    .arg(std::string().setNum(lineWeight)));
            debug_message("Lineweight cannot be negative! Inverting sign.");
            lwtPen.setWidthF(-lineWeight);
        }
    }
    else
    {
        lwtPen.setWidthF(lineWeight);
    }
}

EmbVector Base_objectRubberPoint(const std::string& key) const
{
    if (objRubberPoints.contains(key))
        return objRubberPoints.value(key);

    QGraphicsScene* gscene = scene();
    if (gscene)
        return scene()->property("SCENE_QSNAP_POINT").toPointF();
    return EmbVector();
}

std::string Base_objectRubberText(const std::string& key) const
{
    if (objRubberTexts.contains(key))
        return objRubberTexts.value(key);
    return std::string();
}

EmbRect Base_boundingRect() const
{
    //If gripped, force this object to be drawn even if it is offscreen
    if (objectRubberMode() == OBJ_RUBBER_GRIP)
        return scene()->sceneRect();
    return path().boundingRect();
}

void Base_drawRubberLine(const EmbLine& rubLine, QPainter* painter, const char* colorFromScene)
{
    if (painter) {
        QGraphicsScene* objScene = scene();
        if (!objScene) return;
        QPen colorPen = objPen;
        colorPen.setColor(QColor(objScene->property(colorFromScene).toUInt()));
        painter->setPen(colorPen);
        painter->drawLine(rubLine);
        painter->setPen(objPen);
    }
}

void Base_realRender(QPainter* painter, const QPainterPath& renderPath)
{
    QColor color1 = objectColor();       //lighter color
    QColor color2  = color1.darker(150); //darker color

    //If we have a dark color, lighten it
    int darkness = color1.lightness();
    int threshold = 32; //TODO: This number may need adjusted or maybe just add it to settings.
    if (darkness < threshold)
    {
        color2 = color1;
        if (!darkness) { color1 = QColor(threshold, threshold, threshold); } //lighter() does not affect pure black
        else          { color1 = color2.lighter(100 + threshold); }
    }

    int count = renderPath.elementCount();
    for(int i = 0; i < count-1; ++i)
    {
        QPainterPath::Element elem = renderPath.elementAt(i);
        QPainterPath::Element next = renderPath.elementAt(i+1);

        if (next.isMoveTo()) continue;

        QPainterPath elemPath;
        elemPath.moveTo(elem.x, elem.y);
        elemPath.lineTo(next.x, next.y);

        QPen renderPen(QColor(0,0,0,0));
        renderPen.setWidthF(0);
        painter->setPen(renderPen);
        QPainterPathStroker stroker;
        stroker.setWidth(0.35);
        stroker.setCapStyle(RoundCap);
        stroker.setJoinStyle(RoundJoin);
        QPainterPath realPath = stroker.createStroke(elemPath);
        painter->drawPath(realPath);

        QLinearGradient grad(elemPath.pointAtPercent(0.5), elemPath.pointAtPercent(0.0));
        grad.setColorAt(0, color1);
        grad.setColorAt(1, color2);
        grad.setSpread(QGradient::ReflectSpread);

        painter->fillPath(realPath, QBrush(grad));
    }
}
#endif
