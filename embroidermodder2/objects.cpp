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
 * @todo convert to StringList, move to configuration
 */
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
 * @brief Geometry::allGripPoints
 * @return
 */
std::vector<QPointF>
Geometry::allGripPoints()
{
    std::vector<QPointF> gripPoints;
    switch (Type) {
    case OBJ_TYPE_ARC:
        gripPoints = {
            objectCenter(),
            objectStartPoint(),
            objectMidPoint(),
            objectEndPoint()
        };
        break;
    case OBJ_TYPE_CIRCLE:
    case OBJ_TYPE_ELLIPSE:
        gripPoints = {
            objectCenter(),
            objectQuadrant0(),
            objectQuadrant90(),
            objectQuadrant180(),
            objectQuadrant270()
        };
        break;
    case OBJ_TYPE_DIMLEADER:
        gripPoints = {
            objectEndPoint1(),
            objectEndPoint2()
        };
        if (curved) {
            gripPoints.push_back(objectMidPoint());
        }
        break;
    case OBJ_TYPE_IMAGE:
    case OBJ_TYPE_RECTANGLE:
        gripPoints = {
            objectTopLeft(),     //Top Left Corner QSnap
            objectTopRight(),    //Top Right Corner QSnap
            objectBottomLeft(),  //Bottom Left Corner QSnap
            objectBottomRight()  //Bottom Right Corner QSnap
        };
        break;
    case OBJ_TYPE_LINE:
        gripPoints ={
            objectEndPoint1(),
            objectEndPoint2(),
            objectMidPoint()
        };
        break;
    case OBJ_TYPE_PATH:
    case OBJ_TYPE_POLYGON:
    case OBJ_TYPE_POLYLINE: {
            QPainterPath::Element element;
            for (int i = 0; i < normalPath.elementCount(); ++i) {
                element = normalPath.elementAt(i);
                gripPoints.push_back(mapToScene(element.x, element.y));
            }
        }
        break;
    default:
    case OBJ_TYPE_TEXTSINGLE:
    case OBJ_TYPE_POINT:
        gripPoints = {
            scenePos()
        };
        break;
    }

    return gripPoints;
}

/**
 * @brief Geometry::~Geometry
 */
Geometry::~Geometry()
{
    debug_message("Geometry Destructor()");
}

/**
 * @brief Geometry::run_command
 * @param command
 */
void
Geometry::run_command(String command)
{
    switch (Type) {
    case OBJ_TYPE_BASE:
        break;
    case OBJ_TYPE_ARC:
        run_arc_command(command);
        break;
    case OBJ_TYPE_CIRCLE:
        run_circle_command(command);
        break;
    case OBJ_TYPE_ELLIPSE:
        run_ellipse_command(command);
        break;
    default:
        break;
    }
}

void
Geometry::run_arc_command(String command)
{

}

void
Geometry::run_circle_command(String command)
{

}

void
Geometry::run_ellipse_command(String command)
{

}

void
Geometry::run_image_command(String command)
{

}

/**
 * @brief Geometry::setObjectLineWeight
 * @param lineWeight
 */
void
Geometry::setObjectLineWeight(String lineWeight)
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
 * @brief Geometry::objectRubberPoint
 * @param key
 * @return
 */
QPointF
Geometry::objectRubberPoint(QString  key)
{
    if (objRubberPoints.contains(key))
        return objRubberPoints.value(key);

    QGraphicsScene* gscene = scene();
    if (gscene)
        return scene()->property("SCENE_QSNAP_POINT").toPointF();
    return QPointF();
}

/**
 * @brief Geometry::objectRubberText
 * @param key
 * @return
 */
QString
Geometry::objectRubberText(QString  key)
{
    if (objRubberTexts.contains(key))
        return objRubberTexts.value(key);
    return QString();
}

/**
 * If gripped, force this object to be drawn even if it is offscreen.
 */
QRectF
Geometry::boundingRect()
{
    if (objRubberMode == "OBJ_RUBBER_GRIP") {
        return scene()->sceneRect();
    }
    return path().boundingRect();
}

/**
 * @brief Geometry::drawRubberLine
 * @param rubLine
 * @param painter
 * @param colorFromScene
 */
void
Geometry::drawRubberLine(const QLineF& rubLine, QPainter* painter, const char* colorFromScene)
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
 * @brief Geometry::realRender
 * @param painter
 * @param renderPath
 */
void
Geometry::realRender(QPainter* painter, const QPainterPath& renderPath)
{
    QColor color1 = objPen.color();       //lighter color
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
 * @brief Geometry::mouseSnapPoint
 * @param mousePoint
 * @return the closest snap point to the mouse point.
 */
QPointF
Geometry::mouseSnapPoint(const QPointF& mousePoint)
{
    return closest_point(mousePoint, allGripPoints());
}

/**
 * @brief DimLeaderObject::setObjectEndPoint1
 * @param x1
 * @param y1
 */
void
Geometry::setObjectEndPoint1(EmbVector endPt1)
{
    EmbVector endPt2 = to_EmbVector(objectEndPoint2());
    EmbVector delta = endPt2 - endPt1;
    setRotation(0);
    setLine(0, 0, delta.x, delta.y);
    setPos(endPt1.x, endPt1.y);
    if (Type == OBJ_TYPE_DIMLEADER) {
        updateLeader();
    }
}

/**
 * @brief DimLeaderObject::setObjectEndPoint2
 * @param x2
 * @param y2
 */
void
Geometry::setObjectEndPoint2(EmbVector endPt2)
{
    EmbVector endPt1 = to_EmbVector(scenePos());
    EmbVector delta = endPt2 - endPt1;
    setRotation(0);
    setLine(0, 0, delta.x, delta.y);
    setPos(endPt1.x, endPt1.y);
    if (Type == OBJ_TYPE_DIMLEADER) {
        updateLeader();
    }
}

/**
 * @brief DimLeaderObject::objectEndPoint1
 * @return
 */
QPointF
Geometry::objectEndPoint1()
{
    return scenePos();
}

/**
 * @brief DimLeaderObject::objectEndPoint2
 * @return
 */
QPointF
Geometry::objectEndPoint2()
{
    EmbVector v;
    EmbReal rot = radians(rotation());
    v.x = objLine.x2()*scale();
    v.y = objLine.y2()*scale();
    QPointF rotEnd = to_QPointF(rotate_vector(v, rot));

    return scenePos() + rotEnd;
}

/**
 * @brief DimLeaderObject::updateLeader
 */
void
Geometry::updateLeader()
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
 * @brief DimLeaderObject::updateRubber
 * @param painter
 */
void
Geometry::updateRubber(QPainter* painter)
{
    /*
    switch (Type) {
    case OBJ_TYPE_DIMLEADER: {
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
        break;
    }
    case OBJ_TYPE_ELLIPSE: {
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
            //TODO: updateRubber() gripping for Geometry
        }

        break;
    }
    case OBJ_TYPE_IMAGE: {
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
        break;
    }
    case OBJ_TYPE_LINE: {
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
        break;
    }
    default:
        break;
    }
    */
}

EmbReal
Geometry::objectRadius()
{
    return rect().width()/2.0*scale();
}

EmbReal
Geometry::objectDiameter()
{
    return 2.0 * objectRadius() * scale();
}

EmbReal
Geometry::objectCircumference()
{
    return emb_constant_pi * objectDiameter();
}

QPointF
Geometry::objectQuadrant0()
{
    return objectCenter() + QPointF(objectRadius(), 0);
}

QPointF
Geometry::objectQuadrant90()
{
    return objectCenter() + QPointF(0,-objectRadius());
}

QPointF
Geometry::objectQuadrant180()
{
    return objectCenter() + QPointF(-objectRadius(),0);
}

QPointF
Geometry::objectQuadrant270()
{
    return objectCenter() + QPointF(0, objectRadius());
}

EmbReal
Geometry::objectWidth()
{
    return rect().width()*scale();
}

EmbReal
Geometry::objectHeight()
{
    return rect().height()*scale();
}

EmbReal
Geometry::objectRadiusMajor()
{
    return std::max(rect().width(), rect().height())/2.0*scale();
}

EmbReal
Geometry::objectRadiusMinor()
{
    return std::min(rect().width(), rect().height())/2.0*scale();
}

EmbReal
Geometry::objectDiameterMajor()
{
    return std::max(rect().width(), rect().height())*scale();
}

EmbReal
Geometry::objectDiameterMinor()
{
    return std::min(rect().width(), rect().height())*scale();
}

void
Geometry::setObjectCenter(EmbVector center)
{
    setPos(center.x, center.y);
}

void
Geometry::setObjectCenterX(EmbReal centerX)
{
    setX(centerX);
}

void
Geometry::setObjectCenterY(EmbReal centerY)
{
    setY(centerY);
}

QRectF
Geometry::rect()
{
    return path().boundingRect();
}

void
Geometry::setRect(const QRectF& r)
{
    QPainterPath p;
    p.addRect(r);
    setPath(p);
}

void
Geometry::setRect(EmbReal x, EmbReal y, EmbReal w, EmbReal h)
{
    QPainterPath p;
    p.addRect(x,y,w,h);
    setPath(p);
}

void
Geometry::setLine(const QLineF& li)
{
    QPainterPath p;
    p.moveTo(li.p1());
    p.lineTo(li.p2());
    setPath(p);
    objLine = li;
}

void
Geometry::setLine(EmbReal x1, EmbReal y1, EmbReal x2, EmbReal y2)
{
    QPainterPath p;
    p.moveTo(x1,y1);
    p.lineTo(x2,y2);
    setPath(p);
    objLine.setLine(x1,y1,x2,y2);
}

/**
 * @brief DimLeaderObject::vulcanize
 */
void
Geometry::vulcanize(void)
{
    debug_message("DimLeaderObject vulcanize()");
    updateRubber();

    objRubberMode = "OBJ_RUBBER_OFF";

    if (Type == OBJ_TYPE_POLYGON) {
        if (!normalPath.elementCount()) {
            QMessageBox::critical(0, QObject::tr("Empty Polygon Error"), QObject::tr("The polygon added contains no points. The command that created this object has flawed logic."));
        }
    }
    if (Type == OBJ_TYPE_POLYLINE) {
        if (!normalPath.elementCount()) {
            QMessageBox::critical(0, QObject::tr("Empty Polyline Error"), QObject::tr("The polyline added contains no points. The command that created this object has flawed logic."));
        }
    }
    if (Type == OBJ_TYPE_PATH) {
        if (!normalPath.elementCount()) {
            QMessageBox::critical(0, QObject::tr("Empty Path Error"), QObject::tr("The path added contains no points. The command that created this object has flawed logic."));
        }
    }
}

/**
 * @brief DimLeaderObject::gripEdit
 * @param before
 * @param after
 */
void
Geometry::gripEdit(const QPointF& before, const QPointF& after)
{
    QPointF delta = after-before;

    switch (Type) {
    case OBJ_TYPE_LINE:
    case OBJ_TYPE_DIMLEADER:
        if (before == objectEndPoint1()) {
            setObjectEndPoint1(to_EmbVector(after));
        }
        else if (before == objectEndPoint2()) {
            setObjectEndPoint2(to_EmbVector(after));
        }
        else if (before == objectMidPoint()) {
            moveBy(delta.x(), delta.y());
        }
        break;

    case OBJ_TYPE_CIRCLE:
        if (before == objectCenter()) {
            moveBy(delta.x(), delta.y());
        }
        else {
            setObjectRadius(QLineF(objectCenter(), after).length());
        }
        break;

    case OBJ_TYPE_POINT:
    case OBJ_TYPE_TEXTSINGLE:
        if (before == scenePos()) {
            moveBy(delta.x(), delta.y());
        }
        break;

    case OBJ_TYPE_PATH:
    case OBJ_TYPE_POLYLINE:
    case OBJ_TYPE_POLYGON: {
        gripIndex = findIndex(before);
        if (gripIndex == -1)
            return;
        QPointF a = mapFromScene(after);
        normalPath.setElementPositionAt(gripIndex, a.x(), a.y());
        updatePath();
        gripIndex = -1;
        break;
    }

    case OBJ_TYPE_RECTANGLE:
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
        break;

    default:
        break;
    }
}

/**
 * @brief DimLeaderObject::objectAngle
 * @return
 */
EmbReal
Geometry::objectAngle()
{
    EmbReal angle = objLine.angle() - rotation();
    return std::fmod(angle, 360.0);
}

/**
 * @brief Geometry::objectSavePath
 * @return
 */
QPainterPath
Geometry::objectSavePath()
{
    QPainterPath path;
    switch (Type) {
    case OBJ_TYPE_CIRCLE: {
        QRectF r = rect();
        path.arcMoveTo(r, 0);
        path.arcTo(r, 0, 360);
        break;
    }

    case OBJ_TYPE_ELLIPSE: {
        QRectF r = rect();
        path.arcMoveTo(r, 0);
        path.arcTo(r, 0, 360);
        break;
    }

    case OBJ_TYPE_LINE: {
        /* \todo discards qualifiers error?
        QPointF delta = objectDelta();
        path.moveTo(objectStartPoint());
        path.lineTo(delta.x(), delta.y());
        */
        break;
    }

    case OBJ_TYPE_POINT: {
        path.addRect(-0.00000001, -0.00000001, 0.00000002, 0.00000002);
        break;
    }
    
    case OBJ_TYPE_POLYLINE:
    case OBJ_TYPE_PATH: {
        path = normalPath;
        break;
    }

    case OBJ_TYPE_POLYGON: {
        path = normalPath;
        path.closeSubpath();
        break;
    }

    case OBJ_TYPE_RECTANGLE: {
        QRectF r = rect();
        path.moveTo(r.bottomLeft());
        path.lineTo(r.bottomRight());
        path.lineTo(r.topRight());
        path.lineTo(r.topLeft());
        path.lineTo(r.bottomLeft());
        break;
    }

    default:
        break;
    }

    switch (Type) {
    case OBJ_TYPE_CIRCLE:
    case OBJ_TYPE_ELLIPSE:
    case OBJ_TYPE_POLYLINE:
    case OBJ_TYPE_POLYGON:
    case OBJ_TYPE_PATH:
    case OBJ_TYPE_RECTANGLE: {
        EmbReal s = scale();
        QTransform trans;
        trans.rotate(rotation());
        trans.scale(s,s);
        return trans.map(path);
    }
    default:
        break;
    }

    return path;
}

#if 0
DimLeaderObject::DimLeaderObject(EmbReal x1, EmbReal y1, EmbReal x2, EmbReal y2, QRgb rgb, QGraphicsItem* parent) : Geometry(OBJ_TYPE_DIMLEADER, parent)
{
    debug_message("DimLeaderObject Constructor()");
    init(x1, y1, x2, y2, rgb, Qt::SolidLine); //TODO: getCurrentLineType
}

DimLeaderObject::DimLeaderObject(DimLeaderObject* obj, QGraphicsItem* parent) : Geometry(OBJ_TYPE_DIMLEADER, parent)
{
    debug_message("DimLeaderObject Constructor()");
    if (obj) {
        init(obj->objectX1(), obj->objectY1(), obj->objectX2(), obj->objectY2(), obj->objPen.color().rgb(), Qt::SolidLine); //TODO: getCurrentLineType
    }
}

void
Geometry::DimLeaderObject::init(EmbReal x1, EmbReal y1, EmbReal x2, EmbReal y2, QRgb rgb, Qt::PenStyle lineType)
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
    objPen.setColor(rgb);
    lwtPen.setColor(rgb);
    objPen.setStyle(lineType);
    lwtPen.setStyle(lineType);
    setObjectLineWeight("0.35"); //TODO: pass in proper lineweight
    setPen(objPen);
}

#endif

Geometry::Geometry(Geometry* obj, QGraphicsItem* parent) : QGraphicsPathItem(parent)
{
    debug_message("Geometry Constructor()");
    init();
    if (obj) {
        EmbArc arc;
        arc.start = to_EmbVector(obj->objectStartPoint());
        arc.mid = to_EmbVector(obj->objectMidPoint());
        arc.end = to_EmbVector(obj->objectEndPoint());
        //init(arc, obj->objPen.color().rgb(), Qt::SolidLine); //TODO: getCurrentLineType
        setRotation(obj->rotation());
    }
}

/**
 * @brief Geometry::init
 * @param arc
 * @param rgb
 * @param lineType
Geometry::Geometry(EmbArc arc, QRgb rgb, Qt::PenStyle lineType, QGraphicsItem *parent = 0) : QGraphicsPathItem()
{
    setData(OBJ_TYPE, OBJ_TYPE_ARC);
    setData(OBJ_NAME, "Arc");

    //WARNING: DO NOT enable QGraphicsItem::ItemIsMovable. If it is enabled,
    //WARNING: and the item is double clicked, the scene will erratically move the item while zooming.
    //WARNING: All movement has to be handled explicitly by us, not by the scene.
    setFlag(QGraphicsItem::ItemIsSelectable, true);

    calculateArcData(arc);

    objPen.setColor(rgb);
    lwtPen.setColor(rgb);
    objPen.setStyle(lineType);
    lwtPen.setStyle(lineType);
    setObjectLineWeight("0.35"); //TODO: pass in proper lineweight
    setPen(objPen);
}
 */

/**
 * @brief Geometry::calculateArcData
 * @param arc
 *
 * @todo convert this to update and make it Type sensitive.
 */
void Geometry::calculateArcData(EmbArc arc)
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
 * @brief Geometry::updateArcRect
 * @param radius
 */
void
Geometry::updateArcRect(EmbReal radius)
{
    QRectF arcRect;
    arcRect.setWidth(radius*2.0);
    arcRect.setHeight(radius*2.0);
    arcRect.moveCenter(QPointF(0,0));
    setRect(arcRect);
}

/**
 * @brief Geometry::setObjectRadius
 * @param radius
 */
void
Geometry::setObjectRadius(EmbReal radius)
{
    switch (Type) {
    case OBJ_TYPE_ARC: {
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
        break;
    }

    case OBJ_TYPE_CIRCLE: {
        setObjectDiameter(radius*2.0);
        break;
    }

    default:
        break;
    }
}

/**
 * @brief Geometry::setObjectStartAngle
 * @param angle
 */
void
Geometry::setObjectStartAngle(EmbReal angle)
{
    switch (Type) {
    case OBJ_TYPE_ARC: {
        //TODO: Geometry setObjectStartAngle
        break;
    }
    default:
        break;
    }
}

/**
 * @brief Geometry::setObjectEndAngle
 * @param angle
 */
void
Geometry::setObjectEndAngle(EmbReal angle)
{
    switch (Type) {
    case OBJ_TYPE_ARC: {
        //TODO: Geometry setObjectEndAngle
        break;
    }
    default:
        break;
    }
}

/**
 * @brief Geometry::setObjectStartPoint
 * @param point
 */
void
Geometry::setObjectStartPoint(EmbVector point)
{
    switch (Type) {
    case OBJ_TYPE_ARC: {
        EmbArc arc;
        arc.start = point;
        arc.mid = to_EmbVector(objectMidPoint());
        arc.mid = to_EmbVector(objectEndPoint());
        calculateArcData(arc);
        break;
    }
    default:
        break;
    }
}

/**
 * @brief Geometry::setObjectMidPoint
 * @param point
 */
void
Geometry::setObjectMidPoint(EmbVector point)
{
    switch (Type) {
    case OBJ_TYPE_ARC: {
        EmbArc arc;
        arc.start = to_EmbVector(objectStartPoint());
        arc.mid = point;
        arc.end = to_EmbVector(objectEndPoint());
        calculateArcData(arc);
        break;
    }
    default:
        break;
    }
}

/**
 * @brief Geometry::setObjectEndPoint
 * @param point
 */
void
Geometry::setObjectEndPoint(EmbVector point)
{
    switch (Type) {
    case OBJ_TYPE_ARC: {
        EmbArc arc;
        arc.start = to_EmbVector(objectStartPoint());
        arc.mid = to_EmbVector(objectMidPoint());
        arc.end = point;
        calculateArcData(arc);
        break;
    }
    default:
        break;
    }
}

/**
 * @brief Geometry::objectStartAngle
 * @return
 */
EmbReal
Geometry::objectStartAngle(void)
{
    switch (Type) {
    case OBJ_TYPE_ARC: {
        EmbReal angle = QLineF(scenePos(), objectStartPoint()).angle();
        return std::fmod(angle, 360.0);
    }
    default:
        break;
    }
    return 0.0f;
}

/**
 * @brief Geometry::objectEndAngle
 * @return
 */
EmbReal
Geometry::objectEndAngle(void)
{
    switch (Type) {
    case OBJ_TYPE_ARC: {
        EmbReal angle = QLineF(scenePos(), objectEndPoint()).angle();
        return std::fmod(angle, 360.0);
    }
    default:
        break;
    }
    return 0.0f;
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
 * @brief Geometry::objectStartPoint
 * @return
 */
QPointF
Geometry::objectStartPoint()
{
    switch (Type) {
    case OBJ_TYPE_ARC: {
        EmbReal rot = radians(rotation());
        EmbVector v = to_EmbVector(arcStartPoint) * scale();
        EmbVector rotv = rotate_vector(v, rot);

        return scenePos() + to_QPointF(rotv);
    }
    default:
        break;
    }
    return scenePos();
}

/**
 * @brief Geometry::objectMidPoint
 * @return
 */
QPointF
Geometry::objectMidPoint()
{
    switch (Type) {
    case OBJ_TYPE_ARC: {
        EmbReal rot = radians(rotation());
        EmbVector v = to_EmbVector(arcMidPoint) * scale();
        EmbVector rotv = rotate_vector(v, rot);

        return scenePos() + to_QPointF(rotv);
    }
    case OBJ_TYPE_DIMLEADER: {
        QPointF mp = objLine.pointAt(0.5);
        EmbReal rot = radians(rotation());
        EmbVector m = to_EmbVector(mp) * scale();
        QPointF rotMid = to_QPointF(rotate_vector(m, rot));

        return scenePos() + rotMid;
    }
    default:
        break;
    }
    return scenePos();
}

/**
 * @brief Geometry::objectEndPoint
 * @return
 */
QPointF Geometry::objectEndPoint()
{
    switch (Type) {
    case OBJ_TYPE_ARC: {
        EmbReal rot = radians(rotation());
        EmbVector v = to_EmbVector(arcEndPoint) * scale();
        EmbVector rotv = rotate_vector(v, rot);

        return scenePos() + to_QPointF(rotv);
    }
    default:
        break;
    }
    return scenePos();
}

/**
 * @brief Geometry::objectArea
 * @return
 */
EmbReal
Geometry::objectArea()
{
    switch (Type) {
    case OBJ_TYPE_ARC: {
        //Area of a circular segment
        EmbReal r = objectRadius();
        EmbReal theta = radians(objectIncludedAngle());
        return ((r*r)/2)*(theta - std::sin(theta));
    }
    case OBJ_TYPE_CIRCLE:
        return emb_constant_pi*objectRadius()*objectRadius();
    default:
        break;
    }
    return std::fabs(objectWidth()*objectHeight());
}

/**
 * @brief Geometry::objectArcLength
 * @return
 */
EmbReal
Geometry::objectArcLength(void)
{
    switch (Type) {
    case OBJ_TYPE_ARC: {
        return radians(objectIncludedAngle())*objectRadius();
    }
    default:
        break;
    }
    return 0.0f;
}

/**
 * @brief Geometry::objectChord
 * @return
 */
EmbReal
Geometry::objectChord(void)
{
    switch (Type) {
    case OBJ_TYPE_ARC: {
        return QLineF(
            objectStartPoint().x(),
            objectStartPoint().y(),
            objectEndPoint().x(),
            objectEndPoint().y()).length();
    }
    default:
        break;
    }
    return 0.0f;
}

/**
 * @brief Geometry::objectIncludedAngle
 * @return
 */
EmbReal
Geometry::objectIncludedAngle(void)
{
    switch (Type) {
    case OBJ_TYPE_ARC: {
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
    default:
        break;
    }
    return 0.0f;
}

/**
 * @brief Geometry::objectClockwise
 * @return
 */
bool
Geometry::objectClockwise()
{
    switch (Type) {
    case OBJ_TYPE_ARC: {
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
        break;
    }
    default:
        break;
    }
    return false;
}

/**
 * @brief Geometry::paint
 * @param painter
 * @param option
 */
void
Geometry::paint(QPainter* painter, const QStyleOptionGraphicsItem* option, QWidget* /*widget*/)
{
    QGraphicsScene* objScene = scene();
    if (!objScene) {
        return;
    }

    QPen paintPen = pen();
    painter->setPen(paintPen);

    switch (Type) {
    case OBJ_TYPE_ARC: {
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
        break;
    }
    case OBJ_TYPE_DIMLEADER: {
        updateRubber(painter);
        if (option->state & QStyle::State_Selected) {
            paintPen.setStyle(Qt::DashLine);
        }
        if (objScene->property("ENABLE_LWT").toBool()) {
            paintPen = lwtPen;
        }
        painter->setPen(paintPen);

        painter->drawPath(lineStylePath);
        painter->drawPath(arrowStylePath);

        if (filled) {
            painter->fillPath(arrowStylePath, objPen.color());
        }
        break;
    }
    default:
        break;
    }
}

#if 0
Geometry::Geometry(EmbCircle circle, QRgb rgb, QGraphicsItem* parent) : Geometry(OBJ_TYPE_CIRCLE, parent)
{
    debug_message("Geometry Constructor()");
    init(centerX, centerY, radius, rgb, Qt::SolidLine); //TODO: getCurrentLineType
}

Geometry::Geometry(Geometry* obj, QGraphicsItem* parent) : Geometry(OBJ_TYPE_CIRCLE, parent)
{
    debug_message("Geometry Constructor()");
    if (obj) {
        init(obj->objectCenter().x(), obj->objectCenter().y(), obj->objectRadius(), obj->objPen.color().rgb(), Qt::SolidLine); //TODO: getCurrentLineType
        setRotation(obj->rotation());
    }
}

void
Geometry::init(EmbReal centerX, EmbReal centerY, EmbReal radius, QRgb rgb, Qt::PenStyle lineType)
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
    objPen.setColor(rgb);
    lwtPen.setColor(rgb);
    objPen.setStyle(lineType);
    lwtPen.setStyle(lineType);
    setObjectLineWeight("0.35"); //TODO: pass in proper lineweight
    setPen(objPen);
    updatePath();
}
#endif


/**
 * @brief Geometry::setObjectDiameter
 * @param diameter
 */
void
Geometry::setObjectDiameter(EmbReal diameter)
{
    QRectF circRect;
    circRect.setWidth(diameter);
    circRect.setHeight(diameter);
    circRect.moveCenter(QPointF(0,0));
    setRect(circRect);
    updatePath();
}

/**
 * @brief Geometry::setObjectArea
 * @param area
 */
void
Geometry::setObjectArea(EmbReal area)
{
    EmbReal radius = std::sqrt(area/emb_constant_pi);
    setObjectRadius(radius);
}

/**
 * @brief Geometry::setObjectCircumference
 * @param circumference
 */
void
Geometry::setObjectCircumference(EmbReal circumference)
{
    EmbReal diameter = circumference/emb_constant_pi;
    setObjectDiameter(diameter);
}

#if 0
/**
 * @brief Geometry::paint
 * @param painter
 * @param option
 */
void
Geometry::paint(QPainter* painter, const QStyleOptionGraphicsItem* option, QWidget* /*widget*/)
{
    case OBJ_TYPE_CIRCLE:
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
 * @brief Geometry::updateRubber
 * @param painter
 */
void
Geometry::updateRubber(QPainter* painter)
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

Geometry::Geometry(EmbReal centerX, EmbReal centerY, EmbReal width, EmbReal height, QRgb rgb, QGraphicsItem* parent) : Geometry(OBJ_TYPE_ELLIPSE, parent)
{
    debug_message("Geometry Constructor()");
    init(centerX, centerY, width, height, rgb, Qt::SolidLine); //TODO: getCurrentLineType
}

Geometry::Geometry(Geometry* obj, QGraphicsItem* parent) : Geometry(OBJ_TYPE_ELLIPSE, parent)
{
    debug_message("Geometry Constructor()");
    if (obj) {
        init(
            obj->objectCenter().x(),
            obj->objectCenter().y(),
            obj->objectWidth(),
            obj->objectHeight(),
            obj->objPen.color().rgb(),
            Qt::SolidLine); //TODO: getCurrentLineType
        setRotation(obj->rotation());
    }
}

/**
 * \brief .
 */
void
Geometry::init(EmbReal centerX, EmbReal centerY, EmbReal width, EmbReal height, QRgb rgb, Qt::PenStyle lineType)
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
    objPen.setColor(rgb);
    lwtPen.setColor(rgb);
    objPen.setStyle(lineType);
    lwtPen.setStyle(lineType);
    setObjectLineWeight("0.35"); //TODO: pass in proper lineweight
    setPen(objPen);
    updatePath();
}

/**
 * \brief .
 */
void
Geometry::setObjectSize(EmbReal width, EmbReal height)
{
    QRectF elRect = rect();
    elRect.setWidth(width);
    elRect.setHeight(height);
    elRect.moveCenter(QPointF(0,0));
    setRect(elRect);
}
#endif

void
Geometry::setObjectRadiusMajor(EmbReal radius)
{
    setObjectDiameterMajor(radius*2.0);
}

void
Geometry::setObjectRadiusMinor(EmbReal radius)
{
    setObjectDiameterMinor(radius*2.0);
}

void
Geometry::setObjectDiameterMajor(EmbReal diameter)
{
    QRectF elRect = rect();
    if (elRect.width() > elRect.height())
        elRect.setWidth(diameter);
    else
        elRect.setHeight(diameter);
    elRect.moveCenter(QPointF(0,0));
    setRect(elRect);
}

void
Geometry::setObjectDiameterMinor(EmbReal diameter)
{
    QRectF elRect = rect();
    if (elRect.width() < elRect.height()) {
        elRect.setWidth(diameter);
    }
    else {
        elRect.setHeight(diameter);
    }
    elRect.moveCenter(QPointF(0,0));
    setRect(elRect);
}

#if 0
QPointF
Geometry::objectQuadrant0()
{
    EmbReal halfW = objectWidth()/2.0;
    EmbReal rot = radians(rotation());
    EmbVector v = embVector_unit(rot) * halfW;
    return objectCenter() + to_QPointF(v);
}

QPointF
Geometry::objectQuadrant90()
{
    EmbReal halfH = objectHeight()/2.0;
    EmbReal rot = radians(rotation()+90.0);
    EmbVector v = embVector_unit(rot) * halfH;
    return objectCenter() + to_QPointF(v);
}

QPointF
Geometry::objectQuadrant180()
{
    EmbReal halfW = objectWidth()/2.0;
    EmbReal rot = radians(rotation()+180.0);
    EmbReal x = halfW*std::cos(rot);
    EmbReal y = halfW*std::sin(rot);
    return objectCenter() + QPointF(x,y);
}

QPointF
Geometry::objectQuadrant270()
{
    EmbReal halfH = objectHeight()/2.0;
    EmbReal rot = radians(rotation()+270.0);
    EmbReal x = halfH*std::cos(rot);
    EmbReal y = halfH*std::sin(rot);
    return objectCenter() + QPointF(x,y);
}

void
Geometry::paint(QPainter* painter, const QStyleOptionGraphicsItem* option, QWidget* /*widget*/)
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

ImageObject::ImageObject(EmbReal x, EmbReal y, EmbReal w, EmbReal h, QRgb rgb, QGraphicsItem* parent) : Geometry(OBJ_TYPE_IMAGE, parent)
{
    debug_message("ImageObject Constructor()");
    init(x, y, w, h, rgb, Qt::SolidLine); //TODO: getCurrentLineType
}

ImageObject::ImageObject(ImageObject* obj, QGraphicsItem* parent) : Geometry(OBJ_TYPE_IMAGE, parent)
{
    debug_message("ImageObject Constructor()");
    if (obj) {
        QPointF ptl = obj->objectTopLeft();
        init(ptl.x(), ptl.y(), obj->objectWidth(), obj->objectHeight(), obj->objPen.color().rgb(), Qt::SolidLine); //TODO: getCurrentLineType
        setRotation(obj->rotation());
    }
}

void
Geometry::ImageObject::init(EmbReal x, EmbReal y, EmbReal w, EmbReal h, QRgb rgb, Qt::PenStyle lineType)
{
    setData(OBJ_TYPE, OBJ_TYPE_IMAGE);
    setData(OBJ_NAME, "Image");

    //WARNING: DO NOT enable QGraphicsItem::ItemIsMovable. If it is enabled,
    //WARNING: and the item is double clicked, the scene will erratically move the item while zooming.
    //WARNING: All movement has to be handled explicitly by us, not by the scene.
    setFlag(QGraphicsItem::ItemIsSelectable, true);

    setObjectRect(x, y, w, h);
    objPen.setColor(rgb);
    lwtPen.setColor(rgb);
    objPen.setStyle(lineType);
    lwtPen.setStyle(lineType);
    setObjectLineWeight("0.35"); //TODO: pass in proper lineweight
    setPen(objPen);
}
#endif

/**
 * \brief .
 */
void
Geometry::setObjectRect(EmbReal x, EmbReal y, EmbReal w, EmbReal h)
{
    setPos(x, y);
    setRect(0, 0, w, h);
    updatePath();
}

/**
 * \brief .
 */
QPointF
Geometry::objectTopLeft()
{
    EmbReal rot = radians(rotation());
    QPointF tl = rect().topLeft();
    EmbVector ptl = to_EmbVector(tl) * scale();
    EmbVector ptlRot = rotate_vector(ptl, rot);

    return (scenePos() + QPointF(ptlRot.x, ptlRot.y));
}

/**
 * \brief .
 */
QPointF
Geometry::objectTopRight()
{
    EmbReal rot = radians(rotation());
    QPointF tr = rect().topRight();
    EmbVector ptr = to_EmbVector(tr) * scale();
    EmbVector ptrRot = rotate_vector(ptr, rot);

    return (scenePos() + QPointF(ptrRot.x, ptrRot.y));
}

/**
 * \brief .
 */
QPointF
Geometry::objectBottomLeft()
{
    EmbReal rot = radians(rotation());
    QPointF bl = rect().bottomLeft();
    EmbVector pbl = to_EmbVector(bl) * scale();
    EmbVector pblRot = rotate_vector(pbl, rot);

    return scenePos() + to_QPointF(pblRot);
}

QPointF
Geometry::objectBottomRight()
{
    EmbReal rot = radians(rotation());
    QPointF br = rect().bottomRight();
    EmbVector pbr = to_EmbVector(br) * scale();
    EmbVector pbrRot = rotate_vector(pbr, rot);

    return scenePos() + to_QPointF(pbrRot);
}

#if 0
/**
 * \brief .
 */
void
Geometry::ImageObject::paint(QPainter* painter, const QStyleOptionGraphicsItem* option, QWidget* /*widget*/)
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

Geometry::Geometry(EmbLine line, QRgb rgb, QGraphicsItem* parent) : Geometry(OBJ_TYPE_LINE, parent)
{
    debug_message("Geometry Constructor()");
    init(line, rgb, Qt::SolidLine); //TODO: getCurrentLineType
}

Geometry::Geometry(Geometry* obj, QGraphicsItem* parent) : Geometry(OBJ_TYPE_LINE, parent)
{
    debug_message("Geometry Constructor()");
    if (obj) {
        EmbLine line;
        line.start = to_EmbVector(obj->objectEndPoint1());
        line.end = to_EmbVector(obj->objectEndPoint2());
        init(line, obj->objPen.color().rgb(), Qt::SolidLine); //TODO: getCurrentLineType
    }
}

void
Geometry::init(EmbLine line, QRgb rgb, Qt::PenStyle lineType)
{
    setData(OBJ_TYPE, OBJ_TYPE_LINE);
    setData(OBJ_NAME, "Line");

    //WARNING: DO NOT enable QGraphicsItem::ItemIsMovable. If it is enabled,
    //WARNING: and the item is double clicked, the scene will erratically move the item while zooming.
    //WARNING: All movement has to be handled explicitly by us, not by the scene.
    setFlag(QGraphicsItem::ItemIsSelectable, true);

    setObjectEndPoint1(line.start);
    setObjectEndPoint2(line.end);
    objPen.setColor(rgb);
    lwtPen.setColor(rgb);
    objPen.setStyle(lineType);
    lwtPen.setStyle(lineType);
    setObjectLineWeight("0.35"); //TODO: pass in proper lineweight
    setPen(objPen);
}

/**
 * @brief Geometry::setObjectEndPoint1
 * @param x1
 * @param y1
 */
void
Geometry::setObjectEndPoint1(EmbVector endPt1)
{
    EmbVector endPt2 = to_EmbVector(objectEndPoint2());
    EmbVector delta = endPt2 - endPt1;
    setRotation(0);
    setScale(1);
    setLine(0, 0, delta.x, delta.y);
    setPos(endPt1.x, endPt1.y);
}

/**
 * @brief Geometry::setObjectEndPoint2
 * @param x2
 * @param y2
 */
void
Geometry::setObjectEndPoint2(EmbVector endPt2)
{
    EmbVector endPt1 = to_EmbVector(scenePos());
    EmbVector delta = endPt2 - endPt1;
    setRotation(0);
    setScale(1);
    setLine(0, 0, delta.x, delta.y);
    setPos(endPt1.x, endPt1.y);
}

/**
 * @brief Geometry::objectEndPoint2
 * @return
 */
QPointF
Geometry::objectEndPoint2()
{
    EmbReal rot = radians(rotation());
    EmbVector v;
    v.x = objLine.x2()*scale();
    v.y = objLine.y2()*scale();
    EmbVector rotEnd = rotate_vector(v, rot);

    return scenePos() + to_QPointF(rotEnd);
}

/**
 * @brief Geometry::objectMidPoint
 * @return
 */
QPointF
Geometry::objectMidPoint()
{
    QPointF mp = objLine.pointAt(0.5);
    EmbReal rot = radians(rotation());
    EmbVector m = to_EmbVector(mp) * scale();
    EmbVector rotMid = rotate_vector(m, rot);

    return scenePos() + to_QPointF(rotMid);
}

/**
 * @brief Geometry::paint
 * @param painter
 * @param option
 */
void
Geometry::paint(QPainter* painter, const QStyleOptionGraphicsItem* option, QWidget* /*widget*/)
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

PathObject::PathObject(EmbReal x, EmbReal y, const QPainterPath p, QRgb rgb, QGraphicsItem* parent) : Geometry(OBJ_TYPE_PATH, parent)
{
    debug_message("PathObject Constructor()");
    init(x, y, p, rgb, Qt::SolidLine); //TODO: getCurrentLineType
}

PathObject::PathObject(PathObject* obj, QGraphicsItem* parent) : Geometry(OBJ_TYPE_PATH, parent)
{
    debug_message("PathObject Constructor()");
    if (obj) {
        init(obj->objectPos().x(), obj->objectPos().y(), obj->objectCopyPath(), obj->objPen.color().rgb(), Qt::SolidLine); //TODO: getCurrentLineType
        setRotation(obj->rotation());
        setScale(obj->scale());
    }
}

void
Geometry::PathObject::init(EmbReal x, EmbReal y, const QPainterPath& p, QRgb rgb, Qt::PenStyle lineType)
{
    setData(OBJ_TYPE, OBJ_TYPE_PATH);
    setData(OBJ_NAME, "Path");

    //WARNING: DO NOT enable QGraphicsItem::ItemIsMovable. If it is enabled,
    //WARNING: and the item is double clicked, the scene will erratically move the item while zooming.
    //WARNING: All movement has to be handled explicitly by us, not by the scene.
    setFlag(QGraphicsItem::ItemIsSelectable, true);

    normalPath = p;
    updatePath();
    setObjectPos(x,y);
    objPen.setColor(rgb);
    lwtPen.setColor(rgb);
    objPen.setStyle(lineType);
    lwtPen.setStyle(lineType);
    setObjectLineWeight("0.35"); //TODO: pass in proper lineweight
    setPen(objPen);
}


/**
 * @brief PathObject::paint
 * @param painter
 * @param option
 */
void
Geometry::PathObject::paint(QPainter* painter, const QStyleOptionGraphicsItem* option, QWidget* /*widget*/)
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

#endif

/**
 * @brief PathObject::objectCopyPath
 * @return
 */
QPainterPath
Geometry::objectCopyPath()
{
    return normalPath;
}

#if 0
/**
 * @brief Geometry::Geometry
 * @param x
 * @param y
 * @param rgb
 * @param parent
 */
Geometry::Geometry(EmbReal x, EmbReal y, QRgb rgb, QGraphicsItem* parent) : Geometry(OBJ_TYPE_POINT, parent)
{
    debug_message("Geometry Constructor()");
    init(x, y, rgb, Qt::SolidLine); //TODO: getCurrentLineType
}

/**
 * @brief Geometry::Geometry
 * @param obj
 * @param parent
 */
Geometry::Geometry(Geometry* obj, QGraphicsItem* parent) : Geometry(OBJ_TYPE_POINT, parent)
{
    debug_message("Geometry Constructor()");
    if (obj) {
        init(obj->objectPos().x(), obj->objectPos().y(), obj->objPen.color().rgb(), Qt::SolidLine); //TODO: getCurrentLineType
        setRotation(obj->rotation());
    }
}

void
Geometry::init(EmbReal x, EmbReal y, QRgb rgb, Qt::PenStyle lineType)
{
    setData(OBJ_TYPE, OBJ_TYPE_POINT);
    setData(OBJ_NAME, "Point");

    //WARNING: DO NOT enable QGraphicsItem::ItemIsMovable. If it is enabled,
    //WARNING: and the item is double clicked, the scene will erratically move the item while zooming.
    //WARNING: All movement has to be handled explicitly by us, not by the scene.
    setFlag(QGraphicsItem::ItemIsSelectable, true);

    setRect(-0.00000001, -0.00000001, 0.00000002, 0.00000002);
    setObjectPos(x,y);
    objPen.setColor(rgb);
    lwtPen.setColor(rgb);
    objPen.setStyle(lineType);
    lwtPen.setStyle(lineType);
    setObjectLineWeight("0.35"); //TODO: pass in proper lineweight
    setPen(objPen);
}

/**
 * @brief Geometry::paint
 * @param painter
 * @param option
 */
void
Geometry::paint(QPainter* painter, const QStyleOptionGraphicsItem* option, QWidget* /*widget*/)
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
 * @brief Geometry::updateRubber
 * @param painter
 */
void
Geometry::updateRubber(QPainter* painter)
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

Geometry::Geometry(EmbReal x, EmbReal y, const QPainterPath& p, QRgb rgb, QGraphicsItem* parent) : Geometry(OBJ_TYPE_POLYGON, parent)
{
    debug_message("Geometry Constructor()");
    init(x, y, p, rgb, Qt::SolidLine); //TODO: getCurrentLineType
}

Geometry::Geometry(Geometry* obj, QGraphicsItem* parent) : Geometry(OBJ_TYPE_POLYGON, parent)
{
    debug_message("Geometry Constructor()");
    if (obj) {
        init(obj->objectPos().x(), obj->objectPos().y(), obj->objectCopyPath(), obj->objPen.color().rgb(), Qt::SolidLine); //TODO: getCurrentLineType
        setRotation(obj->rotation());
        setScale(obj->scale());
    }
}

void
Geometry::init(EmbReal x, EmbReal y, const QPainterPath& p, QRgb rgb, Qt::PenStyle lineType)
{
    setData(OBJ_TYPE, OBJ_TYPE_POLYGON);
    setData(OBJ_NAME, "Polygon");

    //WARNING: DO NOT enable QGraphicsItem::ItemIsMovable. If it is enabled,
    //WARNING: and the item is double clicked, the scene will erratically move the item while zooming.
    //WARNING: All movement has to be handled explicitly by us, not by the scene.
    setFlag(QGraphicsItem::ItemIsSelectable, true);

    gripIndex = -1;
    normalPath = p;
    updatePath();
    setObjectPos(x,y);
    objPen.setColor(rgb);
    lwtPen.setColor(rgb);
    objPen.setStyle(lineType);
    lwtPen.setStyle(lineType);
    setObjectLineWeight("0.35"); //TODO: pass in proper lineweight
    setPen(objPen);
}
#endif

/**
 * @brief Geometry::updatePath
 *
 * For path and polyline set normalPath before calling.
 */
void
Geometry::updatePath()
{
    QPainterPath path;
    QRectF r = rect();
    switch (Type) {
    case OBJ_TYPE_ARC: {
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
        setPath(path);
        break;
    }

    case OBJ_TYPE_CIRCLE: {
        //Add the center point
        path.addRect(-0.00000001, -0.00000001, 0.00000002, 0.00000002);
        //Add the circle
        path.arcMoveTo(r, 0);
        path.arcTo(r, 0, 360);
        //NOTE: Reverse the path so that the inside area isn't considered part of the circle
        path.arcTo(r, 0, -360);
        setPath(path);
        break;
    }

    case OBJ_TYPE_ELLIPSE: {
        path.arcMoveTo(r, 0);
        path.arcTo(r, 0, 360);
        //NOTE: Reverse the path so that the inside area isn't considered part of the ellipse
        path.arcTo(r, 0, -360);
        setPath(path);
        break;
    }

    case OBJ_TYPE_RECTANGLE:
    case OBJ_TYPE_IMAGE: {
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
        setPath(path);
        break;
    }

    case OBJ_TYPE_POLYGON: {
        QPainterPath closedPath = normalPath;
        closedPath.closeSubpath();
        QPainterPath reversePath = closedPath.toReversed();
        reversePath.connectPath(closedPath);
        setPath(reversePath);
        break;
    }

    case OBJ_TYPE_PATH: {
        QPainterPath reversePath = normalPath.toReversed();
        reversePath.connectPath(normalPath);
        setPath(reversePath);
        break;
    }

    default:
        break;
    }
}

#if 0
/**
 * @brief Geometry::paint
 * @param painter
 * @param option
 */
void
Geometry::paint(QPainter* painter, const QStyleOptionGraphicsItem* option, QWidget* /*widget*/)
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
 * @brief Geometry::updateRubber
 * @param painter
 */
void
Geometry::updateRubber(QPainter* painter)
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
        objRubberTexts.insert("POLYGON_NUM_POINTS", QString());
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
#endif

/**
 * @brief Geometry::findIndex
 * @param point
 * @return
 */
int
Geometry::findIndex(const QPointF& point)
{
    int i;
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

#if 0
/**
 * @brief Geometry::Geometry
 * @param x
 * @param y
 * @param p
 * @param rgb
 * @param parent
 */
Geometry::Geometry(EmbReal x, EmbReal y, const QPainterPath& p, QRgb rgb, QGraphicsItem* parent) : Geometry(OBJ_TYPE_POLYLINE, parent)
{
    debug_message("Geometry Constructor()");
    init(x, y, p, rgb, Qt::SolidLine); //TODO: getCurrentLineType
}

/**
 * @brief Geometry::Geometry
 * @param obj
 * @param parent
 */
Geometry::Geometry(Geometry* obj, QGraphicsItem* parent) : Geometry(OBJ_TYPE_POLYLINE, parent)
{
    debug_message("Geometry Constructor()");
    if (obj) {
        init(obj->objectX(), obj->objectY(), obj->objectCopyPath(), obj->objPen.color().rgb(), Qt::SolidLine); //TODO: getCurrentLineType
        setRotation(obj->rotation());
        setScale(obj->scale());
    }
}

/**
 * @brief Geometry::init
 * @param x
 * @param y
 * @param p
 * @param rgb
 * @param lineType
 */
void
Geometry::init(EmbReal x, EmbReal y, const QPainterPath& p, QRgb rgb, Qt::PenStyle lineType)
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
    objPen.setColor(rgb);
    lwtPen.setColor(rgb);
    objPen.setStyle(lineType);
    lwtPen.setStyle(lineType);
    setObjectLineWeight("0.35"); //TODO: pass in proper lineweight
    setPen(objPen);
}

/**
 * @brief Geometry::updatePath
 * @param p
 */
void
Geometry::updatePath(const QPainterPath& p)
{
    normalPath = p;
    QPainterPath reversePath = normalPath.toReversed();
    reversePath.connectPath(normalPath);
    setPath(reversePath);
}

/**
 * @brief Geometry::paint
 * @param painter
 * @param option
 */
void
Geometry::paint(QPainter* painter, const QStyleOptionGraphicsItem* option, QWidget* /*widget*/)
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
 * @brief Geometry::updateRubber
 * @param painter
 */
void
Geometry::updateRubber(QPainter* painter)
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
        objRubberTexts.insert("POLYLINE_NUM_POINTS", QString());
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
 * @brief Geometry::findIndex
 * @param point
 * @return
 */
int
Geometry::findIndex(const QPointF& point)
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
 * \brief .
 */
Geometry::Geometry(EmbReal x, EmbReal y, EmbReal w, EmbReal h, QRgb rgb, QGraphicsItem* parent) : Geometry(OBJ_TYPE_RECTANGLE, parent)
{
    debug_message("Geometry Constructor()");
    init(x, y, w, h, rgb, Qt::SolidLine); //TODO: getCurrentLineType
}

/**
 * \brief .
 */
Geometry::Geometry(Geometry* obj, QGraphicsItem* parent) : Geometry(OBJ_TYPE_RECTANGLE, parent)
{
    debug_message("Geometry Constructor()");
    if (obj) {
        QPointF ptl = obj->objectTopLeft();
        init(ptl.x(), ptl.y(), obj->objectWidth(), obj->objectHeight(), obj->objPen.color().rgb(), Qt::SolidLine); //TODO: getCurrentLineType
        setRotation(obj->rotation());
    }
}

/**
 * \brief .
 */
void Geometry::init(EmbReal x, EmbReal y, EmbReal w, EmbReal h, QRgb rgb, Qt::PenStyle lineType)
{
    setData(OBJ_TYPE, OBJ_TYPE_RECTANGLE);
    setData(OBJ_NAME, "Rectangle");

    //WARNING: DO NOT enable QGraphicsItem::ItemIsMovable. If it is enabled,
    //WARNING: and the item is double clicked, the scene will erratically move the item while zooming.
    //WARNING: All movement has to be handled explicitly by us, not by the scene.
    setFlag(QGraphicsItem::ItemIsSelectable, true);

    setObjectRect(x, y, w, h);
    objPen.setColor(rgb);
    lwtPen.setColor(rgb);
    objPen.setStyle(lineType);
    lwtPen.setStyle(lineType);
    setObjectLineWeight("0.35"); //TODO: pass in proper lineweight
    setPen(objPen);
}

/**
 * \brief .
 */
void Geometry::setObjectRect(EmbReal x, EmbReal y, EmbReal w, EmbReal h)
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
Geometry::objectTopLeft()
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
QPointF Geometry::objectTopRight()
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
QPointF Geometry::objectBottomLeft()
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
QPointF Geometry::objectBottomRight()
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
void Geometry::updatePath()
{
    if (Type == OBJ_TYPE_RECTANGLE) {
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
        setPath(path);
    }
}

/**
 * \brief .
 */
void Geometry::paint(QPainter* painter, const QStyleOptionGraphicsItem* option, QWidget* /*widget*/)
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
void Geometry::updateRubber(QPainter* painter)
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

#endif

/**
 * @brief SaveObject::SaveObject
 * @param theScene
 * @param parent
 */
SaveObject::SaveObject(QGraphicsScene* theScene, QObject* parent) : QObject(parent)
{
    debug_message("SaveObject Constructor()");
    gscene = theScene;
    formatType = EMBFORMAT_UNSUPPORTED;
}

/**
 * @brief SaveObject::~SaveObject
 */
SaveObject::~SaveObject()
{
    debug_message("SaveObject Destructor()");
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
            else if (objType == OBJ_TYPE_DIMARCLENGTH) {
                addDimArcLength(pattern, item);
            }
            else if (objType == OBJ_TYPE_DIMDIAMETER) {
                addDimDiameter(pattern, item);
            }
            else if (objType == OBJ_TYPE_DIMLEADER) {
                addDimLeader(pattern, item);
            }
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
    Geometry* obj = static_cast<Geometry*>(item);
    if (obj) {
        if (formatType == EMBFORMAT_STITCHONLY) {
            QPainterPath path = obj->objectSavePath();
            toPolyline(pattern, obj->objectCenter(), path.simplified(), "0", obj->objPen.color(), "CONTINUOUS", "BYLAYER"); //TODO: proper layer/lineType/lineWeight //TODO: Improve precision, replace simplified
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
    Geometry* obj = static_cast<Geometry*>(item);
    if (obj) {
        if (formatType == EMBFORMAT_STITCHONLY) {
            QPainterPath path = obj->objectSavePath();
            toPolyline(pattern, obj->objectCenter(), path.simplified(), "0", obj->objPen.color(), "CONTINUOUS", "BYLAYER"); //TODO: proper layer/lineType/lineWeight //TODO: Improve precision, replace simplified
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
    Geometry* obj = static_cast<Geometry*>(item);
    if (obj) {
        if (formatType == EMBFORMAT_STITCHONLY) {
            toPolyline(pattern, obj->objectEndPoint1(), obj->objectSavePath(), "0", obj->objPen.color(), "CONTINUOUS", "BYLAYER"); //TODO: proper layer/lineType/lineWeight
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
    QGraphicsPathItem* polylineItem = (QGraphicsPathItem*)item;
    if (polylineItem) {
        /*
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
        */
    }
}

/**
 * @brief SaveObject::addPoint
 * @param pattern
 * @param item
 */
void
SaveObject::addPoint(EmbPattern* pattern, QGraphicsItem* item)
{
    Geometry* obj = static_cast<Geometry*>(item);
    if (obj) {
        if (formatType == EMBFORMAT_STITCHONLY) {
            toPolyline(pattern, obj->objectPos(), obj->objectSavePath(), "0", obj->objPen.color(), "CONTINUOUS", "BYLAYER"); //TODO: proper layer/lineType/lineWeight
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
    Geometry* obj = static_cast<Geometry*>(item);
    if (obj) {
        toPolyline(pattern, obj->objectPos(), obj->objectSavePath(), "0", obj->objPen.color(), "CONTINUOUS", "BYLAYER"); //TODO: proper layer/lineType/lineWeight
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
    Geometry* obj = static_cast<Geometry*>(item);
    if (obj) {
        toPolyline(pattern, obj->objectPos(), obj->objectSavePath(), "0", obj->objPen.color(), "CONTINUOUS", "BYLAYER"); //TODO: proper layer/lineType/lineWeight
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
    Geometry* obj = static_cast<Geometry*>(item);
    if (obj) {
        if (formatType == EMBFORMAT_STITCHONLY) {
            toPolyline(pattern, obj->objectPos(), obj->objectSavePath(), "0", obj->objPen.color(), "CONTINUOUS", "BYLAYER"); //TODO: proper layer/lineType/lineWeight
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
    Geometry* obj = static_cast<Geometry*>(item);
    if (obj) {
        if (formatType == EMBFORMAT_STITCHONLY) {
            std::vector<QPainterPath> pathList = obj->objectSavePathList();
            foreach(QPainterPath path, pathList) {
                toPolyline(pattern, obj->objectPos(), path.simplified(), "0", obj->objPen.color(), "CONTINUOUS", "BYLAYER"); //TODO: proper layer/lineType/lineWeight //TODO: Improve precision, replace simplified
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

#if 0
/**
 *
 */
Geometry::Geometry(QString  str, EmbReal x, EmbReal y, QRgb rgb, QGraphicsItem* parent) : Geometry(OBJ_TYPE_TEXTSINGLE, parent)
{
    debug_message("Geometry Constructor()");
    init(str, x, y, rgb, Qt::SolidLine); //TODO: getCurrentLineType
}

/**
 *
 */
Geometry::Geometry(Geometry* obj, QGraphicsItem* parent) : Geometry(OBJ_TYPE_TEXTSINGLE, parent)
{
    debug_message("Geometry Constructor()");
    if (obj) {
        setObjectTextFont(obj->objTextFont);
        setObjectTextSize(obj->objTextSize);
        setRotation(obj->rotation());
        setObjectTextBackward(obj->objTextBackward);
        setObjectTextUpsideDown(obj->objTextUpsideDown);
        setObjectTextStyle(obj->objTextBold, obj->objTextItalic, obj->objTextUnderline, obj->objTextStrikeOut, obj->objTextOverline);
        init(obj->objText, obj->objectX(), obj->objectY(), obj->objPen.color().rgb(), Qt::SolidLine); //TODO: getCurrentLineType
        setScale(obj->scale());
    }
}

/**
 *
 */
void
Geometry::init(QString  str, EmbReal x, EmbReal y, QRgb rgb, Qt::PenStyle lineType)
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
    objPen.setColor(rgb);
    lwtPen.setColor(rgb);
    objPen.setStyle(lineType);
    lwtPen.setStyle(lineType);
    setObjectLineWeight("0.35"); //TODO: pass in proper lineweight
    setPen(objPen);
}

#endif

/**
 *
 */
void Geometry::setObjectText(QString  str)
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
    else if (objTextJustify == "Right") {
    	textPath.translate(-jRect.right(), 0);
    }
    else if (objTextJustify == "Aligned") {
    } //TODO: Geometry Aligned Justification
    else if (objTextJustify == "Middle") {
        textPath.translate(-jRect.center());
    }
    else if (objTextJustify == "Fit") { } //TODO: Geometry Fit Justification
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
    setPath(gripPath);
}

/**
 *
 */
void
Geometry::setObjectTextFont(QString  font)
{
    objTextFont = font;
    setObjectText(objText);
}

/**
 * Verify the string is a valid option, otherwise default to "Left".
 */
void
Geometry::setObjectTextJustify(QString  justify)
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
Geometry::setObjectTextSize(EmbReal size)
{
    objTextSize = size;
    setObjectText(objText);
}

/**
 *
 */
void
Geometry::setObjectTextStyle(bool bold, bool italic, bool under, bool strike, bool over)
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
Geometry::setObjectTextBold(bool val)
{
    objTextBold = val;
    setObjectText(objText);
}

/**
 *
 */
void
Geometry::setObjectTextItalic(bool val)
{
    objTextItalic = val;
    setObjectText(objText);
}

/**
 *
 */
void
Geometry::setObjectTextUnderline(bool val)
{
    objTextUnderline = val;
    setObjectText(objText);
}

/**
 *
 */
void
Geometry::setObjectTextStrikeOut(bool val)
{
    objTextStrikeOut = val;
    setObjectText(objText);
}

/**
 *
 */
void
Geometry::setObjectTextOverline(bool val)
{
    objTextOverline = val;
    setObjectText(objText);
}

/**
 *
 */
void
Geometry::setObjectTextBackward(bool val)
{
    objTextBackward = val;
    setObjectText(objText);
}

/**
 *
 */
void
Geometry::setObjectTextUpsideDown(bool val)
{
    objTextUpsideDown = val;
    setObjectText(objText);
}

#if 0
/**
 *
 */
void
Geometry::paint(QPainter* painter, const QStyleOptionGraphicsItem* option, QWidget* /*widget*/)
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
Geometry::updateRubber(QPainter* painter)
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
#endif

/**
 *
 */
std::vector<QPainterPath>
Geometry::subPathList()
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
 * .
 */
void
Geometry::script_main(void)
{
    if (properties["script"].s == "circle") {
        properties["mode"] = node("CIRCLE_MODE_1P_RAD");
        StringList script = {
            "init",
            "clear_selection",
            "set-prompt-prefix " + _mainWin->tr("Specify center point for circle or [3P/2P/TTR (tan tan radius)]: ").toStdString()
        };
        run_script(script);
    }
}

/**
 * .
 */
void
Geometry::script_context(String str)
{

}

/**
 * .
 */
void
Geometry::script_click(EmbVector v)
{

}

/**
 * .
 */
void
Geometry::script_prompt(String str)
{

}


#if 0
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
Geometry::circle_click(EmbVector v)
{
    if (properties["mode"].s == "CIRCLE_MODE_1P_RAD") {
        auto iter = properties["find("point1");
        if (iter == properties["end()) {
            properties["point1"] = node(v);
            properties["center"] = node(v);
            addRubber("CIRCLE");
            setRubberMode("CIRCLE_1P_RAD");
            setRubberPoint("CIRCLE_CENTER", properties["center.x, properties["center.y);
            actuator("append-prompt");
            setPromptPrefix(tr("Specify radius of circle or [Diameter]: "));
        }
        else {
            properties["point2"] = node(v);
            setRubberPoint("CIRCLE_RADIUS", v);
            actuator("vulcanize");
            actuator("append-prompt");
            actuator("end");
        }
    }
    else if (properties["mode"].s == MODE_1P_DIA) {
        auto iter = properties["find("point1");
        if (iter == properties["end()) {
            error("CIRCLE", tr("This should never happen."));
        }
        else {
            properties["x2 = x;
            properties["y2 = y;
            setRubberPoint("CIRCLE_DIAMETER", properties["x2, properties["y2);
            actuator("vulcanize");
            appendPromptHistory();
            actuator("end");
        }
    }
    else if (properties["mode"].s == "MODE_2P") {
        auto iter1 = properties["find("point1");
        auto iter2 = properties["find("point2");
        if (iter1 == properties["end()) {
            properties["point1 = v;
            addRubber("CIRCLE");
            setRubberMode("CIRCLE_2P");
            setRubberPoint("CIRCLE_TAN1", v);
            appendPromptHistory();
            setPromptPrefix(tr("Specify second end point of circle's diameter: "));
        }
        else if (iter2 == properties["end()) {
            properties["point2 = v;
            setRubberPoint("CIRCLE_TAN2", v);
            actuator("vulcanize");
            appendPromptHistory();
            actuator("end");
        }
        else {
            error("CIRCLE", tr("This should never happen."));
        }
    }
    else if (properties["mode"].s == MODE_3P) {
        if (std::isnan(properties["x1)) {
            properties["x1 = x;
            properties["y1 = y;
            appendPromptHistory();
            setPromptPrefix(tr("Specify second point on circle: "));
        }
        else if (std::isnan(properties["x2)) {
            properties["x2 = x;
            properties["y2 = y;
            addRubber("CIRCLE");
            setRubberMode("CIRCLE_3P");
            setRubberPoint("CIRCLE_TAN1", properties["x1, properties["y1);
            setRubberPoint("CIRCLE_TAN2", properties["x2, properties["y2);
            appendPromptHistory();
            setPromptPrefix(tr("Specify third point on circle: "));
        }
        else if (std::isnan(properties["x3)) {
            properties["x3 = x;
            properties["y3 = y;
            setRubberPoint("CIRCLE_TAN3", properties["x3, properties["y3);
            actuator("vulcanize");
            appendPromptHistory();
            actuator("end");
        }
        else {
            error("CIRCLE", tr("This should never happen."));
        }
    }
    else if (properties["mode"].s == MODE_TTR) {
        if (std::isnan(properties["x1)) {
            properties["x1 = x;
            properties["y1 = y;
            appendPromptHistory();
            setPromptPrefix(tr("Specify point on object for second tangent of circle: "));
        }
        else if (std::isnan(properties["x2)) {
            properties["x2 = x;
            properties["y2 = y;
            appendPromptHistory();
            setPromptPrefix(tr("Specify radius of circle: "));
        }
        else if (std::isnan(properties["x3)) {
            properties["x3 = x;
            properties["y3 = y;
            appendPromptHistory();
            setPromptPrefix(tr("Specify second point: "));
        }
        else {
            todo("CIRCLE", "click() for TTR");
        }
    }
}

/**
 * .
 */
void
Geometry::circle_context(String str)
{
    //todo("CIRCLE", "context()");
}

/**
 * .
 */
void
Geometry::circle_prompt(String str)
{
    if (properties["mode"].s == MODE_1P_RAD) {
        if (std::isnan(properties["x1)) {
            if (str == "2P") {
                properties["mode"].s = MODE_2P;
                setPromptPrefix(tr("Specify first end point of circle's diameter: "));
            }
            else if (str == "3P") {
                properties["mode"].s = MODE_3P;
                setPromptPrefix(tr("Specify first point of circle: "));
            }
            else if (str == "T" || str == "TTR") {
                properties["mode"].s = MODE_TTR;
                setPromptPrefix(tr("Specify point on object for first tangent of circle: "));
            }
            else {
                EmbReal strList = str.split(",");
                if (std::isnan(strList[0]) || std::isnan(strList[1])) {
                    alert(tr("Point or option keyword required."));
                    setPromptPrefix(tr("Specify center point for circle or [3P/2P/Ttr (tan tan radius)]: "));
                }
                else {
                    properties["x1 = node(strList[0]);
                    properties["y1 = node(strList[1]);
                    properties["center.x = properties["x1;
                    properties["center.y = properties["y1;
                    addRubber("CIRCLE");
                    setRubberMode("CIRCLE_1P_RAD");
                    setRubberPoint("CIRCLE_CENTER", properties["center.x, properties["center.y);
                    setPromptPrefix(tr("Specify radius of circle or [Diameter]: "));
                }
            }
        }
        else {
            if (str == "D" || str == "DIAMETER") {
                properties["mode"].s = MODE_1P_DIA;
                setRubberMode("CIRCLE_1P_DIA");
                setPromptPrefix(tr("Specify diameter of circle: "));
            }
            else {
                EmbReal num = node(str);
                if (std::isnan(num)) {
                    alert(tr("Requires numeric radius, point on circumference, or \"D\"."));
                    setPromptPrefix(tr("Specify radius of circle or [Diameter]: "));
                }
                else {
                    properties["rad = num;
                    properties["x2 = properties["x1 + properties["rad;
                    properties["y2 = properties["y1;
                    setRubberPoint("CIRCLE_RADIUS", properties["x2, properties["y2);
                    actuator("vulcanize");
                    actuator("end");
                }
            }
        }
    }
    else if (properties["mode"].s == MODE_1P_DIA) {
        if (std::isnan(properties["x1)) {
            error("CIRCLE", tr("This should never happen."));
        }
        if (std::isnan(properties["x2)) {
            EmbReal num = node(str);
            if (std::isnan(num)) {
                alert(tr("Requires numeric distance or second point."));
                setPromptPrefix(tr("Specify diameter of circle: "));
            }
            else {
                properties["dia = num;
                properties["x2 = properties["x1 + properties["dia;
                properties["y2 = properties["y1;
                setRubberPoint("CIRCLE_DIAMETER", properties["x2, properties["y2);
                actuator("vulcanize");
                actuator("end");
            }
        }
        else {
            error("CIRCLE", tr("This should never happen."));
        }
    }
    else if (properties["mode"].s == MODE_2P) {
        if (std::isnan(properties["x1)) {
            EmbReal strList = str.split(",");
            if (std::isnan(strList[0]) || std::isnan(strList[1])) {
                alert(tr("Invalid point."));
                setPromptPrefix(tr("Specify first end point of circle's diameter: "));
            }
            else {
                properties["x1 = node(strList[0]);
                properties["y1 = node(strList[1]);
                addRubber("CIRCLE");
                setRubberMode("CIRCLE_2P");
                setRubberPoint("CIRCLE_TAN1", properties["x1, properties["y1);
                setPromptPrefix(tr("Specify second end point of circle's diameter: "));
            }
        }
        else if (std::isnan(properties["x2)) {
            EmbReal strList = str.split(",");
            if (std::isnan(strList[0]) || std::isnan(strList[1])) {
                alert(tr("Invalid point."));
                setPromptPrefix(tr("Specify second end point of circle's diameter: "));
            }
            else {
                properties["x2 = node(strList[0]);
                properties["y2 = node(strList[1]);
                setRubberPoint("CIRCLE_TAN2", properties["x2, properties["y2);
                actuator("vulcanize");
                actuator("end");
            }
        }
        else {
            error("CIRCLE", tr("This should never happen."));
        }
    }
    else if (properties["mode"].s == MODE_3P) {
        if (std::isnan(properties["x1)) {
            EmbReal strList = str.split(",");
            if (std::isnan(strList[0]) || std::isnan(strList[1])) {
                alert(tr("Invalid point."));
                setPromptPrefix(tr("Specify first point of circle: "));
            }
            else {
                properties["x1 = node(strList[0]);
                properties["y1 = node(strList[1]);
                setPromptPrefix(tr("Specify second point of circle: "));
            }
        }
        else if (std::isnan(properties["x2)) {
            EmbReal strList = str.split(",");
            if (std::isnan(strList[0]) || std::isnan(strList[1])) {
                alert(tr("Invalid point."));
                setPromptPrefix(tr("Specify second point of circle: "));
            }
            else {
                properties["x2 = node(strList[0]);
                properties["y2 = node(strList[1]);
                addRubber("CIRCLE");
                setRubberMode("CIRCLE_3P");
                setRubberPoint("CIRCLE_TAN1", properties["x1, properties["y1);
                setRubberPoint("CIRCLE_TAN2", properties["x2, properties["y2);
                setPromptPrefix(tr("Specify third point of circle: "));
            }
        }
        else if (std::isnan(properties["x3)) {
            EmbReal strList = str.split(",");
            if (std::isnan(strList[0]) || std::isnan(strList[1])) {
                alert(tr("Invalid point."));
                setPromptPrefix(tr("Specify third point of circle: "));
            }
            else {
                properties["x3 = node(strList[0]);
                properties["y3 = node(strList[1]);
                setRubberPoint("CIRCLE_TAN3", properties["x3, properties["y3);
                actuator("vulcanize");
                actuator("end");
            }
        }
        else {
            error("CIRCLE", tr("This should never happen."));
        }
    }
    else if (properties["mode"].s == MODE_TTR) {
        todo("CIRCLE", "prompt() for TTR");
    }
}

/**
 * .
 */
void
Geometry::distance_main(void)
{
    initCommand();
    clearSelection();
    setPromptPrefix(tr("Specify first point: "));
}

/**
 * .
 */
void
Geometry::distance_click(EmbVector v)
{
    if (std::isnan(properties["x1)) {
        properties["x1 = x;
        properties["y1 = y;
        addRubber("LINE");
        setRubberMode("LINE");
        setRubberPoint("LINE_START", properties["x1, properties["y1);
        appendPromptHistory();
        setPromptPrefix(tr("Specify second point: "));
    }
    else {
        appendPromptHistory();
        properties["x2 = x;
        properties["y2 = y;
        reportDistance();
        actuator("end");
    }
}

/**
 * @brief distance_context
 * @param args
 * @return
 */
void
Geometry::distance_context(String args)
{
    //todo("DISTANCE", "context()");
}

/**
 * @brief distance_prompt
 * @param args
 * @return
 */
void
Geometry::distance_prompt(String args)
{
    EmbReal strList = str.split(",");
    if (std::isnan(properties["x1)) {
        if (std::isnan(strList[0]) || std::isnan(strList[1])) {
            alert(tr("Requires numeric distance or two points."));
            setPromptPrefix(tr("Specify first point: "));
        }
        else {
            properties["x1 = node(strList[0]);
            properties["y1 = node(strList[1]);
            addRubber("LINE");
            setRubberMode("LINE");
            setRubberPoint("LINE_START", properties["x1, properties["y1);
            setPromptPrefix(tr("Specify second point: "));
        }
    }
    else {
        if (std::isnan(strList[0]) || std::isnan(strList[1])) {
            alert(tr("Requires numeric distance or two points."));
            setPromptPrefix(tr("Specify second point: "));
        }
        else {
            properties["x2 = node(strList[0]);
            properties["y2 = node(strList[1]);
            reportDistance();
            actuator("end");
        }
    }
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
Geometry::reportDistance()
{
    EmbVector delta;
    EmbReal dx = properties["x2 - properties["x1;
    EmbReal dy = properties["y2 - properties["y1;

    EmbReal dist = calculateDistance(properties["x1, properties["y1, properties["x2, properties["y2);
    EmbReal angle = calculateAngle(properties["x1, properties["y1, properties["x2, properties["y2);

    setPromptPrefix(tr("Distance") + " = " + dist.toString() + ", " + tr("Angle") + " = " + angle.toString());
    appendPromptHistory();
    setPromptPrefix(tr("Delta X") + " = " + dx.toString() + ", " + tr("Delta Y") + " = " + dy.toString());
    appendPromptHistory();
}

/**
 *
 */
void
Geometry::dolphin_main(String args)
{
    initCommand();
    clearSelection();

    properties["numPoints"] = node(512); //Default //TODO: min:64 max:8192
    properties["center.x"] = node(0.0f);
    properties["center.y;
    properties["sx = 0.04; //Default
    properties["sy = 0.04; //Default
    properties["numPoints;
    properties["mode"].s;

    properties["center.x = NaN;
    properties["center.y = NaN;
    properties["mode"].s = MODE_NUM_POINTS;

    addRubber("POLYGON");
    setRubberMode("POLYGON");
    updateDolphin(properties["numPoints, properties["sx, properties["sy);
    spareRubber("POLYGON");
    actuator("end");
}

/**
 * @brief updateDolphin
 */
void
Geometry::update_dolphin(int numPoints, EmbReal xScale, EmbReal yScale)
{
    for (int i = 0; i <= numPoints; i++) {
        EmbReal t = (2.0 * emb_constant_pi) / numPoints*i;
        EmbVector v;
        v.x = fourier_series(t, dolphin_x);
        v.y = fourier_series(t, dolphin_y);

        setRubberPoint("POLYGON_POINT_" + i.toString(), xx*xScale, yy*yScale);
    }

    setRubberText("POLYGON_NUM_POINTS", numPoints.toString());
}

/**
 * @brief ellipse_main
 * @return
 */
void
Geometry::ellipse_main(void)
{
    properties["mode"].s = node("MODE_MAJORDIAMETER_MINORRADIUS");
    /*
    properties["width;
    properties["height;
    properties["rot;
    properties["mode"].s;
    initCommand();
    clearSelection();
    setPromptPrefix(tr("Specify first axis start point or [Center]: "));
    */
}

/**
 * .
 */
void
Geometry::ellipse_click(EmbVector v)
{
    if (properties["mode"].s == MODE_MAJORDIAMETER_MINORRADIUS) {
        if (std::isnan(properties["x1)) {
            properties["x1"] = node(v.x);
            properties["y1"] = node(v.y);
            addRubber("ELLIPSE");
            setRubberMode("ELLIPSE_LINE");
            setRubberPoint("ELLIPSE_LINE_POINT1", properties["x1, properties["y1);
            appendPromptHistory();
            setPromptPrefix(tr("Specify first axis end point: "));
        }
        else if (std::isnan(properties["x2)) {
            properties["x2"] = node(v.x);
            properties["y2"] = node(v.y);
            properties["center.x"] = node((properties["x1"].r + properties["x2"].r)/2.0);
            properties["center.y"] = node((properties["y1"].r + properties["y2"].r)/2.0);
            properties["width = calculateDistance(properties["x1, properties["y1, properties["x2, properties["y2);
            properties["rot = calculateAngle(properties["x1, properties["y1, properties["x2, properties["y2);
            setRubberMode("ELLIPSE_MAJORDIAMETER_MINORRADIUS");
            setRubberPoint("ELLIPSE_AXIS1_POINT1", properties["x1, properties["y1);
            setRubberPoint("ELLIPSE_AXIS1_POINT2", properties["x2, properties["y2);
            setRubberPoint("ELLIPSE_CENTER", properties["center.x, properties["center.y);
            setRubberPoint("ELLIPSE_WIDTH", properties["width, 0);
            setRubberPoint("ELLIPSE_ROT", properties["rot, 0);
            appendPromptHistory();
            setPromptPrefix(tr("Specify second axis end point or [Rotation]: "));
        }
        else if (std::isnan(properties["x3)) {
            properties["x3 = x;
            properties["y3 = y;
            properties["height = perpendicularDistance(properties["x3, properties["y3, properties["x1, properties["y1, properties["x2, properties["y2)*2.0;
            setRubberPoint("ELLIPSE_AXIS2_POINT2", properties["x3, properties["y3);
            actuator("vulcanize");
            appendPromptHistory();
            actuator("end");
        }
        else {
            error("ELLIPSE", tr("This should never happen."));
        }
    }
    else if (properties["mode"].s == MODE_MAJORRADIUS_MINORRADIUS) {
        if (std::isnan(properties["x1)) {
            properties["x1 = x;
            properties["y1 = y;
            properties["center.x = properties["x1;
            properties["center.y = properties["y1;
            addRubber("ELLIPSE");
            setRubberMode("ELLIPSE_LINE");
            setRubberPoint("ELLIPSE_LINE_POINT1", properties["x1, properties["y1);
            setRubberPoint("ELLIPSE_CENTER", properties["center.x, properties["center.y);
            appendPromptHistory();
            setPromptPrefix(tr("Specify first axis end point: "));
        }
        else if (std::isnan(properties["x2)) {
            properties["x2 = x;
            properties["y2 = y;
            properties["width = calculateDistance(properties["center.x, properties["center.y, properties["x2, properties["y2)*2.0;
            properties["rot = calculateAngle(properties["x1, properties["y1, properties["x2, properties["y2);
            setRubberMode("ELLIPSE_MAJORRADIUS_MINORRADIUS");
            setRubberPoint("ELLIPSE_AXIS1_POINT2", properties["x2, properties["y2);
            setRubberPoint("ELLIPSE_WIDTH", properties["width, 0);
            setRubberPoint("ELLIPSE_ROT", properties["rot, 0);
            appendPromptHistory();
            setPromptPrefix(tr("Specify second axis end point or [Rotation]: "));
        }
        else if (std::isnan(properties["x3)) {
            properties["x3 = x;
            properties["y3 = y;
            properties["height = perpendicularDistance(properties["x3, properties["y3, properties["center.x, properties["center.y, properties["x2, properties["y2)*2.0;
            setRubberPoint("ELLIPSE_AXIS2_POINT2", properties["x3, properties["y3);
            actuator("vulcanize");
            appendPromptHistory();
            actuator("end");
        }
        else {
            error("ELLIPSE", tr("This should never happen."));
        }
    }
    else if (properties["mode"].s == MODE_ELLIPSE_ROTATION) {
        if (std::isnan(properties["x1)) {
            error("ELLIPSE", tr("This should never happen."));
        }
        else if (std::isnan(properties["x2)) {
            error("ELLIPSE", tr("This should never happen."));
        }
        else if (std::isnan(properties["x3)) {
            EmbReal angle = calculateAngle(properties["center.x, properties["center.y, x, y);
            properties["height = cos(angle*PI/180.0)*properties["width;
            addEllipse(properties["center.x, properties["center.y, properties["width, properties["height, properties["rot, false);
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
Geometry::ellipse_context(String args)
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
Geometry::ellipse_prompt(String args)
{
    if (properties["mode"].s == MODE_MAJORDIAMETER_MINORRADIUS) {
        if (std::isnan(properties["x1)) {
            if (str == "C" || str == "CENTER") {
                properties["mode"].s = MODE_MAJORRADIUS_MINORRADIUS;
                setPromptPrefix(tr("Specify center point: "));
            }
            else {
                EmbReal strList = str.split(",");
                if (std::isnan(strList[0]) || std::isnan(strList[1])) {
                    alert(tr("Point or option keyword required."));
                    setPromptPrefix(tr("Specify first axis start point or [Center]: "));
                }
                else {
                    properties["x1 = node(strList[0]);
                    properties["y1 = node(strList[1]);
                    addRubber("ELLIPSE");
                    setRubberMode("ELLIPSE_LINE");
                    setRubberPoint("ELLIPSE_LINE_POINT1", properties["x1, properties["y1);
                    setPromptPrefix(tr("Specify first axis end point: "));
                }
            }
        }
        else if (std::isnan(properties["x2)) {
            EmbReal strList = str.split(",");
            if (std::isnan(strList[0]) || std::isnan(strList[1])) {
                alert(tr("Invalid point."));
                setPromptPrefix(tr("Specify first axis end point: "));
            }
            else {
                properties["x2 = node(strList[0]);
                properties["y2 = node(strList[1]);
                properties["center.x = (properties["x1 + properties["x2)/2.0;
                properties["center.y = (properties["y1 + properties["y2)/2.0;
                properties["width = calculateDistance(properties["x1, properties["y1, properties["x2, properties["y2);
                properties["rot = calculateAngle(properties["x1, properties["y1, properties["x2, properties["y2);
                setRubberMode("ELLIPSE_MAJORDIAMETER_MINORRADIUS");
                setRubberPoint("ELLIPSE_AXIS1_POINT1", properties["x1, properties["y1);
                setRubberPoint("ELLIPSE_AXIS1_POINT2", properties["x2, properties["y2);
                setRubberPoint("ELLIPSE_CENTER", properties["center.x, properties["center.y);
                setRubberPoint("ELLIPSE_WIDTH", properties["width, 0);
                setRubberPoint("ELLIPSE_ROT", properties["rot, 0);
                setPromptPrefix(tr("Specify second axis end point or [Rotation]: "));
            }
        }
        else if (std::isnan(properties["x3)) {
            if (str == "R" || str == "ROTATION") {
                properties["mode"].s = MODE_ELLIPSE_ROTATION;
                setPromptPrefix(tr("Specify rotation: "));
            }
            else {
                EmbReal strList = str.split(",");
                if (std::isnan(strList[0]) || std::isnan(strList[1])) {
                    alert(tr("Point or option keyword required."));
                    setPromptPrefix(tr("Specify second axis end point or [Rotation]: "));
                }
                else {
                    properties["x3 = node(strList[0]);
                    properties["y3 = node(strList[1]);
                    properties["height = perpendicularDistance(properties["x3, properties["y3, properties["x1, properties["y1, properties["x2, properties["y2)*2.0;
                    setRubberPoint("ELLIPSE_AXIS2_POINT2", properties["x3, properties["y3);
                    actuator("vulcanize");
                    actuator("end");
                }
            }
        }
    }
    else if (properties["mode"].s == MODE_MAJORRADIUS_MINORRADIUS) {
        if (std::isnan(properties["x1)) {
            EmbReal strList = str.split(",");
            if (std::isnan(strList[0]) || std::isnan(strList[1])) {
                alert(tr("Invalid point."));
                setPromptPrefix(tr("Specify center point: "));
            }
            else {
                properties["x1 = node(strList[0]);
                properties["y1 = node(strList[1]);
                properties["center.x = properties["x1;
                properties["center.y = properties["y1;
                addRubber("ELLIPSE");
                setRubberMode("ELLIPSE_LINE");
                setRubberPoint("ELLIPSE_LINE_POINT1", properties["x1, properties["y1);
                setRubberPoint("ELLIPSE_CENTER", properties["center.x, properties["center.y);
                setPromptPrefix(tr("Specify first axis end point: "));
            }
        }
        else if (std::isnan(properties["x2)) {
            EmbReal strList = str.split(",");
            if (std::isnan(strList[0]) || std::isnan(strList[1])) {
                alert(tr("Invalid point."));
                setPromptPrefix(tr("Specify first axis end point: "));
            }
            else {
                properties["x2 = node(strList[0]);
                properties["y2 = node(strList[1]);
                properties["width = calculateDistance(properties["x1, properties["y1, properties["x2, properties["y2)*2.0;
                properties["rot = calculateAngle(properties["x1, properties["y1, properties["x2, properties["y2);
                setRubberMode("ELLIPSE_MAJORRADIUS_MINORRADIUS");
                setRubberPoint("ELLIPSE_AXIS1_POINT2", properties["x2, properties["y2);
                setRubberPoint("ELLIPSE_WIDTH", properties["width, 0);
                setRubberPoint("ELLIPSE_ROT", properties["rot, 0);
                setPromptPrefix(tr("Specify second axis end point or [Rotation]: "));
            }
        }
        else if (std::isnan(properties["x3)) {
            if (str == "R" || str == "ROTATION") {
                properties["mode"].s = MODE_ELLIPSE_ROTATION;
                setPromptPrefix(tr("Specify ellipse rotation: "));
            }
            else {
                EmbReal strList = str.split(",");
                if (std::isnan(strList[0]) || std::isnan(strList[1])) {
                    alert(tr("Point or option keyword required."));
                    setPromptPrefix(tr("Specify second axis end point or [Rotation]: "));
                }
                else {
                    properties["x3 = node(strList[0]);
                    properties["y3 = node(strList[1]);
                    properties["height = perpendicularDistance(properties["x3, properties["y3, properties["x1, properties["y1, properties["x2, properties["y2)*2.0;
                    setRubberPoint("ELLIPSE_AXIS2_POINT2", properties["x3, properties["y3);
                    actuator("vulcanize");
                    actuator("end");
                }
            }
        }
    }
    else if (properties["mode"].s == MODE_ELLIPSE_ROTATION) {
        if (std::isnan(properties["x1)) {
            error("ELLIPSE", tr("This should never happen."));
        }
        else if (std::isnan(properties["x2)) {
            error("ELLIPSE", tr("This should never happen."));
        }
        else if (std::isnan(properties["x3)) {
            if (std::isnan(str)) {
                alert(tr("Invalid angle. Input a numeric angle or pick a point."));
                setPromptPrefix(tr("Specify rotation: "));
            }
            else {
                EmbReal angle = node(str);
                properties["height = cos(angle*PI/180.0)*properties["width;
                addEllipse(properties["center.x, properties["center.y, properties["width, properties["height, properties["rot, false);
                actuator("end");
            }
        }
    }
}

void
Geometry::erase_main(void)
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
Geometry::heart_main(void)
{
    properties["numPoints = 512; //Default //TODO: min:64 max:8192
    properties["center.x;
    properties["center.y;
    properties["sx = 1.0;
    properties["sy = 1.0;
    properties["numPoints;
    properties["mode"].s;
    initCommand();
    clearSelection();
    properties["center.x = NaN;
    properties["center.y = NaN;
    properties["mode"].s = MODE_NUM_POINTS;

    //Heart4: 10.0 / 512
    //Heart5: 1.0 / 512

    addRubber("POLYGON");
    setRubberMode("POLYGON");
    updateHeart("HEART5", properties["numPoints, properties["sx, properties["sy);
    spareRubber("POLYGON");
    actuator("end");
}

/**
 * .
 */
void
Geometry::updateHeart(style, numPoints, xScale, yScale)
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
Geometry::line_main(void)
{
    initCommand();
    clearSelection();
    properties["firstRun = true;
    properties["first = {NaN, NaN};
    properties["prev = {NaN, NaN};
    setPromptPrefix(tr("Specify first point: "));
}


void
Geometry::line_click(x, y)
{
    if (properties["firstRun) {
        properties["firstRun = false;
        properties["firstX = x;
        properties["firstY = y;
        properties["prevX = x;
        properties["prevY = y;
        addRubber("LINE");
        setRubberMode("LINE");
        setRubberPoint("LINE_START", properties["firstX, properties["firstY);
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
        properties["prevX = x;
        properties["prevY = y;
    }
}

void
Geometry::line_context(String str)
{
    todo("LINE", "context()");
}

void
Geometry::line_prompt(String args)
{
    if (properties["firstRun) {
        EmbReal strList = str.split(",");
        if (std::isnan(strList[0]) || std::isnan(strList[1])) {
            alert(tr("Invalid point."));
            setPromptPrefix(tr("Specify first point: "));
        }
        else {
            properties["firstRun = false;
            properties["firstX = node(strList[0]);
            properties["firstY = node(strList[1]);
            properties["prevX = properties["firstX;
            properties["prevY = properties["firstY;
            addRubber("LINE");
            setRubberMode("LINE");
            setRubberPoint("LINE_START", properties["firstX, properties["firstY);
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
                EmbReal x = node(strList[0]);
                EmbReal y = node(strList[1]);
                setRubberPoint("LINE_END", x, y);
                actuator("vulcanize");
                addRubber("LINE");
                setRubberMode("LINE");
                setRubberPoint("LINE_START", x, y);
                properties["prevX = x;
                properties["prevY = y;
                setPromptPrefix(tr("Specify next point or [Undo]: "));
            }
        }
    }
}

/**
 * @brief locate_point_main
 * @return
 */
void
Geometry::locate_point_main(void)
{
    initCommand();
    clearSelection();
    setPromptPrefix(tr("Specify point: "));
}

/**
 * @brief locate_point_click
 * @param properties
 * @param v
 * @return
 */
void
Geometry::locate_point_click(EmbVector v)
{
    appendPromptHistory();
    setPromptPrefix("X = " + v.x + ", Y = " + v.y);
    appendPromptHistory();
    actuator("end");
}

void
Geometry::locate_point_context(String str)
{
    //todo("LOCATEPOINT", "context()");
}

void
Geometry::locate_point_prompt(String args)
{
    StringList strList = tokenize(args, ',');
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
}

/**
 * .
 */
void
Geometry::move_main(void)
{
    initCommand();
    properties["firstRun = true;
    properties["baseX  = NaN;
    properties["baseY  = NaN;
    properties["destX  = NaN;
    properties["destY  = NaN;
    properties["deltaX = NaN;
    properties["deltaY = NaN;

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

/**
 * .
 */
void
Geometry::move_click(x, y)
{
    if (properties["firstRun) {
        properties["firstRun = false;
        properties["baseX = x;
        properties["baseY = y;
        addRubber("LINE");
        setRubberMode("LINE");
        setRubberPoint("LINE_START", properties["baseX, properties["baseY);
        previewOn("SELECTED", "MOVE", properties["baseX, properties["baseY, 0);
        appendPromptHistory();
        setPromptPrefix(tr("Specify destination point: "));
    }
    else {
        properties["destX = x;
        properties["destY = y;
        properties["deltaX = properties["destX - properties["baseX;
        properties["deltaY = properties["destY - properties["baseY;
        moveSelected(properties["deltaX, properties["deltaY);
        previewOff();
        actuator("end");
    }
}
*/

/**
 * .
 */
void
Geometry::move_context(String str)
{
    // todo("MOVE", "context()");
}

/**
 * .
 */
void
Geometry::move_prompt(String str)
{
    if (properties["firstRun) {
        EmbReal strList = str.split(",");
        if (std::isnan(strList[0]) || std::isnan(strList[1])) {
            alert(tr("Invalid point."));
            setPromptPrefix(tr("Specify base point: "));
        }
        else {
            properties["firstRun = false;
            properties["baseX = node(strList[0]);
            properties["baseY = node(strList[1]);
            addRubber("LINE");
            setRubberMode("LINE");
            setRubberPoint("LINE_START", properties["baseX, properties["baseY);
            previewOn("SELECTED", "MOVE", properties["baseX, properties["baseY, 0);
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
            properties["destX = node(strList[0]);
            properties["destY = node(strList[1]);
            properties["deltaX = properties["destX - properties["baseX;
            properties["deltaY = properties["destY - properties["baseY;
            moveSelected(properties["deltaX, properties["deltaY);
            previewOff();
            actuator("end");
        }
    }
}

/*
//TODO: The path command is currently broken

void
Geometry::path_main(void)
{
    properties["firstRun;
    properties["first;
    properties["prev;
    initCommand();
    clearSelection();
    properties["firstRun"] = node(true);
    setPromptPrefix(tr("Specify start point: "));
}


void
Geometry::path_click(x, y)
{
    if (properties["firstRun) {
        properties["firstRun = false;
        properties["firstX = x;
        properties["firstY = y;
        properties["prevX = x;
        properties["prevY = y;
        addPath(x,y);
        appendPromptHistory();
        setPromptPrefix(tr("Specify next point or [Arc/Undo]: "));
    }
    else {
        appendPromptHistory();
        appendLineToPath(x,y);
        properties["prevX = x;
        properties["prevY = y;
    }
}

void
Geometry::path_context(String str)
{
    todo("PATH", "context()");
}

void
Geometry::path_prompt(String args)
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
            EmbReal x = node(strList[0]);
            EmbReal y = node(strList[1]);
            if (properties["firstRun) {
                properties["firstRun = false;
                properties["firstX = x;
                properties["firstY = y;
                properties["prevX = x;
                properties["prevY = y;
                addPath(x,y);
                setPromptPrefix(tr("Specify next point or [Arc/Undo]: "));
            }
            else {
                appendLineToPath(x,y);
                properties["prevX = x;
                properties["prevY = y;
            }
        }
    }
}

void
Geometry::point_main(void)
{
    initCommand();
    clearSelection();
    properties["firstRun = true;
    setPromptPrefix("TODO: Current point settings: PDMODE=?  PDSIZE=?"); //TODO: tr needed here when complete
    appendPromptHistory();
    setPromptPrefix(tr("Specify first point: "));
}


void
Geometry::point_click(EmbVector v)
{
    if (properties["firstRun) {
        properties["firstRun = false;
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
Geometry::point_context(String str)
{
    todo("POINT", "context()");
}

void
Geometry::point_prompt(String str)
{
    if (properties["firstRun) {
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
            properties["firstRun = false;
            EmbReal x = node(strList[0]);
            EmbReal y = node(strList[1]);
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
            EmbReal x = node(strList[0]);
            EmbReal y = node(strList[1]);
            setPromptPrefix(tr("Specify next point: "));
            addPoint(x,y);
        }
    }
}

/**
 * .
 */
void
Geometry::polygon_main(void)
{
    initCommand();
    clearSelection();
    properties["centerX = NaN;
    properties["centerY = NaN;
    properties["sideX1  = NaN;
    properties["sideY1  = NaN;
    properties["sideX2  = NaN;
    properties["sideY2  = NaN;
    properties["pointIX = NaN;
    properties["pointIY = NaN;
    properties["pointCX = NaN;
    properties["pointCY = NaN;
    properties["polyType = "Inscribed"; //Default
    properties["numSides = 4;           //Default
    properties["mode"].s = MODE_NUM_SIDES;
    setPromptPrefix(tr("Enter number of sides") + " {" + properties["numSides.toString() + "}: ");
}

/**
 * .
 */
void
Geometry::polygon_click(EmbVector v)
{
    if (properties["mode"].s == MODE_NUM_SIDES) {
        //Do nothing, the prompt controls this.
    }
    else if (properties["mode"].s == MODE_CENTER_PT) {
        properties["centerX = x;
        properties["centerY = y;
        properties["mode"].s = MODE_POLYTYPE;
        appendPromptHistory();
        setPromptPrefix(tr("Specify polygon type [Inscribed in circle/Circumscribed around circle]") + " {" + properties["polyType + "}: ");
    }
    else if (properties["mode"].s == MODE_POLYTYPE) {
        //Do nothing, the prompt controls this.
    }
    else if (properties["mode"].s == MODE_INSCRIBE) {
        properties["pointIX = x;
        properties["pointIY = y;
        setRubberPoint("POLYGON_INSCRIBE_POINT", properties["pointIX, properties["pointIY);
        actuator("vulcanize");
        appendPromptHistory();
        actuator("end");
    }
    else if (properties["mode"].s == MODE_CIRCUMSCRIBE) {
        properties["pointCX = x;
        properties["pointCY = y;
        setRubberPoint("POLYGON_CIRCUMSCRIBE_POINT", properties["pointCX, properties["pointCY);
        actuator("vulcanize");
        appendPromptHistory();
        actuator("end");
    }
    else if (properties["mode"].s == MODE_DISTANCE) {
        //Do nothing, the prompt controls this.
    }
    else if (properties["mode"].s == MODE_SIDE_LEN) {
        todo("POLYGON", "Sidelength mode");
    }
}

/**
 * .
 */
void
Geometry::polygon_context(String str)
{
    //todo("POLYGON", "context()");
}

/**
 * .
 */
void
Geometry::polygon_prompt(String str)
{
    if (properties["mode"].s == MODE_NUM_SIDES) {
        if (str == "" && properties["numSides >= 3 && properties["numSides <= 1024) {
            setPromptPrefix(tr("Specify center point or [Sidelength]: "));
            properties["mode"].s = MODE_CENTER_PT;
        }
        else {
            EmbReal tmp = node(str);
            if (std::isnan(tmp) || !isInt(tmp) || tmp < 3 || tmp > 1024) {
                alert(tr("Requires an integer between 3 and 1024."));
                setPromptPrefix(tr("Enter number of sides") + " {" + properties["numSides.toString() + "}: ");
            }
            else {
                properties["numSides = tmp;
                setPromptPrefix(tr("Specify center point or [Sidelength]: "));
                properties["mode"].s = MODE_CENTER_PT;
            }
        }
    }
    else if (properties["mode"].s == MODE_CENTER_PT) {
        if (str == "S" || str == "SIDELENGTH") {
            properties["mode"].s = MODE_SIDE_LEN;
            setPromptPrefix(tr("Specify start point: "));
        }
        else {
            EmbReal strList = str.split(",");
            if (std::isnan(strList[0]) || std::isnan(strList[1])) {
                alert(tr("Point or option keyword required."));
                setPromptPrefix(tr("Specify center point or [Sidelength]: "));
            }
            else {
                properties["centerX = node(strList[0]);
                properties["centerY = node(strList[1]);
                properties["mode"].s = MODE_POLYTYPE;
                setPromptPrefix(tr("Specify polygon type [Inscribed in circle/Circumscribed around circle]") + " {" + properties["polyType + "}: ");
            }
        }
    }
    else if (properties["mode"].s == MODE_POLYTYPE) {
        if (str == "INSCRIBED") {
            properties["mode"].s = MODE_INSCRIBE;
            properties["polyType = "Inscribed";
            setPromptPrefix(tr("Specify polygon corner point or [Distance]: "));
            addRubber("POLYGON");
            setRubberMode("POLYGON_INSCRIBE");
            setRubberPoint("POLYGON_CENTER", properties["centerX, properties["centerY);
            setRubberPoint("POLYGON_NUM_SIDES", properties["numSides, 0);
        }
        else if (str == "CIRCUMSCRIBED") {
            properties["mode"].s = MODE_CIRCUMSCRIBE;
            properties["polyType = "Circumscribed";
            setPromptPrefix(tr("Specify polygon side point or [Distance]: "));
            addRubber("POLYGON");
            setRubberMode("POLYGON_CIRCUMSCRIBE");
            setRubberPoint("POLYGON_CENTER", properties["centerX, properties["centerY);
            setRubberPoint("POLYGON_NUM_SIDES", properties["numSides, 0);
        }
        else if (str == "") {
            if (properties["polyType == "Inscribed") {
                properties["mode"].s = MODE_INSCRIBE;
                setPromptPrefix(tr("Specify polygon corner point or [Distance]: "));
                addRubber("POLYGON");
                setRubberMode("POLYGON_INSCRIBE");
                setRubberPoint("POLYGON_CENTER", properties["centerX, properties["centerY);
                setRubberPoint("POLYGON_NUM_SIDES", properties["numSides, 0);
            }
            else if (properties["polyType == "Circumscribed") {
                properties["mode"].s = MODE_CIRCUMSCRIBE;
                setPromptPrefix(tr("Specify polygon side point or [Distance]: "));
                addRubber("POLYGON");
                setRubberMode("POLYGON_CIRCUMSCRIBE");
                setRubberPoint("POLYGON_CENTER", properties["centerX, properties["centerY);
                setRubberPoint("POLYGON_NUM_SIDES", properties["numSides, 0);
            }
            else {
                error("POLYGON", tr("Polygon type is not Inscribed or Circumscribed."));
            }
        }
        else {
            alert(tr("Invalid option keyword."));
            setPromptPrefix(tr("Specify polygon type [Inscribed in circle/Circumscribed around circle]") + " {" + properties["polyType + "}: ");
        }
    }
    else if (properties["mode"].s == MODE_INSCRIBE) {
        if (str == "D" || str == "DISTANCE") {
            properties["mode"].s = MODE_DISTANCE;
            setPromptPrefix(tr("Specify distance: "));
        }
        else {
            EmbReal strList = str.split(",");
            if (std::isnan(strList[0]) || std::isnan(strList[1])) {
                alert(tr("Point or option keyword required."));
                setPromptPrefix(tr("Specify polygon corner point or [Distance]: "));
            }
            else {
                properties["pointIX = node(strList[0]);
                properties["pointIY = node(strList[1]);
                setRubberPoint("POLYGON_INSCRIBE_POINT", properties["pointIX, properties["pointIY);
                actuator("vulcanize");
                actuator("end");
            }
        }
    }
    else if (properties["mode"].s == MODE_CIRCUMSCRIBE) {
        if (str == "D" || str == "DISTANCE") {
            properties["mode"].s = MODE_DISTANCE;
            setPromptPrefix(tr("Specify distance: "));
        }
        else {
            EmbReal strList = str.split(",");
            if (std::isnan(strList[0]) || std::isnan(strList[1])) {
                alert(tr("Point or option keyword required."));
                setPromptPrefix(tr("Specify polygon side point or [Distance]: "));
            }
            else {
                properties["pointCX = node(strList[0]);
                properties["pointCY = node(strList[1]);
                setRubberPoint("POLYGON_CIRCUMSCRIBE_POINT", properties["pointCX, properties["pointCY);
                actuator("vulcanize");
                actuator("end");
            }
        }
    }
    else if (properties["mode"].s == MODE_DISTANCE) {
        if (std::isnan(str)) {
            alert(tr("Requires valid numeric distance."));
            setPromptPrefix(tr("Specify distance: "));
        }
        else {
            if (properties["polyType == "Inscribed") {
                properties["pointIX = properties["centerX;
                properties["pointIY = properties["centerY + node(str);
                setRubberPoint("POLYGON_INSCRIBE_POINT", properties["pointIX, properties["pointIY);
                actuator("vulcanize");
                actuator("end");
            }
            else if (properties["polyType == "Circumscribed") {
                properties["pointCX = properties["centerX;
                properties["pointCY = properties["centerY + node(str);
                setRubberPoint("POLYGON_CIRCUMSCRIBE_POINT", properties["pointCX, properties["pointCY);
                actuator("vulcanize");
                actuator("end");
            }
            else {
                error("POLYGON", tr("Polygon type is not Inscribed or Circumscribed."));
            }
        }
    }
    else if (properties["mode"].s == MODE_SIDE_LEN) {
        todo("POLYGON", "Sidelength mode");
    }
}

/**
 * .
 */
void
Geometry::polyline_main(void)
{
    initCommand();
    clearSelection();
    properties["firstRun = true;
    properties["firstX = NaN;
    properties["firstY = NaN;
    properties["prevX = NaN;
    properties["prevY = NaN;
    properties["num = 0;
    setPromptPrefix(tr("Specify first point: "));
}

/**
 * .
 */
void
Geometry::polyline_click(EmbVector v)
{
    if (properties["firstRun"].b) {
        properties["firstRun = false;
        properties["firstX = x;
        properties["firstY = y;
        properties["prevX = x;
        properties["prevY = y;
        addRubber("POLYLINE");
        setRubberMode("POLYLINE");
        setRubberPoint("POLYLINE_POINT_0", properties["firstX, properties["firstY);
        appendPromptHistory();
        setPromptPrefix(tr("Specify next point or [Undo]: "));
    }
    else {
        properties["num++;
        setRubberPoint("POLYLINE_POINT_" + properties["num.toString(), x, y);
        setRubberText("POLYLINE_NUM_POINTS", properties["num.toString());
        spareRubber("POLYLINE");
        appendPromptHistory();
        properties["prevX = x;
        properties["prevY = y;
    }
}

/**
 * .
 */
void
Geometry::polyline_context(String str)
{
    //todo("POLYLINE", "context()");
}

/**
 * .
 */
void
Geometry::polyline_prompt(String str)
{
    if (properties["firstRun"].b) {
        EmbReal strList = str.split(",");
        if (std::isnan(strList[0]) || std::isnan(strList[1])) {
            alert(tr("Invalid point."));
            setPromptPrefix(tr("Specify first point: "));
        }
        else {
            properties["firstRun = false;
            properties["firstX = node(strList[0]);
            properties["firstY = node(strList[1]);
            properties["prevX = properties["firstX;
            properties["prevY = properties["firstY;
            addRubber("POLYLINE");
            setRubberMode("POLYLINE");
            setRubberPoint("POLYLINE_POINT_0", properties["firstX, properties["firstY);
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
                EmbReal x = node(strList[0]);
                EmbReal y = node(strList[1]);
                properties["num++;
                setRubberPoint("POLYLINE_POINT_" + properties["num.toString(), x, y);
                setRubberText("POLYLINE_NUM_POINTS", properties["num.toString());
                spareRubber("POLYLINE");
                properties["prevX = x;
                properties["prevY = y;
                setPromptPrefix(tr("Specify next point or [Undo]: "));
            }
        }
    }
}

/**
 * .
 * \TODO: Adding the text is not complete yet.
 */
void
Geometry::quickleader_main(void)
{
    initCommand();
    clearSelection();
    properties["x1 = NaN;
    properties["y1 = NaN;
    properties["x2 = NaN;
    properties["y2 = NaN;
    setPromptPrefix(tr("Specify first point: "));
}

/**
 * .
 */
void
Geometry::quickleader_click(EmbVector v)
{
    if (std::isnan(properties["x1)) {
        properties["x1 = x;
        properties["y1 = y;
        addRubber("DIMLEADER");
        setRubberMode("DIMLEADER_LINE");
        setRubberPoint("DIMLEADER_LINE_START", properties["x1, properties["y1);
        appendPromptHistory();
        setPromptPrefix(tr("Specify second point: "));
    }
    else {
        properties["x2 = x;
        properties["y2 = y;
        setRubberPoint("DIMLEADER_LINE_END", properties["x2, properties["y2);
        actuator("vulcanize");
        appendPromptHistory();
        actuator("end");
    }
}

/**
 * .
 */
void
Geometry::quickleader_context(String str)
{
    //todo("QUICKLEADER", "context()");
}

/**
 * .
 */
void
Geometry::quickleader_prompt(String str)
{
    EmbReal strList = str.split(",");
    if (std::isnan(properties["x1)) {
        if (std::isnan(strList[0]) || std::isnan(strList[1])) {
            alert(tr("Requires two points."));
            setPromptPrefix(tr("Specify first point: "));
        }
        else {
            properties["x1 = node(strList[0]);
            properties["y1 = node(strList[1]);
            addRubber("DIMLEADER");
            setRubberMode("DIMLEADER_LINE");
            setRubberPoint("DIMLEADER_LINE_START", properties["x1, properties["y1);
            setPromptPrefix(tr("Specify second point: "));
        }
    }
    else {
        if (std::isnan(strList[0]) || std::isnan(strList[1])) {
            alert(tr("Requires two points."));
            setPromptPrefix(tr("Specify second point: "));
        }
        else {
            properties["x2 = node(strList[0]);
            properties["y2 = node(strList[1]);
            setRubberPoint("DIMLEADER_LINE_END", properties["x2, properties["y2);
            actuator("vulcanize");
            actuator("end");
        }
    }
}

/**
 * .
 */
void
Geometry::rectangle_main(void)
{
    initCommand();
    clearSelection();
    properties["newRect = true;
    properties["x1 = NaN;
    properties["y1 = NaN;
    properties["x2 = NaN;
    properties["y2 = NaN;
    setPromptPrefix(tr("Specify first corner point or [Chamfer/Fillet]: "));
}

/**
 * .
 */
void
Geometry::rectangle_click(EmbVector v)
{
    if (properties["newRect) {
        properties["newRect = false;
        properties["x1 = x;
        properties["y1 = y;
        addRubber("RECTANGLE");
        setRubberMode("RECTANGLE");
        setRubberPoint("RECTANGLE_START", x, y);
        setPromptPrefix(tr("Specify other corner point or [Dimensions]: "));
    }
    else {
        properties["newRect = true;
        properties["x2 = x;
        properties["y2 = y;
        setRubberPoint("RECTANGLE_END", x, y);
        actuator("vulcanize");
        actuator("end");
    }
}

/**
 * .
 */
void
Geometry::rectangle_context(String str)
{
    //todo("RECTANGLE", "context()");
}

/**
 * .
 */
void
Geometry::rectangle_prompt(String str)
{
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
            EmbReal x = node(strList[0]);
            EmbReal y = node(strList[1]);
            if (properties["newRect) {
                properties["newRect = false;
                properties["x1 = x;
                properties["y1 = y;
                addRubber("RECTANGLE");
                setRubberMode("RECTANGLE");
                setRubberPoint("RECTANGLE_START", x, y);
                setPromptPrefix(tr("Specify other corner point or [Dimensions]: "));
            }
            else {
                properties["newRect = true;
                properties["x2 = x;
                properties["y2 = y;
                setRubberPoint("RECTANGLE_END", x, y);
                actuator("vulcanize");
                actuator("end");
            }
        }
    }
}

/**
 * .
 */
void
Geometry::rgb_main(void)
{
    initCommand();
    clearSelection();
    properties["mode"] = node("RGB_MODE_BACKGROUND");
    setPromptPrefix(tr("Enter RED,GREEN,BLUE values for background or [Crosshair/Grid]: "));
}

/**
 * .
 */
void
Geometry::rgb_click(EmbVector v)
{
    //Do Nothing, prompt only command.
}

/**
 * .
 */
void
Geometry::rgb_context(String str)
{
    //todo("RGB", "context()");
}

/**
 * .
 */
void
Geometry::rgb_prompt(String str)
{
    if (properties["mode"].s == "RGB_MODE_BACKGROUND") {
        if (str == "C" || str == "CROSSHAIR") {
            properties["mode"].s = node("RGB_MODE_CROSSHAIR");
            setPromptPrefix(tr("Specify crosshair color: "));
        }
        else if (str == "G" || str == "GRID") {
            properties["mode"].s = RGB_MODE_GRID;
            setPromptPrefix(tr("Specify grid color: "));
        }
        else {
            EmbReal strList = str.split(",");
            EmbReal r = node(strList[0]);
            EmbReal g = node(strList[1]);
            EmbReal b = node(strList[2]);
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
    else if (properties["mode"].s == RGB_MODE_CROSSHAIR) {
        EmbReal strList = str.split(",");
        EmbReal r = node(strList[0]);
        EmbReal g = node(strList[1]);
        EmbReal b = node(strList[2]);
        if (!validRGB(r,g,b)) {
            alert(tr("Invalid color. R,G,B values must be in the range of 0-255."));
            setPromptPrefix(tr("Specify crosshair color: "));
        }
        else {
            setCrossHairColor(r,g,b);
            actuator("end");
        }
    }
    else if (properties["mode"].s == RGB_MODE_GRID) {
        EmbReal strList = str.split(",");
        EmbReal r = node(strList[0]);
        EmbReal g = node(strList[1]);
        EmbReal b = node(strList[2]);
        if (!validRGB(r,g,b)) {
            alert(tr("Invalid color. R,G,B values must be in the range of 0-255."));
            setPromptPrefix(tr("Specify grid color: "));
        }
        else {
            setGridColor(r,g,b);
            actuator("end");
        }
    }
}

/**
 * .
 * Command: Rotate
 *
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
void
Geometry::rotate_main(String args)
{
    initCommand();
    properties["mode"].s = ROTATE_MODE_NORMAL;
    properties["firstRun = true;
    properties["base = {NaN, NaN};
    properties["dest = {NaN, NaN};
    properties["angle = NaN;

    properties["baseR = {NaN, NaN};
    properties["destR = {NaN, NaN};
    properties["angleRef = NaN;
    properties["angleNew = NaN;

    if (numSelected() <= 0) {
        //TODO: Prompt to select objects if nothing is preselected
        alert(tr("Preselect objects before invoking the rotate command."));
        actuator("end");
        messageBox("information", tr("Rotate Preselect"), tr("Preselect objects before invoking the rotate command."));
    }
    else {
        setPromptPrefix(tr("Specify base point: "));
    }
}

/**
 * .
 */
void
Geometry::rotate_click(EmbVector v)
{
    /*
    if (properties["mode"].s == ROTATE_MODE_NORMAL) {
        if (properties["firstRun) {
            properties["firstRun = false;
            properties["base = v;
            addRubber("LINE");
            setRubberMode("LINE");
            setRubberPoint("LINE_START", properties["baseX, properties["baseY);
            previewOn("SELECTED", "ROTATE", properties["baseX, properties["baseY, 0);
            appendPromptHistory();
            setPromptPrefix(tr("Specify rotation angle or [Reference]: "));
        }
        else {
            properties["dest = v;
            properties["angle = calculateAngle(properties["baseX, properties["baseY, properties["destX, properties["destY);
            appendPromptHistory();
            rotateSelected(properties["baseX, properties["baseY, properties["angle);
            previewOff();
            actuator("end");
        }
    }
    else if (properties["mode"].s == ROTATE_MODE_REFERENCE) {
        if (std::isnan(properties["baseRX)) {
            properties["baseR = v;
            appendPromptHistory();
            addRubber("LINE");
            setRubberMode("LINE");
            setRubberPoint("LINE_START", properties["baseRX, properties["baseRY);
            setPromptPrefix(tr("Specify second point: "));
        }
        else if (std::isnan(properties["destRX)) {
            properties["destR = v;
            properties["angleRef = calculateAngle(properties["baseRX, properties["baseRY, properties["destRX, properties["destRY);
            setRubberPoint("LINE_START", properties["baseX, properties["baseY);
            previewOn("SELECTED", "ROTATE", properties["baseX, properties["baseY, properties["angleRef);
            appendPromptHistory();
            setPromptPrefix(tr("Specify the new angle: "));
        }
        else if (std::isnan(properties["angleNew)) {
            properties["angleNew = calculateAngle(properties["baseX, properties["baseY, x, y);
            rotateSelected(properties["baseX, properties["baseY, properties["angleNew - properties["angleRef);
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
Geometry::rotate_context(String str)
{
    //todo("ROTATE", "context()");
}

/**
 * .
 */
void
Geometry::rotate_prompt(String str)
{
    /*
    if (properties["mode"].s == ROTATE_MODE_NORMAL) {
        if (properties["firstRun) {
            EmbReal strList = str.split(",");
            if (std::isnan(strList[0]) || std::isnan(strList[1])) {
                alert(tr("Invalid point."));
                setPromptPrefix(tr("Specify base point: "));
            }
            else {
                properties["firstRun = false;
                properties["baseX = node(strList[0]);
                properties["baseY = node(strList[1]);
                addRubber("LINE");
                setRubberMode("LINE");
                setRubberPoint("LINE_START", properties["baseX, properties["baseY);
                previewOn("SELECTED", "ROTATE", properties["baseX, properties["baseY, 0);
                setPromptPrefix(tr("Specify rotation angle or [Reference]: "));
            }
        }
        else {
            if (str == "R" || str == "REFERENCE") {
                properties["mode"].s = MODE_REFERENCE;
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
                    properties["angle = node(str);
                    rotateSelected(properties["baseX, properties["baseY, properties["angle);
                    previewOff();
                    actuator("end");
                }
            }
        }
    }
    else if (properties["mode"].s == MODE_REFERENCE) {
        if (std::isnan(properties["baseRX)) {
            if (std::isnan(str)) {
                EmbReal strList = str.split(",");
                if (std::isnan(strList[0]) || std::isnan(strList[1])) {
                    alert(tr("Requires valid numeric angle or two points."));
                    setPromptPrefix(tr("Specify the reference angle") + " {0.00}: ");
                }
                else {
                    properties["baseRX = node(strList[0]);
                    properties["baseRY = node(strList[1]);
                    addRubber("LINE");
                    setRubberMode("LINE");
                    setRubberPoint("LINE_START", properties["baseRX, properties["baseRY);
                    setPromptPrefix(tr("Specify second point: "));
                }
            }
            else {
                //The base and dest values are only set here to advance the command.
                properties["baseRX = 0.0;
                properties["baseRY = 0.0;
                properties["destRX = 0.0;
                properties["destRY = 0.0;
                //The reference angle is what we will use later.
                properties["angleRef = node(str);
                addRubber("LINE");
                setRubberMode("LINE");
                setRubberPoint("LINE_START", properties["baseX, properties["baseY);
                previewOn("SELECTED", "ROTATE", properties["baseX, properties["baseY, properties["angleRef);
                setPromptPrefix(tr("Specify the new angle: "));
            }
        }
        else if (std::isnan(properties["destRX)) {
            if (std::isnan(str)) {
                EmbReal strList = str.split(",");
                if (std::isnan(strList[0]) || std::isnan(strList[1])) {
                    alert(tr("Requires valid numeric angle or two points."));
                    setPromptPrefix(tr("Specify second point: "));
                }
                else {
                    properties["destRX = node(strList[0]);
                    properties["destRY = node(strList[1]);
                    properties["angleRef = calculateAngle(properties["baseRX, properties["baseRY, properties["destRX, properties["destRY);
                    previewOn("SELECTED", "ROTATE", properties["baseX, properties["baseY, properties["angleRef);
                    setRubberPoint("LINE_START", properties["baseX, properties["baseY);
                    setPromptPrefix(tr("Specify the new angle: "));
                }
            }
            else {
                //The base and dest values are only set here to advance the command.
                properties["baseRX = 0.0;
                properties["baseRY = 0.0;
                properties["destRX = 0.0;
                properties["destRY = 0.0;
                //The reference angle is what we will use later.
                properties["angleRef = node(str);
                previewOn("SELECTED", "ROTATE", properties["baseX, properties["baseY, properties["angleRef);
                setPromptPrefix(tr("Specify the new angle: "));
            }
        }
        else if (std::isnan(properties["angleNew)) {
            if (std::isnan(str)) {
                EmbReal strList = str.split(",");
                if (std::isnan(strList[0]) || std::isnan(strList[1])) {
                    alert(tr("Requires valid numeric angle or second point."));
                    setPromptPrefix(tr("Specify the new angle: "));
                }
                else {
                    EmbReal x = node(strList[0]);
                    EmbReal y = node(strList[1]);
                    properties["angleNew = calculateAngle(properties["baseX, properties["baseY, x, y);
                    rotateSelected(properties["baseX, properties["baseY, properties["angleNew - properties["angleRef);
                    previewOff();
                    actuator("end");
                }
            }
            else {
                properties["angleNew = node(str);
                rotateSelected(properties["baseX, properties["baseY, properties["angleNew - properties["angleRef);
                previewOff();
                actuator("end");
            }
        }
    }
}

//Command: Sandbox

/*String properties;
properties["test1;
properties["test2;
*/

/**
 * .
 */
void
Geometry::sandbox_main(String str)
{
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
}

/**.
 * Command: Scale
 */
void
Geometry::scale_main(void)
{
    initCommand();

    properties["mode"] = node("MODE_NORMAL");
    properties["firstRun"] = node(true);
    properties["base.x"] = node(0.0f);
    properties["base.y"] = node(0.0f);
    properties["dest.x"] = node(0.0f);
    properties["dest.y"] = node(0.0f);
    properties["factor"] = node(0.0f);

    properties["baseR.x"] = node(0.0f);
    properties["baseR.y"] = node(0.0f);
    properties["destR.x"] = node(0.0f);
    properties["destR.y"] = node(0.0f);
    properties["factorRef"] = node(0.0f);
    properties["factorNew"] = node(0.0f);

    if (numSelected() <= 0) {
        //TODO: Prompt to select objects if nothing is preselected
        alert(tr("Preselect objects before invoking the scale command."));
        actuator("end");
        messageBox("information", tr("Scale Preselect"), tr("Preselect objects before invoking the scale command."));
    }
    else {
        setPromptPrefix(tr("Specify base point: "));
    }
}

/**
 * .
 */
void
Geometry::scale_click(EmbVector v)
{
    if (properties["mode"].s == MODE_NORMAL) {
        if (properties["firstRun) {
            properties["firstRun = false;
            properties["base = v;
            addRubber("LINE");
            setRubberMode("LINE");
            setRubberPoint("LINE_START", properties["baseX, properties["baseY);
            previewOn("SELECTED", "SCALE", properties["baseX, properties["baseY, 1);
            appendPromptHistory();
            setPromptPrefix(tr("Specify scale factor or [Reference]: "));
        }
        else {
            properties["dest = v;
            properties["factor = calculateDistance(properties["base, properties["dest);
            appendPromptHistory();
            scaleSelected(properties["baseX, properties["baseY, properties["factor);
            previewOff();
            actuator("end");
        }
    }
    else if (properties["mode"].s == MODE_REFERENCE) {
        if (std::isnan(properties["baseRX)) {
            properties["baseR = v;
            appendPromptHistory();
            addRubber("LINE");
            setRubberMode("LINE");
            setRubberPoint("LINE_START", properties["baseR);
            setPromptPrefix(tr("Specify second point: "));
        }
        else if (std::isnan(properties["destRX)) {
            properties["destR = v;
            properties["factorRef = calculateDistance(properties["baseRX, properties["baseRY, properties["destRX, properties["destRY);
            if (properties["factorRef <= 0.0) {
                properties["destRX    = NaN;
                properties["destRY    = NaN;
                properties["factorRef = NaN;
                alert(tr("Value must be positive and nonzero."));
                setPromptPrefix(tr("Specify second point: "));
            }
            else {
                appendPromptHistory();
                setRubberPoint("LINE_START", properties["baseX, properties["baseY);
                previewOn("SELECTED", "SCALE", properties["baseX, properties["baseY, properties["factorRef);
                setPromptPrefix(tr("Specify new length: "));
            }
        }
        else if (std::isnan(properties["factorNew)) {
            properties["factorNew = calculateDistance(properties["baseX, properties["baseY, x, y);
            if (properties["factorNew <= 0.0) {
                properties["factorNew = NaN;
                alert(tr("Value must be positive and nonzero."));
                setPromptPrefix(tr("Specify new length: "));
            }
            else {
                appendPromptHistory();
                scaleSelected(properties["baseX, properties["baseY, properties["factorNew/properties["factorRef);
                previewOff();
                actuator("end");
            }
        }
    }
}

/**
 * .
 */
void
Geometry::scale_context(String str)
{
    todo("SCALE", "context()");
}

/**
 * .
 */
void
Geometry::scale_prompt(String str)
{
    if (properties["mode"].s == MODE_NORMAL) {
        if (properties["firstRun) {
            EmbReal strList = str.split(",");
            if (std::isnan(strList[0]) || std::isnan(strList[1])) {
                alert(tr("Invalid point."));
                setPromptPrefix(tr("Specify base point: "));
            }
            else {
                properties["firstRun = false;
                properties["baseX = node(strList[0]);
                properties["baseY = node(strList[1]);
                addRubber("LINE");
                setRubberMode("LINE");
                setRubberPoint("LINE_START", properties["baseX, properties["baseY);
                previewOn("SELECTED", "SCALE", properties["baseX, properties["baseY, 1);
                setPromptPrefix(tr("Specify scale factor or [Reference]: "));
            }
        }
        else {
            if (str == "R" || str == "REFERENCE") {
                properties["mode"].s = MODE_REFERENCE;
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
                    properties["factor = node(str);
                    scaleSelected(properties["baseX, properties["baseY, properties["factor);
                    previewOff();
                    actuator("end");
                }
            }
        }
    }
    else if (properties["mode"].s == MODE_REFERENCE) {
        if (std::isnan(properties["baseRX)) {
            if (std::isnan(str)) {
                EmbReal strList = str.split(",");
                if (std::isnan(strList[0]) || std::isnan(strList[1])) {
                    alert(tr("Requires valid numeric distance or two points."));
                    setPromptPrefix(tr("Specify reference length") + " {1}: ");
                }
                else {
                    properties["baseRX = node(strList[0]);
                    properties["baseRY = node(strList[1]);
                    addRubber("LINE");
                    setRubberMode("LINE");
                    setRubberPoint("LINE_START", properties["baseRX, properties["baseRY);
                    setPromptPrefix(tr("Specify second point: "));
                }
            }
            else {
                //The base and dest values are only set here to advance the command.
                properties["baseRX = 0.0;
                properties["baseRY = 0.0;
                properties["destRX = 0.0;
                properties["destRY = 0.0;
                //The reference length is what we will use later.
                properties["factorRef = node(str);
                if (properties["factorRef <= 0.0) {
                    properties["baseRX    = NaN;
                    properties["baseRY    = NaN;
                    properties["destRX    = NaN;
                    properties["destRY    = NaN;
                    properties["factorRef = NaN;
                    alert(tr("Value must be positive and nonzero."));
                    setPromptPrefix(tr("Specify reference length") + " {1}: ");
                }
                else {
                    addRubber("LINE");
                    setRubberMode("LINE");
                    setRubberPoint("LINE_START", properties["baseX, properties["baseY);
                    previewOn("SELECTED", "SCALE", properties["baseX, properties["baseY, properties["factorRef);
                    setPromptPrefix(tr("Specify new length: "));
                }
            }
        }
        else if (std::isnan(properties["destRX)) {
            if (std::isnan(str)) {
                EmbReal strList = str.split(",");
                if (std::isnan(strList[0]) || std::isnan(strList[1])) {
                    alert(tr("Requires valid numeric distance or two points."));
                    setPromptPrefix(tr("Specify second point: "));
                }
                else {
                    properties["destRX = node(strList[0]);
                    properties["destRY = node(strList[1]);
                    properties["factorRef = calculateDistance(properties["baseRX, properties["baseRY, properties["destRX, properties["destRY);
                    if (properties["factorRef <= 0.0) {
                        properties["destRX    = NaN;
                        properties["destRY    = NaN;
                        properties["factorRef = NaN;
                        alert(tr("Value must be positive and nonzero."));
                        setPromptPrefix(tr("Specify second point: "));
                    }
                    else {
                        setRubberPoint("LINE_START", properties["baseX, properties["baseY);
                        previewOn("SELECTED", "SCALE", properties["baseX, properties["baseY, properties["factorRef);
                        setPromptPrefix(tr("Specify new length: "));
                    }
                }
            }
            else {
                //The base and dest values are only set here to advance the command.
                properties["baseRX = 0.0;
                properties["baseRY = 0.0;
                properties["destRX = 0.0;
                properties["destRY = 0.0;
                //The reference length is what we will use later.
                properties["factorRef = node(str);
                if (properties["factorRef <= 0.0) {
                    properties["destRX    = NaN;
                    properties["destRY    = NaN;
                    properties["factorRef = NaN;
                    alert(tr("Value must be positive and nonzero."));
                    setPromptPrefix(tr("Specify second point: "));
                }
                else {
                    setRubberPoint("LINE_START", properties["baseX, properties["baseY);
                    previewOn("SELECTED", "SCALE", properties["baseX, properties["baseY, properties["factorRef);
                    setPromptPrefix(tr("Specify new length: "));
                }
            }
        }
        else if (std::isnan(properties["factorNew)) {
            if (std::isnan(str)) {
                EmbReal strList = str.split(",");
                if (std::isnan(strList[0]) || std::isnan(strList[1])) {
                    alert(tr("Requires valid numeric distance or second point."));
                    setPromptPrefix(tr("Specify new length: "));
                }
                else {
                    EmbReal x = node(strList[0]);
                    EmbReal y = node(strList[1]);
                    properties["factorNew = calculateDistance(properties["baseX, properties["baseY, x, y);
                    if (properties["factorNew <= 0.0) {
                        properties["factorNew = NaN;
                        alert(tr("Value must be positive and nonzero."));
                        setPromptPrefix(tr("Specify new length: "));
                    }
                    else {
                        scaleSelected(properties["baseX, properties["baseY, properties["factorNew/properties["factorRef);
                        previewOff();
                        actuator("end");
                    }
                }
            }
            else {
                properties["factorNew = node(str);
                if (properties["factorNew <= 0.0) {
                    properties["factorNew = NaN;
                    alert(tr("Value must be positive and nonzero."));
                    setPromptPrefix(tr("Specify new length: "));
                }
                else {
                    scaleSelected(properties["baseX, properties["baseY, properties["factorNew/properties["factorRef);
                    previewOff();
                    actuator("end");
                }
            }
        }
    }
}

/**
 * .
 */
void
Geometry::text_single_main(void)
{
    initCommand();
    clearSelection();
    properties["text"] = node("");
    properties["text.x"] = node(0.0f);
    properties["text.y"] = node(0.0f);
    properties["text.justify"] = node("Left");
    properties["textFont"] = textFont();
    properties["textHeight"] = NaN;
    properties["textRotation"] = NaN;
    properties["mode"] = node("MODE_SETGEOM");
    setPromptPrefix(tr("Current font: ") + "{" + properties["textFont + "} " + tr("Text height: ") + "{" +  textSize() + "}");
    appendPromptHistory();
    setPromptPrefix(tr("Specify start point of text or [Justify/Setfont]: "));
}

/**
 * .
 */
void
Geometry::text_single_click(EmbVector v)
{
    /*
    if (properties["mode"].s == MODE_SETGEOM) {
        if (std::isnan(properties["textX)) {
            properties["textX = x;
            properties["textY = y;
            addRubber("LINE");
            setRubberMode("LINE");
            setRubberPoint("LINE_START", properties["textX, properties["textY);
            appendPromptHistory();
            setPromptPrefix(tr("Specify text height") + " {" + textSize() + "}: ");
        }
        else if (std::isnan(properties["textHeight)) {
            properties["textHeight = calculateDistance(properties["textX, properties["textY, x, y);
            setTextSize(properties["textHeight);
            appendPromptHistory();
            setPromptPrefix(tr("Specify text angle") + " {" + textAngle() + "}: ");
        }
        else if (std::isnan(properties["textRotation)) {
            properties["textRotation = calculateAngle(properties["textX, properties["textY, x, y);
            setTextAngle(properties["textRotation);
            appendPromptHistory();
            setPromptPrefix(tr("Enter text: "));
            properties["mode"].s = MODE_RAPID;
            prompt->enableRapidFire();
            clearRubber();
            addRubber("TEXTSINGLE");
            setRubberMode("TEXTSINGLE");
            setRubberPoint("TEXT_POINT", properties["textX, properties["textY);
            setRubberPoint("TEXT_HEIGHT_ROTATION", properties["textHeight, properties["textRotation);
            setRubberText("TEXT_FONT", properties["textFont);
            setRubberText("TEXT_JUSTIFY", properties["textJustify);
            setRubberText("TEXT_RAPID", properties["text);
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
Geometry::text_single_context(String str)
{
    //todo("SINGLELINETEXT", "context()");
}

/**
 * .
 */
void
Geometry::text_single_prompt(String str)
{
    if (properties["mode"].s == MODE_JUSTIFY) {
        if (str == "C" || str == "CENTER") {
            properties["mode"].s = MODE_SETGEOM;
            properties["textJustify = "Center";
            setRubberText("TEXT_JUSTIFY", properties["textJustify);
            setPromptPrefix(tr("Specify center point of text or [Justify/Setfont]: "));
        }
        else if (str == "R" || str == "RIGHT") {
            properties["mode"].s = MODE_SETGEOM;
            properties["textJustify = "Right";
            setRubberText("TEXT_JUSTIFY", properties["textJustify);
            setPromptPrefix(tr("Specify right-end point of text or [Justify/Setfont]: "));
        }
        else if (str == "A" || str == "ALIGN") {
            properties["mode"].s = MODE_SETGEOM;
            properties["textJustify = "Aligned";
            setRubberText("TEXT_JUSTIFY", properties["textJustify);
            setPromptPrefix(tr("Specify start point of text or [Justify/Setfont]: "));
        }
        else if (str == "M" || str == "MIDDLE") {
            properties["mode"].s = MODE_SETGEOM;
            properties["textJustify = "Middle";
            setRubberText("TEXT_JUSTIFY", properties["textJustify);
            setPromptPrefix(tr("Specify middle point of text or [Justify/Setfont]: "));
        }
        else if (str == "F" || str == "FIT") {
            properties["mode"].s = MODE_SETGEOM;
            properties["textJustify = "Fit";
            setRubberText("TEXT_JUSTIFY", properties["textJustify);
            setPromptPrefix(tr("Specify start point of text or [Justify/Setfont]: "));
        }
        else if (str == "TL" || str == "TOPLEFT") {
            properties["mode"].s = MODE_SETGEOM;
            properties["textJustify = "Top Left";
            setRubberText("TEXT_JUSTIFY", properties["textJustify);
            setPromptPrefix(tr("Specify top-left point of text or [Justify/Setfont]: "));
        }
        else if (str == "TC" || str == "TOPCENTER") {
            properties["mode"].s = MODE_SETGEOM;
            properties["textJustify = "Top Center";
            setRubberText("TEXT_JUSTIFY", properties["textJustify);
            setPromptPrefix(tr("Specify top-center point of text or [Justify/Setfont]: "));
        }
        else if (str == "TR" || str == "TOPRIGHT") {
            properties["mode"].s = MODE_SETGEOM;
            properties["textJustify = "Top Right";
            setRubberText("TEXT_JUSTIFY", properties["textJustify);
            setPromptPrefix(tr("Specify top-right point of text or [Justify/Setfont]: "));
        }
        else if (str == "ML" || str == "MIDDLELEFT") {
            properties["mode"].s = MODE_SETGEOM;
            properties["textJustify = "Middle Left";
            setRubberText("TEXT_JUSTIFY", properties["textJustify);
            setPromptPrefix(tr("Specify middle-left point of text or [Justify/Setfont]: "));
        }
        else if (str == "MC" || str == "MIDDLECENTER") {
            properties["mode"].s = MODE_SETGEOM;
            properties["textJustify = "Middle Center";
            setRubberText("TEXT_JUSTIFY", properties["textJustify);
            setPromptPrefix(tr("Specify middle-center point of text or [Justify/Setfont]: "));
        }
        else if (str == "MR" || str == "MIDDLERIGHT") {
            properties["mode"].s = MODE_SETGEOM;
            properties["textJustify = "Middle Right";
            setRubberText("TEXT_JUSTIFY", properties["textJustify);
            setPromptPrefix(tr("Specify middle-right point of text or [Justify/Setfont]: "));
        }
        else if (str == "BL" || str == "BOTTOMLEFT") {
            properties["mode"].s = MODE_SETGEOM;
            properties["textJustify = "Bottom Left";
            setRubberText("TEXT_JUSTIFY", properties["textJustify);
            setPromptPrefix(tr("Specify bottom-left point of text or [Justify/Setfont]: "));
        }
        else if (str == "BC" || str == "BOTTOMCENTER") {
            properties["mode"].s = MODE_SETGEOM;
            properties["textJustify = "Bottom Center";
            setRubberText("TEXT_JUSTIFY", properties["textJustify);
            setPromptPrefix(tr("Specify bottom-center point of text or [Justify/Setfont]: "));
        }
        else if (str == "BR" || str == "BOTTOMRIGHT") {
            properties["mode"].s = MODE_SETGEOM;
            properties["textJustify = "Bottom Right";
            setRubberText("TEXT_JUSTIFY", properties["textJustify);
            setPromptPrefix(tr("Specify bottom-right point of text or [Justify/Setfont]: "));
        }
        else {
            alert(tr("Invalid option keyword."));
            setPromptPrefix(tr("Text Justification Options [Center/Right/Align/Middle/Fit/TL/TC/TR/ML/MC/MR/BL/BC/BR]: "));
        }
    }
    else if (properties["mode"].s == MODE_SETFONT) {
        properties["mode"].s = MODE_SETGEOM;
        properties["textFont = str;
        setRubberText("TEXT_FONT", properties["textFont);
        setTextFont(properties["textFont);
        setPromptPrefix(tr("Specify start point of text or [Justify/Setfont]: "));
    }
    else if (properties["mode"].s == MODE_SETGEOM) {
        if (std::isnan(properties["textX)) {
            if (str == "J" || str == "JUSTIFY") {
                properties["mode"].s = MODE_JUSTIFY;
                setPromptPrefix(tr("Text Justification Options [Center/Right/Align/Middle/Fit/TL/TC/TR/ML/MC/MR/BL/BC/BR]: "));
            }
            else if (str == "S" || str == "SETFONT") {
                properties["mode"].s = MODE_SETFONT;
                setPromptPrefix(tr("Specify font name: "));
            }
            else {
                EmbReal strList = str.split(",");
                if (std::isnan(strList[0]) || std::isnan(strList[1])) {
                    alert(tr("Point or option keyword required."));
                    setPromptPrefix(tr("Specify start point of text or [Justify/Setfont]: "));
                }
                else {
                    properties["textX = node(strList[0]);
                    properties["textY = node(strList[1]);
                    addRubber("LINE");
                    setRubberMode("LINE");
                    setRubberPoint("LINE_START", properties["textX, properties["textY);
                    setPromptPrefix(tr("Specify text height") + " {" + textSize() + "}: ");
                }
            }
        }
        else if (std::isnan(properties["textHeight)) {
            if (str == "") {
                properties["textHeight = textSize();
                setPromptPrefix(tr("Specify text angle") + " {" + textAngle() + "}: ");
            }
            else if (std::isnan(str)) {
                alert(tr("Requires valid numeric distance or second point."));
                setPromptPrefix(tr("Specify text height") + " {" + textSize() + "}: ");
            }
            else {
                properties["textHeight = node(str);
                setTextSize(properties["textHeight);
                setPromptPrefix(tr("Specify text angle") + " {" + textAngle() + "}: ");
            }
        }
        else if (std::isnan(properties["textRotation)) {
            if (str == "") {
                properties["textRotation = textAngle();
                setPromptPrefix(tr("Enter text: "));
                properties["mode"].s = MODE_RAPID;
                prompt->enableRapidFire();
                clearRubber();
                addRubber("TEXTSINGLE");
                setRubberMode("TEXTSINGLE");
                setRubberPoint("TEXT_POINT", properties["textX, properties["textY);
                setRubberPoint("TEXT_HEIGHT_ROTATION", properties["textHeight, properties["textRotation);
                setRubberText("TEXT_FONT", properties["textFont);
                setRubberText("TEXT_JUSTIFY", properties["textJustify);
                setRubberText("TEXT_RAPID", properties["text);
            }
            else if (std::isnan(str)) {
                alert(tr("Requires valid numeric angle or second point."));
                setPromptPrefix(tr("Specify text angle") + " {" + textAngle() + "}: ");
            }
            else {
                properties["textRotation = node(str);
                setTextAngle(properties["textRotation);
                setPromptPrefix(tr("Enter text: "));
                properties["mode"].s = MODE_RAPID;
                prompt->enableRapidFire();
                clearRubber();
                addRubber("TEXTSINGLE");
                setRubberMode("TEXTSINGLE");
                setRubberPoint("TEXT_POINT", properties["textX, properties["textY);
                setRubberPoint("TEXT_HEIGHT_ROTATION", properties["textHeight, properties["textRotation);
                setRubberText("TEXT_FONT", properties["textFont);
                setRubberText("TEXT_JUSTIFY", properties["textJustify);
                setRubberText("TEXT_RAPID", properties["text);
            }
        }
        else {
            //Do nothing, as we are in rapidFire mode now.
        }
    }
    else if (properties["mode"].s == MODE_RAPID) {
        if (str == "RAPID_ENTER") {
            if (properties["text == "") {
                actuator("end");
            }
            else {
                actuator("vulcanize");
                actuator("end");
                //TODO: Rather than ending the command, calculate where the next line would be and modify the x/y to the new point
            }
        }
        else {
            properties["text = str;
            setRubberText("TEXT_RAPID", properties["text);
        }
    }
}

void snowflake_main(void);
void updateSnowflake(EmbVector scale);

void star_main(void);
void star_move(EmbVector v);
void star_prompt(String properties);
void star_context(String properties);
void star_prompt(String properties);
void updateStar(EmbVector mouse);

void syswindows_main(void);
void syswindows_prompt(String properties);

/**
 * .
 */
void
Geometry::snowflake_main(void)
{
    //initCommand();
    //clearSelection();
    properties["numPoints"] = node(2048); //Default //TODO: min:64 max:8192
    properties["center.x"] = node(0.0f);
    properties["center.y"] = node(0.0f);
    properties["scale.x"] = node(0.04f);
    properties["scale.y"] = node(0.04f);
    properties["mode"].s = node("SNOWFLAKE_MODE_NUM_POINTS");

    //addRubber("POLYGON");
    //setRubberMode("POLYGON");
    EmbVector v;
    v.x = 0.04f; v.y = 0.04f;
    properties = updateSnowflake(properties, v);
    //spareRubber("POLYGON");
    actuator("end");
}

/**
 * Snowflake Curve with t [0,2pi]
 */
void
Geometry::updateSnowflake(EmbVector scale)
{
    for (int i = 0; i <= properties["numPoints"].i; i++) {
        EmbReal t = (2.0*emb_constant_pi) / properties["numPoints"].i*i;
        EmbVector v;
        v.x = fourier_series(t, snowflake_x);
        v.y = fourier_series(t, snowflake_y);

        setRubberPoint("POLYGON_POINT_" + i.toString(),
            xx*scale.x, yy*scale.y);
    }

    setRubberText("POLYGON_NUM_POINTS", numPoints.toString());
}

/**
 * .
 */
void
Geometry::star_main(void)
{
    initCommand();
    clearSelection();
    properties["numPoints = 5;
    EmbVector center;
    EmbVector point1;
    EmbVector point2;
    center.x = 0.0;
    center.y = 0.0;
    point1.x = 1.0;
    point1.y = 1.0;
    point2.x = 2.0;
    point2.y = 2.0;
    properties["center"] = node(center);
    properties["point1"] = node(point1);
    properties["point2"] = node(point2);
    properties["mode"].s = STAR_MODE_NUM_POINTS;
    setPromptPrefix(tr("Enter number of star points {5}: "));
}

/**
 * @brief star_click
 */
void
Geometry::star_click(EmbReal mouse)
{
    if (properties["mode"].s == "MODE_NUM_POINTS") {
        //Do nothing, the prompt controls this.
    }
    else if (properties["mode"].s == "MODE_CENTER_PT") {
        properties["center = mouse;
        properties["mode"].s = MODE_RAD_OUTER;
        setPromptPrefix(tr("Specify outer radius of star: "));
        addRubber("POLYGON");
        setRubberMode("POLYGON");
        updateStar(properties, properties["center);
        actuator("enable move-rapid-fire");
    }
    else if (properties["mode"].s == "MODE_RAD_OUTER") {
        properties["point1 = mouse;
        properties["mode"].s = MODE_RAD_INNER;
        setPromptPrefix(tr("Specify inner radius of star: "));
        updateStar(properties["point1);
    }
    else if (properties["mode"].s == "MODE_RAD_INNER") {
        properties["point2 = mouse;
        actuator("disable move-rapid-fire");
        updateStar(properties["point2);
        spareRubber("POLYGON");
        //actuator("end");
    }
}

/**
 * @brief star_move
 */
void
Geometry::star_move(EmbVector v)
{
    if (properties["mode"].s == "STAR_MODE_NUM_POINTS") {
        //Do nothing, the prompt controls this.
    }
    else if (properties["mode"].s == "STAR_MODE_CENTER_PT") {
        //Do nothing, prompt and click controls this.
    }
    else if (properties["mode"].s == "STAR_MODE_RAD_OUTER") {
        properties = updateStar(properties, v);
    }
    else if (properties["mode"].s == "STAR_MODE_RAD_INNER") {
        properties = updateStar(properties, v);
    }
}

/**
 * @brief star_context
 */
void
Geometry::star_context(String str)
{
    //todo("STAR", "context()");
}

/**
 * @brief star_prompt
 */
void
Geometry::star_prompt(String str)
{
    /*
    if (properties["mode"].s == STAR_MODE_NUM_POINTS) {
        if (str == "" && properties["numPoints >= 3 && properties["numPoints <= 1024) {
            setPromptPrefix(tr("Specify center point: "));
            properties["mode"].s = STAR_MODE_CENTER_PT;
        }
        else {
            EmbReal tmp = node(str);
            if (std::isnan(tmp) || !isInt(tmp) || tmp < 3 || tmp > 1024) {
                alert(tr("Requires an integer between 3 and 1024."));
                setPromptPrefix(tr("Enter number of star points") + " {" + properties["numPoints.toString() + "}: ");
            }
            else {
                properties["numPoints = tmp;
                setPromptPrefix(tr("Specify center point: "));
                properties["mode"].s = "MODE_CENTER_PT";
            }
        }
    }
    else if (properties["mode"].s == STAR_MODE_CENTER_PT) {
        EmbReal strList = str.split(",");
        if (std::isnan(strList[0]) || std::isnan(strList[1])) {
            alert(tr("Invalid point."));
            setPromptPrefix(tr("Specify center point: "));
        }
        else {
            properties["center.x"] = node(strList[0]);
            properties["center.y"] = node(strList[1]);
            properties["mode"].s = MODE_RAD_OUTER;
            setPromptPrefix(tr("Specify outer radius of star: "));
            addRubber("POLYGON");
            setRubberMode("POLYGON");
            updateStar(qsnapX(), qsnapY());
            actuator("enable move-rapid-fire");
        }
    }
    else if (properties["mode"].s == STAR_MODE_RAD_OUTER) {
        EmbReal strList = str.split(",");
        if (std::isnan(strList[0]) || std::isnan(strList[1])) {
            alert(tr("Invalid point."));
            setPromptPrefix(tr("Specify outer radius of star: "));
        }
        else {
            properties["x1 = node(strList[0]);
            properties["y1 = node(strList[1]);
            properties["mode"].s = MODE_RAD_INNER;
            setPromptPrefix(tr("Specify inner radius of star: "));
            updateStar(qsnapX(), qsnapY());
        }
    }
    else if (properties["mode"].s == STAR_MODE_RAD_INNER) {
        EmbReal strList = str.split(",");
        if (std::isnan(strList[0]) || std::isnan(strList[1])) {
            alert(tr("Invalid point."));
            setPromptPrefix(tr("Specify inner radius of star: "));
        }
        else {
            properties["x2 = node(strList[0]);
            properties["y2 = node(strList[1]);
            actuator("disable move-rapid-fire");
            updateStar(properties["x2, properties["y2);
            spareRubber("POLYGON");
            //actuator("end");
        }
    }
    */
}

/**
 * @brief updateStar
 */
void
Geometry::Geometry::updateStar(EmbVector mouse)
{
    EmbReal distOuter;
    EmbReal distInner;
    EmbReal angOuter;

    if (properties["mode"].s == "STAR_MODE_RAD_OUTER") {
        EmbVector v = mouse - properties["center"].v;
        angOuter = embVector_angle(v);
        distOuter = embVector_length(v);
        distInner = distOuter/2.0;
    }
    else if (properties["mode"].s == "STAR_MODE_RAD_INNER") {
        EmbVector v = properties["point1"].v - properties["center"].v;
        angOuter = embVector_angle(v);
        distOuter = embVector_length(v);
        distInner = embVector_distance(properties["center"].v, mouse);
    }

    // Calculate the Star Points
    EmbReal angInc = 360.0/(properties["numPoints"].i*2);
    for (int i = 0; i < properties["numPoints"].i*2; i++) {
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
            properties["center.x + v.x,
            properties["center.y + v.y);
        */
    }
    /*
    setRubberText("POLYGON_NUM_POINTS",
        (properties["numPoints*2 - 1).toString());
    */
}

/**
 * .
 */
void
Geometry::syswindows_main(void)
{
    initCommand();
    clearSelection();
    setPromptPrefix(tr("Enter an option [Cascade/Tile]: "));
}

/**
 * .
 */
void
Geometry::syswindows_prompt(String str)
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
#endif
