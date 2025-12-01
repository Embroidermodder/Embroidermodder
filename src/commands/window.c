/*
 * Embroidermodder 2
 * Copyright 2011-2025 The Embroidermodder Team
 *
 * Embroidermodder 2 is free and open software under the zlib license:
 * see LICENSE.md for details.
 *
 * ----------------------------------------------------------------------------
 *
 * Command that alters the arrangement of MDI windows in the MDI area.
 */

#include "../core.h"

int
window_command(State *state)
{
    if (state->argument_count < 1) {
        debug("ERROR: window expects exactly one argument and none were supplied.");
        return 1;
    }
    return window_c(state->arguments[0]);
}

