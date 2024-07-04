/*
 * Embroidermodder 2.
 *
 * Copyright 2011-2024 The Embroidermodder Team
 * Embroidermodder 2 is Open Source Software, see LICENSE.md for licensing terms.
 * Visit https://www.libembroidery.org/refman for advice on altering this file,
 * or read the markdown version in embroidermodder2/docs/refman.
 *
 * Toolbar Data
 */

#include "core.h"

string_table toolbars_when_docs = {
    "View",
    "Zoom",
    "Pan",
    "Icon",
    "Help",
    "Layer",
    "Text",
    "Properties",
    "Draw",
    "END"
};

string_table file_toolbar = {
    "new",
    "open",
    "save",
    "saveas",
    "print",
    "designdetails",
    "---",
    "undo",
    "redo",
    "---",
    "help",
    "END"
};

string_table edit_toolbar = {
    "cut",
    "copy",
    "paste",
    "END"
};

string_table view_toolbar = {
    "day",
    "night",
    "END"
};

string_table zoom_toolbar = {
    "zoomwindow",
    "zoomdynamic",
    "zoomscale",
    "---",
    "zoomcenter",
    "zoomin",
    "zoomout",
    "---",
    "zoomselected",
    "zoomall",
    "zoomextents",
    "END"
};

string_table pan_toolbar = {
    "panrealtime",
    "panpoint",
    "---",
    "panleft",
    "panright",
    "panup",
    "pandown",
    "END"
};

string_table icon_toolbar = {
    "icon16",
    "icon24",
    "icon32",
    "icon48",
    "icon64",
    "icon128",
    "END"
};

string_table help_toolbar = {
    "help",
    "---",
    "changelog",
    "---",
    "about",
    "---",
    "whatsthis",
    "END"
};

string_table draw_toolbar = {
    "path",
    "polyline",
    "---",
    "arc",
    "circle",
    "ellipse",
    "line",
    "point",
    "polygon",
    "rectangle",
    "singlelinetext",
    "---",
    "dolphin",
    "heart",
    "snowflake",
    "star",
    "END"
};

string_table inquiry_toolbar = {
    "distance",
    "locatepoint",
    "END"
};

string_table modify_toolbar = {
    "erase",
    "move",
    "rotate",
    "scale",
    "selectall",
    "END"
};

string_table dimension_toolbar = {
    "quickleader",
    "END"
};

string_table sandbox_toolbar = {
    "sandbox",
    "END"
};
