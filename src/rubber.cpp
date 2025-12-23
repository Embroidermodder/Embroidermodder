/*
 * Embroidermodder 2
 * Copyright 2011-2025 The Embroidermodder Team
 *
 * Embroidermodder 2 is free and open software under the zlib license:
 * see LICENSE.md for details.
 *
 * ----------------------------------------------------------------------------
 *
 * Wrappers for commands that deal with the rubber GUI.
 *
 * Try to keep this section of the file alphabetized in the function name to
 * match the declarations above.
 *
 * All function names should end in `_c`.
 *
 * TODO: void nativeSetRubberFilter(qint64 id);
 * TODO: This is so more than 1 rubber object can exist at one time without
 *       updating all rubber objects at once
 */

#include "embroidermodder.h"

/* TODO: move add_rubber to script */
int
add_rubber(void)
{
    /*
    if (context->argumentCount() != 1) {
        return debug("addRubber() requires one argument");
    }
    if (!context->argument(0).isString()) {
        return debug(TypeError, "addRubber(): first argument is not a string");
    }

    QString objType = context->argument(0).toString().toUpper();

    // FIXME
    cmd("a = allow_rubber()");
    if (!a) {
        return debug(UnknownError, "addRubber(): You must use vulcanize() before you can add another rubber object.");
    }

    double mx = _mainWin->nativeMouseX();
    double my = _mainWin->nativeMouseY();

    if (objType == "ARC") {
        //TODO: handle this type
    }
    else if (objType == "BLOCK") {
        //TODO: handle this type
    }
    else if (objType == "CIRCLE") {
        _mainWin->add_circle(mx, my, 0, false, OBJ_RUBBER_ON);
    }
    else if (objType == "DIMALIGNED") {
        //TODO: handle this type
    }
    else if (objType == "DIMANGULAR") {
        //TODO: handle this type
    }
    else if (objType == "DIMARCLENGTH") {
        //TODO: handle this type
    }
    else if (objType == "DIMDIAMETER") {

} //TODO: handle this type
    else if (objType == "DIMLEADER") {
        _mainWin->add_dim_leader(mx, my, mx, my, 0, OBJ_RUBBER_ON);
    }
    else if (objType == "DIMLINEAR") {

} //TODO: handle this type
    else if (objType == "DIMORDINATE") {

} //TODO: handle this type
    else if (objType == "DIMRADIUS")   {

} //TODO: handle this type
    else if (objType == "ELLIPSE") { _mainWin->add_ellipse(mx, my, 0, 0, 0, 0, OBJ_RUBBER_ON); }
    else if (objType == "ELLIPSEARC") {

} //TODO: handle this type
    else if (objType == "HATCH")       {} //TODO: handle this type
    else if (objType == "IMAGE")       {} //TODO: handle this type
    else if (objType == "INFINITELINE") {} //TODO: handle this type
    else if (objType == "LINE")        { _mainWin->add_line(mx, my, mx, my, 0, OBJ_RUBBER_ON); }
    else if (objType == "PATH")        {} //TODO: handle this type
    else if (objType == "POINT")       {} //TODO: handle this type
    else if (objType == "POLYGON")     { _mainWin->add_polygon(mx, my, QPainterPath(), OBJ_RUBBER_ON); }
    else if (objType == "POLYLINE")    { _mainWin->add_polyline(mx, my, QPainterPath(), OBJ_RUBBER_ON); }
    else if (objType == "RAY")         {} //TODO: handle this type
    else if (objType == "RECTANGLE")   { _mainWin->add_rectangle(mx, my, mx, my, 0, 0, OBJ_RUBBER_ON); }
    else if (objType == "SPLINE")      {} //TODO: handle this type
    else if (objType == "TEXTMULTI")   {} //TODO: handle this type
    else if (objType == "TEXTSINGLE")  { _mainWin->add_text_single("", mx, my, 0, false, OBJ_RUBBER_ON); }
    */
    return 0;
}

/* . */
int
allow_rubber(void)
{
    View* gview = activeView();
    /*
    if (gview) {
        return gview->allowRubber();
    }
    return false;
    */
    return 0;
}

/* . */
int
clear_rubber(void)
{
    View* gview = activeView();
    if (gview) {
        gview->clearRubberRoom();
    }
    return 0;
}

/* Turn on default rubber mode for any selected objects if a selection is active.
 * Otherwise, activate for all objects in design.
 *
 * FIXME:
 */
int
rubber(void)
{
    debug("TODO: add_to_menu");
    return 0;
}

/* . */
int
set_rubber_mode(void)
{
    debug("TODO: add_to_menu");
    View* gview = activeView();
    if (!gview) {
        return 1;
    }
    /*
    if (context->argumentCount() != 1)    return debug("setRubberMode() requires one argument");
    if (!context->argument(0).isString()) return debug(TypeError, "setRubberMode(): first argument is not a string");

    QString mode = context->argument(0).toString().toUpper();

    if (mode == "CIRCLE_1P_RAD") {
        gview->setRubberMode(OBJ_RUBBER_CIRCLE_1P_RAD);
    }
    else if (mode == "CIRCLE_1P_DIA") {
        gview->setRubberMode(OBJ_RUBBER_CIRCLE_1P_DIA);
    }
    else if (mode == "CIRCLE_2P") {
        gview->setRubberMode(OBJ_RUBBER_CIRCLE_2P);
    }
    else if (mode == "CIRCLE_3P") {
        gview->setRubberMode(OBJ_RUBBER_CIRCLE_3P);
    }
    else if (mode == "CIRCLE_TTR") {
        gview->setRubberMode(OBJ_RUBBER_CIRCLE_TTR);
    }
    else if (mode == "CIRCLE_TTR") {
        gview->setRubberMode(OBJ_RUBBER_CIRCLE_TTT);
    }
    else if (mode == "DIMLEADER_LINE") {
        gview->setRubberMode(OBJ_RUBBER_DIMLEADER_LINE);
    }
    else if (mode == "ELLIPSE_LINE") {
        gview->setRubberMode(OBJ_RUBBER_ELLIPSE_LINE);
    }
    else if (mode == "ELLIPSE_MAJORDIAMETER_MINORRADIUS") {
        gview->setRubberMode(OBJ_RUBBER_ELLIPSE_MAJORDIAMETER_MINORRADIUS);
    }
    else if (mode == "ELLIPSE_MAJORRADIUS_MINORRADIUS") {
        gview->setRubberMode(OBJ_RUBBER_ELLIPSE_MAJORRADIUS_MINORRADIUS);
    }
    else if (mode == "ELLIPSE_ROTATION") {
        gview->setRubberMode(OBJ_RUBBER_ELLIPSE_ROTATION);
    }
    else if (mode == "LINE") {
        gview->setRubberMode(OBJ_RUBBER_LINE);
    }
    else if (mode == "POLYGON") {
        gview->setRubberMode(OBJ_RUBBER_POLYGON);
    }
    else if (mode == "POLYGON_INSCRIBE") {
        gview->setRubberMode(OBJ_RUBBER_POLYGON_INSCRIBE);
    }
    else if (mode == "POLYGON_CIRCUMSCRIBE") {
        gview->setRubberMode(OBJ_RUBBER_POLYGON_CIRCUMSCRIBE);
    }
    else if (mode == "POLYLINE") {
        gview->setRubberMode(OBJ_RUBBER_POLYLINE);
    }
    else if (mode == "RECTANGLE") {
        gview->setRubberMode(OBJ_RUBBER_RECTANGLE);
    }
    else if (mode == "TEXTSINGLE") {
        gview->setRubberMode(OBJ_RUBBER_TEXTSINGLE);
    }
    else {
        return debug(UnknownError, "setRubberMode(): unknown rubberMode value");
    }
    */
    return 0;
}

/* TODO: convert set_rubber_point_f. */
int
set_rubber_point(const char *key_, double x, double y)
{
    QString key = QString(key_).toUpper();

    if (std::isnan(x)) {
        debug("TypeError, set_rubber_point_f: second argument failed isNaN check.");
        return 0;
    }
    if (std::isnan(y)) {
        debug("TypeError, set_rubber_point_f: third argument failed isNaN check.");
        return 0;
    }

    View* gview = activeView();
    if (gview) {
        gview->setRubberPoint(key, QPointF(x, -y));
    }
    return 0;
}

/* . */
int
set_rubber_text(const char *key_, const char *txt_)
{
    QString key = QString(key_).toUpper();
    QString txt = txt_;

    View* gview = activeView();
    if (gview) {
        gview->setRubberText(key, txt);
    }
    return 0;
}

/* . */
int
spare_rubber(const char *obj_type)
{
    View* gview = activeView();
    if (!gview) {
        return 1;
    }

    QString objID = QString(obj_type).toUpper();

    if (objID == "PATH") {
        gview->spareRubber(SPARE_RUBBER_PATH);
    }
    else if (objID == "POLYGON") {
        gview->spareRubber(SPARE_RUBBER_POLYGON);
    }
    else if (objID == "POLYLINE") {
        gview->spareRubber(SPARE_RUBBER_POLYLINE);
    }
    else {
        bool ok = false;
        qint64 id = objID.toLongLong(&ok);
        if (!ok) {
            debug("TypeError, spare_rubber(): error converting object ID into an int64");
            return 0;
        }
        gview->spareRubber(id);
    }

    return 0;
}

/* Fix all rubber points. */
int
vulcanize(void)
{
    View* gview = activeView();
    if (gview) {
        gview->vulcanizeRubberRoom();
    }
    return 0;
}

