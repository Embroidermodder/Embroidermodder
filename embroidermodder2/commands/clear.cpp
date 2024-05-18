/*
 * Embroidermodder 2.
 *
 * Copyright 2013-2024 The Embroidermodder Team
 * Embroidermodder 2 is Open Source Software.
 * See LICENSE for licensing terms.
 *
 * Command: about
 */

#include "../commands.h"

ScriptValue
clear_generic(ScriptEnv* context)
{
    if (!argument_checks(context, "clear_generic", "")) {
        return script_false;
    }

    _main->nativeInitCommand();
    _main->nativeClearSelection();
    _main->nativeEndCommand();
    return script_null;
}

Command clear_cmd = {
    .id = ACTION_CLEAR,
    .main = clear_generic,
    .click = clear_generic,
    .context = clear_generic,
    .prompt = clear_generic,
    .icon = "donothing",
    .menu_name = "None",
    .menu_position = 0,
    .toolbar_name = "None",
    .toolbar_position = 0,
    .tooltip = "&Do Nothing",
    .statustip = "Does Nothing.",
    .alias = "DONOTHING"
};
