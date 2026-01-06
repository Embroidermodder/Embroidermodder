/*
 * Embroidermodder 2
 * Copyright 2011-2025 The Embroidermodder Team
 *
 * Embroidermodder 2 is free and open software under the zlib license:
 * see LICENSE.md for details.
 *
 * ----------------------------------------------------------------------------
 *
 * Minimal include for commands/, fills/ and designs/ directories.
 *
 * To speed up compilation, this is the referenced header for much of the
 * codebase allowing access to enough of the program state via calls of this
 * form:
 *
 *     int function(State *state);
 *
 * These function declarations are collected into the headers "commands.h",
 * "fills.h" and "designs.h".
 */

#ifndef __EM2_CORE__
#define __EM2_CORE__

#ifdef __cplusplus
extern "C" {
#endif

#include <inttypes.h>
#include <stdbool.h>
#include <stdarg.h>

#include "embroidery.h"

/*
 * Integer and string constants and external table declarations.
 *
 * These macros are all for optimization of either lookup using explicit indicies
 * or tokens for switch tables.
 *
 * Note that they all use `#define` and are only whole number constants.
 * We prefer not to use `enum` instead keeping all of these constants within
 * an `int32_t` because we give an explicit value every time so we can
 * cross reference these in the configuration.
 */

#define VERSION_MAJOR                  2
#define VERSION_MINOR                  0
#define VERSION_PATCH                  0
#define VERSION_TAG              "alpha"

/* FIXME: this needs changing in libembroidery. */
#undef N_COMMANDS

#define MAX_ARG_LENGTH               200
#define MAX_ARGUMENTS                 20
#define MAX_COMMANDS                 200
#define MAX_SETTINGS                 200
#define MAX_COMMANDS                 200

/* Contexts for command calls. */
#define CONTEXT_MAIN                   0
#define CONTEXT_CLICK                  1
#define CONTEXT_MENU                   2
#define CONTEXT_MOVE                   3
#define CONTEXT_PROMPT                 4

/* Settings IDs */
#define ST_LANGUAGE                    0
#define ST_ICON_THEME                  1
#define ST_ICON_SIZE                   2
#define ST_MDI_BG_USE_LOGO             3
#define ST_MDI_BG_USE_TEXTURE          4
#define ST_MDI_BG_USE_COLOR            5
#define ST_MDI_BG_LOGO                 6
#define ST_MDI_BG_TEXTURE              7
#define ST_MDI_BG_COLOR                8
#define ST_TIP_OF_THE_DAY              9
#define ST_CURRENT_TIP                10
#define ST_SYSTEM_HELP_BROWSER        11
#define ST_CHECK_FOR_UPDATES          12
#define ST_USE_OPENGL                 13
#define ST_RENDERHINT_AA              14
#define ST_RENDERHINT_TEXT_AA         15
#define ST_RENDERHINT_SMOOTHPIX       16
#define ST_RENDERHINT_HIGH_AA         17
#define ST_RENDERHINT_NONCOSMETIC     18
#define ST_SHOW_SCROLLBARS            19
#define ST_SCROLLBAR_WIDGET_NUM       20
#define ST_CROSSHAIR_COLOR            21
#define ST_BG_COLOR                   22
#define ST_SELECTBOX_LEFT_COLOR       23
#define ST_SELECTBOX_LEFT_FILL        24
#define ST_SELECTBOX_RIGHT_COLOR      25
#define ST_SELECTBOX_RIGHT_FILL       26
#define ST_SELECTBOX_ALPHA            27
#define ST_ZOOMSCALE_IN               28
#define ST_ZOOMSCALE_OUT              29
#define ST_CROSSHAIR_PERCENT          30
#define ST_UNITS                      31
#define ST_PROMPT_TEXT_COLOR          32
#define ST_PROMPT_BG_COLOR            33
#define ST_PROMPT_FONT_FAMILY         34
#define ST_PROMPT_FONT_STYLE          35
#define ST_PROMPT_FONT_SIZE           36
#define ST_PROMPT_SAVE_HISTORY        37
#define ST_PROMPT_SAVE_AS_HTML        38
#define ST_PROMPT_SAVE_FNAME          39
#define ST_OPENSAVE_CUSTOM_FILTER     40
#define ST_OPEN_FORMAT                41
#define ST_OPEN_THUMBNAIL             42
#define ST_SAVE_FORMAT                43
#define ST_SAVE_THUMBNAIL             44
#define ST_RECENT_MAX_FILES           45
#define ST_RECENT_FILES               46
#define ST_RECENT_DIRECTORY           47
#define ST_TRIM_DST_NUM_JUMPS         48
#define ST_PRINTING_DEFAULT_DEVICE    49
#define ST_PRINTING_USE_LAST_DEVICE   50
#define ST_PRINTING_DISABLE_BG        51
#define ST_GRID_SHOW_ON_LOAD          52
#define ST_GRID_SHOW_ORIGIN           53
#define ST_GRID_COLOR_MATCH_CROSSHAIR 54
#define ST_GRID_COLOR                 55
#define ST_GRID_LOAD_FROM_FILE        56
#define ST_GRID_TYPE                  57
#define ST_GRID_CENTER_ON_ORIGIN      58
#define ST_GRID_CENTER_X              59
#define ST_GRID_CENTER_Y              60
#define ST_GRID_SIZE_X                61
#define ST_GRID_SIZE_Y                62
#define ST_GRID_SPACING_X             63
#define ST_GRID_SPACING_Y             64
#define ST_GRID_SIZE_RADIUS           65
#define ST_GRID_SPACING_RADIUS        66
#define ST_GRID_SPACING_ANGLE         67
#define ST_RULER_SHOW_ON_LOAD         68
#define ST_RULER_METRIC               69
#define ST_RULER_COLOR                70
#define ST_RULER_PIXEL_SIZE           71
#define ST_QSNAP_ENABLED              72
#define ST_QSNAP_LOCATOR_COLOR        73
#define ST_QSNAP_LOCATOR_SIZE         74
#define ST_QSNAP_APERTURE_SIZE        75
#define ST_QSNAP_ENDPOINT             76
#define ST_QSNAP_MIDPOINT             77
#define ST_QSNAP_CENTER               78
#define ST_QSNAP_NODE                 79
#define ST_QSNAP_QUADRANT             80
#define ST_QSNAP_INTERSECTION         81
#define ST_QSNAP_EXTENSION            82
#define ST_QSNAP_INSERTION            83
#define ST_QSNAP_PERPENDICULAR        84
#define ST_QSNAP_TANGENT              85
#define ST_QSNAP_NEAREST              86
#define ST_QSNAP_APPARENT             87
#define ST_QSNAP_PARALLEL             88
#define ST_LWT_SHOW_LWT               89
#define ST_LWT_REAL_RENDER            90
#define ST_LWT_DEFAULT_LWT            91
#define ST_SELECTION_MODE_PICKFIRST   92
#define ST_SELECTION_MODE_PICKADD     93
#define ST_SELECTION_MODE_PICKDRAG    94
#define ST_SELECTION_COOLGRIP_COLOR   95
#define ST_SELECTION_HOTGRIP_COLOR    96
#define ST_SELECTION_GRIP_SIZE        97
#define ST_SELECTION_PICKBOX_SIZE     98
#define ST_TEXT_FONT                  99
#define ST_TEXT_SIZE                 100
#define ST_TEXT_ANGLE                101
#define ST_TEXT_BOLD                 102
#define ST_TEXT_ITALIC               103
#define ST_TEXT_UNDERLINE            104
#define ST_TEXT_OVERLINE             105
#define ST_TEXT_STRIKEOUT            106
#define N_SETTINGS                   107

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
#define OBJ_NAME                       1
/* value type - str: "USER", "DEFINED", "STRINGS", etc... */
#define OBJ_LAYER                      2
/* value type - int: 0-255
 * TODO: Use color chart in formats/format-dxf.h for this
 */
#define OBJ_COLOR                      3
/* value type - int: See OBJ_LTYPE_VALUES */
#define OBJ_LTYPE                      4
/* value type - int: 0-27 */
#define OBJ_LWT                        5
/* value type - int: See OBJ_RUBBER_VALUES */
#define OBJ_RUBBER                     6

/* Values
 * NOTE: Allow this enum to evaluate false
 * NOTE: Other values >= 65536 ensure compatibility with qgraphicsitem_cast()
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

/* OBJ_SNAP_VALUES
 * NOTE: Allow this enum to evaluate false.
 */
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

/* OBJ_RUBBER_VALUES
 * NOTE: Allow this enum to evaluate false and true
 */
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
#define OBJ_RUBBER_ELLIPSE_MAJORRADIUS_MINORRADIUS   11
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

/* SPARE_RUBBER_VALUES
 * NOTE: Allow this enum to evaluate false
 */
#define SPARE_RUBBER_OFF               0
#define SPARE_RUBBER_PATH              1
#define SPARE_RUBBER_POLYGON           2
#define SPARE_RUBBER_POLYLINE          3

/* PREVIEW_CLONE_VALUES
 * NOTE: Allow this enum to evaluate false
 */
#define PREVIEW_CLONE_NULL             0
#define PREVIEW_CLONE_SELECTED         1
#define PREVIEW_CLONE_RUBBER           2

/* PREVIEW_MODE_VALUES
 * NOTE: Allow this enum to evaluate false
 */
#define PREVIEW_MODE_NULL              0
#define PREVIEW_MODE_MOVE              1
#define PREVIEW_MODE_ROTATE            2
#define PREVIEW_MODE_SCALE             3

/* OBJ_NAME_VALUES */
#define OBJ_NAME_NULL             "Unknown"
#define OBJ_NAME_BASE             "Base"
#define OBJ_NAME_ARC              "Arc"
#define OBJ_NAME_BLOCK            "Block"
#define OBJ_NAME_CIRCLE           "Circle"
#define OBJ_NAME_DIMALIGNED       "Aligned Dimension"
#define OBJ_NAME_DIMANGULAR       "Angular Dimension"
#define OBJ_NAME_DIMARCLENGTH     "Arc Length Dimension"
#define OBJ_NAME_DIMDIAMETER      "Diameter Dimension"
#define OBJ_NAME_DIMLEADER        "Leader Dimension"
#define OBJ_NAME_DIMLINEAR        "Linear Dimension"
#define OBJ_NAME_DIMORDINATE      "Ordinate Dimension"
#define OBJ_NAME_DIMRADIUS        "Radius Dimension"
#define OBJ_NAME_ELLIPSE          "Ellipse"
#define OBJ_NAME_ELLIPSEARC       "Elliptical Arc"
#define OBJ_NAME_RUBBER           "Rubber"
#define OBJ_NAME_GRID             "Grid"
#define OBJ_NAME_HATCH            "Hatch"
#define OBJ_NAME_IMAGE            "Image"
#define OBJ_NAME_INFINITELINE     "Infinite Line"
#define OBJ_NAME_LINE             "Line"
#define OBJ_NAME_PATH             "Path"
#define OBJ_NAME_POINT            "Point"
#define OBJ_NAME_POLYGON          "Polygon"
#define OBJ_NAME_POLYLINE         "Polyline"
#define OBJ_NAME_RAY              "Ray"
#define OBJ_NAME_RECTANGLE        "Rectangle"
#define OBJ_NAME_SLOT             "Slot"
#define OBJ_NAME_SPLINE           "Spline"
#define OBJ_NAME_TEXTMULTI        "Multi Line Text"
#define OBJ_NAME_TEXTSINGLE       "Single Line Text"

#define ENABLE_SNAP               "ENABLE_SNAP"
#define ENABLE_GRID               "ENABLE_GRID"
#define ENABLE_RULER              "ENABLE_RULER"
#define ENABLE_ORTHO              "ENABLE_ORTHO"
#define ENABLE_POLAR              "ENABLE_POLAR"
#define ENABLE_QSNAP              "ENABLE_QSNAP"
#define ENABLE_QTRACK             "ENABLE_QTRACK"
#define ENABLE_LWT                "ENABLE_LWT"
#define ENABLE_REAL               "ENABLE_REAL"

#define SCENE_QSNAP_POINT         "SCENE_QSNAP_POINT"
#define SCENE_MOUSE_POINT         "SCENE_MOUSE_POINT"
#define VIEW_MOUSE_POINT          "VIEW_MOUSE_POINT"
#define RUBBER_ROOM               "RUBBER_ROOM"

#define VIEW_COLOR_BACKGROUND     "VIEW_COLOR_BACKGROUND"
#define VIEW_COLOR_CROSSHAIR      "VIEW_COLOR_CROSSHAIR"
#define VIEW_COLOR_GRID           "VIEW_COLOR_GRID"

typedef struct ScriptValue_ {
    char s[200];
    char l[20][200];
    int32_t i;
    uint32_t u;
    double r;
    bool b;
    int length;
} ScriptValue;

/* Data for a given interface setting. */
typedef struct SettingsData_ {
    char section[100];
    char key[100];
    char default_value[100];
    char type;
    bool enabled;
    char description[1000];
    char icon[1000];
    double lower;
    double upper;
    double single_step;
} SettingsData;

/* Type declarations */
typedef struct ViewData_ {
    uint8_t grippingActive;
    uint8_t rapidMoveActive;
    uint8_t previewActive;
    uint8_t pastingActive;
    uint8_t movingActive;
    uint8_t selectingActive;
    uint8_t zoomWindowActive;
    uint8_t panningRealTimeActive;
    uint8_t panningPointActive;
    uint8_t panningActive;
    uint8_t qSnapActive;
    uint8_t qSnapToggle;
} ViewData;

typedef struct State_ State;

typedef struct FunctionPtr_ {
    char name[100];
    int (*function)(State *state);
} FunctionPtr;

/*
 * This state variable encapuslates the command line's "scripting environment"
 * and also all of the flags that control behaviour outside of that.
 *
 * TODO: include the settings variables as elements in the state.
 *
 * TODO: move over remaining global data.
 */
struct State_ {
    char name[100];
    char version[100];
    char usage_msg[1000];

    uint8_t debug;
    uint8_t play_mode;
    uint8_t shift;
    uint64_t numOfDocs;
    uint64_t docIndex;

    char command_line[MAX_ARGUMENTS*MAX_ARG_LENGTH];
    char command[100];
    char arguments[MAX_ARGUMENTS][MAX_ARG_LENGTH];
    int argument_count;
    uint8_t first_run;
    EmbVector mouse_position;
    EmbVector point1;
    EmbVector point2;

    FunctionPtr command_list[MAX_COMMANDS];
    FunctionPtr design_list[MAX_COMMANDS];
    FunctionPtr fill_list[MAX_COMMANDS];
    FunctionPtr generator_list[MAX_COMMANDS];

    uint8_t testing;
    uint64_t test_script_pos;

    uint64_t simulation_start;
    /* In millimeters per second, so note that 1000.0 mm/s = 1 m/s.
     * TODO: This needs to be a setting.
     */
    float machine_speed;
    float stitch_time;

    uint64_t context;
    uint64_t mode;
};

/* Utilities */
uint8_t willUnderflowInt32(int64_t a, int64_t b);
uint8_t willOverflowInt32(int64_t a, int64_t b);
int32_t roundToMultiple(bool roundUp, int32_t numToRound, int32_t multiple);
uint64_t current_time(void);

/* Debugging */
void debug(const char *msg, ...);
int report_state(State state);

/* Scripting */
void run_cmd(const char *line);
void no_arguments(State *state);
void load_data(void);
ViewData *view_data(int32_t index);
int get_index(FunctionPtr list[MAX_COMMANDS], char *entry);
EmbVector get_vector(State *state, int index);
const char *translate(const char *msg);
void prompt_output(const char *msg);

/*
 * Qt/C++ commands
 *
 * ----------------------------------------------------------------------------
 *
 * C++ commands have the suffix "_c": some of the the "_command" functions
 * are just wrappers for these, others perform parsing work.
 *
 * Note that they have C linkage so they can be called by the "_command"
 * functions which are all in C source code.
 *
 * FIXME: the path-like commands need to pass QPainterPath which can't be
 *        supplied with C linkage.
 *
 */

/* Dialogs: housed in commands.cpp */
int about_dialog(void);
int changelog_dialog(void);
int details_dialog(void);
int help_dialog(void);
int settings_dialog(void);
int tip_of_the_day_dialog(void);

/* Geometry C++ commands: housed in geometry.cpp */
int arc_create(EmbVector start, EmbVector mid, EmbVector end, int32_t rubber_mode);
int circle_create(EmbVector center, EmbReal radius, bool fill, int32_t rubber_mode);
int dim_leader_create(EmbVector start, EmbVector end, EmbReal legHeight, int32_t rubber_mode);
int ellipse_create(EmbVector center, EmbReal width, EmbReal height, EmbReal rot, bool fill,
    int32_t rubber_mode);
int horizontal_dim_create(EmbVector start, EmbVector end, EmbReal legHeight);
int image_create(const char *img, EmbReal x, EmbReal y, EmbReal w, EmbReal h, EmbReal rot);
int infinite_line_create(EmbVector point1, EmbVector point2, EmbReal rot);
int line_create(EmbVector start, EmbVector end, EmbReal rot, int32_t rubber_mode);
int path_create(void);
int polygon_create(void);
int polyline_create(void);
int point_create(EmbVector position);
int rounded_rectangle_create(void);
int ray_create(void);
int rectangle_create(void);
int regular_polygon_create(void);
int slot_create(void);
int text_multi_create(void);
int text_single_create(void);
int triangle_create(void);
int vertical_dim_create(void);

/* Rubber C++ commands: housed in rubber.cpp */
int add_rubber(void);
int allow_rubber(void);
int clear_rubber(void);
int rubber(void);
int spare_rubber(const char *obj_type);
int set_rubber_mode(void);
int set_rubber_point(const char *key_, double x, double y);
int set_rubber_text(const char *key_, const char *txt_);

/* Key callbacks */
void delete_pressed(void);
void escape_pressed(void);

/* Others */
int clear_selection_c(void);
int copy_c(void);
int cut_c(void);
int day_c(void);
int end_c(void);
int exit_program_c(void);
ScriptValue get_c(const char *key);
int icon_resize_c(int size);
int messagebox_c(const char *type, const char *title, const char *text);
int new_file_c(void);
int night_c(void);
int open_c(const char *filename);
int pan_c(const char *subcommand);
int paste_c(void);
int print_file_c(void);
int redo_c(void);
int repeat_c(void);
int save_c(void);
int save_as_c(void);
int script_c(const char *filename);
int select_all_c(void);
int set_c(const char *key, ScriptValue v);
int sleep_c(void);
int stub_testing_c(void);
int undo_c(void);
int whats_this_c(void);
int window_c(const char *subcommand);
int zoom_c(char *subcommand);

extern ScriptValue st[N_SETTINGS];
extern SettingsData settings_table[N_SETTINGS];

#ifdef __cplusplus
}
#endif

#endif

