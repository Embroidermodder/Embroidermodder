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
 * @brief LineObject::LineObject
 * @param x1
 * @param y1
 * @param x2
 * @param y2
 * @param rgb
 * @param parent
 */
LineObject::LineObject(EmbLine line, QRgb rgb, QGraphicsItem* parent) : BaseObject(parent)
{
    debug_message("LineObject Constructor()");
    init(line, rgb, Qt::SolidLine); //TODO: getCurrentLineType
}

/**
 * @brief LineObject::LineObject
 * @param obj
 * @param parent
 */
LineObject::LineObject(LineObject* obj, QGraphicsItem* parent) : BaseObject(parent)
{
    debug_message("LineObject Constructor()");
    if (obj) {
        EmbLine line;
        line.start = to_EmbVector(obj->objectEndPoint1());
        line.end = to_EmbVector(obj->objectEndPoint2());
        init(line, obj->objectColorRGB(), Qt::SolidLine); //TODO: getCurrentLineType
    }
}

/**
 * @brief LineObject::~LineObject
 */
LineObject::~LineObject()
{
    debug_message("LineObject Destructor()");
}

/**
 * @brief LineObject::init
 * @param x1
 * @param y1
 * @param x2
 * @param y2
 * @param rgb
 * @param lineType
 */
void
LineObject::init(EmbLine line, QRgb rgb, Qt::PenStyle lineType)
{
    setData(OBJ_TYPE, OBJ_TYPE_LINE);
    setData(OBJ_NAME, "Line");

    //WARNING: DO NOT enable QGraphicsItem::ItemIsMovable. If it is enabled,
    //WARNING: and the item is double clicked, the scene will erratically move the item while zooming.
    //WARNING: All movement has to be handled explicitly by us, not by the scene.
    setFlag(QGraphicsItem::ItemIsSelectable, true);

    setObjectEndPoint1(line.start.x, line.start.y);
    setObjectEndPoint2(line.end.x, line.end.y);
    setObjectColor(rgb);
    setObjectLineType(lineType);
    setObjectLineWeight(0.35); //TODO: pass in proper lineweight
    setPen(objPen);
}

/**
 * @brief LineObject::setObjectEndPoint1
 * @param endPt1
 */
void
LineObject::setObjectEndPoint1(const QPointF& endPt1)
{
    setObjectEndPoint1(endPt1.x(), endPt1.y());
}

/**
 * @brief LineObject::setObjectEndPoint1
 * @param x1
 * @param y1
 */
void
LineObject::setObjectEndPoint1(EmbReal x1, EmbReal y1)
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

/**
 * @brief LineObject::setObjectEndPoint2
 * @param endPt2
 */
void
LineObject::setObjectEndPoint2(const QPointF& endPt2)
{
    setObjectEndPoint2(endPt2.x(), endPt2.y());
}

/**
 * @brief LineObject::setObjectEndPoint2
 * @param x2
 * @param y2
 */
void
LineObject::setObjectEndPoint2(EmbReal x2, EmbReal y2)
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

/**
 * @brief LineObject::objectEndPoint2
 * @return
 */
QPointF
LineObject::objectEndPoint2() const
{
    QLineF lyne = line();
    EmbReal rot = radians(rotation());
    EmbVector v;
    v.x = lyne.x2()*scale();
    v.y = lyne.y2()*scale();
    EmbVector rotEnd = rotate_vector(v, rot);

    return (scenePos() + QPointF(rotEnd.x, rotEnd.y));
}

/**
 * @brief LineObject::objectMidPoint
 * @return
 */
QPointF
LineObject::objectMidPoint() const
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

/**
 * @brief LineObject::objectAngle
 * @return
 */
EmbReal
LineObject::objectAngle() const
{
    EmbReal angle = line().angle() - rotation();
    return std::fmod(angle, 360.0);
}

/**
 * @brief LineObject::paint
 * @param painter
 * @param option
 */
void
LineObject::paint(QPainter* painter, const QStyleOptionGraphicsItem* option, QWidget* /*widget*/)
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

/**
 * @brief LineObject::updateRubber
 * @param painter
 */
void
LineObject::updateRubber(QPainter* painter)
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

/**
 * @brief LineObject::vulcanize
 */
void
LineObject::vulcanize()
{
    debug_message("LineObject vulcanize()");
    updateRubber();

    objRubberMode = OBJ_RUBBER_OFF;
}

/**
 * @brief LineObject::mouseSnapPoint
 * @param mousePoint
 * @return The closest snap point to the mouse point.
 */
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

/**
 * @brief LineObject::allGripPoints
 * @return
 */
QList<QPointF>
LineObject::allGripPoints()
{
    QList<QPointF> gripPoints;
    gripPoints << objectEndPoint1() << objectEndPoint2() << objectMidPoint();
    return gripPoints;
}

/**
 * @brief LineObject::gripEdit
 * @param before
 * @param after
 */
void
LineObject::gripEdit(const QPointF& before, const QPointF& after)
{
    if (before == objectEndPoint1()) {
        setObjectEndPoint1(after);
    }
    else if (before == objectEndPoint2()) {
        setObjectEndPoint2(after);
    }
    else if (before == objectMidPoint()) {
        QPointF delta = after-before;
        moveBy(delta.x(), delta.y());
    }
}

/**
 * @brief LineObject::objectSavePath
 * @return
 */
QPainterPath
LineObject::objectSavePath() const
{
    QPainterPath path;
    /* \todo discards qualifiers error. */
    // QPointF delta = objectDelta();
    // path.lineTo(delta.x(), delta.y());
    return path;
}
