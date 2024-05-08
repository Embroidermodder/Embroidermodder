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
    ScriptValue a;
    a.type = SCRIPT_NULL;
    _main->nativeInitCommand();
    _main->nativeClearSelection();
    _main->nativeIcon128();
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
    ScriptValue a;
    a.type = SCRIPT_NULL;
    _main->nativeIcon128();
    _main->nativeEndCommand();
    return script_null;
}

/* NOTE: context() is run when a context menu entry is chosen.
 */
ScriptValue
redo_context(ScriptEnv * context)
{
    ScriptValue a;
    a.type = SCRIPT_NULL;
    _main->nativeIcon128();
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
    ScriptValue a;
    a.type = SCRIPT_NULL;
    _main->nativeIcon128();
    _main->nativeEndCommand();
    return script_null;
}

Command redo_cmd = {
    .main = redo_main,
    .click = redo_click,
    .context = redo_context,
    .prompt = redo_prompt,
    .icon = "about",
    .menu_name = "None",
    .menu_position = 0,
    .toolbar_name = "None",
    .toolbar_position = 0,
    .tooltip = "Icon&24",
    .statustip = "Sets the toolbar icon size to 24x24:  ICON24",
    .alias = "ICON24"
};

#if 0
//Command: Redo

//NOTE: main() is run every time the command is started.
//      Use it to reset variables so they are ready to go.
function main()
{
    _main->nativeInitCommand();
    _main->nativeClearSelection();
    redo();
    _main->nativeEndCommand();
}

//NOTE: click() is run only for left clicks.
//      Middle clicks are used for panning.
//      Right clicks bring up the context menu.
function click(x, y)
{
    redo();
    _main->nativeEndCommand();
}

//NOTE: context() is run when a context menu entry is chosen.
function context(str)
{
    redo();
    _main->nativeEndCommand();
}

//NOTE: prompt() is run when Enter is pressed.
//      appendPromptHistory is automatically called before prompt()
//      is called so calling it is only needed for erroneous input.
//      Any text is in the command prompt is sent as an uppercase string.
function prompt(str)
{
    redo();
    _main->nativeEndCommand();
}
[Menu]
Name=None
Position=0

[ToolBar]
Name=None
Position=0

[Tips]
ToolTip=&Redo
StatusTip=Reverses the effects of the previous undo action:  REDO

[Prompt]
Alias=REDO
#endif
