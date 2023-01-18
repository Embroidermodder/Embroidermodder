/*
 *  Embroidermodder 2.
 *
 *  ------------------------------------------------------------
 *
 *  Copyright 2013-2022 The Embroidermodder Team
 *  Embroidermodder 2 is Open Source Software.
 *  See LICENSE for licensing terms.
 *
 *  ------------------------------------------------------------
 *
 *  Use Python's PEP7 style guide.
 *      https://peps.python.org/pep-0007/
 */

#include "embroidermodder.h"

void loadConfiguration(void);

const char* _appName_ = "Embroidermodder";
const char* _appVer_  = "v2.0 alpha";
bool exitApp = false;

/* TTF_Font *interface_font; */
std::vector<EmbWindow> window_list;
std::vector<EmbWidget> widget_list;
std::vector<EmbPattern> pattern_list;
int generated_widgets = 0;
std::vector<int> selected;
int testing = 0;
int debug_mode = 0;
int language = 0;
std::vector<std::vector<std::vector<std::string>>> translation_tables;
EmbColor palette[256];
std::string assets_directory = ".";
std::string os_seperator = "/";

int toolbar_file_entries[] = {
    ACTION_new,
    ACTION_open,
    ACTION_save,
    ACTION_saveas,
    ACTION_print,
    ACTION_designdetails,
    -1,
    ACTION_undo,
    ACTION_redo,
    -1,
    ACTION_help,
    -2
};

int toolbar_edit_entries[] = {
    ACTION_cut,
    ACTION_copy,
    ACTION_paste,
    -2
};

int toolbar_view_entries[] = {
    ACTION_day,
    ACTION_night,
    -2
};

int file_menu_data[] = {
    -1,
    ACTION_save,
    ACTION_saveas,
    -1,
    ACTION_print,
    -1,
    ACTION_windowclose,
    -1,
    ACTION_designdetails,
    -1,
    ACTION_exit,
    -2
};

int edit_menu_data[] = {
    ACTION_undo,
    ACTION_redo,
    -1,
    ACTION_cut,
    ACTION_copy,
    ACTION_paste,
    -1,
    -2
};

int view_menu_data[] = {
    -1,
    ACTION_day,
    ACTION_night,
    -1,
    -2
};

int settings_menu_data[] = {
    ACTION_settingsdialog,
    -1,
    -2
};

int zoom_menu_data[] = {
    ACTION_zoomrealtime,
    ACTION_zoomprevious,
    -1,
    ACTION_zoomwindow,
    ACTION_zoomdynamic,
    ACTION_zoomscale,
    ACTION_zoomcenter,
    -1,
    ACTION_zoomin,
    ACTION_zoomout,
    -1,
    ACTION_zoomselected,
    ACTION_zoomall,
    ACTION_zoomextents,
    -2
};

int pan_menu_data[] = {
    ACTION_panrealtime,
    ACTION_panpoint,
    -1,
    ACTION_panleft,
    ACTION_panright,
    ACTION_panup,
    ACTION_pandown,
    -2
};

int help_menu_data[] = {
    ACTION_help,
    -1,
    ACTION_changelog,
    -1,
    ACTION_tipoftheday,
    -1,
    ACTION_about,
    -1,
    ACTION_whatsthis,
    -2
};

Action action_list[] = {
    {ACTION_donothing, "donothing", "&Do Nothing", "Does Nothing"},
    {ACTION_windowcascade, "windowcascade", "&Cascade", "Cascade the windows."},
    {ACTION_windowtile, "windowtile", "&Tile", "Tile the windows."},
    {ACTION_windowclose, "windowclose", "Cl&ose", "Close the active window."},
    {ACTION_windowcloseall, "windowcloseall", "Close &All", "Close all the windows."},
    {ACTION_windownext, "windownext", "Ne&xt", "Move the focus to the next window."},
    {ACTION_windowprevious, "windowprevious", "Pre&vious", "Move the focus to the previous window."},

    {ACTION_new, "new", "&New", "Create a new file."},
    {ACTION_open, "open", "&Open", "Open an existing file."},
    {ACTION_save, "save", "&Save", "Save the design to disk."},
    {ACTION_saveas, "saveas", "Save &As", "Save the design under a new name."},
    {ACTION_print, "print", "&Print", "Print the design."},
    {ACTION_designdetails, "designdetails", "&Details", "Details of the current design."},
    {ACTION_exit, "exit", "E&xit", "Exit the application."},

    {ACTION_cut, "cut", "Cu&t", "Cut the current selection's contents to the clipboard."},
    {ACTION_copy, "copy", "&Copy", "Copy the current selection's contents to the clipboard."},
    {ACTION_paste, "paste", "&Paste", "Paste the clipboard's contents into the current selection."},

    {ACTION_help, "help", "&Help", "Displays help."},
    {ACTION_changelog, "changelog", "&Changelog", "Describes new features in this product."},
    {ACTION_tipoftheday, "tipoftheday", "&Tip Of The Day", "Displays a dialog with useful tips"},
    {ACTION_about, "about", "&About Embroidermodder " VERSION, "Displays information about this product."},
    {ACTION_whatsthis, "whatsthis", "&What's This?", "What's This? Context Help!"},

    {ACTION_undo, "undo", "&Undo", "Reverses the most recent action."},
    {ACTION_redo, "redo", "&Redo", "Reverses the effects of the previous undo action."},

    {ACTION_icon16, "icon16", "Icon&16", "Sets the toolbar icon size to 16x16."},
    {ACTION_icon24, "icon24", "Icon&24", "Sets the toolbar icon size to 24x24."},
    {ACTION_icon32, "icon32", "Icon&32", "Sets the toolbar icon size to 32x32."},
    {ACTION_icon48, "icon48", "Icon&48", "Sets the toolbar icon size to 48x48."},
    {ACTION_icon64, "icon64", "Icon&64", "Sets the toolbar icon size to 64x64."},
    {ACTION_icon128, "icon128", "Icon12&8", "Sets the toolbar icon size to 128x128."},

    {ACTION_settingsdialog, "settingsdialog", "&Settings", "Configure settings specific to this product."},

    {ACTION_makelayercurrent, "makelayercurrent", "&Make Layer Active", "Makes the layer of a selected object the active layer"},
    {ACTION_layers, "layers", "&Layers", "Manages layers and layer properties:  LAYER"},
    {ACTION_layerselector, "layerselector", "&Layer Selector", "Dropdown selector for changing the current layer"},
    {ACTION_layerprevious, "layerprevious", "&Layer Previous", "Restores the previous layer settings:  LAYERP"},
    {ACTION_colorselector, "colorselector", "&Color Selector", "Dropdown selector for changing the current thread color"},
    {ACTION_linetypeselector, "linetypeselector", "&Stitchtype Selector", "Dropdown selector for changing the current stitch type"},
    {ACTION_lineweightselector, "lineweightselector", "&Threadweight Selector", "Dropdown selector for changing the current thread weight"},
    {ACTION_hidealllayers, "hidealllayers", "&Hide All Layers", "Turns the visibility off for all layers in the current drawing:  HIDEALL"},
    {ACTION_showalllayers, "showalllayers", "&Show All Layers", "Turns the visibility on for all layers in the current drawing:  SHOWALL"},
    {ACTION_freezealllayers, "freezealllayers", "&Freeze All Layers", "Freezes all layers in the current drawing:  FREEZEALL"},
    {ACTION_thawalllayers, "thawalllayers", "&Thaw All Layers", "Thaws all layers in the current drawing:  THAWALL"},
    {ACTION_lockalllayers, "lockalllayers", "&Lock All Layers", "Locks all layers in the current drawing:  LOCKALL"},
    {ACTION_unlockalllayers, "unlockalllayers", "&Unlock All Layers", "Unlocks all layers in the current drawing:  UNLOCKALL"},

    {ACTION_textbold, "textbold", "&Bold Text", "Sets text to be bold."},
    {ACTION_textitalic, "textitalic", "&Italic Text", "Sets text to be italic."},
    {ACTION_textunderline, "textunderline", "&Underline Text", "Sets text to be underlined."},
    {ACTION_textstrikeout, "textstrikeout", "&StrikeOut Text", "Sets text to be striked out."},
    {ACTION_textoverline, "textoverline", "&Overline Text", "Sets text to be overlined."},

    {ACTION_zoomrealtime, "zoomrealtime", "Zoom &Realtime", "Zooms to increase or decrease the apparent size of objects in the current viewport."},
    {ACTION_zoomprevious, "zoomprevious", "Zoom &Previous", "Zooms to display the previous view."},
    {ACTION_zoomwindow, "zoomwindow", "Zoom &Window", "Zooms to display an area specified by a rectangular window."},
    {ACTION_zoomdynamic, "zoomdynamic", "Zoom &Dynamic", "Zooms to display the generated portion of the drawing."},
    {ACTION_zoomscale, "zoomscale", "Zoom &Scale", "Zooms the display using a specified scale factor."},
    {ACTION_zoomcenter, "zoomcenter", "Zoom &Center", "Zooms to display a view specified by a center point and magnification or height."},
    {ACTION_zoomin, "zoomin", "Zoom &In", "Zooms to increase the apparent size of objects."},
    {ACTION_zoomout, "zoomout", "Zoom &Out", "Zooms to decrease the apparent size of objects."},
    {ACTION_zoomselected, "zoomselected", "Zoom Selec&ted", "Zooms to display the selected objects."},
    {ACTION_zoomall, "zoomall", "Zoom &All", "Zooms to display the drawing extents or the grid limits."},
    {ACTION_zoomextents, "zoomextents", "Zoom &Extents", "Zooms to display the drawing extents."},

    {ACTION_panrealtime, "panrealtime", "&Pan Realtime", "Moves the view in the current viewport."},
    {ACTION_panpoint, "panpoint", "&Pan Point", "Moves the view by the specified distance."},
    {ACTION_panleft, "panleft", "&Pan Left", "Moves the view to the left."},
    {ACTION_panright, "panright", "&Pan Right", "Moves the view to the right."},
    {ACTION_panup, "panup", "&Pan Up", "Moves the view up."},
    {ACTION_pandown, "pandown", "&Pan Down", "Moves the view down."},

    {ACTION_day, "day", "&Day", "Updates the current view using day vision settings."},
    {ACTION_night, "night", "&Night", "Updates the current view using night vision settings."},
    {-1, "END", "END", "END"}
};

std::vector<LineEdit> geometry_circle_line_edits = {
    {
        .label = "Center X",
        .icon = "blank",
        .type = "double",
        .signal = "lineEditCircleCenterX",
        .user_editable = false
    },
    {
        .label = "Center Y",
        .icon = "blank",
        .type = "double",
        .signal = "lineEditCircleCenterY",
        .user_editable = false
    },
    {
        .label = "Radius",
        .icon = "blank",
        .type = "double",
        .signal = "lineEditCircleRadius",
        .user_editable = false
    },
    {
        .label = "Diameter",
        .icon = "blank",
        .type = "double",
        .signal = "lineEditCircleDiameter",
        .user_editable = false
    },
    {
        .label = "Area",
        .icon = "blank",
        .type = "double",
        .signal = "lineEditCircleArea",
        .user_editable = false
    },
    {
        .label = "Circumference",
        .icon = "blank",
        .type = "double",
        .signal = "lineEditCircleCircumference",
        .user_editable = false
    }
};

GroupBox geometry_circle = {
    .title = "Geometry",
    .line_edits = geometry_circle_line_edits,
    .obj_type = OBJ_TYPE_CIRCLE
};

int
click_detection(EmbWidget *w, int x, int y)
{
    return (w->rect.x < x)
        && (x < w->rect.x + w->rect.w)
        && (w->rect.y < y)
        && (y < w->rect.y + w->rect.h);
}

Rect
make_rectangle(int x, int y, int w, int h)
{
    Rect rect;
    rect.x = x;
    rect.y = y;
    rect.w = w;
    rect.h = h;
    return rect;
}

EmbWidget *
get_widget_by_key(char *key)
{
    for (std::vector<EmbWidget>::size_type i=0; i<widget_list.size(); i++) {
        if (widget_list[i].key == key) {
            return &(widget_list[i]);
        }
    }
    return NULL;
}

int
get_widget_by_label(char *label)
{
    for (std::vector<EmbWidget>::size_type i=0; i<widget_list.size(); i++) {
        if (widget_list[i].label == label) {
            return i;
        }
    }
    return -1;
}

void
set_visibility(char *label, int visibility)
{
    int index = get_widget_by_label(label);
    if (index >= 0) {
        widget_list[index].visibility = visibility;
    }
}

int
parse_command(int argc, char *argv[], std::vector<std::string> files)
{
    for (int i=1; i<argc; i++) {
        std::string s(argv[i]);
        if ((s == "--local-boot") || (s == "-L")) {
            assets_directory = argv[i+1];
            std::cout << "Booting from \"" << assets_directory << "\"." << std::endl;
            i++;
            continue;
        }
        if ((s == "--debug") || (s == "-d")) {
            debug_mode = 1;
            printf("DEBUG MODE\n");
            continue;
        }
        if ((s == "--help") || (s == "-h")) {
            /* Store internally so we don't need to load
             * the global state before parsing the command.
            print_string_list(help_message); */
            return 0;
        }
        if ((s == "--version") || (s == "-v")) {
            /* For scripts that need the version string */
            std::cout << VERSION;
            return 0;
        }
        if (s == "--test") {
            testing = 1;
            continue;
        }
        files.push_back(s);
    }
    return 1;
}

double
random_uniform(void)
{
    int r = rand() % 100000;
    return 0.00001*r;
}

int
roundToMultiple(int roundUp, int numToRound, int multiple)
{
    if (multiple == 0) {
        return numToRound;
    }
    int remainder = numToRound % multiple;
    if (remainder == 0) {
        return numToRound;
    }

    if (numToRound < 0 && roundUp) {
        return numToRound - remainder;
    }
    if (roundUp) {
        return numToRound + multiple - remainder;
    }
    //else round down
    if (numToRound < 0 && !roundUp) {
        return numToRound - multiple - remainder;
    }
    return numToRound - remainder;
}

uint8_t
willUnderflowInt32(int64_t a, int64_t b)
{
    assert(LLONG_MAX>INT_MAX);
    int64_t c = (int64_t)a-b;
    return c < INT_MIN || c > INT_MAX;
}

uint8_t
willOverflowInt32(int64_t a, int64_t b)
{
    assert(LLONG_MAX>INT_MAX);
    int64_t c = (int64_t)a+b;
    return c < INT_MIN || c > INT_MAX;
}

/* Whenever the code happens across a todo call,
 * write it in a log file. */
void
todo(char *msg, int action)
{
    if (debug_mode) {
        FILE *f;
        f = fopen("todo.txt", "w");
        fseek(f, 0, SEEK_END);
        fprintf(f, "%s: %d\n", msg, action);
        fclose(f);
    }
}

/* Whenever the code happens across a todo call,
 * write it in a log file. */
void
error(char *msg, int action)
{
    if (debug_mode) {
        FILE *f;
        f = fopen("error.txt", "w");
        fseek(f, 0, SEEK_END);
        fprintf(f, "%s: %d\n", msg, action);
        fclose(f);
    }
}

/*
 * Guards against debug messages coming up during normal operation.
 *
 * Just change debug_mode to 1 to activate it. We could have a toggle
 * in the program to turn it on during operation for when something starts
 * acting weird.
 */
void
debug_message(std::string msg)
{
    if (debug_mode) {
        std::cout << msg << std::endl;
    }
}

/* .
 */
int
valid_file_format(char *fname)
{
    if (fname[0] == 0) {
        return 0;
    }
    if (emb_identify_format(fname) >= 0) {
        return 1;
    }
    return 0;
}

/*
 *  Testing.
 *  Mostly tests for actions not causing crashes that shut the program.
 *
 *  Testing actual correct application of the action would be harder.
 */

/* stores what the current error would be, should one occur */

/* Check that the translate call can return an entry from the table.
 */
int
test_translate(void)
{
    std::string translated = translate("?");
    printf("Failed to use translate.");
    return translated == "?";
}

/* .
 */
double
emb_clamp(double lower, double x, double upper)
{
    return std::max(std::min(upper, x), lower);
}

/* .
 */
int
valid_rgb(int red, int green, int blue)
{
    if ((red < 0) || (red > 255)) {
        return 0;
    }
    if ((green < 0) || (green > 255)) {
        return 0;
    }
    if ((blue < 0) || (blue > 255)) {
        return 0;
    }
    return 1;
}

FILE *
load_asset(std::string fname, char *mode)
{
    std::string asset = assets_directory + os_seperator + fname;
    FILE *f = fopen((char*)asset.data(), mode);
    if (!f) {
        std::cout << "Failed to open asset file:"
        << " could not open file " << asset << " in mode." << std::endl;
    }
    return f;
}

std::string
translate(std::string string)
{
    for (size_t i=0; i<translation_tables[language].size(); i++) {
        if (translation_tables[language][i][0] == string) {
            return translation_tables[language][i][1];
        }
    }
    return string;
}

/* https://doc.qt.io/qt-6/qopenglwidget.html
 *
 * Eventually move onto SDL/GLFW

void
GLView::initializeGL(void)
{

}

void
GLView::resizeGL(int w, int h)
{

}

void
GLView::paintGL(void)
{

}
 */

Application::Application(int argc, char **argv) : QApplication(argc, argv), _mainWin(NULL)
{
}

bool Application::event(QEvent *event)
{
    switch (event->type()) {
    case QEvent::FileOpen:
        if (_mainWin) {
            _mainWin->openFilesSelected(QStringList(static_cast<QFileOpenEvent *>(event)->file()));
            return true;
        }
        // Fall through
    default:
        return QApplication::event(event);
    }
}

static void usage(void)
{
    fprintf(stderr,
    " ___ _____ ___  ___   __  _ ___  ___ ___   _____  __  ___  ___  ___ ___    ___ "           "\n"
    "| __|     | _ \\| _ \\ /  \\| |   \\| __| _ \\ |     |/  \\|   \\|   \\| __| _ \\  |__ \\" "\n"
    "| __| | | | _ <|   /| () | | |) | __|   / | | | | () | |) | |) | __|   /  / __/"           "\n"
    "|___|_|_|_|___/|_|\\_\\\\__/|_|___/|___|_|\\_\\|_|_|_|\\__/|___/|___/|___|_|\\_\\ |___|"   "\n"
    " _____________________________________________________________________________ "           "\n"
    "|                                                                             | "          "\n"
    "|                   http://embroidermodder.github.io                          | "          "\n"
    "|_____________________________________________________________________________| "          "\n"
    "                                                                               "           "\n"
    "Usage: embroidermodder [options] files ..."                                      "\n"
   //80CHARS======================================================================MAX
    "Options:"                                                                        "\n"
    "  -d, --debug      Print lots of debugging information."                         "\n"
    "  -h, --help       Print this message and exit."                                 "\n"
    "  -v, --version    Print the version number of embroidermodder and exit."        "\n"
    "\n"
           );
    exitApp = true;
}

static void version()
{
    fprintf(stdout, "%s %s\n", _appName_, _appVer_);
    exitApp = true;
}

int main(int argc, char* argv[])
{
#if defined(Q_OS_MAC)
    Application app(argc, argv);
#else
    QApplication app(argc, argv);
#endif
    app.setApplicationName(_appName_);
    app.setApplicationVersion(_appVer_);

    QStringList filesToOpen;

    for(int i = 1; i < argc; i++)
    {
        if     (!strcmp(argv[i], "-d") || !strcmp(argv[i], "--debug")  ) {  }
        else if(!strcmp(argv[i], "-h") || !strcmp(argv[i], "--help")   ) { usage(); }
        else if(!strcmp(argv[i], "-v") || !strcmp(argv[i], "--version")) { version(); }
        else if(QFile::exists(argv[i]) && MainWindow::validFileFormat(argv[i]))
        {
            filesToOpen << argv[i];
        }
        else
        {
            usage();
        }
    }

    if(exitApp)
        return 1;

    MainWindow* mainWin = new MainWindow();
#if defined(Q_OS_MAC)
    app.setMainWin(mainWin);
#endif

    QObject::connect(&app, SIGNAL(lastWindowClosed()), mainWin, SLOT(quit()));

    mainWin->setWindowTitle(app.applicationName() + " " + app.applicationVersion());
    mainWin->show();

    //NOTE: If openFilesSelected() is called from within the mainWin constructor, slot commands wont work and the window menu will be screwed
    if(!filesToOpen.isEmpty())
        mainWin->openFilesSelected(filesToOpen);

    return app.exec();
}
