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
 *  Defines, typedefs and function prototypes.
 *
 */

#ifndef EMBROIDERMODDER__2__
#define EMBROIDERMODDER__2__

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

/* time, uname */

#ifdef __linux__
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>
#else
#include <SDL.h>
#include <SDL_image.h>
#include <SDL_ttf.h>
#endif

#include "../extern/TinyScheme/src/scheme-private.h"
#include "../extern/libembroidery/src/embroidery.h"

/*
 *  The data for configuring Embroidermodder 2, sets the default
 *  values should it fail to load the ini files.
 */

/* DEFINES
 * -------
 */
#define MAX_STRING_LENGTH           100
#define MAX_MENU_LENGTH              30
#define MAX_ACTIONS                 300
#define MAX_OBJECTS                 300
#define MAX_TOOLBARS                 30
#define MAX_TOOLBAR_ENTRIES          30
#define MAX_UNDO_HISTORY           1000
#define MAX_VARIABLES              1000
#define RECENT_FILES                 10

/* Data
 * ----
 *  The data for configuring Embroidermodder 2, sets the default
 *  values should it fail to load the ini files.
 */

#define COLOR_BACKGROUND              0
#define COLOR_CROSSHAIR               1
#define COLOR_GRID                    2

#define DATA_TYPE_DOUBLE              0
#define DATA_TYPE_INT                 1
#define DATA_TYPE_STRING              2
#define DATA_TYPE_META                3
#define DATA_TYPE_TITLE               4
#define DATA_TYPE_GROUP_BOX           5

#define LINE_STYLE_DASHED             0
#define LINE_STYLE_SOLID              1

#define BRUSH_STYLE_DASHED            0
#define BRUSH_STYLE_SOLID             1

#define DIRECTION_RIGHT               0
#define DIRECTION_LEFT                1

#define RENDER_UI                     0
#define RENDER_TEXT_EDITOR            1

#define TOOLBAR_FILE                  0
#define TOOLBAR_EDIT                  1
#define TOOLBAR_VIEW                  2
#define TOOLBAR_ZOOM                  3
#define TOOLBAR_PAN                   4
#define TOOLBAR_ICON                  5
#define TOOLBAR_HELP                  6
#define TOOLBAR_LAYER                 7
#define TOOLBAR_TEXT                  8
#define TOOLBAR_PROPERTIES            9
#define N_TOOLBARS                   10

#define FILE_MENU                     0
#define EDIT_MENU                     1
#define VIEW_MENU                     2
#define SETTINGS_MENU                 3
#define WINDOW_MENU                   4
#define HELP_MENU                     5
#define RECENT_MENU                   6
#define ZOOM_MENU                     7
#define PAN_MENU                      8
#define N_MENUS                       9

#define TAB_SNAP                      0
#define TAB_GRID                      1
#define TAB_RULER                     2
#define TAB_ORTHO                     3
#define TAB_POLAR                     4
#define TAB_QSNAP                     5
#define TAB_QTRACK                    6
#define TAB_LWT                       7

#define STATE_GRIPPING                0
#define STATE_RAPID_MOVE              1
#define STATE_PREVIEW                 2
#define STATE_PASTING                 3
#define STATE_MOVING                  4
#define STATE_SELECTING               5
#define STATE_ZOOM                    6
#define STATE_PANNING_REAL_TIME       7
#define STATE_PANNING_POINT           8
#define STATE_PANNING                 9
#define STATE_QSNAP                  10
#define N_STATES                     11

#define STATUS_SNAP                   0
#define STATUS_GRID                   1
#define STATUS_RULER                  2
#define STATUS_ORTHO                  3
#define STATUS_POLAR                  4
#define STATUS_QSNAP                  5
#define STATUS_QTRACK                 6
#define STATUS_LWT                    7
#define N_STATUS                      8

/* Permissions System (Not implemented)
 * ------------------------------------
 *
 * The permissions flag determines whether the user or
 * the system can run this action.
 */
#define PERMISSIONS_USER              0
#define PERMISSIONS_SYSTEM            1

/*
 * The mode argument determines what locations in
 * the interface the action will appear in,
 * for example in mode MODE_TOOLBAR,
 * the action appears in the toolbars,
 * in MODE_TOOLBAR | MODE_"double"
 * it also appears as a lineEdit in
 * the property editor expecting a double as input.
 */


/* LineEdits
 * TODO: make sure the lineedits array matches
 * this ordering.
 * ---------------------------------
 */
#define ARC_CENTER_X                  0
#define ARC_CENTER_Y                  1
#define ARC_RADIUS                    2
#define ARC_START_ANGLE               3
#define ARC_END_ANGLE                 4
#define ARC_START_X                   5
#define ARC_START_Y                   6
#define ARC_END_X                     7
#define ARC_END_Y                     8
#define ARC_AREA                      9
#define ARC_LENGTH                   10
#define ARC_CHORD                    11
#define ARC_INC_ANGLE                12
#define TEXT_SINGLE_CONTENTS         13
#define TEXT_SINGLE_HEIGHT           14
#define TEXT_SINGLE_ROTATION         15
#define TEXT_SINGLE_X      16
#define TEXT_SINGLE_Y      17
#define CIRCLE_CENTER_X      18
#define CIRCLE_CENTER_Y      19
#define CIRCLE_RADIUS      20
#define CIRCLE_DIAMETER             21
#define CIRCLE_AREA                 22
#define CIRCLE_CIRCUMFERENCE        23
#define ELLIPSE_CENTER_X            24
#define ELLIPSE_CENTER_Y            25
#define ELLIPSE_RADIUS_MAJOR        26
#define ELLIPSE_RADIUS_MINOR        27
#define ELLIPSE_DIAMETER_MAJOR      28
#define ELLIPSE_DIAMETER_MINOR      29
#define IMAGE_X      30
#define IMAGE_Y      31
#define IMAGE_WIDTH      32
#define IMAGE_HEIGHT      33
#define IMAGE_NAME      34
#define IMAGE_PATH      35
#define INFINITE_LINE_X1      36
#define INFINITE_LINE_Y1      37
#define INFINITE_LINE_X2      38
#define INFINITE_LINE_Y2      39
#define INFINITE_LINE_VECTOR_X      40
#define INFINITE_LINE_VECTOR_Y      41
#define BLOCK_X      42
#define BLOCK_Y      43
#define LINE_START_X      44
#define LINE_START_Y      45
#define LINE_END_X      46
#define LINE_END_Y      47
#define LINE_DELTA_X      48
#define LINE_DELTA_Y      49
#define LINE_ANGLE      50
#define LINE_LENGTH      51
#define POLYGON_CENTER_X      52
#define POLYGON_CENTER_Y      53
#define POLYGON_RADIUS_VERTEX      54
#define POLYGON_RADIUS_SIDE      55
#define POLYGON_DIAMETER_VERTEX      56
#define POLYGON_DIAMETER_SIDE      57
#define POLYGON_INTERIOR_ANGLE      58
#define RECT_CORNER_X1      59
#define RECT_CORNER_Y1      60
#define RECT_CORNER_X2      61
#define RECT_CORNER_Y2      62
#define RECT_CORNER_X3      63
#define RECT_CORNER_Y3      64
#define RECT_CORNER_X4      65
#define RECT_CORNER_Y4      66
#define RECT_HEIGHT      67
#define RECT_WIDTH      68
#define RECT_AREA      69
#define POINT_X      70
#define POINT_Y      71
#define LINEEDIT_PROPERTY_EDITORS      72

/* Comboboxes
 * ---------------------------------
 */
#define ARC_CLOCKWISE                  0
#define GENERAL_LAYER                  1
#define GENERAL_COLOR                  2
#define GENERAL_LINE_TYPE              3
#define GENERAL_LINE_WEIGHT            4
#define TEXT_SINGLE_FONT               5
#define TEXT_SINGLE_JUSTIFY            6
#define COMBOBOX_PROPERTY_EDITORS      7

#define PROPERTY_EDITORS \
    ( LINEEDIT_PROPERTY_EDITORS + COMBOBOX_PROPERTY_EDITORS )

/* Keys
 * ----
 * TYPE value type - int: See OBJ_TYPE_VALUES
 * NAME value type - str: See OBJ_NAME_VALUES
 * LAYER value type - str: "USER", "DEFINED", "STRINGS", etc...
 * COLOR value type - int: 0-255
 *      TODO: Use color chart in formats/format-dxf.h for this
 * LTYPE value type - int: See OBJ_LTYPE_VALUES
 * LWT value type - int: 0-27
 * RUBBER value type - int: See OBJ_RUBBER_VALUES
 */
#define OBJ_TYPE        0
#define OBJ_NAME        1
#define OBJ_LAYER       2
#define OBJ_COLOR       3
#define OBJ_LTYPE       4
#define OBJ_LWT         5
#define OBJ_RUBBER      6

/* object values
 * -------------
 *  
 * The object flag describing which object
 * the action is intended for or "Null" for global effects 
 * like duplicating a pattern.
 */
#define OBJ_TYPE_NULL            0
#define OBJ_TYPE_ARC             1
#define OBJ_TYPE_BLOCK           2
#define OBJ_TYPE_CIRCLE          3
#define OBJ_TYPE_DIMALIGNED      4
#define OBJ_TYPE_DIMANGULAR      5
#define OBJ_TYPE_DIMARCLENGTH    6
#define OBJ_TYPE_DIMDIAMETER     7
#define OBJ_TYPE_DIMLEADER       8
#define OBJ_TYPE_DIMLINEAR       9
#define OBJ_TYPE_DIMORDINATE    10
#define OBJ_TYPE_DIMRADIUS      11
#define OBJ_TYPE_ELLIPSE        12
#define OBJ_TYPE_ELLIPSEARC     13
#define OBJ_TYPE_RUBBER         14
#define OBJ_TYPE_GRID           15
#define OBJ_TYPE_HATCH          16
#define OBJ_TYPE_IMAGE          17
#define OBJ_TYPE_INFINITELINE   18
#define OBJ_TYPE_LINE           19
#define OBJ_TYPE_PATH           20
#define OBJ_TYPE_POINT          21
#define OBJ_TYPE_POLYGON        22
#define OBJ_TYPE_POLYLINE       23
#define OBJ_TYPE_RAY            24
#define OBJ_TYPE_RECTANGLE      25
#define OBJ_TYPE_SLOT           26
#define OBJ_TYPE_SPLINE         27
#define OBJ_TYPE_TEXTMULTI      28
#define OBJ_TYPE_TEXTSINGLE     29
#define OBJ_TYPE_UNKNOWN        30

/* CAD Linetypes
 * (CONT, CENTER, DOT, HIDDEN, PHANTOM, ZIGZAG)
 *
 * Embroidery Stitchtypes
 * RUNNING  ---------
 * SATIN    VVVVVVVVV
 * FISHBONE >>>>>>>>>
 */
#define OBJ_LTYPE_CONT         0
#define OBJ_LTYPE_CENTER       1
#define OBJ_LTYPE_DOT          2
#define OBJ_LTYPE_HIDDEN       3
#define OBJ_LTYPE_PHANTOM      4
#define OBJ_LTYPE_ZIGZAG       5
#define OBJ_LTYPE_RUNNING      6
#define OBJ_LTYPE_SATIN        7
#define OBJ_LTYPE_FISHBONE     8

/* OBJ_LWT_VALUES
 * --------------
 */
#define OBJ_LWT_BYLAYER      (-2)
#define OBJ_LWT_BYBLOCK      (-1)
#define OBJ_LWT_DEFAULT      0
#define OBJ_LWT_01      1
#define OBJ_LWT_02      2
#define OBJ_LWT_03      3
#define OBJ_LWT_04      4
#define OBJ_LWT_05      5
#define OBJ_LWT_06      6
#define OBJ_LWT_07      7
#define OBJ_LWT_08      8
#define OBJ_LWT_09      9
#define OBJ_LWT_10      10
#define OBJ_LWT_11      11
#define OBJ_LWT_12      12
#define OBJ_LWT_13      13
#define OBJ_LWT_14      14
#define OBJ_LWT_15      15
#define OBJ_LWT_16      16
#define OBJ_LWT_17      17
#define OBJ_LWT_18      18
#define OBJ_LWT_19      19
#define OBJ_LWT_20      20
#define OBJ_LWT_21      21
#define OBJ_LWT_22      22
#define OBJ_LWT_23      23
#define OBJ_LWT_24      24

/* OBJ_SNAP_VALUES
 * ---------------
 * NOTE: _allow this enum to evaluate false
 */
#define OBJ_SNAP_NULL      0
#define OBJ_SNAP_ENDPOINT      1
#define OBJ_SNAP_MIDPOINT      2
#define OBJ_SNAP_CENTER      3
#define OBJ_SNAP_NODE      4
#define OBJ_SNAP_QUADRANT      5
#define OBJ_SNAP_INTERSECTION      6
#define OBJ_SNAP_EXTENSION      7
#define OBJ_SNAP_INSERTION      8
#define OBJ_SNAP_PERPENDICULAR      9
#define OBJ_SNAP_TANGENT      10
#define OBJ_SNAP_NEAREST      11
#define OBJ_SNAP_APPINTERSECTION      12
#define OBJ_SNAP_PARALLEL      13
#define SNAP_TYPES      14

/* OBJ_RUBBER_VALUES
 * -----------------
 * NOTE: Allow this enum to evaluate false and true
 */
#define OBJ_RUBBER_OFF      0
#define OBJ_RUBBER_ON      1
#define OBJ_RUBBER_CIRCLE_1P_RAD      2
#define OBJ_RUBBER_CIRCLE_1P_DIA      3
#define OBJ_RUBBER_CIRCLE_2P      4
#define OBJ_RUBBER_CIRCLE_3P      5
#define OBJ_RUBBER_CIRCLE_TTR      6
#define OBJ_RUBBER_CIRCLE_TTT      7
#define OBJ_RUBBER_DIMLEADER_LINE      8
#define OBJ_RUBBER_ELLIPSE_LINE      9
#define OBJ_RUBBER_ELLIPSE_MAJORDIAMETER_MINORRADIUS      10
#define OBJ_RUBBER_ELLIPSE_MAJORRADIUS_MINORRADIUS      11
#define OBJ_RUBBER_ELLIPSE_ROTATION      12
#define OBJ_RUBBER_GRIP      13
#define OBJ_RUBBER_LINE      14
#define OBJ_RUBBER_POLYGON      15
#define OBJ_RUBBER_POLYGON_INSCRIBE      16
#define OBJ_RUBBER_POLYGON_CIRCUMSCRIBE      17
#define OBJ_RUBBER_POLYLINE      18
#define OBJ_RUBBER_IMAGE      19
#define OBJ_RUBBER_RECTANGLE      20
#define OBJ_RUBBER_TEXTSINGLE      21
#define RUBBER_MODES      22

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
 * NOTE: Allow this enum to evaluate false.
 */
#define PREVIEW_MODE_NULL             0
#define PREVIEW_MODE_MOVE             1
#define PREVIEW_MODE_ROTATE           2
#define PREVIEW_MODE_SCALE            3

#define LANGUAGE_ENGLISH              0
#define LANGUAGE_GERMAN               1
#define LANGUAGE_FRENCH               2
#define LANGUAGE_SPANISH              3
#define LANGUAGE_CHINESE_SIMPLIFIED   4
#define LANGUAGE_CHINESE_TRADITIONAL  5
#define LANGUAGE_ARABIC               6
#define LANGUAGE_RUSSIAN              7
#define LANGUAGE_DEFAULT      LANGUAGE_ENGLISH

#define WIDGET_MODE_BLOCK             0
#define WIDGET_MODE_TEXT              1
#define WIDGET_MODE_IMAGE             2
#define WIDGET_MODE_SVG               3
#define WIDGET_MODE_BACKGROUND        4

#define ARCHITECTURAL                 0
#define DECIMAL                       1
#define ENGINEERING                   2
#define FRACTIONAL                    3
#define SCIENTIFIC                    4

#define circle_mode_1P_RAD            0
#define circle_mode_1P_DIA            1
#define circle_mode_2P                2
#define circle_mode_3P                3
#define circle_mode_TTR               4

#define app_folder                    0
#define commands_folder               1
#define help_folder                   2
#define images_folder                 4
#define samples_folder                5
#define translations_folder           6
#define nFolders                      7

#define PATHS_MOVETO                  0
#define PATHS_LINETO                  1
#define PATHS_ARCTO                   2
#define PATHS_ARCMOVETO               3
#define PATHS_ELLIPSE                 4
#define PATHS_END                     5

#define N_TEXTURES                    2

#define INT_GENERAL_TIP_OF_THE_DAY   0
#define INT_DISPLAY_USE_OPENGL       1
#define INT_DISPLAY_RENDER_HINT_AA   2
#define INT_DISPLAY_RENDER_HINT_TEXT_AA    3
#define INT_DISPLAY_RENDER_HINT_SMOOTH_PIX 4
#define INT_DISPLAY_RENDER_HINT_HIGH_AA    5
#define INT_RENDER_HINT_NON_COSMETIC 6
#define INT_SCROLL_BAR_WIDGET_NUM    7
#define INT_PRINTING_DISABLE_BG      8
#define INT_GRID_SHOW_ON_LOAD        9
#define INT_GRID_SHOW_ORIGIN        10
#define INT_QSNAP_ENDPOINT          11
#define INT_QSNAP_MIDPOINT          12
#define INT_QSNAP_CENTER            13
#define INT_QSNAP_NODE              14
#define INT_QSNAP_QUADRANT          15
#define INT_QSNAP_INTERSECTION      16
#define INT_QSNAP_EXTENSION         17
#define INT_QSNAP_INSERTION         18
#define INT_QSNAP_PERPENDICULAR     19
#define INT_QSNAP_TANGENT           20
#define INT_QSNAP_NEAREST           21
#define INT_QSNAP_APPARENT          22
#define INT_QSNAP_PARALLEL          23
#define INT_SELECTION_MODE_PICK_FIRST 24
#define INT_SELECTION_MODE_PICK_ADD   25
#define INT_SELECTION_MODE_PICK_DRAG  26
#define INT_RULER_SHOW_ON_LOAD           27
#define INT_GRID_COLOR_MATCH_CROSSHAIR   28

#define DBL_DISPLAY_ZOOM_SCALE_ACTION_IN  0
#define DBL_DISPLAY_ZOOM_SCALE_ACTION_OUT 1
#define DBL_OPENSAVE_RECENT_MAX_FILES     2
#define DBL_OPENSAVE_TRIM_DST_NUM_JUMPS   3
#define DBL_RULER_PIXEL_SIZE          4
#define DBL_GRID_CENTER_X             5
#define DBL_GRID_CENTER_Y             6
#define DBL_GRID_SIZE_X               7
#define DBL_GRID_SIZE_Y               8
#define DBL_GRID_SPACING_X            9
#define DBL_GRID_SPACING_Y           10
#define DBL_GRID_SIZE_RADIUS         11
#define DBL_GRID_SPACING_RADIUS      12
#define DBL_GRID_SPACING_ANGLE       13
#define DBL_SELECTION_GRIP_SIZE      14
#define DBL_SELECTION_PICKBOX_SIZE   15

#define DISABLE_GRID                  0
#define RECTANGULAR_GRID              1
#define CIRCULAR_GRID                 2
#define ISOMETRIC_GRID                3

#define ALIGN_LEFT                    0
#define ALIGN_RIGHT                   1
#define ALIGN_CENTER                  2

#define PATH_TYPE_MOVETO              0
#define PATH_TYPE_LINETO              1
#define PATH_TYPE_ARCTO               2
#define PATH_TYPE_ARCMOVETO           3
#define PATH_TYPE_ELLIPSE             4
#define PATH_TYPE_END                 5

#define DOLPHIN_MODES_NUM_POINTS      0
#define DOLPHIN_MODES_XSCALE          1
#define DOLPHIN_MODES_YSCALE          2

#define ELLIPSE_MODES_MAJORDIAMETER_MINORRADIUS    0
#define ELLIPSE_MODES_MAJORRADIUS_MINORRADIUS      1
#define ELLIPSE_MODES_ROTATION                     2

#define TREBLE_CLEF_MODES_NUM_POINTS  0
#define TREBLE_CLEF_MODES_XSCALE      1
#define TREBLE_CLEF_MODES_YSCALE      2

/* Polygon modes.
 */
#define POLYGON_MODE_NUM_SIDES        0
#define POLYGON_MODE_CENTER_PT        1
#define POLYGON_MODE_POLYTYPE         2
#define POLYGON_MODE_INSCRIBE         3
#define POLYGON_MODE_CIRCUMSCRIBE     4
#define POLYGON_MODE_DISTANCE         5
#define POLYGON_MODE_SIDE_LEN         6

#define INVISIBLE                     0
#define ALWAYS_VISIBLE                1
#define MENU_VISIBLE                  2

/*
 * TYPEDEFS
 * -----------------------------------------------------------------------------
 */

/* Action: the basic system to encode all user input.
 *
 * This typedef gives structure to the data associated with each action
 * which, in the code, is referred to by the action id (an int from
 * the define table above).
 * -----------------------------------------------------------------------------
 *
 * DESCRIPTION OF STRUCT CONTENTS
 *
 * label
 * -----
 *
 * What is called from Scheme to run the function.
 * It is always in US English, lowercase,
 * seperated with hyphens.
 *
 * For example: new-file.
 *
 * function
 * --------
 *
 * The function pointer, always starts with the prefix scm,
 * in US English, lowercase, seperated with underscores.
 *
 * The words should match those of the label otherwise.
 *
 * For example: scm_new_file.
 *
 * flags
 * -----
 *
 * The bit based flags all collected into a 32-bit integer.
 *
 * | bit(s) | description                                |
 * |--------|--------------------------------------------|
 * | 0      | User (0) or system (1) permissions.        |
 * | 1-3    | The mode of input.                         |
 * | 4-8    | The object classes that this action        |
 * |        | can be applied to.                         |
 * | 9-10   | What menu (if any) should it be present in.|
 * | 11-12  | What                                       |
 *
 * description
 * -----------
 *
 * The string placed in the tooltip describing the action.
 * -----------------------------------------------------------------------------
 */
typedef struct Action_ {
    char command[MAX_STRING_LENGTH];
    pointer (*function)(scheme *sc, pointer args);
    int flags;
    char menu_name[MAX_STRING_LENGTH];
    char description[MAX_STRING_LENGTH];
} Action;

typedef struct MdiArea_ {
    int tabs_closeable;
    int use_logo;
    int use_texture;
    int use_color;
    int bg_logo;
    int bg_texture;
    int bg_color;
} MdiArea;

typedef struct Emb_Tool {
    EmbColor color;
    int style;
} emb_tool;

typedef struct Emb_Toolset {
    emb_tool left_pen;
    emb_tool left_brush;
    emb_tool right_pen;
    emb_tool right_brush;
    emb_tool dir_pen;
    emb_tool dir_brush;
    int alpha;
    int box_dir;
} emb_toolset;

/* Widget: the basic unit of the GUI.
 *
 * OVERVIEW
 *
 * All buttons, shortcuts, menus and regions of the windows should be widgets.
 *
 * The widgets are stored, accessed and altered via a binary tree where the
 * left side is dominant. 
 *
 * The strength of the new GUI relies heavily on this core concept. All the
 * FreeGLUT 3 calls will happen at the end of calls to the widgets.
 *
 * Perhaps the action system should be connected to this somehow?
 *
 * DESCRIPTION OF STRUCT CONTENTS
 *
 * label
 *     If the widget is a text box like a menu bar item
 *     then it needs this char array to store the string.
 *
 * position
 *     Relative to its parent, where should the widget go
 *     (the top left corner's offset from the top left corner).
 * 
 * mode
 *     Whether to use label, svg_path, icon approach.
 */
typedef struct Widget_ {
    SDL_Rect rect;
    SDL_Texture *texture;
    unsigned char color[4];
    char label[MAX_STRING_LENGTH];
    int mode;
    char command[MAX_STRING_LENGTH];
    int visibility;
} widget;

/* Text Properties
 * ---------------
 */
typedef struct Text_Properties {
    float size;
    float angle;
    int bold;
    int italic;
    int underline;
    int overline;
    int strikeout;
    int backward;
    int upsidedown;
} text_properties;

/* Layer Manager
 * -------------
 */
typedef struct Layer_Manager {
    char name[MAX_STRING_LENGTH];
    int visible;
    int frozen;
    int z_value;
    char color[10];
    char line_type[10];
    double line_weight;
} LayerManager;

/* Tabs
 * ----
 * The per-file data associated with each tab.
 * All the statusbar toggles are stored here.
 */
typedef struct WindowTab_ {
    char fname[100];
    LayerManager *layer_mgr;
    int number_mode;
    int snap;
    int grid;
    int ruler;
    int ortho;
    int polar;
    int qsnap;
    int qtrack;
    int lwt;
    int real;
} WindowTab;

/* Settings structs
 * ----------------
 */
typedef struct Setting_ {
    char description[MAX_STRING_LENGTH];
    int index;
    char type[50];
    int min;
    int max;
    int row;
    int column;
    int align;
} Setting;

typedef struct SettingBox_ {
    char *title;
    Setting *settings;
} SettingBox;

typedef struct SettingsTab_ {
    char *title;
    int n_boxes;
    SettingBox *box;
} SettingsTab;

/* Properties structs
 * ------------------
 * Covers Comboboxes, Line edits, Dropdowns etc. */
typedef struct Property_ {
    char description[MAX_STRING_LENGTH];
    unsigned char permissions;
    unsigned char data_type;
} Property;

typedef struct PropertyBox_ {
    char title[MAX_STRING_LENGTH];
    int obj_type;
    char **properties;
} PropertyBox;

/* The parent struct for our GUI.
 */
typedef struct Window_ {
    EmbPattern* pattern[10];
    Action action_list[MAX_ACTIONS];
    widget *widgets;
    int n_widgets;
    SDL_Window *window;
    SDL_Renderer *renderer;
    TTF_Font *font;
    scheme *sc;
    int running;
    int debug_mode;
    int track_mode;
    /* should be per view, not per window. */
    int show_lwt;
    int show_ortho;
    int show_ruler;
} Window;

/* Action functions
 * ----------------
 *
 * These are all functions of the type:
 *     pointer function(scheme *sc, pointer args);
 *
 * Should match the order of the action id table above.
 */
pointer scm_stub_testing(scheme *sc, pointer args);
pointer scm_do_nothing(scheme *sc, pointer args);
pointer scm_debug_message(scheme *sc, pointer args);

/* File Actions */
pointer scm_new_file(scheme *sc, pointer args);
pointer scm_open_file(scheme *sc, pointer args);
pointer scm_save_file(scheme *sc, pointer args);
pointer scm_save_file_as(scheme *sc, pointer args);
pointer scm_print(scheme *sc, pointer args);
pointer scm_exit(scheme *sc, pointer args);

/* Edit Actions */
pointer scm_undo(scheme *sc, pointer args);
pointer scm_redo(scheme *sc, pointer args);
pointer scm_cut(scheme *sc, pointer args);
pointer scm_copy(scheme *sc, pointer args);
pointer scm_paste(scheme *sc, pointer args);

/* Dialog Actions */
pointer scm_changelog(scheme *sc, pointer args);
pointer scm_about(scheme *sc, pointer args);
pointer scm_tip_of_the_day(scheme *sc, pointer args);
pointer scm_help(scheme *sc, pointer args);
pointer scm_design_details(scheme *sc, pointer args);
pointer scm_settings_dialog(scheme *sc, pointer args);
pointer scm_whats_this(scheme *sc, pointer args);
pointer scm_layer_selector(scheme *sc, pointer args);

/* Window Actions */
pointer scm_window_cascade(scheme *sc, pointer args);
pointer scm_window_tile(scheme *sc, pointer args);
pointer scm_window_close(scheme *sc, pointer args);
pointer scm_window_close_all(scheme *sc, pointer args);
pointer scm_window_next(scheme *sc, pointer args);
pointer scm_window_previous(scheme *sc, pointer args);

pointer scm_lineTypeSelector(scheme *sc, pointer args);
pointer scm_lineWeightSelector(scheme *sc, pointer args);

pointer scm_color_selector(scheme *sc, pointer args);

pointer scm_hide_all_layers(scheme *sc, pointer args);
pointer scm_show_all_layers(scheme *sc, pointer args);
pointer scm_freeze_all_layers(scheme *sc, pointer args);
pointer scm_thaw_all_layers(scheme *sc, pointer args);
pointer scm_lock_all_layers(scheme *sc, pointer args);
pointer scm_unlock_all_layers(scheme *sc, pointer args);
pointer scm_make_layer_current(scheme *sc, pointer args);

/* Icon Actions */
pointer scm_icon_16(scheme *sc, pointer args);
pointer scm_icon_24(scheme *sc, pointer args);
pointer scm_icon_32(scheme *sc, pointer args);
pointer scm_icon_48(scheme *sc, pointer args);
pointer scm_icon_64(scheme *sc, pointer args);
pointer scm_icon_128(scheme *sc, pointer args);

/* Text Actions */
pointer scm_text_italic(scheme *sc, pointer args);
pointer scm_text_overline(scheme *sc, pointer args);
pointer scm_text_underline(scheme *sc, pointer args);
pointer scm_text_strikeout(scheme *sc, pointer args);
pointer scm_text_bold(scheme *sc, pointer args);

/* Layer Actions */
pointer scm_make_layer_active(scheme *sc, pointer args);
pointer scm_layer_manager(scheme *sc, pointer args);
pointer scm_layer_previous(scheme *sc, pointer args);

/* Zoom Actions */
pointer scm_zoom_real_time(scheme *sc, pointer args);
pointer scm_zoom_previous(scheme *sc, pointer args);
pointer scm_zoom_window(scheme *sc, pointer args);
pointer scm_zoom_dynamic(scheme *sc, pointer args);
pointer scm_zoom_scale(scheme *sc, pointer args);
pointer scm_zoom_center(scheme *sc, pointer args);
pointer scm_zoom_in(scheme *sc, pointer args);
pointer scm_zoom_out(scheme *sc, pointer args);
pointer scm_zoom_selected(scheme *sc, pointer args);
pointer scm_zoom_all(scheme *sc, pointer args);
pointer scm_zoom_extents(scheme *sc, pointer args);

/* Pan SubMenu */
pointer scm_pan_real_time(scheme *sc, pointer args);
pointer scm_pan_point(scheme *sc, pointer args);
pointer scm_pan_left(scheme *sc, pointer args);
pointer scm_pan_right(scheme *sc, pointer args);
pointer scm_pan_up(scheme *sc, pointer args);
pointer scm_pan_down(scheme *sc, pointer args);

pointer scm_day_vision(scheme *sc, pointer args);
pointer scm_night_vision(scheme *sc, pointer args);

pointer scm_distance(scheme *sc, pointer args);
pointer scm_line(scheme *sc, pointer args);
pointer scm_dolphin(scheme *sc, pointer args);
pointer scm_treble_clef(scheme *sc, pointer args);
pointer scm_locate_point(scheme *sc, pointer args);
pointer scm_heart(scheme *sc, pointer args);
pointer scm_delete_object(scheme *sc, pointer args);
pointer scm_ellipse(scheme *sc, pointer args);
pointer scm_circle(scheme *sc, pointer args);
pointer scm_path(scheme *sc, pointer args);
pointer scm_layers(scheme *sc, pointer args);
pointer scm_line_type_selector(scheme *sc, pointer args);
pointer scm_line_weight_selector(scheme *sc, pointer args);
pointer scm_select_all(scheme *sc, pointer args);
pointer scm_check_for_updates(scheme *sc, pointer args);
pointer scm_print_pattern(scheme *sc, pointer args);
pointer scm_exit_program(scheme *sc, pointer args);
pointer scm_move(scheme *sc, pointer args);

pointer scm_create_widget(scheme *sc, pointer args);
pointer scm_create_label(scheme *sc, pointer args);
pointer scm_create_ui_rect(scheme *sc, pointer args);
pointer scm_create_icon(scheme *sc, pointer args);
pointer scm_export(scheme *sc, pointer args);
pointer scm_set_visibility(scheme *sc, pointer args);

/*
 *  Function prototypes which aren't scheme objects.
 */
int valid_file_format(char *fname);

void debug_message(char *msg);
char *translate(char *msg);

double emb_min(double x, double y);
double emb_max(double x, double y);
double emb_clamp(double lower, double x, double upper);

int click_detection(widget *w, int x, int y);

/* Functions that act on windows */
void process_input();
void main_loop();
void create_widget(SDL_Rect rect, char *action_id);

char *load_str(scheme *sc, char *label);
int load_int(scheme *sc, char *label);
void set_int(scheme *sc, char *label, int i);
int load_config(scheme *sc, char *fname);
int render();

int find_mdi_window(char *file_name);

int load_to_buffer();
int save_from_buffer();
void display_buffer();

void create_widget(SDL_Rect rect, char *command);
int get_widget_by_label(char *label);
void set_visibility(char *label, int visibility);

void get_args(pointer args, pointer arg[10], int n);

extern Window *mainwnd;
extern Action action_list[MAX_ACTIONS];

#endif
