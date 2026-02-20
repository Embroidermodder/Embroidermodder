#ifndef __EMBROIDERMODDER_COMMANDS__
#define __EMBROIDERMODDER_COMMANDS__

#include "state.h"

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

    //TODO: ACTION_spellcheck,
    //TODO: ACTION_quickselect,

    ACTION_scripted,

    ACTION_null
};

enum COMMAND_TYPE
{
    CMD_TYPE_TOGGLE,
    CMD_TYPE_TRIGGER
};

typedef struct CommandData_ {
    enum COMMAND_ACTIONS id;
    enum COMMAND_TYPE type;
    char label[100];
    char tooltip[1000];
    char statustip[1000];
    char shortcut[50];
    char mac_shortcut[50];
    int (*command)(State *state);
} CommandData;

int do_nothing_cmd(State *state);

int new_cmd(State *state);
int open_cmd(State *state);
int save_cmd(State *state);
int save_as_cmd(State *state);
int print_cmd(State *state);
int design_details_cmd(State *state);
int exit_cmd(State *state);
int cut_cmd(State *state);
int copy_cmd(State *state);
int paste_cmd(State *state);

int undo_cmd(State *state);
int redo_cmd(State *state);

// Window Menu
int window_close_cmd(State *state);
int window_close_all_cmd(State *state);
int window_cascade_cmd(State *state);
int window_tile_cmd(State *state);
int window_next_cmd(State *state);
int window_previous_cmd(State *state);

// Help Menu
int help_cmd(State *state);
int changelog_cmd(State *state);
int tip_of_the_day_cmd(State *state);
int about_cmd(State *state);
int whats_this_cmd(State *state);

// Icons
int icon16_cmd(State *state);
int icon24_cmd(State *state);
int icon32_cmd(State *state);
int icon48_cmd(State *state);
int icon64_cmd(State *state);
int icon128_cmd(State *state);

int settingsdialog_cmd(State *state);

// Layer ToolBar
int makelayercurrent_cmd(State *state);
int layers_cmd(State *state);
int layerselector_cmd(State *state);
int layerprevious_cmd(State *state);
int colorselector_cmd(State *state);
int linetypeselector_cmd(State *state);
int lineweightselector_cmd(State *state);
int hidealllayers_cmd(State *state);
int showalllayers_cmd(State *state);
int freezealllayers_cmd(State *state);
int thawalllayers_cmd(State *state);
int lockalllayers_cmd(State *state);
int unlockalllayers_cmd(State *state);

//Text ToolBar
int text_bold_cmd(State *state);
int text_italic_cmd(State *state);
int text_underline_cmd(State *state);
int text_strikeout_cmd(State *state);
int text_overline_cmd(State *state);

// Zoom ToolBar
int zoom_real_time_cmd(State *state);
int zoom_previous_cmd(State *state);
int zoom_window_cmd(State *state);
int zoom_dynamic_cmd(State *state);
int zoom_scale_cmd(State *state);
int zoom_center_cmd(State *state);
int zoom_in_cmd(State *state);
int zoom_out_cmd(State *state);
int zoom_selected_cmd(State *state);
int zoom_all_cmd(State *state);
int zoom_extents_cmd(State *state);

// Pan SubMenu
int pan_real_time_cmd(State *state);
int pan_point_cmd(State *state);
int pan_left_cmd(State *state);
int pan_right_cmd(State *state);
int pan_up_cmd(State *state);
int pan_down_cmd(State *state);

int day_cmd(State *state);
int night_cmd(State *state);

extern CommandData command_table[];

#endif
