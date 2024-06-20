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

const char *menubar_full_list[] = {
    "File",
    "Edit",
    "View",
    "Draw",
    "Tools",
    "Modify",
    "Dimension",
    "Sandbox",
    "Settings",
    "Window",
    "Help",
    "END"
};

const char *menubar_no_docs[] = {
    "File",
    "Edit",
    "Settings",
    "Window",
    "Help",
    "END"
};

const char *file_menu[] = {
    "END"
};

const char *edit_menu[] = {
    "undo",
    "redo",
    "---",
    "cut",
    "copy",
    "paste",
    "---",
    "END"
};

const char *view_menu[] = {
    ">Zoom",
    ">Pan",
    "---",
    "day",
    "night",
    "---",
    "END"
};

const char *zoom_menu[] = {
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

const char *pan_menu[] = {
    "panrealtime",
    "panpoint",
    "---",
    "panleft",
    "panright",
    "panup",
    "pandown",
    "END"
};

const char *help_menu[] = {
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

const char *draw_menu[] = {
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

const char *tools_menu[] = {
    "distance",
    "locatepoint",
    "selectall",
    "END"
};

const char *modify_menu[] = {
    "erase",
    "move",
    "rotate",
    "scale",
    "END"
};

const char *dimension_menu[] = {
    "quickleader",
    "END"
};

const char *sandbox_menu[] = {
    "sandbox",
    "END"
};
