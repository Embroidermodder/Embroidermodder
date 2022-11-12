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

#ifndef EMBROIDERMODDER_2_HEADER
#define EMBROIDERMODDER_2_HEADER

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

/* COMPATIBILITY LAYER
 * -------------------
 * Note that the definition of the window struct is variable and based on what
 * is used here.
 */
#ifndef EM2_ANDROID
#define EM2_ANDROID                   0
#endif

#ifndef EM2_IOS
#define EM2_IOS                       0
#endif

#if __APPLE__
#define EM2_COCOA                     1
#else
#define EM2_COCOA                     0
#endif

#if _WIN32
#define EM2_WIN32                     1
#else
#define EM2_WIN32                     0
#endif

/* X11 covers the most types of systems, so we can define it as everything
 * that isn't one of the other systems.
 */
#define EM2_X11 !(EM2_ANDROID || EM2_IOS || EM2_COCOA || EM2_WIN32)

#if EM2_COCOA
#include <cocoa/cocoa.h>
#endif

#if EM2_WIN32
#ifndef UNICODE
#define UNICODE
#endif

#include <windows.h>
#include <wingdi.h>

LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
#endif

#if EM2_X11
#include <X11/Xlib.h>
#include <X11/Xutil.h>
#include <X11/Xos.h>

#ifndef _GNU_SOURCE
#define _GNU_SOURCE
#endif
#include <unistd.h>
#endif

#if EM2_ANDROID
/* Directory structure is flattened for the android build. */
#include "embroidery.h"
#include "stb_truetype.h"
#include "stb_image_write.h"
#else
#include "libembroidery/src/embroidery.h"
#include "libembroidery/src/stb/stb_truetype.h"
#include "libembroidery/src/stb/stb_image_write.h"
#endif

/* DEFINES
 * -------
 */
#define VERSION           "2.0.0-alpha"

#define MAX_SCREEN_SIZE           10000
#define MAX_STRING_LENGTH           200
#define MAX_ACTIONS                 300
#define MAX_OBJECTS                 300
#define MAX_SELECTED              10000
#define MAX_PATTERNS                 10
#define MAX_RECENT                   20
#define MAX_WIDGETS                1000
#define MAX_SCRIPT_LINES           1000
#define MAX_CSV_ROWS                100
#define MAX_CSV_COLUMNS              10
#define RECENT_FILES                 10
#define MAX_LAYERS                   30
#define MAX_SETTINGS_IN_BOX         100
#define MAX_SETTINGS_BOXES           10
#define MAX_KEYS                    256

#define TABLE(A) \
    char A[MAX_CSV_ROWS][MAX_CSV_COLUMNS][MAX_STRING_LENGTH]

#define WIDGET_MODE_BLOCK             0
#define WIDGET_MODE_TEXT              1
#define WIDGET_MODE_IMAGE             2
#define WIDGET_MODE_SVG               3
#define WIDGET_MODE_BACKGROUND        4
#define WIDGET_MODE_SPINBOX           5
#define WIDGET_MODE_COMBOBOX          6
#define WIDGET_MODE_CONTAINER         7

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

#define COLOR_BACKGROUND              0
#define COLOR_CROSSHAIR               1
#define COLOR_GRID                    2

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

/* object values
 * -------------
 *  
 * The object flag describing which object
 * the action is intended for or "Null" for global effects 
 * like duplicating a pattern.
 */
#define OBJ_TYPE_NULL                 0
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
#define PREVIEW_MODE_OFF              0
#define PREVIEW_MODE_MOVE             1
#define PREVIEW_MODE_ROTATE           2
#define PREVIEW_MODE_SCALE            3

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

#define ARCHITECTURAL                 0
#define DECIMAL                       1
#define ENGINEERING                   2
#define FRACTIONAL                    3
#define SCIENTIFIC                    4

#define DISABLE_GRID                  0
#define RECTANGULAR_GRID              1
#define CIRCULAR_GRID                 2
#define ISOMETRIC_GRID                3

#define ALIGN_LEFT                    0
#define ALIGN_RIGHT                   1
#define ALIGN_CENTER                  2

/* TYPEDEFS
 * -----------------------------------------------------------------------------
 */

typedef struct EmbWidget_ EmbWidget;
typedef struct EmbPanel_ EmbPanel;
typedef struct EmbWindow_ EmbWindow;

/* A system-agnostic rectangle for pixel locations.
 */
typedef struct Rect_ {
    int x;
    int y;
    int w;
    int h;
} Rect;

/* Image data for icons and renders.
 */
typedef struct Image_ {
    char fname[MAX_STRING_LENGTH];
} Image;

/* Can be used by both the system and the user
 * for creating UI elements (cosmetic=1) or
 * stitch designs (cosmetic=0).
 */
typedef struct EmbPen_ {
    EmbColor color;
    char join_style;
    char cosmetic;
    float width;
    int style;
} EmbPen;

/* User interface information for stitch fills.
 *
 * If the user wishes to draw with the brush to then
 * fill the color region in afterwards with stitches
 * then set cosmetic to 1.
 */
typedef struct EmbBrush_ {
    EmbColor color;
    char join_style;
    char cosmetic;
    float width;
    int style;
} EmbBrush;

/* Collects all the pens and brushes into one set of
 * user interface tools.
 *
 * "pen" without a prefix is the system pen, used in
 * drawing the user interface.
 *
 * "path" and "image" are for storing what the EmbPainter
 * has created in either a vectorised or rasterised
 * format respectively.
 */
typedef struct EmbPainter_ {
    EmbPen *pen;
    EmbPath *path;
    EmbImage *image;
    EmbPen *left_pen;
    EmbBrush *left_brush;
    EmbPen *right_pen;
    EmbBrush *right_brush;
    EmbPen *dir_pen;
    EmbBrush *dir_brush;
    int alpha;
    int box_dir;
} EmbPainter;

/* Widget: the basic unit of the GUI.
 */
struct EmbWidget_ {
    EmbWindow *window;
    EmbPanel *panel;
    Rect rect;
    Image *image;
    EmbPainter *painter;
    unsigned char color[4];
    char label[MAX_STRING_LENGTH];
    int mode;
    char command[MAX_STRING_LENGTH];
    char visibility;
    char active;

    /* Spinbox properties */
    char category[MAX_STRING_LENGTH];
    char name[MAX_STRING_LENGTH];
    float single_step;
    float range_lower;
    float range_upper;
    float value;
    float storage;
    int enabled;
    int visible;

    /* For settings this can act as either a settings container or a settings
     * editor, if it acts as an editor then it also stores the relevant data.
     *
     * To tell the difference, when the mode of .
     *
     * SettingBox box[MAX_SETTINGS_BOXES];
     * Setting settings[MAX_SETTINGS_IN_BOX];
     */
    char description[MAX_STRING_LENGTH];
    int index;
    char type[50];
    int min;
    int max;
    int row;
    int column;
    int align;

    /* Properties structs
     * ------------------
     * Covers Comboboxes, Line edits, Dropdowns etc.
     */
    char property_description[MAX_STRING_LENGTH];
    unsigned char property_permissions;
    unsigned char property_data_type;

    char propertybox_title[MAX_STRING_LENGTH];
    int propertybox_obj_type;
    char **propertybox_properties;
};

typedef struct EmbEvent_ {
    int type;
    int state[MAX_KEYS];
} EmbEvent;

/* This should be managed by libembroidery.
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

/* A layer in the pattern.
 */
typedef struct EMLayer_ {
    char name[MAX_STRING_LENGTH];
    EmbPattern *data;
    int visible;
    int frozen;
    int z_value;
    char color[10];
    char line_type[10];
    double line_weight;
} EMLayer;

/* EmbPanel: the container format for all widgets.
 *
 */
struct EmbPanel_ {
    char title[MAX_STRING_LENGTH];
    char fname[MAX_STRING_LENGTH];
    EmbPainter *painter;
    EmbWindow *window;
    EmbWidget *widgets[MAX_WIDGETS];
    int n_widgets;
    EmbPattern *pattern;
    EMLayer layer[MAX_LAYERS];
    int n_boxes;
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
    Rect area;
    int closeable;
    int use_logo;
    int use_texture;
    int use_color;
    int bg_logo;
    int bg_texture;
    int bg_color;
    EmbCircle circle_ghost;
    EmbRect rect_ghost;
};

typedef struct Ruler_ {
    EmbVector position;
    EmbVector size;
    float width;
    int metric;
    int feet;
    float little;
    float medium;
    float fraction;
} Ruler;

typedef struct ViewPort_ {
    Ruler vertical;
    Ruler horizontal;
} ViewPort;

/* The parent struct for our GUI.
 *
 * Tooltip manager: only one tooltip will show at a time,
 * so we can make the data part of the window struct.
 */
struct EmbWindow_ {
    TABLE(data);
    EmbPanel *panels[MAX_PATTERNS];
    int n_panels;
    int tab_index;
    int tabbed;
    int n_docs;
    int screen;
    int selected[MAX_SELECTED];
    int n_selected;
    int menu_state;
    int running;
    int undo_history_position;
    int selecting_active;
    int zoom_window_active;

    text_properties text_style;

    TABLE(undo_history);

#if EM2_WIN32
    HWND hwnd;
#endif

#if EM2_X11
    Display *display;
    Window window;
    GC gc;
#endif

    stbtt_fontinfo font;
    Rect dimension;
};

/*
 *  Function prototypes.
 */
int valid_file_format(char *fname);

Rect make_rectangle(int x, int y, int w, int h);

int render(EmbWindow *window);

void debug_message(char *msg);
char *translate(char *msg);

double emb_min(double x, double y);
double emb_max(double x, double y);
double emb_clamp(double lower, double x, double upper);

int click_detection(EmbWidget *w, int x, int y);

int find_mdi_window(char *file_name);

int load_to_buffer(void);
int save_from_buffer(void);
void display_buffer(void);

/* ui.c function declarations
 *
 * The tools necessary to abstract out what system the program is
 * running on known as the "compatibility layer" are here.
 *
 * TODO: create_icon is not needed when widget packing is established
 *       then we have add_widget(window_id, panel_id, char *fname)
 *       or something similar.
 */
EmbWindow *load_window_data(char *fname);
EmbWindow *create_window(char *fname);
EmbEvent process_input(EmbWindow *window);
void render_clear(EmbWindow *window, EmbColor clear_color);
void render_copy(EmbWindow *window, EmbPanel *panel, EmbImage image);
void render_rect(EmbWindow *window, EmbPanel *panel, EmbColor color, Rect rect);
void destroy_window(EmbWindow *window);
void wait(int);

void create_widget(EmbWindow *window, EmbPanel *panel, Rect rect, char *action_id);
void create_label(EmbWindow *window, EmbPanel *panel, int position[2], char *label, char *command, int visibility);
void create_ui_rect(EmbPanel *panel, Rect rect, EmbColor color, int visibility);
void create_icon(EmbPanel *panel, int n, int m, char *label);
void create_measurement_label(EmbPanel *panel, int offset, int yoffset, int spacing, char *label, float value);

int get_widget_by_label(EmbPanel *panel, char *label);
void set_visibility(EmbPanel *panel, char *label, int visibility);
void horizontal_rule(EmbPanel *panel, int position[2], int w, int visibility);
void vertical_rule(EmbPanel *panel, int position[2], int h, int visibility);

void draw_line(EmbPanel *panel, EmbLine line);
void draw_lines(EmbPanel *panel, EmbLine *line, int n_lines);
void draw_crosshair(EmbWindow *window, EmbPanel *panel);
void draw_rulers(EmbPanel *panel);

int build_menu(char *fname, int x_offset, int menu);

void create_scrollbars(EmbPanel *panel);
void create_grid(EmbPanel *panel);
void create_toolbars(EmbWindow *window, EmbPanel *panel);
void create_statusbar(EmbPanel *panel);
void create_menubar(EmbPanel *panel);
void create_view(EmbPanel *panel);
int create_window_tab(EmbWindow *window, char *fname);

void repaint(EmbPanel *panel);


/* For all user actions the actuator is called with the user action serialised
 * as a single, fixed length string. This allows for non-trivial operations on
 * the edit history because all the edit history is, is a text file with these
 * as lines.
 *
 * This is also how Embroidermodder can be scripted, because there is no
 * distinction, from the perspective of the software, between a CLI instruction
 * and a GUI instruction. However, the user console won't be present until a later
 * version as it is not core functionality.
 *
 * The actuator function is located by itself in src/actuator.c.
 */
int actuator(char *command);

/* File Actions */
void new_file(EmbWindow *window);
void open_file(EmbWindow *window);
void save_file(EmbWindow *window);
void save_file_as(EmbWindow *window);
void print(EmbWindow *window);

/* Edit Actions */
void undo(void);
void redo(void);
void cut(void);
void copy(void);
void paste(void);

/* Dialog Actions */
void about(void);
void tip_of_the_day(void);
void help(void);
void design_details(void);
void settings_dialog(void);
void whats_this(void);
void layer_selector(void);

int allow_zoom_in(EmbPanel *panel);
int allow_zoom_out(EmbPanel *panel);

void lineTypeSelector(void);
void lineWeightSelector(void);

void color_selector(void);

/* Icon Actions */
void icon_resize(int new_size);

void distance(void);
void locate_point(void);
void delete_object(void);
void line_type_selector(void);
void line_weight_selector(void);
void select_all(void);
void check_for_updates(void);
void print_pattern(void);
void move(void);
void export_(void);

void scene_update(EmbPanel *panel);

void set_override_cursor(char *cursor);
void restore_override_cursor(void);

int starts_with(char *str, char *start);

char *translate(char *string);

void crash_test(void);
void run_script(TABLE(script));

/* These drawing functions could do with a new file
 * seperate from ui.c because they are system-agnostic.
 */
EmbPen *create_pen(void);
EmbBrush *create_brush(void);
EmbPainter *create_painter(void);
void destroy_pen(EmbPen *pen);
void destroy_brush(EmbBrush *brush);
void destroy_painter(EmbPainter *painter);

/* data.c functions */
char *get_str(TABLE(state), char *key);
int get_int(TABLE(state), char *key);
float get_float(TABLE(state), char *key);
EmbVector get_vector(TABLE(state), char *key);
EmbColor get_color(TABLE(state), char *);

void set_str(TABLE(state), char *key, char *value);
void set_int(TABLE(state), char *key, int value);
void set_float(TABLE(state), char *key, float value);
void set_vector(TABLE(state), char *key, EmbVector value);

void load_translations(void);
void load_csv(TABLE(table), char *fname);
void load_state(void);
void print_table(TABLE(table));
void load_translations(void);

/* GLOBAL DATA */
extern int dialog_grid_load_from_file;

extern const char *os_seperator;
extern const char *em2_version;

extern int active_view;
extern int active_scene;
extern float dialog_qsnap_locator_size;
extern float dialog_qsnap_aperture_size;
extern float qsnap_aperture_size;
extern int n_tips;

/* Assets */
extern TABLE(tips);
extern TABLE(crash_test_script);
extern TABLE(tooltips);
extern TABLE(action_list);
extern TABLE(settings_state);

extern TABLE(grid_group_rectangular);
extern TABLE(grid_group_circular);

extern char interface_font[MAX_STRING_LENGTH];

extern char main_window_title[MAX_STRING_LENGTH];
extern Rect main_window_dimensions;

extern char settings_dialog_title[MAX_STRING_LENGTH];
extern Rect settings_dialog_dimensions;

extern char about_dialog_title[MAX_STRING_LENGTH];
extern Rect about_dialog_dimensions;

extern char property_editor_title[MAX_STRING_LENGTH];
extern Rect property_editor_dimensions;

extern char *statusbar_labels[MAX_STRING_LENGTH];

extern TABLE(shortcuts);
extern TABLE(global_state);

extern float max_distance;
extern int general_icon_size;

extern char text_font[MAX_STRING_LENGTH];

/* Spinboxen
Widget grid_center_x_sb;
Widget grid_center_y_sb;
Widget grid_size_x_sb;
Widget grid_size_y_sb;
Widget grid_size_radius_sb;
Widget grid_size_angle_sb;
Widget grid_spacing_x_sb;
Widget grid_spacing_y_sb;
Widget grid_spacing_radius_sb;
Widget grid_spacing_angle_sb;
 */
extern int dialog_grid_load_from_file;

extern int n_windows;

extern EmbColor black;
extern EmbColor background_color;
extern EmbColor interface_color;
extern EmbColor ruler_color;
extern EmbColor preview_ruler_color;
extern EmbColor qsnap_locator_color;

/* Grip settings. */
extern EmbColor selection_coolgrip_color;
extern EmbColor selection_hotgrip_color;

extern EmbColor button_background_color;

extern EmbColor grip_color_cool;
extern EmbColor grip_color_hot;
extern EmbColor crosshair_color;

extern int color_mode;
extern int grid_type;
extern int mdi_window;

extern char application_folder[MAX_STRING_LENGTH];

/*
QToolBar* toolbar[10]", "0",
QMenu* menu[10]", "0",
StatusBarButton* status-bar[8]", "0",
QToolButton* toolButton[PROPERTY-EDITORS]", "0",
QLineEdit* lineEdit[LINEEDIT-PROPERTY-EDITORS]", "0",
QComboBox* comboBox[COMBOBOX-PROPERTY-EDITORS]", "0",

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

extern EmbVector paste_delta;
extern EmbVector scene_press_point;
extern EmbVector press_point;
extern EmbVector scene_move_point;
extern EmbVector move_point;
extern EmbVector scene_release_point;
extern EmbVector release_point;
extern EmbVector scene_grip_point;
extern EmbVector view_mouse_point;
extern EmbVector scene_mouse_point;

extern int precision_angle ;
extern int precision_length ;

extern int statusBarMouseCoord ;

/* Used when checking if fields vary */
extern int field_old_text;
extern int field_new_text;
extern int field_varies_text;
extern int field_yes_text;
extern int field_no_text;
extern int field_on_text;
extern int field_off_text;

/* General settings */
extern char general_language[MAX_STRING_LENGTH];
extern char general_icon_theme[MAX_STRING_LENGTH];
extern char general_mdi_bg_logo[MAX_STRING_LENGTH];
extern char general_mdi_bg_texture[MAX_STRING_LENGTH];
extern EmbColor general_mdi_bg_color;
extern int general_current_tip;
extern int general_tip_of_the_day;
extern int general_system_help_browser;
extern int general_check_for_updates;
extern int general_mdi_bg_use_logo;
extern int general_mdi_bg_use_texture;
extern int general_mdi_bg_use_color;

extern int preview_mdi_bg_use_logo;
extern int preview_mdi_bg_use_texture;
extern int preview_mdi_bg_use_color;

extern unsigned char ttf_buffer[1<<25];

/* QSnap Properties */
extern int qsnap_endpoint;
extern int qsnap_midpoint;
extern int qsnap_center;
extern int qsnap_node;
extern int qsnap_quadrant;
extern int qsnap_intersection;
extern int qsnap_extension;
extern int qsnap_insertion;
extern int qsnap_perpendicular;
extern int qsnap_tangent;
extern int qsnap_nearest;
extern int qsnap_apparent;
extern int qsnap_parallel;
extern int qsnap_enabled;
extern float qsnap_locator_size;
extern int qsnap_active;
extern int qsnap_toggle;

/* Line Weight Settings */
extern int lwt_show_lwt;
extern int preview_lwt_show_lwt;
extern int lwt_real_render;
extern float lwt_default_lwt;

extern int preview_lwt_real_render;

extern char current_file_name[MAX_STRING_LENGTH];
extern int preview_mode;

/* Ruler Settings */
extern int ruler_metric;
extern int ruler_show_on_load;
extern int ruler_pixel_size;

/* Grid Settings */
extern float grid_size_radius;
extern float grid_size_angle;
extern int grid_center_on_origin;
extern EmbVector grid_center;
extern EmbVector grid_size;
extern EmbVector grid_spacing;
extern float grid_spacing_radius;
extern float grid_spacing_angle;
extern int grid_show_on_load;
extern int grid_show_origin;
extern int grid_color_match_crosshair;
extern int grid_load_from_file;
extern EmbColor grid_color;
extern EmbColor preview_grid_color;

extern const char *the_greig_message;

extern float symbol_scale;

/* Display Settings */
extern int display_use_opengl;
extern int display_renderhint_aa;
extern int display_renderhint_text_aa;
extern int display_renderhint_smooth_pix;
extern int display_renderhint_high_aa;
extern int display_renderhint_noncosmetic;
extern int display_show_scrollbars;
extern int display_scrollbar_widget_num;
extern int display_zoomscale_in;
extern int display_zoomscale_out;
extern int display_selectbox_alpha;
extern int preview_display_selectbox_alpha;
extern int display_crosshair_percent;

extern int preview_show_scrollbars;
extern float display_zoom_scale_action_in;
extern float display_zoom_scale_action_out;
extern float zoom_in_limit;
extern float zoom_out_limit;

/* Open/Save Settings */
extern char opensave_open_format[MAX_STRING_LENGTH];
extern char opensave_save_format[MAX_STRING_LENGTH];
extern char opensave_recent_directory[MAX_STRING_LENGTH];
extern char opensave_recent_files[MAX_RECENT][MAX_STRING_LENGTH];
extern char opensave_open_thumbnail[MAX_STRING_LENGTH];
extern char opensave_save_thumbnail[MAX_STRING_LENGTH];
extern int opensave_recent_max_files;
extern int opensave_trim_dst_num_jumps;

/* Printing Settings */
extern int printing_use_last_device;
extern int printing_disable_bg;

/* Selection Settings */
extern int selection_mode_pickfirst;
extern int selection_mode_pickadd;
extern int selection_mode_pickdrag;
extern int selection_grip_size;
extern int selection_pickbox_size;

/* Panning Settings */
extern int panning_real_time_active;
extern int panning_point_active;
extern int panning_active;

/* The Color class definition. */
extern int stitches_total;
extern int stitches_real;
extern int stitches_jump;
extern int stitches_trim;
extern int color_total;
extern int color_changes;

/* Display Settings */
extern EmbColor display_crosshair_color;
extern EmbColor display_bg_color;
extern EmbColor display_selectbox_left_color;
extern EmbColor display_selectbox_left_fill;
extern EmbColor display_selectbox_right_color;
extern EmbColor display_selectbox_right_fill;

extern EmbColor preview_display_crosshair_color;
extern EmbColor preview_display_bg_color;
extern EmbColor preview_display_selectbox_left_color;
extern EmbColor preview_display_selectbox_left_fill;
extern EmbColor preview_display_selectbox_right_color;
extern EmbColor preview_display_selectbox_right_fill;

extern int display_units;

extern int file_toolbar_visible;
extern int edit_toolbar_visible;
extern int view_toolbar_visible;
extern int window_toolbar_visible;

void load_csv(TABLE(table), char *fname);

extern int language;
extern TABLE(translation_tables[N_LANGUAGES]);

extern char preview_modes[][MAX_STRING_LENGTH];

extern TABLE(comboboxes);
extern TABLE(settings_tabs_data);
extern TABLE(obj_names);

extern char error_msg[MAX_STRING_LENGTH];
extern char uname_string[MAX_STRING_LENGTH];
extern char current_file_name[MAX_STRING_LENGTH];
extern char details_labels[MAX_STRING_LENGTH];

extern char general_language[MAX_STRING_LENGTH];
extern char general_icon_theme[MAX_STRING_LENGTH];
extern char general_mdi_bg_logo[MAX_STRING_LENGTH];
extern char general_mdi_bg_texture[MAX_STRING_LENGTH];
extern char opensave_open_format[MAX_STRING_LENGTH];
extern char opensave_save_format[MAX_STRING_LENGTH];
extern char opensave_recent_directory[MAX_STRING_LENGTH];
extern char printing_default_device[MAX_STRING_LENGTH];
extern float qsnap_locator_size;
extern float qsnap_aperture_size;

extern int qsnap_mode[20];
extern int state[20];
extern int toggle_status[20];
extern char prefix[100];
extern int shift_key_pressed_state;

/* Should be a ring buffer. */
extern TABLE(undo_history);
extern int undo_history_position;

extern int text_cursor[2];
extern TABLE(text_display);

extern char details_label_text[12][MAX_STRING_LENGTH];

extern int dialog_setting_int[100];
extern double dialog_setting_double[100];

extern TABLE(settings);
extern TABLE(preview);
extern TABLE(dialog);
extern TABLE(accepted_dialog);
extern char opensave_custom_filter[MAX_STRING_LENGTH];
extern char current_fname[MAX_STRING_LENGTH];
extern int line_n;

/* Program state */
extern int running;
extern int debug_mode;
extern int testing;
extern int gripping_active;
extern int preview_active;
extern int moving_active;
extern int rapid_move_active;
extern int pasting_active;
extern int selecting_active;
extern int zoom_window_active;
extern int shift_key_pressed_state;
extern int track_mode;
extern int n_patterns;
extern int n_docs;

/* Layout data */
extern int grip_size;
extern int pick_box_size;
extern int crosshair_size;
extern int icon_size;
extern int icon_padding;
extern int toolbar_padding;
extern int menu_item_height;
extern int menubar_height;
extern int menubar_padding;
extern int menu_state;

extern char statusbar_message[MAX_STRING_LENGTH];

extern EmbColor clear_color;
extern EmbColor toolbar_bg_color;
extern EmbColor bg_color;
extern EmbColor menubar_color;
extern EmbColor white_color;

extern int selected_items[MAX_SELECTED];
extern int n_selected;

extern TABLE(symbols_docstring);
    
/*
 *  Property box contents.
 */
extern TABLE(general_properties);

extern TABLE(arc_properties);
extern TABLE(block_properties);
extern TABLE(circle_properties);
extern TABLE(ellipse_properties);
extern TABLE(image_properties);
extern TABLE(infinite_line_properties);
extern TABLE(line_properties);
extern TABLE(rect_properties);
extern TABLE(polygon_properties);
extern TABLE(point_properties);
extern TABLE(general_properties);
extern TABLE(path_properties);
extern TABLE(polyline_properties);
extern TABLE(infinite_line_properties);
extern TABLE(path_properties);
extern TABLE(polyline_properties);
extern TABLE(ray_properties);
extern TABLE(text_single_properties);
extern TABLE(text_multi_properties);

extern float dialog_display_zoom_scale_action_in;
extern float dialog_display_zoom_scale_action_out;
extern int dialog_printing_disable_bg;

extern EmbColor preview_display_selectbox_color_left;
extern EmbColor preview_display_selectbox_fill_left;
extern EmbColor preview_display_selectbox_color_right;
extern EmbColor preview_display_selectbox_fill_right;
extern int preview_display_selectbox_alpha;

extern EmbColor accept_display_selectbox_color_left;
extern EmbColor accept_display_selectbox_fill_left;
extern EmbColor accept_display_selectbox_color_right;
extern EmbColor accept_display_selectbox_fill_right;
extern int accept_display_selectbox_alpha;


#endif
