/*
 * Embroidermodder 2.
 *
 * Copyright 2011-2024 The Embroidermodder Team
 * Embroidermodder 2 is Open Source Software, see LICENSE.md for licensing terms.
 * Visit https://www.libembroidery.org/refman for advice on altering this file,
 * or read the markdown version in embroidermodder2/docs/refman.
 *
 * View
 */

#include "embroidermodder.h"

ScriptValue
allow_rubber_command(ScriptEnv* context)
{
    if (!argument_checks(context, "allowRubber", "")) {
        return script_false;
    }

    return script_bool(_main->nativeAllowRubber());
}

ScriptValue
set_rubber_mode_command(ScriptEnv* context)
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
    else if (mode == "ELLIPSE_LINE") {
        _main->nativeSetRubberMode(OBJ_RUBBER_ELLIPSE_LINE);
    }
    else if (mode == "ELLIPSE_MAJORDIAMETER_MINORRADIUS") {
        _main->nativeSetRubberMode(OBJ_RUBBER_ELLIPSE_MAJORDIAMETER_MINORRADIUS);
    }
    else if (mode == "ELLIPSE_MAJORRADIUS_MINORRADIUS") {
        _main->nativeSetRubberMode(OBJ_RUBBER_ELLIPSE_MAJORRADIUS_MINORRADIUS);
    }
    else if (mode == "ELLIPSE_ROTATION") {
        _main->nativeSetRubberMode(OBJ_RUBBER_ELLIPSE_ROTATION);
    }
    else if (mode == "LINE") {
        _main->nativeSetRubberMode(OBJ_RUBBER_LINE);
    }
    else if (mode == "POLYGON")                           { _main->nativeSetRubberMode(OBJ_RUBBER_POLYGON); }
    else if (mode == "POLYGON_INSCRIBE")                  { _main->nativeSetRubberMode(OBJ_RUBBER_POLYGON_INSCRIBE); }
    else if (mode == "POLYGON_CIRCUMSCRIBE")              { _main->nativeSetRubberMode(OBJ_RUBBER_POLYGON_CIRCUMSCRIBE); }

    else if (mode == "POLYLINE")                          { _main->nativeSetRubberMode(OBJ_RUBBER_POLYLINE); }

    else if (mode == "RECTANGLE")                         { _main->nativeSetRubberMode(OBJ_RUBBER_RECTANGLE); }

    else if (mode == "TEXTSINGLE")                        { _main->nativeSetRubberMode(OBJ_RUBBER_TEXTSINGLE); }

    else {
        prompt_output("UNKNOWN_ERROR setRubberMode(): unknown rubberMode value");
        return script_false;
    }

    return script_null;
}

ScriptValue
set_rubber_point_command(ScriptEnv* context)
{
    if (!argument_checks(context, "SetRubberPoint", "srr")) {
        return script_false;
    }

    QString key = QSTR(0).toUpper();
    double x     = context->argument[1].r;
    double y     = context->argument[2].r;

    _main->nativeSetRubberPoint(key, x, y);
    return script_null;
}

ScriptValue
set_rubber_text_command(ScriptEnv* context)
{
    if (!argument_checks(context, "SetRubberPoint", "ss")) {
        return script_false;
    }

    QString key = QSTR(0).toUpper();
    QString txt = QSTR(1);

    _main->nativeSetRubberText(key, txt);
    return script_null;
}

ScriptValue
add_rubber_command(ScriptEnv* context)
{
    if (!argument_checks(context, "SetRubberPoint", "s")) {
        return script_false;
    }

    QString objType = QSTR(0).toUpper();

    if (!_main->nativeAllowRubber()) {
        prompt_output("UNKNOWN_ERROR addRubber(): You must use vulcanize() before you can add another rubber object.");
        return script_false;
    }

    /* FIXME: ERROR CHECKING */
    double mx = command_prompt(context, "get mousex").r;
    double my = command_prompt(context, "get mousey").r;

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
    else if (objType == "DIMANGULAR") {
        // TODO: handle this type
    }
    else if (objType == "DIMARCLENGTH") {
        // TODO: handle this type
    }
    else if (objType == "DIMDIAMETER") {
        // TODO: handle this type
    }
    else if (objType == "DIMLEADER") {
        _main->nativeAddDimLeader(mx, my, mx, my, 0, OBJ_RUBBER_ON);
    }
    else if (objType == "DIMLINEAR") {
        // TODO: handle this type
    }
    else if (objType == "DIMORDINATE") {
        // TODO: handle this type
    }
    else if (objType == "DIMRADIUS") {
        // TODO: handle this type
    }
    else if (objType == "ELLIPSE") {
        _main->nativeAddEllipse(mx, my, 0, 0, 0, 0, OBJ_RUBBER_ON);
    }
    else if (objType == "ELLIPSEARC") {
        // TODO: handle this type
    }
    else if (objType == "HATCH") {
        // TODO: handle this type
    }
    else if (objType == "IMAGE") {
        // TODO: handle this type
    }
    else if (objType == "INFINITELINE") {
        // TODO: handle this type
    }
    else if (objType == "LINE") {
        _main->nativeAddLine(mx, my, mx, my, 0, OBJ_RUBBER_ON);
    }
    else if (objType == "PATH") {
        // TODO: handle this type
    }
    else if (objType == "POINT") {
        // TODO: handle this type
    }
    else if (objType == "POLYGON") {
        _main->nativeAddPolygon(mx, my, QPainterPath(), OBJ_RUBBER_ON);
    }
    else if (objType == "POLYLINE") {
        _main->nativeAddPolyline(mx, my, QPainterPath(), OBJ_RUBBER_ON);
    }
    else if (objType == "RAY") {
        // TODO: handle this type
    }
    else if (objType == "RECTANGLE") {
        _main->nativeAddRectangle(mx, my, mx, my, 0, 0, OBJ_RUBBER_ON);
    }
    else if (objType == "SPLINE") {
        // TODO: handle this type
    }
    else if (objType == "TEXTMULTI") {
        // TODO: handle this type
    }
    else if (objType == "TEXTSINGLE") {
        _main->nativeAddTextSingle("", mx, my, 0, false, OBJ_RUBBER_ON);
    }

    return script_null;
}

ScriptValue
clear_rubber_command(ScriptEnv* context)
{
    if (context->argumentCount != 0) {
        prompt_output("clearRubber() requires zero arguments");
        return script_false;
    }

    _main->nativeClearRubber();
    return script_null;
}

ScriptValue
spare_rubber_command(ScriptEnv* context)
{
    if (context->argumentCount != 1) {
        prompt_output("spareRubber() requires one argument");
        return script_false;
    }
    if (context->argument[0].type != SCRIPT_STRING) {
        prompt_output("TYPE_ERROR, spareRubber(): first argument is not a string");
        return script_false;
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
        int64_t id = objID.toLongLong(&ok);
        if (!ok) {
            prompt_output("TYPE_ERROR, spareRubber(): error converting object ID into an int64");
            return script_false;
        }
        _main->nativeSpareRubber(id);
    }

    return script_null;
}
