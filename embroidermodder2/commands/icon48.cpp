/*
 * Embroidermodder 2.
 *
 * Copyright 2013-2024 The Embroidermodder Team
 * Embroidermodder 2 is Open Source Software, see LICENSE for licensing terms.
 * Read CODE-STANDARDS.txt for advice on altering this file.
 *
 * Command: ICON48
 */

#include "../commands.h"

/* ICON48 is not context-dependant */
ScriptValue
icon48_generic(ScriptEnv *context)
{
    if (!argument_checks(context, "icon48_generic", "")) {
        return script_false;
    }
    _main->icon48();
    _main->nativeEndCommand();
    return script_null;
}

/* NOTE: main() is run every time the command is started.
 *       Use it to reset variables so they are ready to go.
 */
ScriptValue
icon48_main(ScriptEnv * context)
{
    if (!argument_checks(context, "icon48_main", "")) {
        return script_false;
    }
    _main->nativeInitCommand();
    _main->nativeClearSelection();
    return icon48_generic(context);
}

Command icon48_cmd = {
    .id = -1,
    .main = icon48_main,
    .click = icon48_generic,
    .context = icon48_generic,
    .prompt = icon48_generic,
    .icon = "icon48",
    .menu_name = "None",
    .menu_position = 0,
    .toolbar_name = "None",
    .toolbar_position = 0,
    .tooltip = "Icon&48",
    .statustip = "Sets the toolbar icon size to 48x48:  ICON48",
    .alias = "ICON48"
};
