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

#include "core.h"

#include <stdlib.h>

EmbVector
embVector_make(EmbReal x, EmbReal y)
{
    EmbVector v;
    v.x = x;
    v.y = y;
    return v;
}

GeometryData*
geometry_init(int type)
{
    GeometryData *g = malloc(sizeof(GeometryData));
    switch (type) {
    case MODE_ARC:
        break;

    case MODE_CIRCLE_1P_RAD:
        break;

    case MODE_CIRCLE_1P_DIA:
        break;

    case MODE_CIRCLE_2P:
        break;

    case MODE_CIRCLE_3P:
        break;

    case MODE_CIRCLE_TTR:
        break;

    case MODE_CIRCLE_TTR_SET_POINT_2:
        break;

    case MODE_CIRCLE_TTR_SET_POINT_3:
        break;

    case MODE_ELLIPSE:
        break;

    case MODE_RECTANGLE:
        break;

    case MODE_STAR_INIT:
        clear_selection();
        g->numPoints = 5;
        g->center = embVector_make(0.0f, 0.0f);
        g->point1 = embVector_make(1.0f, 1.0f);
        g->point2 = embVector_make(2.0f, 2.0f);
        g->mode = MODE_STAR_NUM_POINTS;
        prompt_output(translate("Enter number of star points {5}: "));
        break;

    case MODE_STAR_NUM_POINTS:
        break;

    case MODE_STAR_CENTER_PT:
        break;

    case MODE_STAR_RAD_INNER:
        break;

    case MODE_STAR_RAD_OUTER:
        break;

    case MODE_POLYGON_NUM_SIDES:
        break;

    case MODE_POLYGON_POLYTYPE:
        break;

    case MODE_POLYGON_DISTANCE:
        break;

    case MODE_POLYGON_CENTER_PT:
        break;

    case MODE_POLYGON_INSCRIBE:
        break;

    case MODE_POLYGON_CIRCUMSCRIBE:
        break;

    case MODE_POLYGON_SIDE_LEN:
        break;

    /*
    case MODE_SNOWFLAKE:
        object_script(snowflake_init);
        break;
    */

    default:
        break;
    }
    return g;
}

void
geometry_free(GeometryData *g)
{
    free(g);
}

void
geometry_left_click(GeometryData *g, EmbVector v)
{
    switch (g->mode) {
    case MODE_ARC:
        break;

    case MODE_CIRCLE_1P_RAD:
        break;

    case MODE_CIRCLE_1P_DIA:
        break;

    case MODE_CIRCLE_2P:
        break;

    case MODE_CIRCLE_3P:
        break;

    case MODE_CIRCLE_TTR:
        break;

    case MODE_CIRCLE_TTR_SET_POINT_2:
        break;

    case MODE_CIRCLE_TTR_SET_POINT_3:
        break;

    case MODE_ELLIPSE:
        break;

    case MODE_RECTANGLE:
        break;

    case MODE_STAR_NUM_POINTS: {
        /* Do nothing, the prompt controls this. */
        break;
    }

    /*
    case MODE_STAR_CENTER_PT: {
        g->center = v;
        g->mode = MODE_STAR_RAD_OUTER;
        prompt_output(translate("Specify outer radius of star: "));
        addRubber("POLYGON");
        setRubberMode("POLYGON");
        updateStar(properties, center);
        actuator("enable move-rapid-fire");
        break;
    }

    case MODE_STAR_RAD_OUTER: {
        point1 = v;
        mode.s = "MODE_RAD_INNER";
        prompt_output(translate("Specify inner radius of star: "));
        updateStar(point1);
        break;
    }

    case MODE_STAR_RAD_INNER: {
        point2 = v;
        actuator("disable move-rapid-fire");
        updateStar(point2);
        spareRubber("POLYGON");
        actuator("end");
        break;
    }
    */

    case MODE_POLYGON_NUM_SIDES:
    case MODE_POLYGON_POLYTYPE:
    case MODE_POLYGON_DISTANCE: {
        //Do nothing, the prompt controls this.
        break;
    }

    case MODE_POLYGON_CENTER_PT: {
        g->center = v;
        g->mode = MODE_POLYGON_POLYTYPE;
        actuator("append-prompt-history");
        char msg[MAX_STRING_LENGTH];
        sprintf(msg, "set-prompt-prefix %s {%s}: ",
            translate("Specify polygon type [Inscribed in circle/Circumscribed around circle]"),
            g->polyType);
        actuator(msg);
        break;
    }

    case MODE_POLYGON_INSCRIBE: {
        g->pointI = v;
        /* setRubberPoint("POLYGON_INSCRIBE_POINT", g->pointI); */
        actuator("vulcanize");
        actuator("append-prompt-history");
        actuator("end");
        break;
    }

    case MODE_POLYGON_CIRCUMSCRIBE: {
        g->pointC = v;
        /* setRubberPoint("POLYGON_CIRCUMSCRIBE_POINT", g->pointC); */
        actuator("vulcanize");
        actuator("append-prompt-history");
        actuator("end");
        break;
    }

    case MODE_POLYGON_SIDE_LEN: {
        actuator("todo POLYGON Sidelength mode");
        break;
    }

    default:
        break;
    }
}

void
geometry_prompt(
    GeometryData *g,
    char input[MAX_STRING_LENGTH],
    char output[MAX_STRING_LENGTH])
{
    switch (g->mode) {
    case MODE_ARC:
        break;

    case MODE_CIRCLE_1P_RAD:
        break;

    case MODE_CIRCLE_1P_DIA:
        break;

    case MODE_CIRCLE_2P:
        break;

    case MODE_CIRCLE_3P:
        break;

    case MODE_CIRCLE_TTR:
        break;

    case MODE_CIRCLE_TTR_SET_POINT_2:
        break;

    case MODE_CIRCLE_TTR_SET_POINT_3:
        break;

    case MODE_ELLIPSE:
        break;

    case MODE_RECTANGLE:
        break;

    case MODE_STAR_NUM_POINTS:
        break;

    case MODE_STAR_CENTER_PT:
        break;

    case MODE_STAR_RAD_INNER:
        break;

    case MODE_STAR_RAD_OUTER:
        break;

    case MODE_POLYGON_NUM_SIDES:
        break;

    case MODE_POLYGON_POLYTYPE:
        break;

    case MODE_POLYGON_DISTANCE:
        break;

    case MODE_POLYGON_CENTER_PT:
        break;

    case MODE_POLYGON_INSCRIBE:
        break;

    case MODE_POLYGON_CIRCUMSCRIBE:
        break;

    case MODE_POLYGON_SIDE_LEN:
        break;

    default:
        break;
    }
}

void
geometry_context(
    void *m,
    GeometryData *g,
    char output[MAX_STRING_LENGTH])
{
    switch (g->mode) {
    case MODE_ARC: {
        break;
    }

    case MODE_CIRCLE_1P_RAD:
    case MODE_CIRCLE_1P_DIA:
    case MODE_CIRCLE_2P:
    case MODE_CIRCLE_3P:
    case MODE_CIRCLE_TTR:
    case MODE_CIRCLE_TTR_SET_POINT_2:
    case MODE_CIRCLE_TTR_SET_POINT_3: {
        break;
    }

    case MODE_DISTANCE: {
        actuator("todo DISTANCE context()");
        break;
    }

/*
MODE_LINE
MODE_LOCATEPOINT
MODE_MOVE
MODE_PATH
MODE_POINT
MODE_POLYLINE
MODE_QUICKLEADER
MODE_RGB
MODE_ROTATE
MODE_SCALE
MODE_SINGLELINETEXT

    actuator("todo LINE context()");
    actuator("todo LOCATEPOINT context()");
    actuator("todo MOVE context()");
    actuator("todo PATH context()");
    actuator("todo POINT context()");
    actuator("todo POLYLINE context()");
    actuator("todo QUICKLEADER context()");
    actuator("todo RGB context()");
    actuator("todo ROTATE context()");
    actuator("todo SCALE context()");
    actuator("todo SINGLELINETEXT context()");
*/

    case MODE_ELLIPSE: {
        actuator("todo ELLIPSE context()");
        break;
    }

    case MODE_RECTANGLE: {
        actuator("todo RECTANGLE context()");
        break;
    }

    case MODE_STAR_NUM_POINTS:
    case MODE_STAR_CENTER_PT:
    case MODE_STAR_RAD_INNER:
    case MODE_STAR_RAD_OUTER: {
        actuator("todo STAR context()");
        break;
    }

    case MODE_POLYGON_NUM_SIDES:
    case MODE_POLYGON_POLYTYPE:
    case MODE_POLYGON_DISTANCE:
    case MODE_POLYGON_CENTER_PT:
    case MODE_POLYGON_INSCRIBE:
    case MODE_POLYGON_CIRCUMSCRIBE:
    case MODE_POLYGON_SIDE_LEN: {
        actuator("todo POLYGON context()");
        break;
    }

    default:
        break;
    }
}

void
geometry_update(GeometryData *g)
{
    switch (g->mode) {
    case MODE_ARC: {
        break;
    }

    case MODE_CIRCLE_1P_RAD:
    case MODE_CIRCLE_1P_DIA:
    case MODE_CIRCLE_2P:
    case MODE_CIRCLE_3P:
    case MODE_CIRCLE_TTR:
    case MODE_CIRCLE_TTR_SET_POINT_2:
    case MODE_CIRCLE_TTR_SET_POINT_3: {
        break;
    }

    case MODE_DISTANCE: {
        actuator("todo DISTANCE context()");
        break;
    }

    case MODE_ELLIPSE: {
        actuator("todo ELLIPSE context()");
        break;
    }

    case MODE_RECTANGLE: {
        actuator("todo RECTANGLE context()");
        break;
    }

    case MODE_STAR_NUM_POINTS:
    case MODE_STAR_CENTER_PT:
    case MODE_STAR_RAD_INNER:
    case MODE_STAR_RAD_OUTER: {
        actuator("todo STAR context()");
        break;
    }

    case MODE_POLYGON_NUM_SIDES:
    case MODE_POLYGON_POLYTYPE:
    case MODE_POLYGON_DISTANCE:
    case MODE_POLYGON_CENTER_PT:
    case MODE_POLYGON_INSCRIBE:
    case MODE_POLYGON_CIRCUMSCRIBE:
    case MODE_POLYGON_SIDE_LEN: {
        actuator("todo POLYGON context()");
        break;
    }

    default:
        break;
    }
}

void
geometry_set_flag(GeometryData *g, uint64_t flag)
{

}

void
geometry_unset_flag(GeometryData *g, uint64_t flag)
{

}


/* These may be able to be moved to libembroidery.
 */
EmbVector
get_vector(GeometryData *g, int64_t id)
{
    EmbVector v;
    v.x = 0.0;
    v.y = 0.0;
    switch (id) {
    case VECTOR_ARC_START_POINT: {
        break;
    }

    case VECTOR_ARC_MID_POINT: {
        break;
    }

    case VECTOR_ARC_END_POINT: {
        break;
    }

    default:
        break;
    }
    return v;
}

/* GET REAL!
 */
EmbReal
get_real(GeometryData *g, int64_t id)
{
    EmbReal r;
    switch (id) {
    case REAL_ARC_RADIUS: {
        break;
    }

    case REAL_ARC_START_ANGLE: {
        break;
    }

    case REAL_ARC_END_ANGLE: {
        break;
    }

    case REAL_ARC_DIAMETER: {
        break;
    }

    case REAL_ARC_AREA: {
        break;
    }

    case REAL_ARC_CIRCUMFERENCE: {
        break;
    }

    case REAL_ARC_LENGTH: {
        break;
    }

    case REAL_CHORD: {
        break;
    }

    case REAL_TEXT: {
        break;
    }

    case REAL_TEXT_FONT: {
        break;
    }

    case REAL_TEXT_JUSTIFY: {
        break;
    }

    case REAL_TEXT_SIZE: {
        break;
    }

    case REAL_RADIUS_MAJOR: {
        break;
    }

    case REAL_RADIUS_MINOR: {
        break;
    }

    case REAL_DIAMETER_MAJOR: {
        break;
    }

    case REAL_DIAMETER_MINOR: {
        break;
    }

    case REAL_LENGTH: {
        break;
    }

    case REAL_AREA: {
        break;
    }

    case REAL_ANGLE: {
        break;
    }

    default:
        break;
    }
    return r;
}

int32_t
get_int(GeometryData *g, int64_t id)
{
    int64_t i;
    switch (id) {
    case VECTOR_ARC_START_POINT: {
        break;
    }

    case VECTOR_ARC_MID_POINT: {
        break;
    }

    case VECTOR_ARC_END_POINT: {
        break;
    }

    default:
        break;
    }
    return i;
}

char *
get_str(GeometryData *g, int64_t id)
{
    char s[MAX_STRING_LENGTH];
    s[0] = 0;
    switch (id) {
    case VECTOR_ARC_START_POINT: {
        break;
    }

    case VECTOR_ARC_MID_POINT: {
        break;
    }

    case VECTOR_ARC_END_POINT: {
        break;
    }

    default:
        break;
    }
    return s;
}

/* Set */
void
set_vector(GeometryData *g, int64_t id, EmbVector v)
{
    switch (id) {
    case VECTOR_ARC_START_POINT: {
        break;
    }

    case VECTOR_ARC_MID_POINT: {
        break;
    }

    case VECTOR_ARC_END_POINT: {
        break;
    }

    default:
        break;
    }
}

void
set_real(GeometryData *g, int64_t id, EmbReal r)
{
    switch (id) {
    case REAL_ARC_RADIUS: {
        break;
    }

    case REAL_ARC_START_ANGLE: {
        break;
    }

    case REAL_ARC_END_ANGLE: {
        break;
    }

    case REAL_ARC_DIAMETER: {
        break;
    }

    case REAL_ARC_AREA: {
        break;
    }

    case REAL_ARC_CIRCUMFERENCE: {
        break;
    }

    case REAL_ARC_LENGTH: {
        break;
    }

    case REAL_CHORD: {
        break;
    }

    case REAL_TEXT: {
        break;
    }

    case REAL_TEXT_FONT: {
        break;
    }

    case REAL_TEXT_JUSTIFY: {
        break;
    }

    case REAL_TEXT_SIZE: {
        break;
    }

    case REAL_RADIUS_MAJOR: {
        break;
    }

    case REAL_RADIUS_MINOR: {
        break;
    }

    case REAL_DIAMETER_MAJOR: {
        break;
    }

    case REAL_DIAMETER_MINOR: {
        break;
    }

    case REAL_LENGTH: {
        break;
    }

    case REAL_AREA: {
        break;
    }

    case REAL_ANGLE: {
        break;
    }

    default:
        break;
    }
}

void
set_int(GeometryData *g, int64_t id, int32_t i)
{
    switch (id) {

    default:
        break;
    }
}

void
set_str(GeometryData *g, int64_t id, char *str)
{
    switch (id) {

    default:
        break;
    }
}

#if 0

/* . */
void
Geometry::distance_click(EmbVector v)
{
    if (std::isnan(point1.x)) {
        point1 = v;
        addRubber("LINE");
        setRubberMode("LINE");
        setRubberPoint("LINE_START", point1);
        prompt_output(translate("Specify second point: "));
    }
    else {
        point2 = v;
        reportDistance();
        actuator("end");
    }
}

/* . */
void
Geometry::distance_prompt(char *args)
{
    EmbReal strList = str.split(",");
    if (std::isnan(point1.x)) {
        if (std::isnan(strList[0]) || std::isnan(strList[1])) {
            alert(translate("Requires numeric distance or two points."));
            prompt_output(translate("Specify first point: "));
        }
        else {
            point1.x = atof(strList[0]);
            point1.y = atof(strList[1]);
            addRubber("LINE");
            setRubberMode("LINE");
            setRubberPoint("LINE_START", point1.x, point1.y);
            prompt_output(translate("Specify second point: "));
        }
    }
    else {
        if (std::isnan(strList[0]) || std::isnan(strList[1])) {
            alert(translate("Requires numeric distance or two points."));
            prompt_output(translate("Specify second point: "));
        }
        else {
            point2.x = atof(strList[0]);
            point2.y = atof(strList[1]);
            reportDistance();
            actuator("end");
        }
    }
}

/* Cartesian Coordinate System reported:
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
    actuator("set-prompt-prefix " + tr("Delta X") + " = " + dx.toString() + ", " + tr("Delta Y") + " = " + dy.toString());
}

/* Update the dolphin object. */
void
Geometry::update_dolphin(void)
{
    for (int i = 0; i <= g->numPoints; i++) {
        EmbReal t = (2.0 * CONSTANT_PI) / g->numPoints * i;
        EmbVector v;
        v.x = fourier_series(t, dolphin_x);
        v.y = fourier_series(t, dolphin_y);

        setRubberPoint("POLYGON_POINT_" + i.toString(), xx*xScale, yy*yScale);
    }

    setRubberText("POLYGON_NUM_POINTS", g->numPoints);
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
            setRubberPoint("ELLIPSE_LINE_POINT1", g->point1);
            prompt_output(translate("Specify first axis end point: "));
        }
        else if (std::isnan(point2.x)) {
            point2 = v;
            center.x = atof((point1.x + point2.x)/2.0);
            center.y = atof((point1.y + point2.y)/2.0);
            width = calculateDistance(point1.x, point1.y, point2.x, point2.y);
            rot = calculateAngle(point1.x, point1.y, point2.x, point2.y);
            setRubberMode("ELLIPSE_MAJORDIAMETER_MINORRADIUS");
            setRubberPoint("ELLIPSE_AXIS1_POINT1", point1.x, point1.y);
            setRubberPoint("ELLIPSE_AXIS1_POINT2", point2.x, point2.y);
            setRubberPoint("ELLIPSE_CENTER", center.x, center.y);
            setRubberPoint("ELLIPSE_WIDTH", width, 0);
            setRubberPoint("ELLIPSE_ROT", rot, 0);
            prompt_output(translate("Specify second axis end point or [Rotation]: ");
        }
        else if (std::isnan(point3.x)) {
            point3.x = x;
            point3.y = y;
            height = perpendicularDistance(point3.x, point3.y, point1.x, point1.y, point2.x, point2.y)*2.0;
            setRubberPoint("ELLIPSE_AXIS2_POINT2", point3.x, point3.y);
            vulcanize();
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
            center.x = point1.x;
            center.y = point1.y;
            addRubber("ELLIPSE");
            setRubberMode("ELLIPSE_LINE");
            setRubberPoint("ELLIPSE_LINE_POINT1", point1.x, point1.y);
            setRubberPoint("ELLIPSE_CENTER", center.x, center.y);
            prompt_output(translate("Specify first axis end point: ");
        }
        else if (std::isnan(point2.x)) {
            point2.x = x;
            point2.y = y;
            width = calculateDistance(center.x, center.y, point2.x, point2.y)*2.0;
            rot = calculateAngle(point1.x, point1.y, point2.x, point2.y);
            setRubberMode("ELLIPSE_MAJORRADIUS_MINORRADIUS");
            setRubberPoint("ELLIPSE_AXIS1_POINT2", point2.x, point2.y);
            setRubberPoint("ELLIPSE_WIDTH", width, 0);
            setRubberPoint("ELLIPSE_ROT", rot, 0);
            prompt_output(translate("Specify second axis end point or [Rotation]: ");
        }
        else if (std::isnan(point3.x)) {
            point3.x = x;
            point3.y = y;
            height = perpendicularDistance(point3.x, point3.y, center.x, center.y, point2.x, point2.y)*2.0;
            setRubberPoint("ELLIPSE_AXIS2_POINT2", point3.x, point3.y);
            vulcanize();
            actuator("end");
        }
        else {
            error("ELLIPSE", tr("This should never happen."));
        }
    }
    case MODE_ELLIPSE_ROTATION: {
        if (std::isnan(point1.x)) {
            error("ELLIPSE", tr("This should never happen."));
        }
        else if (std::isnan(point2.x)) {
            error("ELLIPSE", tr("This should never happen."));
        }
        else if (std::isnan(point3.x)) {
            EmbReal angle = calculateAngle(center.x, center.y, x, y);
            height = cos(angle*PI/180.0)*width;
            addEllipse(center.x, center.y, width, height, rot, false);
            actuator("end");
        }
    }
}

/* . */
void
Geometry::ellipse_prompt(char *args)
{
    switch (mode) {
    case MODE_MAJORDIAMETER_MINORRADIUS: {
        if (std::isnan(point1.x)) {
            if (str == "C" || str == "CENTER") {
                mode = MODE_MAJORRADIUS_MINORRADIUS;
                prompt_output(translate("Specify center point: "));
            }
            else {
                EmbReal strList = str.split(",");
                if (std::isnan(strList[0]) || std::isnan(strList[1])) {
                    alert(translate("Point or option keyword required."));
                    prompt_output(translate("Specify first axis start point or [Center]: ");
                }
                else {
                    point1.x = atof(strList[0]);
                    point1.y = atof(strList[1]);
                    addRubber("ELLIPSE");
                    setRubberMode("ELLIPSE_LINE");
                    setRubberPoint("ELLIPSE_LINE_POINT1", point1.x, point1.y);
                    prompt_output(translate("Specify first axis end point: ");
                }
            }
        }
        else if (std::isnan(point2.x)) {
            EmbReal strList = str.split(",");
            if (std::isnan(strList[0]) || std::isnan(strList[1])) {
                alert(translate("Invalid point."));
                prompt_output(translate("Specify first axis end point: ");
            }
            else {
                point2.x = atof(strList[0]);
                point2.y = atof(strList[1]);
                center.x = (point1.x + point2.x)/2.0;
                center.y = (point1.y + point2.y)/2.0;
                width = calculateDistance(point1.x, point1.y, point2.x, point2.y);
                rot = calculateAngle(point1.x, point1.y, point2.x, point2.y);
                setRubberMode("ELLIPSE_MAJORDIAMETER_MINORRADIUS");
                setRubberPoint("ELLIPSE_AXIS1_POINT1", point1.x, point1.y);
                setRubberPoint("ELLIPSE_AXIS1_POINT2", point2.x, point2.y);
                setRubberPoint("ELLIPSE_CENTER", center.x, center.y);
                setRubberPoint("ELLIPSE_WIDTH", width, 0);
                setRubberPoint("ELLIPSE_ROT", rot, 0);
                prompt_output(translate("Specify second axis end point or [Rotation]: ");
            }
        }
        else if (std::isnan(point3.x)) {
            if (str == "R" || str == "ROTATION") {
                mode = MODE_ELLIPSE_ROTATION;
                prompt_output(translate("Specify rotation: ");
            }
            else {
                EmbReal strList = str.split(",");
                if (std::isnan(strList[0]) || std::isnan(strList[1])) {
                    alert(translate("Point or option keyword required."));
                    prompt_output(translate("Specify second axis end point or [Rotation]: ");
                }
                else {
                    point3.x = atof(strList[0]);
                    point3.y = atof(strList[1]);
                    height = perpendicularDistance(point3.x, point3.y, point1.x, point1.y, point2.x, point2.y)*2.0;
                    setRubberPoint("ELLIPSE_AXIS2_POINT2", point3.x, point3.y);
                    vulcanize();
                    actuator("end");
                }
            }
        }
        break;
    }
    case MODE_MAJORRADIUS_MINORRADIUS: {
        if (std::isnan(point1.x)) {
            EmbReal strList = str.split(",");
            if (std::isnan(strList[0]) || std::isnan(strList[1])) {
                alert(translate("Invalid point."));
                prompt_output(translate("Specify center point: ");
            }
            else {
                point1.x = atof(strList[0]);
                point1.y = atof(strList[1]);
                center.x = point1.x;
                center.y = point1.y;
                addRubber("ELLIPSE");
                setRubberMode("ELLIPSE_LINE");
                setRubberPoint("ELLIPSE_LINE_POINT1", point1.x, point1.y);
                setRubberPoint("ELLIPSE_CENTER", center.x, center.y);
                prompt_output(translate("Specify first axis end point: ");
            }
        }
        else if (std::isnan(point2.x)) {
            EmbReal strList = str.split(",");
            if (std::isnan(strList[0]) || std::isnan(strList[1])) {
                alert(translate("Invalid point."));
                prompt_output(translate("Specify first axis end point: ");
            }
            else {
                point2.x = atof(strList[0]);
                point2.y = atof(strList[1]);
                width = calculateDistance(point1.x, point1.y, point2.x, point2.y)*2.0;
                rot = calculateAngle(point1.x, point1.y, point2.x, point2.y);
                setRubberMode("ELLIPSE_MAJORRADIUS_MINORRADIUS");
                setRubberPoint("ELLIPSE_AXIS1_POINT2", point2.x, point2.y);
                setRubberPoint("ELLIPSE_WIDTH", width, 0);
                setRubberPoint("ELLIPSE_ROT", rot, 0);
                prompt_output(translate("Specify second axis end point or [Rotation]: ");
            }
        }
        else if (std::isnan(point3.x)) {
            if (str == "R" || str == "ROTATION") {
                mode = MODE_ELLIPSE_ROTATION;
                prompt_output(translate("Specify ellipse rotation: ");
            }
            else {
                EmbReal strList = str.split(",");
                if (std::isnan(strList[0]) || std::isnan(strList[1])) {
                    alert(translate("Point or option keyword required."));
                    prompt_output(translate("Specify second axis end point or [Rotation]: ");
                }
                else {
                    point3.x = atof(strList[0]);
                    point3.y = atof(strList[1]);
                    height = perpendicularDistance(point3.x, point3.y, point1.x, point1.y, point2.x, point2.y)*2.0;
                    setRubberPoint("ELLIPSE_AXIS2_POINT2", point3.x, point3.y);
                    vulcanize();
                    actuator("end");
                }
            }
        }
    }
    case MODE_ELLIPSE_ROTATION: {
        if (std::isnan(point1.x)) {
            error("ELLIPSE", tr("This should never happen."));
        }
        else if (std::isnan(point2.x)) {
            error("ELLIPSE", tr("This should never happen."));
        }
        else if (std::isnan(point3.x)) {
            if (std::isnan(str)) {
                alert(translate("Invalid angle. Input a numeric angle or pick a point."));
                prompt_output(translate("Specify rotation: ");
            }
            else {
                EmbReal angle = atof(str);
                height = cos(angle*PI/180.0)*width;
                addEllipse(center.x, center.y, width, height, rot, false);
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
    init();

    if (numSelected() <= 0) {
        //TODO: Prompt to select objects if nothing is preselected
        alert(translate("Preselect objects before invoking the delete command."));
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
    numPoints = node_int(512); //Default //TODO: min:64 max:8192
    center.x = 0.0f;
    center.y = 0.0f;
    scale.x = node(1.0f);
    scale.y = node(1.0f);
    init();
    actuator("clear-selection");
    mode = node_str("MODE_NUM_POINTS");

    //Heart4: 10.0 / 512
    //Heart5: 1.0 / 512

    addRubber("POLYGON");
    setRubberMode("POLYGON");
    updateHeart("HEART5", numPoints, scale.x, scale.y);
    spareRubber("POLYGON");
    actuator("end");
}

/* . */
void
Geometry::updateHeart(char *style, int numPoints, EmbReal xScale, EmbReal yScale)
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
    init();
    actuator("clear-selection");
    firstRun = true;
    first = {0.0f, 0.0f};
    prev = {0.0f, 0.0f};
    prompt_output(translate("Specify first point: ");
}

/* . */
void
Geometry::line_click(EmbVector v)
{
    if (firstRun) {
        firstRun = false;
        first = v;
        prev = v;
        addRubber("LINE");
        setRubberMode("LINE");
        setRubberPoint("LINE_START", first);
        prompt_output(translate("Specify next point or [Undo]: ");
    }
    else {
        setRubberPoint("LINE_END", v);
        vulcanize();
        addRubber("LINE");
        setRubberMode("LINE");
        setRubberPoint("LINE_START", v);
        prev = v;
    }
}

/* . */
void
Geometry::line_prompt(char *args)
{
    if (firstRun) {
        EmbReal strList = str.split(",");
        if (std::isnan(strList[0]) || std::isnan(strList[1])) {
            alert(translate("Invalid point."));
            prompt_output(translate("Specify first point: "));
        }
        else {
            firstRun = false;
            first.x = atof(strList[0]);
            first.y = atof(strList[1]);
            prev = first;
            addRubber("LINE");
            setRubberMode("LINE");
            setRubberPoint("LINE_START", first);
            prompt_output(translate("Specify next point or [Undo]: "));
        }
    }
    else {
        if (str == "U" || str == "UNDO") {
            actuator("todo LINE prompt() for UNDO");
        }
        else {
            EmbReal strList = str.split(",");
            if (std::isnan(strList[0]) || std::isnan(strList[1])) {
                alert(translate("Point or option keyword required."));
                prompt_output(translate("Specify next point or [Undo]: ");
            }
            else {
                EmbVector v = get_vector(strList);
                setRubberPoint(g, "LINE_END", v);
                geometry_vulcanize(g);
                addRubber(g, "LINE");
                setRubberMode(g, "LINE");
                setRubberPoint(g, "LINE_START", v);
                prev = v;
                prompt_output(translate("Specify next point or [Undo]: "));
            }
        }
    }
}

/* locate_point_main */
void
Geometry::locate_point_main(void)
{
    actuator("init");
    actuator("clear-selection");
    prompt_output("Specify point: ");
}

/* locate_point_click v */
void
Geometry::locate_point_click(EmbVector v)
{
    actuator("set-prompt-prefix v.x = " + v.x + ", Y = " + v.y);
    actuator("end");
}

/* . */
void
Geometry::locate_point_prompt(char *args)
{
    std::vector<std::string> strList = tokenize(args, ',');
    if (std::isnan(strList[0]) || std::isnan(strList[1])) {
        alert(translate("Invalid point."));
        prompt_output(translate("Specify point: ");
    }
    else {
        actuator("prompt-output .x = " + strList[0] + ", Y = " + strList[1]);
        actuator("end");
    }
}

/* . */
void
Geometry::move_main(void)
{
    init();
    firstRun.b = node_int(true);
    base.x = 0.0f;
    base.y = 0.0f;
    dest.x = 0.0f;
    dest.y = 0.0f;
    delta.x = 0.0f;
    delta.y = 0.0f;

    if (numSelected() <= 0) {
        //TODO: Prompt to select objects if nothing is preselected
        alert(translate("Preselect objects before invoking the move command."));
        actuator("end");
        messageBox("information", tr("Move Preselect"), tr("Preselect objects before invoking the move command."));
    }
    else {
        prompt_output(translate("Specify base point: "));
    }
}

/* . */
void
Geometry::move_click(EmbVector v)
{
    if (firstRun) {
        g->firstRun = false;
        g->base = v;
        addRubber(g, "LINE");
        setRubberMode(g, "LINE");
        setRubberPoint(g, "LINE_START", base);
        previewOn(g, "SELECTED", "MOVE", base, 0);
        prompt_output(translate("Specify destination point: ");
    }
    else {
        g->dest = v;
        g->delta = embVector_subtract(g->dest, g->base);
        moveSelected(g->delta);
        previewOff();
        actuator("end");
    }
}

/* . */
void
Geometry::move_prompt(char *str)
{
    if (firstRun.b) {
        EmbReal strList = str.split(",");
        if (std::isnan(strList[0]) || std::isnan(strList[1])) {
            alert(translate("Invalid point."));
            prompt_output(translate("Specify base point: ");
        }
        else {
            firstRun = node_int(false);
            base.x = atof(strList[0]);
            base.y = atof(strList[1]);
            addRubber("LINE");
            setRubberMode("LINE");
            setRubberPoint("LINE_START", baseX, baseY);
            previewOn("SELECTED", "MOVE", baseX, baseY, 0);
            prompt_output(translate("Specify destination point: ");
        }
    }
    else {
        EmbReal strList = str.split(",");
        if (std::isnan(strList[0]) || std::isnan(strList[1])) {
            alert(translate("Invalid point."));
            prompt_output(translate("Specify destination point: ");
        }
        else {
            dest.x = atof(strList[0]);
            dest.y = atof(strList[1]);
            delta = embVector_subtract(dest, base);
            moveSelected(delta);
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
    first = {0.0, 0.0};
    prev = {0.0, 0.0};
    init();
    actuator("clear-selection");
    firstRun = atof(true);
    prompt_output(translate("Specify start point: ");
}

/* . */
void
Geometry::path_click(EmbVector v)
{
    if (firstRun) {
        firstRun = false;
        first = v;
        prev = v;
        addPath(v);
        prompt_output(translate("Specify next point or [Arc/Undo]: "));
    }
    else {
        appendLineToPath(v);
        prev = v;
    }
}

/* . */
void
Geometry::path_prompt(char *cmd)
{
    if (string_equal(cmd, "A") || string_equal(cmd, "ARC")) {
        actuator("todo PATH prompt() for ARC");
    }
    else if (string_equal(cmd, "U") || string_equal(cmd, "UNDO")) {
        actuator("todo PATH prompt() for UNDO");
    }
    else {
        EmbReal strList = str.split(",");
        if (std::isnan(strList[0]) || std::isnan(strList[1])) {
            alert(translate("Point or option keyword required."));
            prompt_output(translate("Specify next point or [Arc/Undo]: ");
        }
        else {
            EmbReal x = atof(strList[0]);
            EmbReal y = atof(strList[1]);
            if (firstRun) {
                firstRun = false;
                first = embVector(x, y);
                prev = embVector(x, y);
                addPath(x, y);
                prompt_output(translate("Specify next point or [Arc/Undo]: ");
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
    init();
    actuator("clear-selection");
    firstRun = true;
    setPromptPrefix("TODO: Current point settings: PDMODE=?  PDSIZE=?"); //TODO: tr needed here when complete
    prompt_output(translate("Specify first point: ");
}

/* . */
void
Geometry::point_click(EmbVector v)
{
    if (firstRun) {
        firstRun = false;
        prompt_output(translate("Specify next point: "));
        addPoint(v);
    }
    else {
        addPoint(v);
    }
}

/* . */
void
Geometry::point_prompt(char *str)
{
    if (firstRun) {
        if (string_equal(cmd, "M") || string_equal(cmd, "MODE")) {
            actuator("todo POINT prompt() for PDMODE");
        }
        else if (str == "S" || str == "SIZE") {
            actuator("todo POINT prompt() for PDSIZE");
        }
        EmbReal strList = str.split(",");
        if (std::isnan(strList[0]) || std::isnan(strList[1])) {
            alert(translate("Invalid point."));
            prompt_output(translate("Specify first point: "));
        }
        else {
            firstRun = false;
            EmbReal x = atof(strList[0]);
            EmbReal y = atof(strList[1]);
            prompt_output(translate("Specify next point: "));
            addPoint(x,y);
        }
    }
    else {
        EmbReal strList = str.split(",");
        if (std::isnan(strList[0]) || std::isnan(strList[1])) {
            alert(translate("Invalid point."));
            prompt_output(translate("Specify next point: "));
        }
        else {
            EmbReal x = atof(strList[0]);
            EmbReal y = atof(strList[1]);
            prompt_output(translate("Specify next point: "));
            addPoint(x,y);
        }
    }
}

/* . */
void
Geometry::polygon_main(void)
{
    init();
    actuator("clear-selection");
    center = {0.0f, 0.0f};
    side1 = {0.0f, 0.0f};
    side2 = {0.0f, 0.0f};
    pointI = {0.0f, 0.0f};
    pointC = {0.0f, 0.0f};
    polyType = "Inscribed"; //Default
    numSides = 4;           //Default
    mode = MODE_POLYGON_NUM_SIDES;
    prompt_output(translate("Enter number of sides" + " {" + numSides.toString() + "}: ");
}


/* Polygon */
void
Geometry::polygon_prompt(char *str)
{
    case MODE_POLYGON_NUM_SIDES: {
        if (str == "" && numSides >= 3 && numSides <= 1024) {
            prompt_output(translate("Specify center point or [Sidelength]: ");
            mode = MODE_CENTER_PT;
        }
        else {
            EmbReal tmp = atof(str);
            if (std::isnan(tmp) || !isInt(tmp) || tmp < 3 || tmp > 1024) {
                alert(translate("Requires an integer between 3 and 1024."));
                prompt_output(translate("Enter number of sides" + " {" + numSides.toString() + "}: ");
            }
            else {
                numSides = tmp;
                prompt_output(translate("Specify center point or [Sidelength]: "));
                mode = MODE_CENTER_PT;
            }
        }
        break;
    }
    case MODE_POLYGON_CENTER_PT: {
        if (str == "S" || str == "SIDELENGTH") {
            mode = MODE_SIDE_LEN;
            prompt_output(translate("Specify start point: "));
        }
        else {
            EmbReal strList = str.split(",");
            if (std::isnan(strList[0]) || std::isnan(strList[1])) {
                alert(translate("Point or option keyword required."));
                prompt_output(translate("Specify center point or [Sidelength]: "));
            }
            else {
                center.x = atof(strList[0]);
                center.y = atof(strList[1]);
                mode = MODE_POLYTYPE;
                prompt_output(translate("Specify polygon type [Inscribed in circle/Circumscribed around circle]") + " {" + g->polyType + "}: ");
            }
        }
    }
    case MODE_POLYTYPE) {
        if (str == "INSCRIBED") {
            mode = MODE_INSCRIBE;
            polyType = "Inscribed";
            prompt_output(translate("Specify polygon corner point or [Distance]: ");
            addRubber("POLYGON");
            setRubberMode("POLYGON_INSCRIBE");
            setRubberPoint("POLYGON_CENTER", center.x, center.y);
            setRubberPoint("POLYGON_NUM_SIDES", numSides, 0);
        }
        else if (str == "CIRCUMSCRIBED") {
            mode = MODE_CIRCUMSCRIBE;
            polyType = "Circumscribed";
            prompt_output(translate("Specify polygon side point or [Distance]: ");
            addRubber("POLYGON");
            setRubberMode("POLYGON_CIRCUMSCRIBE");
            setRubberPoint("POLYGON_CENTER", center.x, center.y);
            setRubberPoint("POLYGON_NUM_SIDES", numSides, 0);
        }
        else if (str == "") {
            if (polyType == "Inscribed") {
                mode = MODE_INSCRIBE;
                prompt_output(translate("Specify polygon corner point or [Distance]: ");
                addRubber("POLYGON");
                setRubberMode("POLYGON_INSCRIBE");
                setRubberPoint("POLYGON_CENTER", center.x, center.y);
                setRubberPoint("POLYGON_NUM_SIDES", numSides, 0);
            }
            else if (polyType == "Circumscribed") {
                mode = MODE_CIRCUMSCRIBE;
                prompt_output(translate("Specify polygon side point or [Distance]: ");
                addRubber("POLYGON");
                setRubberMode("POLYGON_CIRCUMSCRIBE");
                setRubberPoint("POLYGON_CENTER", center.x, center.y);
                setRubberPoint("POLYGON_NUM_SIDES", numSides, 0);
            }
            else {
                error("POLYGON", tr("Polygon type is not Inscribed or Circumscribed."));
            }
        }
        else {
            alert(translate("Invalid option keyword."));
            prompt_output(translate("Specify polygon type [Inscribed in circle/Circumscribed around circle]") + " {" + polyType + "}: ");
        }
    }
    case MODE_INSCRIBE) {
        if (str == "D" || str == "DISTANCE") {
            mode = MODE_DISTANCE;
            prompt_output(translate("Specify distance: "));
        }
        else {
            EmbReal strList = str.split(",");
            if (std::isnan(strList[0]) || std::isnan(strList[1])) {
                alert(translate("Point or option keyword required."));
                prompt_output(translate("Specify polygon corner point or [Distance]: "));
            }
            else {
                pointI.x = atof(strList[0]);
                pointI.y = atof(strList[1]);
                setRubberPoint("POLYGON_INSCRIBE_POINT", pointI);
                vulcanize();
                actuator("end");
            }
        }
    }
    case MODE_CIRCUMSCRIBE) {
        if (str == "D" || str == "DISTANCE") {
            mode = MODE_DISTANCE;
            prompt_output(translate("Specify distance: "));
        }
        else {
            EmbReal strList = str.split(",");
            if (std::isnan(strList[0]) || std::isnan(strList[1])) {
                alert(translate("Point or option keyword required."));
                prompt_output(translate("Specify polygon side point or [Distance]: "));
            }
            else {
                pointC.x = atof(strList[0]);
                pointC.y = atof(strList[1]);
                setRubberPoint("POLYGON_CIRCUMSCRIBE_POINT", pointC);
                vulcanize();
                actuator("end");
            }
        }
    }
    case MODE_DISTANCE) {
        if (std::isnan(str)) {
            alert(translate("Requires valid numeric distance."));
            prompt_output(translate("Specify distance: "));
        }
        else {
            if (polyType == "Inscribed") {
                pointI = center;
                pointI.y += atof(str);
                setRubberPoint("POLYGON_INSCRIBE_POINT", pointI);
                vulcanize();
                actuator("end");
            }
            else if (polyType == "Circumscribed") {
                pointC = center;
                pointC.y += atof(str);
                setRubberPoint("POLYGON_CIRCUMSCRIBE_POINT", pointC);
                vulcanize();
                actuator("end");
            }
            else {
                error("POLYGON", tr("Polygon type is not Inscribed or Circumscribed."));
            }
        }
    }
    case MODE_SIDE_LEN: {
        actuator("todo POLYGON Sidelength mode");
        break;
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
        firstRun = false;
        first = v;
        prev = v;
        addRubber("POLYLINE");
        setRubberMode("POLYLINE");
        setRubberPoint("POLYLINE_POINT_0", first);
        prompt_output(translate("Specify next point or [Undo]: ");
    }
    else {
        num++;
        setRubberPoint("POLYLINE_POINT_" + num.toString(), v);
        setRubberText("POLYLINE_NUM_POINTS", num.toString());
        spareRubber("POLYLINE");
        prev = v;
    }
}

/* . */
void
Geometry::polyline_prompt(char *str)
{
    if (firstRun.b) {
        EmbReal strList = str.split(",");
        if (std::isnan(strList[0]) || std::isnan(strList[1])) {
            alert(translate("Invalid point."));
            prompt_output(translate("Specify first point: "));
        }
        else {
            firstRun = false;
            first.x = atoi(strList[0]);
            first.y = atoi(strList[1]);
            prev.x = firstX;
            prev.y = first.y;
            addRubber("POLYLINE");
            setRubberMode("POLYLINE");
            setRubberPoint("POLYLINE_POINT_0", firstX, first.y);
            prompt_output(translate("Specify next point or [Undo]: ");
        }
    }
    else {
        if (str == "U" || str == "UNDO") {
            actuator("todo POLYLINE prompt() for UNDO");
        }
        else {
            EmbReal strList = str.split(",");
            if (std::isnan(strList[0]) || std::isnan(strList[1])) {
                alert(translate("Point or option keyword required."));
                prompt_output(translate("Specify next point or [Undo]: ");
            }
            else {
                EmbReal x = atof(strList[0]);
                EmbReal y = atof(strList[1]);
                g->num = g->num + 1;
                setRubberPoint("POLYLINE_POINT_" + g->num, v);
                setRubberText("POLYLINE_NUM_POINTS", g->num);
                spareRubber("POLYLINE");
                prev = v;
                prompt_output(translate("Specify next point or [Undo]: "));
            }
        }
    }
}

const char quickleader_main[][MAX_STRING_LENGTH] = {
    todo Adding the text is not complete yet.;
    init();
    clear_selection();
    x[0] = 0.0f;
    y[0] = 0.0f;
    x[1] = 0.0f;
    y[1] = 0.0f;
    set-prompt-prefix-tr Specify first point: "
};

/* . */
void
Geometry::quickleader_click(EmbVector v)
{
    if (std::isnan(point1.x)) {
        point1 = v;
        addRubber("DIMLEADER");
        setRubberMode("DIMLEADER_LINE");
        setRubberPoint("DIMLEADER_LINE_START", point1);
        prompt_output(translate("Specify second point: "));
    }
    else {
        point2 = v;
        setRubberPoint("DIMLEADER_LINE_END", point2);
        vulcanize();
        actuator("end");
    }
}

/* . */
void
Geometry::quickleader_prompt(char *str)
{
    EmbReal strList = str.split(",");
    if (std::isnan(point1.x)) {
        if (std::isnan(strList[0]) || std::isnan(strList[1])) {
            alert(translate("Requires two points."));
            prompt_output(translate("Specify first point: "));
        }
        else {
            point1.x = atoi(strList[0]);
            point1.y = atoi(strList[1]);
            addRubber("DIMLEADER");
            setRubberMode("DIMLEADER_LINE");
            setRubberPoint("DIMLEADER_LINE_START", point1.x, point1.y);
            prompt_output(translate("Specify second point: ");
        }
    }
    else {
        if (std::isnan(strList[0]) || std::isnan(strList[1])) {
            alert(translate("Requires two points."));
            prompt_output(translate("Specify second point: ");
        }
        else {
            point2.x = atoi(strList[0]);
            point2.y = atoi(strList[1]);
            setRubberPoint("DIMLEADER_LINE_END", point2.x, point2.y);
            vulcanize();
            actuator("end");
        }
    }
}

/* . */
const char rectangle_click_script[][MAX_STRING_LENGTH] = {
    if (newRect) {
        newRect = false;
        point1 = v;
        addRubber("RECTANGLE");
        setRubberMode("RECTANGLE");
        setRubberPoint("RECTANGLE_START", v);
        prompt_output(translate("Specify other corner point or [Dimensions]: "));
    }
    else {
        point2 = v;
        setRubberPoint(RECTANGLE_END, v);
        vulcanize();
        actuator("end");
    }
};

/* . */
void
Geometry::rectangle_prompt(char *str)
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
            alert(translate("Invalid point."));
            prompt_output(translate("Specify first point: "));
        }
        else {
            EmbReal x = atoi(strList[0]);
            EmbReal y = atoi(strList[1]);
            if (g->firstRun) {
                g->firstRun = false;
                point1 = v;
                addRubber("RECTANGLE");
                setRubberMode("RECTANGLE");
                setRubberPoint("RECTANGLE_START", v);
                prompt_output(translate("Specify other corner point or [Dimensions]: "));
            }
            else {
                g->firstRun = true;
                point2 = v;
                setRubberPoint("RECTANGLE_END", v);
                vulcanize();
                actuator("end");
            }
        }
    }
}

/* . */
void
Geometry::rgb_main(void)
{
    init();
    clear_selection();
    g->mode = MODE_RGB_BACKGROUND;
    prompt_output(translate("Enter RED,GREEN,BLUE values for background or [Crosshair/Grid]: "));
}

/* . */
void
Geometry::rgb_click(EmbVector v)
{
    //Do Nothing, prompt only command.
}

/* . */
void
Geometry::rgb_prompt(char *str)
{
    if (mode == "RGB_MODE_BACKGROUND") {
        if (str == "C" || str == "CROSSHAIR") {
            mode = atoi("RGB_MODE_CROSSHAIR");
            prompt_output(translate("Specify crosshair color: "));
        }
        else if (str == "G" || str == "GRID") {
            mode = RGB_MODE_GRID;
            prompt_output(translate("Specify grid color: "));
        }
        else {
            EmbReal strList = str.split(",");
            EmbReal r = atoi(strList[0]);
            EmbReal g = atoi(strList[1]);
            EmbReal b = atoi(strList[2]);
            if (!validRGB(r,g,b)) {
                alert(translate("Invalid color. R,G,B values must be in the range of 0-255."));
                prompt_output(translate("Specify background color: "));
            }
            else {
                setBackgroundColor(r,g,b);
                actuator("end");
            }
        }
    }
    case RGB_MODE_CROSSHAIR) {
        EmbReal strList = str.split(",");
        EmbReal r = atoi(strList[0]);
        EmbReal g = atoi(strList[1]);
        EmbReal b = atoi(strList[2]);
        if (!validRGB(r,g,b)) {
            alert(translate("Invalid color. R,G,B values must be in the range of 0-255."));
            prompt_output(translate("Specify crosshair color: "));
        }
        else {
            setCrossHairColor(r,g,b);
            actuator("end");
        }
    }
    case RGB_MODE_GRID) {
        EmbReal strList = str.split(",");
        EmbReal r = atof(strList[0]);
        EmbReal g = atof(strList[1]);
        EmbReal b = atof(strList[2]);
        if (!validRGB(r,g,b)) {
            alert(translate("Invalid color. R,G,B values must be in the range of 0-255."));
            prompt_output(translate("Specify grid color: "));
        }
        else {
            setGridColor(r,g,b);
            actuator("end");
        }
    }
}

/* . */
void
Geometry::rotate_main(char *args)
{
    geometry_init(g);
    g->mode = ROTATE_MODE_NORMAL;
    g->firstRun = true;
    g->base = {0.0f, 0.0f};
    g->dest = {0.0f, 0.0f};
    g->angle = 0.0f;

    g->baseR = {0.0f, 0.0f};
    g->destR = {0.0f, 0.0f};
    g->angleRef = 0.0f;
    g->angleNew = 0.0f;

    if (numSelected() <= 0) {
        //TODO: Prompt to select objects if nothing is preselected
        alert(translate("Preselect objects before invoking the rotate command."));
        actuator("end");
        messageBox("information", tr("Rotate Preselect"), tr("Preselect objects before invoking the rotate command."));
    }
    else {
        prompt_output(translate("Specify base point: "));
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
            base = v;
            addRubber("LINE");
            setRubberMode("LINE");
            setRubberPoint("LINE_START", g->base);
            previewOn("SELECTED", "ROTATE", g->base, 0);
            prompt_output(translate("Specify rotation angle or [Reference]: "));
        }
        else {
            dest = v;
            angle = calculateAngle(baseX, baseY, dest.x, dest.y);
            rotateSelected(baseX, baseY, angle);
            previewOff();
            actuator("end");
        }
    }
    case ROTATE_MODE_REFERENCE) {
        if (std::isnan(g->baseR.x)) {
            g->baseR = v;
            addRubber("LINE");
            setRubberMode("LINE");
            setRubberPoint("LINE_START", g->baseR);
            prompt_output(translate("Specify second point: "));
        }
        else if (std::isnan(destRX)) {
            g->destR = v;
            angleRef = calculateAngle(g->baseR, g->destR);
            setRubberPoint("LINE_START", g->base);
            previewOn("SELECTED", "ROTATE", g->base, g->angleRef);
            prompt_output(translate("Specify the new angle: "));
        }
        else if (std::isnan(angleNew)) {
            angleNew = calculateAngle(baseX, baseY, x, y);
            rotateSelected(baseX, baseY, angleNew - angleRef);
            previewOff();
            actuator("end");
        }
    }
}

/* . */
void
Geometry::rotate_prompt(char *str)
{
    if (mode == ROTATE_MODE_NORMAL) {
        if (firstRun) {
            EmbReal strList = str.split(",");
            if (std::isnan(strList[0]) || std::isnan(strList[1])) {
                alert(translate("Invalid point."));
                prompt_output(translate("Specify base point: ");
            }
            else {
                firstRun = false;
                base.x = atof(strList[0]);
                base.y = atof(strList[1]);
                addRubber("LINE");
                setRubberMode("LINE");
                setRubberPoint("LINE_START", baseX, baseY);
                previewOn("SELECTED", "ROTATE", baseX, baseY, 0);
                prompt_output(translate("Specify rotation angle or [Reference]: ");
            }
        }
        else {
            if (str == "R" || str == "REFERENCE") {
                mode = MODE_REFERENCE;
                prompt_output(translate("Specify the reference angle") + " {0.00}: ");
                clearRubber();
                previewOff();
            }
            else {
                if (std::isnan(str)) {
                    alert(translate("Requires valid numeric angle, second point, or option keyword."));
                    prompt_output(translate("Specify rotation angle or [Reference]: ");
                }
                else {
                    angle = atof(str);
                    rotateSelected(baseX, baseY, angle);
                    previewOff();
                    actuator("end");
                }
            }
        }
    }
    case MODE_REFERENCE) {
        if (std::isnan(g->baseRX)) {
            if (std::isnan(str)) {
                EmbReal strList = str.split(",");
                if (std::isnan(strList[0]) || std::isnan(strList[1])) {
                    alert(translate("Requires valid numeric angle or two points."));
                    prompt_output(translate("Specify the reference angle {0.00}: ");
                }
                else {
                    g->baseR.x = atof(strList[0]);
                    g->baseR.y = atof(strList[1]);
                    addRubber("LINE");
                    setRubberMode("LINE");
                    setRubberPoint("LINE_START", g->baseR);
                    prompt_output(translate("Specify second point: ");
                }
            }
            else {
                //The base and dest values are only set here to advance the command.
                g->baseR.x = 0.0;
                g->baseR.y = 0.0;
                g->destR.x = 0.0;
                g->destR.y = 0.0;
                //The reference angle is what we will use later.
                angleRef = atof(str);
                addRubber("LINE");
                setRubberMode("LINE");
                setRubberPoint("LINE_START", baseX, baseY);
                previewOn("SELECTED", "ROTATE", baseX, baseY, angleRef);
                prompt_output(translate("Specify the new angle: ");
            }
        }
        else if (std::isnan(destRX)) {
            if (std::isnan(str)) {
                EmbReal strList = str.split(",");
                if (std::isnan(strList[0]) || std::isnan(strList[1])) {
                    alert(translate("Requires valid numeric angle or two points."));
                    prompt_output(translate("Specify second point: "));
                }
                else {
                    g->destR.x = atof(strList[0]);
                    g->destR.y = atof(strList[1]);
                    angleRef = calculateAngle(g->baseRX"], g->baseRY"], destRX, destRY);
                    previewOn("SELECTED", "ROTATE", baseX, baseY, angleRef);
                    setRubberPoint("LINE_START", baseX, baseY);
                    prompt_output(translate("Specify the new angle: ");
                }
            }
            else {
                // The base and dest values are only set here to advance the command.
                baseR = {0.0, 0.0};
                destR = {0.0, 0.0};
                // The reference angle is what we will use later.
                angleRef = atof(str);
                previewOn("SELECTED", "ROTATE", baseX, baseY, angleRef);
                prompt_output(translate("Specify the new angle: ");
            }
        }
        else if (std::isnan(angleNew)) {
            if (std::isnan(str)) {
                EmbReal strList = str.split(",");
                if (std::isnan(strList[0]) || std::isnan(strList[1])) {
                    alert(translate("Requires valid numeric angle or second point."));
                    prompt_output(translate("Specify the new angle: ");
                }
                else {
                    EmbVector v;
                    v.x = atof(strList[0]);
                    v.y = atof(strList[1]);
                    angleNew = calculateAngle(base, v);
                    rotateSelected(base, angleNew - angleRef);
                    previewOff();
                    actuator("end");
                }
            }
            else {
                angleNew = atof(str);
                rotateSelected(base, angleNew - angleRef);
                previewOff();
                actuator("end");
            }
        }
    }
}

//Command: Sandbox

/*char *properties;
g->test1"];
g->test2"];
*/

/* . */
void
Geometry::sandbox_main(char *str)
{
    init();

    //Report number of pre-selected objects
    setPromptPrefix("Number of Objects Selected: " + numSelected().toString());

    mirrorSelected(0,0,0,1);

    //selectAll();
    //rotateSelected(0,0,90);

    //Polyline & Polygon Testing

    offset.x = 0.0;
    offset.y = 0.0;

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

    offset.x = 5.0;
    offset.y = 0.0;

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
    init();

    mode = MODE_SCALE_NORMAL;
    firstRun = true;
    base = {0.0f, 0.0f};
    dest = {0.0f, 0.0f};
    factor = 0.0f;

    baseR = base;
    destR = dest;
    factorRef = 0.0f;
    factorNew = 0.0f;

    if (numSelected() <= 0) {
        //TODO: Prompt to select objects if nothing is preselected
        alert(translate("Preselect objects before invoking the scale command."));
        actuator("end");
        messageBox("information", tr("Scale Preselect"), tr("Preselect objects before invoking the scale command."));
    }
    else {
        prompt_output(translate("Specify base point: "));
    }
}

/* . */
void
Geometry::scale_click(EmbVector v)
{
    switch (mode) {
    case MODE_SCALE_NORMAL: {
        if (firstRun) {
            firstRun = false;
            base = v;
            addRubber("LINE");
            setRubberMode("LINE");
            setRubberPoint("LINE_START", base);
            previewOn("SELECTED", "SCALE", base, 1);
            prompt_output(translate("Specify scale factor or [Reference]: "));
        }
        else {
            dest = v;
            factor = calculateDistance(base, dest);
            scaleSelected(base, factor);
            previewOff();
            actuator("end");
        }
        break;
    }
    case MODE_SCALE_REFERENCE: {
        if (std::isnan(g->baseR.x)) {
            g->baseR = v;
            addRubber("LINE");
            setRubberMode("LINE");
            setRubberPoint("LINE_START", g->baseR);
            prompt_output(translate("Specify second point: "));
        }
        else if (std::isnan(destRX)) {
            g->destR = v;
            factorRef = calculateDistance(g->baseR, g->destR);
            if (factorRef <= 0.0) {
                destRX = 0.0f;
                destRY = 0.0f;
                factorRef = 0.0f;
                alert(translate("Value must be positive and nonzero."));
                prompt_output(translate("Specify second point: "));
            }
            else {
                setRubberPoint("LINE_START", baseX, baseY);
                previewOn("SELECTED", "SCALE", baseX, baseY, factorRef);
                prompt_output(translate("Specify new length: ");
            }
        }
        else if (std::isnan(factorNew)) {
            factorNew = calculateDistance(baseX, baseY, x, y);
            if (factorNew <= 0.0) {
                factorNew = 0.0f;
                alert(translate("Value must be positive and nonzero."));
                prompt_output(translate("Specify new length: ");
            }
            else {
                scaleSelected(baseX, baseY, factorNew/factorRef);
                previewOff();
                actuator("end");
            }
        }
    }
}

/* . */
void
Geometry::scale_prompt(char *str)
{
    if (mode == MODE_NORMAL) {
        if (firstRun) {
            EmbReal strList = str.split(",");
            if (std::isnan(strList[0]) || std::isnan(strList[1])) {
                alert(translate("Invalid point."));
                prompt_output(translate("Specify base point: ");
            }
            else {
                firstRun = false;
                base.x = atof(strList[0]);
                base.y = atof(strList[1]);
                addRubber("LINE");
                setRubberMode("LINE");
                setRubberPoint("LINE_START", baseX, baseY);
                previewOn("SELECTED", "SCALE", baseX, baseY, 1);
                prompt_output(translate("Specify scale factor or [Reference]: ");
            }
        }
        else {
            if (str == "R" || str == "REFERENCE") {
                mode = MODE_REFERENCE;
                prompt_output(translate("Specify reference length {1}: ");
                clearRubber();
                previewOff();
            }
            else {
                if (std::isnan(str)) {
                    alert(translate("Requires valid numeric distance, second point, or option keyword."));
                    prompt_output(translate("Specify scale factor or [Reference]: ");
                }
                else {
                    factor = atof(str);
                    scaleSelected(base.x, base.y, factor);
                    previewOff();
                    actuator("end");
                }
            }
        }
    }
    case "MODE_REFERENCE") {
        if (std::isnan(g->baseR.x)) {
            if (std::isnan(str)) {
                EmbReal strList = str.split(",");
                if (std::isnan(strList[0]) || std::isnan(strList[1])) {
                    alert(translate("Requires valid numeric distance or two points."));
                    prompt_output(translate("Specify reference length {1}: ");
                }
                else {
                    g->baseR.x = atof(strList[0]);
                    g->baseR.y = atof(strList[1]);
                    addRubber("LINE");
                    setRubberMode("LINE");
                    setRubberPoint("LINE_START", g->baseRX, g->baseRY);
                    prompt_output(translate("Specify second point: "));
                }
            }
            else {
                //The base and dest values are only set here to advance the command.
                g->baseR.x = 0.0;
                g->baseR.y = 0.0;
                g->destR.x = 0.0;
                g->destR.y = 0.0;
                //The reference length is what we will use later.
                factorRef = atof(str);
                if (factorRef <= 0.0) {
                    g->baseRX = 0.0f;
                    g->baseRY = 0.0f;
                    destRX = 0.0f;
                    destRY = 0.0f;
                    factorRef = 0.0f;
                    alert(translate("Value must be positive and nonzero."));
                    prompt_output(translate("Specify reference length") + " {1}: ");
                }
                else {
                    addRubber("LINE");
                    setRubberMode("LINE");
                    setRubberPoint("LINE_START", g->baseX, g->baseY);
                    previewOn("SELECTED", "SCALE", g->baseX, g->baseY, g->factorRef);
                    prompt_output(translate("Specify new length: "));
                }
            }
        }
        else if (std::isnan(destRX)) {
            if (std::isnan(str)) {
                EmbReal strList = str.split(",");
                if (std::isnan(strList[0]) || std::isnan(strList[1])) {
                    alert(translate("Requires valid numeric distance or two points."));
                    prompt_output(translate("Specify second point: "));
                }
                else {
                    g->destR.x = atof(strList[0]);
                    g->destR.y = atof(strList[1]);
                    g->factorRef = calculateDistance(g->baseRX, g->baseRY"], destRX, destRY);
                    if (factorRef <= 0.0) {
                        destRX = 0.0f;
                        destRY = 0.0f;
                        factorRef = 0.0f;
                        alert(translate("Value must be positive and nonzero."));
                        prompt_output(translate("Specify second point: "));
                    }
                    else {
                        setRubberPoint("LINE_START", baseX, baseY);
                        previewOn("SELECTED", "SCALE", baseX, baseY, factorRef);
                        prompt_output(translate("Specify new length: ");
                    }
                }
            }
            else {
                //The base and dest values are only set here to advance the command.
                g->baseR.x = 0.0;
                g->baseR.y = 0.0;
                g->destR.x = 0.0;
                g->destR.y = 0.0;
                // The reference length is what we will use later.
                factorRef = atof(str);
                if (factorRef <= 0.0) {
                    destRX = 0.0f;
                    destRY = 0.0f;
                    factorRef = 0.0f;
                    alert(translate("Value must be positive and nonzero."));
                    prompt_output(translate("Specify second point: "));
                }
                else {
                    setRubberPoint("LINE_START", baseX, baseY);
                    previewOn("SELECTED", "SCALE", baseX, baseY, factorRef);
                    prompt_output(translate("Specify new length: "));
                }
            }
        }
        else if (std::isnan(factorNew)) {
            if (std::isnan(str)) {
                EmbReal strList = str.split(",");
                if (std::isnan(strList[0]) || std::isnan(strList[1])) {
                    alert(translate("Requires valid numeric distance or second point."));
                    prompt_output(translate("Specify new length: "));
                }
                else {
                    EmbReal x = atof(strList[0]);
                    EmbReal y = atof(strList[1]);
                    factorNew = calculateDistance(baseX, baseY, x, y);
                    if (factorNew <= 0.0) {
                        factorNew = 0.0f;
                        alert(translate("Value must be positive and nonzero."));
                        prompt_output(translate("Specify new length: "));
                    }
                    else {
                        scaleSelected(baseX, baseY, factorNew/factorRef);
                        previewOff();
                        actuator("end");
                    }
                }
            }
            else {
                factorNew = atof(str);
                if (factorNew <= 0.0) {
                    factorNew = 0.0f;
                    alert(translate("Value must be positive and nonzero."));
                    prompt_output(translate("Specify new length: "));
                }
                else {
                    scaleSelected(g->baseX, g->baseY, g->factorNew/g->factorRef);
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
    init();
    actuator("clear-selection");
    g->text = atof("");
    g->text.x = 0.0f;
    g->text.y = 0.0f;
    g->text.justify = atof("Left");
    g->textFont = textFont();
    g->textHeight = 0.0f;
    g->textRotation = 0.0f;
    mode = MODE_TEXT_SINGLE_SETGEOM;
    prompt_output(translate("Current font: " + "{" + g->textFont + "} " + tr("Text height: ") + "{" +  textSize() + "}");
    prompt_output(translate("Specify start point of text or [Justify/Setfont]: ");
}

/* . */
void
Geometry::text_single_click(EmbVector v)
{
    switch (mode) {

    case MODE_TEXT_SINGLE_SETGEOM: {
        if (std::isnan(textX)) {
            g->text.x = x;
            g->text.y = y;
            addRubber("LINE");
            setRubberMode("LINE");
            setRubberPoint("LINE_START", textX, textY);
            prompt_output(translate("Specify text height" + " {" + textSize() + "}: ");
        }
        else if (std::isnan(g->textHeight)) {
            g->textHeight = calculateDistance(textX, textY, x, y);
            setTextSize(g->textHeight);
            prompt_output(translate("Specify text angle") + " {" + textAngle() + "}: ");
        }
        else if (std::isnan(g->textRotation)) {
            g->textRotation = calculateAngle(textX, textY, x, y);
            setTextAngle(g->textRotation);
            prompt_output(translate("Enter text: "));
            mode = MODE_RAPID;
            prompt->enableRapidFire();
            clearRubber();
            addRubber("TEXTSINGLE");
            setRubberMode("TEXTSINGLE");
            setRubberPoint("TEXT_POINT", textX, textY);
            setRubberPoint("TEXT_HEIGHT_ROTATION", g->textHeight, g->textRotation);
            setRubberText("TEXT_FONT", g->textFont);
            setRubberText("TEXT_JUSTIFY", textJustify);
            setRubberText("TEXT_RAPID", g->text);
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
Geometry::text_single_prompt(char *str)
{
    if (mode == "MODE_JUSTIFY") {
        if (str == "C" || str == "CENTER") {
            mode = MODE_SETGEOM;
            textJustify = node("Center");
            setRubberText("TEXT_JUSTIFY", textJustify);
            prompt_output(translate("Specify center point of text or [Justify/Setfont]: ");
        }
        else if (str == "R" || str == "RIGHT") {
            mode = MODE_SETGEOM;
            textJustify = node("Right");
            setRubberText("TEXT_JUSTIFY", textJustify);
            prompt_output(translate("Specify right-end point of text or [Justify/Setfont]: "));
        }
        else if (str == "A" || str == "ALIGN") {
            mode = MODE_SETGEOM;
            textJustify = "Aligned";
            setRubberText("TEXT_JUSTIFY", textJustify);
            prompt_output(translate("Specify start point of text or [Justify/Setfont]: "));
        }
        else if (str == "M" || str == "MIDDLE") {
            mode = MODE_SETGEOM;
            textJustify = "Middle";
            setRubberText("TEXT_JUSTIFY", textJustify);
            prompt_output(translate("Specify middle point of text or [Justify/Setfont]: ");
        }
        else if (str == "F" || str == "FIT") {
            mode = MODE_SETGEOM;
            textJustify = "Fit";
            setRubberText("TEXT_JUSTIFY", textJustify);
            prompt_output(translate("Specify start point of text or [Justify/Setfont]: "));
        }
        else if (str == "TL" || str == "TOPLEFT") {
            mode = MODE_SETGEOM;
            textJustify = "Top Left";
            setRubberText("TEXT_JUSTIFY", textJustify);
            prompt_output(translate("Specify top-left point of text or [Justify/Setfont]: "));
        }
        else if (str == "TC" || str == "TOPCENTER") {
            mode = MODE_SETGEOM;
            textJustify = "Top Center";
            setRubberText("TEXT_JUSTIFY", textJustify);
            prompt_output(translate("Specify top-center point of text or [Justify/Setfont]: "));
        }
        else if (str == "TR" || str == "TOPRIGHT") {
            mode = MODE_SETGEOM;
            textJustify = "Top Right";
            setRubberText("TEXT_JUSTIFY", textJustify);
            prompt_output(translate("Specify top-right point of text or [Justify/Setfont]: "));
        }
        else if (str == "ML" || str == "MIDDLELEFT") {
            mode = MODE_SETGEOM;
            textJustify = "Middle Left";
            setRubberText("TEXT_JUSTIFY", textJustify);
            prompt_output(translate("Specify middle-left point of text or [Justify/Setfont]: ");
        }
        else if (str == "MC" || str == "MIDDLECENTER") {
            mode = MODE_SETGEOM;
            textJustify = "Middle Center";
            setRubberText("TEXT_JUSTIFY", textJustify);
            prompt_output(translate("Specify middle-center point of text or [Justify/Setfont]: ");
        }
        else if (str == "MR" || str == "MIDDLERIGHT") {
            mode = MODE_SETGEOM;
            textJustify = "Middle Right";
            setRubberText("TEXT_JUSTIFY", textJustify);
            prompt_output(translate("Specify middle-right point of text or [Justify/Setfont]: ");
        }
        else if (str == "BL" || str == "BOTTOMLEFT") {
            mode = MODE_SETGEOM;
            textJustify = "Bottom Left";
            setRubberText("TEXT_JUSTIFY", textJustify);
            prompt_output(translate("Specify bottom-left point of text or [Justify/Setfont]: ");
        }
        else if (str == "BC" || str == "BOTTOMCENTER") {
            mode = MODE_SETGEOM;
            textJustify = "Bottom Center";
            setRubberText("TEXT_JUSTIFY", textJustify);
            prompt_output(translate("Specify bottom-center point of text or [Justify/Setfont]: "));
        }
        else if (str == "BR" || str == "BOTTOMRIGHT") {
            mode = MODE_SETGEOM;
            textJustify = "Bottom Right";
            setRubberText("TEXT_JUSTIFY", textJustify);
            prompt_output(translate("Specify bottom-right point of text or [Justify/Setfont]: "));
        }
        else {
            alert(translate("Invalid option keyword."));
            prompt_output(translate("Text Justification Options [Center/Right/Align/Middle/Fit/TL/TC/TR/ML/MC/MR/BL/BC/BR]: ");
        }
    }
    case "MODE_SETFONT") {
        mode = "MODE_SETGEOM";
        g->textFont = str;
        setRubberText("TEXT_FONT", g->textFont);
        setTextFont(g->textFont);
        prompt_output(translate("Specify start point of text or [Justify/Setfont]: ");
    }
    case "MODE_SETGEOM") {
        if (std::isnan(textX)) {
            if (str == "J" || str == "JUSTIFY") {
                mode = "MODE_JUSTIFY";
                prompt_output(translate("Text Justification Options [Center/Right/Align/Middle/Fit/TL/TC/TR/ML/MC/MR/BL/BC/BR]: ");
            }
            else if (str == "S" || str == "SETFONT") {
                mode = MODE_SETFONT;
                prompt_output(translate("Specify font name: ");
            }
            else {
                EmbReal strList = str.split(",");
                if (std::isnan(strList[0]) || std::isnan(strList[1])) {
                    alert(translate("Point or option keyword required."));
                    prompt_output(translate("Specify start point of text or [Justify/Setfont]: ");
                }
                else {
                    g->text.x = atof(strList[0]);
                    g->text.y = atof(strList[1]);
                    addRubber("LINE");
                    setRubberMode("LINE");
                    setRubberPoint("LINE_START", textX, textY);
                    prompt_output(translate("Specify text height") + " {" + textSize() + "}: ");
                }
            }
        }
        else if (std::isnan(g->textHeight)) {
            if (str == "") {
                g->textHeight = textSize();
                prompt_output(translate("Specify text angle") + " {" + textAngle() + "}: ");
            }
            else if (std::isnan(str)) {
                alert(translate("Requires valid numeric distance or second point."));
                prompt_output(translate("Specify text height") + " {" + textSize() + "}: ");
            }
            else {
                g->textHeight = atof(str);
                setTextSize(g->textHeight);
                prompt_output(translate("Specify text angle") + " {" + textAngle() + "}: ");
            }
        }
        else if (std::isnan(g->textRotation)) {
            if (str == "") {
                g->textRotation = textAngle();
                prompt_output(translate("Enter text: ");
                mode = "MODE_RAPID";
                prompt->enableRapidFire();
                clearRubber(g);
                addRubber("TEXTSINGLE");
                setRubberMode("TEXTSINGLE");
                setRubberPoint("TEXT_POINT", g->textPos);
                setRubberPoint("TEXT_HEIGHT_ROTATION", g->textHeight, g->textRotation);
                setRubberText("TEXT_FONT", g->textFont);
                setRubberText("TEXT_JUSTIFY", textJustify);
                setRubberText("TEXT_RAPID", g->text);
            }
            else if (std::isnan(str)) {
                alert(translate("Requires valid numeric angle or second point."));
                prompt_output(translate("Specify text angle") + " {" + textAngle() + "}: ");
            }
            else {
                g->textRotation = atof(str);
                setTextAngle(g->textRotation);
                prompt_output(translate("Enter text: ");
                mode = MODE_RAPID;
                prompt->enableRapidFire();
                clearRubber(g);
                addRubber("TEXTSINGLE");
                setRubberMode("TEXTSINGLE");
                setRubberPoint("TEXT_POINT", textX, textY);
                setRubberPoint("TEXT_HEIGHT_ROTATION", g->textHeight, g->textRotation);
                setRubberText("TEXT_FONT", g->textFont);
                setRubberText("TEXT_JUSTIFY", textJustify);
                setRubberText("TEXT_RAPID", g->text);
            }
        }
        else {
            //Do nothing, as we are in rapidFire mode now.
        }
    }
    case MODE_RAPID: {
        if (string_equal(str, "RAPID_ENTER")) {
            if (g->text == "") {
                actuator("end");
            }
            else {
                vulcanize();
                actuator("end");
                /* TODO: Rather than ending the command, calculate where the
                 * next line would be and modify the x/y to the new point.
                 */
            }
        }
        else {
            g->text = atof(str);
            setRubberText("TEXT_RAPID", g->text);
        }
    }
}

/* Snowflake Curve with $t \in [0, 2\pi]$. */
void
Geometry::update_snowflake(void)
{
    for (int i = 0; i <= g->numPoints; i++) {
        EmbReal t = (2.0*CONSTANT_PI) / g->numPoints*i;
        EmbVector v;
        v.x = scale.x * fourier_series(t, snowflake_x);
        v.y = scale.y * fourier_series(t, snowflake_y);

        setRubberPoint("POLYGON_POINT_" + i.toString(), v);
    }

    setRubberText("POLYGON_NUM_POINTS", g->numPoints);
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
Geometry::star_prompt(char *str)
{
    switch (mode) {
    case STAR_MODE_NUM_POINTS: {
        if (str == "" && numPoints >= 3 && numPoints <= 1024) {
            prompt_output(translate("Specify center point: "));
            mode = MODE_STAR_CENTER_PT;
        }
        else {
            EmbReal tmp = atof(str);
            if (std::isnan(tmp) || !isInt(tmp) || tmp < 3 || tmp > 1024) {
                alert(translate("Requires an integer between 3 and 1024."));
                prompt_output(translate("Enter number of star points") + " {" + numPoints.toString() + "}: ");
            }
            else {
                numPoints = tmp;
                prompt_output(translate("Specify center point: "));
                mode = MODE_STAR_CENTER_PT;
            }
        }
        break;
    }

    case MODE_STAR_CENTER_PT: {
        EmbReal strList = str.split(",");
        if (std::isnan(strList[0]) || std::isnan(strList[1])) {
            alert(translate("Invalid point."));
            prompt_output(translate("Specify center point: "));
        }
        else {
            g->center.x = atof(strList[0]);
            g->center.y = atof(strList[1]);
            g->mode = MODE_STAR_RAD_OUTER;
            prompt_output(translate("Specify outer radius of star: "));
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
            alert(translate("Invalid point."));
            prompt_output(translate("Specify outer radius of star: "));
        }
        else {
            point1.x = atof(strList[0]);
            point1.y = atof(strList[1]);
            mode = MODE_RAD_INNER;
            prompt_output(translate("Specify inner radius of star: "));
            updateStar(qsnapX(), qsnapY());
        }
        break;
    }

    case MODE_STAR_RAD_INNER: {
        EmbReal strList = str.split(",");
        if (std::isnan(strList[0]) || std::isnan(strList[1])) {
            alert(translate("Invalid point."));
            prompt_output(translate("Specify inner radius of star: "));
        }
        else {
            point2.x = atof(strList[0]);
            point2.y = atof(strList[1]);
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
    EmbVector v = mouse - center;
    EmbReal angOuter = embVector_angle(v);
    EmbReal distOuter = embVector_length(v);
    EmbReal distInner = distOuter/2.0;

    if (mode == MODE_STAR_RAD_INNER) {
        EmbVector v = point1 - center;
        angOuter = embVector_angle(v);
        distOuter = embVector_length(v);
        distInner = embVector_distance(center, mouse);
    }

    // Calculate the Star Points
    EmbReal angInc = 360.0/(numPoints*2);
    for (int i = 0; i < numPoints*2; i++) {
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
            center.x + v.x,
            center.y + v.y);
    }
    setRubberText("POLYGON_NUM_POINTS", (g->numPoints*2 - 1).toString());
}
#endif

