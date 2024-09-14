/*
 * Embroidermodder 2.
 *
 * Copyright 2011-2024 The Embroidermodder Team
 * Embroidermodder 2 is Open Source Software, see LICENSE.md for licensing terms.
 *
 * Read the reference manual (https://www.libembroidery.org/downloads/emrm.pdf)
 * for advice on altering this file.
 *
 * Selectors
 */

#include "../core.h"

EmbStringTable layer_list = {
    "linetypebylayer", "0", "",
    "linetypebylayer", "1", "",
    "linetypebylayer", "2", "",
    "linetypebylayer", "3", "",
    "linetypebylayer", "4", "",
    "linetypebylayer", "5", "",
    "linetypebylayer", "6", "",
    "linetypebylayer", "7", "",
    "linetypebylayer", "8", "",
    "linetypebylayer", "9", "",
    "END", "END", "END"
};

EmbStringTable color_list = {
    "colorbylayer", "ByLayer", "",
    "colorbyblock", "ByBlock", "",
    "colorred", "Red", "255,0,0",
    "coloryellow", "Yellow", "255,255,0",
    "colorgreen", "Green", "0,255,0",
    "colorcyan", "Cyan", "0,255,255",
    "colorblue", "Blue", "0,0,255",
    "colormagenta", "Magenta", "255,0,255",
    "colorwhite", "White", "255,255,255",
    "colorother", "Other...", "",
    "END", "END", "END"
};

EmbStringTable line_type_list = {
    "linetypebylayer", "ByLayer", "",
    "linetypebyblock", "ByBlock", "",
    "linetypecontinuous", "Continuous", "",
    "linetypehidden", "Hidden", "",
    "linetypecenter", "Center", "",
    "linetypeother", "Other...", "",
    "END", "END", "END"
};

/* TODO: Thread weight is weird. See http://en.wikipedia.org/wiki/Thread_(yarn)#Weight */
EmbStringTable line_weight_list = {
    "lineweightbylayer", "ByLayer", "-2.00",
    "lineweightbyblock", "ByBlock", "-1.00",
    "lineweightdefault", "Default", "0.00",
    "lineweight01", "0.00 mm", "0.00",
    "lineweight02", "0.05 mm", "0.05",
    "lineweight03", "0.15 mm", "0.15",
    "lineweight04", "0.20 mm", "0.20",
    "lineweight05", "0.25 mm", "0.25",
    "lineweight06", "0.30 mm", "0.30",
    "lineweight07", "0.35 mm", "0.35",
    "lineweight08", "0.40 mm", "0.40",
    "lineweight09", "0.45 mm", "0.45",
    "lineweight10", "0.50 mm", "0.50",
    "lineweight11", "0.55 mm", "0.55",
    "lineweight12", "0.60 mm", "0.60",
    "lineweight13", "0.65 mm", "0.65",
    "lineweight14", "0.70 mm", "0.70",
    "lineweight15", "0.75 mm", "0.75",
    "lineweight16", "0.80 mm", "0.80",
    "lineweight17", "0.85 mm", "0.85",
    "lineweight18", "0.90 mm", "0.90",
    "lineweight19", "0.95 mm", "0.95",
    "lineweight20", "1.00 mm", "1.00",
    "lineweight21", "1.05 mm", "1.05",
    "lineweight22", "1.10 mm", "1.10",
    "lineweight23", "1.15 mm", "1.15",
    "lineweight24", "1.20 mm", "1.20",
    "END", "END", "END"
};

EmbStringTable text_size_list = {
    "6 pt", "6", "",
    "8 pt", "8", "",
    "9 pt", "9", "",
    "10 pt", "10", "",
    "11 pt", "11", "",
    "12 pt", "12", "",
    "14 pt", "14", "",
    "18 pt", "18", "",
    "24 pt", "24", "",
    "30 pt", "30", "",
    "36 pt", "36", "",
    "48 pt", "48", "",
    "60 pt", "60", "",
    "72 pt", "72", "",
    "END", "END", "END"
};

