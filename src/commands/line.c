/*
 * Embroidermodder 2
 * Copyright 2011-2025 The Embroidermodder Team
 *
 * Embroidermodder 2 is free and open software under the zlib license:
 * see LICENSE.md for details.
 *
 * ----------------------------------------------------------------------------
 *
 * Command that adds a line to the current pattern.
 */

#include "../core.h"

int
line_command(State *state)
{
    EmbVector start = emb_vector(0.0f, 0.0f);
    EmbVector end = emb_vector(0.0f, 10.0f);
    EmbReal rot = 0.0f;
    int rubberMode = OBJ_RUBBER_OFF;
    return line_create(start, end, rot, rubberMode);
}

#if 0
-- Command: Line

var global = {}-- -- Required
state.firstRun;
state.firstX;
state.firstY;
state.prevX;
state.prevY;

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
    setPromptPrefix(qsTr("Specify first point: "));
}

-- NOTE: click() is run only for left clicks.
--       Middle clicks are used for panning.
--       Right clicks bring up the context menu.
function click(x, y)
{
    if(state.firstRun) {
        state.firstRun = false;
        state.firstX = x;
        state.firstY = y;
        state.prevX = x;
        state.prevY = y;
        addRubber("LINE");
        setRubberMode("LINE");
        setRubberPoint("LINE_START", state.firstX, state.firstY);
        appendPromptHistory();
        setPromptPrefix(qsTr("Specify next point or [Undo]: "));
    }
    else {
        setRubberPoint("LINE_END", x, y);
        vulcanize();
        addRubber("LINE");
        setRubberMode("LINE");
        setRubberPoint("LINE_START", x, y);
        appendPromptHistory();
        state.prevX = x;
        state.prevY = y;
    }
}

-- NOTE: context() is run when a context menu entry is chosen.
function context(str)
{
    todo("LINE", "context()");
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
            addRubber("LINE");
            setRubberMode("LINE");
            setRubberPoint("LINE_START", state.firstX, state.firstY);
            setPromptPrefix(qsTr("Specify next point or [Undo]: "));
        }
    }
    else
    {
        if(str == "U" || str == "UNDO") -- TODO: Probably should add additional qsTr calls here.
        {
            todo("LINE", "prompt() for UNDO");
        }
        else
        {
            var strList = str.split(",");
            if(isNaN(strList[0]) || isNaN(strList[1]))
            {
                alert(qsTr("Point or option keyword required."));
                setPromptPrefix(qsTr("Specify next point or [Undo]: "));
            }
            else
            {
                var x = Number(strList[0]);
                var y = Number(strList[1]);
                setRubberPoint("LINE_END", x, y);
                vulcanize();
                addRubber("LINE");
                setRubberMode("LINE");
                setRubberPoint("LINE_START", x, y);
                state.prevX = x;
                state.prevY = y;
                setPromptPrefix(qsTr("Specify next point or [Undo]: "));
            }
        }
    }
}
#endif

