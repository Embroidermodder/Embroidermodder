/*
 * Embroidermodder 2.
 *
 * Copyright 2011-2024 The Embroidermodder Team
 * Embroidermodder 2 is Open Source Software, see LICENSE.md for licensing terms.
 * Visit https://www.libembroidery.org/refman for advice on altering this file,
 * or read the markdown version in embroidermodder2/docs/refman.
 *
 * Commands: LOCATEPOINT, DISTANCE, MOVE
 */

#include "embroidermodder.h"

#define RGB_MODE_BACKGROUND     0
#define RGB_MODE_CROSSHAIR      1
#define RGB_MODE_GRID           2

#define ROTATE_MODE_NORMAL      0
#define ROTATE_MODE_REFERENCE   1

#define SCALE_MODE_NORMAL       0
#define SCALE_MODE_REFERENCE    1

/* LOCATEPOINT */
ScriptValue
locatepoint_command(ScriptEnv *context)
{
    switch (context->context) {
    case CONTEXT_MAIN:
        init_command();
        clear_selection();
        prompt_output(translate("Specify point: "));
        break;
    case CONTEXT_CLICK: {
        char output[200];
        float x = 0.0f;
        float y = 0.0f;
        sprintf(output, "X = %f, Y = %f", x, y);
        prompt_output(output);
        end_command();
        break;
    }
    case CONTEXT_CONTEXT:
        command_prompt(context, "todo LOCATEPOINT context()");
        break;
    case CONTEXT_PROMPT:
        /*
        EmbVector v;
        if (!parse_vector(str, &v)) {
            alert(translate("Invalid point."));
            prompt_output(translate("Specify point: "));
        }
        else {
            char output[200];
            sprintf(output, "X = %f, Y = %f", v.x, v.y);
            prompt_output(output);
            end_command();
        }
        */
        break;
    default:
        break;
    }
    return script_null;
}

/* Cartesian Coordinate System reported: anticlockwise angle from x-axis. */
void
reportDistance(EmbVector a, EmbVector b)
{
    char output[200];
    EmbVector delta = emb_vector_subtract(b, a);

    EmbReal distance = emb_vector_length(delta);
    EmbReal angle = emb_vector_angle(delta);

    sprintf(output, "%s = %d, %s = %d",
        translate("Distance"), distance, translate("Angle"), angle);
    prompt_output(output);
    sprintf(output, "%s = %d, %s = %d",
        translate("Delta X"), delta.x, translate("Delta Y"), delta.y);
    prompt_output(output);
}

/* DISTANCE. */
ScriptValue
distance_command(ScriptEnv *context)
{
    switch (context->context) {
    case CONTEXT_MAIN:
        init_command();
        clear_selection();
        /*
        global.point1.x = 0.0f;
        global.point1.y = 0.0f;
        global.point2.x = 0.0f;
        global.point2.y = 0.0f;
        */
        prompt_output(translate("Specify first point: "));
        break;
    case CONTEXT_CLICK:
        /*
        if (isNaN(global.x1)) {
            global.point1 = v;
            addRubber("LINE");
            setRubberMode("LINE");
            setRubberPoint("LINE_START", global.point1.x, global.point1.y);
            prompt_output(translate("Specify second point: "));
        }
        else {
            global.point2 = v;
            reportDistance();
            end_command();
        }
        */
        break;
    case CONTEXT_CONTEXT:
        /* todo("DISTANCE", "context()"); */
        break;
    case CONTEXT_PROMPT:
        /*
        var strList = str.split(",");
        if (isNaN(global.x1)) {
            if (isNaN(strList[0]) || isNaN(strList[1])) {
                alert(translate("Requires numeric distance or two points."));
                prompt_output(translate("Specify first point: "));
            }
            else {
                global.x1 = Number(strList[0]);
                global.y1 = Number(strList[1]);
                addRubber("LINE");
                setRubberMode("LINE");
                setRubberPoint("LINE_START", global.x1, global.y1);
                prompt_output(translate("Specify second point: "));
            }
        }
        else {
            if (isNaN(strList[0]) || isNaN(strList[1])) {
                alert(translate("Requires numeric distance or two points."));
                prompt_output(translate("Specify second point: "));
            }
            else {
                global.x2 = Number(strList[0]);
                global.y2 = Number(strList[1]);
                reportDistance();
                end_command();
            }
        }
        */
        break;
    default:
        break;
    }
    return script_null;
}

/* MOVE . */
ScriptValue
move_command(ScriptEnv * context)
{
    switch (context->context) {
    case CONTEXT_MAIN:
        break;
    case CONTEXT_CLICK:
        break;
    case CONTEXT_CONTEXT:
        break;
    case CONTEXT_PROMPT:
        break;
    default:
        break;
    }
    return script_null;
}

#if 0
void
main(void)
{
    init_command();
    global.firstRun = true;
    global.base.x = NaN;
    global.base.y = NaN;
    global.dest.x = NaN;
    global.dest.y = NaN;
    global.delta.x = NaN;
    global.delta.y = NaN;

    if (num_selected() <= 0) {
        /* TODO: Prompt to select objects if nothing is preselected. */
        alert(translate("Preselect objects before invoking the move command."));
        end_command();
        messageBox("information", translate("Move Preselect"),
            translate("Preselect objects before invoking the move command."));
    }
    else {
        prompt_output(translate("Specify base point: "));
    }
}

void
click(EmbVector v)
{
    if (global->firstRun) {
        global->firstRun = false;
        global->base = v;
        addRubber("LINE");
        setRubberMode("LINE");
        setRubberPoint("LINE_START", global->base);
        previewOn("SELECTED", "MOVE", global->base, 0);
        prompt_output(translate("Specify destination point: "));
    }
    else {
        global.dest = v;
        global.delta = emb_vector_subtract(global.dest, global.base);
        moveSelected(global.delta);
        previewOff();
        end_command();
    }
}

void context(str)
{
    todo("MOVE", "context()");
}

void prompt(str)
{
    EmbVector v;
    if (global.firstRun) {
        if (!parse_vector(str, &v)) {
            alert(translate("Invalid point."));
            prompt_output(translate("Specify base point: "));
        }
        else {
            global.firstRun = false;
            global.base = v;
            addRubber("LINE");
            setRubberMode("LINE");
            setRubberPoint("LINE_START", global.base.x, global.base.y);
            previewOn("SELECTED", "MOVE", global.base.x, global.base.y, 0);
            prompt_output(translate("Specify destination point: "));
        }
    }
    else {
        if (!parse_vector(str, &v)) {
            alert(translate("Invalid point."));
            prompt_output(translate("Specify destination point: "));
        }
        else {
            global.dest = v;
            global.delta = global.dest - global.base;
            moveSelected(global.delta.x, global.delta.y);
            previewOff();
            end_command();
        }
    }
}
#endif

/* SCALE . */
ScriptValue
scale_command(ScriptEnv * context)
{
    switch (context->context) {
    case CONTEXT_MAIN:
        break;
    case CONTEXT_CLICK:
        break;
    case CONTEXT_CONTEXT:
        break;
    case CONTEXT_PROMPT:
        break;
    default:
        break;
    }
    return script_null;
}

#if 0
void
main(void)
{
    init_command();
    global.mode = global.mode_NORMAL;
    global.firstRun = true;
    global.baseX = NaN;
    global.baseY = NaN;
    global.destX = NaN;
    global.destY = NaN;
    global.factor = NaN;

    global.baseRX = NaN;
    global.baseRY = NaN;
    global.destRX = NaN;
    global.destRY = NaN;
    global.factorRef = NaN;
    global.factorNew = NaN;

    if (num_selected() <= 0) {
        //TODO: Prompt to select objects if nothing is preselected
        alert(translate("Preselect objects before invoking the scale command."));
        endCommand();
        messageBox("information", translate("Scale Preselect"), translate("Preselect objects before invoking the scale command."));
    }
    else {
        prompt_output(translate("Specify base point: "));
    }
}

void
click(EmbVector position)
{
    if (global.mode == global.mode_NORMAL) {
        if (global.firstRun) {
            global.firstRun = false;
            global.base = position;
            addRubber("LINE");
            setRubberMode("LINE");
            setRubberPoint("LINE_START", global.baseX, global.baseY);
            previewOn("SELECTED", "SCALE", global.baseX, global.baseY, 1);
            prompt_output(translate("Specify scale factor or [Reference]: "));
        }
        else {
            global.destX = x;
            global.destY = y;
            global.factor = calculateDistance(global.baseX, global.baseY, global.destX, global.destY);
            scaleSelected(global.baseX, global.baseY, global.factor);
            previewOff();
            endCommand();
        }
    }
    else if (global.mode == global.mode_REFERENCE) {
        if (isNaN(global.baseRX)) {
            global.baseRX = x;
            global.baseRY = y;
            addRubber("LINE");
            setRubberMode("LINE");
            setRubberPoint("LINE_START", global.baseRX, global.baseRY);
            prompt_output(translate("Specify second point: "));
        }
        else if (isNaN(global.destRX)) {
            global.destRX = x;
            global.destRY = y;
            global.factorRef = calculateDistance(global.baseRX, global.baseRY, global.destRX, global.destRY);
            if (global.factorRef <= 0.0) {
                global.destRX    = NaN;
                global.destRY    = NaN;
                global.factorRef = NaN;
                alert(translate("Value must be positive and nonzero."));
                prompt_output(translate("Specify second point: "));
            }
            else {
                setRubberPoint("LINE_START", global.baseX, global.baseY);
                previewOn("SELECTED", "SCALE", global.baseX, global.baseY, global.factorRef);
                prompt_output(translate("Specify new length: "));
            }
        }
        else if (isNaN(global->factorNew)) {
            global->factorNew = calculateDistance(global->base, x, y);
            if (global->factorNew <= 0.0) {
                global->factorNew = NaN;
                alert(translate("Value must be positive and nonzero."));
                prompt_output(translate("Specify new length: "));
            }
            else {
                scaleSelected(global->base, global.factorNew/global.factorRef);
                previewOff();
                end_command();
            }
        }
    }
}

void context(str)
{
    todo("SCALE", "context()");
}

void prompt(str)
{
    EmbVector v;
    switch (global->mode) {
    default:
    case SCALE_MODE_NORMAL: {
        if (global->firstRun) {
            if (!parse_vector(str, &v)) {
                alert(translate("Invalid point."));
                prompt_output(translate("Specify base point: "));
            }
            else {
                global->firstRun = false;
                global->base = v;
                addRubber("LINE");
                setRubberMode("LINE");
                setRubberPoint("LINE_START", global.baseX, global.baseY);
                previewOn("SELECTED", "SCALE", global.baseX, global.baseY, 1);
                prompt_output(translate("Specify scale factor or [Reference]: "));
            }
        }
        else {
            if (str == "R" || str == "REFERENCE") {
                //TODO: Probably should add additional translate calls here.
                global.mode = global.mode_REFERENCE;
                prompt_output(translate("Specify reference length") + " {1}: ");
                clearRubber();
                previewOff();
            }
            else {
                if (isNaN(str)) {
                    alert(translate("Requires valid numeric distance, second point, or option keyword."));
                    prompt_output(translate("Specify scale factor or [Reference]: "));
                }
                else {
                    global.factor = Number(str);
                    scaleSelected(global.baseX, global.baseY, global.factor);
                    previewOff();
                    endCommand();
                }
            }
        }
        break;
    }
    case SCALE_MODE_REFERENCE: {
        if (isNaN(global.baseRX)) {
            if (isNaN(str)) {
                if (!parse_vector(str, &v)) {
                    alert(translate("Requires valid numeric distance or two points."));
                    prompt_output(translate("Specify reference length") + " {1}: ");
                }
                else {
                    global.baseR = v;
                    addRubber("LINE");
                    setRubberMode("LINE");
                    setRubberPoint("LINE_START", global.baseRX, global.baseRY);
                    prompt_output(translate("Specify second point: "));
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
                    alert(translate("Value must be positive and nonzero."));
                    prompt_output(translate("Specify reference length") + " {1}: ");
                }
                else {
                    addRubber("LINE");
                    setRubberMode("LINE");
                    setRubberPoint("LINE_START", global.baseX, global.baseY);
                    previewOn("SELECTED", "SCALE", global.baseX, global.baseY, global.factorRef);
                    prompt_output(translate("Specify new length: "));
                }
            }
        }
        else if (isNaN(global.destRX)) {
            if (isNaN(str)) {
                if (!parse_vector(str, &v)) {
                    alert(translate("Requires valid numeric distance or two points."));
                    prompt_output(translate("Specify second point: "));
                }
                else {
                    global.destR = v;
                    global.factorRef = calculateDistance(global.baseRX, global.baseRY, global.destRX, global.destRY);
                    if (global.factorRef <= 0.0) {
                        global.destRX    = NaN;
                        global.destRY    = NaN;
                        global.factorRef = NaN;
                        alert(translate("Value must be positive and nonzero."));
                        prompt_output(translate("Specify second point: "));
                    }
                    else {
                        setRubberPoint("LINE_START", global.baseX, global.baseY);
                        previewOn("SELECTED", "SCALE", global.baseX, global.baseY, global.factorRef);
                        prompt_output(translate("Specify new length: "));
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
                    alert(translate("Value must be positive and nonzero."));
                    prompt_output(translate("Specify second point: "));
                }
                else {
                    setRubberPoint("LINE_START", global.baseX, global.baseY);
                    previewOn("SELECTED", "SCALE", global.baseX, global.baseY, global.factorRef);
                    prompt_output(translate("Specify new length: "));
                }
            }
        }
        else if (isNaN(global.factorNew)) {
            if (isNaN(str)) {
                if (!parse_vector(str, &v)) {
                    alert(translate("Requires valid numeric distance or second point."));
                    prompt_output(translate("Specify new length: "));
                }
                else {
                    global.factorNew = embVector(global.base, v);
                    if (global.factorNew <= 0.0) {
                        global.factorNew = NaN;
                        alert(translate("Value must be positive and nonzero."));
                        prompt_output(translate("Specify new length: "));
                    }
                    else {
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
                    prompt_output(translate("Specify new length: "));
                }
                else {
                    scaleSelected(global.baseX, global.baseY, global.factorNew/global.factorRef);
                    previewOff();
                    endCommand();
                }
            }
        }
        break;
    }
    }
}
#endif

/* SANDBOX . */
ScriptValue
sandbox_command(ScriptEnv * context)
{
    init_command();
    clear_selection();

    switch (context->context) {
    case CONTEXT_MAIN:
        break;
    case CONTEXT_CLICK:
        break;
    case CONTEXT_CONTEXT:
        break;
    case CONTEXT_PROMPT:
        break;
    default:
        break;
    }

    end_command();
    return script_null;
}

#if 0
//Command: Sandbox

var global = {}; //Required
global.test1;
global.test2;

void main()
{
    init_command();
    
    //Report number of pre-selected objects
    prompt_output("Number of Objects Selected: " + numSelected().toString());
    
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
    

    end_command();
}
#endif

/* SELECTALL . */
ScriptValue
selectall_command(ScriptEnv * context)
{
    init_command();
    select_all();
    end_command();
    return script_null;
}

/* ROTATE . */
ScriptValue
rotate_command(ScriptEnv * context)
{
    init_command();
    clear_selection();

    end_command();
    return script_null;
}

#if 0
void
main()
{
    init_command();
    global.mode = global.mode_NORMAL;
    global.firstRun = true;
    global.base = zero_vector;
    global.dest = zero_vector;
    global.angle = 0.0f;

    global.baseR = zero_vector;
    global.destR = zero_vector;
    global.angleRef = 0.0f;
    global.angleNew = 0.0f;

    if (num_selected() <= 0) {
        /* TODO: Prompt to select objects if nothing is preselected. */
        alert(translate("Preselect objects before invoking the rotate command."));
        end_command();
        messageBox("information", translate("Rotate Preselect"), translate("Preselect objects before invoking the rotate command."));
    }
    else {
        prompt_output(translate("Specify base point: "));
    }
}

void
click(EmbVector v)
{
    if (global.mode == global.mode_NORMAL) {
        if (global.firstRun) {
            global.firstRun = false;
            global.base = v;
            addRubber("LINE");
            setRubberMode("LINE");
            setRubberPoint("LINE_START", global.baseX, global.baseY);
            previewOn("SELECTED", "ROTATE", global.baseX, global.baseY, 0);
            prompt_output(translate("Specify rotation angle or [Reference]: "));
        }
        else {
            global.dest = v;
            global.angle = emb_vector_angle(global.base, global.dest);
            rotateSelected(global.baseX, global.baseY, global.angle);
            previewOff();
            end_command();
        }
    }
    else if (global.mode == global.mode_REFERENCE) {
        if (isNaN(global.baseRX)) {
            global.baseR = v;
            addRubber("LINE");
            setRubberMode("LINE");
            setRubberPoint("LINE_START", global.baseRX, global.baseRY);
            prompt_output(translate("Specify second point: "));
        }
        else if (isNaN(global.destRX)) {
            global.destR = v;
            global.angleRef = calculateAngle(global.baseRX, global.baseRY, global.destRX, global.destRY);
            setRubberPoint("LINE_START", global.baseX, global.baseY);
            previewOn("SELECTED", "ROTATE", global.baseX, global.baseY, global.angleRef);
            prompt_output(translate("Specify the new angle: "));
        }
        else if (isNaN(global.angleNew)) {
            global.angleNew = calculateAngle(global.baseX, global.baseY, x, y);
            rotateSelected(global.baseX, global.baseY, global.angleNew - global.angleRef);
            previewOff();
            end_command();
        }
    }
}

void context(str)
{
    todo("ROTATE", "context()");
}

void prompt(str)
{
    EmbVector v;
    if (global.mode == global.mode_NORMAL) {
        if (global.firstRun) {
            if (!parse_vector(str, &v)) {
                alert(translate("Invalid point."));
                prompt_output(translate("Specify base point: "));
            }
            else {
                global.firstRun = false;
                global.base = v;
                addRubber("LINE");
                setRubberMode("LINE");
                setRubberPoint("LINE_START", global.base.x, global.base.y);
                previewOn("SELECTED", "ROTATE", global.base.x, global.base.y, 0);
                prompt_output(translate("Specify rotation angle or [Reference]: "));
            }
        }
        else {
            if (str == "R" || str == "REFERENCE") {
                global.mode = global.mode_REFERENCE;
                prompt_output(translate("Specify the reference angle") + " {0.00}: ");
                clearRubber();
                previewOff();
            }
            else {
                if (isNaN(str)) {
                    alert(translate("Requires valid numeric angle, second point, or option keyword."));
                    prompt_output(translate("Specify rotation angle or [Reference]: "));
                }
                else {
                    global.angle = Number(str);
                    rotateSelected(global.baseX, global.baseY, global.angle);
                    previewOff();
                    end_command();
                }
            }
        }
    }
    else if (global.mode == global.mode_REFERENCE) {
        if (isNaN(global.baseRX)) {
            if (isNaN(str)) {
                if (!parse_vector(str, &v)) {
                    alert(translate("Requires valid numeric angle or two points."));
                    prompt_output(translate("Specify the reference angle") + " {0.00}: ");
                }
                else {
                    global.baseR = v;
                    addRubber("LINE");
                    setRubberMode("LINE");
                    setRubberPoint("LINE_START", global.baseR.x, global.baseR.y);
                    prompt_output(translate("Specify second point: "));
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
                setRubberPoint("LINE_START", global.base.x, global.base.y);
                previewOn("SELECTED", "ROTATE", global.baseX, global.baseY, global.angleRef);
                prompt_output(translate("Specify the new angle: "));
            }
        }
        else if (isNaN(global.destRX)) {
            if (isNaN(str)) {
                if (!parse_vector(str, &v)) {
                    alert(translate("Requires valid numeric angle or two points."));
                    prompt_output(translate("Specify second point: "));
                }
                else {
                    global.destR = v;
                    global.angleRef = calculateAngle(global.baseRX, global.baseRY, global.destRX, global.destRY);
                    previewOn("SELECTED", "ROTATE", global.baseX, global.baseY, global.angleRef);
                    setRubberPoint("LINE_START", global.baseX, global.baseY);
                    prompt_output(translate("Specify the new angle: "));
                }
            }
            else {
                //The base and dest values are only set here to advance the command.
                global.baseR = zero_vector;
                global.destR = zero_vector;
                //The reference angle is what we will use later.
                global.angleRef = Number(str);
                previewOn("SELECTED", "ROTATE", global.baseX, global.baseY, global.angleRef);
                prompt_output(translate("Specify the new angle: "));
            }
        }
        else if (isNaN(global.angleNew)) {
            if (isNaN(str)) {
                if (!parse_vector(str, &v)) {
                    alert(translate("Requires valid numeric angle or second point."));
                    prompt_output(translate("Specify the new angle: "));
                }
                else {
                    global.angleNew = emb_vector_angle(global.base, v);
                    rotateSelected(global.base.x, global.base.y,
                        global.angleNew - global.angleRef);
                    previewOff();
                    end_command();
                }
            }
            else {
                global.angleNew = Number(str);
                rotateSelected(global.baseX, global.baseY, global.angleNew - global.angleRef);
                previewOff();
                end_command();
            }
        }
    }
}
#endif

/* RGB */
ScriptValue
rgb_command(ScriptEnv *context)
{
    switch (context->context) {
    default:
    case CONTEXT_MAIN:
        init_command();
        clear_selection();
        context->mode = RGB_MODE_BACKGROUND;
        prompt_output(translate("Enter RED,GREEN,BLUE values for background or [Crosshair/Grid]: "));
        break;
    case CONTEXT_CLICK:
        /* Do Nothing, prompt only command. */
        break;
    case CONTEXT_CONTEXT:
        /* todo("RGB", "context()"); */
        break;
    }

    return script_null;
}

#if 0
void prompt(str)
{
    float v[3];
    switch (global.mode) {
    default:
    case RGB_MODE_BACKGROUND: {
        if (str == "C" || str == "CROSSHAIR") {
            /* TODO: Probably should add additional translate calls here. */
            global.mode = RGB_MODE_CROSSHAIR;
            prompt_output(translate("Specify crosshair color: "));
        }
        else if (str == "G" || str == "GRID") {
            /* TODO: Probably should add additional translate calls here. */
            global.mode = RGB_MODE_GRID;
            prompt_output(translate("Specify grid color: "));
        }
        else {
            parse_floats(str, v, 3);
            if (!validRGB(v[0], v[1], v[2])) {
                alert(translate("Invalid color. R,G,B values must be in the range of 0-255."));
                prompt_output(translate("Specify background color: "));
            }
            else {
                setBackgroundColor(v[0], v[1], v[2]);
                end_command();
            }
        }
        break;
    }
    case RGB_MODE_CROSSHAIR: {
        parse_floats(str, v, 3);
        if (!validRGB(v[0], v[1], v[2])) {
            alert(translate("Invalid color. R,G,B values must be in the range of 0-255."));
            prompt_output(translate("Specify crosshair color: "));
        }
        else {
            setCrossHairColor(v[0], v[1], v[2]);
            end_command();
        }
        break;
    }
    case RGB_MODE_GRID: {
        parse_floats(str, v, 3);
        if (!validRGB(v[0], v[1], v[2])) {
            alert(translate("Invalid color. R,G,B values must be in the range of 0-255."));
            prompt_output(translate("Specify grid color: "));
        }
        else {
            setGridColor(v[0], v[1], v[2]);
            end_command();
        }
        break;
    }
    }
}

#endif
