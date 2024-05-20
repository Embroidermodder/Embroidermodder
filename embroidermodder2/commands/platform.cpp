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
platform_command(ScriptEnv * /* context */)
{
    _main->nativeInitCommand();
//    _main->reportPlatform();
    _main->nativeEndCommand();
    return script_null;
}

#if 0
function reportPlatform()
{
    setPromptPrefix(qsTr("Platform") + " = " + platformString());
    appendPromptHistory();
}
[Menu]
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
