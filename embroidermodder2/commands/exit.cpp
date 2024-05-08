/*
 * Embroidermodder 2.
 *
 * Copyright 2013-2024 The Embroidermodder Team
 * Embroidermodder 2 is Open Source Software.
 * See LICENSE for licensing terms.
 *
 * Command: exit
 */

#include "../commands.h"

/* NOTE: main() is run every time the command is started.
 *       Use it to reset variables so they are ready to go.
 */
ScriptValue
exit_main(ScriptEnv * /* context */)
{
    _main->nativeInitCommand();
    _main->nativeClearSelection();
    _main->nativeExit();
    _main->nativeEndCommand();
    return script_null;
}

/* NOTE: click() is run only for left clicks.
 *       Middle clicks are used for panning.
 *       Right clicks bring up the context menu.
 */
ScriptValue
exit_click(ScriptEnv * /* context */)
{
    _main->nativeExit();
    _main->nativeEndCommand();
    return script_null;
}

/* NOTE: context() is run when a context menu entry is chosen.
 */
ScriptValue
exit_context(ScriptEnv * /* context */)
{
    _main->nativeExit();
    _main->nativeEndCommand();
    return script_null;
}

/* NOTE: prompt() is run when Enter is pressed.
 *       appendPromptHistory is automatically called before prompt()
 *       is called so calling it is only needed for erroneous input.
 *       Any text is in the command prompt is sent as an uppercase string.
 */
ScriptValue
exit_prompt(ScriptEnv * /* context */)
{
    _main->nativeExit();
    _main->nativeEndCommand();
    return script_null;
}

Command exit_cmd = {
    .main = exit_main,
    .click = exit_click,
    .context = exit_context,
    .prompt = exit_prompt,
    .icon = "exit",
    .menu_name = "None",
    .menu_position = 0,
    .toolbar_name = "None",
    .toolbar_position = 0,
    .tooltip = "E&xit",
    .statustip = "Exit the application:  EXIT",
    .alias = "EXIT, QUIT"
};
