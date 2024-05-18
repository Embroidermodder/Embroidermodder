/*
 * Embroidermodder 2.
 *
 * Copyright 2013-2024 The Embroidermodder Team
 * Embroidermodder 2 is Open Source Software, see LICENSE for licensing terms.
 * Read CODE-STANDARDS.txt for advice on altering this file.
 */

#include "../commands.h"

/* DISABLE is a prompt-only Command. */
ScriptValue
disable_prompt(ScriptEnv* context)
{
    if (!argument_checks(context, "disable_prompt", "s")) {
        return script_false;
    }

    QString value = QSTR(0);

    _main->nativeInitCommand();

    if (value == "RAPIDFIRE") {
        _main->disablePromptRapidFire();
    }

    _main->nativeEndCommand();
    return script_null;
}

Command disable_cmd = {
    .id = -1,
    .main = do_nothing,
    .click = do_nothing,
    .context = do_nothing,
    .prompt = disable_prompt,
    .icon = "disable",
    .menu_name = "None",
    .menu_position = 0,
    .toolbar_name = "None",
    .toolbar_position = 0,
    .tooltip = "&Disable",
    .statustip = "Disable one of the pre-defined global boolean variables.",
    .alias = "DISABLE"
};
