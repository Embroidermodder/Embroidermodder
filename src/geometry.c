/*
 * Embroidermodder 2.
 *
 * Copyright 2011-2024 The Embroidermodder Team
 * Embroidermodder 2 is Open Source Software, see LICENSE.md for licensing terms.
 *
 * Read the reference manual (https://www.libembroidery.org/downloads/emrm.pdf)
 * for advice on altering this file.
 *
 * Geometry Commands for user interface (internal functions only are in
 * "objects.c")
 */

#include "core.h"

/* (double x1, double y1, double x2, double y2, double x3, double y3, int rubberMode).
 */
ScriptValue
add_arc_command(ScriptEnv *context)
{
    int32_t doc_index = activeDocument();
    if (doc_index < 0) {
        return script_false;
    }
    EmbArc arc;
    arc.start = unpack_vector(context, 0);
    arc.mid = unpack_vector(context, 2);
    arc.end = unpack_vector(context, 4);
    int rubberMode = INT(5);
    uint32_t obj = create_arc(arc, getCurrentColor());
    obj_set_rubber_mode(obj, rubberMode);
    doc_undoable_add_obj(doc_index, obj, rubberMode);
    return script_true;
}

/* add_TextMulti(char *str, double x, double y, double rot, bool fill, int rubberMode).
 */
ScriptValue
add_textmulti_command(ScriptEnv *context)
{
    return script_false;
}

