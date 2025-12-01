/*
 * Embroidermodder 2
 * Copyright 2011-2025 The Embroidermodder Team
 *
 * Embroidermodder 2 is free and open software under the zlib license:
 * see LICENSE.md for details.
 *
 * ----------------------------------------------------------------------------
 *
 * Command that pauses for a set amount of milliseconds so scripts run at a
 * speed that can be watched.
 */

#include "../core.h"

int
sleep_command(State *state)
{
    return sleep_c();
}

