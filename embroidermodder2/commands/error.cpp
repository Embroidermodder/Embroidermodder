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
error_command(ScriptEnv *context)
{
    if (!argument_checks(context, "error_command", "ss")) {
        return script_false;
    }
    _main->nativeInitCommand();
    _main->nativeClearSelection();
    QString s = "ERROR: (" + QSTR(0) + ") " + QSTR(1);
    _main->nativeSetPromptPrefix(s);
    _main->nativeAppendPromptHistory(QString());
    _main->nativeEndCommand();
    return script_null;
}

Command error_cmd = {
    .id = -1,
    .main = error_main,
    .icon = "error",
    .menu_name = "None",
    .menu_position = 0,
    .toolbar_name = "None",
    .toolbar_position = 0,
    .tooltip = "Error",
    .statustip = "Sends an error message to the user. Command: ERROR",
    .alias = "ERROR"
};
