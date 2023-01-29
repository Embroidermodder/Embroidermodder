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

EmbVector point1, point2, point3, center;
double radius;
int mode;

#if 0

var global = {};
global.x1;
global.y1;
global.x2;
global.y2;
global.x3;
global.y3;
global.rad;
global.dia;
global.cx;
global.cy;
global.mode;

//enums
global.mode_1P_RAD = 0;
global.mode_1P_DIA = 1;
global.mode_2P     = 2;
global.mode_3P     = 3;
global.mode_TTR    = 4;

//NOTE: main() is run every time the command is started.
//      Use it to reset variables so they are ready to go.
function main()
{
    initCommand();
    clearSelection();
    global.mode = global.mode_1P_RAD;
    global.x1 = NaN;
    global.y1 = NaN;
    global.x2 = NaN;
    global.y2 = NaN;
    global.x3 = NaN;
    global.y3 = NaN;
    setPromptPrefix(translate("Specify center point for circle or [3P/2P/Ttr (tan tan radius)]: "));
}

//NOTE: click() is run only for left clicks.
//      Middle clicks are used for panning.
//      Right clicks bring up the context menu.
function click(x, y)
{
    if (global.mode == global.mode_1P_RAD)
    {
        if (isNaN(global.x1))
        {
            global.x1 = x;
            global.y1 = y;
            global.cx = x;
            global.cy = y;
            addRubber("CIRCLE");
            setRubberMode("CIRCLE_1P_RAD");
            setRubberPoint("CIRCLE_CENTER", global.cx, global.cy);
            appendPromptHistory();
            setPromptPrefix(translate("Specify radius of circle or [Diameter]: "));
        }
        else
        {
            global.x2 = x;
            global.y2 = y;
            setRubberPoint("CIRCLE_RADIUS", global.x2, global.y2);
            vulcanize();
            appendPromptHistory();
            endCommand();
        }
    }
    else if (global.mode == global.mode_1P_DIA)
    {
        if (isNaN(global.x1))
        {
            error("CIRCLE", translate("This should never happen."));
        }
        else
        {
            global.x2 = x;
            global.y2 = y;
            setRubberPoint("CIRCLE_DIAMETER", global.x2, global.y2);
            vulcanize();
            appendPromptHistory();
            endCommand();
        }
    }
    else if (global.mode == global.mode_2P)
    {
        if (isNaN(global.x1))
        {
            global.x1 = x;
            global.y1 = y;
            addRubber("CIRCLE");
            setRubberMode("CIRCLE_2P");
            setRubberPoint("CIRCLE_TAN1", global.x1, global.y1);
            appendPromptHistory();
            setPromptPrefix(translate("Specify second end point of circle's diameter: "));
        }
        else if (isNaN(global.x2))
        {
            global.x2 = x;
            global.y2 = y;
            setRubberPoint("CIRCLE_TAN2", global.x2, global.y2);
            vulcanize();
            appendPromptHistory();
            endCommand();
        }
        else
        {
            error("CIRCLE", translate("This should never happen."));
        }
    }
    else if (global.mode == global.mode_3P)
    {
        if (isNaN(global.x1))
        {
            global.x1 = x;
            global.y1 = y;
            appendPromptHistory();
            setPromptPrefix(translate("Specify second point on circle: "));
        }
        else if (isNaN(global.x2))
        {
            global.x2 = x;
            global.y2 = y;
            addRubber("CIRCLE");
            setRubberMode("CIRCLE_3P");
            setRubberPoint("CIRCLE_TAN1", global.x1, global.y1);
            setRubberPoint("CIRCLE_TAN2", global.x2, global.y2);
            appendPromptHistory();
            setPromptPrefix(translate("Specify third point on circle: "));
        }
        else if (isNaN(global.x3))
        {
            global.x3 = x;
            global.y3 = y;
            setRubberPoint("CIRCLE_TAN3", global.x3, global.y3);
            vulcanize();
            appendPromptHistory();
            endCommand();
        }
        else
        {
            error("CIRCLE", translate("This should never happen."));
        }
    }
    else if (global.mode == global.mode_TTR)
    {
        if (isNaN(global.x1))
        {
            global.x1 = x;
            global.y1 = y;
            appendPromptHistory();
            setPromptPrefix(translate("Specify point on object for second tangent of circle: "));
        }
        else if (isNaN(global.x2))
        {
            global.x2 = x;
            global.y2 = y;
            appendPromptHistory();
            setPromptPrefix(translate("Specify radius of circle: "));
        }
        else if (isNaN(global.x3))
        {
            global.x3 = x;
            global.y3 = y;
            appendPromptHistory();
            setPromptPrefix(translate("Specify second point: "));
        }
        else
        {
            todo("CIRCLE", "click() for TTR");
        }
    }
}

function context(str)
{
    todo("CIRCLE", "context()");
}

function prompt(str)
{
    if (global.mode == global.mode_1P_RAD)
    {
        if (isNaN(global.x1))
        {
            if (str == "2P") //TODO: Probably should add additional qsTr calls here.
            {
                global.mode = global.mode_2P;
                setPromptPrefix(translate("Specify first end point of circle's diameter: "));
            }
            else if (str == "3P") //TODO: Probably should add additional qsTr calls here.
            {
                global.mode = global.mode_3P;
                setPromptPrefix(translate("Specify first point of circle: "));
            }
            else if (str == "T" || str == "TTR") //TODO: Probably should add additional qsTr calls here.
            {
                global.mode = global.mode_TTR;
                setPromptPrefix(translate("Specify point on object for first tangent of circle: "));
            }
            else
            {
                var strList = str.split(",");
                if (isNaN(strList[0]) || isNaN(strList[1]))
                {
                    alert(translate("Point or option keyword required."));
                    setPromptPrefix(translate("Specify center point for circle or [3P/2P/Ttr (tan tan radius)]: "));
                }
                else
                {
                    global.x1 = Number(strList[0]);
                    global.y1 = Number(strList[1]);
                    global.cx = global.x1;
                    global.cy = global.y1;
                    addRubber("CIRCLE");
                    setRubberMode("CIRCLE_1P_RAD");
                    setRubberPoint("CIRCLE_CENTER", global.cx, global.cy);
                    setPromptPrefix(translate("Specify radius of circle or [Diameter]: "));
                }
            }
        }
        else
        {
            if (str == "D" || str == "DIAMETER") //TODO: Probably should add additional qsTr calls here.
            {
                global.mode = global.mode_1P_DIA;
                setRubberMode("CIRCLE_1P_DIA");
                setPromptPrefix(translate("Specify diameter of circle: "));
            }
            else
            {
                var num = Number(str);
                if (isNaN(num))
                {
                    alert(translate("Requires numeric radius, point on circumference, or \"D\"."));
                    setPromptPrefix(translate("Specify radius of circle or [Diameter]: "));
                }
                else
                {
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
    else if (global.mode == global.mode_1P_DIA)
    {
        if (isNaN(global.x1))
        {
            error("CIRCLE", translate("This should never happen."));
        }
        if (isNaN(global.x2))
        {
            var num = Number(str);
            if (isNaN(num))
            {
                alert(translate("Requires numeric distance or second point."));
                setPromptPrefix(translate("Specify diameter of circle: "));
            }
            else
            {
                global.dia = num;
                global.x2 = global.x1 + global.dia;
                global.y2 = global.y1;
                setRubberPoint("CIRCLE_DIAMETER", global.x2, global.y2);
                vulcanize();
                endCommand();
            }
        }
        else
        {
            error("CIRCLE", translate("This should never happen."));
        }
    }
    else if (global.mode == global.mode_2P)
    {
        if (isNaN(global.x1))
        {
            var strList = str.split(",");
            if (isNaN(strList[0]) || isNaN(strList[1]))
            {
                alert(translate("Invalid point."));
                setPromptPrefix(translate("Specify first end point of circle's diameter: "));
            }
            else {
                global.x1 = Number(strList[0]);
                global.y1 = Number(strList[1]);
                addRubber("CIRCLE");
                setRubberMode("CIRCLE_2P");
                setRubberPoint("CIRCLE_TAN1", global.x1, global.y1);
                setPromptPrefix(translate("Specify second end point of circle's diameter: "));
            }
        }
        else if (isNaN(global.x2)) {
            var strList = str.split(",");
            if (isNaN(strList[0]) || isNaN(strList[1])) {
                alert(translate("Invalid point."));
                setPromptPrefix(translate("Specify second end point of circle's diameter: "));
            }
            else {
                global.x2 = Number(strList[0]);
                global.y2 = Number(strList[1]);
                setRubberPoint("CIRCLE_TAN2", global.x2, global.y2);
                vulcanize();
                endCommand();
            }
        }
        else
        {
            error("CIRCLE", translate("This should never happen."));
        }
    }
    else if (global.mode == global.mode_3P) {
        if (isNaN(global.x1)) {
            var strList = str.split(",");
            if (isNaN(strList[0]) || isNaN(strList[1])) {
                alert(translate("Invalid point."));
                setPromptPrefix(translate("Specify first point of circle: "));
            }
            else {
                global.x1 = Number(strList[0]);
                global.y1 = Number(strList[1]);
                setPromptPrefix(translate("Specify second point of circle: "));
            }
        }
        else if (isNaN(global.x2)) {
            var strList = str.split(",");
            if (isNaN(strList[0]) || isNaN(strList[1])) {
                alert(translate("Invalid point."));
                setPromptPrefix(translate("Specify second point of circle: "));
            }
            else {
                global.x2 = Number(strList[0]);
                global.y2 = Number(strList[1]);
                addRubber("CIRCLE");
                setRubberMode("CIRCLE_3P");
                setRubberPoint("CIRCLE_TAN1", global.x1, global.y1);
                setRubberPoint("CIRCLE_TAN2", global.x2, global.y2);
                setPromptPrefix(translate("Specify third point of circle: "));
            }
        }
        else if (isNaN(global.x3)) {
            var strList = str.split(",");
            if (isNaN(strList[0]) || isNaN(strList[1])) {
                alert(translate("Invalid point."));
                setPromptPrefix(translate("Specify third point of circle: "));
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
            error("CIRCLE", translate("This should never happen."));
        }

    }
    else if (global.mode == global.mode_TTR) {
        todo("CIRCLE", "prompt() for TTR");
    }
}

CircleObject::CircleObject(double centerX, double centerY, double radius, unsigned int rgb, QGraphicsItem* parent) : BaseObject(parent)
{
    debug_message("CircleObject Constructor()");
    init(centerX, centerY, radius, rgb, Qt::SolidLine); //TODO: getCurrentLineType
}

CircleObject::CircleObject(CircleObject* obj, QGraphicsItem* parent) : BaseObject(parent)
{
    debug_message("CircleObject Constructor()");
    if (obj) {
        init(obj->objectCenterX(), obj->objectCenterY(), obj->objectRadius(), obj->objectColorRGB(), Qt::SolidLine); //TODO: getCurrentLineType
        setRotation(obj->rotation());
    }
}

CircleObject::~CircleObject()
{
    debug_message("CircleObject Destructor()");
}

void CircleObject::init(double centerX, double centerY, double radius, unsigned int rgb, Qt::PenStyle lineType)
{
    setData(OBJ_TYPE, type);
    setData(OBJ_NAME, "CIRCLE");

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

void CircleObject::setObjectCenter(EmbVector& center)
{
    setObjectCenter(center.x(), center.y());
}

void CircleObject::setObjectCenter(double centerX, double centerY)
{
    setPos(centerX, centerY);
}

void CircleObject::setObjectCenterX(double centerX)
{
    setX(centerX);
}

void CircleObject::setObjectCenterY(double centerY)
{
    setY(centerY);
}

void CircleObject::setObjectRadius(double radius)
{
    setObjectDiameter(radius*2.0);
}

void CircleObject::setObjectDiameter(double diameter)
{
    EmbRect circRect;
    circRect.setWidth(diameter);
    circRect.setHeight(diameter);
    circRect.moveCenter(EmbVector(0,0));
    setRect(circRect);
    updatePath();
}

void CircleObject::setObjectArea(double area)
{
    double radius = sqrt(area/pi());
    setObjectRadius(radius);
}

void CircleObject::setObjectCircumference(double circumference)
{
    double diameter = circumference/pi();
    setObjectDiameter(diameter);
}

void CircleObject::updatePath()
{
    QPainterPath path;
    EmbRect r = rect();
    //Add the center point
    path.addRect(-0.00000001, -0.00000001, 0.00000002, 0.00000002);
    //Add the circle
    path.arcMoveTo(r, 0);
    path.arcTo(r, 0, 360);
    //NOTE: Reverse the path so that the inside area isn't considered part of the circle
    path.arcTo(r, 0, -360);
    setObjectPath(path);
}

void CircleObject::paint(QPainter* painter, QStyleOptionGraphicsItem* option, QWidget* /*widget*/)
{
    QGraphicsScene* objScene = scene();
    if (!objScene) return;

    QPen paintPen = pen();
    painter->setPen(paintPen);
    updateRubber(painter);
    if (option->state & QStyle::State_Selected)  { paintPen.setStyle(Qt::DashLine); }
    if (objScene->property(ENABLE_LWT).toBool()) { paintPen = lineWeightPen(); }
    painter->setPen(paintPen);

    painter->drawEllipse(rect());
}

void CircleObject::updateRubber(QPainter* painter)
{
    int rubberMode = objectRubberMode();
    if (rubberMode == OBJ_RUBBER_CIRCLE_1P_RAD)
    {
        EmbVector sceneCenterPoint = objectRubberPoint("CIRCLE_CENTER");
        EmbVector sceneQSnapPoint = objectRubberPoint("CIRCLE_RADIUS");
        EmbVector itemCenterPoint = mapFromScene(sceneCenterPoint);
        EmbVector itemQSnapPoint = mapFromScene(sceneQSnapPoint);
        EmbLine itemLine(itemCenterPoint, itemQSnapPoint);
        setObjectCenter(sceneCenterPoint);
        EmbLine sceneLine(sceneCenterPoint, sceneQSnapPoint);
        double radius = sceneLine.length();
        setObjectRadius(radius);
        if (painter) drawRubberLine(itemLine, painter, VIEW_COLOR_CROSSHAIR);
        updatePath();
    }
    else if (rubberMode == OBJ_RUBBER_CIRCLE_1P_DIA) {
        EmbVector sceneCenterPoint = objectRubberPoint("CIRCLE_CENTER");
        EmbVector sceneQSnapPoint = objectRubberPoint("CIRCLE_DIAMETER");
        EmbVector itemCenterPoint = mapFromScene(sceneCenterPoint);
        EmbVector itemQSnapPoint = mapFromScene(sceneQSnapPoint);
        EmbLine itemLine(itemCenterPoint, itemQSnapPoint);
        setObjectCenter(sceneCenterPoint);
        EmbLine sceneLine(sceneCenterPoint, sceneQSnapPoint);
        double diameter = sceneLine.length();
        setObjectDiameter(diameter);
        if (painter) drawRubberLine(itemLine, painter, VIEW_COLOR_CROSSHAIR);
        updatePath();
    }
    else if (rubberMode == OBJ_RUBBER_CIRCLE_2P)
    {
        EmbVector sceneTan1Point = objectRubberPoint("CIRCLE_TAN1");
        EmbVector sceneQSnapPoint = objectRubberPoint("CIRCLE_TAN2");
        EmbLine sceneLine(sceneTan1Point, sceneQSnapPoint);
        setObjectCenter(sceneLine.pointAt(0.5));
        double diameter = sceneLine.length();
        setObjectDiameter(diameter);
        updatePath();
    }
    else if (rubberMode == OBJ_RUBBER_CIRCLE_3P)
    {
        EmbVector sceneTan1Point = objectRubberPoint("CIRCLE_TAN1");
        EmbVector sceneTan2Point = objectRubberPoint("CIRCLE_TAN2");
        EmbVector sceneTan3Point = objectRubberPoint("CIRCLE_TAN3");

        double sceneCenterX;
        double sceneCenterY;
        EmbArc arc;
        EmbVector sceneCenter;
        arc.start.x = sceneTan1Point.x();
        arc.start.y = sceneTan1Point.y();
        arc.mid.x = sceneTan2Point.x();
        arc.mid.y = sceneTan2Point.y();
        arc.end.x = sceneTan3Point.x();
        arc.end.y = sceneTan3Point.y();
        getArcCenter(arc, &sceneCenter);
        EmbVector sceneCenterPoint(sceneCenter.x, sceneCenter.y);
        EmbLine sceneLine(sceneCenterPoint, sceneTan3Point);
        setObjectCenter(sceneCenterPoint);
        double radius = sceneLine.length();
        setObjectRadius(radius);
        updatePath();
    }
    else if (rubberMode == OBJ_RUBBER_GRIP)
    {
        if (painter)
        {
            EmbVector gripPoint = objectRubberPoint("GRIP_POINT");
            if (gripPoint == objectCenter())
            {
                painter->drawEllipse(rect().translated(mapFromScene(objectRubberPoint(std::string()))-mapFromScene(gripPoint)));
            }
            else
            {
                double gripRadius = EmbLine(objectCenter(), objectRubberPoint(std::string())).length();
                painter->drawEllipse(EmbVector(), gripRadius, gripRadius);
            }

            EmbLine rubLine(mapFromScene(gripPoint), mapFromScene(objectRubberPoint(std::string())));
            drawRubberLine(rubLine, painter, VIEW_COLOR_CROSSHAIR);
        }
    }
}

void CircleObject::vulcanize()
{
    debug_message("CircleObject vulcanize()");
    updateRubber();

    setObjectRubberMode(OBJ_RUBBER_OFF);
}

// Returns the closest snap point to the mouse point
EmbVector CircleObject::mouseSnapPoint(EmbVector& mousePoint)
{
    EmbVector center  = objectCenter();
    EmbVector quad0   = objectQuadrant0();
    EmbVector quad90  = objectQuadrant90();
    EmbVector quad180 = objectQuadrant180();
    EmbVector quad270 = objectQuadrant270();

    double cntrDist = EmbLine(mousePoint, center).length();
    double q0Dist   = EmbLine(mousePoint, quad0).length();
    double q90Dist  = EmbLine(mousePoint, quad90).length();
    double q180Dist = EmbLine(mousePoint, quad180).length();
    double q270Dist = EmbLine(mousePoint, quad270).length();

    double minDist = std::min(std::min(std::min(q0Dist, q90Dist), std::min(q180Dist, q270Dist)), cntrDist);

    if     (minDist == cntrDist) return center;
    else if (minDist == q0Dist)   return quad0;
    else if (minDist == q90Dist)  return quad90;
    else if (minDist == q180Dist) return quad180;
    else if (minDist == q270Dist) return quad270;

    return scenePos();
}

std::vector<EmbVector> CircleObject::allGripPoints()
{
    std::vector<EmbVector> gripPoints;
    gripPoints << objectCenter() << objectQuadrant0() << objectQuadrant90() << objectQuadrant180() << objectQuadrant270();
    return gripPoints;
}

void CircleObject::gripEdit(EmbVector& before, EmbVector& after)
{
    if (before == objectCenter()) {
        EmbVector delta = after-before;
        moveBy(delta.x(), delta.y());
    }
    else {
        setObjectRadius(EmbLine(objectCenter(), after).length());
    }
}

QPainterPath CircleObject::objectSavePath() const
{
    QPainterPath path;
    EmbRect r = rect();
    path.arcMoveTo(r, 0);
    path.arcTo(r, 0, 360);

    double s = scale();
    QTransform trans;
    trans.rotate(rotation());
    trans.scale(s,s);
    return trans.map(path);
}
#endif
