/*
 * Embroidermodder 2
 * Copyright 2011-2025 The Embroidermodder Team
 *
 * Embroidermodder 2 is free and open software under the zlib license:
 * see LICENSE.md for details.
 *
 * ----------------------------------------------------------------------------
 *
 * Command that shows the about dialog.
 */

#include "../core.h"

int
about_command(State *state)
{
    return about_dialog();
}

