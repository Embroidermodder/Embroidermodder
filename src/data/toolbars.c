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

#include "../core.h"

EmbStringTable toolbar_list = {
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

EmbStringTable file_toolbar = {
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

EmbStringTable edit_toolbar = {
    "cut",
    "copy",
    "paste",
    "END"
};

EmbStringTable view_toolbar = {
    "day",
    "night",
    "END"
};

EmbStringTable zoom_toolbar = {
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

EmbStringTable pan_toolbar = {
    "panrealtime",
    "panpoint",
    "---",
    "panleft",
    "panright",
    "panup",
    "pandown",
    "END"
};

EmbStringTable icon_toolbar = {
    "icon16",
    "icon24",
    "icon32",
    "icon48",
    "icon64",
    "icon128",
    "END"
};

EmbStringTable help_toolbar = {
    "help",
    "---",
    "changelog",
    "---",
    "about",
    "---",
    "whatsthis",
    "END"
};

EmbStringTable draw_toolbar = {
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

EmbStringTable inquiry_toolbar = {
    "distance",
    "locatepoint",
    "END"
};

EmbStringTable modify_toolbar = {
    "erase",
    "move",
    "rotate",
    "scale",
    "selectall",
    "END"
};

EmbStringTable dimension_toolbar = {
    "quickleader",
    "END"
};

EmbStringTable sandbox_toolbar = {
    "sandbox",
    "END"
};

EmbStringTable layer_toolbar = {
    "END"
};

EmbStringTable properties_toolbar = {
    "END"
};

EmbStringTable text_toolbar = {
    "END"
};

EmbStringTable prompt_toolbar = {
    "END"
};

/* Matches the order of toolbar_list. */
EmbStringTable *toolbar_data = {
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

