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
    //Type = OBJ_TYPE_ARC;
    curved = 0;
}

/*
Object::Object(Object *obj)
{
    switch (type()) {
    case OBJ_TYPE_LINE:
        break;
    default:
        break;
    }
}
*/

Object::~Object()
{
    switch (type()) {
    case OBJ_TYPE_LINE:
        break;
    default:
        break;
    }
}

QPointF
Object::objectEndPoint1()
{
    return QPointF(0.0f, 0.0f);
}

QPointF
Object::objectEndPoint2()
{
    return QPointF(0.0f, 0.0f);
}

QPointF
Object::objectStartPoint()
{
    return QPointF(0.0f, 0.0f);
}

QPointF
Object::objectMidPoint()
{
    return QPointF(0.0f, 0.0f);
}

QPointF
Object::objectEndPoint()
{
    return QPointF(0.0f, 0.0f);
}

QPointF
Object::objectQuadrant0()
{
    return QPointF(0.0f, 0.0f);
}

QPointF
Object::objectQuadrant90()
{
    return QPointF(0.0f, 0.0f);
}

QPointF
Object::objectQuadrant180()
{
    return QPointF(0.0f, 0.0f);
}

QPointF
Object::objectQuadrant270()
{
    return QPointF(0.0f, 0.0f);
}

QPointF
Object::objectTopLeft()
{
    return QPointF(0.0f, 0.0f);
}

QPointF
Object::objectTopRight()
{
    return QPointF(0.0f, 0.0f);
}

QPointF
Object::objectBottomLeft()
{
    return QPointF(0.0f, 0.0f);
}

QPointF
Object::objectBottomRight()
{
    return QPointF(0.0f, 0.0f);
}

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

int
Object::findIndex(QPointF position)
{
    return 0;
}

void
Object::setObjectRubberMode(int mode)
{
}

void
Object::updateRubber(void)
{
}

void
Object::updatePath(QPainterPath path)
{
}

void
Object::setObjectRect(double x1, double y1, double x2, double y2)
{
}

void
Object::setObjectRadius(double radius)
{
}

void
Object::setObjectEndPoint1(QPointF point)
{
}

void
Object::setObjectEndPoint2(QPointF point)
{
}

void
Object::vulcanize()
{
    qDebug("vulcanize()");
    updateRubber();

    setObjectRubberMode(OBJ_RUBBER_OFF);

    switch (type()) {
    case OBJ_TYPE_POLYLINE:
        if (!normalPath.elementCount()) {
            QMessageBox::critical(0,
                QObject::tr("Empty Polyline Error"),
                QObject::tr("The polyline added contains no points. The command that created this object has flawed logic."));
        }
        break;
    case OBJ_TYPE_POLYGON:
        if (!normalPath.elementCount()) {
            QMessageBox::critical(0,
                QObject::tr("Empty Polygon Error"),
                QObject::tr("The polygon added contains no points. The command that created this object has flawed logic."));
        }
        break;
    case OBJ_TYPE_PATH:
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
    switch (type()) {
    case OBJ_TYPE_ARC: {
        gripPoints << objectCenter()
            << objectStartPoint()
            << objectMidPoint()
            << objectEndPoint();
        break;
    }
    case OBJ_TYPE_CIRCLE:
    case OBJ_TYPE_ELLIPSE: {
        gripPoints << objectCenter()
            << objectQuadrant0()
            << objectQuadrant90()
            << objectQuadrant180()
            << objectQuadrant270();
        break;
    }
    case OBJ_TYPE_DIMLEADER: {
        gripPoints << objectEndPoint1() << objectEndPoint2();
        if (curved) {
            gripPoints << objectMidPoint();
        }
        break;
    }
    case OBJ_TYPE_IMAGE: {
        gripPoints << objectTopLeft()
             << objectTopRight()
             << objectBottomLeft()
             << objectBottomRight();
        break;
    }
    case OBJ_TYPE_LINE: {
        gripPoints << objectEndPoint1() << objectEndPoint2() << objectMidPoint();
        break;
    }
    case OBJ_TYPE_PATH: {
        gripPoints << scenePos(); //TODO: loop thru all path Elements and return their points
        break;
    }
    case OBJ_TYPE_POLYGON:
    case OBJ_TYPE_POLYLINE: {
        QPainterPath::Element element;
        for (int i = 0; i < normalPath.elementCount(); ++i) {
            element = normalPath.elementAt(i);
            gripPoints << mapToScene(element.x, element.y);
        }
        break;
    }
    case OBJ_TYPE_TEXTSINGLE:
    case OBJ_TYPE_POINT:
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
    switch (type()) {
    case OBJ_TYPE_ARC: {
        //TODO: gripEdit() for ArcObject
        break;
    }
    case OBJ_TYPE_CIRCLE: {
        if (before == objectCenter()) {
            QPointF delta = after-before;
            moveBy(delta.x(), delta.y());
        }
        else {
            setObjectRadius(QLineF(objectCenter(), after).length());
        }
        break;
    }
    case OBJ_TYPE_DIMLEADER:
    case OBJ_TYPE_LINE: {
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
    case OBJ_TYPE_ELLIPSE: {
        //TODO: gripEdit() for EllipseObject
        break;
    }
    case OBJ_TYPE_IMAGE:
    case OBJ_TYPE_RECTANGLE: {
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
    case OBJ_TYPE_PATH: {
        //TODO: gripEdit() for PathObject
        break;
    }
    case OBJ_TYPE_POLYGON:
    case OBJ_TYPE_POLYLINE: {
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
    case OBJ_TYPE_TEXTSINGLE:
    case OBJ_TYPE_POINT:
    default: {
        if (before == scenePos()) {
            QPointF delta = after-before;
            moveBy(delta.x(), delta.y());
        }
        break;
    }
    }
}

