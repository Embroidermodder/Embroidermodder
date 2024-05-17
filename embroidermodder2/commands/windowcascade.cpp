/*
 * Embroidermodder 2.
 *
 * Copyright 2013-2024 The Embroidermodder Team
 * Embroidermodder 2 is Open Source Software, see LICENSE for licensing terms.
 * Read CODE-STANDARDS.txt for advice on altering this file.
 *
 * Command: WINDOWCASCADE
 */

#include "../commands.h"

/* NOTE: main() is run every time the command is started.
 *       Use it to reset variables so they are ready to go.
 */
ScriptValue
windowcascade_main(ScriptEnv * context)
{
    _main->nativeInitCommand();
    _main->nativeClearSelection();
    _main->mdiArea->cascade();
    _main->nativeEndCommand();
    return script_null;
}

/* NOTE: click() is run only for left clicks.
 *       Middle clicks are used for panning.
 *       Right clicks bring up the context menu.
 */
ScriptValue
windowcascade_click(ScriptEnv * context)
{
    _main->mdiArea->cascade();
    _main->nativeEndCommand();
    return script_null;
}

/* NOTE: context() is run when a context menu entry is chosen.
 */
ScriptValue
windowcascade_context(ScriptEnv * context)
{
    _main->mdiArea->cascade();
    _main->nativeEndCommand();
    return script_null;
}

/* NOTE: prompt() is run when Enter is pressed.
 *       appendPromptHistory is automatically called before prompt()
 *       is called so calling it is only needed for erroneous input.
 *       Any text is in the command prompt is sent as an uppercase string.
 */
ScriptValue
windowcascade_prompt(ScriptEnv * context)
{
    _main->mdiArea->cascade();
    _main->nativeEndCommand();
    return script_null;
}

Command windowcascade_cmd = {
    .id = -1,
    .main = windowcascade_main,
    .click = windowcascade_click,
    .context = windowcascade_context,
    .prompt = windowcascade_prompt,
    .icon = "windowcascade",
    .menu_name = "None",
    .menu_position = 0,
    .toolbar_name = "None",
    .toolbar_position = 0,
    .tooltip = "&Cascade",
    .statustip = "Cascade the windows:  CASCADE",
    .alias = "CASCADE, WINDOWCASCADE"
};
