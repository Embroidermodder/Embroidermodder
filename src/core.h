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

typedef struct ScriptValue_ {
    double r;
    int i;
    bool b;
    char s[MAX_STATE_STRING_LENGTH];
    char label[MAX_LABEL_LENGTH];
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
    char command[MAX_STRING_LENGTH];
    char arguments[MAX_STRING_LENGTH];
    char icon[MAX_STRING_LENGTH];
    char tooltip[MAX_STRING_LENGTH];
    char statustip[MAX_STRING_LENGTH];
    char alias[MAX_STRING_LENGTH];
    char shortcut[MAX_STRING_LENGTH];
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
    char key[MAX_STRING_LENGTH];
    ScriptValue setting;
    ScriptValue dialog;
    ScriptValue accept;
    ScriptValue preview;
} Setting;

typedef struct StringMap_ {
    char key[MAX_LABEL_LENGTH];
    char value[MAX_LABEL_LENGTH];
} StringMap;

typedef struct Editor_ {
    char icon[MAX_STRING_LENGTH];
    char label[MAX_STRING_LENGTH];
    char data_type[MAX_STRING_LENGTH];
    char signal[MAX_STRING_LENGTH];
    int object;
} Editor;

typedef struct SettingsData_ {
    int id;
    char label[MAX_STRING_LENGTH];
    char key[MAX_STRING_LENGTH];
    char value[MAX_STRING_LENGTH];
    char icon[MAX_STRING_LENGTH];
    int type;
} SettingsData;

/* Scripting functions */
ScriptEnv *create_script_env();
void free_script_env(ScriptEnv *);

ScriptValue script_bool(bool b);
ScriptValue script_int(int i);
ScriptValue script_real(double r);
ScriptValue script_string(const char *s);
ScriptValue command_prompt(ScriptEnv *context, const char *line);

ScriptEnv *add_string_argument(ScriptEnv *context, const char *s);
ScriptEnv *add_real_argument(ScriptEnv *context, double r);
ScriptEnv *add_int_argument(ScriptEnv *context, int i);

void set_int(int key, int value);
void set_real(int key, double value);
void set_str(int key, char *value);
void set_bool(int key, bool value);

int get_int(int key);
double get_real(int key);
char *get_str(int key);
bool get_bool(int key);

void add_string_variable(ScriptEnv *context, const char *label, const char *s);
void add_int_variable(ScriptEnv *context, const char *label, int i);
void add_real_variable(ScriptEnv *context, const char *label, double i);

const char *script_get_string(ScriptEnv *context, const char *label);
int script_get_int(ScriptEnv *context, const char *label);
double script_get_real(ScriptEnv *context, const char *label);

int script_set_string(ScriptEnv *context, const char *label, const char *s);
int script_set_int(ScriptEnv *context, const char *label, int i);
int script_set_real(ScriptEnv *context, const char *label, double r);

void prompt_output(const char *);
int argument_checks(ScriptEnv *context, int id);
char *translate(const char *msg);

bool pattern_save(EmbPattern *pattern, const char *fileName);

int string_array_length(char *s[]);
int parse_floats(char *line, float result[], int n);
int parse_vector(char *line, EmbVector *v);
bool validRGB(float r, float g, float b);
void reportDistance(EmbVector a, EmbVector b);

void add_to_menu(int id, char *menu_data[]);
void add_to_toolbar(int id, char *toolbar_data[]);

int load_data(void);

int load_settings(char *appDir, char *configDir);
int save_settings(const char *appDir, const char *configDir);

int get_command_id(const char *);
int get_state_variable(const char *key);

EmbArc emb_arc_set_radius(EmbArc a, EmbReal radius);

bool willUnderflowInt32(int64_t a, int64_t b);
bool willOverflowInt32(int64_t a, int64_t b);
int roundToMultiple(bool roundUp, int numToRound, int multiple);

void information_box(char *title, char *text);
void warning_box(char *title, char *text);
void critical_box(char *title, char *text);
void question_box(char *title, char *text);

/* MainWindow calls */
void end_command(void);
void debug_message(const char *msg);
void wait_cursor(void);
void arrow_cursor(void);
void restore_cursor(void);

void cut(void);

void about_dialog(void);

/* Global variables with c linkage. */
extern Command command_data[MAX_COMMANDS];
extern StringMap aliases[MAX_ALIASES];
extern Setting setting[N_SETTINGS];
extern SettingsData settings_data[N_SETTINGS];

extern ScriptValue script_null;
extern ScriptValue script_true;
extern ScriptValue script_false;

extern int testing_mode;

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

extern int render_hints[];

extern char *coverage_test[];
extern char *object_names[];

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
extern char **menu_data[];

extern char *toolbar_list[];
extern int toolbars_when_docs[];

extern char *file_toolbar[];
extern char *edit_toolbar[];
extern char *view_toolbar[];
extern char *zoom_toolbar[];
extern char *pan_toolbar[] ;
extern char *icon_toolbar[];
extern char *help_toolbar[];
extern char *draw_toolbar[];
extern char *inquiry_toolbar[];
extern char *modify_toolbar[];
extern char *dimension_toolbar[];
extern char *sandbox_toolbar[];
extern char **toolbar_data[];

extern char *layer_list[];
extern char *color_list[];
extern char *line_type_list[];
extern char *line_weight_list[];
extern char *text_size_list[];

extern char *extensions[];
extern char *editor_list[];
extern char *combobox_list[];

extern char *grid_load_from_file_group[];
extern char *defined_origin_group[];
extern char *rectangular_grid_group[];
extern char *circular_grid_group[];
extern char *center_on_origin_group[];

extern char *tips[];

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

/* Natives */
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

void nativeSetCursorShape(char str[MAX_STRING_LENGTH]);
double nativeCalculateDistance(double x1, double y1, double x2, double y2);
double nativePerpendicularDistance(double px, double py, double x1, double y1, double x2, double y2);

double nativeQSnapX();
double nativeQSnapY();

void enableLwt();
void disableLwt();
void enableReal();
void disableReal();

void create_details_dialog(void);

void nativeSetPromptPrefix(char txt[MAX_STRING_LENGTH]);
/* TODO: void nativeSetRubberFilter(int64_t id); */
/* TODO: This is so more than 1 rubber object can exist at one time without updating all rubber objects at once. */
void nativeSetRubberMode(int mode);
void nativeSetRubberPoint(char key[MAX_STRING_LENGTH], double x, double y);
void nativeSetRubberText(char key[MAX_STRING_LENGTH], char txt[MAX_STRING_LENGTH]);

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

/* Geometry */
int emb_approx(EmbVector point1, EmbVector point2);

double emb_width(EmbGeometry *geometry);
double emb_height(EmbGeometry *geometry);
double emb_radius(EmbGeometry *geometry);
double emb_radius_major(EmbGeometry *geometry);
double emb_radius_minor(EmbGeometry *geometry);
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

#ifdef __cplusplus
}
#endif

#endif

