/*
 * Embroidermodder 2.
 *
 * Copyright 2013-2024 The Embroidermodder Team
 * Embroidermodder 2 is Open Source Software, see LICENSE for licensing terms.
 * Read CODE-STANDARDS.txt for advice on altering this file.
 *
 * Command: ICON64
 */

#include "../commands.h"

/* ICON64 is not context-sensitive. */
ScriptValue
icon64_generic(ScriptEnv * context)
{
    _main->icon64();
    _main->nativeEndCommand();
    return script_null;
}

/* NOTE: main() is run every time the command is started.
 *       Use it to reset variables so they are ready to go.
 */
ScriptValue
icon64_main(ScriptEnv * context)
{
    _main->nativeInitCommand();
    _main->nativeClearSelection();
    return icon64_generic(context);
}

Command icon64_cmd = {
    .main = icon64_main,
    .click = icon64_generic,
    .context = icon64_generic,
    .prompt = icon64_generic,
    .icon = "ICON64",
    .menu_name = "None",
    .menu_position = 0,
    .toolbar_name = "None",
    .toolbar_position = 0,
    .tooltip = "Icon&64",
    .statustip = "Sets the toolbar icon size to 64x64:  ICON64",
    .alias = "ICON64"
};
