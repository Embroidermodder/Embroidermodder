/*
 * Embroidermodder 2.
 *
 * ------------------------------------------------------------
 *
 * Copyright 2013-2022 The Embroidermodder Team
 * Embroidermodder 2 is Open Source Software.
 * See LICENSE for licensing terms.
 *
 * ------------------------------------------------------------
 *
 * Use Python's PEP7 style guide.
 *      https://peps.python.org/pep-0007/
 *
 * For additional tips on our style see the linux Kernel style:
 *     https://www.kernel.org/doc/Documentation/process/coding-style.rst
 *
 */

#include "embroidermodder.h"

typedef std::string String;

void addArc(View* view, QGraphicsItem* item);
void addBlock(View* view, QGraphicsItem* item);
void addCircle(View* view, QGraphicsItem* item);
void addDimAligned(View* view, QGraphicsItem* item);
void addDimAngular(View* view, QGraphicsItem* item);
void addDimArcLength(View* view, QGraphicsItem* item);
void addDimDiameter(View* view, QGraphicsItem* item);
void addDimLeader(View* view, QGraphicsItem* item);
void addDimLinear(View* view, QGraphicsItem* item);
void addDimOrdinate(View* view, QGraphicsItem* item);
void addDimRadius(View* view, QGraphicsItem* item);
void addEllipse(View* view, QGraphicsItem* item);
void addEllipseArc(View* view, QGraphicsItem* item);
void addGrid(View* view, QGraphicsItem* item);
void addHatch(View* view, QGraphicsItem* item);
void addImage(View* view, QGraphicsItem* item);
void addInfiniteLine(View* view, QGraphicsItem* item);
void addLine(View* view, QGraphicsItem* item);
void addPath(View* view, QGraphicsItem* item);
void addPoint(View* view, QGraphicsItem* item);
void addPolygon(View* view, QGraphicsItem* item);
void addPolyline(View* view, QGraphicsItem* item);
void addRay(View* view, QGraphicsItem* item);
void addRectangle(View* view, QGraphicsItem* item);
void addSlot(View* view, QGraphicsItem* item);
void addSpline(View* view, QGraphicsItem* item);
void addTextMulti(View* view, QGraphicsItem* item);
void addTextSingle(View* view, QGraphicsItem* item);

void toPolyline(
    View* view,
    QPointF objPos,
    QPainterPath objPath,
    QString layer,
    QColor color,
    QString lineType,
    QString lineWeight);


bool save(View *view, QString f);

/* . */
bool
save_current_file(String fileName)
{
    View* view = activeView();
    QGraphicsScene* gscene = activeScene();
    view->formatType = EMBFORMAT_UNSUPPORTED;
    return save(view, QString::fromStdString(fileName));
}

/* Find closest point to "position" from the list of points given. */
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

/* Add_polyline. */
void
add_polyline(QPainterPath p, String rubberMode)
{
    View* gview = activeView();
    QGraphicsScene* gscene = gview->scene();
    QUndoStack* stack = gview->getUndoStack();
    if (!(gview && gscene && stack)) {
        return;
    }
    Geometry* obj = new Geometry(OBJ_TYPE_POLYLINE, _mainWin->getCurrentColor(), Qt::SolidLine);
    obj->normalPath = p;
    obj->updatePath();
    obj->objRubberMode = rubberMode;
    if (rubberMode != "OBJ_RUBBER_OFF") {
        gview->addToRubberRoom(obj);
        gscene->addItem(obj);
        gscene->update();
    }
    else {
        UndoableCommand* cmd = new UndoableCommand("add", obj->data(OBJ_NAME).toString(), obj, gview);
        stack->push(cmd);
    }
}

/* Construct a new Geometry object of arc type.
 * Initialize common object properties.
 *
 * WARNING
 * -------
 * DO NOT enable QGraphicsItem::ItemIsMovable. If it is enabled,
 * and the item is double clicked, the scene will erratically move the item while zooming.
 * All movement has to be handled explicitly by us, not by the scene.
 *
 * TODO: getCurrentLineType
 */
void
Geometry::init(int type_, QRgb rgb, Qt::PenStyle lineType, QGraphicsItem* parent)
{
    debug_message("Geometry Constructor()");
    Type = type_;
    setData(OBJ_TYPE, Type);

    setFlag(QGraphicsItem::ItemIsSelectable, true);

    objPen.setColor(rgb);
    lwtPen.setColor(rgb);
    objPen.setStyle(lineType);
    lwtPen.setStyle(lineType);
    objPen.setCapStyle(Qt::RoundCap);
    objPen.setJoinStyle(Qt::RoundJoin);
    lwtPen.setCapStyle(Qt::RoundCap);
    lwtPen.setJoinStyle(Qt::RoundJoin);

    objID = QDateTime::currentMSecsSinceEpoch();

    setObjectLineWeight("0.35"); //TODO: pass in proper lineweight

    switch (Type) {
    case OBJ_TYPE_ARC:
        setData(OBJ_NAME, "Arc");
        break;
    case OBJ_TYPE_CIRCLE:
        setData(OBJ_NAME, "Circle");
        break;
    case OBJ_TYPE_ELLIPSE:
        setData(OBJ_NAME, "Ellipse");
        break;
        setData(OBJ_NAME, "Dimension Leader");
        break;
        setData(OBJ_NAME, "Line");
        break;
        setData(OBJ_NAME, "Point");
        break;
        setData(OBJ_NAME, "Polygon");
        break;
        setData(OBJ_NAME, "Polyline");
        break;
        setData(OBJ_NAME, "Rectangle");
        break;
        setData(OBJ_NAME, "Single Line Text");
        break;
        setData(OBJ_NAME, "Multi Line Text");
        break;
    default:
        setData(OBJ_NAME, "Unknown");
        break;
    }

/*
    init_arc(arc); 
    init_circle(circle);
    init_ellipse(ellipse);
    init_line(line); //TODO: getCurrentLineType
    init_point(vector); //TODO: getCurrentLineType
    init_path(p); //TODO: getCurrentLineType
    init_rect(rect); //TODO: getCurrentLineType
    init_text_single(str, v); //TODO: getCurrentLineType
*/

    setPen(objPen);
}

Geometry::Geometry(int type_, QRgb rgb, Qt::PenStyle lineType,
    QGraphicsItem* parent) : QGraphicsPathItem(parent)
{
    init(type_, rgb, lineType, parent);
}

/* TODO: 
 */
void
Geometry::update(void)
{

}

/* Initialise arc object.
 */
void
Geometry::init_arc(EmbArc arc)
{
    calculateArcData(arc);
}

/* Init circle
 */
void
Geometry::init_circle(EmbCircle circle)
{
    setObjectRadius(circle.radius);
    setObjectCenter(circle.center);
    updatePath();
}

/* Geometry::init_ellipse
 */
void
Geometry::init_ellipse(EmbEllipse ellipse)
{
    setObjectSize(ellipse.radius.x, ellipse.radius.y);
    setObjectCenter(ellipse.center);
    updatePath();
}

/* Geometry::init_line
 */
void
Geometry::init_line(EmbLine line)
{
    if (Type==OBJ_TYPE_DIMLEADER) {
        flags |= ((!PROP_CURVED) & PROP_FILLED);
    }

	setObjectEndPoint1(line.start);
	setObjectEndPoint2(line.end);
}

/* Geometry::init_point
 */
void
Geometry::init_point(EmbVector position)
{
    setRect(-0.00000001, -0.00000001, 0.00000002, 0.00000002);
    setPos(position.x, position.y);
}


/* Geometry::init_path
 */
void
Geometry::init_path(QPainterPath p)
{
	gripIndex = -1;
	normalPath = p;
    updatePath();
    if (Type==OBJ_TYPE_POLYLINE) {
        updatePath(p);
    }

	QPainterPath::Element position = p.elementAt(0);
	setPos(position.x, position.y);
}

/* Geometry::init_rect
 */
void Geometry::init_rect(EmbRect rect)
{
    setObjectRect(rect.left, rect.top, rect.right-rect.left, rect.bottom-rect.top);
}

/* Init_text_single.
 *
 * TODO: set the justification properly.
 * TODO: pass in proper lineweight
 */
void
Geometry::init_text_single(QString str, EmbVector v)
{
    objTextJustify = "Left";

    setObjectText(str);
    setPos(v.x, v.y);
}

/* Geometry::Geometry *obj *parent. */
Geometry::Geometry(Geometry* obj, QGraphicsItem* parent) : QGraphicsPathItem(parent)
{
    debug_message("Geometry Constructor()");
    if (!obj) {
        debug_message("ERROR: null obj pointer passed to Geometry contructor.");
        return;
    }
    init(obj->Type, obj->objPen.color().rgb(), Qt::SolidLine); //TODO: getCurrentLineType
    flags = obj->flags;
	Type = obj->Type;
	setRotation(obj->rotation());
	setScale(obj->scale());
	switch (Type) {
	case OBJ_TYPE_ARC: {
		EmbArc arc;
		arc.start = to_EmbVector(obj->objectStartPoint());
		arc.mid = to_EmbVector(obj->objectMidPoint());
		arc.end = to_EmbVector(obj->objectEndPoint());
		init_arc(arc);
		break;
	}

	case OBJ_TYPE_CIRCLE: {
		EmbCircle circle;
		circle.center = to_EmbVector(obj->objectCenter());
		circle.radius = obj->objectRadius();
		init_circle(circle);
		break;
	}

	case OBJ_TYPE_DIMLEADER: {
		EmbLine line;
		line.start = to_EmbVector(obj->objectEndPoint1());
		line.end = to_EmbVector(obj->objectEndPoint2());
		init_line(line);
		break;
	}

	case OBJ_TYPE_ELLIPSE: {
		EmbEllipse ellipse;
		ellipse.center = to_EmbVector(obj->objectCenter());
		ellipse.radius.x = obj->objectWidth();
		ellipse.radius.y = obj->objectHeight();
		init_ellipse(ellipse);
		break;
	}

	case OBJ_TYPE_LINE: {
		EmbLine line;
		line.start = to_EmbVector(obj->objectEndPoint1());
		line.end = to_EmbVector(obj->objectEndPoint2());
		init_line(line);
		break;
	}

	case OBJ_TYPE_POINT: {
		init_point(to_EmbVector(obj->objectPos()));
		break;
	}

	case OBJ_TYPE_POLYGON:
	case OBJ_TYPE_POLYLINE: {
		init_path(obj->objectCopyPath());
		break;
	}

	case OBJ_TYPE_RECTANGLE: {
		QPointF ptl = obj->objectTopLeft();
		EmbRect rect;
		rect.left = ptl.x();
		rect.top = ptl.y();
		rect.right = rect.left + obj->objectWidth();
		rect.bottom = rect.top + obj->objectHeight();
		init_rect(rect);
		break;
	}

	case OBJ_TYPE_TEXTSINGLE: {
		setObjectTextFont(obj->objTextFont);
		setObjectTextSize(obj->text_size);
		EmbVector v;
		v.x = obj->objectX();
		v.y = obj->objectY();
		init_text_single(obj->objText, v);
		setObjectText(obj->objText);
		break;
	}

	default:
		break;
	}
}

/* Geometry::allGripPoints */
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
        if (flags & PROP_CURVED) {
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

/* Geometry::~Geometry
 */
Geometry::~Geometry()
{
    debug_message("Geometry Destructor()");
}

/* Set object line weight. */
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
        QMessageBox::warning(0, translate_str("Error - Negative Lineweight"),
                                translate_str("Lineweight: %1")
                                .arg(QString().setNum(lineWeight)));
        debug_message("Lineweight cannot be negative! Inverting sign.");
        lwtPen.setWidthF(-lineWeight);
    }
    */
}

/* Geometry::objectRubberPoint
 * key */
QPointF
Geometry::objectRubberPoint(QString  key)
{
    if (objRubberPoints.contains(key)) {
        return objRubberPoints.value(key);
    }

    QGraphicsScene* gscene = scene();
    if (gscene) {
        return scene()->property("SCENE_QSNAP_POINT").toPointF();
    }
    return QPointF();
}

/* Geometry::objectRubberText
 * key
 */
QString
Geometry::objectRubberText(QString  key)
{
    if (objRubberTexts.contains(key)) {
        return objRubberTexts.value(key);
    }
    return QString();
}

/* If gripped, force this object to be drawn even if it is offscreen. */
QRectF
Geometry::boundingRect()
{
    if (objRubberMode == "OBJ_RUBBER_GRIP") {
        return scene()->sceneRect();
    }
    return path().boundingRect();
}

/* Geometry::drawRubberLine
 * rubLine, painter, colorFromScene
 */
void
Geometry::drawRubberLine(const QLineF& rubLine, QPainter* painter, const char* colorFromScene)
{
    if (!painter) {
        return;
    }
    QGraphicsScene* objScene = scene();
    if (!objScene) {
        return;
    }
    QPen colorPen = objPen;
    colorPen.setColor(QColor(objScene->property(colorFromScene).toUInt()));
    painter->setPen(colorPen);
    painter->drawLine(rubLine);
    painter->setPen(objPen);
}

/* Geometry::realRender
 * painter, renderPath
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

/* Return the closest snap point to the mouse point. */
QPointF
Geometry::mouseSnapPoint(const QPointF& mousePoint)
{
    return closest_point(mousePoint, allGripPoints());
}

/* Geometry::setObjectEndPoint1
 * x1, y1
 */
void
Geometry::setObjectEndPoint1(EmbVector endPt1)
{
    EmbVector endPt2 = to_EmbVector(objectEndPoint2());
    EmbVector delta = embVector_subtract(endPt2, endPt1);
    // setScale(1); ?
    setRotation(0);
    setLine(0, 0, delta.x, delta.y);
    setPos(endPt1.x, endPt1.y);
    if (Type == OBJ_TYPE_DIMLEADER) {
        updateLeader();
    }
}

/* Geometry::setObjectEndPoint2
 * x2
 * y2
 */
void
Geometry::setObjectEndPoint2(EmbVector endPt2)
{
    EmbVector endPt1 = to_EmbVector(scenePos());
    EmbVector delta = embVector_subtract(endPt2, endPt1);
    setRotation(0);
    // setScale(1); ?
    setLine(0, 0, delta.x, delta.y);
    setPos(endPt1.x, endPt1.y);
    if (Type == OBJ_TYPE_DIMLEADER) {
        updateLeader();
    }
}

/* Geometry::objectEndPoint1 */
QPointF
Geometry::objectEndPoint1()
{
    return scenePos();
}

/* Geometry::objectEndPoint2 */
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

/* Geometry::updateLeader */
void
Geometry::updateLeader()
{
    int arrowStyle = ARROW_STYLE_CLOSED; //TODO: Make this customizable
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

    switch (arrowStyle) {
    case ARROW_STYLE_OPEN: {
        arrowStylePath = QPainterPath();
        arrowStylePath.moveTo(ap1);
        arrowStylePath.lineTo(ap0);
        arrowStylePath.lineTo(ap2);
        arrowStylePath.lineTo(ap0);
        arrowStylePath.lineTo(ap1);
        break;
    }
    case ARROW_STYLE_CLOSED: {
        arrowStylePath = QPainterPath();
        arrowStylePath.moveTo(ap1);
        arrowStylePath.lineTo(ap0);
        arrowStylePath.lineTo(ap2);
        arrowStylePath.lineTo(ap1);
        break;
    }
    case ARROW_STYLE_DOT: {
        arrowStylePath = QPainterPath();
        arrowStylePath.addEllipse(ap0, arrowStyleLength, arrowStyleLength);
        break;
    }
    case ARROW_STYLE_BOX: {
        arrowStylePath = QPainterPath();
        EmbReal side = QLineF(ap1, ap2).length();
        QRectF ar0(0, 0, side, side);
        ar0.moveCenter(ap0);
        arrowStylePath.addRect(ar0);
        break;
    }
    case ARROW_STYLE_TICK:
        break;
    default:
        break;
    }

    lineStylePath = QPainterPath();
    lineStylePath.moveTo(ap0);
    lineStylePath.lineTo(lp0);
}

/* Geometry::updateRubber
 * painter
 */
void
Geometry::updateRubber(QPainter* painter)
{
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
        if (objRubberMode == "OBJ_RUBBER_ELLIPSE_LINE") {
            QPointF sceneLinePoint1 = objectRubberPoint("ELLIPSE_LINE_POINT1");
            QPointF sceneLinePoint2 = objectRubberPoint("ELLIPSE_LINE_POINT2");
            QPointF itemLinePoint1  = mapFromScene(sceneLinePoint1);
            QPointF itemLinePoint2  = mapFromScene(sceneLinePoint2);
            QLineF itemLine(itemLinePoint1, itemLinePoint2);
            if (painter) drawRubberLine(itemLine, painter, "VIEW_COLOR_CROSSHAIR");
            updatePath();
        }
        else if (objRubberMode == "OBJ_RUBBER_ELLIPSE_MAJORDIAMETER_MINORRADIUS") {
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
        else if (objRubberMode == "OBJ_RUBBER_ELLIPSE_MAJORRADIUS_MINORRADIUS") {
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
        else if (objRubberMode == "OBJ_RUBBER_GRIP") {
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

    case OBJ_TYPE_CIRCLE: {
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
            arc.start = to_EmbVector(sceneTan1Point);
            arc.mid = to_EmbVector(sceneTan2Point);
            arc.end = to_EmbVector(sceneTan3Point);
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
        break;
    }

    case OBJ_TYPE_POINT: {
        if (objRubberMode == "OBJ_RUBBER_GRIP") {
            if (painter) {
                QPointF gripPoint = objectRubberPoint("GRIP_POINT");
                if (gripPoint == scenePos()) {
                    QLineF rubLine(mapFromScene(gripPoint), mapFromScene(objectRubberPoint(QString())));
                    drawRubberLine(rubLine, painter, "VIEW_COLOR_CROSSHAIR");
                }
            }
        }
        break;
    }

    case OBJ_TYPE_POLYGON: {
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
            //updatePath(rubberPath);

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
            // \todo fix this
            //updatePath(inscribePath);
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
            // \todo fix this
            // updatePath(circumscribePath);
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
        break;
    }

    case OBJ_TYPE_POLYLINE: {
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
            // \todo fix this
            //updatePath(rubberPath);

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
        break;
    }

    case OBJ_TYPE_RECTANGLE: {
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
                else if (gripPoint == objectTopRight()) {
                    painter->drawPolygon(mapFromScene(QRectF(objectTopLeft().x(), objectTopLeft().y()+delta.y(), objectWidth()+delta.x(), objectHeight()-delta.y())));
                }
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
        break;
    }

    case OBJ_TYPE_TEXTSINGLE: {
        if (objRubberMode == "OBJ_RUBBER_TEXTSINGLE") {
            setObjectTextFont(objectRubberText("TEXT_FONT"));
            setObjectTextJustify(objectRubberText("TEXT_JUSTIFY"));
            setObjectPos(objectRubberPoint("TEXT_POINT"));
            QPointF hr = objectRubberPoint("TEXT_HEIGHT_ROTATION");
            setObjectTextSize(hr.x());
            setRotation(hr.y());
            setObjectText(objectRubberText("TEXT_RAPID"));
        }
        else if (objRubberMode == "OBJ_RUBBER_GRIP") {
            if (painter) {
                QPointF gripPoint = objectRubberPoint("GRIP_POINT");
                if (gripPoint == scenePos()) {
                    painter->drawPath(path().translated(mapFromScene(objectRubberPoint(QString()))-mapFromScene(gripPoint)));
                }

                QLineF rubLine(mapFromScene(gripPoint), mapFromScene(objectRubberPoint(QString())));
                drawRubberLine(rubLine, painter, "VIEW_COLOR_CROSSHAIR");
            }
        }
        break;
    }

    default:
        break;
    }
}

/* . */
EmbReal
Geometry::objectRadius()
{
    return rect().width()/2.0*scale();
}

/* . */
EmbReal
Geometry::objectDiameter()
{
    return 2.0 * objectRadius() * scale();
}

/* . */
EmbReal
Geometry::objectCircumference()
{
    return CONSTANT_PI * objectDiameter();
}

/* QPointF. */
QPointF
Geometry::objectQuadrant0()
{
    if (Type == OBJ_TYPE_ELLIPSE) {
        EmbReal halfW = objectWidth()/2.0;
        EmbReal rot = radians(rotation());
        EmbVector v;
        embVector_multiply(embVector_unit(rot), halfW, &v);
        return objectCenter() + to_QPointF(v);
    }
    return objectCenter() + QPointF(objectRadius(), 0);
}

/* QPointF. */
QPointF
Geometry::objectQuadrant90()
{
    if (Type == OBJ_TYPE_ELLIPSE) {
        EmbReal halfH = objectHeight()/2.0;
        EmbReal rot = radians(rotation()+90.0);
        EmbVector v;
        embVector_multiply(embVector_unit(rot), halfH, &v);
        return objectCenter() + to_QPointF(v);
    }
    return objectCenter() + QPointF(0,-objectRadius());
}

/* QPointF. */
QPointF
Geometry::objectQuadrant180()
{
    if (Type == OBJ_TYPE_ELLIPSE) {
        EmbReal halfW = objectWidth()/2.0;
        EmbReal rot = radians(rotation()+180.0);
        EmbReal x = halfW*std::cos(rot);
        EmbReal y = halfW*std::sin(rot);
        return objectCenter() + QPointF(x,y);
    }
    return objectCenter() + QPointF(-objectRadius(),0);
}

/* QPointF. */
QPointF
Geometry::objectQuadrant270()
{
    if (Type == OBJ_TYPE_ELLIPSE) {
        EmbReal halfH = objectHeight()/2.0;
        EmbReal rot = radians(rotation()+270.0);
        EmbReal x = halfH*std::cos(rot);
        EmbReal y = halfH*std::sin(rot);
        return objectCenter() + QPointF(x,y);
    }
    return objectCenter() + QPointF(0, objectRadius());
}

/* EmbReal. */
EmbReal
Geometry::objectWidth()
{
    return rect().width()*scale();
}

/* EmbReal. */
EmbReal
Geometry::objectHeight()
{
    return rect().height()*scale();
}

/* EmbReal. */
EmbReal
Geometry::objectRadiusMajor()
{
    return std::max(rect().width(), rect().height())/2.0*scale();
}

/* EmbReal. */
EmbReal
Geometry::objectRadiusMinor()
{
    return std::min(rect().width(), rect().height())/2.0*scale();
}

/* EmbReal. */
EmbReal
Geometry::objectDiameterMajor()
{
    return std::max(rect().width(), rect().height())*scale();
}

/* EmbReal. */
EmbReal
Geometry::objectDiameterMinor()
{
    return std::min(rect().width(), rect().height())*scale();
}

/* . */
void
Geometry::setObjectCenter(EmbVector center)
{
    setPos(center.x, center.y);
}

/* . */
QRectF
Geometry::rect()
{
    return path().boundingRect();
}

/* . */
void
Geometry::setRect(EmbReal x, EmbReal y, EmbReal w, EmbReal h)
{
    QPainterPath p;
    p.addRect(x, y, w, h);
    setPath(p);
}

/* . */
void
Geometry::setLine(const QLineF& li)
{
    QPainterPath p;
    p.moveTo(li.p1());
    p.lineTo(li.p2());
    setPath(p);
    objLine = li;
}

/* . */
void 
Geometry::setLine(EmbReal x1, EmbReal y1, EmbReal x2, EmbReal y2)
{
    QPainterPath p;
    p.moveTo(x1,y1);
    p.lineTo(x2,y2);
    setPath(p);
    objLine.setLine(x1,y1,x2,y2);
}

/* Geometry::vulcanize */
void
Geometry::vulcanize(void)
{
    debug_message("DimLeaderObject vulcanize()");
    updateRubber();

    objRubberMode = "OBJ_RUBBER_OFF";

    if (Type == OBJ_TYPE_POLYGON) {
        if (!normalPath.elementCount()) {
            QMessageBox::critical(0, translate_str("Empty Polygon Error"), translate_str("The polygon added contains no points. The command that created this object has flawed logic."));
        }
    }
    if (Type == OBJ_TYPE_POLYLINE) {
        if (!normalPath.elementCount()) {
            QMessageBox::critical(0,
                translate_str("Empty Polyline Error"),
                translate_str("The polyline added contains no points. The command that created this object has flawed logic."));
        }
    }
    if (Type == OBJ_TYPE_PATH) {
        if (!normalPath.elementCount()) {
            QMessageBox::critical(0,
                translate_str("Empty Path Error"),
                translate_str("The path added contains no points. The command that created this object has flawed logic."));
        }
    }
}

/* Geometry::gripEdit
 * before, after
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

/* Geometry::objectAngle */
EmbReal
Geometry::objectAngle()
{
    EmbReal angle = objLine.angle() - rotation();
    return std::fmod(angle, 360.0);
}

/* Geometry::objectSavePath */
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

/* Geometry::calculateArcData
 * arc
 *
 * TODO: convert this to update and make it Type sensitive.
 */
void Geometry::calculateArcData(EmbArc arc)
{
    EmbVector center;
    getArcCenter(arc, &center);

    arcStartPoint = to_QPointF(embVector_subtract(arc.start, center));
    arcMidPoint = to_QPointF(embVector_subtract(arc.mid, center));
    arcEndPoint = to_QPointF(embVector_subtract(arc.end, center));

    setPos(center.x, center.y);

    EmbReal radius = embVector_distance(center, arc.mid);
    updateArcRect(radius);
    updatePath();
    setRotation(0);
    setScale(1);
}

/* Update arc rect radius. */
void
Geometry::updateArcRect(EmbReal radius)
{
    QRectF elRect;
    elRect.setWidth(radius*2.0);
    elRect.setHeight(radius*2.0);
    elRect.moveCenter(QPointF(0,0));
    setRect(elRect.x(), elRect.y(), elRect.width(), elRect.height());
}

/* Set object radius to radius. */
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

/* Set object start angle to angle. */
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

/* SetObjectEndAngle to angle. */
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

/* Geometry::setObjectPoint
 * point
 */
void
Geometry::setObjectPoint(EmbVector point, int64_t point_type)
{
    switch (point_type) {
    case ARC_START_POINT: {
        EmbArc arc;
        arc.start = point;
        arc.mid = to_EmbVector(objectMidPoint());
        arc.mid = to_EmbVector(objectEndPoint());
        calculateArcData(arc);
        break;
    }
    case ARC_MID_POINT: {
        EmbArc arc;
        arc.start = to_EmbVector(objectStartPoint());
        arc.mid = point;
        arc.end = to_EmbVector(objectEndPoint());
        calculateArcData(arc);
        break;
    }
    case ARC_END_POINT: {
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

/* Geometry::setObjectStartPoint
 * point
 */
void
Geometry::setObjectStartPoint(EmbVector point)
{
    switch (Type) {
    case OBJ_TYPE_ARC: {
        setObjectPoint(point, ARC_START_POINT);
        break;
    }
    default:
        break;
    }
}

/* Geometry::setObjectMidPoint
 * point
 */
void
Geometry::setObjectMidPoint(EmbVector point)
{
    switch (Type) {
    case OBJ_TYPE_ARC: {
        setObjectPoint(point, ARC_MID_POINT);
        break;
    }
    default:
        break;
    }
}

/* Set object endPoint to point. */
void
Geometry::setObjectEndPoint(EmbVector point)
{
    switch (Type) {
    case OBJ_TYPE_ARC: {
        setObjectPoint(point, ARC_END_POINT);
        break;
    }
    default:
        break;
    }
}

/* ObjectStartAngle */
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

/* objectEndAngle */
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

/* . */
EmbVector
rotate_vector(EmbVector v, EmbReal alpha)
{
    EmbVector rotv;
    EmbVector u = embVector_unit(alpha);
    rotv.x = v.x*u.x - v.y*u.y;
    rotv.y = v.x*u.y + v.y*u.x;
    return rotv;
}

/* ObjectStartPoint */
QPointF
Geometry::objectStartPoint()
{
    EmbReal rot = radians(rotation());
    QPointF start_point = objLine.pointAt(0.0);
    if (Type == OBJ_TYPE_ARC) {
        start_point = arcMidPoint;
    }
    EmbVector start;
    embVector_multiply(to_EmbVector(start_point), scale(), &start);
    QPointF rotv = to_QPointF(rotate_vector(start, rot));

    return scenePos() + rotv;
}

/* ObjectMidPoint */
QPointF
Geometry::objectMidPoint()
{
    EmbReal rot = radians(rotation());
    QPointF mid_point = objLine.pointAt(0.5);
    if (Type == OBJ_TYPE_ARC) {
        mid_point = arcMidPoint;
    }
    EmbVector mid;
    embVector_multiply(to_EmbVector(mid_point), scale(), &mid);
    QPointF rotv = to_QPointF(rotate_vector(mid, rot));

    return scenePos() + rotv;
}

/* ObjectEndPoint */
QPointF Geometry::objectEndPoint()
{
    EmbReal rot = radians(rotation());
    QPointF end_point = objLine.pointAt(1.0);
    if (Type == OBJ_TYPE_ARC) {
        end_point = arcEndPoint;
    }
    EmbVector end;
    embVector_multiply(to_EmbVector(end_point), scale(), &end);
    QPointF rotv = to_QPointF(rotate_vector(end, rot));

    return scenePos() + rotv;
}

/* objectArea */
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
        return CONSTANT_PI*objectRadius()*objectRadius();
    default:
        break;
    }
    return std::fabs(objectWidth()*objectHeight());
}

EmbReal
Geometry::objectReal(int64_t real_type)
{

    return 0.0f;
}

/* ObjectArcLength. */
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

/* objectChord */
EmbReal
Geometry::objectChord(void)
{
    switch (Type) {
    case OBJ_TYPE_ARC: {
        return embVector_distance(
            to_EmbVector(objectStartPoint()),
            to_EmbVector(objectEndPoint()));
    }
    default:
        break;
    }
    return 0.0f;
}

/* objectIncludedAngle */
EmbReal
Geometry::objectIncludedAngle(void)
{
    switch (Type) {
    case OBJ_TYPE_ARC: {
        EmbReal chord = objectChord();
        EmbReal rad = objectRadius();
        if (chord <= 0 || rad <= 0) return 0; //Prevents division by zero and non-existant circles

        //NOTE: Due to floating point rounding errors, we need to clamp the quotient so it is in the range [-1, 1]
        //      If the quotient is out of that range, then the result of asin() will be node_real(0.0f).
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

/* objectClockwise */
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

/* paint */
void
Geometry::paint(QPainter* painter, const QStyleOptionGraphicsItem* option, QWidget* /*widget*/)
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

    switch (Type) {
    case OBJ_TYPE_ARC: {
        EmbReal startAngle = (objectStartAngle() + rotation())*16;
        EmbReal spanAngle = objectIncludedAngle()*16;

        if (objectClockwise()) {
            spanAngle = -spanAngle;
        }

        EmbReal rad = objectRadius();
        QRectF paintRect(-rad, -rad, rad*2.0, rad*2.0);
        painter->drawArc(paintRect, startAngle, spanAngle);
        break;
    }

    case OBJ_TYPE_CIRCLE: {
        painter->drawEllipse(rect());
        break;
    }

    case OBJ_TYPE_DIMLEADER: {
        painter->drawPath(lineStylePath);
        painter->drawPath(arrowStylePath);

        if (flags & PROP_FILLED) {
            painter->fillPath(arrowStylePath, objPen.color());
        }
        break;
    }

    case OBJ_TYPE_ELLIPSE: {
        painter->drawEllipse(rect());
        break;
    }

    case OBJ_TYPE_IMAGE:
    case OBJ_TYPE_RECTANGLE: {
        painter->drawRect(rect());
        break;
    }

    case OBJ_TYPE_PATH: {
        painter->drawPath(path());
        break;
    }

    case OBJ_TYPE_POLYGON: {
        if (normalPath.elementCount()) {
            painter->drawPath(normalPath);
            QPainterPath::Element zero = normalPath.elementAt(0);
            QPainterPath::Element last = normalPath.elementAt(normalPath.elementCount()-1);
            painter->drawLine(QPointF(zero.x, zero.y), QPointF(last.x, last.y));
        }
        break;
    }

    case OBJ_TYPE_POLYLINE: {
        painter->drawPath(normalPath);

        if (objScene->property("ENABLE_LWT").toBool()
            && objScene->property("ENABLE_REAL").toBool()) {
            realRender(painter, normalPath);
        }
        break;
    }

    case OBJ_TYPE_LINE: {
        if (objRubberMode != "OBJ_RUBBER_LINE") {
            painter->drawLine(objLine);
        }

        if (objScene->property("ENABLE_LWT").toBool()
            && objScene->property("ENABLE_REAL").toBool()) {
            realRender(painter, path());
        }
        break;
    }

    case OBJ_TYPE_POINT: {
        painter->drawPoint(0,0);
        break;
    }

    case OBJ_TYPE_TEXTSINGLE: {
        painter->drawPath(objTextPath);
        break;
    }

    default:
        break;
    }
}

/* Set object diameter. */
void
Geometry::setObjectDiameter(EmbReal diameter)
{
    QRectF elRect;
    elRect.setWidth(diameter);
    elRect.setHeight(diameter);
    elRect.moveCenter(QPointF(0,0));
    setRect(elRect.x(), elRect.y(), elRect.width(), elRect.height());
    updatePath();
}

/* Set object area. */
void
Geometry::setObjectArea(EmbReal area)
{
    EmbReal radius = std::sqrt(area/CONSTANT_PI);
    setObjectRadius(radius);
}

/* Set object circumference. */
void
Geometry::setObjectCircumference(EmbReal circumference)
{
    EmbReal diameter = circumference/CONSTANT_PI;
    setObjectDiameter(diameter);
}

/* . */
void
Geometry::setObjectSize(EmbReal width, EmbReal height)
{
    QRectF elRect = rect();
    elRect.setWidth(width);
    elRect.setHeight(height);
    elRect.moveCenter(QPointF(0,0));
    setRect(elRect.x(), elRect.y(), width, height);
}

/* . */
void
Geometry::setObjectRadiusMajor(EmbReal radius)
{
    setObjectDiameterMajor(radius*2.0);
}

/* . */
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
    setRect(elRect.x(), elRect.y(), elRect.width(), elRect.height());
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
    setRect(elRect.x(), elRect.y(), elRect.width(), elRect.height());
}

/*
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

// WARNING: DO NOT enable QGraphicsItem::ItemIsMovable. If it is enabled,
// WARNING: and the item is double clicked, the scene will erratically move the item while zooming.
// WARNING: All movement has to be handled explicitly by us, not by the scene.
void
Geometry::ImageObject::init(EmbReal x, EmbReal y, EmbReal w, EmbReal h, QRgb rgb, Qt::PenStyle lineType)
{
    setData(OBJ_TYPE, OBJ_TYPE_IMAGE);
    setData(OBJ_NAME, "Image");

    setFlag(QGraphicsItem::ItemIsSelectable, true);

    setObjectRect(x, y, w, h);
    objPen.setColor(rgb);
    lwtPen.setColor(rgb);
    objPen.setStyle(lineType);
    lwtPen.setStyle(lineType);
    setObjectLineWeight("0.35"); //TODO: pass in proper lineweight
    setPen(objPen);
}
*/

/* . */
void
Geometry::setObjectRect(EmbReal x, EmbReal y, EmbReal w, EmbReal h)
{
    setPos(x, y);
    setRect(0, 0, w, h);
    updatePath();
}

/* The top left corner location as a QPointF. */
QPointF
Geometry::objectTopLeft()
{
    EmbReal rot = radians(rotation());
    QPointF tl = rect().topLeft();
    EmbVector ptl;
    embVector_multiply(to_EmbVector(tl), scale(), &ptl);
    EmbVector ptlRot = rotate_vector(ptl, rot);

    return scenePos() + to_QPointF(ptlRot);
}

/* QPointF. */
QPointF
Geometry::objectTopRight()
{
    EmbReal rot = radians(rotation());
    QPointF tr = rect().topRight();
    EmbVector ptr;
    embVector_multiply(to_EmbVector(tr), scale(), &ptr);
    EmbVector ptrRot = rotate_vector(ptr, rot);

    return (scenePos() + QPointF(ptrRot.x, ptrRot.y));
}

/* QPointF. */
QPointF
Geometry::objectBottomLeft()
{
    EmbReal rot = radians(rotation());
    QPointF bl = rect().bottomLeft();
    EmbVector pbl;
    embVector_multiply(to_EmbVector(bl), scale(), &pbl);
    EmbVector pblRot = rotate_vector(pbl, rot);

    return scenePos() + to_QPointF(pblRot);
}

/* QPointF */
QPointF
Geometry::objectBottomRight()
{
    EmbReal rot = radians(rotation());
    QPointF br = rect().bottomRight();
    EmbVector pbr;
    embVector_multiply(to_EmbVector(br), scale(), &pbr);
    EmbVector pbrRot = rotate_vector(pbr, rot);

    return scenePos() + to_QPointF(pbrRot);
}

/*
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
*/

/* WARNING: DO NOT enable QGraphicsItem::ItemIsMovable. If it is enabled,
 * WARNING: and the item is double clicked, the scene will erratically move the item while zooming.
 * WARNING: All movement has to be handled explicitly by us, not by the scene.
 */
/*
void
Geometry::init(EmbVector position, const QPainterPath& p, QRgb rgb, Qt::PenStyle lineType)
{
    setData(OBJ_TYPE, OBJ_TYPE_PATH);
    setData(OBJ_NAME, "Path");

    setFlag(QGraphicsItem::ItemIsSelectable, true);

    normalPath = p;
    updatePath();
    setObjectPos(position.x, position.y);
    objPen.setColor(rgb);
    lwtPen.setColor(rgb);
    objPen.setStyle(lineType);
    lwtPen.setStyle(lineType);
    setObjectLineWeight("0.35"); //TODO: pass in proper lineweight
    setPen(objPen);
}
*/

/* objectCopyPath */
QPainterPath
Geometry::objectCopyPath()
{
    return normalPath;
}

/* Update path.
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

/* Find index of a point within a path.
 * Return as an int, if not found return -1.
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

/* UpdatePath */
void
Geometry::updatePath(const QPainterPath& p)
{
    normalPath = p;
    QPainterPath reversePath = normalPath.toReversed();
    reversePath.connectPath(normalPath);
    setPath(reversePath);
}

/* Returns whether the save to file process was successful.
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
save(View* view, QString fileName)
{
    qDebug("SaveObject save(%s)", qPrintable(fileName));
    QGraphicsScene* gscene = view->scene();
    QUndoStack* stack = view->getUndoStack();
    if (!(gscene && stack)) {
        return false;
    }

    bool writeSuccessful = false;

    view->formatType = formatTable[emb_identify_format(qPrintable(fileName))].type;
    if (view->formatType == EMBFORMAT_UNSUPPORTED) {
        return false;
    }

    EmbPattern* pattern = embPattern_create();
    if (!pattern) {
        debug_message("Could not allocate memory for embroidery pattern");
        return false;
    }

    /* Write */
    if (emb_identify_format(qPrintable(fileName)) < 0) {
        qDebug("Unsupported write file type: %s", qPrintable(fileName));
    }
    else {
        foreach(QGraphicsItem* item, gscene->items(Qt::AscendingOrder)) {
            int objType = item->data(OBJ_TYPE).toInt();

            switch (objType) {
            case OBJ_TYPE_ARC: {
                addArc(view, item);
                break;
            }
            case OBJ_TYPE_BLOCK: {
                addBlock(view, item);
                break;
            }
            case OBJ_TYPE_CIRCLE: {
                addCircle(view, item);
                break;
            }
            case OBJ_TYPE_DIMALIGNED: {
                addDimAligned(view, item);
                break;
            }
            case OBJ_TYPE_DIMANGULAR: {
                addDimAngular(view, item);
                break;
            }
            case OBJ_TYPE_DIMARCLENGTH: {
                addDimArcLength(view, item);
                break;
            }
            case OBJ_TYPE_DIMDIAMETER: {
                addDimDiameter(view, item);
                break;
            }
            case OBJ_TYPE_DIMLEADER: {
                addDimLeader(view, item);
                break;
            }
            case OBJ_TYPE_DIMLINEAR: {
                addDimLinear(view, item);
                break;
            }
            case OBJ_TYPE_DIMORDINATE: {
                addDimOrdinate(view, item);
                break;
            }
            case OBJ_TYPE_DIMRADIUS: {
                addDimRadius(view, item);
                break;
            }
            case OBJ_TYPE_ELLIPSE: {
                addEllipse(view, item);
                break;
            }
            case OBJ_TYPE_ELLIPSEARC: {
                addEllipseArc(view, item);
                break;
            }
            case OBJ_TYPE_GRID: {
                addGrid(view, item);
                break;
            }
            case OBJ_TYPE_HATCH: {
                addHatch(view, item);
                break;
            }
            case OBJ_TYPE_IMAGE: {
                addImage(view, item);
                break;
            }
            case OBJ_TYPE_INFINITELINE: {
                addInfiniteLine(view, item);
                break;
            }
            case OBJ_TYPE_LINE: {
                addLine(view, item);
                break;
            }
            case OBJ_TYPE_POINT: {
                addPoint(view, item);
                break;
            }
            case OBJ_TYPE_POLYGON: {
                addPolygon(view, item);
                break;
            }
            case OBJ_TYPE_POLYLINE: {
                addPolyline(view, item);
                break;
            }
            case OBJ_TYPE_RAY: {
                addRay(view, item);
                break;
            }
            case OBJ_TYPE_RECTANGLE: {
                addRectangle(view, item);
                break;
            }
            case OBJ_TYPE_SPLINE: {
                addSpline(view, item);
                break;
            }
            case OBJ_TYPE_TEXTMULTI: {
                addTextMulti(view, item);
                break;
            }
            case OBJ_TYPE_TEXTSINGLE: {
                addTextSingle(view, item);
                break;
            }
            default: {
                break;
            }
            }
        }

        /*
        //TODO: handle EMBFORMAT_STCHANDOBJ also
        if (view->formatType == EMBFORMAT_STITCHONLY)
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

/* Add arc "item" to "pattern". */
void
addArc(View* view, QGraphicsItem* item)
{
}

/* Add block "item" to "pattern". */
void
addBlock(View* view, QGraphicsItem* item)
{
}

/* Add circle "item" to "pattern". */
void
addCircle(View* view, QGraphicsItem* item)
{
    Geometry* obj = static_cast<Geometry*>(item);
    if (obj) {
        if (view->formatType == EMBFORMAT_STITCHONLY) {
            QPainterPath path = obj->objectSavePath();
            toPolyline(view, obj->objectCenter(), path.simplified(), "0", obj->objPen.color(), "CONTINUOUS", "BYLAYER"); //TODO: proper layer/lineType/lineWeight //TODO: Improve precision, replace simplified
        }
        else {
            EmbCircle circle;
            circle.center.x = (double)obj->objectCenter().x();
            circle.center.y = (double)obj->objectCenter().y();
            circle.radius = (double)obj->objectRadius();

            embPattern_addCircleAbs(view->pattern, circle);
        }
    }
}

/* Add aligned dimension "item" to "pattern". */
void
addDimAligned(View* view, QGraphicsItem* item)
{
    Geometry* obj = static_cast<Geometry*>(item);
    if (obj) {

    }
}

/* Add angular dimension "item" to "pattern". */
void
addDimAngular(View* view, QGraphicsItem* item)
{
    Geometry* obj = static_cast<Geometry*>(item);
    if (obj) {

    }
}

/* Add arc length dimension "item" to "pattern". */
void
addDimArcLength(View* view, QGraphicsItem* item)
{
    Geometry* obj = static_cast<Geometry*>(item);
    if (obj) {

    }
}

/* Add diameter dimension "item" to "pattern". */
void
addDimDiameter(View* view, QGraphicsItem* item)
{
    Geometry* obj = static_cast<Geometry*>(item);
    if (obj) {

    }
}

/* Add dimension leader "item" to "pattern". */
void
addDimLeader(View* view, QGraphicsItem* item)
{
    Geometry* obj = static_cast<Geometry*>(item);
    if (obj) {

    }
}

/* Add linear dimension "item" to "pattern". */
void
addDimLinear(View* view, QGraphicsItem* item)
{
    Geometry* obj = static_cast<Geometry*>(item);
    if (obj) {

    }
}

/* Add ordinate dimension "item" to "pattern". */
void
addDimOrdinate(View* view, QGraphicsItem* item)
{
    Geometry* obj = static_cast<Geometry*>(item);
    if (obj) {
        /* view->pattern */
    }
}

/* Add radius dimension "item" to "pattern". */
void
addDimRadius(View* view, QGraphicsItem* item)
{
    Geometry* obj = static_cast<Geometry*>(item);
    if (obj) {
        /* view->pattern */
    }
}

/* Add ellipse "item" to "pattern". */
void
addEllipse(View* view, QGraphicsItem* item)
{
    Geometry* obj = static_cast<Geometry*>(item);
    if (obj) {
        if (view->formatType == EMBFORMAT_STITCHONLY) {
            QPainterPath path = obj->objectSavePath();
            toPolyline(view, obj->objectCenter(), path.simplified(), "0", obj->objPen.color(), "CONTINUOUS", "BYLAYER"); //TODO: proper layer/lineType/lineWeight //TODO: Improve precision, replace simplified
        }
        else {
            EmbEllipse ellipse;
            ellipse.center.x = (double)obj->objectCenter().x();
            ellipse.center.y = (double)obj->objectCenter().y();
            ellipse.radius.x = (double)obj->objectWidth()/2.0;
            ellipse.radius.y = (double)obj->objectHeight()/2.0;
            //TODO: ellipse rotation
            embPattern_addEllipseAbs(view->pattern, ellipse);
        }
    }
}

/* Add elliptical arc "item" to "pattern". */
void
addEllipseArc(View* view, QGraphicsItem* item)
{
    Geometry* obj = static_cast<Geometry*>(item);
    if (obj) {
        /* view->pattern */
    }
}

/* Add grid "item" to "pattern". */
void
addGrid(View* view, QGraphicsItem* item)
{
    Geometry* obj = static_cast<Geometry*>(item);
    if (obj) {
        /* view->pattern */
    }
}

/* Add hatch "item" to "pattern". */
void
addHatch(View* view, QGraphicsItem* item)
{
    Geometry* obj = static_cast<Geometry*>(item);
    if (obj) {
        /* view->pattern */
    }
}

/* Add image "item" to "pattern". */
void
addImage(View* view, QGraphicsItem* item)
{
    Geometry* obj = static_cast<Geometry*>(item);
    if (obj) {
        /* view->pattern */
    }
}

/* Add infinite line "item" to "pattern". */
void
addInfiniteLine(View* view, QGraphicsItem* item)
{
    Geometry* obj = static_cast<Geometry*>(item);
    if (obj) {
        /* view->pattern */
    }
}

/* Add line "item" to "pattern". */
void
addLine(View* view, QGraphicsItem* item)
{
    Geometry* obj = static_cast<Geometry*>(item);
    if (obj) {
        if (view->formatType == EMBFORMAT_STITCHONLY) {
            toPolyline(view, obj->objectEndPoint1(), obj->objectSavePath(), "0", obj->objPen.color(), "CONTINUOUS", "BYLAYER"); //TODO: proper layer/lineType/lineWeight
        }
        else {
            EmbLine line;
            line.start = to_EmbVector(obj->objectEndPoint1());
            line.end = to_EmbVector(obj->objectEndPoint2());
            embPattern_addLineAbs(view->pattern, line);
        }
    }
}

/* Add path "item" to "pattern".
 *
 * \todo Reimplement addPolyline() using the libembroidery C API
 */
void
addPath(View* view, QGraphicsItem* item)
{
    qDebug("addPolyline()");
    QGraphicsPathItem* polylineItem = (QGraphicsPathItem*)item;
    if (polylineItem) {
        QPainterPath path = polylineItem->path();
        EmbVector start = to_EmbVector(polylineItem->pos());

        for (int i = 0; i < path.elementCount()-1; ++i) {
            QPainterPath::Element element = path.elementAt(i);
            /*
            if (element.isMoveTo()) {
                embPattern_addStitchAbs(view->pattern, (element.x + start.x), -(element.y + start.y), TRIM);
            }
            else if (element.isLineTo()) {
                embPattern_addStitchAbs(view->pattern, (element.x + start.x), -(element.y + start.y), NORMAL);
            }
            else if (element.isCurveTo()) {
                QPainterPath::Element P1 = path.elementAt(i-1); // start point
                QPainterPath::Element P2 = path.elementAt(i);   // control point
                QPainterPath::Element P3 = path.elementAt(i+1); // control point
                QPainterPath::Element P4 = path.elementAt(i+2); // end point

                embPattern_addStitchAbs(P4.x, -P4.y, NORMAL); //TODO: This is temporary
                //TODO: Curved Polyline segments are always arcs
            }
            */
        }
        /* embPattern_addStitchRel(view->pattern, 0, 0, STOP); */
        QColor c= polylineItem->pen().color();
        /* embPattern_addThread(view->pattern, c.red(), c.green(), c.blue(), "", ""); */
    }
}

/* Add point "item" to "pattern". */
void
addPoint(View* view, QGraphicsItem* item)
{
    Geometry* obj = static_cast<Geometry*>(item);
    if (obj) {
        if (view->formatType == EMBFORMAT_STITCHONLY) {
            toPolyline(view, obj->objectPos(), obj->objectSavePath(), "0", obj->objPen.color(), "CONTINUOUS", "BYLAYER"); //TODO: proper layer/lineType/lineWeight
        }
        else {
            EmbPoint po;
            po.position.x = (double)obj->objectPos().x();
            po.position.y = (double)obj->objectPos().y();
            embPattern_addPointAbs(view->pattern, po);
        }
    }
}

/* Add polygon "item" to "pattern". */
void
addPolygon(View* view, QGraphicsItem* item)
{
    Geometry* obj = static_cast<Geometry*>(item);
    if (obj) {
        toPolyline(view, obj->objectPos(), obj->objectSavePath(), "0", obj->objPen.color(), "CONTINUOUS", "BYLAYER"); //TODO: proper layer/lineType/lineWeight
    }
}

/* Add polyline "item" to "pattern". */
void
addPolyline(View* view, QGraphicsItem* item)
{
    Geometry* obj = static_cast<Geometry*>(item);
    if (obj) {
        toPolyline(view, obj->objectPos(), obj->objectSavePath(), "0", obj->objPen.color(), "CONTINUOUS", "BYLAYER"); //TODO: proper layer/lineType/lineWeight
    }
}

/* Add ray "item" to "pattern". */
void
addRay(View* view, QGraphicsItem* item)
{
    Geometry* obj = static_cast<Geometry*>(item);
    if (obj) {
        /* view->pattern */
    }
}

/* Add rectangle "item" to "pattern". */
void
addRectangle(View* view, QGraphicsItem* item)
{
    Geometry* obj = static_cast<Geometry*>(item);
    if (obj) {
        if (view->formatType == EMBFORMAT_STITCHONLY) {
            toPolyline(view, obj->objectPos(), obj->objectSavePath(), "0", obj->objPen.color(), "CONTINUOUS", "BYLAYER"); //TODO: proper layer/lineType/lineWeight
        }
        else {
            //TODO: Review this at some point
            QPointF topLeft = obj->objectTopLeft();
            EmbRect r;
            r.top = topLeft.x();
            r.left = topLeft.y();
            r.right = r.left + (double)obj->objectWidth();
            r.bottom = r.top + (double)obj->objectHeight();
            embPattern_addRectAbs(view->pattern, r);
        }
    }
}

/* Add slot "item" to "pattern". */
void
addSlot(View* view, QGraphicsItem* item)
{
    Geometry* obj = static_cast<Geometry*>(item);
    if (obj) {
        /* view->pattern */
    }
}

/* Add spline "item" to "pattern". */
void
addSpline(View* view, QGraphicsItem* item)
{
    //TODO: abstract bezier into geom-bezier... cubicBezierMagic(P1, P2, P3, P4, 0.0, 1.0, tPoints);
    Geometry* obj = static_cast<Geometry*>(item);
    if (obj) {
        /* view->pattern */
    }
}

/* Add text multi "item" to "pattern". */
void
addTextMulti(View* view, QGraphicsItem* item)
{
    //TODO: saving polygons, polylines and paths must be stable before we go here.
    Geometry* obj = static_cast<Geometry*>(item);
    if (obj) {
        /* view->pattern */
    }
}

/* Add text single "item" to "pattern".
 *
 * TODO: saving polygons, polylines and paths must be stable before we go here.
 *
 * TODO: This needs to work like a path, not a polyline. Improve this.
 */
void
addTextSingle(View* view, QGraphicsItem* item)
{
    Geometry* obj = static_cast<Geometry*>(item);
    if (obj) {
        if (view->formatType == EMBFORMAT_STITCHONLY) {
            std::vector<QPainterPath> pathList = obj->objectSavePathList();
            foreach(QPainterPath path, pathList) {
                toPolyline(view, obj->objectPos(), path.simplified(), "0", obj->objPen.color(), "CONTINUOUS", "BYLAYER"); //TODO: proper layer/lineType/lineWeight //TODO: Improve precision, replace simplified
            }
        }
        else {

        }
    }
}

/* toPolyline
 *
 * NOTE: This function should be used to interpret various object types
 * and save them as polylines for stitchOnly formats.
 */
void
toPolyline(
    View* view,
    QPointF objPos,
    QPainterPath objPath,
    QString layer,
    QColor color,
    QString lineType,
    QString lineWeight)
{
    EmbArray* pointList = 0;
    QPainterPath::Element element;
    for (int i = 0; i < objPath.elementCount(); ++i) {
        element = objPath.elementAt(i);
        if (!pointList) {
            pointList = embArray_create(EMB_POINT);
        }
        EmbPoint po;
        po.position.x = element.x + objPos.x();
        po.position.y = -(element.y + objPos.y());
        embArray_addPoint(pointList, po);
    }

    EmbColor color_out;
    color_out.r = color.red();
    color_out.g = color.green();
    color_out.b = color.blue();

    /**
    TODO: FIX
    EmbPolyline* polyObject = embPolyline_init(pointList, color_out, 1); //TODO: proper lineType
    embPattern_addPolylineAbs(view->pattern, polyObject);
    */
}

/* Set object text. */
void Geometry::setObjectText(QString str)
{
    objText = str;
    QPainterPath textPath;
    QFont font;
    font.setFamily(objTextFont);
    font.setPointSizeF(text_size);
    font.setBold(flags & PROP_BOLD);
    font.setItalic(flags & PROP_ITALIC);
    font.setUnderline(flags & PROP_UNDERLINE);
    font.setStrikeOut(flags & PROP_STRIKEOUT);
    font.setOverline(flags & PROP_OVERLINE);
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
        //TODO: Geometry Aligned Justification
    }
    else if (objTextJustify == "Middle") {
        textPath.translate(-jRect.center());
    }
    else if (objTextJustify == "Fit") {
        //TODO: Geometry Fit Justification
    }
    else if (objTextJustify == "Top Left") {
        textPath.translate(-jRect.topLeft());
    }
    else if (objTextJustify == "Top Center") {
        textPath.translate(-jRect.center().x(), -jRect.top());
    }
    else if (objTextJustify == "Top Right") {
        textPath.translate(-jRect.topRight());
    }
    else if (objTextJustify == "Middle Left") {
        textPath.translate(-jRect.left(), -jRect.top()/2.0);
    }
    else if (objTextJustify == "Middle Center") {
        textPath.translate(-jRect.center().x(), -jRect.top()/2.0);
    }
    else if (objTextJustify == "Middle Right") {
        textPath.translate(-jRect.right(), -jRect.top()/2.0);
    }
    else if (objTextJustify == "Bottom Left") {
        textPath.translate(-jRect.bottomLeft());
    }
    else if (objTextJustify == "Bottom Center") {
        textPath.translate(-jRect.center().x(), -jRect.bottom());
    }
    else if (objTextJustify == "Bottom Right") {
        textPath.translate(-jRect.bottomRight());
    }

    /* Backward or Upside Down */
    if (flags & (PROP_BACKWARD + PROP_UPSIDEDOWN)) {
        EmbReal horiz = 1.0;
        EmbReal vert = 1.0;
        if (flags & PROP_BACKWARD) {
            horiz = -1.0;
        }
        if (flags & PROP_UPSIDEDOWN) {
            vert = -1.0;
        }

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

/* font
 */
void
Geometry::setObjectTextFont(QString font)
{
    objTextFont = font;
    setObjectText(objText);
}

/* Set the text justification.
 * Verify the string is a valid option, otherwise default to "Left".
 */
void
Geometry::setObjectTextJustify(QString justify)
{
    objTextJustify = "Left";
    String justify_ = justify.toStdString();
    for (int i=0; strcmp(justify_options[i], "END"); i++) {
        if (!strcmp(justify_.c_str(), justify_options[i])) {
            objTextJustify = justify;
        }
    }
    setObjectText(objText);
}

/* size
 */
void
Geometry::setObjectTextSize(EmbReal size)
{
    text_size = size;
    setObjectText(objText);
}

/* bold, italic, under, strike, over */
void
Geometry::setObjectTextStyle(bool bold, bool italic, bool under, bool strike, bool over)
{
    flags |= bold * PROP_BOLD;
    flags |= italic * PROP_ITALIC;
    flags |= under * PROP_UNDERLINE;
    flags |= strike * PROP_STRIKEOUT;
    flags |= over * PROP_OVERLINE;
    setObjectText(objText);
}

/* val
 */
void
Geometry::setObjectTextBold(bool val)
{
    flags |= val * PROP_BOLD;
    setObjectText(objText);
}

/* . */
void
Geometry::setObjectTextItalic(bool val)
{
    flags |= val * PROP_ITALIC;
    setObjectText(objText);
}

/* . */
void
Geometry::setObjectTextUnderline(bool val)
{
    flags |= val * PROP_UNDERLINE;
    setObjectText(objText);
}

/* . */
void
Geometry::setObjectTextStrikeOut(bool val)
{
    flags |= val * PROP_STRIKEOUT;
    setObjectText(objText);
}

/* . */
void
Geometry::setObjectTextOverline(bool val)
{
    flags |= val * PROP_OVERLINE;
    setObjectText(objText);
}

/* . */
void
Geometry::setObjectTextBackward(bool val)
{
    flags |= val * PROP_BACKWARD;
    setObjectText(objText);
}

/* . */
void
Geometry::setObjectTextUpsideDown(bool val)
{
    flags |= val * PROP_UPSIDEDOWN;
    setObjectText(objText);
}

/* Returns std::vector<QPainterPath>. */
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

/* Run initialisation script for this object, based on the
 * object Type.
 */
void
Geometry::script_main(void)
{
    /*
    std::vector<std::string> script = {"end"};
    switch (mode) {

    case OBJ_TYPE_CIRCLE: {
		"init",
		"clear_selection",
		"# FIX SELECTING CURRENT OBJECT",
		"select this",
		"set mode CIRCLE_MODE_1P_RAD",
		"set-prompt-prefix-tr Specify center point for circle or [3P/2P/TTR (tan tan radius)]: "
        break;
    }

    case OBJ_TYPE_ELLIPSE: {
        script = scripts["ellipse_init"];
        break;
    }

    case DISTANCE: {
        script = {
            "init",
            "clear-selection",
            "set-prompt-prefix-tr Specify first point: "
        };
        break;
    }

    case DOLPHIN: {
		"init",
		"clear-selection",
		"set mode DOLPHIN_MODE_NUM_POINTS",
		"# FIX SELECTING CURRENT OBJECT",
		"select this",
		"set-selected numPoints 512",
		"set-selected minPoints 64",
		"set-selected maxPoints 8192",
		"set-selected center_x 0.0f",
		"set-selected center_y 0.0f",
		"set-selected scale_x 0.04f",
		"set-selected scale_y 0.04f",
		"add-rubber-selected POLYGON",
		"set-rubber-mode POLYGON",
		"update-dolphin numPoints scale_x scale_y",
		"spare-rubber POLYGON",
		"end"
        break;
    }



ellipse_init_script = [
    "init",
    "clear-selection",
    "# FIX SELECTING CURRENT OBJECT",
    "select this",
    "set mode ELLIPSE_MODE_MAJORDIAMETER_MINORRADIUS",
    "set-selected height 1.0",
    "set-selected width 2.0",
    "set-selected rotation 0.0",
    "set-prompt-prefix-tr Specify first axis start point or [Center]: "
]

polyline_init_script = [
    "init",
    "clear-selection",
    "# FIX SELECTING CURRENT OBJECT",
    "select this",
    "set-selected firstRun true",
    "set-selected first_x 0.0f",
    "set-selected first.y 0.0f",
    "set-selected prev_x 0.0f",
    "set-selected prev_y 0.0f",
    "set-selected num 0",
    "set-prompt-prefix-tr Specify first point: "
]

snowflake_init_script = [
    "init",
    "clear-selection",
    "numPoints=2048",
    "minPoitns=64",
    "maxPoints=8192",
    "center.x=0.0f",
    "center.y=0.0f",
    "scale.x=0.04f",
    "scale.y=0.04f",
    "mode=SNOWFLAKE_MODE_NUM_POINTS",
    "add-rubber POLYGON",
    "set-rubber-mode POLYGON",
    "update-snowflake",
    "spare-rubber POLYGON",
    "end"
]

    default: {
        break;
    }

    }
    */
}

/* Script to change the entries in the context menu when acting
 * on this object. Also, if one is activated carry out that
 * script.
 */
void
Geometry::script_context(String str)
{
    switch (mode) {

    /* . */
    case MODE_CIRCLE_1P_RAD: {
        actuator("todo CIRCLE context()");
        break;
    }

    /* . */
    default: {
        break;
    }

    }
}

/* Script to run on each click for this geometry object.
 *
 * The modes are documented in detail in the reference manual.
 */
void
Geometry::script_click(EmbVector v)
{
    switch (mode) {

/*
    case MODE_CIRCLE_1P_RAD: {
        auto iter = properties.find("point1");
        if (iter == properties.end()) {
            point1 = v;
            center = v;
            addRubber("CIRCLE");
            setRubberMode("CIRCLE_1P_RAD");
            setRubberPoint("CIRCLE_CENTER", properties["center.x"].r, properties["center.y"].r);
            actuator("append-prompt");
            actuator("set-prompt-prefix-tr Specify radius of circle or [Diameter]: ");
        }
        else {
            point2 = v;
            setRubberPoint("CIRCLE_RADIUS", v);
            actuator("vulcanize");
            actuator("append-prompt");
            actuator("end");
        }
        break;
    }

    case MODE_CIRCLE_1P_DIA: {
        auto iter = properties.find("point1");
        if (iter == properties.end()) {
            error("CIRCLE", tr("This should never happen."));
        }
        else {
            point2.x = node_real(v.x);
            point2.y = node_real(v.y);
            setRubberPoint("CIRCLE_DIAMETER", point2.x.r, point2.y.r);
            actuator("vulcanize");
            actuator("append-prompt-history");
            actuator("end");
        }
        break;
    }

    case MODE_CIRCLE_2P: {
        auto iter1 = properties.find("point1");
        auto iter2 = properties.find("point2");
        if (iter1 == properties.end()) {
            properties["point1"] = node_vector(v);
            addRubber("CIRCLE");
            setRubberMode("CIRCLE_2P");
            setRubberPoint("CIRCLE_TAN1", v);
            actuator("append-prompt-history");
            actuator("set-prompt-prefix-tr Specify second end point of circle's diameter: ");
        }
        else if (iter2 == properties.end()) {
            properties["point2"] = node_vector(v);
            setRubberPoint("CIRCLE_TAN2", v);
            actuator("vulcanize");
            actuator("append-prompt-history");
            actuator("end");
        }
        else {
            error("CIRCLE", tr("This should never happen."));
        }
        break;
    }

    case CIRCLE_MODE_3P: {
        if (std::isnan(point1.x.r)) {
            point1.x = node_real(x);
            point1.y = node_real(y);
            actuator("append-prompt-history");
            actuator("set-prompt-prefix-tr Specify second point on circle: ");
        }
        else if (std::isnan(point2.x)) {
            point2.x = node_real(x);
            point2.y = node_real(y);
            addRubber("CIRCLE");
            setRubberMode("CIRCLE_3P");
            setRubberPoint("CIRCLE_TAN1", point1.x.r, point1.y.r);
            setRubberPoint("CIRCLE_TAN2", point2.x.r, point2.y.r);
            run_script({
                "append-prompt-history",
                "set-prompt-prefix-tr Specify third point on circle: "
            });
        }
        else if (std::isnan(point3.x)) {
            point3.x = node_real(x);
            point3.y = node_real(y);
            setRubberPoint("CIRCLE_TAN3", point3.x.r, point3.y.r);
            run_script({
                "vulcanize",
                "append-prompt-history",
                "end"
            });
        }
    }

    case CIRCLE_MODE_TTR: {
        point1.x = node_real(x);
        point1.y = node_real(y);
        actuator("append-prompt-history");
        actuator("set-prompt-prefix-tr Specify point on object for second tangent of circle: ");
        mode = MODE_TTR_SET_POINT_2;
        break;
    }

    case CIRCLE_MODE_TTR_SET_POINT_2: {
        point2.x = node_real(x);
        point2.y = node_real(y);
        actuator("append-prompt-history");
        actuator("set-prompt-prefix-tr Specify radius of circle: ");
        mode = MODE_TTR_SET_POINT_3;
        break;
    }

    case CIRCLE_MODE_TTR_SET_POINT_3: {
        point3.x = node_real(x);
        point3.y = node_real(y);
        actuator("append-prompt-history");
        actuator("set-prompt-prefix-tr Specify second point: ");
        mode = MODE_CIRCLE_DEFAULT;
        break;
    }

    default: {
        error("CIRCLE", tr("This should never happen."));
        break;
    }
*/

    }
}

/* Script to control the behavior of the prompt when processing
 * events for this object.
 */
void
Geometry::script_prompt(String str)
{
    switch (mode) {

/*
    case MODE_CIRCLE_1P_RAD: {
        if (std::isnan(point1.x.r)) {
            if (str == "2P") {
                mode = MODE_CIRCLE_2P;
                actuator("set-prompt-prefix-tr Specify first end point of circle's diameter: ");
            }
            else if (str == "3P") {
                mode = MODE_CIRCLE_3P;
                actuator("set-prompt-prefix-tr Specify first point of circle: ");
            }
            else if (str == "T" || str == "TTR") {
                mode = MODE_CIRCLE_TTR;
                actuator("set-prompt-prefix-tr Specify point on object for first tangent of circle: ");
            }
            else {
                EmbReal strList = str.split(",");
                if (std::isnan(strList[0]) || std::isnan(strList[1])) {
                    alert(tr("Point or option keyword required."));
                    actuator("set-prompt-prefix-tr Specify center point for circle or [3P/2P/Ttr (tan tan radius)]: ");
                }
                else {
                    x1 = std::stof(strList[0]);
                    point1.y = node_real(strList[1]);
                    properties["center.x"] = point1.x;
                    properties["center.y"] = point1.y;
                    addRubber("CIRCLE");
                    setRubberMode("CIRCLE_1P_RAD");
                    setRubberPoint("CIRCLE_CENTER", properties["center.x"], properties["center.y"]);
                    actuator("set-prompt-prefix-tr Specify radius of circle or [Diameter]: ");
                }
            }
        }
        else {
            if (str == "D" || str == "DIAMETER") {
                mode = MODE_CIRCLE_1P_DIA;
                setRubberMode("CIRCLE_1P_DIA");
                actuator("set-prompt-prefix-tr Specify diameter of circle: ");
            }
            else {
                EmbReal num = node_real(str);
                if (std::isnan(num)) {
                    alert(tr("Requires numeric radius, point on circumference, or \"D\"."));
                    actuator("set-prompt-prefix-tr Specify radius of circle or [Diameter]: "));
                }
                else {
                    properties["rad"] = num;
                    point2.x = point1.x + properties["rad"];
                    point2.y = point1.y;
                    setRubberPoint("CIRCLE_RADIUS", point2.x, point2.y);
                    actuator("vulcanize");
                    actuator("end");
                }
            }
        }
    }

    case MODE_CIRCLE_1P_DIA: {
        if (std::isnan(point1.x.r)) {
            error("CIRCLE", tr("This should never happen."));
        }
        if (std::isnan(point2.x)) {
            EmbReal num = node_real(str);
            if (std::isnan(num)) {
                alert(tr("Requires numeric distance or second point."));
                actuator("set-prompt-prefix-tr Specify diameter of circle: "));
            }
            else {
                properties["dia"] = num;
                point2.x = point1.x.r + properties["dia"].r;
                point2.y = point1.y.r;
                setRubberPoint("CIRCLE_DIAMETER", point2.x, point2.y);
                actuator("vulcanize");
                actuator("end");
            }
        }
        else {
            error("CIRCLE", tr("This should never happen."));
        }
    }

    case MODE_CIRCLE_2P: {
        if (std::isnan(point1.x.r)) {
            EmbReal strList = str.split(",");
            if (std::isnan(strList[0]) || std::isnan(strList[1])) {
                alert(tr("Invalid point."));
                actuator("set-prompt-prefix-tr Specify first end point of circle's diameter: ");
            }
            else {
                x1 = node_real(strList[0]);
                y1 = node_real(strList[1]);
                addRubber("CIRCLE");
                setRubberMode("CIRCLE_2P");
                setRubberPoint("CIRCLE_TAN1", point1.x, point1.y);
                actuator("set-prompt-prefix-tr Specify second end point of circle's diameter: ");
            }
        }
        else if (std::isnan(point2.x)) {
            EmbReal strList = str.split(",");
            if (std::isnan(strList[0]) || std::isnan(strList[1])) {
                alert(tr("Invalid point."));
                actuator("set-prompt-prefix-tr Specify second end point of circle's diameter: ");
            }
            else {
                x2 = node_real(strList[0]);
                y2 = node_real(strList[1]);
                setRubberPoint("CIRCLE_TAN2", point2.x, point2.y);
                actuator("vulcanize");
                actuator("end");
            }
        }
        else {
            error("CIRCLE", tr("This should never happen."));
        }
    }

    case MODE_CIRCLE_3P: {
        if (std::isnan(point1.x.r)) {
            EmbReal strList = str.split(",");
            if (std::isnan(strList[0]) || std::isnan(strList[1])) {
                alert(tr("Invalid point."));
                actuator("set-prompt-prefix-tr Specify first point of circle: ");
            }
            else {
                point1.x = node_real(strList[0]);
                point1.y = node_real(strList[1]);
                actuator("set-prompt-prefix-tr Specify second point of circle: ");
            }
        }
        else if (std::isnan(point2.x)) {
            EmbReal strList = str.split(",");
            if (std::isnan(strList[0]) || std::isnan(strList[1])) {
                alert(tr("Invalid point."));
                actuator("set-prompt-prefix-tr Specify second point of circle: ");
            }
            else {
                point2.x = node_real(strList[0]);
                point2.y = node_real(strList[1]);
                addRubber("CIRCLE");
                setRubberMode("CIRCLE_3P");
                setRubberPoint("CIRCLE_TAN1", point1.x, point1.y);
                setRubberPoint("CIRCLE_TAN2", point2.x, point2.y);
                actuator("set-prompt-prefix-tr Specify third point of circle: ");
            }
        }
        else if (std::isnan(point3.x.r)) {
            EmbReal strList = str.split(",");
            if (std::isnan(strList[0]) || std::isnan(strList[1])) {
                alert(tr("Invalid point."));
                actuator("set-prompt-prefix-tr Specify third point of circle: ");
            }
            else {
                point3.x = node_real(strList[0]);
                point3.y = node_real(strList[1]);
                setRubberPoint("CIRCLE_TAN3", point3.x, point3.y);
                actuator("vulcanize");
                actuator("end");
            }
        }
        else {
            error("CIRCLE", tr("This should never happen."));
        }
    }

    case MODE_CIRCLE_TTR: {
        actuator("todo CIRCLE prompt() for TTR");
        break;
    }

    default: {
        break;
    }
*/
    }
}

#if 0

/* . */
void
Geometry::distance_click(EmbVector v)
{
    if (std::isnan(point1.x.r)) {
        point1 = v;
        addRubber("LINE");
        setRubberMode("LINE");
        setRubberPoint("LINE_START", point1.x, point1.y);
        actuator("append-prompt-history");
        actuator("set-prompt-prefix-tr Specify second point: "));
    }
    else {
        actuator("append-prompt-history");
        point2 = v;
        reportDistance();
        actuator("end");
    }
}

/* . */
void
Geometry::distance_context(String args)
{
    actuator("todo DISTANCE context()");
}

/* . */
void
Geometry::distance_prompt(String args)
{
    EmbReal strList = str.split(",");
    if (std::isnan(point1.x)) {
        if (std::isnan(strList[0]) || std::isnan(strList[1])) {
            alert(tr("Requires numeric distance or two points."));
            actuator("set-prompt-prefix-tr Specify first point: "));
        }
        else {
            point1.x = node_real(strList[0]);
            point1.y = node_real(strList[1]);
            addRubber("LINE");
            setRubberMode("LINE");
            setRubberPoint("LINE_START", point1.x, point1.y);
            actuator("set-prompt-prefix-tr Specify second point: "));
        }
    }
    else {
        if (std::isnan(strList[0]) || std::isnan(strList[1])) {
            alert(tr("Requires numeric distance or two points."));
            actuator("set-prompt-prefix-tr Specify second point: "));
        }
        else {
            point2.x = node_real(strList[0]);
            point2.y = node_real(strList[1]);
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
    EmbReal dx = point2.x - point1.x;
    EmbReal dy = point2.y - point1.y;

    EmbReal dist = calculateDistance(point1.x, point1.y, point2.x, point2.y);
    EmbReal angle = calculateAngle(point1.x, point1.y, point2.x, point2.y);

    actuator("set-prompt-prefix " + tr("Distance") + " = " + dist.toString() + ", " + tr("Angle") + " = " + angle.toString());
    actuator("append-prompt-history");
    actuator("set-prompt-prefix " + tr("Delta X") + " = " + dx.toString() + ", " + tr("Delta Y") + " = " + dy.toString());
    actuator("append-prompt-history");
}

/* Update the dolphin object. */
void
Geometry::update_dolphin(int numPoints, EmbReal xScale, EmbReal yScale)
{
    for (int i = 0; i <= numPoints; i++) {
        EmbReal t = (2.0 * CONSTANT_PI) / numPoints*i;
        EmbVector v;
        v.x = fourier_series(t, dolphin_x);
        v.y = fourier_series(t, dolphin_y);

        setRubberPoint("POLYGON_POINT_" + i.toString(), xx*xScale, yy*yScale);
    }

    setRubberText("POLYGON_NUM_POINTS", numPoints.toString());
}

/* . */
void
Geometry::ellipse_click(EmbVector v)
{
    case MODE_ELLIPSE_MAJORDIAMETER_MINORRADIUS: {
        if (std::isnan(point1.x)) {
            point1 = v;
            addRubber("ELLIPSE");
            setRubberMode("ELLIPSE_LINE");
            setRubberPoint("ELLIPSE_LINE_POINT1", point1.x, point1.y);
            actuator("append-prompt-history");
            actuator("set-prompt-prefix-tr Specify first axis end point: ");
        }
        else if (std::isnan(point2.x)) {
            point2 = v;
            properties["center.x"] = node_real((point1.x.r + point2.x.r)/2.0);
            properties["center.y"] = node_real((point1.y.r + point2.y.r)/2.0);
            properties["width"] = calculateDistance(point1.x, point1.y, point2.x, point2.y);
            properties["rot"] = calculateAngle(point1.x, point1.y, point2.x, point2.y);
            setRubberMode("ELLIPSE_MAJORDIAMETER_MINORRADIUS");
            setRubberPoint("ELLIPSE_AXIS1_POINT1", point1.x, point1.y);
            setRubberPoint("ELLIPSE_AXIS1_POINT2", point2.x, point2.y);
            setRubberPoint("ELLIPSE_CENTER", properties["center.x"], properties["center.y"]);
            setRubberPoint("ELLIPSE_WIDTH", properties["width"], 0);
            setRubberPoint("ELLIPSE_ROT", properties["rot"], 0);
            actuator("append-prompt-history");
            actuator("set-prompt-prefix-tr Specify second axis end point or [Rotation]: ");
        }
        else if (std::isnan(point3.x)) {
            point3.x = x;
            point3.y = y;
            properties["height"] = perpendicularDistance(point3.x.r, point3.y, point1.x.r, point1.y.r, point2.x.r, point2.y.r)*2.0;
            setRubberPoint("ELLIPSE_AXIS2_POINT2", point3.x.r, point3.y.r);
            actuator("vulcanize");
            actuator("append-prompt-history");
            actuator("end");
        }
        else {
            error("ELLIPSE", tr("This should never happen."));
        }
    }
    case MODE_ELLIPSE_MAJORRADIUS_MINORRADIUS: {
        if (std::isnan(point1.x)) {
            point1.x = x;
            point1.y = y;
            properties["center.x"] = point1.x;
            properties["center.y"] = point1.y;
            addRubber("ELLIPSE");
            setRubberMode("ELLIPSE_LINE");
            setRubberPoint("ELLIPSE_LINE_POINT1", point1.x, point1.y);
            setRubberPoint("ELLIPSE_CENTER", properties["center.x"], properties["center.y"]);
            actuator("append-prompt-history");
            actuator("set-prompt-prefix-tr Specify first axis end point: ");
        }
        else if (std::isnan(point2.x)) {
            point2.x = x;
            point2.y = y;
            properties["width"] = calculateDistance(properties["center.x"], properties["center.y"], point2.x, point2.y)*2.0;
            properties["rot"] = calculateAngle(point1.x, point1.y, point2.x, point2.y);
            setRubberMode("ELLIPSE_MAJORRADIUS_MINORRADIUS");
            setRubberPoint("ELLIPSE_AXIS1_POINT2", point2.x, point2.y);
            setRubberPoint("ELLIPSE_WIDTH", properties["width"], 0);
            setRubberPoint("ELLIPSE_ROT", properties["rot"], 0);
            actuator("append-prompt-history");
            actuator("set-prompt-prefix-tr Specify second axis end point or [Rotation]: ");
        }
        else if (std::isnan(point3.x)) {
            point3.x = x;
            point3.y = y;
            properties["height"] = perpendicularDistance(point3.x, point3.y, properties["center.x"], properties["center.y"], point2.x, point2.y)*2.0;
            setRubberPoint("ELLIPSE_AXIS2_POINT2", point3.x, point3.y);
            actuator("vulcanize");
            actuator("append-prompt-history");
            actuator("end");
        }
        else {
            error("ELLIPSE", tr("This should never happen."));
        }
    }
    case MODE_ELLIPSE_ROTATION: {
        if (std::isnan(point1.x.r)) {
            error("ELLIPSE", tr("This should never happen."));
        }
        else if (std::isnan(point2.x)) {
            error("ELLIPSE", tr("This should never happen."));
        }
        else if (std::isnan(point3.x)) {
            EmbReal angle = calculateAngle(properties["center.x"], properties["center.y"], x, y);
            properties["height"] = cos(angle*PI/180.0)*properties["width"];
            addEllipse(properties["center.x"], properties["center.y"], properties["width"], properties["height"], properties["rot"], false);
            actuator("append-prompt-history");
            actuator("end");
        }
    }
}

/* . */
void
Geometry::ellipse_context(String args)
{
    debug_message("TODO ELLIPSE context()");
}

/* . */
void
Geometry::ellipse_prompt(String args)
{
    switch (mode) {
    case MODE_MAJORDIAMETER_MINORRADIUS: {
        if (std::isnan(point1.x.r)) {
            if (str == "C" || str == "CENTER") {
                mode = MODE_MAJORRADIUS_MINORRADIUS;
                actuator("set-prompt-prefix-tr Specify center point: "));
            }
            else {
                EmbReal strList = str.split(",");
                if (std::isnan(strList[0]) || std::isnan(strList[1])) {
                    alert(tr("Point or option keyword required."));
                    actuator("set-prompt-prefix-tr Specify first axis start point or [Center]: ");
                }
                else {
                    point1.x = node_real(strList[0]);
                    point1.y = node_real(strList[1]);
                    addRubber("ELLIPSE");
                    setRubberMode("ELLIPSE_LINE");
                    setRubberPoint("ELLIPSE_LINE_POINT1", point1.x, point1.y);
                    actuator("set-prompt-prefix-tr Specify first axis end point: ");
                }
            }
        }
        else if (std::isnan(point2.x)) {
            EmbReal strList = str.split(",");
            if (std::isnan(strList[0]) || std::isnan(strList[1])) {
                alert(tr("Invalid point."));
                actuator("set-prompt-prefix-tr Specify first axis end point: ");
            }
            else {
                point2.x = node_real(strList[0]);
                point2.y = node_real(strList[1]);
                properties["center.x"] = (point1.x + point2.x)/2.0;
                properties["center.y"] = (point1.y + point2.y)/2.0;
                properties["width"] = calculateDistance(point1.x, point1.y, point2.x, point2.y);
                properties["rot"] = calculateAngle(point1.x, point1.y, point2.x, point2.y);
                setRubberMode("ELLIPSE_MAJORDIAMETER_MINORRADIUS");
                setRubberPoint("ELLIPSE_AXIS1_POINT1", point1.x, point1.y);
                setRubberPoint("ELLIPSE_AXIS1_POINT2", point2.x, point2.y);
                setRubberPoint("ELLIPSE_CENTER", properties["center.x"], properties["center.y"]);
                setRubberPoint("ELLIPSE_WIDTH", properties["width"], 0);
                setRubberPoint("ELLIPSE_ROT", properties["rot"], 0);
                actuator("set-prompt-prefix-tr Specify second axis end point or [Rotation]: ");
            }
        }
        else if (std::isnan(point3.x)) {
            if (str == "R" || str == "ROTATION") {
                mode = MODE_ELLIPSE_ROTATION;
                actuator("set-prompt-prefix-tr Specify rotation: ");
            }
            else {
                EmbReal strList = str.split(",");
                if (std::isnan(strList[0]) || std::isnan(strList[1])) {
                    alert(tr("Point or option keyword required."));
                    actuator("set-prompt-prefix-tr Specify second axis end point or [Rotation]: ");
                }
                else {
                    point3.x = node_real(strList[0]);
                    point3.y = node_real(strList[1]);
                    properties["height"] = perpendicularDistance(point3.x, point3.y, point1.x, point1.y, point2.x, point2.y)*2.0;
                    setRubberPoint("ELLIPSE_AXIS2_POINT2", point3.x, point3.y);
                    actuator("vulcanize");
                    actuator("end");
                }
            }
        }
        break;
    }
    case MODE_MAJORRADIUS_MINORRADIUS: {
        if (std::isnan(point1.x.r)) {
            EmbReal strList = str.split(",");
            if (std::isnan(strList[0]) || std::isnan(strList[1])) {
                alert(tr("Invalid point."));
                actuator("set-prompt-prefix-tr Specify center point: ");
            }
            else {
                point1.x = node_real(strList[0]);
                point1.y = node_real(strList[1]);
                properties["center.x"] = point1.x;
                properties["center.y"] = point1.y;
                addRubber("ELLIPSE");
                setRubberMode("ELLIPSE_LINE");
                setRubberPoint("ELLIPSE_LINE_POINT1", point1.x, point1.y);
                setRubberPoint("ELLIPSE_CENTER", properties["center.x"], properties["center.y"]);
                actuator("set-prompt-prefix-tr Specify first axis end point: ");
            }
        }
        else if (std::isnan(point2.x)) {
            EmbReal strList = str.split(",");
            if (std::isnan(strList[0]) || std::isnan(strList[1])) {
                alert(tr("Invalid point."));
                actuator("set-prompt-prefix-tr Specify first axis end point: ");
            }
            else {
                point2.x = node_real(strList[0]);
                point2.y = node_real(strList[1]);
                properties["width"] = calculateDistance(point1.x, point1.y, point2.x, point2.y)*2.0;
                properties["rot"] = calculateAngle(point1.x, point1.y, point2.x, point2.y);
                setRubberMode("ELLIPSE_MAJORRADIUS_MINORRADIUS");
                setRubberPoint("ELLIPSE_AXIS1_POINT2", point2.x, point2.y);
                setRubberPoint("ELLIPSE_WIDTH", properties["width"], 0);
                setRubberPoint("ELLIPSE_ROT", properties["rot"], 0);
                actuator("set-prompt-prefix-tr Specify second axis end point or [Rotation]: ");
            }
        }
        else if (std::isnan(point3.x)) {
            if (str == "R" || str == "ROTATION") {
                mode = MODE_ELLIPSE_ROTATION;
                actuator("set-prompt-prefix-tr Specify ellipse rotation: ");
            }
            else {
                EmbReal strList = str.split(",");
                if (std::isnan(strList[0]) || std::isnan(strList[1])) {
                    alert(tr("Point or option keyword required."));
                    actuator("set-prompt-prefix-tr Specify second axis end point or [Rotation]: ");
                }
                else {
                    point3.x = node_real(strList[0]);
                    point3.y = node_real(strList[1]);
                    properties["height"] = perpendicularDistance(point3.x, point3.y, point1.x, point1.y, point2.x, point2.y)*2.0;
                    setRubberPoint("ELLIPSE_AXIS2_POINT2", point3.x, point3.y);
                    actuator("vulcanize");
                    actuator("end");
                }
            }
        }
    }
    case MODE_ELLIPSE_ROTATION: {
        if (std::isnan(point1.x.r)) {
            error("ELLIPSE", tr("This should never happen."));
        }
        else if (std::isnan(point2.x)) {
            error("ELLIPSE", tr("This should never happen."));
        }
        else if (std::isnan(point3.x)) {
            if (std::isnan(str)) {
                alert(tr("Invalid angle. Input a numeric angle or pick a point."));
                actuator("set-prompt-prefix-tr Specify rotation: ");
            }
            else {
                EmbReal angle = node_real(str);
                properties["height"] = cos(angle*PI/180.0)*properties["width"];
                addEllipse(properties["center.x"], properties["center.y"], properties["width"], properties["height"], properties["rot"], false);
                actuator("end");
            }
        }
        break;
    }
    default:
        break;
    }
}

/* . */
void
Geometry::erase_main(void)
{
    actuator("init");

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

/* . */
void
Geometry::heart_main(void)
{
    properties["numPoints"] = node_int(512); //Default //TODO: min:64 max:8192
    properties["center.x"] = node_real(0.0f);
    properties["center.y"] = node_real(0.0f);
    properties["sx"] = node(1.0f);
    properties["sy"] = node(1.0f);
    actuator("init");
    actuator("clear-selection");
    mode = node_str("MODE_NUM_POINTS");

    //Heart4: 10.0 / 512
    //Heart5: 1.0 / 512

    addRubber("POLYGON");
    setRubberMode("POLYGON");
    updateHeart("HEART5", properties["numPoints"], properties["sx"], properties["sy"]);
    spareRubber("POLYGON");
    actuator("end");
}

/* . */
void
Geometry::updateHeart(String style, int numPoints, EmbReal xScale, EmbReal yScale)
{
    for (int i = 0; i <= numPoints; i++) {
        EmbReal xx, yy;
        EmbReal t = (2.0*CONSTANT_PI)/numPoints*i;

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

/* . */
void
Geometry::line_main(void)
{
    actuator("init");
    actuator("clear-selection");
    firstRun = true;
    properties["first"] = {node_real(0.0f), node_real(0.0f)};
    properties["prev"] = {node_real(0.0f), node_real(0.0f)};
    actuator("set-prompt-prefix-tr Specify first point: ");
}

/* . */
void
Geometry::line_click(EmbReal x, EmbReal y)
{
    if (firstRun) {
        firstRun = false;
        properties["first.x"] = x;
        properties["first.y"] = y;
        properties["prev.x"] = x;
        properties["prev.y"] = y;
        addRubber("LINE");
        setRubberMode("LINE");
        setRubberPoint("LINE_START", properties["firstX"], properties["firstY"]);
        actuator("append-prompt-history");
        actuator("set-prompt-prefix-tr Specify next point or [Undo]: ");
    }
    else {
        setRubberPoint("LINE_END", x, y);
        actuator("vulcanize");
        addRubber("LINE");
        setRubberMode("LINE");
        setRubberPoint("LINE_START", x, y);
        actuator("append-prompt-history");
        properties["prev.x"] = x;
        properties["prev.y"] = y;
    }
}

/**
 * .
 */
void
Geometry::line_context(String str)
{
    actuator("todo LINE context()");
}

/**
 * .
 */
void
Geometry::line_prompt(String args)
{
    if (firstRun) {
        EmbReal strList = str.split(",");
        if (std::isnan(strList[0]) || std::isnan(strList[1])) {
            alert(tr("Invalid point."));
            actuator("set-prompt-prefix-tr Specify first point: "));
        }
        else {
            firstRun = false;
            properties["first.x"] = node_real(strList[0]);
            properties["firstY"] = node_real(strList[1]);
            properties["prev.x"] = properties["first.x"];
            properties["prev.y"] = properties["first.y"];
            addRubber("LINE");
            setRubberMode("LINE");
            setRubberPoint("LINE_START", properties["firstX"], properties["firstY"]);
            actuator("set-prompt-prefix-tr Specify next point or [Undo]: "));
        }
    }
    else {
        if (str == "U" || str == "UNDO") {
            actuator("todo LINE prompt() for UNDO");
        }
        else {
            EmbReal strList = str.split(",");
            if (std::isnan(strList[0]) || std::isnan(strList[1])) {
                alert(tr("Point or option keyword required."));
                actuator("set-prompt-prefix-tr Specify next point or [Undo]: ");
            }
            else {
                EmbReal x = node_real(strList[0]);
                EmbReal y = node_real(strList[1]);
                setRubberPoint("LINE_END", x, y);
                actuator("vulcanize");
                addRubber("LINE");
                setRubberMode("LINE");
                setRubberPoint("LINE_START", x, y);
                properties["prev.x"] = x;
                properties["prevY"] = y;
                actuator("set-prompt-prefix-tr Specify next point or [Undo]: ");
            }
        }
    }
}

/* locate_point_main */
void
Geometry::locate_point_main(void)
{
    script(
        "init\n" \
        "clear-selection\n" \
        "set-prompt-prefix-tr Specify point: \n"
    );
}

/* locate_point_click v */
void
Geometry::locate_point_click(EmbVector v)
{
    actuator("append-prompt-history");
    actuator("set-prompt-prefix v.x = " + v.x + ", Y = " + v.y);
    actuator("append-prompt-history");
    actuator("end");
}

/* . */
void
Geometry::locate_point_context(String str)
{
    actuator("todo LOCATEPOINT context()");
}

/* . */
void
Geometry::locate_point_prompt(String args)
{
    std::vector<std::string> strList = tokenize(args, ',');
    if (std::isnan(strList[0]) || std::isnan(strList[1])) {
        alert(tr("Invalid point."));
        actuator("set-prompt-prefix-tr Specify point: ");
    }
    else {
        actuator("append-prompt");
        actuator("set-prompt-prefix .x = " + strList[0] + ", Y = " + strList[1]);
        actuator("append-prompt");
        actuator("end");
    }
}

/* . */
void
Geometry::move_main(void)
{
    actuator("init");
    firstRun.b = node_int(true);
    properties["base.x"] = node_real(0.0f);
    properties["base.y"] = node_real(0.0f);
    properties["dest.x"] = node_real(0.0f);
    properties["dest.y"] = node_real(0.0f);
    properties["delta.x"] = node_real(0.0f);
    properties["delta.y"] = node_real(0.0f);

    if (numSelected() <= 0) {
        //TODO: Prompt to select objects if nothing is preselected
        alert(tr("Preselect objects before invoking the move command."));
        actuator("end");
        messageBox("information", tr("Move Preselect"), tr("Preselect objects before invoking the move command."));
    }
    else {
        actuator("set-prompt-prefix-tr Specify base point: "));
    }
}

/* . */
void
Geometry::move_click(EmbReal x, EmbReal y)
{
    if (firstRun) {
        firstRun = false;
        properties["base.x"] = x;
        properties["base.y"] = y;
        addRubber("LINE");
        setRubberMode("LINE");
        setRubberPoint("LINE_START", properties["baseX"], properties["baseY"]);
        previewOn("SELECTED", "MOVE", properties["baseX"], properties["baseY"], 0);
        actuator("append-prompt-history");
        actuator("set-prompt-prefix-tr Specify destination point: ");
    }
    else {
        properties["dest.x"] = x;
        properties["dest.y"] = y;
        properties["delta.x"] = properties["destX"] - properties["baseX"];
        properties["delta.y"] = properties["destY"] - properties["baseY"];
        moveSelected(properties["deltaX"], properties["deltaY"]);
        previewOff();
        actuator("end");
    }
}

/* . */
void
Geometry::move_context(String str)
{
    actuator("todo MOVE context()");
}

/* . */
void
Geometry::move_prompt(String str)
{
    if (firstRun.b) {
        EmbReal strList = str.split(",");
        if (std::isnan(strList[0]) || std::isnan(strList[1])) {
            alert(tr("Invalid point."));
            actuator("set-prompt-prefix-tr Specify base point: ");
        }
        else {
            firstRun = node_int(false);
            properties["base.x"] = node_real(strList[0]);
            properties["base.y"] = node_real(strList[1]);
            addRubber("LINE");
            setRubberMode("LINE");
            setRubberPoint("LINE_START", properties["baseX"], properties["baseY"]);
            previewOn("SELECTED", "MOVE", properties["baseX"], properties["baseY"], 0);
            actuator("set-prompt-prefix-tr Specify destination point: ");
        }
    }
    else {
        EmbReal strList = str.split(",");
        if (std::isnan(strList[0]) || std::isnan(strList[1])) {
            alert(tr("Invalid point."));
            actuator("set-prompt-prefix-tr Specify destination point: ");
        }
        else {
            properties["dest.x"] = node_real(strList[0]);
            properties["dest.y"] = node_real(strList[1]);
            properties["delta.x"] = properties["dest.x"] - properties["base.x"];
            properties["delta.y"] = properties["dest.y"] - properties["base.y"];
            moveSelected(properties["deltaX, properties["deltaY);
            previewOff();
            actuator("end");
        }
    }
}

/* TODO: The path command is currently broken. */
void
Geometry::path_main(void)
{
    firstRun = node_int(true);
    properties["first"] = {0.0, 0.0};
    properties["prev"] = {0.0, 0.0};
    actuator("init");
    actuator("clear-selection");
    firstRun = node_real(true);
    actuator("set-prompt-prefix-tr Specify start point: ");
}

/* . */
void
Geometry::path_click(EmbReal x, EmbReal y)
{
    if (firstRun.b) {
        firstRun = false;
        properties["first.x"] = x;
        properties["first.y"] = y;
        properties["prev.x"] = x;
        properties["prev.y"] = y;
        addPath(x, y);
        actuator("append-prompt-history");
        actuator("set-prompt-prefix-tr Specify next point or [Arc/Undo]: ");
    }
    else {
        actuator("append-prompt-history");
        appendLineToPath(x, y);
        properties["prev.x"] = x;
        properties["prev.y"] = y;
    }
}

/* . */
void
Geometry::path_context(String str)
{
    actuator("todo PATH context()");
}

/* . */
void
Geometry::path_prompt(String args)
{
    if (str == "A" || str == "ARC") {
        actuator("todo PATH prompt() for ARC");
    }
    else if (str == "U" || str == "UNDO") {
        actuator("todo PATH prompt() for UNDO");
    }
    else {
        EmbReal strList = str.split(",");
        if (std::isnan(strList[0]) || std::isnan(strList[1])) {
            alert(tr("Point or option keyword required."));
            actuator("set-prompt-prefix-tr Specify next point or [Arc/Undo]: ");
        }
        else {
            EmbReal x = node_real(strList[0]);
            EmbReal y = node_real(strList[1]);
            if (firstRun) {
                firstRun = false;
                first = embVector(x, y);
                prev = embVector(x, y);
                addPath(x, y);
                actuator("set-prompt-prefix-tr Specify next point or [Arc/Undo]: ");
            }
            else {
                appendLineToPath(x, y);
                prev = embVector(x, y);
            }
        }
    }
}

/* . */
void
Geometry::point_main(void)
{
    actuator("init");
    actuator("clear-selection");
    firstRun = true;
    setPromptPrefix("TODO: Current point settings: PDMODE=?  PDSIZE=?"); //TODO: tr needed here when complete
    actuator("append-prompt-history");
    actuator("set-prompt-prefix-tr Specify first point: ");
}

/* . */
void
Geometry::point_click(EmbVector v)
{
    if (firstRun) {
        firstRun = false;
        actuator("append-prompt-history");
        actuator("set-prompt-prefix-tr Specify next point: "));
        addPoint(x, y);
    }
    else {
        actuator("append-prompt-history");
        addPoint(x, y);
    }
}

/* . */
void
Geometry::point_context(String str)
{
    actuator("todo POINT context()");
}

/* . */
void
Geometry::point_prompt(String str)
{
    if (firstRun) {
        if (str == "M" || str == "MODE") {
            actuator("todo POINT prompt() for PDMODE");
        }
        else if (str == "S" || str == "SIZE") {
            actuator("todo POINT prompt() for PDSIZE");
        }
        EmbReal strList = str.split(",");
        if (std::isnan(strList[0]) || std::isnan(strList[1])) {
            alert(tr("Invalid point."));
            actuator("set-prompt-prefix-tr Specify first point: "));
        }
        else {
            firstRun = false;
            EmbReal x = node_real(strList[0]);
            EmbReal y = node_real(strList[1]);
            actuator("set-prompt-prefix-tr Specify next point: "));
            addPoint(x,y);
        }
    }
    else {
        EmbReal strList = str.split(",");
        if (std::isnan(strList[0]) || std::isnan(strList[1])) {
            alert(tr("Invalid point."));
            actuator("set-prompt-prefix-tr Specify next point: "));
        }
        else {
            EmbReal x = node_real(strList[0]);
            EmbReal y = node_real(strList[1]);
            actuator("set-prompt-prefix-tr Specify next point: "));
            addPoint(x,y);
        }
    }
}

/* . */
void
Geometry::polygon_main(void)
{
    actuator("init");
    actuator("clear-selection");
    center = embVector(0.0f, 0.0f);
    side1 = embVector(0.0f, 0.0f);
    side2 = embVector(0.0f, 0.0f);
    pointI = embVector(0.0f, 0.0f);
    pointC = embVector(0.0f, 0.0f);
    polyType = "Inscribed"; //Default
    numSides = 4;           //Default
    mode = MODE_NUM_SIDES;
    actuator("set-prompt-prefix-tr Enter number of sides" + " {" + properties["numSides"].toString() + "}: ");
}

/* . */
void
Geometry::polygon_click(EmbVector v)
{
    switch (mode) {
    case MODE_POLYGON_NUM_SIDES:
    case MODE_POLYGON_POLYTYPE:
    case MODE_POLYGON_DISTANCE:
        //Do nothing, the prompt controls this.
        break;
    case MODE_POLYGON_CENTER_PT: {
        center = v;
        mode = POLYGON_MODE_POLYTYPE;
        actuator("append-prompt-history");
        actuator("set-prompt-prefix-tr Specify polygon type [Inscribed in circle/Circumscribed around circle]") + " {" + properties["polyType"] + "}: ");
        break;
    }
    case MODE_POLYGON_INSCRIBE: {
        pointI = v;
        setRubberPoint("POLYGON_INSCRIBE_POINT", properties["pointIX"], properties["pointIY"]);
        actuator("vulcanize");
        actuator("append-prompt-history");
        actuator("end");
        break;
    }
    case MODE_POLYGON_CIRCUMSCRIBE: {
        pointC = v;
        setRubberPoint("POLYGON_CIRCUMSCRIBE_POINT", properties["pointCX"], properties["pointCY"]);
        actuator("vulcanize");
        actuator("append-prompt-history");
        actuator("end");
        break;
    }
    case MODE_POLYGON_SIDE_LEN: {
        actuator("todo POLYGON Sidelength mode");
        break;
    }
}

/* Polygon */
void
Geometry::polygon_context(String str)
{
    actuator("todo POLYGON context()");
}

/* Polygon */
void
Geometry::polygon_prompt(String str)
{
    case MODE_POLYGON_NUM_SIDES: {
        if (str == "" && properties["numSides"] >= 3 && properties["numSides"] <= 1024) {
            actuator("set-prompt-prefix-tr Specify center point or [Sidelength]: ");
            mode = MODE_CENTER_PT;
        }
        else {
            EmbReal tmp = node_real(str);
            if (std::isnan(tmp) || !isInt(tmp) || tmp < 3 || tmp > 1024) {
                alert(tr("Requires an integer between 3 and 1024."));
                actuator("set-prompt-prefix-tr Enter number of sides" + " {" + properties["numSides"].toString() + "}: ");
            }
            else {
                properties["numSides"] = tmp;
                actuator("set-prompt-prefix-tr Specify center point or [Sidelength]: "));
                mode = MODE_CENTER_PT;
            }
        }
        break;
    }
    case MODE_POLYGON_CENTER_PT: {
        if (str == "S" || str == "SIDELENGTH") {
            mode = MODE_SIDE_LEN;
            actuator("set-prompt-prefix-tr Specify start point: "));
        }
        else {
            EmbReal strList = str.split(",");
            if (std::isnan(strList[0]) || std::isnan(strList[1])) {
                alert(tr("Point or option keyword required."));
                actuator("set-prompt-prefix-tr Specify center point or [Sidelength]: "));
            }
            else {
                properties["center.x"] = node_real(strList[0]);
                properties["center.y"] = node_real(strList[1]);
                mode = MODE_POLYTYPE;
                actuator("set-prompt-prefix-tr Specify polygon type [Inscribed in circle/Circumscribed around circle]") + " {" + properties["polyType + "}: ");
            }
        }
    }
    case MODE_POLYTYPE) {
        if (str == "INSCRIBED") {
            mode = MODE_INSCRIBE;
            properties["polyType"] = "Inscribed";
            actuator("set-prompt-prefix-tr Specify polygon corner point or [Distance]: ");
            addRubber("POLYGON");
            setRubberMode("POLYGON_INSCRIBE");
            setRubberPoint("POLYGON_CENTER", properties["centerX"], properties["centerY"]);
            setRubberPoint("POLYGON_NUM_SIDES", properties["numSides"], 0);
        }
        else if (str == "CIRCUMSCRIBED") {
            mode = MODE_CIRCUMSCRIBE;
            properties["polyType"] = "Circumscribed";
            actuator("set-prompt-prefix-tr Specify polygon side point or [Distance]: ");
            addRubber("POLYGON");
            setRubberMode("POLYGON_CIRCUMSCRIBE");
            setRubberPoint("POLYGON_CENTER", properties["centerX"], properties["centerY"]);
            setRubberPoint("POLYGON_NUM_SIDES", properties["numSides"], 0);
        }
        else if (str == "") {
            if (properties["polyType"] == "Inscribed") {
                mode = MODE_INSCRIBE;
                actuator("set-prompt-prefix-tr Specify polygon corner point or [Distance]: ");
                addRubber("POLYGON");
                setRubberMode("POLYGON_INSCRIBE");
                setRubberPoint("POLYGON_CENTER", properties["centerX"], properties["centerY"]);
                setRubberPoint("POLYGON_NUM_SIDES", properties["numSides"], 0);
            }
            else if (properties["polyType"] == "Circumscribed") {
                mode = MODE_CIRCUMSCRIBE;
                actuator("set-prompt-prefix-tr Specify polygon side point or [Distance]: ");
                addRubber("POLYGON");
                setRubberMode("POLYGON_CIRCUMSCRIBE");
                setRubberPoint("POLYGON_CENTER", properties["centerX"], properties["centerY"]);
                setRubberPoint("POLYGON_NUM_SIDES", properties["numSides"], 0);
            }
            else {
                error("POLYGON", tr("Polygon type is not Inscribed or Circumscribed."));
            }
        }
        else {
            alert(tr("Invalid option keyword."));
            actuator("set-prompt-prefix-tr Specify polygon type [Inscribed in circle/Circumscribed around circle]") + " {" + properties["polyType"] + "}: ");
        }
    }
    case MODE_INSCRIBE) {
        if (str == "D" || str == "DISTANCE") {
            mode = MODE_DISTANCE;
            actuator("set-prompt-prefix-tr Specify distance: "));
        }
        else {
            EmbReal strList = str.split(",");
            if (std::isnan(strList[0]) || std::isnan(strList[1])) {
                alert(tr("Point or option keyword required."));
                actuator("set-prompt-prefix-tr Specify polygon corner point or [Distance]: "));
            }
            else {
                properties["pointI.x"] = node_real(strList[0]);
                properties["pointI.y"] = node_real(strList[1]);
                setRubberPoint("POLYGON_INSCRIBE_POINT", properties["pointIX"], properties["pointIY"]);
                actuator("vulcanize");
                actuator("end");
            }
        }
    }
    case MODE_CIRCUMSCRIBE) {
        if (str == "D" || str == "DISTANCE") {
            mode = MODE_DISTANCE;
            actuator("set-prompt-prefix-tr Specify distance: "));
        }
        else {
            EmbReal strList = str.split(",");
            if (std::isnan(strList[0]) || std::isnan(strList[1])) {
                alert(tr("Point or option keyword required."));
                actuator("set-prompt-prefix-tr Specify polygon side point or [Distance]: "));
            }
            else {
                properties["pointC.x"] = node_real(strList[0]);
                properties["pointC.y"] = node_real(strList[1]);
                setRubberPoint("POLYGON_CIRCUMSCRIBE_POINT", properties["pointCX"], properties["pointCY"]);
                actuator("vulcanize");
                actuator("end");
            }
        }
    }
    case MODE_DISTANCE) {
        if (std::isnan(str)) {
            alert(tr("Requires valid numeric distance."));
            actuator("set-prompt-prefix-tr Specify distance: "));
        }
        else {
            if (properties["polyType"] == "Inscribed") {
                properties["pointI.x"] = properties["centerX"];
                properties["pointI.y"] = properties["centerY"] + node_real(str);
                setRubberPoint("POLYGON_INSCRIBE_POINT", properties["pointIX"], properties["pointIY"]);
                actuator("vulcanize");
                actuator("end");
            }
            else if (properties["polyType"] == "Circumscribed") {
                properties["pointC.x"] = properties["centerX"];
                properties["pointC.y"] = properties["centerY"] + node_real(str);
                setRubberPoint("POLYGON_CIRCUMSCRIBE_POINT", properties["pointCX"], properties["pointCY"]);
                actuator("vulcanize");
                actuator("end");
            }
            else {
                error("POLYGON", tr("Polygon type is not Inscribed or Circumscribed."));
            }
        }
    }
    case MODE_SIDE_LEN) {
        actuator("todo POLYGON Sidelength mode");
    }
}

/* . */
void
Geometry::polyline_main(void)
{
    script(polyline_init);
}

/* . */
void
Geometry::polyline_click(EmbVector v)
{
    if (firstRun.b) {
        firstRun = node_int(false);
        properties["first.x"] = node_real(v.x);
        properties["first.y"] = node_real(v.y);
        properties["prev.x"] = node_real(v.x);
        properties["prev.y"] = node_real(v.y);
        addRubber("POLYLINE");
        setRubberMode("POLYLINE");
        setRubberPoint("POLYLINE_POINT_0", properties["firstX"].r, properties["firstY"].r);
        actuator("append-prompt-history");
        actuator("set-prompt-prefix-tr Specify next point or [Undo]: ");
    }
    else {
        properties["num"] = properties["num"] + 1;
        setRubberPoint("POLYLINE_POINT_" + properties["num"].toString(), x, y);
        setRubberText("POLYLINE_NUM_POINTS", properties["num"].toString());
        spareRubber("POLYLINE");
        actuator("append-prompt-history");
        properties["prev.x"] = x;
        properties["prev.y"] = y;
    }
}

/**
 * .
 */
void
Geometry::polyline_context(String str)
{
    actuator("todo POLYLINE context()");
}

/**
 * .
 */
void
Geometry::polyline_prompt(String str)
{
    if (firstRun.b) {
        EmbReal strList = str.split(",");
        if (std::isnan(strList[0]) || std::isnan(strList[1])) {
            alert(tr("Invalid point."));
            actuator("set-prompt-prefix-tr Specify first point: "));
        }
        else {
            firstRun = false;
            properties["first.x"] = node_real(strList[0]);
            properties["first.y"] = node_real(strList[1]);
            properties["prev.x"] = properties["firstX"];
            properties["prev.y"] = properties["firstY"];
            addRubber("POLYLINE");
            setRubberMode("POLYLINE");
            setRubberPoint("POLYLINE_POINT_0", properties["firstX"], properties["firstY"]);
            actuator("set-prompt-prefix-tr Specify next point or [Undo]: ");
        }
    }
    else {
        if (str == "U" || str == "UNDO") {
            actuator("todo POLYLINE prompt() for UNDO");
        }
        else {
            EmbReal strList = str.split(",");
            if (std::isnan(strList[0]) || std::isnan(strList[1])) {
                alert(tr("Point or option keyword required."));
                actuator("set-prompt-prefix-tr Specify next point or [Undo]: ");
            }
            else {
                EmbReal x = node_real(strList[0]);
                EmbReal y = node_real(strList[1]);
                properties["num"] = properties["num"].i + 1;
                setRubberPoint("POLYLINE_POINT_" + properties["num"].toString(), x, y);
                setRubberText("POLYLINE_NUM_POINTS", properties["num"].toString());
                spareRubber("POLYLINE");
                properties["prev.x"] = x;
                properties["prev.y"] = y;
                actuator("set-prompt-prefix-tr Specify next point or [Undo]: ");
            }
        }
    }
}

const char quickleader_main[][MAX_STRING_LENGTH] = {
    "todo Adding the text is not complete yet.",
    "init",
    "clear-selection",
    "x[0] = 0.0f",
    "y[0] = 0.0f",
    "x[1] = 0.0f",
    "y[1] = 0.0f",
    "set-prompt-prefix-tr Specify first point: "
};

/* . */
void
Geometry::quickleader_click(EmbVector v)
{
    if (std::isnan(point1.x.r)) {
        point1.x = node_real(v.x);
        point1.y = node_real(v.y);
        addRubber("DIMLEADER");
        setRubberMode("DIMLEADER_LINE");
        setRubberPoint("DIMLEADER_LINE_START", point1.x.r, point1.y.r);
        actuator("append-prompt-history");
        actuator("set-prompt-prefix-tr Specify second point: "));
    }
    else {
        point2.x = node_real(v.x);
        point2.y = node_real(v.y);
        setRubberPoint("DIMLEADER_LINE_END", point2.x.r, point2.y.r);
        actuator("vulcanize");
        actuator("append-prompt-history");
        actuator("end");
    }
}

/* . */
void
Geometry::quickleader_context(String str)
{
    actuator("todo QUICKLEADER context()");
}

/* . */
void
Geometry::quickleader_prompt(String str)
{
    EmbReal strList = str.split(",");
    if (std::isnan(point1.x.r)) {
        if (std::isnan(strList[0]) || std::isnan(strList[1])) {
            alert(tr("Requires two points."));
            actuator("set-prompt-prefix-tr Specify first point: "));
        }
        else {
            point1.x = node_real(strList[0]);
            point1.y = node_real(strList[1]);
            addRubber("DIMLEADER");
            setRubberMode("DIMLEADER_LINE");
            setRubberPoint("DIMLEADER_LINE_START", point1.x, point1.y);
            actuator("set-prompt-prefix-tr Specify second point: ");
        }
    }
    else {
        if (std::isnan(strList[0]) || std::isnan(strList[1])) {
            alert(tr("Requires two points."));
            actuator("set-prompt-prefix-tr Specify second point: ");
        }
        else {
            point2.x = node_real(strList[0]);
            point2.y = node_real(strList[1]);
            setRubberPoint("DIMLEADER_LINE_END", point2.x, point2.y);
            actuator("vulcanize");
            actuator("end");
        }
    }
}

/* . */
const char rectangle_click_script[][MAX_STRING_LENGTH] = {
    if (properties["newRect"].b) {
        properties["newRect"].b = false;
        point1.x = node_real(v.x);
        point1.y = node_real(v.y);
        addRubber("RECTANGLE");
        setRubberMode("RECTANGLE");
        setRubberPoint("RECTANGLE_START", x, y);
        actuator("set-prompt-prefix-tr Specify other corner point or [Dimensions]: "));
    }
    else {
        "newRect = true",
        "x2 = node_real(v.x)",
        "y2 = node_real(v.y)",
        "setRubberPoint RECTANGLE_END x y",
        "vulcanize",
        "end"
    }
};

/* . */
void
Geometry::rectangle_context(String str)
{
    actuator("todo RECTANGLE context()");
}

/* . */
void
Geometry::rectangle_prompt(String str)
{
    if (str == "C" || str == "CHAMFER") {
        actuator("todo RECTANGLE prompt() for CHAMFER");
    }
    else if (str == "D" || str == "DIMENSIONS") {
        actuator("todo RECTANGLE prompt() for DIMENSIONS");
    }
    else if (str == "F" || str == "FILLET") {
        actuator("todo RECTANGLE prompt() for FILLET");
    }
    else {
        EmbReal strList = str.split(",");
        if (std::isnan(strList[0]) || std::isnan(strList[1])) {
            alert(tr("Invalid point."));
            actuator("set-prompt-prefix-tr Specify first point: "));
        }
        else {
            EmbReal x = node_real(strList[0]);
            EmbReal y = node_real(strList[1]);
            if (properties["newRect"].b) {
                properties["newRect"] = false;
                point1.x = x;
                point1.y = y;
                addRubber("RECTANGLE");
                setRubberMode("RECTANGLE");
                setRubberPoint("RECTANGLE_START", x, y);
                actuator("set-prompt-prefix-tr Specify other corner point or [Dimensions]: "));
            }
            else {
                properties["newRect"] = true;
                point2.x = x;
                point2.y = y;
                setRubberPoint("RECTANGLE_END", x, y);
                actuator("vulcanize");
                actuator("end");
            }
        }
    }
}

/* . */
void
Geometry::rgb_main(void)
{
    actuator("init");
    actuator("clear-selection");
    actuator("mode=RGB_MODE_BACKGROUND");
    actuator("set-prompt-prefix-tr Enter RED,GREEN,BLUE values for background or [Crosshair/Grid]: "));
}

/* . */
void
Geometry::rgb_click(EmbVector v)
{
    //Do Nothing, prompt only command.
}

/* . */
void
Geometry::rgb_context(String str)
{
    actuator("todo RGB context()");
}

/**
 * .
 */
void
Geometry::rgb_prompt(String str)
{
    if (mode == "RGB_MODE_BACKGROUND") {
        if (str == "C" || str == "CROSSHAIR") {
            mode = node_real("RGB_MODE_CROSSHAIR");
            actuator("set-prompt-prefix-tr Specify crosshair color: "));
        }
        else if (str == "G" || str == "GRID") {
            mode = RGB_MODE_GRID;
            actuator("set-prompt-prefix-tr Specify grid color: "));
        }
        else {
            EmbReal strList = str.split(",");
            EmbReal r = node_real(strList[0]);
            EmbReal g = node_real(strList[1]);
            EmbReal b = node_real(strList[2]);
            if (!validRGB(r,g,b)) {
                alert(tr("Invalid color. R,G,B values must be in the range of 0-255."));
                actuator("set-prompt-prefix-tr Specify background color: "));
            }
            else {
                setBackgroundColor(r,g,b);
                actuator("end");
            }
        }
    }
    case RGB_MODE_CROSSHAIR) {
        EmbReal strList = str.split(",");
        EmbReal r = node_real(strList[0]);
        EmbReal g = node_real(strList[1]);
        EmbReal b = node_real(strList[2]);
        if (!validRGB(r,g,b)) {
            alert(tr("Invalid color. R,G,B values must be in the range of 0-255."));
            actuator("set-prompt-prefix-tr Specify crosshair color: "));
        }
        else {
            setCrossHairColor(r,g,b);
            actuator("end");
        }
    }
    case RGB_MODE_GRID) {
        EmbReal strList = str.split(",");
        EmbReal r = node_real(strList[0]);
        EmbReal g = node_real(strList[1]);
        EmbReal b = node_real(strList[2]);
        if (!validRGB(r,g,b)) {
            alert(tr("Invalid color. R,G,B values must be in the range of 0-255."));
            actuator("set-prompt-prefix-tr Specify grid color: "));
        }
        else {
            setGridColor(r,g,b);
            actuator("end");
        }
    }
}

/* . */
void
Geometry::rotate_main(String args)
{
    actuator("init");
    mode = ROTATE_MODE_NORMAL;
    firstRun = true;
    properties["base"] = {node_real(0.0f), node_real(0.0f)};
    properties["dest"] = {node_real(0.0f), node_real(0.0f)};
    properties["angle"] = node_real(0.0f);

    properties["baseR"] = {node_real(0.0f), node_real(0.0f)};
    properties["destR"] = {node_real(0.0f), node_real(0.0f)};
    properties["angleRef"] = node_real(0.0f);
    properties["angleNew"] = node_real(0.0f);

    if (numSelected() <= 0) {
        //TODO: Prompt to select objects if nothing is preselected
        alert(tr("Preselect objects before invoking the rotate command."));
        actuator("end");
        messageBox("information", tr("Rotate Preselect"), tr("Preselect objects before invoking the rotate command."));
    }
    else {
        actuator("set-prompt-prefix-tr Specify base point: "));
    }
}

/* . */
void
Geometry::rotate_click(EmbVector v)
{
    switch (mode) {
    case ROTATE_MODE_NORMAL:
        if (firstRun) {
            firstRun = false;
            properties["base"] = v;
            addRubber("LINE");
            setRubberMode("LINE");
            setRubberPoint("LINE_START", properties["baseX, properties["baseY);
            previewOn("SELECTED", "ROTATE", properties["baseX, properties["baseY, 0);
            actuator("append-prompt-history");
            actuator("set-prompt-prefix-tr Specify rotation angle or [Reference]: "));
        }
        else {
            properties["dest"] = v;
            properties["angle"] = calculateAngle(properties["baseX"], properties["baseY"], properties["destX"], properties["destY"]);
            actuator("append-prompt-history");
            rotateSelected(properties["baseX"], properties["baseY"], properties["angle"]);
            previewOff();
            actuator("end");
        }
    }
    case ROTATE_MODE_REFERENCE) {
        if (std::isnan(properties["baseRX"])) {
            properties["baseR"] = v;
            actuator("append-prompt-history");
            addRubber("LINE");
            setRubberMode("LINE");
            setRubberPoint("LINE_START", properties["baseRX"], properties["baseRY"]);
            actuator("set-prompt-prefix-tr Specify second point: ");
        }
        else if (std::isnan(properties["destRX"])) {
            properties["destR"] = v;
            properties["angleRef"] = calculateAngle(properties["baseRX"], properties["baseRY"], properties["destRX"], properties["destRY"]);
            setRubberPoint("LINE_START", properties["baseX"], properties["baseY"]);
            previewOn("SELECTED", "ROTATE", properties["baseX"], properties["baseY"], properties["angleRef"]);
            actuator("append-prompt-history");
            actuator("set-prompt-prefix-tr Specify the new angle: ");
        }
        else if (std::isnan(properties["angleNew"])) {
            properties["angleNew"] = calculateAngle(properties["baseX"], properties["baseY"], x, y);
            rotateSelected(properties["baseX"], properties["baseY"], properties["angleNew"] - properties["angleRef"]);
            previewOff();
            actuator("end");
        }
    }
}

/* . */
void
Geometry::rotate_context(String str)
{
    actuator("todo ROTATE context()");
}

/* . */
void
Geometry::rotate_prompt(String str)
{
    if (mode == ROTATE_MODE_NORMAL) {
        if (firstRun) {
            EmbReal strList = str.split(",");
            if (std::isnan(strList[0]) || std::isnan(strList[1])) {
                alert(tr("Invalid point."));
                actuator("set-prompt-prefix-tr Specify base point: ");
            }
            else {
                firstRun = false;
                properties["base.x"] = node_real(strList[0]);
                properties["base.y"] = node_real(strList[1]);
                addRubber("LINE");
                setRubberMode("LINE");
                setRubberPoint("LINE_START", properties["baseX"], properties["baseY"]);
                previewOn("SELECTED", "ROTATE", properties["baseX"], properties["baseY"], 0);
                actuator("set-prompt-prefix-tr Specify rotation angle or [Reference]: ");
            }
        }
        else {
            if (str == "R" || str == "REFERENCE") {
                mode = MODE_REFERENCE;
                actuator("set-prompt-prefix-tr Specify the reference angle") + " {0.00}: ");
                clearRubber();
                previewOff();
            }
            else {
                if (std::isnan(str)) {
                    alert(tr("Requires valid numeric angle, second point, or option keyword."));
                    actuator("set-prompt-prefix-tr Specify rotation angle or [Reference]: ");
                }
                else {
                    properties["angle"] = node_real(str);
                    rotateSelected(properties["baseX"], properties["baseY"], properties["angle"]);
                    previewOff();
                    actuator("end");
                }
            }
        }
    }
    case MODE_REFERENCE) {
        if (std::isnan(properties["baseRX"])) {
            if (std::isnan(str)) {
                EmbReal strList = str.split(",");
                if (std::isnan(strList[0]) || std::isnan(strList[1])) {
                    alert(tr("Requires valid numeric angle or two points."));
                    actuator("set-prompt-prefix-tr Specify the reference angle {0.00}: ");
                }
                else {
                    properties["baseR.x"] = node_real(strList[0]);
                    properties["baseR.y"] = node_real(strList[1]);
                    addRubber("LINE");
                    setRubberMode("LINE");
                    setRubberPoint("LINE_START", properties["baseRX"], properties["baseRY"]);
                    actuator("set-prompt-prefix-tr Specify second point: ");
                }
            }
            else {
                //The base and dest values are only set here to advance the command.
                properties["baseR.x"] = 0.0;
                properties["baseR.y"] = 0.0;
                properties["destR.x"] = 0.0;
                properties["destR.y"] = 0.0;
                //The reference angle is what we will use later.
                properties["angleRef"] = node_real(str);
                addRubber("LINE");
                setRubberMode("LINE");
                setRubberPoint("LINE_START", properties["baseX"], properties["baseY"]);
                previewOn("SELECTED", "ROTATE", properties["baseX"], properties["baseY"], properties["angleRef"]);
                actuator("set-prompt-prefix-tr Specify the new angle: ");
            }
        }
        else if (std::isnan(properties["destRX"])) {
            if (std::isnan(str)) {
                EmbReal strList = str.split(",");
                if (std::isnan(strList[0]) || std::isnan(strList[1])) {
                    alert(tr("Requires valid numeric angle or two points."));
                    actuator("set-prompt-prefix-tr Specify second point: "));
                }
                else {
                    properties["destR.x"] = node_real(strList[0]);
                    properties["destR.y"] = node_real(strList[1]);
                    properties["angleRef"] = calculateAngle(properties["baseRX"], properties["baseRY"], properties["destRX"], properties["destRY"]);
                    previewOn("SELECTED", "ROTATE", properties["baseX"], properties["baseY"], properties["angleRef"]);
                    setRubberPoint("LINE_START", properties["baseX"], properties["baseY"]);
                    actuator("set-prompt-prefix-tr Specify the new angle: ");
                }
            }
            else {
                //The base and dest values are only set here to advance the command.
                properties["baseR.x"] = 0.0;
                properties["baseR.y"] = 0.0;
                properties["destR.x"] = 0.0;
                properties["destR.y"] = 0.0;
                //The reference angle is what we will use later.
                properties["angleRef"] = node_real(str);
                previewOn("SELECTED", "ROTATE", properties["baseX"], properties["baseY"], properties["angleRef"]);
                actuator("set-prompt-prefix-tr Specify the new angle: ");
            }
        }
        else if (std::isnan(properties["angleNew"])) {
            if (std::isnan(str)) {
                EmbReal strList = str.split(",");
                if (std::isnan(strList[0]) || std::isnan(strList[1])) {
                    alert(tr("Requires valid numeric angle or second point."));
                    actuator("set-prompt-prefix-tr Specify the new angle: ");
                }
                else {
                    EmbReal x = node_real(strList[0]);
                    EmbReal y = node_real(strList[1]);
                    properties["angleNew"] = calculateAngle(properties["baseX"], properties["baseY"], x, y);
                    rotateSelected(properties["baseX"], properties["baseY"], properties["angleNew"] - properties["angleRef"]);
                    previewOff();
                    actuator("end");
                }
            }
            else {
                properties["angleNew"] = node_real(str);
                rotateSelected(properties["baseX"], properties["baseY"], properties["angleNew"] - properties["angleRef"]);
                previewOff();
                actuator("end");
            }
        }
    }
}

//Command: Sandbox

/*String properties;
properties["test1"];
properties["test2"];
*/

/* . */
void
Geometry::sandbox_main(String str)
{
    actuator("init");

    //Report number of pre-selected objects
    setPromptPrefix("Number of Objects Selected: " + numSelected().toString());
    actuator("append-prompt-history");

    mirrorSelected(0,0,0,1);

    //selectAll();
    //rotateSelected(0,0,90);

    //Polyline & Polygon Testing

    properties["offset.x"] = node_real(0.0);
    properties["offset.y"] = node_real(0.0);

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

    properties["offset.x"] = 5.0;
    properties["offset.y"] = 0.0;

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

/* Command: Scale. */
void
Geometry::scale_main(void)
{
    actuator("init");

    mode = node_real("MODE_NORMAL");
    firstRun = node_real(true);
    properties["base.x"] = node_real(0.0f);
    properties["base.y"] = node_real(0.0f);
    properties["dest.x"] = node_real(0.0f);
    properties["dest.y"] = node_real(0.0f);
    properties["factor"] = node_real(0.0f);

    properties["baseR.x"] = node_real(0.0f);
    properties["baseR.y"] = node_real(0.0f);
    properties["destR.x"] = node_real(0.0f);
    properties["destR.y"] = node_real(0.0f);
    properties["factorRef"] = node_real(0.0f);
    properties["factorNew"] = node_real(0.0f);

    if (numSelected() <= 0) {
        //TODO: Prompt to select objects if nothing is preselected
        alert(tr("Preselect objects before invoking the scale command."));
        actuator("end");
        messageBox("information", tr("Scale Preselect"), tr("Preselect objects before invoking the scale command."));
    }
    else {
        actuator("set-prompt-prefix-tr Specify base point: "));
    }
}

/* . */
void
Geometry::scale_click(EmbVector v)
{
    switch (mode) {
    case SCALE_MODE_NORMAL: {
        if (firstRun) {
            firstRun = false;
            properties["base"] = v;
            addRubber("LINE");
            setRubberMode("LINE");
            setRubberPoint("LINE_START", properties["baseX"], properties["baseY"]);
            previewOn("SELECTED", "SCALE", properties["baseX"], properties["baseY"], 1);
            actuator("append-prompt-history");
            actuator("set-prompt-prefix-tr Specify scale factor or [Reference]: "));
        }
        else {
            properties["dest"] = v;
            properties["factor"] = calculateDistance(properties["base"], properties["dest"]);
            actuator("append-prompt-history");
            scaleSelected(properties["baseX"], properties["baseY"], properties["factor"]);
            previewOff();
            actuator("end");
        }
        break;
    }
    case "MODE_REFERENCE") {
        if (std::isnan(properties["baseRX"])) {
            properties["baseR"] = v;
            actuator("append-prompt-history");
            addRubber("LINE");
            setRubberMode("LINE");
            setRubberPoint("LINE_START", properties["baseR"]);
            actuator("set-prompt-prefix-tr Specify second point: "));
        }
        else if (std::isnan(properties["destRX"])) {
            properties["destR"] = v;
            properties["factorRef"] = calculateDistance(properties["baseRX"].r, properties["baseRY"].r, properties["destRX"].r, properties["destRY"].r);
            if (properties["factorRef"].r <= 0.0) {
                properties["destRX"] = node_real(0.0f);
                properties["destRY"] = node_real(0.0f);
                properties["factorRef"] = node_real(0.0f);
                alert(tr("Value must be positive and nonzero."));
                actuator("set-prompt-prefix-tr Specify second point: "));
            }
            else {
                actuator("append-prompt-history");
                setRubberPoint("LINE_START", properties["baseX"], properties["baseY"]);
                previewOn("SELECTED", "SCALE", properties["baseX"], properties["baseY"], properties["factorRef"]);
                actuator("set-prompt-prefix-tr Specify new length: ");
            }
        }
        else if (std::isnan(properties["factorNew"].r)) {
            properties["factorNew"] = calculateDistance(properties["baseX"], properties["baseY"], x, y);
            if (properties["factorNew"] <= 0.0) {
                properties["factorNew"] = node_real(0.0f);
                alert(tr("Value must be positive and nonzero."));
                actuator("set-prompt-prefix-tr Specify new length: ");
            }
            else {
                actuator("append-prompt-history");
                scaleSelected(properties["baseX"], properties["baseY"], properties["factorNew"]/properties["factorRef"]);
                previewOff();
                actuator("end");
            }
        }
    }
}

/* . */
void
Geometry::scale_context(String str)
{
    actuator("todo SCALE context()");
}

/* . */
void
Geometry::scale_prompt(String str)
{
    if (mode == MODE_NORMAL) {
        if (firstRun) {
            EmbReal strList = str.split(",");
            if (std::isnan(strList[0]) || std::isnan(strList[1])) {
                alert(tr("Invalid point."));
                actuator("set-prompt-prefix-tr Specify base point: ");
            }
            else {
                firstRun = false;
                properties["base.x"] = node_real(strList[0]);
                properties["base.y"] = node_real(strList[1]);
                addRubber("LINE");
                setRubberMode("LINE");
                setRubberPoint("LINE_START", properties["baseX"], properties["baseY"]);
                previewOn("SELECTED", "SCALE", properties["baseX"], properties["baseY"], 1);
                actuator("set-prompt-prefix-tr Specify scale factor or [Reference]: ");
            }
        }
        else {
            if (str == "R" || str == "REFERENCE") {
                mode = MODE_REFERENCE;
                actuator("set-prompt-prefix-tr Specify reference length {1}: ");
                clearRubber();
                previewOff();
            }
            else {
                if (std::isnan(str)) {
                    alert(tr("Requires valid numeric distance, second point, or option keyword."));
                    actuator("set-prompt-prefix-tr Specify scale factor or [Reference]: ");
                }
                else {
                    properties["factor"] = node_real(str);
                    scaleSelected(properties["base.x"], properties["base.y"], properties["factor"]);
                    previewOff();
                    actuator("end");
                }
            }
        }
    }
    case "MODE_REFERENCE") {
        if (std::isnan(properties["baseRX"].r)) {
            if (std::isnan(str)) {
                EmbReal strList = str.split(",");
                if (std::isnan(strList[0]) || std::isnan(strList[1])) {
                    alert(tr("Requires valid numeric distance or two points."));
                    actuator("set-prompt-prefix-tr Specify reference length {1}: ");
                }
                else {
                    properties["baseR.x"] = node_real(strList[0]);
                    properties["baseR.y"] = node_real(strList[1]);
                    addRubber("LINE");
                    setRubberMode("LINE");
                    setRubberPoint("LINE_START", properties["baseRX, properties["baseRY);
                    actuator("set-prompt-prefix-tr Specify second point: "));
                }
            }
            else {
                //The base and dest values are only set here to advance the command.
                properties["baseR.x"] = 0.0;
                properties["baseR.y"] = 0.0;
                properties["destR.x"] = 0.0;
                properties["destR.y"] = 0.0;
                //The reference length is what we will use later.
                properties["factorRef"] = node_real(str);
                if (properties["factorRef"] <= 0.0) {
                    properties["baseRX"] = node_real(0.0f);
                    properties["baseRY"] = node_real(0.0f);
                    properties["destRX"] = node_real(0.0f);
                    properties["destRY"] = node_real(0.0f);
                    properties["factorRef"] = node_real(0.0f);
                    alert(tr("Value must be positive and nonzero."));
                    actuator("set-prompt-prefix-tr Specify reference length") + " {1}: ");
                }
                else {
                    addRubber("LINE");
                    setRubberMode("LINE");
                    setRubberPoint("LINE_START", properties["baseX, properties["baseY);
                    previewOn("SELECTED", "SCALE", properties["baseX, properties["baseY, properties["factorRef);
                    actuator("set-prompt-prefix-tr Specify new length: "));
                }
            }
        }
        else if (std::isnan(properties["destRX"].r)) {
            if (std::isnan(str)) {
                EmbReal strList = str.split(",");
                if (std::isnan(strList[0]) || std::isnan(strList[1])) {
                    alert(tr("Requires valid numeric distance or two points."));
                    actuator("set-prompt-prefix-tr Specify second point: "));
                }
                else {
                    properties["destR.x"] = node_real(strList[0]);
                    properties["destR.y"] = node_real(strList[1]);
                    properties["factorRef = calculateDistance(properties["baseRX, properties["baseRY"], properties["destRX"], properties["destRY"]);
                    if (properties["factorRef"].r <= 0.0) {
                        properties["destRX"] = node_real(0.0f);
                        properties["destRY"] = node_real(0.0f);
                        properties["factorRef"] = node_real(0.0f);
                        alert(tr("Value must be positive and nonzero."));
                        actuator("set-prompt-prefix-tr Specify second point: "));
                    }
                    else {
                        setRubberPoint("LINE_START", properties["baseX"].r, properties["baseY"].r);
                        previewOn("SELECTED", "SCALE", properties["baseX"].r, properties["baseY"].r, properties["factorRef"].r);
                        actuator("set-prompt-prefix-tr Specify new length: ");
                    }
                }
            }
            else {
                //The base and dest values are only set here to advance the command.
                properties["baseR.x"] = 0.0;
                properties["baseR.y"] = 0.0;
                properties["destR.x"] = 0.0;
                properties["destR.y"] = 0.0;
                // The reference length is what we will use later.
                properties["factorRef"] = node_real(str);
                if (properties["factorRef"] <= 0.0) {
                    properties["destRX"] = node_real(0.0f);
                    properties["destRY"] = node_real(0.0f);
                    properties["factorRef"] = node_real(0.0f);
                    alert(tr("Value must be positive and nonzero."));
                    actuator("set-prompt-prefix-tr Specify second point: "));
                }
                else {
                    setRubberPoint("LINE_START", properties["baseX"], properties["baseY"]);
                    previewOn("SELECTED", "SCALE", properties["baseX"], properties["baseY"], properties["factorRef"]);
                    actuator("set-prompt-prefix-tr Specify new length: "));
                }
            }
        }
        else if (std::isnan(properties["factorNew"])) {
            if (std::isnan(str)) {
                EmbReal strList = str.split(",");
                if (std::isnan(strList[0]) || std::isnan(strList[1])) {
                    alert(tr("Requires valid numeric distance or second point."));
                    actuator("set-prompt-prefix-tr Specify new length: "));
                }
                else {
                    EmbReal x = node_real(strList[0]);
                    EmbReal y = node_real(strList[1]);
                    properties["factorNew"] = calculateDistance(properties["baseX"], properties["baseY"], x, y);
                    if (properties["factorNew"].r <= 0.0) {
                        properties["factorNew"] = node_real(0.0f);
                        alert(tr("Value must be positive and nonzero."));
                        actuator("set-prompt-prefix-tr Specify new length: "));
                    }
                    else {
                        scaleSelected(properties["baseX"].r, properties["baseY"].r, properties["factorNew"].r/properties["factorRef"].r);
                        previewOff();
                        actuator("end");
                    }
                }
            }
            else {
                properties["factorNew"] = node_real(str);
                if (properties["factorNew"].r <= 0.0) {
                    properties["factorNew"].r = node_real(0.0f);
                    alert(tr("Value must be positive and nonzero."));
                    actuator("set-prompt-prefix-tr Specify new length: "));
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

/* . */
void
Geometry::text_single_main(void)
{
    actuator("init");
    actuator("clear-selection");
    properties["text"] = node_real("");
    properties["text.x"] = node_real(0.0f);
    properties["text.y"] = node_real(0.0f);
    properties["text.justify"] = node_real("Left");
    properties["textFont"] = textFont();
    properties["textHeight"] = node_real(0.0f);
    properties["textRotation"] = node_real(0.0f);
    mode = MODE_TEXT_SINGLE_SETGEOM;
    actuator("set-prompt-prefix-tr Current font: " + "{" + properties["textFont"].s + "} " + tr("Text height: ") + "{" +  textSize() + "}");
    actuator("append-prompt-history");
    actuator("set-prompt-prefix-tr Specify start point of text or [Justify/Setfont]: ");
}

/* . */
void
Geometry::text_single_click(EmbVector v)
{
    switch (mode) {

    case MODE_TEXT_SINGLE_SETGEOM: {
        if (std::isnan(properties["textX"].r)) {
            properties["text.x"] = x;
            properties["text.y"] = y;
            addRubber("LINE");
            setRubberMode("LINE");
            setRubberPoint("LINE_START", properties["textX"].r, properties["textY"].r);
            actuator("append-prompt-history");
            actuator("set-prompt-prefix-tr Specify text height" + " {" + textSize() + "}: ");
        }
        else if (std::isnan(properties["textHeight"].r)) {
            properties["textHeight"] = calculateDistance(properties["textX"].r, properties["textY"].r, x, y);
            setTextSize(properties["textHeight"].r);
            actuator("append-prompt-history");
            actuator("set-prompt-prefix-tr Specify text angle") + " {" + textAngle() + "}: ");
        }
        else if (std::isnan(properties["textRotation"].r)) {
            properties["textRotation"] = calculateAngle(properties["textX"].r, properties["textY"].r, x, y);
            setTextAngle(properties["textRotation"].r);
            actuator("append-prompt-history");
            actuator("set-prompt-prefix-tr Enter text: "));
            mode = MODE_RAPID;
            prompt->enableRapidFire();
            clearRubber();
            addRubber("TEXTSINGLE");
            setRubberMode("TEXTSINGLE");
            setRubberPoint("TEXT_POINT", properties["textX"].r, properties["textY"].r);
            setRubberPoint("TEXT_HEIGHT_ROTATION", properties["textHeight"].r, properties["textRotation"].r);
            setRubberText("TEXT_FONT", properties["textFont"].r);
            setRubberText("TEXT_JUSTIFY", properties["textJustify"].r);
            setRubberText("TEXT_RAPID", properties["text"].r);
        }
        else {
            //Do nothing, as we are in rapidFire mode now.
        }
    }

    default:
        break;
    }
}

/* . */
void
Geometry::text_single_context(String str)
{
    actuator("todo SINGLELINETEXT context()");
}

/* . */
void
Geometry::text_single_prompt(String str)
{
    if (mode == "MODE_JUSTIFY") {
        if (str == "C" || str == "CENTER") {
            mode = MODE_SETGEOM;
            properties["textJustify"] = node("Center");
            setRubberText("TEXT_JUSTIFY", properties["textJustify"].s);
            actuator("set-prompt-prefix-tr Specify center point of text or [Justify/Setfont]: ");
        }
        else if (str == "R" || str == "RIGHT") {
            mode = MODE_SETGEOM;
            properties["textJustify"] = node("Right");
            setRubberText("TEXT_JUSTIFY", properties["textJustify"].s);
            actuator("set-prompt-prefix-tr Specify right-end point of text or [Justify/Setfont]: "));
        }
        else if (str == "A" || str == "ALIGN") {
            mode = MODE_SETGEOM;
            properties["textJustify"] = "Aligned";
            setRubberText("TEXT_JUSTIFY", properties["textJustify"].s);
            actuator("set-prompt-prefix-tr Specify start point of text or [Justify/Setfont]: "));
        }
        else if (str == "M" || str == "MIDDLE") {
            mode = MODE_SETGEOM;
            properties["textJustify"] = "Middle";
            setRubberText("TEXT_JUSTIFY", properties["textJustify"].s);
            actuator("set-prompt-prefix-tr Specify middle point of text or [Justify/Setfont]: ");
        }
        else if (str == "F" || str == "FIT") {
            mode = MODE_SETGEOM;
            properties["textJustify"] = "Fit";
            setRubberText("TEXT_JUSTIFY", properties["textJustify"]);
            actuator("set-prompt-prefix-tr Specify start point of text or [Justify/Setfont]: "));
        }
        else if (str == "TL" || str == "TOPLEFT") {
            mode = MODE_SETGEOM;
            properties["textJustify"] = "Top Left";
            setRubberText("TEXT_JUSTIFY", properties["textJustify"]);
            actuator("set-prompt-prefix-tr Specify top-left point of text or [Justify/Setfont]: "));
        }
        else if (str == "TC" || str == "TOPCENTER") {
            mode = MODE_SETGEOM;
            properties["textJustify"] = "Top Center";
            setRubberText("TEXT_JUSTIFY", properties["textJustify"]);
            actuator("set-prompt-prefix-tr Specify top-center point of text or [Justify/Setfont]: "));
        }
        else if (str == "TR" || str == "TOPRIGHT") {
            mode = MODE_SETGEOM;
            properties["textJustify"] = "Top Right";
            setRubberText("TEXT_JUSTIFY", properties["textJustify"].s);
            actuator("set-prompt-prefix-tr Specify top-right point of text or [Justify/Setfont]: "));
        }
        else if (str == "ML" || str == "MIDDLELEFT") {
            mode = MODE_SETGEOM;
            properties["textJustify"] = "Middle Left";
            setRubberText("TEXT_JUSTIFY", properties["textJustify"].s);
            actuator("set-prompt-prefix-tr Specify middle-left point of text or [Justify/Setfont]: ");
        }
        else if (str == "MC" || str == "MIDDLECENTER") {
            mode = MODE_SETGEOM;
            properties["textJustify"] = "Middle Center";
            setRubberText("TEXT_JUSTIFY", properties["textJustify"]);
            actuator("set-prompt-prefix-tr Specify middle-center point of text or [Justify/Setfont]: ");
        }
        else if (str == "MR" || str == "MIDDLERIGHT") {
            mode = MODE_SETGEOM;
            properties["textJustify"] = "Middle Right";
            setRubberText("TEXT_JUSTIFY", properties["textJustify"].s);
            actuator("set-prompt-prefix-tr Specify middle-right point of text or [Justify/Setfont]: ");
        }
        else if (str == "BL" || str == "BOTTOMLEFT") {
            mode = MODE_SETGEOM;
            properties["textJustify"] = "Bottom Left";
            setRubberText("TEXT_JUSTIFY", properties["textJustify"].s);
            actuator("set-prompt-prefix-tr Specify bottom-left point of text or [Justify/Setfont]: ");
        }
        else if (str == "BC" || str == "BOTTOMCENTER") {
            mode = MODE_SETGEOM;
            properties["textJustify"] = "Bottom Center";
            setRubberText("TEXT_JUSTIFY", properties["textJustify"].s);
            actuator("set-prompt-prefix-tr Specify bottom-center point of text or [Justify/Setfont]: "));
        }
        else if (str == "BR" || str == "BOTTOMRIGHT") {
            mode = MODE_SETGEOM;
            properties["textJustify"] = "Bottom Right";
            setRubberText("TEXT_JUSTIFY", properties["textJustify"].s);
            actuator("set-prompt-prefix-tr Specify bottom-right point of text or [Justify/Setfont]: "));
        }
        else {
            alert(translate("Invalid option keyword."));
            actuator("set-prompt-prefix-tr Text Justification Options [Center/Right/Align/Middle/Fit/TL/TC/TR/ML/MC/MR/BL/BC/BR]: ");
        }
    }
    case "MODE_SETFONT") {
        mode = "MODE_SETGEOM";
        properties["textFont"] = str;
        setRubberText("TEXT_FONT", properties["textFont"].s);
        setTextFont(properties["textFont"].s);
        actuator("set-prompt-prefix-tr Specify start point of text or [Justify/Setfont]: ");
    }
    case "MODE_SETGEOM") {
        if (std::isnan(properties["textX"])) {
            if (str == "J" || str == "JUSTIFY") {
                mode = "MODE_JUSTIFY";
                actuator("set-prompt-prefix-tr Text Justification Options [Center/Right/Align/Middle/Fit/TL/TC/TR/ML/MC/MR/BL/BC/BR]: ");
            }
            else if (str == "S" || str == "SETFONT") {
                mode = MODE_SETFONT;
                actuator("set-prompt-prefix-tr Specify font name: ");
            }
            else {
                EmbReal strList = str.split(",");
                if (std::isnan(strList[0]) || std::isnan(strList[1])) {
                    alert(tr("Point or option keyword required."));
                    actuator("set-prompt-prefix-tr Specify start point of text or [Justify/Setfont]: ");
                }
                else {
                    properties["text.x"] = node_real(strList[0]);
                    properties["text.y"] = node_real(strList[1]);
                    addRubber("LINE");
                    setRubberMode("LINE");
                    setRubberPoint("LINE_START", properties["textX"], properties["textY"]);
                    actuator("set-prompt-prefix-tr Specify text height") + " {" + textSize() + "}: ");
                }
            }
        }
        else if (std::isnan(properties["textHeight"].r)) {
            if (str == "") {
                properties["textHeight"] = textSize();
                actuator("set-prompt-prefix-tr Specify text angle") + " {" + textAngle() + "}: ");
            }
            else if (std::isnan(str)) {
                alert(tr("Requires valid numeric distance or second point."));
                actuator("set-prompt-prefix-tr Specify text height") + " {" + textSize() + "}: ");
            }
            else {
                properties["textHeight"] = node_real(str);
                setTextSize(properties["textHeight"]);
                actuator("set-prompt-prefix-tr Specify text angle") + " {" + textAngle() + "}: ");
            }
        }
        else if (std::isnan(properties["textRotation"])) {
            if (str == "") {
                properties["textRotation"] = textAngle();
                actuator("set-prompt-prefix-tr Enter text: ");
                mode = "MODE_RAPID";
                prompt->enableRapidFire();
                clearRubber();
                addRubber("TEXTSINGLE");
                setRubberMode("TEXTSINGLE");
                setRubberPoint("TEXT_POINT", properties["textX"].r, properties["textY"].r);
                setRubberPoint("TEXT_HEIGHT_ROTATION", properties["textHeight"].r, properties["textRotation"].r);
                setRubberText("TEXT_FONT", properties["textFont"].s);
                setRubberText("TEXT_JUSTIFY", properties["textJustify"].s);
                setRubberText("TEXT_RAPID", properties["text"].s);
            }
            else if (std::isnan(str)) {
                alert(tr("Requires valid numeric angle or second point."));
                actuator("set-prompt-prefix-tr Specify text angle") + " {" + textAngle() + "}: ");
            }
            else {
                properties["textRotation"] = node_real(str);
                setTextAngle(properties["textRotation"].r);
                actuator("set-prompt-prefix-tr Enter text: ");
                mode = MODE_RAPID;
                prompt->enableRapidFire();
                clearRubber();
                addRubber("TEXTSINGLE");
                setRubberMode("TEXTSINGLE");
                setRubberPoint("TEXT_POINT", properties["textX"], properties["textY"]);
                setRubberPoint("TEXT_HEIGHT_ROTATION", properties["textHeight"], properties["textRotation"]);
                setRubberText("TEXT_FONT", properties["textFont"]);
                setRubberText("TEXT_JUSTIFY", properties["textJustify"]);
                setRubberText("TEXT_RAPID", properties["text"]);
            }
        }
        else {
            //Do nothing, as we are in rapidFire mode now.
        }
    }
    case "MODE_RAPID") {
        if (str == "RAPID_ENTER") {
            if (properties["text"] == "") {
                actuator("end");
            }
            else {
                actuator("vulcanize");
                actuator("end");
                /** TODO: Rather than ending the command, calculate where the
                 * next line would be and modify the x/y to the new point.
                 */
            }
        }
        else {
            properties["text"] = node_real(str);
            setRubberText("TEXT_RAPID", properties["text"].s);
        }
    }
}

/* . */
void
Geometry::snowflake_main(void)
{
    object_script(snowflake_init);
}

/* Snowflake Curve with $t \in [0, 2\pi]$. */
void
Geometry::update_snowflake(void)
{
    EmbVector scale;
    scale.x = properties["scale.x"].r;
    scale.y = properties["scale.y"].r;
    for (int i = 0; i <= properties["numPoints"].i; i++) {
        EmbReal t = (2.0*CONSTANT_PI) / properties["numPoints"].i*i;
        EmbVector v;
        v.x = fourier_series(t, snowflake_x);
        v.y = fourier_series(t, snowflake_y);

        setRubberPoint("POLYGON_POINT_" + i.toString(), v.x*scale.x, v.y*scale.y);
    }

    setRubberText("POLYGON_NUM_POINTS", numPoints.toString());
}

/* . */
void
Geometry::star_main(void)
{
    actuator("init");
    actuator("clear-selection");
    properties["numPoints"] = node_real(5);
    properties["center.x"] = node_real(0.0f);
    properties["center.y"] = node_real(0.0f);
    properties["point1.x"] = node_real(1.0f);
    properties["point1.y"] = node_real(1.0f);
    properties["point2.x"] = node_real(2.0f);
    properties["point2.y"] = node_real(2.0f);
    mode = MODE_STAR_NUM_POINTS;
    actuator("set-prompt-prefix-tr Enter number of star points {5}: ");
}

/* . */
void
Geometry::star_click(EmbReal mouse)
{
    switch (mode) {

    case MODE_STAR_NUM_POINTS: {
        /* Do nothing, the prompt controls this. */
        break;
    }

    case MODE_STAR_CENTER_PT: {
        properties["center"] = mouse;
        mode = STAR_MODE_RAD_OUTER;
        actuator("set-prompt-prefix-tr Specify outer radius of star: ");
        addRubber("POLYGON");
        setRubberMode("POLYGON");
        updateStar(properties, properties["center"]);
        actuator("enable move-rapid-fire");
        break;
    }

    case MODE_STAR_RAD_OUTER: {
        properties["point1"] = mouse;
        mode.s = "MODE_RAD_INNER";
        actuator("set-prompt-prefix-tr Specify inner radius of star: ");
        updateStar(properties["point1"]);
        break;
    }

    case MODE_STAR_RAD_INNER: {
        properties["point2"] = mouse;
        actuator("disable move-rapid-fire");
        updateStar(properties["point2"]);
        spareRubber("POLYGON");
        actuator("end");
        break;
    }

    default: {
        break;
    }

    }
}

/* . */
const char star_move[][] = {
    (EmbVector v)
    switch (mode) {
    case MODE_STAR_NUM_POINTS: {
        //Do nothing, the prompt controls this.
        break;
    }
    case MODE_STAR_CENTER_PT: {
        //Do nothing, prompt and click controls this.
        break;
    }
    case MODE_STAR_RAD_OUTER: {
        properties = updateStar(properties, v);
        break;
    }
    case MODE_STAR_RAD_INNER: {
        properties = updateStar(properties, v);
        break;
    }
};

/* . */
void
Geometry::star_context(String str)
{
    actuator("todo STAR context()");
}

/* . */
void
Geometry::star_prompt(String str)
{
    switch (mode) {
    case STAR_MODE_NUM_POINTS: {
        if (str == "" && properties["numPoints"].i >= 3 && properties["numPoints"].i <= 1024) {
            actuator("set-prompt-prefix-tr Specify center point: ");
            mode = MODE_STAR_CENTER_PT;
        }
        else {
            EmbReal tmp = node_real(str);
            if (std::isnan(tmp) || !isInt(tmp) || tmp < 3 || tmp > 1024) {
                alert(tr("Requires an integer between 3 and 1024."));
                actuator("set-prompt-prefix-tr Enter number of star points") + " {" + properties["numPoints"].toString() + "}: ");
            }
            else {
                properties["numPoints"] = tmp;
                actuator("set-prompt-prefix-tr Specify center point: ");
                mode = MODE_STAR_CENTER_PT;
            }
        }
        break;
    }

    case MODE_STAR_CENTER_PT: {
        EmbReal strList = str.split(",");
        if (std::isnan(strList[0]) || std::isnan(strList[1])) {
            alert(tr("Invalid point."));
            actuator("set-prompt-prefix-tr Specify center point: ");
        }
        else {
            properties["center.x"] = node_real(strList[0]);
            properties["center.y"] = node_real(strList[1]);
            mode = MODE_STAR_RAD_OUTER;
            actuator("set-prompt-prefix-tr Specify outer radius of star: ");
            addRubber("POLYGON");
            setRubberMode("POLYGON");
            updateStar(qsnapX(), qsnapY());
            actuator("enable move-rapid-fire");
        }
        break;
    }

    case MODE_STAR_RAD_OUTER: {
        EmbReal strList = str.split(",");
        if (std::isnan(strList[0]) || std::isnan(strList[1])) {
            alert(tr("Invalid point."));
            actuator("set-prompt-prefix-tr Specify outer radius of star: ");
        }
        else {
            point1.x = node_real(strList[0]);
            point1.y = node_real(strList[1]);
            mode = MODE_RAD_INNER;
            actuator("set-prompt-prefix-tr Specify inner radius of star: ");
            updateStar(qsnapX(), qsnapY());
        }
        break;
    }

    case MODE_STAR_RAD_INNER: {
        EmbReal strList = str.split(",");
        if (std::isnan(strList[0]) || std::isnan(strList[1])) {
            alert(tr("Invalid point."));
            actuator("set-prompt-prefix-tr Specify inner radius of star: ");
        }
        else {
            point2.x = node_real(strList[0]);
            point2.y = node_real(strList[1]);
            actuator("disable move-rapid-fire");
            updateStar(point2.x, point2.y);
            spareRubber("POLYGON");
            actuator("end");
        }
        break;
    }

    default:
        break;
    }

}

/* Update star. */
void
Geometry::updateStar(EmbVector mouse)
{
    EmbVector v = mouse - properties["center"].v;
    EmbReal angOuter = embVector_angle(v);
    EmbReal distOuter = embVector_length(v);
    EmbReal distInner = distOuter/2.0;

    if (mode == MODE_STAR_RAD_INNER) {
        EmbVector v = properties["point1"].v - properties["center"].v;
        angOuter = embVector_angle(v);
        distOuter = embVector_length(v);
        distInner = embVector_distance(properties["center"].v, mouse);
    }

    // Calculate the Star Points
    EmbReal angInc = 360.0/(properties["numPoints"].i*2);
    for (int i = 0; i < properties["numPoints"].i*2; i++) {
        EmbReal angle = (angOuter + (angInc*i)) * CONSTANT_PI / 180.0;
        EmbVector v = embVector_unit(angle);
        if (i%2 == 0) {
            v = v * distOuter;
        }
        else {
            v = v * distInner;
        }
        setRubberPoint(
            QString::fromStdString("POLYGON_POINT_" + std::to_string(i)),
            properties["center.x"].r + v.x,
            properties["center.y"].r + v.y);
    }
    setRubberText("POLYGON_NUM_POINTS", (properties["numPoints*2"].i - 1).toString());
}
#endif

