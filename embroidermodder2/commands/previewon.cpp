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
previewon_main(ScriptEnv *context)
{
    if (!argument_checks(context, "textBold", "ssrrr")) {
        return script_false;
    }

    QString cloneStr = QSTR(0).toUpper();
    int clone = PREVIEW_CLONE_NULL;
    if (cloneStr == "SELECTED") {
        clone = PREVIEW_CLONE_SELECTED;
    }
    else if (cloneStr == "RUBBER") {
        clone = PREVIEW_CLONE_RUBBER;
    }
    else {
        debug_message("UNKNOWN_ERROR previewOn(): first argument must be \"SELECTED\" or \"RUBBER\".");
        return script_false;
    }

    QString modeStr  = QSTR(1).toUpper();
    int mode = PREVIEW_MODE_NULL;
    if (modeStr == "MOVE") {
        mode = PREVIEW_MODE_MOVE;
    }
    else if (modeStr == "ROTATE") {
        mode = PREVIEW_MODE_ROTATE;
    }
    else if (modeStr == "SCALE") {
        mode = PREVIEW_MODE_SCALE;
    }
    else {
        debug_message("UNKNOWN_ERROR previewOn(): second argument must be \"MOVE\", \"ROTATE\" or \"SCALE\".");
        return script_false;
    }

    _main->nativePreviewOn(clone, mode, REAL(2), REAL(3), REAL(4));
    _main->nativeEndCommand();
    return script_null;
}

/* NOTE: click() is run only for left clicks.
 *       Middle clicks are used for panning.
 *       Right clicks bring up the context menu.
 */
ScriptValue
previewon_click(ScriptEnv *context)
{
    _main->nativeEndCommand();
    return script_null;
}

/* NOTE: context() is run when a context menu entry is chosen.
 */
ScriptValue
previewon_context(ScriptEnv *context)
{
    _main->nativeEndCommand();
    return script_null;
}

/* NOTE: prompt() is run when Enter is pressed.
 *       appendPromptHistory is automatically called before prompt()
 *       is called so calling it is only needed for erroneous input.
 *       Any text is in the command prompt is sent as an uppercase string.
 */
ScriptValue
previewon_prompt(ScriptEnv *context)
{
    _main->nativeEndCommand();
    return script_null;
}

Command previewon_cmd = {
    .id = -1,
    .main = previewon_main,
    .click = previewon_click,
    .context = previewon_context,
    .prompt = previewon_prompt,
    .icon = "donothing",
    .menu_name = "None",
    .menu_position = 0,
    .toolbar_name = "None",
    .toolbar_position = 0,
    .tooltip = "&Do Nothing",
    .statustip = "Does Nothing.",
    .alias = "DONOTHING"
};
