/*
 * Embroidermodder 2.
 *
 * Copyright 2011-2024 The Embroidermodder Team
 * Embroidermodder 2 is Open Source Software, see LICENSE.md for licensing terms.
 * Visit https://www.libembroidery.org/refman for advice on altering this file,
 * or read the markdown version in embroidermodder2/docs/refman.
 *
 * Arc
 */

#include "embroidermodder.h"

#include <QPainter>
#include <QStyleOption>
#include <QGraphicsScene>

/* . */
ScriptValue
arc_command(ScriptEnv *context)
{
    init_command();
    clear_selection();

    end_command();
    return script_null;
}

ArcObject::ArcObject(EmbArc arc, QRgb rgb, QGraphicsItem* parent) : BaseObject(parent)
{
    qDebug("ArcObject Constructor()");
    init(arc, rgb, Qt::SolidLine); //TODO: getCurrentLineType
}

ArcObject::ArcObject(ArcObject* obj, QGraphicsItem* parent) : BaseObject(parent)
{
    qDebug("ArcObject Constructor()");
    if (obj) {
        init(arc, obj->objectColorRGB(), Qt::SolidLine); //TODO: getCurrentLineType
        setRotation(obj->rotation());
    }
}

ArcObject::~ArcObject()
{
    qDebug("ArcObject Destructor()");
}

void ArcObject::init(EmbArc arc, QRgb rgb, Qt::PenStyle lineType)
{
    setData(OBJ_TYPE, type());
    setData(OBJ_NAME, OBJ_NAME_ARC);

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

void ArcObject::calculateArcData(EmbArc arc)
{
    EmbVector center = emb_arc_center(arc);

    arcStartPoint = to_qpointf(emb_vector_subtract(arc.start, center));
    arcMidPoint = to_qpointf(emb_vector_subtract(arc.mid, center));
    arcEndPoint = to_qpointf(emb_vector_subtract(arc.end, center));

    setPos(center.x, center.y);

    qreal radius = emb_vector_distance(center, arc.mid);
    updateArcRect(radius);
    updatePath();
    setRotation(0);
    setScale(1);
}

void ArcObject::updateArcRect(qreal radius)
{
    QRectF arcRect;
    arcRect.setWidth(radius*2.0);
    arcRect.setHeight(radius*2.0);
    arcRect.moveCenter(QPointF(0,0));
    setRect(arcRect);
}

void ArcObject::setObjectCenter(const QPointF& point)
{
    setObjectCenter(point.x(), point.y());
}

void ArcObject::setObjectCenter(qreal pointX, qreal pointY)
{
    setPos(pointX, pointY);
}

void ArcObject::setObjectRadius(qreal radius)
{
    qreal rad = EMB_MAX(radius, 0.0000001);

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
    arc.start = to_emb_vector(arcStartPoint);
    arc.mid = to_emb_vector(arcMidPoint);
    arc.end = to_emb_vector(arcEndPoint);

    calculateArcData(arc);
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
    arc.start.x = pointX;
    arc.start.y = pointY;
    arc.mid = to_emb_vector(arcMidPoint);
    arc.end = to_emb_vector(arcEndPoint);
    calculateArcData(arc);
}

void ArcObject::setObjectMidPoint(const QPointF& point)
{
    setObjectMidPoint(point.x(), point.y());
}

void ArcObject::setObjectMidPoint(qreal pointX, qreal pointY)
{
    arc.start = to_emb_vector(arcStartPoint);
    arc.mid.x = pointX;
    arc.mid.y = pointY;
    arc.end = to_emb_vector(arcEndPoint);
    calculateArcData(arc);
}

void ArcObject::setObjectEndPoint(const QPointF& point)
{
    setObjectEndPoint(point.x(), point.y());
}

void ArcObject::setObjectEndPoint(qreal pointX, qreal pointY)
{
    arc.start = to_emb_vector(arcStartPoint);
    arc.mid = to_emb_vector(arcMidPoint);
    arc.end.x = pointX;
    arc.end.y = pointY;
    calculateArcData(arc);
}

qreal ArcObject::objectStartAngle() const
{
    qreal angle = QLineF(scenePos(), objectStartPoint()).angle();
    return fmod(angle+360.0, 360.0);
}

qreal ArcObject::objectEndAngle() const
{
    qreal angle = QLineF(scenePos(), objectEndPoint()).angle();
    return fmod(angle+360.0, 360.0);
}

QPointF ArcObject::objectStartPoint() const
{
    return scenePos() + scale_and_rotate(arcStartPoint, scale(), rotation());
}

QPointF ArcObject::objectMidPoint() const
{
    return scenePos() + scale_and_rotate(arcMidPoint, scale(), rotation());
}

QPointF ArcObject::objectEndPoint() const
{
    return scenePos() + scale_and_rotate(arcEndPoint, scale(), rotation());
}

qreal ArcObject::objectArea() const
{
    //Area of a circular segment
    qreal r = objectRadius();
    qreal theta = radians(objectIncludedAngle());
    return ((r*r)/2)*(theta - sin(theta));
}

qreal ArcObject::objectArcLength() const
{
    return radians(objectIncludedAngle()) * objectRadius();
}

qreal ArcObject::objectChord() const
{
    return QLineF(objectStartPoint(), objectEndPoint()).length();
}

qreal ArcObject::objectIncludedAngle() const
{
    qreal chord = objectChord();
    qreal rad = objectRadius();
    if (chord <= 0 || rad <= 0) return 0; //Prevents division by zero and non-existant circles

    //NOTE: Due to floating point rounding errors, we need to clamp the quotient so it is in the range [-1, 1]
    //      If the quotient is out of that range, then the result of asin() will be NaN.
    qreal quotient = chord/(2.0*rad);
    quotient = EMB_MIN(1.0, quotient);
    quotient = EMB_MAX(0.0, quotient); //NOTE: 0 rather than -1 since we are enforcing a positive chord and radius
    return degrees(2.0*asin(quotient)); //Properties of a Circle - Get the Included Angle - Reference: ASD9
}

bool ArcObject::objectClockwise() const
{
    //NOTE: Y values are inverted here on purpose
    EmbArc arc;
    arc.start = to_emb_vector(objectStartPoint());
    arc.start.y = -arc.start.y;
    arc.mid = to_emb_vector(objectMidPoint());
    arc.mid.y = -arc.start.y;
    arc.end = to_emb_vector(objectEndPoint());
    arc.end.y = -arc.end.y;
    if (emb_arc_clockwise(arc)) {
        return true;
    }
    return false;
}

void ArcObject::updatePath()
{
    qreal startAngle = (objectStartAngle() + rotation());
    qreal spanAngle = objectIncludedAngle();

    if (objectClockwise())
        spanAngle = -spanAngle;

    QPainterPath path;
    path.arcMoveTo(rect(), startAngle);
    path.arcTo(rect(), startAngle, spanAngle);
    //NOTE: Reverse the path so that the inside area isn't considered part of the arc
    path.arcTo(rect(), startAngle+spanAngle, -spanAngle);
    setObjectPath(path);
}

void ArcObject::paint(QPainter* painter, const QStyleOptionGraphicsItem* option, QWidget* /*widget*/)
{
    QGraphicsScene* objScene = scene();
    if (!objScene) return;

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

    qreal startAngle = (objectStartAngle() + rotation())*16;
    qreal spanAngle = objectIncludedAngle()*16;

    if (objectClockwise())
        spanAngle = -spanAngle;

    qreal rad = objectRadius();
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
    qDebug("ArcObject vulcanize()");
    updateRubber();

    setObjectRubberMode(OBJ_RUBBER_OFF);
}

// Returns the closest snap point to the mouse point
QPointF ArcObject::mouseSnapPoint(const QPointF& mousePoint)
{
    return find_mouse_snap_point(allGripPoints(), mousePoint);
}

QList<QPointF> ArcObject::allGripPoints()
{
    QList<QPointF> gripPoints;
    gripPoints << objectCenter() << objectStartPoint() << objectMidPoint() << objectEndPoint();
    return gripPoints;
}

void ArcObject::gripEdit(const QPointF& before, const QPointF& after)
{
    //TODO: gripEdit() for ArcObject
}
