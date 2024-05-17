/*
 * Embroidermodder 2.
 *
 * Copyright 2013-2024 The Embroidermodder Team
 * Embroidermodder 2 is Open Source Software.
 * See LICENSE for licensing terms.
 *
 * Command: ZOOMIN
 */

#include "../commands.h"

/* ZOOMIN is not context-dependant */
ScriptValue
zoomin_generic(ScriptEnv * /* context */)
{
    _main->nativeZoomIn();
    _main->nativeEndCommand();
    return script_null;
}

/* NOTE: main() is run every time the command is started.
 *       Use it to reset variables so they are ready to go.
 */
ScriptValue
zoomin_main(ScriptEnv *context)
{
    _main->nativeInitCommand();
    _main->nativeClearSelection();
    return zoomin_generic(context);
}

Command zoomin_cmd = {
    .id = -1,
    .main = zoomin_main,
    .click = zoomin_generic,
    .context = zoomin_generic,
    .prompt = zoomin_generic,
    .icon = "zoomin",
    .menu_name = "None",
    .menu_position = 0,
    .toolbar_name = "None",
    .toolbar_position = 0,
    .tooltip = "Zoom &In",
    .statustip = "Zooms to increase the apparent size of objects:  ZOOMIN",
    .alias = "ZOOMIN"
};
