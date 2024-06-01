/*
 * Embroidermodder 2.
 *
 * Copyright 2011-2024 The Embroidermodder Team
 * Embroidermodder 2 is Open Source Software, see LICENSE.md for licensing terms.
 * Visit https://www.libembroidery.org/refman for advice on altering this file,
 * or read the markdown version in embroidermodder2/docs/refman.
 *
 * Rect
 */

#include "embroidermodder.h"

#include <QPainter>
#include <QStyleOption>
#include <QGraphicsScene>

/* NOTE: main() is run every time the command is started.
 *       Use it to reset variables so they are ready to go.
 */
ScriptValue
rectangle_command(ScriptEnv * context)
{
    _main->nativeInitCommand();
    _main->nativeClearSelection();

    _main->nativeEndCommand();
    return script_null;
}

#if 0
//Command: Rectangle

var global = {}; //Required
global.newRect;
global.x1;
global.y1;
global.x2;
global.y2;

//NOTE: main() is run every time the command is started.
//      Use it to reset variables so they are ready to go.
function main()
{
    _main->nativeInitCommand();
    _main->nativeClearSelection();
    global.newRect = true;
    global.x1 = NaN;
    global.y1 = NaN;
    global.x2 = NaN;
    global.y2 = NaN;
    setPromptPrefix(qsTr("Specify first corner point or [Chamfer/Fillet]: "));
}

//NOTE: click() is run only for left clicks.
//      Middle clicks are used for panning.
//      Right clicks bring up the context menu.
function click(x, y)
{
    if(global.newRect)
    {
        global.newRect = false;
        global.x1 = x;
        global.y1 = y;
        addRubber("RECTANGLE");
        setRubberMode("RECTANGLE");
        setRubberPoint("RECTANGLE_START", x, y);
        setPromptPrefix(qsTr("Specify other corner point or [Dimensions]: "));
    }
    else
    {
        global.newRect = true;
        global.x2 = x;
        global.y2 = y;
        setRubberPoint("RECTANGLE_END", x, y);
        vulcanize();
        _main->nativeEndCommand();
    }
}

//NOTE: context() is run when a context menu entry is chosen.
function context(str)
{
    todo("RECTANGLE", "context()");
}

//NOTE: prompt() is run when Enter is pressed.
//      appendPromptHistory is automatically called before prompt()
//      is called so calling it is only needed for erroneous input.
//      Any text in the command prompt is sent as an uppercase string.
function prompt(str)
{
    if(str == "C" || str == "CHAMFER") //TODO: Probably should add additional qsTr calls here.
    {
        todo("RECTANGLE", "prompt() for CHAMFER");
    }
    else if(str == "D" || str == "DIMENSIONS") //TODO: Probably should add additional qsTr calls here.
    {
        todo("RECTANGLE", "prompt() for DIMENSIONS");
    }
    else if(str == "F" || str == "FILLET") //TODO: Probably should add additional qsTr calls here.
    {
        todo("RECTANGLE", "prompt() for FILLET");
    }
    else
    {
        var strList = str.split(",");
        if(isNaN(strList[0]) || isNaN(strList[1]))
        {
            alert(qsTr("Invalid point."));
            setPromptPrefix(qsTr("Specify first point: "));
        }
        else
        {
            var x = Number(strList[0]);
            var y = Number(strList[1]);
            if(global.newRect)
            {
                global.newRect = false;
                global.x1 = x;
                global.y1 = y;
                addRubber("RECTANGLE");
                setRubberMode("RECTANGLE");
                setRubberPoint("RECTANGLE_START", x, y);
                setPromptPrefix(qsTr("Specify other corner point or [Dimensions]: "));
            }
            else
            {
                global.newRect = true;
                global.x2 = x;
                global.y2 = y;
                setRubberPoint("RECTANGLE_END", x, y);
                vulcanize();
                _main->nativeEndCommand();
            }
        }
    }
}
#endif

RectObject::RectObject(qreal x, qreal y, qreal w, qreal h, QRgb rgb, QGraphicsItem* parent) : BaseObject(parent)
{
    qDebug("RectObject Constructor()");
    init(x, y, w, h, rgb, Qt::SolidLine); //TODO: getCurrentLineType
}

RectObject::RectObject(RectObject* obj, QGraphicsItem* parent) : BaseObject(parent)
{
    qDebug("RectObject Constructor()");
    if(obj)
    {
        QPointF ptl = obj->objectTopLeft();
        init(ptl.x(), ptl.y(), obj->objectWidth(), obj->objectHeight(), obj->objectColorRGB(), Qt::SolidLine); //TODO: getCurrentLineType
        setRotation(obj->rotation());
    }
}

RectObject::~RectObject()
{
    qDebug("RectObject Destructor()");
}

void RectObject::init(qreal x, qreal y, qreal w, qreal h, QRgb rgb, Qt::PenStyle lineType)
{
    setData(OBJ_TYPE, type());
    setData(OBJ_NAME, OBJ_NAME_RECTANGLE);

    //WARNING: DO NOT enable QGraphicsItem::ItemIsMovable. If it is enabled,
    //WARNING: and the item is double clicked, the scene will erratically move the item while zooming.
    //WARNING: All movement has to be handled explicitly by us, not by the scene.
    setFlag(QGraphicsItem::ItemIsSelectable, true);

    setObjectRect(x, y, w, h);
    setObjectColor(rgb);
    setObjectLineType(lineType);
    setObjectLineWeight(0.35); //TODO: pass in proper lineweight
    setPen(objectPen());
}

void RectObject::setObjectRect(qreal x, qreal y, qreal w, qreal h)
{
    setPos(x, y);
    setRect(0, 0, w, h);
    updatePath();
}

QPointF RectObject::objectTopLeft() const
{
    qreal rot = radians(rotation());
    qreal cosRot = qCos(rot);
    qreal sinRot = qSin(rot);

    QPointF tl = rect().topLeft();
    qreal ptlX = tl.x()*scale();
    qreal ptlY = tl.y()*scale();
    qreal ptlXrot = ptlX*cosRot - ptlY*sinRot;
    qreal ptlYrot = ptlX*sinRot + ptlY*cosRot;

    return (scenePos() + QPointF(ptlXrot, ptlYrot));
}

QPointF RectObject::objectTopRight() const
{
    qreal rot = radians(rotation());
    qreal cosRot = qCos(rot);
    qreal sinRot = qSin(rot);

    QPointF tr = rect().topRight();
    qreal ptrX = tr.x()*scale();
    qreal ptrY = tr.y()*scale();
    qreal ptrXrot = ptrX*cosRot - ptrY*sinRot;
    qreal ptrYrot = ptrX*sinRot + ptrY*cosRot;

    return (scenePos() + QPointF(ptrXrot, ptrYrot));
}

QPointF RectObject::objectBottomLeft() const
{
    qreal rot = radians(rotation());
    qreal cosRot = qCos(rot);
    qreal sinRot = qSin(rot);

    QPointF bl = rect().bottomLeft();
    qreal pblX = bl.x()*scale();
    qreal pblY = bl.y()*scale();
    qreal pblXrot = pblX*cosRot - pblY*sinRot;
    qreal pblYrot = pblX*sinRot + pblY*cosRot;

    return (scenePos() + QPointF(pblXrot, pblYrot));
}

QPointF RectObject::objectBottomRight() const
{
    qreal rot = radians(rotation());
    qreal cosRot = qCos(rot);
    qreal sinRot = qSin(rot);

    QPointF br = rect().bottomRight();
    qreal pbrX = br.x()*scale();
    qreal pbrY = br.y()*scale();
    qreal pbrXrot = pbrX*cosRot - pbrY*sinRot;
    qreal pbrYrot = pbrX*sinRot + pbrY*cosRot;

    return (scenePos() + QPointF(pbrXrot, pbrYrot));
}

void RectObject::updatePath()
{
    QPainterPath path;
    QRectF r = rect();
    path.moveTo(r.bottomLeft());
    path.lineTo(r.bottomRight());
    path.lineTo(r.topRight());
    path.lineTo(r.topLeft());
    path.lineTo(r.bottomLeft());
    //NOTE: Reverse the path so that the inside area isn't considered part of the rectangle
    path.lineTo(r.topLeft());
    path.lineTo(r.topRight());
    path.lineTo(r.bottomRight());
    path.moveTo(r.bottomLeft());
    setObjectPath(path);
}

void RectObject::paint(QPainter* painter, const QStyleOptionGraphicsItem* option, QWidget* /*widget*/)
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

    painter->drawRect(rect());
}

void RectObject::updateRubber(QPainter* painter)
{
    int rubberMode = objectRubberMode();
    if(rubberMode == OBJ_RUBBER_RECTANGLE)
    {
        QPointF sceneStartPoint = objectRubberPoint("RECTANGLE_START");
        QPointF sceneEndPoint = objectRubberPoint("RECTANGLE_END");
        qreal x = sceneStartPoint.x();
        qreal y = sceneStartPoint.y();
        qreal w = sceneEndPoint.x() - sceneStartPoint.x();
        qreal h = sceneEndPoint.y() - sceneStartPoint.y();
        setObjectRect(x,y,w,h);
        updatePath();
    }
    else if(rubberMode == OBJ_RUBBER_GRIP)
    {
        if(painter)
        {
            //TODO: Make this work with rotation & scaling
            /*
            QPointF gripPoint = objectRubberPoint("GRIP_POINT");
            QPointF after = objectRubberPoint(QString());
            QPointF delta = after-gripPoint;
            if     (gripPoint == objectTopLeft())     { painter->drawPolygon(mapFromScene(QRectF(after.x(), after.y(), objectWidth()-delta.x(), objectHeight()-delta.y()))); }
            else if(gripPoint == objectTopRight())    { painter->drawPolygon(mapFromScene(QRectF(objectTopLeft().x(), objectTopLeft().y()+delta.y(), objectWidth()+delta.x(), objectHeight()-delta.y()))); }
            else if(gripPoint == objectBottomLeft())  { painter->drawPolygon(mapFromScene(QRectF(objectTopLeft().x()+delta.x(), objectTopLeft().y(), objectWidth()-delta.x(), objectHeight()+delta.y()))); }
            else if(gripPoint == objectBottomRight()) { painter->drawPolygon(mapFromScene(QRectF(objectTopLeft().x(), objectTopLeft().y(), objectWidth()+delta.x(), objectHeight()+delta.y()))); }

            QLineF rubLine(mapFromScene(gripPoint), mapFromScene(objectRubberPoint(QString())));
            drawRubberLine(rubLine, painter, VIEW_COLOR_CROSSHAIR);
            */

            QPointF gripPoint = objectRubberPoint("GRIP_POINT");
            QPointF after = objectRubberPoint(QString());
            QPointF delta = after-gripPoint;

            QLineF rubLine(mapFromScene(gripPoint), mapFromScene(objectRubberPoint(QString())));
            drawRubberLine(rubLine, painter, VIEW_COLOR_CROSSHAIR);
        }
    }
}

void RectObject::vulcanize()
{
    qDebug("RectObject vulcanize()");
    updateRubber();

    setObjectRubberMode(OBJ_RUBBER_OFF);
}

// Returns the closest snap point to the mouse point
QPointF RectObject::mouseSnapPoint(const QPointF& mousePoint)
{
    QPointF ptl = objectTopLeft();     //Top Left Corner QSnap
    QPointF ptr = objectTopRight();    //Top Right Corner QSnap
    QPointF pbl = objectBottomLeft();  //Bottom Left Corner QSnap
    QPointF pbr = objectBottomRight(); //Bottom Right Corner QSnap

    qreal ptlDist = QLineF(mousePoint, ptl).length();
    qreal ptrDist = QLineF(mousePoint, ptr).length();
    qreal pblDist = QLineF(mousePoint, pbl).length();
    qreal pbrDist = QLineF(mousePoint, pbr).length();

    qreal minDist = qMin(qMin(ptlDist, ptrDist), qMin(pblDist, pbrDist));

    if     (minDist == ptlDist) return ptl;
    else if(minDist == ptrDist) return ptr;
    else if(minDist == pblDist) return pbl;
    else if(minDist == pbrDist) return pbr;

    return scenePos();
}

QList<QPointF> RectObject::allGripPoints()
{
    QList<QPointF> gripPoints;
    gripPoints << objectTopLeft() << objectTopRight() << objectBottomLeft() << objectBottomRight();
    return gripPoints;
}

void RectObject::gripEdit(const QPointF& before, const QPointF& after)
{
    QPointF delta = after-before;
    if     (before == objectTopLeft())     { setObjectRect(after.x(), after.y(), objectWidth()-delta.x(), objectHeight()-delta.y()); }
    else if(before == objectTopRight())    { setObjectRect(objectTopLeft().x(), objectTopLeft().y()+delta.y(), objectWidth()+delta.x(), objectHeight()-delta.y()); }
    else if(before == objectBottomLeft())  { setObjectRect(objectTopLeft().x()+delta.x(), objectTopLeft().y(), objectWidth()-delta.x(), objectHeight()+delta.y()); }
    else if(before == objectBottomRight()) { setObjectRect(objectTopLeft().x(), objectTopLeft().y(), objectWidth()+delta.x(), objectHeight()+delta.y()); }
}

QPainterPath RectObject::objectSavePath() const
{
    QPainterPath path;
    QRectF r = rect();
    path.moveTo(r.bottomLeft());
    path.lineTo(r.bottomRight());
    path.lineTo(r.topRight());
    path.lineTo(r.topLeft());
    path.lineTo(r.bottomLeft());

    qreal s = scale();
    QTransform trans;
    trans.rotate(rotation());
    trans.scale(s,s);
    return trans.map(path);
}
