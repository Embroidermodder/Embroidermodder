#include "object-arc.h"
#include "object-data.h"
#include "embroidery.h"

#include <QPainter>
#include <QStyleOption>
#include <QGraphicsScene>

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
ArcObject::ArcObject(qreal startX, qreal startY, qreal midX, qreal midY, qreal endX, qreal endY, QRgb rgb, QGraphicsItem* parent) : BaseObject(parent)
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
    if(obj)
    {
        init(obj->objectStartX(), obj->objectStartY(), obj->objectMidX(), obj->objectMidY(), obj->objectEndX(), obj->objectEndY(), obj->objectColorRGB(), Qt::SolidLine); //TODO: getCurrentLineType
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
void ArcObject::init(qreal startX, qreal startY, qreal midX, qreal midY, qreal endX, qreal endY, QRgb rgb, Qt::PenStyle lineType)
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
void ArcObject::calculateArcData(qreal startX, qreal startY, qreal midX, qreal midY, qreal endX, qreal endY)
{
    double centerX;
    double centerY;
    getArcCenter(startX,  startY,
                 midX,    midY,
                 endX,    endY,
                 &centerX, &centerY);

    arcStartPoint = QPointF(startX - centerX, startY - centerY);
    arcMidPoint   = QPointF(midX   - centerX, midY   - centerY);
    arcEndPoint   = QPointF(endX   - centerX, endY   - centerY);

    setPos(centerX, centerY);

    qreal radius = QLineF(centerX, centerY, midX, midY).length();
    updateArcRect(radius);
    updatePath();
    setRotation(0);
    setScale(1);
}

/**
 * @brief ArcObject::updateArcRect
 * @param radius
 */
void ArcObject::updateArcRect(qreal radius)
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

void ArcObject::setObjectCenter(qreal pointX, qreal pointY)
{
    setPos(pointX, pointY);
}

void ArcObject::setObjectCenterX(qreal pointX)
{
    setX(pointX);
}

void ArcObject::setObjectCenterY(qreal pointY)
{
    setY(pointY);
}

void ArcObject::setObjectRadius(qreal radius)
{
    qreal rad;
    if(radius <= 0)
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

void ArcObject::setObjectStartAngle(qreal angle)
{
    //TODO: ArcObject setObjectStartAngle
}

void ArcObject::setObjectEndAngle(qreal angle)
{
    //TODO: ArcObject setObjectEndAngle
}

void ArcObject::setObjectStartPoint(const QPointF& point)
{
    setObjectStartPoint(point.x(), point.y());
}

void ArcObject::setObjectStartPoint(qreal pointX, qreal pointY)
{
    calculateArcData(pointX, pointY, arcMidPoint.x(), arcMidPoint.y(), arcEndPoint.x(), arcEndPoint.y());
}

void ArcObject::setObjectMidPoint(const QPointF& point)
{
    setObjectMidPoint(point.x(), point.y());
}

void ArcObject::setObjectMidPoint(qreal pointX, qreal pointY)
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
void ArcObject::setObjectEndPoint(qreal pointX, qreal pointY)
{
    calculateArcData(arcStartPoint.x(), arcStartPoint.y(), arcMidPoint.x(), arcMidPoint.y(), pointX, pointY);
}

/**
 * @brief ArcObject::objectStartAngle
 * @return
 */
qreal ArcObject::objectStartAngle() const
{
    qreal angle = QLineF(scenePos(), objectStartPoint()).angle();
    while(angle >= 360.0) { angle -= 360.0; }
    while(angle < 0.0)    { angle += 360.0; }
    return angle;
}

/**
 * @brief ArcObject::objectEndAngle
 * @return
 */
qreal ArcObject::objectEndAngle() const
{
    qreal angle = QLineF(scenePos(), objectEndPoint()).angle();
    while(angle >= 360.0) { angle -= 360.0; }
    while(angle < 0.0)    { angle += 360.0; }
    return angle;
}

/**
 * @brief ArcObject::objectStartPoint
 * @return
 */
QPointF ArcObject::objectStartPoint() const
{
    qreal rot = radians(rotation());
    qreal cosRot = qCos(rot);
    qreal sinRot = qSin(rot);
    qreal x = arcStartPoint.x()*scale();
    qreal y = arcStartPoint.y()*scale();
    qreal rotX = x*cosRot - y*sinRot;
    qreal rotY = x*sinRot + y*cosRot;

    return (scenePos() + QPointF(rotX, rotY));
}

/**
 * @brief ArcObject::objectStartX
 * @return
 */
qreal ArcObject::objectStartX() const
{
    return objectStartPoint().x();
}

/**
 * @brief ArcObject::objectStartY
 * @return
 */
qreal ArcObject::objectStartY() const
{
    return objectStartPoint().y();
}

/**
 * @brief ArcObject::objectMidPoint
 * @return
 */
QPointF ArcObject::objectMidPoint() const
{
    qreal rot = radians(rotation());
    qreal cosRot = qCos(rot);
    qreal sinRot = qSin(rot);
    qreal x = arcMidPoint.x()*scale();
    qreal y = arcMidPoint.y()*scale();
    qreal rotX = x*cosRot - y*sinRot;
    qreal rotY = x*sinRot + y*cosRot;

    return (scenePos() + QPointF(rotX, rotY));
}

/**
 * @brief ArcObject::objectMidX
 * @return
 */
qreal ArcObject::objectMidX() const
{
    return objectMidPoint().x();
}

/**
 * @brief ArcObject::objectMidY
 * @return
 */
qreal ArcObject::objectMidY() const
{
    return objectMidPoint().y();
}

/**
 * @brief ArcObject::objectEndPoint
 * @return
 */
QPointF ArcObject::objectEndPoint() const
{
    qreal rot = radians(rotation());
    qreal cosRot = qCos(rot);
    qreal sinRot = qSin(rot);
    qreal x = arcEndPoint.x()*scale();
    qreal y = arcEndPoint.y()*scale();
    qreal rotX = x*cosRot - y*sinRot;
    qreal rotY = x*sinRot + y*cosRot;

    return (scenePos() + QPointF(rotX, rotY));
}

/**
 * @brief ArcObject::objectEndX
 * @return
 */
qreal ArcObject::objectEndX() const
{
    return objectEndPoint().x();
}

/**
 * @brief ArcObject::objectEndY
 * @return
 */
qreal ArcObject::objectEndY() const
{
    return objectEndPoint().y();
}

/**
 * @brief ArcObject::objectArea
 * @return
 */
qreal ArcObject::objectArea() const
{
    //Area of a circular segment
    qreal r = objectRadius();
    qreal theta = radians(objectIncludedAngle());
    return ((r*r)/2)*(theta - qSin(theta));
}

/**
 * @brief ArcObject::objectArcLength
 * @return
 */
qreal ArcObject::objectArcLength() const
{
    return radians(objectIncludedAngle())*objectRadius();
}

/**
 * @brief ArcObject::objectChord
 * @return
 */
qreal ArcObject::objectChord() const
{
    return QLineF(objectStartX(), objectStartY(), objectEndX(), objectEndY()).length();
}

/**
 * @brief ArcObject::objectIncludedAngle
 * @return
 */
qreal ArcObject::objectIncludedAngle() const
{
    qreal chord = objectChord();
    qreal rad = objectRadius();
    if(chord <= 0 || rad <= 0) return 0; //Prevents division by zero and non-existant circles

    //NOTE: Due to floating point rounding errors, we need to clamp the quotient so it is in the range [-1, 1]
    //      If the quotient is out of that range, then the result of asin() will be NaN.
    qreal quotient = chord/(2.0*rad);
    if(quotient > 1.0) quotient = 1.0;
    if(quotient < 0.0) quotient = 0.0; //NOTE: 0 rather than -1 since we are enforcing a positive chord and radius
    return degrees(2.0*asin(quotient)); //Properties of a Circle - Get the Included Angle - Reference: ASD9
}

/**
 * @brief ArcObject::objectClockwise
 * @return
 */
bool ArcObject::objectClockwise() const
{
    //NOTE: Y values are inverted here on purpose
    if(isArcClockwise(objectStartX(), -objectStartY(), objectMidX(), -objectMidY(), objectEndX(), -objectEndY()))
        return true;
    return false;
}

/**
 * @brief ArcObject::updatePath
 */
void ArcObject::updatePath()
{
    qreal startAngle = (objectStartAngle() + rotation());
    qreal spanAngle = objectIncludedAngle();

    if(objectClockwise())
        spanAngle = -spanAngle;

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
    if(!objScene) return;

    QPen paintPen = pen();
    painter->setPen(paintPen);
    updateRubber(painter);
    if(option->state & QStyle::State_Selected)  { paintPen.setStyle(Qt::DashLine); }
    if(objScene->property(ENABLE_LWT).toBool()) { paintPen = lineWeightPen(); }
    painter->setPen(paintPen);

    qreal startAngle = (objectStartAngle() + rotation())*16;
    qreal spanAngle = objectIncludedAngle()*16;

    if(objectClockwise())
        spanAngle = -spanAngle;

    qreal rad = objectRadius();
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

    qreal cntrDist  = QLineF(mousePoint, center).length();
    qreal startDist = QLineF(mousePoint, start).length();
    qreal midDist   = QLineF(mousePoint, mid).length();
    qreal endDist   = QLineF(mousePoint, end).length();

    qreal minDist = qMin(qMin(cntrDist, startDist), qMin(midDist, endDist));

    if     (minDist == cntrDist)  return center;
    else if(minDist == startDist) return start;
    else if(minDist == midDist)   return mid;
    else if(minDist == endDist)   return end;

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
