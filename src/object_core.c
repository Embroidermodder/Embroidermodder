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

#include <stdlib.h>
#include <math.h>

#include "core.h"

#define DESIGN_HEART4          0
#define DESIGN_HEART5          1
#define DESIGN_DOLPHIN         2
#define DESIGN_SNOWFLAKE       3

/* TODO fix this */
EmbReal dolphin_x[] = {
    1.0, 1.0
};
EmbReal dolphin_y[] = {
    1.0, 1.0
};
int32_t dolphin_terms = 2;

EmbReal snowflake_x[] = {
    1.0, 1.0
};
EmbReal snowflake_y[] = {
    1.0, 1.0
};
int32_t snowflake_terms = 2;

/* . */
const char *object_names[] = {
    "Base",
    "Arc",
    "Block",
    "Circle",
    "Aligned Dimension",
    "Angular Dimension",
    "Arclength Dimension",
    "Diameter Dimension",
    "Leader Dimension",
    "Linear Dimension",
    "Ordinate Dimension",
    "Radius Dimension",
    "Ellipse",
    "Image",
    "Infinite Line",
    "Line",
    "Path",
    "Point",
    "Polygon",
    "Polyline",
    "Ray",
    "Rectangle",
    "Multiline Text",
    "Text",
    "Unknown",
    "END"
};

/* TODO: "Aligned"
 * TODO: "Fit"
 */
const char *justify_options[] = {
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
    "Bottom Right",
    "END"
};

char rubber_modes[N_RUBBER_MODES][MAX_STRING_LENGTH] = {
    "CIRCLE_1P_RAD",
    "CIRCLE_1P_DIA",
    "CIRCLE_2P",
    "CIRCLE_3P",
    "CIRCLE_TTR",
    "CIRCLE_TTT",
    "DIMLEADER_LINE",
    "ELLIPSE_LINE",
    "ELLIPSE_MAJORDIAMETER_MINORRADIUS",
    "ELLIPSE_MAJORRADIUS_MINORRADIUS",
    "ELLIPSE_ROTATION",
    "LINE",
    "POLYGON",
    "POLYGON_INSCRIBE",
    "POLYGON_CIRCUMSCRIBE",
    "POLYLINE",
    "RECTANGLE",
    "TEXTSINGLE",
    "END"
};

/* . */
const char rectangle_main_script[][MAX_STRING_LENGTH] = {
    "init",
    "clear-selection",
    "newRect = true",
    "real x1 = 0.0f",
    "real y1 = 0.0f",
    "real x2 = 0.0f",
    "real y2 = 0.0f",
    "set-prompt-prefix-tr Specify first corner point or [Chamfer/Fillet]: "
};

const char *geometry_subcommands[] = {
	"arc",
	"circle",
	"ellipse",
	"horizontal_dimension",
	"image",
	"path",
	"point",
	"polygon",
	"polyline",
	"rectangle",
	"regular_polygon",
	"vertical_dimension",
	"dim_leader",
	"infinite_line",
	"ray",
	"line",
	"triangle",
	"text_multi",
	"text_single",
	"rounded-rectangle",
	"point",
	"slot",
    "END"
};

/*
 */
EmbVector
str_to_vector(char *str, int *error)
{
    EmbVector v;
    prompt_output("TODO: error checking in str_to_vector");
    char *a = strtok(str, ",");
    v.x = atof(a);
    a = strtok(str, NULL);
    v.y = atof(a);
    return v;
}

/*
 */
int32_t
str_to_color(char *str, int *error)
{
    int32_t color = 0;
    prompt_output("TODO: error checking in str_to_color");
    char *a = strtok(str, ",");
    color += atoi(a);
    a = strtok(str, NULL);
    color += 0x100*atoi(a);
    a = strtok(str, NULL);
    color += 0x10000*atoi(a);
    return color;
}

/* . */
void
init_command(void)
{

}

/* . */
void
end_command(void)
{

}

/* . */
void
vulcanize(void)
{

}

/* . */
EmbVector
embVector_make(EmbReal x, EmbReal y)
{
    EmbVector v;
    v.x = x;
    v.y = y;
    return v;
}

/* . */
void
report_vector(char *label, EmbVector v)
{
    char output[MAX_STRING_LENGTH];
    sprintf(output, "%s: x = %f, y = %f", translate(label), v.x, v.y);
    prompt_output(output);
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
report_distance(EmbVector point1, EmbVector point2)
{
    char output[MAX_STRING_LENGTH];
    EmbVector delta = embVector_subtract(point2, point1);
    EmbReal dist = embVector_length(delta);
    EmbReal angle = embVector_angle(delta);

    sprintf(output, "%s = %f, %s = %f",
        translate("Distance"), dist, translate("Angle"), angle);
    prompt_output(output);
    report_vector("Delta", delta);
}

/* . */
void
update_curve(int style, int numPoints, EmbVector scale)
{
    EmbReal h = (2.0*embConstantPi) / numPoints;
    for (int i = 0; i <= numPoints; i++) {
        EmbVector v = embVector_make(1.0f, 1.0f);
        EmbReal t = h * i;

        switch (style) {
        case DESIGN_DOLPHIN: {
            v.x = fourier_series(t, dolphin_x, dolphin_terms);
            v.y = fourier_series(t, dolphin_y, dolphin_terms);
            break;
        }

        case DESIGN_SNOWFLAKE: {
            v.x = fourier_series(t, snowflake_x, snowflake_terms);
            v.y = fourier_series(t, snowflake_y, snowflake_terms);
            break;
        }

        case DESIGN_HEART4: {
            v.x = cos(t)*((sin(t)*sqrt(abs(cos(t))))/(sin(t)+7/5) - 2*sin(t) + 2);
            v.y = sin(t)*((sin(t)*sqrt(abs(cos(t))))/(sin(t)+7/5) - 2*sin(t) + 2);
            break;
        }

        case DESIGN_HEART5: {
            v.x = 16*pow(sin(t), 3);
            v.y = 13*cos(t) - 5*cos(2*t) - 2*cos(3*t) - cos(4*t);
            break;
        }

        default:
            break;
        }

        v.x *= scale.x;
        v.y *= scale.y;
        /* setRubberPoint("POLYGON_POINT_" + i.toString(), v); */
    }

    /* setRubberText("POLYGON_NUM_POINTS", numPoints.toString()); */
}

/* . */
GeometryData*
geometry_init(int type)
{
    GeometryData *g = malloc(sizeof(GeometryData));
    g->preview = 1;
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

    /*
    case MODE_LOCATE_POINT: {
        init_command();
        clear_selection();
        prompt_output(translate("Specify point: "));
        break;
    }

    case MODE_LINE: {
        init_command();
        clear_selection();
        firstRun = true;
        first = embVector_make(0.0f, 0.0f);
        prev = embVector_make(0.0f, 0.0f);
        prompt_output(translate("Specify first point: "));
        break;
    }

    case MODE_POLYGON: {
        init_command();
        clear_selection();
        center = embVector_make(0.0f, 0.0f);
        side1 = embVector_make(0.0f, 0.0f);
        side2 = embVector_make(0.0f, 0.0f);
        pointI = embVector_make(0.0f, 0.0f);
        pointC = embVector_make(0.0f, 0.0f);
        polyType = "Inscribed"; //Default
        numSides = 4;           //Default
        mode = MODE_POLYGON_NUM_SIDES;
        prompt_output(translate("Enter number of sides" + " {" + numSides.toString() + "}: ");
        break;
    }

    case MODE_DESIGN_INIT:
        g->numPoints = 512; //Default //TODO: min:64 max:8192
        g->center.x = 0.0f;
        g->center.y = 0.0f;
        g->scale.x = 1.0f;
        g->scale.y = 1.0f;
        init_command();
        clear_selection();
        g->mode = MODE_NUM_POINTS;

        //Heart4: 10.0 / 512
        //Heart5: 1.0 / 512

        addRubber("POLYGON");
        setRubberMode("POLYGON");
        updateHeart("HEART5", numPoints, scale);
        spareRubber("POLYGON");
        end_command();
        break;

    case MODE_ERASE:
        init_command();

        if (numSelected() <= 0) {
            //TODO: Prompt to select objects if nothing is preselected
            prompt_output(translate("Preselect objects before invoking the delete command."));
            end_command();
            messageBox("information", tr("Delete Preselect"), tr("Preselect objects before invoking the delete command."));
        }
        else {
            deleteSelected();
            end_command();
        }
        break;
    */

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

    case MODE_PATH: {
        firstRun = true;
        first = {0.0, 0.0};
        prev = {0.0, 0.0};
        init_command();
        clear_selection();
        prompt_output(translate("Specify start point: "));
        break;
    }
    */

    default:
        break;
    }
    return g;
}

/* . */
void
geometry_free(GeometryData *g)
{
    free(g);
}

/* . */
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

    /*
    case MODE_LINE: {
        if (firstRun) {
            firstRun = false;
            first = v;
            prev = v;
            addRubber("LINE");
            setRubberMode("LINE");
            setRubberPoint("LINE_START", first);
            prompt_output(translate("Specify next point or [Undo]: "));
        }
        else {
            setRubberPoint("LINE_END", v);
            vulcanize();
            addRubber("LINE");
            setRubberMode("LINE");
            setRubberPoint("LINE_START", v);
            prev = v;
        }
        break;
    }

    case MODE_TEXT_SINGLE_SETGEOM: {
        if (std::isnan(textX)) {
            g->position = v;
            addRubber("LINE");
            setRubberMode("LINE");
            setRubberPoint("LINE_START", v);
            prompt_output(translate("Specify text height" + " {" + textSize() + "}: ");
        }
        else if (std::isnan(g->textHeight)) {
            g->textHeight = embVector_distance(g->position, v);
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
        break;
    }

    case MODE_RECTANGLE: {
        if (firstRun) {
            firstRun = false;
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
            end_command();
        }
        break;
    }

    case MODE_QUICKLEADER_POINT_1: {
        g->point1 = v;
        addRubber("DIMLEADER");
        setRubberMode("DIMLEADER_LINE");
        setRubberPoint("DIMLEADER_LINE_START", g->point1);
        prompt_output(translate("Specify second point: "));
        break;
    }

    case MODE_QUICKLEADER_POINT_2: {
        g->point2 = v;
        setRubberPoint("DIMLEADER_LINE_END", g->point2);
        vulcanize();
        end_command();
        break;
    }

    case MODE_DISTANCE_POINT1: {
        point1 = v;
        addRubber("LINE");
        setRubberMode("LINE");
        setRubberPoint("LINE_START", point1);
        prompt_output(translate("Specify second point: "));
        break;
    }

    case MODE_DISTANCE_POINT1: {
        point2 = v;
        report_distance(g->point1, g->point2);
        end_command();
        break;
    }

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
            center = embVecgtor_average(point1, point2);
            width = embVecgtor_distance(point1, point2);
            rot = embVecgtor_angle(point1, point2);
            setRubberMode("ELLIPSE_MAJORDIAMETER_MINORRADIUS");
            setRubberPoint("ELLIPSE_AXIS1_POINT1", point1);
            setRubberPoint("ELLIPSE_AXIS1_POINT2", point2);
            setRubberPoint("ELLIPSE_CENTER", center.x, center.y);
            setRubberPoint("ELLIPSE_WIDTH", width, 0);
            setRubberPoint("ELLIPSE_ROT", rot, 0);
            prompt_output(translate("Specify second axis end point or [Rotation]: ");
        }
        else if (std::isnan(point3.x)) {
            point3.x = x;
            point3.y = y;
            height = perpendicularDistance(point3, point1, point2)*2.0;
            setRubberPoint("ELLIPSE_AXIS2_POINT2", point3);
            vulcanize();
            end_command();
        }
        else {
            error("ELLIPSE", tr("This should never happen."));
        }
    }
    case MODE_ELLIPSE_MAJORRADIUS_MINORRADIUS: {
        if (std::isnan(point1.x)) {
            point1 = v;
            center = point1;
            addRubber("ELLIPSE");
            setRubberMode("ELLIPSE_LINE");
            setRubberPoint("ELLIPSE_LINE_POINT1", point1);
            setRubberPoint("ELLIPSE_CENTER", center);
            prompt_output(translate("Specify first axis end point: ");
        }
        else if (std::isnan(point2.x)) {
            point2 = v;
            width = calculateDistance(center, point2)*2.0;
            rot = calculateAngle(point1, point2);
            setRubberMode("ELLIPSE_MAJORRADIUS_MINORRADIUS");
            setRubberPoint("ELLIPSE_AXIS1_POINT2", point2);
            setRubberPoint("ELLIPSE_WIDTH", width, 0);
            setRubberPoint("ELLIPSE_ROT", rot, 0);
            prompt_output(translate("Specify second axis end point or [Rotation]: ");
        }
        else if (std::isnan(point3.x)) {
            point3 = v;
            height = perpendicularDistance(point3, center, point2)*2.0;
            setRubberPoint("ELLIPSE_AXIS2_POINT2", point3);
            vulcanize();
            end_command();
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
            EmbReal angle = calculateAngle(center, v);
            height = cos(angle*PI/180.0)*width;
            addEllipse(center.x, center.y, width, height, rot, false);
            end_command();
        }
    }

    case MODE_LINE_MOVE: {
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
            g->preview = false;
            end_command();
        }
        break;
    }

    case MODE_DESIGN_MOVE: {
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
    }
*/

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
        end_command();
        break;
    }

    case MODE_POINT: {
        init_command();
        clear_selection();
        g->firstRun = true;
        setPromptPrefix("TODO: Current point settings: PDMODE=?  PDSIZE=?"); //TODO: tr needed here when complete
        prompt_output(translate("Specify first point: "));
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
        char msg[MAX_STRING_LENGTH];
        sprintf(msg, "%s {%s}: ",
            translate("Specify polygon type [Inscribed in circle/Circumscribed around circle]"),
            g->polyType);
        prompt_output(msg);
        break;
    }

    case MODE_POLYGON_INSCRIBE: {
        g->pointI = v;
        /* setRubberPoint("POLYGON_INSCRIBE_POINT", g->pointI); */
        vulcanize();
        end_command();
        break;
    }

    case MODE_POLYGON_CIRCUMSCRIBE: {
        g->pointC = v;
        /* setRubberPoint("POLYGON_CIRCUMSCRIBE_POINT", g->pointC); */
        vulcanize();
        end_command();
        break;
    }

    case MODE_POLYGON_SIDE_LEN: {
        prompt_output("TODO: POLYGON Sidelength mode");
        break;
    }

    /*
    case MODE_RGB_BACKGROUND:
    case MODE_RGB_CROSSHAIR:
    case MODE_RGB_GRID: {
        //Do Nothing, prompt only command.
    }
    */

    default:
        break;
    }
}

/* . */
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

/*
    case MODE_DISTANCE_POINT1: {
        int error = 0;
        EmbVector v = str_to_vector(str, &error);
        if (error) {
            prompt_output(translate("Requires numeric distance or two points."));
            prompt_output(translate("Specify first point: "));
        }
        else {
            point1 = v;
            addRubber("LINE");
            setRubberMode("LINE");
            setRubberPoint("LINE_START", point1);
            prompt_output(translate("Specify second point: "));
        }
        break;
    }

    case MODE_DISTANCE_POINT2: {
        int error = 0;
        EmbVector v = str_to_vector(str, &error);
        if (error) {
            prompt_output(translate("Requires numeric distance or two points."));
            prompt_output(translate("Specify second point: "));
        }
        else {
            point2 = v;
            report_distance();
            end_command();
        }
        break;
    }

    case MODE_ELLIPSE_MAJORDIAMETER_MINORRADIUS: {
        if (std::isnan(point1.x)) {
            if (str == "C" || str == "CENTER") {
                mode = MODE_MAJORRADIUS_MINORRADIUS;
                prompt_output(translate("Specify center point: "));
            }
            else {
                EmbVector v = str_to_vector(str, &error);
                if (error) {
                    prompt_output(translate("Point or option keyword required."));
                    prompt_output(translate("Specify first axis start point or [Center]: "));
                }
                else {
                    point1 = v;
                    addRubber("ELLIPSE");
                    setRubberMode("ELLIPSE_LINE");
                    setRubberPoint("ELLIPSE_LINE_POINT1", point1);
                    prompt_output(translate("Specify first axis end point: "));
                }
            }
        }
        else if (std::isnan(point2.x)) {
            EmbVector v = str_to_vector(str, &error);
            if (error) {
                prompt_output(translate("Invalid point."));
                prompt_output(translate("Specify first axis end point: "));
            }
            else {
                point2 = v;
                center = embVector_average(point1, point2);
                width = embVector_distance(point1, point2);
                rot = embVector_angle(point1, point2);
                setRubberMode("ELLIPSE_MAJORDIAMETER_MINORRADIUS");
                setRubberPoint("ELLIPSE_AXIS1_POINT1", point1);
                setRubberPoint("ELLIPSE_AXIS1_POINT2", point2);
                setRubberPoint("ELLIPSE_CENTER", center);
                setRubberPoint("ELLIPSE_WIDTH", width, 0);
                setRubberPoint("ELLIPSE_ROT", rot, 0);
                prompt_output(translate("Specify second axis end point or [Rotation]: "));
            }
        }
        else if (std::isnan(point3.x)) {
            if (str == "R" || str == "ROTATION") {
                mode = MODE_ELLIPSE_ROTATION;
                prompt_output(translate("Specify rotation: ");
            }
            else {
                EmbVector v = str_to_vector(str, &error);
                if (error) {
                    prompt_output(translate("Point or option keyword required."));
                    prompt_output(translate("Specify second axis end point or [Rotation]: ");
                }
                else {
                    point3 = v;
                    height = perpendicularDistance(point3, point1, point2)*2.0;
                    setRubberPoint("ELLIPSE_AXIS2_POINT2", point3);
                    vulcanize();
                    end_command();
                }
            }
        }
        break;
    }
    case MODE_ELLIPSE_MAJORRADIUS_MINORRADIUS: {
        if (std::isnan(point1.x)) {
            EmbVector v = str_to_vector(str, &error);
            if (error) {
                prompt_output(translate("Invalid point."));
                prompt_output(translate("Specify center point: ");
            }
            else {
                point1 = v;
                center = point1;
                addRubber("ELLIPSE");
                setRubberMode("ELLIPSE_LINE");
                setRubberPoint("ELLIPSE_LINE_POINT1", point1);
                setRubberPoint("ELLIPSE_CENTER", center);
                prompt_output(translate("Specify first axis end point: ");
            }
        }
        else if (std::isnan(point2.x)) {
            EmbVector v = str_to_vector(str, &error);
            if (error) {
                prompt_output(translate("Invalid point."));
                prompt_output(translate("Specify first axis end point: "));
            }
            else {
                point2 = v;
                width = embVector_distance(point1, point2)*2.0;
                rot = embVector_angle(point1, point2);
                setRubberMode("ELLIPSE_MAJORRADIUS_MINORRADIUS");
                setRubberPoint("ELLIPSE_AXIS1_POINT2", point2);
                setRubberPoint("ELLIPSE_WIDTH", width, 0);
                setRubberPoint("ELLIPSE_ROT", rot, 0);
                prompt_output(translate("Specify second axis end point or [Rotation]: "));
            }
        }
        else if (std::isnan(point3.x)) {
            if (str == "R" || str == "ROTATION") {
                mode = MODE_ELLIPSE_ROTATION;
                prompt_output(translate("Specify ellipse rotation: ");
            }
            else {
                EmbVector v = str_to_vector(str, &error);
                if (error) {
                    prompt_output(translate("Point or option keyword required."));
                    prompt_output(translate("Specify second axis end point or [Rotation]: ");
                }
                else {
                    point3 = v;
                    height = perpendicularDistance(point3, point1, point2)*2.0;
                    setRubberPoint("ELLIPSE_AXIS2_POINT2", point3);
                    vulcanize();
                    end_command();
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
                prompt_output(translate("Invalid angle. Input a numeric angle or pick a point."));
                prompt_output(translate("Specify rotation: ");
            }
            else {
                EmbReal angle = atof(str);
                height = cos(angle*PI/180.0)*width;
                addEllipse(center, width, height, rot, false);
                end_command();
            }
        }
        break;
    }

    case MODE_LINE: {
        if (firstRun) {
            EmbVector v = str_to_vector(str, &error);
            if (error) {
                prompt_output(translate("Invalid point."));
                prompt_output(translate("Specify first point: "));
            }
            else {
                firstRun = false;
                first = v;
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
                EmbVector v = str_to_vector(str, &error);
                if (error) {
                    prompt_output(translate("Point or option keyword required."));
                    prompt_output(translate("Specify next point or [Undo]: ");
                }
                else {
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
        break;
    }
*/
    case MODE_RECTANGLE:
        break;


/*
    case STAR_MODE_NUM_POINTS: {
        if (str == "" && numPoints >= 3 && numPoints <= 1024) {
            prompt_output(translate("Specify center point: "));
            mode = MODE_STAR_CENTER_PT;
        }
        else {
            EmbReal tmp = atof(str);
            if (std::isnan(tmp) || !isInt(tmp) || tmp < 3 || tmp > 1024) {
                prompt_output(translate("Requires an integer between 3 and 1024."));
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
        int error = 0;
        EmbVector v = str_to_vector(str, &error);
        if (error) {
            prompt_output(translate("Invalid point."));
            prompt_output(translate("Specify center point: "));
        }
        else {
            g->center = v;
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
        int error = 0;
        EmbVector v = str_to_vector(str, &error);
        if (error) {
            prompt_output(translate("Invalid point."));
            prompt_output(translate("Specify outer radius of star: "));
        }
        else {
            point1 = v;
            mode = MODE_RAD_INNER;
            prompt_output(translate("Specify inner radius of star: "));
            updateStar(qsnapX(), qsnapY());
        }
        break;
    }

    case MODE_STAR_RAD_INNER: {
        int error = 0;
        EmbVector v = str_to_vector(str, &error);
        if (error) {
            prompt_output(translate("Invalid point."));
            prompt_output(translate("Specify inner radius of star: "));
        }
        else {
            point2 = v;
            actuator("disable move-rapid-fire");
            updateStar(point2);
            spareRubber("POLYGON");
            end_command();
        }
        break;
    }
*/
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

/* . */
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
        prompt_output("todo DISTANCE context()");
        break;
    }

    /*
    case MODE_LINE:
        prompt_output("todo LINE context()");
        break;

    case MODE_LOCATEPOINT:
        prompt_output("todo LOCATEPOINT context()");
        break;

    case MODE_MOVE:
        prompt_output("todo MOVE context()");
        break;

    case MODE_PATH:
        prompt_output("todo PATH context()");
        break;

    case MODE_POINT:
        prompt_output("todo POINT context()");
        break;

    case MODE_POLYLINE:
        prompt_output("todo POLYLINE context()");
        break;

    case MODE_QUICKLEADER:
        prompt_output("todo QUICKLEADER context()");
        break;

    case MODE_RGB:
        prompt_output("todo RGB context()");
        break;

    case MODE_ROTATE:
        prompt_output("todo ROTATE context()");
        break;

    case MODE_SCALE:
        prompt_output("todo SCALE context()");
        break;

    case MODE_SINGLELINETEXT:
        prompt_output("todo SINGLELINETEXT context()");
        break;

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

/* . */
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

/* . */
void
geometry_set_flag(GeometryData *g, uint64_t flag)
{

}

/* . */
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

/* . */
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

/* . */
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

/* Set. */
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

/* . */
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

/* . */
void
set_int(GeometryData *g, int64_t id, int32_t i)
{
    switch (id) {

    default:
        break;
    }
}

/* . */
void
set_str(GeometryData *g, int64_t id, char *str)
{
    switch (id) {

    default:
        break;
    }
}

/* . */
void
locate_point_click(EmbVector v)
{
    report_vector("v", v);
    end_command();
}

/* . */
void
locate_point_prompt(char *args)
{
    int error = 0;
    EmbVector v = str_to_vector(args, &error);
    if (error) {
        prompt_output(translate("Invalid point."));
        prompt_output(translate("Specify point: "));
    }
    else {
        report_vector("v", v);
        end_command();
    }
}

/* . */
void
move_main(void)
{
    init_command();
    /*
    g->firstRun = true;
    g->base = embVector_make(0.0f, 0.0f);
    g->dest = g->base;
    g->delta = g->base;

    if (numSelected() <= 0) {
        //TODO: Prompt to select objects if nothing is preselected
        prompt_output(translate("Preselect objects before invoking the move command."));
        end_command();
        messageBox("information", tr("Move Preselect"), tr("Preselect objects before invoking the move command."));
    }
    else {
        prompt_output(translate("Specify base point: "));
    }
    */
}

/*
void
move_prompt(char *str)
{
    if (firstRun) {
        EmbVector v = str_to_vector(str, &error);
        if (error) {
            prompt_output(translate("Invalid point."));
            prompt_output(translate("Specify base point: "));
        }
        else {
            firstRun = false;
            base = v;
            addRubber("LINE");
            setRubberMode("LINE");
            setRubberPoint("LINE_START", base);
            previewOn("SELECTED", "MOVE", base, 0);
            prompt_output(translate("Specify destination point: "));
        }
    }
    else {
        EmbVector v = str_to_vector(str, &error);
        if (error) {
            prompt_output(translate("Invalid point."));
            prompt_output(translate("Specify destination point: "));
        }
        else {
            dest = v;
            delta = embVector_subtract(dest, base);
            moveSelected(delta);
            g->preview = false;
            end_command();
        }
    }
}
*/

/* . */
void
path_click(EmbVector v)
{
    /*
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
    */
}

/*
void
path_prompt(char *cmd)
{
    if (string_equal(cmd, "A") || string_equal(cmd, "ARC")) {
        actuator("todo PATH prompt() for ARC");
    }
    else if (string_equal(cmd, "U") || string_equal(cmd, "UNDO")) {
        actuator("todo PATH prompt() for UNDO");
    }
    else {
        EmbVector v = str_to_vector(str, &error);
        if (error) {
            prompt_output(translate("Point or option keyword required."));
            prompt_output(translate("Specify next point or [Arc/Undo]: ");
        }
        else {
            if (firstRun) {
                firstRun = false;
                first = v;
                prev = v;
                addPath(x, y);
                prompt_output(translate("Specify next point or [Arc/Undo]: "));
            }
            else {
                appendLineToPath(x, y);
                prev = v;
            }
        }
    }
}

void point_click(EmbVector v)
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

void point_prompt(char *str)
{
    if (firstRun) {
        if (string_equal(cmd, "M") || string_equal(cmd, "MODE")) {
            actuator("todo POINT prompt() for PDMODE");
        }
        else if (str == "S" || str == "SIZE") {
            actuator("todo POINT prompt() for PDSIZE");
        }
        EmbVector v = str_to_vector(str, &error);
        if (error) {
            prompt_output(translate("Invalid point."));
            prompt_output(translate("Specify first point: "));
        }
        else {
            firstRun = false;
            prompt_output(translate("Specify next point: "));
            addPoint(v);
        }
    }
    else {
        EmbVector v = str_to_vector(str, &error);
        if (error) {
            prompt_output(translate("Invalid point."));
        }
        else {
            addPoint(v);
        }
		prompt_output(translate("Specify next point: "));
    }
}

void polygon_prompt(char *str)
{
    case MODE_POLYGON_NUM_SIDES: {
        if (str == "" && numSides >= 3 && numSides <= 1024) {
            prompt_output(translate("Specify center point or [Sidelength]: ");
            mode = MODE_CENTER_PT;
        }
        else {
            EmbReal tmp = atof(str);
            if (std::isnan(tmp) || !isInt(tmp) || tmp < 3 || tmp > 1024) {
                prompt_output(translate("Requires an integer between 3 and 1024."));
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
            EmbVector v = str_to_vector(str, &error);
            if (error) {
                prompt_output(translate("Point or option keyword required."));
                prompt_output(translate("Specify center point or [Sidelength]: "));
            }
            else {
                center = v;
                mode = MODE_POLYTYPE;
                prompt_output(translate("Specify polygon type [Inscribed in circle/Circumscribed around circle]") + " {" + g->polyType + "}: ");
            }
        }
    }
    case MODE_POLYTYPE: {
        if (str == "INSCRIBED") {
            mode = MODE_INSCRIBE;
            polyType = "Inscribed";
            prompt_output(translate("Specify polygon corner point or [Distance]: ");
            addRubber("POLYGON");
            setRubberMode("POLYGON_INSCRIBE");
            setRubberPoint("POLYGON_CENTER", center);
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
                setRubberPoint("POLYGON_CENTER", center);
                setRubberPoint("POLYGON_NUM_SIDES", numSides, 0);
            }
            else if (polyType == "Circumscribed") {
                mode = MODE_CIRCUMSCRIBE;
                prompt_output(translate("Specify polygon side point or [Distance]: ");
                addRubber("POLYGON");
                setRubberMode("POLYGON_CIRCUMSCRIBE");
                setRubberPoint("POLYGON_CENTER", center);
                setRubberPoint("POLYGON_NUM_SIDES", numSides, 0);
            }
            else {
                error("POLYGON", tr("Polygon type is not Inscribed or Circumscribed."));
            }
        }
        else {
            prompt_output(translate("Invalid option keyword."));
            prompt_output(translate("Specify polygon type [Inscribed in circle/Circumscribed around circle]") + " {" + polyType + "}: ");
        }
    }
    case MODE_INSCRIBE: {
        if (str == "D" || str == "DISTANCE") {
            mode = MODE_DISTANCE;
            prompt_output(translate("Specify distance: "));
        }
        else {
            EmbVector v = str_to_vector(str, &error);
            if (error) {
                prompt_output(translate("Point or option keyword required."));
                prompt_output(translate("Specify polygon corner point or [Distance]: "));
            }
            else {
                pointI = v;
                setRubberPoint("POLYGON_INSCRIBE_POINT", pointI);
                vulcanize();
                end_command();
            }
        }
    }
    case MODE_CIRCUMSCRIBE) {
        if (str == "D" || str == "DISTANCE") {
            mode = MODE_DISTANCE;
            prompt_output(translate("Specify distance: "));
        }
        else {
            EmbVector v = str_to_vector(str, &error);
            if (error) {
                prompt_output(translate("Point or option keyword required."));
                prompt_output(translate("Specify polygon side point or [Distance]: "));
            }
            else {
                pointC = v;
                setRubberPoint("POLYGON_CIRCUMSCRIBE_POINT", pointC);
                vulcanize();
                end_command();
            }
        }
    }
    case MODE_DISTANCE) {
        if (std::isnan(str)) {
            prompt_output(translate("Requires valid numeric distance."));
            prompt_output(translate("Specify distance: "));
        }
        else {
            if (polyType == "Inscribed") {
                pointI = center;
                pointI.y += atof(str);
                setRubberPoint("POLYGON_INSCRIBE_POINT", pointI);
                vulcanize();
                end_command();
            }
            else if (polyType == "Circumscribed") {
                pointC = center;
                pointC.y += atof(str);
                setRubberPoint("POLYGON_CIRCUMSCRIBE_POINT", pointC);
                vulcanize();
                end_command();
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

void polyline_main(void)
{
    script(polyline_init);
}

void polyline_click(EmbVector v)
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

void polyline_prompt(char *str)
{
    if (firstRun.b) {
        EmbVector v = str_to_vector(str, &error);
        if (error) {
            prompt_output(translate("Invalid point."));
            prompt_output(translate("Specify first point: "));
        }
        else {
            firstRun = false;
            first = v;
            prev = first;
            addRubber("POLYLINE");
            setRubberMode("POLYLINE");
            setRubberPoint("POLYLINE_POINT_0", first);
            prompt_output(translate("Specify next point or [Undo]: ");
        }
    }
    else {
        if (str == "U" || str == "UNDO") {
            actuator("todo POLYLINE prompt() for UNDO");
        }
        else {
            EmbVector v = str_to_vector(str, &error);
            if (error) {
                prompt_output(translate("Point or option keyword required."));
                prompt_output(translate("Specify next point or [Undo]: ");
            }
            else {
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

quickleader_main
    todo "Adding the text is not complete yet.";
    init_command();
    clear_selection();
    point1 = embVector_make(0.0f, 0.0f);
    point2 = embVector_make(0.0f, 0.0f);
    set-prompt-prefix-tr Specify first point: "

void quickleader_prompt(char *str)
{
    EmbVector v = str_to_vector(str, &error);
    if (std::isnan(point1.x)) {
        if (error) {
            prompt_output(translate("Requires two points."));
            prompt_output(translate("Specify first point: "));
        }
        else {
            point1 = v;
            addRubber("DIMLEADER");
            setRubberMode("DIMLEADER_LINE");
            setRubberPoint("DIMLEADER_LINE_START", point1);
            prompt_output(translate("Specify second point: ");
        }
    }
    else {
        if (error) {
            prompt_output(translate("Requires two points."));
            prompt_output(translate("Specify second point: ");
        }
        else {
            point2 = v;
            setRubberPoint("DIMLEADER_LINE_END", point2);
            vulcanize();
            end_command();
        }
    }
}

void rectangle_prompt(char *str)
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
        EmbVector v = str_to_vector(str, &error);
        if (error) {
            prompt_output(translate("Invalid point."));
            prompt_output(translate("Specify first point: "));
        }
        else {
            if (g->firstRun) {
                g->firstRun = false;
                point1 = v;
                addRubber("RECTANGLE");
                setRubberMode("RECTANGLE");
                setRubberPoint("RECTANGLE_START", v);
                prompt_output(translate("Specify other corner point or [Dimensions]: "));
            }
            else {
                point2 = v;
                setRubberPoint("RECTANGLE_END", v);
                vulcanize();
                end_command();
            }
        }
    }
}

void rgb_main(void)
{
    init_command();
    clear_selection();
    g->mode = MODE_RGB_BACKGROUND;
    prompt_output(translate("Enter RED,GREEN,BLUE values for background or [Crosshair/Grid]: "));
}

void rgb_prompt(char *str)
    case MODE_RGB_BACKGROUND: {
        if (str == "C" || str == "CROSSHAIR") {
            g->mode = MODE_RGB_CROSSHAIR;
            prompt_output(translate("Specify crosshair color: "));
        }
        else if (str == "G" || str == "GRID") {
            g->mode = MODE_RGB_GRID;
            prompt_output(translate("Specify grid color: "));
        }
        else {
            int32_t color = str_to_color(str, &error);
            if (!validRGB(color)) {
                prompt_output(translate("Invalid color. R,G,B values must be in the range of 0-255."));
                prompt_output(translate("Specify background color: "));
            }
            else {
                setBackgroundColor(color);
                end_command();
            }
        }
        break;
    }

    case MODE_RGB_CROSSHAIR: {
		int32_t color = str_to_color(str, &error);
		if (!validRGB(color)) {
            prompt_output(translate("Invalid color. R,G,B values must be in the range of 0-255."));
            prompt_output(translate("Specify crosshair color: "));
        }
        else {
            setCrossHairColor(color);
            end_command();
        }
        break;
    }

    case RGB_MODE_GRID: {
		int32_t color = str_to_color(str, &error);
		if (!validRGB(color)) {
            prompt_output(translate("Invalid color. R,G,B values must be in the range of 0-255."));
            prompt_output(translate("Specify grid color: "));
        }
        else {
            setGridColor(color);
            end_command();
        }
        break;
    }
}

void rotate_main(char *args)
{
    geometry_init(g);
    g->mode = ROTATE_MODE_NORMAL;
    g->firstRun = true;
    g->base = embVector_make(0.0f, 0.0f);
    g->dest = embVector_make(0.0f, 0.0f);
    g->angle = 0.0f;

    g->baseR = embVector_make(0.0f, 0.0f);
    g->destR = embVector_make(0.0f, 0.0f);
    g->angleRef = 0.0f;
    g->angleNew = 0.0f;

    if (numSelected() <= 0) {
        //TODO: Prompt to select objects if nothing is preselected
        prompt_output(translate("Preselect objects before invoking the rotate command."));
        end_command();
        messageBox("information", tr("Rotate Preselect"), tr("Preselect objects before invoking the rotate command."));
    }
    else {
        prompt_output(translate("Specify base point: "));
    }
}

void rotate_click(EmbVector v)
{
    switch (mode) {
    case MODE_ROTATE_NORMAL:
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
            g->dest = v;
            angle = calculateAngle(g->base, g->dest);
            rotateSelected(g->base, angle);
            g->preview = false;
            end_command();
        }
    }
    case MODE_ROTATE_REFERENCE: {
        if (std::isnan(g->baseR.x)) {
            g->baseR = v;
            addRubber("LINE");
            setRubberMode("LINE");
            setRubberPoint("LINE_START", g->baseR);
            prompt_output(translate("Specify second point: "));
        }
        else if (std::isnan(destR.x)) {
            g->destR = v;
            angleRef = calculateAngle(g->baseR, g->destR);
            setRubberPoint("LINE_START", g->base);
            previewOn("SELECTED", "ROTATE", g->base, g->angleRef);
            prompt_output(translate("Specify the new angle: "));
        }
        else if (std::isnan(angleNew)) {
            angleNew = embVector_angle(base, v);
            rotateSelected(base, angleNew - angleRef);
            g->preview = false;
            end_command();
        }
    }
}

void rotate_prompt(char *str)
{
    if (mode == ROTATE_MODE_NORMAL) {
        if (firstRun) {
            EmbVector v = str_to_vector(str, &error);
            if (error) {
                prompt_output(translate("Invalid point."));
                prompt_output(translate("Specify base point: ");
            }
            else {
                firstRun = false;
                base = v;
                addRubber("LINE");
                setRubberMode("LINE");
                setRubberPoint("LINE_START", base);
                previewOn("SELECTED", "ROTATE", base, 0);
                prompt_output(translate("Specify rotation angle or [Reference]: ");
            }
        }
        else {
            if (str == "R" || str == "REFERENCE") {
                mode = MODE_REFERENCE;
                prompt_output(translate("Specify the reference angle") + " {0.00}: ");
                clearRubber();
                g->preview = false;
            }
            else {
                if (std::isnan(str)) {
                    prompt_output(translate("Requires valid numeric angle, second point, or option keyword."));
                    prompt_output(translate("Specify rotation angle or [Reference]: ");
                }
                else {
                    angle = atof(str);
                    rotateSelected(baseX, baseY, angle);
                    g->preview = false;
                    end_command();
                }
            }
        }
    }
    case MODE_REFERENCE) {
        if (std::isnan(g->baseRX)) {
            if (std::isnan(str)) {
                EmbVector v = str_to_vector(str, &error);
                if (error) {
                    prompt_output(translate("Requires valid numeric angle or two points."));
                    prompt_output(translate("Specify the reference angle {0.00}: ");
                }
                else {
                    g->baseR = v;
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
                setRubberPoint("LINE_START", g->base);
                previewOn("SELECTED", "ROTATE", g->base, angleRef);
                prompt_output(translate("Specify the new angle: "));
            }
        }
        else if (std::isnan(destRX)) {
            if (std::isnan(str)) {
                EmbVector v = str_to_vector(str, &error);
                if (error) {
                    prompt_output(translate("Requires valid numeric angle or two points."));
                    prompt_output(translate("Specify second point: "));
                }
                else {
                    g->destR = v;
                    angleRef = calculateAngle(g->baseR, g->destR);
                    previewOn("SELECTED", "ROTATE", g->base, angleRef);
                    setRubberPoint("LINE_START", g->base);
                    prompt_output(translate("Specify the new angle: "));
                }
            }
            else {
                // The base and dest values are only set here to advance the command.
                baseR = {0.0, 0.0};
                destR = {0.0, 0.0};
                // The reference angle is what we will use later.
                angleRef = atof(str);
                previewOn("SELECTED", "ROTATE", g->base, angleRef);
                prompt_output(translate("Specify the new angle: "));
            }
        }
        else if (std::isnan(angleNew)) {
            if (std::isnan(str)) {
                EmbVector v = str_to_vector(str, &error);
                if (error) {
                    prompt_output(translate("Requires valid numeric angle or second point."));
                    prompt_output(translate("Specify the new angle: ");
                }
                else {
                    angleNew = calculateAngle(base, v);
                    rotateSelected(base, angleNew - angleRef);
                    g->preview = false;
                    end_command();
                }
            }
            else {
                angleNew = atof(str);
                rotateSelected(base, angleNew - angleRef);
                g->preview = false;
                end_command();
            }
        }
    }
}

void sandbox_main(char *str)
{
    init_command();

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

    end_command();
}

void scale_main(void)
{
    init_command();

    mode = MODE_SCALE_NORMAL;
    firstRun = true;
    base = embVector_make(0.0f, 0.0f);
    dest = embVector_make(0.0f, 0.0f);
    factor = 0.0f;

    baseR = base;
    destR = dest;
    factorRef = 0.0f;
    factorNew = 0.0f;

    if (numSelected() <= 0) {
        //TODO: Prompt to select objects if nothing is preselected
        prompt_output(translate("Preselect objects before invoking the scale command."));
        end_command();
        messageBox("information", tr("Scale Preselect"), tr("Preselect objects before invoking the scale command."));
    }
    else {
        prompt_output(translate("Specify base point: "));
    }
}

void scale_click(EmbVector v)
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
            g->preview = false;
            end_command();
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
                prompt_output(translate("Value must be positive and nonzero."));
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
                prompt_output(translate("Value must be positive and nonzero."));
                prompt_output(translate("Specify new length: ");
            }
            else {
                scaleSelected(baseX, baseY, factorNew/factorRef);
                g->preview = false;
                end_command();
            }
        }
    }
}

void scale_prompt(char *str)
{
    case MODE_NORMAL: {
        if (firstRun) {
            EmbVector v = str_to_vector(str, &error);
            if (error) {
                prompt_output(translate("Invalid point."));
                prompt_output(translate("Specify base point: ");
            }
            else {
                firstRun = false;
                base = v;
                addRubber("LINE");
                setRubberMode("LINE");
                setRubberPoint("LINE_START", base);
                previewOn("SELECTED", "SCALE", base, 1);
                prompt_output(translate("Specify scale factor or [Reference]: "));
            }
        }
        else {
            if (str == "R" || str == "REFERENCE") {
                mode = MODE_REFERENCE;
                prompt_output(translate("Specify reference length {1}: ");
                clearRubber();
                g->preview = false;
            }
            else {
                if (std::isnan(str)) {
                    prompt_output(translate("Requires valid numeric distance, second point, or option keyword."));
                    prompt_output(translate("Specify scale factor or [Reference]: "));
                }
                else {
                    factor = atof(str);
                    scaleSelected(base, factor);
                    g->preview = false;
                    end_command();
                }
            }
        }
    }
    case MODE_REFERENCE: {
        if (std::isnan(g->baseR.x)) {
            if (std::isnan(str)) {
                EmbVector v = str_to_vector(str, &error);
                if (error) {
                    prompt_output(translate("Requires valid numeric distance or two points."));
                    prompt_output(translate("Specify reference length {1}: ");
                }
                else {
                    g->baseR = v;
                    addRubber("LINE");
                    setRubberMode("LINE");
                    setRubberPoint("LINE_START", g->baseR);
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
                    prompt_output(translate("Value must be positive and nonzero."));
                    prompt_output(translate("Specify reference length") + " {1}: ");
                }
                else {
                    addRubber("LINE");
                    setRubberMode("LINE");
                    setRubberPoint("LINE_START", g->base);
                    previewOn("SELECTED", "SCALE", g->base, g->factorRef);
                    prompt_output(translate("Specify new length: "));
                }
            }
        }
        else if (std::isnan(destRX)) {
            if (std::isnan(str)) {
                EmbVector v = str_to_vector(str, &error);
                if (error) {
                    prompt_output(translate("Requires valid numeric distance or two points."));
                    prompt_output(translate("Specify second point: "));
                }
                else {
                    g->destR = v;
                    g->factorRef = calculateDistance(g->baseR, g->destR);
                    if (factorRef <= 0.0) {
                        destRX = 0.0f;
                        destRY = 0.0f;
                        factorRef = 0.0f;
                        prompt_output(translate("Value must be positive and nonzero."));
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
                g->baseR = embVector_make(0.0, 0.0);
                g->destR = embVector_make(0.0, 0.0);
                // The reference length is what we will use later.
                factorRef = atof(str);
                if (factorRef <= 0.0) {
                    destRX = 0.0f;
                    destRY = 0.0f;
                    factorRef = 0.0f;
                    prompt_output(translate("Value must be positive and nonzero."));
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
                EmbVector v = str_to_vector(str, &error);
                if (error) {
                    prompt_output(translate("Requires valid numeric distance or second point."));
                    prompt_output(translate("Specify new length: "));
                }
                else {
                    factorNew = calculateDistance(base, v);
                    if (factorNew <= 0.0) {
                        factorNew = 0.0f;
                        prompt_output(translate("Value must be positive and nonzero."));
                        prompt_output(translate("Specify new length: "));
                    }
                    else {
                        scaleSelected(base, factorNew/factorRef);
                        g->preview = false;
                        end_command();
                    }
                }
            }
            else {
                factorNew = atof(str);
                if (factorNew <= 0.0) {
                    factorNew = 0.0f;
                    prompt_output(translate("Value must be positive and nonzero."));
                    prompt_output(translate("Specify new length: "));
                }
                else {
                    scaleSelected(g->base, g->factorNew / g->factorRef);
                    g->preview = false;
                    end_command();
                }
            }
        }
    }
}

void text_single_main(void)
{
    init_command();
    clear_selection();
    strcpy(g->text, "");
    g->position = embVector_make(0.0f, 0.0f);
    strcpy(g->justify, "Left");
    g->font = textFont();
    g->height = 0.0f;
    g->rotation = 0.0f;
    g->mode = MODE_TEXT_SINGLE_SETGEOM;
    prompt_output(translate("Current font: " + "{" + g->textFont + "} " + tr("Text height: ") + "{" +  textSize() + "}");
    prompt_output(translate("Specify start point of text or [Justify/Setfont]: "));
}

void text_single_prompt(char *str)

    case MODE_TEXT_SINGLE_JUSTIFY: {
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
            prompt_output(translate("Invalid option keyword."));
            prompt_output(translate("Text Justification Options [Center/Right/Align/Middle/Fit/TL/TC/TR/ML/MC/MR/BL/BC/BR]: ");
        }
    }
    case MODE_SETFONT: {
        mode = MODE_SETGEOM;
        g->textFont = str;
        setRubberText("TEXT_FONT", g->textFont);
        setTextFont(g->textFont);
        prompt_output(translate("Specify start point of text or [Justify/Setfont]: ");
    }
    case MODE_SETGEOM: {
        if (std::isnan(textX)) {
            if (str == "J" || str == "JUSTIFY") {
                mode = MODE_JUSTIFY;
                prompt_output(translate("Text Justification Options [Center/Right/Align/Middle/Fit/TL/TC/TR/ML/MC/MR/BL/BC/BR]: ");
            }
            else if (str == "S" || str == "SETFONT") {
                mode = MODE_SETFONT;
                prompt_output(translate("Specify font name: ");
            }
            else {
                EmbVector v = str_to_vector(str, &error);
                if (error) {
                    prompt_output(translate("Point or option keyword required."));
                    prompt_output(translate("Specify start point of text or [Justify/Setfont]: ");
                }
                else {
                    g->position = v;
                    addRubber("LINE");
                    setRubberMode("LINE");
                    setRubberPoint("LINE_START", textX, textY);
                    prompt_output(translate("Specify text height") + " {" + textSize() + "}: ");
                }
            }
        }
        else if (std::isnan(g->textHeight)) {
            if (str == "") {
                g->height = textSize();
                prompt_output(translate("Specify text angle") + " {" + textAngle() + "}: ");
            }
            else if (std::isnan(str)) {
                prompt_output(translate("Requires valid numeric distance or second point."));
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
                prompt_output(translate("Requires valid numeric angle or second point."));
                prompt_output(translate("Specify text angle") + " {" + textAngle() + "}: ");
            }
            else {
                g->rotation = atof(str);
                setTextAngle(g->rotation);
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
                end_command();
            }
            else {
                vulcanize();
                end_command();
*/
                /* TODO: Rather than ending the command, calculate where the
                 * next line would be and modify the x/y to the new point.
                 */
/*
            }
        }
        else {
            g->text = atof(str);
            setRubberText("TEXT_RAPID", g->text);
        }
    }
}
*/

/* . */
void
updateStar(EmbVector mouse)
{
    /*
    EmbVector v = embVector_subtract(mouse, center);
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
    */
}

