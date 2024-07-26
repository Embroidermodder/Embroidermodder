/*
 * Embroidermodder 2.
 *
 * Copyright 2011-2024 The Embroidermodder Team
 * Embroidermodder 2 is Open Source Software, see LICENSE.md for licensing terms.
 *
 * Download the docs at https://www.libembroidery.org/downloads/emrm_current.pdf
 * for advice on altering this file.
 *
 * Object: attempting to combine the geometry objects into one that changes
 * behaviour depending on its mode.
 */

#include "embroidermodder.h"

Object::Object(EmbArc arc, QRgb rgb, QGraphicsItem *item)
{
    TYPE = OBJ_TYPE_ARC;
    curved = 0;
    geometry->object.arc = arc;
}

Object::Object(EmbCircle circle, QRgb rgb, QGraphicsItem *item)
{
    TYPE = OBJ_TYPE_CIRCLE;
    geometry->object.circle = circle;
}

Object::Object(EmbEllipse ellipse, QRgb rgb, QGraphicsItem *item)
{
    TYPE = OBJ_TYPE_ELLIPSE;
    geometry->object.ellipse = ellipse;
}

/*
Object::Object(Object *obj)
{
    switch (TYPE) {
    case OBJ_TYPE_LINE:
        break;
    default:
        break;
    }
}
*/

Object::~Object()
{
    switch (TYPE) {
    case OBJ_TYPE_LINE:
        break;
    default:
        break;
    }
}

/*
QPointF
Object::objectEndPoint1() const
{
    return QPointF(0.0f, 0.0f);
}

QPointF
Object::objectEndPoint2() const
{
    return QPointF(0.0f, 0.0f);
}
*/

QPointF
Object::objectStartPoint() const
{
    return QPointF(0.0f, 0.0f);
}

QPointF
Object::objectMidPoint() const
{
    return QPointF(0.0f, 0.0f);
}

QPointF
Object::objectEndPoint() const
{
    return QPointF(0.0f, 0.0f);
}

QPointF
Object::objectQuadrant0()
{
    switch (geometry->type) {
    case OBJ_TYPE_CIRCLE:
        return objectCenter() + QPointF(objectRadius(), 0);
    default:
        break;
    }
    return objectCenter();
}

QPointF
Object::objectQuadrant90()
{
    switch (geometry->type) {
    case OBJ_TYPE_CIRCLE:
        return objectCenter() + QPointF(0, -objectRadius());
    default:
        break;
    }
    return objectCenter();
}

QPointF
Object::objectQuadrant180()
{
    switch (geometry->type) {
    case EMB_CIRCLE:
        return objectCenter() + QPointF(-objectRadius(),0);
    default:
        break;
    }
    return objectCenter();
}

QPointF
Object::objectQuadrant270()
{
    switch (geometry->type) {
    case EMB_CIRCLE:
        return objectCenter() + QPointF(0, objectRadius());
    default:
        break;
    }
    return objectCenter();
}

QPointF
Object::objectTopLeft() const
{
    return QPointF(0.0f, 0.0f);
}

QPointF
Object::objectTopRight() const
{
    return QPointF(0.0f, 0.0f);
}

QPointF
Object::objectBottomLeft() const
{
    return QPointF(0.0f, 0.0f);
}

QPointF
Object::objectBottomRight() const
{
    return QPointF(0.0f, 0.0f);
}

/*
QPointF
Object::objectCenter()
{
    return QPointF(0.0f, 0.0f);
}

double
Object::objectWidth()
{
    return 0.0f;
}

double
Object::objectHeight()
{
    return 0.0f;
}

Object::Object(QString const&, double, double, unsigned int, QGraphicsItem*)
{
}

Object::setObjectTextFont(QString const&)
{
}

Object::setObjectTextSize(double)
{
}

Object::setObjectTextStyle(bool, bool, bool, bool, bool)
{
}

Object::setObjectTextBackward(bool)
{
}

Object::setObjectTextUpsideDown(bool)
{
}

Object::Object(double, double, double, double, unsigned int, QGraphicsItem*)
{
}

Object::Object(double, double, double, double, unsigned int, QGraphicsItem*)'
{
}

Object::Object(EmbPoint_, unsigned int, QGraphicsItem*)'
{
}

Object::Object(EmbPath_, int, QPainterPath const&, unsigned int, QGraphicsItem*)'
{
}

Object::Object(EmbPath_, int, QPainterPath const&, unsigned int, QGraphicsItem*)'
{
}

Object::objectStartAngle() const
{
}

Object::objectIncludedAngle() const
{
}

Object::objectClockwise() const
{
}

Object::realRender(QPainter*, QPainterPath const&)
{
}

Object::gripEdit(QPointF const&, QPointF const&)
{
}

Object::updatePath(QPainterPath const&)
{
}
*/

QPointF
Object::objectEndPoint1()
{
    return QPointF(0.0, 0.0);
}

QPointF
Object::objectEndPoint2()
{
    return QPointF(0.0, 0.0);
}

void
Object::setObjectEndPoint1(QPointF const&)
{
}

void
Object::setObjectEndPoint2(QPointF const&)
{
}

/*
Object::boundingRect() const
{
}

Object::objectTextJustifyList() const
{
}

Object::objectEndPoint2() const
{
}
*/

double
Object::objectAngle() const
{
    return 0.0;
}

double
Object::objectStartAngle() const
{
    return 0.0;
}

double
Object::objectEndAngle() const
{
    return 0.0;
}

double
Object::objectArcLength() const
{
    return 0.0;
}

Object::Object(QString const&, double, double, unsigned int, QGraphicsItem*)
{
}

Object::Object(double, double, double, double, unsigned int, QGraphicsItem*)
{
}

Object::Object(EmbPoint_, unsigned int, QGraphicsItem*)
{
}

Object::Object(EmbPath_, int, QPainterPath const&, unsigned int, QGraphicsItem*)
{
}

void
Object::init(EmbArc_, unsigned int, Qt::PenStyle)
{
}

void
Object::realRender(QPainter*, QPainterPath const&)
{

}

void
Object::updatePath(QPainterPath const&)
{
}

QRectF
Object::boundingRect() const
{
    return QRectF(1.0, 1.0, 1.0, 1.0);
}

int
Object::findIndex(const QPointF& position)
{
    return 0;
}

void
Object::updateRubber(QPainter* painter)
{
    switch (geometry->type) {
    case EMB_ARC:
        //TODO: Arc Rubber Modes

        //TODO: updateRubber() gripping for ArcObject
        break;
    default:
        break;
    }
}

/*
void
Object::updatePath(QPainterPath path)
{
}
*/

void
Object::setObjectRect(double x1, double y1, double x2, double y2)
{
}

/*
void
Object::setObjectEndPoint1(QPointF point)
{
}

void
Object::setObjectEndPoint2(QPointF point)
{
}
*/

void
Object::vulcanize()
{
    qDebug("vulcanize()");
    // FIXME: updateRubber(painter);

    setObjectRubberMode(OBJ_RUBBER_OFF);

    switch (geometry->type) {
    case EMB_POLYLINE:
        if (!normalPath.elementCount()) {
            QMessageBox::critical(0,
                QObject::tr("Empty Polyline Error"),
                QObject::tr("The polyline added contains no points. The command that created this object has flawed logic."));
        }
        break;
    case EMB_POLYGON:
        if (!normalPath.elementCount()) {
            QMessageBox::critical(0,
                QObject::tr("Empty Polygon Error"),
                QObject::tr("The polygon added contains no points. The command that created this object has flawed logic."));
        }
        break;
    case EMB_PATH:
        if (!normalPath.elementCount()) {
            QMessageBox::critical(0,
                QObject::tr("Empty Path Error"),
                QObject::tr("The path added contains no points. The command that created this object has flawed logic."));
        }
        break;
    default:
        break;
    }
}

QList<QPointF>
Object::allGripPoints()
{
    QList<QPointF> gripPoints;
    switch (geometry->type) {
    case EMB_ARC: {
        gripPoints << objectCenter()
            << objectStartPoint()
            << objectMidPoint()
            << objectEndPoint();
        break;
    }
    case EMB_CIRCLE:
    case EMB_ELLIPSE: {
        gripPoints << objectCenter()
            << objectQuadrant0()
            << objectQuadrant90()
            << objectQuadrant180()
            << objectQuadrant270();
        break;
    }
    case EMB_DIM_LEADER: {
        gripPoints << objectEndPoint1() << objectEndPoint2();
        if (curved) {
            gripPoints << objectMidPoint();
        }
        break;
    }
    case EMB_IMAGE: {
        gripPoints << objectTopLeft()
             << objectTopRight()
             << objectBottomLeft()
             << objectBottomRight();
        break;
    }
    case EMB_LINE: {
        gripPoints << objectEndPoint1() << objectEndPoint2() << objectMidPoint();
        break;
    }
    case EMB_PATH: {
        gripPoints << scenePos(); //TODO: loop thru all path Elements and return their points
        break;
    }
    case EMB_POLYGON:
    case EMB_POLYLINE: {
        QPainterPath::Element element;
        for (int i = 0; i < normalPath.elementCount(); ++i) {
            element = normalPath.elementAt(i);
            gripPoints << mapToScene(element.x, element.y);
        }
        break;
    }
    case EMB_TEXT_SINGLE:
    case EMB_POINT:
    default:
        gripPoints << scenePos();
        break;
    }
    return gripPoints;
}

/* Returns the closest snap point to the mouse point */
QPointF
Object::mouseSnapPoint(const QPointF& mousePoint)
{
    return find_mouse_snap_point(allGripPoints(), mousePoint);
}

void
Object::gripEdit(const QPointF& before, const QPointF& after)
{
    switch (geometry->type) {
    case EMB_ARC: {
        //TODO: gripEdit() for ArcObject
        break;
    }
    case EMB_CIRCLE: {
        if (before == objectCenter()) {
            QPointF delta = after-before;
            moveBy(delta.x(), delta.y());
        }
        else {
            setObjectRadius(QLineF(objectCenter(), after).length());
        }
        break;
    }
    case EMB_DIM_LEADER:
    case EMB_LINE: {
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
        break;
    }
    case EMB_ELLIPSE: {
        //TODO: gripEdit() for EllipseObject
        break;
    }
    case EMB_IMAGE:
    case EMB_RECT: {
        QPointF delta = after-before;
        if (before == objectTopLeft()) {
            setObjectRect(after.x(), after.y(), objectWidth()-delta.x(),
                objectHeight()-delta.y());
        }
        else if (before == objectTopRight()) {
            setObjectRect(objectTopLeft().x(), objectTopLeft().y()+delta.y(),
                objectWidth()+delta.x(), objectHeight()-delta.y());
        }
        else if (before == objectBottomLeft()) {
            setObjectRect(objectTopLeft().x()+delta.x(), objectTopLeft().y(),
                objectWidth()-delta.x(), objectHeight()+delta.y());
        }
        else if (before == objectBottomRight()) {
            setObjectRect(objectTopLeft().x(), objectTopLeft().y(),
                objectWidth()+delta.x(), objectHeight()+delta.y());
        }
        break;
    }
    case EMB_PATH: {
        //TODO: gripEdit() for PathObject
        break;
    }
    case EMB_POLYGON:
    case EMB_POLYLINE: {
        gripIndex = findIndex(before);
        if (gripIndex == -1) {
            return;
        }
        QPointF a = mapFromScene(after);
        normalPath.setElementPositionAt(gripIndex, a.x(), a.y());
        updatePath(normalPath);
        gripIndex = -1;
        break;
    }
    case EMB_TEXT_SINGLE:
    case EMB_POINT:
    default: {
        if (before == scenePos()) {
            QPointF delta = after-before;
            moveBy(delta.x(), delta.y());
        }
        break;
    }
    }
}

void
Object::paint(QPainter* painter, const QStyleOptionGraphicsItem *option, QWidget *)
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

    switch (geometry->type) {
    case EMB_ARC: {
        double startAngle = (objectStartAngle() + rotation())*16;
        double spanAngle = objectIncludedAngle()*16;

        if (objectClockwise()) {
            spanAngle = -spanAngle;
        }

        double rad = objectRadius();
        QRectF paintRect(-rad, -rad, rad*2.0, rad*2.0);
        painter->drawArc(paintRect, startAngle, spanAngle);
        break;
    }
    case EMB_CIRCLE:
    case EMB_ELLIPSE: {
        painter->drawEllipse(rect());
        break;
    }
    case EMB_DIM_LEADER: {
        painter->drawPath(lineStylePath);
        painter->drawPath(arrowStylePath);

        if (filled) {
            painter->fillPath(arrowStylePath, objPen.color());
        }
        break;
    }
    case EMB_LINE: {
        if (objectRubberMode() != OBJ_RUBBER_LINE) {
            painter->drawLine(line());
        }

        if (objScene->property("ENABLE_LWT").toBool()
            && objScene->property("ENABLE_REAL").toBool()) {
                realRender(painter, path());
        }
        break;
    }
    case EMB_IMAGE: {
        painter->drawRect(rect());
        break;
    }
    case EMB_RECT: {
        painter->drawRect(rect());
        break;
    }
    case EMB_PATH: {
        painter->drawPath(objectPath());
        break;
    }
    case EMB_POLYGON: {
        if (normalPath.elementCount()) {
            painter->drawPath(normalPath);
            QPainterPath::Element zero = normalPath.elementAt(0);
            QPainterPath::Element last = normalPath.elementAt(normalPath.elementCount()-1);
            painter->drawLine(QPointF(zero.x, zero.y), QPointF(last.x, last.y));
        }
        break;
    }
    case EMB_POLYLINE: {
        painter->drawPath(normalPath);

        if (objScene->property("ENABLE_LWT").toBool()
            && objScene->property("ENABLE_REAL").toBool()) {
            realRender(painter, normalPath);
        }
        break;
    }
    case EMB_TEXT_SINGLE: {
        painter->drawPath(objTextPath);
        break;
    }
    default:
    case EMB_POINT: {
        painter->drawPoint(0,0);
        break;
    }
    }
}

