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

/**
 * \file object-rect.cpp
 */

#include "embroidermodder.h"

/**
 * \brief .
 */
ImageObject::ImageObject(EmbReal x, EmbReal y, EmbReal w, EmbReal h, QRgb rgb, QGraphicsItem* parent) : BaseObject(parent)
{
    qDebug("ImageObject Constructor()");
    init(x, y, w, h, rgb, Qt::SolidLine); //TODO: getCurrentLineType
}

/**
 * \brief .
 */
ImageObject::ImageObject(ImageObject* obj, QGraphicsItem* parent) : BaseObject(parent)
{
    qDebug("ImageObject Constructor()");
    if(obj)
    {
        QPointF ptl = obj->objectTopLeft();
        init(ptl.x(), ptl.y(), obj->objectWidth(), obj->objectHeight(), obj->objectColorRGB(), Qt::SolidLine); //TODO: getCurrentLineType
        setRotation(obj->rotation());
    }
}

/**
 * \brief .
 */
ImageObject::~ImageObject()
{
    qDebug("ImageObject Destructor()");
}

/**
 * \brief .
 */
void ImageObject::init(EmbReal x, EmbReal y, EmbReal w, EmbReal h, QRgb rgb, Qt::PenStyle lineType)
{
    setData(OBJ_TYPE, OBJ_TYPE_IMAGE);
    setData(OBJ_NAME, "Image");

    //WARNING: DO NOT enable QGraphicsItem::ItemIsMovable. If it is enabled,
    //WARNING: and the item is double clicked, the scene will erratically move the item while zooming.
    //WARNING: All movement has to be handled explicitly by us, not by the scene.
    setFlag(QGraphicsItem::ItemIsSelectable, true);

    setObjectRect(x, y, w, h);
    setObjectColor(rgb);
    setObjectLineType(lineType);
    setObjectLineWeight(0.35); //TODO: pass in proper lineweight
    setPen(objectPen());
}

/**
 * \brief .
 */
void
ImageObject::setObjectRect(EmbReal x, EmbReal y, EmbReal w, EmbReal h)
{
    setPos(x, y);
    setRect(0, 0, w, h);
    updatePath();
}

/**
 * \brief .
 */
QPointF
ImageObject::objectTopLeft() const
{
    EmbReal rot = radians(rotation());
    EmbReal cosRot = qCos(rot);
    EmbReal sinRot = qSin(rot);

    QPointF tl = rect().topLeft();
    EmbReal ptlX = tl.x()*scale();
    EmbReal ptlY = tl.y()*scale();
    EmbReal ptlXrot = ptlX*cosRot - ptlY*sinRot;
    EmbReal ptlYrot = ptlX*sinRot + ptlY*cosRot;

    return (scenePos() + QPointF(ptlXrot, ptlYrot));
}

/**
 * \brief .
 */
QPointF
ImageObject::objectTopRight() const
{
    EmbReal rot = radians(rotation());
    EmbReal cosRot = qCos(rot);
    EmbReal sinRot = qSin(rot);

    QPointF tr = rect().topRight();
    EmbReal ptrX = tr.x()*scale();
    EmbReal ptrY = tr.y()*scale();
    EmbReal ptrXrot = ptrX*cosRot - ptrY*sinRot;
    EmbReal ptrYrot = ptrX*sinRot + ptrY*cosRot;

    return (scenePos() + QPointF(ptrXrot, ptrYrot));
}

/**
 * \brief .
 */
QPointF
ImageObject::objectBottomLeft() const
{
    EmbReal rot = radians(rotation());
    EmbReal cosRot = qCos(rot);
    EmbReal sinRot = qSin(rot);

    QPointF bl = rect().bottomLeft();
    EmbReal pblX = bl.x()*scale();
    EmbReal pblY = bl.y()*scale();
    EmbReal pblXrot = pblX*cosRot - pblY*sinRot;
    EmbReal pblYrot = pblX*sinRot + pblY*cosRot;

    return (scenePos() + QPointF(pblXrot, pblYrot));
}

/**
 * \brief .
 */
QPointF
ImageObject::objectBottomRight() const
{
    EmbReal rot = radians(rotation());
    EmbReal cosRot = qCos(rot);
    EmbReal sinRot = qSin(rot);

    QPointF br = rect().bottomRight();
    EmbReal pbrX = br.x()*scale();
    EmbReal pbrY = br.y()*scale();
    EmbReal pbrXrot = pbrX*cosRot - pbrY*sinRot;
    EmbReal pbrYrot = pbrX*sinRot + pbrY*cosRot;

    return (scenePos() + QPointF(pbrXrot, pbrYrot));
}

/**
 * \brief .
 */
void
ImageObject::updatePath()
{
    QPainterPath path;
    QRectF r = rect();
    path.moveTo(r.bottomLeft());
    path.lineTo(r.bottomRight());
    path.lineTo(r.topRight());
    path.lineTo(r.topLeft());
    path.lineTo(r.bottomLeft());
    //NOTE: Reverse the path so that the inside area isn't considered part of the rectangle
    path.lineTo(r.topLeft());
    path.lineTo(r.topRight());
    path.lineTo(r.bottomRight());
    path.moveTo(r.bottomLeft());
    setObjectPath(path);
}

/**
 * \brief .
 */
void
ImageObject::paint(QPainter* painter, const QStyleOptionGraphicsItem* option, QWidget* /*widget*/)
{
    QGraphicsScene* objScene = scene();
    if(!objScene) return;

    QPen paintPen = pen();
    painter->setPen(paintPen);
    updateRubber(painter);
    if(option->state & QStyle::State_Selected)  { paintPen.setStyle(Qt::DashLine); }
    if(objScene->property("ENABLE_LWT").toBool()) { paintPen = lineWeightPen(); }
    painter->setPen(paintPen);

    painter->drawRect(rect());
}

/**
 * \brief .
 */
void
ImageObject::updateRubber(QPainter* painter)
{
    int rubberMode = objRubberMode;
    if(rubberMode == OBJ_RUBBER_IMAGE)
    {
        QPointF sceneStartPoint = objectRubberPoint("IMAGE_START");
        QPointF sceneEndPoint = objectRubberPoint("IMAGE_END");
        EmbReal x = sceneStartPoint.x();
        EmbReal y = sceneStartPoint.y();
        EmbReal w = sceneEndPoint.x() - sceneStartPoint.x();
        EmbReal h = sceneEndPoint.y() - sceneStartPoint.y();
        setObjectRect(x,y,w,h);
        updatePath();
    }
    else if(rubberMode == OBJ_RUBBER_GRIP)
    {
        //TODO: updateRubber() gripping for ImageObject
    }
}

/**
 * \brief .
 */
void
ImageObject::vulcanize()
{
    qDebug("ImageObject vulcanize()");
    updateRubber();

    objRubberMode = OBJ_RUBBER_OFF;
}

// Returns the closest snap point to the mouse point
/**
 * \brief .
 */
QPointF
ImageObject::mouseSnapPoint(const QPointF& mousePoint)
{
    QPointF ptl = objectTopLeft();     //Top Left Corner QSnap
    QPointF ptr = objectTopRight();    //Top Right Corner QSnap
    QPointF pbl = objectBottomLeft();  //Bottom Left Corner QSnap
    QPointF pbr = objectBottomRight(); //Bottom Right Corner QSnap

    EmbReal ptlDist = QLineF(mousePoint, ptl).length();
    EmbReal ptrDist = QLineF(mousePoint, ptr).length();
    EmbReal pblDist = QLineF(mousePoint, pbl).length();
    EmbReal pbrDist = QLineF(mousePoint, pbr).length();

    EmbReal minDist = qMin(qMin(ptlDist, ptrDist), qMin(pblDist, pbrDist));

    if     (minDist == ptlDist) return ptl;
    else if(minDist == ptrDist) return ptr;
    else if(minDist == pblDist) return pbl;
    else if(minDist == pbrDist) return pbr;

    return scenePos();
}

/**
 * \brief .
 */
QList<QPointF>
ImageObject::allGripPoints()
{
    QList<QPointF> gripPoints;
    gripPoints << objectTopLeft() << objectTopRight() << objectBottomLeft() << objectBottomRight();
    return gripPoints;
}

/**
 * \brief .
 */
void
ImageObject::gripEdit(const QPointF& before, const QPointF& after)
{
    //TODO: gripEdit() for ImageObject
}
