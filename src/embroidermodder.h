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

#ifndef EMBROIDERMODDER_H
#define EMBROIDERMODDER_H

#define VERSION           "2.0.0-alpha"

#include <iostream>
#include <vector>
#include <unordered_map>

#include <embroidery.h>

#include <GL/glew.h>

#define MAX_PATTERNS                 50

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

/* Permissions System(Not implemented)
 * ------------------------------------
 *
 * The permissions flag determines whether the user or the system can run this
 * action.
 */
#define PERMISSIONS_USER              0
#define PERMISSIONS_SYSTEM            1

#define MITER_JOIN                    0

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
#define OBJ_TYPE_BASE                 0
#define OBJ_TYPE_ARC                  1
#define OBJ_TYPE_BLOCK                2
#define OBJ_TYPE_CIRCLE               3
#define OBJ_TYPE_DIMALIGNED           4
#define OBJ_TYPE_DIMANGULAR           5
#define OBJ_TYPE_DIMARCLENGTH         6
#define OBJ_TYPE_DIMDIAMETER          7
#define OBJ_TYPE_DIMLEADER            8
#define OBJ_TYPE_DIMLINEAR            9
#define OBJ_TYPE_DIMORDINATE         10
#define OBJ_TYPE_DIMRADIUS           11
#define OBJ_TYPE_ELLIPSE             12
#define OBJ_TYPE_ELLIPSEARC          13
#define OBJ_TYPE_RUBBER              14
#define OBJ_TYPE_GRID                15
#define OBJ_TYPE_HATCH               16
#define OBJ_TYPE_IMAGE               17
#define OBJ_TYPE_INFINITELINE        18
#define OBJ_TYPE_LINE                19
#define OBJ_TYPE_PATH                20
#define OBJ_TYPE_POINT               21
#define OBJ_TYPE_POLYGON             22
#define OBJ_TYPE_POLYLINE            23
#define OBJ_TYPE_RAY                 24
#define OBJ_TYPE_RECTANGLE           25
#define OBJ_TYPE_SLOT                26
#define OBJ_TYPE_SPLINE              27
#define OBJ_TYPE_TEXTMULTI           28
#define OBJ_TYPE_TEXTSINGLE          29
#define OBJ_TYPE_UNKNOWN             30

/* CAD Linetypes
 *(CONT, CENTER, DOT, HIDDEN, PHANTOM, ZIGZAG)
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

//(int, QVariant)
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

#define  ENABLE_LWT         "ENABLE_LWT"
#define  ENABLE_REAL       "ENABLE_REAL"

#define  VIEW_COLOR_CROSSHAIR   "VIEW_COLOR_CROSSHAIR"
#define  VIEW_COLOR_GRID        "VIEW_COLOR_GRID"

/* TYPEDEFS
 * -----------------------------------------------------------------------------
 */

typedef struct Action_ {
    std::string command;
    std::string menu_label;
    std::string description;
} Action;

typedef struct Icon_ {
    std::string fname;
    std::string command;
    GLuint texture_id;
} Icon;

typedef struct Setting_ {
    bool bool_value;
    double real_value;
    int int_value;
    std::string string_value;
    int type;
} Setting;

typedef struct LineEdit_ {
    std::string label;
    std::string icon;
    std::string type;
    std::string signal;
    bool user_editable;
} LineEdit;

typedef struct GroupBox_ {
    std::string title;
    std::vector<LineEdit> line_edits;
    int obj_type;
} GroupBox;

typedef std::vector<std::vector<std::string>> string_matrix;

/* Function Prototypes.
 * -----------------------------------------------------------------------------
 */
void set_style(void);
void load_configuration(void);
void actuator(std::string command);
void load_menu(std::string menu_label, std::vector<std::vector<std::string>> menu_layout);
void load_toolbar(std::string menu_label, std::vector<std::vector<std::string>> toolbar_layout);
int render_vector_graphic(std::vector<std::string> description);

void about_dialog(void);
void changelog_dialog(void);
void help_dialog(void);
void alert(void);

std::string translate(std::string string);
void debug_message(std::string string);

void pattern_view(void);

/* Global variables.
 * -----------------------------------------------------------------------------
 */

extern bool running;
extern bool debug_mode;
extern bool show_about_dialog;
extern bool show_editor;
extern bool show_rulers;
extern bool show_grid;
extern int icon_size;
extern std::string language;
extern int pattern_index;
extern int n_patterns;
extern std::string current_fname;
extern std::string assets_dir;
//extern std::vector<Action> action_list;
extern EmbPattern* pattern_list[MAX_PATTERNS];
extern string_matrix translation_table;
extern std::unordered_map<std::string, string_matrix> menu_layout;
extern std::string os_seperator;
extern std::string settings_general_language;
extern std::string settings_general_icon_theme;
extern int settings_general_icon_size;
extern bool settings_general_mdi_bg_use_logo;
extern bool settings_general_mdi_bg_use_texture;
extern bool settings_general_mdi_bg_use_color;
extern std::string settings_general_mdi_bg_logo;
extern std::string settings_general_mdi_bg_texture;
extern unsigned int settings_general_mdi_bg_color;
extern bool settings_general_tip_of_the_day;
extern unsigned int settings_general_current_tip;
extern bool settings_general_system_help_browser;
extern bool settings_general_check_for_updates;
extern bool settings_display_use_opengl;
extern bool settings_display_renderhint_aa;
extern bool settings_display_renderhint_text_aa;
extern bool settings_display_renderhint_smooth_pix;
extern bool settings_display_renderhint_high_aa;
extern bool settings_display_renderhint_noncosmetic;
extern bool settings_display_show_scrollbars;
extern int settings_display_scrollbar_widget_num;
extern unsigned int settings_display_crosshair_color;
extern unsigned int settings_display_bg_color;
extern unsigned int settings_display_selectbox_left_color;
extern unsigned int settings_display_selectbox_left_fill;
extern unsigned int settings_display_selectbox_right_color;
extern unsigned int settings_display_selectbox_right_fill;
extern unsigned char settings_display_selectbox_alpha;
extern double settings_display_zoomscale_in;
extern double settings_display_zoomscale_out;
extern unsigned char settings_display_crosshair_percent;
extern std::string settings_display_units;
extern unsigned int settings_prompt_text_color;
extern unsigned int settings_prompt_bg_color;
extern std::string settings_prompt_font_family;
extern std::string settings_prompt_font_style;
extern unsigned char settings_prompt_font_size;
extern bool settings_prompt_save_history;
extern bool settings_prompt_save_history_as_html;
extern std::string settings_prompt_save_history_filename;
extern std::string settings_opensave_custom_filter;
extern std::string settings_opensave_open_format;
extern bool settings_opensave_open_thumbnail;
extern std::string settings_opensave_save_format;
extern bool settings_opensave_save_thumbnail;
extern unsigned char settings_opensave_recent_max_files;
extern std::vector<std::string> settings_opensave_recent_list_of_files;
extern std::string settings_opensave_recent_directory;
extern unsigned char settings_opensave_trim_dst_num_jumps;
extern std::string settings_printing_default_device;
extern bool settings_printing_use_last_device;
extern bool settings_printing_disable_bg;
extern bool settings_grid_show_on_load;
extern bool settings_grid_show_origin;
extern bool settings_grid_color_match_crosshair;
extern unsigned int settings_grid_color;
extern bool settings_grid_load_from_file;
extern std::string settings_grid_type;
extern bool settings_grid_center_on_origin;
extern double settings_grid_center_x;
extern double settings_grid_center_y;
extern double settings_grid_size_x;
extern double settings_grid_size_y;
extern double settings_grid_spacing_x;
extern double settings_grid_spacing_y;
extern double settings_grid_size_radius;
extern double settings_grid_spacing_radius;
extern double settings_grid_spacing_angle;
extern bool settings_ruler_show_on_load;
extern bool settings_ruler_metric;
extern unsigned int settings_ruler_color;
extern unsigned char settings_ruler_pixel_size;
extern bool settings_qsnap_enabled;
extern unsigned int settings_qsnap_locator_color;
extern unsigned char settings_qsnap_locator_size;
extern unsigned char settings_qsnap_aperture_size;
extern bool settings_qsnap_endpoint;
extern bool settings_qsnap_midpoint;
extern bool settings_qsnap_center;
extern bool settings_qsnap_node;
extern bool settings_qsnap_quadrant;
extern bool settings_qsnap_intersection;
extern bool settings_qsnap_extension;
extern bool settings_qsnap_insertion;
extern bool settings_qsnap_perpendicular;
extern bool settings_qsnap_tangent;
extern bool settings_qsnap_nearest;
extern bool settings_qsnap_apparent;
extern bool settings_qsnap_parallel;
extern bool settings_lwt_show_lwt;
extern bool settings_lwt_real_render;
extern double settings_lwt_default_lwt;
extern bool settings_selection_mode_pickfirst;
extern bool settings_selection_mode_pickadd;
extern bool settings_selection_mode_pickdrag;
extern unsigned int settings_selection_coolgrip_color;
extern unsigned int settings_selection_hotgrip_color;
extern unsigned char settings_selection_grip_size;
extern unsigned char settings_selection_pickbox_size;
extern std::string settings_text_font;
extern double settings_text_size;
extern double settings_text_angle;
extern bool settings_text_style_bold;
extern bool settings_text_style_italic;
extern bool settings_text_style_underline;
extern bool settings_text_style_overline;
extern bool settings_text_style_strikeout;
extern std::vector<std::string> undo_history;

#endif /* EMBROIDERMODDER_H */
