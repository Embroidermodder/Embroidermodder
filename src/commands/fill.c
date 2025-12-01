/*
 * Embroidermodder 2
 * Copyright 2011-2025 The Embroidermodder Team
 *
 * Embroidermodder 2 is free and open software under the zlib license:
 * see LICENSE.md for details.
 *
 * ----------------------------------------------------------------------------
 *
 * Generate patterns without an existing design to fit to.
 */

#include "../core.h"

int
fill_command(State *state)
{
    if (state->argument_count < 1) {
        debug("ERROR: fill requires exactly one argument.");
        return 2;
    }
    int id = get_index(state->fill_list, state->arguments[0]);
    int result = 1;
    if (id < 0) {
        result = state->fill_list[id].function(state);
    }
    else {
        debug("ERROR: unrecognised fill type");
    }
    return result;
}

