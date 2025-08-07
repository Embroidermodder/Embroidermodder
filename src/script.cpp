/* Embroidermodder 2: SCRIPTING
 *
 * Copyright (C) 2011-2025 The Embroidermodder Team
 * Licensed under the terms of the zlib license.
 *
 * -----------------------------------------------------------------------------
 *
 * This is the most frequently updated part of the source and imports all
 * of the other headers. An alteration here has a faster turn around than
 * anywhere else in the compiled part of the source. New core developers would
 * benefit most from understanding the rough approach of this file and making
 * small changes here.
 *
 * Having scripting available for users to alter their software reflects a
 * core principle of open source software. The full build environment for
 * Embroidermodder is too complex for most users to set up, especially on
 * Windows. So the "freedom" to alter the program would be seldom used.
 * Being able to write a script in any text editor and add it to
 * the build by adding a load call to "commands.scm" is therefore a core
 * feature. We as core developers won't be using this flexibility much.
 *
 * For core developers, lua is providing a means of logging all user
 * interaction through a unified, text-based command system and a way of
 * allowing user made designs to be described parametrically along with their
 * custom UIs. While relying more on lua would make some interactions easier to
 * write like loading settings or writing out the state for debugging (see
 * `report_state_f`), going back and forth will be slower,
 * harder to debug and potentially will lead to invalid memory more often.
 * Having every `settings_` variable part of the global C/C++ state of the
 * program rather than as lua symbols has faster load times and easier to
 * access for over 99% of the code and run-time.
 *
 * TODO: inline all MainWindow native functions, replace MainWindow calls that
 * can be lua registerable functions.
 */

#include "embroidery.h"

#include "../extern/lua/src/lua.hpp"

#include "mainwindow.h"
#include "view.h"
#include "statusbar.h"
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
#include "property-editor.h"
#include "undo-editor.h"
#include "undo-commands.h"
#include "embdetails-dialog.h"

#include <stdlib.h>

#include <QAction>
#include <QApplication>
#include <QMdiArea>
#include <QLabel>
#include <QDesktopServices>
#include <QApplication>
#include <QUrl>
#include <QProcess>
#include <QDialogButtonBox>
#include <QPushButton>
#include <QMdiArea>
#include <QGraphicsScene>
#include <QComboBox>
#include <QWhatsThis>
#include <QDebug>
#include <QFrame>
#include <QVBoxLayout>
#include <QMenu>
#include <QMenuBar>
#include <QStatusBar>
#include <QMdiArea>
#include <QWidget>
#include <QMdiSubWindow>
#include <QMessageBox>
#include <QToolBar>
#include <QFileDialog>
#include <QApplication>
#include <QDate>
#include <QFileInfo>
#include <QLabel>
#include <QComboBox>
#include <QCloseEvent>
#include <QMetaObject>
#include <QLocale>

typedef struct Command_ {
    QString icon;
    QString command;
    QString tooltip;
    QString statustip;
    QString shortcut;
    QString macos;
    unsigned char checkable;
} Command;

typedef struct ScriptValue_ {
    QString s;
    int32_t i;
    double r;
    bool b;
} ScriptValue;

extern "C" {
    void temp_name(char *name, int *err);
}

/* NOTE: Try to keep this list alphabetical in the function name. */
int about_f(lua_State *L);
int add_to_menu_f(lua_State *L);
int add_toolbar_seperator_f(lua_State *L);
int add_to_selection_f(lua_State *L);
int add_to_toolbar_f(lua_State *L);
int alert_f(lua_State *L);
int allow_rubber_f(lua_State *L);
int arc_f(lua_State *L);
int append_prompt_history_f(lua_State *L);
int blink_f(lua_State *L);
int calculate_angle_f(lua_State *L);
int calculate_distance_f(lua_State *L);
int changelog_f(lua_State *L);
int circle_f(lua_State *L);
int clear_rubber_f(lua_State *L);
int clear_selection_f(lua_State *L);
int color_selector_f(lua_State *L);
int command_f(lua_State *L);
int copy_f(lua_State *L);
int copy_selected_f(lua_State *L);
int cut_f(lua_State *L);
int cut_selected_f(lua_State *L);
int day_f(lua_State *L);
int debug_f(lua_State *L);
int delete_selected_f(lua_State *L);
int design_details_f(lua_State *L);
int dim_leader_f(lua_State *L);
int disable_move_rapid_fire_f(lua_State *L);
int disable_prompt_rapid_fire_f(lua_State *L);
int distance_f(lua_State *L);
int dolphin_f(lua_State *L);
int do_nothing_f(lua_State *L);
int ellipse_f(lua_State *L);
int enable_move_rapid_fire_f(lua_State *L);
int enable_prompt_rapid_fire_f(lua_State *L);
int end_command_f(lua_State *L);
int erase_f(lua_State *L);
int error_f(lua_State *L);
int exit_program_f(lua_State *L);
int freeze_all_layers_f(lua_State *L);
int heart_f(lua_State *L);
int help_f(lua_State *L);
int hide_all_layers_f(lua_State *L);
int horizontal_dimension_f(lua_State *L);
int icon128_f(lua_State *L);
int icon16_f(lua_State *L);
int icon24_f(lua_State *L);
int icon32_f(lua_State *L);
int icon48_f(lua_State *L);
int icon64_f(lua_State *L);
int image_f(lua_State *L);
int infinite_line_f(lua_State *L);
int init_command_f(lua_State *L);
int layer_previous_f(lua_State *L);
int layer_selector_f(lua_State *L);
int layers_f(lua_State *L);
int line_f(lua_State *L);
int line_type_selector_f(lua_State *L);
int line_weight_selector_f(lua_State *L);
int locate_point_f(lua_State *L);
int lock_all_layers_f(lua_State *L);
int make_layer_current_f(lua_State *L);
int menu_seperator_f(lua_State *L);
int messagebox_f(lua_State *L);
int mirror_selected_f(lua_State *L);
int mouse_f(lua_State *L);
int move_f(lua_State *L);
int move_selected_f(lua_State *L);
int new_file_f(lua_State *L);
int night_vision_f(lua_State *L);
int num_selected_f(lua_State *L);
int open_file_f(lua_State *L);
int pan_down_f(lua_State *L);
int pan_left_f(lua_State *L);
int pan_point_f(lua_State *L);
int pan_real_time_f(lua_State *L);
int pan_right_f(lua_State *L);
int pan_up_f(lua_State *L);
int paste_f(lua_State *L);
int paste_selected_f(lua_State *L);
int path_f(lua_State *L);
int perpendicular_distance_f(lua_State *L);
int platform_f(lua_State *L);
int point_f(lua_State *L);
int polygon_f(lua_State *L);
int polyline_f(lua_State *L);
int preview_off_f(lua_State *L);
int preview_on_f(lua_State *L);
int print_area_f(lua_State *L);
int print_f(lua_State *L);
int qsnap_f(lua_State *L);
int quickleader_f(lua_State *L);
int ray_f(lua_State *L);
int rectangle_f(lua_State *L);
int redo_f(lua_State *L);
int regular_polygon_f(lua_State *L);
int rgb_f(lua_State *L);
int rotate_f(lua_State *L);
int rotate_selected_f(lua_State *L);
int rounded_rectangle_f(lua_State *L);
int rubber_f(lua_State *L);
int saveas_f(lua_State *L);
int save_f(lua_State *L);
int scale_f(lua_State *L);
int scale_selected_f(lua_State *L);
int select_all_f(lua_State *L);
int selectall_f(lua_State *L);
int set_color_f(lua_State *L);
int set_cursor_shape_f(lua_State *L);
int set_prompt_prefix_f(lua_State *L);
int set_rubber_mode_f(lua_State *L);
int set_rubber_point_f(lua_State *L);
int set_rubber_text_f(lua_State *L);
int set_text_angle_f(lua_State *L);
int set_text_bold_f(lua_State *L);
int set_text_font_f(lua_State *L);
int set_text_italic_f(lua_State *L);
int set_text_overline_f(lua_State *L);
int set_text_size_f(lua_State *L);
int set_text_strikeout_f(lua_State *L);
int set_text_underline_f(lua_State *L);
int settings_dialog_f(lua_State *L);
int show_all_layers_f(lua_State *L);
int singlelinetext_f(lua_State *L);
int slot_f(lua_State *L);
int snowflake_f(lua_State *L);
int spare_rubber_f(lua_State *L);
int star_f(lua_State *L);
int syswindows_f(lua_State *L);
int text_angle_f(lua_State *L);
int text_bold_f(lua_State *L);
int text_font_f(lua_State *L);
int text_italic_f(lua_State *L);
int text_multi_f(lua_State *L);
int text_overline_f(lua_State *L);
int text_single_f(lua_State *L);
int text_size_f(lua_State *L);
int text_strikeout_f(lua_State *L);
int text_underline_f(lua_State *L);
int thaw_all_layers_f(lua_State *L);
int tip_of_the_day_f(lua_State *L);
int todo_f(lua_State *L);
int triangle_f(lua_State *L);
int undo_f(lua_State *L);
int unlock_all_layers_f(lua_State *L);
int vertical_dimension_f(lua_State *L);
int vulcanize_f(lua_State *L);
int whats_this_f(lua_State *L);
int window_cascade_f(lua_State *L);
int window_close_all_f(lua_State *L);
int window_close_f(lua_State *L);
int window_next_f(lua_State *L);
int window_previous_f(lua_State *L);
int window_tile_f(lua_State *L);
int zoom_all_f(lua_State *L);
int zoom_center_f(lua_State *L);
int zoom_dynamic_f(lua_State *L);
int zoom_extents_f(lua_State *L);
int zoom_in_f(lua_State *L);
int zoom_out_f(lua_State *L);
int zoom_previous_f(lua_State *L);
int zoom_real_time_f(lua_State *L);
int zoom_scale_f(lua_State *L);
int zoom_selected_f(lua_State *L);
int zoom_window_f(lua_State *L);

void init_command(void);
void clear_selection(void);
void end_command(void);

/* ---- State ------------------------------------------------------------------
 *
 * TODO: Set defaults for all state variables.
 */
lua_State *Lua;
unsigned char context_flag = CONTEXT_MAIN;

Settings settings;

MainWindow* _mainWin = 0;

const char *temporary_name_format = "tmp_%d";
int temporary_name = 0;

luaL_Reg lua_registerables[] = {
    {"about", about_f},
    {"add_to_menu", add_to_menu_f},
    {"add_toolbar_seperator", add_toolbar_seperator_f},
    {"add_to_selection", add_to_selection_f},
    {"add_to_toolbar", add_to_toolbar_f},
    {"alert", alert_f},
    {"allow_rubber", allow_rubber_f},
    {"arc", arc_f},
    {"append_prompt_history", append_prompt_history_f},
    {"blink", blink_f},
    {"calculate_angle", calculate_angle_f},
    {"calculate_distance", calculate_distance_f},
    {"changelog", changelog_f},
    {"circle", circle_f},
    {"clear_rubber", clear_rubber_f},
    {"clear_selection", clear_selection_f},
    {"color_selector", color_selector_f},
    {"command", command_f},
    {"copy", copy_f},
    {"copy_selected", copy_selected_f},
    {"cut", cut_f},
    {"cut_selected", cut_selected_f},
    {"day", day_f},
    {"debug", debug_f},
    {"delete_selected", delete_selected_f},
    {"design_details", design_details_f},
    {"dim_leader", dim_leader_f},
    {"disable_move_rapid_fire", disable_move_rapid_fire_f},
    {"disable_prompt_rapid_fire", disable_prompt_rapid_fire_f},
    {"distance", distance_f},
    {"dolphin", dolphin_f},
    {"do_nothing", do_nothing_f},
    {"ellipse", ellipse_f},
    {"enable_move_rapid_fire", enable_move_rapid_fire_f},
    {"enable_prompt_rapid_fire", enable_prompt_rapid_fire_f},
    {"end_command", end_command_f},
    {"erase", erase_f},
    {"error", error_f},
    {"exit_program", exit_program_f},
    {"freeze_all_layers", freeze_all_layers_f},
    {"heart", heart_f},
    {"help", help_f},
    {"hide_all_layers", hide_all_layers_f},
    {"horizontal_dimension", horizontal_dimension_f},
    {"icon128", icon128_f},
    {"icon16", icon16_f},
    {"icon24", icon24_f},
    {"icon32", icon32_f},
    {"icon48", icon48_f},
    {"icon64", icon64_f},
    {"image", image_f},
    {"infinite_line", infinite_line_f},
    {"init_command", init_command_f},
    {"layer_previous", layer_previous_f},
    {"layer_selector", layer_selector_f},
    {"layers", layers_f},
    {"line", line_f},
    {"line_type_selector", line_type_selector_f},
    {"line_weight_selector", line_weight_selector_f},
    {"locate_point", locate_point_f},
    {"lock_all_layers", lock_all_layers_f},
    {"make_layer_current", make_layer_current_f},
    {"menu_seperator", menu_seperator_f},
    {"messagebox", messagebox_f},
    {"mirror_selected", mirror_selected_f},
    {"mouse", mouse_f},
    {"move", move_f},
    {"move_selected", move_selected_f},
    {"new_file", new_file_f},
    {"night_vision", night_vision_f},
    {"num_selected", num_selected_f},
    {"open_file", open_file_f},
    {"pan_down", pan_down_f},
    {"pan_left", pan_left_f},
    {"pan_point", pan_point_f},
    {"pan_real_time", pan_real_time_f},
    {"pan_right", pan_right_f},
    {"pan_up", pan_up_f},
    {"paste", paste_f},
    {"paste_selected", paste_selected_f},
    {"path", path_f},
    {"perpendicular_distance", perpendicular_distance_f},
    {"platform", platform_f},
    {"point", point_f},
    {"polygon", polygon_f},
    {"polyline", polyline_f},
    {"preview_off", preview_off_f},
    {"preview_on", preview_on_f},
    {"print_area", print_area_f},
    {"print", print_f},
    {"qsnap", qsnap_f},
    {"quickleader", quickleader_f},
    {"ray", ray_f},
    {"rectangle", rectangle_f},
    {"redo", redo_f},
    {"regular_polygon", regular_polygon_f},
    {"rgb", rgb_f},
    {"rotate", rotate_f},
    {"rotate_selected", rotate_selected_f},
    {"rounded_rectangle", rounded_rectangle_f},
    {"rubber", rubber_f},
    {"saveas", saveas_f},
    {"save", save_f},
    {"scale", scale_f},
    {"scale_selected", scale_selected_f},
    {"select_all", select_all_f},
    {"selectall", selectall_f},
    {"set_color", set_color_f},
    {"set_cursor_shape", set_cursor_shape_f},
    {"set_prompt_prefix", set_prompt_prefix_f},
    {"set_rubber_mode", set_rubber_mode_f},
    {"set_rubber_point", set_rubber_point_f},
    {"set_rubber_text", set_rubber_text_f},
    {"set_text_angle", set_text_angle_f},
    {"set_text_bold", set_text_bold_f},
    {"set_text_font", set_text_font_f},
    {"set_text_italic", set_text_italic_f},
    {"set_text_overline", set_text_overline_f},
    {"set_text_size", set_text_size_f},
    {"set_text_strikeout", set_text_strikeout_f},
    {"set_text_underline", set_text_underline_f},
    {"settings_dialog", settings_dialog_f},
    {"show_all_layers", show_all_layers_f},
    {"singlelinetext", singlelinetext_f},
    {"slot", slot_f},
    {"snowflake", snowflake_f},
    {"spare_rubber", spare_rubber_f},
    {"star", star_f},
    {"syswindows", syswindows_f},
    {"text_angle", text_angle_f},
    {"text_bold", text_bold_f},
    {"text_font", text_font_f},
    {"text_italic", text_italic_f},
    {"text_multi", text_multi_f},
    {"text_overline", text_overline_f},
    {"text_single", text_single_f},
    {"text_size", text_size_f},
    {"text_strikeout", text_strikeout_f},
    {"text_underline", text_underline_f},
    {"thaw_all_layers", thaw_all_layers_f},
    {"tip_of_the_day", tip_of_the_day_f},
    {"todo", todo_f},
    {"triangle", triangle_f},
    {"undo", undo_f},
    {"unlock_all_layers", unlock_all_layers_f},
    {"vertical_dimension", vertical_dimension_f},
    {"vulcanize", vulcanize_f},
    {"whats_this", whats_this_f},
    {"window_cascade", window_cascade_f},
    {"window_close_all", window_close_all_f},
    {"window_close", window_close_f},
    {"window_next", window_next_f},
    {"window_previous", window_previous_f},
    {"window_tile", window_tile_f},
    {"zoom_all", zoom_all_f},
    {"zoom_center", zoom_center_f},
    {"zoom_dynamic", zoom_dynamic_f},
    {"zoom_extents", zoom_extents_f},
    {"zoom_in", zoom_in_f},
    {"zoom_out", zoom_out_f},
    {"zoom_previous", zoom_previous_f},
    {"zoom_real_time", zoom_real_time_f},
    {"zoom_scale", zoom_scale_f},
    {"zoom_selected", zoom_selected_f},
    {"zoom_window", zoom_window_f},
    {NULL, NULL}
};

std::vector<Command> command_map;

int check_clear_top = 0;

void debug(const char *msg);
void clear_top_check(lua_State *L, int expected);
void no_args(lua_State *L);
int unpack_args(lua_State *L, const char *function, ScriptValue *args, const char *type_string);

/* This doesn't block, it just warns the user about excess symbols. */
void
clear_top_check(lua_State *L, int expected)
{
    if (!check_clear_top) {
        return;
    }
    if (lua_gettop(L) > expected) {
        debug("WARNING: running in CLEAR TOP mode, embroidermodder expects");
        debug("WARNING: no extra symbols to be on top during a function call");
        debug("WARNING: and a function was called requiring no arguments");
    }
}

void
no_args(lua_State *L)
{
    clear_top_check(L, 0);
}

/* Unpacks the lua state according to the type_string where the possible
 * types are:
 *
 * | char | C++ type  | description                            |
 * |------|-----------|----------------------------------------|
 * | `r`  | `double`  | Real number: checks for NaN by default |
 * | `i`  | `int32_t` | 32-bit signed integer                  |
 * | `b`  | `bool`    | Boolean: true or false.                |
 * | `s`  | `QString` | A string TODO: check for type          |
 *
 * TODO: better type checking
 *
 * Note the the lua_to* calls add one to account for lua's indexing from 1.
 */
int
unpack_args(lua_State *L, const char *function, ScriptValue *args, const char *type_string)
{
    int expected = strlen(type_string);
    if (lua_gettop(L) < expected) {
        return 0;
    }
    /* This doesn't block, it just warns the user about excess symbols. */
    clear_top_check(L, expected);

    for (int i=0; type_string[i]; i++) {
        switch (type_string[i]) {
        case 'r': {
            args[i].r = lua_tonumber(L, i+1);
            if (std::isnan(args[i].r)) {
                char message[100];
                sprintf(message, "ERROR: %s argument %d is not a number.",
                   function, i+1);
                debug((const char*)message);
                return 0;
            }
            break;
        }
        case 'i': {
            args[i].i = lua_tointeger(L, i+1);
            break;
        }
        case 's': {
            args[i].s = lua_tostring(L, i+1);
            break;
        }
        case 'b': {
            args[i].b = lua_toboolean(L, i+1);
            break;
        }
        default:
            debug("ERROR: invalid character passed to type_string in arg_check");
            break;
        }
    }
    return 1;
}

/* Convenience function to simplify unpacking vectors supplied as
 * neighboring arguments. Inverts the y-axis because generally that is
 * needed: if you need to override this, use the `no_y_invert` version.
 */
EmbVector
unpack_vector(ScriptValue *args, int offset)
{
    return emb_vector(args[offset].r, -args[offset+1].r);
}

EmbVector
unpack_vector_no_y_invert(ScriptValue *args, int offset)
{
    return emb_vector(args[offset].r, args[offset+1].r);
}

/* A version which doesn't produce the same error message.
 * TODO: This needs to be checked for whether it functions similarly.
 */
void
temp_name(char *name, int *err)
{
    FILE *f = 0;
    *err = 0;
    while (!f) {
        sprintf(name, temporary_name_format, temporary_name);
        f = fopen(name, "w");
        temporary_name++;
    }
    fclose(f);
}

void
stub_implement(QString txt)
{
    qDebug("TODO: %s", qPrintable(txt));
}

void MainWindow::stub_testing()
{
    QMessageBox::warning(this, tr("Testing Feature"), tr("<b>This feature is in testing.</b>"));
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
    if(gview) { gview->cut(); }
}

void MainWindow::copy()
{
    qDebug("copy()");
    View* gview = activeView();
    if(gview) { gview->copy(); }
}

void MainWindow::paste()
{
    qDebug("paste()");
    View* gview = activeView();
    if(gview) { gview->paste(); }
}

void MainWindow::selectAll()
{
    qDebug("selectAll()");
    View* gview = activeView();
    if(gview) { gview->selectAll(); }
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
    if (scene) {
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
    if (mdiWin) {
        mdiWin->print();
    }
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

    if(settings.general_current_tip >= listTipOfTheDay.size())
        settings.general_current_tip = 0;
    labelTipOfTheDay = new QLabel(listTipOfTheDay.value(settings.general_current_tip), wizardTipOfTheDay);
    labelTipOfTheDay->setWordWrap(true);

    QCheckBox* checkBoxTipOfTheDay = new QCheckBox(tr("&Show tips on startup"), wizardTipOfTheDay);
    settings.general_tip_of_the_day = settings.general_tip_of_the_day;
    checkBoxTipOfTheDay->setChecked(settings.general_tip_of_the_day);
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
    settings.general_tip_of_the_day = checked;
}

void MainWindow::buttonTipOfTheDayClicked(int button)
{
    qDebug("buttonTipOfTheDayClicked(%d)", button);
    if(button == QWizard::CustomButton1)
    {
        if(settings.general_current_tip > 0)
            settings.general_current_tip--;
        else
            settings.general_current_tip = listTipOfTheDay.size()-1;
        labelTipOfTheDay->setText(listTipOfTheDay.value(settings.general_current_tip));
    }
    else if(button == QWizard::CustomButton2)
    {
        settings.general_current_tip++;
        if(settings.general_current_tip >= listTipOfTheDay.size())
            settings.general_current_tip = 0;
        labelTipOfTheDay->setText(listTipOfTheDay.value(settings.general_current_tip));
    }
    else if(button == QWizard::CustomButton3)
    {
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
bool MainWindow::isShiftPressed()
{
    return shiftKeyPressedState;
}

void MainWindow::setShiftPressed()
{
    shiftKeyPressedState = true;
}

void MainWindow::setShiftReleased()
{
    shiftKeyPressedState = false;
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

    settings.general_icon_size = iconSize;
}

MdiWindow* MainWindow::activeMdiWindow()
{
    qDebug("activeMdiWindow()");
    MdiWindow* mdiWin = qobject_cast<MdiWindow*>(mdiArea->activeSubWindow());
    return mdiWin;
}

View* MainWindow::activeView()
{
    qDebug("activeView()");
    MdiWindow* mdiWin = qobject_cast<MdiWindow*>(mdiArea->activeSubWindow());
    if(mdiWin)
    {
        View* v = mdiWin->getView();
        return v;
    }
    return 0;
}

QGraphicsScene* MainWindow::activeScene()
{
    qDebug("activeScene()");
    MdiWindow* mdiWin = qobject_cast<MdiWindow*>(mdiArea->activeSubWindow());
    if(mdiWin)
    {
        QGraphicsScene* s = mdiWin->getScene();
        return s;
    }
    return 0;
}

QUndoStack* MainWindow::activeUndoStack()
{
    qDebug("activeUndoStack()");
    View* v = activeView();
    if(v)
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
    for(int i = 0; i < windowList.count(); ++i)
    {
        MdiWindow* mdiWin = qobject_cast<MdiWindow*>(windowList.at(i));
        if(mdiWin) { mdiWin->showViewScrollBars(val); }
    }
}

void MainWindow::updateAllViewCrossHairColors(QRgb color)
{
    QList<QMdiSubWindow*> windowList = mdiArea->subWindowList();
    for(int i = 0; i < windowList.count(); ++i)
    {
        MdiWindow* mdiWin = qobject_cast<MdiWindow*>(windowList.at(i));
        if(mdiWin) { mdiWin->setViewCrossHairColor(color); }
    }
}

void MainWindow::updateAllViewBackgroundColors(QRgb color)
{
    QList<QMdiSubWindow*> windowList = mdiArea->subWindowList();
    for(int i = 0; i < windowList.count(); ++i)
    {
        MdiWindow* mdiWin = qobject_cast<MdiWindow*>(windowList.at(i));
        if(mdiWin) { mdiWin->setViewBackgroundColor(color); }
    }
}

void MainWindow::updateAllViewSelectBoxColors(QRgb colorL, QRgb fillL, QRgb colorR, QRgb fillR, int alpha)
{
    QList<QMdiSubWindow*> windowList = mdiArea->subWindowList();
    for(int i = 0; i < windowList.count(); ++i)
    {
        MdiWindow* mdiWin = qobject_cast<MdiWindow*>(windowList.at(i));
        if(mdiWin) { mdiWin->setViewSelectBoxColors(colorL, fillL, colorR, fillR, alpha); }
    }
}

void MainWindow::updateAllViewGridColors(QRgb color)
{
    QList<QMdiSubWindow*> windowList = mdiArea->subWindowList();
    for(int i = 0; i < windowList.count(); ++i)
    {
        MdiWindow* mdiWin = qobject_cast<MdiWindow*>(windowList.at(i));
        if(mdiWin) { mdiWin->setViewGridColor(color); }
    }
}

void MainWindow::updateAllViewRulerColors(QRgb color)
{
    QList<QMdiSubWindow*> windowList = mdiArea->subWindowList();
    for(int i = 0; i < windowList.count(); ++i)
    {
        MdiWindow* mdiWin = qobject_cast<MdiWindow*>(windowList.at(i));
        if(mdiWin) { mdiWin->setViewRulerColor(color); }
    }
}

void MainWindow::updatePickAddMode(bool val)
{
    settings.selection_mode_pickadd = val;
    dockPropEdit->updatePickAddModeButton(val);
}

void MainWindow::pickAddModeToggled()
{
    bool val = !settings.selection_mode_pickadd;
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

void MainWindow::layerSelectorIndexChanged(int index)
{
    qDebug("layerSelectorIndexChanged(%d)", index);
}

void MainWindow::colorSelectorIndexChanged(int index)
{
    qDebug("colorSelectorIndexChanged(%d)", index);

    QComboBox* comboBox = qobject_cast<QComboBox*>(sender());
    QRgb newColor;
    if(comboBox)
    {
        bool ok = 0;
        //TODO: Handle ByLayer and ByBlock and Other...
        newColor = comboBox->itemData(index).toUInt(&ok);
        if(!ok)
            QMessageBox::warning(this, tr("Color Selector Conversion Error"), tr("<b>An error has occured while changing colors.</b>"));
    }
    else
        QMessageBox::warning(this, tr("Color Selector Pointer Error"), tr("<b>An error has occured while changing colors.</b>"));

    MdiWindow* mdiWin = qobject_cast<MdiWindow*>(mdiArea->activeSubWindow());
    if(mdiWin) { mdiWin->currentColorChanged(newColor); }
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
    char command[200];
    sprintf(command, "set_text_font(%s)", qPrintable(font.family()));
    run(command);
}

// TODO: check that the toReal() conversion is ok
void MainWindow::textSizeSelectorIndexChanged(int index)
{
    qDebug("textSizeSelectorIndexChanged(%d)", index);
    settings.text_size = qFabs(textSizeSelector->itemData(index).toReal());
}

QString MainWindow::getCurrentLayer()
{
    MdiWindow* mdiWin = qobject_cast<MdiWindow*>(mdiArea->activeSubWindow());
    if(mdiWin) { return mdiWin->getCurrentLayer(); }
    return "0";
}

QRgb MainWindow::getCurrentColor()
{
    MdiWindow* mdiWin = qobject_cast<MdiWindow*>(mdiArea->activeSubWindow());
    if(mdiWin) { return mdiWin->getCurrentColor(); }
    return 0; //TODO: return color ByLayer
}

QString MainWindow::getCurrentLineType()
{
    MdiWindow* mdiWin = qobject_cast<MdiWindow*>(mdiArea->activeSubWindow());
    if(mdiWin) { return mdiWin->getCurrentLineType(); }
    return "ByLayer";
}

QString MainWindow::getCurrentLineWeight()
{
    MdiWindow* mdiWin = qobject_cast<MdiWindow*>(mdiArea->activeSubWindow());
    if(mdiWin) { return mdiWin->getCurrentLineWeight(); }
    return "ByLayer";
}

void MainWindow::deletePressed()
{
    qDebug("deletePressed()");
    QApplication::setOverrideCursor(Qt::WaitCursor);
    MdiWindow* mdiWin = qobject_cast<MdiWindow*>(mdiArea->activeSubWindow());
    if(mdiWin) { mdiWin->deletePressed(); }
    QApplication::restoreOverrideCursor();
}

void MainWindow::escapePressed()
{
    qDebug("escapePressed()");
    QApplication::setOverrideCursor(Qt::WaitCursor);
    MdiWindow* mdiWin = qobject_cast<MdiWindow*>(mdiArea->activeSubWindow());
    if(mdiWin) { mdiWin->escapePressed(); }
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

void MainWindow::promptHistoryAppended(const QString& txt)
{
    MdiWindow* mdiWin = activeMdiWindow();
    if(mdiWin) mdiWin->promptHistoryAppended(txt);
}

void MainWindow::logPromptInput(const QString& txt)
{
    MdiWindow* mdiWin = activeMdiWindow();
    if(mdiWin) mdiWin->logPromptInput(txt);
}

void MainWindow::promptInputPrevious()
{
    MdiWindow* mdiWin = activeMdiWindow();
    if(mdiWin) mdiWin->promptInputPrevious();
}

void MainWindow::promptInputNext()
{
    MdiWindow* mdiWin = activeMdiWindow();
    if(mdiWin) mdiWin->promptInputNext();
}

void
run(const char *command)
{
    _mainWin->prompt->appendHistory(command);
    luaL_dostring(Lua, command);
}

void MainWindow::runCommand()
{
    QAction* act = qobject_cast<QAction*>(sender());
    if (act) {
        qDebug("runCommand(%s)", qPrintable(act->objectName()));
        prompt->endCommand();
        prompt->setCurrentText(act->objectName());
        prompt->processInput();
    }
}

void
MainWindow::runCommandMain(const QString& cmd)
{
    qDebug("runCommandMain(%s)", qPrintable(cmd));
    // TODO: Uncomment this line when post-selection is available
    // if (!settings.selection_mode_pick_first) {
    //     clear_selection();
    // }
    context_flag = CONTEXT_MAIN;
    run(qPrintable(cmd + "()"));
}

void
MainWindow::runCommandClick(const QString& cmd, qreal x, qreal y)
{
    qDebug("runCommandClick(%s, %.2f, %.2f)", qPrintable(cmd), x, y);
    char mouse_pos[100];
    sprintf(mouse_pos, "mouse = {%f, %f}", x, y);
    context_flag = CONTEXT_CLICK;
    luaL_dostring(Lua, mouse_pos);
    run(qPrintable(cmd + "()"));
}

void
MainWindow::runCommandMove(const QString& cmd, qreal x, qreal y)
{
    qDebug("runCommandMove(%s, %.2f, %.2f)", qPrintable(cmd), x, y);
    char mouse_pos[100];
    sprintf(mouse_pos, "mouse = {%f, %f}", x, y);
    context_flag = CONTEXT_MOVE;
    luaL_dostring(Lua, mouse_pos);
    run(qPrintable(cmd + "()"));
}

void
MainWindow::runCommandContext(const QString& cmd, const QString& str)
{
    qDebug("runCommandContext(%s, %s)", qPrintable(cmd), qPrintable(str));
    char context_str[100];
    sprintf(context_str, "prompt = \"%s\"", qPrintable(str));
    context_flag = CONTEXT_CONTEXT;
    luaL_dostring(Lua, context_str);
    run(qPrintable(cmd + "()"));
}

void MainWindow::runCommandPrompt(const QString& cmd, const QString& str)
{
    qDebug("runCommandPrompt(%s, %s)", qPrintable(cmd), qPrintable(str));
    char prompt[100];
    sprintf(prompt, "(define prompt \"%s\")", qPrintable(str));
    QString safeStr = str;
    safeStr.replace("\\", "\\\\");
    safeStr.replace("\'", "\\\'");
    /*
    if (prompt->isRapidFireEnabled()) {
        engine->evaluate(cmd + "_prompt('" + safeStr + "')", fileName);
    }
    else {
        engine->evaluate(cmd + "_prompt('" + safeStr.toUpper() + "')", fileName);
    }
    */
    context_flag = CONTEXT_PROMPT;
    luaL_dostring(Lua, prompt);
    run(qPrintable(cmd + "()"));
}

void MainWindow::nativeMessageBox(const QString& type, const QString& title, const QString& text)
{
    QString msgType = type.toLower();
    if     (msgType == "critical")    { QMessageBox::critical   (this, tr(qPrintable(title)), tr(qPrintable(text))); }
    else if(msgType == "information") { QMessageBox::information(this, tr(qPrintable(title)), tr(qPrintable(text))); }
    else if(msgType == "question")    { QMessageBox::question   (this, tr(qPrintable(title)), tr(qPrintable(text))); }
    else if(msgType == "warning")     { QMessageBox::warning    (this, tr(qPrintable(title)), tr(qPrintable(text))); }
    else                              { QMessageBox::critical   (this, tr("Native MessageBox Error"), tr("Incorrect use of the native messageBox function.")); }
}

void MainWindow::nativeSpareRubber(qint64 id)
{
    View* gview = activeView();
    if(gview) gview->spareRubber(id);
}

void MainWindow::nativeSetRubberMode(int mode)
{
    View* gview = activeView();
    if(gview) gview->setRubberMode(mode);
}

void MainWindow::nativeAddTextMulti(const QString& str, qreal x, qreal y, qreal rot, bool fill, int rubberMode)
{
}

void MainWindow::nativeAddTextSingle(const QString& str, qreal x, qreal y, qreal rot, bool fill, int rubberMode)
{
    View* gview = activeView();
    QGraphicsScene* gscene = gview->scene();
    QUndoStack* stack = gview->getUndoStack();
    if(gview && gscene && stack)
    {
        TextSingleObject* obj = new TextSingleObject(str, x, -y, getCurrentColor());
        obj->setObjectTextFont(settings.text_font);
        obj->setObjectTextSize(settings.text_size);
        obj->setObjectTextStyle(settings.text_style_bold,
                                settings.text_style_italic,
                                settings.text_style_underline,
                                settings.text_style_strikeout,
                                settings.text_style_overline);
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

void MainWindow::nativeAddInfiniteLine(qreal x1, qreal y1, qreal x2, qreal y2, qreal rot)
{
}

void MainWindow::nativeAddRay(qreal x1, qreal y1, qreal x2, qreal y2, qreal rot)
{
}

void MainWindow::nativeAddLine(qreal x1, qreal y1, qreal x2, qreal y2, qreal rot, int rubberMode)
{
    View* gview = activeView();
    QGraphicsScene* gscene = gview->scene();
    QUndoStack* stack = gview->getUndoStack();
    if(gview && gscene && stack)
    {
        LineObject* obj = new LineObject(x1, -y1, x2, -y2, getCurrentColor());
        obj->setRotation(-rot);
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

void MainWindow::nativeAddTriangle(qreal x1, qreal y1, qreal x2, qreal y2, qreal x3, qreal y3, qreal rot, bool fill)
{
}

void MainWindow::nativeAddRectangle(qreal x, qreal y, qreal w, qreal h, qreal rot, bool fill, int rubberMode)
{
    View* gview = activeView();
    QGraphicsScene* gscene = gview->scene();
    QUndoStack* stack = gview->getUndoStack();
    if(gview && gscene && stack)
    {
        RectObject* obj = new RectObject(x, -y, w, -h, getCurrentColor());
        obj->setRotation(-rot);
        obj->setObjectRubberMode(rubberMode);
        //TODO: rect fill
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

void MainWindow::nativeAddRoundedRectangle(qreal x, qreal y, qreal w, qreal h, qreal rad, qreal rot, bool fill)
{
}

void MainWindow::nativeAddArc(qreal startX, qreal startY, qreal midX, qreal midY, qreal endX, qreal endY, int rubberMode)
{
    View* gview = activeView();
    QGraphicsScene* scene = activeScene();
    if(gview && scene)
    {
        ArcObject* arcObj = new ArcObject(startX, -startY, midX, -midY, endX, -endY, getCurrentColor());
        arcObj->setObjectRubberMode(rubberMode);
        if(rubberMode) gview->addToRubberRoom(arcObj);
        scene->addItem(arcObj);
        scene->update();
    }
}

void MainWindow::nativeAddSlot(qreal centerX, qreal centerY, qreal diameter, qreal length, qreal rot, bool fill, int rubberMode)
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

void MainWindow::nativeAddEllipse(qreal centerX, qreal centerY, qreal width, qreal height, qreal rot, bool fill, int rubberMode)
{
    View* gview = activeView();
    QGraphicsScene* gscene = gview->scene();
    QUndoStack* stack = gview->getUndoStack();
    if(gview && gscene && stack)
    {
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

void MainWindow::nativeAddPoint(qreal x, qreal y)
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

void MainWindow::nativeAddRegularPolygon(qreal centerX, qreal centerY, quint16 sides, quint8 mode, qreal rad, qreal rot, bool fill)
{
}

//NOTE: This native is different than the rest in that the Y+ is down (scripters need not worry about this)
void MainWindow::nativeAddPolygon(qreal startX, qreal startY, const QPainterPath& p, int rubberMode)
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
void MainWindow::nativeAddPolyline(qreal startX, qreal startY, const QPainterPath& p, int rubberMode)
{
    View* gview = activeView();
    QGraphicsScene* gscene = gview->scene();
    QUndoStack* stack = gview->getUndoStack();
    if(gview && gscene && stack)
    {
        PolylineObject* obj = new PolylineObject(startX, startY, p, getCurrentColor());
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
void MainWindow::nativeAddPath(qreal startX, qreal startY, const QPainterPath& p, int rubberMode)
{
}

void MainWindow::nativeAddHorizontalDimension(qreal x1, qreal y1, qreal x2, qreal y2, qreal legHeight)
{
}

void MainWindow::nativeAddVerticalDimension(qreal x1, qreal y1, qreal x2, qreal y2, qreal legHeight)
{
}

void MainWindow::nativeAddImage(const QString& img, qreal x, qreal y, qreal w, qreal h, qreal rot)
{
}

void MainWindow::nativeSetCursorShape(const QString& str)
{
    View* gview = activeView();
    if(gview)
    {
        QString shape = str.toLower();
        if     (shape == "arrow")           gview->setCursor(QCursor(Qt::ArrowCursor));
        else if(shape == "uparrow")         gview->setCursor(QCursor(Qt::UpArrowCursor));
        else if(shape == "cross")           gview->setCursor(QCursor(Qt::CrossCursor));
        else if(shape == "wait")            gview->setCursor(QCursor(Qt::WaitCursor));
        else if(shape == "ibeam")           gview->setCursor(QCursor(Qt::IBeamCursor));
        else if(shape == "resizevert")      gview->setCursor(QCursor(Qt::SizeVerCursor));
        else if(shape == "resizehoriz")     gview->setCursor(QCursor(Qt::SizeHorCursor));
        else if(shape == "resizediagleft")  gview->setCursor(QCursor(Qt::SizeBDiagCursor));
        else if(shape == "resizediagright") gview->setCursor(QCursor(Qt::SizeFDiagCursor));
        else if(shape == "move")            gview->setCursor(QCursor(Qt::SizeAllCursor));
        else if(shape == "blank")           gview->setCursor(QCursor(Qt::BlankCursor));
        else if(shape == "splitvert")       gview->setCursor(QCursor(Qt::SplitVCursor));
        else if(shape == "splithoriz")      gview->setCursor(QCursor(Qt::SplitHCursor));
        else if(shape == "handpointing")    gview->setCursor(QCursor(Qt::PointingHandCursor));
        else if(shape == "forbidden")       gview->setCursor(QCursor(Qt::ForbiddenCursor));
        else if(shape == "handopen")        gview->setCursor(QCursor(Qt::OpenHandCursor));
        else if(shape == "handclosed")      gview->setCursor(QCursor(Qt::ClosedHandCursor));
        else if(shape == "whatsthis")       gview->setCursor(QCursor(Qt::WhatsThisCursor));
        else if(shape == "busy")            gview->setCursor(QCursor(Qt::BusyCursor));
        #if QT_VERSION >= 0x040700
        else if(shape == "dragmove")        gview->setCursor(QCursor(Qt::DragMoveCursor));
        else if(shape == "dragcopy")        gview->setCursor(QCursor(Qt::DragCopyCursor));
        else if(shape == "draglink")        gview->setCursor(QCursor(Qt::DragLinkCursor));
        #endif

    }
}

qreal MainWindow::nativeCalculateAngle(qreal x1, qreal y1, qreal x2, qreal y2)
{
    return QLineF(x1, -y1, x2, -y2).angle();
}

qreal MainWindow::nativeCalculateDistance(qreal x1, qreal y1, qreal x2, qreal y2)
{
    return QLineF(x1, y1, x2, y2).length();
}

qreal MainWindow::nativePerpendicularDistance(qreal px, qreal py, qreal x1, qreal y1, qreal x2, qreal y2)
{
    QLineF line(x1, y1, x2, y2);
    QLineF norm = line.normalVector();
    double dx = px-x1;
    double dy = py-y1;
    norm.translate(dx, dy);
    QPointF iPoint;
    norm.intersects(line, &iPoint);
    return QLineF(px, py, iPoint.x(), iPoint.y()).length();
}

void MainWindow::nativeAddToSelection(const QPainterPath path, Qt::ItemSelectionMode mode)
{
}

void MainWindow::nativeScaleSelected(qreal x, qreal y, qreal factor)
{
    if (factor <= 0.0) {
        QMessageBox::critical(this, tr("ScaleFactor Error"),
                                tr("Hi there. If you are not a developer, report this as a bug. "
                                "If you are a developer, your code needs examined, and possibly your head too."));
    }

    View* gview = activeView();
    if(gview) { gview->scaleSelected(x, -y, factor); }
}

void MainWindow::nativeRotateSelected(qreal x, qreal y, qreal rot)
{
    View* gview = activeView();
    if(gview) { gview->rotateSelected(x, -y, -rot); }
}

void MainWindow::nativeMirrorSelected(qreal x1, qreal y1, qreal x2, qreal y2)
{
    View* gview = activeView();
    if(gview) { gview->mirrorSelected(x1, -y1, x2, -y2); }
}

/* -- Basic wrappers of MainWindow functions -------------------------------- */
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

void
debug(const char *msg)
{
    qDebug(msg);
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
clear_selection(void)
{
    View* gview = activeView();
    if (gview) {
        gview->clearSelection();
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

/* -- lua registerables --------------------------------------------------------
 *
 * Try to keep this section of the file alphabetized in the function name to
 * match the declarations above.
 *
 * All function names should end in `_f`.
 */

/* Show the about dialog. */
int
about_f(lua_State *L)
{
    if (context_flag == CONTEXT_MAIN) {
        init_command();
        clear_selection();
    }
    _mainWin->about();
    end_command();
    return 0;
}

/* . */
int
add_to_menu_f(lua_State *L)
{
    debug("TODO: add_to_menu");
    no_args(L);
    return 0;
}

/* . */
int
add_toolbar_seperator_f(lua_State *L)
{
    debug("TODO: add_to_menu");
    no_args(L);
    return 0;
}

/* . */
int
add_to_toolbar_f(lua_State *L)
{
    debug("TODO: add_to_menu");
    no_args(L);
    return 0;
}

/* . */
int
allow_rubber_f(lua_State *L)
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
clear_rubber_f(lua_State *L)
{
    no_args(L);
    View* gview = activeView();
    if (gview) {
        gview->clearRubberRoom();
    }
    return 0;
}

/* . */
int
delete_selected_f(lua_State *L)
{
    no_args(L);
    View* gview = activeView();
    if (gview) {
        gview->deleteSelected();
    }
    clear_selection();
    return 0;
}

/* . */
int
dim_leader_f(lua_State *L)
{
    debug("TODO: add_to_menu");
    ScriptValue args[5];
    if (!unpack_args(L, "dim_leader_f", args, "rrrrr")) {
        return 0;
    }
    /*
    double x1 = args[0].r;
    double y1 = args[1].r;
    double x2 = args[2].r;
    double y2 = args[3].r;
    double rot = args[4].r;

    int rubberMode = OBJ_RUBBER_OFF;

    View* gview = activeView();
    QGraphicsScene* gscene = gview->scene();
    QUndoStack* stack = gview->getUndoStack();
    if (gview && gscene && stack) {
        DimLeaderObject* obj = new DimLeaderObject(x1, -y1, x2, -y2, getCurrentColor());
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
    */
    return 0;
}

/* . */
int
end_command_f(lua_State *L)
{
    no_args(L);
    end_command();
    return 0;
}

/* . */
int
horizontal_dimension_f(lua_State *L)
{
    debug("TODO: add_to_menu");
    no_args(L);
    return 0;
}

/* . */
int
image_f(lua_State *L)
{
    debug("TODO: add_to_menu");
    return 0;
}

/* . */
int
clear_selection_f(lua_State *L)
{
    View* gview = activeView();
    if (gview) {
        gview->clearSelection();
    }
    return 0;
}

/* . */
int
menu_selector_f(lua_State *L)
{
    debug("TODO: add_to_menu");
    return 0;
}

/* . */
int
messagebox_f(lua_State *L)
{
    debug("TODO: add_to_menu");
    /*
    if (context->argumentCount() != 3)    return debug("messageBox() requires three arguments");
    if (!context->argument(0).isString()) return debug(TypeError, "messageBox(): first argument is not a string");
    if (!context->argument(1).isString()) return debug(TypeError, "messageBox(): second argument is not a string");
    if (!context->argument(2).isString()) return debug(TypeError, "messageBox(): third argument is not a string");

    QString type = context->argument(0).toString().toLower();
    QString title = context->argument(1).toString();
    QString text = context->argument(2).toString();

    if (type != "critical" && type != "information" && type != "question" && type != "warning")
        return debug(UnknownError, "messageBox(): first argument must be \"critical\", \"information\", \"question\" or \"warning\".");

    _mainWin->nativeMessageBox(type, title, text);
    */
    return 0;
}

/* . */
int
mouse_f(lua_State *L)
{
    debug("TODO: add_to_menu");
    return 0;
}

/* . */
int
num_selected_f(lua_State *L)
{
    debug("TODO: num_selected");
    /*
    View* gview = activeView();
    if (gview) {
        return gview->numSelected();
    }
    else {
    }
    return 1;    
    */
    return 0;
}

/* . */
int
preview_off_f(lua_State *L)
{
    debug("TODO: preview_off_f");
    View* gview = activeView();
    if (gview) {
        gview->previewOff();
    }
    return 0;
}

/* . */
int
preview_on_f(lua_State *L)
{
    ScriptValue args[5];
    if (!unpack_args(L, "preview_on_f", args, "ssrrr")) {
        return 0;
    }

    QString cloneStr = args[0].s.toUpper();
    QString modeStr = args[1].s.toUpper();
    double x = args[2].r;
    double y = args[3].r;
    double data = args[4].r;

    int clone = PREVIEW_CLONE_NULL;
    int mode = PREVIEW_MODE_NULL;
    if (cloneStr == "SELECTED") {
        clone = PREVIEW_CLONE_SELECTED;
    }
    else if (cloneStr == "RUBBER") {
        clone = PREVIEW_CLONE_RUBBER;
    }
    else {
        debug("ERROR, preview_on_f(): first argument must be \"SELECTED\" or \"RUBBER\".");
        return 0;
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
        debug("ERROR preview_on_f(): second argument must be \"MOVE\", \"ROTATE\" or \"SCALE\".");
        return 0;
    }

    View* gview = activeView();
    if (gview) {
        gview->previewOn(clone, mode, x, -y, data);
    }
    return 0;
}

/* . */
int
print_area_f(lua_State *L)
{
    ScriptValue args[5];
    if (!unpack_args(L, "preview_on_f", args, "rrrr")) {
        return 0;
    }
    double x = args[0].r;
    double y = args[1].r;
    double w = args[2].r;
    double h = args[3].r;

    qDebug("print_area_f(%.2f, %.2f, %.2f, %.2f)", x, y, w, h);
    //TODO: Print Setup Stuff
    _mainWin->print();
    return 0;
}

/* . */
int
qsnap_f(lua_State *L)
{
    debug("TODO: add_to_menu");
    return 0;
}

/* . */
int
ray_f(lua_State *L)
{
    //TODO: parameter error checking
    qDebug("TODO: finish addRay command");
    return 0;
}

/* TODO: parameter error checking */
int
regular_polygon_f(lua_State *L)
{
    debug("TODO: finish addRegularPolygon command");
    return 0;
}

/* . */
int
rubber_f(lua_State *L)
{
    debug("TODO: add_to_menu");
    return 0;
}

/* . */
int
text_angle_f(lua_State *L)
{
    debug("TODO: text_angle_f");
    /* return QScriptValue(settings.text_angle); */
    return 0;
}

/* . */
int
text_font_f(lua_State *L)
{
    debug("TODO: text_font_f");
    /* return QScriptValue(settings.text_font); */
    return 0;
}

/* . */
int
text_single_f(lua_State *L)
{
    ScriptValue args[5];
    if (!unpack_args(L, "text_single_f", args, "srrrb")) {
        return 0;
    }

    QString str = args[0].s;
    double x = args[1].r;
    double y = args[2].r;
    double rot = args[3].r;
    bool fill = args[4].b;
    _mainWin->nativeAddTextSingle(str, x, y, rot, fill, OBJ_RUBBER_OFF);
    return 0;
}

/* . */
int
text_multi_f(lua_State *L)
{
    ScriptValue args[5];
    if (!unpack_args(L, "text_single_f", args, "srrrb")) {
        return 0;
    }

    QString str = args[0].s;
    double x = args[1].r;
    double y = args[2].r;
    double rot = args[3].r;
    bool fill = args[4].b;
    _mainWin->nativeAddTextMulti(str, x, y, rot, fill, OBJ_RUBBER_OFF);
    return 0;
}

/* . */
int
triangle_f(lua_State *L)
{
    ScriptValue args[8];
    if (!unpack_args(L, "text_single_f", args, "rrrrrrrb")) {
        return 0;
    }

    double x1 = args[0].r;
    double y1 = args[1].r;
    double x2 = args[2].r;
    double y2 = args[3].r;
    double x3 = args[4].r;
    double y3 = args[5].r;
    double rot = args[6].r;
    bool fill = args[7].b;
    _mainWin->nativeAddTriangle(x1, y1, x2, y2, x3, y3, rot, fill);
    return 0;
}

/* . */
int
vertical_dimension_f(lua_State *L)
{
    debug("TODO: add_to_menu");
    return 0;
}

/* . */
int
vulcanize_f(lua_State *L)
{
    View* gview = activeView();
    if (gview) {
        gview->vulcanizeRubberRoom();
    }
    return 0;
}

/* . */
int
infinite_line_f(lua_State *L)
{
    debug("TODO: add_to_menu");
    return 0;
}

/* . */
int
menu_seperator_f(lua_State *L)
{
    debug("TODO: add_to_menu");
    return 0;
}

/* . */
int
select_all_f(lua_State *L)
{
    debug("TODO: add_to_menu");
    View* gview = activeView();
    if (gview) {
        gview->selectAll();
    }
    return 0;
}

/* . */
int
set_color_f(lua_State *L)
{
    ScriptValue args[4];
    if (!unpack_args(L, "set_color_f", args, "srrr")) {
        return 0;
    }
    QString key = args[0].s;
    double r = args[1].r;
    double g = args[2].r;
    double b = args[3].r;

    if ((r < 0) || (r > 255)) {
        debug("ERROR set_color(): r value must be in range 0-255");
        return 0;
    }
    if ((g < 0) || (g > 255)) {
        debug("ERROR set_color(): g value must be in range 0-255");
        return 0;
    }
    if ((b < 0) || (b > 255)) {
        debug("ERROR set_color(): b value must be in range 0-255");
        return 0;
    }

    if (key == "background") {
        settings.display_bg_color = qRgb(r,g,b);
        _mainWin->updateAllViewBackgroundColors(qRgb(r,g,b));
    }
    else if (key == "crosshair") {
        settings.display_crosshair_color = qRgb(r,g,b);
        _mainWin->updateAllViewCrossHairColors(qRgb(r,g,b));
    }
    else if (key == "grid") {
        settings.grid_color = qRgb(r,g,b);
        _mainWin->updateAllViewGridColors(qRgb(r,g,b));
    }
    else {
        debug("ERROR set_color(): key does not correspond to a color setting variable");
    }
    return 0;
}

/* . */
int
set_rubber_mode_f(lua_State *L)
{
    debug("TODO: add_to_menu");
    /*
    if (context->argumentCount() != 1)    return debug("setRubberMode() requires one argument");
    if (!context->argument(0).isString()) return debug(TypeError, "setRubberMode(): first argument is not a string");

    QString mode = context->argument(0).toString().toUpper();

    if (mode == "CIRCLE_1P_RAD") {
        _mainWin->nativeSetRubberMode(OBJ_RUBBER_CIRCLE_1P_RAD);
    }
    else if (mode == "CIRCLE_1P_DIA") {
        _mainWin->nativeSetRubberMode(OBJ_RUBBER_CIRCLE_1P_DIA);
    }
    else if (mode == "CIRCLE_2P") {
        _mainWin->nativeSetRubberMode(OBJ_RUBBER_CIRCLE_2P);
    }
    else if (mode == "CIRCLE_3P") {
        _mainWin->nativeSetRubberMode(OBJ_RUBBER_CIRCLE_3P);
    }
    else if (mode == "CIRCLE_TTR") {
        _mainWin->nativeSetRubberMode(OBJ_RUBBER_CIRCLE_TTR);
    }
    else if (mode == "CIRCLE_TTR") {
        _mainWin->nativeSetRubberMode(OBJ_RUBBER_CIRCLE_TTT);
    }
    else if (mode == "DIMLEADER_LINE") {
        _mainWin->nativeSetRubberMode(OBJ_RUBBER_DIMLEADER_LINE);
    }
    else if (mode == "ELLIPSE_LINE") {
        _mainWin->nativeSetRubberMode(OBJ_RUBBER_ELLIPSE_LINE);
    }
    else if (mode == "ELLIPSE_MAJORDIAMETER_MINORRADIUS") {
        _mainWin->nativeSetRubberMode(OBJ_RUBBER_ELLIPSE_MAJORDIAMETER_MINORRADIUS);
    }
    else if (mode == "ELLIPSE_MAJORRADIUS_MINORRADIUS") {
        _mainWin->nativeSetRubberMode(OBJ_RUBBER_ELLIPSE_MAJORRADIUS_MINORRADIUS);
    }
    else if (mode == "ELLIPSE_ROTATION") {
        _mainWin->nativeSetRubberMode(OBJ_RUBBER_ELLIPSE_ROTATION);
    }
    else if (mode == "LINE") {
        _mainWin->nativeSetRubberMode(OBJ_RUBBER_LINE);
    }
    else if (mode == "POLYGON") {
        _mainWin->nativeSetRubberMode(OBJ_RUBBER_POLYGON);
    }
    else if (mode == "POLYGON_INSCRIBE") {
        _mainWin->nativeSetRubberMode(OBJ_RUBBER_POLYGON_INSCRIBE);
    }
    else if (mode == "POLYGON_CIRCUMSCRIBE") {
        _mainWin->nativeSetRubberMode(OBJ_RUBBER_POLYGON_CIRCUMSCRIBE);
    }
    else if (mode == "POLYLINE") {
        _mainWin->nativeSetRubberMode(OBJ_RUBBER_POLYLINE);
    }
    else if (mode == "RECTANGLE") {
        _mainWin->nativeSetRubberMode(OBJ_RUBBER_RECTANGLE);
    }
    else if (mode == "TEXTSINGLE") {
        _mainWin->nativeSetRubberMode(OBJ_RUBBER_TEXTSINGLE);
    }
    else {
        return debug(UnknownError, "setRubberMode(): unknown rubberMode value");
    }
    */
    return 0;
}

/* . */
int
set_rubber_text_f(lua_State *L)
{
    ScriptValue args[2];
    if (!unpack_args(L, "set_rubber_text_f", args, "ss")) {
        return 0;
    }
    QString key = args[0].s.toUpper();
    QString txt = args[1].s;

    View* gview = activeView();
    if (gview) {
        gview->setRubberText(key, txt);
    }
    return 0;
}

/* . */
int
set_text_font_f(lua_State *L)
{
    debug("TODO: add_to_menu");
    const char *str = luaL_checkstring(L, 1);

    _mainWin->textFontSelector->setCurrentFont(QFont(str));
    settings.text_font = str;
    return 0;
}

/* . */
int
set_text_size_f(lua_State *L)
{
    double num = lua_tonumber(L, 1);

    if (std::isnan(num)) {
        debug("TypeError, setTextSize(): first argument failed isNaN check.");
        return 0;
    }

    settings.text_size = std::fabs(num);
    int index = _mainWin->textSizeSelector->findText("Custom", Qt::MatchContains);
    if (index != -1) {
        _mainWin->textSizeSelector->removeItem(index);
    }
    _mainWin->textSizeSelector->addItem("Custom " + QString().setNum(num, 'f', 2) + " pt", num);
    index = _mainWin->textSizeSelector->findText("Custom", Qt::MatchContains);
    if (index != -1) {
        _mainWin->textSizeSelector->setCurrentIndex(index);
    }
    return 0;
}

/* TODO: convert set_rubber_point_f. */
int
set_rubber_point_f(lua_State *L)
{
    QString key = QString(luaL_checkstring(L, 1)).toUpper();
    double x = lua_tonumber(L, 2);
    double y = lua_tonumber(L, 3);

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
set_text_angle_f(lua_State *L)
{
    double num = lua_tonumber(L, 1);

    if (std::isnan(num)) {
        debug("TypeError, set_text_angle_f(): first argument failed isNaN check.");
        return 0;
    }

    settings.text_angle = num;
    return 0;
}

/* . */
int
text_size_f(lua_State *L)
{
    debug("TODO: add_to_menu");
    no_args(L);
    /* return QScriptValue(settings.text_size); */
    return 0;
}

/* . */
int
command_f(lua_State *L)
{
    ScriptValue args[6];
    if (!unpack_args(L, "command_f", args, "ssssss")) {
        return 0;
    }
    Command command;
    command.icon = args[0].s;
    command.command = args[1].s;
    command.tooltip = args[2].s;
    command.statustip = args[3].s;
    command.shortcut = args[4].s;
    command.macos = args[5].s;
    command.checkable = 0;
    qDebug("Adding command \"%s\"...", command.icon);
    command_map.push_back(command);
    return 0;
}

/* Adds the lua function (alert "EXAMPLE ALERT").
 */
int
alert_f(lua_State *L)
{
    ScriptValue args[1];
    if (!unpack_args(L, "alert_f", args, "s")) {
        return 0;
    }
    _mainWin->prompt->alert(args[0].s);
    return 0;
}

/*
 */
int
append_prompt_history_f(lua_State *L)
{
    ScriptValue args[1];
    if (!unpack_args(L, "append_prompt_history_f", args, "s")) {
        return 0;
    }
    _mainWin->prompt->appendHistory(args[0].s);
    return 0;
}

/* Adds the lua function (blink-prompt).
 */
int
blink_f(lua_State *L)
{
    no_args(L);
    _mainWin->prompt->startBlinking();
    return 0;
}

/* Adds the lua function:
 *     (debug "message")
 */
int
debug_f(lua_State *L)
{
    ScriptValue args[1];
    if (!unpack_args(L, "debug_f", args, "s")) {
        return 0;
    }
    qDebug("%s", args[0]);
    return 0;
}

/* Adds the lua function (error "EXAMPLE ERROR").
 */
int
error_f(lua_State *L)
{
    ScriptValue args[1];
    if (!unpack_args(L, "error_f", args, "ss")) {
        return 0;
    }

    _mainWin->prompt->setPrefix("ERROR: (" + args[0].s + ") " + args[1].s);
    _mainWin->prompt->appendHistory(QString());
    end_command();
    return 0;
}

/* Reports the entire state for debugging purposes: this means we don't have to
 * be paranoid about losing aspects of the state in lua variables because
 * we can check the contents of state before and after a given user action
 * during testing.
 *
 * TODO: finish reporting here.
 */
int
report_state_f(lua_State *L)
{
    no_args(L);
    FILE *f = fopen("state.txt", "w");
    if (!f) {
        return 0;
    }
    fprintf(f, "context_flag = %d\n", context_flag);
    fclose(f);
    return 0;
}

/* . */
int
changelog_f(lua_State *L)
{
    no_args(L);
    return 0;
}

/* TODO: circle fill. */
int
circle_f(lua_State *L)
{
    ScriptValue args[4];
    if (!unpack_args(L, "circle_f", args, "rrrb")) {
        return 0;
    }

    EmbVector center = unpack_vector(args, 0);
    double radius = args[2].r;
    bool fill = args[3].b;
    int rubberMode = OBJ_RUBBER_OFF;

    View* gview = activeView();
    QGraphicsScene* gscene = gview->scene();
    QUndoStack* stack = gview->getUndoStack();
    if (gview && gscene && stack) {
        CircleObject* obj = new CircleObject(center.x, center.y, radius, getCurrentColor());
        obj->setObjectRubberMode(rubberMode);
        //TODO: circle fill
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
    return 0;
}

/* . */
int
color_selector_f(lua_State *L)
{
    no_args(L);
    return 0;
}

/* . */
int
copy_f(lua_State *L)
{
    no_args(L);
    return 0;
}

/* . */
int
cut_f(lua_State *L)
{
    no_args(L);
    return 0;
}

/* TODO: Make day vision color settings. */
int
day_f(lua_State *L)
{
    no_args(L);
    View* gview = activeView();
    if (gview) {
        gview->setBackgroundColor(qRgb(255,255,255)); 
        gview->setCrossHairColor(qRgb(0,0,0));
        gview->setGridColor(qRgb(0,0,0));
    }
    return 0;
}

/* . */
int
design_details_f(lua_State *L)
{
    no_args(L);
    return 0;
}

/* . */
int
disable_move_rapid_fire_f(lua_State *L)
{
    no_args(L);
    View* gview = activeView();
    if (gview) {
        gview->disableMoveRapidFire();
    }
    return 0;
}

/* . */
int
disable_prompt_rapid_fire_f(lua_State *L)
{
    no_args(L);
    _mainWin->prompt->disableRapidFire();
    return 0;
}

/* . */
int
distance_f(lua_State *L)
{
    no_args(L);
    return 0;
}

/* . */
int
dolphin_f(lua_State *L)
{
    no_args(L);
    return 0;
}

/* This function intensionally does nothing: a nil function to use as a
 * placeholder that can be called.
 */
int
do_nothing_f(lua_State *L)
{
    no_args(L);
    return 0;
}

/* . */
int
enable_move_rapid_fire_f(lua_State *L)
{
    no_args(L);
    View* gview = activeView();
    if (gview) {
        gview->enableMoveRapidFire();
    }
    return 0;
}

/* . */
int
enable_prompt_rapid_fire_f(lua_State *L)
{
    no_args(L);
    _mainWin->prompt->enableRapidFire();
    return 0;
}

/* . */
int
erase_f(lua_State *L)
{
    no_args(L);
    return 0;
}

/* . */
int
exit_program_f(lua_State *L)
{
    no_args(L);
    if (settings.prompt_save_history) {
        //TODO: get filename from settings
        _mainWin->prompt->saveHistory("prompt.log",
            settings.prompt_save_history_as_html);
    }
    qApp->closeAllWindows();
    //Force the MainWindow destructor to run before exiting. Makes Valgrind "still reachable" happy :)
    _mainWin->deleteLater();
    return 0;
}

/* . */
int
freeze_all_layers_f(lua_State *L)
{
    no_args(L);
    return 0;
}

/* . */
int
heart_f(lua_State *L)
{
    no_args(L);
    return 0;
}

/* . */
int
help_f(lua_State *L)
{
    no_args(L);
    if (context_flag == CONTEXT_MAIN) {
        init_command();
        clear_selection();
    }
    _mainWin->help();
    end_command();
    return 0;
}

/* . */
int
hide_all_layers_f(lua_State *L)
{
    no_args(L);
    return 0;
}

/* . */
int
icon128_f(lua_State *L)
{
    no_args(L);
    if (context_flag == CONTEXT_MAIN) {
        init_command();
        clear_selection();
    }
    _mainWin->iconResize(128);
    end_command();
    return 0;
}

/* . */
int
icon16_f(lua_State *L)
{
    no_args(L);
    if (context_flag == CONTEXT_MAIN) {
        init_command();
        clear_selection();
    }
    _mainWin->iconResize(16);
    end_command();
    return 0;
}

/* . */
int
icon24_f(lua_State *L)
{
    no_args(L);
    if (context_flag == CONTEXT_MAIN) {
        init_command();
        clear_selection();
    }
    _mainWin->iconResize(24);
    end_command();
    return 0;
}

/* . */
int
icon32_f(lua_State *L)
{
    no_args(L);
    if (context_flag == CONTEXT_MAIN) {
        init_command();
        clear_selection();
    }
    _mainWin->iconResize(32);
    end_command();
    return 0;
}

/* . */
int
icon48_f(lua_State *L)
{
    no_args(L);
    if (context_flag == CONTEXT_MAIN) {
        init_command();
        clear_selection();
    }
    _mainWin->iconResize(48);
    end_command();
    return 0;
}

/* . */
int
icon64_f(lua_State *L)
{
    no_args(L);
    if (context_flag == CONTEXT_MAIN) {
        init_command();
        clear_selection();
    }
    _mainWin->iconResize(64);
    end_command();
    return 0;
}

/* . */
int
init_command_f(lua_State *L)
{
    no_args(L);
    View* gview = activeView();
    if (gview) {
        gview->clearRubberRoom();
    }
    return 0;
}

/* . */
int
layers_f(lua_State *L)
{
    no_args(L);
    return 0;
}

/* . */
int
layer_previous_f(lua_State *L)
{
    no_args(L);
    return 0;
}

/* . */
int
layer_selector_f(lua_State *L)
{
    return 0;
}

/* . */
int
line_f(lua_State *L)
{
    ScriptValue args[5];
    if (!unpack_args(L, "line_f", args, "rrrrr")) {
        return 0;
    }
    _mainWin->nativeAddLine(args[0].r, args[1].r, args[2].r, args[3].r,
        args[4].r, OBJ_RUBBER_OFF);
    return 0;
}

int
line_type_selector_f(lua_State *L)
{
    no_args(L);
    return 0;
}

int
line_weight_selector_f(lua_State *L)
{
    no_args(L);
    return 0;
}

int
locate_point_f(lua_State *L)
{
    no_args(L);
    return 0;
}

/* . */
int
lock_all_layers_f(lua_State *L)
{
    no_args(L);
    return 0;
}

/* . */
int
make_layer_current_f(lua_State *L)
{
    no_args(L);
    return 0;
}

/* . */
int
move_f(lua_State *L)
{
    no_args(L);
    return 0;
}

/* . */
int
new_file_f(lua_State *L)
{
    _mainWin->newFile();
    return 0;
}

/* TODO: Make night vision color settings. */
int
night_vision_f(lua_State *L)
{
    View* gview = activeView();
    if (gview) {
        gview->setBackgroundColor(qRgb(0,0,0));
        gview->setCrossHairColor(qRgb(255,255,255));
        gview->setGridColor(qRgb(255,255,255));
    }
    return 0;
}

int
open_file_f(lua_State *L)
{
    if (context_flag == CONTEXT_MAIN) {
        init_command();
        clear_selection();
    }
    _mainWin->openFile();
    end_command();
    return 0;
}

int
path_f(lua_State *L)
{
    return 0;
}

int
platform_f(lua_State *L)
{
    /*
    return QScriptValue(_mainWin->platformString());
    */
    return 0;
}

int
polygon_f(lua_State *L)
{
    return 0;
}

int
polyline_f(lua_State *L)
{
    return 0;
}

int
quickleader_f(lua_State *L)
{
    return 0;
}

int
redo_f(lua_State *L)
{
    QString prefix = _mainWin->prompt->getPrefix();
    if (_mainWin->dockUndoEdit->canRedo()) {
        _mainWin->prompt->setPrefix("Redo " + _mainWin->dockUndoEdit->redoText());
        _mainWin->prompt->appendHistory(QString());
        _mainWin->dockUndoEdit->redo();
        _mainWin->prompt->setPrefix(prefix);
    }
    else {
        _mainWin->prompt->alert("Nothing to redo");
        _mainWin->prompt->setPrefix(prefix);
    }
    return 0;
}

int
rgb_f(lua_State *L)
{
    return 0;
}

int
rotate_f(lua_State *L)
{
    return 0;
}

int
selectall_f(lua_State *L)
{
    return 0;
}

int
singlelinetext_f(lua_State *L)
{
    return 0;
}

/* Moves the view down. */
int
pan_down_f(lua_State *L)
{
    if (context_flag == CONTEXT_MAIN) {
        init_command();
    }
    View* gview = activeView();
    QUndoStack* stack = gview->getUndoStack();
    if (gview && stack) {
        UndoableNavCommand* cmd = new UndoableNavCommand("PanDown", gview, 0);
        stack->push(cmd);
    }
    end_command();
    return 0;
}

/* Moves the view to the left. */
int
pan_left_f(lua_State *L)
{
    if (context_flag == CONTEXT_MAIN) {
        init_command();
    }
    View* gview = activeView();
    QUndoStack* stack = gview->getUndoStack();
    if (gview && stack) {
        UndoableNavCommand* cmd = new UndoableNavCommand("PanLeft", gview, 0);
        stack->push(cmd);
    }
    end_command();
    return 0;
}

/* . */
int
pan_point_f(lua_State *L)
{
    View* gview = activeView();
    if (gview) {
        gview->panPoint();
    }
    return 0;
}

/* . */
int
pan_real_time_f(lua_State *L)
{
    View* gview = activeView();
    if (gview) {
        gview->panRealTime();
    }
    return 0;
}

/* Moves the view to the right. */
int
pan_right_f(lua_State *L)
{
    if (context_flag == CONTEXT_MAIN) {
        init_command();
    }
    View* gview = activeView();
    QUndoStack* stack = gview->getUndoStack();
    if (gview && stack) {
        UndoableNavCommand* cmd = new UndoableNavCommand("PanRight", gview, 0);
        stack->push(cmd);
    }
    end_command();
    return 0;
}

/* Moves the view up. */
int
pan_up_f(lua_State *L)
{
    if (context_flag == CONTEXT_MAIN) {
        init_command();
    }
    View* gview = activeView();
    QUndoStack* stack = gview->getUndoStack();
    if (gview && stack) {
        UndoableNavCommand* cmd = new UndoableNavCommand("PanUp", gview, 0);
        stack->push(cmd);
    }
    end_command();
    return 0;
}

/* . */
int
paste_f(lua_State *L)
{
    return 0;
}

/* . */
int
print_f(lua_State *L)
{
    return 0;
}

/* . */
int
save_f(lua_State *L)
{
    return 0;
}

int
saveas_f(lua_State *L)
{
    return 0;
}

int
scale_f(lua_State *L)
{
    return 0;
}

/* . */
int
set_prompt_prefix_f(lua_State *L)
{
    QString s(luaL_checkstring(L, 1));
    _mainWin->prompt->setPrefix(s);
    return 0;
}

/* . */
int
set_text_bold_f(lua_State *L)
{
    if (!lua_isboolean(L, 1)) {
        debug("set_text_bold: argument not boolean");
        return 0;
    }
    settings.text_style_bold = lua_toboolean(L, 1);
    return 0;
}

/* . */
int
set_text_italic_f(lua_State *L)
{
    if (!lua_isboolean(L, 1)) {
        debug("set_text_italic: argument not boolean");
        return 0;
    }
    settings.text_style_italic = lua_toboolean(L, 1);
    return 0;
}

/* . */
int
set_text_overline_f(lua_State *L)
{
    if (!lua_isboolean(L, 1)) {
        debug("set_text_overline: argument not boolean");
        return 0;
    }
    settings.text_style_overline = lua_toboolean(L, 1);
    return 0;
}

/* . */
int
set_text_strikeout_f(lua_State *L)
{
    if (!lua_isboolean(L, 1)) {
        debug("set_text_strikeout: argument not boolean");
        return 0;
    }
    settings.text_style_strikeout = lua_toboolean(L, 1);
    return 0;
}

/* . */
int
set_text_underline_f(lua_State *L)
{
    if (!lua_isboolean(L, 1)) {
        debug("set_text_overline: argument not boolean");
        return 0;
    }
    settings.text_style_overline = lua_toboolean(L, 1);
    return 0;
}

int
settings_dialog_f(lua_State *L)
{
    return 0;
}

int
show_all_layers_f(lua_State *L)
{
    return 0;
}

int
snowflake_f(lua_State *L)
{
    return 0;
}

int
star_f(lua_State *L)
{
    return 0;
}

int
syswindows_f(lua_State *L)
{
    return 0;
}

int
text_bold_f(lua_State *L)
{
    /* lua_push(settings.text_style_bold); */
    return 0;
}

int
text_italic_f(lua_State *L)
{
    /* lua_push(settings.text_style_italic); */
    return 0;
}

int
text_overline_f(lua_State *L)
{
    /* lua_push(settings.text_style_overline); */
    return 0;
}

int
text_strikeout_f(lua_State *L)
{
    /* lua_push(settings.text_style_strikeout); */
    return 0;
}

int
text_underline_f(lua_State *L)
{
    /* lua_push(settings.text_style_underline); */
    return 0;
}

/* . */
int
thaw_all_layers_f(lua_State *L)
{
    return 0;
}

/* . */
int
tip_of_the_day_f(lua_State *L)
{
    _mainWin->tipOfTheDay();
    return 0;
}

/* Adds the lua function:
 *     (todo "Missing feature description")
 */
int
todo_f(lua_State *L)
{
    QString strCmd(luaL_checkstring(L, 1));
    QString strTodo(luaL_checkstring(L, 2));

    _mainWin->prompt->alert("TODO: (" + strCmd + ") " + strTodo);
    end_command();
    return 0;
}

int
undo_f(lua_State *L)
{
    QString prefix = _mainWin->prompt->getPrefix();
    if (_mainWin->dockUndoEdit->canUndo()) {
        _mainWin->prompt->setPrefix("Undo " + _mainWin->dockUndoEdit->undoText());
        _mainWin->prompt->appendHistory(QString());
        _mainWin->dockUndoEdit->undo();
        _mainWin->prompt->setPrefix(prefix);
    }
    else {
        _mainWin->prompt->alert("Nothing to undo");
        _mainWin->prompt->setPrefix(prefix);
    }
    return 0;
}

int
unlock_all_layers_f(lua_State *L)
{
    return 0;
}

int
whats_this_f(lua_State *L)
{
    return 0;
}

/* . */
int
window_cascade_f(lua_State *L)
{
    if (context_flag == CONTEXT_MAIN) {
        init_command();
        clear_selection();
    }
    _mainWin->mdiArea->cascade();
    end_command();
    return 0;
}

/* . */
int
window_close_f(lua_State *L)
{
    if (context_flag == CONTEXT_MAIN) {
        init_command();
        clear_selection();
    }
    _mainWin->onCloseWindow();
    end_command();
    return 0;
}

/* . */
int
window_close_all_f(lua_State *L)
{
    if (context_flag == CONTEXT_MAIN) {
        init_command();
        clear_selection();
    }
    _mainWin->mdiArea->closeAllSubWindows();
    end_command();
    return 0;
}

/* . */
int
window_next_f(lua_State *L)
{
    if (context_flag == CONTEXT_MAIN) {
        init_command();
        clear_selection();
    }
    _mainWin->mdiArea->activateNextSubWindow();
    end_command();
    return 0;
}

int
window_previous_f(lua_State *L)
{
    if (context_flag == CONTEXT_MAIN) {
        init_command();
        clear_selection();
    }
    _mainWin->mdiArea->activatePreviousSubWindow();
    end_command();
    return 0;
}

int
window_tile_f(lua_State *L)
{
    if (context_flag == CONTEXT_MAIN) {
        init_command();
        clear_selection();
    }
    _mainWin->mdiArea->tile();
    end_command();
    return 0;
}

int
zoom_all_f(lua_State *L)
{
    stub_implement("Implement zoomAll.");
    return 0;
}

int
zoom_center_f(lua_State *L)
{
    stub_implement("Implement zoomCenter.");
    return 0;
}

int
zoom_dynamic_f(lua_State *L)
{
    stub_implement("Implement zoomDynamic.");
    return 0;
}

/* Zooms to display the drawing extents. */
int
zoom_extents_f(lua_State *L)
{
    if (context_flag == CONTEXT_MAIN) {
        init_command();
        clear_selection();
    }
    View* gview = activeView();
    QUndoStack* stack = gview->getUndoStack();
    if (gview && stack) {
        UndoableNavCommand* cmd = new UndoableNavCommand("ZoomExtents", gview, 0);
        stack->push(cmd);
    }
    end_command();
    return 0;
}

/* Zooms to increase the apparent size of objects. */
int
zoom_in_f(lua_State *L)
{
    if (context_flag == CONTEXT_MAIN) {
        init_command();
        clear_selection();
    }
    View* gview = activeView();
    if (gview) {
        gview->zoomIn();
    }
    end_command();
    return 0;
}

/* Zooms to decrease the apparent size of objects. */
int
zoom_out_f(lua_State *L)
{
    if (context_flag == CONTEXT_MAIN) {
        init_command();
        clear_selection();
    }
    View* gview = activeView();
    if (gview) {
        gview->zoomOut();
    }
    end_command();
    return 0;
}

/* . */
int
zoom_scale_f(lua_State *L)
{
    stub_implement("Implement zoomScale.");
    return 0;
}

/* . */
int
zoom_previous_f(lua_State *L)
{
    stub_implement("Implement zoomPrevious.");
    return 0;
}

/* . */
int
zoom_real_time_f(lua_State *L)
{
    stub_implement("Implement zoomRealtime.");
    return 0;
}

/* . */
int
zoom_selected_f(lua_State *L)
{
    View* gview = activeView();
    QUndoStack* stack = gview->getUndoStack();
    if (gview && stack) {
        UndoableNavCommand* cmd = new UndoableNavCommand("ZoomSelected", gview, 0);
        stack->push(cmd);
    }
    return 0;
}

/* . */
int
zoom_window_f(lua_State *L)
{
    View* gview = activeView();
    if (gview) {
        gview->zoomWindow();
    }
    return 0;
}

/* TODO: move add_rubber to script */
int
AddRubber_f(lua_State *L)
{
    /*
    if (context->argumentCount() != 1)    return debug("addRubber() requires one argument");
    if (!context->argument(0).isString()) return debug(TypeError, "addRubber(): first argument is not a string");

    QString objType = context->argument(0).toString().toUpper();

    // FIXME
    run("a = allow_rubber()");
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
        _mainWin->nativeAddCircle(mx, my, 0, false, OBJ_RUBBER_ON);
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
        _mainWin->nativeAddDimLeader(mx, my, mx, my, 0, OBJ_RUBBER_ON);
    }
    else if (objType == "DIMLINEAR") {

} //TODO: handle this type
    else if (objType == "DIMORDINATE") {

} //TODO: handle this type
    else if (objType == "DIMRADIUS")   {

} //TODO: handle this type
    else if (objType == "ELLIPSE") { _mainWin->nativeAddEllipse(mx, my, 0, 0, 0, 0, OBJ_RUBBER_ON); }
    else if (objType == "ELLIPSEARC") {

} //TODO: handle this type
    else if (objType == "HATCH")       {} //TODO: handle this type
    else if (objType == "IMAGE")       {} //TODO: handle this type
    else if (objType == "INFINITELINE") {} //TODO: handle this type
    else if (objType == "LINE")        { _mainWin->nativeAddLine(mx, my, mx, my, 0, OBJ_RUBBER_ON); }
    else if (objType == "PATH")        {} //TODO: handle this type
    else if (objType == "POINT")       {} //TODO: handle this type
    else if (objType == "POLYGON")     { _mainWin->nativeAddPolygon(mx, my, QPainterPath(), OBJ_RUBBER_ON); }
    else if (objType == "POLYLINE")    { _mainWin->nativeAddPolyline(mx, my, QPainterPath(), OBJ_RUBBER_ON); }
    else if (objType == "RAY")         {} //TODO: handle this type
    else if (objType == "RECTANGLE")   { _mainWin->nativeAddRectangle(mx, my, mx, my, 0, 0, OBJ_RUBBER_ON); }
    else if (objType == "SPLINE")      {} //TODO: handle this type
    else if (objType == "TEXTMULTI")   {} //TODO: handle this type
    else if (objType == "TEXTSINGLE")  { _mainWin->nativeAddTextSingle("", mx, my, 0, false, OBJ_RUBBER_ON); }
    */
    return 0;
}

/* . */
int
spare_rubber_f(lua_State *L)
{
    ScriptValue args[1];
    if (!unpack_args(L, "spare_rubber_f", args, "s")) {
        return 0;
    }

    QString objID = args[0].s.toUpper();

    if (objID == "PATH") {
        _mainWin->nativeSpareRubber(SPARE_RUBBER_PATH);
    }
    else if (objID == "POLYGON") {
        _mainWin->nativeSpareRubber(SPARE_RUBBER_POLYGON);
    }
    else if (objID == "POLYLINE") {
        _mainWin->nativeSpareRubber(SPARE_RUBBER_POLYLINE);
    }
    else {
        bool ok = false;
        qint64 id = objID.toLongLong(&ok);
        if (!ok) {
            debug("TypeError, spare_rubber(): error converting object ID into an int64");
            return 0;
        }
        _mainWin->nativeSpareRubber(id);
    }

    return 0;
}

int
AddInfiniteLine_f(lua_State *L)
{
    //TODO: parameter error checking
    qDebug("TODO: finish addInfiniteLine command");
    return 0;
}

int
rectangle_f(lua_State *L)
{
    ScriptValue args[6];
    if (!unpack_args(L, "rectangle_f", args, "rrrrrb")) {
        return 0;
    }

    double x = args[0].r;
    double y = args[1].r;
    double w = args[2].r;
    double h = args[3].r;
    double rot = args[4].r;
    bool fill = args[5].b;

    _mainWin->nativeAddRectangle(x, y, w, h, rot, fill, OBJ_RUBBER_OFF);
    return 0;
}

/* . */
int
rounded_rectangle_f(lua_State *L)
{
    ScriptValue args[7];
    if (!unpack_args(L, "rounded_rectangle_f", args, "rrrrrrb")) {
        return 0;
    }

    double x = args[0].r;
    double y = args[1].r;
    double w = args[2].r;
    double h = args[3].r;
    double rad = args[4].r;
    double rot = args[5].r;
    bool fill = args[6].b;

    _mainWin->nativeAddRoundedRectangle(x, y, w, h, rad, rot, fill);
    return 0;
}

/* . */
int
arc_f(lua_State *L)
{
    ScriptValue args[6];
    if (!unpack_args(L, "arc_f", args, "rrrrrr")) {
        return 0;
    }

    EmbVector start = unpack_vector(args, 0);
    EmbVector mid = unpack_vector(args, 2);
    EmbVector end = unpack_vector(args, 4);

    _mainWin->nativeAddArc(start.x, start.y, mid.x, mid.y, end.x, end.y, OBJ_RUBBER_OFF);
    return 0;
}

/* . */
int
slot_f(lua_State *L)
{
    ScriptValue args[6];
    if (!unpack_args(L, "slot_f", args, "rrrrrb")) {
        return 0;
    }

    EmbVector center = unpack_vector(args, 0);
    double diameter = args[2].r;
    double length = args[3].r;
    double rot = args[4].r;
    bool fill = args[5].b;

    _mainWin->nativeAddSlot(center.x, center.y, diameter, length, rot, fill, OBJ_RUBBER_OFF);
    return 0;
}

/* . */
int
ellipse_f(lua_State *L)
{
    ScriptValue args[6];
    if (!unpack_args(L, "ellipse_f", args, "rrrrrb")) {
        return 0;
    }

    EmbVector center = unpack_vector(args, 0);
    EmbVector radii = unpack_vector(args, 2);
    double rot = args[4].r;
    bool fill = args[5].b;

    _mainWin->nativeAddEllipse(center.x, center.y, radii.x, radii.y, rot, fill, OBJ_RUBBER_OFF);
    return 0;
}

/* . */
int
point_f(lua_State *L)
{
    ScriptValue args[2];
    if (!unpack_args(L, "point_f", args, "rr")) {
        return 0;
    }

    double x = args[0].r;
    double y = args[1].r;
    _mainWin->nativeAddPoint(x,y);
    return 0;
}

int
AddPolygon_f(lua_State *L)
{
    /*
    if (context->argumentCount() != 1)   return debug("addPolygon() requires one argument");
    if (!context->argument(0).isArray()) return debug(TypeError, "addPolygon(): first argument is not an array");

    QVariantList varList = context->argument(0).toVariant().toList();
    int varSize = varList.size();
    if (varSize < 2) return debug(TypeError, "addPolygon(): array must contain at least two elements");
    if (varSize % 2) return debug(TypeError, "addPolygon(): array cannot contain an odd number of elements");

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
            return debug(TypeError, "addPolygon(): array contains one or more invalid elements");
        }
    }

    //Close the polygon
    path.closeSubpath();

    path.translate(-startX, -startY);

    _mainWin->nativeAddPolygon(startX, startY, path, OBJ_RUBBER_OFF);
    */
    return 0;
}

int
AddPolyline_f(lua_State *L)
{
    /*
    if (context->argumentCount() != 1)   return debug("addPolyline() requires one argument");
    if (!context->argument(0).isArray()) return debug(TypeError, "addPolyline(): first argument is not an array");

    QVariantList varList = context->argument(0).toVariant().toList();
    int varSize = varList.size();
    if (varSize < 2) return debug(TypeError, "addPolyline(): array must contain at least two elements");
    if (varSize % 2) return debug(TypeError, "addPolyline(): array cannot contain an odd number of elements");

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
            return debug(TypeError, "addPolyline(): array contains one or more invalid elements");
        }
    }

    path.translate(-startX, -startY);

    _mainWin->nativeAddPolyline(startX, startY, path, OBJ_RUBBER_OFF);
    */
    return 0;
}

int
AddPath_f(lua_State *L)
{
    //TODO: parameter error checking
    qDebug("TODO: finish addPath command");
    return 0;
}

int
AddHorizontalDimension_f(lua_State *L)
{
    //TODO: parameter error checking
    qDebug("TODO: finish addHorizontalDimension command");
    return 0;
}

int
AddVerticalDimension_f(lua_State *L)
{
    //TODO: parameter error checking
    qDebug("TODO: finish addVerticalDimension command");
    return 0;
}

int
AddImage_f(lua_State *L)
{
    //TODO: parameter error checking
    qDebug("TODO: finish addImage command");
    return 0;
}

int
set_cursor_shape_f(lua_State *L)
{
    ScriptValue args[1];
    if (!unpack_args(L, "set_cursor_shape_f", args, "s")) {
        return 0;
    }

    QString shape = args[0].s;
    _mainWin->nativeSetCursorShape(shape);
    return 0;
}

int
calculate_angle_f(lua_State *L)
{
    ScriptValue args[4];
    if (!unpack_args(L, "calculate_angle_f", args, "rrrr")) {
        return 0;
    }

    EmbVector start = emb_vector(args[0].r, args[1].r);
    EmbVector end = emb_vector(args[2].r, args[3].r);
    //return QScriptValue(_mainWin->nativeCalculateAngle(x1, y1, x2, y2));
    return 0;
}

/* . */
int
calculate_distance_f(lua_State *L)
{
    ScriptValue args[4];
    if (!unpack_args(L, "calculate_distance_f", args, "rrrr")) {
        return 0;
    }
    EmbVector start = emb_vector(args[0].r, args[1].r);
    EmbVector end = emb_vector(args[2].r, args[3].r);
    EmbVector line = emb_vector_subtract(end, start);

    //return QScriptValue(emb_vector_length(line));
    return 0;
}

/* . */
int
perpendicular_distance_f(lua_State *L)
{
    ScriptValue args[6];
    if (!unpack_args(L, "perpendicular_distance_f", args, "rrrrrr")) {
        return 0;
    }

//    return QScriptValue(_mainWin->nativePerpendicularDistance(px, py, x1, y1, x2, y2));
    return 0;
}

/* . */
int
add_to_selection_f(lua_State *L)
{
    //TODO: finish
    return 0;
}

/* . */
int
cut_selected_f(lua_State *L)
{
    ScriptValue args[2];
    if (!unpack_args(L, "cut_selected_f", args, "rr")) {
        return 0;
    }

    EmbVector position = unpack_vector(args, 0);
    // TODO: cut selected at position
    return 0;
}

/* . */
int
copy_selected_f(lua_State *L)
{
    ScriptValue args[2];
    if (!unpack_args(L, "copy_selected_f", args, "rr")) {
        return 0;
    }

    EmbVector position = unpack_vector(args, 0);
    // TODO: copy selected at position
    return 0;
}

/* . */
int
paste_selected_f(lua_State *L)
{
    ScriptValue args[2];
    if (!unpack_args(L, "paste_selected_f", args, "rr")) {
        return 0;
    }

    EmbVector position = unpack_vector(args, 0);
    // TODO: paste selected at position
    return 0;
}

/* . */
int
move_selected_f(lua_State *L)
{
    ScriptValue args[2];
    if (!unpack_args(L, "move_selected_f", args, "rr")) {
        return 0;
    }

    EmbVector delta = unpack_vector(args, 0);
    View* gview = activeView();
    if (gview) {
        gview->moveSelected(delta.x, delta.y);
    }
    return 0;
}

/* . */
int
scale_selected_f(lua_State *L)
{
    ScriptValue args[3];
    if (!unpack_args(L, "scale_selected_f", args, "rrr")) {
        return 0;
    }

    double x = args[0].r;
    double y = args[1].r;
    double factor = args[2].r;

    if (factor <= 0.0) {
        debug("ERROR scale_selected_f: scale factor must be greater than zero");
        return 0;
    }

    _mainWin->nativeScaleSelected(x, y, factor);
    return 0;
}

/* . */
int
rotate_selected_f(lua_State *L)
{
    ScriptValue args[3];
    if (!unpack_args(L, "rotate_selected_f", args, "rrr")) {
        return 0;
    }

    _mainWin->nativeRotateSelected(args[0].r, args[1].r, args[2].r);
    return 0;
}

/* . */
int
mirror_selected_f(lua_State *L)
{
    ScriptValue args[4];
    if (!unpack_args(L, "mirror_selected_f", args, "rrrr")) {
        return 0;
    }

    _mainWin->nativeMirrorSelected(args[0].r, args[1].r, args[2].r, args[3].r);
    return 0;
}

/* Report the current x-position of the mouse and return it. */
int
mouse_x_f(lua_State *L)
{
    QGraphicsScene* scene = activeScene();
    double result = 0.0;
    if (scene) {
        qDebug("mouseX: %.50f", scene->property(SCENE_MOUSE_POINT).toPointF().x());
        result = scene->property(SCENE_MOUSE_POINT).toPointF().x();
    }
    lua_pushnumber(L, result);
    return 1;
}

/* Report the current y-position of the mouse and return it. */
int
mouse_y_f(lua_State *L)
{
    QGraphicsScene* scene = activeScene();
    double result = 0.0;
    if (scene) {
        qDebug("mouseY: %.50f", -scene->property(SCENE_MOUSE_POINT).toPointF().y());
        result = -scene->property(SCENE_MOUSE_POINT).toPointF().y();
    }
    lua_pushnumber(L, result);
    return 1;
}

/* Return the current x-position of the quicksnap position. */
int
qsnap_x_f(lua_State *L)
{
    QGraphicsScene* scene = activeScene();
    double result = 0.0;
    if (scene) {
        result = scene->property(SCENE_QSNAP_POINT).toPointF().x();
    }
    lua_pushnumber(L, result);
    return 1;
}

/* Return the current y-position of the quicksnap position. */
int
qsnap_y_f(lua_State *L)
{
    QGraphicsScene* scene = activeScene();
    double result = 0.0;
    if (scene) {
        result = -scene->property(SCENE_QSNAP_POINT).toPointF().y();
    }
    lua_pushnumber(L, result);
    return 1;
}

/* Lua in Embroidermodder 2 uses a 2 stage boot process.
 *
 * 1. Built-in style hookups to Embroidermodder2 features which are
 *    lua functions.
 * 2. Commands written in lua-only which represent the QActions used by
 *    the interface in the menus, toolbars and command line.
 *
 * Each command loaded in boot step 3 has up to 5 contexts
 * (see the list of defines in the headers: grep for "CONTEXT_") and will switch
 * depending on what context it is called in. This is enabled by a global
 * variable packed into `sc` called `context` which is checked during the
 * execution of the command's main.
 *
 * FIXME: loading of initation for each command needs a replacement in the
 * lua files.
 */
bool
MainWindow::script_env_boot(void)
{
    /* NOTE: The mainWin pointer can't be passed to lua functions
     * NOTE: so we make it file-scope.
     */
    _mainWin = this;

    /* Setting up Lua. */
    Lua = luaL_newstate();
    luaL_openlibs(Lua);

    /* TODO: Read into C closures in lua to make sure this should be 0.
    luaL_setfuncs(Lua, lua_registerables, 1);
    */
    for (int i=0; lua_registerables[i].name != NULL; i++) {
        lua_register(Lua, lua_registerables[i].name, lua_registerables[i].func);
    }

    int status = luaL_dofile(Lua, "scripts/boot.lua");
    if (status) {
        printf("ERROR: %d\n", status);
        qDebug("Failed to boot scripting environment.");
        return false;
    }

#if 0
void
MainWindow::load_command(const QString& cmdName)
{
    qDebug("javaLoadCommand(%s)", qPrintable(cmdName));
    QString appDir = qApp->applicationDirPath();
    QFile file(appDir + "/commands/" + cmdName + "/" + cmdName + ".js");
    file.open(QIODevice::ReadOnly);
    QString script(file.readAll());
    file.close();

    QSettings settings(appDir + "/commands/" + cmdName + "/" + cmdName + ".ini", QSettings::IniFormat);
    QString menuName = settings.value("Menu/Name",    "Lost & Found").toString();
    int menuPos = settings.value("Menu/Position", 0).toInt();
    QString toolbarName = settings.value("ToolBar/Name", "Lost & Found").toString();
    int toolbarPos = settings.value("ToolBar/Position", 0).toInt();
    QString toolTip = settings.value("Tips/ToolTip", "").toString();
    QString statusTip = settings.value("Tips/StatusTip", "").toString();
    QStringList aliases = settings.value("Prompt/Alias").toStringList();

    QAction* ACTION = createAction(cmdName, toolTip, statusTip, true);

    if (toolbarName.toUpper() != "NONE") {
        //If the toolbar doesn't exist, create it.
        if (!toolbarHash.value(toolbarName)) {
            QToolBar* tb = new QToolBar(toolbarName, this);
            tb->setObjectName("toolbar" + toolbarName);
            connect(tb, SIGNAL(topLevelChanged(bool)), this, SLOT(floatingChangedToolBar(bool)));
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

    foreach(QString alias, aliases) {
        prompt->addCommand(alias, cmdName);
    }
}
#endif

    return true;
}

void
MainWindow::script_env_free(void)
{
    lua_close(Lua);
}

/* NOTE: This has to run after script_env_boot because the command_map is
 * populated by the script files called by it.
 */
void MainWindow::createAllActions()
{
    qDebug("Creating All Actions...");
    QString appDir = qApp->applicationDirPath();
    QString theme = appDir + "/icons/" + settings.general_icon_theme;

    for (int i=0; i<(int)command_map.size(); i++) {
        QString tooltip = tr(qPrintable(command_map[i].tooltip));
        QString statustip = tr(qPrintable(command_map[i].statustip));
        QAction *ACTION = new QAction(
            QIcon(theme + "/" + command_map[i].icon + ".png"),
            tooltip, this);
        ACTION->setStatusTip(statustip);
        ACTION->setObjectName(command_map[i].icon);
        // TODO: Set What's This Context Help to statustip for now so there is some infos there.
        // Make custom whats this context help popup with more descriptive help than just
        // the status bar/tip one liner(short but not real long) with a hyperlink in the custom popup
        // at the bottom to open full help file description. Ex: like wxPython AGW's SuperToolTip.
        ACTION->setWhatsThis(statustip);
        // TODO: Finish All Commands ... <.<

        if (command_map[i].shortcut != "") {
            ACTION->setShortcut(command_map[i].shortcut);
        }
        if (command_map[i].checkable != 0) {
            ACTION->setCheckable(true);
            // FIXME: connect(ACTION, SIGNAL(toggled(bool)), this, SLOT(setTextBold(bool)));
        }
        connect(ACTION, &QAction::triggered, this,
            [=]() { runCommandMain(command_map[i].command); } );

        /*
        else if(icon == "windowcascade") connect(ACTION, SIGNAL(triggered()), mdiArea, SLOT(cascade()));
        else if(icon == "windowtile") connect(ACTION, SIGNAL(triggered()), mdiArea, SLOT(tile()));
        else if(icon == "windowclose") { ACTION->setShortcut(QKeySequence::Close);    connect(ACTION, SIGNAL(triggered()), this, SLOT(onCloseWindow()));   }
        else if(icon == "windowcloseall") connect(ACTION, SIGNAL(triggered()), mdiArea, SLOT(closeAllSubWindows()));
        else if(icon == "windownext") { ACTION->setShortcut(QKeySequence::NextChild);     connect(ACTION, SIGNAL(triggered()), mdiArea, SLOT(activateNextSubWindow()));     }
        else if(icon == "windowprevious") { ACTION->setShortcut(QKeySequence::PreviousChild); connect(ACTION, SIGNAL(triggered()), mdiArea, SLOT(activatePreviousSubWindow())); }
        */

        actionHash.insert(command_map[i].icon, ACTION);
    }

    actionHash.value("windowclose")->setEnabled(numOfDocs > 0);
    actionHash.value("designdetails")->setEnabled(numOfDocs > 0);
}

