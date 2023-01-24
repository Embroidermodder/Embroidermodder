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
#define STB_IMAGE_IMPLEMENTATION
#include "stb/stb_image.h"

#include <iostream>

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <toml.hpp>
#include <embroidery.h>

#define VERSION "2.0.0-alpha"

typedef struct Icon_ {
    std::string fname;
    std::string command;
    GLuint texture_id;
} Icon;

static int icon_size = 16;
static ImFont *font;
static bool running = true;
static GLuint circle_ = 0;
static std::vector<EmbPattern*> pattern_list;
static std::vector<Icon> icon_list;
static std::string menu_action = "";
static std::string current_pattern = "";
static TextEditor editor;

std::vector<std::vector<std::string>> file_menu_layout = {
    {"New", "new"},
    {"Open", "open"},
    {"---", "---"},
    {"Save", "save"},
    {"Save As...", "saveas"},
    {"Export", "export"},
    {"---", "---"},
    {"Close", "close"},
    {"---", "---"},
    {"Print", "print"},
    {"---", "---"},
    {"Quit", "quit"}
};
std::vector<std::vector<std::string>> edit_menu_layout = {
    {"Undo", "undo"},
    {"Redo", "redo"},
    {"---", "---"},
    {"Cut", "cut"},
    {"Copy", "copy"},
    {"Paste", "paste"}
};
std::vector<std::vector<std::string>> view_menu_layout = {
    {"Zoom Realtime", "zoom realtime"},
    {"---", "---"},
    {"Zoom In", "zoom in"},
    {"Zoom Out", "zoom out"},
    {"Zoom Extents", "zoom extents"},
    {"---", "---"},
    {"Pan Left", "pan left"},
    {"Pan Right", "pan right"},
    {"Pan Up", "pan up"},
    {"Pan Down", "pan down"},
    {"---", "---"},
    {"Day", "day"},
    {"Night", "night"}
};
std::vector<std::vector<std::string>> draw_menu_layout = {
    {"Circle", "circle"},
    {"Ellipse", "ellipse"},
    {"Rectangle", "rectangle"},
    {"Polyline", "polyline"},
    {"Polygon", "polygon"},
};

void set_style(void);
void load_configuration(void);
void actuator(std::string command);
void load_menu(std::string menu_label, std::vector<std::vector<std::string>> menu_layout);
void load_toolbar(std::string menu_label, std::vector<std::vector<std::string>> toolbar_layout);

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
    glBindTexture(GL_TEXTURE_2D, 0);

    return texture;
}

/* Description is a a simple string table of path instructions,
 * 
 */
int
render_vector_graphic(std::vector<std::string> description)
{
    return 0;
}

GLuint
load_texture(const char *fname)
{
    int width, height;
    uint8_t* data = stbi_load(fname, &width, &height, NULL, 4);
    if (!data) {
        return 0;
    }

    GLuint texture_id = gen_gl_texture(data, width, height, 0);
    stbi_image_free(data);

    return texture_id;
}

void
set_style(void)
{
    ImGuiIO& io = ImGui::GetIO();
    font = io.Fonts->AddFontFromFileTTF("fonts/SourceSans3-regular.ttf", 16);

    ImGuiStyle& style = ImGui::GetStyle();
    style.Colors[ImGuiCol_Text] = ImVec4(0.00f, 0.00f, 0.00f, 1.00f);
    style.Colors[ImGuiCol_FrameBgActive] = ImVec4(0.15f, 0.15f, 0.15f, 1.00f);
    style.Colors[ImGuiCol_PopupBg] = ImVec4(0.55f, 0.55f, 0.55f, 1.00f);
    style.Colors[ImGuiCol_WindowBg] = ImVec4(0.65f, 0.65f, 0.65f, 1.00f);
    style.Colors[ImGuiCol_MenuBarBg] = ImVec4(1.0f, 1.0f, 1.0f, 1.00f);
}

void
load_configuration(void)
{
    auto config = toml::parse("imgui_config.toml");

    for (const auto &[i, j] : config.as_table()) {
        if (!config[i].is_table()) {
            continue;
        }
        if (!config[i].contains("type")) {
            continue;
        }
        std::string s = toml::get<std::string>(config[i]["type"]);
        if (s == "top-menu") {
            // If the menu doesn't exist, create it.
            std::string s_label = toml::get<std::string>(config[i]["label"]);
        }
    }
    for (const auto &[i, j] : config.as_table()) {
        if (!config[i].is_table()) {
            continue;
        }
        if (!config[i].contains("type")) {
            continue;
        }
        std::string s = toml::get<std::string>(config[i]["type"]);
        if (s == "menu-item") {
        }
    }
}

void
actuator(std::string command)
{
    if (command == "new") {
        std::cout << "New File" << std::endl;
    }
    if (command == "open") {
        std::cout << "Open File" << std::endl;
    }
    if (command == "quit") {
        running = false;
    }
}

void
load_menu(std::string menu_label, std::vector<std::vector<std::string>> menu_layout)
{
    if (ImGui::BeginMenu(menu_label.c_str())) {
        for (auto i : menu_layout) {
            if (i[0] == "---") {
                ImGui::Separator();
            }
            else {
                if (ImGui::MenuItem(i[0].c_str())) {
                    menu_action = i[1];
                }
            }
        }
        ImGui::EndMenu();
    }
}

void
load_toolbar(std::string menu_label, std::vector<std::vector<std::string>> toolbar_layout)
{
    for (auto i : toolbar_layout) {
        if (i[0] == "---") {
            ImGui::Separator();
        }
        else {
            if (ImGui::Button(i[0].c_str())) {
                menu_action = i[1];
            }
        }
    }
}

void
pattern_view(int index)
{

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

    ImGui::Text("Example");
    menu_action = "";

    ImVec2 size = {50, 50};
	if (ImGui::ImageButton((void*)(intptr_t)circle_, size)) {
        menu_action = "circle";
    }

    if (ImGui::BeginMenuBar()) {
        load_menu("File", file_menu_layout);
        load_menu("Edit", edit_menu_layout);
        load_menu("View", view_menu_layout);
        load_menu("Draw", draw_menu_layout);
        ImGui::EndMenuBar();
    }
    if (menu_action != "") {
        actuator(menu_action);
    }

    editor.Render("Text Editor");

    if (pattern_list.size() > 0) {
        if (ImGui::Button("Close")) {
            std::cout << "Close" << std::endl;
        }
    }

    ImGui::End();
}

int
main(int argc, char* argv[])
{
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

    std::string fname = "assets/icons/default/circle.png";
    circle_ = load_texture(fname.c_str());

    std::string line;
    std::ifstream file;
    file.open("imgui_config.toml");
    while (std::getline(file, line)) {
        editor.InsertText(line);
        editor.InsertText("\n");
    }

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
