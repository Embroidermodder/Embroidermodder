/*
 * Embroidermodder 2
 * Copyright 2011-2025 The Embroidermodder Team
 *
 * Embroidermodder 2 is free and open software under the zlib license:
 * see LICENSE.md for details.
 *
 * ----------------------------------------------------------------------------
 *
 * Command that adds an image to the current view: but not the current
 * pattern for drawing over.
 */

#include "../core.h"

int
image_command(State *state)
{
    const char *img = "icons/default/app.png";
    double x = 10.0;
    double y = 10.0;
    double w = 30.0;
    double h = 20.0;
    double rot = 10.0;
    return image_create(img, x, y, w, h, rot);
}

