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

/* TODO: find a source for compiler flags for operating system detection.
 */
#if defined(_WIN32)
const char *os = "Windows";
#elif defined(__CYGWIN__)
const char *os = "Cygwin";
#elif defined(__linux__)
const char *os = "Linux";
#elif defined(__unix__)
const char *os = "Unix";
#elif defined(__APPLE__) || defined(__MACH__)
const char *os = "Mac OS";
#elif defined(__FREEBSD__)
const char *os = "FreeBSD";
#elif defined(__ANDROID__)
const char *os = "Android";
#else
const char *os = "Unknown Operating System";
#endif

/* -------------------------------- Paths ---------------------------------- */

const char *circle_origin_path = "M 0.0 1.0 " \
    "A -1.0 -1.0 2.0 2.0 1 90.0 360.0 " \
    "A -1.0 -1.0 2.0 2.0 1 90.0 -360.0 " \
    "L 0.0 -1.0 " \
    "A -1.0 -1.0 2.0 2.0 1 270.0 90.0 " \
    "L -1.0 0.0 " \
    "A -1.0 -1.0 2.0 2.0 1 180.0 -90.0 " \
    "Z";

const char *one_path = "m 0.05 0.00 " \
    "l 0.45 0.00 " \
    "m 0.00 -0.75 " \
    "m 0.25 -1.00 " \
    "l 0.25 0.00";

const char *two_path = "m 0.00 -0.75 " \
    "a 0.00 -1.00 0.50 0.50 1 180.00 -216.87 " \
    "l 0.00 0.00 " \
    "l 0.50 0.00";

const char *three_path = "arc m 0.00 -0.50 0.50 0.50 195.00 " \
    "a 0.00 -0.50 0.50 0.50 195.00, 255.00 " \
    "a 0.00 -1.00 0.50 0.50 270.00, 255.00";

const char *four_path = "m 0.50 -0.00 " \
    "l 0.50 -1.00 " \
    "l 0.00 -0.50 " \
    "l 0.50 -0.50";

const char *five_path = "m 0.50 -1.00 " \
    "l 0.00 -1.00 " \
    "l 0.00 -0.50 " \
    "l 0.25 -0.50 " \
    "a 0.00 -0.50 0.50 0.50 90.00 -180.00 " \
    "l 0.00 0.00";

const char *six_path = "E 0.25 -0.25 0.25 0.25 " \
    " m 0.00 -0.25 " \
    "l 0.00 -0.75 " \
    "a 0.00 -1.00 0.50 0.50 180.00, -140.00";

const char *seven_path = "m 0.00 -1.00 " \
    "l 0.50 -1.00 " \
    "l 0.25 -0.25 " \
    "l 0.25 -0.00";

const char *eight_path = "E 0.25 -0.25 0.25 0.25 " \
    "E 0.25 -0.75 0.25 0.25";

const char *nine_path = "E 0.25 -0.75 0.25 0.25 " \
    "m 0.50 -0.75 " \
    "l 0.50 -0.25 " \
    "a 0.00 -0.50 0.50 0.50 0.00, -140.00";

const char *zero_path = "m 0.00 -0.75 " \
    "l 0.00 -0.25 " \
    "a 0.00 -0.50 0.50 0.50 180.00, 180.00 " \
    "l 0.50 -0.75 " \
    "a 0.00 -1.00 0.50 0.50 0.00, 180.00";

const char *minus_path = "m 0.00 -0.50 " \
    "l 0.50 -0.50";

const char *apostrophe_path = "m 0.25 -1.00 " \
    "l 0.25 -0.75";

const char *quote_path = "m 0.10 -1.00 " \
    "l 0.10 -0.75 " \
    "m 0.40 -1.00 " \
    "l 0.40 -0.75";

/* ------------------------------ String Map ------------------------------- */

typedef struct EmbStringMap__ {
    const char *key;
    const char *value;
} EmbStringMap;

StringMap string_data[] = {
    {
        .key = "embroidermodder_version",
        .value = "2.0.0-alpha4"
    },
    {
        .key = "libembroidery_version",
        .value = "1.0.0-alpha"
    },
    {
        .key = "EmbroideryMobile_version",
        .value = "1.0.0-alpha"
    },
    {
        .key = "PET_version",
        .value = "1.0.0-alpha"
    },
    {
        .key = "os",
        .value = os
    },
    {
        .key = "circle_origin_path",
        .value = circle_origin_path
    },
    {
        .key = "one_path",
        .value = one_path
    },
    {
        .key = "two_path",
        .value = two_path
    },
    {
        .key = "three_path",
        .value = three_path
    },
    {
        .key = "four_path",
        .value = four_path
    },
    {
        .key = "five_path",
        .value = five_path
    },
    {
        .key = "six_path",
        .value = six_path
    },
    {
        .key = "seven_path",
        .value = seven_path
    },
    {
        .key = "eight_path",
        .value = eight_path
    },
    {
        .key = "nine_path",
        .value = nine_path
    },
    {
        .key = "zero_path",
        .value = zero_path
    },
    {
        .key = "minus_path",
        .value = minus_path
    },
    {
        .key = "apostrophe_path",
        .value = apostrophe_path
    },
    {
        .key = "quote_path",
        .value = quote_path
    },
    {
        .key = ".END",
        .value = "END"
    }
};

const char *
get_string(const char *key)
{
    for (int i=0; string_tables[i].label[0] != '.'; i++) {
        if (string_equal(string_tables[i].key, key) {
            return string_tables[i].data;
        }
    }
    return NULL;
}

