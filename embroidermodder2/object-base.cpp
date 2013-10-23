#include "object-base.h"

#include <QDebug>
#include <QGraphicsScene>
#include <QMessageBox>
#include <QDateTime>
#include <QPainter>

BaseObject::BaseObject(QGraphicsItem* parent) : QGraphicsPathItem(parent)
{
    qDebug("BaseObject Constructor()");

    objPen.setCapStyle(Qt::RoundCap);
    objPen.setJoinStyle(Qt::RoundJoin);
    lwtPen.setCapStyle(Qt::RoundCap);
    lwtPen.setJoinStyle(Qt::RoundJoin);

    objID = QDateTime::currentMSecsSinceEpoch();
}

BaseObject::~BaseObject()
{
    qDebug("BaseObject Destructor()");
}

void BaseObject::setObjectColor(const QColor& color)
{
    objPen.setColor(color);
    lwtPen.setColor(color);
}

void BaseObject::setObjectColorRGB(QRgb rgb)
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
            qDebug("Lineweight cannot be negative! Inverting sign.");
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

/* kate: bom off; indent-mode cstyle; indent-width 4; replace-trailing-space-save on; */
