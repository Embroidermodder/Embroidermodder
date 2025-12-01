/*
 * Embroidermodder 2
 * Copyright 2011-2025 The Embroidermodder Team
 *
 * Embroidermodder 2 is free and open software under the zlib license:
 * see LICENSE.md for details.
 *
 * ----------------------------------------------------------------------------
 *
 * Command that opens a new design.
 */

#include "../core.h"

int
open_command(State *state)
{
    int error = 0;
    if (state->argument_count == 0) {
        return open_c("");
    }
    for (int i=0; i<state->argument_count; i++) {
        if (open_c(state->arguments[i])) {
            debug("ERROR: Failed to open %s.", state->arguments[i]);
            error++;
        }
    }
    return error;
}

