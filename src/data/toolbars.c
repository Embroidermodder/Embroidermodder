/*
 * Embroidermodder 2.
 *
 * Copyright 2011-2024 The Embroidermodder Team
 * Embroidermodder 2 is Open Source Software, see LICENSE.md for licensing terms.
 *
 * Read the reference manual (https://www.libembroidery.org/downloads/emrm.pdf)
 * for advice on altering this file.
 *
 * Toolbars
 */

#include "constants.h"

char *toolbar_list[] = {
    "File",
    "Edit",
    "View",
    "Zoom",
    "Pan",
    "Icon",
    "Help",
    "Layer",
    "Properties",
    "Text",
    "Command Prompt",
    "Draw",
    "Modify",
    "Inquiry",
    "Dimension",
    "Sandbox",
    "END"
};

int toolbars_when_docs[] = {
    TOOLBAR_VIEW,
    TOOLBAR_ZOOM,
    TOOLBAR_PAN,
    TOOLBAR_ICON,
    TOOLBAR_HELP,
    TOOLBAR_LAYER,
    TOOLBAR_TEXT,
    TOOLBAR_PROPERTIES,
    TOOLBAR_DRAW,
    TERMINATOR_SYMBOL
};

char *file_toolbar[] = {
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

char *edit_toolbar[] = {
    "cut",
    "copy",
    "paste",
    "END"
};

char *view_toolbar[] = {
    "day",
    "night",
    "END"
};

char *zoom_toolbar[] = {
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

char *pan_toolbar[] = {
    "panrealtime",
    "panpoint",
    "---",
    "panleft",
    "panright",
    "panup",
    "pandown",
    "END"
};

char *icon_toolbar[] = {
    "icon16",
    "icon24",
    "icon32",
    "icon48",
    "icon64",
    "icon128",
    "END"
};

char *help_toolbar[] = {
    "help",
    "---",
    "changelog",
    "---",
    "about",
    "---",
    "whatsthis",
    "END"
};

char *draw_toolbar[] = {
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

char *inquiry_toolbar[] = {
    "distance",
    "locatepoint",
    "END"
};

char *modify_toolbar[] = {
    "erase",
    "move",
    "rotate",
    "scale",
    "selectall",
    "END"
};

char *dimension_toolbar[] = {
    "quickleader",
    "END"
};

char *sandbox_toolbar[] = {
    "sandbox",
    "END"
};

char *layer_toolbar[] = {
    "END"
};

char *properties_toolbar[] = {
    "END"
};

char *text_toolbar[] = {
    "END"
};

char *prompt_toolbar[] = {
    "END"
};

/* Matches the order of toolbar_list. */
char **toolbar_data[] = {
    file_toolbar,
    edit_toolbar,
    view_toolbar,
    zoom_toolbar,
    pan_toolbar,
    icon_toolbar,
    help_toolbar,
    layer_toolbar,
    properties_toolbar,
    text_toolbar,
    prompt_toolbar,
    draw_toolbar,
    modify_toolbar,
    inquiry_toolbar,
    dimension_toolbar,
    sandbox_toolbar
};

