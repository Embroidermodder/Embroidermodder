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
EmbVector point1, point2, point3, center;
double width, height, rot;
int mode;


var global = {}; //Required
global.x1;
global.y1;
global.x2;
global.y2;
global.x3;
global.y3;
global.cx;
global.cy;
global.width;
global.height;
global.rot;
global.mode;

//enums
global.mode_MAJORDIAMETER_MINORRADIUS = 0;
global.mode_MAJORRADIUS_MINORRADIUS   = 1;
global.mode_ELLIPSE_ROTATION          = 2;

//NOTE: main() is run every time the command is started.
//      Use it to reset variables so they are ready to go.
function main()
{
    initCommand();
    clearSelection();
    global.mode = global.mode_MAJORDIAMETER_MINORRADIUS;
    global.x1      = NaN;
    global.y1      = NaN;
    global.x2      = NaN;
    global.y2      = NaN;
    global.x3      = NaN;
    global.y3      = NaN;
    setPromptPrefix(translate("Specify first axis start point or [Center]: "));
}

//NOTE: click() is run only for left clicks.
//      Middle clicks are used for panning.
//      Right clicks bring up the context menu.
function click(x, y)
{
    if (global.mode == global.mode_MAJORDIAMETER_MINORRADIUS) {
        if (isNaN(global.x1)) {
            global.x1 = x;
            global.y1 = y;
            addRubber("ELLIPSE");
            setRubberMode("ELLIPSE_LINE");
            setRubberPoint("ELLIPSE_LINE_POINT1", global.x1, global.y1);
            appendPromptHistory();
            setPromptPrefix(translate("Specify first axis end point: "));
        }
        else if (isNaN(global.x2)) {
            global.x2 = x;
            global.y2 = y;
            global.cx = (global.x1 + global.x2)/2.0;
            global.cy = (global.y1 + global.y2)/2.0;
            global.width = calculateDistance(global.x1, global.y1, global.x2, global.y2);
            global.rot = calculateAngle(global.x1, global.y1, global.x2, global.y2);
            setRubberMode("ELLIPSE_MAJORDIAMETER_MINORRADIUS");
            setRubberPoint("ELLIPSE_AXIS1_POINT1", global.x1, global.y1);
            setRubberPoint("ELLIPSE_AXIS1_POINT2", global.x2, global.y2);
            setRubberPoint("ELLIPSE_CENTER", global.cx, global.cy);
            setRubberPoint("ELLIPSE_WIDTH", global.width, 0);
            setRubberPoint("ELLIPSE_ROT", global.rot, 0);
            appendPromptHistory();
            setPromptPrefix(translate("Specify second axis end point or [Rotation]: "));
        }
        else if (isNaN(global.x3)) {
            global.x3 = x;
            global.y3 = y;
            global.height = perpendicularDistance(global.x3, global.y3, global.x1, global.y1, global.x2, global.y2)*2.0;
            setRubberPoint("ELLIPSE_AXIS2_POINT2", global.x3, global.y3);
            vulcanize();
            appendPromptHistory();
            endCommand();
        }
        else
        {
            error("ELLIPSE", translate("This should never happen."));
        }
    }
    else if (global.mode == global.mode_MAJORRADIUS_MINORRADIUS)
    {
        if (isNaN(global.x1))
        {
            global.x1 = x;
            global.y1 = y;
            global.cx = global.x1;
            global.cy = global.y1;
            addRubber("ELLIPSE");
            setRubberMode("ELLIPSE_LINE");
            setRubberPoint("ELLIPSE_LINE_POINT1", global.x1, global.y1);
            setRubberPoint("ELLIPSE_CENTER", global.cx, global.cy);
            appendPromptHistory();
            setPromptPrefix(translate("Specify first axis end point: "));
        }
        else if (isNaN(global.x2))
        {
            global.x2 = x;
            global.y2 = y;
            global.width = calculateDistance(global.cx, global.cy, global.x2, global.y2)*2.0;
            global.rot = calculateAngle(global.x1, global.y1, global.x2, global.y2);
            setRubberMode("ELLIPSE_MAJORRADIUS_MINORRADIUS");
            setRubberPoint("ELLIPSE_AXIS1_POINT2", global.x2, global.y2);
            setRubberPoint("ELLIPSE_WIDTH", global.width, 0);
            setRubberPoint("ELLIPSE_ROT", global.rot, 0);
            appendPromptHistory();
            setPromptPrefix(translate("Specify second axis end point or [Rotation]: "));
        }
        else if (isNaN(global.x3))
        {
            global.x3 = x;
            global.y3 = y;
            global.height = perpendicularDistance(global.x3, global.y3, global.cx, global.cy, global.x2, global.y2)*2.0;
            setRubberPoint("ELLIPSE_AXIS2_POINT2", global.x3, global.y3);
            vulcanize();
            appendPromptHistory();
            endCommand();
        }
        else
        {
            error("ELLIPSE", translate("This should never happen."));
        }
    }
    else if (global.mode == global.mode_ELLIPSE_ROTATION) {
        if (isNaN(global.x1)) {
            error("ELLIPSE", translate("This should never happen."));
        }
        else if (isNaN(global.x2)) {
            error("ELLIPSE", translate("This should never happen."));
        }
        else if (isNaN(global.x3)) {
            var angle = calculateAngle(global.cx, global.cy, x, y);
            global.height = Math.cos(angle*Math.PI/180.0)*global.width;
            addEllipse(global.cx, global.cy, global.width, global.height, global.rot, false);
            appendPromptHistory();
            endCommand();
        }
    }
}

void context(str)
{
    todo("ELLIPSE", "context()");
}

function prompt(str)
{
    if (mode == MAJORDIAMETER_MINORRADIUS) {
        if (isNaN(global.x1))
        {
            if (str == "C" || str == "CENTER") //TODO: Probably should add additional qsTr calls here.
            {
                global.mode = global.mode_MAJORRADIUS_MINORRADIUS;
                setPromptPrefix(translate("Specify center point: "));
            }
            else
            {
                var strList = str.split(",");
                if (isNaN(strList[0]) || isNaN(strList[1]))
                {
                    alert(translate("Point or option keyword required."));
                    setPromptPrefix(translate("Specify first axis start point or [Center]: "));
                }
                else
                {
                    global.x1 = Number(strList[0]);
                    global.y1 = Number(strList[1]);
                    addRubber("ELLIPSE");
                    setRubberMode("ELLIPSE_LINE");
                    setRubberPoint("ELLIPSE_LINE_POINT1", global.x1, global.y1);
                    setPromptPrefix(translate("Specify first axis end point: "));
                }
            }
        }
        else if (isNaN(global.x2)) {
            var strList = str.split(",");
            if (isNaN(strList[0]) || isNaN(strList[1])) {
                alert(translate("Invalid point."));
                setPromptPrefix(translate("Specify first axis end point: "));
            }
            else {
                global.x2 = Number(strList[0]);
                global.y2 = Number(strList[1]);
                global.cx = (global.x1 + global.x2)/2.0;
                global.cy = (global.y1 + global.y2)/2.0;
                global.width = calculateDistance(global.x1, global.y1, global.x2, global.y2);
                global.rot = calculateAngle(global.x1, global.y1, global.x2, global.y2);
                setRubberMode("ELLIPSE_MAJORDIAMETER_MINORRADIUS");
                setRubberPoint("ELLIPSE_AXIS1_POINT1", global.x1, global.y1);
                setRubberPoint("ELLIPSE_AXIS1_POINT2", global.x2, global.y2);
                setRubberPoint("ELLIPSE_CENTER", global.cx, global.cy);
                setRubberPoint("ELLIPSE_WIDTH", global.width, 0);
                setRubberPoint("ELLIPSE_ROT", global.rot, 0);
                setPromptPrefix(translate("Specify second axis end point or [Rotation]: "));
            }
        }
        else if (isNaN(global.x3)) {
            if (str == "R" || str == "ROTATION") {//TODO: Probably should add additional qsTr calls here.
                global.mode = global.mode_ELLIPSE_ROTATION;
                setPromptPrefix(translate("Specify rotation: "));
            }
            else {
                var strList = str.split(",");
                if (isNaN(strList[0]) || isNaN(strList[1])) {
                    alert(translate("Point or option keyword required."));
                    setPromptPrefix(translate("Specify second axis end point or [Rotation]: "));
                }
                else {
                    global.x3 = Number(strList[0]);
                    global.y3 = Number(strList[1]);
                    global.height = perpendicularDistance(global.x3, global.y3, global.x1, global.y1, global.x2, global.y2)*2.0;
                    setRubberPoint("ELLIPSE_AXIS2_POINT2", global.x3, global.y3);
                    vulcanize();
                    endCommand();
                }
            }
        }
    }
    else if (mode == MAJORRADIUS_MINORRADIUS) {
        if (isNaN(global.x1)) {
            var strList = str.split(",");
            if (isNaN(strList[0]) || isNaN(strList[1])) {
                alert(translate("Invalid point."));
                setPromptPrefix(translate("Specify center point: "));
            }
            else {
                global.x1 = Number(strList[0]);
                global.y1 = Number(strList[1]);
                global.cx = global.x1;
                global.cy = global.y1;
                addRubber("ELLIPSE");
                setRubberMode("ELLIPSE_LINE");
                setRubberPoint("ELLIPSE_LINE_POINT1", global.x1, global.y1);
                setRubberPoint("ELLIPSE_CENTER", global.cx, global.cy);
                setPromptPrefix(translate("Specify first axis end point: "));
            }
        }
        else if (isNaN(global.x2))
        {
            var strList = str.split(",");
            if (isNaN(strList[0]) || isNaN(strList[1]))
            {
                alert(translate("Invalid point."));
                setPromptPrefix(translate("Specify first axis end point: "));
            }
            else
            {
                global.x2 = Number(strList[0]);
                global.y2 = Number(strList[1]);
                global.width = calculateDistance(global.x1, global.y1, global.x2, global.y2)*2.0;
                global.rot = calculateAngle(global.x1, global.y1, global.x2, global.y2);
                setRubberMode("ELLIPSE_MAJORRADIUS_MINORRADIUS");
                setRubberPoint("ELLIPSE_AXIS1_POINT2", global.x2, global.y2);
                setRubberPoint("ELLIPSE_WIDTH", global.width, 0);
                setRubberPoint("ELLIPSE_ROT", global.rot, 0);
                setPromptPrefix(translate("Specify second axis end point or [Rotation]: "));
            }
        }
        else if (isNaN(global.x3))
        {
            if (str == "R" || str == "ROTATION") //TODO: Probably should add additional qsTr calls here.
            {
                global.mode = global.mode_ELLIPSE_ROTATION;
                setPromptPrefix(translate("Specify ellipse rotation: "));
            }
            else
            {
                var strList = str.split(",");
                if (isNaN(strList[0]) || isNaN(strList[1]))
                {
                    alert(translate("Point or option keyword required."));
                    setPromptPrefix(translate("Specify second axis end point or [Rotation]: "));
                }
                else
                {
                    global.x3 = Number(strList[0]);
                    global.y3 = Number(strList[1]);
                    global.height = perpendicularDistance(global.x3, global.y3, global.x1, global.y1, global.x2, global.y2)*2.0;
                    setRubberPoint("ELLIPSE_AXIS2_POINT2", global.x3, global.y3);
                    vulcanize();
                    endCommand();
                }
            }
        }
    }
    else if (global.mode == global.mode_ELLIPSE_ROTATION)
    {
        if (isNaN(global.x1))
        {
            error("ELLIPSE", translate("This should never happen."));
        }
        else if (isNaN(global.x2))
        {
            error("ELLIPSE", translate("This should never happen."));
        }
        else if (isNaN(global.x3))
        {
            if (isNaN(str))
            {
                alert(translate("Invalid angle. Input a numeric angle or pick a point."));
                setPromptPrefix(translate("Specify rotation: "));
            }
            else
            {
                var angle = Number(str);
                global.height = Math.cos(angle*Math.PI/180.0)*global.width;
                addEllipse(global.cx, global.cy, global.width, global.height, global.rot, false);
                endCommand();
            }
        }
    }
}

ellipse_EllipseObject(double centerX, double centerY, double width, double height, unsigned int rgb, QGraphicsItem* parent) : BaseObject(parent)
{
    debug_message("EllipseObject Constructor()");
    init(centerX, centerY, width, height, rgb, Qt::SolidLine); //TODO: getCurrentLineType
}

ellipse_EllipseObject(EllipseObject* obj, QGraphicsItem* parent) : BaseObject(parent)
{
    debug_message("EllipseObject Constructor()");
    if (obj) {
        init(obj->objectCenterX(), obj->objectCenterY(), obj->objectWidth(), obj->objectHeight(), obj->objectColorRGB(), Qt::SolidLine); //TODO: getCurrentLineType
        setRotation(obj->rotation());
    }
}

void ellipse_init(double centerX, double centerY, double width, double height, unsigned int rgb, Qt::PenStyle lineType)
{
    setData(OBJ_TYPE, type);
    setData(OBJ_NAME, "Ellipse");

    //WARNING: DO NOT enable QGraphicsItem::ItemIsMovable. If it is enabled,
    //WARNING: and the item is double clicked, the scene will erratically move the item while zooming.
    //WARNING: All movement has to be handled explicitly by us, not by the scene.
    setFlag(QGraphicsItem::ItemIsSelectable, true);

    setObjectSize(width, height);
    setObjectCenter(centerX, centerY);
    setObjectColor(rgb);
    setObjectLineType(lineType);
    setObjectLineWeight(0.35); //TODO: pass in proper lineweight
    setPen(objectPen());
    updatePath();
}

void ellipse_setObjectSize(double width, double height)
{
    EmbRect elRect = rect();
    elRect.setWidth(width);
    elRect.setHeight(height);
    elRect.moveCenter(EmbVector(0,0));
    setRect(elRect);
}

void ellipse_setObjectCenter(const EmbVector& center)
{
    setObjectCenter(center.x(), center.y());
}

void ellipse_setObjectCenter(double centerX, double centerY)
{
    setPos(centerX, centerY);
}

void ellipse_setObjectCenterX(double centerX)
{
    setX(centerX);
}

void ellipse_setObjectCenterY(double centerY)
{
    setY(centerY);
}

void ellipse_setObjectRadiusMajor(double radius)
{
    setObjectDiameterMajor(radius*2.0);
}

void ellipse_setObjectRadiusMinor(double radius)
{
    setObjectDiameterMinor(radius*2.0);
}

void ellipse_setObjectDiameterMajor(double diameter)
{
    EmbRect elRect = rect();
    if (elRect.width() > elRect.height())
        elRect.setWidth(diameter);
    else
        elRect.setHeight(diameter);
    elRect.moveCenter(EmbVector(0,0));
    setRect(elRect);
}

void ellipse_setObjectDiameterMinor(double diameter)
{
    EmbRect elRect = rect();
    if (elRect.width() < elRect.height())
        elRect.setWidth(diameter);
    else
        elRect.setHeight(diameter);
    elRect.moveCenter(EmbVector(0,0));
    setRect(elRect);
}

EmbVector ellipse_objectQuadrant0() const
{
    double halfW = objectWidth()/2.0;
    double rot = radians(rotation());
    double x = halfW*cos(rot);
    double y = halfW*sin(rot);
    return objectCenter() + EmbVector(x,y);
}

EmbVector ellipse_objectQuadrant90() const
{
    double halfH = objectHeight()/2.0;
    double rot = radians(rotation()+90.0);
    double x = halfH * cos(rot);
    double y = halfH * sin(rot);
    return objectCenter() + EmbVector(x,y);
}

EmbVector ellipse_objectQuadrant180() const
{
    double halfW = objectWidth()/2.0;
    double rot = radians(rotation()+180.0);
    double x = halfW*cos(rot);
    double y = halfW*sin(rot);
    return objectCenter() + EmbVector(x,y);
}

EmbVector ellipse_objectQuadrant270() const
{
    double halfH = objectHeight()/2.0;
    double rot = radians(rotation()+270.0);
    double x = halfH*cos(rot);
    double y = halfH*sin(rot);
    return objectCenter() + EmbVector(x,y);
}

void ellipse_updatePath()
{
    QPainterPath path;
    EmbRect r = rect();
    path.arcMoveTo(r, 0);
    path.arcTo(r, 0, 360);
    //NOTE: Reverse the path so that the inside area isn't considered part of the ellipse
    path.arcTo(r, 0, -360);
    setObjectPath(path);
}

void ellipse_paint(QPainter* painter, const QStyleOptionGraphicsItem* option, QWidget* /*widget*/)
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

void ellipse_updateRubber(QPainter* painter)
{
    int rubberMode = objectRubberMode();
    if (rubberMode == OBJ_RUBBER_ELLIPSE_LINE)
    {
        EmbVector sceneLinePoint1 = objectRubberPoint("ELLIPSE_LINE_POINT1");
        EmbVector sceneLinePoint2 = objectRubberPoint("ELLIPSE_LINE_POINT2");
        EmbVector itemLinePoint1  = mapFromScene(sceneLinePoint1);
        EmbVector itemLinePoint2  = mapFromScene(sceneLinePoint2);
        EmbLine itemLine(itemLinePoint1, itemLinePoint2);
        if (painter) drawRubberLine(itemLine, painter, VIEW_COLOR_CROSSHAIR);
        updatePath();
    }
    else if (rubberMode == OBJ_RUBBER_ELLIPSE_MAJORDIAMETER_MINORRADIUS)
    {
        EmbVector sceneAxis1Point1 = objectRubberPoint("ELLIPSE_AXIS1_POINT1");
        EmbVector sceneAxis1Point2 = objectRubberPoint("ELLIPSE_AXIS1_POINT2");
        EmbVector sceneCenterPoint = objectRubberPoint("ELLIPSE_CENTER");
        EmbVector sceneAxis2Point2 = objectRubberPoint("ELLIPSE_AXIS2_POINT2");
        double ellipseWidth = objectRubberPoint("ELLIPSE_WIDTH").x();
        double ellipseRot = objectRubberPoint("ELLIPSE_ROT").x();

        //TODO: incorporate perpendicularDistance() into libcgeometry
        double px = sceneAxis2Point2.x();
        double py = sceneAxis2Point2.y();
        double x1 = sceneAxis1Point1.x();
        double y1 = sceneAxis1Point1.y();
        EmbLine line(sceneAxis1Point1, sceneAxis1Point2);
        EmbLine norm = line.normalVector();
        double dx = px-x1;
        double dy = py-y1;
        norm.translate(dx, dy);
        EmbVector iPoint;
        norm.intersects(line, &iPoint);
        double ellipseHeight = EmbLine(px, py, iPoint.x(), iPoint.y()).length()*2.0;

        setObjectCenter(sceneCenterPoint);
        setObjectSize(ellipseWidth, ellipseHeight);
        setRotation(-ellipseRot);

        EmbVector itemCenterPoint = mapFromScene(sceneCenterPoint);
        EmbVector itemAxis2Point2 = mapFromScene(sceneAxis2Point2);
        EmbLine itemLine(itemCenterPoint, itemAxis2Point2);
        if (painter) drawRubberLine(itemLine, painter, VIEW_COLOR_CROSSHAIR);
        updatePath();
    }
    else if (rubberMode == OBJ_RUBBER_ELLIPSE_MAJORRADIUS_MINORRADIUS)
    {
        EmbVector sceneAxis1Point2 = objectRubberPoint("ELLIPSE_AXIS1_POINT2");
        EmbVector sceneCenterPoint = objectRubberPoint("ELLIPSE_CENTER");
        EmbVector sceneAxis2Point2 = objectRubberPoint("ELLIPSE_AXIS2_POINT2");
        double ellipseWidth = objectRubberPoint("ELLIPSE_WIDTH").x();
        double ellipseRot = objectRubberPoint("ELLIPSE_ROT").x();

        //TODO: incorporate perpendicularDistance() into libcgeometry
        double px = sceneAxis2Point2.x();
        double py = sceneAxis2Point2.y();
        double x1 = sceneCenterPoint.x();
        double y1 = sceneCenterPoint.y();
        EmbLine line(sceneCenterPoint, sceneAxis1Point2);
        EmbLine norm = line.normalVector();
        double dx = px-x1;
        double dy = py-y1;
        norm.translate(dx, dy);
        EmbVector iPoint;
        norm.intersects(line, &iPoint);
        double ellipseHeight = EmbLine(px, py, iPoint.x(), iPoint.y()).length()*2.0;

        setObjectCenter(sceneCenterPoint);
        setObjectSize(ellipseWidth, ellipseHeight);
        setRotation(-ellipseRot);

        EmbVector itemCenterPoint = mapFromScene(sceneCenterPoint);
        EmbVector itemAxis2Point2 = mapFromScene(sceneAxis2Point2);
        EmbLine itemLine(itemCenterPoint, itemAxis2Point2);
        if (painter) drawRubberLine(itemLine, painter, VIEW_COLOR_CROSSHAIR);
        updatePath();
    }
    else if (rubberMode == OBJ_RUBBER_GRIP)
    {
        //TODO: updateRubber() gripping for EllipseObject
    }
}

void ellipse_vulcanize()
{
    debug_message("EllipseObject vulcanize()");
    updateRubber();

    setObjectRubberMode(OBJ_RUBBER_OFF);
}

// Returns the closest snap point to the mouse point
EmbVector ellipse_mouseSnapPoint(const EmbVector& mousePoint)
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

std::vector<EmbVector> ellipse_allGripPoints()
{
    std::vector<EmbVector> gripPoints;
    gripPoints << objectCenter() << objectQuadrant0() << objectQuadrant90() << objectQuadrant180() << objectQuadrant270();
    return gripPoints;
}

void ellipse_gripEdit(const EmbVector& before, const EmbVector& after)
{
    //TODO: gripEdit() for EllipseObject
}

QPainterPath ellipse_objectSavePath() const
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
