/*
 * Embroidermodder 2.
 *
 * Copyright 2011-2024 The Embroidermodder Team
 * Embroidermodder 2 is Open Source Software, see LICENSE for licensing terms.
 * Read CODE-STANDARDS.txt for advice on altering this file.
 *
 * C Core
 */

#ifndef EMBROIDERMODDER_C_CORE_HEADER
#define EMBROIDERMODDER_C_CORE_HEADER

#ifdef __cplusplus
extern "C" {
#endif

#include <stdbool.h>

#define MAX_ARGS                      10
#define MAX_STRING_LENGTH            200

#define SCRIPT_NULL                    0
#define SCRIPT_INT                     1
#define SCRIPT_REAL                    2
#define SCRIPT_COMMAND                 3
#define SCRIPT_STRING                  4
#define SCRIPT_BOOL                    5

#define CONTEXT_MAIN                   0
#define CONTEXT_CLICK                  1
#define CONTEXT_MOVE                   2
#define CONTEXT_CONTEXT                3
#define CONTEXT_PROMPT                 4

enum COMMAND_ACTIONS
{
    ACTION_DO_NOTHING,

    ACTION_NEW,
    ACTION_OPEN,
    ACTION_SAVE,
    ACTION_SAVE_AS,
    ACTION_PRINT,
    ACTION_DESIGN_DETAILS,
    ACTION_EXIT,
    ACTION_CUT,
    ACTION_COPY,
    ACTION_PASTE,

    ACTION_UNDO,
    ACTION_REDO,

    /* Window Menu */
    ACTION_WINDOW_CLOSE,
    ACTION_WINDOW_CLOSE_ALL,
    ACTION_WINDOW_CASCADE,
    ACTION_WINDOW_TILE,
    ACTION_WINDOW_NEXT,
    ACTION_WINDOW_PREVIOUS,

    /* Help Menu */
    ACTION_HELP,
    ACTION_CHANGELOG,
    ACTION_TIP_OF_THE_DAY,
    ACTION_ABOUT,
    ACTION_WHATS_THIS,

    /* Icons */
    ACTION_ICON_16,
    ACTION_ICON_24,
    ACTION_ICON_32,
    ACTION_ICON_48,
    ACTION_ICON_64,
    ACTION_ICON_128,

    ACTION_SETTINGS_DIALOG,

    /* Layer ToolBar */
    ACTION_MAKE_LAYER_CURRENT,
    ACTION_LAYERS,
    ACTION_LAYER_SELECTOR,
    ACTION_LAYER_PREVIOUS,
    ACTION_COLOR_SELECTOR,
    ACTION_LINE_TYPE_SELECTOR,
    ACTION_LINE_WEIGHT_SELECTOR,
    ACTION_HIDE_ALL_LAYERS,
    ACTION_SHOW_ALL_LAYERS,
    ACTION_FREEZE_ALL_LAYERS,
    ACTION_THAW_ALL_LAYERS,
    ACTION_LOCK_ALL_LAYERS,
    ACTION_UNLOCK_ALL_LAYERS,

    /* Text ToolBar */
    ACTION_TEXT_BOLD,
    ACTION_TEXT_ITALIC,
    ACTION_TEXT_UNDERLINE,
    ACTION_TEXT_STRIKEOUT,
    ACTION_TEXT_OVERLINE,

    /* Zoom ToolBar */
    ACTION_ZOOM_REAL_TIME,
    ACTION_ZOOM_PREVIOUS,
    ACTION_ZOOM_WINDOW,
    ACTION_ZOOM_DYNAMIC,
    ACTION_ZOOM_SCALE,
    ACTION_ZOOM_CENTER,
    ACTION_ZOOM_IN,
    ACTION_ZOOM_OUT,
    ACTION_ZOOM_SELECTED,
    ACTION_ZOOM_ALL,
    ACTION_ZOOM_EXTENTS,

    /* Pan SubMenu */
    ACTION_PAN_REAL_TIME,
    ACTION_PAN_POINT,
    ACTION_PAN_LEFT,
    ACTION_PAN_RIGHT,
    ACTION_PAN_UP,
    ACTION_PAN_DOWN,

    ACTION_DAY,
    ACTION_NIGHT,

    /* Prompt. */
    ACTION_ALERT,
    ACTION_GET,
    ACTION_SET,

    /* Selection management. */
    ACTION_CLEAR,

    /* TODO: ACTION_spellcheck, */
    /* TODO: ACTION_quickselect, */

    ACTION_ANGLE,
    ACTION_CIRCLE,
    ACTION_DEBUG,
    ACTION_DISABLE,
    ACTION_DISTANCE,
    ACTION_DOLPHIN,
    ACTION_ELLIPSE,
    ACTION_ENABLE,
    ACTION_ERASE,
    ACTION_ERROR,
    ACTION_HEART,
    ACTION_LINE,
    ACTION_LOCATE_POINT,
    ACTION_MIRROR_SELECTED,
    ACTION_MOVE,
    ACTION_MOVE_SELECTED,
    ACTION_PATH,
    ACTION_PLATFORM,
    ACTION_POINT,
    ACTION_POLYGON,
    ACTION_POLYLINE,
    ACTION_PREVIEW_OFF,
    ACTION_PREVIEW_ON,
    ACTION_QUICKLEADER,
    ACTION_RECTANGLE,
    ACTION_RGB,
    ACTION_ROTATE,
    ACTION_SANDBOX,
    ACTION_SCALE,
    ACTION_SCALE_SELECTED,
    ACTION_SELECT_ALL,
    ACTION_SINGLE_LINE_TEXT,
    ACTION_SNOWFLAKE,
    ACTION_STAR,
    ACTION_SYSWINDOWS,
    ACTION_TODO,
    ACTION_VULCANIZE,

    ACTION_NULL
};

typedef struct ScriptValue_ {
    double r;
    int i;
    bool b;
    char s[MAX_STRING_LENGTH];
    char label[MAX_STRING_LENGTH];
    struct ScriptValue_ *leaf;
    int n_leaves;
    char type;
} ScriptValue;

typedef struct ScriptEnv_ {
    ScriptValue *variable;
    int n_variables;
    int context;
    ScriptValue argument[MAX_ARGS];
    int argumentCount;
} ScriptEnv;

typedef struct Command_ {
    int id;
    ScriptValue (*main)(ScriptEnv *context);
    const char *icon;
    const char *menu_name;
    int menu_position;
    const char *toolbar_name;
    int toolbar_position;
    const char *tooltip;
    const char *statustip;
    const char *alias;
    const char *shortcut;
} Command;

/* Scripting functions */
ScriptEnv *create_script_env();
void free_script_env(ScriptEnv *);

ScriptValue script_bool(bool b);
ScriptValue script_int(int i);
ScriptValue script_real(double r);
ScriptValue script_string(const char *s);
ScriptValue do_nothing(ScriptEnv *context);
ScriptValue stub_implement(const char *function);
ScriptValue command_prompt(const char *line);

ScriptEnv *add_string_argument(ScriptEnv *context, const char *s);
ScriptEnv *add_real_argument(ScriptEnv *context, double r);
ScriptEnv *add_int_argument(ScriptEnv *context, int i);

void add_string_variable(ScriptEnv *context, const char *label, const char *s);
void add_int_variable(ScriptEnv *context, const char *label, int i);
void add_real_variable(ScriptEnv *context, const char *label, double i);

const char *script_get_string(ScriptEnv *context, const char *label);
int script_get_int(ScriptEnv *context, const char *label);
double script_get_real(ScriptEnv *context, const char *label);

int script_set_string(ScriptEnv *context, const char *label, const char *s);
int script_set_int(ScriptEnv *context, const char *label, int i);
int script_set_real(ScriptEnv *context, const char *label, double r);

/* Global variables with c linkage. */
extern Command command_data[];

extern ScriptValue script_null;
extern ScriptValue script_true;
extern ScriptValue script_false;

extern const char *coverage_test[];
extern int testing_mode;

/* Commands */
ScriptValue about_command(ScriptEnv*);
ScriptValue alert_command(ScriptEnv*);
ScriptValue angle_command(ScriptEnv*);
ScriptValue circle_command(ScriptEnv*);
ScriptValue changelog_command(ScriptEnv*);
ScriptValue clear_command(ScriptEnv*);
ScriptValue copy_command(ScriptEnv*);
ScriptValue cut_command(ScriptEnv*);
ScriptValue day_command(ScriptEnv *);
ScriptValue debug_command(ScriptEnv *);
ScriptValue design_details_command(ScriptEnv *);
ScriptValue disable_command(ScriptEnv*);
ScriptValue distance_command(ScriptEnv*);
ScriptValue dolphin_command(ScriptEnv*);
ScriptValue ellipse_command(ScriptEnv *);
ScriptValue enable_command(ScriptEnv *);
ScriptValue erase_command(ScriptEnv *);
ScriptValue error_command(ScriptEnv *);
ScriptValue exit_command(ScriptEnv *);
ScriptValue heart_command(ScriptEnv *);
ScriptValue heart_command(ScriptEnv*);
ScriptValue help_command(ScriptEnv*);
ScriptValue icon128_command(ScriptEnv*);
ScriptValue icon16_command(ScriptEnv*);
ScriptValue icon24_command(ScriptEnv*);
ScriptValue icon32_command(ScriptEnv*);
ScriptValue icon48_command(ScriptEnv*);
ScriptValue icon64_command(ScriptEnv*);
ScriptValue get_command(ScriptEnv*);
ScriptValue line_command(ScriptEnv*);
ScriptValue locatepoint_command(ScriptEnv*);
ScriptValue mirrorselected_command(ScriptEnv*);
ScriptValue move_command(ScriptEnv*);
ScriptValue moveselected_command(ScriptEnv*);
ScriptValue new_command(ScriptEnv*);
ScriptValue night_command(ScriptEnv*);
ScriptValue open_command(ScriptEnv*);
ScriptValue paste_command(ScriptEnv*);
ScriptValue print_command(ScriptEnv*);
ScriptValue redo_command(ScriptEnv*);

ScriptValue panrealtime_command(ScriptEnv*);
ScriptValue pandown_command(ScriptEnv*);
ScriptValue panleft_command(ScriptEnv*);
ScriptValue panright_command(ScriptEnv*);
ScriptValue panup_command(ScriptEnv*);

ScriptValue path_command(ScriptEnv*);
ScriptValue platform_command(ScriptEnv*);
ScriptValue previewoff_command(ScriptEnv*);
ScriptValue previewon_command(ScriptEnv*);
ScriptValue point_command(ScriptEnv*);
ScriptValue polygon_command(ScriptEnv*);
ScriptValue polyline_command(ScriptEnv*);
ScriptValue rectangle_command(ScriptEnv*);
ScriptValue rotate_command(ScriptEnv*);
ScriptValue rgb_command(ScriptEnv*);
ScriptValue quickleader_command(ScriptEnv*);
ScriptValue save_command(ScriptEnv*);
ScriptValue sandbox_command(ScriptEnv*);
ScriptValue scale_command(ScriptEnv*);
ScriptValue scaleselected_command(ScriptEnv*);
ScriptValue selectall_command(ScriptEnv*);
ScriptValue settings_dialog_command(ScriptEnv*);
ScriptValue singlelinetext_command(ScriptEnv*);
ScriptValue set_command(ScriptEnv*);
ScriptValue snowflake_command(ScriptEnv*);
ScriptValue star_command(ScriptEnv*);
ScriptValue syswindows_command(ScriptEnv*);

ScriptValue text_bold_command(ScriptEnv*);
ScriptValue text_italic_command(ScriptEnv*);
ScriptValue text_underline_command(ScriptEnv*);
ScriptValue text_overline_command(ScriptEnv*);
ScriptValue text_strikeout_command(ScriptEnv*);

ScriptValue tipoftheday_command(ScriptEnv*);
ScriptValue todo_command(ScriptEnv*);
ScriptValue undo_command(ScriptEnv*);
ScriptValue vulcanize_command(ScriptEnv*);

ScriptValue whats_this_command(ScriptEnv*);

ScriptValue windowcascade_command(ScriptEnv*);
ScriptValue windowclose_command(ScriptEnv*);
ScriptValue windowcloseall_command(ScriptEnv*);
ScriptValue windownext_command(ScriptEnv*);
ScriptValue windowprevious_command(ScriptEnv*);
ScriptValue windowtile_command(ScriptEnv*);

ScriptValue zoom_all_command(ScriptEnv*);
ScriptValue zoom_center_command(ScriptEnv*);
ScriptValue zoom_dynamic_command(ScriptEnv*);
ScriptValue zoom_extents_command(ScriptEnv*);
ScriptValue zoom_in_command(ScriptEnv*);
ScriptValue zoom_out_command(ScriptEnv*);
ScriptValue zoom_previous_command(ScriptEnv*);
ScriptValue zoom_real_time_command(ScriptEnv*);
ScriptValue zoom_scale_command(ScriptEnv*);
ScriptValue zoom_selected_command(ScriptEnv*);
ScriptValue zoom_window_command(ScriptEnv*);

ScriptValue colorselector_command(ScriptEnv*);
ScriptValue hidealllayers_command(ScriptEnv*);
ScriptValue freezealllayers_command(ScriptEnv*);
ScriptValue layers_command(ScriptEnv*);
ScriptValue layerprevious_command(ScriptEnv*);
ScriptValue layerselector_command(ScriptEnv*);
ScriptValue linetypeselector_command(ScriptEnv*);
ScriptValue lineweightselector_command(ScriptEnv*);
ScriptValue lockalllayers_command(ScriptEnv*);
ScriptValue makelayercurrent_command(ScriptEnv*);
ScriptValue panpoint_command(ScriptEnv*);
ScriptValue showalllayers_command(ScriptEnv*);
ScriptValue thawalllayers_command(ScriptEnv*);
ScriptValue unlockalllayers_command(ScriptEnv*);

#ifdef __cplusplus
}
#endif

#endif
