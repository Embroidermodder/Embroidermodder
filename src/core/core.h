/*
 * Embroidermodder 2 -- C Core
 * Copyright 2011-2026 The Embroidermodder Team
 */

#ifndef EMBROIDERMODDER_CORE_H
#define EMBROIDERMODDER_CORE_H

#ifdef __cplusplus
extern "C" {
#endif

#include <stdbool.h>
#include <inttypes.h>

#include "sds.h"
#include "toml.h"

#define SDSARRAY_CHUNK                100

enum COMMAND_ACTIONS
{
    ACTION_donothing,

    ACTION_new,
    ACTION_open,
    ACTION_save,
    ACTION_saveas,
    ACTION_print,
    ACTION_designdetails,
    ACTION_exit,
    ACTION_cut,
    ACTION_copy,
    ACTION_paste,

    ACTION_undo,
    ACTION_redo,
    // Window Menu
    ACTION_windowclose,
    ACTION_windowcloseall,
    ACTION_windowcascade,
    ACTION_windowtile,
    ACTION_windownext,
    ACTION_windowprevious,
    // Help Menu
    ACTION_help,
    ACTION_changelog,
    ACTION_tipoftheday,
    ACTION_about,
    ACTION_whatsthis,
    // Icons
    ACTION_icon16,
    ACTION_icon24,
    ACTION_icon32,
    ACTION_icon48,
    ACTION_icon64,
    ACTION_icon128,

    ACTION_settingsdialog,

    // Layer ToolBar
    ACTION_makelayercurrent,
    ACTION_layers,
    ACTION_layerselector,
    ACTION_layerprevious,
    ACTION_colorselector,
    ACTION_linetypeselector,
    ACTION_lineweightselector,
    ACTION_hidealllayers,
    ACTION_showalllayers,
    ACTION_freezealllayers,
    ACTION_thawalllayers,
    ACTION_lockalllayers,
    ACTION_unlockalllayers,
    //Text ToolBar
    ACTION_textbold,
    ACTION_textitalic,
    ACTION_textunderline,
    ACTION_textstrikeout,
    ACTION_textoverline,
    // Zoom ToolBar
    ACTION_zoomrealtime,
    ACTION_zoomprevious,
    ACTION_zoomwindow,
    ACTION_zoomdynamic,
    ACTION_zoomscale,
    ACTION_zoomcenter,
    ACTION_zoomin,
    ACTION_zoomout,
    ACTION_zoomselected,
    ACTION_zoomall,
    ACTION_zoomextents,
    // Pan SubMenu
    ACTION_panrealtime,
    ACTION_panpoint,
    ACTION_panleft,
    ACTION_panright,
    ACTION_panup,
    ACTION_pandown,

    ACTION_day,
    ACTION_night,

    //TODO: ACTION_spellcheck,
    //TODO: ACTION_quickselect,

    ACTION_scripted,

    ACTION_null
};

enum COMMAND_TYPE
{
    CMD_TYPE_TOGGLE,
    CMD_TYPE_TRIGGER
};

typedef struct SDSArray_ {
    sds *data;
    int count;
    int memory;
} sdsarray;

typedef struct Settings_ {
    sds general_language;
    sds general_icon_theme;
    int general_icon_size;
    bool general_mdi_bg_use_logo;
    bool general_mdi_bg_use_texture;
    bool general_mdi_bg_use_color;
    sds general_mdi_bg_logo;
    sds general_mdi_bg_texture;
    uint32_t general_mdi_bg_color;
    bool general_tip_of_the_day;
    uint16_t general_current_tip;
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
    uint32_t display_crosshair_color;
    uint32_t display_bg_color;
    uint32_t display_selectbox_left_color;
    uint32_t display_selectbox_left_fill;
    uint32_t display_selectbox_right_color;
    uint32_t display_selectbox_right_fill;
    uint8_t display_selectbox_alpha;
    double display_zoomscale_in;
    double display_zoomscale_out;
    uint8_t display_crosshair_percent;
    sds display_units;
    uint32_t prompt_text_color;
    uint32_t prompt_bg_color;
    sds prompt_font_family;
    sds prompt_font_style;
    uint8_t prompt_font_size;
    bool prompt_save_history;
    bool prompt_save_history_as_html;
    sds prompt_save_history_filename;
    sds opensave_custom_filter;
    sds opensave_open_format;
    bool opensave_open_thumbnail;
    sds opensave_save_format;
    bool opensave_save_thumbnail;
    uint8_t opensave_recent_max_files;
    sdsarray *opensave_recent_list_of_files;
    sds opensave_recent_directory;
    uint8_t opensave_trim_dst_num_jumps;
    sds printing_default_device;
    bool printing_use_last_device;
    bool printing_disable_bg;
    bool grid_show_on_load;
    bool grid_show_origin;
    bool grid_color_match_crosshair;
    uint32_t grid_color;
    bool grid_load_from_file;
    sds grid_type;
    bool grid_center_on_origin;
    double grid_center_x;
    double grid_center_y;
    double grid_size_x;
    double grid_size_y;
    double grid_spacing_x;
    double grid_spacing_y;
    double grid_size_radius;
    double grid_spacing_radius;
    double grid_spacing_angle;
    bool ruler_show_on_load;
    bool ruler_metric;
    uint32_t ruler_color;
    uint8_t ruler_pixel_size;
    bool qsnap_enabled;
    uint32_t qsnap_locator_color;
    uint8_t qsnap_locator_size;
    uint8_t qsnap_aperture_size;
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
    double lwt_default_lwt;
    bool selection_mode_pickfirst;
    bool selection_mode_pickadd;
    bool selection_mode_pickdrag;
    uint32_t selection_coolgrip_color;
    uint32_t selection_hotgrip_color;
    uint8_t selection_grip_size;
    uint8_t selection_pickbox_size;
    sds text_font;
    double text_size;
    double text_angle;
    bool text_style_bold;
    bool text_style_italic;
    bool text_style_underline;
    bool text_style_overline;
    bool text_style_strikeout;
} Settings;

typedef struct State_ {
    Settings settings;

    /* Temporary for instant preview */
    Settings preview;
    Settings accept;

    /* Temporary until changes are accepted */
    Settings dialog;

    /* Paths */
    sds settings_dir;
    sds settings_path;
    sds app_dir;

    /* Documents */
    int32_t num_docs;
    int32_t doc_index;

    bool shift;

    /* Command */
    bool command_active;
    bool rapid_fire;
    bool is_blinking;
    bool blink_state;
    sds command_line;
    sds prefix;
    sds current_command;
    sds last_command;
    sdsarray *arguments;

    /* Configuration tables */
    sdsarray *manifest;
    sdsarray *tips;
    sdsarray *aliases;

    sdsarray *file_menu;
    sdsarray *edit_menu;
    sdsarray *view_menu;
    sdsarray *window_menu;
    sdsarray *help_menu;
    sdsarray *recent_menu;
    sdsarray *zoom_menu;
    sdsarray *pan_menu;

    sdsarray *file_toolbar;
    sdsarray *edit_toolbar;
    sdsarray *view_toolbar;
    sdsarray *zoom_toolbar;
    sdsarray *pan_toolbar;
    sdsarray *icon_toolbar;
    sdsarray *help_toolbar;
    sdsarray *layer_toolbar;
    sdsarray *text_toolbar;
    sdsarray *properties_toolbar;
    sdsarray *prompt_toolbar;
} State;

typedef struct CommandData_ {
    enum COMMAND_ACTIONS id;
    enum COMMAND_TYPE type;
    char label[100];
    char tooltip[1000];
    char statustip[1000];
    char shortcut[50];
    char mac_shortcut[50];
    int (*command)(State *state);
} CommandData;

char *toml_readstr(toml_table_t *table, const char *key, const char *default_value, char *result);
int32_t toml_readint(toml_table_t *table, const char *key, int32_t default_value);
float toml_readreal(toml_table_t *table, const char *key, float default_value);
bool toml_readbool(toml_table_t *table, const char *key, bool default_value);

sdsarray *sdsarray_create(void);
void sdsarray_append(sdsarray *a, const char *s);
void sdsarray_empty(sdsarray *arr);
void sdsarray_copy(sdsarray *dest, sdsarray *src);
void sdsarray_free(sdsarray *a);

void settings_create(Settings *settings); /* FIXME: convert to Settings *settings_create(void); */
int settings_load(Settings *settings, int *window_pos, int *window_size);
int settings_save(Settings *settings, int *window_pos, int *window_size);
void settings_validate(Settings *settings);
void settings_copy(Settings *dest, Settings *src);
void settings_free(Settings *settings);

void state_create(char *settings_dir, char *app_dir);
void state_free(void);

/* COMMANDS */
int do_nothing_cmd(State *state);

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

extern CommandData command_table[];
extern State state;

#ifdef __cplusplus
}
#endif

#endif

