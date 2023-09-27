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

#define MAX_STRING_LENGTH                      200
#define MAX_HISTOGRAM_BINS                    1000

/* Node types. */
#define STRING_TYPE                              0
#define STRING_LIST_TYPE                         1
#define REAL_TYPE                                2
#define INT_TYPE                                 3
#define BOOL_TYPE                                4
#define FUNCTION_TYPE                            5
#define VECTOR_TYPE                              6
#define UNKNOWN_TYPE                             7

/* Actions.
 * These identifiers are subject to change since they are in alphabetical order
 * and the numebrs are increasing.
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
#define ACTION_APPEND_PROMPT_HISTORY            28
#define ACTION_CALCULATE_ANGLE                  29
#define ACTION_CALCULATE_DISTANCE               30
#define ACTION_CHANGELOG                        31
#define ACTION_CLEAR_RUBBER                     32
#define ACTION_CLEAR_SELECTION                  33
#define ACTION_COPY                             34
#define ACTION_COPY_SELECTED                    35
#define ACTION_CUT                              36
#define ACTION_CUT_SELECTED                     37
#define ACTION_DAY_VISION                       38
#define ACTION_DEBUG                            39
#define ACTION_DELETE_SELECTED                  40
#define ACTION_DESIGN_DETAILS                   41
#define ACTION_DO_NOTHING                       42
#define ACTION_END                              43
#define ACTION_ERROR                            44
#define ACTION_HELP                             45
#define ACTION_ICON                             46
#define ACTION_INIT                             47
#define ACTION_MESSAGEBOX                       48
#define ACTION_MIRROR_SELECTED                  49
#define ACTION_MOUSE_X                          50
#define ACTION_MOUSE_Y                          51
#define ACTION_MOVE_SELECTED                    52
#define ACTION_NEW                              53
#define ACTION_NIGHT_VISION                     54
#define ACTION_NUM_SELECTED                     55
#define ACTION_OPEN                             56
#define ACTION_PAN                              57
#define ACTION_PASTE                            58
#define ACTION_PASTE_SELECTED                   59
#define ACTION_PERPENDICULAR_DISTANCE           60
#define ACTION_PLATFORM                         61
#define ACTION_PREVIEW_OFF                      62
#define ACTION_PREVIEW_ON                       63
#define ACTION_PRINT                            64
#define ACTION_PRINT_AREA                       65
#define ACTION_QSNAP_X                          66
#define ACTION_QSNAP_Y                          67
#define ACTION_QUIT                             68
#define ACTION_REDO                             69
#define ACTION_ROTATE_SELECTED                  70
#define ACTION_RUBBER                           71
#define ACTION_SCALE_SELECTED                   72
#define ACTION_SELECT_ALL                       73
#define ACTION_SETTINGS_DIALOG                  74
#define ACTION_SET_BACKGROUND_COLOR             75
#define ACTION_SET_CROSSHAIR_COLOR              76
#define ACTION_SET_CURSOR_SHAPE                 77
#define ACTION_SET_GRID_COLOR                   78
#define ACTION_SET_PROMPT_PREFIX                79
#define ACTION_SET_RUBBER_FILTER                80
#define ACTION_SET_RUBBER_MODE                  81
#define ACTION_SET_RUBBER_POINT                 82
#define ACTION_SET_RUBBER_TEXT                  83
#define ACTION_SPARE_RUBBER                     84
#define ACTION_TIP_OF_THE_DAY                   85
#define ACTION_TODO                             86
#define ACTION_UNDO                             87
#define ACTION_VERSION                          88
#define ACTION_VULCANIZE                        89
#define ACTION_WHATS_THIS                       90
#define ACTION_WINDOW                           91
#define ACTION_ZOOM                             92

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

/*
#define ARC_CIRCUMFERENCE                    9
#define ARC_POSITION                        10
#define OBJECT_TEXT                        12
#define OBJECT_TEXT_FONT                    13
#define OBJECT_TextJustify(QString justify);
#define OBJECT_TextSize(EmbReal size);
#define OBJECT_TextStyle(bool bold, bool italic, bool under, bool strike, bool over);
#define OBJECT_TextBold(bool val);
#define OBJECT_TextItalic(bool val);
#define OBJECT_TextUnderline(bool val);
#define OBJECT_TextStrikeOut(bool val);
#define OBJECT_TextOverline(bool val);
#define OBJECT_TextBackward(bool val);
#define OBJECT_TextUpsideDown(bool val);
#define OBJECT_RadiusMajor(EmbReal radius);
#define OBJECT_RadiusMinor(EmbReal radius);
#define OBJECT_DiameterMajor(EmbReal diameter);
#define OBJECT_DiameterMinor(EmbReal diameter);
*/

/* Per object constants. */
#define POINTS_PER_BASE_OBJECT                 100
#define REALS_PER_BASE_OBJECT                  100
#define BOOLS_PER_BASE_OBJECT                  100
#define STRINGS_PER_BASE_OBJECT                 10

#ifdef __cplusplus
extern "C" {
#endif

#include "../extern/libembroidery/src/embroidery.h"
#include "../extern/tomlc99/toml.h"

typedef struct ActionData_ {
    char type[MAX_STRING_LENGTH];
    char icon[MAX_STRING_LENGTH];
    char command[MAX_STRING_LENGTH];
    char tooltip[MAX_STRING_LENGTH];
    char statustip[MAX_STRING_LENGTH];
} ActionData;

unsigned char validRGB(int r, int g, int b);

extern const ActionData action_table[];
extern const char default_prompt_style[14][MAX_STRING_LENGTH];
extern const char details_labels[12][MAX_STRING_LENGTH];
extern const char *usage_msg;

#ifdef __cplusplus
}
#endif

#endif
