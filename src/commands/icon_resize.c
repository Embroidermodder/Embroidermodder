/*
 * Embroidermodder 2
 * Copyright 2011-2025 The Embroidermodder Team
 *
 * Embroidermodder 2 is free and open software under the zlib license:
 * see LICENSE.md for details.
 *
 * ----------------------------------------------------------------------------
 *
 * Command that sets the size of the icons in pixels.
 */

#include <stdlib.h>

#include "../core.h"

int
icon_resize_command(State *state)
{
    if (state->argument_count < 1) {
        debug("ERROR: no size supplied to icon_resize.");
        return 1;
    }
    /* TODO: warn about size not in {16, 24, 32, 48, 64, 128}. */
    int size = atoi(state->arguments[0]);
    return icon_resize_c(size);
}

