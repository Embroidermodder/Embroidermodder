/*
 * Embroidermodder 2.
 *
 * Copyright 2013-2024 The Embroidermodder Team
 * Embroidermodder 2 is Open Source Software.
 * See LICENSE for licensing terms.
 *
 * Command: HELP
 */

#include "../commands.h"

/* HELP is not context-dependent. */
ScriptValue
help_command(ScriptEnv * /* context */)
{
    _main->nativeInitCommand();
    _main->nativeClearSelection();
    _main->nativeHelp();
    _main->nativeEndCommand();
    return script_null;
}
