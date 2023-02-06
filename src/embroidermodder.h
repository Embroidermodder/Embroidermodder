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
#include <chrono>
#include <unordered_map>

#include <embroidery.h>

#include <GL/glew.h>

/* DEFINES FOR JUMP TABLES/MODES
 */
#define CIRCLE_MODE_1P_RAD              0
#define CIRCLE_MODE_1P_DIA              1
#define CIRCLE_MODE_2P                  2
#define CIRCLE_MODE_3P                  3
#define CIRCLE_MODE_TTR                 4

#define ELLIPSE_MODE_MAJORDIAMETER_MINORRADIUS   0
#define ELLIPSE_MODE_MAJORRADIUS_MINORRADIUS     1
#define ELLIPSE_MODE_ELLIPSE_ROTATION            2

#define DOLPHIN_MODE_NUM_POINTS         0
#define DOLPHIN_MODE_XSCALE             1
#define DOLPHIN_MODE_YSCALE             2

#define HEART_MODE_NUM_POINTS           0
#define HEART_MODE_STYLE                1
#define HEART_MODE_XSCALE               2
#define HEART_MODE_YSCALE               3

#define SCALE_MODE_NORMAL               0
#define SCALE_MODE_REFERENCE            1

#define SINGLE_LINE_TEXT_MODE_JUSTIFY   0
#define SINGLE_LINE_TEXT_MODE_SETFONT   1
#define SINGLE_LINE_TEXT_MODE_SETGEOM   2
#define SINGLE_LINE_TEXT_MODE_RAPID     3

#define STAR_MODE_NUM_POINTS            0
#define STAR_MODE_CENTER_PT             1
#define STAR_MODE_RAD_OUTER             2
#define STAR_MODE_RAD_INNER             3

/* TYPEDEFS
 * -----------------------------------------------------------------------------
 */
typedef struct CircleUi_ {
    EmbVector point1, point2, point3, center;
    float radius;
    float diameter;
    int mode;
} CircleUi;

typedef struct EllipseUi_ {
    EmbVector point1, point2, point3, center;
    float width, height, rot;
    unsigned int mode;
} EllipseUi;

typedef struct SingleLineTextUi_ {
    std::string text;
    EmbVector position;
    std::string textJustify;
    std::string textFont;
    float textHeight;
    float textRotation;
    unsigned int mode;
} SingleLineTextUi;

typedef struct StarUi_ {
    unsigned int numPoints;
    EmbVector center, point1, point2;
    unsigned int mode;
} StarUi;

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
    float real_value;
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
    float scale;
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
    bool simulate;
    std::chrono::time_point<std::chrono::system_clock> simulation_start;
    std::string text_font;
    float text_size;
    float text_angle;
    bool text_style_bold;
    bool text_style_italic;
    bool text_style_underline;
    bool text_style_overline;
    bool text_style_strikeout;
    std::string filename;
    std::vector<std::string> undo_history;
    int undo_history_position;
} View;

typedef struct Settings_ {
    bool running;
    bool debug_mode;
    bool show_about_dialog;
    bool show_settings_editor;
    bool show_editor;
    int icon_size;
    int pattern_index;
    std::string assets_dir;
    bool use_translation;
    std::string language;
    bool mdi_bg_use_logo;
    bool mdi_bg_use_texture;
    bool mdi_bg_use_color;
    bool tip_of_the_day;
    std::string general_mdi_bg_logo;
    std::string general_mdi_bg_texture;
    unsigned int  general_mdi_bg_color;
    unsigned int general_current_tip;
    bool general_system_help_browser;
    bool general_check_for_updates;
    bool display_use_opengl;
    bool display_renderhint_aa;
    bool display_renderhint_text_aa;
    bool display_renderhint_smooth_pix;
    bool display_renderhint_high_aa;
    bool display_renderhint_noncosmetic;
    bool display_show_scrollbars;
    int display_scrollbar_widget_num;
    unsigned int  display_crosshair_color;
    unsigned int  display_bg_color;
    unsigned int  display_selectbox_left_color;
    unsigned int  display_selectbox_left_fill;
    unsigned int  display_selectbox_right_color;
    unsigned int  display_selectbox_right_fill;
    unsigned char  display_selectbox_alpha;
    float display_zoomscale_in;
    float display_zoomscale_out;
    unsigned char  display_crosshair_percent;
    std::string display_units;
    unsigned int  prompt_text_color;
    unsigned int  prompt_bg_color;
    std::string prompt_font_family;
    std::string prompt_font_style;
    unsigned char  prompt_font_size;
    bool prompt_save_history;
    bool prompt_save_history_as_html;
    std::string prompt_save_history_filename;
    std::string opensave_custom_filter;
    std::string opensave_open_format;
    bool opensave_open_thumbnail;
    std::string opensave_save_format;
    bool opensave_save_thumbnail;
    unsigned char  opensave_recent_max_files;
    std::vector<std::string> opensave_recent_list_of_files;
    std::string opensave_recent_directory;
    unsigned char  opensave_trim_dst_num_jumps;
    std::string printing_default_device;
    bool printing_use_last_device;
    bool printing_disable_bg;
    bool grid_show_on_load;
    bool grid_show_origin;
    bool grid_color_match_crosshair;
    unsigned int  grid_color;
    bool grid_load_from_file;
    std::string grid_type;
    bool grid_center_on_origin;
    float grid_center_x;
    float grid_center_y;
    float grid_size_x;
    float grid_size_y;
    float grid_spacing_x;
    float grid_spacing_y;
    float grid_size_radius;
    float grid_spacing_radius;
    float grid_spacing_angle;
    bool ruler_show_on_load;
    bool ruler_metric;
    unsigned int  ruler_color;
    unsigned char  ruler_pixel_size;
    bool qsnap_enabled;
    unsigned int  qsnap_locator_color;
    unsigned char  qsnap_locator_size;
    unsigned char  qsnap_aperture_size;
    bool qsnap_endpoint;
    bool qsnap_midpoint;
    bool qsnap_center;
    bool qsnap_node;
    bool qsnap_quadrant;
    bool qsnap_intersection;
    bool qsnap_extension;
    bool qsnap_insertion;
    bool qsnap_perpendicular;
    bool qsnap_tangent;
    bool qsnap_nearest;
    bool qsnap_apparent;
    bool qsnap_parallel;
    bool lwt_show_lwt;
    bool lwt_real_render;
    float lwt_default_lwt;
    bool selection_mode_pickfirst;
    bool selection_mode_pickadd;
    bool selection_mode_pickdrag;
    unsigned int  selection_coolgrip_color;
    unsigned int  selection_hotgrip_color;
    unsigned char  selection_grip_size;
    unsigned char  selection_pickbox_size;
} Settings;

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
void property_editor(void);

std::string platform_string(void);

View init_view(void);

void about_action(std::vector<std::string> args);
void alert_action(std::vector<std::string> args);
void arc_action(std::vector<std::string> args);
void circle_action(std::vector<std::string> args);
void close_action(std::vector<std::string> args);
void cut_action(std::vector<std::string> args);
void day_vision_action(std::vector<std::string> args);
void debug_action(std::vector<std::string> args);
void do_nothing_action(std::vector<std::string> args);
void editor_action(std::vector<std::string> args);
void error_action(std::vector<std::string> args);
void exit_action(std::vector<std::string> args);
void new_file_action(std::vector<std::string> args);
void night_vision_action(std::vector<std::string> args);
void open_file_action(std::vector<std::string> args);
void icon_action(std::vector<std::string> args);
void pan_action(std::vector<std::string> args);
void redo_action(std::vector<std::string> args);
void settings_editor_action(std::vector<std::string> args);
void simulate_action(std::vector<std::string> args);
void todo_action(std::vector<std::string> args);
void undo_action(std::vector<std::string> args);
void zoom_action(std::vector<std::string> args);

/* Global variables.
 * -----------------------------------------------------------------------------
 */
extern Settings settings;
extern string_matrix translation_table;
extern std::unordered_map<std::string, string_matrix> menu_layout;
extern std::string os_seperator;
extern std::vector<View> views;

#endif /* EMBROIDERMODDER_H */
