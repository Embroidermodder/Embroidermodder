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
#define MAX_COMMANDS                           256
#define MAX_ICONS                              256
#define MAX_SETTINGS                           256
#define MAX_POSITIONS                           26
#define MAX_EDITORS                            300
#define MAX_ARGS                                20
#define MAX_COMBOBOXES                         200

#define WIDGET_GROUPBOX                          0
#define WIDGET_LINEEDIT                          1
#define WIDGET_CHECKBOX                          2
#define WIDGET_SPINBOX                           3
#define WIDGET_DOUBLE_SPINBOX                    4
#define WIDGET_LABEL                             5
#define WIDGET_COMBOBOX                          6
#define WIDGET_TOOLBUTTON                        7
#define WIDGET_ACTION                            8
#define WIDGET_TOOLBAR                           9
#define WIDGET_MENU                             10

#define SUBCOMMAND_ARC                       0
#define SUBCOMMAND_CIRCLE                    1
#define SUBCOMMAND_ELLIPSE                   2
#define SUBCOMMAND_HORIZONTAL_DIMENSION      3
#define SUBCOMMAND_IMAGE                     4
#define SUBCOMMAND_PATH                      5
#define SUBCOMMAND_POLYGON                   6
#define SUBCOMMAND_POLYLINE                  7
#define SUBCOMMAND_RECTANGLE                 8
#define SUBCOMMAND_REGULAR_POLYGON           9
#define SUBCOMMAND_VERTICAL_DIMENSION       10
#define SUBCOMMAND_DIM_LEADER               11
#define SUBCOMMAND_INFINITE_LINE            12
#define SUBCOMMAND_RAY                      13
#define SUBCOMMAND_LINE                     14
#define SUBCOMMAND_TRIANGLE                 15
#define SUBCOMMAND_TEXT_MULTI               16
#define SUBCOMMAND_TEXT_SINGLE              17
#define SUBCOMMAND_ROUNDED_RECTANGLE        18
#define SUBCOMMAND_POINT                    19
#define SUBCOMMAND_SLOT                     20

/* Actions.
 * These identifiers are subject to change since they are in alphabetical order
 * and the numbers are increasing.
 */
#define COMMAND_ABOUT                             0
#define COMMAND_ADD                               1
#define COMMAND_ADD_RUBBER                       18
#define COMMAND_ADD_SLOT                         19
#define COMMAND_ADD_TEXT_MULTI                   20
#define COMMAND_ADD_TEXT_SINGLE                  21
#define COMMAND_ADD_TO_SELECTION                 22
#define COMMAND_ADD_TRIANGLE                     23
#define COMMAND_ADD_VERTICAL_DIMENSION           24
#define COMMAND_ALERT                            25
#define COMMAND_ALLOW_RUBBER                     26
#define COMMAND_APPEND_HISTORY                   27
#define COMMAND_CALCULATE_ANGLE                  28
#define COMMAND_CALCULATE_DISTANCE               29
#define COMMAND_CHANGELOG                        30
#define COMMAND_CLEAR_RUBBER                     31
#define COMMAND_CLEAR_SELECTION                  32
#define COMMAND_COPY                             33
#define COMMAND_COPY_SELECTED                    34
#define COMMAND_CUT                              35
#define COMMAND_CUT_SELECTED                     36
#define COMMAND_DAY                              37
#define COMMAND_DEBUG                            38
#define COMMAND_DELETE_SELECTED                  39
#define COMMAND_DESIGN_DETAILS                   40
#define COMMAND_DO_NOTHING                       41
#define COMMAND_END                              42
#define COMMAND_ERROR                            43
#define COMMAND_HELP                             44
#define COMMAND_ICON                             45
#define COMMAND_INIT                             46
#define COMMAND_MESSAGEBOX                       47
#define COMMAND_MIRROR_SELECTED                  48
#define COMMAND_MOUSE_X                          49
#define COMMAND_MOUSE_Y                          50
#define COMMAND_MOVE_SELECTED                    51
#define COMMAND_NEW                              52
#define COMMAND_NIGHT                            53
#define COMMAND_NUM_SELECTED                     54
#define COMMAND_OPEN                             55
#define COMMAND_PAN                              56
#define COMMAND_PASTE                            57
#define COMMAND_PASTE_SELECTED                   58
#define COMMAND_PERPENDICULAR_DISTANCE           59
#define COMMAND_PLATFORM                         60
#define COMMAND_PREVIEW_OFF                      61
#define COMMAND_PREVIEW_ON                       62
#define COMMAND_PRINT                            63
#define COMMAND_PRINT_AREA                       64
#define COMMAND_QSNAP_X                          65
#define COMMAND_QSNAP_Y                          66
#define COMMAND_EXIT                             67
#define COMMAND_REDO                             68
#define COMMAND_ROTATE_SELECTED                  69
#define COMMAND_RUBBER                           70
#define COMMAND_SCALE_SELECTED                   71
#define COMMAND_SELECT_ALL                       72
#define COMMAND_SETTINGS_DIALOG                  73
#define COMMAND_SET                              74
#define COMMAND_SPARE_RUBBER                     75
#define COMMAND_TIP_OF_THE_DAY                   76
#define COMMAND_TODO                             77
#define COMMAND_UNDO                             78
#define COMMAND_VERSION                          79
#define COMMAND_VULCANIZE                        80
#define COMMAND_WHATS_THIS                       81
#define COMMAND_WINDOW                           82
#define COMMAND_ZOOM                             83
#define COMMAND_TEST                             84
#define COMMAND_SLEEP                            85
#define COMMAND_LAYER_EDITOR                     86
#define COMMAND_MAKE_LAYER_CURRENT               87
#define COMMAND_TEXT                             88
#define COMMAND_LAYER_PREVIOUS                   89
#define COMMAND_SAVE                             90
#define COMMAND_SAVEAS                           91
#define COMMAND_LAYERS                           94
#define COMMAND_LAYER_SELECTOR                   95
#define COMMAND_TREBLECLEF                       96
#define COMMAND_COLOR_SELECTOR                   97
#define COMMAND_LINE_TYPE_SELECTOR               98
#define COMMAND_LINE_WEIGHT_SELECTOR             99
#define COMMAND_HIDE_ALL_LAYERS                 100
#define COMMAND_ADD_HEART                       101
#define COMMAND_ADD_SINGLE_LINE_TEXT            102
#define COMMAND_SHOW_ALL_LAYERS                 103
#define COMMAND_FREEZE_ALL_LAYERS               104
#define COMMAND_THAW_ALL_LAYERS                 105
#define COMMAND_LOCK_ALL_LAYERS                 106
#define COMMAND_UNLOCK_ALL_LAYERS               107
#define COMMAND_ADD_DOLPHIN                     108
#define COMMAND_ADD_DISTANCE                    109
#define COMMAND_LOCATE_POINT                    110
#define COMMAND_QUICKSELECT                     111
#define COMMAND_SPELLCHECK                      112
#define COMMAND_DISTANCE                        113
#define COMMAND_MOVE                            114
#define COMMAND_QUICKLEADER                     115
#define COMMAND_RGB                             116
#define COMMAND_ROTATE                          117
#define COMMAND_SANDBOX                         118
#define COMMAND_ADD_SNOWFLAKE                   119
#define COMMAND_ADD_STAR                        120
#define COMMAND_DELETE                          121
#define COMMAND_SCALE                           122
#define COMMAND_SINGLE_LINE_TEXT                123
#define COMMAND_SYSWINDOWS                      124
#define COMMAND_ENABLE                          125
#define COMMAND_DISABLE                         126
#define COMMAND_SET_COLOR                       127
#define COMMAND_SET_BACKGROUND_COLOR            128
#define COMMAND_SET_CROSSHAIR_COLOR             129
#define COMMAND_SET_CURSOR_SHAPE                130
#define COMMAND_SET_GRID_COLOR                  131
#define COMMAND_SET_PROMPT_PREFIX               132
#define COMMAND_SET_RUBBER_FILTER               133
#define COMMAND_SET_RUBBER_MODE                 134
#define COMMAND_SET_RUBBER_POINT                135
#define COMMAND_SET_RUBBER_TEXT                 136
#define N_COMMANDS                              137

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

/* Rubber modes */
#define RUBBER_CIRCLE_1P_RAD                     0
#define RUBBER_CIRCLE_1P_DIA                     1
#define RUBBER_CIRCLE_2P                         2
#define RUBBER_CIRCLE_3P                         3
#define RUBBER_CIRCLE_TTR                        5
#define RUBBER_CIRCLE_TTT                        6
#define RUBBER_DIMLEADER_LINE                    7
#define RUBBER_ELLIPSE_LINE                      8
#define RUBBER_ELLIPSE_MAJORDIAMETER_MINORDIAMETER 9
#define RUBBER_ELLIPSE_MAJORRADIUS_MINORRADIUS  10
#define RUBBER_ELLIPSE_ROTATION                 11
#define RUBBER_LINE                             12
#define RUBBER_POLYGON                          13
#define RUBBER_POLYGON_INSCRIBE                 14
#define RUBBER_POLYGON_CIRCUMSCRIBE             15
#define RUBBER_POLYLINE                         16
#define RUBBER_RECTANGLE                        17
#define RUBBER_TEXTSINGLE                       18
#define N_RUBBER_MODES                          40

/* Justify */
#define JUSTIFY_LEFT                             0
#define JUSTIFY_CENTER                           1
#define JUSTIFY_RIGHT                            2
#define JUSTIFY_ALIGNED                          3
#define JUSTIFY_MIDDLE                           4
#define JUSTIFY_FIT                              5
#define JUSTIFY_TOP_LEFT                         6
#define JUSTIFY_TOP_CENTER                       7
#define JUSTIFY_TOP_RIGHT                        8
#define JUSTIFY_MIDDLE_LEFT                      9
#define JUSTIFY_MIDDLE_CENTER                   10
#define JUSTIFY_MIDDLE_RIGHT                    11
#define JUSTIFY_BOTTOM_LEFT                     12
#define JUSTIFY_BOTTOM_CENTER                   13
#define JUSTIFY_BOTTOM_RIGHT                    14
#define TOTAL_JUSTIFY                           15

/* User Interface Mode */
#define MODE_ARC                                 0
#define MODE_CIRCLE_1P_RAD                       1
#define MODE_CIRCLE_1P_DIA                       2
#define MODE_CIRCLE_2P                           3
#define MODE_CIRCLE_3P                           4
#define MODE_CIRCLE_TTR                          5
#define MODE_CIRCLE_TTR_SET_POINT_2              6
#define MODE_CIRCLE_TTR_SET_POINT_3              7
#define MODE_DISTANCE                            8
#define MODE_ELLIPSE                             9
#define MODE_RECTANGLE                          10
#define MODE_STAR_INIT                          100
#define MODE_STAR_NUM_POINTS                    11
#define MODE_STAR_CENTER_PT                     12
#define MODE_STAR_RAD_INNER                     13
#define MODE_STAR_RAD_OUTER                     14
#define MODE_POLYGON_NUM_SIDES                  15
#define MODE_POLYGON_POLYTYPE                   16
#define MODE_POLYGON_DISTANCE                   17
#define MODE_POLYGON_CENTER_PT                  18
#define MODE_POLYGON_INSCRIBE                   19
#define MODE_POLYGON_CIRCUMSCRIBE               20
#define MODE_POLYGON_SIDE_LEN                   21
#define MODE_POLYGON_INIT                       22
#define MODE_POINT_INIT                         24
#define MODE_RGB                                25
#define MODE_LINE                               26
#define MODE_ROTATE                             27
#define MODE_SCALE                              28
#define MODE_TEXT_SINGLE_SETGEOM                29
#define MODE_PATH_INIT                          30
#define MODE_POLYLINE_INIT                      31
#define MODE_MOVE                               32
#define MODE_QUICKLEADER_INIT                   34
#define MODE_SNOWFLAKE_INIT                     35
#define MODE_SNOWFLAKE_NUM_POINTS               36
#define MODE_LOCATE_POINT                       37
#define MODE_ELLIPSE_MAJORDIAMETER_MINORRADIUS  38
#define MODE_DOLPHIN_INIT                       39
#define MODE_DOLPHIN_NUM_POINTS                 40
#define MODE_ERASE                              41
#define TOTAL_MODES                                 42

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
#define GB_GEOM_ARC                              1
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

/* Editor keys */
#define ED_GENERAL_LAYER                         0
#define ED_GENERAL_COLOR                         1
#define ED_GENERAL_LINE_TYPE                     2
#define ED_GENERAL_LINE_WEIGHT                   3
#define ED_ARC_CENTER_X                          4
#define ED_ARC_CENTER_Y                          5
#define ED_ARC_RADIUS                            6
#define ED_ARC_START_ANGLE                       7
#define ED_ARC_END_ANGLE                         8
#define ED_ARC_START_X                           9
#define ED_ARC_START_Y                          10
#define ED_ARC_END_X                            11
#define ED_ARC_END_Y                            12
#define ED_ARC_AREA                             13
#define ED_ARC_LENGTH                           14
#define ED_ARC_CHORD                            15
#define ED_ARC_INC_ANGLE                        16
#define ED_ARC_CLOCKWISE                        17
#define ED_BLOCK_X                              18
#define ED_BLOCK_Y                              19
#define ED_CIRCLE_RADIUS                        15
#define ED_CIRCLE_DIAMETER                      16
#define ED_CIRCLE_CENTER_X                      17
#define ED_CIRCLE_CENTER_Y                      18
#define ED_CIRCLE_AREA                          21
#define ED_CIRCLE_CIRCUMFERENCE                 22
#define ED_DIM_ALIGNED_X                        23
#define ED_DIM_ANGULAR_X                        24
#define ED_DIM_ARC_LENGTH_X                     25
#define ED_DIM_DIAMETER_X                       26
#define ED_DIM_LEADER_X                         27
#define ED_DIM_LINEAR_X                         28
#define ED_ELLIPSE_CENTER_X                     29
#define ED_ELLIPSE_CENTER_Y                     30
#define ED_ELLIPSE_RADIUS_MAJOR                 31
#define ED_ELLIPSE_RADIUS_MINOR                 32
#define ED_ELLIPSE_DIAMETER_MAJOR               33
#define ED_ELLIPSE_DIAMETER_MINOR               34
#define ED_TEXT_SINGLE_HEIGHT                   35
#define ED_TEXT_SINGLE_ROTATION                 36
#define ED_TEXT_SINGLE_X                        37
#define ED_TEXT_SINGLE_Y                        38
#define ED_TEXT_SINGLE_BACKWARD                 39
#define ED_TEXT_SINGLE_UPSIDE_DOWN              40
#define ED_TEXT_MULTI_X                         41
#define ED_TEXT_MULTI_Y                         42
#define ED_LINE_START_X                         43
#define ED_LINE_START_Y                         44
#define ED_LINE_END_X                           45
#define ED_LINE_END_Y                           46
#define ED_LINE_POINT_X                         47
#define ED_LINE_POINT_Y                         48
#define ED_TEXT_SINGLE_CONTENTS                 49
#define ED_TEXT_SINGLE_FONT                     50
#define ED_TEXT_SINGLE_JUSTIFY                  51
#define ED_RECTANGLE_CORNER1_X                  52
#define ED_RECTANGLE_CORNER1_Y                  53
#define ED_RECTANGLE_CORNER2_X                  54
#define ED_RECTANGLE_CORNER2_Y                  55
#define ED_RECTANGLE_CORNER3_X                  56
#define ED_RECTANGLE_CORNER3_Y                  57
#define ED_RECTANGLE_CORNER4_X                  58
#define ED_RECTANGLE_CORNER4_Y                  59
#define ED_RECTANGLE_WIDTH                      58
#define ED_RECTANGLE_HEIGHT                     59
#define ED_RECTANGLE_AREA                       50
#define ED_RAY_X1                               61
#define ED_RAY_Y1                               62
#define ED_RAY_X2                               63
#define ED_RAY_Y2                               64
#define ED_RAY_VECTOR_X                         65
#define ED_RAY_VECTOR_Y                         66
#define ED_GEOMETRY_DIM_LINEAR_X                67
#define ED_GEOM_DIM_ORDINATE_X                  68
#define ED_DIM_RADIUS_X                         69
#define ED_ELLIPSE_SEMI_MINOR_AXIS              70
#define ED_ELLIPSE_SEMI_MAJOR_AXIS              71
#define ED_ELLIPSE_MAJOR_AXIS                   72
#define ED_ELLIPSE_MINOR_AXIS                   73
#define ED_IMAGE_X                              74
#define ED_IMAGE_Y                              75
#define ED_IMAGE_WIDTH                          76
#define ED_IMAGE_HEIGHT                         77
#define ED_IMAGE_NAME                           78
#define ED_IMAGE_PATH                           79
#define ED_INFINITE_LINE_X1                     80
#define ED_INFINITE_LINE_Y1                     81
#define ED_INFINITE_LINE_X2                     82
#define ED_INFINITE_LINE_Y2                     83
#define ED_INFINITE_LINE_VECTOR_X               84
#define ED_INFINITE_LINE_VECTOR_Y               85
#define ED_LINE_DELTA_X                         86
#define ED_LINE_DELTA_Y                         87
#define ED_LINE_ANGLE                           88
#define ED_LINE_LENGTH                          89
#define ED_PATH_VERTEX_NUM                      90
#define ED_PATH_VERTEX_X                        91
#define ED_PATH_VERTEX_Y                        92
#define ED_PATH_AREA                            93
#define ED_PATH_LENGTH                          94
#define ED_PATH_CLOSED                          95
#define ED_POINT_X                              96
#define ED_POINT_Y                              97
#define ED_POLYGON_CENTER_X                     98
#define ED_POLYGON_CENTER_Y                     99
#define ED_POLYGON_RADIUS_VERTEX               100
#define ED_POLYGON_RADIUS_SIDE                 101
#define ED_POLYGON_DIAMETER_VERTEX             102
#define ED_POLYGON_DIAMETER_SIDE               103
#define ED_POLYGON_INTERIOR_ANGLE              104
#define ED_POLYLINE_VERTEX_NUM                 105
#define ED_POLYLINE_VERTEX_X                   106
#define ED_POLYLINE_VERTEX_Y                   107
#define ED_POLYLINE_VERTEX_AREA                108
#define ED_POLYLINE_VERTEX_LENGTH              109
#define ED_POLYLINE_VERTEX_CLOSED              110
#define TOTAL_EDITORS                          111

/* Editor types */
#define EDITOR_DOUBLE                            0
#define EDITOR_COMBOBOX                          1
#define EDITOR_FONT                              2
#define EDITOR_INT                               3
#define EDITOR_STRING                            4
#define EDITOR_CHECKBOX                          5

#define SNAP_POINT_TYPES                        13

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

/* Command Prompt Style */
#define CONSOLE_STYLE_COLOR                      0
#define CONSOLE_STYLE_BG_COLOR                   1
#define CONSOLE_STYLE_SELECTION_COLOR            2
#define CONSOLE_STYLE_SELECTION_BG_COLOR         3
#define CONSOLE_STYLE_FONT_FAMILY                4
#define CONSOLE_STYLE_FONT_STYLE                 5
#define CONSOLE_STYLE_FONT_SIZE                  6

/* Statusbar buttons */
#define STATUSBAR_SNAP                           0
#define STATUSBAR_GRID                           1
#define STATUSBAR_RULER                          2
#define STATUSBAR_ORTHO                          3
#define STATUSBAR_POLAR                          4
#define STATUSBAR_QSNAP                          5
#define STATUSBAR_QTRACK                         6
#define STATUSBAR_LWT                            7

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

#include <stdio.h>
#include <stdbool.h>
#include <inttypes.h>

#include "geometry.h"

/* . */
typedef struct MenuData_ {
    int32_t id;
    const char key[MAX_STRING_LENGTH];
    int32_t entries[MAX_TOOLBAR_LENGTH];
} MenuData;

/* . */
typedef struct ToolbarData_ {
    int32_t id;
    const char key[MAX_STRING_LENGTH];
    int32_t entries[MAX_TOOLBAR_LENGTH];
    char horizontal;
} ToolbarData;

/* . */
typedef struct ActionData_ {
    int32_t id;
    char icon[MAX_STRING_LENGTH];
    char command[MAX_STRING_LENGTH];
    char tooltip[MAX_STRING_LENGTH];
    char statustip[MAX_STRING_LENGTH];
    char shortcut[MAX_STRING_LENGTH];
} ActionData;

/* . */
typedef struct CommandData_ {
    int32_t id;
    char command[MAX_STRING_LENGTH];
    int32_t min_args;
    int32_t gview;
    int32_t gscene;
    int32_t undo;
} CommandData;

/* . */
typedef struct EditorData_ {
    int32_t groupbox;
    int32_t key;
    char icon[MAX_STRING_LENGTH];
    char label[MAX_STRING_LENGTH];
    int32_t type;
    char map_signal[MAX_STRING_LENGTH];
} EditorData;

/* . */
typedef struct Setting_ {
    int32_t id;
    char key[MAX_STRING_LENGTH];
    char value[MAX_STRING_LENGTH];
    int type;
} Setting;

/* . */
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
} ViewData;

/* . */
typedef struct RubberPoint_ {
    char key[MAX_STRING_LENGTH];
    char text[MAX_STRING_LENGTH];
    EmbVector position;
} RubberPoint;

/* . */
typedef struct UndoData_ {
    EmbVector pivot;
    EmbVector before;
} UndoData;

/* . */
typedef struct Node_ {
    char s[MAX_STRING_LENGTH];
    int32_t i;
    EmbReal r;
} Node;

/* . */
typedef struct WidgetData_ {
    int type;
    char label[MAX_STRING_LENGTH];
    char icon[MAX_STRING_LENGTH];
    EmbReal single_step;
    EmbReal lower;
    EmbReal upper;
    int key;
    int position[2];
} WidgetData;

void debug_message(char *msg, ...);
int read_settings(void);
void write_settings(void);
EmbVector rotate_vector(EmbVector v, EmbReal alpha);
const char *actuator(char string[MAX_STRING_LENGTH]);
int string_equal(const char *a, const char *b);
void emb_sleep(int seconds);
int string_array_length(const char *list[]);
bool save_current_file(const char *fileName);

const char *run_script_file(char *fname);
const char *run_script(char **script);
const char *translate(char *str);

/* Utility Functions. */
unsigned char validRGB(int r, int g, int b);
int str_contains(char *s, char c);
EmbReal fourier_series(EmbReal arg, EmbReal *terms, int n_terms);
bool willUnderflowInt32(int64_t a, int64_t b);
bool willOverflowInt32(int64_t a, int64_t b);
int roundToMultiple(bool roundUp, int numToRound, int multiple);
int tokenize(char **argv, char *str, const char delim);
void emb_sleep(int seconds);
char *platformString(void);
void get_n_reals(float result[], char *argv[], int n, int offset);
void clear_selection(void);
void prompt_output(char *s);

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

extern ActionData action_table[MAX_ACTIONS];
extern CommandData command_table[MAX_COMMANDS];
extern EditorData all_line_editors[MAX_EDITORS];
extern EditorData all_spinbox_editors[MAX_EDITORS];
extern Setting settings_data[];

/* Properties */
extern int general_props[];
extern int display_props[];
extern int prompt_props[];
extern int quick_snap_props[];
extern int opensave_props[];
extern int selection_props[];
extern int grid_ruler_props[];
extern int accept_preview_props[];
extern int accept_accept_props[];
extern const char *load_from_file_enabled[];
extern const char *grid_type_visibility_lattice[];
extern const char *grid_type_visibility_circular[];
extern const char *tab_names[];

/* Other strings and string tables. */
extern const char *version;
extern const char *usage_msg;
extern const char *extensions[];
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
extern const int32_t group_box_ids[];

extern char *coverage_test_script[];
extern const char *geometry_subcommands[];

/* Widget data */
extern WidgetData grid_geometry_widgets[];
extern WidgetData qsnap_mode_checkboxes[];

#ifdef __cplusplus
}
#endif

#endif
