/*
 * Embroidermodder 2
 * Copyright 2011-2025 The Embroidermodder Team
 *
 * Embroidermodder 2 is free and open software under the zlib license:
 * see LICENSE.md for details.
 *
 * ----------------------------------------------------------------------------
 *
 * Command that adds a horizontal dimension to the current pattern.
 */

#include "../core.h"

int
horizontal_dim_command(State *state)
{
    EmbVector start = emb_vector(8.0f, 12.0f);
    EmbVector end = emb_vector(18.0f, 11.0f);
    EmbReal legHeight = 10.0f;
    return horizontal_dim_c(start, end, legHeight);
}
