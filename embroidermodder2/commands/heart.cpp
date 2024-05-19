/*
 * Embroidermodder 2.
 *
 * Copyright 2013-2024 The Embroidermodder Team
 * Embroidermodder 2 is Open Source Software.
 * See LICENSE for licensing terms.
 *
 * Command: HEART
 */

#include "../commands.h"

#define MODE_NUM_POINTS   0
#define MODE_STYLE        1
#define MODE_XSCALE       2
#define MODE_YSCALE       3

/* NOTE: main() is run every time the command is started.
 *       Use it to reset variables so they are ready to go.
 */
ScriptValue
heart_main(ScriptEnv * /* context */)
{
    _main->nativeInitCommand();
    _main->nativeClearSelection();

    _main->nativeEndCommand();
    return script_null;
}

/* NOTE: click() is run only for left clicks.
 *       Middle clicks are used for panning.
 *       Right clicks bring up the context menu.
 */
ScriptValue
heart_click(ScriptEnv * /* context */)
{

    _main->nativeEndCommand();
    return script_null;
}

/* NOTE: context() is run when a context menu entry is chosen.
 */
ScriptValue
heart_context(ScriptEnv * /* context */)
{

    _main->nativeEndCommand();
    return script_null;
}

/* NOTE: prompt() is run when Enter is pressed.
 *       appendPromptHistory is automatically called before prompt()
 *       is called so calling it is only needed for erroneous input.
 *       Any text is in the command prompt is sent as an uppercase string.
 */
ScriptValue
heart_prompt(ScriptEnv * /* context */)
{

    _main->nativeEndCommand();
    return script_null;
}

#if 0
//Command: Heart

var global = {}; //Required
global.numPoints = 512; //Default //TODO: min:64 max:8192
global.cx;
global.cy;
global.sx = 1.0;
global.sy = 1.0;
global.numPoints;
global.mode;


//NOTE: main() is run every time the command is started.
//      Use it to reset variables so they are ready to go.
function main()
{
    initCommand();
    clearSelection();
    global.cx = NaN;
    global.cy = NaN;
    global.mode = global.mode_NUM_POINTS;

    //Heart4: 10.0 / 512
    //Heart5: 1.0 / 512

    addRubber("POLYGON");
    setRubberMode("POLYGON");
    updateHeart("HEART5", global.numPoints, global.sx, global.sy);
    spareRubber("POLYGON");
    endCommand();
}

//NOTE: click() is run only for left clicks.
//      Middle clicks are used for panning.
//      Right clicks bring up the context menu.
function click(x, y)
{
}

//NOTE: move() is optional. It is run only after
//      enableMoveRapidFire() is called. It
//      will be called every time the mouse moves until
//      disableMoveRapidFire() is called.
function move(x, y)
{
}

//NOTE: context() is run when a context menu entry is chosen.
function context(str)
{
    todo("HEART", "context()");
}

//NOTE: prompt() is run when Enter is pressed.
//      appendPromptHistory is automatically called before prompt()
//      is called so calling it is only needed for erroneous input.
//      Any text in the command prompt is sent as an uppercase string.
function prompt(str)
{
}

function updateHeart(style, numPts, xScale, yScale)
{
    var i;
    var t;
    var xx = NaN;
    var yy = NaN;
    var two_pi = 2*Math.PI;

    for(i = 0; i <= numPts; i++)
    {
        t = two_pi/numPts*i; 

        if(style == "HEART4")
        {
            xx = Math.cos(t)*((Math.sin(t)*Math.sqrt(Math.abs(Math.cos(t))))/(Math.sin(t)+7/5) - 2*Math.sin(t) + 2);
            yy = Math.sin(t)*((Math.sin(t)*Math.sqrt(Math.abs(Math.cos(t))))/(Math.sin(t)+7/5) - 2*Math.sin(t) + 2);
        }
        else if(style == "HEART5")
        {
            xx = 16*Math.pow(Math.sin(t), 3);
            yy = 13*Math.cos(t) - 5*Math.cos(2*t) - 2*Math.cos(3*t) - Math.cos(4*t);
        }

        setRubberPoint("POLYGON_POINT_" + i.toString(), xx*xScale, yy*yScale);
    }

    setRubberText("POLYGON_NUM_POINTS", numPts.toString());
}
#endif
