/*
 *  Embroidermodder 2.
 *
 *  ------------------------------------------------------------
 *
 *  Copyright 2013-2022 The Embroidermodder Team
 *  Embroidermodder 2 is Open Source Software.
 *  See LICENSE for licensing terms.
 *
 *  ------------------------------------------------------------
 *
 *  Use Python's PEP7 style guide.
 *      https://peps.python.org/pep-0007/
 */

#include "core.h"

/* . */
const char *object_names[] = {
    "Base",
    "Arc",
    "Block",
    "Circle",
    "Aligned Dimension",
    "Angular Dimension",
    "Arclength Dimension",
    "Diameter Dimension",
    "Leader Dimension",
    "Linear Dimension",
    "Ordinate Dimension",
    "Radius Dimension",
    "Ellipse",
    "Image",
    "Infinite Line",
    "Line",
    "Path",
    "Point",
    "Polygon",
    "Polyline",
    "Ray",
    "Rectangle",
    "Multiline Text",
    "Text",
    "Unknown",
    "END"
};

/* . */
const char rectangle_main_script[][MAX_STRING_LENGTH] = {
    "init",
    "clear-selection",
    "newRect = true",
    "real x1 = 0.0f",
    "real y1 = 0.0f",
    "real x2 = 0.0f",
    "real y2 = 0.0f",
    "set-prompt-prefix-tr Specify first corner point or [Chamfer/Fillet]: "
};
