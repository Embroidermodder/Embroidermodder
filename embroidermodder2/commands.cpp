/*
 * Embroidermodder 2.
 *
 * Copyright 2013-2024 The Embroidermodder Team
 * Embroidermodder 2 is Open Source Software.
 * See LICENSE for licensing terms.
 *
 * Command: about
 */

#include <QString>
#include <QDebug>

#include <string.h>

#include "commands.h"

#define REAL(i) context->argument[i].r
#define INT(i) context->argument[i].i
#define QSTR(i) context->argument[i].s
#define BOOL(i) context->argument[i].b

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

Command end_symbol_cmd = {
    .menu_position = -1
};

/*
std::unordered_map<std::string, Command> command_map = {
    {"ABOUT", about_cmd}
};
*/

Command command_list[] = {
    about_cmd,
    circle_cmd,
    day_cmd,
    distance_cmd,
    dolphin_cmd,
    ellipse_cmd,
    erase_cmd,
    exit_cmd,
    heart_cmd,
    help_cmd,
    icon128_cmd,
    icon16_cmd,
    icon24_cmd,
    icon32_cmd,
    icon48_cmd,
    icon64_cmd,
    line_cmd,
    locatepoint_cmd,
    move_cmd,
    new_cmd,
    night_cmd,
    open_cmd,
    pandown_cmd,
    panleft_cmd,
    panright_cmd,
    panup_cmd,
    path_cmd,
    platform_cmd,
    point_cmd,
    polygon_cmd,
    polyline_cmd,
    quickleader_cmd,
    rectangle_cmd,
    redo_cmd,
    rgb_cmd,
    rotate_cmd,
    sandbox_cmd,
    scale_cmd,
    selectall_cmd,
    singlelinetext_cmd,
    snowflake_cmd,
    star_cmd,
    syswindows_cmd,
    tipoftheday_cmd,
    undo_cmd,
    windowcascade_cmd,
    windowclose_cmd,
    windowcloseall_cmd,
    windownext_cmd,
    windowprevious_cmd,
    windowtile_cmd,
    zoomextents_cmd,
    zoomin_cmd,
    zoomout_cmd,
    end_symbol_cmd
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

QString
translate(QString msg)
{
    return msg;
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
script_string(QString s)
{
    ScriptValue value;
    value.type = SCRIPT_STRING;
    value.s = s;
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

//NOTE: http://www.qtcentre.org/threads/20432-Can-I-include-a-script-from-script
ScriptValue
javaInclude(ScriptEnv* context)
{
    /*
    QString fileName = context->argument[0].s;
    QFile scriptFile("commands/" + fileName);

    if (!scriptFile.open(QIODevice::ReadOnly)){
        return -1;
    }

    QTextStream stream(&scriptFile);
    QString s=stream.readAll();
    scriptFile.close();

    ScriptEnv* parent = context->parentContext();

    if (parent!=0) {
        context->setActivationObject(context->parentContext()->activationObject());
        context->setThisObject(context->parentContext()->thisObject());
    }

    ScriptValue result = engine->evaluate(s);
    */

    return script_int(0);
}

/* For each Command in command_list, for each alias set up a map from
 * alias to the Command. Then for a given context the call doesn't have to loop?
 *
 * NOTE: Every QScriptProgram must have a unique function name to call. If every function was called main(), then
 *       the ScriptArgs would only call the last script evaluated (which happens to be main() in another script).
 *       Thus, by adding the cmdName before main(), it becomes line_main(), circle_main(), etc...
 *       Do not change this code unless you really know what you are doing. I mean it.
 *
 * Position currently comes from the order of the command_list.
 */
void
MainWindow::javaLoadCommands(void)
{
    qDebug("javaLoadCommand()");
    QString appDir = qApp->applicationDirPath();

    for (int i=0; command_list[i].menu_position >= 0; i++) {
        QString icon(command_list[i].icon);
        QString menuName(command_list[i].menu_name);
        int menuPos = command_list[i].menu_position;
        QString toolbarName(command_list[i].toolbar_name);
        int toolbarPos = command_list[i].toolbar_position;
        QString toolTip(command_list[i].tooltip);
        QString statusTip(command_list[i].statustip);
        QString alias_string(command_list[i].alias);
        /* QStringList aliases = alias_string.toStringList(); */

        QAction* ACTION = createAction(icon, toolTip, statusTip, true);

        qDebug("COMMAND: %s", icon);

        // load globals: engine->evaluate(script);

        if (toolbarName.toUpper() != "NONE") {
            //If the toolbar doesn't exist, create it.
            if (!toolbarHash.value(toolbarName)) {
                QToolBar* tb = new QToolBar(toolbarName, this);
                tb->setObjectName("toolbar" + toolbarName);
                connect(tb, SIGNAL(topLevelChanged(bool)), this,
                    SLOT(floatingChangedToolBar(bool)));
                addToolBar(Qt::LeftToolBarArea, tb);
                addToolBarBreak(Qt::LeftToolBarArea);
                toolbarHash.insert(toolbarName, tb);
            }

            //TODO: order actions position in toolbar based on .ini setting
            toolbarHash.value(toolbarName)->addAction(ACTION);
        }

        if (menuName.toUpper() != "NONE") {
            //If the menu doesn't exist, create it.
            if (!menuHash.value(menuName)) {
                QMenu* menu = new QMenu(menuName, this);
                menu->setTearOffEnabled(true);
                menuBar()->addMenu(menu);
                menuHash.insert(menuName, menu);
            }

            //TODO: order actions position in menu based on .ini setting
            menuHash.value(menuName)->addAction(ACTION);
        }

/*
        foreach (QString alias, aliases) {
            prompt->addCommand(alias, cmdName);
        }
*/
    }
}

ScriptValue
javaDebug(ScriptEnv* context)
{
    if (!argument_checks(context, "debug", "s")) {
        return script_false;
    }
    debug_message(QSTR(0));
    return script_null;
}

ScriptValue javaError(ScriptEnv* context)
{
    if (!argument_checks(context, "debug", "ss")) {
        return script_false;
    }
    QString s = "ERROR: (" + context->argument[0].s + ") " + context->argument[1].s;
    _main->nativeSetPromptPrefix(s);
    _main->nativeAppendPromptHistory(QString());
    _main->nativeEndCommand();
    return script_null;
}

ScriptValue javaTodo(ScriptEnv* context)
{
    if (!argument_checks(context, "debug", "ss")) {
        return script_false;
    }
    QString s = "TODO: (" + QSTR(0) + ") " + QSTR(1);
    _main->nativeAlert(s);
    _main->nativeEndCommand();
    return script_null;
}

ScriptValue javaAlert(ScriptEnv* context)
{
    if (!argument_checks(context, "debug", "s")) {
        return script_false;
    }
    _main->nativeAlert(QSTR(0));
    return script_null;
}

ScriptValue javaBlinkPrompt(ScriptEnv* context)
{
    if (!argument_checks(context, "debug", "")) {
        return script_false;
    }

    _main->nativeBlinkPrompt();
    return script_null;
}

ScriptValue javaSetPromptPrefix(ScriptEnv* context)
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

ScriptValue javaEnablePromptRapidFire(ScriptEnv* context)
{
    if (!argument_checks(context, "debug", "")) {
        return script_false;
    }

    _main->nativeEnablePromptRapidFire();
    return script_null;
}

ScriptValue javaDisablePromptRapidFire(ScriptEnv* context)
{
    if (!argument_checks(context, "debug", "")) {
        return script_false;
    }

    _main->nativeDisablePromptRapidFire();
    return script_null;
}

ScriptValue javaEnableMoveRapidFire(ScriptEnv* context)
{
    if (!argument_checks(context, "debug", "")) {
        return script_false;
    }

    _main->nativeEnableMoveRapidFire();
    return script_null;
}

ScriptValue javaDisableMoveRapidFire(ScriptEnv* context)
{
    if (!argument_checks(context, "debug", "")) {
        return script_false;
    }

    _main->nativeDisableMoveRapidFire();
    return script_null;
}

ScriptValue javaInitCommand(ScriptEnv* context)
{
    if (!argument_checks(context, "debug", "")) {
        return script_false;
    }

    _main->nativeInitCommand();
    return script_null;
}

ScriptValue javaEndCommand(ScriptEnv* context)
{
    if (!argument_checks(context, "debug", "")) {
        return script_false;
    }

    _main->nativeEndCommand();
    return script_null;
}

ScriptValue javaNewFile(ScriptEnv* context)
{
    if (!argument_checks(context, "debug", "")) {
        return script_false;
    }

    _main->nativeNewFile();
    return script_null;
}

ScriptValue javaOpenFile(ScriptEnv* context)
{
    if (!argument_checks(context, "debug", "")) {
        return script_false;
    }

    _main->nativeOpenFile();
    return script_null;
}

ScriptValue javaExit(ScriptEnv* context)
{
    if (!argument_checks(context, "debug", "")) {
        return script_false;
    }

    _main->nativeExit();
    return script_null;
}

ScriptValue javaHelp(ScriptEnv* context)
{
    if (!argument_checks(context, "debug", "")) {
        return script_false;
    }

    _main->nativeHelp();
    return script_null;
}

ScriptValue javaAbout(ScriptEnv* context)
{
    if (!argument_checks(context, "debug", "")) {
        return script_false;
    }

    _main->nativeAbout();
    return script_null;
}

ScriptValue javaTipOfTheDay(ScriptEnv* context)
{
    if (!argument_checks(context, "debug", "")) {
        return script_false;
    }

    _main->nativeTipOfTheDay();
    return script_null;
}

ScriptValue javaWindowCascade(ScriptEnv* context)
{
    if (!argument_checks(context, "debug", "")) {
        return script_false;
    }

    _main->nativeWindowCascade();
    return script_null;
}

ScriptValue javaWindowTile(ScriptEnv* context)
{
    if (!argument_checks(context, "debug", "")) {
        return script_false;
    }

    _main->nativeWindowTile();
    return script_null;
}

ScriptValue javaWindowClose(ScriptEnv* context)
{
    if (!argument_checks(context, "debug", "")) {
        return script_false;
    }

    _main->nativeWindowClose();
    return script_null;
}

ScriptValue javaWindowCloseAll(ScriptEnv* context)
{
    if (!argument_checks(context, "debug", "")) {
        return script_false;
    }

    _main->nativeWindowCloseAll();
    return script_null;
}

ScriptValue javaWindowNext(ScriptEnv* context)
{
    if (!argument_checks(context, "debug", "")) {
        return script_false;
    }

    _main->nativeWindowNext();
    return script_null;
}

ScriptValue javaWindowPrevious(ScriptEnv* context)
{
    if (!argument_checks(context, "debug", "")) {
        return script_false;
    }

    _main->nativeWindowPrevious();
    return script_null;
}

ScriptValue javaPlatformString(ScriptEnv* context)
{
    if (!argument_checks(context, "debug", "")) {
        return script_false;
    }
    return script_string(_main->nativePlatformString());
}

ScriptValue javaMessageBox(ScriptEnv* context)
{
    if (!argument_checks(context, "debug", "sss")) {
        return script_false;
    }

    QString type  = context->argument[0].s.toLower();
    QString title = context->argument[1].s;
    QString text  = context->argument[2].s;

    if (type != "critical" && type != "information" && type != "question" && type != "warning") {
        debug_message("UNKNOWN_ERROR messageBox(): first argument must be \"critical\", \"information\", \"question\" or \"warning\".");
        return script_false;
    }

    _main->nativeMessageBox(type, title, text);
    return script_null;
}

ScriptValue javaIsInt(ScriptEnv* context)
{
    if (!argument_checks(context, "debug", "i")) {
        return script_false;
    }
    return script_true;
}

ScriptValue javaUndo(ScriptEnv* context)
{
    if (!argument_checks(context, "debug", "")) {
        return script_false;
    }

    _main->nativeUndo();
    return script_null;
}

ScriptValue javaRedo(ScriptEnv* context)
{
    if (!argument_checks(context, "debug", "")) {
        return script_false;
    }

    _main->nativeRedo();
    return script_null;
}

ScriptValue javaIcon16(ScriptEnv* context)
{
    if (!argument_checks(context, "debug", "")) {
        return script_false;
    }

    _main->nativeIcon16();
    return script_null;
}

ScriptValue javaIcon24(ScriptEnv* context)
{
    if (!argument_checks(context, "debug", "")) {
        return script_false;
    }

    _main->nativeIcon24();
    return script_null;
}

ScriptValue javaIcon32(ScriptEnv* context)
{
    if (!argument_checks(context, "debug", "")) {
        return script_false;
    }

    _main->nativeIcon32();
    return script_null;
}

ScriptValue javaIcon48(ScriptEnv* context)
{
    if (!argument_checks(context, "debug", "")) {
        return script_false;
    }

    _main->nativeIcon48();
    return script_null;
}

ScriptValue javaIcon64(ScriptEnv* context)
{
    if (!argument_checks(context, "debug", "")) {
        return script_false;
    }

    _main->nativeIcon64();
    return script_null;
}

ScriptValue javaIcon128(ScriptEnv* context)
{
    if (!argument_checks(context, "debug", "")) {
        return script_false;
    }

    _main->nativeIcon128();
    return script_null;
}

ScriptValue javaPanLeft(ScriptEnv* context)
{
    if (!argument_checks(context, "debug", "")) {
        return script_false;
    }

    _main->nativePanLeft();
    return script_null;
}

ScriptValue javaPanRight(ScriptEnv* context)
{
    if (!argument_checks(context, "debug", "")) {
        return script_false;
    }

    _main->nativePanRight();
    return script_null;
}

ScriptValue javaPanUp(ScriptEnv* context)
{
    if (!argument_checks(context, "debug", "")) {
        return script_false;
    }

    _main->nativePanUp();
    return script_null;
}

ScriptValue javaPanDown(ScriptEnv* context)
{
    if (!argument_checks(context, "debug", "")) {
        return script_false;
    }

    _main->nativePanDown();
    return script_null;
}

ScriptValue javaZoomIn(ScriptEnv* context)
{
    if (!argument_checks(context, "debug", "")) {
        return script_false;
    }

    _main->nativeZoomIn();
    return script_null;
}

ScriptValue javaZoomOut(ScriptEnv* context)
{
    if (!argument_checks(context, "debug", "")) {
        return script_false;
    }

    _main->nativeZoomOut();
    return script_null;
}

ScriptValue javaZoomExtents(ScriptEnv* context)
{
    if (!argument_checks(context, "debug", "")) {
        return script_false;
    }

    _main->nativeZoomExtents();
    return script_null;
}

ScriptValue javaPrintArea(ScriptEnv* context)
{
    if (!argument_checks(context, "debug", "rrrr")) {
        return script_false;
    }

    _main->nativePrintArea(REAL(0), REAL(1), REAL(2), REAL(3));
    return script_null;
}

ScriptValue javaDayVision(ScriptEnv* context)
{
    if (!argument_checks(context, "debug", "")) {
        return script_false;
    }

    _main->nativeDayVision();
    return script_null;
}

ScriptValue javaNightVision(ScriptEnv* context)
{
    if (!argument_checks(context, "debug", "")) {
        return script_false;
    }

    _main->nativeNightVision();
    return script_null;
}

ScriptValue javaSetBackgroundColor(ScriptEnv* context)
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

ScriptValue javaSetCrossHairColor(ScriptEnv* context)
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

ScriptValue javaSetGridColor(ScriptEnv* context)
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

ScriptValue javaTextFont(ScriptEnv* context)
{
    if (!argument_checks(context, "debug", "")) {
        return script_false;
    }
    return script_string(_main->nativeTextFont());
}

ScriptValue javaTextSize(ScriptEnv* context)
{
    if (!argument_checks(context, "debug", "")) {
        return script_false;
    }
    return script_real(_main->nativeTextSize());
}

ScriptValue javaTextAngle(ScriptEnv* context)
{
    if (!argument_checks(context, "debug", "")) {
        return script_false;
    }
    return script_real(_main->nativeTextAngle());
}

ScriptValue
javaTextBold(ScriptEnv* context)
{
    if (!argument_checks(context, "textBold", "")) {
        return script_false;
    }
    return script_bool(_main->nativeTextBold());
}

ScriptValue
javaTextItalic(ScriptEnv* context)
{
    if (!argument_checks(context, "textItalic", "")) {
        return script_false;
    }
    return script_bool(_main->nativeTextItalic());
}

ScriptValue
javaTextUnderline(ScriptEnv* context)
{
    if (!argument_checks(context, "textUnderline", "")) {
        return script_false;
    }
    return script_bool(_main->nativeTextUnderline());
}

ScriptValue javaTextStrikeOut(ScriptEnv* context)
{
    if (!argument_checks(context, "textStrikeOut", "")) {
        return script_false;
    }
    return script_bool(_main->nativeTextStrikeOut());
}

ScriptValue javaTextOverline(ScriptEnv* context)
{
    if (!argument_checks(context, "textOverline", "")) {
        return script_false;
    }
    return script_bool(_main->nativeTextOverline());
}

ScriptValue javaSetTextFont(ScriptEnv* context)
{
    if (!argument_checks(context, "debug", "s")) {
        return script_false;
    }

    _main->nativeSetTextFont(context->argument[0].s);
    return script_null;
}

ScriptValue javaSetTextSize(ScriptEnv* context)
{
    if (!argument_checks(context, "debug", "r")) {
        return script_false;
    }

    _main->nativeSetTextSize(context->argument[0].r);
    return script_null;
}

ScriptValue javaSetTextAngle(ScriptEnv* context)
{
    if (!argument_checks(context, "debug", "r")) {
        return script_false;
    }

    qreal num = context->argument[0].r;

    _main->nativeSetTextAngle(num);
    return script_null;
}

ScriptValue
javaSetTextBold(ScriptEnv* context)
{
    if (!argument_checks(context, "textBold", "b")) {
        return script_false;
    }

    _main->nativeSetTextBold(context->argument[0].b);
    return script_null;
}

ScriptValue
javaSetTextItalic(ScriptEnv* context)
{
    if (!argument_checks(context, "textItalic", "b")) {
        return script_false;
    }

    _main->nativeSetTextItalic(context->argument[0].b);
    return script_null;
}

ScriptValue
javaSetTextUnderline(ScriptEnv* context)
{
    if (!argument_checks(context, "textUnderline", "b")) {
        return script_false;
    }

    _main->nativeSetTextUnderline(context->argument[0].b);
    return script_null;
}

ScriptValue
javaSetTextStrikeOut(ScriptEnv* context)
{
    if (!argument_checks(context, "textStrikeOut", "b")) {
        return script_false;
    }

    _main->nativeSetTextStrikeOut(context->argument[0].b);
    return script_null;
}

ScriptValue
javaSetTextOverline(ScriptEnv* context)
{
    if (!argument_checks(context, "textOverline", "b")) {
        return script_false;
    }

    _main->nativeSetTextOverline(context->argument[0].b);
    return script_null;
}

ScriptValue javaPreviewOn(ScriptEnv* context)
{
    if (!argument_checks(context, "textBold", "ssrrr")) {
        return script_false;
    }

    QString cloneStr = context->argument[0].s.toUpper();
    QString modeStr  = context->argument[1].s.toUpper();
    qreal x          = context->argument[2].r;
    qreal y          = context->argument[3].r;
    qreal data       = context->argument[4].r;

    int clone = PREVIEW_CLONE_NULL;
    int mode = PREVIEW_MODE_NULL;
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

    _main->nativePreviewOn(clone, mode, x, y, data);
    return script_null;
}

ScriptValue
javaPreviewOff(ScriptEnv* context)
{
    if (!argument_checks(context, "PreviewOff", "")) {
        return script_false;
    }

    _main->nativePreviewOff();
    return script_null;
}

ScriptValue
javaVulcanize(ScriptEnv* context)
{
    if (!argument_checks(context, "Vulcanize", "")) {
        return script_false;
    }

    _main->nativeVulcanize();
    return script_null;
}

ScriptValue javaAllowRubber(ScriptEnv* context)
{
    if (!argument_checks(context, "allowRubber", "")) {
        return script_false;
    }

    return script_bool(_main->nativeAllowRubber());
}

ScriptValue javaSetRubberMode(ScriptEnv* context)
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
    else if (mode == "CIRCLE_3P")                         { _main->nativeSetRubberMode(OBJ_RUBBER_CIRCLE_3P); }
    else if (mode == "CIRCLE_TTR")                        { _main->nativeSetRubberMode(OBJ_RUBBER_CIRCLE_TTR); }
    else if (mode == "CIRCLE_TTR")                        { _main->nativeSetRubberMode(OBJ_RUBBER_CIRCLE_TTT); }

    else if (mode == "DIMLEADER_LINE")                    { _main->nativeSetRubberMode(OBJ_RUBBER_DIMLEADER_LINE); }

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

ScriptValue javaSetRubberPoint(ScriptEnv* context)
{
    if (!argument_checks(context, "SetRubberPoint", "srr")) {
        return script_false;
    }

    QString key = context->argument[0].s.toUpper();
    qreal x     = context->argument[1].r;
    qreal y     = context->argument[2].r;

    _main->nativeSetRubberPoint(key, x, y);
    return script_null;
}

ScriptValue javaSetRubberText(ScriptEnv* context)
{
    if (!argument_checks(context, "SetRubberPoint", "ss")) {
        return script_false;
    }

    QString key = context->argument[0].s.toUpper();
    QString txt = context->argument[1].s;

    _main->nativeSetRubberText(key, txt);
    return script_null;
}

ScriptValue
javaAddRubber(ScriptEnv* context)
{
    if (!argument_checks(context, "SetRubberPoint", "s")) {
        return script_false;
    }

    QString objType = context->argument[0].s.toUpper();

    if (!_main->nativeAllowRubber()) {
        debug_message("UNKNOWN_ERROR addRubber(): You must use vulcanize() before you can add another rubber object.");
        return script_false;
    }

    qreal mx = _main->nativeMouseX();
    qreal my = _main->nativeMouseY();

    if     (objType == "ARC")          {} //TODO: handle this type
    else if (objType == "BLOCK")        {} //TODO: handle this type
    else if (objType == "CIRCLE")       { _main->nativeAddCircle(mx, my, 0, false, OBJ_RUBBER_ON); }
    else if (objType == "DIMALIGNED")   {} //TODO: handle this type
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
    if (context->argumentCount != 1)
        return throwError("spareRubber() requires one argument");
    if (context->argument[0].type != SCRIPT_STRING)
        return throwError("TYPE_ERROR, spareRubber(): first argument is not a string");

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

ScriptValue javaAddRectangle(ScriptEnv* context)
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

ScriptValue javaAddEllipse(ScriptEnv* context)
{
    if (!argument_checks(context, "mouseX", "rrrrrb")) {
        return script_false;
    }
    _main->nativeAddEllipse(REAL(0), REAL(1), REAL(2), REAL(3), REAL(4), BOOL(5), OBJ_RUBBER_OFF);
    return script_null;
}

ScriptValue javaAddPoint(ScriptEnv* context)
{
    if (!argument_checks(context, "mouseX", "rr")) {
        return script_false;
    }
    _main->nativeAddPoint(REAL(0), REAL(1));
    return script_null;
}

ScriptValue javaAddRegularPolygon(ScriptEnv* context)
{
    //TODO: parameter error checking
    debug_message("TODO: finish addRegularPolygon command");
    return script_null;
}

ScriptValue javaAddPolygon(ScriptEnv* context)
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

ScriptValue javaAddPolyline(ScriptEnv* context)
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
javaClearSelection(ScriptEnv* context)
{
    if (!argument_checks(context, "clearSelected", "")) {
        return script_false;
    }

    _main->nativeClearSelection();
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

ScriptValue
javaMouseX(ScriptEnv* context)
{
    if (!argument_checks(context, "mouseX", "")) {
        return script_false;
    }
    qreal r = _main->nativeMouseX();
    return script_real(r);
}

ScriptValue
javaMouseY(ScriptEnv* context)
{
    if (!argument_checks(context, "mouseY", "")) {
        return script_false;
    }
    qreal r = _main->nativeMouseY();
    return script_real(r);
}
