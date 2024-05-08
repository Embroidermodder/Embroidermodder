/*
 * Embroidermodder 2.
 *
 * Copyright 2013-2024 The Embroidermodder Team
 * Embroidermodder 2 is Open Source Software, see LICENSE for licensing terms.
 * Read CODE-STANDARDS.txt for advice on altering this file.
 *
 * Command: ICON24
 */

#include "../commands.h"

/* ICON24 is not context-dependant */
ScriptValue
icon24_generic(ScriptEnv * context)
{
    _main->nativeIcon24();
    _main->nativeEndCommand();
    return script_null;
}

/* NOTE: main() is run every time the command is started.
 *       Use it to reset variables so they are ready to go.
 */
ScriptValue
icon24_main(ScriptEnv *context)
{
    _main->nativeInitCommand();
    _main->nativeClearSelection();
    return icon24_generic(context);
}

Command icon24_cmd = {
    .main = icon24_main,
    .click = icon24_generic,
    .context = icon24_generic,
    .prompt = icon24_generic,
    .icon = "icon24",
    .menu_name = "None",
    .menu_position = 0,
    .toolbar_name = "None",
    .toolbar_position = 0,
    .tooltip = "Icon&24",
    .statustip = "Sets the toolbar icon size to 24x24:  ICON24",
    .alias = "ICON24"
};

