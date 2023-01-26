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
PolygonObject::PolygonObject(double x, double y, const QPainterPath& p, unsigned int rgb, QGraphicsItem* parent) : BaseObject(parent)
{
    debug_message("PolygonObject Constructor()");
    init(x, y, p, rgb, SolidLine); //TODO: getCurrentLineType
}

PolygonObject::PolygonObject(PolygonObject* obj, QGraphicsItem* parent) : BaseObject(parent)
{
    debug_message("PolygonObject Constructor()");
    if (obj)
    {
        init(obj->objectX(), obj->objectY(), obj->objectCopyPath(), obj->objectColorRGB(), SolidLine); //TODO: getCurrentLineType
        setRotation(obj->rotation());
        setScale(obj->scale());
    }
}

PolygonObject::~PolygonObject()
{
    debug_message("PolygonObject Destructor()");
}

void PolygonObject::init(double x, double y, const QPainterPath& p, unsigned int rgb, PenStyle lineType)
{
    setData(OBJ_TYPE, type);
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
    if (!objScene) return;

    QPen paintPen = pen();
    painter->setPen(paintPen);
    updateRubber(painter);
    if (option->state & QStyle::State_Selected)  { paintPen.setStyle(DashLine); }
    if (objScene->property(ENABLE_LWT).toBool()) { paintPen = lineWeightPen(); }
    painter->setPen(paintPen);

    if (normalPath.elementCount())
    {
        painter->drawPath(normalPath);
        QPainterPath::Element zero = normalPath.elementAt(0);
        QPainterPath::Element last = normalPath.elementAt(normalPath.elementCount()-1);
        painter->drawLine(EmbVector(zero.x, zero.y), EmbVector(last.x, last.y));
    }
}

void PolygonObject::updateRubber(QPainter* painter)
{
    int rubberMode = objectRubberMode();
    if (rubberMode == OBJ_RUBBER_POLYGON)
    {
        setObjectPos(objectRubberPoint("POLYGON_POINT_0"));

        bool ok = false;
        std::string numStr = objectRubberText("POLYGON_NUM_POINTS");
        if (numStr.isNull()) return;
        int num = numStr.toInt(&ok);
        if (!ok) return;

        std::string appendStr;
        QPainterPath rubberPath;
        rubberPath.moveTo(mapFromScene(objectRubberPoint("POLYGON_POINT_0")));
        for(int i = 1; i <= num; i++)
        {
            appendStr = "POLYGON_POINT_" + std::string().setNum(i);
            EmbVector appendPoint = mapFromScene(objectRubberPoint(appendStr));
            rubberPath.lineTo(appendPoint);
        }
        //rubberPath.lineTo(0,0);
        updatePath(rubberPath);

        //Ensure the path isn't updated until the number of points is changed again
        setObjectRubberText("POLYGON_NUM_POINTS", std::string());
    }
    else if (rubberMode == OBJ_RUBBER_POLYGON_INSCRIBE)
    {
        setObjectPos(objectRubberPoint("POLYGON_CENTER"));

        quint16 numSides = objectRubberPoint("POLYGON_NUM_SIDES").x();

        EmbVector inscribePoint = mapFromScene(objectRubberPoint("POLYGON_INSCRIBE_POINT"));
        QLineF inscribeLine = QLineF(EmbVector(0,0), inscribePoint);
        double inscribeAngle = inscribeLine.angle();
        double inscribeInc = 360.0/numSides;

        if (painter) drawRubberLine(inscribeLine, painter, VIEW_COLOR_CROSSHAIR);

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
    else if (rubberMode == OBJ_RUBBER_POLYGON_CIRCUMSCRIBE)
    {
        setObjectPos(objectRubberPoint("POLYGON_CENTER"));

        quint16 numSides = objectRubberPoint("POLYGON_NUM_SIDES").x();

        EmbVector circumscribePoint = mapFromScene(objectRubberPoint("POLYGON_CIRCUMSCRIBE_POINT"));
        QLineF circumscribeLine = QLineF(EmbVector(0,0), circumscribePoint);
        double circumscribeAngle = circumscribeLine.angle();
        double circumscribeInc = 360.0/numSides;

        if (painter) drawRubberLine(circumscribeLine, painter, VIEW_COLOR_CROSSHAIR);

        QPainterPath circumscribePath;
        //First Point
        QLineF prev(circumscribeLine.p2(), EmbVector(0,0));
        prev = prev.normalVector();
        circumscribeLine.setAngle(circumscribeAngle + circumscribeInc);
        QLineF perp(circumscribeLine.p2(), EmbVector(0,0));
        perp = perp.normalVector();
        EmbVector iPoint;
        perp.intersects(prev, &iPoint);
        circumscribePath.moveTo(iPoint);
        //Remaining Points
        for(int i = 2; i <= numSides; i++)
        {
            prev = perp;
            circumscribeLine.setAngle(circumscribeAngle + circumscribeInc*i);
            perp = QLineF(circumscribeLine.p2(), EmbVector(0,0));
            perp = perp.normalVector();
            perp.intersects(prev, &iPoint);
            circumscribePath.lineTo(iPoint);
        }
        updatePath(circumscribePath);
    }
    else if (rubberMode == OBJ_RUBBER_GRIP) {
        if (painter) {
            int elemCount = normalPath.elementCount();
            EmbVector gripPoint = objectRubberPoint("GRIP_POINT");
            if (gripIndex == -1) gripIndex = findIndex(gripPoint);
            if (gripIndex == -1) return;

            int m = 0;
            int n = 0;

            if (!gripIndex)                    { m = elemCount-1; n = 1; }
            else if (gripIndex == elemCount-1) { m = elemCount-2; n = 0; }
            else                              { m = gripIndex-1; n = gripIndex+1; }
            QPainterPath::Element em = normalPath.elementAt(m);
            QPainterPath::Element en = normalPath.elementAt(n);
            EmbVector emPoint = EmbVector(em.x, em.y);
            EmbVector enPoint = EmbVector(en.x, en.y);
            painter->drawLine(emPoint, mapFromScene(objectRubberPoint(std::string())));
            painter->drawLine(enPoint, mapFromScene(objectRubberPoint(std::string())));

            QLineF rubLine(mapFromScene(gripPoint), mapFromScene(objectRubberPoint(std::string())));
            drawRubberLine(rubLine, painter, VIEW_COLOR_CROSSHAIR);
        }
    }
}

void PolygonObject::vulcanize()
{
    debug_message("PolygonObject vulcanize()");
    updateRubber();

    setObjectRubberMode(OBJ_RUBBER_OFF);

    if (!normalPath.elementCount())
        QMessageBox::critical(0, QObject::tr("Empty Polygon Error"), QObject::tr("The polygon added contains no points. The command that created this object has flawed logic."));
}

// Returns the closest snap point to the mouse point
EmbVector PolygonObject::mouseSnapPoint(const EmbVector& mousePoint)
{
    QPainterPath::Element element = normalPath.elementAt(0);
    EmbVector closestPoint = mapToScene(EmbVector(element.x, element.y));
    double closestDist = QLineF(mousePoint, closestPoint).length();
    int elemCount = normalPath.elementCount();
    for (int i = 0; i < elemCount; ++i) {
        element = normalPath.elementAt(i);
        EmbVector elemPoint = mapToScene(element.x, element.y);
        double elemDist = QLineF(mousePoint, elemPoint).length();
        if (elemDist < closestDist) {
            closestPoint = elemPoint;
            closestDist = elemDist;
        }
    }
    return closestPoint;
}

std::vector<EmbVector> PolygonObject::allGripPoints()
{
    std::vector<EmbVector> gripPoints;
    QPainterPath::Element element;
    for (int i = 0; i < normalPath.elementCount(); ++i) {
        element = normalPath.elementAt(i);
        gripPoints << mapToScene(element.x, element.y);
    }
    return gripPoints;
}

int PolygonObject::findIndex(const EmbVector& point)
{
    int i = 0;
    int elemCount = normalPath.elementCount();
    //NOTE: Points here are in item coordinates
    EmbVector itemPoint = mapFromScene(point);
    for(i = 0; i < elemCount; i++)
    {
        QPainterPath::Element e = normalPath.elementAt(i);
        EmbVector elemPoint = EmbVector(e.x, e.y);
        if (itemPoint == elemPoint) return i;
    }
    return -1;
}

void PolygonObject::gripEdit(const EmbVector& before, const EmbVector& after)
{
    gripIndex = findIndex(before);
    if (gripIndex == -1) return;
    EmbVector a = mapFromScene(after);
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
    double s = scale();
    QTransform trans;
    trans.rotate(rotation());
    trans.scale(s,s);
    return trans.map(closedPath);
}
#endif
