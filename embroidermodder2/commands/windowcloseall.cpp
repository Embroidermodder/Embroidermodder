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
windowcloseall_command(ScriptEnv * context)
{
    _main->mdiArea->closeAllSubWindows();
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
ToolTip=Cl&ose
StatusTip=Close the active window:  CLOSE

[Prompt]
Alias=CLOSE, WINDOWCLOSE
#endif
