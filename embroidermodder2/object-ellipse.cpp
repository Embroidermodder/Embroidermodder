/*
 * Embroidermodder 2.
 *
 * Copyright 2013-2024 The Embroidermodder Team
 * Embroidermodder 2 is Open Source Software.
 * See LICENSE for licensing terms.
 *
 * Command: about
 */

#include "../commands.h"

/* NOTE: main() is run every time the command is started.
 *       Use it to reset variables so they are ready to go.
 */
ScriptValue
ellipse_command(ScriptEnv *context)
{
    _main->nativeInitCommand();
    _main->nativeClearSelection();

    _main->nativeEndCommand();
    return script_null;
}

/* NOTE: click() is run only for left clicks.
 *       Middle clicks are used for panning.
 *       Right clicks bring up the context menu.
 */
ScriptValue
ellipse_click(ScriptEnv* /* context */)
{

    _main->nativeEndCommand();
    return script_null;
}

/* NOTE: context() is run when a context menu entry is chosen.
 */
ScriptValue
ellipse_context(ScriptEnv* /* context */)
{

    _main->nativeEndCommand();
    return script_null;
}

/* NOTE: prompt() is run when Enter is pressed.
 *       appendPromptHistory is automatically called before prompt()
 *       is called so calling it is only needed for erroneous input.
 *       Any text is in the command prompt is sent as an uppercase string.
 */
ScriptValue
ellipse_prompt(ScriptEnv* /* context */)
{

    _main->nativeEndCommand();
    return script_null;
}

#if 0
//Command: Ellipse

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
    setPromptPrefix(qsTr("Specify first axis start point or [Center]: "));
}

//NOTE: click() is run only for left clicks.
//      Middle clicks are used for panning.
//      Right clicks bring up the context menu.
function click(x, y)
{
    if(global.mode == global.mode_MAJORDIAMETER_MINORRADIUS)
    {
        if(isNaN(global.x1))
        {
            global.x1 = x;
            global.y1 = y;
            addRubber("ELLIPSE");
            setRubberMode("ELLIPSE_LINE");
            setRubberPoint("ELLIPSE_LINE_POINT1", global.x1, global.y1);
            appendPromptHistory();
            setPromptPrefix(qsTr("Specify first axis end point: "));
        }
        else if(isNaN(global.x2))
        {
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
            setPromptPrefix(qsTr("Specify second axis end point or [Rotation]: "));
        }
        else if(isNaN(global.x3))
        {
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
            error("ELLIPSE", qsTr("This should never happen."));
        }
    }
    else if(global.mode == global.mode_MAJORRADIUS_MINORRADIUS)
    {
        if(isNaN(global.x1))
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
            setPromptPrefix(qsTr("Specify first axis end point: "));
        }
        else if(isNaN(global.x2))
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
            setPromptPrefix(qsTr("Specify second axis end point or [Rotation]: "));
        }
        else if(isNaN(global.x3))
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
            error("ELLIPSE", qsTr("This should never happen."));
        }
    }
    else if(global.mode == global.mode_ELLIPSE_ROTATION)
    {
        if(isNaN(global.x1))
        {
            error("ELLIPSE", qsTr("This should never happen."));
        }
        else if(isNaN(global.x2))
        {
            error("ELLIPSE", qsTr("This should never happen."));
        }
        else if(isNaN(global.x3))
        {
            var angle = calculateAngle(global.cx, global.cy, x, y);
            global.height = Math.cos(angle*Math.PI/180.0)*global.width;
            addEllipse(global.cx, global.cy, global.width, global.height, global.rot, false);
            appendPromptHistory();
            endCommand();
        }
    }
}

//NOTE: context() is run when a context menu entry is chosen.
function context(str)
{
    todo("ELLIPSE", "context()");
}

//NOTE: prompt() is run when Enter is pressed.
//      appendPromptHistory is automatically called before prompt()
//      is called so calling it is only needed for erroneous input.
//      Any text in the command prompt is sent as an uppercase string.
function prompt(str)
{
    if(global.mode == global.mode_MAJORDIAMETER_MINORRADIUS)
    {
        if(isNaN(global.x1))
        {
            if(str == "C" || str == "CENTER") //TODO: Probably should add additional qsTr calls here.
            {
                global.mode = global.mode_MAJORRADIUS_MINORRADIUS;
                setPromptPrefix(qsTr("Specify center point: "));
            }
            else
            {
                var strList = str.split(",");
                if(isNaN(strList[0]) || isNaN(strList[1]))
                {
                    alert(qsTr("Point or option keyword required."));
                    setPromptPrefix(qsTr("Specify first axis start point or [Center]: "));
                }
                else
                {
                    global.x1 = Number(strList[0]);
                    global.y1 = Number(strList[1]);
                    addRubber("ELLIPSE");
                    setRubberMode("ELLIPSE_LINE");
                    setRubberPoint("ELLIPSE_LINE_POINT1", global.x1, global.y1);
                    setPromptPrefix(qsTr("Specify first axis end point: "));
                }
            }
        }
        else if(isNaN(global.x2))
        {
            var strList = str.split(",");
            if(isNaN(strList[0]) || isNaN(strList[1]))
            {
                alert(qsTr("Invalid point."));
                setPromptPrefix(qsTr("Specify first axis end point: "));
            }
            else
            {
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
                setPromptPrefix(qsTr("Specify second axis end point or [Rotation]: "));
            }
        }
        else if(isNaN(global.x3))
        {
            if(str == "R" || str == "ROTATION") //TODO: Probably should add additional qsTr calls here.
            {
                global.mode = global.mode_ELLIPSE_ROTATION;
                setPromptPrefix(qsTr("Specify rotation: "));
            }
            else
            {
                var strList = str.split(",");
                if(isNaN(strList[0]) || isNaN(strList[1]))
                {
                    alert(qsTr("Point or option keyword required."));
                    setPromptPrefix(qsTr("Specify second axis end point or [Rotation]: "));
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
    else if(global.mode == global.mode_MAJORRADIUS_MINORRADIUS)
    {
        if(isNaN(global.x1))
        {
            var strList = str.split(",");
            if(isNaN(strList[0]) || isNaN(strList[1]))
            {
                alert(qsTr("Invalid point."));
                setPromptPrefix(qsTr("Specify center point: "));
            }
            else
            {
                global.x1 = Number(strList[0]);
                global.y1 = Number(strList[1]);
                global.cx = global.x1;
                global.cy = global.y1;
                addRubber("ELLIPSE");
                setRubberMode("ELLIPSE_LINE");
                setRubberPoint("ELLIPSE_LINE_POINT1", global.x1, global.y1);
                setRubberPoint("ELLIPSE_CENTER", global.cx, global.cy);
                setPromptPrefix(qsTr("Specify first axis end point: "));
            }
        }
        else if(isNaN(global.x2))
        {
            var strList = str.split(",");
            if(isNaN(strList[0]) || isNaN(strList[1]))
            {
                alert(qsTr("Invalid point."));
                setPromptPrefix(qsTr("Specify first axis end point: "));
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
                setPromptPrefix(qsTr("Specify second axis end point or [Rotation]: "));
            }
        }
        else if(isNaN(global.x3))
        {
            if(str == "R" || str == "ROTATION") //TODO: Probably should add additional qsTr calls here.
            {
                global.mode = global.mode_ELLIPSE_ROTATION;
                setPromptPrefix(qsTr("Specify ellipse rotation: "));
            }
            else
            {
                var strList = str.split(",");
                if(isNaN(strList[0]) || isNaN(strList[1]))
                {
                    alert(qsTr("Point or option keyword required."));
                    setPromptPrefix(qsTr("Specify second axis end point or [Rotation]: "));
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
    else if(global.mode == global.mode_ELLIPSE_ROTATION)
    {
        if(isNaN(global.x1))
        {
            error("ELLIPSE", qsTr("This should never happen."));
        }
        else if(isNaN(global.x2))
        {
            error("ELLIPSE", qsTr("This should never happen."));
        }
        else if(isNaN(global.x3))
        {
            if(isNaN(str))
            {
                alert(qsTr("Invalid angle. Input a numeric angle or pick a point."));
                setPromptPrefix(qsTr("Specify rotation: "));
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
#endif

#include "object-ellipse.h"
#include "object-data.h"

#include <QPainter>
#include <QStyleOption>
#include <QGraphicsScene>

EllipseObject::EllipseObject(qreal centerX, qreal centerY, qreal width, qreal height, QRgb rgb, QGraphicsItem* parent) : BaseObject(parent)
{
    qDebug("EllipseObject Constructor()");
    init(centerX, centerY, width, height, rgb, Qt::SolidLine); //TODO: getCurrentLineType
}

EllipseObject::EllipseObject(EllipseObject* obj, QGraphicsItem* parent) : BaseObject(parent)
{
    qDebug("EllipseObject Constructor()");
    if(obj)
    {
        init(obj->objectCenterX(), obj->objectCenterY(), obj->objectWidth(), obj->objectHeight(), obj->objectColorRGB(), Qt::SolidLine); //TODO: getCurrentLineType
        setRotation(obj->rotation());
    }
}

EllipseObject::~EllipseObject()
{
    qDebug("EllipseObject Destructor()");
}

void EllipseObject::init(qreal centerX, qreal centerY, qreal width, qreal height, QRgb rgb, Qt::PenStyle lineType)
{
    setData(OBJ_TYPE, type());
    setData(OBJ_NAME, OBJ_NAME_ELLIPSE);

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

void EllipseObject::setObjectSize(qreal width, qreal height)
{
    QRectF elRect = rect();
    elRect.setWidth(width);
    elRect.setHeight(height);
    elRect.moveCenter(QPointF(0,0));
    setRect(elRect);
}

void EllipseObject::setObjectCenter(const QPointF& center)
{
    setObjectCenter(center.x(), center.y());
}

void EllipseObject::setObjectCenter(qreal centerX, qreal centerY)
{
    setPos(centerX, centerY);
}

void EllipseObject::setObjectCenterX(qreal centerX)
{
    setX(centerX);
}

void EllipseObject::setObjectCenterY(qreal centerY)
{
    setY(centerY);
}

void EllipseObject::setObjectRadiusMajor(qreal radius)
{
    setObjectDiameterMajor(radius*2.0);
}

void EllipseObject::setObjectRadiusMinor(qreal radius)
{
    setObjectDiameterMinor(radius*2.0);
}

void EllipseObject::setObjectDiameterMajor(qreal diameter)
{
    QRectF elRect = rect();
    if(elRect.width() > elRect.height())
        elRect.setWidth(diameter);
    else
        elRect.setHeight(diameter);
    elRect.moveCenter(QPointF(0,0));
    setRect(elRect);
}

void EllipseObject::setObjectDiameterMinor(qreal diameter)
{
    QRectF elRect = rect();
    if(elRect.width() < elRect.height())
        elRect.setWidth(diameter);
    else
        elRect.setHeight(diameter);
    elRect.moveCenter(QPointF(0,0));
    setRect(elRect);
}

QPointF EllipseObject::objectQuadrant0() const
{
    qreal halfW = objectWidth()/2.0;
    qreal rot = radians(rotation());
    qreal x = halfW*qCos(rot);
    qreal y = halfW*qSin(rot);
    return objectCenter() + QPointF(x,y);
}

QPointF EllipseObject::objectQuadrant90() const
{
    qreal halfH = objectHeight()/2.0;
    qreal rot = radians(rotation()+90.0);
    qreal x = halfH*qCos(rot);
    qreal y = halfH*qSin(rot);
    return objectCenter() + QPointF(x,y);
}

QPointF EllipseObject::objectQuadrant180() const
{
    qreal halfW = objectWidth()/2.0;
    qreal rot = radians(rotation()+180.0);
    qreal x = halfW*qCos(rot);
    qreal y = halfW*qSin(rot);
    return objectCenter() + QPointF(x,y);
}

QPointF EllipseObject::objectQuadrant270() const
{
    qreal halfH = objectHeight()/2.0;
    qreal rot = radians(rotation()+270.0);
    qreal x = halfH*qCos(rot);
    qreal y = halfH*qSin(rot);
    return objectCenter() + QPointF(x,y);
}

void EllipseObject::updatePath()
{
    QPainterPath path;
    QRectF r = rect();
    path.arcMoveTo(r, 0);
    path.arcTo(r, 0, 360);
    //NOTE: Reverse the path so that the inside area isn't considered part of the ellipse
    path.arcTo(r, 0, -360);
    setObjectPath(path);
}

void EllipseObject::paint(QPainter* painter, const QStyleOptionGraphicsItem* option, QWidget* /*widget*/)
{
    QGraphicsScene* objScene = scene();
    if(!objScene) return;

    QPen paintPen = pen();
    painter->setPen(paintPen);
    updateRubber(painter);
    if(option->state & QStyle::State_Selected)  { paintPen.setStyle(Qt::DashLine); }
    if(objScene->property(ENABLE_LWT).toBool()) { paintPen = lineWeightPen(); }
    painter->setPen(paintPen);

    painter->drawEllipse(rect());
}

void EllipseObject::updateRubber(QPainter* painter)
{
    int rubberMode = objectRubberMode();
    if(rubberMode == OBJ_RUBBER_ELLIPSE_LINE)
    {
        QPointF sceneLinePoint1 = objectRubberPoint("ELLIPSE_LINE_POINT1");
        QPointF sceneLinePoint2 = objectRubberPoint("ELLIPSE_LINE_POINT2");
        QPointF itemLinePoint1  = mapFromScene(sceneLinePoint1);
        QPointF itemLinePoint2  = mapFromScene(sceneLinePoint2);
        QLineF itemLine(itemLinePoint1, itemLinePoint2);
        if(painter) drawRubberLine(itemLine, painter, VIEW_COLOR_CROSSHAIR);
        updatePath();
    }
    else if(rubberMode == OBJ_RUBBER_ELLIPSE_MAJORDIAMETER_MINORRADIUS)
    {
        QPointF sceneAxis1Point1 = objectRubberPoint("ELLIPSE_AXIS1_POINT1");
        QPointF sceneAxis1Point2 = objectRubberPoint("ELLIPSE_AXIS1_POINT2");
        QPointF sceneCenterPoint = objectRubberPoint("ELLIPSE_CENTER");
        QPointF sceneAxis2Point2 = objectRubberPoint("ELLIPSE_AXIS2_POINT2");
        qreal ellipseWidth = objectRubberPoint("ELLIPSE_WIDTH").x();
        qreal ellipseRot = objectRubberPoint("ELLIPSE_ROT").x();

        //TODO: incorporate perpendicularDistance() into libcgeometry
        qreal px = sceneAxis2Point2.x();
        qreal py = sceneAxis2Point2.y();
        qreal x1 = sceneAxis1Point1.x();
        qreal y1 = sceneAxis1Point1.y();
        QLineF line(sceneAxis1Point1, sceneAxis1Point2);
        QLineF norm = line.normalVector();
        qreal dx = px-x1;
        qreal dy = py-y1;
        norm.translate(dx, dy);
        QPointF iPoint;
        norm.intersects(line, &iPoint);
        qreal ellipseHeight = QLineF(px, py, iPoint.x(), iPoint.y()).length()*2.0;

        setObjectCenter(sceneCenterPoint);
        setObjectSize(ellipseWidth, ellipseHeight);
        setRotation(-ellipseRot);

        QPointF itemCenterPoint = mapFromScene(sceneCenterPoint);
        QPointF itemAxis2Point2 = mapFromScene(sceneAxis2Point2);
        QLineF itemLine(itemCenterPoint, itemAxis2Point2);
        if(painter) drawRubberLine(itemLine, painter, VIEW_COLOR_CROSSHAIR);
        updatePath();
    }
    else if(rubberMode == OBJ_RUBBER_ELLIPSE_MAJORRADIUS_MINORRADIUS)
    {
        QPointF sceneAxis1Point2 = objectRubberPoint("ELLIPSE_AXIS1_POINT2");
        QPointF sceneCenterPoint = objectRubberPoint("ELLIPSE_CENTER");
        QPointF sceneAxis2Point2 = objectRubberPoint("ELLIPSE_AXIS2_POINT2");
        qreal ellipseWidth = objectRubberPoint("ELLIPSE_WIDTH").x();
        qreal ellipseRot = objectRubberPoint("ELLIPSE_ROT").x();

        //TODO: incorporate perpendicularDistance() into libcgeometry
        qreal px = sceneAxis2Point2.x();
        qreal py = sceneAxis2Point2.y();
        qreal x1 = sceneCenterPoint.x();
        qreal y1 = sceneCenterPoint.y();
        QLineF line(sceneCenterPoint, sceneAxis1Point2);
        QLineF norm = line.normalVector();
        qreal dx = px-x1;
        qreal dy = py-y1;
        norm.translate(dx, dy);
        QPointF iPoint;
        norm.intersects(line, &iPoint);
        qreal ellipseHeight = QLineF(px, py, iPoint.x(), iPoint.y()).length()*2.0;

        setObjectCenter(sceneCenterPoint);
        setObjectSize(ellipseWidth, ellipseHeight);
        setRotation(-ellipseRot);

        QPointF itemCenterPoint = mapFromScene(sceneCenterPoint);
        QPointF itemAxis2Point2 = mapFromScene(sceneAxis2Point2);
        QLineF itemLine(itemCenterPoint, itemAxis2Point2);
        if(painter) drawRubberLine(itemLine, painter, VIEW_COLOR_CROSSHAIR);
        updatePath();
    }
    else if(rubberMode == OBJ_RUBBER_GRIP)
    {
        //TODO: updateRubber() gripping for EllipseObject
    }
}

void EllipseObject::vulcanize()
{
    qDebug("EllipseObject vulcanize()");
    updateRubber();

    setObjectRubberMode(OBJ_RUBBER_OFF);
}

// Returns the closest snap point to the mouse point
QPointF EllipseObject::mouseSnapPoint(const QPointF& mousePoint)
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

QList<QPointF> EllipseObject::allGripPoints()
{
    QList<QPointF> gripPoints;
    gripPoints << objectCenter() << objectQuadrant0() << objectQuadrant90() << objectQuadrant180() << objectQuadrant270();
    return gripPoints;
}

void EllipseObject::gripEdit(const QPointF& before, const QPointF& after)
{
    //TODO: gripEdit() for EllipseObject
}

QPainterPath EllipseObject::objectSavePath() const
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
