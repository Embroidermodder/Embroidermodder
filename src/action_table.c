/*
 *  Embroidermodder 2.
 *
 *  ------------------------------------------------------------
 *
 *  Copyright 2013-2023 The Embroidermodder Team
 *  Embroidermodder 2 is Open Source Software.
 *  See LICENSE for licensing terms.
 *
 *  ------------------------------------------------------------
 *
 *  Use Python's PEP7 style guide.
 *      https://peps.python.org/pep-0007/
 *
 *  ------------------------------------------------------------
 */

#include "core.h"

/* . */
const ActionData action_table[MAX_ACTIONS] = {
    {
        .id = ACTION_ABOUT,
        .icon = "about",
        .command = "about",
        .tooltip = "&About Embroidermodder 2",
        .statustip = "Displays information about this product: ABOUT.",
        .shortcut = "",
        .min_args = 0,
        .gview = 0,
        .gscene = 0,
        .undo = 0
    },
    {
        .id = ACTION_TREBLECLEF,
        .icon = "trebleclef",
        .command = "trebleclef",
        .tooltip = "Treble Clef",
        .statustip = "Adds a treble clef design to the vector layer.",
        .shortcut = "",
        .min_args = 0,
        .gview = 1,
        .gscene = 1,
        .undo = 1
    },
    {
        .id = ACTION_DO_NOTHING,
        .icon = "donothing",
        .command = "donothing",
        .tooltip = "&Do Nothing",
        .statustip = "Does Nothing",
        .shortcut = "",
        .min_args = 0,
        .gview = 1,
        .gscene = 1,
        .undo = 1
    },
    {
        .id = ACTION_NEW,
        .icon = "new",
        .command = "new",
        .tooltip = "&New",
        .statustip = "Create a new file: NEW.",
        .shortcut = "Ctrl+N",
        .min_args = 0,
        .gview = 1,
        .gscene = 1,
        .undo = 1
    },
    {
        .id = ACTION_OPEN,
        .icon = "open",
        .command = "open",
        .tooltip = "&Open",
        .statustip = "Open an existing file: OPEN",
        .shortcut = "Ctrl+O",
        .min_args = 0,
        .gview = 1,
        .gscene = 1,
        .undo = 1
    },
    {
        .id = ACTION_SAVE,
        .icon = "save",
        .command = "save",
        .tooltip = "&Save",
        .statustip = "Save the design to disk.",
        .shortcut = "Ctrl+S",
        .min_args = 0,
        .gview = 1,
        .gscene = 1,
        .undo = 1
    },
    {
        .id = ACTION_SAVEAS,
        .icon = "saveas",
        .command = "saveas",
        .tooltip = "Save &As",
        .statustip = "Save the design under a new name and type.",
        .shortcut = "Ctrl+Shift+S",
        .min_args = 0,
        .gview = 1,
        .gscene = 1,
        .undo = 1
    },
    {
        .id = ACTION_PRINT,
        .icon = "print",
        .command = "print",
        .tooltip = "&Print",
        .statustip = "Print the design.",
        .shortcut = "Ctrl+P",
        .min_args = 0,
        .gview = 1,
        .gscene = 1,
        .undo = 1
    },
    {
        .id = ACTION_DESIGN_DETAILS,
        .icon = "designdetails",
        .command = "designdetails",
        .tooltip = "&Details",
        .statustip = "Details of the current design.",
        .shortcut = "Ctrl+D",
        .min_args = 0,
        .gview = 1,
        .gscene = 1,
        .undo = 1
    },
    {
        .id = ACTION_EXIT,
        .icon = "exit",
        .command = "exit",
        .tooltip = "E&xit",
        .statustip = "Exit the application: EXIT",
        .shortcut = "Ctrl+Q",
        .min_args = 0,
        .gview = 1,
        .gscene = 1,
        .undo = 1
    },
    {
        .id = ACTION_CUT,
        .icon = "cut",
        .command = "cut",
        .tooltip = "Cu&t",
        .statustip = "Cut the current selection's contents to the clipboard.",
        .shortcut = "Ctrl+X",
        .min_args = 0,
        .gview = 1,
        .gscene = 1,
        .undo = 1
    },
    {
        .id = ACTION_COPY,
        .icon = "copy",
        .command = "copy",
        .tooltip = "&Copy",
        .statustip = "Copy the current selection's contents to the clipboard.",
        .shortcut = "Ctrl+C",
        .min_args = 0,
        .gview = 1,
        .gscene = 1,
        .undo = 1
    },
    {
        .id = ACTION_PASTE,
        .icon = "paste",
        .command = "paste",
        .tooltip = "&Paste",
        .statustip = "Paste the clipboard's contents into the current selection.",
        .shortcut = "Ctrl+V",
        .min_args = 0,
        .gview = 1,
        .gscene = 1,
        .undo = 1
    },
    {
        .id = ACTION_UNDO,
        .icon = "undo",
        .command = "undo",
        .tooltip = "&Undo",
        .statustip = "Reverses the most recent action: UNDO",
        .shortcut = "Ctrl+Z",
        .min_args = 0,
        .gview = 1,
        .gscene = 1,
        .undo = 1
    },
    {
        .id = ACTION_REDO,
        .icon = "redo",
        .command = "redo",
        .tooltip = "&Redo",
        .statustip = "Reverses the effects of the previous undo action: REDO",
        .shortcut = "Ctrl+Shift+Z",
        .min_args = 0,
        .gview = 1,
        .gscene = 1,
        .undo = 1
    },
    {
        .id = ACTION_WINDOW_CLOSE,
        .icon = "windowclose",
        .command = "windowclose",
        .tooltip = "Cl&ose",
        .statustip = "Close the active window: CLOSE",
        .shortcut = "Ctrl+W",
        .min_args = 0,
        .gview = 1,
        .gscene = 1,
        .undo = 1
    },
    {
        .id = ACTION_WINDOW_CLOSE_ALL,
        .icon = "windowcloseall",
        .command = "windowcloseall",
        .tooltip = "Close &All",
        .statustip = "Close all the windows: CLOSEALL",
        .shortcut = "",
        .min_args = 0,
        .gview = 1,
        .gscene = 1,
        .undo = 1
    },
    {
        .id = ACTION_WINDOW_CASCADE,
        .icon = "windowcascade",
        .command = "windowcascade",
        .tooltip = "&Cascade",
        .statustip = "Cascade the windows: CASCADE",
        .shortcut = "",
        .min_args = 0,
        .gview = 1,
        .gscene = 1,
        .undo = 1
    },
    {
        .id = ACTION_WINDOW_TILE,
        .icon = "windowtile",
        .command = "windowtile",
        .tooltip = "&Tile",
        .statustip = "Tile the windows: TILE",
        .shortcut = "",
        .min_args = 0,
        .gview = 1,
        .gscene = 1,
        .undo = 1
    },
    {
        .id = ACTION_WINDOW_NEXT,
        .icon = "windownext",
        .command = "windownext",
        .tooltip = "Ne&xt",
        .statustip = "Move the focus to the next window: NEXT",
        .shortcut = "Ctrl+Tab",
        .min_args = 0,
        .gview = 1,
        .gscene = 1,
        .undo = 1
    },
    {
        .id = ACTION_WINDOW_PREVIOUS,
        .icon = "windowprevious",
        .command = "windowprevious",
        .tooltip = "Pre&vious",
        .statustip = "Move the focus to the previous window: PREVIOUS",
        .shortcut = "Ctrl+Shift+Tab",
        .min_args = 0,
        .gview = 1,
        .gscene = 1,
        .undo = 1
    },
    {
        .id = ACTION_HELP,
        .icon = "help",
        .command = "help",
        .tooltip = "&Help",
        .statustip = "Displays the help file: HELP.",
        .shortcut = "",
        .min_args = 0,
        .gview = 1,
        .gscene = 1,
        .undo = 1
    },
    {
        .id = ACTION_CHANGELOG,
        .icon = "changelog",
        .command = "changelog",
        .tooltip = "&Changelog",
        .statustip = "Describes new features in this product.",
        .shortcut = "",
        .min_args = 0,
        .gview = 1,
        .gscene = 1,
        .undo = 1
    },
    {
        .id = ACTION_TIP_OF_THE_DAY,
        .icon = "tipoftheday",
        .command = "tips",
        .tooltip = "&Tip Of The Day",
        .statustip = "Displays a dialog with useful tips: TIPS",
        .shortcut = "",
        .min_args = 0,
        .gview = 1,
        .gscene = 1,
        .undo = 1
    },
    {
        .id = ACTION_WHATS_THIS,
        .icon = "whatsthis",
        .command = "whatsthis",
        .tooltip = "&What's This?",
        .statustip = "What's This? Context Help!",
        .shortcut = "",
        .min_args = 0,
        .gview = 1,
        .gscene = 1,
        .undo = 1
    },
    {
        .id = ACTION_ICON16,
        .icon = "icon16",
        .command = "icon16",
        .tooltip = "Icon&16",
        .statustip = "Sets the toolbar icon size to 16x16: ICON16",
        .shortcut = "",
        .min_args = 0,
        .gview = 1,
        .gscene = 1,
        .undo = 1
    },
    {
        .id = ACTION_ICON24,
        .icon = "icon24",
        .command = "icon24",
        .tooltip = "Icon&24",
        .statustip = "Sets the toolbar icon size to 24x24: ICON24.",
        .shortcut = "",
        .min_args = 0,
        .gview = 1,
        .gscene = 1,
        .undo = 1
    },
    {
        .id = ACTION_ICON32,
        .icon = "icon32",
        .command = "icon32",
        .tooltip = "Icon&32",
        .statustip = "Sets the toolbar icon size to 32x32: ICON32",
        .shortcut = "",
        .min_args = 0,
        .gview = 1,
        .gscene = 1,
        .undo = 1
    },
    {
        .id = ACTION_ICON48,
        .icon = "icon48",
        .command = "icon48",
        .tooltip = "Icon&48",
        .statustip = "Sets the toolbar icon size to 48x48: ICON48",
        .shortcut = "",
        .min_args = 0,
        .gview = 1,
        .gscene = 1,
        .undo = 1
    },
    {
        .id = ACTION_ICON64,
        .icon = "icon64",
        .command = "icon64",
        .tooltip = "Icon&64",
        .statustip = "Sets the toolbar icon size to 64x64: ICON64",
        .shortcut = "",
        .min_args = 0,
        .gview = 1,
        .gscene = 1,
        .undo = 1
    },
    {
        .id = ACTION_ICON128,
        .icon = "icon128",
        .command = "icon128",
        .tooltip = "Icon12&8",
        .statustip = "Sets the toolbar icon size to 128x128: ICON128",
        .shortcut = "",
        .min_args = 0,
        .gview = 1,
        .gscene = 1,
        .undo = 1
    },
    {
        .id = ACTION_SETTINGS_DIALOG,
        .icon = "settingsdialog",
        .command = "settingsdialog",
        .tooltip = "&Settings",
        .statustip = "Configure settings specific to this product.",
        .shortcut = "",
        .min_args = 0,
        .gview = 1,
        .gscene = 1,
        .undo = 1
    },
    {
        .id = ACTION_MAKE_LAYER_CURRENT,
        .icon = "makelayercurrent",
        .command = "makelayercurrent",
        .tooltip = "&Make Layer Active",
        .statustip = "Makes the layer of a selected object the active layer",
        .shortcut = "",
        .min_args = 0,
        .gview = 1,
        .gscene = 1,
        .undo = 1
    },
    {
        .id = ACTION_LAYERS,
        .icon = "layers",
        .command = "layers",
        .tooltip = "&Layers",
        .statustip = "Manages layers and layer properties:  LAYER",
        .shortcut = "",
        .min_args = 0,
        .gview = 1,
        .gscene = 1,
        .undo = 1
    },
    {
        .id = ACTION_LAYER_SELECTOR,
        .icon = "layerselector",
        .command = "layerselector",
        .tooltip = "&Layer Selector",
        .statustip = "Dropdown selector for changing the current layer",
        .shortcut = "",
        .min_args = 0,
        .gview = 1,
        .gscene = 1,
        .undo = 1
    },
    {
        .id = ACTION_LAYER_PREVIOUS,
        .icon = "layerprevious",
        .command = "layerprevious",
        .tooltip = "&Layer Previous",
        .statustip = "Restores the previous layer settings: LAYERP",
        .shortcut = "",
        .min_args = 0,
        .gview = 1,
        .gscene = 1,
        .undo = 1
    },
    {
        .id = ACTION_COLOR_SELECTOR,
        .icon = "colorselector",
        .command = "colorselector",
        .tooltip = "&Color Selector",
        .statustip = "Dropdown selector for changing the current thread color",
        .shortcut = "",
        .min_args = 0,
        .gview = 1,
        .gscene = 1,
        .undo = 1
    },
    {
        .id = ACTION_LINE_TYPE_SELECTOR,
        .icon = "linetypeselector",
        .command = "linetypeselector",
        .tooltip = "&Stitchtype Selector",
        .statustip = "Dropdown selector for changing the current stitch type",
        .shortcut = "",
        .min_args = 0,
        .gview = 1,
        .gscene = 1,
        .undo = 1
    },
    {
        .id = ACTION_LINE_WEIGHT_SELECTOR,
        .icon = "lineweightselector",
        .command = "lineweightselector",
        .tooltip = "&Threadweight Selector",
        .statustip = "Dropdown selector for changing the current thread weight",
        .shortcut = "",
        .min_args = 0,
        .gview = 1,
        .gscene = 1,
        .undo = 1
    },
    {
        .id = ACTION_HIDE_ALL_LAYERS,
        .icon = "hidealllayers",
        .command = "hidealllayers",
        .tooltip = "&Hide All Layers",
        .statustip = "Turns the visibility off for all layers in the current drawing:  HIDEALL",
        .shortcut = "",
        .min_args = 0,
        .gview = 1,
        .gscene = 1,
        .undo = 1
    },
    {
        .id = ACTION_SHOW_ALL_LAYERS,
        .icon = "showalllayers",
        .command = "showalllayers",
        .tooltip = "&Show All Layers",
        .statustip = "Turns the visibility on for all layers in the current drawing:  SHOWALL",
        .shortcut = "",
        .min_args = 0,
        .gview = 1,
        .gscene = 1,
        .undo = 1
    },
    {
        .id = ACTION_FREEZE_ALL_LAYERS,
        .icon = "freezealllayers",
        .command = "freezealllayers",
        .tooltip = "&Freeze All Layers",
        .statustip = "Freezes all layers in the current drawing:  FREEZEALL",
        .shortcut = "",
        .min_args = 0,
        .gview = 1,
        .gscene = 1,
        .undo = 1
    },
    {
        .id = ACTION_THAW_ALL_LAYERS,
        .icon = "thawlllayers",
        .command = "thawalllayers",
        .tooltip = "&Thaw All Layers",
        .statustip = "Thaws all layers in the current drawing:  THAWALL",
        .shortcut = "",
        .min_args = 0,
        .gview = 1,
        .gscene = 1,
        .undo = 1
    },
    {
        .id = ACTION_LOCK_ALL_LAYERS,
        .icon = "lockalllayers",
        .command = "lockalllayers",
        .tooltip = "&Lock All Layers",
        .statustip = "Locks all layers in the current drawing:  LOCKALL",
        .shortcut = "",
        .min_args = 0,
        .gview = 1,
        .gscene = 1,
        .undo = 1
    },
    {
        .id = ACTION_UNLOCK_ALL_LAYERS,
        .icon = "unlockalllayers",
        .command = "unlockalllayers",
        .tooltip = "&Unlock All Layers",
        .statustip = "Unlocks all layers in the current drawing:  UNLOCKALL",
        .shortcut = "",
        .min_args = 0,
        .gview = 1,
        .gscene = 1,
        .undo = 1
    },
    {
        .id = ACTION_TEXT_BOLD,
        .icon = "textbold",
        .command = "textbold",
        .tooltip = "&Bold Text",
        .statustip = "Sets text to be bold.",
        .shortcut = "",
        .min_args = 0,
        .gview = 1,
        .gscene = 1,
        .undo = 1
    },
    {
        .id = ACTION_TEXT_ITALIC,
        .icon = "textitalic",
        .command = "textitalic",
        .tooltip = "&Italic Text",
        .statustip = "Sets text to be italic.",
        .shortcut = "",
        .min_args = 0,
        .gview = 1,
        .gscene = 1,
        .undo = 1
    },
    {
        .id = ACTION_TEXT_UNDERLINE,
        .icon = "textunderline",
        .command = "textunderline",
        .tooltip = "&Underline Text",
        .statustip = "Sets text to be underlined.",
        .shortcut = "",
        .min_args = 0,
        .gview = 1,
        .gscene = 1,
        .undo = 1
    },
    {
        .id = ACTION_TEXT_STRIKEOUT,
        .icon = "textstrikeout",
        .command = "textstrikeout",
        .tooltip = "&StrikeOut Text",
        .statustip = "Sets text to be striked out.",
        .shortcut = "",
        .min_args = 0,
        .gview = 1,
        .gscene = 1,
        .undo = 1
    },
    {
        .id = ACTION_TEXT_OVERLINE,
        .icon = "textoverline",
        .command = "textoverline",
        .tooltip = "&Overline Text",
        .statustip = "Sets text to be overlined.",
        .shortcut = "",
        .min_args = 0,
        .gview = 1,
        .gscene = 1,
        .undo = 1
    },
    {
        .id = ACTION_ZOOM_REAL_TIME,
        .icon = "zoomrealtime",
        .command = "zoomrealtime",
        .tooltip = "Zoom &Realtime",
        .statustip = "Zooms to increase or decrease the apparent size of objects in the current viewport.",
        .shortcut = "",
        .min_args = 0,
        .gview = 1,
        .gscene = 1,
        .undo = 1
    },
    {
        .id = ACTION_ZOOM_PREVIOUS,
        .icon = "zoomprevious",
        .command = "zoomprevious",
        .tooltip = "Zoom &Previous",
        .statustip = "Zooms to display the previous view.",
        .shortcut = "",
        .min_args = 0,
        .gview = 1,
        .gscene = 1,
        .undo = 1
    },
    {
        .id = ACTION_ZOOM_WINDOW,
        .icon = "zoomwindow",
        .command = "zoomwindow",
        .tooltip = "Zoom &Window",
        .statustip = "Zooms to display an area specified by a rectangular window.",
        .shortcut = "",
        .min_args = 0,
        .gview = 1,
        .gscene = 1,
        .undo = 1
    },
    {
        .id = ACTION_ZOOM_DYNAMIC,
        .icon = "zoomdynamic",
        .command = "zoomdynamic",
        .tooltip = "Zoom &Dynamic",
        .statustip = "Zooms to display the generated portion of the drawing.",
        .shortcut = "",
        .min_args = 0,
        .gview = 1,
        .gscene = 1,
        .undo = 1
    },
    {
        .id = ACTION_ZOOM_SCALE,
        .icon = "zoomscale",
        .command = "zoomscale",
        .tooltip = "Zoom &Scale",
        .statustip = "Zooms the display using a specified scale factor.",
        .shortcut = "",
        .min_args = 0,
        .gview = 1,
        .gscene = 1,
        .undo = 1
    },
    {
        .id = ACTION_ZOOM_CENTER,
        .icon = "zoomcenter",
        .command = "zoomcenter",
        .tooltip = "Zoom &Center",
        .statustip = "Zooms to display a view specified by a center point and magnification or height.",
        .shortcut = "",
        .min_args = 0,
        .gview = 1,
        .gscene = 1,
        .undo = 1
    },
    {
        .id = ACTION_ZOOM_IN,
        .icon = "zoomin",
        .command = "zoomin",
        .tooltip = "Zoom &In",
        .statustip = "Zooms to increase the apparent size of objects: ZOOMIN",
        .shortcut = "",
        .min_args = 0,
        .gview = 1,
        .gscene = 1,
        .undo = 1
    },
    {
        .id = ACTION_ZOOM_OUT,
        .icon = "zoomout",
        .command = "zoomout",
        .tooltip = "Zoom &Out",
        .statustip = "Zooms to decrease the apparent size of objects: ZOOMOUT",
        .shortcut = "",
        .min_args = 0,
        .gview = 1,
        .gscene = 1,
        .undo = 1
    },
    {
        .id = ACTION_ZOOM_SELECTED,
        .icon = "zoomselected",
        .command = "zoomselected",
        .tooltip = "Zoom Selec&ted",
        .statustip = "Zooms to display the selected objects.",
        .shortcut = "",
        .min_args = 0,
        .gview = 1,
        .gscene = 1,
        .undo = 1
    },
    {
        .id = ACTION_ZOOM_ALL,
        .icon = "zoomall",
        .command = "zoomall",
        .tooltip = "Zoom &All",
        .statustip = "Zooms to display the drawing extents or the grid limits.",
        .shortcut = "",
        .min_args = 0,
        .gview = 1,
        .gscene = 1,
        .undo = 1
    },
    {
        .id = ACTION_ZOOM_EXTENTS,
        .icon = "zoomextents",
        .command = "zoomextents",
        .tooltip = "Zoom &Extents",
        .statustip = "Zooms to display the drawing extents: ZOOMEXTENTS",
        .shortcut = "",
        .min_args = 0,
        .gview = 1,
        .gscene = 1,
        .undo = 1
    },
    {
        .id = ACTION_PAN_REAL_TIME,
        .icon = "panrealtime",
        .command = "panrealtime",
        .tooltip = "&Pan Realtime",
        .statustip = "Moves the view in the current viewport.",
        .shortcut = "",
        .min_args = 0,
        .gview = 1,
        .gscene = 1,
        .undo = 1
    },
    {
        .id = ACTION_PAN_POINT,
        .icon = "panpoint",
        .command = "panpoint",
        .tooltip = "&Pan Point",
        .statustip = "Moves the view by the specified distance.",
        .shortcut = "",
        .min_args = 0,
        .gview = 1,
        .gscene = 1,
        .undo = 1
    },
    {
        .id = ACTION_PAN_LEFT,
        .icon = "panleft",
        .command = "panleft",
        .tooltip = "&Pan Left",
        .statustip = "Moves the view to the left: PANLEFT",
        .shortcut = "",
        .min_args = 0,
        .gview = 1,
        .gscene = 1,
        .undo = 1
    },
    {
        .id = ACTION_PAN_RIGHT,
        .icon = "panright",
        .command = "panright",
        .tooltip = "&Pan Right",
        .statustip = "Moves the view to the right: PANRIGHT",
        .shortcut = "",
        .min_args = 0,
        .gview = 1,
        .gscene = 1,
        .undo = 1
    },
    {
        .id = ACTION_PAN_UP,
        .icon = "panup",
        .command = "panup",
        .tooltip = "&Pan Up",
        .statustip = "Moves the view up: PANUP",
        .shortcut = "",
        .min_args = 0,
        .gview = 1,
        .gscene = 1,
        .undo = 1
    },
    {
        .id = ACTION_PAN_DOWN,
        .icon = "pandown",
        .command = "pandown",
        .tooltip = "&Pan Down",
        .statustip = "Moves the view down: PANDOWN",
        .shortcut = "",
        .min_args = 0,
        .gview = 1,
        .gscene = 1,
        .undo = 1
    },
    {
        .id = ACTION_DAY,
        .icon = "day",
        .command = "day",
        .tooltip = "&Day",
        .statustip = "Updates the current view using day vision settings: DAY.",
        .shortcut = "",
        .min_args = 0,
        .gview = 1,
        .gscene = 1,
        .undo = 1
    },
    {
        .id = ACTION_NIGHT,
        .icon = "night_vision",
        .command = "night",
        .tooltip = "&Night",
        .statustip = "Updates the current view using night vision settings: NIGHT",
        .shortcut = "",
        .min_args = 0,
        .gview = 1,
        .gscene = 1,
        .undo = 1
    },
    {
        /* todo */
        .id = ACTION_SPELLCHECK,
        .icon = "spellcheck",
        .command = "spellcheck",
        .tooltip = "",
        .statustip = "",
        .shortcut = "",
        .min_args = 0,
        .gview = 1,
        .gscene = 1,
        .undo = 1
    },
    {
        .id = ACTION_QUICKSELECT,
        .icon = "quickselect",
        .command = "quickselect",
        .tooltip = "",
        .statustip = "",
        .shortcut = "",
        .min_args = 0,
        .gview = 1,
        .gscene = 1,
        .undo = 1
    },
    {
        /* .alias = "C", */
        .id = ACTION_ADD_CIRCLE,
        .icon = "circle",
        .command = "circle",
        .tooltip = "Circle",
        .statustip = "Creates a circle: CIRCLE",
        .shortcut = "",
        .min_args = 0,
        .gview = 1,
        .gscene = 1,
        .undo = 1
    },
    {
        /*
        menu = "Draw",
        toolbar = "Draw",
        */
        .id = ACTION_ADD_HEART,
        .icon = "heart",
        .command = "heart",
        .tooltip = "&Heart",
        .statustip = "Creates a heart: HEART",
        .shortcut = "",
        .min_args = 0,
        .gview = 1,
        .gscene = 1,
        .undo = 1
    },
    {
        /*
        menu = "Tools",
        toolbar = "Inquiry",
        */
        .id = ACTION_DISTANCE,
        .icon = "distance",
        .command = "distance",
        .tooltip = "&Distance",
        .statustip = "Measures the distance and angle between two points: DIST",
        .shortcut = "",
        .min_args = 0,
        .gview = 1,
        .gscene = 1,
        .undo = 1
    },
    {
        /*
        menu = "Draw",
        toolbar = "Draw",
        */
        .id = ACTION_ADD_DOLPHIN,
        .icon = "dolphin",
        .command = "dolphin",
        .tooltip = "&Dolphin",
        .statustip = "Adds a dolphin design to the vector layer: DOLPHIN.",
        .shortcut = "",
        .min_args = 0,
        .gview = 1,
        .gscene = 1,
        .undo = 1
    },
    {
        /*
        menu = "Draw",
        toolbar = "Draw",
        */
        .id = ACTION_ADD_ELLIPSE,
        .icon = "ellipse",
        .command = "ellipse",
        .tooltip = "Ellipse",
        .statustip = "Creates a ellipse: ELLIPSE",
        .shortcut = "",
        .min_args = 0,
        .gview = 1,
        .gscene = 1,
        .undo = 1
    },
    {
        /*
        menu = "Modify",
        toolbar = "Modify",
        */
        .id = ACTION_DELETE,
        .icon = "erase",
        .command = "erase",
        .tooltip = "D&elete",
        .statustip = "Removes objects from a drawing: DELETE.",
        .shortcut = "",
        .min_args = 0,
        .gview = 1,
        .gscene = 1,
        .undo = 1
    },
    {
        /*
        menu = "Draw",
        toolbar = "Draw",
        */
        .id = ACTION_ADD_LINE,
        .icon = "line",
        .command = "line",
        .tooltip = "&Line",
        .statustip = "Creates straight line segments: LINE.",
        .shortcut = "",
        .min_args = 0,
        .gview = 1,
        .gscene = 1,
        .undo = 1
    },
    {
        /*
        menu = "Tools",
        toolbar = "Inquiry",
        */
        .id = ACTION_LOCATE_POINT,
        .icon = "locatepoint",
        .command = "locatepoint",
        .tooltip = "&Locate Point",
        .statustip = "Displays the coordinate values of a location: ID.",
        .shortcut = "",
        .min_args = 0,
        .gview = 1,
        .gscene = 1,
        .undo = 1
    },
    {
        /*
        menu = "Modify",
        toolbar = "Modify"
        */
        .id = ACTION_MOVE,
        .icon = "move",
        .command = "move",
        .tooltip = "&Move",
        .statustip = "Displaces objects a specified distance in a specified direction: MOVE.",
        .shortcut = "",
        .min_args = 0,
        .gview = 1,
        .gscene = 1,
        .undo = 1
    },
    {
        /*
        menu = "Draw",
        toolbar = "Draw"
        */
        .id = ACTION_ADD_PATH,
        .icon = "path",
        .command = "addpath",
        .tooltip = "&Path",
        .statustip = "Creates a 2D path: PATH.",
        .shortcut = "",
        .min_args = 0,
        .gview = 1,
        .gscene = 1,
        .undo = 1
    },
    {
        .id = ACTION_PLATFORM,
        .icon = "platform",
        .command = "platform",
        .tooltip = "&Platform",
        .statustip = "List which platform is in use: PLATFORM.",
        .shortcut = "",
        .min_args = 0,
        .gview = 1,
        .gscene = 1,
        .undo = 1
    },
    {
        /*
        menu = "Draw",
        toolbar = "Draw",
        */
        .id = ACTION_ADD_POINT,
        .icon = "point",
        .command = "addpoint",
        .tooltip = "&Point",
        .statustip = "Creates multiple points: POINT.",
        .shortcut = "",
        .min_args = 0,
        .gview = 1,
        .gscene = 1,
        .undo = 1
    },
    {
        /*
        menu = "Draw",
        toolbar = "Draw",
        */
        .id = ACTION_ADD_POLYGON,
        .icon = "polygon",
        .command = "addpolygon",
        .tooltip = "Pol&ygon",
        .statustip = "Creates a regular polygon: POLYGON.",
        .shortcut = "",
        .min_args = 0,
        .gview = 1,
        .gscene = 1,
        .undo = 1
    },
    {
        /*
        .menu = "Draw",
        .toolbar = "Draw",
        */
        .id = ACTION_ADD_POLYLINE,
        .icon = "polyline",
        .command = "addpolyline",
        .tooltip = "&Polyline",
        .statustip = "Creates a 2D polyline: PLINE.",
        .shortcut = "",
        .min_args = 0,
        .gview = 1,
        .gscene = 1,
        .undo = 1
    },
    {
        /*
        menu = "Dimension",
        toolbar = "Dimension",
        */
        .id = ACTION_QUICKLEADER,
        .icon = "quickleader",
        .command = "addquickleader",
        .tooltip = "&QuickLeader",
        .statustip = "Creates a leader and annotation: QUICKLEADER.",
        .shortcut = "",
        .min_args = 0,
        .gview = 1,
        .gscene = 1,
        .undo = 1
    },
    {
        /*
        menu = "Draw",
        toolbar = "Draw",
        */
        .id = ACTION_ADD_RECTANGLE,
        .icon = "rectangle",
        .command = "rectangle",
        .tooltip = "&Rectangle",
        .statustip = "Creates a rectangular polyline: RECTANGLE.",
        .shortcut = "",
        .min_args = 0,
        .gview = 1,
        .gscene = 1,
        .undo = 1
    },
    {
        /*
        menu = "Sandbox"
        toolbar = "Sandbox"
        */
        .id = ACTION_RGB,
        .icon =  "rgb",
        .command = "rgb",
        .tooltip =  "&RGB",
        .statustip = "Updates the current view colors: RGB.",
        .shortcut = "",
        .min_args = 0,
        .gview = 1,
        .gscene = 1,
        .undo = 1
    },
    {
        /*
        menu = "Modify",
        toolbar = "Modify",
        */
        .id = ACTION_ROTATE,
        .icon = "rotate",
        .command = "rotate",
        .tooltip = "&Rotate",
        .statustip = "Rotates objects about a base point: ROTATE.",
        .shortcut = "",
        .min_args = 0,
        .gview = 1,
        .gscene = 1,
        .undo = 1
    },
    {
        /*
        menu = "Sandbox",
        toolbar = "Sandbox",
        */
        .id = ACTION_SANDBOX,
        .icon = "sandbox",
        .command = "sandbox",
        .tooltip = "Sandbox",
        .statustip = "A sandbox to play in: SANDBOX.",
        .shortcut = "",
        .min_args = 0,
        .gview = 1,
        .gscene = 1,
        .undo = 1
    },
    {
        /*
        menu = "Modify",
        toolbar = "Modify",
        */
        .id = ACTION_SCALE,
        .icon = "scale",
        .command = "scale",
        .tooltip = "Sca&le",
        .statustip = "Enlarges or reduces objects proportionally in the X, Y, and Z directions: SCALE.",
        .shortcut = "",
        .min_args = 0,
        .gview = 1,
        .gscene = 1,
        .undo = 1
    },
    {
        .id = ACTION_SELECT_ALL,
        .icon = "selectall",
        .command = "selectall",
        .tooltip = "&Select All",
        .statustip = "Selects all objects: SELECTALL.",
        .shortcut = "",
        .min_args = 0,
        .gview = 1,
        .gscene = 1,
        .undo = 1
    },
    {
        /*
        menu = "Draw",
        toolbar = "Draw",
        */
        .id = ACTION_SINGLE_LINE_TEXT,
        .icon = "single-line-text",
        .command = "singlelinetext",
        .tooltip = "&Single Line Text",
        .statustip = "Creates single-line text objects: TEXT",
        .shortcut = "",
        .min_args = 0,
        .gview = 1,
        .gscene = 1,
        .undo = 1
    },
    {
        /*
        menu = "Draw",
        toolbar = "Draw",
        */
        .id = ACTION_ADD_SNOWFLAKE,
        .icon = "snowflake",
        .command = "snowflake",
        .tooltip = "&Snowflake",
        .statustip = "Creates a snowflake: SNOWFLAKE",
        .shortcut = "",
        .min_args = 0,
        .gview = 1,
        .gscene = 1,
        .undo = 1
    },
    {
        /*
        menu "Draw",
        toolbar "Draw",
        */
        .id = ACTION_ADD_STAR,
        .icon = "star",
        .command = "star",
        .tooltip = "&Star",
        .statustip = "Creates a star: STAR",
        .shortcut = "",
        .min_args = 0,
        .gview = 1,
        .gscene = 1,
        .undo = 1
    },
    {
        .id = ACTION_SYSWINDOWS,
        .icon = "syswindows",
        .command = "syswindows",
        .tooltip = "&SysWindows",
        .statustip = "Arrange the windows: SYSWINDOWS",
        .shortcut = "",
        .min_args = 0,
        .gview = 1,
        .gscene = 1,
        .undo = 1
    },
    {
        .id = ACTION_TREBLECLEF,
        .icon = "trebleclef",
        .command = "trebleclef",
        .tooltip = "Treble Clef",
        .statustip = "Adds a treble clef design to the vector layer.",
        .shortcut = "",
        .min_args = 0,
        .gview = 1,
        .gscene = 1,
        .undo = 1
    },
    {
        .id = ACTION_EXIT,
        .icon = "exit",
        .command = "exit",
        .tooltip = "E&xit",
        .statustip = "Exit the application: EXIT.",
        .shortcut = "",
        .min_args = 0,
        .gview = 1,
        .gscene = 1,
        .undo = 1
    },
    {
        .id = ACTION_TEST,
        .icon = "test",
        .command = "test",
        .tooltip = "Coverage Test",
        .statustip = "Run all commands at least once to test that nothing crashes.",
        .shortcut = "",
        .min_args = 0,
        .gview = 1,
        .gscene = 1,
        .undo = 1
    },
    {
        .id = ACTION_SLEEP,
        .icon = "sleep",
        .command = "sleep",
        .tooltip = "Sleep for 1 second.",
        .statustip = "Sleep for 1 second.",
        .shortcut = "",
        .min_args = 0,
        .gview = 0,
        .gscene = 0,
        .undo = 0
    },
    {
        .id = ACTION_END,
        .icon = "END",
        .command = "END",
        .tooltip = "END",
        .statustip = "END",
        .shortcut = "END",
        .min_args = 0,
        .gview = 1,
        .gscene = 1,
        .undo = 1
    }
};
