/*
 * Embroidermodder 2
 * Copyright 2011-2025 The Embroidermodder Team
 *
 * Embroidermodder 2 is free and open software under the zlib license:
 * see LICENSE.md for details.
 *
 * ----------------------------------------------------------------------------
 *
 * Command that adds an arc to the design.
 *
 * TODO: argument parsing
 */

#include "../core.h"

int
rectangle_command(State *state)
{
    //FIXME: rectangle_function();
    return 0;
}

#if 0
-- Command: Rectangle

var global = {}-- -- Required
state.newRect;
state.x1;
state.y1;
state.x2;
state.y2;

-- NOTE: main() is run every time the command is started.
--       Use it to reset variables so they are ready to go.
function main()
{
    initCommand();
    clearSelection();
    state.newRect = true;
    state.x1 = NaN;
    state.y1 = NaN;
    state.x2 = NaN;
    state.y2 = NaN;
    setPromptPrefix(qsTr("Specify first corner point or [Chamfer/Fillet]: "));
}

-- NOTE: click() is run only for left clicks.
--       Middle clicks are used for panning.
--       Right clicks bring up the context menu.
function click(x, y)
{
    if(state.newRect) {
        state.newRect = false;
        state.x1 = x;
        state.y1 = y;
        addRubber("RECTANGLE");
        setRubberMode("RECTANGLE");
        setRubberPoint("RECTANGLE_START", x, y);
        setPromptPrefix(qsTr("Specify other corner point or [Dimensions]: "));
    }
    else {
        state.newRect = true;
        state.x2 = x;
        state.y2 = y;
        setRubberPoint("RECTANGLE_END", x, y);
        vulcanize();
        endCommand();
    }
}

-- NOTE: context() is run when a context menu entry is chosen.
function context(str)
{
    todo("RECTANGLE", "context()");
}

-- NOTE: prompt() is run when Enter is pressed.
--       appendPromptHistory is automatically called before prompt()
--       is called so calling it is only needed for erroneous input.
--       Any text in the command prompt is sent as an uppercase string.
function prompt(str)
{
    if(str == "C" || str == "CHAMFER") {
        todo("RECTANGLE", "prompt() for CHAMFER");
    }
    else if(str == "D" || str == "DIMENSIONS") {
        todo("RECTANGLE", "prompt() for DIMENSIONS");
    }
    else if(str == "F" || str == "FILLET") {
        todo("RECTANGLE", "prompt() for FILLET");
    }
    else
    {
        var strList = str.split(",");
        if (isNaN(strList[0]) || isNaN(strList[1])) {
            alert(qsTr("Invalid point."));
            setPromptPrefix(qsTr("Specify first point: "));
        }
        else {
            var x = Number(strList[0]);
            var y = Number(strList[1]);
            if(state.newRect) {
                state.newRect = false;
                state.x1 = x;
                state.y1 = y;
                addRubber("RECTANGLE");
                setRubberMode("RECTANGLE");
                setRubberPoint("RECTANGLE_START", x, y);
                setPromptPrefix(qsTr("Specify other corner point or [Dimensions]: "));
            }
            else {
                state.newRect = true
                state.x2 = x
                state.y2 = y
                setRubberPoint("RECTANGLE_END", x, y)
                vulcanize()
                endCommand()
            }
        }
    }
}
#endif

