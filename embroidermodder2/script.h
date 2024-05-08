#ifndef SCRIPT_H
#define SCRIPT_H

#include <QString>

#include <vector>

#define MAX_ARGS                  10
#define MAX_STRING_LENGTH        200

#define SCRIPT_NULL                0
#define SCRIPT_INT                 1
#define SCRIPT_REAL                2
#define SCRIPT_COMMAND             3
#define SCRIPT_STRING              4
#define SCRIPT_BOOL                5

typedef struct ScriptValue_ {
    qreal r;
    int i;
    bool b;
    QString s;
    QString label;
    std::vector<struct ScriptValue_> children;
    char type;
} ScriptValue;

typedef struct ScriptEnv_ {
    ScriptValue *commands;
    int n_commands;
    ScriptValue argument[MAX_ARGS];
    int argumentCount;
} ScriptEnv;

typedef struct Command_ {
    ScriptValue (*main)(ScriptEnv *context);
    ScriptValue (*click)(ScriptEnv *context);
    ScriptValue (*context)(ScriptEnv *context);
    ScriptValue (*prompt)(ScriptEnv *context);
    const char *icon;
    const char *menu_name;
    int menu_position;
    const char *toolbar_name;
    int toolbar_position;
    const char *tooltip;
    const char *statustip;
    const char *alias;
} Command;

ScriptValue script_bool(bool b);
ScriptValue script_int(int i);
ScriptValue script_real(qreal r);
ScriptValue script_string(const char *s);

#endif // SCRIPT_H
