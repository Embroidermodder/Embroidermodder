/*
 * Embroidermodder 2 -- Commands
 * Copyright 2011-2026 The Embroidermodder Team
 */

#include "commands.h"

CommandData command_table[] = {
    {
        .id = ACTION_donothing,
        .label = "donothing",
        .tooltip = "&Do Nothing",
        .statustip = "Does Nothing",
        .command = do_nothing_cmd
    },
    {
        .id = ACTION_windowcascade,
        .label = "windowcascade",
        .tooltip = "&Cascade",
        .statustip = "Cascade the windows.",
        .command = do_nothing_cmd
    },
    {
        .id = ACTION_windowtile,
        .label = "windowtile",
        .tooltip = "&Tile",
        .statustip = "Tile the windows.",
        .command = do_nothing_cmd
    },
    {
        .id = ACTION_windowclose,
        .label = "windowclose",
        .tooltip = "Cl&ose",
        .statustip = "Close the active window.",
        .command = do_nothing_cmd
    },
    {
        .id = ACTION_windowcloseall,
        .label = "windowcloseall",
        .tooltip = "Close &All",
        .statustip = "Close all the windows.",
        .command = do_nothing_cmd
    },
    {
        .id = ACTION_windownext,
        .label = "windownext",
        .tooltip = "Ne&xt",
        .statustip = "Move the focus to the next window.",
        .command = do_nothing_cmd
    },
    {
        .id = ACTION_windowprevious,
        .label = "windowprevious",
        .tooltip = "Pre&vious",
        .statustip = "Move the focus to the previous window.",
        .command = do_nothing_cmd
    },
    {
        .id = ACTION_new,
        .label = "new",
        .tooltip = "&New",
        .statustip = "Create a new file.",
        .command = do_nothing_cmd
    },
    {
        .id = ACTION_open,
        .label = "open",
        .tooltip = "&Open",
        .statustip = "Open an existing file.",
        .command = do_nothing_cmd
    },
    {
        .id = ACTION_save,
        .label = "save",
        .tooltip = "&Save",
        .statustip = "Save the design to disk.",
        .command = do_nothing_cmd
    },
    {
        .id = ACTION_saveas,
        .label = "saveas",
        .tooltip = "Save &As",
        .statustip = "Save the design under a new name.",
        .command = do_nothing_cmd
    },
    {
        .id = ACTION_print,
        .label = "print",
        .tooltip = "&Print",
        .statustip = "Print the design.",
        .command = do_nothing_cmd
    },
    {
        .id = ACTION_designdetails,
        .label = "designdetails",
        .tooltip = "&Details",
        .statustip = "Details of the current design.",
        .command = do_nothing_cmd
    },
    {
        .id = ACTION_exit,
        .label = "exit",
        .tooltip = "E&xit",
        .statustip = "Exit the application.",
        .command = do_nothing_cmd
    },
    {
        .id = ACTION_cut,
        .label = "cut",
        .tooltip = "Cu&t",
        .statustip = "Cut the current selection's contents to the clipboard.",
        .command = do_nothing_cmd
    },
    {
        .id = ACTION_copy,
        .label = "copy",
        .tooltip = "&Copy",
        .statustip = "Copy the current selection's contents to the clipboard.",
        .command = do_nothing_cmd
    },
    {
        .id = ACTION_paste,
        .label = "paste",
        .tooltip = "&Paste",
        .statustip = "Paste the clipboard's contents into the current selection.",
        .command = do_nothing_cmd
    },
    {
        .id = ACTION_help,
        .label = "help",
        .tooltip = "&Help",
        .statustip = "Displays help.",
        .command = do_nothing_cmd
    },
    {
        .id = ACTION_changelog,
        .label = "changelog",
        .tooltip = "&Changelog",
        .statustip = "Describes new features in this product.",
        .command = do_nothing_cmd
    },
    {
        .id = ACTION_tipoftheday,
        .label = "tipoftheday",
        .tooltip = "&Tip Of The Day",
        .statustip = "Displays a dialog with useful tips",
        .command = do_nothing_cmd
    },
    {
        .id = ACTION_about,
        .label = "about",
        .tooltip = "&About Embroidermodder 2",
        .statustip = "Displays information about this product.",
        .command = about_cmd
    },
    {
        .id = ACTION_whatsthis,
        .label = "whatsthis",
        .tooltip = "&What's This?",
        .statustip = "What's This? Context Help!",
        .command = do_nothing_cmd
    },
    {
        .id = ACTION_undo,
        .label = "undo",
        .tooltip = "&Undo",
        .statustip = "Reverses the most recent action.",
        .command = do_nothing_cmd
    },
    {
        .id = ACTION_redo,
        .label = "redo",
        .tooltip = "&Redo",
        .statustip = "Reverses the effects of the previous undo action.",
        .command = do_nothing_cmd
    },
    {
        .id = ACTION_icon16,
        .label = "icon16",
        .tooltip = "Icon&16",
        .statustip = "Sets the toolbar icon size to 16x16.",
        .command = do_nothing_cmd
    },
    {
        .id = ACTION_icon24,
        .label = "icon24",
        .tooltip = "Icon&24",
        .statustip = "Sets the toolbar icon size to 24x24.",
        .command = do_nothing_cmd
    },
    {
        .id = ACTION_icon32,
        .label = "icon32",
        .tooltip = "Icon&32",
        .statustip = "Sets the toolbar icon size to 32x32.",
        .command = do_nothing_cmd
    },
    {
        .id = ACTION_icon48,
        .label = "icon48",
        .tooltip = "Icon&48",
        .statustip = "Sets the toolbar icon size to 48x48.",
        .command = do_nothing_cmd
    },
    {
        .id = ACTION_icon64,
        .label = "icon64",
        .tooltip = "Icon&64",
        .statustip = "Sets the toolbar icon size to 64x64.",
        .command = do_nothing_cmd
    },
    {
        .id = ACTION_icon128,
        .label = "icon128",
        .tooltip = "Icon12&8",
        .statustip = "Sets the toolbar icon size to 128x128.",
        .command = do_nothing_cmd
    },
    {
        .id = ACTION_settingsdialog,
        .label = "settingsdialog",
        .tooltip = "&Settings",
        .statustip = "Configure settings specific to this product.",
        .command = do_nothing_cmd
    },
    {
        .id = ACTION_makelayercurrent,
        .label = "makelayercurrent",
        .tooltip = "&Make Layer Active",
        .statustip = "Makes the layer of a selected object the active layer",
        .command = do_nothing_cmd
    },
    {
        .id = ACTION_layers,
        .label = "layers",
        .tooltip = "&Layers",
        .statustip = "Manages layers and layer properties:  LAYER",
        .command = do_nothing_cmd
    },
    {
        .id = ACTION_layerselector,
        .label = "layerselector",
        .tooltip = "&Layer Selector",
        .statustip = "Dropdown selector for changing the current layer",
        .command = do_nothing_cmd
    },
    {
        .id = ACTION_layerprevious,
        .label = "layerprevious",
        .tooltip = "&Layer Previous",
        .statustip = "Restores the previous layer settings:  LAYERP",
        .command = do_nothing_cmd
    },
    {
        .id = ACTION_colorselector,
        .label = "colorselector",
        .tooltip = "&Color Selector",
        .statustip = "Dropdown selector for changing the current thread color",
        .command = do_nothing_cmd
    },
    {
        .id = ACTION_linetypeselector,
        .label = "linetypeselector",
        .tooltip = "&Stitchtype Selector",
        .statustip = "Dropdown selector for changing the current stitch type",
        .command = do_nothing_cmd
    },
    {
        .id = ACTION_lineweightselector,
        .label = "lineweightselector",
        .tooltip = "&Threadweight Selector",
        .statustip = "Dropdown selector for changing the current thread weight",
        .command = do_nothing_cmd
    },
    {
        .id = ACTION_hidealllayers,
        .label = "hidealllayers",
        .tooltip = "&Hide All Layers",
        .statustip = "Turns the visibility off for all layers in the current drawing:  HIDEALL",
        .command = do_nothing_cmd
    },
    {
        .id = ACTION_showalllayers,
        .label = "showalllayers",
        .tooltip = "&Show All Layers",
        .statustip = "Turns the visibility on for all layers in the current drawing:  SHOWALL",
        .command = do_nothing_cmd
    },
    {
        .id = ACTION_freezealllayers,
        .label = "freezealllayers",
        .tooltip = "&Freeze All Layers",
        .statustip = "Freezes all layers in the current drawing:  FREEZEALL",
        .command = do_nothing_cmd
    },
    {
        .id = ACTION_thawalllayers,
        .label = "thawalllayers",
        .tooltip = "&Thaw All Layers",
        .statustip = "Thaws all layers in the current drawing:  THAWALL",
        .command = do_nothing_cmd
    },
    {
        .id = ACTION_lockalllayers,
        .label = "lockalllayers",
        .tooltip = "&Lock All Layers",
        .statustip = "Locks all layers in the current drawing:  LOCKALL",
        .command = do_nothing_cmd
    },
    {
        .id = ACTION_unlockalllayers,
        .label = "unlockalllayers",
        .tooltip = "&Unlock All Layers",
        .statustip = "Unlocks all layers in the current drawing:  UNLOCKALL",
        .command = do_nothing_cmd
    },
    {
        .id = ACTION_textbold,
        .label = "textbold",
        .tooltip = "&Bold Text",
        .statustip = "Sets text to be bold.",
        .command = do_nothing_cmd
    },
    {
        .id = ACTION_textitalic,
        .label = "textitalic",
        .tooltip = "&Italic Text",
        .statustip = "Sets text to be italic.",
        .command = do_nothing_cmd
    },
    {
        .id = ACTION_textunderline,
        .label = "textunderline",
        .tooltip = "&Underline Text",
        .statustip = "Sets text to be underlined.",
        .command = do_nothing_cmd
    },
    {
        .id = ACTION_textstrikeout,
        .label = "textstrikeout",
        .tooltip = "&StrikeOut Text",
        .statustip = "Sets text to be striked out.",
        .command = do_nothing_cmd
    },
    {
        .id = ACTION_textoverline,
        .label = "textoverline",
        .tooltip = "&Overline Text",
        .statustip = "Sets text to be overlined.",
        .command = do_nothing_cmd
    },
    {
        .id = ACTION_zoomrealtime,
        .label = "zoomrealtime",
        .tooltip = "Zoom &Realtime",
        .statustip = "Zooms to increase or decrease the apparent size of objects in the current viewport.",
        .command = do_nothing_cmd
    },
    {
        .id = ACTION_zoomprevious,
        .label = "zoomprevious",
        .tooltip = "Zoom &Previous",
        .statustip = "Zooms to display the previous view.",
        .command = do_nothing_cmd
    },
    {
        .id = ACTION_zoomwindow,
        .label = "zoomwindow",
        .tooltip = "Zoom &Window",
        .statustip = "Zooms to display an area specified by a rectangular window.",
        .command = do_nothing_cmd
    },
    {
        .id = ACTION_zoomdynamic,
        .label = "zoomdynamic",
        .tooltip = "Zoom &Dynamic",
        .statustip = "Zooms to display the generated portion of the drawing.",
        .command = do_nothing_cmd
    },
    {
        .id = ACTION_zoomscale,
        .label = "zoomscale",
        .tooltip = "Zoom &Scale",
        .statustip = "Zooms the display using a specified scale factor.",
        .command = do_nothing_cmd
    },
    {
        .id = ACTION_zoomcenter,
        .label = "zoomcenter",
        .tooltip = "Zoom &Center",
        .statustip = "Zooms to display a view specified by a center point and magnification or height.",
        .command = do_nothing_cmd
    },
    {
        .id = ACTION_zoomin,
        .label = "zoomin",
        .tooltip = "Zoom &In",
        .statustip = "Zooms to increase the apparent size of objects.",
        .command = do_nothing_cmd
    },
    {
        .id = ACTION_zoomout,
        .label = "zoomout",
        .tooltip = "Zoom &Out",
        .statustip = "Zooms to decrease the apparent size of objects.",
        .command = do_nothing_cmd
    },
    {
        .id = ACTION_zoomselected,
        .label = "zoomselected",
        .tooltip = "Zoom Selec&ted",
        .statustip = "Zooms to display the selected objects.",
        .command = do_nothing_cmd
    },
    {
        .id = ACTION_zoomall,
        .label = "zoomall",
        .tooltip = "Zoom &All",
        .statustip = "Zooms to display the drawing extents or the grid limits.",
        .command = do_nothing_cmd
    },
    {
        .id = ACTION_zoomextents,
        .label = "zoomextents",
        .tooltip = "Zoom &Extents",
        .statustip = "Zooms to display the drawing extents.",
        .command = do_nothing_cmd
    },
    {
        .id = ACTION_panrealtime,
        .label = "panrealtime",
        .tooltip = "&Pan Realtime",
        .statustip = "Moves the view in the current viewport.",
        .command = do_nothing_cmd
    },
    {
        .id = ACTION_panpoint,
        .label = "panpoint",
        .tooltip = "&Pan Point",
        .statustip = "Moves the view by the specified distance.",
        .command = do_nothing_cmd
    },
    {
        .id = ACTION_panleft,
        .label = "panleft",
        .tooltip = "&Pan Left",
        .statustip = "Moves the view to the left.",
        .command = do_nothing_cmd
    },
    {
        .id = ACTION_panright,
        .label = "panright",
        .tooltip = "&Pan Right",
        .statustip = "Moves the view to the right.",
        .command = do_nothing_cmd
    },
    {
        .id = ACTION_panup,
        .label = "panup",
        .tooltip = "&Pan Up",
        .statustip = "Moves the view up.",
        .command = do_nothing_cmd
    },
    {
        .id = ACTION_pandown,
        .label = "pandown",
        .tooltip = "&Pan Down",
        .statustip = "Moves the view down.",
        .command = do_nothing_cmd
    },
    {
        .id = ACTION_day,
        .label = "day",
        .tooltip = "&Day",
        .statustip = "Updates the current view using day vision settings.",
        .command = do_nothing_cmd
    },
    {
        .id = ACTION_night,
        .label = "night",
        .tooltip = "&Night",
        .statustip = "Updates the current view using night vision settings.",
        .command = do_nothing_cmd
    },
    {
        .id = ACTION_null,
        .label = "END",
        .tooltip = "END",
        .statustip = "END",
        .command = do_nothing_cmd
    }
};

