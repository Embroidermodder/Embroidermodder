/*
 * Embroidermodder 2.
 *
 * Copyright 2011-2024 The Embroidermodder Team
 * Embroidermodder 2 is Open Source Software, see LICENSE.md for licensing terms.
 * Visit https://www.libembroidery.org/refman for advice on altering this file,
 * or read the markdown version in embroidermodder2/docs/refman.
 *
 * Object Circle
 */

#include "object-circle.h"
#include "embroidermodder.h"
#include "view.h"

#include <QPainter>
#include <QStyleOption>
#include <QGraphicsScene>

#define MODE_1P_RAD    0
#define MODE_1P_DIA    1
#define MODE_2P        2
#define MODE_3P        3
#define MODE_TTR       4
#define MODE_TTT       5

/* . */
ScriptValue
circle_command(ScriptEnv *context)
{
    _main->debug_message("ADDING CIRCLE");
    
    _main->nativeInitCommand();
    _main->nativeClearSelection();

    /*
    add_real_variable(context, "x1", 0.0f);
    add_real_variable(context, "y1", 0.0f);
    add_real_variable(context, "x2", 0.0f);
    add_real_variable(context, "y2", 0.0f);
    add_real_variable(context, "x3", 0.0f);
    add_real_variable(context, "y3", 0.0f);
    add_real_variable(context, "rad", 0.0f);
    add_real_variable(context, "dia", 0.0f);
    add_real_variable(context, "cx", 0.0f);
    add_real_variable(context, "cy", 0.0f);
    add_int_variable(context, "mode", MODE_1P_RAD);


    _main->nativeAppendPromptHistory(translate("Specify center point for circle or [3P/2P/Ttr (tan tan radius)]: "));
    */
    EmbCircle c;
    c.center.x = 0.0;
    c.center.y = 0.0;
    c.radius = 10.0;
    View *v = _main->activeView();
    if (v) {
        emb_array_addCircle(v->geometry, c);
    }
    // _main->nativeAddCircle(0.0, 0.0, 10.0, true, OBJ_RUBBER_CIRCLE_1P_DIA);
   
    _main->nativeEndCommand();
    return script_null;
}

/* NOTE: click() is run only for left clicks.
 *       Middle clicks are used for panning.
 *       Right clicks bring up the context menu.
 */
ScriptValue
circle_click(ScriptEnv *context)
{
    int mode = script_get_int(context, "mode");
    #if 0
    switch (mode) {
    case MODE_1P_RAD: {
        if (isNaN(global.x1)) {
            script_set_real(context, "x1", x);
            script_set_real(context, "y1", y);
            script_set_real(context, "cx", x);
            script_set_real(context, "cy", y);
            addRubber("CIRCLE");
            setRubberMode("CIRCLE_1P_RAD");
            setRubberPoint("CIRCLE_CENTER", x, y);
            appendPromptHistory();
            setPromptPrefix(qsTr("Specify radius of circle or [Diameter]: "));
        }
        else {
            script_set_real(context, "x2", x);
            script_set_real(context, "y2", y);
            setRubberPoint("CIRCLE_RADIUS", global.x2, global.y2);
            vulcanize();
            appendPromptHistory();
            endCommand();
        }
        break;
    }
    case MODE_1P_DIA: {
        if (isNaN(global.x1)) {
            error("CIRCLE", qsTr("This should never happen."));
        }
        else {
            script_set_real(context, "x2", x);
            script_set_real(context, "y2", y);
            setRubberPoint("CIRCLE_DIAMETER", global.x2, global.y2);
            vulcanize();
            appendPromptHistory();
            endCommand();
        }
        break;
    }
    case MODE_2P: {
        if (isNaN(global.x1)) {
            script_set_real(context, "x1", x);
            script_set_real(context, "y1", y);
            addRubber("CIRCLE");
            setRubberMode("CIRCLE_2P");
            setRubberPoint("CIRCLE_TAN1", x, y);
            appendPromptHistory();
            setPromptPrefix(qsTr("Specify second end point of circle's diameter: "));
        }
        else if (isNaN(global.x2)) {
            script_set_real(context, "x2", x);
            script_set_real(context, "y2", y);
            setRubberPoint("CIRCLE_TAN2", x, y);
            vulcanize();
            appendPromptHistory();
            endCommand();
        }
        else {
            error("CIRCLE", qsTr("This should never happen."));
        }
        break;
    }
    case MODE_3P: {
        if (isNaN(global.x1)) {
            global.x1 = x;
            global.y1 = y;
            appendPromptHistory();
            setPromptPrefix(qsTr("Specify second point on circle: "));
        }
        else if (isNaN(global.x2)) {
            script_set_real(context, "x2", x);
            script_set_real(context, "y2", y);
            addRubber("CIRCLE");
            setRubberMode("CIRCLE_3P");
            setRubberPoint("CIRCLE_TAN1", global.x1, global.y1);
            setRubberPoint("CIRCLE_TAN2", global.x2, global.y2);
            appendPromptHistory();
            setPromptPrefix(qsTr("Specify third point on circle: "));
        }
        else if (isNaN(global.x3)) {
            global.x3 = x;
            global.y3 = y;
            setRubberPoint("CIRCLE_TAN3", global.x3, global.y3);
            vulcanize();
            appendPromptHistory();
            endCommand();
        }
        else {
            error("CIRCLE", qsTr("This should never happen."));
        }
        break;
    }
    case MODE_TTR: {
        if (isNaN(global.x1)) {
            global.x1 = x;
            global.y1 = y;
            appendPromptHistory();
            setPromptPrefix(qsTr("Specify point on object for second tangent of circle: "));
        }
        else if (isNaN(global.x2)) {
            global.x2 = x;
            global.y2 = y;
            appendPromptHistory();
            setPromptPrefix(qsTr("Specify radius of circle: "));
        }
        else if (isNaN(global.x3)) {
            global.x3 = x;
            global.y3 = y;
            appendPromptHistory();
            setPromptPrefix(qsTr("Specify second point: "));
        }
        else {
            todo("CIRCLE", "click() for TTR");
        }
    }
    default:
        break;
    }
    #endif
    _main->nativeEndCommand();
    return script_null;
}

/* NOTE: context() is run when a context menu entry is chosen.
 */
ScriptValue
circle_context(ScriptEnv *context)
{
    // todo("CIRCLE", "context()");
    _main->nativeEndCommand();
    return script_null;
}

/* NOTE: prompt() is run when Enter is pressed.
 *       appendPromptHistory is automatically called before prompt()
 *       is called so calling it is only needed for erroneous input.
 *       Any text is in the command prompt is sent as an uppercase string.
 */
ScriptValue
circle_prompt(ScriptEnv *context)
{
    int mode = script_get_int(context, "mode");
    #if 0
    if (global.mode == MODE_1P_RAD) {
        if (isNaN(global.x1)) {
            if (str == "2P") //TODO: Probably should add additional qsTr calls here.
            {
                global.mode = global.mode_2P;
                setPromptPrefix(qsTr("Specify first end point of circle's diameter: "));
            }
            else if (str == "3P") //TODO: Probably should add additional qsTr calls here.
            {
                global.mode = global.mode_3P;
                setPromptPrefix(qsTr("Specify first point of circle: "));
            }
            else if (str == "T" || str == "TTR") //TODO: Probably should add additional qsTr calls here.
            {
                global.mode = global.mode_TTR;
                setPromptPrefix(qsTr("Specify point on object for first tangent of circle: "));
            }
            else {
                var strList = str.split(",");
                if (isNaN(strList[0]) || isNaN(strList[1])) {
                    alert(qsTr("Point or option keyword required."));
                    setPromptPrefix(qsTr("Specify center point for circle or [3P/2P/Ttr (tan tan radius)]: "));
                }
                else {
                    global.x1 = Number(strList[0]);
                    global.y1 = Number(strList[1]);
                    global.cx = global.x1;
                    global.cy = global.y1;
                    addRubber("CIRCLE");
                    setRubberMode("CIRCLE_1P_RAD");
                    setRubberPoint("CIRCLE_CENTER", global.cx, global.cy);
                    setPromptPrefix(qsTr("Specify radius of circle or [Diameter]: "));
                }
            }
        }
        else {
            if (str == "D" || str == "DIAMETER") //TODO: Probably should add additional qsTr calls here.
            {
                global.mode = global.mode_1P_DIA;
                setRubberMode("CIRCLE_1P_DIA");
                setPromptPrefix(qsTr("Specify diameter of circle: "));
            }
            else {
                var num = Number(str);
                if (isNaN(num)) {
                    alert(qsTr("Requires numeric radius, point on circumference, or \"D\"."));
                    setPromptPrefix(qsTr("Specify radius of circle or [Diameter]: "));
                }
                else {
                    global.rad = num;
                    global.x2 = global.x1 + global.rad;
                    global.y2 = global.y1;
                    setRubberPoint("CIRCLE_RADIUS", global.x2, global.y2);
                    vulcanize();
                    endCommand();
                }
            }
        }
    }
    else if (global.mode == MODE_1P_DIA) {
        if (isNaN(global.x1)) {
            error("CIRCLE", qsTr("This should never happen."));
        }
        if (isNaN(global.x2)) {
            var num = Number(str);
            if (isNaN(num)) {
                alert(qsTr("Requires numeric distance or second point."));
                setPromptPrefix(qsTr("Specify diameter of circle: "));
            }
            else {
                global.dia = num;
                global.x2 = global.x1 + global.dia;
                global.y2 = global.y1;
                setRubberPoint("CIRCLE_DIAMETER", global.x2, global.y2);
                vulcanize();
                endCommand();
            }
        }
        else {
            error("CIRCLE", qsTr("This should never happen."));
        }
    }
    else if (global.mode == MODE_2P) {
        if (isNaN(global.x1)) {
            var strList = str.split(",");
            if (isNaN(strList[0]) || isNaN(strList[1])) {
                alert(qsTr("Invalid point."));
                setPromptPrefix(qsTr("Specify first end point of circle's diameter: "));
            }
            else {
                global.x1 = Number(strList[0]);
                global.y1 = Number(strList[1]);
                addRubber("CIRCLE");
                setRubberMode("CIRCLE_2P");
                setRubberPoint("CIRCLE_TAN1", global.x1, global.y1);
                setPromptPrefix(qsTr("Specify second end point of circle's diameter: "));
            }
        }
        else if (isNaN(global.x2)) {
            var strList = str.split(",");
            if (isNaN(strList[0]) || isNaN(strList[1])) {
                alert(qsTr("Invalid point."));
                setPromptPrefix(qsTr("Specify second end point of circle's diameter: "));
            }
            else {
                global.x2 = Number(strList[0]);
                global.y2 = Number(strList[1]);
                setRubberPoint("CIRCLE_TAN2", global.x2, global.y2);
                vulcanize();
                endCommand();
            }
        }
        else {
            error("CIRCLE", qsTr("This should never happen."));
        }
    }
    else if (global.mode == MODE_3P) {
        if (isNaN(global.x1)) {
            var strList = str.split(",");
            if (isNaN(strList[0]) || isNaN(strList[1])) {
                alert(qsTr("Invalid point."));
                setPromptPrefix(qsTr("Specify first point of circle: "));
            }
            else {
                global.x1 = Number(strList[0]);
                global.y1 = Number(strList[1]);
                setPromptPrefix(qsTr("Specify second point of circle: "));
            }
        }
        else if (isNaN(global.x2)) {
            var strList = str.split(",");
            if (isNaN(strList[0]) || isNaN(strList[1])) {
                alert(qsTr("Invalid point."));
                setPromptPrefix(qsTr("Specify second point of circle: "));
            }
            else {
                global.x2 = Number(strList[0]);
                global.y2 = Number(strList[1]);
                addRubber("CIRCLE");
                setRubberMode("CIRCLE_3P");
                setRubberPoint("CIRCLE_TAN1", global.x1, global.y1);
                setRubberPoint("CIRCLE_TAN2", global.x2, global.y2);
                setPromptPrefix(qsTr("Specify third point of circle: "));
            }
        }
        else if (isNaN(global.x3)) {
            var strList = str.split(",");
            if (isNaN(strList[0]) || isNaN(strList[1])) {
                alert(qsTr("Invalid point."));
                setPromptPrefix(qsTr("Specify third point of circle: "));
            }
            else {
                global.x3 = Number(strList[0]);
                global.y3 = Number(strList[1]);
                setRubberPoint("CIRCLE_TAN3", global.x3, global.y3);
                vulcanize();
                endCommand();
            }
        }
        else {
            error("CIRCLE", qsTr("This should never happen."));
        }
        
    }
    else if (global.mode == global.mode_TTR) {
        todo("CIRCLE", "prompt() for TTR");
    }
    #endif

    _main->nativeEndCommand();
    return script_null;
}

CircleObject::CircleObject(qreal centerX, qreal centerY, qreal radius, QRgb rgb, QGraphicsItem* parent) : BaseObject(parent)
{
    qDebug("CircleObject Constructor()");
    init(centerX, centerY, radius, rgb, Qt::SolidLine); //TODO: getCurrentLineType
}

CircleObject::CircleObject(CircleObject* obj, QGraphicsItem* parent) : BaseObject(parent)
{
    qDebug("CircleObject Constructor()");
    if(obj)
    {
        init(obj->objectCenterX(), obj->objectCenterY(), obj->objectRadius(), obj->objectColorRGB(), Qt::SolidLine); //TODO: getCurrentLineType
        setRotation(obj->rotation());
    }
}

CircleObject::~CircleObject()
{
    qDebug("CircleObject Destructor()");
}

void CircleObject::init(qreal centerX, qreal centerY, qreal radius, QRgb rgb, Qt::PenStyle lineType)
{
    setData(OBJ_TYPE, type());
    setData(OBJ_NAME, OBJ_NAME_CIRCLE);

    //WARNING: DO NOT enable QGraphicsItem::ItemIsMovable. If it is enabled,
    //WARNING: and the item is double clicked, the scene will erratically move the item while zooming.
    //WARNING: All movement has to be handled explicitly by us, not by the scene.
    setFlag(QGraphicsItem::ItemIsSelectable, true);

    setObjectRadius(radius);
    setObjectCenter(centerX, centerY);
    setObjectColor(rgb);
    setObjectLineType(lineType);
    setObjectLineWeight(0.35); //TODO: pass in proper lineweight
    setPen(objectPen());
    updatePath();
}

void CircleObject::setObjectCenter(const QPointF& center)
{
    setObjectCenter(center.x(), center.y());
}

void CircleObject::setObjectCenter(qreal centerX, qreal centerY)
{
    setPos(centerX, centerY);
}

void CircleObject::setObjectCenterX(qreal centerX)
{
    setX(centerX);
}

void CircleObject::setObjectCenterY(qreal centerY)
{
    setY(centerY);
}

void CircleObject::setObjectRadius(qreal radius)
{
    setObjectDiameter(radius*2.0);
}

void CircleObject::setObjectDiameter(qreal diameter)
{
    QRectF circRect;
    circRect.setWidth(diameter);
    circRect.setHeight(diameter);
    circRect.moveCenter(QPointF(0,0));
    setRect(circRect);
    updatePath();
}

void CircleObject::setObjectArea(qreal area)
{
    qreal radius = qSqrt(area/pi());
    setObjectRadius(radius);
}

void CircleObject::setObjectCircumference(qreal circumference)
{
    qreal diameter = circumference/pi();
    setObjectDiameter(diameter);
}

void CircleObject::updatePath()
{
    QPainterPath path;
    QRectF r = rect();
    //Add the center point
    path.addRect(-0.00000001, -0.00000001, 0.00000002, 0.00000002);
    //Add the circle
    path.arcMoveTo(r, 0);
    path.arcTo(r, 0, 360);
    //NOTE: Reverse the path so that the inside area isn't considered part of the circle
    path.arcTo(r, 0, -360);
    setObjectPath(path);
}

void CircleObject::paint(QPainter* painter, const QStyleOptionGraphicsItem* option, QWidget* /*widget*/)
{
    QGraphicsScene* objScene = scene();
    if(!objScene) return;

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

    painter->drawEllipse(rect());
}

void CircleObject::updateRubber(QPainter* painter)
{
    int rubberMode = objectRubberMode();
    if(rubberMode == OBJ_RUBBER_CIRCLE_1P_RAD)
    {
        QPointF sceneCenterPoint = objectRubberPoint("CIRCLE_CENTER");
        QPointF sceneQSnapPoint = objectRubberPoint("CIRCLE_RADIUS");
        QPointF itemCenterPoint = mapFromScene(sceneCenterPoint);
        QPointF itemQSnapPoint = mapFromScene(sceneQSnapPoint);
        QLineF itemLine(itemCenterPoint, itemQSnapPoint);
        setObjectCenter(sceneCenterPoint);
        QLineF sceneLine(sceneCenterPoint, sceneQSnapPoint);
        qreal radius = sceneLine.length();
        setObjectRadius(radius);
        if(painter) drawRubberLine(itemLine, painter, VIEW_COLOR_CROSSHAIR);
        updatePath();
    }
    else if(rubberMode == OBJ_RUBBER_CIRCLE_1P_DIA)
    {
        QPointF sceneCenterPoint = objectRubberPoint("CIRCLE_CENTER");
        QPointF sceneQSnapPoint = objectRubberPoint("CIRCLE_DIAMETER");
        QPointF itemCenterPoint = mapFromScene(sceneCenterPoint);
        QPointF itemQSnapPoint = mapFromScene(sceneQSnapPoint);
        QLineF itemLine(itemCenterPoint, itemQSnapPoint);
        setObjectCenter(sceneCenterPoint);
        QLineF sceneLine(sceneCenterPoint, sceneQSnapPoint);
        qreal diameter = sceneLine.length();
        setObjectDiameter(diameter);
        if(painter) drawRubberLine(itemLine, painter, VIEW_COLOR_CROSSHAIR);
        updatePath();
    }
    else if(rubberMode == OBJ_RUBBER_CIRCLE_2P)
    {
        QPointF sceneTan1Point = objectRubberPoint("CIRCLE_TAN1");
        QPointF sceneQSnapPoint = objectRubberPoint("CIRCLE_TAN2");
        QLineF sceneLine(sceneTan1Point, sceneQSnapPoint);
        setObjectCenter(sceneLine.pointAt(0.5));
        qreal diameter = sceneLine.length();
        setObjectDiameter(diameter);
        updatePath();
    }
    else if(rubberMode == OBJ_RUBBER_CIRCLE_3P)
    {
        QPointF sceneTan1Point = objectRubberPoint("CIRCLE_TAN1");
        QPointF sceneTan2Point = objectRubberPoint("CIRCLE_TAN2");
        QPointF sceneTan3Point = objectRubberPoint("CIRCLE_TAN3");

        EmbArc arc;
        arc.start.x = sceneTan1Point.x();
        arc.start.y = sceneTan1Point.y();
        arc.mid.x = sceneTan2Point.x();
        arc.mid.y = sceneTan2Point.y();
        arc.end.x = sceneTan3Point.x();
        arc.end.y = sceneTan3Point.y();
        EmbVector sceneCenter = emb_arc_center(arc);
        QPointF sceneCenterPoint(sceneCenter.x, sceneCenter.y);
        QLineF sceneLine(sceneCenterPoint, sceneTan3Point);
        setObjectCenter(sceneCenterPoint);
        qreal radius = sceneLine.length();
        setObjectRadius(radius);
        updatePath();
    }
    else if(rubberMode == OBJ_RUBBER_GRIP)
    {
        if(painter)
        {
            QPointF gripPoint = objectRubberPoint("GRIP_POINT");
            if(gripPoint == objectCenter())
            {
                painter->drawEllipse(rect().translated(mapFromScene(objectRubberPoint(QString()))-mapFromScene(gripPoint)));
            }
            else
            {
                qreal gripRadius = QLineF(objectCenter(), objectRubberPoint(QString())).length();
                painter->drawEllipse(QPointF(), gripRadius, gripRadius);
            }

            QLineF rubLine(mapFromScene(gripPoint), mapFromScene(objectRubberPoint(QString())));
            drawRubberLine(rubLine, painter, VIEW_COLOR_CROSSHAIR);
        }
    }
}

void CircleObject::vulcanize()
{
    qDebug("CircleObject vulcanize()");
    updateRubber();

    setObjectRubberMode(OBJ_RUBBER_OFF);
}

// Returns the closest snap point to the mouse point
QPointF CircleObject::mouseSnapPoint(const QPointF& mousePoint)
{
    QPointF center  = objectCenter();
    QPointF quad0   = objectQuadrant0();
    QPointF quad90  = objectQuadrant90();
    QPointF quad180 = objectQuadrant180();
    QPointF quad270 = objectQuadrant270();

    qreal cntrDist = QLineF(mousePoint, center).length();
    qreal q0Dist   = QLineF(mousePoint, quad0).length();
    qreal q90Dist  = QLineF(mousePoint, quad90).length();
    qreal q180Dist = QLineF(mousePoint, quad180).length();
    qreal q270Dist = QLineF(mousePoint, quad270).length();

    qreal minDist = qMin(qMin(qMin(q0Dist, q90Dist), qMin(q180Dist, q270Dist)), cntrDist);

    if     (minDist == cntrDist) return center;
    else if(minDist == q0Dist)   return quad0;
    else if(minDist == q90Dist)  return quad90;
    else if(minDist == q180Dist) return quad180;
    else if(minDist == q270Dist) return quad270;

    return scenePos();
}

QList<QPointF> CircleObject::allGripPoints()
{
    QList<QPointF> gripPoints;
    gripPoints << objectCenter() << objectQuadrant0() << objectQuadrant90() << objectQuadrant180() << objectQuadrant270();
    return gripPoints;
}

void CircleObject::gripEdit(const QPointF& before, const QPointF& after)
{
    if(before == objectCenter()) { QPointF delta = after-before; moveBy(delta.x(), delta.y()); }
    else                         { setObjectRadius(QLineF(objectCenter(), after).length()); }
}

QPainterPath CircleObject::objectSavePath() const
{
    QPainterPath path;
    QRectF r = rect();
    path.arcMoveTo(r, 0);
    path.arcTo(r, 0, 360);

    qreal s = scale();
    QTransform trans;
    trans.rotate(rotation());
    trans.scale(s,s);
    return trans.map(path);
}

/* kate: bom off; indent-mode cstyle; indent-width 4; replace-trailing-space-save on; */
