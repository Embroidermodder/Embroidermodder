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

#include <QPainter>
#include <QStyleOption>
#include <QGraphicsScene>

/**
 * \brief .
 */
EllipseObject::EllipseObject(EmbReal centerX, EmbReal centerY, EmbReal width, EmbReal height, QRgb rgb, QGraphicsItem* parent) : BaseObject(parent)
{
    qDebug("EllipseObject Constructor()");
    init(centerX, centerY, width, height, rgb, Qt::SolidLine); //TODO: getCurrentLineType
}

/**
 * \brief .
 */
EllipseObject::EllipseObject(EllipseObject* obj, QGraphicsItem* parent) : BaseObject(parent)
{
    qDebug("EllipseObject Constructor()");
    if (obj) {
        init(
            obj->objectCenterX(),
            obj->objectCenterY(),
            obj->objectWidth(),
            obj->objectHeight(),
            obj->objectColorRGB(),
            Qt::SolidLine); //TODO: getCurrentLineType
        setRotation(obj->rotation());
    }
}

/**
 * \brief .
 */
EllipseObject::~EllipseObject()
{
    qDebug("EllipseObject Destructor()");
}

/**
 * \brief .
 */
void
EllipseObject::init(EmbReal centerX, EmbReal centerY, EmbReal width, EmbReal height, QRgb rgb, Qt::PenStyle lineType)
{
    setData(OBJ_TYPE, type());
    setData(OBJ_NAME, OBJ_NAME_ELLIPSE);

    //WARNING: DO NOT enable QGraphicsItem::ItemIsMovable. If it is enabled,
    //WARNING: and the item is double clicked, the scene will erratically move the item while zooming.
    //WARNING: All movement has to be handled explicitly by us, not by the scene.
    setFlag(QGraphicsItem::ItemIsSelectable, true);

    setObjectSize(width, height);
    setObjectCenter(centerX, centerY);
    setObjectColor(rgb);
    setObjectLineType(lineType);
    setObjectLineWeight(0.35); //TODO: pass in proper lineweight
    setPen(objectPen());
    updatePath();
}

/**
 * \brief .
 */
void
EllipseObject::setObjectSize(EmbReal width, EmbReal height)
{
    QRectF elRect = rect();
    elRect.setWidth(width);
    elRect.setHeight(height);
    elRect.moveCenter(QPointF(0,0));
    setRect(elRect);
}

/**
 * \brief .
 */
void
EllipseObject::setObjectCenter(const QPointF& center)
{
    setObjectCenter(center.x(), center.y());
}

/**
 * \brief .
 */
void
EllipseObject::setObjectCenter(EmbReal centerX, EmbReal centerY)
{
    setPos(centerX, centerY);
}

/**
 * \brief .
 */
void
EllipseObject::setObjectCenterX(EmbReal centerX)
{
    setX(centerX);
}

/**
 * \brief .
 */
void
EllipseObject::setObjectCenterY(EmbReal centerY)
{
    setY(centerY);
}

/**
 * \brief .
 */
void
EllipseObject::setObjectRadiusMajor(EmbReal radius)
{
    setObjectDiameterMajor(radius*2.0);
}

/**
 * \brief .
 */
void
EllipseObject::setObjectRadiusMinor(EmbReal radius)
{
    setObjectDiameterMinor(radius*2.0);
}

/**
 * \brief .
 */
void
EllipseObject::setObjectDiameterMajor(EmbReal diameter)
{
    QRectF elRect = rect();
    if(elRect.width() > elRect.height())
        elRect.setWidth(diameter);
    else
        elRect.setHeight(diameter);
    elRect.moveCenter(QPointF(0,0));
    setRect(elRect);
}

/**
 * \brief .
 */
void
EllipseObject::setObjectDiameterMinor(EmbReal diameter)
{
    QRectF elRect = rect();
    if(elRect.width() < elRect.height())
        elRect.setWidth(diameter);
    else
        elRect.setHeight(diameter);
    elRect.moveCenter(QPointF(0,0));
    setRect(elRect);
}

/**
 * \brief .
 */
QPointF
EllipseObject::objectQuadrant0() const
{
    EmbReal halfW = objectWidth()/2.0;
    EmbReal rot = radians(rotation());
    EmbReal x = halfW*qCos(rot);
    EmbReal y = halfW*qSin(rot);
    return objectCenter() + QPointF(x,y);
}

/**
 * \brief .
 */
QPointF
EllipseObject::objectQuadrant90() const
{
    EmbReal halfH = objectHeight()/2.0;
    EmbReal rot = radians(rotation()+90.0);
    EmbReal x = halfH*qCos(rot);
    EmbReal y = halfH*qSin(rot);
    return objectCenter() + QPointF(x,y);
}

/**
 * \brief .
 */
QPointF
EllipseObject::objectQuadrant180() const
{
    EmbReal halfW = objectWidth()/2.0;
    EmbReal rot = radians(rotation()+180.0);
    EmbReal x = halfW*qCos(rot);
    EmbReal y = halfW*qSin(rot);
    return objectCenter() + QPointF(x,y);
}

/**
 * \brief .
 */
QPointF
EllipseObject::objectQuadrant270() const
{
    EmbReal halfH = objectHeight()/2.0;
    EmbReal rot = radians(rotation()+270.0);
    EmbReal x = halfH*qCos(rot);
    EmbReal y = halfH*qSin(rot);
    return objectCenter() + QPointF(x,y);
}

/**
 * \brief .
 */
void
EllipseObject::updatePath()
{
    QPainterPath path;
    QRectF r = rect();
    path.arcMoveTo(r, 0);
    path.arcTo(r, 0, 360);
    //NOTE: Reverse the path so that the inside area isn't considered part of the ellipse
    path.arcTo(r, 0, -360);
    setObjectPath(path);
}

/**
 * \brief .
 */
void
EllipseObject::paint(QPainter* painter, const QStyleOptionGraphicsItem* option, QWidget* /*widget*/)
{
    QGraphicsScene* objScene = scene();
    if(!objScene) return;

    QPen paintPen = pen();
    painter->setPen(paintPen);
    updateRubber(painter);
    if(option->state & QStyle::State_Selected)  { paintPen.setStyle(Qt::DashLine); }
    if(objScene->property(ENABLE_LWT).toBool()) { paintPen = lineWeightPen(); }
    painter->setPen(paintPen);

    painter->drawEllipse(rect());
}

/**
 * \brief .
 */
void
EllipseObject::updateRubber(QPainter* painter)
{
    int rubberMode = objectRubberMode();
    if(rubberMode == OBJ_RUBBER_ELLIPSE_LINE)
    {
        QPointF sceneLinePoint1 = objectRubberPoint("ELLIPSE_LINE_POINT1");
        QPointF sceneLinePoint2 = objectRubberPoint("ELLIPSE_LINE_POINT2");
        QPointF itemLinePoint1  = mapFromScene(sceneLinePoint1);
        QPointF itemLinePoint2  = mapFromScene(sceneLinePoint2);
        QLineF itemLine(itemLinePoint1, itemLinePoint2);
        if(painter) drawRubberLine(itemLine, painter, VIEW_COLOR_CROSSHAIR);
        updatePath();
    }
    else if(rubberMode == OBJ_RUBBER_ELLIPSE_MAJORDIAMETER_MINORRADIUS)
    {
        QPointF sceneAxis1Point1 = objectRubberPoint("ELLIPSE_AXIS1_POINT1");
        QPointF sceneAxis1Point2 = objectRubberPoint("ELLIPSE_AXIS1_POINT2");
        QPointF sceneCenterPoint = objectRubberPoint("ELLIPSE_CENTER");
        QPointF sceneAxis2Point2 = objectRubberPoint("ELLIPSE_AXIS2_POINT2");
        EmbReal ellipseWidth = objectRubberPoint("ELLIPSE_WIDTH").x();
        EmbReal ellipseRot = objectRubberPoint("ELLIPSE_ROT").x();

        //TODO: incorporate perpendicularDistance() into libcgeometry
        EmbReal px = sceneAxis2Point2.x();
        EmbReal py = sceneAxis2Point2.y();
        EmbReal x1 = sceneAxis1Point1.x();
        EmbReal y1 = sceneAxis1Point1.y();
        QLineF line(sceneAxis1Point1, sceneAxis1Point2);
        QLineF norm = line.normalVector();
        EmbReal dx = px-x1;
        EmbReal dy = py-y1;
        norm.translate(dx, dy);
        QPointF iPoint;
        norm.intersects(line, &iPoint);
        EmbReal ellipseHeight = QLineF(px, py, iPoint.x(), iPoint.y()).length()*2.0;

        setObjectCenter(sceneCenterPoint);
        setObjectSize(ellipseWidth, ellipseHeight);
        setRotation(-ellipseRot);

        QPointF itemCenterPoint = mapFromScene(sceneCenterPoint);
        QPointF itemAxis2Point2 = mapFromScene(sceneAxis2Point2);
        QLineF itemLine(itemCenterPoint, itemAxis2Point2);
        if(painter) drawRubberLine(itemLine, painter, VIEW_COLOR_CROSSHAIR);
        updatePath();
    }
    else if(rubberMode == OBJ_RUBBER_ELLIPSE_MAJORRADIUS_MINORRADIUS)
    {
        QPointF sceneAxis1Point2 = objectRubberPoint("ELLIPSE_AXIS1_POINT2");
        QPointF sceneCenterPoint = objectRubberPoint("ELLIPSE_CENTER");
        QPointF sceneAxis2Point2 = objectRubberPoint("ELLIPSE_AXIS2_POINT2");
        EmbReal ellipseWidth = objectRubberPoint("ELLIPSE_WIDTH").x();
        EmbReal ellipseRot = objectRubberPoint("ELLIPSE_ROT").x();

        //TODO: incorporate perpendicularDistance() into libcgeometry
        EmbReal px = sceneAxis2Point2.x();
        EmbReal py = sceneAxis2Point2.y();
        EmbReal x1 = sceneCenterPoint.x();
        EmbReal y1 = sceneCenterPoint.y();
        QLineF line(sceneCenterPoint, sceneAxis1Point2);
        QLineF norm = line.normalVector();
        EmbReal dx = px-x1;
        EmbReal dy = py-y1;
        norm.translate(dx, dy);
        QPointF iPoint;
        norm.intersects(line, &iPoint);
        EmbReal ellipseHeight = QLineF(px, py, iPoint.x(), iPoint.y()).length()*2.0;

        setObjectCenter(sceneCenterPoint);
        setObjectSize(ellipseWidth, ellipseHeight);
        setRotation(-ellipseRot);

        QPointF itemCenterPoint = mapFromScene(sceneCenterPoint);
        QPointF itemAxis2Point2 = mapFromScene(sceneAxis2Point2);
        QLineF itemLine(itemCenterPoint, itemAxis2Point2);
        if(painter) drawRubberLine(itemLine, painter, VIEW_COLOR_CROSSHAIR);
        updatePath();
    }
    else if(rubberMode == OBJ_RUBBER_GRIP)
    {
        //TODO: updateRubber() gripping for EllipseObject
    }
}

/**
 * \brief .
 */
void
EllipseObject::vulcanize()
{
    qDebug("EllipseObject vulcanize()");
    updateRubber();

    setObjectRubberMode(OBJ_RUBBER_OFF);
}

// Returns the closest snap point to the mouse point
/**
 * \brief .
 */
QPointF
EllipseObject::mouseSnapPoint(const QPointF& mousePoint)
{
    QPointF center  = objectCenter();
    QPointF quad0   = objectQuadrant0();
    QPointF quad90  = objectQuadrant90();
    QPointF quad180 = objectQuadrant180();
    QPointF quad270 = objectQuadrant270();

    EmbReal cntrDist = QLineF(mousePoint, center).length();
    EmbReal q0Dist   = QLineF(mousePoint, quad0).length();
    EmbReal q90Dist  = QLineF(mousePoint, quad90).length();
    EmbReal q180Dist = QLineF(mousePoint, quad180).length();
    EmbReal q270Dist = QLineF(mousePoint, quad270).length();

    EmbReal minDist = qMin(qMin(qMin(q0Dist, q90Dist), qMin(q180Dist, q270Dist)), cntrDist);

    if     (minDist == cntrDist) return center;
    else if(minDist == q0Dist)   return quad0;
    else if(minDist == q90Dist)  return quad90;
    else if(minDist == q180Dist) return quad180;
    else if(minDist == q270Dist) return quad270;

    return scenePos();
}

/**
 * \brief .
 */
QList<QPointF>
EllipseObject::allGripPoints()
{
    QList<QPointF> gripPoints;
    gripPoints << objectCenter() << objectQuadrant0() << objectQuadrant90() << objectQuadrant180() << objectQuadrant270();
    return gripPoints;
}

/**
 * \brief .
 */
void EllipseObject::gripEdit(const QPointF& before, const QPointF& after)
{
    //TODO: gripEdit() for EllipseObject
}

/**
 * \brief .
 */
QPainterPath EllipseObject::objectSavePath() const
{
    QPainterPath path;
    QRectF r = rect();
    path.arcMoveTo(r, 0);
    path.arcTo(r, 0, 360);

    EmbReal s = scale();
    QTransform trans;
    trans.rotate(rotation());
    trans.scale(s,s);
    return trans.map(path);
}
