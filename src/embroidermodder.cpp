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


#include "imgui.h"
#include "backends/imgui_impl_glfw.h"
#include "backends/imgui_impl_opengl2.h"
#include "TextEditor.h"
#include "stb/stb_image.h"

#include "embroidermodder.h"

#include <iostream>
#include <fstream>

#include <GLFW/glfw3.h>
#include <embroidery.h>

bool running = true;
bool debug_mode = true;
bool show_about_dialog = false;
bool show_editor = false;
bool show_rulers = true;
bool show_grid = true;
std::string language = "default";
int icon_size = 16;
ImFont *font;
int pattern_index = 0;
int n_patterns = 0;
std::string current_fname = "Untitled.dst";
std::string assets_dir = "../assets/";
std::vector<Action> action_list;
EmbPattern* pattern_list[MAX_PATTERNS];
string_matrix translation_table;
std::unordered_map<std::string, GLuint> textures;
std::unordered_map<std::string, string_matrix> menu_layout;
std::string menu_action = "";
std::string current_pattern = "";
TextEditor editor;
std::vector<std::string> file_toolbar = {
    "new",
    "open",
    "save",
    "saveas",
    "print",
    "designdetails"
};
std::vector<std::string> edit_toolbar = {
    "undo",
    "redo",
    "cut",
    "copy",
    "paste"
};
std::vector<std::string> view_toolbar = {
    "day",
    "night",
    "zoomin",
    "zoomout",
    "zoomextents"
};
std::vector<std::string> help_toolbar = {
    "help"
};
std::vector<std::string> texture_list = {
    "new",
    "open",
    "save",
    "saveas",
    "print",
    "designdetails",
    "undo",
    "redo",
    "cut",
    "copy",
    "paste",
    "day",
    "night",
    "zoomin",
    "zoomout",
    "zoomextents",
    "help"
};
int testing = 0;


int
parse_command(int argc, char *argv[], std::vector<std::string> files)
{
    for (int i=1; i<argc; i++) {
        std::string s(argv[i]);
        if ((s == "--local-boot") || (s == "-L")) {
            assets_dir = argv[i+1];
            std::cout << "Booting from \"" << assets_dir << "\"." << std::endl;
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

GLuint
gen_gl_texture(uint8_t* data, int w, int h, char fmt)
{
    GLuint texture;

    glGenTextures(1, &texture);
    glBindTexture(GL_TEXTURE_2D, texture);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, w, h, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
#if defined(GL_UNPACK_ROW_LENGTH) && !defined(__EMSCRIPTEN__)
    glPixelStorei(GL_UNPACK_ROW_LENGTH, 0);
#endif
    glBindTexture(GL_TEXTURE_2D, 0);

    return texture;
}

int
load_textures(std::vector<std::string> texture_list)
{
    for (std::string icon : texture_list) {
        int width, height;
        std::string fname = assets_dir + "icons/default/" + icon + ".png";
        uint8_t* data = stbi_load(fname.c_str(), &width, &height, NULL, 4);
        if (!data) {
            return 1;
        }

        GLuint texture_id = gen_gl_texture(data, width, height, 0);
        stbi_image_free(data);

        textures[icon] = texture_id;
    }

    return 0;
}

void
set_style(void)
{
    std::string font_file = assets_dir + "fonts/SourceSans3-regular.ttf";
    ImGuiIO& io = ImGui::GetIO();
    ImFontConfig config;
    config.OversampleH = 2;
    config.OversampleV = 2;
    font = io.Fonts->AddFontFromFileTTF(font_file.c_str(), 16, &config);

    ImGuiStyle& style = ImGui::GetStyle();
    style.Colors[ImGuiCol_Text] = ImVec4(0.00f, 0.00f, 0.00f, 1.00f);
    style.Colors[ImGuiCol_FrameBgActive] = ImVec4(0.15f, 0.15f, 0.15f, 1.00f);
    style.Colors[ImGuiCol_PopupBg] = ImVec4(0.55f, 0.55f, 0.55f, 1.00f);
    style.Colors[ImGuiCol_WindowBg] = ImVec4(0.65f, 0.65f, 0.65f, 1.00f);
    style.Colors[ImGuiCol_MenuBarBg] = ImVec4(1.0f, 1.0f, 1.0f, 1.00f);
    style.Colors[ImGuiCol_ChildBg] = ImVec4(1.0f, 1.0f, 1.0f, 1.00f);
}

void
load_menu(std::string menu_label)
{
    if (ImGui::BeginMenu(menu_label.c_str())) {
        for (auto i : menu_layout[menu_label]) {
            if (i[0] == "---") {
                ImGui::Separator();
                continue;
            }
            if (i[1] == "submenu") {
                load_menu(i[0]);
                continue;
            }
            if (ImGui::MenuItem(i[0].c_str())) {
                menu_action = i[1];
            }
        }
        ImGui::EndMenu();
    }
}

void
load_toolbar(std::vector<std::string> toolbar)
{
    int length = toolbar.size();
    for (int i=0; i<length; i++) {
        std::string icon = toolbar[i];
        if (icon == "---") {
            ImGui::Separator();
            continue;
        }
        ImVec2 size = {icon_size, icon_size};
    	if (ImGui::ImageButton((void*)(intptr_t)textures[icon], size)) {
            menu_action = icon;
        }
        if (i+1 < length) {
            ImGui::SameLine();
        }
    }
}

void
main_widget(void)
{
	ImGuiViewport* viewport = ImGui::GetMainViewport();
    ImGui::SetNextWindowPos(ImVec2(0, 0));
    ImGui::SetNextWindowSize(viewport->Size);
    ImGui::Begin("Embroidermodder", NULL, ImGuiWindowFlags_MenuBar
        | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoTitleBar
        | ImGuiWindowFlags_NoMove );
    ImGui::SetWindowFontScale(1.5);

    menu_action = "";

    if (ImGui::BeginMenuBar()) {
        load_menu("File");
        load_menu("Edit");
        load_menu("View");
        load_menu("Draw");
        load_menu("Window");
        load_menu("Settings");
        load_menu("Help");
        ImGui::EndMenuBar();
    }

    load_toolbar(file_toolbar);
    ImGui::SameLine();
    load_toolbar(edit_toolbar);
    ImGui::SameLine();
    load_toolbar(view_toolbar);

    if (menu_action != "") {
        actuator(menu_action);
    }

    ImGui::Columns(2, "Undo History");
    ImGui::SetColumnWidth(-1, 100);
    ImGui::BeginChild("Undo History");
    ImGui::Text("Undo History");
    for (std::string undo_item : undo_history) {
        ImGui::Text(undo_item.c_str());
    }
    ImGui::EndChild();
    ImGui::NextColumn();
    if (ImGui:: BeginTabBar("Tab Bar")) {
        for (int i=0; i<n_patterns; i++) {
            if (ImGui::BeginTabItem("Untitled.dst")) {
                if (i == pattern_index) {
                    pattern_view();
                }
                ImGui::EndTabItem();
            }
        }
        if (show_editor) {
            if (ImGui::BeginTabItem("Text Editor")) {
                editor.Render("Text Editor");
                ImGui::EndTabItem();
            }
        }
        ImGui::EndTabBar();
    }
    ImGui::Columns();

    if (show_about_dialog) {
        about_dialog();
    }

    ImGui::End();
}

void
load_text_file(std::string fname)
{
    std::string line;
    std::ifstream file;
    file.open(fname);
    while (std::getline(file, line)) {
        editor.InsertText(line + "\n");
    }
}

int
main(int argc, char* argv[])
{
    if (argc>1) {
        // assets_dir = std::string(argv[1]);
        current_fname = argv[1];
    }

    load_configuration();

    int width = 640;
    int height = 480;
    if (!glfwInit()) {
        std::cout << "ERROR: Failed to initialise GLFW." << std::endl;
        return 2;
    }
    GLFWwindow *window = glfwCreateWindow(width, height,
        "Embroidermodder " VERSION, NULL, NULL);
    if (!window) {
        std::cout << "ERROR: Failed to create GLFW window." << std::endl;
        glfwTerminate();
        return 1;
    }
    glfwMakeContextCurrent(window);

    IMGUI_CHECKVERSION();
    ImGui::CreateContext();

    ImGui_ImplGlfw_InitForOpenGL(window, true);
    ImGui_ImplOpenGL2_Init();

    glfwGetFramebufferSize(window, &width, &height);
    ImVec2 v = {width, height};
    ImGui::SetNextWindowPos({0.0, 0.0});
    ImGui::SetNextWindowSize(v);

    set_style();

    load_textures(texture_list);

    /* load_text_file("imgui_config.toml"); */

    while (!glfwWindowShouldClose(window)) {
        glfwPollEvents();

        ImGui_ImplOpenGL2_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();

        main_widget();

        ImGui::Render();

        glfwGetFramebufferSize(window, &width, &height);
        glViewport(0, 0, width, height);
        glClearColor(0.1, 0.1, 0.1, 0.0);
        glClear(GL_COLOR_BUFFER_BIT);

        ImGui_ImplOpenGL2_RenderDrawData(ImGui::GetDrawData());

        glfwMakeContextCurrent(window);
        glfwSwapBuffers(window);
        if (!running) {
            break;
        }
    }

    glfwDestroyWindow(window);
    glfwTerminate();
    return 0;
}

#if 0

/*
Action action_list[] = {
    {"donothing", "&Do Nothing", "Does Nothing"},
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
*/

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

bool Application::event(QEvent *event)
{
    switch (event->type()) {
    case QEvent::FileOpen:
        if (_mainWin) {
            _mainWin->openFilesSelected(std::stringList(static_cast<QFileOpenEvent *>(event)->file()));
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

    std::stringList filesToOpen;

    for(int i = 1; i < argc; i++)
    {
        if     (!strcmp(argv[i], "-d") || !strcmp(argv[i], "--debug")  ) {  }
        else if (!strcmp(argv[i], "-h") || !strcmp(argv[i], "--help")   ) { usage(); }
        else if (!strcmp(argv[i], "-v") || !strcmp(argv[i], "--version")) { version(); }
        else if (QFile::exists(argv[i]) && MainWindow::validFileFormat(argv[i]))
        {
            filesToOpen << argv[i];
        }
        else
        {
            usage();
        }
    }

    if (exitApp)
        return 1;

    MainWindow* mainWin = new MainWindow();
#if defined(Q_OS_MAC)
    app.setMainWin(mainWin);
#endif

    QObject::connect(&app, SIGNAL(lastWindowClosed()), mainWin, SLOT(quit()));

    mainWin->setWindowTitle(app.applicationName() + " " + app.applicationVersion());
    mainWin->show();

    //NOTE: If openFilesSelected() is called from within the mainWin constructor, slot commands wont work and the window menu will be screwed
    if (!filesToOpen.isEmpty())
        mainWin->openFilesSelected(filesToOpen);

    return app.exec();
}

#endif