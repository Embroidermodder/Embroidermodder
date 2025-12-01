/*
 * Embroidermodder 2
 * Copyright 2011-2025 The Embroidermodder Team
 *
 * Embroidermodder 2 is free and open software under the zlib license:
 * see LICENSE.md for details.
 *
 * ----------------------------------------------------------------------------
 *
 * Command that clears the rubber points without changing the selection.
 */

#include "../core.h"

int
clear_rubber_command(State *state)
{
    return clear_rubber_c();
}

