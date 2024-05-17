/*
 * Embroidermodder 2.
 *
 * Copyright 2013-2024 The Embroidermodder Team
 * Embroidermodder 2 is Open Source Software, see LICENSE for licensing terms.
 * Read CODE-STANDARDS.txt for advice on altering this file.
 *
 * Command: PANDOWN
 */

#include "../commands.h"

/* PANDOWN is context-independant. */
ScriptValue
pandown_generic(ScriptEnv * context)
{
    _main->panDown();
    _main->nativeEndCommand();
    return script_null;
}

/* NOTE: main() is run every time the command is started.
 *       Use it to reset variables so they are ready to go.
 */
ScriptValue
pandown_main(ScriptEnv * context)
{
    _main->nativeInitCommand();
    _main->nativeClearSelection();
    return pandown_generic(context);
}

Command pandown_cmd = {
    .id = -1,
    .main = pandown_main,
    .click = pandown_generic,
    .context = pandown_generic,
    .prompt = pandown_generic,
    .icon = "pandown",
    .menu_name = "None",
    .menu_position = 0,
    .toolbar_name = "None",
    .toolbar_position = 0,
    .tooltip = "&Pan Down",
    .statustip = "Moves the view down:  PANDOWN",
    .alias = "PANDOWN"
};
