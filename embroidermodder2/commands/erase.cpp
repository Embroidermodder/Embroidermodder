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

Command erase_cmd = {
    .id = -1,
    .main = erase_main,
    .click = erase_generic,
    .context = erase_generic,
    .prompt = erase_generic,
    .icon = "erase",
    .menu_name = "Modify",
    .menu_position = 5,
    .toolbar_name = "Modify",
    .toolbar_position = 0,
    .tooltip = "D&elete",
    .statustip = "Removes objects from a drawing:  DELETE",
    .alias = "E, ERASE, DEL, DELETE"
};
