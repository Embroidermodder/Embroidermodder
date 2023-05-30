/*
 *  Embroidermodder 2.
 *
 *  ------------------------------------------------------------
 *
 *  Copyright 2013-2023 The Embroidermodder Team
 *  Embroidermodder 2 is Open Source Software.
 *  See LICENSE for licensing terms.
 *
 *  ------------------------------------------------------------
 *
 *  Use Python's PEP7 style guide.
 *      https://peps.python.org/pep-0007/
 */

/**
 * \file mainwindow.cpp
 */

#include "embroidermodder.h"

#include <cerrno>
#include <iostream>
#include <fstream>
#include <string>

enum OBJ_LTYPE_VALUES {
    //CAD Linetypes
    OBJ_LTYPE_CONT = 0,
    OBJ_LTYPE_CENTER = 1,
    OBJ_LTYPE_DOT = 2,
    OBJ_LTYPE_HIDDEN = 3,
    OBJ_LTYPE_PHANTOM = 4,
    OBJ_LTYPE_ZIGZAG = 5,
    //Embroidery Stitchtypes
    OBJ_LTYPE_RUNNING = 6, // __________
    OBJ_LTYPE_SATIN = 7, // vvvvvvvvvv
    OBJ_LTYPE_FISHBONE = 8, // >>>>>>>>>>
};

enum OBJ_LWT_VALUES {
    OBJ_LWT_BYLAYER = -2,
    OBJ_LWT_BYBLOCK = -1,
    OBJ_LWT_DEFAULT =  0,
    OBJ_LWT_01 =  1,
    OBJ_LWT_02 =  2,
    OBJ_LWT_03 =  3,
    OBJ_LWT_04 =  4,
    OBJ_LWT_05 =  5,
    OBJ_LWT_06 =  6,
    OBJ_LWT_07 =  7,
    OBJ_LWT_08 =  8,
    OBJ_LWT_09 =  9,
    OBJ_LWT_10 = 10,
    OBJ_LWT_11 = 11,
    OBJ_LWT_12 = 12,
    OBJ_LWT_13 = 13,
    OBJ_LWT_14 = 14,
    OBJ_LWT_15 = 15,
    OBJ_LWT_16 = 16,
    OBJ_LWT_17 = 17,
    OBJ_LWT_18 = 18,
    OBJ_LWT_19 = 19,
    OBJ_LWT_20 = 20,
    OBJ_LWT_21 = 21,
    OBJ_LWT_22 = 22,
    OBJ_LWT_23 = 23,
    OBJ_LWT_24 = 24
};

enum OBJ_SNAP_VALUES {
    OBJ_SNAP_NULL =  0, //NOTE: Allow this enum to evaluate false
    OBJ_SNAP_ENDPOINT =  1,
    OBJ_SNAP_MIDPOINT =  2,
    OBJ_SNAP_CENTER =  3,
    OBJ_SNAP_NODE =  4,
    OBJ_SNAP_QUADRANT =  5,
    OBJ_SNAP_INTERSECTION =  6,
    OBJ_SNAP_EXTENSION =  7,
    OBJ_SNAP_INSERTION =  8,
    OBJ_SNAP_PERPENDICULAR =  9,
    OBJ_SNAP_TANGENT = 10,
    OBJ_SNAP_NEAREST = 11,
    OBJ_SNAP_APPINTERSECTION = 12,
    OBJ_SNAP_PARALLEL = 13
};

/**
 * .
 * If an action calls a script then there will be an entry in
 * config that is a StringList to be interpreted as a script.
 *
 * An alias is another entry in config that is also a StringList
 * containing just the name of the command it aliases.
 */
typedef struct Action__ {
    String icon; /*< The stub used for the icon and the basic command. */
    String command; /*< . */
    String tooltip;  /*< The label in the menus and the message that appears when
                         you hover over an icon. */
    String statustip; /*< The message that appears at the bottom of the . */
    String shortcut; /*< The keyboard shortcut for this action. */
} Action;

MainWindow* _mainWin = 0;
MdiArea* mdiArea = 0;
CmdPrompt* prompt = 0;
PropertyEditor* dockPropEdit = 0;
UndoEditor* dockUndoEdit = 0;
StatusBar* statusbar = 0;

QWizard* wizardTipOfTheDay;
QLabel* labelTipOfTheDay;
QCheckBox* checkBoxTipOfTheDay;

Dictionary settings_;
Dictionary dialog_;
Dictionary config;

std::unordered_map<String, StringList> scripts;

std::unordered_map<String, QAction*> actionHash;
std::unordered_map<String, QToolBar*> toolbarHash;
std::unordered_map<String, QMenu*> menuHash;
std::unordered_map<String, QMenu*> subMenuHash;

std::vector<Action> action_table;

StringList tokenize(String str, const char delim);
String convert_args_to_type(String label, StringList args,
    const char *args_template, NodeList a);

/**
 * ACTIONS
 * \todo these should all be static, since other files
 * use the actuator to call them.
 */
static String about_action(String args);
static String add_arc_action(String args);
static String add_circle_action(String args);
static String add_dim_leader_action(String args);
static String add_ellipse_action(String args);
static String add_geometry_action(String args);
static String add_horizontal_dimension_action(String args);
static String add_image_action(String args);
static String add_infinite_line_action(String args);
static String add_line_action(String args);
static String add_path_action(String args);
static String add_point_action(String args);
static String add_polygon_action(String args);
static String add_polyline_action(String args);
static String add_ray_action(String args);
static String add_rectangle_action(String args);
static String add_regular_polygon_action(String args);
static String add_rounded_rectangle_action(String args);
static String add_rubber_action(String args);
static String add_slot_action(String args);
static String add_text_multi_action(String args);
static String add_text_single_action(String args);
static String add_to_selection_action(String args);
static String add_triangle_action(String args);
static String add_vertical_dimension_action(String args);
static String alert_action(String args);
static String allow_rubber_action(String args);
static String append_history_action(String args);
static String append_prompt_history_action(String args);
static String calculate_angle_action(String args);
static String calculate_distance_action(String args);
static String changelog_action(String args);
static String clear_rubber_action(String args);
static String copy_action(String args);
static String copy_selected_action(String args);
static String cut_action(String args);
static String cut_selected_action(String args);
static String day_vision_action(String args);
static String delete_selected_action(String args);
static String design_details_action(String args);
static String do_nothing_action(String args);
static String end_action(String args);
static String error_action(String args);
static String help_action(String args);
static String icon_action(String args);
static String init_action(String args);
static String messagebox_action(String args);
static String mirror_selected_action(String args);
static String mouse_x_action(String args);
static String mouse_y_action(String args);
static String move_selected_action(String args);
static String new_action(String args);
static String night_vision_action(String args);
static String num_selected_action(String args);
static String open_action(String args);
static String pan_action(String args);
static String paste_action(String args);
static String paste_selected_action(String args);
static String perpendicular_distance_action(String args);
static String platform_action(String args);
static String preview_off_action(String args);
static String preview_on_action(String args);
static String print_action(String args);
static String print_area_action(String args);
static String qsnap_x_action(String args);
static String qsnap_y_action(String args);
static String quit_action(String args);
static String redo_action(String args);
static String rotate_selected_action(String args);
static String scale_selected_action(String args);
static String select_all_action(String args);
static String set_background_color_action(String args);
static String set_crosshair_color_action(String args);
static String set_cursor_shape_action(String args);
static String set_grid_color_action(String args);
static String set_prompt_prefix_action(String args);
static String set_rubber_filter_action(String args);
static String set_rubber_mode_action(String args);
static String set_rubber_point_action(String args);
static String set_rubber_text_action(String args);
static String settings_dialog_action(String args);
static String spare_rubber_action(String args);
static String tip_of_the_day_action(String args);
static String todo_action(String args);
static String undo_action(String args);
static String version_action(String args);
static String whats_this_action(String args);
static String window_action(String args);
static String zoom_action(String args);

/**
 *
 */
std::unordered_map<String, Command> command_map = {
    {"about", about_action},
    {"add_arc", add_arc_action},
    {"add_circle", add_circle_action},
    {"add_dim_leader", add_dim_leader_action},
    {"add_ellipse", add_ellipse_action},
    {"add", add_geometry_action},
    {"add_horizontal_dimension", add_horizontal_dimension_action},
    {"add_image", add_image_action},
    {"add_infinite_line", add_infinite_line_action},
    {"add_line", add_line_action},
    {"add_path", add_path_action},
    {"add_point", add_point_action},
    {"add_polygon", add_polygon_action},
    {"add_polyline", add_polyline_action},
    {"add_ray", add_ray_action},
    {"add_rectangle", add_rectangle_action},
    {"add_regular_polygon", add_regular_polygon_action},
    {"add_rounded_rectangle", add_rounded_rectangle_action},
    {"add_rubber", add_rubber_action},
    {"add_slot", add_slot_action},
    {"add_text_multi", add_text_multi_action},
    {"add_text_single", add_text_single_action},
    {"add_to_selection", add_to_selection_action},
    {"add_triangle", add_triangle_action},
    {"add_vertical_dimension", add_vertical_dimension_action},
    {"alert", alert_action},
    {"allow_rubber", allow_rubber_action},
    {"append_history", append_history_action},
    {"append_prompt_history", append_prompt_history_action},
    {"calculate_angle", calculate_angle_action},
    {"calculate_distance", calculate_distance_action},
    {"changelog", changelog_action},
    {"clear_rubber", clear_rubber_action},
    {"copy", copy_action},
    {"copy_selected", copy_selected_action},
    {"cut", cut_action},
    {"cut_selected", cut_selected_action},
    {"day_vision", day_vision_action},
    {"delete_selected", delete_selected_action},
    {"design_details", design_details_action},
    {"do_nothing", do_nothing_action},
    {"end", end_action},
    {"error", error_action},
    {"help", help_action},
    {"icon", icon_action},
    {"init", init_action},
    {"messagebox", messagebox_action},
    {"mirror_selected", mirror_selected_action},
    {"mouse_x", mouse_x_action},
    {"mouse_y", mouse_y_action},
    {"move_selected", move_selected_action},
    {"new", new_action},
    {"night_vision", night_vision_action},
    {"num_selected", num_selected_action},
    {"open", open_action},
    {"pan", pan_action},
    {"paste", paste_action},
    {"paste_selected", paste_selected_action},
    {"perpendicular_distance", perpendicular_distance_action},
    {"platform", platform_action},
    {"preview_off", preview_off_action},
    {"preview_on", preview_on_action},
    {"print", print_action},
    {"print_area", print_area_action},
    {"qsnap_x", qsnap_x_action},
    {"qsnap_y", qsnap_y_action},
    {"quit", quit_action},
    {"redo", redo_action},
    {"rotate_selected", rotate_selected_action},
    {"scale_selected", scale_selected_action},
    {"select_all", select_all_action},
    {"set_background_color", set_background_color_action},
    {"set_crosshair_color", set_crosshair_color_action},
    {"set_cursor_shape", set_cursor_shape_action},
    {"set_grid_color", set_grid_color_action},
    {"set_prompt_prefix", set_prompt_prefix_action},
    {"set_rubber_filter", set_rubber_filter_action},
    {"set_rubber_mode", set_rubber_mode_action},
    {"set_rubber_point", set_rubber_point_action},
    {"set_rubber_text", set_rubber_text_action},
    {"settings_dialog", settings_dialog_action},
    {"spare_rubber", spare_rubber_action},
    {"tip_of_the_day", tip_of_the_day_action},
    {"todo", todo_action},
    {"undo", undo_action},
    {"version", version_action},
    {"whats_this", whats_this_action},
    {"window", window_action},
    {"zoom", zoom_action},
};

StringList rubber_modes = {
    "CIRCLE_1P_RAD",
    "CIRCLE_1P_DIA",
    "CIRCLE_2P",
    "CIRCLE_3P",
    "CIRCLE_TTR",
    "CIRCLE_TTT",
    "DIMLEADER_LINE",
    "ELLIPSE_LINE",
    "ELLIPSE_MAJORDIAMETER_MINORRADIUS",
    "ELLIPSE_MAJORRADIUS_MINORRADIUS",
    "ELLIPSE_ROTATION",
    "LINE",
    "POLYGON",
    "POLYGON_INSCRIBE",
    "POLYGON_CIRCUMSCRIBE",
    "POLYLINE",
    "RECTANGLE",
    "TEXTSINGLE"
};

static QString
tr(const char *str)
{
    return _mainWin->tr(str);
}

/**
 * @brief set_node
 * @param node
 * @param value
 */
Node
node(bool value)
{
    Node node;
    node.type = BOOL_TYPE;
    node.b = value;
    return node;
}

/**
 * @brief create_node
 * @param mode
 * @return
 */
Node
node(int value)
{
    Node node;
    node.type = INT_TYPE;
    node.i = value;
    return node;
}

/**
 * @brief set_node
 * @param node
 * @param value
 */
Node
node(EmbReal value)
{
    Node node;
    node.type = REAL_TYPE;
    node.r = value;
    return node;
}

/**
 * @brief set_node
 * @param node
 * @param value
 */
Node
node(String value)
{
    Node node;
    node.type = STRING_TYPE;
    node.s = value;
    return node;
}

/**
 * @brief set_node
 * @param node
 * @param value
 */
Node
node(StringList value)
{
    Node node;
    node.type = STRING_LIST_TYPE;
    node.sl = value;
    return node;
}

/**
 * @brief set_node
 * @param node
 * @param value
 */
Node
node(EmbVector value)
{
    Node node;
    node.type = VECTOR_TYPE;
    node.v = value;
    return node;
}

/**
 * .
 * Note: on Unix we include the trailing separator.
 * For Windows compatibility we omit it.
 */
QString SettingsDir()
{
#if defined(Q_OS_UNIX) || defined(Q_OS_MAC)
    QString homePath = QDir::homePath();
    return homePath + "/.embroidermodder2/";
#else
    return "";
#endif
}

/**
 * @brief SettingsPath
 * @return
 */
QString SettingsPath()
{
    QString settingsPath = SettingsDir() + "settings.ini";
    return settingsPath;
}

/**
 * @brief to_string_vector
 * @param list
 * @return
 */
std::vector<String>
to_string_vector(QStringList list)
{
    std::vector<String> a;
    for (int i=0; i<(int)list.size(); i++) {
        a.push_back(list[i].toStdString());
    }
    return a;
}

/**
 * @brief MainWindow::settingsPrompt
 */
void
MainWindow::settingsPrompt()
{
    settings_dialog_action("Prompt");
}

/**
 * @brief settingsDialog
 * @param showTab
 */
String
settings_dialog_action(String showTab)
{
    Settings_Dialog dialog(QString::fromStdString(showTab));
    dialog.exec();
    return "";
}

/**
 * @brief MainWindow::stub_testing
 */
void
MainWindow::stub_testing()
{
    QMessageBox::warning(_mainWin, tr("Testing Feature"), tr("<b>This feature is in testing.</b>"));
}

/**
 * @brief MainWindow::quit
 */
void
MainWindow::quit()
{
    debug_message("quit()");
    if (settings.prompt_save_history) {
        prompt->saveHistory("prompt.log", settings.prompt_save_history_as_html);
        //TODO: get filename from settings
    }
    qApp->closeAllWindows();
    this->deleteLater();
    // Force the MainWindow destructor to run before exiting. Makes Valgrind "still reachable" happy :)
}

/**
 * @brief MainWindow::checkForUpdates
 */
void
MainWindow::checkForUpdates()
{
    debug_message("checkForUpdates()");
    //TODO: Check website for new versions, commands, etc...
}

/**
 * @brief no_argument_debug
 * @param function_name
 * @param args
 */
void
no_argument_debug(String function_name, String args)
{
    debug_message(function_name);
    if (args != "") {
        debug_message(function_name + " was passed an argument that was ignored");
    }
}

/**
 * @brief cut_action
 * @param args
 * @return
 */
static String
cut_action(String args)
{
    no_argument_debug("cut_action()", args);
    View* gview = activeView();
    if (gview) {
        gview->cut();
    }
    return "";
}

/**
 * @brief copy_action
 * @param args
 * @return
 */
static String
copy_action(String args)
{
    no_argument_debug("copy_action()", args);
    View* gview = activeView();
    if (gview) {
        gview->copy();
    }
    return "";
}

/**
 * @brief paste_action
 * @param args
 * @return
 */
static String
paste_action(String args)
{
    no_argument_debug("paste_action()", args);
    View* gview = activeView();
    if (gview) {
        gview->paste();
    }
    return "";
}

/**
 * @brief do_nothing_action This action intensionally does nothing.
 * @param args This is ignored, it's present to make it a Command.
 * @return An empty string.
 */
String
do_nothing_action(String args)
{
    no_argument_debug("do_nothing_action()", args);
    return "";
}

/**
 * @brief get_action_index
 * @param cmd
 * @return
 */
int
get_action_index(std::string cmd)
{
    for (int i=0; i<(int)action_table.size(); i++) {
        if (cmd == action_table[i].icon) {
            return i;
        }
    }
    return 0;
}

/**
 * @brief platformString
 * @return
 */
String
platformString(void)
{
    //TODO: Append QSysInfo to string where applicable.
    String os;
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
    debug_message("Platform: " + os);
    return os;
}

/**
 * .
 */
String
design_details_action(String args)
{
    no_argument_debug("about_action()", args);

    QGraphicsScene* scene = activeScene();
    if (scene) {
        EmbDetailsDialog dialog(scene, _mainWin);
        dialog.exec();
    }
    return "";
}

String
about_action(String args)
{
    no_argument_debug("about_action()", args);
    _mainWin->about();
    return "";
}

/**
 * @brief about_action
 * @param args
 * @return
 */
void
MainWindow::about(void)
{
    //TODO: QTabWidget for about dialog
    QApplication::setOverrideCursor(Qt::ArrowCursor);
    QString appDir = qApp->applicationDirPath();
    QString appName = QApplication::applicationName();
    QString title = "About " + appName;

    QDialog dialog(_mainWin);
    ImageWidget img(appDir + "/images/logo-small");
    QLabel text(appName + tr("\n\n") +
                          tr("https://www.libembroidery.org") +
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

/**
 * @brief whats_this_action
 * @param args
 * @return
 */
String
whats_this_action(String args)
{
    no_argument_debug("whats_this_action()", args);
    QWhatsThis::enterWhatsThisMode();
    return "";
}

/**
 * @brief print_action
 * @param args
 * @return
 */
String
print_action(String args)
{
    no_argument_debug("print_action()", args);
    MdiWindow* mdiWin = qobject_cast<MdiWindow*>(mdiArea->activeSubWindow());
    if (mdiWin) {
        mdiWin->print();
    }
    return "";
}

/**
 * @brief tip_of_the_day_action
 * @param args
 * @return
 */
String
tip_of_the_day_action(String args)
{
    no_argument_debug("tip_of_the_day_action()", args);
    _mainWin->tipOfTheDay();
    return "";
}

/**
 * @brief MainWindow::tipOfTheDay
 */
void
MainWindow::tipOfTheDay(void)
{
    QString appDir = qApp->applicationDirPath();

    wizardTipOfTheDay = new QWizard(_mainWin);
    wizardTipOfTheDay->setAttribute(Qt::WA_DeleteOnClose);
    wizardTipOfTheDay->setWizardStyle(QWizard::ModernStyle);
    wizardTipOfTheDay->setMinimumSize(550, 400);

    QWizardPage* page = new QWizardPage(wizardTipOfTheDay);

    ImageWidget* imgBanner = new ImageWidget(appDir + "/images/did-you-know.png", wizardTipOfTheDay);

    if (settings.general_current_tip >= config["tips"].sl.size()) {
        settings.general_current_tip = 0;
    }
    labelTipOfTheDay = new QLabel(config["tips"].sl[settings.general_current_tip].c_str(), wizardTipOfTheDay);
    labelTipOfTheDay->setWordWrap(true);

    QCheckBox* checkBoxTipOfTheDay = new QCheckBox(tr("&Show tips on startup"), wizardTipOfTheDay);
    checkBoxTipOfTheDay->setChecked(settings.general_tip_of_the_day);
    connect(checkBoxTipOfTheDay, SIGNAL(stateChanged(int)), _mainWin, SLOT(checkBoxTipOfTheDayStateChanged(int)));

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
    connect(wizardTipOfTheDay, SIGNAL(customButtonClicked(int)), _mainWin, SLOT(buttonTipOfTheDayClicked(int)));

    QList<QWizard::WizardButton> listTipOfTheDayButtons;
    listTipOfTheDayButtons << QWizard::Stretch << QWizard::CustomButton1 << QWizard::CustomButton2 << QWizard::CustomButton3;
    wizardTipOfTheDay->setButtonLayout(listTipOfTheDayButtons);

    wizardTipOfTheDay->exec();
}

/**
 * .
 */
void
MainWindow::checkBoxTipOfTheDayStateChanged(int checked)
{
    settings.general_tip_of_the_day = checked;
}

/**
 * .
 */
void
MainWindow::buttonTipOfTheDayClicked(int button)
{
    debug_message("buttonTipOfTheDayClicked(%d)" + std::to_string(button));
    StringList tips = config["tips"].sl;
    if (button == QWizard::CustomButton1) {
        if (settings.general_current_tip > 0) {
            settings.general_current_tip--;
        }
        else {
            settings.general_current_tip = tips.size()-1;
        }
        labelTipOfTheDay->setText(tips[settings.general_current_tip].c_str());
    }
    else if (button == QWizard::CustomButton2) {
        settings.general_current_tip++;
        if (settings.general_current_tip >= tips.size()) {
            settings.general_current_tip = 0;
        }
        labelTipOfTheDay->setText(tips[settings.general_current_tip].c_str());
    }
    else if (button == QWizard::CustomButton3) {
        wizardTipOfTheDay->close();
    }
}

/**
 * @brief help_action
 * @param args
 * @return
 */
static String
help_action(String args)
{
    debug_message("help()");

    // Open the HTML Help in the default browser
    QUrl helpURL("file:///" + qApp->applicationDirPath() + "/help/doc-index.html");
    QDesktopServices::openUrl(helpURL);

    //TODO: This is how to start an external program. Use this elsewhere...
    //QString program = "firefox";
    //QStringList arguments;
    //arguments << "help/commands.html";
    //QProcess *myProcess = new QProcess(this);
    //myProcess->start(program, arguments);
    return "";
}

/**
 * @brief changelog_action
 * @param args
 * @return
 */
static String
changelog_action(String args)
{
    debug_message("changelog()");

    QUrl changelogURL("help/changelog.html");
    QDesktopServices::openUrl(changelogURL);
    return "";
}

/**
 * @brief undo_action
 * @param args
 * @return
 */
static String
undo_action(String args)
{
    debug_message("undo()");
    QString prefix = prompt->getPrefix();
    if (dockUndoEdit->canUndo()) {
        actuator("set-prompt-prefix Undo "
            + dockUndoEdit->undoText().toStdString());
        /* \bug this won't append history because there's no mechanism
           for and empty string */
        actuator("append-history ");
        dockUndoEdit->undo();
        actuator("set-prompt-prefix " + prefix.toStdString());
    }
    else {
        prompt->alert("Nothing to undo");
        actuator("set-prompt-prefix " + prefix.toStdString());
    }
    return "";
}

/**
 * @brief redo_action
 * @param args
 * @return
 */
static String
redo_action(String args)
{
    debug_message("redo()");
    QString prefix = prompt->getPrefix();
    if (dockUndoEdit->canRedo()) {
        actuator("set-prompt-prefix Redo " + dockUndoEdit->redoText().toStdString());
        actuator("append-history ");
        dockUndoEdit->redo();
        actuator("set-prompt-prefix " + prefix.toStdString());
    }
    else {
        prompt->alert("Nothing to redo");
        actuator("set-prompt-prefix " + prefix.toStdString());
    }
    return "";
}

bool MainWindow::isShiftPressed()
{
    return shiftKeyPressedState;
}

void
MainWindow::setShiftPressed()
{
    shiftKeyPressedState = true;
}

void
MainWindow::setShiftReleased()
{
    shiftKeyPressedState = false;
}

// Icons
void
MainWindow::iconResize(int iconSize)
{
    this->setIconSize(QSize(iconSize, iconSize));
    layerSelector->setIconSize(QSize(iconSize*4, iconSize));
    colorSelector->setIconSize(QSize(iconSize, iconSize));
    linetypeSelector->setIconSize(QSize(iconSize*4, iconSize));
    lineweightSelector->setIconSize(QSize(iconSize*4, iconSize));
    //set the minimum combobox width so the text is always readable
    layerSelector->setMinimumWidth(iconSize*4);
    colorSelector->setMinimumWidth(iconSize*2);
    linetypeSelector->setMinimumWidth(iconSize*4);
    lineweightSelector->setMinimumWidth(iconSize*4);

    //TODO: low-priority: open app with iconSize set to 128. resize the icons to a smaller size.

    settings.general_icon_size = iconSize;
}

/**
 * @brief MainWindow::activeMdiWindow
 * @return
 */
MdiWindow *
MainWindow::activeMdiWindow()
{
    debug_message("activeMdiWindow()");
    MdiWindow* mdiWin = qobject_cast<MdiWindow*>(mdiArea->activeSubWindow());
    return mdiWin;
}

/**
 * @brief activeView
 * @return
 */
View *
activeView(void)
{
    debug_message("activeView()");
    MdiWindow* mdiWin = _mainWin->activeMdiWindow();
    if (mdiWin) {
        return mdiWin->gview;
    }
    return 0;
}

/**
 * @brief MainWindow::activeScene
 * @return
 */
QGraphicsScene *
activeScene()
{
    debug_message("activeScene()");
    MdiWindow* mdiWin = qobject_cast<MdiWindow*>(mdiArea->activeSubWindow());
    if (mdiWin) {
        return mdiWin->gscene;
    }
    return 0;
}

/**
 * @brief MainWindow::activeUndoStack
 * @return
 */
QUndoStack *
MainWindow::activeUndoStack()
{
    debug_message("activeUndoStack()");
    View* v = activeView();
    if (v) {
        QUndoStack* u = v->getUndoStack();
        return u;
    }
    return 0;
}

/**
 * @brief MainWindow::setUndoCleanIcon
 * @param opened
 */
void
MainWindow::setUndoCleanIcon(bool opened)
{
    dockUndoEdit->updateCleanIcon(opened);
}

/**
 * @brief MainWindow::updateAllViewScrollBars
 * @param val
 */
void
MainWindow::updateAllViewScrollBars(bool val)
{
    QList<QMdiSubWindow*> windowList = mdiArea->subWindowList();
    for (int i = 0; i < windowList.count(); ++i) {
        MdiWindow* mdiWin = qobject_cast<MdiWindow*>(windowList.at(i));
        if (mdiWin) {
            mdiWin->showViewScrollBars(val);
        }
    }
}

/**
 * @brief MainWindow::updateAllViewCrossHairColors
 * @param color
 */
void
MainWindow::updateAllViewCrossHairColors(QRgb color)
{
    QList<QMdiSubWindow*> windowList = mdiArea->subWindowList();
    for (int i = 0; i < windowList.count(); ++i) {
        MdiWindow* mdiWin = qobject_cast<MdiWindow*>(windowList.at(i));
        if (mdiWin) { mdiWin->setViewCrossHairColor(color); }
    }
}

/**
 * @brief MainWindow::updateAllViewBackgroundColors
 * @param color
 */
void
MainWindow::updateAllViewBackgroundColors(QRgb color)
{
    QList<QMdiSubWindow*> windowList = mdiArea->subWindowList();
    for (int i = 0; i < windowList.count(); ++i) {
        MdiWindow* mdiWin = qobject_cast<MdiWindow*>(windowList.at(i));
        if (mdiWin) {
            mdiWin->setViewBackgroundColor(color);
        }
    }
}

/**
 * @brief MainWindow::updateAllViewSelectBoxColors
 * @param colorL
 * @param fillL
 * @param colorR
 * @param fillR
 * @param alpha
 */
void
MainWindow::updateAllViewSelectBoxColors(QRgb colorL, QRgb fillL, QRgb colorR, QRgb fillR, int alpha)
{
    QList<QMdiSubWindow*> windowList = mdiArea->subWindowList();
    for (int i = 0; i < windowList.count(); ++i) {
        MdiWindow* mdiWin = qobject_cast<MdiWindow*>(windowList.at(i));
        if (mdiWin) {
            mdiWin->setViewSelectBoxColors(colorL, fillL, colorR, fillR, alpha);
        }
    }
}

/**
 * @brief MainWindow::updateAllViewGridColors
 * @param color
 */
void
MainWindow::updateAllViewGridColors(QRgb color)
{
    QList<QMdiSubWindow*> windowList = mdiArea->subWindowList();
    for (int i = 0; i < windowList.count(); ++i) {
        MdiWindow* mdiWin = qobject_cast<MdiWindow*>(windowList.at(i));
        if (mdiWin) {
            mdiWin->setViewGridColor(color);
        }
    }
}

/**
 * @brief MainWindow::updateAllViewRulerColors
 * @param color
 */
void
MainWindow::updateAllViewRulerColors(QRgb color)
{
    QList<QMdiSubWindow*> windowList = mdiArea->subWindowList();
    for (int i = 0; i < windowList.count(); ++i) {
        MdiWindow* mdiWin = qobject_cast<MdiWindow*>(windowList.at(i));
        if (mdiWin) {
            mdiWin->setViewRulerColor(color);
        }
    }
}

/**
 * @brief MainWindow::updatePickAddMode
 * @param val
 */
void
MainWindow::updatePickAddMode(bool val)
{
    settings.selection_mode_pickadd = val;
    dockPropEdit->updatePickAddModeButton(val);
}

/**
 * @brief MainWindow::pickAddModeToggled
 */
void
MainWindow::pickAddModeToggled()
{
    updatePickAddMode(!settings.selection_mode_pickadd);
}

/**
 * @brief MainWindow::makeLayerActive
 * @return
 */
String
make_layer_active_action(String args)
{
    no_argument_debug("make_layer_active_action()", args);
    debug_message("TODO: Implement makeLayerActive.");
    return "";
}

/**
 * @brief layer_manager_action
 * @param args
 * @return
 */
String
layer_manager_action(String args)
{
    no_argument_debug("layer_manager_action()", args);
    debug_message("TODO: Implement layerManager.");
    LayerManager layman(_mainWin);
    layman.exec();
    return "";
}

/**
 * @brief layer_previous_action
 * @param args
 * @return
 */
String
layer_previous_action(String args)
{
    debug_message("layerPrevious()");
    debug_message("TODO: Implement layerPrevious.");
    return "";
}

/**
 * @brief pan_action
 * @param mode
 * @return
 */
String
pan_action(String mode)
{
    View* gview = activeView();
    if (!gview) {
        return "ERROR: no active view found.";
    }
    QUndoStack* stack = gview->getUndoStack();
    if (!stack) {
        return "ERROR: no undo stack found.";
    }
    if (mode == "realtime") {
        debug_message("panrealtime()");
        gview->panRealTime();
        return "";
    }
    if (mode == "point") {
        debug_message("panpoint()");
        gview->panPoint();
        return "";
    }
    if (mode == "left") {
        debug_message("panLeft()");
        UndoableNavCommand* cmd = new UndoableNavCommand("PanLeft", gview, 0);
        stack->push(cmd);
        return "";
    }
    if (mode == "right") {
        debug_message("panRight()");
        UndoableNavCommand* cmd = new UndoableNavCommand("PanRight", gview, 0);
        stack->push(cmd);
        return "";
    }
    if (mode == "up") {
        debug_message("panUp()");
        UndoableNavCommand* cmd = new UndoableNavCommand("PanUp", gview, 0);
        stack->push(cmd);
        return "";
    }
    if (mode == "down") {
        debug_message("panDown()");
        UndoableNavCommand* cmd = new UndoableNavCommand("PanDown", gview, 0);
        stack->push(cmd);
        return "";
    }
    return "ERROR: pan subcommand not recognised.";
}

/**
 * @brief zoom_action
 * @param mode
 * @return
 */
String
zoom_action(String mode)
{
    View* gview = activeView();
    if (!gview) {
        return "ERROR: no active view found.";
    }
    QUndoStack* stack = gview->getUndoStack();
    if (!stack) {
        return "ERROR: no undo stack found.";
    }
    if (mode == "realtime") {
        debug_message("zoomRealtime()");
        debug_message("TODO: Implement zoomRealtime.");
        return "";
    }
    if (mode == "previous") {
        debug_message("zoomPrevious()");
        debug_message("TODO: Implement zoomPrevious.");
        return "";
    }
    if (mode == "window") {
        debug_message("zoomWindow()");
        gview->zoomWindow();
        return "";
    }
    if (mode == "dynamic") {
        debug_message("zoomDynamic()");
        debug_message("TODO: Implement zoomDynamic.");
        return "";
    }
    if (mode == "scale") {
        debug_message("zoomScale()");
        debug_message("TODO: Implement zoomScale.");
        return "";
    }
    if (mode == "center") {
        debug_message("zoomCenter()");
        debug_message("TODO: Implement zoomCenter.");
        return "";
    }
    if (mode == "in") {
        debug_message("zoomIn()");
        gview->zoomIn();
        return "";
    }
    if (mode == "out") {
        debug_message("zoomOut()");
        gview->zoomOut();
        return "";
    }
    if (mode == "selected") {
        debug_message("zoomSelected()");
        UndoableNavCommand* cmd = new UndoableNavCommand("ZoomSelected", gview, 0);
        stack->push(cmd);
        return "";
    }
    if (mode == "all") {
        debug_message("zoomAll()");
        debug_message("TODO: Implement zoomAll.");
        return "";
    }
    if (mode == "extents") {
        debug_message("zoomExtents()");
        UndoableNavCommand* cmd = new UndoableNavCommand("ZoomExtents", gview, 0);
        stack->push(cmd);
        return "";
    }
    return "ERROR: zoom subcommand not recognised.";
}

/**
 * @brief MainWindow::dayVision
 *  \todo Make day vision color settings.
 */
String
day_vision_action(String args)
{
    no_argument_debug("day_vision_action()", args);
    View* gview = activeView();
    if (gview) {
        gview->setBackgroundColor(qRgb(255,255,255));
        gview->setCrossHairColor(qRgb(0,0,0));
        gview->setGridColor(qRgb(0,0,0));
    }
    return "";
}

/**
 * @brief MainWindow::nightVision
 * \todo Make night vision color settings.
 */
String
night_vision_action(String args)
{
    no_argument_debug("night_vision_action()", args);
    View* gview = activeView();
    if (gview) {
        gview->setBackgroundColor(qRgb(0,0,0));
        gview->setCrossHairColor(qRgb(255,255,255));
        gview->setGridColor(qRgb(255,255,255));
    }
    return "";
}

/**
 * @brief MainWindow::layerSelectorIndexChanged
 * @param index
 */
void
MainWindow::layerSelectorIndexChanged(int index)
{
    debug_message("layerSelectorIndexChanged(%d)" + std::to_string(index));
}

/**
 * @brief MainWindow::colorSelectorIndexChanged
 * @param index
 */
void
MainWindow::colorSelectorIndexChanged(int index)
{
    debug_message("colorSelectorIndexChanged(%d)" + std::to_string(index));

    QComboBox* comboBox = qobject_cast<QComboBox*>(sender());
    QRgb newColor;
    if (comboBox) {
        bool ok = 0;
        //TODO: Handle ByLayer and ByBlock and Other...
        newColor = comboBox->itemData(index).toUInt(&ok);
        if (!ok)
            QMessageBox::warning(this, tr("Color Selector Conversion Error"), tr("<b>An error has occured while changing colors.</b>"));
    }
    else
        QMessageBox::warning(this, tr("Color Selector Pointer Error"), tr("<b>An error has occured while changing colors.</b>"));

    MdiWindow* mdiWin = qobject_cast<MdiWindow*>(mdiArea->activeSubWindow());
    if (mdiWin) { mdiWin->currentColorChanged(newColor); }
}

/**
 * .
 */
void
MainWindow::linetypeSelectorIndexChanged(int index)
{
    debug_message("linetypeSelectorIndexChanged(%d)" + std::to_string(index));
}

/**
 * @brief MainWindow::lineweightSelectorIndexChanged
 * @param index
 */
void
MainWindow::lineweightSelectorIndexChanged(int index)
{
    debug_message("lineweightSelectorIndexChanged(%d)" + std::to_string(index));
}

/**
 * @brief MainWindow::textFontSelectorCurrentFontChanged
 * @param font
 */
void
MainWindow::textFontSelectorCurrentFontChanged(const QFont& font)
{
    debug_message("textFontSelectorCurrentFontChanged()");
    settings.text_font = font.family().toStdString();
}

/**
 * @brief MainWindow::textSizeSelectorIndexChanged
 * @param index
 */
void
MainWindow::textSizeSelectorIndexChanged(int index)
{
    debug_message("textSizeSelectorIndexChanged(%d)" + std::to_string(index));
    settings.text_size = fabs(textSizeSelector->itemData(index).toReal()); //TODO: check that the toReal() conversion is ok
}

/**
 * @brief MainWindow::setTextFont
 * @param str
 */
void
MainWindow::setTextFont(QString str)
{
    textFontSelector->setCurrentFont(QFont(str));
    settings.text_font = str.toStdString();
}

/**
 * @brief MainWindow::setTextSize
 * @param num
 */
void
MainWindow::setTextSize(EmbReal num)
{
    settings.text_size = fabs(num);
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

/**
 * @brief MainWindow::getCurrentLayer
 * @return
 */
QString
MainWindow::getCurrentLayer()
{
    MdiWindow* mdiWin = qobject_cast<MdiWindow*>(mdiArea->activeSubWindow());
    if (mdiWin) {
        return mdiWin->curLayer;
    }
    return "0";
}

/**
 * @brief MainWindow::getCurrentColor
 * @return
 */
QRgb
MainWindow::getCurrentColor()
{
    MdiWindow* mdiWin = qobject_cast<MdiWindow*>(mdiArea->activeSubWindow());
    if (mdiWin) {
        return mdiWin->curColor;
    }
    return 0; //TODO: return color ByLayer
}

/**
 * @brief MainWindow::getCurrentLineType
 * @return
 */
QString MainWindow::getCurrentLineType()
{
    MdiWindow* mdiWin = qobject_cast<MdiWindow*>(mdiArea->activeSubWindow());
    if (mdiWin) {
        return mdiWin->curLineType;
    }
    return "ByLayer";
}

/**
 * @brief MainWindow::getCurrentLineWeight
 * @return
 */
QString MainWindow::getCurrentLineWeight()
{
    MdiWindow* mdiWin = qobject_cast<MdiWindow*>(mdiArea->activeSubWindow());
    if (mdiWin) {
        return mdiWin->curLineWeight;
    }
    return "ByLayer";
}

/**
 * @brief MainWindow::deletePressed
 */
void
MainWindow::deletePressed()
{
    debug_message("deletePressed()");
    QApplication::setOverrideCursor(Qt::WaitCursor);
    MdiWindow* mdiWin = qobject_cast<MdiWindow*>(mdiArea->activeSubWindow());
    if (mdiWin) {
        mdiWin->deletePressed();
    }
    QApplication::restoreOverrideCursor();
}

/**
 * @brief MainWindow::escapePressed
 */
void
MainWindow::escapePressed()
{
    debug_message("escapePressed()");
    QApplication::setOverrideCursor(Qt::WaitCursor);
    MdiWindow* mdiWin = qobject_cast<MdiWindow*>(mdiArea->activeSubWindow());
    if (mdiWin) {
        mdiWin->escapePressed();
    }
    QApplication::restoreOverrideCursor();

    actuator("end");
}

/**
 * @brief MainWindow::toggleGrid
 */
void
MainWindow::toggleGrid()
{
    debug_message("toggleGrid()");
    statusbar->statusBarGridButton->toggle();
}

/**
 * @brief MainWindow::toggleRuler
 */
void
MainWindow::toggleRuler()
{
    debug_message("toggleRuler()");
    statusbar->statusBarRulerButton->toggle();
}

/**
 * @brief MainWindow::toggleLwt
 */
void
MainWindow::toggleLwt()
{
    debug_message("toggleLwt()");
    statusbar->statusBarLwtButton->toggle();
}

/**
 * @brief MainWindow::promptHistoryAppended
 * @param txt
 */
void
MainWindow::promptHistoryAppended(QString  txt)
{
    MdiWindow* mdiWin = qobject_cast<MdiWindow*>(mdiArea->activeSubWindow());
    if (mdiWin) {
        mdiWin->promptHistoryAppended(txt);
    }
}

/**
 * @brief MainWindow::logPromptInput
 * @param txt
 */
void
MainWindow::logPromptInput(QString  txt)
{
    MdiWindow* mdiWin = qobject_cast<MdiWindow*>(mdiArea->activeSubWindow());
    if (mdiWin)
        mdiWin->logPromptInput(txt);
}

/**
 * @brief MainWindow::promptInputPrevious
 */
void
MainWindow::promptInputPrevious()
{
    MdiWindow* mdiWin = qobject_cast<MdiWindow*>(mdiArea->activeSubWindow());
    if (mdiWin) {
        mdiWin->promptInputPrevious();
    }
}

/**
 * .
 */
void
MainWindow::promptInputNext()
{
    MdiWindow* mdiWin = qobject_cast<MdiWindow*>(mdiArea->activeSubWindow());
    if (mdiWin) {
        mdiWin->promptInputNext();
    }
}

/**
 * @brief MessageBox
 * \a type
 * \a title
 * \a text
 */
static String
messagebox_action(String args)
{
    /*
    QString type, QString title, QString text
    QString msgType = type.toLower();
    if (msgType == "critical") {
        QMessageBox::critical(_mainWin, tr(qPrintable(title)), tr(qPrintable(text)));
    }
    else if (msgType == "information") {
        QMessageBox::information(_mainWin, tr(qPrintable(title)), tr(qPrintable(text)));
    }
    else if (msgType == "question") {
        QMessageBox::question(_mainWin, tr(qPrintable(title)), tr(qPrintable(text)));
    }
    else if (msgType == "warning") {
        QMessageBox::warning(_mainWin, tr(qPrintable(title)), tr(qPrintable(text)));
    }
    else {
        QMessageBox::critical(_mainWin, tr("Native MessageBox Error"), tr("Incorrect use of the native messageBox function."));
    }
    */
    return "";
}

/**
 * @brief PrintArea
 * \a x \a y \a w \a h
 *
 * EmbReal x, EmbReal y, EmbReal w, EmbReal h
 */
static String
print_area_action(String args)
{
    //    qDebug("nativeprint_area_action(%.2f, %.2f, %.2f, %.2f)", x, y, w, h);
    //TODO: Print Setup Stuff
    actuator("print");
    return "";
}

/**
 * @brief set_background_color_action
 * @param r
 * @param g
 * @param b
 *
 * uint8_t r, uint8_t g, uint8_t b
 */
static String
set_background_color_action(String args)
{
    /*
    EmbReal r = a[0].r;
    EmbReal g = a[1].r;
    EmbReal b = a[2].r;

    if (r < 0 || r > 255) { return context->throwError(QScriptContext::UnknownError, "set_background_color_action(): r value must be in range 0-255"); }
    if (g < 0 || g > 255) { return context->throwError(QScriptContext::UnknownError, "set_background_color_action(): g value must be in range 0-255"); }
    if (b < 0 || b > 255) { return context->throwError(QScriptContext::UnknownError, "set_background_color_action(): b value must be in range 0-255"); }

    settings.display_bg_color = qRgb(r,g,b);
    _mainWin->updateAllViewBackgroundColors(qRgb(r,g,b));
    */
    return "";
}

/**
 * @brief SetCrossHairColor
 * @param r
 * @param g
 * @param b
 */
static String
set_crosshair_color_action(uint8_t r, uint8_t g, uint8_t b)
{
    settings.display_crosshair_color = qRgb(r,g,b);
    _mainWin->updateAllViewCrossHairColors(qRgb(r,g,b));
    return "";
}

/**
 * @brief set_grid_color
 * @param r
 * @param g
 * @param b
 */
static String
set_grid_color_action(uint8_t r, uint8_t g, uint8_t b)
{
    settings.grid_color = qRgb(r,g,b);
    _mainWin->updateAllViewGridColors(qRgb(r,g,b));
    return "";
}

/**
 * @brief PreviewOn
 * @param clone
 * @param mode
 * @param x
 * @param y
 * @param data
 */
static String
preview_on_action(String clone, String mode, EmbReal x, EmbReal y, EmbReal data)
{
    View* gview = activeView();
    if (gview) {
        gview->previewOn(clone, mode, x, -y, data);
    }
    return "";
}

/**
 * @brief PreviewOff
 */
static String
preview_off_action(String args)
{
    View* gview = activeView();
    if (gview) {
        gview->previewOff();
    }
    return "";
}

/**
 * @brief ClearRubber
 */
static String
clear_rubber_action(String args)
{
    View* gview = activeView();
    if (gview) {
        gview->clearRubberRoom();
    }
    return "";
}

/**
 * @brief AllowRubber
 * @return
 */
static String
allow_rubber_action(String args)
{
    View* gview = activeView();
    if (gview) {
        return std::to_string(gview->allowRubber());
    }
    return "false";
}

/**
 * @brief SpareRubber
 * @param qint64 id
 */
static String
spare_rubber_action(String args)
{
    int64_t id = std::stoi(args);
    View* gview = activeView();
    if (gview) {
        gview->spareRubber(id);
    }

    //QString objID = QString::fromStdString(a[0].s).toUpper();

    /*
    if (objID == "PATH") {
        _mainWin->nativespare_rubber_action(SPARE_RUBBER_PATH);
    }
    else if (objID == "POLYGON") {
        _mainWin->nativespare_rubber_action(SPARE_RUBBER_POLYGON);
    }
    else if (objID == "POLYLINE") {
        _mainWin->nativespare_rubber_action(SPARE_RUBBER_POLYLINE);
    }
    else {
        bool ok = false;
        qint64 id = objID.toLongLong(&ok);
        if (!ok) return "TYPE ERROR: spare_rubber_action(): error converting object ID into an int64");
        _mainWin->nativespare_rubber_action(id);
    }
    */
    return "";
}

/**
 * .
 */
static String
set_rubber_mode_action(String args)
{
    /*
    String mode = QString::fromStdString(a[0].s).toUpper().toStdString();

    View* gview = activeView();
    if (gview) {
        if (contains(rubber_modes, mode)) {
            gview->setRubberMode("OBJ_RUBBER_" + mode);
        }
        else {
            return "ERROR: setRubberMode(): unknown rubberMode value";
            //return context->throwError(QScriptContext::UnknownError, "setRubberMode(): unknown rubberMode value");
        }
    }
    */
    return "";
}

/**
 * .
 * QString  key, EmbReal x, EmbReal y
 */
static String
set_rubber_point_action(String args)
{
    /*
    _mainWin->setRubberPoint(a[0].s.toUpper(), a[1].r, a[2].r);
    */
    View* gview = activeView();
    if (gview) {
        //gview->setRubberPoint(key, QPointF(x, -y));
    }
    return "";
}

/**
 * .
 */
static String
SetRubberText(QString key, QString txt)
{
    View* gview = activeView();
    if (gview) {
        gview->setRubberText(key, txt);
    }
    return "";
}

/**
 * @brief add_text_multi_action
 * @return
 *
 * QString str, EmbReal x, EmbReal y, EmbReal rot, bool fill, String rubberMode
 */
static String
add_text_multi_action(String args)
{
    /*
    _mainWin->nativeadd_text_multi_action(a[0].s, a[1].r, a[2].r, a[3].r, a[4].b, OBJ_RUBBER_OFF);
    */
    return "";
}

/**
 * @brief add_text_single_action
 * @return
 *
 * QString str, EmbReal x, EmbReal y, EmbReal rot, bool fill, String rubberMode
 */
static String
add_text_single_action(String args)
{
    /*
    _mainWin->nativeadd_text_single_action(a[0].s, a[1].r, a[2].r, a[3].r, a[4].b, OBJ_RUBBER_OFF);
    */
    View* gview = activeView();
    QGraphicsScene* gscene = gview->scene();
    QUndoStack* stack = gview->getUndoStack();
    if (gview && gscene && stack) {
        /*
        TextSingleObject* obj = new TextSingleObject(str, x, -y, _mainWin->getCurrentColor());
        obj->setObjectTextFont(QString::fromStdString(settings.text_font));
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
        if (rubberMode != "OBJ_RUBBER_OFF") {
            gview->addToRubberRoom(obj);
            gscene->addItem(obj);
            gscene->update();
        }
        else {
            UndoableAddCommand* cmd = new UndoableAddCommand(obj->data(OBJ_NAME).toString(), obj, gview, 0);
            stack->push(cmd);
        }
        */
    }
    return "";
}

/**
 * .
 * EmbReal x1, EmbReal y1, EmbReal x2, EmbReal y2, EmbReal rot
 */
static String
add_infinite_line_action(String args)
{
    /*
    //TODO: Node error checking
    debug_message("TODO: finish addInfiniteLine command");
    */
    return "";
}

/**
 * .
 * EmbReal x1, EmbReal y1, EmbReal x2, EmbReal y2, EmbReal rot
 */
static String
add_ray_action(String args)
{
    /*
    //TODO: Node error checking
    debug_message("TODO: finish addRay command");
    */
    return "";
}

/**
 * .
 * EmbReal x1, EmbReal y1, EmbReal x2, EmbReal y2, EmbReal rot, String rubberMode
 */
static String
add_line_action(String args)
{
    /*
    _mainWin->nativeadd_line_action(a[0].r, a[1].r, a[2].r, a[3].r, a[4].r, OBJ_RUBBER_OFF);
    View* gview = activeView();
    QGraphicsScene* gscene = gview->scene();
    QUndoStack* stack = gview->getUndoStack();
    if (gview && gscene && stack) {
        EmbLine line;
        line.start.x = x1;
        line.start.y = -y1;
        line.end.x = x2;
        line.end.y = -y2;
        LineObject* obj = new LineObject(line,_mainWin->getCurrentColor());
        obj->setRotation(-rot);
        obj->setObjectRubberMode(rubberMode);
        if (rubberMode != "OBJ_RUBBER_OFF") {
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
    return "";
}

/**
 * @brief add_triangle_action
 * @param args
 * @return
 */
static String
add_triangle_action(String args)
{
    /*
    _mainWin->nativeadd_triangle_action(a[0].r, a[1].r, a[2].r, a[3].r, a[4].r, a[5].r, a[6].r, a[7].b);
    */
    return "";
}

/**
 * @brief add_rectangle_action
 * @param args
 * @return
 */
static String
add_rectangle_action(String args)
{
    View* gview = activeView();
    if (!gview) {
        return "ERROR: no active view found.";
    }
    QGraphicsScene* gscene = gview->scene();
    if (!gscene) {
        return "ERROR: no graphics scene view found.";
    }
    QUndoStack* stack = gview->getUndoStack();
    if (stack) {
        StringList arg_list = tokenize(args, ' ');
        EmbReal x = std::stof(arg_list[0]);
        EmbReal y = std::stof(arg_list[1]);
        EmbReal w = std::stof(arg_list[2]);
        EmbReal h = std::stof(arg_list[3]);
        EmbReal rot = std::stof(arg_list[4]);
        bool fill = (arg_list[5] == "1");
        String rubberMode = arg_list[6];

        RectObject* obj = new RectObject(x, -y, w, -h,_mainWin->getCurrentColor());
        obj->setRotation(-rot);
        obj->setObjectRubberMode(rubberMode);
        //TODO: rect fill
        if (rubberMode != "OBJ_RUBBER_OFF") {
            gview->addToRubberRoom(obj);
            gscene->addItem(obj);
            gscene->update();
        }
        else {
            UndoableAddCommand* cmd = new UndoableAddCommand(obj->data(OBJ_NAME).toString(), obj, gview, 0);
            stack->push(cmd);
        }
    }
    return "";
}

/**
 * @brief add_rounded_rectangle_action
 * @param args
 * @return
 *
 * EmbReal x, EmbReal y, EmbReal w, EmbReal h, EmbReal rad, EmbReal rot, bool fill
 */
static String
add_rounded_rectangle_action(String args)
{
    /*
    _mainWin->nativeadd_rounded_rectangle_action(
        a[0].r, a[1].r, a[2].r, a[3].r, a[4].r, a[5].r, a[6].b);
    */
    return "";
}

/**
 * @brief add_arc_action
 * @param args
 * @return
 *
 * EmbReal startX, EmbReal startY, EmbReal midX, EmbReal midY, EmbReal endX, EmbReal endY, String rubberMode
 */
static String
add_arc_action(String args)
{
    View* gview = activeView();
    QGraphicsScene* scene = activeScene();
    if (gview && scene) {
        /*
        EmbArc arc;
        arc.start.x = startX;
        arc.start.x = -startY;
        arc.mid.x = midX;
        arc.mid.x = -midY;
        arc.end.x = endX;
        arc.end.x = -endY;
        ArcObject* arcObj = new ArcObject(arc,_mainWin->getCurrentColor());
        arcObj->setObjectRubberMode(rubberMode);
        if (rubberMode != "OBJ_RUBBER_OFF") {
            gview->addToRubberRoom(arcObj);
        }
        scene->addItem(arcObj);
        scene->update();
        */
    }
    return "";
}

/**
 * @brief add_circle_action
 * @return
 *
 * EmbReal centerX, EmbReal centerY, EmbReal radius, bool fill, String rubberMode
 */
static String
add_circle_action(String args)
{
    View* gview = activeView();
    QGraphicsScene* gscene = gview->scene();
    QUndoStack* stack = gview->getUndoStack();
    if (gview && gscene && stack) {
        EmbReal centerX = 0.0;
        EmbReal centerY = 0.0;
        EmbReal radius = 10.0;
        bool fill = false;
        String rubberMode = "OBJ_RUBBER_OFF";

        CircleObject* obj = new CircleObject(centerX, -centerY, radius,_mainWin->getCurrentColor());
        obj->setObjectRubberMode(rubberMode);
        //TODO: circle fill
        if (rubberMode != "OBJ_RUBBER_OFF") {
            gview->addToRubberRoom(obj);
            gscene->addItem(obj);
            gscene->update();
        }
        else {
            UndoableAddCommand* cmd = new UndoableAddCommand(obj->data(OBJ_NAME).toString(), obj, gview, 0);
            stack->push(cmd);
        }
    }
    return "";
}

/**
 * @brief AddEllipse
 * @param args
 * @return
 *
 * EmbReal centerX, EmbReal centerY, EmbReal width, EmbReal height, EmbReal rot, bool fill, String rubberMode
 */
static String
add_ellipse_action(String args)
{
    View* gview = activeView();
    QGraphicsScene* gscene = gview->scene();
    QUndoStack* stack = gview->getUndoStack();
    if (gview && gscene && stack) {
        /*
        EllipseObject* obj = new EllipseObject(centerX, -centerY, width, height,_mainWin->getCurrentColor());
        obj->setRotation(-rot);
        obj->setObjectRubberMode(rubberMode);
        //TODO: ellipse fill
        if (rubberMode != "OBJ_RUBBER_OFF") {
            gview->addToRubberRoom(obj);
            gscene->addItem(obj);
            gscene->update();
        }
        else {
            UndoableAddCommand* cmd = new UndoableAddCommand(obj->data(OBJ_NAME).toString(), obj, gview, 0);
            stack->push(cmd);
        }
        */
    }
    return "";
}

/**
 * @brief AddPoint
 * @param args
 * @return
 */
static String
add_point_action(EmbReal x, EmbReal y)
{
    View* gview = activeView();
    QUndoStack* stack = gview->getUndoStack();
    if (gview && stack)
    {
        PointObject* obj = new PointObject(x, -y,_mainWin->getCurrentColor());
        UndoableAddCommand* cmd = new UndoableAddCommand(obj->data(OBJ_NAME).toString(), obj, gview, 0);
        stack->push(cmd);
    }
    return "";
}

/**
 * @brief AddRegularPolygon
 * @return
 *
 * EmbReal centerX, EmbReal centerY, quint16 sides, uint8_t mode, EmbReal rad, EmbReal rot, bool fill
 */
static String
add_regular_polygon_action(String args)
{
    return "";
}

//NOTE: This native is different than the rest in that the Y+ is down (scripters need not worry about this)
// EmbReal startX, EmbReal startY, const QPainterPath& p, String rubberMode
static String
add_polyline_action(String args)
{
    /*
    QVariantList varList = a[0].toVariant().toList();
    int varSize = varList.size();
    if (varSize < 2) {
        return "TYPE ERROR: addPolyline(): array must contain at least two elements";
    }
    if (varSize % 2) {
        return "TYPE ERROR: addPolyline(): array cannot contain an odd number of elements";
    }

    bool lineTo = false;
    bool xCoord = true;
    EmbReal x = 0;
    EmbReal y = 0;
    EmbReal startX = 0;
    EmbReal startY = 0;
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
            return "TYPE ERROR: addPolyline(): array contains one or more invalid elements";
        }
    }

    path.translate(-startX, -startY);

    _mainWin->nativeAddPolyline(startX, startY, path, OBJ_RUBBER_OFF);
    */
    View* gview = activeView();
    QGraphicsScene* gscene = gview->scene();
    QUndoStack* stack = gview->getUndoStack();
    if (gview && gscene && stack) {
        /*
        PolylineObject* obj = new PolylineObject(startX, startY, p,_mainWin->getCurrentColor());
        obj->setObjectRubberMode(rubberMode);
        if (rubberMode != "OBJ_RUBBER_OFF") {
            gview->addToRubberRoom(obj);
            gscene->addItem(obj);
            gscene->update();
        }
        else {
            UndoableAddCommand* cmd = new UndoableAddCommand(obj->data(OBJ_NAME).toString(), obj, gview, 0);
            stack->push(cmd);
        }
        */
    }
    return "";
}

/**
 * .
 * \note This native is different than the rest in that
 * the Y+ is down (scripters need not worry about this).
 *
 * EmbReal startX, EmbReal startY, const QPainterPath& p, String rubberMode
 */
static String
add_path_action(String args)
{
    /*
    AddPath(NodeList a)
    // TODO: Node error checking
    debug_message("TODO: finish addPath command");
    */
    return "";
}

/**
 * .
 * EmbReal x1, EmbReal y1, EmbReal x2, EmbReal y2, EmbReal legHeight
 */
static String
add_horizontal_dimension_action(String args)
{
    /*
    AddHorizontalDimension(NodeList a)
    //TODO: Node error checking
    debug_message("TODO: finish addHorizontalDimension command");
    */
    return "";
}

/**
 * .
 * EmbReal x1, EmbReal y1, EmbReal x2, EmbReal y2, EmbReal legHeight
 */
static String
add_vertical_dimension_action(String args)
{
    /*
    AddVerticalDimension(NodeList a)
    //TODO: Node error checking
    debug_message("TODO: finish addVerticalDimension command");
    */
    return "";
}

/**
 * .
 * QString  img, EmbReal x, EmbReal y, EmbReal w, EmbReal h, EmbReal rot
 */
static String
add_image_action(String args)
{
    /*
    AddImage(NodeList a)
    //TODO: Node error checking
    debug_message("TODO: finish addImage command");
    */
    return "";
}

/**
 * .
 * EmbReal x1, EmbReal y1, EmbReal x2, EmbReal y2, EmbReal rot, String rubberMode
 */
static String
add_dim_leader_action(String args)
{
    /*
    AddDimLeader(NodeList a)
    _mainWin->nativeAddDimLeader(a[0].r, a[1].r, a[2].r, a[3].r, a[4].r, OBJ_RUBBER_OFF);
    */
    View* gview = activeView();
    QGraphicsScene* gscene = gview->scene();
    QUndoStack* stack = gview->getUndoStack();
    if (gview && gscene && stack) {
        /*
        DimLeaderObject* obj = new DimLeaderObject(x1, -y1, x2, -y2,_mainWin->getCurrentColor());
        obj->setRotation(-rot);
        obj->setObjectRubberMode(rubberMode);
        if (rubberMode != "OBJ_RUBBER_OFF") {
            gview->addToRubberRoom(obj);
            gscene->addItem(obj);
            gscene->update();
        }
        else {
            UndoableAddCommand* cmd = new UndoableAddCommand(obj->data(OBJ_NAME).toString(), obj, gview, 0);
            stack->push(cmd);
        }
        */
    }
    return "";
}

/**
 * .
 */
static String
set_cursor_shape_action(String str)
{
    View* gview = activeView();
    if (gview) {
        QString shape = QString::fromStdString(str).toLower();
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
    return "";
}

/**
 * @brief calculate_angle_action
 * @param args
 * @return
 */
static String
calculate_angle_action(String args)
{
    StringList arg_list = tokenize(args, ' ');
    EmbReal x1 = std::stof(arg_list[0]);
    EmbReal y1 = std::stof(arg_list[1]);
    EmbReal x2 = std::stof(arg_list[2]);
    EmbReal y2 = std::stof(arg_list[3]);
    return std::to_string(QLineF(x1, -y1, x2, -y2).angle());
}

/**
 * @brief calculate_distance
 * @param args
 * @return
 */
static String
calculate_distance_action(String args)
{
    StringList arg_list = tokenize(args, ' ');
    EmbReal x1 = std::stof(arg_list[0]);
    EmbReal y1 = std::stof(arg_list[1]);
    EmbReal x2 = std::stof(arg_list[2]);
    EmbReal y2 = std::stof(arg_list[3]);
    return std::to_string(QLineF(x1, y1, x2, y2).length());
}

/**
 * .
 */
static String
perpendicular_distance_action(String args)
{
    StringList arg_list = tokenize(args, ' ');
    EmbReal px = std::stof(arg_list[0]);
    EmbReal py = std::stof(arg_list[1]);
    EmbReal x1 = std::stof(arg_list[2]);
    EmbReal y1 = std::stof(arg_list[3]);
    EmbReal x2 = std::stof(arg_list[4]);
    EmbReal y2 = std::stof(arg_list[5]);
    QLineF line(x1, y1, x2, y2);
    QLineF norm = line.normalVector();
    EmbReal dx = px-x1;
    EmbReal dy = py-y1;
    norm.translate(dx, dy);
    QPointF iPoint;
    norm.intersects(line, &iPoint);
    return std::to_string(QLineF(px, py, iPoint.x(), iPoint.y()).length());
}

/**
 * @brief NumSelected
 * @param args
 * @return
 */
static String
num_selected_action(String args)
{
    View* gview = activeView();
    if (gview) {
        return std::to_string(gview->numSelected());
    }
    return "0";
}

/**
 * @brief add_to_selection_action
 * @param args
 * @return
 */
static String
add_to_selection_action(String args)
{
    return "";
}

/**
 * @brief DeleteSelected
 */
static String
delete_selected_action(String args)
{
    View* gview = activeView();
    if (gview) {
        gview->deleteSelected();
    }
    return "";
}

/**
 * @brief CutSelected
 * \a x
 * \a y
 */
static String
cut_selected_action(String args)
{
    /*
    _mainWin->nativeCutSelected(a[0].r, a[1].r);
    */
    return "";
}

/**
 * @brief CopySelected
 * \a x
 * \a y
 */
static String
copy_selected_action(String args)
{
    /*
    _mainWin->nativeCopySelected(a[0].r, a[1].r);
    */
    return "";
}

/**
 * @brief PasteSelected
 * \a x
 * \a y
 */
static String
paste_selected_action(String args)
{
    /*
    _mainWin->nativePasteSelected(a[0].r, a[1].r);
    */
    return "";
}

/**
 * @brief MoveSelected
 * \a dx
 * \a dy
 */
static String
move_selected_action(String args)
{
    StringList arg_list = tokenize(args, ' ');
    EmbReal dx = std::stof(arg_list[0]);
    EmbReal dy = std::stof(arg_list[1]);
    View* gview = activeView();
    if (gview) {
        gview->moveSelected(dx, -dy);
    }
    return "";
}

/**
 * @brief ScaleSelected
 * \a x
 * \a y
 * \a factor
 */
static String
scale_selected_action(String args)
{
    StringList arg_list = tokenize(args, ' ');
    EmbReal x = std::stof(arg_list[0]);
    EmbReal y = std::stof(arg_list[1]);
    EmbReal factor = std::stof(arg_list[2]);

    if (factor <= 0.0) {
        QMessageBox::critical(_mainWin,
            tr("ScaleFactor Error"),
            tr("Hi there. If you are not a developer, report this as a bug. "
            "If you are a developer, your code needs examined, and possibly your head too."));
    }

    View* gview = activeView();
    if (gview) {
        gview->scaleSelected(x, -y, factor);
    }
    return "";
}

/**
 * @brief RotateSelected
 * \a x
 * \a y
 * \a rot
 */
static String
rotate_selected_action(String args)
{
    StringList arg_list = tokenize(args, ' ');
    EmbReal x = std::stof(arg_list[0]);
    EmbReal y = std::stof(arg_list[1]);
    EmbReal rot = std::stof(arg_list[2]);

    View* gview = activeView();
    if (gview) {
        gview->rotateSelected(x, -y, -rot);
    }
    return "";
}

/**
 * @brief MirrorSelected
 * \a x1
 * \a y1
 * \a x2
 * \a y2
 */
static String
mirror_selected_action(String args)
{

    StringList arg_list = tokenize(args, ' ');
    EmbReal x1 = std::stof(arg_list[0]);
    EmbReal y1 = std::stof(arg_list[1]);
    EmbReal x2 = std::stof(arg_list[2]);
    EmbReal y2 = std::stof(arg_list[3]);

    View* gview = activeView();
    if (gview) {
        gview->mirrorSelected(x1, -y1, x2, -y2);
    }
    return "";
}

/**
 * @brief QSnapX
 * @return
 */
static String
qsnap_x_action(String args)
{
    QGraphicsScene* scene = activeScene();
    if (scene) {
        return std::to_string(scene->property("SCENE_QSNAP_POINT").toPointF().x());
    }
    return "0.0";
}

/**
 * @brief QSnapY
 * @return
 */
static String
qsnap_y_action(String args)
{
    QGraphicsScene* scene = activeScene();
    if (scene) {
        return std::to_string(-scene->property("SCENE_QSNAP_POINT").toPointF().y());
    }
    return "0.0";
}

/**
 * @brief MouseX
 * @return
 */
static String
mouse_x_action(String args)
{
    QGraphicsScene* scene = activeScene();
    if (scene) {
        qDebug("mouseX: %.50f", scene->property("SCENE_MOUSE_POINT").toPointF().x());
        return std::to_string(scene->property("SCENE_MOUSE_POINT").toPointF().x());
    }
    return "0.0";
}

/**
 * @brief MouseY
 * @return
 */
static String
mouse_y_action(String args)
{
    QGraphicsScene* scene = activeScene();
    if (scene) {
        qDebug("mouseY: %.50f", -scene->property("SCENE_MOUSE_POINT").toPointF().y());
        return std::to_string(-scene->property("SCENE_MOUSE_POINT").toPointF().y());
    }
    return "0.0";
}

/**
 * @brief construct_command
 * @param command
 * @param fmt
 * @return
 */
String
construct_command(String command, const char *fmt, ...)
{
    va_list args;
    va_start(args, fmt);
    for (int i=0; i<(int)strlen(fmt); i++) {
        if (fmt[i] == 's') {
            String s(va_arg(args, char*));
            command += s;
        }
        if (fmt[i] == 'i') {
            command += std::to_string(va_arg(args, int));
        }
        if (fmt[i] == 'b') {
            command += std::to_string(va_arg(args, bool));
        }
        if (fmt[i] == 'f') {
            command += std::to_string(va_arg(args, EmbReal));
        }
    }
    va_end(args);
    return command;
}

/**
 * @brief tokenize
 * @param str
 * @param delim
 * @return
 */
StringList
tokenize(String str, const char delim)
{
    StringList list;
    std::stringstream str_stream(str);
    String s;
    while (std::getline(str_stream, s, delim)) {
        list.push_back(s);
    }
    return list;
}

/**
 * .
 */
String
read_string_setting(toml_table_t *table, const char *key)
{
    toml_datum_t str = toml_string_in(table, key);
    if (!str.ok) {
        return "";
    }
    String s(str.u.s);
    free(str.u.s);
    return s;
}

/**
 * .
 */
std::vector<String>
read_string_list_setting(toml_table_t *table, const char *key)
{
    std::vector<String> str_list;
    toml_array_t* array = toml_array_in(table, key);
    for (int i=0; ; i++) {
        toml_datum_t str = toml_string_at(array, i);
        if (!str.ok) {
            break;
        }
        String s(str.u.s);
        str_list.push_back(s);
        free(str.u.s);
    }
    return str_list;
}

/**
 * \brief Read the settings from file which aren't editable by the user.
 * These files need to be placed in the install folder.
 *
 * Expected Keys for actions
 * String icon;
 *      The stub used for the icon and the basic command.
 * String command;
 * String tooltip;
 *      The label in the menus and the message that appears when
 *      you hover over an icon.
 * String statustip;
 *       The message that appears at the bottom of the .
 * String shortcut;
 *       The keyboard shortcut for this action.
 * StringList aliases;
 *       A list of all alternative commands, if empty only
 *       the icon sttring will be .
 * StringList script;
 *      If this is a compound action this will be a
 *      list of commands or it can allow for command line
 *      style command aliases. For example: icon16 would become
 *      the string list {"iconResize 16"}.
 */
int
read_configuration(void)
{
    String fname = qApp->applicationDirPath().toStdString() + "/config.toml";
    char error_buffer[200];
    FILE *f = fopen(fname.c_str(), "r");
    if (!f) {
        puts("ERROR: Failed to open settings file:");
        printf("%s", fname.c_str());
        return 0;
    }
    toml_table_t *settings_toml = toml_parse_file(f, error_buffer, sizeof(error_buffer));
    fclose(f);

    if (!settings_toml) {
        puts("ERROR: failed to parse config.toml, continuing with defaults.");
        return 0;
    }

    config["version"] = node(read_string_setting(settings_toml, "version"));
    std::vector<String> action_labels =
        read_string_list_setting(settings_toml, "actions_");

    for (int i=0; i<(int)action_labels.size(); i++) {
        Action action;
        String label = "ACTION_" + action_labels[i];
        toml_table_t *table = toml_table_in(settings_toml, label.c_str());
        action.icon = read_string_setting(table, "icon");
        action.command = read_string_setting(table, "command");
        action.shortcut = read_string_setting(table, "shortcut");
        action.tooltip = read_string_setting(table, "tooltip");
        action.statustip = read_string_setting(table, "statustip");
        action_table.push_back(action);
    }

    StringList string_lists_list = read_string_list_setting(settings_toml, "string_lists");
    for (int i=0; i<(int)string_lists_list.size(); i++) {
        String key = string_lists_list[i];
        config[key] = node(read_string_list_setting(settings_toml, key.c_str()));
    }

    toml_free(settings_toml);

    return 1;
}

/**
 * @brief .
 */
bool
validRGB(int r, int g, int b)
{
    bool result = (r>=0);
    result &= (r<256);
    result &= (g>=0);
    result &= (g<256);
    result &= (b>=0);
    result &= (b<256);
    return result;
}

/**
 * @brief disable_action
 * @param variable
 * @return
 */
String
disable_action(String variable)
{
    if (variable == "text-angle") {
        settings.text_angle = false;
        return "";
    }
    if (variable == "text-bold") {
        settings.text_style_bold = false;
        return "";
    }
    if (variable == "text-italic") {
        settings.text_style_italic = false;
        return "";
    }
    if (variable == "text-underline") {
        settings.text_style_underline = false;
        return "";
    }
    if (variable == "text-strikeout") {
        settings.text_style_strikeout = false;
        return "";
    }
    if (variable == "text-overline") {
        settings.text_style_overline = false;
        return "";
    }
    if (variable == "prompt-rapid-fire") {
        prompt->disableRapidFire();
        return "";
    }
    if (variable == "move-rapid-fire") {
        View* gview = activeView();
        if (gview) {
            gview->disableMoveRapidFire();
        }
        return "";
    }
    return "";
}

/**
 * @brief MainWindow::MainWindow
 */
MainWindow::MainWindow() : QMainWindow(0)
{
    QString appDir = qApp->applicationDirPath();
    read_configuration();
    read_settings();

    //Verify that files/directories needed are actually present.
    QFileInfo check = QFileInfo(appDir + "/help");
    if (!check.exists())
        QMessageBox::critical(_mainWin, tr("Path Error"), tr("Cannot locate: ") + check.absoluteFilePath());
    check = QFileInfo(appDir + "/icons");
    if (!check.exists())
        QMessageBox::critical(_mainWin, tr("Path Error"), tr("Cannot locate: ") + check.absoluteFilePath());
    check = QFileInfo(appDir + "/images");
    if (!check.exists())
        QMessageBox::critical(_mainWin, tr("Path Error"), tr("Cannot locate: ") + check.absoluteFilePath());
    check = QFileInfo(appDir + "/samples");
    if (!check.exists())
        QMessageBox::critical(_mainWin, tr("Path Error"), tr("Cannot locate: ") + check.absoluteFilePath());
    check = QFileInfo(appDir + "/translations");
    if (!check.exists())
        QMessageBox::critical(_mainWin, tr("Path Error"), tr("Cannot locate: ") + check.absoluteFilePath());

    QString lang = QString::fromStdString(settings.general_language);
    debug_message("language: " + lang.toStdString());
    if (lang == "system")
        lang = QLocale::system().languageToString(QLocale::system().language()).toLower();

    //Load translations for the Embroidermodder 2 GUI
    QTranslator translatorEmb;
    translatorEmb.load(appDir + "/translations/" + lang + "/embroidermodder2_" + lang);
    qApp->installTranslator(&translatorEmb);

    //Load translations for the commands
    QTranslator translatorCmd;
    translatorCmd.load(appDir + "/translations/" + lang + "/commands_" + lang);
    qApp->installTranslator(&translatorCmd);

    //Load translations provided by Qt - this covers dialog buttons and other common things.
    QTranslator translatorQt;
    translatorQt.load("qt_" + QLocale::system().name(), QLibraryInfo::location(QLibraryInfo::TranslationsPath)); //TODO: ensure this always loads, ship a copy of this with the app
    qApp->installTranslator(&translatorQt);

    //Init
    _mainWin = this;

    QPoint pos = QPoint(settings.position_x, settings.position_y);
    QSize size = QSize(settings.size_x, settings.size_y);

    move(pos);
    resize(size);

    //Menus
    menuHash["file"] = new QMenu(tr("&File"), this);
    menuHash["edit"] = new QMenu(tr("&Edit"), this);
    menuHash["view"] = new QMenu(tr("&View"), this);
    menuHash["settings"] = new QMenu(tr("&Settings"), this);
    menuHash["window"] = new QMenu(tr("&Window"), this);
    menuHash["help"] = new QMenu(tr("&Help"), this);
    menuHash["draw"] = new QMenu(tr("&Draw"), this);

    //SubMenus
    subMenuHash["recent"] = new QMenu(tr("Open &Recent"), this);
    subMenuHash["zoom"] = new QMenu(tr("&Zoom"), this);
    subMenuHash["pan"] = new QMenu(tr("&Pan"), this);

    //Toolbars
    toolbarHash["file"] = addToolBar(tr("File"));
    toolbarHash["edit"] = addToolBar(tr("Edit"));
    toolbarHash["view"] = addToolBar(tr("View"));
    toolbarHash["zoom"] = addToolBar(tr("Zoom"));
    toolbarHash["pan"] = addToolBar(tr("Pan"));
    toolbarHash["icon"] = addToolBar(tr("Icon"));
    toolbarHash["help"] = addToolBar(tr("Help"));
    toolbarHash["layer"] = addToolBar(tr("Layer"));
    toolbarHash["properties"] = addToolBar(tr("Properties"));
    toolbarHash["text"] = addToolBar(tr("Text"));
    toolbarHash["prompt"] = addToolBar(tr("Command Prompt"));
    toolbarHash["draw"] = addToolBar(tr("Draw"));

    //Selectors
    layerSelector = new QComboBox(this);
    colorSelector = new QComboBox(this);
    linetypeSelector = new QComboBox(this);
    lineweightSelector = new QComboBox(this);
    textFontSelector = new QFontComboBox(this);
    textSizeSelector = new QComboBox(this);

    numOfDocs = 0;
    docIndex = 0;

    shiftKeyPressedState = false;

    setWindowIcon(create_icon("app"));
    setMinimumSize(800, 480); //Require Minimum WVGA

    loadFormats();

    //create the mdiArea
    QFrame* vbox = new QFrame(this);
    QVBoxLayout* layout = new QVBoxLayout(vbox);
    //layout->setMargin(0);
    vbox->setFrameStyle(QFrame::StyledPanel | QFrame::Sunken);
    mdiArea = new MdiArea(vbox);
    mdiArea->useBackgroundLogo(settings.general_mdi_bg_use_logo);
    mdiArea->useBackgroundTexture(settings.general_mdi_bg_use_texture);
    mdiArea->useBackgroundColor(settings.general_mdi_bg_use_color);
    mdiArea->setBackgroundLogo(QString::fromStdString(settings.general_mdi_bg_logo));
    mdiArea->setBackgroundTexture(QString::fromStdString(settings.general_mdi_bg_texture));
    mdiArea->setBackgroundColor(QColor(settings.general_mdi_bg_color));
    mdiArea->setViewMode(QMdiArea::TabbedView);
    mdiArea->setHorizontalScrollBarPolicy(Qt::ScrollBarAsNeeded);
    mdiArea->setVerticalScrollBarPolicy(Qt::ScrollBarAsNeeded);
    mdiArea->setActivationOrder(QMdiArea::ActivationHistoryOrder);
    layout->addWidget(mdiArea);
    setCentralWidget(vbox);

    //create the Command Prompt
    prompt = new CmdPrompt();
    prompt->setFocus(Qt::OtherFocusReason);
    this->setFocusProxy(prompt);
    mdiArea->setFocusProxy(prompt);

    prompt->setPromptTextColor(QColor(settings.prompt_text_color));
    prompt->setPromptBackgroundColor(QColor(settings.prompt_bg_color));

    connect(prompt, SIGNAL(startCommand(QString)), this, SLOT(logPromptInput(QString)));

    connect(prompt, SIGNAL(startCommand(QString)), this, SLOT(runCommandMain(QString)));
    connect(prompt, SIGNAL(runCommand(QString,QString)), this, SLOT(runCommandPrompt(QString,QString)));

    connect(prompt, SIGNAL(deletePressed()), this, SLOT(deletePressed()));
    //TODO: connect(prompt, SIGNAL(tabPressed()), this, SLOT(someUnknownSlot()));
    connect(prompt, SIGNAL(escapePressed()), this, SLOT(escapePressed()));
    connect(prompt, SIGNAL(upPressed()), this, SLOT(promptInputPrevious()));
    connect(prompt, SIGNAL(downPressed()), this, SLOT(promptInputNext()));
    connect(prompt, SIGNAL(F1Pressed()), this, SLOT(help()));
    //TODO: connect(prompt, SIGNAL(F2Pressed()), this, SLOT(floatHistory()));
    //TODO: connect(prompt, SIGNAL(F3Pressed()), this, SLOT(toggleQSNAP()));
    connect(prompt, SIGNAL(F4Pressed()), this, SLOT(toggleLwt())); //TODO: typically this is toggleTablet(), make F-Keys customizable thru settings
    //TODO: connect(prompt, SIGNAL(F5Pressed()), this, SLOT(toggleISO()));
    //TODO: connect(prompt, SIGNAL(F6Pressed()), this, SLOT(toggleCoords()));
    connect(prompt, SIGNAL(F7Pressed()), this, SLOT(toggleGrid()));
    //TODO: connect(prompt, SIGNAL(F8Pressed()), this, SLOT(toggleORTHO()));
    //TODO: connect(prompt, SIGNAL(F9Pressed()), this, SLOT(toggleSNAP()));
    //TODO: connect(prompt, SIGNAL(F10Pressed()), this, SLOT(togglePOLAR()));
    //TODO: connect(prompt, SIGNAL(F11Pressed()), this, SLOT(toggleQTRACK()));
    connect(prompt, SIGNAL(F12Pressed()), this, SLOT(toggleRuler()));
    connect(prompt, SIGNAL(cutPressed()), this, SLOT(cut_action()));
    connect(prompt, SIGNAL(copyPressed()), this, SLOT(copy_action()));
    connect(prompt, SIGNAL(pastePressed()), this, SLOT(paste_action()));
    connect(prompt, SIGNAL(selectAllPressed()), this, SLOT(selectAll()));
    connect(prompt, SIGNAL(undoPressed()), this, SLOT(undo()));
    connect(prompt, SIGNAL(redoPressed()), this, SLOT(redo()));

    connect(prompt, SIGNAL(shiftPressed()), this, SLOT(setShiftPressed()));
    connect(prompt, SIGNAL(shiftReleased()), this, SLOT(setShiftReleased()));

    connect(prompt, SIGNAL(showSettings()), this, SLOT(settingsPrompt()));

    connect(prompt, SIGNAL(historyAppended(QString)), this, SLOT(promptHistoryAppended(QString)));

    //create the Object Property Editor
    dockPropEdit = new PropertyEditor(appDir + "/icons/" + QString::fromStdString(settings.general_icon_theme), settings.selection_mode_pickadd, prompt, this);
    addDockWidget(Qt::LeftDockWidgetArea, dockPropEdit);
    connect(dockPropEdit, SIGNAL(pickAddModeToggled()), this, SLOT(pickAddModeToggled()));

    //create the Command History Undo Editor
    dockUndoEdit = new UndoEditor(appDir + "/icons/" + QString::fromStdString(settings.general_icon_theme), prompt, this);
    addDockWidget(Qt::LeftDockWidgetArea, dockUndoEdit);

    //setDockOptions(QMainWindow::AnimatedDocks | QMainWindow::AllowTabbedDocks | QMainWindow::VerticalTabs); //TODO: Load these from settings
    //tabifyDockWidget(dockPropEdit, dockUndoEdit); //TODO: load this from settings

    statusbar = new StatusBar(this);
    this->setStatusBar(statusbar);

    createAllActions();
    createAllMenus();
    createAllToolbars();

    iconResize(settings.general_icon_size);
    updateMenuToolbarStatusbar();

    //Show date in statusbar after it has been updated
    QDate date = QDate::currentDate();
    QString datestr = date.toString("MMMM d, yyyy");
    statusbar->showMessage(datestr);

    showNormal();

    if (settings.general_tip_of_the_day) {
        actuator("tip-of-the-day");
    }
}

/**
 * @brief MainWindow::~MainWindow
 */
MainWindow::~MainWindow()
{
    debug_message("MainWindow::Destructor()");

    //Prevent memory leaks by deleting any unpasted objects
    qDeleteAll(cutCopyObjectList.begin(), cutCopyObjectList.end());
    cutCopyObjectList.clear();
}

/**
 * @brief MainWindow::createAllActions
 *
 * \todo Set What's This Context Help to statusTip for now so there is some infos there.
 * Make custom whats this context help popup with more descriptive help than just
 * the status bar/tip one liner(short but not real long) with a hyperlink in the custom popup
 * at the bottom to open full help file description. Ex: like wxPython AGW's SuperToolTip.
ACTION->setWhatsThis(statusTip);
 * \todo Finish All Commands ... <.<
 */
void
MainWindow::createAllActions()
{
    debug_message("Creating All Actions...");

    for (int i=0; i<action_table.size(); i++) {
        Action action = action_table[i];

        QIcon icon = create_icon(QString::fromStdString(action.icon));
        QAction *ACTION = new QAction(icon, QString::fromStdString(action.tooltip), this);
        ACTION->setStatusTip(QString::fromStdString(action.statustip));
        ACTION->setObjectName(QString::fromStdString(action.icon));
        if (action.shortcut != "") {
            ACTION->setShortcut(
                QKeySequence(QString::fromStdString(action.shortcut))
            );
        }

        if (   (action.icon == "textbold")
            || (action.icon == "textitalic")
            || (action.icon == "textunderline")
            || (action.icon == "textstrikeout")
            || (action.icon == "textoverline")) {
            ACTION->setCheckable(true);
        }

        std::string command = action.command;
        auto f = [=](){ actuator(command); };
        connect(ACTION, &QAction::triggered, this, f);
        actionHash[action.icon] = ACTION;
    }

    actionHash["windowclose"]->setEnabled(numOfDocs > 0);
    actionHash["designdetails"]->setEnabled(numOfDocs > 0);
}

/**
 * @brief MainWindow::run_script_file
 * @param fname The path of the script to run.
 */
String
run_script_file(String fname)
{
    String output = "", line;
    std::ifstream file(fname);
    while (std::getline(file, line)) {
        output += actuator(line);
    }
    return output;
}

/**
 * @brief A basic line-by-line script processor to allow for extensions to the program.
 *
 * Since the actuator uses command line style parsing, a script is just a text
 * file with each line a compatible command.
 *
 * It should be stressed that this has no control flow on purpose. We don't want
 * this to be hacked into a full scripting language that could cause havoc on
 * the user's system.
 *
 * However, it may be useful to set and get variables and define macros:
 * neither of these will allow for endless loops, stack overflow or other
 * problems that third-party scripts could introduce.
 *
 *     example.sh
 *     ------------------------------------------------------------------
 *     # Save characters by defining functions.
 *     # The syntax features
 *     # Semi-colon ';' seperates out lines like in bash.
 *     # The line ending is the end of the function, but the style
 *     # is a shell function, so we need to write the end brace.
 *
 *     donut() { circle $1 $2 $3 $5 ; circle $1 $2 $4 $5 }
 *
 *     donut 10 20 20 black
 *     donut 20 40 20 black
 *     ------------------------------------------------------------------
 */
String
run_script(StringList script)
{
    String output = "";
    for (int i=0; i<(int)script.size(); i++) {
        output += actuator(script[i]);
    }
    return output;
}

/**
 * @brief MainWindow::actuator
 * @param command
 *
 *
 * RUN COMMAND
 * -----------
 * QAction* act = qobject_cast<QAction*>(sender());
 * if (act) {
 *     prompt->endCommand();
 *     prompt->setCurrentText(act->objectName());
 *     prompt->processInput();
 * }
 *
 * INIT
 * ----
 * QString fileName = "commands/" + cmd + "/" + cmd + ".js";
 * if (!getSettingsSelectionModePickFirst()) { actuator("clear-selection"); }
 * TODO: Uncomment this line when post-selection is available
 * engine->evaluate(cmd + "_main(void)", fileName);
 *
 * PROMPT
 * ------
 * QString fileName = "commands/" + cmd + "/" + cmd + ".js";
 * NOTE: Replace any special characters that will cause a syntax error
 * QString safeStr = str;
 * safeStr.replace("\\", "\\\\");
 * safeStr.replace("\'", "\\\'");
 *
 * if (prompt->isRapidFireEnabled()) {
 *     engine->evaluate(cmd + "_prompt('" + safeStr + "')", fileName);
 * }
 * else {
 *     engine->evaluate(cmd + "_prompt('" + safeStr.toUpper() + "')", fileName);
 * }
 */
String
actuator(String line)
{
    NodeList a;
    std::vector<String> list = tokenize(line, ' ');
    String command = list[0];
    list.erase(list.begin());

    auto iter = command_map.find(command);
    if (iter != command_map.end()) {
        int from = std::min(line.size(), command.size() + 1);
        String args = line.substr(from);
        String result = iter->second(args);
        if (result != "") {
            return "<br/>" + result;
        }
        return "";
    }

    auto script = scripts.find(command);
    if (script != scripts.end()) {
        String result = run_script(script->second);
        if (result != "") {
            return "<br/>" + result;
        }
        return "";
    }

    return "<br/><font color=\"red\">Unknown command \"" + command
           + "\". Press F1 for help.</font>";
}

/**
 * @brief add_geometry_action
 * @param args
 * @return
 */
static String
add_geometry_action(String args)
{
    StringList list = tokenize(args, ' ');
    String command = list[0];
    args = args.substr(std::min(command.size()+1, args.size()));
    StringList subcommands = {
        "arc",
        "circle",
        "ellipse",
        "horizontal_dimension",
        "image",
        "path",
        "point",
        "polygon",
        "polyline",
        "rectangle",
        "regular_polygon",
        "vertical_dimension",
        "dim_leader",
        "infinite_line",
        "ray",
        "line",
        "triangle",
        "text_multi",
        "text_single",
        "rounded-rectangle"
    };
    if (contains(subcommands, command)) {
        return actuator("add_" + command + " " + args);
    }

    return "The add subcommand is not recognised.";
}

// \todo This is so more than 1 rubber object can exist at one time without updating all rubber objects at once
static String
set_rubber_filter_action(String args)
{
    return args;
}

/**
 * @brief alert_action
 * @param args
 * @return
 */
static String
alert_action(String args)
{
    prompt->alert(QString::fromStdString(args));
    return "";
}

/**
 * @brief end_action
 * @param args
 * @return
 */
static String
end_action(String args)
{
    View* gview = activeView();
    if (gview) {
        gview->clearRubberRoom();
        gview->previewOff();
        gview->disableMoveRapidFire();
    }
    prompt->endCommand();
    return "";
}

/**
 * @brief quit_action
 * @param args
 * @return
 */
static String
quit_action(String args)
{
    _mainWin->quit();
    return "";
}

/**
 * @brief init_action
 * @param args
 * @return
 */
static String
init_action(String args)
{
    no_argument_debug("init_action()", args);
    View* gview = activeView();
    if (gview) {
        gview->clearRubberRoom();
    }
    return "";
}

/**
 * @brief platform_action
 * @param args
 * @return
 */
static String
platform_action(String args)
{
    no_argument_debug("init_action()", args);
    return platformString();
}

/**
 * @brief select_all_action
 * @param args
 * @return
 */
static String
select_all_action(String args)
{
    debug_message("selectAll()");
    View* gview = activeView();
    if (gview) {
        gview->selectAll();
    }
    return "";
}

/**
 * @brief append_history_action
 * @param args
 * @return
 */
static String
append_history_action(String args)
{
    prompt->appendHistory(QString::fromStdString(args));
    return "";
}

/**
 * @brief window_action
 * @param args
 * @return
 */
static String
window_action(String args)
{
    StringList list = tokenize(args, ' ');
    String command = list[0];
    if (command == "cascade") {
        mdiArea->cascade();
        return "";
    }
    if (command == "close") {
        _mainWin->onCloseWindow();
        return "";
    }
    if (command == "closeall") {
        mdiArea->closeAllSubWindows();
        return "";
    }
    if (command == "tile") {
        mdiArea->tile();
        return "";
    }
    if (command == "next") {
        mdiArea->activateNextSubWindow();
        return "";
    }
    if (command == "previous") {
        mdiArea->activatePreviousSubWindow();
        return "";
    }
    return "window argument not recognised.";
}

/**
 * @brief open_action
 * @param args
 * @return
 */
static String
open_action(String args)
{
    _mainWin->openFile();
    return "";
}

/**
 * @brief icon_action
 * @param command
 * @return
 */
static String
icon_action(String command)
{
    if (command == "16") {
        debug_message("icon16()");
        _mainWin->iconResize(16);
        return "";
    }
    if (command == "24") {
        debug_message("icon24()");
        _mainWin->iconResize(24);
        return "";
    }
    if (command == "32") {
        debug_message("icon32()");
        _mainWin->iconResize(32);
        return "";
    }
    if (command == "48") {
        debug_message("icon48()");
        _mainWin->iconResize(48);
        return "";
    }
    if (command == "64") {
        debug_message("icon64()");
        _mainWin->iconResize(64);
        return "";
    }
    if (command == "128") {
        debug_message("icon128()");
        _mainWin->iconResize(128);
        return "";
    }
    return "ERROR: this icon size is not supported.";
}

/*
static String
text_action(String command)
{
    if (list.size() < 1) {
        return "text requires an argument.";
    }
    command = list[0];
    if (command == "font") {
        return settings.text_font;
    }
    if (command == "size") {
        return std::to_string(settings.text_size);
    }
    if (command == "angle") {
        return std::to_string(settings.text_angle);
    }
    if (command == "bold") {
        return std::to_string(settings.text_style_bold);
    }
    if (command == "italic") {
        return std::to_string(settings.text_style_italic);
    }
    if (command == "underline") {
        return std::to_string(settings.text_style_underline);
    }
    if (command == "strikeout") {
        return std::to_string(settings.text_style_strikeout);
    }
    if (command == "overline") {
        return std::to_string(settings.text_style_overline);
    }
}

if (command == "set") {
    if (list.size() < 2) {
        return "The command 'set' requires 2 arguments.";
    }
    bool value = (
           list[1] == "true"
        || list[1] == "True"
        || list[1] == "TRUE"
        || list[1] == "on"
        || list[1] == "ON"
        || list[1] == "T"
        || list[1] == "t"
        || list[1] == "1"
    );
    if (list[0] == "text_font") {
        settings.text_font = list[1];
        return "";
    }
    if (list[0] == "text_size") {
        settings.text_size = std::stof(list[1]);
        return "";
    }
    if (command == "text_angle") {
        settings.text_angle = value;
        return "";
    }
    if (command == "text_style_bold") {
        settings.text_style_bold = value;
        return "";
    }
    if (command == "text_style_italic") {
        settings.text_style_italic = value;
        return "";
    }
    if (command == "text_style_underline") {
        settings.text_style_underline = value;
        return "";
    }
    if (command == "text_style_strikeout") {
        settings.text_style_strikeout = value;
        return "";
    }
    if (command == "text_style_overline") {
        settings.text_style_overline = value;
        return "";
    }
}

if (command == "enable") {
    if (list.size() < 1) {
        return "The command 'enable' requires an argument.";
    }
    if (command == "text-angle") {
        settings.text_angle = true;
        return "";
    }
    if (command == "text-bold") {
        settings.text_style_bold = true;
        return "";
    }
    if (command == "text-italic") {
        settings.text_style_italic = true;
        return "";
    }
    if (command == "text-underline") {
        settings.text_style_underline = true;
        return "";
    }
    if (command == "text-strikeout") {
        settings.text_style_strikeout = true;
        return "";
    }
    if (command == "text-overline") {
        settings.text_style_overline = true;
        return "";
    }
    if (command == "prompt-rapid-fire") {
        prompt->enableRapidFire();
        return "";
    }
    if (command == "move-rapid-fire") {
        View* gview = activeView();
        if (gview) {
            gview->enableMoveRapidFire();
        }
        return "";
    }
    return "";
}
*/

/**
 * @brief version_action
 * @param args
 * @return
 */
static String
version_action(String args)
{
    return config["version"].s;
}

/**
 * @brief set_prompt_prefix_action
 * @param args
 * @return
 */
static String
set_prompt_prefix_action(String args)
{
    prompt->setPrefix(QString::fromStdString(args));
    return "";
}

/*
static String
add_to_selection_action(String args)
{
    AddToSelection();
    return "";
}

if (command == "clear-selection") {
    View* gview = activeView();
    if (gview) {
        gview->clearSelection();
    }
    return "";
}

if (command == "delete selection") {
    nativeDeleteSelected();
    return "";
}

if (command == "debug") {
    scriptValDebug();
    return "";
}

if (command == "error") {
    scriptValError();
    return "";
}

if (command == "todo") {
    Todo();
    return "";
}

if (command == "appendPromptHistory") {
    AppendPromptHistory();
    return "";
}

if (command == "isInt") {
    IsInt();
    return "";
}

if (command == "previewOn") {
    preview_on_action();
    return "";
}

if (command == "previewOff")
    preview_off_action();
    return "";
}

if (command == "vulcanize") {
    View* gview = activeView();
    if (gview) {
        gview->vulcanizeRubberRoom();
    }
    return "";
}

if (command == "rubber") {
    if (command == "allow") {
        allow_rubber_action();
        return "";
    }

    if (command == "set-mode") {
        SetRubberMode();
        return "";
    }

    if (command == "set-point") {
        SetRubberPoint();
        return "";
    }
    if (command == "set-text") {
        SetRubberText();
        return "";
    }

    if (command == "add") {
        AddRubber();
        return "";
    }
    if (command == "clear") {
        clear_rubber_action();
        return "";
    }
    if (command == "spare") {
        spare_rubber_action();
        return "";
    }
    return "";
}

if (command == "setCursorShape") {
    SetCursorShape();
    return "";
}

if (command == "calculateAngle") {
    CalculateAngle();
    return "";
}

if (command == "calculateDistance") {
    CalculateDistance();
    return "";
}

if (command == "perpendicularDistance") {
    PerpendicularDistance();
    return "";
}

if (command == "addToSelection") {
    scriptValAddToSelection();
    return "";
}

if (command == "clearSelection") {
    scriptValClearSelection();
    return "";
}

if (command == "deleteSelected") {
    scriptValDeleteSelected();
    return "";
}

if (command == "cutSelected") {
    scriptValCutSelected);
    return "";
}

if (command == "copySelected") {
    scriptValCopySelected);
    return "";
}

if (command == "pasteSelected") {
    scriptValPasteSelected();
    return "";
}

if (command == "moveSelected") {
    scriptValMoveSelected();
    return "";
}

if (command == "scaleSelected") {
    scriptValScaleSelected();
    return "";
}

if (command == "rotateSelected") {
    scriptValRotateSelected();
    return "";
}

if (command == "mirrorSelected") {
    scriptValMirrorSelected();
    return "";
}

if (command == "include") {
    Include();
    return "";
}

if (command == "blink-prompt") {
    prompt->startBlinking();
    return "";
}

if (command == "SetPromptPrefix") {
    setPromptPrefix(args[0]);
    return "";
}
*/

/**
 * @brief Inspired by PyArg_ParseTupleAndKeywords allowing
 * a uniform argument parsing framework.
 * @param label The caller's name.
 * @param args The list of strings passed from the user.
 * @param args_template The string of characters describing the types of the output.
 * @param result The fixed length array of results.
 * @return An error message if an error occured or an empty string if it passes.
 */
String
convert_args_to_type(
    String label,
    std::vector<String> args,
    const char *args_template,
    NodeList a)
{
    int n_args = (int)args.size();
    int required_args = strlen(args_template);
    if (n_args < required_args) {
        String required = std::to_string(required_args);
        return "ERROR: " + label + "requires" + required + "arguments";
    }
    for (int i=0; i<n_args; i++) {
        switch (args_template[i]) {
        case 'i': {
            Node entry;
            entry.i = stoi(args[i]);
            a.push_back(entry);
            if (errno == EINVAL) {
                return "TYPE ERROR: failed to convert argument " + std::to_string(i) + " to int.";
            }
            if (errno == ERANGE) {
                return "RANGE ERROR: argument " + std::to_string(i) + " out of range.";
            }
            break;
        }
        case 'r': {
            Node entry;
            entry.r = stof(args[i]);
            a.push_back(entry);
            if (errno == EINVAL) {
                return "TYPE ERROR: failed to convert argument " + std::to_string(i) + " to floating point.";
            }
            if (std::isnan(a[i].r)) {
                return "NaN ERROR: argument " + std::to_string(i) + " is not a number.";
            }
            break;
        }
        case 's': {
            Node entry;
            entry.s = args[i];
            a.push_back(entry);
            break;
        }
        default:
            break;
        }
    }
    /* Empty string meaning "passes". */
    return "";
}

/**
 * @brief Include
 * @param a
 * @return
 */
String
include_action(NodeList a)
{
    return run_script_file("commands/" + a[0].s);
}

/**
 * @brief Error
 * @param a
 * @return
 */
String
error_action(String args)
{
    /*
    _mainWin->setPromptPrefix("ERROR: (" + a[0].s + ") " + a[1].s);
    _mainWin->nativeAppendPromptHistory(QString());
    actuator("end");
    */
    return "";
}

/**
 * @brief Todo
 * @param a
 * @return
 */
String
todo_action(String args)
{
    /*
    _mainWin->nativealert_action("TODO: (" + a[0].s + ") " + a[1].s);
    actuator("end");
    */
    return "";
}

/**
 * @brief AppendPromptHistory
 * @param a
 * @return
 */
String
append_prompt_history_action(String args)
{
    /*
    int args = args.size();
    if (args == 0) {
        _mainWin->nativeAppendPromptHistory(QString());
    }
    else if (args == 1) {
        _mainWin->nativeAppendPromptHistory(a[0].s);
    }
    else {
        return "ERROR: appendPromptHistory() requires one or zero arguments");
    }
    */
    return "";
}

/**
 * argument string "i"
 */
String
is_int_action(String args)
{
    NodeList result;
    StringList a = tokenize(args, ' ');
    String error = convert_args_to_type("IsInt()", a, "i", result);
    if (error != "") {
        return "false";
    }

    return "true";
}

/**
 * .
 * argument string "iii"
 */
static String
set_crosshair_color_action(String args)
{
    /*
    int r = args[0].r;
    int g = args[1].r;
    int b = args[2].r;

    if (!validRGB(r, g, b)) {
    }

    if (r < 0 || r > 255) {
        return "ERROR setCrossHairColor(): r value must be in range 0-255";
    }
    if (g < 0 || g > 255) {
        return "ERROR setCrossHairColor(): g value must be in range 0-255";
    }
    if (b < 0 || b > 255) {
        return "ERROR setCrossHairColor(): b value must be in range 0-255";
    }

    _mainWin->setCrossHairColor(r, g, b);
    */
    return "";
}

/**
 * .
 * argument string "iii"
 */
String
set_grid_color_action(String args)
{
    /*
    int r = a[0].r;
    int g = a[1].r;
    int b = a[2].r;

    if (r < 0 || r > 255) {
        return "ERROR set_grid_color_action(): r value must be in range 0-255";
    }
    if (g < 0 || g > 255) {
        return "ERROR set_grid_color_action(): g value must be in range 0-255";
    }
    if (b < 0 || b > 255) {
        return "ERROR set_grid_color_action(): b value must be in range 0-255";
    }

    _mainWin->set_grid_color_action(r, g, b);
    */
    return "";
}

String
SetTextAngle_action(String args)
{
    /*
    _mainWin->setTextAngle(a[0].r);
    */
    return "";
}

/**
 * @brief preview_on_action
 * @param args
 * @return
 */
String
preview_on_action(String args)
{
    /*
    QString cloneStr = QString::toStdString(a[0].s).toUpper();
    QString modeStr  = QString::toStdString(a[1].s).toUpper();
    EmbReal x = a[2].r;
    EmbReal y = a[3].r;
    EmbReal data = a[4].r;

    int clone = PREVIEW_CLONE_NULL;
    int mode = PREVIEW_MODE_NULL;
    if     (cloneStr == "SELECTED") { clone = PREVIEW_CLONE_SELECTED; }
    else if (cloneStr == "RUBBER") { clone = PREVIEW_CLONE_RUBBER;   }
    else { return context->throwError(QScriptContext::UnknownError, "preview_on_action(): first argument must be \"SELECTED\" or \"RUBBER\"."); }

    if     (modeStr == "MOVE") { mode = PREVIEW_MODE_MOVE;   }
    else if (modeStr == "ROTATE") { mode = PREVIEW_MODE_ROTATE; }
    else if (modeStr == "SCALE") { mode = PREVIEW_MODE_SCALE;  }
    else { return context->throwError(QScriptContext::UnknownError, "preview_on_action(): second argument must be \"MOVE\", \"ROTATE\" or \"SCALE\"."); }

    _mainWin->nativepreview_on_action(clone, mode, x, y, data);
    */
    return "";
}

/*
"preview off", nativepreview_off_action();
"allow rubber", nativeallow_rubber_action();
*/

/**
 * @brief set_rubber_text_action
 * @param args
 * @return
 */
String
set_rubber_text_action(String args)
{
    //QString key = QString::fromStdString(a[0].s).toUpper();
    //_mainWin->setRubberText(key, a[1].s);
    return "";
}

/**
 * @brief add_rubber_action
 * @param args
 * @return
 */
String
add_rubber_action(String args)
{
    //QString objType = QString::fromStdString(a[0].s).toUpper();

    /*
    if (!_mainWin->nativeallow_rubber_action())
        return context->throwError(QScriptContext::UnknownError, "addRubber(): You must use actuator("vulcanize") before you can add another rubber object.");

    EmbReal mx = _mainWin->nativemouse_x_action();
    EmbReal my = _mainWin->nativemouse_y_action();

    if (objType == "ARC") {
         //TODO: handle this type
    }
    else if (objType == "BLOCK") {

    } //TODO: handle this type
    else if (objType == "CIRCLE") {
        _mainWin->nativeAddCircle(mx, my, 0, false, OBJ_RUBBER_ON);
    }
    else if (objType == "DIMALIGNED") {

    } //TODO: handle this type
    else if (objType == "DIMANGULAR") {

    } //TODO: handle this type
    else if (objType == "DIMARCLENGTH") {

    } //TODO: handle this type
    else if (objType == "DIMDIAMETER") {

    } //TODO: handle this type
    else if (objType == "DIMLEADER") {
        _mainWin->nativeAddDimLeader(mx, my, mx, my, 0, OBJ_RUBBER_ON);
    }
    else if (objType == "DIMLINEAR") {

    } //TODO: handle this type
    else if (objType == "DIMORDINATE") {

    } //TODO: handle this type
    else if (objType == "DIMRADIUS") {

    } //TODO: handle this type
    else if (objType == "ELLIPSE") {
        _mainWin->nativeAddEllipse(mx, my, 0, 0, 0, 0, OBJ_RUBBER_ON);
    }
    else if (objType == "ELLIPSEARC") {

    } //TODO: handle this type
    else if (objType == "HATCH") {
    } //TODO: handle this type
    else if (objType == "IMAGE") {
    } //TODO: handle this type
    else if (objType == "INFINITELINE") {
    } //TODO: handle this type
    else if (objType == "LINE") {
        _mainWin->nativeadd_line_action(mx, my, mx, my, 0, OBJ_RUBBER_ON);
    }
    else if (objType == "PATH") {
    } //TODO: handle this type
    else if (objType == "POINT") {} //TODO: handle this type
    else if (objType == "POLYGON") { _mainWin->nativeadd_polygon_action(mx, my, QPainterPath(), OBJ_RUBBER_ON); }
    else if (objType == "POLYLINE") { _mainWin->nativeAddPolyline(mx, my, QPainterPath(), OBJ_RUBBER_ON); }
    else if (objType == "RAY") {} //TODO: handle this type
    else if (objType == "RECTANGLE") { _mainWin->nativeadd_rectangle_action(mx, my, mx, my, 0, 0, OBJ_RUBBER_ON); }
    else if (objType == "SPLINE") {} //TODO: handle this type
    else if (objType == "TEXTMULTI") {} //TODO: handle this type
    else if (objType == "TEXTSINGLE") { _mainWin->nativeadd_text_single_action("", mx, my, 0, false, OBJ_RUBBER_ON); }
    */
    return "";
}

/*
"clear rubber", nativeclear_rubber_action();
*/

/**
 * @brief add_slot_action
 * @param args
 * @return
 *
 * EmbReal centerX, EmbReal centerY, EmbReal diameter, EmbReal length, EmbReal rot, bool fill, String rubberMode
 */
static String
add_slot_action(String args)
{
    //TODO: Use UndoableAddCommand for slots
    /*
    SlotObject* slotObj = new SlotObject(centerX, -centerY, diameter, length,_mainWin->getCurrentColor());
    slotObj->setRotation(-rot);
    slotObj->setObjectRubberMode(rubberMode);
    if (rubberMode) gview->addToRubberRoom(slotObj);
    scene->addItem(slotObj);
    //TODO: slot fill
    scene->update();
    */
    //_mainWin->nativeAddSlot(a[0].r, a[1].r, a[2].r, a[3].r, a[4].r, a[5].b, OBJ_RUBBER_OFF);
    return "";
}

/**
 * @brief add_point_action
 * @param args
 * @return
 */
static String
add_point_action(String args)
{
    //_mainWin->nativeAddPoint(a[0].r, a[1].r);
    return "";
}

/**
 * @brief add_polygon_action
 * @param args
 * @return
 * NOTE: This native is different than the rest in that the Y+ is down (scripters need not worry about this)
 * EmbReal startX, EmbReal startY, const QPainterPath& p, String rubberMode
 */
static String
add_polygon_action(String args)
{
    /*
    View* gview = activeView();
    QGraphicsScene* gscene = gview->scene();
    QUndoStack* stack = gview->getUndoStack();
    if (gview && gscene && stack) {
        PolygonObject* obj = new PolygonObject(startX, startY, p,_mainWin->getCurrentColor());
        obj->setObjectRubberMode(rubberMode);
        if (rubberMode != "OBJ_RUBBER_OFF") {
            gview->addToRubberRoom(obj);
            gscene->addItem(obj);
            gscene->update();
        }
        else {
            UndoableAddCommand* cmd = new UndoableAddCommand(obj->data(OBJ_NAME).toString(), obj, gview, 0);
            stack->push(cmd);
        }
    }

    QVariantList varList = a[0].toVariant().toList();
    int varSize = varList.size();
    if (varSize < 2) {
        return "TYPE ERROR: add_polygon_action(): array must contain at least two elements";
    }
    if (varSize % 2) {
        return "TYPE ERROR: add_polygon_action(): array cannot contain an odd number of elements";
    }

    bool lineTo = false;
    bool xCoord = true;
    EmbReal x = 0;
    EmbReal y = 0;
    EmbReal startX = 0;
    EmbReal startY = 0;
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
                else { path.moveTo(x,y); lineTo = true; startX = x; startY = y; }
            }
        }
        else {
            return "TYPE ERROR: add_polygon_action(): array contains one or more invalid elements");
        }
    }

    //Close the polygon
    path.closeSubpath();

    path.translate(-startX, -startY);

    _mainWin->nativeadd_polygon_action(startX, startY, path, OBJ_RUBBER_OFF);
    */
    return "";
}

/**
 * @brief MainWindow::recentMenuAboutToShow
 */
void
MainWindow::recentMenuAboutToShow()
{
    debug_message("MainWindow::recentMenuAboutToShow()");
    subMenuHash["recent"]->clear();

    QFileInfo recentFileInfo;
    QString recentValue;
    for (int i = 0; i < settings.opensave_recent_list_of_files.size(); ++i) {
        //If less than the max amount of entries add to menu
        if (i < settings.opensave_recent_max_files) {
            recentFileInfo = QFileInfo(settings.opensave_recent_list_of_files.at(i));
            if (recentFileInfo.exists() && validFileFormat(recentFileInfo.fileName())) {
                recentValue.setNum(i+1);
                QAction* rAction;
                if     (recentValue.toInt() >= 1 && recentValue.toInt() <= 9)
                    rAction = new QAction("&" + recentValue + " " + recentFileInfo.fileName(), this);
                else if (recentValue.toInt() == 10)
                    rAction = new QAction("1&0 " + recentFileInfo.fileName(), this);
                else
                    rAction = new QAction(recentValue + " " + recentFileInfo.fileName(), this);
                rAction->setCheckable(false);
                rAction->setData(settings.opensave_recent_list_of_files.at(i));
                subMenuHash["recent"]->addAction(rAction);
                connect(rAction, SIGNAL(triggered()), this, SLOT(openrecentfile()));
            }
        }
    }
    //Ensure the list only has max amount of entries
    while (settings.opensave_recent_list_of_files.size() > settings.opensave_recent_max_files) {
        settings.opensave_recent_list_of_files.removeLast();
    }
}

/**
 * @brief MainWindow::windowMenuAboutToShow
 */
void
MainWindow::windowMenuAboutToShow()
{
    debug_message("MainWindow::windowMenuAboutToShow()");
    menuHash["window"]->clear();
    menuHash["window"]->addAction(actionHash["windowclose"]);
    menuHash["window"]->addAction(actionHash["windowcloseall"]);
    menuHash["window"]->addSeparator();
    menuHash["window"]->addAction(actionHash["windowcascade"]);
    menuHash["window"]->addAction(actionHash["windowtile"]);
    menuHash["window"]->addSeparator();
    menuHash["window"]->addAction(actionHash["windownext"]);
    menuHash["window"]->addAction(actionHash["windowprevious"]);

    menuHash["window"]->addSeparator();
    QList<QMdiSubWindow*> windows = mdiArea->subWindowList();
    for (int i = 0; i < windows.count(); ++i) {
        QAction* aAction = new QAction(windows.at(i)->windowTitle(), this);
        aAction->setCheckable(true);
        aAction->setData(i);
        menuHash["window"]->addAction(aAction);
        connect(aAction, SIGNAL(toggled(bool)), this, SLOT(windowMenuActivated(bool)));
        aAction->setChecked(mdiArea->activeSubWindow() == windows.at(i));
    }
}

/**
 * @brief MainWindow::windowMenuActivated
 * @param checked
 */
void
MainWindow::windowMenuActivated(bool checked)
{
    debug_message("MainWindow::windowMenuActivated()");
    QAction* aSender = qobject_cast<QAction*>(sender());
    if (!aSender)
        return;
    QWidget* w = mdiArea->subWindowList().at(aSender->data().toInt());
    if (w && checked)
        w->setFocus();
}

/**
 * @brief new_action
 * @param args
 * @return
 */
static String
new_action(String args)
{
    _mainWin->newFile();
    return "";
}

/**
 * @brief MainWindow::newFile
 */
void
MainWindow::newFile()
{
    debug_message("MainWindow::newFile()");
    docIndex++;
    numOfDocs++;
    MdiWindow* mdiWin = new MdiWindow(docIndex, mdiArea, Qt::SubWindow);
    connect(mdiWin, SIGNAL(sendCloseMdiWin(MdiWindow*)), this, SLOT(onCloseMdiWin(MdiWindow*)));
    connect(mdiArea, SIGNAL(subWindowActivated(QMdiSubWindow*)), this, SLOT(onWindowActivated(QMdiSubWindow*)));

    updateMenuToolbarStatusbar();
    windowMenuAboutToShow();

    if (mdiWin->gview) {
        mdiWin->gview->recalculateLimits();
        mdiWin->gview->zoomExtents();
    }
}

/**
 * @brief MainWindow::openFile
 * @param recent
 * @param recentFile
 */
void
MainWindow::openFile(bool recent, QString  recentFile)
{
    debug_message("MainWindow::openFile()");

    QApplication::setOverrideCursor(Qt::ArrowCursor);

    QStringList files;
    bool preview = settings.opensave_open_thumbnail;
    openFilesPath = QString::fromStdString(settings.opensave_recent_directory);

    //Check to see if this from the recent files list
    if (recent) {
        files.append(recentFile);
        openFilesSelected(files);
    }
    else {
        if (!preview) {
            //TODO: set getOpenFileNames' selectedFilter Node from settings.opensave_open_format
            files = QFileDialog::getOpenFileNames(this, tr("Open"), openFilesPath, formatFilterOpen);
            openFilesSelected(files);
        }
        else {
            PreviewDialog* openDialog = new PreviewDialog(this, tr("Open w/Preview"), openFilesPath, formatFilterOpen);
            //TODO: set openDialog->selectNameFilter(QString  filter) from settings.opensave_open_format
            connect(openDialog, SIGNAL(filesSelected(QStringList)), this, SLOT(openFilesSelected(QStringList)));
            openDialog->exec();
        }
    }

    QApplication::restoreOverrideCursor();
}

/**
 * @brief MainWindow::openFilesSelected
 * @param filesToOpen
 */
void
MainWindow::openFilesSelected(const QStringList& filesToOpen)
{
    bool doOnce = true;

    if (filesToOpen.count()) {
        for (int i = 0; i < filesToOpen.count(); i++) {
            if (!validFileFormat(filesToOpen[i]))
                continue;

            QMdiSubWindow* existing = findMdiWindow(filesToOpen[i]);
            if (existing) {
                mdiArea->setActiveSubWindow(existing);
                continue;
            }

            //The docIndex doesn't need increased as it is only used for unnamed files
            numOfDocs++;
            MdiWindow* mdiWin = new MdiWindow(docIndex, mdiArea, Qt::SubWindow);
            connect(mdiWin, SIGNAL(sendCloseMdiWin(MdiWindow*)), this, SLOT(onCloseMdiWin(MdiWindow*)));
            connect(mdiArea, SIGNAL(subWindowActivated(QMdiSubWindow*)), this, SLOT(onWindowActivated(QMdiSubWindow*)));

            //Make sure the toolbars/etc... are shown before doing their zoomExtents
            if (doOnce) { updateMenuToolbarStatusbar(); doOnce = false; }

            if (mdiWin->loadFile(filesToOpen.at(i))) {
                statusbar->showMessage(tr("File(s) loaded"), 2000);
                mdiWin->show();
                mdiWin->showMaximized();
                //Prevent duplicate entries in the recent files list
                if (!settings.opensave_recent_list_of_files.contains(filesToOpen.at(i), Qt::CaseInsensitive)) {
                    settings.opensave_recent_list_of_files.prepend(filesToOpen.at(i));
                }
                //Move the recent file to the top of the list
                else {
                    settings.opensave_recent_list_of_files.removeAll(filesToOpen.at(i));
                    settings.opensave_recent_list_of_files.prepend(filesToOpen.at(i));
                }
                settings.opensave_recent_directory = QFileInfo(filesToOpen.at(i)).absolutePath().toStdString();

                if (mdiWin->gview) {
                    mdiWin->gview->recalculateLimits();
                    mdiWin->gview->zoomExtents();
                }
            }
            else {
                mdiWin->close();
            }
        }
    }

    windowMenuAboutToShow();
}

/**
 * @brief MainWindow::openrecentfile
 */
void
MainWindow::openrecentfile()
{
    debug_message("MainWindow::openrecentfile()");

    //Check to see if this from the recent files list
    QAction* recentSender = qobject_cast<QAction*>(sender());
    if (recentSender) {
        openFile(true, recentSender->data().toString());
    }
}

/**
 * @brief MainWindow::savefile
 */
void
MainWindow::savefile()
{
    debug_message("MainWindow::savefile()");
}

/**
 * @brief MainWindow::saveasfile
 */
void
MainWindow::saveasfile()
{
    debug_message("MainWindow::saveasfile()");
    // need to find the activeSubWindow before it loses focus to the FileDialog
    MdiWindow* mdiWin = qobject_cast<MdiWindow*>(mdiArea->activeSubWindow());
    if (!mdiWin) {
        return;
    }

    QString file;
    openFilesPath = QString::fromStdString(settings.opensave_recent_directory);
    file = QFileDialog::getSaveFileName(this, tr("Save As"), openFilesPath, formatFilterSave);

    mdiWin->saveFile(file);
}

/**
 * @brief MainWindow::findMdiWindow
 * @param fileName
 * @return
 */
QMdiSubWindow *
MainWindow::findMdiWindow(QString fileName)
{
    debug_message("MainWindow::findMdiWindow(%s)" + fileName.toStdString());
    QString canonicalFilePath = QFileInfo(fileName).canonicalFilePath();

    foreach(QMdiSubWindow* subWindow, mdiArea->subWindowList()) {
        MdiWindow* mdiWin = qobject_cast<MdiWindow*>(subWindow);
        if (mdiWin) {
            if (mdiWin->curFile == canonicalFilePath) {
                return subWindow;
            }
        }
    }
    return 0;
}

/**
 * @brief MainWindow::closeEvent
 * @param event
 */
void
MainWindow::closeEvent(QCloseEvent* event)
{
    mdiArea->closeAllSubWindows();
    write_settings();
    event->accept();
}

/**
 * @brief MainWindow::onCloseWindow
 */
void
MainWindow::onCloseWindow()
{
    debug_message("MainWindow::onCloseWindow()");
    MdiWindow* mdiWin = qobject_cast<MdiWindow*>(mdiArea->activeSubWindow());
    if (mdiWin) {
        onCloseMdiWin(mdiWin);
    }
}

/**
 * @brief MainWindow::onCloseMdiWin
 * @param theMdiWin
 */
void
MainWindow::onCloseMdiWin(MdiWindow* theMdiWin)
{
    debug_message("MainWindow::onCloseMdiWin()");
    numOfDocs--;

    bool keepMaximized;
    if (theMdiWin) { keepMaximized = theMdiWin->isMaximized(); }

    mdiArea->removeSubWindow(theMdiWin);
    theMdiWin->deleteLater();

    updateMenuToolbarStatusbar();
    windowMenuAboutToShow();

    if (keepMaximized) {
        MdiWindow* mdiWin = qobject_cast<MdiWindow*>(mdiArea->activeSubWindow());
        if (mdiWin) {
            mdiWin->showMaximized();
        }
    }
}

/**
 * @brief MainWindow::onWindowActivated
 * @param w
 */
void
MainWindow::onWindowActivated(QMdiSubWindow* w)
{
    debug_message("MainWindow::onWindowActivated()");
    MdiWindow* mdiWin = qobject_cast<MdiWindow*>(w);
    if (mdiWin) {
        mdiWin->onWindowActivated();
    }
}

/**
 * @brief MainWindow::resizeEvent
 * @param e
 */
void
MainWindow::resizeEvent(QResizeEvent* e)
{
    debug_message("MainWindow::resizeEvent()");
    QMainWindow::resizeEvent(e);
    statusBar()->setSizeGripEnabled(!isMaximized());
}

/**
 * @brief MainWindow::getFileSeparator
 * @return
 */
QAction*
MainWindow::getFileSeparator()
{
    debug_message("MainWindow::getFileSeparator()");
    return myFileSeparator;
}

/**
 * @brief MainWindow::updateMenuToolbarStatusbar
 */
void
MainWindow::updateMenuToolbarStatusbar()
{
    debug_message("MainWindow::updateMenuToolbarStatusbar()");

    actionHash["print"]->setEnabled(numOfDocs > 0);
    actionHash["windowclose"]->setEnabled(numOfDocs > 0);
    actionHash["designdetails"]->setEnabled(numOfDocs > 0);

    if (numOfDocs) {
        for (auto iter=toolbarHash.begin(); iter != toolbarHash.end(); iter++) {
            iter->second->show();
        }

        //DockWidgets
        dockPropEdit->show();
        dockUndoEdit->show();

        //Menus
        menuBar()->clear();
        StringList menubar_order = config["menubar_order"].sl;
        for (int i=0; i<(int)menubar_order.size(); i++) {
            menuBar()->addMenu(menuHash[menubar_order[i]]);
        }

        menuHash["window"]->setEnabled(true);

        //Statusbar
        statusbar->clearMessage();
        statusbar->statusBarMouseCoord->show();
        statusbar->statusBarSnapButton->show();
        statusbar->statusBarGridButton->show();
        statusbar->statusBarRulerButton->show();
        statusbar->statusBarOrthoButton->show();
        statusbar->statusBarPolarButton->show();
        statusbar->statusBarQSnapButton->show();
        statusbar->statusBarQTrackButton->show();
        statusbar->statusBarLwtButton->show();
    }
    else {
        for (auto iter=toolbarHash.begin(); iter != toolbarHash.end(); iter++) {
            iter->second->hide();
        }

        toolbarHash["file"]->show();
        toolbarHash["edit"]->show();

        //DockWidgets
        dockPropEdit->hide();
        dockUndoEdit->hide();

        //Menus
        menuBar()->clear();
        menuBar()->addMenu(menuHash["file"]);
        menuBar()->addMenu(menuHash["edit"]);
        menuBar()->addMenu(menuHash["settings"]);
        menuBar()->addMenu(menuHash["window"]);
        menuBar()->addMenu(menuHash["help"]);

        menuHash["window"]->setEnabled(false);

        //Statusbar
        statusbar->clearMessage();
        statusbar->statusBarMouseCoord->hide();
        statusbar->statusBarSnapButton->hide();
        statusbar->statusBarGridButton->hide();
        statusbar->statusBarRulerButton->hide();
        statusbar->statusBarOrthoButton->hide();
        statusbar->statusBarPolarButton->hide();
        statusbar->statusBarQSnapButton->hide();
        statusbar->statusBarQTrackButton->hide();
        statusbar->statusBarLwtButton->hide();
    }
    hideUnimplemented();
}

/**
 * @brief MainWindow::hideUnimplemented
 */
void
MainWindow::hideUnimplemented()
{
    debug_message("MainWindow::hideUnimplemented()");
}

/**
 * @brief MainWindow::validFileFormat
 * @param fileName
 * @return
 *
 * \todo check the file exists on the system, rename to validFile?
 */
bool
MainWindow::validFileFormat(QString fileName)
{
    if (fileName == "") {
        return false;
    }
    if (emb_identify_format(qPrintable(fileName)) >= 0) {
        return true;
    }
    return false;
}

/**
 * @brief MainWindow::loadFormats
 */
void
MainWindow::loadFormats()
{
    char stable, unstable;
    QString supportedReaders  = "All Supported Files (";
    QString individualReaders = "All Files (*);;";
    QString supportedWriters  = "All Supported Files (";
    QString individualWriters = "All Files (*);;";
    QString supportedStr;
    QString individualStr;

    //TODO: Stable Only (Settings Option)
    //stable = 'S'; unstable = 'S';

    //Stable + Unstable
    stable = 'S'; unstable = 'U';

    const char* extension = 0;
    const char* description = 0;
    char readerState;
    char writerState;

    for (int i=0; i<numberOfFormats; i++) {
        extension = formatTable[i].extension;
        description = formatTable[i].description;
        readerState = formatTable[i].reader_state;
        writerState = formatTable[i].writer_state;

        QString upperExt = QString(extension).toUpper();
        supportedStr = "*" + upperExt + " ";
        individualStr = upperExt.replace(".", "") + " - " + description + " (*" + extension + ");;";
        if (readerState == stable || readerState == unstable) {
            //Exclude color file formats from open dialogs
            if (upperExt != "COL" && upperExt != "EDR" && upperExt != "INF" && upperExt != "RGB") {
                supportedReaders.append(supportedStr);
                individualReaders.append(individualStr);
            }
        }
        if (writerState == stable || writerState == unstable) {
            supportedWriters.append(supportedStr);
            individualWriters.append(individualStr);
        }
    }

    supportedReaders.append(");;");
    supportedWriters.append(");;");

    formatFilterOpen = supportedReaders + individualReaders;
    formatFilterSave = supportedWriters + individualWriters;

    //TODO: Fixup custom filter
    /*
    QString custom = settings.custom_filter;
    if (custom.contains("supported", Qt::CaseInsensitive))
        custom = ""; //This will hide it
    else if (!custom.contains("*", Qt::CaseInsensitive))
        custom = ""; //This will hide it
    else
        custom = "Custom Filter(" + custom + ");;";

    return tr(qPrintable(custom + supported + all));
    */
}

/**
 * @brief MainWindow::closeToolBar
 * @param action
 */
void
MainWindow::closeToolBar(QAction* action)
{
    if (action->objectName() == "toolbarclose") {
        QToolBar* tb = qobject_cast<QToolBar*>(sender());
        if (tb) {
            debug_message(tb->objectName().toStdString() + "%s closed.");
            tb->hide();
        }
    }
}

/**
 * @brief MainWindow::floatingChangedToolBar
 * @param isFloating
 */
void
MainWindow::floatingChangedToolBar(bool isFloating)
{
    QToolBar* tb = qobject_cast<QToolBar*>(sender());
    if (tb) {
        if (isFloating) {
            /*
            //TODO: Determine best suited close button on various platforms.
            QStyle::SP_DockWidgetCloseButton
            QStyle::SP_TitleBarCloseButton
            QStyle::SP_DialogCloseButton
            */
            QAction *ACTION = new QAction(tb->style()->standardIcon(QStyle::SP_DialogCloseButton), "Close", this);
            ACTION->setStatusTip("Close the " + tb->windowTitle() + " Toolbar");
            ACTION->setObjectName("toolbarclose");
            tb->addAction(ACTION);
            connect(tb, SIGNAL(actionTriggered(QAction*)), this, SLOT(closeToolBar(QAction*)));
        }
        else {
            QList<QAction*> actList = tb->actions();
            for (int i = 0; i < actList.size(); ++i) {
                QAction* ACTION = actList.value(i);
                if (ACTION->objectName() == "toolbarclose") {
                    tb->removeAction(ACTION);
                    disconnect(tb, SIGNAL(actionTriggered(QAction*)), this, SLOT(closeToolBar(QAction*)));
                    delete ACTION;
                }
            }
        }
    }
}
