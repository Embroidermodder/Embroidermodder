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

/**
 * \file object-arc.cpp
 */

#include "embroidermodder.h"

std::vector<EmbReal> snowflake_x;
std::vector<EmbReal> snowflake_y;
std::vector<EmbReal> dolphin_x;
std::vector<EmbReal> dolphin_y;

/**
 * @brief mouse_snap_point
 * @param points
 * @return
 */
QPointF
closest_point(QPointF position, std::vector<QPointF> points)
{
    QPointF result = points[0];
    EmbReal closest = QLineF(position, points[0]).length();
    for (int i=1; i<(int)(points.size()); i++) {
        if (QLineF(position, points[i]).length() < closest) {
            closest = QLineF(position, points[i]).length();
            result = points[i];
        }
    }
    return result;
}

/**
 * @brief ArcObject::ArcObject
 * @param obj
 * @param parent
 */
ArcObject::ArcObject(ArcObject* obj, QGraphicsItem* parent) : BaseObject(OBJ_TYPE_ARC, parent)
{
    debug_message("ArcObject Constructor()");
    if (obj) {
        EmbArc arc;
        arc.start = to_EmbVector(obj->objectStartPoint());
        arc.mid = to_EmbVector(obj->objectMidPoint());
        arc.end = to_EmbVector(obj->objectEndPoint());
        init(arc, obj->objectColorRGB(), Qt::SolidLine); //TODO: getCurrentLineType
        setRotation(obj->rotation());
    }
}

/**
 * @brief ArcObject::~ArcObject
 */
ArcObject::~ArcObject()
{
    debug_message("ArcObject Destructor()");
}

/**
 * @brief ArcObject::init
 * @param arc
 * @param rgb
 * @param lineType
 */
void ArcObject::init(EmbArc arc, QRgb rgb, Qt::PenStyle lineType)
{
    setData(OBJ_TYPE, OBJ_TYPE_ARC);
    setData(OBJ_NAME, "Arc");

    //WARNING: DO NOT enable QGraphicsItem::ItemIsMovable. If it is enabled,
    //WARNING: and the item is double clicked, the scene will erratically move the item while zooming.
    //WARNING: All movement has to be handled explicitly by us, not by the scene.
    setFlag(QGraphicsItem::ItemIsSelectable, true);

    calculateArcData(arc);

    setObjectColor(rgb);
    setObjectLineType(lineType);
    setObjectLineWeight("0.35"); //TODO: pass in proper lineweight
    setPen(objPen);
}

/**
 * @brief ArcObject::calculateArcData
 * @param arc
 */
void ArcObject::calculateArcData(EmbArc arc)
{
    EmbVector center;
    getArcCenter(arc, &center);

    arcStartPoint = to_QPointF(arc.start - center);
    arcMidPoint = to_QPointF(arc.mid - center);
    arcEndPoint = to_QPointF(arc.end - center);

    setPos(center.x, center.y);

    EmbReal radius = embVector_distance(center, arc.mid);
    updateArcRect(radius);
    updatePath();
    setRotation(0);
    setScale(1);
}

/**
 * @brief ArcObject::updateArcRect
 * @param radius
 */
void
ArcObject::updateArcRect(EmbReal radius)
{
    QRectF arcRect;
    arcRect.setWidth(radius*2.0);
    arcRect.setHeight(radius*2.0);
    arcRect.moveCenter(QPointF(0,0));
    setRect(arcRect);
}

/**
 * @brief ArcObject::setObjectRadius
 * @param radius
 */
void
ArcObject::setObjectRadius(EmbReal radius)
{
    EmbReal rad = std::max(radius, 0.0000001f);

    QPointF center = scenePos();
    QLineF startLine = QLineF(center, objectStartPoint());
    QLineF midLine   = QLineF(center, objectMidPoint());
    QLineF endLine   = QLineF(center, objectEndPoint());
    startLine.setLength(rad);
    midLine.setLength(rad);
    endLine.setLength(rad);
    arcStartPoint = startLine.p2();
    arcMidPoint = midLine.p2();
    arcEndPoint = endLine.p2();
    EmbArc arc;
    arc.start = to_EmbVector(arcStartPoint);
    arc.mid = to_EmbVector(arcMidPoint);
    arc.end = to_EmbVector(arcEndPoint);

    calculateArcData(arc);
}

/**
 * @brief ArcObject::setObjectStartAngle
 * @param angle
 */
void
ArcObject::setObjectStartAngle(EmbReal angle)
{
    //TODO: ArcObject setObjectStartAngle
}

/**
 * @brief ArcObject::setObjectEndAngle
 * @param angle
 */
void
ArcObject::setObjectEndAngle(EmbReal angle)
{
    //TODO: ArcObject setObjectEndAngle
}

/**
 * @brief ArcObject::setObjectStartPoint
 * @param point
 */
void
ArcObject::setObjectStartPoint(EmbVector point)
{
    EmbArc arc;
    arc.start = point;
    arc.mid = to_EmbVector(objectMidPoint());
    arc.mid = to_EmbVector(objectEndPoint());
    calculateArcData(arc);
}

/**
 * @brief ArcObject::setObjectMidPoint
 * @param point
 */
void
ArcObject::setObjectMidPoint(EmbVector point)
{
    EmbArc arc;
    arc.start = to_EmbVector(objectStartPoint());
    arc.mid = point;
    arc.end = to_EmbVector(objectEndPoint());
    calculateArcData(arc);
}

/**
 * @brief ArcObject::setObjectEndPoint
 * @param point
 */
void
ArcObject::setObjectEndPoint(EmbVector point)
{
    EmbArc arc;
    arc.start = to_EmbVector(objectStartPoint());
    arc.mid = to_EmbVector(objectMidPoint());
    arc.end = point;
    calculateArcData(arc);
}

/**
 * @brief ArcObject::objectStartAngle
 * @return
 */
EmbReal ArcObject::objectStartAngle()
{
    EmbReal angle = QLineF(scenePos(), objectStartPoint()).angle();
    return std::fmod(angle, 360.0);
}

/**
 * @brief ArcObject::objectEndAngle
 * @return
 */
EmbReal ArcObject::objectEndAngle()
{
    EmbReal angle = QLineF(scenePos(), objectEndPoint()).angle();
    return std::fmod(angle, 360.0);
}

/**
 * @brief 
 * @return 
 */
EmbVector
rotate_vector(EmbVector v, EmbReal alpha)
{
    EmbVector rotv;
    EmbVector u = embVector_unit(alpha);
    rotv.x = v.x*u.x - v.y*u.y;
    rotv.y = v.x*u.y + v.y*u.x;
    return rotv;    
}

/**
 * @brief ArcObject::objectStartPoint
 * @return
 */
QPointF ArcObject::objectStartPoint()
{
    EmbReal rot = radians(rotation());
    EmbVector v = to_EmbVector(arcStartPoint) * scale();
    EmbVector rotv = rotate_vector(v, rot);

    return scenePos() + to_QPointF(rotv);
}

/**
 * @brief ArcObject::objectMidPoint
 * @return
 */
QPointF ArcObject::objectMidPoint()
{
    EmbReal rot = radians(rotation());
    EmbVector v = to_EmbVector(arcMidPoint) * scale();
    EmbVector rotv = rotate_vector(v, rot);

    return scenePos() + to_QPointF(rotv);
}

/**
 * @brief ArcObject::objectEndPoint
 * @return
 */
QPointF ArcObject::objectEndPoint()
{
    EmbReal rot = radians(rotation());
    EmbVector v = to_EmbVector(arcEndPoint) * scale();
    EmbVector rotv = rotate_vector(v, rot);

    return scenePos() + to_QPointF(rotv);
}

/**
 * @brief ArcObject::objectArea
 * @return
 */
EmbReal ArcObject::objectArea()
{
    //Area of a circular segment
    EmbReal r = objectRadius();
    EmbReal theta = radians(objectIncludedAngle());
    return ((r*r)/2)*(theta - std::sin(theta));
}

/**
 * @brief ArcObject::objectArcLength
 * @return
 */
EmbReal ArcObject::objectArcLength()
{
    return radians(objectIncludedAngle())*objectRadius();
}

/**
 * @brief ArcObject::objectChord
 * @return
 */
EmbReal ArcObject::objectChord()
{
    return QLineF(
        objectStartPoint().x(),
        objectStartPoint().y(),
        objectEndPoint().x(),
        objectEndPoint().y()).length();
}

/**
 * @brief ArcObject::objectIncludedAngle
 * @return
 */
EmbReal ArcObject::objectIncludedAngle()
{
    EmbReal chord = objectChord();
    EmbReal rad = objectRadius();
    if (chord <= 0 || rad <= 0) return 0; //Prevents division by zero and non-existant circles

    //NOTE: Due to floating point rounding errors, we need to clamp the quotient so it is in the range [-1, 1]
    //      If the quotient is out of that range, then the result of asin() will be NaN.
    EmbReal quotient = chord/(2.0*rad);
    if (quotient > 1.0) quotient = 1.0;
    if (quotient < 0.0) quotient = 0.0; //NOTE: 0 rather than -1 since we are enforcing a positive chord and radius
    return degrees(2.0*asin(quotient)); //Properties of a Circle - Get the Included Angle - Reference: ASD9
}

/**
 * @brief ArcObject::objectClockwise
 * @return
 */
bool ArcObject::objectClockwise()
{
    //NOTE: Y values are inverted here on purpose
    EmbArc arc;
    arc.start.x = objectStartPoint().x();
    arc.start.y = -objectStartPoint().y();
    arc.mid.x = objectMidPoint().x();
    arc.mid.y = -objectMidPoint().y();
    arc.end.x = objectEndPoint().x();
    arc.end.y = -objectEndPoint().y();
    if (embArc_clockwise(arc)) {
        return true;
    }
    return false;
}

/**
 * @brief ArcObject::updatePath
 */
void ArcObject::updatePath()
{
    EmbReal startAngle = (objectStartAngle() + rotation());
    EmbReal spanAngle = objectIncludedAngle();

    if (objectClockwise()) {
        spanAngle = -spanAngle;
    }

    QPainterPath path;
    path.arcMoveTo(rect(), startAngle);
    path.arcTo(rect(), startAngle, spanAngle);
    //NOTE: Reverse the path so that the inside area isn't considered part of the arc
    path.arcTo(rect(), startAngle+spanAngle, -spanAngle);
    setObjectPath(path);
}

/**
 * @brief ArcObject::paint
 * @param painter
 * @param option
 */
void ArcObject::paint(QPainter* painter, const QStyleOptionGraphicsItem* option, QWidget* /*widget*/)
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
        paintPen = lwtPen;
    }
    painter->setPen(paintPen);

    EmbReal startAngle = (objectStartAngle() + rotation())*16;
    EmbReal spanAngle = objectIncludedAngle()*16;

    if (objectClockwise())
        spanAngle = -spanAngle;

    EmbReal rad = objectRadius();
    QRectF paintRect(-rad, -rad, rad*2.0, rad*2.0);
    painter->drawArc(paintRect, startAngle, spanAngle);
}

/**
 * @brief ArcObject::updateRubber
 * @param painter
 *
 * \todo Arc Rubber Modes
 *
 * \todo updateRubber() gripping for ArcObject
 */
void
ArcObject::updateRubber(QPainter* painter)
{

}

/**
 * @brief ArcObject::vulcanize
 */
void
ArcObject::vulcanize()
{
    debug_message("ArcObject vulcanize()");
    updateRubber();

    objRubberMode = "OBJ_RUBBER_OFF";
}

/**
 * @brief ArcObject::mouseSnapPoint
 * @param mousePoint
 * @return the closest snap point to the mouse point.
 */
QPointF
ArcObject::mouseSnapPoint(const QPointF& mousePoint)
{
    std::vector<QPointF> points = {
        objectCenter(), objectStartPoint(), objectMidPoint(), objectEndPoint()
    };

    return closest_point(mousePoint, points);
}

/**
 * @brief ArcObject::allGripPoints
 * @return
 */
std::vector<QPointF>
ArcObject::allGripPoints()
{
    std::vector<QPointF> gripPoints = {
        objectCenter(),
        objectStartPoint(),
        objectMidPoint(),
        objectEndPoint()
    };
    return gripPoints;
}

/**
 * @brief ArcObject::gripEdit
 * @param before
 * @param after
 *
 * \todo gripEdit() for ArcObject
 */
void
ArcObject::gripEdit(const QPointF& before, const QPointF& after)
{
}

/**
 * @brief BaseObject::~BaseObject
 */
BaseObject::~BaseObject()
{
    debug_message("BaseObject Destructor()");
}

/**
 * @brief BaseObject::run_command
 * @param command
 */
void
BaseObject::run_command(String command)
{

}

/**
 * @brief BaseObject::setObjectColor
 * @param color
 */
void
BaseObject::setObjectColor(const QColor& color)
{
    objPen.setColor(color);
    lwtPen.setColor(color);
}

/**
 * @brief BaseObject::setObjectColorRGB
 * @param rgb
 */
void
BaseObject::setObjectColorRGB(QRgb rgb)
{
    objPen.setColor(QColor(rgb));
    lwtPen.setColor(QColor(rgb));
}

/**
 * @brief BaseObject::setObjectLineType
 * @param lineType
 */
void
BaseObject::setObjectLineType(Qt::PenStyle lineType)
{
    objPen.setStyle(lineType);
    lwtPen.setStyle(lineType);
}

/**
 * @brief BaseObject::setObjectLineWeight
 * @param lineWeight
 */
void
BaseObject::setObjectLineWeight(String lineWeight)
{
    objPen.setWidthF(0); //NOTE: The objPen will always be cosmetic

    if (lineWeight == "OBJ_LWT_BYLAYER") {
        lwtPen.setWidthF(0.35f); //TODO: getLayerLineWeight
    }
    else if (lineWeight == "OBJ_LWT_BYBLOCK") {
        lwtPen.setWidthF(0.35f); //TODO: getBlockLineWeight
    }
    else {
        lwtPen.setWidthF(std::stof(lineWeight));
    }
    /*
    else {
        QMessageBox::warning(0, QObject::tr("Error - Negative Lineweight"),
                                QObject::tr("Lineweight: %1")
                                .arg(QString().setNum(lineWeight)));
        debug_message("Lineweight cannot be negative! Inverting sign.");
        lwtPen.setWidthF(-lineWeight);
    }
    */
}

/**
 * @brief BaseObject::objectRubberPoint
 * @param key
 * @return
 */
QPointF
BaseObject::objectRubberPoint(QString  key)
{
    if (objRubberPoints.contains(key))
        return objRubberPoints.value(key);

    QGraphicsScene* gscene = scene();
    if (gscene)
        return scene()->property("SCENE_QSNAP_POINT").toPointF();
    return QPointF();
}

/**
 * @brief BaseObject::objectRubberText
 * @param key
 * @return
 */
QString
BaseObject::objectRubberText(QString  key)
{
    if (objRubberTexts.contains(key))
        return objRubberTexts.value(key);
    return QString();
}

/**
 * If gripped, force this object to be drawn even if it is offscreen.
 */
QRectF
BaseObject::boundingRect()
{
    if (objRubberMode == "OBJ_RUBBER_GRIP") {
        return scene()->sceneRect();
    }
    return path().boundingRect();
}

/**
 * @brief BaseObject::drawRubberLine
 * @param rubLine
 * @param painter
 * @param colorFromScene
 */
void
BaseObject::drawRubberLine(const QLineF& rubLine, QPainter* painter, const char* colorFromScene)
{
    if (painter) {
        QGraphicsScene* objScene = scene();
        if (!objScene) return;
        QPen colorPen = objPen;
        colorPen.setColor(QColor(objScene->property(colorFromScene).toUInt()));
        painter->setPen(colorPen);
        painter->drawLine(rubLine);
        painter->setPen(objPen);
    }
}

/**
 * @brief BaseObject::realRender
 * @param painter
 * @param renderPath
 */
void
BaseObject::realRender(QPainter* painter, const QPainterPath& renderPath)
{
    QColor color1 = objectColor();       //lighter color
    QColor color2  = color1.darker(150); //darker color

    //If we have a dark color, lighten it
    int darkness = color1.lightness();
    int threshold = 32; //TODO: This number may need adjusted or maybe just add it to settings.
    if (darkness < threshold) {
        color2 = color1;
        if (!darkness) {
            color1 = QColor(threshold, threshold, threshold);
            // lighter() does not affect pure black
        }
        else {
            color1 = color2.lighter(100 + threshold);
        }
    }

    int count = renderPath.elementCount();
    for (int i = 0; i < count-1; ++i) {
        QPainterPath::Element elem = renderPath.elementAt(i);
        QPainterPath::Element next = renderPath.elementAt(i+1);

        if (next.isMoveTo()) continue;

        QPainterPath elemPath;
        elemPath.moveTo(elem.x, elem.y);
        elemPath.lineTo(next.x, next.y);

        QPen renderPen(QColor(0,0,0,0));
        renderPen.setWidthF(0);
        painter->setPen(renderPen);
        QPainterPathStroker stroker;
        stroker.setWidth(0.35);
        stroker.setCapStyle(Qt::RoundCap);
        stroker.setJoinStyle(Qt::RoundJoin);
        QPainterPath realPath = stroker.createStroke(elemPath);
        painter->drawPath(realPath);

        QLinearGradient grad(elemPath.pointAtPercent(0.5), elemPath.pointAtPercent(0.0));
        grad.setColorAt(0, color1);
        grad.setColorAt(1, color2);
        grad.setSpread(QGradient::ReflectSpread);

        painter->fillPath(realPath, QBrush(grad));
    }
}

/**
 * @brief CircleObject::CircleObject
 * @param centerX
 * @param centerY
 * @param radius
 * @param rgb
 * @param parent
 */
CircleObject::CircleObject(EmbReal centerX, EmbReal centerY, EmbReal radius, QRgb rgb, QGraphicsItem* parent) : BaseObject(OBJ_TYPE_CIRCLE, parent)
{
    debug_message("CircleObject Constructor()");
    init(centerX, centerY, radius, rgb, Qt::SolidLine); //TODO: getCurrentLineType
}

/**
 * @brief CircleObject::CircleObject
 * @param obj
 * @param parent
 */
CircleObject::CircleObject(CircleObject* obj, QGraphicsItem* parent) : BaseObject(OBJ_TYPE_CIRCLE, parent)
{
    debug_message("CircleObject Constructor()");
    if (obj) {
        init(obj->objectCenter().x(), obj->objectCenter().y(), obj->objectRadius(), obj->objectColorRGB(), Qt::SolidLine); //TODO: getCurrentLineType
        setRotation(obj->rotation());
    }
}

/**
 * @brief CircleObject::~CircleObject
 */
CircleObject::~CircleObject()
{
    debug_message("CircleObject Destructor()");
}

/**
 * @brief CircleObject::init
 * @param centerX
 * @param centerY
 * @param radius
 * @param rgb
 * @param lineType
 */
void
CircleObject::init(EmbReal centerX, EmbReal centerY, EmbReal radius, QRgb rgb, Qt::PenStyle lineType)
{
    setData(OBJ_TYPE, OBJ_TYPE_CIRCLE);
    setData(OBJ_NAME, "Circle");

    //WARNING: DO NOT enable QGraphicsItem::ItemIsMovable. If it is enabled,
    //WARNING: and the item is double clicked, the scene will erratically move the item while zooming.
    //WARNING: All movement has to be handled explicitly by us, not by the scene.
    setFlag(QGraphicsItem::ItemIsSelectable, true);

    setObjectRadius(radius);
    EmbVector center = {centerX, centerY};
    setObjectCenter(center);
    setObjectColor(rgb);
    setObjectLineType(lineType);
    setObjectLineWeight("0.35"); //TODO: pass in proper lineweight
    setPen(objPen);
    updatePath();
}

/**
 * @brief CircleObject::setObjectRadius
 * @param radius
 */
void
CircleObject::setObjectRadius(EmbReal radius)
{
    setObjectDiameter(radius*2.0);
}

/**
 * @brief CircleObject::setObjectDiameter
 * @param diameter
 */
void
CircleObject::setObjectDiameter(EmbReal diameter)
{
    QRectF circRect;
    circRect.setWidth(diameter);
    circRect.setHeight(diameter);
    circRect.moveCenter(QPointF(0,0));
    setRect(circRect);
    updatePath();
}

/**
 * @brief CircleObject::setObjectArea
 * @param area
 */
void
CircleObject::setObjectArea(EmbReal area)
{
    EmbReal radius = std::sqrt(area/emb_constant_pi);
    setObjectRadius(radius);
}

/**
 * @brief CircleObject::setObjectCircumference
 * @param circumference
 */
void
CircleObject::setObjectCircumference(EmbReal circumference)
{
    EmbReal diameter = circumference/emb_constant_pi;
    setObjectDiameter(diameter);
}

/**
 * @brief CircleObject::updatePath
 */
void
CircleObject::updatePath()
{
    QPainterPath path;
    QRectF r = rect();
    //Add the center point
    path.addRect(-0.00000001, -0.00000001, 0.00000002, 0.00000002);
    //Add the circle
    path.arcMoveTo(r, 0);
    path.arcTo(r, 0, 360);
    //NOTE: Reverse the path so that the inside area isn't considered part of the circle
    path.arcTo(r, 0, -360);
    setObjectPath(path);
}

/**
 * @brief CircleObject::paint
 * @param painter
 * @param option
 */
void
CircleObject::paint(QPainter* painter, const QStyleOptionGraphicsItem* option, QWidget* /*widget*/)
{
    QGraphicsScene* objScene = scene();
    if (!objScene) return;

    QPen paintPen = pen();
    painter->setPen(paintPen);
    updateRubber(painter);
    if (option->state & QStyle::State_Selected) { paintPen.setStyle(Qt::DashLine); }
    if (objScene->property("ENABLE_LWT").toBool()) { paintPen = lwtPen; }
    painter->setPen(paintPen);

    painter->drawEllipse(rect());
}

/**
 * @brief CircleObject::updateRubber
 * @param painter
 */
void
CircleObject::updateRubber(QPainter* painter)
{
    if (objRubberMode == "OBJ_RUBBER_CIRCLE_1P_RAD") {
        QPointF sceneCenterPoint = objectRubberPoint("CIRCLE_CENTER");
        QPointF sceneQSnapPoint = objectRubberPoint("CIRCLE_RADIUS");
        QPointF itemCenterPoint = mapFromScene(sceneCenterPoint);
        QPointF itemQSnapPoint = mapFromScene(sceneQSnapPoint);
        QLineF itemLine(itemCenterPoint, itemQSnapPoint);
        setObjectCenter(to_EmbVector(sceneCenterPoint));
        QLineF sceneLine(sceneCenterPoint, sceneQSnapPoint);
        EmbReal radius = sceneLine.length();
        setObjectRadius(radius);
        if (painter) drawRubberLine(itemLine, painter, "VIEW_COLOR_CROSSHAIR");
        updatePath();
    }
    else if (objRubberMode == "OBJ_RUBBER_CIRCLE_1P_DIA") {
        QPointF sceneCenterPoint = objectRubberPoint("CIRCLE_CENTER");
        QPointF sceneQSnapPoint = objectRubberPoint("CIRCLE_DIAMETER");
        QPointF itemCenterPoint = mapFromScene(sceneCenterPoint);
        QPointF itemQSnapPoint = mapFromScene(sceneQSnapPoint);
        QLineF itemLine(itemCenterPoint, itemQSnapPoint);
        setObjectCenter(to_EmbVector(sceneCenterPoint));
        QLineF sceneLine(sceneCenterPoint, sceneQSnapPoint);
        EmbReal diameter = sceneLine.length();
        setObjectDiameter(diameter);
        if (painter) drawRubberLine(itemLine, painter, "VIEW_COLOR_CROSSHAIR");
        updatePath();
    }
    else if (objRubberMode == "OBJ_RUBBER_CIRCLE_2P") {
        QPointF sceneTan1Point = objectRubberPoint("CIRCLE_TAN1");
        QPointF sceneQSnapPoint = objectRubberPoint("CIRCLE_TAN2");
        QLineF sceneLine(sceneTan1Point, sceneQSnapPoint);
        setObjectCenter(to_EmbVector(sceneLine.pointAt(0.5)));
        EmbReal diameter = sceneLine.length();
        setObjectDiameter(diameter);
        updatePath();
    }
    else if (objRubberMode == "OBJ_RUBBER_CIRCLE_3P") {
        QPointF sceneTan1Point = objectRubberPoint("CIRCLE_TAN1");
        QPointF sceneTan2Point = objectRubberPoint("CIRCLE_TAN2");
        QPointF sceneTan3Point = objectRubberPoint("CIRCLE_TAN3");

        EmbVector sceneCenter;
        EmbArc arc;
        arc.start.x = sceneTan1Point.x();
        arc.start.y = sceneTan1Point.y();
        arc.mid.x = sceneTan2Point.x();
        arc.mid.y = sceneTan2Point.y();
        arc.end.x = sceneTan3Point.x();
        arc.end.y = sceneTan3Point.y();
        getArcCenter(arc, &sceneCenter);
        QPointF sceneCenterPoint(sceneCenter.x, sceneCenter.y);
        QLineF sceneLine(sceneCenterPoint, sceneTan3Point);
        setObjectCenter(to_EmbVector(sceneCenterPoint));
        EmbReal radius = sceneLine.length();
        setObjectRadius(radius);
        updatePath();
    }
    else if (objRubberMode == "OBJ_RUBBER_GRIP") {
        if (painter) {
            QPointF gripPoint = objectRubberPoint("GRIP_POINT");
            if (gripPoint == objectCenter()) {
                painter->drawEllipse(rect().translated(mapFromScene(objectRubberPoint(QString()))-mapFromScene(gripPoint)));
            }
            else {
                EmbReal gripRadius = QLineF(objectCenter(), objectRubberPoint(QString())).length();
                painter->drawEllipse(QPointF(), gripRadius, gripRadius);
            }

            QLineF rubLine(mapFromScene(gripPoint), mapFromScene(objectRubberPoint(QString())));
            drawRubberLine(rubLine, painter, "VIEW_COLOR_CROSSHAIR");
        }
    }
}

/**
 * @brief CircleObject::vulcanize
 */
void
CircleObject::vulcanize()
{
    debug_message("CircleObject vulcanize()");
    updateRubber();

    setObjectRubberMode("OBJ_RUBBER_OFF");
}

/**
 * @brief CircleObject::mouseSnapPoint
 * @param mousePoint
 * @return the closest snap point to the mouse point.
 */
QPointF
CircleObject::mouseSnapPoint(const QPointF& mousePoint)
{
    return closest_point(mousePoint, allGripPoints());
}

/**
 * @brief CircleObject::allGripPoints
 * @return
 */
std::vector<QPointF>
CircleObject::allGripPoints()
{
    std::vector<QPointF> gripPoints = {
        objectCenter(),
        objectQuadrant0(),
        objectQuadrant90(),
        objectQuadrant180(),
        objectQuadrant270()
    };
    return gripPoints;
}

/**
 * @brief CircleObject::gripEdit
 * @param before
 * @param after
 */
void
CircleObject::gripEdit(const QPointF& before, const QPointF& after)
{
    if (before == objectCenter()) { QPointF delta = after-before; moveBy(delta.x(), delta.y()); }
    else { setObjectRadius(QLineF(objectCenter(), after).length()); }
}

/**
 * @brief CircleObject::objectSavePath
 * @return
 */
QPainterPath
CircleObject::objectSavePath()
{
    QPainterPath path;
    QRectF r = rect();
    path.arcMoveTo(r, 0);
    path.arcTo(r, 0, 360);

    EmbReal s = scale();
    QTransform trans;
    trans.rotate(rotation());
    trans.scale(s,s);
    return trans.map(path);
}

/**
 * @brief DimLeaderObject::DimLeaderObject
 * @param x1
 * @param y1
 * @param x2
 * @param y2
 * @param rgb
 * @param parent
 */
DimLeaderObject::DimLeaderObject(EmbReal x1, EmbReal y1, EmbReal x2, EmbReal y2, QRgb rgb, QGraphicsItem* parent) : BaseObject(OBJ_TYPE_DIMLEADER, parent)
{
    debug_message("DimLeaderObject Constructor()");
    init(x1, y1, x2, y2, rgb, Qt::SolidLine); //TODO: getCurrentLineType
}

/**
 * @brief DimLeaderObject::DimLeaderObject
 * @param obj
 * @param parent
 */
DimLeaderObject::DimLeaderObject(DimLeaderObject* obj, QGraphicsItem* parent) : BaseObject(OBJ_TYPE_DIMLEADER, parent)
{
    debug_message("DimLeaderObject Constructor()");
    if (obj) {
        init(obj->objectX1(), obj->objectY1(), obj->objectX2(), obj->objectY2(), obj->objectColorRGB(), Qt::SolidLine); //TODO: getCurrentLineType
    }
}

/**
 * @brief DimLeaderObject::~DimLeaderObject
 */
DimLeaderObject::~DimLeaderObject()
{
    debug_message("DimLeaderObject Destructor()");
}

/**
 * @brief DimLeaderObject::init
 * @param x1
 * @param y1
 * @param x2
 * @param y2
 * @param rgb
 * @param lineType
 */
void
DimLeaderObject::init(EmbReal x1, EmbReal y1, EmbReal x2, EmbReal y2, QRgb rgb, Qt::PenStyle lineType)
{
    setData(OBJ_TYPE, OBJ_TYPE_DIMLEADER);
    setData(OBJ_NAME, "Dimension Leader");

    //WARNING: DO NOT enable QGraphicsItem::ItemIsMovable. If it is enabled,
    //WARNING: and the item is double clicked, the scene will erratically move the item while zooming.
    //WARNING: All movement has to be handled explicitly by us, not by the scene.
    setFlag(QGraphicsItem::ItemIsSelectable, true);

    curved = false;
    filled = true;
    EmbVector endPt1, endPt2;
    endPt1.x = x1;
    endPt1.y = y1;
    endPt2.x = x2;
    endPt2.y = y2;
    setObjectEndPoint1(endPt1);
    setObjectEndPoint2(endPt2);
    setObjectColor(rgb);
    setObjectLineType(lineType);
    setObjectLineWeight("0.35"); //TODO: pass in proper lineweight
    setPen(objPen);
}

/**
 * @brief DimLeaderObject::setObjectEndPoint1
 * @param x1
 * @param y1
 */
void
DimLeaderObject::setObjectEndPoint1(EmbVector endPt1)
{
    EmbVector endPt2 = to_EmbVector(objectEndPoint2());
    EmbVector delta = endPt2 - endPt1;
    setRotation(0);
    setLine(0, 0, delta.x, delta.y);
    setPos(endPt1.x, endPt1.y);
    updateLeader();
}

/**
 * @brief DimLeaderObject::setObjectEndPoint2
 * @param x2
 * @param y2
 */
void
DimLeaderObject::setObjectEndPoint2(EmbVector endPt2)
{
    EmbVector endPt1 = to_EmbVector(scenePos());
    EmbVector delta = endPt2 - endPt1;
    setRotation(0);
    setLine(0, 0, delta.x, delta.y);
    setPos(endPt1.x, endPt1.y);
    updateLeader();
}

/**
 * @brief DimLeaderObject::objectEndPoint1
 * @return
 */
QPointF
DimLeaderObject::objectEndPoint1()
{
    return scenePos();
}

/**
 * @brief DimLeaderObject::objectEndPoint2
 * @return
 */
QPointF
DimLeaderObject::objectEndPoint2()
{
    EmbVector v;
    EmbReal rot = radians(rotation());
    v.x = objLine.x2()*scale();
    v.y = objLine.y2()*scale();
    QPointF rotEnd = to_QPointF(rotate_vector(v, rot));

    return scenePos() + rotEnd;
}

/**
 * @brief DimLeaderObject::objectMidPoint
 * @return
 */
QPointF
DimLeaderObject::objectMidPoint()
{
    QPointF mp = objLine.pointAt(0.5);
    EmbReal rot = radians(rotation());
    EmbVector m = to_EmbVector(mp) * scale();
    QPointF rotMid = to_QPointF(rotate_vector(m, rot));

    return scenePos() + rotMid;
}

/**
 * @brief DimLeaderObject::objectAngle
 * @return
 */
EmbReal
DimLeaderObject::objectAngle()
{
    EmbReal angle = objLine.angle() - rotation();
    return std::fmod(angle, 360.0);
}

/**
 * @brief DimLeaderObject::updateLeader
 */
void
DimLeaderObject::updateLeader()
{
    int arrowStyle = Closed; //TODO: Make this customizable
    EmbReal arrowStyleAngle = 15.0; //TODO: Make this customizable
    EmbReal arrowStyleLength = 1.0; //TODO: Make this customizable
    EmbReal lineStyleAngle = 45.0; //TODO: Make this customizable
    EmbReal lineStyleLength = 1.0; //TODO: Make this customizable

    QLineF lyne = objLine;
    EmbReal angle = lyne.angle();
    QPointF ap0 = lyne.p1();
    QPointF lp0 = lyne.p2();

    //Arrow
    QLineF lynePerp(lyne.pointAt(arrowStyleLength/lyne.length()) ,lp0);
    lynePerp.setAngle(angle + 90);
    QLineF lyne1(ap0, lp0);
    QLineF lyne2(ap0, lp0);
    lyne1.setAngle(angle + arrowStyleAngle);
    lyne2.setAngle(angle - arrowStyleAngle);
    QPointF ap1;
    QPointF ap2;
    lynePerp.intersects(lyne1, &ap1);
    lynePerp.intersects(lyne2, &ap2);

    //Math Diagram
    //                 .(ap1)                     .(lp1)
    //                /|                         /|
    //               / |                        / |
    //              /  |                       /  |
    //             /   |                      /   |
    //            /    |                     /    |
    //           /     |                    /     |
    //          /      |                   /      |
    //         /       |                  /       |
    //        /+(aSA)  |                 /+(lSA)  |
    // (ap0)./__(aSL)__|__________(lp0)./__(lSL)__|
    //       \ -(aSA)  |                \ -(lSA)  |
    //        \        |                 \        |
    //         \       |                  \       |
    //          \      |                   \      |
    //           \     |                    \     |
    //            \    |                     \    |
    //             \   |                      \   |
    //              \  |                       \  |
    //               \ |                        \ |
    //                \|                         \|
    //                 .(ap2)                     .(lp2)

    if (arrowStyle == Open) {
        arrowStylePath = QPainterPath();
        arrowStylePath.moveTo(ap1);
        arrowStylePath.lineTo(ap0);
        arrowStylePath.lineTo(ap2);
        arrowStylePath.lineTo(ap0);
        arrowStylePath.lineTo(ap1);
    }
    else if (arrowStyle == Closed) {
        arrowStylePath = QPainterPath();
        arrowStylePath.moveTo(ap1);
        arrowStylePath.lineTo(ap0);
        arrowStylePath.lineTo(ap2);
        arrowStylePath.lineTo(ap1);
    }
    else if (arrowStyle == Dot) {
        arrowStylePath = QPainterPath();
        arrowStylePath.addEllipse(ap0, arrowStyleLength, arrowStyleLength);
    }
    else if (arrowStyle == Box) {
        arrowStylePath = QPainterPath();
        EmbReal side = QLineF(ap1, ap2).length();
        QRectF ar0(0, 0, side, side);
        ar0.moveCenter(ap0);
        arrowStylePath.addRect(ar0);
    }
    else if (arrowStyle == Tick) {
    }

    lineStylePath = QPainterPath();
    lineStylePath.moveTo(ap0);
    lineStylePath.lineTo(lp0);
}

/**
 * @brief DimLeaderObject::paint
 * @param painter
 * @param option
 */
void
DimLeaderObject::paint(QPainter* painter, const QStyleOptionGraphicsItem* option, QWidget* /*widget*/)
{
    QGraphicsScene* objScene = scene();
    if (!objScene) return;

    QPen paintPen = pen();
    painter->setPen(paintPen);
    updateRubber(painter);
    if (option->state & QStyle::State_Selected) { paintPen.setStyle(Qt::DashLine); }
    if (objScene->property("ENABLE_LWT").toBool()) { paintPen = lwtPen; }
    painter->setPen(paintPen);

    painter->drawPath(lineStylePath);
    painter->drawPath(arrowStylePath);

    if (filled)
        painter->fillPath(arrowStylePath, objectColor());
}
/**
 * @brief DimLeaderObject::updateRubber
 * @param painter
 */
void
DimLeaderObject::updateRubber(QPainter* painter)
{
    if (objRubberMode == "OBJ_RUBBER_DIMLEADER_LINE") {
        QPointF sceneStartPoint = objectRubberPoint("DIMLEADER_LINE_START");
        QPointF sceneQSnapPoint = objectRubberPoint("DIMLEADER_LINE_END");

        setObjectEndPoint1(to_EmbVector(sceneStartPoint));
        setObjectEndPoint2(to_EmbVector(sceneQSnapPoint));
    }
    else if (objRubberMode == "OBJ_RUBBER_GRIP") {
        if (painter) {
            QPointF gripPoint = objectRubberPoint("GRIP_POINT");
            if (gripPoint == objectEndPoint1()) {
                painter->drawLine(objLine.p2(), mapFromScene(objectRubberPoint(QString())));
            }
            else if (gripPoint == objectEndPoint2()) {
                painter->drawLine(objLine.p1(), mapFromScene(objectRubberPoint(QString())));
            }
            else if (gripPoint == objectMidPoint()) {
                painter->drawLine(objLine.translated(mapFromScene(objectRubberPoint(QString()))-mapFromScene(gripPoint)));
            }
        }
    }
}

/**
 * @brief DimLeaderObject::vulcanize
 */
void
DimLeaderObject::vulcanize()
{
    debug_message("DimLeaderObject vulcanize()");
    updateRubber();

    setObjectRubberMode("OBJ_RUBBER_OFF");
}

/**
 * Returns the closest snap point to the mouse point
 *
 * \todo generic closest point from list to point x.
 */
QPointF DimLeaderObject::mouseSnapPoint(const QPointF& mousePoint)
{
    return closest_point(mousePoint, allGripPoints());
}

/**
 * @brief DimLeaderObject::allGripPoints
 * @return
 */
std::vector<QPointF>
DimLeaderObject::allGripPoints()
{
    std::vector<QPointF> gripPoints = {objectEndPoint1(), objectEndPoint2()};
    if (curved) {
        gripPoints.push_back(objectMidPoint());
    }
    return gripPoints;
}

/**
 * @brief DimLeaderObject::gripEdit
 * @param before
 * @param after
 */
void
DimLeaderObject::gripEdit(const QPointF& before, const QPointF& after)
{
    if (before == objectEndPoint1()) {
        setObjectEndPoint1(to_EmbVector(after));
    }
    else if (before == objectEndPoint2()) {
        setObjectEndPoint2(to_EmbVector(after));
    }
    else if (before == objectMidPoint()) {
        QPointF delta = after-before;
        moveBy(delta.x(), delta.y());
    }
}

/**
 * \brief .
 */
EllipseObject::EllipseObject(EmbReal centerX, EmbReal centerY, EmbReal width, EmbReal height, QRgb rgb, QGraphicsItem* parent) : BaseObject(OBJ_TYPE_ELLIPSE, parent)
{
    debug_message("EllipseObject Constructor()");
    init(centerX, centerY, width, height, rgb, Qt::SolidLine); //TODO: getCurrentLineType
}

/**
 * \brief .
 */
EllipseObject::EllipseObject(EllipseObject* obj, QGraphicsItem* parent) : BaseObject(OBJ_TYPE_ELLIPSE, parent)
{
    debug_message("EllipseObject Constructor()");
    if (obj) {
        init(
            obj->objectCenter().x(),
            obj->objectCenter().y(),
            obj->objectWidth(),
            obj->objectHeight(),
            obj->objectColorRGB(),
            Qt::SolidLine); //TODO: getCurrentLineType
        setRotation(obj->rotation());
    }
}

/**
 * \brief .
 */
EllipseObject::~EllipseObject()
{
    debug_message("EllipseObject Destructor()");
}

/**
 * \brief .
 */
void
EllipseObject::init(EmbReal centerX, EmbReal centerY, EmbReal width, EmbReal height, QRgb rgb, Qt::PenStyle lineType)
{
    setData(OBJ_TYPE, OBJ_TYPE_ELLIPSE);
    setData(OBJ_NAME, "Ellipse");

    /**
     * \warning DO NOT enable QGraphicsItem::ItemIsMovable. If it is enabled,
     * and the item is double clicked, the scene will erratically move the item while zooming.
     * All movement has to be handled explicitly by us, not by the scene.
     */
    setFlag(QGraphicsItem::ItemIsSelectable, true);

    setObjectSize(width, height);
    EmbVector center = {centerX, centerY};
    setObjectCenter(center);
    setObjectColor(rgb);
    setObjectLineType(lineType);
    setObjectLineWeight("0.35"); //TODO: pass in proper lineweight
    setPen(objPen);
    updatePath();
}

/**
 * \brief .
 */
void
EllipseObject::setObjectSize(EmbReal width, EmbReal height)
{
    QRectF elRect = rect();
    elRect.setWidth(width);
    elRect.setHeight(height);
    elRect.moveCenter(QPointF(0,0));
    setRect(elRect);
}

/**
 * \brief .
 */
void
EllipseObject::setObjectRadiusMajor(EmbReal radius)
{
    setObjectDiameterMajor(radius*2.0);
}

/**
 * \brief .
 */
void
EllipseObject::setObjectRadiusMinor(EmbReal radius)
{
    setObjectDiameterMinor(radius*2.0);
}

/**
 * \brief .
 */
void
EllipseObject::setObjectDiameterMajor(EmbReal diameter)
{
    QRectF elRect = rect();
    if (elRect.width() > elRect.height())
        elRect.setWidth(diameter);
    else
        elRect.setHeight(diameter);
    elRect.moveCenter(QPointF(0,0));
    setRect(elRect);
}

/**
 * \brief .
 */
void
EllipseObject::setObjectDiameterMinor(EmbReal diameter)
{
    QRectF elRect = rect();
    if (elRect.width() < elRect.height())
        elRect.setWidth(diameter);
    else
        elRect.setHeight(diameter);
    elRect.moveCenter(QPointF(0,0));
    setRect(elRect);
}

/**
 * \brief .
 */
QPointF
EllipseObject::objectQuadrant0()
{
    EmbReal halfW = objectWidth()/2.0;
    EmbReal rot = radians(rotation());
    EmbVector v = embVector_unit(rot) * halfW;
    return objectCenter() + to_QPointF(v);
}

/**
 * \brief .
 */
QPointF
EllipseObject::objectQuadrant90()
{
    EmbReal halfH = objectHeight()/2.0;
    EmbReal rot = radians(rotation()+90.0);
    EmbVector v = embVector_unit(rot) * halfH;
    return objectCenter() + to_QPointF(v);
}

/**
 * \brief .
 */
QPointF
EllipseObject::objectQuadrant180()
{
    EmbReal halfW = objectWidth()/2.0;
    EmbReal rot = radians(rotation()+180.0);
    EmbReal x = halfW*std::cos(rot);
    EmbReal y = halfW*std::sin(rot);
    return objectCenter() + QPointF(x,y);
}

/**
 * \brief .
 */
QPointF
EllipseObject::objectQuadrant270()
{
    EmbReal halfH = objectHeight()/2.0;
    EmbReal rot = radians(rotation()+270.0);
    EmbReal x = halfH*std::cos(rot);
    EmbReal y = halfH*std::sin(rot);
    return objectCenter() + QPointF(x,y);
}

/**
 * \brief .
 */
void
EllipseObject::updatePath()
{
    QPainterPath path;
    QRectF r = rect();
    path.arcMoveTo(r, 0);
    path.arcTo(r, 0, 360);
    //NOTE: Reverse the path so that the inside area isn't considered part of the ellipse
    path.arcTo(r, 0, -360);
    setObjectPath(path);
}

/**
 * \brief .
 */
void
EllipseObject::paint(QPainter* painter, const QStyleOptionGraphicsItem* option, QWidget* /*widget*/)
{
    QGraphicsScene* objScene = scene();
    if (!objScene) return;

    QPen paintPen = pen();
    painter->setPen(paintPen);
    updateRubber(painter);
    if (option->state & QStyle::State_Selected) { paintPen.setStyle(Qt::DashLine); }
    if (objScene->property("ENABLE_LWT").toBool()) { paintPen = lwtPen; }
    painter->setPen(paintPen);

    painter->drawEllipse(rect());
}

/**
 * \brief .
 */
void
EllipseObject::updateRubber(QPainter* painter)
{
    String rubberMode = objRubberMode;
    if (rubberMode == "OBJ_RUBBER_ELLIPSE_LINE") {
        QPointF sceneLinePoint1 = objectRubberPoint("ELLIPSE_LINE_POINT1");
        QPointF sceneLinePoint2 = objectRubberPoint("ELLIPSE_LINE_POINT2");
        QPointF itemLinePoint1  = mapFromScene(sceneLinePoint1);
        QPointF itemLinePoint2  = mapFromScene(sceneLinePoint2);
        QLineF itemLine(itemLinePoint1, itemLinePoint2);
        if (painter) drawRubberLine(itemLine, painter, "VIEW_COLOR_CROSSHAIR");
        updatePath();
    }
    else if (rubberMode == "OBJ_RUBBER_ELLIPSE_MAJORDIAMETER_MINORRADIUS") {
        QPointF sceneAxis1Point1 = objectRubberPoint("ELLIPSE_AXIS1_POINT1");
        QPointF sceneAxis1Point2 = objectRubberPoint("ELLIPSE_AXIS1_POINT2");
        QPointF sceneCenterPoint = objectRubberPoint("ELLIPSE_CENTER");
        QPointF sceneAxis2Point2 = objectRubberPoint("ELLIPSE_AXIS2_POINT2");
        EmbReal ellipseWidth = objectRubberPoint("ELLIPSE_WIDTH").x();
        EmbReal ellipseRot = objectRubberPoint("ELLIPSE_ROT").x();

        //TODO: incorporate perpendicularDistance() into libcgeometry
        EmbReal px = sceneAxis2Point2.x();
        EmbReal py = sceneAxis2Point2.y();
        EmbReal x1 = sceneAxis1Point1.x();
        EmbReal y1 = sceneAxis1Point1.y();
        QLineF line(sceneAxis1Point1, sceneAxis1Point2);
        QLineF norm = line.normalVector();
        EmbReal dx = px-x1;
        EmbReal dy = py-y1;
        norm.translate(dx, dy);
        QPointF iPoint;
        norm.intersects(line, &iPoint);
        EmbReal ellipseHeight = QLineF(px, py, iPoint.x(), iPoint.y()).length()*2.0;

        setObjectCenter(to_EmbVector(sceneCenterPoint));
        setObjectSize(ellipseWidth, ellipseHeight);
        setRotation(-ellipseRot);

        QPointF itemCenterPoint = mapFromScene(sceneCenterPoint);
        QPointF itemAxis2Point2 = mapFromScene(sceneAxis2Point2);
        QLineF itemLine(itemCenterPoint, itemAxis2Point2);
        if (painter) drawRubberLine(itemLine, painter, "VIEW_COLOR_CROSSHAIR");
        updatePath();
    }
    else if (rubberMode == "OBJ_RUBBER_ELLIPSE_MAJORRADIUS_MINORRADIUS") {
        QPointF sceneAxis1Point2 = objectRubberPoint("ELLIPSE_AXIS1_POINT2");
        QPointF sceneCenterPoint = objectRubberPoint("ELLIPSE_CENTER");
        QPointF sceneAxis2Point2 = objectRubberPoint("ELLIPSE_AXIS2_POINT2");
        EmbReal ellipseWidth = objectRubberPoint("ELLIPSE_WIDTH").x();
        EmbReal ellipseRot = objectRubberPoint("ELLIPSE_ROT").x();

        //TODO: incorporate perpendicularDistance() into libcgeometry
        EmbReal px = sceneAxis2Point2.x();
        EmbReal py = sceneAxis2Point2.y();
        EmbReal x1 = sceneCenterPoint.x();
        EmbReal y1 = sceneCenterPoint.y();
        QLineF line(sceneCenterPoint, sceneAxis1Point2);
        QLineF norm = line.normalVector();
        EmbReal dx = px-x1;
        EmbReal dy = py-y1;
        norm.translate(dx, dy);
        QPointF iPoint;
        norm.intersects(line, &iPoint);
        EmbReal ellipseHeight = QLineF(px, py, iPoint.x(), iPoint.y()).length()*2.0;

        setObjectCenter(to_EmbVector(sceneCenterPoint));
        setObjectSize(ellipseWidth, ellipseHeight);
        setRotation(-ellipseRot);

        QPointF itemCenterPoint = mapFromScene(sceneCenterPoint);
        QPointF itemAxis2Point2 = mapFromScene(sceneAxis2Point2);
        QLineF itemLine(itemCenterPoint, itemAxis2Point2);
        if (painter) drawRubberLine(itemLine, painter, "VIEW_COLOR_CROSSHAIR");
        updatePath();
    }
    else if (rubberMode == "OBJ_RUBBER_GRIP") {
        //TODO: updateRubber() gripping for EllipseObject
    }
}

/**
 * \brief .
 */
void
EllipseObject::vulcanize()
{
    debug_message("EllipseObject vulcanize()");
    updateRubber();

    setObjectRubberMode("OBJ_RUBBER_OFF");
}

/**
 * \brief Returns the closest snap point to the mouse point.
 */
QPointF
EllipseObject::mouseSnapPoint(const QPointF& mousePoint)
{
    return closest_point(mousePoint, allGripPoints());
}

/**
 * \brief .
 */
std::vector<QPointF>
EllipseObject::allGripPoints()
{
    std::vector<QPointF> points = {
        objectCenter(),
        objectQuadrant0(),
        objectQuadrant90(),
        objectQuadrant180(),
        objectQuadrant270()
    };
    return points;
}

/**
 * \brief .
 * \todo gripEdit() for EllipseObject
 */
void
EllipseObject::gripEdit(const QPointF& before, const QPointF& after)
{
}

/**
 * \brief .
 */
QPainterPath EllipseObject::objectSavePath()
{
    QPainterPath path;
    QRectF r = rect();
    path.arcMoveTo(r, 0);
    path.arcTo(r, 0, 360);

    EmbReal s = scale();
    QTransform trans;
    trans.rotate(rotation());
    trans.scale(s,s);
    return trans.map(path);
}

/**
 * \brief .
 */
ImageObject::ImageObject(EmbReal x, EmbReal y, EmbReal w, EmbReal h, QRgb rgb, QGraphicsItem* parent) : BaseObject(OBJ_TYPE_IMAGE, parent)
{
    debug_message("ImageObject Constructor()");
    init(x, y, w, h, rgb, Qt::SolidLine); //TODO: getCurrentLineType
}

/**
 * \brief .
 */
ImageObject::ImageObject(ImageObject* obj, QGraphicsItem* parent) : BaseObject(OBJ_TYPE_IMAGE, parent)
{
    debug_message("ImageObject Constructor()");
    if (obj) {
        QPointF ptl = obj->objectTopLeft();
        init(ptl.x(), ptl.y(), obj->objectWidth(), obj->objectHeight(), obj->objectColorRGB(), Qt::SolidLine); //TODO: getCurrentLineType
        setRotation(obj->rotation());
    }
}

/**
 * \brief .
 */
ImageObject::~ImageObject()
{
    debug_message("ImageObject Destructor()");
}

/**
 * \brief .
 */
void ImageObject::init(EmbReal x, EmbReal y, EmbReal w, EmbReal h, QRgb rgb, Qt::PenStyle lineType)
{
    setData(OBJ_TYPE, OBJ_TYPE_IMAGE);
    setData(OBJ_NAME, "Image");

    //WARNING: DO NOT enable QGraphicsItem::ItemIsMovable. If it is enabled,
    //WARNING: and the item is double clicked, the scene will erratically move the item while zooming.
    //WARNING: All movement has to be handled explicitly by us, not by the scene.
    setFlag(QGraphicsItem::ItemIsSelectable, true);

    setObjectRect(x, y, w, h);
    setObjectColor(rgb);
    setObjectLineType(lineType);
    setObjectLineWeight("0.35"); //TODO: pass in proper lineweight
    setPen(objPen);
}

/**
 * \brief .
 */
void
ImageObject::setObjectRect(EmbReal x, EmbReal y, EmbReal w, EmbReal h)
{
    setPos(x, y);
    setRect(0, 0, w, h);
    updatePath();
}

/**
 * \brief .
 */
QPointF
ImageObject::objectTopLeft()
{
    EmbReal rot = radians(rotation());
    QPointF tl = rect().topLeft();
    EmbVector ptl;
    ptl.x = tl.x()*scale();
    ptl.y = tl.y()*scale();
    EmbVector ptlRot = rotate_vector(ptl, rot);

    return (scenePos() + QPointF(ptlRot.x, ptlRot.y));
}

/**
 * \brief .
 */
QPointF
ImageObject::objectTopRight()
{
    EmbReal rot = radians(rotation());
    QPointF tr = rect().topRight();
    EmbVector ptr;
    ptr.x = tr.x()*scale();
    ptr.y = tr.y()*scale();
    EmbVector ptrRot = rotate_vector(ptr, rot);

    return (scenePos() + QPointF(ptrRot.x, ptrRot.y));
}

/**
 * \brief .
 */
QPointF
ImageObject::objectBottomLeft()
{
    EmbReal rot = radians(rotation());
    QPointF bl = rect().bottomLeft();
    EmbVector pbl = to_EmbVector(bl) * scale();
    EmbVector pblRot = rotate_vector(pbl, rot);

    return scenePos() + to_QPointF(pblRot);
}

/**
 * \brief .
 */
QPointF
ImageObject::objectBottomRight()
{
    EmbReal rot = radians(rotation());
    QPointF br = rect().bottomRight();
    EmbVector pbr = to_EmbVector(br) * scale();
    EmbVector pbrRot = rotate_vector(pbr, rot);

    return scenePos() + to_QPointF(pbrRot);
}

/**
 * \brief .
 */
void
ImageObject::updatePath()
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

/**
 * \brief .
 */
void
ImageObject::paint(QPainter* painter, const QStyleOptionGraphicsItem* option, QWidget* /*widget*/)
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
        paintPen = lwtPen;
    }
    painter->setPen(paintPen);

    painter->drawRect(rect());
}

/**
 * \brief .
 */
void
ImageObject::updateRubber(QPainter* painter)
{
    if (objRubberMode == "OBJ_RUBBER_IMAGE") {
        QPointF sceneStartPoint = objectRubberPoint("IMAGE_START");
        QPointF sceneEndPoint = objectRubberPoint("IMAGE_END");
        EmbReal x = sceneStartPoint.x();
        EmbReal y = sceneStartPoint.y();
        EmbReal w = sceneEndPoint.x() - sceneStartPoint.x();
        EmbReal h = sceneEndPoint.y() - sceneStartPoint.y();
        setObjectRect(x,y,w,h);
        updatePath();
    }
    else if (objRubberMode == "OBJ_RUBBER_GRIP") {
        //TODO: updateRubber() gripping for ImageObject
    }
}

/**
 * \brief .
 */
void
ImageObject::vulcanize()
{
    debug_message("ImageObject vulcanize()");
    updateRubber();

    objRubberMode = "OBJ_RUBBER_OFF";
}

// Returns the closest snap point to the mouse point
/**
 * \brief .
 */
QPointF
ImageObject::mouseSnapPoint(const QPointF& mousePoint)
{
    return closest_point(mousePoint, allGripPoints());
}

/**
 * \brief .
 */
std::vector<QPointF>
ImageObject::allGripPoints()
{
    std::vector<QPointF> points = {
        objectTopLeft(),     //Top Left Corner QSnap
        objectTopRight(),    //Top Right Corner QSnap
        objectBottomLeft(),  //Bottom Left Corner QSnap
        objectBottomRight()  //Bottom Right Corner QSnap
    };
    return points;
}

/**
 * \brief .
 */
void
ImageObject::gripEdit(const QPointF& before, const QPointF& after)
{
    //TODO: gripEdit() for ImageObject
}

/**
 * @brief LineObject::LineObject
 * @param x1
 * @param y1
 * @param x2
 * @param y2
 * @param rgb
 * @param parent
 */
LineObject::LineObject(EmbLine line, QRgb rgb, QGraphicsItem* parent) : BaseObject(OBJ_TYPE_LINE, parent)
{
    debug_message("LineObject Constructor()");
    init(line, rgb, Qt::SolidLine); //TODO: getCurrentLineType
}

/**
 * @brief LineObject::LineObject
 * @param obj
 * @param parent
 */
LineObject::LineObject(LineObject* obj, QGraphicsItem* parent) : BaseObject(OBJ_TYPE_LINE, parent)
{
    debug_message("LineObject Constructor()");
    if (obj) {
        EmbLine line;
        line.start = to_EmbVector(obj->objectEndPoint1());
        line.end = to_EmbVector(obj->objectEndPoint2());
        init(line, obj->objectColorRGB(), Qt::SolidLine); //TODO: getCurrentLineType
    }
}

/**
 * @brief LineObject::~LineObject
 */
LineObject::~LineObject()
{
    debug_message("LineObject Destructor()");
}

/**
 * @brief LineObject::init
 * @param x1
 * @param y1
 * @param x2
 * @param y2
 * @param rgb
 * @param lineType
 */
void
LineObject::init(EmbLine line, QRgb rgb, Qt::PenStyle lineType)
{
    setData(OBJ_TYPE, OBJ_TYPE_LINE);
    setData(OBJ_NAME, "Line");

    //WARNING: DO NOT enable QGraphicsItem::ItemIsMovable. If it is enabled,
    //WARNING: and the item is double clicked, the scene will erratically move the item while zooming.
    //WARNING: All movement has to be handled explicitly by us, not by the scene.
    setFlag(QGraphicsItem::ItemIsSelectable, true);

    setObjectEndPoint1(line.start);
    setObjectEndPoint2(line.end);
    setObjectColor(rgb);
    setObjectLineType(lineType);
    setObjectLineWeight("0.35"); //TODO: pass in proper lineweight
    setPen(objPen);
}

/**
 * @brief LineObject::setObjectEndPoint1
 * @param x1
 * @param y1
 */
void
LineObject::setObjectEndPoint1(EmbVector endPt1)
{
    EmbVector endPt2 = to_EmbVector(objectEndPoint2());
    EmbVector delta = endPt2 - endPt1;
    setRotation(0);
    setScale(1);
    setLine(0, 0, delta.x, delta.y);
    setPos(endPt1.x, endPt1.y);
}

/**
 * @brief LineObject::setObjectEndPoint2
 * @param x2
 * @param y2
 */
void
LineObject::setObjectEndPoint2(EmbVector endPt2)
{
    EmbVector endPt1 = to_EmbVector(scenePos());
    EmbVector delta = endPt2 - endPt1;
    setRotation(0);
    setScale(1);
    setLine(0, 0, delta.x, delta.y);
    setPos(endPt1.x, endPt1.y);
}

/**
 * @brief LineObject::objectEndPoint2
 * @return
 */
QPointF
LineObject::objectEndPoint2()
{
    QLineF lyne = objLine;
    EmbReal rot = radians(rotation());
    EmbVector v;
    v.x = lyne.x2()*scale();
    v.y = lyne.y2()*scale();
    EmbVector rotEnd = rotate_vector(v, rot);

    return scenePos() + to_QPointF(rotEnd);
}

/**
 * @brief LineObject::objectMidPoint
 * @return
 */
QPointF
LineObject::objectMidPoint()
{
    QLineF lyne = objLine;
    QPointF mp = lyne.pointAt(0.5);
    EmbReal rot = radians(rotation());
    EmbVector m = to_EmbVector(mp) * scale();
    EmbVector rotMid = rotate_vector(m, rot);

    return scenePos() + to_QPointF(rotMid);
}

/**
 * @brief LineObject::objectAngle
 * @return
 */
EmbReal
LineObject::objectAngle()
{
    EmbReal angle = objLine.angle() - rotation();
    return std::fmod(angle, 360.0);
}

/**
 * @brief LineObject::paint
 * @param painter
 * @param option
 */
void
LineObject::paint(QPainter* painter, const QStyleOptionGraphicsItem* option, QWidget* /*widget*/)
{
    QGraphicsScene* objScene = scene();
    if (!objScene) return;

    QPen paintPen = pen();
    painter->setPen(paintPen);
    updateRubber(painter);
    if (option->state & QStyle::State_Selected) { paintPen.setStyle(Qt::DashLine); }
    if (objScene->property("ENABLE_LWT").toBool()) { paintPen = lwtPen; }
    painter->setPen(paintPen);

    if (objRubberMode != "OBJ_RUBBER_LINE") {
        painter->drawLine(objLine);
    }

    if (objScene->property("ENABLE_LWT").toBool()
        && objScene->property("ENABLE_REAL").toBool()) {
        realRender(painter, path());
    }
}

/**
 * @brief LineObject::updateRubber
 * @param painter
 */
void
LineObject::updateRubber(QPainter* painter)
{
    String rubberMode = objRubberMode;
    if (rubberMode == "OBJ_RUBBER_LINE") {
        QPointF sceneStartPoint = objectRubberPoint("LINE_START");
        QPointF sceneQSnapPoint = objectRubberPoint("LINE_END");

        setObjectEndPoint1(to_EmbVector(sceneStartPoint));
        setObjectEndPoint2(to_EmbVector(sceneQSnapPoint));

        drawRubberLine(objLine, painter, "VIEW_COLOR_CROSSHAIR");
    }
    else if (rubberMode == "OBJ_RUBBER_GRIP") {
        if (painter) {
            QPointF gripPoint = objectRubberPoint("GRIP_POINT");
            if (gripPoint == objectEndPoint1()) {
                painter->drawLine(objLine.p2(), mapFromScene(objectRubberPoint(QString())));
            }
            else if (gripPoint == objectEndPoint2()) {
                painter->drawLine(objLine.p1(), mapFromScene(objectRubberPoint(QString())));
            }
            else if (gripPoint == objectMidPoint())
                painter->drawLine(objLine.translated(mapFromScene(objectRubberPoint(QString()))-mapFromScene(gripPoint)));

            QLineF rubLine(mapFromScene(gripPoint), mapFromScene(objectRubberPoint(QString())));
            drawRubberLine(rubLine, painter, "VIEW_COLOR_CROSSHAIR");
        }
    }
}

/**
 * @brief LineObject::vulcanize
 */
void
LineObject::vulcanize()
{
    debug_message("LineObject vulcanize()");
    updateRubber();

    objRubberMode = "OBJ_RUBBER_OFF";
}

/**
 * @brief LineObject::mouseSnapPoint
 * @param mousePoint
 * @return The closest snap point to the mouse point.
 */
QPointF LineObject::mouseSnapPoint(const QPointF& mousePoint)
{
    std::vector<QPointF> points = {
        objectEndPoint1(),
        objectEndPoint2(),
        objectMidPoint()
    };

    return closest_point(mousePoint, points);
}

/**
 * @brief LineObject::allGripPoints
 * @return
 */
std::vector<QPointF>
LineObject::allGripPoints()
{
    std::vector<QPointF> gripPoints = {
        objectEndPoint1(),
        objectEndPoint2(),
        objectMidPoint()
    };
    return gripPoints;
}

/**
 * @brief LineObject::gripEdit
 * @param before
 * @param after
 */
void
LineObject::gripEdit(const QPointF& before, const QPointF& after)
{
    if (before == objectEndPoint1()) {
        setObjectEndPoint1(to_EmbVector(after));
    }
    else if (before == objectEndPoint2()) {
        setObjectEndPoint2(to_EmbVector(after));
    }
    else if (before == objectMidPoint()) {
        QPointF delta = after-before;
        moveBy(delta.x(), delta.y());
    }
}

/**
 * @brief LineObject::objectSavePath
 * @return
 */
QPainterPath
LineObject::objectSavePath()
{
    QPainterPath path;
    /* \todo discards qualifiers error. */
    // QPointF delta = objectDelta();
    // path.lineTo(delta.x(), delta.y());
    return path;
}

/**
 * @brief PathObject::PathObject
 * @param x
 * @param y
 * @param p
 * @param rgb
 * @param parent
 */
PathObject::PathObject(EmbReal x, EmbReal y, const QPainterPath p, QRgb rgb, QGraphicsItem* parent) : BaseObject(OBJ_TYPE_PATH, parent)
{
    debug_message("PathObject Constructor()");
    init(x, y, p, rgb, Qt::SolidLine); //TODO: getCurrentLineType
}

/**
 * @brief PathObject::PathObject
 * @param obj
 * @param parent
 */
PathObject::PathObject(PathObject* obj, QGraphicsItem* parent) : BaseObject(OBJ_TYPE_PATH, parent)
{
    debug_message("PathObject Constructor()");
    if (obj) {
        init(obj->objectPos().x(), obj->objectPos().y(), obj->objectCopyPath(), obj->objectColorRGB(), Qt::SolidLine); //TODO: getCurrentLineType
        setRotation(obj->rotation());
        setScale(obj->scale());
    }
}

/**
 * @brief PathObject::~PathObject
 */
PathObject::~PathObject()
{
    debug_message("PathObject Destructor()");
}

/**
 * @brief PathObject::init
 * @param x
 * @param y
 * @param p
 * @param rgb
 * @param lineType
 */
void
PathObject::init(EmbReal x, EmbReal y, const QPainterPath& p, QRgb rgb, Qt::PenStyle lineType)
{
    setData(OBJ_TYPE, OBJ_TYPE_PATH);
    setData(OBJ_NAME, "Path");

    //WARNING: DO NOT enable QGraphicsItem::ItemIsMovable. If it is enabled,
    //WARNING: and the item is double clicked, the scene will erratically move the item while zooming.
    //WARNING: All movement has to be handled explicitly by us, not by the scene.
    setFlag(QGraphicsItem::ItemIsSelectable, true);

    updatePath(p);
    setObjectPos(x,y);
    setObjectColor(rgb);
    setObjectLineType(lineType);
    setObjectLineWeight("0.35"); //TODO: pass in proper lineweight
    setPen(objPen);
}

/**
 * @brief PathObject::updatePath
 * @param p
 */
void
PathObject::updatePath(const QPainterPath& p)
{
    normalPath = p;
    QPainterPath reversePath = normalPath.toReversed();
    reversePath.connectPath(normalPath);
    setObjectPath(reversePath);
}

/**
 * @brief PathObject::paint
 * @param painter
 * @param option
 */
void
PathObject::paint(QPainter* painter, const QStyleOptionGraphicsItem* option, QWidget* /*widget*/)
{
    QGraphicsScene* objScene = scene();
    if (!objScene)
        return;

    QPen paintPen = pen();
    painter->setPen(paintPen);
    updateRubber(painter);
    if (option->state & QStyle::State_Selected) {
        paintPen.setStyle(Qt::DashLine);
    }
    if (objScene->property("ENABLE_LWT").toBool()) {
        paintPen = lwtPen;
    }
    painter->setPen(paintPen);

    painter->drawPath(path());
}

/**
 * @brief PathObject::updateRubber
 * @param painter
 */
void
PathObject::updateRubber(QPainter* painter)
{
    //TODO: Path Rubber Modes

    //TODO: updateRubber() gripping for PathObject

}

/**
 * @brief PathObject::vulcanize
 */
void
PathObject::vulcanize()
{
    debug_message("PathObject vulcanize()");
    updateRubber();

    setObjectRubberMode("OBJ_RUBBER_OFF");

    if (!normalPath.elementCount())
        QMessageBox::critical(0, QObject::tr("Empty Path Error"), QObject::tr("The path added contains no points. The command that created this object has flawed logic."));
}

/**
 * @brief PathObject::mouseSnapPoint
 * @param mousePoint
 * @return The closest snap point to the mouse point.
 */
QPointF
PathObject::mouseSnapPoint(const QPointF& mousePoint)
{
    return closest_point(mousePoint, allGripPoints());
}

/**
 * @brief PathObject::allGripPoints
 * @return
 * @todo loop thru all path Elements and return their points
 */
std::vector<QPointF>
PathObject::allGripPoints()
{
    std::vector<QPointF> gripPoints = {scenePos()};
    return gripPoints;
}

/**
 * @brief PathObject::gripEdit
 * @param before
 * @param after
 */
void
PathObject::gripEdit(const QPointF& before, const QPointF& after)
{
    //TODO: gripEdit() for PathObject
}

/**
 * @brief PathObject::objectCopyPath
 * @return
 */
QPainterPath
PathObject::objectCopyPath()
{
    return normalPath;
}

/**
 * @brief PathObject::objectSavePath
 * @return
 */
QPainterPath
PathObject::objectSavePath()
{
    EmbReal s = scale();
    QTransform trans;
    trans.rotate(rotation());
    trans.scale(s,s);
    return trans.map(normalPath);
}

/**
 * @brief PointObject::PointObject
 * @param x
 * @param y
 * @param rgb
 * @param parent
 */
PointObject::PointObject(EmbReal x, EmbReal y, QRgb rgb, QGraphicsItem* parent) : BaseObject(OBJ_TYPE_POINT, parent)
{
    debug_message("PointObject Constructor()");
    init(x, y, rgb, Qt::SolidLine); //TODO: getCurrentLineType
}

/**
 * @brief PointObject::PointObject
 * @param obj
 * @param parent
 */
PointObject::PointObject(PointObject* obj, QGraphicsItem* parent) : BaseObject(OBJ_TYPE_POINT, parent)
{
    debug_message("PointObject Constructor()");
    if (obj) {
        init(obj->objectPos().x(), obj->objectPos().y(), obj->objectColorRGB(), Qt::SolidLine); //TODO: getCurrentLineType
        setRotation(obj->rotation());
    }
}

/**
 * @brief PointObject::~PointObject
 */
PointObject::~PointObject()
{
    debug_message("PointObject Destructor()");
}

/**
 * @brief PointObject::init
 * @param x
 * @param y
 * @param rgb
 * @param lineType
 */
void
PointObject::init(EmbReal x, EmbReal y, QRgb rgb, Qt::PenStyle lineType)
{
    setData(OBJ_TYPE, OBJ_TYPE_POINT);
    setData(OBJ_NAME, "Point");

    //WARNING: DO NOT enable QGraphicsItem::ItemIsMovable. If it is enabled,
    //WARNING: and the item is double clicked, the scene will erratically move the item while zooming.
    //WARNING: All movement has to be handled explicitly by us, not by the scene.
    setFlag(QGraphicsItem::ItemIsSelectable, true);

    setRect(-0.00000001, -0.00000001, 0.00000002, 0.00000002);
    setObjectPos(x,y);
    setObjectColor(rgb);
    setObjectLineType(lineType);
    setObjectLineWeight("0.35"); //TODO: pass in proper lineweight
    setPen(objPen);
}

/**
 * @brief PointObject::paint
 * @param painter
 * @param option
 */
void
PointObject::paint(QPainter* painter, const QStyleOptionGraphicsItem* option, QWidget* /*widget*/)
{
    QGraphicsScene* objScene = scene();
    if (!objScene)
        return;

    QPen paintPen = pen();
    painter->setPen(paintPen);
    updateRubber(painter);
    if (option->state & QStyle::State_Selected) { paintPen.setStyle(Qt::DashLine); }
    if (objScene->property("ENABLE_LWT").toBool()) { paintPen = lwtPen; }
    painter->setPen(paintPen);

    painter->drawPoint(0,0);
}

/**
 * @brief PointObject::updateRubber
 * @param painter
 */
void
PointObject::updateRubber(QPainter* painter)
{
    if (objRubberMode == "OBJ_RUBBER_GRIP") {
        if (painter) {
            QPointF gripPoint = objectRubberPoint("GRIP_POINT");
            if (gripPoint == scenePos()) {
                QLineF rubLine(mapFromScene(gripPoint), mapFromScene(objectRubberPoint(QString())));
                drawRubberLine(rubLine, painter, "VIEW_COLOR_CROSSHAIR");
            }
        }
    }
}

/**
 * @brief PointObject::vulcanize
 */
void
PointObject::vulcanize()
{
    debug_message("PointObject vulcanize()");
    updateRubber();

    objRubberMode = "OBJ_RUBBER_OFF";
}

/**
 * @brief PointObject::mouseSnapPoint
 * @param mousePoint
 * @return The closest snap point to the mouse point
 */
QPointF
PointObject::mouseSnapPoint(const QPointF& mousePoint)
{
    return scenePos();
}

/**
 * @brief PointObject::allGripPoints
 * @return
 */
std::vector<QPointF>
PointObject::allGripPoints()
{
    std::vector<QPointF> gripPoints = {scenePos()};
    return gripPoints;
}

/**
 * @brief PointObject::gripEdit
 * @param before
 * @param after
 */
void
PointObject::gripEdit(const QPointF& before, const QPointF& after)
{
    if (before == scenePos()) {
        QPointF delta = after-before;
        moveBy(delta.x(), delta.y());
    }
}

/**
 * @brief PointObject::objectSavePath
 * @return
 */
QPainterPath
PointObject::objectSavePath()
{
    QPainterPath path;
    path.addRect(-0.00000001, -0.00000001, 0.00000002, 0.00000002);
    return path;
}

/**
 * @brief PolygonObject::PolygonObject
 * @param x
 * @param y
 * @param p
 * @param rgb
 * @param parent
 */
PolygonObject::PolygonObject(EmbReal x, EmbReal y, const QPainterPath& p, QRgb rgb, QGraphicsItem* parent) : BaseObject(OBJ_TYPE_POLYGON, parent)
{
    debug_message("PolygonObject Constructor()");
    init(x, y, p, rgb, Qt::SolidLine); //TODO: getCurrentLineType
}

/**
 * @brief PolygonObject::PolygonObject
 * @param obj
 * @param parent
 */
PolygonObject::PolygonObject(PolygonObject* obj, QGraphicsItem* parent) : BaseObject(OBJ_TYPE_POLYGON, parent)
{
    debug_message("PolygonObject Constructor()");
    if (obj) {
        init(obj->objectPos().x(), obj->objectPos().y(), obj->objectCopyPath(), obj->objectColorRGB(), Qt::SolidLine); //TODO: getCurrentLineType
        setRotation(obj->rotation());
        setScale(obj->scale());
    }
}

/**
 * @brief PolygonObject::~PolygonObject
 */
PolygonObject::~PolygonObject()
{
    debug_message("PolygonObject Destructor()");
}

/**
 * @brief PolygonObject::init
 * @param x
 * @param y
 * @param p
 * @param rgb
 * @param lineType
 */
void
PolygonObject::init(EmbReal x, EmbReal y, const QPainterPath& p, QRgb rgb, Qt::PenStyle lineType)
{
    setData(OBJ_TYPE, OBJ_TYPE_POLYGON);
    setData(OBJ_NAME, "Polygon");

    //WARNING: DO NOT enable QGraphicsItem::ItemIsMovable. If it is enabled,
    //WARNING: and the item is double clicked, the scene will erratically move the item while zooming.
    //WARNING: All movement has to be handled explicitly by us, not by the scene.
    setFlag(QGraphicsItem::ItemIsSelectable, true);

    gripIndex = -1;
    updatePath(p);
    setObjectPos(x,y);
    setObjectColor(rgb);
    setObjectLineType(lineType);
    setObjectLineWeight("0.35"); //TODO: pass in proper lineweight
    setPen(objPen);
}

/**
 * @brief PolygonObject::updatePath
 * @param p
 */
void
PolygonObject::updatePath(const QPainterPath& p)
{
    normalPath = p;
    QPainterPath closedPath = normalPath;
    closedPath.closeSubpath();
    QPainterPath reversePath = closedPath.toReversed();
    reversePath.connectPath(closedPath);
    setObjectPath(reversePath);
}

/**
 * @brief PolygonObject::paint
 * @param painter
 * @param option
 */
void
PolygonObject::paint(QPainter* painter, const QStyleOptionGraphicsItem* option, QWidget* /*widget*/)
{
    QGraphicsScene* objScene = scene();
    if (!objScene) return;

    QPen paintPen = pen();
    painter->setPen(paintPen);
    updateRubber(painter);
    if (option->state & QStyle::State_Selected) { paintPen.setStyle(Qt::DashLine); }
    if (objScene->property("ENABLE_LWT").toBool()) { paintPen = lwtPen; }
    painter->setPen(paintPen);

    if (normalPath.elementCount()) {
        painter->drawPath(normalPath);
        QPainterPath::Element zero = normalPath.elementAt(0);
        QPainterPath::Element last = normalPath.elementAt(normalPath.elementCount()-1);
        painter->drawLine(QPointF(zero.x, zero.y), QPointF(last.x, last.y));
    }
}

/**
 * @brief PolygonObject::updateRubber
 * @param painter
 */
void
PolygonObject::updateRubber(QPainter* painter)
{
    String rubberMode = objRubberMode;
    if (rubberMode == "OBJ_RUBBER_POLYGON") {
        setObjectPos(objectRubberPoint("POLYGON_POINT_0"));

        bool ok = false;
        QString numStr = objectRubberText("POLYGON_NUM_POINTS");
        if (numStr.isNull()) return;
        int num = numStr.toInt(&ok);
        if (!ok) return;

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
    else if (rubberMode == "OBJ_RUBBER_POLYGON_INSCRIBE") {
        setObjectPos(objectRubberPoint("POLYGON_CENTER"));

        quint16 numSides = objectRubberPoint("POLYGON_NUM_SIDES").x();

        QPointF inscribePoint = mapFromScene(objectRubberPoint("POLYGON_INSCRIBE_POINT"));
        QLineF inscribeLine = QLineF(QPointF(0,0), inscribePoint);
        EmbReal inscribeAngle = inscribeLine.angle();
        EmbReal inscribeInc = 360.0/numSides;

        if (painter) drawRubberLine(inscribeLine, painter, "VIEW_COLOR_CROSSHAIR");

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
    else if (rubberMode == "OBJ_RUBBER_POLYGON_CIRCUMSCRIBE") {
        setObjectPos(objectRubberPoint("POLYGON_CENTER"));

        quint16 numSides = objectRubberPoint("POLYGON_NUM_SIDES").x();

        QPointF circumscribePoint = mapFromScene(objectRubberPoint("POLYGON_CIRCUMSCRIBE_POINT"));
        QLineF circumscribeLine = QLineF(QPointF(0,0), circumscribePoint);
        EmbReal circumscribeAngle = circumscribeLine.angle();
        EmbReal circumscribeInc = 360.0/numSides;

        if (painter) drawRubberLine(circumscribeLine, painter, "VIEW_COLOR_CROSSHAIR");

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
    else if (rubberMode == "OBJ_RUBBER_GRIP") {
        if (painter) {
            int elemCount = normalPath.elementCount();
            QPointF gripPoint = objectRubberPoint("GRIP_POINT");
            if (gripIndex == -1) gripIndex = findIndex(gripPoint);
            if (gripIndex == -1) return;

            int m = 0;
            int n = 0;

            if (!gripIndex) { m = elemCount-1; n = 1; }
            else if (gripIndex == elemCount-1) { m = elemCount-2; n = 0; }
            else { m = gripIndex-1; n = gripIndex+1; }
            QPainterPath::Element em = normalPath.elementAt(m);
            QPainterPath::Element en = normalPath.elementAt(n);
            QPointF emPoint = QPointF(em.x, em.y);
            QPointF enPoint = QPointF(en.x, en.y);
            painter->drawLine(emPoint, mapFromScene(objectRubberPoint(QString())));
            painter->drawLine(enPoint, mapFromScene(objectRubberPoint(QString())));

            QLineF rubLine(mapFromScene(gripPoint), mapFromScene(objectRubberPoint(QString())));
            drawRubberLine(rubLine, painter, "VIEW_COLOR_CROSSHAIR");
        }
    }
}

/**
 * @brief PolygonObject::vulcanize
 */
void
PolygonObject::vulcanize()
{
    debug_message("PolygonObject vulcanize()");
    updateRubber();

    setObjectRubberMode("OBJ_RUBBER_OFF");

    if (!normalPath.elementCount())
        QMessageBox::critical(0, QObject::tr("Empty Polygon Error"), QObject::tr("The polygon added contains no points. The command that created this object has flawed logic."));
}

/**
 * @brief PolygonObject::mouseSnapPoint
 * @param mousePoint
 * @return The closest snap point to the mouse point
 */
QPointF
PolygonObject::mouseSnapPoint(const QPointF& mousePoint)
{
    QPainterPath::Element element = normalPath.elementAt(0);
    QPointF closestPoint = mapToScene(QPointF(element.x, element.y));
    EmbReal closestDist = QLineF(mousePoint, closestPoint).length();
    int elemCount = normalPath.elementCount();
    for (int i = 0; i < elemCount; ++i) {
        element = normalPath.elementAt(i);
        QPointF elemPoint = mapToScene(element.x, element.y);
        EmbReal elemDist = QLineF(mousePoint, elemPoint).length();
        if (elemDist < closestDist) {
            closestPoint = elemPoint;
            closestDist = elemDist;
        }
    }
    return closestPoint;
}

/**
 * @brief PolygonObject::allGripPoints
 * @return
 */
std::vector<QPointF>
PolygonObject::allGripPoints()
{
    std::vector<QPointF> gripPoints;
    QPainterPath::Element element;
    for (int i = 0; i < normalPath.elementCount(); ++i) {
        element = normalPath.elementAt(i);
        gripPoints.push_back(mapToScene(element.x, element.y));
    }
    return gripPoints;
}

/**
 * @brief PolygonObject::findIndex
 * @param point
 * @return
 */
int
PolygonObject::findIndex(const QPointF& point)
{
    int i = 0;
    int elemCount = normalPath.elementCount();
    //NOTE: Points here are in item coordinates
    QPointF itemPoint = mapFromScene(point);
    for (i = 0; i < elemCount; i++)
 {
        QPainterPath::Element e = normalPath.elementAt(i);
        QPointF elemPoint = QPointF(e.x, e.y);
        if (itemPoint == elemPoint) return i;
    }
    return -1;
}

/**
 * @brief PolygonObject::gripEdit
 * @param before
 * @param after
 */
void
PolygonObject::gripEdit(const QPointF& before, const QPointF& after)
{
    gripIndex = findIndex(before);
    if (gripIndex == -1)
        return;
    QPointF a = mapFromScene(after);
    normalPath.setElementPositionAt(gripIndex, a.x(), a.y());
    updatePath(normalPath);
    gripIndex = -1;
}

/**
 * @brief PolygonObject::objectCopyPath
 * @return
 */
QPainterPath
PolygonObject::objectCopyPath()
{
    return normalPath;
}

/**
 * @brief PolygonObject::objectSavePath
 * @return
 */
QPainterPath
PolygonObject::objectSavePath()
{
    QPainterPath closedPath = normalPath;
    closedPath.closeSubpath();
    EmbReal s = scale();
    QTransform trans;
    trans.rotate(rotation());
    trans.scale(s,s);
    return trans.map(closedPath);
}

/**
 * @brief PolylineObject::PolylineObject
 * @param x
 * @param y
 * @param p
 * @param rgb
 * @param parent
 */
PolylineObject::PolylineObject(EmbReal x, EmbReal y, const QPainterPath& p, QRgb rgb, QGraphicsItem* parent) : BaseObject(OBJ_TYPE_POLYLINE, parent)
{
    debug_message("PolylineObject Constructor()");
    init(x, y, p, rgb, Qt::SolidLine); //TODO: getCurrentLineType
}

/**
 * @brief PolylineObject::PolylineObject
 * @param obj
 * @param parent
 */
PolylineObject::PolylineObject(PolylineObject* obj, QGraphicsItem* parent) : BaseObject(OBJ_TYPE_POLYLINE, parent)
{
    debug_message("PolylineObject Constructor()");
    if (obj) {
        init(obj->objectX(), obj->objectY(), obj->objectCopyPath(), obj->objectColorRGB(), Qt::SolidLine); //TODO: getCurrentLineType
        setRotation(obj->rotation());
        setScale(obj->scale());
    }
}

/**
 * @brief PolylineObject::~PolylineObject
 */
PolylineObject::~PolylineObject()
{
    debug_message("PolylineObject Destructor()");
}

/**
 * @brief PolylineObject::init
 * @param x
 * @param y
 * @param p
 * @param rgb
 * @param lineType
 */
void
PolylineObject::init(EmbReal x, EmbReal y, const QPainterPath& p, QRgb rgb, Qt::PenStyle lineType)
{
    setData(OBJ_TYPE, OBJ_TYPE_POLYLINE);
    setData(OBJ_NAME, "Polyline");

    //WARNING: DO NOT enable QGraphicsItem::ItemIsMovable. If it is enabled,
    //WARNING: and the item is double clicked, the scene will erratically move the item while zooming.
    //WARNING: All movement has to be handled explicitly by us, not by the scene.
    setFlag(QGraphicsItem::ItemIsSelectable, true);

    gripIndex = -1;
    updatePath(p);
    setPos(x,y);
    setObjectColor(rgb);
    setObjectLineType(lineType);
    setObjectLineWeight("0.35"); //TODO: pass in proper lineweight
    setPen(objPen);
}

/**
 * @brief PolylineObject::updatePath
 * @param p
 */
void
PolylineObject::updatePath(const QPainterPath& p)
{
    normalPath = p;
    QPainterPath reversePath = normalPath.toReversed();
    reversePath.connectPath(normalPath);
    setObjectPath(reversePath);
}

/**
 * @brief PolylineObject::paint
 * @param painter
 * @param option
 */
void
PolylineObject::paint(QPainter* painter, const QStyleOptionGraphicsItem* option, QWidget* /*widget*/)
{
    QGraphicsScene* objScene = scene();
    if (!objScene) return;

    QPen paintPen = pen();
    painter->setPen(paintPen);
    updateRubber(painter);
    if (option->state & QStyle::State_Selected) { paintPen.setStyle(Qt::DashLine); }
    if (objScene->property("ENABLE_LWT").toBool()) { paintPen = lwtPen; }
    painter->setPen(paintPen);

    painter->drawPath(normalPath);

    if (objScene->property("ENABLE_LWT").toBool() && objScene->property("ENABLE_REAL").toBool()) { realRender(painter, normalPath); }
}

/**
 * @brief PolylineObject::updateRubber
 * @param painter
 */
void
PolylineObject::updateRubber(QPainter* painter)
{
    String rubberMode = objRubberMode;
    if (rubberMode == "OBJ_RUBBER_POLYLINE") {
        setObjectPos(objectRubberPoint("POLYLINE_POINT_0"));

        QLineF rubberLine(normalPath.currentPosition(), mapFromScene(objectRubberPoint(QString())));
        if (painter) drawRubberLine(rubberLine, painter, "VIEW_COLOR_CROSSHAIR");

        bool ok = false;
        QString numStr = objectRubberText("POLYLINE_NUM_POINTS");
        if (numStr.isNull()) return;
        int num = numStr.toInt(&ok);
        if (!ok) return;

        QString appendStr;
        QPainterPath rubberPath;
        for (int i = 1; i <= num; i++) {
            appendStr = "POLYLINE_POINT_" + QString().setNum(i);
            QPointF appendPoint = mapFromScene(objectRubberPoint(appendStr));
            rubberPath.lineTo(appendPoint);
        }
        updatePath(rubberPath);

        //Ensure the path isn't updated until the number of points is changed again
        setObjectRubberText("POLYLINE_NUM_POINTS", QString());
    }
    else if (rubberMode == "OBJ_RUBBER_GRIP") {
        if (painter) {
            int elemCount = normalPath.elementCount();
            QPointF gripPoint = objectRubberPoint("GRIP_POINT");
            if (gripIndex == -1) gripIndex = findIndex(gripPoint);
            if (gripIndex == -1) return;

            if (!gripIndex) {
                // First
                QPainterPath::Element ef = normalPath.elementAt(1);
                QPointF efPoint = QPointF(ef.x, ef.y);
                painter->drawLine(efPoint, mapFromScene(objectRubberPoint(QString())));
            }
            else if (gripIndex == elemCount-1) { //Last
                QPainterPath::Element el = normalPath.elementAt(gripIndex-1);
                QPointF elPoint = QPointF(el.x, el.y);
                painter->drawLine(elPoint, mapFromScene(objectRubberPoint(QString())));
            }
            else { //Middle
                QPainterPath::Element em = normalPath.elementAt(gripIndex-1);
                QPainterPath::Element en = normalPath.elementAt(gripIndex+1);
                QPointF emPoint = QPointF(em.x, em.y);
                QPointF enPoint = QPointF(en.x, en.y);
                painter->drawLine(emPoint, mapFromScene(objectRubberPoint(QString())));
                painter->drawLine(enPoint, mapFromScene(objectRubberPoint(QString())));
            }

            QLineF rubLine(mapFromScene(gripPoint), mapFromScene(objectRubberPoint(QString())));
            drawRubberLine(rubLine, painter, "VIEW_COLOR_CROSSHAIR");
        }
    }
}

/**
 * @brief PolylineObject::vulcanize
 */
void
PolylineObject::vulcanize()
{
    debug_message("PolylineObject vulcanize()");
    updateRubber();

    setObjectRubberMode("OBJ_RUBBER_OFF");

    if (!normalPath.elementCount())
        QMessageBox::critical(0, QObject::tr("Empty Polyline Error"), QObject::tr("The polyline added contains no points. The command that created this object has flawed logic."));
}

/**
 * @brief PolylineObject::mouseSnapPoint
 * @param mousePoint
 * @return The closest snap point to the mouse point.
 *
 * \todo use a genericized version.
 */
QPointF
PolylineObject::mouseSnapPoint(const QPointF& mousePoint)
{
    QPainterPath::Element element = normalPath.elementAt(0);
    QPointF closestPoint = mapToScene(QPointF(element.x, element.y));
    EmbReal closestDist = QLineF(mousePoint, closestPoint).length();
    int elemCount = normalPath.elementCount();
    for (int i = 0; i < elemCount; ++i) {
        element = normalPath.elementAt(i);
        QPointF elemPoint = mapToScene(element.x, element.y);
        EmbReal elemDist = QLineF(mousePoint, elemPoint).length();
        if (elemDist < closestDist) {
            closestPoint = elemPoint;
            closestDist = elemDist;
        }
    }
    return closestPoint;
}

/**
 * @brief PolylineObject::allGripPoints
 * @return
 */
std::vector<QPointF>
PolylineObject::allGripPoints()
{
    std::vector<QPointF> gripPoints;
    QPainterPath::Element element;
    for (int i = 0; i < normalPath.elementCount(); ++i) {
        element = normalPath.elementAt(i);
        gripPoints.push_back(mapToScene(element.x, element.y));
    }
    return gripPoints;
}

/**
 * @brief PolylineObject::findIndex
 * @param point
 * @return
 */
int
PolylineObject::findIndex(const QPointF& point)
{
    int elemCount = normalPath.elementCount();
    //NOTE: Points here are in item coordinates
    QPointF itemPoint = mapFromScene(point);
    for (int i = 0; i < elemCount; i++) {
        QPainterPath::Element e = normalPath.elementAt(i);
        QPointF elemPoint = QPointF(e.x, e.y);
        if (itemPoint == elemPoint) return i;
    }
    return -1;
}

/**
 * @brief PolylineObject::gripEdit
 * @param before
 * @param after
 */
void
PolylineObject::gripEdit(const QPointF& before, const QPointF& after)
{
    gripIndex = findIndex(before);
    if (gripIndex == -1) return;
    QPointF a = mapFromScene(after);
    normalPath.setElementPositionAt(gripIndex, a.x(), a.y());
    updatePath(normalPath);
    gripIndex = -1;
}

/**
 * @brief PolylineObject::objectCopyPath
 * @return
 */
QPainterPath
PolylineObject::objectCopyPath()
{
    return normalPath;
}

/**
 * @brief PolylineObject::objectSavePath
 * @return
 */
QPainterPath
PolylineObject::objectSavePath()
{
    EmbReal s = scale();
    QTransform trans;
    trans.rotate(rotation());
    trans.scale(s,s);
    return trans.map(normalPath);
}

/**
 * \brief .
 */
RectObject::RectObject(EmbReal x, EmbReal y, EmbReal w, EmbReal h, QRgb rgb, QGraphicsItem* parent) : BaseObject(OBJ_TYPE_RECTANGLE, parent)
{
    debug_message("RectObject Constructor()");
    init(x, y, w, h, rgb, Qt::SolidLine); //TODO: getCurrentLineType
}

/**
 * \brief .
 */
RectObject::RectObject(RectObject* obj, QGraphicsItem* parent) : BaseObject(OBJ_TYPE_RECTANGLE, parent)
{
    debug_message("RectObject Constructor()");
    if (obj) {
        QPointF ptl = obj->objectTopLeft();
        init(ptl.x(), ptl.y(), obj->objectWidth(), obj->objectHeight(), obj->objectColorRGB(), Qt::SolidLine); //TODO: getCurrentLineType
        setRotation(obj->rotation());
    }
}

/**
 * \brief RectObject destructor.
 */
RectObject::~RectObject()
{
    debug_message("RectObject Destructor()");
}

/**
 * \brief .
 */
void RectObject::init(EmbReal x, EmbReal y, EmbReal w, EmbReal h, QRgb rgb, Qt::PenStyle lineType)
{
    setData(OBJ_TYPE, OBJ_TYPE_RECTANGLE);
    setData(OBJ_NAME, "Rectangle");

    //WARNING: DO NOT enable QGraphicsItem::ItemIsMovable. If it is enabled,
    //WARNING: and the item is double clicked, the scene will erratically move the item while zooming.
    //WARNING: All movement has to be handled explicitly by us, not by the scene.
    setFlag(QGraphicsItem::ItemIsSelectable, true);

    setObjectRect(x, y, w, h);
    setObjectColor(rgb);
    setObjectLineType(lineType);
    setObjectLineWeight("0.35"); //TODO: pass in proper lineweight
    setPen(objPen);
}

/**
 * \brief .
 */
void RectObject::setObjectRect(EmbReal x, EmbReal y, EmbReal w, EmbReal h)
{
    setPos(x, y);
    setRect(0, 0, w, h);
    updatePath();
}

/**
 * \brief .
 * \return The top left corner location as a QPointF.
 */
QPointF
RectObject::objectTopLeft()
{
    EmbReal rot = radians(rotation());
    QPointF tl = rect().topLeft();
    EmbVector ptl = to_EmbVector(tl) * scale();
    EmbVector ptlrot = rotate_vector(ptl, rot);

    return scenePos() + to_QPointF(ptlrot);
}

/**
 * \brief .
 */
QPointF RectObject::objectTopRight()
{
    EmbReal rot = radians(rotation());
    QPointF tr = rect().topRight();
    EmbVector ptr = to_EmbVector(tr) * scale();
    EmbVector ptrrot = rotate_vector(ptr, rot);

    return scenePos() + to_QPointF(ptrrot);
}

/**
 * \brief .
 */
QPointF RectObject::objectBottomLeft()
{
    EmbReal rot = radians(rotation());
    QPointF bl = rect().bottomLeft();
    EmbVector pbl = to_EmbVector(bl) * scale();
    EmbVector pblrot = rotate_vector(pbl, rot);

    return scenePos() + to_QPointF(pblrot);
}

/**
 * \brief .
 */
QPointF RectObject::objectBottomRight()
{
    EmbReal rot = radians(rotation());
    QPointF br = rect().bottomLeft();
    EmbVector pbr = to_EmbVector(br) * scale();
    EmbVector pbrrot = rotate_vector(pbr, rot);

    return scenePos() + to_QPointF(pbrrot);
}

/**
 * \brief .
 */
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

/**
 * \brief .
 */
void RectObject::paint(QPainter* painter, const QStyleOptionGraphicsItem* option, QWidget* /*widget*/)
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
        paintPen = lwtPen;
    }
    painter->setPen(paintPen);

    painter->drawRect(rect());
}

/**
 * \brief .
 */
void RectObject::updateRubber(QPainter* painter)
{
    String rubberMode = objRubberMode;
    if (objRubberMode == "OBJ_RUBBER_RECTANGLE") {
        QPointF sceneStartPoint = objectRubberPoint("RECTANGLE_START");
        QPointF sceneEndPoint = objectRubberPoint("RECTANGLE_END");
        EmbReal x = sceneStartPoint.x();
        EmbReal y = sceneStartPoint.y();
        EmbReal w = sceneEndPoint.x() - sceneStartPoint.x();
        EmbReal h = sceneEndPoint.y() - sceneStartPoint.y();
        setObjectRect(x,y,w,h);
        updatePath();
    }
    else if (rubberMode == "OBJ_RUBBER_GRIP") {
        if (painter) {
            //TODO: Make this work with rotation & scaling
            /*
            QPointF gripPoint = objectRubberPoint("GRIP_POINT");
            QPointF after = objectRubberPoint(QString());
            QPointF delta = after-gripPoint;
            if     (gripPoint == objectTopLeft()) { painter->drawPolygon(mapFromScene(QRectF(after.x(), after.y(), objectWidth()-delta.x(), objectHeight()-delta.y()))); }
            else if (gripPoint == objectTopRight()) { painter->drawPolygon(mapFromScene(QRectF(objectTopLeft().x(), objectTopLeft().y()+delta.y(), objectWidth()+delta.x(), objectHeight()-delta.y()))); }
            else if (gripPoint == objectBottomLeft()) { painter->drawPolygon(mapFromScene(QRectF(objectTopLeft().x()+delta.x(), objectTopLeft().y(), objectWidth()-delta.x(), objectHeight()+delta.y()))); }
            else if (gripPoint == objectBottomRight()) { painter->drawPolygon(mapFromScene(QRectF(objectTopLeft().x(), objectTopLeft().y(), objectWidth()+delta.x(), objectHeight()+delta.y()))); }

            QLineF rubLine(mapFromScene(gripPoint), mapFromScene(objectRubberPoint(QString())));
            drawRubberLine(rubLine, painter, "VIEW_COLOR_CROSSHAIR");
            */

            QPointF gripPoint = objectRubberPoint("GRIP_POINT");
            QPointF after = objectRubberPoint(QString());
            QPointF delta = after-gripPoint;

            QLineF rubLine(mapFromScene(gripPoint), mapFromScene(objectRubberPoint(QString())));
            drawRubberLine(rubLine, painter, "VIEW_COLOR_CROSSHAIR");
        }
    }
}

/**
 * \brief .
 */
void RectObject::vulcanize()
{
    debug_message("RectObject vulcanize()");
    updateRubber();

    setObjectRubberMode("OBJ_RUBBER_OFF");
}

/**
 * \brief .
 * \return The closest snap point to the mouse point.
 *
 */
QPointF RectObject::mouseSnapPoint(const QPointF& mousePoint)
{
    std::vector<QPointF> points = {
        objectTopLeft(),     //Top Left Corner QSnap
        objectTopRight(),    //Top Right Corner QSnap
        objectBottomLeft(),  //Bottom Left Corner QSnap
        objectBottomRight()  //Bottom Right Corner QSnap
    };

    return closest_point(mousePoint, points);
}

/**
 * \brief .
 * \return A list of all grip points for the object.
 * \todo make return value a std::vector<std::string>
 */
std::vector<QPointF> RectObject::allGripPoints()
{
    std::vector<QPointF> gripPoints;
    gripPoints = {objectTopLeft(), objectTopRight(), objectBottomLeft(), objectBottomRight()};
    return gripPoints;
}

/**
 * \brief .
 */
void RectObject::gripEdit(const QPointF& before, const QPointF& after)
{
    QPointF delta = after-before;
    if (before == objectTopLeft()) {
        setObjectRect(after.x(), after.y(), objectWidth()-delta.x(), objectHeight()-delta.y());
    }
    else if (before == objectTopRight()) {
        setObjectRect(objectTopLeft().x(), objectTopLeft().y()+delta.y(), objectWidth()+delta.x(), objectHeight()-delta.y());
    }
    else if (before == objectBottomLeft()) {
        setObjectRect(objectTopLeft().x()+delta.x(), objectTopLeft().y(), objectWidth()-delta.x(), objectHeight()+delta.y());
    }
    else if (before == objectBottomRight()) {
        setObjectRect(objectTopLeft().x(), objectTopLeft().y(), objectWidth()+delta.x(), objectHeight()+delta.y());
    }
}

/**
 * \brief .
 */
QPainterPath
RectObject::objectSavePath()
{
    QPainterPath path;
    QRectF r = rect();
    path.moveTo(r.bottomLeft());
    path.lineTo(r.bottomRight());
    path.lineTo(r.topRight());
    path.lineTo(r.topLeft());
    path.lineTo(r.bottomLeft());

    EmbReal s = scale();
    QTransform trans;
    trans.rotate(rotation());
    trans.scale(s,s);
    return trans.map(path);
}

/**
 * @brief SaveObject::SaveObject
 * @param theScene
 * @param parent
 */
SaveObject::SaveObject(QGraphicsScene* theScene, QObject* parent) : QObject(parent)
{
    qDebug("SaveObject Constructor()");
    gscene = theScene;
    formatType = EMBFORMAT_UNSUPPORTED;
}

/**
 * @brief SaveObject::~SaveObject
 */
SaveObject::~SaveObject()
{
    qDebug("SaveObject Destructor()");
}

/**
 * Returns whether the save to file process was successful.
 *
 * \todo Before saving to a stitch only format, Embroidermodder needs
 * to calculate the optimal path to minimize jump stitches. Also
 * based upon which layer needs to be stitched first,
 * the path to the next object needs to be hidden beneath fills
 * that will come later. When finding the optimal path, we need
 * to take into account the color of the thread, as we do not want
 * to try to hide dark colored stitches beneath light colored fills.
 */
bool
SaveObject::save(QString fileName)
{
    qDebug("SaveObject save(%s)", qPrintable(fileName));

    bool writeSuccessful = false;
    int i;

    formatType = formatTable[emb_identify_format(qPrintable(fileName))].type;
    if (formatType == EMBFORMAT_UNSUPPORTED) {
        return false;
    }

    EmbPattern* pattern = embPattern_create();
    if (!pattern) {
        qDebug("Could not allocate memory for embroidery pattern");
    }

    /* Write */
    if (emb_identify_format(qPrintable(fileName)) < 0) {
        qDebug("Unsupported write file type: %s", qPrintable(fileName));
    }
    else {
        foreach(QGraphicsItem* item, gscene->items(Qt::AscendingOrder)) {
            int objType = item->data(OBJ_TYPE).toInt();

            if (objType == OBJ_TYPE_ARC) {
                addArc(pattern, item);
            }
            else if (objType == OBJ_TYPE_BLOCK) {
                addBlock(pattern, item);
            }
            else if (objType == OBJ_TYPE_CIRCLE) {
                addCircle(pattern, item);
            }
            else if (objType == OBJ_TYPE_DIMALIGNED) {
                addDimAligned(pattern, item);
            }
            else if (objType == OBJ_TYPE_DIMANGULAR) {
                addDimAngular(pattern, item);
            }
            else if (objType == OBJ_TYPE_DIMARCLENGTH) { addDimArcLength(pattern, item); }
            else if (objType == OBJ_TYPE_DIMDIAMETER) { addDimDiameter(pattern, item);  }
            else if (objType == OBJ_TYPE_DIMLEADER) { addDimLeader(pattern, item);    }
            else if (objType == OBJ_TYPE_DIMLINEAR) { addDimLinear(pattern, item);    }
            else if (objType == OBJ_TYPE_DIMORDINATE) { addDimOrdinate(pattern, item);  }
            else if (objType == OBJ_TYPE_DIMRADIUS) { addDimRadius(pattern, item);    }
            else if (objType == OBJ_TYPE_ELLIPSE) { addEllipse(pattern, item);      }
            else if (objType == OBJ_TYPE_ELLIPSEARC) { addEllipseArc(pattern, item);   }
            else if (objType == OBJ_TYPE_GRID) { addGrid(pattern, item);         }
            else if (objType == OBJ_TYPE_HATCH) { addHatch(pattern, item);        }
            else if (objType == OBJ_TYPE_IMAGE) { addImage(pattern, item);        }
            else if (objType == OBJ_TYPE_INFINITELINE) { addInfiniteLine(pattern, item); }
            else if (objType == OBJ_TYPE_LINE) { addLine(pattern, item);         }
            else if (objType == OBJ_TYPE_POINT) { addPoint(pattern, item);        }
            else if (objType == OBJ_TYPE_POLYGON) { addPolygon(pattern, item);      }
            else if (objType == OBJ_TYPE_POLYLINE) { addPolyline(pattern, item);     }
            else if (objType == OBJ_TYPE_RAY) { addRay(pattern, item);          }
            else if (objType == OBJ_TYPE_RECTANGLE) { addRectangle(pattern, item);    }
            else if (objType == OBJ_TYPE_SPLINE) { addSpline(pattern, item);       }
            else if (objType == OBJ_TYPE_TEXTMULTI) { addTextMulti(pattern, item);    }
            else if (objType == OBJ_TYPE_TEXTSINGLE) { addTextSingle(pattern, item);   }
        }

        /*
        //TODO: handle EMBFORMAT_STCHANDOBJ also
        if (formatType == EMBFORMAT_STITCHONLY)
            embPattern_movePolylinesToStitchList(pattern); //TODO: handle all objects like this
        */

        writeSuccessful = embPattern_writeAuto(pattern, qPrintable(fileName));
        if (!writeSuccessful) {
            qDebug("Writing file %s was unsuccessful", qPrintable(fileName));
        }
    }

    //TODO: check the embLog for errors and if any exist, report them.
    embPattern_free(pattern);

    return writeSuccessful;
}

/**
 * @brief SaveObject::addArc
 * @param pattern
 * @param item
 */
void
SaveObject::addArc(EmbPattern* pattern, QGraphicsItem* item)
{
}

/**
 * @brief SaveObject::addBlock
 * @param pattern
 * @param item
 */
void
SaveObject::addBlock(EmbPattern* pattern, QGraphicsItem* item)
{
}

/**
 * @brief SaveObject::addCircle
 * @param pattern
 * @param item
 */
void
SaveObject::addCircle(EmbPattern* pattern, QGraphicsItem* item)
{
    CircleObject* obj = static_cast<CircleObject*>(item);
    if (obj) {
        if (formatType == EMBFORMAT_STITCHONLY) {
            QPainterPath path = obj->objectSavePath();
            toPolyline(pattern, obj->objectCenter(), path.simplified(), "0", obj->objectColor(), "CONTINUOUS", "BYLAYER"); //TODO: proper layer/lineType/lineWeight //TODO: Improve precision, replace simplified
        }
        else {
            EmbCircle circle;
            circle.center.x = (double)obj->objectCenter().x();
            circle.center.y = (double)obj->objectCenter().y();
            circle.radius = (double)obj->objectRadius();
            
            embPattern_addCircleAbs(pattern, circle);
        }
    }
}

/**
 * @brief SaveObject::addDimAligned
 * @param pattern
 * @param item
 */
void
SaveObject::addDimAligned(EmbPattern* pattern, QGraphicsItem* item)
{
}

/**
 * @brief SaveObject::addDimAngular
 * @param pattern
 * @param item
 */
void
SaveObject::addDimAngular(EmbPattern* pattern, QGraphicsItem* item)
{
}

/**
 * @brief SaveObject::addDimArcLength
 * @param pattern
 * @param item
 */
void
SaveObject::addDimArcLength(EmbPattern* pattern, QGraphicsItem* item)
{
}

/**
 * @brief SaveObject::addDimDiameter
 * @param pattern
 * @param item
 */
void
SaveObject::addDimDiameter(EmbPattern* pattern, QGraphicsItem* item)
{
}

/**
 * @brief SaveObject::addDimLeader
 * @param pattern
 * @param item
 */
void
SaveObject::addDimLeader(EmbPattern* pattern, QGraphicsItem* item)
{
}

/**
 * @brief SaveObject::addDimLinear
 * @param pattern
 * @param item
 */
void
SaveObject::addDimLinear(EmbPattern* pattern, QGraphicsItem* item)
{
}

/**
 * @brief SaveObject::addDimOrdinate
 * @param pattern
 * @param item
 */
void
SaveObject::addDimOrdinate(EmbPattern* pattern, QGraphicsItem* item)
{
}

/**
 * @brief SaveObject::addDimRadius
 * @param pattern
 * @param item
 */
void
SaveObject::addDimRadius(EmbPattern* pattern, QGraphicsItem* item)
{
}

/**
 * @brief SaveObject::addEllipse
 * @param pattern
 * @param item
 */
void
SaveObject::addEllipse(EmbPattern* pattern, QGraphicsItem* item)
{
    EllipseObject* obj = static_cast<EllipseObject*>(item);
    if (obj) {
        if (formatType == EMBFORMAT_STITCHONLY) {
            QPainterPath path = obj->objectSavePath();
            toPolyline(pattern, obj->objectCenter(), path.simplified(), "0", obj->objectColor(), "CONTINUOUS", "BYLAYER"); //TODO: proper layer/lineType/lineWeight //TODO: Improve precision, replace simplified
        }
        else {
            EmbEllipse ellipse;
            ellipse.center.x = (double)obj->objectCenter().x();
            ellipse.center.y = (double)obj->objectCenter().y();
            ellipse.radius.x = (double)obj->objectWidth()/2.0;
            ellipse.radius.y = (double)obj->objectHeight()/2.0;
            //TODO: ellipse rotation
            embPattern_addEllipseAbs(pattern, ellipse);
        }
    }
}

/**
 * @brief SaveObject::addEllipseArc
 * @param pattern
 * @param item
 */
void
SaveObject::addEllipseArc(EmbPattern* pattern, QGraphicsItem* item)
{
}

/**
 * @brief SaveObject::addGrid
 * @param pattern
 * @param item
 */
void
SaveObject::addGrid(EmbPattern* pattern, QGraphicsItem* item)
{
}

/**
 * @brief SaveObject::addHatch
 * @param pattern
 * @param item
 */
void
SaveObject::addHatch(EmbPattern* pattern, QGraphicsItem* item)
{
}

/**
 * @brief SaveObject::addImage
 * @param pattern
 * @param item
 */
void
SaveObject::addImage(EmbPattern* pattern, QGraphicsItem* item)
{
}

/**
 * @brief SaveObject::addInfiniteLine
 * @param pattern
 * @param item
 */
void
SaveObject::addInfiniteLine(EmbPattern* pattern, QGraphicsItem* item)
{
}

/**
 * @brief SaveObject::addLine
 * @param pattern
 * @param item
 */
void
SaveObject::addLine(EmbPattern* pattern, QGraphicsItem* item)
{
    LineObject* obj = static_cast<LineObject*>(item);
    if (obj) {
        if (formatType == EMBFORMAT_STITCHONLY) {
            toPolyline(pattern, obj->objectEndPoint1(), obj->objectSavePath(), "0", obj->objectColor(), "CONTINUOUS", "BYLAYER"); //TODO: proper layer/lineType/lineWeight
        }
        else {
            EmbLine line;
            line.start = to_EmbVector(obj->objectEndPoint1());
            line.end = to_EmbVector(obj->objectEndPoint2());
            embPattern_addLineAbs(pattern, line);
        }
    }
}

/**
 * @brief SaveObject::addPath
 * @param pattern
 * @param item
 *
 * \todo Reimplement addPolyline() using the libembroidery C API
 */
void
SaveObject::addPath(EmbPattern* pattern, QGraphicsItem* item)
{
    qDebug("addPolyline()");
    /*
    QGraphicsPathItem* polylineItem = (QGraphicsPathItem*)item;
    if (polylineItem) {
        QPainterPath path = polylineItem->path();
        QPointF pos = polylineItem->pos();
        EmbReal startX = pos.x();
        EmbReal startY = pos.y();

        QPainterPath::Element element;
        QPainterPath::Element P1;
        QPainterPath::Element P2;
        QPainterPath::Element P3;
        QPainterPath::Element P4;

        for (int i = 0; i < path.elementCount()-1; ++i) {
            element = path.elementAt(i);
            if (element.isMoveTo()) {
                pattern.AddStitchAbs((element.x + startX), -(element.y + startY), TRIM);
            }
            else if (element.isLineTo()) {
                pattern.AddStitchAbs((element.x + startX), -(element.y + startY), NORMAL);
            }
            else if (element.isCurveTo()) {
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

/**
 * @brief SaveObject::addPoint
 * @param pattern
 * @param item
 */
void
SaveObject::addPoint(EmbPattern* pattern, QGraphicsItem* item)
{
    PointObject* obj = static_cast<PointObject*>(item);
    if (obj) {
        if (formatType == EMBFORMAT_STITCHONLY) {
            toPolyline(pattern, obj->objectPos(), obj->objectSavePath(), "0", obj->objectColor(), "CONTINUOUS", "BYLAYER"); //TODO: proper layer/lineType/lineWeight
        }
        else {
            EmbPoint po;
            po.position.x = (double)obj->objectPos().x();
            po.position.y = (double)obj->objectPos().y();
            embPattern_addPointAbs(pattern, po);
        }
    }
}

/**
 * @brief SaveObject::addPolygon
 * @param pattern
 * @param item
 */
void
SaveObject::addPolygon(EmbPattern* pattern, QGraphicsItem* item)
{
    PolygonObject* obj = static_cast<PolygonObject*>(item);
    if (obj) {
        toPolyline(pattern, obj->objectPos(), obj->objectSavePath(), "0", obj->objectColor(), "CONTINUOUS", "BYLAYER"); //TODO: proper layer/lineType/lineWeight
    }
}

/**
 * @brief SaveObject::addPolyline
 * @param pattern
 * @param item
 */
void
SaveObject::addPolyline(EmbPattern* pattern, QGraphicsItem* item)
{
    PolylineObject* obj = static_cast<PolylineObject*>(item);
    if (obj) {
        toPolyline(pattern, obj->objectPos(), obj->objectSavePath(), "0", obj->objectColor(), "CONTINUOUS", "BYLAYER"); //TODO: proper layer/lineType/lineWeight
    }
}

/**
 * @brief SaveObject::addRay
 * @param pattern
 * @param item
 */
void
SaveObject::addRay(EmbPattern* pattern, QGraphicsItem* item)
{
}

/**
 * @brief SaveObject::addRectangle
 * @param pattern
 * @param item
 */
void
SaveObject::addRectangle(EmbPattern* pattern, QGraphicsItem* item)
{
    RectObject* obj = static_cast<RectObject*>(item);
    if (obj) {
        if (formatType == EMBFORMAT_STITCHONLY) {
            toPolyline(pattern, obj->objectPos(), obj->objectSavePath(), "0", obj->objectColor(), "CONTINUOUS", "BYLAYER"); //TODO: proper layer/lineType/lineWeight
        }
        else {
            //TODO: Review this at some point
            QPointF topLeft = obj->objectTopLeft();
            EmbRect r;
            r.top = topLeft.x();
            r.left = topLeft.y();
            r.right = r.left + (double)obj->objectWidth();
            r.bottom = r.top + (double)obj->objectHeight();
            embPattern_addRectAbs(pattern, r);
        }
    }
}

/**
 * @brief SaveObject::addSlot
 * @param pattern
 * @param item
 */
void
SaveObject::addSlot(EmbPattern* pattern, QGraphicsItem* item)
{
}

/**
 * @brief SaveObject::addSpline
 * @param pattern
 * @param item
 */
void
SaveObject::addSpline(EmbPattern* pattern, QGraphicsItem* item)
{
    //TODO: abstract bezier into geom-bezier... cubicBezierMagic(P1, P2, P3, P4, 0.0, 1.0, tPoints);
}

/**
 * @brief SaveObject::addTextMulti
 * @param pattern
 * @param item
 */
void
SaveObject::addTextMulti(EmbPattern* pattern, QGraphicsItem* item)
{
    //TODO: saving polygons, polylines and paths must be stable before we go here.
}

/**
 * @brief SaveObject::addTextSingle
 * @param pattern
 * @param item
 */
void
SaveObject::addTextSingle(EmbPattern* pattern, QGraphicsItem* item)
{
    //TODO: saving polygons, polylines and paths must be stable before we go here.

    //TODO: This needs to work like a path, not a polyline. Improve this
    TextSingleObject* obj = static_cast<TextSingleObject*>(item);
    if (obj) {
        if (formatType == EMBFORMAT_STITCHONLY) {
            std::vector<QPainterPath> pathList = obj->objectSavePathList();
            foreach(QPainterPath path, pathList) {
                toPolyline(pattern, obj->objectPos(), path.simplified(), "0", obj->objectColor(), "CONTINUOUS", "BYLAYER"); //TODO: proper layer/lineType/lineWeight //TODO: Improve precision, replace simplified
            }
        }
        else {

        }
    }
}

/**
 * @brief SaveObject::toPolyline
 * @param pattern
 * @param objPos
 * @param objPath
 * @param layer
 * @param color
 * @param lineType
 * @param lineWeight
 *
 * \note This function should be used to interpret various object types and save them as polylines for stitchOnly formats.
 */
void
SaveObject::toPolyline(EmbPattern* pattern, const QPointF& objPos, const QPainterPath& objPath, QString  layer, const QColor& color, QString  lineType, QString  lineWeight)
{
    EmbReal startX = objPos.x();
    EmbReal startY = objPos.y();
    EmbArray* pointList = 0;
    QPainterPath::Element element;
    for (int i = 0; i < objPath.elementCount(); ++i) {
        element = objPath.elementAt(i);
        if (!pointList) {
            pointList = embArray_create(EMB_POINT);
        }
        EmbPoint po;
        po.position.x = element.x + startX;
        po.position.y = -(element.y + startY);
        embArray_addPoint(pointList, po);
    }

    EmbColor color_out;
    color_out.r = color.red();
    color_out.g = color.green();
    color_out.b = color.blue();
    
    /**
    \todo FIX
    EmbPolyline* polyObject = embPolyline_init(pointList, color_out, 1); //TODO: proper lineType
    embPattern_addPolylineAbs(pattern, polyObject);
    */
}

/**
 *
 */
TextSingleObject::TextSingleObject(QString  str, EmbReal x, EmbReal y, QRgb rgb, QGraphicsItem* parent) : BaseObject(OBJ_TYPE_TEXTSINGLE, parent)
{
    debug_message("TextSingleObject Constructor()");
    init(str, x, y, rgb, Qt::SolidLine); //TODO: getCurrentLineType
}

/**
 *
 */
TextSingleObject::TextSingleObject(TextSingleObject* obj, QGraphicsItem* parent) : BaseObject(OBJ_TYPE_TEXTSINGLE, parent)
{
    debug_message("TextSingleObject Constructor()");
    if (obj) {
        setObjectTextFont(obj->objTextFont);
        setObjectTextSize(obj->objTextSize);
        setRotation(obj->rotation());
        setObjectTextBackward(obj->objTextBackward);
        setObjectTextUpsideDown(obj->objTextUpsideDown);
        setObjectTextStyle(obj->objTextBold, obj->objTextItalic, obj->objTextUnderline, obj->objTextStrikeOut, obj->objTextOverline);
        init(obj->objText, obj->objectX(), obj->objectY(), obj->objectColorRGB(), Qt::SolidLine); //TODO: getCurrentLineType
        setScale(obj->scale());
    }
}

/**
 *
 */
TextSingleObject::~TextSingleObject()
{
    debug_message("TextSingleObject Destructor()");
}

/**
 *
 */
void
TextSingleObject::init(QString  str, EmbReal x, EmbReal y, QRgb rgb, Qt::PenStyle lineType)
{
    setData(OBJ_TYPE, OBJ_TYPE_TEXTSINGLE);
    setData(OBJ_NAME, "Single Line Text");

    //WARNING: DO NOT enable QGraphicsItem::ItemIsMovable. If it is enabled,
    //WARNING: and the item is double clicked, the scene will erratically move the item while zooming.
    //WARNING: All movement has to be handled explicitly by us, not by the scene.
    setFlag(QGraphicsItem::ItemIsSelectable, true);

    objTextJustify = "Left"; //TODO: set the justification properly

    setObjectText(str);
    setObjectPos(x,y);
    setObjectColor(rgb);
    setObjectLineType(lineType);
    setObjectLineWeight("0.35"); //TODO: pass in proper lineweight
    setPen(objPen);
}

QStringList justify_options = {
    "Left",
    "Center",
    "Right",
    "Aligned",
    "Middle",
    "Fit",
    "Top Left",
    "Top Center",
    "Top Right",
    "Middle Left",
    "Middle Center",
    "Middle Right",
    "Bottom Left",
    "Bottom Center",
    "Bottom Right"
/* \todo "Aligned"
 * \todo "Fit"
 */
};

/**
 *
 */
void TextSingleObject::setObjectText(QString  str)
{
    objText = str;
    QPainterPath textPath;
    QFont font;
    font.setFamily(objTextFont);
    font.setPointSizeF(objTextSize);
    font.setBold(objTextBold);
    font.setItalic(objTextItalic);
    font.setUnderline(objTextUnderline);
    font.setStrikeOut(objTextStrikeOut);
    font.setOverline(objTextOverline);
    textPath.addText(0, 0, font, str);

    //Translate the path based on the justification
    QRectF jRect = textPath.boundingRect();
    if (objTextJustify == "Left") {
        textPath.translate(-jRect.left(), 0);
    }
    else if (objTextJustify == "Center") {
        textPath.translate(-jRect.center().x(), 0);
    }
    else if (objTextJustify == "Right") { textPath.translate(-jRect.right(), 0); }
    else if (objTextJustify == "Aligned") { } //TODO: TextSingleObject Aligned Justification
    else if (objTextJustify == "Middle") { textPath.translate(-jRect.center()); }
    else if (objTextJustify == "Fit") { } //TODO: TextSingleObject Fit Justification
    else if (objTextJustify == "Top Left") { textPath.translate(-jRect.topLeft()); }
    else if (objTextJustify == "Top Center") { textPath.translate(-jRect.center().x(), -jRect.top()); }
    else if (objTextJustify == "Top Right") { textPath.translate(-jRect.topRight()); }
    else if (objTextJustify == "Middle Left") { textPath.translate(-jRect.left(), -jRect.top()/2.0); }
    else if (objTextJustify == "Middle Center") { textPath.translate(-jRect.center().x(), -jRect.top()/2.0); }
    else if (objTextJustify == "Middle Right") { textPath.translate(-jRect.right(), -jRect.top()/2.0); }
    else if (objTextJustify == "Bottom Left") { textPath.translate(-jRect.bottomLeft()); }
    else if (objTextJustify == "Bottom Center") { textPath.translate(-jRect.center().x(), -jRect.bottom()); }
    else if (objTextJustify == "Bottom Right") { textPath.translate(-jRect.bottomRight()); }

    //Backward or Upside Down
    if (objTextBackward || objTextUpsideDown) {
        EmbReal horiz = 1.0;
        EmbReal vert = 1.0;
        if (objTextBackward) horiz = -1.0;
        if (objTextUpsideDown) vert = -1.0;

        QPainterPath flippedPath;

        QPainterPath::Element element;
        QPainterPath::Element P2;
        QPainterPath::Element P3;
        QPainterPath::Element P4;
        for (int i = 0; i < textPath.elementCount(); ++i) {
            element = textPath.elementAt(i);
            if (element.isMoveTo()) {
                flippedPath.moveTo(horiz * element.x, vert * element.y);
            }
            else if (element.isLineTo()) {
                flippedPath.lineTo(horiz * element.x, vert * element.y);
            }
            else if (element.isCurveTo()) {
                                              // start point P1 is not needed
                P2 = textPath.elementAt(i);   // control point
                P3 = textPath.elementAt(i+1); // control point
                P4 = textPath.elementAt(i+2); // end point

                flippedPath.cubicTo(horiz * P2.x, vert * P2.y,
                                    horiz * P3.x, vert * P3.y,
                                    horiz * P4.x, vert * P4.y);
            }
        }
        objTextPath = flippedPath;
    }
    else {
        objTextPath = textPath;
    }

    //Add the grip point to the shape path
    QPainterPath gripPath = objTextPath;
    gripPath.connectPath(objTextPath);
    gripPath.addRect(-0.00000001, -0.00000001, 0.00000002, 0.00000002);
    setObjectPath(gripPath);
}

/**
 *
 */
void
TextSingleObject::setObjectTextFont(QString  font)
{
    objTextFont = font;
    setObjectText(objText);
}

/**
 * Verify the string is a valid option, otherwise default to "Left".
 */
void
TextSingleObject::setObjectTextJustify(QString  justify)
{
    objTextJustify = "Left";
    for (int i=0; i<justify_options.size(); i++) {
        if (justify == justify_options[i]) {
            objTextJustify = justify;
        }
    }
    setObjectText(objText);
}

/**
 *
 */
void
TextSingleObject::setObjectTextSize(EmbReal size)
{
    objTextSize = size;
    setObjectText(objText);
}

/**
 *
 */
void
TextSingleObject::setObjectTextStyle(bool bold, bool italic, bool under, bool strike, bool over)
{
    objTextBold = bold;
    objTextItalic = italic;
    objTextUnderline = under;
    objTextStrikeOut = strike;
    objTextOverline = over;
    setObjectText(objText);
}

/**
 *
 */
void
TextSingleObject::setObjectTextBold(bool val)
{
    objTextBold = val;
    setObjectText(objText);
}

/**
 *
 */
void
TextSingleObject::setObjectTextItalic(bool val)
{
    objTextItalic = val;
    setObjectText(objText);
}

/**
 *
 */
void
TextSingleObject::setObjectTextUnderline(bool val)
{
    objTextUnderline = val;
    setObjectText(objText);
}

/**
 *
 */
void
TextSingleObject::setObjectTextStrikeOut(bool val)
{
    objTextStrikeOut = val;
    setObjectText(objText);
}

/**
 *
 */
void
TextSingleObject::setObjectTextOverline(bool val)
{
    objTextOverline = val;
    setObjectText(objText);
}

/**
 *
 */
void
TextSingleObject::setObjectTextBackward(bool val)
{
    objTextBackward = val;
    setObjectText(objText);
}

/**
 *
 */
void
TextSingleObject::setObjectTextUpsideDown(bool val)
{
    objTextUpsideDown = val;
    setObjectText(objText);
}

/**
 *
 */
void
TextSingleObject::paint(QPainter* painter, const QStyleOptionGraphicsItem* option, QWidget* /*widget*/)
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
        paintPen = lwtPen;
    }
    painter->setPen(paintPen);

    painter->drawPath(objTextPath);
}

/**
 *
 */
void
TextSingleObject::updateRubber(QPainter* painter)
{
    String rubberMode = objRubberMode;
    if (rubberMode == "OBJ_RUBBER_TEXTSINGLE") {
        setObjectTextFont(objectRubberText("TEXT_FONT"));
        setObjectTextJustify(objectRubberText("TEXT_JUSTIFY"));
        setObjectPos(objectRubberPoint("TEXT_POINT"));
        QPointF hr = objectRubberPoint("TEXT_HEIGHT_ROTATION");
        setObjectTextSize(hr.x());
        setRotation(hr.y());
        setObjectText(objectRubberText("TEXT_RAPID"));
    }
    else if (rubberMode == "OBJ_RUBBER_GRIP") {
        if (painter) {
            QPointF gripPoint = objectRubberPoint("GRIP_POINT");
            if (gripPoint == scenePos()) {
                painter->drawPath(path().translated(mapFromScene(objectRubberPoint(QString()))-mapFromScene(gripPoint)));
            }

            QLineF rubLine(mapFromScene(gripPoint), mapFromScene(objectRubberPoint(QString())));
            drawRubberLine(rubLine, painter, "VIEW_COLOR_CROSSHAIR");
        }
    }
}

/**
 *
 */
void
TextSingleObject::vulcanize()
{
    debug_message("TextSingleObject vulcanize()");
    updateRubber();

    objRubberMode = "OBJ_RUBBER_OFF";
}

/**
 *
 * Returns the closest snap point to the mouse point
 */
QPointF TextSingleObject::mouseSnapPoint(const QPointF& mousePoint)
{
    return scenePos();
}

/**
 *
 */
std::vector<QPointF>
TextSingleObject::allGripPoints()
{
    std::vector<QPointF> gripPoints = {scenePos()};
    return gripPoints;
}

/**
 *
 */
void
TextSingleObject::gripEdit(const QPointF& before, const QPointF& after)
{
    if (before == scenePos()) {
        QPointF delta = after-before;
        moveBy(delta.x(), delta.y());
    }
}

/**
 *
 */
std::vector<QPainterPath>
TextSingleObject::subPathList()
{
    EmbReal s = scale();
    QTransform trans;
    trans.rotate(rotation());
    trans.scale(s,s);

    std::vector<QPainterPath> pathList;

    QPainterPath path = objTextPath;

    QPainterPath::Element element;
    std::vector<int> pathMoves;
    int numMoves = 0;

    for (int i = 0; i < path.elementCount(); i++) {
        element = path.elementAt(i);
        if (element.isMoveTo()) {
            pathMoves.push_back(i);
            numMoves++;
        }
    }

    pathMoves.push_back(path.elementCount());

    for (int p = 0; p < pathMoves.size()-1 && p < numMoves; p++) {
        QPainterPath subPath;
        for (int i = pathMoves[p]; i < pathMoves[p+1]; i++) {
            element = path.elementAt(i);
            if (element.isMoveTo()) {
                subPath.moveTo(element.x, element.y);
            }
            else if (element.isLineTo()) {
                subPath.lineTo(element.x, element.y);
            }
            else if (element.isCurveTo()) {
                subPath.cubicTo(path.elementAt(i  ).x, path.elementAt(i  ).y,  //control point 1
                                path.elementAt(i+1).x, path.elementAt(i+1).y,  //control point 2
                                path.elementAt(i+2).x, path.elementAt(i+2).y); //end point
            }
        }
        pathList.push_back(trans.map(subPath));
    }

    return pathList;
}


/**
 * @brief circle_main
 * @return
 */
Dictionary
circle_main(void)
{
    Dictionary global;
    global["mode"] = node("CIRCLE_MODE_1P_RAD");
    /*
    initCommand();
    clearSelection();
    setPromptPrefix(tr("Specify center point for circle or [3P/2P/Ttr (tan tan radius)]: "));
    */
    return global;
}

/**
 * @brief circle_click
 * @return
 *
 * In CIRCLE_MODE_1P_RAD mode: for the circle object currently focussed,
 * show two rubber points: one for the centre (the anchor) and
 * the other at some point on the radius to adjust the radius.
 *
 * In CIRCLE_MODE_1P_DIA For the curcle object currently focussed,
 * show two rubber points: one for the left of the diameter and one for the right.
 * These rubber points can be moved around the circle, but they always
 * oppose one another.
 */
void
circle_click(Dictionary global, EmbVector v)
{
    /*
    if (global["mode"].s == "CIRCLE_MODE_1P_RAD") {
        auto iter = global.find("point1");
        if (iter == global.end()) {
            global["point1"] = node(v);
            global["center"] = node(v);
            addRubber("CIRCLE");
            setRubberMode("CIRCLE_1P_RAD");
            setRubberPoint("CIRCLE_CENTER", global->center.x, global->center.y);
            actuator("append-prompt");
            setPromptPrefix(tr("Specify radius of circle or [Diameter]: "));
        }
        else {
            global["point2"] = node(v);
            setRubberPoint("CIRCLE_RADIUS", v);
            actuator("vulcanize");
            actuator("append-prompt");
            actuator("end");
        }
    }
    else if (global["mode"].s == MODE_1P_DIA) {
        auto iter = global.find("point1");
        if (iter == global.end()) {
            error("CIRCLE", tr("This should never happen."));
        }
        else {
            global.x2 = x;
            global.y2 = y;
            setRubberPoint("CIRCLE_DIAMETER", global.x2, global.y2);
            actuator("vulcanize");
            appendPromptHistory();
            actuator("end");
        }
    }
    else if (global["mode"].s == "MODE_2P") {
        auto iter1 = global.find("point1");
        auto iter2 = global.find("point2");
        if (iter1 == global.end()) {
            global.point1 = v;
            addRubber("CIRCLE");
            setRubberMode("CIRCLE_2P");
            setRubberPoint("CIRCLE_TAN1", v);
            appendPromptHistory();
            setPromptPrefix(tr("Specify second end point of circle's diameter: "));
        }
        else if (iter2 == global.end()) {
            global.point2 = v;
            setRubberPoint("CIRCLE_TAN2", v);
            actuator("vulcanize");
            appendPromptHistory();
            actuator("end");
        }
        else {
            error("CIRCLE", tr("This should never happen."));
        }
    }
    else if (global["mode"].s == MODE_3P) {
        if (std::isnan(global.x1)) {
            global.x1 = x;
            global.y1 = y;
            appendPromptHistory();
            setPromptPrefix(tr("Specify second point on circle: "));
        }
        else if (std::isnan(global.x2)) {
            global.x2 = x;
            global.y2 = y;
            addRubber("CIRCLE");
            setRubberMode("CIRCLE_3P");
            setRubberPoint("CIRCLE_TAN1", global.x1, global.y1);
            setRubberPoint("CIRCLE_TAN2", global.x2, global.y2);
            appendPromptHistory();
            setPromptPrefix(tr("Specify third point on circle: "));
        }
        else if (std::isnan(global.x3)) {
            global.x3 = x;
            global.y3 = y;
            setRubberPoint("CIRCLE_TAN3", global.x3, global.y3);
            actuator("vulcanize");
            appendPromptHistory();
            actuator("end");
        }
        else {
            error("CIRCLE", tr("This should never happen."));
        }
    }
    else if (global["mode"].s == MODE_TTR) {
        if (std::isnan(global.x1)) {
            global.x1 = x;
            global.y1 = y;
            appendPromptHistory();
            setPromptPrefix(tr("Specify point on object for second tangent of circle: "));
        }
        else if (std::isnan(global.x2)) {
            global.x2 = x;
            global.y2 = y;
            appendPromptHistory();
            setPromptPrefix(tr("Specify radius of circle: "));
        }
        else if (std::isnan(global.x3)) {
            global.x3 = x;
            global.y3 = y;
            appendPromptHistory();
            setPromptPrefix(tr("Specify second point: "));
        }
        else {
            todo("CIRCLE", "click() for TTR");
        }
    }
    */
}

/**
 * .
 */
void
circle_context(String str)
{
    //todo("CIRCLE", "context()");
}

/**
 * .
 */
String
circle_prompt(String str)
{
    /*
    if (global["mode"].s == MODE_1P_RAD) {
        if (std::isnan(global.x1)) {
            if (str == "2P") {
                global.mode = MODE_2P;
                setPromptPrefix(tr("Specify first end point of circle's diameter: "));
            }
            else if (str == "3P") {
                global.mode = MODE_3P;
                setPromptPrefix(tr("Specify first point of circle: "));
            }
            else if (str == "T" || str == "TTR") {
                global.mode = MODE_TTR;
                setPromptPrefix(tr("Specify point on object for first tangent of circle: "));
            }
            else {
                EmbReal strList = str.split(",");
                if (std::isnan(strList[0]) || std::isnan(strList[1])) {
                    alert(tr("Point or option keyword required."));
                    setPromptPrefix(tr("Specify center point for circle or [3P/2P/Ttr (tan tan radius)]: "));
                }
                else {
                    global.x1 = Number(strList[0]);
                    global.y1 = Number(strList[1]);
                    global->center.x = global.x1;
                    global->center.y = global.y1;
                    addRubber("CIRCLE");
                    setRubberMode("CIRCLE_1P_RAD");
                    setRubberPoint("CIRCLE_CENTER", global->center.x, global->center.y);
                    setPromptPrefix(tr("Specify radius of circle or [Diameter]: "));
                }
            }
        }
        else {
            if (str == "D" || str == "DIAMETER") {
                global.mode = MODE_1P_DIA;
                setRubberMode("CIRCLE_1P_DIA");
                setPromptPrefix(tr("Specify diameter of circle: "));
            }
            else {
                EmbReal num = Number(str);
                if (std::isnan(num)) {
                    alert(tr("Requires numeric radius, point on circumference, or \"D\"."));
                    setPromptPrefix(tr("Specify radius of circle or [Diameter]: "));
                }
                else {
                    global.rad = num;
                    global.x2 = global.x1 + global.rad;
                    global.y2 = global.y1;
                    setRubberPoint("CIRCLE_RADIUS", global.x2, global.y2);
                    actuator("vulcanize");
                    actuator("end");
                }
            }
        }
    }
    else if (global["mode"].s == MODE_1P_DIA) {
        if (std::isnan(global.x1)) {
            error("CIRCLE", tr("This should never happen."));
        }
        if (std::isnan(global.x2)) {
            EmbReal num = Number(str);
            if (std::isnan(num)) {
                alert(tr("Requires numeric distance or second point."));
                setPromptPrefix(tr("Specify diameter of circle: "));
            }
            else {
                global.dia = num;
                global.x2 = global.x1 + global.dia;
                global.y2 = global.y1;
                setRubberPoint("CIRCLE_DIAMETER", global.x2, global.y2);
                actuator("vulcanize");
                actuator("end");
            }
        }
        else {
            error("CIRCLE", tr("This should never happen."));
        }
    }
    else if (global["mode"].s == MODE_2P) {
        if (std::isnan(global.x1)) {
            EmbReal strList = str.split(",");
            if (std::isnan(strList[0]) || std::isnan(strList[1])) {
                alert(tr("Invalid point."));
                setPromptPrefix(tr("Specify first end point of circle's diameter: "));
            }
            else {
                global.x1 = Number(strList[0]);
                global.y1 = Number(strList[1]);
                addRubber("CIRCLE");
                setRubberMode("CIRCLE_2P");
                setRubberPoint("CIRCLE_TAN1", global.x1, global.y1);
                setPromptPrefix(tr("Specify second end point of circle's diameter: "));
            }
        }
        else if (std::isnan(global.x2)) {
            EmbReal strList = str.split(",");
            if (std::isnan(strList[0]) || std::isnan(strList[1])) {
                alert(tr("Invalid point."));
                setPromptPrefix(tr("Specify second end point of circle's diameter: "));
            }
            else {
                global.x2 = Number(strList[0]);
                global.y2 = Number(strList[1]);
                setRubberPoint("CIRCLE_TAN2", global.x2, global.y2);
                actuator("vulcanize");
                actuator("end");
            }
        }
        else {
            error("CIRCLE", tr("This should never happen."));
        }
    }
    else if (global["mode"].s == MODE_3P) {
        if (std::isnan(global.x1)) {
            EmbReal strList = str.split(",");
            if (std::isnan(strList[0]) || std::isnan(strList[1])) {
                alert(tr("Invalid point."));
                setPromptPrefix(tr("Specify first point of circle: "));
            }
            else {
                global.x1 = Number(strList[0]);
                global.y1 = Number(strList[1]);
                setPromptPrefix(tr("Specify second point of circle: "));
            }
        }
        else if (std::isnan(global.x2)) {
            EmbReal strList = str.split(",");
            if (std::isnan(strList[0]) || std::isnan(strList[1])) {
                alert(tr("Invalid point."));
                setPromptPrefix(tr("Specify second point of circle: "));
            }
            else {
                global.x2 = Number(strList[0]);
                global.y2 = Number(strList[1]);
                addRubber("CIRCLE");
                setRubberMode("CIRCLE_3P");
                setRubberPoint("CIRCLE_TAN1", global.x1, global.y1);
                setRubberPoint("CIRCLE_TAN2", global.x2, global.y2);
                setPromptPrefix(tr("Specify third point of circle: "));
            }
        }
        else if (std::isnan(global.x3)) {
            EmbReal strList = str.split(",");
            if (std::isnan(strList[0]) || std::isnan(strList[1])) {
                alert(tr("Invalid point."));
                setPromptPrefix(tr("Specify third point of circle: "));
            }
            else {
                global.x3 = Number(strList[0]);
                global.y3 = Number(strList[1]);
                setRubberPoint("CIRCLE_TAN3", global.x3, global.y3);
                actuator("vulcanize");
                actuator("end");
            }
        }
        else {
            error("CIRCLE", tr("This should never happen."));
        }
    }
    else if (global["mode"].s == MODE_TTR) {
        todo("CIRCLE", "prompt() for TTR");
    }
    */
    return "";
}

/**
 * .
 */
Dictionary
distance_main(void)
{
    Dictionary global;
    /*
    initCommand();
    clearSelection();
    setPromptPrefix(tr("Specify first point: "));
    */
    return global;
}

/**
 * .
 */
void
distance_click(Dictionary global, EmbVector v)
{
    /*
    if (std::isnan(global.x1)) {
        global.x1 = x;
        global.y1 = y;
        addRubber("LINE");
        setRubberMode("LINE");
        setRubberPoint("LINE_START", global.x1, global.y1);
        appendPromptHistory();
        setPromptPrefix(tr("Specify second point: "));
    }
    else {
        appendPromptHistory();
        global.x2 = x;
        global.y2 = y;
        reportDistance();
        actuator("end");
    }
    */
}

/**
 * @brief distance_context
 * @param args
 * @return
 */
String
distance_context(String args)
{
    //todo("DISTANCE", "context()");
    return "";
}

/**
 * @brief distance_prompt
 * @param args
 * @return
 */
String
distance_prompt(String args)
{
    /*
    EmbReal strList = str.split(",");
    if (std::isnan(global.x1)) {
        if (std::isnan(strList[0]) || std::isnan(strList[1])) {
            alert(tr("Requires numeric distance or two points."));
            setPromptPrefix(tr("Specify first point: "));
        }
        else {
            global.x1 = Number(strList[0]);
            global.y1 = Number(strList[1]);
            addRubber("LINE");
            setRubberMode("LINE");
            setRubberPoint("LINE_START", global.x1, global.y1);
            setPromptPrefix(tr("Specify second point: "));
        }
    }
    else {
        if (std::isnan(strList[0]) || std::isnan(strList[1])) {
            alert(tr("Requires numeric distance or two points."));
            setPromptPrefix(tr("Specify second point: "));
        }
        else {
            global.x2 = Number(strList[0]);
            global.y2 = Number(strList[1]);
            reportDistance();
            actuator("end");
        }
    }
    */
    return "";
}

/**
 * Cartesian Coordinate System reported:
 *
 *               (+)
 *               90
 *               |
 *      (-) 180__|__0 (+)
 *               |
 *              270
 *              (-)
 */
void
reportDistance()
{
    EmbVector delta;
    /*
    EmbReal dx = global.x2 - global.x1;
    EmbReal dy = global.y2 - global.y1;

    EmbReal dist = calculateDistance(global.x1, global.y1, global.x2, global.y2);
    EmbReal angle = calculateAngle(global.x1, global.y1, global.x2, global.y2);

    setPromptPrefix(tr("Distance") + " = " + dist.toString() + ", " + tr("Angle") + " = " + angle.toString());
    appendPromptHistory();
    setPromptPrefix(tr("Delta X") + " = " + dx.toString() + ", " + tr("Delta Y") + " = " + dy.toString());
    appendPromptHistory();
    */
}

/**
 *
 */
String
dolphin_main(String args)
{
    /*
    Dictionary global;
    global.numPoints = 512; //Default //TODO: min:64 max:8192
    global->center.x;
    global->center.y;
    global.sx = 0.04; //Default
    global.sy = 0.04; //Default
    global.numPoints;
    global.mode;

    initCommand();
    clearSelection();
    global->center.x = NaN;
    global->center.y = NaN;
    global.mode = MODE_NUM_POINTS;

    addRubber("POLYGON");
    setRubberMode("POLYGON");
    updateDolphin(global.numPoints, global.sx, global.sy);
    spareRubber("POLYGON");
    actuator("end");
    */
    return "";
}

/**
 * @brief updateDolphin
 * @param numPoints
 * @param xScale
 * @param yScale
 */
void
updateDolphin(int numPoints, EmbReal xScale, EmbReal yScale)
{
    /*
    for (int i = 0; i <= numPoints; i++) {
        EmbReal t = (2.0 * emb_constant_pi) / numPoints*i;
        EmbVector v;
        v.x = fourier_series(t, dolphin_x);
        v.y = fourier_series(t, dolphin_y);

        setRubberPoint("POLYGON_POINT_" + i.toString(), xx*xScale, yy*yScale);
    }

    setRubberText("POLYGON_NUM_POINTS", numPoints.toString());
    */
}

/**
 * @brief ellipse_main
 * @return
 */
Dictionary
ellipse_main(void)
{
    Dictionary global;
    global["mode"] = node("MODE_MAJORDIAMETER_MINORRADIUS");
    /*
    global.width;
    global.height;
    global.rot;
    global.mode;
    initCommand();
    clearSelection();
    setPromptPrefix(tr("Specify first axis start point or [Center]: "));
    */
    return global;
}

/**
 * .
 */
/*
void
ellipse_click(x, y)
{
    if (global["mode"].s == MODE_MAJORDIAMETER_MINORRADIUS) {
        if (std::isnan(global.x1)) {
            global.x1 = x;
            global.y1 = y;
            addRubber("ELLIPSE");
            setRubberMode("ELLIPSE_LINE");
            setRubberPoint("ELLIPSE_LINE_POINT1", global.x1, global.y1);
            appendPromptHistory();
            setPromptPrefix(tr("Specify first axis end point: "));
        }
        else if (std::isnan(global.x2)) {
            global.x2 = x;
            global.y2 = y;
            global->center.x = (global.x1 + global.x2)/2.0;
            global->center.y = (global.y1 + global.y2)/2.0;
            global.width = calculateDistance(global.x1, global.y1, global.x2, global.y2);
            global.rot = calculateAngle(global.x1, global.y1, global.x2, global.y2);
            setRubberMode("ELLIPSE_MAJORDIAMETER_MINORRADIUS");
            setRubberPoint("ELLIPSE_AXIS1_POINT1", global.x1, global.y1);
            setRubberPoint("ELLIPSE_AXIS1_POINT2", global.x2, global.y2);
            setRubberPoint("ELLIPSE_CENTER", global->center.x, global->center.y);
            setRubberPoint("ELLIPSE_WIDTH", global.width, 0);
            setRubberPoint("ELLIPSE_ROT", global.rot, 0);
            appendPromptHistory();
            setPromptPrefix(tr("Specify second axis end point or [Rotation]: "));
        }
        else if (std::isnan(global.x3)) {
            global.x3 = x;
            global.y3 = y;
            global.height = perpendicularDistance(global.x3, global.y3, global.x1, global.y1, global.x2, global.y2)*2.0;
            setRubberPoint("ELLIPSE_AXIS2_POINT2", global.x3, global.y3);
            actuator("vulcanize");
            appendPromptHistory();
            actuator("end");
        }
        else {
            error("ELLIPSE", tr("This should never happen."));
        }
    }
    else if (global["mode"].s == MODE_MAJORRADIUS_MINORRADIUS) {
        if (std::isnan(global.x1)) {
            global.x1 = x;
            global.y1 = y;
            global->center.x = global.x1;
            global->center.y = global.y1;
            addRubber("ELLIPSE");
            setRubberMode("ELLIPSE_LINE");
            setRubberPoint("ELLIPSE_LINE_POINT1", global.x1, global.y1);
            setRubberPoint("ELLIPSE_CENTER", global->center.x, global->center.y);
            appendPromptHistory();
            setPromptPrefix(tr("Specify first axis end point: "));
        }
        else if (std::isnan(global.x2)) {
            global.x2 = x;
            global.y2 = y;
            global.width = calculateDistance(global->center.x, global->center.y, global.x2, global.y2)*2.0;
            global.rot = calculateAngle(global.x1, global.y1, global.x2, global.y2);
            setRubberMode("ELLIPSE_MAJORRADIUS_MINORRADIUS");
            setRubberPoint("ELLIPSE_AXIS1_POINT2", global.x2, global.y2);
            setRubberPoint("ELLIPSE_WIDTH", global.width, 0);
            setRubberPoint("ELLIPSE_ROT", global.rot, 0);
            appendPromptHistory();
            setPromptPrefix(tr("Specify second axis end point or [Rotation]: "));
        }
        else if (std::isnan(global.x3)) {
            global.x3 = x;
            global.y3 = y;
            global.height = perpendicularDistance(global.x3, global.y3, global->center.x, global->center.y, global.x2, global.y2)*2.0;
            setRubberPoint("ELLIPSE_AXIS2_POINT2", global.x3, global.y3);
            actuator("vulcanize");
            appendPromptHistory();
            actuator("end");
        }
        else {
            error("ELLIPSE", tr("This should never happen."));
        }
    }
    else if (global["mode"].s == MODE_ELLIPSE_ROTATION) {
        if (std::isnan(global.x1)) {
            error("ELLIPSE", tr("This should never happen."));
        }
        else if (std::isnan(global.x2)) {
            error("ELLIPSE", tr("This should never happen."));
        }
        else if (std::isnan(global.x3)) {
            EmbReal angle = calculateAngle(global->center.x, global->center.y, x, y);
            global.height = cos(angle*PI/180.0)*global.width;
            addEllipse(global->center.x, global->center.y, global.width, global.height, global.rot, false);
            appendPromptHistory();
            actuator("end");
        }
    }
}
*/

/**
 * .
 */
void
ellipse_context(String args)
{
    /*
    todo("ELLIPSE", "context()");
    */
}

/**
 * .
 */
/*
void
ellipse_prompt(String args)
{
    if (global["mode"].s == MODE_MAJORDIAMETER_MINORRADIUS) {
        if (std::isnan(global.x1)) {
            if (str == "C" || str == "CENTER") {
                global.mode = MODE_MAJORRADIUS_MINORRADIUS;
                setPromptPrefix(tr("Specify center point: "));
            }
            else {
                EmbReal strList = str.split(",");
                if (std::isnan(strList[0]) || std::isnan(strList[1])) {
                    alert(tr("Point or option keyword required."));
                    setPromptPrefix(tr("Specify first axis start point or [Center]: "));
                }
                else {
                    global.x1 = Number(strList[0]);
                    global.y1 = Number(strList[1]);
                    addRubber("ELLIPSE");
                    setRubberMode("ELLIPSE_LINE");
                    setRubberPoint("ELLIPSE_LINE_POINT1", global.x1, global.y1);
                    setPromptPrefix(tr("Specify first axis end point: "));
                }
            }
        }
        else if (std::isnan(global.x2)) {
            EmbReal strList = str.split(",");
            if (std::isnan(strList[0]) || std::isnan(strList[1])) {
                alert(tr("Invalid point."));
                setPromptPrefix(tr("Specify first axis end point: "));
            }
            else {
                global.x2 = Number(strList[0]);
                global.y2 = Number(strList[1]);
                global->center.x = (global.x1 + global.x2)/2.0;
                global->center.y = (global.y1 + global.y2)/2.0;
                global.width = calculateDistance(global.x1, global.y1, global.x2, global.y2);
                global.rot = calculateAngle(global.x1, global.y1, global.x2, global.y2);
                setRubberMode("ELLIPSE_MAJORDIAMETER_MINORRADIUS");
                setRubberPoint("ELLIPSE_AXIS1_POINT1", global.x1, global.y1);
                setRubberPoint("ELLIPSE_AXIS1_POINT2", global.x2, global.y2);
                setRubberPoint("ELLIPSE_CENTER", global->center.x, global->center.y);
                setRubberPoint("ELLIPSE_WIDTH", global.width, 0);
                setRubberPoint("ELLIPSE_ROT", global.rot, 0);
                setPromptPrefix(tr("Specify second axis end point or [Rotation]: "));
            }
        }
        else if (std::isnan(global.x3)) {
            if (str == "R" || str == "ROTATION") {
                global.mode = MODE_ELLIPSE_ROTATION;
                setPromptPrefix(tr("Specify rotation: "));
            }
            else {
                EmbReal strList = str.split(",");
                if (std::isnan(strList[0]) || std::isnan(strList[1])) {
                    alert(tr("Point or option keyword required."));
                    setPromptPrefix(tr("Specify second axis end point or [Rotation]: "));
                }
                else {
                    global.x3 = Number(strList[0]);
                    global.y3 = Number(strList[1]);
                    global.height = perpendicularDistance(global.x3, global.y3, global.x1, global.y1, global.x2, global.y2)*2.0;
                    setRubberPoint("ELLIPSE_AXIS2_POINT2", global.x3, global.y3);
                    actuator("vulcanize");
                    actuator("end");
                }
            }
        }
    }
    else if (global["mode"].s == MODE_MAJORRADIUS_MINORRADIUS) {
        if (std::isnan(global.x1)) {
            EmbReal strList = str.split(",");
            if (std::isnan(strList[0]) || std::isnan(strList[1])) {
                alert(tr("Invalid point."));
                setPromptPrefix(tr("Specify center point: "));
            }
            else {
                global.x1 = Number(strList[0]);
                global.y1 = Number(strList[1]);
                global->center.x = global.x1;
                global->center.y = global.y1;
                addRubber("ELLIPSE");
                setRubberMode("ELLIPSE_LINE");
                setRubberPoint("ELLIPSE_LINE_POINT1", global.x1, global.y1);
                setRubberPoint("ELLIPSE_CENTER", global->center.x, global->center.y);
                setPromptPrefix(tr("Specify first axis end point: "));
            }
        }
        else if (std::isnan(global.x2)) {
            EmbReal strList = str.split(",");
            if (std::isnan(strList[0]) || std::isnan(strList[1])) {
                alert(tr("Invalid point."));
                setPromptPrefix(tr("Specify first axis end point: "));
            }
            else {
                global.x2 = Number(strList[0]);
                global.y2 = Number(strList[1]);
                global.width = calculateDistance(global.x1, global.y1, global.x2, global.y2)*2.0;
                global.rot = calculateAngle(global.x1, global.y1, global.x2, global.y2);
                setRubberMode("ELLIPSE_MAJORRADIUS_MINORRADIUS");
                setRubberPoint("ELLIPSE_AXIS1_POINT2", global.x2, global.y2);
                setRubberPoint("ELLIPSE_WIDTH", global.width, 0);
                setRubberPoint("ELLIPSE_ROT", global.rot, 0);
                setPromptPrefix(tr("Specify second axis end point or [Rotation]: "));
            }
        }
        else if (std::isnan(global.x3)) {
            if (str == "R" || str == "ROTATION") {
                global.mode = MODE_ELLIPSE_ROTATION;
                setPromptPrefix(tr("Specify ellipse rotation: "));
            }
            else {
                EmbReal strList = str.split(",");
                if (std::isnan(strList[0]) || std::isnan(strList[1])) {
                    alert(tr("Point or option keyword required."));
                    setPromptPrefix(tr("Specify second axis end point or [Rotation]: "));
                }
                else {
                    global.x3 = Number(strList[0]);
                    global.y3 = Number(strList[1]);
                    global.height = perpendicularDistance(global.x3, global.y3, global.x1, global.y1, global.x2, global.y2)*2.0;
                    setRubberPoint("ELLIPSE_AXIS2_POINT2", global.x3, global.y3);
                    actuator("vulcanize");
                    actuator("end");
                }
            }
        }
    }
    else if (global["mode"].s == MODE_ELLIPSE_ROTATION) {
        if (std::isnan(global.x1)) {
            error("ELLIPSE", tr("This should never happen."));
        }
        else if (std::isnan(global.x2)) {
            error("ELLIPSE", tr("This should never happen."));
        }
        else if (std::isnan(global.x3)) {
            if (std::isnan(str)) {
                alert(tr("Invalid angle. Input a numeric angle or pick a point."));
                setPromptPrefix(tr("Specify rotation: "));
            }
            else {
                EmbReal angle = Number(str);
                global.height = cos(angle*PI/180.0)*global.width;
                addEllipse(global->center.x, global->center.y, global.width, global.height, global.rot, false);
                actuator("end");
            }
        }
    }
}

void
erase_main(void)
{
    initCommand();

    if (numSelected() <= 0) {
        //TODO: Prompt to select objects if nothing is preselected
        alert(tr("Preselect objects before invoking the delete command."));
        actuator("end");
        messageBox("information", tr("Delete Preselect"), tr("Preselect objects before invoking the delete command."));
    }
    else {
        deleteSelected();
        actuator("end");
    }
}


void
heart_main(void)
{
    Dictionary global;
    global.numPoints = 512; //Default //TODO: min:64 max:8192
    global->center.x;
    global->center.y;
    global.sx = 1.0;
    global.sy = 1.0;
    global.numPoints;
    global.mode;
    initCommand();
    clearSelection();
    global->center.x = NaN;
    global->center.y = NaN;
    global.mode = MODE_NUM_POINTS;

    //Heart4: 10.0 / 512
    //Heart5: 1.0 / 512

    addRubber("POLYGON");
    setRubberMode("POLYGON");
    updateHeart("HEART5", global.numPoints, global.sx, global.sy);
    spareRubber("POLYGON");
    actuator("end");
}
*/

/**
 * .
 */
/*
void
updateHeart(style, numPoints, xScale, yScale)
{
    for (int i = 0; i <= numPoints; i++) {
        EmbReal xx, yy;
        EmbReal t = (2.0*emb_constant_pi)/numPoints*i;

        if (style == "HEART4") {
            xx = cos(t)*((sin(t)*sqrt(abs(cos(t))))/(sin(t)+7/5) - 2*sin(t) + 2);
            yy = sin(t)*((sin(t)*sqrt(abs(cos(t))))/(sin(t)+7/5) - 2*sin(t) + 2);
        }
        else if (style == "HEART5") {
            xx = 16*pow(sin(t), 3);
            yy = 13*cos(t) - 5*cos(2*t) - 2*cos(3*t) - cos(4*t);
        }

        setRubberPoint("POLYGON_POINT_" + i.toString(), xx*xScale, yy*yScale);
    }

    setRubberText("POLYGON_NUM_POINTS", numPoints.toString());
}

void
line_main(void)
{
    Dictionary global;
    initCommand();
    clearSelection();
    global.firstRun = true;
    global.first = {NaN, NaN};
    global.prev = {NaN, NaN};
    setPromptPrefix(tr("Specify first point: "));
}


void
line_click(x, y)
{
    if (global.firstRun) {
        global.firstRun = false;
        global.firstX = x;
        global.firstY = y;
        global.prevX = x;
        global.prevY = y;
        addRubber("LINE");
        setRubberMode("LINE");
        setRubberPoint("LINE_START", global.firstX, global.firstY);
        appendPromptHistory();
        setPromptPrefix(tr("Specify next point or [Undo]: "));
    }
    else {
        setRubberPoint("LINE_END", x, y);
        actuator("vulcanize");
        addRubber("LINE");
        setRubberMode("LINE");
        setRubberPoint("LINE_START", x, y);
        appendPromptHistory();
        global.prevX = x;
        global.prevY = y;
    }
}

void
line_context(String str)
{
    todo("LINE", "context()");
}

void
line_prompt(String args)
{
    if (global.firstRun) {
        EmbReal strList = str.split(",");
        if (std::isnan(strList[0]) || std::isnan(strList[1])) {
            alert(tr("Invalid point."));
            setPromptPrefix(tr("Specify first point: "));
        }
        else {
            global.firstRun = false;
            global.firstX = Number(strList[0]);
            global.firstY = Number(strList[1]);
            global.prevX = global.firstX;
            global.prevY = global.firstY;
            addRubber("LINE");
            setRubberMode("LINE");
            setRubberPoint("LINE_START", global.firstX, global.firstY);
            setPromptPrefix(tr("Specify next point or [Undo]: "));
        }
    }
    else {
        if (str == "U" || str == "UNDO") {
            todo("LINE", "prompt() for UNDO");
        }
        else {
            EmbReal strList = str.split(",");
            if (std::isnan(strList[0]) || std::isnan(strList[1])) {
                alert(tr("Point or option keyword required."));
                setPromptPrefix(tr("Specify next point or [Undo]: "));
            }
            else {
                EmbReal x = Number(strList[0]);
                EmbReal y = Number(strList[1]);
                setRubberPoint("LINE_END", x, y);
                actuator("vulcanize");
                addRubber("LINE");
                setRubberMode("LINE");
                setRubberPoint("LINE_START", x, y);
                global.prevX = x;
                global.prevY = y;
                setPromptPrefix(tr("Specify next point or [Undo]: "));
            }
        }
    }
}
*/

/**
 * @brief locate_point_main
 * @return
 */
Dictionary
locate_point_main(void)
{
    Dictionary global;
    /*
    initCommand();
    clearSelection();
    setPromptPrefix(tr("Specify point: "));
    */
    return global;
}

/**
 * @brief locate_point_click
 * @param global
 * @param v
 * @return
 */
Dictionary
locate_point_click(Dictionary global, EmbVector v)
{
    /*
    appendPromptHistory();
    setPromptPrefix("X = " + v.x + ", Y = " + v.y);
    appendPromptHistory();
    actuator("end");
    */
    return global;
}

void
locate_point_context(String str)
{
    //todo("LOCATEPOINT", "context()");
}

Dictionary
locate_point_prompt(Dictionary global, String args)
{
    StringList strList = tokenize(args, ',');
    /*
    if (std::isnan(strList[0]) || std::isnan(strList[1])) {
        alert(tr("Invalid point."));
        setPromptPrefix(tr("Specify point: "));
    }
    else {
        actuator("append-prompt");
        actuator("set-prompt-prefix X = " + strList[0] + ", Y = " + strList[1]);
        actuator("append-prompt");
        actuator("end");
    }
    */
    return global;
}

/**
 * .
 */
/*
void
move_main(void)
{
    Dictionary global;
    global.firstRun;
    global.baseX;
    global.baseY;
    global.destX;
    global.destY;
    global.deltaX;
    global.deltaY;
    initCommand();
    global.firstRun = true;
    global.baseX  = NaN;
    global.baseY  = NaN;
    global.destX  = NaN;
    global.destY  = NaN;
    global.deltaX = NaN;
    global.deltaY = NaN;

    if (numSelected() <= 0) {
        //TODO: Prompt to select objects if nothing is preselected
        alert(tr("Preselect objects before invoking the move command."));
        actuator("end");
        messageBox("information", tr("Move Preselect"), tr("Preselect objects before invoking the move command."));
    }
    else {
        setPromptPrefix(tr("Specify base point: "));
    }
}
*/

/**
 * .
 */
/*
void
move_click(x, y)
{
    if (global.firstRun) {
        global.firstRun = false;
        global.baseX = x;
        global.baseY = y;
        addRubber("LINE");
        setRubberMode("LINE");
        setRubberPoint("LINE_START", global.baseX, global.baseY);
        previewOn("SELECTED", "MOVE", global.baseX, global.baseY, 0);
        appendPromptHistory();
        setPromptPrefix(tr("Specify destination point: "));
    }
    else {
        global.destX = x;
        global.destY = y;
        global.deltaX = global.destX - global.baseX;
        global.deltaY = global.destY - global.baseY;
        moveSelected(global.deltaX, global.deltaY);
        previewOff();
        actuator("end");
    }
}
*/

/**
 * .
 */
void
move_context(String str)
{
    // todo("MOVE", "context()");
}

/**
 * .
 */
void
move_prompt(String str)
{
    /*
    if (global.firstRun) {
        EmbReal strList = str.split(",");
        if (std::isnan(strList[0]) || std::isnan(strList[1])) {
            alert(tr("Invalid point."));
            setPromptPrefix(tr("Specify base point: "));
        }
        else {
            global.firstRun = false;
            global.baseX = Number(strList[0]);
            global.baseY = Number(strList[1]);
            addRubber("LINE");
            setRubberMode("LINE");
            setRubberPoint("LINE_START", global.baseX, global.baseY);
            previewOn("SELECTED", "MOVE", global.baseX, global.baseY, 0);
            setPromptPrefix(tr("Specify destination point: "));
        }
    }
    else {
        EmbReal strList = str.split(",");
        if (std::isnan(strList[0]) || std::isnan(strList[1])) {
            alert(tr("Invalid point."));
            setPromptPrefix(tr("Specify destination point: "));
        }
        else {
            global.destX = Number(strList[0]);
            global.destY = Number(strList[1]);
            global.deltaX = global.destX - global.baseX;
            global.deltaY = global.destY - global.baseY;
            moveSelected(global.deltaX, global.deltaY);
            previewOff();
            actuator("end");
        }
    }
    */
}

/*
//TODO: The path command is currently broken

void
path_main(void)
{
    Dictionary global;
    global.firstRun;
    global.first;
    global.prev;
    initCommand();
    clearSelection();
    global["firstRun"] = node(true);
    setPromptPrefix(tr("Specify start point: "));
    return global;
}


void
path_click(x, y)
{
    if (global.firstRun) {
        global.firstRun = false;
        global.firstX = x;
        global.firstY = y;
        global.prevX = x;
        global.prevY = y;
        addPath(x,y);
        appendPromptHistory();
        setPromptPrefix(tr("Specify next point or [Arc/Undo]: "));
    }
    else {
        appendPromptHistory();
        appendLineToPath(x,y);
        global.prevX = x;
        global.prevY = y;
    }
}

void
path_context(String str)
{
    todo("PATH", "context()");
}

void
path_prompt(String args)
{
    if (str == "A" || str == "ARC") {
        todo("PATH", "prompt() for ARC");
    }
    else if (str == "U" || str == "UNDO") {
        todo("PATH", "prompt() for UNDO");
    }
    else {
        EmbReal strList = str.split(",");
        if (std::isnan(strList[0]) || std::isnan(strList[1])) {
            alert(tr("Point or option keyword required."));
            setPromptPrefix(tr("Specify next point or [Arc/Undo]: "));
        }
        else {
            EmbReal x = Number(strList[0]);
            EmbReal y = Number(strList[1]);
            if (global.firstRun) {
                global.firstRun = false;
                global.firstX = x;
                global.firstY = y;
                global.prevX = x;
                global.prevY = y;
                addPath(x,y);
                setPromptPrefix(tr("Specify next point or [Arc/Undo]: "));
            }
            else {
                appendLineToPath(x,y);
                global.prevX = x;
                global.prevY = y;
            }
        }
    }
}

Dictionary
point_main(void)
{
    Dictionary global;
    initCommand();
    clearSelection();
    global.firstRun = true;
    setPromptPrefix("TODO: Current point settings: PDMODE=?  PDSIZE=?"); //TODO: tr needed here when complete
    appendPromptHistory();
    setPromptPrefix(tr("Specify first point: "));
    return global;
}


void
point_click(Dictionary global, EmbVector v)
{
    if (global.firstRun) {
        global.firstRun = false;
        appendPromptHistory();
        setPromptPrefix(tr("Specify next point: "));
        addPoint(x,y);
    }
    else {
        appendPromptHistory();
        addPoint(x,y);
    }
}

void
point_context(String str)
{
    todo("POINT", "context()");
}

void
point_prompt(String str)
{
    if (global.firstRun) {
        if (str == "M" || str == "MODE") {
            todo("POINT", "prompt() for PDMODE");
        }
        else if (str == "S" || str == "SIZE") {
            todo("POINT", "prompt() for PDSIZE");
        }
        EmbReal strList = str.split(",");
        if (std::isnan(strList[0]) || std::isnan(strList[1])) {
            alert(tr("Invalid point."));
            setPromptPrefix(tr("Specify first point: "));
        }
        else {
            global.firstRun = false;
            EmbReal x = Number(strList[0]);
            EmbReal y = Number(strList[1]);
            setPromptPrefix(tr("Specify next point: "));
            addPoint(x,y);
        }
    }
    else {
        EmbReal strList = str.split(",");
        if (std::isnan(strList[0]) || std::isnan(strList[1])) {
            alert(tr("Invalid point."));
            setPromptPrefix(tr("Specify next point: "));
        }
        else {
            EmbReal x = Number(strList[0]);
            EmbReal y = Number(strList[1]);
            setPromptPrefix(tr("Specify next point: "));
            addPoint(x,y);
        }
    }
}

//Command: Polygon

Dictionary global;
global->center;
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
*/

/**
 * .
 */
/*
void
polygon_main(void)
{
    initCommand();
    clearSelection();
    global->centerX = NaN;
    global->centerY = NaN;
    global.sideX1  = NaN;
    global.sideY1  = NaN;
    global.sideX2  = NaN;
    global.sideY2  = NaN;
    global.pointIX = NaN;
    global.pointIY = NaN;
    global.pointCX = NaN;
    global.pointCY = NaN;
    global.mode = MODE_NUM_SIDES;
    setPromptPrefix(tr("Enter number of sides") + " {" + global.numSides.toString() + "}: ");
}
*/

/**
 * .
 */
void
polygon_click(Dictionary global, EmbVector v)
{
    /*
    if (global["mode"].s == MODE_NUM_SIDES) {
        //Do nothing, the prompt controls this.
    }
    else if (global["mode"].s == MODE_CENTER_PT) {
        global->centerX = x;
        global->centerY = y;
        global.mode = MODE_POLYTYPE;
        appendPromptHistory();
        setPromptPrefix(tr("Specify polygon type [Inscribed in circle/Circumscribed around circle]") + " {" + global.polyType + "}: ");
    }
    else if (global["mode"].s == MODE_POLYTYPE) {
        //Do nothing, the prompt controls this.
    }
    else if (global["mode"].s == MODE_INSCRIBE) {
        global.pointIX = x;
        global.pointIY = y;
        setRubberPoint("POLYGON_INSCRIBE_POINT", global.pointIX, global.pointIY);
        actuator("vulcanize");
        appendPromptHistory();
        actuator("end");
    }
    else if (global["mode"].s == MODE_CIRCUMSCRIBE) {
        global.pointCX = x;
        global.pointCY = y;
        setRubberPoint("POLYGON_CIRCUMSCRIBE_POINT", global.pointCX, global.pointCY);
        actuator("vulcanize");
        appendPromptHistory();
        actuator("end");
    }
    else if (global["mode"].s == MODE_DISTANCE) {
        //Do nothing, the prompt controls this.
    }
    else if (global["mode"].s == MODE_SIDE_LEN) {
        todo("POLYGON", "Sidelength mode");
    }
    */
}

/**
 * .
 */
void
polygon_context(String str)
{
    //todo("POLYGON", "context()");
}

/**
 * .
 */
void
polygon_prompt(String str)
{
    /*
    if (global["mode"].s == MODE_NUM_SIDES) {
        if (str == "" && global.numSides >= 3 && global.numSides <= 1024) {
            setPromptPrefix(tr("Specify center point or [Sidelength]: "));
            global.mode = MODE_CENTER_PT;
        }
        else {
            EmbReal tmp = Number(str);
            if (std::isnan(tmp) || !isInt(tmp) || tmp < 3 || tmp > 1024) {
                alert(tr("Requires an integer between 3 and 1024."));
                setPromptPrefix(tr("Enter number of sides") + " {" + global.numSides.toString() + "}: ");
            }
            else {
                global.numSides = tmp;
                setPromptPrefix(tr("Specify center point or [Sidelength]: "));
                global.mode = MODE_CENTER_PT;
            }
        }
    }
    else if (global["mode"].s == MODE_CENTER_PT) {
        if (str == "S" || str == "SIDELENGTH") {
            global.mode = MODE_SIDE_LEN;
            setPromptPrefix(tr("Specify start point: "));
        }
        else {
            EmbReal strList = str.split(",");
            if (std::isnan(strList[0]) || std::isnan(strList[1])) {
                alert(tr("Point or option keyword required."));
                setPromptPrefix(tr("Specify center point or [Sidelength]: "));
            }
            else {
                global->centerX = Number(strList[0]);
                global->centerY = Number(strList[1]);
                global.mode = MODE_POLYTYPE;
                setPromptPrefix(tr("Specify polygon type [Inscribed in circle/Circumscribed around circle]") + " {" + global.polyType + "}: ");
            }
        }
    }
    else if (global["mode"].s == MODE_POLYTYPE) {
        if (str == "INSCRIBED") {
            global.mode = MODE_INSCRIBE;
            global.polyType = "Inscribed";
            setPromptPrefix(tr("Specify polygon corner point or [Distance]: "));
            addRubber("POLYGON");
            setRubberMode("POLYGON_INSCRIBE");
            setRubberPoint("POLYGON_CENTER", global->centerX, global->centerY);
            setRubberPoint("POLYGON_NUM_SIDES", global.numSides, 0);
        }
        else if (str == "CIRCUMSCRIBED") {
            global.mode = MODE_CIRCUMSCRIBE;
            global.polyType = "Circumscribed";
            setPromptPrefix(tr("Specify polygon side point or [Distance]: "));
            addRubber("POLYGON");
            setRubberMode("POLYGON_CIRCUMSCRIBE");
            setRubberPoint("POLYGON_CENTER", global->centerX, global->centerY);
            setRubberPoint("POLYGON_NUM_SIDES", global.numSides, 0);
        }
        else if (str == "") {
            if (global.polyType == "Inscribed") {
                global.mode = MODE_INSCRIBE;
                setPromptPrefix(tr("Specify polygon corner point or [Distance]: "));
                addRubber("POLYGON");
                setRubberMode("POLYGON_INSCRIBE");
                setRubberPoint("POLYGON_CENTER", global->centerX, global->centerY);
                setRubberPoint("POLYGON_NUM_SIDES", global.numSides, 0);
            }
            else if (global.polyType == "Circumscribed") {
                global.mode = MODE_CIRCUMSCRIBE;
                setPromptPrefix(tr("Specify polygon side point or [Distance]: "));
                addRubber("POLYGON");
                setRubberMode("POLYGON_CIRCUMSCRIBE");
                setRubberPoint("POLYGON_CENTER", global->centerX, global->centerY);
                setRubberPoint("POLYGON_NUM_SIDES", global.numSides, 0);
            }
            else {
                error("POLYGON", tr("Polygon type is not Inscribed or Circumscribed."));
            }
        }
        else {
            alert(tr("Invalid option keyword."));
            setPromptPrefix(tr("Specify polygon type [Inscribed in circle/Circumscribed around circle]") + " {" + global.polyType + "}: ");
        }
    }
    else if (global["mode"].s == MODE_INSCRIBE) {
        if (str == "D" || str == "DISTANCE") {
            global.mode = MODE_DISTANCE;
            setPromptPrefix(tr("Specify distance: "));
        }
        else {
            EmbReal strList = str.split(",");
            if (std::isnan(strList[0]) || std::isnan(strList[1])) {
                alert(tr("Point or option keyword required."));
                setPromptPrefix(tr("Specify polygon corner point or [Distance]: "));
            }
            else {
                global.pointIX = Number(strList[0]);
                global.pointIY = Number(strList[1]);
                setRubberPoint("POLYGON_INSCRIBE_POINT", global.pointIX, global.pointIY);
                actuator("vulcanize");
                actuator("end");
            }
        }
    }
    else if (global["mode"].s == MODE_CIRCUMSCRIBE) {
        if (str == "D" || str == "DISTANCE") {
            global.mode = MODE_DISTANCE;
            setPromptPrefix(tr("Specify distance: "));
        }
        else {
            EmbReal strList = str.split(",");
            if (std::isnan(strList[0]) || std::isnan(strList[1])) {
                alert(tr("Point or option keyword required."));
                setPromptPrefix(tr("Specify polygon side point or [Distance]: "));
            }
            else {
                global.pointCX = Number(strList[0]);
                global.pointCY = Number(strList[1]);
                setRubberPoint("POLYGON_CIRCUMSCRIBE_POINT", global.pointCX, global.pointCY);
                actuator("vulcanize");
                actuator("end");
            }
        }
    }
    else if (global["mode"].s == MODE_DISTANCE) {
        if (std::isnan(str)) {
            alert(tr("Requires valid numeric distance."));
            setPromptPrefix(tr("Specify distance: "));
        }
        else {
            if (global.polyType == "Inscribed") {
                global.pointIX = global->centerX;
                global.pointIY = global->centerY + Number(str);
                setRubberPoint("POLYGON_INSCRIBE_POINT", global.pointIX, global.pointIY);
                actuator("vulcanize");
                actuator("end");
            }
            else if (global.polyType == "Circumscribed") {
                global.pointCX = global->centerX;
                global.pointCY = global->centerY + Number(str);
                setRubberPoint("POLYGON_CIRCUMSCRIBE_POINT", global.pointCX, global.pointCY);
                actuator("vulcanize");
                actuator("end");
            }
            else {
                error("POLYGON", tr("Polygon type is not Inscribed or Circumscribed."));
            }
        }
    }
    else if (global["mode"].s == MODE_SIDE_LEN) {
        todo("POLYGON", "Sidelength mode");
    }
    */
}

//Command: Polyline
/*
Dictionary global;
global.firstRun;
global.firstX;
global.firstY;
global.prevX;
global.prevY;
global.num;
*/

/**
 * .
 */
void
polyline_main(void)
{
    /*
    initCommand();
    clearSelection();
    global.firstRun = true;
    global.firstX = NaN;
    global.firstY = NaN;
    global.prevX = NaN;
    global.prevY = NaN;
    global.num = 0;
    setPromptPrefix(tr("Specify first point: "));
    */
}

/**
 * .
 */
void
polyline_click(Dictionary global, EmbVector v)
{
    /*
    if (global.firstRun) {
        global.firstRun = false;
        global.firstX = x;
        global.firstY = y;
        global.prevX = x;
        global.prevY = y;
        addRubber("POLYLINE");
        setRubberMode("POLYLINE");
        setRubberPoint("POLYLINE_POINT_0", global.firstX, global.firstY);
        appendPromptHistory();
        setPromptPrefix(tr("Specify next point or [Undo]: "));
    }
    else {
        global.num++;
        setRubberPoint("POLYLINE_POINT_" + global.num.toString(), x, y);
        setRubberText("POLYLINE_NUM_POINTS", global.num.toString());
        spareRubber("POLYLINE");
        appendPromptHistory();
        global.prevX = x;
        global.prevY = y;
    }
    */
}

/**
 * .
 */
void
polyline_context(String str)
{
    //todo("POLYLINE", "context()");
}

/**
 * .
 */
void
polyline_prompt(String str)
{
    /*
    if (global.firstRun) {
        EmbReal strList = str.split(",");
        if (std::isnan(strList[0]) || std::isnan(strList[1])) {
            alert(tr("Invalid point."));
            setPromptPrefix(tr("Specify first point: "));
        }
        else {
            global.firstRun = false;
            global.firstX = Number(strList[0]);
            global.firstY = Number(strList[1]);
            global.prevX = global.firstX;
            global.prevY = global.firstY;
            addRubber("POLYLINE");
            setRubberMode("POLYLINE");
            setRubberPoint("POLYLINE_POINT_0", global.firstX, global.firstY);
            setPromptPrefix(tr("Specify next point or [Undo]: "));
        }
    }
    else {
        if (str == "U" || str == "UNDO") {
            todo("POLYLINE", "prompt() for UNDO");
        }
        else {
            EmbReal strList = str.split(",");
            if (std::isnan(strList[0]) || std::isnan(strList[1])) {
                alert(tr("Point or option keyword required."));
                setPromptPrefix(tr("Specify next point or [Undo]: "));
            }
            else {
                EmbReal x = Number(strList[0]);
                EmbReal y = Number(strList[1]);
                global.num++;
                setRubberPoint("POLYLINE_POINT_" + global.num.toString(), x, y);
                setRubberText("POLYLINE_NUM_POINTS", global.num.toString());
                spareRubber("POLYLINE");
                global.prevX = x;
                global.prevY = y;
                setPromptPrefix(tr("Specify next point or [Undo]: "));
            }
        }
    }
    */
}

//Command: QuickLeader

/*
Dictionary global;
global.x1;
global.y1;
global.x2;
global.y2;
*/

//TODO: Adding the text is not complete yet.

/**
 * .
 */
void
quickleader_main(void)
{
    /*
    initCommand();
    clearSelection();
    global.x1 = NaN;
    global.y1 = NaN;
    global.x2 = NaN;
    global.y2 = NaN;
    setPromptPrefix(tr("Specify first point: "));
    */
}

/**
 * .
 */
void
quickleader_click(Dictionary global, EmbVector v)
{
    /*
    if (std::isnan(global.x1)) {
        global.x1 = x;
        global.y1 = y;
        addRubber("DIMLEADER");
        setRubberMode("DIMLEADER_LINE");
        setRubberPoint("DIMLEADER_LINE_START", global.x1, global.y1);
        appendPromptHistory();
        setPromptPrefix(tr("Specify second point: "));
    }
    else {
        global.x2 = x;
        global.y2 = y;
        setRubberPoint("DIMLEADER_LINE_END", global.x2, global.y2);
        actuator("vulcanize");
        appendPromptHistory();
        actuator("end");
    }
    */
}

/**
 * .
 */
void
quickleader_context(String str)
{
    //todo("QUICKLEADER", "context()");
}

/**
 * .
 */
void
quickleader_prompt(String str)
{
    /*
    EmbReal strList = str.split(",");
    if (std::isnan(global.x1)) {
        if (std::isnan(strList[0]) || std::isnan(strList[1])) {
            alert(tr("Requires two points."));
            setPromptPrefix(tr("Specify first point: "));
        }
        else {
            global.x1 = Number(strList[0]);
            global.y1 = Number(strList[1]);
            addRubber("DIMLEADER");
            setRubberMode("DIMLEADER_LINE");
            setRubberPoint("DIMLEADER_LINE_START", global.x1, global.y1);
            setPromptPrefix(tr("Specify second point: "));
        }
    }
    else {
        if (std::isnan(strList[0]) || std::isnan(strList[1])) {
            alert(tr("Requires two points."));
            setPromptPrefix(tr("Specify second point: "));
        }
        else {
            global.x2 = Number(strList[0]);
            global.y2 = Number(strList[1]);
            setRubberPoint("DIMLEADER_LINE_END", global.x2, global.y2);
            actuator("vulcanize");
            actuator("end");
        }
    }
    */
}

//Command: Rectangle

/*
Dictionary global;
global.newRect;
global.x1;
global.y1;
global.x2;
global.y2;
*/

/**
 * .
 */
void
rectangle_main(void)
{
    /*
    initCommand();
    clearSelection();
    global.newRect = true;
    global.x1 = NaN;
    global.y1 = NaN;
    global.x2 = NaN;
    global.y2 = NaN;
    setPromptPrefix(tr("Specify first corner point or [Chamfer/Fillet]: "));
    */
}

/**
 * .
 */
void
rectangle_click(Dictionary global, EmbVector v)
{
    /*
    if (global.newRect) {
        global.newRect = false;
        global.x1 = x;
        global.y1 = y;
        addRubber("RECTANGLE");
        setRubberMode("RECTANGLE");
        setRubberPoint("RECTANGLE_START", x, y);
        setPromptPrefix(tr("Specify other corner point or [Dimensions]: "));
    }
    else {
        global.newRect = true;
        global.x2 = x;
        global.y2 = y;
        setRubberPoint("RECTANGLE_END", x, y);
        actuator("vulcanize");
        actuator("end");
    }
    */
}

/**
 * .
 */
void
rectangle_context(String str)
{
    //todo("RECTANGLE", "context()");
}

/**
 * .
 */
void
rectangle_prompt(String str)
{
    /*
    if (str == "C" || str == "CHAMFER") {
        todo("RECTANGLE", "prompt() for CHAMFER");
    }
    else if (str == "D" || str == "DIMENSIONS") {
        todo("RECTANGLE", "prompt() for DIMENSIONS");
    }
    else if (str == "F" || str == "FILLET") {
        todo("RECTANGLE", "prompt() for FILLET");
    }
    else {
        EmbReal strList = str.split(",");
        if (std::isnan(strList[0]) || std::isnan(strList[1])) {
            alert(tr("Invalid point."));
            setPromptPrefix(tr("Specify first point: "));
        }
        else {
            EmbReal x = Number(strList[0]);
            EmbReal y = Number(strList[1]);
            if (global.newRect) {
                global.newRect = false;
                global.x1 = x;
                global.y1 = y;
                addRubber("RECTANGLE");
                setRubberMode("RECTANGLE");
                setRubberPoint("RECTANGLE_START", x, y);
                setPromptPrefix(tr("Specify other corner point or [Dimensions]: "));
            }
            else {
                global.newRect = true;
                global.x2 = x;
                global.y2 = y;
                setRubberPoint("RECTANGLE_END", x, y);
                actuator("vulcanize");
                actuator("end");
            }
        }
    }
    */
}

//Command: RGB

//Dictionary global;
//global.mode;

/**
 * .
 */
void
rgb_main(void)
{
    /*
    initCommand();
    clearSelection();
    global.mode = "RGB_MODE_BACKGROUND";
    setPromptPrefix(tr("Enter RED,GREEN,BLUE values for background or [Crosshair/Grid]: "));
    */
}

/**
 * .
 */
void
rgb_click(Dictionary global, EmbVector v)
{
    //Do Nothing, prompt only command.
}

/**
 * .
 */
void
rgb_context(String str)
{
    //todo("RGB", "context()");
}

/**
 * .
 */
void
rgb_prompt(String str)
{
    /*
    if (global["mode"].s == "RGB_MODE_BACKGROUND") {
        if (str == "C" || str == "CROSSHAIR") {
            global["mode"] = node("RGB_MODE_CROSSHAIR");
            setPromptPrefix(tr("Specify crosshair color: "));
        }
        else if (str == "G" || str == "GRID") {
            global.mode = RGB_MODE_GRID;
            setPromptPrefix(tr("Specify grid color: "));
        }
        else {
            EmbReal strList = str.split(",");
            EmbReal r = Number(strList[0]);
            EmbReal g = Number(strList[1]);
            EmbReal b = Number(strList[2]);
            if (!validRGB(r,g,b)) {
                alert(tr("Invalid color. R,G,B values must be in the range of 0-255."));
                setPromptPrefix(tr("Specify background color: "));
            }
            else {
                setBackgroundColor(r,g,b);
                actuator("end");
            }
        }
    }
    else if (global["mode"].s == RGB_MODE_CROSSHAIR) {
        EmbReal strList = str.split(",");
        EmbReal r = Number(strList[0]);
        EmbReal g = Number(strList[1]);
        EmbReal b = Number(strList[2]);
        if (!validRGB(r,g,b)) {
            alert(tr("Invalid color. R,G,B values must be in the range of 0-255."));
            setPromptPrefix(tr("Specify crosshair color: "));
        }
        else {
            setCrossHairColor(r,g,b);
            actuator("end");
        }
    }
    else if (global["mode"].s == RGB_MODE_GRID) {
        EmbReal strList = str.split(",");
        EmbReal r = Number(strList[0]);
        EmbReal g = Number(strList[1]);
        EmbReal b = Number(strList[2]);
        if (!validRGB(r,g,b)) {
            alert(tr("Invalid color. R,G,B values must be in the range of 0-255."));
            setPromptPrefix(tr("Specify grid color: "));
        }
        else {
            setGridColor(r,g,b);
            actuator("end");
        }
    }
    */
}

/**
 * .
 * Command: Rotate
 *
 * Dictionary global;
 * bool firstRun;
 * EmbVector base;
 * EmbVector dest;
 * EmbReal angle;
 *
 * EmbVector baseR;
 * EmbVector destR;
 * EmbReal angleRef;
 * EmbReal angleNew;
 *
 * int mode;
 */
String
rotate_main(String args)
{
    /*
    initCommand();
    global.mode = ROTATE_MODE_NORMAL;
    global.firstRun = true;
    global.base = {NaN, NaN};
    global.dest = {NaN, NaN};
    global.angle = NaN;

    global.baseR = {NaN, NaN};
    global.destR = {NaN, NaN};
    global.angleRef = NaN;
    global.angleNew = NaN;

    if (numSelected() <= 0) {
        //TODO: Prompt to select objects if nothing is preselected
        alert(tr("Preselect objects before invoking the rotate command."));
        actuator("end");
        messageBox("information", tr("Rotate Preselect"), tr("Preselect objects before invoking the rotate command."));
    }
    else {
        setPromptPrefix(tr("Specify base point: "));
    }
    */
    return "";
}

/**
 * .
 */
void
rotate_click(Dictionary global, EmbVector v)
{
    /*
    if (global["mode"].s == ROTATE_MODE_NORMAL) {
        if (global.firstRun) {
            global.firstRun = false;
            global.base = v;
            addRubber("LINE");
            setRubberMode("LINE");
            setRubberPoint("LINE_START", global.baseX, global.baseY);
            previewOn("SELECTED", "ROTATE", global.baseX, global.baseY, 0);
            appendPromptHistory();
            setPromptPrefix(tr("Specify rotation angle or [Reference]: "));
        }
        else {
            global.dest = v;
            global.angle = calculateAngle(global.baseX, global.baseY, global.destX, global.destY);
            appendPromptHistory();
            rotateSelected(global.baseX, global.baseY, global.angle);
            previewOff();
            actuator("end");
        }
    }
    else if (global["mode"].s == ROTATE_MODE_REFERENCE) {
        if (std::isnan(global.baseRX)) {
            global.baseR = v;
            appendPromptHistory();
            addRubber("LINE");
            setRubberMode("LINE");
            setRubberPoint("LINE_START", global.baseRX, global.baseRY);
            setPromptPrefix(tr("Specify second point: "));
        }
        else if (std::isnan(global.destRX)) {
            global.destR = v;
            global.angleRef = calculateAngle(global.baseRX, global.baseRY, global.destRX, global.destRY);
            setRubberPoint("LINE_START", global.baseX, global.baseY);
            previewOn("SELECTED", "ROTATE", global.baseX, global.baseY, global.angleRef);
            appendPromptHistory();
            setPromptPrefix(tr("Specify the new angle: "));
        }
        else if (std::isnan(global.angleNew)) {
            global.angleNew = calculateAngle(global.baseX, global.baseY, x, y);
            rotateSelected(global.baseX, global.baseY, global.angleNew - global.angleRef);
            previewOff();
            actuator("end");
        }
    }
    */
}

/**
 * .
 */
void
rotate_context(String str)
{
    //todo("ROTATE", "context()");
}

/**
 * .
 */
void
rotate_prompt(String str)
{
    /*
    if (global["mode"].s == ROTATE_MODE_NORMAL) {
        if (global.firstRun) {
            EmbReal strList = str.split(",");
            if (std::isnan(strList[0]) || std::isnan(strList[1])) {
                alert(tr("Invalid point."));
                setPromptPrefix(tr("Specify base point: "));
            }
            else {
                global.firstRun = false;
                global.baseX = Number(strList[0]);
                global.baseY = Number(strList[1]);
                addRubber("LINE");
                setRubberMode("LINE");
                setRubberPoint("LINE_START", global.baseX, global.baseY);
                previewOn("SELECTED", "ROTATE", global.baseX, global.baseY, 0);
                setPromptPrefix(tr("Specify rotation angle or [Reference]: "));
            }
        }
        else {
            if (str == "R" || str == "REFERENCE") {
                global.mode = MODE_REFERENCE;
                setPromptPrefix(tr("Specify the reference angle") + " {0.00}: ");
                clearRubber();
                previewOff();
            }
            else {
                if (std::isnan(str)) {
                    alert(tr("Requires valid numeric angle, second point, or option keyword."));
                    setPromptPrefix(tr("Specify rotation angle or [Reference]: "));
                }
                else {
                    global.angle = Number(str);
                    rotateSelected(global.baseX, global.baseY, global.angle);
                    previewOff();
                    actuator("end");
                }
            }
        }
    }
    else if (global["mode"].s == MODE_REFERENCE) {
        if (std::isnan(global.baseRX)) {
            if (std::isnan(str)) {
                EmbReal strList = str.split(",");
                if (std::isnan(strList[0]) || std::isnan(strList[1])) {
                    alert(tr("Requires valid numeric angle or two points."));
                    setPromptPrefix(tr("Specify the reference angle") + " {0.00}: ");
                }
                else {
                    global.baseRX = Number(strList[0]);
                    global.baseRY = Number(strList[1]);
                    addRubber("LINE");
                    setRubberMode("LINE");
                    setRubberPoint("LINE_START", global.baseRX, global.baseRY);
                    setPromptPrefix(tr("Specify second point: "));
                }
            }
            else {
                //The base and dest values are only set here to advance the command.
                global.baseRX = 0.0;
                global.baseRY = 0.0;
                global.destRX = 0.0;
                global.destRY = 0.0;
                //The reference angle is what we will use later.
                global.angleRef = Number(str);
                addRubber("LINE");
                setRubberMode("LINE");
                setRubberPoint("LINE_START", global.baseX, global.baseY);
                previewOn("SELECTED", "ROTATE", global.baseX, global.baseY, global.angleRef);
                setPromptPrefix(tr("Specify the new angle: "));
            }
        }
        else if (std::isnan(global.destRX)) {
            if (std::isnan(str)) {
                EmbReal strList = str.split(",");
                if (std::isnan(strList[0]) || std::isnan(strList[1])) {
                    alert(tr("Requires valid numeric angle or two points."));
                    setPromptPrefix(tr("Specify second point: "));
                }
                else {
                    global.destRX = Number(strList[0]);
                    global.destRY = Number(strList[1]);
                    global.angleRef = calculateAngle(global.baseRX, global.baseRY, global.destRX, global.destRY);
                    previewOn("SELECTED", "ROTATE", global.baseX, global.baseY, global.angleRef);
                    setRubberPoint("LINE_START", global.baseX, global.baseY);
                    setPromptPrefix(tr("Specify the new angle: "));
                }
            }
            else {
                //The base and dest values are only set here to advance the command.
                global.baseRX = 0.0;
                global.baseRY = 0.0;
                global.destRX = 0.0;
                global.destRY = 0.0;
                //The reference angle is what we will use later.
                global.angleRef = Number(str);
                previewOn("SELECTED", "ROTATE", global.baseX, global.baseY, global.angleRef);
                setPromptPrefix(tr("Specify the new angle: "));
            }
        }
        else if (std::isnan(global.angleNew)) {
            if (std::isnan(str)) {
                EmbReal strList = str.split(",");
                if (std::isnan(strList[0]) || std::isnan(strList[1])) {
                    alert(tr("Requires valid numeric angle or second point."));
                    setPromptPrefix(tr("Specify the new angle: "));
                }
                else {
                    EmbReal x = Number(strList[0]);
                    EmbReal y = Number(strList[1]);
                    global.angleNew = calculateAngle(global.baseX, global.baseY, x, y);
                    rotateSelected(global.baseX, global.baseY, global.angleNew - global.angleRef);
                    previewOff();
                    actuator("end");
                }
            }
            else {
                global.angleNew = Number(str);
                rotateSelected(global.baseX, global.baseY, global.angleNew - global.angleRef);
                previewOff();
                actuator("end");
            }
        }
    }
    */
}

//Command: Sandbox

/*Dictionary global;
global.test1;
global.test2;
*/

/**
 * .
 */
String
sandbox_main(String str)
{
    /*
    initCommand();

    //Report number of pre-selected objects
    setPromptPrefix("Number of Objects Selected: " + numSelected().toString());
    appendPromptHistory();

    mirrorSelected(0,0,0,1);

    //selectAll();
    //rotateSelected(0,0,90);

    //Polyline & Polygon Testing

    EmbReal offsetX = 0.0;
    EmbReal offsetY = 0.0;

    EmbReal polylineArray = [];
    polylineArray.push(1.0 + offsetX);
    polylineArray.push(1.0 + offsetY);
    polylineArray.push(1.0 + offsetX);
    polylineArray.push(2.0 + offsetY);
    polylineArray.push(2.0 + offsetX);
    polylineArray.push(2.0 + offsetY);
    polylineArray.push(2.0 + offsetX);
    polylineArray.push(3.0 + offsetY);
    polylineArray.push(3.0 + offsetX);
    polylineArray.push(3.0 + offsetY);
    polylineArray.push(3.0 + offsetX);
    polylineArray.push(2.0 + offsetY);
    polylineArray.push(4.0 + offsetX);
    polylineArray.push(2.0 + offsetY);
    polylineArray.push(4.0 + offsetX);
    polylineArray.push(1.0 + offsetY);
    addPolyline(polylineArray);

    offsetX = 5.0;
    offsetY = 0.0;

    EmbReal polygonArray = [];
    polygonArray.push(1.0 + offsetX);
    polygonArray.push(1.0 + offsetY);
    polygonArray.push(1.0 + offsetX);
    polygonArray.push(2.0 + offsetY);
    polygonArray.push(2.0 + offsetX);
    polygonArray.push(2.0 + offsetY);
    polygonArray.push(2.0 + offsetX);
    polygonArray.push(3.0 + offsetY);
    polygonArray.push(3.0 + offsetX);
    polygonArray.push(3.0 + offsetY);
    polygonArray.push(3.0 + offsetX);
    polygonArray.push(2.0 + offsetY);
    polygonArray.push(4.0 + offsetX);
    polygonArray.push(2.0 + offsetY);
    polygonArray.push(4.0 + offsetX);
    polygonArray.push(1.0 + offsetY);
    addPolygon(polygonArray);

    actuator("end");
    */
    return "";
}

/**
 * .
 * Command: Scale
 *
 * Dictionary global;
 * global.firstRun;
 * global.baseX;
 * global.baseY;
 * global.destX;
 * global.destY;
 * global.factor;
 *
 * global.baseRX;
 * global.baseRY;
 * global.destRX;
 * global.destRY;
 * global.factorRef;
 * global.factorNew;
 *
 * global.mode;
 */
void
scale_main(void)
{
    /*
    initCommand();
    global.mode = MODE_NORMAL;
    global.firstRun = true;
    global.baseX  = NaN;
    global.baseY  = NaN;
    global.destX  = NaN;
    global.destY  = NaN;
    global.factor = NaN;

    global.baseRX    = NaN;
    global.baseRY    = NaN;
    global.destRX    = NaN;
    global.destRY    = NaN;
    global.factorRef = NaN;
    global.factorNew = NaN;

    if (numSelected() <= 0) {
        //TODO: Prompt to select objects if nothing is preselected
        alert(tr("Preselect objects before invoking the scale command."));
        actuator("end");
        messageBox("information", tr("Scale Preselect"), tr("Preselect objects before invoking the scale command."));
    }
    else {
        setPromptPrefix(tr("Specify base point: "));
    }
    */
}

/**
 * .
 */
void
scale_click(Dictionary global, EmbVector v)
{
    /*
    if (global["mode"].s == MODE_NORMAL) {
        if (global.firstRun) {
            global.firstRun = false;
            global.base = v;
            addRubber("LINE");
            setRubberMode("LINE");
            setRubberPoint("LINE_START", global.baseX, global.baseY);
            previewOn("SELECTED", "SCALE", global.baseX, global.baseY, 1);
            appendPromptHistory();
            setPromptPrefix(tr("Specify scale factor or [Reference]: "));
        }
        else {
            global.dest = v;
            global.factor = calculateDistance(global.base, global.dest);
            appendPromptHistory();
            scaleSelected(global.baseX, global.baseY, global.factor);
            previewOff();
            actuator("end");
        }
    }
    else if (global["mode"].s == MODE_REFERENCE) {
        if (std::isnan(global.baseRX)) {
            global.baseR = v;
            appendPromptHistory();
            addRubber("LINE");
            setRubberMode("LINE");
            setRubberPoint("LINE_START", global.baseR);
            setPromptPrefix(tr("Specify second point: "));
        }
        else if (std::isnan(global.destRX)) {
            global.destR = v;
            global.factorRef = calculateDistance(global.baseRX, global.baseRY, global.destRX, global.destRY);
            if (global.factorRef <= 0.0) {
                global.destRX    = NaN;
                global.destRY    = NaN;
                global.factorRef = NaN;
                alert(tr("Value must be positive and nonzero."));
                setPromptPrefix(tr("Specify second point: "));
            }
            else {
                appendPromptHistory();
                setRubberPoint("LINE_START", global.baseX, global.baseY);
                previewOn("SELECTED", "SCALE", global.baseX, global.baseY, global.factorRef);
                setPromptPrefix(tr("Specify new length: "));
            }
        }
        else if (std::isnan(global.factorNew)) {
            global.factorNew = calculateDistance(global.baseX, global.baseY, x, y);
            if (global.factorNew <= 0.0) {
                global.factorNew = NaN;
                alert(tr("Value must be positive and nonzero."));
                setPromptPrefix(tr("Specify new length: "));
            }
            else {
                appendPromptHistory();
                scaleSelected(global.baseX, global.baseY, global.factorNew/global.factorRef);
                previewOff();
                actuator("end");
            }
        }
    }
    */
}

/**
 * .
 */
void
scale_context(String str)
{
    //todo("SCALE", "context()");
}

/**
 * .
 */
void
scale_prompt(String str)
{
    /*
    if (global["mode"].s == MODE_NORMAL) {
        if (global.firstRun) {
            EmbReal strList = str.split(",");
            if (std::isnan(strList[0]) || std::isnan(strList[1])) {
                alert(tr("Invalid point."));
                setPromptPrefix(tr("Specify base point: "));
            }
            else {
                global.firstRun = false;
                global.baseX = Number(strList[0]);
                global.baseY = Number(strList[1]);
                addRubber("LINE");
                setRubberMode("LINE");
                setRubberPoint("LINE_START", global.baseX, global.baseY);
                previewOn("SELECTED", "SCALE", global.baseX, global.baseY, 1);
                setPromptPrefix(tr("Specify scale factor or [Reference]: "));
            }
        }
        else {
            if (str == "R" || str == "REFERENCE") {
                global.mode = MODE_REFERENCE;
                setPromptPrefix(tr("Specify reference length") + " {1}: ");
                clearRubber();
                previewOff();
            }
            else {
                if (std::isnan(str)) {
                    alert(tr("Requires valid numeric distance, second point, or option keyword."));
                    setPromptPrefix(tr("Specify scale factor or [Reference]: "));
                }
                else {
                    global.factor = Number(str);
                    scaleSelected(global.baseX, global.baseY, global.factor);
                    previewOff();
                    actuator("end");
                }
            }
        }
    }
    else if (global["mode"].s == MODE_REFERENCE) {
        if (std::isnan(global.baseRX)) {
            if (std::isnan(str)) {
                EmbReal strList = str.split(",");
                if (std::isnan(strList[0]) || std::isnan(strList[1])) {
                    alert(tr("Requires valid numeric distance or two points."));
                    setPromptPrefix(tr("Specify reference length") + " {1}: ");
                }
                else {
                    global.baseRX = Number(strList[0]);
                    global.baseRY = Number(strList[1]);
                    addRubber("LINE");
                    setRubberMode("LINE");
                    setRubberPoint("LINE_START", global.baseRX, global.baseRY);
                    setPromptPrefix(tr("Specify second point: "));
                }
            }
            else {
                //The base and dest values are only set here to advance the command.
                global.baseRX = 0.0;
                global.baseRY = 0.0;
                global.destRX = 0.0;
                global.destRY = 0.0;
                //The reference length is what we will use later.
                global.factorRef = Number(str);
                if (global.factorRef <= 0.0) {
                    global.baseRX    = NaN;
                    global.baseRY    = NaN;
                    global.destRX    = NaN;
                    global.destRY    = NaN;
                    global.factorRef = NaN;
                    alert(tr("Value must be positive and nonzero."));
                    setPromptPrefix(tr("Specify reference length") + " {1}: ");
                }
                else {
                    addRubber("LINE");
                    setRubberMode("LINE");
                    setRubberPoint("LINE_START", global.baseX, global.baseY);
                    previewOn("SELECTED", "SCALE", global.baseX, global.baseY, global.factorRef);
                    setPromptPrefix(tr("Specify new length: "));
                }
            }
        }
        else if (std::isnan(global.destRX)) {
            if (std::isnan(str)) {
                EmbReal strList = str.split(",");
                if (std::isnan(strList[0]) || std::isnan(strList[1])) {
                    alert(tr("Requires valid numeric distance or two points."));
                    setPromptPrefix(tr("Specify second point: "));
                }
                else {
                    global.destRX = Number(strList[0]);
                    global.destRY = Number(strList[1]);
                    global.factorRef = calculateDistance(global.baseRX, global.baseRY, global.destRX, global.destRY);
                    if (global.factorRef <= 0.0) {
                        global.destRX    = NaN;
                        global.destRY    = NaN;
                        global.factorRef = NaN;
                        alert(tr("Value must be positive and nonzero."));
                        setPromptPrefix(tr("Specify second point: "));
                    }
                    else {
                        setRubberPoint("LINE_START", global.baseX, global.baseY);
                        previewOn("SELECTED", "SCALE", global.baseX, global.baseY, global.factorRef);
                        setPromptPrefix(tr("Specify new length: "));
                    }
                }
            }
            else {
                //The base and dest values are only set here to advance the command.
                global.baseRX = 0.0;
                global.baseRY = 0.0;
                global.destRX = 0.0;
                global.destRY = 0.0;
                //The reference length is what we will use later.
                global.factorRef = Number(str);
                if (global.factorRef <= 0.0) {
                    global.destRX    = NaN;
                    global.destRY    = NaN;
                    global.factorRef = NaN;
                    alert(tr("Value must be positive and nonzero."));
                    setPromptPrefix(tr("Specify second point: "));
                }
                else {
                    setRubberPoint("LINE_START", global.baseX, global.baseY);
                    previewOn("SELECTED", "SCALE", global.baseX, global.baseY, global.factorRef);
                    setPromptPrefix(tr("Specify new length: "));
                }
            }
        }
        else if (std::isnan(global.factorNew)) {
            if (std::isnan(str)) {
                EmbReal strList = str.split(",");
                if (std::isnan(strList[0]) || std::isnan(strList[1])) {
                    alert(tr("Requires valid numeric distance or second point."));
                    setPromptPrefix(tr("Specify new length: "));
                }
                else {
                    EmbReal x = Number(strList[0]);
                    EmbReal y = Number(strList[1]);
                    global.factorNew = calculateDistance(global.baseX, global.baseY, x, y);
                    if (global.factorNew <= 0.0) {
                        global.factorNew = NaN;
                        alert(tr("Value must be positive and nonzero."));
                        setPromptPrefix(tr("Specify new length: "));
                    }
                    else {
                        scaleSelected(global.baseX, global.baseY, global.factorNew/global.factorRef);
                        previewOff();
                        actuator("end");
                    }
                }
            }
            else {
                global.factorNew = Number(str);
                if (global.factorNew <= 0.0) {
                    global.factorNew = NaN;
                    alert(tr("Value must be positive and nonzero."));
                    setPromptPrefix(tr("Specify new length: "));
                }
                else {
                    scaleSelected(global.baseX, global.baseY, global.factorNew/global.factorRef);
                    previewOff();
                    actuator("end");
                }
            }
        }
    }
    */
}


//Command: Single Line Text
/*
Dictionary global;
global.text;
global.textX;
global.textY;
global.textJustify;
global.textFont;
global.textHeight;
global.textRotation;
global.mode;
*/

/**
 * .
 */
void
text_single_main(void)
{
    /*
    initCommand();
    clearSelection();
    global.text = "";
    global.textX = NaN;
    global.textY = NaN;
    global.textJustify = "Left";
    global.textFont = textFont();
    global.textHeight = NaN;
    global.textRotation = NaN;
    global.mode = MODE_SETGEOM;
    setPromptPrefix(tr("Current font: ") + "{" + global.textFont + "} " + tr("Text height: ") + "{" +  textSize() + "}");
    appendPromptHistory();
    setPromptPrefix(tr("Specify start point of text or [Justify/Setfont]: "));
    */
}

/**
 * .
 */
void
text_single_click(Dictionary global, EmbVector v)
{
    /*
    if (global["mode"].s == MODE_SETGEOM) {
        if (std::isnan(global.textX)) {
            global.textX = x;
            global.textY = y;
            addRubber("LINE");
            setRubberMode("LINE");
            setRubberPoint("LINE_START", global.textX, global.textY);
            appendPromptHistory();
            setPromptPrefix(tr("Specify text height") + " {" + textSize() + "}: ");
        }
        else if (std::isnan(global.textHeight)) {
            global.textHeight = calculateDistance(global.textX, global.textY, x, y);
            setTextSize(global.textHeight);
            appendPromptHistory();
            setPromptPrefix(tr("Specify text angle") + " {" + textAngle() + "}: ");
        }
        else if (std::isnan(global.textRotation)) {
            global.textRotation = calculateAngle(global.textX, global.textY, x, y);
            setTextAngle(global.textRotation);
            appendPromptHistory();
            setPromptPrefix(tr("Enter text: "));
            global.mode = MODE_RAPID;
            prompt->enableRapidFire();
            clearRubber();
            addRubber("TEXTSINGLE");
            setRubberMode("TEXTSINGLE");
            setRubberPoint("TEXT_POINT", global.textX, global.textY);
            setRubberPoint("TEXT_HEIGHT_ROTATION", global.textHeight, global.textRotation);
            setRubberText("TEXT_FONT", global.textFont);
            setRubberText("TEXT_JUSTIFY", global.textJustify);
            setRubberText("TEXT_RAPID", global.text);
        }
        else {
            //Do nothing, as we are in rapidFire mode now.
        }
    }
    */
}

/**
 * .
 */
void
text_single_context(String str)
{
    //todo("SINGLELINETEXT", "context()");
}

/**
 * .
 */
void
text_single_prompt(String str)
{
    /*
    if (global["mode"].s == MODE_JUSTIFY) {
        if (str == "C" || str == "CENTER") {
            global.mode = MODE_SETGEOM;
            global.textJustify = "Center";
            setRubberText("TEXT_JUSTIFY", global.textJustify);
            setPromptPrefix(tr("Specify center point of text or [Justify/Setfont]: "));
        }
        else if (str == "R" || str == "RIGHT") {
            global.mode = MODE_SETGEOM;
            global.textJustify = "Right";
            setRubberText("TEXT_JUSTIFY", global.textJustify);
            setPromptPrefix(tr("Specify right-end point of text or [Justify/Setfont]: "));
        }
        else if (str == "A" || str == "ALIGN") {
            global.mode = MODE_SETGEOM;
            global.textJustify = "Aligned";
            setRubberText("TEXT_JUSTIFY", global.textJustify);
            setPromptPrefix(tr("Specify start point of text or [Justify/Setfont]: "));
        }
        else if (str == "M" || str == "MIDDLE") {
            global.mode = MODE_SETGEOM;
            global.textJustify = "Middle";
            setRubberText("TEXT_JUSTIFY", global.textJustify);
            setPromptPrefix(tr("Specify middle point of text or [Justify/Setfont]: "));
        }
        else if (str == "F" || str == "FIT") {
            global.mode = MODE_SETGEOM;
            global.textJustify = "Fit";
            setRubberText("TEXT_JUSTIFY", global.textJustify);
            setPromptPrefix(tr("Specify start point of text or [Justify/Setfont]: "));
        }
        else if (str == "TL" || str == "TOPLEFT") {
            global.mode = MODE_SETGEOM;
            global.textJustify = "Top Left";
            setRubberText("TEXT_JUSTIFY", global.textJustify);
            setPromptPrefix(tr("Specify top-left point of text or [Justify/Setfont]: "));
        }
        else if (str == "TC" || str == "TOPCENTER") {
            global.mode = MODE_SETGEOM;
            global.textJustify = "Top Center";
            setRubberText("TEXT_JUSTIFY", global.textJustify);
            setPromptPrefix(tr("Specify top-center point of text or [Justify/Setfont]: "));
        }
        else if (str == "TR" || str == "TOPRIGHT") {
            global.mode = MODE_SETGEOM;
            global.textJustify = "Top Right";
            setRubberText("TEXT_JUSTIFY", global.textJustify);
            setPromptPrefix(tr("Specify top-right point of text or [Justify/Setfont]: "));
        }
        else if (str == "ML" || str == "MIDDLELEFT") {
            global.mode = MODE_SETGEOM;
            global.textJustify = "Middle Left";
            setRubberText("TEXT_JUSTIFY", global.textJustify);
            setPromptPrefix(tr("Specify middle-left point of text or [Justify/Setfont]: "));
        }
        else if (str == "MC" || str == "MIDDLECENTER") {
            global.mode = MODE_SETGEOM;
            global.textJustify = "Middle Center";
            setRubberText("TEXT_JUSTIFY", global.textJustify);
            setPromptPrefix(tr("Specify middle-center point of text or [Justify/Setfont]: "));
        }
        else if (str == "MR" || str == "MIDDLERIGHT") {
            global.mode = MODE_SETGEOM;
            global.textJustify = "Middle Right";
            setRubberText("TEXT_JUSTIFY", global.textJustify);
            setPromptPrefix(tr("Specify middle-right point of text or [Justify/Setfont]: "));
        }
        else if (str == "BL" || str == "BOTTOMLEFT") {
            global.mode = MODE_SETGEOM;
            global.textJustify = "Bottom Left";
            setRubberText("TEXT_JUSTIFY", global.textJustify);
            setPromptPrefix(tr("Specify bottom-left point of text or [Justify/Setfont]: "));
        }
        else if (str == "BC" || str == "BOTTOMCENTER") {
            global.mode = MODE_SETGEOM;
            global.textJustify = "Bottom Center";
            setRubberText("TEXT_JUSTIFY", global.textJustify);
            setPromptPrefix(tr("Specify bottom-center point of text or [Justify/Setfont]: "));
        }
        else if (str == "BR" || str == "BOTTOMRIGHT") {
            global.mode = MODE_SETGEOM;
            global.textJustify = "Bottom Right";
            setRubberText("TEXT_JUSTIFY", global.textJustify);
            setPromptPrefix(tr("Specify bottom-right point of text or [Justify/Setfont]: "));
        }
        else {
            alert(tr("Invalid option keyword."));
            setPromptPrefix(tr("Text Justification Options [Center/Right/Align/Middle/Fit/TL/TC/TR/ML/MC/MR/BL/BC/BR]: "));
        }
    }
    else if (global["mode"].s == MODE_SETFONT) {
        global.mode = MODE_SETGEOM;
        global.textFont = str;
        setRubberText("TEXT_FONT", global.textFont);
        setTextFont(global.textFont);
        setPromptPrefix(tr("Specify start point of text or [Justify/Setfont]: "));
    }
    else if (global["mode"].s == MODE_SETGEOM) {
        if (std::isnan(global.textX)) {
            if (str == "J" || str == "JUSTIFY") {
                global.mode = MODE_JUSTIFY;
                setPromptPrefix(tr("Text Justification Options [Center/Right/Align/Middle/Fit/TL/TC/TR/ML/MC/MR/BL/BC/BR]: "));
            }
            else if (str == "S" || str == "SETFONT") {
                global.mode = MODE_SETFONT;
                setPromptPrefix(tr("Specify font name: "));
            }
            else {
                EmbReal strList = str.split(",");
                if (std::isnan(strList[0]) || std::isnan(strList[1])) {
                    alert(tr("Point or option keyword required."));
                    setPromptPrefix(tr("Specify start point of text or [Justify/Setfont]: "));
                }
                else {
                    global.textX = Number(strList[0]);
                    global.textY = Number(strList[1]);
                    addRubber("LINE");
                    setRubberMode("LINE");
                    setRubberPoint("LINE_START", global.textX, global.textY);
                    setPromptPrefix(tr("Specify text height") + " {" + textSize() + "}: ");
                }
            }
        }
        else if (std::isnan(global.textHeight)) {
            if (str == "") {
                global.textHeight = textSize();
                setPromptPrefix(tr("Specify text angle") + " {" + textAngle() + "}: ");
            }
            else if (std::isnan(str)) {
                alert(tr("Requires valid numeric distance or second point."));
                setPromptPrefix(tr("Specify text height") + " {" + textSize() + "}: ");
            }
            else {
                global.textHeight = Number(str);
                setTextSize(global.textHeight);
                setPromptPrefix(tr("Specify text angle") + " {" + textAngle() + "}: ");
            }
        }
        else if (std::isnan(global.textRotation)) {
            if (str == "") {
                global.textRotation = textAngle();
                setPromptPrefix(tr("Enter text: "));
                global.mode = MODE_RAPID;
                prompt->enableRapidFire();
                clearRubber();
                addRubber("TEXTSINGLE");
                setRubberMode("TEXTSINGLE");
                setRubberPoint("TEXT_POINT", global.textX, global.textY);
                setRubberPoint("TEXT_HEIGHT_ROTATION", global.textHeight, global.textRotation);
                setRubberText("TEXT_FONT", global.textFont);
                setRubberText("TEXT_JUSTIFY", global.textJustify);
                setRubberText("TEXT_RAPID", global.text);
            }
            else if (std::isnan(str)) {
                alert(tr("Requires valid numeric angle or second point."));
                setPromptPrefix(tr("Specify text angle") + " {" + textAngle() + "}: ");
            }
            else {
                global.textRotation = Number(str);
                setTextAngle(global.textRotation);
                setPromptPrefix(tr("Enter text: "));
                global.mode = MODE_RAPID;
                prompt->enableRapidFire();
                clearRubber();
                addRubber("TEXTSINGLE");
                setRubberMode("TEXTSINGLE");
                setRubberPoint("TEXT_POINT", global.textX, global.textY);
                setRubberPoint("TEXT_HEIGHT_ROTATION", global.textHeight, global.textRotation);
                setRubberText("TEXT_FONT", global.textFont);
                setRubberText("TEXT_JUSTIFY", global.textJustify);
                setRubberText("TEXT_RAPID", global.text);
            }
        }
        else {
            //Do nothing, as we are in rapidFire mode now.
        }
    }
    else if (global["mode"].s == MODE_RAPID) {
        if (str == "RAPID_ENTER") {
            if (global.text == "") {
                actuator("end");
            }
            else {
                actuator("vulcanize");
                actuator("end");
                //TODO: Rather than ending the command, calculate where the next line would be and modify the x/y to the new point
            }
        }
        else {
            global.text = str;
            setRubberText("TEXT_RAPID", global.text);
        }
    }
    */
}

Dictionary snowflake_main(void);
Dictionary updateSnowflake(Dictionary global, EmbVector scale);

Dictionary star_main(void);
Dictionary star_move(Dictionary global, EmbVector v);
Dictionary star_prompt(Dictionary global);
Dictionary star_context(Dictionary global);
Dictionary star_prompt(Dictionary global);
Dictionary updateStar(Dictionary global, EmbVector mouse);

Dictionary syswindows_main(void);
Dictionary syswindows_prompt(Dictionary global);

/**
 * .
 */
Dictionary
snowflake_main(void)
{
    Dictionary global;
    //initCommand();
    //clearSelection();
    global["numPoints"] = node(2048); //Default //TODO: min:64 max:8192
    global["center.x"] = node(0.0f);
    global["center.y"] = node(0.0f);
    global["scale.x"] = node(0.04f);
    global["scale.y"] = node(0.04f);
    global["mode"] = node("SNOWFLAKE_MODE_NUM_POINTS");

    //addRubber("POLYGON");
    //setRubberMode("POLYGON");
    EmbVector v;
    v.x = 0.04f; v.y = 0.04f;
    global = updateSnowflake(global, v);
    //spareRubber("POLYGON");
    actuator("end");
    return global;
}

/**
 *
 */
EmbReal
fourier_series(EmbReal arg, std::vector<EmbReal> terms)
{
    EmbReal x = 0.0f;
    for (int i=0; i<(int)(terms.size()/3); i++) {
        x += terms[3*i+0] * sin(terms[3*i+1] + terms[3*i+2] * arg);
    }
    return x;
}

/**
 * Snowflake Curve with t [0,2pi]
 */
Dictionary
updateSnowflake(Dictionary global, EmbVector scale)
{
    for (int i = 0; i <= global["numPoints"].i; i++) {
        EmbReal t = (2.0*emb_constant_pi) / global["numPoints"].i*i;
        EmbVector v;
        v.x = fourier_series(t, snowflake_x);
        v.y = fourier_series(t, snowflake_y);

        /*
        setRubberPoint("POLYGON_POINT_" + i.toString(),
            xx*scale.x, yy*scale.y);
        */
    }

    /*
    setRubberText("POLYGON_NUM_POINTS", numPoints.toString());
    */
}

/**
 * .
 */
Dictionary
star_main(void)
{
    Dictionary global;
    /*
    initCommand();
    clearSelection();
    global.numPoints = 5;
    EmbVector center;
    EmbVector point1;
    EmbVector point2;
    center.x = 0.0;
    center.y = 0.0;
    point1.x = 1.0;
    point1.y = 1.0;
    point2.x = 2.0;
    point2.y = 2.0;
    global["center"] = node(center);
    global["point1"] = node(point1);
    global["point2"] = node(point2);
    global.mode = STAR_MODE_NUM_POINTS;
    setPromptPrefix(tr("Enter number of star points {5}: "));
    */
    return global;
}

/**
 * @brief star_click
 */
Dictionary
star_click(Dictionary global, EmbReal mouse)
{
    /*
    if (global["mode"].s == MODE_NUM_POINTS) {
        //Do nothing, the prompt controls this.
    }
    else if (global["mode"].s == MODE_CENTER_PT) {
        global->center = mouse;
        global.mode = MODE_RAD_OUTER;
        setPromptPrefix(tr("Specify outer radius of star: "));
        addRubber("POLYGON");
        setRubberMode("POLYGON");
        updateStar(global, global->center);
        actuator("enable move-rapid-fire");
    }
    else if (global["mode"].s == MODE_RAD_OUTER) {
        global->point1 = mouse;
        global["mode"].s = MODE_RAD_INNER;
        setPromptPrefix(tr("Specify inner radius of star: "));
        updateStar(global.point1);
    }
    else if (global["mode"].s == MODE_RAD_INNER) {
        global->point2 = mouse;
        actuator("disable move-rapid-fire");
        updateStar(global.point2);
        spareRubber("POLYGON");
        //actuator("end");
    }
    */
    return global;
}

/**
 * @brief star_move
 */
Dictionary
star_move(Dictionary global, EmbVector v)
{
    if (global["mode"].s == "STAR_MODE_NUM_POINTS") {
        //Do nothing, the prompt controls this.
    }
    else if (global["mode"].s == "STAR_MODE_CENTER_PT") {
        //Do nothing, prompt and click controls this.
    }
    else if (global["mode"].s == "STAR_MODE_RAD_OUTER") {
        global = updateStar(global, v);
    }
    else if (global["mode"].s == "STAR_MODE_RAD_INNER") {
        global = updateStar(global, v);
    }
    return global;
}

/**
 * @brief star_context
 */
Dictionary
star_context(Dictionary global, String str)
{
    //todo("STAR", "context()");
}

/**
 * @brief star_prompt
 */
Dictionary
star_prompt(Dictionary global, String str)
{
    /*
    if (global["mode"].s == STAR_MODE_NUM_POINTS) {
        if (str == "" && global.numPoints >= 3 && global.numPoints <= 1024) {
            setPromptPrefix(tr("Specify center point: "));
            global.mode = STAR_MODE_CENTER_PT;
        }
        else {
            EmbReal tmp = Number(str);
            if (std::isnan(tmp) || !isInt(tmp) || tmp < 3 || tmp > 1024) {
                alert(tr("Requires an integer between 3 and 1024."));
                setPromptPrefix(tr("Enter number of star points") + " {" + global.numPoints.toString() + "}: ");
            }
            else {
                global.numPoints = tmp;
                setPromptPrefix(tr("Specify center point: "));
                global.mode = MODE_CENTER_PT;
            }
        }
    }
    else if (global["mode"].s == STAR_MODE_CENTER_PT) {
        EmbReal strList = str.split(",");
        if (std::isnan(strList[0]) || std::isnan(strList[1])) {
            alert(tr("Invalid point."));
            setPromptPrefix(tr("Specify center point: "));
        }
        else {
            global->center.x = Number(strList[0]);
            global->center.y = Number(strList[1]);
            global.mode = MODE_RAD_OUTER;
            setPromptPrefix(tr("Specify outer radius of star: "));
            addRubber("POLYGON");
            setRubberMode("POLYGON");
            updateStar(qsnapX(), qsnapY());
            actuator("enable move-rapid-fire");
        }
    }
    else if (global["mode"].s == STAR_MODE_RAD_OUTER) {
        EmbReal strList = str.split(",");
        if (std::isnan(strList[0]) || std::isnan(strList[1])) {
            alert(tr("Invalid point."));
            setPromptPrefix(tr("Specify outer radius of star: "));
        }
        else {
            global.x1 = Number(strList[0]);
            global.y1 = Number(strList[1]);
            global.mode = MODE_RAD_INNER;
            setPromptPrefix(tr("Specify inner radius of star: "));
            updateStar(qsnapX(), qsnapY());
        }
    }
    else if (global["mode"].s == STAR_MODE_RAD_INNER) {
        EmbReal strList = str.split(",");
        if (std::isnan(strList[0]) || std::isnan(strList[1])) {
            alert(tr("Invalid point."));
            setPromptPrefix(tr("Specify inner radius of star: "));
        }
        else {
            global.x2 = Number(strList[0]);
            global.y2 = Number(strList[1]);
            actuator("disable move-rapid-fire");
            updateStar(global.x2, global.y2);
            spareRubber("POLYGON");
            //actuator("end");
        }
    }
    */
}

/**
 * @brief updateStar
 */
Dictionary
updateStar(Dictionary global, EmbVector mouse)
{
    EmbReal distOuter;
    EmbReal distInner;
    EmbReal angOuter;

    if (global["mode"].s == "STAR_MODE_RAD_OUTER") {
        EmbVector v = mouse - global["center"].v;
        angOuter = embVector_angle(v);
        distOuter = embVector_length(v);
        distInner = distOuter/2.0;
    }
    else if (global["mode"].s == "STAR_MODE_RAD_INNER") {
        EmbVector v = global["point1"].v - global["center"].v;
        angOuter = embVector_angle(v);
        distOuter = embVector_length(v);
        distInner = embVector_distance(global["center"].v, mouse);
    }

    // Calculate the Star Points
    EmbReal angInc = 360.0/(global["numPoints"].i*2);
    for (int i = 0; i < global["numPoints"].i*2; i++) {
        EmbReal angle = (angOuter + (angInc*i)) * emb_constant_pi / 180.0;
        EmbVector v = embVector_unit(angle);
        if (i%2 == 0) {
            v = v * distOuter;
        }
        else {
            v = v * distInner;
        }
        /*
        setRubberPoint(
            QString::fromStdString("POLYGON_POINT_" + std::to_string(i)),
            global->center.x + v.x,
            global->center.y + v.y);
        */
    }
    /*
    setRubberText("POLYGON_NUM_POINTS",
        (global.numPoints*2 - 1).toString());
    */
    return global;
}

/**
 * .
 */
Dictionary
syswindows_main(Dictionary global)
{
    /*
    initCommand();
    clearSelection();
    setPromptPrefix(tr("Enter an option [Cascade/Tile]: "));
    */
}

/**
 * .
 */
Dictionary
syswindows_prompt(Dictionary global, String str)
{
    if (str == "C" || str == "CASCADE") {
        actuator("window cascade");
        actuator("end");
    }
    else if (str == "T" || str == "TILE") {
        actuator("window tile");
        actuator("end");
    }
    else {
        /*
        alert(tr("Invalid option keyword."));
        setPromptPrefix(tr("Enter an option [Cascade/Tile]: "));
        */
    }
}
