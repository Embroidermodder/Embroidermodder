/*
 * Embroidermodder 2.
 *
 * Copyright 2013-2024 The Embroidermodder Team
 * Embroidermodder 2 is Open Source Software.
 * See LICENSE for licensing terms.
 *
 * A Command is a bundle of actions and data: .
 */

#include <QString>
#include <QDebug>

#include <string.h>

#include "commands.h"

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
    context->n_commands = 0;
    return context;
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

ScriptValue
throwError(QString s)
{
    debug_message(s);
    return script_false;
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

/* Simple Context-Insensitive Commands
 * -----------------------------------------------------------
 */

/* ACTION_ALERT is a prompt-only command. */
ScriptValue
alert_generic(ScriptEnv *context)
{
    if (!argument_checks(context, "alert", "s")) {
        return script_false;
    }

    _main->nativeInitCommand();
    _main->nativeAlert(QSTR(0));
    _main->nativeEndCommand();
    return script_null;
}

ScriptValue
clear_generic(ScriptEnv* context)
{
    if (!argument_checks(context, "clear_generic", "")) {
        return script_false;
    }

    _main->nativeInitCommand();
    _main->nativeClearSelection();
    _main->nativeEndCommand();
    return script_null;
}

ScriptValue
day_generic(ScriptEnv *context)
{
    _main->nativeInitCommand();
    _main->nativeClearSelection();
    _main->nativeDayVision();
    _main->nativeEndCommand();
    return script_null;
}

ScriptValue
do_nothing(ScriptEnv *context)
{
    if (!argument_checks(context, "do_nothing", "")) {
        return script_false;
    }
    return script_true;
}

/* GET is a prompt-only Command. */
ScriptValue
get_prompt(ScriptEnv* context)
{
    if (!argument_checks(context, "get_prompt", "s")) {
        return script_false;
    }

    QString value = QSTR(0);

    _main->nativeInitCommand();

    if (value == "MOUSEX") {
        QGraphicsScene* scene = _main->activeScene();
        if (!scene) {
            _main->nativeEndCommand();
            return script_false;
        }
        ScriptValue r = script_real(scene->property(SCENE_MOUSE_POINT).toPointF().x());
        //_main->qDebug("mouseY: %.50f", r.r);
        _main->nativeEndCommand();
        return r;
    }
    else if (value == "MOUSEY") {
        QGraphicsScene* scene = _main->activeScene();
        if (!scene) {
            _main->nativeEndCommand();
            return script_false;
        }
        ScriptValue r = script_real(-scene->property(SCENE_MOUSE_POINT).toPointF().y());
        //_main->qDebug("mouseY: %.50f", r.r);
        _main->nativeEndCommand();
        return r;
    }
    else if (value == "TEXTANGLE") {
        return script_real(_main->textAngle());
    }
    else if (value == "TEXTBOLD") {
        return script_bool(_main->textBold());
    }
    else if (value == "TEXTITALIC") {
        return script_bool(_main->textItalic());
    }
    else if (value == "TEXTFONT") {
        return script_string(qPrintable(_main->textFont()));
    }
    else if (value == "TEXTOVERLINE") {
        return script_real(_main->textOverline());
    }
    else if (value == "TEXTSIZE") {
        return script_real(_main->textSize());
    }
    else if (value == "TEXTSTRIKEOUT") {
        return script_real(_main->textStrikeOut());
    }
    else if (value == "TEXTUNDERLINE") {
        return script_bool(_main->textUnderline());
    }

    _main->nativeEndCommand();
    return script_null;
}

ScriptValue
icon128_generic(ScriptEnv *context)
{
    if (!argument_checks(context, "icon128_generic", "")) {
        return script_false;
    }
    _main->nativeInitCommand();
    _main->nativeClearSelection();
    _main->icon128();
    _main->nativeEndCommand();
    return script_null;
}

ScriptValue
icon16_generic(ScriptEnv * context)
{
    if (!argument_checks(context, "icon16_generic", "")) {
        return script_false;
    }
    _main->nativeInitCommand();
    _main->nativeClearSelection();
    _main->icon16();
    _main->nativeEndCommand();
    return script_null;
}

ScriptValue
icon24_generic(ScriptEnv * context)
{
    if (!argument_checks(context, "icon24_generic", "")) {
        return script_false;
    }
    _main->nativeInitCommand();
    _main->nativeClearSelection();
    _main->icon24();
    _main->nativeEndCommand();
    return script_null;
}

ScriptValue
icon32_generic(ScriptEnv * context)
{
    if (!argument_checks(context, "icon32_generic", "")) {
        return script_false;
    }
    _main->nativeInitCommand();
    _main->nativeClearSelection();
    _main->icon32();
    _main->nativeEndCommand();
    return script_null;
}

ScriptValue
icon48_generic(ScriptEnv * context)
{
    if (!argument_checks(context, "icon48_generic", "")) {
        return script_false;
    }
    _main->nativeInitCommand();
    _main->nativeClearSelection();
    _main->icon48();
    _main->nativeEndCommand();
    return script_null;
}

ScriptValue
icon64_generic(ScriptEnv * context)
{
    if (!argument_checks(context, "icon64_generic", "")) {
        return script_false;
    }
    _main->nativeInitCommand();
    _main->nativeClearSelection();
    _main->icon64();
    _main->nativeEndCommand();
    return script_null;
}

ScriptValue
new_generic(ScriptEnv * context)
{
    _main->nativeInitCommand();
    _main->nativeClearSelection();
    _main->nativeNewFile();
    _main->nativeEndCommand();
    return script_null;
}

/* FIXME */
ScriptValue
stub_implement(const char *function)
{

    return script_true;
}

/* All of these need to be filed into the commands/ directory.
 * -----------------------------------------------------------
 */

ScriptValue
javaTodo(ScriptEnv* context)
{
    if (!argument_checks(context, "debug", "ss")) {
        return script_false;
    }
    QString s = "TODO: (" + QSTR(0) + ") " + QSTR(1);
    _main->nativeAlert(s);
    _main->nativeEndCommand();
    return script_null;
}

ScriptValue
javaBlinkPrompt(ScriptEnv* context)
{
    if (!argument_checks(context, "debug", "")) {
        return script_false;
    }

    _main->nativeBlinkPrompt();
    return script_null;
}

ScriptValue
javaSetPromptPrefix(ScriptEnv* context)
{
    if (!argument_checks(context, "debug", "s")) {
        return script_false;
    }
    _main->nativeSetPromptPrefix(QSTR(0));
    return script_null;
}

ScriptValue
javaAppendPromptHistory(ScriptEnv* context)
{
    int args = context->argumentCount;
    if (args == 0) {
        _main->nativeAppendPromptHistory(QString());
    }
    else if (args == 1) {
        _main->nativeAppendPromptHistory(QSTR(0));
    }
    else {
        debug_message("appendPromptHistory() requires one or zero arguments");
        return script_false;
    }
    return script_null;
}

ScriptValue
javaEnablePromptRapidFire(ScriptEnv* context)
{
    if (!argument_checks(context, "debug", "")) {
        return script_false;
    }

    _main->nativeEnablePromptRapidFire();
    return script_null;
}

ScriptValue
javaDisablePromptRapidFire(ScriptEnv* context)
{
    if (!argument_checks(context, "debug", "")) {
        return script_false;
    }

    _main->nativeDisablePromptRapidFire();
    return script_null;
}

ScriptValue
javaEnableMoveRapidFire(ScriptEnv* context)
{
    if (!argument_checks(context, "debug", "")) {
        return script_false;
    }

    _main->nativeEnableMoveRapidFire();
    return script_null;
}

ScriptValue
javaDisableMoveRapidFire(ScriptEnv* context)
{
    if (!argument_checks(context, "debug", "")) {
        return script_false;
    }

    _main->nativeDisableMoveRapidFire();
    return script_null;
}

ScriptValue
javaInitCommand(ScriptEnv* context)
{
    if (!argument_checks(context, "debug", "")) {
        return script_false;
    }

    _main->nativeInitCommand();
    return script_null;
}

ScriptValue
javaEndCommand(ScriptEnv* context)
{
    if (!argument_checks(context, "debug", "")) {
        return script_false;
    }

    _main->nativeEndCommand();
    return script_null;
}

ScriptValue
javaNewFile(ScriptEnv* context)
{
    if (!argument_checks(context, "debug", "")) {
        return script_false;
    }

    _main->newFile();
    return script_null;
}

ScriptValue
javaOpenFile(ScriptEnv* context)
{
    if (!argument_checks(context, "debug", "")) {
        return script_false;
    }

    _main->openFile();
    return script_null;
}

ScriptValue
javaExit(ScriptEnv* context)
{
    if (!argument_checks(context, "debug", "")) {
        return script_false;
    }

    _main->exit();
    return script_null;
}

ScriptValue
javaHelp(ScriptEnv* context)
{
    if (!argument_checks(context, "debug", "")) {
        return script_false;
    }

    _main->help();
    return script_null;
}

ScriptValue
javaTipOfTheDay(ScriptEnv* context)
{
    if (!argument_checks(context, "debug", "")) {
        return script_false;
    }

    _main->tipOfTheDay();
    return script_null;
}

ScriptValue
javaWindowCascade(ScriptEnv* context)
{
    if (!argument_checks(context, "windowCascade", "")) {
        return script_false;
    }
    _main->mdiArea->cascade();
    return script_null;
}

ScriptValue
javaWindowClose(ScriptEnv* context)
{
    if (!argument_checks(context, "windowClose", "")) {
        return script_false;
    }
    _main->onCloseWindow();
    return script_null;
}

ScriptValue
javaWindowCloseAll(ScriptEnv* context)
{
    if (!argument_checks(context, "windowCloseAll", "")) {
        return script_false;
    }
    _main->mdiArea->closeAllSubWindows();
    return script_null;
}

ScriptValue
javaWindowNext(ScriptEnv* context)
{
    if (!argument_checks(context, "debug", "")) {
        return script_false;
    }
    _main->mdiArea->activateNextSubWindow();
    return script_null;
}

ScriptValue
javaWindowPrevious(ScriptEnv* context)
{
    if (!argument_checks(context, "debug", "")) {
        return script_false;
    }
    _main->mdiArea->activatePreviousSubWindow();
    return script_null;
}

ScriptValue
javaPlatformString(ScriptEnv* context)
{
    if (!argument_checks(context, "debug", "")) {
        return script_false;
    }
    return script_string(qPrintable(_main->platformString()));
}

ScriptValue
javaMessageBox(ScriptEnv* context)
{
    if (!argument_checks(context, "debug", "sss")) {
        return script_false;
    }

    QString type  = QSTR(0).toLower();
    QString title = QSTR(1);
    QString text  = QSTR(2);

    if (type != "critical" && type != "information" && type != "question" && type != "warning") {
        debug_message("UNKNOWN_ERROR messageBox(): first argument must be \"critical\", \"information\", \"question\" or \"warning\".");
        return script_false;
    }

    _main->messageBox(type, title, text);
    return script_null;
}

ScriptValue
javaIsInt(ScriptEnv* context)
{
    if (!argument_checks(context, "debug", "i")) {
        return script_false;
    }
    return script_true;
}

ScriptValue
javaPanLeft(ScriptEnv* context)
{
    if (!argument_checks(context, "debug", "")) {
        return script_false;
    }

    _main->panLeft();
    return script_null;
}

ScriptValue
javaPanRight(ScriptEnv* context)
{
    if (!argument_checks(context, "debug", "")) {
        return script_false;
    }
    _main->panRight();
    return script_null;
}

ScriptValue
javaPanUp(ScriptEnv* context)
{
    if (!argument_checks(context, "debug", "")) {
        return script_false;
    }
    _main->panUp();
    return script_null;
}

ScriptValue
javaPanDown(ScriptEnv* context)
{
    if (!argument_checks(context, "debug", "")) {
        return script_false;
    }
    _main->panDown();
    return script_null;
}

ScriptValue
javaZoomIn(ScriptEnv* context)
{
    if (!argument_checks(context, "debug", "")) {
        return script_false;
    }
    _main->zoomIn();
    return script_null;
}

ScriptValue
javaZoomOut(ScriptEnv* context)
{
    if (!argument_checks(context, "debug", "")) {
        return script_false;
    }
    _main->zoomOut();
    return script_null;
}

ScriptValue
javaZoomExtents(ScriptEnv* context)
{
    if (!argument_checks(context, "debug", "")) {
        return script_false;
    }
    _main->zoomExtents();
    return script_null;
}

ScriptValue
javaPrintArea(ScriptEnv* context)
{
    if (!argument_checks(context, "printArea", "rrrr")) {
        return script_false;
    }
    _main->nativePrintArea(REAL(0), REAL(1), REAL(2), REAL(3));
    return script_null;
}

ScriptValue
javaDayVision(ScriptEnv* context)
{
    if (!argument_checks(context, "day", "")) {
        return script_false;
    }
    _main->dayVision();
    return script_null;
}

ScriptValue
javaNightVision(ScriptEnv* context)
{
    if (!argument_checks(context, "night", "")) {
        return script_false;
    }
    _main->nightVision();
    return script_null;
}

ScriptValue
javaSetBackgroundColor(ScriptEnv* context)
{
    if (!argument_checks(context, "debug", "rrr")) {
        return script_false;
    }
    if (REAL(0) < 0 || REAL(0) > 255) {
        debug_message("UNKNOWN_ERROR setBackgroundColor(): r value must be in range 0-255");
        return script_false;
    }
    if (REAL(1) < 0 || REAL(1) > 255) {
        debug_message("UNKNOWN_ERROR setBackgroundColor(): g value must be in range 0-255");
        return script_false;
    }
    if (REAL(2) < 0 || REAL(2) > 255) {
        debug_message("UNKNOWN_ERROR setBackgroundColor(): b value must be in range 0-255");
        return script_false;
    }

    _main->nativeSetBackgroundColor(REAL(0), REAL(1), REAL(2));
    return script_null;
}

ScriptValue
javaSetCrossHairColor(ScriptEnv* context)
{
    if (!argument_checks(context, "debug", "rrr")) {
        return script_false;
    }

    if (REAL(0) < 0 || REAL(0) > 255) {
        debug_message("UNKNOWN_ERROR setCrossHairColor(): r value must be in range 0-255");
        return script_false;
    }
    if (REAL(1) < 0 || REAL(1) > 255) {
        debug_message("UNKNOWN_ERROR setCrossHairColor(): g value must be in range 0-255");
        return script_false;
    }
    if (REAL(2) < 0 || REAL(2) > 255) {
        debug_message("UNKNOWN_ERROR setCrossHairColor(): b value must be in range 0-255");
        return script_false;
    }

    _main->nativeSetCrossHairColor(REAL(0), REAL(1), REAL(2));
    return script_null;
}

ScriptValue
javaSetGridColor(ScriptEnv* context)
{
    if (!argument_checks(context, "debug", "rrr")) {
        return script_false;
    }

    if (REAL(0) < 0 || REAL(0) > 255) {
        debug_message("UNKNOWN_ERROR setGridColor(): r value must be in range 0-255");
        return script_false;
    }
    if (REAL(1) < 0 || REAL(1) > 255) {
        debug_message("UNKNOWN_ERROR setGridColor(): g value must be in range 0-255");
        return script_false;
    }
    if (REAL(2) < 0 || REAL(2) > 255) {
        debug_message("UNKNOWN_ERROR setGridColor(): b value must be in range 0-255");
        return script_false;
    }

    _main->nativeSetGridColor(REAL(0), REAL(1), REAL(2));
    return script_null;
}

ScriptValue
javaAllowRubber(ScriptEnv* context)
{
    if (!argument_checks(context, "allowRubber", "")) {
        return script_false;
    }

    return script_bool(_main->nativeAllowRubber());
}

ScriptValue
javaSetRubberMode(ScriptEnv* context)
{
    if (!argument_checks(context, "allowRubber", "s")) {
        return script_false;
    }

    QString mode = QSTR(0).toUpper();

    if (mode == "CIRCLE_1P_RAD") {
        _main->nativeSetRubberMode(OBJ_RUBBER_CIRCLE_1P_RAD);
    }
    else if (mode == "CIRCLE_1P_DIA") {
        _main->nativeSetRubberMode(OBJ_RUBBER_CIRCLE_1P_DIA);
    }
    else if (mode == "CIRCLE_2P") {
        _main->nativeSetRubberMode(OBJ_RUBBER_CIRCLE_2P);
    }
    else if (mode == "CIRCLE_3P") {
        _main->nativeSetRubberMode(OBJ_RUBBER_CIRCLE_3P);
    }
    else if (mode == "CIRCLE_TTR") {
        _main->nativeSetRubberMode(OBJ_RUBBER_CIRCLE_TTR);
    }
    else if (mode == "CIRCLE_TTT") {
        _main->nativeSetRubberMode(OBJ_RUBBER_CIRCLE_TTT);
    }
    else if (mode == "DIMLEADER_LINE") {
        _main->nativeSetRubberMode(OBJ_RUBBER_DIMLEADER_LINE);
    }
    else if (mode == "ELLIPSE_LINE")                      { _main->nativeSetRubberMode(OBJ_RUBBER_ELLIPSE_LINE); }
    else if (mode == "ELLIPSE_MAJORDIAMETER_MINORRADIUS") { _main->nativeSetRubberMode(OBJ_RUBBER_ELLIPSE_MAJORDIAMETER_MINORRADIUS); }
    else if (mode == "ELLIPSE_MAJORRADIUS_MINORRADIUS")   { _main->nativeSetRubberMode(OBJ_RUBBER_ELLIPSE_MAJORRADIUS_MINORRADIUS); }
    else if (mode == "ELLIPSE_ROTATION")                  { _main->nativeSetRubberMode(OBJ_RUBBER_ELLIPSE_ROTATION); }

    else if (mode == "LINE")                              { _main->nativeSetRubberMode(OBJ_RUBBER_LINE); }

    else if (mode == "POLYGON")                           { _main->nativeSetRubberMode(OBJ_RUBBER_POLYGON); }
    else if (mode == "POLYGON_INSCRIBE")                  { _main->nativeSetRubberMode(OBJ_RUBBER_POLYGON_INSCRIBE); }
    else if (mode == "POLYGON_CIRCUMSCRIBE")              { _main->nativeSetRubberMode(OBJ_RUBBER_POLYGON_CIRCUMSCRIBE); }

    else if (mode == "POLYLINE")                          { _main->nativeSetRubberMode(OBJ_RUBBER_POLYLINE); }

    else if (mode == "RECTANGLE")                         { _main->nativeSetRubberMode(OBJ_RUBBER_RECTANGLE); }

    else if (mode == "TEXTSINGLE")                        { _main->nativeSetRubberMode(OBJ_RUBBER_TEXTSINGLE); }

    else                                                 { return throwError("UNKNOWN_ERROR setRubberMode(): unknown rubberMode value"); }

    return script_null;
}

ScriptValue
javaSetRubberPoint(ScriptEnv* context)
{
    if (!argument_checks(context, "SetRubberPoint", "srr")) {
        return script_false;
    }

    QString key = QSTR(0).toUpper();
    qreal x     = context->argument[1].r;
    qreal y     = context->argument[2].r;

    _main->nativeSetRubberPoint(key, x, y);
    return script_null;
}

ScriptValue
javaSetRubberText(ScriptEnv* context)
{
    if (!argument_checks(context, "SetRubberPoint", "ss")) {
        return script_false;
    }

    QString key = QSTR(0).toUpper();
    QString txt = QSTR(1);

    _main->nativeSetRubberText(key, txt);
    return script_null;
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
        ScriptValue result = _main->command_map[line_list[0]].prompt(context);
        free_script_env(context);
        return result;
    }
    return script_false;
}

ScriptValue
javaAddRubber(ScriptEnv* context)
{
    if (!argument_checks(context, "SetRubberPoint", "s")) {
        return script_false;
    }

    QString objType = QSTR(0).toUpper();

    if (!_main->nativeAllowRubber()) {
        debug_message("UNKNOWN_ERROR addRubber(): You must use vulcanize() before you can add another rubber object.");
        return script_false;
    }

    /* FIXME: ERROR CHECKING */
    qreal mx = command_prompt("mousex").r;
    qreal my = command_prompt("mousey").r;

    if (objType == "ARC") {
        // TODO: handle this type
    }
    else if (objType == "BLOCK") {
        // TODO: handle this type
    }
    else if (objType == "CIRCLE") {
        _main->nativeAddCircle(mx, my, 0, false, OBJ_RUBBER_ON);
    }
    else if (objType == "DIMALIGNED") {
        // TODO: handle this type
    }
    else if (objType == "DIMANGULAR")   {} //TODO: handle this type
    else if (objType == "DIMARCLENGTH") {} //TODO: handle this type
    else if (objType == "DIMDIAMETER")  {} //TODO: handle this type
    else if (objType == "DIMLEADER")    { _main->nativeAddDimLeader(mx, my, mx, my, 0, OBJ_RUBBER_ON); }
    else if (objType == "DIMLINEAR")    {} //TODO: handle this type
    else if (objType == "DIMORDINATE")  {} //TODO: handle this type
    else if (objType == "DIMRADIUS")    {} //TODO: handle this type
    else if (objType == "ELLIPSE")      { _main->nativeAddEllipse(mx, my, 0, 0, 0, 0, OBJ_RUBBER_ON); }
    else if (objType == "ELLIPSEARC")   {} //TODO: handle this type
    else if (objType == "HATCH")        {} //TODO: handle this type
    else if (objType == "IMAGE")        {} //TODO: handle this type
    else if (objType == "INFINITELINE") {} //TODO: handle this type
    else if (objType == "LINE")         { _main->nativeAddLine(mx, my, mx, my, 0, OBJ_RUBBER_ON); }
    else if (objType == "PATH")         {} //TODO: handle this type
    else if (objType == "POINT")        {} //TODO: handle this type
    else if (objType == "POLYGON")      { _main->nativeAddPolygon(mx, my, QPainterPath(), OBJ_RUBBER_ON); }
    else if (objType == "POLYLINE")     { _main->nativeAddPolyline(mx, my, QPainterPath(), OBJ_RUBBER_ON); }
    else if (objType == "RAY")          {} //TODO: handle this type
    else if (objType == "RECTANGLE")    { _main->nativeAddRectangle(mx, my, mx, my, 0, 0, OBJ_RUBBER_ON); }
    else if (objType == "SPLINE")       {} //TODO: handle this type
    else if (objType == "TEXTMULTI")    {} //TODO: handle this type
    else if (objType == "TEXTSINGLE")   { _main->nativeAddTextSingle("", mx, my, 0, false, OBJ_RUBBER_ON); }

    return script_null;
}

ScriptValue
javaClearRubber(ScriptEnv* context)
{
    if (context->argumentCount != 0) return throwError("clearRubber() requires zero arguments");

    _main->nativeClearRubber();
    return script_null;
}

ScriptValue
javaSpareRubber(ScriptEnv* context)
{
    if (context->argumentCount != 1) {
        return throwError("spareRubber() requires one argument");
    }
    if (context->argument[0].type != SCRIPT_STRING) {
        return throwError("TYPE_ERROR, spareRubber(): first argument is not a string");
    }

    QString objID = QSTR(0).toUpper();

    if (objID == "PATH") {
        _main->nativeSpareRubber(SPARE_RUBBER_PATH);
    }
    else if (objID == "POLYGON")  {
        _main->nativeSpareRubber(SPARE_RUBBER_POLYGON);
    }
    else if (objID == "POLYLINE") {
        _main->nativeSpareRubber(SPARE_RUBBER_POLYLINE);
    }
    else {
        bool ok = false;
        qint64 id = objID.toLongLong(&ok);
        if (!ok) return throwError("TYPE_ERROR, spareRubber(): error converting object ID into an int64");
        _main->nativeSpareRubber(id);
    }

    return script_null;
}

ScriptValue
javaAddTextMulti(ScriptEnv* context)
{
    if (!argument_checks(context, "mouseX", "srrrb")) {
        return script_false;
    }
    _main->nativeAddTextMulti(QSTR(0), REAL(1), REAL(2), REAL(3), BOOL(4), OBJ_RUBBER_OFF);
    return script_null;
}

ScriptValue
javaAddTextSingle(ScriptEnv* context)
{
    if (!argument_checks(context, "mouseX", "srrrb")) {
        return script_false;
    }
    _main->nativeAddTextSingle(QSTR(0), REAL(1), REAL(2), REAL(3), BOOL(4), OBJ_RUBBER_OFF);
    return script_null;
}

ScriptValue
javaAddInfiniteLine(ScriptEnv* context)
{
    //TODO: parameter error checking
    debug_message("TODO: finish addInfiniteLine command");
    return script_null;
}

ScriptValue
javaAddRay(ScriptEnv* context)
{
    //TODO: parameter error checking
    debug_message("TODO: finish addRay command");
    return script_null;
}

ScriptValue
javaAddLine(ScriptEnv* context)
{
    if (!argument_checks(context, "mouseX", "rrrrr")) {
        return script_false;
    }
    _main->nativeAddLine(REAL(0), REAL(1), REAL(2), REAL(3), REAL(4), OBJ_RUBBER_OFF);
    return script_null;
}

ScriptValue
javaAddTriangle(ScriptEnv* context)
{
    if (!argument_checks(context, "mouseX", "rrrrrrrb")) {
        return script_false;
    }
    _main->nativeAddTriangle(REAL(0), REAL(1), REAL(2), REAL(3), REAL(4), REAL(5), REAL(6), BOOL(7));
    return script_null;
}

ScriptValue
javaAddRectangle(ScriptEnv* context)
{
    if (!argument_checks(context, "mouseX", "rrrrrb")) {
        return script_false;
    }
    _main->nativeAddRectangle(REAL(0), REAL(1), REAL(2), REAL(3), REAL(4), BOOL(5), OBJ_RUBBER_OFF);
    return script_null;
}

ScriptValue
javaAddRoundedRectangle(ScriptEnv* context)
{
    if (!argument_checks(context, "mouseX", "rrrrrrb")) {
        return script_false;
    }
    _main->nativeAddRoundedRectangle(REAL(0), REAL(1), REAL(2), REAL(3), REAL(4), REAL(5), BOOL(6));
    return script_null;
}

ScriptValue
javaAddArc(ScriptEnv* context)
{
    if (!argument_checks(context, "mouseX", "rrrrrr")) {
        return script_false;
    }
    _main->nativeAddArc(REAL(0), REAL(1), REAL(2), REAL(3), REAL(4), REAL(5), OBJ_RUBBER_OFF);
    return script_null;
}

ScriptValue
javaAddCircle(ScriptEnv* context)
{
    if (!argument_checks(context, "mouseX", "rrrb")) {
        return script_false;
    }
    _main->nativeAddCircle(REAL(0), REAL(1), REAL(2), BOOL(4), OBJ_RUBBER_OFF);
    return script_null;
}

ScriptValue
javaAddSlot(ScriptEnv* context)
{
    if (!argument_checks(context, "mouseX", "rrrrrb")) {
        return script_false;
    }
    _main->nativeAddSlot(REAL(0), REAL(1), REAL(2), REAL(3), REAL(4), BOOL(5), OBJ_RUBBER_OFF);
    return script_null;
}

ScriptValue
javaAddEllipse(ScriptEnv* context)
{
    if (!argument_checks(context, "mouseX", "rrrrrb")) {
        return script_false;
    }
    _main->nativeAddEllipse(REAL(0), REAL(1), REAL(2), REAL(3), REAL(4), BOOL(5), OBJ_RUBBER_OFF);
    return script_null;
}

ScriptValue
javaAddPoint(ScriptEnv* context)
{
    if (!argument_checks(context, "mouseX", "rr")) {
        return script_false;
    }
    _main->nativeAddPoint(REAL(0), REAL(1));
    return script_null;
}

ScriptValue
javaAddRegularPolygon(ScriptEnv* context)
{
    //TODO: parameter error checking
    debug_message("TODO: finish addRegularPolygon command");
    return script_null;
}

ScriptValue
javaAddPolygon(ScriptEnv* context)
{
    #if 0
    if (context->argumentCount != 1)   return throwError("addPolygon() requires one argument");
    if (!context->argument[0].isArray()) return throwError("TYPE_ERROR, addPolygon(): first argument is not an array");

    QVariantList varList = context->argument[0].toVariant().toList();
    int varSize = varList.size();
    if (varSize < 2) return throwError("TYPE_ERROR, addPolygon(): array must contain at least two elements");
    if (varSize % 2) return throwError("TYPE_ERROR, addPolygon(): array cannot contain an odd number of elements");

    bool lineTo = false;
    bool xCoord = true;
    qreal x = 0;
    qreal y = 0;
    qreal startX = 0;
    qreal startY = 0;
    QPainterPath path;
    foreach(QVariant var, varList)
    {
        if (var.canConvert(QVariant::Double))
        {
            if (xCoord)
            {
                xCoord = false;
                x = var.toReal();
            }
            else
            {
                xCoord = true;
                y = -var.toReal();

                if (lineTo) { path.lineTo(x,y); }
                else       { path.moveTo(x,y); lineTo = true; startX = x; startY = y; }
            }
        }
        else
            return throwError("TYPE_ERROR, addPolygon(): array contains one or more invalid elements");
    }

    //Close the polygon
    path.closeSubpath();

    path.translate(-startX, -startY);

    _main->nativeAddPolygon(startX, startY, path, OBJ_RUBBER_OFF);
    #endif
    return script_null;
}

ScriptValue
javaAddPolyline(ScriptEnv* context)
{
    #if 0
    if (context->argumentCount != 1)   return throwError("addPolyline() requires one argument");
    if (!context->argument[0].isArray()) return throwError("TYPE_ERROR, addPolyline(): first argument is not an array");

    QVariantList varList = context->argument[0].toVariant().toList();
    int varSize = varList.size();
    if (varSize < 2) return throwError("TYPE_ERROR, addPolyline(): array must contain at least two elements");
    if (varSize % 2) return throwError("TYPE_ERROR, addPolyline(): array cannot contain an odd number of elements");

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
                else       { path.moveTo(x,y); lineTo = true; startX = x; startY = y; }
            }
        }
        else
            return throwError("TYPE_ERROR, addPolyline(): array contains one or more invalid elements");
    }

    path.translate(-startX, -startY);

    _main->nativeAddPolyline(startX, startY, path, OBJ_RUBBER_OFF);
    #endif
    return script_null;
}

ScriptValue
javaAddPath(ScriptEnv* context)
{
    //TODO: parameter error checking
    debug_message("TODO: finish addPath command");
    return script_null;
}

ScriptValue
javaAddHorizontalDimension(ScriptEnv* context)
{
    //TODO: parameter error checking
    debug_message("TODO: finish addHorizontalDimension command");
    return script_null;
}

ScriptValue
javaAddVerticalDimension(ScriptEnv* context)
{
    //TODO: parameter error checking
    debug_message("TODO: finish addVerticalDimension command");
    return script_null;
}

ScriptValue
javaAddImage(ScriptEnv* context)
{
    //TODO: parameter error checking
    debug_message("TODO: finish addImage command");
    return script_null;
}

ScriptValue
javaAddDimLeader(ScriptEnv* context)
{
    if (!argument_checks(context, "calculateAngle", "rrrrr")) {
        return script_false;
    }

    _main->nativeAddDimLeader(REAL(0), REAL(1), REAL(2), REAL(3), REAL(4), OBJ_RUBBER_OFF);
    return script_null;
}

ScriptValue
javaSetCursorShape(ScriptEnv* context)
{
    if (!argument_checks(context, "calculateAngle", "s")) {
        return script_false;
    }

    _main->nativeSetCursorShape(QSTR(0));
    return script_null;
}

ScriptValue
javaCalculateAngle(ScriptEnv* context)
{
    if (!argument_checks(context, "calculateAngle", "rrrr")) {
        return script_false;
    }

    qreal r = _main->nativeCalculateAngle(REAL(0), REAL(1), REAL(2), REAL(3));
    return script_real(r);
}

ScriptValue
javaCalculateDistance(ScriptEnv* context)
{
    if (!argument_checks(context, "numSelected", "rrrr")) {
        return script_false;
    }

    qreal r = _main->nativeCalculateDistance(REAL(0), REAL(1), REAL(2), REAL(3));
    return script_real(r);
}

ScriptValue
javaPerpendicularDistance(ScriptEnv* context)
{
    if (!argument_checks(context, "numSelected", "rrrrrr")) {
        return script_false;
    }

    qreal r = _main->nativePerpendicularDistance(REAL(0), REAL(1), REAL(2), REAL(3), REAL(4), REAL(5));
    return script_real(r);
}

ScriptValue
javaNumSelected(ScriptEnv* context)
{
    if (!argument_checks(context, "numSelected", "")) {
        return script_false;
    }

    return script_int(_main->nativeNumSelected());
}

ScriptValue
javaSelectAll(ScriptEnv* context)
{
    if (!argument_checks(context, "selectAll", "")) {
        return script_false;
    }

    _main->nativeSelectAll();
    return script_null;
}

/* TODO: finish
 */
ScriptValue
javaAddToSelection(ScriptEnv* context)
{
    return script_null;
}

ScriptValue
javaDeleteSelected(ScriptEnv* context)
{
    if (!argument_checks(context, "deleteSelected", "")) {
        return script_false;
    }

    _main->nativeDeleteSelected();
    return script_null;
}

ScriptValue
javaCutSelected(ScriptEnv* context)
{
    if (!argument_checks(context, "scaleSelected", "rr")) {
        return script_false;
    }

    _main->nativeCutSelected(REAL(0), REAL(1));
    return script_null;
}

ScriptValue
javaCopySelected(ScriptEnv* context)
{
    if (!argument_checks(context, "scaleSelected", "rr")) {
        return script_false;
    }

    _main->nativeCopySelected(REAL(0), REAL(1));
    return script_null;
}

ScriptValue
javaPasteSelected(ScriptEnv* context)
{
    if (!argument_checks(context, "scaleSelected", "rr")) {
        return script_false;
    }

    _main->nativePasteSelected(REAL(0), REAL(1));
    return script_null;
}

ScriptValue
javaMoveSelected(ScriptEnv* context)
{
    if (!argument_checks(context, "scaleSelected", "rr")) {
        return script_false;
    }

    _main->nativeMoveSelected(REAL(0), REAL(1));
    return script_null;
}

ScriptValue
javaScaleSelected(ScriptEnv* context)
{
    if (!argument_checks(context, "scaleSelected", "rrr")) {
        return script_false;
    }

    if (REAL(2) <= 0.0) {
        // throwError("UNKNOWN_ERROR scaleSelected(): scale factor must be greater than zero");
        return script_false;
    }

    _main->nativeScaleSelected(REAL(0), REAL(1), REAL(2));
    return script_null;
}

ScriptValue
javaRotateSelected(ScriptEnv* context)
{
    if (!argument_checks(context, "rotateSelected", "rrr")) {
        return script_false;
    }

    _main->nativeRotateSelected(REAL(0), REAL(1), REAL(2));
    return script_null;
}

ScriptValue
javaMirrorSelected(ScriptEnv* context)
{
    if (!argument_checks(context, "mirrorSelected", "rrrr")) {
        return script_false;
    }

    _main->nativeMirrorSelected(REAL(0), REAL(1), REAL(2), REAL(3));
    return script_null;
}

ScriptValue
javaQSnapX(ScriptEnv* context)
{
    if (!argument_checks(context, "QSnapX", "")) {
        return script_false;
    }
    qreal r = _main->nativeQSnapX();
    return script_real(r);
}

ScriptValue
javaQSnapY(ScriptEnv* context)
{
    if (!argument_checks(context, "QSnapY", "")) {
        return script_false;
    }
    qreal r = _main->nativeQSnapY();
    return script_real(r);
}


