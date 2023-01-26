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

#include "embroidermodder.h"

#if 0
ArcObject::ArcObject(double startX, double startY, double midX, double midY, double endX, double endY, unsigned int rgb, QGraphicsItem* parent) : BaseObject(parent)
{
    debug_message("ArcObject Constructor()");
    init(startX, startY, midX, midY, endX, endY, rgb, SolidLine); //TODO: getCurrentLineType
}

ArcObject::ArcObject(ArcObject* obj, QGraphicsItem* parent) : BaseObject(parent)
{
    debug_message("ArcObject Constructor()");
    if (obj) {
        init(obj->objectStartX(), obj->objectStartY(), obj->objectMidX(), obj->objectMidY(), obj->objectEndX(), obj->objectEndY(), obj->objectColorRGB(), SolidLine); //TODO: getCurrentLineType
        setRotation(obj->rotation());
    }
}

ArcObject::~ArcObject()
{
    debug_message("ArcObject Destructor()");
}

void ArcObject::init(double startX, double startY, double midX, double midY, double endX, double endY, unsigned int rgb, PenStyle lineType)
{
    setData(OBJ_TYPE, type);
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

void ArcObject::calculateArcData(double startX, double startY, double midX, double midY, double endX, double endY)
{
    double centerX;
    double centerY;
    EmbArc arc;
    EmbVector center;
    arc.start.x = startX;
    arc.start.y = startY;
    arc.mid.x = midX;
    arc.mid.y = midY;
    arc.end.x = endX;
    arc.end.y = endY;
    getArcCenter(arc, &center);

    arcStartPoint = embVector_subtract(arc.start, center);
    arcMidPoint = embVector_subtract(arc.mid, center);
    arcEndPoint = embVector_subtract(arc.end, center);

    setPos(center.x, center.y);

    double radius = QLineF(center.x, center.y, arc.mid.x, arc.mid.y).length();
    updateArcRect(radius);
    updatePath();
    setRotation(0);
    setScale(1);
}

void ArcObject::updateArcRect(double radius)
{
    QRectF arcRect;
    arcRect.setWidth(radius*2.0);
    arcRect.setHeight(radius*2.0);
    arcRect.moveCenter(EmbVector(0,0));
    setRect(arcRect);
}

void ArcObject::setObjectCenter(EmbVector& point)
{
    setObjectCenter(point.x(), point.y());
}

void ArcObject::setObjectCenter(double pointX, double pointY)
{
    setPos(pointX, pointY);
}

void ArcObject::setObjectCenterX(double pointX)
{
    setX(pointX);
}

void ArcObject::setObjectCenterY(double pointY)
{
    setY(pointY);
}

void ArcObject::setObjectRadius(double radius)
{
    double rad;
    if (radius <= 0)
    {
        rad = 0.0000001;
    }
    else
        rad = radius;

    EmbVector center = scenePos();
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

void ArcObject::setObjectStartAngle(double angle)
{
    //TODO: ArcObject setObjectStartAngle
}

void ArcObject::setObjectEndAngle(double angle)
{
    //TODO: ArcObject setObjectEndAngle
}

void ArcObject::setObjectStartPoint(EmbVector& point)
{
    setObjectStartPoint(point.x(), point.y());
}

void ArcObject::setObjectStartPoint(double pointX, double pointY)
{
    calculateArcData(pointX, pointY, arcMidPoint.x(), arcMidPoint.y(), arcEndPoint.x(), arcEndPoint.y());
}

void ArcObject::setObjectMidPoint(EmbVector& point)
{
    setObjectMidPoint(point.x(), point.y());
}

void ArcObject::setObjectMidPoint(double pointX, double pointY)
{
    calculateArcData(arcStartPoint.x(), arcStartPoint.y(), pointX, pointY, arcEndPoint.x(), arcEndPoint.y());
}

void ArcObject::setObjectEndPoint(EmbVector& point)
{
    setObjectEndPoint(point.x(), point.y());
}

void ArcObject::setObjectEndPoint(double pointX, double pointY)
{
    calculateArcData(arcStartPoint.x(), arcStartPoint.y(), arcMidPoint.x(), arcMidPoint.y(), pointX, pointY);
}

double ArcObject::objectStartAngle() const
{
    double angle = QLineF(scenePos(), objectStartPoint()).angle();
    while(angle >= 360.0) { angle -= 360.0; }
    while(angle < 0.0)    { angle += 360.0; }
    return angle;
}

double ArcObject::objectEndAngle() const
{
    double angle = QLineF(scenePos(), objectEndPoint()).angle();
    while(angle >= 360.0) { angle -= 360.0; }
    while(angle < 0.0)    { angle += 360.0; }
    return angle;
}

EmbVector ArcObject::objectStartPoint() const
{
    double rot = radians(rotation());
    double cosRot = cos(rot);
    double sinRot = sin(rot);
    double x = arcStartPoint.x()*scale();
    double y = arcStartPoint.y()*scale();
    double rotX = x*cosRot - y*sinRot;
    double rotY = x*sinRot + y*cosRot;

    return (scenePos() + EmbVector(rotX, rotY));
}

double ArcObject::objectStartX() const
{
    return objectStartPoint().x();
}

double ArcObject::objectStartY() const
{
    return objectStartPoint().y();
}

EmbVector ArcObject::objectMidPoint() const
{
    double rot = radians(rotation());
    double cosRot = cos(rot);
    double sinRot = sin(rot);
    double x = arcMidPoint.x()*scale();
    double y = arcMidPoint.y()*scale();
    double rotX = x*cosRot - y*sinRot;
    double rotY = x*sinRot + y*cosRot;

    return (scenePos() + EmbVector(rotX, rotY));
}

double ArcObject::objectMidX() const
{
    return objectMidPoint().x();
}

double ArcObject::objectMidY() const
{
    return objectMidPoint().y();
}

EmbVector ArcObject::objectEndPoint() const
{
    double rot = radians(rotation());
    double cosRot = cos(rot);
    double sinRot = sin(rot);
    double x = arcEndPoint.x()*scale();
    double y = arcEndPoint.y()*scale();
    double rotX = x*cosRot - y*sinRot;
    double rotY = x*sinRot + y*cosRot;

    return (scenePos() + EmbVector(rotX, rotY));
}

double ArcObject::objectEndX() const
{
    return objectEndPoint().x();
}

double ArcObject::objectEndY() const
{
    return objectEndPoint().y();
}

double ArcObject::objectArea() const
{
    //Area of a circular segment
    double r = objectRadius();
    double theta = radians(objectIncludedAngle());
    return ((r*r)/2)*(theta - sin(theta));
}

double ArcObject::objectArcLength() const
{
    return radians(objectIncludedAngle())*objectRadius();
}

double ArcObject::objectChord() const
{
    EmbVector start, end, delta;
    start.x = objectStartX();
    start.y = objectStartY();
    end.x = objectEndX();
    end.y = objectEndY();
    embVector_subtract(start, end, &delta);
    return embVector_length(delta);
}

double ArcObject::objectIncludedAngle() const
{
    double chord = objectChord();
    double rad = objectRadius();
    if (chord <= 0 || rad <= 0) return 0; //Prevents division by zero and non-existant circles

    //NOTE: Due to floating point rounding errors, we need to clamp the quotient so it is in the range [-1, 1]
    //      If the quotient is out of that range, then the result of asin() will be NaN.
    double quotient = chord/(2.0*rad);
    if (quotient > 1.0) quotient = 1.0;
    if (quotient < 0.0) quotient = 0.0; //NOTE: 0 rather than -1 since we are enforcing a positive chord and radius
    return degrees(2.0*asin(quotient)); //Properties of a Circle - Get the Included Angle - Reference: ASD9
}

bool ArcObject::objectClockwise() const
{
    // NOTE: Y values are inverted here on purpose
    EmbArc arc;
    arc.start.x = objectStartX();
    arc.start.y = -objectStartY();
    arc.mid.x = objectMidX();
    arc.mid.y = -objectMidY();
    arc.end.x = objectEndX();
    arc.end.y = -objectEndY();

    if (isArcClockwise(arc)) {
        return true;
    }
    return false;
}

void ArcObject::updatePath()
{
    double startAngle = (objectStartAngle() + rotation());
    double spanAngle = objectIncludedAngle();

    if (objectClockwise())
        spanAngle = -spanAngle;

    QPainterPath path;
    path.arcMoveTo(rect(), startAngle);
    path.arcTo(rect(), startAngle, spanAngle);
    //NOTE: Reverse the path so that the inside area isn't considered part of the arc
    path.arcTo(rect(), startAngle+spanAngle, -spanAngle);
    setObjectPath(path);
}

void ArcObject::paint(QPainter* painter, QStyleOptionGraphicsItem* option, QWidget* /*widget*/)
{
    QGraphicsScene* objScene = scene();
    if (!objScene) return;

    QPen paintPen = pen();
    painter->setPen(paintPen);
    updateRubber(painter);
    if (option->state & QStyle::State_Selected)  { paintPen.setStyle(DashLine); }
    if (objScene->property(ENABLE_LWT).toBool()) { paintPen = lineWeightPen(); }
    painter->setPen(paintPen);

    double startAngle = (objectStartAngle() + rotation())*16;
    double spanAngle = objectIncludedAngle()*16;

    if (objectClockwise())
        spanAngle = -spanAngle;

    double rad = objectRadius();
    QRectF paintRect(-rad, -rad, rad*2.0, rad*2.0);
    painter->drawArc(paintRect, startAngle, spanAngle);
}

void ArcObject::updateRubber(QPainter* painter)
{
    //TODO: Arc Rubber Modes

    //TODO: updateRubber() gripping for ArcObject

}

void ArcObject::vulcanize()
{
    debug_message("ArcObject vulcanize()");
    updateRubber();

    setObjectRubberMode(OBJ_RUBBER_OFF);
}

// Returns the closest snap point to the mouse point
EmbVector ArcObject::mouseSnapPoint(EmbVector& mousePoint)
{
    EmbVector center = objectCenter();
    EmbVector start  = objectStartPoint();
    EmbVector mid    = objectMidPoint();
    EmbVector end    = objectEndPoint();

    double cntrDist  = QLineF(mousePoint, center).length();
    double startDist = QLineF(mousePoint, start).length();
    double midDist   = QLineF(mousePoint, mid).length();
    double endDist   = QLineF(mousePoint, end).length();

    double minDist = std::min(std::min(cntrDist, startDist), std::min(midDist, endDist));

    if     (minDist == cntrDist)  return center;
    else if (minDist == startDist) return start;
    else if (minDist == midDist)   return mid;
    else if (minDist == endDist)   return end;

    return scenePos();
}

QList<EmbVector> ArcObject::allGripPoints()
{
    QList<EmbVector> gripPoints;
    gripPoints << objectCenter() << objectStartPoint() << objectMidPoint() << objectEndPoint();
    return gripPoints;
}

void ArcObject::gripEdit(EmbVector& before, EmbVector& after)
{
    //TODO: gripEdit() for ArcObject
}
#endif
