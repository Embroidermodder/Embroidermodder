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
RectObject::RectObject(double x, double y, double w, double h, unsigned int rgb, QGraphicsItem* parent) : BaseObject(parent)
{
    debug_message("RectObject Constructor()");
    init(x, y, w, h, rgb, Qt::SolidLine); //TODO: getCurrentLineType
}

RectObject::RectObject(RectObject* obj, QGraphicsItem* parent) : BaseObject(parent)
{
    debug_message("RectObject Constructor()");
    if (obj)
    {
        EmbVector ptl = obj->objectTopLeft();
        init(ptl.x(), ptl.y(), obj->objectWidth(), obj->objectHeight(), obj->objectColorRGB(), Qt::SolidLine); //TODO: getCurrentLineType
        setRotation(obj->rotation());
    }
}

RectObject::~RectObject()
{
    debug_message("RectObject Destructor()");
}

void RectObject::init(double x, double y, double w, double h, unsigned int rgb, Qt::PenStyle lineType)
{
    setData(OBJ_TYPE, type);
    setData(OBJ_NAME, "Rectangle");

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

void RectObject::setObjectRect(double x, double y, double w, double h)
{
    setPos(x, y);
    setRect(0, 0, w, h);
    updatePath();
}

EmbVector RectObject::objectTopLeft() const
{
    double rot = radians(rotation());
    double cosRot = cos(rot);
    double sinRot = sin(rot);

    EmbVector tl = rect().topLeft();
    double ptlX = tl.x()*scale();
    double ptlY = tl.y()*scale();
    double ptlXrot = ptlX*cosRot - ptlY*sinRot;
    double ptlYrot = ptlX*sinRot + ptlY*cosRot;

    return (scenePos() + EmbVector(ptlXrot, ptlYrot));
}

EmbVector RectObject::objectTopRight() const
{
    double rot = radians(rotation());
    double cosRot = cos(rot);
    double sinRot = sin(rot);

    EmbVector tr = rect().topRight();
    double ptrX = tr.x()*scale();
    double ptrY = tr.y()*scale();
    double ptrXrot = ptrX*cosRot - ptrY*sinRot;
    double ptrYrot = ptrX*sinRot + ptrY*cosRot;

    return (scenePos() + EmbVector(ptrXrot, ptrYrot));
}

EmbVector RectObject::objectBottomLeft() const
{
    double rot = radians(rotation());
    double cosRot = cos(rot);
    double sinRot = sin(rot);

    EmbVector bl = rect().bottomLeft();
    double pblX = bl.x()*scale();
    double pblY = bl.y()*scale();
    double pblXrot = pblX*cosRot - pblY*sinRot;
    double pblYrot = pblX*sinRot + pblY*cosRot;

    return (scenePos() + EmbVector(pblXrot, pblYrot));
}

EmbVector RectObject::objectBottomRight() const
{
    double rot = radians(rotation());
    double cosRot = cos(rot);
    double sinRot = sin(rot);

    EmbVector br = rect().bottomRight();
    double pbrX = br.x()*scale();
    double pbrY = br.y()*scale();
    double pbrXrot = pbrX*cosRot - pbrY*sinRot;
    double pbrYrot = pbrX*sinRot + pbrY*cosRot;

    return (scenePos() + EmbVector(pbrXrot, pbrYrot));
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
    if (!objScene) return;

    QPen paintPen = pen();
    painter->setPen(paintPen);
    updateRubber(painter);
    if (option->state & QStyle::State_Selected)  { paintPen.setStyle(Qt::DashLine); }
    if (objScene->property(ENABLE_LWT).toBool()) { paintPen = lineWeightPen(); }
    painter->setPen(paintPen);

    painter->drawRect(rect());
}

void RectObject::updateRubber(QPainter* painter)
{
    int rubberMode = objectRubberMode();
    if (rubberMode == OBJ_RUBBER_RECTANGLE)
    {
        EmbVector sceneStartPoint = objectRubberPoint("RECTANGLE_START");
        EmbVector sceneEndPoint = objectRubberPoint("RECTANGLE_END");
        double x = sceneStartPoint.x();
        double y = sceneStartPoint.y();
        double w = sceneEndPoint.x() - sceneStartPoint.x();
        double h = sceneEndPoint.y() - sceneStartPoint.y();
        setObjectRect(x,y,w,h);
        updatePath();
    }
    else if (rubberMode == OBJ_RUBBER_GRIP)
    {
        if (painter)
        {
            //TODO: Make this work with rotation & scaling
            /*
            EmbVector gripPoint = objectRubberPoint("GRIP_POINT");
            EmbVector after = objectRubberPoint(std::string());
            EmbVector delta = after-gripPoint;
            if     (gripPoint == objectTopLeft())     { painter->drawPolygon(mapFromScene(QRectF(after.x(), after.y(), objectWidth()-delta.x(), objectHeight()-delta.y()))); }
            else if (gripPoint == objectTopRight())    { painter->drawPolygon(mapFromScene(QRectF(objectTopLeft().x(), objectTopLeft().y()+delta.y(), objectWidth()+delta.x(), objectHeight()-delta.y()))); }
            else if (gripPoint == objectBottomLeft())  { painter->drawPolygon(mapFromScene(QRectF(objectTopLeft().x()+delta.x(), objectTopLeft().y(), objectWidth()-delta.x(), objectHeight()+delta.y()))); }
            else if (gripPoint == objectBottomRight()) { painter->drawPolygon(mapFromScene(QRectF(objectTopLeft().x(), objectTopLeft().y(), objectWidth()+delta.x(), objectHeight()+delta.y()))); }

            QLineF rubLine(mapFromScene(gripPoint), mapFromScene(objectRubberPoint(std::string())));
            drawRubberLine(rubLine, painter, VIEW_COLOR_CROSSHAIR);
            */

            EmbVector gripPoint = objectRubberPoint("GRIP_POINT");
            EmbVector after = objectRubberPoint(std::string());
            EmbVector delta = after-gripPoint;

            QLineF rubLine(mapFromScene(gripPoint), mapFromScene(objectRubberPoint(std::string())));
            drawRubberLine(rubLine, painter, VIEW_COLOR_CROSSHAIR);
        }
    }
}

void RectObject::vulcanize()
{
    debug_message("RectObject vulcanize()");
    updateRubber();

    setObjectRubberMode(OBJ_RUBBER_OFF);
}

// Returns the closest snap point to the mouse point
EmbVector RectObject::mouseSnapPoint(const EmbVector& mousePoint)
{
    EmbVector ptl = objectTopLeft();     //Top Left Corner QSnap
    EmbVector ptr = objectTopRight();    //Top Right Corner QSnap
    EmbVector pbl = objectBottomLeft();  //Bottom Left Corner QSnap
    EmbVector pbr = objectBottomRight(); //Bottom Right Corner QSnap

    double ptlDist = QLineF(mousePoint, ptl).length();
    double ptrDist = QLineF(mousePoint, ptr).length();
    double pblDist = QLineF(mousePoint, pbl).length();
    double pbrDist = QLineF(mousePoint, pbr).length();

    double minDist = std::min(std::min(ptlDist, ptrDist), std::min(pblDist, pbrDist));

    if     (minDist == ptlDist) return ptl;
    else if (minDist == ptrDist) return ptr;
    else if (minDist == pblDist) return pbl;
    else if (minDist == pbrDist) return pbr;

    return scenePos();
}

QList<EmbVector> RectObject::allGripPoints()
{
    QList<EmbVector> gripPoints;
    gripPoints << objectTopLeft() << objectTopRight() << objectBottomLeft() << objectBottomRight();
    return gripPoints;
}

void RectObject::gripEdit(const EmbVector& before, const EmbVector& after)
{
    EmbVector delta = after-before;
    if     (before == objectTopLeft())     { setObjectRect(after.x(), after.y(), objectWidth()-delta.x(), objectHeight()-delta.y()); }
    else if (before == objectTopRight())    { setObjectRect(objectTopLeft().x(), objectTopLeft().y()+delta.y(), objectWidth()+delta.x(), objectHeight()-delta.y()); }
    else if (before == objectBottomLeft())  { setObjectRect(objectTopLeft().x()+delta.x(), objectTopLeft().y(), objectWidth()-delta.x(), objectHeight()+delta.y()); }
    else if (before == objectBottomRight()) { setObjectRect(objectTopLeft().x(), objectTopLeft().y(), objectWidth()+delta.x(), objectHeight()+delta.y()); }
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

    double s = scale();
    QTransform trans;
    trans.rotate(rotation());
    trans.scale(s,s);
    return trans.map(path);
}
#endif
