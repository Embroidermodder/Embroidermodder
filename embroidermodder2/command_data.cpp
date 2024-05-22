/*
 * Embroidermodder 2.
 *
 * Copyright 2013-2024 The Embroidermodder Team
 * Embroidermodder 2 is Open Source Software, see LICENSE for licensing terms.
 * Read CODE-STANDARDS.txt for advice on altering this file.
 *
 * Command Data
 */

#include "command_data.h"

ScriptValue about_command(ScriptEnv*);
ScriptValue alert_command(ScriptEnv*);
ScriptValue angle_command(ScriptEnv*);
ScriptValue circle_command(ScriptEnv*);
ScriptValue clear_command(ScriptEnv*);
ScriptValue day_command(ScriptEnv *);
ScriptValue debug_command(ScriptEnv *);
ScriptValue disable_command(ScriptEnv*);
ScriptValue distance_command(ScriptEnv*);
ScriptValue dolphin_command(ScriptEnv*);
ScriptValue ellipse_command(ScriptEnv *);
ScriptValue enable_command(ScriptEnv *);
ScriptValue erase_command(ScriptEnv *);
ScriptValue error_command(ScriptEnv *);
ScriptValue exit_command(ScriptEnv *);
ScriptValue heart_command(ScriptEnv *);
ScriptValue heart_command(ScriptEnv*);
ScriptValue help_command(ScriptEnv*);
ScriptValue icon128_command(ScriptEnv*);
ScriptValue icon16_command(ScriptEnv*);
ScriptValue icon24_command(ScriptEnv*);
ScriptValue icon32_command(ScriptEnv*);
ScriptValue icon48_command(ScriptEnv*);
ScriptValue icon64_command(ScriptEnv*);
ScriptValue get_command(ScriptEnv*);
ScriptValue line_command(ScriptEnv*);
ScriptValue locatepoint_command(ScriptEnv*);
ScriptValue mirrorselected_command(ScriptEnv*);
ScriptValue move_command(ScriptEnv*);
ScriptValue moveselected_command(ScriptEnv*);
ScriptValue new_command(ScriptEnv*);
ScriptValue night_command(ScriptEnv*);
ScriptValue open_command(ScriptEnv*);
ScriptValue print_command(ScriptEnv*);
ScriptValue redo_command(ScriptEnv*);
ScriptValue pandown_command(ScriptEnv*);
ScriptValue panleft_command(ScriptEnv*);
ScriptValue panright_command(ScriptEnv*);
ScriptValue panup_command(ScriptEnv*);
ScriptValue path_command(ScriptEnv*);
ScriptValue platform_command(ScriptEnv*);
ScriptValue previewoff_command(ScriptEnv*);
ScriptValue previewon_command(ScriptEnv*);
ScriptValue point_command(ScriptEnv*);
ScriptValue polygon_command(ScriptEnv*);
ScriptValue polyline_command(ScriptEnv*);
ScriptValue rectangle_command(ScriptEnv*);
ScriptValue rotate_command(ScriptEnv*);
ScriptValue rgb_command(ScriptEnv*);
ScriptValue quickleader_command(ScriptEnv*);
ScriptValue save_command(ScriptEnv*);
ScriptValue sandbox_command(ScriptEnv*);
ScriptValue scale_command(ScriptEnv*);
ScriptValue scaleselected_command(ScriptEnv*);
ScriptValue selectall_command(ScriptEnv*);
ScriptValue singlelinetext_command(ScriptEnv*);
ScriptValue set_command(ScriptEnv*);
ScriptValue snowflake_command(ScriptEnv*);
ScriptValue star_command(ScriptEnv*);
ScriptValue syswindows_command(ScriptEnv*);
ScriptValue tipoftheday_command(ScriptEnv*);
ScriptValue todo_command(ScriptEnv*);
ScriptValue undo_command(ScriptEnv*);
ScriptValue vulcanize_command(ScriptEnv*);
ScriptValue windowcascade_command(ScriptEnv*);
ScriptValue windowclose_command(ScriptEnv*);
ScriptValue windowcloseall_command(ScriptEnv*);
ScriptValue windownext_command(ScriptEnv*);
ScriptValue windowprevious_command(ScriptEnv*);
ScriptValue windowtile_command(ScriptEnv*);
ScriptValue zoomextents_command(ScriptEnv*);
ScriptValue zoomin_command(ScriptEnv*);
ScriptValue zoomout_command(ScriptEnv*);

Command command_data[] = {
    {
        .id = ACTION_about,
        .main = about_command,
        .icon = "about",
        .menu_name = "None",
        .menu_position = 0,
        .toolbar_name = "None",
        .toolbar_position = 0,
        .tooltip = "&About Embroidermodder 2",
        .statustip = "Displays information about this product. Command: ABOUT.",
        .alias = "ABOUT"
    },
    {
        .id = ACTION_ALERT,
        .main = alert_command,
        .icon = "warning",
        .menu_name = "None",
        .menu_position = 0,
        .toolbar_name = "None",
        .toolbar_position = 0,
        .tooltip = "&Alert",
        .statustip = "Creates a dialog to alert the user. Command: ALERT.",
        .alias = "ALERT"
    },
    {
        .id = -1,
        .main = angle_command,
        .icon = "angle",
        .menu_name = "None",
        .menu_position = 0,
        .toolbar_name = "None",
        .toolbar_position = 0,
        .tooltip = "&Angle",
        .statustip = "Calculate the angle between two lines and display it. Command: ANGLE, CALCANGLE",
        .alias = "ANGLE, CALCANGLE"
    },
    {
        .id = -1,
        .main = circle_command,
        .icon = "circle",
        .menu_name = "Draw",
        .menu_position = 8,
        .toolbar_name = "Draw",
        .toolbar_position = 8,
        .tooltip = "&Circle",
        .statustip = "Creates a circle. Command: CIRCLE.",
        .alias = "C, CIRCLE"
    },
    {
        .id = ACTION_CLEAR,
        .main = clear_command,
        .icon = "clear",
        .menu_name = "None",
        .menu_position = 0,
        .toolbar_name = "None",
        .toolbar_position = 0,
        .tooltip = "&Do Nothing",
        .statustip = "Does Nothing. Command: DONOTHING.",
        .alias = "DONOTHING"
    },
    {
        .id = ACTION_day,
        .main = day_command,
        .icon = "day",
        .menu_name = "None",
        .menu_position = 0,
        .toolbar_name = "None",
        .toolbar_position = 0,
        .tooltip = "&Day",
        .statustip = "Updates the current view using day vision settings. Command: DAY",
        .alias = "DAY"
    },
    {
        .id = -1,
        .main = debug_command,
        .icon = "about",
        .menu_name = "None",
        .menu_position = 0,
        .toolbar_name = "None",
        .toolbar_position = 0,
        .tooltip = "Debug",
        .statustip = "Displays information about this product:  ABOUT",
        .alias = "DEBUG"
    },
    {
        .id = -1,
        .main = disable_command,
        .icon = "disable",
        .menu_name = "None",
        .menu_position = 0,
        .toolbar_name = "None",
        .toolbar_position = 0,
        .tooltip = "&Disable",
        .statustip = "Disable one of the pre-defined global boolean variables.",
        .alias = "DISABLE"
    },
    {
        .id = -1,
        .main = distance_command,
        .icon = "distance",
        .menu_name = "Tools",
        .menu_position = 7,
        .toolbar_name = "Inquiry",
        .toolbar_position = 0,
        .tooltip = "&Distance",
        .statustip = "Measures the distance and angle between two points. Command: DIST",
        .alias = "DI, DIST, DISTANCE"
    },
    {
        .id = -1,
        .main = dolphin_command,
        .icon = "dolphin",
        .menu_name = "Draw",
        .menu_position = 100,
        .toolbar_name = "Draw",
        .toolbar_position = 100,
        .tooltip = "&Dolphin",
        .statustip = "Creates a dolphin. Command: DOLPHIN.",
        .alias = "DOLPHIN"
    },
    {
        .id = ACTION_donothing,
        .main = do_nothing,
        .icon = "donothing",
        .menu_name = "None",
        .menu_position = 0,
        .toolbar_name = "None",
        .toolbar_position = 0,
        .tooltip = "&Do Nothing",
        .statustip = "Does Nothing. Command: DONOTHING.",
        .alias = "DONOTHING"
    },
    {
        .id = -1,
        .main = ellipse_command,
        .icon = "ellipse",
        .menu_name = "Draw",
        .menu_position = 8,
        .toolbar_name = "Draw",
        .toolbar_position = 8,
        .tooltip = "&Ellipse",
        .statustip = "Creates an ellipse. Command: ELLIPSE.",
        .alias = "EL, ELLIPSE"
    },
    {
        .id = -1,
        .main = enable_command,
        .icon = "enable",
        .menu_name = "None",
        .menu_position = 0,
        .toolbar_name = "None",
        .toolbar_position = 0,
        .tooltip = "&Enable",
        .statustip = "Enable a pre-defined global variables.",
        .alias = "ENABLE"
    },
    {
        .id = -1,
        .main = erase_command,
        .icon = "erase",
        .menu_name = "Modify",
        .menu_position = 5,
        .toolbar_name = "Modify",
        .toolbar_position = 0,
        .tooltip = "D&elete",
        .statustip = "Removes objects from a drawing. Command: DELETE.",
        .alias = "E, ERASE, DEL, DELETE"
    },
    {
        .id = -1,
        .main = error_command,
        .icon = "error",
        .menu_name = "None",
        .menu_position = 0,
        .toolbar_name = "None",
        .toolbar_position = 0,
        .tooltip = "Error",
        .statustip = "Sends an error message to the user. Command: ERROR",
        .alias = "ERROR"
    },
    {
        .id = -1,
        .main = exit_command,
        .icon = "exit",
        .menu_name = "None",
        .menu_position = 0,
        .toolbar_name = "None",
        .toolbar_position = 0,
        .tooltip = "E&xit",
        .statustip = "Exit the application:  EXIT",
        .alias = "EXIT, QUIT"
    },
    {
        .id = -1,
        .main = get_command,
        .icon = "get",
        .menu_name = "None",
        .menu_position = 0,
        .toolbar_name = "None",
        .toolbar_position = 0,
        .tooltip = "&Get",
        .statustip = "Print a value to one of the pre-defined global variables.",
        .alias = "SET"
    },
    {
        .id = -1,
        .main = heart_command,
        .icon = "heart",
        .menu_name = "Draw",
        .menu_position = 100,
        .toolbar_name = "Draw",
        .toolbar_position = 100,
        .tooltip = "&Heart",
        .statustip = "Creates a heart:  HEART",
        .alias = "HEART"
    },
    {
        .id = -1,
        .main = help_command,
        .icon = "help",
        .menu_name = "None",
        .menu_position = 0,
        .toolbar_name = "None",
        .toolbar_position = 0,
        .tooltip = "&Help",
        .statustip = "Displays the help file:  HELP",
        .alias = "?, HELP"
    },
    {
        .id = -1,
        .main = icon128_command,
        .icon = "icon128",
        .menu_name = "None",
        .menu_position = 0,
        .toolbar_name = "None",
        .toolbar_position = 0,
        .tooltip = "Icon12&8",
        .statustip = "Sets the toolbar icon size to 128x128:  ICON128",
        .alias = "ICON128"
    },
    {
        .id = -1,
        .main = icon16_command,
        .icon = "icon16",
        .menu_name = "None",
        .menu_position = 0,
        .toolbar_name = "None",
        .toolbar_position = 0,
        .tooltip = "Icon&16",
        .statustip = "Sets the toolbar icon size to 16x16:  ICON16",
        .alias = "ICON16"
    },
    {
        .id = -1,
        .main = icon24_command,
        .icon = "icon24",
        .menu_name = "None",
        .menu_position = 0,
        .toolbar_name = "None",
        .toolbar_position = 0,
        .tooltip = "Icon&24",
        .statustip = "Sets the toolbar icon size to 24x24:  ICON24",
        .alias = "ICON24"
    },
    {
        .id = -1,
        .main = icon32_command,
        .icon = "icon32",
        .menu_name = "None",
        .menu_position = 0,
        .toolbar_name = "None",
        .toolbar_position = 0,
        .tooltip = "Icon&32",
        .statustip = "Sets the toolbar icon size to 32x32:  ICON32",
        .alias = "ICON32"
    },
    {
        .id = -1,
        .main = icon48_command,
        .icon = "icon48",
        .menu_name = "None",
        .menu_position = 0,
        .toolbar_name = "None",
        .toolbar_position = 0,
        .tooltip = "Icon&48",
        .statustip = "Sets the toolbar icon size to 48x48:  ICON48",
        .alias = "ICON48"
    },
    {
        .id = -1,
        .main = icon64_command,
        .icon = "ICON64",
        .menu_name = "None",
        .menu_position = 0,
        .toolbar_name = "None",
        .toolbar_position = 0,
        .tooltip = "Icon&64",
        .statustip = "Sets the toolbar icon size to 64x64:  ICON64",
        .alias = "ICON64"
    },
    {
        .id = -1,
        .main = line_command,
        .icon = "line",
        .menu_name = "Draw",
        .menu_position = 0,
        .toolbar_name = "Draw",
        .toolbar_position = 0,
        .tooltip = "&Line",
        .statustip = "Creates straight line segments:  LINE",
        .alias = "L, LINE"
    },
    {
        .id = -1,
        .main = locatepoint_command,
        .icon = "locatepoint",
        .menu_name = "Tools",
        .menu_position = 11,
        .toolbar_name = "Inquiry",
        .toolbar_position = 0,
        .tooltip = "&Locate Point",
        .statustip = "Displays the coordinate values of a location:  ID",
        .alias = "ID, LOCATEPOINT"
    },
    {
        .id = -1,
        .main = mirrorselected_command,
        .icon = "mirror",
        .menu_name = "None",
        .menu_position = 0,
        .toolbar_name = "None",
        .toolbar_position = 0,
        .tooltip = "&Mirror Selected",
        .statustip = "Command: MIRRORSELECTED.",
        .alias = "MIRRORSELECTED"
    },
    {
        .id = -1,
        .main = move_command,
        .icon = "move",
        .menu_name = "Modify",
        .menu_position = 10,
        .toolbar_name = "Modify",
        .toolbar_position = 5,
        .tooltip = "&Move",
        .statustip = "Displaces objects a specified distance in a specified direction:  MOVE",
        .alias = "M, MOVE"
    },
    {
        .id = -1,
        .main = moveselected_command,
        .icon = "moveselected",
        .menu_name = "None",
        .menu_position = 0,
        .toolbar_name = "None",
        .toolbar_position = 0,
        .tooltip = "&Do Nothing",
        .statustip = "Does Nothing.",
        .alias = "DONOTHING"
    },
    {
        .id = ACTION_new,
        .main = new_command,
        .icon = "new",
        .menu_name = "None",
        .menu_position = 0,
        .toolbar_name = "None",
        .toolbar_position = 0,
        .tooltip = "&New",
        .statustip = "Create a new file. Command: NEW.",
        .alias = "NEW"
    },
    {
        .id = -1,
        .main = night_command,
        .icon = "night",
        .menu_name = "None",
        .menu_position = 0,
        .toolbar_name = "None",
        .toolbar_position = 0,
        .tooltip = "&Night",
        .statustip = "Updates the current view using night vision settings:  NIGHT",
        .alias = "NIGHT"
    },
    {
        .id = ACTION_open,
        .main = open_command,
        .icon = "open",
        .menu_name = "None",
        .menu_position = 0,
        .toolbar_name = "None",
        .toolbar_position = 0,
        .tooltip = "&Open",
        .statustip = "Open an existing file. Command: OPEN.",
        .alias = "OPEN"
    },
    {
        .id = -1,
        .main = pandown_command,
        .icon = "pandown",
        .menu_name = "None",
        .menu_position = 0,
        .toolbar_name = "None",
        .toolbar_position = 0,
        .tooltip = "&Pan Down",
        .statustip = "Moves the view down:  PANDOWN",
        .alias = "PANDOWN"
    },
    {
        .id = -1,
        .main = panleft_command,
        .icon = "panleft",
        .menu_name = "None",
        .menu_position = 0,
        .toolbar_name = "None",
        .toolbar_position = 0,
        .tooltip = "&Pan Left",
        .statustip = "Moves the view to the left:  PANLEFT",
        .alias = "PANLEFT"
    },
    {
        .id = -1,
        .main = panright_command,
        .icon = "panright",
        .menu_name = "None",
        .menu_position = 0,
        .toolbar_name = "None",
        .toolbar_position = 0,
        .tooltip = "&Pan Right",
        .statustip = "Moves the view to the right:  PANRIGHT",
        .alias = "PANRIGHT"
    },
    {
        .id = -1,
        .main = panup_command,
        .icon = "panup",
        .menu_name = "None",
        .menu_position = 0,
        .toolbar_name = "None",
        .toolbar_position = 0,
        .tooltip = "&Pan Up",
        .statustip = "Moves the view up:  PANUP",
        .alias = "PANUP"
    },
    {
        .id = -1,
        .main = path_command,
        .icon = "path",
        .menu_name = "None",
        .menu_position = 0,
        .toolbar_name = "None",
        .toolbar_position = 0,
        .tooltip = "Icon&24",
        .statustip = "Sets the toolbar icon size to 24x24:  ICON24",
        .alias = "ICON24"
    },
    {
        .id = -1,
        .main = platform_command,
        .icon = "platform",
        .menu_name = "None",
        .menu_position = 0,
        .toolbar_name = "None",
        .toolbar_position = 0,
        .tooltip = "&Platform",
        .statustip = "List which platform is in use. Command: PLATFORM.",
        .alias = "PLATFORM"
    },
    {
        .id = -1,
        .main = point_command,
        .icon = "point",
        .menu_name = "None",
        .menu_position = 0,
        .toolbar_name = "None",
        .toolbar_position = 0,
        .tooltip = "Icon&24",
        .statustip = "Sets the toolbar icon size to 24x24:  ICON24",
        .alias = "ICON24"
    },
    {
        .id = -1,
        .main = polygon_command,
        .icon = "polygon",
        .menu_name = "Draw",
        .menu_position = 4,
        .toolbar_name = "Draw",
        .toolbar_position = 4,
        .tooltip = "Pol&ygon",
        .statustip = "Creates a regular polygon. Command: POLYGON.",
        .alias = "POL, POLYGON"
    },
    {
        .id = -1,
        .main = polyline_command,
        .icon = "polyline",
        .menu_name = "None",
        .menu_position = 0,
        .toolbar_name = "None",
        .toolbar_position = 0,
        .tooltip = "Icon&24",
        .statustip = "Sets the toolbar icon size to 24x24:  ICON24",
        .alias = "ICON24"
    },
    {
        .id = -1,
        .main = previewoff_command,
        .icon = "previewoff",
        .menu_name = "None",
        .menu_position = 0,
        .toolbar_name = "None",
        .toolbar_position = 0,
        .tooltip = "&Do Nothing",
        .statustip = "Does Nothing.",
        .alias = "DONOTHING"
    },
    {
        .id = -1,
        .main = previewon_command,
        .icon = "previewon",
        .menu_name = "None",
        .menu_position = 0,
        .toolbar_name = "None",
        .toolbar_position = 0,
        .tooltip = "&Do Nothing",
        .statustip = "Does Nothing.",
        .alias = "DONOTHING"
    },
    {
        .id = -1,
        .main = print_command,
        .icon = "print",
        .menu_name = "None",
        .menu_position = 0,
        .toolbar_name = "None",
        .toolbar_position = 0,
        .tooltip = "&Print",
        .statustip = "Displays information about this product:  ABOUT",
        .alias = "ABOUT"
    },
    {
        .id = -1,
        .main = quickleader_command,
        .icon = "quickleader",
        .menu_name = "None",
        .menu_position = 0,
        .toolbar_name = "None",
        .toolbar_position = 0,
        .tooltip = "Icon&24",
        .statustip = "Sets the toolbar icon size to 24x24:  ICON24",
        .alias = "ICON24"
    },
    {
        .id = -1,
        .main = rectangle_command,
        .icon = "rectangle",
        .menu_name = "Draw",
        .menu_position = 6,
        .toolbar_name = "Draw",
        .toolbar_position = 6,
        .tooltip = "&Rectangle",
        .statustip = "Creates a rectangular polyline. Command: RECTANGLE.",
        .alias = "REC, RECT, RECTANG, RECTANGLE"
    },
    {
        .id = -1,
        .main = redo_command,
        .icon = "redo",
        .menu_name = "None",
        .menu_position = 0,
        .toolbar_name = "None",
        .toolbar_position = 0,
        .tooltip = "&Redo",
        .statustip = "Reverses the effects of the previous undo action:  REDO",
        .alias = "REDO"
    },
    {
        .id = -1,
        .main = rgb_command,
        .icon = "rgb",
        .menu_name = "None",
        .menu_position = 0,
        .toolbar_name = "None",
        .toolbar_position = 0,
        .tooltip = "Icon&24",
        .statustip = "Sets the toolbar icon size to 24x24:  ICON24",
        .alias = "ICON24"
    },
    {
        .id = -1,
        .main = rotate_command,
        .icon = "rotate",
        .menu_name = "None",
        .menu_position = 0,
        .toolbar_name = "None",
        .toolbar_position = 0,
        .tooltip = "Icon&24",
        .statustip = "Sets the toolbar icon size to 24x24:  ICON24",
        .alias = "ICON24"
    },
    {
        .id = -1,
        .main = sandbox_command,
        .icon = "sandbox",
        .menu_name = "None",
        .menu_position = 0,
        .toolbar_name = "None",
        .toolbar_position = 0,
        .tooltip = "Icon&24",
        .statustip = "Sets the toolbar icon size to 24x24:  ICON24",
        .alias = "ICON24"
    },
    {
        .id = -1,
        .main = save_command,
        .icon = "save",
        .menu_name = "None",
        .menu_position = 0,
        .toolbar_name = "None",
        .toolbar_position = 0,
        .tooltip = "&Do Nothing",
        .statustip = "Does Nothing.",
        .alias = "DONOTHING"
    },
    {
        .id = -1,
        .main = scale_command,
        .icon = "scale",
        .menu_name = "None",
        .menu_position = 0,
        .toolbar_name = "None",
        .toolbar_position = 0,
        .tooltip = "Icon&24",
        .statustip = "Sets the toolbar icon size to 24x24:  ICON24",
        .alias = "SCALE"
    },
    {
        .id = -1,
        .main = scaleselected_command,
        .icon = "scaleselected",
        .menu_name = "None",
        .menu_position = 0,
        .toolbar_name = "None",
        .toolbar_position = 0,
        .tooltip = "&Do Nothing",
        .statustip = "Does Nothing.",
        .alias = "SCALESELECTED"
    },
    {
        .id = -1,
        .main = selectall_command,
        .icon = "selectall",
        .menu_name = "None",
        .menu_position = 0,
        .toolbar_name = "None",
        .toolbar_position = 0,
        .tooltip = "Icon&24",
        .statustip = "Sets the toolbar icon size to 24x24:  ICON24",
        .alias = "SELECTALL, SELALL"
    },
    {
        .id = -1,
        .main = set_command,
        .icon = "set",
        .menu_name = "None",
        .menu_position = 0,
        .toolbar_name = "None",
        .toolbar_position = 0,
        .tooltip = "&Set",
        .statustip = "Assign a value to one of the pre-defined global variables.",
        .alias = "SET"
    },
    {
        .id = -1,
        .main = singlelinetext_command,
        .icon = "singlelinetext",
        .menu_name = "None",
        .menu_position = 0,
        .toolbar_name = "None",
        .toolbar_position = 0,
        .tooltip = "Icon&24",
        .statustip = "Sets the toolbar icon size to 24x24:  ICON24",
        .alias = "SINGLELINETEXT"
    },
    {
        .id = -1,
        .main = snowflake_command,
        .icon = "snowflake",
        .menu_name = "None",
        .menu_position = 0,
        .toolbar_name = "None",
        .toolbar_position = 0,
        .tooltip = "Icon&24",
        .statustip = "Sets the toolbar icon size to 24x24:  ICON24",
        .alias = "SNOWFLAKE"
    },
    {
        .id = -1,
        .main = star_command,
        .icon = "star",
        .menu_name = "None",
        .menu_position = 0,
        .toolbar_name = "None",
        .toolbar_position = 0,
        .tooltip = "Icon&24",
        .statustip = "Sets the toolbar icon size to 24x24:  ICON24",
        .alias = "STAR"
    },
    {
        .id = -1,
        .main = syswindows_command,
        .icon = "syswindows",
        .menu_name = "None",
        .menu_position = 0,
        .toolbar_name = "None",
        .toolbar_position = 0,
        .tooltip = "&SysWindows",
        .statustip = "Arrange the windows. Command: SYSWINDOWS",
        .alias = "WINDOWS, SYSWINDOWS"
    },
    {
        .id = -1,
        .main = tipoftheday_command,
        .icon = "tipoftheday",
        .menu_name = "None",
        .menu_position = 0,
        .toolbar_name = "None",
        .toolbar_position = 0,
        .tooltip = "&Tip Of The Day",
        .statustip = "Displays a dialog with useful tips:  TIPS",
        .alias = "TIPS, TIPOFTHEDAY"
    },
    {
        .main = todo_command,
        .icon = "todo",
        .menu_name = "None",
        .menu_position = 0,
        .toolbar_name = "None",
        .toolbar_position = 0,
        .tooltip = "&About",
        .statustip = "Displays information about this product:  ABOUT",
        .alias = "TODO"
    },
    {
        .id = -1,
        .main = undo_command,
        .icon = "undo",
        .menu_name = "None",
        .menu_position = 0,
        .toolbar_name = "None",
        .toolbar_position = 0,
        .tooltip = "&Undo",
        .statustip = "Reverses the most recent action:  UNDO",
        .alias = "U, UNDO"
    },
    {
        .id = -1,
        .main = vulcanize_command,
        .icon = "vulcanize",
        .menu_name = "None",
        .menu_position = 0,
        .toolbar_name = "None",
        .toolbar_position = 0,
        .tooltip = "&Undo",
        .statustip = "Reverses the most recent action:  UNDO",
        .alias = "VULCANIZE"
    },
    {
        .id = ACTION_windowcascade,
        .main = windowcascade_command,
        .icon = "windowcascade",
        .menu_name = "None",
        .menu_position = 0,
        .toolbar_name = "None",
        .toolbar_position = 0,
        .tooltip = "&Cascade",
        .statustip = "Cascade the windows. Command: CASCADE.",
        .alias = "CASCADE, WINDOWCASCADE"
    },
    {
        .id = ACTION_windowclose,
        .main = windowclose_command,
        .icon = "windowcascade",
        .menu_name = "None",
        .menu_position = 0,
        .toolbar_name = "None",
        .toolbar_position = 0,
        .tooltip = "Cl&ose",
        .statustip = "Close the active window. Command: CLOSE.",
        .alias = "CLOSE, WINDOWCLOSE"
    },
    {
        .id = ACTION_windowcloseall,
        .main = windowcloseall_command,
        .icon = "windowcloseall",
        .menu_name = "None",
        .menu_position = 0,
        .toolbar_name = "None",
        .toolbar_position = 0,
        .tooltip = "Close &All",
        .statustip = "Close all the windows. Command: CLOSEALL",
        .alias = "CLOSEALL, WINDOWCLOSEALL"
    },
    {
        .id = -1,
        .main = windownext_command,
        .icon = "windownext",
        .menu_name = "None",
        .menu_position = 0,
        .toolbar_name = "None",
        .toolbar_position = 0,
        .tooltip = "Ne&xt",
        .statustip = "Move the focus to the next window:  NEXT",
        .alias = "NEXT, WINDOWNEXT"
    },
    {
        .id = -1,
        .main = windowprevious_command,
        .icon = "windowprevious",
        .menu_name = "None",
        .menu_position = 0,
        .toolbar_name = "None",
        .toolbar_position = 0,
        .tooltip = "Pre&vious",
        .statustip = "Move the focus to the previous window:  PREVIOUS",
        .alias = "PREV, PREVIOUS, WINDOWPREVIOUS"
    },
    {
        .id = -1,
        .main = windowtile_command,
        .icon = "windowtile",
        .menu_name = "None",
        .menu_position = 0,
        .toolbar_name = "None",
        .toolbar_position = 0,
        .tooltip = "&Tile",
        .statustip = "Tile the windows:  TILE",
        .alias = "TILE, WINDOWTILE"
    },
    {
        .id = -1,
        .main = zoomextents_command,
        .icon = "zoomextents",
        .menu_name = "None",
        .menu_position = 0,
        .toolbar_name = "None",
        .toolbar_position = 0,
        .tooltip = "Zoom &Extents",
        .statustip = "Zooms to display the drawing extents:  ZOOMEXTENTS",
        .alias = "ZOOMEXTENTS"
    },
    {
        .id = -1,
        .main = zoomin_command,
        .icon = "zoomin",
        .menu_name = "None",
        .menu_position = 0,
        .toolbar_name = "None",
        .toolbar_position = 0,
        .tooltip = "Zoom &In",
        .statustip = "Zooms to increase the apparent size of objects:  ZOOMIN",
        .alias = "ZOOMIN"
    },
    {
        .id = -1,
        .main = zoomout_command,
        .icon = "zoomout",
        .menu_name = "None",
        .menu_position = 0,
        .toolbar_name = "None",
        .toolbar_position = 0,
        .tooltip = "Zoom &Out",
        .statustip = "Zooms to decrease the apparent size of objects:  ZOOMOUT",
        .alias = "ZOOMOUT"
    },
    {
        .id = -1,
        .main = do_nothing,
        .icon = "blank",
        .menu_name = "None",
        .menu_position = -1,
        .toolbar_name = "None",
        .toolbar_position = -1,
        .tooltip = "NULL",
        .statustip = "NULL",
        .alias = "NULL"
    }
};
