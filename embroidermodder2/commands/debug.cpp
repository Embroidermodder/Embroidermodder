/*
 * Embroidermodder 2.
 *
 * Copyright 2013-2024 The Embroidermodder Team
 * Embroidermodder 2 is Open Source Software.
 * See LICENSE for licensing terms.
 *
 * Command: DEBUG
 */

#include "../commands.h"

/* . */
ScriptValue
debug_command(ScriptEnv *context)
{
    if (!argument_checks(context, "about", "s")) {
        return script_false;
    }
    _main->nativeAppendPromptHistory(QSTR(0));
    _main->nativeEndCommand();
    return script_null;
}

/* NOTE: main() is run every time the command is started.
 *       Use it to reset variables so they are ready to go.
 */
ScriptValue
debug_main(ScriptEnv *context)
{
    if (!argument_checks(context, "about", "s")) {
        return script_false;
    }
    _main->nativeInitCommand();
    _main->nativeClearSelection();
    return debug_generic(context);
}

Command debug_cmd = {
    .main = debug_main,
    .click = debug_click,
    .context = debug_context,
    .prompt = debug_prompt,
    .icon = "about",
    .menu_name = "None",
    .menu_position = 0,
    .toolbar_name = "None",
    .toolbar_position = 0,
    .tooltip = "Debug",
    .statustip = "Displays information about this product:  ABOUT",
    .alias = "DEBUG"
};
