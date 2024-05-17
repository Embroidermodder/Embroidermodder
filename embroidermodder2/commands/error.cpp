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

/* Error is not context-dependant. */
ScriptValue
error_generic(ScriptEnv *context)
{
    if (!argument_checks(context, "debug", "ss")) {
        return script_false;
    }
    QString s = "ERROR: (" + QSTR(0) + ") " + QSTR(1);
    _main->nativeSetPromptPrefix(s);
    _main->nativeAppendPromptHistory(QString());
    _main->nativeEndCommand();
    return script_null;
}

/* NOTE: main() is run every time the command is started.
 *       Use it to reset variables so they are ready to go.
 */
ScriptValue
error_main(ScriptEnv *context)
{
    if (!argument_checks(context, "debug", "ss")) {
        return script_false;
    }
    _main->nativeInitCommand();
    _main->nativeClearSelection();
    return error_generic(context);
}

Command error_cmd = {
    .main = error_main,
    .click = error_generic,
    .context = error_generic,
    .prompt = error_generic,
    .icon = "error",
    .menu_name = "None",
    .menu_position = 0,
    .toolbar_name = "None",
    .toolbar_position = 0,
    .tooltip = "Error",
    .statustip = "Sends an error message to the user. Command: ERROR",
    .alias = "ERROR"
};
