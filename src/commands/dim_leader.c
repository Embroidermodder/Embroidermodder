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
    return dim_leader_c(start, end, rot, rubberMode);
}
