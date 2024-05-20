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

/* NOTE: main() is run every time the command is started.
 *       Use it to reset variables so they are ready to go.
 */
ScriptValue
arc_command(ScriptEnv *context)
{
    _main->nativeInitCommand();
    _main->nativeClearSelection();
    _main->about();
    _main->nativeEndCommand();
    return script_null;
}

/* NOTE: click() is run only for left clicks.
 *       Middle clicks are used for panning.
 *       Right clicks bring up the context menu.
 */
ScriptValue
about_click(ScriptEnv *context)
{
    _main->about();
    _main->nativeEndCommand();
    return script_null;
}

/* NOTE: context() is run when a context menu entry is chosen.
 */
ScriptValue
about_context(ScriptEnv *context)
{
    _main->about();
    _main->nativeEndCommand();
    return script_null;
}

/* NOTE: prompt() is run when Enter is pressed.
 *       appendPromptHistory is automatically called before prompt()
 *       is called so calling it is only needed for erroneous input.
 *       Any text is in the command prompt is sent as an uppercase string.
 */
ScriptValue
about_prompt(ScriptEnv *context)
{
    _main->about();
    _main->nativeEndCommand();
    return script_null;
}

Command about_cmd = {
    .main = about_main,
    .click = about_click,
    .context = about_context,
    .prompt = about_prompt,
    .icon = "about",
    .menu_name = "None",
    .menu_position = 0,
    .toolbar_name = "None",
    .toolbar_position = 0,
    .tooltip = "&About",
    .statustip = "Displays information about this product:  ABOUT",
    .alias = "ABOUT"
};
