/*
 *  Embroidermodder 2.
 *
 *  ------------------------------------------------------------
 *
 *  Copyright 2013-2022 The Embroidermodder Team
 *  Embroidermodder 2 is Open Source Software.
 *  See LICENSE for licensing terms.
 *
 *  ------------------------------------------------------------
 *
 *  Use Python's PEP7 style guide.
 *      https://peps.python.org/pep-0007/
 */

#ifndef EMBROIDERMODDER_H
#define EMBROIDERMODDER_H

#define VERSION           "2.0.0-alpha"

#include <iostream>
#include <vector>
#include <unordered_map>

#include <embroidery.h>

#include <GL/glew.h>

/* TYPEDEFS
 * -----------------------------------------------------------------------------
 */

typedef struct Action_ {
    std::string command;
    std::string menu_label;
    std::string description;
} Action;

typedef struct Icon_ {
    std::string fname;
    std::string command;
    GLuint texture_id;
} Icon;

typedef struct Setting_ {
    bool bool_value;
    double real_value;
    int int_value;
    std::string string_value;
    int type;
} Setting;

typedef struct LineEdit_ {
    std::string label;
    std::string icon;
    std::string type;
    std::string signal;
    bool user_editable;
} LineEdit;

typedef struct GroupBox_ {
    std::string title;
    std::vector<LineEdit> line_edits;
    int obj_type;
} GroupBox;

typedef struct View_ {
    EmbPattern *pattern;
    EmbVector origin;
    double scale;
    std::string grid_type;
    bool snap_mode;
    bool grid_mode;
    bool ruler_mode;
    bool ortho_mode;
    bool polar_mode;
    bool qsnap_mode;
    bool qtrack_mode;
    bool lwt_mode;
    bool metric;
    std::string text_font;
    double text_size;
    double text_angle;
    bool text_style_bold;
    bool text_style_italic;
    bool text_style_underline;
    bool text_style_overline;
    bool text_style_strikeout;
    std::string filename;
    std::vector<std::string> undo_history;
    int undo_history_position;
} View;

typedef std::vector<std::vector<std::string>> string_matrix;

/* Function Prototypes.
 * -----------------------------------------------------------------------------
 */
void set_style(void);
void load_configuration(void);
void actuator(std::string command);
void load_menu(std::string menu_label, std::vector<std::vector<std::string>> menu_layout);
void load_toolbar(std::string menu_label, std::vector<std::vector<std::string>> toolbar_layout);

void settings_editor(void);
void about_dialog(void);
void changelog_dialog(void);
void help_dialog(void);
void alert(void);

std::string translate(std::string string);
void debug_message(std::string string);

void pattern_view(void);
void status_bar(void);

std::string load_string(std::string s);
std::string store_string(std::string s);

View init_view(void);

void about_action(std::vector<std::string> args);
void alert_action(std::vector<std::string> args);
void arc_action(std::vector<std::string> args);
void circle_action(std::vector<std::string> args);
void day_vision_action(std::vector<std::string> args);
void debug_action(std::vector<std::string> args);
void error_action(std::vector<std::string> args);
void open_file_action(std::vector<std::string> args);
void todo_action(std::vector<std::string> args);
void exit_action(std::vector<std::string> args);
void icon_action(std::vector<std::string> args);
void new_file_action(std::vector<std::string> args);
void night_vision_action(std::vector<std::string> args);
void pan_action(std::vector<std::string> args);
void zoom_action(std::vector<std::string> args);
void settings_editor_action(std::vector<std::string> args);

/* Global variables.
 * -----------------------------------------------------------------------------
 */

extern std::unordered_map<std::string, std::string> str_settings;

extern bool running;
extern bool debug_mode;
extern bool show_about_dialog;
extern bool show_settings_editor;
extern bool show_editor;
extern int icon_size;
extern std::string language;
extern int pattern_index;
extern std::string current_fname;
extern std::string assets_dir;
//extern std::vector<Action> action_list;
extern string_matrix translation_table;
extern std::unordered_map<std::string, string_matrix> menu_layout;
extern std::string os_seperator;
extern std::string settings_general_language;
extern std::string settings_general_icon_theme;
extern int settings_general_icon_size;
extern bool settings_general_mdi_bg_use_logo;
extern bool settings_general_mdi_bg_use_texture;
extern bool settings_general_mdi_bg_use_color;
extern std::string settings_general_mdi_bg_logo;
extern std::string settings_general_mdi_bg_texture;
extern unsigned int settings_general_mdi_bg_color;
extern bool settings_general_tip_of_the_day;
extern unsigned int settings_general_current_tip;
extern bool settings_general_system_help_browser;
extern bool settings_general_check_for_updates;
extern bool settings_display_use_opengl;
extern bool settings_display_renderhint_aa;
extern bool settings_display_renderhint_text_aa;
extern bool settings_display_renderhint_smooth_pix;
extern bool settings_display_renderhint_high_aa;
extern bool settings_display_renderhint_noncosmetic;
extern bool settings_display_show_scrollbars;
extern int settings_display_scrollbar_widget_num;
extern unsigned int settings_display_crosshair_color;
extern unsigned int settings_display_bg_color;
extern unsigned int settings_display_selectbox_left_color;
extern unsigned int settings_display_selectbox_left_fill;
extern unsigned int settings_display_selectbox_right_color;
extern unsigned int settings_display_selectbox_right_fill;
extern unsigned char settings_display_selectbox_alpha;
extern double settings_display_zoomscale_in;
extern double settings_display_zoomscale_out;
extern unsigned char settings_display_crosshair_percent;
extern std::string settings_display_units;
extern unsigned int settings_prompt_text_color;
extern unsigned int settings_prompt_bg_color;
extern std::string settings_prompt_font_family;
extern std::string settings_prompt_font_style;
extern unsigned char settings_prompt_font_size;
extern bool settings_prompt_save_history;
extern bool settings_prompt_save_history_as_html;
extern std::string settings_prompt_save_history_filename;
extern std::string settings_opensave_custom_filter;
extern std::string settings_opensave_open_format;
extern bool settings_opensave_open_thumbnail;
extern std::string settings_opensave_save_format;
extern bool settings_opensave_save_thumbnail;
extern unsigned char settings_opensave_recent_max_files;
extern std::vector<std::string> settings_opensave_recent_list_of_files;
extern std::string settings_opensave_recent_directory;
extern unsigned char settings_opensave_trim_dst_num_jumps;
extern std::string settings_printing_default_device;
extern bool settings_printing_use_last_device;
extern bool settings_printing_disable_bg;
extern bool settings_grid_show_on_load;
extern bool settings_grid_show_origin;
extern bool settings_grid_color_match_crosshair;
extern unsigned int settings_grid_color;
extern bool settings_grid_load_from_file;
extern std::string settings_grid_type;
extern bool settings_grid_center_on_origin;
extern double settings_grid_center_x;
extern double settings_grid_center_y;
extern double settings_grid_size_x;
extern double settings_grid_size_y;
extern double settings_grid_spacing_x;
extern double settings_grid_spacing_y;
extern double settings_grid_size_radius;
extern double settings_grid_spacing_radius;
extern double settings_grid_spacing_angle;
extern bool settings_ruler_show_on_load;
extern bool settings_ruler_metric;
extern unsigned int settings_ruler_color;
extern unsigned char settings_ruler_pixel_size;
extern bool settings_qsnap_enabled;
extern unsigned int settings_qsnap_locator_color;
extern unsigned char settings_qsnap_locator_size;
extern unsigned char settings_qsnap_aperture_size;
extern bool settings_qsnap_endpoint;
extern bool settings_qsnap_midpoint;
extern bool settings_qsnap_center;
extern bool settings_qsnap_node;
extern bool settings_qsnap_quadrant;
extern bool settings_qsnap_intersection;
extern bool settings_qsnap_extension;
extern bool settings_qsnap_insertion;
extern bool settings_qsnap_perpendicular;
extern bool settings_qsnap_tangent;
extern bool settings_qsnap_nearest;
extern bool settings_qsnap_apparent;
extern bool settings_qsnap_parallel;
extern bool settings_lwt_show_lwt;
extern bool settings_lwt_real_render;
extern double settings_lwt_default_lwt;
extern bool settings_selection_mode_pickfirst;
extern bool settings_selection_mode_pickadd;
extern bool settings_selection_mode_pickdrag;
extern unsigned int settings_selection_coolgrip_color;
extern unsigned int settings_selection_hotgrip_color;
extern unsigned char settings_selection_grip_size;
extern unsigned char settings_selection_pickbox_size;
extern std::vector<View> views;

#endif /* EMBROIDERMODDER_H */
