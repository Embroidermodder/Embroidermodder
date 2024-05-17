/*
 * Embroidermodder 2.
 *
 * Copyright 2013-2024 The Embroidermodder Team
 * Embroidermodder 2 is Open Source Software.
 * See LICENSE for licensing terms.
 *
 * Command: icon24
 */

#include "../commands.h"

/* NOTE: main() is run every time the command is started.
 *       Use it to reset variables so they are ready to go.
 */
ScriptValue
windowprevious_main(ScriptEnv * context)
{
    return script_null;
}

/* NOTE: click() is run only for left clicks.
 *       Middle clicks are used for panning.
 *       Right clicks bring up the context menu.
 */
ScriptValue
windowprevious_click(ScriptEnv * context)
{
    return script_null;
}

/* NOTE: context() is run when a context menu entry is chosen.
 */
ScriptValue
windowprevious_context(ScriptEnv * context)
{
    return script_null;
}

/* NOTE: prompt() is run when Enter is pressed.
 *       appendPromptHistory is automatically called before prompt()
 *       is called so calling it is only needed for erroneous input.
 *       Any text is in the command prompt is sent as an uppercase string.
 */
ScriptValue
windowprevious_prompt(ScriptEnv * context)
{
    return script_null;
}

Command windowprevious_cmd = {
    .id = -1,
    .main = windowprevious_main,
    .click = windowprevious_click,
    .context = windowprevious_context,
    .prompt = windowprevious_prompt,
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
//Command: WindowPrevious

//NOTE: main() is run every time the command is started.
//      Use it to reset variables so they are ready to go.
function main()
{
    initCommand();
    clearSelection();
    windowPrevious();
    endCommand();
}

//NOTE: click() is run only for left clicks.
//      Middle clicks are used for panning.
//      Right clicks bring up the context menu.
function click(x, y)
{
    windowPrevious();
    endCommand();
}

//NOTE: context() is run when a context menu entry is chosen.
function context(str)
{
    windowPrevious();
    endCommand();
}

//NOTE: prompt() is run when Enter is pressed.
//      appendPromptHistory is automatically called before prompt()
//      is called so calling it is only needed for erroneous input.
//      Any text is in the command prompt is sent as an uppercase string.
function prompt(str)
{
    windowPrevious();
    endCommand();
}
[Menu]
Name=None
Position=0

[ToolBar]
Name=None
Position=0

[Tips]
ToolTip=Pre&vious
StatusTip=Move the focus to the previous window:  PREVIOUS

[Prompt]
Alias=PREV, PREVIOUS, WINDOWPREVIOUS
#endif
