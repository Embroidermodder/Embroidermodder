/*
 * Embroidermodder 2 -- Commands
 * Copyright 2011-2026 The Embroidermodder Team
 */

#include "mainwindow.h"
#include "commands.h"

QtScriptEnv script_env;

/*
 * This action intentionally does nothing: it is present as a dummy function
 * or the "null" action.
 */
int
do_nothing_cmd(State *state)
{
    return 0;
}

int
new_cmd(State *state)
{
    script_env.mainWin->newFile();
    return 0;
}

int
open_cmd(State *state)
{
    script_env.mainWin->openFile();
    return 0;
}

int
save_cmd(State *state)
{
    script_env.mainWin->savefile();
    return 0;
}

int
save_as_cmd(State *state)
{
    script_env.mainWin->saveasfile();
    return 0;
}

int
print_cmd(State *state)
{
    script_env.mainWin->print();
    return 0;
}

int
design_details_cmd(State *state)
{
    script_env.mainWin->designDetails();
    return 0;
}

int
exit_cmd(State *state)
{
    script_env.mainWin->exit();
    return 0;
}

int
cut_cmd(State *state)
{
    script_env.mainWin->cut();
    return 0;
}

int
copy_cmd(State *state)
{
    script_env.mainWin->copy();
    return 0;
}

int
paste_cmd(State *state)
{
    script_env.mainWin->paste();
    return 0;
}

int
undo_cmd(State *state)
{
    script_env.mainWin->undo();
    return 0;
}

int
redo_cmd(State *state)
{
    script_env.mainWin->redo();
    return 0;
}

int
window_close_cmd(State *state)
{
    script_env.mainWin->onCloseWindow();
    return 0;
}

int
window_close_all_cmd(State *state)
{
    script_env.mainWin->mdiArea->closeAllSubWindows();
    return 0;
}

int
window_cascade_cmd(State *state)
{
    script_env.mainWin->mdiArea->cascade();
    return 0;
}

int
window_tile_cmd(State *state)
{
    script_env.mainWin->mdiArea->tile();
    return 0;
}

int
window_next_cmd(State *state)
{
    script_env.mainWin->mdiArea->activateNextSubWindow();
    return 0;
}

int
window_previous_cmd(State *state)
{
    script_env.mainWin->mdiArea->activatePreviousSubWindow();
    return 0;
}

int
help_cmd(State *state)
{
    script_env.mainWin->help();
    return 0;
}

int
changelog_cmd(State *state)
{
    script_env.mainWin->changelog();
    return 0;
}

int
tip_of_the_day_cmd(State *state)
{
    script_env.mainWin->tipOfTheDay();
    return 0;
}

int
about_cmd(State *state)
{
    script_env.mainWin->about();
    return 0;
}

int
whats_this_cmd(State *state)
{
    script_env.mainWin->whatsThis();
    return 0;
}

int
icon16_cmd(State *state)
{
    script_env.mainWin->icon16();
    return 0;
}

int
icon24_cmd(State *state)
{
    script_env.mainWin->icon24();
    return 0;
}

int
icon32_cmd(State *state)
{
    script_env.mainWin->icon32();
    return 0;
}

int
icon48_cmd(State *state)
{
    script_env.mainWin->icon48();
    return 0;
}

int
icon64_cmd(State *state)
{
    script_env.mainWin->icon64();
    return 0;
}

int
icon128_cmd(State *state)
{
    script_env.mainWin->icon128();
    return 0;
}

int
settingsdialog_cmd(State *state)
{
    script_env.mainWin->settingsDialog();
    return 0;
}

int
makelayercurrent_cmd(State *state)
{
    script_env.mainWin->makeLayerActive();
    return 0;
}

int
layers_cmd(State *state)
{
    script_env.mainWin->layerManager();
    return 0;
}

/* FIXME */
int
layerselector_cmd(State *state)
{
    return 0;
}

int
layerprevious_cmd(State *state)
{
    script_env.mainWin->layerPrevious();
    return 0;
}

/* FIXME */
int
colorselector_cmd(State *state)
{
    return 0;
}

/* FIXME */
int
linetypeselector_cmd(State *state)
{
    return 0;
}

/* FIXME */
int
lineweightselector_cmd(State *state)
{
    return 0;
}

/* FIXME */
int
hidealllayers_cmd(State *state)
{
    return 0;
}

/* FIXME */
int
showalllayers_cmd(State *state)
{
    return 0;
}

/* FIXME */
int
freezealllayers_cmd(State *state)
{
    return 0;
}

/* FIXME */
int
thawalllayers_cmd(State *state)
{
    return 0;
}

/* FIXME */
int
lockalllayers_cmd(State *state)
{
    return 0;
}

/* FIXME */
int
unlockalllayers_cmd(State *state)
{
    return 0;
}

int
text_bold_cmd(State *state)
{
    script_env.mainWin->textBold();
    return 0;
}

int
text_italic_cmd(State *state)
{
    script_env.mainWin->textItalic();
    return 0;
}

int
text_underline_cmd(State *state)
{
    script_env.mainWin->textUnderline();
    return 0;
}

int
text_strikeout_cmd(State *state)
{
    script_env.mainWin->textStrikeOut();
    return 0;
}

int
text_overline_cmd(State *state)
{
    script_env.mainWin->textOverline();
    return 0;
}

int
zoom_real_time_cmd(State *state)
{
    script_env.mainWin->zoomRealtime();
    return 0;
}

int
zoom_previous_cmd(State *state)
{
    script_env.mainWin->zoomPrevious();
    return 0;
}

int
zoom_window_cmd(State *state)
{
    script_env.mainWin->zoomWindow();
    return 0;
}

int
zoom_dynamic_cmd(State *state)
{
    script_env.mainWin->zoomDynamic();
    return 0;
}

int
zoom_scale_cmd(State *state)
{
    script_env.mainWin->zoomScale();
    return 0;
}

int
zoom_center_cmd(State *state)
{
    script_env.mainWin->zoomCenter();
    return 0;
}

int
zoom_in_cmd(State *state)
{
    script_env.mainWin->zoomIn();
    return 0;
}

int
zoom_out_cmd(State *state)
{
    script_env.mainWin->zoomOut();
    return 0;
}

int
zoom_selected_cmd(State *state)
{
    script_env.mainWin->zoomSelected();
    return 0;
}

int
zoom_all_cmd(State *state)
{
    script_env.mainWin->zoomAll();
    return 0;
}

int
zoom_extents_cmd(State *state)
{
    script_env.mainWin->zoomExtents();
    return 0;
}

int
pan_real_time_cmd(State *state)
{
    script_env.mainWin->panrealtime();
    return 0;
}

int
pan_point_cmd(State *state)
{
    script_env.mainWin->panpoint();
    return 0;
}

int
pan_left_cmd(State *state)
{
    script_env.mainWin->panLeft();
    return 0;
}

int
pan_right_cmd(State *state)
{
    script_env.mainWin->panRight();
    return 0;
}

int
pan_up_cmd(State *state)
{
    script_env.mainWin->panUp();
    return 0;
}

int
pan_down_cmd(State *state)
{
    script_env.mainWin->panDown();
    return 0;
}

int
day_cmd(State *state)
{
    script_env.mainWin->dayVision();
    return 0;
}

int
night_cmd(State *state)
{
    script_env.mainWin->nightVision();
    return 0;
}

