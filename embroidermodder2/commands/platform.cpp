/*
 * Embroidermodder 2.
 *
 * Copyright 2013-2024 The Embroidermodder Team
 * Embroidermodder 2 is Open Source Software, see LICENSE for licensing terms.
 * Read CODE-STANDARDS.txt for advice on altering this file.
 *
 * Command: PLATFORM
 */

#include "../commands.h"

/* NOTE: main() is run every time the command is started.
 *       Use it to reset variables so they are ready to go.
 */
ScriptValue
platform_main(ScriptEnv * /* context */)
{
    return script_null;
}

/* NOTE: click() is run only for left clicks.
 *       Middle clicks are used for panning.
 *       Right clicks bring up the context menu.
 */
ScriptValue
platform_click(ScriptEnv * /* context */)
{
    return script_null;
}

/* NOTE: context() is run when a context menu entry is chosen.
 */
ScriptValue
platform_context(ScriptEnv * /* context */)
{
    return script_null;
}

/* NOTE: prompt() is run when Enter is pressed.
 *       appendPromptHistory is automatically called before prompt()
 *       is called so calling it is only needed for erroneous input.
 *       Any text is in the command prompt is sent as an uppercase string.
 */
ScriptValue
platform_prompt(ScriptEnv * /* context */)
{
    return script_null;
}

#if 0
//Command: Platform

//NOTE: main() is run every time the command is started.
//      Use it to reset variables so they are ready to go.
function main()
{
    initCommand();
    clearSelection();
    reportPlatform();
    endCommand();
}

//NOTE: click() is run only for left clicks.
//      Middle clicks are used for panning.
//      Right clicks bring up the context menu.
function click(x, y)
{
    reportPlatform();
    endCommand();
}

//NOTE: context() is run when a context menu entry is chosen.
function context(str)
{
    reportPlatform();
    endCommand();
}

//NOTE: prompt() is run when Enter is pressed.
//      appendPromptHistory is automatically called before prompt()
//      is called so calling it is only needed for erroneous input.
//      Any text is in the command prompt is sent as an uppercase string.
function prompt(str)
{
    reportPlatform();
    endCommand();
}

function reportPlatform()
{
    setPromptPrefix(qsTr("Platform") + " = " + platformString());
    appendPromptHistory();
}[Menu]
Name=None
Position=0

[ToolBar]
Name=None
Position=0

[Tips]
ToolTip=&Platform
StatusTip=List which platform is in use:  PLATFORM

[Prompt]
Alias=PLATFORM
#endif
