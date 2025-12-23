/*
 * Embroidermodder 2
 * Copyright 2011-2025 The Embroidermodder Team
 *
 * Embroidermodder 2 is free and open software under the zlib license:
 * see LICENSE.md for details.
 *
 * ----------------------------------------------------------------------------
 *
 * Command that adds an infinite line to the design.
 *
 * TODO: argument parsing
 */

#include "../core.h"

int
infinite_line_command(State *state)
{
    EmbVector point1 = emb_vector(0.0f, 0.0f);
    EmbVector point2 = emb_vector(0.0f, 10.0f);
    EmbReal rot = 0.0f;
    return infinite_line_create(point1, point2, rot);
}
