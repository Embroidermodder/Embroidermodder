/*
 * Embroidermodder 2.
 *
 * Copyright 2013-2024 The Embroidermodder Team
 * Embroidermodder 2 is Open Source Software, see LICENSE for licensing terms.
 * Read CODE-STANDARDS.txt for advice on altering this file.
 */

#include "../commands.h"

/* SET is a prompt-only Command.
 *
 * We can't use the argument_checks function because the 2nd argument is a wildcard.
 */
ScriptValue
set_command(ScriptEnv* context)
{
    if (context->argumentCount != 2) {
        return script_false;
    }

    QString value = QSTR(0);

    _main->nativeInitCommand();

    if (value == "TEXTANGLE") {
        if (context->argument[1].type != SCRIPT_REAL) {
            return script_false;
        }
        _main->setSettingsTextAngle(REAL(1));
    }
    else if (value == "TEXTBOLD") {
        if (context->argument[1].type != SCRIPT_BOOL) {
            return script_false;
        }
        _main->setSettingsTextStyleBold(BOOL(1));
    }
    else if (value == "TEXTITALIC") {
        if (context->argument[1].type != SCRIPT_BOOL) {
            return script_false;
        }
        _main->setSettingsTextStyleItalic(BOOL(1));
    }
    else if (value == "TEXTFONT") {
        if (context->argument[1].type != SCRIPT_STRING) {
            return script_false;
        }
        _main->setSettingsTextFont(context->argument[1].s);
    }
    else if (value == "TEXTOVERLINE") {
        if (context->argument[1].type != SCRIPT_BOOL) {
            return script_false;
        }
        _main->setSettingsTextStyleOverline(BOOL(1));
    }
    else if (value == "TEXTSIZE") {
        if (context->argument[1].type != SCRIPT_REAL) {
            return script_false;
        }
        _main->setSettingsTextSize(REAL(1));
    }
    else if (value == "TEXTSTRIKEOUT") {
        if (context->argument[1].type != SCRIPT_BOOL) {
            return script_false;
        }
        _main->setSettingsTextStyleStrikeOut(BOOL(1));
    }
    else if (value == "TEXTUNDERLINE") {
        if (context->argument[1].type != SCRIPT_BOOL) {

            return script_false;
        }
        _main->setSettingsTextStyleUnderline(BOOL(1));
    }

    _main->nativeEndCommand();
    return script_null;
}
