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

/* NOTE: main() is run every time the command is started.
 *       Use it to reset variables so they are ready to go.
 */
ScriptValue
erase_main(ScriptEnv * /* context */)
{
    _main->nativeInitCommand();

    if (_main->nativeNumSelected() <= 0) {
        //TODO: Prompt to select objects if nothing is preselected
        _main->nativeAlert(translate("Preselect objects before invoking the delete command."));
        _main->nativeEndCommand();
        _main->nativeMessageBox("information", translate("Delete Preselect"), translate("Preselect objects before invoking the delete command."));
    }
    else {
        _main->nativeDeleteSelected();
        _main->nativeEndCommand();
    }

    return script_null;
}

/* NOTE: click() is run only for left clicks.
 *       Middle clicks are used for panning.
 *       Right clicks bring up the context menu.
 */
ScriptValue
erase_click(ScriptEnv * /* context */)
{
    _main->nativeDeleteSelected();
    _main->nativeEndCommand();
    return script_null;
}

/* NOTE: context() is run when a context menu entry is chosen.
 */
ScriptValue
erase_context(ScriptEnv * /* context */)
{
    _main->nativeDeleteSelected();
    _main->nativeEndCommand();
    return script_null;
}

/* NOTE: prompt() is run when Enter is pressed.
 *       appendPromptHistory is automatically called before prompt()
 *       is called so calling it is only needed for erroneous input.
 *       Any text is in the command prompt is sent as an uppercase string.
 */
ScriptValue
erase_prompt(ScriptEnv * /* context */)
{
    _main->nativeDeleteSelected();
    _main->nativeEndCommand();
    return script_null;
}

Command erase_cmd = {
    .main = erase_main,
    .click = erase_click,
    .context = erase_context,
    .prompt = erase_prompt,
    .icon = "erase",
    .menu_name = "Modify",
    .menu_position = 5,
    .toolbar_name = "Modify",
    .toolbar_position = 0,
    .tooltip = "D&elete",
    .statustip = "Removes objects from a drawing:  DELETE",
    .alias = "E, ERASE, DEL, DELETE"
};
