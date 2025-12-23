/*
 * Embroidermodder 2
 * Copyright 2011-2025 The Embroidermodder Team
 *
 * Embroidermodder 2 is free and open software under the zlib license:
 * see LICENSE.md for details.
 *
 * ----------------------------------------------------------------------------
 *
 * Command that adds a dimension leader to the current pattern.
 */

#include "../core.h"

int
dim_leader_command(State *state)
{
    EmbVector start = emb_vector(8.0f, 1.0f);
    EmbVector end = emb_vector(1.0f, 3.0f);
    EmbReal rot = 10.0;
    int rubberMode = OBJ_RUBBER_OFF;
    return dim_leader_create(start, end, rot, rubberMode);
}

#if 0
-- Command: QuickLeader

var global = {}-- -- Required
global.x1;
global.y1;
global.x2;
global.y2;

-- TODO: Adding the text is not complete yet.

-- NOTE: main() is run every time the command is started.
--       Use it to reset variables so they are ready to go.
function main()
{
    initCommand();
    clearSelection();
    global.x1 = NaN;
    global.y1 = NaN;
    global.x2 = NaN;
    global.y2 = NaN;
    setPromptPrefix(qsTr("Specify first point: "));
}

-- NOTE: click() is run only for left clicks.
--       Middle clicks are used for panning.
--       Right clicks bring up the context menu.
function click(x, y)
{
    if(isNaN(global.x1))
    {
        global.x1 = x;
        global.y1 = y;
        addRubber("DIMLEADER");
        setRubberMode("DIMLEADER_LINE");
        setRubberPoint("DIMLEADER_LINE_START", global.x1, global.y1);
        appendPromptHistory();
        setPromptPrefix(qsTr("Specify second point: "));
    }
    else
    {
        global.x2 = x;
        global.y2 = y;
        setRubberPoint("DIMLEADER_LINE_END", global.x2, global.y2);
        vulcanize();
        appendPromptHistory();
        endCommand();
    }
}

-- NOTE: context() is run when a context menu entry is chosen.
function context(str)
{
    todo("QUICKLEADER", "context()");
}

-- NOTE: prompt() is run when Enter is pressed.
--       appendPromptHistory is automatically called before prompt()
--       is called so calling it is only needed for erroneous input.
--       Any text in the command prompt is sent as an uppercase string.
function prompt(str)
{
    var strList = str.split(",");
    if(isNaN(global.x1))
    {
        if(isNaN(strList[0]) || isNaN(strList[1]))
        {
            alert(qsTr("Requires two points."));
            setPromptPrefix(qsTr("Specify first point: "));
        }
        else
        {
            global.x1 = Number(strList[0]);
            global.y1 = Number(strList[1]);
            addRubber("DIMLEADER");
            setRubberMode("DIMLEADER_LINE");
            setRubberPoint("DIMLEADER_LINE_START", global.x1, global.y1);
            setPromptPrefix(qsTr("Specify second point: "));
        }
    }
    else
        if(isNaN(strList[0]) || isNaN(strList[1])) then
            alert(qsTr("Requires two points."));
            setPromptPrefix(qsTr("Specify second point: "));
        else
            global.x2 = Number(strList[0]);
            global.y2 = Number(strList[1]);
            setRubberPoint("DIMLEADER_LINE_END", global.x2, global.y2);
            vulcanize();
            endCommand();
        }
    }
}
#endif

