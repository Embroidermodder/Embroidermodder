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
[Menu]
Name=Draw
Position=0

[ToolBar]
Name=Draw
Position=0

[Tips]
ToolTip=&Line
StatusTip=Creates straight line segments:  LINE

[Prompt]
Alias=L, LINE

//Command: Line

var global = {}; //Required
global.firstRun;
global.firstX;
global.firstY;
global.prevX;
global.prevY;

void main()
{
    initCommand();
    clearSelection();
    global.firstRun = true;
    global.firstX = NaN;
    global.firstY = NaN;
    global.prevX = NaN;
    global.prevY = NaN;
    setPromptPrefix(qsTr("Specify first point: "));
}

void
click(EmbVector p)
{
    if (global.firstRun)
    {
        global.firstRun = false;
        global.firstX = p.x;
        global.firstY = p.y;
        global.prevX = p.x;
        global.prevY = p.y;
        addRubber("LINE");
        setRubberMode("LINE");
        setRubberPoint("LINE_START", global.firstX, global.firstY);
        appendPromptHistory();
        setPromptPrefix(qsTr("Specify next point or [Undo]: "));
    }
    else
    {
        setRubberPoint("LINE_END", x, y);
        vulcanize();
        addRubber("LINE");
        setRubberMode("LINE");
        setRubberPoint("LINE_START", x, y);
        appendPromptHistory();
        global.prevX = x;
        global.prevY = y;
    }
}

//NOTE: context() is run when a context menu entry is chosen.
function context(str)
{
    todo("LINE", "context()");
}

void
prompt(std::string str)
{
    if (global.firstRun)
    {
        var strList = str.split(",");
        if (isNaN(strList[0]) || isNaN(strList[1]))
        {
            alert(qsTr("Invalid point."));
            setPromptPrefix(qsTr("Specify first point: "));
        }
        else {
            global.firstRun = false;
            global.firstX = Number(strList[0]);
            global.firstY = Number(strList[1]);
            global.prevX = global.firstX;
            global.prevY = global.firstY;
            addRubber("LINE");
            setRubberMode("LINE");
            setRubberPoint("LINE_START", global.firstX, global.firstY);
            setPromptPrefix(qsTr("Specify next point or [Undo]: "));
        }
    }
    else {
        if (str == "U" || str == "UNDO") {
            todo("LINE", "prompt() for UNDO");
        }
        else {
            var strList = str.split(",");
            if (isNaN(strList[0]) || isNaN(strList[1]))
            {
                alert(qsTr("Point or option keyword required."));
                setPromptPrefix(qsTr("Specify next point or [Undo]: "));
            }
            else {
                var x = Number(strList[0]);
                var y = Number(strList[1]);
                setRubberPoint("LINE_END", x, y);
                vulcanize();
                addRubber("LINE");
                setRubberMode("LINE");
                setRubberPoint("LINE_START", x, y);
                global.prevX = x;
                global.prevY = y;
                setPromptPrefix(qsTr("Specify next point or [Undo]: "));
            }
        }
    }
}

void line_init(EmbLine line_in, unsigned int rgb, PenStyle lineType)
{
    setData(OBJ_TYPE, type);
    setData(OBJ_NAME, "Line");

    line = line_in;

    //WARNING: DO NOT enable QGraphicsItem::ItemIsMovable. If it is enabled,
    //WARNING: and the item is double clicked, the scene will erratically move the item while zooming.
    //WARNING: All movement has to be handled explicitly by us, not by the scene.
    setFlag(QGraphicsItem::ItemIsSelectable, true);

    setObjectColor(rgb);
    setObjectLineType(lineType);
    setObjectLineWeight(0.35); //TODO: pass in proper lineweight
    setPen(objPen);
}

void line_setObjectEndPoint1(EmbVector point1)
{
    double dx = line.start.x - point1.x;
    double dy = line.start.y - point1.y;
    setRotation(0);
    setScale(1);
    setLine(0, 0, dx, dy);
    setPos(point1);
}

void line_setObjectEndPoint2(EmbVector point1)
{
    double dx = line.end.x - point1.x;
    double dy = line.end.y - point1.y;
    setRotation(0);
    setScale(1);
    setLine(0, 0, dx, dy);
    setPos(point1);
}

EmbVector line_objectEndPoint2() const
{
    EmbLine lyne = line();
    double alpha = radians(rotation());
    EmbVector point2;
    point2.x = lyne.x2()*scale();
    point2.y = lyne.y2()*scale();
    EmbVector rotEnd = embVector_rotate(point2, alpha);

    return scenePos() + rotEnd;
}

EmbVector line_objectMidPoint() const
{
    EmbLine lyne = line();
    EmbVector mp = lyne.pointAt(0.5) * scale();
    double alpha = radians(rotation());
    EmbVector rotMid = embVector_rotate(mp, alpha);

    return scenePos() + rotMid;
}

double line_objectAngle() const
{
    return std::fmodf(line().angle() - rotation(), 360.0);
}

void line_paint(QPainter* painter, QStyleOptionGraphicsItem* option, QWidget* /*widget*/)
{
    QGraphicsScene* objScene = scene();
    if (!objScene) return;

    QPen paintPen = pen();
    painter->setPen(paintPen);
    updateRubber(painter);
    if (option->state & QStyle::State_Selected)  {
        paintPen.setStyle(Qt::DashLine);
    }
    if (objScene->property(ENABLE_LWT).toBool()) {
        paintPen = lineWeightPen();
    }
    painter->setPen(paintPen);

    if (objectRubberMode() != OBJ_RUBBER_LINE) painter->drawLine(line());

    if (objScene->property(ENABLE_LWT).toBool() && objScene->property("ENABLE_REAL").toBool()) {
        realRender(painter, path());
    }
}

void line_updateRubber(QPainter* painter)
{
    int rubberMode = objectRubberMode();
    if (rubberMode == OBJ_RUBBER_LINE) {
        EmbVector sceneStartPoint = objectRubberPoint("LINE_START");
        EmbVector sceneQSnapPoint = objectRubberPoint("LINE_END");

        setObjectEndPoint1(sceneStartPoint);
        setObjectEndPoint2(sceneQSnapPoint);

        drawRubberLine(line(), painter, VIEW_COLOR_CROSSHAIR);
    }
    else if (rubberMode == OBJ_RUBBER_GRIP) {
        if (painter) {
            EmbVector gripPoint = objectRubberPoint("GRIP_POINT");
            if     (gripPoint == objectEndPoint1()) painter->drawLine(line().p2(), mapFromScene(objectRubberPoint(std::string())));
            else if (gripPoint == objectEndPoint2()) painter->drawLine(line().p1(), mapFromScene(objectRubberPoint(std::string())));
            else if (gripPoint == objectMidPoint())  painter->drawLine(line().translated(mapFromScene(objectRubberPoint(std::string()))-mapFromScene(gripPoint)));

            EmbLine rubLine(mapFromScene(gripPoint), mapFromScene(objectRubberPoint(std::string())));
            drawRubberLine(rubLine, painter, VIEW_COLOR_CROSSHAIR);
        }
    }
}

void line_vulcanize()
{
    debug_message("LineObject vulcanize()");
    updateRubber();

    setObjectRubberMode(OBJ_RUBBER_OFF);
}

// Returns the closest snap point to the mouse point
EmbVector line_mouseSnapPoint(EmbVector& mousePoint)
{
    EmbVector endPoint1 = objectEndPoint1();
    EmbVector endPoint2 = objectEndPoint2();
    EmbVector midPoint  = objectMidPoint();

    double end1Dist = EmbLine(mousePoint, endPoint1).length();
    double end2Dist = EmbLine(mousePoint, endPoint2).length();
    double midDist  = EmbLine(mousePoint, midPoint).length();

    double minDist = std::min(std::min(end1Dist, end2Dist), midDist);

    if     (minDist == end1Dist) return endPoint1;
    else if (minDist == end2Dist) return endPoint2;
    else if (minDist == midDist)  return midPoint;

    return scenePos();
}

std::vector<EmbVector> line_allGripPoints()
{
    std::vector<EmbVector> gripPoints;
    gripPoints << objectEndPoint1() << objectEndPoint2() << objectMidPoint();
    return gripPoints;
}

void line_gripEdit(EmbVector& before, EmbVector& after)
{
    if     (before == objectEndPoint1()) { setObjectEndPoint1(after); }
    else if (before == objectEndPoint2()) { setObjectEndPoint2(after); }
    else if (before == objectMidPoint())  { EmbVector delta = after-before; moveBy(delta.x(), delta.y()); }
}

QPainterPath line_objectSavePath() const
{
    QPainterPath path;
    path.lineTo(objectDeltaX(), objectDeltaY());
    return path;
}
#endif
