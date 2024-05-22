/*
 * Embroidermodder 2.
 *
 * Copyright 2013-2024 The Embroidermodder Team
 * Embroidermodder 2 is Open Source Software, see LICENSE for licensing terms.
 * Read CODE-STANDARDS.txt for advice on altering this file.
 *
 * Command: POINT
 */

#include "../commands.h"

/* NOTE: main() is run every time the command is started.
 *       Use it to reset variables so they are ready to go.
 */
ScriptValue
point_command(ScriptEnv * context)
{
    return script_null;
}

#if 0
//Command: Point

var global = {}; //Required
global.firstRun;

//NOTE: main() is run every time the command is started.
//      Use it to reset variables so they are ready to go.
function main()
{
    _main->nativeInitCommand();
    _main->nativeClearSelection();
    global.firstRun = true;
    setPromptPrefix("TODO: Current point settings: PDMODE=?  PDSIZE=?"); //TODO: qsTr needed here when complete
    appendPromptHistory();
    setPromptPrefix(qsTr("Specify first point: "));
}

//NOTE: click() is run only for left clicks.
//      Middle clicks are used for panning.
//      Right clicks bring up the context menu.
function click(x, y)
{
    if(global.firstRun)
    {
        global.firstRun = false;
        appendPromptHistory();
        setPromptPrefix(qsTr("Specify next point: "));
        addPoint(x,y);
    }
    else
    {
        appendPromptHistory();
        addPoint(x,y);
    }
}

//NOTE: context() is run when a context menu entry is chosen.
function context(str)
{
    todo("POINT", "context()");
}

//NOTE: prompt() is run when Enter is pressed.
//      appendPromptHistory is automatically called before prompt()
//      is called so calling it is only needed for erroneous input.
//      Any text in the command prompt is sent as an uppercase string.
function prompt(str)
{
    if(global.firstRun)
    {
        if(str == "M" || str == "MODE") //TODO: Probably should add additional qsTr calls here.
        {
            todo("POINT", "prompt() for PDMODE");
        }
        else if(str == "S" || str == "SIZE") //TODO: Probably should add additional qsTr calls here.
        {
            todo("POINT", "prompt() for PDSIZE");
        }
        var strList = str.split(",");
        if(isNaN(strList[0]) || isNaN(strList[1]))
        {
            alert(qsTr("Invalid point."));
            setPromptPrefix(qsTr("Specify first point: "));
        }
        else
        {
            global.firstRun = false;
            var x = Number(strList[0]);
            var y = Number(strList[1]);
            setPromptPrefix(qsTr("Specify next point: "));
            addPoint(x,y);
        }
    }
    else
    {
        var strList = str.split(",");
        if(isNaN(strList[0]) || isNaN(strList[1]))
        {
            alert(qsTr("Invalid point."));
            setPromptPrefix(qsTr("Specify next point: "));
        }
        else
        {
            var x = Number(strList[0]);
            var y = Number(strList[1]);
            setPromptPrefix(qsTr("Specify next point: "));
            addPoint(x,y);
        }
    }
}
[Menu]
Name=Draw
Position=10

[ToolBar]
Name=Draw
Position=10

[Tips]
ToolTip=&Point
StatusTip=Creates multiple points:  POINT

[Prompt]
Alias=PO, POINT
#endif

#include "object-point.h"
#include "object-data.h"

#include <QPainter>
#include <QStyleOption>
#include <QGraphicsScene>

PointObject::PointObject(qreal x, qreal y, QRgb rgb, QGraphicsItem* parent) : BaseObject(parent)
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
    if(!objScene) return;

    QPen paintPen = pen();
    painter->setPen(paintPen);
    updateRubber(painter);
    if(option->state & QStyle::State_Selected)  { paintPen.setStyle(Qt::DashLine); }
    if(objScene->property(ENABLE_LWT).toBool()) { paintPen = lineWeightPen(); }
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
    if(before == scenePos()) { QPointF delta = after-before; moveBy(delta.x(), delta.y()); }
}

QPainterPath PointObject::objectSavePath() const
{
    QPainterPath path;
    path.addRect(-0.00000001, -0.00000001, 0.00000002, 0.00000002);
    return path;
}

/* kate: bom off; indent-mode cstyle; indent-width 4; replace-trailing-space-save on; */
