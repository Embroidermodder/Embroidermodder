/*
 * Embroidermodder 2.
 *
 * Copyright 2013-2024 The Embroidermodder Team
 * Embroidermodder 2 is Open Source Software.
 * See LICENSE for licensing terms.
 *
 * Command: WINDOWNEXT
 */

#include "../commands.h"

/* NOTE: main() is run every time the command is started.
 *       Use it to reset variables so they are ready to go.
 */
ScriptValue
windownext_command(ScriptEnv * context)
{
    _main->mdiArea->activateNextSubWindow();
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
ToolTip=Ne&xt
StatusTip=Move the focus to the next window:  NEXT

[Prompt]
Alias=NEXT, WINDOWNEXT
#endif
