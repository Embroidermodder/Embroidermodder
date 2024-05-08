/*
 * Embroidermodder 2.
 *
 * Copyright 2013-2024 The Embroidermodder Team
 * Embroidermodder 2 is Open Source Software, see LICENSE for licensing terms.
 * Read CODE-STANDARDS.txt for advice on altering this file.
 *
 * Command: ICON128
 */

#include "../commands.h"

/* ICON128 is not context-sensitive. */
ScriptValue
icon128_generic(ScriptEnv *context)
{
    _main->nativeIcon128();
    _main->nativeEndCommand();
    return script_null;
}

/* NOTE: main() is run every time the command is started.
 *       Use it to reset variables so they are ready to go.
 */
ScriptValue
icon128_main(ScriptEnv *context)
{
    _main->nativeInitCommand();
    _main->nativeClearSelection();
    return icon128_generic(context);
}

Command icon128_cmd = {
    .main = icon128_main,
    .click = icon128_generic,
    .context = icon128_generic,
    .prompt = icon128_generic,
    .icon = "icon128",
    .menu_name = "None",
    .menu_position = 0,
    .toolbar_name = "None",
    .toolbar_position = 0,
    .tooltip = "Icon12&8",
    .statustip = "Sets the toolbar icon size to 128x128:  ICON128",
    .alias = "ICON128"
};
