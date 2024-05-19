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
ScriptValue clear_generic(ScriptEnv*);
ScriptValue day_generic(ScriptEnv *);
ScriptValue icon128_generic(ScriptEnv*);
ScriptValue icon16_generic(ScriptEnv*);
ScriptValue icon24_generic(ScriptEnv*);
ScriptValue icon32_generic(ScriptEnv*);
ScriptValue icon48_generic(ScriptEnv*);
ScriptValue icon64_generic(ScriptEnv*);
ScriptValue get_prompt(ScriptEnv*);
ScriptValue new_generic(ScriptEnv*);

/* Context-sensitive commands: mostly geometric. */
ScriptValue circle_main(ScriptEnv*);
ScriptValue circle_click(ScriptEnv*);
ScriptValue circle_context(ScriptEnv*);
ScriptValue circle_prompt(ScriptEnv*);

ScriptValue distance_main(ScriptEnv*);
ScriptValue distance_click(ScriptEnv*);
ScriptValue distance_context(ScriptEnv*);
ScriptValue distance_prompt(ScriptEnv*);

ScriptValue dolphin_main(ScriptEnv*);
ScriptValue dolphin_click(ScriptEnv*);
ScriptValue dolphin_context(ScriptEnv*);
ScriptValue dolphin_prompt(ScriptEnv*);

ScriptValue ellipse_click(ScriptEnv *);
ScriptValue ellipse_context(ScriptEnv *);
ScriptValue ellipse_main(ScriptEnv *);
ScriptValue ellipse_prompt(ScriptEnv *);

ScriptValue erase_generic(ScriptEnv *);
ScriptValue erase_main(ScriptEnv *);

ScriptValue exit_click(ScriptEnv *);
ScriptValue exit_context(ScriptEnv *);
ScriptValue exit_main(ScriptEnv *);
ScriptValue exit_prompt(ScriptEnv *);

ScriptValue heart_generic(ScriptEnv *);
ScriptValue heart_main(ScriptEnv *);

ScriptValue help_generic(ScriptEnv*);
ScriptValue help_main(ScriptEnv*);

ScriptValue heart_click(ScriptEnv*);
ScriptValue heart_context(ScriptEnv*);
ScriptValue heart_main(ScriptEnv*);
ScriptValue heart_prompt(ScriptEnv*);

ScriptValue line_click(ScriptEnv*);
ScriptValue line_context(ScriptEnv*);
ScriptValue line_main(ScriptEnv*);
ScriptValue line_prompt(ScriptEnv*);

ScriptValue locatepoint_click(ScriptEnv*);
ScriptValue locatepoint_context(ScriptEnv*);
ScriptValue locatepoint_main(ScriptEnv*);
ScriptValue locatepoint_prompt(ScriptEnv*);

ScriptValue move_main(ScriptEnv*);
ScriptValue move_click(ScriptEnv*);
ScriptValue move_context(ScriptEnv*);
ScriptValue move_prompt(ScriptEnv*);

ScriptValue moveselected_click(ScriptEnv*);
ScriptValue moveselected_main(ScriptEnv*);
ScriptValue moveselected_context(ScriptEnv*);
ScriptValue moveselected_prompt(ScriptEnv*);

ScriptValue night_click(ScriptEnv*);
ScriptValue night_context(ScriptEnv*);
ScriptValue night_main(ScriptEnv*);
ScriptValue night_prompt(ScriptEnv*);

ScriptValue open_click(ScriptEnv*);
ScriptValue open_context(ScriptEnv*);
ScriptValue open_main(ScriptEnv*);
ScriptValue open_prompt(ScriptEnv*);

ScriptValue print_click(ScriptEnv*);
ScriptValue print_context(ScriptEnv*);
ScriptValue print_main(ScriptEnv*);
ScriptValue print_prompt(ScriptEnv*);

ScriptValue redo_click(ScriptEnv*);
ScriptValue redo_context(ScriptEnv*);
ScriptValue redo_main(ScriptEnv*);
ScriptValue redo_prompt(ScriptEnv*);

ScriptValue pandown_generic(ScriptEnv*);
ScriptValue pandown_context(ScriptEnv*);
ScriptValue pandown_main(ScriptEnv*);
ScriptValue pandown_prompt(ScriptEnv*);

ScriptValue panleft_click(ScriptEnv*);
ScriptValue panleft_context(ScriptEnv*);
ScriptValue panleft_main(ScriptEnv*);
ScriptValue panleft_prompt(ScriptEnv*);

ScriptValue panright_click(ScriptEnv*);
ScriptValue panright_context(ScriptEnv*);
ScriptValue panright_main(ScriptEnv*);
ScriptValue panright_prompt(ScriptEnv*);

ScriptValue panup_click(ScriptEnv*);
ScriptValue panup_context(ScriptEnv*);
ScriptValue panup_main(ScriptEnv*);
ScriptValue panup_prompt(ScriptEnv*);

ScriptValue path_click(ScriptEnv*);
ScriptValue path_context(ScriptEnv*);
ScriptValue path_main(ScriptEnv*);
ScriptValue path_prompt(ScriptEnv*);

ScriptValue platform_click(ScriptEnv*);
ScriptValue platform_context(ScriptEnv*);
ScriptValue platform_main(ScriptEnv*);
ScriptValue platform_prompt(ScriptEnv*);

ScriptValue previewoff_prompt(ScriptEnv*);
ScriptValue previewoff_click(ScriptEnv*);
ScriptValue previewoff_context(ScriptEnv*);
ScriptValue previewoff_main(ScriptEnv*);

ScriptValue previewon_prompt(ScriptEnv*);
ScriptValue previewon_click(ScriptEnv*);
ScriptValue previewon_context(ScriptEnv*);
ScriptValue previewon_main(ScriptEnv*);

ScriptValue point_click(ScriptEnv*);
ScriptValue point_context(ScriptEnv*);
ScriptValue point_main(ScriptEnv*);
ScriptValue point_prompt(ScriptEnv*);

ScriptValue polygon_click(ScriptEnv*);
ScriptValue polygon_context(ScriptEnv*);
ScriptValue polygon_main(ScriptEnv*);
ScriptValue polygon_prompt(ScriptEnv*);

ScriptValue polyline_click(ScriptEnv*);
ScriptValue polyline_context(ScriptEnv*);
ScriptValue polyline_main(ScriptEnv*);
ScriptValue polyline_prompt(ScriptEnv*);

ScriptValue rectangle_click(ScriptEnv*);
ScriptValue rectangle_context(ScriptEnv*);
ScriptValue rectangle_main(ScriptEnv*);
ScriptValue rectangle_prompt(ScriptEnv*);

ScriptValue rotate_click(ScriptEnv*);
ScriptValue rotate_context(ScriptEnv*);
ScriptValue rotate_main(ScriptEnv*);
ScriptValue rotate_prompt(ScriptEnv*);

ScriptValue rgb_click(ScriptEnv*);
ScriptValue rgb_context(ScriptEnv*);
ScriptValue rgb_main(ScriptEnv*);
ScriptValue rgb_prompt(ScriptEnv*);

ScriptValue quickleader_click(ScriptEnv*);
ScriptValue quickleader_context(ScriptEnv*);
ScriptValue quickleader_main(ScriptEnv*);
ScriptValue quickleader_prompt(ScriptEnv*);

ScriptValue save_click(ScriptEnv*);
ScriptValue save_context(ScriptEnv*);
ScriptValue save_main(ScriptEnv*);
ScriptValue save_prompt(ScriptEnv*);

ScriptValue sandbox_click(ScriptEnv*);
ScriptValue sandbox_context(ScriptEnv*);
ScriptValue sandbox_main(ScriptEnv*);
ScriptValue sandbox_prompt(ScriptEnv*);

ScriptValue scale_click(ScriptEnv*);
ScriptValue scale_context(ScriptEnv*);
ScriptValue scale_main(ScriptEnv*);
ScriptValue scale_prompt(ScriptEnv*);

ScriptValue scaleselected_click(ScriptEnv*);
ScriptValue scaleselected_context(ScriptEnv*);
ScriptValue scaleselected_main(ScriptEnv*);
ScriptValue scaleselected_prompt(ScriptEnv*);

ScriptValue selectall_click(ScriptEnv*);
ScriptValue selectall_context(ScriptEnv*);
ScriptValue selectall_main(ScriptEnv*);
ScriptValue selectall_prompt(ScriptEnv*);

ScriptValue singlelinetext_click(ScriptEnv*);
ScriptValue singlelinetext_context(ScriptEnv*);
ScriptValue singlelinetext_main(ScriptEnv*);
ScriptValue singlelinetext_prompt(ScriptEnv*);

ScriptValue set_prompt(ScriptEnv*);

ScriptValue snowflake_click(ScriptEnv*);
ScriptValue snowflake_context(ScriptEnv*);
ScriptValue snowflake_main(ScriptEnv*);
ScriptValue snowflake_prompt(ScriptEnv*);

ScriptValue star_click(ScriptEnv*);
ScriptValue star_context(ScriptEnv*);
ScriptValue star_main(ScriptEnv*);
ScriptValue star_prompt(ScriptEnv*);

ScriptValue syswindows_click(ScriptEnv*);
ScriptValue syswindows_context(ScriptEnv*);
ScriptValue syswindows_main(ScriptEnv*);
ScriptValue syswindows_prompt(ScriptEnv*);

ScriptValue tipoftheday_click(ScriptEnv*);
ScriptValue tipoftheday_context(ScriptEnv*);
ScriptValue tipoftheday_main(ScriptEnv*);
ScriptValue tipoftheday_prompt(ScriptEnv*);

ScriptValue todo_click(ScriptEnv*);
ScriptValue todo_context(ScriptEnv*);
ScriptValue todo_main(ScriptEnv*);
ScriptValue todo_prompt(ScriptEnv*);

ScriptValue undo_click(ScriptEnv*);
ScriptValue undo_context(ScriptEnv*);
ScriptValue undo_main(ScriptEnv*);
ScriptValue undo_prompt(ScriptEnv*);

ScriptValue vulcanize_click(ScriptEnv*);
ScriptValue vulcanize_main(ScriptEnv*);
ScriptValue vulcanize_context(ScriptEnv*);
ScriptValue vulcanize_prompt(ScriptEnv*);

ScriptValue windowcascade_click(ScriptEnv*);
ScriptValue windowcascade_context(ScriptEnv*);
ScriptValue windowcascade_main(ScriptEnv*);
ScriptValue windowcascade_prompt(ScriptEnv*);

ScriptValue windowclose_click(ScriptEnv*);
ScriptValue windowclose_context(ScriptEnv*);
ScriptValue windowclose_main(ScriptEnv*);
ScriptValue windowclose_prompt(ScriptEnv*);

ScriptValue windowcloseall_click(ScriptEnv*);
ScriptValue windowcloseall_context(ScriptEnv*);
ScriptValue windowcloseall_main(ScriptEnv*);
ScriptValue windowcloseall_prompt(ScriptEnv*);

ScriptValue windownext_click(ScriptEnv*);
ScriptValue windownext_context(ScriptEnv*);
ScriptValue windownext_main(ScriptEnv*);
ScriptValue windownext_prompt(ScriptEnv*);

ScriptValue windowprevious_click(ScriptEnv*);
ScriptValue windowprevious_context(ScriptEnv*);
ScriptValue windowprevious_main(ScriptEnv*);
ScriptValue windowprevious_prompt(ScriptEnv*);

ScriptValue windowtile_generic(ScriptEnv*);
ScriptValue windowtile_main(ScriptEnv*);

ScriptValue zoomextents_main(ScriptEnv*);
ScriptValue zoomextents_generic(ScriptEnv*);

ScriptValue zoomin_main(ScriptEnv*);
ScriptValue zoomin_generic(ScriptEnv*);

ScriptValue zoomout_main(ScriptEnv*);
ScriptValue zoomout_generic(ScriptEnv*);

Command command_data[] = {
    {
        .id = ACTION_about,
        .main = about_general,
        .click = about_general,
        .context = about_general,
        .prompt = about_general,
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
        .main = do_nothing,
        .click = do_nothing,
        .context = do_nothing,
        .prompt = alert_generic,
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
        .click = circle_click,
        .context = circle_context,
        .prompt = circle_prompt,
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
        .click = clear_generic,
        .context = clear_generic,
        .prompt = clear_generic,
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
        .main = day_generic,
        .click = day_generic,
        .context = day_generic,
        .prompt = day_generic,
        .icon = "day",
        .menu_name = "None",
        .menu_position = 0,
        .toolbar_name = "None",
        .toolbar_position = 0,
        .tooltip = "&Day",
        .statustip = "Updates the current view using day vision settings:  DAY",
        .alias = "DAY"
    },
    {
        .id = -1,
        .main = distance_main,
        .click = distance_click,
        .context = distance_context,
        .prompt = distance_prompt,
        .icon = "distance",
        .menu_name = "Tools",
        .menu_position = 7,
        .toolbar_name = "Inquiry",
        .toolbar_position = 0,
        .tooltip = "&Distance",
        .statustip = "Measures the distance and angle between two points:  DIST",
        .alias = "DI, DIST, DISTANCE"
    },
    {
        .id = -1,
        .main = dolphin_main,
        .click = dolphin_click,
        .context = dolphin_context,
        .prompt = dolphin_prompt,
        .icon = "dolphin",
        .menu_name = "Draw",
        .menu_position = 100,
        .toolbar_name = "Draw",
        .toolbar_position = 100,
        .tooltip = "&Dolphin",
        .statustip = "Creates a dolphin:  DOLPHIN",
        .alias = "DOLPHIN"
    },
    {
        .id = ACTION_donothing,
        .main = do_nothing,
        .click = do_nothing,
        .context = do_nothing,
        .prompt = do_nothing,
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
        .main = ellipse_main,
        .click = ellipse_click,
        .context = ellipse_context,
        .prompt = ellipse_prompt,
        .icon = "ellipse",
        .menu_name = "Draw",
        .menu_position = 8,
        .toolbar_name = "Draw",
        .toolbar_position = 8,
        .tooltip = "&Ellipse",
        .statustip = "Creates an ellipse:  ELLIPSE",
        .alias = "EL, ELLIPSE"
    },
    {
        .id = -1,
        .main = erase_main,
        .click = erase_generic,
        .context = erase_generic,
        .prompt = erase_generic,
        .icon = "erase",
        .menu_name = "Modify",
        .menu_position = 5,
        .toolbar_name = "Modify",
        .toolbar_position = 0,
        .tooltip = "D&elete",
        .statustip = "Removes objects from a drawing:  DELETE",
        .alias = "E, ERASE, DEL, DELETE"
    },
    {
        .id = -1,
        .main = exit_main,
        .click = exit_click,
        .context = exit_context,
        .prompt = exit_prompt,
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
        .main = do_nothing,
        .click = do_nothing,
        .context = do_nothing,
        .prompt = get_prompt,
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
        .click = heart_click,
        .context = heart_context,
        .prompt = heart_prompt,
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
        .click = help_generic,
        .context = help_generic,
        .prompt = help_generic,
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
        .click = icon128_generic,
        .context = icon128_generic,
        .prompt = icon128_generic,
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
        .click = icon16_generic,
        .context = icon16_generic,
        .prompt = icon16_generic,
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
        .click = icon24_generic,
        .context = icon24_generic,
        .prompt = icon24_generic,
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
        .click = icon32_generic,
        .context = icon32_generic,
        .prompt = icon32_generic,
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
        .click = icon48_generic,
        .context = icon48_generic,
        .prompt = icon48_generic,
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
        .click = icon64_generic,
        .context = icon64_generic,
        .prompt = icon64_generic,
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
        .click = line_click,
        .context = line_context,
        .prompt = line_prompt,
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
        .click = locatepoint_click,
        .context = locatepoint_context,
        .prompt = locatepoint_prompt,
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
        .click = move_click,
        .context = move_context,
        .prompt = move_prompt,
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
        .click = moveselected_click,
        .context = moveselected_context,
        .prompt = moveselected_prompt,
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
        .click = new_generic,
        .context = new_generic,
        .prompt = new_generic,
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
        .click = night_click,
        .context = night_context,
        .prompt = night_prompt,
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
        .click = open_click,
        .context = open_context,
        .prompt = open_prompt,
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
        .click = pandown_generic,
        .context = pandown_generic,
        .prompt = pandown_generic,
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
        .click = panleft_click,
        .context = panleft_context,
        .prompt = panleft_prompt,
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
        .click = panright_click,
        .context = panright_context,
        .prompt = panright_prompt,
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
        .click = panup_click,
        .context = panup_context,
        .prompt = panup_prompt,
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
        .click = path_click,
        .context = path_context,
        .prompt = path_prompt,
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
        .click = platform_click,
        .context = platform_context,
        .prompt = platform_prompt,
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
        .click = point_click,
        .context = point_context,
        .prompt = point_prompt,
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
        .click = polygon_click,
        .context = polygon_context,
        .prompt = polygon_prompt,
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
        .click = polyline_click,
        .context = polyline_context,
        .prompt = polyline_prompt,
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
        .click = previewoff_click,
        .context = previewoff_context,
        .prompt = previewoff_prompt,
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
        .click = previewon_click,
        .context = previewon_context,
        .prompt = previewon_prompt,
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
        .main = print_main,
        .click = print_click,
        .context = print_context,
        .prompt = print_prompt,
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
        .click = quickleader_click,
        .context = quickleader_context,
        .prompt = quickleader_prompt,
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
        .click = rectangle_click,
        .context = rectangle_context,
        .prompt = rectangle_prompt,
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
        .click = redo_click,
        .context = redo_context,
        .prompt = redo_prompt,
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
        .click = rgb_click,
        .context = rgb_context,
        .prompt = rgb_prompt,
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
        .click = rotate_click,
        .context = rotate_context,
        .prompt = rotate_prompt,
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
        .click = sandbox_click,
        .context = sandbox_context,
        .prompt = sandbox_prompt,
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
        .click = save_click,
        .context = save_context,
        .prompt = save_prompt,
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
        .click = scale_click,
        .context = scale_context,
        .prompt = scale_prompt,
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
        .click = scaleselected_click,
        .context = scaleselected_context,
        .prompt = scaleselected_prompt,
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
        .click = selectall_click,
        .context = selectall_context,
        .prompt = selectall_prompt,
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
        .main = do_nothing,
        .click = do_nothing,
        .context = do_nothing,
        .prompt = set_prompt,
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
        .click = singlelinetext_click,
        .context = singlelinetext_context,
        .prompt = singlelinetext_prompt,
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
        .click = snowflake_click,
        .context = snowflake_context,
        .prompt = snowflake_prompt,
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
        .click = star_click,
        .context = star_context,
        .prompt = star_prompt,
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
        .click = syswindows_click,
        .context = syswindows_context,
        .prompt = syswindows_prompt,
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
        .click = tipoftheday_click,
        .context = tipoftheday_context,
        .prompt = tipoftheday_prompt,
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
        .click = todo_click,
        .context = todo_context,
        .prompt = todo_prompt,
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
        .click = undo_click,
        .context = undo_context,
        .prompt = undo_prompt,
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
        .click = vulcanize_click,
        .context = vulcanize_context,
        .prompt = vulcanize_prompt,
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
        .click = windowcascade_click,
        .context = windowcascade_context,
        .prompt = windowcascade_prompt,
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
        .click = windowclose_click,
        .context = windowclose_context,
        .prompt = windowclose_prompt,
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
        .click = windowcloseall_click,
        .context = windowcloseall_context,
        .prompt = windowcloseall_prompt,
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
        .click = windownext_click,
        .context = windownext_context,
        .prompt = windownext_prompt,
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
        .click = windowprevious_click,
        .context = windowprevious_context,
        .prompt = windowprevious_prompt,
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
        .click = windowtile_generic,
        .context = windowtile_generic,
        .prompt = windowtile_generic,
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
        .click = zoomextents_generic,
        .context = zoomextents_generic,
        .prompt = zoomextents_generic,
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
        .click = zoomin_generic,
        .context = zoomin_generic,
        .prompt = zoomin_generic,
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
        .click = zoomout_generic,
        .context = zoomout_generic,
        .prompt = zoomout_generic,
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
