/*
 * Embroidermodder 2 -- Callbacks
 * Copyright 2011-2026 The Embroidermodder Team
 */

#include <QLabel>
#include <QDesktopServices>
#include <QApplication>
#include <QUrl>
#include <QProcess>
#include <QMessageBox>
#include <QDialogButtonBox>
#include <QPushButton>
#include <QMdiArea>
#include <QGraphicsScene>
#include <QComboBox>
#include <QWhatsThis>

#include "mainwindow.h"
#include "view.h"
#include "statusbar.h"
#include "statusbar-button.h"
#include "imagewidget.h"
#include "layer-manager.h"
#include "object-data.h"
#include "object-arc.h"
#include "object-circle.h"
#include "object-dimleader.h"
#include "object-ellipse.h"
#include "object-image.h"
#include "object-line.h"
#include "object-path.h"
#include "object-point.h"
#include "object-polygon.h"
#include "object-polyline.h"
#include "object-rect.h"
#include "object-textsingle.h"
#include "embroidery.h"
#include "property-editor.h"
#include "undo-editor.h"
#include "undo-commands.h"
#include "embdetails-dialog.h"
#include "gui.h"
#include "mainwindow.h"
#include "commands.h"
#include "object-data.h"
#include "commands.h"

QtScriptEnv script_env;

int call(State *state, const char *txt)
{
    return script_env.mainWin->call(txt);
}

void set_prompt_prefix(const char *txt)
{
    script_env.mainWin->prompt->setPrefix(txt);
}

void append_prompt_history(const char *txt)
{
    script_env.mainWin->appendPromptHistory(txt);
}

void new_file(void)
{
    script_env.mainWin->newFile();
}

void open_file(void)
{
    script_env.mainWin->openFile();
}

void exit_program(void)
{
    script_env.mainWin->exit();
}

void undo_action(void)
{
    script_env.mainWin->undo();
}

void redo_action(void)
{
    script_env.mainWin->redo();
}

/* FIXME */
void repeat_action(void)
{

}

void about_dialog(void)
{
    script_env.mainWin->about();    
}

void help_dialog(void)
{
    script_env.mainWin->help();
}

void tip_of_the_day_dialog(void)
{
    script_env.mainWin->tipOfTheDay();
}

void window_cascade(void)
{
    script_env.mainWin->mdiArea->cascade();
}

void window_close(void)
{
    script_env.mainWin->onCloseWindow();
}

void window_close_all(void)
{
    script_env.mainWin->mdiArea->closeAllSubWindows();
}

void window_next(void)
{
    script_env.mainWin->mdiArea->activateNextSubWindow();
}

void window_previous(void)
{
    script_env.mainWin->mdiArea->activatePreviousSubWindow();
}

void window_tile(void)
{
    script_env.mainWin->mdiArea->tile();
}

void select_all(void)
{
    script_env.mainWin->selectAll();
}

/*
 * This action intentionally does nothing: it is present as a dummy function
 * or the "null" action.
 */
int do_nothing_cmd(State *state)
{
    return 0;
}

int save_cmd(State *state)
{
    script_env.mainWin->savefile();
    return 0;
}

int save_as_cmd(State *state)
{
    script_env.mainWin->saveasfile();
    return 0;
}

int print_cmd(State *state)
{
    script_env.mainWin->print();
    return 0;
}

int design_details_cmd(State *state)
{
    script_env.mainWin->designDetails();
    return 0;
}

int cut_cmd(State *state)
{
    script_env.mainWin->cut();
    return 0;
}

int copy_cmd(State *state)
{
    script_env.mainWin->copy();
    return 0;
}

int paste_cmd(State *state)
{
    script_env.mainWin->paste();
    return 0;
}

int changelog_cmd(State *state)
{
    script_env.mainWin->changelog();
    return 0;
}

int whats_this_cmd(State *state)
{
    script_env.mainWin->whatsThis();
    return 0;
}

int settingsdialog_cmd(State *state)
{
    script_env.mainWin->settingsDialog();
    return 0;
}

int makelayercurrent_cmd(State *state)
{
    script_env.mainWin->makeLayerActive();
    return 0;
}

int layers_cmd(State *state)
{
    script_env.mainWin->layerManager();
    return 0;
}

/* FIXME */
int layerselector_cmd(State *state)
{
    return 0;
}

int layerprevious_cmd(State *state)
{
    script_env.mainWin->layerPrevious();
    return 0;
}

/* FIXME */
int colorselector_cmd(State *state)
{
    return 0;
}

/* FIXME */
int linetypeselector_cmd(State *state)
{
    return 0;
}

/* FIXME */
int lineweightselector_cmd(State *state)
{
    return 0;
}

/* FIXME */
int hidealllayers_cmd(State *state)
{
    return 0;
}

/* FIXME */
int showalllayers_cmd(State *state)
{
    return 0;
}

/* FIXME */
int freezealllayers_cmd(State *state)
{
    return 0;
}

/* FIXME */
int thawalllayers_cmd(State *state)
{
    return 0;
}

/* FIXME */
int lockalllayers_cmd(State *state)
{
    return 0;
}

/* FIXME */
int unlockalllayers_cmd(State *state)
{
    return 0;
}

int text_bold_cmd(State *state)
{
    script_env.mainWin->textBold();
    return 0;
}

int text_italic_cmd(State *state)
{
    script_env.mainWin->textItalic();
    return 0;
}

int text_underline_cmd(State *state)
{
    script_env.mainWin->textUnderline();
    return 0;
}

int text_strikeout_cmd(State *state)
{
    script_env.mainWin->textStrikeOut();
    return 0;
}

int text_overline_cmd(State *state)
{
    script_env.mainWin->textOverline();
    return 0;
}

int zoom_dynamic_cmd(State *state)
{
    zoom_dynamic();
    return 0;
}

int zoom_scale_cmd(State *state)
{
    zoom_scale();
    return 0;
}

int zoom_center_cmd(State *state)
{
    zoom_center();
    return 0;
}

int zoom_all_cmd(State *state)
{
    zoom_all();
    return 0;
}

void repaint(void)
{
    script_env.mainWin->repaint();
}

#if 0
void
throwError(const char *message)
{
    script_env.mainWin->engine.throwError(QJSValue::GenericError, message);
}

void
throwError(QJSValue::ErrorType type, const char *message)
{
    script_env.mainWin->engine.throwError(type, message);
}

void
debug(QString message)
{
    qDebug("%s", qPrintable(message));
}

void
error(QString cmd, QString err)
{
    script_env.mainWin->setPromptPrefix("ERROR: (" + cmd + ") " + err);
    script_env.mainWin->appendPromptHistory(QString());
    end_command();
}

void
todo(QString cmd, QString msg)
{
    script_env.mainWin->alert("TODO: (" + cmd + ") " + msg);
    // Why was this here?
    end_command();
}

void
messageBox(QString type, QString title, QString text)
{
    type  = type.toLower();

    if (type != "critical" && type != "information" && type != "question" && type != "warning") {
        throwError(QJSValue::GenericError, "messageBox(): first argument must be \"critical\", \"information\", \"question\" or \"warning\".");
        return;
    }

    script_env.mainWin->messageBox(type, title, text);
}

int printArea(float x, float y, float w, float h)
{
    NAN_CHECK("printArea", "first", x)
    NAN_CHECK("printArea", "second", y)
    NAN_CHECK("printArea", "third", w)
    NAN_CHECK("printArea", "fourth", h)

    script_env.mainWin->printArea(x, y, w, h);
    return 0;
}

int setBackgroundColor(float r, float g, float b)
{
    NAN_CHECK("setBackgroundColor", "first", r)
    NAN_CHECK("setBackgroundColor", "second", g)
    NAN_CHECK("setBackgroundColor", "third", b)

    if (r < 0 || r > 255) {
        throwError(QJSValue::GenericError, "setBackgroundColor(): r value must be in range 0-255");
        return 1;
    }
    if (g < 0 || g > 255) {
        throwError(QJSValue::GenericError, "setBackgroundColor(): g value must be in range 0-255");
        return 1;
    }
    if (b < 0 || b > 255) {
        throwError(QJSValue::GenericError, "setBackgroundColor(): b value must be in range 0-255");
        return 1;
    }

    script_env.mainWin->setBackgroundColor(r, g, b);
    return 0;
}

int setCrossHairColor(float r, float g, float b)
{
    NAN_CHECK("setCrossHairColor", "first", r)
    NAN_CHECK("setCrossHairColor", "second", g)
    NAN_CHECK("setCrossHairColor", "third", b)

    if (r < 0 || r > 255) {
        throwError(QJSValue::GenericError, "setCrossHairColor(): r value must be in range 0-255");
        return 1;
    }
    if (g < 0 || g > 255) {
        throwError(QJSValue::GenericError, "setCrossHairColor(): g value must be in range 0-255");
        return 1;
    }
    if (b < 0 || b > 255) {
        throwError(QJSValue::GenericError, "setCrossHairColor(): b value must be in range 0-255");
        return 1;
    }

    script_env.mainWin->setCrossHairColor(r, g, b);
    return 0;
}

int setGridColor(float r, float g, float b)
{
    NAN_CHECK("setGridColor", "first", r)
    NAN_CHECK("setGridColor", "second", g)
    NAN_CHECK("setGridColor", "third", b)

    if (r < 0 || r > 255) {
        throwError(QJSValue::GenericError, "setGridColor(): r value must be in range 0-255");
        return 1;
    }
    if (g < 0 || g > 255) {
        throwError(QJSValue::GenericError, "setGridColor(): g value must be in range 0-255");
        return 1;
    }
    if (b < 0 || b > 255) {
        throwError(QJSValue::GenericError, "setGridColor(): b value must be in range 0-255");
        return 1;
    }

    script_env.mainWin->setGridColor(r, g, b);
    return 0;
}

int setTextSize(float num)
{
    NAN_CHECK("setTextSize", "first", num)

    script_env.mainWin->setTextSize(num);
    return 0;
}

int setTextAngle(float num)
{
    NAN_CHECK("setTextAngle", "first", num)

    script_env.mainWin->setTextAngle(num);
    return 0;
}

int previewOn(QString cloneStr, QString modeStr, float x, float y, float data)
{
    cloneStr = cloneStr.toUpper();
    modeStr = modeStr.toUpper();

    int clone = PREVIEW_CLONE_NULL;
    int mode = PREVIEW_MODE_NULL;
    if (cloneStr == "SELECTED") {
        clone = PREVIEW_CLONE_SELECTED;
    }
    else if (cloneStr == "RUBBER") {
        clone = PREVIEW_CLONE_RUBBER;
    }
    else {
        throwError(QJSValue::GenericError, "previewOn(): first argument must be \"SELECTED\" or \"RUBBER\".");
        return 1;
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
        throwError(QJSValue::GenericError, "previewOn(): second argument must be \"MOVE\", \"ROTATE\" or \"SCALE\".");
        return 1;
    }

    NAN_CHECK("previewOn", "third", x)
    NAN_CHECK("previewOn", "fourth", y)
    NAN_CHECK("previewOn", "fifth", data)

    script_env.mainWin->previewOn(clone, mode, x, y, data);
    return 0;
}

int setRubberMode(QString mode)
{
    mode = mode.toUpper();

    if (mode == "CIRCLE_1P_RAD") {
        script_env.mainWin->setRubberMode(OBJ_RUBBER_CIRCLE_1P_RAD);
    }
    else if (mode == "CIRCLE_1P_DIA")                     { script_env.mainWin->setRubberMode(OBJ_RUBBER_CIRCLE_1P_DIA); }
    else if (mode == "CIRCLE_2P")                         { script_env.mainWin->setRubberMode(OBJ_RUBBER_CIRCLE_2P); }
    else if (mode == "CIRCLE_3P")                         { script_env.mainWin->setRubberMode(OBJ_RUBBER_CIRCLE_3P); }
    else if (mode == "CIRCLE_TTR")                        { script_env.mainWin->setRubberMode(OBJ_RUBBER_CIRCLE_TTR); }
    else if (mode == "CIRCLE_TTR")                        { script_env.mainWin->setRubberMode(OBJ_RUBBER_CIRCLE_TTT); }

    else if (mode == "DIMLEADER_LINE")                    { script_env.mainWin->setRubberMode(OBJ_RUBBER_DIMLEADER_LINE); }

    else if (mode == "ELLIPSE_LINE")                      { script_env.mainWin->setRubberMode(OBJ_RUBBER_ELLIPSE_LINE); }
    else if (mode == "ELLIPSE_MAJORDIAMETER_MINORRADIUS") { script_env.mainWin->setRubberMode(OBJ_RUBBER_ELLIPSE_MAJORDIAMETER_MINORRADIUS); }
    else if (mode == "ELLIPSE_MAJORRADIUS_MINORRADIUS")   { script_env.mainWin->setRubberMode(OBJ_RUBBER_ELLIPSE_MAJORRADIUS_MINORRADIUS); }
    else if (mode == "ELLIPSE_ROTATION")                  { script_env.mainWin->setRubberMode(OBJ_RUBBER_ELLIPSE_ROTATION); }

    else if (mode == "LINE")                              { script_env.mainWin->setRubberMode(OBJ_RUBBER_LINE); }

    else if (mode == "POLYGON")                           { script_env.mainWin->setRubberMode(OBJ_RUBBER_POLYGON); }
    else if (mode == "POLYGON_INSCRIBE")                  { script_env.mainWin->setRubberMode(OBJ_RUBBER_POLYGON_INSCRIBE); }
    else if (mode == "POLYGON_CIRCUMSCRIBE")              { script_env.mainWin->setRubberMode(OBJ_RUBBER_POLYGON_CIRCUMSCRIBE); }

    else if (mode == "POLYLINE")                          { script_env.mainWin->setRubberMode(OBJ_RUBBER_POLYLINE); }

    else if (mode == "RECTANGLE")                         { script_env.mainWin->setRubberMode(OBJ_RUBBER_RECTANGLE); }

    else if (mode == "TEXTSINGLE")                        { script_env.mainWin->setRubberMode(OBJ_RUBBER_TEXTSINGLE); }

    else {
        throwError(QJSValue::GenericError, "setRubberMode(): unknown rubberMode value");
        return 1;
    }

    return 0;
}

int setRubberPoint(QString key, float x, float y)
{
    key = key.toUpper();
    NAN_CHECK("setRubberPoint", "second", x)
    NAN_CHECK("setRubberPoint", "third", y)

    script_env.mainWin->setRubberPoint(key, x, y);
    return 0;
}

int setRubberText(QString key, QString txt)
{
    key = key.toUpper();

    script_env.mainWin->setRubberText(key, txt);
    return 0;
}

int addRubber(QString objType)
{
    objType = objType.toUpper();

    if (!mainwin->allowRubber()) {
        throwError(QJSValue::GenericError, "addRubber(): You must use vulcanize() before you can add another rubber object.");
        return 1;
    }

    float mx = script_env.mainWin->mouseX();
    float my = script_env.mainWin->mouseY();

    if     (objType == "ARC")          {} //TODO: handle this type
    else if (objType == "BLOCK")        {} //TODO: handle this type
    else if (objType == "CIRCLE")       { script_env.mainWin->addCircle(mx, my, 0, false, OBJ_RUBBER_ON); }
    else if (objType == "DIMALIGNED")   {} //TODO: handle this type
    else if (objType == "DIMANGULAR")   {} //TODO: handle this type
    else if (objType == "DIMARCLENGTH") {} //TODO: handle this type
    else if (objType == "DIMDIAMETER")  {} //TODO: handle this type
    else if (objType == "DIMLEADER")    { script_env.mainWin->addDimLeader(mx, my, mx, my, 0, OBJ_RUBBER_ON); }
    else if (objType == "DIMLINEAR")    {} //TODO: handle this type
    else if (objType == "DIMORDINATE")  {} //TODO: handle this type
    else if (objType == "DIMRADIUS")    {} //TODO: handle this type
    else if (objType == "ELLIPSE")      { script_env.mainWin->addEllipse(mx, my, 0, 0, 0, 0, OBJ_RUBBER_ON); }
    else if (objType == "ELLIPSEARC")   {} //TODO: handle this type
    else if (objType == "HATCH")        {} //TODO: handle this type
    else if (objType == "IMAGE")        {} //TODO: handle this type
    else if (objType == "INFINITELINE") {} //TODO: handle this type
    else if (objType == "LINE")         { script_env.mainWin->addLine(mx, my, mx, my, 0, OBJ_RUBBER_ON); }
    else if (objType == "PATH")         {} //TODO: handle this type
    else if (objType == "POINT")        {} //TODO: handle this type
    else if (objType == "POLYGON")      { script_env.mainWin->addPolygon(mx, my, QPainterPath(), OBJ_RUBBER_ON); }
    else if (objType == "POLYLINE")     { script_env.mainWin->addPolyline(mx, my, QPainterPath(), OBJ_RUBBER_ON); }
    else if (objType == "RAY")          {} //TODO: handle this type
    else if (objType == "RECTANGLE")    { script_env.mainWin->addRectangle(mx, my, mx, my, 0, 0, OBJ_RUBBER_ON); }
    else if (objType == "SPLINE")       {} //TODO: handle this type
    else if (objType == "TEXTMULTI")    {} //TODO: handle this type
    else if (objType == "TEXTSINGLE")   { script_env.mainWin->addTextSingle("", mx, my, 0, false, OBJ_RUBBER_ON); }

    return 0;
}

int spareRubber(QString objID)
{
    objID = objID.toUpper();

    if (objID == "PATH")     { script_env.mainWin->spareRubber(SPARE_RUBBER_PATH);     }
    else if (objID == "POLYGON")  { script_env.mainWin->spareRubber(SPARE_RUBBER_POLYGON);  }
    else if (objID == "POLYLINE") { script_env.mainWin->spareRubber(SPARE_RUBBER_POLYLINE); }
    else {
        bool ok = false;
        qint64 id = objID.toLongLong(&ok);
        if (!ok) {
            throwError(QJSValue::TypeError, "spareRubber(): error converting object ID into an int64");
            return 1;
        }
        script_env.mainWin->spareRubber(id);
    }

    return 0;
}

int addTextMulti(QString str, float x, float y, float rot, bool fill)
{
    NAN_CHECK("addTextMulti", "second", x)
    NAN_CHECK("addTextMulti", "third", y)
    NAN_CHECK("addTextMulti", "fourth", rot)

    script_env.mainWin->addTextMulti(str, x, y, rot, fill, OBJ_RUBBER_OFF);
    return 0;
}

int addTextSingle(QString str, float x, float y, float rot, bool fill)
{
    NAN_CHECK("addTextSingle", "second", x)
    NAN_CHECK("addTextSingle", "third", y)
    NAN_CHECK("addTextSingle", "fourth", rot)

    script_env.mainWin->addTextSingle(str, x, y, rot, fill, OBJ_RUBBER_OFF);
    return 0;
}

int addInfiniteLine(void)
{
    //TODO: parameter error checking
    qDebug("TODO: finish addInfiniteLine command");
    return 0;
}

int addRay(void)
{
    //TODO: parameter error checking
    qDebug("TODO: finish addRay command");
    return 0;
}

int addLine(float x1, float y1, float x2, float y2, float rot)
{
    NAN_CHECK("addLine", "first", x1)
    NAN_CHECK("addLine", "second", y1)
    NAN_CHECK("addLine", "third", x2)
    NAN_CHECK("addLine", "fourth", y2)
    NAN_CHECK("addLine", "fifth", rot)

    script_env.mainWin->addLine(x1, y1, x2, y2, rot, OBJ_RUBBER_OFF);
    return 0;
}

int addTriangle(float x1, float y1, float x2, float y2, float x3, float y3, float rot, bool fill)
{
    NAN_CHECK("addTriangle", "first", x1)
    NAN_CHECK("addTriangle", "second", y1)
    NAN_CHECK("addTriangle", "third", x2)
    NAN_CHECK("addTriangle", "fourth", y2)
    NAN_CHECK("addTriangle", "fifth", x3)
    NAN_CHECK("addTriangle", "sixth", y3)
    NAN_CHECK("addTriangle", "seventh", rot)

    script_env.mainWin->addTriangle(x1, y1, x2, y2, x3, y3, rot, fill);
    return 0;
}

int addRectangle(float x, float y, float w, float h, float rot, bool fill)
{
    NAN_CHECK("addRectangle", "first", x)
    NAN_CHECK("addRectangle", "second", y)
    NAN_CHECK("addRectangle", "third", w)
    NAN_CHECK("addRectangle", "fourth", h)
    NAN_CHECK("addRectangle", "fifth", rot)

    script_env.mainWin->addRectangle(x, y, w, h, rot, fill, OBJ_RUBBER_OFF);
    return 0;
}

int addRoundedRectangle(float x, float y, float w, float h, float rad, float rot, bool fill)
{
    NAN_CHECK("addRoundedRectangle", "first", x)
    NAN_CHECK("addRoundedRectangle", "second", y)
    NAN_CHECK("addRoundedRectangle", "third", w)
    NAN_CHECK("addRoundedRectangle", "fourth", h)
    NAN_CHECK("addRoundedRectangle", "fifth", rad)
    NAN_CHECK("addRoundedRectangle", "sixth", rot)

    script_env.mainWin->addRoundedRectangle(x, y, w, h, rad, rot, fill);
    return 0;
}

int addArc(float startX, float startY, float midX, float midY, float endX, float endY)
{
    NAN_CHECK("addArc", "first", startX)
    NAN_CHECK("addArc", "second", startY)
    NAN_CHECK("addArc", "third", midX)
    NAN_CHECK("addArc", "fourth", midY)
    NAN_CHECK("addArc", "fifth", endX)
    NAN_CHECK("addArc", "sixth", endY)

    script_env.mainWin->addArc(startX, startY, midX, midY, endX, endY, OBJ_RUBBER_OFF);
    return 0;
}

int addCircle(float centerX, float centerY, float radius, bool fill)
{
    NAN_CHECK("addCircle", "first", centerX)
    NAN_CHECK("addCircle", "second", centerY)
    NAN_CHECK("addCircle", "third", radius)

    script_env.mainWin->addCircle(centerX, centerY, radius, fill, OBJ_RUBBER_OFF);
    return 0;
}

int addSlot(float centerX, float centerY, float diameter, float length, float rot, bool fill)
{
    NAN_CHECK("addSlot", "first", centerX)
    NAN_CHECK("addSlot", "second", centerY)
    NAN_CHECK("addSlot", "third", diameter)
    NAN_CHECK("addSlot", "fourth", length)
    NAN_CHECK("addSlot", "fifth", rot)

    script_env.mainWin->addSlot(centerX, centerY, diameter, length, rot, fill, OBJ_RUBBER_OFF);
    return 0;
}

int addEllipse(float centerX, float centerY, float radX, float radY, float rot, bool fill)
{
    NAN_CHECK("addSlot", "first", centerX)
    NAN_CHECK("addSlot", "second", centerY)
    NAN_CHECK("addSlot", "third", radX)
    NAN_CHECK("addSlot", "fourth", radY)
    NAN_CHECK("addSlot", "fifth", rot)

    script_env.mainWin->addEllipse(centerX, centerY, radX, radY, rot, fill, OBJ_RUBBER_OFF);
    return 0;
}

int addPoint(float x, float y)
{
    NAN_CHECK("addPoint", "first", x)
    NAN_CHECK("addPoint", "second", y)

    script_env.mainWin->addPoint(x,y);
    return 0;
}

int addRegularPolygon(QJSValueList args)
{
    //TODO: parameter error checking
    qDebug("TODO: finish addRegularPolygon command");
    return 0;
}

int addPolygon(QJSValueList args)
{
    if (args.count() != 1) {
        throwError("addPolygon() requires one argument");
        return 1;
    }
    if (!args.at(0).isArray()) {
        throwError(QJSValue::TypeError, "addPolygon(): first argument is not an array");
        return 1;
    }

    QVariantList varList = args.at(0).toVariant().toList();
    int varSize = varList.size();
    if (varSize < 2) {
        throwError(QJSValue::TypeError, "addPolygon(): array must contain at least two elements");
        return 1;
    }
    if (varSize % 2) {
        throwError(QJSValue::TypeError, "addPolygon(): array cannot contain an odd number of elements");
        return 1;
    }

    bool lineTo = false;
    bool xCoord = true;
    float x = 0;
    float y = 0;
    float startX = 0;
    float startY = 0;
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
        else {
            throwError(QJSValue::TypeError, "addPolygon(): array contains one or more invalid elements");
            return 1;
        }
    }

    //Close the polygon
    path.closeSubpath();

    path.translate(-startX, -startY);

    script_env.mainWin->addPolygon(startX, startY, path, OBJ_RUBBER_OFF);
    return 0;
}

int addPolyline(QJSValueList args)
{
    if (args.count() != 1) {
        throwError("addPolyline() requires one argument");
        return 1;
    }
    if (!args.at(0).isArray()) {
        throwError(QJSValue::TypeError, "addPolyline(): first argument is not an array");
        return 1;
    }

    QVariantList varList = args.at(0).toVariant().toList();
    int varSize = varList.size();
    if (varSize < 2) {
        throwError(QJSValue::TypeError, "addPolyline(): array must contain at least two elements");
        return 1;
    }
    if (varSize % 2) {
        throwError(QJSValue::TypeError, "addPolyline(): array cannot contain an odd number of elements");
        return 1;
    }

    bool lineTo = false;
    bool xCoord = true;
    float x = 0;
    float y = 0;
    float startX = 0;
    float startY = 0;
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
        else {
            throwError(QJSValue::TypeError, "addPolyline(): array contains one or more invalid elements");
            return 1;
        }
    }

    path.translate(-startX, -startY);

    script_env.mainWin->addPolyline(startX, startY, path, OBJ_RUBBER_OFF);
    return 0;
}

int addPath(void)
{
    //TODO: parameter error checking
    qDebug("TODO: finish addPath command");
    return 0;
}

int addHorizontalDimension(void)
{
    //TODO: parameter error checking
    qDebug("TODO: finish addHorizontalDimension command");
    return 0;
}

int addVerticalDimension(void)
{
    //TODO: parameter error checking
    qDebug("TODO: finish addVerticalDimension command");
    return 0;
}

int addImage(void)
{
    //TODO: parameter error checking
    qDebug("TODO: finish addImage command");
    return 0;
}

int addDimLeader(float x1, float y1, float x2, float y2, float rot)
{
    NAN_CHECK("addDimLeader", "first", x1)
    NAN_CHECK("addDimLeader", "second", y1)
    NAN_CHECK("addDimLeader", "third", x2)
    NAN_CHECK("addDimLeader", "fourth", y2)
    NAN_CHECK("addDimLeader", "fifth", rot)

    script_env.mainWin->addDimLeader(x1, y1, x2, y2, rot, OBJ_RUBBER_OFF);
    return 0;
}

int addToSelection(void)
{
    //TODO: finish
    return 0;
}

int cutSelected(float x, float y)
{
    NAN_CHECK("cutSelected", "first", x)
    NAN_CHECK("cutSelected", "second", y)

    script_env.mainWin->cutSelected(x, y);
    return 0;
}

int copySelected(float x, float y)
{
    NAN_CHECK("copySelected", "first", x)
    NAN_CHECK("copySelected", "second", y)

    script_env.mainWin->copySelected(x, y);
    return 0;
}

int pasteSelected(float x, float y)
{
    NAN_CHECK("pasteSelected", "first", x)
    NAN_CHECK("pasteSelected", "second", y)

    script_env.mainWin->pasteSelected(x, y);
    return 0;
}

int rotate_selected(float x, float y, float rot)
{
    NAN_CHECK("mirrorSelected", "first", x)
    NAN_CHECK("mirrorSelected", "second", y)
    NAN_CHECK("mirrorSelected", "third", rot)

    script_env.mainWin->rotateSelected(x, y, rot);
    return 0;
}

int mirrorSelected(float x1, float y1, float x2, float y2)
{
    NAN_CHECK("mirrorSelected", "first", x1)
    NAN_CHECK("mirrorSelected", "second", y1)
    NAN_CHECK("mirrorSelected", "third", x2)
    NAN_CHECK("mirrorSelected", "fourth", y2)

    mirror_selected(x1, y1, x2, y2);
    return 0;
}
#endif

/* Call a command from the command table using the name of the command. */
int
MainWindow::call(QString name)
{
    for (int i=0; command_table[i].label[0] != '^'; i++) {
        if (QString(command_table[i].label) == name) {
            qDebug("> %s", qPrintable(name));
            return command_table[i].command(&state);
        }
    }
    qDebug("[ERROR]: Unknown command %s.", qPrintable(name));
    return -1;
}

/* Wrapper for the in-built Qt debug for the commands to use. */
void MainWindow::debug(QString txt)
{
    qDebug("%s", qPrintable(txt));
}

void stub_implement(QString txt)
{
    qDebug("TODO: %s", qPrintable(txt));
}

void MainWindow::stub_testing()
{
    QMessageBox::warning(this, tr("Testing Feature"), tr("<b>This feature is in testing.</b>"));
}

void MainWindow::exit()
{
    qDebug("exit()");
    if (state.settings.prompt_save_history) {
        /* TODO: get filename from settings */
        prompt->saveHistory("prompt.log", state.settings.prompt_save_history_as_html);
    }
    qApp->closeAllWindows();
    this->deleteLater(); //Force the MainWindow destructor to run before exiting. Makes Valgrind "still reachable" happy :)
}

void MainWindow::quit()
{
    qDebug("quit()");
    exit();
}

void MainWindow::checkForUpdates()
{
    qDebug("checkForUpdates()");
    //TODO: Check website for new versions, commands, etc...
}

void MainWindow::cut()
{
    qDebug("cut()");
    View* gview = activeView();
    if (gview) { gview->cut(); }
}

void MainWindow::copy()
{
    qDebug("copy()");
    View* gview = activeView();
    if (gview) { gview->copy(); }
}

void MainWindow::paste()
{
    qDebug("paste()");
    View* gview = activeView();
    if (gview) { gview->paste(); }
}

void MainWindow::selectAll()
{
    qDebug("selectAll()");
    View* gview = activeView();
    if (gview) { gview->selectAll(); }
}

QString MainWindow::platformString()
{
    //TODO: Append QSysInfo to string where applicable.
    QString os;
    #if   defined(Q_OS_AIX)
    os = "AIX";
    #elif defined(Q_OS_BSD4)
    os = "BSD 4.4";
    #elif defined(Q_OS_BSDI)
    os = "BSD/OS";
    #elif defined(Q_OS_CYGWIN)
    os = "Cygwin";
    #elif defined(Q_OS_DARWIN)
    os = "Mac OS";
    #elif defined(Q_OS_DGUX)
    os = "DG/UX";
    #elif defined(Q_OS_DYNIX)
    os = "DYNIX/ptx";
    #elif defined(Q_OS_FREEBSD)
    os = "FreeBSD";
    #elif defined(Q_OS_HPUX)
    os = "HP-UX";
    #elif defined(Q_OS_HURD)
    os = "GNU Hurd";
    #elif defined(Q_OS_IRIX)
    os = "SGI Irix";
    #elif defined(Q_OS_LINUX)
    os = "Linux";
    #elif defined(Q_OS_LYNX)
    os = "LynxOS";
    #elif defined(Q_OS_MAC)
    os = "Mac OS";
    #elif defined(Q_OS_MSDOS)
    os = "MS-DOS";
    #elif defined(Q_OS_NETBSD)
    os = "NetBSD";
    #elif defined(Q_OS_OS2)
    os = "OS/2";
    #elif defined(Q_OS_OPENBSD)
    os = "OpenBSD";
    #elif defined(Q_OS_OS2EMX)
    os = "XFree86 on OS/2";
    #elif defined(Q_OS_OSF)
    os = "HP Tru64 UNIX";
    #elif defined(Q_OS_QNX)
    os = "QNX Neutrino";
    #elif defined(Q_OS_RELIANT)
    os = "Reliant UNIX";
    #elif defined(Q_OS_SCO)
    os = "SCO OpenServer 5";
    #elif defined(Q_OS_SOLARIS)
    os = "Sun Solaris";
    #elif defined(Q_OS_SYMBIAN)
    os = "Symbian";
    #elif defined(Q_OS_ULTRIX)
    os = "DEC Ultrix";
    #elif defined(Q_OS_UNIX)
    os = "UNIX BSD/SYSV";
    #elif defined(Q_OS_UNIXWARE)
    os = "UnixWare";
    #elif defined(Q_OS_WIN32)
    os = "Windows";
    #elif defined(Q_OS_WINCE)
    os = "Windows CE";
    #endif
    qDebug("Platform: %s", qPrintable(os));
    return os;
}

void MainWindow::designDetails()
{
    QGraphicsScene* scene = activeScene();
    if (scene)
    {
        EmbDetailsDialog dialog(scene, this);
        dialog.exec();
    }
}

void MainWindow::about()
{
    //TODO: QTabWidget for about dialog
    QApplication::setOverrideCursor(Qt::ArrowCursor);
    qDebug("about()");
    QString appDir = qApp->applicationDirPath();
    QString appName = QApplication::applicationName();
    QString title = "About " + appName;

    QDialog dialog(this);
    ImageWidget img(appDir + "/images/logo-small");
    QLabel text(appName + tr("\n\n") +
                          tr("http://embroidermodder.github.io") +
                          tr("\n\n") +
                          tr("Available Platforms: GNU/Linux, Windows, Mac OSX, Raspberry Pi") +
                          tr("\n\n") +
                          tr("Embroidery formats by Josh Varga.") +
                          tr("\n") +
                          tr("User Interface by Jonathan Greig.") +
                          tr("\n\n") +
                          tr("Free under the zlib/libpng license.")
                          #if defined(BUILD_GIT_HASH)
                          + tr("\n\n") +
                          tr("Build Hash: ") + qPrintable(BUILD_GIT_HASH)
                          #endif
                          );
    text.setWordWrap(true);

    QDialogButtonBox buttonbox(Qt::Horizontal, &dialog);
    QPushButton button(&dialog);
    button.setText("Oh, Yeah!");
    buttonbox.addButton(&button, QDialogButtonBox::AcceptRole);
    buttonbox.setCenterButtons(true);
    connect(&buttonbox, SIGNAL(accepted()), &dialog, SLOT(accept()));

    QVBoxLayout layout;
    layout.setAlignment(Qt::AlignCenter);
    layout.addWidget(&img);
    layout.addWidget(&text);
    layout.addWidget(&buttonbox);

    dialog.setWindowTitle(title);
    dialog.setMinimumWidth(img.minimumWidth()+30);
    dialog.setMinimumHeight(img.minimumHeight()+50);
    dialog.setLayout(&layout);
    dialog.exec();
    QApplication::restoreOverrideCursor();
}

void MainWindow::whatsThisContextHelp()
{
    qDebug("whatsThisContextHelp()");
    QWhatsThis::enterWhatsThisMode();
}

void MainWindow::print()
{
    qDebug("print()");
    MdiWindow* mdiWin = qobject_cast<MdiWindow*>(mdiArea->activeSubWindow());
    if (mdiWin) { mdiWin->print(); }
}

void MainWindow::tipOfTheDay()
{
    qDebug("tipOfTheDay()");

    QString appDir = qApp->applicationDirPath();

    wizardTipOfTheDay = new QWizard(this);
    wizardTipOfTheDay->setAttribute(Qt::WA_DeleteOnClose);
    wizardTipOfTheDay->setWizardStyle(QWizard::ModernStyle);
    wizardTipOfTheDay->setMinimumSize(550, 400);

    QWizardPage* page = new QWizardPage(wizardTipOfTheDay);

    ImageWidget* imgBanner = new ImageWidget(appDir + "/images/did-you-know.png", wizardTipOfTheDay);

    /* FIXME */
    int n_tips;
    for (n_tips=0; tips[n_tips][0] != '^'; n_tips++) {}
    if (state.settings.general_current_tip >= n_tips) {
        state.settings.general_current_tip = 0;
    }
    labelTipOfTheDay = new QLabel(tips[state.settings.general_current_tip], wizardTipOfTheDay);
    labelTipOfTheDay->setWordWrap(true);

    QCheckBox* checkBoxTipOfTheDay = new QCheckBox(tr("&Show tips on startup"), wizardTipOfTheDay);
    state.settings.general_tip_of_the_day = state.settings.general_tip_of_the_day;
    checkBoxTipOfTheDay->setChecked(state.settings.general_tip_of_the_day);
    connect(checkBoxTipOfTheDay, SIGNAL(stateChanged(int)), this, SLOT(checkBoxTipOfTheDayStateChanged(int)));

    QVBoxLayout* layout = new QVBoxLayout(wizardTipOfTheDay);
    layout->addWidget(imgBanner);
    layout->addStrut(1);
    layout->addWidget(labelTipOfTheDay);
    layout->addStretch(1);
    layout->addWidget(checkBoxTipOfTheDay);
    page->setLayout(layout);
    wizardTipOfTheDay->addPage(page);

    wizardTipOfTheDay->setWindowTitle("Tip of the Day");

    //TODO: Add icons to buttons by using wizardTipOfTheDay->setButton(QWizard::CustomButton1, buttonPrevious)
    //TODO: Add icons to buttons by using wizardTipOfTheDay->setButton(QWizard::CustomButton1, buttonNext)
    //TODO: Add icons to buttons by using wizardTipOfTheDay->setButton(QWizard::CustomButton1, buttonClose)
    wizardTipOfTheDay->setButtonText(QWizard::CustomButton1, tr("&Previous"));
    wizardTipOfTheDay->setButtonText(QWizard::CustomButton2, tr("&Next"));
    wizardTipOfTheDay->setButtonText(QWizard::CustomButton3, tr("&Close"));
    wizardTipOfTheDay->setOption(QWizard::HaveCustomButton1, true);
    wizardTipOfTheDay->setOption(QWizard::HaveCustomButton2, true);
    wizardTipOfTheDay->setOption(QWizard::HaveCustomButton3, true);
    connect(wizardTipOfTheDay, SIGNAL(customButtonClicked(int)), this, SLOT(buttonTipOfTheDayClicked(int)));

    QList<QWizard::WizardButton> listTipOfTheDayButtons;
    listTipOfTheDayButtons << QWizard::Stretch << QWizard::CustomButton1 << QWizard::CustomButton2 << QWizard::CustomButton3;
    wizardTipOfTheDay->setButtonLayout(listTipOfTheDayButtons);

    wizardTipOfTheDay->exec();
}

void MainWindow::checkBoxTipOfTheDayStateChanged(int checked)
{
    state.settings.general_tip_of_the_day = checked;
}

void MainWindow::buttonTipOfTheDayClicked(int button)
{
    qDebug("buttonTipOfTheDayClicked(%d)", button);
    /* FIXME */
    int n_tips;
    for (n_tips=0; tips[n_tips][0] != '^'; n_tips++) {}
    if (button == QWizard::CustomButton1) {
        if (state.settings.general_current_tip > 0) {
            state.settings.general_current_tip--;
        }
        else {
            state.settings.general_current_tip = n_tips - 1;
        }
        labelTipOfTheDay->setText(tips[state.settings.general_current_tip]);
    }
    else if (button == QWizard::CustomButton2) {
        state.settings.general_current_tip++;
        if (state.settings.general_current_tip >= n_tips) {
            state.settings.general_current_tip = 0;
        }
        labelTipOfTheDay->setText(tips[state.settings.general_current_tip]);
    }
    else if (button == QWizard::CustomButton3) {
        wizardTipOfTheDay->close();
    }
}

void MainWindow::help()
{
    qDebug("help()");

    // Open the HTML Help in the default browser
    QUrl helpURL("file:///" + qApp->applicationDirPath() + "/help/doc-index.html");
    QDesktopServices::openUrl(helpURL);

    //TODO: This is how to start an external program. Use this elsewhere...
    //QString program = "firefox";
    //QStringList arguments;
    //arguments << "help/commands.html";
    //QProcess *myProcess = new QProcess(this);
    //myProcess->start(program, arguments);
}

void MainWindow::changelog()
{
    qDebug("changelog()");

    QUrl changelogURL("help/changelog.html");
    QDesktopServices::openUrl(changelogURL);
}

// Standard Slots
void MainWindow::undo()
{
    qDebug("undo()");
    QString prefix = state.prefix->data;
    if (dockUndoEdit->canUndo()) {
        prompt->setPrefix("Undo " + dockUndoEdit->undoText());
        prompt->appendHistory(QString());
        dockUndoEdit->undo();
        prompt->setPrefix(prefix);
    }
    else {
        prompt->alert("Nothing to undo");
        prompt->setPrefix(prefix);
    }
}

void MainWindow::redo()
{
    qDebug("redo()");
    QString prefix = state.prefix->data;
    if (dockUndoEdit->canRedo()) {
        prompt->setPrefix("Redo " + dockUndoEdit->redoText());
        prompt->appendHistory(QString());
        dockUndoEdit->redo();
        prompt->setPrefix(prefix);
    }
    else {
        prompt->alert("Nothing to redo");
        prompt->setPrefix(prefix);
    }
}

void MainWindow::setShiftPressed()
{
    state.shift = true;
}

void MainWindow::setShiftReleased()
{
    state.shift = false;
}

// Icons
void MainWindow::iconResize(int iconSize)
{
    this->setIconSize(QSize(iconSize, iconSize));
    layerSelector->     setIconSize(QSize(iconSize*4, iconSize));
    colorSelector->     setIconSize(QSize(iconSize,   iconSize));
    linetypeSelector->  setIconSize(QSize(iconSize*4, iconSize));
    lineweightSelector->setIconSize(QSize(iconSize*4, iconSize));
    //set the minimum combobox width so the text is always readable
    layerSelector->     setMinimumWidth(iconSize*4);
    colorSelector->     setMinimumWidth(iconSize*2);
    linetypeSelector->  setMinimumWidth(iconSize*4);
    lineweightSelector->setMinimumWidth(iconSize*4);

    //TODO: low-priority: open app with iconSize set to 128. resize the icons to a smaller size.

    state.settings.general_icon_size = iconSize;
}

void icon_resize(int size)
{
    qDebug("icon_resize(%d)", size);
    script_env.mainWin->iconResize(size);
}

MdiWindow* MainWindow::activeMdiWindow()
{
    qDebug("activeMdiWindow()");
    MdiWindow* mdiWin = qobject_cast<MdiWindow*>(mdiArea->activeSubWindow());
    return mdiWin;
}

View*
activeView(void)
{
    qDebug("activeView()");
    MdiWindow* mdiWin = qobject_cast<MdiWindow*>(script_env.mainWin->mdiArea->activeSubWindow());
    if (mdiWin) {
        View* v = mdiWin->getView();
        return v;
    }
    return 0;
}

QGraphicsScene* activeScene(void)
{
    qDebug("activeScene()");
    MdiWindow* mdiWin = qobject_cast<MdiWindow*>(
        script_env.mainWin->mdiArea->activeSubWindow());
    if (mdiWin) {
        QGraphicsScene* s = mdiWin->getScene();
        return s;
    }
    return 0;
}

QUndoStack* MainWindow::activeUndoStack()
{
    qDebug("activeUndoStack()");
    View* v = activeView();
    if (v)
    {
        QUndoStack* u = v->getUndoStack();
        return u;
    }
    return 0;
}

void MainWindow::setUndoCleanIcon(bool opened)
{
    dockUndoEdit->updateCleanIcon(opened);
}

void MainWindow::updateAllViewScrollBars(bool val)
{
    QList<QMdiSubWindow*> windowList = mdiArea->subWindowList();
    for (int i = 0; i < windowList.count(); ++i)
    {
        MdiWindow* mdiWin = qobject_cast<MdiWindow*>(windowList.at(i));
        if (mdiWin) { mdiWin->showViewScrollBars(val); }
    }
}

void MainWindow::updateAllViewCrossHairColors(QRgb color)
{
    QList<QMdiSubWindow*> windowList = mdiArea->subWindowList();
    for (int i = 0; i < windowList.count(); ++i)
    {
        MdiWindow* mdiWin = qobject_cast<MdiWindow*>(windowList.at(i));
        if (mdiWin) { mdiWin->setViewCrossHairColor(color); }
    }
}

void MainWindow::updateAllViewBackgroundColors(QRgb color)
{
    QList<QMdiSubWindow*> windowList = mdiArea->subWindowList();
    for (int i = 0; i < windowList.count(); ++i)
    {
        MdiWindow* mdiWin = qobject_cast<MdiWindow*>(windowList.at(i));
        if (mdiWin) { mdiWin->setViewBackgroundColor(color); }
    }
}

void MainWindow::updateAllViewSelectBoxColors(QRgb colorL, QRgb fillL, QRgb colorR, QRgb fillR, int alpha)
{
    QList<QMdiSubWindow*> windowList = mdiArea->subWindowList();
    for (int i = 0; i < windowList.count(); ++i)
    {
        MdiWindow* mdiWin = qobject_cast<MdiWindow*>(windowList.at(i));
        if (mdiWin) { mdiWin->setViewSelectBoxColors(colorL, fillL, colorR, fillR, alpha); }
    }
}

void MainWindow::updateAllViewGridColors(QRgb color)
{
    QList<QMdiSubWindow*> windowList = mdiArea->subWindowList();
    for (int i = 0; i < windowList.count(); ++i)
    {
        MdiWindow* mdiWin = qobject_cast<MdiWindow*>(windowList.at(i));
        if (mdiWin) { mdiWin->setViewGridColor(color); }
    }
}

void MainWindow::updateAllViewRulerColors(QRgb color)
{
    QList<QMdiSubWindow*> windowList = mdiArea->subWindowList();
    for (int i = 0; i < windowList.count(); ++i)
    {
        MdiWindow* mdiWin = qobject_cast<MdiWindow*>(windowList.at(i));
        if (mdiWin) { mdiWin->setViewRulerColor(color); }
    }
}

void MainWindow::updatePickAddMode(bool val)
{
    state.settings.selection_mode_pickadd = val;
    dockPropEdit->updatePickAddModeButton(val);
}

void MainWindow::pickAddModeToggled()
{
    bool val = !state.settings.selection_mode_pickadd;
    updatePickAddMode(val);
}

// Layer ToolBar
void MainWindow::makeLayerActive()
{
    qDebug("makeLayerActive()");
    stub_implement("Implement makeLayerActive.");
}

void MainWindow::layerManager()
{
    qDebug("layerManager()");
    stub_implement("Implement layerManager.");
    LayerManager layman(this, this);
    layman.exec();
}

void MainWindow::layerPrevious()
{
    qDebug("layerPrevious()");
    stub_implement("Implement layerPrevious.");
}

// Zoom ToolBar
void zoom_real_time(void)
{
    qDebug("zoomRealtime()");
    stub_implement("Implement zoomRealtime.");
}

void zoom_previous(void)
{
    qDebug("zoomPrevious()");
    stub_implement("Implement zoomPrevious.");
}

void zoom_window(void)
{
    qDebug("zoomWindow()");
    View* gview = activeView();
    if (gview) {
    	gview->zoomWindow();
    }
}

void zoom_dynamic(void)
{
    qDebug("zoomDynamic()");
    stub_implement("Implement zoomDynamic.");
}

void zoom_scale(void)
{
    qDebug("zoomScale()");
    stub_implement("Implement zoomScale.");
}

void zoom_center(void)
{
    qDebug("zoomCenter()");
    stub_implement("Implement zoomCenter.");
}

void zoom_in(void)
{
    qDebug("zoomIn()");
    View* gview = activeView();
    if (gview) {
        gview->zoomIn();
    }
}

void zoom_out(void)
{
    qDebug("zoomOut()");
    View* gview = activeView();
    if (gview) {
        gview->zoomOut();
    }
}

void zoom_selected(void)
{
    qDebug("zoomSelected()");
    View* gview = activeView();
    QUndoStack* stack = gview->getUndoStack();
    if (gview && stack) {
        UndoableNavCommand* cmd = new UndoableNavCommand("ZoomSelected", gview, 0);
        stack->push(cmd);
    }
}

void zoom_all(void)
{
    qDebug("zoomAll()");
    stub_implement("Implement zoomAll.");
}

void zoom_extents(void)
{
    qDebug("zoomExtents()");
    View* gview = activeView();
    QUndoStack* stack = gview->getUndoStack();
    if (gview && stack) {
        UndoableNavCommand* cmd = new UndoableNavCommand("ZoomExtents", gview, 0);
        stack->push(cmd);
    }
}

// Pan SubMenu
void pan_real_time(void)
{
    qDebug("panrealtime()");
    View* gview = activeView();
    if (gview) {
        gview->panRealTime();
    }
}

void pan_point(void)
{
    qDebug("panpoint()");
    View* gview = activeView();
    if (gview) {
        gview->panPoint();
    }
}

void pan_left(void)
{
    qDebug("panLeft()");
    View* gview = activeView();
    QUndoStack* stack = gview->getUndoStack();
    if (gview && stack) {
        UndoableNavCommand* cmd = new UndoableNavCommand("PanLeft", gview, 0);
        stack->push(cmd);
    }
}

void pan_right(void)
{
    qDebug("panRight()");
    View* gview = activeView();
    QUndoStack* stack = gview->getUndoStack();
    if (gview && stack) {
        UndoableNavCommand* cmd = new UndoableNavCommand("PanRight", gview, 0);
        stack->push(cmd);
    }
}

void pan_up(void)
{
    qDebug("panUp()");
    View* gview = activeView();
    QUndoStack* stack = gview->getUndoStack();
    if (gview && stack) {
        UndoableNavCommand* cmd = new UndoableNavCommand("PanUp", gview, 0);
        stack->push(cmd);
    }
}

void pan_down(void)
{
    qDebug("panDown()");
    View* gview = activeView();
    QUndoStack* stack = gview->getUndoStack();
    if (gview && stack) {
        UndoableNavCommand* cmd = new UndoableNavCommand("PanDown", gview, 0);
        stack->push(cmd);
    }
}

/* TODO: Make day vision color settings. */
void day_vision(void)
{
    View* gview = activeView();
    if (gview) {
        gview->setBackgroundColor(qRgb(255,255,255));
        gview->setCrossHairColor(qRgb(0,0,0));
        gview->setGridColor(qRgb(0,0,0));
    }
}

/* TODO: Make night vision color settings. */
void night_vision(void)
{
    View* gview = activeView();
    if (gview) {
        gview->setBackgroundColor(qRgb(0,0,0));
        gview->setCrossHairColor(qRgb(255,255,255));
        gview->setGridColor(qRgb(255,255,255));
    }
}

void MainWindow::layerSelectorIndexChanged(int index)
{
    qDebug("layerSelectorIndexChanged(%d)", index);
}

void MainWindow::colorSelectorIndexChanged(int index)
{
    qDebug("colorSelectorIndexChanged(%d)", index);

    QComboBox* comboBox = qobject_cast<QComboBox*>(sender());
    QRgb newColor;
    if (comboBox) {
        bool ok = 0;
        //TODO: Handle ByLayer and ByBlock and Other...
        newColor = comboBox->itemData(index).toUInt(&ok);
        if (!ok)
            QMessageBox::warning(this, tr("Color Selector Conversion Error"), tr("<b>An error has occured while changing colors.</b>"));
    }
    else {
        QMessageBox::warning(this, tr("Color Selector Pointer Error"), tr("<b>An error has occured while changing colors.</b>"));
    }

    MdiWindow* mdiWin = qobject_cast<MdiWindow*>(mdiArea->activeSubWindow());
    if (mdiWin) { mdiWin->currentColorChanged(newColor); }
}

void MainWindow::linetypeSelectorIndexChanged(int index)
{
    qDebug("linetypeSelectorIndexChanged(%d)", index);
}

void MainWindow::lineweightSelectorIndexChanged(int index)
{
    qDebug("lineweightSelectorIndexChanged(%d)", index);
}

void MainWindow::textFontSelectorCurrentFontChanged(const QFont& font)
{
    qDebug("textFontSelectorCurrentFontChanged()");
    setTextFont(font.family());
}

void MainWindow::textSizeSelectorIndexChanged(int index)
{
    qDebug("textSizeSelectorIndexChanged(%d)", index);
    /* TODO: check that the toReal() conversion is ok */
    state.settings.text_size = qFabs(textSizeSelector->itemData(index).toReal());
}

QString MainWindow::textFont()
{
    return state.settings.text_font->data;
}

qreal MainWindow::textSize()
{
    return state.settings.text_size;
}

qreal MainWindow::textAngle()
{
    return state.settings.text_angle;
}

bool MainWindow::textBold()
{
    return state.settings.text_style_bold;
}

bool MainWindow::textItalic()
{
    return state.settings.text_style_italic;
}

bool MainWindow::textUnderline()
{
    return state.settings.text_style_underline;
}

bool MainWindow::textStrikeOut()
{
    return state.settings.text_style_strikeout;
}

bool MainWindow::textOverline()
{
    return state.settings.text_style_overline;
}

void
MainWindow::setTextFont(const QString& str)
{
    textFontSelector->setCurrentFont(QFont(str));
    str_const(state.settings.text_font, qPrintable(str));
}

void
MainWindow::setTextSize(qreal num)
{
    state.settings.text_size = qFabs(num);
    int index = textSizeSelector->findText("Custom", Qt::MatchContains);
    if (index != -1) {
        textSizeSelector->removeItem(index);
    }
    textSizeSelector->addItem("Custom " + QString().setNum(num, 'f', 2) + " pt", num);
    index = textSizeSelector->findText("Custom", Qt::MatchContains);
    if (index != -1) {
        textSizeSelector->setCurrentIndex(index);
    }
}

void MainWindow::setTextAngle(qreal num)
{
    state.settings.text_angle = num;
}

void MainWindow::setTextBold(bool val)
{
    state.settings.text_style_bold = val;
}

void MainWindow::setTextItalic(bool val)
{
    state.settings.text_style_italic = val;
}

void MainWindow::setTextUnderline(bool val)
{
    state.settings.text_style_underline = val;
}

void MainWindow::setTextStrikeOut(bool val)
{
    state.settings.text_style_strikeout = val;
}

void MainWindow::setTextOverline(bool val)
{
    state.settings.text_style_overline = val;
}

QString MainWindow::getCurrentLayer()
{
    MdiWindow* mdiWin = qobject_cast<MdiWindow*>(mdiArea->activeSubWindow());
    if (mdiWin) { return mdiWin->getCurrentLayer(); }
    return "0";
}

QRgb MainWindow::getCurrentColor()
{
    MdiWindow* mdiWin = qobject_cast<MdiWindow*>(mdiArea->activeSubWindow());
    if (mdiWin) { return mdiWin->getCurrentColor(); }
    return 0; //TODO: return color ByLayer
}

QString MainWindow::getCurrentLineType()
{
    MdiWindow* mdiWin = qobject_cast<MdiWindow*>(mdiArea->activeSubWindow());
    if (mdiWin) { return mdiWin->getCurrentLineType(); }
    return "ByLayer";
}

QString MainWindow::getCurrentLineWeight()
{
    MdiWindow* mdiWin = qobject_cast<MdiWindow*>(mdiArea->activeSubWindow());
    if (mdiWin) { return mdiWin->getCurrentLineWeight(); }
    return "ByLayer";
}

void MainWindow::deletePressed()
{
    qDebug("deletePressed()");
    QApplication::setOverrideCursor(Qt::WaitCursor);
    MdiWindow* mdiWin = qobject_cast<MdiWindow*>(mdiArea->activeSubWindow());
    if (mdiWin) { mdiWin->deletePressed(); }
    QApplication::restoreOverrideCursor();
}

void MainWindow::escapePressed()
{
    qDebug("escapePressed()");
    QApplication::setOverrideCursor(Qt::WaitCursor);
    MdiWindow* mdiWin = qobject_cast<MdiWindow*>(mdiArea->activeSubWindow());
    if (mdiWin) { mdiWin->escapePressed(); }
    QApplication::restoreOverrideCursor();

    end_command();
}

void MainWindow::toggleGrid()
{
    qDebug("toggleGrid()");
    statusbar->statusBarGridButton->toggle();
}

void MainWindow::toggleRuler()
{
    qDebug("toggleRuler()");
    statusbar->statusBarRulerButton->toggle();
}

void MainWindow::toggleLwt()
{
    qDebug("toggleLwt()");
    statusbar->statusBarLwtButton->toggle();
}

void MainWindow::enablePromptRapidFire()
{
    state.rapid_fire = true;
}

void MainWindow::disablePromptRapidFire()
{
    state.rapid_fire = false;
}

void MainWindow::enableMoveRapidFire()
{
    View* gview = activeView();
    if (gview) gview->enableMoveRapidFire();
}

void MainWindow::disableMoveRapidFire()
{
    View* gview = activeView();
    if (gview) gview->disableMoveRapidFire();
}

void MainWindow::promptHistoryAppended(const QString& txt)
{
    MdiWindow* mdiWin = activeMdiWindow();
    if (mdiWin) mdiWin->promptHistoryAppended(txt);
}

void MainWindow::logPromptInput(const QString& txt)
{
    MdiWindow* mdiWin = activeMdiWindow();
    if (mdiWin) mdiWin->logPromptInput(txt);
}

void MainWindow::promptInputPrevious()
{
    MdiWindow* mdiWin = activeMdiWindow();
    if (mdiWin) mdiWin->promptInputPrevious();
}

void MainWindow::promptInputNext()
{
    MdiWindow* mdiWin = activeMdiWindow();
    if (mdiWin) mdiWin->promptInputNext();
}

void MainWindow::runCommand()
{
    QAction* act = qobject_cast<QAction*>(sender());
    if (act) {
        qDebug("runCommand(%s)", qPrintable(act->objectName()));
        prompt->end_command();
        prompt->setCurrentText(act->objectName());
        prompt->processInput();
    }
}

void MainWindow::runCommandMain(const QString& cmd)
{
    qDebug("runCommandMain(%s)", qPrintable(cmd));
    QString fileName = "commands/" + cmd + "/" + cmd + ".js";
    //if (!state.settings.selection_mode_pick_first) { clear_selection(); } //TODO: Uncomment this line when post-selection is available
    state.context = CONTEXT_MAIN;
    call(cmd);
}

void MainWindow::runCommandClick(const QString& cmd, qreal x, qreal y)
{
    qDebug("runCommandClick(%s, %.2f, %.2f)", qPrintable(cmd), x, y);
    /*
    FIXME:
    state.mouse.x = x;
    state.mouse.y = -y;
    */
    state.context = CONTEXT_CLICK;
    call(cmd);
}

void MainWindow::runCommandMove(const QString& cmd, qreal x, qreal y)
{
    qDebug("runCommandMove(%s, %.2f, %.2f)", qPrintable(cmd), x, y);
    /*
    FIXME:
    state.mouse.x = x;
    state.mouse.y = -y;
    */
    state.context = CONTEXT_MOVE;
    call(cmd);
}

void MainWindow::runCommandContext(const QString& cmd, const QString& str)
{
    qDebug("runCommandContext(%s, %s)", qPrintable(cmd), qPrintable(str));
    state.context = CONTEXT_MENU;
    /* FIXME: copy str to state.context ? */
    call(cmd);
}

void MainWindow::runCommandPrompt(const QString& cmd, const QString& str)
{
    qDebug("runCommandPrompt(%s, %s)", qPrintable(cmd), qPrintable(str));
    state.context = CONTEXT_PROMPT;
    if (state.rapid_fire) {
        /* FIXME: Sanitize str input and copy into a state variable. */
    }
    call(cmd);
}

void MainWindow::alert(const QString& txt)
{
    prompt->alert(txt);
}

void MainWindow::blinkPrompt()
{
    prompt->startBlinking();
}

/* FIXME */
const char *translate_str(char *str)
{
    return (const char *)str;
}

void MainWindow::appendPromptHistory(const QString& txt)
{
    prompt->appendHistory(txt);
}

void
init_command(void)
{
    View* gview = activeView();
    if (gview) {
        gview->clearRubberRoom();
    }
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
    script_env.mainWin->prompt->end_command();
}

void messagebox(const char *type, const char *title, const char *text)
{
     script_env.mainWin->messageBox(type, title, text);
}

void MainWindow::messageBox(const QString& type, const QString& title, const QString& text)
{
    QString msgType = type.toLower();
    if (msgType == "critical") {
        QMessageBox::critical(this, tr(qPrintable(title)), tr(qPrintable(text)));
    }
    else if (msgType == "information") {
        QMessageBox::information(this, tr(qPrintable(title)), tr(qPrintable(text)));
    }
    else if (msgType == "question") {
        QMessageBox::question(this, tr(qPrintable(title)), tr(qPrintable(text)));
    }
    else if (msgType == "warning") {
        QMessageBox::warning(this, tr(qPrintable(title)), tr(qPrintable(text)));
    }
    else {
        QMessageBox::critical(this, tr("MessageBox Error"), tr("Incorrect use of the messageBox function."));
    }
}

void MainWindow::printArea(qreal x, qreal y, qreal w, qreal h)
{
    qDebug("printArea(%.2f, %.2f, %.2f, %.2f)", x, y, w, h);
    //TODO: Print Setup Stuff
    print();
}

void MainWindow::setBackgroundColor(quint8 r, quint8 g, quint8 b)
{
    state.settings.display_bg_color = qRgb(r,g,b);
    updateAllViewBackgroundColors(qRgb(r,g,b));
}

void MainWindow::setCrossHairColor(quint8 r, quint8 g, quint8 b)
{
    state.settings.display_crosshair_color = qRgb(r,g,b);
    updateAllViewCrossHairColors(qRgb(r,g,b));
}

void MainWindow::setGridColor(quint8 r, quint8 g, quint8 b)
{
    state.settings.grid_color = qRgb(r,g,b);
    updateAllViewGridColors(qRgb(r,g,b));
}

void MainWindow::previewOn(int clone, int mode, qreal x, qreal y, qreal data)
{
    View* gview = activeView();
    if (gview) gview->previewOn(clone, mode, x, -y, data);
}

void MainWindow::previewOff()
{
    View* gview = activeView();
    if (gview) gview->previewOff();
}

void MainWindow::vulcanize()
{
    View* gview = activeView();
    if (gview) gview->vulcanizeRubberRoom();
}

void MainWindow::clearRubber()
{
    View* gview = activeView();
    if (gview) {
        gview->clearRubberRoom();
    }
}

bool
MainWindow::allowRubber()
{
    View* gview = activeView();
    if (gview) return gview->allowRubber();
    return false;
}

void MainWindow::spareRubber(qint64 id)
{
    View* gview = activeView();
    if (gview) gview->spareRubber(id);
}

void MainWindow::setRubberMode(int mode)
{
    View* gview = activeView();
    if (gview) gview->setRubberMode(mode);
}

void MainWindow::setRubberPoint(const QString& key, qreal x, qreal y)
{
    View* gview = activeView();
    if (gview) gview->setRubberPoint(key, QPointF(x, -y));
}

void MainWindow::setRubberText(const QString& key, const QString& txt)
{
    View* gview = activeView();
    if (gview) gview->setRubberText(key, txt);
}

void MainWindow::addTextMulti(const QString& str, qreal x, qreal y, qreal rot, bool fill, int rubberMode)
{
}

void MainWindow::addTextSingle(const QString& str, qreal x, qreal y, qreal rot, bool fill, int rubberMode)
{
    View* gview = activeView();
    QGraphicsScene* gscene = gview->scene();
    QUndoStack* stack = gview->getUndoStack();
    if (gview && gscene && stack)
    {
        TextSingleObject* obj = new TextSingleObject(str, x, -y, getCurrentColor());
        obj->setObjectTextFont(state.settings.text_font->data);
        obj->setObjectTextSize(state.settings.text_size);
        obj->setObjectTextStyle(state.settings.text_style_bold,
                                state.settings.text_style_italic,
                                state.settings.text_style_underline,
                                state.settings.text_style_strikeout,
                                state.settings.text_style_overline);
        obj->setObjectTextBackward(false);
        obj->setObjectTextUpsideDown(false);
        obj->setRotation(-rot);
        //TODO: single line text fill
        obj->setObjectRubberMode(rubberMode);
        if (rubberMode)
        {
            gview->addToRubberRoom(obj);
            gscene->addItem(obj);
            gscene->update();
        }
        else
        {
            UndoableAddCommand* cmd = new UndoableAddCommand(obj->data(OBJ_NAME).toString(), obj, gview, 0);
            stack->push(cmd);
        }
    }
}

void MainWindow::addInfiniteLine(qreal x1, qreal y1, qreal x2, qreal y2, qreal rot)
{
}

void MainWindow::addRay(qreal x1, qreal y1, qreal x2, qreal y2, qreal rot)
{
}

void MainWindow::addLine(qreal x1, qreal y1, qreal x2, qreal y2, qreal rot, int rubberMode)
{
    View* gview = activeView();
    QGraphicsScene* gscene = gview->scene();
    QUndoStack* stack = gview->getUndoStack();
    if (gview && gscene && stack)
    {
        LineObject* obj = new LineObject(x1, -y1, x2, -y2, getCurrentColor());
        obj->setRotation(-rot);
        obj->setObjectRubberMode(rubberMode);
        if (rubberMode)
        {
            gview->addToRubberRoom(obj);
            gscene->addItem(obj);
            gscene->update();
        }
        else
        {
            UndoableAddCommand* cmd = new UndoableAddCommand(obj->data(OBJ_NAME).toString(), obj, gview, 0);
            stack->push(cmd);
        }
    }
}

void MainWindow::addTriangle(qreal x1, qreal y1, qreal x2, qreal y2, qreal x3, qreal y3, qreal rot, bool fill)
{
}

void MainWindow::addRectangle(qreal x, qreal y, qreal w, qreal h, qreal rot, bool fill, int rubberMode)
{
    View* gview = activeView();
    QGraphicsScene* gscene = gview->scene();
    QUndoStack* stack = gview->getUndoStack();
    if (gview && gscene && stack)
    {
        RectObject* obj = new RectObject(x, -y, w, -h, getCurrentColor());
        obj->setRotation(-rot);
        obj->setObjectRubberMode(rubberMode);
        //TODO: rect fill
        if (rubberMode)
        {
            gview->addToRubberRoom(obj);
            gscene->addItem(obj);
            gscene->update();
        }
        else
        {
            UndoableAddCommand* cmd = new UndoableAddCommand(obj->data(OBJ_NAME).toString(), obj, gview, 0);
            stack->push(cmd);
        }
    }
}

void MainWindow::addRoundedRectangle(qreal x, qreal y, qreal w, qreal h, qreal rad, qreal rot, bool fill)
{
}

void MainWindow::addArc(qreal startX, qreal startY, qreal midX, qreal midY, qreal endX, qreal endY, int rubberMode)
{
    View* gview = activeView();
    QGraphicsScene* scene = activeScene();
    if (gview && scene)
    {
        ArcObject* arcObj = new ArcObject(startX, -startY, midX, -midY, endX, -endY, getCurrentColor());
        arcObj->setObjectRubberMode(rubberMode);
        if (rubberMode) gview->addToRubberRoom(arcObj);
        scene->addItem(arcObj);
        scene->update();
    }
}

void MainWindow::addCircle(qreal centerX, qreal centerY, qreal radius, bool fill, int rubberMode)
{
    View* gview = activeView();
    QGraphicsScene* gscene = gview->scene();
    QUndoStack* stack = gview->getUndoStack();
    if (gview && gscene && stack)
    {
        CircleObject* obj = new CircleObject(centerX, -centerY, radius, getCurrentColor());
        obj->setObjectRubberMode(rubberMode);
        //TODO: circle fill
        if (rubberMode)
        {
            gview->addToRubberRoom(obj);
            gscene->addItem(obj);
            gscene->update();
        }
        else
        {
            UndoableAddCommand* cmd = new UndoableAddCommand(obj->data(OBJ_NAME).toString(), obj, gview, 0);
            stack->push(cmd);
        }
    }
}

void MainWindow::addSlot(qreal centerX, qreal centerY, qreal diameter, qreal length, qreal rot, bool fill, int rubberMode)
{
    //TODO: Use UndoableAddCommand for slots
    /*
    SlotObject* slotObj = new SlotObject(centerX, -centerY, diameter, length, getCurrentColor());
    slotObj->setRotation(-rot);
    slotObj->setObjectRubberMode(rubberMode);
    if (rubberMode) gview->addToRubberRoom(slotObj);
    scene->addItem(slotObj);
    //TODO: slot fill
    scene->update();
    */
}

void MainWindow::addEllipse(qreal centerX, qreal centerY, qreal width, qreal height, qreal rot, bool fill, int rubberMode)
{
    View* gview = activeView();
    QGraphicsScene* gscene = gview->scene();
    QUndoStack* stack = gview->getUndoStack();
    if (gview && gscene && stack)
    {
        EllipseObject* obj = new EllipseObject(centerX, -centerY, width, height, getCurrentColor());
        obj->setRotation(-rot);
        obj->setObjectRubberMode(rubberMode);
        //TODO: ellipse fill
        if (rubberMode)
        {
            gview->addToRubberRoom(obj);
            gscene->addItem(obj);
            gscene->update();
        }
        else
        {
            UndoableAddCommand* cmd = new UndoableAddCommand(obj->data(OBJ_NAME).toString(), obj, gview, 0);
            stack->push(cmd);
        }
    }
}

void MainWindow::addPoint(qreal x, qreal y)
{
    View* gview = activeView();
    QUndoStack* stack = gview->getUndoStack();
    if (gview && stack)
    {
        PointObject* obj = new PointObject(x, -y, getCurrentColor());
        UndoableAddCommand* cmd = new UndoableAddCommand(obj->data(OBJ_NAME).toString(), obj, gview, 0);
        stack->push(cmd);
    }
}

void MainWindow::addRegularPolygon(qreal centerX, qreal centerY, quint16 sides, quint8 mode, qreal rad, qreal rot, bool fill)
{
}

//NOTE: This native is different than the rest in that the Y+ is down (scripters need not worry about this)
void MainWindow::addPolygon(qreal startX, qreal startY, const QPainterPath& p, int rubberMode)
{
    View* gview = activeView();
    QGraphicsScene* gscene = gview->scene();
    QUndoStack* stack = gview->getUndoStack();
    if (gview && gscene && stack)
    {
        PolygonObject* obj = new PolygonObject(startX, startY, p, getCurrentColor());
        obj->setObjectRubberMode(rubberMode);
        if (rubberMode)
        {
            gview->addToRubberRoom(obj);
            gscene->addItem(obj);
            gscene->update();
        }
        else
        {
            UndoableAddCommand* cmd = new UndoableAddCommand(obj->data(OBJ_NAME).toString(), obj, gview, 0);
            stack->push(cmd);
        }
    }
}

//NOTE: This native is different than the rest in that the Y+ is down (scripters need not worry about this)
void MainWindow::addPolyline(qreal startX, qreal startY, const QPainterPath& p, int rubberMode)
{
    View* gview = activeView();
    QGraphicsScene* gscene = gview->scene();
    QUndoStack* stack = gview->getUndoStack();
    if (gview && gscene && stack)
    {
        PolylineObject* obj = new PolylineObject(startX, startY, p, getCurrentColor());
        obj->setObjectRubberMode(rubberMode);
        if (rubberMode)
        {
            gview->addToRubberRoom(obj);
            gscene->addItem(obj);
            gscene->update();
        }
        else
        {
            UndoableAddCommand* cmd = new UndoableAddCommand(obj->data(OBJ_NAME).toString(), obj, gview, 0);
            stack->push(cmd);
        }
    }
}

//NOTE: This native is different than the rest in that the Y+ is down (scripters need not worry about this)
void MainWindow::addPath(qreal startX, qreal startY, const QPainterPath& p, int rubberMode)
{
}

void MainWindow::addHorizontalDimension(qreal x1, qreal y1, qreal x2, qreal y2, qreal legHeight)
{
}

void MainWindow::addVerticalDimension(qreal x1, qreal y1, qreal x2, qreal y2, qreal legHeight)
{
}

void MainWindow::addImage(const QString& img, qreal x, qreal y, qreal w, qreal h, qreal rot)
{
}

void MainWindow::addDimLeader(qreal x1, qreal y1, qreal x2, qreal y2, qreal rot, int rubberMode)
{
    View* gview = activeView();
    QGraphicsScene* gscene = gview->scene();
    QUndoStack* stack = gview->getUndoStack();
    if (gview && gscene && stack)
    {
        DimLeaderObject* obj = new DimLeaderObject(x1, -y1, x2, -y2, getCurrentColor());
        obj->setRotation(-rot);
        obj->setObjectRubberMode(rubberMode);
        if (rubberMode)
        {
            gview->addToRubberRoom(obj);
            gscene->addItem(obj);
            gscene->update();
        }
        else
        {
            UndoableAddCommand* cmd = new UndoableAddCommand(obj->data(OBJ_NAME).toString(), obj, gview, 0);
            stack->push(cmd);
        }
    }
}

void MainWindow::setCursorShape(const QString& str)
{
    View* gview = activeView();
    if (gview)
    {
        QString shape = str.toLower();
        if     (shape == "arrow")           gview->setCursor(QCursor(Qt::ArrowCursor));
        else if (shape == "uparrow")         gview->setCursor(QCursor(Qt::UpArrowCursor));
        else if (shape == "cross")           gview->setCursor(QCursor(Qt::CrossCursor));
        else if (shape == "wait")            gview->setCursor(QCursor(Qt::WaitCursor));
        else if (shape == "ibeam")           gview->setCursor(QCursor(Qt::IBeamCursor));
        else if (shape == "resizevert")      gview->setCursor(QCursor(Qt::SizeVerCursor));
        else if (shape == "resizehoriz")     gview->setCursor(QCursor(Qt::SizeHorCursor));
        else if (shape == "resizediagleft")  gview->setCursor(QCursor(Qt::SizeBDiagCursor));
        else if (shape == "resizediagright") gview->setCursor(QCursor(Qt::SizeFDiagCursor));
        else if (shape == "move")            gview->setCursor(QCursor(Qt::SizeAllCursor));
        else if (shape == "blank")           gview->setCursor(QCursor(Qt::BlankCursor));
        else if (shape == "splitvert")       gview->setCursor(QCursor(Qt::SplitVCursor));
        else if (shape == "splithoriz")      gview->setCursor(QCursor(Qt::SplitHCursor));
        else if (shape == "handpointing")    gview->setCursor(QCursor(Qt::PointingHandCursor));
        else if (shape == "forbidden")       gview->setCursor(QCursor(Qt::ForbiddenCursor));
        else if (shape == "handopen")        gview->setCursor(QCursor(Qt::OpenHandCursor));
        else if (shape == "handclosed")      gview->setCursor(QCursor(Qt::ClosedHandCursor));
        else if (shape == "whatsthis")       gview->setCursor(QCursor(Qt::WhatsThisCursor));
        else if (shape == "busy")            gview->setCursor(QCursor(Qt::BusyCursor));
        else if (shape == "dragmove")        gview->setCursor(QCursor(Qt::DragMoveCursor));
        else if (shape == "dragcopy")        gview->setCursor(QCursor(Qt::DragCopyCursor));
        else if (shape == "draglink")        gview->setCursor(QCursor(Qt::DragLinkCursor));
    }
}

int num_selected(void)
{
    View* gview = activeView();
    if (gview) {
        return gview->numSelected();
    }
    return 0;
}

void MainWindow::addToSelection(const QPainterPath path, Qt::ItemSelectionMode mode)
{
}

void
clear_selection(void)
{
    View* gview = activeView();
    if (gview) {
        gview->clear_selection();
    }
}

void delete_selected(void)
{
    View* gview = activeView();
    if (gview) {
        gview->deleteSelected();
    }
}

void cut_selected(float x, float y)
{
}

void copy_selected(float x, float y)
{
}

void paste_selected(float x, float y)
{
}

void move_selected(float dx, float dy)
{
    View* gview = activeView();
    if (gview) {
        gview->moveSelected(dx, -dy);
    }
}

void scale_selected(float x, float y, float factor)
{
    if (factor <= 0.0) {
        messagebox("critical", "ScaleFactor Error",
            "Hi there. If you are not a developer, report this as a bug. "
            "If you are a developer, your code needs examined, and possibly your head too.");
    }

    View* gview = activeView();
    if (gview) {
        gview->scaleSelected(x, -y, factor);
    }
}

void rotate_selected(float x, float y, float rot)
{
    View* gview = activeView();
    if (gview) {
        gview->rotateSelected(x, -y, -rot);
    }
}

void mirror_selected(float x1, float y1, float x2, float y2)
{
    View* gview = activeView();
    if (gview) {
        gview->mirrorSelected(x1, -y1, x2, -y2);
    }
}

float qsnapx(void)
{
    QGraphicsScene* scene = activeScene();
    if (scene) {
        return scene->property(SCENE_QSNAP_POINT).toPointF().x();
    }
    return 0.0;
}

float qsnapy(void)
{
    QGraphicsScene* scene = activeScene();
    if (scene) {
        return -scene->property(SCENE_QSNAP_POINT).toPointF().y();
    }
    return 0.0;
}

float mousex(void)
{
    QGraphicsScene* scene = activeScene();
    if (scene) {
        qDebug("mouseX: %.50f", scene->property(SCENE_MOUSE_POINT).toPointF().x());
        return scene->property(SCENE_MOUSE_POINT).toPointF().x();
    }
    return 0.0;
}

float mousey(void)
{
    QGraphicsScene* scene = activeScene();
    if (scene) {
        qDebug("mouseY: %.50f", -scene->property(SCENE_MOUSE_POINT).toPointF().y());
        return -scene->property(SCENE_MOUSE_POINT).toPointF().y();
    }
    return 0.0;
}

