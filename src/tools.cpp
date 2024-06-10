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

bool validRGB(float r, float g, float b);
void reportDistance(EmbVector a, EmbVector b);

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
        var strList = str.split(",");
        if (isnan(strList[0]) || isnan(strList[1])) {
            alert(translate("Invalid point."));
            setPromptPrefix(translate("Specify point: "));
        }
        else {
            appendPromptHistory();
            setPromptPrefix("X = " + strList[0].toString() + ", Y = " + strList[1].toString());
            appendPromptHistory();
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
        global.x1 = NaN;
        global.y1 = NaN;
        global.x2 = NaN;
        global.y2 = NaN;
        */
        prompt_output(translate("Specify first point: "));
        break;
    case CONTEXT_CLICK:
        /*
        if (isNaN(global.x1)) {
            global.x1 = x;
            global.y1 = y;
            addRubber("LINE");
            setRubberMode("LINE");
            setRubberPoint("LINE_START", global.x1, global.y1);
            prompt_output(translate("Specify second point: "));
        }
        else {
            appendPromptHistory();
            global.x2 = x;
            global.y2 = y;
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
                setPromptPrefix(translate("Specify first point: "));
            }
            else {
                global.x1 = Number(strList[0]);
                global.y1 = Number(strList[1]);
                addRubber("LINE");
                setRubberMode("LINE");
                setRubberPoint("LINE_START", global.x1, global.y1);
                setPromptPrefix(translate("Specify second point: "));
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
var global = {}; //Required
global.firstRun;
global.baseX;
global.baseY;
global.destX;
global.destY;
global.deltaX;
global.deltaY;

function main()
{
    init_command();
    global.firstRun = true;
    global.baseX  = NaN;
    global.baseY  = NaN;
    global.destX  = NaN;
    global.destY  = NaN;
    global.deltaX = NaN;
    global.deltaY = NaN;

    if (numSelected() <= 0) {
        //TODO: Prompt to select objects if nothing is preselected
        alert(translate("Preselect objects before invoking the move command."));
        end_command();
        messageBox("information", translate("Move Preselect"), translate("Preselect objects before invoking the move command."));
    }
    else {
        setPromptPrefix(translate("Specify base point: "));
    }
}

function click(x, y)
{
    if (global.firstRun) {
        global.firstRun = false;
        global.baseX = x;
        global.baseY = y;
        addRubber("LINE");
        setRubberMode("LINE");
        setRubberPoint("LINE_START", global.baseX, global.baseY);
        previewOn("SELECTED", "MOVE", global.baseX, global.baseY, 0);
        prompt_output(translate("Specify destination point: "));
    }
    else {
        global.destX = x;
        global.destY = y;
        global.deltaX = global.destX - global.baseX;
        global.deltaY = global.destY - global.baseY;
        moveSelected(global.deltaX, global.deltaY);
        previewOff();
        end_command();
    }
}

function context(str)
{
    todo("MOVE", "context()");
}

function prompt(str)
{
    if (global.firstRun) {
        var strList = str.split(",");
        if (isNaN(strList[0]) || isNaN(strList[1])) {
            alert(translate("Invalid point."));
            prompt_output(translate("Specify base point: "));
        }
        else {
            global.firstRun = false;
            global.baseX = Number(strList[0]);
            global.baseY = Number(strList[1]);
            addRubber("LINE");
            setRubberMode("LINE");
            setRubberPoint("LINE_START", global.baseX, global.baseY);
            previewOn("SELECTED", "MOVE", global.baseX, global.baseY, 0);
            prompt_output(translate("Specify destination point: "));
        }
    }
    else {
        var strList = str.split(",");
        if (isNaN(strList[0]) || isNaN(strList[1])) {
            alert(translate("Invalid point."));
            prompt_output(translate("Specify destination point: "));
        }
        else {
            global.destX = Number(strList[0]);
            global.destY = Number(strList[1]);
            global.deltaX = global.destX - global.baseX;
            global.deltaY = global.destY - global.baseY;
            moveSelected(global.deltaX, global.deltaY);
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
//Command: Scale

var global = {}; //Required
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

//enums
global.mode_NORMAL    = 0;
global.mode_REFERENCE = 1;

function main()
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

function click(x, y)
{
    if (global.mode == global.mode_NORMAL) {
        if (global.firstRun) {
            global.firstRun = false;
            global.baseX = x;
            global.baseY = y;
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
            appendPromptHistory();
            scaleSelected(global.baseX, global.baseY, global.factor);
            previewOff();
            endCommand();
        }
    }
    else if (global.mode == global.mode_REFERENCE) {
        if (isNaN(global.baseRX)) {
            global.baseRX = x;
            global.baseRY = y;
            appendPromptHistory();
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
                appendPromptHistory();
                setRubberPoint("LINE_START", global.baseX, global.baseY);
                previewOn("SELECTED", "SCALE", global.baseX, global.baseY, global.factorRef);
                prompt_output(translate("Specify new length: "));
            }
        }
        else if (isNaN(global.factorNew)) {
            global.factorNew = calculateDistance(global.baseX, global.baseY, x, y);
            if (global.factorNew <= 0.0) {
                global.factorNew = NaN;
                alert(translate("Value must be positive and nonzero."));
                prompt_output(translate("Specify new length: "));
            }
            else {
                appendPromptHistory();
                scaleSelected(global.baseX, global.baseY, global.factorNew/global.factorRef);
                previewOff();
                end_command();
            }
        }
    }
}

function context(str)
{
    todo("SCALE", "context()");
}

function prompt(str)
{
    if (global.mode == global.mode_NORMAL) {
        if (global.firstRun) {
            var strList = str.split(",");
            if (isNaN(strList[0]) || isNaN(strList[1])) {
                alert(translate("Invalid point."));
                prompt_output(translate("Specify base point: "));
            }
            else {
                global.firstRun = false;
                global.baseX = Number(strList[0]);
                global.baseY = Number(strList[1]);
                addRubber("LINE");
                setRubberMode("LINE");
                setRubberPoint("LINE_START", global.baseX, global.baseY);
                previewOn("SELECTED", "SCALE", global.baseX, global.baseY, 1);
                prompt_output(translate("Specify scale factor or [Reference]: "));
            }
        }
        else {
            if (str == "R" || str == "REFERENCE") {
                //TODO: Probably should add additional qsTr calls here.
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
    }
    else if (global.mode == global.mode_REFERENCE) {
        if (isNaN(global.baseRX)) {
            if (isNaN(str)) {
                var strList = str.split(",");
                if (isNaN(strList[0]) || isNaN(strList[1])) {
                    alert(translate("Requires valid numeric distance or two points."));
                    setPromptPrefix(translate("Specify reference length") + " {1}: ");
                }
                else {
                    global.baseRX = Number(strList[0]);
                    global.baseRY = Number(strList[1]);
                    addRubber("LINE");
                    setRubberMode("LINE");
                    setRubberPoint("LINE_START", global.baseRX, global.baseRY);
                    setPromptPrefix(translate("Specify second point: "));
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
                    setPromptPrefix(translate("Specify reference length") + " {1}: ");
                }
                else {
                    addRubber("LINE");
                    setRubberMode("LINE");
                    setRubberPoint("LINE_START", global.baseX, global.baseY);
                    previewOn("SELECTED", "SCALE", global.baseX, global.baseY, global.factorRef);
                    setPromptPrefix(translate("Specify new length: "));
                }
            }
        }
        else if (isNaN(global.destRX)) {
            if (isNaN(str)) {
                var strList = str.split(",");
                if (isNaN(strList[0]) || isNaN(strList[1])) {
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
                    else {
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
                if (global.factorRef <= 0.0) {
                    global.destRX    = NaN;
                    global.destRY    = NaN;
                    global.factorRef = NaN;
                    alert(translate("Value must be positive and nonzero."));
                    setPromptPrefix(translate("Specify second point: "));
                }
                else {
                    setRubberPoint("LINE_START", global.baseX, global.baseY);
                    previewOn("SELECTED", "SCALE", global.baseX, global.baseY, global.factorRef);
                    setPromptPrefix(translate("Specify new length: "));
                }
            }
        }
        else if (isNaN(global.factorNew)) {
            if (isNaN(str)) {
                var strList = str.split(",");
                if (isNaN(strList[0]) || isNaN(strList[1])) {
                    alert(translate("Requires valid numeric distance or second point."));
                    setPromptPrefix(translate("Specify new length: "));
                }
                else {
                    var x = Number(strList[0]);
                    var y = Number(strList[1]);
                    global.factorNew = calculateDistance(global.baseX, global.baseY, x, y);
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

function main()
{
    init_command();
    
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
    

    end_command();
}
#endif

/* SELECTALL . */
ScriptValue
selectall_command(ScriptEnv * context)
{
    init_command();
    _main->selectAll();
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

function main()
{
    init_command();
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
        end_command();
        messageBox("information", translate("Rotate Preselect"), translate("Preselect objects before invoking the rotate command."));
    }
    else {
        setPromptPrefix(translate("Specify base point: "));
    }
}

function click(x, y)
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
            prompt_output(translate("Specify rotation angle or [Reference]: "));
        }
        else {
            global.destX = x;
            global.destY = y;
            global.angle = calculateAngle(global.baseX, global.baseY, global.destX, global.destY);
            appendPromptHistory();
            rotateSelected(global.baseX, global.baseY, global.angle);
            previewOff();
            end_command();
        }
    }
    else if (global.mode == global.mode_REFERENCE) {
        if (isNaN(global.baseRX)) {
            global.baseRX = x;
            global.baseRY = y;
            appendPromptHistory();
            addRubber("LINE");
            setRubberMode("LINE");
            setRubberPoint("LINE_START", global.baseRX, global.baseRY);
            prompt_output(translate("Specify second point: "));
        }
        else if (isNaN(global.destRX)) {
            global.destRX = x;
            global.destRY = y;
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

function context(str)
{
    todo("ROTATE", "context()");
}

function prompt(str)
{
    if (global.mode == global.mode_NORMAL) {
        if (global.firstRun) {
            var strList = str.split(",");
            if (isNaN(strList[0]) || isNaN(strList[1])) {
                alert(translate("Invalid point."));
                prompt_output(translate("Specify base point: "));
            }
            else {
                global.firstRun = false;
                global.baseX = Number(strList[0]);
                global.baseY = Number(strList[1]);
                addRubber("LINE");
                setRubberMode("LINE");
                setRubberPoint("LINE_START", global.baseX, global.baseY);
                previewOn("SELECTED", "ROTATE", global.baseX, global.baseY, 0);
                prompt_output(translate("Specify rotation angle or [Reference]: "));
            }
        }
        else {
            if (str == "R" || str == "REFERENCE") //TODO: Probably should add additional qsTr calls here.
            {
                global.mode = global.mode_REFERENCE;
                prompt_output(translate("Specify the reference angle") + " {0.00}: ");
                clearRubber();
                previewOff();
            }
            else {
                if (isNaN(str)) {
                    alert(translate("Requires valid numeric angle, second point, or option keyword."));
                    setPromptPrefix(translate("Specify rotation angle or [Reference]: "));
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
                var strList = str.split(",");
                if (isNaN(strList[0]) || isNaN(strList[1])) {
                    alert(translate("Requires valid numeric angle or two points."));
                    setPromptPrefix(translate("Specify the reference angle") + " {0.00}: ");
                }
                else {
                    global.baseRX = Number(strList[0]);
                    global.baseRY = Number(strList[1]);
                    addRubber("LINE");
                    setRubberMode("LINE");
                    setRubberPoint("LINE_START", global.baseRX, global.baseRY);
                    setPromptPrefix(translate("Specify second point: "));
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
                setPromptPrefix(translate("Specify the new angle: "));
            }
        }
        else if (isNaN(global.destRX)) {
            if (isNaN(str)) {
                var strList = str.split(",");
                if (isNaN(strList[0]) || isNaN(strList[1])) {
                    alert(translate("Requires valid numeric angle or two points."));
                    setPromptPrefix(translate("Specify second point: "));
                }
                else {
                    global.destRX = Number(strList[0]);
                    global.destRY = Number(strList[1]);
                    global.angleRef = calculateAngle(global.baseRX, global.baseRY, global.destRX, global.destRY);
                    previewOn("SELECTED", "ROTATE", global.baseX, global.baseY, global.angleRef);
                    setRubberPoint("LINE_START", global.baseX, global.baseY);
                    setPromptPrefix(translate("Specify the new angle: "));
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
                setPromptPrefix(translate("Specify the new angle: "));
            }
        }
        else if (isNaN(global.angleNew)) {
            if (isNaN(str)) {
                var strList = str.split(",");
                if (isNaN(strList[0]) || isNaN(strList[1])) {
                    alert(translate("Requires valid numeric angle or second point."));
                    setPromptPrefix(translate("Specify the new angle: "));
                }
                else {
                    var x = Number(strList[0]);
                    var y = Number(strList[1]);
                    global.angleNew = calculateAngle(global.baseX, global.baseY, x, y);
                    rotateSelected(global.baseX, global.baseY, global.angleNew - global.angleRef);
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
    init_command();
    clear_selection();

    end_command();
    return script_null;
}

#if 0
var global = {}; //Required
global.mode;

function main()
{
    init_command();
    clear_selection();
    global.mode = global.mode_BACKGROUND;
    prompt_output(translate("Enter RED,GREEN,BLUE values for background or [Crosshair/Grid]: "));
}

function click(x, y)
{
    //Do Nothing, prompt only command.
}

function context(str)
{
    todo("RGB", "context()");
}

function prompt(str)
{
    if (global.mode == RGB_MODE_BACKGROUND) {
        if (str == "C" || str == "CROSSHAIR") {
            //TODO: Probably should add additional translate calls here.
            global.mode = global.mode_CROSSHAIR;
            prompt_output(translate("Specify crosshair color: "));
        }
        else if (str == "G" || str == "GRID") {
            //TODO: Probably should add additional translate calls here.
            global.mode = global.mode_GRID;
            prompt_output(translate("Specify grid color: "));
        }
        else {
            var strList = str.split(",");
            var r = Number(strList[0]);
            var g = Number(strList[1]);
            var b = Number(strList[2]);
            if (!validRGB(r,g,b)) {
                alert(translate("Invalid color. R,G,B values must be in the range of 0-255."));
                setPromptPrefix(translate("Specify background color: "));
            }
            else {
                setBackgroundColor(r,g,b);
                end_command();
            }
        }
    }
    else if (global.mode == global.mode_CROSSHAIR) {
        var strList = str.split(",");
        var r = Number(strList[0]);
        var g = Number(strList[1]);
        var b = Number(strList[2]);
        if (!validRGB(r,g,b)) {
            alert(translate("Invalid color. R,G,B values must be in the range of 0-255."));
            setPromptPrefix(translate("Specify crosshair color: "));
        }
        else {
            setCrossHairColor(r,g,b);
            end_command();
        }
    }
    else if (global.mode == global.mode_GRID) {
        var strList = str.split(",");
        var r = Number(strList[0]);
        var g = Number(strList[1]);
        var b = Number(strList[2]);
        if (!validRGB(r,g,b)) {
            alert(translate("Invalid color. R,G,B values must be in the range of 0-255."));
            setPromptPrefix(translate("Specify grid color: "));
        }
        else {
            setGridColor(r,g,b);
            end_command();
        }
    }
}

#endif

bool
validRGB(float r, float g, float b)
{
    if (isnan(r)) {
        return false;
    }
    if (isnan(g)) {
        return false;
    }
    if (isnan(b)) {
        return false;
    }
    if (r < 0 || r > 255) {
        return false;
    }
    if (g < 0 || g > 255) {
        return false;
    }
    if (b < 0 || b > 255) {
        return false;
    }
    return true;
}
