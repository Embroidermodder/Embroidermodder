/*
 * Embroidermodder 2.
 *
 * Copyright 2011-2024 The Embroidermodder Team
 * Embroidermodder 2 is Open Source Software, see LICENSE.md for licensing terms.
 * Visit https://www.libembroidery.org/refman for advice on altering this file,
 * or read the markdown version in embroidermodder2/docs/refman.
 *
 * Menu Data
 */

#include "core.h"

string_table menubar_full_list;

string_table menubar_no_docs = {
    "File",
    "Edit",
    "Settings",
    "Window",
    "Help",
    "END"
};

string_table file_menu = {
    "END"
};

string_table edit_menu = {
    "undo",
    "redo",
    "---",
    "cut",
    "copy",
    "paste",
    "---",
    "END"
};

string_table view_menu = {
    ">Zoom",
    ">Pan",
    "---",
    "day",
    "night",
    "---",
    "END"
};

string_table zoom_menu = {
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

string_table pan_menu = {
    "panrealtime",
    "panpoint",
    "---",
    "panleft",
    "panright",
    "panup",
    "pandown",
    "END"
};

string_table help_menu = {
    "help",
    "---",
    "changelog",
    "---",
    "tipoftheday",
    "---",
    "about",
    "---",
    "whatsthis",
    "END"
};

string_table draw_menu = {
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

string_table tools_menu = {
    "distance",
    "locatepoint",
    "selectall",
    "END"
};

string_table modify_menu = {
    "erase",
    "move",
    "rotate",
    "scale",
    "END"
};

string_table dimension_menu = {
    "quickleader",
    "END"
};

string_table sandbox_menu = {
    "sandbox",
    "END"
};
