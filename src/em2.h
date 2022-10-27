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
#define MAX_STRING_LENGTH           200
#define MAX_MENU_LENGTH              30
#define MAX_ACTIONS                 300
#define MAX_OBJECTS                 300
#define MAX_TOOLBARS                 30
#define MAX_TOOLBAR_ENTRIES          30
#define MAX_UNDO_HISTORY           1000
#define MAX_VARIABLES              5000
#define MAX_SELECTED              10000
#define RECENT_FILES                 10

#define WIDGET_MODE_BLOCK             0
#define WIDGET_MODE_TEXT              1
#define WIDGET_MODE_IMAGE             2
#define WIDGET_MODE_SVG               3
#define WIDGET_MODE_BACKGROUND        4

/* Permissions System (Not implemented)
 * ------------------------------------
 *
 * The permissions flag determines whether the user or
 * the system can run this action.
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
    void (*function)(void);
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

typedef struct Pen_ {
    EmbColor color;
    char join_style;
    char cosmetic;
    float width;
} Pen;

typedef struct Painter_ {
    Pen *pen;
    EmbPath *path;
} Painter;

typedef struct Tool_ {
    EmbColor color;
    int style;
} Tool;

typedef struct Toolset_ {
    Tool left_pen;
    Tool left_brush;
    Tool right_pen;
    Tool right_brush;
    Tool dir_pen;
    Tool dir_brush;
    int alpha;
    int box_dir;
} Toolset;

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
 */

/* Properties structs
 * ------------------
 * Covers Comboboxes, Line edits, Dropdowns etc.
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
 */

/* The parent struct for our GUI.
 */
typedef struct Window_ {
    EmbPattern* pattern[10];
    widget *widgets;
    int n_widgets;
    SDL_Window *window;
    SDL_Renderer *renderer;
    TTF_Font *font;
    char state[2*MAX_VARIABLES][MAX_STRING_LENGTH];
    scheme *sc;
} Window;

/*
 *  Function prototypes which aren't scheme objects.
 */
int valid_file_format(char *fname);

SDL_Rect make_rectangle(int x, int y, int w, int h);

Window *create_window(char state[2*MAX_VARIABLES][MAX_STRING_LENGTH]);
void main_loop(void);
void destroy_window(Window *w);
void process_input(Window *w);
int render(Window *w);

void init_state(Window *window, char state[2*MAX_VARIABLES][MAX_STRING_LENGTH]);
char *get_str(char dict[2*MAX_VARIABLES][MAX_STRING_LENGTH], char *key);
int get_int(char dict[2*MAX_VARIABLES][MAX_STRING_LENGTH], char *key);
float get_float(char dict[2*MAX_VARIABLES][MAX_STRING_LENGTH], char *key);
EmbVector get_vector(char dict[2*MAX_VARIABLES][MAX_STRING_LENGTH], char *key);
void set_str(char dict[2*MAX_VARIABLES][MAX_STRING_LENGTH], char *key, char *value);
void set_int(char dict[2*MAX_VARIABLES][MAX_STRING_LENGTH], char *key, int value);
void set_float(char dict[2*MAX_VARIABLES][MAX_STRING_LENGTH], char *key, float value);
void set_vector(char dict[2*MAX_VARIABLES][MAX_STRING_LENGTH], char *key, EmbVector value);

void debug_message(char *msg);
char *translate(char *msg);

double emb_min(double x, double y);
double emb_max(double x, double y);
double emb_clamp(double lower, double x, double upper);

int click_detection(widget *w, int x, int y);

int find_mdi_window(char *file_name);

int load_to_buffer();
int save_from_buffer();
void display_buffer();

void display_file_menu(void);
void display_edit_menu(void);
void display_view_menu(void);
void display_settings_menu(void);
void display_window_menu(void);
void display_help_menu(void);
void display_recent_menu(void);
void display_zoom_menu(void);
void display_pan_menu(void);

/* Operate on the window. */
void create_widget(Window *window, SDL_Rect rect, char *action_id);
void create_label(Window *window, int x, int y, char *label, char *command, int visibility);
void create_ui_rect(Window *window, SDL_Rect rect, unsigned char *color, int visibility);
void create_icon(Window *window, int n, int m, char *label);
int get_widget_by_label(Window *window, char *label);
void set_visibility(Window *window, char *label, int visibility);
void horizontal_rule(Window *window, int x, int y, int w, int visibility);
void vertical_rule(Window *window, int x, int y, int h, int visibility);

void get_args(pointer args, pointer arg[10], int n);

/* Action functions
 * ----------------
 *
 * These are all functions of the type:
 *     pointer function(void);
 *
 * Should match the order of the action id table above.
 */
void stub_testing(void);
void do_nothing(void);

/* File Actions */
void new_file(void);
void open_file(void);
void save_file(void);
void save_file_as(void);
void print(void);

/* Edit Actions */
void undo(void);
void redo(void);
void cut(void);
void copy(void);
void paste(void);

/* Dialog Actions */
void changelog(void);
void about(void);
void tip_of_the_day(void);
void help(void);
void design_details(void);
void settings_dialog(void);
void whats_this(void);
void layer_selector(void);

/* Window Actions */
void window_cascade(void);
void window_tile(void);
void window_close(void);
void window_close_all(void);
void window_next(void);
void window_previous(void);

void lineTypeSelector(void);
void lineWeightSelector(void);

void color_selector(void);

void hide_all_layers(void);
void show_all_layers(void);
void freeze_all_layers(void);
void thaw_all_layers(void);
void lock_all_layers(void);
void unlock_all_layers(void);
void make_layer_current(void);

/* Icon Actions */
void icon_16(void);
void icon_24(void);
void icon_32(void);
void icon_48(void);
void icon_64(void);
void icon_128(void);

/* Text Actions */
void text_italic(void);
void text_overline(void);
void text_underline(void);
void text_strikeout(void);
void text_bold(void);

/* Layer Actions */
void make_layer_active(void);
void layer_manager(void);
void layer_previous(void);

/* Zoom Actions */
void zoom_real_time(void);
void zoom_previous(void);
void zoom_window(void);
void zoom_dynamic(void);
void zoom_scale(void);
void zoom_center(void);
void zoom_in(void);
void zoom_out(void);
void zoom_selected(void);
void zoom_all(void);
void zoom_extents(void);

/* Pan SubMenu */
void pan_real_time(void);
void pan_point(void);
void pan_left(void);
void pan_right(void);
void pan_up(void);
void pan_down(void);

void day_vision(void);
void night_vision(void);

void distance(void);
void line(void);
void dolphin(void);
void treble_clef(void);
void locate_point(void);
void heart(void);
void delete_object(void);
void ellipse(void);
void circle(void);
void path(void);
void layers(void);
void line_type_selector(void);
void line_weight_selector(void);
void select_all(void);
void check_for_updates(void);
void print_pattern(void);
void exit_program(void);
void move(void);
void export_(void);

void decrement(char state[2*MAX_VARIABLES][MAX_STRING_LENGTH], char *key);
void increment(char state[2*MAX_VARIABLES][MAX_STRING_LENGTH], char *key);

void scene_update(void);

void set_override_cursor(char *);
void restore_override_cursor(void);

void menus_init(void);

EmbColor get_color(char state[2*MAX_VARIABLES][MAX_STRING_LENGTH], char *);

Pen *create_pen(void);
Painter *create_painter(void);
void destroy_pen(Pen *pen);
void destroy_painter(Painter *painter);

void draw_rulers();

/* Global Data */
extern Window *mainwnd;
extern Window *settings_dialog_wnd;
extern Window *about_dialog_wnd;
extern Window *property_editor_wnd;

extern Action action_list[MAX_ACTIONS];

extern char state[2*MAX_VARIABLES][MAX_STRING_LENGTH];
extern char undo_history[2*MAX_VARIABLES][MAX_STRING_LENGTH];

extern unsigned char clear_color[];
extern unsigned char toolbar_bg_color[];
extern unsigned char bg_color[];
extern unsigned char menubar_color[];
extern unsigned char white[];

extern char *icon_layout[];

extern char *file_labels[MAX_STRING_LENGTH];
extern char *edit_labels[MAX_STRING_LENGTH];
extern char *view_labels[MAX_STRING_LENGTH];
extern char *settings_labels[MAX_STRING_LENGTH];
extern char *zoom_labels[MAX_STRING_LENGTH];
extern char *pan_labels[MAX_STRING_LENGTH];
extern char *window_labels[MAX_STRING_LENGTH];
extern char *help_labels[MAX_STRING_LENGTH];
extern char *recent_labels[MAX_STRING_LENGTH];

extern int selected_items[MAX_SELECTED];
extern int n_selected;

extern const char *symbols_docstring;

extern char statusbar_message[MAX_STRING_LENGTH];

extern int running;
extern int testing;
extern int debug_mode;

extern int toolbar_padding;
extern int icon_size;
extern int menu_item_height;
extern int menubar_height;
extern int menubar_padding;

#endif

