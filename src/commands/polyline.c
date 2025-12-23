/*
 * Embroidermodder 2
 * Copyright 2011-2025 The Embroidermodder Team
 *
 * Embroidermodder 2 is free and open software under the zlib license:
 * see LICENSE.md for details.
 *
 * ----------------------------------------------------------------------------
 *
 * Command that adds a polyline to the current pattern.
 */

#include "../core.h"

int
polyline_command(State *state)
{
    //FIXME: polyline_function();
    return 0;
}

#if 0
-- Command: Polyline

var global = {}-- -- Required
state.firstRun;
state.firstX;
state.firstY;
state.prevX;
state.prevY;
state.num;

-- NOTE: main() is run every time the command is started.
--       Use it to reset variables so they are ready to go.
function main()
{
    initCommand();
    clearSelection();
    state.firstRun = true;
    state.firstX = NaN;
    state.firstY = NaN;
    state.prevX = NaN;
    state.prevY = NaN;
    state.num = 0;
    setPromptPrefix(qsTr("Specify first point: "));
}

-- NOTE: click() is run only for left clicks.
--       Middle clicks are used for panning.
--       Right clicks bring up the context menu.
function click(x, y)
{
    if (state.firstRun) {
        state.firstRun = false;
        state.firstX = x;
        state.firstY = y;
        state.prevX = x;
        state.prevY = y;
        addRubber("POLYLINE");
        setRubberMode("POLYLINE");
        setRubberPoint("POLYLINE_POINT_0", state.firstX, state.firstY);
        appendPromptHistory();
        setPromptPrefix(qsTr("Specify next point or [Undo]: "));
    }
    else {
        state.num++;
        setRubberPoint("POLYLINE_POINT_" + state.num.toString(), x, y);
        setRubberText("POLYLINE_NUM_POINTS", state.num.toString());
        spareRubber("POLYLINE");
        appendPromptHistory();
        state.prevX = x;
        state.prevY = y;
    }
}

-- NOTE: context() is run when a context menu entry is chosen.
function context(str)
{
    todo("POLYLINE", "context()");
}

-- NOTE: prompt() is run when Enter is pressed.
--       appendPromptHistory is automatically called before prompt()
--       is called so calling it is only needed for erroneous input.
--       Any text in the command prompt is sent as an uppercase string.
function prompt(str)
{
    if(state.firstRun)
    {
        var strList = str.split(",");
        if(isNaN(strList[0]) || isNaN(strList[1]))
        {
            alert(qsTr("Invalid point."));
            setPromptPrefix(qsTr("Specify first point: "));
        }
        else
        {
            state.firstRun = false;
            state.firstX = Number(strList[0]);
            state.firstY = Number(strList[1]);
            state.prevX = state.firstX;
            state.prevY = state.firstY;
            addRubber("POLYLINE");
            setRubberMode("POLYLINE");
            setRubberPoint("POLYLINE_POINT_0", state.firstX, state.firstY);
            setPromptPrefix(qsTr("Specify next point or [Undo]: "));
        }
    }
    else {
        if(str == "U" || str == "UNDO") {
            todo("POLYLINE", "prompt() for UNDO");
        }
        else {
            var strList = str.split(",");
            if (isNaN(strList[0]) || isNaN(strList[1])) then
                alert(qsTr("Point or option keyword required."));
                setPromptPrefix(qsTr("Specify next point or [Undo]: "));
            else {
                var x = Number(strList[0]);
                var y = Number(strList[1]);
                state.num++;
                setRubberPoint("POLYLINE_POINT_" + state.num.toString(), x, y);
                setRubberText("POLYLINE_NUM_POINTS", state.num.toString());
                spareRubber("POLYLINE");
                state.prevX = x;
                state.prevY = y;
                setPromptPrefix(qsTr("Specify next point or [Undo]: "));
            }
        }
    }
}
#endif

