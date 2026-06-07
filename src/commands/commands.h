/*
 * Embroidermodder 2 -- Commands
 * Copyright 2011-2026 The Embroidermodder Team
 */

#ifndef __EMBROIDERMODDER_COMMANDS__
#define __EMBROIDERMODDER_COMMANDS__

#ifdef __cplusplus
extern "C" {
#endif

#include "core.h"
#include "gui.h"

/* COMMANDS */
int do_nothing_cmd(State *state);

int test_cmd(State *state);

int new_cmd(State *state);
int open_cmd(State *state);
int save_cmd(State *state);
int save_as_cmd(State *state);
int print_cmd(State *state);
int design_details_cmd(State *state);
int exit_cmd(State *state);
int cut_cmd(State *state);
int copy_cmd(State *state);
int paste_cmd(State *state);

int undo_cmd(State *state);
int redo_cmd(State *state);

// Window Menu
int window_close_cmd(State *state);
int window_close_all_cmd(State *state);
int window_cascade_cmd(State *state);
int window_tile_cmd(State *state);
int window_next_cmd(State *state);
int window_previous_cmd(State *state);

// Help Menu
int help_cmd(State *state);
int changelog_cmd(State *state);
int tip_of_the_day_cmd(State *state);
int about_cmd(State *state);
int whats_this_cmd(State *state);

// Icons
int icon16_cmd(State *state);
int icon24_cmd(State *state);
int icon32_cmd(State *state);
int icon48_cmd(State *state);
int icon64_cmd(State *state);
int icon128_cmd(State *state);

int settingsdialog_cmd(State *state);

// Layer ToolBar
int makelayercurrent_cmd(State *state);
int layers_cmd(State *state);
int layerselector_cmd(State *state);
int layerprevious_cmd(State *state);
int colorselector_cmd(State *state);
int linetypeselector_cmd(State *state);
int lineweightselector_cmd(State *state);
int hidealllayers_cmd(State *state);
int showalllayers_cmd(State *state);
int freezealllayers_cmd(State *state);
int thawalllayers_cmd(State *state);
int lockalllayers_cmd(State *state);
int unlockalllayers_cmd(State *state);

//Text ToolBar
int text_bold_cmd(State *state);
int text_italic_cmd(State *state);
int text_underline_cmd(State *state);
int text_strikeout_cmd(State *state);
int text_overline_cmd(State *state);

// Zoom ToolBar
int zoom_real_time_cmd(State *state);
int zoom_previous_cmd(State *state);
int zoom_window_cmd(State *state);
int zoom_dynamic_cmd(State *state);
int zoom_scale_cmd(State *state);
int zoom_center_cmd(State *state);
int zoom_in_cmd(State *state);
int zoom_out_cmd(State *state);
int zoom_selected_cmd(State *state);
int zoom_all_cmd(State *state);
int zoom_extents_cmd(State *state);

// Pan SubMenu
int pan_real_time_cmd(State *state);
int pan_point_cmd(State *state);
int pan_left_cmd(State *state);
int pan_right_cmd(State *state);
int pan_up_cmd(State *state);
int pan_down_cmd(State *state);

int day_cmd(State *state);
int night_cmd(State *state);

/* Draw */
int circle_cmd(State *state);
int distance_cmd(State *state);
int dolphin_cmd(State *state);
int ellipse_cmd(State *state);
int heart_cmd(State *state);
int line_cmd(State *state);
int path_cmd(State *state);
int point_cmd(State *state);
int polygon_cmd(State *state);
int polyline_cmd(State *state);
int rectangle_cmd(State *state);
int snowflake_cmd(State *state);
int star_cmd(State *state);

int quickleader_cmd(State *state);

int erase_cmd(State *state);
int locate_point_cmd(State *state);
int move_cmd(State *state);
int platform_cmd(State *state);
int rgb_cmd(State *state);
int rotate_cmd(State *state);
int scale_cmd(State *state);
int select_all_cmd(State *state);
int single_line_text_cmd(State *state);
int syswindows_cmd(State *state);
int tip_of_the_day_cmd(State *state);

#ifdef __cplusplus
}
#endif

#endif

