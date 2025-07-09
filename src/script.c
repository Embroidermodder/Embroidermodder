#include "script.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

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

/* For commands that require no arguments and clear selection we can use this
 * basic template.
 */
#define BASIC_COMMAND(A) \
    ScriptValue \
    A##_command(ScriptContext *context) \
    { \
        if (context->argument_count != 0) { \
            return throwError(GeneralError, #A "() requires zero arguments"); \
        } \
        if (context->context == CONTEXT_MAIN) { \
            init_command(); \
            clear_selection(); \
        } \
        A(); \
        end_command(); \
        return script_null; \
    }

#define BASIC_COMMAND_NOCLR(A) \
    ScriptValue \
    A##_command(ScriptContext *context) \
    { \
        if (context->argument_count != 0) { \
            return throwError(GeneralError, #A "() requires zero arguments"); \
        } \
        if (context->context == CONTEXT_MAIN) { \
            init_command(); \
        } \
        A(); \
        end_command(); \
        return script_null; \
    }

BASIC_COMMAND(about)
BASIC_COMMAND(day)
BASIC_COMMAND(help)
BASIC_COMMAND(icon16)
BASIC_COMMAND(icon24)
BASIC_COMMAND(icon32)
BASIC_COMMAND(icon48)
BASIC_COMMAND(icon64)
BASIC_COMMAND(icon128)
BASIC_COMMAND(new_file)
BASIC_COMMAND(open_file)
BASIC_COMMAND(night)
BASIC_COMMAND(redo)
BASIC_COMMAND(tip_of_the_day)
BASIC_COMMAND(undo)
BASIC_COMMAND(window_cascade)
BASIC_COMMAND(window_close)
BASIC_COMMAND(window_close_all)
BASIC_COMMAND(window_next)
BASIC_COMMAND(window_previous)
BASIC_COMMAND(window_tile)

BASIC_COMMAND_NOCLR(pan_down)
BASIC_COMMAND_NOCLR(pan_left)
BASIC_COMMAND_NOCLR(pan_right)
BASIC_COMMAND_NOCLR(pan_up)
BASIC_COMMAND_NOCLR(zoom_extents)
BASIC_COMMAND_NOCLR(zoom_in)
BASIC_COMMAND_NOCLR(zoom_out)

/*!
 * \brief javaAlert
 * \param context
 * \return
 */
ScriptValue
alert_command(ScriptContext* context)
{
    if (context->argument_count != 1) {
        return throwError(GeneralError, "alert() requires one argument");
    }
    if (!is_string(context->arguments[0])) {
        return throwError(TypeError, "alert(): first argument is not a string");
    }

    alert(context->arguments[0].s);
    return script_null;
}

/*!
 * \brief javaBlinkPrompt
 * \param context
 * \return
 */
ScriptValue
blink_prompt_command(ScriptContext* context)
{
    if (context->argument_count != 0) {
        return throwError(GeneralError,
            "blinkPrompt() requires zero arguments");
    }

    blink_prompt();
    return script_null;
}

/*!
 * \brief javaDebug
 */
ScriptValue
debug_command(ScriptContext* context)
{
    if (context->argument_count != 1) {
        return throwError(NotEnoughArgumentsError,
            "debug() requires one argument");
    }
    if (!is_string(context->arguments[0])) {
        return throwError(TypeError,
            "debug(): first argument is not a string");
    }

    debug(context->arguments[0].s);
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
    if (context->argument_count != 1) {
        return throwError(GeneralError,
            "enablePromptRapidFire() requires one argument");
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
    if (context->argument_count != 1) {
        return throwError(GeneralError,
            "enablePromptRapidFire() requires one argument");
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
 * \brief Report on incomplete features inside commands to developers.
 */
ScriptValue
todo_command(ScriptContext* context)
{
    if (context->argument_count != 2) {
        return throwError(NotEnoughArgumentsError, "todo() requires two arguments");
    }
    if (!is_string(context->arguments[0])) {
        return throwError(TypeError, "todo(): first argument is not a string");
    }
    if (!is_string(context->arguments[1])) {
        return throwError(TypeError, "todo(): second argument is not a string");
    }

    char message[1000];
    sprintf(message, "TODO: (%s) %s", context->arguments[0].s,
        context->arguments[1].s);
    alert(message);
    end_command();
    return script_null;
}

