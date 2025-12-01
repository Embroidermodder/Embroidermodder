/*
 * Embroidermodder 2
 * Copyright 2011-2025 The Embroidermodder Team
 *
 * Embroidermodder 2 is free and open software under the zlib license:
 * see LICENSE.md for details.
 *
 * ----------------------------------------------------------------------------
 *
 * The view zoom commands.
 */

#include "../core.h"

int
zoom_command(State *state)
{
    if (state->argument_count < 1) {
        debug("ERROR: zoom expects exactly one argument and none were supplied.");
        return 1;
    }
    return zoom_c(state->arguments[0]);
}

