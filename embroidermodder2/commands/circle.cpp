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

#define MODE_1P_RAD    0
#define MODE_1P_DIA    1
#define MODE_2P        2
#define MODE_3P        3
#define MODE_TTR       4
#define MODE_TTT       5

/* NOTE: main() is run every time the command is started.
 *       Use it to reset variables so they are ready to go.
 */
ScriptValue
circle_main(ScriptEnv *context)
{
    _main->nativeInitCommand();
    _main->nativeClearSelection();

    add_real_variable(context, "x1", 0.0f);
    add_real_variable(context, "y1", 0.0f);
    add_real_variable(context, "x2", 0.0f);
    add_real_variable(context, "y2", 0.0f);
    add_real_variable(context, "x3", 0.0f);
    add_real_variable(context, "y3", 0.0f);
    add_real_variable(context, "rad", 0.0f);
    add_real_variable(context, "dia", 0.0f);
    add_real_variable(context, "cx", 0.0f);
    add_real_variable(context, "cy", 0.0f);
    add_int_variable(context, "mode", MODE_1P_RAD);

    _main->nativeAppendPromptHistory(translate("Specify center point for circle or [3P/2P/Ttr (tan tan radius)]: "));
    return script_null;
}

/* NOTE: click() is run only for left clicks.
 *       Middle clicks are used for panning.
 *       Right clicks bring up the context menu.
 */
ScriptValue
circle_click(ScriptEnv *context)
{
    int mode = script_get_int(context, "mode");
    #if 0
    switch (mode) {
    case MODE_1P_RAD: {
        if (isNaN(global.x1)) {
            script_set_real(context, "x1", x);
            script_set_real(context, "y1", y);
            script_set_real(context, "cx", x);
            script_set_real(context, "cy", y);
            addRubber("CIRCLE");
            setRubberMode("CIRCLE_1P_RAD");
            setRubberPoint("CIRCLE_CENTER", x, y);
            appendPromptHistory();
            setPromptPrefix(qsTr("Specify radius of circle or [Diameter]: "));
        }
        else {
            script_set_real(context, "x2", x);
            script_set_real(context, "y2", y);
            setRubberPoint("CIRCLE_RADIUS", global.x2, global.y2);
            vulcanize();
            appendPromptHistory();
            endCommand();
        }
        break;
    }
    case MODE_1P_DIA: {
        if (isNaN(global.x1)) {
            error("CIRCLE", qsTr("This should never happen."));
        }
        else {
            script_set_real(context, "x2", x);
            script_set_real(context, "y2", y);
            setRubberPoint("CIRCLE_DIAMETER", global.x2, global.y2);
            vulcanize();
            appendPromptHistory();
            endCommand();
        }
        break;
    }
    case MODE_2P: {
        if (isNaN(global.x1)) {
            script_set_real(context, "x1", x);
            script_set_real(context, "y1", y);
            addRubber("CIRCLE");
            setRubberMode("CIRCLE_2P");
            setRubberPoint("CIRCLE_TAN1", x, y);
            appendPromptHistory();
            setPromptPrefix(qsTr("Specify second end point of circle's diameter: "));
        }
        else if (isNaN(global.x2)) {
            script_set_real(context, "x2", x);
            script_set_real(context, "y2", y);
            setRubberPoint("CIRCLE_TAN2", x, y);
            vulcanize();
            appendPromptHistory();
            endCommand();
        }
        else {
            error("CIRCLE", qsTr("This should never happen."));
        }
        break;
    }
    case MODE_3P: {
        if (isNaN(global.x1)) {
            global.x1 = x;
            global.y1 = y;
            appendPromptHistory();
            setPromptPrefix(qsTr("Specify second point on circle: "));
        }
        else if (isNaN(global.x2)) {
            script_set_real(context, "x2", x);
            script_set_real(context, "y2", y);
            addRubber("CIRCLE");
            setRubberMode("CIRCLE_3P");
            setRubberPoint("CIRCLE_TAN1", global.x1, global.y1);
            setRubberPoint("CIRCLE_TAN2", global.x2, global.y2);
            appendPromptHistory();
            setPromptPrefix(qsTr("Specify third point on circle: "));
        }
        else if (isNaN(global.x3)) {
            global.x3 = x;
            global.y3 = y;
            setRubberPoint("CIRCLE_TAN3", global.x3, global.y3);
            vulcanize();
            appendPromptHistory();
            endCommand();
        }
        else {
            error("CIRCLE", qsTr("This should never happen."));
        }
        break;
    }
    case MODE_TTR: {
        if (isNaN(global.x1)) {
            global.x1 = x;
            global.y1 = y;
            appendPromptHistory();
            setPromptPrefix(qsTr("Specify point on object for second tangent of circle: "));
        }
        else if (isNaN(global.x2)) {
            global.x2 = x;
            global.y2 = y;
            appendPromptHistory();
            setPromptPrefix(qsTr("Specify radius of circle: "));
        }
        else if (isNaN(global.x3)) {
            global.x3 = x;
            global.y3 = y;
            appendPromptHistory();
            setPromptPrefix(qsTr("Specify second point: "));
        }
        else {
            todo("CIRCLE", "click() for TTR");
        }
    }
    default:
        break;
    }
    #endif
    _main->nativeEndCommand();
    return script_null;
}

/* NOTE: context() is run when a context menu entry is chosen.
 */
ScriptValue
circle_context(ScriptEnv *context)
{
    // todo("CIRCLE", "context()");
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
    int mode = script_get_int(context, "mode");
    #if 0
    if (global.mode == MODE_1P_RAD) {
        if (isNaN(global.x1)) {
            if (str == "2P") //TODO: Probably should add additional qsTr calls here.
            {
                global.mode = global.mode_2P;
                setPromptPrefix(qsTr("Specify first end point of circle's diameter: "));
            }
            else if (str == "3P") //TODO: Probably should add additional qsTr calls here.
            {
                global.mode = global.mode_3P;
                setPromptPrefix(qsTr("Specify first point of circle: "));
            }
            else if (str == "T" || str == "TTR") //TODO: Probably should add additional qsTr calls here.
            {
                global.mode = global.mode_TTR;
                setPromptPrefix(qsTr("Specify point on object for first tangent of circle: "));
            }
            else {
                var strList = str.split(",");
                if (isNaN(strList[0]) || isNaN(strList[1])) {
                    alert(qsTr("Point or option keyword required."));
                    setPromptPrefix(qsTr("Specify center point for circle or [3P/2P/Ttr (tan tan radius)]: "));
                }
                else {
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
        else {
            if (str == "D" || str == "DIAMETER") //TODO: Probably should add additional qsTr calls here.
            {
                global.mode = global.mode_1P_DIA;
                setRubberMode("CIRCLE_1P_DIA");
                setPromptPrefix(qsTr("Specify diameter of circle: "));
            }
            else {
                var num = Number(str);
                if (isNaN(num)) {
                    alert(qsTr("Requires numeric radius, point on circumference, or \"D\"."));
                    setPromptPrefix(qsTr("Specify radius of circle or [Diameter]: "));
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
    else if (global.mode == MODE_1P_DIA) {
        if (isNaN(global.x1)) {
            error("CIRCLE", qsTr("This should never happen."));
        }
        if (isNaN(global.x2)) {
            var num = Number(str);
            if (isNaN(num)) {
                alert(qsTr("Requires numeric distance or second point."));
                setPromptPrefix(qsTr("Specify diameter of circle: "));
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
            error("CIRCLE", qsTr("This should never happen."));
        }
    }
    else if (global.mode == MODE_2P) {
        if (isNaN(global.x1)) {
            var strList = str.split(",");
            if (isNaN(strList[0]) || isNaN(strList[1])) {
                alert(qsTr("Invalid point."));
                setPromptPrefix(qsTr("Specify first end point of circle's diameter: "));
            }
            else {
                global.x1 = Number(strList[0]);
                global.y1 = Number(strList[1]);
                addRubber("CIRCLE");
                setRubberMode("CIRCLE_2P");
                setRubberPoint("CIRCLE_TAN1", global.x1, global.y1);
                setPromptPrefix(qsTr("Specify second end point of circle's diameter: "));
            }
        }
        else if (isNaN(global.x2)) {
            var strList = str.split(",");
            if (isNaN(strList[0]) || isNaN(strList[1])) {
                alert(qsTr("Invalid point."));
                setPromptPrefix(qsTr("Specify second end point of circle's diameter: "));
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
            error("CIRCLE", qsTr("This should never happen."));
        }
    }
    else if (global.mode == MODE_3P) {
        if (isNaN(global.x1)) {
            var strList = str.split(",");
            if (isNaN(strList[0]) || isNaN(strList[1])) {
                alert(qsTr("Invalid point."));
                setPromptPrefix(qsTr("Specify first point of circle: "));
            }
            else {
                global.x1 = Number(strList[0]);
                global.y1 = Number(strList[1]);
                setPromptPrefix(qsTr("Specify second point of circle: "));
            }
        }
        else if (isNaN(global.x2)) {
            var strList = str.split(",");
            if (isNaN(strList[0]) || isNaN(strList[1])) {
                alert(qsTr("Invalid point."));
                setPromptPrefix(qsTr("Specify second point of circle: "));
            }
            else {
                global.x2 = Number(strList[0]);
                global.y2 = Number(strList[1]);
                addRubber("CIRCLE");
                setRubberMode("CIRCLE_3P");
                setRubberPoint("CIRCLE_TAN1", global.x1, global.y1);
                setRubberPoint("CIRCLE_TAN2", global.x2, global.y2);
                setPromptPrefix(qsTr("Specify third point of circle: "));
            }
        }
        else if (isNaN(global.x3)) {
            var strList = str.split(",");
            if (isNaN(strList[0]) || isNaN(strList[1])) {
                alert(qsTr("Invalid point."));
                setPromptPrefix(qsTr("Specify third point of circle: "));
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
            error("CIRCLE", qsTr("This should never happen."));
        }
        
    }
    else if (global.mode == global.mode_TTR) {
        todo("CIRCLE", "prompt() for TTR");
    }
    #endif

    _main->nativeEndCommand();
    return script_null;
}
