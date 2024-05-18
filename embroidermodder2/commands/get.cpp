/*
 * Embroidermodder 2.
 *
 * Copyright 2013-2024 The Embroidermodder Team
 * Embroidermodder 2 is Open Source Software, see LICENSE for licensing terms.
 * Read CODE-STANDARDS.txt for advice on altering this file.
 */

#include "../commands.h"

#include <QGraphicsScene>

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

Command get_cmd = {
    .id = -1,
    .main = do_nothing,
    .click = do_nothing,
    .context = do_nothing,
    .prompt = get_prompt,
    .icon = "get",
    .menu_name = "None",
    .menu_position = 0,
    .toolbar_name = "None",
    .toolbar_position = 0,
    .tooltip = "&Get",
    .statustip = "Print a value to one of the pre-defined global variables.",
    .alias = "SET"
};
