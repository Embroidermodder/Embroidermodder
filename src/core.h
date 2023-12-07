/*
 * Embroidermodder 2.
 *
 * ------------------------------------------------------------
 *
 * Copyright 2013-2023 The Embroidermodder Team
 * Embroidermodder 2 is Open Source Software.
 * See LICENSE for licensing terms.
 *
 * ------------------------------------------------------------
 *
 * Use Python's PEP7 style guide.
 *     https://peps.python.org/pep-0007/
 *
 * ------------------------------------------------------------
 *
 * Since we use a lot of switch tables and a generally
 * procedural style, most of these constants are for identifying
 * our internal types. More general constants belong
 * in "embroidery.h" which does all the grunt work for our geometry.
 */

#ifndef __EMBROIDERMODDER_CONSTANTS__
#define __EMBROIDERMODDER_CONSTANTS__

/* Maximums for C-style memory arrays. */
#define MAX_STRING_LENGTH                      200
#define MAX_HISTOGRAM_BINS                    1000
#define MAX_TOOLBAR_LENGTH                      50
#define MAX_MENU_LENGTH                         30
#define MAX_MENUBAR_LENGTH                      10
#define MAX_TOOLBARS                            40
#define MAX_MENUS                               40
#define VECTOR_CHUNK_SIZE                       50
#define MAX_ACTIONS                            256
#define MAX_ICONS                              256
#define MAX_SETTINGS                           256
#define MAX_POSITIONS                           26
#define MAX_EDITORS                            300

/* Node types. */
#define NODE_NULL                                0
#define NODE_STRING                              1
#define NODE_REAL                                2
#define NODE_INT                                 3
#define NODE_BOOL                                4
#define NODE_FUNCTION                            5
#define NODE_DICTIONARY                          6
#define NODE_ARRAY                               7
#define NODE_VECTOR                              8
#define NODE_UNKNOWN                             9

/* Actions.
 * These identifiers are subject to change since they are in alphabetical order
 * and the numbers are increasing.
 */
#define ACTION_ABOUT                             0
#define ACTION_ADD_ARC                           1
#define ACTION_ADD_CIRCLE                        2
#define ACTION_ADD_DIM_LEADER                    3
#define ACTION_ADD_ELLIPSE                       4
#define ACTION_ADD_GEOMETRY                      5
#define ACTION_ADD_HORIZONTAL_DIMENSION          6
#define ACTION_ADD_IMAGE                         7
#define ACTION_ADD_INFINITE_LINE                 8
#define ACTION_ADD_LINE                          9
#define ACTION_ADD_PATH                         10
#define ACTION_ADD_POINT                        11
#define ACTION_ADD_POLYGON                      12
#define ACTION_ADD_POLYLINE                     13
#define ACTION_ADD_RAY                          14
#define ACTION_ADD_RECTANGLE                    15
#define ACTION_ADD_REGULAR_POLYGON              16
#define ACTION_ADD_ROUNDED_RECTANGLE            17
#define ACTION_ADD_RUBBER                       18
#define ACTION_ADD_SLOT                         19
#define ACTION_ADD_TEXT_MULTI                   20
#define ACTION_ADD_TEXT_SINGLE                  21
#define ACTION_ADD_TO_SELECTION                 22
#define ACTION_ADD_TRIANGLE                     23
#define ACTION_ADD_VERTICAL_DIMENSION           24
#define ACTION_ALERT                            25
#define ACTION_ALLOW_RUBBER                     26
#define ACTION_APPEND_HISTORY                   27
#define ACTION_CALCULATE_ANGLE                  28
#define ACTION_CALCULATE_DISTANCE               29
#define ACTION_CHANGELOG                        30
#define ACTION_CLEAR_RUBBER                     31
#define ACTION_CLEAR_SELECTION                  32
#define ACTION_COPY                             33
#define ACTION_COPY_SELECTED                    34
#define ACTION_CUT                              35
#define ACTION_CUT_SELECTED                     36
#define ACTION_DAY                              37
#define ACTION_DEBUG                            38
#define ACTION_DELETE_SELECTED                  39
#define ACTION_DESIGN_DETAILS                   40
#define ACTION_DO_NOTHING                       41
#define ACTION_END                              42
#define ACTION_ERROR                            43
#define ACTION_HELP                             44
#define ACTION_ICON                             45
#define ACTION_INIT                             46
#define ACTION_MESSAGEBOX                       47
#define ACTION_MIRROR_SELECTED                  48
#define ACTION_MOUSE_X                          49
#define ACTION_MOUSE_Y                          50
#define ACTION_MOVE_SELECTED                    51
#define ACTION_NEW                              52
#define ACTION_NIGHT                            53
#define ACTION_NUM_SELECTED                     54
#define ACTION_OPEN                             55
#define ACTION_PAN                              56
#define ACTION_PASTE                            57
#define ACTION_PASTE_SELECTED                   58
#define ACTION_PERPENDICULAR_DISTANCE           59
#define ACTION_PLATFORM                         60
#define ACTION_PREVIEW_OFF                      61
#define ACTION_PREVIEW_ON                       62
#define ACTION_PRINT                            63
#define ACTION_PRINT_AREA                       64
#define ACTION_QSNAP_X                          65
#define ACTION_QSNAP_Y                          66
#define ACTION_EXIT                             67
#define ACTION_REDO                             68
#define ACTION_ROTATE_SELECTED                  69
#define ACTION_RUBBER                           70
#define ACTION_SCALE_SELECTED                   71
#define ACTION_SELECT_ALL                       72
#define ACTION_SETTINGS_DIALOG                  73
#define ACTION_SET_BACKGROUND_COLOR             74
#define ACTION_SET_CROSSHAIR_COLOR              75
#define ACTION_SET_CURSOR_SHAPE                 76
#define ACTION_SET_GRID_COLOR                   77
#define ACTION_SET_PROMPT_PREFIX                78
#define ACTION_SET_RUBBER_FILTER                79
#define ACTION_SET_RUBBER_MODE                  80
#define ACTION_SET_RUBBER_POINT                 81
#define ACTION_SET_RUBBER_TEXT                  82
#define ACTION_SPARE_RUBBER                     83
#define ACTION_TIP_OF_THE_DAY                   84
#define ACTION_TODO                             85
#define ACTION_UNDO                             86
#define ACTION_VERSION                          87
#define ACTION_VULCANIZE                        88
#define ACTION_WHATS_THIS                       89
#define ACTION_WINDOW_CLOSE                     90
#define ACTION_WINDOW_CLOSE_ALL                 91
#define ACTION_WINDOW_TILE                      92
#define ACTION_WINDOW_CASCADE                   93
#define ACTION_WINDOW_NEXT                      94
#define ACTION_WINDOW_PREVIOUS                  95
#define ACTION_ZOOM                             96
#define ACTION_ZOOM_IN                          97
#define ACTION_TEST                             98
#define ACTION_SLEEP                            99
#define ACTION_LAYER_EDITOR                    100
#define ACTION_MAKE_LAYER_CURRENT              101
#define ACTION_TEXT_BOLD                       102
#define ACTION_TEXT_ITALIC                     103
#define ACTION_TEXT_UNDERLINE                  104
#define ACTION_TEXT_STRIKEOUT                  105
#define ACTION_TEXT_OVERLINE                   106
#define ACTION_LAYER_PREVIOUS                  107
#define ACTION_ICON16                          108
#define ACTION_ICON24                          109
#define ACTION_ICON32                          110
#define ACTION_ICON48                          111
#define ACTION_ICON64                          112
#define ACTION_ICON128                         113
#define ACTION_SAVE                            114
#define ACTION_SAVEAS                          115
#define ACTION_PAN_REAL_TIME                   116
#define ACTION_PAN_POINT                       117
#define ACTION_PAN_LEFT                        118
#define ACTION_PAN_RIGHT                       119
#define ACTION_PAN_UP                          120
#define ACTION_PAN_DOWN                        121
#define ACTION_ZOOM_REAL_TIME                  122
#define ACTION_ZOOM_PREVIOUS                   123
#define ACTION_ZOOM_WINDOW                     124
#define ACTION_ZOOM_DYNAMIC                    125
#define ACTION_ZOOM_OUT                        126
#define ACTION_ZOOM_EXTENTS                    127
#define ACTION_LAYERS                          128
#define ACTION_LAYER_SELECTOR                  129
#define ACTION_TREBLECLEF                      130
#define ACTION_COLOR_SELECTOR                  131
#define ACTION_LINE_TYPE_SELECTOR              132
#define ACTION_LINE_WEIGHT_SELECTOR            133
#define ACTION_ZOOM_SCALE                      134
#define ACTION_ZOOM_CENTER                     135
#define ACTION_HIDE_ALL_LAYERS                 136
#define ACTION_ZOOM_SELECTED                   137
#define ACTION_ZOOM_ALL                        138
#define ACTION_ADD_HEART                       139
#define ACTION_ADD_SINGLE_LINE_TEXT            140
#define ACTION_SHOW_ALL_LAYERS                 141
#define ACTION_FREEZE_ALL_LAYERS               142
#define ACTION_THAW_ALL_LAYERS                 143
#define ACTION_LOCK_ALL_LAYERS                 144
#define ACTION_UNLOCK_ALL_LAYERS               145
#define ACTION_ADD_DOLPHIN                     146
#define ACTION_ADD_DISTANCE                    147
#define ACTION_LOCATE_POINT                    148
#define ACTION_QUICKSELECT                     149
#define ACTION_SPELLCHECK                      150
#define ACTION_DISTANCE                        151
#define ACTION_MOVE                            152
#define ACTION_QUICKLEADER                     153
#define ACTION_RGB                             154
#define ACTION_ROTATE                          155
#define ACTION_SANDBOX                         156
#define ACTION_ADD_SNOWFLAKE                   157
#define ACTION_ADD_STAR                        158
#define ACTION_DELETE                          159
#define ACTION_SCALE                           160
#define ACTION_SINGLE_LINE_TEXT                161
#define ACTION_SYSWINDOWS                      162
#define N_ACTIONS                              163

/* OBJ_LTYPE_VALUES */
// CAD Linetypes
#define OBJ_LTYPE_CONT                          0
#define OBJ_LTYPE_CENTER                        1
#define OBJ_LTYPE_DOT                           2
#define OBJ_LTYPE_HIDDEN                        3
#define OBJ_LTYPE_PHANTOM                       4
#define OBJ_LTYPE_ZIGZAG                        5
/* Embroidery Stitchtypes */
#define OBJ_LTYPE_RUNNING                       6    // __________
#define OBJ_LTYPE_SATIN                         7    // vvvvvvvvvv
#define OBJ_LTYPE_FISHBONE                      8    // >>>>>>>>>>


/* OBJ_LWT_VALUES */
#define OBJ_LWT_BYLAYER                        -2
#define OBJ_LWT_BYBLOCK                        -1
#define OBJ_LWT_DEFAULT                         0
#define OBJ_LWT_01                              1
#define OBJ_LWT_02                              2
#define OBJ_LWT_03                              3
#define OBJ_LWT_04                              4
#define OBJ_LWT_05                              5
#define OBJ_LWT_06                              6
#define OBJ_LWT_07                              7
#define OBJ_LWT_08                              8
#define OBJ_LWT_09                              9
#define OBJ_LWT_10                             10
#define OBJ_LWT_11                             11
#define OBJ_LWT_12                             12
#define OBJ_LWT_13                             13
#define OBJ_LWT_14                             14
#define OBJ_LWT_15                             15
#define OBJ_LWT_16                             16
#define OBJ_LWT_17                             17
#define OBJ_LWT_18                             18
#define OBJ_LWT_19                             19
#define OBJ_LWT_20                             20
#define OBJ_LWT_21                             21
#define OBJ_LWT_22                             22
#define OBJ_LWT_23                             23
#define OBJ_LWT_24                             24


/* OBJ_SNAP_VALUES */
/* NOTE: Allow this enum to evaluate false. */
#define OBJ_SNAP_NULL                            0
#define OBJ_SNAP_ENDPOINT                        1
#define OBJ_SNAP_MIDPOINT                        2
#define OBJ_SNAP_CENTER                          3
#define OBJ_SNAP_NODE                            4
#define OBJ_SNAP_QUADRANT                        5
#define OBJ_SNAP_INTERSECTION                    6
#define OBJ_SNAP_EXTENSION                       7
#define OBJ_SNAP_INSERTION                       8
#define OBJ_SNAP_PERPENDICULAR                   9
#define OBJ_SNAP_TANGENT                        10
#define OBJ_SNAP_NEAREST                        11
#define OBJ_SNAP_APPINTERSECTION                12
#define OBJ_SNAP_PARALLEL                       13

/* Allow OBJ_TYPE to evaluate false.
 * Values >= 65536 ensure compatibility with qgraphicsitem_cast().
 *
 * For SVG we need: Block.
 * For DXF we meed: Aligned Dimension, Angular Dimension, Arc Length Dimension,
 *     Linear Dimension, Ordinate Dimension, Radial Dimension and Infinite Line
 *     object.
 *
 * The objects which should be removed from stitch output as they exist for
 * drafting reasons are all the ...
 *
 * Ray?
 */
#define OBJ_TYPE_NULL                            0
#define OBJ_TYPE_BASE                       100000
#define OBJ_TYPE_ARC                        100001
#define OBJ_TYPE_BLOCK                      100002
#define OBJ_TYPE_CIRCLE                     100003
#define OBJ_TYPE_DIMALIGNED                 100004
#define OBJ_TYPE_DIMANGULAR                 100005
#define OBJ_TYPE_DIMARCLENGTH               100006
#define OBJ_TYPE_DIMDIAMETER                100007
#define OBJ_TYPE_DIMLEADER                  100008
#define OBJ_TYPE_DIMLINEAR                  100009
#define OBJ_TYPE_DIMORDINATE                100010
#define OBJ_TYPE_DIMRADIUS                  100011
#define OBJ_TYPE_ELLIPSE                    100012
#define OBJ_TYPE_ELLIPSEARC                 100013
#define OBJ_TYPE_RUBBER                     100014
#define OBJ_TYPE_GRID                       100015
#define OBJ_TYPE_HATCH                      100016
#define OBJ_TYPE_IMAGE                      100017
#define OBJ_TYPE_INFINITELINE               100018
#define OBJ_TYPE_LINE                       100019
#define OBJ_TYPE_PATH                       100020
#define OBJ_TYPE_POINT                      100021
#define OBJ_TYPE_POLYGON                    100022
#define OBJ_TYPE_POLYLINE                   100023
#define OBJ_TYPE_RAY                        100024
#define OBJ_TYPE_RECTANGLE                  100025
#define OBJ_TYPE_SLOT                       100026
#define OBJ_TYPE_SPLINE                     100027
#define OBJ_TYPE_TEXTMULTI                  100028
#define OBJ_TYPE_TEXTSINGLE                 100029
#define OBJ_TYPE_UNKNOWN                    100030

/*
 * Custom Data used in QGraphicsItems
 *
 *                    (     int, const QVariant)
 * I.E. object.setData(OBJ_TYPE, OBJ_TYPE_LINE);
 * I.E. object.setData(OBJ_LAYER, "OUTLINE");
 * I.E. object.setData(OBJ_COLOR, 123);
 * I.E. object.setData(OBJ_LTYPE, OBJ_LTYPE_CONT);
 *
 * OBJ_KEYS
 *
 * TYPE value type - int: See OBJ_TYPE_VALUES
 * NAME value type - str: See OBJ_NAME_VALUES
 * LAYER value type - str: "USER", "DEFINED", "STRINGS", etc...
 * COLOR value type - int: 0-255
 *     TODO: Use color chart in formats/format-dxf.h for this.
 * LTYPE value type - int: See OBJ_LTYPE_VALUES
 * LWT value type - int: 0-27
 * RUBBER value type - int: See OBJ_RUBBER_VALUES
 */
#define OBJ_TYPE                                 0
#define OBJ_NAME                                 1
#define OBJ_LAYER                                2
#define OBJ_COLOR                                3
#define OBJ_LTYPE                                4
#define OBJ_LWT                                  5
#define OBJ_RUBBER                               6

/* Arrow Styles */
/* NOTE: Allow this enum to evaluate false. */
#define ARROW_STYLE_NONE                         0
#define ARROW_STYLE_OPEN                         1
#define ARROW_STYLE_CLOSED                       2
#define ARROW_STYLE_DOT                          3
#define ARROW_STYLE_BOX                          4
#define ARROW_STYLE_TICK                         5

/* Line Styles */
/* NOTE: Allow this enum to evaluate false. */
#define LINE_STYLE_NONE                          0
#define LINE_STYLE_FLARED                        1
#define LINE_STYLE_FLETCHING                     2

/* Mathematical Constants. */
#define CONSTANT_PI         3.14159265358979323846
#define RADIANS_TO_DEGREES    (180.0f/CONSTANT_PI)
#define DEGREES_TO_RADIANS    (CONSTANT_PI/180.0f)

/* UI Editing Modes */

/* Polygon editing modes. */
#define POLYGON_MODE_NUM_SIDES                   0
#define POLYGON_MODE_CENTER_PT                   1
#define POLYGON_MODE_POLYTYPE                    2
#define POLYGON_MODE_INSCRIBE                    3
#define POLYGON_MODE_CIRCUMSCRIBE                4
#define POLYGON_MODE_DISTANCE                    5
#define POLYGON_MODE_SIDE_LEN                    6

/* Point identifiers. */
#define ARC_START_POINT                          0
#define ARC_MID_POINT                            1
#define ARC_END_POINT                            2

/* Real Identifiers */
#define ARC_RADIUS                               3
#define ARC_START_ANGLE                          4
#define ARC_END_ANGLE                            5
#define ARC_DIAMETER                             6
#define ARC_AREA                                 7
#define ARC_CIRCUMFERENCE                        9
#define ARC_POSITION                            10
#define OBJECT_TEXT                             12
#define OBJECT_TEXT_FONT                        13
#define OBJECT_TEXT_JUSTIFY                     14
#define OBJECT_TEXT_SIZE                        15
#define OBJECT_TEXT_BOLD                        16
#define OBJECT_TEXT_ITALIC                      17
#define OBJECT_TEXT_UNDERLINE                   18
#define OBJECT_TEXT_STRIKEOUT                   19
#define OBJECT_TEXT_OVERLINE                    20
#define OBJECT_TEXT_BACKWARD                    21
#define OBJECT_TEXT_UPSIDEDOWN                  22
#define OBJECT_RADIUS_MAJOR                     23
#define OBJECT_RADIUS_MINOR                     24
#define OBJECT_DIAMETER_MAJOR                   25
#define OBJECT_DIAMETER_MINOR                   26


#define N_RUBBER_MODES                          40

/* Per object constants. */
#define POINTS_PER_BASE_OBJECT                 100
#define REALS_PER_BASE_OBJECT                  100
#define BOOLS_PER_BASE_OBJECT                  100
#define STRINGS_PER_BASE_OBJECT                 10

/* User Interface Mode */
#define MODE_ARC                                 0
#define MODE_CIRCLE_1P_RAD                       1
#define MODE_CIRCLE_1P_DIA                       2
#define MODE_CIRCLE_2P                           3
#define MODE_CIRCLE_3P                           4
#define MODE_CIRCLE_TTR                          5
#define MODE_CIRCLE_TTR_SET_POINT_2              6
#define MODE_CIRCLE_TTR_SET_POINT_3              7
#define MODE_ELLIPSE                             8
#define MODE_RECTANGLE                           9
#define MODE_STAR_NUM_POINTS                    10
#define MODE_STAR_CENTER_PT                     11
#define MODE_STAR_RAD_INNER                     12
#define MODE_STAR_RAD_OUTER                     13
#define MODE_POLYGON_NUM_SIDES                  14
#define MODE_POLYGON_POLYTYPE                   15
#define MODE_POLYGON_DISTANCE                   16
#define MODE_POLYGON_CENTER_PT                  17
#define MODE_POLYGON_INSCRIBE                   18
#define MODE_POLYGON_CIRCUMSCRIBE               19
#define MODE_POLYGON_SIDE_LEN                   20

/* Object Properties: packed into the uint64_t flags variable. */
#define PROP_BOLD                           0x0001
#define PROP_ITALIC                         0x0002
#define PROP_UNDERLINE                      0x0004
#define PROP_STRIKEOUT                      0x0008
#define PROP_OVERLINE                       0x0010
#define PROP_BACKWARD                       0x0020
#define PROP_UPSIDEDOWN                     0x0040
#define PROP_CURVED                         0x0080
#define PROP_FILLED                         0x0100

/* Groupboxen */
#define GB_GENERAL                               0
#define GB_GROUPBOX_ARC                          1
#define GB_MISC_ARC                              2
#define GB_GEOM_BLOCK                            3
#define GB_GEOM_CIRCLE                           4
#define GB_GEOM_DIM_ALIGNED                      5
#define GB_GEOM_DIM_ANGULAR                      6
#define GB_GEOM_DIM_ARC_LENGTH                   7
#define GB_GEOM_DIM_DIAMETER                     8
#define GB_GEOM_DIM_LEADER                       9
#define GB_GEOM_DIM_LINEAR                      10
#define GB_GEOM_DIM_ORDINATE                    11
#define GB_GEOM_DIM_RADIUS                      12
#define GB_GEOM_ELLIPSE                         13
#define GB_GEOM_IMAGE                           14
#define GB_MISC_IMAGE                           15
#define GB_INFINITE_LINE                        16
#define GB_GEOM_LINE                            17
#define GB_GEOM_PATH                            18
#define GB_MISC_PATH                            19
#define GB_GEOM_POINT                           20
#define GB_GEOM_POLYGON                         21
#define GB_GEOM_POLYLINE                        22
#define GB_MISC_POLYLINE                        23
#define GB_GEOM_RAY                             24
#define GB_GEOM_RECTANGLE                       25
#define GB_GEOM_TEXT_MULTI                      26
#define GB_TEXT_TEXT_SINGLE                     27
#define GB_GEOM_TEXT_SINGLE                     28
#define GB_MISC_TEXT_SINGLE                     29
#define GB_TOTAL                                30

/* Settings indices. */
#define ST_LANGUAGE                              0
#define ST_ICON_THEME                            1
#define ST_ICON_SIZE                             2
#define ST_MDI_USE_LOGO                          3
#define ST_MDI_USE_TEXTURE                       4
#define ST_MDI_USE_COLOR                         5
#define ST_MDI_LOGO                              6
#define ST_MDI_TEXTURE                           7
#define ST_MDI_COLOR                             8
#define ST_TIP_OF_THE_DAY                        9
#define ST_CURRENT_TIP                          10
#define ST_SYSTEM_HELP_BROWSER                  11

/* Window settings. */
#define ST_WINDOW_POS_X                         12
#define ST_WINDOW_POS_Y                         13
#define ST_WINDOW_SIZE_X                        14
#define ST_WINDOW_SIZE_Y                        15

/* Display settings. */
#define ST_USE_OPENGL                           16
#define ST_ANTI_ALIAS                           17
#define ST_TEXT_ANTI_ALIAS                      18
#define ST_SMOOTH_PIXMAP                        19
#define ST_HQ_ANTI_ALIAS                        20
#define ST_NON_COSMETIC                         21
#define ST_SHOW_SCROLLBARS                      22
#define ST_SCROLLBAR_WIDGET_NUM                 23
#define ST_CROSSHAIR_COLOR                      24
#define ST_BG_COLOR                             25
#define ST_SELECTBOX_LEFT_COLOR                 26
#define ST_SELECTBOX_LEFT_FILL                  27
#define ST_SELECTBOX_RIGHT_COLOR                28
#define ST_SELECTBOX_RIGHT_FILL                 29
#define ST_SELECTBOX_ALPHA                      30
#define ST_ZOOMSCALE_IN                         31
#define ST_ZOOMSCALE_OUT                        32
#define ST_CROSSHAIR_PERCENT                    33
#define ST_DISPLAY_UNITS                        34

/* Prompt settings. */
#define ST_PROMPT_TEXT_COLOR                    35
#define ST_PROMPT_BG_COLOR                      36
#define ST_PROMPT_FONT_FAMILY                   37
#define ST_PROMPT_FONT_STYLE                    38
#define ST_PROMPT_FONT_SIZE                     39
#define ST_SAVE_HISTORY                         40
#define ST_HTML_OUTPUT                          41
#define ST_HISTORY_FILE                         42

/* Open/save settings. */
#define ST_OPENSAVE_FILTER                      43
#define ST_OPENSAVE_FORMAT                      44
#define ST_OPEN_THUMBNAIL                       45
#define ST_SAVE_FORMAT                          46
#define ST_SAVE_THUMBNAIL                       47
#define ST_RECENT_MAX                           48
#define ST_RECENT_FILES                         49
#define ST_RECENT_DIRECTORY                     50
#define ST_TRIM_NUM_JUMPS                       51

/* Print settings. */
#define ST_DEFAULT_PRINTER                      52
#define ST_USE_LAST_PRINTER                     53
#define ST_PRINT_DISABLE_BG                     54

/* Grid settings. */
#define ST_GRID_ON_LOAD                         55
#define ST_SHOW_ORIGIN                          56
#define ST_MATCH_GRID_CROSSHAIR                 57
#define ST_GRID_COLOR                           58
#define ST_GRID_LOAD_FROM_FILE                  59
#define ST_GRID_TYPE                            60
#define ST_GRID_CENTER_ORIGIN                   61
#define ST_GRID_SIZE_RADIUS                     62
#define ST_GRID_SPACING_RADIUS                  63
#define ST_GRID_SPACING_ANGLE                   64

/* Ruler settings. */
#define ST_RULER_ON_LOAD                        65
#define ST_RULER_METRIC                         66
#define ST_RULER_COLOR                          67
#define ST_RULER_SIZE                           68

/* Quicksnap settings. */
#define ST_QSNAP_ENABLED                        69
#define ST_QSNAP_LOCATOR_COLOR                  70
#define ST_QSNAP_LOCATOR_SIZE                   71
#define ST_QSNAP_APERTURE_SIZE                  72
#define ST_QSNAP_ENDPOINT                       73
#define ST_QSNAP_MIDPOINT                       74
#define ST_QSNAP_CENTER                         75
#define ST_QSNAP_NODE                           76
#define ST_QSNAP_QUADRANT                       77
#define ST_QSNAP_INTERSECTION                   78
#define ST_QSNAP_EXTENSION                      79
#define ST_QSNAP_INSERTION                      80
#define ST_QSNAP_PERPENDICULAR                  81
#define ST_QSNAP_TANGENT                        82
#define ST_QSNAP_NEAREST                        83
#define ST_QSNAP_APPARENT                       84
#define ST_QSNAP_PARALLEL                       85

/* Lineweight settings. */
#define ST_LWT_SHOW                             86
#define ST_LWT_REAL                             87
#define ST_LWT_DEFAULT                          88

/* Selection settings. */
#define ST_SELECTION_PICK_FIRST                 89
#define ST_SELECTION_PICK_ADD                   90
#define ST_SELECTION_PICK_DRAG                  91
#define ST_SELECTION_COOLGRIP_COLOR             92
#define ST_SELECTION_HOTGRIP_COLOR              93
#define ST_SELECTION_GRIP_SIZE                  94
#define ST_SELECTION_PICKBOX_SIZE               95

/* Text settings. */
#define ST_TEXT_FONT                            96
#define ST_TEXT_SIZE                            97
#define ST_TEXT_ANGLE                           98
#define ST_TEXT_BOLD                            99
#define ST_TEXT_ITALIC                         100
#define ST_TEXT_UNDERLINE                      101
#define ST_TEXT_STRIKEOUT                      102
#define ST_TEXT_OVERLINE                       103

#define ST_TICK_DEPTH                          104
#define ST_MAJOR_TICK_SEPERATION               105
#define ST_NEEDLE_SPEED                        106
#define ST_STITCH_TIME                         107

#define ST_GRID_CENTER_X                       108
#define ST_GRID_CENTER_Y                       109
#define ST_GRID_SIZE_X                         110
#define ST_GRID_SIZE_Y                         111
#define ST_GRID_SPACING_X                      112
#define ST_GRID_SPACING_Y                      113

#define SETTINGS_TOTAL                         114

/* Line editors */
#define LE_ARC_CENTER_X                          0
#define LE_ARC_CENTER_Y                          1
#define LE_ARC_RADIUS                            2
#define LE_ARC_START_ANGLE                       3
#define LE_ARC_END_ANGLE                         4
#define LE_ARC_START_X                           5
#define LE_ARC_START_Y                           6
#define LE_ARC_END_X                             7
#define LE_ARC_END_Y                             8
#define LE_ARC_AREA                              9
#define LE_ARC_LENGTH                           10
#define LE_ARC_CHORD                            11
#define LE_ARC_INC_ANGLE                        12
#define LE_ARC_CLOCKWISE                        13

/* View state */
#define VIEW_STATE_GRID                       0x01
#define VIEW_STATE_ORTHO                      0x02
#define VIEW_STATE_POLAR                      0x04
#define VIEW_STATE_ISO                        0x08
#define VIEW_STATE_QSNAP                      0x10
#define VIEW_STATE_QTRACK                     0x20
#define VIEW_STATE_LWT                        0x40
#define VIEW_STATE_REAL                       0x80
#define VIEW_STATE_METRIC                    0x100
#define VIEW_STATE_SIMULATE                  0x200
#define VIEW_STATE_SNAP                      0x400
#define VIEW_STATE_RULER                     0x800

/* Preview mode */
#define PREVIEW_MODE_NULL                        0
#define PREVIEW_MODE_MOVE                        1
#define PREVIEW_MODE_ROTATE                      2
#define PREVIEW_MODE_SCALE                       3

/* Preview clone */
#define PREVIEW_CLONE_NULL                       0
#define PREVIEW_CLONE_RUBBER                     1
#define PREVIEW_CLONE_SELECTED                   2

/* Menus */
#define MENU_ICON                               -4
#define MENU_SUBMENU                            -3
#define MENU_SEPERATOR                          -2
#define MENU_END                                -1
#define MENU_FILE                                0
#define MENU_EDIT                                1
#define MENU_PAN                                 2
#define MENU_ZOOM                                3
#define MENU_VIEW                                4
#define MENU_SETTINGS                            5
#define MENU_WINDOW                              6
#define MENU_HELP                                7
#define MENU_DRAW                                8
#define MENU_RECENT                              9
#define TOTAL_MENUS                             10

/* Toolbars */
#define TOOLBAR_SEPERATOR                       -2
#define TOOLBAR_END                             -1
#define TOOLBAR_FILE                             0
#define TOOLBAR_EDIT                             1
#define TOOLBAR_PAN                              2
#define TOOLBAR_ZOOM                             3
#define TOOLBAR_VIEW                             4
#define TOOLBAR_SETTINGS                         5
#define TOOLBAR_WINDOW                           6
#define TOOLBAR_HELP                             7
#define TOOLBAR_DRAW                             8
#define TOOLBAR_ICON                             9
#define TOOLBAR_LAYER                           10
#define TOOLBAR_PROPERTIES                      11
#define TOOLBAR_TEXT                            12
#define TOOLBAR_PROMPT                          13
#define TOTAL_TOOLBARS                          14

#ifdef __cplusplus
extern "C" {
#endif

/* C/C++ Standard Libraries. */
#include <stdio.h>
#include <math.h>
#include <time.h>
#include <inttypes.h>
#include <stdarg.h>
#include <stdbool.h>
#include <assert.h>
#include <errno.h>

/* We assume here that all free systems and MacOS are POSIX compliant. */
#if defined(WIN32)
#include <windows.h>
#else
#include <unistd.h>
#include <sys/utsname.h>
#endif

#include "../extern/libembroidery/src/embroidery.h"

/*
 * Expected Keys for actions
 * String icon
 *      The stub used for finding the icon image file.
 * String command
 *      The command sent to the actuator.
 * String tooltip
 *      The label in the menus and the message that appears when
 *      you hover over an icon.
 * String statustip
 *       The message that appears at the bottom of the .
 * String shortcut
 *       The keyboard shortcut for this action.
 * StringList aliases
 *       A list of all alternative commands, if empty only
 *       the icon sttring will be .
 * StringList script
 *      If this is a compound action this will be a
 *      list of commands or it can allow for command line
 *      style command aliases. For example: icon16 would become
 *      the string list {"iconResize 16"}.
 */
typedef struct ActionData_ {
    int32_t id;
    char icon[MAX_STRING_LENGTH];
    char command[MAX_STRING_LENGTH];
    char tooltip[MAX_STRING_LENGTH];
    char statustip[MAX_STRING_LENGTH];
    char shortcut[MAX_STRING_LENGTH];
    int32_t min_args;
    int32_t gview;
    int32_t gscene;
    int32_t undo;
} ActionData;

/*
 */
typedef struct LineEditData_ {
    int32_t id;
    char groupbox[MAX_STRING_LENGTH];
    char key[MAX_STRING_LENGTH];
    char icon[MAX_STRING_LENGTH];
    char label[MAX_STRING_LENGTH];
    char type[MAX_STRING_LENGTH];
    char map_signal[MAX_STRING_LENGTH];
} LineEditData;

/*
 */
typedef struct SpinBoxEditData_ {
    int32_t id;
    char groupbox[MAX_STRING_LENGTH];
    char key[MAX_STRING_LENGTH];
    char icon[MAX_STRING_LENGTH];
    char label[MAX_STRING_LENGTH];
    char type[MAX_STRING_LENGTH];
    char map_signal[MAX_STRING_LENGTH];
} SpinBoxEditData;

/*
 */
typedef struct Setting_ {
    int32_t id;
    char key[MAX_STRING_LENGTH];
    char value[MAX_STRING_LENGTH];
    int type;
} Setting;

/*
 */
typedef struct GeometryData_ {
    EmbVector vector;
} GeometryData;

/* To allow us to resize general C arrays when necessary.
 * Note that for char arrays, the buffer is a normal c-style string.
 */
typedef struct Cvector_ {
    char *buffer;
    int32_t size;
    int32_t max_length;
    int32_t element_size;
} Cvector;

/*
 */
typedef struct ToolbarData_ {
    int32_t id;
    const char key[MAX_STRING_LENGTH];
    int32_t entries[MAX_TOOLBAR_LENGTH];
    char horizontal;
} ToolbarData;

/*
 */
typedef struct MenuData_ {
    int32_t id;
    const char key[MAX_STRING_LENGTH];
    int32_t entries[MAX_TOOLBAR_LENGTH];
} MenuData;

/*
 *
 */
struct Node_ {
    struct Node_ **leaves;
    int32_t n_leaves;
    int32_t max_leaves;
    char key[MAX_STRING_LENGTH];
    char data[MAX_STRING_LENGTH];
    char s[MAX_STRING_LENGTH];
    int32_t i;
    EmbReal r;
    Cvector *vec;
    int32_t type;
};

typedef struct Node_ Node;

/* Memory management. */
Cvector *cvector_create(size_t element_size);
void cvector_append(Cvector *a, Cvector *b);
void cvector_add_cstr(Cvector *a, char *b);
Cvector *cvector_copy(Cvector *a);
void cvector_free(Cvector *vector);
int string_array_length(const char *list[]);

Node *create_node(int type);
int add_leaf(Node *branch, Node *leaf);
void print_tree(Node *branch, int indent);
Node *find_node(Node *branch, char key[MAX_STRING_LENGTH]);
void free_node(Node *branch);
Node *create_and_add_leaf(Node *parent, char *key, char *value);
int insert_node(Node *branch, char key[MAX_STRING_LENGTH], Node *node);

/* Utility Functions. */
unsigned char validRGB(int r, int g, int b);
int str_contains(char *s, char c);
EmbReal fourier_series(EmbReal arg, EmbReal *terms, int n_terms);
bool willUnderflowInt32(int64_t a, int64_t b);
bool willOverflowInt32(int64_t a, int64_t b);
int roundToMultiple(bool roundUp, int numToRound, int multiple);
int tokenize(char **argv, char *str, const char delim);

/* Global memory. */
extern Node *root;

/* Geometry UI Processors */
GeometryData *geometry_init(int type);
void geometry_free(GeometryData *g);
void geometry_left_click(GeometryData *geometry, EmbVector v);
void geometry_prompt(
    GeometryData *geometry,
    char input[MAX_STRING_LENGTH],
    char output[MAX_STRING_LENGTH]);
void geometry_context(
    void *m,
    GeometryData *geometry,
    char output[MAX_STRING_LENGTH]);

/* The Settings System
 *
 * Rather than pollute the global namespace, we collect together all the global
 * settings into a structure that stores them. This also allows us to create a
 * complete copy of the settings for the purpose of restoring them if the user
 * cancels out of the Settings Dialog.
 */
extern const char *settings_labels[];
extern Node settings[SETTINGS_TOTAL], dialog[SETTINGS_TOTAL],
    preview[SETTINGS_TOTAL], accept_[SETTINGS_TOTAL];

extern const ActionData action_table[MAX_ACTIONS];
extern const LineEditData all_line_editors[MAX_EDITORS];
extern const SpinBoxEditData all_spinbox_editors[MAX_EDITORS];
extern Setting settings_data[];

extern const char *version;
extern const char *usage_msg;
extern const char *extensions[];
extern int general_props[];
extern int display_props[];
extern int prompt_props[];
extern int quick_snap_props[];
extern int opensave_props[];
extern const char *default_prompt_style[];
extern const char *details_labels[];
extern const char *command_labels[];
extern const char *justify_options[];
extern const char *object_names[];
extern const char *button_list[];
extern const char *tips[];

/* Menus data */
extern MenuData menu_data[MAX_MENUS];
extern int32_t menubar_order[];
extern int32_t file_menu[];
extern int32_t edit_menu[];
extern int32_t pan_menu[];
extern int32_t zoom_menu[];
extern int32_t view_menu[];
extern int32_t settings_menu[];
extern int32_t window_menu[];
extern int32_t help_menu[];
extern int32_t draw_menu[];

/* Toolbar data */
extern ToolbarData toolbar_data[MAX_TOOLBARS];
extern int32_t top_toolbar_layout[];
extern int32_t bottom_toolbar_layout[];
extern int32_t side_toolbar_layout[];

/* Property editor data */
extern const char *group_box_data[];
extern const char *group_box_types[];
extern const int32_t group_box_ids[];

#ifdef __cplusplus
}
#endif

#endif
