/*
 * Embroidermodder 2.
 *
 * Copyright 2013-2024 The Embroidermodder Team
 * Embroidermodder 2 is Open Source Software.
 * See LICENSE for licensing terms.
 *
 * Command: WINDOWCLOSEALL
 */

#include "../commands.h"

/* NOTE: main() is run every time the command is started.
 *       Use it to reset variables so they are ready to go.
 */
ScriptValue
windowcloseall_main(ScriptEnv * context)
{
    return script_null;
}

/* NOTE: click() is run only for left clicks.
 *       Middle clicks are used for panning.
 *       Right clicks bring up the context menu.
 */
ScriptValue
windowcloseall_click(ScriptEnv * context)
{
    return script_null;
}

/* NOTE: context() is run when a context menu entry is chosen.
 */
ScriptValue
windowcloseall_context(ScriptEnv * context)
{
    return script_null;
}

/* NOTE: prompt() is run when Enter is pressed.
 *       appendPromptHistory is automatically called before prompt()
 *       is called so calling it is only needed for erroneous input.
 *       Any text is in the command prompt is sent as an uppercase string.
 */
ScriptValue
windowcloseall_prompt(ScriptEnv * context)
{
    return script_null;
}

#if 0
//Command: WindowClose

//NOTE: main() is run every time the command is started.
//      Use it to reset variables so they are ready to go.
function main()
{
    initCommand();
    clearSelection();
    windowClose();
    endCommand();
}

//NOTE: click() is run only for left clicks.
//      Middle clicks are used for panning.
//      Right clicks bring up the context menu.
function click(x, y)
{
    windowClose();
    endCommand();
}

//NOTE: context() is run when a context menu entry is chosen.
function context(str)
{
    windowClose();
    endCommand();
}

//NOTE: prompt() is run when Enter is pressed.
//      appendPromptHistory is automatically called before prompt()
//      is called so calling it is only needed for erroneous input.
//      Any text is in the command prompt is sent as an uppercase string.
function prompt(str)
{
    windowClose();
    endCommand();
}
[Menu]
Name=None
Position=0

[ToolBar]
Name=None
Position=0

[Tips]
ToolTip=Cl&ose
StatusTip=Close the active window:  CLOSE

[Prompt]
Alias=CLOSE, WINDOWCLOSE
#endif
