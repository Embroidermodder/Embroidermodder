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

#include "constants.h"

char *menu_list[] = {
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

char *file_menu[] = {
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

char *edit_menu[] = {
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

char *view_menu[] = {
    ">&Zoom",
    ">&Pan",
    "---",
    "day",
    "night",
    "---",
    "END"
};

char *zoom_menu[] = {
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

char *pan_menu[] = {
    "panrealtime",
    "panpoint",
    "---",
    "panleft",
    "panright",
    "panup",
    "pandown",
    "END"
};

char *help_menu[] = {
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

char *draw_menu[] = {
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

char *tools_menu[] = {
    "distance",
    "locatepoint",
    "selectall",
    "END"
};

char *modify_menu[] = {
    "erase",
    "move",
    "rotate",
    "scale",
    "END"
};

char *dimension_menu[] = {
    "quickleader",
    "END"
};

char *sandbox_menu[] = {
    "sandbox",
    "END"
};

char *recent_menu[] = {
    "END"
};

char *window_menu[] = {
    "END"
};

char **menu_data[] = {
    file_menu,
    edit_menu,
    view_menu,
    draw_menu,
    tools_menu,
    modify_menu,
    dimension_menu,
    sandbox_menu,
    window_menu,
    help_menu,
    recent_menu,
    zoom_menu,
    pan_menu
};

