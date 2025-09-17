/* Embroidermodder 2
 * Copyright 2011-2025 The Embroidermodder Team
 *
 * Embroidermodder 2 is free and open software under the zlib license:
 * see LICENSE.md for details.
 *
 * ---------------------------------------------------------------------------
 *
 * The code in this directory is all C99 compliant.
 *
 * These macros are all for optimization of either lookup using explicit indicies
 * or tokens for switch tables.
 *
 * Note that they all use `#define` and are only whole number constants.
 * We prefer not to use `enum` instead keeping all of these constants within
 * an `int32_t` because we give an explicit value every time so we can
 * cross reference these in the configuration.
 */

#ifndef EM2_DATA__
#define EM2_DATA__

#ifdef __cplusplus
extern "C" {
#endif

#include <stdbool.h>

/* FIXME: this needs changing in libembroidery. */
#undef N_COMMANDS

/* Command IDs: for faster internal calls of CAD commands
 * (see the switch table in command.cpp).
 */
#define CMD_NULL                       0
#define CMD_ABOUT                      1
#define CMD_STUB                       2
#define CMD_CUT                        3
#define CMD_COPY                       4
#define CMD_PASTE                      5
#define CMD_SELECT_ALL                 6
#define CMD_DETAILS                    7
#define CMD_UPDATES                    8
#define CMD_WHATS_THIS                 9
#define CMD_PRINT                     10
#define CMD_HELP                      11
#define CMD_CHANGELOG                 12
#define CMD_UNDO                      13
#define CMD_REDO                      14
#define CMD_REPEAT                    15
#define CMD_ICON16                    16
#define CMD_ICON24                    17
#define CMD_ICON32                    18
#define CMD_ICON48                    19
#define CMD_ICON64                    20
#define CMD_ICON128                   21
#define CMD_PLAY                      22
#define CMD_SLEEP                     23
#define CMD_NEW                       24
#define CMD_OPEN                      25
#define CMD_SAVE                      26
#define CMD_SAVE_AS                   27
#define CMD_PAN_LEFT                  28
#define CMD_PAN_DOWN                  29
#define CMD_PAN_RIGHT                 30
#define CMD_PAN_UP                    31
#define CMD_PAN_POINT                 32
#define CMD_PAN_REAL_TIME             33
#define CMD_WINDOW_CASCADE            34
#define CMD_WINDOW_CLOSE_ALL          35
#define CMD_WINDOW_CLOSE              36
#define CMD_WINDOW_NEXT               37
#define CMD_WINDOW_PREVIOUS           38
#define CMD_WINDOW_TILE               39
#define CMD_ZOOM_ALL                  40
#define CMD_ZOOM_CENTER               41
#define CMD_ZOOM_DYNAMIC              42
#define CMD_ZOOM_EXTENTS              43
#define CMD_ZOOM_IN                   44
#define CMD_ZOOM_OUT                  45
#define CMD_ZOOM_PREVIOUS             46
#define CMD_ZOOM_REAL_TIME            47
#define CMD_ZOOM_SCALE                48
#define CMD_ZOOM_SELECTED             49
#define CMD_ZOOM_WINDOW               50
#define N_COMMANDS                    51
#define MAX_COMMANDS                 100

/* Contexts for Lua calls */
#define CONTEXT_MAIN                   0
#define CONTEXT_CLICK                  1
#define CONTEXT_CONTEXT                2
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

typedef struct SettingsData_ {
    char *section;
    char *key;
    char *default_value;
    char type;
    bool enabled;
    char *description;
    char *icon;
    double lower;
    double upper;
    double single_step;
} SettingsData;

extern bool testing;
extern int test_script_pos;
extern char test_script[200][200];

extern const char *command_names[MAX_COMMANDS];

extern const SettingsData settings_table[N_SETTINGS];

#ifdef __cplusplus
}
#endif

#endif

