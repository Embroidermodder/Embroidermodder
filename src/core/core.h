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

#include "toml.h"
#include "embroidery.h"

#define CONTEXT_MAIN                    0
#define CONTEXT_MENU                    1
#define CONTEXT_PROMPT                  2
#define CONTEXT_CLICK                   3
#define CONTEXT_MOVE                    4

#define STR_CHUNK                     100
#define STRARRAY_CHUNK                100

enum COMMAND_ACTIONS
{
    ACTION_donothing,

    ACTION_test,

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

    ACTION_circle,
    ACTION_distance,
    ACTION_dolphin,
    ACTION_ellipse,
    ACTION_erase,
    ACTION_heart,
    ACTION_line,
    ACTION_locatepoint,
    ACTION_move,
    ACTION_path,
    ACTION_platform,
    ACTION_point,
    ACTION_polygon,
    ACTION_polyline,
    ACTION_quickleader,
    ACTION_rectangle,
    ACTION_rgb,
    ACTION_rotate,
    ACTION_scale,
    ACTION_selectall,
    ACTION_singlelinetext,
    ACTION_snowflake,
    ACTION_star,
    ACTION_syswindows,

    //TODO: ACTION_spellcheck,
    //TODO: ACTION_quickselect,

    ACTION_null
};

enum COMMAND_TYPE
{
    CMD_TYPE_TOGGLE, 
    CMD_TYPE_TRIGGER
};

/* Null-terminated string on heap memory. */
typedef struct String_ {
    char *data;
    int memory;
    int length;
} String;

typedef struct StrArray_ {
    String **data;
    int count;
    int memory;
} StrArray;

/* ROOT and DICTIONARY nodes are assumed to contain no data.
 * The ROOT is a DICTIONARY node that has no parent.
 */
#define NODE_ROOT               0
#define NODE_DICTIONARY         1
#define NODE_STR                2
#define NODE_STRARRAY           3
#define NODE_INT                4
#define NODE_UINT               5
#define NODE_REAL               6
#define NODE_UNKNOWN            7

typedef struct Node_ Node;

struct Node_ {
    char label[50];

    /* Data */
    String *s;
    StrArray *l;
    int32_t i;
    uint32_t u;
    double r;
    int type;

    /* Leaves */
    Node *leaves;
    int n_leaves;
};

typedef struct Settings_ {
    String *general_language;
    String *general_icon_theme;
    int general_icon_size;
    bool general_mdi_bg_use_logo;
    bool general_mdi_bg_use_texture;
    bool general_mdi_bg_use_color;
    String *general_mdi_bg_logo;
    String *general_mdi_bg_texture;
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
    String *display_units;
    uint32_t prompt_text_color;
    uint32_t prompt_bg_color;
    String *prompt_font_family;
    String *prompt_font_style;
    uint8_t prompt_font_size;
    bool prompt_save_history;
    bool prompt_save_history_as_html;
    String *prompt_save_history_filename;
    String *opensave_custom_filter;
    String *opensave_open_format;
    bool opensave_open_thumbnail;
    String *opensave_save_format;
    bool opensave_save_thumbnail;
    uint8_t opensave_recent_max_files;
    StrArray *opensave_recent_list_of_files;
    String *opensave_recent_directory;
    uint8_t opensave_trim_dst_num_jumps;
    String *printing_default_device;
    bool printing_use_last_device;
    bool printing_disable_bg;
    bool grid_show_on_load;
    bool grid_show_origin;
    bool grid_color_match_crosshair;
    uint32_t grid_color;
    bool grid_load_from_file;
    String *grid_type;
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
    String *text_font;
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
    String *settings_dir;
    String *settings_path;
    String *app_dir;

    /* Documents */
    int32_t num_docs;
    int32_t doc_index;

    bool shift;

    /* Command */
    bool command_active;
    bool rapid_fire;
    bool is_blinking;
    bool blink_state;
    String *command_line;
    String *prefix;
    String *current_command;
    String *last_command;
    StrArray *arguments;
    EmbVector points[10];
    int8_t points_set;
    int8_t context;

    /* Configuration tables */
    StrArray *manifest;
    StrArray *tips;
    StrArray *aliases;

    StrArray *file_menu;
    StrArray *edit_menu;
    StrArray *view_menu;
    StrArray *window_menu;
    StrArray *help_menu;
    StrArray *recent_menu;
    StrArray *zoom_menu;
    StrArray *pan_menu;
    StrArray *draw_menu;
    StrArray *tools_menu;
    StrArray *dimension_menu;
    StrArray *modify_menu;

    StrArray *file_toolbar;
    StrArray *edit_toolbar;
    StrArray *view_toolbar;
    StrArray *zoom_toolbar;
    StrArray *pan_toolbar;
    StrArray *icon_toolbar;
    StrArray *help_toolbar;
    StrArray *layer_toolbar;
    StrArray *text_toolbar;
    StrArray *properties_toolbar;
    StrArray *prompt_toolbar;
    StrArray *draw_toolbar;
    StrArray *dimension_toolbar;
    StrArray *inquiry_toolbar;
    StrArray *modify_toolbar;
} State;

typedef struct CommandData_ {
    enum COMMAND_ACTIONS id;
    enum COMMAND_TYPE type;
    char label[100];
    char tooltip[1000];
    char statustip[1000];
    char shortcut[50];
    char mac_shortcut[50];
    char aliases[1000];
    int (*command)(State *state);
} CommandData;

int command_id(const char *cmd);

int toml_readstr(toml_table_t *table, const char *key, const char *default_value, String *result);
int32_t toml_readint(toml_table_t *table, const char *key, int32_t default_value);
float toml_readreal(toml_table_t *table, const char *key, float default_value);
bool toml_readbool(toml_table_t *table, const char *key, bool default_value);

String *str_create(const char *value);
void str_const(String *dest, const char *src);
void str_copy(String *dest, String *src);
void str_concat(String *dest, const char *src);
int str_compare(String *dest, const char *src);
int str_find(String *src, const char *key);
void str_replace(String *src, String *from, String *to);
void str_free(String *str);

StrArray *strarray_create(void);
void strarray_append(StrArray *a, const char *s);
void strarray_insert(StrArray *a, int positon, String *s);
void strarray_empty(StrArray *arr);
void strarray_copy(StrArray *dest, StrArray *src);
void strarray_free(StrArray *a);
int strarray_from_tree(Node *tree, const char *key, StrArray *a);
void print_strarray(StrArray *a);

Node *root_node(void);
Node *create_leaf(Node *trunk);
void add_str_leaf(Node *trunk, const char *label, const char *str);
/* NOTE: When creating a StrArray leaf node it begins empty. */
void add_strarray_leaf(Node *trunk, const char *label);
void add_int_leaf(Node *trunk, const char *label, int32_t i);
void add_uint_leaf(Node *trunk, const char *label, uint32_t u);
void add_real_leaf(Node *trunk, const char *label, double r);
/* Find an exact match to the key as the label of a node on this level. */
Node *find_leaf(Node *trunk, const char *key);
/* Find an exact match to the key as the label of a node on any lower level. */
Node *recursive_find_leaf(Node *root, const char *key);
/* Find leaf given the full path in "dot.notation" from the root. */
Node *get_leaf(Node *root, const char *key);
Node *pop_leaf(Node *trunk);
void free_leaf(Node *trunk);
Node *load_xml(const char *fname);
void print_tree(Node *trunk, int indent);
int tree_test(void);

void settings_create(Settings *settings); /* FIXME: convert to Settings *settings_create(void); */
int settings_load(Settings *settings, int *window_pos, int *window_size);
int settings_save(Settings *settings, int *window_pos, int *window_size);
void settings_validate(Settings *settings);
void settings_copy(Settings *dest, Settings *src);
void settings_free(Settings *settings);

void state_create(char *settings_dir, char *app_dir);
int state_load(void);
void state_free(void);

int script_env_init(void);
void script_env_run(const char *line);
void script_env_free(void);

int call(State *state, const char *cmd);

/* Configuration: note that this is all constant. */
extern const CommandData command_table[];

/* Program state: contains all global variables. */
extern State state;

#ifdef __cplusplus
}
#endif

#endif

