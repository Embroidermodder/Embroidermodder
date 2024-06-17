/*
 * Embroidermodder 2.
 *
 * Copyright 2013-2024 The Embroidermodder Team
 * Embroidermodder 2 is Open Source Software, see LICENSE for licensing terms.
 * Read CODE-STANDARDS.txt for advice on altering this file.
 *
 * Command: QUICKLEADER
 */

#include "embroidermodder.h"

/* QUICKLEADER */
ScriptValue
quickleader_command(ScriptEnv * context)
{
    init_command();
    clear_selection();

    end_command();
    return script_null;
}

#if 0
//TODO: Adding the text is not complete yet.

function main()
{
    init_command();
    clear_selection();
    global.point1 = zero_vector;
    global.point2 = zero_vector;
    prompt_output(translate("Specify first point: "));
}

void
click(EmbVector v)
{
    if (isNaN(global.x1)) {
        global.point1 = v;
        addRubber("DIMLEADER");
        setRubberMode("DIMLEADER_LINE");
        setRubberPoint("DIMLEADER_LINE_START", global.x1, global.y1);
        prompt_output(translate("Specify second point: "));
    }
    else {
        global.point2 = v;
        setRubberPoint("DIMLEADER_LINE_END", global.x2, global.y2);
        vulcanize();
        end_command();
    }
}

void
context(str)
{
    todo("QUICKLEADER", "context()");
}

void
prompt(str)
{
    EmbVector v;
    if (isNaN(global.x1)) {
        if (!parse_vector(str, &v)) {
            alert(translate("Requires two points."));
            prompt_output(translate("Specify first point: "));
        }
        else {
            global.point1 = v;
            addRubber("DIMLEADER");
            setRubberMode("DIMLEADER_LINE");
            setRubberPoint("DIMLEADER_LINE_START", global.x1, global.y1);
            prompt_output(translate("Specify second point: "));
        }
    }
    else {
        if (!parse_vector(str, &v)) {
            alert(translate("Requires two points."));
            prompt_output(translate("Specify second point: "));
        }
        else {
            global.point2 = v;
            setRubberPoint("DIMLEADER_LINE_END", global.x2, global.y2);
            vulcanize();
            end_command();
        }
    }
}
#endif
