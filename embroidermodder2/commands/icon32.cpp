/*
 * Embroidermodder 2.
 *
 * Copyright 2013-2024 The Embroidermodder Team
 * Embroidermodder 2 is Open Source Software, see LICENSE for licensing terms.
 * Read CODE-STANDARDS.txt for advice on altering this file.
 *
 * Command: ICON32
 */

#include "../commands.h"

/* ICON24 is not context-dependant */
ScriptValue
icon32_generic(ScriptEnv *context)
{
    if (!argument_checks(context, "icon32_generic", "")) {
        return script_false;
    }
    _main->icon32();
    _main->nativeEndCommand();
    return script_null;
}

/* NOTE: main() is run every time the command is started.
 *       Use it to reset variables so they are ready to go.
 */
ScriptValue
icon32_main(ScriptEnv *context)
{
    if (!argument_checks(context, "icon32_main", "")) {
        return script_false;
    }
    _main->nativeInitCommand();
    _main->nativeClearSelection();
    return icon32_generic(context);
}

Command icon32_cmd = {
    .id = -1,
    .main = icon32_main,
    .click = icon32_generic,
    .context = icon32_generic,
    .prompt = icon32_generic,
    .icon = "icon32",
    .menu_name = "None",
    .menu_position = 0,
    .toolbar_name = "None",
    .toolbar_position = 0,
    .tooltip = "Icon&32",
    .statustip = "Sets the toolbar icon size to 32x32:  ICON32",
    .alias = "ICON32"
};
