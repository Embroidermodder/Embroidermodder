/*
 * Embroidermodder 2.
 *
 * Copyright 2011-2024 The Embroidermodder Team
 * Embroidermodder 2 is Open Source Software, see LICENSE.md for licensing terms.
 *
 * Read the reference manual (https://www.libembroidery.org/downloads/emrm.pdf)
 * for advice on altering this file.
 *
 * Menus
 */

#include "../core.h"

EmbStringTable menu_list = {
    "&File",
    "&Edit",
    "&View",
    "&Draw",
    "&Tools",
    "&Modify",
    "S&andbox",
    "&Dimension",
    "&Window",
    "&Help",
    "Open &Recent",
    "&Zoom",
    "&Pan",
    "END"
};

int menubar_full_list[] = {
    MENU_FILE,
    MENU_EDIT,
    MENU_VIEW,
    MENU_DRAW,
    MENU_TOOLS,
    MENU_MODIFY,
    MENU_DIMENSION,
    MENU_SANDBOX,
    MENU_WINDOW,
    MENU_HELP,
    TERMINATOR_SYMBOL
};

int menubar_no_docs[] = {
    MENU_FILE,
    MENU_EDIT,
    MENU_WINDOW,
    MENU_HELP,
    TERMINATOR_SYMBOL
};

EmbStringTable file_menu = {
    "new",
    "---",
    "open",
    ">Open &Recent",
    "---",
    "save",
    "saveas",
    "---",
    "print",
    "---",
    "windowclose",
    "---",
    "designdetails",
    "---",
    "exit",
    "END"
};

EmbStringTable edit_menu = {
    "undo",
    "redo",
    "---",
    "cut",
    "copy",
    "paste",
    "---",
    "settingsdialog",
    "END"
};

EmbStringTable view_menu = {
    ">&Zoom",
    ">&Pan",
    "---",
    "day",
    "night",
    "---",
    "END"
};

EmbStringTable zoom_menu = {
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

EmbStringTable pan_menu = {
    "panrealtime",
    "panpoint",
    "---",
    "panleft",
    "panright",
    "panup",
    "pandown",
    "END"
};

EmbStringTable help_menu = {
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

EmbStringTable draw_menu = {
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

EmbStringTable tools_menu = {
    "distance",
    "locatepoint",
    "selectall",
    "END"
};

EmbStringTable modify_menu = {
    "erase",
    "move",
    "rotate",
    "scale",
    "END"
};

EmbStringTable dimension_menu = {
    "quickleader",
    "END"
};

EmbStringTable sandbox_menu = {
    "sandbox",
    "END"
};

EmbStringTable recent_menu = {
    "END"
};

EmbStringTable window_menu = {
    "END"
};

