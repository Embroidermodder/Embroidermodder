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

void image_init(EmbRect rect, unsigned int rgb, Qt::PenStyle lineType)
{
    setData(OBJ_TYPE, type);
    setData(OBJ_NAME, "Image");

    //WARNING: DO NOT enable QGraphicsItem::ItemIsMovable. If it is enabled,
    //WARNING: and the item is double clicked, the scene will erratically move the item while zooming.
    //WARNING: All movement has to be handled explicitly by us, not by the scene.
    setFlag(QGraphicsItem::ItemIsSelectable, true);

    setObjectRect(rect);
    setObjectColor(rgb);
    setObjectLineType(lineType);
    setObjectLineWeight(0.35); //TODO: pass in proper lineweight
    setPen(objPen);
}

void ImageObject::setObjectRect(double x, double y, double w, double h)
{
    setPos(x, y);
    setRect(0, 0, w, h);
    updatePath();
}

EmbVector ImageObject::objectTopLeft() const
{
    double alpha = radians(rotation());
    EmbVector tl = rect().topRight() * scale();
    EmbVector ptlrot = embVector_rotate(tl, alpha);
    return scenePos() + ptlrot;
}

EmbVector ImageObject::objectTopRight() const
{
    double alpha = radians(rotation());
    EmbVector tr = rect().topRight() * scale();
    EmbVector ptrrot = embVector_rotate(tr, alpha);
    return scenePos() + ptrrot;
}

EmbVector ImageObject::objectBottomLeft() const
{
    double alpha = radians(rotation());
    EmbVector bl = rect().topRight() * scale();
    EmbVector pblrot = embVector_rotate(bl, alpha);
    return scenePos() + pblrot;
}

EmbVector ImageObject::objectBottomRight() const
{
    double alpha = radians(rotation());
    EmbVector br = rect().topRight() * scale();
    EmbVector pbrrot = embVector_rotate(br, alpha);
    return scenePos() + pbrrot;
}

void ImageObject::updatePath()
{
    QPainterPath path;
    EmbRect r = rect();
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

void ImageObject::paint(QPainter* painter, const QStyleOptionGraphicsItem* option, QWidget* /*widget*/)
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

void ImageObject::updateRubber(QPainter* painter)
{
    int rubberMode = objectRubberMode();
    if (rubberMode == OBJ_RUBBER_IMAGE)
    {
        EmbVector sceneStartPoint = objectRubberPoint("IMAGE_START");
        EmbVector sceneEndPoint = objectRubberPoint("IMAGE_END");
        double x = sceneStartPoint.x();
        double y = sceneStartPoint.y();
        double w = sceneEndPoint.x() - sceneStartPoint.x();
        double h = sceneEndPoint.y() - sceneStartPoint.y();
        setObjectRect(x,y,w,h);
        updatePath();
    }
    else if (rubberMode == OBJ_RUBBER_GRIP)
    {
        //TODO: updateRubber() gripping for ImageObject
    }
}

void ImageObject::vulcanize()
{
    debug_message("ImageObject vulcanize()");
    updateRubber();

    setObjectRubberMode(OBJ_RUBBER_OFF);
}

// Returns the closest snap point to the mouse point
EmbVector ImageObject::mouseSnapPoint(const EmbVector& mousePoint)
{
    EmbVector ptl = objectTopLeft();     //Top Left Corner QSnap
    EmbVector ptr = objectTopRight();    //Top Right Corner QSnap
    EmbVector pbl = objectBottomLeft();  //Bottom Left Corner QSnap
    EmbVector pbr = objectBottomRight(); //Bottom Right Corner QSnap

    double ptlDist = EmbLine(mousePoint, ptl).length();
    double ptrDist = EmbLine(mousePoint, ptr).length();
    double pblDist = EmbLine(mousePoint, pbl).length();
    double pbrDist = EmbLine(mousePoint, pbr).length();

    double minDist = std::min(std::min(ptlDist, ptrDist), std::min(pblDist, pbrDist));

    if     (minDist == ptlDist) return ptl;
    else if (minDist == ptrDist) return ptr;
    else if (minDist == pblDist) return pbl;
    else if (minDist == pbrDist) return pbr;

    return scenePos();
}

std::vector<EmbVector> ImageObject::allGripPoints()
{
    std::vector<EmbVector> gripPoints;
    gripPoints << objectTopLeft() << objectTopRight() << objectBottomLeft() << objectBottomRight();
    return gripPoints;
}

void ImageObject::gripEdit(const EmbVector& before, const EmbVector& after)
{
    //TODO: gripEdit() for ImageObject
}
#endif
