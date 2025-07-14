#include "native-scripting.h"
#include "mainwindow.h"
#include "view.h"

#include <iostream>
#include <algorithm>
#include <map>

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
#include "dialog.h"

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

void add_polygon(qreal startX, qreal startY, const QPainterPath& p, int rubberMode);
void add_polyline(qreal startX, qreal startY, const QPainterPath& p, int rubberMode);

MainWindow* _mainWin = 0;

View*
activeView(void)
{
    return _mainWin->activeView();
}

QGraphicsScene*
activeScene(void)
{
    return activeView()->scene();
}

QRgb
getCurrentColor(void)
{
    return _mainWin->getCurrentColor();
}

const char *
getSettingsTextFont()
{
    return qPrintable(_mainWin->getSettingsTextFont());
}

double
getSettingsTextSize()
{
    return _mainWin->getSettingsTextSize();
}

bool
getSettingsTextStyleBold(void)
{
    return _mainWin->getSettingsTextStyleBold();
}

bool
getSettingsTextStyleItalic(void)
{
    return _mainWin->getSettingsTextStyleItalic();
}

bool
getSettingsTextStyleUnderline() {
    return _mainWin->getSettingsTextStyleUnderline();
}

bool
getSettingsTextStyleStrikeOut(void)
{
    return _mainWin->getSettingsTextStyleStrikeOut();
}

bool
getSettingsTextStyleOverline()
{
    return _mainWin->getSettingsTextStyleOverline();
}

/*!
 * \brief throwError
 */
ScriptValue
throwError(int type, const char *message)
{
    qDebug("ERROR %d: %s", type, message);
    return script_null;
}

int
streq(char *a, const char *b)
{
    return !strncmp(a, b, 200);
}

void
debug(const char *s)
{
    qDebug(s);
}

/* C-linkage compatible wrappers of the natives. */
void
alert(const char *s)
{
    _mainWin->prompt->alert(s);
}

void
blink_prompt(void)
{
    _mainWin->prompt->startBlinking();
}

void
set_prompt_prefix(const char *s)
{
    _mainWin->prompt->setPrefix(s);
}

void
append_prompt_history(const char *s)
{
    _mainWin->prompt->appendHistory(s);
}

void
enable_prompt_rapid_fire(void)
{
    _mainWin->prompt->enableRapidFire();
}

void
disable_prompt_rapid_fire(void)
{
    _mainWin->prompt->disableRapidFire();
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
    _mainWin->prompt->endCommand();
}

void
enable_move_rapid_fire(void)
{
    View* gview = activeView();
    if (gview) {
        gview->enableMoveRapidFire();
    }
}

void
disable_move_rapid_fire(void)
{
    View* gview = activeView();
    if (gview) {
        gview->disableMoveRapidFire();
    }
}

void
new_file(void)
{
    _mainWin->newFile();
}

void
open_file(void)
{
    _mainWin->openFile();
}

void
exit_program(void)
{
    _mainWin->exit();
}

void
help(void)
{
    _mainWin->help();
}

void
about(void)
{
    _mainWin->about();
}

void
tip_of_the_day(void)
{
    _mainWin->tipOfTheDay();
}

void
window_cascade(void)
{
    _mainWin->mdiArea->cascade();
}

void
window_tile(void)
{
    _mainWin->mdiArea->tile();
}

void
window_close(void)
{
    _mainWin->onCloseWindow();
}

void
window_close_all(void)
{
    _mainWin->mdiArea->closeAllSubWindows();
}

void
window_next(void)
{
    _mainWin->mdiArea->activateNextSubWindow();
}

void
window_previous(void)
{
    _mainWin->mdiArea->activatePreviousSubWindow();
}

const char *
platform_string(void)
{
    return qPrintable(_mainWin->platformString());
}

void
message_box(const char *type, const char *title, const char *text)
{
    QString msgType = QString(type).toLower();
    if (msgType == "critical") {
        QMessageBox::critical(_mainWin, _mainWin->tr(title), _mainWin->tr(text));
    }
    else if (msgType == "information") {
        QMessageBox::information(_mainWin, _mainWin->tr(title), _mainWin->tr(text));
    }
    else if (msgType == "question") {
        QMessageBox::question(_mainWin, _mainWin->tr(title), _mainWin->tr(text));
    }
    else if (msgType == "warning") {
        QMessageBox::warning(_mainWin, _mainWin->tr(title), _mainWin->tr(text));
    }
    else {
        QMessageBox::critical(_mainWin,
            _mainWin->tr("Native MessageBox Error"),
            _mainWin->tr("Incorrect use of the native messageBox function."));
    }
}

void
undo(void)
{
    _mainWin->undo();
}

void
redo(void)
{
    _mainWin->redo();
}

void
icon16(void)
{
    _mainWin->iconResize(16);
}

void
icon24(void)
{
    _mainWin->iconResize(24);
}

void
icon32(void)
{
    _mainWin->iconResize(32);
}

void
icon48(void)
{
    _mainWin->iconResize(48);
}

void
icon64(void)
{
    _mainWin->iconResize(64);
}

void
icon128(void)
{
    _mainWin->iconResize(128);
}

void
pan_left(void)
{
    qDebug("panLeft()");
    View* gview = activeView();
    QUndoStack* stack = gview->getUndoStack();
    if (gview && stack) {
        UndoableNavCommand* cmd = new UndoableNavCommand("PanLeft", gview, 0);
        stack->push(cmd);
    }
}

void
pan_right(void)
{
    qDebug("panRight()");
    View* gview = activeView();
    QUndoStack* stack = gview->getUndoStack();
    if (gview && stack) {
        UndoableNavCommand* cmd = new UndoableNavCommand("PanRight", gview, 0);
        stack->push(cmd);
    }
}

void
pan_up(void)
{
    qDebug("panUp()");
    View* gview = activeView();
    QUndoStack* stack = gview->getUndoStack();
    if (gview && stack) {
        UndoableNavCommand* cmd = new UndoableNavCommand("PanUp", gview, 0);
        stack->push(cmd);
    }
}

void
pan_down(void)
{
    qDebug("panDown()");
    View* gview = activeView();
    QUndoStack* stack = gview->getUndoStack();
    if (gview && stack) {
        UndoableNavCommand* cmd = new UndoableNavCommand("PanDown", gview, 0);
        stack->push(cmd);
    }
}

void
zoom_in(void)
{
    _mainWin->zoomIn();
}

void
zoom_out(void)
{
    _mainWin->zoomOut();
}

void
zoom_extents(void)
{
    _mainWin->zoomExtents();
}

void
print_area(double x, double y, double w, double h)
{
    qDebug("nativePrintArea(%.2f, %.2f, %.2f, %.2f)", x, y, w, h);
    //TODO: Print Setup Stuff
    _mainWin->print();
}

/* TODO: Make day vision color settings. */
void
day(void)
{
    View* gview = activeView();
    if (gview) {
        gview->setBackgroundColor(qRgb(255,255,255));
        gview->setCrossHairColor(qRgb(0,0,0));
        gview->setGridColor(qRgb(0,0,0));
    }
}

/* TODO: Make night vision color settings. */
void
night(void)
{
    View* gview = activeView();
    if (gview) {
        gview->setBackgroundColor(qRgb(0,0,0));
        gview->setCrossHairColor(qRgb(255,255,255));
        gview->setGridColor(qRgb(255,255,255));
    }
}

void
set_background_color(uint8_t r, uint8_t g, uint8_t b)
{
    _mainWin->setSettingsDisplayBGColor(qRgb(r,g,b));
    _mainWin->updateAllViewBackgroundColors(qRgb(r,g,b));
}

void
set_crosshair_color(uint8_t r, uint8_t g, uint8_t b)
{
    _mainWin->setSettingsDisplayCrossHairColor(qRgb(r,g,b));
    _mainWin->updateAllViewCrossHairColors(qRgb(r,g,b));
}

void
set_grid_color(uint8_t r, uint8_t g, uint8_t b)
{
    _mainWin->setSettingsGridColor(qRgb(r,g,b));
    _mainWin->updateAllViewGridColors(qRgb(r,g,b));
}

const char *
text_font(void)
{
    return qPrintable(_mainWin->textFont());
}

double
text_size(void)
{
    return _mainWin->textSize();
}

double
text_angle(void)
{
    return _mainWin->textAngle();
}

bool
text_bold(void)
{
    return _mainWin->textBold();
}

bool
text_italic(void)
{
    return _mainWin->textItalic();
}

bool
text_underline(void)
{
    return _mainWin->textUnderline();
}

bool
text_strikeout(void)
{
    return _mainWin->textStrikeOut();
}

bool
text_overline(void)
{
    return _mainWin->textOverline();
}

void
set_text_font(const char *s)
{
    _mainWin->setTextFont(s);
}

void
set_text_size(double size)
{
    _mainWin->setTextSize(size);
}

void
set_text_angle(double angle)
{
    _mainWin->setTextAngle(angle);
}

void
set_text_bold(bool bold)
{
    _mainWin->setTextBold(bold);
}

void
set_text_italic(bool italic)
{
    _mainWin->setTextItalic(italic);
}

void
set_text_underline(bool underline)
{
    _mainWin->setTextUnderline(underline);
}

void
set_text_strikeout(bool strikeout)
{
    _mainWin->setTextStrikeOut(strikeout);
}

void
set_text_overline(bool overline)
{
    _mainWin->setTextOverline(overline);
}

void
preview_on(int clone, int mode, double x, double y, double data)
{
    View* gview = activeView();
    if (gview) {
        gview->previewOn(clone, mode, x, -y, data);
    }
}

void
preview_off(void)
{
    View* gview = activeView();
    if (gview) {
        gview->previewOff();
    }
}

void
vulcanize(void)
{
    View* gview = activeView();
    if (gview) {
        gview->vulcanizeRubberRoom();
    }
}

void
clear_rubber(void)
{
    View* gview = activeView();
    if (gview) {
        gview->clearRubberRoom();
    }
}

bool
allow_rubber(void)
{
    View* gview = activeView();
    if (gview) {
        return gview->allowRubber();
    }
    return false;
}

void
spare_rubber(int64_t id)
{
    View* gview = activeView();
    if (gview) {
        gview->spareRubber(id);
    }
}

void
set_rubber_filter(int64_t id)
{
    //TODO: void nativeSetRubberFilter(qint64 id); //TODO: This is so more than 1 rubber object can exist at one time without updating all rubber objects at once
}

void
set_rubber_mode(int mode)
{
    View* gview = activeView();
    if (gview) {
        gview->setRubberMode(mode);
    }
}

void
set_rubber_point(const char *key, double x, double y)
{
    View* gview = activeView();
    if (gview) {
        gview->setRubberPoint(key, QPointF(x, -y));
    }
}

void
set_rubber_text(const char *key, const char *txt)
{
    View* gview = activeView();
    if (gview) {
        gview->setRubberText(key, txt);
    }
}

void
add_text_multi(const char *s, double x, double y, double rot, bool fill, int rubberMode)
{
    /* TODO */
}

void
add_text_single(const char *s, double x, double y, double rot, bool fill, int rubberMode)
{
    View* gview = activeView();
    QGraphicsScene* gscene = gview->scene();
    QUndoStack* stack = gview->getUndoStack();
    if (gview && gscene && stack) {
        TextSingleObject* obj = new TextSingleObject(s, x, -y, getCurrentColor());
        obj->setObjectTextFont(getSettingsTextFont());
        obj->setObjectTextSize(getSettingsTextSize());
        obj->setObjectTextStyle(getSettingsTextStyleBold(),
                                getSettingsTextStyleItalic(),
                                getSettingsTextStyleUnderline(),
                                getSettingsTextStyleStrikeOut(),
                                getSettingsTextStyleOverline());
        obj->setObjectTextBackward(false);
        obj->setObjectTextUpsideDown(false);
        obj->setRotation(-rot);
        //TODO: single line text fill
        obj->setObjectRubberMode(rubberMode);
        if(rubberMode)
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

void
add_infinite_line(double x1, double y1, double x2, double y2, double rot)
{
    /* TODO */
}

void
add_ray(double x1, double y1, double x2, double y2, double rot)
{
    /* TODO */
}

void
add_line(double x1, double y1, double x2, double y2, double rot, int rubberMode)
{
    View* gview = activeView();
    QGraphicsScene* gscene = gview->scene();
    QUndoStack* stack = gview->getUndoStack();
    if (gview && gscene && stack) {
        LineObject* obj = new LineObject(x1, -y1, x2, -y2, getCurrentColor());
        obj->setRotation(-rot);
        obj->setObjectRubberMode(rubberMode);
        if (rubberMode) {
            gview->addToRubberRoom(obj);
            gscene->addItem(obj);
            gscene->update();
        }
        else {
            UndoableAddCommand* cmd = new UndoableAddCommand(obj->data(OBJ_NAME).toString(), obj, gview, 0);
            stack->push(cmd);
        }
    }
}

void
add_triangle(double x1, double y1, double x2, double y2, double x3, double y3, double rot, bool fill)
{
    /* TODO */
}

void
add_rectangle(double x, double y, double w, double h, double rot, bool fill, int rubberMode)
{
    View* gview = activeView();
    QGraphicsScene* gscene = gview->scene();
    QUndoStack* stack = gview->getUndoStack();
    if (gview && gscene && stack) {
        RectObject* obj = new RectObject(x, -y, w, -h, getCurrentColor());
        obj->setRotation(-rot);
        obj->setObjectRubberMode(rubberMode);
        //TODO: rect fill
        if(rubberMode) {
            gview->addToRubberRoom(obj);
            gscene->addItem(obj);
            gscene->update();
        }
        else {
            UndoableAddCommand* cmd = new UndoableAddCommand(obj->data(OBJ_NAME).toString(), obj, gview, 0);
            stack->push(cmd);
        }
    }
}

void
add_rounded_rectangle(double x, double y, double w, double h, double rad, double rot, bool fill)
{
    /* TODO */
}

void
add_arc(double startX, double startY, double midX, double midY, double endX, double endY, int rubberMode)
{
    View* gview = activeView();
    QGraphicsScene* scene = activeScene();
    if (gview && scene) {
        ArcObject* arcObj = new ArcObject(startX, -startY, midX, -midY, endX, -endY, getCurrentColor());
        arcObj->setObjectRubberMode(rubberMode);
        if(rubberMode) gview->addToRubberRoom(arcObj);
        scene->addItem(arcObj);
        scene->update();
    }
}

void
add_circle(double centerX, double centerY, double radius, bool fill, int rubberMode)
{
    View* gview = activeView();
    QGraphicsScene* gscene = gview->scene();
    QUndoStack* stack = gview->getUndoStack();
    if(gview && gscene && stack)
    {
        CircleObject* obj = new CircleObject(centerX, -centerY, radius, getCurrentColor());
        obj->setObjectRubberMode(rubberMode);
        //TODO: circle fill
        if(rubberMode)
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

void
add_slot(double centerX, double centerY, double diameter, double length, double rot, bool fill, int rubberMode)
{
    //TODO: Use UndoableAddCommand for slots
    /*
    SlotObject* slotObj = new SlotObject(centerX, -centerY, diameter, length, getCurrentColor());
    slotObj->setRotation(-rot);
    slotObj->setObjectRubberMode(rubberMode);
    if(rubberMode) gview->addToRubberRoom(slotObj);
    scene->addItem(slotObj);
    //TODO: slot fill
    scene->update();
    */
}

void
add_ellipse(double centerX, double centerY, double width, double height, double rot, bool fill, int rubberMode)
{
    View* gview = activeView();
    QGraphicsScene* gscene = gview->scene();
    QUndoStack* stack = gview->getUndoStack();
    if(gview && gscene && stack) {
        EllipseObject* obj = new EllipseObject(centerX, -centerY, width, height, getCurrentColor());
        obj->setRotation(-rot);
        obj->setObjectRubberMode(rubberMode);
        //TODO: ellipse fill
        if(rubberMode)
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

void
add_point(double x, double y)
{
    View* gview = activeView();
    QUndoStack* stack = gview->getUndoStack();
    if(gview && stack)
    {
        PointObject* obj = new PointObject(x, -y, getCurrentColor());
        UndoableAddCommand* cmd = new UndoableAddCommand(obj->data(OBJ_NAME).toString(), obj, gview, 0);
        stack->push(cmd);
    }
}

void
add_regular_polygon(double centerX, double centerY, int sides, int mode, double rad, double rot, bool fill)
{

}

//NOTE: This native is different than the rest in that the Y+ is down (scripters need not worry about this)
void
add_polygon(qreal startX, qreal startY, const QPainterPath& p, int rubberMode)
{
    View* gview = activeView();
    QGraphicsScene* gscene = gview->scene();
    QUndoStack* stack = gview->getUndoStack();
    if(gview && gscene && stack)
    {
        PolygonObject* obj = new PolygonObject(startX, startY, p, getCurrentColor());
        obj->setObjectRubberMode(rubberMode);
        if(rubberMode)
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
void
add_polyline(qreal startX, qreal startY, const QPainterPath& p, int rubberMode)
{
    View* gview = activeView();
    QGraphicsScene* gscene = gview->scene();
    QUndoStack* stack = gview->getUndoStack();
    if (gview && gscene && stack) {
        PolylineObject* obj = new PolylineObject(startX, startY, p, getCurrentColor());
        obj->setObjectRubberMode(rubberMode);
        if(rubberMode) {
            gview->addToRubberRoom(obj);
            gscene->addItem(obj);
            gscene->update();
        }
        else {
            UndoableAddCommand* cmd = new UndoableAddCommand(obj->data(OBJ_NAME).toString(), obj, gview, 0);
            stack->push(cmd);
        }
    }
}

/*
void nativeAddPath(qreal startX, qreal startY, const QPainterPath& p, int rubberMode);
*/
void
add_path(double startX, double startY, int rubberMode)
{
    /* TODO */
}

void
add_horizontal_dimension(double x1, double y1, double x2, double y2, double legHeight)
{
    /* TODO */
}

void
add_veritical_dimension(double x1, double y1, double x2, double y2, double legHeight)
{
    /* TODO */
}

void
add_image(const char *img, double x, double y, double w, double h, double rot)
{
    /* TODO */
}

void
add_dimleader(double x1, double y1, double x2, double y2, double rot, int rubberMode)
{
    View* gview = activeView();
    QGraphicsScene* gscene = gview->scene();
    QUndoStack* stack = gview->getUndoStack();
    if (gview && gscene && stack) {
        DimLeaderObject* obj = new DimLeaderObject(x1, -y1, x2, -y2, getCurrentColor());
        obj->setRotation(-rot);
        obj->setObjectRubberMode(rubberMode);
        if(rubberMode) {
            gview->addToRubberRoom(obj);
            gscene->addItem(obj);
            gscene->update();
        }
        else {
            UndoableAddCommand* cmd = new UndoableAddCommand(obj->data(OBJ_NAME).toString(), obj, gview, 0);
            stack->push(cmd);
        }
    }
}

void
set_cursor_shape(const char *str)
{
    View* gview = activeView();
    if (!gview) {
        return;
    }
    QString shape = QString(str).toLower();
    if (shape == "arrow")
        gview->setCursor(QCursor(Qt::ArrowCursor));
    else if (shape == "uparrow")
        gview->setCursor(QCursor(Qt::UpArrowCursor));
    else if (shape == "cross")
        gview->setCursor(QCursor(Qt::CrossCursor));
    else if (shape == "wait")
        gview->setCursor(QCursor(Qt::WaitCursor));
    else if (shape == "ibeam")
        gview->setCursor(QCursor(Qt::IBeamCursor));
    else if (shape == "resizevert")
        gview->setCursor(QCursor(Qt::SizeVerCursor));
    else if (shape == "resizehoriz")
        gview->setCursor(QCursor(Qt::SizeHorCursor));
    else if (shape == "resizediagleft")
        gview->setCursor(QCursor(Qt::SizeBDiagCursor));
    else if (shape == "resizediagright")
        gview->setCursor(QCursor(Qt::SizeFDiagCursor));
    else if (shape == "move")
        gview->setCursor(QCursor(Qt::SizeAllCursor));
    else if (shape == "blank")
        gview->setCursor(QCursor(Qt::BlankCursor));
    else if (shape == "splitvert")
        gview->setCursor(QCursor(Qt::SplitVCursor));
    else if (shape == "splithoriz")
        gview->setCursor(QCursor(Qt::SplitHCursor));
    else if (shape == "handpointing")
        gview->setCursor(QCursor(Qt::PointingHandCursor));
    else if (shape == "forbidden")
        gview->setCursor(QCursor(Qt::ForbiddenCursor));
    else if (shape == "handopen")
        gview->setCursor(QCursor(Qt::OpenHandCursor));
    else if (shape == "handclosed")
        gview->setCursor(QCursor(Qt::ClosedHandCursor));
    else if (shape == "whatsthis")
        gview->setCursor(QCursor(Qt::WhatsThisCursor));
    else if (shape == "busy")
        gview->setCursor(QCursor(Qt::BusyCursor));
    else if (shape == "dragmove")
        gview->setCursor(QCursor(Qt::DragMoveCursor));
    else if (shape == "dragcopy")
        gview->setCursor(QCursor(Qt::DragCopyCursor));
    else if (shape == "draglink")
        gview->setCursor(QCursor(Qt::DragLinkCursor));
}

double
calculate_angle(double x1, double y1, double x2, double y2)
{
    return QLineF(x1, -y1, x2, -y2).angle();
}

double
calculate_distance(double x1, double y1, double x2, double y2)
{
    return QLineF(x1, y1, x2, y2).length();
}

double
perpendicular_distance(double px, double py, double x1, double y1, double x2, double y2)
{
    QLineF line(x1, y1, x2, y2);
    QLineF norm = line.normalVector();
    qreal dx = px-x1;
    qreal dy = py-y1;
    norm.translate(dx, dy);
    QPointF iPoint;
    norm.intersects(line, &iPoint);
    return QLineF(px, py, iPoint.x(), iPoint.y()).length();
}

int
num_selected(void)
{
    View* gview = activeView();
    if (gview) {
        return gview->numSelected();
    }
    return 0;
}

void
select_all(void)
{
    View* gview = activeView();
    if (gview) {
        gview->selectAll();
    }
}

void
add_to_selection(void)
{
    /* TODO (const QPainterPath path, Qt::ItemSelectionMode mode) */
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
delete_selected(void)
{
    View* gview = activeView();
    if (gview) {
        gview->deleteSelected();
    }
}

void
cut_selected(double x, double y)
{
    /* TODO */
}

void
copy_selected(double x, double y)
{
    /* TODO */
}

void
paste_selected(double x, double y)
{
    /* TODO */
}

void
move_selected(double dx, double dy)
{
    View* gview = activeView();
    if (gview) {
        gview->moveSelected(dx, -dy);
    }
}

void
scale_selected(double x, double y, double factor)
{
    if (factor <= 0.0) {
        message_box("critical", "ScaleFactor Error",
            "Hi there. If you are not a developer, report this as a bug. "
            "If you are a developer, your code needs examined, and possibly your head too.");
    }

    View* gview = activeView();
    if (gview) {
        gview->scaleSelected(x, -y, factor);
    }
}

void
rotate_selected(double x, double y, double rot)
{
    View* gview = activeView();
    if (gview) {
        gview->rotateSelected(x, -y, -rot);
    }
}

void
mirror_selected(double x1, double y1, double x2, double y2)
{
    View* gview = activeView();
    if (gview) {
        gview->mirrorSelected(x1, -y1, x2, -y2);
    }
}

double
qsnapx(void)
{
    QGraphicsScene* scene = activeScene();
    if (scene) {
        return scene->property(SCENE_QSNAP_POINT).toPointF().x();
    }
    return 0.0;
}

double
qsnapy(void)
{
    QGraphicsScene* scene = activeScene();
    if (scene) {
        return -scene->property(SCENE_QSNAP_POINT).toPointF().y();
    }
    return 0.0;
}

double
mousex(void)
{
    QGraphicsScene* scene = activeScene();
    if (scene) {
        double x = scene->property(SCENE_MOUSE_POINT).toPointF().x();
        qDebug("mouseX: %.50f", x);
        return x;
    }
    return 0.0;
}

double
mousey(void)
{
    QGraphicsScene* scene = activeScene();
    if (scene) {
        double y = -scene->property(SCENE_MOUSE_POINT).toPointF().y();
        qDebug("mouseY: %.50f", y);
        return y;
    }
    return 0.0;
}

/*!
 * \brief javaLoadCommand
 *
 * For external (not built-in) commands.
 *
 * \note Every QScriptProgram must have a unique function name to call. If every function was called main(), then
 *       the ScriptEngine would only call the last script evaluated (which happens to be main() in another script).
 *       Thus, by adding the cmdName before main(), it becomes line_main(), circle_main(), etc...
 *       Do not change this code unless you really know what you are doing. I mean it.
 */
void
MainWindow::loadCommands(void)
{
    qDebug("loadCommands()");
    /*!
     * \brief Initiate the MainWin pointer.
     *
     * \note qthelp://com.trolltech.qt.470/qdoc/scripting.html
     * \note Wrapping a Native Function
     * \note It is currently not possible to wrap member functions; i.e., methods of
     *       a C++ class that require a this object.
     */
    _mainWin = this;

    QString appDir = qApp->applicationDirPath();

    /* Load all command data in a loop. */
    QDir commandDir(appDir + "/commands");
    QStringList format;
    format << "*.ini";
    QStringList cmdList = commandDir.entryList(format, QDir::Files);
    foreach(QString cmdFile, cmdList) {
        QString fname = appDir + "/commands/" + cmdFile;
        QString cmdName = cmdFile.replace(".ini", "");
        Command command;
        QSettings settings(fname, QSettings::IniFormat);
        command.menu_name = qPrintable(settings.value("Menu/Name", "Lost & Found").toString());
        command.menu_pos = settings.value("Menu/Position", 0).toInt();
        command.toolbar_name = qPrintable(settings.value("ToolBar/Name", "Lost & Found").toString());
        command.toolbar_pos = settings.value("ToolBar/Position", 0).toInt();
        command.tooltip = qPrintable(settings.value("Tips/ToolTip", "").toString());
        command.statustip = qPrintable(settings.value("Tips/StatusTip", "").toString());
        command.aliases = qPrintable(settings.value("Prompt/Alias").toString());
        command.main = qPrintable(settings.value("Functions/Main", "").toString());
        command.click = qPrintable(settings.value("Functions/Click",
            "").toString());
        command.move = qPrintable(settings.value("Functions/Move",
            "").toString());
        command.context = qPrintable(settings.value("Functions/Context",
            "").toString());
        command.builtin = settings.value("Functions/Builtin", 0).toInt();
        command.action = createAction(cmdName, QString(command.tooltip.c_str()),
            QString(command.statustip.c_str()), true);
        command_list[qPrintable(cmdName)] = command;

        QStringList aliases = settings.value("Prompt/Alias").toStringList();

        foreach(QString alias, aliases) {
            prompt->addCommand(alias, cmdName);
        }
    }
}

bool
comp(std::pair<std::string, int> a, std::pair<std::string, int> b)
{
    return a.second < b.second;
}

/* Populate Toolbars */
void
MainWindow::loadToolbars(void)
{
    qDebug("loadToolbars()");
    std::vector<std::pair<std::string, int>> order;
    for (const auto &iter : command_list) {
        order.push_back({iter.first, iter.second.toolbar_pos});
    }

    std::sort(order.begin(), order.end(), comp);

    for (const auto &iter : order) {
        Command command = command_list[iter.first];
        QString name = QString(command.toolbar_name.c_str());
        if (name.toUpper() != "NONE") {
            std::cout << qPrintable(name) << "  " << iter.first << "  " << iter.second << std::endl;

            /* If the toolbar doesn't exist, create it. */
            if (!toolbarHash.value(name)) {
                QToolBar* tb = new QToolBar(name, this);
                tb->setObjectName("toolbar" + name);
                connect(tb, SIGNAL(topLevelChanged(bool)), this,
                    SLOT(floatingChangedToolBar(bool)));
                addToolBar(Qt::LeftToolBarArea, tb);
                addToolBarBreak(Qt::LeftToolBarArea);
                toolbarHash.insert(name, tb);
            }

            toolbarHash.value(name)->addAction(command.action);
        }
    }
}

/* Populate Menus */
void
MainWindow::loadMenus(void)
{
    qDebug("loadMenus()");
    std::vector<std::pair<std::string, int>> order;
    for (const auto &iter : command_list) {
        order.push_back({iter.first, iter.second.menu_pos});
    }

    std::sort(order.begin(), order.end(), comp);

    for (const auto &iter : order) {
        Command command = command_list[iter.first];
        QString name = QString(command.menu_name.c_str());
        if (name.toUpper() != "NONE") {
            std::cout << "  " << iter.first << "  " << iter.second << std::endl;

            /* If the menu doesn't exist, create it. */
            if (!menuHash.value(name)) {
                QMenu* menu = new QMenu(name, this);
                menu->setTearOffEnabled(true);
                menuBar()->addMenu(menu);
                menuHash.insert(name, menu);
            }
            menuHash.value(name)->addAction(command.action);
        }
    }
}

