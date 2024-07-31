/*
 * Embroidermodder 2.
 *
 * Copyright 2011-2024 The Embroidermodder Team
 * Embroidermodder 2 is Open Source Software, see LICENSE.md for licensing terms.
 *
 * Download the docs at https://www.libembroidery.org/downloads/emrm_current.pdf
 * for advice on altering this file.
 *
 * Object: attempting to combine the geometry objects into one that changes
 * behaviour depending on its mode.
 */

#include "embroidermodder.h"

/* . */
Object::Object(EmbArc arc, QRgb rgb, QGraphicsItem *item)
{
    qDebug("ArcObject Constructor()");
    TYPE = OBJ_TYPE_ARC;
    curved = 0;
    geometry->object.arc = arc;
    init(arc, rgb, Qt::SolidLine); //TODO: getCurrentLineType
}

/* . */
Object::Object(EmbCircle circle, QRgb rgb, QGraphicsItem *item)
{
    TYPE = OBJ_TYPE_CIRCLE;
    geometry->object.circle = circle;
}

/* . */
Object::Object(EmbEllipse ellipse, QRgb rgb, QGraphicsItem *item)
{
    TYPE = OBJ_TYPE_ELLIPSE;
    geometry->object.ellipse = ellipse;
}

/* . */
Object::Object(Object* obj, QGraphicsItem* parent)
{
    qDebug("ArcObject Constructor()");
    if (obj) {
        init(obj->geometry->object.arc, obj->objectColorRGB(), Qt::SolidLine);
        //TODO: getCurrentLineType
        setRotation(obj->rotation());
    }
}

/* . */
Object::~Object()
{
    qDebug("ArcObject Destructor()");
    free(geometry);
}

/*
QPointF
Object::objectEndPoint1() const
{
    return QPointF(0.0f, 0.0f);
}

QPointF
Object::objectEndPoint2() const
{
    return QPointF(0.0f, 0.0f);
}
*/

/* . */
QPointF
Object::objectStartPoint() const
{
    switch (geometry->type) {
    case EMB_ARC: {
        return to_qpointf(geometry->object.arc.start);
    }
    default:
        break;
    }
    return scenePos();
}

/* . */
QPointF
Object::objectMidPoint() const
{
    switch (geometry->type) {
    case EMB_ARC: {
        return to_qpointf(geometry->object.arc.mid);
    }
    case EMB_LINE:
    case EMB_DIM_LEADER: {
        QLineF lyne = line();
        QPointF mp = lyne.pointAt(0.5);
        return scenePos() + scale_and_rotate(mp, scale(), rotation());
    }
    default:
        break;
    }
    return scenePos();
}

/* . */
QPointF
Object::objectEndPoint() const
{
    switch (geometry->type) {
    case EMB_ARC: {
        return to_qpointf(geometry->object.arc.end);
    }
    default:
        break;
    }
    return scenePos();
}

/* . */
QPointF
Object::objectQuadrant0()
{
    switch (geometry->type) {
    case OBJ_TYPE_CIRCLE:
        return objectCenter() + QPointF(objectRadius(), 0);
    default:
        break;
    }
    return objectCenter();
}

QPointF
Object::objectQuadrant90()
{
    switch (geometry->type) {
    case OBJ_TYPE_CIRCLE:
        return objectCenter() + QPointF(0, -objectRadius());
    default:
        break;
    }
    return objectCenter();
}

QPointF
Object::objectQuadrant180()
{
    switch (geometry->type) {
    case EMB_CIRCLE:
        return objectCenter() + QPointF(-objectRadius(),0);
    default:
        break;
    }
    return objectCenter();
}

QPointF
Object::objectQuadrant270()
{
    switch (geometry->type) {
    case EMB_CIRCLE:
        return objectCenter() + QPointF(0, objectRadius());
    default:
        break;
    }
    return objectCenter();
}

/* . */
QPointF
Object::objectTopLeft() const
{
    return scenePos() + scale_and_rotate(rect().topLeft(), scale(), rotation());
}

/* . */
QPointF
Object::objectTopRight() const
{
    return scenePos() + scale_and_rotate(rect().topRight(), scale(), rotation());
}

/* . */
QPointF
Object::objectBottomLeft() const
{
    return scenePos() + scale_and_rotate(rect().bottomLeft(), scale(), rotation());
}

/* . */
QPointF
Object::objectBottomRight() const
{
    return scenePos() + scale_and_rotate(rect().bottomRight(), scale(), rotation());
}

/*
QPointF
Object::objectCenter()
{
    return QPointF(0.0f, 0.0f);
}

double
Object::objectWidth()
{
    return 0.0f;
}

double
Object::objectHeight()
{
    return 0.0f;
}

Object::Object(QString const&, double, double, unsigned int, QGraphicsItem*)
{
}
*/

/* . */
QStringList
Object::objectTextJustifyList() const
{
    QStringList justifyList;
    justifyList << "Left" << "Center" << "Right" /* TODO: << "Aligned" */ << "Middle" /* TODO: << "Fit" */ ;
    justifyList << "Top Left" << "Top Center" << "Top Right";
    justifyList << "Middle Left" << "Middle Center" << "Middle Right";
    justifyList << "Bottom Left" << "Bottom Center" << "Bottom Right";
    return justifyList;
}

/* . */
void
Object::setObjectText(const QString& str)
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
    if     (objTextJustify == "Left") {
        textPath.translate(-jRect.left(), 0);
    }
    else if (objTextJustify == "Center") {
        textPath.translate(-jRect.center().x(), 0);
    }
    else if (objTextJustify == "Right") {
        textPath.translate(-jRect.right(), 0);
    }
    else if (objTextJustify == "Aligned") {
        // TODO: TextSingleObject Aligned Justification
    }
    else if (objTextJustify == "Middle") {
        textPath.translate(-jRect.center());
    }
    else if (objTextJustify == "Fit") {
        // TODO: TextSingleObject Fit Justification
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

    //Backward or Upside Down
    if (objTextBackward || objTextUpsideDown) {
        double horiz = 1.0;
        double vert = 1.0;
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
    else
        objTextPath = textPath;

    //Add the grip point to the shape path
    QPainterPath gripPath = objTextPath;
    gripPath.connectPath(objTextPath);
    gripPath.addRect(-0.00000001, -0.00000001, 0.00000002, 0.00000002);
    setObjectPath(gripPath);
}

/* . */
void
Object::setObjectTextFont(const QString& font)
{
    objTextFont = font;
    setObjectText(objText);
}

/* . */
void
Object::setObjectTextJustify(const QString& justify)
{
    //Verify the string is a valid option
    if (justify == "Left") {
        objTextJustify = justify;
    }
    else if (justify == "Center") {
        objTextJustify = justify;
    }
    else if (justify == "Right") {
        objTextJustify = justify;
    }
    else if (justify == "Aligned") {
        objTextJustify = justify;
    }
    else if (justify == "Middle") {
        objTextJustify = justify;
    }
    else if (justify == "Fit") {
        objTextJustify = justify;
    }
    else if (justify == "Top Left") {
        objTextJustify = justify;
    }
    else if (justify == "Top Center") {
        objTextJustify = justify;
    }
    else if (justify == "Top Right") {
        objTextJustify = justify;
    }
    else if (justify == "Middle Left") {
        objTextJustify = justify;
    }
    else if (justify == "Middle Center") {
        objTextJustify = justify;
    }
    else if (justify == "Middle Right") {
        objTextJustify = justify;
    }
    else if (justify == "Bottom Left") {
        objTextJustify = justify;
    }
    else if (justify == "Bottom Center") {
        objTextJustify = justify;
    }
    else if (justify == "Bottom Right") {
        objTextJustify = justify;
    }
    else {
        // Default
        objTextJustify = "Left";
    }
    setObjectText(objText);
}

/* . */
void
Object::setObjectTextSize(double size)
{
    objTextSize = size;
    setObjectText(objText);
}

/* . */
void
Object::setObjectTextStyle(bool bold, bool italic, bool under, bool strike, bool over)
{
    objTextBold = bold;
    objTextItalic = italic;
    objTextUnderline = under;
    objTextStrikeOut = strike;
    objTextOverline = over;
    setObjectText(objText);
}

void
Object::setObjectTextBold(bool val)
{
    objTextBold = val;
    setObjectText(objText);
}

/* . */
void
Object::setObjectTextItalic(bool val)
{
    objTextItalic = val;
    setObjectText(objText);
}

/* . */
void
Object::setObjectTextUnderline(bool val)
{
    objTextUnderline = val;
    setObjectText(objText);
}

/* . */
void
Object::setObjectTextStrikeOut(bool val)
{
    objTextStrikeOut = val;
    setObjectText(objText);
}

/* . */
void
Object::setObjectTextOverline(bool val)
{
    objTextOverline = val;
    setObjectText(objText);
}

/* . */
void
Object::setObjectTextBackward(bool val)
{
    objTextBackward = val;
    setObjectText(objText);
}

/* . */
void
Object::setObjectTextUpsideDown(bool val)
{
    objTextUpsideDown = val;
    setObjectText(objText);
}

/*
Object::Object(double, double, double, double, unsigned int, QGraphicsItem*)
{
}

Object::Object(double, double, double, double, unsigned int, QGraphicsItem*)'
{
}

Object::Object(EmbPoint_, unsigned int, QGraphicsItem*)'
{
}

Object::Object(EmbPath_, int, QPainterPath const&, unsigned int, QGraphicsItem*)'
{
}

Object::Object(EmbPath_, int, QPainterPath const&, unsigned int, QGraphicsItem*)'
{
}

Object::objectIncludedAngle() const
{
}

Object::objectClockwise() const
{
}

Object::realRender(QPainter*, QPainterPath const&)
{
}

Object::gripEdit(QPointF const&, QPointF const&)
{
}

Object::updatePath(QPainterPath const&)
{
}
*/

QPointF
Object::objectEndPoint1()
{
    return QPointF(0.0, 0.0);
}

QPointF
Object::objectEndPoint2()
{
    return QPointF(0.0, 0.0);
}

/* . */
void
Object::setObjectEndPoint1(const QPointF& endPt1)
{
    setObjectEndPoint1(endPt1.x(), endPt1.y());
}

/* . */
void
Object::setObjectEndPoint1(double x1, double y1)
{
    QPointF endPt2 = objectEndPoint2();
    double x2 = endPt2.x();
    double y2 = endPt2.y();
    double dx = x2 - x1;
    double dy = y2 - y1;
    setRotation(0);
    setScale(1);
    setLine(0, 0, dx, dy);
    setPos(x1, y1);
}

/* . */
double
Object::objectAngle() const
{
    double angle = line().angle() - rotation();
    while (angle >= 360.0) {
        angle -= 360.0;
    }
    while (angle < 0.0) {
        angle += 360.0;
    }
    return angle;
}

/* . */
double
Object::objectStartAngle() const
{
    switch (geometry->type) {
    case EMB_ARC: {
        double angle = QLineF(scenePos(), objectStartPoint()).angle();
        return fmod(angle+360.0, 360.0);
    }
    default:
        break;
    }
    return 0.0f;
}

/* . */
double
Object::objectEndAngle() const
{
    switch (geometry->type) {
    case EMB_ARC: {
        double angle = QLineF(scenePos(), objectEndPoint()).angle();
        return fmod(angle+360.0, 360.0);
    }
    default:
        break;
    }
    return 0.0f;
}

/* . */
double
Object::objectArcLength() const
{
    switch (geometry->type) {
    case EMB_ARC: {
        return radians(objectIncludedAngle()) * objectRadius();
    }
    default:
        break;
    }
    return 0.0;
}


/* . */
void
Object::setObjectStartAngle(double angle)
{
    switch (geometry->type) {
    case EMB_ARC: {
        //TODO: ArcObject setObjectStartAngle
        break;
    }
    default:
        break;
    }
}

/* . */
void
Object::setObjectEndAngle(double angle)
{
    switch (geometry->type) {
    case EMB_ARC: {
        //TODO: ArcObject setObjectEndAngle
        break;
    }
    default:
        break;
    }
}

/* . */
void
Object::setObjectStartPoint(EmbVector point)
{
    switch (geometry->type) {
    case EMB_ARC: {
        geometry->object.arc.start = point;
        calculateArcData(geometry->object.arc);
        break;
    }
    default:
        break;
    }
}

/* . */
void
Object::setObjectMidPoint(EmbVector point)
{
    switch (geometry->type) {
    case EMB_ARC: {
        geometry->object.arc.mid = point;
        calculateArcData(geometry->object.arc);
        break;
    }
    default:
        break;
    }
}

/* . */
void
Object::setObjectEndPoint(EmbVector point)
{
    switch (geometry->type) {
    case EMB_ARC: {
        geometry->object.arc.end = point;
        calculateArcData(geometry->object.arc);
        break;
    }
    default:
        break;
    }
}

/*
double
Object::objectArea() const
{
    switch (geometry->type) {
    case EMB_ARC: {
        //Area of a circular segment
        double r = objectRadius();
        double theta = radians(objectIncludedAngle());
        return ((r*r)/2)*(theta - sin(theta));
    }
    default:
        break;
    }
    return 0.0;
}
*/

/* . */
double
Object::objectChord() const
{
    switch (geometry->type) {
    case EMB_ARC: {
        return QLineF(objectStartPoint(), objectEndPoint()).length();
    }
    default:
        break;
    }
    return 0.0;
}

/* . */
double
Object::objectIncludedAngle() const
{
    switch (geometry->type) {
    case EMB_ARC: {
        double chord = objectChord();
        double rad = objectRadius();
        if (chord <= 0 || rad <= 0) {
            /* Prevents division by zero and non-existant circles. */
            return 0;
        }

        //NOTE: Due to floating point rounding errors, we need to clamp the quotient so it is in the range [-1, 1]
        //      If the quotient is out of that range, then the result of asin() will be NaN.
        double quotient = chord/(2.0*rad);
        quotient = EMB_MIN(1.0, quotient);
        quotient = EMB_MAX(0.0, quotient); //NOTE: 0 rather than -1 since we are enforcing a positive chord and radius
        return degrees(2.0*asin(quotient)); //Properties of a Circle - Get the Included Angle - Reference: ASD9
    }
    default:
        break;
    }
    return 0.0;
}

/* . */
bool
Object::objectClockwise() const
{
    switch (geometry->type) {
    case EMB_ARC: {
        //NOTE: Y values are inverted here on purpose
        EmbArc arc;
        geometry->object.arc.start.y = -geometry->object.arc.start.y;
        geometry->object.arc.mid.y = -geometry->object.arc.start.y;
        geometry->object.arc.end.y = -geometry->object.arc.end.y;
        if (emb_arc_clockwise(arc)) {
            return true;
        }
        break;
    }
    default:
        break;
    }
    return false;
}

/* . */
void
Object::updatePath()
{
    switch (geometry->type) {
    case EMB_ARC: {
        double startAngle = (objectStartAngle() + rotation());
        double spanAngle = objectIncludedAngle();

        if (objectClockwise()) {
            spanAngle = -spanAngle;
        }

        QPainterPath path;
        path.arcMoveTo(rect(), startAngle);
        path.arcTo(rect(), startAngle, spanAngle);
        //NOTE: Reverse the path so that the inside area isn't considered part of the arc
        path.arcTo(rect(), startAngle+spanAngle, -spanAngle);
        setObjectPath(path);
        break;
    }
    default:
        break;
    }
}

/* . */
Object::Object(QString const&, double, double, unsigned int, QGraphicsItem*)
{
}

/* . */
Object::Object(double, double, double, double, unsigned int, QGraphicsItem*)
{
}

/* . */
Object::Object(EmbPoint_, unsigned int, QGraphicsItem*)
{
}

/* . */
Object::Object(EmbPath_, int, QPainterPath const&, unsigned int, QGraphicsItem*)
{
}

/* WARNING: DO NOT enable QGraphicsItem::ItemIsMovable. If it is enabled,
 * and the item is double clicked, the scene will erratically move the item while zooming.
 * All movement has to be handled explicitly by us, not by the scene.
 */
void
Object::init(EmbArc arc, QRgb rgb, Qt::PenStyle lineType)
{
    setData(OBJ_TYPE, type());
    setData(OBJ_NAME, OBJ_NAME_ARC);

    setFlag(QGraphicsItem::ItemIsSelectable, true);

    geometry->object.arc = arc;
    geometry->object.color.r = qRed(rgb);
    geometry->object.color.g = qGreen(rgb);
    geometry->object.color.b = qBlue(rgb);
    geometry->lineType = lineType;
    geometry->type = EMB_ARC;

    calculateArcData(geometry->object.arc);

    setPos(arc.start.x, arc.start.y);
    setObjectLineWeight(0.35); /* TODO: pass in proper lineweight */
    setPen(objPen);
}

/* . */
void
Object::calculateArcData(EmbArc arc)
{
    EmbVector center = emb_arc_center(arc);

    double radius = emb_vector_distance(center, arc.mid);
    updateArcRect(radius);
    updatePath();
    setRotation(0);
    setScale(1);
}

/* . */
void
Object::updateArcRect(double radius)
{
    QRectF arcRect;
    arcRect.setWidth(radius*2.0);
    arcRect.setHeight(radius*2.0);
    arcRect.moveCenter(QPointF(0,0));
    setRect(arcRect);
}

/* . */
void
Object::setObjectLineWeight(double lineWeight)
{
    objPen.setWidthF(0); //NOTE: The objPen will always be cosmetic

    if (lineWeight < 0) {
        if (lineWeight == OBJ_LWT_BYLAYER) {
            lwtPen.setWidthF(0.35); //TODO: getLayerLineWeight
        }
        else if (lineWeight == OBJ_LWT_BYBLOCK) {
            lwtPen.setWidthF(0.35); //TODO: getBlockLineWeight
        }
        else {
            QMessageBox::warning(0, QObject::tr("Error - Negative Lineweight"),
                                    QObject::tr("Lineweight: %1")
                                    .arg(QString().setNum(lineWeight)));
            qDebug("Lineweight cannot be negative! Inverting sign.");
            lwtPen.setWidthF(-lineWeight);
        }
    }
    else {
        lwtPen.setWidthF(lineWeight);
    }
}

/* . */
void
Object::realRender(QPainter*, QPainterPath const&)
{

}

/* . */
void
Object::updatePath(QPainterPath const& p)
{
}

/* . */
void
Object::setObjectColor(const QColor& color)
{
    objPen.setColor(color);
    lwtPen.setColor(color);
}

/* . */
void
Object::setObjectColorRGB(QRgb rgb)
{
    objPen.setColor(QColor(rgb));
    lwtPen.setColor(QColor(rgb));
}

/* . */
void
Object::setObjectLineType(Qt::PenStyle lineType)
{
    objPen.setStyle(lineType);
    lwtPen.setStyle(lineType);
}

/* . */
QPointF
Object::objectRubberPoint(const QString& key) const
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

/* . */
QString
Object::objectRubberText(const QString& key) const
{
    if (objRubberTexts.contains(key)) {
        return objRubberTexts.value(key);
    }
    return QString();
}

/* . */
QRectF
Object::boundingRect() const
{
    /* If gripped, force this object to be drawn even if it is offscreen. */
    if (objectRubberMode() == OBJ_RUBBER_GRIP) {
        return scene()->sceneRect();
    }
    return path().boundingRect();
}

/* . */
void
Object::setObjectCenter(EmbVector center)
{
    setPos(center.x, center.y);
}

/* . */
void
Object::setObjectCenterX(double centerX)
{
    setX(centerX);
}

/* . */
void
Object::setObjectCenterY(double centerY)
{
    setY(centerY);
}

/* . */
void
Object::setObjectRadius(double radius)
{
    switch (geometry->type) {
    case EMB_ARC: {
        geometry->object.arc = emb_arc_set_radius(geometry->object.arc, radius);
        calculateArcData(geometry->object.arc);
        break;
    }
    case EMB_CIRCLE:
        setObjectDiameter(radius*2.0);
        break;
    default:
        break;
    }
}

/* . */
void
Object::setObjectDiameter(double diameter)
{
    switch (geometry->type) {
    case EMB_CIRCLE: {
        QRectF circRect;
        circRect.setWidth(diameter);
        circRect.setHeight(diameter);
        circRect.moveCenter(QPointF(0,0));
        setRect(circRect);
        //FIXME: updatePath();
        break;
    }
    default:
        break;
    }
}

/* . */
void
Object::setObjectArea(double area)
{
    switch (geometry->type) {
    case EMB_CIRCLE: {
        double radius = sqrt(area / embConstantPi);
        setObjectRadius(radius);
        break;
    }
    default:
        break;
    }
}

/* . */
void
Object::setObjectCircumference(double circumference)
{
    switch (geometry->type) {
    case EMB_CIRCLE: {
        double diameter = circumference / embConstantPi;
        setObjectDiameter(diameter);
        break;
    }
    default:
        break;
    }
}

/* . */
int
Object::findIndex(const QPointF& position)
{
    return 0;
}

void
Object::updateRubber(QPainter* painter)
{
    switch (geometry->type) {
    case EMB_ARC:
        //TODO: Arc Rubber Modes

        //TODO: updateRubber() gripping for ArcObject
        break;
    default:
        break;
    }
}

/* . */
void
Object::setObjectRect(double x, double y, double w, double h)
{
    setPos(x, y);
    setRect(0, 0, w, h);
    updatePath();
}

/*
void
Object::setObjectEndPoint1(QPointF point)
{
}
*/

/* . */
QPointF
Object::objectEndPoint2() const
{
    QLineF lyne = line();
    QPointF endPoint2(lyne.x2(), lyne.y2());
    return scenePos() + scale_and_rotate(endPoint2, scale(), rotation());
}

/* . */
void
Object::setObjectEndPoint2(const QPointF& endPt2)
{
    setObjectEndPoint2(endPt2.x(), endPt2.y());
}

/* . */
void
Object::setObjectEndPoint2(double x2, double y2)
{
    QPointF endPt1 = scenePos();
    double x1 = endPt1.x();
    double y1 = endPt1.y();
    double dx = x2 - x1;
    double dy = y2 - y1;
    setRotation(0);
    setScale(1);
    setLine(0, 0, dx, dy);
    setPos(x1, y1);
}

/* . */
void
Object::vulcanize()
{
    qDebug("vulcanize()");
    // FIXME: updateRubber(painter);

    setObjectRubberMode(OBJ_RUBBER_OFF);

    switch (geometry->type) {
    case EMB_POLYLINE:
        if (!normalPath.elementCount()) {
            QMessageBox::critical(0,
                QObject::tr("Empty Polyline Error"),
                QObject::tr("The polyline added contains no points. The command that created this object has flawed logic."));
        }
        break;
    case EMB_POLYGON:
        if (!normalPath.elementCount()) {
            QMessageBox::critical(0,
                QObject::tr("Empty Polygon Error"),
                QObject::tr("The polygon added contains no points. The command that created this object has flawed logic."));
        }
        break;
    case EMB_PATH:
        if (!normalPath.elementCount()) {
            QMessageBox::critical(0,
                QObject::tr("Empty Path Error"),
                QObject::tr("The path added contains no points. The command that created this object has flawed logic."));
        }
        break;
    default:
        break;
    }
}

QList<QPointF>
Object::allGripPoints()
{
    QList<QPointF> gripPoints;
    switch (geometry->type) {
    case EMB_ARC: {
        gripPoints << objectCenter()
            << objectStartPoint()
            << objectMidPoint()
            << objectEndPoint();
        break;
    }
    case EMB_CIRCLE:
    case EMB_ELLIPSE: {
        gripPoints << objectCenter()
            << objectQuadrant0()
            << objectQuadrant90()
            << objectQuadrant180()
            << objectQuadrant270();
        break;
    }
    case EMB_DIM_LEADER: {
        gripPoints << objectEndPoint1() << objectEndPoint2();
        if (curved) {
            gripPoints << objectMidPoint();
        }
        break;
    }
    case EMB_IMAGE: {
        gripPoints << objectTopLeft()
             << objectTopRight()
             << objectBottomLeft()
             << objectBottomRight();
        break;
    }
    case EMB_LINE: {
        gripPoints << objectEndPoint1() << objectEndPoint2() << objectMidPoint();
        break;
    }
    case EMB_PATH: {
        gripPoints << scenePos(); //TODO: loop thru all path Elements and return their points
        break;
    }
    case EMB_POLYGON:
    case EMB_POLYLINE: {
        QPainterPath::Element element;
        for (int i = 0; i < normalPath.elementCount(); ++i) {
            element = normalPath.elementAt(i);
            gripPoints << mapToScene(element.x, element.y);
        }
        break;
    }
    case EMB_TEXT_SINGLE:
    case EMB_POINT:
    default:
        gripPoints << scenePos();
        break;
    }
    return gripPoints;
}

/* Returns the closest snap point to the mouse point */
QPointF
Object::mouseSnapPoint(const QPointF& mousePoint)
{
    return find_mouse_snap_point(allGripPoints(), mousePoint);
}

void
Object::gripEdit(const QPointF& before, const QPointF& after)
{
    switch (geometry->type) {
    case EMB_ARC: {
        //TODO: gripEdit() for ArcObject
        break;
    }
    case EMB_CIRCLE: {
        if (before == objectCenter()) {
            QPointF delta = after-before;
            moveBy(delta.x(), delta.y());
        }
        else {
            setObjectRadius(QLineF(objectCenter(), after).length());
        }
        break;
    }
    case EMB_DIM_LEADER:
    case EMB_LINE: {
        if (before == objectEndPoint1()) {
            setObjectEndPoint1(after);
        }
        else if (before == objectEndPoint2()) {
            setObjectEndPoint2(after);
        }
        else if (before == objectMidPoint()) {
            QPointF delta = after-before;
            moveBy(delta.x(), delta.y());
        }
        break;
    }
    case EMB_ELLIPSE: {
        //TODO: gripEdit() for EllipseObject
        break;
    }
    case EMB_IMAGE:
    case EMB_RECT: {
        QPointF delta = after-before;
        if (before == objectTopLeft()) {
            setObjectRect(after.x(), after.y(), objectWidth()-delta.x(),
                objectHeight()-delta.y());
        }
        else if (before == objectTopRight()) {
            setObjectRect(objectTopLeft().x(), objectTopLeft().y()+delta.y(),
                objectWidth()+delta.x(), objectHeight()-delta.y());
        }
        else if (before == objectBottomLeft()) {
            setObjectRect(objectTopLeft().x()+delta.x(), objectTopLeft().y(),
                objectWidth()-delta.x(), objectHeight()+delta.y());
        }
        else if (before == objectBottomRight()) {
            setObjectRect(objectTopLeft().x(), objectTopLeft().y(),
                objectWidth()+delta.x(), objectHeight()+delta.y());
        }
        break;
    }
    case EMB_PATH: {
        //TODO: gripEdit() for PathObject
        break;
    }
    case EMB_POLYGON:
    case EMB_POLYLINE: {
        gripIndex = findIndex(before);
        if (gripIndex == -1) {
            return;
        }
        QPointF a = mapFromScene(after);
        normalPath.setElementPositionAt(gripIndex, a.x(), a.y());
        updatePath(normalPath);
        gripIndex = -1;
        break;
    }
    case EMB_TEXT_SINGLE:
    case EMB_POINT:
    default: {
        if (before == scenePos()) {
            QPointF delta = after-before;
            moveBy(delta.x(), delta.y());
        }
        break;
    }
    }
}

void
Object::paint(QPainter* painter, const QStyleOptionGraphicsItem *option, QWidget *)
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
        paintPen = lineWeightPen();
    }
    painter->setPen(paintPen);

    switch (geometry->type) {
    case EMB_ARC: {
        double startAngle = (objectStartAngle() + rotation())*16;
        double spanAngle = objectIncludedAngle()*16;

        if (objectClockwise()) {
            spanAngle = -spanAngle;
        }

        double rad = objectRadius();
        QRectF paintRect(-rad, -rad, rad*2.0, rad*2.0);
        painter->drawArc(paintRect, startAngle, spanAngle);
        break;
    }
    case EMB_CIRCLE:
    case EMB_ELLIPSE: {
        painter->drawEllipse(rect());
        break;
    }
    case EMB_DIM_LEADER: {
        painter->drawPath(lineStylePath);
        painter->drawPath(arrowStylePath);

        if (filled) {
            painter->fillPath(arrowStylePath, objPen.color());
        }
        break;
    }
    case EMB_LINE: {
        if (objectRubberMode() != OBJ_RUBBER_LINE) {
            painter->drawLine(line());
        }

        if (objScene->property("ENABLE_LWT").toBool()
            && objScene->property("ENABLE_REAL").toBool()) {
                realRender(painter, path());
        }
        break;
    }
    case EMB_IMAGE: {
        painter->drawRect(rect());
        break;
    }
    case EMB_RECT: {
        painter->drawRect(rect());
        break;
    }
    case EMB_PATH: {
        painter->drawPath(objectPath());
        break;
    }
    case EMB_POLYGON: {
        if (normalPath.elementCount()) {
            painter->drawPath(normalPath);
            QPainterPath::Element zero = normalPath.elementAt(0);
            QPainterPath::Element last = normalPath.elementAt(normalPath.elementCount()-1);
            painter->drawLine(QPointF(zero.x, zero.y), QPointF(last.x, last.y));
        }
        break;
    }
    case EMB_POLYLINE: {
        painter->drawPath(normalPath);

        if (objScene->property("ENABLE_LWT").toBool()
            && objScene->property("ENABLE_REAL").toBool()) {
            realRender(painter, normalPath);
        }
        break;
    }
    case EMB_TEXT_SINGLE: {
        painter->drawPath(objTextPath);
        break;
    }
    default:
    case EMB_POINT: {
        painter->drawPoint(0,0);
        break;
    }
    }
}

