/*
 * Embroidermodder 2.
 *
 * Copyright 2011-2024 The Embroidermodder Team
 * Embroidermodder 2 is Open Source Software, see LICENSE.md for licensing terms.
 * Visit https://www.libembroidery.org/refman for advice on altering this file,
 * or read the markdown version in embroidermodder2/docs/refman.
 *
 * View Commands
 */

#include "embroidermodder.h"

#include <QPainter>
#include <QStyleOption>
#include <QGraphicsScene>

/* POINT */
ScriptValue
point_command(ScriptEnv * context)
{
    return script_null;
}

#if 0
function main()
{
    init_command();
    clear_selection();
    global->firstRun = true;
    prompt_output("TODO: Current point settings: PDMODE=?  PDSIZE=?"); //TODO: translate needed here when complete
    prompt_output(translate("Specify first point: "));
}

void
click(EmbVector v)
{
    if (global.firstRun) {
        global.firstRun = false;
        prompt_output(translate("Specify next point: "));
        addPoint(x,y);
    }
    else {
        addPoint(x,y);
    }
}

function context(str)
{
    todo("POINT", "context()");
}

function prompt(str)
{
    EmbVector v;
    if (global->firstRun) {
        if (str == "M" || str == "MODE") {
            //TODO: Probably should add additional translate calls here.
            todo("POINT", "prompt() for PDMODE");
        }
        else if (str == "S" || str == "SIZE") {
            //TODO: Probably should add additional translate calls here.
            todo("POINT", "prompt() for PDSIZE");
        }
        if (!parse_vector(str, v)) {
            alert(translate("Invalid point."));
            prompt_output(translate("Specify first point: "));
        }
        else {
            global.firstRun = false;
            prompt_output(translate("Specify next point: "));
            addPoint(v.x, v.y);
        }
    }
    else {
        if (!parse_vector(str, v)) {
            alert(translate("Invalid point."));
            prompt_output(translate("Specify next point: "));
        }
        else {
            prompt_output(translate("Specify next point: "));
            addPoint(v.x, v.y);
        }
    }
}
#endif


PointObject::PointObject(qreal x, qreal y, QRgb rgb, QGraphicsItem* parent) : BaseObject(parent)
{
    qDebug("PointObject Constructor()");
    init(x, y, rgb, Qt::SolidLine); //TODO: getCurrentLineType
}

PointObject::PointObject(PointObject* obj, QGraphicsItem* parent) : BaseObject(parent)
{
    qDebug("PointObject Constructor()");
    if (obj) {
        init(obj->objectX(), obj->objectY(), obj->objectColorRGB(), Qt::SolidLine); //TODO: getCurrentLineType
        setRotation(obj->rotation());
    }
}

PointObject::~PointObject()
{
    qDebug("PointObject Destructor()");
}

void PointObject::init(qreal x, qreal y, QRgb rgb, Qt::PenStyle lineType)
{
    setData(OBJ_TYPE, type());
    setData(OBJ_NAME, OBJ_NAME_POINT);

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

    painter->drawPoint(0,0);
}

void PointObject::updateRubber(QPainter* painter)
{
    int rubberMode = objectRubberMode();
    if (rubberMode == OBJ_RUBBER_GRIP) {
        if (painter) {
            QPointF gripPoint = objectRubberPoint("GRIP_POINT");
            if (gripPoint == scenePos()) {
                QLineF rubLine(mapFromScene(gripPoint), mapFromScene(objectRubberPoint(QString())));
                drawRubberLine(rubLine, painter, VIEW_COLOR_CROSSHAIR);
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
    if (before == scenePos()) {
        QPointF delta = after-before; moveBy(delta.x(), delta.y());
    }
}

QPainterPath PointObject::objectSavePath() const
{
    QPainterPath path;
    path.addRect(-0.00000001, -0.00000001, 0.00000002, 0.00000002);
    return path;
}
