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

#include "embroidermodder_imgui.h"

#include <iostream>
#include <fstream>

#include <GLFW/glfw3.h>
#include <embroidery.h>

bool running = true;
bool debug_mode = true;
bool show_about_dialog = false;
bool show_editor = false;
int icon_size = 16;
ImFont *font;
GLuint circle_ = 0;
int pattern_index = 0;
int n_patterns = 0;
std::string current_fname = "Untitled.dst";
std::string assets_dir = "../assets/";
std::vector<Action> action_list;
std::vector<EmbPattern*> pattern_list;
std::unordered_map<std::string, string_matrix> menu_layout;
std::vector<Icon> icon_list;
std::string menu_action = "";
std::string current_pattern = "";
TextEditor editor;

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
    std::string font_file = assets_dir + "fonts/SourceSans3-regular.ttf";
    ImGuiIO& io = ImGui::GetIO();
    font = io.Fonts->AddFontFromFileTTF(font_file.c_str(), 16);

    ImGuiStyle& style = ImGui::GetStyle();
    style.Colors[ImGuiCol_Text] = ImVec4(0.00f, 0.00f, 0.00f, 1.00f);
    style.Colors[ImGuiCol_FrameBgActive] = ImVec4(0.15f, 0.15f, 0.15f, 1.00f);
    style.Colors[ImGuiCol_PopupBg] = ImVec4(0.55f, 0.55f, 0.55f, 1.00f);
    style.Colors[ImGuiCol_WindowBg] = ImVec4(0.65f, 0.65f, 0.65f, 1.00f);
    style.Colors[ImGuiCol_MenuBarBg] = ImVec4(1.0f, 1.0f, 1.0f, 1.00f);
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
load_toolbar(std::string toolbar_label)
{
    for (auto i : menu_layout.at(toolbar_label)) {
        if (i[0] == "---") {
            ImGui::Separator();
            continue;
        }
        ImGui::Button(i[0].c_str());
        ImGui::SameLine();
        ImVec2 size = {50, 50};
    	if (ImGui::ImageButton((void*)(intptr_t)circle_, size)) {
            menu_action = "circle";
        }
        /* menu_action = i[1]; */
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

    ImGui::Text("Example");
    menu_action = "";

    if (ImGui::BeginMenuBar()) {
        load_menu("File");
        load_menu("Edit");
        load_menu("View");
        load_menu("Draw");
        ImGui::EndMenuBar();
    }
    if (menu_action != "") {
        actuator(menu_action);
    }

    if (show_about_dialog) {
        about_dialog();
    }

    /* load_toolbar(); */

    if (show_editor) {
        editor.Render("Text Editor");
    }

    if (pattern_list.size() > 0) {
        if (ImGui::Button("Close")) {
            std::cout << "Close" << std::endl;
        }
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
        assets_dir = std::string(argv[1]);
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

    std::string fname = assets_dir + "icons/default/circle.png";
    circle_ = load_texture(fname.c_str());

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
