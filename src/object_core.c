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

GeometryData*
geometry_init(int type)
{
    GeometryData *g = malloc(sizeof(GeometryData));
    return g;
}

void
geometry_free(GeometryData *g)
{
    free(g);
}

void
geometry_left_click(GeometryData *geometry, EmbVector v)
{

}

void
geometry_prompt(
    GeometryData *geometry,
    char input[MAX_STRING_LENGTH],
    char output[MAX_STRING_LENGTH])
{

}

void
geometry_context(
    void *m,
    GeometryData *geometry,
    char output[MAX_STRING_LENGTH])
{

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
        setRubberPoint("LINE_START", point1.x, point1.y);
        append_prompt_history();
        set_prompt_prefix(tr("Specify second point: "));
    }
    else {
        append_prompt_history();
        point2 = v;
        reportDistance();
        end();
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
            set_prompt_prefix(tr("Specify first point: "));
        }
        else {
            point1.x = atof(strList[0]);
            point1.y = atof(strList[1]);
            addRubber("LINE");
            setRubberMode("LINE");
            setRubberPoint("LINE_START", point1.x, point1.y);
            set_prompt_prefix(tr("Specify second point: "));
        }
    }
    else {
        if (std::isnan(strList[0]) || std::isnan(strList[1])) {
            alert(tr("Requires numeric distance or two points."));
            set_prompt_prefix(tr("Specify second point: "));
        }
        else {
            point2.x = atof(strList[0]);
            point2.y = atof(strList[1]);
            reportDistance();
            end();
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
    append_prompt_history();
    actuator("set-prompt-prefix " + tr("Delta X") + " = " + dx.toString() + ", " + tr("Delta Y") + " = " + dy.toString());
    append_prompt_history();
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
            append_prompt_history();
            set_prompt_prefix(tr("Specify first axis end point: ");
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
            append_prompt_history();
            set_prompt_prefix(tr("Specify second axis end point or [Rotation]: ");
        }
        else if (std::isnan(point3.x)) {
            point3.x = x;
            point3.y = y;
            height = perpendicularDistance(point3.x, point3.y, point1.x, point1.y, point2.x, point2.y)*2.0;
            setRubberPoint("ELLIPSE_AXIS2_POINT2", point3.x, point3.y);
            vulcanize();
            append_prompt_history();
            end();
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
            append_prompt_history();
            set_prompt_prefix(tr("Specify first axis end point: ");
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
            append_prompt_history();
            set_prompt_prefix(tr("Specify second axis end point or [Rotation]: ");
        }
        else if (std::isnan(point3.x)) {
            point3.x = x;
            point3.y = y;
            height = perpendicularDistance(point3.x, point3.y, center.x, center.y, point2.x, point2.y)*2.0;
            setRubberPoint("ELLIPSE_AXIS2_POINT2", point3.x, point3.y);
            vulcanize();
            append_prompt_history();
            end();
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
            append_prompt_history();
            end();
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
        if (std::isnan(point1.x)) {
            if (str == "C" || str == "CENTER") {
                mode = MODE_MAJORRADIUS_MINORRADIUS;
                set_prompt_prefix(tr("Specify center point: "));
            }
            else {
                EmbReal strList = str.split(",");
                if (std::isnan(strList[0]) || std::isnan(strList[1])) {
                    alert(tr("Point or option keyword required."));
                    set_prompt_prefix(tr("Specify first axis start point or [Center]: ");
                }
                else {
                    point1.x = atof(strList[0]);
                    point1.y = atof(strList[1]);
                    addRubber("ELLIPSE");
                    setRubberMode("ELLIPSE_LINE");
                    setRubberPoint("ELLIPSE_LINE_POINT1", point1.x, point1.y);
                    set_prompt_prefix(tr("Specify first axis end point: ");
                }
            }
        }
        else if (std::isnan(point2.x)) {
            EmbReal strList = str.split(",");
            if (std::isnan(strList[0]) || std::isnan(strList[1])) {
                alert(tr("Invalid point."));
                set_prompt_prefix(tr("Specify first axis end point: ");
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
                set_prompt_prefix(tr("Specify second axis end point or [Rotation]: ");
            }
        }
        else if (std::isnan(point3.x)) {
            if (str == "R" || str == "ROTATION") {
                mode = MODE_ELLIPSE_ROTATION;
                set_prompt_prefix(tr("Specify rotation: ");
            }
            else {
                EmbReal strList = str.split(",");
                if (std::isnan(strList[0]) || std::isnan(strList[1])) {
                    alert(tr("Point or option keyword required."));
                    set_prompt_prefix(tr("Specify second axis end point or [Rotation]: ");
                }
                else {
                    point3.x = atof(strList[0]);
                    point3.y = atof(strList[1]);
                    height = perpendicularDistance(point3.x, point3.y, point1.x, point1.y, point2.x, point2.y)*2.0;
                    setRubberPoint("ELLIPSE_AXIS2_POINT2", point3.x, point3.y);
                    vulcanize();
                    end();
                }
            }
        }
        break;
    }
    case MODE_MAJORRADIUS_MINORRADIUS: {
        if (std::isnan(point1.x)) {
            EmbReal strList = str.split(",");
            if (std::isnan(strList[0]) || std::isnan(strList[1])) {
                alert(tr("Invalid point."));
                set_prompt_prefix(tr("Specify center point: ");
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
                set_prompt_prefix(tr("Specify first axis end point: ");
            }
        }
        else if (std::isnan(point2.x)) {
            EmbReal strList = str.split(",");
            if (std::isnan(strList[0]) || std::isnan(strList[1])) {
                alert(tr("Invalid point."));
                set_prompt_prefix(tr("Specify first axis end point: ");
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
                set_prompt_prefix(tr("Specify second axis end point or [Rotation]: ");
            }
        }
        else if (std::isnan(point3.x)) {
            if (str == "R" || str == "ROTATION") {
                mode = MODE_ELLIPSE_ROTATION;
                set_prompt_prefix(tr("Specify ellipse rotation: ");
            }
            else {
                EmbReal strList = str.split(",");
                if (std::isnan(strList[0]) || std::isnan(strList[1])) {
                    alert(tr("Point or option keyword required."));
                    set_prompt_prefix(tr("Specify second axis end point or [Rotation]: ");
                }
                else {
                    point3.x = atof(strList[0]);
                    point3.y = atof(strList[1]);
                    height = perpendicularDistance(point3.x, point3.y, point1.x, point1.y, point2.x, point2.y)*2.0;
                    setRubberPoint("ELLIPSE_AXIS2_POINT2", point3.x, point3.y);
                    vulcanize();
                    end();
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
                alert(tr("Invalid angle. Input a numeric angle or pick a point."));
                set_prompt_prefix(tr("Specify rotation: ");
            }
            else {
                EmbReal angle = atof(str);
                height = cos(angle*PI/180.0)*width;
                addEllipse(center.x, center.y, width, height, rot, false);
                end();
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
        alert(tr("Preselect objects before invoking the delete command."));
        end();
        messageBox("information", tr("Delete Preselect"), tr("Preselect objects before invoking the delete command."));
    }
    else {
        deleteSelected();
        end();
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
    end();
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
    init();
    actuator("clear-selection");
    firstRun = true;
    first = {0.0f, 0.0f};
    prev = {0.0f, 0.0f};
    set_prompt_prefix(tr("Specify first point: ");
}

/* . */
void
Geometry::line_click(EmbReal x, EmbReal y)
{
    if (firstRun) {
        firstRun = false;
        first.x = x;
        first.y = y;
        prev.x = x;
        prev.y = y;
        addRubber("LINE");
        setRubberMode("LINE");
        setRubberPoint("LINE_START", firstX, first.y);
        append_prompt_history();
        set_prompt_prefix(tr("Specify next point or [Undo]: ");
    }
    else {
        setRubberPoint("LINE_END", x, y);
        vulcanize();
        addRubber("LINE");
        setRubberMode("LINE");
        setRubberPoint("LINE_START", x, y);
        append_prompt_history();
        prev.x = x;
        prev.y = y;
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
            set_prompt_prefix(tr("Specify first point: "));
        }
        else {
            firstRun = false;
            first.x = atof(strList[0]);
            first.y = atof(strList[1]);
            prev.x = first.x;
            prev.y = first.y;
            addRubber("LINE");
            setRubberMode("LINE");
            setRubberPoint("LINE_START", firstX, first.y);
            set_prompt_prefix(tr("Specify next point or [Undo]: "));
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
                set_prompt_prefix(tr("Specify next point or [Undo]: ");
            }
            else {
                EmbReal x = atof(strList[0]);
                EmbReal y = atof(strList[1]);
                setRubberPoint("LINE_END", x, y);
                vulcanize();
                addRubber("LINE");
                setRubberMode("LINE");
                setRubberPoint("LINE_START", x, y);
                prev.x = x;
                properties["prevY"] = y;
                set_prompt_prefix(tr("Specify next point or [Undo]: ");
            }
        }
    }
}

/* locate_point_main */
void
Geometry::locate_point_main(void)
{
    script(
        init\n" \
        clear-selection\n" \
        set-prompt-prefix-tr Specify point: \n"
    );
}

/* locate_point_click v */
void
Geometry::locate_point_click(EmbVector v)
{
    append_prompt_history();
    actuator("set-prompt-prefix v.x = " + v.x + ", Y = " + v.y);
    append_prompt_history();
    end();
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
        set_prompt_prefix(tr("Specify point: ");
    }
    else {
        actuator("append-prompt");
        actuator("set-prompt-prefix .x = " + strList[0] + ", Y = " + strList[1]);
        actuator("append-prompt");
        end();
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
        alert(tr("Preselect objects before invoking the move command."));
        end();
        messageBox("information", tr("Move Preselect"), tr("Preselect objects before invoking the move command."));
    }
    else {
        set_prompt_prefix(tr("Specify base point: "));
    }
}

/* . */
void
Geometry::move_click(EmbReal x, EmbReal y)
{
    if (firstRun) {
        firstRun = false;
        base.x = x;
        base.y = y;
        addRubber("LINE");
        setRubberMode("LINE");
        setRubberPoint("LINE_START", baseX, baseY);
        previewOn("SELECTED", "MOVE", baseX, baseY, 0);
        append_prompt_history();
        set_prompt_prefix(tr("Specify destination point: ");
    }
    else {
        dest.x = x;
        dest.y = y;
        delta.x = dest.x - baseX;
        delta.y = dest.y - baseY;
        moveSelected(delta.x, delta.y);
        previewOff();
        end();
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
            set_prompt_prefix(tr("Specify base point: ");
        }
        else {
            firstRun = node_int(false);
            base.x = atof(strList[0]);
            base.y = atof(strList[1]);
            addRubber("LINE");
            setRubberMode("LINE");
            setRubberPoint("LINE_START", baseX, baseY);
            previewOn("SELECTED", "MOVE", baseX, baseY, 0);
            set_prompt_prefix(tr("Specify destination point: ");
        }
    }
    else {
        EmbReal strList = str.split(",");
        if (std::isnan(strList[0]) || std::isnan(strList[1])) {
            alert(tr("Invalid point."));
            set_prompt_prefix(tr("Specify destination point: ");
        }
        else {
            dest.x = atof(strList[0]);
            dest.y = atof(strList[1]);
            delta.x = dest.x - base.x;
            delta.y = dest.y - base.y;
            moveSelected(deltaX, deltaY);
            previewOff();
            end();
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
    set_prompt_prefix(tr("Specify start point: ");
}

/* . */
void
Geometry::path_click(EmbReal x, EmbReal y)
{
    if (firstRun.b) {
        firstRun = false;
        first.x = x;
        first.y = y;
        prev.x = x;
        prev.y = y;
        addPath(x, y);
        append_prompt_history();
        set_prompt_prefix(tr("Specify next point or [Arc/Undo]: ");
    }
    else {
        append_prompt_history();
        appendLineToPath(x, y);
        prev.x = x;
        prev.y = y;
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
            set_prompt_prefix(tr("Specify next point or [Arc/Undo]: ");
        }
        else {
            EmbReal x = atof(strList[0]);
            EmbReal y = atof(strList[1]);
            if (firstRun) {
                firstRun = false;
                first = embVector(x, y);
                prev = embVector(x, y);
                addPath(x, y);
                set_prompt_prefix(tr("Specify next point or [Arc/Undo]: ");
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
    append_prompt_history();
    set_prompt_prefix(tr("Specify first point: ");
}

/* . */
void
Geometry::point_click(EmbVector v)
{
    if (firstRun) {
        firstRun = false;
        append_prompt_history();
        set_prompt_prefix(tr("Specify next point: "));
        addPoint(x, y);
    }
    else {
        append_prompt_history();
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
            set_prompt_prefix(tr("Specify first point: "));
        }
        else {
            firstRun = false;
            EmbReal x = atof(strList[0]);
            EmbReal y = atof(strList[1]);
            set_prompt_prefix(tr("Specify next point: "));
            addPoint(x,y);
        }
    }
    else {
        EmbReal strList = str.split(",");
        if (std::isnan(strList[0]) || std::isnan(strList[1])) {
            alert(tr("Invalid point."));
            set_prompt_prefix(tr("Specify next point: "));
        }
        else {
            EmbReal x = atof(strList[0]);
            EmbReal y = atof(strList[1]);
            set_prompt_prefix(tr("Specify next point: "));
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
    center = embVector(0.0f, 0.0f);
    side1 = embVector(0.0f, 0.0f);
    side2 = embVector(0.0f, 0.0f);
    pointI = embVector(0.0f, 0.0f);
    pointC = embVector(0.0f, 0.0f);
    polyType = "Inscribed"; //Default
    numSides = 4;           //Default
    mode = MODE_NUM_SIDES;
    set_prompt_prefix(tr("Enter number of sides" + " {" + numSides.toString() + "}: ");
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
        append_prompt_history();
        set_prompt_prefix(tr("Specify polygon type [Inscribed in circle/Circumscribed around circle]") + " {" + polyType + "}: ");
        break;
    }
    case MODE_POLYGON_INSCRIBE: {
        pointI = v;
        setRubberPoint("POLYGON_INSCRIBE_POINT", pointI.x, pointI.y);
        vulcanize();
        append_prompt_history();
        end();
        break;
    }
    case MODE_POLYGON_CIRCUMSCRIBE: {
        pointC = v;
        setRubberPoint("POLYGON_CIRCUMSCRIBE_POINT", properties["pointCX"], pointC.y);
        vulcanize();
        append_prompt_history();
        end();
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
        if (str == "" && numSides >= 3 && numSides <= 1024) {
            set_prompt_prefix(tr("Specify center point or [Sidelength]: ");
            mode = MODE_CENTER_PT;
        }
        else {
            EmbReal tmp = atof(str);
            if (std::isnan(tmp) || !isInt(tmp) || tmp < 3 || tmp > 1024) {
                alert(tr("Requires an integer between 3 and 1024."));
                set_prompt_prefix(tr("Enter number of sides" + " {" + numSides.toString() + "}: ");
            }
            else {
                numSides = tmp;
                set_prompt_prefix(tr("Specify center point or [Sidelength]: "));
                mode = MODE_CENTER_PT;
            }
        }
        break;
    }
    case MODE_POLYGON_CENTER_PT: {
        if (str == "S" || str == "SIDELENGTH") {
            mode = MODE_SIDE_LEN;
            set_prompt_prefix(tr("Specify start point: "));
        }
        else {
            EmbReal strList = str.split(",");
            if (std::isnan(strList[0]) || std::isnan(strList[1])) {
                alert(tr("Point or option keyword required."));
                set_prompt_prefix(tr("Specify center point or [Sidelength]: "));
            }
            else {
                center.x = atof(strList[0]);
                center.y = atof(strList[1]);
                mode = MODE_POLYTYPE;
                set_prompt_prefix(tr("Specify polygon type [Inscribed in circle/Circumscribed around circle]") + " {" + properties["polyType + "}: ");
            }
        }
    }
    case MODE_POLYTYPE) {
        if (str == "INSCRIBED") {
            mode = MODE_INSCRIBE;
            polyType = "Inscribed";
            set_prompt_prefix(tr("Specify polygon corner point or [Distance]: ");
            addRubber("POLYGON");
            setRubberMode("POLYGON_INSCRIBE");
            setRubberPoint("POLYGON_CENTER", center.x, center.y);
            setRubberPoint("POLYGON_NUM_SIDES", numSides, 0);
        }
        else if (str == "CIRCUMSCRIBED") {
            mode = MODE_CIRCUMSCRIBE;
            polyType = "Circumscribed";
            set_prompt_prefix(tr("Specify polygon side point or [Distance]: ");
            addRubber("POLYGON");
            setRubberMode("POLYGON_CIRCUMSCRIBE");
            setRubberPoint("POLYGON_CENTER", center.x, center.y);
            setRubberPoint("POLYGON_NUM_SIDES", numSides, 0);
        }
        else if (str == "") {
            if (polyType == "Inscribed") {
                mode = MODE_INSCRIBE;
                set_prompt_prefix(tr("Specify polygon corner point or [Distance]: ");
                addRubber("POLYGON");
                setRubberMode("POLYGON_INSCRIBE");
                setRubberPoint("POLYGON_CENTER", center.x, center.y);
                setRubberPoint("POLYGON_NUM_SIDES", numSides, 0);
            }
            else if (polyType == "Circumscribed") {
                mode = MODE_CIRCUMSCRIBE;
                set_prompt_prefix(tr("Specify polygon side point or [Distance]: ");
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
            alert(tr("Invalid option keyword."));
            set_prompt_prefix(tr("Specify polygon type [Inscribed in circle/Circumscribed around circle]") + " {" + polyType + "}: ");
        }
    }
    case MODE_INSCRIBE) {
        if (str == "D" || str == "DISTANCE") {
            mode = MODE_DISTANCE;
            set_prompt_prefix(tr("Specify distance: "));
        }
        else {
            EmbReal strList = str.split(",");
            if (std::isnan(strList[0]) || std::isnan(strList[1])) {
                alert(tr("Point or option keyword required."));
                set_prompt_prefix(tr("Specify polygon corner point or [Distance]: "));
            }
            else {
                pointI.x = atof(strList[0]);
                pointI.y = atof(strList[1]);
                setRubberPoint("POLYGON_INSCRIBE_POINT", pointI);
                vulcanize();
                end();
            }
        }
    }
    case MODE_CIRCUMSCRIBE) {
        if (str == "D" || str == "DISTANCE") {
            mode = MODE_DISTANCE;
            set_prompt_prefix(tr("Specify distance: "));
        }
        else {
            EmbReal strList = str.split(",");
            if (std::isnan(strList[0]) || std::isnan(strList[1])) {
                alert(tr("Point or option keyword required."));
                set_prompt_prefix(tr("Specify polygon side point or [Distance]: "));
            }
            else {
                pointC.x = atof(strList[0]);
                pointC.y = atof(strList[1]);
                setRubberPoint("POLYGON_CIRCUMSCRIBE_POINT", pointC);
                vulcanize();
                end();
            }
        }
    }
    case MODE_DISTANCE) {
        if (std::isnan(str)) {
            alert(tr("Requires valid numeric distance."));
            set_prompt_prefix(tr("Specify distance: "));
        }
        else {
            if (polyType == "Inscribed") {
                pointI.x = center.x;
                pointI.y = center.y + atof(str);
                setRubberPoint("POLYGON_INSCRIBE_POINT", pointI);
                vulcanize();
                end();
            }
            else if (polyType == "Circumscribed") {
                pointC.x = center.x;
                pointC.y = center.y + atof(str);
                setRubberPoint("POLYGON_CIRCUMSCRIBE_POINT", pointC);
                vulcanize();
                end();
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
        append_prompt_history();
        set_prompt_prefix(tr("Specify next point or [Undo]: ");
    }
    else {
        num++;
        setRubberPoint("POLYLINE_POINT_" + num.toString(), v);
        setRubberText("POLYLINE_NUM_POINTS", num.toString());
        spareRubber("POLYLINE");
        append_prompt_history();
        prev = v;
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
            set_prompt_prefix(tr("Specify first point: "));
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
            set_prompt_prefix(tr("Specify next point or [Undo]: ");
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
                set_prompt_prefix(tr("Specify next point or [Undo]: ");
            }
            else {
                EmbReal x = atof(strList[0]);
                EmbReal y = atof(strList[1]);
                properties["num"] = properties["num"].i + 1;
                setRubberPoint("POLYLINE_POINT_" + properties["num"].toString(), x, y);
                setRubberText("POLYLINE_NUM_POINTS", properties["num"].toString());
                spareRubber("POLYLINE");
                prev.x = x;
                prev.y = y;
                set_prompt_prefix(tr("Specify next point or [Undo]: ");
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
        point1.x = v.x;
        point1.y = v.y;
        addRubber("DIMLEADER");
        setRubberMode("DIMLEADER_LINE");
        setRubberPoint("DIMLEADER_LINE_START", point1.x, point1.y);
        append_prompt_history();
        set_prompt_prefix(tr("Specify second point: "));
    }
    else {
        point2.x = v.x;
        point2.y = v.y;
        setRubberPoint("DIMLEADER_LINE_END", point2.x, point2.y);
        vulcanize();
        append_prompt_history();
        end();
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
    if (std::isnan(point1.x)) {
        if (std::isnan(strList[0]) || std::isnan(strList[1])) {
            alert(tr("Requires two points."));
            set_prompt_prefix(tr("Specify first point: "));
        }
        else {
            point1.x = atoi(strList[0]);
            point1.y = atoi(strList[1]);
            addRubber("DIMLEADER");
            setRubberMode("DIMLEADER_LINE");
            setRubberPoint("DIMLEADER_LINE_START", point1.x, point1.y);
            set_prompt_prefix(tr("Specify second point: ");
        }
    }
    else {
        if (std::isnan(strList[0]) || std::isnan(strList[1])) {
            alert(tr("Requires two points."));
            set_prompt_prefix(tr("Specify second point: ");
        }
        else {
            point2.x = atoi(strList[0]);
            point2.y = atoi(strList[1]);
            setRubberPoint("DIMLEADER_LINE_END", point2.x, point2.y);
            vulcanize();
            end();
        }
    }
}

/* . */
const char rectangle_click_script[][MAX_STRING_LENGTH] = {
    if (properties["newRect"].b) {
        properties["newRect"].b = false;
        point1.x = v.x;
        point1.y = v.y;
        addRubber("RECTANGLE");
        setRubberMode("RECTANGLE");
        setRubberPoint("RECTANGLE_START", x, y);
        set_prompt_prefix(tr("Specify other corner point or [Dimensions]: "));
    }
    else {
        newRect = true;
        x2 = v.x;
        y2 = v.y;
        setRubberPoint RECTANGLE_END x y;
        vulcanize;
        end"
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
            set_prompt_prefix(tr("Specify first point: "));
        }
        else {
            EmbReal x = atoi(strList[0]);
            EmbReal y = atoi(strList[1]);
            if (properties["newRect"].b) {
                properties["newRect"] = false;
                point1.x = x;
                point1.y = y;
                addRubber("RECTANGLE");
                setRubberMode("RECTANGLE");
                setRubberPoint("RECTANGLE_START", x, y);
                set_prompt_prefix(tr("Specify other corner point or [Dimensions]: "));
            }
            else {
                properties["newRect"] = true;
                point2.x = x;
                point2.y = y;
                setRubberPoint("RECTANGLE_END", x, y);
                vulcanize();
                end();
            }
        }
    }
}

/* . */
void
Geometry::rgb_main(void)
{
    init();
    actuator("clear-selection");
    actuator("mode=RGB_MODE_BACKGROUND");
    set_prompt_prefix(tr("Enter RED,GREEN,BLUE values for background or [Crosshair/Grid]: "));
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
            mode = atoi("RGB_MODE_CROSSHAIR");
            set_prompt_prefix(tr("Specify crosshair color: "));
        }
        else if (str == "G" || str == "GRID") {
            mode = RGB_MODE_GRID;
            set_prompt_prefix(tr("Specify grid color: "));
        }
        else {
            EmbReal strList = str.split(",");
            EmbReal r = atoi(strList[0]);
            EmbReal g = atoi(strList[1]);
            EmbReal b = atoi(strList[2]);
            if (!validRGB(r,g,b)) {
                alert(tr("Invalid color. R,G,B values must be in the range of 0-255."));
                set_prompt_prefix(tr("Specify background color: "));
            }
            else {
                setBackgroundColor(r,g,b);
                end();
            }
        }
    }
    case RGB_MODE_CROSSHAIR) {
        EmbReal strList = str.split(",");
        EmbReal r = atoi(strList[0]);
        EmbReal g = atoi(strList[1]);
        EmbReal b = atoi(strList[2]);
        if (!validRGB(r,g,b)) {
            alert(tr("Invalid color. R,G,B values must be in the range of 0-255."));
            set_prompt_prefix(tr("Specify crosshair color: "));
        }
        else {
            setCrossHairColor(r,g,b);
            end();
        }
    }
    case RGB_MODE_GRID) {
        EmbReal strList = str.split(",");
        EmbReal r = atof(strList[0]);
        EmbReal g = atof(strList[1]);
        EmbReal b = atof(strList[2]);
        if (!validRGB(r,g,b)) {
            alert(tr("Invalid color. R,G,B values must be in the range of 0-255."));
            set_prompt_prefix(tr("Specify grid color: "));
        }
        else {
            setGridColor(r,g,b);
            end();
        }
    }
}

/* . */
void
Geometry::rotate_main(String args)
{
    init();
    mode = ROTATE_MODE_NORMAL;
    firstRun = true;
    base = {0.0f, 0.0f};
    dest = {0.0f, 0.0f};
    angle = 0.0f;

    properties["baseR"] = {0.0f, 0.0f};
    properties["destR"] = {0.0f, 0.0f};
    angleRef = 0.0f;
    angleNew = 0.0f;

    if (numSelected() <= 0) {
        //TODO: Prompt to select objects if nothing is preselected
        alert(tr("Preselect objects before invoking the rotate command."));
        end();
        messageBox("information", tr("Rotate Preselect"), tr("Preselect objects before invoking the rotate command."));
    }
    else {
        set_prompt_prefix(tr("Specify base point: "));
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
            setRubberPoint("LINE_START", properties["baseX, properties["baseY);
            previewOn("SELECTED", "ROTATE", properties["baseX, properties["baseY, 0);
            append_prompt_history();
            set_prompt_prefix(tr("Specify rotation angle or [Reference]: "));
        }
        else {
            dest = v;
            angle = calculateAngle(baseX, baseY, dest.x, dest.y);
            append_prompt_history();
            rotateSelected(baseX, baseY, angle);
            previewOff();
            end();
        }
    }
    case ROTATE_MODE_REFERENCE) {
        if (std::isnan(properties["baseRX"])) {
            properties["baseR"] = v;
            append_prompt_history();
            addRubber("LINE");
            setRubberMode("LINE");
            setRubberPoint("LINE_START", properties["baseRX"], properties["baseRY"]);
            set_prompt_prefix(tr("Specify second point: ");
        }
        else if (std::isnan(destRX)) {
            properties["destR"] = v;
            angleRef = calculateAngle(properties["baseRX"], properties["baseRY"], destRX, destRY);
            setRubberPoint("LINE_START", baseX, baseY);
            previewOn("SELECTED", "ROTATE", baseX, baseY, angleRef);
            append_prompt_history();
            set_prompt_prefix(tr("Specify the new angle: ");
        }
        else if (std::isnan(angleNew)) {
            angleNew = calculateAngle(baseX, baseY, x, y);
            rotateSelected(baseX, baseY, angleNew - angleRef);
            previewOff();
            end();
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
                set_prompt_prefix(tr("Specify base point: ");
            }
            else {
                firstRun = false;
                base.x = atof(strList[0]);
                base.y = atof(strList[1]);
                addRubber("LINE");
                setRubberMode("LINE");
                setRubberPoint("LINE_START", baseX, baseY);
                previewOn("SELECTED", "ROTATE", baseX, baseY, 0);
                set_prompt_prefix(tr("Specify rotation angle or [Reference]: ");
            }
        }
        else {
            if (str == "R" || str == "REFERENCE") {
                mode = MODE_REFERENCE;
                set_prompt_prefix(tr("Specify the reference angle") + " {0.00}: ");
                clearRubber();
                previewOff();
            }
            else {
                if (std::isnan(str)) {
                    alert(tr("Requires valid numeric angle, second point, or option keyword."));
                    set_prompt_prefix(tr("Specify rotation angle or [Reference]: ");
                }
                else {
                    angle = atof(str);
                    rotateSelected(baseX, baseY, angle);
                    previewOff();
                    end();
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
                    set_prompt_prefix(tr("Specify the reference angle {0.00}: ");
                }
                else {
                    properties["baseR.x"] = atof(strList[0]);
                    properties["baseR.y"] = atof(strList[1]);
                    addRubber("LINE");
                    setRubberMode("LINE");
                    setRubberPoint("LINE_START", properties["baseRX"], properties["baseRY"]);
                    set_prompt_prefix(tr("Specify second point: ");
                }
            }
            else {
                //The base and dest values are only set here to advance the command.
                properties["baseR.x"] = 0.0;
                properties["baseR.y"] = 0.0;
                properties["destR.x"] = 0.0;
                properties["destR.y"] = 0.0;
                //The reference angle is what we will use later.
                angleRef = atof(str);
                addRubber("LINE");
                setRubberMode("LINE");
                setRubberPoint("LINE_START", baseX, baseY);
                previewOn("SELECTED", "ROTATE", baseX, baseY, angleRef);
                set_prompt_prefix(tr("Specify the new angle: ");
            }
        }
        else if (std::isnan(destRX)) {
            if (std::isnan(str)) {
                EmbReal strList = str.split(",");
                if (std::isnan(strList[0]) || std::isnan(strList[1])) {
                    alert(tr("Requires valid numeric angle or two points."));
                    set_prompt_prefix(tr("Specify second point: "));
                }
                else {
                    properties["destR.x"] = atof(strList[0]);
                    properties["destR.y"] = atof(strList[1]);
                    angleRef = calculateAngle(properties["baseRX"], properties["baseRY"], destRX, destRY);
                    previewOn("SELECTED", "ROTATE", baseX, baseY, angleRef);
                    setRubberPoint("LINE_START", baseX, baseY);
                    set_prompt_prefix(tr("Specify the new angle: ");
                }
            }
            else {
                // The base and dest values are only set here to advance the command.
                baseR = {0.0, 0.0};
                destR = {0.0, 0.0};
                // The reference angle is what we will use later.
                angleRef = atof(str);
                previewOn("SELECTED", "ROTATE", baseX, baseY, angleRef);
                set_prompt_prefix(tr("Specify the new angle: ");
            }
        }
        else if (std::isnan(angleNew)) {
            if (std::isnan(str)) {
                EmbReal strList = str.split(",");
                if (std::isnan(strList[0]) || std::isnan(strList[1])) {
                    alert(tr("Requires valid numeric angle or second point."));
                    set_prompt_prefix(tr("Specify the new angle: ");
                }
                else {
                    EmbVector v;
                    v.x = atof(strList[0]);
                    v.y = atof(strList[1]);
                    angleNew = calculateAngle(base, v);
                    rotateSelected(base, angleNew - angleRef);
                    previewOff();
                    end();
                }
            }
            else {
                angleNew = atof(str);
                rotateSelected(base, angleNew - angleRef);
                previewOff();
                end();
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
    init();

    //Report number of pre-selected objects
    setPromptPrefix("Number of Objects Selected: " + numSelected().toString());
    append_prompt_history();

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

    end();
}

/* Command: Scale. */
void
Geometry::scale_main(void)
{
    init();

    mode = MODE_NORMAL;
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
        alert(tr("Preselect objects before invoking the scale command."));
        end();
        messageBox("information", tr("Scale Preselect"), tr("Preselect objects before invoking the scale command."));
    }
    else {
        set_prompt_prefix(tr("Specify base point: "));
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
            base = v;
            addRubber("LINE");
            setRubberMode("LINE");
            setRubberPoint("LINE_START", base);
            previewOn("SELECTED", "SCALE", base, 1);
            append_prompt_history();
            set_prompt_prefix(tr("Specify scale factor or [Reference]: "));
        }
        else {
            dest = v;
            factor = calculateDistance(base, dest);
            append_prompt_history();
            scaleSelected(base, factor);
            previewOff();
            end();
        }
        break;
    }
    case "MODE_REFERENCE") {
        if (std::isnan(properties["baseRX"])) {
            properties["baseR"] = v;
            append_prompt_history();
            addRubber("LINE");
            setRubberMode("LINE");
            setRubberPoint("LINE_START", properties["baseR"]);
            set_prompt_prefix(tr("Specify second point: "));
        }
        else if (std::isnan(destRX)) {
            properties["destR"] = v;
            factorRef = calculateDistance(properties["baseRX"].r, properties["baseRY"].r, destRX, destRY);
            if (factorRef <= 0.0) {
                destRX = 0.0f;
                destRY = 0.0f;
                factorRef = 0.0f;
                alert(tr("Value must be positive and nonzero."));
                set_prompt_prefix(tr("Specify second point: "));
            }
            else {
                append_prompt_history();
                setRubberPoint("LINE_START", baseX, baseY);
                previewOn("SELECTED", "SCALE", baseX, baseY, factorRef);
                set_prompt_prefix(tr("Specify new length: ");
            }
        }
        else if (std::isnan(factorNew)) {
            factorNew = calculateDistance(baseX, baseY, x, y);
            if (factorNew <= 0.0) {
                factorNew = 0.0f;
                alert(tr("Value must be positive and nonzero."));
                set_prompt_prefix(tr("Specify new length: ");
            }
            else {
                append_prompt_history();
                scaleSelected(baseX, baseY, factorNew/factorRef);
                previewOff();
                end();
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
                set_prompt_prefix(tr("Specify base point: ");
            }
            else {
                firstRun = false;
                base.x = atof(strList[0]);
                base.y = atof(strList[1]);
                addRubber("LINE");
                setRubberMode("LINE");
                setRubberPoint("LINE_START", baseX, baseY);
                previewOn("SELECTED", "SCALE", baseX, baseY, 1);
                set_prompt_prefix(tr("Specify scale factor or [Reference]: ");
            }
        }
        else {
            if (str == "R" || str == "REFERENCE") {
                mode = MODE_REFERENCE;
                set_prompt_prefix(tr("Specify reference length {1}: ");
                clearRubber();
                previewOff();
            }
            else {
                if (std::isnan(str)) {
                    alert(tr("Requires valid numeric distance, second point, or option keyword."));
                    set_prompt_prefix(tr("Specify scale factor or [Reference]: ");
                }
                else {
                    factor = atof(str);
                    scaleSelected(base.x, base.y, factor);
                    previewOff();
                    end();
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
                    set_prompt_prefix(tr("Specify reference length {1}: ");
                }
                else {
                    properties["baseR.x"] = atof(strList[0]);
                    properties["baseR.y"] = atof(strList[1]);
                    addRubber("LINE");
                    setRubberMode("LINE");
                    setRubberPoint("LINE_START", properties["baseRX, properties["baseRY);
                    set_prompt_prefix(tr("Specify second point: "));
                }
            }
            else {
                //The base and dest values are only set here to advance the command.
                properties["baseR.x"] = 0.0;
                properties["baseR.y"] = 0.0;
                properties["destR.x"] = 0.0;
                properties["destR.y"] = 0.0;
                //The reference length is what we will use later.
                factorRef = atof(str);
                if (factorRef <= 0.0) {
                    properties["baseRX"] = 0.0f;
                    properties["baseRY"] = 0.0f;
                    destRX = 0.0f;
                    destRY = 0.0f;
                    factorRef = 0.0f;
                    alert(tr("Value must be positive and nonzero."));
                    set_prompt_prefix(tr("Specify reference length") + " {1}: ");
                }
                else {
                    addRubber("LINE");
                    setRubberMode("LINE");
                    setRubberPoint("LINE_START", properties["baseX, properties["baseY);
                    previewOn("SELECTED", "SCALE", properties["baseX, properties["baseY, properties["factorRef);
                    set_prompt_prefix(tr("Specify new length: "));
                }
            }
        }
        else if (std::isnan(destRX)) {
            if (std::isnan(str)) {
                EmbReal strList = str.split(",");
                if (std::isnan(strList[0]) || std::isnan(strList[1])) {
                    alert(tr("Requires valid numeric distance or two points."));
                    set_prompt_prefix(tr("Specify second point: "));
                }
                else {
                    properties["destR.x"] = atof(strList[0]);
                    properties["destR.y"] = atof(strList[1]);
                    properties["factorRef = calculateDistance(properties["baseRX, properties["baseRY"], destRX, destRY);
                    if (factorRef <= 0.0) {
                        destRX = 0.0f;
                        destRY = 0.0f;
                        factorRef = 0.0f;
                        alert(tr("Value must be positive and nonzero."));
                        set_prompt_prefix(tr("Specify second point: "));
                    }
                    else {
                        setRubberPoint("LINE_START", baseX, baseY);
                        previewOn("SELECTED", "SCALE", baseX, baseY, factorRef);
                        set_prompt_prefix(tr("Specify new length: ");
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
                factorRef = atof(str);
                if (factorRef <= 0.0) {
                    destRX = 0.0f;
                    destRY = 0.0f;
                    factorRef = 0.0f;
                    alert(tr("Value must be positive and nonzero."));
                    set_prompt_prefix(tr("Specify second point: "));
                }
                else {
                    setRubberPoint("LINE_START", baseX, baseY);
                    previewOn("SELECTED", "SCALE", baseX, baseY, factorRef);
                    set_prompt_prefix(tr("Specify new length: "));
                }
            }
        }
        else if (std::isnan(factorNew)) {
            if (std::isnan(str)) {
                EmbReal strList = str.split(",");
                if (std::isnan(strList[0]) || std::isnan(strList[1])) {
                    alert(tr("Requires valid numeric distance or second point."));
                    set_prompt_prefix(tr("Specify new length: "));
                }
                else {
                    EmbReal x = atof(strList[0]);
                    EmbReal y = atof(strList[1]);
                    factorNew = calculateDistance(baseX, baseY, x, y);
                    if (factorNew <= 0.0) {
                        factorNew = 0.0f;
                        alert(tr("Value must be positive and nonzero."));
                        set_prompt_prefix(tr("Specify new length: "));
                    }
                    else {
                        scaleSelected(baseX, baseY, factorNew/factorRef);
                        previewOff();
                        end();
                    }
                }
            }
            else {
                factorNew = atof(str);
                if (factorNew <= 0.0) {
                    factorNew = 0.0f;
                    alert(tr("Value must be positive and nonzero."));
                    set_prompt_prefix(tr("Specify new length: "));
                }
                else {
                    scaleSelected(properties["baseX, properties["baseY, properties["factorNew/properties["factorRef);
                    previewOff();
                    end();
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
    properties["text"] = atof("");
    properties["text.x"] = 0.0f;
    properties["text.y"] = 0.0f;
    properties["text.justify"] = atof("Left");
    properties["textFont"] = textFont();
    properties["textHeight"] = 0.0f;
    properties["textRotation"] = 0.0f;
    mode = MODE_TEXT_SINGLE_SETGEOM;
    set_prompt_prefix(tr("Current font: " + "{" + properties["textFont"].s + "} " + tr("Text height: ") + "{" +  textSize() + "}");
    append_prompt_history();
    set_prompt_prefix(tr("Specify start point of text or [Justify/Setfont]: ");
}

/* . */
void
Geometry::text_single_click(EmbVector v)
{
    switch (mode) {

    case MODE_TEXT_SINGLE_SETGEOM: {
        if (std::isnan(textX)) {
            properties["text.x"] = x;
            properties["text.y"] = y;
            addRubber("LINE");
            setRubberMode("LINE");
            setRubberPoint("LINE_START", textX, textY);
            append_prompt_history();
            set_prompt_prefix(tr("Specify text height" + " {" + textSize() + "}: ");
        }
        else if (std::isnan(properties["textHeight"].r)) {
            properties["textHeight"] = calculateDistance(textX, textY, x, y);
            setTextSize(properties["textHeight"].r);
            append_prompt_history();
            set_prompt_prefix(tr("Specify text angle") + " {" + textAngle() + "}: ");
        }
        else if (std::isnan(properties["textRotation"].r)) {
            properties["textRotation"] = calculateAngle(textX, textY, x, y);
            setTextAngle(properties["textRotation"].r);
            append_prompt_history();
            set_prompt_prefix(tr("Enter text: "));
            mode = MODE_RAPID;
            prompt->enableRapidFire();
            clearRubber();
            addRubber("TEXTSINGLE");
            setRubberMode("TEXTSINGLE");
            setRubberPoint("TEXT_POINT", textX, textY);
            setRubberPoint("TEXT_HEIGHT_ROTATION", properties["textHeight"].r, properties["textRotation"].r);
            setRubberText("TEXT_FONT", properties["textFont"].r);
            setRubberText("TEXT_JUSTIFY", textJustify);
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
            textJustify = node("Center");
            setRubberText("TEXT_JUSTIFY", textJustify);
            set_prompt_prefix(tr("Specify center point of text or [Justify/Setfont]: ");
        }
        else if (str == "R" || str == "RIGHT") {
            mode = MODE_SETGEOM;
            textJustify = node("Right");
            setRubberText("TEXT_JUSTIFY", textJustify);
            set_prompt_prefix(tr("Specify right-end point of text or [Justify/Setfont]: "));
        }
        else if (str == "A" || str == "ALIGN") {
            mode = MODE_SETGEOM;
            textJustify = "Aligned";
            setRubberText("TEXT_JUSTIFY", textJustify);
            set_prompt_prefix(tr("Specify start point of text or [Justify/Setfont]: "));
        }
        else if (str == "M" || str == "MIDDLE") {
            mode = MODE_SETGEOM;
            textJustify = "Middle";
            setRubberText("TEXT_JUSTIFY", textJustify);
            set_prompt_prefix(tr("Specify middle point of text or [Justify/Setfont]: ");
        }
        else if (str == "F" || str == "FIT") {
            mode = MODE_SETGEOM;
            textJustify = "Fit";
            setRubberText("TEXT_JUSTIFY", textJustify);
            set_prompt_prefix(tr("Specify start point of text or [Justify/Setfont]: "));
        }
        else if (str == "TL" || str == "TOPLEFT") {
            mode = MODE_SETGEOM;
            textJustify = "Top Left";
            setRubberText("TEXT_JUSTIFY", textJustify);
            set_prompt_prefix(tr("Specify top-left point of text or [Justify/Setfont]: "));
        }
        else if (str == "TC" || str == "TOPCENTER") {
            mode = MODE_SETGEOM;
            textJustify = "Top Center";
            setRubberText("TEXT_JUSTIFY", textJustify);
            set_prompt_prefix(tr("Specify top-center point of text or [Justify/Setfont]: "));
        }
        else if (str == "TR" || str == "TOPRIGHT") {
            mode = MODE_SETGEOM;
            textJustify = "Top Right";
            setRubberText("TEXT_JUSTIFY", textJustify);
            set_prompt_prefix(tr("Specify top-right point of text or [Justify/Setfont]: "));
        }
        else if (str == "ML" || str == "MIDDLELEFT") {
            mode = MODE_SETGEOM;
            textJustify = "Middle Left";
            setRubberText("TEXT_JUSTIFY", textJustify);
            set_prompt_prefix(tr("Specify middle-left point of text or [Justify/Setfont]: ");
        }
        else if (str == "MC" || str == "MIDDLECENTER") {
            mode = MODE_SETGEOM;
            textJustify = "Middle Center";
            setRubberText("TEXT_JUSTIFY", textJustify);
            set_prompt_prefix(tr("Specify middle-center point of text or [Justify/Setfont]: ");
        }
        else if (str == "MR" || str == "MIDDLERIGHT") {
            mode = MODE_SETGEOM;
            textJustify = "Middle Right";
            setRubberText("TEXT_JUSTIFY", textJustify);
            set_prompt_prefix(tr("Specify middle-right point of text or [Justify/Setfont]: ");
        }
        else if (str == "BL" || str == "BOTTOMLEFT") {
            mode = MODE_SETGEOM;
            textJustify = "Bottom Left";
            setRubberText("TEXT_JUSTIFY", textJustify);
            set_prompt_prefix(tr("Specify bottom-left point of text or [Justify/Setfont]: ");
        }
        else if (str == "BC" || str == "BOTTOMCENTER") {
            mode = MODE_SETGEOM;
            textJustify = "Bottom Center";
            setRubberText("TEXT_JUSTIFY", textJustify);
            set_prompt_prefix(tr("Specify bottom-center point of text or [Justify/Setfont]: "));
        }
        else if (str == "BR" || str == "BOTTOMRIGHT") {
            mode = MODE_SETGEOM;
            textJustify = "Bottom Right";
            setRubberText("TEXT_JUSTIFY", textJustify);
            set_prompt_prefix(tr("Specify bottom-right point of text or [Justify/Setfont]: "));
        }
        else {
            alert(translate("Invalid option keyword."));
            set_prompt_prefix(tr("Text Justification Options [Center/Right/Align/Middle/Fit/TL/TC/TR/ML/MC/MR/BL/BC/BR]: ");
        }
    }
    case "MODE_SETFONT") {
        mode = "MODE_SETGEOM";
        properties["textFont"] = str;
        setRubberText("TEXT_FONT", properties["textFont"].s);
        setTextFont(properties["textFont"].s);
        set_prompt_prefix(tr("Specify start point of text or [Justify/Setfont]: ");
    }
    case "MODE_SETGEOM") {
        if (std::isnan(textX)) {
            if (str == "J" || str == "JUSTIFY") {
                mode = "MODE_JUSTIFY";
                set_prompt_prefix(tr("Text Justification Options [Center/Right/Align/Middle/Fit/TL/TC/TR/ML/MC/MR/BL/BC/BR]: ");
            }
            else if (str == "S" || str == "SETFONT") {
                mode = MODE_SETFONT;
                set_prompt_prefix(tr("Specify font name: ");
            }
            else {
                EmbReal strList = str.split(",");
                if (std::isnan(strList[0]) || std::isnan(strList[1])) {
                    alert(tr("Point or option keyword required."));
                    set_prompt_prefix(tr("Specify start point of text or [Justify/Setfont]: ");
                }
                else {
                    properties["text.x"] = atof(strList[0]);
                    properties["text.y"] = atof(strList[1]);
                    addRubber("LINE");
                    setRubberMode("LINE");
                    setRubberPoint("LINE_START", textX, textY);
                    set_prompt_prefix(tr("Specify text height") + " {" + textSize() + "}: ");
                }
            }
        }
        else if (std::isnan(properties["textHeight"].r)) {
            if (str == "") {
                properties["textHeight"] = textSize();
                set_prompt_prefix(tr("Specify text angle") + " {" + textAngle() + "}: ");
            }
            else if (std::isnan(str)) {
                alert(tr("Requires valid numeric distance or second point."));
                set_prompt_prefix(tr("Specify text height") + " {" + textSize() + "}: ");
            }
            else {
                properties["textHeight"] = atof(str);
                setTextSize(properties["textHeight"]);
                set_prompt_prefix(tr("Specify text angle") + " {" + textAngle() + "}: ");
            }
        }
        else if (std::isnan(properties["textRotation"])) {
            if (str == "") {
                properties["textRotation"] = textAngle();
                set_prompt_prefix(tr("Enter text: ");
                mode = "MODE_RAPID";
                prompt->enableRapidFire();
                clearRubber();
                addRubber("TEXTSINGLE");
                setRubberMode("TEXTSINGLE");
                setRubberPoint("TEXT_POINT", textX, textY);
                setRubberPoint("TEXT_HEIGHT_ROTATION", properties["textHeight"].r, properties["textRotation"].r);
                setRubberText("TEXT_FONT", properties["textFont"].s);
                setRubberText("TEXT_JUSTIFY", textJustify);
                setRubberText("TEXT_RAPID", properties["text"].s);
            }
            else if (std::isnan(str)) {
                alert(tr("Requires valid numeric angle or second point."));
                set_prompt_prefix(tr("Specify text angle") + " {" + textAngle() + "}: ");
            }
            else {
                properties["textRotation"] = atof(str);
                setTextAngle(properties["textRotation"].r);
                set_prompt_prefix(tr("Enter text: ");
                mode = MODE_RAPID;
                prompt->enableRapidFire();
                clearRubber();
                addRubber("TEXTSINGLE");
                setRubberMode("TEXTSINGLE");
                setRubberPoint("TEXT_POINT", textX, textY);
                setRubberPoint("TEXT_HEIGHT_ROTATION", properties["textHeight"], properties["textRotation"]);
                setRubberText("TEXT_FONT", properties["textFont"]);
                setRubberText("TEXT_JUSTIFY", textJustify);
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
                end();
            }
            else {
                vulcanize();
                end();
                /** TODO: Rather than ending the command, calculate where the
                 * next line would be and modify the x/y to the new point.
                 */
            }
        }
        else {
            properties["text"] = atof(str);
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
    for (int i = 0; i <= numPoints; i++) {
        EmbReal t = (2.0*CONSTANT_PI) / numPoints*i;
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
    init();
    actuator("clear-selection");
    numPoints = atof(5);
    center.x = 0.0f;
    center.y = 0.0f;
    point1.x = atof(1.0f);
    point1.y = atof(1.0f);
    point2.x = atof(2.0f);
    point2.y = atof(2.0f);
    mode = MODE_STAR_NUM_POINTS;
    set_prompt_prefix(tr("Enter number of star points {5}: ");
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
        center = mouse;
        mode = STAR_MODE_RAD_OUTER;
        set_prompt_prefix(tr("Specify outer radius of star: ");
        addRubber("POLYGON");
        setRubberMode("POLYGON");
        updateStar(properties, center);
        actuator("enable move-rapid-fire");
        break;
    }

    case MODE_STAR_RAD_OUTER: {
        point1 = mouse;
        mode.s = "MODE_RAD_INNER";
        set_prompt_prefix(tr("Specify inner radius of star: ");
        updateStar(point1);
        break;
    }

    case MODE_STAR_RAD_INNER: {
        point2 = mouse;
        actuator("disable move-rapid-fire");
        updateStar(point2);
        spareRubber("POLYGON");
        end();
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
        if (str == "" && numPoints >= 3 && numPoints <= 1024) {
            set_prompt_prefix(tr("Specify center point: ");
            mode = MODE_STAR_CENTER_PT;
        }
        else {
            EmbReal tmp = atof(str);
            if (std::isnan(tmp) || !isInt(tmp) || tmp < 3 || tmp > 1024) {
                alert(tr("Requires an integer between 3 and 1024."));
                set_prompt_prefix(tr("Enter number of star points") + " {" + numPoints.toString() + "}: ");
            }
            else {
                numPoints = tmp;
                set_prompt_prefix(tr("Specify center point: ");
                mode = MODE_STAR_CENTER_PT;
            }
        }
        break;
    }

    case MODE_STAR_CENTER_PT: {
        EmbReal strList = str.split(",");
        if (std::isnan(strList[0]) || std::isnan(strList[1])) {
            alert(tr("Invalid point."));
            set_prompt_prefix(tr("Specify center point: ");
        }
        else {
            center.x = atof(strList[0]);
            center.y = atof(strList[1]);
            mode = MODE_STAR_RAD_OUTER;
            set_prompt_prefix(tr("Specify outer radius of star: ");
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
            set_prompt_prefix(tr("Specify outer radius of star: ");
        }
        else {
            point1.x = atof(strList[0]);
            point1.y = atof(strList[1]);
            mode = MODE_RAD_INNER;
            set_prompt_prefix(tr("Specify inner radius of star: ");
            updateStar(qsnapX(), qsnapY());
        }
        break;
    }

    case MODE_STAR_RAD_INNER: {
        EmbReal strList = str.split(",");
        if (std::isnan(strList[0]) || std::isnan(strList[1])) {
            alert(tr("Invalid point."));
            set_prompt_prefix(tr("Specify inner radius of star: ");
        }
        else {
            point2.x = atof(strList[0]);
            point2.y = atof(strList[1]);
            actuator("disable move-rapid-fire");
            updateStar(point2.x, point2.y);
            spareRubber("POLYGON");
            end();
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
    setRubberText("POLYGON_NUM_POINTS", (properties["numPoints*2"].i - 1).toString());
}
#endif

