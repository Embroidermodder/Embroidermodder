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

#include "embroidery.h"

#include "constants.h"

#define REAL(arg) context->argument[arg].r
#define INT(arg)  context->argument[arg].i
#define STR(arg)  context->argument[arg].s
#define BOOL(arg) context->argument[arg].b

#define DIALOG_INT_SLOT(A) \
    SLOT([=](int value) { setting[A].dialog.i = value; } )
#define DIALOG_REAL_SLOT(A) \
    SLOT([=](EmbReal value) { setting[A].dialog.r = value; } )
#define DIALOG_STRING_SLOT(A) \
    SLOT([=](QString value) { strncpy(setting[A].dialog.s, qPrintable(value), MAX_STRING_LENGTH); } )

#define set_int(key, value)                setting[key].setting.i = value
#define set_real(key, value)               setting[key].setting.r = value
#define set_str(key, value) strncpy(setting[key].setting.s, value, MAX_STRING_LENGTH)
#define set_bool(key, value)               setting[key].setting.b = value

#define get_int(key)       setting[key].setting.i
#define get_str(key)       setting[key].setting.s
#define get_real(key)      setting[key].setting.r
#define get_bool(key)      setting[key].setting.b

/* For switch tables we can use this trick to use two character indices.
 * For example: "case TWO_CHAR_INDEX(DUTCH_SHORTCODE):".
 */
#define TWO_CHAR_INDEX(A)             (0x100*A[0] + A[1])

typedef struct Command_ {
    char *command;
    char *arguments;
    char *icon;
    char *tooltip;
    char *statustip;
    char *alias;
    char *shortcut;
    int32_t flags;
} Command;

#define MAX_LAYERS 20

typedef struct LayerData_ {
    char name[200];
    bool visible;
    bool frozen;
    EmbReal zValue;
    int32_t color;
    char *lineType;
    char *lineWeight;
    bool print;
} LayerData;

typedef struct Setting_ {
    char key[MAX_STRING_LENGTH];
    ScriptValue setting;
    ScriptValue dialog;
    ScriptValue accept;
    ScriptValue preview;
} Setting;

typedef struct GroupBoxData_ {
    int32_t id;
    char *key;
    char *label;
    int rows;
    char *data[100];
    int object;
} GroupBoxData;

typedef struct ObjectCore_ {
    int32_t doc;

    EmbGeometry *geometry;

    LabelledVector *rubber_points;
    int n_rubber_points;
    StringMap *rubber_texts;
    int n_rubber_texts;

    EmbVector position;

    EmbReal scale;
    EmbReal rotation;

    uint32_t rgb;

    char OBJ_NAME[MAX_STRING_LENGTH];

    char text[MAX_STRING_LENGTH];
    char textFont[MAX_STRING_LENGTH];
    char textJustify[MAX_STRING_LENGTH];
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
    char label[MAX_STRING_LENGTH];
    int type;
    int32_t doc;
    EmbVector delta;
    EmbVector pivot;
    EmbReal angle;
    EmbReal factor;
    char navType[MAX_STRING_LENGTH];
    EmbVector fromCenter;
    EmbVector toCenter;
    bool done;
    EmbVector before;
    EmbVector after;
    EmbLine mirrorLine;
    uint32_t obj;
} UndoData;

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

    bool properties[N_VIEW_PROPS];

    uint32_t rulerColor;
    bool rulerMetric;
    uint8_t rulerPixelSize;

    uint32_t backgroundColor;

    bool fileWasLoaded;
    int myIndex;

    char curFile[MAX_STRING_LENGTH];
    char curLayer[MAX_STRING_LENGTH];
    uint32_t curColor;
    char curLineType[MAX_STRING_LENGTH];
    char curLineWeight[MAX_STRING_LENGTH];
} DocumentData;

/* For dialogs and tabs */
typedef struct WidgetDesc_ {
    int id;
    char *label;
    char *key;
    char *icon;
    char *value;
    char *editor_data;
    int type;
    int n_leaves;
} WidgetDesc;

typedef struct Button_ {
    EmbRect rect;
    EmbColor color;
    EmbColor highlight_color;
    char *text;
    char *font;
    EmbColor text_color;
    int state;
} Button;

typedef struct Tab_ {
    int state;
} Tab;

typedef struct Translation_ {
    char *ar;
    char *en;
    char *es;
    char *fr;
    char *ru;
    char *zh;
    int32_t status;
} Translation;

typedef struct WidgetData_ {
    char *key;
    char *label;
    int id;
    int type;
} WidgetData;

typedef struct Design_ {
    char *command;
    float lower;
    float upper;
    int num_points;
    int min_points;
    int max_points;
    float xscale;
    float yscale;
    char *parameter;
    char *x;
    char *y;
} Design;

typedef struct ViewData_ {
    int32_t doc;
    double little;
    double medium;
    EmbLine *lines;
    int n_lines;
    EmbGeometry *geometry;
    int n_geometry;
    int width;
    int height;
    int unit;
    bool feet;
    double fraction;
    int xStart;
    int yStart;
} ViewData;

/* -------------------------------- Scripting ---------------------------- */

ScriptEnv *create_script_env(void);
void free_script_env(ScriptEnv *);
ScriptEnv *pack(ScriptEnv *context, const char *fmt, ...);
ScriptValue run_command(ScriptEnv *context, const char *cmd);

ScriptValue script_bool(bool b);
ScriptValue script_int(int i);
ScriptValue script_real(EmbReal r);
ScriptValue script_string(char *s);
ScriptValue command_prompt(ScriptEnv *context, const char *line);

EmbVector find_mouse_snap_point(EmbVectorList *snap_points, EmbVector mouse_point);

ViewData *create_view_data(int32_t doc, int width, int height, int unit);
void free_view_data(ViewData *vdata);
int create_ruler_lines(ViewData *data);

bool string_equal(char *a, const char *b);
void string_copy(char *dst, const char *src);
int find_in_map(StringMap *hash, int length, const char *key);
int table_length(char *table[]);

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

ScriptValue *setting_ptr(int key, int mode);
void copy_setting(int key, int dst, int src);

char *load_file(char *fname);

void prompt_output(const char *);
int argument_checks(ScriptEnv *context, int id);
char *translate(const char *msg);

bool pattern_save(EmbPattern *pattern, char *fileName);

int parse_floats(const char *line, float result[], int n);
int parse_vector(const char *line, EmbVector *v);
bool valid_rgb(float r, float g, float b);
void report_distance(EmbVector a, EmbVector b);

void add_to_menu(int id, char *menu_data[]);
void add_to_toolbar(int id, char *toolbar_data[]);

int load_data(void);

int load_settings(const char *appDir, const char *configDir);
int save_settings(char *appDir, char *configDir);

int get_command_id(char *);

EmbArc emb_arc_set_radius(EmbArc a, EmbReal radius);

bool int32_underflow(int64_t a, int64_t b);
bool int32_overflow(int64_t a, int64_t b);
int round_to_multiple(bool roundUp, int numToRound, int multiple);

void messagebox(const char* logo, const char *title, const char *text);

const char *get_current_layer(void);
uint32_t get_current_color(void);
const char *get_current_line_type(void);
const char *get_current_line_weight(void);

void statusbar_toggle(const char *key, bool on);
void zoom_extents_all_sub_windows(void);
bool loadFile(const char *fileName);

int32_t free_glfw(void);

int glfw_application(int argc, char *argv[]);

int find_int_map(IntMap *, int);

char *file_extension(char *fileName);

/* ------------------------------ Widgets ------------------------------- */

void set_visibility(const char *key, bool visibility);
void set_enabled(const char *key, bool visibility);
void set_visibility_group(char *key[], bool visibility);
void set_enabled_group(char *key[], bool visibility);

/* ------------------------------ Prompt ------------------------------- */

void set_prompt_text_color(uint32_t color);
void set_prompt_background_color(uint32_t color);
void set_prompt_font_family(char *family);
void set_prompt_font_style(char *style);
void set_prompt_font_size(int size);

void prompt_history_appended(char *txt);
void log_prompt_input(char *txt);
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

void set_text_font(char *str);
void set_text_size(EmbReal num);

void prompt_history_appended(char *txt);
void log_prompt_input(char *txt);

void open_file(bool recent, char *recentFile);
void open_selected_files(char *table[]);

void settings_dialog(const char *showTab);

bool valid_file_format(char *fileName);

void on_close_window(void);

void set_undo_clean_icon(bool opened);

void mdiarea_set_bg(uint32_t color);
void accept_interface_color(int32_t key, uint32_t color);
void preview_interface_color(int32_t key, uint32_t color);
void dialog_interface_color(int32_t key, uint32_t color);

void current_layer_changed(char *layer);
void current_color_changed(uint32_t color);
void current_linetype_changed(char *type);
void current_lineweight_changed(char *weight);

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

void icon_resize(int iconSize);

void read_settings(void);
void write_settings(void);

void create_all_actions(void);

void load_formats(void);

void settings_prompt(void);

void end_command(void);
void debug_message(const char *msg, ...);
void wait_cursor(void);
void arrow_cursor(void);
void restore_cursor(void);

void cut(void);

void about_dialog(void);

/* --------------------------------- Editors -------------------------------- */

void check_box_lwt_real_render_changed(int);
void combo_box_prompt_font_family_changed(char *);
void combo_box_prompt_font_style_changed(char *);
void spin_box_display_select_box_alpha_changed(int);
void spin_box_prompt_font_size_changed(int);

void checkBoxGridCenterOnOriginStateChanged(int);
void comboBoxGridTypeCurrentIndexChanged(const char *);
void checkBoxGridColorMatchCrossHairStateChanged(int);
void checkBoxGridLoadFromFileStateChanged(int);
void checkBoxLwtShowLwtStateChanged(int);

void accept_settings(void);

void useBackgroundLogo(bool use);
void useBackgroundTexture(bool use);
void useBackgroundColor(bool use);

void setBackgroundLogo(char *fileName);
void setBackgroundTexture(char *fileName);
void setBackgroundColor(uint32_t color);

void update_view(void);
void preview_update(void);

/* -------------------------------- Commands -------------------------------- */

void about_dialog(void);
void stub_testing(void);
void run_testing(void);
void exit_program(void);
void check_for_updates(void);
void new_file(void);
void open_recent_file(void);
void save_file(void);
int save_as_file(void);
void update_interface(void);
void window_menu_about_to_show(void);
void start_blinking(void);
void stop_blinking(void);
void repeat_action(void);
void move_action(void);

void nanosleep_(int);

void native_blink_prompt(void);

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

ScriptValue native_redo(void);

void set_mouse_coord(EmbReal x, EmbReal y);

void native_print_area(EmbReal x, EmbReal y, EmbReal w, EmbReal h);

void set_background_color(uint8_t r, uint8_t g, uint8_t b);
void set_cross_hair_color(uint8_t r, uint8_t g, uint8_t b);
void set_grid_color(uint8_t r, uint8_t g, uint8_t b);

void native_clear_rubber(void);
bool native_allow_rubber(void);
void native_spare_rubber(int64_t id);

void native_delete_selected(void);
void native_cut_selected(EmbReal x, EmbReal y);
void native_copy_selected(EmbReal x, EmbReal y);
void native_paste_selected(EmbReal x, EmbReal y);
void native_move_selected(EmbReal dx, EmbReal dy);
void native_scale_selected(EmbReal x, EmbReal y, EmbReal factor);
void native_rotate_selected(EmbReal x, EmbReal y, EmbReal rot);
void native_mirror_selected(EmbReal x1, EmbReal y1, EmbReal x2, EmbReal y2);

void set_cursor_shape(char *shape);
double native_calculate_distance(EmbReal x1, EmbReal y1, EmbReal x2, EmbReal y2);
double native_perpendicular_distance(EmbReal px, EmbReal py, EmbReal x1, EmbReal y1, EmbReal x2, EmbReal y2);

double native_q_snap_x(void);
double native_q_snap_y(void);

void create_details_dialog(void);

void set_prompt_prefix(char *txt);
/* TODO: void set_RubberFilter(int64_t id); */
/* TODO: This is so more than 1 rubber object can exist at one time without updating all rubber objects at once. */
void set_rubber_mode(int mode);
void set_rubber_point(const char *key, EmbReal x, EmbReal y);
void set_rubber_text(const char *key, char *txt);

void toggle(int mode);

/* Help Menu */
void tip_of_the_day(void);
void button_tip_of_the_day_clicked(int);
void check_box_tip_of_the_day_changed(int);
void help(void);
void changelog(void);
void whats_this_context_help(void);

int make_application(int argc, char* argv[]);

void set_object_rubber_mode(ObjectCore *core, int mode);

EmbVector unpack_vector(ScriptEnv *context, int offset);

void undoable_add(int32_t doc, uint32_t obj, char *label);
void undoable_delete(int32_t doc, uint32_t obj, char *label);
void undoable_scale(int32_t doc, uint32_t obj, EmbVector center, EmbReal factor, char *label);
void undoable_move(int32_t doc, uint32_t obj, EmbVector delta, char *msg);
void undoable_rotate(int32_t doc, uint32_t obj, EmbVector v, char *msg);
void undoable_mirror(int32_t doc, uint32_t obj, EmbVector start, EmbVector end,
    char *msg);

void update_all_background_color(uint32_t color);

void create_all_menus(void);
void create_all_toolbars(void);

void clear_all_fields(void);
void line_edit_clear(const char *key);
void combo_box_clear(const char *key);
void clear_font_combobox(void);

int get_id(char *table[], char *);

const char *platform_string(void);

void button_tip_of_the_day_clicked(int button);
void free_objects(EmbIdList *);

/* ------------------------------- Document -------------------------------- */

void doc_init(int32_t doc);
void free_doc(int32_t doc);

bool doc_allow_zoom_in(int32_t doc);
bool doc_allow_zoom_out(int32_t doc);
void doc_zoom_window(int32_t doc);
void doc_zoom_selected(int32_t doc);
void doc_zoom_to_point(int32_t doc, EmbVector mousePoint, int zoomDir);

void doc_nav(char *label, int32_t doc);
void doc_toggle(int32_t doc, const char *key, bool on);

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
void doc_rotate_selected(int32_t doc, EmbVector position, EmbReal rot);
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
void doc_create_grid(int32_t doc, char *gridType);
void doc_copy_selected(int32_t doc);

void doc_show_scroll_bars(int32_t doc, bool val);
void doc_set_prop(int32_t doc, int key, bool value);
void doc_set_corner_button(int32_t doc);
void doc_set_cross_hair_color(int32_t doc, uint32_t color);
void doc_set_cross_hair_size(int32_t doc, uint8_t percent);
void doc_set_background_color(int32_t doc, uint32_t color);
void doc_set_select_box_colors(int32_t doc, uint32_t colorL, uint32_t fillL,
    uint32_t colorR, uint32_t fillR, int alpha);

bool doc_allow_rubber(int32_t doc);
void doc_vulcanize_rubber_room(int32_t doc);
void doc_clear_rubber_room(int32_t doc);
void doc_spare_rubber(int32_t doc, int64_t id);
void doc_set_rubber_mode(int32_t doc, int mode);
void doc_set_rubber_point(int32_t doc, char *key, EmbVector point);
void doc_set_rubber_text(int32_t doc, char *key, char *txt);

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
void doc_begin_macro(int32_t doc, char *s);
void doc_end_macro(int32_t doc);

void update_color_linetype_lineweight(void);

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

/* -------------------------------- Interface ------------------------------ */

int init_glfw(void);

void create_properties_group_box(int32_t);

void add_command(char *alias, char *cmd);

void preview_update(void);
void update_editors(int32_t id);
void edit_field(int32_t id, const char *objName, const char *text);

void show_widget(const char *key, int type);
void hide_widget(const char *key, int type);
void widget_clear(const char *key, int type);
int find_widget_map(const char *key);

void show_groups(int objType);
void show_one_type(int index);
void hide_all_groups(void);
void clear_all_fields(void);

const char *combobox_text(const char *key);
int32_t combobox_find_text(const char *key, const char *text);

void update_lineedit_num(const char *key, EmbReal num, bool useAnglePrecision);
void update_line_edit_str_if_varies(const char *key, const char *str);
void update_lineedit_str(const char *key, const char *str, char *strList[]);
void update_lineedit_bool(const char *key, bool val, bool yesOrNoText);
void update_font_combo_box_str_if_varies(const char *str);

void setHistory(const char *txt);
void obj_set_rect(uint32_t obj, EmbReal x, EmbReal y, EmbReal w, EmbReal h);

void combobox_selection_index_changed(const char *key, int32_t setting_key,
    int32_t index, uint32_t defaultColor);

void combo_ruler_metric_index_changed(int);
void combo_qsnap_locator_color_changed(int);
void combo_cool_grip_color_changed(int);
void combobox_hot_grip_color_index_changed(int);

void choose_mdi_bg_logo(void);
void choose_mdi_bg_texture(void);

void prompt_history_appended(char *txt);
void log_prompt_input(char *txt);
void prompt_input_previous(void);
void prompt_input_next(void);

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
uint32_t create_text_single(char *str, EmbVector v, uint32_t rgb);

EmbVector obj_rubber_point(int32_t id, const char *key);
const char *obj_rubber_text(int32_t id, const char *key);
EmbVector obj_map_rubber(int32_t obj, const char *key);

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
void obj_set_rubber_point(uint32_t id, char *key, EmbVector value);
void obj_set_rubber_text(uint32_t id, char *key, char *value);
void obj_vulcanize(int32_t obj);

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

void obj_move_by(int32_t id, EmbVector delta);
int32_t obj_find_index(int32_t id, EmbVector delta);
void obj_update_rubber_grip(uint32_t obj);

EmbVectorList *all_grip_points(int32_t obj_id);
EmbVector mouse_snap_point(int32_t obj_id, EmbVector mousePoint);

/* ---------------------------- Global Variables --------------------------- */
/* Global variables and constants we need to access anywhere in the program
 * with minimal overhead.
 */

extern Command command_data[MAX_COMMANDS];
extern StringMap aliases[MAX_ALIASES];
extern Setting setting[N_SETTINGS];
extern GroupBoxData group_box_list[];
extern Translation translations[];
extern Design designs[];
extern const char *svgs[];

extern const char *config_table[];
extern char *settings_data[];

extern ScriptValue *config;
extern int n_variables;

extern const char *help_msg;

extern const char *index_th_name[];
extern const char *os;
extern bool exitApp;

/* Versions */
extern const char *embroidermodder_version;
extern const char *libembroidery_version;
extern const char *EmbroideryMobile_version;
extern const char *PET_version;

/* Paths */
extern const char *circle_origin_path;
extern const char *one_path;
extern const char *two_path;
extern const char *three_path;
extern const char *four_path;
extern const char *five_path;
extern const char *six_path;
extern const char *seven_path;
extern const char *eight_path;
extern const char *nine_path;
extern const char *zero_path;
extern const char *minus_path;
extern const char *apostrophe_path;
extern const char *quote_path;

/* Menus */
extern char *menu_list[];
extern int menubar_full_list[];
extern int menubar_no_docs[];

extern char *file_menu[];
extern char *edit_menu[];
extern char *view_menu[];
extern char *zoom_menu[];
extern char *pan_menu[];
extern char *help_menu[];
extern char *draw_menu[];
extern char *tools_menu[];
extern char *modify_menu[];
extern char *dimension_menu[];
extern char *sandbox_menu[];
extern char *recent_menu[];
extern char *window_menu[];

/* Toolbars */
extern char *toolbar_list[];
extern int toolbars_when_docs[];

extern int top_toolbar[];
extern int left_toolbar[];
extern int bottom_toolbar[];
extern int toolbar_horizontal[];

extern char *file_toolbar[];
extern char *edit_toolbar[];
extern char *view_toolbar[];
extern char *zoom_toolbar[];
extern char *pan_toolbar[];
extern char *icon_toolbar[];
extern char *help_toolbar[];
extern char *draw_toolbar[];
extern char *inquiry_toolbar[];
extern char *modify_toolbar[];
extern char *dimension_toolbar[];
extern char *sandbox_toolbar[];
extern char *layer_toolbar[];
extern char *properties_toolbar[];
extern char *text_toolbar[];
extern char *prompt_toolbar[];

/* Widget Groups */
extern char *grid_load_from_file_group[];
extern char *defined_origin_group[];
extern char *rectangular_grid_group[];
extern char *circular_grid_group[];
extern char *center_on_origin_group[];
extern char *grid_enabled_group[];
extern char *rectangular_grid_visible_group[];
extern char *circular_grid_visible_group[];

/* Settings */
extern char *settings_tab_labels[];
extern int preview_to_dialog[];
extern int accept_to_dialog[];
extern int render_hints[];

/* Objects */
extern char *object_names[];

/* Testing */
extern char *coverage_test[];

/* Misc */
extern char *tips[];
extern char *extensions[];

extern bool document_memory[MAX_OPEN_FILES];

extern char formatFilterOpen[MAX_LONG_STRING];
extern char formatFilterSave[MAX_LONG_STRING];
extern char open_files_path[MAX_STRING_LENGTH];
extern char prompt_color_[MAX_STRING_LENGTH];
extern char prompt_selection_bg_color_[MAX_STRING_LENGTH];
extern char prompt_bg_color_[MAX_STRING_LENGTH];
extern char prompt_selection_color_[MAX_STRING_LENGTH];

extern char lastCmd[MAX_STRING_LENGTH];

extern ScriptEnv *global;

extern const ScriptValue script_null;
extern const ScriptValue script_true;
extern const ScriptValue script_false;

extern bool blinkState;

extern int testing_mode;
extern char *button_list[];

extern char *_appName_;
extern char *_appVer_;
extern char settings_file[MAX_STRING_LENGTH];

extern char *layer_list[];
extern char *color_list[];
extern char *line_type_list[];
extern char *line_weight_list[];
extern char *text_size_list[];

extern char *editor_list[];
extern char *combobox_list[];

extern char *grid_layout[];
extern char *zoom_layout[];

extern char **xpm_icons[];
extern char *xpm_icon_labels[];

extern char recent_files[MAX_FILES][MAX_STRING_LENGTH];

extern int numOfDocs;
extern int docIndex;
extern bool shiftKeyPressedState;
extern bool cmdActive;
extern bool rapidFireEnabled;
extern bool isBlinking;
extern bool key_state[N_KEY_SEQUENCES];

extern StringMap aliasHash[MAX_ALIASES];
extern IntMap obj_index[MAX_OBJECTS];

extern EmbIdList *cutCopyObjectList;

extern bool blinkState;

extern int iconSize;

extern bool pickAdd;

extern int promptInputNum;

extern int precisionAngle;
extern int precisionLength;

extern int n_aliases;
extern int n_objects;
extern int n_commands;
extern int n_widgets;
extern int n_actions;

extern char curText[MAX_STRING_LENGTH];

extern char promptHistoryData[MAX_LONG_STRING];

extern char defaultPrefix[200];
extern char prefix[200];
extern char curCmd[200];

/* Used when checking if fields vary. */
extern char fieldOldText[MAX_STRING_LENGTH];
extern char fieldNewText[MAX_STRING_LENGTH];
extern char fieldVariesText[MAX_STRING_LENGTH];
extern char fieldYesText[MAX_STRING_LENGTH];
extern char fieldNoText[MAX_STRING_LENGTH];
extern char fieldOnText[MAX_STRING_LENGTH];
extern char fieldOffText[MAX_STRING_LENGTH];

extern WidgetData grid_gb_data[];
extern WidgetData zoom_gb_data[];

#ifdef __cplusplus
}
#endif

#endif

