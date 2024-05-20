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

/* Context-insensitive commands. */
ScriptValue about_general(ScriptEnv*);
ScriptValue alert_generic(ScriptEnv*);
ScriptValue circle_main(ScriptEnv*);
ScriptValue clear_generic(ScriptEnv*);
ScriptValue day_generic(ScriptEnv *);
ScriptValue distance_main(ScriptEnv*);
ScriptValue dolphin_main(ScriptEnv*);
ScriptValue ellipse_main(ScriptEnv *);
ScriptValue erase_main(ScriptEnv *);
ScriptValue exit_main(ScriptEnv *);
ScriptValue heart_generic(ScriptEnv *);
ScriptValue heart_main(ScriptEnv*);
ScriptValue help_main(ScriptEnv*);
ScriptValue icon128_generic(ScriptEnv*);
ScriptValue icon16_generic(ScriptEnv*);
ScriptValue icon24_generic(ScriptEnv*);
ScriptValue icon32_generic(ScriptEnv*);
ScriptValue icon48_generic(ScriptEnv*);
ScriptValue icon64_generic(ScriptEnv*);
ScriptValue get_prompt(ScriptEnv*);
ScriptValue line_main(ScriptEnv*);
ScriptValue locatepoint_main(ScriptEnv*);
ScriptValue move_main(ScriptEnv*);
ScriptValue moveselected_main(ScriptEnv*);
ScriptValue new_generic(ScriptEnv*);
ScriptValue night_main(ScriptEnv*);
ScriptValue open_main(ScriptEnv*);
ScriptValue print_main(ScriptEnv*);
ScriptValue redo_main(ScriptEnv*);
ScriptValue pandown_main(ScriptEnv*);
ScriptValue panleft_main(ScriptEnv*);
ScriptValue panright_main(ScriptEnv*);
ScriptValue panup_main(ScriptEnv*);
ScriptValue path_main(ScriptEnv*);
ScriptValue platform_main(ScriptEnv*);
ScriptValue previewoff_main(ScriptEnv*);
ScriptValue previewon_main(ScriptEnv*);
ScriptValue point_main(ScriptEnv*);
ScriptValue polygon_main(ScriptEnv*);
ScriptValue polyline_main(ScriptEnv*);
ScriptValue rectangle_main(ScriptEnv*);
ScriptValue rotate_main(ScriptEnv*);
ScriptValue rgb_main(ScriptEnv*);
ScriptValue quickleader_main(ScriptEnv*);
ScriptValue save_main(ScriptEnv*);
ScriptValue sandbox_main(ScriptEnv*);
ScriptValue scale_main(ScriptEnv*);
ScriptValue scaleselected_main(ScriptEnv*);
ScriptValue selectall_main(ScriptEnv*);
ScriptValue singlelinetext_main(ScriptEnv*);
ScriptValue set_prompt(ScriptEnv*);
ScriptValue snowflake_main(ScriptEnv*);
ScriptValue star_main(ScriptEnv*);
ScriptValue syswindows_main(ScriptEnv*);
ScriptValue tipoftheday_main(ScriptEnv*);
ScriptValue todo_main(ScriptEnv*);
ScriptValue undo_main(ScriptEnv*);
ScriptValue vulcanize_main(ScriptEnv*);
ScriptValue windowcascade_main(ScriptEnv*);
ScriptValue windowclose_main(ScriptEnv*);
ScriptValue windowcloseall_main(ScriptEnv*);
ScriptValue windownext_main(ScriptEnv*);
ScriptValue windowprevious_main(ScriptEnv*);
ScriptValue windowtile_main(ScriptEnv*);
ScriptValue zoomextents_main(ScriptEnv*);
ScriptValue zoomin_main(ScriptEnv*);
ScriptValue zoomout_main(ScriptEnv*);

Command command_data[] = {
    {
        .id = ACTION_about,
        .main = about_general,
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
        .main = alert_generic,
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
        .main = circle_main,
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
        .main = clear_generic,
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
        .id = ACTION_day,
        .main = day_generic,
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
        .main = distance_main,
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
        .main = dolphin_main,
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
        .main = ellipse_main,
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
        .main = erase_main,
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
        .main = exit_main,
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
        .main = get_prompt,
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
        .main = heart_main,
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
        .main = help_main,
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
        .main = icon128_generic,
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
        .main = icon16_generic,
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
        .main = icon24_generic,
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
        .main = icon32_generic,
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
        .main = icon48_generic,
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
        .main = icon64_generic,
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
        .main = line_main,
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
        .main = locatepoint_main,
        .icon = "about",
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
        .main = move_main,
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
        .main = moveselected_main,
        .icon = "donothing",
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
        .main = new_generic,
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
        .main = night_main,
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
        .main = open_main,
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
        .main = pandown_main,
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
        .main = panleft_main,
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
        .main = panright_main,
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
        .main = panup_main,
        .icon = "about",
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
        .main = path_main,
        .icon = "about",
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
        .main = platform_main,
        .icon = "about",
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
        .main = point_main,
        .icon = "about",
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
        .main = polygon_main,
        .icon = "about",
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
        .main = polyline_main,
        .icon = "about",
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
        .main = previewoff_main,
        .icon = "donothing",
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
        .main = previewon_main,
        .icon = "donothing",
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
        .main = print_main,
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
        .main = quickleader_main,
        .icon = "about",
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
        .main = rectangle_main,
        .icon = "about",
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
        .main = redo_main,
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
        .main = rgb_main,
        .icon = "about",
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
        .main = rotate_main,
        .icon = "about",
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
        .main = sandbox_main,
        .icon = "about",
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
        .main = save_main,
        .icon = "donothing",
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
        .main = scale_main,
        .icon = "about",
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
        .main = scaleselected_main,
        .icon = "donothing",
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
        .main = selectall_main,
        .icon = "about",
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
        .main = set_prompt,
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
        .main = singlelinetext_main,
        .icon = "about",
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
        .main = snowflake_main,
        .icon = "about",
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
        .main = star_main,
        .icon = "about",
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
        .main = syswindows_main,
        .icon = "about",
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
        .main = tipoftheday_main,
        .icon = "about",
        .menu_name = "None",
        .menu_position = 0,
        .toolbar_name = "None",
        .toolbar_position = 0,
        .tooltip = "Icon&24",
        .statustip = "Sets the toolbar icon size to 24x24:  ICON24",
        .alias = "ICON24"
    },
    {
        .main = todo_main,
        .icon = "about",
        .menu_name = "None",
        .menu_position = 0,
        .toolbar_name = "None",
        .toolbar_position = 0,
        .tooltip = "&About",
        .statustip = "Displays information about this product:  ABOUT",
        .alias = "ABOUT"
    },
    {
        .id = -1,
        .main = undo_main,
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
        .main = vulcanize_main,
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
        .id = ACTION_windowcascade,
        .main = windowcascade_main,
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
        .main = windowclose_main,
        .icon = "about",
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
        .main = windowcloseall_main,
        .icon = "about",
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
        .main = windownext_main,
        .icon = "about",
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
        .main = windowprevious_main,
        .icon = "about",
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
        .main = windowtile_main,
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
        .main = zoomextents_main,
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
        .main = zoomin_main,
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
        .main = zoomout_main,
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
        .menu_position = -1
    }
};
