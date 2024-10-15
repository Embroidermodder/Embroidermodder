/*
 * Embroidermodder 2.
 *
 * Copyright 2011-2024 The Embroidermodder Team
 * Embroidermodder 2 is Open Source Software, see LICENSE.md for licensing terms.
 *
 * Read the reference manual (https://www.libembroidery.org/downloads/emrm.pdf)
 * for advice on altering this file.
 *
 * String Data
 */

#include "core.h"

State state = {

/* TODO: find a source for compiler flags for operating system detection.
 */
#if defined(_WIN32)
    .os = "Windows",
#elif defined(__CYGWIN__)
    .os = "Cygwin",
#elif defined(__linux__)
    .os = "Linux",
#elif defined(__unix__)
    .os = "Unix",
#elif defined(__APPLE__) || defined(__MACH__)
    .os = "Mac OS",
#elif defined(__FREEBSD__)
    .os = "FreeBSD",
#elif defined(__ANDROID__)
    .os = "Android",
#else
    .os = "Unknown Operating System",
#endif

    /* -------------------------- Versions ------------------------------ */

    .embroidermodder_version = "2.0.0-alpha4",
    .libembroidery_version = "1.0.0-alpha",
    .EmbroideryMobile_version = "1.0.0-alpha",
    .PET_version = "1.0.0-alpha",

    /* ---------------------------- Paths ------------------------------ */

    .circle_origin_path = "M 0.0 1.0 " \
        "A -1.0 -1.0 2.0 2.0 1 90.0 360.0 " \
        "A -1.0 -1.0 2.0 2.0 1 90.0 -360.0 " \
        "L 0.0 -1.0 " \
        "A -1.0 -1.0 2.0 2.0 1 270.0 90.0 " \
        "L -1.0 0.0 " \
        "A -1.0 -1.0 2.0 2.0 1 180.0 -90.0 ",
//      "Z"

    .one_path = "m 0.05 0.00 " \
        "l 0.45 0.00 " \
        "m 0.00 -0.75 " \
        "m 0.25 -1.00 " \
        "l 0.25 0.00",

    .two_path = "m 0.00 -0.75 " \
        "a 0.00 -1.00 0.50 0.50 1 180.00 -216.87 " \
        "l 0.00 0.00 " \
        "l 0.50 0.00",

    .three_path = "arc m 0.00 -0.50 0.50 0.50 195.00 " \
        "a 0.00 -0.50 0.50 0.50 195.00, 255.00 " \
        "a 0.00 -1.00 0.50 0.50 270.00, 255.00",

    .four_path = "m 0.50 -0.00 " \
        "l 0.50 -1.00 " \
        "l 0.00 -0.50 " \
        "l 0.50 -0.50",

    .five_path = "m 0.50 -1.00 " \
        "l 0.00 -1.00 " \
        "l 0.00 -0.50 " \
        "l 0.25 -0.50 " \
        "a 0.00 -0.50 0.50 0.50 90.00 -180.00 " \
        "l 0.00 0.00",

    .six_path = "E 0.25 -0.25 0.25 0.25 " \
        " m 0.00 -0.25 " \
        "l 0.00 -0.75 " \
        "a 0.00 -1.00 0.50 0.50 180.00, -140.00",

    .seven_path = "m 0.00 -1.00 " \
        "l 0.50 -1.00 " \
        "l 0.25 -0.25 " \
        "l 0.25 -0.00",

    .eight_path = "E 0.25 -0.25 0.25 0.25 " \
        "E 0.25 -0.75 0.25 0.25",

    .nine_path = "E 0.25 -0.75 0.25 0.25 " \
        "m 0.50 -0.75 " \
        "l 0.50 -0.25 " \
        "a 0.00 -0.50 0.50 0.50 0.00, -140.00",

    .zero_path = "m 0.00 -0.75 " \
        "l 0.00 -0.25 " \
        "a 0.00 -0.50 0.50 0.50 180.00, 180.00 " \
        "l 0.50 -0.75 " \
        "a 0.00 -1.00 0.50 0.50 0.00, 180.00",

    .minus_path = "m 0.00 -0.50 " \
        "l 0.50 -0.50",

    .apostrophe_path = "m 0.25 -1.00 " \
        "l 0.25 -0.75",

    .quote_path = "m 0.10 -1.00 " \
        "l 0.10 -0.75 " \
        "m 0.40 -1.00 " \
        "l 0.40 -0.75"

};

