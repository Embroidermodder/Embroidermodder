/*
 * Embroidermodder 2.
 *
 * Copyright 2011-2024 The Embroidermodder Team
 * Embroidermodder 2 is Open Source Software, see LICENSE.md for licensing terms.
 *
 * Read the reference manual (https://www.libembroidery.org/downloads/emrm.pdf)
 * for advice on altering this file.
 *
 * C Core
 */

#ifndef EMBROIDERMODDER_C_CORE_HEADER
#define EMBROIDERMODDER_C_CORE_HEADER

#ifdef __cplusplus
extern "C" {
#endif

#include <inttypes.h>
#include <stdlib.h>
#include <stdbool.h>
#include <stdarg.h>
#include <math.h>

#include "../extern/libembroidery/embroidery.h"

#include "constants.h"

#define REAL(arg) context->argument[arg].r
#define INT(arg)  context->argument[arg].i
#define STR(arg)  context->argument[arg].s
#define BOOL(arg) context->argument[arg].b

#define MAX_LONG_STRING             1000
#define MAX_TABLE_LENGTH             500
#define MAX_OPEN_FILES               100

typedef char EmbStringTable[MAX_TABLE_LENGTH][MAX_STRING_LENGTH];

typedef struct ScriptValue_ {
    EmbReal r;
    int i;
    bool b;
    EmbString s;
    EmbString label;
    int n_leaves;
    char type;
} ScriptValue;

typedef struct ScriptEnv_ {
    ScriptValue *variable;
    int n_variables;
    int context;
    int mode;
    bool firstRun;
    ScriptValue argument[MAX_ARGS];
    int argumentCount;
} ScriptEnv;

typedef struct Command_ {
    int32_t id;
    EmbString command;
    EmbString arguments;
    EmbString icon;
    EmbString tooltip;
    EmbString statustip;
    EmbString alias;
    EmbString shortcut;
    int32_t flags;
} Command;

typedef struct EmbDimLeader_ {
    EmbVector start;
    EmbVector end;
    int arrow_style;
    int line_style;
    EmbColor color;
} EmbDimLeader;

typedef struct Setting_ {
    EmbString key;
    ScriptValue setting;
    ScriptValue dialog;
    ScriptValue accept;
    ScriptValue preview;
} Setting;

typedef struct LabelledVector_ {
    EmbString key;
    EmbVector vector;
} LabelledVector;

typedef struct StringMap_ {
    EmbString key;
    EmbString value;
} StringMap;

typedef struct Editor_ {
    EmbString icon;
    EmbString label;
    EmbString data_type;
    EmbString signal;
    int object;
} Editor;

typedef struct GroupBoxData_ {
    int32_t id;
    const char *key;
    const char *label;
    Editor *data;
} GroupBoxData;

typedef struct SettingsData_ {
    int id;
    EmbString label;
    EmbString key;
    EmbString icon;
    EmbString value;
    EmbString editor_data;
    int type;
} SettingsData;

typedef struct ObjectCore_ {
    int32_t doc;

    EmbGeometry *geometry;

    EmbVector position;

    EmbReal scale;
    EmbReal rotation;

    uint32_t rgb;

    EmbString OBJ_NAME;

    EmbString text;
    EmbString textFont;
    EmbString textJustify;
    EmbReal textSize;
    bool textBold;
    bool textItalic;
    bool textUnderline;
    bool textStrikeOut;
    bool textOverline;
    bool textBackward;
    bool textUpsideDown;

    int rubber_mode;
    uint32_t objID;

    bool curved;
    bool filled;
    EmbReal arrowStyleAngle;
    EmbReal arrowStyleLength;
    EmbReal lineStyleAngle;
    EmbReal lineStyleLength;

    int gripIndex;
} ObjectCore;

typedef struct UndoData_ {
    EmbString label;
    int type;
    int32_t doc;
    EmbVector delta;
    EmbVector pivot;
    EmbReal angle;
    EmbReal factor;
    EmbString navType;
    EmbVector fromCenter;
    EmbVector toCenter;
    bool done;
    EmbVector before;
    EmbVector after;
    EmbLine mirrorLine;
    uint32_t obj;
} UndoData;

typedef struct EmbVectorList_ {
    EmbVector *data;
    int count;
    int size;
} EmbVectorList;

typedef struct EmbIdList_ {
    int32_t *data;
    int count;
    int size;
} EmbIdList;

/* . */
typedef struct DocumentData_ {
    EmbPattern *pattern;

    UndoData undo_stack[MAX_UNDO];

    int32_t id;

    EmbIdList *selectedItems;
    EmbIdList *rubberRoomList;
    EmbIdList *previewObjectList;
    EmbIdList *spareRubberList;
    EmbIdList *cutCopyObjectList;
    EmbIdList *previewObjectItemGroup;
    EmbIdList *hashDeletedObjects;
    EmbIdList *pasteObjectItemGroup;

    EmbVectorList *selectedGripPoints;

    /* UI State */
    bool grippingActive;
    bool rapidMoveActive;
    bool previewActive;
    bool pastingActive;
    bool movingActive;
    bool selectingActive;
    bool zoomWindowActive;
    bool panningRealTimeActive;
    bool panningPointActive;
    bool panningActive;
    bool qSnapActive;
    bool qSnapToggle;

    EmbVector previewPoint;
    EmbReal previewData;
    int previewMode;

    EmbVector viewMousePoint;
    EmbVector sceneMousePoint;
    EmbVector sceneQSnapPoint;

    uint8_t qsnapLocatorSize;
    uint32_t qsnapLocatorColor;
    uint8_t qsnapApertureSize;

    uint32_t gripColorCool;
    uint32_t gripColorHot;
    uint32_t gridColor;
    uint8_t gripSize;
    uint8_t pickBoxSize;

    uint32_t crosshairColor;
    uint32_t crosshairSize;

    EmbVector scenePressPoint;
    EmbVector pressPoint;
    EmbVector sceneMovePoint;
    EmbVector movePoint;
    EmbVector sceneReleasePoint;
    EmbVector releasePoint;
    EmbVector sceneGripPoint;

    EmbVector cutCopyMousePoint;
    EmbVector pasteDelta;

    int panDistance;
    int panStartX;
    int panStartY;

    bool enableSnap;
    bool enableGrid;
    bool enableOrtho;
    bool enablePolar;
    bool enableQSnap;
    bool enableQTrack;
    bool enable_lwt;
    bool enable_real;

    bool enableRuler;
    uint32_t rulerColor;
    bool rulerMetric;
    uint8_t rulerPixelSize;

    uint32_t backgroundColor;

    bool fileWasLoaded;
    int myIndex;

    EmbString curFile;
    EmbString curLayer;
    uint32_t curColor;
    EmbString curLineType;
    EmbString curLineWeight;
} DocumentData;

typedef struct Button_ {
    EmbRect rect;
    EmbColor color;
    EmbColor highlight_color;
    EmbString text;
    EmbString font;
    EmbColor text_color;
    int state;
} Button;

typedef struct Tab_ {
    int state;
} Tab;

/* Translations */
#define UNFINISHED                     0
#define DRAFT                          1
#define FINISHED                       2

#define ARABIC_SHORTCODE            "ar"
#define AFRIKAANS_SHORTCODE         "af"
#define CHINESE_SHORTCODE           "zh"
#define CZECH_SHORTCODE             "cs"
#define DANISH_SHORTCODE            "da"
#define DUTCH_SHORTCODE             "nl"
#define FINNISH_SHORTCODE           "fi"
#define GERMAN_SHORTCODE            "de"
#define GREEK_SHORTCODE             "el"
#define ITALIAN_SHORTCODE           "it"
#define JAPANESE_SHORTCODE          "ja"
#define KOREAN_SHORTCODE            "ko"
#define PORTUGUESE_SHORTCODE        "pt"
#define ROMANIAN_SHORTCODE          "ro"
#define RUSSIAN_SHORTCODE           "ru"
#define SPANISH_SHORTCODE           "es"
#define SWEDISH_SHORTCODE           "sv"
#define TURKISH_SHORTCODE           "tr"

/* For switch tables we can use this trick to use two character indices.
 * For example: "case TWO_CHAR_INDEX(DUTCH_SHORTCODE):".
 */
#define TWO_CHAR_INDEX(A)             (0x100*A[0] + A[1])

typedef struct Translation_ {
    EmbString source;
    EmbString target;
    int32_t status;
} Translation;

typedef int EmbIntTable[100];

/* Global variables and constants we need to access anywhere in the program
 * with minimal overhead.
 *
 * These are all fixed-size allocations so if they are capable of being
 * replaced and makes the structure of the program in memory more interpretable.
 */
typedef struct State_ {
    EmbString os;

    /* Versions */
    EmbString embroidermodder_version;
    EmbString libembroidery_version;
    EmbString EmbroideryMobile_version;
    EmbString PET_version;

    /* Paths */
    EmbString circle_origin_path;
    EmbString one_path;
    EmbString two_path;
    EmbString three_path;
    EmbString four_path;
    EmbString five_path;
    EmbString six_path;
    EmbString seven_path;
    EmbString eight_path;
    EmbString nine_path;
    EmbString zero_path;
    EmbString minus_path;
    EmbString apostrophe_path;
    EmbString quote_path;

    /* Menus */
    EmbStringTable menu_list;
    EmbIntTable menubar_full_list;
    EmbIntTable menubar_no_docs;

    EmbStringTable file_menu;
    EmbStringTable edit_menu;
    EmbStringTable view_menu;
    EmbStringTable zoom_menu;
    EmbStringTable pan_menu;
    EmbStringTable help_menu;
    EmbStringTable draw_menu;
    EmbStringTable tools_menu;
    EmbStringTable modify_menu;
    EmbStringTable dimension_menu;
    EmbStringTable sandbox_menu;
    EmbStringTable recent_menu;
    EmbStringTable window_menu;

    /* Toolbars */
    EmbStringTable toolbar_list;
    EmbIntTable toolbars_when_docs;

    EmbIntTable top_toolbar;
    EmbIntTable left_toolbar;
    EmbIntTable bottom_toolbar;
    EmbIntTable toolbar_horizontal;

    EmbStringTable file_toolbar;
    EmbStringTable edit_toolbar; 
    EmbStringTable view_toolbar; 
    EmbStringTable zoom_toolbar; 
    EmbStringTable pan_toolbar;
    EmbStringTable icon_toolbar; 
    EmbStringTable help_toolbar; 
    EmbStringTable draw_toolbar; 
    EmbStringTable inquiry_toolbar; 
    EmbStringTable modify_toolbar; 
    EmbStringTable dimension_toolbar; 
    EmbStringTable sandbox_toolbar;
    EmbStringTable layer_toolbar;
    EmbStringTable properties_toolbar;
    EmbStringTable text_toolbar;
    EmbStringTable prompt_toolbar;

    /* Widget Groups */
    EmbStringTable grid_load_from_file_group; 
    EmbStringTable defined_origin_group; 
    EmbStringTable rectangular_grid_group; 
    EmbStringTable circular_grid_group; 
    EmbStringTable center_on_origin_group; 

    /* Settings */
    EmbStringTable settings_tab_labels;
    EmbIntTable preview_to_dialog;
    EmbIntTable accept_to_dialog;
    EmbIntTable render_hints;

    /* Objects */
    EmbStringTable object_names;

    /* Testing */
    EmbStringTable coverage_test;

    /* Misc */
    EmbStringTable tips;
    EmbStringTable extensions;
} State;

/* Scripting functions */
ScriptEnv *create_script_env();
void free_script_env(ScriptEnv *);
ScriptEnv *pack(ScriptEnv *context, const char *fmt, ...);
ScriptValue run_command(ScriptEnv *context, const char *cmd);

ScriptValue script_bool(bool b);
ScriptValue script_int(int i);
ScriptValue script_real(EmbReal r);
ScriptValue script_string(EmbString s);
ScriptValue command_prompt(ScriptEnv *context, EmbString line);

EmbVector find_mouse_snap_point(EmbVectorList *snap_points, EmbVector mouse_point);

EmbVectorList *create_vector_list(void);
void append_vector_to_list(EmbVectorList *, EmbVector);
void remove_vector_from_list(EmbVectorList *, int32_t);
void free_vector_list(EmbVectorList *);

EmbIdList *create_id_list(void);
void append_id_to_list(EmbIdList *list, int32_t);
void remove_id_from_list(EmbIdList *list, int32_t);
bool id_list_contains(EmbIdList *list, int32_t);
void copy_object_list(EmbIdList *dst, EmbIdList *src);
void free_id_list(EmbIdList *);

ScriptEnv *add_string_argument(ScriptEnv *context, EmbString s);
ScriptEnv *add_real_argument(ScriptEnv *context, EmbReal r);
ScriptEnv *add_int_argument(ScriptEnv *context, int i);

ScriptValue *setting_ptr(int key, int mode);
void copy_setting(int key, int dst, int src);

void set_int(int key, int value);
void set_real(int key, EmbReal value);
void set_str(int key, EmbString value);
void set_bool(int key, bool value);

int get_int(int key);
EmbReal get_real(int key);
char *get_str(int key);
bool get_bool(int key);

void add_string_variable(ScriptEnv *context, const EmbString label, EmbString s);
void add_int_variable(ScriptEnv *context, const EmbString label, int i);
void add_real_variable(ScriptEnv *context, const EmbString label, EmbReal i);

const char *script_get_string(ScriptEnv *context, const EmbString label);
int script_get_int(ScriptEnv *context, const EmbString label);
EmbReal script_get_real(ScriptEnv *context, const EmbString label);

int script_set_string(ScriptEnv *context, const EmbString label, EmbString s);
int script_set_int(ScriptEnv *context, const EmbString label, int i);
int script_set_real(ScriptEnv *context, const EmbString label, EmbReal r);

void prompt_output(const char *);
int argument_checks(ScriptEnv *context, int id);
char *translate(const char *msg);

bool pattern_save(EmbPattern *pattern, EmbString fileName);

int parse_floats(const char *line, float result[], int n);
int parse_vector(const char *line, EmbVector *v);
bool valid_rgb(float r, float g, float b);
void report_distance(EmbVector a, EmbVector b);

void add_to_menu(int id, EmbStringTable menu_data);
void add_to_toolbar(int id, EmbStringTable toolbar_data);

int load_data(void);

int load_settings(const char *appDir, const char *configDir);
int save_settings(EmbString appDir, EmbString configDir);

int get_command_id(EmbString );
int get_state_variable(EmbString key);

EmbArc emb_arc_set_radius(EmbArc a, EmbReal radius);

bool int32_underflow(int64_t a, int64_t b);
bool int32_overflow(int64_t a, int64_t b);
int round_to_multiple(bool roundUp, int numToRound, int multiple);

void messagebox(const char* logo, const char *title, const char *text);
void information_box(const char *title, const char *text);
void warning_box(const char *title, const char *text);
void critical_box(const char *title, const char *text);
void question_box(const char *title, const char *text);

const char *get_current_layer();
uint32_t get_current_color();
const char *get_current_line_type();
const char *get_current_line_weight();

void statusbar_toggle(EmbString key, bool on);
void zoom_extents_all_sub_windows(void);
bool loadFile(const char *fileName);

int32_t free_glfw(void);

int glfw_application(int argc, char *argv[]);

/* ------------------------------ Prompt ------------------------------- */

void set_prompt_text_color(uint32_t color);
void set_prompt_background_color(uint32_t color);
void set_prompt_font_family(EmbString family);
void set_prompt_font_style(EmbString style);
void set_prompt_font_size(int size);

void prompt_history_appended(EmbString txt);
void log_prompt_input(EmbString txt);
void prompt_input_previous(void);
void prompt_input_next(void);
void prompt_update_style(void);
void prompt_end_command(void);
void prompt_set_current_text(const char *);
void process_input(char);

/* -------------------------- Main Functions --------------------------- */

void run_command_main(const char *cmd);
void run_command_click(const char *cmd, EmbReal x, EmbReal y);
void run_command_move(const char *cmd, EmbReal x, EmbReal y);
void run_command_context(const char *cmd, const char *str);
void run_command_prompt(const char *cmd);

uint32_t rgb(uint8_t r, uint8_t g, uint8_t b);

void update_all_view_scroll_bars(bool val);
void update_all_view_cross_hair_colors(uint32_t color);
void update_all_view_background_colors(uint32_t color);
void update_all_view_select_box_colors(uint32_t colorL, uint32_t fillL,
    uint32_t colorR, uint32_t fillR, int alpha);
void update_all_view_grid_colors(uint32_t color);
void update_all_view_ruler_colors(uint32_t color);

void update_pick_add_mode(bool val);
void pick_add_mode_toggled(void);

void set_text_font(EmbString str);
void set_text_size(EmbReal num);

void prompt_history_appended(EmbString txt);
void log_prompt_input(EmbString txt);

void open_file(bool recent, EmbString recentFile);
void open_filesSelected(EmbStringTable);

void settings_dialog(const char *showTab);

bool valid_file_format(EmbString fileName);

void on_close_window(void);

void set_undo_clean_icon(bool opened);

void mdiarea_set_bg(uint32_t color);
void accept_interface_color(int32_t key, uint32_t color);
void preview_interface_color(int32_t key, uint32_t color);
void dialog_interface_color(int32_t key, uint32_t color);

void current_layer_changed(EmbString layer);
void current_color_changed(uint32_t color);
void current_linetype_changed(EmbString type);
void current_lineweight_changed(EmbString weight);

void prompt_input_previous(void);
void prompt_input_next(void);

void print_command(void);
void undo_command(void);
void redo_command(void);

void layer_selector_changed(int index);
void linetype_selector_changed(int index);
void lineweight_selector_changed(int index);
void text_size_selector_index_changed(int index);

void make_layer_active(void);
void layer_manager(void);
void layer_previous(void);

void delete_pressed(void);
void escape_pressed(void);

bool is_shift_pressed(void);
void set_shift_pressed(void);
void set_shift_released(void);

void icon_resize(int iconSize);

void read_settings(void);
void write_settings(void);

void create_all_actions(void);

void load_formats(void);

void settings_prompt(void);

void end_command(void);
void debug_message(const char *msg);
void wait_cursor(void);
void arrow_cursor(void);
void restore_cursor(void);

void cut(void);

void about_dialog(void);

/* --------------------------------- Editors -------------------------------- */

void combo_box_scroll_bar_widget_changed(int);
void spin_box_display_select_box_alpha_changed(int);
void combo_box_prompt_font_family_changed(EmbString);
void combo_box_prompt_font_style_changed(EmbString);
void spin_box_prompt_font_size_changed(int);
void spin_box_recent_max_files_changed(int value);
void spin_box_trim_dst_num_jumps_changed(int value);
void spin_box_ruler_pixel_size_changed(EmbReal);
void slider_qsnap_locator_size_changed(int);
void slider_qsnap_aperture_size_changed(int);
void check_box_lwt_real_render_changed(int);
void slider_selection_grip_size_changed(int);
void slider_selection_pick_box_size_changed(int);

/* -------------------------------- Commands -------------------------------- */

void about_dialog(void);
void todo(const char *txt);
void fixme(const char *msg);
void stub_testing(void);
void run_testing(void);
void exit_program(void);
void check_for_updates(void);
void new_file(void);
void open_recent_file(void);
void save_file(void);
int save_as_file(void);
void update_interface(void);
void window_menu_about_to_show();
void hide_unimplemented(void);
void start_blinking(void);
void stop_blinking(void);
void repeat_action(void);
void move_action(void);

void nanosleep_(int);

void native_blink_prompt();

void check_box_tip_of_the_day_changed(int checked);
void button_tip_of_the_day_clicked(int button);

ScriptValue add_arc_command(ScriptEnv *context);
ScriptValue add_circle_command(ScriptEnv *context);
ScriptValue add_dimleader_command(ScriptEnv *context);
ScriptValue add_ellipse_command(ScriptEnv *context);
ScriptValue add_horizontal_dimension_command(ScriptEnv *context);
ScriptValue add_image_command(ScriptEnv *context);
ScriptValue add_infinite_line_command(ScriptEnv *context);
ScriptValue add_line_command(ScriptEnv *context);
ScriptValue add_ray_command(ScriptEnv *context);
ScriptValue add_triangle_command(ScriptEnv *context);
ScriptValue add_rectangle_command(ScriptEnv *context);
ScriptValue add_rounded_rectangle_command(ScriptEnv *context);
ScriptValue add_slot_command(ScriptEnv *context);
ScriptValue add_point_command(ScriptEnv *context);
ScriptValue add_regular_polygon_command(ScriptEnv *context);
ScriptValue add_vertical_dimension_command(ScriptEnv *context);
ScriptValue add_textmulti_command(ScriptEnv *context);
ScriptValue add_textsingle_command(ScriptEnv *context);
ScriptValue get_command(ScriptEnv *context);
ScriptValue move_command(ScriptEnv *context);
ScriptValue previewon_command(ScriptEnv *context);
ScriptValue sandbox_command(ScriptEnv *context);
ScriptValue set_command(ScriptEnv *context);

ScriptValue native_redo();

void set_mouse_coord(EmbReal x, EmbReal y);

void native_print_area(EmbReal x, EmbReal y, EmbReal w, EmbReal h);

void set_background_color(uint8_t r, uint8_t g, uint8_t b);
void set_cross_hair_color(uint8_t r, uint8_t g, uint8_t b);
void set_grid_color(uint8_t r, uint8_t g, uint8_t b);

void native_clear_rubber();
bool native_allow_rubber();
void native_spare_rubber(int64_t id);

void native_delete_selected();
void native_cut_selected(EmbReal x, EmbReal y);
void native_copy_selected(EmbReal x, EmbReal y);
void native_paste_selected(EmbReal x, EmbReal y);
void native_move_selected(EmbReal dx, EmbReal dy);
void native_scale_selected(EmbReal x, EmbReal y, EmbReal factor);
void native_rotate_selected(EmbReal x, EmbReal y, EmbReal rot);
void native_mirror_selected(EmbReal x1, EmbReal y1, EmbReal x2, EmbReal y2);

void set_cursor_shape(EmbString shape);
double native_calculate_distance(EmbReal x1, EmbReal y1, EmbReal x2, EmbReal y2);
double native_perpendicular_distance(EmbReal px, EmbReal py, EmbReal x1, EmbReal y1, EmbReal x2, EmbReal y2);

double native_q_snap_x();
double native_q_snap_y();

void enable_lwt();
void disable_lwt();
void enable_real();
void disable_real();

void create_details_dialog(void);

void set_prompt_prefix(EmbString txt);
/* TODO: void set_RubberFilter(int64_t id); */
/* TODO: This is so more than 1 rubber object can exist at one time without updating all rubber objects at once. */
void set_rubber_mode(int mode);
void set_rubber_point(const EmbString key, EmbReal x, EmbReal y);
void set_rubber_text(const EmbString key, EmbString txt);

void toggle_grid(void);
void toggle_ruler(void);
void toggle_lwt(void);

/* Help Menu */
void tip_of_the_day();
void button_tip_of_the_day_clicked(int);
void check_box_tip_of_the_day_changed(int);
void help();
void changelog();
void whats_this_context_help();

int make_application(int argc, char* argv[]);

void set_object_rubber_mode(ObjectCore *core, int mode);

EmbVector unpack_vector(ScriptEnv *context, int offset);

void undoable_add(int32_t doc, uint32_t obj, EmbString label);
void undoable_delete(int32_t doc, uint32_t obj, EmbString label);
void undoable_scale(int32_t doc, uint32_t obj, EmbVector center, EmbReal factor, EmbString label);
void undoable_move(int32_t doc, uint32_t obj, EmbVector delta, EmbString msg);
void undoable_rotate(int32_t doc, uint32_t obj, EmbVector v, EmbString msg);
void undoable_mirror(int32_t doc, uint32_t obj, EmbVector start, EmbVector end,
    EmbString msg);

void update_all_background_color(uint32_t color);

void create_all_menus(void);
void create_all_toolbars(void);

void clear_all_fields(void);
void line_edit_clear(const char *key);
void combo_box_clear(const char *key);
void clear_font_combobox(void);

int get_id(EmbStringTable, EmbString);

const char *platform_string(void);

void button_tip_of_the_day_clicked(int button);
void free_objects(EmbIdList *);

/* ------------------------------- Document -------------------------------- */

void doc_init(int32_t doc);
void free_doc(int32_t doc);

bool doc_allow_zoom_in(int32_t doc);
bool doc_allow_zoom_out(int32_t doc);
void doc_zoom_in(int32_t doc);
void doc_zoom_out(int32_t doc);
void doc_zoom_window(int32_t doc);
void doc_zoom_selected(int32_t doc);
void doc_zoom_extents(int32_t doc);
void doc_zoom_to_point(int32_t doc, EmbVector mousePoint, int zoomDir);

void doc_pan_real_time(int32_t doc);
void doc_pan_point(int32_t doc);
void doc_pan_left(int32_t doc);
void doc_pan_right(int32_t doc);
void doc_pan_up(int32_t doc);
void doc_pan_down(int32_t doc);
void doc_select_all(int32_t doc);
void doc_selection_changed(int32_t doc);
void doc_clear_selection(int32_t doc);
void doc_delete_selected(int32_t doc);
void doc_move_selected(int32_t doc, EmbVector delta);
void doc_cut(int32_t doc);
void doc_copy(int32_t doc);
void doc_paste(int32_t doc);
void doc_repeat_action(int32_t doc);
void doc_move_action(int32_t doc);
void doc_scale_action(int32_t doc);
void doc_scale_selected(int32_t doc, EmbReal x, EmbReal y, EmbReal factor);
void doc_rotate_action(int32_t doc);
void doc_rotate_selected(int32_t doc, EmbReal x, EmbReal y, EmbReal rot);
void doc_mirror_selected(int32_t doc, EmbReal x1, EmbReal y1, EmbReal x2, EmbReal y2);
int doc_num_selected(int32_t doc);

void doc_add_to_rubber_room(int32_t doc, int32_t item);

void doc_stop_gripping(int32_t, bool);
void hide_selectbox(int32_t);

void remove_paste_object_item_group(int32_t doc);
void doc_empty_grid(int32_t doc);
void doc_set_grid_color(int32_t doc, uint32_t color);
void doc_set_ruler_color(int32_t doc, uint32_t color);

void set_prompt_text_color(uint32_t color);
void set_prompt_background_color(uint32_t color);

void doc_preview_on(int32_t doc, int clone, int mode, EmbReal x, EmbReal y, EmbReal data);
void doc_preview_off(int32_t doc);

void doc_delete_pressed(int32_t doc);
void doc_escape_pressed(int32_t doc);

void doc_corner_button_clicked(int32_t doc);
void doc_create_grid(int32_t doc, EmbString gridType);
void doc_copy_selected(int32_t doc);

void doc_show_scroll_bars(int32_t doc, bool val);
void doc_set_corner_button(int32_t doc);
void doc_set_cross_hair_color(int32_t doc, uint32_t color);
void doc_set_cross_hair_size(int32_t doc, uint8_t percent);
void doc_set_background_color(int32_t doc, uint32_t color);
void doc_set_select_box_colors(int32_t doc, uint32_t colorL, uint32_t fillL,
    uint32_t colorR, uint32_t fillR, int alpha);
void doc_toggle_snap(int32_t doc, bool on);
void doc_toggle_grid(int32_t doc, bool on);
void doc_toggle_ruler(int32_t doc, bool on);
void doc_toggle_ortho(int32_t doc, bool on);
void doc_toggle_polar(int32_t doc, bool on);
void doc_toggle_qsnap(int32_t doc, bool on);
void doc_toggle_qtrack(int32_t doc, bool on);
void doc_toggle_lwt(int32_t doc, bool on);
void doc_toggle_real(int32_t doc, bool on);

void doc_enable_move_rapid_fire(int32_t doc);
void doc_disable_move_rapid_fire(int32_t doc);

bool doc_allow_rubber(int32_t doc);
void doc_vulcanize_rubber_room(int32_t doc);
void doc_clear_rubber_room(int32_t doc);
void doc_spare_rubber(int32_t doc, int64_t id);
void doc_set_rubber_mode(int32_t doc, int mode);
void doc_set_rubber_point(int32_t doc, EmbString key, EmbVector point);
void doc_set_rubber_text(int32_t doc, EmbString key, EmbString txt);

void doc_recalculate_limits(int32_t doc);
void doc_center_at(int32_t doc, EmbVector centerPoint);
EmbVector doc_center(int32_t doc);

void doc_create_grid_rect(int32_t doc);
void doc_create_grid_polar(int32_t doc);
void doc_create_grid_iso(int32_t doc);
void doc_create_origin(int32_t doc);

void doc_load_ruler_settings(int32_t doc);

void doc_update_mouse_coords(int32_t doc, int x, int y);

void doc_pan_start(int32_t doc, EmbVector point);

void doc_align_scene_point_with_view_point(int32_t doc, EmbVector scenePoint, EmbVector viewPoint);

void doc_undoable_add_obj(int32_t doc, uint32_t id, int rubberMode);

double doc_width(int32_t doc);
double doc_height(int32_t doc);

void doc_corner_button_clicked(int32_t doc);
void doc_add_item(int32_t doc, uint32_t id);
void doc_update(int32_t doc_index);

void doc_center_on(int32_t doc, EmbVector v);

DocumentData *doc_data(int32_t doc);
EmbVector doc_center(int32_t doc_id);
void doc_scale(int32_t doc_id, EmbReal s);
void doc_begin_macro(int32_t doc, EmbString s);
void doc_end_macro(int32_t doc);

void update_color_linetype_lineweight();

void rotate_action(void);
void scale_action(void);

EmbVector doc_map_from_scene(int32_t doc, EmbVector v);
EmbVector doc_map_to_scene(int32_t doc, EmbVector v);

void doc_add_object(int32_t doc, uint32_t obj);
void doc_delete_object(int32_t doc, uint32_t obj);
void doc_vulcanize_object(int32_t doc, uint32_t obj);

int32_t active_document(void);

void whats_this_mode(void);

void window_close_all(void);
void window_cascade(void);
void window_tile(void);
void window_next(void);
void window_previous(void);

void enable_rapid_fire(void);
void disable_rapid_fire(void);

void append_history(const char *txt);

/* -------------------------------- Interface ------------------------------ */

void update_editors(int32_t id);
void edit_field(int32_t id, const char *objName, const char *text);

void show_groups(int objType);
void show_one_type(int index);
void hide_all_groups(void);
void clear_all_fields(void);
void hide_group_box(int32_t key);
void show_group_box(int32_t key);

void update_lineedit_num(const char *key, EmbReal num, bool useAnglePrecision);
void update_line_edit_str_if_varies(const char *key, const char *str);
void update_lineedit_str(const char *key, const char *str, EmbStringTable strList);
void update_lineedit_bool(const char *key, bool val, bool yesOrNoText);
void update_font_combo_box_str_if_varies(const char *str);

/* -------------------------------- EmbString ------------------------------ */

void emb_string(EmbString s, const char *str);
bool string_equal(EmbString a, const char *b);
int string_compare(EmbString a, const char *b);
void string_copy(EmbString dst, const char *src);
int string_array_length(EmbString s[]);
int string_list_contains(EmbStringTable list, EmbString entry);

/* ----------------------------- Object Core ------------------------------- */

void free_object(ObjectCore *core);

ObjectCore *obj_get_core(uint32_t id);

uint32_t create_object(int type_, uint32_t rgb);
uint32_t copy_object(uint32_t);

uint32_t create_arc(EmbArc arc, uint32_t rgb);
uint32_t create_circle(EmbCircle circle, uint32_t rgb);
uint32_t create_dimleader(EmbLine leader, uint32_t rgb);
uint32_t create_ellipse(EmbEllipse ellipse, uint32_t rgb);
uint32_t create_line(EmbLine line, uint32_t rgb);
uint32_t create_path(EmbPath *p, uint32_t rgb);
uint32_t create_point(EmbPoint p, uint32_t rgb);
uint32_t create_polygon(EmbPath *p, uint32_t rgb);
uint32_t create_polyline(EmbPath *path, uint32_t rgb);
uint32_t create_rect(EmbRect rect, uint32_t rgb);
uint32_t create_text_single(EmbString str, EmbVector v, uint32_t rgb);

void obj_set_rubber_point(uint32_t id, EmbString key, EmbVector value);
void obj_set_rubber_text(uint32_t id, EmbString key, EmbString value);

EmbVector obj_pos(ObjectCore *obj);
double obj_x(ObjectCore *obj);
double obj_y(ObjectCore *obj);
EmbVector obj_center(ObjectCore *obj);
double obj_center_x(ObjectCore *obj);
double obj_center_y(ObjectCore *obj);
double obj_radius(ObjectCore *obj);
double obj_diameter(ObjectCore *obj);
double obj_circumference(ObjectCore *obj);
EmbVector obj_delta(ObjectCore *obj);

EmbVector obj_end_point_1(ObjectCore *obj);
EmbVector obj_end_point_2(ObjectCore *obj);
EmbVector obj_start_point(ObjectCore *obj);
EmbVector obj_mid_point(ObjectCore *obj);
EmbVector obj_end_point(ObjectCore *obj);

double obj_length(ObjectCore *obj);

void obj_set_pos(ObjectCore *obj, EmbVector point);
void obj_set_x(ObjectCore *obj, EmbReal x);
void obj_set_y(ObjectCore *obj, EmbReal y);

void obj_set_center(ObjectCore *obj, EmbVector point);
void obj_set_center_x(ObjectCore *obj, EmbReal centerX);
void obj_set_center_y(ObjectCore *obj, EmbReal centerY);

void obj_set_end_point_1(ObjectCore *obj, EmbVector endPt1);
void obj_set_end_point_2(ObjectCore *obj, EmbVector endPt2);

void obj_set_x1(ObjectCore *obj, EmbReal x);
void obj_set_y1(ObjectCore *obj, EmbReal y);
void obj_set_x2(ObjectCore *obj, EmbReal x);
void obj_set_y2(ObjectCore *obj, EmbReal y);

void obj_set_rect(uint32_t obj, EmbReal x, EmbReal y, EmbReal w, EmbReal h);
void obj_set_rotation(uint32_t id, EmbReal rotation);
void obj_set_rubber_mode(uint32_t id, int mode);
void obj_set_rubber_point(uint32_t id, EmbString key, EmbVector v);
void obj_set_rubber_text(uint32_t id, EmbString key, EmbString value);

void obj_calculate_data(uint32_t obj);

EmbVector obj_top_left(ObjectCore *obj);
EmbVector obj_top_right(ObjectCore *obj);
EmbVector obj_bottom_left(ObjectCore *obj);
EmbVector obj_bottom_right(ObjectCore *obj);

EmbRect obj_rect(ObjectCore *obj);

void obj_set_text(ObjectCore *obj, const char *str);
void obj_set_text_font(ObjectCore *obj, const char *font);
void obj_set_text_justify(ObjectCore *obj, const char *justify);
void obj_set_text_size(ObjectCore *obj, EmbReal size);
void obj_set_text_style(ObjectCore *obj, bool bold, bool italic, bool under, bool strike, bool over);
void obj_set_text_bold(ObjectCore *obj, bool val);
void obj_set_text_italic(ObjectCore *obj, bool val);
void obj_set_text_underline(ObjectCore *obj, bool val);
void obj_set_text_strikeout(ObjectCore *obj, bool val);
void obj_set_text_overline(ObjectCore *obj, bool val);
void obj_set_text_backward(ObjectCore *obj, bool val);
void obj_set_text_upside_down(ObjectCore *obj, bool val);

/* ---------------------------- Global Variables --------------------------- */

extern Command command_data[MAX_COMMANDS];
extern StringMap aliases[MAX_ALIASES];
extern Setting setting[N_SETTINGS];
extern SettingsData settings_data[N_SETTINGS];
extern GroupBoxData group_box_list[];

extern bool document_memory[MAX_OPEN_FILES];

extern char formatFilterOpen[MAX_LONG_STRING];
extern char formatFilterSave[MAX_LONG_STRING];
extern EmbString open_filesPath;
extern EmbString prompt_color_;
extern EmbString prompt_selection_bg_color_;
extern EmbString prompt_bg_color_;
extern EmbString prompt_selection_color_;

extern EmbString lastCmd;

extern ScriptEnv *global;

extern const ScriptValue script_null;
extern const ScriptValue script_true;
extern const ScriptValue script_false;

extern bool blinkState;

extern int testing_mode;
extern EmbStringTable button_list;

extern EmbString end_symbol;

extern EmbString _appName_;
extern EmbString _appVer_;
extern EmbString settings_file;

extern EmbStringTable layer_list; 
extern EmbStringTable color_list; 
extern EmbStringTable line_type_list; 
extern EmbStringTable line_weight_list; 
extern EmbStringTable text_size_list; 

extern EmbStringTable editor_list; 
extern EmbStringTable combobox_list; 

extern char **xpm_icons[];
extern EmbStringTable xpm_icon_labels;

extern EmbString recent_files[MAX_FILES];

extern int numOfDocs;
extern int docIndex;
extern bool shiftKeyPressedState;
extern bool cmdActive;
extern bool rapidFireEnabled;
extern bool isBlinking;
extern bool key_state[N_KEY_SEQUENCES];

extern State state;

#ifdef __cplusplus
}
#endif

#endif

