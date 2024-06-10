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
polygon_command(ScriptEnv * context)
{
    init_command();
    clear_selection();

    end_command();
    return script_null;
}

#if 0
//Command: Polygon

var global = {}; //Required
global.centerX;
global.centerY;
global.sideX1;
global.sideY1;
global.sideX2;
global.sideY2;
global.pointIX;
global.pointIY;
global.pointCX;
global.pointCY;
global.polyType = "Inscribed"; //Default
global.numSides = 4;           //Default
global.mode;

//enums
global.mode_NUM_SIDES    = 0;
global.mode_CENTER_PT    = 1;
global.mode_POLYTYPE     = 2;
global.mode_INSCRIBE     = 3;
global.mode_CIRCUMSCRIBE = 4;
global.mode_DISTANCE     = 5;
global.mode_SIDE_LEN     = 6;

//NOTE: main() is run every time the command is started.
//      Use it to reset variables so they are ready to go.
function main()
{
    _main->nativeInitCommand();
    _main->nativeClearSelection();
    global.centerX = NaN;
    global.centerY = NaN;
    global.sideX1  = NaN;
    global.sideY1  = NaN;
    global.sideX2  = NaN;
    global.sideY2  = NaN;
    global.pointIX = NaN;
    global.pointIY = NaN;
    global.pointCX = NaN;
    global.pointCY = NaN;
    global.mode = global.mode_NUM_SIDES;
    setPromptPrefix(qsTr("Enter number of sides") + " {" + global.numSides.toString() + "}: ");
}

//NOTE: click() is run only for left clicks.
//      Middle clicks are used for panning.
//      Right clicks bring up the context menu.
function click(x, y)
{
    if(global.mode == global.mode_NUM_SIDES)
    {
        //Do nothing, the prompt controls this.
    }
    else if(global.mode == global.mode_CENTER_PT)
    {
        global.centerX = x;
        global.centerY = y;
        global.mode = global.mode_POLYTYPE;
        appendPromptHistory();
        setPromptPrefix(qsTr("Specify polygon type [Inscribed in circle/Circumscribed around circle]") + " {" + global.polyType + "}: ");
    }
    else if(global.mode == global.mode_POLYTYPE)
    {
        //Do nothing, the prompt controls this.
    }
    else if(global.mode == global.mode_INSCRIBE)
    {
        global.pointIX = x;
        global.pointIY = y;
        setRubberPoint("POLYGON_INSCRIBE_POINT", global.pointIX, global.pointIY);
        vulcanize();
        appendPromptHistory();
        _main->nativeEndCommand();
    }
    else if(global.mode == global.mode_CIRCUMSCRIBE)
    {
        global.pointCX = x;
        global.pointCY = y;
        setRubberPoint("POLYGON_CIRCUMSCRIBE_POINT", global.pointCX, global.pointCY);
        vulcanize();
        appendPromptHistory();
        _main->nativeEndCommand();
    }
    else if(global.mode == global.mode_DISTANCE)
    {
        //Do nothing, the prompt controls this.
    }
    else if(global.mode == global.mode_SIDE_LEN)
    {
        todo("POLYGON", "Sidelength mode");
    }
}

//NOTE: context() is run when a context menu entry is chosen.
function context(str)
{
    todo("POLYGON", "context()");
}

//NOTE: prompt() is run when Enter is pressed.
//      appendPromptHistory is automatically called before prompt()
//      is called so calling it is only needed for erroneous input.
//      Any text in the command prompt is sent as an uppercase string.
function prompt(str)
{
    if(global.mode == global.mode_NUM_SIDES)
    {
        if(str == "" && global.numSides >= 3 && global.numSides <= 1024)
        {
            setPromptPrefix(qsTr("Specify center point or [Sidelength]: "));
            global.mode = global.mode_CENTER_PT;
        }
        else
        {
            var tmp = Number(str);
            if(isNaN(tmp) || !isInt(tmp) || tmp < 3 || tmp > 1024)
            {
                alert(qsTr("Requires an integer between 3 and 1024."));
                setPromptPrefix(qsTr("Enter number of sides") + " {" + global.numSides.toString() + "}: ");
            }
            else
            {
                global.numSides = tmp;
                setPromptPrefix(qsTr("Specify center point or [Sidelength]: "));
                global.mode = global.mode_CENTER_PT;
            }
        }
    }
    else if(global.mode == global.mode_CENTER_PT)
    {
        if(str == "S" || str == "SIDELENGTH") //TODO: Probably should add additional qsTr calls here.
        {
            global.mode = global.mode_SIDE_LEN;
            setPromptPrefix(qsTr("Specify start point: "));
        }
        else
        {
            var strList = str.split(",");
            if(isNaN(strList[0]) || isNaN(strList[1]))
            {
                alert(qsTr("Point or option keyword required."));
                setPromptPrefix(qsTr("Specify center point or [Sidelength]: "));
            }
            else
            {
                global.centerX = Number(strList[0]);
                global.centerY = Number(strList[1]);
                global.mode = global.mode_POLYTYPE;
                setPromptPrefix(qsTr("Specify polygon type [Inscribed in circle/Circumscribed around circle]") + " {" + global.polyType + "}: ");
            }
        }
    }
    else if(global.mode == global.mode_POLYTYPE)
    {
        if(str == "I"        ||
           str == "IN"       ||
           str == "INS"      ||
           str == "INSC"     ||
           str == "INSCR"    ||
           str == "INSCRI"   ||
           str == "INSCRIB"  ||
           str == "INSCRIBE" ||
           str == "INSCRIBED") //TODO: Probably should add additional qsTr calls here.
        {
            global.mode = global.mode_INSCRIBE;
            global.polyType = "Inscribed";
            setPromptPrefix(qsTr("Specify polygon corner point or [Distance]: "));
            addRubber("POLYGON");
            setRubberMode("POLYGON_INSCRIBE");
            setRubberPoint("POLYGON_CENTER", global.centerX, global.centerY);
            setRubberPoint("POLYGON_NUM_SIDES", global.numSides, 0);
        }
        else if(str == "C"            ||
                str == "CI"           ||
                str == "CIR"          ||
                str == "CIRC"         ||
                str == "CIRCU"        ||
                str == "CIRCUM"       ||
                str == "CIRCUMS"      ||
                str == "CIRCUMSC"     ||
                str == "CIRCUMSCR"    ||
                str == "CIRCUMSCRI"   ||
                str == "CIRCUMSCRIB"  ||
                str == "CIRCUMSCRIBE" ||
                str == "CIRCUMSCRIBED") //TODO: Probably should add additional qsTr calls here.
        {
            global.mode = global.mode_CIRCUMSCRIBE;
            global.polyType = "Circumscribed";
            setPromptPrefix(qsTr("Specify polygon side point or [Distance]: "));
            addRubber("POLYGON");
            setRubberMode("POLYGON_CIRCUMSCRIBE");
            setRubberPoint("POLYGON_CENTER", global.centerX, global.centerY);
            setRubberPoint("POLYGON_NUM_SIDES", global.numSides, 0);
        }
        else if(str == "")
        {
            if(global.polyType == "Inscribed")
            {
                global.mode = global.mode_INSCRIBE;
                setPromptPrefix(qsTr("Specify polygon corner point or [Distance]: "));
                addRubber("POLYGON");
                setRubberMode("POLYGON_INSCRIBE");
                setRubberPoint("POLYGON_CENTER", global.centerX, global.centerY);
                setRubberPoint("POLYGON_NUM_SIDES", global.numSides, 0);
            }
            else if(global.polyType == "Circumscribed")
            {
                global.mode = global.mode_CIRCUMSCRIBE;
                setPromptPrefix(qsTr("Specify polygon side point or [Distance]: "));
                addRubber("POLYGON");
                setRubberMode("POLYGON_CIRCUMSCRIBE");
                setRubberPoint("POLYGON_CENTER", global.centerX, global.centerY);
                setRubberPoint("POLYGON_NUM_SIDES", global.numSides, 0);
            }
            else
            {
                error("POLYGON", qsTr("Polygon type is not Inscribed or Circumscribed."));
            }
        }
        else
        {
            alert(qsTr("Invalid option keyword."));
            setPromptPrefix(qsTr("Specify polygon type [Inscribed in circle/Circumscribed around circle]") + " {" + global.polyType + "}: ");
        }
    }
    else if(global.mode == global.mode_INSCRIBE)
    {
        if(str == "D" || str == "DISTANCE") //TODO: Probably should add additional qsTr calls here.
        {
            global.mode = global.mode_DISTANCE;
            setPromptPrefix(qsTr("Specify distance: "));
        }
        else
        {
            var strList = str.split(",");
            if(isNaN(strList[0]) || isNaN(strList[1]))
            {
                alert(qsTr("Point or option keyword required."));
                setPromptPrefix(qsTr("Specify polygon corner point or [Distance]: "));
            }
            else
            {
                global.pointIX = Number(strList[0]);
                global.pointIY = Number(strList[1]);
                setRubberPoint("POLYGON_INSCRIBE_POINT", global.pointIX, global.pointIY);
                vulcanize();
                _main->nativeEndCommand();
            }
        }
    }
    else if(global.mode == global.mode_CIRCUMSCRIBE)
    {
        if(str == "D" || str == "DISTANCE") //TODO: Probably should add additional qsTr calls here.
        {
            global.mode = global.mode_DISTANCE;
            setPromptPrefix(qsTr("Specify distance: "));
        }
        else
        {
            var strList = str.split(",");
            if(isNaN(strList[0]) || isNaN(strList[1]))
            {
                alert(qsTr("Point or option keyword required."));
                setPromptPrefix(qsTr("Specify polygon side point or [Distance]: "));
            }
            else
            {
                global.pointCX = Number(strList[0]);
                global.pointCY = Number(strList[1]);
                setRubberPoint("POLYGON_CIRCUMSCRIBE_POINT", global.pointCX, global.pointCY);
                vulcanize();
                _main->nativeEndCommand();
            }
        }
    }
    else if(global.mode == global.mode_DISTANCE)
    {
        if(isNaN(str))
        {
            alert(qsTr("Requires valid numeric distance."));
            setPromptPrefix(qsTr("Specify distance: "));
        }
        else
        {
            if(global.polyType == "Inscribed")
            {
                global.pointIX = global.centerX;
                global.pointIY = global.centerY + Number(str);
                setRubberPoint("POLYGON_INSCRIBE_POINT", global.pointIX, global.pointIY);
                vulcanize();
                _main->nativeEndCommand();
            }
            else if(global.polyType == "Circumscribed")
            {
                global.pointCX = global.centerX;
                global.pointCY = global.centerY + Number(str);
                setRubberPoint("POLYGON_CIRCUMSCRIBE_POINT", global.pointCX, global.pointCY);
                vulcanize();
                _main->nativeEndCommand();
            }
            else
            {
                error("POLYGON", qsTr("Polygon type is not Inscribed or Circumscribed."));
            }
        }
    }
    else if(global.mode == global.mode_SIDE_LEN)
    {
        todo("POLYGON", "Sidelength mode");
    }
}
#endif

PolygonObject::PolygonObject(qreal x, qreal y, const QPainterPath& p, QRgb rgb, QGraphicsItem* parent) : BaseObject(parent)
{
    qDebug("PolygonObject Constructor()");
    init(x, y, p, rgb, Qt::SolidLine); //TODO: getCurrentLineType
}

PolygonObject::PolygonObject(PolygonObject* obj, QGraphicsItem* parent) : BaseObject(parent)
{
    qDebug("PolygonObject Constructor()");
    if(obj)
    {
        init(obj->objectX(), obj->objectY(), obj->objectCopyPath(), obj->objectColorRGB(), Qt::SolidLine); //TODO: getCurrentLineType
        setRotation(obj->rotation());
        setScale(obj->scale());
    }
}

PolygonObject::~PolygonObject()
{
    qDebug("PolygonObject Destructor()");
}

void PolygonObject::init(qreal x, qreal y, const QPainterPath& p, QRgb rgb, Qt::PenStyle lineType)
{
    setData(OBJ_TYPE, type());
    setData(OBJ_NAME, OBJ_NAME_POLYGON);

    //WARNING: DO NOT enable QGraphicsItem::ItemIsMovable. If it is enabled,
    //WARNING: and the item is double clicked, the scene will erratically move the item while zooming.
    //WARNING: All movement has to be handled explicitly by us, not by the scene.
    setFlag(QGraphicsItem::ItemIsSelectable, true);

    gripIndex = -1;
    updatePath(p);
    setObjectPos(x,y);
    setObjectColor(rgb);
    setObjectLineType(lineType);
    setObjectLineWeight(0.35); //TODO: pass in proper lineweight
    setPen(objectPen());
}

void PolygonObject::updatePath(const QPainterPath& p)
{
    normalPath = p;
    QPainterPath closedPath = normalPath;
    closedPath.closeSubpath();
    QPainterPath reversePath = closedPath.toReversed();
    reversePath.connectPath(closedPath);
    setObjectPath(reversePath);
}

void PolygonObject::paint(QPainter* painter, const QStyleOptionGraphicsItem* option, QWidget* /*widget*/)
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

    if (normalPath.elementCount()) {
        painter->drawPath(normalPath);
        QPainterPath::Element zero = normalPath.elementAt(0);
        QPainterPath::Element last = normalPath.elementAt(normalPath.elementCount()-1);
        painter->drawLine(QPointF(zero.x, zero.y), QPointF(last.x, last.y));
    }
}

void PolygonObject::updateRubber(QPainter* painter)
{
    int rubberMode = objectRubberMode();
    if (rubberMode == OBJ_RUBBER_POLYGON) {
        setObjectPos(objectRubberPoint("POLYGON_POINT_0"));

        bool ok = false;
        QString numStr = objectRubberText("POLYGON_NUM_POINTS");
        if(numStr.isNull()) return;
        int num = numStr.toInt(&ok);
        if(!ok) return;

        QString appendStr;
        QPainterPath rubberPath;
        rubberPath.moveTo(mapFromScene(objectRubberPoint("POLYGON_POINT_0")));
        for(int i = 1; i <= num; i++)
        {
            appendStr = "POLYGON_POINT_" + QString().setNum(i);
            QPointF appendPoint = mapFromScene(objectRubberPoint(appendStr));
            rubberPath.lineTo(appendPoint);
        }
        //rubberPath.lineTo(0,0);
        updatePath(rubberPath);

        //Ensure the path isn't updated until the number of points is changed again
        setObjectRubberText("POLYGON_NUM_POINTS", QString());
    }
    else if(rubberMode == OBJ_RUBBER_POLYGON_INSCRIBE)
    {
        setObjectPos(objectRubberPoint("POLYGON_CENTER"));

        quint16 numSides = objectRubberPoint("POLYGON_NUM_SIDES").x();

        QPointF inscribePoint = mapFromScene(objectRubberPoint("POLYGON_INSCRIBE_POINT"));
        QLineF inscribeLine = QLineF(QPointF(0,0), inscribePoint);
        qreal inscribeAngle = inscribeLine.angle();
        qreal inscribeInc = 360.0/numSides;

        if(painter) drawRubberLine(inscribeLine, painter, VIEW_COLOR_CROSSHAIR);

        QPainterPath inscribePath;
        //First Point
        inscribePath.moveTo(inscribePoint);
        //Remaining Points
        for(int i = 1; i < numSides; i++)
        {
            inscribeLine.setAngle(inscribeAngle + inscribeInc*i);
            inscribePath.lineTo(inscribeLine.p2());
        }
        updatePath(inscribePath);
    }
    else if(rubberMode == OBJ_RUBBER_POLYGON_CIRCUMSCRIBE)
    {
        setObjectPos(objectRubberPoint("POLYGON_CENTER"));

        quint16 numSides = objectRubberPoint("POLYGON_NUM_SIDES").x();

        QPointF circumscribePoint = mapFromScene(objectRubberPoint("POLYGON_CIRCUMSCRIBE_POINT"));
        QLineF circumscribeLine = QLineF(QPointF(0,0), circumscribePoint);
        qreal circumscribeAngle = circumscribeLine.angle();
        qreal circumscribeInc = 360.0/numSides;

        if(painter) drawRubberLine(circumscribeLine, painter, VIEW_COLOR_CROSSHAIR);

        QPainterPath circumscribePath;
        //First Point
        QLineF prev(circumscribeLine.p2(), QPointF(0,0));
        prev = prev.normalVector();
        circumscribeLine.setAngle(circumscribeAngle + circumscribeInc);
        QLineF perp(circumscribeLine.p2(), QPointF(0,0));
        perp = perp.normalVector();
        QPointF iPoint;
        perp.intersects(prev, &iPoint);
        circumscribePath.moveTo(iPoint);
        //Remaining Points
        for(int i = 2; i <= numSides; i++)
        {
            prev = perp;
            circumscribeLine.setAngle(circumscribeAngle + circumscribeInc*i);
            perp = QLineF(circumscribeLine.p2(), QPointF(0,0));
            perp = perp.normalVector();
            perp.intersects(prev, &iPoint);
            circumscribePath.lineTo(iPoint);
        }
        updatePath(circumscribePath);
    }
    else if(rubberMode == OBJ_RUBBER_GRIP)
    {
        if(painter)
        {
            int elemCount = normalPath.elementCount();
            QPointF gripPoint = objectRubberPoint("GRIP_POINT");
            if(gripIndex == -1) gripIndex = findIndex(gripPoint);
            if(gripIndex == -1) return;

            int m = 0;
            int n = 0;

            if(!gripIndex)                    { m = elemCount-1; n = 1; }
            else if(gripIndex == elemCount-1) { m = elemCount-2; n = 0; }
            else                              { m = gripIndex-1; n = gripIndex+1; }
            QPainterPath::Element em = normalPath.elementAt(m);
            QPainterPath::Element en = normalPath.elementAt(n);
            QPointF emPoint = QPointF(em.x, em.y);
            QPointF enPoint = QPointF(en.x, en.y);
            painter->drawLine(emPoint, mapFromScene(objectRubberPoint(QString())));
            painter->drawLine(enPoint, mapFromScene(objectRubberPoint(QString())));

            QLineF rubLine(mapFromScene(gripPoint), mapFromScene(objectRubberPoint(QString())));
            drawRubberLine(rubLine, painter, VIEW_COLOR_CROSSHAIR);
        }
    }
}

void PolygonObject::vulcanize()
{
    qDebug("PolygonObject vulcanize()");
    updateRubber();

    setObjectRubberMode(OBJ_RUBBER_OFF);

    if(!normalPath.elementCount())
        QMessageBox::critical(0, QObject::tr("Empty Polygon Error"), QObject::tr("The polygon added contains no points. The command that created this object has flawed logic."));
}

// Returns the closest snap point to the mouse point
QPointF PolygonObject::mouseSnapPoint(const QPointF& mousePoint)
{
    QPainterPath::Element element = normalPath.elementAt(0);
    QPointF closestPoint = mapToScene(QPointF(element.x, element.y));
    qreal closestDist = QLineF(mousePoint, closestPoint).length();
    int elemCount = normalPath.elementCount();
    for(int i = 0; i < elemCount; ++i)
    {
        element = normalPath.elementAt(i);
        QPointF elemPoint = mapToScene(element.x, element.y);
        qreal elemDist = QLineF(mousePoint, elemPoint).length();
        if(elemDist < closestDist)
        {
            closestPoint = elemPoint;
            closestDist = elemDist;
        }
    }
    return closestPoint;
}

QList<QPointF> PolygonObject::allGripPoints()
{
    QList<QPointF> gripPoints;
    QPainterPath::Element element;
    for(int i = 0; i < normalPath.elementCount(); ++i)
    {
        element = normalPath.elementAt(i);
        gripPoints << mapToScene(element.x, element.y);
    }
    return gripPoints;
}

int PolygonObject::findIndex(const QPointF& point)
{
    int i = 0;
    int elemCount = normalPath.elementCount();
    //NOTE: Points here are in item coordinates
    QPointF itemPoint = mapFromScene(point);
    for(i = 0; i < elemCount; i++)
    {
        QPainterPath::Element e = normalPath.elementAt(i);
        QPointF elemPoint = QPointF(e.x, e.y);
        if(itemPoint == elemPoint) return i;
    }
    return -1;
}

void PolygonObject::gripEdit(const QPointF& before, const QPointF& after)
{
    gripIndex = findIndex(before);
    if(gripIndex == -1) return;
    QPointF a = mapFromScene(after);
    normalPath.setElementPositionAt(gripIndex, a.x(), a.y());
    updatePath(normalPath);
    gripIndex = -1;
}

QPainterPath PolygonObject::objectCopyPath() const
{
    return normalPath;
}

QPainterPath PolygonObject::objectSavePath() const
{
    QPainterPath closedPath = normalPath;
    closedPath.closeSubpath();
    qreal s = scale();
    QTransform trans;
    trans.rotate(rotation());
    trans.scale(s,s);
    return trans.map(closedPath);
}
