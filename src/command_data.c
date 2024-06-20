/*
 * Embroidermodder 2.
 *
 * Copyright 2011-2024 The Embroidermodder Team
 * Embroidermodder 2 is Open Source Software, see LICENSE.md for licensing terms.
 * Visit https://www.libembroidery.org/refman for advice on altering this file,
 * or read the markdown version in embroidermodder2/docs/refman.
 *
 * Command Data
 *
 * Shortcuts should match: https://doc.qt.io/qt-6/qkeysequence.html#standard-shortcuts
 * Apple platforms may need an additional argument like .apple_shortcut
 */

#include "core.h"

ScriptValue test_command(ScriptEnv *) {return script_true;}

Command command_data[] = {
    {
        .id = ACTION_ABOUT,
        .main = about_command,
        .arguments = "",
        .icon = "about",
        .tooltip = "&About Embroidermodder 2",
        .statustip = "Displays information about this product. Command: ABOUT.",
        .alias = "ABOUT",
        .shortcut = ""
    },
    {
        .id = -1,
        .main = arc_command,
        .icon = "arc",
        .tooltip = "&Arc",
        .statustip = "Displays information about this product:  ARC",
        .alias = "ARC",
        .shortcut = ""
    },
    {
        .id = ACTION_ALERT,
        .main = alert_command,
        .arguments = "ss",
        .icon = "warning",
        .tooltip = "&Alert",
        .statustip = "Creates a dialog to alert the user. Command: ALERT.",
        .alias = "ALERT",
        .shortcut = ""
    },
    {
        .id = ACTION_ANGLE,
        .main = angle_command,
        .arguments = "rrr",
        .icon = "angle",
        .tooltip = "&Angle",
        .statustip = "Calculate the angle between two lines and display it. Command: ANGLE, CALCANGLE",
        .alias = "ANGLE, CALCANGLE",
        .shortcut = ""
    },
    {
        .id = ACTION_CIRCLE,
        .main = circle_command,
        .arguments = "rrr",
        .icon = "circle",
        .tooltip = "&Circle",
        .statustip = "Creates a circle. Command: CIRCLE.",
        .alias = "C, CIRCLE",
        .shortcut = ""
    },
    {
        .id = ACTION_CHANGELOG,
        .main = changelog_command,
        .arguments = "",
        .icon = "changelog",
        .tooltip = "&Changelog",
        .statustip = "Describes new features in this product. Command: CHANGELOG.",
        .alias = "CHANGELOG",
        .shortcut = ""
    },
    {
        .id = ACTION_CLEAR,
        .main = clear_command,
        .arguments = "",
        .icon = "clear",
        .tooltip = "&Do Nothing",
        .statustip = "Does Nothing. Command: DONOTHING.",
        .alias = "DONOTHING",
        .shortcut = ""
    },
    {
        .id = ACTION_COLOR_SELECTOR,
        .main = colorselector_command,
        .arguments = "",
        .icon = "colorselector",
        .tooltip = "&Color Selector",
        .statustip = "Dropdown selector for changing the current thread color",
        .alias = "COLORSELECTOR",
        .shortcut = ""
    },
    {
        .id = ACTION_COPY,
        .main = copy_command,
        .arguments = "",
        .icon = "copy",
        .tooltip = "&Copy",
        .statustip = "Copy the current selection's contents to the clipboard. Command: COPY.",
        .alias = "COPY",
        .shortcut = "Ctrl+C"
    },
    {
        .id = ACTION_CUT,
        .main = cut_command,
        .arguments = "",
        .icon = "cut",
        .tooltip = "Cu&t",
        .statustip = "Cut the current selection's contents to the clipboard. Command: CUT.",
        .alias = "CUT",
        .shortcut = "Ctrl+X"
    },
    {
        .id = ACTION_DAY,
        .main = day_command,
        .arguments = "",
        .icon = "day",
        .tooltip = "&Day",
        .statustip = "Updates the current view using day vision settings. Command: DAY",
        .alias = "DAY",
        .shortcut = ""
    },
    {
        .id = ACTION_DEBUG,
        .main = debug_command,
        .arguments = "",
        .icon = "debug",
        .tooltip = "Debug",
        .statustip = "Displays information about this product:  ABOUT",
        .alias = "DEBUG",
        .shortcut = ""
    },
    {
        .id = ACTION_DESIGN_DETAILS,
        .main = design_details_command,
        .arguments = "",
        .icon = "designdetails",
        .tooltip = "&Details",
        .statustip = "Details of the current design. Command: DETAILS",
        .alias = "DESIGNDETAILS, DETAILS",
        .shortcut = "Ctrl+D"
    },
    {
        .id = ACTION_DISABLE,
        .main = disable_command,
        .arguments = "",
        .icon = "disable",
        .tooltip = "&Disable",
        .statustip = "Disable one of the pre-defined global boolean variables.",
        .alias = "DISABLE",
        .shortcut = ""
    },
    {
        .id = ACTION_DISTANCE,
        .main = distance_command,
        .arguments = "rrrr",
        .icon = "distance",
        .tooltip = "&Distance",
        .statustip = "Measures the distance and angle between two points. Command: DIST",
        .alias = "DI, DIST, DISTANCE",
        .shortcut = ""
    },
    {
        .id = ACTION_DOLPHIN,
        .main = dolphin_command,
        .arguments = "",
        .icon = "dolphin",
        .tooltip = "&Dolphin",
        .statustip = "Creates a dolphin. Command: DOLPHIN.",
        .alias = "DOLPHIN",
        .shortcut = ""
    },
    {
        .id = ACTION_DO_NOTHING,
        .main = do_nothing,
        .arguments = "",
        .icon = "donothing",
        .tooltip = "&Do Nothing",
        .statustip = "Does Nothing. Command: DONOTHING.",
        .alias = "DONOTHING",
        .shortcut = ""
    },
    {
        .id = ACTION_ELLIPSE,
        .main = ellipse_command,
        .arguments = "",
        .icon = "ellipse",
        .tooltip = "&Ellipse",
        .statustip = "Creates an ellipse. Command: ELLIPSE.",
        .alias = "EL, ELLIPSE",
        .shortcut = ""
    },
    {
        .id = ACTION_ENABLE,
        .main = enable_command,
        .arguments = "",
        .icon = "enable",
        .tooltip = "&Enable",
        .statustip = "Enable a pre-defined global variables.",
        .alias = "ENABLE",
        .shortcut = ""
    },
    {
        .id = ACTION_ERASE,
        .main = erase_command,
        .arguments = "",
        .icon = "erase",
        .tooltip = "D&elete",
        .statustip = "Removes objects from a drawing. Command: DELETE.",
        .alias = "E, ERASE, DEL, DELETE",
        .shortcut = ""
    },
    {
        .id = ACTION_ERROR,
        .main = error_command,
        .arguments = "",
        .icon = "error",
        .tooltip = "Error",
        .statustip = "Sends an error message to the user. Command: ERROR",
        .alias = "ERROR",
        .shortcut = ""
    },
    {
        .id = ACTION_EXIT,
        .main = exit_command,
        .arguments = "",
        .icon = "exit",
        .tooltip = "E&xit",
        .statustip = "Exit the application:  EXIT",
        .alias = "EXIT, QUIT",
        .shortcut = "Ctrl+Q"
    },
    {
        .id = ACTION_GET,
        .main = get_command,
        .arguments = "",
        .icon = "get",
        .tooltip = "&Get",
        .statustip = "Print a value to one of the pre-defined global variables.",
        .alias = "GET",
        .shortcut = ""
    },
    {
        .id = ACTION_HEART,
        .main = heart_command,
        .arguments = "",
        .icon = "heart",
        .tooltip = "&Heart",
        .statustip = "Creates a heart:  HEART",
        .alias = "HEART",
        .shortcut = ""
    },
    {
        .id = ACTION_HELP,
        .main = help_command,
        .arguments = "",
        .icon = "help",
        .tooltip = "&Help",
        .statustip = "Displays help. Command: HELP",
        .alias = "?, HELP",
        .shortcut = "F1"
    },
    {
        .id = ACTION_HIDE_ALL_LAYERS,
        .main = hidealllayers_command,
        .arguments = "",
        .icon = "hidealllayers",
        .tooltip = "&Hide All Layers",
        .statustip = "Turns the visibility off for all layers in the current drawing:  HIDEALL",
        .alias = "HIDEALLLAYERS",
        .shortcut = ""
    },
    {
        .id = ACTION_ICON_128,
        .main = icon128_command,
        .arguments = "",
        .icon = "icon128",
        .tooltip = "Icon12&8",
        .statustip = "Sets the toolbar icon size to 128x128:  ICON128",
        .alias = "ICON128",
        .shortcut = ""
    },
    {
        .id = ACTION_ICON_16,
        .main = icon16_command,
        .arguments = "",
        .icon = "icon16",
        .tooltip = "Icon&16",
        .statustip = "Sets the toolbar icon size to 16x16:  ICON16",
        .alias = "ICON16",
        .shortcut = ""
    },
    {
        .id = ACTION_ICON_24,
        .main = icon24_command,
        .arguments = "",
        .icon = "icon24",
        .tooltip = "Icon&24",
        .statustip = "Sets the toolbar icon size to 24x24:  ICON24",
        .alias = "ICON24",
        .shortcut = ""
    },
    {
        .id = ACTION_ICON_32,
        .main = icon32_command,
        .arguments = "",
        .icon = "icon32",
        .tooltip = "Icon&32",
        .statustip = "Sets the toolbar icon size to 32x32:  ICON32",
        .alias = "ICON32",
        .shortcut = ""
    },
    {
        .id = ACTION_ICON_48,
        .main = icon48_command,
        .arguments = "",
        .icon = "icon48",
        .tooltip = "Icon&48",
        .statustip = "Sets the toolbar icon size to 48x48:  ICON48",
        .alias = "ICON48",
        .shortcut = ""
    },
    {
        .id = ACTION_ICON_64,
        .main = icon64_command,
        .arguments = "",
        .icon = "icon64",
        .tooltip = "Icon&64",
        .statustip = "Sets the toolbar icon size to 64x64:  ICON64",
        .alias = "ICON64",
        .shortcut = ""
    },
    {
        .id = ACTION_FREEZE_ALL_LAYERS,
        .main = freezealllayers_command,
        .arguments = "",
        .icon = "freezealllayers",
        .tooltip = "&Freeze All Layers",
        .statustip = "Freezes all layers in the current drawing:  FREEZEALL",
        .alias = "FREEZEALLLAYERS",
        .shortcut = ""
    },
    {
        .id = ACTION_LAYERS,
        .main = layers_command,
        .arguments = "",
        .icon = "layers",
        .tooltip = "&Layers",
        .statustip = "Manages layers and layer properties:  LAYER",
        .alias = "LAYER",
        .shortcut = ""
    },
    {
        .id = ACTION_LAYER_PREVIOUS,
        .main = layerprevious_command,
        .arguments = "",
        .icon = "layerprevious",
        .tooltip = "&Layer Previous",
        .statustip = "Restores the previous layer settings:  LAYERP",
        .alias = "LAYERP",
        .shortcut = ""
    },
    {
        .id = ACTION_LAYER_SELECTOR,
        .main = layerselector_command,
        .arguments = "",
        .icon = "layerselector",
        .tooltip = "&Layer Selector",
        .statustip = "Dropdown selector for changing the current layer",
        .alias = "LAYERSELECTOR",
        .shortcut = ""
    },
    {
        .id = ACTION_LINE,
        .main = line_command,
        .arguments = "",
        .icon = "line",
        .tooltip = "&Line",
        .statustip = "Creates straight line segments:  LINE",
        .alias = "L, LINE",
        .shortcut = ""
    },
    {
        .id = ACTION_LINE_TYPE_SELECTOR,
        .main = linetypeselector_command,
        .arguments = "",
        .icon = "linetypeselector",
        .tooltip = "&Stitchtype Selector",
        .statustip = "Dropdown selector for changing the current stitch type",
        .alias = "LINETYPESELECTOR",
        .shortcut = ""
    },
    {
        .id = ACTION_LINE_WEIGHT_SELECTOR,
        .main = lineweightselector_command,
        .arguments = "",
        .icon = "lineweightselector",
        .tooltip = "&Threadweight Selector",
        .statustip = "Dropdown selector for changing the current thread weight",
        .alias = "LINEWEIGHTSELECTOR",
        .shortcut = ""
    },
    {
        .id = ACTION_LOCATE_POINT,
        .main = locatepoint_command,
        .arguments = "",
        .icon = "locatepoint",
        .tooltip = "&Locate Point",
        .statustip = "Displays the coordinate values of a location:  ID",
        .alias = "ID, LOCATEPOINT",
        .shortcut = ""
    },
    {
        .id = ACTION_LOCK_ALL_LAYERS,
        .main = lockalllayers_command,
        .arguments = "",
        .icon = "lockalllayers",
        .tooltip = "&Lock All Layers",
        .statustip = "Locks all layers in the current drawing:  LOCKALL",
        .alias = "LOCKALL",
        .shortcut = ""
    },
    {
        .id = ACTION_MAKE_LAYER_CURRENT,
        .main = makelayercurrent_command,
        .arguments = "",
        .icon = "makelayercurrent",
        .tooltip = "&Make Layer Active",
        .statustip = "Makes the layer of a selected object the active layer",
        .alias = "MAKELAYERCURRENT",
        .shortcut = ""
    },
    {
        .id = ACTION_MIRROR_SELECTED,
        .main = mirrorselected_command,
        .arguments = "",
        .icon = "mirror",
        .tooltip = "&Mirror Selected",
        .statustip = "Command: MIRRORSELECTED.",
        .alias = "MIRRORSELECTED",
        .shortcut = ""
    },
    {
        .id = ACTION_MOVE,
        .main = move_command,
        .arguments = "",
        .icon = "move",
        .tooltip = "&Move",
        .statustip = "Displaces objects a specified distance in a specified direction:  MOVE",
        .alias = "M, MOVE",
        .shortcut = ""
    },
    {
        .id = ACTION_MOVE_SELECTED,
        .main = moveselected_command,
        .arguments = "",
        .icon = "moveselected",
        .tooltip = "&Do Nothing",
        .statustip = "Does Nothing.",
        .alias = "DONOTHING",
        .shortcut = ""
    },
    {
        .id = ACTION_NEW,
        .main = new_command,
        .arguments = "",
        .icon = "new",
        .tooltip = "&New",
        .statustip = "Create a new file. Command: NEW.",
        .alias = "NEW",
        .shortcut = "Ctrl+N"
    },
    {
        .id = ACTION_NIGHT,
        .main = night_command,
        .arguments = "",
        .icon = "night",
        .tooltip = "&Night",
        .statustip = "Updates the current view using night vision settings. Command: NIGHT.",
        .alias = "NIGHT",
        .shortcut = ""
    },
    {
        .id = ACTION_OPEN,
        .main = open_command,
        .arguments = "",
        .icon = "open",
        .tooltip = "&Open",
        .statustip = "Open an existing file. Command: OPEN.",
        .alias = "OPEN",
        .shortcut = "Ctrl+O"
    },
    {
        .id = ACTION_PAN_DOWN,
        .main = pandown_command,
        .arguments = "",
        .icon = "pandown",
        .tooltip = "&Pan Down",
        .statustip = "Moves the view down:  PANDOWN",
        .alias = "PANDOWN",
        .shortcut = ""
    },
    {
        .id = ACTION_PAN_LEFT,
        .main = panleft_command,
        .arguments = "",
        .icon = "panleft",
        .tooltip = "&Pan Left",
        .statustip = "Moves the view to the left:  PANLEFT",
        .alias = "PANLEFT",
        .shortcut = ""
    },
    {
        .id = ACTION_PAN_POINT,
        .main = panpoint_command,
        .arguments = "",
        .icon = "panpoint",
        .tooltip = "&Pan Point",
        .statustip = "Moves the view by the specified distance.",
        .alias = "PANPOINT",
        .shortcut = ""
    },
    {
        .id = ACTION_PAN_REAL_TIME,
        .main = panrealtime_command,
        .arguments = "",
        .icon = "panrealtime",
        .tooltip = "&Pan Realtime",
        .statustip = "Moves the view in the current viewport. Command: PANREALTIME",
        .alias = "PANREALTIME",
        .shortcut = ""
    },
    {
        .id = ACTION_PAN_RIGHT,
        .main = panright_command,
        .arguments = "",
        .icon = "panright",
        .tooltip = "&Pan Right",
        .statustip = "Moves the view to the right:  PANRIGHT",
        .alias = "PANRIGHT",
        .shortcut = ""
    },
    {
        .id = ACTION_PAN_UP,
        .main = panup_command,
        .arguments = "",
        .icon = "panup",
        .tooltip = "&Pan Up",
        .statustip = "Moves the view up:  PANUP",
        .alias = "PANUP",
        .shortcut = ""
    },
    {
        .id = ACTION_PASTE,
        .main = paste_command,
        .arguments = "",
        .icon = "paste",
        .tooltip = "&Paste",
        .statustip = "Paste the clipboard's contents into the current selection. Command: PASTE.",
        .alias = "PASTE",
        .shortcut = "Ctrl+V"
    },
    {
        .id = ACTION_PATH,
        .main = path_command,
        .arguments = "",
        .icon = "path",
        .tooltip = "&Path",
        .statustip = "Creates a 2D path:  PATH",
        .alias = "PA, PATH",
        .shortcut = ""
    },
    {
        .id = ACTION_PLATFORM,
        .main = platform_command,
        .arguments = "",
        .icon = "platform",
        .tooltip = "&Platform",
        .statustip = "List which platform is in use. Command: PLATFORM.",
        .alias = "PLATFORM",
        .shortcut = ""
    },
    {
        .id = ACTION_POINT,
        .main = point_command,
        .arguments = "",
        .icon = "point",
        .tooltip = "&Point",
        .statustip = "Creates multiple points:  POINT",
        .alias = "PO, POINT",
        .shortcut = ""
    },
    {
        .id = ACTION_POLYGON,
        .main = polygon_command,
        .arguments = "",
        .icon = "polygon",
        .tooltip = "Pol&ygon",
        .statustip = "Creates a regular polygon. Command: POLYGON.",
        .alias = "POL, POLYGON",
        .shortcut = ""
    },
    {
        .id = ACTION_POLYLINE,
        .main = polyline_command,
        .arguments = "",
        .icon = "polyline",
        .tooltip = "&Polyline",
        .statustip = "Creates a 2D polyline:  PLINE",
        .alias = "PL, PLINE, POLYLINE",
        .shortcut = ""
    },
    {
        .id = ACTION_PREVIEW_OFF,
        .main = previewoff_command,
        .arguments = "",
        .icon = "previewoff",
        .tooltip = "&Do Nothing",
        .statustip = "Does Nothing.",
        .alias = "DONOTHING",
        .shortcut = ""
    },
    {
        .id = ACTION_PREVIEW_ON,
        .main = previewon_command,
        .arguments = "",
        .icon = "previewon",
        .tooltip = "&Preview On",
        .statustip = "Preview on.",
        .alias = "PREVIEWON",
        .shortcut = ""
    },
    {
        .id = ACTION_PRINT,
        .main = print_command,
        .arguments = "",
        .icon = "print",
        .tooltip = "&Print",
        .statustip = "Print the design. Command: PRINT.",
        .alias = "PRINT",
        .shortcut = "Ctrl+P"
    },
    {
        .id = ACTION_QUICKLEADER,
        .main = quickleader_command,
        .arguments = "",
        .icon = "quickleader",
        .tooltip = "&QuickLeader",
        .statustip = "Creates a leader and annotation:  QUICKLEADER",
        .alias = "LE, LEADER, QLEADER, QUICKLEADER",
        .shortcut = ""
    },
    {
        .id = ACTION_RECTANGLE,
        .main = rectangle_command,
        .arguments = "",
        .icon = "rectangle",
        .tooltip = "&Rectangle",
        .statustip = "Creates a rectangular polyline. Command: RECTANGLE.",
        .alias = "REC, RECT, RECTANG, RECTANGLE",
        .shortcut = ""
    },
    {
        .id = ACTION_REDO,
        .main = redo_command,
        .arguments = "",
        .icon = "redo",
        .tooltip = "&Redo",
        .statustip = "Reverses the effects of the previous undo action. Command: REDO.",
        .alias = "REDO",
        .shortcut = "Ctrl+Shift+Z"
    },
    {
        .id = ACTION_RGB,
        .main = rgb_command,
        .arguments = "",
        .icon = "rgb",
        .tooltip = "Icon&24",
        .statustip = "Sets the toolbar icon size to 24x24:  ICON24",
        .alias = "ICON24",
        .shortcut = ""
    },
    {
        .id = ACTION_ROTATE,
        .main = rotate_command,
        .arguments = "",
        .icon = "rotate",
        .tooltip = "&Rotate",
        .statustip = "Rotates objects about a base point:  ROTATE",
        .alias = "RO, ROTATE",
        .shortcut = ""
    },
    {
        .id = ACTION_SANDBOX,
        .main = sandbox_command,
        .arguments = "",
        .icon = "sandbox",
        .tooltip = "&Sandbox",
        .statustip = "A sandbox to play in: SANDBOX",
        .alias = "SAND, SANDBOX",
        .shortcut = ""
    },
    {
        .id = ACTION_SAVE,
        .main = save_command,
        .arguments = "",
        .icon = "save",
        .tooltip = "&Save",
        .statustip = "Save the design to disk. Command: SAVE.",
        .alias = "SAVE",
        .shortcut = "Ctrl+S"
    },
    {
        .id = ACTION_SAVE_AS,
        .main = save_command,
        .arguments = "",
        .icon = "saveas",
        .tooltip = "Save &As",
        .statustip = "Save the design under a new name. Command: SAVEAS.",
        .alias = "SAVEAS",
        .shortcut = "Ctrl+Shift+S"
    },
    {
        .id = ACTION_SCALE,
        .main = scale_command,
        .arguments = "",
        .icon = "scale",
        .tooltip = "Sca&le",
        .statustip = "Enlarges or reduces objects proportionally in the X, Y, and Z directions:  SCALE",
        .alias = "SC, SCALE",
        .shortcut = ""
    },
    {
        .id = ACTION_SCALE_SELECTED,
        .main = scaleselected_command,
        .arguments = "",
        .icon = "scaleselected",
        .tooltip = "&Do Nothing",
        .statustip = "Does Nothing.",
        .alias = "SCALESELECTED",
        .shortcut = ""
    },
    {
        .id = ACTION_SELECT_ALL,
        .main = selectall_command,
        .arguments = "",
        .icon = "selectall",
        .tooltip = "&Select All",
        .statustip = "Selects all objects:  SELECTALL",
        .alias = "AI_SELALL, SELALL, SELECTALL",
        .shortcut = ""
    },
    {
        .id = ACTION_SET,
        .main = set_command,
        .arguments = "",
        .icon = "set",
        .tooltip = "&Set",
        .statustip = "Assign a value to one of the pre-defined global variables.",
        .alias = "SET",
        .shortcut = ""
    },
    {
        .id = ACTION_SETTINGS_DIALOG,
        .main = settings_dialog_command,
        .arguments = "",
        .icon = "settingsdialog",
        .tooltip = "&Settings",
        .statustip = "Configure settings specific to this product. Command: SETTINGS.",
        .alias = "SETTINGS",
        .shortcut = ""
    },
    {
        .id = ACTION_SHOW_ALL_LAYERS,
        .main = showalllayers_command,
        .arguments = "",
        .icon = "showalllayers",
        .tooltip = "&Show All Layers",
        .statustip = "Turns the visibility on for all layers in the current drawing:  SHOWALL",
        .alias = "SHOWALLLAYERS",
        .shortcut = ""
    },
    {
        .id = ACTION_SINGLE_LINE_TEXT,
        .main = singlelinetext_command,
        .arguments = "",
        .icon = "singlelinetext",
        .tooltip = "&Single Line Text",
        .statustip = "Creates single-line text objects:  TEXT",
        .alias = "DT, DTEXT, TEXT, SINGLELINETEXT",
        .shortcut = ""
    },
    {
        .id = ACTION_SNOWFLAKE,
        .main = snowflake_command,
        .arguments = "",
        .icon = "snowflake",
        .tooltip = "&Snowflake",
        .statustip = "Creates a snowflake:  SNOWFLAKE",
        .alias = "SNOWFLAKE",
        .shortcut = ""
    },
    {
        .id = ACTION_STAR,
        .main = star_command,
        .arguments = "",
        .icon = "star",
        .tooltip = "&Star",
        .statustip = "Creates a star:  STAR",
        .alias = "STAR",
        .shortcut = ""
    },
    {
        .id = ACTION_SYSWINDOWS,
        .main = syswindows_command,
        .arguments = "",
        .icon = "syswindows",
        .tooltip = "&SysWindows",
        .statustip = "Arrange the windows. Command: SYSWINDOWS",
        .alias = "WINDOWS, SYSWINDOWS",
        .shortcut = ""
    },
    {
        .id = ACTION_TEXT_BOLD,
        .main = text_bold_command,
        .arguments = "",
        .icon = "textbold",
        .tooltip = "&Bold Text",
        .statustip = "Sets text to be bold. Command: BOLD.",
        .alias = "BOLD",
        .shortcut = ""
    },
    {
        .id = ACTION_TEST,
        .main = test_command,
        .arguments = "",
        .icon = "test",
        .tooltip = "&Test",
        .statustip = "Run a sequence of commands for QA. Command: TEST.",
        .alias = "TEST",
        .shortcut = ""
    },
    {
        .id = ACTION_TEXT_ITALIC,
        .main = text_italic_command,
        .arguments = "",
        .icon = "textitalic",
        .tooltip = "&Italic Text",
        .statustip = "Sets text to be italic. Command: ITALIC.",
        .alias = "ITALIC",
        .shortcut = ""
    },
    {
        .id = ACTION_TEXT_UNDERLINE,
        .main = text_underline_command,
        .arguments = "",
        .icon = "textunderline",
        .tooltip = "&Underline Text",
        .statustip = "Sets text to be underlined. Command: UNDERLINE.",
        .alias = "UNDERLINE",
        .shortcut = ""
    },
    {
        .id = ACTION_TEXT_STRIKEOUT,
        .main = text_strikeout_command,
        .arguments = "",
        .icon = "textstrikeout",
        .tooltip = "&StrikeOut Text",
        .statustip = "Sets text to be striked out. Command: STRIKEOUT.",
        .alias = "STRIKEOUT",
        .shortcut = ""
    },
    {
        .id = ACTION_TEXT_OVERLINE,
        .main = text_overline_command,
        .arguments = "",
        .icon = "textoverline",
        .tooltip = "&Overline Text",
        .statustip = "Sets text to be overlined. Command: OVERLINE.",
        .alias = "OVERLINE",
        .shortcut = ""
    },
    {
        .id = ACTION_THAW_ALL_LAYERS,
        .main = thawalllayers_command,
        .arguments = "",
        .icon = "thawalllayers",
        .tooltip = "&Thaw All Layers",
        .statustip = "Thaws all layers in the current drawing:  THAWALL",
        .alias = "THAWALL",
        .shortcut = ""
    },
    {
        .id = ACTION_TIP_OF_THE_DAY,
        .main = tipoftheday_command,
        .arguments = "",
        .icon = "tipoftheday",
        .tooltip = "&Tip Of The Day",
        .statustip = "Displays a dialog with useful tips:  TIPS",
        .alias = "TIPS, TIPOFTHEDAY",
        .shortcut = ""
    },
    {
        .id = ACTION_TODO,
        .main = todo_command,
        .arguments = "",
        .icon = "todo",
        .tooltip = "&About",
        .statustip = "Displays information about this product:  ABOUT",
        .alias = "TODO",
        .shortcut = ""
    },
    {
        .id = ACTION_UNDO,
        .main = undo_command,
        .arguments = "",
        .icon = "undo",
        .tooltip = "&Undo",
        .statustip = "Reverses the most recent action. Command: UNDO.",
        .alias = "U, UNDO",
        .shortcut = "Ctrl+Z"
    },
    {
        .id = ACTION_UNLOCK_ALL_LAYERS,
        .main = unlockalllayers_command,
        .arguments = "",
        .icon = "unlockalllayers",
        .tooltip = "&Unlock All Layers",
        .statustip = "Unlocks all layers in the current drawing:  UNLOCKALL",
        .alias = "UNLOCKALL",
        .shortcut = ""
    },
    {
        .id = ACTION_VULCANIZE,
        .main = vulcanize_command,
        .arguments = "",
        .icon = "vulcanize",
        .tooltip = "&Undo",
        .statustip = "Reverses the most recent action:  UNDO",
        .alias = "VULCANIZE",
        .shortcut = ""
    },
    {
        .id = ACTION_WINDOW_CASCADE,
        .main = windowcascade_command,
        .arguments = "",
        .icon = "windowcascade",
        .tooltip = "&Cascade",
        .statustip = "Cascade the windows. Command: CASCADE.",
        .alias = "CASCADE, WINDOWCASCADE",
        .shortcut = ""
    },
    {
        .id = ACTION_WHATS_THIS,
        .main = whats_this_command,
        .arguments = "",
        .icon = "whatsthis",
        .tooltip = "&What's This?",
        .statustip = "What's This? Context Help! Command: WHATSTHIS.",
        .alias = "WHATSTHIS",
        .shortcut = ""
    },
    {
        .id = ACTION_WINDOW_CLOSE,
        .main = windowclose_command,
        .arguments = "",
        .icon = "windowclose",
        .tooltip = "Cl&ose",
        .statustip = "Close the active window. Command: CLOSE.",
        .alias = "CLOSE, WINDOWCLOSE",
        .shortcut = "Ctrl+W"
    },
    {
        .id = ACTION_WINDOW_CLOSE_ALL,
        .main = windowcloseall_command,
        .arguments = "",
        .icon = "windowcloseall",
        .tooltip = "Close &All",
        .statustip = "Close all the windows. Command: CLOSEALL",
        .alias = "CLOSEALL, WINDOWCLOSEALL",
        .shortcut = ""
    },
    {
        .id = ACTION_WINDOW_NEXT,
        .main = windownext_command,
        .arguments = "",
        .icon = "windownext",
        .tooltip = "Ne&xt",
        .statustip = "Move the focus to the next window:  NEXT",
        .alias = "NEXT, WINDOWNEXT",
        .shortcut = "Ctrl+Tab"
    },
    {
        .id = ACTION_WINDOW_PREVIOUS,
        .main = windowprevious_command,
        .arguments = "",
        .icon = "windowprevious",
        .tooltip = "Pre&vious",
        .statustip = "Move the focus to the previous window:  PREVIOUS",
        .alias = "PREV, PREVIOUS, WINDOWPREVIOUS",
        .shortcut = "Ctrl+Shift+Tab"
    },
    {
        .id = ACTION_WINDOW_TILE,
        .main = windowtile_command,
        .arguments = "",
        .icon = "windowtile",
        .tooltip = "&Tile",
        .statustip = "Tile the windows:  TILE",
        .alias = "TILE, WINDOWTILE",
        .shortcut = ""
    },
    {
        .id = ACTION_ZOOM_ALL,
        .main = zoom_all_command,
        .arguments = "",
        .icon = "zoomall",
        .tooltip = "Zoom &All",
        .statustip = "Zooms to display the drawing extents or the grid limits. Command: ZOOMALL.",
        .alias = "ZOOMALL",
        .shortcut = ""
    },
    {
        .id = ACTION_ZOOM_CENTER,
        .main = zoom_center_command,
        .arguments = "",
        .icon = "zoomcenter",
        .tooltip = "Zoom &Center",
        .statustip = "Zooms to display a view specified by a center point and magnification or height. Command: ZOOMCENTER.",
        .alias = "ZOOMCENTER",
        .shortcut = ""
    },
    {
        .id = ACTION_ZOOM_DYNAMIC,
        .main = zoom_dynamic_command,
        .arguments = "",
        .icon = "zoomdynamic",
        .tooltip = "Zoom &Dynamic",
        .statustip = "Zooms to display the generated portion of the drawing. Command: ZOOMDYNAMIC.",
        .alias = "ZOOMDYNAMIC",
        .shortcut = ""
    },
    {
        .id = ACTION_ZOOM_EXTENTS,
        .main = zoom_extents_command,
        .arguments = "",
        .icon = "zoomextents",
        .tooltip = "Zoom &Extents",
        .statustip = "Zooms to display the drawing extents. Command: ZOOMEXTENTS",
        .alias = "ZOOMEXTENTS",
        .shortcut = ""
    },
    {
        .id = ACTION_ZOOM_IN,
        .main = zoom_in_command,
        .arguments = "",
        .icon = "zoomin",
        .tooltip = "Zoom &In",
        .statustip = "Zooms to increase the apparent size of objects. Command: ZOOMIN",
        .alias = "ZOOMIN",
        .shortcut = "Ctrl+Plus"
    },
    {
        .id = ACTION_ZOOM_OUT,
        .main = zoom_out_command,
        .arguments = "",
        .icon = "zoomout",
        .tooltip = "Zoom &Out",
        .statustip = "Zooms to decrease the apparent size of objects. Command: ZOOMOUT",
        .alias = "ZOOMOUT",
        .shortcut = "Ctrl+Minus"
    },
    {
        .id = ACTION_ZOOM_PREVIOUS,
        .main = zoom_previous_command,
        .arguments = "",
        .icon = "zoomprevious",
        .tooltip = "Zoom &Previous",
        .statustip = "Zooms to display the previous view. Command: ZOOMPREVIOUS.",
        .alias = "ZOOMPREVIOUS",
        .shortcut = ""
    },
    {
        .id = ACTION_ZOOM_REAL_TIME,
        .main = zoom_real_time_command,
        .arguments = "",
        .icon = "zoomrealtime",
        .tooltip = "Zoom &Real Time",
        .statustip = "Zooms to increase or decrease the apparent size of objects in the current viewport. Command: ZOOMREALTIME",
        .alias = "ZOOMREALTIME",
        .shortcut = ""
    },
    {
        .id = ACTION_ZOOM_SCALE,
        .main = zoom_scale_command,
        .arguments = "",
        .icon = "zoomscale",
        .tooltip = "Zoom &Scale",
        .statustip = "Zooms the display using a specified scale factor. Command: ZOOMSCALE.",
        .alias = "ZOOMSCALE",
        .shortcut = ""
    },
    {
        .id = ACTION_ZOOM_SELECTED,
        .main = zoom_selected_command,
        .arguments = "",
        .icon = "zoomselected",
        .tooltip = "Zoom Selec&ted",
        .statustip = "Zooms to display the selected objects. Command: ZOOMSELECTED.",
        .alias = "ZOOMSELECTED",
        .shortcut = ""
    },
    {
        .id = ACTION_ZOOM_WINDOW,
        .main = zoom_window_command,
        .arguments = "",
        .icon = "zoomwindow",
        .tooltip = "Zoom &Window",
        .statustip = "Zooms to display an area specified by a rectangular window. Command: ZOOMIN",
        .alias = "ZOOMWINDOW",
        .shortcut = ""
    },
    {
        .id = -2,
        .main = do_nothing,
        .arguments = "",
        .icon = "blank",
        .tooltip = "NULL",
        .statustip = "NULL",
        .alias = "NULL",
        .shortcut = ""
    }
};
