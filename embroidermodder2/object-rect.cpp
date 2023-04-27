/**
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
RectObject::RectObject(EmbReal x, EmbReal y, EmbReal w, EmbReal h, QRgb rgb, QGraphicsItem* parent) : BaseObject(parent)
{
    qDebug("RectObject Constructor()");
    init(x, y, w, h, rgb, Qt::SolidLine); //TODO: getCurrentLineType
}

/**
 * \brief .
 */
RectObject::RectObject(RectObject* obj, QGraphicsItem* parent) : BaseObject(parent)
{
    qDebug("RectObject Constructor()");
    if (obj) {
        QPointF ptl = obj->objectTopLeft();
        init(ptl.x(), ptl.y(), obj->objectWidth(), obj->objectHeight(), obj->objectColorRGB(), Qt::SolidLine); //TODO: getCurrentLineType
        setRotation(obj->rotation());
    }
}

/**
 * \brief RectObject destructor.
 */
RectObject::~RectObject()
{
    qDebug("RectObject Destructor()");
}

/**
 * \brief .
 */
void RectObject::init(EmbReal x, EmbReal y, EmbReal w, EmbReal h, QRgb rgb, Qt::PenStyle lineType)
{
    setData(OBJ_TYPE, OBJ_TYPE_RECTANGLE);
    setData(OBJ_NAME, "Rectangle");

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
void RectObject::setObjectRect(EmbReal x, EmbReal y, EmbReal w, EmbReal h)
{
    setPos(x, y);
    setRect(0, 0, w, h);
    updatePath();
}

/**
 * \brief .
 * \return The top left corner location as a QPointF.
 */
QPointF
RectObject::objectTopLeft() const
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
QPointF RectObject::objectTopRight() const
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
QPointF RectObject::objectBottomLeft() const
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
QPointF RectObject::objectBottomRight() const
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
void RectObject::updatePath()
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
void RectObject::paint(QPainter* painter, const QStyleOptionGraphicsItem* option, QWidget* /*widget*/)
{
    QGraphicsScene* objScene = scene();
    if (!objScene) return;

    QPen paintPen = pen();
    painter->setPen(paintPen);
    updateRubber(painter);
    if (option->state & QStyle::State_Selected)  { paintPen.setStyle(Qt::DashLine); }
    if (objScene->property("ENABLE_LWT").toBool()) { paintPen = lineWeightPen(); }
    painter->setPen(paintPen);

    painter->drawRect(rect());
}

/**
 * \brief .
 */
void RectObject::updateRubber(QPainter* painter)
{
    int rubberMode = objectRubberMode();
    if (rubberMode == OBJ_RUBBER_RECTANGLE)
    {
        QPointF sceneStartPoint = objectRubberPoint("RECTANGLE_START");
        QPointF sceneEndPoint = objectRubberPoint("RECTANGLE_END");
        EmbReal x = sceneStartPoint.x();
        EmbReal y = sceneStartPoint.y();
        EmbReal w = sceneEndPoint.x() - sceneStartPoint.x();
        EmbReal h = sceneEndPoint.y() - sceneStartPoint.y();
        setObjectRect(x,y,w,h);
        updatePath();
    }
    else if (rubberMode == OBJ_RUBBER_GRIP)
    {
        if (painter)
        {
            //TODO: Make this work with rotation & scaling
            /*
            QPointF gripPoint = objectRubberPoint("GRIP_POINT");
            QPointF after = objectRubberPoint(QString());
            QPointF delta = after-gripPoint;
            if     (gripPoint == objectTopLeft())     { painter->drawPolygon(mapFromScene(QRectF(after.x(), after.y(), objectWidth()-delta.x(), objectHeight()-delta.y()))); }
            else if (gripPoint == objectTopRight())    { painter->drawPolygon(mapFromScene(QRectF(objectTopLeft().x(), objectTopLeft().y()+delta.y(), objectWidth()+delta.x(), objectHeight()-delta.y()))); }
            else if (gripPoint == objectBottomLeft())  { painter->drawPolygon(mapFromScene(QRectF(objectTopLeft().x()+delta.x(), objectTopLeft().y(), objectWidth()-delta.x(), objectHeight()+delta.y()))); }
            else if (gripPoint == objectBottomRight()) { painter->drawPolygon(mapFromScene(QRectF(objectTopLeft().x(), objectTopLeft().y(), objectWidth()+delta.x(), objectHeight()+delta.y()))); }

            QLineF rubLine(mapFromScene(gripPoint), mapFromScene(objectRubberPoint(QString())));
            drawRubberLine(rubLine, painter, "VIEW_COLOR_CROSSHAIR");
            */

            QPointF gripPoint = objectRubberPoint("GRIP_POINT");
            QPointF after = objectRubberPoint(QString());
            QPointF delta = after-gripPoint;

            QLineF rubLine(mapFromScene(gripPoint), mapFromScene(objectRubberPoint(QString())));
            drawRubberLine(rubLine, painter, "VIEW_COLOR_CROSSHAIR");
        }
    }
}

/**
 * \brief .
 */
void RectObject::vulcanize()
{
    qDebug("RectObject vulcanize()");
    updateRubber();

    setObjectRubberMode(OBJ_RUBBER_OFF);
}

/**
 * \brief .
 * \return The closest snap point to the mouse point.
 *
 */
QPointF RectObject::mouseSnapPoint(const QPointF& mousePoint)
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
    else if (minDist == ptrDist) return ptr;
    else if (minDist == pblDist) return pbl;
    else if (minDist == pbrDist) return pbr;

    return scenePos();
}

/**
 * \brief .
 * \return A list of all grip points for the object.
 * \todo make return value a std::vector<std::string>
 */
QList<QPointF> RectObject::allGripPoints()
{
    QList<QPointF> gripPoints;
    gripPoints << objectTopLeft() << objectTopRight() << objectBottomLeft() << objectBottomRight();
    return gripPoints;
}

/**
 * \brief .
 */
void RectObject::gripEdit(const QPointF& before, const QPointF& after)
{
    QPointF delta = after-before;
    if     (before == objectTopLeft())     { setObjectRect(after.x(), after.y(), objectWidth()-delta.x(), objectHeight()-delta.y()); }
    else if (before == objectTopRight())    { setObjectRect(objectTopLeft().x(), objectTopLeft().y()+delta.y(), objectWidth()+delta.x(), objectHeight()-delta.y()); }
    else if (before == objectBottomLeft())  { setObjectRect(objectTopLeft().x()+delta.x(), objectTopLeft().y(), objectWidth()-delta.x(), objectHeight()+delta.y()); }
    else if (before == objectBottomRight()) { setObjectRect(objectTopLeft().x(), objectTopLeft().y(), objectWidth()+delta.x(), objectHeight()+delta.y()); }
}

/**
 * \brief .
 */
QPainterPath RectObject::objectSavePath() const
{
    QPainterPath path;
    QRectF r = rect();
    path.moveTo(r.bottomLeft());
    path.lineTo(r.bottomRight());
    path.lineTo(r.topRight());
    path.lineTo(r.topLeft());
    path.lineTo(r.bottomLeft());

    EmbReal s = scale();
    QTransform trans;
    trans.rotate(rotation());
    trans.scale(s,s);
    return trans.map(path);
}
