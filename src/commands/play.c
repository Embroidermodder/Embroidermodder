/*
 * Embroidermodder 2
 * Copyright 2011-2025 The Embroidermodder Team
 *
 * Embroidermodder 2 is free and open software under the zlib license:
 * see LICENSE.md for details.
 *
 * ----------------------------------------------------------------------------
 *
 * Command that starts animating the current design.
 */

#include "../core.h"

int
play_command(State *state)
{
    no_arguments(state);
    state->play_mode = 1;
    state->simulation_start = current_time();
    return 0;
}

