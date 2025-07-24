/*!
 * \file object-circle.cpp
 * \brief Tools for drawing and manipulating EmbCircles via the GUI.
 */

#include "embroidermodder.h"

#include <QPainter>
#include <QStyleOption>
#include <QGraphicsScene>

CircleObject::CircleObject(qreal centerX, qreal centerY, qreal radius, QRgb rgb, QGraphicsItem* parent) : BaseObject(parent)
{
    debug("CircleObject Constructor()");
    init(centerX, centerY, radius, rgb, Qt::SolidLine); //TODO: getCurrentLineType
}

CircleObject::CircleObject(CircleObject* obj, QGraphicsItem* parent) : BaseObject(parent)
{
    debug("CircleObject Constructor()");
    if (obj) {
        init(obj->objectCenterX(), obj->objectCenterY(), obj->objectRadius(), obj->objectColorRGB(), Qt::SolidLine); //TODO: getCurrentLineType
        setRotation(obj->rotation());
    }
}

CircleObject::~CircleObject()
{
    debug("CircleObject Destructor()");
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
    if(option->state & QStyle::State_Selected)  { paintPen.setStyle(Qt::DashLine); }
    if(objScene->property(ENABLE_LWT).toBool()) { paintPen = lineWeightPen(); }
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

        EmbGeometry arc = emb_arc(
            sceneTan1Point.x(), sceneTan1Point.y(),
            sceneTan2Point.x(), sceneTan2Point.y(),
            sceneTan3Point.x(), sceneTan3Point.y());
        EmbError error = EMB_NO_ERR;
        EmbVector sceneCenter = emb_center(&arc, &error);
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

#if 0
-- Command: Circle

circle_global = {
  x1 = nil,
  y1 = nil,
  x2 = nil,
  y2 = nil,
  x3 = nil,
  y3 = nil,
  rad = nil,
  dia = nil,
  cx = nil,
  cy = nil,
  mode = nil
}

-- enums
CIRCLE_MODE_1P_RAD = 0
CIRCLE_MODE_1P_DIA = 1
CIRCLE_MODE_2P = 2
CIRCLE_MODE_3P = 3
CIRCLE_MODE_TTR = 4

-- NOTE: main() is run every time the command is started.
--       Use it to reset variables so they are ready to go.
function circle_main()
  init_command()
  clear_selection()
  circle_global.mode = CIRCLE_MODE_1P_RAD
  circle_global.x1 = nil
  circle_global.y1 = nil
  circle_global.x2 = nil
  circle_global.y2 = nil
  circle_global.x3 = nil
  circle_global.y3 = nil
  set_prompt_prefix(qsTr("Specify center point for circle or [3P/2P/Ttr (tan tan radius)]: "))
end

-- NOTE: click() is run only for left clicks.
--       Middle clicks are used for panning.
--       Right clicks bring up the context menu.
function click(x, y)
  if circle_global.mode == CIRCLE_MODE_1P_RAD then

    if circle_global.x1 == nil then
      global.x1 = x
      global.y1 = y
      global.cx = x
      global.cy = y
      add_rubber("CIRCLE")
      set_rubber_mode("CIRCLE_1P_RAD")
      set_rubber_point("CIRCLE_CENTER", circle_global.cx, circle_global.cy)
      append_prompt_history()
      set_prompt_prefix(qsTr("Specify radius of circle or [Diameter]: "))
    else
      circle_global.x2 = x
      circle_global.y2 = y
      setRubberPoint("CIRCLE_RADIUS", global.x2, global.y2)
      vulcanize()
      appendPromptHistory()
      endCommand()
    end

  elseif circle_global.mode == CIRCLE_MODE_1P_DIA then

    if circle_global.x1 == nil then
      error("CIRCLE", qsTr("This should never happen."))
    else
      global.x2 = x
      global.y2 = y
      setRubberPoint("CIRCLE_DIAMETER", global.x2, global.y2)
      vulcanize()
      appendPromptHistory()
      endCommand()
    end

  elseif global.mode == CIRCLE_MODE_2P then

    if circle_global.x1 == nil then
      global.x1 = x
      global.y1 = y
      addRubber("CIRCLE")
      setRubberMode("CIRCLE_2P")
      setRubberPoint("CIRCLE_TAN1", global.x1, global.y1)
      appendPromptHistory()
      setPromptPrefix(qsTr("Specify second end point of circle's diameter: "))
    elseif global.x2 == nil then
      global.x2 = x
      global.y2 = y
      setRubberPoint("CIRCLE_TAN2", global.x2, global.y2)
      vulcanize()
      appendPromptHistory()
      endCommand()
    else
      error("CIRCLE", qsTr("This should never happen."))
    end

  else if(global.mode == global.mode_3P)
    {
        if(isNaN(global.x1))
        {
            global.x1 = x
            global.y1 = y
            appendPromptHistory()
            setPromptPrefix(qsTr("Specify second point on circle: "))
        }
        else if(isNaN(global.x2))
        {
            global.x2 = x
            global.y2 = y
            addRubber("CIRCLE")
            setRubberMode("CIRCLE_3P")
            setRubberPoint("CIRCLE_TAN1", global.x1, global.y1)
            setRubberPoint("CIRCLE_TAN2", global.x2, global.y2)
            appendPromptHistory()
            setPromptPrefix(qsTr("Specify third point on circle: "))
        }
        else if(isNaN(global.x3))
        {
            global.x3 = x
            global.y3 = y
            setRubberPoint("CIRCLE_TAN3", global.x3, global.y3)
            vulcanize()
            appendPromptHistory()
            endCommand()
        }
        else
        {
            error("CIRCLE", qsTr("This should never happen."))
        }
    }
    else if(global.mode == global.mode_TTR)
    {
        if(isNaN(global.x1))
        {
            global.x1 = x
            global.y1 = y
            appendPromptHistory()
            setPromptPrefix(qsTr("Specify point on object for second tangent of circle: "))
        }
        else if(isNaN(global.x2))
        {
            global.x2 = x
            global.y2 = y
            appendPromptHistory()
            setPromptPrefix(qsTr("Specify radius of circle: "))
        }
        else if(isNaN(global.x3))
        {
            global.x3 = x
            global.y3 = y
            appendPromptHistory()
            setPromptPrefix(qsTr("Specify second point: "))
        }
        else
        {
            todo("CIRCLE", "click() for TTR")
        }
    }
end

-- NOTE: context() is run when a context menu entry is chosen.
function circle_context (str)
  todo("CIRCLE", "context()")
end

-- NOTE: prompt() is run when Enter is pressed.
--       appendPromptHistory is automatically called before prompt()
--       is called so calling it is only needed for erroneous input.
--       Any text in the command prompt is sent as an uppercase string.
function circle_prompt (str)
    if(global.mode == global.mode_1P_RAD)
    {
        if(isNaN(global.x1))
        {
            if(str == "2P") -- TODO: Probably should add additional qsTr calls here.
            {
                global.mode = global.mode_2P
                setPromptPrefix(qsTr("Specify first end point of circle's diameter: "))
            }
            else if(str == "3P") -- TODO: Probably should add additional qsTr calls here.
            {
                global.mode = global.mode_3P
                setPromptPrefix(qsTr("Specify first point of circle: "))
            }
            else if(str == "T" || str == "TTR") -- TODO: Probably should add additional qsTr calls here.
            {
                global.mode = global.mode_TTR
                setPromptPrefix(qsTr("Specify point on object for first tangent of circle: "))
            }
            else
            {
                var strList = str.split(",")
                if(isNaN(strList[0]) || isNaN(strList[1]))
                {
                    alert(qsTr("Point or option keyword required."))
                    setPromptPrefix(qsTr("Specify center point for circle or [3P/2P/Ttr (tan tan radius)]: "))
                }
                else
                {
                    global.x1 = Number(strList[0])
                    global.y1 = Number(strList[1])
                    global.cx = global.x1
                    global.cy = global.y1
                    addRubber("CIRCLE")
                    setRubberMode("CIRCLE_1P_RAD")
                    setRubberPoint("CIRCLE_CENTER", global.cx, global.cy)
                    setPromptPrefix(qsTr("Specify radius of circle or [Diameter]: "))
                }
            }
        }
        else
        {
            if(str == "D" || str == "DIAMETER") -- TODO: Probably should add additional qsTr calls here.
            {
                global.mode = global.mode_1P_DIA
                setRubberMode("CIRCLE_1P_DIA")
                setPromptPrefix(qsTr("Specify diameter of circle: "))
            }
            else
            {
                var num = Number(str)
                if(isNaN(num))
                {
                    alert(qsTr("Requires numeric radius, point on circumference, or \"D\"."))
                    setPromptPrefix(qsTr("Specify radius of circle or [Diameter]: "))
                }
                else
                {
                    global.rad = num
                    global.x2 = global.x1 + global.rad
                    global.y2 = global.y1
                    setRubberPoint("CIRCLE_RADIUS", global.x2, global.y2)
                    vulcanize()
                    endCommand()
                }
            }
        }
    }
    else if(global.mode == global.mode_1P_DIA)
    {
        if(isNaN(global.x1))
        {
            error("CIRCLE", qsTr("This should never happen."))
        }
        if(isNaN(global.x2))
        {
            var num = Number(str)
            if(isNaN(num))
            {
                alert(qsTr("Requires numeric distance or second point."))
                setPromptPrefix(qsTr("Specify diameter of circle: "))
            }
            else
            {
                global.dia = num
                global.x2 = global.x1 + global.dia
                global.y2 = global.y1
                setRubberPoint("CIRCLE_DIAMETER", global.x2, global.y2)
                vulcanize()
                endCommand()
            }
        }
        else
        {
            error("CIRCLE", qsTr("This should never happen."))
        }
    }
    else if(global.mode == global.mode_2P)
    {
        if(isNaN(global.x1))
        {
            var strList = str.split(",")
            if(isNaN(strList[0]) || isNaN(strList[1]))
            {
                alert(qsTr("Invalid point."))
                setPromptPrefix(qsTr("Specify first end point of circle's diameter: "))
            }
            else
            {
                global.x1 = Number(strList[0])
                global.y1 = Number(strList[1])
                addRubber("CIRCLE")
                setRubberMode("CIRCLE_2P")
                setRubberPoint("CIRCLE_TAN1", global.x1, global.y1)
                setPromptPrefix(qsTr("Specify second end point of circle's diameter: "))
            }
        }
        else if(isNaN(global.x2))
        {
            var strList = str.split(",")
            if(isNaN(strList[0]) || isNaN(strList[1]))
            {
                alert(qsTr("Invalid point."))
                setPromptPrefix(qsTr("Specify second end point of circle's diameter: "))
            }
            else
            {
                global.x2 = Number(strList[0])
                global.y2 = Number(strList[1])
                setRubberPoint("CIRCLE_TAN2", global.x2, global.y2)
                vulcanize()
                endCommand()
            }
        }
        else
        {
            error("CIRCLE", qsTr("This should never happen."))
        }
    }
    else if(global.mode == global.mode_3P)
    {
        if(isNaN(global.x1))
        {
            var strList = str.split(",")
            if(isNaN(strList[0]) || isNaN(strList[1]))
            {
                alert(qsTr("Invalid point."))
                setPromptPrefix(qsTr("Specify first point of circle: "))
            }
            else
            {
                global.x1 = Number(strList[0])
                global.y1 = Number(strList[1])
                setPromptPrefix(qsTr("Specify second point of circle: "))
            }
        }
        else if(isNaN(global.x2))
        {
            var strList = str.split(",")
            if(isNaN(strList[0]) || isNaN(strList[1]))
            {
                alert(qsTr("Invalid point."))
                setPromptPrefix(qsTr("Specify second point of circle: "))
            }
            else
            {
                global.x2 = Number(strList[0])
                global.y2 = Number(strList[1])
                addRubber("CIRCLE")
                setRubberMode("CIRCLE_3P")
                setRubberPoint("CIRCLE_TAN1", global.x1, global.y1)
                setRubberPoint("CIRCLE_TAN2", global.x2, global.y2)
                setPromptPrefix(qsTr("Specify third point of circle: "))
            }
        }
        else if(isNaN(global.x3))
        {
            var strList = str.split(",")
            if(isNaN(strList[0]) || isNaN(strList[1]))
            {
                alert(qsTr("Invalid point."))
                setPromptPrefix(qsTr("Specify third point of circle: "))
            }
            else
            {                
                global.x3 = Number(strList[0])
                global.y3 = Number(strList[1])
                setRubberPoint("CIRCLE_TAN3", global.x3, global.y3)
                vulcanize()
                endCommand()
            }
        }
        else
        {
            error("CIRCLE", qsTr("This should never happen."))
        }
        
    }
    else if(global.mode == global.mode_TTR)
    {
        todo("CIRCLE", "prompt() for TTR")
    }
end

#endif

