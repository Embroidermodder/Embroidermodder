/**
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
 *
 * \file main.c
 * \brief As much of the code as possible, to keep the structure
 * simple and in a procedural style.
 */

#include "embroidermodder.h"

/**
 * The actuator changes the program state via these global variables.
 */
Settings settings = {
    .version = "2.0.0-alpha",
    .running = true,
    .testing = false,
    .debug_mode = 1,
    .show_about_dialog = false,
    .show_settings_editor = false,
    .show_editor = false,
    .show_details_dialog = false,
    .show_open_file_dialog = false,
    .icon_size = 16,
    .icon_theme = "default",
    .pattern_index = 0,
    .assets_dir = "assets/",
    .use_translation = false,
    .language = "default",
    .mdi_bg_use_logo = true,
    .mdi_bg_use_texture = true,
    .mdi_bg_use_color = true,
    .general_mdi_bg_logo = "./images/logo-spirals.png",
    .general_mdi_bg_texture = "./images/texture-spirals.png",
    .general_mdi_bg_color = 0xAAAAAAFF,
    .tip_of_the_day = true,
    .general_current_tip = 0,
    .general_system_help_browser = true,
    .display_use_opengl = false,
    .display_renderhint_aa = false,
    .display_renderhint_text_aa = false,
    .display_renderhint_smooth_pix = false,
    .display_renderhint_high_aa = false,
    .display_renderhint_noncosmetic = false,
    .display_show_scrollbars = true,
    .display_scrollbar_widget_num = 0,
    .display_crosshair_color = 0x000000FF,
    .display_bg_color = 0xEEEEEEFF,
    .display_selectbox_left_color = 0x008000FF,
    .display_selectbox_left_fill = 0x00FF00FF,
    .display_selectbox_right_color = 0x000080FF,
    .display_selectbox_right_fill = 0x0000FFFF,
    .display_selectbox_alpha = 32,
    .display_zoomscale_in = 2.0,
    .display_zoomscale_out = 0.5,
    .display_crosshair_percent = 5,
    .display_units = "mm",
    .opensave_custom_filter = "supported",
    .opensave_open_format = "*.*",
    .opensave_open_thumbnail = false,
    .opensave_save_format = "*.*",
    .opensave_save_thumbnail = false,
    .opensave_recent_max_files = 10,
    // .opensave_recent_list_of_files = {},
    .opensave_recent_directory = "./samples",
    .opensave_trim_dst_num_jumps = 5,
    .printing_default_device = "",
    .printing_use_last_device = false,
    .printing_disable_bg = true,
    .grid_show_on_load = true,
    .grid_show_origin = true,
    .grid_color_match_crosshair = true,
    .grid_color = 0x000000FF,
    .grid_load_from_file = true,
    .grid_type = "Rectangular",
    .grid_center_on_origin = true,
    .grid_center = {.x=0.0, .y=0.0},
    .grid_size_x = 100.0,
    .grid_size_y = 100.0,
    .grid_spacing_x = 25.0,
    .grid_spacing_y = 25.0,
    .grid_size_radius = 50.0,
    .grid_spacing_radius = 25.0,
    .grid_spacing_angle = 45.0,
    .ruler_show_on_load = true,
    .ruler_metric = true,
    .ruler_color = 0xAAAA55FF, //qRgb(210,210, 50);
    .ruler_pixel_size = 20,
    .qsnap_enabled = true,
    .qsnap_locator_color = 0xFFFF00FF,
    .qsnap_locator_size = 4,
    .qsnap_aperture_size = 10,
    .qsnap_endpoint = true,
    .qsnap_midpoint = true,
    .qsnap_center = true,
    .qsnap_node = true,
    .qsnap_quadrant = true,
    .qsnap_intersection = true,
    .qsnap_extension = true,
    .qsnap_insertion = false,
    .qsnap_perpendicular = true,
    .qsnap_tangent = true,
    .qsnap_nearest = false,
    .qsnap_apparent = false,
    .qsnap_parallel = false,
    .lwt_show_lwt = false,
    .lwt_real_render = true,
    .lwt_default_lwt = 0,
    .selection_mode_pickfirst = true,
    .selection_mode_pickadd = true,
    .selection_mode_pickdrag = false,
    .selection_coolgrip_color = 0x0000FFFF,
    .selection_hotgrip_color = 0xFF0000FF,
    .selection_grip_size = 4,
    .selection_pickbox_size = 4,
    .text_font  = "Arial",
    .text_size = 12,
    .text_angle = 0,
    .text_style_bold = false,
    .text_style_italic = false,
    .text_style_underline = false,
    .text_style_overline = false,
    .text_style_strikeout = false,
    .zoomInLimit = 1.0e-10,
    .zoomOutLimit = 1.0e10,
    .grid_spacing = {40.0, 40.0},
    .ruler_width = 50.0f,
    .tick_depth = 30.0f,
    .major_tick_seperation = 10.0f,
    .needle_speed = 100.0,
    .stitch_time = 0.1
};

Index *menu_layout; /*< */
Index *toolbar_layout;  /*< */
EmbView views[50]; /*< */
int n_views = 0; /*< */
char menu_action[200]; /*< */
char current_directory[200] = "/"; /*< */
char to_open[200] = ""; /*< This char array is for the open file dialog to store what  */

/**
 * The directory on the user's system to keep our configuration in:
 * this will be some variant of "~/.embroidermodder2".
 */
char settings_dir[200];

/**
 * This file needs to be read from the users home directory to ensure it is writable.
 */
char settings_file[200];

/**
 * These copies of the settings struct are for restoring the state if
 * the user doesn't want to accept their changes in the settings dialog.
 */
Settings dialog, preview;

/**
 * \todo Move to the settings struct.
 */
bool just_opened = true;

Dictionary *translation_table;

/**
 * The view focussed (that is the last view to have a click or keypress sent):
 * this has to be manually set whenever it changes including being set to NULL when
 * all views are closed.
 */
EmbView *active_view = NULL;

/**
 * @brief Generate a random number in the range (0.0, 1.0).
 *
 * \todo move to libembroidery.
 *
 * Returns EmbReal A randomly generated real number.
 */
EmbReal
random_uniform(void)
{
    int r = rand() % 100000;
    return 0.00001*r;
}

/**
 * @brief Rounds integers to multiples of another given integer.
 *
 * This is used by the rulers to scale appropriately.
 *
 * \a roundUp Whether to round up or down: 1 if up, 0 if down.
 * \a numToRound Input to be rounded.
 * \a multiple The number which must be a factor of the result.
 * Returns int The multiple which is the closest to numToRound.
 */
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

/**
 * @brief Check whether an subtraction will cause underflow before we rely on the result.
 *
 * \a a The first argument to the subtraction.
 * \a b The second argument to the subtraction.
 * Returns true If underflow should occur.
 * Returns false If underflow won't occur.
 */
bool
willUnderflowInt32(int32_t a, int32_t b)
{
    assert(LLONG_MAX>INT_MAX);
    int64_t c = (int64_t)a-b;
    return c < INT_MIN || c > INT_MAX;
}

/**
 * @brief Check whether an addition will cause overflow before we rely on the result.
 *
 * \a a The first argument to the addition.
 * \a b The second argument to the addition.
 * Returns true If overflow should occur.
 * Returns false If overflow won't occur.
 */
bool
willOverflowInt32(int32_t a, int32_t b)
{
    assert(LLONG_MAX>INT_MAX);
    int64_t c = (int64_t)a+b;
    return c < INT_MIN || c > INT_MAX;
}

/**
 * @brief Check if the filename is valid before we attempt to read or write it.
 *
 * \a fname The file name to check (absolute or relative).
 * Returns true If this file can be parsed by libembroidery.
 * Returns false If it cannot be, or no file name appears to be present.
 */
bool
valid_file_format(char *fname)
{
    if (fname[0] == 0) {
        return false;
    }
    if (emb_identify_format(fname) >= 0) {
        return true;
    }
    return false;
}

void
c_split(char input[200], int *argc, char argv[10][200])
{
    *argc = 0;
    char buffer[200];
    strcpy(buffer, input);
    char *p = strtok(buffer, " ");
    for (int i=0; i<10; i++) {
        strcpy(argv[*argc], p);
        (*argc)++;
        p = strtok(NULL, " ");
        if (p == NULL) {
            break;
        }
    }
}

/**
 * @brief Ensure that x lies in the range [\a lower, \a upper] by rounding up
 * or down if x is outside of that range.
 *
 * \a lower The minimum permittable value \a x can take.
 * \a x The value to be processed.
 * \a upper The maximum permittable value \a x can take.
 * Returns EmbReal The clamped value.
 *
 * \todo Move to libembroidery.
 */
EmbReal
emb_clamp(EmbReal lower, EmbReal x, EmbReal upper)
{
    if (lower > x) {
        return lower;
    }
    if (upper < x) {
        return upper;
    }
    return x;
}

/**
 * @brief Simplifies a path by removing the .. and . symbols in place.
 *
 * \a path The character array to operate on.
 */
void
simplify_path(char *path)
{
    char storage[200];
    strcpy(storage, path);
    char *separator = "/";
    #if _WIN32
    separator = "\\";
    #endif
    char tokens_to_use[30][100];
    int n_tokens = 0;
    char *token = strtok(storage, separator);
    while (token != NULL) {
        if (streq(token, "..")) {
            n_tokens--;
        }
        if (strcmp(token, "..") && strcmp(token, ".") && strcmp(token, "")) {
            strcpy(tokens_to_use[n_tokens], token);
            n_tokens++;
        }
        token = strtok(NULL, separator);
    }

    #if _WIN32
    strcpy(path, "");
    #else
    strcpy(path, "/");
    #endif

    for (int i=0; i<n_tokens; i++) {
        strcat(path, tokens_to_use[i]);
        if (i < n_tokens-1) {
            strcat(path, separator);
        }
    }
}

/**
 * \brief Read the settings from file which aren't editable by the user.
 * These files need to be placed in the install folder.
 */
int
read_settings(const char *settings_file)
{
    char error_buffer[200];
    FILE *f;
    f = fopen(settings_file, "r");
    if (!f) {
        puts("ERROR: Failed to open settings file:");
        printf("%s", settings_file);
        return 0;
    }
    toml_table_t *setting_toml = toml_parse_file(f, error_buffer, sizeof(error_buffer));
    fclose(f);

    if (!setting_toml) {
        puts("ERROR: failed to parse settings.toml, continuing with defaults.");
        return 0;
    }

    return 1;
}

#if 0

#ifdef __linux__
#include <sys/utsname.h>
#endif

#include <tinydir.h>

#include "toml.h"

#define STB_IMAGE_IMPLEMENTATION
#include "stb/stb_image.h"

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include "imgui.h"
#include "backends/imgui_impl_glfw.h"
#include "backends/imgui_impl_opengl2.h"

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

void
draw_line(EmbVector start, EmbVector end, int color)
{
    ImDrawList *draw_list = ImGui::GetWindowDrawList();;
    draw_list->AddLine(ImVec2(start.x, start.y), ImVec2(end.x, end.y), color);
}

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

void
draw_text(EmbVector position, int color, const char *text)
{
    ImDrawList *draw_list = ImGui::GetWindowDrawList();
    draw_list->AddText(to_ImVec2(position), color, text);
}

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

void
begin_ui(const char *label, int height, int width)
{
    ImGui::SetNextWindowSize(ImVec2(height, width));
    ImGui::Begin(label);
    ImGui::SetWindowFontScale(1.5);
}

bool
button(const char *label)
{
    return ImGui::Button(translate(label));
}

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

void
show_int(const char *label, int data)
{
    ImGui::Text(label);
    ImGui::SameLine();
    char s[200];
    sprintf(s, "%d", data);
    ImGui::Text(s);
}

void
translated_text(const char *str)
{
    ImGui::Text(translate(str));
}

void
checkbox_data(const char *label, bool *stor, bool *value)
{
    *stor = *value;
    ImGui::Checkbox(translate(label), stor);
    *value = *stor;
}

void
edit_real(const char *label, EmbReal *data)
{
    ImGui::InputFloat(translate(label), data, *data);
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

void
get_current_directory(char directory[200])
{
    strcpy(directory, std::filesystem::current_path().string().c_str());
}

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
imgui_render(GLFWwindow *window)
{
    int width, height;
    ImGui::Render();

    glfwGetFramebufferSize(window, &width, &height);
    glEmbViewport(0, 0, width, height);
    glClearColor(0.1, 0.1, 0.1, 0.0);
    glClear(GL_COLOR_BUFFER_BIT);

    ImGui_ImplOpenGL2_RenderDrawData(ImGui::GetDrawData());
}

/**
 * @brief The program entry point on all platforms.
 *
 * \a argc The number of command line arguments sent (including the program executable).
 * \a argv An array of char arrays for each of the command line arguments.
 * Returns 0 If the program closed without fault.
 * Returns 1 Otherwise
 *
 * \todo detail all possible error return values here.
 */
int
main(int argc, char *argv[])
{
    char command[200];
    strcpy(command, "open");
    parse_command(argc, argv, command);
    if (!settings.running) {
        return 0;
    }

    strcpy(settings_file, "./settings.toml");
    read_settings(settings_file);
    load_configuration();

    if (strcmp(command, "open")) {
        actuator(command);
    }

    EmbColor ruler_color_ = {150, 170, 0};
    EmbColor black_color_ = {0, 0, 0};
    EmbColor white_color_ = {255, 255, 255};
    settings.ruler_color = embColor_to_int(ruler_color_, 255);
    settings.ticks_color = embColor_to_int(black_color_, 255);
    settings.grid_color = embColor_to_int(black_color_, 255);
    settings.shine_color = embColor_to_int(white_color_, 20);

    int width = 1080;
    int height = 576;
    if (!glfwInit()) {
        log_debug("ERROR: Failed to initialise GLFW.");
        return 2;
    }
    char title[200];
    sprintf(title, "Embroidermodder %s", settings.version);
    GLFWwindow *window = glfwCreateWindow(width, height, title, NULL, NULL);
    if (!window) {
        log_debug("Failed to create GLFW window.");
        glfwTerminate();
        return 1;
    }
    glfwMakeContextCurrent(window);

    char font_file[200];
    sprintf(font_file, "%sfonts/SourceSans3-Regular.ttf", settings.assets_dir);

    int width, height;
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();

    ImGui_ImplGlfw_InitForOpenGL(window, true);
    ImGui_ImplOpenGL2_Init();

    glfwGetFramebufferSize(window, &width, &height);
    ImVec2 v = {1.0f*width, 1.0f*height};
    ImGui::SetNextWindowPos({0.0, 0.0});
    ImGui::SetNextWindowSize(v);

    set_style(font_file);
    load_textures(settings.texture_list);

    while (!glfwWindowShouldClose(window)) {
        glfwPollEvents();

        ImGui_ImplOpenGL2_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();

        ImGuiEmbViewport* viewport = ImGui::GetMainEmbViewport();
        ImGui::SetNextWindowPos(ImVec2(0, 0));
        ImGui::SetNextWindowSize(viewport->Size);
        ImGui::Begin("Embroidermodder", NULL, ImGuiWindowFlags_MenuBar
            | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoTitleBar
            | ImGuiWindowFlags_NoMove );
        ImGui::SetWindowFontScale(1.5);

        strcpy(menu_action, "");

        /** Load all menus in the main window.
         *
         * \todo add icons and shortcut prompts to menus.
             if (ImGui::BeginMenuBar()) {
            load_menu("File");
            load_menu("Edit");
            load_menu("EmbView");
            load_menu("Tools");
            load_menu("Draw");
            load_menu("Window");
            load_menu("Settings");
            load_menu("Help");
            ImGui::EndMenuBar();
        }

        /** Load all toolbars in main window. */
        load_toolbar("File");
        ImGui::SameLine();
        load_toolbar("Edit");
        ImGui::SameLine();
        load_toolbar("Help");
        ImGui::SameLine();
        load_toolbar("Icon");
        ImGui::SameLine();
        load_toolbar("Draw");

        load_toolbar("Zoom");
        ImGui::SameLine();
        load_toolbar("Pan");
        ImGui::SameLine();
        load_toolbar("EmbView");

        load_toolbar("Layer");
        ImGui::SameLine();
        load_toolbar("Properties");

        load_toolbar("Text");

        if (strlen(menu_action)) {
            actuator(menu_action);
        }

        if (n_views > 0) {
            if (ImGui::BeginTabBar("Tab Bar")) {
                for (int i=0; i<n_views; i++) {
                    create_tab(i);
                }
                ImGui::EndTabBar();
            }
        }
        else {
            //EmbVector size = {500, 300};
            //ImGui::Image((void*)(intptr_t)textures["texture-spirals"], size);
        }

        if (settings.show_about_dialog) {
            about_dialog();
        }

        if (settings.show_details_dialog) {
            details_dialog();
        }

        if (settings.show_open_file_dialog) {
            open_file_dialog();
        }

        if (settings.show_settings_editor) {
            settings_editor();
        }

        /** Build statusbar.
         *
         * Possible inputs:
         *     Snap "Snap"
         *     Grid "Grid/Ruler"
         *     Ruler "Grid/Ruler"
         *     Ortho "Ortho/Polar"
         *     Polar "Ortho/Polar"
         *     QSnap "QuickSnap"
         *     QTrack "QuickTrack"
         *     Lwt "LineWeight"
         *
         * void
         * statusbar_button(const char *key)
         * {
         *    settings_dialog(key);
         * }
             //ImGuiIO &io = ImGui_GetIO();
        //ImGui_SetNextWindowSize(EmbVector(50, 100));
        // ImGui_SetNextWindowPos(EmbVector(io.DisplaySize.x, io.DisplaySize.y - 50));
        //ImGui_BeginChild("Statusbar");
        if (button("SNAP")) {
            views[settings.pattern_index].snap_mode = !views[settings.pattern_index].snap_mode;
            /* change button depressed state */
        }
        ImGui::SameLine();
        if (button("GRID")) {
            views[settings.pattern_index].grid_mode = !views[settings.pattern_index].grid_mode;
        }
        ImGui::SameLine();
        if (button("RULER")) {
            views[settings.pattern_index].ruler_mode = !views[settings.pattern_index].ruler_mode;
        }
        ImGui::SameLine();
        if (button("ORTHO")) {
            views[settings.pattern_index].ortho_mode = !views[settings.pattern_index].ortho_mode;
        }
        ImGui::SameLine();
        if (button("POLAR")) {
            views[settings.pattern_index].polar_mode = !views[settings.pattern_index].polar_mode;
        }
        ImGui::SameLine();
        if (button("QSNAP")) {
            views[settings.pattern_index].qsnap_mode = !views[settings.pattern_index].qsnap_mode;
        }
        ImGui::SameLine();
        if (button("QTRACK")) {
            views[settings.pattern_index].qtrack_mode = !views[settings.pattern_index].qtrack_mode;
        }
        ImGui::SameLine();
        if (button("LWT")) {
            views[settings.pattern_index].lwt_mode = !views[settings.pattern_index].lwt_mode;
        }
        ImGui::SameLine();
        translated_text("\todo Mouse co-ordinates here.");

        // statusBarMouseCoord->setMinimumWidth(300); // Must fit this text always
        // statusBarMouseCoord->setMaximumWidth(300); // "+1.2345E+99, +1.2345E+99, +1.2345E+99"
        //ImGui_EndChild();

        ImGui::EndUi();

        imgui_render(window);

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

/**
 * @brief Check that the translate call can return an entry from the table.
 *
 * Returns true If the test has been passed.
 * Returns false Otherwise.
 */
bool
test_translate(void)
{
    const char *translated = translate("?");
    printf("Failed to use translate.");
    return streq(translated, "?");
}


void
undo_history_viewer(void)
{
    char str[200];
    sprintf(str, "Undo History %s", views[settings.pattern_index].filename);
    ImGui::BeginChild(str);
    header("Undo History");
    for (int i=0; i<views[settings.pattern_index].undo_history.length; i++) {
        translated_text(views[settings.pattern_index].undo_history.data[i]);
    }
    ImGui::EndChild();
}

/**
 * @brief Create the i-th view as a tab.
 *
 * \a i Which entry in the views array to use.
 */
void
create_tab(int i)
{
    int width, height;
    get_display_size(&width, &height);
    if (ImGui::BeginTabItem(views[i].filename)) {
        settings.pattern_index = i;
        active_view = &(views[i]);
        char str[200];
        sprintf(str, "EmbView %s", views[settings.pattern_index].filename);
        ImGui::Columns(3, str);
        ImGui::SetColumnWidth(-1, 200);
        undo_history_viewer();
        ImGui::NextColumn();
        ImGui::SetColumnWidth(-1, width - 605);
        pattern_view();
        ImGui::NextColumn();
        ImGui::SetColumnWidth(-1, 400);
        property_editor();
        ImGui::Columns();
        ImGui::EndTabItem();
    }
}

void
load_toolbar(const char *label)
{
    Dictionary *toolbar = dictionary_from_index(toolbar_layout, label);
    for (int i=0; i<toolbar->length; i++) {
        if (streq(toolbar->data[i].key, "---")) {
            ImGui::Separator();
            continue;
        }
        EmbVector size = {1.0f*settings.icon_size, 1.0f*settings.icon_size};
        if (image_button(toolbar->data[i].key, size)) {
            strcpy(menu_action, toolbar->data[i].value);
        }
        if (i+1 < toolbar->length) {
            ImGui::SameLine();
        }
    }
}

void
open_file_dialog(void)
{
    char separator = '/';
    #if _WIN32
    separator = '\\';
    #endif
    // char typed_filename[400];
    begin_ui("Open File", 600, 400);
    tinydir_dir dir;
    if (tinydir_open_sorted(&dir, current_directory) == -1) {
        translated_text("Failed to open directory.");
        ImGui::EndUi();
        return;
    }
    int n_columns = 1 + dir.n_files/5;
    ImGui::Columns(n_columns, "files");
    for (size_t i=0; i<dir.n_files; i++) {
        tinydir_file file;
        tinydir_readfile_n(&dir, &file, i);
        if (button(file.name)) {
            if (file.is_dir) {
                sprintf(current_directory, "%s%c%s",
                    current_directory, separator, file.name);
                simplify_path(current_directory);
            }
            else {
                sprintf(to_open, "%s%s", current_directory, file.name);
            }
        }
        tinydir_next(&dir);
        if (i%5 == 4) {
            ImGui::NextColumn();
        }
    }
    ImGui::Columns();
    ImGui::Text(current_directory);
    ImGui::Text(to_open);
    //ImGui::InputText("", typed_filename, 100);
    if (button("Open")) {
        char cmd[200];
        sprintf(cmd, "open %s", to_open);
        log_debug(to_open);
        actuator(cmd);
        settings.show_open_file_dialog = false;
    }
    ImGui::SameLine();
    if (button("Cancel")) {
        settings.show_open_file_dialog = false;
    }
    tinydir_close(&dir);
    ImGui::EndUi();
}

void
parse_command(int argc, char *argv[], char command[200])
{
    for (int i=1; i<argc; i++) {
        char *s = argv[i];
        if (streq(s, "--local-boot") || streq(s, "-L")) {
            strcpy(settings.assets_dir, argv[i+1]);
            printf("Booting from \"%s\".\n", settings.assets_dir);
            i++;
            continue;
        }
        if (streq(s, "--debug") || streq(s, "-d")) {
            settings.debug_mode = 1;
            printf("DEBUG MODE\n");
            continue;
        }
        if (streq(s, "--help") || streq(s, "-h")) {
            usage();
            settings.running = false;
        }
        if (streq(s, "--version") || streq(s, "-v")) {
            /* For scripts that need the version string */
            printf("%s", settings.version);
            settings.running = false;
        }
        if (streq(s, "--test")) {
            settings.testing = true;
            continue;
        }
        strcat(command, " ");
        strcat(command, s);
    }

    get_current_directory(settings.current_directory);
}


void
load_menu(const char *menu_label)
{
    if (ImGui::BeginMenu(menu_label)) {
        Dictionary *dict = dictionary_from_index(menu_layout, menu_label);
        if (dict == NULL) {
            ImGui::EndMenu();
            return;
        }
        for (int i=0; i<dict->length; i++) {
            DictionaryEntry line = dict->data[i];
            if (!strcmp(line.key, "---")) {
                ImGui::Separator();
                continue;
            }
            if (!strcmp(line.value, "submenu")) {
                load_menu(line.key);
                continue;
            }
            if (ImGui::MenuItem(line.key)) {
                strcpy(menu_action, line.value);
            }
        }
        ImGui::EndMenu();
    }
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

GLuint
load_texture(char fname[200])
{
    int width, height;
    char fname2[200];
    sprintf(fname2, "%s%s", settings.assets_dir, fname);
    uint8_t* data = stbi_load(fname2, &width, &height, NULL, 4);
    if (!data) {
        return 0;
    }

    GLuint texture_id = gen_gl_texture(data, width, height, GL_CLAMP_TO_EDGE);
    stbi_image_free(data);

    return texture_id;
}

void
view_empty_double_click(void)
{
    actuator("open");
}

void
view_empty_paint_event(EmbView *view, int* event)
{
    QWidget* vport = viewport();
    EmbRect rect = vport->rect();

    QPainter painter(vport);
    painter.setRenderHint(QPainter_SmoothPixmapTransform);

    /* Always fill with a solid color first. */
    if (view->useColor) {
        painter.fillRect(rect, bgColor);
    }
    else {
        painter.fillRect(rect, background());
    }

    /* Then overlay the texture. */
    if (view->useTexture) {
        QBrush bgBrush(bgTexture);
        painter.fillRect(rect, bgBrush);
    }

    /* Overlay the logo last. */
    if (view->useLogo) {
        //Center the pixmap
        int dx = (rect.width()-bgLogo.width())/2;
        int dy = (rect.height()-bgLogo.height())/2;
        painter.drawPixmap(dx, dy, bgLogo.width(), bgLogo.height(), bgLogo);
    }
}

void
zoom_extents_all_subviews(void)
{
    for (int i=0; i<n_views; i++) {
        EmbView* v = &(views[i]);
        recalculateLimits(v);
        zoomExtents(v);
    }
}

/**
 * @brief .
 *
 * \todo Save a Brother PEL image (An 8bpp, 130x113 pixel monochromatic? bitmap image) Why 8bpp when only 1bpp is needed?
 *
 * \todo Should BMC be limited to ~32KB or is this a mix up with Bitmap Cache?
 *
 * \todo Is there/should there be other embedded data in the bitmap besides the image itself?
 *
 * NOTE: Can save a Singer BMC image (An 8bpp, 130x113 pixel colored bitmap image)
 *
 * \todo Make BMC background into it's own setting?
 *
 * \todo figure out how to center the image, right now it just plops it to the left side.
 */
void
save_bmc(void)
{
    QImage img(150, 150, QImage_Format_ARGB32_Premultiplied);
    img.fill(qRgb(255,255,255));
    EmbRect extents = active_view->itemsBoundingRect();

    QPainter painter(&img);
    EmbRect targetRect(0,0,150,150);
    if (settings_printing_disable_bg) {
        QBrush brush = active_view->backgroundBrush();
        active_view->setBackgroundBrush(Qt_NoBrush);
            active_view->render(&painter, targetRect, extents, Qt_KeepAspectRatio);
        active_view->setBackgroundBrush(brush);
    }
    else {
        active_view->render(&painter, targetRect, extents, Qt_KeepAspectRatio);
    }

    img.convertToFormat(QImage_Format_Indexed8, Qt_ThresholdDither|Qt_AvoidDither).save("test.bmc", "BMP");
}

void
set_current_file(const char *fileName)
{
    curFile = QFileInfo(fileName).canonicalFilePath();
    setWindowModified(false);
    setWindowTitle(getShortCurrentFile());
}

const char *
get_short_current_file(void)
{
    return QFileInfo(curFile).fileName();
}

const char *
file_extension(const char * fileName)
{
    return QFileInfo(fileName).suffix().toLower();
}

/**
 * @brief
 *
 * \todo need close_view_event and close_window_event? or action
 */
void
close_event(int* event)
{
    log_debug("closeEvent()");
    // emit sendCloseMdiWin(this);

    active_view->closeAllSubWindows();
    writeSettings();
    event->accept();
}

void
on_window_activated(void)
{
    log_debug("onWindowActivated()");
    gview->undo_stack->setActive(true);
    setUndoCleanIcon(fileWasLoaded);
    statusbarSnapButton_set_checked(active_view->SNAP);
    statusbarGridButton_set_checked(active_view->GRID);
    statusbarRulerButton_set_checked(active_view->RULER);
    statusbarOrthoButton_set_checked(active_view->ORTHO);
    statusbarPolarButton_set_checked(active_view->POLAR);
    statusbarQSnapButton_set_checked(active_view->QSNAP);
    statusbarQTrackButton_set_checked(active_view->QTRACK);
    statusbarLwtButton_set_checked(active_view->LWT);
    prompt->setHistory(promptHistory);

    if (active_view) {
        active_view->onWindowActivated();
    }
}

/**
 * @brief
 *
 * Returns EmbVector
 */
EmbVector
size_hint(void)
{
    log_debug("EmbView sizeHint()");
    EmbVector v = {450, 300};
    return v;
}

void
current_layer_changed(const char * layer)
{
    //curLayer = layer;
}

void
current_color_changed(unsigned int color)
{
    // curColor = color;
}

void
current_linetype_changed(const char *type)
{
    // curLineType = type;
}

void
current_lineweight_changed(const char * weight)
{
    // curLineWeight = weight;
}

void
updateColorLinetypeLineweight(void)
{

}

void
show_view_scrollbars(bool val)
{
    // gview->showscrollbars(val);
}

void
set_view_crosshair_color(unsigned int color)
{
    // gview->setCrossHairColor(color);
}

void
set_view_background_color(unsigned int color)
{
    // gview->setBackgroundColor(color);
}

void
set_view_selectbox_colors(
    unsigned int colorL,
    unsigned int fillL,
    unsigned int colorR,
    unsigned int fillR,
    int alpha)
{
    // gview->setSelectBoxColors(colorL, fillL, colorR, fillR, alpha);
}

void
set_viewGridColor(unsigned int color)
{
    // gview->setGridColor(color);
}

void
set_viewRulerColor(unsigned int color)
{
    // gview->setRulerColor(color);
}

void
prompt_input_prev_next(bool prev)
{
    if (promptInputList.isEmpty()) {
        if (prev) {
            critical_messagebox(translate("Prompt Previous Error"), translate("The prompt input is empty! Please report this as a bug!"));
        }
        else {
            critical_messagebox(translate("Prompt Next Error"), translate("The prompt input is empty! Please report this as a bug!"));
            log_debug("The prompt input is empty! Please report this as a bug!");
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
            prompt->set_current_text("");
        }
        else if (promptInputNum >= maxNum) {
            promptInputNum = maxNum;
            prompt->set_current_text("");
        }
        else {
            prompt->set_current_text(promptInputList.at(promptInputNum));
        }
    }
}

/**
 * \todo finish set_prompt_prefix
 */
void
set_prompt_prefix(const char *msg)
{

}

/**
 * \todo get actual thumbnail image from file,
 * lets also use a size of 128x128 for now...
 *
 * \todo make thumbnail size adjustable thru settings dialog
 *
 * \todo connect the currentChanged signal to update the preview imgWidget.
 */
void
preview_dialog(void)
    QWidget* parent,
    const char * caption,
    const char * dir,
    const char * filter) : QFileDialog(parent, caption, dir, filter)
{
    log_debug("PreviewDialog Constructor");

    imgWidget = ImageWidget("icons/default/nopreview.png", this);

    QLayout* lay = layout();
    if (<QGridLayout*>(lay)) {
        QGridLayout* grid = <QGridLayout*>(lay);
        grid->add_widget(imgWidget, 0, grid->columnCount(), grid->rowCount(), 1);
    }

    setModal(true);
    setOption(QFileDialog_DontUseDialog);
    set_viewMode(QFileDialog_Detail);
    setFileMode(QFileDialog_ExistingFiles);
}

/**
 * @brief Set the Mouse Coord object.
 *
 * \a pos
 *
 * \todo set format from settings (Architectural, Decimal,
 * Engineering, Fractional, Scientific)
 *
 * \todo use precision from unit settings
 */
void
set_mouse_coord(EmbVector pos)
{
    char s[200];
    sprintf(s, "%f %f", pos.x, pos.y);

    /* Decimal */
    // sprintf(s, "%4f %4f", pos.x, pos.y);

    /* Scientific */
    // sprintf(s, "%4e %4e", pos.x, pos.y);

    translated_text(s);
}

/**
 * @brief This appears not to be used any more.
 */
void
status_bar_context_menu(void)
{
    QMenu menu(this);
    if (objectName() == "statusbar_buttonSNAP") {
        QAction* settingsSnapAction = QAction(
            load_icon("gridsnapsettings"),
            "&Settings...",
            &menu);
        connect(settingsSnapAction, triggered(), settingsSnap());
        menu.addAction(settingsSnapAction);
    }
    else if (objectName() == "statusbar_buttonGRID") {
        QAction* settingsGridAction = QAction(
            load_icon("gridsettings"),
            "&Settings...",
            &menu);
        menu.addAction(settingsGridAction);
    }
    else if (objectName() == "statusbar_buttonRULER") {
        QAction* settingsRulerAction = QAction(
            load_icon("rulersettings"),
            "&Settings...",
            &menu);
        menu.addAction(settingsRulerAction);
    }
    else if (objectName() == "statusbar_buttonORTHO") {
        QAction* settingsOrthoAction = QAction(
            load_icon("orthosettings"), "&Settings...", &menu);
        menu.addAction(settingsOrthoAction);
    }
    else if (objectName() == "statusbar_buttonPOLAR") {
        QAction* settingsPolarAction = QAction(
            load_icon("polarsettings"), "&Settings...", &menu);
        connect(settingsPolarAction, triggered(), settingsPolar());
        menu.addAction(settingsPolarAction);
    }
    else if (objectName() == "statusbar_buttonQSNAP") {
        QAction* settingsQSnapAction = QAction(
            load_icon("qsnapsettings"), "&Settings...", &menu);
        connect(settingsQSnapAction, triggered(), settingsQSnap());
        menu.addAction(settingsQSnapAction);
    }
    else if (objectName() == "statusbar_buttonQTRACK") {
        QAction* settingsQTrackAction = QAction(
            load_icon("qtracksettings"), "&Settings...", &menu);
        connect(settingsQTrackAction, triggered(), settingsQTrack());
        menu.addAction(settingsQTrackAction);
    }
    else if (objectName() == "statusbar_buttonLWT") {
        QAction* enableRealAction = QAction(
            load_icon("realrender"),
            "&RealRender On",
            &menu);
        enableRealAction->setEnabled(!gview->isRealEnabled());
        connect(enableRealAction, triggered(), enableReal());
        menu.addAction(enableRealAction);

        QAction* disableRealAction = QAction(
            load_icon("realrender"),
            "&RealRender Off",
            &menu);
        disableRealAction->setEnabled(gview->isRealEnabled());
        connect(disableRealAction, triggered(), disableReal());
        menu.addAction(disableRealAction);

        QAction* settingsLwtAction = QAction(
            load_icon("lineweightsettings"),
            "&Settings...",
            &menu);
        connect(settingsLwtAction, triggered(), settingsLwt());
        menu.addAction(settingsLwtAction);
    }
    menu.exec(event->globalPos());
    statusbar_clearMessage();
}

/**
 * @brief Renderer for the about dialog.
 * Called by the main widget.
 */
void
about_dialog(void)
{
    char out[200];
    log_debug("about_dialog()");

    begin_ui("About Embroidermodder 2", 500, 200);
    // ImageWidget img(assets_dir + "/images/logo-small");
    sprintf(out, "Embroidermodder %s", settings.version);
    translated_text(out);
    translated_text("https://www.libembroidery.org");
    translated_text("Available Platforms: GNU/Linux, Windows, Mac OSX, Raspberry Pi.");
    translated_text("Embroidery formats by Josh Varga and Robin Swift.");
    translated_text("User Interface by Jonathan Greig and Robin Swift.");
    translated_text("Free under the zlib/libpng license.");
    if (button("Oh, Yeah!")) {
        settings.show_about_dialog = false;
    }
    ImGui::EndUi();
}

/**
 * @brief Renderer for the changelog dialog.
 * Called by the main widget.
 */
void
changelog_dialog(void)
{
    begin_ui("Changelog", 500, 200);
    ImGui::EndUi();
}

/**
 * @brief Renderer for the alert dialog.
 * Called by the main widget, with the message passed as \a txt.
 */
void
alert(const char *txt)
{
    const char *title = "";
    begin_ui(title, 500, 200);
    translated_text(txt);
    if (button("Ok")) {

    }
    ImGui::EndUi();
}

/**
 * @brief Renderer for the help dialog.
 * Called by the main widget.
 */
void
help_dialog(void)
{
    begin_ui("Help", 700, 500);

    translated_text("Help -- Embroidermodder 2");

    /* Minimum size (600, 500). */

    FILE *f;
    char *s;
    char fname[200];
    sprintf(fname, "%sdocs/manual.md", settings.assets_dir);
    f = fopen(fname, "r");
    if (f) {
        fseek(f, 0, SEEK_END);
        size_t length = ftell(f);
        fseek(f, 0, SEEK_SET);
        s = (char*)malloc(length+2);
        fread(s, 1, length, f);
        fclose(f);
        /* This needs a different function for wrapping. */
        translated_text(s);
        free(s);
    }
    else {
        translated_text("Failed to load manual.");
    }

    /* Center button */
    if (button("Close")) {

    }

    ImGui::EndUi();
}

/**
 * \todo Move majority of the calculation code into libembroidery
 *
 * \todo Color Histogram
 *
 * \todo Stitch Distribution Histogram
 *
 * Switch to grid layout.
 */
void
details_dialog(void)
{
    if (n_views < 1) {
        return;
    }

    begin_ui(translate("Embroidery Design Details"), 500, 500);

    EmbRect boundingRect = embPattern_calcBoundingBox(active_view->pattern);

    int colorChanges  = 0;

    if (active_view->pattern->stitch_list->count == 0) {
            alert(translate("No Design Loaded"),
            translate("<b>A design needs to be loaded or created before details can be determined.</b>"));
            return;
    }

    std_vector<EmbReal> stitchLengths;
    int colors = 0;
    EmbReal totalColorLength = 0.0;
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

    EmbReal binSize = max_stitchlength / NUMBINS;
    const char *str;
    for (int i = 0; i < NUMBINS; i++) {
        str += std_to_string(binSize * (i));
        str += " - " + std_to_string(binSize * (i+1));
        str += " mm: " +  std_to_string(bin[i]) + "\n\n";
    }

    translated_text("Stitch Distribution:");
    translated_text("%s", str.c_stranslate());
    translated_text("Thread Length By Color:");
    int currentRow = 12;

    for (int i = 0; i < pattern->n_threads; i++) {
        QFrame *frame = QFrame();
        frame->setGeometry(0,0,30,30);
        QPalette palette = frame->palette();
        EmbColor t = embThreadList_getAt(pattern->threadList, i).color;
        palette.setColor(backgroundRole(), EmbColor( t.r, t.g, t.b ) );
        frame->setPalette( palette );
        frame->setAutoFillBackground(true);
        grid->add_widget(frame, currentRow,0,1,1);
        log_debug("size: %d i: %d", stitchLengths.size(), i);
        grid->add_widget(new QLabel(std_string_number(stitchLengths.at(i)) + " mm"), currentRow,1,1,1);
        currentRow++;
    }

    /* setMinimumSize(750,550); */

    /* Collapsing header? */
    translated_text("General Information");

    show_real("Total Stitches:", active_view->pattern->stitch_list->count);
    show_real("Total Stitch Length:", embPattern_totalStitchLength(active_view->pattern));
    show_int("Real Stitches:", embPattern_realStitches(active_view->pattern));
    show_int("Jump Stitches:", embPattern_jumpStitches(active_view->pattern));
    show_int("Trim Stitches:", embPattern_trimStitches(active_view->pattern));

    show_int("Total Colors:", active_view->pattern->thread_list->count);
    show_int("Color Changes:", colorChanges);

    translated_text("Left:");
    ImGui::SameLine();
    char s[200];
    sprintf(s, "%f mm", boundingRect.left);
    translated_text(s);

    translated_text("Top:");
    ImGui::SameLine();
    sprintf(s, "%f mm", boundingRect.top);
    translated_text(s);

    translated_text("Right:");
    ImGui::SameLine();
    sprintf(s, "%f mm", boundingRect.right);
    translated_text(s);

    translated_text("Bottom:");
    ImGui::SameLine();
    sprintf(s, "%f mm", boundingRect.bottom);
    translated_text(s);

    translated_text("Width:");
    ImGui::SameLine();
    sprintf(s, "%f mm", boundingRect.right-boundingRect.left);
    translated_text(s);

    translated_text("Height:");
    ImGui::SameLine();
    sprintf(s, "%f mm", boundingRect.bottom-boundingRect.top);
    translated_text(s);

    if (button("Ok")) {
        settings.show_details_dialog = false;
    }

    ImGui::EndUi();

    /* Stitch Distribution */
    //QGroupBox* groupBoxDist = QGroupBox(translate("Stitch Distribution"), widget);
}

/**
 * \todo Add icons to buttons by using wizardTipOfTheDay->setButton(CustomButton1, buttonPrevious)
 *
 * \todo Add icons to buttons by using wizardTipOfTheDay->setButton(CustomButton1, buttonNext)
 *
 * \todo Add icons to buttons by using wizardTipOfTheDay->setButton(CustomButton1, buttonClose)
 */
void
tip_of_the_day_dialog(void)
{
    log_debug("tipOfTheDay()");

    const char *appDir = qApp->applicationDirPath();

    wizardTipOfTheDay = QWizard(this);
    wizardTipOfTheDay->setAttribute(WA_DeleteOnClose);
    wizardTipOfTheDay->setWizardStyle(ModernStyle);
    wizardTipOfTheDay->setMinimumSize(550, 400);

    QWizardPage* page = QWizardPage(wizardTipOfTheDay);

    ImageWidget* imgBanner = ImageWidget(appDir + "/images/did-you-know.png", wizardTipOfTheDay);

    if (settings_general_current_tip >= listTipOfTheDay.size()) {
        settings_general_current_tip = 0;
    }
    labelTipOfTheDay = QLabel(listTipOfTheDay.value(settings_general_current_tip), wizardTipOfTheDay);
    labelTipOfTheDay->setWordWrap(true);

    QCheckBox* checkBoxTipOfTheDay = QCheckBox(translate("&Show tips on startup"), wizardTipOfTheDay);
    settings_general_tip_of_the_day = settings_general_tip_of_the_day;
    checkBoxTipOfTheDay_set_checked(settings_general_tip_of_the_day);
    connect(checkBoxTipOfTheDay, stateChanged(int), checkBoxTipOfTheDayStateChanged(int)));

    QVBoxLayout* layout = QVBoxLayout(wizardTipOfTheDay);
    layout->add_widget(imgBanner);
    layout->addStrut(1);
    layout->add_widget(labelTipOfTheDay);
    layout->addStretch(1);
    layout->add_widget(checkBoxTipOfTheDay);
    page->set_layout(layout);
    wizardTipOfTheDay->addPage(page);

    wizardTipOfTheDay->setWindowTitle("Tip of the Day");

    wizardTipOfTheDay->setButtonText(CustomButton1, translate("&Previous"));
    wizardTipOfTheDay->setButtonText(CustomButton2, translate("&Next"));
    wizardTipOfTheDay->setButtonText(CustomButton3, translate("&Close"));
    wizardTipOfTheDay->setOption(HaveCustomButton1, true);
    wizardTipOfTheDay->setOption(HaveCustomButton2, true);
    wizardTipOfTheDay->setOption(HaveCustomButton3, true);
    connect(wizardTipOfTheDay, customButtonClicked(int), buttonTipOfTheDayClicked(int)));

    std_vector<WizardButton> listTipOfTheDayButtons;
    listTipOfTheDayButtons << Stretch << CustomButton1 << CustomButton2 << CustomButton3;
    wizardTipOfTheDay->setButtonLayout(listTipOfTheDayButtons);

    wizardTipOfTheDay->exec();
}

void
image_widget_init(const char *filename)
{
    log_debug("ImageWidget Constructor");

    img.load(filename);

    setMinimumWidth(img.width());
    setMinimumHeight(img.height());
    setMaximumWidth(img.width());
    setMaximumHeight(img.height());

 this->show();
}

bool
image_widget_load(const char *fileName)
{
    img.load(fileName);
    return true;
}

bool
image_widget_save(const char *fileName)
{
    img.save(fileName, "PNG");
    return true;
}

void
image_widget_paint_event(void)
{
    QPainter painter(this);
    painter.set_viewport(0, 0, width(), height());
    painter.setWindow(0, 0, width(), height());
    painter.drawImage(0, 0, img);
}

void
layer_manager(EmbView *view)
{
    layerModel = QStandardItemModel(0, 8, this);

    layerModelSorted = QSortFilterProxyModel;
    layerModelSorted->setDynamicSortFilter(true);
    layerModelSorted->setSourceModel(layerModel);

    treeEmbView = QTreeEmbView;
    treeEmbView->setRootIsDecorated(false);
    treeEmbView->setAlternatingRowColors(true);
    treeEmbView->setModel(layerModelSorted);
    treeEmbView->setSortingEnabled(true);
    treeEmbView->sortByColumn(0, AscendingOrder);

    QVBoxLayout *mainLayout = QVBoxLayout;
    mainLayout->add_widget(treeEmbView);
    set_layout(mainLayout);

    set_window_title(translate("Layer Manager"));
    set_minimum_size(750, 550);

    layerModel->setHeaderData(0, Horizontal, translate("Name"));
    layerModel->setHeaderData(1, Horizontal, translate("Visible"));
    layerModel->setHeaderData(2, Horizontal, translate("Frozen"));
    layerModel->setHeaderData(3, Horizontal, translate("Z Value"));
    layerModel->setHeaderData(4, Horizontal, translate("Color"));
    layerModel->setHeaderData(5, Horizontal, translate("Linetype"));
    layerModel->setHeaderData(6, Horizontal, translate("Lineweight"));
    layerModel->setHeaderData(7, Horizontal, translate("Print"));

    addLayer("0", true, false, 0.0, qRgb(0,0,0), "Continuous", "Default", true);
    addLayer("1", true, false, 1.0, qRgb(0,0,0), "Continuous", "Default", true);
    addLayer("2", true, false, 2.0, qRgb(0,0,0), "Continuous", "Default", true);
    addLayer("3", true, false, 3.0, qRgb(0,0,0), "Continuous", "Default", true);
    addLayer("4", true, false, 4.0, qRgb(0,0,0), "Continuous", "Default", true);
    addLayer("5", true, false, 5.0, qRgb(0,0,0), "Continuous", "Default", true);
    addLayer("6", true, false, 6.0, qRgb(0,0,0), "Continuous", "Default", true);
    addLayer("7", true, false, 7.0, qRgb(0,0,0), "Continuous", "Default", true);
    addLayer("8", true, false, 8.0, qRgb(0,0,0), "Continuous", "Default", true);
    addLayer("9", true, false, 9.0, qRgb(0,0,0), "Continuous", "Default", true);

    for (int i = 0; i < layerModel->columnCount(); ++i) {
        treeEmbView->resizeColumnToContents(i);
    }
}

void
Layer_manager_add_layer(
    const char *name,
    bool visible,
    bool frozen,
    EmbReal zValue,
    unsigned int color,
    const char *lineType,
    const char *lineWeight,
    bool print)
{
    layerModel->insertRow(0);
    layerModel->setData(layerModel->index(0, 0), name);
    layerModel->setData(layerModel->index(0, 1), visible);
    layerModel->setData(layerModel->index(0, 2), frozen);
    layerModel->setData(layerModel->index(0, 3), zValue);

    QPixmap colorPix(QSize(16,16));
    colorPix.fill(EmbColor(color));
    layerModel->itemFromIndex(layerModel->index(0, 4))->setIcon(QIcon(colorPix));
    layerModel->setData(layerModel->index(0, 4), EmbColor(color));

    layerModel->setData(layerModel->index(0, 5), lineType);
    layerModel->setData(layerModel->index(0, 6), lineWeight);
    layerModel->setData(layerModel->index(0, 7), print);
}

/**
 * \todo Review OpenGL direct calls (rather than via GLFW).
 *
 * \todo panDistance needs a setting.
 *
 * \todo load toggleReal from file, else settings with default being true.
 */
void
EmbView__(void)
{
    mainWin = mw;
    active_view = theScene;

    setFrameShape(QFrame_NoFrame);

    /* NOTE: This has to be done before setting mouse tracking.
    if (settings_display_use_opengl) {
        log_debug("Using OpenGL...");
        set_viewport(new QGLWidget(QGLFormat(QGL_DoubleBuffer)));
    }

    set_viewportUpdateMode(QGraphicsFullEmbViewportUpdate);

    panDistance = 10;

    setCursor(BlankCursor);
    horizontalscrollbar()->setCursor(ArrowCursor);
    verticalscrollbar()->setCursor(ArrowCursor);
    qsnapLocatorColor = settings.qsnap_locator_color;
    qsnapLocatorSize = settings.qsnap_locator_size;
    qsnapApertureSize = settings_qsnap_aperture_size;
    gripColorCool = settings.selection_coolgrip_color;
    gripColorHot = settings.selection_hotgrip_color;
    gripSize = settings.selection_grip_size;
    pickBoxSize = settings.selection_pickbox_size;
    setCrossHairColor(settings.display_crosshair_color);
    setCrossHairSize(settings.display_crosshair_percent);
    setGridColor(settings.grid_color);

    if (settings.grid_show_on_load) {
        createGrid(settings.grid_type);
    }
    else {
        createGrid("");
    }

    toggleRuler(settings.ruler_show_on_load);
    toggleReal(true);

    grippingActive = false;
    rapidMoveActive = false;
    previewMode = PREVIEW_MODE_NULL;
    previewData = 0;
    previewObjectItemGroup = 0;
    pasteObjectItemGroup = 0;
    previewActive = false;
    active_view->pasting_active = false;
    movingActive = false;
    selectingActive = false;
    zoomWindowActive = false;
    panningRealTimeActive = false;
    panningPointActive = false;
    panningActive = false;
    qSnapActive = false;

    // Randomize the hot grip location initially so it's not located at (0,0)
    srand(time(NULL));
    sceneGripPoint = EmbVector(random_uniform()*1000, random_uniform()*1000);

    gripBaseObj = 0;
    tempBaseObj = 0;

    selectBox = SelectBox(QRubberBand_Rectangle, this);
    selectBox->set_colors(
        settings.display_selectbox_left_color,
        settings.display_selectbox_left_fill,
        settings.display_selectbox_right_color,
        settings.display_selectbox_right_fill,
        settings.display_selectbox_alpha);

    show_scrollbars(settings.display_show_scrollbars);
    setCornerButton();

    undo_stack = QUndoStack(this);
    dockUndoEdit->addStack(undo_stack);

    installEventFilter(this);

    setMouseTracking(true);
    setBackgroundColor(settings.display_bg_color);
    //\todo wrap this with a setBackgroundPixmap() function: setBackgroundBrush(QPixmap("images/canvas.png"));

    connect(active_view, selectionChanged(), selectionChanged()));
}

/**
 * @brief Prevent memory leaks by deleting any objects that were removed
 * from the scene.
 */
void
view_free(void)
{
    delete_all(hashDeletedObjects.begin(), hashDeletedObjects.end());
    hashDeletedObjects.clear();

    //Prevent memory leaks by deleting any unused instances
    delete_all(previewObjectList.begin(), previewObjectList.end());
    previewObjectList.clear();
}

void
enter_event(int* event)
{
    if (active_view != NULL) {
        active_view->setActiveSubWindow();
    }
}

void
add_object(EmbGeometry* obj)
{
    active_view->addItem(obj);
    hashDeletedObjects.remove(obj->objID);
}

/**
 * @brief We really just remove the objects from the scene.
 * Deletion actually occurs in the destructor.
 */
void
delete_object(EmbGeometry* obj)
{
    obj->set_selected(false);
    active_view->removeItem(obj);
    hashDeletedObjects.insert(obj->objID, obj);
}

void
preview_on(int clone, int mode, EmbReal x, EmbReal y, EmbReal data)
{
    log_debug("EmbView previewOn()");
    if (active_view == NULL) {
        return;
    }
    active_view->previewOn(clone, mode, x, -y, data);

    previewOff(); //Free the old objects before creating new ones

    previewMode = mode;

    //Create new objects and add them to the scene in an item group.
    if (clone == PREVIEW_CLONE_SELECTED)
        previewObjectList = createObjectList( active_view->selected);
    else if (clone == PREVIEW_CLONE_RUBBER)
        previewObjectList = createObjectList(rubberRoomList);
    else {
        return;
    }
    previewObjectItemGroup = active_view->createItemGroup(previewObjectList);

    if (previewMode == preview.MODE_MOVE   ||
       previewMode == preview.MODE_ROTATE ||
       previewMode == preview.MODE_SCALE) {
        previewPoint = EmbVector(x, y); //NOTE: Move: basePt; Rotate: basePt;   Scale: basePt;
        previewData = data;           //NOTE: Move: unused; Rotate: refAngle; Scale: refFactor;
        previewActive = true;
    }
    else {
        previewMode = PREVIEW_MODE_NULL;
        previewPoint = EmbVector();
        previewData = 0;
        previewActive = false;
    }
}

void
preview_off(void)
{
    if (active_view == NULL) {
        return;
    }

    /* Prevent memory leaks by deleting any unused instances. */
    delete_all(previewObjectList.begin(), previewObjectList.end());
    previewObjectList.clear();

    if (previewObjectItemGroup) {
        active_view->removeItem(previewObjectItemGroup);
        delete previewObjectItemGroup;
        previewObjectItemGroup = 0;
    }

    previewActive = false;
}

void
enableMoveRapidFire(void)
{
    if (active_view) active_view->enableMoveRapidFire();
    active_view->move_rapid_fire = true;
    rapidMoveActive = true;
}

void
disable_move_rapid_fire(void)
{
    if (active_view) active_view->disableMoveRapidFire();
    active_view->move_rapid_fire = false;
    rapidMoveActive = false;
}

/**
 * \todo the rubberRoomList size check should be removed later.
 */
bool
allow_rubber(void)
{
    if (active_view == NULL) {
        return false;
    }

    /* if (!rubberRoomList.size()) */ {
        return true;
    }
    return false;
}

void
add_to_rubber_room(EmbGeometry* item)
{
    rubberRoomList.append(item);
    item->show();
}

void
vulcanize_rubber_room(void)
{
    for (EmbGeometry* item, rubberRoomList) {
        EmbGeometry* base = static_cast<EmbGeometry*>(item);
        if (base) vulcanizeObject(base);
    }
    rubberRoomList.clear();
}

void
vulcanizeObject(EmbGeometry* obj)
{
    if (!obj) {
        return;
    }

    active_view->removeItem(obj); //Prevent Qt Runtime Warning, QGraphicsScene_addItem: item has already been added to this scene
    obj->vulcanize();

    UndoableAddCommand* cmd = UndoableAddCommand(obj->data(OBJ_NAME).toString(), obj, 0);
    if (cmd) undo_stack->push(cmd);
}

void
clearRubberRoom(void)
{
    for (EmbGeometry* item, rubberRoomList) {
        EmbGeometry* base = static_cast<EmbGeometry*>(item);
        if (base) {
            int type = base->type;
            if ((type == EMB_PATH     && spareRubberList.contains(SPARE_RUBBER_PATH))     ||
               (type == EMB_POLYGON  && spareRubberList.contains(SPARE_RUBBER_POLYGON))  ||
               (type == EMB_POLYLINE && spareRubberList.contains(SPARE_RUBBER_POLYLINE)) ||
               (spareRubberList.contains(base->objID))) {
                if (!base->objectPath().elementCount()) {
                    critical_messagebox(translate("Empty Rubber Object Error"),
                                          translate("The rubber object added contains no points. "
                                          "The command that created this object has flawed logic. "
                                          "The object will be deleted."));
                    active_view->removeItem(item);
                    delete item;
                }
                else {
                    vulcanizeObject(base);
                }
            }
            else {
                active_view->removeItem(item);
                delete item;
            }
        }
    }

    rubberRoomList.clear();
    spareRubberList.clear();
}

void
setRubberMode(int mode)
{
    for (EmbGeometry* item : rubberRoomList) {
        EmbGeometry* base = static_cast<EmbGeometry*>(item);
        if (base) {
            base->setObjectRubberMode(mode);
        }
    }
}

void
setRubberPoint(const char *key, EmbVector point)
{
    for (EmbGeometry* item : rubberRoomList) {
        EmbGeometry* base = static_cast<EmbGeometry*>(item);
        if (base) {
            base->setObjectRubberPoint(key, point);
        }
    }
}

void
setRubberText(const char *key, const char *txt)
{
    for (EmbGeometry* item : rubberRoomList) {
        EmbGeometry* base = static_cast<EmbGeometry*>(item);
        if (base) {
            base->setObjectRubberText(key, txt);
        }
    }
}

void
setGridColor(unsigned int color)
{
    gridColor = EmbColor(color);
    active_view->setProperty(VIEW_COLOR_GRID, color);
}

void
setRulerColor(unsigned int color)
{
    rulerColor = EmbColor(color);
}

void
createGrid(const char *gridType)
{
    if (gridType == "Rectangular") {
        createGridRect();
        active_view->setProperty("ENABLE_GRID", true);
    }
    else if (gridType == "Circular") {
        createGridPolar();
        active_view->setProperty("ENABLE_POLAR", true);
    }
    else if (gridType == "Isometric") {
        createGridIso();
        active_view->setProperty("ENABLE_ISOMETRIC", true);
    }
    else {
        gridPath = EmbPath();
        active_view->setProperty("ENABLE_GRID", false);
    }

    createOrigin();
}

/**
 * \todo Make Origin Customizable
 */
void
createOrigin(void)
{
    originPath = EmbPath();

    if (settings.grid_show_origin) {
        //originPath.addEllipse(EmbVector(0,0), 0.5, 0.5); //\todo Make Origin Customizable
        EmbReal rad = 0.5;
        originPath.moveTo(0.0, rad);
        originPath.arcTo(-rad, -rad, rad*2.0, rad*2.0, 90.0, 360.0);
        originPath.arcTo(-rad, -rad, rad*2.0, rad*2.0, 90.0, -360.0);
        originPath.lineTo(0.0, -rad);
        originPath.arcTo(-rad, -rad, rad*2.0, rad*2.0, 270.0, 90.0);
        originPath.lineTo(-rad, 0.0);
        originPath.arcTo(-rad, -rad, rad*2.0, rad*2.0, 180.0, -90.0);
        originPath.closeSubpath();
    }
}

void
createGridRect(void)
{
    EmbRect gr(0, 0, settings.grid_size_x, -settings.grid_size_y);
    //Ensure the loop will work correctly with negative numbers
    EmbReal x1 = EMB_MIN(gr.left(), gr.right());
    EmbReal y1 = EMB_MIN(gr.top(), gr.bottom());
    EmbReal x2 = EMB_MAX(gr.left(), gr.right());
    EmbReal y2 = EMB_MAX(gr.top(), gr.bottom());

    gridPath = EmbPath();
    gridPath.addRect(gr);
    for (EmbReal gx = x1; gx < x2; gx += settings.grid_spacing_x) {
        for (EmbReal gy = y1; gy < y2; gy += settings.grid_spacing_y) {
            gridPath.moveTo(x1,gy);
            gridPath.lineTo(x2,gy);
            gridPath.moveTo(gx,y1);
            gridPath.lineTo(gx,y2);
        }
    }

    //Center the Grid
    EmbRect gridRect = gridPath.boundingRect();
    EmbReal bx = gridRect.width()/2.0;
    EmbReal by = -gridRect.height()/2.0;
    EmbReal dx = settings.grid_center.x - bx;
    EmbReal dy = settings.grid_center.y - by;

    if (settings.grid_center_on_origin) {
        gridPath.translate(-bx, -by);
    }
    else {
        gridPath.translate(dx, dy);
    }
}

void
createGridPolar(void)
{
    EmbReal rad = settings.grid_size_radius;

    gridPath = EmbPath();
    gridPath.addEllipse(EmbVector(0, 0), rad, rad);
    for (EmbReal r = 0; r < rad; r += settings.grid_spacing_radius) {
        gridPath.addEllipse(EmbVector(0,0), r, r);
    }
    for (EmbReal ang = 0; ang < 360; ang += settings.grid_spacing_angle) {
        gridPath.moveTo(0, 0);
        gridPath.lineTo(EmbLine_fromPolar(rad, ang).p2());
    }

    if (!settings.grid_center_on_origin) {
        gridPath.translate(settings.grid_center.x, -settings.grid_center.y);
    }
}

void
createGridIso(void)
{
    // Ensure the loop will work correctly with negative numbers
    EmbReal isoW = fabs(settings.grid_size_x);
    EmbReal isoH = fabs(settings.grid_size_y);

    EmbVector p1 = EmbVector(0,0);
    EmbVector p2 = EmbLine_fromPolar(isoW,  30).p2();
    EmbVector p3 = EmbLine_fromPolar(isoH, 150).p2();
    EmbVector p4 = p2 + p3;

    gridPath = EmbPath();
    gridPath.moveTo(p1);
    gridPath.lineTo(p2);
    gridPath.lineTo(p4);
    gridPath.lineTo(p3);
    gridPath.lineTo(p1);

    for (float x = 0.0; x < isoW; x += settings.grid_spacing_x) {
        for (float y = 0.0; y < isoH; y += settings.grid_spacing_y) {
            EmbVector px = EmbLine_fromPolar(x,  30).p2();
            EmbVector py = EmbLine_fromPolar(y, 150).p2();

            gridPath.moveTo(px);
            gridPath.lineTo(px+p3);
            gridPath.moveTo(py);
            gridPath.lineTo(py+p2);
        }
    }

    /* Center the Grid */
    EmbRect gridRect = gridPath.boundingRect();
    // bx is unused
    EmbReal by = -gridRect.height()/2.0;

    if (settings.grid_center_on_origin) {
        gridPath.translate(0, -by);
    }
    else {
        gridPath.translate(0, -by);
        gridPath.translate(settings.grid_center.x, -settings.grid_center.y);
    }
}

void
toggleGrid(bool on)
{
    if (on) {
        /* createGrid(settings.grid_type); */
    }
    else {
        /* createGrid(""); */
    }
}

void
drawBackground(int* painter, const EmbRect rect)
{
    painter->fillRect(rect, backgroundBrush());

    if (active_view->property("ENABLE_GRID").toBool() && rect.intersects(gridPath.controlPointRect())) {
        QPen gridPen(gridColor);
        gridPen.setJoinStyle(MiterJoin);
        gridPen.setCosmetic(true);
        painter->setPen(gridPen);
        painter->drawPath(gridPath);
        painter->drawPath(originPath);
        painter->fillPath(originPath, gridColor);
    }
}

void
drawForeground(int* painter, EmbRect rect)
{
    /**
     * Draw grip points for all selected objects
 
    QPen gripPen(gripColorCool);
    gripPen.setWidth(2);
    gripPen.setJoinStyle(MiterJoin);
    gripPen.setCosmetic(true);
    painter->setPen(gripPen);
    EmbVector gripOffset(gripSize, gripSize);

    EmbVector *selectedGripPoints;
    EmbGeometry *selectedItemList =  active_view->selected;
    if (selectedItemList.size() <= 100) {
        for (EmbGeometry* item, selectedItemList) {
            if (item->type() >= EMB_BASE) {
                tempBaseObj = static_cast<EmbGeometry*>(item);
                if (tempBaseObj) { selectedGripPoints = tempBaseObj->allGripPoints(); }

                for (EmbVector ssp, selectedGripPoints) {
                    EmbVector p1 = map_from_scene(ssp) - gripOffset;
                    EmbVector q1 = map_from_scene(ssp) + gripOffset;

                    if (ssp == sceneGripPoint) {
                        painter->fillRect(EmbRect(map_to_scene(p1), map_to_scene(q1)), gripColorHot);
                    }
                    else {
                        painter->drawRect(EmbRect(map_to_scene(p1), map_to_scene(q1)));
                    }
                }
            }
        }
    }

    /* ==================================================
     * Draw the closest qsnap point
     * ================================================== */

    /* \todo && findClosestSnapPoint == true */
    if (!selectingActive) {
        QPen qsnapPen(qsnapLocatorColor);
        qsnapPen.setWidth(2);
        qsnapPen.setJoinStyle(MiterJoin);
        qsnapPen.setCosmetic(true);
        painter->setPen(qsnapPen);
        EmbVector qsnapOffset(qsnapLocatorSize, qsnapLocatorSize);

        EmbVector *apertureSnapPoints;
        GraphicsItem *> apertureItemList = items(
            viewMousePoint.x-qsnapApertureSize,
            viewMousePoint.y-qsnapApertureSize,
            qsnapApertureSize*2,
            qsnapApertureSize*2);
        for (EmbGeometry* item, apertureItemList) {
            if (item->type() >= EMB_BASE) {
                tempBaseObj = static_cast<EmbGeometry*>(item);
                if (tempBaseObj) { apertureSnapPoints << tempBaseObj->mouseSnapPoint(sceneMousePoint); }
            }
        }
        //\todo Check for intersection snap points and add them to the list
        for (EmbVector asp, apertureSnapPoints) {
            EmbVector p1 = map_from_scene(asp) - qsnapOffset;
            EmbVector q1 = map_from_scene(asp) + qsnapOffset;
            painter->drawRect(EmbRect(map_to_scene(p1), map_to_scene(q1)));
        }
    }

    //==================================================
    //Draw the crosshair
    //==================================================

    if (!selectingActive) {
        //painter->setBrush(NoBrush);
        QPen crosshairPen(crosshairColor);
        crosshairPen.setCosmetic(true);
        painter->setPen(crosshairPen);
        painter->drawLine(EmbLine(map_to_scene(viewMousePoint.x, viewMousePoint.y-crosshairSize),
                                 map_to_scene(viewMousePoint.x, viewMousePoint.y+crosshairSize)));
        painter->drawLine(EmbLine(
            map_to_scene(viewMousePoint.x-crosshairSize, viewMousePoint.y),
            map_to_scene(viewMousePoint.x+crosshairSize, viewMousePoint.y)));
        painter->drawRect(EmbRect(
            map_to_scene(viewMousePoint.x-pickBoxSize, viewMousePoint.y-pickBoxSize),
            map_to_scene(viewMousePoint.x+pickBoxSize, viewMousePoint.y+pickBoxSize)));
    }
}

void
update_mouse_coords(EmbVector position)
{
    viewMousePoint = position;
    sceneMousePoint = map_to_scene(viewMousePoint);
    active_view->setProperty("SCENE_QSNAP_POINT", sceneMousePoint); //\todo if qsnap functionality is enabled, use it rather than the mouse point
    active_view->setProperty("SCENE_MOUSE_POINT", sceneMousePoint);
    active_view->setProperty("VIEW_MOUSE_POINT", viewMousePoint);
    statusbar_setMouseCoord(sceneMousePoint.x, -sceneMousePoint.y);
}

void
setCrossHairSize(uint8_t percent)
{
    //NOTE: crosshairSize is in pixels and is a percentage of your screen width
    //NOTE: Example: (1280*0.05)/2 = 32, thus 32 + 1 + 32 = 65 pixel wide crosshair
    QScreen *screen = qApp->primaryScreen();
    quint32 screenWidth = screen->grabWindow(0).width();
    if (percent > 0 && percent < 100) {
        crosshairSize = (screenWidth*(percent/100.0))/2;
    }
    else {
        crosshairSize = screenWidth;
    }
}

void
setCornerButton(void)
{
    int num = settings.display_scrollbar_widget_num;
    if (num) {
        QPushButton* cornerButton = QPushButton(this);
        cornerButton->setFlat(true);
        QAction* act = actionHash.value(num);
        //NOTE: Prevent crashing if the action is NULL.
        if (!act) {
            information_messagebox(translate("Corner Widget Error"), translate("There are unused enum values in COMMAND_ACTIONS. Please report this as a bug."));
            setCornerWidget(0);
        }
        else {
            cornerButton->setIcon(act->icon());
            connect(cornerButton, clicked(), cornerButtonClicked()));
            setCornerWidget(cornerButton);
            cornerButton->setCursor(ArrowCursor);
        }
    }
    else {
        setCornerWidget(0);
    }
}

void
cornerButtonClicked(void)
{
    log_debug("Corner Button Clicked.");
    actionHash.value(settings.display_scrollbar_widget_num)->trigger();
}

void
selection_changed(void)
{
    if (dockPropEdit->isVisible()) {
        dockPropEdit->set_selectedItems( active_view->selected);
    }
}

void
mouse_double_click_event(int* event)
{
    if (event->button() == EMB_LEFT_BUTTON) {
        EmbGeometry* item = active_view->itemAt(map_to_scene(event->pos()), QTransform());
        if (item) {
            dockPropEdit->show();
        }
    }
}

void
mouse_press_event(int* event)
{
    updateMouseCoords(event->x, event->y);
    if (event->button() == EMB_LEFT_BUTTON) {
        if (is_command_active()) {
            EmbVector cmdPoint = map_to_scene(event->pos());
            run_command_click(activeCommand(), cmdPoint.x, cmdPoint.y);
            return;
        }
        EmbPath path;
        EmbGeometry *pickList = active_view->items(
            EmbRect(
                map_to_scene(viewMousePoint.x-pickBoxSize,
                    viewMousePoint.y-pickBoxSize),
                map_to_scene(viewMousePoint.x+pickBoxSize,
                    viewMousePoint.y+pickBoxSize)));

        bool itemsInPickBox = pickList.size();
        if (itemsInPickBox && !selectingActive && !grippingActive) {
            bool itemsAlreadySelected = pickList.at(0)->isSelected();
            if (!itemsAlreadySelected) {
                pickList.at(0)->set_selected(true);
            }
            else {
                bool foundGrip = false;
                EmbGeometry* base = static_cast<EmbGeometry*>(pickList.at(0)); //\todo Allow multiple objects to be gripped at once
                if (!base) return;

                EmbVector qsnapOffset(qsnapLocatorSize, qsnapLocatorSize);
                EmbVector gripPoint = base->mouseSnapPoint(sceneMousePoint);
                EmbVector p1 = map_from_scene(gripPoint) - qsnapOffset;
                EmbVector q1 = map_from_scene(gripPoint) + qsnapOffset;
                EmbRect gripRect = EmbRect(map_to_scene(p1), map_to_scene(q1));
                EmbRect pickRect = EmbRect(
                    map_to_scene(viewMousePoint.x-pickBoxSize,
                        viewMousePoint.y-pickBoxSize),
                    map_to_scene(viewMousePoint.x+pickBoxSize,
                        viewMousePoint.y+pickBoxSize));
                if (gripRect.intersects(pickRect))
                    foundGrip = true;

                //If the pick point is within the item's grip box, start gripping
                if (foundGrip) {
                    startGripping(base);
                }
                // start moving
                else {
                    movingActive = true;
                    pressPoint = event->pos();
                    scenePressPoint = map_to_scene(pressPoint);
                }
            }
        }
        else if (grippingActive) {
            stopGripping(true);
        }
        else if (!selectingActive) {
            selectingActive = true;
            pressPoint = event->pos();
            scenePressPoint = map_to_scene(pressPoint);

            if (!selectBox)
                selectBox = SelectBox(QRubberBand_Rectangle, this);
            selectBox->setGeometry(EmbRect(pressPoint, pressPoint));
            selectBox->show();
        }
        else {
            selectingActive = false;
            selectBox->hide();
            releasePoint = event->pos();
            sceneReleasePoint = map_to_scene(releasePoint);

            //Start SelectBox Code
            path.addPolygon(map_to_scene(selectBox->geometry()));
            if (sceneReleasePoint.x > scenePressPoint.x) {
                if (settings.selection_mode_pickadd) {
                    if (isShiftPressed()) {
                        EmbGeometry *item_list = active_view->items(path, ContainsItemShape);
                        for (EmbGeometry* item, item_list)
                            item->set_selected(false);
                    }
                    else {
                        EmbGeometry*> item_list = active_view->items(path, ContainsItemShape);
                        for (EmbGeometry* item, item_list)
                            item->set_selected(true);
                    }
                }
                else {
                    if (isShiftPressed()) {
                        EmbGeometry *item_list = active_view->items(path, ContainsItemShape);
                        if (!item_list.size())
                            clear_selection();
                        else {
                            for (EmbGeometry* item, item_list)
                                item->set_selected(!item->isSelected()); //Toggle selected
                        }
                    }
                    else {
                        clear_selection();
                        EmbGeometry *item_list = active_view->items(path, ContainsItemShape);
                        for (EmbGeometry* item, item_list)
                            item->set_selected(true);
                    }
                }
            }
            else {
                if (settings.selection_mode_pickadd) {
                    if (isShiftPressed()) {
                        EmbGeometry *item_list = active_view->items(path, IntersectsItemShape);
                        for(EmbGeometry* item, item_list)
                            item->set_selected(false);
                    }
                    else {
                        EmbGeometry *item_list = active_view->items(path, IntersectsItemShape);
                        for (EmbGeometry* item, item_list)
                            item->set_selected(true);
                    }
                }
                else {
                    if (isShiftPressed()) {
                        EmbGeometry *item_list = active_view->items(path, IntersectsItemShape);
                        if (!item_list.size())
                            clear_selection();
                        else {
                            for (EmbGeometry* item, item_list)
                                item->set_selected(!item->isSelected()); //Toggle selected
                        }
                    }
                    else {
                        clear_selection();
                        EmbGeometry *item_list = active_view->items(path, IntersectsItemShape);
                        for (EmbGeometry* item, item_list)
                            item->set_selected(true);
                    }
                }
            }
            //End SelectBox Code
        }

        if (active_view->pasting_active) {
            EmbGeometry* item_list = pasteObjectItemGroup->childItems();
            active_view->destroyItemGroup(pasteObjectItemGroup);
            for (EmbGeometry* item, item_list) {
                active_view->removeItem(item); //Prevent Qt Runtime Warning, QGraphicsScene_addItem: item has already been added to this scene
            }

            undo_stack->beginMacro("Paste");
            for (EmbGeometry* item, item_list) {
                EmbGeometry* base = static_cast<EmbGeometry*>(item);
                if (base) {
                    UndoableAddCommand* cmd = UndoableAddCommand(base->data(OBJ_NAME).toString(), base, 0);
                    if (cmd) undo_stack->push(cmd);
                }
            }
            undo_stack->end_macro();

            active_view->pasting_active = false;
            selectingActive = false;
        }
        if (zoomWindowActive) {
            fitInEmbView(path.boundingRect(), KeepAspectRatio);
            clear_selection();
        }
    }
    if (event->button() == EMB_MIDDLE_BUTTON) {
        panStart(event->pos());
        //The Undo command will record the spot where the pan started.
        UndoableNavCommand* cmd = UndoableNavCommand("PanStart", 0);
        undo_stack->push(cmd);
        event->accept();
    }
}

void
panStart(EmbVector point)
{
    recalculateLimits();

    alignScenePointWithEmbViewPoint(map_to_scene(point), point);

    panningActive = true;
    panStartX = point.x;
    panStartY = point.y;
}

/**
 * NOTE:
 * Increase the sceneRect limits if the point we want to go to lies outside of sceneRect's limits
 * If the sceneRect limits aren't increased, you cannot pan past its limits
 */
void
recalculateLimits(void)
{
    EmbRect  viewRect(map_to_scene(rect().topLeft()), map_to_scene(rect().bottomRight()));
    EmbRect  sceneRect(active_view->sceneRect());
    EmbRect  newRect = viewRect.adjusted(-viewRect.width(), -viewRect.height(), viewRect.width(), viewRect.height());
    if (!sceneRect.contains(newRect.topLeft())
     || !sceneRect.contains(newRect.bottomRight())) {
        active_view->setSceneRect(
            sceneRect.adjusted(
                -viewRect.width(),
                -viewRect.height(),
                viewRect.width(),
                viewRect.height()));
    }
}

void
centerAt(EmbVector centerPoint)
{
    //centerOn also updates the scrollbars, which shifts things out of wack o_O
    centerOn(centerPoint);
    //Reshift to the new center
    EmbVector offset = centerPoint - center();
    EmbVector newCenter = centerPoint + offset;
    centerOn(newCenter);
}

void
alignScenePointWithEmbViewPoint(EmbVector scenePoint, EmbVector viewPoint)
{
    EmbVector viewCenter = center();
    EmbVector pointBefore = scenePoint;
    //centerOn also updates the scrollbars, which shifts things out of wack o_O
    centerOn(viewCenter);
    //Reshift to the new center so the scene and view points align
    EmbVector pointAfter = map_to_scene(viewPoint);
    EmbVector offset = pointBefore - pointAfter;
    EmbVector newCenter = viewCenter + offset;
    centerOn(newCenter);
}

void
mouseMoveEvent(int* event)
{
    updateMouseCoords(event->position().x, event->position().y);
    movePoint = event->pos();
    sceneMovePoint = map_to_scene(movePoint);

    if (is_command_active()) {
        if (rapidMoveActive) {
            runCommandMove(activeCommand(), sceneMovePoint.x, sceneMovePoint.y);
        }
    }
    if (previewActive) {
        if (previewMode == preview.MODE_MOVE) {
            previewObjectItemGroup->setPos(sceneMousePoint - previewPoint);
        }
        else if (previewMode == preview.MODE_ROTATE) {
            EmbReal mouseAngle = EmbLine(previewPoint, sceneMousePoint).angle();

            EmbVector p;
            EmbReal alpha = radians(previewData - mouseAngle);
            p.x = -previewPoint.x;
            p.y = -previewPoint.y;
            rot = embVector_rotate(p, alpha);
            rot = embVector_add(rot, previewPoint);

            previewObjectItemGroup->setPos(rot);
            previewObjectItemGroup->setRotation(previewData - mouseAngle);
        }
        else if (previewMode == preview.MODE_SCALE) {
            EmbReal scaleFactor = previewData;

            EmbReal factor = EmbLine(previewPoint, sceneMousePoint).length()/scaleFactor;

            previewObjectItemGroup->setScale(1);
            previewObjectItemGroup->setPos(0,0);

            if (scaleFactor <= 0.0) {
                critical_messagebox(translate("ScaleFactor Error"),
                                    translate("Hi there. If you are not a developer, report this as a bug. "
                                    "If you are a developer, your code needs examined, and possibly your head too."));
            }
            else {
                //Calculate the offset
                EmbVector old(0, 0);
                EmbLine scaleLine(preview.point, old);
                scaleLine.setLength(scaleLine.length()*factor);

                EmbVector delta = embVector_subtract(scaleLine.end, old);

                previewObjectItemGroup->setScale(previewObjectItemGroup->scale()*factor);
                previewObjectItemGroup->moveBy(delta);
            }
        }
    }
    if (active_view->pasting_active) {
        pasteObjectItemGroup->setPos(sceneMousePoint - pasteDelta);
    }
    if (movingActive) {
        //Ensure that the preview is only shown if the mouse has moved.
        if (!previewActive)
            previewOn(PREVIEW_CLONE_SELECTED, preview.MODE_MOVE, scenePressPoint.x, scenePressPoint.y, 0);
    }
    if (selectingActive) {
        if (sceneMovePoint.x >= scenePressPoint.x) {
            selectBox->setDirection(1);
        }
        else {
            selectBox->setDirection(0);
        }
        selectBox->setGeometry(EmbRect(map_from_scene(scenePressPoint), event->pos()).normalized());
        event->accept();
    }
    if (panningActive) {
        horizontalscrollbar()->setValue(horizontalscrollbar()->value() - (event->x - panStartX));
        verticalscrollbar()->setValue(verticalscrollbar()->value() - (event->y - panStartY));
        panStartX = event->x;
        panStartY = event->y;
        event->accept();
    }
}

void
mouseReleaseEvent(int* event)
{
    updateMouseCoords(event->x, event->y);
    if (event->button() == EMB_LEFT_BUTTON) {
        if (movingActive) {
            previewOff();
            EmbReal dx = sceneMousePoint.x-scenePressPoint.x;
            EmbReal dy = sceneMousePoint.y-scenePressPoint.y;
            //Ensure that moving only happens if the mouse has moved.
            if (dx || dy) moveSelected(dx, dy);
            movingActive = false;
        }
        event->accept();
    }
    if (event->button() == EMB_MIDDLE_BUTTON) {
        panningActive = false;
        //The Undo command will record the spot where the pan completed.
        UndoableNavCommand* cmd = UndoableNavCommand("PanStop", 0);
        undo_stack->push(cmd);
        event->accept();
    }
    if (event->button() == XButton1) {
        log_debug("XButton1");
        undo(); //\todo Make this customizable
        event->accept();
    }
    if (event->button() == XButton2) {
        log_debug("XButton2");
        redo(); //\todo Make this customizable
        event->accept();
    }
}

bool
allow_zoom_in(void)
{
    EmbVector origin = map_to_scene(0, 0);
    EmbVector corner = map_to_scene(width(), height());
    EmbVector max_dimension = embVector_subtract(corner, origin);

    if ((max_dimension.x < settings.zoomInLimit) || (max_dimension.y < settings.zoomInLimit)) {
        log_debug("ZoomIn limit reached. (limit=%.10f)", settings.zoomInLimit);
        return false;
    }

    return true;
}

bool
allow_zoom_out(void)
{
    EmbVector origin = map_to_scene(0, 0);
    EmbVector corner = map_to_scene(width(), height());
    EmbVector max_dimension = embVector_subtract(corner, origin);

    if ((max_dimension.x > zoomOutLimit) || (max_dimension.y > zoomOutLimit)) {
        log_debug("ZoomOut limit reached. (limit=%.1f)", zoomOutLimit);
        return false;
    }

    return true;
}

void
wheel_event(int* event)
{
    int zoomDir = 1; // \todo event->delta();
    EmbVector mousePoint = event->position();

    updateMouseCoords(mousePoint.x, mousePoint.y);
    if (zoomDir > 0) {
        UndoableNavCommand* cmd = UndoableNavCommand("ZoomInToPoint", 0);
        undo_stack->push(cmd);
    }
    else {
        UndoableNavCommand* cmd = UndoableNavCommand("ZoomOutToPoint", 0);
        undo_stack->push(cmd);
    }
}

void
zoom_to_point(EmbVector mousePoint, int zoomDir)
{
    EmbVector pointBeforeScale(map_to_scene(mousePoint));

    // Do The zoom
    EmbReal s = 1.0;
    if (zoomDir > 0) {
        if (!allowZoomIn()) {
            return;
        }
        s = settings.display_zoomscale_in;
    }
    else {
        if (!allowZoomOut()) {
            return;
        }
        s = settings.display_zoomscale_out;
    }

    scale(s, s);
    alignScenePointWithEmbViewPoint(pointBeforeScale, mousePoint);
    recalculateLimits();
    alignScenePointWithEmbViewPoint(pointBeforeScale, mousePoint);

    updateMouseCoords(mousePoint.x, mousePoint.y);
    if (active_view->pasting_active) {
        pasteObjectItemGroup->setPos(sceneMousePoint - pasteDelta);
    }
    if (selectingActive) {
        selectBox->setGeometry(EmbRect(map_from_scene(scenePressPoint), mousePoint).normalized());
    }
}

void
context_menu_event(void) // (QContextMenuEvent* event)
{
    QMenu menu;
    EmbGeometry* item_list =  active_view->selected;
    bool selectionEmpty = item_list.isEmpty();

    for (int i = 0; i < item_list.size(); i++) {
        if (item_list.at(i)->data(OBJ_TYPE) != EMB_NULL) {
            selectionEmpty = false;
            break;
        }
    }

    if (active_view->pasting_active) {
        return;
    }
    if (!prompt->is_command_active()) {
        char lastCmd[200] = prompt->lastCommand();
        QAction* repeat_action = QAction(load_icon(lastCmd), "Repeat " + lastCmd, this);
        repeat_action->setStatusTip("Repeats the previously issued command.");
        connect(repeat_action, triggered(), repeat_action()));
        menu.addAction(repeat_action);
    }
    if (zoomWindowActive) {
        QAction* cancelZoomWinAction = QAction("&Cancel (ZoomWindow)", this);
        cancelZoomWinAction->setStatusTip("Cancels the ZoomWindow Command.");
        connect(cancelZoomWinAction, triggered(), escapePressed()));
        menu.addAction(cancelZoomWinAction);
    }

    menu.addSeparator();
    menu.addAction(actionHash.value(ACTION_cut));
    menu.addAction(actionHash.value(ACTION_copy));
    menu.addAction(actionHash.value(ACTION_paste));
    menu.addSeparator();

    if (!selectionEmpty) {
        QAction* deleteAction = QAction(load_icon("erase"), "D&elete", this);
        deleteAction->setStatusTip("Removes objects from a drawing.");
        connect(deleteAction, triggered(), delete_selection()));
        menu.addAction(deleteAction);

        QAction* moveAction = QAction(load_icon("move"), "&Move", this);
        moveAction->setStatusTip("Displaces objects a specified distance in a specified direction.");
        connect(moveAction, triggered(), moveAction()));
        menu.addAction(moveAction);

        QAction* scaleAction = QAction(load_icon("scale"), "Sca&le", this);
        scaleAction->setStatusTip("Enlarges or reduces objects proportionally in the X, Y, and Z directions.");
        connect(scaleAction, triggered(), scaleAction()));
        menu.addAction(scaleAction);

        QAction* rotateAction = QAction(load_icon("rotate"), "R&otate", this);
        rotateAction->setStatusTip("Rotates objects about a base point.");
        connect(rotateAction, triggered(), rotateAction()));
        menu.addAction(rotateAction);

        menu.addSeparator();

        QAction* clearAction = QAction("Cle&ar Selection", this);
        clearAction->setStatusTip("Removes all objects from the selection set.");
        connect(clearAction, triggered(), clear_selection()));
        menu.addAction(clearAction);
    }

    menu.exec(event->globalPos());
}

void
delete_pressed(void)
{
    if (active_view->pasting_active) {
        active_view->removeItem(pasteObjectItemGroup);
        delete pasteObjectItemGroup;
    }
    active_view->pasting_active = false;
    zoomWindowActive = false;
    selectingActive = false;
    selectBox->hide();
    stopGripping(false);
    delete_selection();
}

/**
 * @brief \todo document this.
 *
 * In the actuator, this could be just "escape" which we emit when the escape key is pressed or any remapping of it.
 */
void
escape_pressed(void)
{
    if (active_view->pasting_active) {
        active_view->removeItem(pasteObjectItemGroup);
        delete pasteObjectItemGroup;
    }
    active_view->pasting_active = false;
    zoomWindowActive = false;
    selectingActive = false;
    selectBox->hide();
    if (grippingActive) {
        stopGripping(false);
    }
    else {
        clear_selection();
    }

    end_command();
}

void
start_gripping(EmbGeometry* obj)
{
    if (!obj) {
        return;
    }
    grippingActive = true;
    gripBaseObj = obj;
    sceneGripPoint = gripBaseObj->mouseSnapPoint(sceneMousePoint);
    gripBaseObj->setObjectRubberPoint("GRIP_POINT", sceneGripPoint);
    gripBaseObj->setObjectRubberMode(OBJ_RUBBER_GRIP);
}

void
stop_gripping(bool accept)
{
    grippingActive = false;
    if (gripBaseObj) {
        gripBaseObj->vulcanize();
        if (accept) {
            UndoableGripEditCommand* cmd = UndoableGripEditCommand(sceneGripPoint, sceneMousePoint, translate("Grip Edit ") + gripBaseObj->data(OBJ_NAME).toString(), gripBaseObj, 0);
            if (cmd) undo_stack->push(cmd);
            selectionChanged(); //Update the Property Editor
        }
        gripBaseObj = 0;
    }
    //Move the sceneGripPoint to a place where it will never be hot
    sceneGripPoint = sceneRect().topLeft();
}

/**
 * @brief Clear the current view's selection by ignoring the data contained in it.
 */
void
clear_selection(void)
{
    views[settings.pattern_index].n_selected = 0;
}

void
delete_selected(void)
{
    EmbGeometry *item_list =  active_view->selected;
    int  active_view->n_selected = item_list.size();
    if ( active_view->n_selected > 1) {
        undo_stack->beginMacro("Delete " + setNum(item_list.size()));
    }
    for (int i = 0; i < item_list.size(); i++) {
        if (item_list.at(i)->data(OBJ_TYPE) != EMB_NULL) {
            EmbGeometry* base = static_cast<EmbGeometry*>(item_list.at(i));
            if (base) {
                UndoableDeleteCommand* cmd = UndoableDeleteCommand(translate("Delete 1 ") + base->data(OBJ_NAME).toString(), base, 0);
                if (cmd)
                undo_stack->push(cmd);
            }
        }
    }
    if ( active_view->n_selected > 1) {
        undo_stack->end_macro();
    }
}

void
cut(void)
{
    if (active_view->n_selected == 0) {
        information_messagebox(translate("Cut Preselect"), translate("Preselect objects before invoking the cut command."));
        return; //\todo Prompt to select objects if nothing is preselected
    }

    undo_stack->beginMacro("Cut");
    copy_selection();
    delete_selection();
    undo_stack->end_macro();
}

/**
 * \todo Prompt to select objects if nothing is preselected.
 */
void
copy(void)
{
    if (!views[settings.pattern_index].n_selected) {
            information_messagebox(translate("Copy Preselect"),
            translate("Preselect objects before invoking the copy command."));
            return;
    }

    copy_selection();
    clear_selection();
}

/**
 * Prevent memory leaks by deleting any unpasted instances
 */
void
copy_selection(void)
{
    EmbGeometry *selectedList =  active_view->selected;

    delete_all(cutCopyObjectList.begin(), cutCopyObjectList.end());
    cutCopyObjectList.clear();

    /* Create new objects but do not add them to the scene just yet.
     * By creating them now, ensures that pasting will still work
     * if the original objects are deleted before the paste occurs.
     */
    cutCopyObjectList = createObjectList(selectedList);
}

void
paste(void)
{
    if (active_view->pasting_active) {
        active_view->removeItem(pasteObjectItemGroup);
        delete pasteObjectItemGroup;
    }

    pasteObjectItemGroup = active_view->createItemGroup(cutCopyObjectList);
    pasteDelta = pasteObjectItemGroup->boundingRect().bottomLeft();
    pasteObjectItemGroup->setPos(sceneMousePoint - pasteDelta);
    active_view->pasting_active = true;*/

    /* Re-create the list in case of multiple pastes. */
    cutCopyObjectList = createObjectList(cutCopyObjectList);
}

/**
 * @brief
 *
 * \a list
 * Returns EmbGeometry*
 *
 * \todo cut/copy blocks
 *
 * \todo cut/copy aligned dimensions
 *
 * \todo cut/copy elliptical arcs
 *
 * \todo cut/copy images
 *
 * \todo cut/copy infinite lines
 *
 * \todo cut/copy linear dimensions
 *
 * \todo cut/copy ordinate dimensions
 *
 * \todo cut/copy radius dimensions
 */
EmbArray *
createObjectList(EmbArray *list)
{
    int i;
    EmbArray *copyList = embArray_create(EMB_LINE);

    for (i=0; i < list->count; i++) {
            EmbGeometry* item = list.at(i);
        if (!item) {
            continue;
        }

        int objType = item->data(OBJ_TYPE).toInt();

        switch (objType) {
        case EMB_ARC: {
            ArcObject* arcObj = static_cast<ArcObject*>(item);
            if (arcObj) {
                ArcObject* copyArcObj = ArcObject(arcObj);
                copyList.append(copyArcObj);
            }
            break;
        }
        case EMB_BLOCK: {
            break;
        }
        case EMB_CIRCLE: {
            CircleObject* circObj = static_cast<CircleObject*>(item);
            if (circObj) {
                CircleObject* copyCircObj = CircleObject(circObj);
                copyList.append(copyCircObj);
            }
            break;
        }
        case EMB_DIM_ALIGNED: {
            break;
        }
        case EMB_DIM_ANGULAR: {
            //\todo cut/copy angular dimensions
            break;
        }
        case EMB_DIM_ARCLENGTH: {
            //\todo cut/copy arclength dimensions
            break;
        }
        case EMB_DIM_DIAMETER: {
            //\todo cut/copy diameter dimensions
            break;
        }
        case EMB_DIM_LEADER: {
            EmbDimLeader* dimLeaderObj = static_cast<EmbDimLeader*>(item);
            if (dimLeaderObj) {
                EmbDimLeader* copyDimLeaderObj = EmbDimLeader(dimLeaderObj);
                copyList.append(copyDimLeaderObj);
            }
            break;
        }
        case EMB_DIMLINEAR: {
            break;
        }
        case EMB_DIMORDINATE: {
            break;
        }
        case EMB_DIMRADIUS: {
            break;
        }
        case EMB_ELLIPSE: {
            EmbEllipse* elipObj = static_cast<EmbEllipse*>(item);
            if (elipObj) {
                EmbEllipse* copyElipObj = EmbEllipse(elipObj);
                copyList.append(copyElipObj);
            }
            break;
        }
        case EMB_ELLIPSEARC: {
            break;
        }
        case EMB_IMAGE: {
            break;
        }
        case EMB_INFINITELINE: {
            break;
        }
        case EMB_LINE: {
            LineObject* lineObj = static_cast<LineObject*>(item);
            if (lineObj) {
                LineObject* copyLineObj = LineObject(lineObj);
                copyList.append(copyLineObj);
            }
            break;
        }
        case EMB_PATH: {
            PathObject* pathObj = item.path;
            if (pathObj) {
                PathObject* copyPathObj = PathObject(pathObj);
                copyList.append(copyPathObj);
            }
            break;
        }
        case EMB_POINT: {
            PointObject* pointObj = item.point;
            if (pointObj) {
                PointObject* copyPointObj = PointObject(pointObj);
                copyList.append(copyPointObj);
            }
            break;
        }
        case EMB_POLYGON: {
            EmbPolygon* pgonObj = item.polygon;
            if (pgonObj) {
                EmbPolygon* copyPgonObj = EmbPolygon(pgonObj);
                copyList.append(copyPgonObj);
            }
            break;
        }
        case EMB_POLYLINE: {
            PolylineObject* plineObj = static_cast<PolylineObject*>(item);
            if (plineObj) {
                PolylineObject* copyPlineObj = PolylineObject(plineObj);
                copyList.append(copyPlineObj);
            }
            break;
        }
        case EMB_RAY: {
            //\todo cut/copy rays
            break;
        }
        case EMB_RECT: {
            RectObject* rectObj = static_cast<RectObject*>(item);
            if (rectObj) {
                RectObject* copyRectObj = RectObject(rectObj);
                copyList.append(copyRectObj);
            }
            break;
        }
        case EMB_TEXT_SINGLE: {
            TextSingleObject* textObj = static_cast<TextSingleObject*>(item);
            if (textObj) {
                TextSingleObject* copyTextObj = TextSingleObject(textObj);
                copyList.append(copyTextObj);
            }
            break;
        }
        default:
            break;
        }
        }

    return copyList;
}

void
repeat_action(void)
{
    prompt->end_command();
    prompt->set_current_text(prompt->lastCommand());
    prompt->process_input(Key_Return);
}

void
moveAction(void)
{
    prompt->end_command();
    prompt->set_current_text("move");
    prompt->process_input(Key_Return);
}

void
moveSelected(EmbReal dx, EmbReal dy)
{
    EmbGeometry*item_list =  active_view->selected;
    int  active_view->n_selected = item_list.size();
    if ( active_view->n_selected > 1)
        undo_stack->beginMacro("Move " + setNum(item_list.size()));
    for (EmbGeometry* item, item_list) {
        EmbGeometry* base = static_cast<EmbGeometry*>(item);
        if (base)
        {
            UndoableMoveCommand* cmd = UndoableMoveCommand(dx, dy, translate("Move 1 ") + base->data(OBJ_NAME).toString(), base, 0);
            if (cmd) undo_stack->push(cmd);
        }
    }
    if ( active_view->n_selected > 1)
        undo_stack->end_macro();

    //Always clear the selection after a move
    active_view->clear_selection();
}

void
rotateAction(void)
{
    prompt->end_command();
    prompt->set_current_text("rotate");
    prompt->process_input(Key_Return);
}

void
rotateSelected(EmbReal x, EmbReal y, EmbReal rot)
{
    if (active_view == NULL) {
        return;
    }

    EmbGeometry *item_list =  active_view->selected;
    if (active_view->n_selected > 1) {
        undo_stack->beginMacro("Rotate " + setNum(item_list.size()));
    }

    for (EmbGeometry* item, item_list) {
        EmbGeometry* base = static_cast<EmbGeometry*>(item);
        if (base)
        {
            UndoableRotateCommand* cmd = UndoableRotateCommand(x, y, rot, translate("Rotate 1 ") + base->data(OBJ_NAME).toString(), base, 0);
            if (cmd) undo_stack->push(cmd);
        }
    }

    if ( active_view->n_selected > 1) {
        undo_stack->end_macro();
    }

    /* Always clear the selection after a rotate */
    active_view->n_selected = 0;
}

void
mirrorSelected(EmbReal x1, EmbReal y1, EmbReal x2, EmbReal y2)
{
    if (active_view == NULL) {
        return;
    }

    EmbGeometry *item_list = active_view->selected;
    if (active_view->n_selected > 1) {
        undo_stack->beginMacro("Mirror " + setNum(item_list.size()));
    }

    for (EmbGeometry* item, item_list) {
        EmbGeometry* base = static_cast<EmbGeometry*>(item);
        if (base)
        {
            UndoableMirrorCommand* cmd = UndoableMirrorCommand(x1, y1, x2, y2, translate("Mirror 1 ") + base->data(OBJ_NAME).toString(), base, 0);
            if (cmd) undo_stack->push(cmd);
        }
    }

    if (active_view->n_selected > 1) {
        undo_stack->end_macro();
    }

    /* Always clear the selection after a mirror */
    active_view->n_selected = 0;
}

void
scaleAction(void)
{
    prompt->end_command();
    prompt->set_current_text("scale");
    prompt->process_input(Key_Return);
}

void
scaleSelected(EmbReal x, EmbReal y, EmbReal factor)
{
    if (active_view == NULL) {
        return;
    }

    EmbGeometry* item_list =  active_view->selected;
    if (active_view->n_selected > 1) {
        undo_stack->beginMacro("Scale " + setNum(item_list.size()));
    }

    for (EmbGeometry* item, item_list) {
        EmbGeometry* base = static_cast<EmbGeometry*>(item);
        if (base)
        {
            UndoableScaleCommand* cmd = UndoableScaleCommand(x, y, factor, translate("Scale 1 ") + base->data(OBJ_NAME).toString(), base, 0);
            if (cmd) undo_stack->push(cmd);
        }
    }

    if (active_view->n_selected > 1) {
        undo_stack->end_macro();
    }

    /* Always clear the selection after a scale */
    active_view->n_selected = 0;
}

void
showscrollbars(bool val)
{
    if (val) {
        setHorizontalscrollbarPolicy(SCROLLBAR_ALWAYS_ON);
        setVerticalscrollbarPolicy(SCROLLBAR_ALWAYS_ON);
    }
    else {
        setHorizontalscrollbarPolicy(SCROLLBAR_ALWAYS_OFF);
        setVerticalscrollbarPolicy(SCROLLBAR_ALWAYS_OFF);
    }
}

void
setCrossHairColor(unsigned int color)
{
    crosshairColor = color;
    active_view->setProperty(VIEW_COLOR_CROSSHAIR, color);
    if (active_view) active_view->update();
}

void
setBackgroundColor(EmbColor color)
{
    setBackgroundBrush(EmbColor(color));
    active_view->setProperty("VIEW_COLOR_BACKGROUND", color);
    if (active_view) active_view->update();
}

void
setSelectBoxColors(
    unsigned int colorL,
    unsigned int fillL,
    unsigned int colorR,
    unsigned int fillR,
    int alpha)
{
    // selectBox->set_colors(EmbColor(colorL), EmbColor(fillL), EmbColor(colorR), EmbColor(fillR), alpha);
}

void
selectbox_SelectBox(void) //Shape s, QWidget* parent) : QRubberBand(s, parent)
{
    //Default values
    set_colors(EmbColor(darkGreen), EmbColor(green), EmbColor(darkBlue), EmbColor(blue), 32);
}

void
selectbox_setDirection(int dir)
{
    if (!dir) {
        dirPen = leftPen;
        dirBrush = leftBrush;
    }
    else {
        dirPen = rightPen;
        dirBrush = rightBrush;
    }
    boxDir = dir;
}

void
selectbox_set_colors(
    EmbColor colorL,
    EmbColor fillL,
    EmbColor colorR,
    EmbColor fillR,
    int newAlpha)
{
    alpha = newAlpha;

    leftPenColor = colorL; //\todo allow customization
    leftBrushColor = EmbColor(fillL.red(), fillL.green(), fillL.blue(), alpha);
    rightPenColor = colorR; //\todo allow customization
    rightBrushColor = EmbColor(fillR.red(), fillR.green(), fillR.blue(), alpha);

    leftPen.setColor(leftPenColor);
    leftPen.setStyle(DashLine);
    leftBrush.setStyle(SolidPattern);
    leftBrush.setColor(leftBrushColor);

    rightPen.setColor(rightPenColor);
    rightPen.setStyle(SolidLine);
    rightBrush.setStyle(SolidPattern);
    rightBrush.setColor(rightBrushColor);

    if (!boxDir) { dirPen = leftPen;  dirBrush = leftBrush;  }
    else        { dirPen = rightPen; dirBrush = rightBrush; }
}

void
selectbox_paintEvent(int* paint_event)
{
    QPainter painter(this);
    painter.setPen(dirPen);
    painter.fillRect(0,0,width()-1, height()-1, dirBrush);
    painter.drawRect(0,0,width()-1, height()-1);
}

/**
 * \todo Load precisionAngle from settings and provide function for updating from settings
 * \todo Load precisionLength from settings and provide function for updating from settings
 */
void
property_editor(void)
{
    if (n_views == 0) {
        return;
    }

    EmbPattern *pattern = views[settings.pattern_index].pattern;

    char msg[200];
    sprintf(msg, "%s%s",
        translate("Property Editor"),
        views[settings.pattern_index].filename);
    ImGui::BeginChild(msg);
    translated_text("Properties");
    propertyEditorButtonStyle = ToolButtonTextBesideIcon; //\todo Make customizable

    pickAdd = pickAddMode;

    precisionAngle  = 0;
    precisionLength = 4;

    if (pattern) {
        create_group_box_general(settings.pattern_index, pattern);
    }

    for (int i=0; i<pattern->geometry->count; i++) {
        if (pattern->geometry->geometry[i].type == EMB_ARC) {
            EmbArc *arc = &(pattern->geometry->geometry[i].object.arc);
            create_group_box_arc(i, arc);
        }
    }

    if (0) {
        create_group_box_block(0, NULL);
    }

    for (int i=0; i<pattern->geometry->count; i++) {
        if (pattern->geometry->geometry[i].type == EMB_CIRCLE) {
            EmbCircle *circle = &(pattern->geometry->geometry[i].object.circle);
            create_group_box_circle(i, circle);
        }
    }

    if (0) {
        create_group_box_dim_aligned(0, NULL);
    }

    if (0) {
        create_group_box_dim_angular(0, NULL);
    }

    if (0) {
        create_group_box_dim_arc_length(0, NULL);
    }

    if (0) {
        create_group_box_dim_diameter(0, NULL);
    }

    if (0) {
        create_group_box_dim_leader(0, NULL);
    }

    if (0) {
        create_group_box_dim_linear(0, NULL);
    }

    if (0) {
        create_group_box_dim_ordinate(0, NULL);
    }

    if (0) {
        create_group_box_dim_radius(0, NULL);
    }

    for (int i=0; i<pattern->geometry->count; i++) {
        if (pattern->geometry->geometry[i].type == EMB_ELLIPSE) {
            EmbEllipse *ellipse = &(pattern->geometry->geometry[i].object.ellipse);
            create_group_box_ellipse(i, ellipse);
        }
    }

    if (0) {
        create_group_box_image(0, NULL);
    }

    if (0) {
        create_group_box_infinite_line(0, NULL);
    }

    for (int i=0; i<pattern->geometry->count; i++) {
        if (pattern->geometry->geometry[i].type == EMB_LINE) {
            EmbLine *line = &(pattern->geometry->geometry[i].object.line);
            create_group_box_line(i, line);
        }
    }

    for (int i=0; i<pattern->geometry->count; i++) {
        if (pattern->geometry->geometry[i].type == EMB_PATH) {
            EmbPath *path = &(pattern->geometry->geometry[i].object.path);
            create_group_box_path(i, path);
        }
    }

    for (int i=0; i<pattern->geometry->count; i++) {
        if (pattern->geometry->geometry[i].type == EMB_POINT) {
            EmbPoint *point = &(pattern->geometry->geometry[i].object.point);
            create_group_box_point(i, point);
        }
    }

    for (int i=0; i<pattern->geometry->count; i++) {
        if (pattern->geometry->geometry[i].type == EMB_POLYGON) {
            EmbPolygon *polygon = &(pattern->geometry->geometry[i].object.polygon);
            create_group_box_polygon(i, polygon);
        }
    }

    for (int i=0; i<pattern->geometry->count; i++) {
        if (pattern->geometry->geometry[i].type == EMB_POLYLINE) {
            EmbPolyline *polyline = &(pattern->geometry->geometry[i].object.polyline);
            create_group_box_polyline(i, polyline);
        }
    }

    if (0) {
        create_group_box_ray(0, NULL);
    }

    for (int i=0; i<pattern->geometry->count; i++) {
        if (pattern->geometry->geometry[i].type == EMB_RECT) {
            EmbRect *rect = &(pattern->geometry->geometry[i].object.rect);
            create_group_box_rectangle(i, rect);
        }
    }

    if (pattern->stitch_list->count > 0) {
        create_group_box_stitch_list(settings.pattern_index, pattern->stitch_list);
    }

    if (0) {
        create_group_box_text_multi(0, NULL);
    }

    if (0) {
        create_group_box_text_single(0, NULL);
    }

    ImGui::EndChild();
}

bool
property_editor_eventFilter(void)
{
    if (event->type() == QEvent_KeyPress) {
        QKeyEvent* pressedKey = (QKeyEvent*)event;
        int key = pressedKey->key();
        switch(key)
        {
            case Key_Escape:
                if (focusWidget)
                    focusWidget->setFocus(OtherFocusReason);
                return true;
                break;
            default:
                pressedKey->ignore();
        }
    }
    return QObject_eventFilter(obj, event);
    return true;
}

/**
 * @brief
 *
 * Returns int*
 */
int*
property_editor_createComboBoxSelected(void)
{
    comboBoxSelected = int(this);
    comboBoxSelected->addItem(translate("No Selection"));
    return comboBoxSelected;
    return NULL;
}

int*
property_editor_create_tool_buttonQSelect(void)
{
    toolButtonQSelect = QToolButton(this);
    toolButtonQSelect->setIcon(QIcon(iconDir + "/" + "quickselect" + ".png"));
    toolButtonQSelect->setIconSize(QSize(iconSize, iconSize));
    toolButtonQSelect->setText("QSelect");
    toolButtonQSelect->setToolTip("QSelect"); //\todo Better Description
    toolButtonQSelect->setToolButtonStyle(ToolButtonIconOnly);
    return toolButtonQSelect;
    return NULL;
}

int*
property_editor_create_tool_buttonPickAdd(void)
{
    //\todo Set as PickAdd or PickNew based on settings
    toolButtonPickAdd = QToolButton(this);
    updatePickAddModeButton(pickAdd);
    connect(toolButtonPickAdd, clicked(bool), togglePickAddMode()));
    return toolButtonPickAdd;
    return NULL;
}

void
property_editor_updatePickAddModeButton(bool pickAddMode)
{
    pickAdd = pickAddMode;
    if (pickAdd) {
        toolButtonPickAdd->setIcon(QIcon(iconDir + "/pickadd.png"));
        toolButtonPickAdd->setIconSize(QSize(iconSize, iconSize));
        toolButtonPickAdd->setText("PickAdd");
        toolButtonPickAdd->setToolTip("PickAdd Mode - Add to current selection.\nClick to switch to PickNew Mode.");
        toolButtonPickAdd->setToolButtonStyle(ToolButtonIconOnly);
    }
    else {
        toolButtonPickAdd->setIcon(QIcon(iconDir + "/picknew.png"));
        toolButtonPickAdd->setIconSize(QSize(iconSize, iconSize));
        toolButtonPickAdd->setText("PickNew");
        toolButtonPickAdd->setToolTip("PickNew Mode - Replace current selection.\nClick to switch to PickAdd Mode.");
        toolButtonPickAdd->setToolButtonStyle(ToolButtonIconOnly);
    }
}

void
property_editor_togglePickAddMode(void)
{
    emit pickAddModeToggled();
}

void
property_editor_set_selectedItems(int *item_list, int length)
{
    selectedItemList = item_list;
    //Hide all the groups initially, then decide which ones to show
    hideAllGroups();
    comboBoxSelected->clear();

    QSet<int> typeSet;

    if (length == 0) {
        comboBoxSelected->addItem(translate("No Selection"));
        return;
    }

    int num_geometry[NUMBER_OF_OBJECT_TYPES];

    for (int i=0; i<length; i++) {
        typeSet.insert(objType);

        if (item_list[i].type < NUMBER_OF_OBJECT_TYPES) {
            num_geometry[item_list[i].type]++;
        }
        else {
            num_geometry[OBJ_UNKNOWN]++;
        }
    }

    int numTypes = typeSet.size();

    // Populate the selection comboBox
    if (numTypes > 1) {
        comboBoxSelected->addItem(translate("Varies"), length) + ")");
        connect(comboBoxSelected, currentIndexChanged(int), showOneType(int)));
    }

    char object_labels[][200] = {
        "Arc",
        "Block",
        "Circle",
        "Aligned Dimension"
    };

    char *comboBoxStr[200];
    for (int objType=0; objType < EMB_UNKNOWN; objType++) {
        switch (objType) {
        case EMB_ARC:
            sprintf(comboBoxStr, "%s(%d)", translate("Arc"), numArc);
            break;
        case EMB_BLOCK:
            sprintf(comboBoxStr, "%s(%d)", translate("Block"), numBlock);
            break;
        case EMB_CIRCLE:
            sprintf(comboBoxStr, "%s(%d)", translate("Circle"), numCircle);
            break;
        case EMB_DIM_ALIGNED:
            sprintf(comboBoxStr, "%s(%d)", translate("Aligned Dimension"), numDimAlign);
            break;
        case EMB_DIM_ANGULAR:
            sprintf(comboBoxStr, "%s(%d)", translate("Angular Dimension"), numDimAngular);
            break;
        case EMB_DIM_ARCLENGTH:
            sprintf(comboBoxStr, "%s(%d)", translate("Arclength Dimension"), numDimArcLen);
            break;
        case EMB_DIM_DIAMETER:
            sprintf(comboBoxStr, "%s(%d)", translate("Diameter Dimension"), numDimDiam);
            break;
        case EMB_DIM_LEADER:
            sprintf(comboBoxStr, "%s(%d)", translate("Leader Dimension"), numDimLeader);
            break;
        case EMB_DIM_LINEAR:
            sprintf(comboBoxStr, "%s(%d)", translate("Linear Dimension"), numDimLinear);
            break;
        case EMB_DIM_ORDINATE:
            sprintf(comboBoxStr, "%s(%d)", translate("Ordinate Dimension"), numDimOrd);
            break;
        case EMB_DIM_RADIUS:
            sprintf(comboBoxStr, "%s(%d)", translate("Radius Dimension"), numDimRadius);
            break;
        case EMB_ELLIPSE:
            sprintf(comboBoxStr, "%s(%d)", translate("Ellipse"), numEllipse);
            break;
        case EMB_IMAGE:
            sprintf(comboBoxStr, "%s(%d)", translate("Image"), numImage);
            break;
        case EMB_INFINITELINE:
            sprintf(comboBoxStr, "%s(%d)", translate("Infinite Line"), numInfLine);
            break;
        case EMB_LINE:
            sprintf(comboBoxStr, "%s(%d)", translate("Line"), numLine);
            break;
        case EMB_PATH:
            sprintf(comboBoxStr, "%s(%d)", translate("Path"), numPath);
            break;
        case EMB_POINT:
            sprintf(comboBoxStr, "%s(%d)", translate("Point"), numPoint);
            break;
        case EMB_POLYGON:
            sprintf(comboBoxStr, "%s(%d)", translate("Polygon"), numPolygon);
            break;
        case EMB_POLYLINE:
            sprintf(comboBoxStr, "%s(%d)", translate("Polyline"), numPolyline);
            break;
        case EMB_RAY:
            sprintf(comboBoxStr, "%s(%d)", translate("Ray"), numRay);
            break;
        case EMB_RECT:
            sprintf(comboBoxStr, "%s(%d)", translate("Rectangle"), numRect);
            break;
        case EMB_TEXT_MULTI:
            sprintf(comboBoxStr, "%s(%d)", translate("Multiline Text"), numTextMulti);
            break;
        case EMB_TEXT_SINGLE:
            sprintf(comboBoxStr, "%s(%d)", translate("Text"), numTextSingle);
            break;
        default:
            sprintf(comboBoxStr, "%s(%d)", translate("Unknown"), numUnknown);
            break;
        }

        comboBoxSelected->addItem(comboBoxStr, objType);
    }

    // Load Data into the fields

    // Clear fields first so if the selected data varies, the comparison is simple
    clearAllFields();

    for (UiObject item : item_list) {
        if (!item) {
            continue;
        }

        //\todo load data into the General field
        switch (item->type) {
        case EMB_ARC:
            ArcObject* obj = static_cast<ArcObject*>(item);
            create_group_box_arc(id, obj);
            break;
        case EMB_BLOCK:
            create_group_box_block(id, obj);
            break;
        case EMB_CIRCLE:
            CircleObject* obj = static_cast<CircleObject*>(item);
            create_group_box_circle(id, obj);
            break;
        case EMB_DIM_ALIGNED:
            create_group_box_dim_aligned(id, obj);
            break;
        case EMB_DIM_ANGULAR:
            create_group_box_dim_angular(id, obj);
        }
        case EMB_DIM_ARCLENGTH) {
            create_group_box_dim_arc_length(id, obj);
        }
        case EMB_DIM_DIAMETER) {
            create_group_box_dim_diameter(id, obj);
        }
        case EMB_DIM_LEADER) {
            create_group_box_dim_leader(id, obj);
            break;
        }
        case EMB_DIM_LINEAR: {
            create_group_box_dim_linear(id, obj);
            break;
        }
        case EMB_DIM_ORDINATE: {
            create_group_box_dim_ordinate(id, obj);
            break;
        }
        case EMB_DIM_RADIUS: {
            create_group_box_dim_radius(id, obj);
            break;
        }
        case EMB_ELLIPSE: {
            EmbEllipse* obj = static_cast<EmbEllipse*>(item);
            create_group_box_ellipse(id, obj);
            break;
        }
        case EMB_IMAGE: {
            create_group_box_image(id, obj);
            break;
        }
        case EMB_INFINITELINE: {
            create_group_box_infinite_line(id, obj);
            break;
        }
        case EMB_LINE: {
            create_group_box_line(id, obj);
            break;
        }
        case EMB_PATH: {
            create_group_box_path(id, obj);
            break;
        }
        case EMB_POINT: {
            create_group_box_point(id, obj);
            break;
        }
        case EMB_POLYGON: {
            create_group_box_polygon(id, obj);
            break;
        }
        case EMB_POLYLINE: {
            create_group_box_polyline(id, obj);
            break;
        }
        case EMB_RAY: {
            create_group_box_ray(id, obj);
            break;
        }
        case EMB_RECT: {
            create_group_box_rectangle(id, obj);
            break;
        }
        case EMB_TEXT_MULTI: {
            create_group_box_text_multi(id, obj);
            break;
        }
        case EMB_TEXT_SINGLE: {
            create_group_box_text_single(id, obj);
            break;
        }
        default:
            break;
        }
    }

    // Only show fields if all objects are the same type
    if (numTypes == 1) {
        for(int objType : typeSet) {
            showGroups(objType);
        }
    }
}

void
property_editor_updateLineEditStrIfVaries(int* lineEdit, const char * str)
{
    fieldOldText = lineEdit->text();
    fieldNewText = str;

    if     (fieldOldText.isEmpty())       lineEdit->setText(fieldNewText);
    else if (fieldOldText != fieldNewText) lineEdit->setText(fieldVariesText);
}

void
property_editor_updateLineEditNumIfVaries(
    int* lineEdit,
    EmbReal num,
    bool useAnglePrecision)
{
    int precision = 0;
    if (useAnglePrecision) precision = precisionAngle;
    else                  precision = precisionLength;

    fieldOldText = lineEdit->text();
    fieldNewText.setNum(num, 'f', precision);

    //Prevent negative zero :D
    const char *negativeZero = "-0.";
    for (int i = 0; i < precision; ++i)
        negativeZero.append('0');
    if (fieldNewText == negativeZero)
        fieldNewText = negativeZero.replace("-", "");

    if     (fieldOldText.isEmpty())       lineEdit->setText(fieldNewText);
    else if (fieldOldText != fieldNewText) lineEdit->setText(fieldVariesText);
}

void
property_editor_updateFontComboBoxStrIfVaries(
    int* fontComboBox,
    const char * str)
{
    fieldOldText = fontComboBox->property("FontFamily").toString();
    fieldNewText = str;
    //debug_message("old: %d %s, new: %d %s", oldIndex, fontComboBox->currentText()), newIndex, str));
    if (fieldOldText.isEmpty()) {
        fontComboBox->setCurrentFont(QFont(fieldNewText));
        fontComboBox->setProperty("FontFamily", fieldNewText);
    }
    else if (fieldOldText != fieldNewText) {
        // Prevent multiple entries
        if (fontComboBox->findText(fieldVariesText) == -1)
            fontComboBox->addItem(fieldVariesText);
        fontComboBox->setCurrentIndex(fontComboBox->findText(fieldVariesText));
    }
}

void
property_editor_updateComboBoxStrIfVaries(
    int* comboBox,
    const char *str,
    const char **strList)
{
    fieldOldText = comboBox->currentText();
    fieldNewText = str;

    if (fieldOldText.isEmpty()) {
        for (const char *s, strList) {
            comboBox->addItem(s, s);
        }
        comboBox->setCurrentIndex(comboBox->findText(fieldNewText));
    }
    else if (fieldOldText != fieldNewText) {
        if (comboBox->findText(fieldVariesText) == -1) //Prevent multiple entries
            comboBox->addItem(fieldVariesText);
        comboBox->setCurrentIndex(comboBox->findText(fieldVariesText));
    }
}

void
property_editor_updateComboBoxBoolIfVaries(int* comboBox, bool val, bool yesOrNoText)
{
    fieldOldText = comboBox->currentText();
    if (yesOrNoText) {
        if (val) fieldNewText = fieldYesText;
        else    fieldNewText = fieldNoText;
    }
    else {
        if (val) fieldNewText = fieldOnText;
        else    fieldNewText = fieldOffText;
    }

    if (fieldOldText.isEmpty()) {
        if (yesOrNoText) {
            comboBox->addItem(fieldYesText, true);
            comboBox->addItem(fieldNoText, false);
        }
        else {
            comboBox->addItem(fieldOnText, true);
            comboBox->addItem(fieldOffText, false);
        }
        comboBox->setCurrentIndex(comboBox->findText(fieldNewText));
    }
    else if (fieldOldText != fieldNewText) {
        if (comboBox->findText(fieldVariesText) == -1) //Prevent multiple entries
            comboBox->addItem(fieldVariesText);
        comboBox->setCurrentIndex(comboBox->findText(fieldVariesText));
    }
}

void
property_editor_showGroups(int objType)
{
    switch (objType) {
    case EMB_ARC:
        // groupBoxGeometryArc->show();
        // groupBoxMiscArc->show();
        break;
    case EMB_BLOCK:
        // groupBoxGeometryBlock->show();
        break;
    case EMB_CIRCLE:
        // groupBoxGeometryCircle->show();
        break;
    case EMB_DIM_ALIGNED:
        // groupBoxGeometryDimAligned->show();
        break;
    case EMB_DIM_ANGULAR:
        // groupBoxGeometryDimAngular->show();
        break;
    case EMB_DIM_ARCLENGTH:
        // groupBoxGeometryDimArcLength->show();
        break;
    case EMB_DIM_DIAMETER:
        // groupBoxGeometryDimDiameter->show();
        break;
    case EMB_DIM_LEADER:
        // groupBoxGeometryDimLeader->show();
        break;
    case EMB_DIM_LINEAR:
        // groupBoxGeometryDimLinear->show();
        break;
    case EMB_DIM_ORDINATE:
        // groupBoxGeometryDimOrdinate->show();
        break;
    case EMB_DIM_RADIUS:
        // groupBoxGeometryDimRadius->show();
        break;
    case EMB_ELLIPSE:
        // groupBoxGeometryEllipse->show();
        break;
    case EMB_IMAGE:
        // groupBoxGeometryImage->show();
        // groupBoxMiscImage->show();
        break;
    case EMB_INFINITELINE:
        // groupBoxGeometryInfiniteLine->show();
        break;
    case EMB_LINE:
        // groupBoxGeometryLine->show();
        break;
    case EMB_PATH:
        // groupBoxGeometryPath->show();
        // groupBoxMiscPath->show();
        break;
    case EMB_POINT:
        // groupBoxGeometryPoint->show();
        break;
    case EMB_POLYGON:
        // groupBoxGeometryPolygon->show();
        break;
    case EMB_POLYLINE:
        // groupBoxGeometryPolyline->show();
        // groupBoxMiscPolyline->show();
        break;
    case EMB_RAY:
        // groupBoxGeometryRay->show();
        break;
    case EMB_RECT:
        // groupBoxGeometryRectangle->show();
        break;
    case EMB_TEXT_MULTI:
        // groupBoxGeometryTextMulti->show();
        break;
    case EMB_TEXT_SINGLE:
        // groupBoxTextTextSingle->show();
        // groupBoxGeometryTextSingle->show();
        // groupBoxMiscTextSingle->show();
        break;
    default:
        break;
    }
}

void
property_editor_showOneType(int id)
{
    hideAllGroups();
    showGroups(comboBoxSelected->itemData(id).toInt());
}

void
property_editor_hideAllGroups(void)
{
    //NOTE: General group will never be hidden
    groupBoxGeometryArc->hide();
    groupBoxMiscArc->hide();
    groupBoxGeometryBlock->hide();
    groupBoxGeometryCircle->hide();
    groupBoxGeometryDimAligned->hide();
    groupBoxGeometryDimAngular->hide();
    groupBoxGeometryDimArcLength->hide();
    groupBoxGeometryDimDiameter->hide();
    groupBoxGeometryDimLeader->hide();
    groupBoxGeometryDimLinear->hide();
    groupBoxGeometryDimOrdinate->hide();
    groupBoxGeometryDimRadius->hide();
    groupBoxGeometryEllipse->hide();
    groupBoxGeometryImage->hide();
    groupBoxMiscImage->hide();
    groupBoxGeometryInfiniteLine->hide();
    groupBoxGeometryLine->hide();
    groupBoxGeometryPath->hide();
    groupBoxMiscPath->hide();
    groupBoxGeometryPoint->hide();
    groupBoxGeometryPolygon->hide();
    groupBoxGeometryPolyline->hide();
    groupBoxMiscPolyline->hide();
    groupBoxGeometryRay->hide();
    groupBoxGeometryRectangle->hide();
    groupBoxGeometryTextMulti->hide();
    groupBoxTextTextSingle->hide();
    groupBoxGeometryTextSingle->hide();
    groupBoxMiscTextSingle->hide();
}

/**
 * @brief Create a group box general object.
 */
void
create_group_box_general(int id, EmbPattern *pattern)
{
    char label[200];
    sprintf(label, "%s##%d", translate("General"), id+1);
    printf("%d", pattern->stitch_list->count);
    if (ImGui::CollapsingHeader(label)) {
            if (ImGui_ComboBox("Layer")) {
            for (int i=0; i<) {

            }
        }
        if (ImGui_ComboBox("Color")) {
            for () {

            }
        }
        if (ImGui_ComboBox("LineType")) {
            for () {

            }
        }
        if (ImGui_ComboBox("LineWeight")) {
            for () {

            }
        }
        }
}

/**
 * @brief Create a group box arc object.
 */
void
create_group_box_arc(int id, EmbArc *arc)
{
    char label[200];
    sprintf(label, "%s ID: %d", translate("Arc"), id+1);
    if (ImGui::CollapsingHeader(label)) {
        translated_text("Geometry");

        show_real("Start X", arc->start.x);
        show_real("Start Y", arc->start.y);
        show_real("Mid X", arc->mid.x);
        show_real("Mid Y", arc->mid.y);
        show_real("End X", arc->end.x);
        show_real("End Y", arc->end.y);

            toolButtonArcCenterX    = create_tool_button("blank", translate("Center X"));
        toolButtonArcCenterY    = create_tool_button("blank", translate("Center Y"));
        toolButtonArcRadius     = create_tool_button("blank", translate("Radius"));
        toolButtonArcStartAngle = create_tool_button("blank", translate("Start Angle"));
        toolButtonArcEndAngle   = create_tool_button("blank", translate("End Angle"));
        toolButtonArcStartX     = create_tool_button("blank", translate("Start X"));
        toolButtonArcStartY     = create_tool_button("blank", translate("Start Y"));
        toolButtonArcEndX       = create_tool_button("blank", translate("End X"));
        toolButtonArcEndY       = create_tool_button("blank", translate("End Y"));
        toolButtonArcArea       = create_tool_button("blank", translate("Area"));
        toolButtonArcLength     = create_tool_button("blank", translate("Arc Length"));
        toolButtonArcChord      = create_tool_button("blank", translate("Chord"));
        toolButtonArcIncAngle   = create_tool_button("blank", translate("Included Angle"));

        lineEditArcCenterX    = create_line_edit("EmbReal", false);
        lineEditArcCenterY    = create_line_edit("EmbReal", false);
        lineEditArcRadius     = create_line_edit("EmbReal", false);
        lineEditArcStartAngle = create_line_edit("EmbReal", false);
        lineEditArcEndAngle   = create_line_edit("EmbReal", false);
        lineEditArcStartX     = create_line_edit("EmbReal", true);
        lineEditArcStartY     = create_line_edit("EmbReal", true);
        lineEditArcEndX       = create_line_edit("EmbReal", true);
        lineEditArcEndY       = create_line_edit("EmbReal", true);
        lineEditArcArea       = create_line_edit("EmbReal", true);
        lineEditArcLength     = create_line_edit("EmbReal", true);
        lineEditArcChord      = create_line_edit("EmbReal", true);
        lineEditArcIncAngle   = create_line_edit("EmbReal", true);

        show_real("Area", embArc);
        show_real("Chord", embVector_angle(delta));
        show_real("Included Angle", embVector_length(delta));
    
        translated_text("Misc");

            toolButtonArcClockwise = create_tool_button("blank", translate("Clockwise"));

        comboBoxArcClockwise = createComboBox(true);
        }
}

/**
 * @brief Create a group box block object.
 *
 * \todo editors for blocks.
 */
void
create_group_box_block(int id, EmbBlock *obj)
{
    char label[200];
    sprintf(label, "%s ID: %d", translate("Block"), id+1);
    if (ImGui::CollapsingHeader(label)) {
        translated_text("Geometry");
        printf("%f", obj->position.x);
            toolButtonBlockX = create_tool_button("blank", translate("Position X"));
        toolButtonBlockY = create_tool_button("blank", translate("Position Y"));

        lineEditBlockX = create_line_edit("EmbReal", false);
        lineEditBlockY = create_line_edit("EmbReal", false);*/
    }
}

/**
 * @brief Create a group box circle object
 *
 * \bug When the user selects multiple entries across different circles
 * then edits the program throws an error.
 */
void
create_group_box_circle(int id, EmbCircle *circle)
{
    char label[200];
    sprintf(label, "%s ID: %d", translate("Circle"), id+1);
    if (ImGui::CollapsingHeader(label)) {
        translated_text("Geometry");

        edit_real("Center X", &(circle->center.x));
        edit_real("Center Y", &(circle->center.y));
        edit_real("Radius", &(circle->radius));
    }
}

/**
 * @brief Create a group box dim aligned object
 *
 * \todo editors for aligned dimension.
 */
void
create_group_box_dim_aligned(int id, EmbAlignedDim *obj)
{
    char label[200];
    sprintf(label, "%s ID: %d", translate("Aligned Dimension"), id+1);
    if (ImGui::CollapsingHeader(label)) {
        translated_text("Geometry");
        printf("%f", obj->position.x);
    }
}

/* \todo editors for angular dimension.
 */

/**
 * @brief Create a group box dim angular object
 *
 */
void
create_group_box_dim_angular(int id, EmbAngularDim *obj)
{
    char label[200];
    sprintf(label, "%s ID: %d", translate("Angular Dimension"), id+1);
    if (ImGui::CollapsingHeader(label)) {
        translated_text("Geometry");
        printf("%f", obj->position.x);
    }
}

/* \todo editors for arc length dimension.
 */

/**
 * @brief Create a group box dim arc length object
 *
 */
void
create_group_box_dim_arc_length(int id, EmbArcLengthDim *obj)
{
    char label[200];
    sprintf(label, "%s ID: %d", translate("Arc Length Dimension"), id+1);
    if (ImGui::CollapsingHeader(label)) {
        translated_text("Geometry");
        printf("%f", obj->position.x);
    }
}

/* \todo editors for diameter dimension.
 */

/**
 * @brief Create a group box dim diameter object
 *
 */
void
create_group_box_dim_diameter(int id, EmbDiameterDim *dim_diameter)
{
    char label[200];
    sprintf(label, "%s ID: %d", translate("Diameter Dimension"), id+1);
    if (ImGui::CollapsingHeader(label)) {
        translated_text("Geometry");
        printf("%f", dim_diameter->position.x);
    }
}

/* \todo editors for dimension leader.
 */

/**
 * @brief Create a group box dim leader object
 *
 */
void
create_group_box_dim_leader(int id, EmbLeaderDim *obj)
{
    char label[200];
    sprintf(label, "%s ID: %d", translate("Dimension Leader"), id+1);
    if (ImGui::CollapsingHeader(label)) {
        translated_text("Geometry");
        printf("%f", obj->position.x);
    }
}

/**
 * @brief Create a group box dim linear object
 *
 * \todo editors for linear dimension.
 *
 */
void
create_group_box_dim_linear(int id, EmbLinearDim *obj)
{
    char label[200];
    sprintf(label, "%s ID: %d", translate("Linear Dimension"), id+1);
    if (ImGui::CollapsingHeader(label)) {
        translated_text("Geometry");
        printf("%f", obj->position.x);
    }
}

/**
 * @brief Create a group box dim ordinate object
 *
 * \todo editors for linear dimension.
 *
 */
void
create_group_box_dim_ordinate(int id, EmbOrdinateDim *obj)
{
    char label[200];
    sprintf(label, "%s ID: %d", translate("Ordinate Dimension"), id+1);
    if (ImGui::CollapsingHeader(label)) {
        translated_text("Geometry");
        printf("%f", obj->position.x);
    }
}

/**
 * @brief Create a group box dim radius object
 *
 * \todo editors for radius dimension.
 */
void
create_group_box_dim_radius(int id, EmbRadiusDim *dim_radius)
{
    char label[200];
    sprintf(label, "%s ID: %d", translate("Radius Dimension"), id+1);
    if (ImGui::CollapsingHeader(label)) {
        translated_text("Geometry");
        printf("%f", dim_radius->position.x);
    }
}

/**
 * @brief Create a group box ellipse object
 *
 */
void
create_group_box_ellipse(int id, EmbEllipse *ellipse)
{
    char label[200];
    sprintf(label, "%s ID: %d", translate("Ellipse"), id+1);
    if (ImGui::CollapsingHeader(label)) {
        translated_text("Geometry");

        edit_real("Center X", &(ellipse->center.x));
        edit_real("Center Y", &(ellipse->center.y));
        edit_real("Major Radius", &(ellipse->radius.x));
        edit_real("Minor Radius", &(ellipse->radius.y));
        show_real("Major Diameter", 2.0*ellipse->radius.x);
        show_real("Minor Diameter", 2.0*ellipse->radius.y);
    }
}

/**
 * @brief Create a group box image object
 *
 */
void
create_group_box_image(int id, EmbImage *image)
{
    char label[200];
    sprintf(label, "%s ID: %d", translate("Image"), id+1);
    if (ImGui::CollapsingHeader(label)) {
        translated_text("Geometry");

        edit_real(translate("Position X"), &(image->position.x));
        edit_real(translate("Position Y"), &(image->position.y));
        //edit_real(translate("Width"), &(image->width));
        //edit_real(translate("Height"), &(image->height));

        translated_text("Misc");

        translated_text("Name");
        ImGui::SameLine();
        translated_text(image->name);

        translated_text("Path");
        ImGui::SameLine();
        translated_text(image->path);
    }
}

/**
 * @brief Create a group box infinite line object.
 *
 */
void
create_group_box_infinite_line(int id, EmbInfiniteLine *obj)
{
    char label[200];
    sprintf(label, "%s ID: %d", translate("Infinite Line"), id+1);
    if (ImGui::CollapsingHeader(label)) {
        translated_text("Geometry");
        printf("%f", obj->position.x);

            toolButtonInfiniteLineX1      = create_tool_button("blank", translate("Start X"));
        toolButtonInfiniteLineY1      = create_tool_button("blank", translate("Start Y"));
        toolButtonInfiniteLineX2      = create_tool_button("blank", translate("2nd X"));
        toolButtonInfiniteLineY2      = create_tool_button("blank", translate("2nd Y"));
        toolButtonInfiniteLineVectorX = create_tool_button("blank", translate("Vector X"));
        toolButtonInfiniteLineVectorY = create_tool_button("blank", translate("Vector Y"));

        lineEditInfiniteLineX1      = create_line_edit("EmbReal", false);
        lineEditInfiniteLineY1      = create_line_edit("EmbReal", false);
        lineEditInfiniteLineX2      = create_line_edit("EmbReal", false);
        lineEditInfiniteLineY2      = create_line_edit("EmbReal", false);
        lineEditInfiniteLineVectorX = create_line_edit("EmbReal", true);
        lineEditInfiniteLineVectorY = create_line_edit("EmbReal", true);
        }
}

/**
 * @brief Create a group box line object.
 *
 */
void
create_group_box_line(int id, EmbLine *line)
{
    char label[200];
    sprintf(label, "%s ID: %d", translate("Line"), id+1);
    if (ImGui::CollapsingHeader(label)) {
        translated_text("Geometry");

        // \todo icons
            translated_text("Start X");
        input_double("Start X", &(line->start.x), line->start.x);
        translated_text("Start Y");
        input_double("Start Y", &(line->start.y), line->start.y);
        translated_text("End X");
        input_double("End Y", &(line->end.y), line->end.y);
        translated_text("End Y");
        input_double("End Y", &(line->end.y), line->end.y);
    
        EmbVector delta;
        delta = embVector_subtract(line->end, line->start);
        show_real("Delta X", delta.x);
        show_real("Delta Y", delta.y);
        show_real("Angle", embVector_angle(delta));
        show_real("Length", embVector_length(delta));
    }
}

/**
 * @brief Create a group box path object.
 *
 */
void
create_group_box_path(int id, EmbPath *path)
{
    char label[200];
    sprintf(label, "%s ID: %d", translate("Path"), id+1);
    if (ImGui::CollapsingHeader(label)) {
        translated_text("Geometry");
        printf("%d", path->pointList->count);
            toolButtonPathVertexNum = create_tool_button("blank", translate("Vertex #"));
        toolButtonPathVertexX   = create_tool_button("blank", translate("Vertex X"));
        toolButtonPathVertexY   = create_tool_button("blank", translate("Vertex Y"));
        toolButtonPathArea      = create_tool_button("blank", translate("Area"));
        toolButtonPathLength    = create_tool_button("blank", translate("Length"));

        comboBoxPathVertexNum = createComboBox(false);
        lineEditPathVertexX   = create_line_edit("EmbReal", false);
        lineEditPathVertexY   = create_line_edit("EmbReal", false);
        lineEditPathArea      = create_line_edit("EmbReal", true);
        lineEditPathLength    = create_line_edit("EmbReal", true);*/

        translated_text("Misc");

            toolButtonPathClosed = create_tool_button("blank", translate("Closed"));

        comboBoxPathClosed = createComboBox(false);
        }
}

/**
 * @brief Create a group box point object.
 *
 */
void
create_group_box_point(int id, EmbPoint *point)
{
    char label[200];
    sprintf(label, "%s ID: %d", translate("Point"), id+1);
    if (ImGui::CollapsingHeader(label)) {
        translated_text("Geometry");
        printf("%f", point->position.x);

            toolButtonPointX = create_tool_button("blank", translate("Position X"));
        toolButtonPointY = create_tool_button("blank", translate("Position Y"));

        lineEditPointX = create_line_edit("EmbReal", false);
        lineEditPointY = create_line_edit("EmbReal", false);
        }
}

/**
 * @brief Create a group box polygon object.
 *
 */
void
create_group_box_polygon(int id, EmbPolygon *polygon)
{
    char label[200];
    sprintf(label, "%s ID: %d", translate("Polygon"), id+1);
    if (ImGui::CollapsingHeader(label)) {
        translated_text("Geometry");
        printf("%d", polygon->pointList->count);

            toolButtonPolygonCenterX        = create_tool_button("blank", translate("Center X"));
        toolButtonPolygonCenterY        = create_tool_button("blank", translate("Center Y"));
        toolButtonPolygonRadiusVertex   = create_tool_button("blank", translate("Vertex Radius"));
        toolButtonPolygonRadiusSide     = create_tool_button("blank", translate("Side Radius"));
        toolButtonPolygonDiameterVertex = create_tool_button("blank", translate("Vertex Diameter"));
        toolButtonPolygonDiameterSide   = create_tool_button("blank", translate("Side Diameter"));
        toolButtonPolygonInteriorAngle  = create_tool_button("blank", translate("Interior Angle"));

        lineEditPolygonCenterX        = create_line_edit("EmbReal", false);
        lineEditPolygonCenterY        = create_line_edit("EmbReal", false);
        lineEditPolygonRadiusVertex   = create_line_edit("EmbReal", false);
        lineEditPolygonRadiusSide     = create_line_edit("EmbReal", false);
        lineEditPolygonDiameterVertex = create_line_edit("EmbReal", false);
        lineEditPolygonDiameterSide   = create_line_edit("EmbReal", false);
        lineEditPolygonInteriorAngle  = create_line_edit("EmbReal", true);
        }
}

/**
 * @brief Create a group box polyline object.
 *
 */
void
create_group_box_polyline(int id, EmbPolyline *polyline)
{
    char label[200];
    sprintf(label, "%s ID: %d", translate("Polyline"), id+1);
    if (ImGui::CollapsingHeader(label)) {
        translated_text("Geometry");
        printf("%d", polyline->pointList->count);

            toolButtonPolylineVertexNum = create_tool_button("blank", translate("Vertex #"));
        toolButtonPolylineVertexX   = create_tool_button("blank", translate("Vertex X"));
        toolButtonPolylineVertexY   = create_tool_button("blank", translate("Vertex Y"));
        toolButtonPolylineArea      = create_tool_button("blank", translate("Area"));
        toolButtonPolylineLength    = create_tool_button("blank", translate("Length"));

        comboBoxPolylineVertexNum = createComboBox(false);
        lineEditPolylineVertexX   = create_line_edit("EmbReal", false);
        lineEditPolylineVertexY   = create_line_edit("EmbReal", false);
        lineEditPolylineArea      = create_line_edit("EmbReal", true);
        lineEditPolylineLength    = create_line_edit("EmbReal", true);
    
        translated_text("Misc");

            toolButtonPolylineClosed = create_tool_button("blank", translate("Closed"));

        comboBoxPolylineClosed = createComboBox(false);
        }
}

/**
 * @brief Create a group box ray object.
 *
 */
void
create_group_box_ray(int id, EmbRay *ray)
{
    char label[200];
    sprintf(label, "%s ID: %d", translate("Ray"), id+1);
    if (ImGui::CollapsingHeader(label)) {
        translated_text("Geometry");
        printf("%f", ray->position.x);

            toolButtonRayX1      = create_tool_button("blank", translate("Start X"));
        toolButtonRayY1      = create_tool_button("blank", translate("Start Y"));
        toolButtonRayX2      = create_tool_button("blank", translate("2nd X"));
        toolButtonRayY2      = create_tool_button("blank", translate("2nd Y"));
        toolButtonRayVectorX = create_tool_button("blank", translate("Vector X"));
        toolButtonRayVectorY = create_tool_button("blank", translate("Vector Y"));

        lineEditRayX1      = create_line_edit("EmbReal", false);
        lineEditRayY1      = create_line_edit("EmbReal", false);
        lineEditRayX2      = create_line_edit("EmbReal", false);
        lineEditRayY2      = create_line_edit("EmbReal", false);
        lineEditRayVectorX = create_line_edit("EmbReal", true);
        lineEditRayVectorY = create_line_edit("EmbReal", true);*/
    }
}

/**
 * @brief Create a group box rectangle object.
 *
 */
void
create_group_box_rectangle(int id, EmbRect *rect)
{
    char label[200];
    sprintf(label, "%s ID: %d", translate("Rectangle"), id+1);
    if (ImGui::CollapsingHeader(label)) {
        translated_text("Geometry");
        printf("%f", rect->top);

            toolButtonRectangleCorner1X = create_tool_button("blank", translate("Corner 1 X"));
        toolButtonRectangleCorner1Y = create_tool_button("blank", translate("Corner 1 Y"));
        toolButtonRectangleCorner2X = create_tool_button("blank", translate("Corner 2 X"));
        toolButtonRectangleCorner2Y = create_tool_button("blank", translate("Corner 2 Y"));
        toolButtonRectangleCorner3X = create_tool_button("blank", translate("Corner 3 X"));
        toolButtonRectangleCorner3Y = create_tool_button("blank", translate("Corner 3 Y"));
        toolButtonRectangleCorner4X = create_tool_button("blank", translate("Corner 4 X"));
        toolButtonRectangleCorner4Y = create_tool_button("blank", translate("Corner 4 Y"));
        toolButtonRectangleWidth    = create_tool_button("blank", translate("Width"));
        toolButtonRectangleHeight   = create_tool_button("blank", translate("Height"));
        toolButtonRectangleArea     = create_tool_button("blank", translate("Area"));

        lineEdits["rectangle-corner-1x"] = create_line_edit("EmbReal", false);
        lineEditRectangleCorner1Y = create_line_edit("EmbReal", false);
        lineEditRectangleCorner2X = create_line_edit("EmbReal", false);
        lineEditRectangleCorner2Y = create_line_edit("EmbReal", false);
        lineEditRectangleCorner3X = create_line_edit("EmbReal", false);
        lineEditRectangleCorner3Y = create_line_edit("EmbReal", false);
        lineEditRectangleCorner4X = create_line_edit("EmbReal", false);
        lineEditRectangleCorner4Y = create_line_edit("EmbReal", false);
        lineEditRectangleWidth    = create_line_edit("EmbReal", false);
        lineEditRectangleHeight   = create_line_edit("EmbReal", false);
        lineEditRectangleArea     = create_line_edit("EmbReal", true);*/
    }
}

/**
 *
 */
bool stor[10];

/**
 * @brief Create a group box stitch object
 *
 */
void
create_group_box_stitch(int i, EmbStitch *st)
{
    char decorator[200], label[200];
    sprintf(decorator, "##stitch%d %d", settings.pattern_index, i);
    sprintf(label, "Stitch %d%s", i, decorator);
    if (i == 0) {
        sprintf(label, "Home%s", decorator);
    }
    if (ImGui::CollapsingHeader(label)) {
        char s[200];
        sprintf(s, "X%s", decorator);
        edit_real(s, &(st->x));
        sprintf(s, "Y%s", decorator);
        edit_real(s, &(st->y));
        /* \todo checkboxes for individual flags */
        translated_text("Flags");

        /* The stors need to be distinct memory locations. */
        stor[0] = st->flags & TRIM;
        sprintf(s, "TRIM%s", decorator);
        checkbox_data(s, stor, stor);

        stor[1] = st->flags & NORMAL;
        sprintf(s, "NORMAL%s", decorator);
        checkbox_data(s, stor+1, stor+1);

        stor[2] = st->flags & END;
        sprintf(s, "END%s", decorator);
        checkbox_data(s, stor+2, stor+2);

        st->flags = (stor[0] & TRIM) + (stor[1] & NORMAL) + (stor[2] & END);

        sprintf(s, "Thread Index%s", decorator);
        edit_int(s, &(st->color));
    }
}

/**
 *
 */
int from_ = 0;
int to_ = 10;

/**
 * @brief Create a group box stitch list object.
 *
 */
void
create_group_box_stitch_list(int id, EmbArray *stitch_list)
{
    char label[200];
    sprintf(label, "%s ID:%d", translate("Stitch List"), id+1);
    if (ImGui::CollapsingHeader(label)) {
        char s[200];
        sprintf(s, "From##%d", id+1);
        edit_int(s, &from_);
        sprintf(s, "To##%d", id+1);
        edit_int(s, &to_);
        if (to_ < from_) {
            to_ = from_;
        }
        if (to_ > stitch_list->count) {
            to_ = stitch_list->count;
        }
        for (int i=from_; i<to_; i++) {
            EmbStitch *st = &(stitch_list->stitch[i]);
            create_group_box_stitch(i, st);
        }
    }
}

/**
 * @brief Create a group box text multi object.
 *
 */
void
create_group_box_text_multi(int id, EmbTextMulti *text_multi)
{
    if (ImGui::CollapsingHeader("Text Multi")) {
        translated_text("Geometry");
        translated_text("Text");

            toolButtonTextMultiX = create_tool_button("blank", translate("Position X"));
        toolButtonTextMultiY = create_tool_button("blank", translate("Position Y"));

        lineEditTextMultiX = create_line_edit("EmbReal", false);
        lineEditTextMultiY = create_line_edit("EmbReal", false);*/
    }
}

/**
 * @brief Create a group box text single object
 *
 */
void
create_group_box_text_single(int id, EmbTextSingle *text_single)
{
    if (ImGui::CollapsingHeader("Text Single")) {
            toolButtonTextSingleContents = create_tool_button("blank", translate("Contents"));
        toolButtonTextSingleFont     = create_tool_button("blank", translate("Font"));
        toolButtonTextSingleJustify  = create_tool_button("blank", translate("Justify"));
        toolButtonTextSingleHeight   = create_tool_button("blank", translate("Height"));
        toolButtonTextSingleRotation = create_tool_button("blank", translate("Rotation"));

        lineEditTextSingleContents = create_line_edit("string", false);
        comboBoxTextSingleFont     = createFontComboBox(false);
        comboBoxTextSingleJustify  = createComboBox(false);
        lineEditTextSingleHeight   = create_line_edit("EmbReal", false);
        lineEditTextSingleRotation = create_line_edit("EmbReal", false);*/

        translated_text("Geometry");

            toolButtonTextSingleX = create_tool_button("blank", translate("Position X"));
        toolButtonTextSingleY = create_tool_button("blank", translate("Position Y"));

        lineEditTextSingleX = create_line_edit("EmbReal", false);
        lineEditTextSingleY = create_line_edit("EmbReal", false);*/

        translated_text("Misc");

            toolButtonTextSingleBackward   = create_tool_button("blank", translate("Backward"));
        toolButtonTextSingleUpsideDown = create_tool_button("blank", translate("UpsideDown"));

        comboBoxTextSingleBackward   = createComboBox(false);
        comboBoxTextSingleUpsideDown = createComboBox(false);*/
    }
}

/**
 * @brief
 *
 * Returns Index*
 */
Index *index_init(void)
{
    Index *index = (Index*)malloc(sizeof(Index));
    index->max_length = 1000;
    index->length = 0;
    index->data = (IndexEntry*)malloc(1000*sizeof(IndexEntry));
    return index;
}

void
index_free(Index *index)
{
    free(index->data);
    free(index);
}

Dictionary *
dictionary_from_index(Index *index, const char *label)
{
    for (int i=0; i<index->length; i++) {
        if (!strcmp(index->data[i].key, label)) {
            return index->data[i].value;
        }
    }
    return NULL;
}

/**
 * @brief
 *
 * Returns UndoHistory
 */
UndoHistory
undo_history_init(void)
{
    UndoHistory undo_history;
    undo_history.max_length = 100;
    undo_history.length = 0;
    undo_history.position = 0;
    return undo_history;
}

void
add_to_undo_history(UndoHistory *undo_history, const char *command_line)
{
    int pos = undo_history->position;
    strcpy(undo_history->data[pos], command_line);
    undo_history->position++;
    undo_history->length++;
    if (undo_history->position >= 80) {
        for (int i=0; i<80; i++) {
            strcpy(undo_history->data[i], undo_history->data[i+20]);
        }
        undo_history->position -= 20;
        undo_history->length -= 20;
    }
}

void
load_configuration(void)
{
    char error_buffer[200];
    FILE *f;
    char fname[200];
    sprintf(fname, "%sem2_config.toml", settings.assets_dir);
    f = fopen(fname, "r");
    if (!f) {
        log_debug("ERROR: failed to open configuration file \"em2_config.toml\".");
        settings.running = false;
        return;
    }
    toml_table_t *config = toml_parse_file(f, error_buffer, sizeof(error_buffer));
    fclose(f);

    if (!config) {
        log_debug("ERROR: failed to parse \"em2_config.toml\".");
        settings.running = false;
        return;
    }

    Dictionary *menu = read_dictionary_setting(config, "menu_layout");
    char cmd[200];
    sprintf(cmd, "menu length: %d", menu->length);
    log_debug(cmd);
    menu_layout = index_init();
    for (int i=0; i<menu->length; i++) {
        strcpy(menu_layout->data[i].key, menu->data[i].key);
        menu_layout->data[i].value = read_dictionary_setting(config, menu->data[i].value);
        menu_layout->length++;
    }

    Dictionary *toolbar = read_dictionary_setting(config, "toolbar_layout");
    sprintf(cmd, "toolbar length: %d", toolbar->length);
    log_debug(cmd);
    toolbar_layout = index_init();
    for (int i=0; i<toolbar->length; i++) {
        strcpy(toolbar_layout->data[i].key, toolbar->data[i].key);
        toolbar_layout->data[i].value = read_dictionary_setting(config, toolbar->data[i].value);
        toolbar_layout->length++;
    }

    /* string_matrix actions = load_string_matrix(config, "action_list"); */

    settings.texture_list = read_dictionary_setting(config, "texture_list");
    sprintf(cmd, "texture_list length: %d", settings.texture_list->length);
    log_debug(cmd);

    read_str_setting(config, "version", settings.version);
    log_debug(settings.version);
    toml_free(config);
}

void
read_str_setting(toml_table_t *table, const char *key, char s[200])
{
    toml_datum_t result_in = toml_string_in(table, key);
    if (result_in.ok) {
        strcpy(s, result_in.u.s);
    }
    else {
        char error[200];
        sprintf(error, "ERROR: failed to read %s.\n", key);
        log_debug(error);
    }
    free(result_in.u.s);
}

int
read_int_setting(toml_table_t *table, const char *key)
{
    toml_datum_t result_in = toml_int_in(table, key);
    if (!result_in.ok) {
        char s[200];
        sprintf(s, "ERROR: failed to read %s.\n", key);
        log_debug(s);
        return 0;
    }
    return result_in.u.i;
}

bool
read_bool_setting(toml_table_t *table, const char *key)
{
    toml_datum_t result_in = toml_bool_in(table, key);
    if (!result_in.ok) {
        char s[200];
        sprintf(s, "ERROR: failed to read %s.\n", key);
        log_debug(s);
    }
    return result_in.u.b;
}

Dictionary *
read_dictionary_setting(toml_table_t *table, const char *key)
{
    Dictionary *dict = (Dictionary*)malloc(sizeof(Dictionary));
    dict->data = (DictionaryEntry*)malloc(200*sizeof(DictionaryEntry));
    dict->length = 0;
    dict->max_length = 200;
    toml_array_t *array = toml_array_in(table, key);

    if (!array) {
        char s[200];
        sprintf(s, "ERROR: failed to read %s.\n", key);
        log_debug(s);
        return NULL;
    }

    for (int i=0; ; i++) {
        toml_datum_t str1;
        dict->length = i;
        toml_array_t *a2 = toml_array_at(array, i);
        if (!a2) {
            break;
        }
        str1 = toml_string_at(a2, 0);
        if (!str1.ok) {
            free(str1.u.s);
            break;
        }
        strcpy(dict->data[i].key, str1.u.s);

        str1 = toml_string_at(a2, 1);
        if (!str1.ok) {
            free(str1.u.s);
            break;
        }
        strcpy(dict->data[i].value, str1.u.s);
        free(str1.u.s);
    }

    return dict;
}

void
write_float_setting(FILE *file, const char *key, float value)
{
    fprintf(file, "%s = %f\n", key, value);
}

void
write_int_setting(FILE *file, const char *key, int value)
{
    fprintf(file, "%s = %d\n", key, value);
}

void
write_uint_setting(FILE *file, const char *key, unsigned int value)
{
    fprintf(file, "%s = %u\n", key, value);
}

void
write_str_setting(FILE *file, const char *key, const char value[200])
{
    fprintf(file, "%s = \"%s\"\n", key, value);
}

void
write_bool_setting(FILE *file, const char *key, bool value)
{
    if (value) {
        fprintf(file, "%s = true\n", key);
    }
    else {
        fprintf(file, "%s = false\n", key);
    }
}

void
write_dictionary_setting(FILE *file, const char *key, Dictionary dict)
{
    fprintf(file, "%s = [\n", key);
    for (int i=0; i<dict.length-1; i++) {
        fprintf(file, "    [%s, %s],\n", dict.data[i].key, dict.data[i].value);
    }
    fprintf(file, "    [%s, %s]\n",
        dict.data[dict.length-1].key,
        dict.data[dict.length-1].value);
    fprintf(file, "]\n\n");
}

void
read_settings(const char *settings_file)
{
    EmbVector pos(0,0);
    QSize size(800,600);

    /* some layout state recording? */
    char error_buffer[200];
    FILE *f;
    f = fopen(settings_file, "r");
    if (!f) {
        return;
    }
    toml_table_t *setting_toml = toml_parse_file(f, error_buffer, sizeof(error_buffer));
    fclose(f);

    if (!setting_toml) {
        puts("ERROR: failed to parse settings.toml, continuing with defaults.");
        return;
    }

    if (setting_toml.contains("window")) {
        toml_table_t table = read_setting(setting_toml, "window");
        if (table.contains("position-x")) {
            pos.setX(read_setting(table, "position_x"));
        }
        if (table.contains("position-y")) {
            pos.setY(read_setting(table, "position_y"));
        }
        if (table.contains("size-x")) {
            size.setWidth(read_setting(table, "size_x"));
        }
        if (table.contains("size-y")) {
            size.setHeight(read_setting(table, "size_y"));
        }
    }

    toml_table_t *table = toml_table_in(setting_toml, "general");
    if (table) {
        read_str_setting(table, "language", settings.language);
        settings.icon_size = read_int_setting(table, "icon_size");
        read_str_setting(table, "icon_theme", settings.icon_theme);
        settings.use_translation = read_bool_setting(table, "use_translation");
        settings.mdi_bg_use_logo = read_bool_setting(table, "mdi_bg_use_logo");
        settings.mdi_bg_use_texture = read_bool_setting(table, "mdi_bg_use_texture");
        settings.mdi_bg_use_color = read_bool_setting(table, "mdi_bg_use_color");
            settings.general_mdi_bg_logo = read_setting(table, "mdi_bg_logo");
        settings.general_mdi_bg_texture = read_setting(table, "mdi_bg_texture");
        settings.general_mdi_bg_color = read_setting(table, "mdi_bg_color");
        settings.tip_of_the_day = read_setting(table, "tip_of_the_day");
        settings.general_current_tip = read_setting(table, "current_tip");
        settings.general_system_help_browser = read_setting(table, "general_system_help_browser");
        }

    if (setting_toml.contains("display")) {
        toml_table_t table = toml_find<toml_table_t>(setting_toml, "display");
    }

    .display_use_opengl = false,
    .display_renderhint_aa = false,
    .display_renderhint_text_aa = false,
    .display_renderhint_smooth_pix = false,
    .display_renderhint_high_aa = false,
    .display_renderhint_noncosmetic = false,
    .display_show_scrollbars = true,
    .display_scrollbar_widget_num = 0,
    .display_crosshair_color = 0x000000FF,
    .display_bg_color = 0xEEEEEEFF,
    .display_selectbox_left_color = 0x008000FF,
    .display_selectbox_left_fill = 0x00FF00FF,
    .display_selectbox_right_color = 0x000080FF,
    .display_selectbox_right_fill = 0x0000FFFF,
    .display_selectbox_alpha = 32,
    .display_zoomscale_in = 2.0,
    .display_zoomscale_out = 0.5,
    .display_crosshair_percent = 5,
    .display_units = "mm",
    .opensave_custom_filter = "supported",
    .opensave_open_format = "*.*",
    .opensave_open_thumbnail = false,
    .opensave_save_format = "*.*",
    .opensave_save_thumbnail = false,
    .opensave_recent_max_files = 10,
    // .opensave_recent_list_of_files = {},
    .opensave_recent_directory = "./samples",
    .opensave_trim_dst_num_jumps = 5,
    .printing_default_device = "",
    .printing_use_last_device = false,
    .printing_disable_bg = true,
    .grid_show_on_load = true,
    .grid_show_origin = true,
    .grid_color_match_crosshair = true,
    .grid_color = 0x000000FF,
    .grid_load_from_file = true,
    .grid_type = "Rectangular",
    .grid_center_on_origin = true,
    .grid_center.x = 0.0,
    .grid_center.y = 0.0,
    .grid_size_x = 100.0,
    .grid_size_y = 100.0,
    .grid_spacing_x = 25.0,
    .grid_spacing_y = 25.0,
    .grid_size_radius = 50.0,
    .grid_spacing_radius = 25.0,
    .grid_spacing_angle = 45.0,
    .ruler_show_on_load = true,
    .ruler_metric = true,
    .ruler_color = 0xAAAA55FF, //qRgb(210,210, 50);
    .ruler_pixel_size = 20,
    .qsnap_enabled = true,
    .qsnap_locator_color = 0xFFFF00FF,
    .qsnap_locator_size = 4,
    .qsnap_aperture_size = 10,
    .qsnap_endpoint = true,
    .qsnap_midpoint = true,
    .qsnap_center = true,
    .qsnap_node = true,
    .qsnap_quadrant = true,
    .qsnap_intersection = true,
    .qsnap_extension = true,
    .qsnap_insertion = false,
    .qsnap_perpendicular = true,
    .qsnap_tangent = true,
    .qsnap_nearest = false,
    .qsnap_apparent = false,
    .qsnap_parallel = false,
    .lwt_show_lwt = false,
    .lwt_real_render = true,
    .lwt_default_lwt = 0,
    .selection_mode_pickfirst = true,
    .selection_mode_pickadd = true,
    .selection_mode_pickdrag = false,
    .selection_coolgrip_color = 0x0000FFFF,
    .selection_hotgrip_color = 0xFF0000FF,
    .selection_grip_size = 4,
    .selection_pickbox_size = 4,
    .text_font  = "Arial",
    .text_size = 12,
    .text_angle = 0,
    .text_style_bold = false,
    .text_style_italic = false,
    .text_style_underline = false,
    .text_style_overline = false,
    .text_style_strikeout = false

    move(pos);
    resize(size);*/
}

/**
 * @brief Write settings to file with file name \a settings_file.
 */
void
write_settings(const char *settings_file)
{
    FILE *file = fopen(settings_file, "w");

    fprintf(file, "[Window]\n");
    write_bool_setting(file, "PositionX", pos().x);
    write_bool_setting(file, "PositionY", pos().y);
    write_bool_setting(file, "SizeX", size().x);
    write_bool_setting(file, "SizeX", size().y);

    fprintf(file, "[general]\n");
    //write_bool_setting(file, "LayoutState", layoutState);
    write_str_setting(file, "language", settings.language);
    write_str_setting(file, "icon_theme", settings.icon_theme);
    write_int_setting(file, "icon_size", settings.icon_size);
    write_bool_setting(file, "mdi_bg_use_logo", settings.mdi_bg_use_logo);
    write_bool_setting(file, "mdi_bg_use_texture", settings.mdi_bg_use_texture);
    write_bool_setting(file, "mdi_bg_use_color", settings.mdi_bg_use_color);
    write_str_setting(file, "mdi_bg_logo", settings.general_mdi_bg_logo);
    write_str_setting(file, "mdi_bg_texture", settings.general_mdi_bg_texture);
    write_uint_setting(file, "mdi_bg_color", settings.general_mdi_bg_color);
    write_int_setting(file, "tip_of_the_day", settings.tip_of_the_day);
    write_int_setting(file, "current_tip", settings.general_current_tip + 1);
    write_bool_setting(file, "system_help_browser", settings.general_system_help_browser);
    fprintf(file, "\n");

    fprintf(file, "[display]\n");
    write_bool_setting(file, "UseOpenGL", settings.display_use_opengl);
    write_bool_setting(file, "RenderHintAntiAlias", settings.display_renderhint_aa);
    write_bool_setting(file, "RenderHintTextAntiAlias", settings.display_renderhint_text_aa);
    write_bool_setting(file, "RenderHintSmoothPixmap", settings.display_renderhint_smooth_pix);
    write_bool_setting(file, "RenderHintHighQualityAntiAlias", settings.display_renderhint_high_aa);
    write_bool_setting(file, "RenderHintNonCosmetic", settings.display_renderhint_noncosmetic);
    write_bool_setting(file, "Showscrollbars", settings.display_show_scrollbars);
    write_int_setting(file, "scrollbarWidgetNum", settings.display_scrollbar_widget_num);
    write_uint_setting(file, "CrossHairColor", settings.display_crosshair_color);
    write_uint_setting(file, "BackgroundColor", settings.display_bg_color);
    write_uint_setting(file, "SelectBoxLeftColor", settings.display_selectbox_left_color);
    write_uint_setting(file, "SelectBoxLeftFill", settings.display_selectbox_left_fill);
    write_uint_setting(file, "SelectBoxRightColor", settings.display_selectbox_right_color);
    write_bool_setting(file, "SelectBoxRightFill", settings.display_selectbox_right_fill);
    write_bool_setting(file, "SelectBoxAlpha", settings.display_selectbox_alpha);
    write_bool_setting(file, "ZoomScaleIn", settings.display_zoomscale_in);
    write_bool_setting(file, "ZoomScaleOut", settings.display_zoomscale_out);
    write_bool_setting(file, "CrossHairPercent", settings.display_crosshair_percent);
    write_str_setting(file, "Units", settings.display_units);
    fprintf(file, "\n");

    fprintf(file, "[OpenSave]\n");
    write_str_setting(file, "CustomFilter", settings.opensave_custom_filter);
    write_str_setting(file, "OpenFormat", settings.opensave_open_format);
    write_bool_setting(file, "OpenThumbnail", settings.opensave_open_thumbnail);
    write_str_setting(file, "SaveFormat", settings.opensave_save_format);
    write_bool_setting(file, "SaveThumbnail", settings.opensave_save_thumbnail);
    write_bool_setting(file, "RecentMax", settings.opensave_recent_max_files);
    //write_bool_setting(file, "RecentFiles", settings.opensave_recent_list_of_files);
    write_str_setting(file, "RecentDirectory", settings.opensave_recent_directory);
    write_int_setting(file, "TrimDstNumJumps", settings.opensave_trim_dst_num_jumps);
    fprintf(file, "\n");

    fprintf(file, "[Printing]\n");
    write_str_setting(file, "DefaultDevice", settings.printing_default_device);
    write_bool_setting(file, "UseLastDevice", settings.printing_use_last_device);
    write_bool_setting(file, "DisableBG", settings.printing_disable_bg);
    fprintf(file, "\n");

    fprintf(file, "[Grid]\n");
    write_bool_setting(file, "ShowOnLoad", settings.grid_show_on_load);
    write_bool_setting(file, "ShowOrigin", settings.grid_show_origin);
    write_bool_setting(file, "ColorMatchCrossHair", settings.grid_color_match_crosshair);
    write_bool_setting(file, "Color", settings.grid_color);
    write_bool_setting(file, "LoadFromFile", settings.grid_load_from_file);
    write_str_setting(file, "Type", settings.grid_type);
    write_bool_setting(file, "CenterOnOrigin", settings.grid_center_on_origin);
    write_float_setting(file, "CenterX", settings.grid_center.x);
    write_float_setting(file, "CenterY", settings.grid_center.y);
    write_float_setting(file, "SizeX", settings.grid_size_x);
    write_float_setting(file, "SizeY", settings.grid_size_y);
    write_float_setting(file, "SpacingX", settings.grid_spacing_x);
    write_float_setting(file, "SpacingY", settings.grid_spacing_y);
    write_float_setting(file, "SizeRadius", settings.grid_size_radius);
    write_float_setting(file, "SpacingRadius", settings.grid_spacing_radius);
    write_float_setting(file, "SpacingAngle", settings.grid_spacing_angle);
    fprintf(file, "\n");

    fprintf(file, "[Ruler]\n");
    write_bool_setting(file, "ShowOnLoad", settings.ruler_show_on_load);
    write_bool_setting(file, "Metric", settings.ruler_metric);
    write_uint_setting(file, "Color", settings.ruler_color);
    write_int_setting(file, "PixelSize", settings.ruler_pixel_size);
    fprintf(file, "\n");

    fprintf(file, "[QuickSnap]\n");
    write_bool_setting(file, "Enabled", settings.qsnap_enabled);
    write_bool_setting(file, "LocatorColor", settings.qsnap_locator_color);
    write_bool_setting(file, "LocatorSize", settings.qsnap_locator_size);
    write_bool_setting(file, "ApertureSize", settings.qsnap_aperture_size);
    write_bool_setting(file, "EndPoint", settings.qsnap_endpoint);
    write_bool_setting(file, "MidPoint", settings.qsnap_midpoint);
    write_bool_setting(file, "Center", settings.qsnap_center);
    write_bool_setting(file, "Node", settings.qsnap_node);
    write_bool_setting(file, "Quadrant", settings.qsnap_quadrant);
    write_bool_setting(file, "Intersection", settings.qsnap_intersection);
    write_bool_setting(file, "Extension", settings.qsnap_extension);
    write_bool_setting(file, "Insertion", settings.qsnap_insertion);
    write_bool_setting(file, "Perpendicular", settings.qsnap_perpendicular);
    write_bool_setting(file, "Tangent", settings.qsnap_tangent);
    write_bool_setting(file, "Nearest", settings.qsnap_nearest);
    write_bool_setting(file, "Apparent", settings.qsnap_apparent);
    write_bool_setting(file, "Parallel", settings.qsnap_parallel);
    fprintf(file, "\n");

    fprintf(file, "[LineWeight]\n");
    write_bool_setting(file, "ShowLineWeight", settings.lwt_show_lwt);
    write_bool_setting(file, "RealRender", settings.lwt_real_render);
    write_bool_setting(file, "DefaultLineWeight", settings.lwt_default_lwt);
    fprintf(file, "\n");

    fprintf(file, "[Selection]\n");
    write_bool_setting(file, "PickFirst", settings.selection_mode_pickfirst);
    write_bool_setting(file, "PickAdd", settings.selection_mode_pickadd);
    write_bool_setting(file, "PickDrag", settings.selection_mode_pickdrag);
    write_bool_setting(file, "CoolGripColor", settings.selection_coolgrip_color);
    write_bool_setting(file, "HotGripColor", settings.selection_hotgrip_color);
    write_bool_setting(file, "GripSize", settings.selection_grip_size);
    write_bool_setting(file, "PickBoxSize", settings.selection_pickbox_size);
    fprintf(file, "\n");

    fprintf(file, "[Text]\n");
    write_str_setting(file, "Font", settings.text_font);
    write_int_setting(file, "Size", settings.text_size);
    write_float_setting(file, "Angle", settings.text_angle);
    write_bool_setting(file, "StyleBold", settings.text_style_bold);
    write_bool_setting(file, "StyleItalic", settings.text_style_italic);
    write_bool_setting(file, "StyleUnderline", settings.text_style_underline);
    write_bool_setting(file, "StyleStrikeOut", settings.text_style_strikeout);
    write_bool_setting(file, "StyleOverline", settings.text_style_overline);
    fclose(file);
}

/**
 * @brief Report the platform, usually as part of a debugging effort accross platforms.
 * \a str A chararacter array pointer of where to store the result.
 */
void
platform_string(char str[200])
{
    strcpy(str, "Unknown plaform.");
    #if _WIN32
    strcpy(str, "Windows");
    #endif
    #ifdef __linux__
    struct utsname buffer;
    uname(&buffer);
    sprintf(str, "%s %s %s %s %s",
        buffer.sysname,
        buffer.nodename,
        buffer.release,
        buffer.version,
        buffer.machine);
    #endif
}

/**
 * @brief Advice to users on how to use command line arguments.
 */
void
usage(void)
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
    "\n");
}

/**
 * @brief Rather than outputting debug information to the command line it is sent to a file called
 * "debug.txt" which is in the current directory.
 *
 * \a msg The message to be be written to the end of the debug file.
 *
 * The benefits of always logging to file are: they are easily requested from users who need help and
 * we can search the debug log using grep and other tool.
 *
 * \todo log current time and process id alongside all debug information.
 */
void
log_debug(const char *msg)
{
    if (settings.debug_mode) {
        FILE *f = fopen("debug.txt", "a");
        fprintf(f, "%s\n", msg);
        fclose(f);
    }
}

/**
 * @brief Check that \a r, \a g and \a b are in the range 0-255 inclusive.
 */
bool
validRGB(int r, int g, int b)
{
    if (r < 0 || r > 255) {
        log_debug("ERROR: r value must be in range 0-255");
        return false;
    }
    if (g < 0 || g > 255) {
        log_debug("ERROR: g value must be in range 0-255");
        return false;
    }
    if (b < 0 || b > 255) {
        log_debug("ERROR: b value must be in range 0-255");
        return 0;
    }
    return true;
}

/**
 * @brief Wrapper for the the standard strncmp that is aware of our
 * 200 character string limit.
 *
 * \a s1 The first string, usually the variable data.
 * \a s2 The second string, usually the contant to compare against.
 * Returns true when the strings match for the first 200 chracters,
 * for longer strings that match this far this is the wrong tool.
 */
bool
streq(const char *s1, const char *s2)
{
    return !strncmp(s1, s2, 200);
}

/**
 * @brief Rather than using .po files we translate at runtime using this function.
 *
 * \a string The string to be translated.
 * Returns const char* The translated string.
 *
 * \todo check for translation_table loaded and use it.
 */
const char *
translate(const char *string)
{
    if (settings.use_translation) {
        return string;
    }
    return string;
    for (int i=0; i<translation_table->length; i++) {
        if (streq(translation_table->data[i].key, string)) {
            return translation_table->data[i].value;
        }
    }
    return string;
}

EmbLine stitch_line_to_scene(EmbView view, EmbStitch prev, EmbStitch st);

int
render_pattern(EmbPattern *p)
{
    for (int i=0; i<p->geometry->count; i++) {
        EmbGeometry g = p->geometry->geometry[i];
        int color = embColor_to_int(g.color, 255);
        switch (g.type) {
        case EMB_CIRCLE: {
            EmbCircle c = g.object.circle;
            EmbVector scene_center = embVector_add(c.center, active_view->origin);
            EmbReal scene_radius = c.radius * active_view->scale;
            draw_circle(scene_center, scene_radius, color, false);
            // \todo fill
            break;
        }
        case EMB_ELLIPSE: {
                    EmbEllipse e = g.object.ellipse;
            int color = embColor_to_int(e.color);
            //draw_list->AddEllipse(e.center.x, e.center.y, e.radius.x, e.radius.y, color);
                    break;
        }
        case EMB_LINE: {
            EmbVector scene_start, scene_end;
            EmbLine li = g.object.line;
            scene_start = embVector_add(li.start, active_view->origin);
            scene_end = embVector_add(li.end, active_view->origin);
            draw_line(scene_start, scene_end, color);
            break;
        }
        case EMB_PATH: {
                    for (int i=0; i<p->paths->count; i++) {
                EmbPath path;
                EmbArray *curPointList = path.pointList;
                if (curPointList) {
                    EmbPoint pp = curPointList->point[0];
                    pathPath.moveTo(pp.position.x, -pp.position.y);
                    //NOTE: Qt Y+ is down and libembroidery Y+ is up, so inverting the Y is needed.
                }
                for (int j=0; j<curPointList->count; j++) {
                    EmbPoint pp = curPointList->point[j];
                    pathPath.lineTo(pp.position.x, -pp.position.y);
                    // NOTE: Qt Y+ is down and libembroidery Y+ is up, so inverting the Y is needed.
                }

                QPen loadPen(qRgb(thisColor.r, thisColor.g, thisColor.b));
                loadPen.setWidthF(0.35);
                loadPen.setCapStyle(RoundCap);
                loadPen.setJoinStyle(RoundJoin);

                PathObject* obj = PathObject(0,0, pathPath, loadPen.color().rgb());
                obj->setObjectRubberMode(OBJ_RUBBER_OFF);
                active_view->addItem(obj);
            }
                    break;
        }
        case EMB_POINT: {
            EmbPoint po = g.object.point;
            EmbReal point_size = 10.0;
            draw_circle(po.position, point_size, color, false);
            break;
        }
        case EMB_POLYGON: {
                    for (int i=0; i<p->polygons->count; i++) {
                // EmbPath polygonPath;
                EmbPolygon polygon = p->polygons->polygon[i];
                bool firstPoint = false;
                EmbVector start = {0.0, 0.0};
                EmbReal x = 0.0, y = 0.0;
                EmbArray *curPointList = polygon.pointList;
                for (int j=0; j<curPointList->count; j++) {
                    EmbPoint pp = curPointList->point[j];
                    x = pp.position.x;
                    y = -pp.position.y;

                    if (firstPoint) {
                        polygonPath.lineTo(x,y);
                    }
                    else {
                        polygonPath.moveTo(x,y);
                        firstPoint = true;
                        startX = x;
                        startY = y;
                    }
                }

                polygonPath.translate(-startX, -startY);
                AddPolygon(startX, startY, polygonPath, OBJ_RUBBER_OFF);
            }
                    break;
        }
        /* NOTE: Polylines should only contain NORMAL stitches. */
        case EMB_POLYLINE: {
            EmbPolyline pl = g.object.polyline;
            EmbArray *curPointList = pl.pointList;
            EmbLine line;
            line.start = curPointList->geometry[0].object.point.position;
            for (int j=1; j<curPointList->count; j++) {
                EmbPoint pp = curPointList->geometry[j].object.point;
                line.end.x = pp.position.x;
                line.end.y = -pp.position.y;
                draw_line(line.start, line.end, color);
            }

                    polylinePath.translate(-startX, -startY);
            addPolyline(startX, startY, polylinePath, OBJ_RUBBER_OFF);
                    break;
        }
        case EMB_RECT: {
                    EmbRect r = p->rects->rect[i];
            EmbVector position1 = {(float)r.left, (float)r.top};
            EmbVector position2 = {(float)r.right, (float)r.bottom};
            draw_rect(position1, position2, color); //\todo rotation and fill
                    break;
        }
        default:
            break;
        }
    }
    if (p->stitch_list->count > 1) {
        for (int i = 1; i<p->stitch_list->count; i++) {
            EmbStitch prev = p->stitch_list->stitch[i-1];
            EmbStitch st = p->stitch_list->stitch[i];
            EmbLine line = stitch_line_to_scene(*active_view, prev, st);
            EmbThread thread = p->thread_list->thread[st.color];
            int color = embColor_to_int(thread.color, 255);
            draw_line(line.start, line.end, color);
        }
    }
    return 0;
}

/**
 * @brief Add a rectangular shine in the direction indicated by \a normal.
 *
 * \a start The start point in the middle of one short side of the rectangle.
 * \a end The start point in the middle of one short side of the rectangle.
 */
void
render_shine(EmbVector start, EmbVector end, EmbVector normal, float start_frac, float end_frac)
{
    EmbVector delta, shine_start, shine_end, corner1, corner2, corner3, corner4;
    delta = embVector_subtract(end, start);
    shine_start = start;
    shine_end = end;
    shine_start.x += start_frac*delta.x;
    shine_start.y += start_frac*delta.y;
    shine_end.x += end_frac*delta.x;
    shine_end.y += end_frac*delta.y;

    corner1 = embVector_add(shine_start, normal);
    corner2 = embVector_subtract(shine_start, normal);
    corner3 = embVector_subtract(shine_end, normal);
    corner4 = embVector_add(shine_end, normal);

    draw_quad(corner1, corner2, corner3, corner4, settings.shine_color);
}

EmbLine
stitch_line_to_scene(EmbView view, EmbStitch prev, EmbStitch st)
{
    EmbVector offset = get_window_pos();
    EmbLine line;
    line.start.x = view.scale*prev.x;
    line.start.y = -view.scale*prev.y;
    line.end.x = view.scale*st.x;
    line.end.y = -view.scale*st.y;
    offset = embVector_add(offset, view.origin);
    line.start = embVector_add(line.start, offset);
    line.end = embVector_add(line.end, offset);
    return line;
}

void
real_render_pattern(EmbPattern *p)
{
    if (p->stitch_list->count > 1) {
        for (int i = 1; i<p->stitch_list->count; i++) {
            EmbStitch prev = p->stitch_list->stitch[i-1];
            EmbStitch st = p->stitch_list->stitch[i];
            EmbLine line = stitch_line_to_scene(*active_view, prev, st);

            float lwt = 2.0;
            EmbVector delta;
            delta.x = prev.y - st.y;
            delta.y = prev.x - st.x;
            EmbVector normal_;
            embVector_normalize(delta, &normal_);
            normal_.x *= lwt;
            normal_.y *= lwt;

            EmbThread thread = p->thread_list->thread[st.color];
            int color = embColor_to_int(thread.color, 255);
            EmbVector corner1, corner2, corner3, corner4;
            corner1 = embVector_add(line.start, normal_);
            corner2 = embVector_subtract(line.start, normal_);
            corner3 = embVector_subtract(line.end, normal_);
            corner4 = embVector_add(line.end, normal_);
            draw_quad(corner1, corner2, corner3, corner4, color);
            render_shine(line.start, line.end, normal_, 0.333*0.3, 0.333*0.7);
            render_shine(line.start, line.end, normal_, 0.333*0.1, 0.333*0.9);
            render_shine(line.start, line.end, normal_, 0.333*1.3, 0.333*1.7);
            render_shine(line.start, line.end, normal_, 0.333*1.1, 0.333*1.9);
            render_shine(line.start, line.end, normal_, 0.333*2.3, 0.333*2.7);
            render_shine(line.start, line.end, normal_, 0.333*2.1, 0.333*2.9);
        }
    }
}

void
simulate_pattern(EmbPattern *p)
{
    clock_t duration = clock() - active_view->simulation_start;
    float time_passed = duration / (1.0 * CLOCKS_PER_SEC);
    int stitches = 0;
    if (p->stitch_list->count > 1) {
        float current_time = 0.0;
        for (int i = 1; i<p->stitch_list->count; i++) {
            EmbStitch prev = p->stitch_list->stitch[i-1];
            EmbStitch st = p->stitch_list->stitch[i];
            EmbLine line = stitch_line_to_scene(*active_view, prev, st);

            EmbThread thread = p->thread_list->thread[st.color];
            int color = embColor_to_int(thread.color, 255);
            EmbVector delta = {prev.x-st.x, prev.y-st.y};
            float time_for_stitch = settings.stitch_time
                + embVector_length(delta)/settings.needle_speed;
            current_time += time_for_stitch;
            stitches = i;
            if (current_time > time_passed) {
                float t = current_time-time_passed-time_for_stitch;
                EmbVector scene_delta;
                scene_delta = embVector_subtract(line.end, line.start);
                line.end.x = line.start.x - t*scene_delta.x;
                line.end.y = line.start.y - t*scene_delta.y;
                draw_line(line.start, line.end, color);
                break;
            }
            else {
                draw_line(line.start, line.end, color);
            }
        }
    }

    begin_ui("Controls", 500, 500);
    char s[200];
    sprintf(s, "Stitch: %d/%d", stitches, p->stitch_list->count);
    translated_text(s);
    show_real("Needle Speed: ", settings.needle_speed);
    show_real("Stitch Time: ", settings.stitch_time);
    show_real("Time Passed: ", time_passed);

    if (button("Slower")) {
        settings.needle_speed /= 2.0;
        settings.stitch_time *= 2.0;
    }
    ImGui::SameLine();
    if (button("Faster")) {
        settings.needle_speed *= 2.0;
        settings.stitch_time /= 2.0;
    }

    if (button("Start")) {
        active_view->simulation_start = clock();
    }
    ImGui::SameLine();
    if (button("Previous Stitch")) {

    }
    ImGui::SameLine();
    if (button("Next Stitch")) {

    }
    ImGui::SameLine();
    if (button("End")) {

    }

    if (button("Close")) {
        active_view->simulate = false;
    }
    ImGui::EndUi();
}

/**
 * @brief
 *
 * \todo labels for measurements.
 */
void
draw_rulers(void)
{
    EmbVector offset = get_window_pos();
    EmbVector screen_size = get_screen_size();
    EmbVector dimensions;
    dimensions.x = offset.x + settings.ruler_width;
    dimensions.y = offset.y + screen_size.y;
    draw_rect(offset, dimensions, settings.ruler_color, true);
    dimensions.x = offset.x + screen_size.x;
    dimensions.y = offset.y + settings.ruler_width;
    draw_rect(offset, dimensions, settings.ruler_color, true);
    EmbVector origin_x, origin_y;
    origin_x.x = active_view->origin.x;
    origin_x.y = 0.0;
    origin_y.x = 0.0;
    origin_y.y = active_view->origin.y;
    float major_tick_seperation_scaled = active_view->scale * settings.major_tick_seperation;
    if (active_view->metric) {
        int repeats = 100;
        for (int i=-repeats; i<repeats; i++) {
            float ruler_pos = settings.ruler_width
                + major_tick_seperation_scaled*i;
            EmbLine line;
            line.start.x = offset.x + origin_x.x + ruler_pos;
            line.start.y = offset.y + origin_x.y;
            line.end.x = offset.x + origin_x.x + ruler_pos;
            line.end.y = offset.y + origin_x.y + settings.ruler_width;
            draw_line(line.start, line.end, settings.ticks_color);
            char label[200];
            sprintf(label, "%d", i);
            line.start.x += 5.0;
            draw_text(line.start, settings.ticks_color, label);
            for (int j=0; j<10; j++) {
                ruler_pos += major_tick_seperation_scaled/10.0;
                EmbLine tick;
                tick.start.x = offset.x + origin_x.x + ruler_pos;
                tick.start.y = offset.y + origin_x.y + settings.tick_depth;
                tick.end.x = offset.x + origin_x.x + ruler_pos;
                tick.end.y = offset.y + origin_x.y + settings.ruler_width;
                draw_line(tick.start, tick.end, settings.ticks_color);
            }
        }
        for (int i=-repeats; i<repeats; i++) {
            float ruler_pos = settings.ruler_width + major_tick_seperation_scaled*i;
            EmbLine line;
            line.start.x = offset.x + origin_y.x;
            line.start.y = offset.y + origin_y.y + ruler_pos;
            line.end.x = offset.x + origin_y.x + settings.ruler_width;
            line.end.y = offset.y + origin_y.y + ruler_pos;
            draw_line(line.start, line.end, settings.ticks_color);
            char label[200];
            sprintf(label, "%d", i);
            line.start.x += 5.0;
            draw_text(line.start, settings.ticks_color, label);
            for (int j=0; j<10; j++) {
                ruler_pos += major_tick_seperation_scaled/10.0;
                EmbLine tick;
                tick.start.x = offset.x + origin_y.x + settings.tick_depth;
                tick.start.y = offset.y + origin_y.y + ruler_pos;
                tick.end.x = offset.x + origin_y.x + settings.ruler_width;
                tick.end.y = offset.y + origin_y.y + ruler_pos;
                draw_line(tick.start, tick.end, settings.ticks_color);
            }
        }
    }
    else {
            int repeats = 100;
        for (int i=0; i<repeats; i++) {
            float ruler_pos = settings.ruler_width + settings.major_tick_seperation*i;
            EmbVector start, end;
            start = offset;
            end = offset;
            start.x += ruler_pos;
            end.x += ruler_pos;
            end.y += settings.ruler_width;
            draw_line(start, end, ticks_color);
            char label[200];
            sprintf(label, "%d", i);
            start.x += 5.0;
            draw_text(start, ticks_color, label);
            for (int j=0; j<16; j++) {
                ruler_pos += settings.major_tick_seperation/16.0;
                draw_list->AddLine(
                    to_EmbVector(offset) + EmbVector(ruler_pos, settings.tick_depth),
                    to_EmbVector(offset) + EmbVector(ruler_pos, settings.ruler_width),
                    ticks_color
                );
            }
        }
        for (int i=0; i<repeats; i++) {
            float ruler_pos = settings.ruler_width + settings.major_tick_seperation*i;
            draw_list->AddLine(
                to_EmbVector(offset) + EmbVector(0.0, ruler_pos),
                to_EmbVector(offset) + EmbVector(ruler_width, ruler_pos),
                ticks_color
            );
            char label[200];
            sprintf(label, "%d", i);
            draw_list->AddText(
                to_EmbVector(offset) + EmbVector(0.0, ruler_pos),
                ticks_color, label);
            for (int j=0; j<16; j++) {
                ruler_pos += settings.major_tick_seperation/16.0;
                draw_list->AddLine(
                    to_EmbVector(offset) + EmbVector(settings.tick_depth, ruler_pos),
                    to_EmbVector(offset) + EmbVector(settings.ruler_width, ruler_pos),
                    ticks_color
                );
            }
        }
        }

    EmbVector dim = {settings.ruler_width, settings.ruler_width};
    EmbVector corner = embVector_add(offset, dim);
    draw_rect(offset, corner, settings.ruler_color, true);
}

void
draw_grid(void)
{
    EmbVector offset = get_window_pos();
    EmbVector screen_size = get_screen_size();
    for (float x=offset.x; x<screen_size.x; x+=settings.grid_spacing.x) {
        EmbVector start, end;
        start.x = x;
        start.y = offset.y;
        end.x = x;
        end.y = screen_size.y;
        draw_line(start, end, settings.grid_color);
    }
    for (float y=offset.y; y<screen_size.y; y+=settings.grid_spacing.y) {
        EmbVector start, end;
        start.x = offset.x;
        start.y = y;
        end.x = screen_size.x;
        end.y = y;
        draw_line(start, end, settings.grid_color);
    }
}

void
pattern_view(void)
{
    EmbPattern *pattern = views[settings.pattern_index].pattern;
    ImGui::BeginChild(views[settings.pattern_index].filename);
    if (views[settings.pattern_index].grid_mode) {
        draw_grid();
    }
    if (views[settings.pattern_index].lwt_mode) {
        real_render_pattern(pattern);
    }
    else {
        if (views[settings.pattern_index].simulate) {
            simulate_pattern(pattern);
        }
        else {
            render_pattern(pattern);
        }
    }
    if (views[settings.pattern_index].ruler_mode) {
        draw_rulers();
    }
    ImGui::EndChild();
}

/**
 * @brief Create a new view to represent the file being edited.
 *
 * Returns EmbView A properly initialised view.
 *
 * On creation, view claims the active_view and therefore captures
 * key presses and mouse actions.
 *
 * \todo Originally, EM2 allowed for multiple views of the same file,
 * we can allow for this, but currently means we have multiple pattern
 * pointers. So we'd need to set the memory locations to be the same place.
 *
 * \todo Dropping files onto dock on Mac to open them?
 *
 * \todo automatically maximises, but it could allow for
 * multiple views, panes and windows.
 */
EmbView
init_view(void)
{
    (const int theIndex, MainWindow* mw, QMdiArea* parent, int wflags)

    fileWasLoaded = false;

    setAttribute(Qt_WA_DeleteOnClose);

    this->setWindowIcon(load_icon("app"));

    active_view = QGraphicsScene(0, 0, 0, 0, this);
    gview = EmbView(mainWin, active_view, this);

    setWidget(gview);

    gview->setFocusProxy(prompt);

    resize(sizeHint());

    promptHistory = "Welcome to Embroidermodder 2!<br/>Open some of our sample files. Many formats are supported.<br/>For help, press F1.";
    prompt->setHistory(promptHistory);
    promptInputList << "";
    promptInputNum = 0;

    curLayer = "0";
    curColor = 0; //\todo color ByLayer
    curLineType = "ByLayer";
    curLineWeight = "ByLayer";

    show();
    showMaximized();

    setFocusPolicy(Qt_WheelFocus);
    setFocus();

    onWindowActivated();

    view.useLogo = false;
    view.useTexture = false;
    view.useColor = false;
    view.bgLogo = "";
    view.bgTexture = "";
    view.bgColor = IM_COL32(40, 20, 10, 255);
    EmbView view;
    view.origin.x = 400.0;
    view.origin.y = 300.0;
    view.scale = 5.0;
    strcpy(view.grid_type, "RECTANGULAR");
    view.snap_mode = true;
    view.grid_mode = true;
    view.ruler_mode = true;
    view.ortho_mode = false;
    view.polar_mode = false;
    view.qsnap_mode = false;
    view.qtrack_mode = false;
    view.lwt_mode = false;
    view.metric = true;
    view.simulate = false;
    view.simulation_start = clock();
    strcpy(view.text_font, "default");
    view.text_size = 16;
    view.text_angle = 0.0;
    view.text_style_bold = false;
    view.text_style_italic = false;
    view.text_style_underline = false;
    view.text_style_overline = false;
    view.text_style_strikeout = false;
    if (n_views > 0) {
        sprintf(view.filename, "Untitled%02d.dst", n_views);
    }
    else {
        strcpy(view.filename, "Untitled.dst");
    }
    view.undo_history.position = 0;
    view.undo_history.length = 0;
    view.undo_history.max_length = 100;
    view.pattern = embPattern_create();
    view.rubber_mode = OBJ_RUBBER_OFF;

    return view;
}

/**
 * @brief Renderer for the interface of the settings editor, called by main_widget.
 */
void
settings_editor(void)
{
    if (just_opened) {
        dialog = settings;
        preview = dialog;
        just_opened = false;
    }
    // ImGuiTabItemFlags_SetSelected
    begin_ui("Settings", 750, 550);
    if (ImGui::BeginTabBar("Tab Bar")) {
        if (ImGui::BeginTabItem("General")) {
            create_tab_general();
            ImGui::EndTabItem();
        }
        if (ImGui::BeginTabItem("Files/Paths")) {
            create_tab_files_paths();
            ImGui::EndTabItem();
        }
        if (ImGui::BeginTabItem("Display")) {
            create_tab_display();
            ImGui::EndTabItem();
        }
        if (ImGui::BeginTabItem("Open/Save")) {
            create_tab_open_save();
            ImGui::EndTabItem();
        }
        if (ImGui::BeginTabItem("Printing")) {
            create_tab_printing();
            ImGui::EndTabItem();
        }
        if (ImGui::BeginTabItem("Snap")) {
            create_tab_snap();
            ImGui::EndTabItem();
        }
        if (ImGui::BeginTabItem("Grid/Ruler")) {
            create_tab_grid_ruler();
            ImGui::EndTabItem();
        }
        if (ImGui::BeginTabItem("Ortho/Polar")) {
            create_tab_ortho_polar();
            ImGui::EndTabItem();
        }
        if (ImGui::BeginTabItem("QuickSnap")) {
            create_tab_quicksnap();
            ImGui::EndTabItem();
        }
        if (ImGui::BeginTabItem("QuickTrack")) {
            create_tab_quicktrack();
            ImGui::EndTabItem();
        }
        if (ImGui::BeginTabItem("LineWeight")) {
            create_tab_lineweight();
            ImGui::EndTabItem();
        }
        if (ImGui::BeginTabItem("Selection")) {
            create_tab_selection();
            ImGui::EndTabItem();
        }
        ImGui::EndTabBar();
    }

    if (button("Accept")) {
        log_debug("Writing Settings...");
        write_settings(settings_file);
        settings = dialog;
        settings.show_settings_editor = false;
    }
    ImGui::SameLine();
    if (button("Cancel")) {
        settings.show_settings_editor = false;
    }
    ImGui::EndUi();
}

/**
 * @brief The language strings for identifying where to look for
 * translation tables.
 *
 * \todo this should be loaded from configuration.
 */
const char *languages[20] = {
    "default",
    "spanish",
    "french",
    "END"
};

/**
 * @brief Create a tab general object
 *
 */
void
create_tab_general(void)
{
    translated_text("Language");

    /* \todo Detect what the system language is, so we can default
     *     to that when it can be detected. */
    if (ImGui::BeginCombo("Language", dialog.language)) {
        for (int i=0; ; i++) {
            if (streq(languages[i], "END")) {
                break;
            }
            bool current_language = streq(dialog.language, languages[i]);
            if (ImGui::Selectable(languages[i], current_language)) {
                strcpy(dialog.language, languages[i]);
                dialog.use_translation = true;
            }
        }
        ImGui::EndCombo();
    }
    QLabel* labelLanguage = QLabel(translate("Language (Requires Restart)"), groupBoxLanguage);
    int* comboBoxLanguage = int(groupBoxLanguage);
    dialog.general_language = settings_general_language;
    comboBoxLanguage->addItem("Default");
    comboBoxLanguage->addItem("System");
    comboBoxLanguage->insertSeparator(2);
    QDir trDir(qApp->applicationDirPath());
    trDir.cd("translations");
    for(const char *dirName, trDir.entryList(QDir_Dirs | QDir_NoDotAndDotDot))
    {
        dirName[0] = dirName[0].toUpper();
        comboBoxLanguage->addItem(dirName);
    }
    const char *current = dialog.general_language;
    current[0] = current[0].toUpper();
    comboBoxLanguage->setCurrentIndex(comboBoxLanguage->findText(current));

    translated_text("Icons");

    if (ImGui_BeginCombo(translate("Icon Theme"), settings.icon_size)) {
        QDir dir(qApp->applicationDirPath());
        dir.cd("icons");
        dialog.general_icon_theme.string_value = settings_general_icon_theme;
        for (const char *dirName, dir.entryList(QDir_Dirs | QDir_NoDotAndDotDot)) {
            comboBoxIconTheme->addItem(QIcon("icons/" + dirName + "/" + "theme" + ".png"), dirName);
        }
        comboBoxIconTheme->setCurrentIndex(comboBoxIconTheme->findText(dialog.general_icon_theme.string_value));
        ImGui_EndCombo();
    }

    if (ImGui_BeginCombo(translate("Icon Size"), settings.icon_size)) {
        const char *s = "icons/" + dialog.general_icon_theme.string_value + "/icon16.png";
        comboBoxIconSize->addItem(QIcon(s), "Very Small", 16);
        s = "icons/" + dialog.general_icon_theme.string_value + "/icon24.png";
        comboBoxIconSize->addItem(QIcon(s), "Small", 24);
        s = "icons/" + dialog.general_icon_theme.string_value + "/icon32.png";
        comboBoxIconSize->addItem(QIcon(s), "Medium",     32);
        s = "icons/" + dialog.general_icon_theme.string_value + "/icon48.png";
        comboBoxIconSize->addItem(QIcon(s), "Large",      48);
        s = "icons/" + dialog.general_icon_theme.string_value + "/icon64.png";
        comboBoxIconSize->addItem(QIcon(s), "Very Large", 64);
        s = "icons/" + dialog.general_icon_theme.string_value + "/icon128.png";
        comboBoxIconSize->addItem(QIcon(s), "I'm Blind", 128);
        dialog.general_icon_size = settings_general_icon_size;
        comboBoxIconSize->setCurrentIndex(comboBoxIconSize->findData(dialog.general_icon_size));
        ImGui_EndCombo();
    }

    translated_text("Background");
    checkbox_data("Use Logo", &(dialog.general_mdi_bg_use_logo));

    if (button("Choose") {
        buttonMdiBGLogo->setEnabled(dialog.general_mdi_bg_use_logo);
        dialog.general_mdi_bg_logo = settings_general_mdi_bg_logo;
        accept_general_mdi_bg_logo = dialog.general_mdi_bg_logo;
        connect(buttonMdiBGLogo, clicked(), chooseGeneralMdiBackgroundLogo()));
        connect(checkBoxMdiBGUseLogo, toggled(bool)), buttonMdiBGLogo, setEnabled(bool)));
    }

    checkbox_data("Use Texture", &(dialog.general_mdi_bg_use_texture));

    if (button("Choose")) {
        buttonMdiBGTexture->setEnabled(dialog.general_mdi_bg_use_texture);
        dialog.general_mdi_bg_texture  = settings_general_mdi_bg_texture;
        accept_general_mdi_bg_texture  = dialog.general_mdi_bg_texture;
        connect(buttonMdiBGTexture, clicked(), chooseGeneralMdiBackgroundTexture()));
        connect(checkBoxMdiBGUseTexture, toggled(bool)), buttonMdiBGTexture, setEnabled(bool)));
    }

    checkbox_data("Use Color", &(dialog.general_mdi_bg_use_color));

    if (button("Choose")) {
        buttonMdiBGColor->setEnabled(dialog.general_mdi_bg_use_color);
        dialog.general_mdi_bg_color = settings_general_mdi_bg_color;
        preview.general_mdi_bg_color = dialog.general_mdi_bg_color;
        accept_general_mdi_bg_color = dialog.general_mdi_bg_color;
        QPixmap mdiBGPix(16,16);
        mdiBGPix.fill(EmbColor(preview.general_mdi_bg_color));
        buttonMdiBGColor->setIcon(QIcon(mdiBGPix));
        connect(buttonMdiBGColor, clicked(), chooseGeneralMdiBackgroundColor()));
        connect(checkBoxMdiBGUseColor, toggled(bool)), buttonMdiBGColor, setEnabled(bool)));
    }

    QGridLayout* gridLayoutMdiBG = QGridLayout(widget);
    gridLayoutMdiBG->add_widget(checkBoxMdiBGUseLogo,    0, 0, EMB_ALIGN_LEFT);
    gridLayoutMdiBG->add_widget(buttonMdiBGLogo,         0, 1, EMB_ALIGN_RIGHT);
    gridLayoutMdiBG->add_widget(checkBoxMdiBGUseTexture, 1, 0, EMB_ALIGN_LEFT);
    gridLayoutMdiBG->add_widget(buttonMdiBGTexture,      1, 1, EMB_ALIGN_RIGHT);
    gridLayoutMdiBG->add_widget(checkBoxMdiBGUseColor,   2, 0, EMB_ALIGN_LEFT);
    gridLayoutMdiBG->add_widget(buttonMdiBGColor,        2, 1, EMB_ALIGN_RIGHT);
    groupBoxMdiBG->set_layout(gridLayoutMdiBG);

    translated_text("Tips");

    checkbox_data("Show Tip of the Day on startup", &(dialog.general_tip_of_the_day));

    translated_text("Help Browser");

    QRadioButton* radioButtonSystemHelpBrowser = QRadioButton(translate("System"), groupBoxHelpBrowser);
    radioButtonSystemHelpBrowser_set_checked(settings_general_system_help_browser);
    QRadioButton* radioButtonCustomHelpBrowser = QRadioButton(translate("Custom"), groupBoxHelpBrowser);
    radioButtonCustomHelpBrowser_set_checked(!settings_general_system_help_browser);
    radioButtonCustomHelpBrowser->setEnabled(false); //\todo finish this

    QVBoxLayout* vboxLayoutHelpBrowser = QVBoxLayout(groupBoxHelpBrowser);
    vboxLayoutHelpBrowser->add_widget(radioButtonSystemHelpBrowser);
    vboxLayoutHelpBrowser->add_widget(radioButtonCustomHelpBrowser);
    groupBoxHelpBrowser->set_layout(vboxLayoutHelpBrowser);

    /* Widget Layout */
    QVBoxLayout* vboxLayoutMain = QVBoxLayout(widget);
    vboxLayoutMain->add_widget(groupBoxLanguage);
    vboxLayoutMain->add_widget(groupBoxIcon);
    vboxLayoutMain->add_widget(groupBoxMdiBG);
    vboxLayoutMain->add_widget(groupBoxTips);
    vboxLayoutMain->add_widget(groupBoxHelpBrowser);
    vboxLayoutMain->addStretch(1);
    widget->set_layout(vboxLayoutMain);
}

/**
 * @brief Create a tab files paths object
 *
 */
void
create_tab_files_paths(void)
{

}

/**
 * @brief Create a tab display object
 *
 */
void
create_tab_display(void)
{
    //\todo Review OpenGL and Rendering settings for future inclusion
    translated_text("Rendering");

    bool dialog_display_use_opengl;
    bool dialog_display_renderhint_aa;
    bool dialog_display_renderhint_text_aa;
    bool dialog_display_renderhint_smooth_pix;
    bool dialog_display_renderhint_high_aa;
    bool dialog_display_renderhint_noncosmetic;

    bool dialog_display_show_scrollbars;

    checkbox_data("Use OpenGL", &dialog_display_use_opengl, &(dialog.display_use_opengl));
    checkbox_data("Antialias", &dialog_display_renderhint_aa, &(dialog.display_renderhint_aa));
    checkbox_data("Antialias Text", &dialog_display_renderhint_text_aa, &(dialog.display_renderhint_text_aa));
    checkbox_data("Smooth Pixmap", &dialog_display_renderhint_smooth_pix, &(dialog.display_renderhint_smooth_pix));
    checkbox_data("High Quality Antialiasing (OpenGL)", &dialog_display_renderhint_high_aa, &(dialog.display_renderhint_high_aa));
    checkbox_data("Non Cosmetic", &dialog_display_renderhint_noncosmetic, &(dialog.display_renderhint_noncosmetic));

    translated_text("Scroll Bars");

    checkbox_data("Show scrollbars", &dialog_display_show_scrollbars, &(dialog.display_show_scrollbars));
    QLabel* labelscrollbarWidget = QLabel(translate("Perform action when clicking corner widget"), groupBoxscrollbars);
    int* comboBoxscrollbarWidget = int(groupBoxscrollbars);
    dialog.display_scrollbar_widget_num = settings_display_scrollbar_widget_num;
    int numActions = actionHash.size();
    for(int i = 0; i < numActions; i++)
    {
        QAction* action = actionHash.value(i);
        if (action) comboBoxscrollbarWidget->addItem(action->icon(), action->text().replace("&", ""));
    }
    comboBoxscrollbarWidget->setCurrentIndex(dialog.display_scrollbar_widget_num);
    connect(comboBoxscrollbarWidget, currentIndexChanged(int), comboBoxscrollbarWidgetCurrentIndexChanged(int)));

    QVBoxLayout* vboxLayoutscrollbars = QVBoxLayout(groupBoxscrollbars);
    vboxLayoutscrollbars->add_widget(checkBoxShowscrollbars);
    vboxLayoutscrollbars->add_widget(labelscrollbarWidget);
    vboxLayoutscrollbars->add_widget(comboBoxscrollbarWidget);
    groupBoxscrollbars->set_layout(vboxLayoutscrollbars);

    translated_text("Colors");

    translated_text("Crosshair Color");
    QPushButton* buttonCrossHairColor = QPushButton(translate("Choose"), groupBoxColor);
    dialog.display_crosshair_color = settings_display_crosshair_color;
    preview.display_crosshair_color = dialog.display_crosshair_color;
    accept_display_crosshair_color = dialog.display_crosshair_color;
    QPixmap crosshairPix(16,16);
    crosshairPix.fill(EmbColor(preview.display_crosshair_color));
    buttonCrossHairColor->setIcon(QIcon(crosshairPix));
    connect(buttonCrossHairColor, clicked(), chooseDisplayCrossHairColor()));

    translated_text("Background Color");
    QPushButton* buttonBGColor = QPushButton(translate("Choose"), groupBoxColor);
    dialog.display_bg_color = settings_display_bg_color;
    preview.display_bg_color = dialog.display_bg_color;
    accept_display_bg_color = dialog.display_bg_color;
    QPixmap bgPix(16,16);
    bgPix.fill(EmbColor(preview.display_bg_color));
    buttonBGColor->setIcon(QIcon(bgPix));
    connect(buttonBGColor, clicked(), chooseDisplayBackgroundColor()));

    translated_text("Selection Box Color (Crossing)");
    QPushButton* buttonSelectBoxLeftColor = QPushButton(translate("Choose"), groupBoxColor);
    dialog.display_selectbox_left_color = settings_display_selectbox_left_color;
    preview.display_selectbox_left_color = dialog.display_selectbox_left_color;
    accept_display_selectbox_left_color = dialog.display_selectbox_left_color;
    QPixmap sBoxLCPix(16,16);
    sBoxLCPix.fill(EmbColor(preview.display_selectbox_left_color));
    buttonSelectBoxLeftColor->setIcon(QIcon(sBoxLCPix));
    connect(buttonSelectBoxLeftColor, clicked(), chooseDisplaySelectBoxLeftColor()));

    translated_text("Selection Box Fill (Crossing)");
    QPushButton* buttonSelectBoxLeftFill = QPushButton(translate("Choose"), groupBoxColor);
    dialog.display_selectbox_left_fill = settings_display_selectbox_left_fill;
    preview.display_selectbox_left_fill = dialog.display_selectbox_left_fill;
    accept_display_selectbox_left_fill = dialog.display_selectbox_left_fill;
    QPixmap sBoxLFPix(16,16);
    sBoxLFPix.fill(EmbColor(preview.display_selectbox_left_fill));
    buttonSelectBoxLeftFill->setIcon(QIcon(sBoxLFPix));
    connect(buttonSelectBoxLeftFill, clicked(), chooseDisplaySelectBoxLeftFill()));

    translated_text("Selection Box Color (Window)");
    button(translate("Choose"), groupBoxColor);
    dialog.display_selectbox_right_color = settings_display_selectbox_right_color;
    preview.display_selectbox_right_color = dialog.display_selectbox_right_color;
    accept_display_selectbox_right_color = dialog.display_selectbox_right_color;
    QPixmap sBoxRCPix(16,16);
    sBoxRCPix.fill(EmbColor(preview.display_selectbox_right_color));
    buttonSelectBoxRightColor->setIcon(QIcon(sBoxRCPix));
    connect(buttonSelectBoxRightColor, clicked(), chooseDisplaySelectBoxRightColor()));

    translated_text("Selection Box Fill (Window)"));
    button(translate("Choose"), groupBoxColor);
    dialog.display_selectbox_right_fill = settings_display_selectbox_right_fill;
    preview.display_selectbox_right_fill = dialog.display_selectbox_right_fill;
    accept_display_selectbox_right_fill = dialog.display_selectbox_right_fill;
    QPixmap sBoxRFPix(16,16);
    sBoxRFPix.fill(EmbColor(preview.display_selectbox_right_fill));
    buttonSelectBoxRightFill->setIcon(QIcon(sBoxRFPix));
    connect(buttonSelectBoxRightFill, clicked(), chooseDisplaySelectBoxRightFill()));

    translated_text("Selection Box Fill Alpha");
    QSpinBox* spinBoxSelectBoxAlpha = QSpinBox(groupBoxColor);
    spinBoxSelectBoxAlpha->setRange(0, 255);
    dialog.display_selectbox_alpha = settings_display_selectbox_alpha;
    preview.display_selectbox_alpha = dialog.display_selectbox_alpha;
    spinBoxSelectBoxAlpha->setValue(preview.display_selectbox_alpha);
    connect(spinBoxSelectBoxAlpha, valueChanged(int), spinBoxDisplaySelectBoxAlphaValueChanged(int)));

    QGridLayout* gridLayoutColor = QGridLayout(widget);
    gridLayoutColor->add_widget(labelCrossHairColor,       0, 0, EMB_ALIGN_LEFT);
    gridLayoutColor->add_widget(buttonCrossHairColor,      0, 1, EMB_ALIGN_RIGHT);
    gridLayoutColor->add_widget(labelBGColor,              1, 0, EMB_ALIGN_LEFT);
    gridLayoutColor->add_widget(buttonBGColor,             1, 1, EMB_ALIGN_RIGHT);
    gridLayoutColor->add_widget(labelSelectBoxLeftColor,   2, 0, EMB_ALIGN_LEFT);
    gridLayoutColor->add_widget(buttonSelectBoxLeftColor,  2, 1, EMB_ALIGN_RIGHT);
    gridLayoutColor->add_widget(labelSelectBoxLeftFill,    3, 0, EMB_ALIGN_LEFT);
    gridLayoutColor->add_widget(buttonSelectBoxLeftFill,   3, 1, EMB_ALIGN_RIGHT);
    gridLayoutColor->add_widget(labelSelectBoxRightColor,  4, 0, EMB_ALIGN_LEFT);
    gridLayoutColor->add_widget(buttonSelectBoxRightColor, 4, 1, EMB_ALIGN_RIGHT);
    gridLayoutColor->add_widget(labelSelectBoxRightFill,   5, 0, EMB_ALIGN_LEFT);
    gridLayoutColor->add_widget(buttonSelectBoxRightFill,  5, 1, EMB_ALIGN_RIGHT);
    gridLayoutColor->add_widget(labelSelectBoxAlpha,       6, 0, EMB_ALIGN_LEFT);
    gridLayoutColor->add_widget(spinBoxSelectBoxAlpha,     6, 1, EMB_ALIGN_RIGHT);
    groupBoxColor->set_layout(gridLayoutColor);

    translated_text("Zoom");

    translated_text(translate("Zoom In Scale"), groupBoxZoom);
    QDoubleSpinBox* spinBoxZoomScaleIn = QDoubleSpinBox(groupBoxZoom);
    dialog.display_zoomscale_in = settings_display_zoomscale_in;
    spinBoxZoomScaleIn->setValue(dialog.display_zoomscale_in);
    spinBoxZoomScaleIn->setSingleStep(0.01);
    spinBoxZoomScaleIn->setRange(1.01, 10.00);
    connect(spinBoxZoomScaleIn, valueChanged(EmbReal), spinBoxZoomScaleInValueChanged(EmbReal)));

    translated_text(translate("Zoom Out Scale"), groupBoxZoom);
    QDoubleSpinBox* spinBoxZoomScaleOut = QDoubleSpinBox(groupBoxZoom);
    dialog.display_zoomscale_out = settings_display_zoomscale_out;
    spinBoxZoomScaleOut->setValue(dialog.display_zoomscale_out);
    spinBoxZoomScaleOut->setSingleStep(0.01);
    spinBoxZoomScaleOut->setRange(0.01, 0.99);
    connect(spinBoxZoomScaleOut, valueChanged(EmbReal), spinBoxZoomScaleOutValueChanged(EmbReal)));

    QGridLayout* gridLayoutZoom = QGridLayout(groupBoxZoom);
    gridLayoutZoom->add_widget(labelZoomScaleIn,    0, 0, EMB_ALIGN_LEFT);
    gridLayoutZoom->add_widget(spinBoxZoomScaleIn,  0, 1, EMB_ALIGN_RIGHT);
    gridLayoutZoom->add_widget(labelZoomScaleOut,   1, 0, EMB_ALIGN_LEFT);
    gridLayoutZoom->add_widget(spinBoxZoomScaleOut, 1, 1, EMB_ALIGN_RIGHT);
    groupBoxZoom->set_layout(gridLayoutZoom);
}

/**
 * @brief Create a tab open save object
 *
 */
void
create_tab_open_save(void)
{
    char extensions[100][20] = {
        "100", "10o", "ART", "BMC", "BRO", "CND", "COL", "CSD",
        "CSV", "DAT", "DEM", "DSB", "DST", "DSZ", "DXF", "EDR",
        "EMD", "EXP", "EXY", "EYS", "FXY", "GNC", "GT", "HUS",
        "INB", "JEF", "KSM", "PCD", "PCM", "PCQ", "PCS", "PEC",
        "PEL", "PEM", "PES", "PHB", "PHC", "RGB", "SEW", "SHV",
        "SST", "STX", "SVG", "T09", "TAP", "THR", "TXT",
        "U00", "U01", "VIP", "VP3", "XXX", "ZSK"
    };

    printf("%s", extensions[100]);

    //Custom Filter
    QGroupBox* groupBoxCustomFilter = QGroupBox(translate("Custom Filter"), widget);
    groupBoxCustomFilter->setEnabled(false); //\todo Fixup custom filter

    dialog.opensave_custom_filter = settings_opensave_custom_filter;

    QCheckBox* checkBoxCustomFilter[200];

    for (std_vector<std_string>_size_type i=0; i<extensions.size(); i++) {
        checkBoxCustomFilter[i] = QCheckBox(extensions[i],
                                                groupBoxCustomFilter);
        bool checked = dialog.opensave_custom_filter.contains
            "*."+extensions[i],
            Qt_CaseInsensitive);
        checkBoxCustomFilter[i]_set_checked(checked);
        connect(checkBoxCustomFilter[i], stateChanged(int)), this,
 checkBoxCustomFilterStateChanged(int)));
    }

    QPushButton* buttonCustomFilterSelectAll = QPushButton(translate("Select All"), groupBoxCustomFilter);
    connect(buttonCustomFilterSelectAll, clicked(), buttonCustomFilterSelectAllClicked()));
    for (std_vector<std_string>_size_type i=0; i<extensions.size(); i++) {
        connect(buttonCustomFilterSelectAll(bool)), checkBoxCustomFilter[i], setChecked(bool)));
    }

    QPushButton* buttonCustomFilterClearAll = QPushButton("Clear All", groupBoxCustomFilter);
    connect(buttonCustomFilterClearAll, clicked(), buttonCustomFilterClearAllClicked()));
    for (std_vector<std_string>_size_type i=0; i<extensions.size(); i++) {
        connect(buttonCustomFilterClearAll(bool)), checkBoxCustomFilter[i], setChecked(bool)));
    }

    QGridLayout* gridLayoutCustomFilter = QGridLayout(groupBoxCustomFilter);
    for (int i=0; i<10; i++)
    for (int j=0; j<10; j++) {
        if (10*i+j >= extensions.size()) {
            break;
        }
        gridLayoutCustomFilter->add_widget(checkBoxCustomFilter[10*i+j], j, i, EMB_ALIGN_LEFT);
    }
    gridLayoutCustomFilter->add_widget(buttonCustomFilterSelectAll, 0, 6, EMB_ALIGN_LEFT);
    gridLayoutCustomFilter->add_widget(buttonCustomFilterClearAll,  1, 6, EMB_ALIGN_LEFT);
    gridLayoutCustomFilter->setColumnStretch(6,1);
    groupBoxCustomFilter->set_layout(gridLayoutCustomFilter);

    if (dialog.opensave_custom_filter.contains("supported", Qt_CaseInsensitive)) buttonCustomFilterSelectAllClicked();

    //Opening
    QGroupBox* groupBoxOpening = QGroupBox(translate("File Open"), widget);

    int* comboBoxOpenFormat = int(groupBoxOpening);

    QCheckBox* checkBoxOpenThumbnail = QCheckBox(translate("Preview Thumbnails"), groupBoxOpening);
    checkBoxOpenThumbnail_set_checked(false);

    //\todo Add a button to clear the recent history.

    QLabel* labelRecentMaxFiles = QLabel(translate("Number of recently accessed files to show"), groupBoxOpening);
    QSpinBox* spinBoxRecentMaxFiles = QSpinBox(groupBoxOpening);
    spinBoxRecentMaxFiles->setRange(0, 10);
    dialog.opensave_recent_max_files = settings_opensave_recent_max_files;
    spinBoxRecentMaxFiles->setValue(dialog.opensave_recent_max_files);
    connect(spinBoxRecentMaxFiles, valueChanged(int), spinBoxRecentMaxFilesValueChanged(int)));

    QFrame* frameRecent = QFrame(groupBoxOpening);
    QGridLayout* gridLayoutRecent = QGridLayout(frameRecent);
    gridLayoutRecent->add_widget(labelRecentMaxFiles,   0, 0, EMB_ALIGN_LEFT);
    gridLayoutRecent->add_widget(spinBoxRecentMaxFiles, 0, 1, EMB_ALIGN_RIGHT);
    frameRecent->set_layout(gridLayoutRecent);

    QVBoxLayout* vboxLayoutOpening = QVBoxLayout(groupBoxOpening);
    vboxLayoutOpening->add_widget(comboBoxOpenFormat);
    vboxLayoutOpening->add_widget(checkBoxOpenThumbnail);
    vboxLayoutOpening->add_widget(frameRecent);
    groupBoxOpening->set_layout(vboxLayoutOpening);

    translated_text("File Save");

    int* comboBoxSaveFormat = int(groupBoxSaving);

    QCheckBox* checkBoxSaveThumbnail = QCheckBox(translate("Save Thumbnails"), groupBoxSaving);
    checkBoxSaveThumbnail_set_checked(false);

    QCheckBox* checkBoxAutoSave = QCheckBox(translate("AutoSave"), groupBoxSaving);
    checkBoxAutoSave_set_checked(false);

    QVBoxLayout* vboxLayoutSaving = QVBoxLayout(groupBoxSaving);
    vboxLayoutSaving->add_widget(comboBoxSaveFormat);
    vboxLayoutSaving->add_widget(checkBoxSaveThumbnail);
    vboxLayoutSaving->add_widget(checkBoxAutoSave);
    groupBoxSaving->set_layout(vboxLayoutSaving);

    //Trimming
    QGroupBox* groupBoxTrim = QGroupBox(translate("Trimming"), widget);

    QLabel* labelTrimDstNumJumps = QLabel(translate("DST Only: Minimum number of jumps to trim"), groupBoxTrim);
    QSpinBox* spinBoxTrimDstNumJumps = QSpinBox(groupBoxTrim);
    spinBoxTrimDstNumJumps->setRange(1, 20);
    dialog.opensave_trim_dst_num_jumps = settings_opensave_trim_dst_num_jumps;
    spinBoxTrimDstNumJumps->setValue(dialog.opensave_trim_dst_num_jumps);
    connect(spinBoxTrimDstNumJumps, valueChanged(int), spinBoxTrimDstNumJumpsValueChanged(int)));

    QFrame* frameTrimDstNumJumps = QFrame(groupBoxTrim);
    QGridLayout* gridLayoutTrimDstNumJumps = QGridLayout(frameTrimDstNumJumps);
    gridLayoutTrimDstNumJumps->add_widget(labelTrimDstNumJumps,   0, 0, EMB_ALIGN_LEFT);
    gridLayoutTrimDstNumJumps->add_widget(spinBoxTrimDstNumJumps, 0, 1, EMB_ALIGN_RIGHT);
    frameTrimDstNumJumps->set_layout(gridLayoutTrimDstNumJumps);

    QVBoxLayout* vboxLayoutTrim = QVBoxLayout(groupBoxTrim);
    vboxLayoutTrim->add_widget(frameTrimDstNumJumps);
    groupBoxTrim->set_layout(vboxLayoutTrim);
}

/**
 * @brief Create a tab printing object
 *
 */
void
create_tab_printing(void)
{
    translated_text("Default Printer");
    int checked;
    ImGui_RadioButton(translate("Use as default device"), &checked);
    ImGui_RadioButton(translate("Use last used device"), &checked);

    ImGui_ComboBox(translate("Available Printers"), &checked);
    std_vector<PrinterInfo> listAvailPrinters = PrinterInfo_availablePrinters();
    for (PrinterInfo info : listAvailPrinters) {
        comboBoxDefaultDevice->addItem(load_icon("print"), info.printerName());
    }

    translated_text("Save Ink");
    checkbox_data("Disable Background", &(dialog.printing_disable_bg));
}

/**
 * @brief Create a tab snap object
 *
 */
void
create_tab_snap(void)
{

}

/**
 * @brief Create a tab grid ruler object
 *
 */
void
create_tab_grid_ruler(void)
{
    translated_text("Grid Misc");

    checkBox(translate("Initially show grid when loading a file"), groupBoxGridMisc);
    dialog.grid_show_on_load = settings_grid_show_on_load;
    checkBoxGridShowOnLoad_set_checked(dialog.grid_show_on_load);
    connect(checkBoxGridShowOnLoad, stateChanged(int), checkBoxGridShowOnLoadStateChanged(int)));

    QCheckBox* checkBoxGridShowOrigin = QCheckBox(translate("Show the origin when the grid is enabled"), groupBoxGridMisc);
    dialog.grid_show_origin = settings_grid_show_origin;
    checkBoxGridShowOrigin_set_checked(dialog.grid_show_origin);
    connect(checkBoxGridShowOrigin, stateChanged(int), checkBoxGridShowOriginStateChanged(int)));

    QGridLayout* gridLayoutGridMisc = QGridLayout(widget);
    gridLayoutGridMisc->add_widget(checkBoxGridShowOnLoad, 0, 0, EMB_ALIGN_LEFT);
    gridLayoutGridMisc->add_widget(checkBoxGridShowOrigin, 1, 0, EMB_ALIGN_LEFT);
    groupBoxGridMisc->set_layout(gridLayoutGridMisc);

    //Grid Color
    QGroupBox* groupBoxGridColor = QGroupBox(translate("Grid Color"), widget);

    QCheckBox* checkBoxGridColorMatchCrossHair = QCheckBox(translate("Match grid color to crosshair color"), groupBoxGridColor);
    dialog.grid_color_match_crosshair = settings_grid_color_match_crosshair;
    checkBoxGridColorMatchCrossHair_set_checked(dialog.grid_color_match_crosshair);
    connect(checkBoxGridColorMatchCrossHair, stateChanged(int), checkBoxGridColorMatchCrossHairStateChanged(int)));

    QLabel* labelGridColor = QLabel(translate("Grid Color"), groupBoxGridColor);
    labelGridColor->setObjectName("labelGridColor");
    QPushButton* buttonGridColor = QPushButton(translate("Choose"), groupBoxGridColor);
    buttonGridColor->setObjectName("buttonGridColor");
    if (dialog.grid_color_match_crosshair) {
        dialog.grid_color = settings_display_crosshair_color;
    }
    else {
        dialog.grid_color = settings_grid_color;
    }
    preview.grid_color = dialog.grid_color;
    accept_grid_color = dialog.grid_color;
    QPixmap gridPix(16,16);
    gridPix.fill(EmbColor(preview.grid_color));
    buttonGridColor->setIcon(QIcon(gridPix));
    connect(buttonGridColor, clicked(), chooseGridColor()));

    labelGridColor->setEnabled(!dialog.grid_color_match_crosshair);
    buttonGridColor->setEnabled(!dialog.grid_color_match_crosshair);

    QGridLayout* gridLayoutGridColor = QGridLayout(widget);
    gridLayoutGridColor->add_widget(checkBoxGridColorMatchCrossHair, 0, 0, EMB_ALIGN_LEFT);
    gridLayoutGridColor->add_widget(labelGridColor,                  1, 0, EMB_ALIGN_LEFT);
    gridLayoutGridColor->add_widget(buttonGridColor,                 1, 1, EMB_ALIGN_RIGHT);
    groupBoxGridColor->set_layout(gridLayoutGridColor);

    //Grid Geometry
    QGroupBox* groupBoxGridGeom = QGroupBox(translate("Grid Geometry"), widget);

    QCheckBox* checkBoxGridLoadFromFile = QCheckBox(translate("Set grid size from opened file"), groupBoxGridGeom);
    dialog.grid_load_from_file = settings_grid_load_from_file;
    checkBoxGridLoadFromFile_set_checked(dialog.grid_load_from_file);
    connect(checkBoxGridLoadFromFile, stateChanged(int), checkBoxGridLoadFromFileStateChanged(int)));

    QLabel* labelGridType = QLabel(translate("Grid Type"), groupBoxGridGeom);
    labelGridType->setObjectName("labelGridType");
    int* comboBoxGridType = int(groupBoxGridGeom);
    comboBoxGridType->setObjectName("comboBoxGridType");
    comboBoxGridType->addItem("Rectangular");
    comboBoxGridType->addItem("Circular");
    comboBoxGridType->addItem("Isometric");
    dialog.grid_type = settings_grid_type;
    comboBoxGridType->setCurrentIndex(comboBoxGridType->findText(dialog.grid_type));
    connect(comboBoxGridType, currentIndexChanged(const char *), comboBoxGridTypeCurrentIndexChanged(const char *)));

    QCheckBox* checkBoxGridCenterOnOrigin = QCheckBox(translate("Center the grid on the origin"), groupBoxGridGeom);
    checkBoxGridCenterOnOrigin->setObjectName("checkBoxGridCenterOnOrigin");
    dialog.grid_center_on_origin = settings_grid_center_on_origin;
    checkBoxGridCenterOnOrigin_set_checked(dialog.grid_center_on_origin);
    connect(checkBoxGridCenterOnOrigin, stateChanged(int), checkBoxGridCenterOnOriginStateChanged(int)));

    QLabel* labelGridCenterX = QLabel(translate("Grid Center X"), groupBoxGridGeom);
    labelGridCenterX->setObjectName("labelGridCenterX");
    QDoubleSpinBox* spinBoxGridCenterX = QDoubleSpinBox(groupBoxGridGeom);
    spinBoxGridCenterX->setObjectName("spinBoxGridCenterX");
    dialog.grid_center.x = settings.grid_center.x;
    spinBoxGridCenterX->setSingleStep(1.000);
    spinBoxGridCenterX->setRange(-1000.000, 1000.000);
    spinBoxGridCenterX->setValue(dialog.grid_center.x);
    connect(spinBoxGridCenterX, valueChanged(EmbReal), spinBoxGridCenterXValueChanged(EmbReal)));

    QLabel* labelGridCenterY = QLabel(translate("Grid Center Y"), groupBoxGridGeom);
    labelGridCenterY->setObjectName("labelGridCenterY");
    QDoubleSpinBox* spinBoxGridCenterY = QDoubleSpinBox(groupBoxGridGeom);
    spinBoxGridCenterY->setObjectName("spinBoxGridCenterY");
    dialog.grid_center_y = settings_grid_center_y;
    spinBoxGridCenterY->setSingleStep(1.000);
    spinBoxGridCenterY->setRange(-1000.000, 1000.000);
    spinBoxGridCenterY->setValue(dialog.grid_center_y);
    connect(spinBoxGridCenterY, valueChanged(EmbReal), spinBoxGridCenterYValueChanged(EmbReal)));

    QLabel* labelGridSizeX = QLabel(translate("Grid Size X"), groupBoxGridGeom);
    labelGridSizeX->setObjectName("labelGridSizeX");
    QDoubleSpinBox* spinBoxGridSizeX = QDoubleSpinBox(groupBoxGridGeom);
    spinBoxGridSizeX->setObjectName("spinBoxGridSizeX");
    dialog.grid_size_x = settings_grid_size_x;
    spinBoxGridSizeX->setSingleStep(1.000);
    spinBoxGridSizeX->setRange(1.000, 1000.000);
    spinBoxGridSizeX->setValue(dialog.grid_size_x);
    connect(spinBoxGridSizeX, valueChanged(EmbReal), spinBoxGridSizeXValueChanged(EmbReal)));

    QLabel* labelGridSizeY = QLabel(translate("Grid Size Y"), groupBoxGridGeom);
    labelGridSizeY->setObjectName("labelGridSizeY");
    QDoubleSpinBox* spinBoxGridSizeY = QDoubleSpinBox(groupBoxGridGeom);
    spinBoxGridSizeY->setObjectName("spinBoxGridSizeY");
    dialog.grid_size_y = settings_grid_size_y;
    spinBoxGridSizeY->setSingleStep(1.000);
    spinBoxGridSizeY->setRange(1.000, 1000.000);
    spinBoxGridSizeY->setValue(dialog.grid_size_y);
    connect(spinBoxGridSizeY, valueChanged(EmbReal), spinBoxGridSizeYValueChanged(EmbReal)));

    QLabel* labelGridSpacingX = QLabel(translate("Grid Spacing X"), groupBoxGridGeom);
    labelGridSpacingX->setObjectName("labelGridSpacingX");
    QDoubleSpinBox* spinBoxGridSpacingX = QDoubleSpinBox(groupBoxGridGeom);
    spinBoxGridSpacingX->setObjectName("spinBoxGridSpacingX");
    dialog.grid_spacing_x = settings_grid_spacing_x;
    spinBoxGridSpacingX->setSingleStep(1.000);
    spinBoxGridSpacingX->setRange(0.001, 1000.000);
    spinBoxGridSpacingX->setValue(dialog.grid_spacing_x);
    connect(spinBoxGridSpacingX, valueChanged(EmbReal), spinBoxGridSpacingXValueChanged(EmbReal)));

    QLabel* labelGridSpacingY = QLabel(translate("Grid Spacing Y"), groupBoxGridGeom);
    labelGridSpacingY->setObjectName("labelGridSpacingY");
    QDoubleSpinBox* spinBoxGridSpacingY = QDoubleSpinBox(groupBoxGridGeom);
    spinBoxGridSpacingY->setObjectName("spinBoxGridSpacingY");
    dialog.grid_spacing_y = settings_grid_spacing_y;
    spinBoxGridSpacingY->setSingleStep(1.000);
    spinBoxGridSpacingY->setRange(0.001, 1000.000);
    spinBoxGridSpacingY->setValue(dialog.grid_spacing_y);
    connect(spinBoxGridSpacingY, valueChanged(EmbReal), spinBoxGridSpacingYValueChanged(EmbReal)));

    QLabel* labelGridSizeRadius = QLabel(translate("Grid Size Radius"), groupBoxGridGeom);
    labelGridSizeRadius->setObjectName("labelGridSizeRadius");
    QDoubleSpinBox* spinBoxGridSizeRadius = QDoubleSpinBox(groupBoxGridGeom);
    spinBoxGridSizeRadius->setObjectName("spinBoxGridSizeRadius");
    dialog.grid_size_radius = settings_grid_size_radius;
    spinBoxGridSizeRadius->setSingleStep(1.000);
    spinBoxGridSizeRadius->setRange(1.000, 1000.000);
    spinBoxGridSizeRadius->setValue(dialog.grid_size_radius);
    connect(spinBoxGridSizeRadius, valueChanged(EmbReal), spinBoxGridSizeRadiusValueChanged(EmbReal)));

    QLabel* labelGridSpacingRadius = QLabel(translate("Grid Spacing Radius"), groupBoxGridGeom);
    labelGridSpacingRadius->setObjectName("labelGridSpacingRadius");
    QDoubleSpinBox* spinBoxGridSpacingRadius = QDoubleSpinBox(groupBoxGridGeom);
    spinBoxGridSpacingRadius->setObjectName("spinBoxGridSpacingRadius");
    dialog.grid_spacing_radius = settings_grid_spacing_radius;
    spinBoxGridSpacingRadius->setSingleStep(1.000);
    spinBoxGridSpacingRadius->setRange(0.001, 1000.000);
    spinBoxGridSpacingRadius->setValue(dialog.grid_spacing_radius);
    connect(spinBoxGridSpacingRadius, valueChanged(EmbReal), spinBoxGridSpacingRadiusValueChanged(EmbReal)));

    QLabel* labelGridSpacingAngle = QLabel(translate("Grid Spacing Angle"), groupBoxGridGeom);
    labelGridSpacingAngle->setObjectName("labelGridSpacingAngle");
    QDoubleSpinBox* spinBoxGridSpacingAngle = QDoubleSpinBox(groupBoxGridGeom);
    spinBoxGridSpacingAngle->setObjectName("spinBoxGridSpacingAngle");
    dialog.grid_spacing_angle = settings_grid_spacing_angle;
    spinBoxGridSpacingAngle->setSingleStep(1.000);
    spinBoxGridSpacingAngle->setRange(0.001, 1000.000);
    spinBoxGridSpacingAngle->setValue(dialog.grid_spacing_angle);
    connect(spinBoxGridSpacingAngle, valueChanged(EmbReal), spinBoxGridSpacingAngleValueChanged(EmbReal)));

    labelGridType->setEnabled(!dialog.grid_load_from_file);
    comboBoxGridType->setEnabled(!dialog.grid_load_from_file);
    checkBoxGridCenterOnOrigin->setEnabled(!dialog.grid_load_from_file);
    labelGridCenterX->setEnabled(!dialog.grid_load_from_file);
    spinBoxGridCenterX->setEnabled(!dialog.grid_load_from_file);
    labelGridCenterY->setEnabled(!dialog.grid_load_from_file);
    spinBoxGridCenterY->setEnabled(!dialog.grid_load_from_file);
    labelGridSizeX->setEnabled(!dialog.grid_load_from_file);
    spinBoxGridSizeX->setEnabled(!dialog.grid_load_from_file);
    labelGridSizeY->setEnabled(!dialog.grid_load_from_file);
    spinBoxGridSizeY->setEnabled(!dialog.grid_load_from_file);
    labelGridSpacingX->setEnabled(!dialog.grid_load_from_file);
    spinBoxGridSpacingX->setEnabled(!dialog.grid_load_from_file);
    labelGridSpacingY->setEnabled(!dialog.grid_load_from_file);
    spinBoxGridSpacingY->setEnabled(!dialog.grid_load_from_file);
    labelGridSizeRadius->setEnabled(!dialog.grid_load_from_file);
    spinBoxGridSizeRadius->setEnabled(!dialog.grid_load_from_file);
    labelGridSpacingRadius->setEnabled(!dialog.grid_load_from_file);
    spinBoxGridSpacingRadius->setEnabled(!dialog.grid_load_from_file);
    labelGridSpacingAngle->setEnabled(!dialog.grid_load_from_file);
    spinBoxGridSpacingAngle->setEnabled(!dialog.grid_load_from_file);

    bool visibility = false;
    if (dialog.grid_type == "Circular") visibility = true;
    labelGridSizeX->setVisible(!visibility);
    spinBoxGridSizeX->setVisible(!visibility);
    labelGridSizeY->setVisible(!visibility);
    spinBoxGridSizeY->setVisible(!visibility);
    labelGridSpacingX->setVisible(!visibility);
    spinBoxGridSpacingX->setVisible(!visibility);
    labelGridSpacingY->setVisible(!visibility);
    spinBoxGridSpacingY->setVisible(!visibility);
    labelGridSizeRadius->setVisible(visibility);
    spinBoxGridSizeRadius->setVisible(visibility);
    labelGridSpacingRadius->setVisible(visibility);
    spinBoxGridSpacingRadius->setVisible(visibility);
    labelGridSpacingAngle->setVisible(visibility);
    spinBoxGridSpacingAngle->setVisible(visibility);

    QGridLayout* gridLayoutGridGeom = QGridLayout(groupBoxGridGeom);
    gridLayoutGridGeom->add_widget(checkBoxGridLoadFromFile,   0, 0, EMB_ALIGN_LEFT);
    gridLayoutGridGeom->add_widget(labelGridType,              1, 0, EMB_ALIGN_LEFT);
    gridLayoutGridGeom->add_widget(comboBoxGridType,           1, 1, EMB_ALIGN_RIGHT);
    gridLayoutGridGeom->add_widget(checkBoxGridCenterOnOrigin, 2, 0, EMB_ALIGN_LEFT);
    gridLayoutGridGeom->add_widget(labelGridCenterX,           3, 0, EMB_ALIGN_LEFT);
    gridLayoutGridGeom->add_widget(spinBoxGridCenterX,         3, 1, EMB_ALIGN_RIGHT);
    gridLayoutGridGeom->add_widget(labelGridCenterY,           4, 0, EMB_ALIGN_LEFT);
    gridLayoutGridGeom->add_widget(spinBoxGridCenterY,         4, 1, EMB_ALIGN_RIGHT);
    gridLayoutGridGeom->add_widget(labelGridSizeX,             5, 0, EMB_ALIGN_LEFT);
    gridLayoutGridGeom->add_widget(spinBoxGridSizeX,           5, 1, EMB_ALIGN_RIGHT);
    gridLayoutGridGeom->add_widget(labelGridSizeY,             6, 0, EMB_ALIGN_LEFT);
    gridLayoutGridGeom->add_widget(spinBoxGridSizeY,           6, 1, EMB_ALIGN_RIGHT);
    gridLayoutGridGeom->add_widget(labelGridSpacingX,          7, 0, EMB_ALIGN_LEFT);
    gridLayoutGridGeom->add_widget(spinBoxGridSpacingX,        7, 1, EMB_ALIGN_RIGHT);
    gridLayoutGridGeom->add_widget(labelGridSpacingY,          8, 0, EMB_ALIGN_LEFT);
    gridLayoutGridGeom->add_widget(spinBoxGridSpacingY,        8, 1, EMB_ALIGN_RIGHT);
    gridLayoutGridGeom->add_widget(labelGridSizeRadius,        9, 0, EMB_ALIGN_LEFT);
    gridLayoutGridGeom->add_widget(spinBoxGridSizeRadius,      9, 1, EMB_ALIGN_RIGHT);
    gridLayoutGridGeom->add_widget(labelGridSpacingRadius,    10, 0, EMB_ALIGN_LEFT);
    gridLayoutGridGeom->add_widget(spinBoxGridSpacingRadius,  10, 1, EMB_ALIGN_RIGHT);
    gridLayoutGridGeom->add_widget(labelGridSpacingAngle,     11, 0, EMB_ALIGN_LEFT);
    gridLayoutGridGeom->add_widget(spinBoxGridSpacingAngle,   11, 1, EMB_ALIGN_RIGHT);
    groupBoxGridGeom->set_layout(gridLayoutGridGeom);

    //Ruler Misc
    QGroupBox* groupBoxRulerMisc = QGroupBox(translate("Ruler Misc"), widget);

    QCheckBox* checkBoxRulerShowOnLoad = QCheckBox(translate("Initially show ruler when loading a file"), groupBoxRulerMisc);
    dialog.ruler_show_on_load = settings_ruler_show_on_load;
    checkBoxRulerShowOnLoad_set_checked(dialog.ruler_show_on_load);
    connect(checkBoxRulerShowOnLoad, stateChanged(int), checkBoxRulerShowOnLoadStateChanged(int)));

    QLabel* labelRulerMetric = QLabel(translate("Ruler Units"), groupBoxRulerMisc);
    int* comboBoxRulerMetric = int(groupBoxRulerMisc);
    comboBoxRulerMetric->addItem("Imperial", false);
    comboBoxRulerMetric->addItem("Metric",   true);
    dialog.ruler_metric = settings_ruler_metric;
    comboBoxRulerMetric->setCurrentIndex(comboBoxRulerMetric->findData(dialog.ruler_metric));
    connect(comboBoxRulerMetric, currentIndexChanged(int), comboBoxRulerMetricCurrentIndexChanged(int)));

    QGridLayout* gridLayoutRulerMisc = QGridLayout(widget);
    gridLayoutRulerMisc->add_widget(checkBoxRulerShowOnLoad, 0, 0, EMB_ALIGN_LEFT);
    gridLayoutRulerMisc->add_widget(labelRulerMetric,        1, 0, EMB_ALIGN_LEFT);
    gridLayoutRulerMisc->add_widget(comboBoxRulerMetric,     1, 1, EMB_ALIGN_RIGHT);
    groupBoxRulerMisc->set_layout(gridLayoutRulerMisc);

    //Ruler Color
    QGroupBox* groupBoxRulerColor = QGroupBox(translate("Ruler Color"), widget);

    QLabel* labelRulerColor = QLabel(translate("Ruler Color"), groupBoxRulerColor);
    labelRulerColor->setObjectName("labelRulerColor");
    QPushButton* buttonRulerColor = QPushButton(translate("Choose"), groupBoxRulerColor);
    buttonRulerColor->setObjectName("buttonRulerColor");
    dialog.ruler_color = settings_ruler_color;
    preview.ruler_color = dialog.ruler_color;
    accept_ruler_color  = dialog.ruler_color;
    QPixmap rulerPix(16,16);
    rulerPix.fill(EmbColor(preview.ruler_color));
    buttonRulerColor->setIcon(QIcon(rulerPix));
    connect(buttonRulerColor, clicked(), chooseRulerColor()));

    QGridLayout* gridLayoutRulerColor = QGridLayout(widget);
    gridLayoutRulerColor->add_widget(labelRulerColor,  1, 0, EMB_ALIGN_LEFT);
    gridLayoutRulerColor->add_widget(buttonRulerColor, 1, 1, EMB_ALIGN_RIGHT);
    groupBoxRulerColor->set_layout(gridLayoutRulerColor);

    //Ruler Geometry
    QGroupBox* groupBoxRulerGeom = QGroupBox(translate("Ruler Geometry"), widget);

    QLabel* labelRulerPixelSize = QLabel(translate("Ruler Pixel Size"), groupBoxRulerGeom);
    labelRulerPixelSize->setObjectName("labelRulerPixelSize");
    QDoubleSpinBox* spinBoxRulerPixelSize = QDoubleSpinBox(groupBoxRulerGeom);
    spinBoxRulerPixelSize->setObjectName("spinBoxRulerPixelSize");
    dialog.ruler_pixel_size = settings_ruler_pixel_size;
    spinBoxRulerPixelSize->setSingleStep(1.000);
    spinBoxRulerPixelSize->setRange(20.000, 100.000);
    spinBoxRulerPixelSize->setValue(dialog.ruler_pixel_size);
    connect(spinBoxRulerPixelSize, valueChanged(EmbReal), spinBoxRulerPixelSizeValueChanged(EmbReal)));

    QGridLayout* gridLayoutRulerGeom = QGridLayout(groupBoxRulerGeom);
    gridLayoutRulerGeom->add_widget(labelRulerPixelSize,   0, 0, EMB_ALIGN_LEFT);
    gridLayoutRulerGeom->add_widget(spinBoxRulerPixelSize, 0, 1, EMB_ALIGN_RIGHT);
    groupBoxRulerGeom->set_layout(gridLayoutRulerGeom);

    //Widget Layout
    QVBoxLayout *vboxLayoutMain = QVBoxLayout(widget);
    vboxLayoutMain->add_widget(groupBoxGridMisc);
    vboxLayoutMain->add_widget(groupBoxGridColor);
    vboxLayoutMain->add_widget(groupBoxGridGeom);
    vboxLayoutMain->add_widget(groupBoxRulerMisc);
    vboxLayoutMain->add_widget(groupBoxRulerColor);
    vboxLayoutMain->add_widget(groupBoxRulerGeom);
    vboxLayoutMain->addStretch(1);
    widget->set_layout(vboxLayoutMain);

    QScrollArea* scrollArea = QScrollArea(this);
    scrollArea->setWidgetResizable(true);
    scrollArea->setWidget(widget);
    return scrollArea;
}

/**
 * @brief Create a tab ortho polar object
 *
 */
void
create_tab_ortho_polar(void)
{
    //\todo finish this

    QScrollArea* scrollArea = QScrollArea(this);
    scrollArea->setWidgetResizable(true);
    scrollArea->setWidget(widget);
}

/**
 * @brief Create a tab quicksnap object
 *
 */
void
create_tab_quicksnap(void)
{
    const char *iconTheme = settings_general_icon_theme;

    //QSnap Locators
    QGroupBox* groupBoxQSnapLoc = QGroupBox(translate("Locators Used"), widget);

    QCheckBox* checkBoxQSnapEndPoint = QCheckBox(translate("Endpoint"), groupBoxQSnapLoc);
    dialog.qsnap_endpoint = settings_qsnap_endpoint;
    checkBoxQSnapEndPoint_set_checked(dialog.qsnap_endpoint);
    checkBoxQSnapEndPoint->setIcon(load_icon("locator-snaptoendpoint"));
    connect(checkBoxQSnapEndPoint, stateChanged(int), checkBoxQSnapEndPointStateChanged(int)));

    QCheckBox* checkBoxQSnapMidPoint = QCheckBox(translate("Midpoint"), groupBoxQSnapLoc);
    dialog.qsnap_midpoint = settings_qsnap_midpoint;
    checkBoxQSnapMidPoint_set_checked(dialog.qsnap_midpoint);
    checkBoxQSnapMidPoint->setIcon(load_icon("locator-snaptomidpoint"));
    connect(checkBoxQSnapMidPoint, stateChanged(int), checkBoxQSnapMidPointStateChanged(int)));

    QCheckBox* checkBoxQSnapCenter = QCheckBox(translate("Center"), groupBoxQSnapLoc);
    dialog.qsnap_center = settings_qsnap_center;
    checkBoxQSnapCenter_set_checked(dialog.qsnap_center);
    checkBoxQSnapCenter->setIcon(load_icon("locator-snaptocenter"));
    connect(checkBoxQSnapCenter, stateChanged(int), checkBoxQSnapCenterStateChanged(int)));

    QCheckBox* checkBoxQSnapNode = QCheckBox(translate("Node"), groupBoxQSnapLoc);
    dialog.qsnap_node = settings_qsnap_node;
    checkBoxQSnapNode_set_checked(dialog.qsnap_node);
    checkBoxQSnapNode->setIcon(load_icon("locator-snaptonode"));
    connect(checkBoxQSnapNode, stateChanged(int), checkBoxQSnapNodeStateChanged(int)));

    QCheckBox* checkBoxQSnapQuadrant = QCheckBox(translate("Quadrant"), groupBoxQSnapLoc);
    dialog.qsnap_quadrant = settings_qsnap_quadrant;
    checkBoxQSnapQuadrant_set_checked(dialog.qsnap_quadrant);
    checkBoxQSnapQuadrant->setIcon(QIcon("icons/" + iconTheme + "/" + "locator-snaptoquadrant" + ".png"));
    connect(checkBoxQSnapQuadrant, stateChanged(int), checkBoxQSnapQuadrantStateChanged(int)));

    QCheckBox* checkBoxQSnapIntersection = QCheckBox(translate("Intersection"), groupBoxQSnapLoc);
    dialog.qsnap_intersection = settings_qsnap_intersection;
    checkBoxQSnapIntersection_set_checked(dialog.qsnap_intersection);
    checkBoxQSnapIntersection->setIcon(QIcon("icons/" + iconTheme + "/" + "locator-snaptointersection" + ".png"));
    connect(checkBoxQSnapIntersection, stateChanged(int), checkBoxQSnapIntersectionStateChanged(int)));

    QCheckBox* checkBoxQSnapExtension = QCheckBox(translate("Extension"), groupBoxQSnapLoc);
    dialog.qsnap_extension = settings_qsnap_extension;
    checkBoxQSnapExtension_set_checked(dialog.qsnap_extension);
    checkBoxQSnapExtension->setIcon(QIcon("icons/" + iconTheme + "/" + "locator-snaptoextension" + ".png"));
    connect(checkBoxQSnapExtension, stateChanged(int), checkBoxQSnapExtensionStateChanged(int)));

    QCheckBox* checkBoxQSnapInsertion = QCheckBox(translate("Insertion"), groupBoxQSnapLoc);
    dialog.qsnap_insertion = settings_qsnap_insertion;
    checkBoxQSnapInsertion_set_checked(dialog.qsnap_insertion);
    checkBoxQSnapInsertion->setIcon(QIcon("icons/" + iconTheme + "/" + "locator-snaptoinsert" + ".png"));
    connect(checkBoxQSnapInsertion, stateChanged(int), checkBoxQSnapInsertionStateChanged(int)));

    QCheckBox* checkBoxQSnapPerpendicular = QCheckBox(translate("Perpendicular"), groupBoxQSnapLoc);
    dialog.qsnap_perpendicular = settings_qsnap_perpendicular;
    checkBoxQSnapPerpendicular_set_checked(dialog.qsnap_perpendicular);
    checkBoxQSnapPerpendicular->setIcon(QIcon("icons/" + iconTheme + "/" + "locator-snaptoperpendicular" + ".png"));
    connect(checkBoxQSnapPerpendicular, stateChanged(int), checkBoxQSnapPerpendicularStateChanged(int)));

    QCheckBox* checkBoxQSnapTangent = QCheckBox(translate("Tangent"), groupBoxQSnapLoc);
    dialog.qsnap_tangent = settings_qsnap_tangent;
    checkBoxQSnapTangent_set_checked(dialog.qsnap_tangent);
    checkBoxQSnapTangent->setIcon(QIcon("icons/" + iconTheme + "/" + "locator-snaptotangent" + ".png"));
    connect(checkBoxQSnapTangent, stateChanged(int), checkBoxQSnapTangentStateChanged(int)));

    QCheckBox* checkBoxQSnapNearest = QCheckBox(translate("Nearest"), groupBoxQSnapLoc);
    dialog.qsnap_nearest = settings_qsnap_nearest;
    checkBoxQSnapNearest_set_checked(dialog.qsnap_nearest);
    checkBoxQSnapNearest->setIcon(load_icon("locator-snaptonearest"));
    connect(checkBoxQSnapNearest, stateChanged(int), checkBoxQSnapNearestStateChanged(int)));

    QCheckBox* checkBoxQSnapApparent = QCheckBox(translate("Apparent Intersection"), groupBoxQSnapLoc);
    dialog.qsnap_apparent = settings_qsnap_apparent;
    checkBoxQSnapApparent_set_checked(dialog.qsnap_apparent);
    checkBoxQSnapApparent->setIcon(load_icon("locator-snaptoapparentintersection"));
    connect(checkBoxQSnapApparent, stateChanged(int), checkBoxQSnapApparentStateChanged(int)));

    QCheckBox* checkBoxQSnapParallel = QCheckBox(translate("Parallel"), groupBoxQSnapLoc);
    dialog.qsnap_parallel = settings_qsnap_parallel;
    checkBoxQSnapParallel_set_checked(dialog.qsnap_parallel);
    checkBoxQSnapParallel->setIcon(load_icon("locator-snaptoparallel"));
    connect(checkBoxQSnapParallel, stateChanged(int), checkBoxQSnapParallelStateChanged(int)));

    QPushButton* buttonQSnapSelectAll = QPushButton(translate("Select All"), groupBoxQSnapLoc);
    connect(buttonQSnapSelectAll, clicked(), buttonQSnapSelectAllClicked()));
    connect(buttonQSnapSelectAll(bool)), checkBoxQSnapEndPoint, setChecked(bool)));
    connect(buttonQSnapSelectAll(bool)), checkBoxQSnapMidPoint, setChecked(bool)));
    connect(buttonQSnapSelectAll(bool)), checkBoxQSnapCenter, setChecked(bool)));
    connect(buttonQSnapSelectAll(bool)), checkBoxQSnapNode, setChecked(bool)));
    connect(buttonQSnapSelectAll(bool)), checkBoxQSnapQuadrant, setChecked(bool)));
    connect(buttonQSnapSelectAll(bool)), checkBoxQSnapIntersection, setChecked(bool)));
    connect(buttonQSnapSelectAll(bool)), checkBoxQSnapExtension, setChecked(bool)));
    connect(buttonQSnapSelectAll(bool)), checkBoxQSnapInsertion, setChecked(bool)));
    connect(buttonQSnapSelectAll(bool)), checkBoxQSnapPerpendicular, setChecked(bool)));
    connect(buttonQSnapSelectAll(bool)), checkBoxQSnapTangent, setChecked(bool)));
    connect(buttonQSnapSelectAll(bool)), checkBoxQSnapNearest, setChecked(bool)));
    connect(buttonQSnapSelectAll(bool)), checkBoxQSnapApparent, setChecked(bool)));
    connect(buttonQSnapSelectAll(bool)), checkBoxQSnapParallel, setChecked(bool)));

    QPushButton* buttonQSnapClearAll = QPushButton(translate("Clear All"), groupBoxQSnapLoc);
    connect(buttonQSnapClearAll, clicked(), buttonQSnapClearAllClicked()));
    connect(buttonQSnapClearAll(bool)), checkBoxQSnapEndPoint, setChecked(bool)));
    connect(buttonQSnapClearAll(bool)), checkBoxQSnapMidPoint, setChecked(bool)));
    connect(buttonQSnapClearAll(bool)), checkBoxQSnapCenter, setChecked(bool)));
    connect(buttonQSnapClearAll(bool)), checkBoxQSnapNode, setChecked(bool)));
    connect(buttonQSnapClearAll(bool)), checkBoxQSnapQuadrant, setChecked(bool)));
    connect(buttonQSnapClearAll(bool)), checkBoxQSnapIntersection, setChecked(bool)));
    connect(buttonQSnapClearAll(bool)), checkBoxQSnapExtension, setChecked(bool)));
    connect(buttonQSnapClearAll(bool)), checkBoxQSnapInsertion, setChecked(bool)));
    connect(buttonQSnapClearAll(bool)), checkBoxQSnapPerpendicular, setChecked(bool)));
    connect(buttonQSnapClearAll(bool)), checkBoxQSnapTangent, setChecked(bool)));
    connect(buttonQSnapClearAll(bool)), checkBoxQSnapNearest, setChecked(bool)));
    connect(buttonQSnapClearAll(bool)), checkBoxQSnapApparent, setChecked(bool)));
    connect(buttonQSnapClearAll(bool)), checkBoxQSnapParallel, setChecked(bool)));

    QGridLayout* gridLayoutQSnap = QGridLayout(groupBoxQSnapLoc);
    gridLayoutQSnap->add_widget(checkBoxQSnapEndPoint,      0, 0, EMB_ALIGN_LEFT);
    gridLayoutQSnap->add_widget(checkBoxQSnapMidPoint,      1, 0, EMB_ALIGN_LEFT);
    gridLayoutQSnap->add_widget(checkBoxQSnapCenter,        2, 0, EMB_ALIGN_LEFT);
    gridLayoutQSnap->add_widget(checkBoxQSnapNode,          3, 0, EMB_ALIGN_LEFT);
    gridLayoutQSnap->add_widget(checkBoxQSnapQuadrant,      4, 0, EMB_ALIGN_LEFT);
    gridLayoutQSnap->add_widget(checkBoxQSnapIntersection,  5, 0, EMB_ALIGN_LEFT);
    gridLayoutQSnap->add_widget(checkBoxQSnapExtension,     6, 0, EMB_ALIGN_LEFT);
    gridLayoutQSnap->add_widget(checkBoxQSnapInsertion,     0, 1, EMB_ALIGN_LEFT);
    gridLayoutQSnap->add_widget(checkBoxQSnapPerpendicular, 1, 1, EMB_ALIGN_LEFT);
    gridLayoutQSnap->add_widget(checkBoxQSnapTangent,       2, 1, EMB_ALIGN_LEFT);
    gridLayoutQSnap->add_widget(checkBoxQSnapNearest,       3, 1, EMB_ALIGN_LEFT);
    gridLayoutQSnap->add_widget(checkBoxQSnapApparent,      4, 1, EMB_ALIGN_LEFT);
    gridLayoutQSnap->add_widget(checkBoxQSnapParallel,      5, 1, EMB_ALIGN_LEFT);
    gridLayoutQSnap->add_widget(buttonQSnapSelectAll,       0, 2, EMB_ALIGN_LEFT);
    gridLayoutQSnap->add_widget(buttonQSnapClearAll,        1, 2, EMB_ALIGN_LEFT);
    gridLayoutQSnap->setColumnStretch(2,1);
    groupBoxQSnapLoc->set_layout(gridLayoutQSnap);

    //QSnap Visual Config
    QGroupBox* groupBoxQSnapVisual = QGroupBox(translate("Visual Configuration"), widget);

    QLabel* labelQSnapLocColor = QLabel(translate("Locator Color"), groupBoxQSnapVisual);
    int* comboBoxQSnapLocColor = int(groupBoxQSnapVisual);
    addColorsToComboBox(comboBoxQSnapLocColor);
    dialog.qsnap_locator_color = settings_qsnap_locator_color;
    comboBoxQSnapLocColor->setCurrentIndex(comboBoxQSnapLocColor->findData(dialog.qsnap_locator_color));
    connect(comboBoxQSnapLocColor, currentIndexChanged(int), comboBoxQSnapLocatorColorCurrentIndexChanged(int)));

    QLabel* labelQSnapLocSize = QLabel(translate("Locator Size"), groupBoxQSnapVisual);
    QSlider* sliderQSnapLocSize = QSlider(Qt_Horizontal, groupBoxQSnapVisual);
    sliderQSnapLocSize->setRange(1,20);
    dialog.qsnap_locator_size = settings_qsnap_locator_size;
    sliderQSnapLocSize->setValue(dialog.qsnap_locator_size);
    connect(sliderQSnapLocSize, valueChanged(int), sliderQSnapLocatorSizeValueChanged(int)));

    QVBoxLayout* vboxLayoutQSnapVisual = QVBoxLayout(groupBoxQSnapVisual);
    vboxLayoutQSnapVisual->add_widget(labelQSnapLocColor);
    vboxLayoutQSnapVisual->add_widget(comboBoxQSnapLocColor);
    vboxLayoutQSnapVisual->add_widget(labelQSnapLocSize);
    vboxLayoutQSnapVisual->add_widget(sliderQSnapLocSize);
    groupBoxQSnapVisual->set_layout(vboxLayoutQSnapVisual);

    //QSnap Sensitivity Config
    QGroupBox* groupBoxQSnapSensitivity = QGroupBox(translate("Sensitivity"), widget);

    QLabel* labelQSnapApertureSize = QLabel(translate("Aperture Size"), groupBoxQSnapSensitivity);
    QSlider* sliderQSnapApertureSize = QSlider(Qt_Horizontal, groupBoxQSnapSensitivity);
    sliderQSnapApertureSize->setRange(1,20);
    dialog.qsnap_aperture_size = settings_qsnap_aperture_size;
    sliderQSnapApertureSize->setValue(dialog.qsnap_aperture_size);
    connect(sliderQSnapApertureSize, valueChanged(int), sliderQSnapApertureSizeValueChanged(int)));

    QVBoxLayout* vboxLayoutQSnapSensitivity = QVBoxLayout(groupBoxQSnapSensitivity);
    vboxLayoutQSnapSensitivity->add_widget(labelQSnapApertureSize);
    vboxLayoutQSnapSensitivity->add_widget(sliderQSnapApertureSize);
    groupBoxQSnapSensitivity->set_layout(vboxLayoutQSnapSensitivity);

    //Widget Layout
    QVBoxLayout *vboxLayoutMain = QVBoxLayout(widget);
    vboxLayoutMain->add_widget(groupBoxQSnapLoc);
    vboxLayoutMain->add_widget(groupBoxQSnapVisual);
    vboxLayoutMain->add_widget(groupBoxQSnapSensitivity);
    vboxLayoutMain->addStretch(1);
    widget->set_layout(vboxLayoutMain);

    QScrollArea* scrollArea = QScrollArea(this);
    scrollArea->setWidgetResizable(true);
    scrollArea->setWidget(widget);
    return scrollArea;
}

/**
 * @brief Create a tab quicktrack object
 *
 */
void
create_tab_quicktrack(void)
{
    QWidget* widget = QWidget(this);

    //\todo finish this

    QScrollArea* scrollArea = QScrollArea(this);
    scrollArea->setWidgetResizable(true);
    scrollArea->setWidget(widget);
    return scrollArea;
}

/**
 * @brief Create a tab lineweight object
 *
 * \todo finish this
 */
void
create_tab_lineweight(void)
{
    QWidget* widget = QWidget(this);

    /* Misc */
    QGroupBox* groupBoxLwtMisc = QGroupBox(translate("LineWeight Misc"), widget);

    QGraphicsScene* s = activeScene();

    QCheckBox* checkBoxShowLwt = QCheckBox(translate("Show LineWeight"), groupBoxLwtMisc);
    if (s) {
        dialog.lwt_show_lwt = s->property(ENABLE_LWT).toBool();
    }
    else {
        dialog.lwt_show_lwt = settings_lwt_show_lwt;
    }
    preview.lwt_show_lwt = dialog.lwt_show_lwt;
    checkBoxShowLwt_set_checked(preview.lwt_show_lwt);
    connect(checkBoxShowLwt, stateChanged(int), checkBoxLwtShowLwtStateChanged(int)));

    QCheckBox* checkBoxRealRender = QCheckBox(translate("RealRender"), groupBoxLwtMisc);
    checkBoxRealRender->setObjectName("checkBoxRealRender");
    if (s) {
        dialog.lwt_real_render = s->property(ENABLE_REAL).toBool();
    }
    else {
        dialog.lwt_real_render = settings_lwt_real_render;
    }
    preview.lwt_real_render = dialog.lwt_real_render;
    checkBoxRealRender_set_checked(preview.lwt_real_render);
    connect(checkBoxRealRender, stateChanged(int), checkBoxLwtRealRenderStateChanged(int)));
    checkBoxRealRender->setEnabled(dialog.lwt_show_lwt);

    QLabel* labelDefaultLwt = QLabel(translate("Default weight"), groupBoxLwtMisc);
    labelDefaultLwt->setEnabled(false); //\todo remove later
    int* comboBoxDefaultLwt = int(groupBoxLwtMisc);
    dialog.lwt_default_lwt = settings_lwt_default_lwt;
    //\todo populate the comboBox and set the initial value
    comboBoxDefaultLwt->addItem(std_string().setNum(dialog.lwt_default_lwt, 'F', 2).append(" mm"), dialog.lwt_default_lwt);
    comboBoxDefaultLwt->setEnabled(false); //\todo remove later

    QVBoxLayout* vboxLayoutLwtMisc = QVBoxLayout(groupBoxLwtMisc);
    vboxLayoutLwtMisc->add_widget(checkBoxShowLwt);
    vboxLayoutLwtMisc->add_widget(checkBoxRealRender);
    vboxLayoutLwtMisc->add_widget(labelDefaultLwt);
    vboxLayoutLwtMisc->add_widget(comboBoxDefaultLwt);
    groupBoxLwtMisc->set_layout(vboxLayoutLwtMisc);

    /* Widget Layout */
    QVBoxLayout *vboxLayoutMain = QVBoxLayout(widget);
    vboxLayoutMain->add_widget(groupBoxLwtMisc);
    vboxLayoutMain->addStretch(1);
    widget->set_layout(vboxLayoutMain);

    QScrollArea* scrollArea = QScrollArea(this);
    scrollArea->setWidgetResizable(true);
    scrollArea->setWidget(widget);
    return scrollArea;
}

/**
 * @brief Create a tab selection object
 *
 */
void
create_tab_selection(void)
{
    QWidget* widget = QWidget(this);
    /* Selection Modes */
    QGroupBox* groupBoxSelectionModes = QGroupBox(translate("Modes"), widget);

    QCheckBox* checkBoxSelectionModePickFirst = QCheckBox(translate("Allow Preselection (PickFirst)"), groupBoxSelectionModes);
    dialog.selection_mode_pickfirst = settings_selection_mode_pickfirst;
    checkBoxSelectionModePickFirst_set_checked(dialog.selection_mode_pickfirst);
    checkBoxSelectionModePickFirst_set_checked(true); checkBoxSelectionModePickFirst->setEnabled(false); //\todo Remove this line when Post-selection is available
    connect(checkBoxSelectionModePickFirst, stateChanged(int), checkBoxSelectionModePickFirstStateChanged(int)));

    QCheckBox* checkBoxSelectionModePickAdd = QCheckBox(translate("Add to Selection (PickAdd)"), groupBoxSelectionModes);
    dialog.selection_mode_pickadd = settings_selection_mode_pickadd;
    checkBoxSelectionModePickAdd_set_checked(dialog.selection_mode_pickadd);
    connect(checkBoxSelectionModePickAdd, stateChanged(int), checkBoxSelectionModePickAddStateChanged(int)));

    QCheckBox* checkBoxSelectionModePickDrag = QCheckBox(translate("Drag to Select (PickDrag)"), groupBoxSelectionModes);
    dialog.selection_mode_pickdrag = settings_selection_mode_pickdrag;
    checkBoxSelectionModePickDrag_set_checked(dialog.selection_mode_pickdrag);
    checkBoxSelectionModePickDrag_set_checked(false); checkBoxSelectionModePickDrag->setEnabled(false); //\todo Remove this line when this functionality is available
    connect(checkBoxSelectionModePickDrag, stateChanged(int), checkBoxSelectionModePickDragStateChanged(int)));

    QVBoxLayout* vboxLayoutSelectionModes = QVBoxLayout(groupBoxSelectionModes);
    vboxLayoutSelectionModes->add_widget(checkBoxSelectionModePickFirst);
    vboxLayoutSelectionModes->add_widget(checkBoxSelectionModePickAdd);
    vboxLayoutSelectionModes->add_widget(checkBoxSelectionModePickDrag);
    groupBoxSelectionModes->set_layout(vboxLayoutSelectionModes);

    /* Selection Colors */
    QGroupBox* groupBoxSelectionColors = QGroupBox(translate("Colors"), widget);

    QLabel* labelCoolGripColor = QLabel(translate("Cool Grip (Unselected)"), groupBoxSelectionColors);
    int* comboBoxCoolGripColor = int(groupBoxSelectionColors);
    addColorsToComboBox(comboBoxCoolGripColor);
    dialog.selection_coolgrip_color = settings_selection_coolgrip_color;
    comboBoxCoolGripColor->setCurrentIndex(comboBoxCoolGripColor->findData(dialog.selection_coolgrip_color));
    connect(comboBoxCoolGripColor, currentIndexChanged(int), comboBoxSelectionCoolGripColorCurrentIndexChanged(int)));

    QLabel* labelHotGripColor = QLabel(translate("Hot Grip (Selected)"), groupBoxSelectionColors);
    int* comboBoxHotGripColor = int(groupBoxSelectionColors);
    addColorsToComboBox(comboBoxHotGripColor);
    dialog.selection_hotgrip_color = settings_selection_hotgrip_color;
    comboBoxHotGripColor->setCurrentIndex(comboBoxHotGripColor->findData(dialog.selection_hotgrip_color));
    connect(comboBoxHotGripColor, currentIndexChanged(int), comboBoxSelectionHotGripColorCurrentIndexChanged(int)));

    QVBoxLayout* vboxLayoutSelectionColors = QVBoxLayout(groupBoxSelectionColors);
    vboxLayoutSelectionColors->add_widget(labelCoolGripColor);
    vboxLayoutSelectionColors->add_widget(comboBoxCoolGripColor);
    vboxLayoutSelectionColors->add_widget(labelHotGripColor);
    vboxLayoutSelectionColors->add_widget(comboBoxHotGripColor);
    groupBoxSelectionColors->set_layout(vboxLayoutSelectionColors);

    /* Selection Sizes */
    QGroupBox* groupBoxSelectionSizes = QGroupBox(translate("Sizes"), widget);

    QLabel* labelSelectionGripSize = QLabel(translate("Grip Size"), groupBoxSelectionSizes);
    QSlider* sliderSelectionGripSize = QSlider(Qt_Horizontal, groupBoxSelectionSizes);
    sliderSelectionGripSize->setRange(1,20);
    dialog.selection_grip_size = settings_selection_grip_size;
    sliderSelectionGripSize->setValue(dialog.selection_grip_size);
    connect(sliderSelectionGripSize, valueChanged(int), sliderSelectionGripSizeValueChanged(int)));

    QLabel* labelSelectionPickBoxSize = QLabel(translate("Pickbox Size"), groupBoxSelectionSizes);
    QSlider* sliderSelectionPickBoxSize = QSlider(Qt_Horizontal, groupBoxSelectionSizes);
    sliderSelectionPickBoxSize->setRange(1,20);
    dialog.selection_pickbox_size = settings_selection_pickbox_size;
    sliderSelectionPickBoxSize->setValue(dialog.selection_pickbox_size);
    connect(sliderSelectionPickBoxSize, valueChanged(int), sliderSelectionPickBoxSizeValueChanged(int)));

    QVBoxLayout* vboxLayoutSelectionSizes = QVBoxLayout(groupBoxSelectionSizes);
    vboxLayoutSelectionSizes->add_widget(labelSelectionGripSize);
    vboxLayoutSelectionSizes->add_widget(sliderSelectionGripSize);
    vboxLayoutSelectionSizes->add_widget(labelSelectionPickBoxSize);
    vboxLayoutSelectionSizes->add_widget(sliderSelectionPickBoxSize);
    groupBoxSelectionSizes->set_layout(vboxLayoutSelectionSizes);

    /* Widget Layout */
    QVBoxLayout *vboxLayoutMain = QVBoxLayout(widget);
    vboxLayoutMain->add_widget(groupBoxSelectionModes);
    vboxLayoutMain->add_widget(groupBoxSelectionColors);
    vboxLayoutMain->add_widget(groupBoxSelectionSizes);
    vboxLayoutMain->addStretch(1);
    widget->set_layout(vboxLayoutMain);

    QScrollArea* scrollArea = QScrollArea(this);
    scrollArea->setWidgetResizable(true);
    scrollArea->setWidget(widget);
    return scrollArea;
}

/**
 * @brief .
 *
 * \todo Add Other... so the user can select custom colors
 */
void
settings_dialog_addColorsToComboBox(int* comboBox)
{
    comboBox->addItem(load_icon("colorred"), translate("Red"), qRgb(255, 0, 0));
    comboBox->addItem(load_icon("coloryellow"), translate("Yellow"),  qRgb(255,255,  0));
    comboBox->addItem(load_icon("colorgreen"),   translate("Green"),   qRgb(  0,255,  0));
    comboBox->addItem(load_icon("colorcyan"),    translate("Cyan"),    qRgb(  0,255,255));
    comboBox->addItem(load_icon("colorblue"),    translate("Blue"),    qRgb(  0,  0,255));
    comboBox->addItem(load_icon("colormagenta"), translate("Magenta"), qRgb(255,  0,255));
    comboBox->addItem(load_icon("colorwhite"),   translate("White"),   qRgb(255,255,255));
}

void
settings_dialog_comboBoxLanguageCurrentIndexChanged(const char * lang)
{
    dialog.general_language = lang.toLower();
}

void
settings_dialog_comboBoxIconThemeCurrentIndexChanged(const char *theme)
{
    dialog.general_icon_theme.string_value = theme.toLocal8Bit();
}

void
settings_dialog_comboBoxIconSizeCurrentIndexChanged(int index)
{
    int* comboBox = <int*>(sender());
    if (comboBox) {
        bool ok = 0;
        dialog.general_icon_size = comboBox->itemData(index).toUInt(&ok);
        if (!ok) {
            dialog.general_icon_size = 16;
        }
    }
    else {
        dialog.general_icon_size = 16;
    }
}

void
settings_dialog_checkBoxGeneralMdiBGUseLogoStateChanged(int checked)
{
    preview.general_mdi_bg_use_logo = checked;
    active_view->useBackgroundLogo(checked);
}

void
settings_dialog_chooseGeneralMdiBackgroundLogo(void)
{
    QPushButton* button = <QPushButton*>(sender());
    if (button) {
        const char *selectedImage;
        selectedImage = QFileDialog_getOpenFileName(translate("Open File"),
                        QStandardPaths_writableLocation(QStandardPaths_PicturesLocation),
                        translate("Images (*.bmp *.png *.jpg)"));

        if (!selectedImage.isNull()) {
            accept_general_mdi_bg_logo = selectedImage;
        }

        //Update immediately so it can be previewed
        active_view->setBackgroundLogo(accept_general_mdi_bg_logo);
    }
}

void
settings_dialog_checkBoxGeneralMdiBGUseTextureStateChanged(int checked)
{
    preview.general_mdi_bg_use_texture = checked;
    active_view->useBackgroundTexture(checked);
}

void
settings_dialog_chooseGeneralMdiBackgroundTexture(void)
{
    QPushButton* button = <QPushButton*>(sender());
    if (button) {
        const char *selectedImage;
        selectedImage = QFileDialog_getOpenFileName(translate("Open File"),
                        QStandardPaths_writableLocation(QStandardPaths_PicturesLocation),
                        translate("Images (*.bmp *.png *.jpg)"));

        if (!selectedImage.isNull()) {
            accept_general_mdi_bg_texture = selectedImage;
        }

        //Update immediately so it can be previewed
        active_view->setBackgroundTexture(accept_general_mdi_bg_texture);
    }
}

/**
 * @brief \a checked.
 */
void
settings_dialog_checkBoxGeneralMdiBGUseColorStateChanged(int checked)
{
    preview.general_mdi_bg_use_color = checked;
    active_view->useBackgroundColor(checked);
}

void
settings_dialog_chooseGeneralMdiBackgroundColor(void)
{
    QPushButton* button = <QPushButton*>(sender());
    if (button) {
        EmbColorDialog* colorDialog = EmbColorDialog(EmbColor(accept_general_mdi_bg_color), this);
        connect(colorDialog, currentColorChanged(EmbColor), currentGeneralMdiBackgroundColorChanged(EmbColor)));
        colorDialog->exec();

        if (colorDialog->result() == QDialog_Accepted) {
            accept_general_mdi_bg_color = colorDialog->selectedColor().rgb();
            QPixmap pix(16,16);
            pix.fill(EmbColor(accept_general_mdi_bg_color));
            button->setIcon(QIcon(pix));
            active_view->setBackgroundColor(EmbColor(accept_general_mdi_bg_color));
        }
        else {
            active_view->setBackgroundColor(EmbColor(dialog.general_mdi_bg_color));
        }
    }
}

/**
 * @brief \a color.
 */
void
settings_dialog_currentGeneralMdiBackgroundColorChanged(EmbColor color)
{
    preview.general_mdi_bg_color = color.rgb();
    active_view->setBackgroundColor(EmbColor(preview.general_mdi_bg_color));
}

/**
 * @brief \a checked.
 */
void
settings_dialog_checkBoxShowscrollbarsStateChanged(int checked)
{
    preview.display_show_scrollbars = checked;
    updateAllEmbViewscrollbars(preview.display_show_scrollbars);
}

/**
 * @brief  .
 */
void
settings_dialog_chooseDisplayCrossHairColor(void)
{
    QPushButton* button = <QPushButton*>(sender());
    if (button) {
        EmbColorDialog* colorDialog = EmbColorDialog(EmbColor(accept_display_crosshair_color), this);
        connect(colorDialog, currentColorChanged(EmbColor), currentDisplayCrossHairColorChanged(EmbColor)));
        colorDialog->exec();

        if (colorDialog->result() == QDialog_Accepted) {
            accept_display_crosshair_color = colorDialog->selectedColor().rgb();
            QPixmap pix(16,16);
            pix.fill(EmbColor(accept_display_crosshair_color));
            button->setIcon(QIcon(pix));
            updateAllEmbViewCrossHairColors(accept_display_crosshair_color);
        }
        else {
            updateAllEmbViewCrossHairColors(dialog.display_crosshair_color);
        }
    }
}

/**
 * @brief \a color.
 */
void
settings_dialog_currentDisplayCrossHairColorChanged(EmbColor color)
{
    preview.display_crosshair_color = color.rgb();
    updateAllEmbViewCrossHairColors(preview.display_crosshair_color);
}

void
settings_dialog_chooseDisplayBackgroundColor(void)
{
    QPushButton* button = <QPushButton*>(sender());
    if (button) {
        EmbColorDialog* colorDialog = EmbColorDialog(EmbColor(accept_display_bg_color), this);
        connect(colorDialog, currentColorChanged(EmbColor), currentDisplayBackgroundColorChanged(EmbColor)));
        colorDialog->exec();

        if (colorDialog->result() == QDialog_Accepted) {
            accept_display_bg_color = colorDialog->selectedColor().rgb();
            QPixmap pix(16,16);
            pix.fill(EmbColor(accept_display_bg_color));
            button->setIcon(QIcon(pix));
            updateAllEmbViewBackgroundColors(accept_display_bg_color);
        }
        else {
            updateAllEmbViewBackgroundColors(dialog.display_bg_color);
        }
    }
}

/**
 * @brief \a color .
 */
void
settings_dialog_currentDisplayBackgroundColorChanged(EmbColor color)
{
    preview.display_bg_color = color.rgb();
    updateAllEmbViewBackgroundColors(preview.display_bg_color);
}

void
settings_dialog_chooseDisplaySelectBoxLeftColor(void)
{
    QPushButton* button = <QPushButton*>(sender());
    if (button) {
        EmbColorDialog* colorDialog = EmbColorDialog(EmbColor(accept_display_selectbox_left_color), this);
        connect(colorDialog, currentColorChanged(EmbColor), currentDisplaySelectBoxLeftColorChanged(EmbColor)));
        colorDialog->exec();

        if (colorDialog->result() == QDialog_Accepted) {
            accept_display_selectbox_left_color = colorDialog->selectedColor().rgb();
            QPixmap pix(16,16);
            pix.fill(EmbColor(accept_display_selectbox_left_color));
            button->setIcon(QIcon(pix));
            updateAllEmbViewSelectBoxColors(
                accept_display_selectbox_left_color,
                accept_display_selectbox_left_fill,
                accept_display_selectbox_right_color,
                accept_display_selectbox_right_fill,
                preview.display_selectbox_alpha);
        }
        else {
            updateAllEmbViewSelectBoxColors(
                dialog.display_selectbox_left_color,
                dialog.display_selectbox_left_fill,
                dialog.display_selectbox_right_color,
                dialog.display_selectbox_right_fill,
                preview.display_selectbox_alpha);
        }
    }
}

/**
 * @brief \a color .
 */
void
settings_dialog_currentDisplaySelectBoxLeftColorChanged(EmbColor color)
{
    preview.display_selectbox_left_color = color.rgb();
    updateAllEmbViewSelectBoxColors(
        preview.display_selectbox_left_color,
        preview.display_selectbox_left_fill,
        preview.display_selectbox_right_color,
        preview.display_selectbox_right_fill,
        preview.display_selectbox_alpha);
}

void
settings_dialog_chooseDisplaySelectBoxLeftFill(void)
{
    QPushButton* button = <QPushButton*>(sender());
    if (button) {
        EmbColorDialog* colorDialog = EmbColorDialog(EmbColor(accept_display_selectbox_left_fill), this);
        connect(colorDialog, currentColorChanged(EmbColor), currentDisplaySelectBoxLeftFillChanged(EmbColor)));
        colorDialog->exec();

        if (colorDialog->result() == QDialog_Accepted) {
            accept_display_selectbox_left_fill = colorDialog->selectedColor().rgb();
            QPixmap pix(16,16);
            pix.fill(EmbColor(accept_display_selectbox_left_fill));
            button->setIcon(QIcon(pix));
            updateAllEmbViewSelectBoxColors(
                accept_display_selectbox_left_color,
                accept_display_selectbox_left_fill,
                accept_display_selectbox_right_color,
                accept_display_selectbox_right_fill,
                preview.display_selectbox_alpha);
        }
        else {
            updateAllEmbViewSelectBoxColors(
                dialog.display_selectbox_left_color,
                dialog.display_selectbox_left_fill,
                dialog.display_selectbox_right_color,
                dialog.display_selectbox_right_fill,
                preview.display_selectbox_alpha);
        }
    }
}

/**
 * @brief color .
 */
void
settings_dialog_currentDisplaySelectBoxLeftFillChanged(EmbColor color)
{
    preview.display_selectbox_left_fill = color.rgb();
    updateAllEmbViewSelectBoxColors(
        preview.display_selectbox_left_color,
        preview.display_selectbox_left_fill,
        preview.display_selectbox_right_color,
        preview.display_selectbox_right_fill,
        preview.display_selectbox_alpha);
}

void
settings_dialog_chooseDisplaySelectBoxRightColor(void)
{
    QPushButton* button = <QPushButton*>(sender());
    if (button)
    {
        EmbColorDialog* colorDialog = EmbColorDialog(EmbColor(accept_display_selectbox_right_color), this);
        connect(colorDialog, currentColorChanged(EmbColor), currentDisplaySelectBoxRightColorChanged(EmbColor)));
        colorDialog->exec();

        if (colorDialog->result() == QDialog_Accepted) {
            accept_display_selectbox_right_color = colorDialog->selectedColor().rgb();
            QPixmap pix(16,16);
            pix.fill(EmbColor(accept_display_selectbox_right_color));
            button->setIcon(QIcon(pix));
            updateAllEmbViewSelectBoxColors(accept_display_selectbox_left_color,
                                                  accept_display_selectbox_left_fill,
                                                  accept_display_selectbox_right_color,
                                                  accept_display_selectbox_right_fill,
                                                  preview.display_selectbox_alpha);
        }
        else {
            updateAllEmbViewSelectBoxColors(dialog.display_selectbox_left_color,
                                                  dialog.display_selectbox_left_fill,
                                                  dialog.display_selectbox_right_color,
                                                  dialog.display_selectbox_right_fill,
                                                  preview.display_selectbox_alpha);
        }
    }
}

/**
 * @brief \a color .
 */
void
settings_dialog_currentDisplaySelectBoxRightColorChanged(EmbColor color)
{
    preview.display_selectbox_right_color = embColor_to_int(color, 255);
    updateAllEmbViewSelectBoxColors(preview.display_selectbox_left_color,
                                          preview.display_selectbox_left_fill,
                                          preview.display_selectbox_right_color,
                                          preview.display_selectbox_right_fill,
                                          preview.display_selectbox_alpha);
}

void
settings_dialog_chooseDisplaySelectBoxRightFill(void)
{
    QPushButton* button = <QPushButton*>(sender());
    if (button) {
        EmbColorDialog* colorDialog = EmbColorDialog(EmbColor(accept_display_selectbox_right_fill), this);
        connect(colorDialog, currentColorChanged(EmbColor), currentDisplaySelectBoxRightFillChanged(EmbColor)));
        colorDialog->exec();

        if (colorDialog->result() == QDialog_Accepted) {
            accept_display_selectbox_right_fill = colorDialog->selectedColor().rgb();
            QPixmap pix(16,16);
            pix.fill(EmbColor(accept_display_selectbox_right_fill));
            button->setIcon(QIcon(pix));
            updateAllEmbViewSelectBoxColors(accept_display_selectbox_left_color,
                accept_display_selectbox_left_fill,
                accept_display_selectbox_right_color,
                accept_display_selectbox_right_fill,
                preview.display_selectbox_alpha);
        }
        else {
            updateAllEmbViewSelectBoxColors(dialog.display_selectbox_left_color,
                dialog.display_selectbox_left_fill,
                dialog.display_selectbox_right_color,
                dialog.display_selectbox_right_fill,
                preview.display_selectbox_alpha);
        }
    }
}

/**
 * @brief \a color .
 */
void
settings_dialog_currentDisplaySelectBoxRightFillChanged(EmbColor color)
{
    preview.display_selectbox_right_fill = embColor_to_int(color, 255);
    updateAllEmbViewSelectBoxColors(
        preview.display_selectbox_left_color,
        preview.display_selectbox_left_fill,
        preview.display_selectbox_right_color,
        preview.display_selectbox_right_fill,
        preview.display_selectbox_alpha);
}

/**
 * @brief \a value.
 */
void
settings_dialog_spinBoxDisplaySelectBoxAlphaValueChanged(int value)
{
    preview.display_selectbox_alpha = value;
    updateAllEmbViewSelectBoxColors(
        accept_display_selectbox_left_color,
        accept_display_selectbox_left_fill,
        accept_display_selectbox_right_color,
        accept_display_selectbox_right_fill,
        preview.display_selectbox_alpha);
}

void
settings_dialog_choosePromptTextColor(void)
{
    QPushButton* button = <QPushButton*>(sender());
    if (button) {
        EmbColorDialog* colorDialog = EmbColorDialog(EmbColor(accept_prompt_text_color), this);
        connect(colorDialog, currentColorChanged(EmbColor), currentPromptTextColorChanged(EmbColor)));
        colorDialog->exec();

        if (colorDialog->result() == QDialog_Accepted) {
            accept_prompt_text_color = colorDialog->selectedColor().rgb();
            QPixmap pix(16,16);
            pix.fill(EmbColor(accept_prompt_text_color));
            button->setIcon(QIcon(pix));
            prompt->setPromptTextColor(EmbColor(accept_prompt_text_color));
        }
        else {
            prompt->setPromptTextColor(EmbColor(dialog.prompt_text_color));
        }
    }
}

/**
 * @brief \a color .
 */
void
settings_dialog_currentPromptTextColorChanged(EmbColor color)
{
    preview.prompt_text_color = color.rgb();
    prompt->setPromptTextColor(EmbColor(preview.prompt_text_color));
}

void
settings_dialog_choosePromptBackgroundColor(void)
{
    QPushButton* button = <QPushButton*>(sender());
    if (button) {
        EmbColorDialog* colorDialog = EmbColorDialog(EmbColor(accept_prompt_bg_color), this);
        connect(colorDialog, currentColorChanged(EmbColor), currentPromptBackgroundColorChanged(EmbColor)));
        colorDialog->exec();

        if (colorDialog->result() == QDialog_Accepted) {
            accept_prompt_bg_color = colorDialog->selectedColor().rgb();
            QPixmap pix(16,16);
            pix.fill(EmbColor(accept_prompt_bg_color));
            button->setIcon(QIcon(pix));
            prompt->setPromptBackgroundColor(EmbColor(accept_prompt_bg_color));
        }
        else {
            prompt->setPromptBackgroundColor(EmbColor(dialog.prompt_bg_color));
        }
    }
}

void
settings_dialog_currentPromptBackgroundColorChanged(EmbColor color)
{
    preview.prompt_bg_color = color.rgb();
    prompt->setPromptBackgroundColor(EmbColor(preview.prompt_bg_color));
}

void
settings_dialog_comboBoxPromptFontFamilyCurrentIndexChanged(const char *family)
{
    preview.prompt_font_family = family;
    prompt->setPromptFontFamily(preview.prompt_font_family);
}

void
settings_dialog_comboBoxPromptFontStyleCurrentIndexChanged(const char *style)
{
    preview.prompt_font_style = style;
    prompt->setPromptFontStyle(preview.prompt_font_style);
}

void
settings_dialog_spinBoxPromptFontSizeValueChanged(int value)
{
    preview.prompt_font_size = value;
    prompt->setPromptFontSize(preview.prompt_font_size);
}

void
settings_dialog_checkBoxCustomFilterStateChanged(int checked)
{
    QCheckBox* checkBox = <QCheckBox*>(sender());
    if (checkBox) {
        const char *format = checkBox->text();
        log_debug("CustomFilter: %s %d", format), checked);
        if (checked)
            dialog.opensave_custom_filter.append(" *." + format.toLower());
        else
            dialog.opensave_custom_filter.remove("*." + format, Qt_CaseInsensitive);
        //dialog.opensave_custom_filter = checked; //TODO
    }
}

void
settings_dialog_buttonCustomFilterSelectAllClicked(void)
{
    emit buttonCustomFilterSelectAll(true);
    dialog.opensave_custom_filter = "supported";
}

void
settings_dialog_buttonCustomFilterClearAllClicked(void)
{
    emit buttonCustomFilterClearAll(false);
    dialog.opensave_custom_filter.clear();
}

void
settings_dialog_checkBoxGridColorMatchCrossHairStateChanged(int checked)
{
    dialog.grid_color_match_crosshair = checked;
    if (dialog.grid_color_match_crosshair) {
        updateAllEmbViewGridColors(accept_display_crosshair_color);
    }
    else {
        updateAllEmbViewGridColors(accept_grid_color);
    }

    QObject* senderObj = sender();
    if (senderObj) {
        QObject* parent = senderObj->parent();
        if (parent) {
            QLabel* labelGridColor = parent->findChild<QLabel*>("labelGridColor");
            if (labelGridColor) labelGridColor->setEnabled(!dialog.grid_color_match_crosshair);
            QPushButton* buttonGridColor = parent->findChild<QPushButton*>("buttonGridColor");
            if (buttonGridColor) {
                buttonGridColor->setEnabled(!dialog.grid_color_match_crosshair);
            }
        }
    }
}

void
settings_dialog_chooseGridColor(void)
{
    QPushButton* button = <QPushButton*>(sender());
    if (button) {
        EmbColorDialog* colorDialog = EmbColorDialog(EmbColor(accept_grid_color), this);
        connect(colorDialog, currentColorChanged(EmbColor), currentGridColorChanged(EmbColor)));
        colorDialog->exec();

        if (colorDialog->result() == QDialog_Accepted) {
            accept_grid_color = colorDialog->selectedColor().rgb();
            QPixmap pix(16,16);
            pix.fill(EmbColor(accept_grid_color));
            button->setIcon(QIcon(pix));
            updateAllEmbViewGridColors(accept_grid_color);
        }
        else {
            updateAllEmbViewGridColors(dialog.grid_color);
        }
    }
}

void
settings_dialog_currentGridColorChanged(EmbColor color)
{
    preview.grid_color = color.rgb();
    updateAllEmbViewGridColors(preview.grid_color);
}

void
settings_dialog_checkBoxGridLoadFromFileStateChanged(int checked)
{
    dialog.grid_load_from_file = checked;

    QObject* senderObj = sender();
    if (!senderObj) {
        return;
    }
    QObject* parent = senderObj->parent();
    if (!parent) {
        return;
    }
    QLabel* labelGridType = parent->findChild<QLabel*>("labelGridType");
    if (labelGridType) {
        labelGridType->setEnabled(!dialog.grid_load_from_file);
    }
    int* comboBoxGridType = parent->findChild<int*>("comboBoxGridType");
    if (comboBoxGridType) {
        comboBoxGridType->setEnabled(!dialog.grid_load_from_file);
    }
    QCheckBox* checkBoxGridCenterOnOrigin = parent->findChild<QCheckBox*>("checkBoxGridCenterOnOrigin");
    if (checkBoxGridCenterOnOrigin)
        checkBoxGridCenterOnOrigin->setEnabled(!dialog.grid_load_from_file);
    QLabel* labelGridCenterX = parent->findChild<QLabel*>("labelGridCenterX");
    if (labelGridCenterX)
        labelGridCenterX->setEnabled(!dialog.grid_load_from_file && !dialog.grid_center_on_origin);
    QDoubleSpinBox* spinBoxGridCenterX = parent->findChild<QDoubleSpinBox*>("spinBoxGridCenterX");
    if (spinBoxGridCenterX)
        spinBoxGridCenterX->setEnabled(!dialog.grid_load_from_file && !dialog.grid_center_on_origin);
    QLabel* labelGridCenterY = parent->findChild<QLabel*>("labelGridCenterY");
    if (labelGridCenterY)
        labelGridCenterY->setEnabled(!dialog.grid_load_from_file && !dialog.grid_center_on_origin);
    QDoubleSpinBox* spinBoxGridCenterY = parent->findChild<QDoubleSpinBox*>("spinBoxGridCenterY");
    if (spinBoxGridCenterY)
        spinBoxGridCenterY->setEnabled(!dialog.grid_load_from_file && !dialog.grid_center_on_origin);
    QLabel* labelGridSizeX = parent->findChild<QLabel*>("labelGridSizeX");
    if (labelGridSizeX)
        labelGridSizeX->setEnabled(!dialog.grid_load_from_file);
    QDoubleSpinBox* spinBoxGridSizeX = parent->findChild<QDoubleSpinBox*>("spinBoxGridSizeX");
    if (spinBoxGridSizeX)
        spinBoxGridSizeX->setEnabled(!dialog.grid_load_from_file);
    QLabel* labelGridSizeY = parent->findChild<QLabel*>("labelGridSizeY");
    if (labelGridSizeY)
        labelGridSizeY->setEnabled(!dialog.grid_load_from_file);
    QDoubleSpinBox* spinBoxGridSizeY = parent->findChild<QDoubleSpinBox*>("spinBoxGridSizeY");
    if (spinBoxGridSizeY)
        spinBoxGridSizeY->setEnabled(!dialog.grid_load_from_file);
    QLabel* labelGridSpacingX = parent->findChild<QLabel*>("labelGridSpacingX");
    if (labelGridSpacingX)
        labelGridSpacingX->setEnabled(!dialog.grid_load_from_file);
    QDoubleSpinBox* spinBoxGridSpacingX = parent->findChild<QDoubleSpinBox*>("spinBoxGridSpacingX");
    if (spinBoxGridSpacingX)
        spinBoxGridSpacingX->setEnabled(!dialog.grid_load_from_file);
    QLabel* labelGridSpacingY = parent->findChild<QLabel*>("labelGridSpacingY");
    if (labelGridSpacingY)
        labelGridSpacingY->setEnabled(!dialog.grid_load_from_file);
    QDoubleSpinBox* spinBoxGridSpacingY = parent->findChild<QDoubleSpinBox*>("spinBoxGridSpacingY");
    if (spinBoxGridSpacingY)
        spinBoxGridSpacingY->setEnabled(!dialog.grid_load_from_file);
    QLabel* labelGridSizeRadius = parent->findChild<QLabel*>("labelGridSizeRadius");
    if (labelGridSizeRadius)
        labelGridSizeRadius->setEnabled(!dialog.grid_load_from_file);
    QDoubleSpinBox* spinBoxGridSizeRadius = parent->findChild<QDoubleSpinBox*>("spinBoxGridSizeRadius");
    if (spinBoxGridSizeRadius) {
        spinBoxGridSizeRadius->setEnabled(!dialog.grid_load_from_file);
    }
    QLabel* labelGridSpacingRadius = parent->findChild<QLabel*>("labelGridSpacingRadius");
    if (labelGridSpacingRadius) {
        labelGridSpacingRadius->setEnabled(!dialog.grid_load_from_file);
    }
    QDoubleSpinBox* spinBoxGridSpacingRadius = parent->findChild<QDoubleSpinBox*>("spinBoxGridSpacingRadius");
    if (spinBoxGridSpacingRadius) {
        spinBoxGridSpacingRadius->setEnabled(!dialog.grid_load_from_file);
    }
    QLabel* labelGridSpacingAngle = parent->findChild<QLabel*>("labelGridSpacingAngle");
    if (labelGridSpacingAngle) {
        labelGridSpacingAngle->setEnabled(!dialog.grid_load_from_file);
    }
    QDoubleSpinBox* spinBoxGridSpacingAngle = parent->findChild<QDoubleSpinBox*>("spinBoxGridSpacingAngle");
    if (spinBoxGridSpacingAngle) {
        spinBoxGridSpacingAngle->setEnabled(!dialog.grid_load_from_file);
    }
}

void
settings_dialog_comboBoxGridTypeCurrentIndexChanged(const char *type)
{
    dialog.grid_type = type;

    QObject* senderObj = sender();
    if (!senderObj) {
        return;
    }
    QObject* parent = senderObj->parent();
    if (!parent) {
        return;
    }
    bool visibility = false;
    if (type == "Circular") {
        visibility = true;
    }

    QLabel* labelGridSizeX = parent->findChild<QLabel*>("labelGridSizeX");
    if (labelGridSizeX) {
        labelGridSizeX->setVisible(!visibility);
    }
    QDoubleSpinBox* spinBoxGridSizeX = parent->findChild<QDoubleSpinBox*>("spinBoxGridSizeX");
    if (spinBoxGridSizeX) {
        spinBoxGridSizeX->setVisible(!visibility);
    }
    QLabel* labelGridSizeY = parent->findChild<QLabel*>("labelGridSizeY");
    if (labelGridSizeY)
        labelGridSizeY->setVisible(!visibility);
    QDoubleSpinBox* spinBoxGridSizeY = parent->findChild<QDoubleSpinBox*>("spinBoxGridSizeY");
    if (spinBoxGridSizeY)
        spinBoxGridSizeY->setVisible(!visibility);
    QLabel* labelGridSpacingX = parent->findChild<QLabel*>("labelGridSpacingX");
    if (labelGridSpacingX)
        labelGridSpacingX->setVisible(!visibility);
    QDoubleSpinBox* spinBoxGridSpacingX = parent->findChild<QDoubleSpinBox*>("spinBoxGridSpacingX");
    if (spinBoxGridSpacingX) spinBoxGridSpacingX->setVisible(!visibility);
    QLabel* labelGridSpacingY = parent->findChild<QLabel*>("labelGridSpacingY");
    if (labelGridSpacingY) labelGridSpacingY->setVisible(!visibility);
    QDoubleSpinBox* spinBoxGridSpacingY = parent->findChild<QDoubleSpinBox*>("spinBoxGridSpacingY");
    if (spinBoxGridSpacingY) spinBoxGridSpacingY->setVisible(!visibility);
    QLabel* labelGridSizeRadius = parent->findChild<QLabel*>("labelGridSizeRadius");
    if (labelGridSizeRadius) labelGridSizeRadius->setVisible(visibility);
    QDoubleSpinBox* spinBoxGridSizeRadius = parent->findChild<QDoubleSpinBox*>("spinBoxGridSizeRadius");
    if (spinBoxGridSizeRadius) spinBoxGridSizeRadius->setVisible(visibility);
    QLabel* labelGridSpacingRadius = parent->findChild<QLabel*>("labelGridSpacingRadius");
    if (labelGridSpacingRadius) labelGridSpacingRadius->setVisible(visibility);
    QDoubleSpinBox* spinBoxGridSpacingRadius = parent->findChild<QDoubleSpinBox*>("spinBoxGridSpacingRadius");
    if (spinBoxGridSpacingRadius) spinBoxGridSpacingRadius->setVisible(visibility);
    QLabel* labelGridSpacingAngle = parent->findChild<QLabel*>("labelGridSpacingAngle");
    if (labelGridSpacingAngle) labelGridSpacingAngle->setVisible(visibility);
    QDoubleSpinBox* spinBoxGridSpacingAngle = parent->findChild<QDoubleSpinBox*>("spinBoxGridSpacingAngle");
    if (spinBoxGridSpacingAngle) spinBoxGridSpacingAngle->setVisible(visibility);
}

void
settings_dialog_checkBoxGridCenterOnOriginStateChanged(int checked)
{
    dialog.grid_center_on_origin = checked;

    QObject* senderObj = sender();
    if (senderObj) {
        QObject* parent = senderObj->parent();
        if (parent) {
            QLabel* labelGridCenterX = parent->findChild<QLabel*>("labelGridCenterX");
            if (labelGridCenterX) labelGridCenterX->setEnabled(!dialog.grid_center_on_origin);
            QDoubleSpinBox* spinBoxGridCenterX = parent->findChild<QDoubleSpinBox*>("spinBoxGridCenterX");
            if (spinBoxGridCenterX) spinBoxGridCenterX->setEnabled(!dialog.grid_center_on_origin);
            QLabel* labelGridCenterY = parent->findChild<QLabel*>("labelGridCenterY");
            if (labelGridCenterY) labelGridCenterY->setEnabled(!dialog.grid_center_on_origin);
            QDoubleSpinBox* spinBoxGridCenterY = parent->findChild<QDoubleSpinBox*>("spinBoxGridCenterY");
            if (spinBoxGridCenterY) spinBoxGridCenterY->setEnabled(!dialog.grid_center_on_origin);
        }
    }
}

void
settings_dialog_comboBoxRulerMetricCurrentIndexChanged(int index)
{
    int* comboBox = <int*>(sender());
    if (comboBox) {
        dialog.ruler_metric = comboBox->itemData(index).toBool();
    }
    else {
        dialog.ruler_metric = true;
    }
}

void
settings_dialog_chooseRulerColor(void)
{
    QPushButton* button = <QPushButton*>(sender());
    if (button) {
        EmbColorDialog* colorDialog = EmbColorDialog(EmbColor(accept_ruler_color), this);
        connect(colorDialog, currentColorChanged(EmbColor), currentRulerColorChanged(EmbColor)));
        colorDialog->exec();

        if (colorDialog->result() == QDialog_Accepted) {
            accept_ruler_color = colorDialog->selectedColor().rgb();
            QPixmap pix(16,16);
            pix.fill(EmbColor(accept_ruler_color));
            button->setIcon(QIcon(pix));
            updateAllEmbViewRulerColors(accept_ruler_color);
        }
        else {
            updateAllEmbViewRulerColors(dialog.ruler_color);
        }
    }
}

void
settings_dialog_currentRulerColorChanged(EmbColor color)
{
    preview.ruler_color = color.rgb();
    updateAllEmbViewRulerColors(preview.ruler_color);
}

void
settings_dialog_buttonQSnapSelectAllClicked(void)
{
    emit buttonQSnapSelectAll(true);
}

void
settings_dialog_buttonQSnapClearAllClicked(void)
{
    emit buttonQSnapClearAll(false);
}

/**
 * @brief \a index .
 *
 * \todo Figure out how to abstract the slot in a way that it can be used for comboBoxes in general
 *       Currently comboBoxQSnapLocatorColorCurrentIndexChanged(int index)
 *                 comboBoxSelectionCoolGripColorCurrentIndexChanged(int index)
 *                 comboBoxSelectionHotGripColorCurrentIndexChanged(int index)
 *       are all similar except the dialog. variable being worked on and the QVariant.
 */
void
settings_dialog_comboBoxQSnapLocatorColorCurrentIndexChanged(int index)
{
    //\todo Alert user if color matched the display bg color
    int* comboBox = <int*>(sender());
    unsigned int defaultColor = qRgb(255,255,0); //Yellow
    if (comboBox) {
        bool ok = 0;
        dialog.qsnap_locator_color = comboBox->itemData(index).toUInt(&ok);
        if (!ok) {
            dialog.qsnap_locator_color = defaultColor;
        }
    }
    else {
        dialog.qsnap_locator_color = defaultColor;
    }
}

/**
 * @brief \a checked.
 */
void
settings_dialog_checkBoxLwtShowLwtStateChanged(int checked)
{
    preview.lwt_show_lwt = checked;
    if (preview.lwt_show_lwt) {
        statusbarLwtButton->enableLwt();
    }
    else {
        statusbarLwtButton->disableLwt();
    }

    QObject* senderObj = sender();
    if (senderObj)
    {
        QObject* parent = senderObj->parent();
        if (parent)
        {
            QCheckBox* checkBoxRealRender = parent->findChild<QCheckBox*>("checkBoxRealRender");
            if (checkBoxRealRender) checkBoxRealRender->setEnabled(preview.lwt_show_lwt);
        }
    }
}

/**
 * @brief \a checked .
 */
void
settings_dialog_checkBoxLwtRealRenderStateChanged(int checked)
{
    preview.lwt_real_render = checked;
    if (preview.lwt_real_render) {
        statusbarLwtButton->enableReal();
    }
    else {
        statusbarLwtButton->disableReal();
    }
}

/**
 * @brief \a index .
 *
 * \todo Alert user if color matched the display bg color
 */
void
settings_dialog_combobox_selectionCoolGripColorCurrentIndexChanged(int index)
{
    int* comboBox = <int*>(sender());
    unsigned int defaultColor = qRgb(0,0,255); //Blue
    if (comboBox) {
        bool ok = 0;
        dialog.selection_coolgrip_color = comboBox->itemData(index).toUInt(&ok);
        if (!ok)
            dialog.selection_coolgrip_color = defaultColor;
    }
    else {
        dialog.selection_coolgrip_color = defaultColor;
    }
}

/**
 * @brief \a index .
 *
 * \todo Alert user if color matched the display bg color
 */
void
settings_dialog_combobox_selectionHotGripColorCurrentIndexChanged(int index)
{
    int* comboBox = <int*>(sender());
    unsigned int defaultColor = qRgb(255,0,0); //Red
    if (comboBox)
    {
        bool ok = 0;
        dialog.selection_hotgrip_color = comboBox->itemData(index).toUInt(&ok);
        if (!ok)
            dialog.selection_hotgrip_color = defaultColor;
    }
    else
        dialog.selection_hotgrip_color = defaultColor;
}

void
settings_dialog_acceptChanges(void)
{
    dialog.general_mdi_bg_use_logo = preview.general_mdi_bg_use_logo;
    dialog.general_mdi_bg_use_texture = preview.general_mdi_bg_use_texture;
    dialog.general_mdi_bg_use_color = preview.general_mdi_bg_use_color;
    dialog.general_mdi_bg_logo = accept_general_mdi_bg_logo;
    dialog.general_mdi_bg_texture = accept_general_mdi_bg_texture;
    dialog.general_mdi_bg_color = accept_general_mdi_bg_color;
    dialog.display_show_scrollbars = preview.display_show_scrollbars;
    dialog.display_crosshair_color = accept_display_crosshair_color;
    dialog.display_bg_color = accept_display_bg_color;
    dialog.display_selectbox_left_color = accept_display_selectbox_left_color;
    dialog.display_selectbox_left_fill = accept_display_selectbox_left_fill;
    dialog.display_selectbox_right_color = accept_display_selectbox_right_color;
    dialog.display_selectbox_right_fill = accept_display_selectbox_right_fill;
    dialog.display_selectbox_alpha = preview.display_selectbox_alpha;
    dialog.prompt_text_color = accept_prompt_text_color;
    dialog.prompt_bg_color = accept_prompt_bg_color;
    dialog.prompt_font_family = preview.prompt_font_family;
    dialog.prompt_font_style = preview.prompt_font_style;
    dialog.prompt_font_size = preview.prompt_font_size;
    if (dialog.grid_color_match_crosshair) {
        dialog.grid_color = accept_display_crosshair_color;
    }
    else {
        dialog.grid_color = accept_grid_color;
    }
    dialog.ruler_color = accept_ruler_color;
    dialog.lwt_show_lwt = preview.lwt_show_lwt;
    dialog.lwt_real_render = preview.lwt_real_render;

    settings = dialog;

    //Make sure the user sees the changes applied immediately
    active_view->useBackgroundLogo(dialog.general_mdi_bg_use_logo);
    active_view->useBackgroundTexture(dialog.general_mdi_bg_use_texture);
    active_view->useBackgroundColor(dialog.general_mdi_bg_use_color);
    active_view->setBackgroundLogo(dialog.general_mdi_bg_logo);
    active_view->setBackgroundTexture(dialog.general_mdi_bg_texture);
    active_view->setBackgroundColor(dialog.general_mdi_bg_color);
    iconResize(dialog.general_icon_size);
    updateAllEmbViewscrollbars(dialog.display_show_scrollbars);
    updateAllEmbViewCrossHairColors(dialog.display_crosshair_color);
    updateAllEmbViewBackgroundColors(dialog.display_bg_color);
    updateAllEmbViewSelectBoxColors(dialog.display_selectbox_left_color,
                                          dialog.display_selectbox_left_fill,
                                          dialog.display_selectbox_right_color,
                                          dialog.display_selectbox_right_fill,
                                          dialog.display_selectbox_alpha);
    prompt->setPromptTextColor(EmbColor(dialog.prompt_text_color));
    prompt->setPromptBackgroundColor(EmbColor(dialog.prompt_bg_color));
    prompt->setPromptFontFamily(dialog.prompt_font_family);
    prompt->setPromptFontStyle(dialog.prompt_font_style);
    prompt->setPromptFontSize(dialog.prompt_font_size);
    updateAllEmbViewGridColors(dialog.grid_color);
    updateAllEmbViewRulerColors(dialog.ruler_color);
    if (dialog.lwt_show_lwt) {
        statusbarLwtButton->enableLwt();
    }
    else {
        statusbarLwtButton->disableLwt();
    }
    if (dialog.lwt_real_render) {
        statusbarLwtButton->enableReal();
    }
    else {
        statusbarLwtButton->disableReal();
    }
    updatePickAddMode(dialog.selection_mode_pickadd);

    writeSettings();
    accept();
}

/**
 * @brief \todo document this.
 *
 * \todo inform the user if they have changed settings
 */
void
settings_dialog_rejectChanges(void)
{
    /**
    //Update the view since the user must accept the preview
    active_view->useBackgroundLogo(dialog.general_mdi_bg_use_logo);
    active_view->useBackgroundTexture(dialog.general_mdi_bg_use_texture);
    active_view->useBackgroundColor(dialog.general_mdi_bg_use_color);
    active_view->setBackgroundLogo(dialog.general_mdi_bg_logo);
    active_view->setBackgroundTexture(dialog.general_mdi_bg_texture);
    active_view->setBackgroundColor(dialog.general_mdi_bg_color);
    updateAllEmbViewscrollbars(dialog.display_show_scrollbars);
    updateAllEmbViewCrossHairColors(dialog.display_crosshair_color);
    updateAllEmbViewBackgroundColors(dialog.display_bg_color);
    updateAllEmbViewSelectBoxColors(dialog.display_selectbox_left_color,
                                          dialog.display_selectbox_left_fill,
                                          dialog.display_selectbox_right_color,
                                          dialog.display_selectbox_right_fill,
                                          dialog.display_selectbox_alpha);
    prompt->setPromptTextColor(EmbColor(dialog.prompt_text_color));
    prompt->setPromptBackgroundColor(EmbColor(dialog.prompt_bg_color));
    prompt->setPromptFontFamily(dialog.prompt_font_family);
    prompt->setPromptFontStyle(dialog.prompt_font_style);
    prompt->setPromptFontSize(dialog.prompt_font_size);
    updateAllEmbViewGridColors(dialog.grid_color);
    updateAllEmbViewRulerColors(dialog.ruler_color);
    if (dialog.lwt_show_lwt) {
        statusbarLwtButton->enableLwt();
    }
    else {
        statusbarLwtButton->disableLwt();
    }
    if (dialog.lwt_real_render) {
        statusbarLwtButton->enableReal();
    }
    else {
        statusbarLwtButton->disableReal();
    }

    reject();
}

/**
 * @brief
 *
 * \a msg
 */
void
stub_implement(const char *msg)
{
    log_debug(msg);
}

/**
 * @brief The unified user interaction system which operates like a command line parser.
 *
 * \a command_line
 *
 * In order to have a complex version of saving work, with backups,
 * undo history and forks we need a good recording of what has happened.
 *
 * An action has been taken, we are at the current head of the stack.
 *
 * The action string is command similar to how a command
 * line works: seperated by spaces we have a function followed
 * by arguments.
 *
 * \todo Make day vision color settings.
 *
 * \todo check-for-updates should create a dialog.
 *
 * \todo changelog
 */
void
actuator(char command_line[200])
{
    char argv[11][200];
    int argc = 0;
    c_split(command_line, &argc, argv);
    const char *cmd = argv[0];

    if (n_views > 0) {
        add_to_undo_history(&(active_view->undo_history), command_line);
    }

    /* Command/argument seperation is done in order to reduce the number of
     * checks on every click before an action can run.
     *
     * File actions can be grouped under starts_with "file" etc.
     log_debug(command_line);

    /* Try to keep these if conditions alphabetical within each
     * indentation level.
     *
     * For actions with less than 5 lines of code, don't call a seperate
     * "*_action" function.
     if (streq(cmd, "about")) {
        settings.show_about_dialog = true;
        return;
    }
    if (streq(cmd, "alert")) {
        /* settings.show_alert = true; */
        return;
    }
    if (streq(cmd, "arc")) {
        arc_action();
        return;
    }
    if (streq(cmd, "circle")) {
        circle_action();
        return;
    }
    if (streq(cmd, "changelog")) {
        /* settings.show_changelog = true; */
        return;
    }
    if (streq(cmd, "close")) {
        close_action();
        return;
    }
    if (streq(cmd, "copy")) {
        copy_action();
        return;
    }
    if (streq(cmd, "cut")) {
        cut_action();
        return;
    }
    if (streq(cmd, "day")) {
            if (active_view != NULL) {
            active_view->setBackgroundColor(qRgb(255,255,255));
            active_view->setCrossHairColor(qRgb(0,0,0));
            active_view->setGridColor(qRgb(0,0,0));
        }
            return;
    }
    if (streq(cmd, "debug")) {
        if (strlen(cmd) > 6) {
            log_debug(command_line + 6);
        }
        return;
    }
    if (streq(cmd, "designdetails")) {
        settings.show_details_dialog = true;
        return;
    }
    if (streq(cmd, "donothing")) {
        log_debug("This action intentionally does nothing.");
        return;
    }
    if (streq(cmd, "editor")) {
        settings.show_editor = true;
        return;
    }
    if (streq(cmd, "exit") || streq(cmd, "quit")) {
        exit_action();
        return;
    }
    if (streq(cmd, "check-for-updates")) {
        printf("Visit https://libembroidery.org for information about new releases.\n");
        printf("Your version is: %s.\n", settings.version);
        return;
    }
    if (streq(cmd, "save")) {
        if (active_view != NULL) {
            embPattern_writeAuto(active_view->pattern, active_view->filename);
        }
        return;
    }
    if (streq(cmd, "platform")) {
        char s[200];
        platform_string(s);
        printf("%s\n", s);
        return;
    }
    if (streq(cmd, "settingsdialog")) {
        settings.show_settings_editor = true;
        return;
    }
    if (streq(cmd, "simulate")) {
        if (active_view != NULL) {
            active_view->simulate = true;
            active_view->simulation_start = clock();
        }
        return;
    }

    if (streq(cmd, "ellipse")) {
        ellipse_action();
        return;
    }
    if (streq(cmd, "error")) {
        error_action(argv[1]);
        return;
    }
    if (streq(cmd, "help")) {
        error_action(argv[1]);
        return;
    }
    if (streq(cmd, "icon")) {
        icon_action(atoi(argv[1]));
        return;
    }
    if (streq(cmd, "new")) {
        new_file_action();
        return;
    }
    if (streq(cmd, "night")) {
        /**
         * \todo Make night vision color settings.
                 if (active_view) {
            active_view->setBackgroundColor(qRgb(0,0,0));
            active_view->setCrossHairColor(qRgb(255,255,255));
            active_view->setGridColor(qRgb(255,255,255));
        }
            return;
    }
    if (streq(cmd, "open")) {
        open_file_action(argc-1, argv+1);
        return;
    }
    if (streq(cmd, "pan")) {
        pan_action(argv[1]);
        return;
    }
    if (streq(cmd, "paste")) {
        paste_action();
        return;
    }
    if (streq(cmd, "print")) {
        print_action();
        return;
    }
    if (streq(cmd, "rectangle")) {
        rectangle_action();
        return;
    }
    if (streq(cmd, "redo")) {
        redo_action();
        return;
    }
    if (streq(cmd, "undo")) {
        undo_action();
        return;
    }
    if (streq(cmd, "vulcanize")) {
        vulcanize_action();
        return;
    }
    if (streq(cmd, "window")) {
        if (argc < 2) {
            /* \todo missing argument error */
        }
        else {
            window_action(argv[1]);
        }
        return;
    }
    if (streq(cmd, "zoom")) {
        if (argc < 2) {
            /* \todo missing argument error */
        }
        else {
            zoom_action(argv[1]);
        }
        return;
    }

    if (streq(cmd, "blinkPrompt")) {
        error_action(argc-1, argv+1);
        return;
    }
    if (streq(cmd, "set_prompt_prefix")) {
        error_action(argc-1, argv+1);
        return;
    }
    if (streq(cmd, "append_prompt_history")) {
        error_action(argc-1, argv+1);
        return;
    }
    if (streq(cmd, "enablePromptRapidFire")) {
        error_action(argc-1, argv+1);
        return;
    }
    if (streq(cmd, "disablePromptRapidFire")) {
        error_action(argc-1, argv+1);
        return;
    }
    if (streq(cmd, "enableMoveRapidFire")) {
        error_action(argc-1, argv+1);
        return;
    }
    if (streq(cmd, "disableMoveRapidFire")) {
        error_action(argc-1, argv+1);
        return;
    }
    if (streq(cmd, "init_command")) {
        error_action(argc-1, argv+1);
        return;
    }
    if (streq(cmd, "end_command")) {
        error_action(argc-1, argv+1);
        return;
    }
    if (streq(cmd, "tip-of-the-day")) {
        error_action(argc-1, argv+1);
        return;
    }
    if (streq(cmd, "message-box")) {
        error_action(argc-1, argv+1);
        return;
    }
    if (streq(cmd, "print-area")) {
        error_action(argc-1, argv+1);
        return;
    }
    if (streq(cmd, "setBackgroundColor")) {
        error_action(argc-1, argv+1);
        return;
    }
    if (streq(cmd, "setCrossHairColor")) {
        error_action(argc-1, argv+1);
        return;
    }
    if (streq(cmd, "setGridColor")) {
        error_action(argc-1, argv+1);
        return;
    }
    if (streq(cmd, "textFont")) {
        error_action(argc-1, argv+1);
        return;
    }
    if (streq(cmd, "textSize")) {
        error_action(argc-1, argv+1);
        return;
    }
    if (streq(cmd, "textAngle")) {
        error_action(argc-1, argv+1);
        return;
    }
    if (streq(cmd, "textBold")) {
        error_action(argc-1, argv+1);
        return;
    }
    if (streq(cmd, "textItalic")) {
        error_action(argc-1, argv+1);
        return;
    }
    if (streq(cmd, "textUnderline")) {
        error_action(argc-1, argv+1);
        return;
    }
    if (streq(cmd, "textStrikeOut")) {
        error_action(argc-1, argv+1);
        return;
    }
    if (streq(cmd, "textOverline")) {
        error_action(argc-1, argv+1);
        return;
    }
    if (streq(cmd, "setTextFont")) {
        error_action(argc-1, argv+1);
        return;
    }
    if (streq(cmd, "setTextSize")) {
        error_action(argc-1, argv+1);
        return;
    }
    if (streq(cmd, "setTextAngle")) {
        error_action(argc-1, argv+1);
        return;
    }
    if (streq(cmd, "setTextBold")) {
        error_action(argc-1, argv+1);
        return;
    }
    if (streq(cmd, "setTextItalic")) {
        error_action(argc-1, argv+1);
        return;
    }
    if (streq(cmd, "setTextUnderline")) {
        error_action(argc-1, argv+1);
        return;
    }
    if (streq(cmd, "setTextStrikeOut")) {
        error_action(argc-1, argv+1);
        return;
    }
    if (streq(cmd, "setTextOverline")) {
        error_action(argc-1, argv+1);
        return;
    }
    if (streq(cmd, "previewOn")) {
        error_action(argc-1, argv+1);
        return;
    }
    if (streq(cmd, "previewOff")) {
        error_action(argc-1, argv+1);
        return;
    }
    if (streq(cmd, "allowRubber")) {
        error_action(argc-1, argv+1);
        return;
    }
    if (streq(cmd, "setRubberMode")) {
        error_action(argc-1, argv+1);
        return;
    }
    if (streq(cmd, "setRubberPoint")) {
        error_action(argc-1, argv+1);
        return;
    }
    if (streq(cmd, "setRubberText")) {
        error_action(argc-1, argv+1);
        return;
    }
    if (streq(cmd, "add_rubber")) {
        error_action(argc-1, argv+1);
        return;
    }
    if (streq(cmd, "clearRubber")) {
        error_action(argc-1, argv+1);
        return;
    }
    if (streq(cmd, "spareRubber")) {
        error_action(argc-1, argv+1);
        return;
    }
    if (streq(cmd, "addTextMulti")) {
        error_action(argc-1, argv+1);
        return;
    }
    if (streq(cmd, "addTextSingle")) {
        error_action(argc-1, argv+1);
        return;
    }
    if (streq(cmd, "addInfiniteLine")) {
        error_action(argc-1, argv+1);
        return;
    }
    if (streq(cmd, "addRay")) {
        error_action(argc-1, argv+1);
        return;
    }
    if (streq(cmd, "addLine")) {
        error_action(argc-1, argv+1);
        return;
    }
    if (streq(cmd, "addTriangle")) {
        error_action(argc-1, argv+1);
        return;
    }
    if (streq(cmd, "addRectangle")) {
        error_action(argc-1, argv+1);
        return;
    }
    if (streq(cmd, "addRoundedRectangle")) {
        error_action(argc-1, argv+1);
        return;
    }
    if (streq(cmd, "addArc")) {
        error_action(argc-1, argv+1);
        return;
    }
    if (streq(cmd, "addCircle")) {
        error_action(argc-1, argv+1);
        return;
    }
    if (streq(cmd, "addEllipse")) {
        error_action(argc-1, argv+1);
        return;
    }
    if (streq(cmd, "addPoint")) {
        error_action(argc-1, argv+1);
        return;
    }
    if (streq(cmd, "regularPolygon")) {
        error_action(argc-1, argv+1);
        return;
    }
    if (streq(cmd, "polygon")) {
        error_action(argc-1, argv+1);
        return;
    }
    if (streq(cmd, "polyline")) {
        error_action(argc-1, argv+1);
        return;
    }
    if (streq(cmd, "path")) {
        error_action(argc-1, argv+1);
        return;
    }
    if (streq(cmd, "addHorizontalDimension")) {
        error_action(argc-1, argv+1);
        return;
    }
    if (streq(cmd, "addVerticalDimension")) {
        error_action(argc-1, argv+1);
        return;
    }
    if (streq(cmd, "addImage")) {
        error_action(argc-1, argv+1);
        return;
    }
    if (streq(cmd, "addDimLeader")) {
        error_action(argc-1, argv+1);
        return;
    }
    if (streq(cmd, "setCursorShape")) {
        error_action(argc-1, argv+1);
        return;
    }
    if (streq(cmd, "calculateAngle")) {
        error_action(argc-1, argv+1);
        return;
    }
    if (streq(cmd, "calculateDistance")) {
        error_action(argc-1, argv+1);
        return;
    }
    if (streq(cmd, "perpendicularDistance")) {
        error_action(argc-1, argv+1);
        return;
    }

    \todo selection details need to be shown to the user,
        like they were through the property editor.

    if (streq(cmd, "selectAll")) {
        EmbPath allPath;
        allPath.addRect(active_view->sceneRect());
        // active_view->setSelectionArea(allPath, IntersectsItemShape, this->transform());
        return;
    }
    if (streq(cmd, "clear_selection")) {
        error_action(argc-1, argv+1);
        return;
    }
    if (streq(cmd, "delete")) {
        error_action(argc-1, argv+1);
        return;
    }
    if (streq(cmd, "move")) {
        error_action(argc-1, argv+1);
        return;
    }
    if (streq(cmd, "scale")) {
        error_action(argc-1, argv+1);
        return;
    }
    if (streq(cmd, "rotate")) {
        error_action(argc-1, argv+1);
        return;
    }
    if (streq(cmd, "mirror")) {
        error_action(argc-1, argv+1);
        return;
    }
    if (streq(cmd, "qsnapX")) {
        error_action(argc-1, argv+1);
        return;
    }
    if (streq(cmd, "qsnapY")) {
        error_action(argc-1, argv+1);
        return;
    }
    if (streq(cmd, "include")) {
        error_action(argc-1, argv+1);
        return;
    }
    if (streq(cmd, "details")) {
        details_action(argc-1, argv+1);
        return;
    }
    if (streq(cmd, "paste")) {
        paste_action(argc-1, argv+1);
        return;
    }
    if (streq(cmd, "polygon")) {
        polygon_action(argc-1, argv+1);
        return;
    }
    if (streq(cmd, "polyline")) {
        polyline_action(argc-1, argv+1);
        return;
    }
    if (streq(cmd, "help")) {
        help_action(argc-1, argv+1);
        return;
    }
    if (streq(cmd, "saveas")) {
        save_file_as_action(argc-1, argv+1);
        return;
    }
    if (streq(cmd, "whatsthis")) {
        whatsThisContextHelp_action(argc-1, argv+1);
        return;
    }
    if (streq(cmd, "makelayercurrent")) {
        makeLayerActive_action(argc-1, argv+1);
        return;
    }
    if (streq(cmd, "layers")) {
        layerManager_action(argc-1, argv+1);
        return;
    }
    if (streq(cmd, "layerprevious")) {
        layerPrevious_action(argc-1, argv+1);
        return;
    }
    if (streq(cmd, "selectAll")) {
        for (i=0; i<active_view->pattern->objects; i++) {
            active_view->selected[i] = active_view->pattern->objects[i];
        }
        return;
    }
    if (streq(cmd, "addToSelection")) {
        Using cursor input, we interpret what to add.
        active_view->selected[active_view->n_selected] = ;
        active_view->n_selected++;
        return;
    }
    if (streq(cmd, "clear_selection")) {
        active_view->n_selected = 0;
        return;
    }
    if (streq(cmd, "delete_selection")) {
        DeleteSelected_action(argc-1, argv+1);
        return;
    }
    if (streq(cmd, "path")) {
        path_action(argc-1, argv+1);
        return;
    }
    if (streq(cmd, "heart")) {
        heart_action(argc-1, argv+1);
        return;
    }
    if (streq(cmd, "treble clef")) {
        treble_clef_action(argc-1, argv+1);
        return;
    }
    if (streq(cmd, "line")) {
        line_action(argc-1, argv+1);
        return;
    }
    if (streq(cmd, "dolphin")) {
        dolphin_action(argc-1, argv+1);
        return;
    }
    if (streq(cmd, "tab")) {
        tab_action(argc-1, argv+1);
        return;
    }
    if (streq(cmd, "save-as")) {
        save_file_as_action(argc-1, argv+1);
        return;
    }
    if (streq(cmd, "select-all")) {
        select_all(argc-1, argv+1);
        return;
    }
    if (streq(cmd, "whats-this")) {
        whats_this(argc-1, argv+1);
        return;
    }
    if (streq(cmd, "print-pattern")) {
        print_pattern(argc-1, argv+1);
        return;
    }
    if (streq(cmd, "copy-object")) {
        copy(argc-1, argv+1);
        return;
    }
    if (streq(cmd, "paste-object")) {
        paste_action(argc-1, argv+1);
        return;
    }
    if (streq(cmd, "help")) {
        help_action(argc-1, argv+1);
        return;
    }
    if (streq(cmd, "changelog-dialog")) {
        changelog(argc-1, argv+1);
        return;
    }
    if (streq(cmd, "settings-dialog")) {
        error_action(argc-1, argv+1);
        return;
    }
    if (streq(cmd, "make-layer-current")) {
        error_action(argc-1, argv+1);
        return;
    }
    if (streq(cmd, "layer-manager")) {
        error_action(argc-1, argv+1);
        return;
    }
    if (streq(cmd, "layer-selector")) {
        error_action(argc-1, argv+1);
        return;
    }
    if (streq(cmd, "color-selector")) {
        error_action(argc-1, argv+1);
        return;
    }
    if (streq(cmd, "line-type-selector")) {
        error_action(argc-1, argv+1);
        return;
    }
    if (streq(cmd, "line-weight-selector")) {
        error_action(argc-1, argv+1);
        return;
    }
    if (streq(cmd, "layer-previous")) {
        error_action(argc-1, argv+1);
        return;
    }
    if (streq(cmd, "hide-all-layers")) {
        error_action(argc-1, argv+1);
        return;
    }
    if (streq(cmd, "show-all-layers")) {
        error_action(argc-1, argv+1);
        return;
    }
    if (streq(cmd, "freeze-all-layers")) {
        error_action(argc-1, argv+1);
        return;
    }
    if (streq(cmd, "thaw-all-layers")) {
        error_action(argc-1, argv+1);
        return;
    }
    if (streq(cmd, "lock-all-layers")) {
        error_action(argc-1, argv+1);
        return;
    }
    if (streq(cmd, "unlock-all-layers")) {
        error_action(argc-1, argv+1);
        return;
    }
    if (streq(cmd, "text")) {
        if (streq(cmd, "font", "angle"(argc-1, argv+1);
        return;
    }
    if (streq(cmd, "distance")) {
        distance(argc-1, argv+1);
        return;
    }
    if (streq(cmd, "delete-object")) {
        delete_object(argc-1, argv+1);
        return;
    }
    if (streq(cmd, "locate_point")) {
        locate_point(argc-1, argv+1);
        return;
    }
    if (streq(cmd, "move")) {
        move_action(argc-1, argv+1);
        return;
    }
    if (streq(cmd, "export")) {
        export_action(argc-1, argv+1);
        return;
    }
    if (streq(cmd, "enable")) {
        if (argc < 2) {
            return;
        }
        if (n_views == 0) {
            return;
        }
        if (streq(argv[1], "grid")) {
            active_view->grid_mode = true;
            return;
        }
        if (streq(argv[1], "real")) {
            active_view->real_render = true;
            return;
        }
        if (streq(argv[1], "ruler")) {
            active_view->ruler_mode = true;
            return;
        }
        if (streq(argv[1], "ortho")) {
            active_view->ortho_mode = true;
            return;
        }
        if (streq(argv[1], "qsnap")) {
            active_view->qsnap_mode = true;
            return;
        }
        if (streq(argv[1], "polar")) {
            active_view->polar_mode = true;
            return;
        }
        if (streq(argv[1], "track")) {
            active_view->qtrack_mode = true;
            return;
        }
        if (streq(argv[1], "lwt")) {
            active_view->lwt_mode = true;
            return;
        }
        if (streq(argv[1], "metric")) {
            active_view->metric = true;
            return;
        }
        if (streq(argv[1], "text-bold")) {
            active_view->text_style_bold = true;
            return;
        }
        if (streq(argv[1], "text-italic")) {
            active_view->text_style_italic = true;
            return;
        }
        if (streq(argv[1], "text-underline")) {
            active_view->text_style_underline = true;
            return;
        }
        if (streq(argv[1], "text-overline")) {
            active_view->text_style_overline = true;
            return;
        }
        if (streq(argv[1], "text-strikeout")) {
            active_view->text_style_strikeout = true;
            return;
        }
        return;
    }
    if (streq(cmd, "disable")) {
        if (argc < 2) {
            return;
        }
        if (n_views == 0) {
            return;
        }
        if (streq(argv[1], "grid")) {
            active_view->grid_mode = false;
            return;
        }
        if (streq(argv[1], "real")) {
            active_view->real_render = false;
            return;
        }
        if (streq(argv[1], "ruler")) {
            active_view->ruler_mode = false;
            return;
        }
        if (streq(argv[1], "ortho")) {
            active_view->ortho_mode = false;
            return;
        }
        if (streq(argv[1], "qsnap")) {
            active_view->qsnap_mode = false;
            return;
        }
        if (streq(argv[1], "polar")) {
            active_view->polar_mode = false;
            return;
        }
        if (streq(argv[1], "track")) {
            active_view->qtrack_mode = false;
            return;
        }
        if (streq(argv[1], "lwt")) {
            active_view->lwt_mode = false;
            return;
        }
        if (streq(argv[1], "metric")) {
            active_view->metric = false;
            return;
        }
        if (streq(argv[1], "text-bold")) {
            active_view->text_style_bold = false;
            return;
        }
        if (streq(argv[1], "text-italic")) {
            active_view->text_style_italic = false;
            return;
        }
        if (streq(argv[1], "text-underline")) {
            active_view->text_style_underline = false;
            return;
        }
        if (streq(argv[1], "text-overline")) {
            active_view->text_style_overline = false;
            return;
        }
        if (streq(argv[1], "text-strikeout")) {
            active_view->text_style_strikeout = false;
            return;
        }
        return;
    }
    if (streq(cmd, "toggle")) {
        if (argc < 2) {
            return;
        }
        if (n_views == 0) {
            return;
        }
        if (streq(argv[1], "snap")) {
            active_view->snap_mode = !active_view->snap_mode;
            return;
        }
        if (streq(argv[1], "grid")) {
            active_view->grid_mode = !active_view->grid_mode;
            return;
        }
        if (streq(argv[1], "real")) {
            active_view->real_render = !active_view->real_render;
            return;
        }
        if (streq(argv[1], "ruler")) {
            active_view->ruler_mode = !active_view->ruler_mode;
            /**
            \todo These should be part of init_view
            rulerMetric = settings.ruler_metric;
            rulerColor = EmbColor(settings.ruler_color);
            rulerPixelSize = settings.ruler_pixel_size;
                    return;
        }
        if (streq(argv[1], "ortho")) {
            active_view->ortho_mode = !active_view->ortho_mode;
            return;
        }
        if (streq(argv[1], "qsnap")) {
            active_view->qsnap_mode = !active_view->qsnap_mode;
            return;
        }
        if (streq(argv[1], "qtrack")) {
            /* FIXME */
            active_view->qsnap_mode = !active_view->qsnap_mode;
            return;
        }
        if (streq(argv[1], "polar")) {
            active_view->polar_mode = !active_view->polar_mode;
            return;
        }
        if (streq(argv[1], "track")) {
            active_view->qtrack_mode = !active_view->qtrack_mode;
            return;
        }
        if (streq(argv[1], "lwt")) {
            active_view->lwt_mode = !active_view->lwt_mode;
            return;
        }
        if (streq(argv[1], "metric")) {
            active_view->metric = !active_view->metric;
            return;
        }
        if (streq(argv[1], "text-bold")) {
            active_view->text_style_bold = !active_view->text_style_bold;
            return;
        }
        if (streq(argv[1], "text-italic")) {
            active_view->text_style_italic = !active_view->text_style_italic;
            return;
        }
        if (streq(argv[1], "text-underline")) {
            active_view->text_style_underline = !active_view->text_style_underline;
            return;
        }
        if (streq(argv[1], "text-overline")) {
            active_view->text_style_overline = !active_view->text_style_overline;
            return;
        }
        if (streq(argv[1], "text-strikeout")) {
            active_view->text_style_strikeout = !active_view->text_style_strikeout;
            return;
        }
        return;
    }

    char msg[200];
    sprintf(msg, "ERROR: action %s not in function_table.", cmd);
    log_debug(msg);
}

/**
 * @brief     odo document this.
 */
void
arc_action(void)
{
    EmbArc arc;

    /* Should be created based on the current view if arguments aren't supplied. */
    arc.start.x = 0.0;
    arc.start.y = 0.0;
    arc.mid.x = 10.0;
    arc.mid.y = 20.0;
    arc.end.x = 30.0;
    arc.end.y = 40.0;

    printf("%f\n", arc.start.x);
    arc.color = getCurrentColor();

    if (view && active_scene) {
        embPattern_addArc(patterns[settings.pattern_index], arc);
        char * name;
        name  = "pattern" + atoi(pattern_index);
        name += ".arc" + atoi(patterns[settings.pattern_index]->arcs->count);
        rubber_points_mode[name] = OBJ_RUBBER_OFF;
        if (OBJ_RUBBER_OFF) {
            active_view->addToRubberRoom(arcObj);
        }
    }

    embPattern_addArcAbs(views[settings.pattern_index].pattern, arc);
}

/**
 * @brief .
 *
 * \todo circle fill
 */
void
circle_action(void)
{
    EmbCircle circle;

    /* Defaults */
    circle.center.x = 10.0;
    circle.center.y = 10.0;
    circle.radius = 20.0;
    /* getCurrentColor */

    if (argc > 1) {
        circle.center.x = atof(argv[0]);
        circle.center.y = -atof(argv[1]);
        if (isnan(circle.center.x)) {
            return;
        }
        if (isnan(circle.center.y)) {
            return;
        }
    }
    if (argc > 2) {
        circle.radius = atof(argv[2]);
        if (isnan(circle.radius)) {
            return;
        }
    }

    /* bool fill = results[3];
    obj->setObjectRubberMode(rubberMode);
    if (rubber_mode) {
        active_view->addToRubberRoom(obj);
        active_view->addItem(obj);
        }
    else {
        UndoableAddCommand* cmd = UndoableAddCommand(obj->data(OBJ_NAME).toString(), obj, view, 0);
        stack->push(cmd);
    }
    embPattern_addCircleAbs(views[settings.pattern_index].pattern, circle);
}

/**
 * @brief Close the current view.
 *
 * \todo finish copy_action
 */
void
close_action(void)
{

}

/**
 * @brief Copy what is currently in active_view->selected.
 *
 * \todo finish copy_action
 */
void
copy_action(void)
{

}

/**
 * @brief Cut what is currently in active_view->selected.
 *
 * \todo finish cut_action
 */
void
cut_action(void)
{

}

/**
 * @brief     odo document this.
 */
void
ellipse_action(void)
{
    EmbEllipse ellipse;
    double rot = atof(results[4]);
    bool fill = results[5];

    OBJ_RUBBER_OFF
    ellipse.center.x = result[0].double_data;
    ellipse.center.y = result[1].double_data;
    ellipse.radius.x = result[2].double_data;
    ellipse.radius.y = result[3].double_data;
    embPattern_addEllipseAbs(views[settings.pattern_index].pattern, ellipse);
}

/**
 * @brief
 *
 * \a msg
 */
void
error_action(const char *msg)
{
    set_prompt_prefix("ERROR: (" + results[0].str_data + ") " + results[1].str_data);
    append_prompt_history(char *());
    end_command();
}

void
exit_action(void)
{
    log_debug("Closing Embroidermodder 2.0.");

    settings.running = false;
    if (settings.prompt_save_history) {
        prompt->saveHistory("prompt.log", settings_prompt_save_history_as_html);
        // \todo get filename from settings
    }
    closeAllWindows();
}

/**
 * @brief
 *
 * \a filename
 */
void
export_action(const char *filename)
{
    if (n_views > 0) {
        embPattern_writeAuto(views[settings.pattern_index].pattern, filename);
    }
}

/**
 * @brief
 *
 * \a new_size
 */
void
icon_action(int new_size)
{
    if (new_size >= 16 && new_size <= 128) {
        settings.icon_size = new_size;
    }

    int iconSize;
    this->setIconSize(QSize(iconSize, iconSize));
    layerSelector->setIconSize(QSize(iconSize*4, iconSize));
    colorSelector->setIconSize(QSize(iconSize,   iconSize));
    linetypeSelector->setIconSize(QSize(iconSize*4, iconSize));
    lineweightSelector->setIconSize(QSize(iconSize*4, iconSize));
    //set the minimum combobox width so the text is always readable
    layerSelector->setMinimumWidth(iconSize*4);
    colorSelector->setMinimumWidth(iconSize*2);
    linetypeSelector->setMinimumWidth(iconSize*4);
    lineweightSelector->setMinimumWidth(iconSize*4);

    //\todo low-priority: open app with iconSize set to 128. resize the icons to a smaller size.

    settings_general_icon_size = iconSize;
}

void
new_file_action(void)
{
    views[n_views] = init_view();
    views[n_views].pattern = embPattern_create();
    settings.pattern_index = n_views;
    active_view = &(views[n_views]);
    n_views++;
    updateMenuToolbarStatusbar();
    windowMenuAboutToShow();

    if (active_view != NULL) {
        active_view->recalculateLimits();
        actuator("zoom extents");
    }
}

/**
 * @brief
 *
 * \a argc
 * \a argv
 *
 * \todo warn user of too many open tabs.
 */
void
open_file_action(int argc, char argv[10][200])
{
    if (argc == 0) {
        settings.show_open_file_dialog = true;
    }
    else {
        for (int i=1; i<argc; i++) {
            if (n_views < 50) {
                views[n_views] = init_view();
                strcpy(views[n_views].filename, argv[i]);
                embPattern_readAuto(views[n_views].pattern, views[n_views].filename);
                settings.pattern_index = n_views;
                active_view = &(views[n_views]);
                n_views++;
            }
            else {
            }
        }
    }
    log_debug("openFile()");

    char *List files;
    bool preview = settings_opensave_open_thumbnail;
    openFilesPath = settings_opensave_recent_directory;

    // Check to see if this from the recent files list
    if (recent) {
        files.append(recentFile);
        openFilesSelected(files);
    }
    else {
        if (!preview) {
            \todo set getOpenFileNames' selectedFilter parameter from settings_opensave_open_format");
            files = QFileDialog_getOpenFileNames(translate("Open"), openFilesPath, formatFilterOpen);
            openFilesSelected(files);
        }
        else {
            PreviewDialog* openDialog = PreviewDialog(translate("Open w/Preview"), openFilesPath, formatFilterOpen);
            \todo set openDialog->selectNameFilter(const char * filter) from settings_opensave_open_format");
            connect(openDialog, filesSelected(const char *List&), openFilesSelected(const char *List&)));
            openDialog->exec();
        }
    }

    openFileSelected

    bool doOnce = true;

    if (filesToOpen.count()) {
        for (int i = 0; i < filesToOpen.count(); i++) {
            if (!validFileFormat(filesToOpen[i])) {
                continue;
            }

            SubEmbView* existing = findEmbView(filesToOpen[i]);
            if (existing) {
                active_view->setActiveSubWindow(existing);
                continue;
            }

            //The docIndex doesn't need increased as it is only used for unnamed files
            numOfDocs++;
            EmbView* mdiWin = EmbView(docIndex, mainWin, mdiArea, SubWindow);
            connect(mdiWin, sendCloseMdiWin(EmbView*), onCloseMdiWin(EmbView*)));
            connect(mdiArea, subWindowActivated(SubEmbView*), onWindowActivated(SubEmbView*)));

            //Make sure the toolbars/etc... are shown before doing their zoomExtents
            if (doOnce) { updateMenuToolbarStatusbar(); doOnce = false; }

            if (mdiWin->loadFile(filesToOpen.at(i))) {
                statusbar_showMessage(translate("File(s) loaded"), 2000);
                mdiWin->show();
                mdiWin->showMaximized();
                //Prevent duplicate entries in the recent files list
                if (!settings_opensave_recent_list_of_files.contains(filesToOpen.at(i), CaseInsensitive))
                {
                    settings_opensave_recent_list_of_files.prepend(filesToOpen.at(i));
                }
                //Move the recent file to the top of the list
                else
                {
                    settings_opensave_recent_list_of_files.removeAll(filesToOpen.at(i));
                    settings_opensave_recent_list_of_files.prepend(filesToOpen.at(i));
                }
                settings_opensave_recent_directory = QFileInfo(filesToOpen.at(i)).absolutePath();

                if (active_view) {
                    active_view->recalculateLimits();
                    actuator("zoom extents");
                }
            }
            else {
                mdiWin->close();
            }
        }
    }

    windowMenuAboutToShow();

    log_debug("EmbView loadFile()");

    unsigned int tmpColor = getCurrentColor();

    QFile file(fileName);
    if (!file.open(QFile_ReadOnly | QFile_Text)) {
        warning_messagebox(translate("Error reading file"),
                             translate("Cannot read file %1:\n%2.")
                             .arg(fileName)
                             .arg(file.errorString()));
        return false;
    }

    char * ext = fileExtension(fileName);
    log_debug("ext: %s", ext));

    //Read
    EmbPattern* p = embPattern_create();
    if (!p) {
        printf("Could not allocate memory for embroidery pattern\n");
        exit(1);
    }
    int readSuccessful = 0;
    char * readError;
    int reader = emb_identify_format(fileName));
    if (reader < 0) {
        readSuccessful = 0;
        readError = "Unsupported read file type: " + fileName;
        log_debug("Unsupported read file type: " + fileName.toStdString());
    }
    else {
        readSuccessful = embPattern_read(p, fileName), reader);
        if (!readSuccessful) {
            readError = "Reading file was unsuccessful: " + fileName;
            log_debug("Reading file was unsuccessful: " + fileName.toStdString());
        }
    }
    if (!readSuccessful) {
        messagebox(MSG_WARNING, translate("Error reading pattern"), translate(readError)));
        messagebox(MSG_WARNING, translate("Error reading pattern"), translate("Cannot read pattern"));
    }
    else {
        \todo test embPattern_moveStitchListToPolylines more.
        embPattern_moveStitchListToPolylines(p);
        int stitchCount = p->stitchList->count;
        EmbPath path;


        setCurrentFile(fileName);
        statusbar_showMessage("File loaded.");
        char * stitches;
        stitches.setNum(stitchCount);

        if (settings_grid_load_from_file) {
            \todo Josh, provide me a hoop size and/or grid spacing from the pattern.
        }
    }
    embPattern_free(p);

    //Clear the undo stack so it is not possible to undo past this point.
    gview->undo_stack->clear();

    setCurrentColor(tmpColor);

    fileWasLoaded = true;
    setUndoCleanIcon(fileWasLoaded);
    return fileWasLoaded;
}

void
redo_action(void)
{
    char * prefix = prompt->getPrefix();
    if (dockUndoEdit->canRedo()) {
        prompt->setPrefix("Redo " + dockUndoEdit->redoText());
        prompt->appendHistory(char *());
        dockUndoEdit->redo();
        prompt->setPrefix(prefix);
    }
    else {
        prompt->alert("Nothing to redo");
        prompt->setPrefix(prefix);
    }
}

/**
 * @brief
 *
 * \a mode
 *
 * \todo Undo records for pan action.
 */
void
pan_action(const char *mode)
{
    if (streq(mode, "realtime")) {
        /* active_view->panningRealTimeActive = true; */
        return;
    }
    if (streq(mode, "point")) {
        /* active_view->panningPointActive = true; */
        return;
    }
    if (streq(mode, "left")) {
        active_view->origin.x += active_view->scale;
            horizontalscrollbar()->setValue(horizontalscrollbar()->value() + panDistance);
        updateMouseCoords(viewMousePoint);
            return;
    }
    if (streq(mode, "right")) {
        active_view->origin.x -= active_view->scale;
            horizontalscrollbar()->setValue(horizontalscrollbar()->value() - panDistance);
        updateMouseCoords(viewMousePoint);
            return;
    }
    if (streq(mode, "up")) {
        active_view->origin.y += active_view->scale;
            verticalscrollbar()->setValue(verticalscrollbar()->value() + panDistance);
        updateMouseCoords(viewMousePoint);
            return;
    }
    if (streq(mode, "down")) {
        active_view->origin.y -= active_view->scale;
            verticalscrollbar()->setValue(verticalscrollbar()->value() - panDistance);
        updateMouseCoords(viewMousePoint);
            return;
    }
}

void
paste_action(void)
{

}

void
print_action(void)
{
    if (active_view == NULL) {
        return;
    }

    log_debug("print()");
    QPrintDialog dialog(&printer, this);
    if (dialog.exec() == QDialog_Accepted) {
        QPainter painter(&printer);
        if (settings_printing_disable_bg) {
            //Save current bg
            QBrush brush = gview->backgroundBrush();
            //Save ink by not printing the bg at all
            gview->setBackgroundBrush(NoBrush);
            //Print, fitting the viewport contents into a full page
            gview->render(&painter);
            //Restore the bg
            gview->setBackgroundBrush(brush);
        }
        else {
            //Print, fitting the viewport contents into a full page
            gview->render(&painter);
        }
    }
}

/**
 * @brief
 *
 * \todo rect fill for rectangle_action
 */
void
rectangle_action(void)
{
    EmbRect rect;
    rect.top = results[0].double_data;
    rect.bottom = results[1].double_data;
    rect.left = results[2].double_data;
    rect.right = results[3].double_data;
    printf("%f", rect.top);
    double rot = results[4].double_data;
    bool fill = results[5].toBool();

    addRectangle(x, y, w, h, rot, fill, OBJ_RUBBER_OFF);

    if (active_view == NULL) {
        return;
    }

    RectObject* obj = RectObject(x, -y, w, -h, getCurrentColor());
    obj->setRotation(-rot);
    obj->setObjectRubberMode(rubberMode);
    if (rubberMode) {
        active_view->addToRubberRoom(obj);
        active_view->addItem(obj);
    }
    else {
        UndoableAddCommand* cmd = UndoableAddCommand(obj->data(OBJ_NAME).toString(), obj, view, 0);
        stack->push(cmd);
    }
}

/**
 * @brief
 *
 * \a filename
 */
void
save_as_file_action(char *filename)
{
    if (active_view != NULL) {
        embPattern_writeAuto(active_view->pattern, filename);
            openFilesPath = settings_opensave_recent_directory;
        file = QFileDialog_getSaveFileName(translate("Save As"), openFilesPath, formatFilterSave);
        }
}

/**
 * @brief
 *
 * \a font
 */
void
text_font_action(const char *font)
{
    strcpy(settings.text_font, font);
}

/**
 * @brief
 *
 * \a size
 */
void
text_size_action(EmbReal size)
{
    settings.text_size = size;
}

/**
 * @brief
 *
 * \a angle
 */
void
text_angle_action(EmbReal angle)
{
    settings.text_angle = angle;
}

/* The triangle added is based on the interaction mode,
 * so .
 */

void
triangle_action(void)
{
    /* EmbPolygon polygon;
    EmbVector v1, v2, v3;
    v1.x = results[0].double_data;
    v1.y = results[1].double_data;
    v2.x = results[2].double_data;
    v2.y = results[3].double_data;
    v3.x = results[4].double_data;
    v3.y = results[5].double_data;
    double rot = results[6].double_data;
    int fill = results[7].int_data;

    printf("%f %f %f %f %d\n", v1.x, v2.x, v3.y, rot, fill);
    embPattern_addPolygonAbs(views[settings.pattern_index].pattern, polygon); */
}

void
undo_action(void)
{
    char * prefix = prompt->getPrefix();
    if (dockUndoEdit->canUndo()) {
        prompt->setPrefix("Undo " + dockUndoEdit->undoText());
        prompt->appendHistory(char *());
        dockUndoEdit->undo();
        prompt->setPrefix(prefix);
    }
    else {
        prompt->alert("Nothing to undo");
        prompt->setPrefix(prefix);
    }
}

/**
 * @brief Alter the window layout based on the char array \a arg.
 */
void
window_action(const char *arg)
{
    if (active_view == NULL) {
        return;
    }

    if (streq(arg, "cascade")) {
            cascadeSubWindows();
        zoomExtentsAllSubWindows();
            return;
    }
    if (streq(arg, "tile")) {
            tileSubWindows();
        zoomExtentsAllSubWindows();
            return;
    }
    if (streq(arg, "close")) {
        log_debug("onCloseWindow()");

            numOfDocs--;

        bool keepMaximized;
        if (theMdiWin) {
            keepMaximized = theMdiWin->isMaximized();
        }

        active_view->removeSubWindow(theMdiWin);
        active_view->deleteLater();

        updateMenuToolbarStatusbar();
        windowMenuAboutToShow();

        if (keepMaximized) {
            active_view->showMaximized();
        }
            return;
    }
    if (streq(arg, "closeall")) {
        /* active_view->closeAllSubWindows(); */
        return;
    }
    if (streq(arg, "next")) {
        /* active_view->activateNextSubWindow(); */
        return;
    }
    if (streq(arg, "previous")) {
        /* active_view->activatePreviousSubWindow(); */
        return;
    }
}

/**
 * @brief
 *
 * \a arg
 */
void
zoom_action(const char *arg)
{
    if (active_view == NULL) {
        return;
    }

    if (streq(arg, "in")) {
        if (!allow_zoom_in()) {
            return;
        }

        active_view->scale *= 2.0;

            EmbVector cntr = map_to_scene(EmbVector(width()/2,height()/2));
        scale(settings_display_zoomscale_in, settings_display_zoomscale_in);

        centerOn(cntr);
            return;
    }
    if (streq(arg, "out")) {
        if (!allow_zoom_out()) {
            return;
        }

        active_view->scale /= 2.0;

            EmbVector cntr = map_to_scene(EmbVector(width()/2,height()/2));
        scale(settings_display_zoomscale_out, settings_display_zoomscale_out);

        centerOn(cntr);
            return;
    }
    if (streq(arg, "all")) {
        log_debug("zoom all");
            stub_implement("Implement zoomAll.");
            return;
    }
    if (streq(arg, "window")) {
        log_debug("zoom window");
            active_view->zoomWindowActive = true;
        active_view->selectingActive = false;
            clear_selection();
        return;
    }
    if (streq(arg, "center")) {
        log_debug("zoom center");
        /**
         * \todo Implement zoomCenter.
             return;
    }
    if (streq(arg, "dynamic")) {
        log_debug("zoom dynamic");
        /**
         * \todo Implement zoomDynamic.
             return;
    }
    if (streq(arg, "extents")) {
        log_debug("zoom extents");
            EmbRect extents = active_view->itemsBoundingRect();
        if (extents.isNull()) {
            extents.setWidth(settings.grid_size_x);
            extents.setHeight(settings.grid_size_y);
            extents.moveCenter(EmbVector(0,0));
        }
        fitInEmbView(extents, KeepAspectRatio);

        active_view->undo_stack;
        UndoableNavCommand* cmd = UndoableNavCommand("ZoomExtents", view, 0);
        stack->push(cmd);
            return;
    }
    if (streq(arg, "previous")) {
        log_debug("zoom previous");
        /**
         * \todo Implement zoomPrevious.
             return;
    }
    if (streq(arg, "realtime")) {
        log_debug("zoomRealtime()");
        /**
         * \todo Implement zoomRealtime.
             return;
    }
    if (streq(arg, "scale")) {
        log_debug("zoomScale()");
        /**
         * \todo Implement zoomScale.
             return;
    }
    if (streq(arg, "selected")) {
        log_debug("zoomSelected()");
            QUndoStack* stack = active_view->undo_stack;
        UndoableNavCommand* cmd = UndoableNavCommand("ZoomSelected", active_view, 0);
        stack->push(cmd);

        EmbPath selectedRectPath;
        for (EmbGeometry* item, active_view->selected) {
            selectedRectPath.addPolygon(item->map_to_scene(item->boundingRect()));
        }
        EmbRect selectedRect = selectedRectPath.boundingRect();
        if (selectedRect.isNull()) {
            information_messagebox(translate("ZoomSelected Preselect"),
                translate("Preselect objects before invoking the zoomSelected command."));
            //\todo Support Post selection of objects
        }
        fitInEmbView(selectedRect, KeepAspectRatio);
            return;
    }
}

/*
 * EnablePromptRapidFire();
 * DisablePromptRapidFire();
 * EnableMoveRapidFire();
 * DisableMoveRapidFire();
 */

void
messagebox_action(void)
{
    char * type = results[0];
    char * title = results[1];
    char * text = results[2];

    if (type != "critical" && type != "information" && type != "question" && type != "warning") {
        log_debug("messageBox(): first argument must be \"critical\", \"information\", \"question\" or \"warning\".");
    }

    MessageBox(type, title, text);
}

void
print_area_action(void)
{
    int x = results[0].int_data;
    int y = results[1].int_data;
    int w = results[2].int_data;
    int h = results[3].int_data;

    PrintArea(x, y, w, h);
}

void
background_color_action(void)
{
    int r = results[0].int_data;
    int g = results[1].int_data;
    int b = results[2].int_data;

    if (!validRGB(r, g, b)) {
        return;
    }

    setBackgroundColor(r, g, b);
}

void
crosshair_color_action(void)
{
    int r = results[0].int_data;
    int g = results[1].int_data;
    int b = results[2].int_data;

    if (!validRGB(r, g, b)) {
        return;
    }

    setCrossHairColor(r, g, b);
}

void
grid_color_action(void)
{
    int r = results[0].int_data;
    int g = results[1].int_data;
    int b = results[2].int_data;

    if (!validRGB(r, g, b)) {
        return;
    }

    setGridColor(r, g, b);
}

void
preview_on_action(void)
{
    char * cloneStr = results[0).toString().toUpper();
    char * modeStr  = results[1).toString().toUpper();
    EmbReal x          = results[2];
    EmbReal y          = results[3];
    EmbReal data       = results[4];

    int clone = PREVIEW_CLONE_NULL;
    int mode = PREVIEW_MODE_NULL;
    if     (cloneStr == "SELECTED") { clone = PREVIEW_CLONE_SELECTED; }
    else if (cloneStr == "RUBBER")   { clone = PREVIEW_CLONE_RUBBER;   }
    else                            { log_debug("previewOn(): first argument must be \"SELECTED\" or \"RUBBER\"."); }

    if     (modeStr == "MOVE")   { mode = preview.MODE_MOVE;   }
    else if (modeStr == "ROTATE") { mode = preview.MODE_ROTATE; }
    else if (modeStr == "SCALE")  { mode = preview.MODE_SCALE;  }
    else                         { log_debug("previewOn(): second argument must be \"MOVE\", \"ROTATE\" or \"SCALE\"."); }

    PreviewOn(clone, mode, x, y, data);
}

/**
 * @brief Set the rubber mode action object
 *
 * \a label
 * \a mode
 */
void
set_rubber_mode_action(char *label, int mode)
{
    if (active_view == NULL) {
        return;
    }
    switch (mode) {
    case CIRCLE_MODE_1P_RAD:
        active_view->rubber_mode = OBJ_RUBBER_CIRCLE_1P_RAD;
        break;
    case CIRCLE_1P_DIA:
        active_view->rubber_mode = OBJ_RUBBER_CIRCLE_1P_DIA;
        break;
    case CIRCLE_2P:
        active_view->rubber_mode = OBJ_RUBBER_CIRCLE_2P;
        break;
    case CIRCLE_3P:
        active_view->rubber_mode = OBJ_RUBBER_CIRCLE_3P;
        break;
    case CIRCLE_TTR:
        active_view->rubber_mode = OBJ_RUBBER_CIRCLE_TTR;
        break;
    case CIRCLE_TTT:
        active_view->rubber_mode = OBJ_RUBBER_CIRCLE_TTT;
        break;
    case DIM_LEADER_LINE:
        active_view->rubber_mode = OBJ_RUBBER_DIMLEADER_LINE;
        break;
    case ELLIPSE_LINE:
        active_view->rubber_mode = OBJ_RUBBER_ELLIPSE_LINE;
        break;
    case ELLIPSE_MAJORDIAMETER_MINORRADIUS:
        active_view->rubber_mode = OBJ_RUBBER_ELLIPSE_MAJORDIAMETER_MINORRADIUS;
        break;
    case ELLIPSE_MAJORRADIUS_MINORRADIUS:
        active_view->rubber_mode = OBJ_RUBBER_ELLIPSE_MAJORRADIUS_MINORRADIUS;
        break;
    case ELLIPSE_ROTATION:
        active_view->rubber_mode = OBJ_RUBBER_ELLIPSE_ROTATION;
        break;
    case LINE:
        active_view->rubber_mode = OBJ_RUBBER_LINE;
        break;
    case POLYGON:
        active_view->rubber_mode = OBJ_RUBBER_POLYGON;
        break;
    case POLYGON_INSCRIBE:
        active_view->rubber_mode = OBJ_RUBBER_POLYGON_INSCRIBE;
        break;
    case POLYGON_CIRCUMSCRIBE:
        active_view->rubber_mode = OBJ_RUBBER_POLYGON_CIRCUMSCRIBE;
        break;
    case POLYLINE:
        active_view->rubber_mode = OBJ_RUBBER_POLYLINE;
        break;
    case RECTANGLE:
        active_view->rubber_mode = OBJ_RUBBER_RECTANGLE;
        break;
    case TEXTSINGLE:
        active_view->rubber_mode = OBJ_RUBBER_TEXTSINGLE;
        break;
    default:
        log_debug("Unknown rubber_mode value.");
        active_view->rubber_mode = OBJ_RUBBER_OFF;
        break;
    }
}

void
rubber_point_action(void)
{
    char * key = results[0].toUpper();
    EmbReal x = results[1];
    EmbReal y = results[2];

    setRubberPoint(key, x, y);
}

void
rubber_text_action(void)
{
    char * key = results[0].toUpper();
    char * txt = results[1];

    setRubberText(key, txt);
}

/**
 * @brief
 *
 * \todo handle arc type
 * \todo handle block type
 * \todo handle DimAligned type
 * \todo handle DimAngular rubber action.
 * \todo handle DimArcLength rubber action.
 * \todo handle DimDiameter rubber action.
 * \todo handle ellipse arc type
 * \todo handle hatch type
 * \todo handle image type
 * \todo handle infinite line type
 * \todo handle path type
 * \todo handle point type
 *
 *  \todo handle dim linear type
 *
 *  \todo handle dim ordinate type
 *
 *  \todo handle dim radius type
 */
void
add_rubber_action(void)
{
    /**
    char * objType = results[0).toString().toUpper();

    if (!allowRubber()) {
        log_debug("add_rubber(): You must use vulcanize() before you can add another rubber object.");
        return;
    }

    EmbReal mx = MouseX();
    EmbReal my = MouseY();

    switch (objType) {
    case EMB_ARC: {
        break;
    }
    case EMB_BLOCK: {
        break;
    }
    case EMB_CIRCLE: {
        addCircle(mx, my, 0, false, OBJ_RUBBER_ON);
        break;
    }
    case EMB_DIMALIGNED: {
        break;
    }
    case EMB_DIMANGULAR: {
        break;
    }
    case EMB_DIMARCLENGTH: {
        break;
    }
    case EMB_DIMDIAMETER: {
        break;
    }
    case EMB_DIM_LEADER: {
        addDimLeader(mx, my, mx, my, 0, OBJ_RUBBER_ON);
    }
    case EMB_DIMLINEAR: {
        break;
    }
    case EMB_DIMORDINATE: {
        break;
    }
    case EMB_DIMRADIUS: {
        break;
    }
    case EMB_ELLIPSE: {
        addEllipse(mx, my, 0, 0, 0, 0, OBJ_RUBBER_ON);
        break;
    }
    case EMB_ELLIPSEARC: {
        break;
    }
    case EMB_HATCH: {
        break;
    }
    case EMB_IMAGE: {
        break;
    }
    case EMB_INFINITELINE: {
        break;
    }
    case EMB_LINE: {
        addLine(mx, my, mx, my, 0, OBJ_RUBBER_ON);
        break;
    }
    case EMB_PATH: {
        break;
    }
    case EMB_POINT: {
        break;
    }
    case EMB_POLYGON: {
        addPolygon(mx, my, EmbPath(), OBJ_RUBBER_ON);
        break;
    }
    case EMB_POLYLINE: {
        addPolyline(mx, my, EmbPath(), OBJ_RUBBER_ON);
        break;
    }
    case EMB_RAY: {
        //\todo handle this type
        break;
    }
    case EMB_RECT: {
        addRectangle(mx, my, mx, my, 0, 0, OBJ_RUBBER_ON);
        break;
    }
    case EMB_SPLINE: {
        //\todo handle this type
        break;
    }
    case EMB_TEXTMULTI: {
        //\todo handle this type
        break;
    }
    case EMB_TEXTSINGLE: {
        addTextSingle("", mx, my, 0, false, OBJ_RUBBER_ON);
        break;
    }
    default:
        break;
    }
}

/**
 * @brief
 *
 * \todo ClearRubber(); function
 */
void
spare_rubber_action(void)
{
    char * objID = results[0].toUpper();

    if (objID == "PATH") {
        spareRubber(SPARE_RUBBER_PATH);
    }
    else if (objID == "POLYGON") {
        spareRubber(SPARE_RUBBER_POLYGON);
    }
    else if (objID == "POLYLINE") {
        spareRubber(SPARE_RUBBER_POLYLINE);
    }
    else {
        bool ok = false;
        long int id = objID.toLongLong(&ok);
        if (!ok) return log_debug("spareRubber(): error converting object ID into an int64");
        spareRubber(id);
    }
}

void
text_multi_action(void)
{
    char *str = results[0].toString();
    EmbReal x = results[1];
    EmbReal y = results[2];
    EmbReal rot = results[3];
    bool fill = results[4].toBool();

    addTextMulti(str, x, y, rot, fill, OBJ_RUBBER_OFF);
}

void
text_single_action(void)
{
    char * str = results[0];
    EmbReal   x     = results[1];
    EmbReal   y     = results[2];
    EmbReal   rot   = results[3];
    bool   fill  = results[4).toBool();

    addTextSingle(str, x, y, rot, fill, OBJ_RUBBER_OFF);
}

void
infinite_line_action(void)
{
    //\todo parameter error checking
    log_debug("\todo finish addInfiniteLine command");
}

void
ray_action(void)
{
    //\todo parameter error checking
    log_debug("\todo finish addRay command");
}

void
line_action(void)
{
    EmbReal x1  = results[0];
    EmbReal y1  = results[1];
    EmbReal x2  = results[2];
    EmbReal y2  = results[3];
    EmbReal rot = results[4];

    embPattern_addLine(x1, y1, x2, y2, rot, OBJ_RUBBER_OFF);
}

void
rounded_rectangle_action(void)
{
    EmbReal x = atof(results[0]);
    EmbReal y = atof(results[1]);
    EmbReal w = results[2].toNumber();
    EmbReal h = results[3].toNumber();
    EmbReal rad = results[4].toNumber();
    EmbReal rot = results[5].toNumber();
    bool fill = results[6].toBool();

    addRoundedRectangle(x, y, w, h, rad, rot, fill);
}

/**
 * @brief
 *
 * \todo Use UndoableAddCommand for slots
 *
 * \todo slot fill
 */
void
add_slot_action(void)
{
    int rubberMode = OBJ_RUBBER_OFF;
    EmbReal centerX  = results[0];
    EmbReal centerY  = results[1];
    EmbReal diameter = results[2];
    EmbReal length = results[3];
    EmbReal rot = results[4];
    bool fill = results[5];

    SlotObject* slotObj = SlotObject(centerX, -centerY, diameter, length, getCurrentColor());
    slotObj->setRotation(-rot);
    slotObj->setObjectRubberMode(rubberMode);
    if (rubberMode) active_view->addToRubberRoom(slotObj);
    scene->addItem(slotObj);
    scene->update();
}

void
add_point_action(void)
{
    EmbReal x = atof(results[0]);
    EmbReal y = atof(results[1]);

    addPoint(x, y);
}

/**
 * @brief
 *
 * \todo finish polygon_action.
 */
void
polygon_action(void)
{
    QVariantList varList = results[0).toVariant().toList();
    int varSize = varList.size();
    int varSize = 10;
    if (varSize < 2) {
        log_debug("addPolygon(): array must contain at least two elements");
        return;
    }
    if (varSize % 2) {
        log_debug("addPolygon(): array cannot contain an odd number of elements");
        return;
    }

    bool lineTo = false;
    bool xCoord = true;
    EmbVector position = {0.0, 0.0};
    EmbVector start = {0.0, 0.0};
    EmbPath *path = NULL;
    int rubberMode = OBJ_RUBBER_OFF;
    for (QVariant var : varList) {
        if (var.canConvert(QVariant_Double)) {
            if (xCoord) {
                xCoord = false;
                x = var.toReal();
            }
            else {
                xCoord = true;
                y = -var.toReal();

                if (lineTo) { path.lineTo(x,y); }
                else       { path.moveTo(x,y); lineTo = true; startX = x; startY = y; }
            }
        }
        else {
            log_debug("addPolygon(): array contains one or more invalid elements");
            return;
        }
    }
    /* Close the polygon */
    path.closeSubpath();

    path.translate(-startX, -startY);

    embPattern_addPolygon(startX, startY, path, OBJ_RUBBER_OFF);

    ---------------------------------------------------

    QGraphicsScene* active_view = active_view->scene();
    QUndoStack* stack = active_view->undo_stack;
    EmbPolygon* obj = EmbPolygon(startX, startY, p, getCurrentColor());
    obj->setObjectRubberMode(rubberMode);
    if (rubberMode) {
        active_view->addToRubberRoom(obj);
        active_view->addItem(obj);
    }
    else {
        UndoableAddCommand* cmd = UndoableAddCommand(obj->data(OBJ_NAME).toString(), obj, view, 0);
        stack->push(cmd);
    }
}

/**
 * @brief
 *
 * NOTE: This  is different than the rest in that the Y+ is down (scripters need not worry about this)
 *
 * \a start
 * \a p
 * \a rubberMode
 *
 * \todo finish polyline_action.
 */
void
polyline_action(EmbVector start, EmbPath *p, int rubberMode)
{
    QGraphicsScene* active_view = active_view->scene();
    QUndoStack* stack = active_view->undo_stack;
    if (view && active_view && stack) {
        PolylineObject* obj = PolylineObject(startX, startY, p, getCurrentColor());
        obj->setObjectRubberMode(rubberMode);
        if (rubberMode) {
            active_view->addToRubberRoom(obj);
            active_view->addItem(obj);
                }
        else {
            UndoableAddCommand* cmd = UndoableAddCommand(obj->data(OBJ_NAME).toString(), obj, view, 0);
            stack->push(cmd);
        }
    }
    if (argc < 1)
        log_debug("addPolyline() requires one argument");
    if (!results[0).isArray())
        return log_debug("addPolyline(): first argument is not an array");

    QVariantList varList = results[0).toVariant().toList();
    int varSize = varList.size();
    if (varSize < 2) return log_debug("addPolyline(): array must contain at least two elements");
    if (varSize % 2) return log_debug("addPolyline(): array cannot contain an odd number of elements");

    bool lineTo = false;
    bool xCoord = true;
    EmbVector position = {0.0f, 0.0f};
    EmbPath *path = NULL;
    for (QVariant var, varList) {
        if (var.canConvert(QVariant_Double)) {
            if (xCoord) {
                xCoord = false;
                x = var.toReal();
            }
            else {
                xCoord = true;
                y = -var.toReal();

                if (lineTo) { path.lineTo(x,y); }
                else       { path.moveTo(x,y); lineTo = true; startX = x; startY = y; }
            }
        }
        else
            return log_debug("addPolyline(): array contains one or more invalid elements");
    }

    path.translate(-startX, -startY);

    addPolyline(startX, startY, path, OBJ_RUBBER_OFF);
}

/**
 * @brief Set the cursor shape action object
 *
 * \a shape
 *
 * \todo Uses the CURSOR_SHAPE_ defines.
 */
void
set_cursor_shape_action(int shape)
{
    /* Get view here */

    switch (shape) {
    case 0:
        break;
    default:
        break;
    }

    char * shape = str.toLower();
    if (shape == "arrow")
        active_view->setCursor(QCursor(ArrowCursor));
    else if (shape == "uparrow")
        active_view->setCursor(QCursor(UpArrowCursor));
    else if (shape == "cross")
        active_view->setCursor(QCursor(CrossCursor));
    else if (shape == "wait")
        active_view->setCursor(QCursor(WaitCursor));
    else if (shape == "ibeam")
        active_view->setCursor(QCursor(IBeamCursor));
    else if (shape == "resizevert")
        active_view->setCursor(QCursor(SizeVerCursor));
    else if (shape == "resizehoriz")
        active_view->setCursor(QCursor(SizeHorCursor));
    else if (shape == "resizediagleft")
        active_view->setCursor(QCursor(SizeBDiagCursor));
    else if (shape == "resizediagright")
        active_view->setCursor(QCursor(SizeFDiagCursor));
    else if (shape == "move")
        active_view->setCursor(QCursor(SizeAllCursor));
    else if (shape == "blank")
        active_view->setCursor(QCursor(BlankCursor));
    else if (shape == "splitvert")
        active_view->setCursor(QCursor(SplitVCursor));
    else if (shape == "splithoriz")
        active_view->setCursor(QCursor(SplitHCursor));
    else if (shape == "handpointing")
        active_view->setCursor(QCursor(PointingHandCursor));
    else if (shape == "forbidden")
        active_view->setCursor(QCursor(ForbiddenCursor));
    else if (shape == "handopen")
        active_view->setCursor(QCursor(OpenHandCursor));
    else if (shape == "handclosed")
        active_view->setCursor(QCursor(ClosedHandCursor));
    else if (shape == "whatsthis")
        active_view->setCursor(QCursor(WhatsThisCursor));
    else if (shape == "busy")
        active_view->setCursor(QCursor(BusyCursor));
    else if (shape == "dragmove")
        active_view->setCursor(QCursor(DragMoveCursor));
    else if (shape == "dragcopy")
        active_view->setCursor(QCursor(DragCopyCursor));
    else if (shape == "draglink")
        active_view->setCursor(QCursor(DragLinkCursor));
}

/**
 * @brief
 *
 * \todo Get mouse point.
 */
void
calculate_angle_action(void)
{
    EmbReal x1 = results[0];
    EmbReal y1 = results[1];
    EmbReal x2 = results[2];
    EmbReal y2 = results[3];

    output = embVector_angle(x1, y1, x2, y2);
}

/**
 * @brief
 *
 * \todo Get mouse point.
 */
void
calculate_distance_action(void)
{
    EmbLine line;
    line.start.x = results[0];
    line.start.y = results[1];
    line.end.x = results[2];
    line.end.y = results[3];

    return embVector_length(embLine_vector(line));
}

/**
 * @brief
 *
 * \a p
 * \a line
 * Returns EmbReal
 */
EmbReal
embVector_perpendicularDistance(EmbVector p, EmbLine line)
{
    EmbVector direction = embVector_subtract(line.end, line.start);
    EmbReal length = embVector_length(direction);
    EmbVector p_start = embVector_subtract(line.start, p);
    EmbReal v = embVector_cross(direction, p_start);
    return v / length;
}

/**
 * @brief
 *
 * \todo Get mouse point/move
 */
void
cut_selected(void)
{
    EmbVector pos;
    EmbReal x = results[0].double_data;
    double y = results[1].double_data;

    CutSelected(x, y);
}

/**
 * @brief
 *
 * \todo Get mouse point/move
 */
void
copy_selected(void)
{
    double x = results[0].double_data;
    double y = results[1].double_data;

    CopySelected(x, y);
}

/**
 * @brief
 *
 * \todo Get mouse point/move
 */
void
paste_selected(void)
{
    double x = results[0].double_data;
    double y = results[1].double_data;

    PasteSelected(x, y);
}

/**
 * @brief
 *
 * \todo Get mouse point/move
 */
void
move_selected_action(void)
{
    double dx = results[0].double_data;
    double dy = results[1].double_data;

    active_view->moveSelected(dx, -dy); }
}

/**
 * @brief
 *
 * \todo Get mouse point/move
 */
void
scale_selected_action(void)
{
    double x = results[0].double_data;
    double y = results[1].double_data;
    double factor = atof(results[2]);

    if (factor <= 0.0) {
        critical_messagebox(translate("ScaleFactor Error"),
                            translate("Hi there. If you are not a developer, report this as a bug. "
                            "If you are a developer, your code needs examined, and possibly your head too."));
    }

    active_view->scaleSelected(x, -y, factor);
}

void
rotate_selected_action(void)
{
    if (argc < 3) {
        log_debug("rotateSelected() requires three arguments");
        return;
    }

    double x = results[0].double_data;
    double y = results[1].double_data;
    double rot = atof(results[2]);

    active_view->rotateSelected(x, -y, -rot);
}

void
mirror_selected_action(EmbLine line)
{
    active_view->mirrorSelected(x1, -y1, x2, -y2);
}

void
stub_testing_action(void)
{
    warning_messagebox(translate("Testing Feature"), translate("<b>This feature is in testing.</b>"));
}

/**
 * @brief
 *
 * \todo Check website for new versions, commands, etc...
 */
void
checkForUpdates(void)
{
    log_debug("checkForUpdates()");
}

void
whats_this_context_help_action(void)
{
    log_debug("whatsThisContextHelp()");
    // whats_this_active = true;
}

void
button_tip_of_the_day_clicked(int button)
{
    log_debug("buttonTipOfTheDayClicked(%d)"); //, button);
    if (button == CustomButton1) {
        if (settings_general_current_tip > 0)
            settings_general_current_tip--;
        else
            settings_general_current_tip = listTipOfTheDay.size()-1;
        labelTipOfTheDay->setText(listTipOfTheDay.value(settings_general_current_tip));
    }
    else if (button == CustomButton2) {
        settings_general_current_tip++;
        if (settings_general_current_tip >= listTipOfTheDay.size())
            settings_general_current_tip = 0;
        labelTipOfTheDay->setText(listTipOfTheDay.value(settings_general_current_tip));
    }
    else if (button == CustomButton3) {
        wizardTipOfTheDay->close();
    }
}

/**
 * @brief Set the Undo Clean Icon object
 *
 */
void
setUndoCleanIcon(void)
{
    /* bool opened */
    //dockUndoEdit->updateCleanIcon(opened);
}

void
updateAllEmbViewscrollbars(bool val)
{
    for (int i = 0; i < n_views; i++) {
        EmbView* mdiWin = &(views[i]);
            showEmbViewscrollbars(mdiWin, val);
        }
}

void
updateAllEmbViewCrossHairColors(unsigned int color)
{
    for (int i = 0; i < n_views; i++) {
        EmbView* mdiWin = &(views[i]);
        // set_viewCrossHairColor(mdiWin, color);
    }
}

/**
 * @brief
 *
 * \a color
 */
void
updateAllEmbViewBackgroundColors(unsigned int color)
{
    for (int i = 0; i < n_views; i++) {
        EmbView* mdiWin = &(views[i]);
        // set_viewBackgroundColor(mdiWin, color);
    }
}

void
updateAllEmbViewSelectBoxColors(
    unsigned int colorL,
    unsigned int fillL,
    unsigned int colorR,
    unsigned int fillR,
    int alpha)
{
    for (int i = 0; i < n_views; i++) {
        EmbView* mdiWin = &(views[i]);
        // set_view_selectBoxColors(mdiWin, colorL, fillL, colorR, fillR, alpha);
    }
}

void
updateAllEmbViewGridColors(unsigned int color)
{
    for (int i = 0; i < n_views; i++) {
        EmbView* view = &(views[i]);
        // set_viewGridColor(view, color);
    }
}

/**
 * @brief
 *
 * \a color
 */
void
updateAllEmbViewRulerColors(unsigned int color)
{
    for (int i = 0; i < n_views; i++) {
        EmbView* view = &(views[i]);
        //set_viewRulerColor(view, color);
    }
}

void
updatePickAddMode(bool val)
{
    settings.selection_mode_pickadd = val;
    dockPropEdit->updatePickAddModeButton(val);
}

void
pickAddModeToggled(void)
{
    bool val = !settings_selection_mode_pickadd;
    updatePickAddMode(val);
}

/**
 * @brief
 *
 * Layer Toolbar
 */
void
makeLayerActive(void)
{
    log_debug("makeLayerActive()");
    stub_implement("Implement makeLayerActive.");
}

void
layerManager(void)
{
    log_debug("layerManager()");
    stub_implement("Implement layerManager.");
    LayerManager layman(this);
    layman.exec();
}

void
layerPrevious(void)
{
    log_debug("layerPrevious()");
    stub_implement("Implement layerPrevious.");
}

void
layerSelectorIndexChanged(int index)
{
    char msg[200];
    sprintf(msg, "layerSelectorIndexChanged(%d)", index);
    log_debug(msg);
}

/**
 * @brief
 *
 * \a index
 *
 * \todo Handle ByLayer and ByBlock and Other...
 */
void
colorSelectorIndexChanged(int index)
{
    char msg[200];
    sprintf(msg, "colorSelectorIndexChanged(%d)", index);
    log_debug(msg);

    unsigned int newColor;
    int* comboBox = <int*>(sender());
    if (comboBox) {
        bool ok = 0;
        newColor = comboBox->itemData(index).toUInt(&ok);
        if (!ok) {
            messagebox_warning(
                translate("Color Selector Conversion Error"),
                translate("<b>An error has occured while changing colors.</b>"));
        }
    }
    else {
        messagebox_warning(
            translate("Color Selector Pointer Error"),
            translate("<b>An error has occured while changing colors.</b>"));
    }

    currentColorChanged(mdiWin, newColor);
}

void
linetypeSelectorIndexChanged(int index)
{
    log_debug("linetypeSelectorIndexChanged(%d)"); //, index);
}

void
lineweightSelectorIndexChanged(int index)
{
    log_debug("lineweightSelectorIndexChanged(%d)"); //, index);
}

void
textFontSelectorCurrentFontChanged(int font)
{
    log_debug("textFontSelectorCurrentFontChanged()");
    /* setTextFont(font.family()); */
}

/**
 * @brief
 *
 * \a index
 *
 * \todo check that the toReal() conversion is ok
 */
void
textSizeSelectorIndexChanged(int index)
{
    log_debug("textSizeSelectorIndexChanged(%d)"); //, index);
    /* settings_text_size = fabs(textSizeSelector->itemData(index).toReal()); */
}

/**
 * @brief Set the Text Font object
 *
 * \a str
 */
void
setTextFont(const char *str)
{
    textFontSelector->setCurrentFont(QFont(str));
    settings_text_font = str;
}

/**
 * @brief Set the Text Size object
 *
 * \a num
 */
void
setTextSize(double num)
{
    settings_text_size = fabs(num);
    int index = textSizeSelector->findText("Custom", MatchContains);
    if (index != -1) {
        textSizeSelector->removeItem(index);
    }
    textSizeSelector->addItem("Custom " + char *().setNum(num, 'f', 2) + " pt", num);
    index = textSizeSelector->findText("Custom", MatchContains);
    if (index != -1) {
        textSizeSelector->setCurrentIndex(index);
    }
}

/**
 * @brief Get the Current Layer object
 *
 * Returns char*
 */
char *
getCurrentLayer(void)
{
    /* return active_view->curLayer; */
    return "0";
}

/**
 * @brief Get the Current Color object
 *
 * Returns unsigned int
 *
 * \todo return color ByLayer
 */
unsigned int
getCurrentColor(void)
{
    /* return getCurrentColor(view); */
    return 0;
}

/**
 * @brief Get the Current Line Type object
 *
 * Returns char*
 */
char *
getCurrentLineType(void)
{
    /* return getCurrentLineType(active_view); */
    return "ByLayer";
}

/**
 * @brief Get the Current Line Weight object
 *
 * Returns char*
 */
char *
getCurrentLineWeight(void)
{
    /* return getCurrentLineWeight(active_view); */
    return "ByLayer";
}

/**
 * @brief
 *
 * \a txt
 */
void
promptHistoryAppended(const char *txt)
{
    promptHistoryAppended(active_view, txt);
    promptHistory.append("<br/>" + txt);
}

/**
 * @brief
 *
 * \a txt
 */
void
logPromptInput(const char *txt)
{
    logPromptInput(active_view, txt);
    promptInputList << txt;
    promptInputNum = promptInputList.size();
}

void
promptInputPrevious(void)
{
    active_view->promptInputPrevious();
    promptInputPrevNext(true);
}

void
promptInputNext(void)
{
    active_view->promptInputNext();

    promptInputPrevNext(false);
}

void
enablePromptRapidFire(void)
{
    active_view->rapid_fire = true;
}

void
disablePromptRapidFire(void)
{
    active_view->rapid_fire = false;
}

void
init_command(void)
{
    /* clearRubberRoom(view); */
}

void
end_command(void)
{
    if (active_view != NULL) {
        active_view->clearRubberRoom();
        active_view->previewOff();
        active_view->disableMoveRapidFire();
    }
    prompt->end_command();
}

void
Help(void)
{
    /* help(); */
}

void
TipOfTheDay(void)
{
    /* tipOfTheDay(); */
}

void
messagebox(int type, char title[200], char text[200])
{
    switch (type) {
    default:
    case MSG_CRITICAL:
        /* critical_messagebox(translate(title), translate(text)); */
        break;
    case MSG_INFORMATION:
        /* information_messagebox(translate(title), translate(text)); */
        break;
    case MSG_QUESTION:
        /* messagebox_question(translate(title), translate(text)); */
        break;
    case MSG_WARNING:
        /* messagebox_warning(translate(title), translate(text)); */
        break;
    }
}

/**
 * @brief
 *
 * \a x
 * \a y
 * \a w
 * \a h
 *
 * \todo Print Setup Stuff
 */
void
PrintArea(EmbReal x, EmbReal y, EmbReal w, EmbReal h)
{
    EmbRect rect;
    rect.top = y;
    rect.left = x;
    rect.bottom = y+h;
    rect.right = x+w;
    char msg[200];
    sprintf(msg, "PrintArea(%.2f, %.2f, %.2f, %.2f)", x, y, w, h);
    log_debug(msg);
    /* print(); */
}

/**
 * @brief Set the Background Color object
 *
 * \a r
 * \a g
 * \a b
 */
void
setBackgroundColor_(int r, int g, int b)
{
    EmbColor color;
    color.r = r;
    color.g = g;
    color.b = b;
    settings.display_bg_color = embColor_to_int(color, 255);
    /* updateAllEmbViewBackgroundColors(color); */
}

/**
 * @brief Set the crosshair color object
 *
 * \a r
 * \a g
 * \a b
 */
void
set_crosshair_color(int r, int g, int b)
{
    EmbColor color;
    color.r = r;
    color.g = g;
    color.b = b;
    settings.display_crosshair_color = embColor_to_int(color, 255);
    /* updateAllEmbViewCrossHairColors(color); */
}

/**
 * @brief Set the grid color object
 *
 * \a r
 * \a g
 * \a b
 */
void
set_grid_color(int r, int g, int b)
{
    EmbColor color;
    color.r = r;
    color.g = g;
    color.b = b;
    settings.grid_color = embColor_to_int(color, 255);
    /* updateAllEmbViewGridColors(color); */
}

void
vulcanize_action(void)
{
    active_view->vulcanizeRubberRoom();
}

void
clear_rubber(void)
{
    active_view->clearRubberRoom();
}

/**
 * @brief \a id.
 */
void
spare_rubber(size_t id)
{
    active_view->spareRubber(id);
    spareRubberList.append(id);
}

/**
 * @brief Set the rubber point object.
 */
void
set_rubber_point(const char *key, EmbVector position)
{
    // active_view->setRubberPoint(key, EmbVector(x, -y));
}

/**
 * @brief Set the rubber text object.
 */
void
set_rubber_text(const char *key, const char *txt)
{
    // active_view->setRubberText(key, txt);
}

void
add_text_multi(const char *str, EmbVector pos, EmbReal rot, bool fill, int rubberMode)
{

}

/**
 * @brief .
 *
 * \todo single line text fill
 */
void
add_text_single(const char *str, EmbReal x, EmbReal y, EmbReal rot, bool fill, int rubberMode)
{
    QGraphicsScene* active_view = active_view->scene();
    QUndoStack* stack = active_view->undo_stack;
    TextSingleObject* obj = TextSingleObject(str, x, -y, getCurrentColor());
    obj->setObjectTextFont(settings_text_font);
    obj->setObjectTextSize(settings_text_size);
    obj->setObjectTextStyle(settings_text_style_bold,
                            settings_text_style_italic,
                            settings_text_style_underline,
                            settings_text_style_strikeout,
                            settings_text_style_overline);
    obj->setObjectTextBackward(false);
    obj->setObjectTextUpsideDown(false);
    obj->setRotation(-rot);
    obj->setObjectRubberMode(rubberMode);
    if (rubberMode) {
        active_view->addToRubberRoom(obj);
        active_view->addItem(obj);
    }
    else {
        UndoableAddCommand* cmd = UndoableAddCommand(obj->data(OBJ_NAME).toString(), obj, view, 0);
        stack->push(cmd);
    }
}

/**
 * @brief
 *
 * \a x1
 * \a y1
 * \a x2
 * \a y2
 * \a rot
 */
void
add_infinite_line(EmbReal x1, EmbReal y1, EmbReal x2, EmbReal y2, EmbReal rot)
{
    EmbVector v1, v2;
    v1.x = x1;
    v1.y = y1;
    v2.x = x2;
    v2.y = y2;
}

/**
 * @brief
 *
 * \a x1
 * \a y1
 * \a x2
 * \a y2
 * \a rot
 */
void
add_ray(EmbReal x1, EmbReal y1, EmbReal x2, EmbReal y2, EmbReal rot)
{
    EmbVector v1, v2;
    v1.x = x1;
    v1.y = y1;
    v2.x = x2;
    v2.y = y2;
}

/**
 * @brief
 *
 * \a line
 * \a rot
 * \a rubberMode
 */
void
add_line(EmbLine line, EmbReal rot, int rubberMode)
{
    QGraphicsScene* active_view = active_view->scene();
    QUndoStack* stack = active_view->undo_stack;
    if (view && active_view && stack) {
        LineObject* obj = LineObject(x1, -y1, x2, -y2, getCurrentColor());
        obj->setRotation(-rot);
        obj->setObjectRubberMode(rubberMode);
        if (rubberMode) {
            active_view->addToRubberRoom(obj);
            active_view->addItem(obj);
                }
        else {
            UndoableAddCommand* cmd = UndoableAddCommand(obj->data(OBJ_NAME).toString(), obj, view, 0);
            stack->push(cmd);
        }
    }
}

void
add_ellipse(EmbReal centerX, EmbReal centerY, EmbReal width, EmbReal height, EmbReal rot, bool fill, int rubberMode)
{
    EmbEllipse ellipse;
    ellipse.center.x = centerX;
    ellipse.center.y = centerY;
    ellipse.radius.x = width;
    ellipse.radius.y = height;
    /* FIXME: To silence the unused variable warning. */
    printf("%d %f", active_view->metric, ellipse.center.x);
    printf("%f %d %d\n", rot, fill, rubberMode);
    QGraphicsScene* active_view = active_view->scene();
    QUndoStack* stack = active_view->undo_stack;

    EmbEllipse* obj = EmbEllipse(centerX, -centerY, width, height, getCurrentColor());
    obj->setRotation(-rot);
    obj->setObjectRubberMode(rubberMode);
    //\todo ellipse fill
    if (rubberMode) {
        active_view->addToRubberRoom(obj);
        active_view->addItem(obj);
    }
    else {
        UndoableAddCommand* cmd = UndoableAddCommand(obj->data(OBJ_NAME).toString(), obj, view, 0);
        stack->push(cmd);
    }
}

void
add_point(EmbReal x, EmbReal y)
{
    EmbVector position;
    position.x = x;
    position.y = y;
    QUndoStack* stack = active_view->undo_stack;
    if (view && stack) {
        PointObject* obj = PointObject(x, -y, getCurrentColor());
        UndoableAddCommand* cmd = UndoableAddCommand(obj->data(OBJ_NAME).toString(), obj, view, 0);
        stack->push(cmd);
    }
}

void
add_regular_polygon(EmbReal centerX, EmbReal centerY, int sides, int mode, EmbReal rad, EmbReal rot, bool fill)
{
    char msg[200];
    printf("%d", active_view->metric);
    sprintf(msg, "use add_regular_polygon variables: %f %f %d %d %f %f %d", centerX, centerY, sides, mode, rad, rot, fill);
    log_debug(msg);
}

void
path_action(EmbReal startX, EmbReal startY, EmbPath *p, int rubberMode)
{
    EmbVector start;
    start.x = startX;
    start.y = startY;
    printf("%f %d %d", start.x, p->pointList->count, rubberMode);
}

void
horizontal_dimension_action(void)
{

}

void
vertical_dimension_action(void)
{

}

/**
 * @brief Add an EmbImage to the current view.
 *
 * \todo finish image_action.
 */
void
image_action(void)
{

}

/**
 * @brief Add a EmbDimLeader to the current view.
 *
 * \todo Finish dim_leader_action.
 */
void
dim_leader_action(void)
{
    int rubberMode
    EmbReal x1  = results[0];
    EmbReal y1  = results[1];
    EmbReal x2  = results[2];
    EmbReal y2  = results[3];
    EmbReal rot = results[4];

    embPattern_addDimLeader(x1, y1, x2, y2, rot, OBJ_RUBBER_OFF);

    QGraphicsScene* active_view = active_view->scene();
    QUndoStack* stack = active_view->undo_stack;
    if (active_view != NULL) {
        EmbDimLeader* obj = EmbDimLeader(x1, -y1, x2, -y2, getCurrentColor());
        obj->setRotation(-rot);
        obj->setObjectRubberMode(rubberMode);
        if (rubberMode) {
            active_view->addToRubberRoom(obj);
            active_view->addItem(obj);
                }
        else {
            UndoableAddCommand* cmd = UndoableAddCommand(obj->data(OBJ_NAME).toString(), obj, view, 0);
            stack->push(cmd);
        }
    }
}

EmbReal
calculate_angle(EmbReal x1, EmbReal y1, EmbReal x2, EmbReal y2)
{
    EmbLine line;
    line.start.x = x1;
    line.start.y = y1;
    line.end.x = x2;
    line.end.y = y2;
    printf("%f", line.start.x);
    return embLine_angle(line);
    return 0.0;
}

EmbReal
perpendicular_distance(EmbVector p, EmbLine line)
{
    EmbVector iPoint;
    EmbVector norm;
    EmbVector delta = embVector_subtract(p, line.start);
    norm = embLine_normal(line);
    norm = embVector_add(norm, delta);
    norm.intersects(line, &iPoint);
    return embVector_distance(p, iPoint);
    return 0.0;
}

/**
 * @brief Create a All Actions object.
 *
 * \todo SCENE_QSNAP_POINT for each scene
 *
 * \todo Set What's This Context Help to statusTip for now so there is some infos there.
 * Make custom whats this context help popup with more descriptive help than just
 * the status bar/tip one liner(short but not real long) with a hyperlink in the custom popup
 * at the bottom to open full help file description. Ex: like wxPython AGW's SuperToolTip.
 *
 * \todo Finish All Commands ... <.<
 */
void
createAllActions(void)
{
    log_debug("Creating All Actions...");
    for (int i=0; action_list[i].action>=0; i++) {
        char * icon = action_list[i].command;
        char * toolTip = translate(action_list[i].menu_label);
        char * statusTip = translate(action_list[i].description);
        QAction *ACTION = QAction(load_icon(icon), toolTip, this);
        ACTION->setStatusTip(statusTip);
        ACTION->setObjectName(icon);
        ACTION->setWhatsThis(statusTip);

        if (icon == "textbold") {
            ACTION->setCheckable(true);
            connect(ACTION, toggled(bool), setTextBold(bool)));
        }
        else if (icon == "textitalic") {
            ACTION->setCheckable(true);
            connect(ACTION, toggled(bool), setTextItalic(bool)));
        }
        else if (icon == "textunderline") {
            ACTION->setCheckable(true);
            connect(ACTION, toggled(bool), setTextUnderline(bool)));
        }
        else if (icon == "textstrikeout") {
            ACTION->setCheckable(true);
            connect(ACTION, toggled(bool), setTextStrikeOut(bool)));
        }
        else if (icon == "textoverline") {
            ACTION->setCheckable(true);
            connect(ACTION, toggled(bool), setTextOverline(bool)));
        }
        else {
            connect(ACTION, &QAction_triggered, [=](){
                actuator(ACTION->objectName().toStdString());
            });
        }

        if (icon == "new") {
            ACTION->setShortcut(QKeySequence_New);
        }
        else if (icon == "open") {
            ACTION->setShortcut(QKeySequence_Open);
        }
        else if (icon == "save") {
            ACTION->setShortcut(QKeySequence_Save);
        }
        else if (icon == "saveas") {
            ACTION->setShortcut(QKeySequence_SaveAs);
        }
        else if (icon == "print") {
            ACTION->setShortcut(QKeySequence_Print);
        }
        else if (icon == "designdetails") {
            ACTION->setShortcut(QKeySequence("Ctrl+D"));
        }
        else if (icon == "exit") {
            ACTION->setShortcut(QKeySequence("Ctrl+Q"));
        }
        else if (icon == "cut") {
            ACTION->setShortcut(QKeySequence_Cut);
        }
        else if (icon == "copy") {
            ACTION->setShortcut(QKeySequence_Copy);
        }
        else if (icon == "paste") {
            ACTION->setShortcut(QKeySequence_Paste);
        }
        else if (icon == "windowclose") {
            ACTION->setShortcut(QKeySequence_Close);
        }
        else if (icon == "windownext") {
            ACTION->setShortcut(QKeySequence_NextChild);
        }
        else if (icon == "windowprevious") {
            ACTION->setShortcut(QKeySequence_PreviousChild);
        }
        actionHash.insert(action_list[i].action, ACTION);
    }

    actionHash.value(ACTION_windowclose)->setEnabled(numOfDocs > 0);
    actionHash.value(ACTION_designdetails)->setEnabled(numOfDocs > 0);
}

/**
 * @brief Create a layer toolbar object.
 *
 */
void
create_layer_toolbar(void)
{
    log_debug("MainWindow createLayerToolbar()");

    toolbarLayer->setObjectName("toolbarLayer");
    toolbarLayer->addAction(actionHash.value(ACTION_makelayercurrent));
    toolbarLayer->addAction(actionHash.value(ACTION_layers));

    layerSelector->setFocusProxy(prompt);
    //NOTE: Qt4.7 wont load icons without an extension...
    //\todo Create layer pixmaps by concatenating several icons
    layerSelector->addItem(load_icon("linetypebylayer"), "0");
    layerSelector->addItem(load_icon("linetypebylayer"), "1");
    layerSelector->addItem(load_icon("linetypebylayer"), "2");
    layerSelector->addItem(load_icon("linetypebylayer"), "3");
    layerSelector->addItem(load_icon("linetypebylayer"), "4");
    layerSelector->addItem(load_icon("linetypebylayer"), "5");
    layerSelector->addItem(load_icon("linetypebylayer"), "6");
    layerSelector->addItem(load_icon("linetypebylayer"), "7");
    layerSelector->addItem(load_icon("linetypebylayer"), "8");
    layerSelector->addItem(load_icon("linetypebylayer"), "9");
    toolbarLayer->add_widget(layerSelector);
    connect(layerSelector, currentIndexChanged(int), layerSelectorIndexChanged(int)));

    toolbarLayer->addAction(actionHash.value(ACTION_layerprevious));

    connect(toolbarLayer, topLevelChanged(bool), floatingChangedToolBar(bool)));
}

/**
 * @brief Create a properties toolbar object.
 *
 */
void
create_properties_toolbar(void)
{
    log_debug("MainWindow createPropertiesToolbar()");

    toolbarProperties->setObjectName("toolbarProperties");

    colorSelector->setFocusProxy(prompt);
    colorSelector->addItem(load_icon("colorbylayer"), "ByLayer");
    colorSelector->addItem(load_icon("colorbyblock"), "ByBlock");
    colorSelector->addItem(load_icon("colorred"    ), translate("Red"),     qRgb(255,  0,  0));
    colorSelector->addItem(load_icon("coloryellow" ), translate("Yellow"),  qRgb(255,255,  0));
    colorSelector->addItem(load_icon("colorgreen"  ), translate("Green"),   qRgb(  0,255,  0));
    colorSelector->addItem(load_icon("colorcyan"   ), translate("Cyan"),    qRgb(  0,255,255));
    colorSelector->addItem(load_icon("colorblue"   ), translate("Blue"),    qRgb(  0,  0,255));
    colorSelector->addItem(load_icon("colormagenta"), translate("Magenta"), qRgb(255,  0,255));
    colorSelector->addItem(load_icon("colorwhite"  ), translate("White"),   qRgb(255,255,255));
    colorSelector->addItem(load_icon("colorother"  ), translate("Other..."));
    toolbarProperties->add_widget(colorSelector);
    connect(colorSelector, currentIndexChanged(int), colorSelectorIndexChanged(int)));

    toolbarProperties->addSeparator();
    linetypeSelector->setFocusProxy(prompt);
    linetypeSelector->addItem(load_icon("linetypebylayer"   ), "ByLayer");
    linetypeSelector->addItem(load_icon("linetypebyblock"   ), "ByBlock");
    linetypeSelector->addItem(load_icon("linetypecontinuous"), "Continuous");
    linetypeSelector->addItem(load_icon("linetypehidden"    ), "Hidden");
    linetypeSelector->addItem(load_icon("linetypecenter"    ), "Center");
    linetypeSelector->addItem(load_icon("linetypeother"     ), "Other...");
    toolbarProperties->add_widget(linetypeSelector);
    connect(linetypeSelector, currentIndexChanged(int), linetypeSelectorIndexChanged(int)));

    toolbarProperties->addSeparator();
    lineweightSelector->setFocusProxy(prompt);
    lineweightSelector->addItem(load_icon("lineweightbylayer"), "ByLayer", -2.00);
    lineweightSelector->addItem(load_icon("lineweightbyblock"), "ByBlock", -1.00);
    lineweightSelector->addItem(load_icon("lineweightdefault"), "Default",  0.00);
    //\todo Thread weight is weird. See http://en.wikipedia.org/wiki/Thread_(yarn)#Weight
    lineweightSelector->addItem(load_icon("lineweight01"), "0.00 mm", 0.00);
    lineweightSelector->addItem(load_icon("lineweight02"), "0.05 mm", 0.05);
    lineweightSelector->addItem(load_icon("lineweight03"), "0.15 mm", 0.15);
    lineweightSelector->addItem(load_icon("lineweight04"), "0.20 mm", 0.20);
    lineweightSelector->addItem(load_icon("lineweight05"), "0.25 mm", 0.25);
    lineweightSelector->addItem(load_icon("lineweight06"), "0.30 mm", 0.30);
    lineweightSelector->addItem(load_icon("lineweight07"), "0.35 mm", 0.35);
    lineweightSelector->addItem(load_icon("lineweight08"), "0.40 mm", 0.40);
    lineweightSelector->addItem(load_icon("lineweight09"), "0.45 mm", 0.45);
    lineweightSelector->addItem(load_icon("lineweight10"), "0.50 mm", 0.50);
    lineweightSelector->addItem(load_icon("lineweight11"), "0.55 mm", 0.55);
    lineweightSelector->addItem(load_icon("lineweight12"), "0.60 mm", 0.60);
    lineweightSelector->addItem(load_icon("lineweight13"), "0.65 mm", 0.65);
    lineweightSelector->addItem(load_icon("lineweight14"), "0.70 mm", 0.70);
    lineweightSelector->addItem(load_icon("lineweight15"), "0.75 mm", 0.75);
    lineweightSelector->addItem(load_icon("lineweight16"), "0.80 mm", 0.80);
    lineweightSelector->addItem(load_icon("lineweight17"), "0.85 mm", 0.85);
    lineweightSelector->addItem(load_icon("lineweight18"), "0.90 mm", 0.90);
    lineweightSelector->addItem(load_icon("lineweight19"), "0.95 mm", 0.95);
    lineweightSelector->addItem(load_icon("lineweight20"), "1.00 mm", 1.00);
    lineweightSelector->addItem(load_icon("lineweight21"), "1.05 mm", 1.05);
    lineweightSelector->addItem(load_icon("lineweight22"), "1.10 mm", 1.10);
    lineweightSelector->addItem(load_icon("lineweight23"), "1.15 mm", 1.15);
    lineweightSelector->addItem(load_icon("lineweight24"), "1.20 mm", 1.20);
    lineweightSelector->setMinimumContentsLength(8); // Prevent dropdown text readability being squish...d.
    toolbarProperties->add_widget(lineweightSelector);
    connect(lineweightSelector, currentIndexChanged(int), lineweightSelectorIndexChanged(int)));

    connect(toolbarProperties, topLevelChanged(bool), floatingChangedToolBar(bool)));
}

/**
 * @brief Create a text toolbar object
 *
 */
void
create_text_toolbar(void)
{
    log_debug("MainWindow createTextToolbar()");

    toolbarText->setObjectName("toolbarText");

    toolbarText->add_widget(textFontSelector);
    textFontSelector->setCurrentFont(QFont(settings_text_font));
    connect(textFontSelector, currentFontChanged(const QFont&), textFontSelectorCurrentFontChanged(const QFont&)));

    toolbarText->addAction(actionHash.value(ACTION_textbold));
    actionHash.value(ACTION_textbold)_set_checked(settings_text_style_bold);
    toolbarText->addAction(actionHash.value(ACTION_textitalic));
    actionHash.value(ACTION_textitalic)_set_checked(settings_text_style_italic);
    toolbarText->addAction(actionHash.value(ACTION_textunderline));
    actionHash.value(ACTION_textunderline)_set_checked(settings_text_style_underline);
    toolbarText->addAction(actionHash.value(ACTION_textstrikeout));
    actionHash.value(ACTION_textstrikeout)_set_checked(settings_text_style_strikeout);
    toolbarText->addAction(actionHash.value(ACTION_textoverline));
    actionHash.value(ACTION_textoverline)_set_checked(settings_text_style_overline);

    textSizeSelector->setFocusProxy(prompt);
    textSizeSelector->addItem("6 pt",   6);
    textSizeSelector->addItem("8 pt",   8);
    textSizeSelector->addItem("9 pt",   9);
    textSizeSelector->addItem("10 pt", 10);
    textSizeSelector->addItem("11 pt", 11);
    textSizeSelector->addItem("12 pt", 12);
    textSizeSelector->addItem("14 pt", 14);
    textSizeSelector->addItem("18 pt", 18);
    textSizeSelector->addItem("24 pt", 24);
    textSizeSelector->addItem("30 pt", 30);
    textSizeSelector->addItem("36 pt", 36);
    textSizeSelector->addItem("48 pt", 48);
    textSizeSelector->addItem("60 pt", 60);
    textSizeSelector->addItem("72 pt", 72);
    setTextSize(settings_text_size);
    toolbarText->add_widget(textSizeSelector);
    connect(textSizeSelector, currentIndexChanged(int), textSizeSelectorIndexChanged(int)));

    connect(toolbarText, topLevelChanged(bool), floatingChangedToolBar(bool)));
}

/**
 * @brief Create a prompt toolbar object
 *
 */
void
create_prompt_toolbar(void)
{
    log_debug("MainWindow createPromptToolbar()");

    toolbarPrompt->setObjectName("toolbarPrompt");
    toolbarPrompt->add_widget(prompt);
    toolbarPrompt->setAllowedAreas(TopToolBarArea | BottomToolBarArea);
    connect(toolbarPrompt, topLevelChanged(bool)), prompt, floatingChanged(bool)));
}

MainWindow *
MainWindow_init(void)
{
    readSettings();

    char * appDir = qApp->applicationDirPath();
    // Verify that files/directories needed are actually present.
    check_load_file("/designs");
    check_load_file("/docs");
    check_load_file("/icons");
    check_load_file("/images");
    check_load_file("/samples");
    check_load_file("/translations");

    log_debug("language: " + settings_general_language.toStdString());
    if (settings_general_language == "system") {
        settings_general_language = QLocale_system().languageToString(QLocale_system().language()).toLocal8Bit().toLower();
    }

    //Selectors
    layerSelector      = int(this);
    colorSelector      = int(this);
    linetypeSelector   = int(this);
    lineweightSelector = int(this);
    textFontSelector   = QFontComboBox(this);
    textSizeSelector   = int(this);

    numOfDocs = 0;
    docIndex = 0;

    shiftKeyPressedState = false;

    setWindowIcon(load_icon("app"));
    setMinimumSize(800, 480); //Require Minimum WVGA

    loadFormats();

    //create the mdiArea
    QFrame* vbox = QFrame(this);
    QVBoxLayout* layout = QVBoxLayout(vbox);
    // layout->setMargin(0);
    vbox->setFrameStyle(QFrame_StyledPanel | QFrame_Sunken);
    mdiArea = MdiArea(vbox);
    active_view->useBackgroundLogo(settings_general_mdi_bg_use_logo);
    active_view->useBackgroundTexture(settings_general_mdi_bg_use_texture);
    active_view->useBackgroundColor(settings_general_mdi_bg_use_color);
    active_view->setBackgroundLogo(settings_general_mdi_bg_logo);
    active_view->setBackgroundTexture(settings_general_mdi_bg_texture);
    active_view->setBackgroundColor(EmbColor(settings_general_mdi_bg_color));
    active_view->set_viewMode(QTabbedEmbView);
    active_view->setHorizontalscrollbarPolicy(scrollbarAsNeeded);
    active_view->setVerticalscrollbarPolicy(scrollbarAsNeeded);
    active_view->setActivationOrder(QActivationHistoryOrder);
    layout->add_widget(mdiArea);
    setCentralWidget(vbox);

    //create the Command Prompt
    prompt = CmdPrompt(this);
    prompt->setFocus(OtherFocusReason);
 this->setFocusProxy(prompt);
    active_view->setFocusProxy(prompt);

    prompt->setPromptTextColor(EmbColor(settings_prompt_text_color));
    prompt->setPromptBackgroundColor(EmbColor(settings_prompt_bg_color));

    connect(prompt, startCommand(const char *), logPromptInput(const char *)));

    connect(prompt, startCommand(const char *), runCommandMain(const char *)));
    connect(prompt, runCommand(const char *, const char *), runCommandPrompt(const char *, const char *)));

    connect(prompt, deletePressed(), deletePressed()));
    //\todo connect(prompt, tabPressed(), someUnknownSlot()));
    connect(prompt, escapePressed(), escapePressed()));
    connect(prompt, upPressed(), promptInputPrevious()));
    connect(prompt, downPressed(), promptInputNext()));
    connect(prompt, F1Pressed(), help()));
    //\todo connect(prompt, F2Pressed(), floatHistory()));
    //\todo connect(prompt, F3Pressed(), toggleQSNAP()));
    connect(prompt, F4Pressed(), toggleLwt())); //\todo typically this is toggleTablet(), make F-Keys customizable thru settings
    //\todo connect(prompt, F5Pressed(), toggleISO()));
    //\todo connect(prompt, F6Pressed(), toggleCoords()));
    connect(prompt, F7Pressed(), toggleGrid()));
    //\todo connect(prompt, F8Pressed(), toggleORTHO()));
    //\todo connect(prompt, F9Pressed(), toggleSNAP()));
    //\todo connect(prompt, F10Pressed(), togglePOLAR()));
    //\todo connect(prompt, F11Pressed(), toggleQTRACK()));
    connect(prompt, F12Pressed(), toggleRuler()));
    connect(prompt, cutPressed(), cut()));
    connect(prompt, copyPressed(), copy()));
    connect(prompt, pastePressed(), paste()));
    connect(prompt, selectAllPressed(), selectAll()));
    connect(prompt, undoPressed(), undo()));
    connect(prompt, redoPressed(), redo()));

    connect(prompt, shiftPressed(), setShiftPressed()));
    connect(prompt, shiftReleased(), setShiftReleased()));

    connect(prompt, showSettings(), settingsPrompt()));

    connect(prompt, historyAppended(const char *), promptHistoryAppended(const char *)));

    //create the Object Property Editor
    char * s = appDir + "/icons/" + settings_general_icon_theme;
    dockPropEdit = PropertyEditor(s, settings_selection_mode_pickadd, prompt, this);
    addDockWidget(LeftDockWidgetArea, dockPropEdit);
    connect(dockPropEdit, pickAddModeToggled(), pickAddModeToggled()));

    //create the Command History Undo Editor
    dockUndoEdit = UndoEditor(s, prompt, this);
    addDockWidget(LeftDockWidgetArea, dockUndoEdit);

    //setDockOptions(QAnimatedDocks | QAllowTabbedDocks | QVerticalTabs); //\todo Load these from settings
    //tabifyDockWidget(dockPropEdit, dockUndoEdit); //\todo load this from settings

    statusbar = StatusBar(this);
 this->setStatusBar(statusbar);

    createAllActions();
    createAllMenus();
    createAllToolbars();

    iconResize(settings_general_icon_size);
    updateMenuToolbarStatusbar();

    //Show date in statusbar after it has been updated
    QDate date = QDate_currentDate();
    char * datestr = date.toString("MMMM d, yyyy");
    statusbar_showMessage(datestr);

    showNormal();

    //Load tips from external file
    std_vector<char *> tips;

    QFile tipFile(appDir + "/docs/tips.txt");
    if (tipFile.open(QFile_ReadOnly)) {
        QTextStream stream(&tipFile);
        char * tipLine;
        do {
            tipLine = stream.readLine();
            if (!tipLine.isEmpty())
                listTipOfTheDay << tipLine;
        }
        while(!tipLine.isNull());
    }
    if (settings_general_tip_of_the_day) {
        tipOfTheDay();
    }
}

/**
 * @brief
 *
 *
 * \todo Prevent memory leaks by deleting any unpasted objects.
 */
void
MainWindow(void)
{
    log_debug("Destructor()");
    delete_all(cutCopyObjectList.begin(), cutCopyObjectList.end());
    cutCopyObjectList.clear();
}

void
recentMenuAboutToShow(void)
{
    log_debug("recentMenuAboutToShow()");
    recentMenu->clear();

    QFileInfo recentFileInfo;
    char * recentValue;
    for (int i = 0; i < settings_opensave_recent_list_of_files.size(); ++i) {
        //If less than the max amount of entries add to menu
        if (i < settings_opensave_recent_max_files) {
            recentFileInfo = QFileInfo(settings_opensave_recent_list_of_files.at(i));
            if (recentFileInfo.exists() && validFileFormat(recentFileInfo.fileName())) {
                recentValue.setNum(i+1);
                QAction* rAction;
                if     (recentValue.toInt() >= 1 && recentValue.toInt() <= 9) rAction = QAction("&" + recentValue + " " + recentFileInfo.fileName(), this);
                else if (recentValue.toInt() == 10)                            rAction = QAction("1&0 "                  + recentFileInfo.fileName(), this);
                else                                                          rAction = QAction(      recentValue + " " + recentFileInfo.fileName(), this);
                rAction->setCheckable(false);
                rAction->setData(settings_opensave_recent_list_of_files.at(i));
                recentMenu->addAction(rAction);
                connect(rAction, triggered(), openrecentfile()));
            }
        }
    }
    //Ensure the list only has max amount of entries
    while(settings_opensave_recent_list_of_files.size() > settings_opensave_recent_max_files) {
        settings_opensave_recent_list_of_files.removeLast();
    }
}

void
window_menu_about_to_show(void)
{
    log_debug("windowMenuAboutToShow()");
    std_vector<SubEmbView*> windows = active_view->subWindowList();
    for (int i = 0; i < windows.count(); ++i) {
        QAction* aAction = QAction(windows.at(i)->windowTitle(), this);
        aAction->setCheckable(true);
        aAction->setData(i);
        windowMenu->addAction(aAction);
        connect(aAction, toggled(bool), windowMenuActivated(bool)));
        aAction_set_checked(view->activeSubWindow() == windows.at(i));
    }
}

void
window_menu_activated(bool checked)
{
    log_debug("windowMenuActivated()");
    printf("windowMenuActivated %d\n", checked);
    QAction* aSender = <QAction*>(sender());
    if (!aSender)
        return;
    QWidget* w = active_view->subWindowList().at(aSender->data().toInt());
    if (w && checked)
        w->setFocus();
}

void
open_recent_file(void)
{
    log_debug("open_recent_file()");

    // Check to see if this from the recent files list
    QAction* recentSender = <QAction*>(sender());
    if (recentSender) {
        openFile(true, recentSender->data().toString());
    }
}

int
find_view(const char *fileName)
{
    printf("find_view: %s\n", fileName);
    log_debug("findEmbView(" + fileName.toStdString() + ")");
    char * canonicalFilePath = QFileInfo(fileName).canonicalFilePath();

    for (SubEmbView* subWindow, active_view->subWindowList()) {
        if (subWindow) {
            if (subWindow->curFile == canonicalFilePath) {
                return subWindow;
            }
        }
    }
    return 0;
}

void
resize_event(void)
{
    log_debug("resizeEvent()");
    QresizeEvent(e);
    statusBar()->setSizeGripEnabled(!isMaximized());
}

void
update_menu_toolbar_statusbar(void)
{
    log_debug("updateMenuToolbarStatusbar()");

    actionHash.value(ACTION_print)->setEnabled(numOfDocs > 0);
    actionHash.value(ACTION_windowclose)->setEnabled(numOfDocs > 0);
    actionHash.value(ACTION_designdetails)->setEnabled(numOfDocs > 0);

    if (numOfDocs) {
        //Toolbars
        toolbarEmbView->show();
        toolbarZoom->show();
        toolbarPan->show();
        toolbarIcon->show();
        toolbarHelp->show();
        toolbarLayer->show();
        toolbarText->show();
        toolbarProperties->show();
        toolbarPrompt->show();

        for (QToolBar* tb, toolbarHash) {
            tb->show();
        }

        //DockWidgets
        dockPropEdit->show();
        dockUndoEdit->show();

        //Menus
        menuBar()->clear();
        menuBar()->addMenu(fileMenu);
        menuBar()->addMenu(editMenu);
        menuBar()->addMenu(viewMenu);

        for (QMenu* menu, menuHash) {
            menuBar()->addMenu(menu);
        }

        menuBar()->addMenu(settingsMenu);
        menuBar()->addMenu(windowMenu);
        menuBar()->addMenu(helpMenu);

        windowMenu->setEnabled(true);

        //Statusbar
        statusbar_clearMessage();
        statusbarMouseCoord->show();
        statusbarSnapButton->show();
        statusbarGridButton->show();
        statusbarRulerButton->show();
        statusbarOrthoButton->show();
        statusbarPolarButton->show();
        statusbarQSnapButton->show();
        statusbarQTrackButton->show();
        statusbarLwtButton->show();
    }
    else {
        //Toolbars
        toolbarEmbView->hide();
        toolbarZoom->hide();
        toolbarPan->hide();
        toolbarIcon->hide();
        toolbarHelp->hide();
        toolbarLayer->hide();
        toolbarText->hide();
        toolbarProperties->hide();
        toolbarPrompt->hide();
        for (QToolBar* tb : toolbarHash) {
            tb->hide();
        }

        //DockWidgets
        dockPropEdit->hide();
        dockUndoEdit->hide();

        //Menus
        menuBar()->clear();
        menuBar()->addMenu(fileMenu);
        menuBar()->addMenu(editMenu);
        menuBar()->addMenu(settingsMenu);
        menuBar()->addMenu(windowMenu);
        menuBar()->addMenu(helpMenu);

        windowMenu->setEnabled(false);

        //Statusbar
        statusbar_clearMessage();
        statusbarMouseCoord->hide();
        statusbarSnapButton->hide();
        statusbarGridButton->hide();
        statusbarRulerButton->hide();
        statusbarOrthoButton->hide();
        statusbarPolarButton->hide();
        statusbarQSnapButton->hide();
        statusbarQTrackButton->hide();
        statusbarLwtButton->hide();
    }
}

void
load_formats(void)
{
    char stable, unstable;
    char * supportedReaders  = "All Supported Files (";
    char * individualReaders = "All Files (*);;";
    char * supportedWriters  = "All Supported Files (";
    char * individualWriters = "All Files (*);;";
    char * supportedStr;
    char * individualStr;

    //\todo Stable Only (Settings Option)
    //stable = 'S'; unstable = 'S';

    //Stable + Unstable
    stable = 'S'; unstable = 'U';

    const char* extension = 0;
    const char* description = 0;
    char readerState;
    char writerState;

    EmbFormatList* curFormat = 0;
    for (int i=0; i < numberOfFormats; i++) {
        extension = formatTable[i].extension;
        description = formatTable[i].description;
        readerState = formatTable[i].reader_state;
        writerState = formatTable[i].writer_state;

        char * upperExt = char *(extension).toUpper();
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

    //\todo Fixup custom filter
    char * custom = getSettingsCustomFilter();
    if (custom.contains("supported", CaseInsensitive))
        custom = ""; //This will hide it
    else if (!custom.contains("*", CaseInsensitive))
        custom = ""; //This will hide it
    else
        custom = "Custom Filter(" + custom + ");;";

    return translate(custom + supported + all));
}

void
close_toolbar_action(int* action)
{
    if (action->objectName() == "toolbarclose") {
        QToolBar* tb = <QToolBar*>(sender());
        if (tb) {
            log_debug("%s closed.", tb->objectName()));
            tb->hide();
        }
    }
}

void
floating_changed_toolbar_action(bool isFloating)
{
    QToolBar* tb = <QToolBar*>(sender());
    if (tb) {
        if (isFloating) {
            //\todo Determine best suited close button on various platforms.
            QStyle_SP_DockWidgetCloseButton
            QStyle_SP_TitleBarCloseButton
            QStyle_SP_DialogCloseButton
            QAction *ACTION = QAction(tb->style()->standardIcon(QStyle_SP_DialogCloseButton), "Close", this);
            ACTION->setStatusTip("Close the " + tb->windowTitle() + " Toolbar");
            ACTION->setObjectName("toolbarclose");
            tb->addAction(ACTION);
            connect(tb, actionTriggered(QAction*), closeToolBar(QAction*)));
        }
        else {
            std_vector<QAction*> actList = tb->actions();
            for (int i = 0; i < actList.size(); ++i) {
                QAction* ACTION = actList.value(i);
                if (ACTION->objectName() == "toolbarclose") {
                    tb->removeAction(ACTION);
                    disconnect(tb, actionTriggered(QAction*), closeToolBar(QAction*)));
                    delete ACTION;
                }
            }
        }
    }
}

void
undo_editor_init(char *iconDirectory)
{
    iconDir = iconDirectory;
    iconSize = 16;
    setMinimumSize(100,100);

    undoGroup = QUndoGroup(this);
    undoEmbView = QUndoEmbView(undoGroup, this);
    updateCleanIcon(false);

    setWidget(undoEmbView);
    setWindowTitle(translate("History"));
    setAllowedAreas(LeftDockWidgetArea | RightDockWidgetArea);

    this->setFocusProxy(widgetToFocus);
    undoEmbView->setFocusProxy(widgetToFocus);
}

void
update_clean_icon(bool opened)
{
    if (opened) {
            undoEmbView->setEmptyLabel(translate("Open"));
        undoEmbView->setCleanIcon(QIcon(iconDir + "/" + "open" + ".png"));
        }
    else {
            undoEmbView->setEmptyLabel(translate("New"));
        undoEmbView->setCleanIcon(QIcon(iconDir + "/" + "new" + ".png"));
        }
}

/**
 * @brief
 *
 * Actions that were Qt internals were here.
 * addStack, canUndo, canRedo, undoText, redoText, undo, redo
 *
 * \a point
 * \a rot
 */
void
rotate(EmbVector point, EmbReal rot)
{
    printf("rotate, %f %f %lf\n", point.x, point.y, rot);
    EmbReal alpha = radians(rot);
    EmbVector p = embVector_subtract(object->scenePos(), point);
    EmbVector rot = embVector_rotate(p, alpha);
    rot = embVector(rot, point);

    object->setPos(rot);
    object->setRotation(object->rotation() + alpha);
}

void
scale_action(
    EmbVector position,
    EmbReal scaleFactor,
    char *text,
    EmbGeometry* obj,
    EmbView* v)
{
    gview = v;
    object = obj;
    setText(text);

    // Prevent division by zero and other wacky behavior
    if (scaleFactor <= 0.0) {
        delta.x = 0.0;
        delta.y = 0.0;
        factor = 1.0;
        critical_message(0, translate("ScaleFactor Error"),
                        translate("Hi there. If you are not a developer, report this as a bug. "
                        "If you are a developer, your code needs examined, and possibly your head too."));
    }
    else {
        //Calculate the offset
        EmbLine scaleLine(position, object->position);
        scaleLine.setLength(scaleLine.length()*scaleFactor);

        delta = embVector_subtract(scaleLine.end, object->position.x);
        factor = scaleFactor;
    }
}

void
scale_undo(void)
{
    object->setScale(object->scale()*(1/factor));
    object->moveBy(-dx, -dy);
}

void
scale_redo(void)
{
    object->setScale(object->scale()*factor);
    object->moveBy(dx, dy);
}

void
undoable_navigation_command(char * type, EmbView* v)
{
    gview = v;
    navType = type;
    setText(translate("Navigation"));
    done = false;
    fromTransform = gview->transform();
    fromCenter = gview->center();
}

/**
 * @brief
 *
 * Returns Boolean value representing the success or failure of the function.
 */
bool
merge_with(void) // (QUndoCommand* newest)
{
    if (newest->id() != id()) {
        // make sure other is also an UndoableNavCommand
         return false;
    }

    UndoableNavCommand* cmd = static_cast<const UndoableNavCommand*>(newest);
    toTransform = cmd->toTransform;
    toCenter = cmd->toCenter;

    return true;
}

void
undo_navigation(void)
{
    if (!done) {
        toTransform = gview->transform();
        toCenter = gview->center();
    }
    done = true;

    gview->setTransform(fromTransform);
    gview->centerAt(fromCenter);
}

void
redo_navigation(void)
{
    if (!done) {
        if (navType == "ZoomInToPoint")  {
            gview->zoomToPoint(
                gview->scene()->property("VIEW_MOUSE_POINT").toPoint(),
                +1);
        }
        else if (navType == "ZoomOutToPoint") {
            gview->zoomToPoint(
                gview->scene()->property("VIEW_MOUSE_POINT").toPoint(),
                -1);
        }
        else if (navType == "ZoomExtents") {
            actuator("zoom extents");
        }
        else if (navType == "ZoomSelected") {
            actuator("zoom selected");
        }
        else if (navType == "PanStart") {
            // Do Nothing. We are just recording the spot where the pan started.
        }
        else if (navType == "PanStop") {
            // Do Nothing. We are just recording the spot where the pan stopped.
        }
        else if (navType == "pan left") {
            actuator("pan left");
        }
        else if (navType == "pan right") {
            actuator("pan right");
        }
        else if (navType == "pan up") {
            actuator("pan up");
        }
        else if (navType == "pan down") {
            actuator("pan down");
        }
        toTransform = gview->transform();
        toCenter = gview->center();
    }
    else {
        gview->setTransform(toTransform);
        gview->centerAt(toCenter);
    }
}

void
embCircle_click(UiObject *global, EmbVector position)
{
    if (n_views == 0) {
        return;
    }

    switch (global->mode) {
    case CIRCLE_MODE_1P_RAD: {
            if (isnan(global.x1)) {
            global.point1 = position;
            global.center = position;
            add_rubber("CIRCLE");
            setRubberMode("CIRCLE_1P_RAD");
            setRubberPoint("CIRCLE_CENTER", global.cx, global.cy);
            append_prompt_history();
            set_prompt_prefix(translate("Specify radius of circle or [Diameter]: "));
        }
        else {
            global.x2 = position.x;
            global.y2 = position.y;
            setRubberPoint("CIRCLE_RADIUS", global.x2, global.y2);
            vulcanize();
            append_prompt_history();
            end_command();
        }
            break;
    }
    case CIRCLE_MODE_1P_DIA: {
            if (isnan(global.x1)) {
            error("CIRCLE", translate("This should never happen."));
        }
        else {
            global.x2 = position.x;
            global.y2 = position.y;
            setRubberPoint("CIRCLE_DIAMETER", global.x2, global.y2);
            vulcanize();
            append_prompt_history();
            end_command();
        }
            break;
    }
    case CIRCLE_MODE_2P: {
            if (isnan(global.x1)) {
            global.x1 = position.x;
            global.y1 = position.y;
            add_rubber("CIRCLE");
            setRubberMode("CIRCLE_2P");
            setRubberPoint("CIRCLE_TAN1", global.x1, global.y1);
            append_prompt_history();
            set_prompt_prefix(translate("Specify second end point of circle's diameter: "));
        }
        else if (isnan(global.x2)) {
            global.x2 = position.x;
            global.y2 = position.y;
            setRubberPoint("CIRCLE_TAN2", global.x2, global.y2);
            vulcanize();
            append_prompt_history();
            end_command();
        }
        else {
            error("CIRCLE", translate("This should never happen."));
        }
            break;
    }
    case CIRCLE_MODE_3P: {
            if (isnan(global.x1)) {
            global.x1 = position.x;
            global.y1 = position.y;
            append_prompt_history();
            set_prompt_prefix(translate("Specify second point on circle: "));
        }
        else if (isnan(global.x2)) {
            global.x2 = position.x;
            global.y2 = position.y;
            add_rubber("CIRCLE");
            setRubberMode("CIRCLE_3P");
            setRubberPoint("CIRCLE_TAN1", global.x1, global.y1);
            setRubberPoint("CIRCLE_TAN2", global.x2, global.y2);
            append_prompt_history();
            set_prompt_prefix(translate("Specify third point on circle: "));
        }
        else if (isnan(global.x3)) {
            global.x3 = position.x;
            global.y3 = position.y;
            setRubberPoint("CIRCLE_TAN3", global.x3, global.y3);
            vulcanize();
            append_prompt_history();
            end_command();
        }
        else {
            error("CIRCLE", translate("This should never happen."));
        }
            break;
    }
    case CIRCLE_MODE_TTR: {
            if (isnan(global.x1)) {
            global.x1 = position.x;
            global.y1 = position.y;
            append_prompt_history();
            set_prompt_prefix(translate("Specify point on object for second tangent of circle: "));
        }
        else if (isnan(global.x2)) {
            global.x2 = position.x;
            global.y2 = position.y;
            append_prompt_history();
            set_prompt_prefix(translate("Specify radius of circle: "));
        }
        else if (isnan(global.x3)) {
            global.x3 = position.x;
            global.y3 = position.y;
            append_prompt_history();
            set_prompt_prefix(translate("Specify second point: "));
        }
        else {
            todo("CIRCLE", "click() for TTR");
        }
            break;
    }
    default:
        break;
    }
}

void
embCircle_prompt(UiObject *global, const char *str)
{
    if (n_views == 0) {
        return;
    }

    printf("%s", str);

    if (global->mode == CIRCLE_MODE_1P_RAD) {
            if (isnan(global.x1)) {
            if (streq(str, "2P") {
                //\todo Probably should add additional translate calls here.
                global->mode = "CIRCLE_MODE_2P";
                set_prompt_prefix(translate("Specify first end point of circle's diameter: "));
            }
            else if (streq(str, "3P") {
                global->mode = "CIRCLE_MODE_3P";
                set_prompt_prefix(translate("Specify first point of circle: "));
            }
            else if (streq(str, "T" || streq(str, "TTR")  {
                global->mode = "CIRCLE_MODE_TTR";
                set_prompt_prefix(translate("Specify point on object for first tangent of circle: "));
            }
            else {
                std_vector<char *> strList = str.split(",");
                if (isnan(strList[0]) || isnan(strList[1])) {
                    alert(translate("Point or option keyword required."));
                    set_prompt_prefix(translate("Specify center point for circle or [3P/2P/Ttr (tan tan radius)]: "));
                }
                else {
                    global.x1 = Number(strList[0]);
                    global.y1 = Number(strList[1]);
                    global.cx = global.x1;
                    global.cy = global.y1;
                    add_rubber("CIRCLE");
                    setRubberMode("CIRCLE_1P_RAD");
                    setRubberPoint("CIRCLE_CENTER", global.cx, global.cy);
                    set_prompt_prefix(translate("Specify radius of circle or [Diameter]: "));
                }
            }
        }
        else {
            if (streq(str, "D" || streq(str, "DIAMETER") {
                //\todo Probably should add additional translate calls here.
                global->mode = "CIRCLE_MODE_1P_DIA";
                setRubberMode("CIRCLE_1P_DIA");
                set_prompt_prefix(translate("Specify diameter of circle: "));
            }
            else {
                float num = Number(str);
                if (isnan(num)) {
                    alert(translate("Requires numeric radius, point on circumference, or \"D\"."));
                    set_prompt_prefix(translate("Specify radius of circle or [Diameter]: "));
                }
                else {
                    global.rad = num;
                    global.x2 = global.x1 + global.rad;
                    global.y2 = global.y1;
                    setRubberPoint("CIRCLE_RADIUS", global.x2, global.y2);
                    vulcanize();
                    end_command();
                }
            }
        }
    }
    case MODE_1P_DIA) {
        if (isnan(global.x1)) {
            error("CIRCLE", translate("This should never happen."));
        }
        if (isnan(global.x2)) {
            float num = Number(str);
            if (isnan(num)) {
                alert(translate("Requires numeric distance or second point."));
                set_prompt_prefix(translate("Specify diameter of circle: "));
            }
            else {
                global.dia = num;
                global.x2 = global.x1 + global.dia;
                global.y2 = global.y1;
                setRubberPoint("CIRCLE_DIAMETER", global.x2, global.y2);
                vulcanize();
                end_command();
            }
        }
        else {
            error("CIRCLE", translate("This should never happen."));
        }
    }
    case MODE_2P) {
        if (isnan(global.x1)) {
            std_vector<char *> strList = str.split(",");
            if (isnan(strList[0]) || isnan(strList[1])) {
                alert(translate("Invalid point."));
                set_prompt_prefix(translate("Specify first end point of circle's diameter: "));
            }
            else {
                global.x1 = Number(strList[0]);
                global.y1 = Number(strList[1]);
                add_rubber("CIRCLE");
                setRubberMode("CIRCLE_2P");
                setRubberPoint("CIRCLE_TAN1", global.x1, global.y1);
                set_prompt_prefix(translate("Specify second end point of circle's diameter: "));
            }
        }
        else if (isnan(global.x2)) {
            std_vector<char *> strList = str.split(",");
            if (isnan(strList[0]) || isnan(strList[1])) {
                alert(translate("Invalid point."));
                set_prompt_prefix(translate("Specify second end point of circle's diameter: "));
            }
            else {
                global.x2 = Number(strList[0]);
                global.y2 = Number(strList[1]);
                setRubberPoint("CIRCLE_TAN2", global.x2, global.y2);
                vulcanize();
                end_command();
            }
        }
        else {
            error("CIRCLE", translate("This should never happen."));
        }
    }
    case MODE_3P) {
        if (isnan(global.x1)) {
            std_vector<char *> strList = str.split(",");
            if (isnan(strList[0]) || isnan(strList[1])) {
                alert(translate("Invalid point."));
                set_prompt_prefix(translate("Specify first point of circle: "));
            }
            else {
                global.x1 = Number(strList[0]);
                global.y1 = Number(strList[1]);
                set_prompt_prefix(translate("Specify second point of circle: "));
            }
        }
        else if (isnan(global.x2)) {
            std_vector<char *> strList = str.split(",");
            if (isnan(strList[0]) || isnan(strList[1])) {
                alert(translate("Invalid point."));
                set_prompt_prefix(translate("Specify second point of circle: "));
            }
            else {
                global.x2 = Number(strList[0]);
                global.y2 = Number(strList[1]);
                add_rubber("CIRCLE");
                setRubberMode("CIRCLE_3P");
                setRubberPoint("CIRCLE_TAN1", global.x1, global.y1);
                setRubberPoint("CIRCLE_TAN2", global.x2, global.y2);
                set_prompt_prefix(translate("Specify third point of circle: "));
            }
        }
        else if (isnan(global.x3)) {
            std_vector<char *> strList = str.split(",");
            if (isnan(strList[0]) || isnan(strList[1])) {
                alert(translate("Invalid point."));
                set_prompt_prefix(translate("Specify third point of circle: "));
            }
            else {
                global.x3 = Number(strList[0]);
                global.y3 = Number(strList[1]);
                setRubberPoint("CIRCLE_TAN3", global.x3, global.y3);
                vulcanize();
                end_command();
            }
        }
        else {
            error("CIRCLE", translate("This should never happen."));
        }

    }
    case MODE_TTR) {
        todo("CIRCLE", "prompt() for TTR");
    }
}

/**
 * @brief
 *
 * \todo Before saving to a stitch only format, Embroidermodder needs
 * to calculate the optimal path to minimize jump stitches. Also
 * based upon which layer needs to be stitched first,
 * the path to the next object needs to be hidden beneath fills
 * that will come later. When finding the optimal path, we need
 * to take into account the color of the thread, as we do not want
 * to try to hide dark colored stitches beneath light colored fills.
 *
 * \a fileName
 * Returns true
 * Returns false
 */
bool
save_save(const char *fileName)
{
    char debug_msg[200];
    sprintf(debug_msg, "save %s", fileName);
    log_debug(debug_msg);
    printf("%s", fileName);
    bool writeSuccessful = false;
    int i;

    int formatType = emb_identify_format(fileName);
    if (formatTable[formatType].type == EMBFORMAT_UNSUPPORTED) {
        sprintf(debug_msg, "Unsupported write file type: %s", fileName);
        log_debug(debug_msg);
        return false;
    }

    EmbPattern* pattern = embPattern_create();
    if (!pattern) {
        log_debug("Could not allocate memory for embroidery pattern");
        return false;
    }

    /**
     * \todo handle EMBFORMAT_STCHANDOBJ also
     if (formatTable[formatType].type == EMBFORMAT_STITCHONLY) {
        embPattern_movePolylinesToStitchList(pattern);
            /**
          * \todo handle all objects like this
              }

    const char *s = "output";
    s += formatTable[formatType].extension;

    writeSuccessful = embPattern_write(pattern, s), formatType);
    if (!writeSuccessful) {
        debug_message("Writing file %s was unsuccessful", fileName));
    }

    /**
     * \todo check the embLog for errors and if any exist, report them.
     embPattern_free(pattern);
    return writeSuccessful;
}

/**
 * @brief Old conversion method for saving to non-design formats: move to libembroidery.
 *
 * NOTE: This function should be used to interpret various
 * object types and save them as polylines for stitchOnly formats.
 *
 * \a pattern The pointer to the pattern to operate on.
 * \a objPos The position of object in the scene.
 * \a objPath The path to be converted.
 * \a layer Which layer to place the result in.
 * \a color
 * \a lineType
 * \a lineWeight
 */
void
save_toPolyline(
    EmbPattern* pattern,
    EmbVector objPos,
    EmbPath objPath,
    const char *layer,
    EmbColor color,
    const char *lineType,
    const char *lineWeight)
{
    int i;
    EmbVector start = objPos;
    EmbArray *pointList = embArray_create(EMB_POINT);
    EmbPoint lastPoint;
    EmbPath_Element element;
    for (i = 0; i < objPath.count; ++i) {
        element = objPath.elementAt(i);
        if (pointList->count == 0) {
            lastPoint.position.x = element.x + startX;
            lastPoint.position.y = -(element.y + startY);
            embArray_addPoint(pointList, lastPoint);
        }
        else {
            lastPoint.position.x += element.x + startX;
            lastPoint.position.y += -(element.y + startY);
        }
    }

    EmbPolyline polyObject;
    polyObject.pointList = pointList;
    polyObject.color = color;
    /**
     * \todo proper lineType
     //embPattern_addPolylineAbs(pattern, polyObject);
}

/**
 * @brief Initialise a Dolphin design element.
 */
UiObject
dolphin_main(void)
{
    UiObject global = generic_design_init();
    global.numPoints = 512;
    global.minPoints = 64;
    global.maxPoints = 8192;
    global.center.x = 0.0f;
    global.center.y = 0.0f;
    global.scale.x = 0.04f;
    global.scale.y = 0.04f;
    global.mode = DOLPHIN_MODE_NUM_POINTS;
    strcpy(global.path_desc, "");

    clear_selection();
    views[settings.pattern_index].ui_mode = DOLPHIN_MODE_NUM_POINTS;
    add_rubber("POLYGON");
    setRubberMode("POLYGON");
    updateDolphin(global);
    spareRubber("POLYGON");
    return global;
}

void
updateDolphin(EmbArray* path, UiObject global)
{
    float xx = 0.0f;
    float yy = 0.0f;
    float two_pi = 2*Math.PI;

    for (int i = 0; i <= numPts; i++) {
        float t = two_pi/numPts*i;

        xx = ;
        yy = ;

        setRubberPoint("POLYGON_POINT_" + i.toString(), xx*xScale, yy*yScale);
    }

    setRubberText("POLYGON_NUM_POINTS", numPts.toString());
}

UiObject
heart_init(void)
{
    UiObject global = generic_design_init();
    clear_selection();
    global.numPoints = 512; //Default //\todo min:64 max:8192
    global.scale = {1.0, 1.0};
    global.center = {0.0f, 0.0f};
    global->mode = HEART_MODE_NUM_POINTS;

    //Heart4: 10.0 / 512
    //Heart5: 1.0 / 512

    add_rubber("POLYGON");
    setRubberMode("POLYGON");
    updateHeart("HEART5", global.numPoints, global.scale);
    spareRubber("POLYGON");
    return global;
}

void
update_heart(EmbPath *path, UiObject global)
{
    EmbVector v;
    float two_pi = 2*Math.PI;

    for (int i = 0; i <= numPts; i++) {
        float t = two_pi/numPts*i;

        if (style == "HEART4") {
            v.x = cos(t)*((sin(t)*sqrt(fabs(cos(t))))/(sin(t)+7/5) - 2*sin(t) + 2);
            v.y = sin(t)*((sin(t)*sqrt(fabs(cos(t))))/(sin(t)+7/5) - 2*sin(t) + 2);
        }
        else if (style == "HEART5") {
            v.x = 16*Math.pow(sin(t), 3);
            v.y = 13*cos(t) - 5*cos(2*t) - 2*cos(3*t) - cos(4*t);
        }

        setRubberPoint("POLYGON_POINT_" + i.toString(), v.x*xScale, v.y*yScale);
    }

    setRubberText("POLYGON_NUM_POINTS", numPts.toString());
}

UiObject
path_init(void)
{
    UiObject global = generic_design_init();
    init_command();
    clear_selection();
    global.firstRun = true;
    global.first = {0.0f, 0.0f};
    global.prev = {0.0f, 0.0f};
    set_prompt_prefix(translate("Specify start point: "));
    return global;
}

/**
 * @brief Alter \a global using the supplied \a position, depending on the mode.
 */
void
path_click(UiObject *global, EmbVector position)
{
    if (global->firstRun) {
            global->firstRun = false;
        global->first = position;
        global->prev = position;
        addPath(position);
        append_prompt_history();
            set_prompt_prefix(translate("Specify next point or [Arc/Undo]: "));
    }
    else {
            append_prompt_history();
        appendLineToPath(position);
        global.prev = position;
        }
}

void
path_prompt(const char *str)
{
    if (streq(str, "A" || streq(str, "ARC") {
        //\todo Probably should add additional translate calls here.
        \todo PATH prompt() for ARC
    }
    else if (streq(str, "U" || streq(str, "UNDO") {
        //\todo Probably should add additional translate calls here.
        \todo "PATH prompt() for UNDO
    }
    else {
        std_vector<char *> strList = str.split(",");
        if (isnan(strList[0]) || isnan(strList[1])) {
            alert(translate("Point or option keyword required."));
            set_prompt_prefix(translate("Specify next point or [Arc/Undo]: "));
        }
        else {
            float x = Number(strList[0]);
            float y = Number(strList[1]);
            if (global.firstRun) {
                global.firstRun = false;
                global.firstX = x;
                global.firstY = y;
                global.prevX = x;
                global.prevY = y;
                addPath(x,y);
                set_prompt_prefix(translate("Specify next point or [Arc/Undo]: "));
            }
            else {
                appendLineToPath(x,y);
                global.prevX = x;
                global.prevY = y;
            }
        }
    }
}

UiObject
point_init(void)
{
    UiObject global = generic_design_init();
    init_command();
    clear_selection();

    global.firstRun = true;
    set_prompt_prefix("\todo Current point settings: PDMODE=?  PDSIZE=?"); //\todo translate needed here when complete
    append_prompt_history();
    set_prompt_prefix(translate("Specify first point: "));
    return global;
}

void
point_click(UiObject global, EmbVector position)
{
    if (global.firstRun) {
            global.firstRun = false;
        append_prompt_history();
        set_prompt_prefix(translate("Specify next point: "));
        addPoint(x,y);
        }
    else {
            append_prompt_history();
        addPoint(x,y);
        }
}

void
point_prompt(const char *str)
{
    if (global.firstRun) {
        if (streq(str, "M" || streq(str, "MODE") {
            todo("POINT", "prompt() for PDMODE");
        }
        else if (streq(str, "S" || streq(str, "SIZE") {
            todo("POINT", "prompt() for PDSIZE");
        }
        std_vector<char *> strList = str.split(",");
        if (isnan(strList[0]) || isnan(strList[1])) {
            alert(translate("Invalid point."));
            set_prompt_prefix(translate("Specify first point: "));
        }
        else {
            global.firstRun = false;
            float x = Number(strList[0]);
            float y = Number(strList[1]);
            set_prompt_prefix(translate("Specify next point: "));
            addPoint(x,y);
        }
    }
    else {
        std_vector<char *> strList = str.split(",");
        if (isnan(strList[0]) || isnan(strList[1])) {
            alert(translate("Invalid point."));
            set_prompt_prefix(translate("Specify next point: "));
        }
        else {
            float x = Number(strList[0]);
            float y = Number(strList[1]);
            set_prompt_prefix(translate("Specify next point: "));
            addPoint(x,y);
        }
    }
}

/**
 * @brief Command: Polygon
 *
float global = {}; //Required
global.centerX;
global.centerY;
global.sideX1;
global.sideY1;
global.sideX2;
global.sideY2;
global.pointIX;
global.pointIY;
global.pointCX;
global.pointCY;
global.polyType = "Inscribed"; //Default
global.numSides = 4;           //Default
global->mode;

//enums
MODE_NUM_SIDES    = 0;
MODE_CENTER_PT    = 1;
MODE_POLYTYPE     = 2;
MODE_INSCRIBE     = 3;
MODE_CIRCUMSCRIBE = 4;
MODE_DISTANCE     = 5;
MODE_SIDE_LEN     = 6;
 */
void
polygon_main(void)
{
    init_command();
    clear_selection();
    global.centerX = 0.0f;
    global.centerY = 0.0f;
    global.sideX1  = 0.0f;
    global.sideY1  = 0.0f;
    global.sideX2  = 0.0f;
    global.sideY2  = 0.0f;
    global.pointIX = 0.0f;
    global.pointIY = 0.0f;
    global.pointCX = 0.0f;
    global.pointCY = 0.0f;
    global->mode = MODE_NUM_SIDES;
    set_prompt_prefix(translate("Enter number of sides") + " {" + global.numSides.toString() + "}: ");
}

void
polygon_click(float x, float y)
{
    if (global->mode == MODE_NUM_SIDES)
    {
        //Do nothing, the prompt controls this.
    }
    case MODE_CENTER_PT)
    {
        global.centerX = x;
        global.centerY = y;
        global->mode = MODE_POLYTYPE;
        append_prompt_history();
        set_prompt_prefix(translate("Specify polygon type [Inscribed in circle/Circumscribed around circle]") + " {" + global.polyType + "}: ");
    }
    case MODE_POLYTYPE) {
        //Do nothing, the prompt controls this.
    }
    case MODE_INSCRIBE) {
        global.pointIX = x;
        global.pointIY = y;
        setRubberPoint("POLYGON_INSCRIBE_POINT", global.pointIX, global.pointIY);
        vulcanize();
        append_prompt_history();
        end_command();
    }
    case MODE_CIRCUMSCRIBE) {
        global.pointCX = x;
        global.pointCY = y;
        setRubberPoint("POLYGON_CIRCUMSCRIBE_POINT", global.pointCX, global.pointCY);
        vulcanize();
        append_prompt_history();
        end_command();
    }
    case MODE_DISTANCE) {
        //Do nothing, the prompt controls this.
    }
    case MODE_SIDE_LEN) {
        todo("POLYGON", "Sidelength mode");
    }
}

void
polygon_prompt(const char *str)
{
    if (global->mode == MODE_NUM_SIDES) {
        if (streq(str, "" && global.numSides >= 3 && global.numSides <= 1024) {
            set_prompt_prefix(translate("Specify center point or [Sidelength]: "));
            global->mode = MODE_CENTER_PT;
        }
        else {
            float tmp = Number(str);
            if (isnan(tmp) || !isInt(tmp) || tmp < 3 || tmp > 1024) {
                alert(translate("Requires an integer between 3 and 1024."));
                set_prompt_prefix(translate("Enter number of sides") + " {" + global.numSides.toString() + "}: ");
            }
            else {
                global.numSides = tmp;
                set_prompt_prefix(translate("Specify center point or [Sidelength]: "));
                global->mode = MODE_CENTER_PT;
            }
        }
    }
    case MODE_CENTER_PT) {
        if (streq(str, "S" || streq(str, "SIDELENGTH") {
            global->mode = MODE_SIDE_LEN;
            set_prompt_prefix(translate("Specify start point: "));
        }
        else {
            std_vector<char *> strList = str.split(",");
            if (isnan(strList[0]) || isnan(strList[1])) {
                alert(translate("Point or option keyword required."));
                set_prompt_prefix(translate("Specify center point or [Sidelength]: "));
            }
            else {
                global.centerX = Number(strList[0]);
                global.centerY = Number(strList[1]);
                global->mode = MODE_POLYTYPE;
                set_prompt_prefix(translate("Specify polygon type [Inscribed in circle/Circumscribed around circle]") + " {" + global.polyType + "}: ");
            }
        }
    }
    case MODE_POLYTYPE) {
        if (streq(str, "INSCRIBED") {
            global->mode = MODE_INSCRIBE;
            global.polyType = "Inscribed";
            set_prompt_prefix(translate("Specify polygon corner point or [Distance]: "));
            add_rubber("POLYGON");
            setRubberMode("POLYGON_INSCRIBE");
            setRubberPoint("POLYGON_CENTER", global.centerX, global.centerY);
            setRubberPoint("POLYGON_NUM_SIDES", global.numSides, 0);
        }
        else if (streq(str, "CIRCUMSCRIBED") {
            global->mode = MODE_CIRCUMSCRIBE;
            global.polyType = "Circumscribed";
            set_prompt_prefix(translate("Specify polygon side point or [Distance]: "));
            add_rubber("POLYGON");
            setRubberMode("POLYGON_CIRCUMSCRIBE");
            setRubberPoint("POLYGON_CENTER", global.centerX, global.centerY);
            setRubberPoint("POLYGON_NUM_SIDES", global.numSides, 0);
        }
        else if (streq(str, "") {
            if (global.polyType == "Inscribed") {
                global->mode = MODE_INSCRIBE;
                set_prompt_prefix(translate("Specify polygon corner point or [Distance]: "));
                add_rubber("POLYGON");
                setRubberMode("POLYGON_INSCRIBE");
                setRubberPoint("POLYGON_CENTER", global.centerX, global.centerY);
                setRubberPoint("POLYGON_NUM_SIDES", global.numSides, 0);
            }
            else if (global.polyType == "Circumscribed") {
                global->mode = MODE_CIRCUMSCRIBE;
                set_prompt_prefix(translate("Specify polygon side point or [Distance]: "));
                add_rubber("POLYGON");
                setRubberMode("POLYGON_CIRCUMSCRIBE");
                setRubberPoint("POLYGON_CENTER", global.centerX, global.centerY);
                setRubberPoint("POLYGON_NUM_SIDES", global.numSides, 0);
            }
            else {
                error("POLYGON", translate("Polygon type is not Inscribed or Circumscribed."));
            }
        }
        else {
            alert(translate("Invalid option keyword."));
            set_prompt_prefix(translate("Specify polygon type [Inscribed in circle/Circumscribed around circle]") + " {" + global.polyType + "}: ");
        }
    }
    case MODE_INSCRIBE) {
        if (streq(str, "D" || streq(str, "DISTANCE") {
            global->mode = MODE_DISTANCE;
            set_prompt_prefix(translate("Specify distance: "));
        }
        else {
            std_vector<char *> strList = str.split(",");
            if (isnan(strList[0]) || isnan(strList[1])) {
                alert(translate("Point or option keyword required."));
                set_prompt_prefix(translate("Specify polygon corner point or [Distance]: "));
            }
            else {
                global.pointIX = Number(strList[0]);
                global.pointIY = Number(strList[1]);
                setRubberPoint("POLYGON_INSCRIBE_POINT", global.pointIX, global.pointIY);
                vulcanize();
                end_command();
            }
        }
    }
    case MODE_CIRCUMSCRIBE) {
        if (streq(str, "D" || streq(str, "DISTANCE") {
            global->mode = MODE_DISTANCE;
            set_prompt_prefix(translate("Specify distance: "));
        }
        else {
            std_vector<char *> strList = str.split(",");
            if (isnan(strList[0]) || isnan(strList[1])) {
                alert(translate("Point or option keyword required."));
                set_prompt_prefix(translate("Specify polygon side point or [Distance]: "));
            }
            else {
                global.pointCX = Number(strList[0]);
                global.pointCY = Number(strList[1]);
                setRubberPoint("POLYGON_CIRCUMSCRIBE_POINT", global.pointCX, global.pointCY);
                vulcanize();
                end_command();
            }
        }
    }
    case MODE_DISTANCE) {
        if (isnan(str)) {
            alert(translate("Requires valid numeric distance."));
            set_prompt_prefix(translate("Specify distance: "));
        }
        else {
            if (global.polyType == "Inscribed") {
                global.pointIX = global.centerX;
                global.pointIY = global.centerY + Number(str);
                setRubberPoint("POLYGON_INSCRIBE_POINT", global.pointIX, global.pointIY);
                vulcanize();
                end_command();
            }
            else if (global.polyType == "Circumscribed") {
                global.pointCX = global.centerX;
                global.pointCY = global.centerY + Number(str);
                setRubberPoint("POLYGON_CIRCUMSCRIBE_POINT", global.pointCX, global.pointCY);
                vulcanize();
                end_command();
            }
            else {
                error("POLYGON", translate("Polygon type is not Inscribed or Circumscribed."));
            }
        }
    }
    case MODE_SIDE_LEN) {
        todo("POLYGON", "Sidelength mode");
    }
}

/**
 * @brief Command: Polyline
 *
float global = {}; //Required
global.firstRun;
global.firstX;
global.firstY;
global.prevX;
global.prevY;
global.num;
 */
void
embPolyline_main(void)
{
    init_command();
    clear_selection();
    global.firstRun = true;
    global.firstX = 0.0f;
    global.firstY = 0.0f;
    global.prevX = 0.0f;
    global.prevY = 0.0f;
    global.num = 0;
    set_prompt_prefix(translate("Specify first point: "));
}

void
embPolyline_click(UiObject *global, float x, float y)
{
    if (global->firstRun) {
        global->firstRun = false;
        global->controlPoints[0].x = x;
        global->controlPoints[0].y = y;
        global->controlPoints[1].x = x;
        global->controlPoints[1].y = y;
            add_rubber("POLYLINE");
        setRubberMode("POLYLINE");
        setRubberPoint("POLYLINE_POINT_0",
            global->controlPoints[0].x,
            global->controlPoints[0].y);
        append_prompt_history();
            set_prompt_prefix(translate("Specify next point or [Undo]: "));
    }
    else {
        global->numPoints++;
            setRubberPoint("POLYLINE_POINT_" + global.num.toString(), x, y);
        setRubberText("POLYLINE_NUM_POINTS", global.num.toString());
        spareRubber("POLYLINE");
        append_prompt_history();
            global->controlPoints[1].x = x;
        global->controlPoints[1].y = y;
    }
}

void
embPolyline_prompt(UiObject global, const char *str)
{
    if (global.firstRun) {
            std_vector<char *> strList = str.split(",");
        if (isnan(strList[0]) || isnan(strList[1])) {
            alert(translate("Invalid point."));
                    set_prompt_prefix(translate("Specify first point: "));
                }
        else {
                    global.firstRun = false;
                    global.firstX = Number(strList[0]);
            global.firstY = Number(strList[1]);
            global.prevX = global.firstX;
            global.prevY = global.firstY;
            add_rubber("POLYLINE");
            setRubberMode("POLYLINE");
            setRubberPoint("POLYLINE_POINT_0", global.firstX, global.firstY);
                    set_prompt_prefix(translate("Specify next point or [Undo]: "));
                }
        }
    else {
            if (streq(str, "U" || streq(str, "UNDO") {
            todo("POLYLINE", "prompt() for UNDO");
        }
        else {
            std_vector<char *> strList = str.split(",");
            if (isnan(strList[0]) || isnan(strList[1])) {
                alert(translate("Point or option keyword required."));
                set_prompt_prefix(translate("Specify next point or [Undo]: "));
            }
            else {
                float x = Number(strList[0]);
                float y = Number(strList[1]);
                global.num++;
                setRubberPoint("POLYLINE_POINT_" + global.num.toString(), x, y);
                setRubberText("POLYLINE_NUM_POINTS", global.num.toString());
                spareRubber("POLYLINE");
                global.prevX = x;
                global.prevY = y;
                set_prompt_prefix(translate("Specify next point or [Undo]: "));
            }
        }
        }
}

/**
 * @brief Initialise command: Locate Point.
 */
UiObject
locate_point_main(void)
{
    UiObject global = generic_design_init();
    init_command();
    clear_selection();
    set_prompt_prefix(translate("Specify point: "));
    return global;
}

/**
 * @brief Process a left-click in the locate point context.
 */
void
locate_point_click(UiObject *global, EmbVector v)
{
    append_prompt_history();
    set_prompt_prefix("X = " + x.toString() + ", Y = " + y.toString());
    append_prompt_history();
    end_command();
}

void
located_point_prompt(UiObject *global, const char *str)
{
    std_vector<char *> strList = str.split(",");
    if (isnan(strList[0]) || isnan(strList[1])) {
        alert(translate("Invalid point."));
        set_prompt_prefix(translate("Specify point: "));
    }
    else {
        append_prompt_history();
        set_prompt_prefix("X = " + strList[0].toString() + ", Y = " + strList[1].toString());
        append_prompt_history();
        end_command();
    }
}

/**
 * @brief Initialise move UI event.
 *
 * \todo Prompt to select objects if nothing is preselected
 */
UiObject
move_main(void)
{
    UiObject global = generic_design_init();
    init_command();
    global.firstRun = true;
    /* base */
    global.controlPoints[0].x = 0.0f;
    global.controlPoints[0].y = 0.0f;
    /* dest */
    global.controlPoints[1].x = 0.0f;
    global.controlPoints[1].y = 0.0f;
    /* delta */
    global.controlPoints[2].x = 0.0f;
    global.controlPoints[2].y = 0.0f;
    if ( active_view->n_selected() <= 0) {
        alert(translate("Preselect objects before invoking the move command."));
        end_command();
        messageBox("information", translate("Move Preselect"), translate("Preselect objects before invoking the move command."));
    }
    else {
        set_prompt_prefix(translate("Specify base point: "));
    }
    return global;
}

void
move_click(UiObject *global, EmbVector v)
{
    if (global->firstRun) {
        global->firstRun = false;
            global->baseX = x;
        global->baseY = y;
        add_rubber("LINE");
        setRubberMode("LINE");
        setRubberPoint("LINE_START", global.baseX, global.baseY);
        previewOn("SELECTED", "MOVE", global.baseX, global.baseY, 0);
        append_prompt_history();
            set_prompt_prefix(translate("Specify destination point: "));
    }
    else {
            global->destX = x;
        global->destY = y;
        global->deltaX = global.destX - global.baseX;
        global->deltaY = global.destY - global.baseY;
        moveSelected(global.deltaX, global.deltaY);
        previewOff();
            end_command();
    }
}

void
move_prompt(UiObject *global, const char *str)
{
    if (global->firstRun) {
            std_vector<char *> strList = str.split(",");
        if (isnan(strList[0]) || isnan(strList[1])) {
            alert(translate("Invalid point."));
            set_prompt_prefix(translate("Specify base point: "));
        }
        else {
            global.firstRun = false;
            global.baseX = Number(strList[0]);
            global.baseY = Number(strList[1]);
            add_rubber("LINE");
            setRubberMode("LINE");
            setRubberPoint("LINE_START", global.baseX, global.baseY);
            previewOn("SELECTED", "MOVE", global.baseX, global.baseY, 0);
            set_prompt_prefix(translate("Specify destination point: "));
        }
        }
    else {
            std_vector<char *> strList = str.split(",");
        if (isnan(strList[0]) || isnan(strList[1])) {
            alert(translate("Invalid point."));
            set_prompt_prefix(translate("Specify destination point: "));
        }
        else {
            global.dest.x = Number(strList[0]);
            global.dest.y = Number(strList[1]);
            global.delta = embVector_subtract(global.dest, global.base);
            moveSelected(global.delta);
            previewOff();
            end_command();
        }
        }
}

/**
 * @brief .
 *
 * \todo Adding the text is not complete yet.
 *
 */
UiObject
quickleader_main(void)
{
    UiObject global = generic_design_init();
    global.firstRun = true;
    init_command();
    clear_selection();
    global.x1 = 0.0f;
    global.y1 = 0.0f;
    global.x2 = 0.0f;
    global.y2 = 0.0f;
    set_prompt_prefix(translate("Specify first point: "));
    return global;
}

void
quickleader_click(UiObject *global, EmbVector v)
{
    if (isnan(global.x1)) {
        global.x1 = x;
        global.y1 = y;
        add_rubber("DIMLEADER");
        setRubberMode("DIMLEADER_LINE");
        setRubberPoint("DIMLEADER_LINE_START", global.x1, global.y1);
        append_prompt_history();
        set_prompt_prefix(translate("Specify second point: "));
    }
    else {
        global.x2 = x;
        global.y2 = y;
        setRubberPoint("DIMLEADER_LINE_END", global.x2, global.y2);
        vulcanize();
        append_prompt_history();
        end_command();
    }
}

void
quickleader_prompt(UiObject *global, const char *str)
{
    std_vector<char *> strList = str.split(",");
    if (isnan(global.x1)) {
        if (isnan(strList[0]) || isnan(strList[1])) {
            alert(translate("Requires two points."));
            set_prompt_prefix(translate("Specify first point: "));
        }
        else {
            global.x1 = Number(strList[0]);
            global.y1 = Number(strList[1]);
            add_rubber("DIMLEADER");
            setRubberMode("DIMLEADER_LINE");
            setRubberPoint("DIMLEADER_LINE_START", global.x1, global.y1);
            set_prompt_prefix(translate("Specify second point: "));
        }
    }
    else {
        if (isnan(strList[0]) || isnan(strList[1])) {
            alert(translate("Requires two points."));
            set_prompt_prefix(translate("Specify second point: "));
        }
        else {
            global.x2 = Number(strList[0]);
            global.y2 = Number(strList[1]);
            setRubberPoint("DIMLEADER_LINE_END", global.x2, global.y2);
            vulcanize();
            end_command();
        }
    }
}

/**
 * @brief Create a rectangle object on the current pattern.
 */
UiObject
rectangle_main(void)
{
    UiObject global = generic_design_init();
    init_command();
    clear_selection();
    global.newRect = true;
    global.controlPoints[0].x = 0.0f;
    global.controlPoints[0].y = 0.0f;
    global.controlPoints[1].x = 0.0f;
    global.controlPoints[1].y = 0.0f;
    set_prompt_prefix(translate("Specify first corner point or [Chamfer/Fillet]: "));
    return global;
}

void
rectangle_click(UiObject *global, EmbVector position)
{
    if (global.newRect) {
        global.newRect = false;
        global.x1 = x;
        global.y1 = y;
        add_rubber("RECTANGLE");
        setRubberMode("RECTANGLE");
        setRubberPoint("RECTANGLE_START", x, y);
        set_prompt_prefix(translate("Specify other corner point or [Dimensions]: "));
    }
    else {
        global.newRect = true;
        global.x2 = x;
        global.y2 = y;
        setRubberPoint("RECTANGLE_END", x, y);
        vulcanize();
        end_command();
    }
}

/**
 * @brief
 */
void
rectangle_prompt(UiObject *global, const char *str)
{
    if (streq(str, "C" || streq(str, "CHAMFER") {
        todo("RECTANGLE", "prompt() for CHAMFER");
    }
    else if (streq(str, "D" || streq(str, "DIMENSIONS") {
        todo("RECTANGLE", "prompt() for DIMENSIONS");
    }
    else if (streq(str, "F" || streq(str, "FILLET") {
        todo("RECTANGLE", "prompt() for FILLET");
    }
    else {
        std_vector<char *> strList = str.split(",");
        if (isnan(strList[0]) || isnan(strList[1])) {
            alert(translate("Invalid point."));
            set_prompt_prefix(translate("Specify first point: "));
        }
        else {
            float x = Number(strList[0]);
            float y = Number(strList[1]);
            if (global.newRect) {
                global.newRect = false;
                global.x1 = x;
                global.y1 = y;
                add_rubber("RECTANGLE");
                setRubberMode("RECTANGLE");
                setRubberPoint("RECTANGLE_START", x, y);
                set_prompt_prefix(translate("Specify other corner point or [Dimensions]: "));
            }
            else {
                global.newRect = true;
                global.x2 = x;
                global.y2 = y;
                setRubberPoint("RECTANGLE_END", x, y);
                vulcanize();
                end_command();
            }
        }
    }
}

#define RGB_MODE_BACKGROUND   0
#define RGB_MODE_CROSSHAIR    1
#define RGB_MODE_GRID         2

/**
 * @brief Command: RGB (prompt only command).
 */
UiObject
rgb_main(void)
{
    UiObject global = generic_design_init();
    init_command();
    clear_selection();
    global.mode = RGB_MODE_BACKGROUND;
    set_prompt_prefix(translate("Enter RED,GREEN,BLUE values for background or [Crosshair/Grid]: "));
    return global;
}

void
rgb_prompt(const char *str)
{
    if (global->mode == MODE_BACKGROUND) {
        if (streq(str, "C" || streq(str, "CROSSHAIR") {
            //\todo Probably should add additional translate calls here.
            global->mode = MODE_CROSSHAIR;
            set_prompt_prefix(translate("Specify crosshair color: "));
        }
        else if (streq(str, "G" || streq(str, "GRID") {
            //\todo Probably should add additional translate calls here.
            global->mode = MODE_GRID;
            set_prompt_prefix(translate("Specify grid color: "));
        }
        else {
            std_vector<char *> strList = str.split(",");
            float r = Number(strList[0]);
            float g = Number(strList[1]);
            float b = Number(strList[2]);
            if (!validRGB(r,g,b)) {
                alert(translate("Invalid color. R,G,B values must be in the range of 0-255."));
                set_prompt_prefix(translate("Specify background color: "));
            }
            else {
                setBackgroundColor(r,g,b);
                end_command();
            }
        }
    }
    case MODE_CROSSHAIR) {
        std_vector<char *> strList = str.split(",");
        float r = Number(strList[0]);
        float g = Number(strList[1]);
        float b = Number(strList[2]);
        if (!validRGB(r,g,b)) {
            alert(translate("Invalid color. R,G,B values must be in the range of 0-255."));
            set_prompt_prefix(translate("Specify crosshair color: "));
        }
        else {
            setCrossHairColor(r,g,b);
            end_command();
        }
    }
    case MODE_GRID) {
        std_vector<char *> strList = str.split(",");
        float r = Number(strList[0]);
        float g = Number(strList[1]);
        float b = Number(strList[2]);
        if (!validRGB(r,g,b)) {
            alert(translate("Invalid color. R,G,B values must be in the range of 0-255."));
            set_prompt_prefix(translate("Specify grid color: "));
        }
        else {
            setGridColor(r,g,b);
            end_command();
        }
    }
}

/**
 * @brief
 *
 * \todo Prompt to select objects if nothing is preselected
 */
UiObject
rotate_main(void)
{
    UiObject global = generic_design_init();
    init_command();
    global.firstRun = true;
    global->mode = MODE_NORMAL;
    global.base.x = 0.0f;
    global.base.y = 0.0f;
    global.dest.x = 0.0f;
    global.dest.y = 0.0f;
    global.angle = 0.0f;

    global.baseR.x   = 0.0f;
    global.baseR.y   = 0.0f;
    global.destR.x   = 0.0f;
    global.destR.y   = 0.0f;
    global.angleRef = 0.0f;
    global.angleNew = 0.0f;

    if (active_view->n_selected <= 0) {
            alert(translate("Preselect objects before invoking the rotate command."));
        end_command();
        messageBox("information", translate("Rotate Preselect"), translate("Preselect objects before invoking the rotate command."));
        }
    else {
        set_prompt_prefix(translate("Specify base point: "));
    }
    return global;
}

void
rotate_click(UiObject *global, EmbVector pos)
{
    switch (global->mode) {
    case ROTATE_MODE_NORMAL: {
            if (global.firstRun) {
            global.firstRun = false;
            global.baseX = x;
            global.baseY = y;
            add_rubber("LINE");
            setRubberMode("LINE");
            setRubberPoint("LINE_START", global.baseX, global.baseY);
            previewOn("SELECTED", "ROTATE", global.baseX, global.baseY, 0);
            append_prompt_history();
            set_prompt_prefix(translate("Specify rotation angle or [Reference]: "));
        }
        else {
            global.destX = x;
            global.destY = y;
            global.angle = calculateAngle(global.baseX, global.baseY, global.destX, global.destY);
            append_prompt_history();
            rotateSelected(global.baseX, global.baseY, global.angle);
            previewOff();
            end_command();
        }
            break;
    }
    case ROTATE_MODE_REFERENCE: {
            if (isnan(global.baseR.x)) {
            global.baseR.x = x;
            global.baseR.y = y;
            append_prompt_history();
            add_rubber("LINE");
            setRubberMode("LINE");
            setRubberPoint("LINE_START", global.baseR.x, global.baseR.y);
            set_prompt_prefix(translate("Specify second point: "));
        }
        else if (isnan(global.destR.x)) {
            global.destR.x = x;
            global.destR.y = y;
            global.angleRef = calculateAngle(global.baseR.x, global.baseR.y, global.destR.x, global.destR.y);
            setRubberPoint("LINE_START", global.baseX, global.baseY);
            previewOn("SELECTED", "ROTATE", global.baseX, global.baseY, global.angleRef);
            append_prompt_history();
            set_prompt_prefix(translate("Specify the new angle: "));
        }
        else if (isnan(global.angleNew)) {
            global.angleNew = calculateAngle(global.baseX, global.baseY, x, y);
            rotateSelected(global.baseX, global.baseY, global.angleNew - global.angleRef);
            previewOff();
            end_command();
        }
            break;
    }
    default:
        break;
    }
}

void
rotate_prompt(UiObject *global, const char *str)
{
    switch (global->mode) {
    case ROTATE_MODE_NORMAL: {
        if (global->firstRun) {
                    std_vector<char *> strList = str.split(",");
            if (isnan(strList[0]) || isnan(strList[1])) {
                alert(translate("Invalid point."));
                set_prompt_prefix(translate("Specify base point: "));
            }
            else {
                global.firstRun = false;
                global.baseX = Number(strList[0]);
                global.baseY = Number(strList[1]);
                add_rubber("LINE");
                setRubberMode("LINE");
                setRubberPoint("LINE_START", global.baseX, global.baseY);
                previewOn("SELECTED", "ROTATE", global.baseX, global.baseY, 0);
                set_prompt_prefix(translate("Specify rotation angle or [Reference]: "));
            }
                }
        else {
                    if (streq(str, "R" || streq(str, "REFERENCE") {
                global->mode = MODE_REFERENCE;
                set_prompt_prefix(translate("Specify the reference angle") + " {0.00}: ");
                clearRubber();
                previewOff();
            }
            else {
                if (isnan(str)) {
                    alert(translate("Requires valid numeric angle, second point, or option keyword."));
                    set_prompt_prefix(translate("Specify rotation angle or [Reference]: "));
                }
                else {
                    global.angle = Number(str);
                    rotateSelected(global.baseX, global.baseY, global.angle);
                    previewOff();
                    end_command();
                }
            }
                }
        break;
    }
    case ROTATE_MODE_REFERENCE: {
            if (isnan(global.baseR.x)) {
            if (isnan(str)) {
                std_vector<char *> strList = str.split(",");
                if (isnan(strList[0]) || isnan(strList[1])) {
                    alert(translate("Requires valid numeric angle or two points."));
                    set_prompt_prefix(translate("Specify the reference angle") + " {0.00}: ");
                }
                else {
                    global.baseR.x = Number(strList[0]);
                    global.baseR.y = Number(strList[1]);
                    add_rubber("LINE");
                    setRubberMode("LINE");
                    setRubberPoint("LINE_START", global.baseR.x, global.baseR.y);
                    set_prompt_prefix(translate("Specify second point: "));
                }
            }
            else {
                //The base and dest values are only set here to advance the command.
                global.baseR.x = 0.0;
                global.baseR.y = 0.0;
                global.destR.x = 0.0;
                global.destR.y = 0.0;
                //The reference angle is what we will use later.
                global.angleRef = Number(str);
                add_rubber("LINE");
                setRubberMode("LINE");
                setRubberPoint("LINE_START", global.baseX, global.baseY);
                previewOn("SELECTED", "ROTATE", global.baseX, global.baseY, global.angleRef);
                set_prompt_prefix(translate("Specify the new angle: "));
            }
        }
        else if (isnan(global.destR.x)) {
            if (isnan(str)) {
                std_vector<char *> strList = str.split(",");
                if (isnan(strList[0]) || isnan(strList[1])) {
                    alert(translate("Requires valid numeric angle or two points."));
                    set_prompt_prefix(translate("Specify second point: "));
                }
                else {
                    global.destR.x = Number(strList[0]);
                    global.destR.y = Number(strList[1]);
                    global.angleRef = calculateAngle(global.baseR.x, global.baseR.y, global.destR.x, global.destR.y);
                    previewOn("SELECTED", "ROTATE", global.baseX, global.baseY, global.angleRef);
                    setRubberPoint("LINE_START", global.baseX, global.baseY);
                    set_prompt_prefix(translate("Specify the new angle: "));
                }
            }
            else {
                //The base and dest values are only set here to advance the command.
                global.baseR.x = 0.0;
                global.baseR.y = 0.0;
                global.destR.x = 0.0;
                global.destR.y = 0.0;
                //The reference angle is what we will use later.
                global.angleRef = Number(str);
                previewOn("SELECTED", "ROTATE", global.baseX, global.baseY, global.angleRef);
                set_prompt_prefix(translate("Specify the new angle: "));
            }
        }
        else if (isnan(global.angleNew)) {
            if (isnan(str)) {
                std_vector<char *> strList = str.split(",");
                if (isnan(strList[0]) || isnan(strList[1])) {
                    alert(translate("Requires valid numeric angle or second point."));
                    set_prompt_prefix(translate("Specify the new angle: "));
                }
                else {
                    float x = Number(strList[0]);
                    float y = Number(strList[1]);
                    global.angleNew = calculateAngle(global.baseX, global.baseY, x, y);
                    rotateSelected(global.baseX, global.baseY, global.angleNew - global.angleRef);
                    previewOff();
                    end_command();
                }
            }
            else {
                global.angleNew = Number(str);
                rotateSelected(global.baseX, global.baseY, global.angleNew - global.angleRef);
                previewOff();
                end_command();
            }
        }
    }
        break;
    }
    default:
        break;
    }
}


/**
 * @brief
 *
 * float global = {}; //Required
 * global.test1;
 * global.test2;
 */
UiObject
sandbox_main(void)
{
    UiObject global = generic_design_init();
    init_command();
    //Report number of pre-selected objects
    set_prompt_prefix("Number of Objects Selected: " +  active_view->n_selected().toString());
    append_prompt_history();

    mirrorSelected(0,0,0,1);

    //selectAll();
    //rotateSelected(0,0,90);

    //Polyline & Polygon Testing

    EmbVector offset = {0.0f, 0.0f};

    float polylineArray = [];
    polylineArray.push(1.0 + offsetX);
    polylineArray.push(1.0 + offsetY);
    polylineArray.push(1.0 + offsetX);
    polylineArray.push(2.0 + offsetY);
    polylineArray.push(2.0 + offsetX);
    polylineArray.push(2.0 + offsetY);
    polylineArray.push(2.0 + offsetX);
    polylineArray.push(3.0 + offsetY);
    polylineArray.push(3.0 + offsetX);
    polylineArray.push(3.0 + offsetY);
    polylineArray.push(3.0 + offsetX);
    polylineArray.push(2.0 + offsetY);
    polylineArray.push(4.0 + offsetX);
    polylineArray.push(2.0 + offsetY);
    polylineArray.push(4.0 + offsetX);
    polylineArray.push(1.0 + offsetY);
    addPolyline(polylineArray);

    offsetX = 5.0;
    offsetY = 0.0;

    float polygonArray = [];
    polygonArray.push(1.0 + offsetX);
    polygonArray.push(1.0 + offsetY);
    polygonArray.push(1.0 + offsetX);
    polygonArray.push(2.0 + offsetY);
    polygonArray.push(2.0 + offsetX);
    polygonArray.push(2.0 + offsetY);
    polygonArray.push(2.0 + offsetX);
    polygonArray.push(3.0 + offsetY);
    polygonArray.push(3.0 + offsetX);
    polygonArray.push(3.0 + offsetY);
    polygonArray.push(3.0 + offsetX);
    polygonArray.push(2.0 + offsetY);
    polygonArray.push(4.0 + offsetX);
    polygonArray.push(2.0 + offsetY);
    polygonArray.push(4.0 + offsetX);
    polygonArray.push(1.0 + offsetY);
    addPolygon(polygonArray);

    end_command();
    return global;
}

/**
 * @brief
 */
void
sandbox_click(UiObject *global, EmbVector pos)
{

}

/**
 * @brief
 */
void
sandbox_prompt(UiObject *global, const char *str)
{

}

UiObject
scale_main(void)
{
    UiObject global = generic_design_init();
    init_command();
    global->mode = MODE_NORMAL;
    global.firstRun = true;
    global.base.x = 0.0f;
    global.base.y = 0.0f;
    global.dest.x = 0.0f;
    global.dest.y = 0.0f;
    global.factor = 0.0f;

    global.baseR.x = 0.0f;
    global.baseR.y = 0.0f;
    global.destR.x = 0.0f;
    global.destR.y = 0.0f;
    global.factorRef = 0.0f;
    global.factorNew = 0.0f;

    if ( active_view->n_selected() <= 0) {
        //\todo Prompt to select objects if nothing is preselected
        alert(translate("Preselect objects before invoking the scale command."));
        end_command();
        messageBox("information", translate("Scale Preselect"), translate("Preselect objects before invoking the scale command."));
    }
    else */ {
        set_prompt_prefix(translate("Specify base point: "));
    }
    return global;
}

/**
 * @brief
 */
void
scale_click(UiObject *global, EmbVector pos)
{
    switch (global->mode) {
    case SCALE_MODE_NORMAL: {
            if (global.firstRun) {
            global.firstRun = false;
            global.baseX = x;
            global.baseY = y;
            add_rubber("LINE");
            setRubberMode("LINE");
            setRubberPoint("LINE_START", global.baseX, global.baseY);
            previewOn("SELECTED", "SCALE", global.baseX, global.baseY, 1);
            append_prompt_history();
            set_prompt_prefix(translate("Specify scale factor or [Reference]: "));
        }
        else {
            global.destX = x;
            global.destY = y;
            global.factor = calculateDistance(global.baseX, global.baseY, global.destX, global.destY);
            append_prompt_history();
            scaleSelected(global.baseX, global.baseY, global.factor);
            previewOff();
            end_command();
        }
            break;
    }
    case SCALE_MODE_REFERENCE: {
            if (isnan(global.baseR.x)) {
            global.baseR.x = x;
            global.baseR.y = y;
            append_prompt_history();
            add_rubber("LINE");
            setRubberMode("LINE");
            setRubberPoint("LINE_START", global.baseR.x, global.baseR.y);
            set_prompt_prefix(translate("Specify second point: "));
        }
        else if (isnan(global.destR.x)) {
            global.destR.x = x;
            global.destR.y = y;
            global.factorRef = calculateDistance(global.baseR.x, global.baseR.y, global.destR.x, global.destR.y);
            if (global.factorRef <= 0.0) {
                global.destR.x = 0.0f;
                global.destR.y = 0.0f;
                global.factorRef = 0.0f;
                alert(translate("Value must be positive and nonzero."));
                set_prompt_prefix(translate("Specify second point: "));
            }
            else {
                append_prompt_history();
                setRubberPoint("LINE_START", global.baseX, global.baseY);
                previewOn("SELECTED", "SCALE", global.baseX, global.baseY, global.factorRef);
                set_prompt_prefix(translate("Specify new length: "));
            }
        }
        else if (isnan(global.factorNew)) {
            global.factorNew = calculateDistance(global.baseX, global.baseY, x, y);
            if (global.factorNew <= 0.0) {
                global.factorNew = 0.0f;
                alert(translate("Value must be positive and nonzero."));
                set_prompt_prefix(translate("Specify new length: "));
            }
            else {
                append_prompt_history();
                scaleSelected(global.baseX, global.baseY, global.factorNew/global.factorRef);
                previewOff();
                end_command();
            }
        }
            break;
    }
    default:
        break;
    }
}

void
scale_prompt(UiObject *global, const char *str)
{
    switch (global->mode) {
    case SCALE_MODE_NORMAL: {
        if (global->firstRun) {
                    std_vector<char *> strList = str.split(",");
            if (isnan(strList[0]) || isnan(strList[1])) {
                alert(translate("Invalid point."));
                set_prompt_prefix(translate("Specify base point: "));
            }
            else {
                global.firstRun = false;
                global.baseX = Number(strList[0]);
                global.baseY = Number(strList[1]);
                add_rubber("LINE");
                setRubberMode("LINE");
                setRubberPoint("LINE_START", global.baseX, global.baseY);
                previewOn("SELECTED", "SCALE", global.baseX, global.baseY, 1);
                set_prompt_prefix(translate("Specify scale factor or [Reference]: "));
            }
                }
        else {
                    if (streq(str, "R" || streq(str, "REFERENCE") {
                global->mode = MODE_REFERENCE;
                set_prompt_prefix(translate("Specify reference length") + " {1}: ");
                clearRubber();
                previewOff();
            }
            else {
                if (isnan(str)) {
                    alert(translate("Requires valid numeric distance, second point, or option keyword."));
                    set_prompt_prefix(translate("Specify scale factor or [Reference]: "));
                }
                else {
                    global.factor = Number(str);
                    scaleSelected(global.baseX, global.baseY, global.factor);
                    previewOff();
                    end_command();
                }
            }
                }
        break;
    }
    case SCALE_MODE_REFERENCE: {
            if (isnan(global.baseR.x)) {
            if (isnan(str)) {
                std_vector<char *> strList = str.split(",");
                if (isnan(strList[0]) || isnan(strList[1])) {
                    alert(translate("Requires valid numeric distance or two points."));
                    set_prompt_prefix(translate("Specify reference length") + " {1}: ");
                }
                else {
                    global.baseR.x = Number(strList[0]);
                    global.baseR.y = Number(strList[1]);
                    add_rubber("LINE");
                    setRubberMode("LINE");
                    setRubberPoint("LINE_START", global.baseR.x, global.baseR.y);
                    set_prompt_prefix(translate("Specify second point: "));
                }
            }
            else {
                //The base and dest values are only set here to advance the command.
                global.baseR.x = 0.0;
                global.baseR.y = 0.0;
                global.destR.x = 0.0;
                global.destR.y = 0.0;
                //The reference length is what we will use later.
                global.factorRef = Number(str);
                if (global.factorRef <= 0.0) {
                    global.baseR.x = 0.0f;
                    global.baseR.y = 0.0f;
                    global.destR.x = 0.0f;
                    global.destR.y = 0.0f;
                    global.factorRef = 0.0f;
                    alert(translate("Value must be positive and nonzero."));
                    set_prompt_prefix(translate("Specify reference length") + " {1}: ");
                }
                else {
                    add_rubber("LINE");
                    setRubberMode("LINE");
                    setRubberPoint("LINE_START", global.baseX, global.baseY);
                    previewOn("SELECTED", "SCALE", global.baseX, global.baseY, global.factorRef);
                    set_prompt_prefix(translate("Specify new length: "));
                }
            }
        }
        else if (isnan(global.destR.x)) {
            if (isnan(str)) {
                std_vector<char *> strList = str.split(",");
                if (isnan(strList[0]) || isnan(strList[1])) {
                    alert(translate("Requires valid numeric distance or two points."));
                    set_prompt_prefix(translate("Specify second point: "));
                }
                else {
                    global.destR.x = Number(strList[0]);
                    global.destR.y = Number(strList[1]);
                    global.factorRef = calculateDistance(global.baseR.x, global.baseR.y, global.destR.x, global.destR.y);
                    if (global.factorRef <= 0.0) {
                        global.destR.x    = 0.0f;
                        global.destR.y    = 0.0f;
                        global.factorRef = 0.0f;
                        alert(translate("Value must be positive and nonzero."));
                        set_prompt_prefix(translate("Specify second point: "));
                    }
                    else {
                        setRubberPoint("LINE_START", global.baseX, global.baseY);
                        previewOn("SELECTED", "SCALE", global.baseX, global.baseY, global.factorRef);
                        set_prompt_prefix(translate("Specify new length: "));
                    }
                }
            }
            else {
                //The base and dest values are only set here to advance the command.
                global.baseR.x = 0.0;
                global.baseR.y = 0.0;
                global.destR.x = 0.0;
                global.destR.y = 0.0;
                //The reference length is what we will use later.
                global.factorRef = Number(str);
                if (global.factorRef <= 0.0) {
                    global.destR.x    = 0.0f;
                    global.destR.y    = 0.0f;
                    global.factorRef = 0.0f;
                    alert(translate("Value must be positive and nonzero."));
                    set_prompt_prefix(translate("Specify second point: "));
                }
                else {
                    setRubberPoint("LINE_START", global.baseX, global.baseY);
                    previewOn("SELECTED", "SCALE", global.baseX, global.baseY, global.factorRef);
                    set_prompt_prefix(translate("Specify new length: "));
                }
            }
        }
        else if (isnan(global.factorNew)) {
            if (isnan(str)) {
                std_vector<char *> strList = str.split(",");
                if (isnan(strList[0]) || isnan(strList[1])) {
                    alert(translate("Requires valid numeric distance or second point."));
                    set_prompt_prefix(translate("Specify new length: "));
                }
                else {
                    float x = Number(strList[0]);
                    float y = Number(strList[1]);
                    global.factorNew = calculateDistance(global.baseX, global.baseY, x, y);
                    if (global.factorNew <= 0.0)
                    {
                        global.factorNew = 0.0f;
                        alert(translate("Value must be positive and nonzero."));
                        set_prompt_prefix(translate("Specify new length: "));
                    }
                    else {
                        scaleSelected(global.baseX, global.baseY, global.factorNew/global.factorRef);
                        previewOff();
                        end_command();
                    }
                }
            }
            else {
                global.factorNew = Number(str);
                if (global.factorNew <= 0.0) {
                    global.factorNew = 0.0f;
                    alert(translate("Value must be positive and nonzero."));
                    set_prompt_prefix(translate("Specify new length: "));
                }
                else {
                    scaleSelected(global.baseX, global.baseY, global.factorNew/global.factorRef);
                    previewOff();
                    end_command();
                }
            }
        }
            break;
    }
    default:
        break;
    }
}

UiObject
single_line_text_main(void)
{
    UiObject global = generic_design_init();
    init_command();
    clear_selection();
    global.text = "";
    global.position = {0.0f, 0.0f};
    global.textJustify = "Left";
    global.textFont = textFont();
    global.textHeight = 0.0f;
    global.textRotation = 0.0f;
    global->mode = SINGLE_LINE_TEXT_MODE_SETGEOM;
    set_prompt_prefix(translate("Current font: ") + "{" + global.textFont + "} " + translate("Text height: ") + "{" +  textSize() + "}");
    append_prompt_history();
    set_prompt_prefix(translate("Specify start point of text or [Justify/Setfont]: "));
    return global;
}

void
single_line_text_click(UiObject *global, EmbVector pos)
{
    if (global->mode == MODE_SETGEOM) {
        if (isnan(global.textX)) {
            global.textX = x;
            global.textY = y;
            add_rubber("LINE");
            setRubberMode("LINE");
            setRubberPoint("LINE_START", global.textX, global.textY);
            append_prompt_history();
            set_prompt_prefix(translate("Specify text height") + " {" + textSize() + "}: ");
        }
        else if (isnan(global.textHeight)) {
            global.textHeight = calculateDistance(global.textX, global.textY, x, y);
            setTextSize(global.textHeight);
            append_prompt_history();
            set_prompt_prefix(translate("Specify text angle") + " {" + textAngle() + "}: ");
        }
        else if (isnan(global.textRotation)) {
            global.textRotation = calculateAngle(global.textX, global.textY, x, y);
            setTextAngle(global.textRotation);
            append_prompt_history();
            set_prompt_prefix(translate("Enter text: "));
            global->mode = MODE_RAPID;
            enablePromptRapidFire();
            clearRubber();
            add_rubber("TEXTSINGLE");
            setRubberMode("TEXTSINGLE");
            setRubberPoint("TEXT_POINT", global.textX, global.textY);
            setRubberPoint("TEXT_HEIGHT_ROTATION", global.textHeight, global.textRotation);
            setRubberText("TEXT_FONT", global.textFont);
            setRubberText("TEXT_JUSTIFY", global.textJustify);
            setRubberText("TEXT_RAPID", global.text);
        }
        else {
            //Do nothing, as we are in rapidFire mode now.
        }
    }
}

void
single_line_text_prompt(UiObject *global, const char *str)
{
    switch (global->mode) {
    case SINGLE_LINE_TEXT_MODE_JUSTIFY:
        global->mode = SINGLE_LINE_TEXT_MODE_SETGEOM;
        if (streq(str, "C") || streq(str, "CENTER")) {
            global->textJustify = JUSTIFY_TOPCENTER;
                    setRubberText("TEXT_JUSTIFY", global.textJustify);
            set_prompt_prefix(translate("Specify center point of text or [Justify/Setfont]: "));
                    break;
        }
        if (streq(str, "R") || streq(str, "RIGHT")) {
            global->textJustify = JUSTIFY_RIGHT;
                    setRubberText("TEXT_JUSTIFY", global.textJustify);
            set_prompt_prefix(translate("Specify right-end point of text or [Justify/Setfont]: "));
                    break;
        }
            if (streq(str, "A") || streq(str, "ALIGN")) {
            global.textJustify = "Aligned";
            setRubberText("TEXT_JUSTIFY", global.textJustify);
            set_prompt_prefix(translate("Specify start point of text or [Justify/Setfont]: "));
            break;
        }
        if (streq(str, "M" || streq(str, "MIDDLE") {
            global.textJustify = "Middle";
            setRubberText("TEXT_JUSTIFY", global.textJustify);
            set_prompt_prefix(translate("Specify middle point of text or [Justify/Setfont]: "));
        }
        else if (streq(str, "F" || streq(str, "FIT") {
            global.textJustify = "Fit";
            setRubberText("TEXT_JUSTIFY", global.textJustify);
            set_prompt_prefix(translate("Specify start point of text or [Justify/Setfont]: "));
        }
        else if (streq(str, "TL" || streq(str, "TOPLEFT") {
            global.textJustify = "Top Left";
            setRubberText("TEXT_JUSTIFY", global.textJustify);
            set_prompt_prefix(translate("Specify top-left point of text or [Justify/Setfont]: "));
        }
        else if (streq(str, "TC" || streq(str, "TOPCENTER") {
            global.textJustify = "Top Center";
            setRubberText("TEXT_JUSTIFY", global.textJustify);
            set_prompt_prefix(translate("Specify top-center point of text or [Justify/Setfont]: "));
        }
        else if (streq(str, "TR" || streq(str, "TOPRIGHT") {
            global.textJustify = "Top Right";
            setRubberText("TEXT_JUSTIFY", global.textJustify);
            set_prompt_prefix(translate("Specify top-right point of text or [Justify/Setfont]: "));
        }
        else if (streq(str, "ML" || streq(str, "MIDDLELEFT") {
            global.textJustify = "Middle Left";
            setRubberText("TEXT_JUSTIFY", global.textJustify);
            set_prompt_prefix(translate("Specify middle-left point of text or [Justify/Setfont]: "));
        }
        else if (streq(str, "MC" || streq(str, "MIDDLECENTER") {
            global.textJustify = "Middle Center";
            setRubberText("TEXT_JUSTIFY", global.textJustify);
            set_prompt_prefix(translate("Specify middle-center point of text or [Justify/Setfont]: "));
        }
        else if (streq(str, "MR" || streq(str, "MIDDLERIGHT") {
            global.textJustify = "Middle Right";
            setRubberText("TEXT_JUSTIFY", global.textJustify);
            set_prompt_prefix(translate("Specify middle-right point of text or [Justify/Setfont]: "));
        }
        else if (streq(str, "BL" || streq(str, "BOTTOMLEFT") {
            global.textJustify = "Bottom Left";
            setRubberText("TEXT_JUSTIFY", global.textJustify);
            set_prompt_prefix(translate("Specify bottom-left point of text or [Justify/Setfont]: "));
        }
        else if (streq(str, "BC" || streq(str, "BOTTOMCENTER") {
            global.textJustify = "Bottom Center";
            setRubberText("TEXT_JUSTIFY", global.textJustify);
            set_prompt_prefix(translate("Specify bottom-center point of text or [Justify/Setfont]: "));
        }
        else if (streq(str, "BR" || streq(str, "BOTTOMRIGHT") {
            global.textJustify = "Bottom Right";
            setRubberText("TEXT_JUSTIFY", global.textJustify);
            set_prompt_prefix(translate("Specify bottom-right point of text or [Justify/Setfont]: "));
        }
        else {
            alert(translate("Invalid option keyword."));
            set_prompt_prefix(translate("Text Justification Options [Center/Right/Align/Middle/Fit/TL/TC/TR/ML/MC/MR/BL/BC/BR]: "));
        }
    }
            break;

    case SINGLE_LINE_TEXT_MODE_SETFONT:
        global->mode = SINGLE_LINE_TEXT_MODE_SETGEOM;
            strcpy(global.textFont, str);
        setRubberText("TEXT_FONT", global.textFont);
        setTextFont(global.textFont);
        set_prompt_prefix(translate("Specify start point of text or [Justify/Setfont]: "));
            break;

    case SINGLE_LINE_TEXT_MODE_SETGEOM:
            if (isnan(global.textX)) {
            if (streq(str, "J" || streq(str, "JUSTIFY") {
                global->mode = MODE_JUSTIFY;
                set_prompt_prefix(translate("Text Justification Options [Center/Right/Align/Middle/Fit/TL/TC/TR/ML/MC/MR/BL/BC/BR]: "));
            }
            else if (streq(str, "S" || streq(str, "SETFONT") {
                global->mode = MODE_SETFONT;
                set_prompt_prefix(translate("Specify font name: "));
            }
            else {
                std_vector<char *> strList = str.split(",");
                if (isnan(strList[0]) || isnan(strList[1])) {
                    alert(translate("Point or option keyword required."));
                    set_prompt_prefix(translate("Specify start point of text or [Justify/Setfont]: "));
                }
                else {
                    global.textX = Number(strList[0]);
                    global.textY = Number(strList[1]);
                    add_rubber("LINE");
                    setRubberMode("LINE");
                    setRubberPoint("LINE_START", global.textX, global.textY);
                    set_prompt_prefix(translate("Specify text height") + " {" + textSize() + "}: ");
                }
            }
        }
        else if (isnan(global.textHeight)) {
            if (streq(str, "") {
                global.textHeight = textSize();
                set_prompt_prefix(translate("Specify text angle") + " {" + textAngle() + "}: ");
            }
            else if (isnan(str)) {
                alert(translate("Requires valid numeric distance or second point."));
                set_prompt_prefix(translate("Specify text height") + " {" + textSize() + "}: ");
            }
            else {
                global.textHeight = Number(str);
                setTextSize(global.textHeight);
                set_prompt_prefix(translate("Specify text angle") + " {" + textAngle() + "}: ");
            }
        }
        else if (isnan(global.textRotation)) {
            if (streq(str, "") {
                global.textRotation = textAngle();
                set_prompt_prefix(translate("Enter text: "));
                global->mode = MODE_RAPID;
                enablePromptRapidFire();
                clearRubber();
                add_rubber("TEXTSINGLE");
                setRubberMode("TEXTSINGLE");
                setRubberPoint("TEXT_POINT", global.textX, global.textY);
                setRubberPoint("TEXT_HEIGHT_ROTATION", global.textHeight, global.textRotation);
                setRubberText("TEXT_FONT", global.textFont);
                setRubberText("TEXT_JUSTIFY", global.textJustify);
                setRubberText("TEXT_RAPID", global.text);
            }
            else if (isnan(str)) {
                alert(translate("Requires valid numeric angle or second point."));
                set_prompt_prefix(translate("Specify text angle") + " {" + textAngle() + "}: ");
            }
            else {
                global.textRotation = Number(str);
                setTextAngle(global.textRotation);
                set_prompt_prefix(translate("Enter text: "));
                global->mode = MODE_RAPID;
                enablePromptRapidFire();
                clearRubber();
                add_rubber("TEXTSINGLE");
                setRubberMode("TEXTSINGLE");
                setRubberPoint("TEXT_POINT", global.textX, global.textY);
                setRubberPoint("TEXT_HEIGHT_ROTATION", global.textHeight, global.textRotation);
                setRubberText("TEXT_FONT", global.textFont);
                setRubberText("TEXT_JUSTIFY", global.textJustify);
                setRubberText("TEXT_RAPID", global.text);
            }
        }
        else {
            //Do nothing, as we are in rapidFire mode now.
        }
            break;

    case SINGLE_LINE_TEXT_MODE_RAPID:
        /**
        if (streq(str, "RAPID_ENTER") {
            if (global.text == "") {
                end_command();
            }
            else {
                vulcanize();
                end_command(); //\todo Rather than ending the command, calculate where the next line would be and modify the x/y to the new point
            }
        }
        else {
            global.text = str;
            setRubberText("TEXT_RAPID", global.text);
        }
            break;

    default:
        break;
    }
}

UiObject
snowflake_main(void)
{
    UiObject global = generic_design_init();
    global.numPoints = 2048;
    global.minPoints = 64;
    global.maxPoints = 8192;
    global.scale.x = 0.04f;
    global.scale.y = 0.04f;
    global.mode = SNOWFLAKE_MODE_NUM_POINTS;

    add_rubber("POLYGON");
    setRubberMode("POLYGON");
    updateSnowflake(path, global);
    spareRubber("POLYGON");
    end_command();
    return global;
}

UiObject
snowflake_click(UiObject global, EmbVector position)
{

    return global;
}

/**
 * @brief Move the snowflake design by a specified amount.
 *
 * \a global The design specification (not including the path).
 * \a delta The vector to display the design by.
 */
UiObject
snowflake_move(UiObject global, EmbVector delta)
{
    global.center = embVector_add(global.center, delta);
    return global;
}

void
updateSnowflake(EmbPath *path, UiObject global)
{
    int i;
    float t;
    EmbVector v;
    v.x = 0.0;
    v.y = 0.0;

    float two_pi = 2*Math.PI;

    for (i = 0; i <= numPts; i++) {
        t = two_pi/numPts*i;

        // Snowflake Curve with t [0,2pi]
        xx =;
        yy =;

        setRubberPoint("POLYGON_POINT_" + i.toString(), xx*xScale, yy*yScale);
    }

    setRubberText("POLYGON_NUM_POINTS", numPts.toString());
}

UiObject
generic_design_init(void)
{
    UiObject design;
    init_command();
    clear_selection();
    design.center.x = 0.0f;
    design.center.y = 0.0f;
    for (int i=0; i<10; i++) {
        design.controlPoints[i].x = 0.0f;
        design.controlPoints[i].y = 0.0f;
    }
    return design;
}

UiObject
star_main(void)
{
    char msg[200];
    UiObject global = generic_design_init();
    global.numPoints = 5;
    global.mode = STAR_MODE_NUM_POINTS;
    sprintf(msg, "%s {%d}: ", translate("Enter number of star points"), global.numPoints);
    set_prompt_prefix(msg);
    return global;
}

UiObject
star_click(UiObject global, EmbVector pos)
{
    switch (global.mode) {
    case STAR_MODE_NUM_POINTS:
        /* Do nothing, the prompt controls this. */
        break;
    case STAR_MODE_CENTER_PT: {
            global.cx = x;
        global.cy = y;
        global->mode = MODE_RAD_OUTER;
        set_prompt_prefix(translate("Specify outer radius of star: "));
        add_rubber("POLYGON");
        setRubberMode("POLYGON");
        updateStar(global.cx, global.cy);
        enableMoveRapidFire();
            break;
    }
    case STAR_MODE_RAD_OUTER: {
            global.x1 = x;
        global.y1 = y;
        global->mode = MODE_RAD_INNER;
        set_prompt_prefix(translate("Specify inner radius of star: "));
        updateStar(global.x1, global.y1);
            break;
    }
    case STAR_MODE_RAD_INNER: {
            global.x2 = x;
        global.y2 = y;
        disableMoveRapidFire();
        updateStar(global.x2, global.y2);
        spareRubber("POLYGON");
        end_command();
            break;
    }
    default:
        break;
    }
}

UiObject
star_move(UiObject global, EmbVector delta)
{
    switch (global.mode) {
    case STAR_MODE_NUM_POINTS:
        //Do nothing, the prompt controls this.
        break;
    case STAR_MODE_CENTER_PT:
        //Do nothing, prompt and click controls this.
        break;
    case STAR_MODE_RAD_OUTER:
        // updateStar(delta);
        break;
    case STAR_MODE_RAD_INNER:
        // updateStar(delta);
        break;
    default:
        break;
    }
    return global;
}

void
star_prompt(UiObject design, const char *str)
{
    switch (design.mode) {
    case STAR_MODE_NUM_POINTS:
            if (streq(str, "" && global.numPoints >= 3 && global.numPoints <= 1024) {
            set_prompt_prefix(translate("Specify center point: "));
            global->mode = MODE_CENTER_PT;
        }
        else {
            float tmp = Number(str);
            if (isnan(tmp) || !isInt(tmp) || tmp < 3 || tmp > 1024) {
                alert(translate("Requires an integer between 3 and 1024."));
                set_prompt_prefix(translate("Enter number of star points") + " {" + global.numPoints.toString() + "}: ");
            }
            else {
                global.numPoints = tmp;
                set_prompt_prefix(translate("Specify center point: "));
                global->mode = MODE_CENTER_PT;
            }
        }
            break;
    case STAR_MODE_CENTER_PT:
            std_vector<char *> strList = str.split(",");
        if (isnan(strList[0]) || isnan(strList[1])) {
            alert(translate("Invalid point."));
            set_prompt_prefix(translate("Specify center point: "));
        }
        else {
            global.center.x = Number(strList[0]);
            global.center.y = Number(strList[1]);
            global->mode = MODE_RAD_OUTER;
            set_prompt_prefix(translate("Specify outer radius of star: "));
            add_rubber("POLYGON");
            setRubberMode("POLYGON");
            updateStar(qsnapX(), qsnapY());
            enableMoveRapidFire();
        }
            break;
    case STAR_MODE_RAD_INNER:
            std_vector<char *> strList = str.split(",");
        if (isnan(strList[0]) || isnan(strList[1])) {
            alert(translate("Invalid point."));
            set_prompt_prefix(translate("Specify inner radius of star: "));
        }
        else {
            global.x2 = Number(strList[0]);
            global.y2 = Number(strList[1]);
            disableMoveRapidFire();
            updateStar(global.x2, global.y2);
            spareRubber("POLYGON");
            end_command();
        }
            break;
    case STAR_MODE_RAD_OUTER:
            std_vector<char *> strList = str.split(",");
        if (isnan(strList[0]) || isnan(strList[1])) {
            alert(translate("Invalid point."));
            set_prompt_prefix(translate("Specify outer radius of star: "));
        }
        else {
            global.point1.x = Number(strList[0]);
            global.point1.y = Number(strList[1]);
            global->mode = MODE_RAD_INNER;
            set_prompt_prefix(translate("Specify inner radius of star: "));
            updateStar(qsnapX(), qsnapY());
        }
            break;
    default:
        break;
    }
}

void
updateStar(EmbPath *path, UiObject global)
{
    float distOuter = 0.0;
    float distInner = 0.0;
    float angOuter = 0.0;

    if (n_views == 0) {
        return;
    }

    switch (global.mode) {
    case STAR_MODE_RAD_OUTER: {
        EmbVector delta;
        delta = embVector_subtract(global.center, global.controlPoints[1]);
        angOuter = embVector_angle(delta);
        distOuter = embVector_distance(global.center, global.controlPoints[1]);
        distInner = distOuter/2.0;
        break;
    }
    case STAR_MODE_RAD_INNER: {
        EmbVector delta;
        delta = embVector_subtract(global.center, global.controlPoints[0]);
        angOuter = embVector_angle(delta);
        distOuter = embVector_distance(global.center, global.controlPoints[0]);
        distInner = embVector_distance(global.center, global.controlPoints[1]);
        break;
    }
    default:
        break;
    }

    //Calculate the Star Points
    float angInc = 360.0/(global.numPoints*2);

    for (int i = 0; i < (int)global.numPoints*2; i++) {
        EmbVector point;
        EmbReal angle = (angOuter+(angInc*i)) * embConstantPi / 180.0;
        if (i%2 == 1) {
            point.x = distOuter*cos(angle);
            point.y = distOuter*sin(angle);
        }
        else {
            point.x = distInner*cos(angle);
            point.y = distInner*sin(angle);
        }
        printf("%f %f", point.x, point.y);
            setRubberPoint("POLYGON_POINT_" + i.toString(), global.cx + xx, global.cy + yy);
        }
    setRubberText("POLYGON_NUM_POINTS", (global.numPoints*2 - 1).toString());
}

#endif
