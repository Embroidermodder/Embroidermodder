/*
 * Embroidermodder 2.
 *
 * Copyright 2013-2024 The Embroidermodder Team
 * Embroidermodder 2 is Open Source Software, see LICENSE for licensing terms.
 * Read CODE-STANDARDS.txt for advice on altering this file.
 *
 * Command: REDO
 */

#include "../commands.h"

/* NOTE: main() is run every time the command is started.
 *       Use it to reset variables so they are ready to go.
 */
ScriptValue
redo_main(ScriptEnv * context)
{
    if (!argument_checks(context, "redo_main", "")) {
        return script_false;
    }
    _main->nativeInitCommand();
    _main->nativeClearSelection();
    _main->redo();
    _main->nativeEndCommand();
    return script_null;
}

/* NOTE: click() is run only for left clicks.
 *       Middle clicks are used for panning.
 *       Right clicks bring up the context menu.
 */
ScriptValue
redo_click(ScriptEnv * context)
{
    if (!argument_checks(context, "redo_click", "")) {
        return script_false;
    }
    _main->redo();
    _main->nativeEndCommand();
    return script_null;
}

/* NOTE: context() is run when a context menu entry is chosen.
 */
ScriptValue
redo_context(ScriptEnv * context)
{
    if (!argument_checks(context, "redo_context", "")) {
        return script_false;
    }
    _main->redo();
    _main->nativeEndCommand();
    return script_null;
}

/* NOTE: prompt() is run when Enter is pressed.
 *       appendPromptHistory is automatically called before prompt()
 *       is called so calling it is only needed for erroneous input.
 *       Any text is in the command prompt is sent as an uppercase string.
 */
ScriptValue
redo_prompt(ScriptEnv * context)
{
    if (!argument_checks(context, "redo_prompt", "")) {
        return script_false;
    }
    _main->redo();
    _main->nativeEndCommand();
    return script_null;
}
