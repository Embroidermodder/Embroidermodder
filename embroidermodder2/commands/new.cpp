/*
 * Embroidermodder 2.
 *
 * Copyright 2013-2024 The Embroidermodder Team
 * Embroidermodder 2 is Open Source Software, see LICENSE for licensing terms.
 * Read CODE-STANDARDS.txt for advice on altering this file.
 *
 * Command: icon24
 */

#include "../commands.h"


/* NOTE: main() is run every time the command is started.
 *       Use it to reset variables so they are ready to go.
 */
ScriptValue
new_main(ScriptEnv * context)
{
    _main->nativeInitCommand();
    _main->nativeClearSelection();
    _main->nativeNewFile();
    _main->nativeEndCommand();
    return script_null;
}

/* NOTE: click() is run only for left clicks.
 *       Middle clicks are used for panning.
 *       Right clicks bring up the context menu.
 */
ScriptValue
new_click(ScriptEnv * context)
{
    _main->nativeNewFile();
    _main->nativeEndCommand();
    return script_null;
}

/* NOTE: context() is run when a context menu entry is chosen.
 */
ScriptValue
new_context(ScriptEnv * context)
{
    _main->nativeNewFile();
    _main->nativeEndCommand();
    return script_null;
}

/* NOTE: prompt() is run when Enter is pressed.
 *       appendPromptHistory is automatically called before prompt()
 *       is called so calling it is only needed for erroneous input.
 *       Any text is in the command prompt is sent as an uppercase string.
 */
ScriptValue
new_prompt(ScriptEnv * context)
{
    _main->nativeNewFile();
    _main->nativeEndCommand();
    return script_null;

}

Command new_cmd = {
    .id = -1,
    .main = new_main,
    .click = new_click,
    .context = new_context,
    .prompt = new_prompt,
    .icon = "new",
    .menu_name = "None",
    .menu_position = 0,
    .toolbar_name = "None",
    .toolbar_position = 0,
    .tooltip = "&New",
    .statustip = "Create a new file. Command: NEW.",
    .alias = "NEW"
};
