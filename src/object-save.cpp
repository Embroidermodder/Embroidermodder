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
SaveObject::SaveObject(QGraphicsScene* theScene, QObject* parent) : QObject(parent)
{
    debug_message("SaveObject Constructor()");
    gscene = theScene;
    formatType = EMBFORMAT_UNSUPPORTED;
}

SaveObject::~SaveObject()
{
    debug_message("SaveObject Destructor()");
}

bool SaveObject::save(const std::string &fileName)
{
    debug_message("SaveObject save(%s)", qPrintable(fileName));

    /* TODO: Before saving to a stitch only format, Embroidermodder needs
     *       to calculate the optimal path to minimize jump stitches. Also
     *       based upon which layer needs to be stitched first,
     *       the path to the next object needs to be hidden beneath fills
     *       that will come later. When finding the optimal path, we need
     *       to take into account the color of the thread, as we do not want
     *       to try to hide dark colored stitches beneath light colored fills.
     */

    bool writeSuccessful = false;
    int i;

    int formatType = emb_identify_format(qPrintable(fileName));
    if (formatType == EMBFORMAT_UNSUPPORTED) {
        debug_message("Unsupported write file type: %s", qPrintable(fileName));
        return false;
    }

    EmbPattern* pattern = embPattern_create();
    if (!pattern) {
        debug_message("Could not allocate memory for embroidery pattern");
    }

    /* Write */
    foreach(QGraphicsItem* item, gscene->items(Qt::AscendingOrder)) {
        int objType = item->data(OBJ_TYPE).toInt();

        if     (objType == OBJ_TYPE_ARC)          { addArc(pattern, item);          }
        else if (objType == OBJ_TYPE_BLOCK)        { addBlock(pattern, item);        }
        else if (objType == OBJ_TYPE_CIRCLE)       { addCircle(pattern, item);       }
        else if (objType == OBJ_TYPE_DIMALIGNED)   { addDimAligned(pattern, item);   }
        else if (objType == OBJ_TYPE_DIMANGULAR)   { addDimAngular(pattern, item);   }
        else if (objType == OBJ_TYPE_DIMARCLENGTH) { addDimArcLength(pattern, item); }
        else if (objType == OBJ_TYPE_DIMDIAMETER)  { addDimDiameter(pattern, item);  }
        else if (objType == OBJ_TYPE_DIMLEADER)    { addDimLeader(pattern, item);    }
        else if (objType == OBJ_TYPE_DIMLINEAR)    { addDimLinear(pattern, item);    }
        else if (objType == OBJ_TYPE_DIMORDINATE)  { addDimOrdinate(pattern, item);  }
        else if (objType == OBJ_TYPE_DIMRADIUS)    { addDimRadius(pattern, item);    }
        else if (objType == OBJ_TYPE_ELLIPSE)      { addEllipse(pattern, item);      }
        else if (objType == OBJ_TYPE_ELLIPSEARC)   { addEllipseArc(pattern, item);   }
        else if (objType == OBJ_TYPE_GRID)         { addGrid(pattern, item);         }
        else if (objType == OBJ_TYPE_HATCH)        { addHatch(pattern, item);        }
        else if (objType == OBJ_TYPE_IMAGE)        { addImage(pattern, item);        }
        else if (objType == OBJ_TYPE_INFINITELINE) { addInfiniteLine(pattern, item); }
        else if (objType == OBJ_TYPE_LINE)         { addLine(pattern, item);         }
        else if (objType == OBJ_TYPE_POINT)        { addPoint(pattern, item);        }
        else if (objType == OBJ_TYPE_POLYGON)      { addPolygon(pattern, item);      }
        else if (objType == OBJ_TYPE_POLYLINE)     { addPolyline(pattern, item);     }
        else if (objType == OBJ_TYPE_RAY)          { addRay(pattern, item);          }
        else if (objType == OBJ_TYPE_RECTANGLE)    { addRectangle(pattern, item);    }
        else if (objType == OBJ_TYPE_SPLINE)       { addSpline(pattern, item);       }
        else if (objType == OBJ_TYPE_TEXTMULTI)    { addTextMulti(pattern, item);    }
        else if (objType == OBJ_TYPE_TEXTSINGLE)   { addTextSingle(pattern, item);   }
    }

    //TODO: handle EMBFORMAT_STCHANDOBJ also
    if (formatTable[formatType].type == EMBFORMAT_STITCHONLY) {
        embPattern_movePolylinesToStitchList(pattern);
        //TODO: handle all objects like this
    }

    std::string s = "output";
    s += formatTable[formatType].extension;

    writeSuccessful = embPattern_write(pattern, qPrintable(s), formatType);
    if (!writeSuccessful) {
        debug_message("Writing file %s was unsuccessful", qPrintable(fileName));
    }

    //TODO: check the embLog for errors and if any exist, report them.

    embPattern_free(pattern);

    return writeSuccessful;
}

void SaveObject::addArc(EmbPattern* pattern, QGraphicsItem* item)
{
}

void SaveObject::addBlock(EmbPattern* pattern, QGraphicsItem* item)
{
}

void SaveObject::addCircle(EmbPattern* pattern, QGraphicsItem* item)
{
    CircleObject* obj = static_cast<CircleObject*>(item);
    if (obj) {
        if (formatType == EMBFORMAT_STITCHONLY) {
            QPainterPath path = obj->objectSavePath();
            toPolyline(pattern, obj->objectCenter(), path.simplified(), "0", obj->objectColor(), "CONTINUOUS", "BYLAYER"); //TODO: proper layer/lineType/lineWeight //TODO: Improve precision, replace simplified
        }
        else {
            EmbCircle circle;
            circle.center.x = (double)obj->objectCenterX();
            circle.center.y = (double)obj->objectCenterY();
            circle.radius = (double)obj->objectRadius();
            embPattern_addCircleAbs(pattern, circle);
        }
    }
}

void SaveObject::addDimAligned(EmbPattern* pattern, QGraphicsItem* item)
{
}

void SaveObject::addDimAngular(EmbPattern* pattern, QGraphicsItem* item)
{
}

void SaveObject::addDimArcLength(EmbPattern* pattern, QGraphicsItem* item)
{
}

void SaveObject::addDimDiameter(EmbPattern* pattern, QGraphicsItem* item)
{
}

void SaveObject::addDimLeader(EmbPattern* pattern, QGraphicsItem* item)
{
}

void SaveObject::addDimLinear(EmbPattern* pattern, QGraphicsItem* item)
{
}

void SaveObject::addDimOrdinate(EmbPattern* pattern, QGraphicsItem* item)
{
}

void SaveObject::addDimRadius(EmbPattern* pattern, QGraphicsItem* item)
{
}

void SaveObject::addEllipse(EmbPattern* pattern, QGraphicsItem* item)
{
    EllipseObject* obj = static_cast<EllipseObject*>(item);
    if (obj) {
        if (formatType == EMBFORMAT_STITCHONLY) {
            QPainterPath path = obj->objectSavePath();
            toPolyline(pattern, obj->objectCenter(), path.simplified(), "0", obj->objectColor(), "CONTINUOUS", "BYLAYER"); //TODO: proper layer/lineType/lineWeight //TODO: Improve precision, replace simplified
        }
        else {
            //TODO: ellipse rotation
            EmbEllipse ellipse;
            ellipse.center.x = (double)obj->objectCenterX();
            ellipse.center.y = (double)obj->objectCenterY();
            ellipse.radius.x = (double)obj->objectWidth()/2.0;
            ellipse.radius.y = (double)obj->objectHeight()/2.0;
            embPattern_addEllipseAbs(pattern, ellipse);
        }
    }
}

void SaveObject::addEllipseArc(EmbPattern* pattern, QGraphicsItem* item)
{
}

void SaveObject::addGrid(EmbPattern* pattern, QGraphicsItem* item)
{
}

void SaveObject::addHatch(EmbPattern* pattern, QGraphicsItem* item)
{
}

void SaveObject::addImage(EmbPattern* pattern, QGraphicsItem* item)
{
}

void SaveObject::addInfiniteLine(EmbPattern* pattern, QGraphicsItem* item)
{
}

void SaveObject::addLine(EmbPattern* pattern, QGraphicsItem* item)
{
    LineObject* obj = static_cast<LineObject*>(item);
    if (obj) {
        if (formatType == EMBFORMAT_STITCHONLY) {
            toPolyline(pattern, obj->objectEndPoint1(), obj->objectSavePath(), "0", obj->objectColor(), "CONTINUOUS", "BYLAYER"); //TODO: proper layer/lineType/lineWeight
        }
        else {
            EmbLine line;
            line.start.x = (double)obj->objectX1();
            line.start.y = (double)obj->objectY1();
            line.end.x = (double)obj->objectX2();
            line.end.y = (double)obj->objectY2();
            embPattern_addLineAbs(pattern, line);
        }
    }
}

void SaveObject::addPath(EmbPattern* pattern, QGraphicsItem* item)
{
    //TODO: Reimplement addPolyline() using the libembroidery C API
    /*
    debug_message("addPolyline()");
    QGraphicsPathItem* polylineItem = (QGraphicsPathItem*)item;
    if (polylineItem)
    {
        QPainterPath path = polylineItem->path();
        EmbVector pos = polylineItem->pos();
        double startX = pos.x();
        double startY = pos.y();

        QPainterPath::Element element;
        QPainterPath::Element P1;
        QPainterPath::Element P2;
        QPainterPath::Element P3;
        QPainterPath::Element P4;

        for(int i = 0; i < path.elementCount()-1; ++i)
        {
            element = path.elementAt(i);
            if (element.isMoveTo())
            {
                pattern.AddStitchAbs((element.x + startX), -(element.y + startY), TRIM);
            }
            else if (element.isLineTo())
            {
                pattern.AddStitchAbs((element.x + startX), -(element.y + startY), NORMAL);
            }
            else if (element.isCurveTo())
            {
                P1 = path.elementAt(i-1); // start point
                P2 = path.elementAt(i);   // control point
                P3 = path.elementAt(i+1); // control point
                P4 = path.elementAt(i+2); // end point

                pattern.AddStitchAbs(P4.x, -P4.y, NORMAL); //TODO: This is temporary
                //TODO: Curved Polyline segments are always arcs
            }
        }
        pattern.AddStitchRel(0, 0, STOP);
        QColor c= polylineItem->pen().color();
        pattern.AddColor(c.red(), c.green(), c.blue(), "", "");
    }
    */
}

void SaveObject::addPoint(EmbPattern* pattern, QGraphicsItem* item)
{
    PointObject* obj = static_cast<PointObject*>(item);
    if (obj) {
        if (formatType == EMBFORMAT_STITCHONLY) {
            toPolyline(pattern, obj->objectPos(), obj->objectSavePath(), "0", obj->objectColor(), "CONTINUOUS", "BYLAYER"); //TODO: proper layer/lineType/lineWeight
        }
        else {
            EmbPoint point;
            point.position.x = (double)obj->objectX();
            point.position.y = (double)obj->objectY();
            embPattern_addPointAbs(pattern, point);
        }
    }
}

void SaveObject::addPolygon(EmbPattern* pattern, QGraphicsItem* item)
{
    PolygonObject* obj = static_cast<PolygonObject*>(item);
    if (obj) {
        toPolyline(pattern, obj->objectPos(), obj->objectSavePath(), "0", obj->objectColor(), "CONTINUOUS", "BYLAYER"); //TODO: proper layer/lineType/lineWeight
    }
}

void SaveObject::addPolyline(EmbPattern* pattern, QGraphicsItem* item)
{
#if 0
    PolylineObject* obj = static_cast<PolylineObject*>(item);
    if (obj)
    {
        toPolyline(pattern, obj->objectPos(), obj->objectSavePath(), "0", obj->objectColor(), "CONTINUOUS", "BYLAYER"); //TODO: proper layer/lineType/lineWeight
    }
#endif
}

void SaveObject::addRay(EmbPattern* pattern, QGraphicsItem* item)
{
}

void SaveObject::addRectangle(EmbPattern* pattern, QGraphicsItem* item)
{
    RectObject* obj = static_cast<RectObject*>(item);
    if (obj)
    {
        if (formatType == EMBFORMAT_STITCHONLY)
        {
            toPolyline(pattern, obj->objectPos(), obj->objectSavePath(), "0", obj->objectColor(), "CONTINUOUS", "BYLAYER"); //TODO: proper layer/lineType/lineWeight
        }
        else
        {
            //TODO: Review this at some point
            EmbVector topLeft = obj->objectTopLeft();
            EmbRect rect;
            rect.left = (double)topLeft.x();
            rect.top = (double)topLeft.y();
            rect.bottom = rect.top + (double)obj->objectHeight();
            rect.right = rect.left + (double)obj->objectWidth();
            embPattern_addRectAbs(pattern, rect);
        }
    }
}

void SaveObject::addSlot(EmbPattern* pattern, QGraphicsItem* item)
{
}

void SaveObject::addSpline(EmbPattern* pattern, QGraphicsItem* item)
{
    //TODO: abstract bezier into geom-bezier... cubicBezierMagic(P1, P2, P3, P4, 0.0, 1.0, tPoints);
}

void SaveObject::addTextMulti(EmbPattern* pattern, QGraphicsItem* item)
{
    //TODO: saving polygons, polylines and paths must be stable before we go here.
}

void SaveObject::addTextSingle(EmbPattern* pattern, QGraphicsItem* item)
{
    //TODO: saving polygons, polylines and paths must be stable before we go here.

    //TODO: This needs to work like a path, not a polyline. Improve this
    TextSingleObject* obj = static_cast<TextSingleObject*>(item);
    if (obj)
    {
        if (formatType == EMBFORMAT_STITCHONLY)
        {
            QList<QPainterPath> pathList = obj->objectSavePathList();
            foreach(QPainterPath path, pathList)
            {
                toPolyline(pattern, obj->objectPos(), path.simplified(), "0", obj->objectColor(), "CONTINUOUS", "BYLAYER"); //TODO: proper layer/lineType/lineWeight //TODO: Improve precision, replace simplified
            }
        }
        else
        {

        }
    }
}

//NOTE: This function should be used to interpret various object types and save them as polylines for stitchOnly formats.
void SaveObject::toPolyline(EmbPattern* pattern, const EmbVector& objPos, const QPainterPath& objPath, const std::string& layer, const QColor& color, const std::string& lineType, const std::string& lineWeight)
{
    double startX = objPos.x();
    double startY = objPos.y();
    EmbArray *pointList = embArray_create(EMB_POINT);
    EmbPoint lastPoint;
    QPainterPath::Element element;
    for (int i = 0; i < objPath.elementCount(); ++i) {
        element = objPath.elementAt(i);
        if (pointList->count == 0) {
            lastPoint.position.x = element.x + startX;
            lastPoint.position.y = -(element.y + startY);
            embArray_addPoint(pointList, lastPoint);
        }
        else {
            lastPoint.position.x += element.x + startX;
            lastPoint.position.y += -(element.y + startY);
        }
    }

    EmbPolyline polyObject;
    polyObject.pointList = pointList;
    polyObject.color.r = color.red();
    polyObject.color.g = color.green();
    polyObject.color.b = color.blue();
    //TODO: proper lineType
    #if 0
    embPattern_addPolylineAbs(pattern, polyObject);
    #endif
}
#endif
