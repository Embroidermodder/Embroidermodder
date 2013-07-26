#include "object-base.h"

#include <QDebug>
#include <QGraphicsScene>
#include <QMessageBox>
#include <QDateTime>

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
    return scene()->property(SCENE_QSNAP_POINT).toPointF();
}

QString BaseObject::objectRubberText(const QString& key) const
{
    if(objRubberTexts.contains(key))
        return objRubberTexts.value(key);
    return QString();
}

/* kate: bom off; indent-mode cstyle; indent-width 4; replace-trailing-space-save on; */
