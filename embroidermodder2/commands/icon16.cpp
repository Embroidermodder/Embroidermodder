/*
 * Embroidermodder 2.
 *
 * Copyright 2013-2024 The Embroidermodder Team
 * Embroidermodder 2 is Open Source Software, see LICENSE for licensing terms.
 * Read CODE-STANDARDS.txt for advice on altering this file.
 *
 * Command: ICON16
 */

#include "../commands.h"

/* ICON16 is not context-dependant */
ScriptValue
icon16_generic(ScriptEnv * /* context */)
{
    _main->icon16();
    _main->nativeEndCommand();
    return script_null;
}

/* NOTE: main() is run every time the command is started.
 *       Use it to reset variables so they are ready to go.
 */
ScriptValue
icon16_main(ScriptEnv *context)
{
    _main->nativeInitCommand();
    _main->nativeClearSelection();
    return icon16_generic(context);
}

Command icon16_cmd = {
    .main = icon16_main,
    .click = icon16_generic,
    .context = icon16_generic,
    .prompt = icon16_generic,
    .icon = "icon16",
    .menu_name = "None",
    .menu_position = 0,
    .toolbar_name = "None",
    .toolbar_position = 0,
    .tooltip = "Icon&16",
    .statustip = "Sets the toolbar icon size to 16x16:  ICON16",
    .alias = "ICON16"
};
