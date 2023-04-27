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
 * \file object-rect.cpp
 */

#include "embroidermodder.h"

PointObject::PointObject(EmbReal x, EmbReal y, QRgb rgb, QGraphicsItem* parent) : BaseObject(parent)
{
    qDebug("PointObject Constructor()");
    init(x, y, rgb, Qt::SolidLine); //TODO: getCurrentLineType
}

PointObject::PointObject(PointObject* obj, QGraphicsItem* parent) : BaseObject(parent)
{
    qDebug("PointObject Constructor()");
    if(obj)
    {
        init(obj->objectX(), obj->objectY(), obj->objectColorRGB(), Qt::SolidLine); //TODO: getCurrentLineType
        setRotation(obj->rotation());
    }
}

PointObject::~PointObject()
{
    qDebug("PointObject Destructor()");
}

void PointObject::init(EmbReal x, EmbReal y, QRgb rgb, Qt::PenStyle lineType)
{
    setData(OBJ_TYPE, OBJ_TYPE_POINT);
    setData(OBJ_NAME, "Point");

    //WARNING: DO NOT enable QGraphicsItem::ItemIsMovable. If it is enabled,
    //WARNING: and the item is double clicked, the scene will erratically move the item while zooming.
    //WARNING: All movement has to be handled explicitly by us, not by the scene.
    setFlag(QGraphicsItem::ItemIsSelectable, true);

    setRect(-0.00000001, -0.00000001, 0.00000002, 0.00000002);
    setObjectPos(x,y);
    setObjectColor(rgb);
    setObjectLineType(lineType);
    setObjectLineWeight(0.35); //TODO: pass in proper lineweight
    setPen(objectPen());
}

void PointObject::paint(QPainter* painter, const QStyleOptionGraphicsItem* option, QWidget* /*widget*/)
{
    QGraphicsScene* objScene = scene();
    if(!objScene) return;

    QPen paintPen = pen();
    painter->setPen(paintPen);
    updateRubber(painter);
    if(option->state & QStyle::State_Selected)  { paintPen.setStyle(Qt::DashLine); }
    if(objScene->property("ENABLE_LWT").toBool()) { paintPen = lineWeightPen(); }
    painter->setPen(paintPen);

    painter->drawPoint(0,0);
}

void PointObject::updateRubber(QPainter* painter)
{
    int rubberMode = objectRubberMode();
    if(rubberMode == OBJ_RUBBER_GRIP)
    {
        if(painter)
        {
            QPointF gripPoint = objectRubberPoint("GRIP_POINT");
            if(gripPoint == scenePos())
            {
                QLineF rubLine(mapFromScene(gripPoint), mapFromScene(objectRubberPoint(QString())));
                drawRubberLine(rubLine, painter, "VIEW_COLOR_CROSSHAIR");
            }
        }
    }
}

void PointObject::vulcanize()
{
    qDebug("PointObject vulcanize()");
    updateRubber();

    setObjectRubberMode(OBJ_RUBBER_OFF);
}

// Returns the closest snap point to the mouse point
QPointF PointObject::mouseSnapPoint(const QPointF& mousePoint)
{
    return scenePos();
}

QList<QPointF> PointObject::allGripPoints()
{
    QList<QPointF> gripPoints;
    gripPoints << scenePos();
    return gripPoints;
}

void PointObject::gripEdit(const QPointF& before, const QPointF& after)
{
    if(before == scenePos()) { QPointF delta = after-before; moveBy(delta.x(), delta.y()); }
}

QPainterPath PointObject::objectSavePath() const
{
    QPainterPath path;
    path.addRect(-0.00000001, -0.00000001, 0.00000002, 0.00000002);
    return path;
}
