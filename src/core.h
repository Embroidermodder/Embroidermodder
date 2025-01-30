/*
 * Embroidermodder 2.
 *
 * Copyright 2011-2025 The Embroidermodder Team
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

#include <inttypes.h>
#include <stdlib.h>
#include <stdbool.h>
#include <stdarg.h>

#include "embroidery.h"

/*
 * Integer Constants
 */
#define MAX_ARGS                      10
#define MAX_FILES                     30
#define MAX_STRING_LENGTH            200
#define MAX_COMMANDS                 300
#define MAX_COMMAND_LENGTH            30
#define MAX_MENU_LENGTH               30
#define MAX_TOOLBAR_LENGTH            30
#define MAX_ALIASES                  500
#define MAX_STATE_VARIABLES          500
#define MAX_STATE_STRING_LENGTH      200
#define MAX_LABEL_LENGTH              50
#define MAX_UNDO                     100
#define MAX_ACTIONS                 1000
#define MAX_OBJECTS                20000
#define MAX_WIDGETS                 1000
#define MAX_EDITORS                 1000
#define MAX_LONG_STRING             1000
#define MAX_TABLE_LENGTH             500
#define MAX_OPEN_FILES               100

#define NUMBINS                       10

#define WIDGET_COMBOBOX                0
#define WIDGET_LINEEDIT                1
#define WIDGET_GROUP_BOX               2
#define WIDGET_CHECKBOX                3
#define WIDGET_SPINBOX                 4
#define WIDGET_LABEL                   5
#define WIDGET_UNKNOWN                 6

#define SCRIPT_NULL                    0
#define SCRIPT_INT                     1
#define SCRIPT_REAL                    2
#define SCRIPT_COMMAND                 3
#define SCRIPT_STRING                  4
#define SCRIPT_BOOL                    5
#define SCRIPT_TABLE                   6

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

/* Geometry */
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

/* Simulation controls */
#define ACTION_SIMULATE              119
#define ACTION_PLAY                  120
#define ACTION_STOP                  121
#define ACTION_PAUSE                 122
#define ACTION_FAST_FORWARD          123
#define ACTION_REWIND                124
#define ACTION_EXPORT_VIDEO          125

/* Generators */
#define ACTION_QR                    126
#define ACTION_LETTERING             127
#define ACTION_PATTERN               128
#define ACTION_DESIGN                129

#define N_ACTIONS                    130

/* UI Modes */
#define CIRCLE_1P_RAD_ONE              0
#define CIRCLE_1P_RAD_TWO              1
#define CIRCLE_1P_DIA_ONE              2
#define CIRCLE_1P_DIA_TWO              3
#define CIRCLE_2P                      4
#define CIRCLE_3P                      5
#define CIRCLE_TTR                     6
#define CIRCLE_TTT                     7

#define ELLIPSE_MAJDIA_MINRAD_ONE      0
#define ELLIPSE_MAJDIA_MINRAD_TWO      1
#define ELLIPSE_MAJDIA_MINRAD_THREE    2
#define ELLIPSE_MAJRAD_MINRAD_ONE      3
#define ELLIPSE_MAJRAD_MINRAD_TWO      4
#define ELLIPSE_MAJRAD_MINRAD_THREE    5
#define ELLIPSE_ROTATION               6
#define ELLIPSE_NEUTRAL                7

#define POLYGON_NUM_SIDES              0
#define POLYGON_CENTER_PT              1
#define POLYGON_POLYTYPE               2
#define POLYGON_INSCRIBE               3
#define POLYGON_CIRCUMSCRIBE           4
#define POLYGON_DISTANCE               5
#define POLYGON_SIDE_LEN               6

#define TEXTSINGLE_JUSTIFY             0
#define TEXTSINGLE_SETFONT             1
#define TEXTSINGLE_SETGEOM             2
#define TEXTSINGLE_RAPID               3

/* NOTE: Allow this enum to evaluate false */
#define ARROW_STYLE_NO_ARROW           0
#define ARROW_STYLE_OPEN               1
#define ARROW_STYLE_CLOSED             2
#define ARROW_STYLE_DOT                3
#define ARROW_STYLE_BOX                4
#define ARROW_STYLE_TICK               5

/* NOTE: Allow this enum to evaluate false */
#define LINE_STYLE_NO_LINE             0
#define LINE_STYLE_FLARED              1
#define LINE_STYLE_FLETCHING           2

#define RGB_BACKGROUND                 0
#define RGB_CROSSHAIR                  1
#define RGB_GRID                       2

#define ROTATE_NORMAL                  0
#define ROTATE_REFERENCE               1

#define SCALE_NORMAL                   0
#define SCALE_REFERENCE                1

#define HEART_NUM_POINTS               0
#define HEART_STYLE                    1
#define HEART_XSCALE                   2
#define HEART_YSCALE                   3

#define HEART_STYLE_4                  0
#define HEART_STYLE_5                  1

#define STAR_NUM_POINTS                0
#define STAR_CENTER_PT                 1
#define STAR_RAD_OUTER                 2
#define STAR_RAD_INNER                 3

#define DOLPHIN_NUM_POINTS             0
#define DOLPHIN_XSCALE                 1
#define DOLPHIN_YSCALE                 2

#define SNOWFLAKE_NUM_POINTS           0
#define SNOWFLAKE_XSCALE               1
#define SNOWFLAKE_YSCALE               2

/* Custom Data used in QGraphicsItems
 *
 *                    (     int, const QVariant)
 * I.E. object.setData(OBJ_TYPE, OBJ_LINE);
 * I.E. object.setData(OBJ_LAYER, "OUTLINE");
 * I.E. object.setData(OBJ_COLOR, 123);
 * I.E. object.setData(OBJ_LTYPE, OBJ_LTYPE_CONT);
 */

/* Keys */
/* value type - int: See OBJ_VALUES */
// #define OBJ_TYPE                       0
/* value type - str: See OBJ_NAME_VALUES */
//#define OBJ_NAME                       1
/* value type - str: "USER", "DEFINED", "STRINGS" etc... */
// #define OBJ_LAYER                      2
/* value type - int: 0-255 */
/* TODO: Use color chart in formats/format-dxf.h for this */
// #define OBJ_COLOR                      3
/* value type - int: See OBJ_LTYPE_VALUES */
// #define OBJ_LTYPE                      4
/* value type - int: 0-27 */
// #define OBJ_LWT                        5
/* value type - int: See RUBBER_VALUES */
// #define OBJ_RUBBER                     6

/* Values
 * NOTE: Allow this enum to evaluate false
 * NOTE: Values >= 65536 ensure compatibility with qgraphicsitem_cast()
 */
#define OBJ_NULL                       0
#define OBJ_BASE                  100000
#define OBJ_ARC                   100001
#define OBJ_BLOCK                 100002
#define OBJ_CIRCLE                100003
#define OBJ_DIMALIGNED            100004
#define OBJ_DIMANGULAR            100005
#define OBJ_DIMARCLENGTH          100006
#define OBJ_DIMDIAMETER           100007
#define OBJ_DIMLEADER             100008
#define OBJ_DIMLINEAR             100009
#define OBJ_DIMORDINATE           100010
#define OBJ_DIMRADIUS             100011
#define OBJ_ELLIPSE               100012
#define OBJ_ELLIPSEARC            100013
#define OBJ_RUBBER                100014
#define OBJ_GRID                  100015
#define OBJ_HATCH                 100016
#define OBJ_IMAGE                 100017
#define OBJ_INFINITELINE          100018
#define OBJ_LINE                  100019
#define OBJ_PATH                  100020
#define OBJ_POINT                 100021
#define OBJ_POLYGON               100022
#define OBJ_POLYLINE              100023
#define OBJ_RAY                   100024
#define OBJ_RECTANGLE             100025
#define OBJ_SLOT                  100026
#define OBJ_SPLINE                100027
#define OBJ_TEXTMULTI             100028
#define OBJ_TEXTSINGLE            100029
#define OBJ_UNKNOWN               100030

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
#define RUBBER_OFF                     0
#define RUBBER_ON                      1

#define RUBBER_CIRCLE_1P_RAD           2
#define RUBBER_CIRCLE_1P_DIA           3
#define RUBBER_CIRCLE_2P               4
#define RUBBER_CIRCLE_3P               5
#define RUBBER_CIRCLE_TTR              6
#define RUBBER_CIRCLE_TTT              7

#define RUBBER_DIMLEADER_LINE          8

#define RUBBER_ELLIPSE_LINE            9
#define RUBBER_ELLIPSE_MAJDIA_MINRAD  10
#define RUBBER_ELLIPSE_MAJRAD_MINRAD  11
#define RUBBER_ELLIPSE_ROTATION       12

#define RUBBER_GRIP                   13

#define RUBBER_LINE                   14

#define RUBBER_POLYGON                15
#define RUBBER_POLYGON_INSCRIBE       16
#define RUBBER_POLYGON_CIRCUMSCRIBE   17

#define RUBBER_POLYLINE               18

#define RUBBER_IMAGE                  19

#define RUBBER_RECTANGLE              20

#define RUBBER_TEXTSINGLE             21

/* Statusbar Buttons */
#define SB_SNAP                        0
#define SB_GRID                        1
#define SB_RULER                       2
#define SB_ORTHO                       3
#define SB_POLAR                       4
#define SB_QSNAP                       5
#define SB_QTRACK                      6
#define SB_LWT                         7
#define N_SB_BUTTONS                   8

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
#define PREVIEW_NULL              0
#define PREVIEW_MOVE              1
#define PREVIEW_ROTATE            2
#define PREVIEW_SCALE             3

/* Used for general int tables. */
#define TERMINATOR_SYMBOL             -1

/* Menus: matches the order of menu_list. */
#define MENU_FILE                      0
#define MENU_EDIT                      1
#define MENU_VIEW                      2
#define MENU_DRAW                      3
#define MENU_TOOLS                     4
#define MENU_MODIFY                    5
#define MENU_SANDBOX                   6
#define MENU_DIMENSION                 7
#define MENU_WINDOW                    8
#define MENU_HELP                      9
#define MENU_RECENT                   10
#define MENU_ZOOM                     11
#define MENU_PAN                      12
#define N_MENUS                       13

/* Toolbars: matches the order of toolbar_list. */
#define TOOLBAR_FILE                   0
#define TOOLBAR_EDIT                   1
#define TOOLBAR_VIEW                   2
#define TOOLBAR_ICON                   3
#define TOOLBAR_HELP                   4
#define TOOLBAR_LAYER                  5
#define TOOLBAR_PROPERTIES             6
#define TOOLBAR_TEXT                   7
#define TOOLBAR_PROMPT                 8
#define TOOLBAR_DRAW                   9
#define TOOLBAR_MODIFY                10
#define TOOLBAR_INQUIRY               11
#define TOOLBAR_DIMENSION             12
#define TOOLBAR_SANDBOX               13
#define N_TOOLBARS                    14
#define TOOLBAR_BREAK                 -2

/* Settings */
#define GENERAL_LANGUAGE               0
#define GENERAL_ICON_THEME             1
#define GENERAL_ICON_SIZE              2
#define GENERAL_MDI_BG_USE_LOGO        3
#define GENERAL_MDI_BG_USE_TEXTURE     4
#define GENERAL_MDI_BG_USE_COLOR       5
#define GENERAL_MDI_BG_LOGO            6
#define GENERAL_MDI_BG_TEXTURE         7
#define GENERAL_MDI_BG_COLOR           8
#define GENERAL_TIP_OF_THE_DAY         9
#define GENERAL_CURRENT_TIP           10
#define GENERAL_SYSTEM_HELP_BROWSER   11
#define GENERAL_CHECK_FOR_UPDATES     12
#define DISPLAY_USE_OPENGL            13
#define DISPLAY_RENDERHINT_AA         14
#define DISPLAY_RENDERHINT_TEXT_AA    15
#define DISPLAY_RENDERHINT_SMOOTH_PIX 16
#define DISPLAY_RENDERHINT_HIGH_AA    17
#define DISPLAY_RENDERHINT_NONCOSMETIC 18
#define DISPLAY_SHOW_SCROLLBARS       19
#define DISPLAY_SCROLLBAR_WIDGET_NUM  20
#define DISPLAY_CROSSHAIR_COLOR       21
#define DISPLAY_BG_COLOR              22
#define DISPLAY_SELECTBOX_LEFT_COLOR  23
#define DISPLAY_SELECTBOX_LEFT_FILL   24
#define DISPLAY_SELECTBOX_RIGHT_COLOR 25
#define DISPLAY_SELECTBOX_RIGHT_FILL  26
#define DISPLAY_SELECTBOX_ALPHA       27
#define DISPLAY_ZOOMSCALE_IN          28
#define DISPLAY_ZOOMSCALE_OUT         29
#define DISPLAY_CROSSHAIR_PERCENT     30
#define DISPLAY_UNITS                 31
#define PROMPT_TEXT_COLOR             32
#define PROMPT_BG_COLOR               33
#define PROMPT_FONT_FAMILY            34
#define PROMPT_FONT_STYLE             35
#define PROMPT_FONT_SIZE              36
#define PROMPT_SAVE_HISTORY           37
#define PROMPT_SAVE_HISTORY_AS_HTML   38
#define PROMPT_SAVE_HISTORY_FILENAME  39
#define OPENSAVE_CUSTOM_FILTER        40
#define OPENSAVE_OPEN_FORMAT          41
#define OPENSAVE_OPEN_THUMBNAIL       42
#define OPENSAVE_SAVE_FORMAT          43
#define OPENSAVE_SAVE_THUMBNAIL       44
#define OPENSAVE_RECENT_MAX_FILES     45
#define OPENSAVE_RECENT_LIST_OF_FILES 46
#define OPENSAVE_RECENT_DIRECTORY     47
#define OPENSAVE_TRIM_DST_NUM_JUMPS   48
#define PRINTING_DEFAULT_DEVICE       49
#define PRINTING_USE_LAST_DEVICE      50
#define PRINTING_DISABLE_BG           51
#define GRID_SHOW_ON_LOAD             52
#define GRID_SHOW_ORIGIN              53
#define GRID_COLOR_MATCH_CROSSHAIR    54
#define GRID_COLOR                    55
#define GRID_LOAD_FROM_FILE           56
#define GRID_TYPE                     57
#define GRID_CENTER_ON_ORIGIN         58
#define GRID_CENTER_X                 59
#define GRID_CENTER_Y                 60
#define GRID_SIZE_X                   61
#define GRID_SIZE_Y                   62
#define GRID_SPACING_X                63
#define GRID_SPACING_Y                64
#define GRID_SIZE_RADIUS              65
#define GRID_SPACING_RADIUS           66
#define GRID_SPACING_ANGLE            67
#define RULER_SHOW_ON_LOAD            68
#define RULER_METRIC                  69
#define RULER_COLOR                   70
#define RULER_PIXEL_SIZE              71
#define QSNAP_ENABLED                 72
#define QSNAP_LOCATOR_COLOR           73
#define QSNAP_LOCATOR_SIZE            74
#define QSNAP_APERTURE_SIZE           75
#define QSNAP_ENDPOINT                76
#define QSNAP_MIDPOINT                77
#define QSNAP_CENTER                  78
#define QSNAP_NODE                    79
#define QSNAP_QUADRANT                80
#define QSNAP_INTERSECTION            81
#define QSNAP_EXTENSION               82
#define QSNAP_INSERTION               83
#define QSNAP_PERPENDICULAR           84
#define QSNAP_TANGENT                 85
#define QSNAP_NEAREST                 86
#define QSNAP_APPARENT                87
#define QSNAP_PARALLEL                88
#define LWT_SHOW_LWT                  89
#define LWT_REAL_RENDER               90
#define LWT_DEFAULT_LWT               91
#define SELECTION_MODE_PICKFIRST      92
#define SELECTION_MODE_PICKADD        93
#define SELECTION_MODE_PICKDRAG       94
#define SELECTION_COOLGRIP_COLOR      95
#define SELECTION_HOTGRIP_COLOR       96
#define SELECTION_GRIP_SIZE           97
#define SELECTION_PICKBOX_SIZE        98
#define TEXT_FONT                     99
#define TEXT_SIZE                    100
#define TEXT_ANGLE                   101
#define TEXT_STYLE_BOLD              102
#define TEXT_STYLE_ITALIC            103
#define TEXT_STYLE_UNDERLINE         104
#define TEXT_STYLE_OVERLINE          105
#define TEXT_STYLE_STRIKEOUT         106
#define N_SETTINGS                   107

/* Settings memory */
#define SETTING_SETTING                0
#define SETTING_DIALOG                 1
#define SETTING_ACCEPT                 2
#define SETTING_PREVIEW                3

/* Justification */
#define JUSTIFY_LEFT                   0
#define JUSTIFY_CENTER                 1
#define JUSTIFY_RIGHT                  2
#define JUSTIFY_ALIGNED                3
#define JUSTIFY_MIDDLE                 4
#define JUSTIFY_FIT                    5
#define JUSTIFY_TOP_LEFT               6
#define JUSTIFY_TOP_CENTER             7
#define JUSTIFY_TOP_RIGHT              8
#define JUSTIFY_MIDDLE_LEFT            9
#define JUSTIFY_MIDDLE_CENTER         10
#define JUSTIFY_MIDDLE_RIGHT          11
#define JUSTIFY_BOTTOM_LEFT           12
#define JUSTIFY_BOTTOM_CENTER         13
#define JUSTIFY_BOTTOM_RIGHT          14
#define N_JUSTIFY                     15

/* Object Properties */
#define PROP_ARC_CENTER_X              0
#define PROP_ARC_CENTER_Y              1
#define PROP_ARC_RADIUS                2
#define N_PROPS                        3

/* View Properties */
#define VIEW_SNAP                      0
#define VIEW_GRID                      1
#define VIEW_ORTHO                     2
#define VIEW_POLAR                     3
#define VIEW_QSNAP                     4
#define VIEW_QTRACK                    5
#define VIEW_LWT                       6
#define VIEW_REAL                      7
#define VIEW_RULER                     8
#define VIEW_METRIC                    9
#define VIEW_GRIPPING                 10
#define VIEW_RAPID_MOVING             11
#define VIEW_PREVIEWING               12
#define VIEW_PASTING                  13
#define VIEW_MOVING                   14
#define VIEW_SELECTING                15
#define VIEW_ZOOMING                  16
#define VIEW_PANNING_RT               17
#define VIEW_PANNING_POINT            18
#define VIEW_PANNING                  19
#define N_VIEW_PROPS                  20

/* Translations */
#define UNFINISHED                     0
#define DRAFT                          1
#define FINISHED                       2

#define ARABIC_SHORTCODE            "ar"
#define CHINESE_SHORTCODE           "zh"
#define ENGLISH_SHORTCODE           "en"
#define FRENCH_SHORTCODE            "fr"
#define RUSSIAN_SHORTCODE           "ru"
#define SPANISH_SHORTCODE           "es"

#define END_SYMBOL                 "END"

#define GB_GENERAL                     0
#define GB_GEOM_ARC                    1
#define GB_MISC_ARC                    2
#define GB_GEOM_BLOCK                  3
#define GB_GEOM_CIRCLE                 4
#define GB_GEOM_DIMALIGNED             5
#define GB_GEOM_DIMANGULAR             6
#define GB_GEOM_DIMARCLENGTH           7
#define GB_GEOM_DIMDIAMETER            8
#define GB_GEOM_DIMLEADER              9
#define GB_GEOM_DIMLINEAR             10
#define GB_GEOM_DIMORDINATE           11
#define GB_GEOM_DIMRADIUS             12
#define GB_GEOM_ELLIPSE               13
#define GB_GEOM_IMAGE                 14
#define GB_MISC_IMAGE                 15
#define GB_GEOM_INFINITE_LINE         16
#define GB_GEOM_LINE                  17
#define GB_GEOM_PATH                  18
#define GB_MISC_PATH                  19
#define GB_GEOM_POINT                 20
#define GB_GEOM_POLYGON               21
#define GB_GEOM_POLYLINE              22
#define GB_MISC_POLYLINE              23
#define GB_GEOM_RAY                   24
#define GB_GEOM_RECT                  25
#define GB_GEOM_TEXT_MULTI            26
#define GB_TEXT_TEXT_SINGLE           27
#define GB_GEOM_TEXT_SINGLE           28
#define GB_MISC_TEXT_SINGLE           29
#define N_GROUPBOX                    30

#define REAL(arg) context->argument[arg].r
#define INT(arg)  context->argument[arg].i
#define STR(arg)  context->argument[arg].s
#define BOOL(arg) context->argument[arg].b

#define DIALOG_INT_SLOT(A) \
    SLOT([=](int value) { setting[A].dialog.i = value; } )
#define DIALOG_REAL_SLOT(A) \
    SLOT([=](EmbReal value) { setting[A].dialog.r = value; } )
#define DIALOG_STRING_SLOT(A) \
    SLOT([=](QString value) { strncpy(setting[A].dialog.s, qPrintable(value), MAX_STRING_LENGTH); } )

#define set_int(key, value)                setting[key].setting.i = value
#define set_real(key, value)               setting[key].setting.r = value
#define set_str(key, value) strncpy(setting[key].setting.s, value, MAX_STRING_LENGTH)
#define set_bool(key, value)               setting[key].setting.b = value

#define get_int(key)       setting[key].setting.i
#define get_str(key)       setting[key].setting.s
#define get_real(key)      setting[key].setting.r
#define get_bool(key)      setting[key].setting.b

/* For switch tables we can use this trick to use two character indices.
 * For example: "case TWO_CHAR_INDEX(DUTCH_SHORTCODE):".
 */
#define TWO_CHAR_INDEX(A)             (0x100*A[0] + A[1])

typedef ScriptValue (*Command)(ScriptEnv *context);

typedef struct CommandData_ {
    char *command;
    char *arguments;
    char *icon;
    char *tooltip;
    char *statustip;
    char *alias;
    char *shortcut;
    int32_t flags;
    Command action;
} CommandData;

#define MAX_LAYERS 20

typedef struct LayerData_ {
    char name[200];
    bool visible;
    bool frozen;
    EmbReal zValue;
    int32_t color;
    char *lineType;
    char *lineWeight;
    bool print;
} LayerData;

typedef struct Setting_ {
    char key[MAX_STRING_LENGTH];
    ScriptValue setting;
    ScriptValue dialog;
    ScriptValue accept;
    ScriptValue preview;
} Setting;

typedef struct GroupBoxData_ {
    int32_t id;
    char *key;
    char *label;
    int rows;
    char *data[100];
    int object;
} GroupBoxData;

typedef struct ObjectCore_ {
    int32_t doc;

    EmbGeometry *geometry;

    LabelledVector *rubber_points;
    int n_rubber_points;
    StringMap *rubber_texts;
    int n_rubber_texts;

    EmbVector position;

    EmbReal scale;
    EmbReal rotation;

    uint32_t rgb;

    char OBJ_NAME[MAX_STRING_LENGTH];

    char text[MAX_STRING_LENGTH];
    char textFont[MAX_STRING_LENGTH];
    char textJustify[MAX_STRING_LENGTH];
    EmbReal textSize;
    bool textBold;
    bool textItalic;
    bool textUnderline;
    bool textStrikeOut;
    bool textOverline;
    bool textBackward;
    bool textUpsideDown;

    int rubber_mode;
    uint32_t objID;

    bool curved;
    bool filled;
    EmbReal arrowStyleAngle;
    EmbReal arrowStyleLength;
    EmbReal lineStyleAngle;
    EmbReal lineStyleLength;

    int gripIndex;
} ObjectCore;

typedef struct UndoData_ {
    char label[MAX_STRING_LENGTH];
    int type;
    int32_t doc;
    EmbVector delta;
    EmbVector pivot;
    EmbReal angle;
    EmbReal factor;
    char navType[MAX_STRING_LENGTH];
    EmbVector fromCenter;
    EmbVector toCenter;
    bool done;
    EmbVector before;
    EmbVector after;
    EmbLine mirrorLine;
    uint32_t obj;
} UndoData;

/* . */
typedef struct DocumentData_ {
    EmbPattern *pattern;

    UndoData undo_stack[MAX_UNDO];

    int32_t id;

    EmbIdList *selectedItems;
    EmbIdList *rubberRoomList;
    EmbIdList *previewObjectList;
    EmbIdList *spareRubberList;
    EmbIdList *cutCopyObjectList;
    EmbIdList *previewObjectItemGroup;
    EmbIdList *hashDeletedObjects;
    EmbIdList *pasteObjectItemGroup;

    EmbVectorList *selectedGripPoints;

    /* UI State */
    bool qSnapActive;
    bool qSnapToggle;

    EmbVector previewPoint;
    EmbReal previewData;
    int previewMode;

    EmbVector viewMousePoint;
    EmbVector sceneMousePoint;
    EmbVector sceneQSnapPoint;

    uint8_t qsnapLocatorSize;
    uint32_t qsnapLocatorColor;
    uint8_t qsnapApertureSize;

    uint32_t gripColorCool;
    uint32_t gripColorHot;
    uint32_t gridColor;
    uint8_t gripSize;
    uint8_t pickBoxSize;

    uint32_t crosshairColor;
    uint32_t crosshairSize;

    EmbVector scenePressPoint;
    EmbVector pressPoint;
    EmbVector sceneMovePoint;
    EmbVector movePoint;
    EmbVector sceneReleasePoint;
    EmbVector releasePoint;
    EmbVector sceneGripPoint;

    EmbVector cutCopyMousePoint;
    EmbVector pasteDelta;

    int panDistance;
    int panStartX;
    int panStartY;

    bool properties[N_VIEW_PROPS];

    uint32_t rulerColor;
    bool rulerMetric;
    uint8_t rulerPixelSize;

    uint32_t backgroundColor;

    bool fileWasLoaded;
    int myIndex;

    char curFile[MAX_STRING_LENGTH];
    char curLayer[MAX_STRING_LENGTH];
    uint32_t curColor;
    char curLineType[MAX_STRING_LENGTH];
    char curLineWeight[MAX_STRING_LENGTH];
} DocumentData;

/* For dialogs and tabs */
typedef struct WidgetDesc_ {
    int id;
    char *label;
    char *key;
    char *icon;
    char *value;
    char *editor_data;
    int type;
    int n_leaves;
} WidgetDesc;

typedef struct Button_ {
    EmbRect rect;
    EmbColor color;
    EmbColor highlight_color;
    char *text;
    char *font;
    EmbColor text_color;
    int state;
} Button;

typedef struct Tab_ {
    int state;
} Tab;

typedef struct Translation_ {
    char *ar;
    char *en;
    char *es;
    char *fr;
    char *ru;
    char *zh;
    int32_t status;
} Translation;

typedef struct WidgetData_ {
    char *key;
    char *label;
    int id;
    int type;
} WidgetData;

typedef struct Design_ {
    char *command;
    float lower;
    float upper;
    int num_points;
    int min_points;
    int max_points;
    float xscale;
    float yscale;
    char *parameter;
    char *x;
    char *y;
} Design;

typedef struct ViewData_ {
    int32_t doc;
    double little;
    double medium;
    EmbLine *lines;
    int n_lines;
    EmbGeometry *geometry;
    int n_geometry;
    int width;
    int height;
    int unit;
    bool feet;
    double fraction;
    int xStart;
    int yStart;
} ViewData;

/* -------------------------------- Scripting ---------------------------- */

ScriptEnv *create_script_env(void);
void free_script_env(ScriptEnv *);
ScriptEnv *pack(ScriptEnv *context, const char *fmt, ...);
ScriptValue call(ScriptEnv *context, char *function, ...);
ScriptValue run_cmd(ScriptEnv *context, const char *line);

ScriptValue script_bool(bool b);
ScriptValue script_int(int i);
ScriptValue script_real(EmbReal r);
ScriptValue script_string(char *s);
ScriptValue command_prompt(ScriptEnv *context, const char *line);

EmbVector find_mouse_snap_point(EmbVectorList *snap_points, EmbVector mouse_point);

ViewData *create_view_data(int32_t doc, int width, int height, int unit);
void free_view_data(ViewData *vdata);
int create_ruler_lines(ViewData *data);

bool string_equal(char *a, const char *b);
void string_copy(char *dst, const char *src);
int find_in_map(StringMap *hash, int length, const char *key);
int table_length(char *table[]);

EmbVectorList *create_vector_list(void);
void append_vector_to_list(EmbVectorList *, EmbVector);
void remove_vector_from_list(EmbVectorList *, int32_t);
void free_vector_list(EmbVectorList *);

EmbIdList *create_id_list(void);
void append_id_to_list(EmbIdList *list, int32_t);
void remove_id_from_list(EmbIdList *list, int32_t);
bool id_list_contains(EmbIdList *list, int32_t);
void copy_object_list(EmbIdList *dst, EmbIdList *src);
void free_id_list(EmbIdList *);

ScriptValue *setting_ptr(int key, int mode);
void copy_setting(int key, int dst, int src);

char *load_file(char *fname);

void prompt_output(const char *);
int argument_checks(ScriptEnv *context, int id);
char *translate(const char *msg);

bool pattern_save(EmbPattern *pattern, char *fileName);

int parse_floats(const char *line, float result[], int n);
int parse_vector(const char *line, EmbVector *v);
bool valid_rgb(float r, float g, float b);
void report_distance(EmbVector a, EmbVector b);

void add_to_menu(int id, char *menu_data[]);
void add_to_toolbar(int id, char *toolbar_data[]);

int load_data(void);

int load_settings(const char *appDir, const char *configDir);
int save_settings(char *appDir, char *configDir);

int get_command_id(char *);

EmbArc emb_arc_set_radius(EmbArc a, EmbReal radius);

bool int32_underflow(int64_t a, int64_t b);
bool int32_overflow(int64_t a, int64_t b);
int round_to_multiple(bool roundUp, int numToRound, int multiple);

void messagebox(const char* logo, const char *title, const char *text);

const char *get_current_layer(void);
uint32_t get_current_color(void);
const char *get_current_line_type(void);
const char *get_current_line_weight(void);

void statusbar_toggle(const char *key, bool on);
void zoom_extents_all_sub_windows(void);
bool loadFile(const char *fileName);

int32_t free_glfw(void);

int glfw_application(int argc, char *argv[]);

int find_int_map(IntMap *, int);

char *file_extension(char *fileName);

/* ------------------------------ Widgets ------------------------------- */

void set_visibility(const char *key, bool visibility);
void set_enabled(const char *key, bool visibility);
void set_visibility_group(char *key[], bool visibility);
void set_enabled_group(char *key[], bool visibility);

/* ------------------------------ Prompt ------------------------------- */

void set_prompt_text_color(uint32_t color);
void set_prompt_background_color(uint32_t color);
void set_prompt_font_family(char *family);
void set_prompt_font_style(char *style);
void set_prompt_font_size(int size);

void prompt_history_appended(char *txt);
void log_prompt_input(char *txt);
void prompt_input_previous(void);
void prompt_input_next(void);
void prompt_update_style(void);
void prompt_end_command(void);
void prompt_set_current_text(const char *);
void process_input(char);

/* -------------------------- Main Functions --------------------------- */

void run_command_main(const char *cmd);
void run_command_click(const char *cmd, EmbReal x, EmbReal y);
void run_command_move(const char *cmd, EmbReal x, EmbReal y);
void run_command_context(const char *cmd, const char *str);
void run_command_prompt(const char *cmd);

uint32_t rgb(uint8_t r, uint8_t g, uint8_t b);

void update_all_view_scroll_bars(bool val);
void update_all_view_cross_hair_colors(uint32_t color);
void update_all_view_background_colors(uint32_t color);
void update_all_view_select_box_colors(uint32_t colorL, uint32_t fillL,
    uint32_t colorR, uint32_t fillR, int alpha);
void update_all_view_grid_colors(uint32_t color);
void update_all_view_ruler_colors(uint32_t color);

void update_pick_add_mode(bool val);
void pick_add_mode_toggled(void);

void set_text_font(char *str);
void set_text_size(EmbReal num);

void prompt_history_appended(char *txt);
void log_prompt_input(char *txt);

void open_file(bool recent, char *recentFile);
void open_selected_files(char *table[]);

void settings_dialog(const char *showTab);

bool valid_file_format(char *fileName);

void set_undo_clean_icon(bool opened);

void mdiarea_set_bg(uint32_t color);
void accept_interface_color(int32_t key, uint32_t color);
void preview_interface_color(int32_t key, uint32_t color);
void dialog_interface_color(int32_t key, uint32_t color);

void current_layer_changed(char *layer);
void current_linetype_changed(char *type);
void current_lineweight_changed(char *weight);

void prompt_input_previous(void);
void prompt_input_next(void);

void layer_selector_changed(int index);
void linetype_selector_changed(int index);
void lineweight_selector_changed(int index);
void text_size_selector_index_changed(int index);

void make_layer_active(void);
void layer_manager(void);
void layer_previous(void);

void delete_pressed(void);
void escape_pressed(void);

void icon_resize(int iconSize);

void read_settings(void);
void write_settings(void);

void create_all_actions(void);

void load_formats(void);

void settings_prompt(void);

void end_command(void);
void debug_message(const char *msg, ...);
void wait_cursor(void);
void arrow_cursor(void);
void restore_cursor(void);

void cut(void);

void about_dialog(void);

/* --------------------------------- Editors -------------------------------- */

void check_box_lwt_real_render_changed(int);
void combo_box_prompt_font_family_changed(char *);
void combo_box_prompt_font_style_changed(char *);
void spin_box_display_select_box_alpha_changed(int);
void spin_box_prompt_font_size_changed(int);

void checkBoxGridCenterOnOriginStateChanged(int);
void comboBoxGridTypeCurrentIndexChanged(const char *);
void checkBoxGridColorMatchCrossHairStateChanged(int);
void checkBoxGridLoadFromFileStateChanged(int);
void checkBoxLwtShowLwtStateChanged(int);

void accept_settings(void);

void useBackgroundLogo(bool use);
void useBackgroundTexture(bool use);
void useBackgroundColor(bool use);

void setBackgroundLogo(char *fileName);
void setBackgroundTexture(char *fileName);
void setBackgroundColor(uint32_t color);

void update_view(void);
void preview_update(void);

/* -------------------------------- Commands -------------------------------- */

void stub_testing(void);
void run_testing(void);
void check_for_updates(void);
void open_recent_file(void);
int save_as_file(void);
void update_interface(void);
void window_menu_about_to_show(void);
void start_blinking(void);
void stop_blinking(void);
void repeat_action(void);
void move_action(void);

void nanosleep_(int);

void button_tip_of_the_day_clicked(int button);

void set_mouse_coord(EmbReal x, EmbReal y);

void set_background_color(uint8_t r, uint8_t g, uint8_t b);
void set_cross_hair_color(uint8_t r, uint8_t g, uint8_t b);
void set_grid_color(uint8_t r, uint8_t g, uint8_t b);

void set_cursor_shape(char *shape);

void create_details_dialog(void);

void set_prompt_prefix(char *txt);
/* TODO: void set_RubberFilter(int64_t id); */
/* TODO: This is so more than 1 rubber object can exist at one time without updating all rubber objects at once. */
void set_rubber_mode(int mode);
void set_rubber_point(const char *key, EmbReal x, EmbReal y);
void set_rubber_text(const char *key, char *txt);

void toggle(int mode);

/* Help Menu */
void button_tip_of_the_day_clicked(int);
void check_box_tip_of_the_day_changed(int);
void whats_this_context_help(void);

int make_application(int argc, char* argv[]);

void set_object_rubber_mode(ObjectCore *core, int mode);

EmbVector unpack_vector(ScriptEnv *context, int offset);

void undoable_add(int32_t doc, uint32_t obj, char *label);
void undoable_delete(int32_t doc, uint32_t obj, char *label);
void undoable_scale(int32_t doc, uint32_t obj, EmbVector center, EmbReal factor, char *label);
void undoable_move(int32_t doc, uint32_t obj, EmbVector delta, char *msg);
void undoable_rotate(int32_t doc, uint32_t obj, EmbVector v, char *msg);
void undoable_mirror(int32_t doc, uint32_t obj, EmbVector start, EmbVector end,
    char *msg);

void update_all_background_color(uint32_t color);

void create_all_menus(void);
void create_all_toolbars(void);

void clear_all_fields(void);
void line_edit_clear(const char *key);
void combo_box_clear(const char *key);
void clear_font_combobox(void);

int get_id(char *table[], char *);

const char *platform_string(void);

void button_tip_of_the_day_clicked(int button);
void free_objects(EmbIdList *);

/* All of the pointers used by command_data.
 *
 * Note that these are not global scope functions because they are only needed
 * for this table.
 */
ScriptValue about_command(ScriptEnv *context);
ScriptValue add_arc_command(ScriptEnv *context);
ScriptValue add_circle_command(ScriptEnv *context);
ScriptValue add_dimleader_command(ScriptEnv *context);
ScriptValue add_ellipse_command(ScriptEnv *context);
ScriptValue add_horizontal_dimension_command(ScriptEnv *context);
ScriptValue add_image_command(ScriptEnv *context);
ScriptValue add_infinite_line_command(ScriptEnv *context);
ScriptValue add_line_command(ScriptEnv *context);
ScriptValue add_ray_command(ScriptEnv *context);
ScriptValue add_triangle_command(ScriptEnv *context);
ScriptValue add_rectangle_command(ScriptEnv *context);
ScriptValue add_rounded_rectangle_command(ScriptEnv *context);
ScriptValue add_slot_command(ScriptEnv *context);
ScriptValue add_point_command(ScriptEnv *context);
ScriptValue add_regular_polygon_command(ScriptEnv *context);
ScriptValue add_vertical_dimension_command(ScriptEnv *context);
ScriptValue add_textmulti_command(ScriptEnv *context);
ScriptValue add_textsingle_command(ScriptEnv *context);
ScriptValue alert_command(ScriptEnv *context);
ScriptValue angle_command(ScriptEnv *context);
ScriptValue changelog_command(ScriptEnv *context);
ScriptValue copy_command(ScriptEnv *context);
ScriptValue color_selector_command(ScriptEnv *context);
ScriptValue current_color_changed_command(ScriptEnv *context);
ScriptValue debug_command(ScriptEnv *context);
ScriptValue details_command(ScriptEnv *context);
ScriptValue disable_command(ScriptEnv *context);
ScriptValue do_nothing_command(ScriptEnv *context);
ScriptValue enable_command(ScriptEnv *context);
ScriptValue exit_command(ScriptEnv *context);
ScriptValue get_command(ScriptEnv *context);
ScriptValue help_command(ScriptEnv *context);
ScriptValue layer_previous_command(ScriptEnv *context);
ScriptValue make_layer_active_command(ScriptEnv *context);
ScriptValue move_command(ScriptEnv *context);
ScriptValue new_command(ScriptEnv *context);
ScriptValue open_command(ScriptEnv *context);
ScriptValue paste_command(ScriptEnv *context);
ScriptValue previewon_command(ScriptEnv *context);
ScriptValue print_command(ScriptEnv *context);
ScriptValue redo_command(ScriptEnv *context);
ScriptValue sandbox_command(ScriptEnv *context);
ScriptValue save_command(ScriptEnv *context);
ScriptValue save_as_command(ScriptEnv *context);
ScriptValue set_command(ScriptEnv *context);
ScriptValue tip_of_the_day_command(ScriptEnv *context);
ScriptValue undo_command(ScriptEnv *context);
ScriptValue window_cascade_command(ScriptEnv *context);
ScriptValue window_close_command(ScriptEnv *context);
ScriptValue window_close_all_command(ScriptEnv *context);
ScriptValue window_next_command(ScriptEnv *context);
ScriptValue window_previous_command(ScriptEnv *context);
ScriptValue window_tile_command(ScriptEnv *context);

ScriptValue cut_command(ScriptEnv *context);
ScriptValue copy_selected_command(ScriptEnv *context);
ScriptValue paste_selected_command(ScriptEnv *context);
ScriptValue whats_this_command(ScriptEnv *context);
ScriptValue icon16_command(ScriptEnv *context);
ScriptValue icon24_command(ScriptEnv *context);
ScriptValue icon32_command(ScriptEnv *context);
ScriptValue icon48_command(ScriptEnv *context);
ScriptValue icon64_command(ScriptEnv *context);
ScriptValue icon128_command(ScriptEnv *context);
ScriptValue settings_command(ScriptEnv *context);
ScriptValue layers_command(ScriptEnv *context);
ScriptValue layer_selector_command(ScriptEnv *context);
ScriptValue line_type_selector_command(ScriptEnv *context);
ScriptValue line_weight_selector_command(ScriptEnv *context);
ScriptValue hide_all_layers_command(ScriptEnv *context);
ScriptValue show_all_layers_command(ScriptEnv *context);
ScriptValue freeze_all_layers_command(ScriptEnv *context);
ScriptValue thaw_all_layers_command(ScriptEnv *context);
ScriptValue lock_all_layers_command(ScriptEnv *context);
ScriptValue unlock_all_layers_command(ScriptEnv *context);
ScriptValue text_bold_command(ScriptEnv *context);
ScriptValue text_italic_command(ScriptEnv *context);
ScriptValue text_underline_command(ScriptEnv *context);
ScriptValue text_strikeout_command(ScriptEnv *context);
ScriptValue text_overline_command(ScriptEnv *context);
ScriptValue zoom_real_time_command(ScriptEnv *context);
ScriptValue zoom_previous_command(ScriptEnv *context);
ScriptValue zoom_window_command(ScriptEnv *context);
ScriptValue zoom_dynamic_command(ScriptEnv *context);
ScriptValue zoom_scale_command(ScriptEnv *context);
ScriptValue zoom_center_command(ScriptEnv *context);
ScriptValue zoom_in_command(ScriptEnv *context);
ScriptValue zoom_out_command(ScriptEnv *context);
ScriptValue zoom_selected_command(ScriptEnv *context);
ScriptValue zoom_all_command(ScriptEnv *context);
ScriptValue zoom_extents_command(ScriptEnv *context);
ScriptValue panrealtime_command(ScriptEnv *context);
ScriptValue panpoint_command(ScriptEnv *context);
ScriptValue panleft_command(ScriptEnv *context);
ScriptValue panright_command(ScriptEnv *context);
ScriptValue panup_command(ScriptEnv *context);
ScriptValue pandown_command(ScriptEnv *context);
ScriptValue day_command(ScriptEnv *context);
ScriptValue night_command(ScriptEnv *context);
ScriptValue circle_command(ScriptEnv *context);
ScriptValue dimleader_command(ScriptEnv *context);
ScriptValue distance_command(ScriptEnv *context);
ScriptValue dolphin_command(ScriptEnv *context);
ScriptValue ellipse_command(ScriptEnv *context);
ScriptValue erase_command(ScriptEnv *context);
ScriptValue error_command(ScriptEnv *context);
ScriptValue heart_command(ScriptEnv *context);
ScriptValue line_command(ScriptEnv *context);
ScriptValue located_point_command(ScriptEnv *context);
ScriptValue mirror_selected_command(ScriptEnv *context);
ScriptValue move_selected_command(ScriptEnv *context);
ScriptValue path_command(ScriptEnv *context);
ScriptValue platform_command(ScriptEnv *context);
ScriptValue point_command(ScriptEnv *context);
ScriptValue polygon_command(ScriptEnv *context);
ScriptValue polyline_command(ScriptEnv *context);
ScriptValue previewoff_command(ScriptEnv *context);
ScriptValue quickleader_command(ScriptEnv *context);
ScriptValue rectangle_command(ScriptEnv *context);
ScriptValue rgb_command(ScriptEnv *context);
ScriptValue rotate_command(ScriptEnv *context);
ScriptValue scale_command(ScriptEnv *context);
ScriptValue scale_selected_command(ScriptEnv *context);
ScriptValue select_all_command(ScriptEnv *context);
ScriptValue single_line_text_command(ScriptEnv *context);
ScriptValue snowflake_command(ScriptEnv *context);
ScriptValue star_command(ScriptEnv *context);
ScriptValue syswindows_command(ScriptEnv *context);
ScriptValue todo_command(ScriptEnv *context);
ScriptValue vulcanize_command(ScriptEnv *context);
ScriptValue add_geometry_command(ScriptEnv *context);
ScriptValue delete_command(ScriptEnv *context);
ScriptValue grip_edit_command(ScriptEnv *context);
ScriptValue nav_command(ScriptEnv *context);
ScriptValue mirror_command(ScriptEnv *context);
ScriptValue test_command(ScriptEnv *context);
ScriptValue simulate_command(ScriptEnv *context);
ScriptValue play_command(ScriptEnv *context);
ScriptValue stop_command(ScriptEnv *context);
ScriptValue pause_command(ScriptEnv *context);
ScriptValue fast_forward_command(ScriptEnv *context);
ScriptValue export_command(ScriptEnv *context);
ScriptValue qr_command(ScriptEnv *context);
ScriptValue lettering_command(ScriptEnv *context);
ScriptValue pattern_command(ScriptEnv *context);
ScriptValue design_command(ScriptEnv *context);


/* ------------------------------- Document -------------------------------- */

void doc_init(int32_t doc);
void free_doc(int32_t doc);

bool doc_allow_zoom_in(int32_t doc);
bool doc_allow_zoom_out(int32_t doc);
void doc_zoom_window(int32_t doc);
void doc_zoom_selected(int32_t doc);
void doc_zoom_to_point(int32_t doc, EmbVector mousePoint, int zoomDir);

void doc_nav(char *label, int32_t doc);
void doc_toggle(int32_t doc, const char *key, bool on);

void doc_select_all(int32_t doc);
void doc_selection_changed(int32_t doc);
void doc_clear_selection(int32_t doc);
void doc_delete_selected(int32_t doc);
void doc_move_selected(int32_t doc, EmbVector delta);
void doc_cut(int32_t doc);
void doc_copy(int32_t doc);
void doc_paste(int32_t doc);
void doc_repeat_action(int32_t doc);
void doc_move_action(int32_t doc);
void doc_scale_action(int32_t doc);
void doc_scale_selected(int32_t doc, EmbReal x, EmbReal y, EmbReal factor);
void doc_rotate_action(int32_t doc);
void doc_rotate_selected(int32_t doc, EmbVector position, EmbReal rot);
void doc_mirror_selected(int32_t doc, EmbReal x1, EmbReal y1, EmbReal x2, EmbReal y2);
int doc_num_selected(int32_t doc);

void doc_add_to_rubber_room(int32_t doc, int32_t item);

void doc_stop_gripping(int32_t, bool);
void hide_selectbox(int32_t);

void remove_paste_object_item_group(int32_t doc);
void doc_empty_grid(int32_t doc);
void doc_set_grid_color(int32_t doc, uint32_t color);
void doc_set_ruler_color(int32_t doc, uint32_t color);

void set_prompt_text_color(uint32_t color);
void set_prompt_background_color(uint32_t color);

void doc_preview_on(int32_t doc, int clone, int mode, EmbReal x, EmbReal y, EmbReal data);
void doc_preview_off(int32_t doc);

void doc_delete_pressed(int32_t doc);
void doc_escape_pressed(int32_t doc);

void doc_corner_button_clicked(int32_t doc);
void doc_create_grid(int32_t doc, char *gridType);
void doc_copy_selected(int32_t doc);

void doc_show_scroll_bars(int32_t doc, bool val);
void doc_set_prop(int32_t doc, int key, bool value);
void doc_set_corner_button(int32_t doc);
void doc_set_cross_hair_color(int32_t doc, uint32_t color);
void doc_set_cross_hair_size(int32_t doc, uint8_t percent);
void doc_set_background_color(int32_t doc, uint32_t color);
void doc_set_select_box_colors(int32_t doc, uint32_t colorL, uint32_t fillL,
    uint32_t colorR, uint32_t fillR, int alpha);

bool doc_allow_rubber(int32_t doc);
void doc_vulcanize_rubber_room(int32_t doc);
void doc_clear_rubber_room(int32_t doc);
void doc_spare_rubber(int32_t doc, int64_t id);
void doc_set_rubber_mode(int32_t doc, int mode);
void doc_set_rubber_point(int32_t doc, char *key, EmbVector point);
void doc_set_rubber_text(int32_t doc, char *key, char *txt);

void doc_recalculate_limits(int32_t doc);
void doc_center_at(int32_t doc, EmbVector centerPoint);
EmbVector doc_center(int32_t doc);

void doc_create_grid_rect(int32_t doc);
void doc_create_grid_polar(int32_t doc);
void doc_create_grid_iso(int32_t doc);
void doc_create_origin(int32_t doc);

void doc_load_ruler_settings(int32_t doc);

void doc_update_mouse_coords(int32_t doc, int x, int y);

void doc_pan_start(int32_t doc, EmbVector point);

void doc_align_scene_point_with_view_point(int32_t doc, EmbVector scenePoint, EmbVector viewPoint);

void doc_undoable_add_obj(int32_t doc, uint32_t id, int rubberMode);

double doc_width(int32_t doc);
double doc_height(int32_t doc);

void doc_corner_button_clicked(int32_t doc);
void doc_add_item(int32_t doc, uint32_t id);
void doc_update(int32_t doc_index);

void doc_center_on(int32_t doc, EmbVector v);

DocumentData *doc_data(int32_t doc);
EmbVector doc_center(int32_t doc_id);
void doc_scale(int32_t doc_id, EmbReal s);
void doc_begin_macro(int32_t doc, char *s);
void doc_end_macro(int32_t doc);

void update_color_linetype_lineweight(void);

void rotate_action(void);
void scale_action(void);

EmbVector doc_map_from_scene(int32_t doc, EmbVector v);
EmbVector doc_map_to_scene(int32_t doc, EmbVector v);

void doc_add_object(int32_t doc, uint32_t obj);
void doc_delete_object(int32_t doc, uint32_t obj);
void doc_vulcanize_object(int32_t doc, uint32_t obj);

int32_t active_document(void);

void whats_this_mode(void);

void window_close_all(void);
void window_cascade(void);
void window_tile(void);
void window_next(void);
void window_previous(void);

void enable_rapid_fire(void);
void disable_rapid_fire(void);

/* -------------------------------- Interface ------------------------------ */

void create_properties_group_box(int32_t);

void add_command(char *alias, char *cmd);

void preview_update(void);
void update_editors(int32_t id);
void edit_field(int32_t id, const char *objName, const char *text);

void show_widget(const char *key, int type);
void hide_widget(const char *key, int type);
void widget_clear(const char *key, int type);
int find_widget_map(const char *key);

void show_groups(int objType);
void show_one_type(int index);
void hide_all_groups(void);
void clear_all_fields(void);

const char *combobox_text(const char *key);
int32_t combobox_find_text(const char *key, const char *text);

void update_lineedit_num(const char *key, EmbReal num, bool useAnglePrecision);
void update_line_edit_str_if_varies(const char *key, const char *str);
void update_lineedit_str(const char *key, const char *str, char *strList[]);
void update_lineedit_bool(const char *key, bool val, bool yesOrNoText);
void update_font_combo_box_str_if_varies(const char *str);

void setHistory(const char *txt);
void obj_set_rect(uint32_t obj, EmbReal x, EmbReal y, EmbReal w, EmbReal h);

void combobox_selection_index_changed(const char *key, int32_t setting_key,
    int32_t index, uint32_t defaultColor);

void combo_ruler_metric_index_changed(int);
void combo_qsnap_locator_color_changed(int);
void combo_cool_grip_color_changed(int);
void combobox_hot_grip_color_index_changed(int);

void choose_mdi_bg_logo(void);
void choose_mdi_bg_texture(void);

void prompt_history_appended(char *txt);
void log_prompt_input(char *txt);
void prompt_input_previous(void);
void prompt_input_next(void);

/* ----------------------------- Object Core ------------------------------- */

void free_object(ObjectCore *core);

ObjectCore *obj_get_core(uint32_t id);

uint32_t create_object(int type_, uint32_t rgb);
uint32_t copy_object(uint32_t);

uint32_t create_arc(EmbArc arc, uint32_t rgb);
uint32_t create_circle(EmbCircle circle, uint32_t rgb);
uint32_t create_dimleader(EmbLine leader, uint32_t rgb);
uint32_t create_ellipse(EmbEllipse ellipse, uint32_t rgb);
uint32_t create_line(EmbLine line, uint32_t rgb);
uint32_t create_path(EmbPath *p, uint32_t rgb);
uint32_t create_point(EmbPoint p, uint32_t rgb);
uint32_t create_polygon(EmbPath *p, uint32_t rgb);
uint32_t create_polyline(EmbPath *path, uint32_t rgb);
uint32_t create_rect(EmbRect rect, uint32_t rgb);
uint32_t create_text_single(char *str, EmbVector v, uint32_t rgb);

EmbVector obj_rubber_point(int32_t id, const char *key);
const char *obj_rubber_text(int32_t id, const char *key);
EmbVector obj_map_rubber(int32_t obj, const char *key);

EmbVector obj_pos(ObjectCore *obj);
double obj_x(ObjectCore *obj);
double obj_y(ObjectCore *obj);
EmbVector obj_center(ObjectCore *obj);
double obj_center_x(ObjectCore *obj);
double obj_center_y(ObjectCore *obj);
double obj_radius(ObjectCore *obj);
double obj_diameter(ObjectCore *obj);
double obj_circumference(ObjectCore *obj);
EmbVector obj_delta(ObjectCore *obj);

EmbVector obj_end_point_1(ObjectCore *obj);
EmbVector obj_end_point_2(ObjectCore *obj);
EmbVector obj_start_point(ObjectCore *obj);
EmbVector obj_mid_point(ObjectCore *obj);
EmbVector obj_end_point(ObjectCore *obj);

double obj_length(ObjectCore *obj);

void obj_set_pos(ObjectCore *obj, EmbVector point);
void obj_set_x(ObjectCore *obj, EmbReal x);
void obj_set_y(ObjectCore *obj, EmbReal y);

void obj_set_center(ObjectCore *obj, EmbVector point);
void obj_set_center_x(ObjectCore *obj, EmbReal centerX);
void obj_set_center_y(ObjectCore *obj, EmbReal centerY);

void obj_set_end_point_1(ObjectCore *obj, EmbVector endPt1);
void obj_set_end_point_2(ObjectCore *obj, EmbVector endPt2);

void obj_set_x1(ObjectCore *obj, EmbReal x);
void obj_set_y1(ObjectCore *obj, EmbReal y);
void obj_set_x2(ObjectCore *obj, EmbReal x);
void obj_set_y2(ObjectCore *obj, EmbReal y);

void obj_set_rect(uint32_t obj, EmbReal x, EmbReal y, EmbReal w, EmbReal h);
void obj_set_rotation(uint32_t id, EmbReal rotation);
void obj_set_rubber_mode(uint32_t id, int mode);
void obj_set_rubber_point(uint32_t id, char *key, EmbVector value);
void obj_set_rubber_text(uint32_t id, char *key, char *value);
void obj_vulcanize(int32_t obj);

void obj_calculate_data(uint32_t obj);

EmbVector obj_top_left(ObjectCore *obj);
EmbVector obj_top_right(ObjectCore *obj);
EmbVector obj_bottom_left(ObjectCore *obj);
EmbVector obj_bottom_right(ObjectCore *obj);

EmbRect obj_rect(ObjectCore *obj);

void obj_set_text(ObjectCore *obj, const char *str);
void obj_set_text_font(ObjectCore *obj, const char *font);
void obj_set_text_justify(ObjectCore *obj, const char *justify);
void obj_set_text_size(ObjectCore *obj, EmbReal size);
void obj_set_text_style(ObjectCore *obj, bool bold, bool italic, bool under, bool strike, bool over);
void obj_set_text_bold(ObjectCore *obj, bool val);
void obj_set_text_italic(ObjectCore *obj, bool val);
void obj_set_text_underline(ObjectCore *obj, bool val);
void obj_set_text_strikeout(ObjectCore *obj, bool val);
void obj_set_text_overline(ObjectCore *obj, bool val);
void obj_set_text_backward(ObjectCore *obj, bool val);
void obj_set_text_upside_down(ObjectCore *obj, bool val);

void obj_move_by(int32_t id, EmbVector delta);
int32_t obj_find_index(int32_t id, EmbVector delta);
void obj_update_rubber_grip(uint32_t obj);

EmbVectorList *all_grip_points(int32_t obj_id);
EmbVector mouse_snap_point(int32_t obj_id, EmbVector mousePoint);

/* ---------------------------- Global Variables --------------------------- */
/* Global variables and constants we need to access anywhere in the program
 * with minimal overhead.
 */

extern CommandData command_data[MAX_COMMANDS];
extern StringMap aliases[MAX_ALIASES];
extern Setting setting[N_SETTINGS];
extern GroupBoxData group_box_list[];
extern Translation translations[];
extern Design designs[];
extern const char *svgs[];

extern const char *config_table[];
extern char *settings_data[];

extern ScriptValue *config;
extern int n_variables;

extern const char *index_th_name[];
extern const char *os;
extern bool exitApp;

/* Versions */
extern const char *embroidermodder_version;
extern const char *libembroidery_version;
extern const char *EmbroideryMobile_version;
extern const char *PET_version;

/* Paths */
extern const char *circle_origin_path;
extern const char *one_path;
extern const char *two_path;
extern const char *three_path;
extern const char *four_path;
extern const char *five_path;
extern const char *six_path;
extern const char *seven_path;
extern const char *eight_path;
extern const char *nine_path;
extern const char *zero_path;
extern const char *minus_path;
extern const char *apostrophe_path;
extern const char *quote_path;

/* Menus */
extern char *menu_list[];
extern int menubar_full_list[];
extern int menubar_no_docs[];

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
extern char *recent_menu[];
extern char *window_menu[];

/* Toolbars */
extern char *toolbar_list[];
extern int toolbars_when_docs[];

extern int top_toolbar[];
extern int left_toolbar[];
extern int bottom_toolbar[];
extern int toolbar_horizontal[];

extern char *file_toolbar[];
extern char *edit_toolbar[];
extern char *view_toolbar[];
extern char *zoom_toolbar[];
extern char *pan_toolbar[];
extern char *icon_toolbar[];
extern char *help_toolbar[];
extern char *draw_toolbar[];
extern char *inquiry_toolbar[];
extern char *modify_toolbar[];
extern char *dimension_toolbar[];
extern char *sandbox_toolbar[];
extern char *layer_toolbar[];
extern char *properties_toolbar[];
extern char *text_toolbar[];
extern char *prompt_toolbar[];

/* Widget Groups */
extern char *grid_load_from_file_group[];
extern char *defined_origin_group[];
extern char *rectangular_grid_group[];
extern char *circular_grid_group[];
extern char *center_on_origin_group[];
extern char *grid_enabled_group[];
extern char *rectangular_grid_visible_group[];
extern char *circular_grid_visible_group[];

/* Settings */
extern char *settings_tab_labels[];
extern int preview_to_dialog[];
extern int accept_to_dialog[];
extern int render_hints[];

/* Objects */
extern char *object_names[];

/* Testing */
extern char *coverage_test[];

/* Misc */
extern char *tips[];
extern char *extensions[];

extern bool document_memory[MAX_OPEN_FILES];

extern char formatFilterOpen[MAX_LONG_STRING];
extern char formatFilterSave[MAX_LONG_STRING];
extern char open_files_path[MAX_STRING_LENGTH];
extern char prompt_color_[MAX_STRING_LENGTH];
extern char prompt_selection_bg_color_[MAX_STRING_LENGTH];
extern char prompt_bg_color_[MAX_STRING_LENGTH];
extern char prompt_selection_color_[MAX_STRING_LENGTH];

extern char lastCmd[MAX_STRING_LENGTH];

extern ScriptEnv *global;

extern const ScriptValue script_null;
extern const ScriptValue script_true;
extern const ScriptValue script_false;

extern bool blinkState;

extern int testing_mode;
extern char *button_list[];

extern char *_appName_;
extern char *_appVer_;
extern char settings_file[MAX_STRING_LENGTH];

extern char *layer_list[];
extern char *color_list[];
extern char *line_type_list[];
extern char *line_weight_list[];
extern char *text_size_list[];

extern char *geometry_type_keys[];

extern char *editor_list[];
extern char *combobox_list[];

extern char *grid_layout[];
extern char *zoom_layout[];

extern char **xpm_icons[];
extern char *xpm_icon_labels[];

extern char recent_files[MAX_FILES][MAX_STRING_LENGTH];

extern int numOfDocs;
extern int docIndex;
extern bool shiftKeyPressedState;
extern bool cmdActive;
extern bool rapidFireEnabled;
extern bool isBlinking;
extern bool key_state[N_KEY_SEQUENCES];

extern StringMap aliasHash[MAX_ALIASES];
extern IntMap obj_index[MAX_OBJECTS];

extern EmbIdList *cutCopyObjectList;

extern bool blinkState;

extern int iconSize;

extern bool pickAdd;

extern int promptInputNum;

extern int precisionAngle;
extern int precisionLength;

extern int n_aliases;
extern int n_objects;
extern int n_commands;
extern int n_widgets;
extern int n_actions;

extern char curText[MAX_STRING_LENGTH];

extern char promptHistoryData[MAX_LONG_STRING];

extern char defaultPrefix[200];
extern char prefix[200];
extern char curCmd[200];

/* Used when checking if fields vary. */
extern char fieldOldText[MAX_STRING_LENGTH];
extern char fieldNewText[MAX_STRING_LENGTH];
extern char fieldVariesText[MAX_STRING_LENGTH];
extern char fieldYesText[MAX_STRING_LENGTH];
extern char fieldNoText[MAX_STRING_LENGTH];
extern char fieldOnText[MAX_STRING_LENGTH];
extern char fieldOffText[MAX_STRING_LENGTH];

extern WidgetData grid_gb_data[];
extern WidgetData zoom_gb_data[];

#ifdef __cplusplus
}

/* Qt wrapper */

#include <QAction>
#include <QApplication>
#include <QGraphicsPathItem>
#include <QtPrintSupport>

class Object: public QGraphicsPathItem
{
public:
    ObjectCore *core;

    QGraphicsPathItem path_;
    QPen objPen;
    QPen lwtPen;
    QLineF objLine;
    QPainter* obj_painter;

    QPainterPath textPath;
    QPainterPath lineStylePath;
    QPainterPath arrowStylePath;
    QPainterPath normalPath;

    Object(int type_, QRgb rgb, Qt::PenStyle lineType, QGraphicsItem* item = 0);
    ~Object() { free_object(core); }

    void gripEdit(EmbVector before, EmbVector after);
    QPainterPath shape() const { return path(); }

    void drawRubberLine(QLineF rubLine, QPainter* painter = 0, const char* colorFromScene = 0);

    void setObjectSize(double width, EmbReal height);

    QPainterPath objectCopyPath() const;
    QPainterPath objectSavePath() const;
    QList<QPainterPath> objectSavePathList() const { return subPathList(); }
    QList<QPainterPath> subPathList() const;

protected:
    void paint(QPainter*, const QStyleOptionGraphicsItem*, QWidget*);
};

class SelectBox: public QRubberBand
{
    Q_OBJECT

public:
    SelectBox(Shape s, QWidget* parent = 0);

public slots:
    void setDirection(int dir);
    void setColors(const QColor& colorL, const QColor& fillL, const QColor& colorR, const QColor& fillR, int newAlpha);

protected:
    void paintEvent(QPaintEvent*);

private:
    QColor leftBrushColor;
    QColor rightBrushColor;
    QColor leftPenColor;
    QColor rightPenColor;
    uint8_t alpha;

    QBrush dirBrush;
    QBrush leftBrush;
    QBrush rightBrush;

    QPen dirPen;
    QPen leftPen;
    QPen rightPen;

    bool boxDir;

    void forceRepaint();
};

class LayerManager: public QDialog
{
    Q_OBJECT

public:
    LayerManager(QWidget *parent = 0);
    ~LayerManager() { restore_cursor(); }
    LayerData data[MAX_LAYERS];

    void addLayer(char *name, bool visible, bool frozen, EmbReal zValue,
        QRgb color, char *lineType, char *lineWeight, bool print);

    QStandardItemModel* layerModel;
    QSortFilterProxyModel* layerModelSorted;
    QTreeView* treeView;
};

class MdiArea: public QMdiArea
{
    Q_OBJECT

public:
    MdiArea(QWidget* parent = 0);
    ~MdiArea() {}

    bool useLogo;
    bool useTexture;
    bool useColor;

    QPixmap bgLogo;
    QPixmap bgTexture;
    QColor bgColor;

    void forceRepaint();

public slots:
    void cascade();
    void tile();
protected:
    virtual void mouseDoubleClickEvent(QMouseEvent* e);
    virtual void paintEvent(QPaintEvent* e);
};

/* . */
class Document: public QGraphicsView
{
    Q_OBJECT

public:
    Document(QGraphicsScene* theScene, QWidget* parent);
    ~Document() { free_doc(data->id); }

    DocumentData *data;

    QVector<int64_t> hashDeletedObjects;
    QPainterPath gridPath;
    QPainterPath originPath;

    QPainter* scene_painter;

    Object* gripBaseObj;

    QGraphicsScene* gscene;
    QUndoStack* undoStack;

    SelectBox* selectBox;
    QGraphicsItemGroup* pasteObjectItemGroup;

protected:
    void mouseDoubleClickEvent(QMouseEvent* event);
    void mousePressEvent(QMouseEvent* event);
    void mouseMoveEvent(QMouseEvent* event);
    void mouseReleaseEvent(QMouseEvent* event);
    void wheelEvent(QWheelEvent* event);
    void contextMenuEvent(QContextMenuEvent* event);
    void drawBackground(QPainter* painter, const QRectF& rect);
    void draw_rulers(QPainter* painter, const QRectF& rect);
    void drawForeground(QPainter* painter, const QRectF& rect);
    void enterEvent(QEvent* event);
};

extern Document *documents[MAX_OPEN_FILES];

class UndoableCommand : public QUndoCommand
{
public:
    UndoableCommand(int type_, QString text, Object* obj, int32_t v,
        QUndoCommand* parent = 0);
    UndoableCommand(int type_, EmbVector delta, QString text, Object* obj,
        int32_t v, QUndoCommand* parent = 0);
    UndoableCommand(int type_, EmbVector pivot, EmbReal rotAngle, QString text,
        Object* obj, int32_t v, QUndoCommand* parent = 0);
    UndoableCommand(int type_, QString type, int32_t v, QUndoCommand* parent = 0);
    UndoableCommand(int type_, EmbVector start, EmbVector end, QString text,
        Object* obj, int32_t v, QUndoCommand* parent = 0);

    void undo();
    void redo();
    void rotate(double x, EmbReal y, EmbReal rot);
    int id() const { return 1234; }
    bool mergeWith(const QUndoCommand* command);
    void mirror();

    UndoData data;
    Object *object;
    QTransform toTransform;
    QTransform fromTransform;
};

class UndoEditor : public QDockWidget
{
    Q_OBJECT

public:
    UndoEditor(QString iconDirectory = "", QWidget* widgetToFocus = 0,
        QWidget* parent = 0); /*, Qt::WindowFlags flags = 0); */
    ~UndoEditor() {}

    void addStack(QUndoStack* stack);

    bool canUndo() const;
    bool canRedo() const;

    QString undoText() const;
    QString redoText() const;

public slots:
    void undo();
    void redo();

    void updateCleanIcon(bool opened);

private:
    QWidget* focusWidget;

    QString iconDir;
    int iconSize;

    QUndoGroup* undoGroup;
    QUndoView*  undoView;
};

class PropertyEditor: public QDockWidget
{
    Q_OBJECT

public:
    PropertyEditor(QString iconDirectory = "", bool pickAddMode = true,
        QWidget* widgetToFocus = 0, QWidget* parent = 0);
        /*, Qt::WindowFlags flags = 0); */
    ~PropertyEditor() {}

    void togglePickAddMode();

protected:
    bool eventFilter(QObject *obj, QEvent *event);

signals:
    void pick_add_mode_toggled();

public slots:
    void setSelectedItems(QList<QGraphicsItem*> itemList);
    void update_pick_add_modeButton(bool pickAddMode);
};

class MdiWindow: public QMdiSubWindow
{
    Q_OBJECT

public:
    MdiWindow(const int theIndex, QMdiArea* parent, Qt::WindowFlags wflags);
    ~MdiWindow() {
        debug_message("MdiWindow Destructor()");
        emb_pattern_free(documents[doc_index]->data->pattern);
    }

    QMdiArea* mdiArea;
    QGraphicsScene* gscene;
    int32_t doc_index;

    QPrinter printer;

    virtual QSize sizeHint() const;
    QString getShortCurrentFile();
    bool loadFile(char *fileName);
signals:
    void sendCloseMdiWin(MdiWindow*);

public slots:
    void closeEvent(QCloseEvent* e);
    void onWindowActivated();

    void print();
    void saveBMC();

private:
    void setCurrentFile(char *fileName);
};

/* On Mac, if the user drops a file on the app's Dock icon, or uses Open As,
 * then this is how the app actually opens the file.
 */
class Application : public QApplication
{
    Q_OBJECT
public:
    Application(int argc, char **argv);
protected:
    virtual bool event(QEvent *e);
};

class CmdPromptInput: public QLineEdit
{
    Q_OBJECT

public:
    CmdPromptInput(QWidget* parent = 0);
    ~CmdPromptInput() {}

protected:
    void contextMenuEvent(QContextMenuEvent *event);
    bool eventFilter(QObject *obj, QEvent *event);

public slots:
    void checkSelection();
    void updateCurrentText(QString txt);
    void checkEditedText(QString txt);
    void checkChangedText(QString txt);
    void checkCursorPosition(int oldpos, int newpos);
private slots:
    void copyClip();
    void pasteClip();
};

class MainWindow: public QMainWindow
{
    Q_OBJECT

public:
    MainWindow();
    ~MainWindow() {
        debug_message("Destructor()");

        /* Prevent memory leaks by deleting any unpasted objects. */
        free_objects(cutCopyObjectList);
        free_id_list(cutCopyObjectList);
    }

    void add_toolbar_to_window(Qt::ToolBarArea area, int data[]);

public slots:
    void recentMenuAboutToShow();
    void windowMenuActivated(bool checked /*, int id*/);

    void closeToolBar(QAction*);
    void floatingChangedToolBar(bool);

    void runCommand();

    void colorSelectorIndexChanged(int index);

protected:
    virtual void resizeEvent(QResizeEvent*);
    void closeEvent(QCloseEvent *event);
};

#endif

#endif

