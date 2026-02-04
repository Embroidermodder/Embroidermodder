#ifndef __EMBROIDERMODDER_COMMANDS__
#define __EMBROIDERMODDER_COMMANDS__

#include "mainwindow.h"
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

    ACTION_null
};

typedef struct CommandData_ {
    enum COMMAND_ACTIONS id;
    char label[100];
    char tooltip[1000];
    char statustip[1000];
    int (*command)(State *state);
} CommandData;

int about_cmd(State *state);
int do_nothing_cmd(State *state);

extern CommandData command_table[];

#endif
