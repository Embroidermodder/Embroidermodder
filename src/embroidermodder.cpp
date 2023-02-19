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


#include "imgui_ext.h"
#include "backends/imgui_impl_glfw.h"
#include "backends/imgui_impl_opengl2.h"
#include "TextEditor.h"
#include "stb/stb_image.h"

#include "embroidermodder.h"

#ifdef __linux__
#include <sys/utsname.h>
#endif

#include <cmath>
#include <string>
#include <iostream>
#include <fstream>

#include <tinydir.h>
#include <GLFW/glfw3.h>
#include <embroidery.h>

ImFont *font;
ImFont *header_font;
bool show_open_file_dialog = false;
string_matrix translation_table;
std::unordered_map<std::string, std::string> str_settings;
std::unordered_map<std::string, GLuint> textures;
std::unordered_map<std::string, string_matrix> menu_layout;
std::string menu_action = "";
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
std::vector<std::string> draw_toolbar = {
    "arc",
    "circle",
    "ellipse",
    "rectangle"
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
    "help",
    "arc",
    "circle",
    "ellipse"
};
int testing = 0;

void about_dialog(void)
{
    debug_message("about_dialog()");

    ImGui::Begin("About Embroidermodder 2");
    // ImageWidget img(assets_dir + "/images/logo-small");
    ImGui::Text("Embroidermodder " VERSION);
    ImGui::Text("https://www.libembroidery.org");
    ImGui::Text("Available Platforms: GNU/Linux, Windows, Mac OSX, Raspberry Pi.");
    ImGui::Text("Embroidery formats by Josh Varga and Robin Swift.");
    ImGui::Text("User Interface by Jonathan Greig and Robin Swift.");
    ImGui::Text("Free under the zlib/libpng license.");
    if (ImGui::Button("Oh, Yeah!")) {
        settings.show_about_dialog = false;
    }
    ImGui::End();
}

void changelog_dialog(void)
{
    ImGui::Begin("Changelog");
    ImGui::End();
}

void help_dialog(void)
{
    ImGui::Begin("Help");

    ImGui::Text("Help -- Embroidermodder 2");

    /* Minimum size (600, 500). */

    std::ifstream file;
    file.open(settings.assets_dir + "docs/manual.md");
    std::string s, line;
    while (std::getline(file, line)) {
        s += line + "\n";
    }

    ImGui::TextWrapped("%s", s.c_str());

    /* Center button */
    if (ImGui::Button(translate("Close").c_str())) {

    }

    ImGui::End();    
}

void alert(std::string title, std::string message)
{
    ImGui::Begin(title.c_str());
    ImGui::TranslatedText(message);
    if (ImGui::Button(translate("Ok").c_str())) {
        
    }
    ImGui::End();
}

std::string to_open = "";
std::string current_directory = ".";

void open_file_dialog(void)
{
    // char typed_filename[400];
    ImGui::Begin("Open File");
    ImGui::SetWindowFontScale(1.5);
    tinydir_dir dir;
    if (tinydir_open_sorted(&dir, current_directory.c_str()) == -1) {
        ImGui::Text("Failed to open directory.");
        ImGui::End();
        return;
    }
    int n_columns = 1 + dir.n_files/5;
    ImGui::Columns(n_columns, "files");
    for (size_t i=0; i<dir.n_files; i++) {
        tinydir_file file;
        tinydir_readfile_n(&dir, &file, i);
        if (ImGui::Button(file.name)) {
            if (file.is_dir) {
                /* TODO: this could lead to nonsense like "dir/../dir/../dir"
                 * rather than just "dir".
                 */
                current_directory = current_directory + "/" + std::string(file.name);
            }
            else {
                to_open = current_directory + "/" + std::string(file.name);
            }
        }
        tinydir_next(&dir);
        if (i%5 == 4) {
            ImGui::NextColumn();
        }
    }
    ImGui::Columns();
    ImGui::Text("%s", to_open.c_str());
    //ImGui::InputText("", typed_filename, 100);
    ImGui::SameLine();
    if (ImGui::Button("Open")) {
        actuator("open " + to_open);
        show_open_file_dialog = false;
    }
    ImGui::SameLine();
    if (ImGui::Button("Cancel")) {
        show_open_file_dialog = false;
    }
    tinydir_close(&dir);
    ImGui::End();
}

/* TODO: Move majority of the calculation code into libembroidery
 *
 * Switch to grid layout.
 */
void details_dialog(void)
{
    if (views.size() < 1) {
        return;
    }
    View view = views[settings.pattern_index];

    ImGui::Begin(translate("Embroidery Design Details").c_str());
    ImGui::SetWindowFontScale(1.5);

    EmbRect boundingRect = embPattern_calcBoundingBox(view.pattern);

    int colorChanges  = 0;

    if (view.pattern->stitchList->count == 0) {
        alert(translate("No Design Loaded"), translate("<b>A design needs to be loaded or created before details can be determined.</b>"));
        return;
    }

    std::vector<double> stitchLengths;

    /*
    int colors = 0;
    double totalColorLength = 0.0;
    for (int i = 0; i < sts->count; i++) {
        EmbStitch st = sts->stitch[i];
        totalColorLength += length;
        if (i > 0 && sts->stitch[i-1].flags != NORMAL)
            length = 0.0; //can't count first normal stitch;
        if (sts->stitch[i].flags & STOP) {
            stitchLengths.push_back(totalColorLength);
            totalColorLength = 0;
            colors++;
        }
        if (sts->stitch[i].flags & END) {
            stitchLengths.push_back(totalColorLength);
        }
    }

    //second pass to fill bins now that we know max stitch length
#define NUMBINS 10
    int bin[NUMBINS+1];
    embPattern_lengthHistogram(sts, bin, NUMBINS);

    double binSize = max_stitchlength / NUMBINS;
    std::string str;
    for (int i = 0; i < NUMBINS; i++) {
        str += std::to_string(binSize * (i));
        str += " - " + std::to_string(binSize * (i+1));
        str += " mm: " +  std::to_string(bin[i]) + "\n\n";
    }

    ImGui::TranslatedText("Stitch Distribution:");
    ImGui::Text("%s", str.c_str());
    ImGui::TranslatedText("Thread Length By Color:");
    int currentRow = 12;

    for (int i = 0; i < pattern->n_threads; i++) {
        QFrame *frame = new QFrame();
        frame->setGeometry(0,0,30,30);
        QPalette palette = frame->palette();
        EmbColor t = embThreadList_getAt(pattern->threadList, i).color;
        palette.setColor(backgroundRole(), QColor( t.r, t.g, t.b ) );
        frame->setPalette( palette );
        frame->setAutoFillBackground(true);
        grid->addWidget(frame, currentRow,0,1,1);
        debug_message("size: %d i: %d", stitchLengths.size(), i);
        grid->addWidget(new QLabel(std::string::number(stitchLengths.at(i)) + " mm"), currentRow,1,1,1);
        currentRow++;
    }
    */

    /* setMinimumSize(750,550); */

    /* Collapsing header? */
    ImGui::TranslatedText("General Information");

    ImGui::TranslatedText("Total Stitches:");
    ImGui::SameLine();
    ImGui::Text("%s", std::to_string(view.pattern->stitchList->count).c_str());

    ImGui::TranslatedText("Total Stitch Length:");
    ImGui::SameLine();
    ImGui::Text("%s", std::to_string(embPattern_totalStitchLength(view.pattern)).c_str());

    ImGui::TranslatedText("Real Stitches:");
    ImGui::SameLine();
    ImGui::Text("%s", std::to_string(embPattern_realStitches(view.pattern)).c_str());

    ImGui::TranslatedText("Jump Stitches:");
    ImGui::SameLine();
    ImGui::Text("%s", std::to_string(embPattern_jumpStitches(view.pattern)).c_str());

    ImGui::TranslatedText("Trim Stitches:");
    ImGui::SameLine();
    ImGui::Text("%s", std::to_string(embPattern_trimStitches(view.pattern)).c_str());

    ImGui::TranslatedText("Total Colors:");
    ImGui::SameLine();
    ImGui::Text("%s", std::to_string(view.pattern->n_threads).c_str());

    ImGui::TranslatedText("Color Changes:");
    ImGui::SameLine();
    ImGui::Text("%s", std::to_string(colorChanges).c_str());

    ImGui::TranslatedText("Left:");
    ImGui::SameLine();
    ImGui::Text("%s", (std::to_string(boundingRect.left) + " mm").c_str());

    ImGui::TranslatedText("Top:");
    ImGui::SameLine();
    ImGui::Text("%s", (std::to_string(boundingRect.top) + " mm").c_str());

    ImGui::TranslatedText("Right:");
    ImGui::SameLine();
    ImGui::Text("%s", (std::to_string(boundingRect.right)  + " mm").c_str());

    ImGui::TranslatedText("Bottom:");
    ImGui::SameLine();
    ImGui::Text("%s", (std::to_string(boundingRect.bottom) + " mm").c_str());

    ImGui::TranslatedText("Width:");
    ImGui::SameLine();
    ImGui::Text("%s", (std::to_string(boundingRect.right - boundingRect.left)  + " mm").c_str());

    ImGui::TranslatedText("Height:");
    ImGui::SameLine();
    ImGui::Text("%s", (std::to_string(boundingRect.bottom - boundingRect.top) + " mm").c_str());

    if (ImGui::Button("Ok")) {
        show_details_dialog = false;
    }

    ImGui::End();

    //TODO: Color Histogram

    //Stitch Distribution
    //QGroupBox* groupBoxDist = new QGroupBox(tr("Stitch Distribution"), widget);

    //TODO: Stitch Distribution Histogram
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
    settings.running = false;
}

std::vector<std::string> parse_command(int argc, char *argv[])
{
    std::vector<std::string> files;
    for (int i=1; i<argc; i++) {
        std::string s(argv[i]);
        if ((s == "--local-boot") || (s == "-L")) {
            settings.assets_dir = argv[i+1];
            std::cout << "Booting from \"" << settings.assets_dir << "\"." << std::endl;
            i++;
            continue;
        }
        if ((s == "--debug") || (s == "-d")) {
            settings.debug_mode = 1;
            printf("DEBUG MODE\n");
            continue;
        }
        if ((s == "--help") || (s == "-h")) {
            usage();
        }
        if ((s == "--version") || (s == "-v")) {
            /* For scripts that need the version string */
            std::cout << VERSION << std::endl;
            settings.running = false;
        }
        if (s == "--test") {
            testing = 1;
            continue;
        }
        files.push_back(s);
    }
    return files;
}

GLuint
gen_gl_texture(uint8_t* data, int w, int h, int wrap)
{
    GLuint texture;

    glGenTextures(1, &texture);
    glBindTexture(GL_TEXTURE_2D, texture);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, wrap);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, wrap);
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
        std::string fname = settings.assets_dir + "icons/default/" + icon + ".png";
        uint8_t* data = stbi_load(fname.c_str(), &width, &height, NULL, 4);
        if (!data) {
            return 1;
        }

        GLuint texture_id = gen_gl_texture(data, width, height, GL_CLAMP_TO_EDGE);
        stbi_image_free(data);

        textures[icon] = texture_id;
    }

    {
        int width, height;
        std::string fname = settings.assets_dir + "images/texture-spirals.png";
        uint8_t* data = stbi_load(fname.c_str(), &width, &height, NULL, 4);
        if (!data) {
            return 1;
        }

        GLuint texture_id = gen_gl_texture(data, width, height, GL_REPEAT);
        stbi_image_free(data);

        textures["texture-spirals"] = texture_id;
    }

    return 0;
}

void
set_style(void)
{
    std::string font_file = settings.assets_dir + "fonts/SourceSans3-Regular.ttf";
    ImGuiIO& io = ImGui::GetIO();
    ImFontConfig config;
    config.OversampleH = 2;
    config.OversampleV = 2;
    if (file_exists(font_file)) {
        font = io.Fonts->AddFontFromFileTTF(font_file.c_str(), 16, &config);
        header_font = io.Fonts->AddFontFromFileTTF(font_file.c_str(), 24, &config);
    }
    else {
        std::cout << "ERROR: Failed to load font file." << std::endl;
    }

    ImGuiStyle& style = ImGui::GetStyle();
    style.Colors[ImGuiCol_Text] = ImVec4(0.00f, 0.00f, 0.00f, 1.00f);
    style.Colors[ImGuiCol_FrameBgActive] = ImVec4(0.75f, 0.75f, 0.75f, 1.00f);
    style.Colors[ImGuiCol_PopupBg] = ImVec4(0.85f, 0.85f, 0.85f, 1.00f);
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
        ImVec2 size = {1.0f*settings.icon_size, 1.0f*settings.icon_size};
    	if (ImGui::ImageButton((void*)(intptr_t)textures[icon], size)) {
            menu_action = icon;
        }
        if (i+1 < length) {
            ImGui::SameLine();
        }
    }
}

void
undo_history_viewer(void)
{
    ImGui::BeginChild(("Undo History " + views[settings.pattern_index].filename).c_str());
    ImGui::PushFont(header_font);
    ImGui::TranslatedText("Undo History");
    ImGui::PopFont();
    for (std::string undo_item : views[settings.pattern_index].undo_history) {
        ImGui::TranslatedText(undo_item);
    }
    ImGui::EndChild();
}

void
view_tab(int i)
{
    ImGuiIO& io = ImGui::GetIO();
    if (ImGui::BeginTabItem(views[i].filename.c_str())) {
        settings.pattern_index = i;
        ImGui::Columns(3, ("Undo History" + views[settings.pattern_index].filename).c_str());
        ImGui::SetColumnWidth(-1, 200);
        undo_history_viewer();
        ImGui::NextColumn();
        ImGui::SetColumnWidth(-1, io.DisplaySize.x - 605);
        pattern_view();
        ImGui::NextColumn();
        ImGui::SetColumnWidth(-1, 400);
        property_editor();
        ImGui::Columns();
        ImGui::EndTabItem();
    }
}

void
create_toolbars(void)
{
    load_toolbar(file_toolbar);
    ImGui::SameLine();
    load_toolbar(edit_toolbar);
    ImGui::SameLine();
    load_toolbar(draw_toolbar);

    load_toolbar(view_toolbar);
    /*
    debug_message("MainWindow createAllToolbars()");

    add_to_toolbar(toolbarFile, "toolbarFile", toolbar_file_entries);
    add_to_toolbar(toolbarEdit, "toolbarEdit", toolbar_edit_entries);
    add_to_toolbar(toolbarView, "toolbarView", toolbar_view_entries);
    add_to_toolbar(toolbarZoom, "toolbarZoom", toolbar_zoom_entries);
    add_to_toolbar(toolbarPan, "toolbarPan", toolbar_pan_entries);
    add_to_toolbar(toolbarIcon, "toolbarIcon", toolbar_icon_entries);
    add_to_toolbar(toolbarHelp, "toolbarHelp", toolbar_help_entries);

    createLayerToolbar();
    createPropertiesToolbar();
    createTextToolbar();
    createPromptToolbar();

    // Horizontal
    toolbarView->setOrientation(Horizontal);
    toolbarZoom->setOrientation(Horizontal);
    toolbarLayer->setOrientation(Horizontal);
    toolbarProperties->setOrientation(Horizontal);
    toolbarText->setOrientation(Horizontal);
    toolbarPrompt->setOrientation(Horizontal);
    // Top
    addToolBarBreak(TopToolBarArea);
    addToolBar(TopToolBarArea, toolbarFile);
    addToolBar(TopToolBarArea, toolbarEdit);
    addToolBar(TopToolBarArea, toolbarHelp);
    addToolBar(TopToolBarArea, toolbarIcon);
    addToolBarBreak(TopToolBarArea);
    addToolBar(TopToolBarArea, toolbarZoom);
    addToolBar(TopToolBarArea, toolbarPan);
    addToolBar(TopToolBarArea, toolbarView);
    addToolBarBreak(TopToolBarArea);
    addToolBar(TopToolBarArea, toolbarLayer);
    addToolBar(TopToolBarArea, toolbarProperties);
    addToolBarBreak(TopToolBarArea);
    addToolBar(TopToolBarArea, toolbarText);
    // Bottom
    addToolBar(BottomToolBarArea, toolbarPrompt);

    //zoomToolBar->setToolButtonStyle(ToolButtonTextOnly);
    */
}

void create_menus(void)
{
    if (ImGui::BeginMenuBar()) {
        load_menu("File");
        load_menu("Edit");
        load_menu("View");
        load_menu("Tools");
        load_menu("Draw");
        load_menu("Window");
        load_menu("Settings");
        load_menu("Help");
        ImGui::EndMenuBar();
    }
    /*
    menuBar()->addMenu(fileMenu);
    fileMenu->addAction(actionHash.value(ACTION_new));
    fileMenu->addSeparator();
    fileMenu->addAction(actionHash.value(ACTION_open));

    fileMenu->addMenu(recentMenu);
    connect(recentMenu, SIGNAL(aboutToShow()), this, SLOT(recentMenuAboutToShow()));
    //Do not allow the Recent Menu to be torn off. It's a pain in the ass to maintain.
    recentMenu->setTearOffEnabled(false);
    create_menu(actionHash, fileMenu, file_menu_data, false);

    menuBar()->addMenu(editMenu);
    create_menu(actionHash, editMenu, edit_menu_data, true);

    menuBar()->addMenu(viewMenu);
    viewMenu->addSeparator();
    viewMenu->addMenu(zoomMenu);
    zoomMenu->setIcon(load_icon("zoom"));
    create_menu(actionHash, zoomMenu, zoom_menu_data, true);
    viewMenu->addMenu(panMenu);
    panMenu->setIcon(load_icon("pan"));
    create_menu(actionHash, panMenu, pan_menu_data, true);
    create_menu(actionHash, viewMenu, view_menu_data, true);

    menuBar()->addMenu(settingsMenu);
    create_menu(actionHash, settingsMenu, settings_menu_data, true);

    menuBar()->addMenu(windowMenu);
    connect(windowMenu, SIGNAL(aboutToShow()), this, SLOT(windowMenuAboutToShow()));
    // Do not allow the Window Menu to be torn off. It's a pain in the ass to maintain.
    windowMenu->setTearOffEnabled(false);

    menuBar()->addMenu(helpMenu);
    create_menu(actionHash, helpMenu, help_menu_data, true);
    */
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

    create_menus();

    create_toolbars();

    if (menu_action != "") {
        actuator(menu_action);
    }

    if (views.size() > 0) {
        if (ImGui:: BeginTabBar("Tab Bar")) {
            int n_views = views.size();
            for (int i=0; i<n_views; i++) {
                view_tab(i);
            }
            if (settings.show_editor) {
                if (ImGui::BeginTabItem("Text Editor")) {
                    editor.Render("Text Editor");
                    ImGui::EndTabItem();
                }
            }
            ImGui::EndTabBar();
        }
    }
    else {
        ImVec2 size = {500, 300};
        ImGui::Image((void*)(intptr_t)textures["texture-spirals"], size);
    }

    if (settings.show_about_dialog) {
        about_dialog();
    }

    if (show_details_dialog) {
        details_dialog();
    }

    if (show_open_file_dialog) {
        open_file_dialog();
    }

    if (settings.show_settings_editor) {
        settings_editor();
    }

    status_bar();

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
    std::vector<std::string> files = parse_command(argc, argv);
    if (!settings.running) {
        return 0;
    }

    load_configuration();
    read_settings();

    std::string command = "open";
    for (std::string file : files) {
        command += " " + file;
    }
    if (command != "open") {
        actuator(command);
    }

    int width = 1080;
    int height = 576;
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
    ImVec2 v = {1.0f*width, 1.0f*height};
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
        if (!settings.running) {
            break;
        }
    }

    glfwDestroyWindow(window);
    glfwTerminate();
    return 0;
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
double emb_clamp(double lower, double x, double upper)
{
    return std::max(std::min(upper, x), lower);
}

/* .
 */
int valid_rgb(int red, int green, int blue)
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

/*
bool Application_event(QEvent *event)
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

void init_mdi_area(int i)
{
    mdi_area[i].useLogo = false;
    mdi_area[i].useTexture = false;
    mdi_area[i].useColor = false;
    mdi_area[i].bgLogo = "";
    mdi_area[i].bgTexture = "";
    mdi_area[i].bgColor = IM_COL32(40, 20, 10, 255);
}

void mdi_area_double_click(void)
{
    mainWin->openFile();
}

void MdiArea_paintEvent(QPaintEvent* e)
{
    QWidget* vport = viewport();
    QRect rect = vport->rect();

    QPainter painter(vport);
    painter.setRenderHint(QPainter::SmoothPixmapTransform);

    //Always fill with a solid color first
    if (useColor) 
        painter.fillRect(rect, bgColor);
    else
        painter.fillRect(rect, background());

    //Then overlay the texture
    if (useTexture) {
        QBrush bgBrush(bgTexture);
        painter.fillRect(rect, bgBrush);
    }

    //Overlay the logo last
    if (useLogo) {
        //Center the pixmap
        int dx = (rect.width()-bgLogo.width())/2;
        int dy = (rect.height()-bgLogo.height())/2;
        painter.drawPixmap(dx, dy, bgLogo.width(), bgLogo.height(), bgLogo);
    }
}

void MdiArea_cascade()
{
    cascadeSubWindows();
    zoomExtentsAllSubWindows();
}

void MdiArea_tile()
{
    tileSubWindows();
    zoomExtentsAllSubWindows();
}

void MdiArea_zoomExtentsAllSubWindows()
{
    for (QMdiSubWindow* window : subWindowList()) {
        MdiWindow* mdiWin = qobject_cast<MdiWindow*>(window);
        if (mdiWin) {
            View* v = mdiWin->gview;
            if (v) {
                v->recalculateLimits();
                v->zoomExtents();
            }
        }
    }
}

void init_MdiWindow(const int theIndex, MainWindow* mw, QMdiArea* parent, Qt::WindowFlags wflags)
{
    mainWin = mw;
    mdiArea = parent;

    myIndex = theIndex;

    fileWasLoaded = false;

    setAttribute(Qt::WA_DeleteOnClose);

    std::string aName;
    // TODO sprintf in Qt6
    //curFile = aName.sprintf("Untitled%d.dst", myIndex);
    curFile = "Untitled.dst";
    this->setWindowTitle(curFile);

    this->setWindowIcon(mainWin->load_icon("app"));

    gscene = new QGraphicsScene(0,0,0,0, this);
    gview = new View(mainWin, gscene, this);

    setWidget(gview);

    //WARNING: DO NOT SET THE QMDISUBWINDOW (this) FOCUSPROXY TO THE PROMPT
    //WARNING: AS IT WILL CAUSE THE WINDOW MENU TO NOT SWITCH WINDOWS PROPERLY!
    //WARNING: ALTHOUGH IT SEEMS THAT SETTING INTERNAL WIDGETS FOCUSPROXY IS OK.
    gview->setFocusProxy(mainWin->prompt);

    resize(sizeHint());

    promptHistory = "Welcome to Embroidermodder 2!<br/>Open some of our sample files. Many formats are supported.<br/>For help, press F1.";
    mainWin->prompt->setHistory(promptHistory);
    promptInputList << "";
    promptInputNum = 0;

    curLayer = "0";
    curColor = 0; //TODO: color ByLayer
    curLineType = "ByLayer";
    curLineWeight = "ByLayer";

    // Due to strange Qt4.2.3 feature the child window icon is not drawn
    // in the main menu if showMaximized() is called for a non-visible child window
    // Therefore calling show() first...
    show();
    showMaximized();

    setFocusPolicy(Qt::WheelFocus);
    setFocus();

    onWindowActivated();
}

//TODO: Save a Brother PEL image (An 8bpp, 130x113 pixel monochromatic? bitmap image) Why 8bpp when only 1bpp is needed?

//TODO: Should BMC be limited to ~32KB or is this a mix up with Bitmap Cache?
//TODO: Is there/should there be other embedded data in the bitmap besides the image itself?
//NOTE: Can save a Singer BMC image (An 8bpp, 130x113 pixel colored bitmap image)
void MdiWindow::saveBMC()
{
    //TODO: figure out how to center the image, right now it just plops it to the left side.
    QImage img(150, 150, QImage::Format_ARGB32_Premultiplied);
    img.fill(qRgb(255,255,255));
    EmbRect extents = gscene->itemsBoundingRect();

    QPainter painter(&img);
    EmbRect targetRect(0,0,150,150);
    // TODO: Make BMC background into it's own setting?
    if (settings_printing_disable_bg) {
        QBrush brush = gscene->backgroundBrush();
        gscene->setBackgroundBrush(Qt::NoBrush);
        gscene->update();
        gscene->render(&painter, targetRect, extents, Qt::KeepAspectRatio);
        gscene->setBackgroundBrush(brush);
    }
    else {
        gscene->update();
        gscene->render(&painter, targetRect, extents, Qt::KeepAspectRatio);
    }

    img.convertToFormat(QImage::Format_Indexed8, Qt::ThresholdDither|Qt::AvoidDither).save("test.bmc", "BMP");
}

void MdiWindow::setCurrentFile(const std::string &fileName)
{
    curFile = QFileInfo(fileName).canonicalFilePath();
    setWindowModified(false);
    setWindowTitle(getShortCurrentFile());
}

std::string MdiWindow::getShortCurrentFile()
{
    return QFileInfo(curFile).fileName();
}

std::string MdiWindow::fileExtension(const std::string& fileName)
{
    return QFileInfo(fileName).suffix().toLower();
}

void MdiWindow::closeEvent(QCloseEvent* e)
{
    debug_message("MdiWindow closeEvent()");
    emit sendCloseMdiWin(this);
}

void MdiWindow::onWindowActivated()
{
    debug_message("MdiWindow onWindowActivated()");
    gview->undoStack->setActive(true);
    mainWin->setUndoCleanIcon(fileWasLoaded);
    mainWin->statusbar->statusBarSnapButton->setChecked(gscene->property("ENABLE_SNAP").toBool());
    mainWin->statusbar->statusBarGridButton->setChecked(gscene->property("ENABLE_GRID").toBool());
    mainWin->statusbar->statusBarRulerButton->setChecked(gscene->property("ENABLE_RULER").toBool());
    mainWin->statusbar->statusBarOrthoButton->setChecked(gscene->property("ENABLE_ORTHO").toBool());
    mainWin->statusbar->statusBarPolarButton->setChecked(gscene->property("ENABLE_POLAR").toBool());
    mainWin->statusbar->statusBarQSnapButton->setChecked(gscene->property("ENABLE_QSNAP").toBool());
    mainWin->statusbar->statusBarQTrackButton->setChecked(gscene->property("ENABLE_QTRACK").toBool());
    mainWin->statusbar->statusBarLwtButton->setChecked(gscene->property("ENABLE_LWT").toBool());
    mainWin->prompt->setHistory(promptHistory);
}

QSize MdiWindow::sizeHint() const
{
    debug_message("MdiWindow sizeHint()");
    return QSize(450, 300);
}

void MdiWindow::currentLayerChanged(const std::string& layer)
{
    curLayer = layer;
}

void MdiWindow::currentColorChanged(const unsigned int& color)
{
    curColor = color;
}

void MdiWindow::currentLinetypeChanged(const std::string& type)
{
    curLineType = type;
}

void MdiWindow::currentLineweightChanged(const std::string& weight)
{
    curLineWeight = weight;
}

void MdiWindow::updateColorLinetypeLineweight()
{
}

void MdiWindow::deletePressed()
{
    gview->deletePressed();
}

void MdiWindow::escapePressed()
{
    gview->escapePressed();
}

void MdiWindow::showViewScrollBars(bool val)
{
    gview->showScrollBars(val);
}

void MdiWindow::setViewCrossHairColor(unsigned int color)
{
    gview->setCrossHairColor(color);
}

void MdiWindow::setViewBackgroundColor(unsigned int color)
{
    gview->setBackgroundColor(color);
}

void MdiWindow::setViewSelectBoxColors(unsigned int colorL, unsigned int fillL, unsigned int colorR, unsigned int fillR, int alpha)
{
    gview->setSelectBoxColors(colorL, fillL, colorR, fillR, alpha);
}

void MdiWindow::setViewGridColor(unsigned int color)
{
    gview->setGridColor(color);
}

void MdiWindow::setViewRulerColor(unsigned int color)
{
    gview->setRulerColor(color);
}

void MdiWindow::promptHistoryAppended(const std::string& txt)
{
    promptHistory.append("<br/>" + txt);
}

void MdiWindow::logPromptInput(const std::string& txt)
{
    promptInputList << txt;
    promptInputNum = promptInputList.size();
}

void MdiWindow::promptInputPrevious()
{
    promptInputPrevNext(true);
}

void MdiWindow::promptInputNext()
{
    promptInputPrevNext(false);
}

void MdiWindow::promptInputPrevNext(bool prev)
{
    if (promptInputList.isEmpty()) {
        if (prev) {
            critical_messagebox(translate("Prompt Previous Error"), translate("The prompt input is empty! Please report this as a bug!"));
        }
        else {
            critical_messagebox(translate("Prompt Next Error"), translate("The prompt input is empty! Please report this as a bug!"));
            debug_message("The prompt input is empty! Please report this as a bug!");
        }
    }
    else {
        if (prev) {
            promptInputNum--;
        }
        else {
            promptInputNum++;
        }
        int maxNum = promptInputList.size();
        if (promptInputNum < 0) {
            promptInputNum = 0;
            mainWin->prompt->setCurrentText("");
        }
        else if (promptInputNum >= maxNum) {
            promptInputNum = maxNum;
            mainWin->prompt->setCurrentText("");
        }
        else {
            mainWin->prompt->setCurrentText(promptInputList.at(promptInputNum));
        }
    }
}

void PreviewDialog(QWidget* parent,
                   std::string& caption,
                   std::string& dir,
                   std::string& filter) : QFileDialog(parent, caption, dir, filter)
{
    debug_message("PreviewDialog Constructor");

    //TODO: get actual thumbnail image from file, lets also use a size of 128x128 for now...
    //TODO: make thumbnail size adjustable thru settings dialog
    imgWidget = new ImageWidget("icons/default/nopreview.png", this);

    QLayout* lay = layout();
    if (qobject_cast<QGridLayout*>(lay))
    {
        QGridLayout* grid = qobject_cast<QGridLayout*>(lay);
        grid->addWidget(imgWidget, 0, grid->columnCount(), grid->rowCount(), 1);
    }

    setModal(true);
    setOption(QFileDialog::DontUseNativeDialog);
    setViewMode(QFileDialog::Detail);
    setFileMode(QFileDialog::ExistingFiles);

    //TODO: connect the currentChanged signal to update the preview imgWidget.
}
*/

void status_bar(void)
{
    //ImGuiIO &io = ImGui::GetIO();
    //ImGui::SetNextWindowSize(ImVec2(50, 100));
    // ImGui::SetNextWindowPos(ImVec2(io.DisplaySize.x, io.DisplaySize.y - 50));
    //ImGui::BeginChild("Statusbar");
    if (ImGui::Button("SNAP")) {
        views[settings.pattern_index].snap_mode = !views[settings.pattern_index].snap_mode;
        /* change button depressed state */
    }
    ImGui::SameLine();
    if (ImGui::Button("GRID")) {
        views[settings.pattern_index].grid_mode = !views[settings.pattern_index].grid_mode;
    }
    ImGui::SameLine();
    if (ImGui::Button("RULER")) {
        views[settings.pattern_index].ruler_mode = !views[settings.pattern_index].ruler_mode;
    }
    ImGui::SameLine();
    if (ImGui::Button("ORTHO")) {
        views[settings.pattern_index].ortho_mode = !views[settings.pattern_index].ortho_mode;
    }
    ImGui::SameLine();
    if (ImGui::Button("POLAR")) {
        views[settings.pattern_index].polar_mode = !views[settings.pattern_index].polar_mode;
    }
    ImGui::SameLine();
    if (ImGui::Button("QSNAP")) {
        views[settings.pattern_index].qsnap_mode = !views[settings.pattern_index].qsnap_mode;
    }
    ImGui::SameLine();
    if (ImGui::Button("QTRACK")) {
        views[settings.pattern_index].qtrack_mode = !views[settings.pattern_index].qtrack_mode;
    }
    ImGui::SameLine();
    if (ImGui::Button("LWT")) {
        views[settings.pattern_index].lwt_mode = !views[settings.pattern_index].lwt_mode;
    }
    ImGui::SameLine();
    ImGui::Text("TODO: Mouse co-ordinates here.");

    // statusBarMouseCoord->setMinimumWidth(300); // Must fit this text always
    // statusBarMouseCoord->setMaximumWidth(300); // "+1.2345E+99, +1.2345E+99, +1.2345E+99"
    //ImGui::EndChild();
}


void setMouseCoord(double x, double y)
{
    //TODO: set format from settings (Architectural, Decimal, Engineering, Fractional, Scientific)

    //Decimal
    //statusBarMouseCoord->setText(std::string().setNum(x, 'F', 4) + ", " + std::string().setNum(y, 'F', 4)); //TODO: use precision from unit settings

    //Scientific
    //statusBarMouseCoord->setText(std::string().setNum(x, 'E', 4) + ", " + std::string().setNum(y, 'E', 4)); //TODO: use precision from unit settings
}

/*
void status_bar_context_menu(void)
{
    QMenu menu(this);
    if (objectName() == "StatusBarButtonSNAP") {
        QAction* settingsSnapAction = new QAction(mainWin->load_icon("gridsnapsettings"), "&Settings...", &menu);
        connect(settingsSnapAction, SIGNAL(triggered()), this, SLOT(settingsSnap()));
        menu.addAction(settingsSnapAction);
    }
    else if (objectName() == "StatusBarButtonGRID") {
        QAction* settingsGridAction = new QAction(mainWin->load_icon("gridsettings"), "&Settings...", &menu);
        menu.addAction(settingsGridAction);
    }
    else if (objectName() == "StatusBarButtonRULER") {
        QAction* settingsRulerAction = new QAction(mainWin->load_icon("rulersettings"), "&Settings...", &menu);
        menu.addAction(settingsRulerAction);
    }
    else if (objectName() == "StatusBarButtonORTHO") {
        QAction* settingsOrthoAction = new QAction(mainWin->load_icon("orthosettings"), "&Settings...", &menu);
        menu.addAction(settingsOrthoAction);
    }
    else if (objectName() == "StatusBarButtonPOLAR") {
        QAction* settingsPolarAction = new QAction(mainWin->load_icon("polarsettings"), "&Settings...", &menu);
        connect(settingsPolarAction, SIGNAL(triggered()), this, SLOT(settingsPolar()));
        menu.addAction(settingsPolarAction);
    }
    else if (objectName() == "StatusBarButtonQSNAP") {
        QAction* settingsQSnapAction = new QAction(mainWin->load_icon("qsnapsettings"), "&Settings...", &menu);
        connect(settingsQSnapAction, SIGNAL(triggered()), this, SLOT(settingsQSnap()));
        menu.addAction(settingsQSnapAction);
    }
    else if (objectName() == "StatusBarButtonQTRACK") {
        QAction* settingsQTrackAction = new QAction(mainWin->load_icon("qtracksettings"), "&Settings...", &menu);
        connect(settingsQTrackAction, SIGNAL(triggered()), this, SLOT(settingsQTrack()));
        menu.addAction(settingsQTrackAction);
    }
    else if (objectName() == "StatusBarButtonLWT") {
        View* gview = mainWin->activeView();
        if (gview) {
            QAction* enableRealAction = new QAction(mainWin->load_icon("realrender"), "&RealRender On", &menu);
            enableRealAction->setEnabled(!gview->isRealEnabled());
            connect(enableRealAction, SIGNAL(triggered()), this, SLOT(enableReal()));
            menu.addAction(enableRealAction);

            QAction* disableRealAction = new QAction(mainWin->load_icon("realrender"), "&RealRender Off", &menu);
            disableRealAction->setEnabled(gview->isRealEnabled());
            connect(disableRealAction, SIGNAL(triggered()), this, SLOT(disableReal()));
            menu.addAction(disableRealAction);
        }

        QAction* settingsLwtAction = new QAction(mainWin->load_icon("lineweightsettings"), "&Settings...", &menu);
        connect(settingsLwtAction, SIGNAL(triggered()), this, SLOT(settingsLwt()));
        menu.addAction(settingsLwtAction);
    }
    menu.exec(event->globalPos());
    statusbar->clearMessage();
}

void StatusBarButton::settingsSnap()
{
    mainWin->settingsDialog("Snap");
}

void StatusBarButton::settingsGrid()
{
    mainWin->settingsDialog("Grid/Ruler");
}

void StatusBarButton::settingsRuler()
{
    mainWin->settingsDialog("Grid/Ruler");
}

void StatusBarButton::settingsOrtho()
{
    mainWin->settingsDialog("Ortho/Polar");
}

void StatusBarButton::settingsPolar()
{
    mainWin->settingsDialog("Ortho/Polar");
}

void StatusBarButton::settingsQSnap()
{
    mainWin->settingsDialog("QuickSnap");
}

void StatusBarButton::settingsQTrack()
{
    mainWin->settingsDialog("QuickTrack");
}

void StatusBarButton::settingsLwt()
{
    mainWin->settingsDialog("LineWeight");
}
*/

std::string platform_string(void)
{
    std::string os = "Unknown plaform.";
    #if _WIN32
    os = "Windows";
    #endif
    #ifdef __linux__
    struct utsname buffer;
    uname(&buffer);
    os = std::string(buffer.sysname);
    os += " " + std::string(buffer.nodename);
    os += " " + std::string(buffer.release);
    os += " " + std::string(buffer.version);
    os += " " + std::string(buffer.machine);
    #endif
    return os;
}
