/*
 * Embroidermodder 2.
 *
 * Copyright 2011-2024 The Embroidermodder Team
 * Embroidermodder 2 is Open Source Software, see LICENSE.md for licensing terms.
 *
 * Read the reference manual (https://www.libembroidery.org/downloads/emrm.pdf)
 * for advice on altering this file.
 *
 * Integer Constants
 */

#ifndef EMBROIDERMODDER_CONSTANTS
#define EMBROIDERMODDER_CONSTANTS

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
#define TOOLBAR_ZOOM                   3
#define TOOLBAR_PAN                    4
#define TOOLBAR_ICON                   5
#define TOOLBAR_HELP                   6
#define TOOLBAR_LAYER                  7
#define TOOLBAR_PROPERTIES             8
#define TOOLBAR_TEXT                   9
#define TOOLBAR_PROMPT                10
#define TOOLBAR_DRAW                  11
#define TOOLBAR_MODIFY                12
#define TOOLBAR_INQUIRY               13
#define TOOLBAR_DIMENSION             14
#define TOOLBAR_SANDBOX               15
#define N_TOOLBARS                    16

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

#endif

