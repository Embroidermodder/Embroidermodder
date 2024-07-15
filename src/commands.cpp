/*
 * Embroidermodder 2.
 *
 * Copyright 2011-2024 The Embroidermodder Team
 * Embroidermodder 2 is Open Source Software, see LICENSE.md for licensing terms.
 * Visit https://www.libembroidery.org/refman for advice on altering this file,
 * or read the markdown version in embroidermodder2/docs/refman.
 *
 * Commands
 */

#include "embroidermodder.h"

/* Compatibility layer for C files */
void
init_command(void)
{
    View* gview = activeView();
    if (gview) {
        gview->clearRubberRoom();
    }
}

void
clear_selection(void)
{
    View* gview = activeView();
    if (gview) {
        gview->clearSelection();
    }
}

void
prompt_output(const char *txt)
{
    _main->prompt->appendHistory(QString(txt));
}

void
end_command(void)
{
    View* gview = activeView();
    if (gview) {
        gview->clearRubberRoom();
        gview->previewOff();
        gview->disableMoveRapidFire();
    }
    _main->prompt->endCommand();
}

/* Simple Commands (other commands, like circle_command are housed in their
 * own file with their associated functions)
 * ------------------------------------------------------------------------
 */

/* TODO: QTabWidget for about dialog
 */
ScriptValue
about_command(ScriptEnv *context)
{
    if (!argument_checks(context, "about", "")) {
        return script_false;
    }

    init_command();

    QApplication::setOverrideCursor(Qt::ArrowCursor);
    qDebug("about()");

    QString appDir = qApp->applicationDirPath();
    QString appName = QApplication::applicationName();
    QString title = "About " + appName;

    QDialog dialog(_main);
    QLabel image_label;
    QPixmap img(appDir + "/images/logo-small.png");
    image_label.setPixmap(img);
    QLabel text(appName + "\n\n" +
        _main->tr("http://www.libembroidery.org") +
        "\n\n" +
        _main->tr("Available Platforms: GNU/Linux, Windows, Mac OSX, Raspberry Pi") +
        "\n\n" +
        _main->tr("Embroidery formats by Josh Varga.") +
        "\n" +
        _main->tr("User Interface by Jonathan Greig.") +
        "\n\n" +
        _main->tr("Free under the zlib/libpng license.")
        #if defined(BUILD_GIT_HASH)
        + "\n\n" +
        _main->tr("Build Hash: ") + qPrintable(BUILD_GIT_HASH)
        #endif
        );
    text.setWordWrap(true);

    QDialogButtonBox buttonbox(Qt::Horizontal, &dialog);
    QPushButton button(&dialog);
    button.setText("Oh, Yeah!");
    buttonbox.addButton(&button, QDialogButtonBox::AcceptRole);
    buttonbox.setCenterButtons(true);
    _main->connect(&buttonbox, SIGNAL(accepted()), &dialog, SLOT(accept()));

    QVBoxLayout layout;
    layout.setAlignment(Qt::AlignCenter);
    layout.addWidget(&image_label);
    layout.addWidget(&text);
    layout.addWidget(&buttonbox);

    dialog.setWindowTitle(title);
    dialog.setMinimumWidth(image_label.minimumWidth()+30);
    dialog.setMinimumHeight(image_label.minimumHeight()+50);
    dialog.setLayout(&layout);
    dialog.exec();

    QApplication::restoreOverrideCursor();
    end_command();
}

/* ALERT is a prompt-only command. */
ScriptValue
alert_command(ScriptEnv *context)
{
    if (!argument_checks(context, "alert", "s")) {
        return script_false;
    }

    init_command();
    _main->prompt->alert(QSTR(0));
    end_command();
    return script_null;
}

/* ANGLE. */
ScriptValue
angle_command(ScriptEnv *context)
{
    init_command();
    clear_selection();

    end_command();
    return script_null;
}

/* CHANGELOG is not context-dependant */
ScriptValue
changelog_command(ScriptEnv *context)
{
    if (!argument_checks(context, "whats_this_command", "")) {
        return script_false;
    }
    init_command();
    clear_selection();
    end_command();
    return script_null;
}

/* CLEAR is not context-dependant. */
ScriptValue
clear_command(ScriptEnv* context)
{
    if (!argument_checks(context, "clear_command", "")) {
        return script_false;
    }

    init_command();
    clear_selection();
    end_command();
    return script_null;
}

/* COPY is not context-dependant */
ScriptValue
copy_command(ScriptEnv *context)
{
    if (!argument_checks(context, "whats_this_command", "")) {
        return script_false;
    }
    init_command();
    clear_selection();
    end_command();
    return script_null;
}

/* CUT is not context-dependant */
ScriptValue
cut_command(ScriptEnv *context)
{
    if (!argument_checks(context, "whats_this_command", "")) {
        return script_false;
    }
    init_command();
    clear_selection();
    end_command();
    return script_null;
}

/* . */
ScriptValue
colorselector_command(ScriptEnv* context)
{
    if (!argument_checks(context, "color_selector_command", "")) {
        return script_false;
    }
    init_command();
    end_command();
    return script_null;
}

/* . */
ScriptValue
debug_command(ScriptEnv *context)
{
    if (!argument_checks(context, "about", "s")) {
        return script_false;
    }
    init_command();
    clear_selection();
    _main->nativeAppendPromptHistory(QSTR(0));
    end_command();
    return script_null;
}

/* DONOTHING is not context-dependant. */
ScriptValue
design_details_command(ScriptEnv *context)
{
    if (!argument_checks(context, "design_details_command", "")) {
        return script_false;
    }
    init_command();
    clear_selection();
    _main->designDetails();
    end_command();
    return script_true;
}

/* DISABLE is a prompt-only Command. */
ScriptValue
disable_command(ScriptEnv* context)
{
    if (!argument_checks(context, "disable_prompt", "s")) {
        return script_false;
    }

    QString value = QSTR(0);

    init_command();

    if (value == "RAPIDFIRE") {
        _main->disablePromptRapidFire();
    }

    end_command();
    return script_null;
}

/* DONOTHING is not context-dependant. */
ScriptValue
do_nothing(ScriptEnv *context)
{
    if (!argument_checks(context, "do_nothing", "")) {
        return script_false;
    }
    return script_true;
}

/* ENABLE is a prompt-only Command. */
ScriptValue
enable_command(ScriptEnv* context)
{
    if (context->argumentCount != 2) {
        return script_false;
    }

    QString value = QSTR(0);

    init_command();

    if (value == "RAPIDFIRE") {
        _main->enablePromptRapidFire();
    }

    end_command();
    return script_null;
}

/* Erase is not context-dependant. */
ScriptValue
erase_command(ScriptEnv * /* context */)
{
    init_command();
    if (_main->nativeNumSelected() <= 0) {
        /* TODO: Prompt to select objects if nothing is preselected. */
        _main->prompt->alert(translate("Preselect objects before invoking the delete command."));
        end_command();
        _main->messageBox("information", translate("Delete Preselect"), translate("Preselect objects before invoking the delete command."));
    }
    else {
        _main->nativeDeleteSelected();
        end_command();
    }
    end_command();
    return script_null;
}

/* Error is not context-dependant. */
ScriptValue
error_command(ScriptEnv *context)
{
    if (!argument_checks(context, "error_command", "ss")) {
        return script_false;
    }
    init_command();
    clear_selection();
    QString s = "ERROR: (" + QSTR(0) + ") " + QSTR(1);
    _main->nativeSetPromptPrefix(s);
    _main->nativeAppendPromptHistory(QString());
    end_command();
    return script_null;
}

/* . */
ScriptValue
exit_command(ScriptEnv * /* context */)
{
    init_command();
    clear_selection();
    _main->exit();
    end_command();
    return script_null;
}

/* GET is a prompt-only Command. */
ScriptValue
get_command(ScriptEnv* context)
{
    if (!argument_checks(context, "get_prompt", "s")) {
        return script_false;
    }

    QString value = QSTR(0);

    init_command();

    if (value == "MOUSEX") {
        QGraphicsScene* scene = activeScene();
        if (!scene) {
            end_command();
            return script_false;
        }
        ScriptValue r = script_real(scene->property("SCENE_MOUSE_POINT").toPointF().x());
        /* _main->qDebug("mouseY: %.50f", r.r); */
        end_command();
        return r;
    }
    else if (value == "MOUSEY") {
        QGraphicsScene* scene = activeScene();
        if (!scene) {
            end_command();
            return script_false;
        }
        ScriptValue r = script_real(-scene->property("SCENE_MOUSE_POINT").toPointF().y());
        //_main->qDebug("mouseY: %.50f", r.r);
        end_command();
        return r;
    }
    else if (value == "TEXTANGLE") {
        return script_real(text_angle.setting);
    }
    else if (value == "TEXTBOLD") {
        return script_bool(text_style_bold.setting);
    }
    else if (value == "TEXTITALIC") {
        return script_bool(text_style_italic.setting);
    }
    else if (value == "TEXTFONT") {
        return script_string(text_font.setting);
    }
    else if (value == "TEXTOVERLINE") {
        return script_real(text_style_overline.setting);
    }
    else if (value == "TEXTSIZE") {
        return script_real(text_size.setting);
    }
    else if (value == "TEXTSTRIKEOUT") {
        return script_real(text_style_strikeout.setting);
    }
    else if (value == "TEXTUNDERLINE") {
        return script_bool(text_style_underline.setting);
    }
    else if (value == "QSNAPX") {
        return script_bool(_main->nativeQSnapX());
    }
    else if (value == "QSNAPY") {
        return script_bool(_main->nativeQSnapY());
    }

    end_command();
    return script_null;
}

/* HELP is not context-dependent. */
ScriptValue
help_command(ScriptEnv * /* context */)
{
    init_command();
    clear_selection();
    _main->help();
    end_command();
    return script_null;
}

ScriptValue
icon128_command(ScriptEnv *context)
{
    if (!argument_checks(context, "icon128_command", "")) {
        return script_false;
    }
    init_command();
    clear_selection();
    _main->iconResize(128);
    end_command();
    return script_null;
}

ScriptValue
icon16_command(ScriptEnv * context)
{
    if (!argument_checks(context, "icon16_command", "")) {
        return script_false;
    }
    init_command();
    clear_selection();
    _main->iconResize(16);
    end_command();
    return script_null;
}

ScriptValue
icon24_command(ScriptEnv * context)
{
    if (!argument_checks(context, "icon24_command", "")) {
        return script_false;
    }
    init_command();
    clear_selection();
    _main->iconResize(24);
    end_command();
    return script_null;
}

ScriptValue
icon32_command(ScriptEnv * context)
{
    if (!argument_checks(context, "icon32_command", "")) {
        return script_false;
    }
    init_command();
    clear_selection();
    _main->iconResize(32);
    end_command();
    return script_null;
}

ScriptValue
icon48_command(ScriptEnv * context)
{
    if (!argument_checks(context, "icon48_command", "")) {
        return script_false;
    }
    init_command();
    clear_selection();
    _main->iconResize(48);
    end_command();
    return script_null;
}

ScriptValue
icon64_command(ScriptEnv * context)
{
    if (!argument_checks(context, "icon64_command", "")) {
        return script_false;
    }
    init_command();
    clear_selection();
    _main->iconResize(64);
    end_command();
    return script_null;
}

/* MIRRORSELECTED */
ScriptValue
mirrorselected_command(ScriptEnv *context)
{
    if (!argument_checks(context, "mirrorSelected", "rrrr")) {
        return script_false;
    }

    init_command();
    _main->nativeMirrorSelected(REAL(0), REAL(1), REAL(2), REAL(3));
    end_command();
    return script_null;
}

/* MOVESELECTED */
ScriptValue
moveselected_command(ScriptEnv *context)
{
    end_command();
    return script_null;
}

/* NEW is not context-dependant. */
ScriptValue
new_command(ScriptEnv * context)
{
    init_command();
    clear_selection();
    _main->newFile();
    end_command();
    return script_null;
}

/* OPEN is not context-sensitive. */
ScriptValue
open_command(ScriptEnv * context)
{
    init_command();
    clear_selection();
    _main->openFile();
    end_command();
    return script_null;
}


/* WINDOWTILE is not context-dependant */
ScriptValue
paste_command(ScriptEnv *context)
{
    if (!argument_checks(context, "whats_this_command", "")) {
        return script_false;
    }
    init_command();
    clear_selection();
    end_command();
    return script_null;
}

/* PLATFORM is not context-sensitive.
 * Should this display in the command prompt or just return like GET?
 */
ScriptValue
platform_command(ScriptEnv *context)
{
    if (!argument_checks(context, "debug", "")) {
        return script_false;
    }

    init_command();
//  prompt_output(translate("Platform") + " = " + _main->platformString());
    end_command();
    return script_null;
}

/* PREVIEWOFF . */
ScriptValue
previewoff_command(ScriptEnv *context)
{
    if (!argument_checks(context, "PreviewOff", "")) {
        return script_false;
    }

    View* gview = activeView();
    if (gview) {
        gview->previewOff();
    }
    end_command();
    return script_null;
}

/* PREVIEWON . */
ScriptValue
previewon_command(ScriptEnv *context)
{
    if (!argument_checks(context, "previewon_command", "ssrrr")) {
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
        prompt_output("UNKNOWN_ERROR previewOn(): first argument must be \"SELECTED\" or \"RUBBER\".");
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
        prompt_output("UNKNOWN_ERROR previewOn(): second argument must be \"MOVE\", \"ROTATE\" or \"SCALE\".");
        return script_false;
    }

    View* gview = activeView();
    if (gview) {
        gview->previewOn(clone, mode, REAL(2), -REAL(3), REAL(4));
    }
    else {
        prompt_output("Preview on requires an active view.");
    }
    end_command();
    return script_null;
}

/* PRINT . */
ScriptValue
print_command(ScriptEnv *context)
{
    init_command();
    clear_selection();

    end_command();
    return script_null;
}

/* REDO is not context-sensitive. */
ScriptValue
redo_command(ScriptEnv * context)
{
    if (!argument_checks(context, "redo_command", "")) {
        return script_false;
    }
    init_command();
    clear_selection();
    _main->redo();
    end_command();
    return script_null;
}

/* SAVE. */
ScriptValue
save_command(ScriptEnv *context)
{
    if (!argument_checks(context, "redo_command", "")) {
        return script_false;
    }
    init_command();
    clear_selection();
    end_command();
    return script_null;
}

/* SCALESELECTED . */
ScriptValue
scaleselected_command(ScriptEnv *context)
{
    end_command();
    return script_null;
}

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

    init_command();

    if (value == "TEXTANGLE") {
        if (context->argument[1].type != SCRIPT_REAL) {
            return script_false;
        }
        text_angle.setting = REAL(1);
    }
    else if (value == "TEXTBOLD") {
        if (context->argument[1].type != SCRIPT_BOOL) {
            return script_false;
        }
        text_style_bold.setting = BOOL(1);
    }
    else if (value == "TEXTITALIC") {
        if (context->argument[1].type != SCRIPT_BOOL) {
            return script_false;
        }
        text_style_italic.setting = BOOL(1);
    }
    else if (value == "TEXTFONT") {
        if (context->argument[1].type != SCRIPT_STRING) {
            return script_false;
        }
        strcpy(text_font.setting, STR(1));
    }
    else if (value == "TEXTOVERLINE") {
        if (context->argument[1].type != SCRIPT_BOOL) {
            return script_false;
        }
        text_style_overline.setting = BOOL(1);
    }
    else if (value == "TEXTSIZE") {
        if (context->argument[1].type != SCRIPT_REAL) {
            return script_false;
        }
        text_size.setting = REAL(1);
    }
    else if (value == "TEXTSTRIKEOUT") {
        if (context->argument[1].type != SCRIPT_BOOL) {
            return script_false;
        }
        text_style_strikeout.setting = BOOL(1);
    }
    else if (value == "TEXTUNDERLINE") {
        if (context->argument[1].type != SCRIPT_BOOL) {
            return script_false;
        }
        text_style_underline.setting = BOOL(1);
    }

    end_command();
    return script_null;
}

/* WINDOWTILE is not context-dependant */
ScriptValue
settings_dialog_command(ScriptEnv *context)
{
    if (!argument_checks(context, "settings_dialog_command", "")) {
        return script_false;
    }
    init_command();
    clear_selection();
    _main->settingsDialog();
    end_command();
    return script_null;
}

/* SYSWINDOWS */
ScriptValue
syswindows_command(ScriptEnv * context)
{
    init_command();
    clear_selection();
    prompt_output(translate("Enter an option [Cascade/Tile]: "));

    // Do nothing for click, context
    
    #if 0
    if (str == "C" || str == "CASCADE") {
        //TODO: Probably should add additional translate calls here.
        _main->windowCascade();
        end_command();
    }
    else if (str == "T" || str == "TILE") {
        //TODO: Probably should add additional translate calls here.
        _main->windowTile();
        end_command();
    }
    else {
        alert(translate("Invalid option keyword."));
        prompt_output(translate("Enter an option [Cascade/Tile]: "));
    }
    #endif
    return script_null;
}

/* ZOOMIN is not context-dependant */
ScriptValue
text_bold_command(ScriptEnv *context)
{
    if (!argument_checks(context, "text_bold_command", "")) {
        return script_false;
    }
    init_command();
    clear_selection();
    /* text_style_bold.setting; */
    end_command();
    return script_null;
}

/* ZOOMIN is not context-dependant */
ScriptValue
text_italic_command(ScriptEnv *context)
{
    if (!argument_checks(context, "text_italic_command", "")) {
        return script_false;
    }
    init_command();
    clear_selection();
    /* text_style_italic.setting; */
    end_command();
    return script_null;
}

/* ZOOMIN is not context-dependant */
ScriptValue
text_underline_command(ScriptEnv *context)
{
    if (!argument_checks(context, "text_underline_command", "")) {
        return script_false;
    }
    init_command();
    clear_selection();
    /* text_style_underline.setting; */
    end_command();
    return script_null;
}

/* ZOOMIN is not context-dependant */
ScriptValue
text_overline_command(ScriptEnv *context)
{
    if (!argument_checks(context, "text_overline_command", "")) {
        return script_false;
    }
    init_command();
    clear_selection();
    /* text_style_overline.setting; */
    end_command();
    return script_null;
}

/* ZOOMIN is not context-dependant */
ScriptValue
text_strikeout_command(ScriptEnv *context)
{
    if (!argument_checks(context, "text_strikeout_command", "")) {
        return script_false;
    }
    init_command();
    clear_selection();
    /* text_style_strikeout.setting; */
    end_command();
    return script_null;
}


/* TIPOFTHEDAY is not context-sensitive. */
ScriptValue
tipoftheday_command(ScriptEnv * context)
{
    init_command();
    clear_selection();
    _main->tipOfTheDay();
    end_command();
    return script_null;
}

/* TODO is not context-sensitive. */
ScriptValue
todo_command(ScriptEnv *context)
{
    if (!argument_checks(context, "todo", "ss")) {
        return script_false;
    }
    init_command();
    _main->prompt->alert("TODO: (" + QSTR(0) + ") " + QSTR(1));
    end_command();
    return script_null;
}

/* UNDO */
ScriptValue
undo_command(ScriptEnv * context)
{
    init_command();
    clear_selection();
    _main->undo();
    end_command();
    return script_null;
}

/* VULCANIZE is not context-sensitve. */
ScriptValue
vulcanize_command(ScriptEnv * context)
{
    if (!argument_checks(context, "vulcanize", "")) {
        return script_false;
    }

    init_command();
    clear_selection();
    _main->nativeVulcanize();
    end_command();
    return script_null;
}

/* WINDOWTILE is not context-dependant */
ScriptValue
whats_this_command(ScriptEnv *context)
{
    if (!argument_checks(context, "whats_this_command", "")) {
        return script_false;
    }
    init_command();
    clear_selection();
    end_command();
    return script_null;
}


/* --------------------------------------------------------------------------
 */

ScriptValue
blink_prompt_command(ScriptEnv* context)
{
    if (!argument_checks(context, "debug", "")) {
        return script_false;
    }

    _main->nativeBlinkPrompt();
    return script_null;
}

ScriptValue
set_prompt_prefix_command(ScriptEnv* context)
{
    if (!argument_checks(context, "debug", "s")) {
        return script_false;
    }
    _main->nativeSetPromptPrefix(QSTR(0));
    return script_null;
}

ScriptValue
append_prompt_history(ScriptEnv* context)
{
    int args = context->argumentCount;
    if (args == 0) {
        prompt_output("");
    }
    else if (args == 1) {
        prompt_output(STR(0));
    }
    else {
        prompt_output("appendPromptHistory() requires one or zero arguments");
        return script_false;
    }
    return script_null;
}

ScriptValue
enable_prompt_rapid_fire(ScriptEnv* context)
{
    if (!argument_checks(context, "debug", "")) {
        return script_false;
    }

    _main->nativeEnablePromptRapidFire();
    return script_null;
}

ScriptValue
disable_prompt_rapid_fire(ScriptEnv* context)
{
    if (!argument_checks(context, "debug", "")) {
        return script_false;
    }

    _main->nativeDisablePromptRapidFire();
    return script_null;
}

ScriptValue
enable_move_rapid_fire(ScriptEnv* context)
{
    if (!argument_checks(context, "debug", "")) {
        return script_false;
    }

    _main->nativeEnableMoveRapidFire();
    return script_null;
}

ScriptValue
disable_move_rapid_fire(ScriptEnv* context)
{
    if (!argument_checks(context, "debug", "")) {
        return script_false;
    }

    _main->nativeDisableMoveRapidFire();
    return script_null;
}

ScriptValue
messagebox(ScriptEnv* context)
{
    if (!argument_checks(context, "debug", "sss")) {
        return script_false;
    }

    QString type  = QSTR(0).toLower();
    QString title = QSTR(1);
    QString text  = QSTR(2);

    if (type != "critical" && type != "information" && type != "question" && type != "warning") {
        prompt_output("UNKNOWN_ERROR messageBox(): first argument must be \"critical\", \"information\", \"question\" or \"warning\".");
        return script_false;
    }

    _main->messageBox(type, title, text);
    return script_null;
}

ScriptValue
is_int_command(ScriptEnv* context)
{
    if (!argument_checks(context, "debug", "i")) {
        return script_false;
    }
    return script_true;
}

ScriptValue
print_area_command(ScriptEnv* context)
{
    if (!argument_checks(context, "printArea", "rrrr")) {
        return script_false;
    }
    _main->nativePrintArea(REAL(0), REAL(1), REAL(2), REAL(3));
    return script_null;
}

ScriptValue
set_background_color_command(ScriptEnv* context)
{
    if (!argument_checks(context, "debug", "rrr")) {
        return script_false;
    }
    if (REAL(0) < 0 || REAL(0) > 255) {
        prompt_output("UNKNOWN_ERROR setBackgroundColor(): r value must be in range 0-255");
        return script_false;
    }
    if (REAL(1) < 0 || REAL(1) > 255) {
        prompt_output("UNKNOWN_ERROR setBackgroundColor(): g value must be in range 0-255");
        return script_false;
    }
    if (REAL(2) < 0 || REAL(2) > 255) {
        prompt_output("UNKNOWN_ERROR setBackgroundColor(): b value must be in range 0-255");
        return script_false;
    }

    _main->nativeSetBackgroundColor(REAL(0), REAL(1), REAL(2));
    return script_null;
}

ScriptValue
set_crosshair_color_command(ScriptEnv* context)
{
    if (!argument_checks(context, "debug", "rrr")) {
        return script_false;
    }

    if (REAL(0) < 0 || REAL(0) > 255) {
        prompt_output("UNKNOWN_ERROR setCrossHairColor(): r value must be in range 0-255");
        return script_false;
    }
    if (REAL(1) < 0 || REAL(1) > 255) {
        prompt_output("UNKNOWN_ERROR setCrossHairColor(): g value must be in range 0-255");
        return script_false;
    }
    if (REAL(2) < 0 || REAL(2) > 255) {
        prompt_output("UNKNOWN_ERROR setCrossHairColor(): b value must be in range 0-255");
        return script_false;
    }

    _main->nativeSetCrossHairColor(REAL(0), REAL(1), REAL(2));
    return script_null;
}

ScriptValue
set_grid_color_command(ScriptEnv* context)
{
    if (!argument_checks(context, "debug", "rrr")) {
        return script_false;
    }

    if (REAL(0) < 0 || REAL(0) > 255) {
        prompt_output("UNKNOWN_ERROR setGridColor(): r value must be in range 0-255");
        return script_false;
    }
    if (REAL(1) < 0 || REAL(1) > 255) {
        prompt_output("UNKNOWN_ERROR setGridColor(): g value must be in range 0-255");
        return script_false;
    }
    if (REAL(2) < 0 || REAL(2) > 255) {
        prompt_output("UNKNOWN_ERROR setGridColor(): b value must be in range 0-255");
        return script_false;
    }

    _main->nativeSetGridColor(REAL(0), REAL(1), REAL(2));
    return script_null;
}

ScriptValue
add_text_multi_command(ScriptEnv* context)
{
    if (!argument_checks(context, "mouseX", "srrrb")) {
        return script_false;
    }
    _main->nativeAddTextMulti(QSTR(0), REAL(1), REAL(2), REAL(3), BOOL(4), OBJ_RUBBER_OFF);
    return script_null;
}

ScriptValue
add_text_single_command(ScriptEnv* context)
{
    if (!argument_checks(context, "mouseX", "srrrb")) {
        return script_false;
    }
    _main->nativeAddTextSingle(QSTR(0), REAL(1), REAL(2), REAL(3), BOOL(4), OBJ_RUBBER_OFF);
    return script_null;
}

ScriptValue
add_infinite_line(ScriptEnv* context)
{
    /* TODO: parameter error checking */
    prompt_output("TODO: finish addInfiniteLine command");
    return script_null;
}

ScriptValue
add_ray_command(ScriptEnv* context)
{
    /* TODO: parameter error checking */
    prompt_output("TODO: finish addRay command");
    return script_null;
}

ScriptValue
add_line_command(ScriptEnv* context)
{
    if (!argument_checks(context, "add_line_command", "rrrrr")) {
        return script_false;
    }
    _main->nativeAddLine(REAL(0), REAL(1), REAL(2), REAL(3), REAL(4), OBJ_RUBBER_OFF);
    return script_null;
}

ScriptValue
add_triangle_command(ScriptEnv* context)
{
    if (!argument_checks(context, "add_triangle_command", "rrrrrrrb")) {
        return script_false;
    }
    _main->nativeAddTriangle(REAL(0), REAL(1), REAL(2), REAL(3), REAL(4), REAL(5), REAL(6), BOOL(7));
    return script_null;
}

ScriptValue
add_rectangle_command(ScriptEnv* context)
{
    if (!argument_checks(context, "add_rectangle_command", "rrrrrb")) {
        return script_false;
    }
    _main->nativeAddRectangle(REAL(0), REAL(1), REAL(2), REAL(3), REAL(4), BOOL(5), OBJ_RUBBER_OFF);
    return script_null;
}

ScriptValue
add_rounded_rectangle_command(ScriptEnv* context)
{
    if (!argument_checks(context, "add_rounded_rectangle", "rrrrrrb")) {
        return script_false;
    }
    _main->nativeAddRoundedRectangle(REAL(0), REAL(1), REAL(2), REAL(3), REAL(4), REAL(5), BOOL(6));
    return script_null;
}

ScriptValue
add_arc_command(ScriptEnv* context)
{
    if (!argument_checks(context, "mouseX", "rrrrrr")) {
        return script_false;
    }
    EmbArc arc;
    arc.start.x = REAL(0);
    arc.start.y = REAL(1);
    arc.mid.x = REAL(2);
    arc.mid.y = REAL(3);
    arc.end.x = REAL(4);
    arc.end.y = REAL(5);
    _main->nativeAddArc(arc, OBJ_RUBBER_OFF);
    return script_null;
}

ScriptValue
add_circle_command(ScriptEnv* context)
{
    if (!argument_checks(context, "mouseX", "rrrb")) {
        return script_false;
    }
    _main->nativeAddCircle(REAL(0), REAL(1), REAL(2), BOOL(4), OBJ_RUBBER_OFF);
    return script_null;
}

ScriptValue
add_slot_command(ScriptEnv* context)
{
    if (!argument_checks(context, "mouseX", "rrrrrb")) {
        return script_false;
    }
    _main->nativeAddSlot(REAL(0), REAL(1), REAL(2), REAL(3), REAL(4), BOOL(5), OBJ_RUBBER_OFF);
    return script_null;
}

ScriptValue
add_ellipse_command(ScriptEnv* context)
{
    if (!argument_checks(context, "mouseX", "rrrrrb")) {
        return script_false;
    }
    _main->nativeAddEllipse(REAL(0), REAL(1), REAL(2), REAL(3), REAL(4), BOOL(5), OBJ_RUBBER_OFF);
    return script_null;
}

ScriptValue
add_point_command(ScriptEnv* context)
{
    if (!argument_checks(context, "mouseX", "rr")) {
        return script_false;
    }
    _main->nativeAddPoint(REAL(0), REAL(1));
    return script_null;
}

ScriptValue
add_regular_polygon_command(ScriptEnv* context)
{
    /* TODO: parameter error checking */
    prompt_output("TODO: finish addRegularPolygon command");
    return script_null;
}

ScriptValue
add_polygon_command(ScriptEnv* context)
{
    #if 0
    if (context->argumentCount != 1) {
        prompt_output("addPolygon() requires one argument");
        return script_false;
    }
    if (!context->argument[0].isArray()) {
        prompt_output("TYPE_ERROR, addPolygon(): first argument is not an array");
        return script_false;
    }

    QVariantList varList = context->argument[0].toVariant().toList();
    int varSize = varList.size();
    if (varSize < 2) {
        prompt_output("TYPE_ERROR, addPolygon(): array must contain at least two elements");
        return script_false;
    }
    if (varSize % 2) {
        prompt_output("TYPE_ERROR, addPolygon(): array cannot contain an odd number of elements");
        return script_false;
    }

    bool lineTo = false;
    bool xCoord = true;
    double x = 0;
    double y = 0;
    double startX = 0;
    double startY = 0;
    QPainterPath path;
    foreach (QVariant var, varList) {
        if (var.canConvert(QVariant::Double)) {
            if (xCoord) {
                xCoord = false;
                x = var.toReal();
            }
            else {
                xCoord = true;
                y = -var.toReal();

                if (lineTo) {
                    path.lineTo(x,y);
                }
                else {
                    path.moveTo(x,y);
                    lineTo = true;
                    startX = x;
                    startY = y;
                }
            }
        }
        else {
            prompt_output("TYPE_ERROR, addPolygon(): array contains one or more invalid elements");
            return script_false;
        }
    }

    //Close the polygon
    path.closeSubpath();

    path.translate(-startX, -startY);

    _main->nativeAddPolygon(startX, startY, path, OBJ_RUBBER_OFF);
    #endif
    return script_null;
}

ScriptValue
add_polyline_command(ScriptEnv* context)
{
    #if 0
    if (context->argumentCount != 1) {
        prompt_output("addPolyline() requires one argument");
        return script_false;
    }
    if (!context->argument[0].isArray()) {
        prompt_output("TYPE_ERROR, addPolyline(): first argument is not an array");
        return script_false;
    }

    QVariantList varList = context->argument[0].toVariant().toList();
    int varSize = varList.size();
    if (varSize < 2) {
        prompt_output("TYPE_ERROR, addPolyline(): array must contain at least two elements");
        return script_false;
    }
    if (varSize % 2) {
        prompt_output("TYPE_ERROR, addPolyline(): array cannot contain an odd number of elements");
        return script_false;
    }

    bool lineTo = false;
    bool xCoord = true;
    double x = 0;
    double y = 0;
    double startX = 0;
    double startY = 0;
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

                if (lineTo) {
                    path.lineTo(x,y);
                }
                else {
                    path.moveTo(x,y);
                    lineTo = true;
                    startX = x;
                    startY = y;
                }
            }
        }
        else {
            prompt_output("TYPE_ERROR, addPolyline(): array contains one or more invalid elements");
            return script_false;
        }
    }

    path.translate(-startX, -startY);

    _main->nativeAddPolyline(startX, startY, path, OBJ_RUBBER_OFF);
    #endif
    return script_null;
}

/* . */
ScriptValue
add_path_command(ScriptEnv* context)
{
    /* TODO: parameter error checking */
    prompt_output("TODO: finish addPath command");
    return script_null;
}

/* . */
ScriptValue
add_horizontal_dimension_command(ScriptEnv* context)
{
    /* TODO: parameter error checking */
    prompt_output("TODO: finish addHorizontalDimension command");
    return script_null;
}

/* . */
ScriptValue
add_vertical_dimension_command(ScriptEnv* context)
{
    /* TODO: parameter error checking */
    prompt_output("TODO: finish addVerticalDimension command");
    return script_null;
}

ScriptValue
add_image_command(ScriptEnv* context)
{
    /* TODO: parameter error checking */
    prompt_output("TODO: finish addImage command");
    return script_null;
}

ScriptValue
add_dimleader_command(ScriptEnv* context)
{
    if (!argument_checks(context, "calculateAngle", "rrrrr")) {
        return script_false;
    }

    _main->nativeAddDimLeader(REAL(0), REAL(1), REAL(2), REAL(3), REAL(4), OBJ_RUBBER_OFF);
    return script_null;
}

ScriptValue
set_cursor_shape_command(ScriptEnv* context)
{
    if (!argument_checks(context, "calculateAngle", "s")) {
        return script_false;
    }

    _main->nativeSetCursorShape(QSTR(0));
    return script_null;
}

ScriptValue
calculate_angle_command(ScriptEnv* context)
{
    if (!argument_checks(context, "calculateAngle", "rrrr")) {
        return script_false;
    }

    double r = _main->nativeCalculateAngle(REAL(0), REAL(1), REAL(2), REAL(3));
    return script_real(r);
}

ScriptValue
calculate_distance_command(ScriptEnv* context)
{
    if (!argument_checks(context, "numSelected", "rrrr")) {
        return script_false;
    }

    double r = _main->nativeCalculateDistance(REAL(0), REAL(1), REAL(2), REAL(3));
    return script_real(r);
}

ScriptValue
perpendicular_distance_command(ScriptEnv* context)
{
    if (!argument_checks(context, "numSelected", "rrrrrr")) {
        return script_false;
    }

    double r = _main->nativePerpendicularDistance(REAL(0), REAL(1), REAL(2), REAL(3), REAL(4), REAL(5));
    return script_real(r);
}

ScriptValue
num_selected_command(ScriptEnv* context)
{
    if (!argument_checks(context, "numSelected", "")) {
        return script_false;
    }

    return script_int(_main->nativeNumSelected());
}

ScriptValue
select_all_command(ScriptEnv* context)
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
add_to_selection_command(ScriptEnv* context)
{
    return script_null;
}

ScriptValue
delete_selected_command(ScriptEnv* context)
{
    if (!argument_checks(context, "deleteSelected", "")) {
        return script_false;
    }

    _main->nativeDeleteSelected();
    return script_null;
}

ScriptValue
cut_selected_command(ScriptEnv* context)
{
    if (!argument_checks(context, "scaleSelected", "rr")) {
        return script_false;
    }

    _main->nativeCutSelected(REAL(0), REAL(1));
    return script_null;
}

ScriptValue
copy_selected_command(ScriptEnv* context)
{
    if (!argument_checks(context, "scaleSelected", "rr")) {
        return script_false;
    }

    _main->nativeCopySelected(REAL(0), REAL(1));
    return script_null;
}

ScriptValue
paste_selected_command(ScriptEnv* context)
{
    if (!argument_checks(context, "scaleSelected", "rr")) {
        return script_false;
    }

    _main->nativePasteSelected(REAL(0), REAL(1));
    return script_null;
}

ScriptValue
move_selected_command(ScriptEnv* context)
{
    if (!argument_checks(context, "scaleSelected", "rr")) {
        return script_false;
    }

    _main->nativeMoveSelected(REAL(0), REAL(1));
    return script_null;
}

ScriptValue
scale_selected_command(ScriptEnv* context)
{
    if (!argument_checks(context, "scaleSelected", "rrr")) {
        return script_false;
    }

    if (REAL(2) <= 0.0) {
        prompt_output("UNKNOWN_ERROR scaleSelected(): scale factor must be greater than zero");
        return script_false;
    }

    _main->nativeScaleSelected(REAL(0), REAL(1), REAL(2));
    return script_null;
}

ScriptValue
rotate_selected_command(ScriptEnv* context)
{
    if (!argument_checks(context, "rotate_selected_command", "rrr")) {
        return script_false;
    }

    _main->nativeRotateSelected(REAL(0), REAL(1), REAL(2));
    return script_null;
}
