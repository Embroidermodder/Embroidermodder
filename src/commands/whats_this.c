/*
 * Embroidermodder 2
 * Copyright 2011-2025 The Embroidermodder Team
 *
 * Embroidermodder 2 is free and open software under the zlib license:
 * see LICENSE.md for details.
 *
 * ----------------------------------------------------------------------------
 *
 * Command that enters "what's this" mode.
 */

#include "../core.h"

int
whats_this_command(State *state)
{
    no_arguments(state);
    whats_this_c();
    return 0;
}

