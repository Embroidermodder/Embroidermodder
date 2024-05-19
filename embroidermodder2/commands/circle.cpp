/*
 * Embroidermodder 2.
 *
 * Copyright 2013-2024 The Embroidermodder Team
 * Embroidermodder 2 is Open Source Software.
 * See LICENSE for licensing terms.
 *
 * Command: about
 */

#include "../commands.h"

/* NOTE: main() is run every time the command is started.
 *       Use it to reset variables so they are ready to go.
 */
ScriptValue
circle_main(ScriptEnv *context)
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
circle_click(ScriptEnv *context)
{

    _main->nativeEndCommand();
    return script_null;
}

/* NOTE: context() is run when a context menu entry is chosen.
 */
ScriptValue
circle_context(ScriptEnv *context)
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
circle_prompt(ScriptEnv *context)
{

    _main->nativeEndCommand();
    return script_null;
}

#if 0
//Command: Circle

var global = {}; //Required
global.x1;
global.y1;
global.x2;
global.y2;
global.x3;
global.y3;
global.rad;
global.dia;
global.cx;
global.cy;
global.mode;

//enums
global.mode_1P_RAD = 0;
global.mode_1P_DIA = 1;
global.mode_2P     = 2;
global.mode_3P     = 3;
global.mode_TTR    = 4;

//NOTE: main() is run every time the command is started.
//      Use it to reset variables so they are ready to go.
function main()
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
    setPromptPrefix(qsTr("Specify center point for circle or [3P/2P/Ttr (tan tan radius)]: "));
}

//NOTE: click() is run only for left clicks.
//      Middle clicks are used for panning.
//      Right clicks bring up the context menu.
function click(x, y)
{
    if(global.mode == global.mode_1P_RAD)
    {
        if(isNaN(global.x1))
        {
            global.x1 = x;
            global.y1 = y;
            global.cx = x;
            global.cy = y;
            addRubber("CIRCLE");
            setRubberMode("CIRCLE_1P_RAD");
            setRubberPoint("CIRCLE_CENTER", global.cx, global.cy);
            appendPromptHistory();
            setPromptPrefix(qsTr("Specify radius of circle or [Diameter]: "));
        }
        else
        {
            global.x2 = x;
            global.y2 = y;
            setRubberPoint("CIRCLE_RADIUS", global.x2, global.y2);
            vulcanize();
            appendPromptHistory();
            endCommand();
        }
    }
    else if(global.mode == global.mode_1P_DIA)
    {
        if(isNaN(global.x1))
        {
            error("CIRCLE", qsTr("This should never happen."));
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
    else if(global.mode == global.mode_2P)
    {
        if(isNaN(global.x1))
        {
            global.x1 = x;
            global.y1 = y;
            addRubber("CIRCLE");
            setRubberMode("CIRCLE_2P");
            setRubberPoint("CIRCLE_TAN1", global.x1, global.y1);
            appendPromptHistory();
            setPromptPrefix(qsTr("Specify second end point of circle's diameter: "));
        }
        else if(isNaN(global.x2))
        {
            global.x2 = x;
            global.y2 = y;
            setRubberPoint("CIRCLE_TAN2", global.x2, global.y2);
            vulcanize();
            appendPromptHistory();
            endCommand();
        }
        else
        {
            error("CIRCLE", qsTr("This should never happen."));
        }
    }
    else if(global.mode == global.mode_3P)
    {
        if(isNaN(global.x1))
        {
            global.x1 = x;
            global.y1 = y;
            appendPromptHistory();
            setPromptPrefix(qsTr("Specify second point on circle: "));
        }
        else if(isNaN(global.x2))
        {
            global.x2 = x;
            global.y2 = y;
            addRubber("CIRCLE");
            setRubberMode("CIRCLE_3P");
            setRubberPoint("CIRCLE_TAN1", global.x1, global.y1);
            setRubberPoint("CIRCLE_TAN2", global.x2, global.y2);
            appendPromptHistory();
            setPromptPrefix(qsTr("Specify third point on circle: "));
        }
        else if(isNaN(global.x3))
        {
            global.x3 = x;
            global.y3 = y;
            setRubberPoint("CIRCLE_TAN3", global.x3, global.y3);
            vulcanize();
            appendPromptHistory();
            endCommand();
        }
        else
        {
            error("CIRCLE", qsTr("This should never happen."));
        }
    }
    else if(global.mode == global.mode_TTR)
    {
        if(isNaN(global.x1))
        {
            global.x1 = x;
            global.y1 = y;
            appendPromptHistory();
            setPromptPrefix(qsTr("Specify point on object for second tangent of circle: "));
        }
        else if(isNaN(global.x2))
        {
            global.x2 = x;
            global.y2 = y;
            appendPromptHistory();
            setPromptPrefix(qsTr("Specify radius of circle: "));
        }
        else if(isNaN(global.x3))
        {
            global.x3 = x;
            global.y3 = y;
            appendPromptHistory();
            setPromptPrefix(qsTr("Specify second point: "));
        }
        else
        {
            todo("CIRCLE", "click() for TTR");
        }
    }
}

//NOTE: context() is run when a context menu entry is chosen.
function context(str)
{
    todo("CIRCLE", "context()");
}

//NOTE: prompt() is run when Enter is pressed.
//      appendPromptHistory is automatically called before prompt()
//      is called so calling it is only needed for erroneous input.
//      Any text in the command prompt is sent as an uppercase string.
function prompt(str)
{
    if(global.mode == global.mode_1P_RAD)
    {
        if(isNaN(global.x1))
        {
            if(str == "2P") //TODO: Probably should add additional qsTr calls here.
            {
                global.mode = global.mode_2P;
                setPromptPrefix(qsTr("Specify first end point of circle's diameter: "));
            }
            else if(str == "3P") //TODO: Probably should add additional qsTr calls here.
            {
                global.mode = global.mode_3P;
                setPromptPrefix(qsTr("Specify first point of circle: "));
            }
            else if(str == "T" || str == "TTR") //TODO: Probably should add additional qsTr calls here.
            {
                global.mode = global.mode_TTR;
                setPromptPrefix(qsTr("Specify point on object for first tangent of circle: "));
            }
            else
            {
                var strList = str.split(",");
                if(isNaN(strList[0]) || isNaN(strList[1]))
                {
                    alert(qsTr("Point or option keyword required."));
                    setPromptPrefix(qsTr("Specify center point for circle or [3P/2P/Ttr (tan tan radius)]: "));
                }
                else
                {
                    global.x1 = Number(strList[0]);
                    global.y1 = Number(strList[1]);
                    global.cx = global.x1;
                    global.cy = global.y1;
                    addRubber("CIRCLE");
                    setRubberMode("CIRCLE_1P_RAD");
                    setRubberPoint("CIRCLE_CENTER", global.cx, global.cy);
                    setPromptPrefix(qsTr("Specify radius of circle or [Diameter]: "));
                }
            }
        }
        else
        {
            if(str == "D" || str == "DIAMETER") //TODO: Probably should add additional qsTr calls here.
            {
                global.mode = global.mode_1P_DIA;
                setRubberMode("CIRCLE_1P_DIA");
                setPromptPrefix(qsTr("Specify diameter of circle: "));
            }
            else
            {
                var num = Number(str);
                if(isNaN(num))
                {
                    alert(qsTr("Requires numeric radius, point on circumference, or \"D\"."));
                    setPromptPrefix(qsTr("Specify radius of circle or [Diameter]: "));
                }
                else
                {
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
    else if(global.mode == global.mode_1P_DIA)
    {
        if(isNaN(global.x1))
        {
            error("CIRCLE", qsTr("This should never happen."));
        }
        if(isNaN(global.x2))
        {
            var num = Number(str);
            if(isNaN(num))
            {
                alert(qsTr("Requires numeric distance or second point."));
                setPromptPrefix(qsTr("Specify diameter of circle: "));
            }
            else
            {
                global.dia = num;
                global.x2 = global.x1 + global.dia;
                global.y2 = global.y1;
                setRubberPoint("CIRCLE_DIAMETER", global.x2, global.y2);
                vulcanize();
                endCommand();
            }
        }
        else
        {
            error("CIRCLE", qsTr("This should never happen."));
        }
    }
    else if(global.mode == global.mode_2P)
    {
        if(isNaN(global.x1))
        {
            var strList = str.split(",");
            if(isNaN(strList[0]) || isNaN(strList[1]))
            {
                alert(qsTr("Invalid point."));
                setPromptPrefix(qsTr("Specify first end point of circle's diameter: "));
            }
            else
            {
                global.x1 = Number(strList[0]);
                global.y1 = Number(strList[1]);
                addRubber("CIRCLE");
                setRubberMode("CIRCLE_2P");
                setRubberPoint("CIRCLE_TAN1", global.x1, global.y1);
                setPromptPrefix(qsTr("Specify second end point of circle's diameter: "));
            }
        }
        else if(isNaN(global.x2))
        {
            var strList = str.split(",");
            if(isNaN(strList[0]) || isNaN(strList[1]))
            {
                alert(qsTr("Invalid point."));
                setPromptPrefix(qsTr("Specify second end point of circle's diameter: "));
            }
            else
            {
                global.x2 = Number(strList[0]);
                global.y2 = Number(strList[1]);
                setRubberPoint("CIRCLE_TAN2", global.x2, global.y2);
                vulcanize();
                endCommand();
            }
        }
        else
        {
            error("CIRCLE", qsTr("This should never happen."));
        }
    }
    else if(global.mode == global.mode_3P)
    {
        if(isNaN(global.x1))
        {
            var strList = str.split(",");
            if(isNaN(strList[0]) || isNaN(strList[1]))
            {
                alert(qsTr("Invalid point."));
                setPromptPrefix(qsTr("Specify first point of circle: "));
            }
            else
            {
                global.x1 = Number(strList[0]);
                global.y1 = Number(strList[1]);
                setPromptPrefix(qsTr("Specify second point of circle: "));
            }
        }
        else if(isNaN(global.x2))
        {
            var strList = str.split(",");
            if(isNaN(strList[0]) || isNaN(strList[1]))
            {
                alert(qsTr("Invalid point."));
                setPromptPrefix(qsTr("Specify second point of circle: "));
            }
            else
            {
                global.x2 = Number(strList[0]);
                global.y2 = Number(strList[1]);
                addRubber("CIRCLE");
                setRubberMode("CIRCLE_3P");
                setRubberPoint("CIRCLE_TAN1", global.x1, global.y1);
                setRubberPoint("CIRCLE_TAN2", global.x2, global.y2);
                setPromptPrefix(qsTr("Specify third point of circle: "));
            }
        }
        else if(isNaN(global.x3))
        {
            var strList = str.split(",");
            if(isNaN(strList[0]) || isNaN(strList[1]))
            {
                alert(qsTr("Invalid point."));
                setPromptPrefix(qsTr("Specify third point of circle: "));
            }
            else
            {                
                global.x3 = Number(strList[0]);
                global.y3 = Number(strList[1]);
                setRubberPoint("CIRCLE_TAN3", global.x3, global.y3);
                vulcanize();
                endCommand();
            }
        }
        else
        {
            error("CIRCLE", qsTr("This should never happen."));
        }
        
    }
    else if(global.mode == global.mode_TTR)
    {
        todo("CIRCLE", "prompt() for TTR");
    }
}

#endif
