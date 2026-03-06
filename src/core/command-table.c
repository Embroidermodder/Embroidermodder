/*
 * Embroidermodder 2 -- Commands
 * Copyright 2011-2026 The Embroidermodder Team
 *
 * TODO: sort out mac shortcuts.
 *
 * TODO: Look up what these are defined as:
 *      "windowclose" QKeySequence::Close
 *      "windownext" QKeySequence::NextChild
 *      "windowprevious" QKeySequence::PreviousChild
 */

#include "core.h"

CommandData command_table[] = {
    {
        .id = ACTION_donothing,
        .type = CMD_TYPE_TRIGGER,
        .label = "donothing",
        .tooltip = "&Do Nothing",
        .statustip = "Does Nothing",
        .shortcut = "",
        .mac_shortcut = "",
        .command = do_nothing_cmd
    },
    {
        .id = ACTION_windowcascade,
        .type = CMD_TYPE_TRIGGER,
        .label = "windowcascade",
        .tooltip = "&Cascade",
        .statustip = "Cascade the windows.",
        .shortcut = "",
        .mac_shortcut = "",
        .command = window_cascade_cmd
    },
    {
        .id = ACTION_windowtile,
        .type = CMD_TYPE_TRIGGER,
        .label = "windowtile",
        .tooltip = "&Tile",
        .statustip = "Tile the windows.",
        .shortcut = "",
        .mac_shortcut = "",
        .command = window_tile_cmd
    },
    {
        .id = ACTION_windowclose,
        .type = CMD_TYPE_TRIGGER,
        .label = "windowclose",
        .tooltip = "Cl&ose",
        .statustip = "Close the active window.",
        .shortcut = "",
        .mac_shortcut = "",
        .command = window_close_cmd
    },
    {
        .id = ACTION_windowcloseall,
        .type = CMD_TYPE_TRIGGER,
        .label = "windowcloseall",
        .tooltip = "Close &All",
        .statustip = "Close all the windows.",
        .shortcut = "",
        .mac_shortcut = "",
        .command = window_close_all_cmd
    },
    {
        .id = ACTION_windownext,
        .type = CMD_TYPE_TRIGGER,
        .label = "windownext",
        .tooltip = "Ne&xt",
        .statustip = "Move the focus to the next window.",
        .shortcut = "",
        .mac_shortcut = "",
        .command = window_next_cmd
    },
    {
        .id = ACTION_windowprevious,
        .type = CMD_TYPE_TRIGGER,
        .label = "windowprevious",
        .tooltip = "Pre&vious",
        .statustip = "Move the focus to the previous window.",
        .shortcut = "",
        .mac_shortcut = "",
        .command = window_previous_cmd
    },
    {
        .id = ACTION_new,
        .type = CMD_TYPE_TRIGGER,
        .label = "new",
        .tooltip = "&New",
        .statustip = "Create a new file.",
        .shortcut = "Ctrl+N",
        .mac_shortcut = "",
        .command = new_cmd
    },
    {
        .id = ACTION_open,
        .type = CMD_TYPE_TRIGGER,
        .label = "open",
        .tooltip = "&Open",
        .statustip = "Open an existing file.",
        .shortcut = "Ctrl+O",
        .mac_shortcut = "",
        .command = open_cmd
    },
    {
        .id = ACTION_save,
        .type = CMD_TYPE_TRIGGER,
        .label = "save",
        .tooltip = "&Save",
        .statustip = "Save the design to disk.",
        .shortcut = "Ctrl+S",
        .mac_shortcut = "",
        .command = save_cmd
    },
    {
        .id = ACTION_saveas,
        .type = CMD_TYPE_TRIGGER,
        .label = "saveas",
        .tooltip = "Save &As",
        .statustip = "Save the design under a new name.",
        .shortcut = "Ctrl+Shift+S",
        .mac_shortcut = "",
        .command = save_as_cmd
    },
    {
        .id = ACTION_print,
        .type = CMD_TYPE_TRIGGER,
        .label = "print",
        .tooltip = "&Print",
        .statustip = "Print the design.",
        .shortcut = "Ctrl+P",
        .mac_shortcut = "",
        .command = print_cmd
    },
    {
        .id = ACTION_designdetails,
        .type = CMD_TYPE_TRIGGER,
        .label = "designdetails",
        .tooltip = "&Details",
        .statustip = "Details of the current design.",
        .shortcut = "Ctrl+D",
        .mac_shortcut = "",
        .command = design_details_cmd
    },
    {
        .id = ACTION_exit,
        .type = CMD_TYPE_TRIGGER,
        .label = "exit",
        .tooltip = "E&xit",
        .statustip = "Exit the application.",
        .shortcut = "Ctrl+Q",
        .mac_shortcut = "",
        .command = exit_cmd
    },
    {
        .id = ACTION_cut,
        .type = CMD_TYPE_TRIGGER,
        .label = "cut",
        .tooltip = "Cu&t",
        .statustip = "Cut the current selection's contents to the clipboard.",
        .shortcut = "Ctrl+X",
        .mac_shortcut = "",
        .command = cut_cmd
    },
    {
        .id = ACTION_copy,
        .type = CMD_TYPE_TRIGGER,
        .label = "copy",
        .tooltip = "&Copy",
        .statustip = "Copy the current selection's contents to the clipboard.",
        .shortcut = "Ctrl+C",
        .mac_shortcut = "",
        .command = copy_cmd
    },
    {
        .id = ACTION_paste,
        .type = CMD_TYPE_TRIGGER,
        .label = "paste",
        .tooltip = "&Paste",
        .statustip = "Paste the clipboard's contents into the current selection.",
        .shortcut = "Ctrl+V",
        .mac_shortcut = "",
        .command = paste_cmd
    },
    {
        .id = ACTION_help,
        .type = CMD_TYPE_TRIGGER,
        .label = "help",
        .tooltip = "&Help",
        .statustip = "Displays help.",
        .shortcut = "",
        .mac_shortcut = "",
        .command = help_cmd
    },
    {
        .id = ACTION_changelog,
        .type = CMD_TYPE_TRIGGER,
        .label = "changelog",
        .tooltip = "&Changelog",
        .statustip = "Describes new features in this product.",
        .shortcut = "",
        .mac_shortcut = "",
        .command = changelog_cmd
    },
    {
        .id = ACTION_tipoftheday,
        .type = CMD_TYPE_TRIGGER,
        .label = "tipoftheday",
        .tooltip = "&Tip Of The Day",
        .statustip = "Displays a dialog with useful tips",
        .shortcut = "",
        .mac_shortcut = "",
        .command = tip_of_the_day_cmd
    },
    {
        .id = ACTION_about,
        .type = CMD_TYPE_TRIGGER,
        .label = "about",
        .tooltip = "&About Embroidermodder 2",
        .statustip = "Displays information about this product.",
        .shortcut = "",
        .mac_shortcut = "",
        .command = about_cmd
    },
    {
        .id = ACTION_whatsthis,
        .type = CMD_TYPE_TRIGGER,
        .label = "whatsthis",
        .tooltip = "&What's This?",
        .statustip = "What's This? Context Help!",
        .shortcut = "",
        .mac_shortcut = "",
        .command = whats_this_cmd
    },
    {
        .id = ACTION_undo,
        .type = CMD_TYPE_TRIGGER,
        .label = "undo",
        .tooltip = "&Undo",
        .statustip = "Reverses the most recent action.",
        .shortcut = "",
        .mac_shortcut = "",
        .command = undo_cmd
    },
    {
        .id = ACTION_redo,
        .type = CMD_TYPE_TRIGGER,
        .label = "redo",
        .tooltip = "&Redo",
        .statustip = "Reverses the effects of the previous undo action.",
        .shortcut = "",
        .mac_shortcut = "",
        .command = redo_cmd
    },
    {
        .id = ACTION_icon16,
        .type = CMD_TYPE_TRIGGER,
        .label = "icon16",
        .tooltip = "Icon&16",
        .statustip = "Sets the toolbar icon size to 16x16.",
        .shortcut = "",
        .mac_shortcut = "",
        .command = icon16_cmd
    },
    {
        .id = ACTION_icon24,
        .type = CMD_TYPE_TRIGGER,
        .label = "icon24",
        .tooltip = "Icon&24",
        .statustip = "Sets the toolbar icon size to 24x24.",
        .shortcut = "",
        .mac_shortcut = "",
        .command = icon24_cmd
    },
    {
        .id = ACTION_icon32,
        .type = CMD_TYPE_TRIGGER,
        .label = "icon32",
        .tooltip = "Icon&32",
        .statustip = "Sets the toolbar icon size to 32x32.",
        .shortcut = "",
        .mac_shortcut = "",
        .command = icon32_cmd
    },
    {
        .id = ACTION_icon48,
        .type = CMD_TYPE_TRIGGER,
        .label = "icon48",
        .tooltip = "Icon&48",
        .statustip = "Sets the toolbar icon size to 48x48.",
        .shortcut = "",
        .mac_shortcut = "",
        .command = icon48_cmd
    },
    {
        .id = ACTION_icon64,
        .type = CMD_TYPE_TRIGGER,
        .label = "icon64",
        .tooltip = "Icon&64",
        .statustip = "Sets the toolbar icon size to 64x64.",
        .shortcut = "",
        .mac_shortcut = "",
        .command = icon64_cmd
    },
    {
        .id = ACTION_icon128,
        .type = CMD_TYPE_TRIGGER,
        .label = "icon128",
        .tooltip = "Icon12&8",
        .statustip = "Sets the toolbar icon size to 128x128.",
        .shortcut = "",
        .mac_shortcut = "",
        .command = icon128_cmd
    },
    {
        .id = ACTION_settingsdialog,
        .type = CMD_TYPE_TRIGGER,
        .label = "settingsdialog",
        .tooltip = "&Settings",
        .statustip = "Configure settings specific to this product.",
        .shortcut = "",
        .mac_shortcut = "",
        .command = settingsdialog_cmd
    },
    {
        .id = ACTION_makelayercurrent,
        .type = CMD_TYPE_TRIGGER,
        .label = "makelayercurrent",
        .tooltip = "&Make Layer Active",
        .statustip = "Makes the layer of a selected object the active layer",
        .shortcut = "",
        .mac_shortcut = "",
        .command = makelayercurrent_cmd
    },
    {
        .id = ACTION_layers,
        .type = CMD_TYPE_TRIGGER,
        .label = "layers",
        .tooltip = "&Layers",
        .statustip = "Manages layers and layer properties:  LAYER",
        .shortcut = "",
        .mac_shortcut = "",
        .command = layers_cmd
    },
    {
        .id = ACTION_layerselector,
        .type = CMD_TYPE_TRIGGER,
        .label = "layerselector",
        .tooltip = "&Layer Selector",
        .statustip = "Dropdown selector for changing the current layer",
        .shortcut = "",
        .mac_shortcut = "",
        .command = layerselector_cmd
    },
    {
        .id = ACTION_layerprevious,
        .type = CMD_TYPE_TRIGGER,
        .label = "layerprevious",
        .tooltip = "&Layer Previous",
        .statustip = "Restores the previous layer settings:  LAYERP",
        .shortcut = "",
        .mac_shortcut = "",
        .command = layerprevious_cmd
    },
    {
        .id = ACTION_colorselector,
        .type = CMD_TYPE_TRIGGER,
        .label = "colorselector",
        .tooltip = "&Color Selector",
        .statustip = "Dropdown selector for changing the current thread color",
        .shortcut = "",
        .mac_shortcut = "",
        .command = colorselector_cmd
    },
    {
        .id = ACTION_linetypeselector,
        .type = CMD_TYPE_TRIGGER,
        .label = "linetypeselector",
        .tooltip = "&Stitchtype Selector",
        .statustip = "Dropdown selector for changing the current stitch type",
        .shortcut = "",
        .mac_shortcut = "",
        .command = linetypeselector_cmd
    },
    {
        .id = ACTION_lineweightselector,
        .type = CMD_TYPE_TRIGGER,
        .label = "lineweightselector",
        .tooltip = "&Threadweight Selector",
        .statustip = "Dropdown selector for changing the current thread weight",
        .shortcut = "",
        .mac_shortcut = "",
        .command = lineweightselector_cmd
    },
    {
        .id = ACTION_hidealllayers,
        .type = CMD_TYPE_TRIGGER,
        .label = "hidealllayers",
        .tooltip = "&Hide All Layers",
        .statustip = "Turns the visibility off for all layers in the current drawing:  HIDEALL",
        .shortcut = "",
        .mac_shortcut = "",
        .command = hidealllayers_cmd
    },
    {
        .id = ACTION_showalllayers,
        .type = CMD_TYPE_TRIGGER,
        .label = "showalllayers",
        .tooltip = "&Show All Layers",
        .statustip = "Turns the visibility on for all layers in the current drawing:  SHOWALL",
        .shortcut = "",
        .mac_shortcut = "",
        .command = showalllayers_cmd
    },
    {
        .id = ACTION_freezealllayers,
        .type = CMD_TYPE_TRIGGER,
        .label = "freezealllayers",
        .tooltip = "&Freeze All Layers",
        .statustip = "Freezes all layers in the current drawing:  FREEZEALL",
        .shortcut = "",
        .mac_shortcut = "",
        .command = freezealllayers_cmd
    },
    {
        .id = ACTION_thawalllayers,
        .type = CMD_TYPE_TRIGGER,
        .label = "thawalllayers",
        .tooltip = "&Thaw All Layers",
        .statustip = "Thaws all layers in the current drawing:  THAWALL",
        .shortcut = "",
        .mac_shortcut = "",
        .command = thawalllayers_cmd
    },
    {
        .id = ACTION_lockalllayers,
        .type = CMD_TYPE_TRIGGER,
        .label = "lockalllayers",
        .tooltip = "&Lock All Layers",
        .statustip = "Locks all layers in the current drawing:  LOCKALL",
        .shortcut = "",
        .mac_shortcut = "",
        .command = lockalllayers_cmd
    },
    {
        .id = ACTION_unlockalllayers,
        .type = CMD_TYPE_TRIGGER,
        .label = "unlockalllayers",
        .tooltip = "&Unlock All Layers",
        .statustip = "Unlocks all layers in the current drawing:  UNLOCKALL",
        .shortcut = "",
        .mac_shortcut = "",
        .command = unlockalllayers_cmd
    },
    {
        .id = ACTION_textbold,
        .type = CMD_TYPE_TOGGLE,
        .label = "textbold",
        .tooltip = "&Bold Text",
        .statustip = "Sets text to be bold.",
        .shortcut = "",
        .mac_shortcut = "",
        .command = text_bold_cmd
    },
    {
        .id = ACTION_textitalic,
        .type = CMD_TYPE_TOGGLE,
        .label = "textitalic",
        .tooltip = "&Italic Text",
        .statustip = "Sets text to be italic.",
        .shortcut = "",
        .mac_shortcut = "",
        .command = text_italic_cmd
    },
    {
        .id = ACTION_textunderline,
        .type = CMD_TYPE_TOGGLE,
        .label = "textunderline",
        .tooltip = "&Underline Text",
        .statustip = "Sets text to be underlined.",
        .shortcut = "",
        .mac_shortcut = "",
        .command = text_underline_cmd
    },
    {
        .id = ACTION_textstrikeout,
        .type = CMD_TYPE_TOGGLE,
        .label = "textstrikeout",
        .tooltip = "&StrikeOut Text",
        .statustip = "Sets text to be striked out.",
        .shortcut = "",
        .mac_shortcut = "",
        .command = text_strikeout_cmd
    },
    {
        .id = ACTION_textoverline,
        .type = CMD_TYPE_TOGGLE,
        .label = "textoverline",
        .tooltip = "&Overline Text",
        .statustip = "Sets text to be overlined.",
        .shortcut = "",
        .mac_shortcut = "",
        .command = text_overline_cmd
    },
    {
        .id = ACTION_zoomrealtime,
        .type = CMD_TYPE_TRIGGER,
        .label = "zoomrealtime",
        .tooltip = "Zoom &Realtime",
        .statustip = "Zooms to increase or decrease the apparent size of objects in the current viewport.",
        .shortcut = "",
        .mac_shortcut = "",
        .command = zoom_real_time_cmd
    },
    {
        .id = ACTION_zoomprevious,
        .type = CMD_TYPE_TRIGGER,
        .label = "zoomprevious",
        .tooltip = "Zoom &Previous",
        .statustip = "Zooms to display the previous view.",
        .shortcut = "",
        .mac_shortcut = "",
        .command = zoom_previous_cmd
    },
    {
        .id = ACTION_zoomwindow,
        .type = CMD_TYPE_TRIGGER,
        .label = "zoomwindow",
        .tooltip = "Zoom &Window",
        .statustip = "Zooms to display an area specified by a rectangular window.",
        .shortcut = "",
        .mac_shortcut = "",
        .command = zoom_window_cmd
    },
    {
        .id = ACTION_zoomdynamic,
        .type = CMD_TYPE_TRIGGER,
        .label = "zoomdynamic",
        .tooltip = "Zoom &Dynamic",
        .statustip = "Zooms to display the generated portion of the drawing.",
        .shortcut = "",
        .mac_shortcut = "",
        .command = zoom_dynamic_cmd
    },
    {
        .id = ACTION_zoomscale,
        .type = CMD_TYPE_TRIGGER,
        .label = "zoomscale",
        .tooltip = "Zoom &Scale",
        .statustip = "Zooms the display using a specified scale factor.",
        .shortcut = "",
        .mac_shortcut = "",
        .command = zoom_scale_cmd
    },
    {
        .id = ACTION_zoomcenter,
        .type = CMD_TYPE_TRIGGER,
        .label = "zoomcenter",
        .tooltip = "Zoom &Center",
        .statustip = "Zooms to display a view specified by a center point and magnification or height.",
        .shortcut = "",
        .mac_shortcut = "",
        .command = zoom_center_cmd
    },
    {
        .id = ACTION_zoomin,
        .type = CMD_TYPE_TRIGGER,
        .label = "zoomin",
        .tooltip = "Zoom &In",
        .statustip = "Zooms to increase the apparent size of objects.",
        .shortcut = "",
        .mac_shortcut = "",
        .command = zoom_in_cmd
    },
    {
        .id = ACTION_zoomout,
        .type = CMD_TYPE_TRIGGER,
        .label = "zoomout",
        .tooltip = "Zoom &Out",
        .statustip = "Zooms to decrease the apparent size of objects.",
        .shortcut = "",
        .mac_shortcut = "",
        .command = zoom_out_cmd
    },
    {
        .id = ACTION_zoomselected,
        .type = CMD_TYPE_TRIGGER,
        .label = "zoomselected",
        .tooltip = "Zoom Selec&ted",
        .statustip = "Zooms to display the selected objects.",
        .shortcut = "",
        .mac_shortcut = "",
        .command = zoom_selected_cmd
    },
    {
        .id = ACTION_zoomall,
        .type = CMD_TYPE_TRIGGER,
        .label = "zoomall",
        .tooltip = "Zoom &All",
        .statustip = "Zooms to display the drawing extents or the grid limits.",
        .shortcut = "",
        .mac_shortcut = "",
        .command = zoom_all_cmd
    },
    {
        .id = ACTION_zoomextents,
        .type = CMD_TYPE_TRIGGER,
        .label = "zoomextents",
        .tooltip = "Zoom &Extents",
        .statustip = "Zooms to display the drawing extents.",
        .shortcut = "",
        .mac_shortcut = "",
        .command = zoom_extents_cmd
    },
    {
        .id = ACTION_panrealtime,
        .type = CMD_TYPE_TRIGGER,
        .label = "panrealtime",
        .tooltip = "&Pan Realtime",
        .statustip = "Moves the view in the current viewport.",
        .shortcut = "",
        .mac_shortcut = "",
        .command = pan_real_time_cmd
    },
    {
        .id = ACTION_panpoint,
        .type = CMD_TYPE_TRIGGER,
        .label = "panpoint",
        .tooltip = "&Pan Point",
        .statustip = "Moves the view by the specified distance.",
        .shortcut = "",
        .mac_shortcut = "",
        .command = pan_point_cmd
    },
    {
        .id = ACTION_panleft,
        .type = CMD_TYPE_TRIGGER,
        .label = "panleft",
        .tooltip = "&Pan Left",
        .statustip = "Moves the view to the left.",
        .shortcut = "",
        .mac_shortcut = "",
        .command = pan_left_cmd
    },
    {
        .id = ACTION_panright,
        .type = CMD_TYPE_TRIGGER,
        .label = "panright",
        .tooltip = "&Pan Right",
        .statustip = "Moves the view to the right.",
        .shortcut = "",
        .mac_shortcut = "",
        .command = pan_right_cmd
    },
    {
        .id = ACTION_panup,
        .type = CMD_TYPE_TRIGGER,
        .label = "panup",
        .tooltip = "&Pan Up",
        .statustip = "Moves the view up.",
        .shortcut = "",
        .mac_shortcut = "",
        .command = pan_up_cmd
    },
    {
        .id = ACTION_pandown,
        .type = CMD_TYPE_TRIGGER,
        .label = "pandown",
        .tooltip = "&Pan Down",
        .statustip = "Moves the view down.",
        .shortcut = "",
        .mac_shortcut = "",
        .command = pan_down_cmd
    },
    {
        .id = ACTION_day,
        .type = CMD_TYPE_TRIGGER,
        .label = "day",
        .tooltip = "&Day",
        .statustip = "Updates the current view using day vision settings.",
        .shortcut = "",
        .mac_shortcut = "",
        .command = day_cmd
    },
    {
        .id = ACTION_night,
        .type = CMD_TYPE_TRIGGER,
        .label = "night",
        .tooltip = "&Night",
        .statustip = "Updates the current view using night vision settings.",
        .shortcut = "",
        .mac_shortcut = "",
        .command = night_cmd
    },
    {
        .id = ACTION_null,
        .type = CMD_TYPE_TRIGGER,
        .label = "END",
        .tooltip = "END",
        .statustip = "END",
        .shortcut = "",
        .mac_shortcut = "",
        .command = do_nothing_cmd
    }
};

