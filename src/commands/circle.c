/*
 * Embroidermodder 2
 * Copyright 2011-2025 The Embroidermodder Team
 *
 * Embroidermodder 2 is free and open software under the zlib license:
 * see LICENSE.md for details.
 *
 * ----------------------------------------------------------------------------
 *
 * Command that adds a circle to the design.
 */

#include "../core.h"

int
circle_command(State *state)
{
    EmbVector center = emb_vector(0.0f, 0.0f);
    EmbReal radius = 10.0;
    bool fill = false;
    int rubberMode = OBJ_RUBBER_OFF;
    return circle_c(center, radius, fill, rubberMode);
}

