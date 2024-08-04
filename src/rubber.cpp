/*
 * Embroidermodder 2.
 *
 * Copyright 2011-2024 The Embroidermodder Team
 * Embroidermodder 2 is Open Source Software, see LICENSE.md for licensing terms.
 *
 * Read the reference manual (https://www.libembroidery.org/downloads/emrm.pdf)
 * for advice on altering this file.
 *
 * Rubber
 */

#include "embroidermodder.h"

ScriptValue
allow_rubber_command(ScriptEnv* context)
{
    if (!argument_checks(context, "allowRubber", "")) {
        return script_false;
    }

    return script_bool(nativeAllowRubber());
}

ScriptValue
set_rubber_mode_command(ScriptEnv* context)
{
    if (!argument_checks(context, "allowRubber", "s")) {
        return script_false;
    }

    std::string mode(STR(0));

    if (mode == "CIRCLE_1P_RAD") {
        nativeSetRubberMode(OBJ_RUBBER_CIRCLE_1P_RAD);
    }
    else if (mode == "CIRCLE_1P_DIA") {
        nativeSetRubberMode(OBJ_RUBBER_CIRCLE_1P_DIA);
    }
    else if (mode == "CIRCLE_2P") {
        nativeSetRubberMode(OBJ_RUBBER_CIRCLE_2P);
    }
    else if (mode == "CIRCLE_3P") {
        nativeSetRubberMode(OBJ_RUBBER_CIRCLE_3P);
    }
    else if (mode == "CIRCLE_TTR") {
        nativeSetRubberMode(OBJ_RUBBER_CIRCLE_TTR);
    }
    else if (mode == "CIRCLE_TTT") {
        nativeSetRubberMode(OBJ_RUBBER_CIRCLE_TTT);
    }
    else if (mode == "DIMLEADER_LINE") {
        nativeSetRubberMode(OBJ_RUBBER_DIMLEADER_LINE);
    }
    else if (mode == "ELLIPSE_LINE") {
        nativeSetRubberMode(OBJ_RUBBER_ELLIPSE_LINE);
    }
    else if (mode == "ELLIPSE_MAJORDIAMETER_MINORRADIUS") {
        nativeSetRubberMode(OBJ_RUBBER_ELLIPSE_MAJORDIAMETER_MINORRADIUS);
    }
    else if (mode == "ELLIPSE_MAJORRADIUS_MINORRADIUS") {
        nativeSetRubberMode(OBJ_RUBBER_ELLIPSE_MAJORRADIUS_MINORRADIUS);
    }
    else if (mode == "ELLIPSE_ROTATION") {
        nativeSetRubberMode(OBJ_RUBBER_ELLIPSE_ROTATION);
    }
    else if (mode == "LINE") {
        nativeSetRubberMode(OBJ_RUBBER_LINE);
    }
    else if (mode == "POLYGON") {
        nativeSetRubberMode(OBJ_RUBBER_POLYGON);
    }
    else if (mode == "POLYGON_INSCRIBE") {
        nativeSetRubberMode(OBJ_RUBBER_POLYGON_INSCRIBE);
    }
    else if (mode == "POLYGON_CIRCUMSCRIBE") {
        nativeSetRubberMode(OBJ_RUBBER_POLYGON_CIRCUMSCRIBE);
    }
    else if (mode == "POLYLINE") {
        nativeSetRubberMode(OBJ_RUBBER_POLYLINE);
    }
    else if (mode == "RECTANGLE") {
        nativeSetRubberMode(OBJ_RUBBER_RECTANGLE);
    }
    else if (mode == "TEXTSINGLE") {
        nativeSetRubberMode(OBJ_RUBBER_TEXTSINGLE);
    }
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

    std::string key(STR(0));
    nativeSetRubberPoint(key, REAL(1), REAL(2));
    return script_null;
}

ScriptValue
set_rubber_text_command(ScriptEnv* context)
{
    if (!argument_checks(context, "SetRubberPoint", "ss")) {
        return script_false;
    }

    std::string key(STR(0));
    std::string txt(STR(1));

    nativeSetRubberText(key, txt);
    return script_null;
}

ScriptValue
add_rubber_command(ScriptEnv* context)
{
    if (!argument_checks(context, "SetRubberPoint", "s")) {
        return script_false;
    }

    std::string objType(STR(0));

    if (!nativeAllowRubber()) {
        prompt_output("UNKNOWN_ERROR addRubber(): You must use vulcanize() before you can add another rubber object.");
        return script_false;
    }

    /* FIXME: ERROR CHECKING */
    double mx = _main->runCommandCore("get mousex", context).r;
    double my = _main->runCommandCore("get mousey", context).r;

    if (objType == "ARC") {
        // TODO: handle this type
    }
    else if (objType == "BLOCK") {
        // TODO: handle this type
    }
    else if (objType == "CIRCLE") {
        nativeAddCircle(mx, my, 0, false, OBJ_RUBBER_ON);
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
        nativeAddDimLeader(mx, my, mx, my, 0, OBJ_RUBBER_ON);
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
        nativeAddEllipse(mx, my, 0, 0, 0, 0, OBJ_RUBBER_ON);
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
        nativeAddLine(mx, my, mx, my, 0, OBJ_RUBBER_ON);
    }
    else if (objType == "PATH") {
        // TODO: handle this type
    }
    else if (objType == "POINT") {
        // TODO: handle this type
    }
    else if (objType == "POLYGON") {
        nativeAddPolygon(mx, my, QPainterPath(), OBJ_RUBBER_ON);
    }
    else if (objType == "POLYLINE") {
        nativeAddPolyline(mx, my, QPainterPath(), OBJ_RUBBER_ON);
    }
    else if (objType == "RAY") {
        // TODO: handle this type
    }
    else if (objType == "RECTANGLE") {
        nativeAddRectangle(mx, my, mx, my, 0, 0, OBJ_RUBBER_ON);
    }
    else if (objType == "SPLINE") {
        // TODO: handle this type
    }
    else if (objType == "TEXTMULTI") {
        // TODO: handle this type
    }
    else if (objType == "TEXTSINGLE") {
        nativeAddTextSingle("", mx, my, 0, false, OBJ_RUBBER_ON);
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

    nativeClearRubber();
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

    QString objID(STR(0));

    if (objID == "PATH") {
        nativeSpareRubber(SPARE_RUBBER_PATH);
    }
    else if (objID == "POLYGON") {
        nativeSpareRubber(SPARE_RUBBER_POLYGON);
    }
    else if (objID == "POLYLINE") {
        nativeSpareRubber(SPARE_RUBBER_POLYLINE);
    }
    else {
        bool ok = false;
        int64_t id = objID.toLongLong(&ok);
        if (!ok) {
            prompt_output("TYPE_ERROR, spareRubber(): error converting object ID into an int64");
            return script_false;
        }
        nativeSpareRubber(id);
    }

    return script_null;
}
