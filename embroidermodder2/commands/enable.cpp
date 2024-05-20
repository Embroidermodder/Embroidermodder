/*
 * Embroidermodder 2.
 *
 * Copyright 2013-2024 The Embroidermodder Team
 * Embroidermodder 2 is Open Source Software, see LICENSE for licensing terms.
 * Read CODE-STANDARDS.txt for advice on altering this file.
 */

#include "../commands.h"

/* ENABLE is a prompt-only Command. */
ScriptValue
enable_prompt(ScriptEnv* context)
{
    if (context->argumentCount != 2) {
        return script_false;
    }

    QString value = QSTR(0);

    _main->nativeInitCommand();

    if (value == "RAPIDFIRE") {
        _main->enablePromptRapidFire();
    }

    _main->nativeEndCommand();
    return script_null;
}

Command enable_cmd = {
    .id = -1,
    .main = enable_prompt,
    .icon = "enable",
    .menu_name = "None",
    .menu_position = 0,
    .toolbar_name = "None",
    .toolbar_position = 0,
    .tooltip = "&Enable",
    .statustip = "Enable a pre-defined global variables.",
    .alias = "ENABLE"
};
