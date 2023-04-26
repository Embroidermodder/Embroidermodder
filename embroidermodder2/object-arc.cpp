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
 * \file object-arc.cpp
 */

#include "embroidermodder.h"

/**
 * @brief ArcObject::ArcObject
 * @param arc
 * @param rgb
 * @param parent
 */
ArcObject::ArcObject(EmbArc arc, QRgb rgb, QGraphicsItem* parent) : BaseObject(parent)
{
    qDebug("ArcObject Constructor()");
    init(
        arc.start.x, arc.start.y,
        arc.mid.x, arc.mid.y,
        arc.end.x, arc.end.y,
        rgb, Qt::SolidLine); //TODO: getCurrentLineType
}

/**
 * @brief ArcObject::ArcObject
 * @param startX
 * @param startY
 * @param midX
 * @param midY
 * @param endX
 * @param endY
 * @param rgb
 * @param parent
 */
ArcObject::ArcObject(EmbReal startX, EmbReal startY, EmbReal midX, EmbReal midY, EmbReal endX, EmbReal endY, QRgb rgb, QGraphicsItem* parent) : BaseObject(parent)
{
    qDebug("ArcObject Constructor()");
    init(startX, startY, midX, midY, endX, endY, rgb, Qt::SolidLine); //TODO: getCurrentLineType
}

/**
 * @brief ArcObject::ArcObject
 * @param obj
 * @param parent
 */
ArcObject::ArcObject(ArcObject* obj, QGraphicsItem* parent) : BaseObject(parent)
{
    qDebug("ArcObject Constructor()");
    if (obj) {
        init(
            obj->objectStartX(),
            obj->objectStartY(),
            obj->objectMidX(),
            obj->objectMidY(),
            obj->objectEndX(),
            obj->objectEndY(),
            obj->objectColorRGB(),
            Qt::SolidLine); //TODO: getCurrentLineType
        setRotation(obj->rotation());
    }
}

/**
 * @brief ArcObject::~ArcObject
 */
ArcObject::~ArcObject()
{
    qDebug("ArcObject Destructor()");
}

/**
 * @brief ArcObject::init
 * @param startX
 * @param startY
 * @param midX
 * @param midY
 * @param endX
 * @param endY
 * @param rgb
 * @param lineType
 */
void ArcObject::init(EmbReal startX, EmbReal startY, EmbReal midX, EmbReal midY, EmbReal endX, EmbReal endY, QRgb rgb, Qt::PenStyle lineType)
{
    setData(OBJ_TYPE, type());
    setData(OBJ_NAME, OBJ_NAME_ARC);

    //WARNING: DO NOT enable QGraphicsItem::ItemIsMovable. If it is enabled,
    //WARNING: and the item is double clicked, the scene will erratically move the item while zooming.
    //WARNING: All movement has to be handled explicitly by us, not by the scene.
    setFlag(QGraphicsItem::ItemIsSelectable, true);

    calculateArcData(startX, startY, midX, midY, endX, endY);

    setObjectColor(rgb);
    setObjectLineType(lineType);
    setObjectLineWeight(0.35); //TODO: pass in proper lineweight
    setPen(objectPen());
}

/**
 * @brief ArcObject::calculateArcData
 * @param startX
 * @param startY
 * @param midX
 * @param midY
 * @param endX
 * @param endY
 */
void ArcObject::calculateArcData(EmbReal startX, EmbReal startY, EmbReal midX, EmbReal midY, EmbReal endX, EmbReal endY)
{
    EmbVector center;
    EmbArc arc;
    arc.start.x = startX;
    arc.start.x = startY;
    arc.mid.x = midX;
    arc.mid.x = midY;
    arc.end.x = endX;
    arc.end.x = endY;
    getArcCenter(arc, &center);

    arcStartPoint = QPointF(startX - center.x, startY - center.y);
    arcMidPoint   = QPointF(midX   - center.x, midY   - center.y);
    arcEndPoint   = QPointF(endX   - center.x, endY   - center.y);

    setPos(center.x, center.y);

    EmbReal radius = QLineF(center.x, center.y, midX, midY).length();
    updateArcRect(radius);
    updatePath();
    setRotation(0);
    setScale(1);
}

/**
 * @brief ArcObject::updateArcRect
 * @param radius
 */
void ArcObject::updateArcRect(EmbReal radius)
{
    QRectF arcRect;
    arcRect.setWidth(radius*2.0);
    arcRect.setHeight(radius*2.0);
    arcRect.moveCenter(QPointF(0,0));
    setRect(arcRect);
}

/**
 * @brief ArcObject::setObjectCenter
 * @param point
 */
void ArcObject::setObjectCenter(const QPointF& point)
{
    setObjectCenter(point.x(), point.y());
}

void ArcObject::setObjectCenter(EmbReal pointX, EmbReal pointY)
{
    setPos(pointX, pointY);
}

void ArcObject::setObjectCenterX(EmbReal pointX)
{
    setX(pointX);
}

void ArcObject::setObjectCenterY(EmbReal pointY)
{
    setY(pointY);
}

void ArcObject::setObjectRadius(EmbReal radius)
{
    EmbReal rad;
    if (radius <= 0)
    {
        rad = 0.0000001;
    }
    else
        rad = radius;

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

    calculateArcData(arcStartPoint.x(), arcStartPoint.y(), arcMidPoint.x(), arcMidPoint.y(), arcEndPoint.x(), arcEndPoint.y());
}

void ArcObject::setObjectStartAngle(EmbReal angle)
{
    //TODO: ArcObject setObjectStartAngle
}

void ArcObject::setObjectEndAngle(EmbReal angle)
{
    //TODO: ArcObject setObjectEndAngle
}

void ArcObject::setObjectStartPoint(const QPointF& point)
{
    setObjectStartPoint(point.x(), point.y());
}

void ArcObject::setObjectStartPoint(EmbReal pointX, EmbReal pointY)
{
    calculateArcData(pointX, pointY, arcMidPoint.x(), arcMidPoint.y(), arcEndPoint.x(), arcEndPoint.y());
}

void ArcObject::setObjectMidPoint(const QPointF& point)
{
    setObjectMidPoint(point.x(), point.y());
}

void ArcObject::setObjectMidPoint(EmbReal pointX, EmbReal pointY)
{
    calculateArcData(arcStartPoint.x(), arcStartPoint.y(), pointX, pointY, arcEndPoint.x(), arcEndPoint.y());
}

/**
 * @brief ArcObject::setObjectEndPoint
 * @param point
 */
void ArcObject::setObjectEndPoint(const QPointF& point)
{
    setObjectEndPoint(point.x(), point.y());
}

/**
 * @brief ArcObject::setObjectEndPoint
 * @param pointX
 * @param pointY
 */
void ArcObject::setObjectEndPoint(EmbReal pointX, EmbReal pointY)
{
    calculateArcData(arcStartPoint.x(), arcStartPoint.y(), arcMidPoint.x(), arcMidPoint.y(), pointX, pointY);
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
 * @brief ArcObject::objectStartX
 * @return
 */
EmbReal ArcObject::objectStartX() const
{
    return objectStartPoint().x();
}

/**
 * @brief ArcObject::objectStartY
 * @return
 */
EmbReal ArcObject::objectStartY() const
{
    return objectStartPoint().y();
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
 * @brief ArcObject::objectMidX
 * @return
 */
EmbReal ArcObject::objectMidX() const
{
    return objectMidPoint().x();
}

/**
 * @brief ArcObject::objectMidY
 * @return
 */
EmbReal ArcObject::objectMidY() const
{
    return objectMidPoint().y();
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
 * @brief ArcObject::objectEndX
 * @return
 */
EmbReal ArcObject::objectEndX() const
{
    return objectEndPoint().x();
}

/**
 * @brief ArcObject::objectEndY
 * @return
 */
EmbReal ArcObject::objectEndY() const
{
    return objectEndPoint().y();
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
    return QLineF(objectStartX(), objectStartY(), objectEndX(), objectEndY()).length();
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
    arc.start.x = objectStartX();
    arc.start.y = -objectStartY();
    arc.mid.x = objectMidX();
    arc.mid.y = -objectMidY();
    arc.end.x = objectEndX();
    arc.end.y = -objectEndY();
    if (embArc_clockwise(arc))
        return true;
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
    if (objScene->property(ENABLE_LWT).toBool()) {
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
    qDebug("ArcObject vulcanize()");
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
