/*
 * Embroidermodder 2.
 *
 * Copyright 2013-2024 The Embroidermodder Team
 * Embroidermodder 2 is Open Source Software, see LICENSE for licensing terms.
 * Read CODE-STANDARDS.txt for advice on altering this file.
 *
 * Command: TIPOFTHEDAY
 */

#include "../commands.h"

/* NOTE: main() is run every time the command is started.
 *       Use it to reset variables so they are ready to go.
 */
ScriptValue
tipoftheday_main(ScriptEnv * context)
{
    return script_null;
}

/* NOTE: click() is run only for left clicks.
 *       Middle clicks are used for panning.
 *       Right clicks bring up the context menu.
 */
ScriptValue
tipoftheday_click(ScriptEnv * context)
{
    return script_null;
}

/* NOTE: context() is run when a context menu entry is chosen.
 */
ScriptValue
tipoftheday_context(ScriptEnv * context)
{
    return script_null;
}

/* NOTE: prompt() is run when Enter is pressed.
 *       appendPromptHistory is automatically called before prompt()
 *       is called so calling it is only needed for erroneous input.
 *       Any text is in the command prompt is sent as an uppercase string.
 */
ScriptValue
tipoftheday_prompt(ScriptEnv * context)
{
    return script_null;
}


#if 0
//Command: TipOfTheDay

//NOTE: main() is run every time the command is started.
//      Use it to reset variables so they are ready to go.
function main()
{
    initCommand();
    clearSelection();
    tipOfTheDay();
    endCommand();
}

//NOTE: click() is run only for left clicks.
//      Middle clicks are used for panning.
//      Right clicks bring up the context menu.
function click(x, y)
{
    tipOfTheDay();
    endCommand();
}

//NOTE: context() is run when a context menu entry is chosen.
function context(str)
{
    tipOfTheDay();
    endCommand();
}

//NOTE: prompt() is run when Enter is pressed.
//      appendPromptHistory is automatically called before prompt()
//      is called so calling it is only needed for erroneous input.
//      Any text is in the command prompt is sent as an uppercase string.
function prompt(str)
{
    tipOfTheDay();
    endCommand();
}
[Menu]
Name=None
Position=0

[ToolBar]
Name=None
Position=0

[Tips]
ToolTip=&Tip Of The Day
StatusTip=Displays a dialog with useful tips:  TIPS

[Prompt]
Alias=TIPS, TIPOFTHEDAY
#endif
