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

#include <stdlib.h>
#include <stdbool.h>
#include <inttypes.h>
#include <math.h>

#include "../extern/libembroidery/embroidery.h"

#include "data/constants.h"

#define REAL(i)   context->argument[i].r
#define INT(i)    context->argument[i].i
#define STR(i)    context->argument[i].s
#define BOOL(i)   context->argument[i].b

#define MAX_TABLE_LENGTH             500
#define MAX_OPEN_FILES               100

typedef char EmbString[MAX_STRING_LENGTH];
typedef char EmbStringTable[MAX_TABLE_LENGTH][MAX_STRING_LENGTH];

typedef struct ScriptValue_ {
    double r;
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

    double scale;
    double rotation;

    uint32_t rgb;

    EmbString OBJ_NAME;

    EmbString text;
    EmbString textFont;
    EmbString textJustify;
    double textSize;
    bool textBold;
    bool textItalic;
    bool textUnderline;
    bool textStrikeOut;
    bool textOverline;
    bool textBackward;
    bool textUpsideDown;

    int rubber_mode;
    int64_t objID;

    bool curved;
    bool filled;
    double arrowStyleAngle;
    double arrowStyleLength;
    double lineStyleAngle;
    double lineStyleLength;

    int gripIndex;
} ObjectCore;

/* . */
typedef struct DocumentData_ {
    int32_t id;

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
    double previewData;
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
    bool enableLwt;
    bool enableReal;

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

typedef struct UndoData_ {
    int type;
    int32_t doc;
    EmbVector delta;
    EmbVector pivot;
    double angle;
    double factor;
    EmbString navType;
    EmbVector fromCenter;
    EmbVector toCenter;
    bool done;
    EmbVector before;
    EmbVector after;
    EmbLine mirrorLine;
} UndoData;

/* Scripting functions */
ScriptEnv *create_script_env();
void free_script_env(ScriptEnv *);

ScriptValue script_bool(bool b);
ScriptValue script_int(int i);
ScriptValue script_real(double r);
ScriptValue script_string(EmbString s);
ScriptValue command_prompt(ScriptEnv *context, EmbString line);

ScriptEnv *add_string_argument(ScriptEnv *context, EmbString s);
ScriptEnv *add_real_argument(ScriptEnv *context, double r);
ScriptEnv *add_int_argument(ScriptEnv *context, int i);

ScriptValue *setting_ptr(int key, int mode);
void copy_setting(int key, int dst, int src);

void set_int(int key, int value);
void set_real(int key, double value);
void set_str(int key, EmbString value);
void set_bool(int key, bool value);

int get_int(int key);
double get_real(int key);
char *get_str(int key);
bool get_bool(int key);

void add_string_variable(ScriptEnv *context, const EmbString label, EmbString s);
void add_int_variable(ScriptEnv *context, const EmbString label, int i);
void add_real_variable(ScriptEnv *context, const EmbString label, double i);

const char *script_get_string(ScriptEnv *context, const EmbString label);
int script_get_int(ScriptEnv *context, const EmbString label);
double script_get_real(ScriptEnv *context, const EmbString label);

int script_set_string(ScriptEnv *context, const EmbString label, EmbString s);
int script_set_int(ScriptEnv *context, const EmbString label, int i);
int script_set_real(ScriptEnv *context, const EmbString label, double r);

void prompt_output(const char *);
int argument_checks(ScriptEnv *context, int id);
char *translate(const char *msg);

bool pattern_save(EmbPattern *pattern, EmbString fileName);

int parse_floats(const char *line, float result[], int n);
int parse_vector(const char *line, EmbVector *v);
bool validRGB(float r, float g, float b);
void reportDistance(EmbVector a, EmbVector b);

void add_to_menu(int id, EmbStringTable menu_data);
void add_to_toolbar(int id, EmbStringTable toolbar_data);

int load_data(void);

int load_settings(const char *appDir, const char *configDir);
int save_settings(EmbString appDir, EmbString configDir);

int get_command_id(EmbString );
int get_state_variable(EmbString key);

EmbArc emb_arc_set_radius(EmbArc a, EmbReal radius);

bool willUnderflowInt32(int64_t a, int64_t b);
bool willOverflowInt32(int64_t a, int64_t b);
int roundToMultiple(bool roundUp, int numToRound, int multiple);

void information_box(const char *title, const char *text);
void warning_box(const char *title, const char *text);
void critical_box(const char *title, const char *text);
void question_box(const char *title, const char *text);

char *get_svg_token(char *svg, char token[MAX_STRING_LENGTH]);
char *get_svg_vector(char *svg, EmbVector *v);

const char *getCurrentLayer();
uint32_t getCurrentColor();
const char *getCurrentLineType();
const char *getCurrentLineWeight();

/* ------------------------------ Prompt ------------------------------- */

void setPromptTextColor(uint32_t color);
void setPromptBackgroundColor(uint32_t color);
void setPromptFontFamily(EmbString family);
void setPromptFontStyle(EmbString style);
void setPromptFontSize(int size);

/* -------------------------- Main Functions --------------------------- */

void runCommandMain(const char *cmd);
void runCommandClick(const char *cmd, double x, double y);
void runCommandMove(const char *cmd, double x, double y);
void runCommandContext(const char *cmd, const char *str);
void runCommandPrompt(const char *cmd);

uint32_t rgb(uint8_t r, uint8_t g, uint8_t b);

void updateAllViewScrollBars(bool val);
void updateAllViewCrossHairColors(uint32_t color);
void updateAllViewBackgroundColors(uint32_t color);
void updateAllViewSelectBoxColors(uint32_t colorL, uint32_t fillL,
    uint32_t colorR, uint32_t fillR, int alpha);
void updateAllViewGridColors(uint32_t color);
void updateAllViewRulerColors(uint32_t color);

void updatePickAddMode(bool val);
void pickAddModeToggled(void);

void setTextFont(EmbString str);
void setTextSize(double num);

void promptHistoryAppended(EmbString txt);
void logPromptInput(EmbString txt);

void openFile(bool recent, EmbString recentFile);
void openFilesSelected(EmbStringTable);

void settingsDialog(EmbString showTab);

bool validFileFormat(EmbString fileName);

void onCloseWindow(void);

void setUndoCleanIcon(bool opened);

void currentLayerChanged(EmbString layer);
void currentColorChanged(uint32_t color);
void currentLinetypeChanged(EmbString type);
void currentLineweightChanged(EmbString weight);

void promptInputPrevious(void);
void promptInputNext(void);

void print_command(void);
void undo_command(void);
void redo_command(void);

void layerSelectorIndexChanged(int index);
void linetypeSelectorIndexChanged(int index);
void lineweightSelectorIndexChanged(int index);
void textSizeSelectorIndexChanged(int index);

void makeLayerActive(void);
void layerManager(void);
void layerPrevious(void);

void deletePressed(void);
void escapePressed(void);

bool isShiftPressed(void);
void setShiftPressed(void);
void setShiftReleased(void);

void iconResize(int iconSize);

void readSettings(void);
void writeSettings(void);

void createAllActions(void);

void loadFormats(void);

void settingsPrompt(void);

void end_command(void);
void debug_message(const char *msg);
void wait_cursor(void);
void arrow_cursor(void);
void restore_cursor(void);

void cut(void);

void about_dialog(void);

/* ------------------------------------- Natives --------------------------- */

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
void save_as_file(void);
void update_interface(void);
void windowMenuAboutToShow();
void hide_unimplemented(void);
void start_blinking(void);
void stop_blinking(void);
void repeat_action(void);
void move_action(void);

ScriptValue run_command(const char *cmd, ScriptEnv *context);

void nativeBlinkPrompt();

void checkBoxTipOfTheDayStateChanged(int checked);
void buttonTipOfTheDayClicked(int button);

ScriptValue previewon_command(ScriptEnv *context);
ScriptValue get_command(ScriptEnv *context);
ScriptValue set_command(ScriptEnv *context);
ScriptValue move_command(ScriptEnv *context);
ScriptValue sandbox_command(ScriptEnv *context);

void nativeAddInfiniteLine(double x1, double y1, double x2, double y2, double rot);
void nativeAddRay(double x1, double y1, double x2, double y2, double rot);
void nativeAddLine(double x1, double y1, double x2, double y2, double rot, int rubberMode);
void nativeAddTriangle(double x1, double y1, double x2, double y2, double x3, double y3, double rot, bool fill);
void nativeAddRectangle(double x, double y, double w, double h, double rot, bool fill, int rubberMode);
void nativeAddRoundedRectangle(double x, double y, double w, double h, double rad, double rot, bool fill);
void nativeAddArc(double x1, double y1, double x2, double y2, double x3, double y3, int rubberMode);
void nativeAddCircle(double centerX, double centerY, double radius, bool fill, int rubberMode);
void nativeAddSlot(double centerX, double centerY, double diameter, double length, double rot, bool fill, int rubberMode);
void nativeAddEllipse(double centerX, double centerY, double width, double height, double rot, bool fill, int rubberMode);
void nativeAddPoint(double x, double y);
void nativeAddRegularPolygon(double centerX, double centerY, uint16_t sides, uint8_t mode, double rad, double rot, bool fill);
void nativeAddHorizontalDimension(double x1, double y1, double x2, double y2, double legHeight);
void nativeAddVerticalDimension(double x1, double y1, double x2, double y2, double legHeight);
void nativeAddDimLeader(double x1, double y1, double x2, double y2, double rot, int rubberMode);

void nativeAddTextMulti(char *str, double x, double y, double rot, bool fill, int rubberMode);
void nativeAddTextSingle(char *str, double x, double y, double rot, bool fill, int rubberMode);

void nativeAddImage(char *img, double x, double y, double w, double h, double rot);

void nativeRedo();

void setMouseCoord(double x, double y);

void nativePrintArea(double x, double y, double w, double h);

void nativeSetBackgroundColor(uint8_t r, uint8_t g, uint8_t b);
void nativeSetCrossHairColor(uint8_t r, uint8_t g, uint8_t b);
void nativeSetGridColor(uint8_t r, uint8_t g, uint8_t b);

void nativeClearRubber();
bool nativeAllowRubber();
void nativeSpareRubber(int64_t id);

void nativeDeleteSelected();
void nativeCutSelected(double x, double y);
void nativeCopySelected(double x, double y);
void nativePasteSelected(double x, double y);
void nativeMoveSelected(double dx, double dy);
void nativeScaleSelected(double x, double y, double factor);
void nativeRotateSelected(double x, double y, double rot);
void nativeMirrorSelected(double x1, double y1, double x2, double y2);

void nativeSetCursorShape(EmbString str);
double nativeCalculateDistance(double x1, double y1, double x2, double y2);
double nativePerpendicularDistance(double px, double py, double x1, double y1, double x2, double y2);

double nativeQSnapX();
double nativeQSnapY();

void enableLwt();
void disableLwt();
void enableReal();
void disableReal();

void create_details_dialog(void);

void nativeSetPromptPrefix(EmbString txt);
/* TODO: void nativeSetRubberFilter(int64_t id); */
/* TODO: This is so more than 1 rubber object can exist at one time without updating all rubber objects at once. */
void nativeSetRubberMode(int mode);
void nativeSetRubberPoint(const EmbString key, double x, double y);
void nativeSetRubberText(const EmbString key, EmbString txt);

void toggleGrid(void);
void toggleRuler(void);
void toggleLwt(void);

/* Help Menu */
void tipOfTheDay();
void buttonTipOfTheDayClicked(int);
void checkBoxTipOfTheDayStateChanged(int);
void help();
void changelog();
void whatsThisContextHelp();

int make_application(int argc, char* argv[]);

void setObjectRubberMode(ObjectCore *core, int mode);

/* ------------------------------- Document -------------------------------- */

bool doc_allow_zoom_in(int32_t doc);
bool doc_allow_zoom_out(int32_t doc);
void doc_zoom_in(int32_t doc);
void doc_zoom_out(int32_t doc);
void doc_zoom_window(int32_t doc);
void doc_zoom_selected(int32_t doc);
void doc_zoom_extents(int32_t doc);
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
void doc_move_selected(int32_t doc, double dx, double dy);
void doc_cut(int32_t doc);
void doc_copy(int32_t doc);
void doc_paste(int32_t doc);
void doc_repeat_action(int32_t doc);
void doc_move_action(int32_t doc);
void doc_scale_action(int32_t doc);
void doc_scale_selected(int32_t doc, double x, double y, double factor);
void doc_rotate_action(int32_t doc);
void doc_rotate_selected(int32_t doc, double x, double y, double rot);
void doc_mirror_selected(int32_t doc, double x1, double y1, double x2, double y2);
int doc_num_selected(int32_t doc);

void doc_set_grid_color(int32_t doc, uint32_t color);
void doc_set_ruler_color(int32_t doc, uint32_t color);

void doc_preview_on(int32_t doc, int clone, int mode, double x, double y, double data);
void doc_preview_off(int32_t doc);

void doc_delete_pressed(int32_t doc);
void doc_escape_pressed(int32_t doc);

void doc_corner_button_clicked(int32_t doc);

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
void doc_zoom_to_point(int32_t doc, EmbVector mousePoint, int zoomDir);
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

void doc_pan_left(int doc_index);
void doc_pan_right(int doc_index);
void doc_pan_up(int doc_index);
void doc_pan_down(int doc_index);

void doc_zoom_selected(int doc_index);
void doc_zoom_extents(int doc_index);

double doc_width(int doc_index);
double doc_height(int doc_index);

void doc_update(int doc_index);

void doc_center_on(int32_t doc, EmbVector v);

DocumentData *doc_data(int32_t doc);

void move_action(void);
void rotate_action(void);
void scale_action(void);

EmbVector doc_map_from_scene(int32_t doc, EmbVector v);
EmbVector doc_map_to_scene(int32_t doc, EmbVector v);

int32_t activeDocument(void);

void whats_this_mode(void);

void window_close_all(void);
void window_cascade(void);
void window_tile(void);
void window_next(void);
void window_previous(void);

void enable_rapid_fire(void);
void disable_rapid_fire(void);

void appendHistory(EmbString txt);

/* -------------------------------- EmbString ------------------------------ */

void emb_string(EmbString s, const char *str);
int string_equal(EmbString a, const char *b);
int string_compare(EmbString a, const char *b);
void string_copy(EmbString dst, const char *src);
int string_array_length(EmbString s[]);
int string_list_contains(EmbStringTable list, EmbString entry);

/* ----------------------------- Object Core ------------------------------- */

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
void obj_set_x(ObjectCore *obj, double x);
void obj_set_y(ObjectCore *obj, double y);

void obj_set_center(ObjectCore *obj, EmbVector point);
void obj_set_center_x(ObjectCore *obj, double centerX);
void obj_set_center_y(ObjectCore *obj, double centerY);

void obj_set_end_point_1(ObjectCore *obj, EmbVector endPt1);
void obj_set_end_point_2(ObjectCore *obj, EmbVector endPt2);

void obj_set_x1(ObjectCore *obj, double x);
void obj_set_y1(ObjectCore *obj, double y);
void obj_set_x2(ObjectCore *obj, double x);
void obj_set_y2(ObjectCore *obj, double y);

void obj_set_rubber_mode(ObjectCore *core, int mode);

EmbVector obj_top_left(ObjectCore *obj);
EmbVector obj_top_right(ObjectCore *obj);
EmbVector obj_bottom_left(ObjectCore *obj);
EmbVector obj_bottom_right(ObjectCore *obj);

EmbRect obj_rect(ObjectCore *obj);

void obj_set_text(ObjectCore *obj, const char *str);
void obj_set_text_font(ObjectCore *obj, const char *font);
void obj_set_text_justify(ObjectCore *obj, const char *justify);
void obj_set_text_size(ObjectCore *obj, double size);
void obj_set_text_style(ObjectCore *obj, bool bold, bool italic, bool under, bool strike, bool over);
void obj_set_text_bold(ObjectCore *obj, bool val);
void obj_set_text_italic(ObjectCore *obj, bool val);
void obj_set_text_underline(ObjectCore *obj, bool val);
void obj_set_text_strikeout(ObjectCore *obj, bool val);
void obj_set_text_overline(ObjectCore *obj, bool val);
void obj_set_text_backward(ObjectCore *obj, bool val);
void obj_set_text_upside_down(ObjectCore *obj, bool val);

/* ---------------------------------- Geometry ----------------------------- */

uint32_t emb_pattern_real_count(EmbPattern *pattern);
uint32_t emb_pattern_count_type(EmbPattern *pattern, uint32_t flags);
double emb_pattern_shortest_stitch(EmbPattern *pattern);
double emb_pattern_longest_stitch(EmbPattern *pattern);
void emb_color_histogram(EmbPattern *pattern, int **bins);
void emb_length_histogram(EmbPattern *pattern, int *bins);
double emb_total_thread_length(EmbPattern *pattern);
double emb_total_thread_of_color(EmbPattern *pattern, int thread_index);

int emb_approx(EmbVector point1, EmbVector point2);

EmbVector scale_and_rotate(EmbVector v, double angle, double scale);

double emb_width(EmbGeometry *geometry);
double emb_height(EmbGeometry *geometry);
double emb_radius(EmbGeometry *geometry);
double emb_radius_major(EmbGeometry *geometry);
double emb_radius_minor(EmbGeometry *geometry);
double emb_diameter(EmbGeometry *geometry);
double emb_diameter_major(EmbGeometry *geometry);
double emb_diameter_minor(EmbGeometry *geometry);
EmbVector emb_quadrant(EmbGeometry *geometry, int degrees);
double emb_angle(EmbGeometry *geometry);
double emb_start_angle(EmbGeometry *geometry);
double emb_end_angle(EmbGeometry *geometry);
double emb_arc_length(EmbGeometry *geometry);
double emb_area(EmbGeometry *geometry);
double emb_chord(EmbGeometry *geometry);
double emb_included_angle(EmbGeometry *geometry);
bool emb_clockwise(EmbGeometry *geometry);
double emb_circumference(EmbGeometry *geometry);

void emb_set_start_angle(EmbGeometry *geometry, double angle);
void emb_set_end_angle(EmbGeometry *geometry, double angle);
void emb_set_start_point(EmbGeometry *geometry, EmbVector point);
void emb_set_mid_point(EmbGeometry *geometry, EmbVector point);
void emb_set_end_point(EmbGeometry *geometry, EmbVector point);
void emb_set_diameter(EmbGeometry *geometry, double diameter);
void emb_set_area(EmbGeometry *geometry, double area);
void emb_set_circumference(EmbGeometry *geometry, double circumference);
void emb_set_radius(EmbGeometry *geometry, double radius);
void emb_set_radius_major(EmbGeometry *geometry, double radius);
void emb_set_radius_minor(EmbGeometry *geometry, double radius);
void emb_set_diameter_major(EmbGeometry *geometry, double diameter);
void emb_set_diameter_minor(EmbGeometry *geometry, double diameter);

/* ---------------------------- Global Variables --------------------------- */

extern Command command_data[MAX_COMMANDS];
extern StringMap aliases[MAX_ALIASES];
extern Setting setting[N_SETTINGS];
extern SettingsData settings_data[N_SETTINGS];

extern ScriptValue script_null;
extern ScriptValue script_true;
extern ScriptValue script_false;

extern int testing_mode;

extern EmbString end_symbol;

extern Editor general_editor_data[];
extern Editor geometry_arc_editor_data[];
extern Editor misc_arc_editor_data[];
extern Editor geometry_block_editor_data[];
extern Editor geometry_circle_editor_data[];
extern Editor geometry_dim_aligned_editor_data[];
extern Editor geometry_dim_angular_editor_data[];
extern Editor geometry_dim_arc_length_editor_data[];
extern Editor geometry_dim_diameter_editor_data[];
extern Editor geometry_dim_leader_editor_data[];
extern Editor geometry_dim_linear_editor_data[];
extern Editor geometry_dim_ordinate_editor_data[];
extern Editor geometry_dim_radius_editor_data[];
extern Editor geometry_ellipse_editor_data[];
extern Editor geometry_image_editor_data[];
extern Editor misc_image_editor_data[];
extern Editor geometry_infinite_line_editor_data[];
extern Editor geometry_line_editor_data[];
extern Editor geometry_path_editor_data[];
extern Editor misc_path_editor_data[];
extern Editor geometry_point_editor_data[];
extern Editor geometry_polygon_editor_data[];
extern Editor geometry_polyline_editor_data[];
extern Editor misc_polyline_editor_data[];
extern Editor geometry_ray_editor_data[];
extern Editor geometry_rectangle_editor_data[];
extern Editor geometry_text_multi_editor_data[];
extern Editor text_text_single_editor_data[];
extern Editor geometry_text_single_editor_data[];
extern Editor misc_text_single_editor_data[];

extern const char *_appName_;
extern const char *_appVer_;
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
extern const char *circle_origin_path;
extern EmbString settings_file;

extern int render_hints[];

extern EmbStringTable coverage_test;
extern EmbStringTable object_names;

extern EmbStringTable menu_list;
extern int menubar_full_list[];
extern int menubar_no_docs[];

extern EmbStringTable file_menu;
extern EmbStringTable edit_menu;
extern EmbStringTable view_menu;
extern EmbStringTable zoom_menu;
extern EmbStringTable pan_menu;
extern EmbStringTable help_menu;
extern EmbStringTable draw_menu;
extern EmbStringTable tools_menu;
extern EmbStringTable modify_menu;
extern EmbStringTable dimension_menu;
extern EmbStringTable sandbox_menu;

extern EmbStringTable toolbar_list;
extern int toolbars_when_docs[];

extern int preview_to_dialog[];
extern int accept_to_dialog[];

extern EmbStringTable file_toolbar;
extern EmbStringTable edit_toolbar; 
extern EmbStringTable view_toolbar; 
extern EmbStringTable zoom_toolbar; 
extern EmbStringTable pan_toolbar;
extern EmbStringTable icon_toolbar; 
extern EmbStringTable help_toolbar; 
extern EmbStringTable draw_toolbar; 
extern EmbStringTable inquiry_toolbar; 
extern EmbStringTable modify_toolbar; 
extern EmbStringTable dimension_toolbar; 
extern EmbStringTable sandbox_toolbar;

extern int top_toolbar[];
extern int left_toolbar[];
extern int bottom_toolbar[];
extern int toolbar_horizontal[];

extern EmbStringTable layer_list; 
extern EmbStringTable color_list; 
extern EmbStringTable line_type_list; 
extern EmbStringTable line_weight_list; 
extern EmbStringTable text_size_list; 

extern EmbStringTable extensions; 
extern EmbStringTable editor_list; 
extern EmbStringTable combobox_list; 

extern EmbStringTable grid_load_from_file_group; 
extern EmbStringTable defined_origin_group; 
extern EmbStringTable rectangular_grid_group; 
extern EmbStringTable circular_grid_group; 
extern EmbStringTable center_on_origin_group; 

extern EmbStringTable tips; 

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

#ifdef __cplusplus
}
#endif

#endif

