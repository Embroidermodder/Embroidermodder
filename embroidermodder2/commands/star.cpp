/*
 * Embroidermodder 2.
 *
 * Copyright 2013-2024 The Embroidermodder Team
 * Embroidermodder 2 is Open Source Software, see LICENSE for licensing terms.
 * Read CODE-STANDARDS.txt for advice on altering this file.
 *
 * Command: STAR
 */

#include "../commands.h"

/* NOTE: main() is run every time the command is started.
 *       Use it to reset variables so they are ready to go.
 */
ScriptValue
star_command(ScriptEnv * context)
{
    return script_null;
}

#if 0
//Command: Star

var global = {}; //Required
global.numPoints = 5; //Default
global.cx;
global.cy;
global.x1;
global.y1;
global.x2;
global.y2;
global.mode;

//enums
global.mode_NUM_POINTS = 0;
global.mode_CENTER_PT  = 1;
global.mode_RAD_OUTER  = 2;
global.mode_RAD_INNER  = 3;

//NOTE: main() is run every time the command is started.
//      Use it to reset variables so they are ready to go.
function main()
{
    initCommand();
    clearSelection();
    global.cx       = NaN;
    global.cy       = NaN;
    global.x1       = NaN;
    global.y1       = NaN;
    global.x2       = NaN;
    global.y2       = NaN;
    global.mode = global.mode_NUM_POINTS;
    setPromptPrefix(qsTr("Enter number of star points") + " {" + global.numPoints.toString() + "}: ");
}

//NOTE: click() is run only for left clicks.
//      Middle clicks are used for panning.
//      Right clicks bring up the context menu.
function click(x, y)
{
    if(global.mode == global.mode_NUM_POINTS)
    {
        //Do nothing, the prompt controls this.
    }
    else if(global.mode == global.mode_CENTER_PT)
    {
        global.cx = x;
        global.cy = y;
        global.mode = global.mode_RAD_OUTER;
        setPromptPrefix(qsTr("Specify outer radius of star: "));
        addRubber("POLYGON");
        setRubberMode("POLYGON");
        updateStar(global.cx, global.cy);
        enableMoveRapidFire();
    }
    else if(global.mode == global.mode_RAD_OUTER)
    {
        global.x1 = x;
        global.y1 = y;
        global.mode = global.mode_RAD_INNER;
        setPromptPrefix(qsTr("Specify inner radius of star: "));
        updateStar(global.x1, global.y1);
    }
    else if(global.mode == global.mode_RAD_INNER)
    {
        global.x2 = x;
        global.y2 = y;
        disableMoveRapidFire();
        updateStar(global.x2, global.y2);
        spareRubber("POLYGON");
        endCommand();
    }
}

//NOTE: move() is optional. It is run only after
//      enableMoveRapidFire() is called. It
//      will be called every time the mouse moves until
//      disableMoveRapidFire() is called.
function move(x, y)
{
    if(global.mode == global.mode_NUM_POINTS)
    {
        //Do nothing, the prompt controls this.
    }
    else if(global.mode == global.mode_CENTER_PT)
    {
        //Do nothing, prompt and click controls this.
    }
    else if(global.mode == global.mode_RAD_OUTER)
    {
        updateStar(x, y);
    }
    else if(global.mode == global.mode_RAD_INNER)
    {
        updateStar(x, y);
    }
}

//NOTE: context() is run when a context menu entry is chosen.
function context(str)
{
    todo("STAR", "context()");
}

//NOTE: prompt() is run when Enter is pressed.
//      appendPromptHistory is automatically called before prompt()
//      is called so calling it is only needed for erroneous input.
//      Any text in the command prompt is sent as an uppercase string.
function prompt(str)
{
    if(global.mode == global.mode_NUM_POINTS)
    {
        if(str == "" && global.numPoints >= 3 && global.numPoints <= 1024)
        {
            setPromptPrefix(qsTr("Specify center point: "));
            global.mode = global.mode_CENTER_PT;
        }
        else
        {
            var tmp = Number(str);
            if(isNaN(tmp) || !isInt(tmp) || tmp < 3 || tmp > 1024)
            {
                alert(qsTr("Requires an integer between 3 and 1024."));
                setPromptPrefix(qsTr("Enter number of star points") + " {" + global.numPoints.toString() + "}: ");
            }
            else
            {
                global.numPoints = tmp;
                setPromptPrefix(qsTr("Specify center point: "));
                global.mode = global.mode_CENTER_PT;
            }
        }
    }
    else if(global.mode == global.mode_CENTER_PT)
    {
        var strList = str.split(",");
        if(isNaN(strList[0]) || isNaN(strList[1]))
        {
            alert(qsTr("Invalid point."));
            setPromptPrefix(qsTr("Specify center point: "));
        }
        else
        {
            global.cx = Number(strList[0]);
            global.cy = Number(strList[1]);
            global.mode = global.mode_RAD_OUTER;
            setPromptPrefix(qsTr("Specify outer radius of star: "));
            addRubber("POLYGON");
            setRubberMode("POLYGON");
            updateStar(qsnapX(), qsnapY());
            enableMoveRapidFire();
        }
    }
    else if(global.mode == global.mode_RAD_OUTER)
    {
        var strList = str.split(",");
        if(isNaN(strList[0]) || isNaN(strList[1]))
        {
            alert(qsTr("Invalid point."));
            setPromptPrefix(qsTr("Specify outer radius of star: "));
        }
        else
        {
            global.x1 = Number(strList[0]);
            global.y1 = Number(strList[1]);
            global.mode = global.mode_RAD_INNER;
            setPromptPrefix(qsTr("Specify inner radius of star: "));
            updateStar(qsnapX(), qsnapY());
        }
    }
    else if(global.mode == global.mode_RAD_INNER)
    {
        var strList = str.split(",");
        if(isNaN(strList[0]) || isNaN(strList[1]))
        {
            alert(qsTr("Invalid point."));
            setPromptPrefix(qsTr("Specify inner radius of star: "));
        }
        else
        {
            global.x2 = Number(strList[0]);
            global.y2 = Number(strList[1]);
            disableMoveRapidFire();
            updateStar(global.x2, global.y2);
            spareRubber("POLYGON");
            endCommand();
        }
    }
}

function updateStar(x, y)
{
    var distOuter;
    var distInner;
    var angOuter;

    if(global.mode == global.mode_RAD_OUTER)
    {
        angOuter = calculateAngle(global.cx, global.cy, x, y);
        distOuter = calculateDistance(global.cx, global.cy, x, y);
        distInner = distOuter/2.0;
    }
    else if(global.mode == global.mode_RAD_INNER)
    {
        angOuter = calculateAngle(global.cx, global.cy, global.x1, global.y1);
        distOuter = calculateDistance(global.cx, global.cy, global.x1, global.y1);
        distInner = calculateDistance(global.cx, global.cy, x, y);
    }

    //Calculate the Star Points
    var angInc = 360.0/(global.numPoints*2);
    var odd = true;
    for(var i = 0; i < global.numPoints*2; i++)
    {
        var xx;
        var yy;
        if(odd)
        {
            xx = distOuter*Math.cos((angOuter+(angInc*i))*Math.PI/180.0);
            yy = distOuter*Math.sin((angOuter+(angInc*i))*Math.PI/180.0);
        }
        else
        {
            xx = distInner*Math.cos((angOuter+(angInc*i))*Math.PI/180.0);
            yy = distInner*Math.sin((angOuter+(angInc*i))*Math.PI/180.0);
        }
        odd = !odd;
        setRubberPoint("POLYGON_POINT_" + i.toString(), global.cx + xx, global.cy + yy);
    }
    setRubberText("POLYGON_NUM_POINTS", (global.numPoints*2 - 1).toString());
}

[Menu]
Name=Draw
Position=100

[ToolBar]
Name=Draw
Position=100

[Tips]
ToolTip=&Star
StatusTip=Creates a star:  STAR

[Prompt]
Alias=*, STAR
#endif
