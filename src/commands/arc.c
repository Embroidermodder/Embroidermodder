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
arc_command(State *state)
{
    EmbVector start = emb_vector(0.0f, 0.0f);
    EmbVector mid = emb_vector(10.0f, 0.0f);
    EmbVector end = emb_vector(10.0f, 10.0f);
    int32_t rubber_mode = OBJ_RUBBER_OFF;
    return arc_c(start, mid, end, rubber_mode);
}

