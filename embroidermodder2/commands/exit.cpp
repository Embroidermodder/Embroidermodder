/*
 * Embroidermodder 2.
 *
 * Copyright 2013-2024 The Embroidermodder Team
 * Embroidermodder 2 is Open Source Software.
 * See LICENSE for licensing terms.
 *
 * Command: exit
 */

#include "../commands.h"

/* NOTE: main() is run every time the command is started.
 *       Use it to reset variables so they are ready to go.
 */
ScriptValue
exit_command(ScriptEnv * /* context */)
{
    _main->nativeInitCommand();
    _main->nativeClearSelection();
    _main->nativeExit();
    _main->nativeEndCommand();
    return script_null;
}
