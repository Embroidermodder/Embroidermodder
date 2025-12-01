/*
 * Embroidermodder 2
 * Copyright 2011-2025 The Embroidermodder Team
 *
 * Embroidermodder 2 is free and open software under the zlib license:
 * see LICENSE.md for details.
 *
 * ----------------------------------------------------------------------------
 *
 * Command that opens a new empty pattern.
 */

#include "../core.h"

int
new_command(State *state)
{
    no_arguments(state);
    return new_file_c();
}
