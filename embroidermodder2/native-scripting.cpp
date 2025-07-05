#include "native-scripting.h"

#include <iostream>
#include <algorithm>
#include <map>

MainWindow* _mainWin = 0;

/*!
 * \brief Initiate the MainWin pointer.
 *
 * \note qthelp://com.trolltech.qt.470/qdoc/scripting.html
 * \note Wrapping a Native Function
 * \note It is currently not possible to wrap member functions; i.e., methods of
 *       a C++ class that require a this object.
 */
void initMainWinPointer(MainWindow* mw)
{
    _mainWin = mw;
}

/*!
 * \brief mainWin
 */
MainWindow* mainWin()
{
    return _mainWin;
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
    mainWin()->nativeAlert(s);
}

void
blink_prompt(void)
{
    mainWin()->nativeBlinkPrompt();
}

void
set_prompt_prefix(const char *s)
{
    mainWin()->nativeSetPromptPrefix(s);
}

void
append_prompt_history(const char *s)
{
    mainWin()->nativeAppendPromptHistory(s);
}

void
enable_prompt_rapid_fire(void)
{
    mainWin()->nativeEnablePromptRapidFire();
}

void
disable_prompt_rapid_fire(void)
{
    mainWin()->nativeDisablePromptRapidFire();
}

void
init_command(void)
{
    mainWin()->nativeInitCommand();
}

void
end_command(void)
{
    mainWin()->nativeEndCommand();
}

void
enable_move_rapid_fire(void)
{
    mainWin()->nativeEnableMoveRapidFire();
}

void
disable_move_rapid_fire(void)
{
    mainWin()->nativeDisableMoveRapidFire();
}

void
new_file(void)
{
    mainWin()->nativeNewFile();
}

void
open_file(void)
{
    mainWin()->nativeOpenFile();
}

void
exit_program(void)
{
    mainWin()->nativeExit();
}

void
help(void)
{
    mainWin()->nativeHelp();
}

void
about(void)
{
    mainWin()->nativeAbout();
}

void
tip_of_the_day(void)
{
    mainWin()->nativeTipOfTheDay();
}

void
window_cascade(void)
{
    mainWin()->nativeWindowCascade();
}

void
window_tile(void)
{
    mainWin()->nativeWindowTile();
}

void
window_close(void)
{
    mainWin()->nativeWindowClose();
}

void
window_close_all(void)
{
    mainWin()->nativeWindowCloseAll();
}

void
window_next(void)
{
    mainWin()->nativeWindowNext();
}

void
window_previous(void)
{
    mainWin()->nativeWindowPrevious();
}

const char *
platform_string(void)
{
    return qPrintable(mainWin()->nativePlatformString());
}

void
message_box(const char *type, const char *title, const char *text)
{
    mainWin()->nativeMessageBox(type, title, text);
}

void
undo(void)
{
    mainWin()->nativeUndo();
}

void
redo(void)
{
    mainWin()->nativeRedo();
}

void
icon16(void)
{
    mainWin()->nativeIcon16();
}

void
icon24(void)
{
    mainWin()->nativeIcon24();
}

void
icon32(void)
{
    mainWin()->nativeIcon32();
}

void
icon48(void)
{
    mainWin()->nativeIcon48();
}

void
icon64(void)
{
    mainWin()->nativeIcon64();
}

void
icon128(void)
{
    mainWin()->nativeIcon128();
}

void
pan_left(void)
{
    mainWin()->nativePanLeft();
}

void
pan_right(void)
{
    mainWin()->nativePanRight();
}

void
pan_up(void)
{
    mainWin()->nativePanUp();
}

void
pan_down(void)
{
    mainWin()->nativePanDown();
}

void
zoom_in(void)
{
    mainWin()->nativeZoomIn();
}

void
zoom_out(void)
{
    mainWin()->nativeZoomOut();
}

void
zoom_extents(void)
{
    mainWin()->nativeZoomExtents();
}

void
print_area(double x, double y, double w, double h)
{
    mainWin()->nativePrintArea(x, y, w, h);
}

void
day(void)
{
    mainWin()->nativeDayVision();
}

void
night(void)
{
    mainWin()->nativeNightVision();
}

void
set_background_color(uint8_t r, uint8_t g, uint8_t b)
{
    mainWin()->nativeSetBackgroundColor(r, g, b);
}

void
set_crosshair_color(uint8_t r, uint8_t g, uint8_t b)
{
    mainWin()->nativeSetCrossHairColor(r, g, b);
}

void
set_grid_color(uint8_t r, uint8_t g, uint8_t b)
{
    mainWin()->nativeSetGridColor(r, g, b);
}

const char *
text_font(void)
{
    return qPrintable(mainWin()->nativeTextFont());
}

double
text_size(void)
{
    return mainWin()->nativeTextSize();
}

double
text_angle(void)
{
    return mainWin()->nativeTextAngle();
}

bool
text_bold(void)
{
    return mainWin()->nativeTextBold();
}

bool
text_italic(void)
{
    return mainWin()->nativeTextItalic();
}

bool
text_underline(void)
{
    return mainWin()->nativeTextUnderline();
}

bool
text_strikeout(void)
{
    return mainWin()->nativeTextStrikeOut();
}

bool
text_overline(void)
{
    return mainWin()->nativeTextOverline();
}

void
set_text_font(const char *s)
{
    mainWin()->nativeSetTextFont(s);
}

void
set_text_size(double size)
{
    mainWin()->nativeSetTextSize(size);
}

void
set_text_angle(double angle)
{
    mainWin()->nativeSetTextAngle(angle);
}

void
set_text_bold(bool bold)
{
    mainWin()->nativeSetTextBold(bold);
}

void
set_text_italic(bool italic)
{
    mainWin()->nativeSetTextItalic(italic);
}

void
set_text_underline(bool underline)
{
    mainWin()->nativeSetTextUnderline(underline);
}

void
set_text_strikeout(bool strikeout)
{
    mainWin()->nativeSetTextStrikeOut(strikeout);
}

void
set_text_overline(bool overline)
{
    mainWin()->nativeSetTextOverline(overline);
}

void
preview_on(int clone, int mode, double x, double y, double data)
{
    mainWin()->nativePreviewOn(clone, mode, x, y, data);
}

void
preview_off(void)
{
    mainWin()->nativePreviewOff();
}

void
vulcanize(void)
{
    mainWin()->nativeVulcanize();
}

void
clear_rubber(void)
{
    mainWin()->nativeClearRubber();
}

bool
allow_rubber(void)
{
    return mainWin()->nativeAllowRubber();
}

void
spare_rubber(int64_t id)
{
    mainWin()->nativeSpareRubber(id);
}

void
set_rubber_filter(int64_t id)
{
    //TODO: void nativeSetRubberFilter(qint64 id); //TODO: This is so more than 1 rubber object can exist at one time without updating all rubber objects at once
}

void
set_rubber_mode(int mode)
{
    mainWin()->nativeSetRubberMode(mode);
}

void
set_rubber_point(const char *key, double x, double y)
{
    mainWin()->nativeSetRubberPoint(key, x, y);
}

void
set_rubber_text(const char *key, const char *txt)
{
    mainWin()->nativeSetRubberText(key, txt);
}

void
add_text_multi(const char *s, double x, double y, double rot, bool fill, int rubberMode)
{
    mainWin()->nativeAddTextMulti(s, x, y, rot, fill, rubberMode);
}

void
add_text_single(const char *s, double x, double y, double rot, bool fill, int rubberMode)
{
    mainWin()->nativeAddTextSingle(s, x, y, rot, fill, rubberMode);
}

void
add_infinite_line(double x1, double y1, double x2, double y2, double rot)
{
    mainWin()->nativeAddInfiniteLine(x1, y1, x2, y2, rot);
}

void
add_ray(double x1, double y1, double x2, double y2, double rot)
{
    mainWin()->nativeAddRay(x1, y1, x2, y2, rot);
}

void
add_line(double x1, double y1, double x2, double y2, double rot, int rubberMode)
{
    mainWin()->nativeAddLine(x1, y1, x2, y2, rot, rubberMode);
}

void
add_triangle(double x1, double y1, double x2, double y2, double x3, double y3, double rot, bool fill)
{
    mainWin()->nativeAddTriangle(x1, y1, x2, y2, x3, y3, rot, fill);
}

void
add_rectangle(double x, double y, double w, double h, double rot, bool fill, int rubberMode)
{
    mainWin()->nativeAddRectangle(x, y, w, h, rot, fill, rubberMode);
}

void
add_rounded_rectangle(double x, double y, double w, double h, double rad, double rot, bool fill)
{
    mainWin()->nativeAddRoundedRectangle(x, y, w, h, rad, rot, fill);
}

void
add_arc(double startX, double startY, double midX, double midY, double endX, double endY, int rubberMode)
{
    mainWin()->nativeAddArc(startX, startY, midX, midY, endX, endY, rubberMode);
}

void
add_circle(double centerX, double centerY, double radius, bool fill, int rubberMode)
{
    mainWin()->nativeAddCircle(centerX, centerY, radius, fill, rubberMode);
}

void
add_slot(double centerX, double centerY, double diameter, double length, double rot, bool fill, int rubberMode)
{
    mainWin()->nativeAddSlot(centerX, centerY, diameter, length, rot, fill, rubberMode);
}

void
add_ellipse(double centerX, double centerY, double width, double height, double rot, bool fill, int rubberMode)
{
    mainWin()->nativeAddEllipse(centerX, centerY, width, height, rot, fill, rubberMode);
}

void
add_point(double x, double y)
{
    mainWin()->nativeAddPoint(x, y);
}

void
add_regular_polygon(double centerX, double centerY, int sides, int mode, double rad, double rot, bool fill)
{
    mainWin()->nativeAddRegularPolygon(centerX, centerY, sides, mode, rad, rot, fill);
}

void
add_polygon(double startX, double startY, int rubberMode)
{
    /*
    mainWin()->nativeAddPolygon(double startX, double startY, const QPainterPath& p, int rubberMode);
    */
}

void
add_polyline(double startX, double startY, int rubberMode)
{
    /*
    mainWin()->nativeAddPolyline(double startX, double startY, const QPainterPath& p, int rubberMode);
    */
}

void
add_path(double startX, double startY, int rubberMode)
{
    /*
    mainWin()->nativeAddPath(double startX, double startY, const QPainterPath& p, int rubberMode);
    */
}

void
add_horizontal_dimension(double x1, double y1, double x2, double y2, double legHeight)
{
    mainWin()->nativeAddHorizontalDimension(x1, y1, x2, y2, legHeight);
}

void
add_veritical_dimension(double x1, double y1, double x2, double y2, double legHeight)
{
    mainWin()->nativeAddVerticalDimension(x1, y1, x2, y2, legHeight);
}

void
add_image(const char *img, double x, double y, double w, double h, double rot)
{
    mainWin()->nativeAddImage(img, x, y, w, h, rot);
}

void
add_dimleader(double x1, double y1, double x2, double y2, double rot, int rubberMode)
{
    mainWin()->nativeAddDimLeader(x1, y1, x2, y2, rot, rubberMode);
}

void
set_cursor_shape(const char *shape)
{
    mainWin()-> nativeSetCursorShape(shape);
}

double
calculate_angle(double x1, double y1, double x2, double y2)
{
    return mainWin()->nativeCalculateAngle(x1, y1, x2, y2);
}

double
calculate_distance(double x1, double y1, double x2, double y2)
{
    return mainWin()->nativeCalculateDistance(x1, y1, x2, y2);
}

double
perpendicular_distance(double px, double py, double x1, double y1, double x2, double y2)
{
    return mainWin()->nativePerpendicularDistance(px, py, x1, y1, x2, y2);
}

int
num_selected(void)
{
    return mainWin()->nativeNumSelected();
}

void
select_all(void)
{
    mainWin()->nativeSelectAll();
}

void
add_to_selection(void)
{
    /*mainWin()->nativeAddToSelection(const QPainterPath path, Qt::ItemSelectionMode mode);*/
}

void
clear_selection(void)
{
    mainWin()->nativeClearSelection();
}

void
delete_selected(void)
{
    mainWin()->nativeDeleteSelected();
}

void
cut_selected(double x, double y)
{
    mainWin()->nativeCutSelected(x, y);
}

void
copy_selected(double x, double y)
{
    mainWin()->nativeCopySelected(x, y);
}

void
paste_selected(double x, double y)
{
    mainWin()->nativePasteSelected(x, y);
}

void
move_selected(double dx, double dy)
{
    mainWin()->nativeMoveSelected(dx, dy);
}

void
scale_selected(double x, double y, double factor)
{
    mainWin()->nativeScaleSelected(x, y, factor);
}

void
rotate_selected(double x, double y, double rot)
{
    mainWin()->nativeRotateSelected(x, y, rot);
}

void
mirror_selected(double x1, double y1, double x2, double y2)
{
    mainWin()->nativeMirrorSelected(x1, y1, x2, y2);
}

double
qsnapx(void)
{
    return mainWin()->nativeQSnapX();
}

double
qsnapy(void)
{
    return mainWin()->nativeQSnapY();
}

double
mousex(void)
{
    return mainWin()->nativeMouseX();
}

double
mousey(void)
{
    return mainWin()->nativeMouseY();
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
    initMainWinPointer(this);

    QString appDir = qApp->applicationDirPath();

    /* Load all command data in a loop. */
    QDir commandDir(appDir + "/commands");
    QStringList cmdList = commandDir.entryList(QDir::Dirs | QDir::NoDotAndDotDot);
    foreach(QString cmdName, cmdList) {
        Command command;
        QSettings settings(appDir + "/commands/" + cmdName + "/" + cmdName + ".ini",
            QSettings::IniFormat);
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

