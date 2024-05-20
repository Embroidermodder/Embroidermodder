/*
 * Embroidermodder 2.
 *
 * Copyright 2013-2024 The Embroidermodder Team
 * Embroidermodder 2 is Open Source Software.
 * See LICENSE for licensing terms.
 *
 * Command: about
 */

#include "../commands.h"

/* NOTE: main() is run every time the command is started.
 *       Use it to reset variables so they are ready to go.
 */
ScriptValue
previewoff_command(ScriptEnv *context)
{
    if (!argument_checks(context, "PreviewOff", "")) {
        return script_false;
    }

    _main->nativePreviewOff();
    _main->nativeEndCommand();
    return script_null;
}
