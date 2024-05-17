/*
 * Embroidermodder 2.
 *
 * Copyright 2013-2024 The Embroidermodder Team
 * Embroidermodder 2 is Open Source Software, see LICENSE for licensing terms.
 * Read CODE-STANDARDS.txt for advice on altering this file.
 *
 * Command: UNDO
 */

#include "../commands.h"

/* NOTE: main() is run every time the command is started.
 *       Use it to reset variables so they are ready to go.
 */
ScriptValue
set_main(ScriptEnv* context)
{
    _main->nativeInitCommand();
    _main->nativeClearSelection();
    _main->undo();
    _main->nativeEndCommand();
    return script_null;
}

/* NOTE: prompt() is run when Enter is pressed.
 *       appendPromptHistory is automatically called before prompt()
 *       is called so calling it is only needed for erroneous input.
 *       Any text is in the command prompt is sent as an uppercase string.
 */
ScriptValue
set_prompt(ScriptEnv* context)
{
    _main->undo();
    _main->nativeEndCommand();
    return script_null;
}

Command set_cmd = {
    .id = -1,
    .main = set_main,
    .click = do_nothing,
    .context = do_nothing,
    .prompt = set_prompt,
    .icon = "undo",
    .menu_name = "None",
    .menu_position = 0,
    .toolbar_name = "None",
    .toolbar_position = 0,
    .tooltip = "&Undo",
    .statustip = "Reverses the most recent action:  UNDO",
    .alias = "U, UNDO"
};
