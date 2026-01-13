#include "object-ellipse.h"
#include "object-data.h"

#include <QPainter>
#include <QStyleOption>
#include <QGraphicsScene>

EllipseObject::EllipseObject(qreal centerX, qreal centerY, qreal width, qreal height, QRgb rgb, QGraphicsItem* parent) : BaseObject(parent)
{
    qDebug("EllipseObject Constructor()");
    init(centerX, centerY, width, height, rgb, Qt::SolidLine); //TODO: getCurrentLineType
}

EllipseObject::EllipseObject(EllipseObject* obj, QGraphicsItem* parent) : BaseObject(parent)
{
    qDebug("EllipseObject Constructor()");
    if(obj)
    {
        init(obj->objectCenterX(), obj->objectCenterY(), obj->objectWidth(), obj->objectHeight(), obj->objectColorRGB(), Qt::SolidLine); //TODO: getCurrentLineType
        setRotation(obj->rotation());
    }
}

EllipseObject::~EllipseObject()
{
    qDebug("EllipseObject Destructor()");
}

void EllipseObject::init(qreal centerX, qreal centerY, qreal width, qreal height, QRgb rgb, Qt::PenStyle lineType)
{
    setData(OBJ_TYPE, type());
    setData(OBJ_NAME, OBJ_NAME_ELLIPSE);

    //WARNING: DO NOT enable QGraphicsItem::ItemIsMovable. If it is enabled,
    //WARNING: and the item is double clicked, the scene will erratically move the item while zooming.
    //WARNING: All movement has to be handled explicitly by us, not by the scene.
    setFlag(QGraphicsItem::ItemIsSelectable, true);

    setObjectSize(width, height);
    setObjectCenter(centerX, centerY);
    setObjectColor(rgb);
    setObjectLineType(lineType);
    setObjectLineWeight(0.35); //TODO: pass in proper lineweight
    setPen(objectPen());
    updatePath();
}

void EllipseObject::setObjectSize(qreal width, qreal height)
{
    QRectF elRect = rect();
    elRect.setWidth(width);
    elRect.setHeight(height);
    elRect.moveCenter(QPointF(0,0));
    setRect(elRect);
}

void EllipseObject::setObjectCenter(const QPointF& center)
{
    setObjectCenter(center.x(), center.y());
}

void EllipseObject::setObjectCenter(qreal centerX, qreal centerY)
{
    setPos(centerX, centerY);
}

void EllipseObject::setObjectCenterX(qreal centerX)
{
    setX(centerX);
}

void EllipseObject::setObjectCenterY(qreal centerY)
{
    setY(centerY);
}

void EllipseObject::setObjectRadiusMajor(qreal radius)
{
    setObjectDiameterMajor(radius*2.0);
}

void EllipseObject::setObjectRadiusMinor(qreal radius)
{
    setObjectDiameterMinor(radius*2.0);
}

void EllipseObject::setObjectDiameterMajor(qreal diameter)
{
    QRectF elRect = rect();
    if(elRect.width() > elRect.height())
        elRect.setWidth(diameter);
    else
        elRect.setHeight(diameter);
    elRect.moveCenter(QPointF(0,0));
    setRect(elRect);
}

void EllipseObject::setObjectDiameterMinor(qreal diameter)
{
    QRectF elRect = rect();
    if(elRect.width() < elRect.height())
        elRect.setWidth(diameter);
    else
        elRect.setHeight(diameter);
    elRect.moveCenter(QPointF(0,0));
    setRect(elRect);
}

QPointF EllipseObject::objectQuadrant0() const
{
    qreal halfW = objectWidth()/2.0;
    qreal rot = radians(rotation());
    qreal x = halfW*qCos(rot);
    qreal y = halfW*qSin(rot);
    return objectCenter() + QPointF(x,y);
}

QPointF EllipseObject::objectQuadrant90() const
{
    qreal halfH = objectHeight()/2.0;
    qreal rot = radians(rotation()+90.0);
    qreal x = halfH*qCos(rot);
    qreal y = halfH*qSin(rot);
    return objectCenter() + QPointF(x,y);
}

QPointF EllipseObject::objectQuadrant180() const
{
    qreal halfW = objectWidth()/2.0;
    qreal rot = radians(rotation()+180.0);
    qreal x = halfW*qCos(rot);
    qreal y = halfW*qSin(rot);
    return objectCenter() + QPointF(x,y);
}

QPointF EllipseObject::objectQuadrant270() const
{
    qreal halfH = objectHeight()/2.0;
    qreal rot = radians(rotation()+270.0);
    qreal x = halfH*qCos(rot);
    qreal y = halfH*qSin(rot);
    return objectCenter() + QPointF(x,y);
}

void EllipseObject::updatePath()
{
    QPainterPath path;
    QRectF r = rect();
    path.arcMoveTo(r, 0);
    path.arcTo(r, 0, 360);
    //NOTE: Reverse the path so that the inside area isn't considered part of the ellipse
    path.arcTo(r, 0, -360);
    setObjectPath(path);
}

void EllipseObject::paint(QPainter* painter, const QStyleOptionGraphicsItem* option, QWidget* /*widget*/)
{
    QGraphicsScene* objScene = scene();
    if(!objScene) return;

    QPen paintPen = pen();
    painter->setPen(paintPen);
    updateRubber(painter);
    if(option->state & QStyle::State_Selected)  { paintPen.setStyle(Qt::DashLine); }
    if(objScene->property(ENABLE_LWT).toBool()) { paintPen = lineWeightPen(); }
    painter->setPen(paintPen);

    painter->drawEllipse(rect());
}

void EllipseObject::updateRubber(QPainter* painter)
{
    int rubberMode = objectRubberMode();
    if(rubberMode == OBJ_RUBBER_ELLIPSE_LINE)
    {
        QPointF sceneLinePoint1 = objectRubberPoint("ELLIPSE_LINE_POINT1");
        QPointF sceneLinePoint2 = objectRubberPoint("ELLIPSE_LINE_POINT2");
        QPointF itemLinePoint1  = mapFromScene(sceneLinePoint1);
        QPointF itemLinePoint2  = mapFromScene(sceneLinePoint2);
        QLineF itemLine(itemLinePoint1, itemLinePoint2);
        if(painter) drawRubberLine(itemLine, painter, VIEW_COLOR_CROSSHAIR);
        updatePath();
    }
    else if(rubberMode == OBJ_RUBBER_ELLIPSE_MAJORDIAMETER_MINORRADIUS)
    {
        QPointF sceneAxis1Point1 = objectRubberPoint("ELLIPSE_AXIS1_POINT1");
        QPointF sceneAxis1Point2 = objectRubberPoint("ELLIPSE_AXIS1_POINT2");
        QPointF sceneCenterPoint = objectRubberPoint("ELLIPSE_CENTER");
        QPointF sceneAxis2Point2 = objectRubberPoint("ELLIPSE_AXIS2_POINT2");
        qreal ellipseWidth = objectRubberPoint("ELLIPSE_WIDTH").x();
        qreal ellipseRot = objectRubberPoint("ELLIPSE_ROT").x();

        //TODO: incorporate perpendicularDistance() into libcgeometry
        qreal px = sceneAxis2Point2.x();
        qreal py = sceneAxis2Point2.y();
        qreal x1 = sceneAxis1Point1.x();
        qreal y1 = sceneAxis1Point1.y();
        QLineF line(sceneAxis1Point1, sceneAxis1Point2);
        QLineF norm = line.normalVector();
        qreal dx = px-x1;
        qreal dy = py-y1;
        norm.translate(dx, dy);
        QPointF iPoint;
        norm.intersects(line, &iPoint);
        qreal ellipseHeight = QLineF(px, py, iPoint.x(), iPoint.y()).length()*2.0;

        setObjectCenter(sceneCenterPoint);
        setObjectSize(ellipseWidth, ellipseHeight);
        setRotation(-ellipseRot);

        QPointF itemCenterPoint = mapFromScene(sceneCenterPoint);
        QPointF itemAxis2Point2 = mapFromScene(sceneAxis2Point2);
        QLineF itemLine(itemCenterPoint, itemAxis2Point2);
        if(painter) drawRubberLine(itemLine, painter, VIEW_COLOR_CROSSHAIR);
        updatePath();
    }
    else if(rubberMode == OBJ_RUBBER_ELLIPSE_MAJORRADIUS_MINORRADIUS)
    {
        QPointF sceneAxis1Point2 = objectRubberPoint("ELLIPSE_AXIS1_POINT2");
        QPointF sceneCenterPoint = objectRubberPoint("ELLIPSE_CENTER");
        QPointF sceneAxis2Point2 = objectRubberPoint("ELLIPSE_AXIS2_POINT2");
        qreal ellipseWidth = objectRubberPoint("ELLIPSE_WIDTH").x();
        qreal ellipseRot = objectRubberPoint("ELLIPSE_ROT").x();

        //TODO: incorporate perpendicularDistance() into libcgeometry
        qreal px = sceneAxis2Point2.x();
        qreal py = sceneAxis2Point2.y();
        qreal x1 = sceneCenterPoint.x();
        qreal y1 = sceneCenterPoint.y();
        QLineF line(sceneCenterPoint, sceneAxis1Point2);
        QLineF norm = line.normalVector();
        qreal dx = px-x1;
        qreal dy = py-y1;
        norm.translate(dx, dy);
        QPointF iPoint;
        norm.intersects(line, &iPoint);
        qreal ellipseHeight = QLineF(px, py, iPoint.x(), iPoint.y()).length()*2.0;

        setObjectCenter(sceneCenterPoint);
        setObjectSize(ellipseWidth, ellipseHeight);
        setRotation(-ellipseRot);

        QPointF itemCenterPoint = mapFromScene(sceneCenterPoint);
        QPointF itemAxis2Point2 = mapFromScene(sceneAxis2Point2);
        QLineF itemLine(itemCenterPoint, itemAxis2Point2);
        if(painter) drawRubberLine(itemLine, painter, VIEW_COLOR_CROSSHAIR);
        updatePath();
    }
    else if(rubberMode == OBJ_RUBBER_GRIP)
    {
        //TODO: updateRubber() gripping for EllipseObject
    }
}

void EllipseObject::vulcanize()
{
    qDebug("EllipseObject vulcanize()");
    updateRubber();

    setObjectRubberMode(OBJ_RUBBER_OFF);
}

// Returns the closest snap point to the mouse point
QPointF EllipseObject::mouseSnapPoint(const QPointF& mousePoint)
{
    QPointF center  = objectCenter();
    QPointF quad0   = objectQuadrant0();
    QPointF quad90  = objectQuadrant90();
    QPointF quad180 = objectQuadrant180();
    QPointF quad270 = objectQuadrant270();

    qreal cntrDist = QLineF(mousePoint, center).length();
    qreal q0Dist   = QLineF(mousePoint, quad0).length();
    qreal q90Dist  = QLineF(mousePoint, quad90).length();
    qreal q180Dist = QLineF(mousePoint, quad180).length();
    qreal q270Dist = QLineF(mousePoint, quad270).length();

    qreal minDist = qMin(qMin(qMin(q0Dist, q90Dist), qMin(q180Dist, q270Dist)), cntrDist);

    if     (minDist == cntrDist) return center;
    else if(minDist == q0Dist)   return quad0;
    else if(minDist == q90Dist)  return quad90;
    else if(minDist == q180Dist) return quad180;
    else if(minDist == q270Dist) return quad270;

    return scenePos();
}

QList<QPointF> EllipseObject::allGripPoints()
{
    QList<QPointF> gripPoints;
    gripPoints << objectCenter() << objectQuadrant0() << objectQuadrant90() << objectQuadrant180() << objectQuadrant270();
    return gripPoints;
}

void EllipseObject::gripEdit(const QPointF& before, const QPointF& after)
{
    //TODO: gripEdit() for EllipseObject
}

QPainterPath EllipseObject::objectSavePath() const
{
    QPainterPath path;
    QRectF r = rect();
    path.arcMoveTo(r, 0);
    path.arcTo(r, 0, 360);

    qreal s = scale();
    QTransform trans;
    trans.rotate(rotation());
    trans.scale(s,s);
    return trans.map(path);
}

/* kate: bom off; indent-mode cstyle; indent-width 4; replace-trailing-space-save on; */
