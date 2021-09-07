#include "embroidermodder.h"

#include <QStyleOption>
#include <QGraphicsScene>
#include <QMessageBox>

PolylineObject::PolylineObject(qreal x, qreal y, const QPainterPath& p, QRgb rgb, QGraphicsItem* parent) : BaseObject(parent)
{
    qDebug("PolylineObject Constructor()");
    init(x, y, p, rgb, Qt::SolidLine); //TODO: getCurrentLineType
}

PolylineObject::PolylineObject(PolylineObject* obj, QGraphicsItem* parent) : BaseObject(parent)
{
    qDebug("PolylineObject Constructor()");
    if(obj)
    {
        init(obj->objectX(), obj->objectY(), obj->objectCopyPath(), obj->objectColorRGB(), Qt::SolidLine); //TODO: getCurrentLineType
        setRotation(obj->rotation());
        setScale(obj->scale());
    }
}

PolylineObject::~PolylineObject()
{
    qDebug("PolylineObject Destructor()");
}

void PolylineObject::init(qreal x, qreal y, const QPainterPath& p, QRgb rgb, Qt::PenStyle lineType)
{
    setData(OBJ_TYPE, PolylineObject::Type);
    setData(OBJ_NAME, OBJ_NAME_POLYLINE);

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

void PolylineObject::updatePath(const QPainterPath& p)
{
    normalPath = p;
    QPainterPath reversePath = normalPath.toReversed();
    reversePath.connectPath(normalPath);
    setObjectPath(reversePath);
}

void PolylineObject::paint(QPainter* painter, const QStyleOptionGraphicsItem* option, QWidget* /*widget*/)
{
    QGraphicsScene* objScene = scene();
    if(!objScene) return;

    QPen paintPen = pen();
    painter->setPen(paintPen);
    updateRubber(painter);
    if(option->state & QStyle::State_Selected)  { paintPen.setStyle(Qt::DashLine); }
    if(objScene->property(ENABLE_LWT).toBool()) { paintPen = lineWeightPen(); }
    painter->setPen(paintPen);

    painter->drawPath(normalPath);

    if(objScene->property(ENABLE_LWT).toBool() && objScene->property(ENABLE_REAL).toBool()) { realRender(painter, normalPath); }
}

void PolylineObject::updateRubber(QPainter* painter)
{
    int rubberMode = objectRubberMode();
    if(rubberMode == OBJ_RUBBER_POLYLINE)
    {
        setObjectPos(objectRubberPoint("POLYLINE_POINT_0"));

        QLineF rubberLine(normalPath.currentPosition(), mapFromScene(objectRubberPoint(QString())));
        if(painter) drawRubberLine(rubberLine, painter, VIEW_COLOR_CROSSHAIR);

        bool ok = false;
        QString numStr = objectRubberText("POLYLINE_NUM_POINTS");
        if(numStr.isNull()) return;
        int num = numStr.toInt(&ok);
        if(!ok) return;

        QString appendStr;
        QPainterPath rubberPath;
        for(int i = 1; i <= num; i++)
        {
            appendStr = "POLYLINE_POINT_" + QString().setNum(i);
            QPointF appendPoint = mapFromScene(objectRubberPoint(appendStr));
            rubberPath.lineTo(appendPoint);
        }
        updatePath(rubberPath);

        //Ensure the path isn't updated until the number of points is changed again
        setObjectRubberText("POLYLINE_NUM_POINTS", QString());
    }
    else if(rubberMode == OBJ_RUBBER_GRIP)
    {
        if(painter)
        {
            int elemCount = normalPath.elementCount();
            QPointF gripPoint = objectRubberPoint("GRIP_POINT");
            if(gripIndex == -1) gripIndex = findIndex(gripPoint);
            if(gripIndex == -1) return;

            if(!gripIndex) //First
            {
                QPainterPath::Element ef = normalPath.elementAt(1);
                QPointF efPoint = QPointF(ef.x, ef.y);
                painter->drawLine(efPoint, mapFromScene(objectRubberPoint(QString())));
            }
            else if(gripIndex == elemCount-1) //Last
            {
                QPainterPath::Element el = normalPath.elementAt(gripIndex-1);
                QPointF elPoint = QPointF(el.x, el.y);
                painter->drawLine(elPoint, mapFromScene(objectRubberPoint(QString())));
            }
            else //Middle
            {
                QPainterPath::Element em = normalPath.elementAt(gripIndex-1);
                QPainterPath::Element en = normalPath.elementAt(gripIndex+1);
                QPointF emPoint = QPointF(em.x, em.y);
                QPointF enPoint = QPointF(en.x, en.y);
                painter->drawLine(emPoint, mapFromScene(objectRubberPoint(QString())));
                painter->drawLine(enPoint, mapFromScene(objectRubberPoint(QString())));
            }

            QLineF rubLine(mapFromScene(gripPoint), mapFromScene(objectRubberPoint(QString())));
            drawRubberLine(rubLine, painter, VIEW_COLOR_CROSSHAIR);
        }
    }
}

void PolylineObject::vulcanize()
{
    qDebug("PolylineObject vulcanize()");
    updateRubber();

    setObjectRubberMode(OBJ_RUBBER_OFF);

    if(!normalPath.elementCount())
        QMessageBox::critical(0, QObject::tr("Empty Polyline Error"), QObject::tr("The polyline added contains no points. The command that created this object has flawed logic."));
}

// Returns the closest snap point to the mouse point
QPointF PolylineObject::mouseSnapPoint(const QPointF& mousePoint)
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

QList<QPointF> PolylineObject::allGripPoints()
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

int PolylineObject::findIndex(const QPointF& point)
{
    int elemCount = normalPath.elementCount();
    //NOTE: Points here are in item coordinates
    QPointF itemPoint = mapFromScene(point);
    for(int i = 0; i < elemCount; i++)
    {
        QPainterPath::Element e = normalPath.elementAt(i);
        QPointF elemPoint = QPointF(e.x, e.y);
        if(itemPoint == elemPoint) return i;
    }
    return -1;
}

void PolylineObject::gripEdit(const QPointF& before, const QPointF& after)
{
    gripIndex = findIndex(before);
    if(gripIndex == -1) return;
    QPointF a = mapFromScene(after);
    normalPath.setElementPositionAt(gripIndex, a.x(), a.y());
    updatePath(normalPath);
    gripIndex = -1;
}

QPainterPath PolylineObject::objectCopyPath() const
{
    return normalPath;
}

QPainterPath PolylineObject::objectSavePath() const
{
    qreal s = scale();
    QTransform trans;
    trans.rotate(rotation());
    trans.scale(s,s);
    return trans.map(normalPath);
}

/* kate: bom off; indent-mode cstyle; indent-width 4; replace-trailing-space-save on; */
