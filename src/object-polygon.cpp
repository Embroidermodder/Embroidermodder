/*
 * Embroidermodder 2.
 *
 * Copyright 2011-2024 The Embroidermodder Team
 * Embroidermodder 2 is Open Source Software, see LICENSE.md for licensing terms.
 * Visit https://www.libembroidery.org/refman for advice on altering this file,
 * or read the markdown version in embroidermodder2/docs/refman.
 *
 * Polygon Commands
 */

#include "embroidermodder.h"

#include <QPainter>
#include <QStyleOption>
#include <QGraphicsScene>
#include <QMessageBox>

#define POLYGON_MODE_NUM_SIDES      0
#define POLYGON_MODE_CENTER_PT      1
#define POLYGON_MODE_POLYTYPE       2
#define POLYGON_MODE_INSCRIBE       3
#define POLYGON_MODE_CIRCUMSCRIBE   4
#define POLYGON_MODE_DISTANCE       5
#define POLYGON_MODE_SIDE_LEN       6

/* POLYGON */
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
EmbVector center;
EmbVector side1;
EmbVector side2;
EmbVector pointI;
EmbVector pointC;
char *polyType = "Inscribed"; //Default
int numSides = 4;           //Default
int mode;

function
main()
{
    init_command();
    clear_selection();
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
    global.mode = POLYGON_MODE_NUM_SIDES;
    prompt_output(translate("Enter number of sides") + " {" + global.numSides.toString() + "}: ");
}

function
click(EmbVector v)
{
    switch (global.mode) {
    default:
    case POLYGON_MODE_NUM_SIDES:
    case POLYGON_MODE_POLYTYPE:
    case POLYGON_MODE_DISTANCE: {
        //Do nothing, the prompt controls these.
        break;
    }
    case POLYGON_MODE_CENTER_PT: {
        global.center = v;
        global.mode = POLYGON_MODE_POLYTYPE;
        prompt_output(translate("Specify polygon type [Inscribed in circle/Circumscribed around circle]") + " {" + global.polyType + "}: ");
        break;
    }
    case POLYGON_MODE_INSCRIBE: {
        global.pointI = v;
        setRubberPoint("POLYGON_INSCRIBE_POINT", global.pointIX, global.pointIY);
        vulcanize();
        end_command();
        break;
    }
    case POLYGON_MODE_CIRCUMSCRIBE: {
        global.pointC = v;
        setRubberPoint("POLYGON_CIRCUMSCRIBE_POINT", global.pointCX, global.pointCY);
        vulcanize();
        end_command();
        break;
    }
    case POLYGON_MODE_SIDE_LEN: {
        todo("POLYGON", "Sidelength mode");
        break;
    }
    }
}

function
context(char *str)
{
    todo("POLYGON", "context()");
}

function
prompt(char *str)
{
    EmbVector v;
    switch (global->mode) {
    case POLYGON_MODE_NUM_SIDES: {
        if (str == "" && global.numSides >= 3 && global.numSides <= 1024) {
            prompt_output(translate("Specify center point or [Sidelength]: "));
            global.mode = POLYGON_MODE_CENTER_PT;
        }
        else {
            var tmp = Number(str);
            if (isNaN(tmp) || !isInt(tmp) || tmp < 3 || tmp > 1024) {
                alert(translate("Requires an integer between 3 and 1024."));
                prompt_output(translate("Enter number of sides") + " {" + global.numSides.toString() + "}: ");
            }
            else {
                global.numSides = tmp;
                prompt_output(translate("Specify center point or [Sidelength]: "));
                global.mode = POLYGON_MODE_CENTER_PT;
            }
        }
        break;
    }
    case POLYGON_MODE_CENTER_PT: {
        if (str == "S" || str == "SIDELENGTH") {
            /* TODO: Probably should add additional translate calls here. */
            global.mode = POLYGON_MODE_SIDE_LEN;
            prompt_output(translate("Specify start point: "));
        }
        else {
            if (!parse_vector(str, &v)) {
                alert(translate("Point or option keyword required."));
                prompt_output(translate("Specify center point or [Sidelength]: "));
            }
            else {
                global.center = v;
                global.mode = POLYGON_MODE_POLYTYPE;
                prompt_output(translate("Specify polygon type [Inscribed in circle/Circumscribed around circle]") + " {" + global.polyType + "}: ");
            }
        }
        break;
    }
    case POLYGON_MODE_POLYTYPE: {
        if (str == "I" || str == "INSCRIBED") {
            /* TODO: Probably should add additional translate calls here. */
            global.mode = POLYGON_MODE_INSCRIBE;
            global.polyType = "Inscribed";
            prompt_output(translate("Specify polygon corner point or [Distance]: "));
            addRubber("POLYGON");
            setRubberMode("POLYGON_INSCRIBE");
            setRubberPoint("POLYGON_CENTER", global.centerX, global.centerY);
            setRubberPoint("POLYGON_NUM_SIDES", global.numSides, 0);
        }
        else if (str == "C" || str == "CIRCUMSCRIBED") {
            /* TODO: Probably should add additional translate calls here. */
            global.mode = POLYGON_MODE_CIRCUMSCRIBE;
            global.polyType = "Circumscribed";
            prompt_output(translate("Specify polygon side point or [Distance]: "));
            addRubber("POLYGON");
            setRubberMode("POLYGON_CIRCUMSCRIBE");
            setRubberPoint("POLYGON_CENTER", global.centerX, global.centerY);
            setRubberPoint("POLYGON_NUM_SIDES", global.numSides, 0);
        }
        else if (str == "") {
            if (global.polyType == "Inscribed") {
                global.mode = POLYGON_MODE_INSCRIBE;
                prompt_output(translate("Specify polygon corner point or [Distance]: "));
                addRubber("POLYGON");
                setRubberMode("POLYGON_INSCRIBE");
                setRubberPoint("POLYGON_CENTER", global.centerX, global.centerY);
                setRubberPoint("POLYGON_NUM_SIDES", global.numSides, 0);
            }
            else if (global.polyType == "Circumscribed") {
                global.mode = POLYGON_MODE_CIRCUMSCRIBE;
                prompt_output(translate("Specify polygon side point or [Distance]: "));
                addRubber("POLYGON");
                setRubberMode("POLYGON_CIRCUMSCRIBE");
                setRubberPoint("POLYGON_CENTER", global.centerX, global.centerY);
                setRubberPoint("POLYGON_NUM_SIDES", global.numSides, 0);
            }
            else {
                error("POLYGON", translate("Polygon type is not Inscribed or Circumscribed."));
            }
        }
        else {
            alert(translate("Invalid option keyword."));
            prompt_output(translate("Specify polygon type [Inscribed in circle/Circumscribed around circle]") + " {" + global.polyType + "}: ");
        }
        break;
    }
    case POLYGON_MODE_INSCRIBE: {
        if (str == "D" || str == "DISTANCE") {
            global.mode = POLYGON_MODE_DISTANCE;
            prompt_output(translate("Specify distance: "));
        }
        else {
            if (!parse_vector(str, &v)) {
                alert(translate("Point or option keyword required."));
                prompt_output(translate("Specify polygon corner point or [Distance]: "));
            }
            else {
                global.pointI = v;
                setRubberPoint("POLYGON_INSCRIBE_POINT", global.pointIX, global.pointIY);
                vulcanize();
                end_command();
            }
        }
        break;
    }
    case POLYGON_MODE_CIRCUMSCRIBE: {
        if (str == "D" || str == "DISTANCE") {
            global.mode = POLYGON_MODE_DISTANCE;
            prompt_output(translate("Specify distance: "));
        }
        else {
            if (!parse_vector(str, &v)) {
                alert(translate("Point or option keyword required."));
                prompt_output(translate("Specify polygon side point or [Distance]: "));
            }
            else {
                global.pointC = v;
                setRubberPoint("POLYGON_CIRCUMSCRIBE_POINT", global.pointCX, global.pointCY);
                vulcanize();
                end_command();
            }
        }
        break;
    }
    case POLYGON_MODE_DISTANCE: {
        if (isNaN(str)) {
            alert(translate("Requires valid numeric distance."));
            prompt_output(translate("Specify distance: "));
        }
        else {
            if (global.polyType == "Inscribed") {
                global.pointI.x = global.center.x;
                global.pointI.y = global.center.y + Number(str);
                setRubberPoint("POLYGON_INSCRIBE_POINT", global.pointIX, global.pointIY);
                vulcanize();
                end_command();
            }
            else if (global.polyType == "Circumscribed") {
                global.pointC.x = global.center.x;
                global.pointC.y = global.center.y + Number(str);
                setRubberPoint("POLYGON_CIRCUMSCRIBE_POINT", global.pointCX, global.pointCY);
                vulcanize();
                end_command();
            }
            else {
                error("POLYGON", translate("Polygon type is not Inscribed or Circumscribed."));
            }
        }
        break;
    }
    case POLYGON_MODE_SIDE_LEN: {
        todo("POLYGON", "Sidelength mode");
        break;
    }
    default:
        break;
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
    if (obj) {
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

void
PolygonObject::paint(QPainter* painter, const QStyleOptionGraphicsItem* option, QWidget* /*widget*/)
{
    QGraphicsScene* objScene = scene();
    if (!objScene) {
        return;
    }

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

void
PolygonObject::updateRubber(QPainter* painter)
{
    int rubberMode = objectRubberMode();
    if (rubberMode == OBJ_RUBBER_POLYGON) {
        setObjectPos(objectRubberPoint("POLYGON_POINT_0"));

        bool ok = false;
        QString numStr = objectRubberText("POLYGON_NUM_POINTS");
        if (numStr.isNull()) {
            return;
        }
        int num = numStr.toInt(&ok);
        if (!ok) {
            return;
        }

        QString appendStr;
        QPainterPath rubberPath;
        rubberPath.moveTo(mapFromScene(objectRubberPoint("POLYGON_POINT_0")));
        for (int i = 1; i <= num; i++) {
            appendStr = "POLYGON_POINT_" + QString().setNum(i);
            QPointF appendPoint = mapFromScene(objectRubberPoint(appendStr));
            rubberPath.lineTo(appendPoint);
        }
        //rubberPath.lineTo(0,0);
        updatePath(rubberPath);

        //Ensure the path isn't updated until the number of points is changed again
        setObjectRubberText("POLYGON_NUM_POINTS", QString());
    }
    else if (rubberMode == OBJ_RUBBER_POLYGON_INSCRIBE) {
        setObjectPos(objectRubberPoint("POLYGON_CENTER"));

        quint16 numSides = objectRubberPoint("POLYGON_NUM_SIDES").x();

        QPointF inscribePoint = mapFromScene(objectRubberPoint("POLYGON_INSCRIBE_POINT"));
        QLineF inscribeLine = QLineF(QPointF(0,0), inscribePoint);
        qreal inscribeAngle = inscribeLine.angle();
        qreal inscribeInc = 360.0/numSides;

        if (painter) {
            drawRubberLine(inscribeLine, painter, VIEW_COLOR_CROSSHAIR);
        }

        QPainterPath inscribePath;
        //First Point
        inscribePath.moveTo(inscribePoint);
        //Remaining Points
        for (int i = 1; i < numSides; i++) {
            inscribeLine.setAngle(inscribeAngle + inscribeInc*i);
            inscribePath.lineTo(inscribeLine.p2());
        }
        updatePath(inscribePath);
    }
    else if (rubberMode == OBJ_RUBBER_POLYGON_CIRCUMSCRIBE) {
        setObjectPos(objectRubberPoint("POLYGON_CENTER"));

        quint16 numSides = objectRubberPoint("POLYGON_NUM_SIDES").x();

        QPointF circumscribePoint = mapFromScene(objectRubberPoint("POLYGON_CIRCUMSCRIBE_POINT"));
        QLineF circumscribeLine = QLineF(QPointF(0,0), circumscribePoint);
        qreal circumscribeAngle = circumscribeLine.angle();
        qreal circumscribeInc = 360.0/numSides;

        if (painter) {
            drawRubberLine(circumscribeLine, painter, VIEW_COLOR_CROSSHAIR);
        }

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
        for (int i = 2; i <= numSides; i++) {
            prev = perp;
            circumscribeLine.setAngle(circumscribeAngle + circumscribeInc*i);
            perp = QLineF(circumscribeLine.p2(), QPointF(0,0));
            perp = perp.normalVector();
            perp.intersects(prev, &iPoint);
            circumscribePath.lineTo(iPoint);
        }
        updatePath(circumscribePath);
    }
    else if (rubberMode == OBJ_RUBBER_GRIP) {
        if (painter) {
            int elemCount = normalPath.elementCount();
            QPointF gripPoint = objectRubberPoint("GRIP_POINT");
            if (gripIndex == -1) {
                gripIndex = findIndex(gripPoint);
                if (gripIndex == -1) {
                    return;
                }
            }

            int m = 0;
            int n = 0;

            if (!gripIndex) {
                m = elemCount - 1;
                n = 1;
            }
            else if (gripIndex == elemCount-1) {
                m = elemCount - 2;
                n = 0;
            }
            else {
                m = gripIndex - 1;
                n = gripIndex + 1;
            }
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

    if (!normalPath.elementCount())
        QMessageBox::critical(0, QObject::tr("Empty Polygon Error"), QObject::tr("The polygon added contains no points. The command that created this object has flawed logic."));
}

// Returns the closest snap point to the mouse point
QPointF PolygonObject::mouseSnapPoint(const QPointF& mousePoint)
{
    return find_mouse_snap_point(allGripPoints(), mousePoint);
}

QList<QPointF> PolygonObject::allGripPoints()
{
    QList<QPointF> gripPoints;
    QPainterPath::Element element;
    for (int i = 0; i < normalPath.elementCount(); ++i) {
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
    for (i = 0; i < elemCount; i++) {
        QPainterPath::Element e = normalPath.elementAt(i);
        QPointF elemPoint = QPointF(e.x, e.y);
        if (itemPoint == elemPoint) {
            return i;
        }
    }
    return -1;
}

void
PolygonObject::gripEdit(const QPointF& before, const QPointF& after)
{
    gripIndex = findIndex(before);
    if (gripIndex == -1) {
        return;
    }
    QPointF a = mapFromScene(after);
    normalPath.setElementPositionAt(gripIndex, a.x(), a.y());
    updatePath(normalPath);
    gripIndex = -1;
}

QPainterPath
PolygonObject::objectCopyPath() const
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
