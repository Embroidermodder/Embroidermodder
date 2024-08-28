/*
 * Embroidermodder 2.
 *
 * Copyright 2011-2024 The Embroidermodder Team
 * Embroidermodder 2 is Open Source Software, see LICENSE.md for licensing terms.
 *
 * Read the reference manual (https://www.libembroidery.org/downloads/emrm.pdf)
 * for advice on altering this file.
 *
 * C Core
 */

#ifndef EMBROIDERMODDER_C_CORE_HEADER
#define EMBROIDERMODDER_C_CORE_HEADER

#ifdef __cplusplus
extern "C" {
#endif

#include <stdlib.h>
#include <stdbool.h>
#include <inttypes.h>
#include <math.h>

#include "../extern/libembroidery/embroidery.h"

#define REAL(i)   context->argument[i].r
#define INT(i)    context->argument[i].i
#define STR(i)    context->argument[i].s
#define BOOL(i)   context->argument[i].b

#define MAX_ARGS                      10
#define MAX_FILES                     30
#define MAX_STRING_LENGTH           1000
#define MAX_COMMANDS                 300
#define MAX_COMMAND_LENGTH            30
#define MAX_MENU_LENGTH               30
#define MAX_TOOLBAR_LENGTH            30
#define MAX_ALIASES                  500
#define MAX_STATE_VARIABLES          500
#define MAX_STATE_STRING_LENGTH      200
#define MAX_LABEL_LENGTH              50

#define SCRIPT_NULL                    0
#define SCRIPT_INT                     1
#define SCRIPT_REAL                    2
#define SCRIPT_COMMAND                 3
#define SCRIPT_STRING                  4
#define SCRIPT_BOOL                    5

/* main() is run every time the command is started.
 * Use it to reset variables so they are ready to go.
 */
#define CONTEXT_MAIN                   0
/* click() is run only for left clicks. Middle clicks are used for
 * panning. Right clicks bring up the context menu.
 */
#define CONTEXT_CLICK                  1
/* move() is optional. It is run only after enableMoveRapidFire() is
 * called. It will be called every time the mouse moves until
 * disableMoveRapidFire() is called.
 */
#define CONTEXT_MOVE                   2
/* context() is run when a context menu entry is chosen. */
#define CONTEXT_CONTEXT                3
/* prompt() is run when Enter is pressed.
 * appendPromptHistory is automatically called before prompt()
 * is called so calling it is only needed for erroneous input.
 * Any text in the command prompt is sent as an uppercase string.
 */
#define CONTEXT_PROMPT                 4

/* Command requirements. */
#define CONTEXT_FREE                0x00
#define REQUIRED_VIEW               0x01
#define REQUIRED_SCENE              0x02
#define REQUIRED_UNDO               0x04
#define DONT_INITIALIZE             0x08
#define CLEAR_SELECTION             0x10
#define DONT_END_COMMAND            0x20

/* Key Sequence */
#define CUT_SEQUENCE                   0
#define COPY_SEQUENCE                  1
#define PASTE_SEQUENCE                 2
#define SELECT_ALL_SEQUENCE            3
#define UNDO_SEQUENCE                  4
#define REDO_SEQUENCE                  5
#define DELETE_KEY                     6
#define TAB_KEY                        7
#define ESCAPE_KEY                     8
#define UP_KEY                         9
#define DOWN_KEY                      10
#define SHIFT_KEY                     11
#define F1_KEY                        12
#define F2_KEY                        13
#define F3_KEY                        14
#define F4_KEY                        15
#define F5_KEY                        16
#define F6_KEY                        17
#define F7_KEY                        18
#define F8_KEY                        19
#define F9_KEY                        20
#define F10_KEY                       21
#define F11_KEY                       22
#define F12_KEY                       23
#define N_KEY_SEQUENCES               24

/* Command Actions */
#define ACTION_NULL                    0
#define ACTION_DO_NOTHING              1

#define ACTION_NEW                     2
#define ACTION_OPEN                    3
#define ACTION_SAVE                    4
#define ACTION_SAVE_AS                 5
#define ACTION_PRINT                   6
#define ACTION_DESIGN_DETAILS          7
#define ACTION_EXIT                    8
#define ACTION_CUT                     9
#define ACTION_COPY                   10
#define ACTION_COPY_SELECTED          11
#define ACTION_PASTE                  12
#define ACTION_PASTE_SELECTED         13

#define ACTION_UNDO                   14
#define ACTION_REDO                   15

/* Window Menu */
#define ACTION_WINDOW_CLOSE           16
#define ACTION_WINDOW_CLOSE_ALL       17
#define ACTION_WINDOW_CASCADE         18
#define ACTION_WINDOW_TILE            19
#define ACTION_WINDOW_NEXT            20
#define ACTION_WINDOW_PREVIOUS        21

/* Help Menu */
#define ACTION_HELP                   22
#define ACTION_CHANGELOG              23
#define ACTION_TIP_OF_THE_DAY         24
#define ACTION_ABOUT                  25
#define ACTION_WHATS_THIS             26

/* Icons */
#define ACTION_ICON_16                27
#define ACTION_ICON_24                28
#define ACTION_ICON_32                29
#define ACTION_ICON_48                30
#define ACTION_ICON_64                31
#define ACTION_ICON_128               32

#define ACTION_SETTINGS_DIALOG        33

/* Layer ToolBar */
#define ACTION_MAKE_LAYER_CURRENT     34
#define ACTION_LAYERS                 35
#define ACTION_LAYER_SELECTOR         36
#define ACTION_LAYER_PREVIOUS         37
#define ACTION_COLOR_SELECTOR         38
#define ACTION_LINE_TYPE_SELECTOR     39
#define ACTION_LINE_WEIGHT_SELECTOR   40
#define ACTION_HIDE_ALL_LAYERS        41
#define ACTION_SHOW_ALL_LAYERS        42
#define ACTION_FREEZE_ALL_LAYERS      43
#define ACTION_THAW_ALL_LAYERS        44
#define ACTION_LOCK_ALL_LAYERS        45
#define ACTION_UNLOCK_ALL_LAYERS      46

/* Text ToolBar */
#define ACTION_TEXT_BOLD              47
#define ACTION_TEXT_ITALIC            48
#define ACTION_TEXT_UNDERLINE         49
#define ACTION_TEXT_STRIKEOUT         50
#define ACTION_TEXT_OVERLINE          51

/* Zoom ToolBar */
#define ACTION_ZOOM_REAL_TIME         52
#define ACTION_ZOOM_PREVIOUS          53
#define ACTION_ZOOM_WINDOW            54
#define ACTION_ZOOM_DYNAMIC           55
#define ACTION_ZOOM_SCALE             56
#define ACTION_ZOOM_CENTER            57
#define ACTION_ZOOM_IN                58
#define ACTION_ZOOM_OUT               59
#define ACTION_ZOOM_SELECTED          60
#define ACTION_ZOOM_ALL               61
#define ACTION_ZOOM_EXTENTS           62

/* Pan SubMenu */
#define ACTION_PAN_REAL_TIME          63
#define ACTION_PAN_POINT              64
#define ACTION_PAN_LEFT               65
#define ACTION_PAN_RIGHT              66
#define ACTION_PAN_UP                 67
#define ACTION_PAN_DOWN               68

#define ACTION_DAY                    69
#define ACTION_NIGHT                  70

/* Prompt. */
#define ACTION_ALERT                  71
#define ACTION_GET                    72
#define ACTION_SET                    73

/* Selection management. */
#define ACTION_CLEAR                  74

/* TODO: ACTION_spellcheck, */
/* TODO: ACTION_quickselect, */

#define ACTION_ANGLE                  75
#define ACTION_CIRCLE                 76
#define ACTION_DEBUG                  77
#define ACTION_DIM_LEADER             78
#define ACTION_DISABLE                79
#define ACTION_DISTANCE               80
#define ACTION_DOLPHIN                81
#define ACTION_ELLIPSE                82
#define ACTION_ENABLE                 83
#define ACTION_ERASE                  84
#define ACTION_ERROR                  85
#define ACTION_HEART                  86
#define ACTION_LINE                   87
#define ACTION_LOCATE_POINT           88
#define ACTION_MIRROR_SELECTED        89
#define ACTION_MOVE                   90
#define ACTION_MOVE_SELECTED          91
#define ACTION_PATH                   92
#define ACTION_PLATFORM               93
#define ACTION_POINT                  94
#define ACTION_POLYGON                95
#define ACTION_POLYLINE               96
#define ACTION_PREVIEW_OFF            97
#define ACTION_PREVIEW_ON             98
#define ACTION_QUICKLEADER            99
#define ACTION_RECTANGLE             100
#define ACTION_RGB                   101
#define ACTION_ROTATE                102
#define ACTION_SANDBOX               103
#define ACTION_SCALE                 104
#define ACTION_SCALE_SELECTED        105
#define ACTION_SELECT_ALL            106
#define ACTION_SINGLE_LINE_TEXT      107
#define ACTION_SNOWFLAKE             108
#define ACTION_STAR                  109
#define ACTION_SYSWINDOWS            110
#define ACTION_TODO                  111
#define ACTION_VULCANIZE             112

#define ACTION_ADD                   113
#define ACTION_DELETE                114
#define ACTION_GRIP_EDIT             115
#define ACTION_NAV                   116
#define ACTION_MIRROR                117

#define ACTION_TEST                  118

/* UI Modes */
#define CIRCLE_MODE_1P_RAD_ONE         0
#define CIRCLE_MODE_1P_RAD_TWO         1
#define CIRCLE_MODE_1P_DIA_ONE         2
#define CIRCLE_MODE_1P_DIA_TWO         3
#define CIRCLE_MODE_2P                 4
#define CIRCLE_MODE_3P                 5
#define CIRCLE_MODE_TTR                6
#define CIRCLE_MODE_TTT                7

#define ELLIPSE_MODE_MAJDIA_MINRAD_ONE     0
#define ELLIPSE_MODE_MAJDIA_MINRAD_TWO     1
#define ELLIPSE_MODE_MAJDIA_MINRAD_THREE   2
#define ELLIPSE_MODE_MAJRAD_MINRAD_ONE     3
#define ELLIPSE_MODE_MAJRAD_MINRAD_TWO     4
#define ELLIPSE_MODE_MAJRAD_MINRAD_THREE   5
#define ELLIPSE_MODE_ROTATION              6
#define ELLIPSE_MODE_NEUTRAL               7

#define POLYGON_MODE_NUM_SIDES      0
#define POLYGON_MODE_CENTER_PT      1
#define POLYGON_MODE_POLYTYPE       2
#define POLYGON_MODE_INSCRIBE       3
#define POLYGON_MODE_CIRCUMSCRIBE   4
#define POLYGON_MODE_DISTANCE       5
#define POLYGON_MODE_SIDE_LEN       6

#define TEXTSINGLE_MODE_JUSTIFY   0
#define TEXTSINGLE_MODE_SETFONT   1
#define TEXTSINGLE_MODE_SETGEOM   2
#define TEXTSINGLE_MODE_RAPID     3

/* NOTE: Allow this enum to evaluate false */
#define ARROW_STYLE_NO_ARROW    0
#define ARROW_STYLE_OPEN        1
#define ARROW_STYLE_CLOSED      2
#define ARROW_STYLE_DOT         3
#define ARROW_STYLE_BOX         4
#define ARROW_STYLE_TICK        5

/* NOTE: Allow this enum to evaluate false */
#define LINE_STYLE_NO_LINE      0
#define LINE_STYLE_FLARED       1
#define LINE_STYLE_FLETCHING    2

#define RGB_MODE_BACKGROUND     0
#define RGB_MODE_CROSSHAIR      1
#define RGB_MODE_GRID           2

#define ROTATE_MODE_NORMAL      0
#define ROTATE_MODE_REFERENCE   1

#define SCALE_MODE_NORMAL       0
#define SCALE_MODE_REFERENCE    1

#define HEART_MODE_NUM_POINTS     0
#define HEART_MODE_STYLE          1
#define HEART_MODE_XSCALE         2
#define HEART_MODE_YSCALE         3

#define HEART_STYLE_4             0
#define HEART_STYLE_5             1

#define STAR_MODE_NUM_POINTS      0
#define STAR_MODE_CENTER_PT       1
#define STAR_MODE_RAD_OUTER       2
#define STAR_MODE_RAD_INNER       3

#define DOLPHIN_MODE_NUM_POINTS   0
#define DOLPHIN_MODE_XSCALE       1
#define DOLPHIN_MODE_YSCALE       2

#define SNOWFLAKE_MODE_NUM_POINTS   0
#define SNOWFLAKE_MODE_XSCALE       1
#define SNOWFLAKE_MODE_YSCALE       2

/* Custom Data used in QGraphicsItems
 *
 *                    (     int, const QVariant)
 * I.E. object.setData(OBJ_TYPE, OBJ_TYPE_LINE);
 * I.E. object.setData(OBJ_LAYER, "OUTLINE");
 * I.E. object.setData(OBJ_COLOR, 123);
 * I.E. object.setData(OBJ_LTYPE, OBJ_LTYPE_CONT);
 */

/* Keys */
/* value type - int: See OBJ_TYPE_VALUES */
#define OBJ_TYPE                       0
/* value type - str: See OBJ_NAME_VALUES */
//#define OBJ_NAME                       1
/* value type - str: "USER", "DEFINED", "STRINGS" etc... */
#define OBJ_LAYER                      2
/* value type - int: 0-255 */
/* TODO: Use color chart in formats/format-dxf.h for this */
#define OBJ_COLOR                      3
/* value type - int: See OBJ_LTYPE_VALUES */
#define OBJ_LTYPE                      4
/* value type - int: 0-27 */
#define OBJ_LWT                        5
/* value type - int: See OBJ_RUBBER_VALUES */
#define OBJ_RUBBER                     6

/* Values
 * NOTE: Allow this enum to evaluate false
 * NOTE: Values >= 65536 ensure compatibility with qgraphicsitem_cast()
 */
#define OBJ_TYPE_NULL                  0
#define OBJ_TYPE_BASE             100000
#define OBJ_TYPE_ARC              100001
#define OBJ_TYPE_BLOCK            100002
#define OBJ_TYPE_CIRCLE           100003
#define OBJ_TYPE_DIMALIGNED       100004
#define OBJ_TYPE_DIMANGULAR       100005
#define OBJ_TYPE_DIMARCLENGTH     100006
#define OBJ_TYPE_DIMDIAMETER      100007
#define OBJ_TYPE_DIMLEADER        100008
#define OBJ_TYPE_DIMLINEAR        100009
#define OBJ_TYPE_DIMORDINATE      100010
#define OBJ_TYPE_DIMRADIUS        100011
#define OBJ_TYPE_ELLIPSE          100012
#define OBJ_TYPE_ELLIPSEARC       100013
#define OBJ_TYPE_RUBBER           100014
#define OBJ_TYPE_GRID             100015
#define OBJ_TYPE_HATCH            100016
#define OBJ_TYPE_IMAGE            100017
#define OBJ_TYPE_INFINITELINE     100018
#define OBJ_TYPE_LINE             100019
#define OBJ_TYPE_PATH             100020
#define OBJ_TYPE_POINT            100021
#define OBJ_TYPE_POLYGON          100022
#define OBJ_TYPE_POLYLINE         100023
#define OBJ_TYPE_RAY              100024
#define OBJ_TYPE_RECTANGLE        100025
#define OBJ_TYPE_SLOT             100026
#define OBJ_TYPE_SPLINE           100027
#define OBJ_TYPE_TEXTMULTI        100028
#define OBJ_TYPE_TEXTSINGLE       100029
#define OBJ_TYPE_UNKNOWN          100030

/* OBJ_LTYPE_VALUES */
/* CAD Linetypes */
#define OBJ_LTYPE_CONT                 0
#define OBJ_LTYPE_CENTER               1
#define OBJ_LTYPE_DOT                  2
#define OBJ_LTYPE_HIDDEN               3
#define OBJ_LTYPE_PHANTOM              4
#define OBJ_LTYPE_ZIGZAG               5
/* Embroidery Stitchtypes */
/* __________ */
#define OBJ_LTYPE_RUNNING              6
/* vvvvvvvvvv */
#define OBJ_LTYPE_SATIN                7
/* >>>>>>>>>> */
#define OBJ_LTYPE_FISHBONE             8

/* OBJ_LWT_VALUES */
#define OBJ_LWT_BYLAYER               -2
#define OBJ_LWT_BYBLOCK               -1
#define OBJ_LWT_DEFAULT                0
#define OBJ_LWT_01                     1
#define OBJ_LWT_02                     2
#define OBJ_LWT_03                     3
#define OBJ_LWT_04                     4
#define OBJ_LWT_05                     5
#define OBJ_LWT_06                     6
#define OBJ_LWT_07                     7
#define OBJ_LWT_08                     8
#define OBJ_LWT_09                     9
#define OBJ_LWT_10                    10
#define OBJ_LWT_11                    11
#define OBJ_LWT_12                    12
#define OBJ_LWT_13                    13
#define OBJ_LWT_14                    14
#define OBJ_LWT_15                    15
#define OBJ_LWT_16                    16
#define OBJ_LWT_17                    17
#define OBJ_LWT_18                    18
#define OBJ_LWT_19                    19
#define OBJ_LWT_20                    20
#define OBJ_LWT_21                    21
#define OBJ_LWT_22                    22
#define OBJ_LWT_23                    23
#define OBJ_LWT_24                    24

/* OBJ_SNAP_VALUES NOTE: Allow this enum to evaluate false */
#define OBJ_SNAP_NULL                  0
#define OBJ_SNAP_ENDPOINT              1
#define OBJ_SNAP_MIDPOINT              2
#define OBJ_SNAP_CENTER                3
#define OBJ_SNAP_NODE                  4
#define OBJ_SNAP_QUADRANT              5
#define OBJ_SNAP_INTERSECTION          6
#define OBJ_SNAP_EXTENSION             7
#define OBJ_SNAP_INSERTION             8
#define OBJ_SNAP_PERPENDICULAR         9
#define OBJ_SNAP_TANGENT              10
#define OBJ_SNAP_NEAREST              11
#define OBJ_SNAP_APPINTERSECTION      12
#define OBJ_SNAP_PARALLEL             13

/* NOTE: Allow this enum to evaluate true and false */
#define OBJ_RUBBER_OFF                 0
#define OBJ_RUBBER_ON                  1

#define OBJ_RUBBER_CIRCLE_1P_RAD       2
#define OBJ_RUBBER_CIRCLE_1P_DIA       3
#define OBJ_RUBBER_CIRCLE_2P           4
#define OBJ_RUBBER_CIRCLE_3P           5
#define OBJ_RUBBER_CIRCLE_TTR          6
#define OBJ_RUBBER_CIRCLE_TTT          7

#define OBJ_RUBBER_DIMLEADER_LINE      8

#define OBJ_RUBBER_ELLIPSE_LINE        9
#define OBJ_RUBBER_ELLIPSE_MAJORDIAMETER_MINORRADIUS 10
#define OBJ_RUBBER_ELLIPSE_MAJORRADIUS_MINORRADIUS 11
#define OBJ_RUBBER_ELLIPSE_ROTATION   12

#define OBJ_RUBBER_GRIP               13

#define OBJ_RUBBER_LINE               14

#define OBJ_RUBBER_POLYGON            15
#define OBJ_RUBBER_POLYGON_INSCRIBE   16
#define OBJ_RUBBER_POLYGON_CIRCUMSCRIBE 17

#define OBJ_RUBBER_POLYLINE           18

#define OBJ_RUBBER_IMAGE              19

#define OBJ_RUBBER_RECTANGLE          20

#define OBJ_RUBBER_TEXTSINGLE         21

/* NOTE: Allow this enum to evaluate false */
#define SPARE_RUBBER_OFF               0
#define SPARE_RUBBER_PATH              1
#define SPARE_RUBBER_POLYGON           2
#define SPARE_RUBBER_POLYLINE          3

/* NOTE: Allow this enum to evaluate false */
#define PREVIEW_CLONE_NULL             0
#define PREVIEW_CLONE_SELECTED         1
#define PREVIEW_CLONE_RUBBER           2

/* NOTE: Allow this enum to evaluate false */
#define PREVIEW_MODE_NULL              0
#define PREVIEW_MODE_MOVE              1
#define PREVIEW_MODE_ROTATE            2
#define PREVIEW_MODE_SCALE             3

typedef struct ScriptValue_ {
    double r;
    int i;
    bool b;
    char s[MAX_STATE_STRING_LENGTH];
    char label[MAX_LABEL_LENGTH];
    int n_leaves;
    char type;
} ScriptValue;

typedef struct ScriptEnv_ {
    ScriptValue *variable;
    int n_variables;
    int context;
    int mode;
    bool firstRun;
    ScriptValue argument[MAX_ARGS];
    int argumentCount;
} ScriptEnv;

typedef struct Command_ {
    int32_t id;
    char command[MAX_STRING_LENGTH];
    char arguments[MAX_STRING_LENGTH];
    char icon[MAX_STRING_LENGTH];
    char tooltip[MAX_STRING_LENGTH];
    char statustip[MAX_STRING_LENGTH];
    char alias[MAX_STRING_LENGTH];
    char shortcut[MAX_STRING_LENGTH];
    int32_t flags;
} Command;

typedef struct EmbDimLeader_ {
    EmbVector start;
    EmbVector end;
    int arrow_style;
    int line_style;
    EmbColor color;
} EmbDimLeader;

typedef struct RealSetting_ {
    char key[MAX_STRING_LENGTH];
    double setting;
    double dialog;
    double accept;
    double preview;
} RealSetting;

typedef struct BoolSetting_ {
    char key[MAX_STRING_LENGTH];
    bool setting;
    bool dialog;
    bool accept;
    bool preview;
} BoolSetting;

typedef struct IntSetting_ {
    char key[MAX_STRING_LENGTH];
    int32_t setting;
    int32_t dialog;
    int32_t accept;
    int32_t preview;
} IntSetting;

typedef struct StringSetting_ {
    char key[MAX_STRING_LENGTH];
    char setting[MAX_STRING_LENGTH];
    char dialog[MAX_STRING_LENGTH];
    char accept[MAX_STRING_LENGTH];
    char preview[MAX_STRING_LENGTH];
} StringSetting;

typedef struct StringTableSetting_ {
    char key[MAX_STRING_LENGTH];
    char setting[MAX_FILES][MAX_STRING_LENGTH];
    char dialog[MAX_FILES][MAX_STRING_LENGTH];
    char accept[MAX_FILES][MAX_STRING_LENGTH];
    char preview[MAX_FILES][MAX_STRING_LENGTH];
} StringTableSetting;

typedef struct StringMap_ {
    char key[MAX_LABEL_LENGTH];
    char value[MAX_LABEL_LENGTH];
} StringMap;

typedef struct Editor_ {
    char icon[MAX_STRING_LENGTH];
    char label[MAX_STRING_LENGTH];
    char data_type[MAX_STRING_LENGTH];
    char signal[MAX_STRING_LENGTH];
    int object;
} Editor;

typedef struct WidgetData_ {
    char label[MAX_STRING_LENGTH];
    BoolSetting *setting;
    char icon[MAX_STRING_LENGTH];
} WidgetData;

/* Scripting functions */
ScriptEnv *create_script_env();
void free_script_env(ScriptEnv *);

ScriptValue script_bool(bool b);
ScriptValue script_int(int i);
ScriptValue script_real(double r);
ScriptValue script_string(const char *s);
ScriptValue command_prompt(ScriptEnv *context, const char *line);

ScriptEnv *add_string_argument(ScriptEnv *context, const char *s);
ScriptEnv *add_real_argument(ScriptEnv *context, double r);
ScriptEnv *add_int_argument(ScriptEnv *context, int i);

void add_string_variable(ScriptEnv *context, const char *label, const char *s);
void add_int_variable(ScriptEnv *context, const char *label, int i);
void add_real_variable(ScriptEnv *context, const char *label, double i);

const char *script_get_string(ScriptEnv *context, const char *label);
int script_get_int(ScriptEnv *context, const char *label);
double script_get_real(ScriptEnv *context, const char *label);

int script_set_string(ScriptEnv *context, const char *label, const char *s);
int script_set_int(ScriptEnv *context, const char *label, int i);
int script_set_real(ScriptEnv *context, const char *label, double r);

void prompt_output(const char *);
int argument_checks(ScriptEnv *context, int id);
char *translate(const char *msg);

bool pattern_save(EmbPattern *pattern, const char *fileName);

int string_array_length(char *s[]);
int parse_floats(char *line, float result[], int n);
int parse_vector(char *line, EmbVector *v);
bool validRGB(float r, float g, float b);
void reportDistance(EmbVector a, EmbVector b);

int load_data(void);

int load_settings(char *appDir, char *configDir);
int save_settings(const char *appDir, const char *configDir);

int get_command_id(const char *);
int get_state_variable(const char *key);

EmbArc emb_arc_set_radius(EmbArc a, EmbReal radius);

bool willUnderflowInt32(int64_t a, int64_t b);
bool willOverflowInt32(int64_t a, int64_t b);
int roundToMultiple(bool roundUp, int numToRound, int multiple);

/* MainWindow calls */
void end_command(void);
void debug_message(const char *msg);
void wait_cursor(void);
void arrow_cursor(void);
void restore_cursor(void);

void cut(void);

void about_dialog(void);

/* Global variables with c linkage. */
extern Command command_data[MAX_COMMANDS];
extern StringMap aliases[MAX_ALIASES];

extern ScriptValue script_null;
extern ScriptValue script_true;
extern ScriptValue script_false;

extern int testing_mode;

extern StringSetting general_language;
extern StringSetting general_icon_theme;
extern IntSetting general_icon_size;
extern BoolSetting general_mdi_bg_use_logo;
extern BoolSetting general_mdi_bg_use_texture;
extern BoolSetting general_mdi_bg_use_color;
extern StringSetting general_mdi_bg_logo;
extern StringSetting general_mdi_bg_texture;
extern IntSetting general_mdi_bg_color;
extern BoolSetting general_tip_of_the_day;
extern IntSetting general_current_tip;
extern BoolSetting general_system_help_browser;
extern BoolSetting general_check_for_updates;
extern BoolSetting display_use_opengl;
extern BoolSetting display_renderhint_aa;
extern BoolSetting display_renderhint_text_aa;
extern BoolSetting display_renderhint_smooth_pix;
extern BoolSetting display_renderhint_high_aa;
extern BoolSetting display_renderhint_noncosmetic;
extern BoolSetting display_show_scrollbars;
extern IntSetting display_scrollbar_widget_num;
extern IntSetting display_crosshair_color;
extern IntSetting display_bg_color;
extern IntSetting display_selectbox_left_color;
extern IntSetting display_selectbox_left_fill;
extern IntSetting display_selectbox_right_color;
extern IntSetting display_selectbox_right_fill;
extern IntSetting display_selectbox_alpha;
extern RealSetting display_zoomscale_in;
extern RealSetting display_zoomscale_out;
extern IntSetting display_crosshair_percent;
extern StringSetting display_units;
extern IntSetting prompt_text_color;
extern IntSetting prompt_bg_color;
extern StringSetting prompt_font_family;
extern StringSetting prompt_font_style;
extern IntSetting prompt_font_size;
extern BoolSetting prompt_save_history;
extern BoolSetting prompt_save_history_as_html;
extern StringSetting prompt_save_history_filename;
extern StringSetting opensave_custom_filter;
extern StringSetting opensave_open_format;
extern BoolSetting opensave_open_thumbnail;
extern StringSetting opensave_save_format;
extern BoolSetting opensave_save_thumbnail;
extern IntSetting opensave_recent_max_files;
extern StringTableSetting opensave_recent_list_of_files;
extern StringSetting opensave_recent_directory;
extern IntSetting opensave_trim_dst_num_jumps;
extern StringSetting printing_default_device;
extern BoolSetting printing_use_last_device;
extern BoolSetting printing_disable_bg;
extern BoolSetting grid_show_on_load;
extern BoolSetting grid_show_origin;
extern BoolSetting grid_color_match_crosshair;
extern IntSetting grid_color;
extern BoolSetting grid_load_from_file;
extern StringSetting grid_type;
extern BoolSetting grid_center_on_origin;
extern RealSetting grid_center_x;
extern RealSetting grid_center_y;
extern RealSetting grid_size_x;
extern RealSetting grid_size_y;
extern RealSetting grid_spacing_x;
extern RealSetting grid_spacing_y;
extern RealSetting grid_size_radius;
extern RealSetting grid_spacing_radius;
extern RealSetting grid_spacing_angle;
extern BoolSetting ruler_show_on_load;
extern BoolSetting ruler_metric;
extern IntSetting ruler_color;
extern IntSetting ruler_pixel_size;
extern BoolSetting qsnap_enabled;
extern IntSetting qsnap_locator_color;
extern IntSetting qsnap_locator_size;
extern IntSetting qsnap_aperture_size;
extern BoolSetting qsnap_endpoint;
extern BoolSetting qsnap_midpoint;
extern BoolSetting qsnap_center;
extern BoolSetting qsnap_node;
extern BoolSetting qsnap_quadrant;
extern BoolSetting qsnap_intersection;
extern BoolSetting qsnap_extension;
extern BoolSetting qsnap_insertion;
extern BoolSetting qsnap_perpendicular;
extern BoolSetting qsnap_tangent;
extern BoolSetting qsnap_nearest;
extern BoolSetting qsnap_apparent;
extern BoolSetting qsnap_parallel;
extern BoolSetting lwt_show_lwt;
extern BoolSetting lwt_real_render;
extern RealSetting lwt_default_lwt;
extern BoolSetting selection_mode_pickfirst;
extern BoolSetting selection_mode_pickadd;
extern BoolSetting selection_mode_pickdrag;
extern IntSetting selection_coolgrip_color;
extern IntSetting selection_hotgrip_color;
extern IntSetting selection_grip_size;
extern IntSetting selection_pickbox_size;
extern StringSetting text_font;
extern RealSetting text_size;
extern RealSetting text_angle;
extern BoolSetting text_style_bold;
extern BoolSetting text_style_italic;
extern BoolSetting text_style_underline;
extern BoolSetting text_style_overline;
extern BoolSetting text_style_strikeout;

//extern ScriptValue state[MAX_STATE_VARIABLES];

extern Editor general_editor_data[];
extern Editor geometry_arc_editor_data[];
extern Editor misc_arc_editor_data[];
extern Editor geometry_block_editor_data[];
extern Editor geometry_circle_editor_data[];
extern Editor geometry_dim_aligned_editor_data[];
extern Editor geometry_dim_angular_editor_data[];
extern Editor geometry_dim_arc_length_editor_data[];
extern Editor geometry_dim_diameter_editor_data[];
extern Editor geometry_dim_leader_editor_data[];
extern Editor geometry_dim_linear_editor_data[];
extern Editor geometry_dim_ordinate_editor_data[];
extern Editor geometry_dim_radius_editor_data[];
extern Editor geometry_ellipse_editor_data[];
extern Editor geometry_image_editor_data[];
extern Editor misc_image_editor_data[];
extern Editor geometry_infinite_line_editor_data[];
extern Editor geometry_line_editor_data[];
extern Editor geometry_path_editor_data[];
extern Editor misc_path_editor_data[];
extern Editor geometry_point_editor_data[];
extern Editor geometry_polygon_editor_data[];
extern Editor geometry_polyline_editor_data[];
extern Editor misc_polyline_editor_data[];
extern Editor geometry_ray_editor_data[];
extern Editor geometry_rectangle_editor_data[];
extern Editor geometry_text_multi_editor_data[];
extern Editor text_text_single_editor_data[];
extern Editor geometry_text_single_editor_data[];
extern Editor misc_text_single_editor_data[];

extern const char *_appName_;
extern const char *_appVer_;

extern WidgetData render_hints[];

extern const char *coverage_test[];
extern const char *object_names[];

extern char *menubar_full_list[];
extern char *menubar_no_docs[];

extern char *file_menu[];
extern char *edit_menu[];
extern char *view_menu[];
extern char *zoom_menu[];
extern char *pan_menu[];
extern char *help_menu[];
extern char *draw_menu[];
extern char *tools_menu[];
extern char *modify_menu[];
extern char *dimension_menu[];
extern char *sandbox_menu[];

extern char *toolbars_when_docs[];

extern char *file_toolbar[];
extern char *edit_toolbar[];
extern char *view_toolbar[];
extern char *zoom_toolbar[];
extern char *pan_toolbar[] ;
extern char *icon_toolbar[];
extern char *help_toolbar[];
extern char *draw_toolbar[];
extern char *inquiry_toolbar[];
extern char *modify_toolbar[];
extern char *dimension_toolbar[];
extern char *sandbox_toolbar[];

extern char *extensions[];
extern char *editor_list[];
extern char *combobox_list[];

extern char *grid_load_from_file_group[];
extern char *defined_origin_group[];
extern char *rectangular_grid_group[];
extern char *circular_grid_group[];
extern char *center_on_origin_group[];

extern char *tips[];

extern int numOfDocs;
extern int docIndex;
extern bool shiftKeyPressedState;
extern bool cmdActive;
extern bool rapidFireEnabled;
extern bool isBlinking;
extern bool key_state[N_KEY_SEQUENCES];

/* Natives */
void about_dialog(void);
void stub_implement(const char *txt);
void stub_testing(void);
void run_testing(void);
void exit_program(void);
void check_for_updates(void);
void new_file(void);
void open_recent_file(void);
void save_file(void);
void save_as_file(void);
void update_interface(void);
void windowMenuAboutToShow();
void hide_unimplemented(void);
void start_blinking(void);
void stop_blinking(void);

ScriptValue run_command(const char *cmd, ScriptEnv *context);

void nativeBlinkPrompt();

void checkBoxTipOfTheDayStateChanged(int checked);
void buttonTipOfTheDayClicked(int button);

ScriptValue previewon_command(ScriptEnv *context);
ScriptValue get_command(ScriptEnv *context);
ScriptValue set_command(ScriptEnv *context);
ScriptValue move_command(ScriptEnv *context);
ScriptValue sandbox_command(ScriptEnv *context);

void nativeAddInfiniteLine(double x1, double y1, double x2, double y2, double rot);
void nativeAddRay(double x1, double y1, double x2, double y2, double rot);
void nativeAddLine(double x1, double y1, double x2, double y2, double rot, int rubberMode);
void nativeAddTriangle(double x1, double y1, double x2, double y2, double x3, double y3, double rot, bool fill);
void nativeAddRectangle(double x, double y, double w, double h, double rot, bool fill, int rubberMode);
void nativeAddRoundedRectangle(double x, double y, double w, double h, double rad, double rot, bool fill);
void nativeAddArc(double x1, double y1, double x2, double y2, double x3, double y3, int rubberMode);
void nativeAddCircle(double centerX, double centerY, double radius, bool fill, int rubberMode);
void nativeAddSlot(double centerX, double centerY, double diameter, double length, double rot, bool fill, int rubberMode);
void nativeAddEllipse(double centerX, double centerY, double width, double height, double rot, bool fill, int rubberMode);
void nativeAddPoint(double x, double y);
void nativeAddRegularPolygon(double centerX, double centerY, uint16_t sides, uint8_t mode, double rad, double rot, bool fill);
void nativeAddHorizontalDimension(double x1, double y1, double x2, double y2, double legHeight);
void nativeAddVerticalDimension(double x1, double y1, double x2, double y2, double legHeight);
void nativeAddDimLeader(double x1, double y1, double x2, double y2, double rot, int rubberMode);

void nativeRedo();

void setMouseCoord(double x, double y);

void nativePrintArea(double x, double y, double w, double h);

void nativeSetBackgroundColor(uint8_t r, uint8_t g, uint8_t b);
void nativeSetCrossHairColor(uint8_t r, uint8_t g, uint8_t b);
void nativeSetGridColor(uint8_t r, uint8_t g, uint8_t b);

void nativeClearRubber();
bool nativeAllowRubber();
void nativeSpareRubber(int64_t id);

void nativeDeleteSelected();
void nativeCutSelected(double x, double y);
void nativeCopySelected(double x, double y);
void nativePasteSelected(double x, double y);
void nativeMoveSelected(double dx, double dy);
void nativeScaleSelected(double x, double y, double factor);
void nativeRotateSelected(double x, double y, double rot);
void nativeMirrorSelected(double x1, double y1, double x2, double y2);

void nativeSetCursorShape(char str[MAX_STRING_LENGTH]);
double nativeCalculateDistance(double x1, double y1, double x2, double y2);
double nativePerpendicularDistance(double px, double py, double x1, double y1, double x2, double y2);

double nativeQSnapX();
double nativeQSnapY();

void enableLwt();
void disableLwt();
void enableReal();
void disableReal();

void create_details_dialog(void);

void nativeSetPromptPrefix(char txt[MAX_STRING_LENGTH]);
/* TODO: void nativeSetRubberFilter(int64_t id); */
/* TODO: This is so more than 1 rubber object can exist at one time without updating all rubber objects at once. */
void nativeSetRubberMode(int mode);
void nativeSetRubberPoint(char key[MAX_STRING_LENGTH], double x, double y);
void nativeSetRubberText(char key[MAX_STRING_LENGTH], char txt[MAX_STRING_LENGTH]);

void toggleGrid(void);
void toggleRuler(void);
void toggleLwt(void);

/* Help Menu */
void tipOfTheDay();
void buttonTipOfTheDayClicked(int);
void checkBoxTipOfTheDayStateChanged(int);
void help();
void changelog();
void whatsThisContextHelp();

int make_application(int argc, char* argv[]);

/* Geometry */
double get_width(EmbGeometry geometry);
double get_height(EmbGeometry geometry);
double get_radius(EmbGeometry geometry);
double get_radius_major(EmbGeometry geometry);
double get_radius_minor(EmbGeometry geometry);
double get_diameter_major(EmbGeometry geometry);
double get_diameter_minor(EmbGeometry geometry);
EmbVector get_quadrant(EmbGeometry geometry, int degrees);
double get_angle(EmbGeometry geometry);
double get_start_angle(EmbGeometry geometry);
double get_end_angle(EmbGeometry geometry);
double get_arc_length(EmbGeometry geometry);
double get_area(EmbGeometry geometry);
double get_chord(EmbGeometry geometry);
double get_included_angle(EmbGeometry geometry);
bool get_clockwise(EmbGeometry geometry);

void set_start_angle(EmbGeometry *geometry, double angle);
void set_end_angle(EmbGeometry *geometry, double angle);
void set_start_point(EmbGeometry *geometry, EmbVector point);
void set_mid_point(EmbGeometry *geometry, EmbVector point);
void set_end_point(EmbGeometry *geometry, EmbVector point);
void set_diameter(EmbGeometry *geometry, double diameter);
void set_area(EmbGeometry *geometry, double area);
void set_circumference(EmbGeometry *geometry, double circumference);
void set_radius(EmbGeometry *geometry, double radius);
void set_radius_major(EmbGeometry *geometry, double radius);
void set_radius_minor(EmbGeometry *geometry, double radius);
void set_diameter_major(EmbGeometry *geometry, double diameter);
void set_diameter_minor(EmbGeometry *geometry, double diameter);

#ifdef __cplusplus
}
#endif

#endif
