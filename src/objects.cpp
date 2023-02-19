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

#include <math.h>

void embArc_setCenter(EmbArc *arc, EmbVector point);
void embArc_setRadius(EmbArc *arc, float radius);
void embArc_setStartAngle(EmbArc *arc, float angle);
void embArc_setEndAngle(EmbArc *arc, float angle);
float embArc_startAngle(EmbArc arc);
float embArc_endAngle(EmbArc arc);

/*
void arc_init(EmbArc arc_in, unsigned int rgb, PenStyle lineType)
{
    setData(OBJ_TYPE, type);
    setData(OBJ_NAME, "Arc");

    arc = arc_in;

    setFlag(QGraphicsItem::ItemIsSelectable, true);

    calculateArcData(arc);

    setObjectColor(rgb);
    setObjectLineType(lineType);
    setObjectLineWeight(0.35); //TODO: pass in proper lineweight
    setPen(objPen);
}

void calculate_arc_data(EmbArc arc)
{
    EmbVector center;
    getArcCenter(arc, &center);

    arcStartPoint = embVector_subtract(arc.start, center);
    arcMidPoint = embVector_subtract(arc.mid, center);
    arcEndPoint = embVector_subtract(arc.end, center);

    setPos(center);

    float radius = EmbLine(center, arc.mid).length();
    updateArcRect(radius);
    updatePath();
    setRotation(0);
    setScale(1);
}

void update_arc_rect(float radius)
{
    EmbRect arcRect;
    arcRect.setWidth(radius*2.0);
    arcRect.setHeight(radius*2.0);
    arcRect.moveCenter(EmbVector(0,0));
    setRect(arcRect);
}
*/

void embArc_setCenter(EmbArc *arc, EmbVector point)
{
    EmbVector delta;
    EmbVector old_center;
    getArcCenter(*arc, &old_center);
    embVector_subtract(point, old_center, &delta);
    embVector_add(arc->start, delta,  &(arc->start));
    embVector_add(arc->mid, delta,  &(arc->mid));
    embVector_add(arc->end, delta,  &(arc->end));
}

void embArc_setRadius(EmbArc *arc, float radius)
{
    EmbVector delta;
    float rad;
    if (radius <= 0.0f) {
        rad = 0.0000001f;
    }
    else {
        rad = radius;
    }

    EmbVector center;
    getArcCenter(*arc, &center);
    double delta_length;

    embVector_subtract(arc->start, center, &delta);
    delta_length = embVector_length(delta);
    embVector_multiply(delta, rad/delta_length, &delta);
    embVector_add(center, delta, &(arc->start));

    embVector_subtract(arc->mid, center, &delta);
    delta_length = embVector_length(delta);
    embVector_multiply(delta, rad/delta_length, &delta);
    embVector_add(center, delta, &(arc->mid));

    embVector_subtract(arc->end, center, &delta);
    delta_length = embVector_length(delta);
    embVector_multiply(delta, rad/delta_length, &delta);
    embVector_add(center, delta, &(arc->end));
}

void embArc_startAngle(EmbArc *arc, float angle)
{
    //TODO: ArcObject setObjectStartAngle
}

void embAarc_endAngle(EmbArc *arc, float angle)
{
    //TODO: ArcObject setObjectEndAngle
}

float embArc_startAngle(EmbArc arc)
{
    EmbVector delta;
    EmbVector center;
    getArcCenter(arc, &center);
    embVector_subtract(arc.end, center, &delta);
    float angle = embVector_angle(delta);
    return fmodf(angle, 360.0);
}

float embArc_endAngle(EmbArc arc)
{
    EmbVector delta;
    EmbVector center;
    getArcCenter(arc, &center);
    embVector_subtract(arc.end, center, &delta);
    float angle = embVector_angle(delta);
    return fmodf(angle, 360.0);
}

/*
float embArc_area(EmbArc arc)
{
    // Area of a circular segment
    float r = objectRadius();
    float theta = radians(objectIncludedAngle());
    return ((r*r)/2)*(theta - sin(theta));
}

float embArc_arcLength(EmbArc arc)
{
    return radians(objectIncludedAngle())*objectRadius();
}

float embArc_chord()
{
    return embVector_distance(arc.start, arc.end);
}

float Arc_IncludedAngle()
{
    float chord = objectChord();
    float rad = objectRadius();
    if (chord <= 0 || rad <= 0) return 0; //Prevents division by zero and non-existant circles

    //NOTE: Due to floating point rounding errors, we need to clamp the quotient so it is in the range [-1, 1]
    //      If the quotient is out of that range, then the result of asin() will be NaN.
    float quotient = chord/(2.0*rad);
    if (quotient > 1.0) quotient = 1.0;
    if (quotient < 0.0) quotient = 0.0; //NOTE: 0 rather than -1 since we are enforcing a positive chord and radius
    return degrees(2.0*asin(quotient)); //Properties of a Circle - Get the Included Angle - Reference: ASD9
}

bool Arc_Clockwise()
{
    // NOTE: Y values are inverted here on purpose
    EmbArc arc2 = arc;
    arc2.start.y *= -1.0;
    arc2.mid.y *= -1.0;
    arc2.end.y *= -1.0;

    return embArc_clockwise(arc2);
}

void Arc_updatePath()
{
    float startAngle = (objectStartAngle() + rotation());
    float spanAngle = objectIncludedAngle();

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
*/

#if 0
void Arc_paint(QPainter* painter, QStyleOptionGraphicsItem* option, QWidget* /*widget*/)
{
    QGraphicsScene* objScene = scene();
    if (!objScene) return;

    QPen paintPen = pen();
    painter->setPen(paintPen);
    updateRubber(painter);
    if (option->state & QStyle::State_Selected)  { paintPen.setStyle(DashLine); }
    if (objScene->property(ENABLE_LWT).toBool()) { paintPen = lineWeightPen(); }
    painter->setPen(paintPen);

    float startAngle = (objectStartAngle() + rotation())*16;
    float spanAngle = objectIncludedAngle()*16;

    if (objectClockwise())
        spanAngle = -spanAngle;

    float rad = objectRadius();
    EmbRect paintRect(-rad, -rad, rad*2.0, rad*2.0);
    painter->drawArc(paintRect, startAngle, spanAngle);
}

void Arc_updateRubber(QPainter* painter)
{
    //TODO: Arc Rubber Modes

    //TODO: updateRubber() gripping for ArcObject

}

void Arc_vulcanize()
{
    debug_message("ArcObject vulcanize()");
    updateRubber();

    setObjectRubberMode(OBJ_RUBBER_OFF);
}

// Returns the closest snap point to the mouse point
EmbVector Arc_mouseSnapPoint(EmbVector& mousePoint)
{
    EmbVector center = objectCenter();
    EmbVector start  = objectStartPoint();
    EmbVector mid    = objectMidPoint();
    EmbVector end    = objectEndPoint();

    float cntrDist  = EmbLine(mousePoint, center).length();
    float startDist = EmbLine(mousePoint, start).length();
    float midDist   = EmbLine(mousePoint, mid).length();
    float endDist   = EmbLine(mousePoint, end).length();

    float minDist = std::min(std::min(cntrDist, startDist), std::min(midDist, endDist));

    if     (minDist == cntrDist)  return center;
    else if (minDist == startDist) return start;
    else if (minDist == midDist)   return mid;
    else if (minDist == endDist)   return end;

    return scenePos();
}

std::vector<EmbVector> Arc_allGripPoints()
{
    std::vector<EmbVector> gripPoints;
    gripPoints << objectCenter() << objectStartPoint() << objectMidPoint() << objectEndPoint();
    return gripPoints;
}

void Arc_gripEdit(EmbVector& before, EmbVector& after)
{
    //TODO: gripEdit() for ArcObject
}

Base_BaseObject(QGraphicsItem* parent)
{
    debug_message("BaseObject Constructor()");

    objPen.setCapStyle(RoundCap);
    objPen.setJoinStyle(RoundJoin);
    lwtPen.setCapStyle(RoundCap);
    lwtPen.setJoinStyle(RoundJoin);

    objID = QDateTime::currentMSecsSinceEpoch();
}

void Base_setObjectColor(const QColor& color)
{
    objPen.setColor(color);
    lwtPen.setColor(color);
}

void Base_setObjectColorRGB(unsigned int rgb)
{
    objPen.setColor(QColor(rgb));
    lwtPen.setColor(QColor(rgb));
}

void Base_setObjectLineType(PenStyle lineType)
{
    objPen.setStyle(lineType);
    lwtPen.setStyle(lineType);
}

void Base_setObjectLineWeight(float lineWeight)
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
            QMessageBox::warning(0, translate("Error - Negative Lineweight"),
                                    translate("Lineweight: %1")
                                    .arg(std::string().setNum(lineWeight)));
            debug_message("Lineweight cannot be negative! Inverting sign.");
            lwtPen.setWidthF(-lineWeight);
        }
    }
    else {
        lwtPen.setWidthF(lineWeight);
    }
}

EmbVector Base_objectRubberPoint(const std::string& key) const
{
    if (objRubberPoints.contains(key)) {
        return objRubberPoints.value(key);
    }

    QGraphicsScene* gscene = scene();
    if (gscene) {
        return scene()->property("SCENE_QSNAP_POINT").toPointF();
    }
    return EmbVector();
}

std::string Base_objectRubberText(const std::string& key) const
{
    if (objRubberTexts.contains(key))
        return objRubberTexts.value(key);
    return std::string();
}

EmbRect Base_boundingRect()
{
    //If gripped, force this object to be drawn even if it is offscreen
    if (objectRubberMode() == OBJ_RUBBER_GRIP)
        return scene()->sceneRect();
    return path().boundingRect();
}

void Base_drawRubberLine(const EmbLine& rubLine, QPainter* painter, const char* colorFromScene)
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

void Base_realRender(QPainter* painter, const QPainterPath& renderPath)
{
    QColor color1 = objectColor();       //lighter color
    QColor color2  = color1.darker(150); //darker color

    //If we have a dark color, lighten it
    int darkness = color1.lightness();
    int threshold = 32; //TODO: This number may need adjusted or maybe just add it to settings.
    if (darkness < threshold)
    {
        color2 = color1;
        if (!darkness) { color1 = QColor(threshold, threshold, threshold); } //lighter() does not affect pure black
        else          { color1 = color2.lighter(100 + threshold); }
    }

    int count = renderPath.elementCount();
    for (int i = 0; i < count-1; ++i)
    {
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
        stroker.setCapStyle(RoundCap);
        stroker.setJoinStyle(RoundJoin);
        QPainterPath realPath = stroker.createStroke(elemPath);
        painter->drawPath(realPath);

        QLinearGradient grad(elemPath.pointAtPercent(0.5), elemPath.pointAtPercent(0.0));
        grad.setColorAt(0, color1);
        grad.setColorAt(1, color2);
        grad.setSpread(QGradient::ReflectSpread);

        painter->fillPath(realPath, QBrush(grad));
    }
}

void embCircle_main()
{
    initCommand();
    clearSelection();
    global.mode = global.mode_1P_RAD;
    global.x1 = NaN;
    global.y1 = NaN;
    global.x2 = NaN;
    global.y2 = NaN;
    global.x3 = NaN;
    global.y3 = NaN;
    setPromptPrefix(translate("Specify center point for circle or [3P/2P/Ttr (tan tan radius)]: "));
}

void embCircle_click(float x, float y)
{
    if (global.mode == global.mode_1P_RAD) {
        if (isnan(global.x1)) {
            global.x1 = x;
            global.y1 = y;
            global.cx = x;
            global.cy = y;
            addRubber("CIRCLE");
            setRubberMode("CIRCLE_1P_RAD");
            setRubberPoint("CIRCLE_CENTER", global.cx, global.cy);
            appendPromptHistory();
            setPromptPrefix(translate("Specify radius of circle or [Diameter]: "));
        }
        else {
            global.x2 = x;
            global.y2 = y;
            setRubberPoint("CIRCLE_RADIUS", global.x2, global.y2);
            vulcanize();
            appendPromptHistory();
            endCommand();
        }
    }
    else if (global.mode == CIRCLE_MODE_1P_DIA)
    {
        if (isnan(global.x1))
        {
            error("CIRCLE", translate("This should never happen."));
        }
        else
        {
            global.x2 = x;
            global.y2 = y;
            setRubberPoint("CIRCLE_DIAMETER", global.x2, global.y2);
            vulcanize();
            appendPromptHistory();
            endCommand();
        }
    }
    else if (global.mode == CIRCLE_MODE_2P) {
        if (isnan(global.x1)) {
            global.x1 = x;
            global.y1 = y;
            addRubber("CIRCLE");
            setRubberMode("CIRCLE_2P");
            setRubberPoint("CIRCLE_TAN1", global.x1, global.y1);
            appendPromptHistory();
            setPromptPrefix(translate("Specify second end point of circle's diameter: "));
        }
        else if (isnan(global.x2)) {
            global.x2 = x;
            global.y2 = y;
            setRubberPoint("CIRCLE_TAN2", global.x2, global.y2);
            vulcanize();
            appendPromptHistory();
            endCommand();
        }
        else {
            error("CIRCLE", translate("This should never happen."));
        }
    }
    else if (global.mode == CIRCLE_MODE_3P) {
        if (isnan(global.x1)) {
            global.x1 = x;
            global.y1 = y;
            appendPromptHistory();
            setPromptPrefix(translate("Specify second point on circle: "));
        }
        else if (isnan(global.x2)) {
            global.x2 = x;
            global.y2 = y;
            addRubber("CIRCLE");
            setRubberMode("CIRCLE_3P");
            setRubberPoint("CIRCLE_TAN1", global.x1, global.y1);
            setRubberPoint("CIRCLE_TAN2", global.x2, global.y2);
            appendPromptHistory();
            setPromptPrefix(translate("Specify third point on circle: "));
        }
        else if (isnan(global.x3)) {
            global.x3 = x;
            global.y3 = y;
            setRubberPoint("CIRCLE_TAN3", global.x3, global.y3);
            vulcanize();
            appendPromptHistory();
            endCommand();
        }
        else {
            error("CIRCLE", translate("This should never happen."));
        }
    }
    else if (global.mode == CIRCLE_MODE_TTR) {
        if (isnan(global.x1)) {
            global.x1 = x;
            global.y1 = y;
            appendPromptHistory();
            setPromptPrefix(translate("Specify point on object for second tangent of circle: "));
        }
        else if (isnan(global.x2)) {
            global.x2 = x;
            global.y2 = y;
            appendPromptHistory();
            setPromptPrefix(translate("Specify radius of circle: "));
        }
        else if (isnan(global.x3)) {
            global.x3 = x;
            global.y3 = y;
            appendPromptHistory();
            setPromptPrefix(translate("Specify second point: "));
        }
        else {
            todo("CIRCLE", "click() for TTR");
        }
    }
}

void embCircle_context(std::string str)
{
    todo("CIRCLE", "context()");
}

void embCircle_prompt(std::string str)
{
    if (global.mode == global.mode_1P_RAD) {
        if (isnan(global.x1)) {
            if (str == "2P") {
                //TODO: Probably should add additional qsTr calls here.
                global.mode = CIRCLE_MODE_2P;
                setPromptPrefix(translate("Specify first end point of circle's diameter: "));
            }
            else if (str == "3P") //TODO: Probably should add additional qsTr calls here.
            {
                global.mode = CIRCLE_MODE_3P;
                setPromptPrefix(translate("Specify first point of circle: "));
            }
            else if (str == "T" || str == "TTR") //TODO: Probably should add additional qsTr calls here.
            {
                global.mode = CIRCLE_MODE_TTR;
                setPromptPrefix(translate("Specify point on object for first tangent of circle: "));
            }
            else {
                var strList = str.split(",");
                if (isnan(strList[0]) || isnan(strList[1])) {
                    alert(translate("Point or option keyword required."));
                    setPromptPrefix(translate("Specify center point for circle or [3P/2P/Ttr (tan tan radius)]: "));
                }
                else {
                    global.x1 = Number(strList[0]);
                    global.y1 = Number(strList[1]);
                    global.cx = global.x1;
                    global.cy = global.y1;
                    addRubber("CIRCLE");
                    setRubberMode("CIRCLE_1P_RAD");
                    setRubberPoint("CIRCLE_CENTER", global.cx, global.cy);
                    setPromptPrefix(translate("Specify radius of circle or [Diameter]: "));
                }
            }
        }
        else {
            if (str == "D" || str == "DIAMETER") {
                //TODO: Probably should add additional translate calls here.
                global.mode = global.mode_1P_DIA;
                setRubberMode("CIRCLE_1P_DIA");
                setPromptPrefix(translate("Specify diameter of circle: "));
            }
            else {
                var num = Number(str);
                if (isnan(num)) {
                    alert(translate("Requires numeric radius, point on circumference, or \"D\"."));
                    setPromptPrefix(translate("Specify radius of circle or [Diameter]: "));
                }
                else {
                    global.rad = num;
                    global.x2 = global.x1 + global.rad;
                    global.y2 = global.y1;
                    setRubberPoint("CIRCLE_RADIUS", global.x2, global.y2);
                    vulcanize();
                    endCommand();
                }
            }
        }
    }
    else if (global.mode == global.mode_1P_DIA) {
        if (isnan(global.x1)) {
            error("CIRCLE", translate("This should never happen."));
        }
        if (isnan(global.x2)) {
            var num = Number(str);
            if (isnan(num)) {
                alert(translate("Requires numeric distance or second point."));
                setPromptPrefix(translate("Specify diameter of circle: "));
            }
            else {
                global.dia = num;
                global.x2 = global.x1 + global.dia;
                global.y2 = global.y1;
                setRubberPoint("CIRCLE_DIAMETER", global.x2, global.y2);
                vulcanize();
                endCommand();
            }
        }
        else {
            error("CIRCLE", translate("This should never happen."));
        }
    }
    else if (global.mode == global.mode_2P) {
        if (isnan(global.x1)) {
            var strList = str.split(",");
            if (isnan(strList[0]) || isnan(strList[1])) {
                alert(translate("Invalid point."));
                setPromptPrefix(translate("Specify first end point of circle's diameter: "));
            }
            else {
                global.x1 = Number(strList[0]);
                global.y1 = Number(strList[1]);
                addRubber("CIRCLE");
                setRubberMode("CIRCLE_2P");
                setRubberPoint("CIRCLE_TAN1", global.x1, global.y1);
                setPromptPrefix(translate("Specify second end point of circle's diameter: "));
            }
        }
        else if (isnan(global.x2)) {
            var strList = str.split(",");
            if (isnan(strList[0]) || isnan(strList[1])) {
                alert(translate("Invalid point."));
                setPromptPrefix(translate("Specify second end point of circle's diameter: "));
            }
            else {
                global.x2 = Number(strList[0]);
                global.y2 = Number(strList[1]);
                setRubberPoint("CIRCLE_TAN2", global.x2, global.y2);
                vulcanize();
                endCommand();
            }
        }
        else {
            error("CIRCLE", translate("This should never happen."));
        }
    }
    else if (global.mode == global.mode_3P) {
        if (isnan(global.x1)) {
            var strList = str.split(",");
            if (isnan(strList[0]) || isnan(strList[1])) {
                alert(translate("Invalid point."));
                setPromptPrefix(translate("Specify first point of circle: "));
            }
            else {
                global.x1 = Number(strList[0]);
                global.y1 = Number(strList[1]);
                setPromptPrefix(translate("Specify second point of circle: "));
            }
        }
        else if (isnan(global.x2)) {
            var strList = str.split(",");
            if (isnan(strList[0]) || isnan(strList[1])) {
                alert(translate("Invalid point."));
                setPromptPrefix(translate("Specify second point of circle: "));
            }
            else {
                global.x2 = Number(strList[0]);
                global.y2 = Number(strList[1]);
                addRubber("CIRCLE");
                setRubberMode("CIRCLE_3P");
                setRubberPoint("CIRCLE_TAN1", global.x1, global.y1);
                setRubberPoint("CIRCLE_TAN2", global.x2, global.y2);
                setPromptPrefix(translate("Specify third point of circle: "));
            }
        }
        else if (isnan(global.x3)) {
            var strList = str.split(",");
            if (isnan(strList[0]) || isnan(strList[1])) {
                alert(translate("Invalid point."));
                setPromptPrefix(translate("Specify third point of circle: "));
            }
            else {
                global.x3 = Number(strList[0]);
                global.y3 = Number(strList[1]);
                setRubberPoint("CIRCLE_TAN3", global.x3, global.y3);
                vulcanize();
                endCommand();
            }
        }
        else {
            error("CIRCLE", translate("This should never happen."));
        }

    }
    else if (global.mode == global.mode_TTR) {
        todo("CIRCLE", "prompt() for TTR");
    }
}

void embCircle_CircleObject(float centerX, float centerY, float radius, unsigned int rgb, QGraphicsItem* parent)
{
    debug_message("CircleObject Constructor()");
    init(centerX, centerY, radius, rgb, Qt::SolidLine); //TODO: getCurrentLineType
}

void embCircle_CircleObject(CircleObject* obj, QGraphicsItem* parent)
{
    debug_message("CircleObject Constructor()");
    if (obj) {
        init(obj->objectCenterX(), obj->objectCenterY(), obj->objectRadius(), obj->objectColorRGB(), Qt::SolidLine); //TODO: getCurrentLineType
        setRotation(obj->rotation());
    }
}

void embCircle_init(float centerX, float centerY, float radius, unsigned int rgb, Qt::PenStyle lineType)
{
    setData(OBJ_TYPE, type);
    setData(OBJ_NAME, "CIRCLE");

    //WARNING: DO NOT enable QGraphicsItem::ItemIsMovable. If it is enabled,
    //WARNING: and the item is float clicked, the scene will erratically move the item while zooming.
    //WARNING: All movement has to be handled explicitly by us, not by the scene.
    setFlag(QGraphicsItem::ItemIsSelectable, true);

    setObjectRadius(radius);
    setObjectCenter(centerX, centerY);
    setObjectColor(rgb);
    setObjectLineType(lineType);
    setObjectLineWeight(0.35); //TODO: pass in proper lineweight
    setPen(objectPen());
    updatePath();
}

void embCircle_setObjectCenter(EmbVector& center)
{
    setObjectCenter(center.x(), center.y());
}

void embCircle_setObjectCenter(float centerX, float centerY)
{
    setPos(centerX, centerY);
}

void embCircle_setObjectDiameter(EmbCircle *circle, float diameter)
{
    circle->radius = diameter*0.5;
    EmbRect circRect;
    circRect.setWidth(diameter);
    circRect.setHeight(diameter);
    circRect.moveCenter(EmbVector(0,0));
    setRect(circRect);
    updatePath();
}

void embCircle_setObjectArea(float area)
{
    float radius = sqrt(area/pi());
    setObjectRadius(radius);
}

void embCircle_setObjectCircumference(float circumference)
{
    float diameter = circumference/pi();
    setObjectDiameter(diameter);
}

void embCircle_updatePath()
{
    QPainterPath path;
    EmbRect r = rect();
    //Add the center point
    path.addRect(-0.00000001, -0.00000001, 0.00000002, 0.00000002);
    //Add the circle
    path.arcMoveTo(r, 0);
    path.arcTo(r, 0, 360);
    //NOTE: Reverse the path so that the inside area isn't considered part of the circle
    path.arcTo(r, 0, -360);
    setObjectPath(path);
}

void embCircle_paint(QPainter* painter, QStyleOptionGraphicsItem* option, QWidget* /*widget*/)
{
    QGraphicsScene* objScene = scene();
    if (!objScene) return;

    QPen paintPen = pen();
    painter->setPen(paintPen);
    updateRubber(painter);
    if (option->state & QStyle::State_Selected)  { paintPen.setStyle(Qt::DashLine); }
    if (objScene->property(ENABLE_LWT).toBool()) { paintPen = lineWeightPen(); }
    painter->setPen(paintPen);

    painter->drawEllipse(rect());
}

void embCircle_updateRubber(QPainter* painter)
{
    int rubberMode = objectRubberMode();
    if (rubberMode == OBJ_RUBBER_CIRCLE_1P_RAD)
    {
        EmbVector sceneCenterPoint = objectRubberPoint("CIRCLE_CENTER");
        EmbVector sceneQSnapPoint = objectRubberPoint("CIRCLE_RADIUS");
        EmbVector itemCenterPoint = mapFromScene(sceneCenterPoint);
        EmbVector itemQSnapPoint = mapFromScene(sceneQSnapPoint);
        EmbLine itemLine(itemCenterPoint, itemQSnapPoint);
        setObjectCenter(sceneCenterPoint);
        EmbLine sceneLine(sceneCenterPoint, sceneQSnapPoint);
        float radius = sceneLine.length();
        setObjectRadius(radius);
        if (painter) drawRubberLine(itemLine, painter, VIEW_COLOR_CROSSHAIR);
        updatePath();
    }
    else if (rubberMode == OBJ_RUBBER_CIRCLE_1P_DIA) {
        EmbVector sceneCenterPoint = objectRubberPoint("CIRCLE_CENTER");
        EmbVector sceneQSnapPoint = objectRubberPoint("CIRCLE_DIAMETER");
        EmbVector itemCenterPoint = mapFromScene(sceneCenterPoint);
        EmbVector itemQSnapPoint = mapFromScene(sceneQSnapPoint);
        EmbLine itemLine(itemCenterPoint, itemQSnapPoint);
        setObjectCenter(sceneCenterPoint);
        EmbLine sceneLine(sceneCenterPoint, sceneQSnapPoint);
        float diameter = sceneLine.length();
        setObjectDiameter(diameter);
        if (painter) drawRubberLine(itemLine, painter, VIEW_COLOR_CROSSHAIR);
        updatePath();
    }
    else if (rubberMode == OBJ_RUBBER_CIRCLE_2P)
    {
        EmbVector sceneTan1Point = objectRubberPoint("CIRCLE_TAN1");
        EmbVector sceneQSnapPoint = objectRubberPoint("CIRCLE_TAN2");
        EmbLine sceneLine(sceneTan1Point, sceneQSnapPoint);
        setObjectCenter(sceneLine.pointAt(0.5));
        float diameter = sceneLine.length();
        setObjectDiameter(diameter);
        updatePath();
    }
    else if (rubberMode == OBJ_RUBBER_CIRCLE_3P)
    {
        EmbVector sceneTan1Point = objectRubberPoint("CIRCLE_TAN1");
        EmbVector sceneTan2Point = objectRubberPoint("CIRCLE_TAN2");
        EmbVector sceneTan3Point = objectRubberPoint("CIRCLE_TAN3");

        float sceneCenterX;
        float sceneCenterY;
        EmbArc arc;
        EmbVector sceneCenter;
        arc.start.x = sceneTan1Point.x();
        arc.start.y = sceneTan1Point.y();
        arc.mid.x = sceneTan2Point.x();
        arc.mid.y = sceneTan2Point.y();
        arc.end.x = sceneTan3Point.x();
        arc.end.y = sceneTan3Point.y();
        getArcCenter(arc, &sceneCenter);
        EmbVector sceneCenterPoint(sceneCenter.x, sceneCenter.y);
        EmbLine sceneLine(sceneCenterPoint, sceneTan3Point);
        setObjectCenter(sceneCenterPoint);
        float radius = sceneLine.length();
        setObjectRadius(radius);
        updatePath();
    }
    else if (rubberMode == OBJ_RUBBER_GRIP)
    {
        if (painter)
        {
            EmbVector gripPoint = objectRubberPoint("GRIP_POINT");
            if (gripPoint == objectCenter())
            {
                painter->drawEllipse(rect().translated(mapFromScene(objectRubberPoint(std::string()))-mapFromScene(gripPoint)));
            }
            else
            {
                float gripRadius = EmbLine(objectCenter(), objectRubberPoint(std::string())).length();
                painter->drawEllipse(EmbVector(), gripRadius, gripRadius);
            }

            EmbLine rubLine(mapFromScene(gripPoint), mapFromScene(objectRubberPoint(std::string())));
            drawRubberLine(rubLine, painter, VIEW_COLOR_CROSSHAIR);
        }
    }
}

void embCircle_vulcanize()
{
    debug_message("CircleObject vulcanize()");
    updateRubber();

    setObjectRubberMode(OBJ_RUBBER_OFF);
}

// Returns the closest snap point to the mouse point
EmbVector Circle_mouseSnapPoint(EmbVector& mousePoint)
{
    EmbVector center  = objectCenter();
    EmbVector quad0   = objectQuadrant0();
    EmbVector quad90  = objectQuadrant90();
    EmbVector quad180 = objectQuadrant180();
    EmbVector quad270 = objectQuadrant270();

    float cntrDist = EmbLine(mousePoint, center).length();
    float q0Dist   = EmbLine(mousePoint, quad0).length();
    float q90Dist  = EmbLine(mousePoint, quad90).length();
    float q180Dist = EmbLine(mousePoint, quad180).length();
    float q270Dist = EmbLine(mousePoint, quad270).length();

    float minDist = std::min(std::min(std::min(q0Dist, q90Dist), std::min(q180Dist, q270Dist)), cntrDist);

    if     (minDist == cntrDist) return center;
    else if (minDist == q0Dist)   return quad0;
    else if (minDist == q90Dist)  return quad90;
    else if (minDist == q180Dist) return quad180;
    else if (minDist == q270Dist) return quad270;

    return scenePos();
}

std::vector<EmbVector> Circle_allGripPoints()
{
    std::vector<EmbVector> gripPoints;
    gripPoints << objectCenter() << objectQuadrant0() << objectQuadrant90() << objectQuadrant180() << objectQuadrant270();
    return gripPoints;
}

void embCircle_gripEdit(EmbVector& before, EmbVector& after)
{
    if (before == objectCenter()) {
        EmbVector delta = after-before;
        moveBy(delta.x(), delta.y());
    }
    else {
        setObjectRadius(EmbLine(objectCenter(), after).length());
    }
}

QPainterPath Circle_objectSavePath()
{
    QPainterPath path;
    EmbRect r = rect();
    path.arcMoveTo(r, 0);
    path.arcTo(r, 0, 360);

    float s = scale();
    QTransform trans;
    trans.rotate(rotation());
    trans.scale(s,s);
    return trans.map(path);
}

void dim_leader_init(EmbLine line, unsigned int rgb, Qt::PenStyle lineType)
{
    setData(OBJ_TYPE, type);
    setData(OBJ_NAME, "Leader Dimension");

    //WARNING: DO NOT enable QGraphicsItem::ItemIsMovable. If it is enabled,
    //WARNING: and the item is float clicked, the scene will erratically move the item while zooming.
    //WARNING: All movement has to be handled explicitly by us, not by the scene.
    setFlag(QGraphicsItem::ItemIsSelectable, true);

    curved = false;
    filled = true;
    setObjectEndPoint1(x1, y1);
    setObjectEndPoint2(x2, y2);
    setObjectColor(rgb);
    setObjectLineType(lineType);
    setObjectLineWeight(0.35); //TODO: pass in proper lineweight
    setPen(objPen);
}

void dimleader_setObjectEndPoint1(EmbVector endPt1)
{
    EmbVector delta;
    EmbVector endPt2 = objectEndPoint2();
    delta.x = endPt2.x - endPt1.x;
    delta.y = endPt2.y - endPt1.y;
    setRotation(0);
    setLine(0, 0, dx, dy);
    setPos(endPt1);
    updateLeader();
}

void dimleader_setObjectEndPoint2(EmbVector endPt2)
{
    EmbVector delta;
    EmbVector endPt1 = scenePos();
    delta.x = endPt2.x - endPt1.x;
    delta.y = endPt2.y - endPt1.y;
    setRotation(0);
    setLine(0, 0, delta.x, delta.y);
    setPos(endPt1);
    updateLeader();
}

EmbVector dimleader_objectEndPoint1()
{
    return scenePos();
}

EmbVector dimleader_objectEndPoint2()
{
    EmbLine lyne = line();
    float rot = radians(rotation());
    EmbVector point2;
    point2.x = lyne.x2()*scale();
    point2.y = lyne.y2()*scale();
    EmbVector rot = embVector_rotate(point2, alpha);

    return (scenePos() + rot);
}

EmbVector dimleader_objectMidPoint()
{
    EmbVector mp = line().pointAt(0.5) * scale();
    float alpha = radians(rotation());
    EmbVector rotMid = embVector_rotate(mp, alpha);
    return scenePos() + rotMid;
}

float dimleader_objectAngle()
{
    return fmodf(line().angle() - rotation(), 360.0);
}

void dimleader_updateLeader()
{
    int arrowStyle = Closed; //TODO: Make this customizable
    float arrowStyleAngle = 15.0; //TODO: Make this customizable
    float arrowStyleLength = 1.0; //TODO: Make this customizable
    float lineStyleAngle = 45.0; //TODO: Make this customizable
    float lineStyleLength = 1.0; //TODO: Make this customizable

    EmbLine lyne = line();
    float angle = lyne.angle();
    EmbVector ap0 = lyne.p1();
    EmbVector lp0 = lyne.p2();

    //Arrow
    EmbLine lynePerp(lyne.pointAt(arrowStyleLength/lyne.length()) ,lp0);
    lynePerp.setAngle(angle + 90);
    EmbLine lyne1(ap0, lp0);
    EmbLine lyne2(ap0, lp0);
    lyne1.setAngle(angle + arrowStyleAngle);
    lyne2.setAngle(angle - arrowStyleAngle);
    EmbVector ap1;
    EmbVector ap2;
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
        float side = EmbLine(ap1, ap2).length();
        EmbRect ar0(0, 0, side, side);
        ar0.moveCenter(ap0);
        arrowStylePath.addRect(ar0);
    }
    else if (arrowStyle == Tick) {
    }

    lineStylePath = QPainterPath();
    lineStylePath.moveTo(ap0);
    lineStylePath.lineTo(lp0);
}

void dimleader_paint(QPainter* painter, const QStyleOptionGraphicsItem* option, QWidget* /*widget*/)
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

    painter->drawPath(lineStylePath);
    painter->drawPath(arrowStylePath);

    if (filled) {
        painter->fillPath(arrowStylePath, objectColor());
    }
}

void dimleader_updateRubber(QPainter* painter)
{
    int rubberMode = objectRubberMode();
    if (rubberMode == OBJ_RUBBER_DIMLEADER_LINE) {
        EmbVector sceneStartPoint = objectRubberPoint("DIMLEADER_LINE_START");
        EmbVector sceneQSnapPoint = objectRubberPoint("DIMLEADER_LINE_END");

        setObjectEndPoint1(sceneStartPoint);
        setObjectEndPoint2(sceneQSnapPoint);
    }
    else if (rubberMode == OBJ_RUBBER_GRIP) {
        if (painter) {
            EmbVector gripPoint = objectRubberPoint("GRIP_POINT");
            if (gripPoint == objectEndPoint1())
                painter->drawLine(line().p2(), mapFromScene(objectRubberPoint(std::string())));
            else if (gripPoint == objectEndPoint2())
                painter->drawLine(line().p1(), mapFromScene(objectRubberPoint(std::string())));
            else if (gripPoint == objectMidPoint())
                painter->drawLine(line().translated(mapFromScene(objectRubberPoint(std::string()))-mapFromScene(gripPoint)));
        }
    }
}

void dimleader_vulcanize()
{
    debug_message("DimLeaderObject vulcanize()");
    updateRubber();

    setObjectRubberMode(OBJ_RUBBER_OFF);
}

// Returns the closest snap point to the mouse point
EmbVector dimleader_mouseSnapPoint(const EmbVector& mousePoint)
{
    EmbVector endPoint1 = objectEndPoint1();
    EmbVector endPoint2 = objectEndPoint2();
    EmbVector midPoint  = objectMidPoint();

    float end1Dist = EmbLine(mousePoint, endPoint1).length();
    float end2Dist = EmbLine(mousePoint, endPoint2).length();
    float midDist  = EmbLine(mousePoint, midPoint).length();

    float minDist = std::min(end1Dist, end2Dist);

    if (curved)
        minDist = std::min(minDist, midDist);

    if     (minDist == end1Dist) return endPoint1;
    else if (minDist == end2Dist) return endPoint2;
    else if (minDist == midDist)  return midPoint;

    return scenePos();
}

std::vector<EmbVector> DimLeaderObject_allGripPoints()
{
    std::vector<EmbVector> gripPoints;
    gripPoints << objectEndPoint1() << objectEndPoint2();
    if (curved) {
        gripPoints << objectMidPoint();
    }
    return gripPoints;
}

void dimleader_gripEdit(const EmbVector& before, const EmbVector& after)
{
    if (before == objectEndPoint1()) {
        setObjectEndPoint1(after);
    }
    else if (before == objectEndPoint2()) {
        setObjectEndPoint2(after);
    }
    else if (before == objectMidPoint()) {
        EmbVector delta = embVector_subtract(after, before);
        moveBy(delta);
    }
}

//NOTE: main() is run every time the command is started.
//      Use it to reset variables so they are ready to go.
void embEllipse_main()
{
    initCommand();
    clearSelection();
    global.mode = global.mode_MAJORDIAMETER_MINORRADIUS;
    global.x1      = NaN;
    global.y1      = NaN;
    global.x2      = NaN;
    global.y2      = NaN;
    global.x3      = NaN;
    global.y3      = NaN;
    setPromptPrefix(translate("Specify first axis start point or [Center]: "));
}

//NOTE: click() is run only for left clicks.
//      Middle clicks are used for panning.
//      Right clicks bring up the context menu.
void click(float x, float y)
{
    if (global.mode == global.mode_MAJORDIAMETER_MINORRADIUS) {
        if (isnan(global.x1)) {
            global.x1 = x;
            global.y1 = y;
            addRubber("ELLIPSE");
            setRubberMode("ELLIPSE_LINE");
            setRubberPoint("ELLIPSE_LINE_POINT1", global.x1, global.y1);
            appendPromptHistory();
            setPromptPrefix(translate("Specify first axis end point: "));
        }
        else if (isnan(global.x2)) {
            global.x2 = x;
            global.y2 = y;
            global.cx = (global.x1 + global.x2)/2.0;
            global.cy = (global.y1 + global.y2)/2.0;
            global.width = calculateDistance(global.x1, global.y1, global.x2, global.y2);
            global.rot = calculateAngle(global.x1, global.y1, global.x2, global.y2);
            setRubberMode("ELLIPSE_MAJORDIAMETER_MINORRADIUS");
            setRubberPoint("ELLIPSE_AXIS1_POINT1", global.x1, global.y1);
            setRubberPoint("ELLIPSE_AXIS1_POINT2", global.x2, global.y2);
            setRubberPoint("ELLIPSE_CENTER", global.cx, global.cy);
            setRubberPoint("ELLIPSE_WIDTH", global.width, 0);
            setRubberPoint("ELLIPSE_ROT", global.rot, 0);
            appendPromptHistory();
            setPromptPrefix(translate("Specify second axis end point or [Rotation]: "));
        }
        else if (isnan(global.x3)) {
            global.x3 = x;
            global.y3 = y;
            global.height = perpendicularDistance(global.x3, global.y3, global.x1, global.y1, global.x2, global.y2)*2.0;
            setRubberPoint("ELLIPSE_AXIS2_POINT2", global.x3, global.y3);
            vulcanize();
            appendPromptHistory();
            endCommand();
        }
        else
        {
            error("ELLIPSE", translate("This should never happen."));
        }
    }
    else if (global.mode == global.mode_MAJORRADIUS_MINORRADIUS)
    {
        if (isnan(global.x1))
        {
            global.x1 = x;
            global.y1 = y;
            global.cx = global.x1;
            global.cy = global.y1;
            addRubber("ELLIPSE");
            setRubberMode("ELLIPSE_LINE");
            setRubberPoint("ELLIPSE_LINE_POINT1", global.x1, global.y1);
            setRubberPoint("ELLIPSE_CENTER", global.cx, global.cy);
            appendPromptHistory();
            setPromptPrefix(translate("Specify first axis end point: "));
        }
        else if (isnan(global.x2)) {
            global.x2 = x;
            global.y2 = y;
            global.width = calculateDistance(global.cx, global.cy, global.x2, global.y2)*2.0;
            global.rot = calculateAngle(global.x1, global.y1, global.x2, global.y2);
            setRubberMode("ELLIPSE_MAJORRADIUS_MINORRADIUS");
            setRubberPoint("ELLIPSE_AXIS1_POINT2", global.x2, global.y2);
            setRubberPoint("ELLIPSE_WIDTH", global.width, 0);
            setRubberPoint("ELLIPSE_ROT", global.rot, 0);
            appendPromptHistory();
            setPromptPrefix(translate("Specify second axis end point or [Rotation]: "));
        }
        else if (isnan(global.x3)) {
            global.x3 = x;
            global.y3 = y;
            global.height = perpendicularDistance(global.x3, global.y3, global.cx, global.cy, global.x2, global.y2)*2.0;
            setRubberPoint("ELLIPSE_AXIS2_POINT2", global.x3, global.y3);
            vulcanize();
            appendPromptHistory();
            endCommand();
        }
        else {
            error("ELLIPSE", translate("This should never happen."));
        }
    }
    else if (global.mode == global.mode_ELLIPSE_ROTATION) {
        if (isnan(global.x1)) {
            error("ELLIPSE", translate("This should never happen."));
        }
        else if (isnan(global.x2)) {
            error("ELLIPSE", translate("This should never happen."));
        }
        else if (isnan(global.x3)) {
            var angle = calculateAngle(global.cx, global.cy, x, y);
            global.height = cos(angle*Math.PI/180.0)*global.width;
            addEllipse(global.cx, global.cy, global.width, global.height, global.rot, false);
            appendPromptHistory();
            endCommand();
        }
    }
}

void context(std::string str)
{
    todo("ELLIPSE", "context()");
}

void prompt(std::string str)
{
    if (mode == MAJORDIAMETER_MINORRADIUS) {
        if (isnan(global.x1))
        {
            if (str == "C" || str == "CENTER") //TODO: Probably should add additional qsTr calls here.
            {
                global.mode = global.mode_MAJORRADIUS_MINORRADIUS;
                setPromptPrefix(translate("Specify center point: "));
            }
            else
            {
                var strList = str.split(",");
                if (isnan(strList[0]) || isnan(strList[1]))
                {
                    alert(translate("Point or option keyword required."));
                    setPromptPrefix(translate("Specify first axis start point or [Center]: "));
                }
                else
                {
                    global.x1 = Number(strList[0]);
                    global.y1 = Number(strList[1]);
                    addRubber("ELLIPSE");
                    setRubberMode("ELLIPSE_LINE");
                    setRubberPoint("ELLIPSE_LINE_POINT1", global.x1, global.y1);
                    setPromptPrefix(translate("Specify first axis end point: "));
                }
            }
        }
        else if (isnan(global.x2)) {
            var strList = str.split(",");
            if (isnan(strList[0]) || isnan(strList[1])) {
                alert(translate("Invalid point."));
                setPromptPrefix(translate("Specify first axis end point: "));
            }
            else {
                global.x2 = Number(strList[0]);
                global.y2 = Number(strList[1]);
                global.cx = (global.x1 + global.x2)/2.0;
                global.cy = (global.y1 + global.y2)/2.0;
                global.width = calculateDistance(global.x1, global.y1, global.x2, global.y2);
                global.rot = calculateAngle(global.x1, global.y1, global.x2, global.y2);
                setRubberMode("ELLIPSE_MAJORDIAMETER_MINORRADIUS");
                setRubberPoint("ELLIPSE_AXIS1_POINT1", global.x1, global.y1);
                setRubberPoint("ELLIPSE_AXIS1_POINT2", global.x2, global.y2);
                setRubberPoint("ELLIPSE_CENTER", global.cx, global.cy);
                setRubberPoint("ELLIPSE_WIDTH", global.width, 0);
                setRubberPoint("ELLIPSE_ROT", global.rot, 0);
                setPromptPrefix(translate("Specify second axis end point or [Rotation]: "));
            }
        }
        else if (isnan(global.x3)) {
            if (str == "R" || str == "ROTATION") {//TODO: Probably should add additional qsTr calls here.
                global.mode = global.mode_ELLIPSE_ROTATION;
                setPromptPrefix(translate("Specify rotation: "));
            }
            else {
                var strList = str.split(",");
                if (isnan(strList[0]) || isnan(strList[1])) {
                    alert(translate("Point or option keyword required."));
                    setPromptPrefix(translate("Specify second axis end point or [Rotation]: "));
                }
                else {
                    global.x3 = Number(strList[0]);
                    global.y3 = Number(strList[1]);
                    global.height = perpendicularDistance(global.x3, global.y3, global.x1, global.y1, global.x2, global.y2)*2.0;
                    setRubberPoint("ELLIPSE_AXIS2_POINT2", global.x3, global.y3);
                    vulcanize();
                    endCommand();
                }
            }
        }
    }
    else if (mode == MAJORRADIUS_MINORRADIUS) {
        if (isnan(global.x1)) {
            var strList = str.split(",");
            if (isnan(strList[0]) || isnan(strList[1])) {
                alert(translate("Invalid point."));
                setPromptPrefix(translate("Specify center point: "));
            }
            else {
                global.x1 = Number(strList[0]);
                global.y1 = Number(strList[1]);
                global.cx = global.x1;
                global.cy = global.y1;
                addRubber("ELLIPSE");
                setRubberMode("ELLIPSE_LINE");
                setRubberPoint("ELLIPSE_LINE_POINT1", global.x1, global.y1);
                setRubberPoint("ELLIPSE_CENTER", global.cx, global.cy);
                setPromptPrefix(translate("Specify first axis end point: "));
            }
        }
        else if (isnan(global.x2))
        {
            var strList = str.split(",");
            if (isnan(strList[0]) || isnan(strList[1]))
            {
                alert(translate("Invalid point."));
                setPromptPrefix(translate("Specify first axis end point: "));
            }
            else
            {
                global.x2 = Number(strList[0]);
                global.y2 = Number(strList[1]);
                global.width = calculateDistance(global.x1, global.y1, global.x2, global.y2)*2.0;
                global.rot = calculateAngle(global.x1, global.y1, global.x2, global.y2);
                setRubberMode("ELLIPSE_MAJORRADIUS_MINORRADIUS");
                setRubberPoint("ELLIPSE_AXIS1_POINT2", global.x2, global.y2);
                setRubberPoint("ELLIPSE_WIDTH", global.width, 0);
                setRubberPoint("ELLIPSE_ROT", global.rot, 0);
                setPromptPrefix(translate("Specify second axis end point or [Rotation]: "));
            }
        }
        else if (isnan(global.x3))
        {
            if (str == "R" || str == "ROTATION") //TODO: Probably should add additional qsTr calls here.
            {
                global.mode = global.mode_ELLIPSE_ROTATION;
                setPromptPrefix(translate("Specify ellipse rotation: "));
            }
            else
            {
                var strList = str.split(",");
                if (isnan(strList[0]) || isnan(strList[1]))
                {
                    alert(translate("Point or option keyword required."));
                    setPromptPrefix(translate("Specify second axis end point or [Rotation]: "));
                }
                else {
                    global.x3 = Number(strList[0]);
                    global.y3 = Number(strList[1]);
                    global.height = perpendicularDistance(global.x3, global.y3, global.x1, global.y1, global.x2, global.y2)*2.0;
                    setRubberPoint("ELLIPSE_AXIS2_POINT2", global.x3, global.y3);
                    vulcanize();
                    endCommand();
                }
            }
        }
    }
    else if (global.mode == global.mode_ELLIPSE_ROTATION) {
        if (isnan(global.x1)) {
            error("ELLIPSE", translate("This should never happen."));
        }
        else if (isnan(global.x2)) {
            error("ELLIPSE", translate("This should never happen."));
        }
        else if (isnan(global.x3)) {
            if (isnan(str)) {
                alert(translate("Invalid angle. Input a numeric angle or pick a point."));
                setPromptPrefix(translate("Specify rotation: "));
            }
            else {
                var angle = Number(str);
                global.height = cos(angle*Math.PI/180.0)*global.width;
                addEllipse(global.cx, global.cy, global.width, global.height, global.rot, false);
                endCommand();
            }
        }
    }
}

void embEllipse_EllipseObject(float centerX, float centerY, float width, float height, unsigned int rgb, QGraphicsItem* parent)
{
    debug_message("EllipseObject Constructor()");
    init(centerX, centerY, width, height, rgb, Qt::SolidLine); //TODO: getCurrentLineType
}

void embEllipse_EllipseObject(EllipseObject* obj, QGraphicsItem* parent)
{
    debug_message("EllipseObject Constructor()");
    if (obj) {
        init(obj->objectCenterX(), obj->objectCenterY(), obj->objectWidth(), obj->objectHeight(), obj->objectColorRGB(), Qt::SolidLine); //TODO: getCurrentLineType
        setRotation(obj->rotation());
    }
}

void embEllipse_init(float centerX, float centerY, float width, float height, unsigned int rgb, Qt::PenStyle lineType)
{
    setData(OBJ_TYPE, type);
    setData(OBJ_NAME, "Ellipse");

    //WARNING: DO NOT enable QGraphicsItem::ItemIsMovable. If it is enabled,
    //WARNING: and the item is float clicked, the scene will erratically move the item while zooming.
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

void embEllipse_setObjectSize(float width, float height)
{
    EmbRect elRect = rect();
    elRect.setWidth(width);
    elRect.setHeight(height);
    elRect.moveCenter(EmbVector(0,0));
    setRect(elRect);
}

void embEllipse_setObjectCenter(const EmbVector& center)
{
    setObjectCenter(center.x(), center.y());
}

void embEllipse_setObjectCenter(float centerX, float centerY)
{
    setPos(centerX, centerY);
}

void embEllipse_setObjectCenterX(float centerX)
{
    setX(centerX);
}

void embEllipse_setObjectCenterY(float centerY)
{
    setY(centerY);
}

void embEllipse_setObjectRadiusMajor(float radius)
{
    setObjectDiameterMajor(radius*2.0);
}

void embEllipse_setObjectRadiusMinor(float radius)
{
    setObjectDiameterMinor(radius*2.0);
}

void embEllipse_setObjectDiameterMajor(float diameter)
{
    EmbRect elRect = rect();
    if (elRect.width() > elRect.height())
        elRect.setWidth(diameter);
    else
        elRect.setHeight(diameter);
    elRect.moveCenter(EmbVector(0,0));
    setRect(elRect);
}

void embEllipse_setObjectDiameterMinor(float diameter)
{
    EmbRect elRect = rect();
    if (elRect.width() < elRect.height())
        elRect.setWidth(diameter);
    else
        elRect.setHeight(diameter);
    elRect.moveCenter(EmbVector(0,0));
    setRect(elRect);
}

EmbVector ellipse_objectQuadrant0()
{
    float halfW = objectWidth()/2.0;
    float rot = radians(rotation());
    float x = halfW*cos(rot);
    float y = halfW*sin(rot);
    return objectCenter() + EmbVector(x,y);
}

EmbVector ellipse_objectQuadrant90()
{
    float halfH = objectHeight()/2.0;
    float rot = radians(rotation()+90.0);
    float x = halfH * cos(rot);
    float y = halfH * sin(rot);
    return objectCenter() + EmbVector(x,y);
}

EmbVector ellipse_objectQuadrant180()
{
    float halfW = objectWidth()/2.0;
    float rot = radians(rotation()+180.0);
    float x = halfW*cos(rot);
    float y = halfW*sin(rot);
    return objectCenter() + EmbVector(x,y);
}

EmbVector ellipse_objectQuadrant270()
{
    float halfH = objectHeight()/2.0;
    float rot = radians(rotation()+270.0);
    float x = halfH*cos(rot);
    float y = halfH*sin(rot);
    return objectCenter() + EmbVector(x,y);
}

void embEllipse_updatePath()
{
    QPainterPath path;
    EmbRect r = rect();
    path.arcMoveTo(r, 0);
    path.arcTo(r, 0, 360);
    //NOTE: Reverse the path so that the inside area isn't considered part of the ellipse
    path.arcTo(r, 0, -360);
    setObjectPath(path);
}

void embEllipse_paint(QPainter* painter, const QStyleOptionGraphicsItem* option, QWidget* /*widget*/)
{
    QGraphicsScene* objScene = scene();
    if (!objScene) return;

    QPen paintPen = pen();
    painter->setPen(paintPen);
    updateRubber(painter);
    if (option->state & QStyle::State_Selected)  { paintPen.setStyle(Qt::DashLine); }
    if (objScene->property(ENABLE_LWT).toBool()) { paintPen = lineWeightPen(); }
    painter->setPen(paintPen);

    painter->drawEllipse(rect());
}

void embEllipse_updateRubber(QPainter* painter)
{
    int rubberMode = objectRubberMode();
    if (rubberMode == OBJ_RUBBER_ELLIPSE_LINE)
    {
        EmbVector sceneLinePoint1 = objectRubberPoint("ELLIPSE_LINE_POINT1");
        EmbVector sceneLinePoint2 = objectRubberPoint("ELLIPSE_LINE_POINT2");
        EmbVector itemLinePoint1  = mapFromScene(sceneLinePoint1);
        EmbVector itemLinePoint2  = mapFromScene(sceneLinePoint2);
        EmbLine itemLine(itemLinePoint1, itemLinePoint2);
        if (painter) drawRubberLine(itemLine, painter, VIEW_COLOR_CROSSHAIR);
        updatePath();
    }
    else if (rubberMode == OBJ_RUBBER_ELLIPSE_MAJORDIAMETER_MINORRADIUS)
    {
        EmbVector sceneAxis1Point1 = objectRubberPoint("ELLIPSE_AXIS1_POINT1");
        EmbVector sceneAxis1Point2 = objectRubberPoint("ELLIPSE_AXIS1_POINT2");
        EmbVector sceneCenterPoint = objectRubberPoint("ELLIPSE_CENTER");
        EmbVector sceneAxis2Point2 = objectRubberPoint("ELLIPSE_AXIS2_POINT2");
        float ellipseWidth = objectRubberPoint("ELLIPSE_WIDTH").x();
        float ellipseRot = objectRubberPoint("ELLIPSE_ROT").x();

        //TODO: incorporate perpendicularDistance() into libcgeometry
        float px = sceneAxis2Point2.x();
        float py = sceneAxis2Point2.y();
        float x1 = sceneAxis1Point1.x();
        float y1 = sceneAxis1Point1.y();
        EmbLine line(sceneAxis1Point1, sceneAxis1Point2);
        EmbLine norm = line.normalVector();
        float dx = px-x1;
        float dy = py-y1;
        norm.translate(dx, dy);
        EmbVector iPoint;
        norm.intersects(line, &iPoint);
        float ellipseHeight = EmbLine(px, py, iPoint.x(), iPoint.y()).length()*2.0;

        setObjectCenter(sceneCenterPoint);
        setObjectSize(ellipseWidth, ellipseHeight);
        setRotation(-ellipseRot);

        EmbVector itemCenterPoint = mapFromScene(sceneCenterPoint);
        EmbVector itemAxis2Point2 = mapFromScene(sceneAxis2Point2);
        EmbLine itemLine(itemCenterPoint, itemAxis2Point2);
        if (painter) drawRubberLine(itemLine, painter, VIEW_COLOR_CROSSHAIR);
        updatePath();
    }
    else if (rubberMode == OBJ_RUBBER_ELLIPSE_MAJORRADIUS_MINORRADIUS)
    {
        EmbVector sceneAxis1Point2 = objectRubberPoint("ELLIPSE_AXIS1_POINT2");
        EmbVector sceneCenterPoint = objectRubberPoint("ELLIPSE_CENTER");
        EmbVector sceneAxis2Point2 = objectRubberPoint("ELLIPSE_AXIS2_POINT2");
        float ellipseWidth = objectRubberPoint("ELLIPSE_WIDTH").x();
        float ellipseRot = objectRubberPoint("ELLIPSE_ROT").x();

        //TODO: incorporate perpendicularDistance() into libcgeometry
        float px = sceneAxis2Point2.x();
        float py = sceneAxis2Point2.y();
        float x1 = sceneCenterPoint.x();
        float y1 = sceneCenterPoint.y();
        EmbLine line(sceneCenterPoint, sceneAxis1Point2);
        EmbLine norm = line.normalVector();
        float dx = px-x1;
        float dy = py-y1;
        norm.translate(dx, dy);
        EmbVector iPoint;
        norm.intersects(line, &iPoint);
        float ellipseHeight = EmbLine(px, py, iPoint.x(), iPoint.y()).length()*2.0;

        setObjectCenter(sceneCenterPoint);
        setObjectSize(ellipseWidth, ellipseHeight);
        setRotation(-ellipseRot);

        EmbVector itemCenterPoint = mapFromScene(sceneCenterPoint);
        EmbVector itemAxis2Point2 = mapFromScene(sceneAxis2Point2);
        EmbLine itemLine(itemCenterPoint, itemAxis2Point2);
        if (painter) drawRubberLine(itemLine, painter, VIEW_COLOR_CROSSHAIR);
        updatePath();
    }
    else if (rubberMode == OBJ_RUBBER_GRIP)
    {
        //TODO: updateRubber() gripping for EllipseObject
    }
}

void embEllipse_vulcanize()
{
    debug_message("EllipseObject vulcanize()");
    updateRubber();

    setObjectRubberMode(OBJ_RUBBER_OFF);
}

// Returns the closest snap point to the mouse point
EmbVector ellipse_mouseSnapPoint(const EmbVector& mousePoint)
{
    EmbVector center  = objectCenter();
    EmbVector quad0   = objectQuadrant0();
    EmbVector quad90  = objectQuadrant90();
    EmbVector quad180 = objectQuadrant180();
    EmbVector quad270 = objectQuadrant270();

    float cntrDist = EmbLine(mousePoint, center).length();
    float q0Dist   = EmbLine(mousePoint, quad0).length();
    float q90Dist  = EmbLine(mousePoint, quad90).length();
    float q180Dist = EmbLine(mousePoint, quad180).length();
    float q270Dist = EmbLine(mousePoint, quad270).length();

    float minDist = std::min(std::min(std::min(q0Dist, q90Dist), std::min(q180Dist, q270Dist)), cntrDist);

    if     (minDist == cntrDist) return center;
    else if (minDist == q0Dist)   return quad0;
    else if (minDist == q90Dist)  return quad90;
    else if (minDist == q180Dist) return quad180;
    else if (minDist == q270Dist) return quad270;

    return scenePos();
}

std::vector<EmbVector> ellipse_allGripPoints()
{
    std::vector<EmbVector> gripPoints;
    gripPoints << objectCenter() << objectQuadrant0() << objectQuadrant90() << objectQuadrant180() << objectQuadrant270();
    return gripPoints;
}

void embEllipse_gripEdit(const EmbVector& before, const EmbVector& after)
{
    //TODO: gripEdit() for EllipseObject
}

QPainterPath ellipse_objectSavePath()
{
    QPainterPath path;
    EmbRect r = rect();
    path.arcMoveTo(r, 0);
    path.arcTo(r, 0, 360);

    float s = scale();
    QTransform trans;
    trans.rotate(rotation());
    trans.scale(s,s);
    return trans.map(path);
}

void image_init(EmbRect rect, unsigned int rgb, Qt::PenStyle lineType)
{
    setData(OBJ_TYPE, type);
    setData(OBJ_NAME, "Image");

    //WARNING: DO NOT enable QGraphicsItem::ItemIsMovable. If it is enabled,
    //WARNING: and the item is float clicked, the scene will erratically move the item while zooming.
    //WARNING: All movement has to be handled explicitly by us, not by the scene.
    setFlag(QGraphicsItem::ItemIsSelectable, true);

    setObjectRect(rect);
    setObjectColor(rgb);
    setObjectLineType(lineType);
    setObjectLineWeight(0.35); //TODO: pass in proper lineweight
    setPen(objPen);
}

void image_setObjectRect(float x, float y, float w, float h)
{
    setPos(x, y);
    setRect(0, 0, w, h);
    updatePath();
}

EmbVector image_objectTopLeft()
{
    float alpha = radians(rotation());
    EmbVector tl = rect().topRight() * scale();
    EmbVector ptlrot = embVector_rotate(tl, alpha);
    return scenePos() + ptlrot;
}

EmbVector image_objectTopRight()
{
    float alpha = radians(rotation());
    EmbVector tr = rect().topRight() * scale();
    EmbVector ptrrot = embVector_rotate(tr, alpha);
    return scenePos() + ptrrot;
}

EmbVector image_objectBottomLeft()
{
    float alpha = radians(rotation());
    EmbVector bl = rect().topRight() * scale();
    EmbVector pblrot = embVector_rotate(bl, alpha);
    return scenePos() + pblrot;
}

EmbVector image_objectBottomRight()
{
    float alpha = radians(rotation());
    EmbVector br = rect().topRight() * scale();
    EmbVector pbrrot = embVector_rotate(br, alpha);
    return scenePos() + pbrrot;
}

void image_updatePath()
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

void image_paint(QPainter* painter, const QStyleOptionGraphicsItem* option, QWidget* /*widget*/)
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

void image_updateRubber(QPainter* painter)
{
    int rubberMode = objectRubberMode();
    if (rubberMode == OBJ_RUBBER_IMAGE)
    {
        EmbVector sceneStartPoint = objectRubberPoint("IMAGE_START");
        EmbVector sceneEndPoint = objectRubberPoint("IMAGE_END");
        float x = sceneStartPoint.x();
        float y = sceneStartPoint.y();
        float w = sceneEndPoint.x() - sceneStartPoint.x();
        float h = sceneEndPoint.y() - sceneStartPoint.y();
        setObjectRect(x,y,w,h);
        updatePath();
    }
    else if (rubberMode == OBJ_RUBBER_GRIP)
    {
        //TODO: updateRubber() gripping for ImageObject
    }
}

void image_vulcanize()
{
    debug_message("ImageObject vulcanize()");
    updateRubber();

    setObjectRubberMode(OBJ_RUBBER_OFF);
}

// Returns the closest snap point to the mouse point
EmbVector image_mouseSnapPoint(const EmbVector& mousePoint)
{
    EmbVector ptl = objectTopLeft();     //Top Left Corner QSnap
    EmbVector ptr = objectTopRight();    //Top Right Corner QSnap
    EmbVector pbl = objectBottomLeft();  //Bottom Left Corner QSnap
    EmbVector pbr = objectBottomRight(); //Bottom Right Corner QSnap

    float ptlDist = EmbLine(mousePoint, ptl).length();
    float ptrDist = EmbLine(mousePoint, ptr).length();
    float pblDist = EmbLine(mousePoint, pbl).length();
    float pbrDist = EmbLine(mousePoint, pbr).length();

    float minDist = std::min(std::min(ptlDist, ptrDist), std::min(pblDist, pbrDist));

    if     (minDist == ptlDist) return ptl;
    else if (minDist == ptrDist) return ptr;
    else if (minDist == pblDist) return pbl;
    else if (minDist == pbrDist) return pbr;

    return scenePos();
}

std::vector<EmbVector> image_allGripPoints()
{
    std::vector<EmbVector> gripPoints;
    gripPoints << objectTopLeft() << objectTopRight() << objectBottomLeft() << objectBottomRight();
    return gripPoints;
}

void image_gripEdit(const EmbVector& before, const EmbVector& after)
{
    //TODO: gripEdit() for ImageObject
}

[Menu]
Name=Draw
Position=0

[ToolBar]
Name=Draw
Position=0

[Tips]
ToolTip=&Line
StatusTip=Creates straight line segments:  LINE

[Prompt]
Alias=L, LINE

//Command: Line

var global = {}; //Required
global.firstRun;
global.firstX;
global.firstY;
global.prevX;
global.prevY;

void main()
{
    initCommand();
    clearSelection();
    global.firstRun = true;
    global.firstX = NaN;
    global.firstY = NaN;
    global.prevX = NaN;
    global.prevY = NaN;
    setPromptPrefix(translate("Specify first point: "));
}

void
click(EmbVector p)
{
    if (global.firstRun)
    {
        global.firstRun = false;
        global.firstX = p.x;
        global.firstY = p.y;
        global.prevX = p.x;
        global.prevY = p.y;
        addRubber("LINE");
        setRubberMode("LINE");
        setRubberPoint("LINE_START", global.firstX, global.firstY);
        appendPromptHistory();
        setPromptPrefix(translate("Specify next point or [Undo]: "));
    }
    else
    {
        setRubberPoint("LINE_END", x, y);
        vulcanize();
        addRubber("LINE");
        setRubberMode("LINE");
        setRubberPoint("LINE_START", x, y);
        appendPromptHistory();
        global.prevX = x;
        global.prevY = y;
    }
}

//NOTE: context() is run when a context menu entry is chosen.
void context(std::string str)
{
    todo("LINE", "context()");
}

void
prompt(std::string str)
{
    if (global.firstRun)
    {
        var strList = str.split(",");
        if (isnan(strList[0]) || isnan(strList[1]))
        {
            alert(translate("Invalid point."));
            setPromptPrefix(translate("Specify first point: "));
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
            setPromptPrefix(translate("Specify next point or [Undo]: "));
        }
    }
    else {
        if (str == "U" || str == "UNDO") {
            todo("LINE", "prompt() for UNDO");
        }
        else {
            var strList = str.split(",");
            if (isnan(strList[0]) || isnan(strList[1]))
            {
                alert(translate("Point or option keyword required."));
                setPromptPrefix(translate("Specify next point or [Undo]: "));
            }
            else {
                var x = Number(strList[0]);
                var y = Number(strList[1]);
                setRubberPoint("LINE_END", x, y);
                vulcanize();
                addRubber("LINE");
                setRubberMode("LINE");
                setRubberPoint("LINE_START", x, y);
                global.prevX = x;
                global.prevY = y;
                setPromptPrefix(translate("Specify next point or [Undo]: "));
            }
        }
    }
}

void line_init(EmbLine line_in, unsigned int rgb, PenStyle lineType)
{
    setData(OBJ_TYPE, type);
    setData(OBJ_NAME, "Line");

    line = line_in;

    //WARNING: DO NOT enable QGraphicsItem::ItemIsMovable. If it is enabled,
    //WARNING: and the item is float clicked, the scene will erratically move the item while zooming.
    //WARNING: All movement has to be handled explicitly by us, not by the scene.
    setFlag(QGraphicsItem::ItemIsSelectable, true);

    setObjectColor(rgb);
    setObjectLineType(lineType);
    setObjectLineWeight(0.35); //TODO: pass in proper lineweight
    setPen(objPen);
}

void line_setObjectEndPoint1(EmbVector point1)
{
    float dx = line.start.x - point1.x;
    float dy = line.start.y - point1.y;
    setRotation(0);
    setScale(1);
    setLine(0, 0, dx, dy);
    setPos(point1);
}

void line_setObjectEndPoint2(EmbVector point1)
{
    float dx = line.end.x - point1.x;
    float dy = line.end.y - point1.y;
    setRotation(0);
    setScale(1);
    setLine(0, 0, dx, dy);
    setPos(point1);
}

EmbVector line_objectEndPoint2()
{
    EmbLine lyne = line();
    float alpha = radians(rotation());
    EmbVector point2;
    point2.x = lyne.x2()*scale();
    point2.y = lyne.y2()*scale();
    EmbVector rotEnd = embVector_rotate(point2, alpha);

    return scenePos() + rotEnd;
}

EmbVector line_objectMidPoint()
{
    EmbLine lyne = line();
    EmbVector mp = lyne.pointAt(0.5) * scale();
    float alpha = radians(rotation());
    EmbVector rotMid = embVector_rotate(mp, alpha);

    return scenePos() + rotMid;
}

float line_objectAngle()
{
    return std::fmodf(line().angle() - rotation(), 360.0);
}

void line_paint(QPainter* painter, QStyleOptionGraphicsItem* option, QWidget* /*widget*/)
{
    QGraphicsScene* objScene = scene();
    if (!objScene) return;

    QPen paintPen = pen();
    painter->setPen(paintPen);
    updateRubber(painter);
    if (option->state & QStyle::State_Selected)  {
        paintPen.setStyle(Qt::DashLine);
    }
    if (objScene->property(ENABLE_LWT).toBool()) {
        paintPen = lineWeightPen();
    }
    painter->setPen(paintPen);

    if (objectRubberMode() != OBJ_RUBBER_LINE) painter->drawLine(line());

    if (objScene->property(ENABLE_LWT).toBool() && objScene->property("ENABLE_REAL").toBool()) {
        realRender(painter, path());
    }
}

void line_updateRubber(QPainter* painter)
{
    int rubberMode = objectRubberMode();
    if (rubberMode == OBJ_RUBBER_LINE) {
        EmbVector sceneStartPoint = objectRubberPoint("LINE_START");
        EmbVector sceneQSnapPoint = objectRubberPoint("LINE_END");

        setObjectEndPoint1(sceneStartPoint);
        setObjectEndPoint2(sceneQSnapPoint);

        drawRubberLine(line(), painter, VIEW_COLOR_CROSSHAIR);
    }
    else if (rubberMode == OBJ_RUBBER_GRIP) {
        if (painter) {
            EmbVector gripPoint = objectRubberPoint("GRIP_POINT");
            if     (gripPoint == objectEndPoint1()) painter->drawLine(line().p2(), mapFromScene(objectRubberPoint(std::string())));
            else if (gripPoint == objectEndPoint2()) painter->drawLine(line().p1(), mapFromScene(objectRubberPoint(std::string())));
            else if (gripPoint == objectMidPoint())  painter->drawLine(line().translated(mapFromScene(objectRubberPoint(std::string()))-mapFromScene(gripPoint)));

            EmbLine rubLine(mapFromScene(gripPoint), mapFromScene(objectRubberPoint(std::string())));
            drawRubberLine(rubLine, painter, VIEW_COLOR_CROSSHAIR);
        }
    }
}

void line_vulcanize()
{
    debug_message("LineObject vulcanize()");
    updateRubber();

    setObjectRubberMode(OBJ_RUBBER_OFF);
}

// Returns the closest snap point to the mouse point
EmbVector line_mouseSnapPoint(EmbVector& mousePoint)
{
    EmbVector endPoint1 = objectEndPoint1();
    EmbVector endPoint2 = objectEndPoint2();
    EmbVector midPoint  = objectMidPoint();

    float end1Dist = EmbLine(mousePoint, endPoint1).length();
    float end2Dist = EmbLine(mousePoint, endPoint2).length();
    float midDist  = EmbLine(mousePoint, midPoint).length();

    float minDist = std::min(std::min(end1Dist, end2Dist), midDist);

    if     (minDist == end1Dist) return endPoint1;
    else if (minDist == end2Dist) return endPoint2;
    else if (minDist == midDist)  return midPoint;

    return scenePos();
}

std::vector<EmbVector> line_allGripPoints()
{
    std::vector<EmbVector> gripPoints;
    gripPoints << objectEndPoint1() << objectEndPoint2() << objectMidPoint();
    return gripPoints;
}

void line_gripEdit(EmbVector& before, EmbVector& after)
{
    if     (before == objectEndPoint1()) { setObjectEndPoint1(after); }
    else if (before == objectEndPoint2()) { setObjectEndPoint2(after); }
    else if (before == objectMidPoint())  { EmbVector delta = after-before; moveBy(delta.x(), delta.y()); }
}

QPainterPath line_objectSavePath()
{
    QPainterPath path;
    path.lineTo(objectDeltaX(), objectDeltaY());
    return path;
}

path_PathObject(float x, float y, const QPainterPath p, unsigned int rgb, QGraphicsItem* parent)
{
    debug_message("PathObject Constructor()");
    init(x, y, p, rgb, Qt::SolidLine); //TODO: getCurrentLineType
}

path_PathObject(PathObject* obj, QGraphicsItem* parent)
{
    debug_message("PathObject Constructor()");
    if (obj) {
        init(obj->objectX(), obj->objectY(), obj->objectCopyPath(), obj->objectColorRGB(), Qt::SolidLine); //TODO: getCurrentLineType
        setRotation(obj->rotation());
        setScale(obj->scale());
    }
}

void path_init(float x, float y, const QPainterPath& p, unsigned int rgb, Qt::PenStyle lineType)
{
    setData(OBJ_TYPE, type);
    setData(OBJ_NAME, "Path");

    //WARNING: DO NOT enable QGraphicsItem::ItemIsMovable. If it is enabled,
    //WARNING: and the item is float clicked, the scene will erratically move the item while zooming.
    //WARNING: All movement has to be handled explicitly by us, not by the scene.
    setFlag(QGraphicsItem::ItemIsSelectable, true);

    updatePath(p);
    setObjectPos(x,y);
    setObjectColor(rgb);
    setObjectLineType(lineType);
    setObjectLineWeight(0.35); //TODO: pass in proper lineweight
    setPen(objPen);
}

void path_updatePath(const QPainterPath& p)
{
    normalPath = p;
    QPainterPath reversePath = normalPath.toReversed();
    reversePath.connectPath(normalPath);
    setObjectPath(reversePath);
}

void path_paint(QPainter* painter, const QStyleOptionGraphicsItem* option, QWidget* /*widget*/)
{
    QGraphicsScene* objScene = scene();
    if (!objScene) return;

    QPen paintPen = pen();
    painter->setPen(paintPen);
    updateRubber(painter);
    if (option->state & QStyle::State_Selected)  { paintPen.setStyle(Qt::DashLine); }
    if (objScene->property(ENABLE_LWT).toBool()) { paintPen = lineWeightPen(); }
    painter->setPen(paintPen);

    painter->drawPath(objectPath());
}

void path_updateRubber(QPainter* painter)
{
    //TODO: Path Rubber Modes

    //TODO: updateRubber() gripping for PathObject

}

void path_vulcanize()
{
    debug_message("PathObject vulcanize()");
    updateRubber();

    setObjectRubberMode(OBJ_RUBBER_OFF);

    if (!normalPath.elementCount())
        critical_messagebox(0, translate("Empty Path Error"), translate("The path added contains no points. The command that created this object has flawed logic."));
}

// Returns the closest snap point to the mouse point
EmbVector path_mouseSnapPoint(const EmbVector& mousePoint)
{
    return scenePos();
}

std::vector<EmbVector> path_allGripPoints()
{
    std::vector<EmbVector> gripPoints;
    gripPoints << scenePos(); //TODO: loop thru all path Elements and return their points
    return gripPoints;
}

void path_gripEdit(const EmbVector& before, const EmbVector& after)
{
    //TODO: gripEdit() for PathObject
}

QPainterPath path_objectCopyPath()
{
    return normalPath;
}

QPainterPath path_objectSavePath()
{
    float s = scale();
    QTransform trans;
    trans.rotate(rotation());
    trans.scale(s,s);
    return trans.map(normalPath);
}

void point_init(float x, float y, unsigned int rgb, int lineType)
{
    setData(OBJ_TYPE, type);
    setData(OBJ_NAME, "Point");

    //WARNING: DO NOT enable QGraphicsItem::ItemIsMovable. If it is enabled,
    //WARNING: and the item is float clicked, the scene will erratically move the item while zooming.
    //WARNING: All movement has to be handled explicitly by us, not by the scene.
    setFlag(QGraphicsItem::ItemIsSelectable, true);

    setRect(-0.00000001, -0.00000001, 0.00000002, 0.00000002);
    setObjectPos(x,y);
    setObjectColor(rgb);
    setObjectLineType(lineType);
    setObjectLineWeight(0.35); //TODO: pass in proper lineweight
    setPen(objPen);
}

void point_paint(QPainter* painter, QStyleOptionGraphicsItem* option, QWidget* /*widget*/)
{
    QGraphicsScene* objScene = scene();
    if (!objScene) return;

    QPen paintPen = pen();
    painter->setPen(paintPen);
    updateRubber(painter);
    if (option->state & QStyle::State_Selected)  { paintPen.setStyle(Qt::DashLine); }
    if (objScene->property(ENABLE_LWT).toBool()) { paintPen = lineWeightPen(); }
    painter->setPen(paintPen);

    painter->drawPoint(0,0);
}

void point_updateRubber(QPainter* painter)
{
    int rubberMode = objectRubberMode();
    if (rubberMode == OBJ_RUBBER_GRIP) {
        if (painter) {
            EmbVector gripPoint = objectRubberPoint("GRIP_POINT");
            if (gripPoint == scenePos()) {
                EmbLine rubLine(mapFromScene(gripPoint), mapFromScene(objectRubberPoint(std::string())));
                drawRubberLine(rubLine, painter, VIEW_COLOR_CROSSHAIR);
            }
        }
    }
}

void point_vulcanize()
{
    debug_message("PointObject vulcanize()");
    updateRubber();

    setObjectRubberMode(OBJ_RUBBER_OFF);
}

// Returns the closest snap point to the mouse point
EmbVector point_mouseSnapPoint(EmbVector& mousePoint)
{
    return scenePos();
}

std::vector<EmbVector> point_allGripPoints()
{
    std::vector<EmbVector> gripPoints;
    gripPoints << scenePos();
    return gripPoints;
}

void point_gripEdit(EmbVector& before, EmbVector& after)
{
    if (before == scenePos()) { EmbVector delta = after-before; moveBy(delta.x(), delta.y()); }
}

QPainterPath point_objectSavePath()
{
    QPainterPath path;
    path.addRect(-0.00000001, -0.00000001, 0.00000002, 0.00000002);
    return path;
}

void polygon_PolygonObject(float x, float y, const QPainterPath& p, unsigned int rgb, QGraphicsItem* parent)
{
    debug_message("PolygonObject Constructor()");
    init(x, y, p, rgb, SolidLine); //TODO: getCurrentLineType
}

void polygon_PolygonObject(PolygonObject* obj, QGraphicsItem* parent)
{
    debug_message("PolygonObject Constructor()");
    if (obj)
    {
        init(obj->objectX(), obj->objectY(), obj->objectCopyPath(), obj->objectColorRGB(), SolidLine); //TODO: getCurrentLineType
        setRotation(obj->rotation());
        setScale(obj->scale());
    }
}

void polygon_init(float x, float y, const QPainterPath& p, unsigned int rgb, PenStyle lineType)
{
    setData(OBJ_TYPE, type);
    setData(OBJ_NAME, "Polygon");

    //WARNING: DO NOT enable QGraphicsItem::ItemIsMovable. If it is enabled,
    //WARNING: and the item is float clicked, the scene will erratically move the item while zooming.
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

void polygon_updatePath(const QPainterPath& p)
{
    normalPath = p;
    QPainterPath closedPath = normalPath;
    closedPath.closeSubpath();
    QPainterPath reversePath = closedPath.toReversed();
    reversePath.connectPath(closedPath);
    setObjectPath(reversePath);
}

void polygon_paint(QPainter* painter, const QStyleOptionGraphicsItem* option, QWidget* /*widget*/)
{
    QGraphicsScene* objScene = scene();
    if (!objScene) return;

    QPen paintPen = pen();
    painter->setPen(paintPen);
    updateRubber(painter);
    if (option->state & QStyle::State_Selected)  { paintPen.setStyle(DashLine); }
    if (objScene->property(ENABLE_LWT).toBool()) { paintPen = lineWeightPen(); }
    painter->setPen(paintPen);

    if (normalPath.elementCount())
    {
        painter->drawPath(normalPath);
        QPainterPath::Element zero = normalPath.elementAt(0);
        QPainterPath::Element last = normalPath.elementAt(normalPath.elementCount()-1);
        painter->drawLine(EmbVector(zero.x, zero.y), EmbVector(last.x, last.y));
    }
}

void polygon_updateRubber(QPainter* painter)
{
    int rubberMode = objectRubberMode();
    if (rubberMode == OBJ_RUBBER_POLYGON)
    {
        setObjectPos(objectRubberPoint("POLYGON_POINT_0"));

        bool ok = false;
        std::string numStr = objectRubberText("POLYGON_NUM_POINTS");
        if (numStr.isNull()) return;
        int num = numStr.toInt(&ok);
        if (!ok) return;

        std::string appendStr;
        QPainterPath rubberPath;
        rubberPath.moveTo(mapFromScene(objectRubberPoint("POLYGON_POINT_0")));
        for (int i = 1; i <= num; i++)
        {
            appendStr = "POLYGON_POINT_" + std::string().setNum(i);
            EmbVector appendPoint = mapFromScene(objectRubberPoint(appendStr));
            rubberPath.lineTo(appendPoint);
        }
        //rubberPath.lineTo(0,0);
        updatePath(rubberPath);

        //Ensure the path isn't updated until the number of points is changed again
        setObjectRubberText("POLYGON_NUM_POINTS", std::string());
    }
    else if (rubberMode == OBJ_RUBBER_POLYGON_INSCRIBE)
    {
        setObjectPos(objectRubberPoint("POLYGON_CENTER"));

        quint16 numSides = objectRubberPoint("POLYGON_NUM_SIDES").x();

        EmbVector inscribePoint = mapFromScene(objectRubberPoint("POLYGON_INSCRIBE_POINT"));
        EmbLine inscribeLine = EmbLine(EmbVector(0,0), inscribePoint);
        float inscribeAngle = inscribeLine.angle();
        float inscribeInc = 360.0/numSides;

        if (painter) drawRubberLine(inscribeLine, painter, VIEW_COLOR_CROSSHAIR);

        QPainterPath inscribePath;
        //First Point
        inscribePath.moveTo(inscribePoint);
        //Remaining Points
        for (int i = 1; i < numSides; i++)
        {
            inscribeLine.setAngle(inscribeAngle + inscribeInc*i);
            inscribePath.lineTo(inscribeLine.p2());
        }
        updatePath(inscribePath);
    }
    else if (rubberMode == OBJ_RUBBER_POLYGON_CIRCUMSCRIBE)
    {
        setObjectPos(objectRubberPoint("POLYGON_CENTER"));

        quint16 numSides = objectRubberPoint("POLYGON_NUM_SIDES").x();

        EmbVector circumscribePoint = mapFromScene(objectRubberPoint("POLYGON_CIRCUMSCRIBE_POINT"));
        EmbLine circumscribeLine = EmbLine(EmbVector(0,0), circumscribePoint);
        float circumscribeAngle = circumscribeLine.angle();
        float circumscribeInc = 360.0/numSides;

        if (painter) drawRubberLine(circumscribeLine, painter, VIEW_COLOR_CROSSHAIR);

        QPainterPath circumscribePath;
        //First Point
        EmbLine prev(circumscribeLine.p2(), EmbVector(0,0));
        prev = prev.normalVector();
        circumscribeLine.setAngle(circumscribeAngle + circumscribeInc);
        EmbLine perp(circumscribeLine.p2(), EmbVector(0,0));
        perp = perp.normalVector();
        EmbVector iPoint;
        perp.intersects(prev, &iPoint);
        circumscribePath.moveTo(iPoint);
        //Remaining Points
        for (int i = 2; i <= numSides; i++)
        {
            prev = perp;
            circumscribeLine.setAngle(circumscribeAngle + circumscribeInc*i);
            perp = EmbLine(circumscribeLine.p2(), EmbVector(0,0));
            perp = perp.normalVector();
            perp.intersects(prev, &iPoint);
            circumscribePath.lineTo(iPoint);
        }
        updatePath(circumscribePath);
    }
    else if (rubberMode == OBJ_RUBBER_GRIP) {
        if (painter) {
            int elemCount = normalPath.elementCount();
            EmbVector gripPoint = objectRubberPoint("GRIP_POINT");
            if (gripIndex == -1) gripIndex = findIndex(gripPoint);
            if (gripIndex == -1) return;

            int m = 0;
            int n = 0;

            if (!gripIndex)                    { m = elemCount-1; n = 1; }
            else if (gripIndex == elemCount-1) { m = elemCount-2; n = 0; }
            else                              { m = gripIndex-1; n = gripIndex+1; }
            QPainterPath::Element em = normalPath.elementAt(m);
            QPainterPath::Element en = normalPath.elementAt(n);
            EmbVector emPoint = EmbVector(em.x, em.y);
            EmbVector enPoint = EmbVector(en.x, en.y);
            painter->drawLine(emPoint, mapFromScene(objectRubberPoint(std::string())));
            painter->drawLine(enPoint, mapFromScene(objectRubberPoint(std::string())));

            EmbLine rubLine(mapFromScene(gripPoint), mapFromScene(objectRubberPoint(std::string())));
            drawRubberLine(rubLine, painter, VIEW_COLOR_CROSSHAIR);
        }
    }
}

void polygon_vulcanize()
{
    debug_message("PolygonObject vulcanize()");
    updateRubber();

    setObjectRubberMode(OBJ_RUBBER_OFF);

    if (!normalPath.elementCount())
        critical_messagebox(0, translate("Empty Polygon Error"), translate("The polygon added contains no points. The command that created this object has flawed logic."));
}

// Returns the closest snap point to the mouse point
EmbVector polygon_mouseSnapPoint(const EmbVector& mousePoint)
{
    QPainterPath::Element element = normalPath.elementAt(0);
    EmbVector closestPoint = mapToScene(EmbVector(element.x, element.y));
    float closestDist = EmbLine(mousePoint, closestPoint).length();
    int elemCount = normalPath.elementCount();
    for (int i = 0; i < elemCount; ++i) {
        element = normalPath.elementAt(i);
        EmbVector elemPoint = mapToScene(element.x, element.y);
        float elemDist = EmbLine(mousePoint, elemPoint).length();
        if (elemDist < closestDist) {
            closestPoint = elemPoint;
            closestDist = elemDist;
        }
    }
    return closestPoint;
}

std::vector<EmbVector> polygon_allGripPoints()
{
    std::vector<EmbVector> gripPoints;
    QPainterPath::Element element;
    for (int i = 0; i < normalPath.elementCount(); ++i) {
        element = normalPath.elementAt(i);
        gripPoints << mapToScene(element.x, element.y);
    }
    return gripPoints;
}

int polygon_findIndex(const EmbVector& point)
{
    int i = 0;
    int elemCount = normalPath.elementCount();
    //NOTE: Points here are in item coordinates
    EmbVector itemPoint = mapFromScene(point);
    for (i = 0; i < elemCount; i++)
    {
        QPainterPath::Element e = normalPath.elementAt(i);
        EmbVector elemPoint = EmbVector(e.x, e.y);
        if (itemPoint == elemPoint) return i;
    }
    return -1;
}

void polygon_gripEdit(const EmbVector& before, const EmbVector& after)
{
    gripIndex = findIndex(before);
    if (gripIndex == -1) return;
    EmbVector a = mapFromScene(after);
    normalPath.setElementPositionAt(gripIndex, a.x(), a.y());
    updatePath(normalPath);
    gripIndex = -1;
}

QPainterPath polygon_objectCopyPath()
{
    return normalPath;
}

QPainterPath polygon_objectSavePath()
{
    QPainterPath closedPath = normalPath;
    closedPath.closeSubpath();
    float s = scale();
    QTransform trans;
    trans.rotate(rotation());
    trans.scale(s,s);
    return trans.map(closedPath);
}

void polyline_PolylineObject(float x, float y, const QPainterPath& p, unsigned int rgb, QGraphicsItem* parent)
{
    debug_message("PolylineObject Constructor()");
    init(x, y, p, rgb, Qt::SolidLine); //TODO: getCurrentLineType
}

void polyline_PolylineObject(PolylineObject* obj, QGraphicsItem* parent)
{
    debug_message("PolylineObject Constructor()");
    if (obj)
    {
        init(obj->objectX(), obj->objectY(), obj->objectCopyPath(), obj->objectColorRGB(), Qt::SolidLine); //TODO: getCurrentLineType
        setRotation(obj->rotation());
        setScale(obj->scale());
    }
}

void polyline_init(float x, float y, const QPainterPath& p, unsigned int rgb, Qt::PenStyle lineType)
{
    setData(OBJ_TYPE, type);
    setData(OBJ_NAME, "Polyline");

    //WARNING: DO NOT enable QGraphicsItem::ItemIsMovable. If it is enabled,
    //WARNING: and the item is float clicked, the scene will erratically move the item while zooming.
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

void polyline_updatePath(const QPainterPath& p)
{
    normalPath = p;
    QPainterPath reversePath = normalPath.toReversed();
    reversePath.connectPath(normalPath);
    setObjectPath(reversePath);
}

void polyline_paint(QPainter* painter, const QStyleOptionGraphicsItem* option, QWidget* /*widget*/)
{
    QGraphicsScene* objScene = scene();
    if (!objScene) return;

    QPen paintPen = pen();
    painter->setPen(paintPen);
    updateRubber(painter);
    if (option->state & QStyle::State_Selected)  { paintPen.setStyle(Qt::DashLine); }
    if (objScene->property(ENABLE_LWT).toBool()) { paintPen = lineWeightPen(); }
    painter->setPen(paintPen);

    painter->drawPath(normalPath);

    if (objScene->property(ENABLE_LWT).toBool() && objScene->property(ENABLE_REAL).toBool()) { realRender(painter, normalPath); }
}

void polyline_updateRubber(QPainter* painter)
{
    int rubberMode = objectRubberMode();
    if (rubberMode == OBJ_RUBBER_POLYLINE)
    {
        setObjectPos(objectRubberPoint("POLYLINE_POINT_0"));

        EmbLine rubberLine(normalPath.currentPosition(), mapFromScene(objectRubberPoint(std::string())));
        if (painter) drawRubberLine(rubberLine, painter, VIEW_COLOR_CROSSHAIR);

        bool ok = false;
        std::string numStr = objectRubberText("POLYLINE_NUM_POINTS");
        if (numStr.isNull()) return;
        int num = numStr.toInt(&ok);
        if (!ok) return;

        std::string appendStr;
        QPainterPath rubberPath;
        for (int i = 1; i <= num; i++)
        {
            appendStr = "POLYLINE_POINT_" + std::string().setNum(i);
            EmbVector appendPoint = mapFromScene(objectRubberPoint(appendStr));
            rubberPath.lineTo(appendPoint);
        }
        updatePath(rubberPath);

        //Ensure the path isn't updated until the number of points is changed again
        setObjectRubberText("POLYLINE_NUM_POINTS", std::string());
    }
    else if (rubberMode == OBJ_RUBBER_GRIP)
    {
        if (painter)
        {
            int elemCount = normalPath.elementCount();
            EmbVector gripPoint = objectRubberPoint("GRIP_POINT");
            if (gripIndex == -1) gripIndex = findIndex(gripPoint);
            if (gripIndex == -1) return;

            if (!gripIndex) //First
            {
                QPainterPath::Element ef = normalPath.elementAt(1);
                EmbVector efPoint = EmbVector(ef.x, ef.y);
                painter->drawLine(efPoint, mapFromScene(objectRubberPoint(std::string())));
            }
            else if (gripIndex == elemCount-1) //Last
            {
                QPainterPath::Element el = normalPath.elementAt(gripIndex-1);
                EmbVector elPoint = EmbVector(el.x, el.y);
                painter->drawLine(elPoint, mapFromScene(objectRubberPoint(std::string())));
            }
            else //Middle
            {
                QPainterPath::Element em = normalPath.elementAt(gripIndex-1);
                QPainterPath::Element en = normalPath.elementAt(gripIndex+1);
                EmbVector emPoint = EmbVector(em.x, em.y);
                EmbVector enPoint = EmbVector(en.x, en.y);
                painter->drawLine(emPoint, mapFromScene(objectRubberPoint(std::string())));
                painter->drawLine(enPoint, mapFromScene(objectRubberPoint(std::string())));
            }

            EmbLine rubLine(mapFromScene(gripPoint), mapFromScene(objectRubberPoint(std::string())));
            drawRubberLine(rubLine, painter, VIEW_COLOR_CROSSHAIR);
        }
    }
}

void polyline_vulcanize()
{
    debug_message("PolylineObject vulcanize()");
    updateRubber();

    setObjectRubberMode(OBJ_RUBBER_OFF);

    if (!normalPath.elementCount())
        critical_messagebox(0, translate("Empty Polyline Error"), translate("The polyline added contains no points. The command that created this object has flawed logic."));
}

// Returns the closest snap point to the mouse point
EmbVector polyline_mouseSnapPoint(const EmbVector& mousePoint)
{
    QPainterPath::Element element = normalPath.elementAt(0);
    EmbVector closestPoint = mapToScene(EmbVector(element.x, element.y));
    float closestDist = EmbLine(mousePoint, closestPoint).length();
    int elemCount = normalPath.elementCount();
    for (int i = 0; i < elemCount; ++i)
    {
        element = normalPath.elementAt(i);
        EmbVector elemPoint = mapToScene(element.x, element.y);
        float elemDist = EmbLine(mousePoint, elemPoint).length();
        if (elemDist < closestDist)
        {
            closestPoint = elemPoint;
            closestDist = elemDist;
        }
    }
    return closestPoint;
}

std::vector<EmbVector> polyline_allGripPoints()
{
    std::vector<EmbVector> gripPoints;
    QPainterPath::Element element;
    for (int i = 0; i < normalPath.elementCount(); ++i)
    {
        element = normalPath.elementAt(i);
        gripPoints << mapToScene(element.x, element.y);
    }
    return gripPoints;
}

int polyline_findIndex(const EmbVector& point)
{
    int elemCount = normalPath.elementCount();
    //NOTE: Points here are in item coordinates
    EmbVector itemPoint = mapFromScene(point);
    for (int i = 0; i < elemCount; i++) {
        QPainterPath::Element e = normalPath.elementAt(i);
        EmbVector elemPoint = EmbVector(e.x, e.y);
        if (itemPoint == elemPoint) return i;
    }
    return -1;
}

void polyline_gripEdit(const EmbVector& before, const EmbVector& after)
{
    gripIndex = findIndex(before);
    if (gripIndex == -1) return;
    EmbVector a = mapFromScene(after);
    normalPath.setElementPositionAt(gripIndex, a.x(), a.y());
    updatePath(normalPath);
    gripIndex = -1;
}

QPainterPath polyline_objectCopyPath()
{
    return normalPath;
}

QPainterPath polyline_objectSavePath()
{
    float s = scale();
    QTransform trans;
    trans.rotate(rotation());
    trans.scale(s,s);
    return trans.map(normalPath);
}

void rect_init(EmbRect rect, unsigned int rgb, PenStyle lineType)
{
    setData(OBJ_TYPE, type);
    setData(OBJ_NAME, "Rectangle");

    //WARNING: DO NOT enable QGraphicsItem::ItemIsMovable. If it is enabled,
    //WARNING: and the item is float clicked, the scene will erratically move the item while zooming.
    //WARNING: All movement has to be handled explicitly by us, not by the scene.
    setFlag(QGraphicsItem::ItemIsSelectable, true);

    setObjectRect(x, y, w, h);
    setObjectColor(rgb);
    setObjectLineType(lineType);
    setObjectLineWeight(0.35); //TODO: pass in proper lineweight
    setPen(objectPen());
}

void rect_setObjectRect(float x, float y, float w, float h)
{
    setPos(x, y);
    setRect(0, 0, w, h);
    updatePath();
}

EmbVector rect_objectTopLeft()
{
    float alpha = radians(rotation());
    EmbVector tl = rect().topLeft() * scale();
    EmbVector ptlrot = embVector_rotate(t1, alpha);
    return scenePos() + ptlrot;
}

EmbVector rect_objectTopRight()
{
    float alpha = radians(rotation());
    EmbVector tr = rect().topRight() * scale();
    EmbVector ptlrot = embVector_rotate(t1, alpha);
    return scenePos() + ptrrot;
}

EmbVector rect_objectBottomLeft()
{
    float alpha = radians(rotation());
    EmbVector bl = rect().bottomLeft() * scale();
    EmbVector pblrot = embVector_rotate(b1, alpha);
    return scenePos() + pblrot;
}

EmbVector rect_objectBottomRight()
{
    float alpha = radians(rotation());
    EmbVector br = rect().bottomRight() * scale();
    EmbVector pbrrot = embVector_rotate(br, alpha);
    return scenePos() + pbrrot;
}

void rect_updatePath()
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

void rect_paint(QPainter* painter, const QStyleOptionGraphicsItem* option, QWidget* /*widget*/)
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

void rect_updateRubber(QPainter* painter)
{
    int rubberMode = objectRubberMode();
    if (rubberMode == OBJ_RUBBER_RECTANGLE) {
        EmbVector sceneStartPoint = objectRubberPoint("RECTANGLE_START");
        EmbVector sceneEndPoint = objectRubberPoint("RECTANGLE_END");
        float x = sceneStartPoint.x();
        float y = sceneStartPoint.y();
        float w = sceneEndPoint.x() - sceneStartPoint.x();
        float h = sceneEndPoint.y() - sceneStartPoint.y();
        setObjectRect(x,y,w,h);
        updatePath();
    }
    else if (rubberMode == OBJ_RUBBER_GRIP) {
        if (painter)
        {
            //TODO: Make this work with rotation & scaling
            /*
            EmbVector gripPoint = objectRubberPoint("GRIP_POINT");
            EmbVector after = objectRubberPoint(std::string());
            EmbVector delta = after-gripPoint;
            if     (gripPoint == objectTopLeft())     { painter->drawPolygon(mapFromScene(EmbRect(after.x(), after.y(), objectWidth()-delta.x(), objectHeight()-delta.y()))); }
            else if (gripPoint == objectTopRight())    { painter->drawPolygon(mapFromScene(EmbRect(objectTopLeft().x(), objectTopLeft().y()+delta.y(), objectWidth()+delta.x(), objectHeight()-delta.y()))); }
            else if (gripPoint == objectBottomLeft())  { painter->drawPolygon(mapFromScene(EmbRect(objectTopLeft().x()+delta.x(), objectTopLeft().y(), objectWidth()-delta.x(), objectHeight()+delta.y()))); }
            else if (gripPoint == objectBottomRight()) { painter->drawPolygon(mapFromScene(EmbRect(objectTopLeft().x(), objectTopLeft().y(), objectWidth()+delta.x(), objectHeight()+delta.y()))); }

            EmbLine rubLine(mapFromScene(gripPoint), mapFromScene(objectRubberPoint(std::string())));
            drawRubberLine(rubLine, painter, VIEW_COLOR_CROSSHAIR);
            */

            EmbVector gripPoint = objectRubberPoint("GRIP_POINT");
            EmbVector after = objectRubberPoint(std::string());
            EmbVector delta = after-gripPoint;

            EmbLine rubLine(mapFromScene(gripPoint), mapFromScene(objectRubberPoint(std::string())));
            drawRubberLine(rubLine, painter, VIEW_COLOR_CROSSHAIR);
        }
    }
}

void rect_vulcanize()
{
    debug_message("RectObject vulcanize()");
    updateRubber();

    setObjectRubberMode(OBJ_RUBBER_OFF);
}

// Returns the closest snap point to the mouse point
EmbVector rect_mouseSnapPoint(const EmbVector& mousePoint)
{
    EmbVector ptl = objectTopLeft();     //Top Left Corner QSnap
    EmbVector ptr = objectTopRight();    //Top Right Corner QSnap
    EmbVector pbl = objectBottomLeft();  //Bottom Left Corner QSnap
    EmbVector pbr = objectBottomRight(); //Bottom Right Corner QSnap

    float ptlDist = EmbLine(mousePoint, ptl).length();
    float ptrDist = EmbLine(mousePoint, ptr).length();
    float pblDist = EmbLine(mousePoint, pbl).length();
    float pbrDist = EmbLine(mousePoint, pbr).length();

    float minDist = std::min(std::min(ptlDist, ptrDist), std::min(pblDist, pbrDist));

    if     (minDist == ptlDist) return ptl;
    else if (minDist == ptrDist) return ptr;
    else if (minDist == pblDist) return pbl;
    else if (minDist == pbrDist) return pbr;

    return scenePos();
}

std::vector<EmbVector> rect_allGripPoints()
{
    std::vector<EmbVector> gripPoints;
    gripPoints << objectTopLeft() << objectTopRight() << objectBottomLeft() << objectBottomRight();
    return gripPoints;
}

void rect_gripEdit(const EmbVector& before, const EmbVector& after)
{
    EmbVector delta = after-before;
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

QPainterPath rect_objectSavePath()
{
    QPainterPath path;
    EmbRect r = rect();
    path.moveTo(r.bottomLeft());
    path.lineTo(r.bottomRight());
    path.lineTo(r.topRight());
    path.lineTo(r.topLeft());
    path.lineTo(r.bottomLeft());

    float s = scale();
    QTransform trans;
    trans.rotate(rotation());
    trans.scale(s,s);
    return trans.map(path);
}

bool save_save(const std::string &fileName)
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

//NOTE: This void should be used to interpret various object types and save them as polylines for stitchOnly formats.
void save_toPolyline(EmbPattern* pattern, const EmbVector& objPos, const QPainterPath& objPath, const std::string& layer, const QColor& color, const std::string& lineType, const std::string& lineWeight)
{
    float startX = objPos.x();
    float startY = objPos.y();
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
    embPattern_addPolylineAbs(pattern, polyObject);
}

void text_single_TextSingleObject(const std::string& str, float x, float y, unsigned int rgb, QGraphicsItem* parent)
{
    debug_message("TextSingleObject Constructor()");
    init(str, x, y, rgb, Qt::SolidLine); //TODO: getCurrentLineType
}

void text_single_TextSingleObject(TextSingleObject* obj, QGraphicsItem* parent)
{
    debug_message("TextSingleObject Constructor()");
    if (obj) {
        setObjectTextFont(obj->objTextFont);
        setObjectTextSize(obj->objTextSize);
        setRotation(obj->rotation());
        setObjectTextBackward(obj->objTextBackward);
        setObjectTextUpsideDown(obj->objTextUpsideDown);
        setObjectTextStyle(obj->objTextBold, obj->objTextItalic, obj->objTextUnderline, obj->objTextStrikeOut, obj->objTextOverline);
        init(obj->objText, obj->objectX(), obj->objectY(), obj->objectColorRGB(), SolidLine); //TODO: getCurrentLineType
        setScale(obj->scale());
    }
}

void text_single_init(const std::string& str, float x, float y, unsigned int rgb, Qt::PenStyle lineType)
{
    setData(OBJ_TYPE, type);
    setData(OBJ_NAME, "Single Line Text");

    //WARNING: DO NOT enable QGraphicsItem::ItemIsMovable. If it is enabled,
    //WARNING: and the item is float clicked, the scene will erratically move the item while zooming.
    //WARNING: All movement has to be handled explicitly by us, not by the scene.
    setFlag(QGraphicsItem::ItemIsSelectable, true);

    objTextJustify = "Left"; //TODO: set the justification properly

    setObjectText(str);
    setObjectPos(x,y);
    setObjectColor(rgb);
    setObjectLineType(lineType);
    setObjectLineWeight(0.35); //TODO: pass in proper lineweight
    setPen(objPen);
}

std::stringList text_single_objectTextJustifyList()
{
    std::stringList justifyList;
    justifyList << "Left" << "Center" << "Right" /* TODO: << "Aligned" */ << "Middle" /* TODO: << "Fit" */ ;
    justifyList << "Top Left" << "Top Center" << "Top Right";
    justifyList << "Middle Left" << "Middle Center" << "Middle Right";
    justifyList << "Bottom Left" << "Bottom Center" << "Bottom Right";
    return justifyList;
}

void text_single_setObjectText(const std::string& str)
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
    EmbRect jRect = textPath.boundingRect();
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
        
    } //TODO: TextSingleObject Aligned Justification
    else if (objTextJustify == "Middle") {
        textPath.translate(-jRect.center()); }
    else if (objTextJustify == "Fit") {
        
    } //TODO: TextSingleObject Fit Justification
    else if (objTextJustify == "Top Left") {
        textPath.translate(-jRect.topLeft());
    }
    else if (objTextJustify == "Top Center") {
        textPath.translate(-jRect.center().x(), -jRect.top());
    }
    else if (objTextJustify == "Top Right")     {
        textPath.translate(-jRect.topRight()); }
    else if (objTextJustify == "Middle Left")   {
        textPath.translate(-jRect.left(), -jRect.top()/2.0); }
    else if (objTextJustify == "Middle Center") { textPath.translate(-jRect.center().x(), -jRect.top()/2.0); }
    else if (objTextJustify == "Middle Right")  { textPath.translate(-jRect.right(), -jRect.top()/2.0); }
    else if (objTextJustify == "Bottom Left")   { textPath.translate(-jRect.bottomLeft()); }
    else if (objTextJustify == "Bottom Center") { textPath.translate(-jRect.center().x(), -jRect.bottom()); }
    else if (objTextJustify == "Bottom Right")  { textPath.translate(-jRect.bottomRight()); }

    //Backward or Upside Down
    if (objTextBackward || objTextUpsideDown) {
        float horiz = 1.0;
        float vert = 1.0;
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

void text_single_setObjectTextFont(const std::string& font)
{
    objTextFont = font;
    setObjectText(objText);
}

void text_single_setObjectTextJustify(const std::string& justify)
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
    else if (justify == "Middle")        { objTextJustify = justify; }
    else if (justify == "Fit")           { objTextJustify = justify; }
    else if (justify == "Top Left")      { objTextJustify = justify; }
    else if (justify == "Top Center")    { objTextJustify = justify; }
    else if (justify == "Top Right")     { objTextJustify = justify; }
    else if (justify == "Middle Left")   { objTextJustify = justify; }
    else if (justify == "Middle Center") { objTextJustify = justify; }
    else if (justify == "Middle Right")  { objTextJustify = justify; }
    else if (justify == "Bottom Left")   { objTextJustify = justify; }
    else if (justify == "Bottom Center") { objTextJustify = justify; }
    else if (justify == "Bottom Right")  { objTextJustify = justify; }
    else                                { objTextJustify = "Left";  } //Default
    setObjectText(objText);
}

void text_single_setObjectTextSize(float size)
{
    objTextSize = size;
    setObjectText(objText);
}

void text_single_setObjectTextStyle(bool bold, bool italic, bool under, bool strike, bool over)
{
    objTextBold = bold;
    objTextItalic = italic;
    objTextUnderline = under;
    objTextStrikeOut = strike;
    objTextOverline = over;
    setObjectText(objText);
}

void text_single_setObjectTextBold(bool val)
{
    objTextBold = val;
    setObjectText(objText);
}

void text_single_setObjectTextItalic(bool val)
{
    objTextItalic = val;
    setObjectText(objText);
}

void text_single_setObjectTextUnderline(bool val)
{
    objTextUnderline = val;
    setObjectText(objText);
}

void text_single_setObjectTextStrikeOut(bool val)
{
    objTextStrikeOut = val;
    setObjectText(objText);
}

void text_single_setObjectTextOverline(bool val)
{
    objTextOverline = val;
    setObjectText(objText);
}

void text_single_setObjectTextBackward(bool val)
{
    objTextBackward = val;
    setObjectText(objText);
}

void text_single_setObjectTextUpsideDown(bool val)
{
    objTextUpsideDown = val;
    setObjectText(objText);
}

void text_single_paint(QPainter* painter, const QStyleOptionGraphicsItem* option, QWidget* /*widget*/)
{
    QGraphicsScene* objScene = scene();
    if (!objScene) return;

    QPen paintPen = pen();
    painter->setPen(paintPen);
    updateRubber(painter);
    if (option->state & QStyle::State_Selected)  { paintPen.setStyle(Qt::DashLine); }
    if (objScene->property(ENABLE_LWT).toBool()) { paintPen = lineWeightPen(); }
    painter->setPen(paintPen);

    painter->drawPath(objTextPath);
}

void text_single_updateRubber(QPainter* painter)
{
    int rubberMode = objectRubberMode();
    if (rubberMode == OBJ_RUBBER_TEXTSINGLE) {
        setObjectTextFont(objectRubberText("TEXT_FONT"));
        setObjectTextJustify(objectRubberText("TEXT_JUSTIFY"));
        setObjectPos(objectRubberPoint("TEXT_POINT"));
        EmbVector hr = objectRubberPoint("TEXT_HEIGHT_ROTATION");
        setObjectTextSize(hr.x());
        setRotation(hr.y());
        setObjectText(objectRubberText("TEXT_RAPID"));
    }
    else if (rubberMode == OBJ_RUBBER_GRIP) {
        if (painter) {
            EmbVector gripPoint = objectRubberPoint("GRIP_POINT");
            if (gripPoint == scenePos()) {
                painter->drawPath(objectPath().translated(mapFromScene(objectRubberPoint(std::string()))-mapFromScene(gripPoint)));
            }

            EmbLine rubLine(mapFromScene(gripPoint), mapFromScene(objectRubberPoint(std::string())));
            drawRubberLine(rubLine, painter, VIEW_COLOR_CROSSHAIR);
        }
    }
}

void text_single_vulcanize()
{
    debug_message("TextSingleObject vulcanize()");
    updateRubber();

    setObjectRubberMode(OBJ_RUBBER_OFF);
}

// Returns the closest snap point to the mouse point
EmbVector text_single_mouseSnapPoint(const EmbVector& mousePoint)
{
    return scenePos();
}

std::vector<EmbVector> text_single_allGripPoints()
{
    std::vector<EmbVector> gripPoints;
    gripPoints << scenePos();
    return gripPoints;
}

void text_single_gripEdit(const EmbVector& before, const EmbVector& after)
{
    if (before == scenePos()) { EmbVector delta = after-before; moveBy(delta.x(), delta.y()); }
}

std::vector<QPainterPath> text_single_subPathList()
{
    float s = scale();
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
            pathMoves << i;
            numMoves++;
        }
    }

    pathMoves << path.elementCount();

    for (int p = 0; p < pathMoves.size()-1 && p < numMoves; p++) {
        QPainterPath subPath;
        for (int i = pathMoves.value(p); i < pathMoves.value(p+1); i++) {
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
        pathList.append(trans.map(subPath));
    }

    return pathList;
}

/*
 * NOTE: main() is run every time the command is started.
 *      Use it to reset variables so they are ready to go.
 *
 * NOTE: click() is run only for left clicks.
 *      Middle clicks are used for panning.
 *      Right clicks bring up the context menu.
 *
 * NOTE: move() is optional. It is run only after
 *      enableMoveRapidFire() is called. It
 *      will be called every time the mouse moves until
 *      disableMoveRapidFire() is called.
 *
 * NOTE: prompt() is run when Enter is pressed.
 *      appendPromptHistory is automatically called before prompt()
 *      is called so calling it is only needed for erroneous input.
 *      Any text in the command prompt is sent as an uppercase string.
 *
 * NOTE: context() is run when a context menu entry is chosen.
 */

typedef struct DolphinUi_ {
    int numPoints;
    EmbVector center, scale;
    unsigned int mode;
} DolphinUi;

 //Default //TODO: min:64 max:8192
numPoints = 512;
global.sx = 0.04; //Default
global.sy = 0.04; //Default

void dolphin_main()
{
    initCommand();
    clearSelection();
    global.cx = NaN;
    global.cy = NaN;
    global.mode = global.mode_NUM_POINTS;

    addRubber("POLYGON");
    setRubberMode("POLYGON");
    updateDolphin(global.numPoints, global.sx, global.sy);
    spareRubber("POLYGON");
    endCommand();
}

void click(float x, float y)
{
}

void move(x, y)
{
}

void context(std::string str)
{
    todo("DOLPHIN", "context()");
}

void prompt(std::string str)
{
}

void updateDolphin(int numPts, float xScale, float yScale)
{
    float xx = NaN;
    float yy = NaN;
    float two_pi = 2*Math.PI;

    for (int i = 0; i <= numPts; i++) {
        float t = two_pi/numPts*i; 

        xx = 4/23*sin(62/33-58*t)+
        8/11*sin(10/9-56*t)+
        17/24*sin(38/35-55*t)+
        30/89*sin(81/23-54*t)+
        3/17*sin(53/18-53*t)+
        21/38*sin(29/19-52*t)+
        11/35*sin(103/40-51*t)+
        7/16*sin(79/18-50*t)+
        4/15*sin(270/77-49*t)+
        19/35*sin(59/27-48*t)+
        37/43*sin(71/17-47*t)+
        sin(18/43-45*t)+
        21/26*sin(37/26-44*t)+
        27/19*sin(111/32-42*t)+
        8/39*sin(13/25-41*t)+
        23/30*sin(27/8-40*t)+
        23/21*sin(32/35-37*t)+
        18/37*sin(91/31-36*t)+
        45/22*sin(29/37-35*t)+
        56/45*sin(11/8-33*t)+
        4/7*sin(32/19-32*t)+
        54/23*sin(74/29-31*t)+
        28/19*sin(125/33-30*t)+
        19/9*sin(73/27-29*t)+
        16/17*sin(737/736-28*t)+
        52/33*sin(130/29-27*t)+
        41/23*sin(43/30-25*t)+
        29/20*sin(67/26-24*t)+
        64/25*sin(136/29-23*t)+
        162/37*sin(59/34-21*t)+
        871/435*sin(199/51-20*t)+
        61/42*sin(58/17-19*t)+
        159/25*sin(77/31-17*t)+
        241/15*sin(94/31-13*t)+
        259/18*sin(114/91-12*t)+
        356/57*sin(23/25-11*t)+
        2283/137*sin(23/25-10*t)+
        1267/45*sin(139/42-9*t)+
        613/26*sin(41/23-8*t)+
        189/16*sin(122/47-6*t)+
        385/6*sin(151/41-5*t)+
        2551/38*sin(106/35-4*t)+
        1997/18*sin(6/5-2*t)+
        43357/47*sin(81/26-t)-
        4699/35*sin(3*t+25/31)-
        1029/34*sin(7*t+20/21)-
        250/17*sin(14*t+7/40)-
        140/17*sin(15*t+14/25)-
        194/29*sin(16*t+29/44)-
        277/52*sin(18*t+37/53)-
        94/41*sin(22*t+33/31)-
        57/28*sin(26*t+44/45)-
        128/61*sin(34*t+11/14)-
        111/95*sin(38*t+55/37)-
        85/71*sin(39*t+4/45)-
        25/29*sin(43*t+129/103)-
        7/37*sin(46*t+9/20)-
        17/32*sin(57*t+11/28)-
        5/16*sin(59*t+32/39);

        yy = 5/11*sin(163/37-59*t)+
        7/22*sin(19/41-58*t)+
        30/41*sin(1-57*t)+
        37/29*sin(137/57-56*t)+
        5/7*sin(17/6-55*t)+
        11/39*sin(46/45-52*t)+
        25/28*sin(116/83-51*t)+
        25/34*sin(11/20-47*t)+
        8/27*sin(81/41-46*t)+
        44/39*sin(78/37-45*t)+
        11/25*sin(107/37-44*t)+
        7/20*sin(7/16-41*t)+
        30/31*sin(19/5-40*t)+
        37/27*sin(148/59-39*t)+
        44/39*sin(17/27-38*t)+
        13/11*sin(7/11-37*t)+
        28/33*sin(119/39-36*t)+
        27/13*sin(244/81-35*t)+
        13/23*sin(113/27-34*t)+
        47/38*sin(127/32-33*t)+
        155/59*sin(173/45-29*t)+
        105/37*sin(22/43-27*t)+
        106/27*sin(23/37-26*t)+
        97/41*sin(53/29-25*t)+
        83/45*sin(109/31-24*t)+
        81/31*sin(96/29-23*t)+
        56/37*sin(29/10-22*t)+
        44/13*sin(29/19-19*t)+
        18/5*sin(34/31-18*t)+
        163/51*sin(75/17-17*t)+
        152/31*sin(61/18-16*t)+
        146/19*sin(47/20-15*t)+
        353/35*sin(55/48-14*t)+
        355/28*sin(102/25-12*t)+
        1259/63*sin(71/18-11*t)+
        17/35*sin(125/52-10*t)+
        786/23*sin(23/26-6*t)+
        2470/41*sin(77/30-5*t)+
        2329/47*sin(47/21-4*t)+
        2527/33*sin(23/14-3*t)+
        9931/33*sin(51/35-2*t)-
        11506/19*sin(t+56/67)-
        2081/42*sin(7*t+9/28)-
        537/14*sin(8*t+3/25)-
        278/29*sin(9*t+23/33)-
        107/15*sin(13*t+35/26)-
        56/19*sin(20*t+5/9)-
        5/9*sin(21*t+1/34)-
        17/24*sin(28*t+36/23)-
        21/11*sin(30*t+27/37)-
        138/83*sin(31*t+1/7)-
        10/17*sin(32*t+29/48)-
        31/63*sin(42*t+27/28)-
        4/27*sin(43*t+29/43)-
        13/24*sin(48*t+5/21)-
        4/7*sin(49*t+29/23)-
        26/77*sin(50*t+29/27)-
        19/14*sin(53*t+61/48)+
        34/25*sin(54*t+37/26);

        setRubberPoint("POLYGON_POINT_" + i.toString(), xx*xScale, yy*yScale);
    }

    setRubberText("POLYGON_NUM_POINTS", numPts.toString());
}

typedef struct HeartUi_ {
    int numPoints;
    EmbVector center, scale;
    unsigned int mode;
} HeartUi;

void heart_main()
{
    initCommand();
    clearSelection();
    global.numPoints = 512; //Default //TODO: min:64 max:8192
    global.sx = 1.0;
    global.sy = 1.0;
    global.cx = NaN;
    global.cy = NaN;
    global.mode = HEART_MODE_NUM_POINTS;

    //Heart4: 10.0 / 512
    //Heart5: 1.0 / 512

    addRubber("POLYGON");
    setRubberMode("POLYGON");
    updateHeart("HEART5", global.numPoints, global.sx, global.sy);
    spareRubber("POLYGON");
    endCommand();
}

void click(float x, float y)
{
}

void move(x, y)
{
}

void context(std::string str)
{
    todo("HEART", "context()");
}

void prompt(std::string str)
{
}

void updateHeart(int style, int numPts, float xScale, float yScale)
{
    float xx = NaN;
    float yy = NaN;
    float two_pi = 2*Math.PI;

    for (int i = 0; i <= numPts; i++) {
        float t = two_pi/numPts*i; 

        if (style == "HEART4") {
            xx = cos(t)*((sin(t)*sqrt(fabs(cos(t))))/(sin(t)+7/5) - 2*sin(t) + 2);
            yy = sin(t)*((sin(t)*sqrt(fabs(cos(t))))/(sin(t)+7/5) - 2*sin(t) + 2);
        }
        else if (style == "HEART5") {
            xx = 16*Math.pow(sin(t), 3);
            yy = 13*cos(t) - 5*cos(2*t) - 2*cos(3*t) - cos(4*t);
        }

        setRubberPoint("POLYGON_POINT_" + i.toString(), xx*xScale, yy*yScale);
    }

    setRubberText("POLYGON_NUM_POINTS", numPts.toString());
}

typedef struct PathUi_ {
    bool firstRun;
    EmbVector first;
    EmbVector prev;
} PathUi;

void path_main(void)
{
    PathUi global;
    initCommand();
    clearSelection();
    global.firstRun = true;
    global.first = {NaN, NaN};
    global.prev = {NaN, NaN};
    setPromptPrefix(translate("Specify start point: "));
}

void click(float x, float y)
{
    if (global.firstRun) {
        global.firstRun = false;
        global.firstX = x;
        global.firstY = y;
        global.prevX = x;
        global.prevY = y;
        addPath(x,y);
        appendPromptHistory();
        setPromptPrefix(translate("Specify next point or [Arc/Undo]: "));
    }
    else {
        appendPromptHistory();
        appendLineToPath(x,y);
        global.prevX = x;
        global.prevY = y;
    }
}

void context(std::string str)
{
    todo("PATH", "context()");
}

void prompt(std::string str)
{
    if (str == "A" || str == "ARC") {
        //TODO: Probably should add additional qsTr calls here.
        todo("PATH", "prompt() for ARC");
    }
    else if (str == "U" || str == "UNDO") {
        //TODO: Probably should add additional qsTr calls here.
        todo("PATH", "prompt() for UNDO");
    }
    else {
        var strList = str.split(",");
        if (isnan(strList[0]) || isnan(strList[1])) {
            alert(translate("Point or option keyword required."));
            setPromptPrefix(translate("Specify next point or [Arc/Undo]: "));
        }
        else {
            float x = Number(strList[0]);
            float y = Number(strList[1]);
            if (global.firstRun) {
                global.firstRun = false;
                global.firstX = x;
                global.firstY = y;
                global.prevX = x;
                global.prevY = y;
                addPath(x,y);
                setPromptPrefix(translate("Specify next point or [Arc/Undo]: "));
            }
            else {
                appendLineToPath(x,y);
                global.prevX = x;
                global.prevY = y;
            }
        }
    }
}

typedef struct PointUi_ {
    bool firstRun;
} PointUi;

void point_main()
{
    initCommand();
    clearSelection();
    global.firstRun = true;
    setPromptPrefix("TODO: Current point settings: PDMODE=?  PDSIZE=?"); //TODO: qsTr needed here when complete
    appendPromptHistory();
    setPromptPrefix(translate("Specify first point: "));
}

void point_click(float x, float y)
{
    if (global.firstRun) {
        global.firstRun = false;
        appendPromptHistory();
        setPromptPrefix(translate("Specify next point: "));
        addPoint(x,y);
    }
    else {
        appendPromptHistory();
        addPoint(x,y);
    }
}

void point_context(std::string str)
{
    todo("POINT", "context()");
}

void point_prompt(std::string str)
{
    if (global.firstRun)
    {
        if (str == "M" || str == "MODE") //TODO: Probably should add additional qsTr calls here.
        {
            todo("POINT", "prompt() for PDMODE");
        }
        else if (str == "S" || str == "SIZE") //TODO: Probably should add additional qsTr calls here.
        {
            todo("POINT", "prompt() for PDSIZE");
        }
        var strList = str.split(",");
        if (isnan(strList[0]) || isnan(strList[1]))
        {
            alert(translate("Invalid point."));
            setPromptPrefix(translate("Specify first point: "));
        }
        else
        {
            global.firstRun = false;
            var x = Number(strList[0]);
            var y = Number(strList[1]);
            setPromptPrefix(translate("Specify next point: "));
            addPoint(x,y);
        }
    }
    else {
        var strList = str.split(",");
        if (isnan(strList[0]) || isnan(strList[1])) {
            alert(translate("Invalid point."));
            setPromptPrefix(translate("Specify next point: "));
        }
        else {
            var x = Number(strList[0]);
            var y = Number(strList[1]);
            setPromptPrefix(translate("Specify next point: "));
            addPoint(x,y);
        }
    }
}

//Command: Polygon

var global = {}; //Required
global.centerX;
global.centerY;
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

//enums
global.mode_NUM_SIDES    = 0;
global.mode_CENTER_PT    = 1;
global.mode_POLYTYPE     = 2;
global.mode_INSCRIBE     = 3;
global.mode_CIRCUMSCRIBE = 4;
global.mode_DISTANCE     = 5;
global.mode_SIDE_LEN     = 6;

void main()
{
    initCommand();
    clearSelection();
    global.centerX = NaN;
    global.centerY = NaN;
    global.sideX1  = NaN;
    global.sideY1  = NaN;
    global.sideX2  = NaN;
    global.sideY2  = NaN;
    global.pointIX = NaN;
    global.pointIY = NaN;
    global.pointCX = NaN;
    global.pointCY = NaN;
    global.mode = global.mode_NUM_SIDES;
    setPromptPrefix(translate("Enter number of sides") + " {" + global.numSides.toString() + "}: ");
}

void click(float x, float y)
{
    if (global.mode == global.mode_NUM_SIDES)
    {
        //Do nothing, the prompt controls this.
    }
    else if (global.mode == global.mode_CENTER_PT)
    {
        global.centerX = x;
        global.centerY = y;
        global.mode = global.mode_POLYTYPE;
        appendPromptHistory();
        setPromptPrefix(translate("Specify polygon type [Inscribed in circle/Circumscribed around circle]") + " {" + global.polyType + "}: ");
    }
    else if (global.mode == global.mode_POLYTYPE)
    {
        //Do nothing, the prompt controls this.
    }
    else if (global.mode == global.mode_INSCRIBE)
    {
        global.pointIX = x;
        global.pointIY = y;
        setRubberPoint("POLYGON_INSCRIBE_POINT", global.pointIX, global.pointIY);
        vulcanize();
        appendPromptHistory();
        endCommand();
    }
    else if (global.mode == global.mode_CIRCUMSCRIBE)
    {
        global.pointCX = x;
        global.pointCY = y;
        setRubberPoint("POLYGON_CIRCUMSCRIBE_POINT", global.pointCX, global.pointCY);
        vulcanize();
        appendPromptHistory();
        endCommand();
    }
    else if (global.mode == global.mode_DISTANCE)
    {
        //Do nothing, the prompt controls this.
    }
    else if (global.mode == global.mode_SIDE_LEN)
    {
        todo("POLYGON", "Sidelength mode");
    }
}

void context(std::string str)
{
    todo("POLYGON", "context()");
}

void prompt(std::string str)
{
    if (global.mode == global.mode_NUM_SIDES)
    {
        if (str == "" && global.numSides >= 3 && global.numSides <= 1024)
        {
            setPromptPrefix(translate("Specify center point or [Sidelength]: "));
            global.mode = global.mode_CENTER_PT;
        }
        else
        {
            var tmp = Number(str);
            if (isnan(tmp) || !isInt(tmp) || tmp < 3 || tmp > 1024)
            {
                alert(translate("Requires an integer between 3 and 1024."));
                setPromptPrefix(translate("Enter number of sides") + " {" + global.numSides.toString() + "}: ");
            }
            else
            {
                global.numSides = tmp;
                setPromptPrefix(translate("Specify center point or [Sidelength]: "));
                global.mode = global.mode_CENTER_PT;
            }
        }
    }
    else if (global.mode == global.mode_CENTER_PT)
    {
        if (str == "S" || str == "SIDELENGTH") //TODO: Probably should add additional qsTr calls here.
        {
            global.mode = global.mode_SIDE_LEN;
            setPromptPrefix(translate("Specify start point: "));
        }
        else
        {
            var strList = str.split(",");
            if (isnan(strList[0]) || isnan(strList[1]))
            {
                alert(translate("Point or option keyword required."));
                setPromptPrefix(translate("Specify center point or [Sidelength]: "));
            }
            else
            {
                global.centerX = Number(strList[0]);
                global.centerY = Number(strList[1]);
                global.mode = global.mode_POLYTYPE;
                setPromptPrefix(translate("Specify polygon type [Inscribed in circle/Circumscribed around circle]") + " {" + global.polyType + "}: ");
            }
        }
    }
    else if (global.mode == global.mode_POLYTYPE)
    {
        if (str == "I"        ||
           str == "IN"       ||
           str == "INS"      ||
           str == "INSC"     ||
           str == "INSCR"    ||
           str == "INSCRI"   ||
           str == "INSCRIB"  ||
           str == "INSCRIBE" ||
           str == "INSCRIBED") //TODO: Probably should add additional qsTr calls here.
        {
            global.mode = global.mode_INSCRIBE;
            global.polyType = "Inscribed";
            setPromptPrefix(translate("Specify polygon corner point or [Distance]: "));
            addRubber("POLYGON");
            setRubberMode("POLYGON_INSCRIBE");
            setRubberPoint("POLYGON_CENTER", global.centerX, global.centerY);
            setRubberPoint("POLYGON_NUM_SIDES", global.numSides, 0);
        }
        else if (str == "C"            ||
                str == "CI"           ||
                str == "CIR"          ||
                str == "CIRC"         ||
                str == "CIRCU"        ||
                str == "CIRCUM"       ||
                str == "CIRCUMS"      ||
                str == "CIRCUMSC"     ||
                str == "CIRCUMSCR"    ||
                str == "CIRCUMSCRI"   ||
                str == "CIRCUMSCRIB"  ||
                str == "CIRCUMSCRIBE" ||
                str == "CIRCUMSCRIBED") //TODO: Probably should add additional qsTr calls here.
        {
            global.mode = global.mode_CIRCUMSCRIBE;
            global.polyType = "Circumscribed";
            setPromptPrefix(translate("Specify polygon side point or [Distance]: "));
            addRubber("POLYGON");
            setRubberMode("POLYGON_CIRCUMSCRIBE");
            setRubberPoint("POLYGON_CENTER", global.centerX, global.centerY);
            setRubberPoint("POLYGON_NUM_SIDES", global.numSides, 0);
        }
        else if (str == "")
        {
            if (global.polyType == "Inscribed")
            {
                global.mode = global.mode_INSCRIBE;
                setPromptPrefix(translate("Specify polygon corner point or [Distance]: "));
                addRubber("POLYGON");
                setRubberMode("POLYGON_INSCRIBE");
                setRubberPoint("POLYGON_CENTER", global.centerX, global.centerY);
                setRubberPoint("POLYGON_NUM_SIDES", global.numSides, 0);
            }
            else if (global.polyType == "Circumscribed")
            {
                global.mode = global.mode_CIRCUMSCRIBE;
                setPromptPrefix(translate("Specify polygon side point or [Distance]: "));
                addRubber("POLYGON");
                setRubberMode("POLYGON_CIRCUMSCRIBE");
                setRubberPoint("POLYGON_CENTER", global.centerX, global.centerY);
                setRubberPoint("POLYGON_NUM_SIDES", global.numSides, 0);
            }
            else
            {
                error("POLYGON", translate("Polygon type is not Inscribed or Circumscribed."));
            }
        }
        else
        {
            alert(translate("Invalid option keyword."));
            setPromptPrefix(translate("Specify polygon type [Inscribed in circle/Circumscribed around circle]") + " {" + global.polyType + "}: ");
        }
    }
    else if (global.mode == global.mode_INSCRIBE)
    {
        if (str == "D" || str == "DISTANCE") //TODO: Probably should add additional qsTr calls here.
        {
            global.mode = global.mode_DISTANCE;
            setPromptPrefix(translate("Specify distance: "));
        }
        else
        {
            var strList = str.split(",");
            if (isnan(strList[0]) || isnan(strList[1]))
            {
                alert(translate("Point or option keyword required."));
                setPromptPrefix(translate("Specify polygon corner point or [Distance]: "));
            }
            else
            {
                global.pointIX = Number(strList[0]);
                global.pointIY = Number(strList[1]);
                setRubberPoint("POLYGON_INSCRIBE_POINT", global.pointIX, global.pointIY);
                vulcanize();
                endCommand();
            }
        }
    }
    else if (global.mode == global.mode_CIRCUMSCRIBE)
    {
        if (str == "D" || str == "DISTANCE") //TODO: Probably should add additional qsTr calls here.
        {
            global.mode = global.mode_DISTANCE;
            setPromptPrefix(translate("Specify distance: "));
        }
        else
        {
            var strList = str.split(",");
            if (isnan(strList[0]) || isnan(strList[1]))
            {
                alert(translate("Point or option keyword required."));
                setPromptPrefix(translate("Specify polygon side point or [Distance]: "));
            }
            else
            {
                global.pointCX = Number(strList[0]);
                global.pointCY = Number(strList[1]);
                setRubberPoint("POLYGON_CIRCUMSCRIBE_POINT", global.pointCX, global.pointCY);
                vulcanize();
                endCommand();
            }
        }
    }
    else if (global.mode == global.mode_DISTANCE)
    {
        if (isnan(str))
        {
            alert(translate("Requires valid numeric distance."));
            setPromptPrefix(translate("Specify distance: "));
        }
        else
        {
            if (global.polyType == "Inscribed")
            {
                global.pointIX = global.centerX;
                global.pointIY = global.centerY + Number(str);
                setRubberPoint("POLYGON_INSCRIBE_POINT", global.pointIX, global.pointIY);
                vulcanize();
                endCommand();
            }
            else if (global.polyType == "Circumscribed")
            {
                global.pointCX = global.centerX;
                global.pointCY = global.centerY + Number(str);
                setRubberPoint("POLYGON_CIRCUMSCRIBE_POINT", global.pointCX, global.pointCY);
                vulcanize();
                endCommand();
            }
            else
            {
                error("POLYGON", translate("Polygon type is not Inscribed or Circumscribed."));
            }
        }
    }
    else if (global.mode == global.mode_SIDE_LEN)
    {
        todo("POLYGON", "Sidelength mode");
    }
}

//Command: Polyline

var global = {}; //Required
global.firstRun;
global.firstX;
global.firstY;
global.prevX;
global.prevY;
global.num;

void polyline_main()
{
    initCommand();
    clearSelection();
    global.firstRun = true;
    global.firstX = NaN;
    global.firstY = NaN;
    global.prevX = NaN;
    global.prevY = NaN;
    global.num = 0;
    setPromptPrefix(translate("Specify first point: "));
}

void polyline_click(float x, float y)
{
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
        setPromptPrefix(translate("Specify next point or [Undo]: "));
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
}

void polyline_context(std::string str)
{
    todo("POLYLINE", "context()");
}

void polyline_prompt(std::string str)
{
    if (global.firstRun)
    {
        var strList = str.split(",");
        if (isnan(strList[0]) || isnan(strList[1]))
        {
            alert(translate("Invalid point."));
            setPromptPrefix(translate("Specify first point: "));
        }
        else
        {
            global.firstRun = false;
            global.firstX = Number(strList[0]);
            global.firstY = Number(strList[1]);
            global.prevX = global.firstX;
            global.prevY = global.firstY;
            addRubber("POLYLINE");
            setRubberMode("POLYLINE");
            setRubberPoint("POLYLINE_POINT_0", global.firstX, global.firstY);
            setPromptPrefix(translate("Specify next point or [Undo]: "));
        }
    }
    else
    {
        if (str == "U" || str == "UNDO") //TODO: Probably should add additional qsTr calls here.
        {
            todo("POLYLINE", "prompt() for UNDO");
        }
        else
        {
            var strList = str.split(",");
            if (isnan(strList[0]) || isnan(strList[1]))
            {
                alert(translate("Point or option keyword required."));
                setPromptPrefix(translate("Specify next point or [Undo]: "));
            }
            else
            {
                var x = Number(strList[0]);
                var y = Number(strList[1]);
                global.num++;
                setRubberPoint("POLYLINE_POINT_" + global.num.toString(), x, y);
                setRubberText("POLYLINE_NUM_POINTS", global.num.toString());
                spareRubber("POLYLINE");
                global.prevX = x;
                global.prevY = y;
                setPromptPrefix(translate("Specify next point or [Undo]: "));
            }
        }
    }
}

//Command: Locate Point

void locate_point_main()
{
    initCommand();
    clearSelection();
    setPromptPrefix(translate("Specify point: "));
}

void click(float x, float y)
{
    appendPromptHistory();
    setPromptPrefix("X = " + x.toString() + ", Y = " + y.toString());
    appendPromptHistory();
    endCommand();
}

void context(std::string str)
{
    todo("LOCATEPOINT", "context()");
}

void prompt(std::string str)
{
    var strList = str.split(",");
    if (isnan(strList[0]) || isnan(strList[1]))
    {
        alert(translate("Invalid point."));
        setPromptPrefix(translate("Specify point: "));
    }
    else
    {
        appendPromptHistory();
        setPromptPrefix("X = " + strList[0].toString() + ", Y = " + strList[1].toString());
        appendPromptHistory();
        endCommand();
    }
}

typedef struct MoveUi_ {
    bool firstRun;
    float baseX;
    float baseY;
    float destX;
    float destY;
    float deltaX;
    float deltaY;
} MoveUi;

void move_main()
{
    initCommand();
    global.firstRun = true;
    global.baseX  = NaN;
    global.baseY  = NaN;
    global.destX  = NaN;
    global.destY  = NaN;
    global.deltaX = NaN;
    global.deltaY = NaN;

    if (numSelected() <= 0)
    {
        //TODO: Prompt to select objects if nothing is preselected
        alert(translate("Preselect objects before invoking the move command."));
        endCommand();
        messageBox("information", translate("Move Preselect"), translate("Preselect objects before invoking the move command."));
    }
    else
    {
        setPromptPrefix(translate("Specify base point: "));
    }
}

void move_click(float x, float y)
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
        setPromptPrefix(translate("Specify destination point: "));
    }
    else {
        global.destX = x;
        global.destY = y;
        global.deltaX = global.destX - global.baseX;
        global.deltaY = global.destY - global.baseY;
        moveSelected(global.deltaX, global.deltaY);
        previewOff();
        endCommand();
    }
}

void context(std::string str)
{
    todo("MOVE", "context()");
}

void prompt(std::string str)
{
    if (global.firstRun)
    {
        var strList = str.split(",");
        if (isnan(strList[0]) || isnan(strList[1]))
        {
            alert(translate("Invalid point."));
            setPromptPrefix(translate("Specify base point: "));
        }
        else
        {
            global.firstRun = false;
            global.baseX = Number(strList[0]);
            global.baseY = Number(strList[1]);
            addRubber("LINE");
            setRubberMode("LINE");
            setRubberPoint("LINE_START", global.baseX, global.baseY);
            previewOn("SELECTED", "MOVE", global.baseX, global.baseY, 0);
            setPromptPrefix(translate("Specify destination point: "));
        }
    }
    else
    {
        var strList = str.split(",");
        if (isnan(strList[0]) || isnan(strList[1]))
        {
            alert(translate("Invalid point."));
            setPromptPrefix(translate("Specify destination point: "));
        }
        else
        {
            global.destX = Number(strList[0]);
            global.destY = Number(strList[1]);
            global.deltaX = global.destX - global.baseX;
            global.deltaY = global.destY - global.baseY;
            moveSelected(global.deltaX, global.deltaY);
            previewOff();
            endCommand();
        }
    }
}

quickleader
var global = {}; //Required
global.x1;
global.y1;
global.x2;
global.y2;

//TODO: Adding the text is not complete yet.

void main()
{
    initCommand();
    clearSelection();
    global.x1 = NaN;
    global.y1 = NaN;
    global.x2 = NaN;
    global.y2 = NaN;
    setPromptPrefix(translate("Specify first point: "));
}

void click(float x, float y)
{
    if (isnan(global.x1))
    {
        global.x1 = x;
        global.y1 = y;
        addRubber("DIMLEADER");
        setRubberMode("DIMLEADER_LINE");
        setRubberPoint("DIMLEADER_LINE_START", global.x1, global.y1);
        appendPromptHistory();
        setPromptPrefix(translate("Specify second point: "));
    }
    else
    {
        global.x2 = x;
        global.y2 = y;
        setRubberPoint("DIMLEADER_LINE_END", global.x2, global.y2);
        vulcanize();
        appendPromptHistory();
        endCommand();
    }
}

void context(std::string str)
{
    todo("QUICKLEADER", "context()");
}

void prompt(std::string str)
{
    var strList = str.split(",");
    if (isnan(global.x1))
    {
        if (isnan(strList[0]) || isnan(strList[1]))
        {
            alert(translate("Requires two points."));
            setPromptPrefix(translate("Specify first point: "));
        }
        else
        {
            global.x1 = Number(strList[0]);
            global.y1 = Number(strList[1]);
            addRubber("DIMLEADER");
            setRubberMode("DIMLEADER_LINE");
            setRubberPoint("DIMLEADER_LINE_START", global.x1, global.y1);
            setPromptPrefix(translate("Specify second point: "));
        }
    }
    else
    {
        if (isnan(strList[0]) || isnan(strList[1]))
        {
            alert(translate("Requires two points."));
            setPromptPrefix(translate("Specify second point: "));
        }
        else
        {
            global.x2 = Number(strList[0]);
            global.y2 = Number(strList[1]);
            setRubberPoint("DIMLEADER_LINE_END", global.x2, global.y2);
            vulcanize();
            endCommand();
        }
    }
}

rectangle

var global = {}; //Required
global.newRect;
global.x1;
global.y1;
global.x2;
global.y2;

void main()
{
    initCommand();
    clearSelection();
    global.newRect = true;
    global.x1 = NaN;
    global.y1 = NaN;
    global.x2 = NaN;
    global.y2 = NaN;
    setPromptPrefix(translate("Specify first corner point or [Chamfer/Fillet]: "));
}

void click(float x, float y)
{
    if (global.newRect)
    {
        global.newRect = false;
        global.x1 = x;
        global.y1 = y;
        addRubber("RECTANGLE");
        setRubberMode("RECTANGLE");
        setRubberPoint("RECTANGLE_START", x, y);
        setPromptPrefix(translate("Specify other corner point or [Dimensions]: "));
    }
    else
    {
        global.newRect = true;
        global.x2 = x;
        global.y2 = y;
        setRubberPoint("RECTANGLE_END", x, y);
        vulcanize();
        endCommand();
    }
}

void context(std::string str)
{
    todo("RECTANGLE", "context()");
}

void prompt(std::string str)
{
    if (str == "C" || str == "CHAMFER") //TODO: Probably should add additional qsTr calls here.
    {
        todo("RECTANGLE", "prompt() for CHAMFER");
    }
    else if (str == "D" || str == "DIMENSIONS") //TODO: Probably should add additional qsTr calls here.
    {
        todo("RECTANGLE", "prompt() for DIMENSIONS");
    }
    else if (str == "F" || str == "FILLET") //TODO: Probably should add additional qsTr calls here.
    {
        todo("RECTANGLE", "prompt() for FILLET");
    }
    else {
        var strList = str.split(",");
        if (isnan(strList[0]) || isnan(strList[1])) {
            alert(translate("Invalid point."));
            setPromptPrefix(translate("Specify first point: "));
        }
        else {
            var x = Number(strList[0]);
            var y = Number(strList[1]);
            if (global.newRect) {
                global.newRect = false;
                global.x1 = x;
                global.y1 = y;
                addRubber("RECTANGLE");
                setRubberMode("RECTANGLE");
                setRubberPoint("RECTANGLE_START", x, y);
                setPromptPrefix(translate("Specify other corner point or [Dimensions]: "));
            }
            else {
                global.newRect = true;
                global.x2 = x;
                global.y2 = y;
                setRubberPoint("RECTANGLE_END", x, y);
                vulcanize();
                endCommand();
            }
        }
    }
}

//Command: RGB

var global = {}; //Required
global.mode;

//enums
global.mode_BACKGROUND = 0;
global.mode_CROSSHAIR  = 1;
global.mode_GRID       = 2;

void main()
{
    initCommand();
    clearSelection();
    global.mode = global.mode_BACKGROUND;
    setPromptPrefix(translate("Enter RED,GREEN,BLUE values for background or [Crosshair/Grid]: "));
}

void click(float x, float y)
{
    //Do Nothing, prompt only command.
}

void context(std::string str)
{
    todo("RGB", "context()");
}

void prompt(std::string str)
{
    if (global.mode == global.mode_BACKGROUND) {
        if (str == "C" || str == "CROSSHAIR") {
            //TODO: Probably should add additional qsTr calls here.
            global.mode = global.mode_CROSSHAIR;
            setPromptPrefix(translate("Specify crosshair color: "));
        }
        else if (str == "G" || str == "GRID") {
            //TODO: Probably should add additional qsTr calls here.
            global.mode = global.mode_GRID;
            setPromptPrefix(translate("Specify grid color: "));
        }
        else {
            var strList = str.split(",");
            var r = Number(strList[0]);
            var g = Number(strList[1]);
            var b = Number(strList[2]);
            if (!validRGB(r,g,b))
            {
                alert(translate("Invalid color. R,G,B values must be in the range of 0-255."));
                setPromptPrefix(translate("Specify background color: "));
            }
            else
            {
                setBackgroundColor(r,g,b);
                endCommand();
            }
        }
    }
    else if (global.mode == global.mode_CROSSHAIR)
    {
        var strList = str.split(",");
        var r = Number(strList[0]);
        var g = Number(strList[1]);
        var b = Number(strList[2]);
        if (!validRGB(r,g,b))
        {
            alert(translate("Invalid color. R,G,B values must be in the range of 0-255."));
            setPromptPrefix(translate("Specify crosshair color: "));
        }
        else
        {
            setCrossHairColor(r,g,b);
            endCommand();
        }
    }
    else if (global.mode == global.mode_GRID)
    {
        var strList = str.split(",");
        var r = Number(strList[0]);
        var g = Number(strList[1]);
        var b = Number(strList[2]);
        if (!validRGB(r,g,b))
        {
            alert(translate("Invalid color. R,G,B values must be in the range of 0-255."));
            setPromptPrefix(translate("Specify grid color: "));
        }
        else
        {
            setGridColor(r,g,b);
            endCommand();
        }
    }
}

void validRGB(r, g, b)
{
    if (isnan(r)) return false;
    if (isnan(g)) return false;
    if (isnan(b)) return false;
    if (r < 0 || r > 255) return false;
    if (g < 0 || g > 255) return false;
    if (b < 0 || b > 255) return false;
    return true;
}

//Command: Rotate

var global = {}; //Required
global.firstRun;
global.baseX;
global.baseY;
global.destX;
global.destY;
global.angle;

global.baseRX;
global.baseRY;
global.destRX;
global.destRY;
global.angleRef;
global.angleNew;

global.mode;

//enums
global.mode_NORMAL    = 0;
global.mode_REFERENCE = 1;

void main()
{
    initCommand();
    global.mode = global.mode_NORMAL;
    global.firstRun = true;
    global.baseX = NaN;
    global.baseY = NaN;
    global.destX = NaN;
    global.destY = NaN;
    global.angle = NaN;

    global.baseRX   = NaN;
    global.baseRY   = NaN;
    global.destRX   = NaN;
    global.destRY   = NaN;
    global.angleRef = NaN;
    global.angleNew = NaN;

    if (numSelected() <= 0) {
        //TODO: Prompt to select objects if nothing is preselected
        alert(translate("Preselect objects before invoking the rotate command."));
        endCommand();
        messageBox("information", translate("Rotate Preselect"), translate("Preselect objects before invoking the rotate command."));
    }
    else {
        setPromptPrefix(translate("Specify base point: "));
    }
}

void click(float x, float y)
{
    if (global.mode == global.mode_NORMAL) {
        if (global.firstRun) {
            global.firstRun = false;
            global.baseX = x;
            global.baseY = y;
            addRubber("LINE");
            setRubberMode("LINE");
            setRubberPoint("LINE_START", global.baseX, global.baseY);
            previewOn("SELECTED", "ROTATE", global.baseX, global.baseY, 0);
            appendPromptHistory();
            setPromptPrefix(translate("Specify rotation angle or [Reference]: "));
        }
        else {
            global.destX = x;
            global.destY = y;
            global.angle = calculateAngle(global.baseX, global.baseY, global.destX, global.destY);
            appendPromptHistory();
            rotateSelected(global.baseX, global.baseY, global.angle);
            previewOff();
            endCommand();
        }
    }
    else if (global.mode == global.mode_REFERENCE) {
        if (isnan(global.baseRX)) {
            global.baseRX = x;
            global.baseRY = y;
            appendPromptHistory();
            addRubber("LINE");
            setRubberMode("LINE");
            setRubberPoint("LINE_START", global.baseRX, global.baseRY);
            setPromptPrefix(translate("Specify second point: "));
        }
        else if (isnan(global.destRX)) {
            global.destRX = x;
            global.destRY = y;
            global.angleRef = calculateAngle(global.baseRX, global.baseRY, global.destRX, global.destRY);
            setRubberPoint("LINE_START", global.baseX, global.baseY);
            previewOn("SELECTED", "ROTATE", global.baseX, global.baseY, global.angleRef);
            appendPromptHistory();
            setPromptPrefix(translate("Specify the new angle: "));
        }
        else if (isnan(global.angleNew))
        {
            global.angleNew = calculateAngle(global.baseX, global.baseY, x, y);
            rotateSelected(global.baseX, global.baseY, global.angleNew - global.angleRef);
            previewOff();
            endCommand();
        }
    }
}

void context(std::string str)
{
    todo("ROTATE", "context()");
}

void prompt(std::string str)
{
    if (global.mode == global.mode_NORMAL)
    {
        if (global.firstRun)
        {
            var strList = str.split(",");
            if (isnan(strList[0]) || isnan(strList[1]))
            {
                alert(translate("Invalid point."));
                setPromptPrefix(translate("Specify base point: "));
            }
            else
            {
                global.firstRun = false;
                global.baseX = Number(strList[0]);
                global.baseY = Number(strList[1]);
                addRubber("LINE");
                setRubberMode("LINE");
                setRubberPoint("LINE_START", global.baseX, global.baseY);
                previewOn("SELECTED", "ROTATE", global.baseX, global.baseY, 0);
                setPromptPrefix(translate("Specify rotation angle or [Reference]: "));
            }
        }
        else
        {
            if (str == "R" || str == "REFERENCE") //TODO: Probably should add additional qsTr calls here.
            {
                global.mode = global.mode_REFERENCE;
                setPromptPrefix(translate("Specify the reference angle") + " {0.00}: ");
                clearRubber();
                previewOff();
            }
            else
            {
                if (isnan(str))
                {
                    alert(translate("Requires valid numeric angle, second point, or option keyword."));
                    setPromptPrefix(translate("Specify rotation angle or [Reference]: "));
                }
                else
                {
                    global.angle = Number(str);
                    rotateSelected(global.baseX, global.baseY, global.angle);
                    previewOff();
                    endCommand();
                }
            }
        }
    }
    else if (global.mode == global.mode_REFERENCE)
    {
        if (isnan(global.baseRX))
        {
            if (isnan(str))
            {
                var strList = str.split(",");
                if (isnan(strList[0]) || isnan(strList[1]))
                {
                    alert(translate("Requires valid numeric angle or two points."));
                    setPromptPrefix(translate("Specify the reference angle") + " {0.00}: ");
                }
                else
                {
                    global.baseRX = Number(strList[0]);
                    global.baseRY = Number(strList[1]);
                    addRubber("LINE");
                    setRubberMode("LINE");
                    setRubberPoint("LINE_START", global.baseRX, global.baseRY);
                    setPromptPrefix(translate("Specify second point: "));
                }
            }
            else
            {
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
                setPromptPrefix(translate("Specify the new angle: "));
            }
        }
        else if (isnan(global.destRX))
        {
            if (isnan(str))
            {
                var strList = str.split(",");
                if (isnan(strList[0]) || isnan(strList[1]))
                {
                    alert(translate("Requires valid numeric angle or two points."));
                    setPromptPrefix(translate("Specify second point: "));
                }
                else
                {
                    global.destRX = Number(strList[0]);
                    global.destRY = Number(strList[1]);
                    global.angleRef = calculateAngle(global.baseRX, global.baseRY, global.destRX, global.destRY);
                    previewOn("SELECTED", "ROTATE", global.baseX, global.baseY, global.angleRef);
                    setRubberPoint("LINE_START", global.baseX, global.baseY);
                    setPromptPrefix(translate("Specify the new angle: "));
                }
            }
            else
            {
                //The base and dest values are only set here to advance the command.
                global.baseRX = 0.0;
                global.baseRY = 0.0;
                global.destRX = 0.0;
                global.destRY = 0.0;
                //The reference angle is what we will use later.
                global.angleRef = Number(str);
                previewOn("SELECTED", "ROTATE", global.baseX, global.baseY, global.angleRef);
                setPromptPrefix(translate("Specify the new angle: "));
            }
        }
        else if (isnan(global.angleNew)) {
            if (isnan(str)) {
                var strList = str.split(",");
                if (isnan(strList[0]) || isnan(strList[1])) {
                    alert(translate("Requires valid numeric angle or second point."));
                    setPromptPrefix(translate("Specify the new angle: "));
                }
                else
                {
                    var x = Number(strList[0]);
                    var y = Number(strList[1]);
                    global.angleNew = calculateAngle(global.baseX, global.baseY, x, y);
                    rotateSelected(global.baseX, global.baseY, global.angleNew - global.angleRef);
                    previewOff();
                    endCommand();
                }
            }
            else
            {
                global.angleNew = Number(str);
                rotateSelected(global.baseX, global.baseY, global.angleNew - global.angleRef);
                previewOff();
                endCommand();
            }
        }
    }
}


var global = {}; //Required
global.test1;
global.test2;

void sandbox_main()
{
    initCommand();
    
    //Report number of pre-selected objects
    setPromptPrefix("Number of Objects Selected: " + numSelected().toString());
    appendPromptHistory();
    
    mirrorSelected(0,0,0,1);
    
    //selectAll();
    //rotateSelected(0,0,90);
    
    //Polyline & Polygon Testing
    
    var offsetX = 0.0;
    var offsetY = 0.0;
    
    var polylineArray = [];
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
    
    var polygonArray = [];
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
    

    endCommand();
}

void click(float x, float y)
{
}

void context(std::string str)
{
}

void prompt(std::string str)
{
}

typedef struct ScaleUi_ {
    global.firstRun;
    global.baseX;
    global.baseY;
    global.destX;
    global.destY;
    global.factor;

    global.baseRX;
    global.baseRY;
    global.destRX;
    global.destRY;
    global.factorRef;
    global.factorNew;

    global.mode;
} ScaleUi;

void scale_main()
{
    initCommand();
    global.mode = global.mode_NORMAL;
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
        alert(translate("Preselect objects before invoking the scale command."));
        endCommand();
        messageBox("information", translate("Scale Preselect"), translate("Preselect objects before invoking the scale command."));
    }
    else {
        setPromptPrefix(translate("Specify base point: "));
    }
}

void scale_click(float x, float y)
{
    if (global.mode == global.mode_NORMAL)
    {
        if (global.firstRun)
        {
            global.firstRun = false;
            global.baseX = x;
            global.baseY = y;
            addRubber("LINE");
            setRubberMode("LINE");
            setRubberPoint("LINE_START", global.baseX, global.baseY);
            previewOn("SELECTED", "SCALE", global.baseX, global.baseY, 1);
            appendPromptHistory();
            setPromptPrefix(translate("Specify scale factor or [Reference]: "));
        }
        else
        {
            global.destX = x;
            global.destY = y;
            global.factor = calculateDistance(global.baseX, global.baseY, global.destX, global.destY);
            appendPromptHistory();
            scaleSelected(global.baseX, global.baseY, global.factor);
            previewOff();
            endCommand();
        }
    }
    else if (global.mode == global.mode_REFERENCE) {
        if (isnan(global.baseRX)) {
            global.baseRX = x;
            global.baseRY = y;
            appendPromptHistory();
            addRubber("LINE");
            setRubberMode("LINE");
            setRubberPoint("LINE_START", global.baseRX, global.baseRY);
            setPromptPrefix(translate("Specify second point: "));
        }
        else if (isnan(global.destRX)) {
            global.destRX = x;
            global.destRY = y;
            global.factorRef = calculateDistance(global.baseRX, global.baseRY, global.destRX, global.destRY);
            if (global.factorRef <= 0.0)
            {
                global.destRX    = NaN;
                global.destRY    = NaN;
                global.factorRef = NaN;
                alert(translate("Value must be positive and nonzero."));
                setPromptPrefix(translate("Specify second point: "));
            }
            else
            {
                appendPromptHistory();
                setRubberPoint("LINE_START", global.baseX, global.baseY);
                previewOn("SELECTED", "SCALE", global.baseX, global.baseY, global.factorRef);
                setPromptPrefix(translate("Specify new length: "));
            }
        }
        else if (isnan(global.factorNew))
        {
            global.factorNew = calculateDistance(global.baseX, global.baseY, x, y);
            if (global.factorNew <= 0.0)
            {
                global.factorNew = NaN;
                alert(translate("Value must be positive and nonzero."));
                setPromptPrefix(translate("Specify new length: "));
            }
            else
            {
                appendPromptHistory();
                scaleSelected(global.baseX, global.baseY, global.factorNew/global.factorRef);
                previewOff();
                endCommand();
            }
        }
    }
}

void context(std::string str)
{
    todo("SCALE", "context()");
}

void prompt(std::string str)
{
    if (global.mode == global.mode_NORMAL) {
        if (global.firstRun) {
            var strList = str.split(",");
            if (isnan(strList[0]) || isnan(strList[1])) {
                alert(translate("Invalid point."));
                setPromptPrefix(translate("Specify base point: "));
            }
            else {
                global.firstRun = false;
                global.baseX = Number(strList[0]);
                global.baseY = Number(strList[1]);
                addRubber("LINE");
                setRubberMode("LINE");
                setRubberPoint("LINE_START", global.baseX, global.baseY);
                previewOn("SELECTED", "SCALE", global.baseX, global.baseY, 1);
                setPromptPrefix(translate("Specify scale factor or [Reference]: "));
            }
        }
        else
        {
            if (str == "R" || str == "REFERENCE") //TODO: Probably should add additional qsTr calls here.
            {
                global.mode = global.mode_REFERENCE;
                setPromptPrefix(translate("Specify reference length") + " {1}: ");
                clearRubber();
                previewOff();
            }
            else
            {
                if (isnan(str))
                {
                    alert(translate("Requires valid numeric distance, second point, or option keyword."));
                    setPromptPrefix(translate("Specify scale factor or [Reference]: "));
                }
                else
                {
                    global.factor = Number(str);
                    scaleSelected(global.baseX, global.baseY, global.factor);
                    previewOff();
                    endCommand();
                }
            }
        }
    }
    else if (global.mode == global.mode_REFERENCE)
    {
        if (isnan(global.baseRX))
        {
            if (isnan(str))
            {
                var strList = str.split(",");
                if (isnan(strList[0]) || isnan(strList[1]))
                {
                    alert(translate("Requires valid numeric distance or two points."));
                    setPromptPrefix(translate("Specify reference length") + " {1}: ");
                }
                else
                {
                    global.baseRX = Number(strList[0]);
                    global.baseRY = Number(strList[1]);
                    addRubber("LINE");
                    setRubberMode("LINE");
                    setRubberPoint("LINE_START", global.baseRX, global.baseRY);
                    setPromptPrefix(translate("Specify second point: "));
                }
            }
            else
            {
                //The base and dest values are only set here to advance the command.
                global.baseRX = 0.0;
                global.baseRY = 0.0;
                global.destRX = 0.0;
                global.destRY = 0.0;
                //The reference length is what we will use later.
                global.factorRef = Number(str);
                if (global.factorRef <= 0.0)
                {
                    global.baseRX    = NaN;
                    global.baseRY    = NaN;
                    global.destRX    = NaN;
                    global.destRY    = NaN;
                    global.factorRef = NaN;
                    alert(translate("Value must be positive and nonzero."));
                    setPromptPrefix(translate("Specify reference length") + " {1}: ");
                }
                else
                {
                    addRubber("LINE");
                    setRubberMode("LINE");
                    setRubberPoint("LINE_START", global.baseX, global.baseY);
                    previewOn("SELECTED", "SCALE", global.baseX, global.baseY, global.factorRef);
                    setPromptPrefix(translate("Specify new length: "));
                }
            }
        }
        else if (isnan(global.destRX)) {
            if (isnan(str)) {
                var strList = str.split(",");
                if (isnan(strList[0]) || isnan(strList[1])) {
                    alert(translate("Requires valid numeric distance or two points."));
                    setPromptPrefix(translate("Specify second point: "));
                }
                else {
                    global.destRX = Number(strList[0]);
                    global.destRY = Number(strList[1]);
                    global.factorRef = calculateDistance(global.baseRX, global.baseRY, global.destRX, global.destRY);
                    if (global.factorRef <= 0.0) {
                        global.destRX    = NaN;
                        global.destRY    = NaN;
                        global.factorRef = NaN;
                        alert(translate("Value must be positive and nonzero."));
                        setPromptPrefix(translate("Specify second point: "));
                    }
                    else
                    {
                        setRubberPoint("LINE_START", global.baseX, global.baseY);
                        previewOn("SELECTED", "SCALE", global.baseX, global.baseY, global.factorRef);
                        setPromptPrefix(translate("Specify new length: "));
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
                if (global.factorRef <= 0.0)
                {
                    global.destRX    = NaN;
                    global.destRY    = NaN;
                    global.factorRef = NaN;
                    alert(translate("Value must be positive and nonzero."));
                    setPromptPrefix(translate("Specify second point: "));
                }
                else
                {
                    setRubberPoint("LINE_START", global.baseX, global.baseY);
                    previewOn("SELECTED", "SCALE", global.baseX, global.baseY, global.factorRef);
                    setPromptPrefix(translate("Specify new length: "));
                }
            }
        }
        else if (isnan(global.factorNew)) {
            if (isnan(str)) {
                var strList = str.split(",");
                if (isnan(strList[0]) || isnan(strList[1])) {
                    alert(translate("Requires valid numeric distance or second point."));
                    setPromptPrefix(translate("Specify new length: "));
                }
                else {
                    var x = Number(strList[0]);
                    var y = Number(strList[1]);
                    global.factorNew = calculateDistance(global.baseX, global.baseY, x, y);
                    if (global.factorNew <= 0.0)
                    {
                        global.factorNew = NaN;
                        alert(translate("Value must be positive and nonzero."));
                        setPromptPrefix(translate("Specify new length: "));
                    }
                    else
                    {
                        scaleSelected(global.baseX, global.baseY, global.factorNew/global.factorRef);
                        previewOff();
                        endCommand();
                    }
                }
            }
            else {
                global.factorNew = Number(str);
                if (global.factorNew <= 0.0) {
                    global.factorNew = NaN;
                    alert(translate("Value must be positive and nonzero."));
                    setPromptPrefix(translate("Specify new length: "));
                }
                else {
                    scaleSelected(global.baseX, global.baseY, global.factorNew/global.factorRef);
                    previewOff();
                    endCommand();
                }
            }
        }
    }
}


void single_line_text_main()
{
    initCommand();
    clearSelection();
    global.text = "";
    global.position = {NaN, NaN};
    global.textJustify = "Left";
    global.textFont = textFont();
    global.textHeight = NaN;
    global.textRotation = NaN;
    global.mode = SINGLE_LINE_TEXT_MODE_SETGEOM;
    setPromptPrefix(translate("Current font: ") + "{" + global.textFont + "} " + translate("Text height: ") + "{" +  textSize() + "}");
    appendPromptHistory();
    setPromptPrefix(translate("Specify start point of text or [Justify/Setfont]: "));
}

void click(float x, float y)
{
    if (global.mode == global.mode_SETGEOM)
    {
        if (isnan(global.textX))
        {
            global.textX = x;
            global.textY = y;
            addRubber("LINE");
            setRubberMode("LINE");
            setRubberPoint("LINE_START", global.textX, global.textY);
            appendPromptHistory();
            setPromptPrefix(translate("Specify text height") + " {" + textSize() + "}: ");
        }
        else if (isnan(global.textHeight))
        {
            global.textHeight = calculateDistance(global.textX, global.textY, x, y);
            setTextSize(global.textHeight);
            appendPromptHistory();
            setPromptPrefix(translate("Specify text angle") + " {" + textAngle() + "}: ");
        }
        else if (isnan(global.textRotation))
        {
            global.textRotation = calculateAngle(global.textX, global.textY, x, y);
            setTextAngle(global.textRotation);
            appendPromptHistory();
            setPromptPrefix(translate("Enter text: "));
            global.mode = global.mode_RAPID;
            enablePromptRapidFire();
            clearRubber();
            addRubber("TEXTSINGLE");
            setRubberMode("TEXTSINGLE");
            setRubberPoint("TEXT_POINT", global.textX, global.textY);
            setRubberPoint("TEXT_HEIGHT_ROTATION", global.textHeight, global.textRotation);
            setRubberText("TEXT_FONT", global.textFont);
            setRubberText("TEXT_JUSTIFY", global.textJustify);
            setRubberText("TEXT_RAPID", global.text);
        }
        else
        {
            //Do nothing, as we are in rapidFire mode now.
        }
    }
}

void context(std::string str)
{
    todo("SINGLELINETEXT", "context()");
}

void prompt(std::string str)
{
    if (global.mode == global.mode_JUSTIFY)
    {
        if (str == "C" || str == "CENTER") //TODO: Probably should add additional qsTr calls here.
        {
            global.mode = global.mode_SETGEOM;
            global.textJustify = "Center";
            setRubberText("TEXT_JUSTIFY", global.textJustify);
            setPromptPrefix(translate("Specify center point of text or [Justify/Setfont]: "));
        }
        else if (str == "R" || str == "RIGHT") //TODO: Probably should add additional qsTr calls here.
        {
            global.mode = global.mode_SETGEOM;
            global.textJustify = "Right";
            setRubberText("TEXT_JUSTIFY", global.textJustify);
            setPromptPrefix(translate("Specify right-end point of text or [Justify/Setfont]: "));
        }
        else if (str == "A" || str == "ALIGN") //TODO: Probably should add additional qsTr calls here.
        {
            global.mode = global.mode_SETGEOM;
            global.textJustify = "Aligned";
            setRubberText("TEXT_JUSTIFY", global.textJustify);
            setPromptPrefix(translate("Specify start point of text or [Justify/Setfont]: "));
        }
        else if (str == "M" || str == "MIDDLE") //TODO: Probably should add additional qsTr calls here.
        {
            global.mode = global.mode_SETGEOM;
            global.textJustify = "Middle";
            setRubberText("TEXT_JUSTIFY", global.textJustify);
            setPromptPrefix(translate("Specify middle point of text or [Justify/Setfont]: "));
        }
        else if (str == "F" || str == "FIT") //TODO: Probably should add additional qsTr calls here.
        {
            global.mode = global.mode_SETGEOM;
            global.textJustify = "Fit";
            setRubberText("TEXT_JUSTIFY", global.textJustify);
            setPromptPrefix(translate("Specify start point of text or [Justify/Setfont]: "));
        }
        else if (str == "TL" || str == "TOPLEFT") //TODO: Probably should add additional qsTr calls here.
        {
            global.mode = global.mode_SETGEOM;
            global.textJustify = "Top Left";
            setRubberText("TEXT_JUSTIFY", global.textJustify);
            setPromptPrefix(translate("Specify top-left point of text or [Justify/Setfont]: "));
        }
        else if (str == "TC" || str == "TOPCENTER") //TODO: Probably should add additional qsTr calls here.
        {
            global.mode = global.mode_SETGEOM;
            global.textJustify = "Top Center";
            setRubberText("TEXT_JUSTIFY", global.textJustify);
            setPromptPrefix(translate("Specify top-center point of text or [Justify/Setfont]: "));
        }
        else if (str == "TR" || str == "TOPRIGHT") //TODO: Probably should add additional qsTr calls here.
        {
            global.mode = global.mode_SETGEOM;
            global.textJustify = "Top Right";
            setRubberText("TEXT_JUSTIFY", global.textJustify);
            setPromptPrefix(translate("Specify top-right point of text or [Justify/Setfont]: "));
        }
        else if (str == "ML" || str == "MIDDLELEFT") //TODO: Probably should add additional qsTr calls here.
        {
            global.mode = global.mode_SETGEOM;
            global.textJustify = "Middle Left";
            setRubberText("TEXT_JUSTIFY", global.textJustify);
            setPromptPrefix(translate("Specify middle-left point of text or [Justify/Setfont]: "));
        }
        else if (str == "MC" || str == "MIDDLECENTER") //TODO: Probably should add additional qsTr calls here.
        {
            global.mode = global.mode_SETGEOM;
            global.textJustify = "Middle Center";
            setRubberText("TEXT_JUSTIFY", global.textJustify);
            setPromptPrefix(translate("Specify middle-center point of text or [Justify/Setfont]: "));
        }
        else if (str == "MR" || str == "MIDDLERIGHT") //TODO: Probably should add additional qsTr calls here.
        {
            global.mode = global.mode_SETGEOM;
            global.textJustify = "Middle Right";
            setRubberText("TEXT_JUSTIFY", global.textJustify);
            setPromptPrefix(translate("Specify middle-right point of text or [Justify/Setfont]: "));
        }
        else if (str == "BL" || str == "BOTTOMLEFT") //TODO: Probably should add additional qsTr calls here.
        {
            global.mode = global.mode_SETGEOM;
            global.textJustify = "Bottom Left";
            setRubberText("TEXT_JUSTIFY", global.textJustify);
            setPromptPrefix(translate("Specify bottom-left point of text or [Justify/Setfont]: "));
        }
        else if (str == "BC" || str == "BOTTOMCENTER") //TODO: Probably should add additional qsTr calls here.
        {
            global.mode = global.mode_SETGEOM;
            global.textJustify = "Bottom Center";
            setRubberText("TEXT_JUSTIFY", global.textJustify);
            setPromptPrefix(translate("Specify bottom-center point of text or [Justify/Setfont]: "));
        }
        else if (str == "BR" || str == "BOTTOMRIGHT") //TODO: Probably should add additional qsTr calls here.
        {
            global.mode = global.mode_SETGEOM;
            global.textJustify = "Bottom Right";
            setRubberText("TEXT_JUSTIFY", global.textJustify);
            setPromptPrefix(translate("Specify bottom-right point of text or [Justify/Setfont]: "));
        }
        else
        {
            alert(translate("Invalid option keyword."));
            setPromptPrefix(translate("Text Justification Options [Center/Right/Align/Middle/Fit/TL/TC/TR/ML/MC/MR/BL/BC/BR]: "));
        }
    }
    else if (global.mode == global.mode_SETFONT)
    {
        global.mode = global.mode_SETGEOM;
        global.textFont = str;
        setRubberText("TEXT_FONT", global.textFont);
        setTextFont(global.textFont);
        setPromptPrefix(translate("Specify start point of text or [Justify/Setfont]: "));
    }
    else if (global.mode == global.mode_SETGEOM) {
        if (isnan(global.textX)) {
            if (str == "J" || str == "JUSTIFY") //TODO: Probably should add additional qsTr calls here.
            {
                global.mode = global.mode_JUSTIFY;
                setPromptPrefix(translate("Text Justification Options [Center/Right/Align/Middle/Fit/TL/TC/TR/ML/MC/MR/BL/BC/BR]: "));
            }
            else if (str == "S" || str == "SETFONT") //TODO: Probably should add additional qsTr calls here.
            {
                global.mode = global.mode_SETFONT;
                setPromptPrefix(translate("Specify font name: "));
            }
            else {
                var strList = str.split(",");
                if (isnan(strList[0]) || isnan(strList[1]))
                {
                    alert(translate("Point or option keyword required."));
                    setPromptPrefix(translate("Specify start point of text or [Justify/Setfont]: "));
                }
                else {
                    global.textX = Number(strList[0]);
                    global.textY = Number(strList[1]);
                    addRubber("LINE");
                    setRubberMode("LINE");
                    setRubberPoint("LINE_START", global.textX, global.textY);
                    setPromptPrefix(translate("Specify text height") + " {" + textSize() + "}: ");
                }
            }
        }
        else if (isnan(global.textHeight))
        {
            if (str == "")
            {
                global.textHeight = textSize();
                setPromptPrefix(translate("Specify text angle") + " {" + textAngle() + "}: ");
            }
            else if (isnan(str))
            {
                alert(translate("Requires valid numeric distance or second point."));
                setPromptPrefix(translate("Specify text height") + " {" + textSize() + "}: ");
            }
            else
            {
                global.textHeight = Number(str);
                setTextSize(global.textHeight);
                setPromptPrefix(translate("Specify text angle") + " {" + textAngle() + "}: ");
            }
        }
        else if (isnan(global.textRotation))
        {
            if (str == "")
            {
                global.textRotation = textAngle();
                setPromptPrefix(translate("Enter text: "));
                global.mode = global.mode_RAPID;
                enablePromptRapidFire();
                clearRubber();
                addRubber("TEXTSINGLE");
                setRubberMode("TEXTSINGLE");
                setRubberPoint("TEXT_POINT", global.textX, global.textY);
                setRubberPoint("TEXT_HEIGHT_ROTATION", global.textHeight, global.textRotation);
                setRubberText("TEXT_FONT", global.textFont);
                setRubberText("TEXT_JUSTIFY", global.textJustify);
                setRubberText("TEXT_RAPID", global.text);
            }
            else if (isnan(str))
            {
                alert(translate("Requires valid numeric angle or second point."));
                setPromptPrefix(translate("Specify text angle") + " {" + textAngle() + "}: ");
            }
            else
            {
                global.textRotation = Number(str);
                setTextAngle(global.textRotation);
                setPromptPrefix(translate("Enter text: "));
                global.mode = global.mode_RAPID;
                enablePromptRapidFire();
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
        else
        {
            //Do nothing, as we are in rapidFire mode now.
        }
    }
    else if (global.mode == global.mode_RAPID) {
        if (str == "RAPID_ENTER") {
            if (global.text == "") {
                endCommand();
            }
            else {
                vulcanize();
                endCommand(); //TODO: Rather than ending the command, calculate where the next line would be and modify the x/y to the new point
            }
        }
        else
        {
            global.text = str;
            setRubberText("TEXT_RAPID", global.text);
        }
    }
}


var global = {}; //Required
global.numPoints = 2048; //Default //TODO: min:64 max:8192
global.cx;
global.cy;
global.sx = 0.04; //Default
global.sy = 0.04; //Default
global.numPoints;
global.mode;

//enums
global.mode_NUM_POINTS = 0;
global.mode_XSCALE     = 1;
global.mode_YSCALE     = 2;

void snowflake_main()
{
    initCommand();
    clearSelection();
    global.cx = NaN;
    global.cy = NaN;
    global.mode = global.mode_NUM_POINTS;

    addRubber("POLYGON");
    setRubberMode("POLYGON");
    updateSnowflake(global.numPoints, global.sx, global.sy);
    spareRubber("POLYGON");
    endCommand();
}

void snowflake_click(float x, float y)
{
}

void snowflake_move(x, y)
{
}

void snowflake_context(std::string str)
{
    todo("SNOWFLAKE", "context()");
}

void snowflake_prompt(std::string str)
{
}

void updateSnowflake(numPts, xScale, yScale)
{
    var i;
    var t;
    var xx = NaN;
    var yy = NaN;
    var two_pi = 2*Math.PI;

    for (i = 0; i <= numPts; i++)
    {
        t = two_pi/numPts*i; 

//Snowflake Curve with t [0,2pi]

xx = 4/7*sin(20/11-318*t)+
3/13*sin(19/11-317*t)+
3/5*sin(21/16-316*t)+
1/6*sin(17/5-315*t)+
2/9*sin(20/19-314*t)+
5/9*sin(35/9-313*t)+
7/12*sin(9/8-310*t)+
5/16*sin(33/8-309*t)+
5/11*sin(31/11-308*t)+
4/7*sin(3/8-307*t)+
4/11*sin(9/8-306*t)+
7/8*sin(21/11-305*t)+
2/3*sin(55/13-304*t)+
5/9*sin(17/7-303*t)+
3/10*sin(3/13-302*t)+
4/11*sin(60/17-301*t)+
6/11*sin(48/11-300*t)+
9/19*sin(1/6-299*t)+
4/5*sin(19/11-298*t)+
7/13*sin(25/8-297*t)+
7/11*sin(19/7-296*t)+
1/2*sin(1-295*t)+
4/9*sin(24/11-294*t)+
1/3*sin(7/2-291*t)+
6/17*sin(15/13-290*t)+
11/17*sin(32/7-288*t)+
3/8*sin(33/8-287*t)+
4/7*sin(15/7-286*t)+
4/5*sin(48/11-284*t)+
6/7*sin(10/7-283*t)+
6/7*sin(20/11-282*t)+
3/8*sin(11/7-281*t)+
5/7*sin(23/6-280*t)+
1/21*sin(19/12-279*t)+
4/9*sin(1/5-278*t)+
5/8*sin(5/9-276*t)+
9/10*sin(2/3-274*t)+
5/8*sin(5/11-273*t)+
1/6*sin(9/2-272*t)+
12/25*sin(29/12-271*t)+
7/13*sin(59/15-270*t)+
5/7*sin(23/9-269*t)+
3/4*sin(9/2-268*t)+
5/11*sin(37/9-267*t)+
10/11*sin(11/7-266*t)+
1/3*sin(3/7-264*t)+
7/9*sin(33/17-262*t)+
5/8*sin(9/8-261*t)+
5/8*sin(38/13-260*t)+
11/21*sin(36/13-259*t)+
3/11*sin(1/29-258*t)+
8/15*sin(31/8-257*t)+
2/5*sin(3/13-256*t)+
1/2*sin(47/10-255*t)+
1/10*sin(33/10-254*t)+
2/5*sin(1/2-253*t)+
4/7*sin(33/7-252*t)+
6/17*sin(3/8-250*t)+
5/7*sin(25/9-249*t)+
7/9*sin(35/8-248*t)+
2/7*sin(81/20-247*t)+
5/8*sin(25/6-244*t)+
5/16*sin(11/21-243*t)+
11/13*sin(167/42-242*t)+
11/15*sin(18/5-241*t)+
13/14*sin(37/11-240*t)+
1/4*sin(20/9-239*t)+
9/14*sin(52/15-238*t)+
9/14*sin(17/14-237*t)+
6/13*sin(69/17-236*t)+
5/8*sin(74/21-235*t)+
7/15*sin(76/25-234*t)+
10/11*sin(15/8-232*t)+
5/11*sin(5/9-230*t)+
1/8*sin(8/3-229*t)+
5/9*sin(2/7-227*t)+
4/13*sin(32/9-226*t)+
2/3*sin(45/11-225*t)+
1/30*sin(53/15-223*t)+
7/11*sin(4/11-222*t)+
10/19*sin(31/13-221*t)+
sin(13/7-219*t)+
9/14*sin(33/7-216*t)+
2/3*sin(19/9-215*t)+
3/5*sin(27/11-214*t)+
9/11*sin(43/10-210*t)+
5/7*sin(13/8-209*t)+
5/9*sin(21/5-208*t)+
2/7*sin(14/9-206*t)+
9/8*sin(23/7-205*t)+
18/13*sin(11/9-203*t)+
7/4*sin(47/12-201*t)+
10/7*sin(8/9-200*t)+
7/10*sin(6/11-199*t)+
5/3*sin(7/6-198*t)+
19/11*sin(11/6-196*t)+
15/8*sin(9/8-195*t)+
8/17*sin(9/7-192*t)+
8/3*sin(39/10-191*t)+
23/10*sin(2/7-188*t)+
3/4*sin(3/5-187*t)+
7/12*sin(50/11-185*t)+
57/29*sin(4-184*t)+
9/8*sin(6/7-183*t)+
9/7*sin(15/13-182*t)+
5/13*sin(16/7-181*t)+
18/7*sin(5/14-180*t)+
17/9*sin(35/12-179*t)+
5/4*sin(5/7-178*t)+
22/23*sin(3/4-176*t)+
3/8*sin(48/13-175*t)+
15/11*sin(13/11-174*t)+
25/17*sin(23/5-173*t)+
18/11*sin(19/8-172*t)+
11/16*sin(5/3-170*t)+
39/38*sin(15/7-169*t)+
7/6*sin(36/11-166*t)+
15/11*sin(11/6-163*t)+
17/13*sin(3-162*t)+
11/9*sin(20/7-161*t)+
9/7*sin(35/9-160*t)+
7/6*sin(3/2-159*t)+
8/7*sin(9/10-158*t)+
12/25*sin(13/5-156*t)+
6/13*sin(25/13-154*t)+
9/13*sin(7/8-152*t)+
23/10*sin(33/14-151*t)+
8/11*sin(36/11-150*t)+
15/7*sin(26/7-149*t)+
6/5*sin(53/12-148*t)+
14/11*sin(3/2-147*t)+
9/8*sin(4/3-146*t)+
5/8*sin(18/13-145*t)+
15/7*sin(3/8-143*t)+
5/8*sin(5/6-142*t)+
6/7*sin(35/9-139*t)+
16/13*sin(1/2-138*t)+
9/4*sin(7/2-137*t)+
20/9*sin(15/8-135*t)+
11/8*sin(9/4-134*t)+
sin(19/10-133*t)+
22/7*sin(48/11-132*t)+
23/14*sin(1-131*t)+
19/9*sin(27/8-130*t)+
19/5*sin(20/7-129*t)+
18/5*sin(76/25-128*t)+
27/8*sin(4/5-126*t)+
37/8*sin(3/8-125*t)+
62/11*sin(11/3-124*t)+
49/11*sin(7/6-123*t)+
21/22*sin(23/12-122*t)+
223/74*sin(11/3-121*t)+
11/5*sin(19/5-120*t)+
13/4*sin(33/13-119*t)+
27/8*sin(22/5-117*t)+
24/7*sin(13/7-114*t)+
69/17*sin(18/17-113*t)+
10/9*sin(2/7-112*t)+
133/66*sin(12/7-111*t)+
2/5*sin(47/24-110*t)+
13/5*sin(11/6-108*t)+
16/7*sin(39/11-105*t)+
11/5*sin(25/9-104*t)+
151/50*sin(19/7-103*t)+
19/7*sin(12/5-101*t)+
26/7*sin(101/25-99*t)+
43/21*sin(41/14-98*t)+
13/3*sin(31/9-97*t)+
10/13*sin(1-95*t)+
17/7*sin(39/10-93*t)+
145/48*sin(3-92*t)+
37/6*sin(47/13-91*t)+
5/6*sin(36/13-89*t)+
9/4*sin(3/7-87*t)+
48/13*sin(26/17-86*t)+
7/3*sin(28/19-82*t)+
31/6*sin(8/7-81*t)+
36/7*sin(12/7-80*t)+
38/9*sin(25/9-79*t)+
17/2*sin(37/14-76*t)+
16/3*sin(19/20-75*t)+
81/16*sin(4/5-74*t)+
67/10*sin(19/15-73*t)+
40/11*sin(32/11-72*t)+
71/13*sin(21/20-71*t)+
68/15*sin(46/15-70*t)+
52/15*sin(27/10-69*t)+
57/14*sin(7/8-67*t)+
7/4*sin(42/13-66*t)+
39/11*sin(43/21-65*t)+
30/11*sin(33/8-64*t)+
7/5*sin(20/7-63*t)+
4/7*sin(13/14-62*t)+
39/10*sin(16/9-61*t)+
7/6*sin(137/34-59*t)+
16/13*sin(107/27-58*t)+
26/27*sin(17/5-57*t)+
4/3*sin(9/14-56*t)+
46/11*sin(5/3-55*t)+
11/6*sin(13/4-54*t)+
19/4*sin(17/5-53*t)+
19/7*sin(43/11-52*t)+
25/12*sin(30/7-51*t)+
15/7*sin(5/11-50*t)+
53/5*sin(21/13-49*t)+
62/13*sin(67/15-48*t)+
122/9*sin(48/13-47*t)+
20/13*sin(1-46*t)+
7/6*sin(32/7-43*t)+
12/7*sin(13/25-42*t)+
11/17*sin(9/10-40*t)+
11/9*sin(2-39*t)+
4/3*sin(19/7-38*t)+
12/5*sin(47/11-37*t)+
10/7*sin(12/7-36*t)+
108/17*sin(3/4-35*t)+
25/9*sin(19/5-34*t)+
7/13*sin(22/5-33*t)+
9/4*sin(13/11-32*t)+
181/15*sin(25/11-31*t)+
202/11*sin(57/13-29*t)+
2/11*sin(26/7-28*t)+
129/13*sin(38/15-25*t)+
13/6*sin(1/8-24*t)+
77/13*sin(11/8-23*t)+
19/6*sin(15/7-22*t)+
18/7*sin(29/10-21*t)+
9*sin(13/5-18*t)+
342/7*sin(11/6-17*t)+
3/5*sin(49/11-15*t)+
38/3*sin(19/7-14*t)+
994/9*sin(25/8-13*t)+
22/9*sin(49/12-10*t)+
97/9*sin(1/14-8*t)+
559/7*sin(47/14-7*t)+
19/13*sin(5/6-6*t)+
3*sin(57/17-4*t)+
28/5*sin(1-3*t)+
10/3*sin(22/7-2*t)+
1507/3*sin(29/8-t)-
1407/13*sin(5*t+8/11)-
15/2*sin(9*t+2/5)-
1193/9*sin(11*t+28/27)-
209/15*sin(12*t+2/5)-
116/15*sin(16*t+40/39)-
1105/33*sin(19*t+1/3)-
45/13*sin(20*t+7/6)-
91/46*sin(26*t+4/7)-
43/16*sin(27*t+12/11)-
46/13*sin(30*t+14/9)-
29/10*sin(41*t+3/14)-
31/11*sin(44*t+15/14)-
22/7*sin(45*t+10/7)-
7/8*sin(60*t+22/15)-
54/53*sin(68*t+5/4)-
214/15*sin(77*t+5/9)-
54/11*sin(78*t+1/13)-
47/6*sin(83*t+5/11)-
1/2*sin(84*t+8/7)-
2/3*sin(85*t+4/9)-
7/3*sin(88*t+7/6)-
15/4*sin(90*t+1/6)-
35/6*sin(94*t+17/18)-
77/26*sin(96*t+2/7)-
64/11*sin(100*t+34/23)-
13/6*sin(102*t+14/11)-
19/7*sin(106*t+5/6)-
13/6*sin(107*t+10/11)-
42/13*sin(109*t+8/7)-
69/35*sin(115*t+10/21)-
12/7*sin(116*t+17/16)-
8/3*sin(118*t+5/9)-
1/6*sin(127*t+17/12)-
13/7*sin(136*t+8/7)-
7/10*sin(140*t+7/5)-
15/7*sin(141*t+19/14)-
6/11*sin(144*t+5/16)-
3/2*sin(153*t+9/14)-
6/5*sin(155*t+3/10)-
3/8*sin(157*t+10/11)-
20/11*sin(164*t+19/14)-
7/5*sin(165*t+7/6)-
8/13*sin(167*t+20/13)-
7/8*sin(168*t+3/7)-
5/14*sin(171*t+16/13)-
22/7*sin(177*t+3/13)-
23/8*sin(186*t+7/8)-
13/7*sin(189*t+11/9)-
9/5*sin(190*t+32/21)-
27/28*sin(193*t+1)-
5/12*sin(194*t+1/2)-
44/43*sin(197*t+6/5)-
5/11*sin(202*t+1/5)-
8/7*sin(204*t+1/23)-
16/15*sin(207*t+7/10)-
1/2*sin(211*t+2/5)-
5/8*sin(212*t+3/5)-
10/13*sin(213*t+6/5)-
21/16*sin(217*t+4/3)-
11/5*sin(218*t+24/25)-
2/3*sin(220*t+5/9)-
13/10*sin(224*t+7/8)-
17/8*sin(228*t+1/9)-
3/7*sin(231*t+14/9)-
5/12*sin(233*t+9/11)-
3/5*sin(245*t+4/7)-
2/3*sin(246*t+15/11)-
3/8*sin(251*t+4/7)-
2/9*sin(263*t+19/20)-
1/2*sin(265*t+13/11)-
3/8*sin(275*t+3/2)-
17/35*sin(277*t+9/13)-
3/7*sin(285*t+3/11)-
9/10*sin(289*t+25/19)-
4/9*sin(292*t+20/13)-
12/25*sin(293*t+5/4)-
3/5*sin(311*t+9/8)-
33/32*sin(312*t+1/2);

yy = 3/7*sin(24/11-318*t)+
5/12*sin(3-317*t)+
5/14*sin(21/16-316*t)+
9/19*sin(31/9-315*t)+
2/9*sin(13/6-314*t)+
3/5*sin(9/7-312*t)+
2/5*sin(49/12-311*t)+
1/13*sin(30/7-310*t)+
4/13*sin(19/12-309*t)+
1/3*sin(32/7-307*t)+
5/8*sin(22/5-306*t)+
4/11*sin(25/11-305*t)+
8/15*sin(9/8-304*t)+
1/8*sin(35/9-303*t)+
3/5*sin(51/25-302*t)+
2/5*sin(9/8-301*t)+
4/7*sin(2/7-300*t)+
2/7*sin(50/11-299*t)+
3/13*sin(35/8-297*t)+
5/14*sin(14/5-295*t)+
8/13*sin(47/14-294*t)+
2/9*sin(25/8-293*t)+
8/17*sin(136/45-291*t)+
2/7*sin(17/7-290*t)+
3/5*sin(8/7-288*t)+
3/13*sin(19/8-286*t)+
6/11*sin(10/19-285*t)+
9/10*sin(121/40-283*t)+
8/5*sin(21/5-282*t)+
1/10*sin(87/25-281*t)+
7/13*sin(22/7-279*t)+
3/7*sin(8/5-278*t)+
4/5*sin(3/14-277*t)+
7/10*sin(19/13-276*t)+
1/5*sin(6/13-274*t)+
7/10*sin(20/9-273*t)+
1/3*sin(9/4-272*t)+
4/13*sin(47/11-271*t)+
18/17*sin(22/7-269*t)+
1/7*sin(31/9-268*t)+
7/10*sin(43/17-267*t)+
8/11*sin(24/7-266*t)+
5/8*sin(13/6-264*t)+
9/10*sin(17/13-262*t)+
4/11*sin(31/8-261*t)+
1/5*sin(66/19-260*t)+
1/10*sin(23/5-259*t)+
3/10*sin(66/19-255*t)+
1/8*sin(6/7-253*t)+
9/13*sin(16/5-252*t)+
3/7*sin(8/9-251*t)+
4/11*sin(30/13-250*t)+
7/11*sin(66/19-247*t)+
1/19*sin(2-246*t)+
1/4*sin(16/7-245*t)+
8/17*sin(41/10-244*t)+
15/16*sin(2/11-240*t)+
5/7*sin(19/18-239*t)+
1/6*sin(5/12-238*t)+
5/11*sin(16/17-236*t)+
3/10*sin(25/12-235*t)+
8/17*sin(16/7-233*t)+
5/8*sin(47/12-231*t)+
9/11*sin(11/8-230*t)+
3/11*sin(33/7-229*t)+
9/10*sin(20/7-226*t)+
4/9*sin(39/14-225*t)+
4/9*sin(10/9-224*t)+
6/7*sin(19/13-222*t)+
7/9*sin(29/7-221*t)+
8/11*sin(33/8-220*t)+
16/9*sin(2/7-219*t)+
25/14*sin(1/8-218*t)+
8/11*sin(5/9-217*t)+
9/11*sin(11/10-216*t)+
21/13*sin(27/7-215*t)+
3/7*sin(1/12-213*t)+
13/9*sin(15/16-212*t)+
23/8*sin(1/8-210*t)+
sin(32/11-209*t)+
9/13*sin(1/9-208*t)+
7/9*sin(33/10-206*t)+
2/3*sin(9/4-205*t)+
3/4*sin(1/2-204*t)+
3/13*sin(11/17-203*t)+
3/7*sin(31/12-202*t)+
19/12*sin(17/8-201*t)+
7/8*sin(75/19-200*t)+
6/5*sin(21/10-198*t)+
3/2*sin(7/5-194*t)+
28/27*sin(3/2-193*t)+
4/9*sin(16/5-192*t)+
22/13*sin(13/6-189*t)+
18/11*sin(19/10-188*t)+
sin(7/6-187*t)+
16/7*sin(13/11-186*t)+
9/5*sin(11/9-184*t)+
16/11*sin(2/5-183*t)+
10/13*sin(10/3-182*t)+
9/7*sin(38/9-181*t)+
45/13*sin(8/9-180*t)+
7/9*sin(35/8-179*t)+
2/3*sin(35/8-176*t)+
10/7*sin(6/19-175*t)+
40/13*sin(15/7-174*t)+
20/13*sin(1/2-173*t)+
3/11*sin(20/7-171*t)+
17/16*sin(50/11-169*t)+
2/9*sin(1/31-168*t)+
4/9*sin(7/2-165*t)+
1/12*sin(26/17-164*t)+
21/22*sin(27/26-163*t)+
13/12*sin(17/8-162*t)+
19/14*sin(39/10-160*t)+
18/11*sin(5/7-159*t)+
3/5*sin(15/14-158*t)+
11/9*sin(35/8-157*t)+
5/8*sin(30/7-156*t)+
3/2*sin(28/11-155*t)+
4/5*sin(5/11-151*t)+
25/19*sin(11/10-150*t)+
10/11*sin(11/14-148*t)+
13/9*sin(7/4-147*t)+
7/13*sin(19/6-146*t)+
1/5*sin(37/14-145*t)+
11/8*sin(42/13-144*t)+
20/11*sin(32/9-143*t)+
2/3*sin(22/5-141*t)+
10/11*sin(9/7-140*t)+
8/7*sin(23/9-138*t)+
5/2*sin(9/19-137*t)+
7/5*sin(193/48-136*t)+
5/8*sin(67/66-135*t)+
8/7*sin(7/15-134*t)+
13/6*sin(13/7-133*t)+
19/7*sin(16/5-132*t)+
16/7*sin(39/11-131*t)+
28/17*sin(69/35-130*t)+
84/17*sin(7/8-129*t)+
114/23*sin(10/9-128*t)+
29/11*sin(1/7-127*t)+
63/10*sin(65/32-124*t)+
74/17*sin(37/16-121*t)+
31/16*sin(35/11-120*t)+
19/5*sin(23/12-119*t)+
82/27*sin(27/7-118*t)+
49/11*sin(8/3-117*t)+
29/14*sin(63/16-116*t)+
9/13*sin(35/8-114*t)+
29/19*sin(5/4-113*t)+
13/7*sin(20/7-112*t)+
9/7*sin(11/23-111*t)+
19/8*sin(27/26-110*t)+
sin(4/7-109*t)+
119/40*sin(22/5-108*t)+
7/5*sin(47/46-107*t)+
5/3*sin(1/6-106*t)+
2*sin(14/5-105*t)+
7/3*sin(10/3-104*t)+
3/2*sin(15/4-103*t)+
19/11*sin(3/4-102*t)+
74/17*sin(13/10-99*t)+
98/33*sin(26/11-98*t)+
36/11*sin(13/3-97*t)+
43/12*sin(26/25-96*t)+
13/2*sin(3/13-95*t)+
6/7*sin(24/7-94*t)+
16/5*sin(6/5-93*t)+
5/7*sin(9/14-92*t)+
55/12*sin(27/14-90*t)+
15/11*sin(14/3-88*t)+
7/3*sin(7/10-87*t)+
11/4*sin(2/9-86*t)+
13/4*sin(35/12-84*t)+
26/9*sin(38/9-83*t)+
7/2*sin(5/7-82*t)+
31/8*sin(27/8-78*t)+
91/6*sin(35/8-77*t)+
37/5*sin(7/10-76*t)+
70/13*sin(17/11-73*t)+
76/25*sin(56/19-70*t)+
19/8*sin(17/8-68*t)+
59/13*sin(42/17-67*t)+
28/17*sin(49/13-64*t)+
9/7*sin(79/17-63*t)+
1/8*sin(7/11-62*t)+
39/8*sin(49/15-61*t)+
53/18*sin(33/8-59*t)+
9/7*sin(41/9-58*t)+
8/7*sin(65/14-57*t)+
10/11*sin(16/7-56*t)+
68/13*sin(42/13-55*t)+
21/10*sin(7/8-54*t)+
6/7*sin(41/14-53*t)+
31/11*sin(55/12-51*t)+
59/17*sin(27/7-50*t)+
124/9*sin(37/11-49*t)+
24/11*sin(3/5-48*t)+
65/6*sin(12/5-47*t)+
11/7*sin(49/11-45*t)+
13/25*sin(11/13-42*t)+
7/4*sin(5/8-40*t)+
43/42*sin(2/5-39*t)+
20/9*sin(4/7-38*t)+
19/8*sin(4/11-37*t)+
5/4*sin(15/4-36*t)+
1/5*sin(11/13-34*t)+
12/7*sin(23/5-32*t)+
409/34*sin(39/10-31*t)+
10/7*sin(5/2-30*t)+
180/11*sin(3-29*t)+
23/8*sin(53/12-26*t)+
71/8*sin(56/13-25*t)+
12/5*sin(10/21-24*t)+
10/3*sin(34/9-22*t)+
27/16*sin(12/11-21*t)+
49/6*sin(13/7-20*t)+
69/2*sin(19/14-19*t)+
475/9*sin(3/10-17*t)+
68/13*sin(57/28-16*t)+
40/17*sin(1/6-15*t)+
77/13*sin(29/11-12*t)+
4954/39*sin(15/4-11*t)+
1075/11*sin(4-5*t)+
191/24*sin(5/4-4*t)+
84/17*sin(2/7-3*t)-
12/5*sin(74*t)-
4/5*sin(166*t)-
1523/3*sin(t+12/11)-
25/3*sin(2*t+17/18)-
13/8*sin(6*t+1/9)-
5333/62*sin(7*t+9/7)-
56/9*sin(8*t+5/12)-
65/8*sin(9*t+2/5)-
106/9*sin(10*t+1/8)-
1006/9*sin(13*t+11/7)-
67/8*sin(14*t+6/5)-
25/8*sin(18*t+15/11)-
40/11*sin(23*t+1/16)-
4/7*sin(27*t+6/5)-
41/8*sin(28*t+7/12)-
8/5*sin(33*t+5/6)-
137/17*sin(35*t+4/5)-
29/12*sin(41*t+22/15)-
25/9*sin(43*t+6/7)-
12/25*sin(44*t+16/11)-
31/6*sin(46*t+4/3)-
19/5*sin(52*t+16/13)-
19/11*sin(60*t+8/17)-
16/7*sin(65*t+6/13)-
25/12*sin(66*t+11/13)-
8/9*sin(69*t+4/11)-
25/7*sin(71*t+7/5)-
11/10*sin(72*t+3/2)-
14/5*sin(75*t+7/9)-
107/14*sin(79*t+3/4)-
67/8*sin(80*t+2/11)-
161/27*sin(81*t+5/11)-
55/18*sin(85*t+3/7)-
161/40*sin(89*t+1/21)-
32/7*sin(91*t+38/25)-
sin(100*t+19/20)-
27/5*sin(101*t+2/13)-
26/9*sin(115*t+1/44)-
17/11*sin(122*t+1/16)-
87/22*sin(123*t+2/3)-
37/8*sin(125*t+9/11)-
10/7*sin(126*t+8/7)-
7/8*sin(139*t+3/5)-
3/7*sin(142*t+5/6)-
71/36*sin(149*t+5/16)-
7/6*sin(152*t+1/9)-
63/25*sin(153*t+29/19)-
27/20*sin(154*t+8/15)-
8/15*sin(161*t+12/13)-
5/3*sin(167*t+13/10)-
17/25*sin(170*t+3/5)-
10/9*sin(172*t+3/8)-
5/7*sin(177*t+5/8)-
1/2*sin(178*t+7/6)-
34/13*sin(185*t+5/8)-
11/13*sin(190*t+38/39)-
25/19*sin(191*t+11/8)-
11/12*sin(195*t+18/19)-
51/26*sin(196*t+2/7)-
14/9*sin(197*t+4/11)-
19/12*sin(199*t+1)-
19/11*sin(207*t+11/8)-
6/11*sin(211*t+1/20)-
11/7*sin(214*t+1/14)-
7/13*sin(223*t+8/11)-
3/5*sin(227*t+12/13)-
4/5*sin(228*t+29/19)-
11/10*sin(232*t+2/7)-
1/6*sin(234*t+7/11)-
sin(237*t+60/59)-
5/11*sin(241*t+7/8)-
1/2*sin(242*t+8/7)-
7/15*sin(243*t+15/16)-
5/8*sin(248*t+2/3)-
1/3*sin(249*t+4/11)-
2/3*sin(254*t+8/7)-
10/19*sin(256*t+14/11)-
4/9*sin(257*t+8/11)-
3/4*sin(258*t+3/7)-
sin(263*t+2/7)-
3/10*sin(265*t+1/28)-
1/2*sin(270*t+1)-
12/13*sin(275*t+5/8)-
1/4*sin(280*t+16/13)-
1/10*sin(284*t+5/8)-
13/25*sin(287*t+3/7)-
9/13*sin(289*t+3/5)-
22/23*sin(292*t+17/13)-
9/11*sin(296*t+17/11)-
3/7*sin(298*t+12/11)-
5/6*sin(308*t+1/2)-
7/15*sin(313*t+1/3);

        setRubberPoint("POLYGON_POINT_" + i.toString(), xx*xScale, yy*yScale);
    }

    setRubberText("POLYGON_NUM_POINTS", numPts.toString());
}

void star_main()
{
    initCommand();
    clearSelection();
    ui.numPoints = 5;
    global.cx = NaN;
    global.cy = NaN;
    global.x1 = NaN;
    global.y1 = NaN;
    global.x2 = NaN;
    global.y2 = NaN;
    global.mode = global.mode_NUM_POINTS;
    setPromptPrefix(translate("Enter number of star points") + " {" + global.numPoints.toString() + "}: ");
}

void star_click(float x, float y)
{
    if (global.mode == global.mode_NUM_POINTS) {
        //Do nothing, the prompt controls this.
    }
    else if (global.mode == global.mode_CENTER_PT) {
        global.cx = x;
        global.cy = y;
        global.mode = global.mode_RAD_OUTER;
        setPromptPrefix(translate("Specify outer radius of star: "));
        addRubber("POLYGON");
        setRubberMode("POLYGON");
        updateStar(global.cx, global.cy);
        enableMoveRapidFire();
    }
    else if (global.mode == global.mode_RAD_OUTER)
    {
        global.x1 = x;
        global.y1 = y;
        global.mode = global.mode_RAD_INNER;
        setPromptPrefix(translate("Specify inner radius of star: "));
        updateStar(global.x1, global.y1);
    }
    else if (global.mode == global.mode_RAD_INNER)
    {
        global.x2 = x;
        global.y2 = y;
        disableMoveRapidFire();
        updateStar(global.x2, global.y2);
        spareRubber("POLYGON");
        endCommand();
    }
}

void star_move(x, y)
{
    if (global.mode == global.mode_NUM_POINTS) {
        //Do nothing, the prompt controls this.
    }
    else if (global.mode == global.mode_CENTER_PT) {
        //Do nothing, prompt and click controls this.
    }
    else if (global.mode == global.mode_RAD_OUTER) {
        updateStar(x, y);
    }
    else if (global.mode == global.mode_RAD_INNER) {
        updateStar(x, y);
    }
}

void star_context(std::string str)
{
    todo("STAR", "context()");
}

void star_prompt(std::string str)
{
    if (global.mode == global.mode_NUM_POINTS) {
        if (str == "" && global.numPoints >= 3 && global.numPoints <= 1024)
        {
            setPromptPrefix(translate("Specify center point: "));
            global.mode = global.mode_CENTER_PT;
        }
        else {
            var tmp = Number(str);
            if (isnan(tmp) || !isInt(tmp) || tmp < 3 || tmp > 1024) {
                alert(translate("Requires an integer between 3 and 1024."));
                setPromptPrefix(translate("Enter number of star points") + " {" + global.numPoints.toString() + "}: ");
            }
            else {
                global.numPoints = tmp;
                setPromptPrefix(translate("Specify center point: "));
                global.mode = global.mode_CENTER_PT;
            }
        }
    }
    else if (global.mode == global.mode_CENTER_PT) {
        var strList = str.split(",");
        if (isnan(strList[0]) || isnan(strList[1])) {
            alert(translate("Invalid point."));
            setPromptPrefix(translate("Specify center point: "));
        }
        else {
            global.cx = Number(strList[0]);
            global.cy = Number(strList[1]);
            global.mode = global.mode_RAD_OUTER;
            setPromptPrefix(translate("Specify outer radius of star: "));
            addRubber("POLYGON");
            setRubberMode("POLYGON");
            updateStar(qsnapX(), qsnapY());
            enableMoveRapidFire();
        }
    }
    else if (global.mode == global.mode_RAD_OUTER) {
        var strList = str.split(",");
        if (isnan(strList[0]) || isnan(strList[1])) {
            alert(translate("Invalid point."));
            setPromptPrefix(translate("Specify outer radius of star: "));
        }
        else {
            global.point1.x = Number(strList[0]);
            global.point1.y = Number(strList[1]);
            global.mode = global.mode_RAD_INNER;
            setPromptPrefix(translate("Specify inner radius of star: "));
            updateStar(qsnapX(), qsnapY());
        }
    }
    else if (global.mode == global.mode_RAD_INNER) {
        var strList = str.split(",");
        if (isnan(strList[0]) || isnan(strList[1])) {
            alert(translate("Invalid point."));
            setPromptPrefix(translate("Specify inner radius of star: "));
        }
        else {
            global.x2 = Number(strList[0]);
            global.y2 = Number(strList[1]);
            disableMoveRapidFire();
            updateStar(global.x2, global.y2);
            spareRubber("POLYGON");
            endCommand();
        }
    }
}

void updateStar(float x, float y)
{
    var distOuter;
    var distInner;
    var angOuter;

    if (global.mode == STAR_MODE_RAD_OUTER) {
        angOuter = calculateAngle(global.cx, global.cy, x, y);
        distOuter = calculateDistance(global.cx, global.cy, x, y);
        distInner = distOuter/2.0;
    }
    else if (global.mode == STAR_MODE_RAD_INNER) {
        angOuter = calculateAngle(global.cx, global.cy, global.x1, global.y1);
        distOuter = calculateDistance(global.cx, global.cy, global.x1, global.y1);
        distInner = calculateDistance(global.cx, global.cy, x, y);
    }

    //Calculate the Star Points
    var angInc = 360.0/(global.numPoints*2);
    var odd = true;
    for (var i = 0; i < global.numPoints*2; i++) {
        var xx;
        var yy;
        if (odd) {
            xx = distOuter*cos((angOuter+(angInc*i))*Math.PI/180.0);
            yy = distOuter*sin((angOuter+(angInc*i))*Math.PI/180.0);
        }
        else {
            xx = distInner*cos((angOuter+(angInc*i))*Math.PI/180.0);
            yy = distInner*sin((angOuter+(angInc*i))*Math.PI/180.0);
        }
        odd = !odd;
        setRubberPoint("POLYGON_POINT_" + i.toString(), global.cx + xx, global.cy + yy);
    }
    setRubberText("POLYGON_NUM_POINTS", (global.numPoints*2 - 1).toString());
}

void syswindows_main()
{
    initCommand();
    clearSelection();
    setPromptPrefix(translate("Enter an option [Cascade/Tile]: "));
}

void syswindows_prompt(std::string str)
{
    if (str == "C" || str == "CASCADE") {
        windowCascade();
        endCommand();
    }
    else if (str == "T" || str == "TILE") {
        windowTile();
        endCommand();
    }
    else {
        alert(translate("Invalid option keyword."));
        setPromptPrefix(translate("Enter an option [Cascade/Tile]: "));
    }
}

#endif