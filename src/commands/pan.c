/*
 * Embroidermodder 2
 * Copyright 2011-2025 The Embroidermodder Team
 *
 * Embroidermodder 2 is free and open software under the zlib license:
 * see LICENSE.md for details.
 *
 * ----------------------------------------------------------------------------
 *
 * The view panning command.
 */

#include "../core.h"

int
pan_command(State *state)
{
    if (state->argument_count < 1) {
        debug("ERROR: pan expects exactly one argument but none were supplied.");
        return 1;
    }
    return pan_c(state->arguments[0]);
}
