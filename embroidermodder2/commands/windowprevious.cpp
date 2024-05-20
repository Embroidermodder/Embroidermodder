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
windowprevious_command(ScriptEnv * context)
{
    _main->mdiArea->activatePreviousSubWindow();
    _main->nativeEndCommand();
    return script_null;
}

#if 0
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
