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
#include <QMessageBox>

/* NOTE: main() is run every time the command is started.
 *       Use it to reset variables so they are ready to go.
 */
ScriptValue
path_command(ScriptEnv * /* context */)
{
    return script_null;
}

#if 0
//Command: Path

//TODO: The path command is currently broken
var global = {}; //Required
bool firstRun;
EmbVector first;
EmbVector prev;

//NOTE: main() is run every time the command is started.
//      Use it to reset variables so they are ready to go.
function main()
{
    initCommand();
    clearSelection();
    global.firstRun = true;
    global.firstX = NaN;
    global.firstY = NaN;
    global.prevX = NaN;
    global.prevY = NaN;
    prompt_output(qsTr("Specify start point: "));
}

function click(EmbVector v)
{
    if (global.firstRun) {
        global.firstRun = false;
        global.first = v;
        global.prev = v;
        addPath(v.x, v.y);
        prompt_output(qsTr("Specify next point or [Arc/Undo]: "));
    }
    else {
        appendLineToPath(x,y);
        global.prev = v;
    }
}

function
context(str)
{
    todo("PATH", "context()");
}

function
prompt(str)
{
    if (str == "A" || str == "ARC") {
        //TODO: Probably should add additional qsTr calls here.
        todo("PATH", "prompt() for ARC");
    }
    else if (str == "U" || str == "UNDO") {
        //TODO: Probably should add additional qsTr calls here.
        todo("PATH", "prompt() for UNDO");
    }
    else {
        if (!parse_vector(str, &v)) {
            alert(qsTr("Point or option keyword required."));
            prompt_output(qsTr("Specify next point or [Arc/Undo]: "));
        }
        else {
            if (global.firstRun) {
                global.firstRun = false;
                global.first = v;
                global.prev = v;
                addPath(v.x, v.y);
                prompt_output(qsTr("Specify next point or [Arc/Undo]: "));
            }
            else {
                appendLineToPath(v.x, v.y);
                global.prev = v;
            }
        }
    }
}
#endif

PathObject::PathObject(qreal x, qreal y, const QPainterPath p, QRgb rgb, QGraphicsItem* parent) : BaseObject(parent)
{
    qDebug("PathObject Constructor()");
    init(x, y, p, rgb, Qt::SolidLine); //TODO: getCurrentLineType
}

PathObject::PathObject(PathObject* obj, QGraphicsItem* parent) : BaseObject(parent)
{
    qDebug("PathObject Constructor()");
    if (obj) {
        init(obj->objectX(), obj->objectY(), obj->objectCopyPath(), obj->objectColorRGB(), Qt::SolidLine); //TODO: getCurrentLineType
        setRotation(obj->rotation());
        setScale(obj->scale());
    }
}

PathObject::~PathObject()
{
    qDebug("PathObject Destructor()");
}

void PathObject::init(qreal x, qreal y, const QPainterPath& p, QRgb rgb, Qt::PenStyle lineType)
{
    setData(OBJ_TYPE, type());
    setData(OBJ_NAME, OBJ_NAME_PATH);

    //WARNING: DO NOT enable QGraphicsItem::ItemIsMovable. If it is enabled,
    //WARNING: and the item is double clicked, the scene will erratically move the item while zooming.
    //WARNING: All movement has to be handled explicitly by us, not by the scene.
    setFlag(QGraphicsItem::ItemIsSelectable, true);

    updatePath(p);
    setObjectPos(x,y);
    setObjectColor(rgb);
    setObjectLineType(lineType);
    setObjectLineWeight(0.35); //TODO: pass in proper lineweight
    setPen(objectPen());
}

void PathObject::updatePath(const QPainterPath& p)
{
    normalPath = p;
    QPainterPath reversePath = normalPath.toReversed();
    reversePath.connectPath(normalPath);
    setObjectPath(reversePath);
}

void PathObject::paint(QPainter* painter, const QStyleOptionGraphicsItem* option, QWidget* /*widget*/)
{
    QGraphicsScene* objScene = scene();
    if (!objScene) return;

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

    painter->drawPath(objectPath());
}

void PathObject::updateRubber(QPainter* painter)
{
    //TODO: Path Rubber Modes

    //TODO: updateRubber() gripping for PathObject

}

void PathObject::vulcanize()
{
    qDebug("PathObject vulcanize()");
    updateRubber();

    setObjectRubberMode(OBJ_RUBBER_OFF);

    if (!normalPath.elementCount())
        QMessageBox::critical(0, QObject::tr("Empty Path Error"), QObject::tr("The path added contains no points. The command that created this object has flawed logic."));
}

// Returns the closest snap point to the mouse point
QPointF PathObject::mouseSnapPoint(const QPointF& mousePoint)
{
    return scenePos();
}

QList<QPointF> PathObject::allGripPoints()
{
    QList<QPointF> gripPoints;
    gripPoints << scenePos(); //TODO: loop thru all path Elements and return their points
    return gripPoints;
}

void PathObject::gripEdit(const QPointF& before, const QPointF& after)
{
    //TODO: gripEdit() for PathObject
}

QPainterPath PathObject::objectCopyPath() const
{
    return normalPath;
}

QPainterPath PathObject::objectSavePath() const
{
    qreal s = scale();
    QTransform trans;
    trans.rotate(rotation());
    trans.scale(s,s);
    return trans.map(normalPath);
}
