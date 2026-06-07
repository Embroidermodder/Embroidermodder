/* Command: Distance */

#include <stdio.h>

#include "commands.h"

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
void report_distance(State *state)
{
    double dx = state->points[1].x - state->points[0].x;
    double dy = state->points[1].y - state->points[0].y;

    double dist = embVector_distance(state->points[0], state->points[1]);
    double angle = embVector_angle(state->points[0], state->points[1]);

    char line[1000];
    sprintf(line, "%s = %f, %s = %f\n",
        translate_str("Distance"), dist,
        translate_str("Angle"), angle);
    append_prompt_history(line);
    sprintf(line, "%s = %f, %s = %f\n",
        translate_str("Delta X"), dx,
        translate_str("Delta Y"), dy);
    append_prompt_history(line);
}

int distance_cmd(State *state)
{
    switch (state->context) {
    case CONTEXT_MAIN: {
        init_command();
        clear_selection();
        state->points_set = 0;
        set_prompt_prefix(translate_str("Specify first point: "));
        break;
    }
    case CONTEXT_CLICK:
        break;
    default:
        return 1;
    }
    return 0;
}

#if 0
//NOTE: click() is run only for left clicks.
//      Middle clicks are used for panning.
//      Right clicks bring up the context menu.
function click(x, y)
{
    if (isNaN(global.x1)) {
        state->points[0] = mouse_pos();
        add_rubber("LINE");
        set_rubber_mode("LINE");
        set_rubber_point("LINE_START", global.x1, global.y1);
        append_prompt_history();
        set_prompt_prefix(translate_str("Specify second point: "));
    }
    else {
        append_prompt_history();
        state->points[1] = mouse_pos();
        report_distance(state);
        end_command();
    }
}

//NOTE: context() is run when a context menu entry is chosen.
function context(str)
{
    todo("DISTANCE", "context()");
}

//NOTE: prompt() is run when Enter is pressed.
//      appendPromptHistory is automatically called before prompt()
//      is called so calling it is only needed for erroneous input.
//      Any text in the command prompt is sent as an uppercase string.
function prompt(str)
{
    var strList = str.split(",");
    if(isNaN(global.x1)) {
        if(isNaN(strList[0]) || isNaN(strList[1])) {
            alert(qsTr("Requires numeric distance or two points."));
            setPromptPrefix(qsTr("Specify first point: "));
        }
        else {
            global.x1 = Number(strList[0]);
            global.y1 = Number(strList[1]);
            addRubber("LINE");
            setRubberMode("LINE");
            setRubberPoint("LINE_START", global.x1, global.y1);
            setPromptPrefix(qsTr("Specify second point: "));
        }
    }
    else {
        if(isNaN(strList[0]) || isNaN(strList[1])) {
            alert(qsTr("Requires numeric distance or two points."));
            setPromptPrefix(qsTr("Specify second point: "));
        }
        else {
            global.x2 = Number(strList[0]);
            global.y2 = Number(strList[1]);
            reportDistance();
            end_command();
        }
    }
}

#endif

