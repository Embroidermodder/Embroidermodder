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
 * \file object-arc.cpp
 */

#include "embroidermodder.h"

/**
 * @brief ArcObject::ArcObject
 * @param obj
 * @param parent
 */
ArcObject::ArcObject(ArcObject* obj, QGraphicsItem* parent) : BaseObject(parent)
{
    debug_message("ArcObject Constructor()");
    if (obj) {
        EmbArc arc;
        arc.start = to_EmbVector(obj->objectStartPoint());
        arc.mid = to_EmbVector(obj->objectMidPoint());
        arc.end = to_EmbVector(obj->objectEndPoint());
        init(arc, obj->objectColorRGB(), Qt::SolidLine); //TODO: getCurrentLineType
        setRotation(obj->rotation());
    }
}

/**
 * @brief ArcObject::~ArcObject
 */
ArcObject::~ArcObject()
{
    debug_message("ArcObject Destructor()");
}

/**
 * @brief ArcObject::init
 * @param arc
 * @param rgb
 * @param lineType
 */
void ArcObject::init(EmbArc arc, QRgb rgb, Qt::PenStyle lineType)
{
    setData(OBJ_TYPE, OBJ_TYPE_ARC);
    setData(OBJ_NAME, "Arc");

    //WARNING: DO NOT enable QGraphicsItem::ItemIsMovable. If it is enabled,
    //WARNING: and the item is double clicked, the scene will erratically move the item while zooming.
    //WARNING: All movement has to be handled explicitly by us, not by the scene.
    setFlag(QGraphicsItem::ItemIsSelectable, true);

    calculateArcData(arc);

    setObjectColor(rgb);
    setObjectLineType(lineType);
    setObjectLineWeight(0.35); //TODO: pass in proper lineweight
    setPen(objectPen());
}

/**
 * @brief ArcObject::calculateArcData
 * @param arc
 */
void ArcObject::calculateArcData(EmbArc arc)
{
    EmbVector center;
    getArcCenter(arc, &center);

    arcStartPoint = QPointF(arc.start.x - center.x, arc.start.y - center.y);
    arcMidPoint   = QPointF(arc.mid.x   - center.x, arc.mid.y   - center.y);
    arcEndPoint   = QPointF(arc.end.x   - center.x, arc.end.y   - center.y);

    setPos(center.x, center.y);

    EmbReal radius = QLineF(center.x, center.y, arc.mid.x, arc.mid.y).length();
    updateArcRect(radius);
    updatePath();
    setRotation(0);
    setScale(1);
}

/**
 * @brief ArcObject::updateArcRect
 * @param radius
 */
void
ArcObject::updateArcRect(EmbReal radius)
{
    QRectF arcRect;
    arcRect.setWidth(radius*2.0);
    arcRect.setHeight(radius*2.0);
    arcRect.moveCenter(QPointF(0,0));
    setRect(arcRect);
}

/**
 * @brief ArcObject::setObjectRadius
 * @param radius
 */
void
ArcObject::setObjectRadius(EmbReal radius)
{
    EmbReal rad = std::max(radius, 0.0000001f);

    QPointF center = scenePos();
    QLineF startLine = QLineF(center, objectStartPoint());
    QLineF midLine   = QLineF(center, objectMidPoint());
    QLineF endLine   = QLineF(center, objectEndPoint());
    startLine.setLength(rad);
    midLine.setLength(rad);
    endLine.setLength(rad);
    arcStartPoint = startLine.p2();
    arcMidPoint = midLine.p2();
    arcEndPoint = endLine.p2();
    EmbArc arc;
    arc.start = to_EmbVector(arcStartPoint);
    arc.mid = to_EmbVector(arcMidPoint);
    arc.end = to_EmbVector(arcEndPoint);

    calculateArcData(arc);
}

/**
 * @brief ArcObject::setObjectStartAngle
 * @param angle
 */
void
ArcObject::setObjectStartAngle(EmbReal angle)
{
    //TODO: ArcObject setObjectStartAngle
}

/**
 * @brief ArcObject::setObjectEndAngle
 * @param angle
 */
void
ArcObject::setObjectEndAngle(EmbReal angle)
{
    //TODO: ArcObject setObjectEndAngle
}

/**
 * @brief ArcObject::setObjectStartPoint
 * @param point
 */
void
ArcObject::setObjectStartPoint(EmbVector point)
{
    EmbArc arc;
    arc.start = point;
    arc.mid = to_EmbVector(objectMidPoint());
    arc.mid = to_EmbVector(objectEndPoint());
    calculateArcData(arc);
}

/**
 * @brief ArcObject::setObjectMidPoint
 * @param point
 */
void
ArcObject::setObjectMidPoint(EmbVector point)
{
    EmbArc arc;
    arc.start = to_EmbVector(objectStartPoint());
    arc.mid = point;
    arc.end = to_EmbVector(objectEndPoint());
    calculateArcData(arc);
}

/**
 * @brief ArcObject::setObjectEndPoint
 * @param point
 */
void
ArcObject::setObjectEndPoint(EmbVector point)
{
    EmbArc arc;
    arc.start = to_EmbVector(objectStartPoint());
    arc.mid = to_EmbVector(objectMidPoint());
    arc.end = point;
    calculateArcData(arc);
}

/**
 * @brief ArcObject::objectStartAngle
 * @return
 */
EmbReal ArcObject::objectStartAngle() const
{
    EmbReal angle = QLineF(scenePos(), objectStartPoint()).angle();
    return std::fmod(angle, 360.0);
}

/**
 * @brief ArcObject::objectEndAngle
 * @return
 */
EmbReal ArcObject::objectEndAngle() const
{
    EmbReal angle = QLineF(scenePos(), objectEndPoint()).angle();
    return std::fmod(angle, 360.0);
}

/**
 * @brief 
 * @return 
 */
EmbVector
rotate_vector(EmbVector v, EmbReal alpha)
{
    EmbVector rotv;
    EmbVector u = embVector_unit(alpha);
    rotv.x = v.x*u.x - v.y*u.y;
    rotv.y = v.x*u.y + v.y*u.x;
    return rotv;    
}

/**
 * @brief ArcObject::objectStartPoint
 * @return
 */
QPointF ArcObject::objectStartPoint() const
{
    EmbVector v;
    EmbReal rot = radians(rotation());
    embVector_multiply(to_EmbVector(arcStartPoint), scale(), &v);
    EmbVector rotv = rotate_vector(v, rot);

    return scenePos() + to_QPointF(rotv);
}

/**
 * @brief ArcObject::objectMidPoint
 * @return
 */
QPointF ArcObject::objectMidPoint() const
{
    EmbVector v;
    EmbReal rot = radians(rotation());
    embVector_multiply(to_EmbVector(arcMidPoint), scale(), &v);
    EmbVector rotv = rotate_vector(v, rot);

    return scenePos() + to_QPointF(rotv);
}

/**
 * @brief ArcObject::objectEndPoint
 * @return
 */
QPointF ArcObject::objectEndPoint() const
{
    EmbVector v;
    EmbReal rot = radians(rotation());
    embVector_multiply(to_EmbVector(arcEndPoint), scale(), &v);
    EmbVector rotv = rotate_vector(v, rot);

    return scenePos() + to_QPointF(rotv);
}

/**
 * @brief ArcObject::objectArea
 * @return
 */
EmbReal ArcObject::objectArea() const
{
    //Area of a circular segment
    EmbReal r = objectRadius();
    EmbReal theta = radians(objectIncludedAngle());
    return ((r*r)/2)*(theta - std::sin(theta));
}

/**
 * @brief ArcObject::objectArcLength
 * @return
 */
EmbReal ArcObject::objectArcLength() const
{
    return radians(objectIncludedAngle())*objectRadius();
}

/**
 * @brief ArcObject::objectChord
 * @return
 */
EmbReal ArcObject::objectChord() const
{
    return QLineF(
        objectStartPoint().x(),
        objectStartPoint().y(),
        objectEndPoint().x(),
        objectEndPoint().y()).length();
}

/**
 * @brief ArcObject::objectIncludedAngle
 * @return
 */
EmbReal ArcObject::objectIncludedAngle() const
{
    EmbReal chord = objectChord();
    EmbReal rad = objectRadius();
    if (chord <= 0 || rad <= 0) return 0; //Prevents division by zero and non-existant circles

    //NOTE: Due to floating point rounding errors, we need to clamp the quotient so it is in the range [-1, 1]
    //      If the quotient is out of that range, then the result of asin() will be NaN.
    EmbReal quotient = chord/(2.0*rad);
    if (quotient > 1.0) quotient = 1.0;
    if (quotient < 0.0) quotient = 0.0; //NOTE: 0 rather than -1 since we are enforcing a positive chord and radius
    return degrees(2.0*asin(quotient)); //Properties of a Circle - Get the Included Angle - Reference: ASD9
}

/**
 * @brief ArcObject::objectClockwise
 * @return
 */
bool ArcObject::objectClockwise() const
{
    //NOTE: Y values are inverted here on purpose
    EmbArc arc;
    arc.start.x = objectStartPoint().x();
    arc.start.y = -objectStartPoint().y();
    arc.mid.x = objectMidPoint().x();
    arc.mid.y = -objectMidPoint().y();
    arc.end.x = objectEndPoint().x();
    arc.end.y = -objectEndPoint().y();
    if (embArc_clockwise(arc)) {
        return true;
    }
    return false;
}

/**
 * @brief ArcObject::updatePath
 */
void ArcObject::updatePath()
{
    EmbReal startAngle = (objectStartAngle() + rotation());
    EmbReal spanAngle = objectIncludedAngle();

    if (objectClockwise()) {
        spanAngle = -spanAngle;
    }

    QPainterPath path;
    path.arcMoveTo(rect(), startAngle);
    path.arcTo(rect(), startAngle, spanAngle);
    //NOTE: Reverse the path so that the inside area isn't considered part of the arc
    path.arcTo(rect(), startAngle+spanAngle, -spanAngle);
    setObjectPath(path);
}

/**
 * @brief ArcObject::paint
 * @param painter
 * @param option
 */
void ArcObject::paint(QPainter* painter, const QStyleOptionGraphicsItem* option, QWidget* /*widget*/)
{
    QGraphicsScene* objScene = scene();
    if (!objScene) {
        return;
    }

    QPen paintPen = pen();
    painter->setPen(paintPen);
    updateRubber(painter);
    if (option->state & QStyle::State_Selected) {
        paintPen.setStyle(Qt::DashLine);
    }
    if (objScene->property("ENABLE_LWT").toBool()) {
        paintPen = lineWeightPen();
    }
    painter->setPen(paintPen);

    EmbReal startAngle = (objectStartAngle() + rotation())*16;
    EmbReal spanAngle = objectIncludedAngle()*16;

    if (objectClockwise())
        spanAngle = -spanAngle;

    EmbReal rad = objectRadius();
    QRectF paintRect(-rad, -rad, rad*2.0, rad*2.0);
    painter->drawArc(paintRect, startAngle, spanAngle);
}

/**
 * @brief ArcObject::updateRubber
 * @param painter
 *
 * \todo Arc Rubber Modes
 *
 * \todo updateRubber() gripping for ArcObject
 */
void
ArcObject::updateRubber(QPainter* painter)
{

}

/**
 * @brief ArcObject::vulcanize
 */
void
ArcObject::vulcanize()
{
    debug_message("ArcObject vulcanize()");
    updateRubber();

    setObjectRubberMode(OBJ_RUBBER_OFF);
}

/**
 * @brief ArcObject::mouseSnapPoint
 * @param mousePoint
 * @return the closest snap point to the mouse point.
 */
QPointF
ArcObject::mouseSnapPoint(const QPointF& mousePoint)
{
    QPointF center = objectCenter();
    QPointF start  = objectStartPoint();
    QPointF mid    = objectMidPoint();
    QPointF end    = objectEndPoint();

    EmbReal cntrDist  = QLineF(mousePoint, center).length();
    EmbReal startDist = QLineF(mousePoint, start).length();
    EmbReal midDist   = QLineF(mousePoint, mid).length();
    EmbReal endDist   = QLineF(mousePoint, end).length();

    EmbReal minDist = qMin(qMin(cntrDist, startDist), qMin(midDist, endDist));

    if     (minDist == cntrDist)  return center;
    else if (minDist == startDist) return start;
    else if (minDist == midDist)   return mid;
    else if (minDist == endDist)   return end;

    return scenePos();
}

/**
 * @brief ArcObject::allGripPoints
 * @return
 */
QList<QPointF>
ArcObject::allGripPoints()
{
    QList<QPointF> gripPoints;
    gripPoints << objectCenter() << objectStartPoint() << objectMidPoint() << objectEndPoint();
    return gripPoints;
}

/**
 * @brief ArcObject::gripEdit
 * @param before
 * @param after
 *
 * \todo gripEdit() for ArcObject
 */
void
ArcObject::gripEdit(const QPointF& before, const QPointF& after)
{
}
