/*
 *  Embroidermodder 2.
 *
 *  Copyright 2013-2024 The Embroidermodder Team
 *  Embroidermodder 2 is Open Source Software.
 *  See LICENSE for licensing terms.
 *
 */

#ifndef SCRIPT_H
#define SCRIPT_H

#include "../extern/libembroidery/embroidery.h"

#define REAL(i)             context->argument[i].r
#define INT(i)              context->argument[i].i
#define QSTR(i)    QString(context->argument[i].s)
#define BOOL(i)             context->argument[i].b

#define MAX_ARGS                      10
#define MAX_STRING_LENGTH            200

#define SCRIPT_NULL                    0
#define SCRIPT_INT                     1
#define SCRIPT_REAL                    2
#define SCRIPT_COMMAND                 3
#define SCRIPT_STRING                  4
#define SCRIPT_BOOL                    5

typedef struct ScriptValue_ {
    double r;
    int i;
    bool b;
    char s[100];
    char label[100];
    struct ScriptValue_ *leaf;
    int n_leaves;
    char type;
} ScriptValue;

typedef struct ScriptEnv_ {
    ScriptValue *commands;
    int n_commands;
    ScriptValue argument[MAX_ARGS];
    int argumentCount;
} ScriptEnv;

typedef struct Command_ {
    int id;
    ScriptValue (*main)(ScriptEnv *context);
    ScriptValue (*click)(ScriptEnv *context);
    ScriptValue (*context)(ScriptEnv *context);
    ScriptValue (*prompt)(ScriptEnv *context);
    const char *icon;
    const char *menu_name;
    int menu_position;
    const char *toolbar_name;
    int toolbar_position;
    const char *tooltip;
    const char *statustip;
    const char *alias;
} Command;

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

extern Command about_cmd;
extern Command alert_cmd;
extern Command circle_cmd;
extern Command clear_cmd;
extern Command day_cmd;
extern Command distance_cmd;
extern Command dolphin_cmd;
extern Command donothing_cmd;
extern Command ellipse_cmd;
extern Command erase_cmd;
extern Command exit_cmd;
extern Command heart_cmd;
extern Command help_cmd;
extern Command get_cmd;
extern Command icon128_cmd;
extern Command icon16_cmd;
extern Command icon24_cmd;
extern Command icon32_cmd;
extern Command icon48_cmd;
extern Command icon64_cmd;
extern Command line_cmd;
extern Command locatepoint_cmd;
extern Command move_cmd;
extern Command new_cmd;
extern Command night_cmd;
extern Command open_cmd;
extern Command pandown_cmd;
extern Command panleft_cmd;
extern Command panright_cmd;
extern Command panup_cmd;
extern Command path_cmd;
extern Command platform_cmd;
extern Command point_cmd;
extern Command polygon_cmd;
extern Command polyline_cmd;
extern Command quickleader_cmd;
extern Command rectangle_cmd;
extern Command redo_cmd;
extern Command rgb_cmd;
extern Command rotate_cmd;
extern Command sandbox_cmd;
extern Command scale_cmd;
extern Command selectall_cmd;
extern Command set_cmd;
extern Command singlelinetext_cmd;
extern Command snowflake_cmd;
extern Command star_cmd;
extern Command syswindows_cmd;
extern Command tipoftheday_cmd;
extern Command undo_cmd;
extern Command windowcascade_cmd;
extern Command windowclose_cmd;
extern Command windowcloseall_cmd;
extern Command windownext_cmd;
extern Command windowprevious_cmd;
extern Command windowtile_cmd;
extern Command zoomextents_cmd;
extern Command zoomin_cmd;
extern Command zoomout_cmd;

extern Command command_list[];

extern ScriptValue script_null;
extern ScriptValue script_true;
extern ScriptValue script_false;

enum COMMAND_ACTIONS
{
    ACTION_donothing,

    ACTION_new,
    ACTION_open,
    ACTION_save,
    ACTION_saveas,
    ACTION_print,
    ACTION_designdetails,
    ACTION_exit,
    ACTION_cut,
    ACTION_copy,
    ACTION_paste,

    ACTION_undo,
    ACTION_redo,
    // Window Menu
    ACTION_windowclose,
    ACTION_windowcloseall,
    ACTION_windowcascade,
    ACTION_windowtile,
    ACTION_windownext,
    ACTION_windowprevious,
    // Help Menu
    ACTION_help,
    ACTION_changelog,
    ACTION_tipoftheday,
    ACTION_about,
    ACTION_whatsthis,
    // Icons
    ACTION_icon16,
    ACTION_icon24,
    ACTION_icon32,
    ACTION_icon48,
    ACTION_icon64,
    ACTION_icon128,

    ACTION_settingsdialog,

    // Layer ToolBar
    ACTION_makelayercurrent,
    ACTION_layers,
    ACTION_layerselector,
    ACTION_layerprevious,
    ACTION_colorselector,
    ACTION_linetypeselector,
    ACTION_lineweightselector,
    ACTION_hidealllayers,
    ACTION_showalllayers,
    ACTION_freezealllayers,
    ACTION_thawalllayers,
    ACTION_lockalllayers,
    ACTION_unlockalllayers,
    //Text ToolBar
    ACTION_textbold,
    ACTION_textitalic,
    ACTION_textunderline,
    ACTION_textstrikeout,
    ACTION_textoverline,
    // Zoom ToolBar
    ACTION_zoomrealtime,
    ACTION_zoomprevious,
    ACTION_zoomwindow,
    ACTION_zoomdynamic,
    ACTION_zoomscale,
    ACTION_zoomcenter,
    ACTION_zoomin,
    ACTION_zoomout,
    ACTION_zoomselected,
    ACTION_zoomall,
    ACTION_zoomextents,
    // Pan SubMenu
    ACTION_panrealtime,
    ACTION_panpoint,
    ACTION_panleft,
    ACTION_panright,
    ACTION_panup,
    ACTION_pandown,

    ACTION_day,
    ACTION_night,

    /* Prompt. */
    ACTION_ALERT,
    ACTION_GET,
    ACTION_SET,

    /* Selection management. */
    ACTION_CLEAR,

    //TODO: ACTION_spellcheck,
    //TODO: ACTION_quickselect,

    ACTION_null
};

#endif // SCRIPT_H
