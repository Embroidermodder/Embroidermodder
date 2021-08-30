/* This file is part of Embroidermodder 2.
 * ------------------------------------------------------------
 * Copyright 2021 The Embroidermodder Team
 * Embroidermodder 2 is Open Source Software.
 * See LICENSE.txt for licensing terms.
 * ------------------------------------------------------------
 * This file is only for data and declarations that
 * are compiled into the source.
 */

#include "embroidermodder.h"

int file_toolbar[] = {
    ACTION_new,
    ACTION_open,
    ACTION_save,
    ACTION_saveas,
    ACTION_print,
    ACTION_designdetails,
    -1,
    ACTION_undo,
    ACTION_redo,
    -1,
    ACTION_help,
    -2
};

int edit_toolbar[] = {
    ACTION_cut,
    ACTION_copy,
    ACTION_paste,
    -2
};

int view_toolbar[] = {
    ACTION_day,
    ACTION_night,
    -2
};

int pan_toolbar[] = {
    ACTION_panrealtime,
    ACTION_panpoint,
    -1,
    ACTION_panleft,
    ACTION_panright,
    ACTION_panup,
    ACTION_pandown,
    -2
};

int icon_toolbar[] = {
    ACTION_icon16,
    ACTION_icon24,
    ACTION_icon32,
    ACTION_icon48,
    ACTION_icon64,
    ACTION_icon128,
    -2
};

int help_toolbar[] = {
    ACTION_help,
    -1,
    ACTION_changelog,
    -1,
    ACTION_about,
    -1,
    ACTION_whatsthis,
    -2
};

int zoom_toolbar[] = {
    ACTION_zoomwindow,
    ACTION_zoomdynamic,
    ACTION_zoomscale,
    -1,
    ACTION_zoomcenter,
    ACTION_zoomin,
    ACTION_zoomout,
    -1,
    ACTION_zoomselected,
    ACTION_zoomall,
    ACTION_zoomextents,
    -2
};

int layer_toolbar[] = {
    -2
};

int text_toolbar[] = {
    -2
};

int properties_toolbar[] = {
    -2
};

int *toolbars[] = {
    file_toolbar,
    edit_toolbar,
    view_toolbar,
    zoom_toolbar,
    pan_toolbar,
    icon_toolbar,
    help_toolbar,
    layer_toolbar,
    text_toolbar,
    properties_toolbar
};


int file_menu[] = {
    ACTION_new,
    ACTION_open,
    ACTION_save,
    ACTION_saveas,
    ACTION_print,
    ACTION_designdetails,
    -1,
    ACTION_undo,
    ACTION_redo,
    -1,
    ACTION_help,
    -2
};

int edit_menu[] = {
    ACTION_cut,
    ACTION_copy,
    ACTION_paste,
    -2
};

int view_menu[] = {
    ACTION_day,
    ACTION_night,
    -2
};

int pan_menu[] = {
    ACTION_panrealtime,
    ACTION_panpoint,
    -1,
    ACTION_panleft,
    ACTION_panright,
    ACTION_panup,
    ACTION_pandown,
    -2
};

int icon_menu[] = {
    ACTION_icon16,
    ACTION_icon24,
    ACTION_icon32,
    ACTION_icon48,
    ACTION_icon64,
    ACTION_icon128,
    -2
};

int help_menu[] = {
    ACTION_help,
    -1,
    ACTION_changelog,
    -1,
    ACTION_tipoftheday,
    -1,
    ACTION_about,
    -1,
    ACTION_whatsthis,
    -2
};

int zoom_menu[] = {
    ACTION_zoomwindow,
    ACTION_zoomdynamic,
    ACTION_zoomscale,
    -1,
    ACTION_zoomcenter,
    ACTION_zoomin,
    ACTION_zoomout,
    -1,
    ACTION_zoomselected,
    ACTION_zoomall,
    ACTION_zoomextents,
    -2
};

int layer_menu[] = {
    -2
};

int text_menu[] = {
    -2
};

int properties_menu[] = {
    -2
};

int *menus[] = {
    file_menu,
    edit_menu,
    view_menu,
    zoom_menu,
    pan_menu,
    icon_menu,
    help_menu,
    layer_menu,
    text_menu,
    properties_menu
};

char *toolbar_label[] = {
    "File",
    "Edit",
    "View",
    "Zoom",
    "Pan",
    "Icon",
    "Help",
    "Layer",
    "Text",
    "Properties"
};

char *menu_label[] = {
    "File",
    "Edit",
    "View",
    "Zoom",
    "Pan",
    "Icon",
    "Help",
    "Layer",
    "Text",
    "Properties"
};

int n_toolbars = 10;
int n_menus = 10;

path_symbol icon_zero[] = {
    /* path.addEllipse(QPointF(x+0.25*xScale, y-0.50*yScale), 0.25*xScale, 0.50*yScale); */
    {PATHS_MOVETO, {0.0, -0.75}},
    {PATHS_LINETO, {0.0, -0.25}},
    {PATHS_ARCTO, {0.0, -0.5, 0.5, 0.5, 180.0, 180.0}},
    {PATHS_LINETO, {0.5, -0.75}},
    {PATHS_ARCTO, {0.0, -1.0, 0.5, 0.5, 0.0, 180.0}},
    {PATHS_END, {0.0, 0.0}}
};

path_symbol icon_one[] = {
    {PATHS_MOVETO, {0.05, 0.00}},
    {PATHS_LINETO, {0.45, 0.00}},
    {PATHS_MOVETO, {0.0, -0.75}},
    {PATHS_LINETO, {0.25, -1.00}},
    {PATHS_LINETO, {0.25, 0.00}},
    {PATHS_END, {0.0, 0.0}}
};

path_symbol icon_two[] = {
    {PATHS_MOVETO, {0.0, -0.75}},
    {PATHS_ARCTO, {0.45, 1.00, 0.50, 180.00, -216.87}},
    {PATHS_LINETO, {0.0, 0.0}},
    {PATHS_LINETO, {0.50, 0.0}},
    {PATHS_END, {0.0, 0.0}}
};

path_symbol icon_three[] = {
    {PATHS_ARCMOVETO, {0.0, -0.50, 0.50, 0.50, 195.00}},
    {PATHS_ARCTO, {0.0, -0.50, 0.50, 195.00, 255.00}},
    {PATHS_ARCTO, {0.0, -0.50, 0.50, 270.00, 255.00}},
    {PATHS_END, {0.0, 0.0}}
};

path_symbol icon_four[] = {
    {PATHS_MOVETO, {0.50, 0.0}},
    {PATHS_LINETO, {0.50, -1.00}},
    {PATHS_LINETO, {0.0, -0.50}},
    {PATHS_LINETO, {0.50, -0.50}},
    {PATHS_END, {0.0, 0.0}}
};

path_symbol icon_five[] = {
    {PATHS_MOVETO, {0.50, -1.0}},
    {PATHS_LINETO, {0.0, -1.00}},
    {PATHS_LINETO, {0.0, -0.50}},
    {PATHS_LINETO, {0.25, -0.50}},
    {PATHS_LINETO, {0.0, -0.5, 0.5, 0.5, 90.0, -180.0}},
    {PATHS_LINETO, {0.0, 0.0}},
    {PATHS_END, {0.0, 0.0}}
};

path_symbol icon_six[] = {
    /*
    path.addEllipse(QPointF(x+0.25*xScale, y-0.25*yScale), 0.25*xScale, 0.25*yScale);
    path.moveTo(x+0.00*xScale, y-0.25*yScale);
    path.lineTo(x+0.00*xScale, y-0.75*yScale);
    path.arcTo(x+0.00*xScale, y-1.00*yScale, 0.50*xScale, 0.50*yScale, 180.00, -140.00);
    */
    {PATHS_END, {0.0, 0.0}}
};

path_symbol icon_seven[] = {
    {PATHS_MOVETO, {0.0, -1.0}},
    /*
    path.moveTo(x+0.00*xScale, y-1.00*yScale);
    path.lineTo(x+0.50*xScale, y-1.00*yScale);
    path.lineTo(x+0.25*xScale, y-0.25*yScale);
    path.lineTo(x+0.25*xScale, y-0.00*yScale);
    */
    {PATHS_END, {0.0, 0.0}}
};

path_symbol icon_eight[] = {
    /*
    path.addEllipse(QPointF(x+0.25*xScale, y-0.25*yScale), 0.25*xScale, 0.25*yScale);
    path.addEllipse(QPointF(x+0.25*xScale, y-0.75*yScale), 0.25*xScale, 0.25*yScale);
    */
    {PATHS_END, {0.0, 0.0}}
};

path_symbol icon_nine[] = {
    /*
    path.addEllipse(QPointF(x+0.25*xScale, y-0.75*yScale), 0.25*xScale, 0.25*yScale);
    path.moveTo(x+0.50*xScale, y-0.75*yScale);
    path.lineTo(x+0.50*xScale, y-0.25*yScale);
    path.arcTo(x+0.00*xScale, y-0.50*yScale, 0.50*xScale, 0.50*yScale, 0.00, -140.00);
    */
    {PATHS_END, {0.0, 0.0}}
};

path_symbol icon_minus[] = {
    /*
    path.moveTo(x+0.00*xScale, y-0.50*yScale);
    path.lineTo(x+0.50*xScale, y-0.50*yScale);
    */
    {PATHS_END, {0.0, 0.0}}
};

path_symbol icon_apostrophe[] = {
    /*
    path.moveTo(x+0.25*xScale, y-1.00*yScale);
    path.lineTo(x+0.25*xScale, y-0.75*yScale);
    */
    {PATHS_END, {0.0, 0.0}}
};

path_symbol icon_quote[] = {
    /*
    path.moveTo(x+0.10*xScale, y-1.00*yScale);
    path.lineTo(x+0.10*xScale, y-0.75*yScale);
    path.moveTo(x+0.40*xScale, y-1.00*yScale);
    path.lineTo(x+0.40*xScale, y-0.75*yScale);
    */
    {PATHS_END, {0.0, 0.0}}
};

path_symbol *symbol_list[] = {
    icon_zero,
    icon_one,
    icon_two,
    icon_three,
    icon_four,
    icon_five,
    icon_six,
    icon_seven,
    icon_eight,
    icon_nine,
    icon_minus,
    icon_apostrophe,
    icon_quote
};

int n_actions = 68;

int actions_indices[] = {
    ACTION_donothing,
    ACTION_windowcascade,
    ACTION_windowtile,
    ACTION_windowclose,
    ACTION_windowcloseall,
    ACTION_windownext,
    ACTION_windowprevious,
    ACTION_new,
    ACTION_open,
    ACTION_save,
    ACTION_saveas,
    ACTION_print,
    ACTION_designdetails,
    ACTION_exit,
    ACTION_cut,
    ACTION_copy,
    ACTION_paste,
    ACTION_help,
    ACTION_changelog,
    ACTION_tipoftheday,
    ACTION_about,
    ACTION_whatsthis,
    ACTION_undo,
    ACTION_redo,
    ACTION_icon16,
    ACTION_icon24,
    ACTION_icon32,
    ACTION_icon48,
    ACTION_icon64,
    ACTION_icon128,
    ACTION_settingsdialog,
    ACTION_makelayercurrent,
    ACTION_layers,
    ACTION_layerselector,
    ACTION_layerprevious,
    ACTION_colorselector,
    ACTION_linetypeselector,
    ACTION_lineweightselector,
    ACTION_hidealllayers,
    ACTION_showalllayers,
    ACTION_freezealllayers,
    ACTION_thawalllayers,
    ACTION_lockalllayers,
    ACTION_unlockalllayers,
    ACTION_textbold,
    ACTION_textitalic,
    ACTION_textunderline,
    ACTION_textstrikeout,
    ACTION_textoverline,
    ACTION_zoomrealtime,
    ACTION_zoomprevious,
    ACTION_zoomwindow,
    ACTION_zoomdynamic,
    ACTION_zoomscale,
    ACTION_zoomcenter,
    ACTION_zoomin,
    ACTION_zoomout,
    ACTION_zoomselected,
    ACTION_zoomall,
    ACTION_zoomextents,
    ACTION_panrealtime,
    ACTION_panpoint,
    ACTION_panleft,
    ACTION_panright,
    ACTION_panup,
    ACTION_pandown,
    ACTION_day,
    ACTION_night
};

const char *actions_strings[] = {
    "donothing", "&Do Nothing", "Does nothing.",
    "windowcascade", "&Cascade", "Cascade the windows.",
    "windowtile", "&Tile", "Tile the windows.",
    "windowclose", "Cl&ose", "Close the active window.",
    "windowcloseall", "Close &All", "Close all the windows.",
    "windownext", "Ne&xt", "Move the focus to the next window.",
    "windowprevious", "Pre&vious", "Move the focus to the previous window.",
    "new", "&New", "Create a new file.",
    "open", "&Open", "Open an existing file.",
    "save", "&Save", "Save the design to disk.",
    "saveas", "Save &As", "Save the design under a new name.",
    "print", "&Print", "Print the design.",
    "designdetails", "&Details", "Details of the current design.",
    "exit", "E&xit", "Exit the application.",
    "cut", "Cu&t", "Cut the current selection's contents to the clipboard.",
    "copy", "&Copy", "Copy the current selection's contents to the clipboard.",
    "paste", "&Paste", "Paste the clipboard's contents into the current selection.",
    "help", "&Help", "Displays help.",
    "changelog", "&Changelog", "Describes new features in this product.",
    "tipoftheday", "&Tip Of The Day", "Displays a dialog with useful tips",
    "about", "&About Embroidermodder 2", "Displays information about this product.",
    "whatsthis", "&What's This?", "What's This? Context Help!",
    "undo", "&Undo", "Reverses the most recent action.",
    "redo", "&Redo", "Reverses the effects of the previous undo action.",
    "icon16", "Icon&16", "Sets the toolbar icon size to 16x16.",
    "icon24", "Icon&24", "Sets the toolbar icon size to 24x24.",
    "icon32", "Icon&32", "Sets the toolbar icon size to 32x32.",
    "icon48", "Icon&48", "Sets the toolbar icon size to 48x48.",
    "icon64", "Icon&64", "Sets the toolbar icon size to 64x64.",
    "icon128", "Icon12&8", "Sets the toolbar icon size to 128x128.",
    "settingsdialog", "&Settings", "Configure settings specific to this product.",
    "makelayercurrent", "&Make Layer Active", "Makes the layer of a selected object the active layer",
    "layers", "&Layers", "Manages layers and layer properties:  LAYER",
    "layerselector", "&Layer Selector", "Dropdown selector for changing the current layer",
    "layerprevious", "&Layer Previous", "Restores the previous layer settings:  LAYERP",
    "colorselector", "&Color Selector", "Dropdown selector for changing the current thread color",
    "linetypeselector", "&Stitchtype Selector", "Dropdown selector for changing the current stitch type",
    "lineweightselector", "&Threadweight Selector", "Dropdown selector for changing the current thread weight",
    "hidealllayers", "&Hide All Layers", "Turns the visibility off for all layers in the current drawing:  HIDEALL",
    "showalllayers", "&Show All Layers", "Turns the visibility on for all layers in the current drawing:  SHOWALL",
    "freezealllayers", "&Freeze All Layers", "Freezes all layers in the current drawing:  FREEZEALL",
    "thawalllayers", "&Thaw All Layers", "Thaws all layers in the current drawing:  THAWALL",
    "lockalllayers", "&Lock All Layers", "Locks all layers in the current drawing:  LOCKALL",
    "unlockalllayers", "&Unlock All Layers", "Unlocks all layers in the current drawing:  UNLOCKALL",
    "textbold", "&Bold Text", "Sets text to be bold.",
    "textitalic", "&Italic Text", "Sets text to be italic.",
    "textunderline", "&Underline Text", "Sets text to be underlined.",
    "textstrikeout", "&StrikeOut Text", "Sets text to be striked out.",
    "textoverline", "&Overline Text", "Sets text to be overlined.",
    "zoomrealtime", "Zoom &Realtime", "Zooms to increase or decrease the apparent size of objects in the current viewport.",
    "zoomprevious", "Zoom &Previous", "Zooms to display the previous view.",
    "zoomwindow", "Zoom &Window", "Zooms to display an area specified by a rectangular window.",
    "zoomdynamic", "Zoom &Dynamic", "Zooms to display the generated portion of the drawing.",
    "zoomscale", "Zoom &Scale", "Zooms the display using a specified scale factor.",
    "zoomcenter", "Zoom &Center", "Zooms to display a view specified by a center point and magnification or height.",
    "zoomin", "Zoom &In", "Zooms to increase the apparent size of objects.",
    "zoomout", "Zoom &Out", "Zooms to decrease the apparent size of objects.",
    "zoomselected", "Zoom Selec&ted", "Zooms to display the selected objects.",
    "zoomall", "Zoom &All", "Zooms to display the drawing extents or the grid limits.",
    "zoomextents", "Zoom &Extents", "Zooms to display the drawing extents.",
    "panrealtime", "&Pan Realtime", "Moves the view in the current viewport.",
    "panpoint", "&Pan Point", "Moves the view by the specified distance.",
    "panleft", "&Pan Left", "Moves the view to the left.",
    "panright", "&Pan Right", "Moves the view to the right.",
    "panup", "&Pan Up", "Moves the view up.",
    "pandown", "&Pan Down", "Moves the view down.",
    "day", "&Day", "Updates the current view using day vision settings.",
    "night", "&Night", "Updates the current view using night vision settings."
};

