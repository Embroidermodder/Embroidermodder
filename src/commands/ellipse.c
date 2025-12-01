/*
 * Embroidermodder 2
 * Copyright 2011-2025 The Embroidermodder Team
 *
 * Embroidermodder 2 is free and open software under the zlib license:
 * see LICENSE.md for details.
 *
 * ----------------------------------------------------------------------------
 *
 * Command that clears the rubber points without changing the selection.
 */

#include "../core.h"

int
ellipse_command(State *state)
{
    EmbVector center = emb_vector(0.0f, 0.0f);
    EmbReal width = 10.0;
    EmbReal height = 30.0;
    EmbReal rot = 1.0;
    bool fill = false;
    int rubberMode = OBJ_RUBBER_OFF;
    return ellipse_c(center, width, height, rot, fill, rubberMode);
}

