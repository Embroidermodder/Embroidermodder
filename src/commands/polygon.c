/*
 * Embroidermodder 2
 * Copyright 2011-2025 The Embroidermodder Team
 *
 * Embroidermodder 2 is free and open software under the zlib license:
 * see LICENSE.md for details.
 *
 * ----------------------------------------------------------------------------
 *
 * Command that adds a polygon to the current pattern.
 */

#include "../core.h"

#define POLYGON_NUM_SIDES      0
#define POLYGON_CENTER_PT      1
#define POLYGON_POLYTYPE       2
#define POLYGON_INSCRIBE       3
#define POLYGON_CIRCUMSCRIBE   4
#define POLYGON_DISTANCE       5
#define POLYGON_SIDE_LEN       6

int
polygon_command(State *state)
{
    //FIXME: polygon_function();
    return 0;
}

#if 0
-- Command: Polygon

var global = {}-- -- Required
state.center;
state.side1;
state.side2;
state.pointI;
state.pointC;
state.polyType = "Inscribed"-- -- Default
state.numSides = 4--           -- Default
state.mode;

-- enums

-- NOTE: main() is run every time the command is started.
--       Use it to reset variables so they are ready to go.
function main()
{
    initCommand();
    clearSelection();
    state.center = emb_vector(NaN, NaN);
    state.side1 = emb_vector(NaN, NaN);
    state.side2 = emb_vector(NaN, NaN);
    state.pointI = emb_vector(NaN, NaN);
    state.pointC = emb_vector(NaN, NaN);
    state.mode = POLYGON_NUM_SIDES;
    setPromptPrefix(qsTr("Enter number of sides") + " {" + state.numSides.toString() + "}: ");
}

-- NOTE: click() is run only for left clicks.
--       Middle clicks are used for panning.
--       Right clicks bring up the context menu.
function click(x, y)
{
    if (state.mode == POLYGON_NUM_SIDES) {
        -- Do nothing, the prompt controls this.
    }
    else if (state.mode == POLYGON_CENTER_PT) {
        state.centerX = x;
        state.centerY = y;
        state.mode = POLYGON_POLYTYPE;
        appendPromptHistory();
        setPromptPrefix(qsTr("Specify polygon type [Inscribed in circle/Circumscribed around circle]") + " {" + state.polyType + "}: ");
    }
    else if (state.mode == POLYGON_POLYTYPE) {
        -- Do nothing, the prompt controls this.
    }
    else if (state.mode == POLYGON_INSCRIBE) {
        state.pointIX = x;
        state.pointIY = y;
        setRubberPoint("POLYGON_INSCRIBE_POINT", state.pointIX, state.pointIY);
        vulcanize();
        appendPromptHistory();
        endCommand();
    }
    else if (state.mode == POLYGON_CIRCUMSCRIBE) {
        state.pointCX = x;
        state.pointCY = y;
        setRubberPoint("POLYGON_CIRCUMSCRIBE_POINT", state.pointCX, state.pointCY);
        vulcanize();
        appendPromptHistory();
        endCommand();
    }
    else if (state.mode == POLYGON_DISTANCE) {
        -- Do nothing, the prompt controls this.
    }
    else if (state.mode == POLYGON_SIDE_LEN) {
        todo("POLYGON", "Sidelength mode");
    }
}

-- NOTE: context() is run when a context menu entry is chosen.
function context(str)
{
    todo("POLYGON", "context()");
}

-- NOTE: prompt() is run when Enter is pressed.
--       appendPromptHistory is automatically called before prompt()
--       is called so calling it is only needed for erroneous input.
--       Any text in the command prompt is sent as an uppercase string.
function prompt(str)
{
    if (state.mode == POLYGON_NUM_SIDES) {
        if (str == "" && state.numSides >= 3 && state.numSides <= 1024) {
            setPromptPrefix(qsTr("Specify center point or [Sidelength]: "));
            state.mode = POLYGON_CENTER_PT;
        }
        else {
            var tmp = Number(str);
            if (isNaN(tmp) || !isInt(tmp) || tmp < 3 || tmp > 1024) {
                alert(qsTr("Requires an integer between 3 and 1024."));
                setPromptPrefix(qsTr("Enter number of sides") + " {" + state.numSides.toString() + "}: ");
            }
            else {
                state.numSides = tmp;
                setPromptPrefix(qsTr("Specify center point or [Sidelength]: "));
                state.mode = POLYGON_CENTER_PT;
            }
        }
    }
    else if (state.mode == POLYGON_CENTER_PT) {
        if (str == "S" || str == "SIDELENGTH") {
            state.mode = POLYGON_SIDE_LEN;
            setPromptPrefix(qsTr("Specify start point: "));
        }
        else {
            var strList = str.split(",");
            if (isNaN(strList[0]) || isNaN(strList[1]))
            {
                alert(qsTr("Point or option keyword required."));
                setPromptPrefix(qsTr("Specify center point or [Sidelength]: "));
            }
            else
            {
                state.centerX = Number(strList[0]);
                state.centerY = Number(strList[1]);
                state.mode = POLYGON_POLYTYPE;
                setPromptPrefix(qsTr("Specify polygon type [Inscribed in circle/Circumscribed around circle]") + " {" + state.polyType + "}: ");
            }
        }
    }
    else if (state.mode == POLYGON_POLYTYPE)
    {
        if (str == "I"        ||
           str == "IN"       ||
           str == "INS"      ||
           str == "INSC"     ||
           str == "INSCR"    ||
           str == "INSCRI"   ||
           str == "INSCRIB"  ||
           str == "INSCRIBE" ||
           str == "INSCRIBED") -- TODO: Probably should add additional qsTr calls here.
        {
            state.mode = POLYGON_INSCRIBE;
            state.polyType = "Inscribed";
            setPromptPrefix(qsTr("Specify polygon corner point or [Distance]: "));
            addRubber("POLYGON");
            setRubberMode("POLYGON_INSCRIBE");
            setRubberPoint("POLYGON_CENTER", state.centerX, state.centerY);
            setRubberPoint("POLYGON_NUM_SIDES", state.numSides, 0);
        }
        else if (str == "C"            ||
                str == "CI"           ||
                str == "CIR"          ||
                str == "CIRC"         ||
                str == "CIRCU"        ||
                str == "CIRCUM"       ||
                str == "CIRCUMS"      ||
                str == "CIRCUMSC"     ||
                str == "CIRCUMSCR"    ||
                str == "CIRCUMSCRI"   ||
                str == "CIRCUMSCRIB"  ||
                str == "CIRCUMSCRIBE" ||
                str == "CIRCUMSCRIBED") -- TODO: Probably should add additional qsTr calls here.
        {
            state.mode = POLYGON_CIRCUMSCRIBE;
            state.polyType = "Circumscribed";
            setPromptPrefix(qsTr("Specify polygon side point or [Distance]: "));
            addRubber("POLYGON");
            setRubberMode("POLYGON_CIRCUMSCRIBE");
            setRubberPoint("POLYGON_CENTER", state.centerX, state.centerY);
            setRubberPoint("POLYGON_NUM_SIDES", state.numSides, 0);
        }
        else if (str == "") {
            if (state.polyType == "Inscribed") {
                state.mode = POLYGON_INSCRIBE;
                setPromptPrefix(qsTr("Specify polygon corner point or [Distance]: "));
                addRubber("POLYGON");
                setRubberMode("POLYGON_INSCRIBE");
                setRubberPoint("POLYGON_CENTER", state.centerX, state.centerY);
                setRubberPoint("POLYGON_NUM_SIDES", state.numSides, 0);
            }
            else if (state.polyType == "Circumscribed") {
                state.mode = POLYGON_CIRCUMSCRIBE;
                setPromptPrefix(qsTr("Specify polygon side point or [Distance]: "));
                addRubber("POLYGON");
                setRubberMode("POLYGON_CIRCUMSCRIBE");
                setRubberPoint("POLYGON_CENTER", state.centerX, state.centerY);
                setRubberPoint("POLYGON_NUM_SIDES", state.numSides, 0);
            }
            else {
                error("POLYGON", qsTr("Polygon type is not Inscribed or Circumscribed."));
            }
        }
        else {
            alert(qsTr("Invalid option keyword."));
            setPromptPrefix(qsTr("Specify polygon type [Inscribed in circle/Circumscribed around circle]") + " {" + state.polyType + "}: ");
        }
    }
    else if (state.mode == POLYGON_INSCRIBE)
    {
        if (str == "D" || str == "DISTANCE") -- TODO: Probably should add additional qsTr calls here.
        {
            state.mode = POLYGON_DISTANCE;
            setPromptPrefix(qsTr("Specify distance: "));
        }
        else
        {
            var strList = str.split(",");
            if (isNaN(strList[0]) || isNaN(strList[1]))
            {
                alert(qsTr("Point or option keyword required."));
                setPromptPrefix(qsTr("Specify polygon corner point or [Distance]: "));
            }
            else
            {
                state.pointIX = Number(strList[0]);
                state.pointIY = Number(strList[1]);
                setRubberPoint("POLYGON_INSCRIBE_POINT", state.pointIX, state.pointIY);
                vulcanize();
                endCommand();
            }
        }
    }
    else if (state.mode == POLYGON_CIRCUMSCRIBE)
    {
        if (str == "D" || str == "DISTANCE") -- TODO: Probably should add additional qsTr calls here.
        {
            state.mode = POLYGON_DISTANCE;
            setPromptPrefix(qsTr("Specify distance: "));
        }
        else {
            var strList = str.split(",");
            if (isNaN(strList[0]) || isNaN(strList[1])) {
                alert(qsTr("Point or option keyword required."));
                setPromptPrefix(qsTr("Specify polygon side point or [Distance]: "));
            }
            else {
                state.pointCX = Number(strList[0]);
                state.pointCY = Number(strList[1]);
                setRubberPoint("POLYGON_CIRCUMSCRIBE_POINT", state.pointCX, state.pointCY);
                vulcanize();
                endCommand();
            }
        }
    }
    else if (state.mode == POLYGON_DISTANCE) {
        if (isNaN(str)) {
            alert(qsTr("Requires valid numeric distance."));
            setPromptPrefix(qsTr("Specify distance: "));
        }
        else {
            if (state.polyType == "Inscribed")
            {
                state.pointIX = state.centerX;
                state.pointIY = state.centerY + Number(str);
                setRubberPoint("POLYGON_INSCRIBE_POINT", state.pointIX, state.pointIY);
                vulcanize();
                endCommand();
            }
            else if (state.polyType == "Circumscribed") {
                state.pointCX = state.centerX;
                state.pointCY = state.centerY + Number(str);
                setRubberPoint("POLYGON_CIRCUMSCRIBE_POINT", state.pointCX, state.pointCY);
                vulcanize();
                endCommand();
            }
            else {
                error("POLYGON", qsTr("Polygon type is not Inscribed or Circumscribed."));
            }
        }
    }
    else if (state.mode == POLYGON_SIDE_LEN) {
        todo("POLYGON", "Sidelength mode");
    }
}
#endif

