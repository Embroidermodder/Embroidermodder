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
    return line_c(start, end, rot, rubberMode);
}

