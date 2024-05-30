/*
 * Embroidermodder 2.
 *
 * Copyright 2011-2024 The Embroidermodder Team
 * Embroidermodder 2 is Open Source Software, see LICENSE.md for licensing terms.
 * Visit https://www.libembroidery.org/refman for advice on altering this file,
 * or read the markdown version in embroidermodder2/docs/refman.
 *
 * Script
 */

#include <string.h>

#include "embroidermodder.h"

QString index_name[] = {
    "one",
    "two",
    "three",
    "four",
    "five",
    "six",
    "seven"
};

QString index_th_name[] = {
    "first",
    "second",
    "third",
    "fourth",
    "fifth",
    "sixth",
    "seventh"
};

ScriptValue script_null = {
    .r = 0.0f,
    .i = 0,
    .b = false,
    .s = "",
    .label = "NULL",
    .type = SCRIPT_NULL
};

ScriptValue script_true = {
    .r = 0.0f,
    .i = 1,
    .b = true,
    .s = "",
    .label = "true",
    .type = SCRIPT_BOOL
};

ScriptValue script_false = {
    .r = 0.0f,
    .i = 0,
    .b = false,
    .s = "",
    .label = "false",
    .type = SCRIPT_BOOL
};

int
argument_checks(ScriptEnv *context, QString function, const char *args)
{
    int i;
    if (context->argumentCount != strlen(args)) {
        QString s = function + "() requires " +  + " arguments";
        debug_message(s);
        return 0;
    }
    for (i=0; args[i]; i++) {
        if (args[i] == 'r') {
            if (context->argument[i].type != SCRIPT_REAL) {
                QString s = "TYPE_ERROR, " + function + "(): " + index_th_name[i] + " argument is not a real number.";
                debug_message(s);
                return 0;
            }
            qreal variable = context->argument[i].r;
            if (qIsNaN(variable)) {
                QString s = "TYPE_ERROR, " + function + "(): " + index_th_name[i] + " argument is not a real number.";
                debug_message(s);
                return 0;
            }
        }
        if (args[i] == 'i') {
            if (context->argument[i].type != SCRIPT_INT) {
                QString s = "TYPE_ERROR, " + function + "(): " + index_th_name[i] + " argument is not a integer.";
                debug_message(s);
                return 0;
            }
        }
        if (args[i] == 'b') {
            if (context->argument[i].type != SCRIPT_BOOL) {
                QString s = "TYPE_ERROR, " + function + "(): " + index_th_name[i] + " argument is not a boolean.";
                debug_message(s);
                return 0;
            }
        }
        if (args[i] == 's') {
            if (context->argument[i].type != SCRIPT_STRING) {
                QString s = "TYPE_ERROR, " + function + "(): " + index_th_name[i] + " argument is not a string.";
                debug_message(s);
                return 0;
            }
        }
    }
    return 1;
}

QString
translate(QString msg)
{
    return msg;
}

ScriptEnv *
create_script_env()
{
    ScriptEnv *context = (ScriptEnv*)malloc(sizeof(ScriptEnv));
    context->argumentCount = 0;
    context->n_variables = 0;
    return context;
}

void
add_string_variable(ScriptEnv *context, const char *label, const char *s)
{
    strcpy(context->variable[context->n_variables].label, label);
    strcpy(context->variable[context->n_variables].s, s);
    context->variable[context->n_variables].type = SCRIPT_STRING;
    context->n_variables++;
}

void
add_int_variable(ScriptEnv *context, const char *label, int i)
{
    strcpy(context->variable[context->n_variables].label, label);
    context->variable[context->n_variables].i = i;
    context->variable[context->n_variables].type = SCRIPT_INT;
    context->n_variables++;
}

void
add_real_variable(ScriptEnv *context, const char *label, double r)
{
    strcpy(context->variable[context->n_variables].label, label);
    context->variable[context->n_variables].r = r;
    context->variable[context->n_variables].type = SCRIPT_REAL;
    context->n_variables++;
}

const char *
script_get_string(ScriptEnv *context, const char *label)
{
    int i;
    for (i=0; i<context->n_variables; i++) {
        if (!strcmp(context->variable[i].label, label)) {
            return context->variable[i].s;
        }
    }
    return "ERROR: string not found.";
}

int
script_get_int(ScriptEnv *context, const char *label)
{
    int i;
    for (i=0; i<context->n_variables; i++) {
        if (!strcmp(context->variable[i].label, label)) {
            return context->variable[i].i;
        }
    }
    return -1;
}

double
script_get_real(ScriptEnv *context, const char *label)
{
    int i;
    for (i=0; i<context->n_variables; i++) {
        if (!strcmp(context->variable[i].label, label)) {
            return context->variable[i].r;
        }
    }
    return -1.0;
}

int
script_set_string(ScriptEnv *context, const char *label, const char *s)
{
    int i;
    for (i=0; i<context->n_variables; i++) {
        if (!strcmp(context->variable[i].label, label)) {
            strcpy(context->variable[i].s, s);
            return 1;
        }
    }
    return 0;
}

int
script_set_int(ScriptEnv *context, const char *label, int x)
{
    int i;
    for (i=0; i<context->n_variables; i++) {
        if (!strcmp(context->variable[i].label, label)) {
            context->variable[i].i = x;
            return 1;
        }
    }
    return 0;
}

int
script_set_real(ScriptEnv *context, const char *label, double r)
{
    int i;
    for (i=0; i<context->n_variables; i++) {
        if (!strcmp(context->variable[i].label, label)) {
            context->variable[i].r = r;
            return 1;
        }
    }
    return 0;
}

void
free_script_env(ScriptEnv* context)
{
    free(context);
}

ScriptValue
script_bool(bool b)
{
    ScriptValue value;
    value.type = SCRIPT_BOOL;
    value.b = b;
    return value;
}

ScriptValue
script_int(int i)
{
    ScriptValue value;
    value.type = SCRIPT_INT;
    value.i = i;
    return value;
}

ScriptValue
script_real(qreal r)
{
    ScriptValue value;
    value.type = SCRIPT_REAL;
    value.r = r;
    return value;
}

ScriptValue
script_string(const char *s)
{
    ScriptValue value;
    value.type = SCRIPT_STRING;
    strcpy(value.s, s);
    return value;
}

void
debug_message(QString s)
{
    _main->nativeAppendPromptHistory(s);
}

/* These pack the arguments for function calls in the command environment. */
ScriptEnv *
add_string_argument(ScriptEnv *context, const char *s)
{
    strcpy(context->argument[context->argumentCount].s, s);
    context->argument[context->argumentCount].type = SCRIPT_STRING;
    context->argumentCount++;
}

ScriptEnv *
add_real_argument(ScriptEnv *context, double r)
{
    context->argument[context->argumentCount].r = r;
    context->argument[context->argumentCount].type = SCRIPT_REAL;
    context->argumentCount++;
}

ScriptEnv *
add_int_argument(ScriptEnv *context, int i)
{
    context->argument[context->argumentCount].i = i;
    context->argument[context->argumentCount].type = SCRIPT_INT;
    context->argumentCount++;
}

/* FIXME: detect types.
 */
ScriptValue
command_prompt(const char *line)
{
    QRegExp split_char(" ");
    QStringList line_list = QString(line).split(split_char);
    if (_main->command_map.contains(line_list[0])) {
        ScriptEnv* context = create_script_env();
        int i = 0;
        for (QString argument : line_list) {
            if (i > 0) {
                strcpy(context->argument[i-1].s, qPrintable(argument));
                context->argumentCount++;
            }
        }
        ScriptValue result = _main->command_map[line_list[0]].main(context);
        free_script_env(context);
        return result;
    }
    return script_false;
}
