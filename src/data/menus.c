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

char *menu_list[] = {
    "File", "&File",
    "Edit", "&Edit",
    "View", "&View",
    "Draw", "&Draw",
    "Tools", "&Tools",
    "Modify", "&Modify",
    "Sandbox", "S&andbox",
    "Dimension", "&Dimension",
    "Settings", "&Settings",
    "Window", "&Window",
    "Help", "&Help",
    "Recent", "Open &Recent",
    "Zoom", "&Zoom",
    "Pan", "&Pan",
    "END", "END"
};

char *menubar_full_list[] = {
    "File",
    "Edit",
    "View",
    "Draw",
    "Tools",
    "Modify",
    "Dimension",
    "Sandbox",
    "Window",
    "Help",
    "END"
};

char *menubar_no_docs[] = {
    "File",
    "Edit",
    "Settings",
    "Window",
    "Help",
    "END"
};

char *file_menu[] = {
    "new",
    "---",
    "open",
    ">Recent",
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
    ">Zoom",
    ">Pan",
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

