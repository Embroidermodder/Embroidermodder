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
ActionData action_table[MAX_ACTIONS] = {
    {
        .id = ACTION_ABOUT,
        .icon = "about",
        .command = "about",
        .tooltip = "&About Embroidermodder 2",
        .statustip = "Displays information about this product: ABOUT.",
        .shortcut = ""
    },
    {
        .id = ACTION_TREBLECLEF,
        .icon = "trebleclef",
        .command = "trebleclef",
        .tooltip = "Treble Clef",
        .statustip = "Adds a treble clef design to the vector layer.",
        .shortcut = ""
    },
    {
        .id = ACTION_DO_NOTHING,
        .icon = "donothing",
        .command = "donothing",
        .tooltip = "&Do Nothing",
        .statustip = "Does Nothing",
        .shortcut = ""
    },
    {
        .id = ACTION_NEW,
        .icon = "new",
        .command = "new",
        .tooltip = "&New",
        .statustip = "Create a new file: NEW.",
        .shortcut = "Ctrl+N"
    },
    {
        .id = ACTION_OPEN,
        .icon = "open",
        .command = "open",
        .tooltip = "&Open",
        .statustip = "Open an existing file: OPEN",
        .shortcut = "Ctrl+O"
    },
    {
        .id = ACTION_SAVE,
        .icon = "save",
        .command = "save",
        .tooltip = "&Save",
        .statustip = "Save the design to disk.",
        .shortcut = "Ctrl+S"
    },
    {
        .id = ACTION_SAVEAS,
        .icon = "saveas",
        .command = "saveas",
        .tooltip = "Save &As",
        .statustip = "Save the design under a new name and type.",
        .shortcut = "Ctrl+Shift+S"
    },
    {
        .id = ACTION_PRINT,
        .icon = "print",
        .command = "print",
        .tooltip = "&Print",
        .statustip = "Print the design.",
        .shortcut = "Ctrl+P"
    },
    {
        .id = ACTION_DESIGN_DETAILS,
        .icon = "designdetails",
        .command = "designdetails",
        .tooltip = "&Details",
        .statustip = "Details of the current design.",
        .shortcut = "Ctrl+D"
    },
    {
        .id = ACTION_EXIT,
        .icon = "exit",
        .command = "exit",
        .tooltip = "E&xit",
        .statustip = "Exit the application: EXIT",
        .shortcut = "Ctrl+Q"
    },
    {
        .id = ACTION_CUT,
        .icon = "cut",
        .command = "cut",
        .tooltip = "Cu&t",
        .statustip = "Cut the current selection's contents to the clipboard.",
        .shortcut = "Ctrl+X"
    },
    {
        .id = ACTION_COPY,
        .icon = "copy",
        .command = "copy",
        .tooltip = "&Copy",
        .statustip = "Copy the current selection's contents to the clipboard.",
        .shortcut = "Ctrl+C"
    },
    {
        .id = ACTION_PASTE,
        .icon = "paste",
        .command = "paste",
        .tooltip = "&Paste",
        .statustip = "Paste the clipboard's contents into the current selection.",
        .shortcut = "Ctrl+V"
    },
    {
        .id = ACTION_UNDO,
        .icon = "undo",
        .command = "undo",
        .tooltip = "&Undo",
        .statustip = "Reverses the most recent action: UNDO",
        .shortcut = "Ctrl+Z"
    },
    {
        .id = ACTION_REDO,
        .icon = "redo",
        .command = "redo",
        .tooltip = "&Redo",
        .statustip = "Reverses the effects of the previous undo action: REDO",
        .shortcut = "Ctrl+Shift+Z"
    },
    {
        .id = ACTION_WINDOW_CLOSE,
        .icon = "windowclose",
        .command = "window close",
        .tooltip = "Cl&ose",
        .statustip = "Close the active window: CLOSE",
        .shortcut = "Ctrl+W"
    },
    {
        .id = ACTION_WINDOW_CLOSE_ALL,
        .icon = "windowcloseall",
        .command = "window closeall",
        .tooltip = "Close &All",
        .statustip = "Close all the windows: CLOSEALL",
        .shortcut = ""
    },
    {
        .id = ACTION_WINDOW_CASCADE,
        .icon = "windowcascade",
        .command = "window cascade",
        .tooltip = "&Cascade",
        .statustip = "Cascade the windows: CASCADE",
        .shortcut = ""
    },
    {
        .id = ACTION_WINDOW_TILE,
        .icon = "windowtile",
        .command = "window tile",
        .tooltip = "&Tile",
        .statustip = "Tile the windows: TILE",
        .shortcut = ""
    },
    {
        .id = ACTION_WINDOW_NEXT,
        .icon = "windownext",
        .command = "window next",
        .tooltip = "Ne&xt",
        .statustip = "Move the focus to the next window: NEXT",
        .shortcut = "Ctrl+Tab"
    },
    {
        .id = ACTION_WINDOW_PREVIOUS,
        .icon = "windowprevious",
        .command = "window previous",
        .tooltip = "Pre&vious",
        .statustip = "Move the focus to the previous window: PREVIOUS",
        .shortcut = "Ctrl+Shift+Tab"
    },
    {
        .id = ACTION_HELP,
        .icon = "help",
        .command = "help",
        .tooltip = "&Help",
        .statustip = "Displays the help file: HELP.",
        .shortcut = ""
    },
    {
        .id = ACTION_CHANGELOG,
        .icon = "changelog",
        .command = "changelog",
        .tooltip = "&Changelog",
        .statustip = "Describes new features in this product.",
        .shortcut = ""
    },
    {
        .id = ACTION_TIP_OF_THE_DAY,
        .icon = "tipoftheday",
        .command = "tips",
        .tooltip = "&Tip Of The Day",
        .statustip = "Displays a dialog with useful tips: TIPS",
        .shortcut = ""
    },
    {
        .id = ACTION_WHATS_THIS,
        .icon = "whatsthis",
        .command = "whatsthis",
        .tooltip = "&What's This?",
        .statustip = "What's This? Context Help!",
        .shortcut = ""
    },
    {
        .id = ACTION_ICON16,
        .icon = "icon16",
        .command = "icon 16",
        .tooltip = "Icon&16",
        .statustip = "Sets the toolbar icon size to 16x16: ICON16",
        .shortcut = ""
    },
    {
        .id = ACTION_ICON24,
        .icon = "icon24",
        .command = "icon 24",
        .tooltip = "Icon&24",
        .statustip = "Sets the toolbar icon size to 24x24: ICON24.",
        .shortcut = ""
    },
    {
        .id = ACTION_ICON32,
        .icon = "icon32",
        .command = "icon 32",
        .tooltip = "Icon&32",
        .statustip = "Sets the toolbar icon size to 32x32: ICON32",
        .shortcut = ""
    },
    {
        .id = ACTION_ICON48,
        .icon = "icon48",
        .command = "icon 48",
        .tooltip = "Icon&48",
        .statustip = "Sets the toolbar icon size to 48x48: ICON48",
        .shortcut = ""
    },
    {
        .id = ACTION_ICON64,
        .icon = "icon64",
        .command = "icon 64",
        .tooltip = "Icon&64",
        .statustip = "Sets the toolbar icon size to 64x64: ICON64",
        .shortcut = ""
    },
    {
        .id = ACTION_ICON128,
        .icon = "icon128",
        .command = "icon 128",
        .tooltip = "Icon12&8",
        .statustip = "Sets the toolbar icon size to 128x128: ICON128",
        .shortcut = ""
    },
    {
        .id = ACTION_SETTINGS_DIALOG,
        .icon = "settingsdialog",
        .command = "settingsdialog",
        .tooltip = "&Settings",
        .statustip = "Configure settings specific to this product.",
        .shortcut = ""
    },
    {
        .id = ACTION_MAKE_LAYER_CURRENT,
        .icon = "makelayercurrent",
        .command = "makelayercurrent",
        .tooltip = "&Make Layer Active",
        .statustip = "Makes the layer of a selected object the active layer",
        .shortcut = ""
    },
    {
        .id = ACTION_LAYERS,
        .icon = "layers",
        .command = "layers",
        .tooltip = "&Layers",
        .statustip = "Manages layers and layer properties:  LAYER",
        .shortcut = ""
    },
    {
        .id = ACTION_LAYER_SELECTOR,
        .icon = "layerselector",
        .command = "layerselector",
        .tooltip = "&Layer Selector",
        .statustip = "Dropdown selector for changing the current layer",
        .shortcut = ""
    },
    {
        .id = ACTION_LAYER_PREVIOUS,
        .icon = "layerprevious",
        .command = "layerprevious",
        .tooltip = "&Layer Previous",
        .statustip = "Restores the previous layer settings: LAYERP",
        .shortcut = ""
    },
    {
        .id = ACTION_COLOR_SELECTOR,
        .icon = "colorselector",
        .command = "colorselector",
        .tooltip = "&Color Selector",
        .statustip = "Dropdown selector for changing the current thread color",
        .shortcut = ""
    },
    {
        .id = ACTION_LINE_TYPE_SELECTOR,
        .icon = "linetypeselector",
        .command = "linetypeselector",
        .tooltip = "&Stitchtype Selector",
        .statustip = "Dropdown selector for changing the current stitch type",
        .shortcut = ""
    },
    {
        .id = ACTION_LINE_WEIGHT_SELECTOR,
        .icon = "lineweightselector",
        .command = "lineweightselector",
        .tooltip = "&Threadweight Selector",
        .statustip = "Dropdown selector for changing the current thread weight",
        .shortcut = ""
    },
    {
        .id = ACTION_HIDE_ALL_LAYERS,
        .icon = "hidealllayers",
        .command = "hidealllayers",
        .tooltip = "&Hide All Layers",
        .statustip = "Turns the visibility off for all layers in the current drawing:  HIDEALL",
        .shortcut = ""
    },
    {
        .id = ACTION_SHOW_ALL_LAYERS,
        .icon = "showalllayers",
        .command = "showalllayers",
        .tooltip = "&Show All Layers",
        .statustip = "Turns the visibility on for all layers in the current drawing:  SHOWALL",
        .shortcut = ""
    },
    {
        .id = ACTION_FREEZE_ALL_LAYERS,
        .icon = "freezealllayers",
        .command = "freezealllayers",
        .tooltip = "&Freeze All Layers",
        .statustip = "Freezes all layers in the current drawing:  FREEZEALL",
        .shortcut = ""
    },
    {
        .id = ACTION_THAW_ALL_LAYERS,
        .icon = "thawlllayers",
        .command = "thawalllayers",
        .tooltip = "&Thaw All Layers",
        .statustip = "Thaws all layers in the current drawing:  THAWALL",
        .shortcut = ""
    },
    {
        .id = ACTION_LOCK_ALL_LAYERS,
        .icon = "lockalllayers",
        .command = "lockalllayers",
        .tooltip = "&Lock All Layers",
        .statustip = "Locks all layers in the current drawing:  LOCKALL",
        .shortcut = ""
    },
    {
        .id = ACTION_UNLOCK_ALL_LAYERS,
        .icon = "unlockalllayers",
        .command = "unlockalllayers",
        .tooltip = "&Unlock All Layers",
        .statustip = "Unlocks all layers in the current drawing:  UNLOCKALL",
        .shortcut = ""
    },
    {
        .id = ACTION_TEXT_BOLD,
        .icon = "textbold",
        .command = "text bold",
        .tooltip = "&Bold Text",
        .statustip = "Sets text to be bold.",
        .shortcut = ""
    },
    {
        .id = ACTION_TEXT_ITALIC,
        .icon = "textitalic",
        .command = "text italic",
        .tooltip = "&Italic Text",
        .statustip = "Sets text to be italic.",
        .shortcut = ""
    },
    {
        .id = ACTION_TEXT_UNDERLINE,
        .icon = "textunderline",
        .command = "text underline",
        .tooltip = "&Underline Text",
        .statustip = "Sets text to be underlined.",
        .shortcut = ""
    },
    {
        .id = ACTION_TEXT_STRIKEOUT,
        .icon = "textstrikeout",
        .command = "text strikeout",
        .tooltip = "&StrikeOut Text",
        .statustip = "Sets text to be striked out.",
        .shortcut = ""
    },
    {
        .id = ACTION_TEXT_OVERLINE,
        .icon = "textoverline",
        .command = "text overline",
        .tooltip = "&Overline Text",
        .statustip = "Sets text to be overlined.",
        .shortcut = ""
    },
    {
        .id = ACTION_ZOOM_REAL_TIME,
        .icon = "zoomrealtime",
        .command = "zoom realtime",
        .tooltip = "Zoom &Realtime",
        .statustip = "Zooms to increase or decrease the apparent size of objects in the current viewport.",
        .shortcut = ""
    },
    {
        .id = ACTION_ZOOM_PREVIOUS,
        .icon = "zoomprevious",
        .command = "zoom previous",
        .tooltip = "Zoom &Previous",
        .statustip = "Zooms to display the previous view.",
        .shortcut = ""
    },
    {
        .id = ACTION_ZOOM_WINDOW,
        .icon = "zoomwindow",
        .command = "zoom window",
        .tooltip = "Zoom &Window",
        .statustip = "Zooms to display an area specified by a rectangular window.",
        .shortcut = ""
    },
    {
        .id = ACTION_ZOOM_DYNAMIC,
        .icon = "zoomdynamic",
        .command = "zoom dynamic",
        .tooltip = "Zoom &Dynamic",
        .statustip = "Zooms to display the generated portion of the drawing.",
        .shortcut = ""
    },
    {
        .id = ACTION_ZOOM_SCALE,
        .icon = "zoomscale",
        .command = "zoom scale",
        .tooltip = "Zoom &Scale",
        .statustip = "Zooms the display using a specified scale factor.",
        .shortcut = ""
    },
    {
        .id = ACTION_ZOOM_CENTER,
        .icon = "zoomcenter",
        .command = "zoom center",
        .tooltip = "Zoom &Center",
        .statustip = "Zooms to display a view specified by a center point and magnification or height.",
        .shortcut = ""
    },
    {
        .id = ACTION_ZOOM_IN,
        .icon = "zoomin",
        .command = "zoom in",
        .tooltip = "Zoom &In",
        .statustip = "Zooms to increase the apparent size of objects: ZOOMIN",
        .shortcut = ""
    },
    {
        .id = ACTION_ZOOM_OUT,
        .icon = "zoomout",
        .command = "zoom out",
        .tooltip = "Zoom &Out",
        .statustip = "Zooms to decrease the apparent size of objects: ZOOMOUT",
        .shortcut = ""
    },
    {
        .id = ACTION_ZOOM_SELECTED,
        .icon = "zoomselected",
        .command = "zoom selected",
        .tooltip = "Zoom Selec&ted",
        .statustip = "Zooms to display the selected objects.",
        .shortcut = ""
    },
    {
        .id = ACTION_ZOOM_ALL,
        .icon = "zoomall",
        .command = "zoom all",
        .tooltip = "Zoom &All",
        .statustip = "Zooms to display the drawing extents or the grid limits.",
        .shortcut = ""
    },
    {
        .id = ACTION_ZOOM_EXTENTS,
        .icon = "zoomextents",
        .command = "zoom extents",
        .tooltip = "Zoom &Extents",
        .statustip = "Zooms to display the drawing extents: ZOOMEXTENTS",
        .shortcut = ""
    },
    {
        .id = ACTION_PAN_REAL_TIME,
        .icon = "panrealtime",
        .command = "pan realtime",
        .tooltip = "&Pan Realtime",
        .statustip = "Moves the view in the current viewport.",
        .shortcut = ""
    },
    {
        .id = ACTION_PAN_POINT,
        .icon = "panpoint",
        .command = "pan point",
        .tooltip = "&Pan Point",
        .statustip = "Moves the view by the specified distance.",
        .shortcut = ""
    },
    {
        .id = ACTION_PAN_LEFT,
        .icon = "panleft",
        .command = "pan left",
        .tooltip = "&Pan Left",
        .statustip = "Moves the view to the left: PANLEFT",
        .shortcut = ""
    },
    {
        .id = ACTION_PAN_RIGHT,
        .icon = "panright",
        .command = "pan right",
        .tooltip = "&Pan Right",
        .statustip = "Moves the view to the right: PANRIGHT",
        .shortcut = ""
    },
    {
        .id = ACTION_PAN_UP,
        .icon = "panup",
        .command = "pan up",
        .tooltip = "&Pan Up",
        .statustip = "Moves the view up: PANUP",
        .shortcut = ""
    },
    {
        .id = ACTION_PAN_DOWN,
        .icon = "pandown",
        .command = "pan down",
        .tooltip = "&Pan Down",
        .statustip = "Moves the view down: PANDOWN",
        .shortcut = ""
    },
    {
        .id = ACTION_DAY,
        .icon = "day",
        .command = "day",
        .tooltip = "&Day",
        .statustip = "Updates the current view using day vision settings: DAY.",
        .shortcut = ""
    },
    {
        .id = ACTION_NIGHT,
        .icon = "night_vision",
        .command = "night",
        .tooltip = "&Night",
        .statustip = "Updates the current view using night vision settings: NIGHT",
        .shortcut = ""
    },
    {
        /* todo */
        .id = ACTION_SPELLCHECK,
        .icon = "spellcheck",
        .command = "spellcheck",
        .tooltip = "",
        .statustip = "",
        .shortcut = ""
    },
    {
        .id = ACTION_QUICKSELECT,
        .icon = "quickselect",
        .command = "quickselect",
        .tooltip = "",
        .statustip = "",
        .shortcut = ""
    },
    {
        /* .alias = "C", */
        .id = ACTION_ADD_CIRCLE,
        .icon = "circle",
        .command = "circle",
        .tooltip = "Circle",
        .statustip = "Creates a circle: CIRCLE",
        .shortcut = ""
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
        .shortcut = ""
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
        .shortcut = ""
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
        .shortcut = ""
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
        .shortcut = ""
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
        .shortcut = ""
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
        .shortcut = ""
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
        .shortcut = ""
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
        .shortcut = ""
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
        .shortcut = ""
    },
    {
        .id = ACTION_PLATFORM,
        .icon = "platform",
        .command = "platform",
        .tooltip = "&Platform",
        .statustip = "List which platform is in use: PLATFORM.",
        .shortcut = ""
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
        .shortcut = ""
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
        .shortcut = ""
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
        .shortcut = ""
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
        .shortcut = ""
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
        .shortcut = ""
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
        .shortcut = ""
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
        .shortcut = ""
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
        .shortcut = ""
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
        .shortcut = ""
    },
    {
        .id = ACTION_SELECT_ALL,
        .icon = "selectall",
        .command = "selectall",
        .tooltip = "&Select All",
        .statustip = "Selects all objects: SELECTALL.",
        .shortcut = ""
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
        .shortcut = ""
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
        .shortcut = ""
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
        .shortcut = ""
    },
    {
        .id = ACTION_SYSWINDOWS,
        .icon = "syswindows",
        .command = "syswindows",
        .tooltip = "&SysWindows",
        .statustip = "Arrange the windows: SYSWINDOWS",
        .shortcut = ""
    },
    {
        .id = ACTION_TREBLECLEF,
        .icon = "trebleclef",
        .command = "trebleclef",
        .tooltip = "Treble Clef",
        .statustip = "Adds a treble clef design to the vector layer.",
        .shortcut = ""
    },
    {
        .id = ACTION_EXIT,
        .icon = "exit",
        .command = "exit",
        .tooltip = "E&xit",
        .statustip = "Exit the application: EXIT.",
        .shortcut = ""
    },
    {
        .id = ACTION_TEST,
        .icon = "test",
        .command = "test",
        .tooltip = "Coverage Test",
        .statustip = "Run all commands at least once to test that nothing crashes.",
        .shortcut = ""
    },
    {
        .id = ACTION_SLEEP,
        .icon = "sleep",
        .command = "sleep",
        .tooltip = "Sleep for 1 second.",
        .statustip = "Sleep for 1 second.",
        .shortcut = ""
    },
    {
        .id = ACTION_END,
        .icon = "END",
        .command = "END",
        .tooltip = "END",
        .statustip = "END",
        .shortcut = "END"
    }
};
