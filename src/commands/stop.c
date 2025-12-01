/*
 * Embroidermodder 2
 * Copyright 2011-2025 The Embroidermodder Team
 *
 * Embroidermodder 2 is free and open software under the zlib license:
 * see LICENSE.md for details.
 *
 * ----------------------------------------------------------------------------
 *
 * Command that stops whatever animation is currently running.
 */

#include "../core.h"

int
stop_command(State *state)
{
    state->play_mode = 0;
    return 0;
}

