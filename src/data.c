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
 *
 *  ------------------------------------------------------------
 *
 *  Data
 *  ----
 *  The data for configuring Embroidermodder 2,
 *  sets the default values should it not be present
 *  in configuration.
 */

#include "em2.h"


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


Action action_list[MAX_ACTIONS] = {
    {
        "do-nothing",
        do_nothing,
        PERMISSIONS_USER,
        "Do &Nothing",
        "An action that intensionally does nothing."
    },
    /*
    {
        "debug-message",
        debug_message,
        PERMISSIONS_USER,
        "Debug &Message",
        "Prints to the console (that the program is launched from) any debugging information."
    },
    */
    {
        "new-file",
        new_file,
        PERMISSIONS_USER,
        "&New",
        "Create a new file in a new tab."
    },
    {
        "open-file",
        open_file,
        PERMISSIONS_USER,
        "&Open",
        "Open an existing file."
    },
    {
        "save-file",
        save_file,
        PERMISSIONS_USER,
        "&Save",
        "Save the design to disk."
    },
    {
        "save-file-as",
        save_file_as,
        PERMISSIONS_USER,
        "&Save File As...",
        "Save the design under a new name and/or format."
    },
    {
        "check-for-updates",
        check_for_updates,
        PERMISSIONS_USER,
        "Check for &Updates",
        "NULL"
    },
    {
        "select-all",
        select_all,
        PERMISSIONS_USER,
        "&Select All",
        "Selects all objects: SELECTALL"
    },
    {
        "whats-this",
        whats_this,
        PERMISSIONS_USER,
        "&What's This?",
        "What's This? Context Help!"
    },
    {
        "design-details",
        design_details,
        PERMISSIONS_USER,
        "&Design Details",
        "Show the design details dialog for the current design."
    },
    {
        "print-pattern",
        print_pattern,
        PERMISSIONS_USER,
        "&Print",
        "Opens the print dialog to print pattern information and images."
    },
    {
        "exit-program",
        exit_program,
        PERMISSIONS_USER,
        "E&xit",
        "Exit the application."
    },
    {
        "cut-object",
        cut,
        PERMISSIONS_USER,
        "Cu&t",
        "Cut the current selection's contents to the clipboard."
    },
    {
        "copy-object",
        copy,
        PERMISSIONS_USER,
        "&Copy",
        "Copy the current selection's contents to the clipboard."
    },
    {
        "paste-object",
        paste,
        PERMISSIONS_USER,
        "&Paste",
        "Paste the clipboard contents into the selection within the scene."
    },
    {
        "undo",
        undo,
        PERMISSIONS_USER,
        "&Undo",
        "Reverses the most recent action."
    },
    {
        "redo",
        redo,
        PERMISSIONS_USER,
        "&Redo",
        "Reverses the effects of the previous undo action."
    },
    {
        "window-close",
        window_close,
        PERMISSIONS_USER,
        "Cl&ose",
        "Close the active window."
    },
    {
        "window-close-all",
        window_close_all,
        PERMISSIONS_USER,
        "Close &All",
        "Close all the windows."
    },
    {
        "window-cascade",
        window_cascade,
        PERMISSIONS_USER,
        "&Cascade",
        "Cascade the windows."
    },
    {
        "window-tile",
        window_tile,
        PERMISSIONS_USER,
        "&Tile",
        "Tile the windows."
    },
    {
        "window-next",
        window_next,
        PERMISSIONS_USER,
        "Ne&xt",
        "Move the focus to the next window."
    },
    {
        "window-previous",
        window_previous,
        PERMISSIONS_USER,
        "Pre&vious",
        "Move the focus to the previous window."
    },
    {
        "help",
        help,
        PERMISSIONS_USER,
        "&Help",
        "Displays help."
    },
    {
        "changelog-dialog",
        changelog,
        PERMISSIONS_USER,
        "&Changelog",
        "Opens a log of recent, new features in this product."
    },
    {
        "tip-of-the-day-dialog",
        tip_of_the_day,
        PERMISSIONS_USER,
        "&Tip Of The Day",
        "Displays a dialog with useful tips."
    },
    {
        "about-dialog",
        about,
        PERMISSIONS_USER,
        "&About Embroidermodder 2",
        "Displays information about this product."
    },
    {
        "icon-16",
        icon_16,
        PERMISSIONS_USER,
        "Icon&16",
        "Sets the toolbar icon size to 16x16."
    },
    {
        "icon-24",
        icon_24,
        PERMISSIONS_USER,
        "Icon&24",
        "Sets the toolbar icon size to 24x24."
    },
    {
        "icon-32",
        icon_32,
        PERMISSIONS_USER,
        "Icon&36",
        "Sets the toolbar icon size to 36x36."
    },
    {
        "icon-48",
        icon_48,
        PERMISSIONS_USER,
        "Icon&48",
        "Sets the toolbar icon size to 48x48."
    },
    {
        "icon-64",
        icon_64,
        PERMISSIONS_USER,
        "Icon&64",
        "Sets the toolbar icon size to 64x64."
    },
    {
        "icon-128",
        icon_128,
        PERMISSIONS_USER,
        "Icon 12&8",
        "Sets the toolbar icon size to 128x128."
    },
    {
        "settings-dialog",
        settings_dialog,
        PERMISSIONS_USER,
        "NULL",
        "NULL"
    },
    {
        "make-layer-current",
        make_layer_current,
        PERMISSIONS_USER,
        "&Make Layer Active",
        "Makes the layer of a selected object the active layer"
    },
    {
        "layers",
        layers,
        PERMISSIONS_USER,
        "&Layers",
        "Manages layers and layer properties: LAYER"
    },
    {
        "layer-selector",
        layer_selector,
        PERMISSIONS_USER,
        "&Layer Selector",
        "Dropdown selector for changing the current layer"
    },
    {
        "layer-previous",
        layer_previous,
        PERMISSIONS_USER,
        "&Layer Previous",
        "Restores the previous layer settings: LAYERP"
    },
    {
        "color-selector",
        color_selector,
        PERMISSIONS_USER,
        "&Color Selector",
        "Dropdown selector for changing the current thread color"
    },
    {
        "line-type-selector",
        line_type_selector,
        PERMISSIONS_USER,
        "&Stitchtype Selector",
        "Dropdown selector for changing the current stitch type"
    },
    {
        "line-weight-selector",
        line_weight_selector,
        PERMISSIONS_USER,
        "&Threadweight Selector",
        "Dropdown selector for changing the current thread weight"
    },
    {
        "hide-all-layers",
        hide_all_layers,
        PERMISSIONS_USER,
        "&Hide All Layers",
        "Turns the visibility off for all layers in the current drawing: HIDEALL"
    },
    {
        "show-all-layers",
        show_all_layers,
        PERMISSIONS_USER,
        "&Show All Layers",
        "Turns the visibility on for all layers in the current drawing: show-all-layers"
    },
    {
        "freeze-all-layers",
        freeze_all_layers,
        PERMISSIONS_USER,
        "&Freeze All Layers",
        "Freezes all layers in the current drawing:  FREEZEALL"
    },
    {
        "thaw-all-layers",
        thaw_all_layers,
        PERMISSIONS_USER,
        "&Thaw All Layers",
        "Thaws all layers in the current drawing: thaw_all_layers"
    },
    {
        "lock-all-layers",
        lock_all_layers,
        PERMISSIONS_USER,
        "&Lock All Layers",
        "Locks all layers in the current drawing: lock-all-layers"
    },
    {
        "unlock-all-layers",
        unlock_all_layers,
        PERMISSIONS_USER,
        "&Unlock All Layers",
        "Unlocks all layers in the current drawing: unlock-all-layers"
    },
    {
        "text-bold",
        text_bold,
        PERMISSIONS_USER,
        "&Bold Text",
        "Sets text to be bold."
    },
    {
        "text-italic",
        text_italic,
        PERMISSIONS_USER,
        "&Italic Text",
        "Sets text to be italic."
    },
    {
        "text-underline",
        text_underline,
        PERMISSIONS_USER,
        "&Underline Text",
        "Sets text to be underlined."
    },
    {
        "text-strikeout",
        text_strikeout,
        PERMISSIONS_USER,
        "&StrikeOut Text",
        "Sets text to be striked out."
    },
    {
        "text-overline",
        text_overline,
        PERMISSIONS_USER,
        "&Overline Text",
        "Sets text to be overlined."
    },
    {
        "zoom-real-time",
        zoom_real_time,
        PERMISSIONS_USER,
        "Zoom &Realtime",
        "Zooms to increase or decrease the apparent size of objects in the current viewport."
    },
    {
        "zoom-previous",
        zoom_previous,
        PERMISSIONS_USER,
        "Zoom &Previous",
        "Zooms to display the previous view."
    },
    {
        "zoom-window",
        zoom_window,
        PERMISSIONS_USER,
        "Zoom &Window",
        "Zooms to display an area specified by a rectangular window."
    },
    {
        "zoom-dynamic",
        zoom_dynamic,
        PERMISSIONS_USER,
        "Zoom &Dynamic",
        "Zooms to display the generated portion of the drawing."
    },
    {
        "zoom-scale",
        zoom_scale,
        PERMISSIONS_USER,
        "Zoom Sca&le",
        "Zooms the display using a specified scale factor."
    },
    {
        "zoom-center",
        zoom_center,
        PERMISSIONS_USER,
        "Zoom &Center",
        "Zooms to display a view specified by a center point and  magnification or height."
    },
    {
        "zoom-in",
        zoom_in,
        PERMISSIONS_USER,
        "Zoom &In",
        "Zooms to increase the apparent size of objects."
    },
    {
        "zoom-out",
        zoom_out,
        PERMISSIONS_USER,
        "Zoom &Out",
        "Zoom to decrease the apparent size of object."
    },
    {
        "zoom-selected",
        zoom_selected,
        PERMISSIONS_USER,
        "Zoom Selec&ted",
        "Zooms to display the selected objects."
    },
    {
        "zoom-all",
        zoom_all,
        PERMISSIONS_USER,
        "Zoom &All",
        "Zooms to display the drawing extents or the grid limits."
    },
    {
        "zoom-extents",
        zoom_extents,
        PERMISSIONS_USER,
        "Zoom &Extents",
        "Zooms to display the drawing extents."
    },
    {
        "pan-real-time",
        pan_real_time,
        PERMISSIONS_USER,
        "&Pan Realtime",
        "Moves the view in the current viewport."
    },
    {
        "pan-point",
        pan_point,
        PERMISSIONS_USER,
        "Pan &Point",
        "Moves the view by the specified distance."
    },
    {
        "pan-left",
        pan_left,
        PERMISSIONS_USER,
        "&Pan Left",
        "Moves the view to the left."
    },
    {
        "pan-right",
        pan_right,
        PERMISSIONS_USER,
        "&Pan Right",
        "Moves the view to the right."
    },
    {
        "pan-up",
        pan_up,
        PERMISSIONS_USER,
        "&Pan Up",
        "Moves the view up."
    },
    {
        "pan-down",
        pan_down,
        PERMISSIONS_USER,
        "&Pan Down",
        "Moves the view down."
    },
    {
        "day-vision",
        day_vision,
        PERMISSIONS_USER,
        "&Day",
        "Updates the current view using day vision settings."
    },
    {
        "night-vision",
        night_vision,
        PERMISSIONS_USER,
        "&Night",
         "Updates the current view using night vision settings."
    },
    {
        "treble-clef",
        treble_clef,
        PERMISSIONS_USER,
        "&TrebleClef",
        "Creates a treble clef: TREBLECLEF"
    },
    {
        "path",
        path,
        PERMISSIONS_USER,
        "&Path",
		"Add a path object to the current view."
    },
    {
        "circle",
        circle,
        PERMISSIONS_USER,
        "&Circle",
        "Adds a circle to the vector layer."
    },
    {
        "line",
        line,
        PERMISSIONS_USER,
        "&Line",
        "Creates straight line segments: LINE"
    },
    {
        "distance",
        distance,
        PERMISSIONS_USER,
        "&Distance",
        "Measures the distance and angle between two points: DIST"
    },
    {
        "dolphin",
        dolphin,
        PERMISSIONS_USER,
        "&Dolphin",
		"Create a dolphin design in the vector layer."
    },
    {
        "ellipse",
        ellipse,
        PERMISSIONS_USER,
        "&Ellipse",
        "Add an ellipse to the vector layer."
    },
    {
        "delete-object",
        delete_object,
        PERMISSIONS_USER,
        "D&elete",
        "Removes objects from a drawing: delete"
    },
    {
        "heart",
        heart,
        PERMISSIONS_USER,
        "&Heart",
        "Creates a heart: HEART"
    },
    {
        "locate-point",
        locate_point,
        PERMISSIONS_USER,
        "&Locate Point",
        "Find a point that is near the indicated region."
    },
    {
        "move",
        move,
        PERMISSIONS_USER,
        "&Move",
        "Displaces objects a specified distance in a specified direction: MOVE"
    },
    {
        "export",
        export_,
        PERMISSIONS_USER,
        "&Export",
        "Export the current tab as a non-stitch format that will lose more data."
    },
    {
        "create-ui-rect",
        NULL,
        PERMISSIONS_SYSTEM,
        "NULL",
        "NULL"
    },
    {
        "create-label",
        NULL,
        PERMISSIONS_SYSTEM,
        "NULL",
        "NULL"
    },
    {
        "create-widget",
        NULL,
        PERMISSIONS_SYSTEM,
        "NULL",
        "NULL"
    },
    {
        "create-icon",
        NULL,
        PERMISSIONS_SYSTEM,
        "NULL",
        "NULL"
    },
    {
        "set-visibility",
        NULL,
        PERMISSIONS_SYSTEM,
        "NULL",
        "NULL"
    },
    {
        "file-menu",
        display_file_menu,
        PERMISSIONS_SYSTEM,
        "NULL",
        "NULL"
    },
    {
        "edit-menu",
        display_edit_menu,
        PERMISSIONS_SYSTEM,
        "NULL",
        "NULL"
    },
    {
        "view-menu",
        display_view_menu,
        PERMISSIONS_SYSTEM,
        "NULL",
        "NULL"
    },
    {
        "settings-menu",
        display_settings_menu,
        PERMISSIONS_SYSTEM,
        "NULL",
        "NULL"
    },
    {
        "END",
        NULL,
        0,
        "NULL",
        "NULL"
    }
};

    /*
    {
		"assets/objects/dolphin/dolphin.scm",
		num_points=512
		scale_x=0.04
		scale_y=0.04
        "Creates a dolphin: dolphin"
    },
    {
        "heart4",
        heart4,
        PERMISSIONS_USER,
		"&Heart4",
		"Adds a heart to the vector layer: heart4."
		"assets/scripts/heart4.scm"
    },
    {
		"heart5",
        heart5,
        PERMISSIONS_USER,
		"&Heart5",
		"Adds a heart to the vector layer."
		"assets/scripts/heart5.scm"
    },
    {
        "single-line-text",
        PERMISSIONS_USER,
        single_line_text,
        "&Single Line Text",
        "Creates single-line text objects: TEXT"
    },
    {
        "spell-check",
        PERMISSIONS_USER | OBJ_TYPE_TEXT,
        spell_check,
        "S&pell Check",
        "Spell check the text object selected."
    },
    {
        "quick-select",
        PERMISSIONS_USER,
        quick_select,
    },
    {
        "rectangle",
        PERMISSIONS_USER,
        rectangle,
        "&Rectangle",
        "Creates a rectangular polyline: RECTANGLE"
    },
    {
        "rgb",
        PERMISSIONS_USER,
        rgb,
        "&RGB",
        "Updates the current view colors: RGB"
    },
    {
        "rotate",
        PERMISSIONS_USER,
        rotate,
        "&Rotate",
        "Rotates objects about a base point: ROTATE"
    },
    {
        "sandbox",
        PERMISSIONS_USER,
        sandbox,
        "&Sandbox",
        "A sandbox to play in: SANDBOX"
    },
    {
        "quickleader",
        quickleader,
        PERMISSIONS_USER,
        "&QuickLeader",
        "Creates a leader and annotation: QUICKLEADER"
    },
    {
        "snowflake",
        snowflake,
        PERMISSIONS_USER,
        "&Snowflake",
        "Creates a snowflake: SNOWFLAKE"
    },
    {
        "star",
        star,
        PERMISSIONS_USER,
        "&Star",
        "Creates a star: STAR"
    },
    {
        "platform",
        platform,
        PERMISSIONS_USER,
        "&Platform",
        "List which platform is in use: PLATFORM"
    },
    {
        "point",
        point,
        PERMISSIONS_USER,
        "&Point",
        "Creates multiple points: POINT"
    },
    {
        "polygon",
        polygon,
        PERMISSIONS_USER,
        "Pol&ygon",
        "Creates a regular polygon: POLYGON"
    },
    {
        "polyline",
        polyline,
        PERMISSIONS_USER,
        "&Polyline",
        "Creates a 2D polyline: PLINE"
    },
    {
        "settings-dialog",
        settings_dialog,
        PERMISSIONS_USER,
        "&Settings",
        "Configure settings specific to this product."
    },
    {
        "quickleader",
        quickleader,
        PERMISSIONS_USER,
        "&QuickLeader",
        "Creates a leader and annotation: QUICKLEADER"
    },
    {
        "locate-point",
        locate_point,
        PERMISSIONS_USER,
        "&Locate Point",
        "Displays the coordinate values of a location: ID"
    },
    {
        "point",
        point,
        PERMISSIONS_USER,
        "&Point",
        "Creates multiple points: POINT"
    }
    */

char state[2*MAX_VARIABLES][MAX_STRING_LENGTH] = {
    "running", "1",
    "testing", "0",
    "debug-mode", "1",
    "track-mode", "0",
    "show-lwt", "1",
    "show-ortho", "1",
    "show-ruler", "1",
    "title", "Embroidermodder",
    "version", "2.0.0-alpha",
    "interface-font", "assets/fonts/source-sans/TTF/SourceSans3-Regular.ttf",
    "text-font", "assets/fonts/source-code-pro/TTF/SourceCodePro-Black.ttf",
    "n-patterns", "0",
    "real-render", "0",
    "max-string-length", "500",

    /* Window Settings */
    "window-offset-x", "100",
    "window-offset-y", "100",
    "window-width", "640",
    "window-height", "480",

    /* Tab Settings */
    "n-tabs", "0",
    "tab-index", "0",

    "max-distance", "1000000.0",
    "general-icon-size", "16",
    "background-color", "446688",
    "interface-color", "6688AA",
    "button-background-color", "88AAFF",
    "color-mode", "0",
    "selection-coolgrip-color", "FFFFFF",
    "selection-hotgrip-color", "FFFFFF",

    "application-folder", "~/.embroidermodder2/",
    "printing-default-device", "0",
    "ruler-color", "FFFFFF",
    "qsnap-locator-color", "FFFFFF",
    "grid-type", "cartesian",

    /* Grip settings. */
    "selection-coolgrip-color", "FFFFFF",
    "selection-hotgrip-color", "FFFFFF",

    "translation-table 0", "default",
    "translation-table 1", "english (UK)",
    "translation-table 2", "english (US)",
    "translation-table 3", "french",
    "translation-table 4", "german",
    "translation-table 5", "spanish",

    "mdi-window", "0",

    /*
    QToolBar* toolbar[10]", "0",
    QMenu* menu[10]", "0",
    StatusBarButton* status-bar[8]", "0",
    QToolButton* toolButton[PROPERTY-EDITORS]", "0",
    QLineEdit* lineEdit[LINEEDIT-PROPERTY-EDITORS]", "0",
    QComboBox* comboBox[COMBOBOX-PROPERTY-EDITORS]", "0",
    */

    "opensave-custom-filter", "0",

    "paste-delta", "0",
    "scene-press-point", "0",
    "press-point", "0",
    "sceneMovePoint", "0",
    "movePoint", "0",
    "sceneReleasePoint", "0",
    "releasePoint", "0",
    "sceneGripPoint", "0",

    "view-mouse-point", "0",
    "scene-mouse-point", "0",
    "qsnap-locator-color", "0",
    "grip-color-cool", "0",
    "grip-color-hot", "0",
    "crosshair-color", "0",
    "precision-angle", "0",
    "precision-length", "0",

    "statusBarMouseCoord", "0",

    /* Used when checking if fields vary */
    "field-old-text", "0",
    "field-new-text", "0",
    "field-varies-text", "0",
    "field-yes-text", "0",
    "field-no-text", "0",
    "field-on-text", "0",
    "field-off-text", "0",

    /*
    QToolButton* toolButtonArcClockwise
    QComboBox* comboBoxArcClockwise

    Qgroup-box* group-boxGeometry[32]
    Qgroup-box* group-boxGeneral
    Qgroup-box* group-boxMiscArc
    Qgroup-box* group-boxMiscPath
    Qgroup-box* group-boxMiscPolyline
    Qgroup-box* group-boxTextTextSingle
    Qgroup-box* group-boxMiscTextSingle
    */

    /* General settings */
    "general-language", "default",
    "general-icon-theme", "default",
    "general-mdi-bg-logo", "assets/icons/logo.png",
    "general-mdi-bg-texture", "assets/icons/texture.png",
    "general-mdi-bg-color", "FFFFFF",
    "general-current-tip", "0",
    "general-tip-of-the-day", "0",
    "general-system-help-browser", "1",
    "general-check-for-updates", "0",
    "general-mdi-bg-use-logo", "0",
    "general-mdi-bg-use-texture", "0",
    "general-mdi-bg-use-color", "0",

    /* QSnap Properties */
    "qsnap-locator-color", "FFFFFF",
    "qsnap-endpoint", "0",
    "qsnap-midpoint", "0",
    "qsnap-center", "0",
    "qsnap-node", "0",
    "qsnap-quadrant", "0",
    "qsnap-intersection", "0",
    "qsnap-extension", "0",
    "qsnap-insertion", "0",
    "qsnap-perpendicular", "0",
    "qsnap-tangent", "0",
    "qsnap-nearest", "0",
    "qsnap-apparent", "0",
    "qsnap-parallel", "0",
    "qsnap-enabled", "0",
    "qsnap-locator-size", "16",
    "qsnap-aperture-size", "16",
    "qsnap-active", "0",
    "qsnap-toggle", "0",

    /* Text Settings */
    "text-style-bold", "0",
    "text-style-italic", "0",
    "text-style-underline", "0",
    "text-style-strikethrough", "0",

    /* Line Weight Settings */
    "lwt-show-lwt", "0",
    "lwt-real-render", "0",
    "lwt-default-lwt", "1.0",

    /* Ruler Settings */
    "ruler-metric", "1",
    "ruler-show-on-load", "1",
    "ruler-pixel-size", "30",

    /* Grid Settings */
    "grid-size-radius", "10.0",
    "grid-center-on-origin", "0",
    "grid-center", "0.5 0.5",
    "grid-size", "10.0 10.0",
    "grid-spacing", "10.0 10.0",
    "grid-size-radius", "10.0",
    "grid-spacing-radius", "10.0",
    "grid-spacing-angle", "10.0",
    "grid-show-on-load", "0",
    "grid-show-origin", "1",
    "grid-color-match-crosshair", "1",
    "grid-load-from-file", "1",
    "grid-color", "FFFFFF",

    "symbol-scale", "0.01",

    /* Display Settings */
    "display-use-opengl", "1",
    "display-renderhint-aa", "1",
    "display-renderhint-text-aa", "1",
    "display-renderhint-smooth-pix", "1",
    "display-renderhint-high-aa", "1",
    "display-renderhint-noncosmetic", "1",
    "display-show-scrollbars", "1",
    "display-scrollbar-widget-num", "1",
    "display-zoomscale-in", "1",
    "display-zoomscale-out", "1",
    "display-selectbox-alpha", "1",
    "display-crosshair-percent", "10",

    "opensave-open-format", "csv",
    "opensave-save-format", "csv",
    "opensave-recent-directory", "~",
    "opensave-recent-files 0", "new-file.csv",
    "opensave-open-thumbnail", "assets/icon/open_file_16.png",
    "opensave-save-thumbnail", "assets/icon/save_file_16.png",
    "opensave-recent-max-files", "10",
    "opensave-trim-dst-num-jumps", "6",

    "printing-use-last-device", "0",
    "printing-disable-bg", "0",

    /* Selection Settings */
    "selection-mode-pickfirst", "0",
    "selection-mode-pickadd", "0",
    "selection-mode-pickdrag", "0",
    "selection-grip-size", "16",
    "selection-pickbox-size", "16",

    "grip-size", "16",
    "pick-box-size", "40",
    "crosshair-size", "16",
    "shift-key-pressed-state", "0",
    "gripping-active", "0",
    "rapid-move-active", "0",
    "previewActive", "0",
    "pastingActive", "0",
    "movingActive", "0",
    "selecting-active", "0",
    "zoom-window-active", "0",

    /* Panning Settings */
    "panning-real-time-active", "0",
    "panning-point-active", "0",
    "panning-active", "0",

    /* Display Settings */
    "display-crosshair-color", "FFFFFF",
    "display-bg-color", "FFFFFF",
    "display-selectbox-left-color", "FFFFFF",
    "display-selectbox-left-fill", "FFFFFF",
    "display-selectbox-right-color", "FFFFFF",
    "display-selectbox-right-fill", "FFFFFF",
    "display-units", "Scientific",

    /* Shortcuts */
    "shortcut Ctrl+N", "new-file",
    "shortcut Ctrl+O", "open-file",
    "shortcut Ctrl+S", "save-file",
    "shortcut Ctrl+Shift+S", "save-file-as",
    "shortcut Ctrl+P", "print",
    "shortcut Ctrl+D", "design-details",
    "shortcut Ctrl+Q", "exit-program",
    "shortcut Ctrl+X", "cut",
    "shortcut Ctrl+C", "copy",
    "shortcut Ctrl+V", "paste",
    "shortcut Ctrl+Z", "undo",
    "shortcut Ctrl+Shift+Z", "redo",
    "shortcut F1", "help",
    "shortcut F2", "about",
    "shortcut F11", "fullscreen",

    /* Tips */
    "tip length", "15",
    "tip 0", "we need more tips?",
    "tip 1", "you can change the color of the display through settings?",
    "tip 2", "you can hide the scrollbars to increase the viewable area through settings?",
    "tip 3", "you can change the icon size for increased visibility?",
    "tip 4", "you can toggle the grid on and off by pressing the button in the statusbar?",
    "tip 5", "the grid size can be changed to match your hoop size through settings?",
    "tip 6", "the crosshair size is based on a percentage of your screen size? Setting it to 100 may help you visually line things up better.",
    "tip 7", "you can pan by pressing the middle mouse button and dragging your mouse across the screen?",
    "tip 8", "you can open and edit multiple designs simultaneously?",
    "tip 9", "that many embroidery machines support the .dst format?",
    "tip 10", "that you can zoom in and out using your mouse wheel?",
    "tip 11", "that you can use circular and isometric grids?",
    "tip 12", "about our command line format converter?",
    "tip 13", "that you can use the 'DAY' and 'NIGHT' commands to quickly switch the  view colors to commonly used white or black?",
    "tip 14", "that you can quickly change the background, crosshair and grid colors using the 'RGB' command?",
    
    /*
 In the order (icons, labels, value)
    "thread_weights)
  (vector
    (vector "assets/icons/lineweightbylayer.png" "ByLayer" -2.0)
    (vector "assets/icons/lineweightbyblock.png" "ByBlock" -1.0)
    (vector "assets/icons/lineweightdefault.png" "Default" 0.0)
    (vector "assets/icons/lineweight01.png" "0.00 mm" 0.0)
    (vector "assets/icons/lineweight02.png" "0.05 mm" 0.05)
    (vector "assets/icons/lineweight03.png" "0.15 mm" 0.15)
    (vector "assets/icons/lineweight04.png" "0.20 mm" 0.2)
    (vector "assets/icons/lineweight05.png" "0.25 mm" 0.25)
    (vector "assets/icons/lineweight06.png" "0.30 mm" 0.3)
    (vector "assets/icons/lineweight07.png" "0.35 mm" 0.35)
    (vector "assets/icons/lineweight08.png" "0.40 mm" 0.4)
    (vector "assets/icons/lineweight09.png" "0.45 mm" 0.45)
    (vector "assets/icons/lineweight10.png" "0.50 mm" 0.5)
    (vector "assets/icons/lineweight11.png" "0.55 mm" 0.55)
    (vector "assets/icons/lineweight12.png" "0.60 mm" 0.6)
    (vector "assets/icons/lineweight13.png" "0.65 mm" 0.65)
    (vector "assets/icons/lineweight14.png" "0.70 mm" 0.7)
    (vector "assets/icons/lineweight15.png" "0.75 mm" 0.75)
    (vector "assets/icons/lineweight16.png" "0.80 mm" 0.8)
    (vector "assets/icons/lineweight17.png" "0.85 mm" 0.85)
    (vector "assets/icons/lineweight18.png" "0.90 mm" 0.9)
    (vector "assets/icons/lineweight19.png" "0.95 mm" 0.95)
    (vector "assets/icons/lineweight20.png" "1.00 mm" 1.0)
    (vector "assets/icons/lineweight21.png" "1.05 mm" 1.05)
    (vector "assets/icons/lineweight22.png" "1.10 mm" 1.1)
    (vector "assets/icons/lineweight23.png" "1.15 mm" 1.15)
    (vector "assets/icons/lineweight24.png" "1.20 mm" 1.2)))

 This will parse correctly but may not
 run and could cause crashes.

 Use 2 space indentation and stack up trailing parentheses.

    "usage)
  (debug-message "Usage Message"))

    "rgb red green blue)
  (+
    blue
    (* 256 green)
    (* 256 256 red)))

 These toggle functions could be made by a macro.
    "toggle-polar)
   (let*
      (debug-message "StatusBarButton togglePolar()")
          "show-polar) (not show-polar))))

    "toggle-snap)
   (let*
      (debug-message "StatusBarButton toggleQSnap()")
          "snap-mode) (not snap-mode))))))

 Strings that don't need to be easily editable.

#define DATA_TYPE_END_MARKER -1

char preview_modes[][100] = {
    "null",
    "move",
    "rotate",
    "scale"
);

char comboboxes[][100] = {
    "ARC-CLOCKWISE",
    "GENERAL-LAYER",
    "GENERAL-COLOR",
    "GENERAL-LINE-TYPE",
    "GENERAL-LINE-WEIGHT",
    "TEXT-SINGLE-FONT",
    "TEXT-SINGLE-JUSTIFY"
);

Property circle_geometry_properties[] = {
    {"Circle", "system" DATA_TYPE_TITLE),
    {"Geometry", "system" DATA_TYPE_GROUP_BOX),
    {"Center X", "user" DATA_TYPE_DOUBLE),
    {"Center Y", "user" DATA_TYPE_DOUBLE),
    {"Radius", "user" DATA_TYPE_DOUBLE),
    {"Diameter", "system" DATA_TYPE_DOUBLE),
    {"Area", "system" DATA_TYPE_DOUBLE),
    {"Circumference", "system" DATA_TYPE_DOUBLE),
    {"END", "system" DATA_TYPE_END_MARKER)
);

Property ellipse_properties[] = {
    {"Ellipse", "system" DATA_TYPE_TITLE),
    {"Geometry", "system" DATA_TYPE_GROUP_BOX),
    {"Center X", "user" DATA_TYPE_DOUBLE),
    {"Center Y", "user" DATA_TYPE_DOUBLE),
    {"Radius Major", "user" DATA_TYPE_DOUBLE),
    {"Radius Minor", "user" DATA_TYPE_DOUBLE),
    {"Diameter Major", "user" DATA_TYPE_DOUBLE),
    {"Diameter Minor", "user" DATA_TYPE_DOUBLE),
    {"END", "system" DATA_TYPE_END_MARKER)
);

Property image_properties[] = {
 (vector
    {"Image", "system" DATA_TYPE_TITLE),
    {"Geometry", "system" DATA_TYPE_GROUP_BOX),
    {"Position X", "user" DATA_TYPE_DOUBLE),
    {"Position Y", "user" DATA_TYPE_DOUBLE),
    {"Width", "user" DATA_TYPE_DOUBLE),
    {"Height", "user" DATA_TYPE_DOUBLE),
    {"Misc", "system" DATA_TYPE_GROUP_BOX),
    {"Name", "user" DATA_TYPE_STRING),
    {"Path", "user" DATA_TYPE_STRING),
    {"END", "system" DATA_TYPE_END_MARKER)
))

Property infinite_line_properties[] = {
    {"Infinite Line", "system" DATA_TYPE_TITLE),
    {"Geometry", "system" DATA_TYPE_GROUP_BOX),
    {"Position X1", "user" DATA_TYPE_DOUBLE),
    {"Position Y1", "user" DATA_TYPE_DOUBLE),
    {"Position X2", "user" DATA_TYPE_DOUBLE),
    {"Position Y2", "user" DATA_TYPE_DOUBLE),
    {"Vector X", "system" DATA_TYPE_DOUBLE),
    {"Vector Y", "system" DATA_TYPE_DOUBLE),
    {"END", "system" DATA_TYPE_END_MARKER)
);

Arc
Geometry
Center X,double,user
Center Y,double,user
Radius,double,user
Start Angle,double,user
End Angle,double,user
Start X,double,system
Start Y,double,system
End X,double,system
End Y,double,system
Area,double,system
Length,double,system
Chord,double,system
Included Angle,double,system
END,END,END

char *settings_tabs[MAX_STRING_LENGTH] = {
    "General",
    "Files/Path",
    "Display",
    "Open/Save",
    "Printing",
    "Snap",
    "Grid/Ruler",
    "Ortho/Polar",
    "QuickSnap",
    "QuickTrack",
    "LineWeight",
    "Selection",
    "END"
);


"Rect"
"GroupBox" "Geometry"
char *geometry_rect_properties[MAX_STRING_LENGTH] = {
    "Corner X1,double,user",
    "Corner Y1,double,user",
    "Corner X2,double,user",
    "Corner Y2,double,user",
    "Corner X3,double,user",
    "Corner Y3,double,user",
    "Corner X4,double,user",
    "Corner Y4,double,user",
    "Height,double,user",
    "Width,double,user",
    "Area,double,system",
    "END"
);

char *arc_properties[MAX_STRING_LENGTH] = {
    "Center X,double,user",
    "Center Y,double,user",
    "Radius,double,user",
    "Start Angle,double,user",
    "End Angle,double,user",
    "Start X,double,system",
    "Start Y,double,system",
    "End X,double,system",
    "End Y,double,system",
    "Area,double,system",
    "Length,double,system",
    "Chord,double,system",
    "Included Angle,double,system",
    "END"
);

char *details_labels[MAX_STRING_LENGTH] = {
    "Total Stitches:",
    "Real Stitches:",
    "Jump Stitches:",
    "Trim Stitches:",
    "Total Colors:",
    "Color Changes:",
    "Left:",
    "Top:",
    "Right:",
    "Bottom:",
    "Width:",
    "Height:",
    "END"
);

char *statusbar_labels[MAX_STRING_LENGTH] = {
    "SNAP",
    "GRID",
    "RULER",
    "ORTHO",
    "POLAR",
    "QSNAP",
    "QTRACK",
    "LWT",
    "END"
);

    "obj-names 0", "Null",
    "obj-names 1", "Unknown",
    "obj-names 2", "Base",
    "obj-names 3", "Arc",
    "obj-names 4", "Block",
    "obj-names 5", "Circle",
    "obj-names 6", "Aligned Dimension",
    "obj-names 7", "Angular Dimension",
    "obj-names 8", "Arc Length Dimension",
    "obj-names 9", "Diameter Dimension",
    "obj-names 10", "Leader Dimension",
    "obj-names 11", "Linear Dimension",
    "obj-names 12", "Ordinate Dimension",
    "obj-names 13", "Radius Dimension",
    "obj-names 14", "Ellipse",
    "obj-names 15", "Elliptical Arc",
    "obj-names 16", "Rubber",
    "obj-names 17", "Grid",
    "obj-names 18", "Hatch",
    "obj-names 19", "Image",
    "obj-names 20", "Infinite Line",
    "obj-names 21", "Line",
    "obj-names 22", "Path",
    "obj-names 23", "Point",
    "obj-names 24", "Polygon",
    "obj-names 25", "Polyline",
    "obj-names 26", "Ray",
    "obj-names 27", "Rectangle",
    "obj-names 28", "Slot",
    "obj-names 29", "Spline",
    "obj-names 30", "Multi Line Text",
    "obj-names 31", "Single Line Text",
    "obj-names 32", "Unknown",
    "obj-names 33", "END",

    "text-single-properties)
  (vector
    "Text"
    "Text Single"
    "Contents" "string" "user"
    "Font" "dropdown" "user"
    "Justify" "dropdown" "user"
    "Height" "double" "system"
    "Rotation" "double" "system"
    "Backward" "int" "user"
    "Upside Down" "int" "user"
    "Position X" "double" "system"
    "Position Y" "double" "system"
    "END" "END" "END"))

    "block-properties)
  (vector
    "Block"
    "Block"
    "Position X" "double" "system"
    "Position Y" "double" "system"
    "END" "END" "END"))

    "line-properties)
  (vector
    "Line"
    "Line"
    "Start X" "double" "user"
    "Start Y" "double" "user"
    "End X" "double" "user"
    "End Y" "double" "user"
    "Delta X" "double" "system"
    "Delta Y" "double" "system"
    "Angle" "double" "system"
    "Length" "double" "system"
    "END" "END" "END"))

    "polygon-properties)
  (vector
    "Polygon"
    "Polygon"
    "Center X" "double" "user"
    "Center Y" "double" "user"
    "Radius Vertex" "double" "system"
    "Radius Side" "double" "system"
    "Diameter Vertex" "double" "system"
    "Diameter Side" "double" "system"
    "Interior Angle" "double" "system"
    "END" "END" "END"))

    "point-properties)
  (vector
    "Point"
    "Point"
    "Point X" "double" "user"
    "Point Y" "double" "user"
    "END" "END" "END"))

    "general-general-properties)
  (vector
    "General"
    "General"
    "Layer" "dropdown" "user"
    "Color" "dropdown" "user"
    "Line Type" "dropdown" "user"
    "Line Weight" "dropdown" "user"
    "END" "END" "END"))

    "misc-arc-properties)
  (vector
    "Misc"
    "Arc"
    "Clockwise" "int" "system"
    "END" "END" "END"))

    "misc-image-properties)
  (vector
    "Misc"
    "Image"
    "Name" "string" "system"
    "Path" "string" "system"
    "END" "END" "END"))

    "misc-path-properties)
  (vector
    "Misc"
    "Path"
    "Closed" "int" "user"
    "END" "END" "END"))

    "misc-polyline-properties)
  (vector
    "Misc"
    "Polyline"
    "Closed" "int" "user"
    "END" "END" "END"))

    "misc-text-single-properties)
  (vector
    "Misc"
    "Text Single"
    "Backward" "int" "user"
    "Upside down" "int" "user"
    "END" "END" "END"))

    "geometry-circle-properties)
  (vector
    "Geometry"
    "Circle"
    "Center X" "double" "user"
    "Center Y" "double" "user"
    "Radius" "double" "user"
    "Diameter" "double" "user"
    "Area" "double" "user"
    "Circumference" "double" "user"
    "END" "END" "END"))

    "geometry-image-properties)
  (vector
    "Geometry"
    "Image"
    "Position X" "double" "user"
    "Position Y" "double" "user"
    "Width" "double" "user"
    "Height" "double" "user"
    "END" "END" "END"))

    "geometry-infinite-line-properties)
  (vector
    "Geometry"
    "Infinite Line"
    "Start X" "double" "user"
    "Start Y" "double" "user"
    "2nd X" "double" "user"
    "2nd Y" "double" "user"
    "Vector X" "double" "system"
    "Vector Y" "double" "system"
    "END" "END" "END"))

    "GroupBox10)
  (vector
    "Geometry"
    "Line"
    "Start X" "double" "user"
    "Start Y" "double" "user"
    "End X" "double" "user"
    "End Y""double" "user"
    "Delta X" "double" "system"
    "Delta Y" "double" "system"
    "Angle X" "double" "system"
    "Length Y""double" "system"
    "END" "END" "END"))

    "GroupBox9)
  (vector
    "Geometry"
    "Path"
    "Vertex #" "int" "user"
    "Vertex X" "double" "user"
    "Vertex Y" "double" "user"
    "Area" "double" "system"
    "Length" "double" "system"
    "END" "END" "END"))

    "GroupBox8)
  (vector
    "Misc"
    "Path"
    "Closed" "int" "user"
    "END" "END" "END"))

    "geometry-polygon-properties)
  (vector
    "Geometry"
    "Polygon"
    "Center X" "double" "user"
    "Center Y" "double" "user"
    "Vertex Radius" "double" "user"
    "Side Radius" "double" "user"
    "Vertex Diameter" "double" "user"
    "Side Diameter" "double" "user"
    "Interior Angle" "double" "system"
    "END" "END" "END"))

    "geometry-polyline-properties)
  (vector
    "Geometry"
    "Polyline"
    "Vertex #" "int" "user"
    "Vertex X" "double" "user"
    "Vertex Y" "double" "user"
    "Area" "double" "system"
    "Length" "double" "system"
    "END" "END" "END"))

    "geometry-ray-properties)
  (vector
    "Geometry"
    "Ray"
    "Start X" "double" "user"
    "Start Y" "double" "user"
    "2nd X" "double" "user"
    "2nd Y" "double" "user"
    "Vector X" "double" "system"
    "Vector Y" "double" "system"
    "END" "END" "END"))

    "geometry-text-multi-properties)
  (vector
    "Geometry"
    "Text Multi"
    "Position X" "double" "user"
    "Position Y" "double" "user"
    "END" "END" "END"))

(define GroupBox3[] =
  (vector
    "Text"
    "Text Single"
    "Contents" "string" "user"
    "Font" "dropdown" "user"
    "Justify" "dropdown" "user"
    "Height" "double" "user"
    "Rotation" "double" "user"
    "END" "END" "END"))

(define GroupBox2[] =
  (vector
    "Geometry"
    "Text Single"
    "Position X" "double" "user"
    "Position Y" "double" "user"
    "END" "END" "END"))

(define GroupBox1[] =
  (vector
    "Misc"
    "Text Single"
    "Backward" "int" "user"
    "Upside Down" "int" "user"
    "END" "END" "END"))

(define General1[] =
  (vector
    "Layer (Toolbutton" " combobox)"
    "Color (toolbutton" " combobox)"
    "LineType (toolbutton" " combobox)"
    "LineWeight (toolbutton" " combobox)"
    "END" "END" "END"))

(define Path[] =
  (vector
    "VertexNum (toolbutton" " combobox)"
    "VertexX (toolbutton" " lineedit)"
    "VertexY (toolbutton" " lineedit)"
    "Area (toolbutton" " lineedit)"
    "Length (toolbutton" " lineedit)"
    "Closed (toolbutton" " combobox)"
    "END" "END" "END"))

    "Polyline)
  (vector
    "Polyline"
    "VertexNum (toolbutton" " combobox)"
    "VertexX (toolbutton" " lineedit)"
    "VertexY (toolbutton" " lineedit)"
    "Area (toolbutton" " lineedit)"
    "Length (toolbutton" " lineedit)"
    "Closed (toolbutton" " combobox)"
    "END" "END" "END"))

(define Ray)
    "X1 (toolbutton" " lineedit)"
    "Y1 (toolbutton" " lineedit)"
    "X2 (toolbutton" " lineedit)"
    "Y2 (toolbutton" " lineedit)"
    "VectorX (toolbutton" " lineedit)"
    "VectorY (toolbutton" " lineedit)"
    "END" "END" "END"))

(define TextMulti)
    "X (toolbutton" " lineedit)"
    "Y (toolbutton" " lineedit)"
    "END" "END" "END"))

    "error-msg", "None",
    "uname-string", "None",
    "current-file-name", "None",
    "arc-properties", "None",
    "details-labels", "None",
    "statusbar-labels", "None",

    "preview-active", "0",
    "moving-active", "0",
    "panning-active", "0",
    "rapid-move-active", "0",
    "pasting-active", "0",
    "selecting-active", "0",
    "zoom-window-active", "0",
    "gripping-active", "0",
    "general-tip-of-the-day", "0",
    "ruler-metric", "1",
    "display-use-open-gl", "0",
    "display-renderhint-aa", "0",
    "display-renderhint-text-aa", "0",
    "display-renderhint-smooth-pix", "0",
    "display-renderhint-high-aa", "0",
    "display-renderhint-noncosmetic", "0",
    "display-show-scrollbars", "1",
    "display-scrollbar-widget-num", "0",
    "display-zoom-scale-action-in", "0.0",
    "display-zoom-scale-action-out", "0.0",
    "display-selectbox-alpha", "0",
    "display-crosshair-percent", "0",
    "zoom-in-limit", "0.00001",
    "zoom-out-limit", "10000.0",

    "grid-size-radius",
    "grid-spacing-radius",
    "grid-spacing-angle",
    "ruler-show-on-load",
    "general-system-help-browser",
    "general-check-for-updates",
    "opensave-open-thumbnail",
    "opensave-save-thumbnail",
    "opensave-recent-max-files",
    "opensave-trim-dst-num-jumps",
    "printing-use-last-device",
    "printing-disable-bg",
    "qsnap-enabled",
    "qSnapToggle",
    "general-language[MAX-STRING-LENGTH]",
    "general-icon-theme[MAX-STRING-LENGTH]",
    "general-mdi-bg-logo[MAX-STRING-LENGTH]",
    "general-mdi-bg-texture[MAX-STRING-LENGTH]",
    "general-mdi-bg-color",
unsigned short general-current-tip",
    "opensave-open-format[MAX-STRING-LENGTH]",
    "opensave-save-format[MAX-STRING-LENGTH]",
    "opensave-recent-directory[MAX-STRING-LENGTH]",
    "printing-default-device[MAX-STRING-LENGTH]",
    "qsnap-locator-color",
    "grid-type[MAX-STRING-LENGTH]",
    "selection-coolgrip-color",
    "selection-hotgrip-color",
    "qsnap-locator-size",
    "qsnap-aperture-size",
text-properties text-style",
    "grip-size) 16",
    "pick-box-size) 16",
    "crosshair-size) 16",

int qsnap-mode[20]",
int state[20]",
int toggle-status[20]",
    "prefix[100]",
int shift-key-pressed-state",

 Should be a ring buffer.
    "undo-history", "(vector ""))
    "undo-history-position) 0)
    "undo-history-max) 500)

    "text-cursor) (vector 0 0))
    "text-display) (vector ""))

Variable data
    "obj_names[MAX_OBJECTS][MAX_STRING_LENGTH]",
    "details_label_text[12][MAX_STRING_LENGTH]",

int dialog_setting_int[100];
double dialog_setting_double[100];

    "text_font[MAX_STRING_LENGTH];
preview = settings.copy();
dialog = settings.copy();
accept = settings.copy();
    "opensave_custom_filter[MAX_STRING_LENGTH];

    "toolbar_entries[MAX_TOOLBARS][MAX_TOOLBAR_ENTRIES][MAX_STRING_LENGTH];
    "menu_entries[MAX_TOOLBARS][MAX_TOOLBAR_ENTRIES][MAX_STRING_LENGTH];
    "statusbar_label[MAX_MENU_LENGTH][MAX_STRING_LENGTH];

 Settings Editor
 ---------------

Setting
grid_ruler_misc_settings[] = {;
    ;{
        "Initially show grid when loading a file",
        INT_GRID_SHOW_ON_LOAD,
        "int",
        0,
        1,
        0,
        0,
        ALIGN_LEFT
 ;   ),
  ;  {
        "Show the origin when the grid is enabled",
        INT_GRID_SHOW_ORIGIN,
        "int",
        0,
        1,
        1,
        0,
        ALIGN_LEFT
    ),
    {
        "END",
        -1,
        -1,
        -1,
        -1,
        -1,
        -1,
        -1
    )
);

Setting
grid_ruler_color_settings[] = {
    {
        "Match grid color to crosshair color",
        INT_GRID_COLOR_MATCH_CROSSHAIR,
        "int", 0, 1, 0, 0, ALIGN_LEFT
    },
    {
        "END", -1, -1, -1, -1, -1, -1, -1
    }
)

    "grid_ruler_boxes)
SettingBox
grid_ruler_boxes[] = {
    (vector
          "Grid Misc",
        grid_ruler_misc_settings
    ),
    (vector
        "Grid Color",
        grid_ruler_color_settings
    )
)

(define 
SettingsTab grid_ruler_settings = {
    "Grid/Ruler",
    2,
    grid_ruler_boxes
) */

    /* Toolbars */
    "file-toolbar length", "7",
    "file-toolbar 0", "new",
    "file-toolbar 1", "open",
    "file-toolbar 2", "save",
    "file-toolbar 3", "save_as",
    "file-toolbar 4", "print",
    "file-toolbar 5", "design_details",
    "file-toolbar 6", "help",

    "edit-toolbar length", "5",
    "edit-toolbar 0", "undo",
    "edit-toolbar 1", "redo",
    "edit-toolbar 2", "cut",
    "edit-toolbar 3", "copy",
    "edit-toolbar 4", "paste",

    "view-toolbar length", "2",
    "view-toolbar 0", "day",
    "view-toolbar 1", "night",

    "zoom-toolbar length", "9",
    "zoom-toolbar 0", "zoom-window",
    "zoom-toolbar 1", "zoom-dynamic",
    "zoom-toolbar 2", "zoom-scale",
    "zoom-toolbar 3", "zoom-center",
    "zoom-toolbar 4", "zoom-in",
    "zoom-toolbar 5", "zoom-out",
    "zoom-toolbar 6", "zoom-selected",
    "zoom-toolbar 7", "zoom-all",
    "zoom-toolbar 8", "zoom-extents",

    "pan-toolbar length", "6",
    "pan-toolbar 0", "pan-real-time",
    "pan-toolbar 1", "pan-point",
    "pan-toolbar 2", "pan-left",
    "pan-toolbar 3", "pan-right",
    "pan-toolbar 4", "pan-up",
    "pan-toolbar 5", "pan-down",

    "icon-toolbar length", "6",
    "icon-toolbar 0", "icon16",
    "icon-toolbar 1", "icon24",
    "icon-toolbar 2", "icon32",
    "icon-toolbar 3", "icon48",
    "icon-toolbar 4", "icon64",
    "icon-toolbar 5", "icon128",

    "help-toolbar length", "4",
    "help-toolbar 0", "help",
    "help-toolbar 1", "changelog",
    "help-toolbar 2", "about",
    "help-toolbar 3", "whats_this",

    "layer-toolbar length", "1",
    "layer-toolbar 0", "layer-previous",

    "text-toolbar length", "1",
    "text-toolbar 0", "text-bold",

    "properties-toolbar length", "1",
    "properties-toolbar 0", "property-editor",

    "other-toolbar length", "1",
    "other-toolbar 0", "sandbox",

    /* User Interface */
    "toolbar-width", "632",
    "icon-padding", "4",

    "file-toolbar-visible", "1",
    "edit-toolbar-visible", "1",
    "view-toolbar-visible", "1",
    "window-toolbar-visible", "1",

    /* Terminator Symbols */
    "END", "END"
};

int icon_size = 24;
int toolbar_padding = 4;
int menu_item_height = 20;
int menubar_height = 24;
int menubar_padding = 2;

char statusbar_message[MAX_STRING_LENGTH]
    = "Embroidermodder 2.0.0-alpha";

char undo_history[2*MAX_VARIABLES][MAX_STRING_LENGTH] = {
    "END", "END"
};

unsigned char clear_color[] = {50, 50, 50};
unsigned char toolbar_bg_color[] = {150, 150, 200};
unsigned char bg_color[] = {100, 150, 210};
unsigned char menubar_color[] = {200, 200, 250};
unsigned char white[] = {255, 255, 255};

char *icon_layout[] = {
    "1", "1", "new",
    "2", "1", "open",
    "3", "1", "save",
    "4", "1", "save-as",
    "5", "1", "cut",
    "6", "1", "copy",
    "7", "1", "paste",
    "8", "1", "delete",
    "9", "1", "undo",
    "10", "1", "redo",
    /*
    "11", "1", "check-for-updates",
    "12", "1", "select-all",
    */
    "13", "1", "whats-this",
    "14", "1", "design-details",
    "15", "1", "print-pattern",
    "16", "1", "exit-program",

    "17", "1", "window-close",
    "18", "1", "window-close-all",
    "19", "1", "window-cascade",
    "20", "1", "window-tile",
    "21", "1", "window-next",
    "22", "1", "window-previous",

    "1", "2", "help",
    /*
    "2", "2", "changelog-dialog",
    */
    "3", "2", "tip-of-the-day-dialog",
    "4", "2", "about-dialog",

    "5", "2", "icon-16",
    "6", "2", "icon-24",
    "7", "2", "icon-32",
    "8", "2", "icon-48",
    "9", "2", "icon-64",
    "10", "2", "icon-128",

    "11", "2", "settings-dialog",
    "12", "2", "make-layer-current",

    "13", "2", "pan-real-time",
    "14", "2", "pan-point",
    "15", "2", "pan-left",
    "16", "2", "pan-right",
    "17", "2", "pan-up",
    "18", "2", "pan-down",

    "19", "2", "day",
    "20", "2", "night",

    "1", "3", "layers",
    "2", "3", "layer-selector",
    "3", "3", "layer-previous",
    "4", "3", "color-selector",
    "5", "3", "line-type-selector",
    "6", "3", "line-weight-selector",
    "7", "3", "hide-all-layers",
    "8", "3", "show-all-layers",
    "9", "3", "freeze-all-layers",
    "10", "3", "thaw-all-layers",
    "11", "3", "lock-all-layers",
    "12", "3", "unlock-all-layers",

    "13", "3", "text-bold",
    "14", "3", "text-italic",
    "15", "3", "text-underline",
    "16", "3", "text-strikeout",
    "17", "3", "text-overline",
    
    "18", "3", "zoom-real-time",
    "19", "3", "zoom-previous",
    "20", "3", "zoom-window",
    "21", "3", "zoom-dynamic",
    "22", "3", "zoom-scale",
    "23", "3", "zoom-center",
    "24", "3", "zoom-in",
    "25", "3", "zoom-out",
    "26", "3", "zoom-selected",
    "27", "3", "zoom-all",
    "28", "3", "zoom-extents",

    "1", "4", "treble-clef",
    "2", "4", "path",
    "3", "4", "circle",
    "4", "4", "line",
    "5", "4", "distance",
    "6", "4", "dolphin",
    "7", "4", "ellipse",

    "8", "4", "heart",
    "9", "4", "locate-point",
    "10", "4", "move",
    /*
    "11", "4", "export",
    "12", "4", "heart4",
    "13", "4", "heart5",
    "14", "4", "single-line-text",
    "15", "4", "spell-check",
    "16", "4", "quick-select"
    "17", "4", "rectangle",
    "18", "4", "rgb",
    "19", "4", "rotate",
    "20", "4", "sandbox",
    "21", "4", "quickleader",
    "22", "4", "snowflake",
    "23", "4", "star",
    "24", "4", "platform",
    "25", "4", "point",
    "26", "4", "polygon",
    "27", "4", "polyline",
    "28", "4", "settings-dialog",
    "29", "4", "quickleader",
    "30", "4", "locate-point",
    "31", "4", "point",
    */
    "END", "END", "END"
};

char *file_labels[MAX_STRING_LENGTH] = {
    "File", "file-menu",
    "New", "new",
    "Open", "open",
    "---", "---",
    "Save", "save",
    "Save as...", "save-as",
    "Export", "export",
    "---", "---",
    "Print", "print",
    "---", "---",
    "Design Details", "design-details",
    "---", "---",
    "Exit", "exit-program",
    "END", "END"
};

char *edit_labels[MAX_STRING_LENGTH] = {
    "Edit", "edit-menu",
    "Undo", "undo",
    "Redo", "redo",
    "---", "---",
    "Cut", "cut",
    "Copy", "copy",
    "Paste", "paste",
    "END", "END"
};

char *view_labels[MAX_STRING_LENGTH] = {
    "View", "view-menu",
    "Day", "day-vision",
    "Night", "night-vision",
    "---", "---",
    "Icon 16", "icon-16",
    "Icon 24", "icon-24",
    "Icon 32", "icon-32",
    "Icon 48", "icon-48",
    "Icon 64", "icon-64",
    "Icon 128", "icon-128",    
    "END", "END"
};

char *settings_labels[MAX_STRING_LENGTH] = {
    "Settings", "settings-menu",
    "Settings Dialog", "settings-dialog",
    "END", "END"
};

char *zoom_labels[MAX_STRING_LENGTH] = {
    "Zoom", "zoom-menu",
    "Zoom Real Time", "zoom-real-time",
    "Zoom Previous", "zoom-previous",
    "---", "---",
    "Zoom Window", "zoom-window",
    "Zoom Dynamic", "zoom-dynamic",
    "Zoom Scale", "zoom-scale",
    "Zoom Center", "zoom-center",
    "---", "---",
    "Zoom In", "zoom-in",
    "Zoom Out", "zoom-out",
    "---", "---",
    "Zoom Selected", "zoom-selected",
    "Zoom All", "zoom-all",
    "Zoom Extents", "zoom-extents",
    "END", "END"
};

char *pan_labels[MAX_STRING_LENGTH] = {
    "Pan", "pan-menu",
    "Pan Real Time", "pan-real-time",
    "Pan Point", "pan-point",
    "---", "---",
    "Pan Left", "pan-left",
    "Pan Right", "pan-right",
    "Pan Up", "pan-up",
    "Pan Down", "pan-down",
    "END", "END"
};
 
char *window_labels[MAX_STRING_LENGTH] = {
    "Window", "window-menu",
    "Window Cascade", "window-cascade",
    "END", "END"
};

char *help_labels[MAX_STRING_LENGTH] = {
    "Help", "help-menu",
    "Help", "help",
    "---", "---",
    "Changelog", "changelog",
    "---", "---",
    "Tip of the Day", "tip-of-the-day",
    "---", "---",
    "About", "about",
    "---", "---",
    "What's This?", "whats-this",
    "END", "END"
};

char *recent_labels[MAX_STRING_LENGTH] = {
    "Recent", "recent-menu",
    "None", "do-nothing",
    "END", "END"
};

int selected_items[MAX_SELECTED];
int n_selected = 0;

const char *symbols_docstring =
    "Symbols use the SVG path syntax.\n" \
    "\n" \
    "In theory, we could combine the icons and symbols systems,\n" \
    "since they could be rendered once and stored as icons in Qt.\n" \
    "(Or as textures in FreeGLUT.)\n" \
    "\n" \
    "Also we want to render the patterns themselves using SVG\n" \
    "syntax, so it would save on repeated work overall.\n";
    
