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

#include "gui_backend.h"

#include <vector>
#include <unordered_map>
#include <string>
#include <filesystem>

ImFont *font;
ImFont *header_font;
std::unordered_map<std::string, GLuint> textures;

/* filesystem homePath equivalent? */
/*
std::string settingsDir = QDir::homePath() + "/.embroidermodder2/";
*/

inline ImVec2
to_ImVec2(EmbVector v)
{
    return ImVec2(v.x, v.y);
}

/**
 * @brief Wrapper for ImGui's IM_COL32 macro.
 *
 * This function doesn't deal with opacity. 
 *
 * @param c The color as an EmbColor.
 * @return unsigned int  The color encoded as an integer.
 */
unsigned int
embColor_to_int(EmbColor c, int alpha)
{
    return IM_COL32(c.r, c.g, c.b, alpha);
}

/**
 * @brief 
 * 
 * @param start 
 * @param end 
 * @param color 
 */
void
draw_line(EmbVector start, EmbVector end, int color)
{
    ImDrawList *draw_list = ImGui::GetWindowDrawList();;
    draw_list->AddLine(ImVec2(start.x, start.y), ImVec2(end.x, end.y), color);
}

/**
 * @brief 
 * 
 * @param position 
 * @param radius 
 * @param color 
 * @param filled 
 */
void
draw_circle(EmbVector position, float radius, int color, bool filled)
{
    ImDrawList *draw_list = ImGui::GetWindowDrawList();
    if (filled) {
        draw_list->AddCircleFilled(ImVec2(position.x, position.y), radius, color);
    }
    else {
        draw_list->AddCircle(ImVec2(position.x, position.y), radius, color);
    }
}

/**
 * @brief 
 * 
 * @param bottom_left 
 * @param top_right 
 * @param color 
 * @param filled 
 */
void
draw_rect(EmbVector bottom_left, EmbVector top_right, int color, bool filled)
{
    ImDrawList *draw_list = ImGui::GetWindowDrawList();
    if (filled) {
        draw_list->AddRectFilled(
            ImVec2(bottom_left.x, bottom_left.y),
            ImVec2(top_right.x, top_right.y),
            color);
    }
    else {
        draw_list->AddRect(
            ImVec2(bottom_left.x, bottom_left.y),
            ImVec2(top_right.x, top_right.y),
            color);
    }
}

/**
 * @brief 
 * 
 * @param position 
 * @param color 
 * @param text 
 */
void
draw_text(EmbVector position, int color, const char *text)
{
    ImDrawList *draw_list = ImGui::GetWindowDrawList();
    draw_list->AddText(to_ImVec2(position), color, text);
}

/**
 * @brief 
 * 
 * @param c1 
 * @param c2 
 * @param c3 
 * @param c4 
 * @param color 
 */
void
draw_quad(EmbVector c1, EmbVector c2, EmbVector c3, EmbVector c4, int color)
{
    ImDrawList *draw_list = ImGui::GetWindowDrawList();
    draw_list->AddQuadFilled(
        to_ImVec2(c1),
        to_ImVec2(c2),
        to_ImVec2(c3),
        to_ImVec2(c4),
        color);
}

/**
 * @brief 
 * 
 * @param label 
 * @param height 
 * @param width 
 */
void
begin_ui(const char *label, int height, int width)
{
    ImGui::SetNextWindowSize(ImVec2(height, width));
    ImGui::Begin(label);
    ImGui::SetWindowFontScale(1.5);
}

/**
 * @brief 
 * 
 */
void
end_ui(void)
{
    ImGui::End();
}

/**
 * @brief 
 * 
 * @param label 
 * @return true 
 * @return false 
 */
bool
begin_tab_bar(const char *label)
{
    return ImGui::BeginTabBar(label);
}

/**
 * @brief 
 * 
 */
void
end_tab_bar(void)
{
    ImGui::EndTabBar();
}

/**
 * @brief 
 * 
 * @param label 
 * @param data 
 * @return true 
 * @return false 
 */
bool
begin_combo(const char *label, char *data)
{
    return ImGui::BeginCombo(label, data);
}

/**
 * @brief 
 * 
 * @param label 
 * @param current 
 * @return true 
 * @return false 
 */
bool
selectable(const char *label, bool current)
{
    return ImGui::Selectable(label, current);
}

/**
 * @brief 
 * 
 */
void
end_combo(void)
{
    ImGui::EndCombo();
}

/**
 * @brief 
 * 
 * @param label 
 * @return true 
 * @return false 
 */
bool
collapsing_header(const char *label)
{
    return ImGui::CollapsingHeader(label);
}

/**
 * @brief 
 * 
 * @param label 
 * @return true 
 * @return false 
 */
bool
button(const char *label)
{
    return ImGui::Button(translate(label));
}

/**
 * @brief 
 * 
 * @param label 
 * @return true 
 * @return false 
 */
bool
begin_tab_item(const char *label)
{
    return ImGui::BeginTabItem(label);
}

/**
 * @brief 
 * 
 */
void
end_tab_item(void)
{
    ImGui::EndTabItem();
}

/**
 * @brief 
 * 
 * @param label 
 * @param data 
 */
void
edit_int(const char *label, int *data)
{
    ImGui::InputInt(label, data, *data);
}

/* Need validation for the input number before applying to object.
 * and to add to undo history
 */
void
show_real(const char *label, EmbReal data)
{
    ImGui::Text(label);
    ImGui::SameLine();
    char s[200];
    sprintf(s, "%f", data);
    ImGui::Text(s);
}

/**
 * @brief 
 * 
 * @param label 
 * @param data 
 */
void
show_int(const char *label, int data)
{
    ImGui::Text(label);
    ImGui::SameLine();
    char s[200];
    sprintf(s, "%d", data);
    ImGui::Text(s);
}

/**
 * @brief 
 * 
 * @param str 
 */
void
translated_text(const char *str)
{
    ImGui::Text(translate(str));
}

/**
 * @brief 
 * 
 * @param label 
 * @param stor 
 * @param value 
 */
void
checkbox_data(const char *label, bool *stor, bool *value)
{
    *stor = *value;
    ImGui::Checkbox(translate(label), stor);
    *value = *stor;
}

/**
 * @brief 
 * 
 * @param label 
 * @param data 
 */
void
edit_real(const char *label, EmbReal *data)
{
    ImGui::InputFloat(translate(label), data, *data);
}

/**
 * @brief 
 * 
 */
void
same_line(void)
{
    ImGui::SameLine();
}

/**
 * @brief 
 * 
 * @param label 
 */
void
begin_child(const char *label)
{
    ImGui::BeginChild(label);
}

/**
 * @brief 
 * 
 */
void
end_child()
{
    ImGui::EndChild();
}

/**
 * @brief Get the window pos object
 * 
 * @return EmbVector 
 */
EmbVector
get_window_pos(void)
{
    EmbVector em_offset;
    ImVec2 offset = ImGui::GetWindowPos();
    em_offset.x = offset.x;
    em_offset.y = offset.y;
    return em_offset;
}

/**
 * @brief Get the screen size object
 * 
 * @return EmbVector 
 */
EmbVector
get_screen_size(void)
{
    EmbVector screen_size;
    ImGuiIO &io = ImGui::GetIO();
    screen_size.x = io.DisplaySize.x;
    screen_size.y = io.DisplaySize.y;
    return screen_size;
}

/**
 * @brief Get the display size object
 * 
 * @param width 
 * @param height 
 */
void
get_display_size(int *width, int *height)
{
    ImGuiIO& io = ImGui::GetIO();
    *width = io.DisplaySize.x;
    *height = io.DisplaySize.y;
}

/**
 * @brief Set the style object
 * 
 */
void
set_style(char font_file[200])
{
    ImGuiIO& io = ImGui::GetIO();
    ImFontConfig config;
    config.OversampleH = 2;
    config.OversampleV = 2;
    FILE *f = fopen(font_file, "r");
    if (f) {
        fclose(f);
        font = io.Fonts->AddFontFromFileTTF(font_file, 16, &config);
        header_font = io.Fonts->AddFontFromFileTTF(font_file, 24, &config);
    }
    else {
        log_debug("ERROR: Failed to load font file.");
    }

    ImGuiStyle& style = ImGui::GetStyle();
    style.Colors[ImGuiCol_Text] = ImVec4(0.00f, 0.00f, 0.00f, 1.00f);
    style.Colors[ImGuiCol_FrameBgActive] = ImVec4(0.75f, 0.75f, 0.75f, 1.00f);
    style.Colors[ImGuiCol_PopupBg] = ImVec4(0.85f, 0.85f, 0.85f, 1.00f);
    style.Colors[ImGuiCol_WindowBg] = ImVec4(0.65f, 0.65f, 0.65f, 1.00f);
    style.Colors[ImGuiCol_MenuBarBg] = ImVec4(1.0f, 1.0f, 1.0f, 1.00f);
    style.Colors[ImGuiCol_ChildBg] = ImVec4(1.0f, 1.0f, 1.0f, 1.00f);
}

/**
 * @brief
 */
void
columns(int n, char *label)
{
    ImGui::Columns(n, label);
}

/**
 * @brief
 */
void
next_column(void)
{
    ImGui::NextColumn();
}

/**
 * @brief
 */
void
end_columns(void)
{
    ImGui::Columns();
}

/**
 * @brief
 */
void
display_text(const char *msg)
{
    ImGui::Text(msg);
}

/**
 * @brief
 */
void
get_current_directory(char directory[200])
{
    strcpy(directory, std::filesystem::current_path().string().c_str());
}

/**
 * @brief
 */
bool
menu_item(const char *key)
{
    return ImGui::MenuItem(key);
}

/**
 * @brief 
 * 
 * @param texture_list 
 * @return int 
 */
int
load_textures(Dictionary *texture_list)
{
    for (int i=0; i<texture_list->length; i++) {
        DictionaryEntry d = texture_list->data[i];
        textures[d.key] = load_texture(d.value);
        if (!textures[d.key]) {
            char error[200];
            sprintf(error, "ERROR: Failed to open %s.", d.value);
            log_debug(error);
            /* return 0; */
        }
    }

    return 1;
}

/**
 * @brief
 */
void
menu_separator(void)
{
    ImGui::Separator();
}

/**
 * @brief
 */
bool
begin_menu(const char *label)
{
    return ImGui::BeginMenu(label);
}

/**
 * @brief
 */
void
end_menu()
{
    ImGui::EndMenu();
}

/**
 * @brief 
 *
 */
bool
image_button(char* texture_label, EmbVector size)
{
    ImVec2 size_ = {size.x, size.y};
    std::string label(texture_label);
    return ImGui::ImageButton((void*)(intptr_t)textures[label], size_);
}

void
header(const char *title)
{
    ImGui::PushFont(header_font);
    translated_text(title);
    ImGui::PopFont();    
}

void
set_column_width(int start, int end)
{
    ImGui::SetColumnWidth(start, end);
}

/**
 *
 */
bool
begin_menu_bar(void)
{
    return ImGui::BeginMenuBar();
}

/**
 *
 */
void
end_menu_bar(void)
{
    ImGui::EndMenuBar();
}

/**
 */
void
setup_main_widget(void)
{
	ImGuiViewport* viewport = ImGui::GetMainViewport();
    ImGui::SetNextWindowPos(ImVec2(0, 0));
    ImGui::SetNextWindowSize(viewport->Size);
    ImGui::Begin("Embroidermodder", NULL, ImGuiWindowFlags_MenuBar
        | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoTitleBar
        | ImGuiWindowFlags_NoMove );
    ImGui::SetWindowFontScale(1.5);
}

/**
 */
void
setup_imgui(GLFWwindow *window)
{
    int width, height;
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();

    ImGui_ImplGlfw_InitForOpenGL(window, true);
    ImGui_ImplOpenGL2_Init();

    glfwGetFramebufferSize(window, &width, &height);
    ImVec2 v = {1.0f*width, 1.0f*height};
    ImGui::SetNextWindowPos({0.0, 0.0});
    ImGui::SetNextWindowSize(v);
}

/**
 */
void
imgui_render(GLFWwindow *window)
{
    int width, height;
    ImGui::Render();

    glfwGetFramebufferSize(window, &width, &height);
    glViewport(0, 0, width, height);
    glClearColor(0.1, 0.1, 0.1, 0.0);
    glClear(GL_COLOR_BUFFER_BIT);

    ImGui_ImplOpenGL2_RenderDrawData(ImGui::GetDrawData());
}

/**
 */
void
imgui_new_frame(void)
{
    ImGui_ImplOpenGL2_NewFrame();
    ImGui_ImplGlfw_NewFrame();
    ImGui::NewFrame();
}
