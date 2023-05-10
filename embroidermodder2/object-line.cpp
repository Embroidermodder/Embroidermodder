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

LineObject::LineObject(EmbReal x1, EmbReal y1, EmbReal x2, EmbReal y2, QRgb rgb, QGraphicsItem* parent) : BaseObject(parent)
{
    debug_message("LineObject Constructor()");
    init(x1, y1, x2, y2, rgb, Qt::SolidLine); //TODO: getCurrentLineType
}

LineObject::LineObject(LineObject* obj, QGraphicsItem* parent) : BaseObject(parent)
{
    debug_message("LineObject Constructor()");
    if (obj)
    {
        init(obj->objectX1(), obj->objectY1(), obj->objectX2(), obj->objectY2(), obj->objectColorRGB(), Qt::SolidLine); //TODO: getCurrentLineType
    }
}

LineObject::~LineObject()
{
    debug_message("LineObject Destructor()");
}

void LineObject::init(EmbReal x1, EmbReal y1, EmbReal x2, EmbReal y2, QRgb rgb, Qt::PenStyle lineType)
{
    setData(OBJ_TYPE, OBJ_TYPE_LINE);
    setData(OBJ_NAME, "Line");

    //WARNING: DO NOT enable QGraphicsItem::ItemIsMovable. If it is enabled,
    //WARNING: and the item is double clicked, the scene will erratically move the item while zooming.
    //WARNING: All movement has to be handled explicitly by us, not by the scene.
    setFlag(QGraphicsItem::ItemIsSelectable, true);

    setObjectEndPoint1(x1, y1);
    setObjectEndPoint2(x2, y2);
    setObjectColor(rgb);
    setObjectLineType(lineType);
    setObjectLineWeight(0.35); //TODO: pass in proper lineweight
    setPen(objPen);
}

void LineObject::setObjectEndPoint1(const QPointF& endPt1)
{
    setObjectEndPoint1(endPt1.x(), endPt1.y());
}

void LineObject::setObjectEndPoint1(EmbReal x1, EmbReal y1)
{
    QPointF endPt2 = objectEndPoint2();
    EmbReal x2 = endPt2.x();
    EmbReal y2 = endPt2.y();
    EmbReal dx = x2 - x1;
    EmbReal dy = y2 - y1;
    setRotation(0);
    setScale(1);
    setLine(0, 0, dx, dy);
    setPos(x1, y1);
}

void LineObject::setObjectEndPoint2(const QPointF& endPt2)
{
    setObjectEndPoint2(endPt2.x(), endPt2.y());
}

void LineObject::setObjectEndPoint2(EmbReal x2, EmbReal y2)
{
    QPointF endPt1 = scenePos();
    EmbReal x1 = endPt1.x();
    EmbReal y1 = endPt1.y();
    EmbReal dx = x2 - x1;
    EmbReal dy = y2 - y1;
    setRotation(0);
    setScale(1);
    setLine(0, 0, dx, dy);
    setPos(x1, y1);
}

QPointF LineObject::objectEndPoint2() const
{
    QLineF lyne = line();
    EmbReal rot = radians(rotation());
    EmbVector v;
    v.x = lyne.x2()*scale();
    v.y = lyne.y2()*scale();
    EmbVector rotEnd = rotate_vector(v, rot);

    return (scenePos() + QPointF(rotEnd.x, rotEnd.y));
}

QPointF LineObject::objectMidPoint() const
{
    QLineF lyne = line();
    QPointF mp = lyne.pointAt(0.5);
    EmbReal rot = radians(rotation());
    EmbVector m;
    m.x = mp.x()*scale();
    m.y = mp.y()*scale();
    EmbVector rotMid = rotate_vector(m, rot);

    return (scenePos() + QPointF(rotMid.x, rotMid.y));
}

EmbReal LineObject::objectAngle() const
{
    EmbReal angle = line().angle() - rotation();
    return std::fmod(angle, 360.0);
}

void LineObject::paint(QPainter* painter, const QStyleOptionGraphicsItem* option, QWidget* /*widget*/)
{
    QGraphicsScene* objScene = scene();
    if (!objScene) return;

    QPen paintPen = pen();
    painter->setPen(paintPen);
    updateRubber(painter);
    if (option->state & QStyle::State_Selected)  { paintPen.setStyle(Qt::DashLine); }
    if (objScene->property("ENABLE_LWT").toBool()) { paintPen = lineWeightPen(); }
    painter->setPen(paintPen);

    if (objRubberMode != OBJ_RUBBER_LINE) {
        painter->drawLine(line());
    }

    if (objScene->property("ENABLE_LWT").toBool()
        && objScene->property("ENABLE_REAL").toBool()) {
        realRender(painter, path());
    }
}

void LineObject::updateRubber(QPainter* painter)
{
    int rubberMode = objRubberMode;
    if (rubberMode == OBJ_RUBBER_LINE) {
        QPointF sceneStartPoint = objectRubberPoint("LINE_START");
        QPointF sceneQSnapPoint = objectRubberPoint("LINE_END");

        setObjectEndPoint1(sceneStartPoint);
        setObjectEndPoint2(sceneQSnapPoint);

        drawRubberLine(line(), painter, "VIEW_COLOR_CROSSHAIR");
    }
    else if (rubberMode == OBJ_RUBBER_GRIP) {
        if (painter) {
            QPointF gripPoint = objectRubberPoint("GRIP_POINT");
            if (gripPoint == objectEndPoint1()) {
                painter->drawLine(line().p2(), mapFromScene(objectRubberPoint(QString())));
            }
            else if (gripPoint == objectEndPoint2()) {
                painter->drawLine(line().p1(), mapFromScene(objectRubberPoint(QString())));
            }
            else if (gripPoint == objectMidPoint())
                painter->drawLine(line().translated(mapFromScene(objectRubberPoint(QString()))-mapFromScene(gripPoint)));

            QLineF rubLine(mapFromScene(gripPoint), mapFromScene(objectRubberPoint(QString())));
            drawRubberLine(rubLine, painter, "VIEW_COLOR_CROSSHAIR");
        }
    }
}

void LineObject::vulcanize()
{
    debug_message("LineObject vulcanize()");
    updateRubber();

    objRubberMode = OBJ_RUBBER_OFF;
}

// Returns the closest snap point to the mouse point
QPointF LineObject::mouseSnapPoint(const QPointF& mousePoint)
{
    QPointF endPoint1 = objectEndPoint1();
    QPointF endPoint2 = objectEndPoint2();
    QPointF midPoint  = objectMidPoint();

    EmbReal end1Dist = QLineF(mousePoint, endPoint1).length();
    EmbReal end2Dist = QLineF(mousePoint, endPoint2).length();
    EmbReal midDist  = QLineF(mousePoint, midPoint).length();

    EmbReal minDist = qMin(qMin(end1Dist, end2Dist), midDist);

    if     (minDist == end1Dist) return endPoint1;
    else if (minDist == end2Dist) return endPoint2;
    else if (minDist == midDist)  return midPoint;

    return scenePos();
}

QList<QPointF> LineObject::allGripPoints()
{
    QList<QPointF> gripPoints;
    gripPoints << objectEndPoint1() << objectEndPoint2() << objectMidPoint();
    return gripPoints;
}

void LineObject::gripEdit(const QPointF& before, const QPointF& after)
{
    if     (before == objectEndPoint1()) { setObjectEndPoint1(after); }
    else if (before == objectEndPoint2()) { setObjectEndPoint2(after); }
    else if (before == objectMidPoint())  { QPointF delta = after-before; moveBy(delta.x(), delta.y()); }
}

QPainterPath LineObject::objectSavePath() const
{
    QPainterPath path;
    path.lineTo(objectDeltaX(), objectDeltaY());
    return path;
}
