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
 * \file object-circle.cpp
 */

#include "embroidermodder.h"

CircleObject::CircleObject(EmbReal centerX, EmbReal centerY, EmbReal radius, QRgb rgb, QGraphicsItem* parent) : BaseObject(parent)
{
    qDebug("CircleObject Constructor()");
    init(centerX, centerY, radius, rgb, Qt::SolidLine); //TODO: getCurrentLineType
}

CircleObject::CircleObject(CircleObject* obj, QGraphicsItem* parent) : BaseObject(parent)
{
    qDebug("CircleObject Constructor()");
    if(obj)
    {
        init(obj->objectCenterX(), obj->objectCenterY(), obj->objectRadius(), obj->objectColorRGB(), Qt::SolidLine); //TODO: getCurrentLineType
        setRotation(obj->rotation());
    }
}

CircleObject::~CircleObject()
{
    qDebug("CircleObject Destructor()");
}

void CircleObject::init(EmbReal centerX, EmbReal centerY, EmbReal radius, QRgb rgb, Qt::PenStyle lineType)
{
    setData(OBJ_TYPE, OBJ_TYPE_CIRCLE);
    setData(OBJ_NAME, "Circle");

    //WARNING: DO NOT enable QGraphicsItem::ItemIsMovable. If it is enabled,
    //WARNING: and the item is double clicked, the scene will erratically move the item while zooming.
    //WARNING: All movement has to be handled explicitly by us, not by the scene.
    setFlag(QGraphicsItem::ItemIsSelectable, true);

    setObjectRadius(radius);
    EmbVector center = {centerX, centerY};
    setObjectCenter(center);
    setObjectColor(rgb);
    setObjectLineType(lineType);
    setObjectLineWeight(0.35); //TODO: pass in proper lineweight
    setPen(objPen);
    updatePath();
}

void CircleObject::setObjectRadius(EmbReal radius)
{
    setObjectDiameter(radius*2.0);
}

void CircleObject::setObjectDiameter(EmbReal diameter)
{
    QRectF circRect;
    circRect.setWidth(diameter);
    circRect.setHeight(diameter);
    circRect.moveCenter(QPointF(0,0));
    setRect(circRect);
    updatePath();
}

void CircleObject::setObjectArea(EmbReal area)
{
    EmbReal radius = std::sqrt(area/emb_constant_pi);
    setObjectRadius(radius);
}

void CircleObject::setObjectCircumference(EmbReal circumference)
{
    EmbReal diameter = circumference/emb_constant_pi;
    setObjectDiameter(diameter);
}

void CircleObject::updatePath()
{
    QPainterPath path;
    QRectF r = rect();
    //Add the center point
    path.addRect(-0.00000001, -0.00000001, 0.00000002, 0.00000002);
    //Add the circle
    path.arcMoveTo(r, 0);
    path.arcTo(r, 0, 360);
    //NOTE: Reverse the path so that the inside area isn't considered part of the circle
    path.arcTo(r, 0, -360);
    setObjectPath(path);
}

void CircleObject::paint(QPainter* painter, const QStyleOptionGraphicsItem* option, QWidget* /*widget*/)
{
    QGraphicsScene* objScene = scene();
    if(!objScene) return;

    QPen paintPen = pen();
    painter->setPen(paintPen);
    updateRubber(painter);
    if(option->state & QStyle::State_Selected)  { paintPen.setStyle(Qt::DashLine); }
    if(objScene->property("ENABLE_LWT").toBool()) { paintPen = lineWeightPen(); }
    painter->setPen(paintPen);

    painter->drawEllipse(rect());
}

void CircleObject::updateRubber(QPainter* painter)
{
    if (objRubberMode == OBJ_RUBBER_CIRCLE_1P_RAD) {
        QPointF sceneCenterPoint = objectRubberPoint("CIRCLE_CENTER");
        QPointF sceneQSnapPoint = objectRubberPoint("CIRCLE_RADIUS");
        QPointF itemCenterPoint = mapFromScene(sceneCenterPoint);
        QPointF itemQSnapPoint = mapFromScene(sceneQSnapPoint);
        QLineF itemLine(itemCenterPoint, itemQSnapPoint);
        setObjectCenter(to_EmbVector(sceneCenterPoint));
        QLineF sceneLine(sceneCenterPoint, sceneQSnapPoint);
        EmbReal radius = sceneLine.length();
        setObjectRadius(radius);
        if(painter) drawRubberLine(itemLine, painter, "VIEW_COLOR_CROSSHAIR");
        updatePath();
    }
    else if (objRubberMode == OBJ_RUBBER_CIRCLE_1P_DIA) {
        QPointF sceneCenterPoint = objectRubberPoint("CIRCLE_CENTER");
        QPointF sceneQSnapPoint = objectRubberPoint("CIRCLE_DIAMETER");
        QPointF itemCenterPoint = mapFromScene(sceneCenterPoint);
        QPointF itemQSnapPoint = mapFromScene(sceneQSnapPoint);
        QLineF itemLine(itemCenterPoint, itemQSnapPoint);
        setObjectCenter(to_EmbVector(sceneCenterPoint));
        QLineF sceneLine(sceneCenterPoint, sceneQSnapPoint);
        EmbReal diameter = sceneLine.length();
        setObjectDiameter(diameter);
        if(painter) drawRubberLine(itemLine, painter, "VIEW_COLOR_CROSSHAIR");
        updatePath();
    }
    else if (objRubberMode == OBJ_RUBBER_CIRCLE_2P) {
        QPointF sceneTan1Point = objectRubberPoint("CIRCLE_TAN1");
        QPointF sceneQSnapPoint = objectRubberPoint("CIRCLE_TAN2");
        QLineF sceneLine(sceneTan1Point, sceneQSnapPoint);
        setObjectCenter(to_EmbVector(sceneLine.pointAt(0.5)));
        EmbReal diameter = sceneLine.length();
        setObjectDiameter(diameter);
        updatePath();
    }
    else if (objRubberMode == OBJ_RUBBER_CIRCLE_3P) {
        QPointF sceneTan1Point = objectRubberPoint("CIRCLE_TAN1");
        QPointF sceneTan2Point = objectRubberPoint("CIRCLE_TAN2");
        QPointF sceneTan3Point = objectRubberPoint("CIRCLE_TAN3");

        EmbVector sceneCenter;
        EmbArc arc;
        arc.start.x = sceneTan1Point.x();
        arc.start.y = sceneTan1Point.y();
        arc.mid.x = sceneTan2Point.x();
        arc.mid.y = sceneTan2Point.y();
        arc.end.x = sceneTan3Point.x();
        arc.end.y = sceneTan3Point.y();
        getArcCenter(arc, &sceneCenter);
        QPointF sceneCenterPoint(sceneCenter.x, sceneCenter.y);
        QLineF sceneLine(sceneCenterPoint, sceneTan3Point);
        setObjectCenter(to_EmbVector(sceneCenterPoint));
        EmbReal radius = sceneLine.length();
        setObjectRadius(radius);
        updatePath();
    }
    else if (objRubberMode == OBJ_RUBBER_GRIP) {
        if (painter) {
            QPointF gripPoint = objectRubberPoint("GRIP_POINT");
            if(gripPoint == objectCenter())
            {
                painter->drawEllipse(rect().translated(mapFromScene(objectRubberPoint(QString()))-mapFromScene(gripPoint)));
            }
            else
            {
                EmbReal gripRadius = QLineF(objectCenter(), objectRubberPoint(QString())).length();
                painter->drawEllipse(QPointF(), gripRadius, gripRadius);
            }

            QLineF rubLine(mapFromScene(gripPoint), mapFromScene(objectRubberPoint(QString())));
            drawRubberLine(rubLine, painter, "VIEW_COLOR_CROSSHAIR");
        }
    }
}

void CircleObject::vulcanize()
{
    qDebug("CircleObject vulcanize()");
    updateRubber();

    setObjectRubberMode(OBJ_RUBBER_OFF);
}

// Returns the closest snap point to the mouse point
QPointF CircleObject::mouseSnapPoint(const QPointF& mousePoint)
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

QList<QPointF> CircleObject::allGripPoints()
{
    QList<QPointF> gripPoints;
    gripPoints << objectCenter() << objectQuadrant0() << objectQuadrant90() << objectQuadrant180() << objectQuadrant270();
    return gripPoints;
}

void CircleObject::gripEdit(const QPointF& before, const QPointF& after)
{
    if(before == objectCenter()) { QPointF delta = after-before; moveBy(delta.x(), delta.y()); }
    else                         { setObjectRadius(QLineF(objectCenter(), after).length()); }
}

QPainterPath CircleObject::objectSavePath() const
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
