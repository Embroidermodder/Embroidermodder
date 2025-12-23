/*
 * Embroidermodder 2
 * Copyright 2011-2025 The Embroidermodder Team
 *
 * Embroidermodder 2 is free and open software under the zlib license:
 * see LICENSE.md for details.
 *
 * ----------------------------------------------------------------------------
 *
 * Command that shows the changelog dialog.
 */

#include "../core.h"

int
changelog_command(State *state)
{
    return changelog_dialog();
}

