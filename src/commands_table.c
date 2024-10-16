/*
 * Embroidermodder 2.
 *
 * Copyright 2011-2024 The Embroidermodder Team
 * Embroidermodder 2 is Open Source Software, see LICENSE.md for licensing terms.
 *
 * Read the reference manual (https://www.libembroidery.org/downloads/emrm.pdf)
 * for advice on altering this file.
 *
 * Command Data
 *
 * Shortcuts should match: https://doc.qt.io/qt-6/qkeysequence.html#standard-shortcuts
 * Apple platforms may need an additional argument like .apple_shortcut
 *
 * Long term this file will become "command_data.toml" with the format:
 *
 * # No arguments, shortcut or flags arguments because they're implicitly
 * # empty strings.
 * [about]
 * id = 0 # Will this have to be a magic number to allow for a jump table?
 * command = "about"
 * icon = "about"
 * tooltip = "&About Embroidermodder 2",
 * statustip = "Displays information about this product. Command: ABOUT.",
 * alias = "ABOUT",
 */

#include "core.h"

Command command_data[MAX_COMMANDS] = {
    {
        .id = ACTION_ABOUT,
        .command = "about",
        .arguments = "",
        .icon = "about",
        .tooltip = "&About Embroidermodder 2",
        .statustip = "Displays information about this product. Command: ABOUT.",
        .alias = "ABOUT",
        .shortcut = "",
        .flags = CONTEXT_FREE
    },
    {
        .id = -1,
        .command = "arc",
        .icon = "arc",
        .tooltip = "&Arc",
        .statustip = "Displays information about this product:  ARC",
        .alias = "ARC",
        .shortcut = "",
        .flags = REQUIRED_VIEW
    },
    {
        .id = ACTION_ALERT,
        .command = "alert",
        .arguments = "s",
        .icon = "warning",
        .tooltip = "&Alert",
        .statustip = "Creates a dialog to alert the user. Command: ALERT.",
        .alias = "ALERT",
        .shortcut = "",
        .flags = REQUIRED_VIEW
    },
    {
        .id = ACTION_ANGLE,
        .command = "angle",
        .arguments = "rrr",
        .icon = "angle",
        .tooltip = "&Angle",
        .statustip = "Calculate the angle between two lines and display it. Command: ANGLE, CALCANGLE",
        .alias = "ANGLE, CALCANGLE",
        .shortcut = "",
        .flags = REQUIRED_VIEW | CLEAR_SELECTION
    },
    {
        .id = ACTION_CIRCLE,
        .command = "circle",
        .arguments = "rrr",
        .icon = "circle",
        .tooltip = "&Circle",
        .statustip = "Creates a circle. Command: CIRCLE.",
        .alias = "C, CIRCLE",
        .shortcut = "",
        .flags = REQUIRED_VIEW
    },
    {
        .id = ACTION_CHANGELOG,
        .command = "changelog",
        .arguments = "",
        .icon = "changelog",
        .tooltip = "&Changelog",
        .statustip = "Describes new features in this product. Command: CHANGELOG.",
        .alias = "CHANGELOG",
        .shortcut = "",
        .flags = REQUIRED_VIEW
    },
    {
        .id = ACTION_CLEAR,
        .command = "clear",
        .arguments = "",
        .icon = "clear",
        .tooltip = "&Do Nothing",
        .statustip = "Does Nothing. Command: DONOTHING.",
        .alias = "DONOTHING",
        .shortcut = "",
        .flags = REQUIRED_VIEW | CLEAR_SELECTION
    },
    {
        .id = ACTION_COLOR_SELECTOR,
        .command = "colorselector",
        .arguments = "",
        .icon = "colorselector",
        .tooltip = "&Color Selector",
        .statustip = "Dropdown selector for changing the current thread color",
        .alias = "COLORSELECTOR",
        .shortcut = "",
        .flags = REQUIRED_VIEW
    },
    {
        .id = ACTION_COPY,
        .command = "copy",
        .arguments = "",
        .icon = "copy",
        .tooltip = "&Copy",
        .statustip = "Copy the current selection's contents to the clipboard. Command: COPY.",
        .alias = "COPY",
        .shortcut = "Ctrl+C",
        .flags = REQUIRED_VIEW
    },
    {
        .id = ACTION_CUT,
        .command = "cut",
        .arguments = "",
        .icon = "cut",
        .tooltip = "Cu&t",
        .statustip = "Cut the current selection's contents to the clipboard. Command: CUT.",
        .alias = "CUT",
        .shortcut = "Ctrl+X",
        .flags = REQUIRED_VIEW
    },
    {
        .id = ACTION_DAY,
        .command = "day",
        .arguments = "",
        .icon = "day",
        .tooltip = "&Day",
        .statustip = "Updates the current view using day vision settings. Command: DAY",
        .alias = "DAY",
        .shortcut = "",
        .flags = REQUIRED_VIEW
    },
    {
        .id = ACTION_DEBUG,
        .command = "debug",
        .arguments = "s",
        .icon = "debug",
        .tooltip = "Debug",
        .statustip = "Displays information about this product:  ABOUT",
        .alias = "DEBUG",
        .shortcut = "",
        .flags = REQUIRED_VIEW
    },
    {
        .id = ACTION_DESIGN_DETAILS,
        .command = "designdetails",
        .arguments = "",
        .icon = "designdetails",
        .tooltip = "&Details",
        .statustip = "Details of the current design. Command: DETAILS",
        .alias = "DESIGNDETAILS, DETAILS",
        .shortcut = "Ctrl+D",
        .flags = REQUIRED_VIEW
    },
    {
        .id = ACTION_DISABLE,
        .command = "disable",
        .arguments = "",
        .icon = "disable",
        .tooltip = "&Disable",
        .statustip = "Disable one of the pre-defined global boolean variables.",
        .alias = "DISABLE",
        .shortcut = "",
        .flags = REQUIRED_VIEW
    },
    {
        .id = ACTION_DISTANCE,
        .command = "distance",
        .arguments = "rrrr",
        .icon = "distance",
        .tooltip = "&Distance",
        .statustip = "Measures the distance and angle between two points. Command: DIST",
        .alias = "DI, DIST, DISTANCE",
        .shortcut = "",
        .flags = REQUIRED_VIEW
    },
    {
        .id = ACTION_DOLPHIN,
        .command = "dolphin",
        .arguments = "",
        .icon = "dolphin",
        .tooltip = "&Dolphin",
        .statustip = "Creates a dolphin. Command: DOLPHIN.",
        .alias = "DOLPHIN",
        .shortcut = "",
        .flags = REQUIRED_VIEW
    },
    {
        .id = ACTION_DO_NOTHING,
        .command = "donothing",
        .arguments = "",
        .icon = "donothing",
        .tooltip = "&Do Nothing",
        .statustip = "Does Nothing. Command: DONOTHING.",
        .alias = "DONOTHING",
        .shortcut = "",
        .flags = CONTEXT_FREE
    },
    {
        .id = ACTION_ELLIPSE,
        .command = "ellipse",
        .arguments = "",
        .icon = "ellipse",
        .tooltip = "&Ellipse",
        .statustip = "Creates an ellipse. Command: ELLIPSE.",
        .alias = "EL, ELLIPSE",
        .shortcut = "",
        .flags = REQUIRED_VIEW
    },
    {
        .id = ACTION_ENABLE,
        .command = "enable",
        .arguments = "",
        .icon = "enable",
        .tooltip = "&Enable",
        .statustip = "Enable a pre-defined global variables.",
        .alias = "ENABLE",
        .shortcut = "",
        .flags = REQUIRED_VIEW
    },
    {
        .id = ACTION_ERASE,
        .command = "erase",
        .arguments = "",
        .icon = "erase",
        .tooltip = "D&elete",
        .statustip = "Removes objects from a drawing. Command: DELETE.",
        .alias = "E, ERASE, DEL, DELETE",
        .shortcut = "",
        .flags = REQUIRED_VIEW
    },
    {
        .id = ACTION_ERROR,
        .command = "error",
        .arguments = "",
        .icon = "error",
        .tooltip = "Error",
        .statustip = "Sends an error message to the user. Command: ERROR",
        .alias = "ERROR",
        .shortcut = "",
        .flags = REQUIRED_VIEW
    },
    {
        .id = ACTION_EXIT,
        .command = "exit",
        .arguments = "",
        .icon = "exit",
        .tooltip = "E&xit",
        .statustip = "Exit the application:  EXIT",
        .alias = "EXIT, QUIT",
        .shortcut = "Ctrl+Q",
        .flags = REQUIRED_VIEW
    },
    {
        .id = ACTION_GET,
        .command = "get",
        .arguments = "",
        .icon = "get",
        .tooltip = "&Get",
        .statustip = "Print a value to one of the pre-defined global variables.",
        .alias = "GET",
        .shortcut = "",
        .flags = REQUIRED_VIEW
    },
    {
        .id = ACTION_HEART,
        .command = "heart",
        .arguments = "",
        .icon = "heart",
        .tooltip = "&Heart",
        .statustip = "Creates a heart:  HEART",
        .alias = "HEART",
        .shortcut = "",
        .flags = REQUIRED_VIEW
    },
    {
        .id = ACTION_HELP,
        .command = "help",
        .arguments = "",
        .icon = "help",
        .tooltip = "&Help",
        .statustip = "Displays help. Command: HELP",
        .alias = "?, HELP",
        .shortcut = "F1",
        .flags = CONTEXT_FREE
    },
    {
        .id = ACTION_HIDE_ALL_LAYERS,
        .command = "hidealllayers",
        .arguments = "",
        .icon = "hidealllayers",
        .tooltip = "&Hide All Layers",
        .statustip = "Turns the visibility off for all layers in the current drawing:  HIDEALL",
        .alias = "HIDEALLLAYERS",
        .shortcut = "",
        .flags = REQUIRED_VIEW
    },
    {
        .id = ACTION_ICON_128,
        .command = "icon128",
        .arguments = "",
        .icon = "icon128",
        .tooltip = "Icon12&8",
        .statustip = "Sets the toolbar icon size to 128x128:  ICON128",
        .alias = "ICON128",
        .shortcut = "",
        .flags = REQUIRED_VIEW
    },
    {
        .id = ACTION_ICON_16,
        .command = "icon16",
        .arguments = "",
        .icon = "icon16",
        .tooltip = "Icon&16",
        .statustip = "Sets the toolbar icon size to 16x16:  ICON16",
        .alias = "ICON16",
        .shortcut = "",
        .flags = REQUIRED_VIEW
    },
    {
        .id = ACTION_ICON_24,
        .command = "icon24",
        .arguments = "",
        .icon = "icon24",
        .tooltip = "Icon&24",
        .statustip = "Sets the toolbar icon size to 24x24:  ICON24",
        .alias = "ICON24",
        .shortcut = "",
        .flags = REQUIRED_VIEW
    },
    {
        .id = ACTION_ICON_32,
        .command = "icon32",
        .arguments = "",
        .icon = "icon32",
        .tooltip = "Icon&32",
        .statustip = "Sets the toolbar icon size to 32x32:  ICON32",
        .alias = "ICON32",
        .shortcut = "",
        .flags = REQUIRED_VIEW
    },
    {
        .id = ACTION_ICON_48,
        .command = "icon48",
        .arguments = "",
        .icon = "icon48",
        .tooltip = "Icon&48",
        .statustip = "Sets the toolbar icon size to 48x48:  ICON48",
        .alias = "ICON48",
        .shortcut = "",
        .flags = REQUIRED_VIEW
    },
    {
        .id = ACTION_ICON_64,
        .command = "icon64",
        .arguments = "",
        .icon = "icon64",
        .tooltip = "Icon&64",
        .statustip = "Sets the toolbar icon size to 64x64:  ICON64",
        .alias = "ICON64",
        .shortcut = "",
        .flags = REQUIRED_VIEW
    },
    {
        .id = ACTION_FREEZE_ALL_LAYERS,
        .command = "freezealllayers",
        .arguments = "",
        .icon = "freezealllayers",
        .tooltip = "&Freeze All Layers",
        .statustip = "Freezes all layers in the current drawing:  FREEZEALL",
        .alias = "FREEZEALLLAYERS",
        .shortcut = "",
        .flags = REQUIRED_VIEW
    },
    {
        .id = ACTION_LAYERS,
        .command = "layers",
        .arguments = "",
        .icon = "layers",
        .tooltip = "&Layers",
        .statustip = "Manages layers and layer properties:  LAYER",
        .alias = "LAYER",
        .shortcut = "",
        .flags = REQUIRED_VIEW
    },
    {
        .id = ACTION_LAYER_PREVIOUS,
        .command = "layerprevious",
        .arguments = "",
        .icon = "layerprevious",
        .tooltip = "&Layer Previous",
        .statustip = "Restores the previous layer settings:  LAYERP",
        .alias = "LAYERP",
        .shortcut = "",
        .flags = REQUIRED_VIEW
    },
    {
        .id = ACTION_LAYER_SELECTOR,
        .command = "layerselector",
        .arguments = "",
        .icon = "layerselector",
        .tooltip = "&Layer Selector",
        .statustip = "Dropdown selector for changing the current layer",
        .alias = "LAYERSELECTOR",
        .shortcut = "",
        .flags = REQUIRED_VIEW
    },
    {
        .id = ACTION_LINE,
        .command = "line",
        .arguments = "",
        .icon = "line",
        .tooltip = "&Line",
        .statustip = "Creates straight line segments:  LINE",
        .alias = "L, LINE",
        .shortcut = "",
        .flags = REQUIRED_VIEW
    },
    {
        .id = ACTION_LINE_TYPE_SELECTOR,
        .command = "linetypeselector",
        .arguments = "",
        .icon = "linetypeselector",
        .tooltip = "&Stitchtype Selector",
        .statustip = "Dropdown selector for changing the current stitch type",
        .alias = "LINETYPESELECTOR",
        .shortcut = "",
        .flags = REQUIRED_VIEW
    },
    {
        .id = ACTION_LINE_WEIGHT_SELECTOR,
        .command = "lineweightselector",
        .arguments = "",
        .icon = "lineweightselector",
        .tooltip = "&Threadweight Selector",
        .statustip = "Dropdown selector for changing the current thread weight",
        .alias = "LINEWEIGHTSELECTOR",
        .shortcut = "",
        .flags = REQUIRED_VIEW
    },
    {
        .id = ACTION_LOCATE_POINT,
        .command = "locatepoint",
        .arguments = "",
        .icon = "locatepoint",
        .tooltip = "&Locate Point",
        .statustip = "Displays the coordinate values of a location:  ID",
        .alias = "ID, LOCATEPOINT",
        .shortcut = "",
        .flags = REQUIRED_VIEW
    },
    {
        .id = ACTION_LOCK_ALL_LAYERS,
        .command = "lockalllayers",
        .arguments = "",
        .icon = "lockalllayers",
        .tooltip = "&Lock All Layers",
        .statustip = "Locks all layers in the current drawing:  LOCKALL",
        .alias = "LOCKALL",
        .shortcut = "",
        .flags = REQUIRED_VIEW
    },
    {
        .id = ACTION_MAKE_LAYER_CURRENT,
        .command = "makelayercurrent",
        .arguments = "",
        .icon = "makelayercurrent",
        .tooltip = "&Make Layer Active",
        .statustip = "Makes the layer of a selected object the active layer",
        .alias = "MAKELAYERCURRENT",
        .shortcut = "",
        .flags = REQUIRED_VIEW
    },
    {
        .id = ACTION_MIRROR_SELECTED,
        .command = "mirrorselected",
        .arguments = "",
        .icon = "mirror",
        .tooltip = "&Mirror Selected",
        .statustip = "Command: MIRRORSELECTED.",
        .alias = "MIRRORSELECTED",
        .shortcut = "",
        .flags = REQUIRED_VIEW
    },
    {
        .id = ACTION_MOVE,
        .command = "move",
        .arguments = "",
        .icon = "move",
        .tooltip = "&Move",
        .statustip = "Displaces objects a specified distance in a specified direction:  MOVE",
        .alias = "M, MOVE",
        .shortcut = "",
        .flags = REQUIRED_VIEW
    },
    {
        .id = ACTION_MOVE_SELECTED,
        .command = "moveselected",
        .arguments = "",
        .icon = "moveselected",
        .tooltip = "&Do Nothing",
        .statustip = "Does Nothing.",
        .alias = "DONOTHING",
        .shortcut = "",
        .flags = REQUIRED_VIEW
    },
    {
        .id = ACTION_NEW,
        .command = "new",
        .arguments = "",
        .icon = "new",
        .tooltip = "&New",
        .statustip = "Create a new file. Command: NEW.",
        .alias = "NEW",
        .shortcut = "Ctrl+N",
        .flags = CONTEXT_FREE | CLEAR_SELECTION
    },
    {
        .id = ACTION_NIGHT,
        .command = "night",
        .arguments = "",
        .icon = "night",
        .tooltip = "&Night",
        .statustip = "Updates the current view using night vision settings. Command: NIGHT.",
        .alias = "NIGHT",
        .shortcut = "",
        .flags = REQUIRED_VIEW
    },
    {
        .id = ACTION_OPEN,
        .command = "open",
        .arguments = "",
        .icon = "open",
        .tooltip = "&Open",
        .statustip = "Open an existing file. Command: OPEN.",
        .alias = "OPEN",
        .shortcut = "Ctrl+O",
        .flags = REQUIRED_VIEW
    },
    {
        .id = ACTION_PAN_DOWN,
        .command = "pandown",
        .arguments = "",
        .icon = "pandown",
        .tooltip = "&Pan Down",
        .statustip = "Moves the view down:  PANDOWN",
        .alias = "PANDOWN",
        .shortcut = "",
        .flags = REQUIRED_VIEW
    },
    {
        .id = ACTION_PAN_LEFT,
        .command = "panleft",
        .arguments = "",
        .icon = "panleft",
        .tooltip = "&Pan Left",
        .statustip = "Moves the view to the left:  PANLEFT",
        .alias = "PANLEFT",
        .shortcut = "",
        .flags = REQUIRED_VIEW
    },
    {
        .id = ACTION_PAN_POINT,
        .command = "panpoint",
        .arguments = "",
        .icon = "panpoint",
        .tooltip = "&Pan Point",
        .statustip = "Moves the view by the specified distance.",
        .alias = "PANPOINT",
        .shortcut = "",
        .flags = REQUIRED_VIEW
    },
    {
        .id = ACTION_PAN_REAL_TIME,
        .command = "panrealtime",
        .arguments = "",
        .icon = "panrealtime",
        .tooltip = "&Pan Realtime",
        .statustip = "Moves the view in the current viewport. Command: PANREALTIME",
        .alias = "PANREALTIME",
        .shortcut = "",
        .flags = REQUIRED_VIEW
    },
    {
        .id = ACTION_PAN_RIGHT,
        .command = "panright",
        .arguments = "",
        .icon = "panright",
        .tooltip = "&Pan Right",
        .statustip = "Moves the view to the right:  PANRIGHT",
        .alias = "PANRIGHT",
        .shortcut = "",
        .flags = REQUIRED_VIEW
    },
    {
        .id = ACTION_PAN_UP,
        .command = "panup",
        .arguments = "",
        .icon = "panup",
        .tooltip = "&Pan Up",
        .statustip = "Moves the view up:  PANUP",
        .alias = "PANUP",
        .shortcut = "",
        .flags = REQUIRED_VIEW
    },
    {
        .id = ACTION_PASTE,
        .command = "paste",
        .arguments = "",
        .icon = "paste",
        .tooltip = "&Paste",
        .statustip = "Paste the clipboard's contents into the current selection. Command: PASTE.",
        .alias = "PASTE",
        .shortcut = "Ctrl+V",
        .flags = REQUIRED_VIEW
    },
    {
        .id = ACTION_PATH,
        .command = "path",
        .arguments = "",
        .icon = "path",
        .tooltip = "&Path",
        .statustip = "Creates a 2D path:  PATH",
        .alias = "PA, PATH",
        .shortcut = "",
        .flags = REQUIRED_VIEW
    },
    {
        .id = ACTION_PLATFORM,
        .command = "platform",
        .arguments = "",
        .icon = "platform",
        .tooltip = "&Platform",
        .statustip = "List which platform is in use. Command: PLATFORM.",
        .alias = "PLATFORM",
        .shortcut = "",
        .flags = REQUIRED_VIEW
    },
    {
        .id = ACTION_POINT,
        .command = "point",
        .arguments = "",
        .icon = "point",
        .tooltip = "&Point",
        .statustip = "Creates multiple points:  POINT",
        .alias = "PO, POINT",
        .shortcut = "",
        .flags = REQUIRED_VIEW
    },
    {
        .id = ACTION_POLYGON,
        .command = "polygon",
        .arguments = "",
        .icon = "polygon",
        .tooltip = "Pol&ygon",
        .statustip = "Creates a regular polygon. Command: POLYGON.",
        .alias = "POL, POLYGON",
        .shortcut = "",
        .flags = REQUIRED_VIEW
    },
    {
        .id = ACTION_POLYLINE,
        .command = "polyline",
        .arguments = "",
        .icon = "polyline",
        .tooltip = "&Polyline",
        .statustip = "Creates a 2D polyline:  PLINE",
        .alias = "PL, PLINE, POLYLINE",
        .shortcut = "",
        .flags = REQUIRED_VIEW
    },
    {
        .id = ACTION_PREVIEW_OFF,
        .command = "previewoff",
        .arguments = "",
        .icon = "previewoff",
        .tooltip = "&Do Nothing",
        .statustip = "Does Nothing.",
        .alias = "DONOTHING",
        .shortcut = "",
        .flags = REQUIRED_VIEW
    },
    {
        .id = ACTION_PREVIEW_ON,
        .command = "previewon",
        .arguments = "",
        .icon = "previewon",
        .tooltip = "&Preview On",
        .statustip = "Preview on.",
        .alias = "PREVIEWON",
        .shortcut = "",
        .flags = REQUIRED_VIEW
    },
    {
        .id = ACTION_PRINT,
        .command = "print",
        .arguments = "",
        .icon = "print",
        .tooltip = "&Print",
        .statustip = "Print the design. Command: PRINT.",
        .alias = "PRINT",
        .shortcut = "Ctrl+P",
        .flags = REQUIRED_VIEW
    },
    {
        .id = ACTION_QUICKLEADER,
        .command = "quickleader",
        .arguments = "",
        .icon = "quickleader",
        .tooltip = "&QuickLeader",
        .statustip = "Creates a leader and annotation:  QUICKLEADER",
        .alias = "LE, LEADER, QLEADER, QUICKLEADER",
        .shortcut = "",
        .flags = REQUIRED_VIEW
    },
    {
        .id = ACTION_RECTANGLE,
        .command = "rectangle",
        .arguments = "",
        .icon = "rectangle",
        .tooltip = "&Rectangle",
        .statustip = "Creates a rectangular polyline. Command: RECTANGLE.",
        .alias = "REC, RECT, RECTANG, RECTANGLE",
        .shortcut = "",
        .flags = REQUIRED_VIEW
    },
    {
        .id = ACTION_REDO,
        .command = "redo",
        .arguments = "",
        .icon = "redo",
        .tooltip = "&Redo",
        .statustip = "Reverses the effects of the previous undo action. Command: REDO.",
        .alias = "REDO",
        .shortcut = "Ctrl+Shift+Z",
        .flags = REQUIRED_VIEW
    },
    {
        .id = ACTION_RGB,
        .command = "rgb",
        .arguments = "",
        .icon = "rgb",
        .tooltip = "Icon&24",
        .statustip = "Sets the toolbar icon size to 24x24:  ICON24",
        .alias = "ICON24",
        .shortcut = "",
        .flags = REQUIRED_VIEW
    },
    {
        .id = ACTION_ROTATE,
        .command = "rotate",
        .arguments = "",
        .icon = "rotate",
        .tooltip = "&Rotate",
        .statustip = "Rotates objects about a base point:  ROTATE",
        .alias = "RO, ROTATE",
        .shortcut = "",
        .flags = REQUIRED_VIEW
    },
    {
        .id = ACTION_SANDBOX,
        .command = "sandbox",
        .arguments = "",
        .icon = "sandbox",
        .tooltip = "&Sandbox",
        .statustip = "A sandbox to play in: SANDBOX",
        .alias = "SAND, SANDBOX",
        .shortcut = "",
        .flags = REQUIRED_VIEW
    },
    {
        .id = ACTION_SAVE,
        .command = "save",
        .arguments = "",
        .icon = "save",
        .tooltip = "&Save",
        .statustip = "Save the design to disk. Command: SAVE.",
        .alias = "SAVE",
        .shortcut = "Ctrl+S",
        .flags = REQUIRED_VIEW
    },
    {
        .id = ACTION_SAVE_AS,
        .command = "saveas",
        .arguments = "",
        .icon = "saveas",
        .tooltip = "Save &As",
        .statustip = "Save the design under a new name. Command: SAVEAS.",
        .alias = "SAVEAS",
        .shortcut = "Ctrl+Shift+S",
        .flags = REQUIRED_VIEW
    },
    {
        .id = ACTION_SCALE,
        .command = "scale",
        .arguments = "",
        .icon = "scale",
        .tooltip = "Sca&le",
        .statustip = "Enlarges or reduces objects proportionally in the X, Y, and Z directions:  SCALE",
        .alias = "SC, SCALE",
        .shortcut = "",
        .flags = REQUIRED_VIEW
    },
    {
        .id = ACTION_SCALE_SELECTED,
        .command = "scaleselected",
        .arguments = "",
        .icon = "scaleselected",
        .tooltip = "&Do Nothing",
        .statustip = "Does Nothing.",
        .alias = "SCALESELECTED",
        .shortcut = "",
        .flags = REQUIRED_VIEW
    },
    {
        .id = ACTION_SELECT_ALL,
        .command = "selectall",
        .arguments = "",
        .icon = "selectall",
        .tooltip = "&Select All",
        .statustip = "Selects all objects:  SELECTALL",
        .alias = "AI_SELALL, SELALL, SELECTALL",
        .shortcut = "",
        .flags = REQUIRED_VIEW
    },
    {
        .id = ACTION_SET,
        .command = "set",
        .arguments = "",
        .icon = "set",
        .tooltip = "&Set",
        .statustip = "Assign a value to one of the pre-defined global variables.",
        .alias = "SET",
        .shortcut = "",
        .flags = REQUIRED_VIEW
    },
    {
        .id = ACTION_SETTINGS_DIALOG,
        .command = "settingsdialog",
        .arguments = "",
        .icon = "settingsdialog",
        .tooltip = "&Settings",
        .statustip = "Configure settings specific to this product. Command: SETTINGS.",
        .alias = "SETTINGS",
        .shortcut = "",
        .flags = REQUIRED_VIEW
    },
    {
        .id = ACTION_SHOW_ALL_LAYERS,
        .command = "showalllayers",
        .arguments = "",
        .icon = "showalllayers",
        .tooltip = "&Show All Layers",
        .statustip = "Turns the visibility on for all layers in the current drawing:  SHOWALL",
        .alias = "SHOWALLLAYERS",
        .shortcut = "",
        .flags = REQUIRED_VIEW
    },
    {
        .id = ACTION_SINGLE_LINE_TEXT,
        .command = "singlelinetext",
        .arguments = "",
        .icon = "singlelinetext",
        .tooltip = "&Single Line Text",
        .statustip = "Creates single-line text objects:  TEXT",
        .alias = "DT, DTEXT, TEXT, SINGLELINETEXT",
        .shortcut = "",
        .flags = REQUIRED_VIEW
    },
    {
        .id = ACTION_SNOWFLAKE,
        .command = "snowflake",
        .arguments = "",
        .icon = "snowflake",
        .tooltip = "&Snowflake",
        .statustip = "Creates a snowflake:  SNOWFLAKE",
        .alias = "SNOWFLAKE",
        .shortcut = "",
        .flags = REQUIRED_VIEW
    },
    {
        .id = ACTION_STAR,
        .command = "star",
        .arguments = "",
        .icon = "star",
        .tooltip = "&Star",
        .statustip = "Creates a star:  STAR",
        .alias = "STAR",
        .shortcut = "",
        .flags = REQUIRED_VIEW
    },
    {
        .id = ACTION_SYSWINDOWS,
        .command = "syswindows",
        .arguments = "",
        .icon = "syswindows",
        .tooltip = "&SysWindows",
        .statustip = "Arrange the windows. Command: SYSWINDOWS",
        .alias = "WINDOWS, SYSWINDOWS",
        .shortcut = "",
        .flags = REQUIRED_VIEW
    },
    {
        .id = ACTION_TEXT_BOLD,
        .command = "textbold",
        .arguments = "",
        .icon = "textbold",
        .tooltip = "&Bold Text",
        .statustip = "Sets text to be bold. Command: BOLD.",
        .alias = "BOLD",
        .shortcut = "",
        .flags = REQUIRED_VIEW
    },
    {
        .id = ACTION_TEST,
        .command = "test",
        .arguments = "",
        .icon = "test",
        .tooltip = "&Test",
        .statustip = "Run a sequence of commands for QA. Command: TEST.",
        .alias = "TEST",
        .shortcut = "",
        .flags = REQUIRED_VIEW
    },
    {
        .id = ACTION_TEXT_ITALIC,
        .command = "testitalic",
        .arguments = "",
        .icon = "textitalic",
        .tooltip = "&Italic Text",
        .statustip = "Sets text to be italic. Command: ITALIC.",
        .alias = "ITALIC",
        .shortcut = "",
        .flags = REQUIRED_VIEW
    },
    {
        .id = ACTION_TEXT_UNDERLINE,
        .command = "testunderline",
        .arguments = "",
        .icon = "textunderline",
        .tooltip = "&Underline Text",
        .statustip = "Sets text to be underlined. Command: UNDERLINE.",
        .alias = "UNDERLINE",
        .shortcut = "",
        .flags = REQUIRED_VIEW
    },
    {
        .id = ACTION_TEXT_STRIKEOUT,
        .command = "teststrikeout",
        .arguments = "",
        .icon = "textstrikeout",
        .tooltip = "&StrikeOut Text",
        .statustip = "Sets text to be striked out. Command: STRIKEOUT.",
        .alias = "STRIKEOUT",
        .shortcut = "",
        .flags = REQUIRED_VIEW
    },
    {
        .id = ACTION_TEXT_OVERLINE,
        .command = "testoverline",
        .arguments = "",
        .icon = "textoverline",
        .tooltip = "&Overline Text",
        .statustip = "Sets text to be overlined. Command: OVERLINE.",
        .alias = "OVERLINE",
        .shortcut = "",
        .flags = REQUIRED_VIEW
    },
    {
        .id = ACTION_THAW_ALL_LAYERS,
        .command = "thawalllayers",
        .arguments = "",
        .icon = "thawalllayers",
        .tooltip = "&Thaw All Layers",
        .statustip = "Thaws all layers in the current drawing:  THAWALL",
        .alias = "THAWALL",
        .shortcut = "",
        .flags = REQUIRED_VIEW
    },
    {
        .id = ACTION_TIP_OF_THE_DAY,
        .command = "tipoftheday",
        .arguments = "",
        .icon = "tipoftheday",
        .tooltip = "&Tip Of The Day",
        .statustip = "Displays a dialog with useful tips:  TIPS",
        .alias = "TIPS, TIPOFTHEDAY",
        .shortcut = "",
        .flags = REQUIRED_VIEW
    },
    {
        .id = ACTION_TODO,
        .command = "todo",
        .arguments = "",
        .icon = "todo",
        .tooltip = "&About",
        .statustip = "Displays information about this product:  ABOUT",
        .alias = "TODO",
        .shortcut = "",
        .flags = REQUIRED_VIEW
    },
    {
        .id = ACTION_UNDO,
        .command = "undo",
        .arguments = "",
        .icon = "undo",
        .tooltip = "&Undo",
        .statustip = "Reverses the most recent action. Command: UNDO.",
        .alias = "U, UNDO",
        .shortcut = "Ctrl+Z",
        .flags = REQUIRED_VIEW
    },
    {
        .id = ACTION_UNLOCK_ALL_LAYERS,
        .command = "unlockalllayers",
        .arguments = "",
        .icon = "unlockalllayers",
        .tooltip = "&Unlock All Layers",
        .statustip = "Unlocks all layers in the current drawing:  UNLOCKALL",
        .alias = "UNLOCKALL",
        .shortcut = "",
        .flags = REQUIRED_VIEW
    },
    {
        .id = ACTION_VULCANIZE,
        .command = "vulcanize",
        .arguments = "",
        .icon = "vulcanize",
        .tooltip = "&Undo",
        .statustip = "Reverses the most recent action:  UNDO",
        .alias = "VULCANIZE",
        .shortcut = "",
        .flags = REQUIRED_VIEW
    },
    {
        .id = ACTION_WINDOW_CASCADE,
        .command = "windowcascade",
        .arguments = "",
        .icon = "windowcascade",
        .tooltip = "&Cascade",
        .statustip = "Cascade the windows. Command: CASCADE.",
        .alias = "CASCADE, WINDOWCASCADE",
        .shortcut = "",
        .flags = REQUIRED_VIEW
    },
    {
        .id = ACTION_WHATS_THIS,
        .command = "whatsthis",
        .arguments = "",
        .icon = "whatsthis",
        .tooltip = "&What's This?",
        .statustip = "What's This? Context Help! Command: WHATSTHIS.",
        .alias = "WHATSTHIS",
        .shortcut = "",
        .flags = REQUIRED_VIEW
    },
    {
        .id = ACTION_WINDOW_CLOSE,
        .command = "windowclose",
        .arguments = "",
        .icon = "windowclose",
        .tooltip = "Cl&ose",
        .statustip = "Close the active window. Command: CLOSE.",
        .alias = "CLOSE, WINDOWCLOSE",
        .shortcut = "Ctrl+W",
        .flags = REQUIRED_VIEW
    },
    {
        .id = ACTION_WINDOW_CLOSE_ALL,
        .command = "windowcloseall",
        .arguments = "",
        .icon = "windowcloseall",
        .tooltip = "Close &All",
        .statustip = "Close all the windows. Command: CLOSEALL",
        .alias = "CLOSEALL, WINDOWCLOSEALL",
        .shortcut = "",
        .flags = REQUIRED_VIEW
    },
    {
        .id = ACTION_WINDOW_NEXT,
        .command = "windownext",
        .arguments = "",
        .icon = "windownext",
        .tooltip = "Ne&xt",
        .statustip = "Move the focus to the next window:  NEXT",
        .alias = "NEXT, WINDOWNEXT",
        .shortcut = "Ctrl+Tab",
        .flags = REQUIRED_VIEW
    },
    {
        .id = ACTION_WINDOW_PREVIOUS,
        .command = "windowprevious",
        .arguments = "",
        .icon = "windowprevious",
        .tooltip = "Pre&vious",
        .statustip = "Move the focus to the previous window:  PREVIOUS",
        .alias = "PREV, PREVIOUS, WINDOWPREVIOUS",
        .shortcut = "Ctrl+Shift+Tab",
        .flags = REQUIRED_VIEW
    },
    {
        .id = ACTION_WINDOW_TILE,
        .command = "windowtile",
        .arguments = "",
        .icon = "windowtile",
        .tooltip = "&Tile",
        .statustip = "Tile the windows:  TILE",
        .alias = "TILE, WINDOWTILE",
        .shortcut = "",
        .flags = REQUIRED_VIEW
    },
    {
        .id = ACTION_ZOOM_ALL,
        .command = "zoomall",
        .arguments = "",
        .icon = "zoomall",
        .tooltip = "Zoom &All",
        .statustip = "Zooms to display the drawing extents or the grid limits. Command: ZOOMALL.",
        .alias = "ZOOMALL",
        .shortcut = "",
        .flags = REQUIRED_VIEW
    },
    {
        .id = ACTION_ZOOM_CENTER,
        .command = "zoomcenter",
        .arguments = "",
        .icon = "zoomcenter",
        .tooltip = "Zoom &Center",
        .statustip = "Zooms to display a view specified by a center point and magnification or height. Command: ZOOMCENTER.",
        .alias = "ZOOMCENTER",
        .shortcut = "",
        .flags = REQUIRED_VIEW
    },
    {
        .id = ACTION_ZOOM_DYNAMIC,
        .command = "zoomdynamic",
        .arguments = "",
        .icon = "zoomdynamic",
        .tooltip = "Zoom &Dynamic",
        .statustip = "Zooms to display the generated portion of the drawing. Command: ZOOMDYNAMIC.",
        .alias = "ZOOMDYNAMIC",
        .shortcut = "",
        .flags = REQUIRED_VIEW
    },
    {
        .id = ACTION_ZOOM_EXTENTS,
        .command = "zoomextents",
        .arguments = "",
        .icon = "zoomextents",
        .tooltip = "Zoom &Extents",
        .statustip = "Zooms to display the drawing extents. Command: ZOOMEXTENTS",
        .alias = "ZOOMEXTENTS",
        .shortcut = "",
        .flags = REQUIRED_VIEW
    },
    {
        .id = ACTION_ZOOM_IN,
        .command = "zoomin",
        .arguments = "",
        .icon = "zoomin",
        .tooltip = "Zoom &In",
        .statustip = "Zooms to increase the apparent size of objects. Command: ZOOMIN",
        .alias = "ZOOMIN",
        .shortcut = "Ctrl+Plus",
        .flags = REQUIRED_VIEW
    },
    {
        .id = ACTION_ZOOM_OUT,
        .command = "zoomout",
        .arguments = "",
        .icon = "zoomout",
        .tooltip = "Zoom &Out",
        .statustip = "Zooms to decrease the apparent size of objects. Command: ZOOMOUT",
        .alias = "ZOOMOUT",
        .shortcut = "Ctrl+Minus",
        .flags = REQUIRED_VIEW
    },
    {
        .id = ACTION_ZOOM_PREVIOUS,
        .command = "zoomprevious",
        .arguments = "",
        .icon = "zoomprevious",
        .tooltip = "Zoom &Previous",
        .statustip = "Zooms to display the previous view. Command: ZOOMPREVIOUS.",
        .alias = "ZOOMPREVIOUS",
        .shortcut = "",
        .flags = REQUIRED_VIEW
    },
    {
        .id = ACTION_ZOOM_REAL_TIME,
        .command = "zoomrealtime",
        .arguments = "",
        .icon = "zoomrealtime",
        .tooltip = "Zoom &Real Time",
        .statustip = "Zooms to increase or decrease the apparent size of objects in the current viewport. Command: ZOOMREALTIME",
        .alias = "ZOOMREALTIME",
        .shortcut = "",
        .flags = REQUIRED_VIEW
    },
    {
        .id = ACTION_ZOOM_SCALE,
        .command = "zoomscale",
        .arguments = "",
        .icon = "zoomscale",
        .tooltip = "Zoom &Scale",
        .statustip = "Zooms the display using a specified scale factor. Command: ZOOMSCALE.",
        .alias = "ZOOMSCALE",
        .shortcut = "",
        .flags = REQUIRED_VIEW
    },
    {
        .id = ACTION_ZOOM_SELECTED,
        .command = "zoomselected",
        .arguments = "",
        .icon = "zoomselected",
        .tooltip = "Zoom Selec&ted",
        .statustip = "Zooms to display the selected objects. Command: ZOOMSELECTED.",
        .alias = "ZOOMSELECTED",
        .shortcut = "",
        .flags = REQUIRED_VIEW
    },
    {
        .id = ACTION_ZOOM_WINDOW,
        .command = "zoomwindow",
        .arguments = "",
        .icon = "zoomwindow",
        .tooltip = "Zoom &Window",
        .statustip = "Zooms to display an area specified by a rectangular window. Command: ZOOMIN",
        .alias = "ZOOMWINDOW",
        .shortcut = "",
        .flags = REQUIRED_VIEW
    },
    {
        .id = -2,
        .command = "blank",
        .arguments = "",
        .icon = "blank",
        .tooltip = "NULL",
        .statustip = "NULL",
        .alias = "NULL",
        .shortcut = "",
        .flags = CONTEXT_FREE
    }
};
