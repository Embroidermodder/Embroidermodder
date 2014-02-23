#include "object-save.h"
#include "object-data.h"

#include "object-arc.h"
#include "object-circle.h"
#include "object-dimleader.h"
#include "object-ellipse.h"
#include "object-line.h"
#include "object-point.h"
#include "object-polyline.h"
#include "object-rect.h"
#include "object-textsingle.h"

#include "emb-color.h"
#include "emb-format.h"

#include <QGraphicsScene>
#include <QGraphicsItem>

SaveObject::SaveObject(QGraphicsScene* theScene, QObject* parent) : QObject(parent)
{
    qDebug("SaveObject Constructor()");
    gscene = theScene;
    formatType = EMBFORMAT_UNSUPPORTED;
}

SaveObject::~SaveObject()
{
    qDebug("SaveObject Destructor()");
}

bool SaveObject::save(const QString &fileName)
{
    qDebug("SaveObject save(%s)", qPrintable(fileName));

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

    char* unusedStr = 0;
    char unusedChar;

    if(!embFormat_info(qPrintable(fileName), &unusedStr, &unusedStr, &unusedChar, &unusedChar, &formatType))
        return false;
    if(formatType == EMBFORMAT_UNSUPPORTED)
        return false;

    EmbPattern* pattern = 0;
    EmbReaderWriter* writer = 0;

    pattern = embPattern_create();
    if(!pattern) { qDebug("Could not allocate memory for embroidery pattern"); }

    /* Write */
    writer = embReaderWriter_getByFileName(qPrintable(fileName));
    if(!writer) { qDebug("Unsupported write file type: %s", qPrintable(fileName)); }
    else
    {
        foreach(QGraphicsItem* item, gscene->items())
        {
            int objType = item->data(OBJ_TYPE).toInt();

            if     (objType == OBJ_TYPE_ARC)          { addArc(pattern, item);          }
            else if(objType == OBJ_TYPE_BLOCK)        { addBlock(pattern, item);        }
            else if(objType == OBJ_TYPE_CIRCLE)       { addCircle(pattern, item);       }
            else if(objType == OBJ_TYPE_DIMALIGNED)   { addDimAligned(pattern, item);   }
            else if(objType == OBJ_TYPE_DIMANGULAR)   { addDimAngular(pattern, item);   }
            else if(objType == OBJ_TYPE_DIMARCLENGTH) { addDimArcLength(pattern, item); }
            else if(objType == OBJ_TYPE_DIMDIAMETER)  { addDimDiameter(pattern, item);  }
            else if(objType == OBJ_TYPE_DIMLEADER)    { addDimLeader(pattern, item);    }
            else if(objType == OBJ_TYPE_DIMLINEAR)    { addDimLinear(pattern, item);    }
            else if(objType == OBJ_TYPE_DIMORDINATE)  { addDimOrdinate(pattern, item);  }
            else if(objType == OBJ_TYPE_DIMRADIUS)    { addDimRadius(pattern, item);    }
            else if(objType == OBJ_TYPE_ELLIPSE)      { addEllipse(pattern, item);      }
            else if(objType == OBJ_TYPE_ELLIPSEARC)   { addEllipseArc(pattern, item);   }
            else if(objType == OBJ_TYPE_GRID)         { addGrid(pattern, item);         }
            else if(objType == OBJ_TYPE_HATCH)        { addHatch(pattern, item);        }
            else if(objType == OBJ_TYPE_IMAGE)        { addImage(pattern, item);        }
            else if(objType == OBJ_TYPE_INFINITELINE) { addInfiniteLine(pattern, item); }
            else if(objType == OBJ_TYPE_LINE)         { addLine(pattern, item);         }
            else if(objType == OBJ_TYPE_POINT)        { addPoint(pattern, item);        }
            else if(objType == OBJ_TYPE_POLYGON)      { addPolygon(pattern, item);      }
            else if(objType == OBJ_TYPE_POLYLINE)     { addPolyline(pattern, item);     }
            else if(objType == OBJ_TYPE_RAY)          { addRay(pattern, item);          }
            else if(objType == OBJ_TYPE_RECTANGLE)    { addRectangle(pattern, item);    }
            else if(objType == OBJ_TYPE_SPLINE)       { addSpline(pattern, item);       }
            else if(objType == OBJ_TYPE_TEXTMULTI)    { addTextMulti(pattern, item);    }
            else if(objType == OBJ_TYPE_TEXTSINGLE)   { addTextSingle(pattern, item);   }
        }

        //TODO: handle EMBFORMAT_STCHANDOBJ also
        if(formatType == EMBFORMAT_STITCHONLY)
            embPattern_movePolylinesToStitchList(pattern); //TODO: handle all objects like this

        writeSuccessful = writer->writer(pattern, qPrintable(fileName));
        if(!writeSuccessful) { qDebug("Writing file %s was unsuccessful", qPrintable(fileName)); }
    }

    free(writer);
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
    if(obj)
    {
        if(formatType == EMBFORMAT_STITCHONLY)
        {
            QPainterPath path;
            path.addEllipse(QPointF(0,0), obj->objectRadius(), obj->objectRadius());
            path = path.simplified();
            toPolyline(pattern, obj->objectCenter(), path, "0", obj->objectColor(), "CONTINUOUS", "BYLAYER"); //TODO: proper layer/lineType/lineWeight
        }
        else
        {
            embPattern_addCircleObjectAbs(pattern, (double)obj->objectCenterX(), (double)obj->objectCenterY(), (double)obj->objectRadius());
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
    if(obj)
    {
        if(formatType == EMBFORMAT_STITCHONLY)
        {
            QPainterPath path;
            path.addEllipse(QPointF(0,0), obj->objectWidth()/2.0, obj->objectHeight()/2.0);
            path = path.simplified();
            toPolyline(pattern, obj->objectCenter(), path, "0", obj->objectColor(), "CONTINUOUS", "BYLAYER"); //TODO: proper layer/lineType/lineWeight
        }
        else
        {
            //TODO: ellipse rotation
            embPattern_addEllipseObjectAbs(pattern, (double)obj->objectCenterX(), (double)obj->objectCenterY(), (double)obj->objectWidth()/2.0, (double)obj->objectHeight()/2.0);
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
    if(obj)
    {
        if(formatType == EMBFORMAT_STITCHONLY)
        {
            QPainterPath path;
            path.lineTo(obj->objectDeltaX(), obj->objectDeltaY());
            toPolyline(pattern, obj->objectEndPoint1(), path, "0", obj->objectColor(), "CONTINUOUS", "BYLAYER"); //TODO: proper layer/lineType/lineWeight
        }
        else
        {
            embPattern_addLineObjectAbs(pattern, (double)obj->objectX1(), (double)obj->objectY1(), (double)obj->objectX2(), (double)obj->objectY2());
        }
    }
}

void SaveObject::addPath(EmbPattern* pattern, QGraphicsItem* item)
{
    //TODO: Reimplement addPolyline() using the libembroidery C API
    /*
    qDebug("addPolyline()");
    QGraphicsPathItem* polylineItem = (QGraphicsPathItem*)item;
    if(polylineItem)
    {
        QPainterPath path = polylineItem->path();
        QPointF pos = polylineItem->pos();
        qreal startX = pos.x();
        qreal startY = pos.y();

        QPainterPath::Element element;
        QPainterPath::Element P1;
        QPainterPath::Element P2;
        QPainterPath::Element P3;
        QPainterPath::Element P4;

        for(int i = 0; i < path.elementCount()-1; ++i)
        {
            element = path.elementAt(i);
            if(element.isMoveTo())
            {
                pattern.AddStitchAbs((element.x + startX), -(element.y + startY), TRIM);
            }
            else if(element.isLineTo())
            {
                pattern.AddStitchAbs((element.x + startX), -(element.y + startY), NORMAL);
            }
            else if(element.isCurveTo())
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
    if(obj)
    {
        if(formatType == EMBFORMAT_STITCHONLY)
        {
            QPainterPath path;
            path.addRect(-0.00000001, -0.00000001, 0.00000002, 0.00000002);
            toPolyline(pattern, obj->objectPos(), path, "0", obj->objectColor(), "CONTINUOUS", "BYLAYER"); //TODO: proper layer/lineType/lineWeight
        }
        else
        {
            embPattern_addPointObjectAbs(pattern, (double)obj->objectX(), (double)obj->objectY());
        }
    }
}

void SaveObject::addPolygon(EmbPattern* pattern, QGraphicsItem* item)
{
}

void SaveObject::addPolyline(EmbPattern* pattern, QGraphicsItem* item)
{
    PolylineObject* obj = static_cast<PolylineObject*>(item);
    if(obj)
    {
        toPolyline(pattern, obj->pos(), obj->objectSavePath(), "0", obj->objectColor(), "CONTINUOUS", "BYLAYER"); //TODO: proper layer/lineType/lineWeight
    }
}

void SaveObject::addRay(EmbPattern* pattern, QGraphicsItem* item)
{
}

void SaveObject::addRectangle(EmbPattern* pattern, QGraphicsItem* item)
{
    RectObject* obj = static_cast<RectObject*>(item);
    if(obj)
    {
        if(formatType == EMBFORMAT_STITCHONLY)
        {
            //TODO: This needs work
            QPainterPath path;
            path.addRect(obj->objectTopLeft().x(), obj->objectTopLeft().y(), obj->objectWidth(), obj->objectHeight());
            toPolyline(pattern, obj->objectTopLeft(), path, "0", obj->objectColor(), "CONTINUOUS", "BYLAYER"); //TODO: proper layer/lineType/lineWeight
        }
        else
        {
            //TODO: Review this at some point
            QPointF topLeft = obj->objectTopLeft();
            embPattern_addRectObjectAbs(pattern, (double)topLeft.x(), (double)topLeft.y(), (double)obj->objectWidth(), (double)obj->objectHeight());
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
    if(obj)
    {
        if(formatType == EMBFORMAT_STITCHONLY)
        {
            QTransform transform;
            transform.rotate(obj->rotation());
            QPainterPath path = transform.map(obj->objectSavePath().simplified());
            toPolyline(pattern, obj->objectPos(), path, "0", obj->objectColor(), "CONTINUOUS", "BYLAYER"); //TODO: proper layer/lineType/lineWeight
        }
        else
        {

        }
    }
}

//NOTE: This function should be used to interpret various object types and save them as polylines for stitchOnly formats.
void SaveObject::toPolyline(EmbPattern* pattern, const QPointF& objPos, const QPainterPath& objPath, const QString& layer, const QColor& color, const QString& lineType, const QString& lineWeight)
{
    EmbPolylineObject* polyObject = (EmbPolylineObject*) malloc(sizeof(EmbPolylineObject));
    if(!polyObject) {  } //TODO: error
    polyObject->color = embColor_make(color.red(), color.green(), color.blue());
    //polyObject->lineType = obj->type(); //TODO: This is wrong! type() is not lineType!

    qreal startX = objPos.x();
    qreal startY = objPos.y();
    EmbPointList* pointList = 0;
    QPainterPath::Element element;
    for(int i = 0; i < objPath.elementCount(); ++i)
    {
        element = objPath.elementAt(i);
        if(!pointList)
        {
            polyObject->pointList = pointList = embPointList_create(element.x + startX, -(element.y + startY));
        }
        else
        {
            pointList = embPointList_add(pointList, embPoint_make(element.x + startX, -(element.y + startY)));
        }
    }

    embPattern_addPolylineObjectAbs(pattern, polyObject);
}

/* kate: bom off; indent-mode cstyle; indent-width 4; replace-trailing-space-save on; */
