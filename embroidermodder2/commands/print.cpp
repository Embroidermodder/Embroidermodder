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
print_main(ScriptEnv *context)
{
    _main->nativeInitCommand();
    _main->nativeClearSelection();

    _main->nativeEndCommand();
    return script_null;
}

/* NOTE: click() is run only for left clicks.
 *       Middle clicks are used for panning.
 *       Right clicks bring up the context menu.
 */
ScriptValue
print_click(ScriptEnv *context)
{

    _main->nativeEndCommand();
    return script_null;
}

/* NOTE: context() is run when a context menu entry is chosen.
 */
ScriptValue
print_context(ScriptEnv *context)
{

    _main->nativeEndCommand();
    return script_null;
}

/* NOTE: prompt() is run when Enter is pressed.
 *       appendPromptHistory is automatically called before prompt()
 *       is called so calling it is only needed for erroneous input.
 *       Any text is in the command prompt is sent as an uppercase string.
 */
ScriptValue
print_prompt(ScriptEnv *context)
{

    _main->nativeEndCommand();
    return script_null;
}
