/*! \file script.c
 * \brief C Core of all non-scripted commands.
 */

#include "script.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

#define STR(index) context->arguments[index].s
#define REAL(index) context->arguments[index].r
#define INT(index) context->arguments[index].i
#define BOOL(index) context->arguments[index].b

int
argument_checks(ScriptContext* context, const char *name, const char *args)
{
    char error_msg[1000];
    if (context->argument_count != strlen(args)) {
        sprintf(error_msg, "%s() requires %d arguments", name,
            (int)strlen(args));
        throwError(GeneralError, error_msg);
        return 0;
    }
    for (int i=0; args[i]; i++) {
        switch (args[i]) {
        case 's': {
            if (!is_string(context->arguments[i])) {
                sprintf(error_msg, "%s(): argument %d is not a string",
                    name, i);
                throwError(TypeError, error_msg);
                return 0;
            }
            break;
        }
        case 'i': {
            if (!is_int(context->arguments[i])) {
                sprintf(error_msg, "%s(): argument %d is not an integer",
                    name, i);
                throwError(TypeError, error_msg);
                return 0;
            }
            break;
        }
        case 'f': {
            if (!is_real(context->arguments[i])) {
                sprintf(error_msg, "%s(): argument %d is not a real number",
                    name, i);
                throwError(TypeError, error_msg);
                return 0;
            }
            if (isnan(context->arguments[i].r)) {
                sprintf(error_msg, "%s(): argument %d has the value NaN",
                    name, i);
                throwError(TypeError, error_msg);
                return 0;
            }
            break;
        }
        case 'b': {
            if (!is_bool(context->arguments[i])) {
                sprintf(error_msg, "%s(): argument %d is not a boolean value",
                    name, i);
                throwError(TypeError, error_msg);
                return 0;
            }
            break;
        }
        default:
            break;
        }
    }
    return 1;
}

/*!
 * \brief script_null
 */
ScriptValue script_null = {
    .s = "null",
    .i = 0,
    .r = 0.0,
    .b = false,
    .type = '0'
};

/*!
 * \brief script_true
 */
ScriptValue script_true = {
    .s = "true",
    .i = 1,
    .r = 1.0,
    .b = true,
    .type = 'b'
};

/*!
 * \brief script_false
 */
ScriptValue script_false = {
    .s = "false",
    .i = 0,
    .r = 0.0,
    .b = false,
    .type = 'b'
};

/*!
 * \brief script_bool
 * \param b
 * \return
 */
ScriptValue
script_bool(bool b)
{
    if (b) {
        return script_true;
    }
    return script_false;
}

/*!
 * \brief script_int
 * \param i
 * \return
 */
ScriptValue
script_int(int i)
{
    ScriptValue v;
    v.i = i;
    v.type = 'i';
    return v;
}

/*!
 * \brief script_real
 * \param r
 * \return
 */
ScriptValue
script_real(double r)
{
    ScriptValue v;
    v.r = r;
    v.type = 'r';
    return v;
}

/*!
 * \brief script_command
 * \param c The command function pointer.
 * \return
 */
ScriptValue
script_command(CommandPtr *c)
{
    ScriptValue v;
    v.c = c;
    v.type = 'c';
    return v;
}

/*!
 * \brief script_str
 * \param s
 * \return
 */
ScriptValue
script_str(const char *s)
{
    ScriptValue v;
    strncpy(v.s, s, 999);
    v.type = 's';
    return v;
}

/*!
 * \brief Determines whether the supplied variable v is marked as a string so it is
 *        definately safe rely on as_string.
 */
int
is_string(ScriptValue v)
{
    return (v.type == 's');
}

/*!
 * \brief Determines whether the supplied variable v is marked as an integer so it is
 *        definately safe rely on as_int.
 */
int
is_int(ScriptValue v)
{
    return (v.type == 'i');
}

/*!
 * \brief Determines whether the supplied variable v is marked as real so it is
 *        definately safe rely on as_real.
 */
int
is_real(ScriptValue v)
{
    return (v.type == 'r');
}

/*!
 * \brief Determines whether the supplied variable v is marked as a boolean so it is
 *        definately safe rely on as_bool.
 */
int
is_bool(ScriptValue v)
{
    return (v.type == 'b');
}

/*!
 * \brief Determines whether the supplied variable v is marked as null.
 */
int
is_null(ScriptValue v)
{
    return (v.type == '0');
}

/*!
 * \brief is_command
 */
int
is_command(ScriptValue v)
{
    return (v.type == 'c');
}

/*!
 * \brief as_string
 */
char *
as_string(ScriptValue *v)
{
    switch (v->type) {
    case 's':
        return v->s;
    case 'r':
        sprintf(v->s, "%f", v->r);
        break;
    case 'i':
        sprintf(v->s, "%d", v->i);
        break;
    case 'b':
        if (v->b) {
            sprintf(v->s, "true");
        }
        else {
            sprintf(v->s, "false");
        }
        break;
    default:
    case '0':
        sprintf(v->s, "NULL");
        break;
    }
    return v->s;
}

/*!
 * \brief as_int
 */
int
as_int(ScriptValue v)
{
    switch (v.type) {
    case 'i':
        return v.i;
    case 'r':
        return (int)floor(v.r);
    case 's':
        return atoi(v.s);
    default:
        break;
    }
    throwError(GeneralError, "as_real(): Failed to convert to real number.");
    return 0.0;
}

/*!
 * \brief The ScriptValue as a real number.
 *
 * Returns either v->r or the string value converted to a real using atof.
 */
double
as_real(ScriptValue v)
{
    switch (v.type) {
    case 'r':
        return v.r;
    case 'i':
        return 1.0 * v.i;
    case 's':
        return atof(v.s);
    default:
        break;
    }
    throwError(GeneralError, "as_real(): Failed to convert to real number.");
    return 0.0;
}

/*!
 * \brief The script value as a boolean value.
 * \param v
 * \return
 */
bool
as_bool(ScriptValue v)
{
    switch (v.type) {
    case 'b':
        return v.b;
    case 'i':
        if (v.i) {
            return true;
        }
        break;
    case 's':
        if (strncmp(v.s, "true", 999)) {
            return true;
        }
        break;
    default:
        break;
    }
    throwError(GeneralError, "as_bool(): Failed to convert to boolean value.");
    return false;
}

/*!
 * \brief about_command
 */
ScriptValue
about_command(ScriptContext* context)
{
    if (!argument_checks(context, "about", "")) {
        return script_null;
    }
    if (context->context == CONTEXT_MAIN) {
        init_command();
        clear_selection();
    }
    about();
    end_command();
    return script_null;
}

/*!
 * \brief alert_command
 * \param context
 * \return null
 */
ScriptValue
alert_command(ScriptContext* context)
{
    if (!argument_checks(context, "alert", "s")) {
        return script_null;
    }
    alert(STR(0));
    return script_null;
}

/*!
 * \brief blink_prompt_command
 * \param context
 * \return null
 */
ScriptValue
blink_prompt_command(ScriptContext* context)
{
    if (!argument_checks(context, "blinkPrompt", "s")) {
        return script_null;
    }
    blink_prompt();
    return script_null;
}

/*!
 * 
 */
ScriptValue
day_command(ScriptContext* context)
{
    if (!argument_checks(context, "day", "")) {
        return script_null;
    }
    if (context->context == CONTEXT_MAIN) {
        init_command();
        clear_selection();
    }
    day();
    end_command();
    return script_null;
}

/*!
 * \brief debug_command
 */
ScriptValue
debug_command(ScriptContext* context)
{
    if (!argument_checks(context, "debug", "s")) {
        return script_null;
    }
    debug(STR(0));
    return script_null;
}

/*!
 * \brief javaDisablePromptRapidFire
 * \param context
 * \return
 */
ScriptValue
disable_command(ScriptContext* context)
{
    if (!argument_checks(context, "disable", "s")) {
        return script_null;
    }
    const char *line = context->arguments[0].s;
    if (!strcmp(line, "prompt-rapid-fire")) {
        disable_prompt_rapid_fire();
    }
    if (!strcmp(line, "move-rapid-fire")) {
        disable_move_rapid_fire();
    }
    return script_null;
}

/*!
 * \brief enable
 * \param context
 * \return
 */
ScriptValue
enable_command(ScriptContext* context)
{
    if (!argument_checks(context, "enable", "s")) {
        return script_null;
    }
    const char *line = context->arguments[0].s;
    if (!strcmp(line, "prompt-rapid-fire")) {
        enable_prompt_rapid_fire();
    }
    if (!strcmp(line, "move-rapid-fire")) {
        enable_move_rapid_fire();
    }
    return script_null;
}

/*!
 */
ScriptValue
error_command(ScriptContext* context)
{
    if (!argument_checks(context, "error", "s")) {
        return script_null;
    }
    char message[1000];
    sprintf(message, "ERROR: %s", STR(0));
    alert(message);
    end_command();
    return script_null;
}

/*!
 * 
 */
ScriptValue
help_command(ScriptContext* context)
{
    if (!argument_checks(context, "help", "")) {
        return script_null;
    }
    if (context->context == CONTEXT_MAIN) {
        init_command();
        clear_selection();
    }
    help();
    end_command();
    return script_null;
}

/*!
 * 
 */
ScriptValue
icon128_command(ScriptContext* context)
{
    if (!argument_checks(context, "icon128", "")) {
        return script_null;
    }
    if (context->context == CONTEXT_MAIN) {
        init_command();
        clear_selection();
    }
    icon128();
    end_command();
    return script_null;
}

/*!
 * 
 */
ScriptValue
icon16_command(ScriptContext* context)
{
    if (!argument_checks(context, "icon16", "")) {
        return script_null;
    }
    if (context->context == CONTEXT_MAIN) {
        init_command();
        clear_selection();
    }
    icon16();
    end_command();
    return script_null;
}

/*!
 * 
 */
ScriptValue
icon24_command(ScriptContext* context)
{
    if (!argument_checks(context, "icon24", "")) {
        return script_null;
    }
    if (context->context == CONTEXT_MAIN) {
        init_command();
        clear_selection();
    }
    icon24();
    end_command();
    return script_null;
}

/*!
 * 
 */
ScriptValue
icon32_command(ScriptContext* context)
{
    if (!argument_checks(context, "icon32", "")) {
        return script_null;
    }
    if (context->context == CONTEXT_MAIN) {
        init_command();
        clear_selection();
    }
    icon32();
    end_command();
    return script_null;
}

/*!
 * 
 */
ScriptValue
icon48_command(ScriptContext* context)
{
    if (!argument_checks(context, "icon48", "")) {
        return script_null;
    }
    if (context->context == CONTEXT_MAIN) {
        init_command();
        clear_selection();
    }
    icon48();
    end_command();
    return script_null;
}

/*!
 * 
 */
ScriptValue
icon64_command(ScriptContext* context)
{
    if (!argument_checks(context, "icon64", "")) {
        return script_null;
    }
    if (context->context == CONTEXT_MAIN) {
        init_command();
        clear_selection();
    }
    icon64();
    end_command();
    return script_null;
}

/*!
 * 
 */
ScriptValue
new_file_command(ScriptContext* context)
{
    if (!argument_checks(context, "new_file", "")) {
        return script_null;
    }
    if (context->context == CONTEXT_MAIN) {
        init_command();
        clear_selection();
    }
    new_file();
    end_command();
    return script_null;
}

ScriptValue
open_file_command(ScriptContext* context)
{
    if (!argument_checks(context, "open_file", "")) {
        return script_null;
    }
    if (context->context == CONTEXT_MAIN) {
        init_command();
        clear_selection();
    }
    open_file();
    end_command();
    return script_null;
}

/*!
 */
ScriptValue
night_command(ScriptContext* context)
{
    if (!argument_checks(context, "night", "")) {
        return script_null;
    }
    if (context->context == CONTEXT_MAIN) {
        init_command();
        clear_selection();
    }
    night();
    end_command();
    return script_null;
}

/*!
 */
ScriptValue
pan_down_command(ScriptContext* context)
{
    if (!argument_checks(context, "pan_down", "")) {
        return script_null;
    }
    if (context->context == CONTEXT_MAIN) {
        init_command();
    }
    pan_down();
    end_command();
    return script_null;
}

/*!
 */
ScriptValue
pan_left_command(ScriptContext* context)
{
    if (!argument_checks(context, "pan_left", "")) {
        return script_null;
    }
    if (context->context == CONTEXT_MAIN) {
        init_command();
    }
    pan_left();
    end_command();
    return script_null;
}

ScriptValue
pan_right_command(ScriptContext* context)
{
    if (!argument_checks(context, "pan_right", "")) {
        return script_null;
    }
    if (context->context == CONTEXT_MAIN) {
        init_command();
    }
    pan_right();
    end_command();
    return script_null;
}

ScriptValue
pan_up_command(ScriptContext* context)
{
    if (!argument_checks(context, "pan_up", "")) {
        return script_null;
    }
    if (context->context == CONTEXT_MAIN) {
        init_command();
    }
    pan_up();
    end_command();
    return script_null;
}

ScriptValue
redo_command(ScriptContext* context)
{
    if (!argument_checks(context, "redo", "")) {
        return script_null;
    }
    if (context->context == CONTEXT_MAIN) {
        init_command();
        clear_selection();
    }
    redo();
    end_command();
    return script_null;
}

#if 0
ScriptValue
SetPromptPrefix(ScriptContext* context)
{
    if (!argument_checks(context, "set_prompt_prefix", "s")) {
        return script_null;
    }
    set_prompt_prefix(STR(0));
    return script_null;
}

ScriptValue
AppendPromptHistory(ScriptContext* context)
{
    int args = context->argument_count;
    if (args == 0) {
        appendPromptHistory("");
    }
    else if (args == 1) {
        appendPromptHistory(STR(0));
    }
    else {
        return context->throwError("appendPromptHistory() requires one or zero arguments");
    }
    return script_null;
}

ScriptValue
EnablePromptRapidFire(ScriptContext* context)
{
    if (context->argumentCount() != 0) {
        return context->throwError("enablePromptRapidFire() requires zero arguments");
    }
    mainWin()->nativeEnablePromptRapidFire();
    return script_null;
}

ScriptValue
DisablePromptRapidFire(ScriptContext* context)
{
    if (context->argumentCount() != 0) {
        return context->throwError("disablePromptRapidFire() requires zero arguments");
    }
    mainWin()->nativeDisablePromptRapidFire();
    return script_null;
}

ScriptValue
EnableMoveRapidFire(ScriptContext* context)
{
    if (context->argumentCount() != 0) return context->throwError("enableMoveRapidFire() requires zero arguments");

    mainWin()->nativeEnableMoveRapidFire();
    return script_null;
}

ScriptValue
DisableMoveRapidFire(ScriptContext* context)
{
    if (context->argumentCount() != 0) return context->throwError("disableMoveRapidFire() requires zero arguments");

    mainWin()->nativeDisableMoveRapidFire();
    return script_null;
}

ScriptValue
InitCommand(ScriptContext* context)
{
    if (context->argumentCount() != 0) return context->throwError("initCommand() requires zero arguments");

    mainWin()->nativeInitCommand();
    return script_null;
}

ScriptValue
EndCommand(ScriptContext* context)
{
    if (context->argumentCount() != 0) {
        return context->throwError("endCommand() requires zero arguments");
    }
    mainWin()->nativeEndCommand();
    return script_null;
}

ScriptValue
Exit(ScriptContext* context)
{
    if (context->argumentCount() != 0)
        return context->throwError("exit() requires zero arguments");

    mainWin()->nativeExit();
    return script_null;
}

ScriptValue
Help(ScriptContext* context)
{
    if (context->argumentCount() != 0)
        return context->throwError("help() requires zero arguments");

    mainWin()->nativeHelp();
    return script_null;
}


ScriptValue
TipOfTheDay(ScriptContext* context)
{
    if (context->argumentCount() != 0)
        return context->throwError("tipOfTheDay() requires zero arguments");

    mainWin()->nativeTipOfTheDay();
    return script_null;
}

ScriptValue
PlatformString(ScriptContext* context)
{
    if (context->argumentCount() != 0) return context->throwError("platformString() requires zero arguments");
    return script_str(mainWin()->nativePlatformString());
}

ScriptValue
MessageBox(ScriptContext* context)
{
    if (!argument_checks(context, "pan_up", "sss")) {
        return script_null;
    }

    QString type  = context->argument(0).toString().toLower();
    QString title = context->argument(1).toString();
    QString text  = context->argument(2).toString();

    if (type != "critical" && type != "information" && type != "question" && type != "warning")
        return context->throwError(ScriptContext::UnknownError, "messageBox(): first argument must be \"critical\", \"information\", \"question\" or \"warning\".");

    mainWin()->nativeMessageBox(type, title, text);
    return script_null;
}

ScriptValue
IsInt(ScriptContext* context)
{
    if (!argument_checks(context, "pan_up", "i")) {
        return script_null;
    }

    qreal num = context->argument(0).r;

    if (fmod(num, 1) == 0) {
        return script_true;
    }
    return script_false;
}

ScriptValue
print_area_command(ScriptContext* context)
{
    if (!argument_checks(context, "print_area", "rrrr")) {
        return script_null;
    }
    print_area(REAL(0), REAL(1), REAL(2), REAL(3));
    return script_null;
}

ScriptValue
DayVision(ScriptContext* context)
{
    if (context->argumentCount() != 0) return context->throwError("dayVision() requires zero arguments");

    mainWin()->nativeDayVision();
    return script_null;
}

ScriptValue
NightVision(ScriptContext* context)
{
    if (context->argumentCount() != 0) return context->throwError("nightVision() requires zero arguments");

    mainWin()->nativeNightVision();
    return script_null;
}

ScriptValue
SetBackgroundColor(ScriptContext* context)
{
    if (!argument_checks(context, "set_background_color", "iii")) {
        return script_null;
    }

    qreal r = context->argument(0).r;
    qreal g = context->argument(1).r;
    qreal b = context->argument(2).r;

    if (r < 0 || r > 255) { return context->throwError(ScriptContext::UnknownError, "setBackgroundColor(): r value must be in range 0-255"); }
    if (g < 0 || g > 255) { return context->throwError(ScriptContext::UnknownError, "setBackgroundColor(): g value must be in range 0-255"); }
    if (b < 0 || b > 255) { return context->throwError(ScriptContext::UnknownError, "setBackgroundColor(): b value must be in range 0-255"); }

    mainWin()->nativeSetBackgroundColor(INT(0), INT(1), INT(2));
    return script_null;
}

ScriptValue
SetCrossHairColor(ScriptContext* context)
{
    if (!argument_checks(context, "set_crosshair_color", "iii")) {
        return script_null;
    }

    qreal r = context->argument(0).r;
    qreal g = context->argument(1).r;
    qreal b = context->argument(2).r;

    if (r < 0 || r > 255) { return context->throwError(ScriptContext::UnknownError, "setCrossHairColor(): r value must be in range 0-255"); }
    if (g < 0 || g > 255) { return context->throwError(ScriptContext::UnknownError, "setCrossHairColor(): g value must be in range 0-255"); }
    if (b < 0 || b > 255) { return context->throwError(ScriptContext::UnknownError, "setCrossHairColor(): b value must be in range 0-255"); }

    mainWin()->nativeSetCrossHairColor(INT(0), INT(1), INT(2));
    return script_null;
}

ScriptValue
SetGridColor(ScriptContext* context)
{
    if (!argument_checks(context, "set_grid_color", "iii")) {
        return script_null;
    }

    qreal r = context->argument(0).r;
    qreal g = context->argument(1).r;
    qreal b = context->argument(2).r;

    if (r < 0 || r > 255) { return context->throwError(ScriptContext::UnknownError, "setGridColor(): r value must be in range 0-255"); }
    if (g < 0 || g > 255) { return context->throwError(ScriptContext::UnknownError, "setGridColor(): g value must be in range 0-255"); }
    if (b < 0 || b > 255) { return context->throwError(ScriptContext::UnknownError, "setGridColor(): b value must be in range 0-255"); }

    mainWin()->nativeSetGridColor(INT(0), INT(1), INT(2));
    return script_null;
}

ScriptValue
TextFont(ScriptContext* context)
{
    if (!argument_checks(context, "text_font", "")) {
        return script_null;
    }
    return script_str(mainWin()->nativeTextFont());
}

ScriptValue
TextSize(ScriptContext* context)
{
    if (context->argumentCount() != 0) return context->throwError("textSize() requires zero arguments");
    return ScriptValue(mainWin()->nativeTextSize());
}

ScriptValue
TextAngle(ScriptContext* context)
{
    if (context->argumentCount() != 0) return context->throwError("textAngle() requires zero arguments");
    return ScriptValue(mainWin()->nativeTextAngle());
}

ScriptValue
TextBold(ScriptContext* context)
{
    if (context->argumentCount() != 0) return context->throwError("textBold() requires zero arguments");
    return ScriptValue(mainWin()->nativeTextBold());
}

ScriptValue
TextItalic(ScriptContext* context)
{
    if (context->argumentCount() != 0) return context->throwError("textItalic() requires zero arguments");
    return ScriptValue(mainWin()->nativeTextItalic());
}

ScriptValue
TextUnderline(ScriptContext* context)
{
    if (context->argumentCount() != 0) return context->throwError("textUnderline() requires zero arguments");
    return ScriptValue(mainWin()->nativeTextUnderline());
}

ScriptValue
TextStrikeOut(ScriptContext* context)
{
    if (context->argumentCount() != 0) return context->throwError("textStrikeOut() requires zero arguments");
    return ScriptValue(mainWin()->nativeTextStrikeOut());
}

ScriptValue
TextOverline(ScriptContext* context)
{
    if (context->argumentCount() != 0) return context->throwError("textOverline() requires zero arguments");
    return ScriptValue(mainWin()->nativeTextOverline());
}

ScriptValue
SetTextFont(ScriptContext* context)
{
    if (context->argumentCount() != 1)    return context->throwError("setTextFont() requires one argument");
    if (!context->argument(0).isString()) return context->throwError(TypeError, "setTextFont(): first argument is not a string");

    mainWin()->nativeSetTextFont(context->argument(0).toString());
    return script_null;
}

ScriptValue
SetTextSize(ScriptContext* context)
{
    if (context->argumentCount() != 1)    return context->throwError("setTextSize() requires one argument");
    if (!context->argument(0).isNumber()) return context->throwError(TypeError, "setTextSize(): first argument is not a number");

    qreal num = context->argument(0).r;

    mainWin()->nativeSetTextSize(num);
    return script_null;
}

ScriptValue
SetTextAngle(ScriptContext* context)
{
    if (context->argumentCount() != 1)    return context->throwError("setTextAngle() requires one argument");
    if (!context->argument(0).isNumber()) return context->throwError(TypeError, "setTextAngle(): first argument is not a number");

    qreal num = context->argument(0).r;

    mainWin()->nativeSetTextAngle(num);
    return script_null;
}

ScriptValue
SetTextBold(ScriptContext* context)
{
    if (context->argumentCount() != 1)    return context->throwError("setTextBold() requires one argument");
    if (!context->argument(0).isBool()) return context->throwError(TypeError, "setTextBold(): first argument is not a bool");

    mainWin()->nativeSetTextBold(context->argument(0).toBool());
    return script_null;
}

ScriptValue
SetTextItalic(ScriptContext* context)
{
    if (context->argumentCount() != 1)    return context->throwError("setTextItalic() requires one argument");
    if (!context->argument(0).isBool()) return context->throwError(TypeError, "setTextItalic(): first argument is not a bool");

    mainWin()->nativeSetTextItalic(context->argument(0).toBool());
    return script_null;
}

ScriptValue
SetTextUnderline(ScriptContext* context)
{
    if (context->argumentCount() != 1)    return context->throwError("setTextUnderline() requires one argument");
    if (!context->argument(0).isBool()) return context->throwError(TypeError, "setTextUnderline(): first argument is not a bool");

    mainWin()->nativeSetTextUnderline(context->argument(0).toBool());
    return script_null;
}

ScriptValue
SetTextStrikeOut(ScriptContext* context)
{
    if (context->argumentCount() != 1)    return context->throwError("setTextStrikeOut() requires one argument");
    if (!context->argument(0).isBool()) return context->throwError(TypeError, "setTextStrikeOut(): first argument is not a bool");

    mainWin()->nativeSetTextStrikeOut(BOOL(0));
    return script_null;
}

ScriptValue
SetTextOverline(ScriptContext* context)
{
    if (context->argumentCount() != 1)    return context->throwError("setTextOverline() requires one argument");
    if (!context->argument(0).isBool()) return context->throwError(TypeError, "setTextOverline(): first argument is not a bool");

    mainWin()->nativeSetTextOverline(BOOL(0));
    return script_null;
}

ScriptValue
PreviewOn(ScriptContext* context)
{
    if (!argument_checks(context, "preview_on", "ssrrr")) {
        return script_null;
    }

    QString cloneStr = context->argument(0).toString().toUpper();
    QString modeStr  = context->argument(1).toString().toUpper();
    qreal x          = context->argument(2).r;
    qreal y          = context->argument(3).r;
    qreal data       = context->argument(4).r;

    int clone = PREVIEW_CLONE_NULL;
    int mode = PREVIEW_MODE_NULL;
    if     (cloneStr == "SELECTED") { clone = PREVIEW_CLONE_SELECTED; }
    else if (cloneStr == "RUBBER") {
        clone = PREVIEW_CLONE_RUBBER;
    }
    else {
        return context->throwError(ScriptContext::UnknownError, "previewOn(): first argument must be \"SELECTED\" or \"RUBBER\".");
    }

    if     (modeStr == "MOVE") { mode = PREVIEW_MODE_MOVE;   }
    else if (modeStr == "ROTATE") { mode = PREVIEW_MODE_ROTATE; }
    else if (modeStr == "SCALE") { mode = PREVIEW_MODE_SCALE;  }
    else { return context->throwError(ScriptContext::UnknownError, "previewOn(): second argument must be \"MOVE\", \"ROTATE\" or \"SCALE\"."); }

    mainWin()->nativePreviewOn(clone, mode, x, y, data);
    return script_null;
}

ScriptValue
PreviewOff(ScriptContext* context)
{
    if (context->argumentCount() != 0) return context->throwError("previewOff() requires zero arguments");

    mainWin()->nativePreviewOff();
    return script_null;
}

ScriptValue
Vulcanize(ScriptContext* context)
{
    if (context->argumentCount() != 0) return context->throwError("vulcanize() requires zero arguments");

    mainWin()->nativeVulcanize();
    return script_null;
}

ScriptValue
AllowRubber(ScriptContext* context)
{
    if (context->argumentCount() != 0) return context->throwError("allowRubber() requires zero arguments");

    return ScriptValue(allowRubber());
}

ScriptValue
SetRubberMode(ScriptContext* context)
{
    if (context->argumentCount() != 1)    return context->throwError("setRubberMode() requires one argument");
    if (!context->argument(0).isString()) return context->throwError(TypeError, "setRubberMode(): first argument is not a string");

    QString mode = context->argument(0).toString().toUpper();

    if     (mode == "CIRCLE_1P_RAD") { mainWin()->nativeSetRubberMode(OBJ_RUBBER_CIRCLE_1P_RAD); }
    else if (mode == "CIRCLE_1P_DIA") { mainWin()->nativeSetRubberMode(OBJ_RUBBER_CIRCLE_1P_DIA); }
    else if (mode == "CIRCLE_2P") { mainWin()->nativeSetRubberMode(OBJ_RUBBER_CIRCLE_2P); }
    else if (mode == "CIRCLE_3P") { mainWin()->nativeSetRubberMode(OBJ_RUBBER_CIRCLE_3P); }
    else if (mode == "CIRCLE_TTR") { mainWin()->nativeSetRubberMode(OBJ_RUBBER_CIRCLE_TTR); }
    else if (mode == "CIRCLE_TTR") { mainWin()->nativeSetRubberMode(OBJ_RUBBER_CIRCLE_TTT); }

    else if (mode == "DIMLEADER_LINE") { mainWin()->nativeSetRubberMode(OBJ_RUBBER_DIMLEADER_LINE); }

    else if (mode == "ELLIPSE_LINE") { mainWin()->nativeSetRubberMode(OBJ_RUBBER_ELLIPSE_LINE); }
    else if (mode == "ELLIPSE_MAJORDIAMETER_MINORRADIUS") { mainWin()->nativeSetRubberMode(OBJ_RUBBER_ELLIPSE_MAJORDIAMETER_MINORRADIUS); }
    else if (mode == "ELLIPSE_MAJORRADIUS_MINORRADIUS") { mainWin()->nativeSetRubberMode(OBJ_RUBBER_ELLIPSE_MAJORRADIUS_MINORRADIUS); }
    else if (mode == "ELLIPSE_ROTATION") { mainWin()->nativeSetRubberMode(OBJ_RUBBER_ELLIPSE_ROTATION); }

    else if (mode == "LINE") { mainWin()->nativeSetRubberMode(OBJ_RUBBER_LINE); }

    else if (mode == "POLYGON") { mainWin()->nativeSetRubberMode(OBJ_RUBBER_POLYGON); }
    else if (mode == "POLYGON_INSCRIBE") { mainWin()->nativeSetRubberMode(OBJ_RUBBER_POLYGON_INSCRIBE); }
    else if (mode == "POLYGON_CIRCUMSCRIBE") { mainWin()->nativeSetRubberMode(OBJ_RUBBER_POLYGON_CIRCUMSCRIBE); }

    else if (mode == "POLYLINE") { mainWin()->nativeSetRubberMode(OBJ_RUBBER_POLYLINE); }

    else if (mode == "RECTANGLE") { mainWin()->nativeSetRubberMode(OBJ_RUBBER_RECTANGLE); }

    else if (mode == "TEXTSINGLE") { mainWin()->nativeSetRubberMode(OBJ_RUBBER_TEXTSINGLE); }

    else              { return context->throwError(ScriptContext::UnknownError, "setRubberMode(): unknown rubberMode value"); }

    return script_null;
}

ScriptValue
SetRubberPoint(ScriptContext* context)
{
    if (!argument_checks(context, "set_rubber_point", "srr")) {
        return script_null;
    }

    QString key = context->argument(0).toString().toUpper();
    qreal x     = context->argument(1).r;
    qreal y     = context->argument(2).r;

    mainWin()->nativeSetRubberPoint(key, x, y);
    return script_null;
}

ScriptValue
SetRubberText(ScriptContext* context)
{
    if (!argument_checks(context, "set_rubber_text", "ss")) {
        return script_null;
    }
    set_rubber_text(STR(0), STR(1));
    return script_null;
}

ScriptValue
AddRubber(ScriptContext* context)
{
    if (context->argumentCount() != 1)    return context->throwError("addRubber() requires one argument");
    if (!context->argument(0).isString()) return context->throwError(TypeError, "addRubber(): first argument is not a string");

    QString objType = context->argument(0).toString().toUpper();

    if (!allowRubber())
        return context->throwError(ScriptContext::UnknownError, "addRubber(): You must use vulcanize() before you can add another rubber object.");

    qreal mx = mainWin()->nativeMouseX();
    qreal my = mainWin()->nativeMouseY();

    if     (objType == "ARC") {} //TODO: handle this type
    else if (objType == "BLOCK") {} //TODO: handle this type
    else if (objType == "CIRCLE") { addCircle(mx, my, 0, false, OBJ_RUBBER_ON); }
    else if (objType == "DIMALIGNED") {} //TODO: handle this type
    else if (objType == "DIMANGULAR") {} //TODO: handle this type
    else if (objType == "DIMARCLENGTH") {} //TODO: handle this type
    else if (objType == "DIMDIAMETER") {} //TODO: handle this type
    else if (objType == "DIMLEADER") { addDimLeader(mx, my, mx, my, 0, OBJ_RUBBER_ON); }
    else if (objType == "DIMLINEAR") {} //TODO: handle this type
    else if (objType == "DIMORDINATE") {} //TODO: handle this type
    else if (objType == "DIMRADIUS") {} //TODO: handle this type
    else if (objType == "ELLIPSE") { addEllipse(mx, my, 0, 0, 0, 0, OBJ_RUBBER_ON); }
    else if (objType == "ELLIPSEARC") {} //TODO: handle this type
    else if (objType == "HATCH") {} //TODO: handle this type
    else if (objType == "IMAGE") {} //TODO: handle this type
    else if (objType == "INFINITELINE") {} //TODO: handle this type
    else if (objType == "LINE") { addLine(mx, my, mx, my, 0, OBJ_RUBBER_ON); }
    else if (objType == "PATH") {} //TODO: handle this type
    else if (objType == "POINT") {} //TODO: handle this type
    else if (objType == "POLYGON") { addPolygon(mx, my, QPainterPath(), OBJ_RUBBER_ON); }
    else if (objType == "POLYLINE") { addPolyline(mx, my, QPainterPath(), OBJ_RUBBER_ON); }
    else if (objType == "RAY") {} //TODO: handle this type
    else if (objType == "RECTANGLE") { addRectangle(mx, my, mx, my, 0, 0, OBJ_RUBBER_ON); }
    else if (objType == "SPLINE") {} //TODO: handle this type
    else if (objType == "TEXTMULTI") {} //TODO: handle this type
    else if (objType == "TEXTSINGLE") { addTextSingle("", mx, my, 0, false, OBJ_RUBBER_ON); }

    return script_null;
}

ScriptValue
ClearRubber(ScriptContext* context)
{
    if (context->argumentCount() != 0) return context->throwError("clearRubber() requires zero arguments");

    mainWin()->nativeClearRubber();
    return script_null;
}

ScriptValue
SpareRubber(ScriptContext* context)
{
    if (context->argumentCount() != 1)    return context->throwError("spareRubber() requires one argument");
    if (!context->argument(0).isString()) return context->throwError(TypeError, "spareRubber(): first argument is not a string");

    QString objID = context->argument(0).toString().toUpper();

    if     (objID == "PATH") { mainWin()->nativeSpareRubber(SPARE_RUBBER_PATH);     }
    else if (objID == "POLYGON") { mainWin()->nativeSpareRubber(SPARE_RUBBER_POLYGON);  }
    else if (objID == "POLYLINE") { mainWin()->nativeSpareRubber(SPARE_RUBBER_POLYLINE); }
    else {
        bool ok = false;
        qint64 id = objID.toLongLong(&ok);
        if (!ok) return context->throwError(TypeError, "spareRubber(): error converting object ID into an int64");
        mainWin()->nativeSpareRubber(id);
    }

    return script_null;
}

ScriptValue
AddTextMulti(ScriptContext* context)
{
    if (!argument_checks(context, "add_text_multi", "srrrb")) {
        return script_null;
    }
    addTextMulti(STR(0), REAL(1), REAL(2), REAL(3), BOOL(4),
        OBJ_RUBBER_OFF);
    return script_null;
}

ScriptValue
AddTextSingle(ScriptContext* context)
{
    if (!argument_checks(context, "add_text_multi", "srrrb")) {
        return script_null;
    }
    addTextSingle(STR(0), REAL(1), REAL(2), REAL(3), BOOL(4),
        OBJ_RUBBER_OFF);
    return script_null;
}

ScriptValue
AddInfiniteLine(ScriptContext* context)
{
    //TODO: parameter error checking
    qDebug("TODO: finish addInfiniteLine command");
    return script_null;
}

ScriptValue
AddRay(ScriptContext* context)
{
    //TODO: parameter error checking
    qDebug("TODO: finish addRay command");
    return script_null;
}

ScriptValue
AddLine(ScriptContext* context)
{
    if (!argument_checks(context, "add_text_multi", "rrrrr")) {
        return script_null;
    }
    addLine(REAL(0), REAL(1), REAL(2), REAL(3), REAL(4),
        OBJ_RUBBER_OFF);
    return script_null;
}

ScriptValue
add_triangle_command(ScriptContext* context)
{
    if (!argument_checks(context, "add_triangle", "rrrrrrrb")) {
        return script_null;
    }
    add_triangle(REAL(0), REAL(1), REAL(2), REAL(3),
        REAL(4), REAL(5), REAL(6), BOOL(7));
    return script_null;
}

ScriptValue
AddRectangle(ScriptContext* context)
{
    if (!argument_checks(context, "add_text_multi", "rrrrrb")) {
        return script_null;
    }

    qreal x    = context->argument(0).r;
    qreal y    = context->argument(1).r;
    qreal w    = context->argument(2).r;
    qreal h    = context->argument(3).r;
    qreal rot  = context->argument(4).r;
    bool  fill = context->argument(5).toBool();

    addRectangle(x, y, w, h, rot, fill, OBJ_RUBBER_OFF);
    return script_null;
}

ScriptValue
AddRoundedRectangle(ScriptContext* context)
{
    if (!argument_checks(context, "add_text_multi", "rrrrrrb")) {
        return script_null;
    }

    qreal x    = context->argument(0).r;
    qreal y    = context->argument(1).r;
    qreal w    = context->argument(2).r;
    qreal h    = context->argument(3).r;
    qreal rad  = context->argument(4).r;
    qreal rot  = context->argument(5).r;
    bool  fill = context->argument(6).toBool();

    addRoundedRectangle(x, y, w, h, rad, rot, fill);
    return script_null;
}

ScriptValue
AddArc(ScriptContext* context)
{
    if (!argument_checks(context, "add_text_multi", "rrrrrr")) {
        return script_null;
    }

    qreal startX = context->argument(0).r;
    qreal startY = context->argument(1).r;
    qreal midX   = context->argument(2).r;
    qreal midY   = context->argument(3).r;
    qreal endX   = context->argument(4).r;
    qreal endY   = context->argument(5).r;

    addArc(startX, startY, midX, midY, endX, endY, OBJ_RUBBER_OFF);
    return script_null;
}

ScriptValue
AddCircle(ScriptContext* context)
{
    if (!argument_checks(context, "add_text_multi", "rrrb")) {
        return script_null;
    }

    qreal centerX = context->argument(0).r;
    qreal centerY = context->argument(1).r;
    qreal radius  = context->argument(2).r;
    bool  fill    = context->argument(3).toBool();

    addCircle(centerX, centerY, radius, fill, OBJ_RUBBER_OFF);
    return script_null;
}

ScriptValue
AddSlot(ScriptContext* context)
{
    if (!argument_checks(context, "add_slot", "rrrrrb")) {
        return script_null;
    }

    qreal centerX  = context->argument(0).r;
    qreal centerY  = context->argument(1).r;
    qreal diameter = context->argument(2).r;
    qreal length   = context->argument(3).r;
    qreal rot      = context->argument(4).r;
    bool  fill     = context->argument(5).toBool();

    addSlot(centerX, centerY, diameter, length, rot, fill, OBJ_RUBBER_OFF);
    return script_null;
}

ScriptValue
AddEllipse(ScriptContext* context)
{
    if (!argument_checks(context, "add_ellipse", "rrrrrb")) {
        return script_null;
    }

    qreal centerX = context->argument(0).r;
    qreal centerY = context->argument(1).r;
    qreal radX    = context->argument(2).r;
    qreal radY    = context->argument(3).r;
    qreal rot     = context->argument(4).r;
    bool  fill    = context->argument(5).toBool();

    addEllipse(centerX, centerY, radX, radY, rot, fill, OBJ_RUBBER_OFF);
    return script_null;
}

ScriptValue
AddPoint(ScriptContext* context)
{
    if (context->argumentCount() != 2)    return context->throwError("addPoint() requires two arguments");
    if (!context->argument(0).isNumber()) return context->throwError(TypeError, "addPoint(): first argument is not a number");
    if (!context->argument(1).isNumber()) return context->throwError(TypeError, "addPoint(): second argument is not a number");

    qreal x = context->argument(0).r;
    qreal y = context->argument(1).r;

    addPoint(x,y);
    return script_null;
}

ScriptValue
AddRegularPolygon(ScriptContext* context)
{
    //TODO: parameter error checking
    qDebug("TODO: finish addRegularPolygon command");
    return script_null;
}

ScriptValue
AddPolygon(ScriptContext* context)
{
    if (context->argumentCount() != 1)   return context->throwError("addPolygon() requires one argument");
    if (!context->argument(0).isArray()) return context->throwError(TypeError, "addPolygon(): first argument is not an array");

    QVariantList varList = context->argument(0).toVariant().toList();
    int varSize = varList.size();
    if (varSize < 2) return context->throwError(TypeError, "addPolygon(): array must contain at least two elements");
    if (varSize % 2) return context->throwError(TypeError, "addPolygon(): array cannot contain an odd number of elements");

    bool lineTo = false;
    bool xCoord = true;
    qreal x = 0;
    qreal y = 0;
    qreal startX = 0;
    qreal startY = 0;
    QPainterPath path;
    foreach(QVariant var, varList) {
        if (var.canConvert(QVariant::Double)) {
            if (xCoord) {
                xCoord = false;
                x = var.toReal();
            }
            else {
                xCoord = true;
                y = -var.toReal();

                if (lineTo) { path.lineTo(x,y); }
                else { path.moveTo(x,y); lineTo = true; startX = x; startY = y; }
            }
        }
        else
            return context->throwError(TypeError, "addPolygon(): array contains one or more invalid elements");
    }

    //Close the polygon
    path.closeSubpath();

    path.translate(-startX, -startY);

    addPolygon(startX, startY, path, OBJ_RUBBER_OFF);
    return script_null;
}

ScriptValue
AddPolyline(ScriptContext* context)
{
    if (context->argumentCount() != 1)   return context->throwError("addPolyline() requires one argument");
    if (!context->argument(0).isArray()) return context->throwError(TypeError, "addPolyline(): first argument is not an array");

    QVariantList varList = context->argument(0).toVariant().toList();
    int varSize = varList.size();
    if (varSize < 2) return context->throwError(TypeError, "addPolyline(): array must contain at least two elements");
    if (varSize % 2) return context->throwError(TypeError, "addPolyline(): array cannot contain an odd number of elements");

    bool lineTo = false;
    bool xCoord = true;
    qreal x = 0;
    qreal y = 0;
    qreal startX = 0;
    qreal startY = 0;
    QPainterPath path;
    foreach(QVariant var, varList) {
        if (var.canConvert(QVariant::Double)) {
            if (xCoord) {
                xCoord = false;
                x = var.toReal();
            }
            else {
                xCoord = true;
                y = -var.toReal();

                if (lineTo) { path.lineTo(x,y); }
                else { path.moveTo(x,y); lineTo = true; startX = x; startY = y; }
            }
        }
        else
            return context->throwError(TypeError, "addPolyline(): array contains one or more invalid elements");
    }

    path.translate(-startX, -startY);

    addPolyline(startX, startY, path, OBJ_RUBBER_OFF);
    return script_null;
}

ScriptValue
AddPath(ScriptContext* context)
{
    //TODO: parameter error checking
    qDebug("TODO: finish addPath command");
    return script_null;
}

ScriptValue
AddHorizontalDimension(ScriptContext* context)
{
    //TODO: parameter error checking
    qDebug("TODO: finish addHorizontalDimension command");
    return script_null;
}

ScriptValue
AddVerticalDimension(ScriptContext* context)
{
    //TODO: parameter error checking
    qDebug("TODO: finish addVerticalDimension command");
    return script_null;
}

ScriptValue
addImage(ScriptContext* context)
{
    //TODO: parameter error checking
    qDebug("TODO: finish addImage command");
    return script_null;
}

ScriptValue
addDimLeader(ScriptContext* context)
{
    if (!argument_checks(context, "add_ellipse", "rrrrr")) {
        return script_null;
    }

    qreal x1  = context->argument(0).r;
    qreal y1  = context->argument(1).r;
    qreal x2  = context->argument(2).r;
    qreal y2  = context->argument(3).r;
    qreal rot = context->argument(4).r;

    addDimLeader(x1, y1, x2, y2, rot, OBJ_RUBBER_OFF);
    return script_null;
}

ScriptValue
SetCursorShape(ScriptContext* context)
{
    if (!argument_checks(context, "set_cursor_shape", "s")) {
        return script_null;
    }

    QString shape = context->argument(0).s;
    mainWin()->nativeSetCursorShape(shape);
    return script_null;
}

ScriptValue
CalculateAngle(ScriptContext* context)
{
    if (!argument_checks(context, "calculate_angle", "rrrr")) {
        return script_null;
    }
    double result = calculateAngle(REAL(0), REAL(1), REAL(2), REAL(3));
    return script_real(result);
}

ScriptValue
CalculateDistance(ScriptContext* context)
{
    if (!argument_checks(context, "calculate_angle", "rrrr")) {
        return script_null;
    }
    double result = calculate_distance(REAL(0), REAL(1), REAL(2), REAL(3));
    return script_real(result);
}

ScriptValue
PerpendicularDistance(ScriptContext* context)
{
    if (!argument_checks(context, "calculate_angle", "rrrrrr")) {
        return script_null;
    }
    double result = perpendicular_distance(REAL(0), REAL(1), REAL(2), REAL(3),
        REAL(4), REAL(5));
    return script_real(result);
}

ScriptValue
NumSelected(ScriptContext* context)
{
    if (context->argumentCount() != 0) return context->throwError("numSelected() requires zero arguments");
    return ScriptValue(mainWin()->nativeNumSelected());
}

ScriptValue
SelectAll(ScriptContext* context)
{
    if (context->argumentCount() != 0) return context->throwError("selectAll() requires zero arguments");

    mainWin()->nativeSelectAll();
    return script_null;
}

ScriptValue
AddToSelection(ScriptContext* context)
{
    //TODO: finish
    return script_null;
}

ScriptValue
ClearSelection(ScriptContext* context)
{
    if (context->argumentCount() != 0) return context->throwError("clearSelection() requires zero arguments");

    mainWin()->nativeClearSelection();
    return script_null;
}

ScriptValue
DeleteSelected(ScriptContext* context)
{
    if (context->argumentCount() != 0) return context->throwError("deleteSelected() requires zero arguments");

    mainWin()->nativeDeleteSelected();
    return script_null;
}

ScriptValue
CutSelected(ScriptContext* context)
{
    if (!argument_checks(context, "cut_selected", "rr")) {
        return script_null;
    }
    cut_selected(REAL(0), REAL(1));
    return script_null;
}

ScriptValue
CopySelected(ScriptContext* context)
{
    if (!argument_checks(context, "cut_selected", "rr")) {
        return script_null;
    }
    copy_selected(REAL(0), REAL(1));
    return script_null;
}

ScriptValue
PasteSelected(ScriptContext* context)
{
    if (context->argumentCount() != 2)    return context->throwError("pasteSelected() requires two arguments");
    if (!context->argument(0).isNumber()) return context->throwError(TypeError, "pasteSelected(): first argument is not a number");
    if (!context->argument(1).isNumber()) return context->throwError(TypeError, "pasteSelected(): second argument is not a number");

    qreal x = context->argument(0).r;
    qreal y = context->argument(1).r;

    mainWin()->nativePasteSelected(x, y);
    return script_null;
}

ScriptValue
MoveSelected(ScriptContext* context)
{
    if (!argument_checks(context, "move_selected", "rr")) {
        return script_null;
    }

    qreal dx = context->argument(0).r;
    qreal dy = context->argument(1).r;

    mainWin()->nativeMoveSelected(dx, dy);
    return script_null;
}

ScriptValue
scale_selected_command(ScriptContext* context)
{
    if (!argument_checks(context, "scale_selected", "rrr")) {
        return script_null;
    }

    qreal x      = context->argument(0).r;
    qreal y      = context->argument(1).r;
    qreal factor = context->argument(2).r;

    if (factor <= 0.0) return context->throwError(ScriptContext::UnknownError, "scaleSelected(): scale factor must be greater than zero");

    scale_selected(x, y, factor);
    return script_null;
}
#endif

ScriptValue
rotate_selected_command(ScriptContext* context)
{
    if (!argument_checks(context, "rotate_selected", "rrr")) {
        return script_null;
    }
    rotate_selected(
        context->arguments[0].r,
        context->arguments[1].r,
        context->arguments[2].r);
    return script_null;
}

ScriptValue
mirror_selected_command(ScriptContext* context)
{
    if (!argument_checks(context, "mirror_selected", "rrrr")) {
        return script_null;
    }
    mirror_selected(
        context->arguments[0].r,
        context->arguments[1].r,
        context->arguments[2].r,
        context->arguments[3].r);
    return script_null;
}

/* Report the x-position of the current quicksnap point. */
ScriptValue
qsnap_x_command(ScriptContext* context)
{
    if (!argument_checks(context, "qsnapx", "")) {
        return script_null;
    }
    return script_real(qsnapx());
}

/* Report the y-position of the current quicksnap point. */
ScriptValue
qsnap_y_command(ScriptContext* context)
{
    if (!argument_checks(context, "qsnapy", "")) {
        return script_null;
    }
    return script_real(qsnapy());
}

/* Report the x-position of the mouse. */
ScriptValue
mouse_x_command(ScriptContext* context)
{
    if (!argument_checks(context, "mousex", "")) {
        return script_null;
    }
    return script_real(mousex());
}

/* Report the y-position of the mouse. */
ScriptValue
mouse_y_command(ScriptContext* context)
{
    if (!argument_checks(context, "mousey", "")) {
        return script_null;
    }
    return script_real(mousey());
}

#if 0
==> commands/circle.cpp <==
//Command: Circle

var global = {}; //Required
global.x1;
global.y1;
global.x2;
global.y2;
global.x3;
global.y3;
global.rad;
global.dia;
global.cx;
global.cy;
global.mode;

//enums
global.mode_1P_RAD = 0;
global.mode_1P_DIA = 1;
global.mode_2P     = 2;
global.mode_3P     = 3;
global.mode_TTR    = 4;

//NOTE: main() is run every time the command is started.
//      Use it to reset variables so they are ready to go.
function main()
{
    initCommand();
    clearSelection();
    global.mode = global.mode_1P_RAD;
    global.x1 = NaN;
    global.y1 = NaN;
    global.x2 = NaN;
    global.y2 = NaN;
    global.x3 = NaN;
    global.y3 = NaN;
    setPromptPrefix(qsTr("Specify center point for circle or [3P/2P/Ttr (tan tan radius)]: "));
}

//NOTE: click() is run only for left clicks.
//      Middle clicks are used for panning.
//      Right clicks bring up the context menu.
function click(x, y)
{
    if (global.mode == global.mode_1P_RAD) {
        if (isNaN(global.x1)) {
            global.x1 = x;
            global.y1 = y;
            global.cx = x;
            global.cy = y;
            addRubber("CIRCLE");
            setRubberMode("CIRCLE_1P_RAD");
            setRubberPoint("CIRCLE_CENTER", global.cx, global.cy);
            appendPromptHistory();
            setPromptPrefix(qsTr("Specify radius of circle or [Diameter]: "));
        }
        else {
            global.x2 = x;
            global.y2 = y;
            setRubberPoint("CIRCLE_RADIUS", global.x2, global.y2);
            vulcanize();
            appendPromptHistory();
            endCommand();
        }
    }
    else if (global.mode == global.mode_1P_DIA) {
        if (isNaN(global.x1)) {
            error("CIRCLE", qsTr("This should never happen."));
        }
        else {
            global.x2 = x;
            global.y2 = y;
            setRubberPoint("CIRCLE_DIAMETER", global.x2, global.y2);
            vulcanize();
            appendPromptHistory();
            endCommand();
        }
    }
    else if (global.mode == global.mode_2P) {
        if (isNaN(global.x1)) {
            global.x1 = x;
            global.y1 = y;
            addRubber("CIRCLE");
            setRubberMode("CIRCLE_2P");
            setRubberPoint("CIRCLE_TAN1", global.x1, global.y1);
            appendPromptHistory();
            setPromptPrefix(qsTr("Specify second end point of circle's diameter: "));
        }
        else if (isNaN(global.x2)) {
            global.x2 = x;
            global.y2 = y;
            setRubberPoint("CIRCLE_TAN2", global.x2, global.y2);
            vulcanize();
            appendPromptHistory();
            endCommand();
        }
        else {
            error("CIRCLE", qsTr("This should never happen."));
        }
    }
    else if (global.mode == global.mode_3P) {
        if (isNaN(global.x1)) {
            global.x1 = x;
            global.y1 = y;
            appendPromptHistory();
            setPromptPrefix(qsTr("Specify second point on circle: "));
        }
        else if (isNaN(global.x2)) {
            global.x2 = x;
            global.y2 = y;
            addRubber("CIRCLE");
            setRubberMode("CIRCLE_3P");
            setRubberPoint("CIRCLE_TAN1", global.x1, global.y1);
            setRubberPoint("CIRCLE_TAN2", global.x2, global.y2);
            appendPromptHistory();
            setPromptPrefix(qsTr("Specify third point on circle: "));
        }
        else if (isNaN(global.x3)) {
            global.x3 = x;
            global.y3 = y;
            setRubberPoint("CIRCLE_TAN3", global.x3, global.y3);
            vulcanize();
            appendPromptHistory();
            endCommand();
        }
        else {
            error("CIRCLE", qsTr("This should never happen."));
        }
    }
    else if (global.mode == global.mode_TTR) {
        if (isNaN(global.x1)) {
            global.x1 = x;
            global.y1 = y;
            appendPromptHistory();
            setPromptPrefix(qsTr("Specify point on object for second tangent of circle: "));
        }
        else if (isNaN(global.x2)) {
            global.x2 = x;
            global.y2 = y;
            appendPromptHistory();
            setPromptPrefix(qsTr("Specify radius of circle: "));
        }
        else if (isNaN(global.x3)) {
            global.x3 = x;
            global.y3 = y;
            appendPromptHistory();
            setPromptPrefix(qsTr("Specify second point: "));
        }
        else {
            todo("CIRCLE", "click() for TTR");
        }
    }
}

//NOTE: context() is run when a context menu entry is chosen.
function context(str)
{
    todo("CIRCLE", "context()");
}

//NOTE: prompt() is run when Enter is pressed.
//      appendPromptHistory is automatically called before prompt()
//      is called so calling it is only needed for erroneous input.
//      Any text in the command prompt is sent as an uppercase string.
function prompt(str)
{
    if (global.mode == global.mode_1P_RAD) {
        if (isNaN(global.x1)) {
            if (str == "2P") {
                global.mode = global.mode_2P;
                setPromptPrefix(qsTr("Specify first end point of circle's diameter: "));
            }
            else if (str == "3P") {
                global.mode = global.mode_3P;
                setPromptPrefix(qsTr("Specify first point of circle: "));
            }
            else if (str == "T" || str == "TTR") {
                global.mode = global.mode_TTR;
                setPromptPrefix(qsTr("Specify point on object for first tangent of circle: "));
            }
            else {
                var strList = str.split(",");
                if (isNaN(strList[0]) || isNaN(strList[1])) {
                    alert(qsTr("Point or option keyword required."));
                    setPromptPrefix(qsTr("Specify center point for circle or [3P/2P/Ttr (tan tan radius)]: "));
                }
                else {
                    global.x1 = Number(strList[0]);
                    global.y1 = Number(strList[1]);
                    global.cx = global.x1;
                    global.cy = global.y1;
                    addRubber("CIRCLE");
                    setRubberMode("CIRCLE_1P_RAD");
                    setRubberPoint("CIRCLE_CENTER", global.cx, global.cy);
                    setPromptPrefix(qsTr("Specify radius of circle or [Diameter]: "));
                }
            }
        }
        else {
            if (str == "D" || str == "DIAMETER") {
                global.mode = global.mode_1P_DIA;
                setRubberMode("CIRCLE_1P_DIA");
                setPromptPrefix(qsTr("Specify diameter of circle: "));
            }
            else {
                var num = Number(str);
                if (isNaN(num)) {
                    alert(qsTr("Requires numeric radius, point on circumference, or \"D\"."));
                    setPromptPrefix(qsTr("Specify radius of circle or [Diameter]: "));
                }
                else {
                    global.rad = num;
                    global.x2 = global.x1 + global.rad;
                    global.y2 = global.y1;
                    setRubberPoint("CIRCLE_RADIUS", global.x2, global.y2);
                    vulcanize();
                    endCommand();
                }
            }
        }
    }
    else if (global.mode == global.mode_1P_DIA) {
        if (isNaN(global.x1)) {
            error("CIRCLE", qsTr("This should never happen."));
        }
        if (isNaN(global.x2)) {
            var num = Number(str);
            if (isNaN(num)) {
                alert(qsTr("Requires numeric distance or second point."));
                setPromptPrefix(qsTr("Specify diameter of circle: "));
            }
            else {
                global.dia = num;
                global.x2 = global.x1 + global.dia;
                global.y2 = global.y1;
                setRubberPoint("CIRCLE_DIAMETER", global.x2, global.y2);
                vulcanize();
                endCommand();
            }
        }
        else {
            error("CIRCLE", qsTr("This should never happen."));
        }
    }
    else if (global.mode == global.mode_2P) {
        if (isNaN(global.x1)) {
            var strList = str.split(",");
            if (isNaN(strList[0]) || isNaN(strList[1])) {
                alert(qsTr("Invalid point."));
                setPromptPrefix(qsTr("Specify first end point of circle's diameter: "));
            }
            else {
                global.x1 = Number(strList[0]);
                global.y1 = Number(strList[1]);
                addRubber("CIRCLE");
                setRubberMode("CIRCLE_2P");
                setRubberPoint("CIRCLE_TAN1", global.x1, global.y1);
                setPromptPrefix(qsTr("Specify second end point of circle's diameter: "));
            }
        }
        else if (isNaN(global.x2)) {
            var strList = str.split(",");
            if (isNaN(strList[0]) || isNaN(strList[1])) {
                alert(qsTr("Invalid point."));
                setPromptPrefix(qsTr("Specify second end point of circle's diameter: "));
            }
            else {
                global.x2 = Number(strList[0]);
                global.y2 = Number(strList[1]);
                setRubberPoint("CIRCLE_TAN2", global.x2, global.y2);
                vulcanize();
                endCommand();
            }
        }
        else {
            error("CIRCLE", qsTr("This should never happen."));
        }
    }
    else if (global.mode == global.mode_3P) {
        if (isNaN(global.x1)) {
            var strList = str.split(",");
            if (isNaN(strList[0]) || isNaN(strList[1])) {
                alert(qsTr("Invalid point."));
                setPromptPrefix(qsTr("Specify first point of circle: "));
            }
            else {
                global.x1 = Number(strList[0]);
                global.y1 = Number(strList[1]);
                setPromptPrefix(qsTr("Specify second point of circle: "));
            }
        }
        else if (isNaN(global.x2)) {
            var strList = str.split(",");
            if (isNaN(strList[0]) || isNaN(strList[1])) {
                alert(qsTr("Invalid point."));
                setPromptPrefix(qsTr("Specify second point of circle: "));
            }
            else {
                global.x2 = Number(strList[0]);
                global.y2 = Number(strList[1]);
                addRubber("CIRCLE");
                setRubberMode("CIRCLE_3P");
                setRubberPoint("CIRCLE_TAN1", global.x1, global.y1);
                setRubberPoint("CIRCLE_TAN2", global.x2, global.y2);
                setPromptPrefix(qsTr("Specify third point of circle: "));
            }
        }
        else if (isNaN(global.x3)) {
            var strList = str.split(",");
            if (isNaN(strList[0]) || isNaN(strList[1])) {
                alert(qsTr("Invalid point."));
                setPromptPrefix(qsTr("Specify third point of circle: "));
            }
            else {                
                global.x3 = Number(strList[0]);
                global.y3 = Number(strList[1]);
                setRubberPoint("CIRCLE_TAN3", global.x3, global.y3);
                vulcanize();
                endCommand();
            }
        }
        else {
            error("CIRCLE", qsTr("This should never happen."));
        }
        
    }
    else if (global.mode == global.mode_TTR) {
        todo("CIRCLE", "prompt() for TTR");
    }
}

==> commands/distance.cpp <==
//Command: Distance

var global = {}; //Required
global.x1;
global.y1;
global.x2;
global.y2;

//NOTE: main() is run every time the command is started.
//      Use it to reset variables so they are ready to go.
function main()
{
    initCommand();
    clearSelection();
    global.x1 = NaN;
    global.y1 = NaN;
    global.x2 = NaN;
    global.y2 = NaN;
    setPromptPrefix(qsTr("Specify first point: "));
}

//NOTE: click() is run only for left clicks.
//      Middle clicks are used for panning.
//      Right clicks bring up the context menu.
function click(x, y)
{
    if (isNaN(global.x1)) {
        global.x1 = x;
        global.y1 = y;
        addRubber("LINE");
        setRubberMode("LINE");
        setRubberPoint("LINE_START", global.x1, global.y1);
        appendPromptHistory();
        setPromptPrefix(qsTr("Specify second point: "));
    }
    else {
        appendPromptHistory();
        global.x2 = x;
        global.y2 = y;
        reportDistance();
        endCommand();
    }
}

//NOTE: context() is run when a context menu entry is chosen.
function context(str)
{
    todo("DISTANCE", "context()");
}

//NOTE: prompt() is run when Enter is pressed.
//      appendPromptHistory is automatically called before prompt()
//      is called so calling it is only needed for erroneous input.
//      Any text in the command prompt is sent as an uppercase string.
function prompt(str)
{
    var strList = str.split(",");
    if (isNaN(global.x1)) {
        if (isNaN(strList[0]) || isNaN(strList[1])) {
            alert(qsTr("Requires numeric distance or two points."));
            setPromptPrefix(qsTr("Specify first point: "));
        }
        else {
            global.x1 = Number(strList[0]);
            global.y1 = Number(strList[1]);
            addRubber("LINE");
            setRubberMode("LINE");
            setRubberPoint("LINE_START", global.x1, global.y1);
            setPromptPrefix(qsTr("Specify second point: "));
        }
    }
    else {
        if (isNaN(strList[0]) || isNaN(strList[1])) {
            alert(qsTr("Requires numeric distance or two points."));
            setPromptPrefix(qsTr("Specify second point: "));
        }
        else {
            global.x2 = Number(strList[0]);
            global.y2 = Number(strList[1]);
            reportDistance();
            endCommand();
        }
    }
}

//Cartesian Coordinate System reported:
//
//               (+)
//               90
//               |
//      (-) 180__|__0 (+)
//               |
//              270
//              (-)

function reportDistance()
{
    var dx = global.x2 - global.x1;
    var dy = global.y2 - global.y1;

    var dist = calculateDistance(global.x1,global.y1,global.x2, global.y2);
    var angle = calculateAngle(global.x1,global.y1,global.x2, global.y2);

    setPromptPrefix(qsTr("Distance") + " = " + dist.toString() + ", " + qsTr("Angle") + " = " + angle.toString());
    appendPromptHistory();
    setPromptPrefix(qsTr("Delta X") + " = " + dx.toString() + ", " + qsTr("Delta Y") + " = " + dy.toString());
    appendPromptHistory();
}

==> commands/dolphin.cpp <==
//Command: Dolphin

var global = {}; //Required
global.numPoints = 512; //Default //TODO: min:64 max:8192
global.cx;
global.cy;
global.sx = 0.04; //Default
global.sy = 0.04; //Default
global.numPoints;
global.mode;

//enums
global.mode_NUM_POINTS = 0;
global.mode_XSCALE     = 1;
global.mode_YSCALE     = 2;

//NOTE: main() is run every time the command is started.
//      Use it to reset variables so they are ready to go.
function main()
{
    initCommand();
    clearSelection();
    global.cx = NaN;
    global.cy = NaN;
    global.mode = global.mode_NUM_POINTS;

    addRubber("POLYGON");
    setRubberMode("POLYGON");
    updateDolphin(global.numPoints, global.sx, global.sy);
    spareRubber("POLYGON");
    endCommand();
}

//NOTE: click() is run only for left clicks.
//      Middle clicks are used for panning.
//      Right clicks bring up the context menu.
function click(x, y)
{
}

//NOTE: move() is optional. It is run only after
//      enableMoveRapidFire() is called. It
//      will be called every time the mouse moves until
//      disableMoveRapidFire() is called.
function move(x, y)
{
}

//NOTE: context() is run when a context menu entry is chosen.
function context(str)
{
    todo("DOLPHIN", "context()");
}

//NOTE: prompt() is run when Enter is pressed.
//      appendPromptHistory is automatically called before prompt()
//      is called so calling it is only needed for erroneous input.
//      Any text in the command prompt is sent as an uppercase string.
function prompt(str)
{
}

function updateDolphin(numPts, xScale, yScale)
{
    var i;
    var t;
    var xx = NaN;
    var yy = NaN;
    var two_pi = 2*Math.PI;

    for(i = 0; i <= numPts; i++) {
        t = two_pi/numPts*i; 

        xx = 4/23*Math.sin(62/33-58*t)+
        8/11*Math.sin(10/9-56*t)+
        17/24*Math.sin(38/35-55*t)+
        30/89*Math.sin(81/23-54*t)+
        3/17*Math.sin(53/18-53*t)+
        21/38*Math.sin(29/19-52*t)+
        11/35*Math.sin(103/40-51*t)+
        7/16*Math.sin(79/18-50*t)+
        4/15*Math.sin(270/77-49*t)+
        19/35*Math.sin(59/27-48*t)+
        37/43*Math.sin(71/17-47*t)+
        Math.sin(18/43-45*t)+
        21/26*Math.sin(37/26-44*t)+
        27/19*Math.sin(111/32-42*t)+
        8/39*Math.sin(13/25-41*t)+
        23/30*Math.sin(27/8-40*t)+
        23/21*Math.sin(32/35-37*t)+
        18/37*Math.sin(91/31-36*t)+
        45/22*Math.sin(29/37-35*t)+
        56/45*Math.sin(11/8-33*t)+
        4/7*Math.sin(32/19-32*t)+
        54/23*Math.sin(74/29-31*t)+
        28/19*Math.sin(125/33-30*t)+
        19/9*Math.sin(73/27-29*t)+
        16/17*Math.sin(737/736-28*t)+
        52/33*Math.sin(130/29-27*t)+
        41/23*Math.sin(43/30-25*t)+
        29/20*Math.sin(67/26-24*t)+
        64/25*Math.sin(136/29-23*t)+
        162/37*Math.sin(59/34-21*t)+
        871/435*Math.sin(199/51-20*t)+
        61/42*Math.sin(58/17-19*t)+
        159/25*Math.sin(77/31-17*t)+
        241/15*Math.sin(94/31-13*t)+
        259/18*Math.sin(114/91-12*t)+
        356/57*Math.sin(23/25-11*t)+
        2283/137*Math.sin(23/25-10*t)+
        1267/45*Math.sin(139/42-9*t)+
        613/26*Math.sin(41/23-8*t)+
        189/16*Math.sin(122/47-6*t)+
        385/6*Math.sin(151/41-5*t)+
        2551/38*Math.sin(106/35-4*t)+
        1997/18*Math.sin(6/5-2*t)+
        43357/47*Math.sin(81/26-t)-
        4699/35*Math.sin(3*t+25/31)-
        1029/34*Math.sin(7*t+20/21)-
        250/17*Math.sin(14*t+7/40)-
        140/17*Math.sin(15*t+14/25)-
        194/29*Math.sin(16*t+29/44)-
        277/52*Math.sin(18*t+37/53)-
        94/41*Math.sin(22*t+33/31)-
        57/28*Math.sin(26*t+44/45)-
        128/61*Math.sin(34*t+11/14)-
        111/95*Math.sin(38*t+55/37)-
        85/71*Math.sin(39*t+4/45)-
        25/29*Math.sin(43*t+129/103)-
        7/37*Math.sin(46*t+9/20)-
        17/32*Math.sin(57*t+11/28)-
        5/16*Math.sin(59*t+32/39);

        yy = 5/11*Math.sin(163/37-59*t)+
        7/22*Math.sin(19/41-58*t)+
        30/41*Math.sin(1-57*t)+
        37/29*Math.sin(137/57-56*t)+
        5/7*Math.sin(17/6-55*t)+
        11/39*Math.sin(46/45-52*t)+
        25/28*Math.sin(116/83-51*t)+
        25/34*Math.sin(11/20-47*t)+
        8/27*Math.sin(81/41-46*t)+
        44/39*Math.sin(78/37-45*t)+
        11/25*Math.sin(107/37-44*t)+
        7/20*Math.sin(7/16-41*t)+
        30/31*Math.sin(19/5-40*t)+
        37/27*Math.sin(148/59-39*t)+
        44/39*Math.sin(17/27-38*t)+
        13/11*Math.sin(7/11-37*t)+
        28/33*Math.sin(119/39-36*t)+
        27/13*Math.sin(244/81-35*t)+
        13/23*Math.sin(113/27-34*t)+
        47/38*Math.sin(127/32-33*t)+
        155/59*Math.sin(173/45-29*t)+
        105/37*Math.sin(22/43-27*t)+
        106/27*Math.sin(23/37-26*t)+
        97/41*Math.sin(53/29-25*t)+
        83/45*Math.sin(109/31-24*t)+
        81/31*Math.sin(96/29-23*t)+
        56/37*Math.sin(29/10-22*t)+
        44/13*Math.sin(29/19-19*t)+
        18/5*Math.sin(34/31-18*t)+
        163/51*Math.sin(75/17-17*t)+
        152/31*Math.sin(61/18-16*t)+
        146/19*Math.sin(47/20-15*t)+
        353/35*Math.sin(55/48-14*t)+
        355/28*Math.sin(102/25-12*t)+
        1259/63*Math.sin(71/18-11*t)+
        17/35*Math.sin(125/52-10*t)+
        786/23*Math.sin(23/26-6*t)+
        2470/41*Math.sin(77/30-5*t)+
        2329/47*Math.sin(47/21-4*t)+
        2527/33*Math.sin(23/14-3*t)+
        9931/33*Math.sin(51/35-2*t)-
        11506/19*Math.sin(t+56/67)-
        2081/42*Math.sin(7*t+9/28)-
        537/14*Math.sin(8*t+3/25)-
        278/29*Math.sin(9*t+23/33)-
        107/15*Math.sin(13*t+35/26)-
        56/19*Math.sin(20*t+5/9)-
        5/9*Math.sin(21*t+1/34)-
        17/24*Math.sin(28*t+36/23)-
        21/11*Math.sin(30*t+27/37)-
        138/83*Math.sin(31*t+1/7)-
        10/17*Math.sin(32*t+29/48)-
        31/63*Math.sin(42*t+27/28)-
        4/27*Math.sin(43*t+29/43)-
        13/24*Math.sin(48*t+5/21)-
        4/7*Math.sin(49*t+29/23)-
        26/77*Math.sin(50*t+29/27)-
        19/14*Math.sin(53*t+61/48)+
        34/25*Math.sin(54*t+37/26);

        setRubberPoint("POLYGON_POINT_" + i.toString(), xx*xScale, yy*yScale);
    }

    setRubberText("POLYGON_NUM_POINTS", numPts.toString());
}

==> commands/ellipse.cpp <==
//Command: Ellipse

var global = {}; //Required
global.x1;
global.y1;
global.x2;
global.y2;
global.x3;
global.y3;
global.cx;
global.cy;
global.width;
global.height;
global.rot;
global.mode;

//enums
global.mode_MAJORDIAMETER_MINORRADIUS = 0;
global.mode_MAJORRADIUS_MINORRADIUS   = 1;
global.mode_ELLIPSE_ROTATION          = 2;

//NOTE: main() is run every time the command is started.
//      Use it to reset variables so they are ready to go.
function main()
{
    initCommand();
    clearSelection();
    global.mode = global.mode_MAJORDIAMETER_MINORRADIUS;
    global.x1      = NaN;
    global.y1      = NaN;
    global.x2      = NaN;
    global.y2      = NaN;
    global.x3      = NaN;
    global.y3      = NaN;
    setPromptPrefix(qsTr("Specify first axis start point or [Center]: "));
}

//NOTE: click() is run only for left clicks.
//      Middle clicks are used for panning.
//      Right clicks bring up the context menu.
function click(x, y)
{
    if (global.mode == global.mode_MAJORDIAMETER_MINORRADIUS) {
        if (isNaN(global.x1)) {
            global.x1 = x;
            global.y1 = y;
            addRubber("ELLIPSE");
            setRubberMode("ELLIPSE_LINE");
            setRubberPoint("ELLIPSE_LINE_POINT1", global.x1, global.y1);
            appendPromptHistory();
            setPromptPrefix(qsTr("Specify first axis end point: "));
        }
        else if (isNaN(global.x2)) {
            global.x2 = x;
            global.y2 = y;
            global.cx = (global.x1 + global.x2)/2.0;
            global.cy = (global.y1 + global.y2)/2.0;
            global.width = calculateDistance(global.x1, global.y1, global.x2, global.y2);
            global.rot = calculateAngle(global.x1, global.y1, global.x2, global.y2);
            setRubberMode("ELLIPSE_MAJORDIAMETER_MINORRADIUS");
            setRubberPoint("ELLIPSE_AXIS1_POINT1", global.x1, global.y1);
            setRubberPoint("ELLIPSE_AXIS1_POINT2", global.x2, global.y2);
            setRubberPoint("ELLIPSE_CENTER", global.cx, global.cy);
            setRubberPoint("ELLIPSE_WIDTH", global.width, 0);
            setRubberPoint("ELLIPSE_ROT", global.rot, 0);
            appendPromptHistory();
            setPromptPrefix(qsTr("Specify second axis end point or [Rotation]: "));
        }
        else if (isNaN(global.x3)) {
            global.x3 = x;
            global.y3 = y;
            global.height = perpendicularDistance(global.x3, global.y3, global.x1, global.y1, global.x2, global.y2)*2.0;
            setRubberPoint("ELLIPSE_AXIS2_POINT2", global.x3, global.y3);
            vulcanize();
            appendPromptHistory();
            endCommand();
        }
        else {
            error("ELLIPSE", qsTr("This should never happen."));
        }
    }
    else if (global.mode == global.mode_MAJORRADIUS_MINORRADIUS) {
        if (isNaN(global.x1)) {
            global.x1 = x;
            global.y1 = y;
            global.cx = global.x1;
            global.cy = global.y1;
            addRubber("ELLIPSE");
            setRubberMode("ELLIPSE_LINE");
            setRubberPoint("ELLIPSE_LINE_POINT1", global.x1, global.y1);
            setRubberPoint("ELLIPSE_CENTER", global.cx, global.cy);
            appendPromptHistory();
            setPromptPrefix(qsTr("Specify first axis end point: "));
        }
        else if (isNaN(global.x2)) {
            global.x2 = x;
            global.y2 = y;
            global.width = calculateDistance(global.cx, global.cy, global.x2, global.y2)*2.0;
            global.rot = calculateAngle(global.x1, global.y1, global.x2, global.y2);
            setRubberMode("ELLIPSE_MAJORRADIUS_MINORRADIUS");
            setRubberPoint("ELLIPSE_AXIS1_POINT2", global.x2, global.y2);
            setRubberPoint("ELLIPSE_WIDTH", global.width, 0);
            setRubberPoint("ELLIPSE_ROT", global.rot, 0);
            appendPromptHistory();
            setPromptPrefix(qsTr("Specify second axis end point or [Rotation]: "));
        }
        else if (isNaN(global.x3)) {
            global.x3 = x;
            global.y3 = y;
            global.height = perpendicularDistance(global.x3, global.y3, global.cx, global.cy, global.x2, global.y2)*2.0;
            setRubberPoint("ELLIPSE_AXIS2_POINT2", global.x3, global.y3);
            vulcanize();
            appendPromptHistory();
            endCommand();
        }
        else {
            error("ELLIPSE", qsTr("This should never happen."));
        }
    }
    else if (global.mode == global.mode_ELLIPSE_ROTATION) {
        if (isNaN(global.x1)) {
            error("ELLIPSE", qsTr("This should never happen."));
        }
        else if (isNaN(global.x2)) {
            error("ELLIPSE", qsTr("This should never happen."));
        }
        else if (isNaN(global.x3)) {
            var angle = calculateAngle(global.cx, global.cy, x, y);
            global.height = Math.cos(angle*Math.PI/180.0)*global.width;
            addEllipse(global.cx, global.cy, global.width, global.height, global.rot, false);
            appendPromptHistory();
            endCommand();
        }
    }
}

//NOTE: context() is run when a context menu entry is chosen.
function context(str)
{
    todo("ELLIPSE", "context()");
}

//NOTE: prompt() is run when Enter is pressed.
//      appendPromptHistory is automatically called before prompt()
//      is called so calling it is only needed for erroneous input.
//      Any text in the command prompt is sent as an uppercase string.
function prompt(str)
{
    if (global.mode == global.mode_MAJORDIAMETER_MINORRADIUS) {
        if (isNaN(global.x1)) {
            if (str == "C" || str == "CENTER") {
                global.mode = global.mode_MAJORRADIUS_MINORRADIUS;
                setPromptPrefix(qsTr("Specify center point: "));
            }
            else {
                var strList = str.split(",");
                if (isNaN(strList[0]) || isNaN(strList[1])) {
                    alert(qsTr("Point or option keyword required."));
                    setPromptPrefix(qsTr("Specify first axis start point or [Center]: "));
                }
                else {
                    global.x1 = Number(strList[0]);
                    global.y1 = Number(strList[1]);
                    addRubber("ELLIPSE");
                    setRubberMode("ELLIPSE_LINE");
                    setRubberPoint("ELLIPSE_LINE_POINT1", global.x1, global.y1);
                    setPromptPrefix(qsTr("Specify first axis end point: "));
                }
            }
        }
        else if (isNaN(global.x2)) {
            var strList = str.split(",");
            if (isNaN(strList[0]) || isNaN(strList[1])) {
                alert(qsTr("Invalid point."));
                setPromptPrefix(qsTr("Specify first axis end point: "));
            }
            else {
                global.x2 = Number(strList[0]);
                global.y2 = Number(strList[1]);
                global.cx = (global.x1 + global.x2)/2.0;
                global.cy = (global.y1 + global.y2)/2.0;
                global.width = calculateDistance(global.x1, global.y1, global.x2, global.y2);
                global.rot = calculateAngle(global.x1, global.y1, global.x2, global.y2);
                setRubberMode("ELLIPSE_MAJORDIAMETER_MINORRADIUS");
                setRubberPoint("ELLIPSE_AXIS1_POINT1", global.x1, global.y1);
                setRubberPoint("ELLIPSE_AXIS1_POINT2", global.x2, global.y2);
                setRubberPoint("ELLIPSE_CENTER", global.cx, global.cy);
                setRubberPoint("ELLIPSE_WIDTH", global.width, 0);
                setRubberPoint("ELLIPSE_ROT", global.rot, 0);
                setPromptPrefix(qsTr("Specify second axis end point or [Rotation]: "));
            }
        }
        else if (isNaN(global.x3)) {
            if (str == "R" || str == "ROTATION") {
                global.mode = global.mode_ELLIPSE_ROTATION;
                setPromptPrefix(qsTr("Specify rotation: "));
            }
            else {
                var strList = str.split(",");
                if (isNaN(strList[0]) || isNaN(strList[1])) {
                    alert(qsTr("Point or option keyword required."));
                    setPromptPrefix(qsTr("Specify second axis end point or [Rotation]: "));
                }
                else {
                    global.x3 = Number(strList[0]);
                    global.y3 = Number(strList[1]);
                    global.height = perpendicularDistance(global.x3, global.y3, global.x1, global.y1, global.x2, global.y2)*2.0;
                    setRubberPoint("ELLIPSE_AXIS2_POINT2", global.x3, global.y3);
                    vulcanize();
                    endCommand();
                }
            }
        }
    }
    else if (global.mode == global.mode_MAJORRADIUS_MINORRADIUS) {
        if (isNaN(global.x1)) {
            var strList = str.split(",");
            if (isNaN(strList[0]) || isNaN(strList[1])) {
                alert(qsTr("Invalid point."));
                setPromptPrefix(qsTr("Specify center point: "));
            }
            else {
                global.x1 = Number(strList[0]);
                global.y1 = Number(strList[1]);
                global.cx = global.x1;
                global.cy = global.y1;
                addRubber("ELLIPSE");
                setRubberMode("ELLIPSE_LINE");
                setRubberPoint("ELLIPSE_LINE_POINT1", global.x1, global.y1);
                setRubberPoint("ELLIPSE_CENTER", global.cx, global.cy);
                setPromptPrefix(qsTr("Specify first axis end point: "));
            }
        }
        else if (isNaN(global.x2)) {
            var strList = str.split(",");
            if (isNaN(strList[0]) || isNaN(strList[1])) {
                alert(qsTr("Invalid point."));
                setPromptPrefix(qsTr("Specify first axis end point: "));
            }
            else {
                global.x2 = Number(strList[0]);
                global.y2 = Number(strList[1]);
                global.width = calculateDistance(global.x1, global.y1, global.x2, global.y2)*2.0;
                global.rot = calculateAngle(global.x1, global.y1, global.x2, global.y2);
                setRubberMode("ELLIPSE_MAJORRADIUS_MINORRADIUS");
                setRubberPoint("ELLIPSE_AXIS1_POINT2", global.x2, global.y2);
                setRubberPoint("ELLIPSE_WIDTH", global.width, 0);
                setRubberPoint("ELLIPSE_ROT", global.rot, 0);
                setPromptPrefix(qsTr("Specify second axis end point or [Rotation]: "));
            }
        }
        else if (isNaN(global.x3)) {
            if (str == "R" || str == "ROTATION") {
                global.mode = global.mode_ELLIPSE_ROTATION;
                setPromptPrefix(qsTr("Specify ellipse rotation: "));
            }
            else {
                var strList = str.split(",");
                if (isNaN(strList[0]) || isNaN(strList[1])) {
                    alert(qsTr("Point or option keyword required."));
                    setPromptPrefix(qsTr("Specify second axis end point or [Rotation]: "));
                }
                else {
                    global.x3 = Number(strList[0]);
                    global.y3 = Number(strList[1]);
                    global.height = perpendicularDistance(global.x3, global.y3, global.x1, global.y1, global.x2, global.y2)*2.0;
                    setRubberPoint("ELLIPSE_AXIS2_POINT2", global.x3, global.y3);
                    vulcanize();
                    endCommand();
                }
            }
        }
    }
    else if (global.mode == global.mode_ELLIPSE_ROTATION) {
        if (isNaN(global.x1)) {
            error("ELLIPSE", qsTr("This should never happen."));
        }
        else if (isNaN(global.x2)) {
            error("ELLIPSE", qsTr("This should never happen."));
        }
        else if (isNaN(global.x3)) {
            if (isNaN(str)) {
                alert(qsTr("Invalid angle. Input a numeric angle or pick a point."));
                setPromptPrefix(qsTr("Specify rotation: "));
            }
            else {
                var angle = Number(str);
                global.height = Math.cos(angle*Math.PI/180.0)*global.width;
                addEllipse(global.cx, global.cy, global.width, global.height, global.rot, false);
                endCommand();
            }
        }
    }
}

==> commands/erase.cpp <==
//Command: Erase/Delete

//NOTE: main() is run every time the command is started.
//      Use it to reset variables so they are ready to go.
function main()
{
    initCommand();

    if (numSelected() <= 0) {
        //TODO: Prompt to select objects if nothing is preselected
        alert(qsTr("Preselect objects before invoking the delete command."));
        endCommand();
        messageBox("information", qsTr("Delete Preselect"), qsTr("Preselect objects before invoking the delete command."));
    }
    else {
        deleteSelected();
        endCommand();
    }
}

//NOTE: click() is run only for left clicks.
//      Middle clicks are used for panning.
//      Right clicks bring up the context menu.
function click(x, y)
{
    deleteSelected();
    endCommand();
}

//NOTE: context() is run when a context menu entry is chosen.
function context(str)
{
    deleteSelected();
    endCommand();
}

//NOTE: prompt() is run when Enter is pressed.
//      appendPromptHistory is automatically called before prompt()
//      is called so calling it is only needed for erroneous input.
//      Any text is in the command prompt is sent as an uppercase string.
function prompt(str)
{
    deleteSelected();
    endCommand();
}

==> commands/exit.cpp <==
//Command: Exit

//NOTE: main() is run every time the command is started.
//      Use it to reset variables so they are ready to go.
function main()
{
    initCommand();
    clearSelection();
    exit();
    endCommand();
}

//NOTE: click() is run only for left clicks.
//      Middle clicks are used for panning.
//      Right clicks bring up the context menu.
function click(x, y)
{
    exit();
    endCommand();
}

//NOTE: context() is run when a context menu entry is chosen.
function context(str)
{
    exit();
    endCommand();
}

//NOTE: prompt() is run when Enter is pressed.
//      appendPromptHistory is automatically called before prompt()
//      is called so calling it is only needed for erroneous input.
//      Any text is in the command prompt is sent as an uppercase string.
function prompt(str)
{
    exit();
    endCommand();
}

==> commands/heart.cpp <==
//Command: Heart

var global = {}; //Required
global.numPoints = 512; //Default //TODO: min:64 max:8192
global.cx;
global.cy;
global.sx = 1.0;
global.sy = 1.0;
global.numPoints;
global.mode;

//enums
global.mode_NUM_POINTS = 0;
global.mode_STYLE      = 1;
global.mode_XSCALE     = 2;
global.mode_YSCALE     = 3;

//NOTE: main() is run every time the command is started.
//      Use it to reset variables so they are ready to go.
function main()
{
    initCommand();
    clearSelection();
    global.cx = NaN;
    global.cy = NaN;
    global.mode = global.mode_NUM_POINTS;

    //Heart4: 10.0 / 512
    //Heart5: 1.0 / 512

    addRubber("POLYGON");
    setRubberMode("POLYGON");
    updateHeart("HEART5", global.numPoints, global.sx, global.sy);
    spareRubber("POLYGON");
    endCommand();
}

//NOTE: click() is run only for left clicks.
//      Middle clicks are used for panning.
//      Right clicks bring up the context menu.
function click(x, y)
{
}

//NOTE: move() is optional. It is run only after
//      enableMoveRapidFire() is called. It
//      will be called every time the mouse moves until
//      disableMoveRapidFire() is called.
function move(x, y)
{
}

//NOTE: context() is run when a context menu entry is chosen.
function context(str)
{
    todo("HEART", "context()");
}

//NOTE: prompt() is run when Enter is pressed.
//      appendPromptHistory is automatically called before prompt()
//      is called so calling it is only needed for erroneous input.
//      Any text in the command prompt is sent as an uppercase string.
function prompt(str)
{
}

function updateHeart(style, numPts, xScale, yScale)
{
    var i;
    var t;
    var xx = NaN;
    var yy = NaN;
    var two_pi = 2*Math.PI;

    for(i = 0; i <= numPts; i++) {
        t = two_pi/numPts*i; 

        if (style == "HEART4") {
            xx = Math.cos(t)*((Math.sin(t)*Math.sqrt(Math.abs(Math.cos(t))))/(Math.sin(t)+7/5) - 2*Math.sin(t) + 2);
            yy = Math.sin(t)*((Math.sin(t)*Math.sqrt(Math.abs(Math.cos(t))))/(Math.sin(t)+7/5) - 2*Math.sin(t) + 2);
        }
        else if (style == "HEART5") {
            xx = 16*Math.pow(Math.sin(t), 3);
            yy = 13*Math.cos(t) - 5*Math.cos(2*t) - 2*Math.cos(3*t) - Math.cos(4*t);
        }

        setRubberPoint("POLYGON_POINT_" + i.toString(), xx*xScale, yy*yScale);
    }

    setRubberText("POLYGON_NUM_POINTS", numPts.toString());
}

==> commands/help.cpp <==
//Command: Help

//NOTE: main() is run every time the command is started.
//      Use it to reset variables so they are ready to go.
function main()
{
    initCommand();
    clearSelection();
    help();
    endCommand();
}

//NOTE: click() is run only for left clicks.
//      Middle clicks are used for panning.
//      Right clicks bring up the context menu.
function click(x, y)
{
    help();
    endCommand();
}

//NOTE: context() is run when a context menu entry is chosen.
function context(str)
{
    help();
    endCommand();
}

//NOTE: prompt() is run when Enter is pressed.
//      appendPromptHistory is automatically called before prompt()
//      is called so calling it is only needed for erroneous input.
//      Any text is in the command prompt is sent as an uppercase string.
function prompt(str)
{
    help();
    endCommand();
}

==> commands/line.cpp <==
//Command: Line

var global = {}; //Required
global.firstRun;
global.firstX;
global.firstY;
global.prevX;
global.prevY;

//NOTE: main() is run every time the command is started.
//      Use it to reset variables so they are ready to go.
function main()
{
    initCommand();
    clearSelection();
    global.firstRun = true;
    global.firstX = NaN;
    global.firstY = NaN;
    global.prevX = NaN;
    global.prevY = NaN;
    setPromptPrefix(qsTr("Specify first point: "));
}

//NOTE: click() is run only for left clicks.
//      Middle clicks are used for panning.
//      Right clicks bring up the context menu.
function click(x, y)
{
    if (global.firstRun) {
        global.firstRun = false;
        global.firstX = x;
        global.firstY = y;
        global.prevX = x;
        global.prevY = y;
        addRubber("LINE");
        setRubberMode("LINE");
        setRubberPoint("LINE_START", global.firstX, global.firstY);
        appendPromptHistory();
        setPromptPrefix(qsTr("Specify next point or [Undo]: "));
    }
    else {
        setRubberPoint("LINE_END", x, y);
        vulcanize();
        addRubber("LINE");
        setRubberMode("LINE");
        setRubberPoint("LINE_START", x, y);
        appendPromptHistory();
        global.prevX = x;
        global.prevY = y;
    }
}

//NOTE: context() is run when a context menu entry is chosen.
function context(str)
{
    todo("LINE", "context()");
}

//NOTE: prompt() is run when Enter is pressed.
//      appendPromptHistory is automatically called before prompt()
//      is called so calling it is only needed for erroneous input.
//      Any text in the command prompt is sent as an uppercase string.
function prompt(str)
{
    if (global.firstRun) {
        var strList = str.split(",");
        if (isNaN(strList[0]) || isNaN(strList[1])) {
            alert(qsTr("Invalid point."));
            setPromptPrefix(qsTr("Specify first point: "));
        }
        else {
            global.firstRun = false;
            global.firstX = Number(strList[0]);
            global.firstY = Number(strList[1]);
            global.prevX = global.firstX;
            global.prevY = global.firstY;
            addRubber("LINE");
            setRubberMode("LINE");
            setRubberPoint("LINE_START", global.firstX, global.firstY);
            setPromptPrefix(qsTr("Specify next point or [Undo]: "));
        }
    }
    else {
        if (str == "U" || str == "UNDO") {
            todo("LINE", "prompt() for UNDO");
        }
        else {
            var strList = str.split(",");
            if (isNaN(strList[0]) || isNaN(strList[1])) {
                alert(qsTr("Point or option keyword required."));
                setPromptPrefix(qsTr("Specify next point or [Undo]: "));
            }
            else {
                var x = Number(strList[0]);
                var y = Number(strList[1]);
                setRubberPoint("LINE_END", x, y);
                vulcanize();
                addRubber("LINE");
                setRubberMode("LINE");
                setRubberPoint("LINE_START", x, y);
                global.prevX = x;
                global.prevY = y;
                setPromptPrefix(qsTr("Specify next point or [Undo]: "));
            }
        }
    }
}

==> commands/locatepoint.cpp <==
//Command: Locate Point

//NOTE: main() is run every time the command is started.
//      Use it to reset variables so they are ready to go.
function main()
{
    initCommand();
    clearSelection();
    setPromptPrefix(qsTr("Specify point: "));
}

//NOTE: click() is run only for left clicks.
//      Middle clicks are used for panning.
//      Right clicks bring up the context menu.
function click(x, y)
{
    appendPromptHistory();
    setPromptPrefix("X = " + x.toString() + ", Y = " + y.toString());
    appendPromptHistory();
    endCommand();
}

//NOTE: context() is run when a context menu entry is chosen.
function context(str)
{
    todo("LOCATEPOINT", "context()");
}

//NOTE: prompt() is run when Enter is pressed.
//      appendPromptHistory is automatically called before prompt()
//      is called so calling it is only needed for erroneous input.
//      Any text in the command prompt is sent as an uppercase string.
function prompt(str)
{
    var strList = str.split(",");
    if (isNaN(strList[0]) || isNaN(strList[1])) {
        alert(qsTr("Invalid point."));
        setPromptPrefix(qsTr("Specify point: "));
    }
    else {
        appendPromptHistory();
        setPromptPrefix("X = " + strList[0].toString() + ", Y = " + strList[1].toString());
        appendPromptHistory();
        endCommand();
    }
}

==> commands/move.cpp <==
//Command: Move

var global = {}; //Required
global.firstRun;
global.baseX;
global.baseY;
global.destX;
global.destY;
global.deltaX;
global.deltaY;

//NOTE: main() is run every time the command is started.
//      Use it to reset variables so they are ready to go.
function main()
{
    initCommand();
    global.firstRun = true;
    global.baseX  = NaN;
    global.baseY  = NaN;
    global.destX  = NaN;
    global.destY  = NaN;
    global.deltaX = NaN;
    global.deltaY = NaN;

    if (numSelected() <= 0) {
        //TODO: Prompt to select objects if nothing is preselected
        alert(qsTr("Preselect objects before invoking the move command."));
        endCommand();
        messageBox("information", qsTr("Move Preselect"), qsTr("Preselect objects before invoking the move command."));
    }
    else {
        setPromptPrefix(qsTr("Specify base point: "));
    }
}

//NOTE: click() is run only for left clicks.
//      Middle clicks are used for panning.
//      Right clicks bring up the context menu.
function click(x, y)
{
    if (global.firstRun) {
        global.firstRun = false;
        global.baseX = x;
        global.baseY = y;
        addRubber("LINE");
        setRubberMode("LINE");
        setRubberPoint("LINE_START", global.baseX, global.baseY);
        previewOn("SELECTED", "MOVE", global.baseX, global.baseY, 0);
        appendPromptHistory();
        setPromptPrefix(qsTr("Specify destination point: "));
    }
    else {
        global.destX = x;
        global.destY = y;
        global.deltaX = global.destX - global.baseX;
        global.deltaY = global.destY - global.baseY;
        moveSelected(global.deltaX, global.deltaY);
        previewOff();
        endCommand();
    }
}

//NOTE: context() is run when a context menu entry is chosen.
function context(str)
{
    todo("MOVE", "context()");
}

//NOTE: prompt() is run when Enter is pressed.
//      appendPromptHistory is automatically called before prompt()
//      is called so calling it is only needed for erroneous input.
//      Any text in the command prompt is sent as an uppercase string.
function prompt(str)
{
    if (global.firstRun) {
        var strList = str.split(",");
        if (isNaN(strList[0]) || isNaN(strList[1])) {
            alert(qsTr("Invalid point."));
            setPromptPrefix(qsTr("Specify base point: "));
        }
        else {
            global.firstRun = false;
            global.baseX = Number(strList[0]);
            global.baseY = Number(strList[1]);
            addRubber("LINE");
            setRubberMode("LINE");
            setRubberPoint("LINE_START", global.baseX, global.baseY);
            previewOn("SELECTED", "MOVE", global.baseX, global.baseY, 0);
            setPromptPrefix(qsTr("Specify destination point: "));
        }
    }
    else {
        var strList = str.split(",");
        if (isNaN(strList[0]) || isNaN(strList[1])) {
            alert(qsTr("Invalid point."));
            setPromptPrefix(qsTr("Specify destination point: "));
        }
        else {
            global.destX = Number(strList[0]);
            global.destY = Number(strList[1]);
            global.deltaX = global.destX - global.baseX;
            global.deltaY = global.destY - global.baseY;
            moveSelected(global.deltaX, global.deltaY);
            previewOff();
            endCommand();
        }
    }
}

==> commands/path.cpp <==
//Command: Path

//TODO: The path command is currently broken

var global = {}; //Required
global.firstRun;
global.firstX;
global.firstY;
global.prevX;
global.prevY;

//NOTE: main() is run every time the command is started.
//      Use it to reset variables so they are ready to go.
function main()
{
    initCommand();
    clearSelection();
    global.firstRun = true;
    global.firstX = NaN;
    global.firstY = NaN;
    global.prevX = NaN;
    global.prevY = NaN;
    setPromptPrefix(qsTr("Specify start point: "));
}

//NOTE: click() is run only for left clicks.
//      Middle clicks are used for panning.
//      Right clicks bring up the context menu.
function click(x, y)
{
    if (global.firstRun) {
        global.firstRun = false;
        global.firstX = x;
        global.firstY = y;
        global.prevX = x;
        global.prevY = y;
        addPath(x,y);
        appendPromptHistory();
        setPromptPrefix(qsTr("Specify next point or [Arc/Undo]: "));
    }
    else {
        appendPromptHistory();
        appendLineToPath(x,y);
        global.prevX = x;
        global.prevY = y;
    }
}

//NOTE: context() is run when a context menu entry is chosen.
function context(str)
{
    todo("PATH", "context()");
}

//NOTE: prompt() is run when Enter is pressed.
//      appendPromptHistory is automatically called before prompt()
//      is called so calling it is only needed for erroneous input.
//      Any text in the command prompt is sent as an uppercase string.
function prompt(str)
{
    if (str == "A" || str == "ARC")//TODO: Probably should add additional qsTr calls here. {
        todo("PATH", "prompt() for ARC");
    }
    else if (str == "U" || str == "UNDO") {
        todo("PATH", "prompt() for UNDO");
    }
    else {
        var strList = str.split(",");
        if (isNaN(strList[0]) || isNaN(strList[1])) {
            alert(qsTr("Point or option keyword required."));
            setPromptPrefix(qsTr("Specify next point or [Arc/Undo]: "));
        }
        else {
            var x = Number(strList[0]);
            var y = Number(strList[1]);
            if (global.firstRun) {
                global.firstRun = false;
                global.firstX = x;
                global.firstY = y;
                global.prevX = x;
                global.prevY = y;
                addPath(x,y);
                setPromptPrefix(qsTr("Specify next point or [Arc/Undo]: "));
            }
            else {
                appendLineToPath(x,y);
                global.prevX = x;
                global.prevY = y;
            }
        }
    }
}

==> commands/platform.cpp <==
//Command: Platform

//NOTE: main() is run every time the command is started.
//      Use it to reset variables so they are ready to go.
function main()
{
    initCommand();
    clearSelection();
    reportPlatform();
    endCommand();
}

//NOTE: click() is run only for left clicks.
//      Middle clicks are used for panning.
//      Right clicks bring up the context menu.
function click(x, y)
{
    reportPlatform();
    endCommand();
}

//NOTE: context() is run when a context menu entry is chosen.
function context(str)
{
    reportPlatform();
    endCommand();
}

//NOTE: prompt() is run when Enter is pressed.
//      appendPromptHistory is automatically called before prompt()
//      is called so calling it is only needed for erroneous input.
//      Any text is in the command prompt is sent as an uppercase string.
function prompt(str)
{
    reportPlatform();
    endCommand();
}

function reportPlatform()
{
    setPromptPrefix(qsTr("Platform") + " = " + platformString());
    appendPromptHistory();
}
==> commands/point.cpp <==
//Command: Point

var global = {}; //Required
global.firstRun;

//NOTE: main() is run every time the command is started.
//      Use it to reset variables so they are ready to go.
function main()
{
    initCommand();
    clearSelection();
    global.firstRun = true;
    setPromptPrefix("TODO: Current point settings: PDMODE=?  PDSIZE=?"); //TODO: qsTr needed here when complete
    appendPromptHistory();
    setPromptPrefix(qsTr("Specify first point: "));
}

//NOTE: click() is run only for left clicks.
//      Middle clicks are used for panning.
//      Right clicks bring up the context menu.
function click(x, y)
{
    if (global.firstRun) {
        global.firstRun = false;
        appendPromptHistory();
        setPromptPrefix(qsTr("Specify next point: "));
        addPoint(x,y);
    }
    else {
        appendPromptHistory();
        addPoint(x,y);
    }
}

//NOTE: context() is run when a context menu entry is chosen.
function context(str)
{
    todo("POINT", "context()");
}

//NOTE: prompt() is run when Enter is pressed.
//      appendPromptHistory is automatically called before prompt()
//      is called so calling it is only needed for erroneous input.
//      Any text in the command prompt is sent as an uppercase string.
function prompt(str)
{
    if (global.firstRun) {
        if (str == "M" || str == "MODE") {
            todo("POINT", "prompt() for PDMODE");
        }
        else if (str == "S" || str == "SIZE") {
            todo("POINT", "prompt() for PDSIZE");
        }
        var strList = str.split(",");
        if (isNaN(strList[0]) || isNaN(strList[1])) {
            alert(qsTr("Invalid point."));
            setPromptPrefix(qsTr("Specify first point: "));
        }
        else {
            global.firstRun = false;
            var x = Number(strList[0]);
            var y = Number(strList[1]);
            setPromptPrefix(qsTr("Specify next point: "));
            addPoint(x,y);
        }
    }
    else {
        var strList = str.split(",");
        if (isNaN(strList[0]) || isNaN(strList[1])) {
            alert(qsTr("Invalid point."));
            setPromptPrefix(qsTr("Specify next point: "));
        }
        else {
            var x = Number(strList[0]);
            var y = Number(strList[1]);
            setPromptPrefix(qsTr("Specify next point: "));
            addPoint(x,y);
        }
    }
}

==> commands/polygon.cpp <==
//Command: Polygon

var global = {}; //Required
global.centerX;
global.centerY;
global.sideX1;
global.sideY1;
global.sideX2;
global.sideY2;
global.pointIX;
global.pointIY;
global.pointCX;
global.pointCY;
global.polyType = "Inscribed"; //Default
global.numSides = 4;           //Default
global.mode;

//enums
global.mode_NUM_SIDES    = 0;
global.mode_CENTER_PT    = 1;
global.mode_POLYTYPE     = 2;
global.mode_INSCRIBE     = 3;
global.mode_CIRCUMSCRIBE = 4;
global.mode_DISTANCE     = 5;
global.mode_SIDE_LEN     = 6;

//NOTE: main() is run every time the command is started.
//      Use it to reset variables so they are ready to go.
function main()
{
    initCommand();
    clearSelection();
    global.centerX = NaN;
    global.centerY = NaN;
    global.sideX1  = NaN;
    global.sideY1  = NaN;
    global.sideX2  = NaN;
    global.sideY2  = NaN;
    global.pointIX = NaN;
    global.pointIY = NaN;
    global.pointCX = NaN;
    global.pointCY = NaN;
    global.mode = global.mode_NUM_SIDES;
    setPromptPrefix(qsTr("Enter number of sides") + " {" + global.numSides.toString() + "}: ");
}

//NOTE: click() is run only for left clicks.
//      Middle clicks are used for panning.
//      Right clicks bring up the context menu.
function click(x, y)
{
    if (global.mode == global.mode_NUM_SIDES) {
        //Do nothing, the prompt controls this.
    }
    else if (global.mode == global.mode_CENTER_PT) {
        global.centerX = x;
        global.centerY = y;
        global.mode = global.mode_POLYTYPE;
        appendPromptHistory();
        setPromptPrefix(qsTr("Specify polygon type [Inscribed in circle/Circumscribed around circle]") + " {" + global.polyType + "}: ");
    }
    else if (global.mode == global.mode_POLYTYPE) {
        //Do nothing, the prompt controls this.
    }
    else if (global.mode == global.mode_INSCRIBE) {
        global.pointIX = x;
        global.pointIY = y;
        setRubberPoint("POLYGON_INSCRIBE_POINT", global.pointIX, global.pointIY);
        vulcanize();
        appendPromptHistory();
        endCommand();
    }
    else if (global.mode == global.mode_CIRCUMSCRIBE) {
        global.pointCX = x;
        global.pointCY = y;
        setRubberPoint("POLYGON_CIRCUMSCRIBE_POINT", global.pointCX, global.pointCY);
        vulcanize();
        appendPromptHistory();
        endCommand();
    }
    else if (global.mode == global.mode_DISTANCE) {
        //Do nothing, the prompt controls this.
    }
    else if (global.mode == global.mode_SIDE_LEN) {
        todo("POLYGON", "Sidelength mode");
    }
}

//NOTE: context() is run when a context menu entry is chosen.
function context(str)
{
    todo("POLYGON", "context()");
}

//NOTE: prompt() is run when Enter is pressed.
//      appendPromptHistory is automatically called before prompt()
//      is called so calling it is only needed for erroneous input.
//      Any text in the command prompt is sent as an uppercase string.
function prompt(str)
{
    if (global.mode == global.mode_NUM_SIDES) {
        if (str == "" && global.numSides >= 3 && global.numSides <= 1024) {
            setPromptPrefix(qsTr("Specify center point or [Sidelength]: "));
            global.mode = global.mode_CENTER_PT;
        }
        else {
            var tmp = Number(str);
            if (isNaN(tmp) || !isInt(tmp) || tmp < 3 || tmp > 1024) {
                alert(qsTr("Requires an integer between 3 and 1024."));
                setPromptPrefix(qsTr("Enter number of sides") + " {" + global.numSides.toString() + "}: ");
            }
            else {
                global.numSides = tmp;
                setPromptPrefix(qsTr("Specify center point or [Sidelength]: "));
                global.mode = global.mode_CENTER_PT;
            }
        }
    }
    else if (global.mode == global.mode_CENTER_PT) {
        if (str == "S" || str == "SIDELENGTH") {
            global.mode = global.mode_SIDE_LEN;
            setPromptPrefix(qsTr("Specify start point: "));
        }
        else {
            var strList = str.split(",");
            if (isNaN(strList[0]) || isNaN(strList[1])) {
                alert(qsTr("Point or option keyword required."));
                setPromptPrefix(qsTr("Specify center point or [Sidelength]: "));
            }
            else {
                global.centerX = Number(strList[0]);
                global.centerY = Number(strList[1]);
                global.mode = global.mode_POLYTYPE;
                setPromptPrefix(qsTr("Specify polygon type [Inscribed in circle/Circumscribed around circle]") + " {" + global.polyType + "}: ");
            }
        }
    }
    else if (global.mode == global.mode_POLYTYPE) {
        if (str == "I" || str == "INSCRIBED") {
            global.mode = global.mode_INSCRIBE;
            global.polyType = "Inscribed";
            setPromptPrefix(qsTr("Specify polygon corner point or [Distance]: "));
            addRubber("POLYGON");
            setRubberMode("POLYGON_INSCRIBE");
            setRubberPoint("POLYGON_CENTER", global.centerX, global.centerY);
            setRubberPoint("POLYGON_NUM_SIDES", global.numSides, 0);
        }
        else if (str == "C" || str == "CIRCUMSCRIBED") {
            global.mode = global.mode_CIRCUMSCRIBE;
            global.polyType = "Circumscribed";
            setPromptPrefix(qsTr("Specify polygon side point or [Distance]: "));
            addRubber("POLYGON");
            setRubberMode("POLYGON_CIRCUMSCRIBE");
            setRubberPoint("POLYGON_CENTER", global.centerX, global.centerY);
            setRubberPoint("POLYGON_NUM_SIDES", global.numSides, 0);
        }
        else if (str == "") {
            if (global.polyType == "Inscribed") {
                global.mode = global.mode_INSCRIBE;
                setPromptPrefix(qsTr("Specify polygon corner point or [Distance]: "));
                addRubber("POLYGON");
                setRubberMode("POLYGON_INSCRIBE");
                setRubberPoint("POLYGON_CENTER", global.centerX, global.centerY);
                setRubberPoint("POLYGON_NUM_SIDES", global.numSides, 0);
            }
            else if (global.polyType == "Circumscribed") {
                global.mode = global.mode_CIRCUMSCRIBE;
                setPromptPrefix(qsTr("Specify polygon side point or [Distance]: "));
                addRubber("POLYGON");
                setRubberMode("POLYGON_CIRCUMSCRIBE");
                setRubberPoint("POLYGON_CENTER", global.centerX, global.centerY);
                setRubberPoint("POLYGON_NUM_SIDES", global.numSides, 0);
            }
            else {
                error("POLYGON", qsTr("Polygon type is not Inscribed or Circumscribed."));
            }
        }
        else {
            alert(qsTr("Invalid option keyword."));
            setPromptPrefix(qsTr("Specify polygon type [Inscribed in circle/Circumscribed around circle]") + " {" + global.polyType + "}: ");
        }
    }
    else if (global.mode == global.mode_INSCRIBE) {
        if (str == "D" || str == "DISTANCE") {
            global.mode = global.mode_DISTANCE;
            setPromptPrefix(qsTr("Specify distance: "));
        }
        else {
            var strList = str.split(",");
            if (isNaN(strList[0]) || isNaN(strList[1])) {
                alert(qsTr("Point or option keyword required."));
                setPromptPrefix(qsTr("Specify polygon corner point or [Distance]: "));
            }
            else {
                global.pointIX = Number(strList[0]);
                global.pointIY = Number(strList[1]);
                setRubberPoint("POLYGON_INSCRIBE_POINT", global.pointIX, global.pointIY);
                vulcanize();
                endCommand();
            }
        }
    }
    else if (global.mode == global.mode_CIRCUMSCRIBE) {
        if (str == "D" || str == "DISTANCE") {
            global.mode = global.mode_DISTANCE;
            setPromptPrefix(qsTr("Specify distance: "));
        }
        else {
            var strList = str.split(",");
            if (isNaN(strList[0]) || isNaN(strList[1])) {
                alert(qsTr("Point or option keyword required."));
                setPromptPrefix(qsTr("Specify polygon side point or [Distance]: "));
            }
            else {
                global.pointCX = Number(strList[0]);
                global.pointCY = Number(strList[1]);
                setRubberPoint("POLYGON_CIRCUMSCRIBE_POINT", global.pointCX, global.pointCY);
                vulcanize();
                endCommand();
            }
        }
    }
    else if (global.mode == global.mode_DISTANCE) {
        if (isNaN(str)) {
            alert(qsTr("Requires valid numeric distance."));
            setPromptPrefix(qsTr("Specify distance: "));
        }
        else {
            if (global.polyType == "Inscribed") {
                global.pointIX = global.centerX;
                global.pointIY = global.centerY + Number(str);
                setRubberPoint("POLYGON_INSCRIBE_POINT", global.pointIX, global.pointIY);
                vulcanize();
                endCommand();
            }
            else if (global.polyType == "Circumscribed") {
                global.pointCX = global.centerX;
                global.pointCY = global.centerY + Number(str);
                setRubberPoint("POLYGON_CIRCUMSCRIBE_POINT", global.pointCX, global.pointCY);
                vulcanize();
                endCommand();
            }
            else {
                error("POLYGON", qsTr("Polygon type is not Inscribed or Circumscribed."));
            }
        }
    }
    else if (global.mode == global.mode_SIDE_LEN) {
        todo("POLYGON", "Sidelength mode");
    }
}

==> commands/polyline.cpp <==
//Command: Polyline

var global = {}; //Required
global.firstRun;
global.firstX;
global.firstY;
global.prevX;
global.prevY;
global.num;

//NOTE: main() is run every time the command is started.
//      Use it to reset variables so they are ready to go.
function main()
{
    initCommand();
    clearSelection();
    global.firstRun = true;
    global.firstX = NaN;
    global.firstY = NaN;
    global.prevX = NaN;
    global.prevY = NaN;
    global.num = 0;
    setPromptPrefix(qsTr("Specify first point: "));
}

//NOTE: click() is run only for left clicks.
//      Middle clicks are used for panning.
//      Right clicks bring up the context menu.
function click(x, y)
{
    if (global.firstRun) {
        global.firstRun = false;
        global.firstX = x;
        global.firstY = y;
        global.prevX = x;
        global.prevY = y;
        addRubber("POLYLINE");
        setRubberMode("POLYLINE");
        setRubberPoint("POLYLINE_POINT_0", global.firstX, global.firstY);
        appendPromptHistory();
        setPromptPrefix(qsTr("Specify next point or [Undo]: "));
    }
    else {
        global.num++;
        setRubberPoint("POLYLINE_POINT_" + global.num.toString(), x, y);
        setRubberText("POLYLINE_NUM_POINTS", global.num.toString());
        spareRubber("POLYLINE");
        appendPromptHistory();
        global.prevX = x;
        global.prevY = y;
    }
}

//NOTE: context() is run when a context menu entry is chosen.
function context(str)
{
    todo("POLYLINE", "context()");
}

//NOTE: prompt() is run when Enter is pressed.
//      appendPromptHistory is automatically called before prompt()
//      is called so calling it is only needed for erroneous input.
//      Any text in the command prompt is sent as an uppercase string.
function prompt(str)
{
    if (global.firstRun) {
        var strList = str.split(",");
        if (isNaN(strList[0]) || isNaN(strList[1])) {
            alert(qsTr("Invalid point."));
            setPromptPrefix(qsTr("Specify first point: "));
        }
        else {
            global.firstRun = false;
            global.firstX = Number(strList[0]);
            global.firstY = Number(strList[1]);
            global.prevX = global.firstX;
            global.prevY = global.firstY;
            addRubber("POLYLINE");
            setRubberMode("POLYLINE");
            setRubberPoint("POLYLINE_POINT_0", global.firstX, global.firstY);
            setPromptPrefix(qsTr("Specify next point or [Undo]: "));
        }
    }
    else {
        if (str == "U" || str == "UNDO") {
            todo("POLYLINE", "prompt() for UNDO");
        }
        else {
            var strList = str.split(",");
            if (isNaN(strList[0]) || isNaN(strList[1])) {
                alert(qsTr("Point or option keyword required."));
                setPromptPrefix(qsTr("Specify next point or [Undo]: "));
            }
            else {
                var x = Number(strList[0]);
                var y = Number(strList[1]);
                global.num++;
                setRubberPoint("POLYLINE_POINT_" + global.num.toString(), x, y);
                setRubberText("POLYLINE_NUM_POINTS", global.num.toString());
                spareRubber("POLYLINE");
                global.prevX = x;
                global.prevY = y;
                setPromptPrefix(qsTr("Specify next point or [Undo]: "));
            }
        }
    }
}

==> commands/quickleader.cpp <==
//Command: QuickLeader

var global = {}; //Required
global.x1;
global.y1;
global.x2;
global.y2;

//TODO: Adding the text is not complete yet.

//NOTE: main() is run every time the command is started.
//      Use it to reset variables so they are ready to go.
function main()
{
    initCommand();
    clearSelection();
    global.x1 = NaN;
    global.y1 = NaN;
    global.x2 = NaN;
    global.y2 = NaN;
    setPromptPrefix(qsTr("Specify first point: "));
}

//NOTE: click() is run only for left clicks.
//      Middle clicks are used for panning.
//      Right clicks bring up the context menu.
function click(x, y)
{
    if (isNaN(global.x1)) {
        global.x1 = x;
        global.y1 = y;
        addRubber("DIMLEADER");
        setRubberMode("DIMLEADER_LINE");
        setRubberPoint("DIMLEADER_LINE_START", global.x1, global.y1);
        appendPromptHistory();
        setPromptPrefix(qsTr("Specify second point: "));
    }
    else {
        global.x2 = x;
        global.y2 = y;
        setRubberPoint("DIMLEADER_LINE_END", global.x2, global.y2);
        vulcanize();
        appendPromptHistory();
        endCommand();
    }
}

//NOTE: context() is run when a context menu entry is chosen.
function context(str)
{
    todo("QUICKLEADER", "context()");
}

//NOTE: prompt() is run when Enter is pressed.
//      appendPromptHistory is automatically called before prompt()
//      is called so calling it is only needed for erroneous input.
//      Any text in the command prompt is sent as an uppercase string.
function prompt(str)
{
    var strList = str.split(",");
    if (isNaN(global.x1)) {
        if (isNaN(strList[0]) || isNaN(strList[1])) {
            alert(qsTr("Requires two points."));
            setPromptPrefix(qsTr("Specify first point: "));
        }
        else {
            global.x1 = Number(strList[0]);
            global.y1 = Number(strList[1]);
            addRubber("DIMLEADER");
            setRubberMode("DIMLEADER_LINE");
            setRubberPoint("DIMLEADER_LINE_START", global.x1, global.y1);
            setPromptPrefix(qsTr("Specify second point: "));
        }
    }
    else {
        if (isNaN(strList[0]) || isNaN(strList[1])) {
            alert(qsTr("Requires two points."));
            setPromptPrefix(qsTr("Specify second point: "));
        }
        else {
            global.x2 = Number(strList[0]);
            global.y2 = Number(strList[1]);
            setRubberPoint("DIMLEADER_LINE_END", global.x2, global.y2);
            vulcanize();
            endCommand();
        }
    }
}

==> commands/rectangle.cpp <==
//Command: Rectangle

var global = {}; //Required
global.newRect;
global.x1;
global.y1;
global.x2;
global.y2;

//NOTE: main() is run every time the command is started.
//      Use it to reset variables so they are ready to go.
function main()
{
    initCommand();
    clearSelection();
    global.newRect = true;
    global.x1 = NaN;
    global.y1 = NaN;
    global.x2 = NaN;
    global.y2 = NaN;
    setPromptPrefix(qsTr("Specify first corner point or [Chamfer/Fillet]: "));
}

//NOTE: click() is run only for left clicks.
//      Middle clicks are used for panning.
//      Right clicks bring up the context menu.
function click(x, y)
{
    if (global.newRect) {
        global.newRect = false;
        global.x1 = x;
        global.y1 = y;
        addRubber("RECTANGLE");
        setRubberMode("RECTANGLE");
        setRubberPoint("RECTANGLE_START", x, y);
        setPromptPrefix(qsTr("Specify other corner point or [Dimensions]: "));
    }
    else {
        global.newRect = true;
        global.x2 = x;
        global.y2 = y;
        setRubberPoint("RECTANGLE_END", x, y);
        vulcanize();
        endCommand();
    }
}

//NOTE: context() is run when a context menu entry is chosen.
function context(str)
{
    todo("RECTANGLE", "context()");
}

//NOTE: prompt() is run when Enter is pressed.
//      appendPromptHistory is automatically called before prompt()
//      is called so calling it is only needed for erroneous input.
//      Any text in the command prompt is sent as an uppercase string.
function prompt(str)
{
    if (str == "C" || str == "CHAMFER") {
        todo("RECTANGLE", "prompt() for CHAMFER");
    }
    else if (str == "D" || str == "DIMENSIONS") {
        todo("RECTANGLE", "prompt() for DIMENSIONS");
    }
    else if (str == "F" || str == "FILLET") {
        todo("RECTANGLE", "prompt() for FILLET");
    }
    else {
        var strList = str.split(",");
        if (isNaN(strList[0]) || isNaN(strList[1])) {
            alert(qsTr("Invalid point."));
            setPromptPrefix(qsTr("Specify first point: "));
        }
        else {
            var x = Number(strList[0]);
            var y = Number(strList[1]);
            if (global.newRect) {
                global.newRect = false;
                global.x1 = x;
                global.y1 = y;
                addRubber("RECTANGLE");
                setRubberMode("RECTANGLE");
                setRubberPoint("RECTANGLE_START", x, y);
                setPromptPrefix(qsTr("Specify other corner point or [Dimensions]: "));
            }
            else {
                global.newRect = true;
                global.x2 = x;
                global.y2 = y;
                setRubberPoint("RECTANGLE_END", x, y);
                vulcanize();
                endCommand();
            }
        }
    }
}

==> commands/rgb.cpp <==
//Command: RGB

var global = {}; //Required
global.mode;

//enums
global.mode_BACKGROUND = 0;
global.mode_CROSSHAIR  = 1;
global.mode_GRID       = 2;

//NOTE: main() is run every time the command is started.
//      Use it to reset variables so they are ready to go.
function main()
{
    initCommand();
    clearSelection();
    global.mode = global.mode_BACKGROUND;
    setPromptPrefix(qsTr("Enter RED,GREEN,BLUE values for background or [Crosshair/Grid]: "));
}

//NOTE: click() is run only for left clicks.
//      Middle clicks are used for panning.
//      Right clicks bring up the context menu.
function click(x, y)
{
    //Do Nothing, prompt only command.
}

//NOTE: context() is run when a context menu entry is chosen.
function context(str)
{
    todo("RGB", "context()");
}

//NOTE: prompt() is run when Enter is pressed.
//      appendPromptHistory is automatically called before prompt()
//      is called so calling it is only needed for erroneous input.
//      Any text is in the command prompt is sent as an uppercase string.
function prompt(str)
{
    if (global.mode == global.mode_BACKGROUND) {
        if (str == "C" || str == "CROSSHAIR") {
            global.mode = global.mode_CROSSHAIR;
            setPromptPrefix(qsTr("Specify crosshair color: "));
        }
        else if (str == "G" || str == "GRID") {
            global.mode = global.mode_GRID;
            setPromptPrefix(qsTr("Specify grid color: "));
        }
        else {
            var strList = str.split(",");
            var r = Number(strList[0]);
            var g = Number(strList[1]);
            var b = Number(strList[2]);
            if (!validRGB(r,g,b)) {
                alert(qsTr("Invalid color. R,G,B values must be in the range of 0-255."));
                setPromptPrefix(qsTr("Specify background color: "));
            }
            else {
                setBackgroundColor(r,g,b);
                endCommand();
            }
        }
    }
    else if (global.mode == global.mode_CROSSHAIR) {
        var strList = str.split(",");
        var r = Number(strList[0]);
        var g = Number(strList[1]);
        var b = Number(strList[2]);
        if (!validRGB(r,g,b)) {
            alert(qsTr("Invalid color. R,G,B values must be in the range of 0-255."));
            setPromptPrefix(qsTr("Specify crosshair color: "));
        }
        else {
            setCrossHairColor(r,g,b);
            endCommand();
        }
    }
    else if (global.mode == global.mode_GRID) {
        var strList = str.split(",");
        var r = Number(strList[0]);
        var g = Number(strList[1]);
        var b = Number(strList[2]);
        if (!validRGB(r,g,b)) {
            alert(qsTr("Invalid color. R,G,B values must be in the range of 0-255."));
            setPromptPrefix(qsTr("Specify grid color: "));
        }
        else {
            setGridColor(r,g,b);
            endCommand();
        }
    }
}

function validRGB(r, g, b)
{
    if (isNaN(r)) return false;
    if (isNaN(g)) return false;
    if (isNaN(b)) return false;
    if (r < 0 || r > 255) return false;
    if (g < 0 || g > 255) return false;
    if (b < 0 || b > 255) return false;
    return true;
}
==> commands/rotate.cpp <==
//Command: Rotate

var global = {}; //Required
global.firstRun;
global.baseX;
global.baseY;
global.destX;
global.destY;
global.angle;

global.baseRX;
global.baseRY;
global.destRX;
global.destRY;
global.angleRef;
global.angleNew;

global.mode;

//enums
global.mode_NORMAL    = 0;
global.mode_REFERENCE = 1;

//NOTE: main() is run every time the command is started.
//      Use it to reset variables so they are ready to go.
function main()
{
    initCommand();
    global.mode = global.mode_NORMAL;
    global.firstRun = true;
    global.baseX = NaN;
    global.baseY = NaN;
    global.destX = NaN;
    global.destY = NaN;
    global.angle = NaN;

    global.baseRX   = NaN;
    global.baseRY   = NaN;
    global.destRX   = NaN;
    global.destRY   = NaN;
    global.angleRef = NaN;
    global.angleNew = NaN;

    if (numSelected() <= 0) {
        //TODO: Prompt to select objects if nothing is preselected
        alert(qsTr("Preselect objects before invoking the rotate command."));
        endCommand();
        messageBox("information", qsTr("Rotate Preselect"), qsTr("Preselect objects before invoking the rotate command."));
    }
    else {
        setPromptPrefix(qsTr("Specify base point: "));
    }
}

//NOTE: click() is run only for left clicks.
//      Middle clicks are used for panning.
//      Right clicks bring up the context menu.
function click(x, y)
{
    if (global.mode == global.mode_NORMAL) {
        if (global.firstRun) {
            global.firstRun = false;
            global.baseX = x;
            global.baseY = y;
            addRubber("LINE");
            setRubberMode("LINE");
            setRubberPoint("LINE_START", global.baseX, global.baseY);
            previewOn("SELECTED", "ROTATE", global.baseX, global.baseY, 0);
            appendPromptHistory();
            setPromptPrefix(qsTr("Specify rotation angle or [Reference]: "));
        }
        else {
            global.destX = x;
            global.destY = y;
            global.angle = calculateAngle(global.baseX, global.baseY, global.destX, global.destY);
            appendPromptHistory();
            rotateSelected(global.baseX, global.baseY, global.angle);
            previewOff();
            endCommand();
        }
    }
    else if (global.mode == global.mode_REFERENCE) {
        if (isNaN(global.baseRX)) {
            global.baseRX = x;
            global.baseRY = y;
            appendPromptHistory();
            addRubber("LINE");
            setRubberMode("LINE");
            setRubberPoint("LINE_START", global.baseRX, global.baseRY);
            setPromptPrefix(qsTr("Specify second point: "));
        }
        else if (isNaN(global.destRX)) {
            global.destRX = x;
            global.destRY = y;
            global.angleRef = calculateAngle(global.baseRX, global.baseRY, global.destRX, global.destRY);
            setRubberPoint("LINE_START", global.baseX, global.baseY);
            previewOn("SELECTED", "ROTATE", global.baseX, global.baseY, global.angleRef);
            appendPromptHistory();
            setPromptPrefix(qsTr("Specify the new angle: "));
        }
        else if (isNaN(global.angleNew)) {
            global.angleNew = calculateAngle(global.baseX, global.baseY, x, y);
            rotateSelected(global.baseX, global.baseY, global.angleNew - global.angleRef);
            previewOff();
            endCommand();
        }
    }
}

//NOTE: context() is run when a context menu entry is chosen.
function context(str)
{
    todo("ROTATE", "context()");
}

//NOTE: prompt() is run when Enter is pressed.
//      appendPromptHistory is automatically called before prompt()
//      is called so calling it is only needed for erroneous input.
//      Any text in the command prompt is sent as an uppercase string.
function prompt(str)
{
    if (global.mode == global.mode_NORMAL) {
        if (global.firstRun) {
            var strList = str.split(",");
            if (isNaN(strList[0]) || isNaN(strList[1])) {
                alert(qsTr("Invalid point."));
                setPromptPrefix(qsTr("Specify base point: "));
            }
            else {
                global.firstRun = false;
                global.baseX = Number(strList[0]);
                global.baseY = Number(strList[1]);
                addRubber("LINE");
                setRubberMode("LINE");
                setRubberPoint("LINE_START", global.baseX, global.baseY);
                previewOn("SELECTED", "ROTATE", global.baseX, global.baseY, 0);
                setPromptPrefix(qsTr("Specify rotation angle or [Reference]: "));
            }
        }
        else {
            if (str == "R" || str == "REFERENCE") {
                global.mode = global.mode_REFERENCE;
                setPromptPrefix(qsTr("Specify the reference angle") + " {0.00}: ");
                clearRubber();
                previewOff();
            }
            else {
                if (isNaN(str)) {
                    alert(qsTr("Requires valid numeric angle, second point, or option keyword."));
                    setPromptPrefix(qsTr("Specify rotation angle or [Reference]: "));
                }
                else {
                    global.angle = Number(str);
                    rotateSelected(global.baseX, global.baseY, global.angle);
                    previewOff();
                    endCommand();
                }
            }
        }
    }
    else if (global.mode == global.mode_REFERENCE) {
        if (isNaN(global.baseRX)) {
            if (isNaN(str)) {
                var strList = str.split(",");
                if (isNaN(strList[0]) || isNaN(strList[1])) {
                    alert(qsTr("Requires valid numeric angle or two points."));
                    setPromptPrefix(qsTr("Specify the reference angle") + " {0.00}: ");
                }
                else {
                    global.baseRX = Number(strList[0]);
                    global.baseRY = Number(strList[1]);
                    addRubber("LINE");
                    setRubberMode("LINE");
                    setRubberPoint("LINE_START", global.baseRX, global.baseRY);
                    setPromptPrefix(qsTr("Specify second point: "));
                }
            }
            else {
                //The base and dest values are only set here to advance the command.
                global.baseRX = 0.0;
                global.baseRY = 0.0;
                global.destRX = 0.0;
                global.destRY = 0.0;
                //The reference angle is what we will use later.
                global.angleRef = Number(str);
                addRubber("LINE");
                setRubberMode("LINE");
                setRubberPoint("LINE_START", global.baseX, global.baseY);
                previewOn("SELECTED", "ROTATE", global.baseX, global.baseY, global.angleRef);
                setPromptPrefix(qsTr("Specify the new angle: "));
            }
        }
        else if (isNaN(global.destRX)) {
            if (isNaN(str)) {
                var strList = str.split(",");
                if (isNaN(strList[0]) || isNaN(strList[1])) {
                    alert(qsTr("Requires valid numeric angle or two points."));
                    setPromptPrefix(qsTr("Specify second point: "));
                }
                else {
                    global.destRX = Number(strList[0]);
                    global.destRY = Number(strList[1]);
                    global.angleRef = calculateAngle(global.baseRX, global.baseRY, global.destRX, global.destRY);
                    previewOn("SELECTED", "ROTATE", global.baseX, global.baseY, global.angleRef);
                    setRubberPoint("LINE_START", global.baseX, global.baseY);
                    setPromptPrefix(qsTr("Specify the new angle: "));
                }
            }
            else {
                //The base and dest values are only set here to advance the command.
                global.baseRX = 0.0;
                global.baseRY = 0.0;
                global.destRX = 0.0;
                global.destRY = 0.0;
                //The reference angle is what we will use later.
                global.angleRef = Number(str);
                previewOn("SELECTED", "ROTATE", global.baseX, global.baseY, global.angleRef);
                setPromptPrefix(qsTr("Specify the new angle: "));
            }
        }
        else if (isNaN(global.angleNew)) {
            if (isNaN(str)) {
                var strList = str.split(",");
                if (isNaN(strList[0]) || isNaN(strList[1])) {
                    alert(qsTr("Requires valid numeric angle or second point."));
                    setPromptPrefix(qsTr("Specify the new angle: "));
                }
                else {
                    var x = Number(strList[0]);
                    var y = Number(strList[1]);
                    global.angleNew = calculateAngle(global.baseX, global.baseY, x, y);
                    rotateSelected(global.baseX, global.baseY, global.angleNew - global.angleRef);
                    previewOff();
                    endCommand();
                }
            }
            else {
                global.angleNew = Number(str);
                rotateSelected(global.baseX, global.baseY, global.angleNew - global.angleRef);
                previewOff();
                endCommand();
            }
        }
    }
}

==> commands/sandbox.cpp <==
//Command: Sandbox

var global = {}; //Required
global.test1;
global.test2;

//NOTE: main() is run every time the command is started.
//      Use it to reset variables so they are ready to go.
function main()
{
    initCommand();
    
    //Report number of pre-selected objects
    setPromptPrefix("Number of Objects Selected: " + numSelected().toString());
    appendPromptHistory();
    
    mirrorSelected(0,0,0,1);
    
    //selectAll();
    //rotateSelected(0,0,90);
    
    //Polyline & Polygon Testing
    
    var offsetX = 0.0;
    var offsetY = 0.0;
    
    var polylineArray = [];
    polylineArray.push(1.0 + offsetX);
    polylineArray.push(1.0 + offsetY);
    polylineArray.push(1.0 + offsetX);
    polylineArray.push(2.0 + offsetY);
    polylineArray.push(2.0 + offsetX);
    polylineArray.push(2.0 + offsetY);
    polylineArray.push(2.0 + offsetX);
    polylineArray.push(3.0 + offsetY);
    polylineArray.push(3.0 + offsetX);
    polylineArray.push(3.0 + offsetY);
    polylineArray.push(3.0 + offsetX);
    polylineArray.push(2.0 + offsetY);
    polylineArray.push(4.0 + offsetX);
    polylineArray.push(2.0 + offsetY);
    polylineArray.push(4.0 + offsetX);
    polylineArray.push(1.0 + offsetY);
    addPolyline(polylineArray);
    
    offsetX = 5.0;
    offsetY = 0.0;
    
    var polygonArray = [];
    polygonArray.push(1.0 + offsetX);
    polygonArray.push(1.0 + offsetY);
    polygonArray.push(1.0 + offsetX);
    polygonArray.push(2.0 + offsetY);
    polygonArray.push(2.0 + offsetX);
    polygonArray.push(2.0 + offsetY);
    polygonArray.push(2.0 + offsetX);
    polygonArray.push(3.0 + offsetY);
    polygonArray.push(3.0 + offsetX);
    polygonArray.push(3.0 + offsetY);
    polygonArray.push(3.0 + offsetX);
    polygonArray.push(2.0 + offsetY);
    polygonArray.push(4.0 + offsetX);
    polygonArray.push(2.0 + offsetY);
    polygonArray.push(4.0 + offsetX);
    polygonArray.push(1.0 + offsetY);
    addPolygon(polygonArray);
    

    endCommand();
}

//NOTE: click() is run only for left clicks.
//      Middle clicks are used for panning.
//      Right clicks bring up the context menu.
function click(x, y)
{
}

//NOTE: context() is run when a context menu entry is chosen.
function context(str)
{
}

//NOTE: prompt() is run when Enter is pressed.
//      appendPromptHistory is automatically called before prompt()
//      is called so calling it is only needed for erroneous input.
//      Any text in the command prompt is sent as an uppercase string.
function prompt(str)
{
}


==> commands/scale.cpp <==
//Command: Scale

var global = {}; //Required
global.firstRun;
global.baseX;
global.baseY;
global.destX;
global.destY;
global.factor;

global.baseRX;
global.baseRY;
global.destRX;
global.destRY;
global.factorRef;
global.factorNew;

global.mode;

//enums
global.mode_NORMAL    = 0;
global.mode_REFERENCE = 1;

//NOTE: main() is run every time the command is started.
//      Use it to reset variables so they are ready to go.
function main()
{
    initCommand();
    global.mode = global.mode_NORMAL;
    global.firstRun = true;
    global.baseX  = NaN;
    global.baseY  = NaN;
    global.destX  = NaN;
    global.destY  = NaN;
    global.factor = NaN;

    global.baseRX    = NaN;
    global.baseRY    = NaN;
    global.destRX    = NaN;
    global.destRY    = NaN;
    global.factorRef = NaN;
    global.factorNew = NaN;

    if (numSelected() <= 0) {
        //TODO: Prompt to select objects if nothing is preselected
        alert(qsTr("Preselect objects before invoking the scale command."));
        endCommand();
        messageBox("information", qsTr("Scale Preselect"), qsTr("Preselect objects before invoking the scale command."));
    }
    else {
        setPromptPrefix(qsTr("Specify base point: "));
    }
}

//NOTE: click() is run only for left clicks.
//      Middle clicks are used for panning.
//      Right clicks bring up the context menu.
function click(x, y)
{
    if (global.mode == global.mode_NORMAL) {
        if (global.firstRun) {
            global.firstRun = false;
            global.baseX = x;
            global.baseY = y;
            addRubber("LINE");
            setRubberMode("LINE");
            setRubberPoint("LINE_START", global.baseX, global.baseY);
            previewOn("SELECTED", "SCALE", global.baseX, global.baseY, 1);
            appendPromptHistory();
            setPromptPrefix(qsTr("Specify scale factor or [Reference]: "));
        }
        else {
            global.destX = x;
            global.destY = y;
            global.factor = calculateDistance(global.baseX, global.baseY, global.destX, global.destY);
            appendPromptHistory();
            scaleSelected(global.baseX, global.baseY, global.factor);
            previewOff();
            endCommand();
        }
    }
    else if (global.mode == global.mode_REFERENCE) {
        if (isNaN(global.baseRX)) {
            global.baseRX = x;
            global.baseRY = y;
            appendPromptHistory();
            addRubber("LINE");
            setRubberMode("LINE");
            setRubberPoint("LINE_START", global.baseRX, global.baseRY);
            setPromptPrefix(qsTr("Specify second point: "));
        }
        else if (isNaN(global.destRX)) {
            global.destRX = x;
            global.destRY = y;
            global.factorRef = calculateDistance(global.baseRX, global.baseRY, global.destRX, global.destRY);
            if (global.factorRef <= 0.0) {
                global.destRX    = NaN;
                global.destRY    = NaN;
                global.factorRef = NaN;
                alert(qsTr("Value must be positive and nonzero."));
                setPromptPrefix(qsTr("Specify second point: "));
            }
            else {
                appendPromptHistory();
                setRubberPoint("LINE_START", global.baseX, global.baseY);
                previewOn("SELECTED", "SCALE", global.baseX, global.baseY, global.factorRef);
                setPromptPrefix(qsTr("Specify new length: "));
            }
        }
        else if (isNaN(global.factorNew)) {
            global.factorNew = calculateDistance(global.baseX, global.baseY, x, y);
            if (global.factorNew <= 0.0) {
                global.factorNew = NaN;
                alert(qsTr("Value must be positive and nonzero."));
                setPromptPrefix(qsTr("Specify new length: "));
            }
            else {
                appendPromptHistory();
                scaleSelected(global.baseX, global.baseY, global.factorNew/global.factorRef);
                previewOff();
                endCommand();
            }
        }
    }
}

//NOTE: context() is run when a context menu entry is chosen.
function context(str)
{
    todo("SCALE", "context()");
}

//NOTE: prompt() is run when Enter is pressed.
//      appendPromptHistory is automatically called before prompt()
//      is called so calling it is only needed for erroneous input.
//      Any text in the command prompt is sent as an uppercase string.
function prompt(str)
{
    if (global.mode == global.mode_NORMAL) {
        if (global.firstRun) {
            var strList = str.split(",");
            if (isNaN(strList[0]) || isNaN(strList[1])) {
                alert(qsTr("Invalid point."));
                setPromptPrefix(qsTr("Specify base point: "));
            }
            else {
                global.firstRun = false;
                global.baseX = Number(strList[0]);
                global.baseY = Number(strList[1]);
                addRubber("LINE");
                setRubberMode("LINE");
                setRubberPoint("LINE_START", global.baseX, global.baseY);
                previewOn("SELECTED", "SCALE", global.baseX, global.baseY, 1);
                setPromptPrefix(qsTr("Specify scale factor or [Reference]: "));
            }
        }
        else {
            if (str == "R" || str == "REFERENCE") {
                global.mode = global.mode_REFERENCE;
                setPromptPrefix(qsTr("Specify reference length") + " {1}: ");
                clearRubber();
                previewOff();
            }
            else {
                if (isNaN(str)) {
                    alert(qsTr("Requires valid numeric distance, second point, or option keyword."));
                    setPromptPrefix(qsTr("Specify scale factor or [Reference]: "));
                }
                else {
                    global.factor = Number(str);
                    scaleSelected(global.baseX, global.baseY, global.factor);
                    previewOff();
                    endCommand();
                }
            }
        }
    }
    else if (global.mode == global.mode_REFERENCE) {
        if (isNaN(global.baseRX)) {
            if (isNaN(str)) {
                var strList = str.split(",");
                if (isNaN(strList[0]) || isNaN(strList[1])) {
                    alert(qsTr("Requires valid numeric distance or two points."));
                    setPromptPrefix(qsTr("Specify reference length") + " {1}: ");
                }
                else {
                    global.baseRX = Number(strList[0]);
                    global.baseRY = Number(strList[1]);
                    addRubber("LINE");
                    setRubberMode("LINE");
                    setRubberPoint("LINE_START", global.baseRX, global.baseRY);
                    setPromptPrefix(qsTr("Specify second point: "));
                }
            }
            else {
                //The base and dest values are only set here to advance the command.
                global.baseRX = 0.0;
                global.baseRY = 0.0;
                global.destRX = 0.0;
                global.destRY = 0.0;
                //The reference length is what we will use later.
                global.factorRef = Number(str);
                if (global.factorRef <= 0.0) {
                    global.baseRX    = NaN;
                    global.baseRY    = NaN;
                    global.destRX    = NaN;
                    global.destRY    = NaN;
                    global.factorRef = NaN;
                    alert(qsTr("Value must be positive and nonzero."));
                    setPromptPrefix(qsTr("Specify reference length") + " {1}: ");
                }
                else {
                    addRubber("LINE");
                    setRubberMode("LINE");
                    setRubberPoint("LINE_START", global.baseX, global.baseY);
                    previewOn("SELECTED", "SCALE", global.baseX, global.baseY, global.factorRef);
                    setPromptPrefix(qsTr("Specify new length: "));
                }
            }
        }
        else if (isNaN(global.destRX)) {
            if (isNaN(str)) {
                var strList = str.split(",");
                if (isNaN(strList[0]) || isNaN(strList[1])) {
                    alert(qsTr("Requires valid numeric distance or two points."));
                    setPromptPrefix(qsTr("Specify second point: "));
                }
                else {
                    global.destRX = Number(strList[0]);
                    global.destRY = Number(strList[1]);
                    global.factorRef = calculateDistance(global.baseRX, global.baseRY, global.destRX, global.destRY);
                    if (global.factorRef <= 0.0) {
                        global.destRX    = NaN;
                        global.destRY    = NaN;
                        global.factorRef = NaN;
                        alert(qsTr("Value must be positive and nonzero."));
                        setPromptPrefix(qsTr("Specify second point: "));
                    }
                    else {
                        setRubberPoint("LINE_START", global.baseX, global.baseY);
                        previewOn("SELECTED", "SCALE", global.baseX, global.baseY, global.factorRef);
                        setPromptPrefix(qsTr("Specify new length: "));
                    }
                }
            }
            else {
                //The base and dest values are only set here to advance the command.
                global.baseRX = 0.0;
                global.baseRY = 0.0;
                global.destRX = 0.0;
                global.destRY = 0.0;
                //The reference length is what we will use later.
                global.factorRef = Number(str);
                if (global.factorRef <= 0.0) {
                    global.destRX    = NaN;
                    global.destRY    = NaN;
                    global.factorRef = NaN;
                    alert(qsTr("Value must be positive and nonzero."));
                    setPromptPrefix(qsTr("Specify second point: "));
                }
                else {
                    setRubberPoint("LINE_START", global.baseX, global.baseY);
                    previewOn("SELECTED", "SCALE", global.baseX, global.baseY, global.factorRef);
                    setPromptPrefix(qsTr("Specify new length: "));
                }
            }
        }
        else if (isNaN(global.factorNew)) {
            if (isNaN(str)) {
                var strList = str.split(",");
                if (isNaN(strList[0]) || isNaN(strList[1])) {
                    alert(qsTr("Requires valid numeric distance or second point."));
                    setPromptPrefix(qsTr("Specify new length: "));
                }
                else {
                    var x = Number(strList[0]);
                    var y = Number(strList[1]);
                    global.factorNew = calculateDistance(global.baseX, global.baseY, x, y);
                    if (global.factorNew <= 0.0) {
                        global.factorNew = NaN;
                        alert(qsTr("Value must be positive and nonzero."));
                        setPromptPrefix(qsTr("Specify new length: "));
                    }
                    else {
                        scaleSelected(global.baseX, global.baseY, global.factorNew/global.factorRef);
                        previewOff();
                        endCommand();
                    }
                }
            }
            else {
                global.factorNew = Number(str);
                if (global.factorNew <= 0.0) {
                    global.factorNew = NaN;
                    alert(qsTr("Value must be positive and nonzero."));
                    setPromptPrefix(qsTr("Specify new length: "));
                }
                else {
                    scaleSelected(global.baseX, global.baseY, global.factorNew/global.factorRef);
                    previewOff();
                    endCommand();
                }
            }
        }
    }
}

==> commands/selectall.cpp <==
//Command: SelectAll

//NOTE: main() is run every time the command is started.
//      Use it to reset variables so they are ready to go.
function main()
{
    initCommand();
    selectAll();
    endCommand();
}

//NOTE: click() is run only for left clicks.
//      Middle clicks are used for panning.
//      Right clicks bring up the context menu.
function click(x, y)
{
    selectAll();
    endCommand();
}

//NOTE: context() is run when a context menu entry is chosen.
function context(str)
{
    selectAll();
    endCommand();
}

//NOTE: prompt() is run when Enter is pressed.
//      appendPromptHistory is automatically called before prompt()
//      is called so calling it is only needed for erroneous input.
//      Any text is in the command prompt is sent as an uppercase string.
function prompt(str)
{
    selectAll();
    endCommand();
}

==> commands/singlelinetext.cpp <==
//Command: Single Line Text

var global = {}; //Required
global.text;
global.textX;
global.textY;
global.textJustify;
global.textFont;
global.textHeight;
global.textRotation;
global.mode;

//enums
global.mode_JUSTIFY = 0;
global.mode_SETFONT = 1;
global.mode_SETGEOM = 2;
global.mode_RAPID   = 3;

//NOTE: main() is run every time the command is started.
//      Use it to reset variables so they are ready to go.
function main()
{
    initCommand();
    clearSelection();
    global.text = "";
    global.textX = NaN;
    global.textY = NaN;
    global.textJustify = "Left";
    global.textFont = textFont();
    global.textHeight = NaN;
    global.textRotation = NaN;
    global.mode = global.mode_SETGEOM;
    setPromptPrefix(qsTr("Current font: ") + "{" + global.textFont + "} " + qsTr("Text height: ") + "{" +  textSize() + "}");
    appendPromptHistory();
    setPromptPrefix(qsTr("Specify start point of text or [Justify/Setfont]: "));
}

//NOTE: click() is run only for left clicks.
//      Middle clicks are used for panning.
//      Right clicks bring up the context menu.
function click(x, y)
{
    if (global.mode == global.mode_SETGEOM) {
        if (isNaN(global.textX)) {
            global.textX = x;
            global.textY = y;
            addRubber("LINE");
            setRubberMode("LINE");
            setRubberPoint("LINE_START", global.textX, global.textY);
            appendPromptHistory();
            setPromptPrefix(qsTr("Specify text height") + " {" + textSize() + "}: ");
        }
        else if (isNaN(global.textHeight)) {
            global.textHeight = calculateDistance(global.textX, global.textY, x, y);
            setTextSize(global.textHeight);
            appendPromptHistory();
            setPromptPrefix(qsTr("Specify text angle") + " {" + textAngle() + "}: ");
        }
        else if (isNaN(global.textRotation)) {
            global.textRotation = calculateAngle(global.textX, global.textY, x, y);
            setTextAngle(global.textRotation);
            appendPromptHistory();
            setPromptPrefix(qsTr("Enter text: "));
            global.mode = global.mode_RAPID;
            enablePromptRapidFire();
            clearRubber();
            addRubber("TEXTSINGLE");
            setRubberMode("TEXTSINGLE");
            setRubberPoint("TEXT_POINT", global.textX, global.textY);
            setRubberPoint("TEXT_HEIGHT_ROTATION", global.textHeight, global.textRotation);
            setRubberText("TEXT_FONT", global.textFont);
            setRubberText("TEXT_JUSTIFY", global.textJustify);
            setRubberText("TEXT_RAPID", global.text);
        }
        else {
            //Do nothing, as we are in rapidFire mode now.
        }
    }
}

//NOTE: context() is run when a context menu entry is chosen.
function context(str)
{
    todo("SINGLELINETEXT", "context()");
}

//NOTE: prompt() is run when Enter is pressed.
//      appendPromptHistory is automatically called before prompt()
//      is called so calling it is only needed for erroneous input.
//      Any text in the command prompt is sent as an uppercase string.
function prompt(str)
{
    if (global.mode == global.mode_JUSTIFY) {
        if (str == "C" || str == "CENTER") {
            global.mode = global.mode_SETGEOM;
            global.textJustify = "Center";
            setRubberText("TEXT_JUSTIFY", global.textJustify);
            setPromptPrefix(qsTr("Specify center point of text or [Justify/Setfont]: "));
        }
        else if (str == "R" || str == "RIGHT") {
            global.mode = global.mode_SETGEOM;
            global.textJustify = "Right";
            setRubberText("TEXT_JUSTIFY", global.textJustify);
            setPromptPrefix(qsTr("Specify right-end point of text or [Justify/Setfont]: "));
        }
        else if (str == "A" || str == "ALIGN") {
            global.mode = global.mode_SETGEOM;
            global.textJustify = "Aligned";
            setRubberText("TEXT_JUSTIFY", global.textJustify);
            setPromptPrefix(qsTr("Specify start point of text or [Justify/Setfont]: "));
        }
        else if (str == "M" || str == "MIDDLE") {
            global.mode = global.mode_SETGEOM;
            global.textJustify = "Middle";
            setRubberText("TEXT_JUSTIFY", global.textJustify);
            setPromptPrefix(qsTr("Specify middle point of text or [Justify/Setfont]: "));
        }
        else if (str == "F" || str == "FIT") {
            global.mode = global.mode_SETGEOM;
            global.textJustify = "Fit";
            setRubberText("TEXT_JUSTIFY", global.textJustify);
            setPromptPrefix(qsTr("Specify start point of text or [Justify/Setfont]: "));
        }
        else if (str == "TL" || str == "TOPLEFT") {
            global.mode = global.mode_SETGEOM;
            global.textJustify = "Top Left";
            setRubberText("TEXT_JUSTIFY", global.textJustify);
            setPromptPrefix(qsTr("Specify top-left point of text or [Justify/Setfont]: "));
        }
        else if (str == "TC" || str == "TOPCENTER") {
            global.mode = global.mode_SETGEOM;
            global.textJustify = "Top Center";
            setRubberText("TEXT_JUSTIFY", global.textJustify);
            setPromptPrefix(qsTr("Specify top-center point of text or [Justify/Setfont]: "));
        }
        else if (str == "TR" || str == "TOPRIGHT") {
            global.mode = global.mode_SETGEOM;
            global.textJustify = "Top Right";
            setRubberText("TEXT_JUSTIFY", global.textJustify);
            setPromptPrefix(qsTr("Specify top-right point of text or [Justify/Setfont]: "));
        }
        else if (str == "ML" || str == "MIDDLELEFT") {
            global.mode = global.mode_SETGEOM;
            global.textJustify = "Middle Left";
            setRubberText("TEXT_JUSTIFY", global.textJustify);
            setPromptPrefix(qsTr("Specify middle-left point of text or [Justify/Setfont]: "));
        }
        else if (str == "MC" || str == "MIDDLECENTER") {
            global.mode = global.mode_SETGEOM;
            global.textJustify = "Middle Center";
            setRubberText("TEXT_JUSTIFY", global.textJustify);
            setPromptPrefix(qsTr("Specify middle-center point of text or [Justify/Setfont]: "));
        }
        else if (str == "MR" || str == "MIDDLERIGHT") {
            global.mode = global.mode_SETGEOM;
            global.textJustify = "Middle Right";
            setRubberText("TEXT_JUSTIFY", global.textJustify);
            setPromptPrefix(qsTr("Specify middle-right point of text or [Justify/Setfont]: "));
        }
        else if (str == "BL" || str == "BOTTOMLEFT") {
            global.mode = global.mode_SETGEOM;
            global.textJustify = "Bottom Left";
            setRubberText("TEXT_JUSTIFY", global.textJustify);
            setPromptPrefix(qsTr("Specify bottom-left point of text or [Justify/Setfont]: "));
        }
        else if (str == "BC" || str == "BOTTOMCENTER") {
            global.mode = global.mode_SETGEOM;
            global.textJustify = "Bottom Center";
            setRubberText("TEXT_JUSTIFY", global.textJustify);
            setPromptPrefix(qsTr("Specify bottom-center point of text or [Justify/Setfont]: "));
        }
        else if (str == "BR" || str == "BOTTOMRIGHT") {
            global.mode = global.mode_SETGEOM;
            global.textJustify = "Bottom Right";
            setRubberText("TEXT_JUSTIFY", global.textJustify);
            setPromptPrefix(qsTr("Specify bottom-right point of text or [Justify/Setfont]: "));
        }
        else {
            alert(qsTr("Invalid option keyword."));
            setPromptPrefix(qsTr("Text Justification Options [Center/Right/Align/Middle/Fit/TL/TC/TR/ML/MC/MR/BL/BC/BR]: "));
        }
    }
    else if (global.mode == global.mode_SETFONT) {
        global.mode = global.mode_SETGEOM;
        global.textFont = str;
        setRubberText("TEXT_FONT", global.textFont);
        setTextFont(global.textFont);
        setPromptPrefix(qsTr("Specify start point of text or [Justify/Setfont]: "));
    }
    else if (global.mode == global.mode_SETGEOM) {
        if (isNaN(global.textX)) {
            if (str == "J" || str == "JUSTIFY") {
                global.mode = global.mode_JUSTIFY;
                setPromptPrefix(qsTr("Text Justification Options [Center/Right/Align/Middle/Fit/TL/TC/TR/ML/MC/MR/BL/BC/BR]: "));
            }
            else if (str == "S" || str == "SETFONT") {
                global.mode = global.mode_SETFONT;
                setPromptPrefix(qsTr("Specify font name: "));
            }
            else {
                var strList = str.split(",");
                if (isNaN(strList[0]) || isNaN(strList[1])) {
                    alert(qsTr("Point or option keyword required."));
                    setPromptPrefix(qsTr("Specify start point of text or [Justify/Setfont]: "));
                }
                else {
                    global.textX = Number(strList[0]);
                    global.textY = Number(strList[1]);
                    addRubber("LINE");
                    setRubberMode("LINE");
                    setRubberPoint("LINE_START", global.textX, global.textY);
                    setPromptPrefix(qsTr("Specify text height") + " {" + textSize() + "}: ");
                }
            }
        }
        else if (isNaN(global.textHeight)) {
            if (str == "") {
                global.textHeight = textSize();
                setPromptPrefix(qsTr("Specify text angle") + " {" + textAngle() + "}: ");
            }
            else if (isNaN(str)) {
                alert(qsTr("Requires valid numeric distance or second point."));
                setPromptPrefix(qsTr("Specify text height") + " {" + textSize() + "}: ");
            }
            else {
                global.textHeight = Number(str);
                setTextSize(global.textHeight);
                setPromptPrefix(qsTr("Specify text angle") + " {" + textAngle() + "}: ");
            }
        }
        else if (isNaN(global.textRotation)) {
            if (str == "") {
                global.textRotation = textAngle();
                setPromptPrefix(qsTr("Enter text: "));
                global.mode = global.mode_RAPID;
                enablePromptRapidFire();
                clearRubber();
                addRubber("TEXTSINGLE");
                setRubberMode("TEXTSINGLE");
                setRubberPoint("TEXT_POINT", global.textX, global.textY);
                setRubberPoint("TEXT_HEIGHT_ROTATION", global.textHeight, global.textRotation);
                setRubberText("TEXT_FONT", global.textFont);
                setRubberText("TEXT_JUSTIFY", global.textJustify);
                setRubberText("TEXT_RAPID", global.text);
            }
            else if (isNaN(str)) {
                alert(qsTr("Requires valid numeric angle or second point."));
                setPromptPrefix(qsTr("Specify text angle") + " {" + textAngle() + "}: ");
            }
            else {
                global.textRotation = Number(str);
                setTextAngle(global.textRotation);
                setPromptPrefix(qsTr("Enter text: "));
                global.mode = global.mode_RAPID;
                enablePromptRapidFire();
                clearRubber();
                addRubber("TEXTSINGLE");
                setRubberMode("TEXTSINGLE");
                setRubberPoint("TEXT_POINT", global.textX, global.textY);
                setRubberPoint("TEXT_HEIGHT_ROTATION", global.textHeight, global.textRotation);
                setRubberText("TEXT_FONT", global.textFont);
                setRubberText("TEXT_JUSTIFY", global.textJustify);
                setRubberText("TEXT_RAPID", global.text);
            }
        }
        else {
            //Do nothing, as we are in rapidFire mode now.
        }
    }
    else if (global.mode == global.mode_RAPID) {
        if (str == "RAPID_ENTER") {
            if (global.text == "") {
                endCommand();
            }
            else {
                vulcanize();
                endCommand(); //TODO: Rather than ending the command, calculate where the next line would be and modify the x/y to the new point
            }
        }
        else {
            global.text = str;
            setRubberText("TEXT_RAPID", global.text);
        }
    }
}

==> commands/snowflake.cpp <==
//Command: Snowflake

var global = {}; //Required
global.numPoints = 2048; //Default //TODO: min:64 max:8192
global.cx;
global.cy;
global.sx = 0.04; //Default
global.sy = 0.04; //Default
global.numPoints;
global.mode;

//enums
global.mode_NUM_POINTS = 0;
global.mode_XSCALE     = 1;
global.mode_YSCALE     = 2;

//NOTE: main() is run every time the command is started.
//      Use it to reset variables so they are ready to go.
function main()
{
    initCommand();
    clearSelection();
    global.cx = NaN;
    global.cy = NaN;
    global.mode = global.mode_NUM_POINTS;

    addRubber("POLYGON");
    setRubberMode("POLYGON");
    updateSnowflake(global.numPoints, global.sx, global.sy);
    spareRubber("POLYGON");
    endCommand();
}

//NOTE: click() is run only for left clicks.
//      Middle clicks are used for panning.
//      Right clicks bring up the context menu.
function click(x, y)
{
}

//NOTE: move() is optional. It is run only after
//      enableMoveRapidFire() is called. It
//      will be called every time the mouse moves until
//      disableMoveRapidFire() is called.
function move(x, y)
{
}

//NOTE: context() is run when a context menu entry is chosen.
function context(str)
{
    todo("SNOWFLAKE", "context()");
}

//NOTE: prompt() is run when Enter is pressed.
//      appendPromptHistory is automatically called before prompt()
//      is called so calling it is only needed for erroneous input.
//      Any text in the command prompt is sent as an uppercase string.
function prompt(str)
{
}

function updateSnowflake(numPts, xScale, yScale)
{
    var i;
    var t;
    var xx = NaN;
    var yy = NaN;
    var two_pi = 2*Math.PI;

    for(i = 0; i <= numPts; i++) {
        t = two_pi/numPts*i; 

//Snowflake Curve with t [0,2pi]

xx = 4/7*Math.sin(20/11-318*t)+
3/13*Math.sin(19/11-317*t)+
3/5*Math.sin(21/16-316*t)+
1/6*Math.sin(17/5-315*t)+
2/9*Math.sin(20/19-314*t)+
5/9*Math.sin(35/9-313*t)+
7/12*Math.sin(9/8-310*t)+
5/16*Math.sin(33/8-309*t)+
5/11*Math.sin(31/11-308*t)+
4/7*Math.sin(3/8-307*t)+
4/11*Math.sin(9/8-306*t)+
7/8*Math.sin(21/11-305*t)+
2/3*Math.sin(55/13-304*t)+
5/9*Math.sin(17/7-303*t)+
3/10*Math.sin(3/13-302*t)+
4/11*Math.sin(60/17-301*t)+
6/11*Math.sin(48/11-300*t)+
9/19*Math.sin(1/6-299*t)+
4/5*Math.sin(19/11-298*t)+
7/13*Math.sin(25/8-297*t)+
7/11*Math.sin(19/7-296*t)+
1/2*Math.sin(1-295*t)+
4/9*Math.sin(24/11-294*t)+
1/3*Math.sin(7/2-291*t)+
6/17*Math.sin(15/13-290*t)+
11/17*Math.sin(32/7-288*t)+
3/8*Math.sin(33/8-287*t)+
4/7*Math.sin(15/7-286*t)+
4/5*Math.sin(48/11-284*t)+
6/7*Math.sin(10/7-283*t)+
6/7*Math.sin(20/11-282*t)+
3/8*Math.sin(11/7-281*t)+
5/7*Math.sin(23/6-280*t)+
1/21*Math.sin(19/12-279*t)+
4/9*Math.sin(1/5-278*t)+
5/8*Math.sin(5/9-276*t)+
9/10*Math.sin(2/3-274*t)+
5/8*Math.sin(5/11-273*t)+
1/6*Math.sin(9/2-272*t)+
12/25*Math.sin(29/12-271*t)+
7/13*Math.sin(59/15-270*t)+
5/7*Math.sin(23/9-269*t)+
3/4*Math.sin(9/2-268*t)+
5/11*Math.sin(37/9-267*t)+
10/11*Math.sin(11/7-266*t)+
1/3*Math.sin(3/7-264*t)+
7/9*Math.sin(33/17-262*t)+
5/8*Math.sin(9/8-261*t)+
5/8*Math.sin(38/13-260*t)+
11/21*Math.sin(36/13-259*t)+
3/11*Math.sin(1/29-258*t)+
8/15*Math.sin(31/8-257*t)+
2/5*Math.sin(3/13-256*t)+
1/2*Math.sin(47/10-255*t)+
1/10*Math.sin(33/10-254*t)+
2/5*Math.sin(1/2-253*t)+
4/7*Math.sin(33/7-252*t)+
6/17*Math.sin(3/8-250*t)+
5/7*Math.sin(25/9-249*t)+
7/9*Math.sin(35/8-248*t)+
2/7*Math.sin(81/20-247*t)+
5/8*Math.sin(25/6-244*t)+
5/16*Math.sin(11/21-243*t)+
11/13*Math.sin(167/42-242*t)+
11/15*Math.sin(18/5-241*t)+
13/14*Math.sin(37/11-240*t)+
1/4*Math.sin(20/9-239*t)+
9/14*Math.sin(52/15-238*t)+
9/14*Math.sin(17/14-237*t)+
6/13*Math.sin(69/17-236*t)+
5/8*Math.sin(74/21-235*t)+
7/15*Math.sin(76/25-234*t)+
10/11*Math.sin(15/8-232*t)+
5/11*Math.sin(5/9-230*t)+
1/8*Math.sin(8/3-229*t)+
5/9*Math.sin(2/7-227*t)+
4/13*Math.sin(32/9-226*t)+
2/3*Math.sin(45/11-225*t)+
1/30*Math.sin(53/15-223*t)+
7/11*Math.sin(4/11-222*t)+
10/19*Math.sin(31/13-221*t)+
Math.sin(13/7-219*t)+
9/14*Math.sin(33/7-216*t)+
2/3*Math.sin(19/9-215*t)+
3/5*Math.sin(27/11-214*t)+
9/11*Math.sin(43/10-210*t)+
5/7*Math.sin(13/8-209*t)+
5/9*Math.sin(21/5-208*t)+
2/7*Math.sin(14/9-206*t)+
9/8*Math.sin(23/7-205*t)+
18/13*Math.sin(11/9-203*t)+
7/4*Math.sin(47/12-201*t)+
10/7*Math.sin(8/9-200*t)+
7/10*Math.sin(6/11-199*t)+
5/3*Math.sin(7/6-198*t)+
19/11*Math.sin(11/6-196*t)+
15/8*Math.sin(9/8-195*t)+
8/17*Math.sin(9/7-192*t)+
8/3*Math.sin(39/10-191*t)+
23/10*Math.sin(2/7-188*t)+
3/4*Math.sin(3/5-187*t)+
7/12*Math.sin(50/11-185*t)+
57/29*Math.sin(4-184*t)+
9/8*Math.sin(6/7-183*t)+
9/7*Math.sin(15/13-182*t)+
5/13*Math.sin(16/7-181*t)+
18/7*Math.sin(5/14-180*t)+
17/9*Math.sin(35/12-179*t)+
5/4*Math.sin(5/7-178*t)+
22/23*Math.sin(3/4-176*t)+
3/8*Math.sin(48/13-175*t)+
15/11*Math.sin(13/11-174*t)+
25/17*Math.sin(23/5-173*t)+
18/11*Math.sin(19/8-172*t)+
11/16*Math.sin(5/3-170*t)+
39/38*Math.sin(15/7-169*t)+
7/6*Math.sin(36/11-166*t)+
15/11*Math.sin(11/6-163*t)+
17/13*Math.sin(3-162*t)+
11/9*Math.sin(20/7-161*t)+
9/7*Math.sin(35/9-160*t)+
7/6*Math.sin(3/2-159*t)+
8/7*Math.sin(9/10-158*t)+
12/25*Math.sin(13/5-156*t)+
6/13*Math.sin(25/13-154*t)+
9/13*Math.sin(7/8-152*t)+
23/10*Math.sin(33/14-151*t)+
8/11*Math.sin(36/11-150*t)+
15/7*Math.sin(26/7-149*t)+
6/5*Math.sin(53/12-148*t)+
14/11*Math.sin(3/2-147*t)+
9/8*Math.sin(4/3-146*t)+
5/8*Math.sin(18/13-145*t)+
15/7*Math.sin(3/8-143*t)+
5/8*Math.sin(5/6-142*t)+
6/7*Math.sin(35/9-139*t)+
16/13*Math.sin(1/2-138*t)+
9/4*Math.sin(7/2-137*t)+
20/9*Math.sin(15/8-135*t)+
11/8*Math.sin(9/4-134*t)+
Math.sin(19/10-133*t)+
22/7*Math.sin(48/11-132*t)+
23/14*Math.sin(1-131*t)+
19/9*Math.sin(27/8-130*t)+
19/5*Math.sin(20/7-129*t)+
18/5*Math.sin(76/25-128*t)+
27/8*Math.sin(4/5-126*t)+
37/8*Math.sin(3/8-125*t)+
62/11*Math.sin(11/3-124*t)+
49/11*Math.sin(7/6-123*t)+
21/22*Math.sin(23/12-122*t)+
223/74*Math.sin(11/3-121*t)+
11/5*Math.sin(19/5-120*t)+
13/4*Math.sin(33/13-119*t)+
27/8*Math.sin(22/5-117*t)+
24/7*Math.sin(13/7-114*t)+
69/17*Math.sin(18/17-113*t)+
10/9*Math.sin(2/7-112*t)+
133/66*Math.sin(12/7-111*t)+
2/5*Math.sin(47/24-110*t)+
13/5*Math.sin(11/6-108*t)+
16/7*Math.sin(39/11-105*t)+
11/5*Math.sin(25/9-104*t)+
151/50*Math.sin(19/7-103*t)+
19/7*Math.sin(12/5-101*t)+
26/7*Math.sin(101/25-99*t)+
43/21*Math.sin(41/14-98*t)+
13/3*Math.sin(31/9-97*t)+
10/13*Math.sin(1-95*t)+
17/7*Math.sin(39/10-93*t)+
145/48*Math.sin(3-92*t)+
37/6*Math.sin(47/13-91*t)+
5/6*Math.sin(36/13-89*t)+
9/4*Math.sin(3/7-87*t)+
48/13*Math.sin(26/17-86*t)+
7/3*Math.sin(28/19-82*t)+
31/6*Math.sin(8/7-81*t)+
36/7*Math.sin(12/7-80*t)+
38/9*Math.sin(25/9-79*t)+
17/2*Math.sin(37/14-76*t)+
16/3*Math.sin(19/20-75*t)+
81/16*Math.sin(4/5-74*t)+
67/10*Math.sin(19/15-73*t)+
40/11*Math.sin(32/11-72*t)+
71/13*Math.sin(21/20-71*t)+
68/15*Math.sin(46/15-70*t)+
52/15*Math.sin(27/10-69*t)+
57/14*Math.sin(7/8-67*t)+
7/4*Math.sin(42/13-66*t)+
39/11*Math.sin(43/21-65*t)+
30/11*Math.sin(33/8-64*t)+
7/5*Math.sin(20/7-63*t)+
4/7*Math.sin(13/14-62*t)+
39/10*Math.sin(16/9-61*t)+
7/6*Math.sin(137/34-59*t)+
16/13*Math.sin(107/27-58*t)+
26/27*Math.sin(17/5-57*t)+
4/3*Math.sin(9/14-56*t)+
46/11*Math.sin(5/3-55*t)+
11/6*Math.sin(13/4-54*t)+
19/4*Math.sin(17/5-53*t)+
19/7*Math.sin(43/11-52*t)+
25/12*Math.sin(30/7-51*t)+
15/7*Math.sin(5/11-50*t)+
53/5*Math.sin(21/13-49*t)+
62/13*Math.sin(67/15-48*t)+
122/9*Math.sin(48/13-47*t)+
20/13*Math.sin(1-46*t)+
7/6*Math.sin(32/7-43*t)+
12/7*Math.sin(13/25-42*t)+
11/17*Math.sin(9/10-40*t)+
11/9*Math.sin(2-39*t)+
4/3*Math.sin(19/7-38*t)+
12/5*Math.sin(47/11-37*t)+
10/7*Math.sin(12/7-36*t)+
108/17*Math.sin(3/4-35*t)+
25/9*Math.sin(19/5-34*t)+
7/13*Math.sin(22/5-33*t)+
9/4*Math.sin(13/11-32*t)+
181/15*Math.sin(25/11-31*t)+
202/11*Math.sin(57/13-29*t)+
2/11*Math.sin(26/7-28*t)+
129/13*Math.sin(38/15-25*t)+
13/6*Math.sin(1/8-24*t)+
77/13*Math.sin(11/8-23*t)+
19/6*Math.sin(15/7-22*t)+
18/7*Math.sin(29/10-21*t)+
9*Math.sin(13/5-18*t)+
342/7*Math.sin(11/6-17*t)+
3/5*Math.sin(49/11-15*t)+
38/3*Math.sin(19/7-14*t)+
994/9*Math.sin(25/8-13*t)+
22/9*Math.sin(49/12-10*t)+
97/9*Math.sin(1/14-8*t)+
559/7*Math.sin(47/14-7*t)+
19/13*Math.sin(5/6-6*t)+
3*Math.sin(57/17-4*t)+
28/5*Math.sin(1-3*t)+
10/3*Math.sin(22/7-2*t)+
1507/3*Math.sin(29/8-t)-
1407/13*Math.sin(5*t+8/11)-
15/2*Math.sin(9*t+2/5)-
1193/9*Math.sin(11*t+28/27)-
209/15*Math.sin(12*t+2/5)-
116/15*Math.sin(16*t+40/39)-
1105/33*Math.sin(19*t+1/3)-
45/13*Math.sin(20*t+7/6)-
91/46*Math.sin(26*t+4/7)-
43/16*Math.sin(27*t+12/11)-
46/13*Math.sin(30*t+14/9)-
29/10*Math.sin(41*t+3/14)-
31/11*Math.sin(44*t+15/14)-
22/7*Math.sin(45*t+10/7)-
7/8*Math.sin(60*t+22/15)-
54/53*Math.sin(68*t+5/4)-
214/15*Math.sin(77*t+5/9)-
54/11*Math.sin(78*t+1/13)-
47/6*Math.sin(83*t+5/11)-
1/2*Math.sin(84*t+8/7)-
2/3*Math.sin(85*t+4/9)-
7/3*Math.sin(88*t+7/6)-
15/4*Math.sin(90*t+1/6)-
35/6*Math.sin(94*t+17/18)-
77/26*Math.sin(96*t+2/7)-
64/11*Math.sin(100*t+34/23)-
13/6*Math.sin(102*t+14/11)-
19/7*Math.sin(106*t+5/6)-
13/6*Math.sin(107*t+10/11)-
42/13*Math.sin(109*t+8/7)-
69/35*Math.sin(115*t+10/21)-
12/7*Math.sin(116*t+17/16)-
8/3*Math.sin(118*t+5/9)-
1/6*Math.sin(127*t+17/12)-
13/7*Math.sin(136*t+8/7)-
7/10*Math.sin(140*t+7/5)-
15/7*Math.sin(141*t+19/14)-
6/11*Math.sin(144*t+5/16)-
3/2*Math.sin(153*t+9/14)-
6/5*Math.sin(155*t+3/10)-
3/8*Math.sin(157*t+10/11)-
20/11*Math.sin(164*t+19/14)-
7/5*Math.sin(165*t+7/6)-
8/13*Math.sin(167*t+20/13)-
7/8*Math.sin(168*t+3/7)-
5/14*Math.sin(171*t+16/13)-
22/7*Math.sin(177*t+3/13)-
23/8*Math.sin(186*t+7/8)-
13/7*Math.sin(189*t+11/9)-
9/5*Math.sin(190*t+32/21)-
27/28*Math.sin(193*t+1)-
5/12*Math.sin(194*t+1/2)-
44/43*Math.sin(197*t+6/5)-
5/11*Math.sin(202*t+1/5)-
8/7*Math.sin(204*t+1/23)-
16/15*Math.sin(207*t+7/10)-
1/2*Math.sin(211*t+2/5)-
5/8*Math.sin(212*t+3/5)-
10/13*Math.sin(213*t+6/5)-
21/16*Math.sin(217*t+4/3)-
11/5*Math.sin(218*t+24/25)-
2/3*Math.sin(220*t+5/9)-
13/10*Math.sin(224*t+7/8)-
17/8*Math.sin(228*t+1/9)-
3/7*Math.sin(231*t+14/9)-
5/12*Math.sin(233*t+9/11)-
3/5*Math.sin(245*t+4/7)-
2/3*Math.sin(246*t+15/11)-
3/8*Math.sin(251*t+4/7)-
2/9*Math.sin(263*t+19/20)-
1/2*Math.sin(265*t+13/11)-
3/8*Math.sin(275*t+3/2)-
17/35*Math.sin(277*t+9/13)-
3/7*Math.sin(285*t+3/11)-
9/10*Math.sin(289*t+25/19)-
4/9*Math.sin(292*t+20/13)-
12/25*Math.sin(293*t+5/4)-
3/5*Math.sin(311*t+9/8)-
33/32*Math.sin(312*t+1/2);

yy = 3/7*Math.sin(24/11-318*t)+
5/12*Math.sin(3-317*t)+
5/14*Math.sin(21/16-316*t)+
9/19*Math.sin(31/9-315*t)+
2/9*Math.sin(13/6-314*t)+
3/5*Math.sin(9/7-312*t)+
2/5*Math.sin(49/12-311*t)+
1/13*Math.sin(30/7-310*t)+
4/13*Math.sin(19/12-309*t)+
1/3*Math.sin(32/7-307*t)+
5/8*Math.sin(22/5-306*t)+
4/11*Math.sin(25/11-305*t)+
8/15*Math.sin(9/8-304*t)+
1/8*Math.sin(35/9-303*t)+
3/5*Math.sin(51/25-302*t)+
2/5*Math.sin(9/8-301*t)+
4/7*Math.sin(2/7-300*t)+
2/7*Math.sin(50/11-299*t)+
3/13*Math.sin(35/8-297*t)+
5/14*Math.sin(14/5-295*t)+
8/13*Math.sin(47/14-294*t)+
2/9*Math.sin(25/8-293*t)+
8/17*Math.sin(136/45-291*t)+
2/7*Math.sin(17/7-290*t)+
3/5*Math.sin(8/7-288*t)+
3/13*Math.sin(19/8-286*t)+
6/11*Math.sin(10/19-285*t)+
9/10*Math.sin(121/40-283*t)+
8/5*Math.sin(21/5-282*t)+
1/10*Math.sin(87/25-281*t)+
7/13*Math.sin(22/7-279*t)+
3/7*Math.sin(8/5-278*t)+
4/5*Math.sin(3/14-277*t)+
7/10*Math.sin(19/13-276*t)+
1/5*Math.sin(6/13-274*t)+
7/10*Math.sin(20/9-273*t)+
1/3*Math.sin(9/4-272*t)+
4/13*Math.sin(47/11-271*t)+
18/17*Math.sin(22/7-269*t)+
1/7*Math.sin(31/9-268*t)+
7/10*Math.sin(43/17-267*t)+
8/11*Math.sin(24/7-266*t)+
5/8*Math.sin(13/6-264*t)+
9/10*Math.sin(17/13-262*t)+
4/11*Math.sin(31/8-261*t)+
1/5*Math.sin(66/19-260*t)+
1/10*Math.sin(23/5-259*t)+
3/10*Math.sin(66/19-255*t)+
1/8*Math.sin(6/7-253*t)+
9/13*Math.sin(16/5-252*t)+
3/7*Math.sin(8/9-251*t)+
4/11*Math.sin(30/13-250*t)+
7/11*Math.sin(66/19-247*t)+
1/19*Math.sin(2-246*t)+
1/4*Math.sin(16/7-245*t)+
8/17*Math.sin(41/10-244*t)+
15/16*Math.sin(2/11-240*t)+
5/7*Math.sin(19/18-239*t)+
1/6*Math.sin(5/12-238*t)+
5/11*Math.sin(16/17-236*t)+
3/10*Math.sin(25/12-235*t)+
8/17*Math.sin(16/7-233*t)+
5/8*Math.sin(47/12-231*t)+
9/11*Math.sin(11/8-230*t)+
3/11*Math.sin(33/7-229*t)+
9/10*Math.sin(20/7-226*t)+
4/9*Math.sin(39/14-225*t)+
4/9*Math.sin(10/9-224*t)+
6/7*Math.sin(19/13-222*t)+
7/9*Math.sin(29/7-221*t)+
8/11*Math.sin(33/8-220*t)+
16/9*Math.sin(2/7-219*t)+
25/14*Math.sin(1/8-218*t)+
8/11*Math.sin(5/9-217*t)+
9/11*Math.sin(11/10-216*t)+
21/13*Math.sin(27/7-215*t)+
3/7*Math.sin(1/12-213*t)+
13/9*Math.sin(15/16-212*t)+
23/8*Math.sin(1/8-210*t)+
Math.sin(32/11-209*t)+
9/13*Math.sin(1/9-208*t)+
7/9*Math.sin(33/10-206*t)+
2/3*Math.sin(9/4-205*t)+
3/4*Math.sin(1/2-204*t)+
3/13*Math.sin(11/17-203*t)+
3/7*Math.sin(31/12-202*t)+
19/12*Math.sin(17/8-201*t)+
7/8*Math.sin(75/19-200*t)+
6/5*Math.sin(21/10-198*t)+
3/2*Math.sin(7/5-194*t)+
28/27*Math.sin(3/2-193*t)+
4/9*Math.sin(16/5-192*t)+
22/13*Math.sin(13/6-189*t)+
18/11*Math.sin(19/10-188*t)+
Math.sin(7/6-187*t)+
16/7*Math.sin(13/11-186*t)+
9/5*Math.sin(11/9-184*t)+
16/11*Math.sin(2/5-183*t)+
10/13*Math.sin(10/3-182*t)+
9/7*Math.sin(38/9-181*t)+
45/13*Math.sin(8/9-180*t)+
7/9*Math.sin(35/8-179*t)+
2/3*Math.sin(35/8-176*t)+
10/7*Math.sin(6/19-175*t)+
40/13*Math.sin(15/7-174*t)+
20/13*Math.sin(1/2-173*t)+
3/11*Math.sin(20/7-171*t)+
17/16*Math.sin(50/11-169*t)+
2/9*Math.sin(1/31-168*t)+
4/9*Math.sin(7/2-165*t)+
1/12*Math.sin(26/17-164*t)+
21/22*Math.sin(27/26-163*t)+
13/12*Math.sin(17/8-162*t)+
19/14*Math.sin(39/10-160*t)+
18/11*Math.sin(5/7-159*t)+
3/5*Math.sin(15/14-158*t)+
11/9*Math.sin(35/8-157*t)+
5/8*Math.sin(30/7-156*t)+
3/2*Math.sin(28/11-155*t)+
4/5*Math.sin(5/11-151*t)+
25/19*Math.sin(11/10-150*t)+
10/11*Math.sin(11/14-148*t)+
13/9*Math.sin(7/4-147*t)+
7/13*Math.sin(19/6-146*t)+
1/5*Math.sin(37/14-145*t)+
11/8*Math.sin(42/13-144*t)+
20/11*Math.sin(32/9-143*t)+
2/3*Math.sin(22/5-141*t)+
10/11*Math.sin(9/7-140*t)+
8/7*Math.sin(23/9-138*t)+
5/2*Math.sin(9/19-137*t)+
7/5*Math.sin(193/48-136*t)+
5/8*Math.sin(67/66-135*t)+
8/7*Math.sin(7/15-134*t)+
13/6*Math.sin(13/7-133*t)+
19/7*Math.sin(16/5-132*t)+
16/7*Math.sin(39/11-131*t)+
28/17*Math.sin(69/35-130*t)+
84/17*Math.sin(7/8-129*t)+
114/23*Math.sin(10/9-128*t)+
29/11*Math.sin(1/7-127*t)+
63/10*Math.sin(65/32-124*t)+
74/17*Math.sin(37/16-121*t)+
31/16*Math.sin(35/11-120*t)+
19/5*Math.sin(23/12-119*t)+
82/27*Math.sin(27/7-118*t)+
49/11*Math.sin(8/3-117*t)+
29/14*Math.sin(63/16-116*t)+
9/13*Math.sin(35/8-114*t)+
29/19*Math.sin(5/4-113*t)+
13/7*Math.sin(20/7-112*t)+
9/7*Math.sin(11/23-111*t)+
19/8*Math.sin(27/26-110*t)+
Math.sin(4/7-109*t)+
119/40*Math.sin(22/5-108*t)+
7/5*Math.sin(47/46-107*t)+
5/3*Math.sin(1/6-106*t)+
2*Math.sin(14/5-105*t)+
7/3*Math.sin(10/3-104*t)+
3/2*Math.sin(15/4-103*t)+
19/11*Math.sin(3/4-102*t)+
74/17*Math.sin(13/10-99*t)+
98/33*Math.sin(26/11-98*t)+
36/11*Math.sin(13/3-97*t)+
43/12*Math.sin(26/25-96*t)+
13/2*Math.sin(3/13-95*t)+
6/7*Math.sin(24/7-94*t)+
16/5*Math.sin(6/5-93*t)+
5/7*Math.sin(9/14-92*t)+
55/12*Math.sin(27/14-90*t)+
15/11*Math.sin(14/3-88*t)+
7/3*Math.sin(7/10-87*t)+
11/4*Math.sin(2/9-86*t)+
13/4*Math.sin(35/12-84*t)+
26/9*Math.sin(38/9-83*t)+
7/2*Math.sin(5/7-82*t)+
31/8*Math.sin(27/8-78*t)+
91/6*Math.sin(35/8-77*t)+
37/5*Math.sin(7/10-76*t)+
70/13*Math.sin(17/11-73*t)+
76/25*Math.sin(56/19-70*t)+
19/8*Math.sin(17/8-68*t)+
59/13*Math.sin(42/17-67*t)+
28/17*Math.sin(49/13-64*t)+
9/7*Math.sin(79/17-63*t)+
1/8*Math.sin(7/11-62*t)+
39/8*Math.sin(49/15-61*t)+
53/18*Math.sin(33/8-59*t)+
9/7*Math.sin(41/9-58*t)+
8/7*Math.sin(65/14-57*t)+
10/11*Math.sin(16/7-56*t)+
68/13*Math.sin(42/13-55*t)+
21/10*Math.sin(7/8-54*t)+
6/7*Math.sin(41/14-53*t)+
31/11*Math.sin(55/12-51*t)+
59/17*Math.sin(27/7-50*t)+
124/9*Math.sin(37/11-49*t)+
24/11*Math.sin(3/5-48*t)+
65/6*Math.sin(12/5-47*t)+
11/7*Math.sin(49/11-45*t)+
13/25*Math.sin(11/13-42*t)+
7/4*Math.sin(5/8-40*t)+
43/42*Math.sin(2/5-39*t)+
20/9*Math.sin(4/7-38*t)+
19/8*Math.sin(4/11-37*t)+
5/4*Math.sin(15/4-36*t)+
1/5*Math.sin(11/13-34*t)+
12/7*Math.sin(23/5-32*t)+
409/34*Math.sin(39/10-31*t)+
10/7*Math.sin(5/2-30*t)+
180/11*Math.sin(3-29*t)+
23/8*Math.sin(53/12-26*t)+
71/8*Math.sin(56/13-25*t)+
12/5*Math.sin(10/21-24*t)+
10/3*Math.sin(34/9-22*t)+
27/16*Math.sin(12/11-21*t)+
49/6*Math.sin(13/7-20*t)+
69/2*Math.sin(19/14-19*t)+
475/9*Math.sin(3/10-17*t)+
68/13*Math.sin(57/28-16*t)+
40/17*Math.sin(1/6-15*t)+
77/13*Math.sin(29/11-12*t)+
4954/39*Math.sin(15/4-11*t)+
1075/11*Math.sin(4-5*t)+
191/24*Math.sin(5/4-4*t)+
84/17*Math.sin(2/7-3*t)-
12/5*Math.sin(74*t)-
4/5*Math.sin(166*t)-
1523/3*Math.sin(t+12/11)-
25/3*Math.sin(2*t+17/18)-
13/8*Math.sin(6*t+1/9)-
5333/62*Math.sin(7*t+9/7)-
56/9*Math.sin(8*t+5/12)-
65/8*Math.sin(9*t+2/5)-
106/9*Math.sin(10*t+1/8)-
1006/9*Math.sin(13*t+11/7)-
67/8*Math.sin(14*t+6/5)-
25/8*Math.sin(18*t+15/11)-
40/11*Math.sin(23*t+1/16)-
4/7*Math.sin(27*t+6/5)-
41/8*Math.sin(28*t+7/12)-
8/5*Math.sin(33*t+5/6)-
137/17*Math.sin(35*t+4/5)-
29/12*Math.sin(41*t+22/15)-
25/9*Math.sin(43*t+6/7)-
12/25*Math.sin(44*t+16/11)-
31/6*Math.sin(46*t+4/3)-
19/5*Math.sin(52*t+16/13)-
19/11*Math.sin(60*t+8/17)-
16/7*Math.sin(65*t+6/13)-
25/12*Math.sin(66*t+11/13)-
8/9*Math.sin(69*t+4/11)-
25/7*Math.sin(71*t+7/5)-
11/10*Math.sin(72*t+3/2)-
14/5*Math.sin(75*t+7/9)-
107/14*Math.sin(79*t+3/4)-
67/8*Math.sin(80*t+2/11)-
161/27*Math.sin(81*t+5/11)-
55/18*Math.sin(85*t+3/7)-
161/40*Math.sin(89*t+1/21)-
32/7*Math.sin(91*t+38/25)-
Math.sin(100*t+19/20)-
27/5*Math.sin(101*t+2/13)-
26/9*Math.sin(115*t+1/44)-
17/11*Math.sin(122*t+1/16)-
87/22*Math.sin(123*t+2/3)-
37/8*Math.sin(125*t+9/11)-
10/7*Math.sin(126*t+8/7)-
7/8*Math.sin(139*t+3/5)-
3/7*Math.sin(142*t+5/6)-
71/36*Math.sin(149*t+5/16)-
7/6*Math.sin(152*t+1/9)-
63/25*Math.sin(153*t+29/19)-
27/20*Math.sin(154*t+8/15)-
8/15*Math.sin(161*t+12/13)-
5/3*Math.sin(167*t+13/10)-
17/25*Math.sin(170*t+3/5)-
10/9*Math.sin(172*t+3/8)-
5/7*Math.sin(177*t+5/8)-
1/2*Math.sin(178*t+7/6)-
34/13*Math.sin(185*t+5/8)-
11/13*Math.sin(190*t+38/39)-
25/19*Math.sin(191*t+11/8)-
11/12*Math.sin(195*t+18/19)-
51/26*Math.sin(196*t+2/7)-
14/9*Math.sin(197*t+4/11)-
19/12*Math.sin(199*t+1)-
19/11*Math.sin(207*t+11/8)-
6/11*Math.sin(211*t+1/20)-
11/7*Math.sin(214*t+1/14)-
7/13*Math.sin(223*t+8/11)-
3/5*Math.sin(227*t+12/13)-
4/5*Math.sin(228*t+29/19)-
11/10*Math.sin(232*t+2/7)-
1/6*Math.sin(234*t+7/11)-
Math.sin(237*t+60/59)-
5/11*Math.sin(241*t+7/8)-
1/2*Math.sin(242*t+8/7)-
7/15*Math.sin(243*t+15/16)-
5/8*Math.sin(248*t+2/3)-
1/3*Math.sin(249*t+4/11)-
2/3*Math.sin(254*t+8/7)-
10/19*Math.sin(256*t+14/11)-
4/9*Math.sin(257*t+8/11)-
3/4*Math.sin(258*t+3/7)-
Math.sin(263*t+2/7)-
3/10*Math.sin(265*t+1/28)-
1/2*Math.sin(270*t+1)-
12/13*Math.sin(275*t+5/8)-
1/4*Math.sin(280*t+16/13)-
1/10*Math.sin(284*t+5/8)-
13/25*Math.sin(287*t+3/7)-
9/13*Math.sin(289*t+3/5)-
22/23*Math.sin(292*t+17/13)-
9/11*Math.sin(296*t+17/11)-
3/7*Math.sin(298*t+12/11)-
5/6*Math.sin(308*t+1/2)-
7/15*Math.sin(313*t+1/3);

        setRubberPoint("POLYGON_POINT_" + i.toString(), xx*xScale, yy*yScale);
    }

    setRubberText("POLYGON_NUM_POINTS", numPts.toString());
}

==> commands/star.cpp <==
//Command: Star

var global = {}; //Required
global.numPoints = 5; //Default
global.cx;
global.cy;
global.x1;
global.y1;
global.x2;
global.y2;
global.mode;

//enums
global.mode_NUM_POINTS = 0;
global.mode_CENTER_PT  = 1;
global.mode_RAD_OUTER  = 2;
global.mode_RAD_INNER  = 3;

//NOTE: main() is run every time the command is started.
//      Use it to reset variables so they are ready to go.
function main()
{
    initCommand();
    clearSelection();
    global.cx       = NaN;
    global.cy       = NaN;
    global.x1       = NaN;
    global.y1       = NaN;
    global.x2       = NaN;
    global.y2       = NaN;
    global.mode = global.mode_NUM_POINTS;
    setPromptPrefix(qsTr("Enter number of star points") + " {" + global.numPoints.toString() + "}: ");
}

//NOTE: click() is run only for left clicks.
//      Middle clicks are used for panning.
//      Right clicks bring up the context menu.
function click(x, y)
{
    if (global.mode == global.mode_NUM_POINTS) {
        //Do nothing, the prompt controls this.
    }
    else if (global.mode == global.mode_CENTER_PT) {
        global.cx = x;
        global.cy = y;
        global.mode = global.mode_RAD_OUTER;
        setPromptPrefix(qsTr("Specify outer radius of star: "));
        addRubber("POLYGON");
        setRubberMode("POLYGON");
        updateStar(global.cx, global.cy);
        enableMoveRapidFire();
    }
    else if (global.mode == global.mode_RAD_OUTER) {
        global.x1 = x;
        global.y1 = y;
        global.mode = global.mode_RAD_INNER;
        setPromptPrefix(qsTr("Specify inner radius of star: "));
        updateStar(global.x1, global.y1);
    }
    else if (global.mode == global.mode_RAD_INNER) {
        global.x2 = x;
        global.y2 = y;
        disableMoveRapidFire();
        updateStar(global.x2, global.y2);
        spareRubber("POLYGON");
        endCommand();
    }
}

//NOTE: move() is optional. It is run only after
//      enableMoveRapidFire() is called. It
//      will be called every time the mouse moves until
//      disableMoveRapidFire() is called.
function move(x, y)
{
    if (global.mode == global.mode_NUM_POINTS) {
        //Do nothing, the prompt controls this.
    }
    else if (global.mode == global.mode_CENTER_PT) {
        //Do nothing, prompt and click controls this.
    }
    else if (global.mode == global.mode_RAD_OUTER) {
        updateStar(x, y);
    }
    else if (global.mode == global.mode_RAD_INNER) {
        updateStar(x, y);
    }
}

//NOTE: context() is run when a context menu entry is chosen.
function context(str)
{
    todo("STAR", "context()");
}

//NOTE: prompt() is run when Enter is pressed.
//      appendPromptHistory is automatically called before prompt()
//      is called so calling it is only needed for erroneous input.
//      Any text in the command prompt is sent as an uppercase string.
function prompt(str)
{
    if (global.mode == global.mode_NUM_POINTS) {
        if (str == "" && global.numPoints >= 3 && global.numPoints <= 1024) {
            setPromptPrefix(qsTr("Specify center point: "));
            global.mode = global.mode_CENTER_PT;
        }
        else {
            var tmp = Number(str);
            if (isNaN(tmp) || !isInt(tmp) || tmp < 3 || tmp > 1024) {
                alert(qsTr("Requires an integer between 3 and 1024."));
                setPromptPrefix(qsTr("Enter number of star points") + " {" + global.numPoints.toString() + "}: ");
            }
            else {
                global.numPoints = tmp;
                setPromptPrefix(qsTr("Specify center point: "));
                global.mode = global.mode_CENTER_PT;
            }
        }
    }
    else if (global.mode == global.mode_CENTER_PT) {
        var strList = str.split(",");
        if (isNaN(strList[0]) || isNaN(strList[1])) {
            alert(qsTr("Invalid point."));
            setPromptPrefix(qsTr("Specify center point: "));
        }
        else {
            global.cx = Number(strList[0]);
            global.cy = Number(strList[1]);
            global.mode = global.mode_RAD_OUTER;
            setPromptPrefix(qsTr("Specify outer radius of star: "));
            addRubber("POLYGON");
            setRubberMode("POLYGON");
            updateStar(qsnapX(), qsnapY());
            enableMoveRapidFire();
        }
    }
    else if (global.mode == global.mode_RAD_OUTER) {
        var strList = str.split(",");
        if (isNaN(strList[0]) || isNaN(strList[1])) {
            alert(qsTr("Invalid point."));
            setPromptPrefix(qsTr("Specify outer radius of star: "));
        }
        else {
            global.x1 = Number(strList[0]);
            global.y1 = Number(strList[1]);
            global.mode = global.mode_RAD_INNER;
            setPromptPrefix(qsTr("Specify inner radius of star: "));
            updateStar(qsnapX(), qsnapY());
        }
    }
    else if (global.mode == global.mode_RAD_INNER) {
        var strList = str.split(",");
        if (isNaN(strList[0]) || isNaN(strList[1])) {
            alert(qsTr("Invalid point."));
            setPromptPrefix(qsTr("Specify inner radius of star: "));
        }
        else {
            global.x2 = Number(strList[0]);
            global.y2 = Number(strList[1]);
            disableMoveRapidFire();
            updateStar(global.x2, global.y2);
            spareRubber("POLYGON");
            endCommand();
        }
    }
}

function updateStar(x, y)
{
    var distOuter;
    var distInner;
    var angOuter;

    if (global.mode == global.mode_RAD_OUTER) {
        angOuter = calculateAngle(global.cx, global.cy, x, y);
        distOuter = calculateDistance(global.cx, global.cy, x, y);
        distInner = distOuter/2.0;
    }
    else if (global.mode == global.mode_RAD_INNER) {
        angOuter = calculateAngle(global.cx, global.cy, global.x1, global.y1);
        distOuter = calculateDistance(global.cx, global.cy, global.x1, global.y1);
        distInner = calculateDistance(global.cx, global.cy, x, y);
    }

    //Calculate the Star Points
    var angInc = 360.0/(global.numPoints*2);
    var odd = true;
    for(var i = 0; i < global.numPoints*2; i++) {
        var xx;
        var yy;
        if (odd) {
            xx = distOuter*Math.cos((angOuter+(angInc*i))*Math.PI/180.0);
            yy = distOuter*Math.sin((angOuter+(angInc*i))*Math.PI/180.0);
        }
        else {
            xx = distInner*Math.cos((angOuter+(angInc*i))*Math.PI/180.0);
            yy = distInner*Math.sin((angOuter+(angInc*i))*Math.PI/180.0);
        }
        odd = !odd;
        setRubberPoint("POLYGON_POINT_" + i.toString(), global.cx + xx, global.cy + yy);
    }
    setRubberText("POLYGON_NUM_POINTS", (global.numPoints*2 - 1).toString());
}


==> commands/syswindows.cpp <==
//Command: SysWindows

//NOTE: main() is run every time the command is started.
//      Use it to reset variables so they are ready to go.
function main()
{
    initCommand();
    clearSelection();
    setPromptPrefix(qsTr("Enter an option [Cascade/Tile]: "));
}

//NOTE: click() is run only for left clicks.
//      Middle clicks are used for panning.
//      Right clicks bring up the context menu.
function click(x, y)
{
    //Do Nothing
}

//NOTE: context() is run when a context menu entry is chosen.
function context(str)
{
    //Do Nothing
}

//NOTE: prompt() is run when Enter is pressed.
//      appendPromptHistory is automatically called before prompt()
//      is called so calling it is only needed for erroneous input.
//      Any text in the command prompt is sent as an uppercase string.
function prompt(str)
{
    if (str == "C" || str == "CASCADE") {
        windowCascade();
        endCommand();
    }
    else if (str == "T" || str == "TILE") {
        windowTile();
        endCommand();
    }
    else {
        alert(qsTr("Invalid option keyword."));
        setPromptPrefix(qsTr("Enter an option [Cascade/Tile]: "));
    }
}

#endif

ScriptValue
tip_of_the_day_command(ScriptContext *context)
{
    if (!argument_checks(context, "tip_of_the_day", "")) {
        return script_null;
    }
    if (context->context == CONTEXT_MAIN) {
        init_command();
        clear_selection();
    }
    tip_of_the_day();
    end_command();
    return script_null;
}

/*!
 *
 */
ScriptValue
undo_command(ScriptContext *context)
{
    if (!argument_checks(context, "undo", "")) {
        return script_null;
    }
    if (context->context == CONTEXT_MAIN) {
        init_command();
        clear_selection();
    }
    undo();
    end_command();
    return script_null;
}

/*!
 * \brief Report on incomplete features inside commands to developers.
 */
ScriptValue
todo_command(ScriptContext* context)
{
    if (!argument_checks(context, "todo", "s")) {
        return script_null;
    }
    char message[1000];
    sprintf(message, "TODO: %s", context->arguments[0].s);
    alert(message);
    end_command();
    return script_null;
}

ScriptValue
window_cascade_command(ScriptContext *context)
{
    if (!argument_checks(context, "window_cascade", "")) {
        return script_null;
    }
    if (context->context == CONTEXT_MAIN) {
        init_command();
        clear_selection();
    }
    window_cascade();
    end_command();
    return script_null;
}

ScriptValue
window_close_all_command(ScriptContext *context)
{
    if (!argument_checks(context, "window_close_all", "")) {
        return script_null;
    }
    if (context->context == CONTEXT_MAIN) {
        init_command();
        clear_selection();
    }
    window_close_all();
    end_command();
    return script_null;
}


ScriptValue
window_close_command(ScriptContext *context)
{
    if (!argument_checks(context, "window_close", "")) {
        return script_null;
    }
    if (context->context == CONTEXT_MAIN) {
        init_command();
        clear_selection();
    }
    window_close();
    end_command();
    return script_null;
}

ScriptValue
window_next_command(ScriptContext *context)
{
    if (!argument_checks(context, "window_next", "")) {
        return script_null;
    }
    if (context->context == CONTEXT_MAIN) {
        init_command();
        clear_selection();
    }
    window_next();
    end_command();
    return script_null;
}

ScriptValue
window_previous_command(ScriptContext *context)
{
    if (!argument_checks(context, "window_previous", "")) {
        return script_null;
    }
    if (context->context == CONTEXT_MAIN) {
        init_command();
        clear_selection();
    }
    window_previous();
    end_command();
    return script_null;
}

ScriptValue
window_tile_command(ScriptContext *context)
{
    if (!argument_checks(context, "window_tile", "")) {
        return script_null;
    }
    if (context->context == CONTEXT_MAIN) {
        init_command();
        clear_selection();
    }
    window_tile();
    end_command();
    return script_null;
}

ScriptValue
zoom_extents_command(ScriptContext *context)
{
    if (!argument_checks(context, "zoom_extents", "")) {
        return script_null;
    }
    if (context->context == CONTEXT_MAIN) {
        init_command();
    }
    zoom_extents();
    end_command();
    return script_null;
}

ScriptValue
zoom_in_command(ScriptContext *context)
{
    if (!argument_checks(context, "zoom_in", "")) {
        return script_null;
    }
    if (context->context == CONTEXT_MAIN) {
        init_command();
    }
    zoom_in();
    end_command();
    return script_null;
}

ScriptValue
zoom_out_command(ScriptContext *context)
{
    if (!argument_checks(context, "zoom_out", "")) {
        return script_null;
    }
    if (context->context == CONTEXT_MAIN) {
        init_command();
    }
    zoom_out();
    end_command();
    return script_null;
}

