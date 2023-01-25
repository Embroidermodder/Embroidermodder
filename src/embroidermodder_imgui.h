/*
 *  Embroidermodder 2.
 *
 *  ------------------------------------------------------------
 *
 *  Copyright 2013-2022 The Embroidermodder Team
 *  Embroidermodder 2 is Open Source Software.
 *  See LICENSE for licensing terms.
 *
 *  ------------------------------------------------------------
 *
 *  Use Python's PEP7 style guide.
 *      https://peps.python.org/pep-0007/
 */

#ifndef EMBROIDERMODDER_IMGUI_H
#define EMBROIDERMODDER_IMGUI_H

#define VERSION           "2.0.0-alpha"

#include <iostream>
#include <vector>
#include <unordered_map>

#include <toml.hpp>
#include <embroidery.h>

#define LANGUAGE_DEFAULT              0
#define LANGUAGE_ENGLISH_US           1
#define LANGUAGE_ENGLISH_UK           2
#define LANGUAGE_FRENCH               3
#define LANGUAGE_SPANISH              4
#define LANGUAGE_HINDI                5
#define LANGUAGE_CHINESE_TRADITIONAL  6
#define LANGUAGE_CHINESE_SIMPLIFIED   7
#define LANGUAGE_ARABIC               8
#define LANGUAGE_RUSSIAN              9
#define N_LANGUAGES                  10

#define ACTION_donothing              0
#define ACTION_new                    1
#define ACTION_open                   2
#define ACTION_save                   3
#define ACTION_saveas                 4
#define ACTION_print                  5
#define ACTION_designdetails          6
#define ACTION_exit                   7
#define ACTION_cut                    8
#define ACTION_copy                   9
#define ACTION_paste                 10
#define ACTION_undo                  11
#define ACTION_redo                  12
// Window Menu
#define ACTION_windowclose           13
#define ACTION_windowcloseall        14
#define ACTION_windowcascade         15
#define ACTION_windowtile            16
#define ACTION_windownext            17
#define ACTION_windowprevious        18
// Help Menu
#define ACTION_help                  19
#define ACTION_changelog             20
#define ACTION_tipoftheday           21
#define ACTION_about                 22
#define ACTION_whatsthis             23
// Icons
#define ACTION_icon16                24
#define ACTION_icon24                25
#define ACTION_icon32                26
#define ACTION_icon48                27
#define ACTION_icon64                28
#define ACTION_icon128               29
#define ACTION_settingsdialog        30
// Layer ToolBar
#define ACTION_makelayercurrent      31
#define ACTION_layers                32
#define ACTION_layerselector         33
#define ACTION_layerprevious         34
#define ACTION_colorselector         35
#define ACTION_linetypeselector      36
#define ACTION_lineweightselector    37
#define ACTION_hidealllayers         38
#define ACTION_showalllayers         39
#define ACTION_freezealllayers       40
#define ACTION_thawalllayers         41
#define ACTION_lockalllayers         42
#define ACTION_unlockalllayers       43
//Text ToolBar
#define ACTION_textbold              44
#define ACTION_textitalic            45
#define ACTION_textunderline         46
#define ACTION_textstrikeout         47
#define ACTION_textoverline          48
// Zoom ToolBar
#define ACTION_zoomrealtime          49
#define ACTION_zoomprevious          50
#define ACTION_zoomwindow            51
#define ACTION_zoomdynamic           52
#define ACTION_zoomscale             53
#define ACTION_zoomcenter            54
#define ACTION_zoomin                55
#define ACTION_zoomout               56
#define ACTION_zoomselected          57
#define ACTION_zoomall               58
#define ACTION_zoomextents           59
// Pan SubMenu
#define ACTION_panrealtime           60
#define ACTION_panpoint              61
#define ACTION_panleft               62
#define ACTION_panright              63
#define ACTION_panup                 64
#define ACTION_pandown               65
#define ACTION_day                   66
#define ACTION_night                 67
// TODO
#define ACTION_spellcheck            68
#define ACTION_quickselect           69
#define ACTION_null                  70

#define WIDGET_MODE_BLOCK             0
#define WIDGET_MODE_TEXT              1
#define WIDGET_MODE_IMAGE             2
#define WIDGET_MODE_SVG               3
#define WIDGET_MODE_BACKGROUND        4
#define WIDGET_MODE_SPINBOX           5
#define WIDGET_MODE_COMBOBOX          6
#define WIDGET_MODE_CONTAINER         7
#define WIDGET_MODE_PANEL             8
#define WIDGET_MODE_VIEW              9
#define WIDGET_MODE_ROOT             10

#define VIEW_STATE_SNAP          0x0000
#define VIEW_STATE_GRIP          0x0001
#define VIEW_STATE_RULER         0x0002
#define VIEW_STATE_ORTHO         0x0004
#define VIEW_STATE_POLAR         0x0008
#define VIEW_STATE_QSNAP         0x0010
#define VIEW_STATE_QTRACK        0x0020
#define VIEW_STATE_LWT           0x0040
#define VIEW_STATE_REAL          0x0080
#define VIEW_STATE_CLOSEABLE     0x0100
#define VIEW_STATE_USE_LOGO      0x0200
#define VIEW_STATE_USE_TEXTURE   0x0400
#define VIEW_STATE_USE_COLOR     0x0800
#define VIEW_STATE_GRID          0x1000

/* Permissions System (Not implemented)
 * ------------------------------------
 *
 * The permissions flag determines whether the user or the system can run this
 * action.
 */
#define PERMISSIONS_USER              0
#define PERMISSIONS_SYSTEM            1

/* Visibility */
#define INVISIBLE                     0
#define ALWAYS_VISIBLE                1
#define FILE_MENU                     2
#define EDIT_MENU                     3
#define VIEW_MENU                     4
#define SETTINGS_MENU                 5
#define WINDOW_MENU                   6
#define HELP_MENU                     7
#define RECENT_MENU                   8
#define ZOOM_MENU                     9
#define PAN_MENU                     10

#define MITER_JOIN                    0

#define DATA_TYPE_DOUBLE              0
#define DATA_TYPE_INT                 1
#define DATA_TYPE_STRING              2
#define DATA_TYPE_META                3
#define DATA_TYPE_TITLE               4
#define DATA_TYPE_GROUP_BOX           5
#define DATA_TYPE_END_MARKER         -1

#define LINE_STYLE_DASHED             0
#define LINE_STYLE_SOLID              1

#define BRUSH_STYLE_DASHED            0
#define BRUSH_STYLE_SOLID             1

#define DIRECTION_RIGHT               0
#define DIRECTION_LEFT                1

#define RENDER_UI                     0
#define RENDER_TEXT_EDITOR            1

/* object values
 * -------------
 *
 * The object flag describing which object
 * the action is intended for or "Null" for global effects
 * like duplicating a pattern.
 */
/* NOTE: Allow this enum to evaluate false */
#define OBJ_TYPE_NULL                 0
/* NOTE: Values >= 65536 ensure compatibility with qgraphicsitem_cast() */
#define OBJ_TYPE_BASE            100000
#define OBJ_TYPE_ARC             100001
#define OBJ_TYPE_BLOCK           100002
#define OBJ_TYPE_CIRCLE          100003
#define OBJ_TYPE_DIMALIGNED      100004
#define OBJ_TYPE_DIMANGULAR      100005
#define OBJ_TYPE_DIMARCLENGTH    100006
#define OBJ_TYPE_DIMDIAMETER     100007
#define OBJ_TYPE_DIMLEADER       100008
#define OBJ_TYPE_DIMLINEAR       100009
#define OBJ_TYPE_DIMORDINATE     100010
#define OBJ_TYPE_DIMRADIUS       100011
#define OBJ_TYPE_ELLIPSE         100012
#define OBJ_TYPE_ELLIPSEARC      100013
#define OBJ_TYPE_RUBBER          100014
#define OBJ_TYPE_GRID            100015
#define OBJ_TYPE_HATCH           100016
#define OBJ_TYPE_IMAGE           100017
#define OBJ_TYPE_INFINITELINE    100018
#define OBJ_TYPE_LINE            100019
#define OBJ_TYPE_PATH            100020
#define OBJ_TYPE_POINT           100021
#define OBJ_TYPE_POLYGON         100022
#define OBJ_TYPE_POLYLINE        100023
#define OBJ_TYPE_RAY             100024
#define OBJ_TYPE_RECTANGLE       100025
#define OBJ_TYPE_SLOT            100026
#define OBJ_TYPE_SPLINE          100027
#define OBJ_TYPE_TEXTMULTI       100028
#define OBJ_TYPE_TEXTSINGLE      100029
#define OBJ_TYPE_UNKNOWN         100030

/* CAD Linetypes
 * (CONT, CENTER, DOT, HIDDEN, PHANTOM, ZIGZAG)
 *
 * Embroidery Stitchtypes
 * RUNNING  ---------
 * SATIN    VVVVVVVVV
 * FISHBONE >>>>>>>>>
 */
#define OBJ_LTYPE_CONT                0
#define OBJ_LTYPE_CENTER              1
#define OBJ_LTYPE_DOT                 2
#define OBJ_LTYPE_HIDDEN              3
#define OBJ_LTYPE_PHANTOM             4
#define OBJ_LTYPE_ZIGZAG              5
#define OBJ_LTYPE_RUNNING             6
#define OBJ_LTYPE_SATIN               7
#define OBJ_LTYPE_FISHBONE            8

/* OBJ_LWT_VALUES
 * --------------
 */
#define OBJ_LWT_BYLAYER            (-2)
#define OBJ_LWT_BYBLOCK            (-1)
#define OBJ_LWT_DEFAULT               0
#define OBJ_LWT_01                    1
#define OBJ_LWT_02                    2
#define OBJ_LWT_03                    3
#define OBJ_LWT_04                    4
#define OBJ_LWT_05                    5
#define OBJ_LWT_06                    6
#define OBJ_LWT_07                    7
#define OBJ_LWT_08                    8
#define OBJ_LWT_09                    9
#define OBJ_LWT_10                   10
#define OBJ_LWT_11                   11
#define OBJ_LWT_12                   12
#define OBJ_LWT_13                   13
#define OBJ_LWT_14                   14
#define OBJ_LWT_15                   15
#define OBJ_LWT_16                   16
#define OBJ_LWT_17                   17
#define OBJ_LWT_18                   18
#define OBJ_LWT_19                   19
#define OBJ_LWT_20                   20
#define OBJ_LWT_21                   21
#define OBJ_LWT_22                   22
#define OBJ_LWT_23                   23
#define OBJ_LWT_24                   24

/* OBJ_SNAP_VALUES
 * ---------------
 * NOTE: _allow this enum to evaluate false
 */
#define OBJ_SNAP_NULL                 0
#define OBJ_SNAP_ENDPOINT             1
#define OBJ_SNAP_MIDPOINT             2
#define OBJ_SNAP_CENTER               3
#define OBJ_SNAP_NODE                 4
#define OBJ_SNAP_QUADRANT             5
#define OBJ_SNAP_INTERSECTION         6
#define OBJ_SNAP_EXTENSION            7
#define OBJ_SNAP_INSERTION            8
#define OBJ_SNAP_PERPENDICULAR        9
#define OBJ_SNAP_TANGENT             10
#define OBJ_SNAP_NEAREST             11
#define OBJ_SNAP_APPINTERSECTION     12
#define OBJ_SNAP_PARALLEL            13
#define SNAP_TYPES                   14

/* OBJ_RUBBER_VALUES
 * -----------------
 * NOTE: Allow this enum to evaluate false and true
 */
#define OBJ_RUBBER_OFF                0
#define OBJ_RUBBER_ON                 1
#define OBJ_RUBBER_CIRCLE_1P_RAD      2
#define OBJ_RUBBER_CIRCLE_1P_DIA      3
#define OBJ_RUBBER_CIRCLE_2P          4
#define OBJ_RUBBER_CIRCLE_3P          5
#define OBJ_RUBBER_CIRCLE_TTR         6
#define OBJ_RUBBER_CIRCLE_TTT         7
#define OBJ_RUBBER_DIMLEADER_LINE     8
#define OBJ_RUBBER_ELLIPSE_LINE       9
#define OBJ_RUBBER_ELLIPSE_MAJORDIAMETER_MINORRADIUS    10
#define OBJ_RUBBER_ELLIPSE_MAJORRADIUS_MINORRADIUS      11
#define OBJ_RUBBER_ELLIPSE_ROTATION  12
#define OBJ_RUBBER_GRIP              13
#define OBJ_RUBBER_LINE              14
#define OBJ_RUBBER_POLYGON           15
#define OBJ_RUBBER_POLYGON_INSCRIBE  16
#define OBJ_RUBBER_POLYGON_CIRCUMSCRIBE 17
#define OBJ_RUBBER_POLYLINE          18
#define OBJ_RUBBER_IMAGE             19
#define OBJ_RUBBER_RECTANGLE         20
#define OBJ_RUBBER_TEXTSINGLE        21
#define RUBBER_MODES                 22

/* SPARE_RUBBER_VALUES
 * -------------------
 * NOTE: Allow this enum to evaluate false.
 */
#define SPARE_RUBBER_OFF              0
#define SPARE_RUBBER_PATH             1
#define SPARE_RUBBER_POLYGON          2
#define SPARE_RUBBER_POLYLINE         3

/* PREVIEW_CLONE_VALUES
 * --------------------
 * NOTE: Allow this enum to evaluate false.
 */
#define PREVIEW_CLONE_NULL            0
#define PREVIEW_CLONE_SELECTED        1
#define PREVIEW_CLONE_RUBBER          2

/* PREVIEW_MODE_VALUES
 * -------------------
 */
#define PREVIEW_MODE_NULL             0
#define PREVIEW_MODE_OFF              0
#define PREVIEW_MODE_MOVE             1
#define PREVIEW_MODE_ROTATE           2
#define PREVIEW_MODE_SCALE            3

#define NUMBER_ARCHITECTURAL          0
#define NUMBER_DECIMAL                1
#define NUMBER_ENGINEERING            2
#define NUMBER_FRACTIONAL             3
#define NUMBER_SCIENTIFIC             4

#define DISABLE_GRID                  0
#define RECTANGULAR_GRID              1
#define CIRCULAR_GRID                 2
#define ISOMETRIC_GRID                3

// Custom Data used in QGraphicsItems

//     (int, const QVariant)
// I.E. object.setData(OBJ_TYPE, OBJ_TYPE_LINE);
// I.E. object.setData(OBJ_LAYER, "OUTLINE");
// I.E. object.setData(OBJ_COLOR, 123);
// I.E. object.setData(OBJ_LTYPE, OBJ_LTYPE_CONT);

// Keys
enum OBJ_KEYS {
OBJ_TYPE   = 0, //value type - int: See OBJ_TYPE_VALUES
OBJ_NAME   = 1, //value type - str: See OBJ_NAME_VALUES
OBJ_LAYER  = 2, //value type - str: "USER", "DEFINED", "STRINGS", etc...
OBJ_COLOR  = 3, //value type - int: 0-255 //TODO: Use color chart in formats/format-dxf.h for this
OBJ_LTYPE  = 4, //value type - int: See OBJ_LTYPE_VALUES
OBJ_LWT    = 5, //value type - int: 0-27
OBJ_RUBBER = 6  //value type - int: See OBJ_RUBBER_VALUES
};

//OBJ_NAME_VALUES
#define  OBJ_NAME_NULL          "Unknown"
#define  OBJ_NAME_BASE          "Base"
#define  OBJ_NAME_ARC           "Arc"
#define  OBJ_NAME_BLOCK         "Block"
#define  OBJ_NAME_CIRCLE        "Circle"
#define  OBJ_NAME_DIMALIGNED    "Aligned Dimension"
#define  OBJ_NAME_DIMANGULAR    "Angular Dimension"
#define  OBJ_NAME_DIMARCLENGTH  "Arc Length Dimension"
#define  OBJ_NAME_DIMDIAMETER   "Diameter Dimension"
#define  OBJ_NAME_DIMLEADER     "Leader Dimension"
#define  OBJ_NAME_DIMLINEAR     "Linear Dimension"
#define  OBJ_NAME_DIMORDINATE   "Ordinate Dimension"
#define  OBJ_NAME_DIMRADIUS     "Radius Dimension"
#define  OBJ_NAME_ELLIPSE       "Ellipse"
#define  OBJ_NAME_ELLIPSEARC    "Elliptical Arc"
#define  OBJ_NAME_RUBBER        "Rubber"
#define  OBJ_NAME_GRID          "Grid"
#define  OBJ_NAME_HATCH         "Hatch"
#define  OBJ_NAME_IMAGE         "Image"
#define  OBJ_NAME_INFINITELINE  "Infinite Line"
#define  OBJ_NAME_LINE          "Line"
#define  OBJ_NAME_PATH          "Path"
#define  OBJ_NAME_POINT         "Point"
#define  OBJ_NAME_POLYGON       "Polygon"
#define  OBJ_NAME_POLYLINE      "Polyline"
#define  OBJ_NAME_RAY           "Ray"
#define  OBJ_NAME_RECTANGLE     "Rectangle"
#define  OBJ_NAME_SLOT          "Slot"
#define  OBJ_NAME_SPLINE        "Spline"
#define  OBJ_NAME_TEXTMULTI     "Multi Line Text"
#define  OBJ_NAME_TEXTSINGLE    "Single Line Text"

#define  ENABLE_LWT         "ENABLE_LWT"
#define  ENABLE_REAL       "ENABLE_REAL"

#define  SCENE_QSNAP_POINT   "SCENE_QSNAP_POINT"
#define  SCENE_MOUSE_POINT   "SCENE_MOUSE_POINT"
#define  VIEW_MOUSE_POINT     "VIEW_MOUSE_POINT"
#define  RUBBER_ROOM               "RUBBER_ROOM"

#define  VIEW_COLOR_BACKGROUND  "VIEW_COLOR_BACKGROUND"
#define  VIEW_COLOR_CROSSHAIR   "VIEW_COLOR_CROSSHAIR"
#define  VIEW_COLOR_GRID        "VIEW_COLOR_GRID"

#define BOOL_VALUE   0
#define REAL_VALUE   1
#define INT_VALUE    2
#define STRING_VALUE 3

/* TYPEDEFS
 * -----------------------------------------------------------------------------
 */

typedef struct Action_ {
    int action;
    char command[80];
    char menu_label[80];
    char description[200];
} Action;

extern bool running;

#endif /* EMBROIDERMODDER_IMGUI_H */

