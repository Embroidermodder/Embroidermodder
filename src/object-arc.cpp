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
void ArcObject::init(EmbArc arc_in, unsigned int rgb, PenStyle lineType)
{
    setData(OBJ_TYPE, type);
    setData(OBJ_NAME, "Arc");

    arc = arc_in;

    setFlag(QGraphicsItem::ItemIsSelectable, true);

    calculateArcData(arc);

    setObjectColor(rgb);
    setObjectLineType(lineType);
    setObjectLineWeight(0.35); //TODO: pass in proper lineweight
    setPen(objPen);
}

void ArcObject::calculateArcData(EmbArc arc)
{
    EmbVector center;
    getArcCenter(arc, &center);

    arcStartPoint = embVector_subtract(arc.start, center);
    arcMidPoint = embVector_subtract(arc.mid, center);
    arcEndPoint = embVector_subtract(arc.end, center);

    setPos(center);

    double radius = EmbLine(center, arc.mid).length();
    updateArcRect(radius);
    updatePath();
    setRotation(0);
    setScale(1);
}

void ArcObject::updateArcRect(double radius)
{
    EmbRect arcRect;
    arcRect.setWidth(radius*2.0);
    arcRect.setHeight(radius*2.0);
    arcRect.moveCenter(EmbVector(0,0));
    setRect(arcRect);
}

void ArcObject::setObjectCenter(EmbVector point)
{
    setPos(point);
}

void ArcObject::setObjectRadius(double radius)
{
    double rad;
    if (radius <= 0) {
        rad = 0.0000001;
    }
    else {
        rad = radius;
    }

    EmbVector center = scenePos();
    EmbLine startLine = EmbLine(center, objectStartPoint());
    EmbLine midLine   = EmbLine(center, objectMidPoint());
    EmbLine endLine   = EmbLine(center, objectEndPoint());
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

void ArcObject::setObjectStartPoint(EmbVector point)
{
    arc.start = point;
    calculateArcData(arc);
}

void ArcObject::setObjectMidPoint(EmbVector point)
{
    arc.mid = point;
    calculateArcData(arc);
}

void ArcObject::setObjectEndPoint(EmbVector point)
{
    arc.end = point;
    calculateArcData(arc);
}

double ArcObject::objectStartAngle() const
{
    return std::fmodf(EmbLine(scenePos(), objectStartPoint()).angle(), 360.0);;
}

double ArcObject::objectEndAngle() const
{
    return std::fmodf(EmbLine(scenePos(), objectEndPoint()).angle(), 360.0);
}

EmbVector ArcObject::objectStartPoint() const
{
    double alpha = radians(rotation());
    EmbVector position = embVector_scale(arc.start, scale());
    EmbVector rot = embVector_rotate(postion, alpha);

    return scenePos() + rot;
}

EmbVector ArcObject::objectMidPoint() const
{
    double alpha = radians(rotation());
    EmbVector position = embVector_scale(arc.mid, scale());
    EmbVector rot = embVector_rotate(postion, alpha);

    return scenePos() + rot;
}

EmbVector ArcObject::objectEndPoint() const
{
    double alpha = radians(rotation());
    EmbVector position = embVector_scale(arc.end, scale());
    EmbVector rot = embVector_rotate(postion, alpha);

    return scenePos() + rot;
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
    return embVector_distance(arc.start, arc.end);
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
    EmbArc arc2 = arc;
    arc2.start.y *= -1.0;
    arc2.mid.y *= -1.0;
    arc2.end.y *= -1.0;

    return embArc_clockwise(arc2);
}

void ArcObject::updatePath()
{
    double startAngle = (objectStartAngle() + rotation());
    double spanAngle = objectIncludedAngle();

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
    EmbRect paintRect(-rad, -rad, rad*2.0, rad*2.0);
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

    double cntrDist  = EmbLine(mousePoint, center).length();
    double startDist = EmbLine(mousePoint, start).length();
    double midDist   = EmbLine(mousePoint, mid).length();
    double endDist   = EmbLine(mousePoint, end).length();

    double minDist = std::min(std::min(cntrDist, startDist), std::min(midDist, endDist));

    if     (minDist == cntrDist)  return center;
    else if (minDist == startDist) return start;
    else if (minDist == midDist)   return mid;
    else if (minDist == endDist)   return end;

    return scenePos();
}

std::vector<EmbVector> ArcObject::allGripPoints()
{
    std::vector<EmbVector> gripPoints;
    gripPoints << objectCenter() << objectStartPoint() << objectMidPoint() << objectEndPoint();
    return gripPoints;
}

void ArcObject::gripEdit(EmbVector& before, EmbVector& after)
{
    //TODO: gripEdit() for ArcObject
}
#endif
