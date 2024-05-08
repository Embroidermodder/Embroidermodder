/*
 * Embroidermodder 2.
 *
 * Copyright 2013-2024 The Embroidermodder Team
 * Embroidermodder 2 is Open Source Software.
 * See LICENSE for licensing terms.
 *
 * Command: WINDOWTILE
 */

#include "../commands.h"

/* WINDOWTILE is not context-dependant */
ScriptValue
windowtile_generic(ScriptEnv * /* context */)
{
    _main->nativeWindowTile();
    _main->nativeEndCommand();
    return script_null;
}

/* NOTE: main() is run every time the command is started.
 *       Use it to reset variables so they are ready to go.
 */
ScriptValue
windowtile_main(ScriptEnv *context)
{
    _main->nativeInitCommand();
    _main->nativeClearSelection();
    return windowtile_generic(context);
}

Command windowtile_cmd = {
    .main = windowtile_main,
    .click = windowtile_generic,
    .context = windowtile_generic,
    .prompt = windowtile_generic,
    .icon = "windowtile",
    .menu_name = "None",
    .menu_position = 0,
    .toolbar_name = "None",
    .toolbar_position = 0,
    .tooltip = "&Tile",
    .statustip = "Tile the windows:  TILE",
    .alias = "TILE, WINDOWTILE"
};
