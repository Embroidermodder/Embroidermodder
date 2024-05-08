/*
 * Embroidermodder 2.
 *
 * Copyright 2013-2024 The Embroidermodder Team
 * Embroidermodder 2 is Open Source Software.
 * See LICENSE for licensing terms.
 *
 * Command: ZOOMOUT
 */

#include "../commands.h"

/* ICON16 is not context-dependant */
ScriptValue
zoomout_generic(ScriptEnv * /* context */)
{
    _main->nativeZoomOut();
    _main->nativeEndCommand();
    return script_null;
}

/* NOTE: main() is run every time the command is started.
 *       Use it to reset variables so they are ready to go.
 */
ScriptValue
zoomout_main(ScriptEnv *context)
{
    _main->nativeInitCommand();
    _main->nativeClearSelection();
    return zoomout_generic(context);
}

Command zoomout_cmd = {
    .main = zoomout_main,
    .click = zoomout_generic,
    .context = zoomout_generic,
    .prompt = zoomout_generic,
    .icon = "zoomout",
    .menu_name = "None",
    .menu_position = 0,
    .toolbar_name = "None",
    .toolbar_position = 0,
    .tooltip = "Zoom &Out",
    .statustip = "Zooms to decrease the apparent size of objects:  ZOOMOUT",
    .alias = "ZOOMOUT"
};
