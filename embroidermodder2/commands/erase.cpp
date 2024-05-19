/*
 * Embroidermodder 2.
 *
 * Copyright 2013-2024 The Embroidermodder Team
 * Embroidermodder 2 is Open Source Software.
 * See LICENSE for licensing terms.
 *
 * Command: Erase/Delete
 */

#include "../commands.h"

/* Erase is not context-dependant. */
ScriptValue
erase_generic(ScriptEnv * /* context */)
{
    if (_main->nativeNumSelected() <= 0) {
        //TODO: Prompt to select objects if nothing is preselected
        _main->nativeAlert(translate("Preselect objects before invoking the delete command."));
        _main->nativeEndCommand();
        _main->messageBox("information", translate("Delete Preselect"), translate("Preselect objects before invoking the delete command."));
    }
    else {
        _main->nativeDeleteSelected();
        _main->nativeEndCommand();
    }
    _main->nativeEndCommand();
    return script_null;
}

/* NOTE: main() is run every time the command is started.
 *       Use it to reset variables so they are ready to go.
 */
ScriptValue
erase_main(ScriptEnv *context)
{
    _main->nativeInitCommand();
    return erase_generic(context);
}
