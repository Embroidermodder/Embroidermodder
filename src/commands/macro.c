/*
 * Embroidermodder 2
 * Copyright 2011-2025 The Embroidermodder Team
 *
 * Embroidermodder 2 is free and open software under the zlib license:
 * see LICENSE.md for details.
 *
 * ----------------------------------------------------------------------------
 *
 * Command that runs a macro based on the name it's stored under.
 *
 * Embroidermodder 2 supports up to 50 user created macros which can be bound
 * to keyboard shortcuts.
 */

#include "../core.h"

int
macro_command(State *state)
{
    if (state->argument_count < 1) {
        debug("ERROR: macro expects exactly one command. That is: the name of the macro.");
        return 1;
    }
    debug("TODO: macro support");
    return 0;
}

