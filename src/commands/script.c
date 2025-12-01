/*
 * Embroidermodder 2
 * Copyright 2011-2025 The Embroidermodder Team
 *
 * Embroidermodder 2 is free and open software under the zlib license:
 * see LICENSE.md for details.
 *
 * ----------------------------------------------------------------------------
 *
 * Command that adds runs a script based on the filename.
 */

#include "../core.h"

int
script_command(State *state)
{
    if (state->argument_count < 1) {
        return 1;
    }
    //FIXME: script(state->arguments[0]);
    return 0;
}

