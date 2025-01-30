/*
 * Embroidermodder 2 (GLFW version)
 *
 * Copyright 2011-2025 The Embroidermodder Team
 * Embroidermodder 2 is Open Source Software, see LICENSE.md for licensing terms.
 *
 * Read the reference manual (https://www.libembroidery.org/downloads/emrm.pdf)
 * for advice on altering this file.
 *
 * Main window wrangling and drawing functions.
 *
 * Operating system-specific system calls like usleep go in this file:
 * see the block of system-specific headers below.
 */

#include <stdio.h>
#include <string.h>
#include <assert.h>
#include <stdlib.h>
#include <limits.h>
#include <errno.h>
#include <time.h>
#include <math.h>

/* Advice on safe C preprocessor directive use:
 * https://github.com/cpredef/predef
 */
#if defined(linux) || defined(__unix) || defined(__unix__) || defined(__APPLE__)

#include <unistd.h>
#include <sys/types.h>
#include <pwd.h>

#elif defined(WINDOWS)

#define WIN32_LEAN_AND_MEAN
#include <windows.h>

#endif

#if !defined(WINDOWS)
#include <X11/X.h>
#include <X11/Xlib.h>
#include <X11/Xatom.h>
#endif

/* Needed by nanovg_gl. */
#include "glad/glad.h"

/* These have to follow "glad". */
#include <GL/gl.h>
#include <GL/glx.h>
#include <GL/glu.h>

#include <GLFW/glfw3.h>

#include "nanovg.h"
#define NANOVG_GLES2_IMPLEMENTATION
#include "nanovg_gl.h"

#define NANOSVG_IMPLEMENTATION
#include "nanosvg.h"

#include "core.h"

#define MAX_HISTORY            300

/* TODO: << "Aligned" << "Fit" */
char *objectTextJustifyList[] = {
    "Left",
    "Center",
    "Right",
    "Middle",
    "Top Left",
    "Top Center",
    "Top Right",
    "Middle Left",
    "Middle Center",
    "Middle Right",
    "Bottom Left",
    "Bottom Center",
    "Bottom Right",
    "END"
};

EmbVector doc_map_to_scene(int32_t doc, EmbVector v);

const char *help_msg = \
     " ___ _____ ___  ___   __  _ ___  ___ ___   _____  __  ___  ___  ___ ___    ___ \n" \
    "| __|     | _ \\| _ \\ /  \\| |   \\| __| _ \\ |     |/  \\|   \\|   \\| __| _ \\  |__ \\\n" \
    "| __| | | | _ <|   /| () | | |) | __|   / | | | | () | |) | |) | __|   /  / __/\n" \
    "|___|_|_|_|___/|_|\\_\\\\__/|_|___/|___|_|\\_\\|_|_|_|\\__/|___/|___/|___|_|\\_\\ |___|\n" \
    " _____________________________________________________________________________ \n" \
    "|                                                                             |\n" \
    "|                   https://www.libembroidery.org                             |\n" \
    "|_____________________________________________________________________________|\n" \
    "\n" \
    "Usage: embroidermodder [options] files ...\n" \
    "Options:\n" \
    "  -d, --debug      Print lots of debugging information.\n" \
    "  -h, --help       Print this message and exit.\n" \
    "  -v, --version    Print the version number of embroidermodder and exit.\n" \
    "\n";

/*
 * Create the SVG for the foreground and background of the view. This is then
 * rendered based on the system-specifics.
 */
char *ruler_svg = "<svg width=\"WIDTH\" height=\"HEIGHT\" viewBox=\"0 0 WIDTH HEIGHT\" fill=\"none\" xmlns=\"http://www.w3.org/2000/svg\">" \
    "<rect x=\"0\" y=\"0\" width=\"WIDTH\" height=\"HEIGHT\" fill=\"RULER_COLOR\" stroke=\"RULER_BORDER\" stroke-width=\"1\" />" \
    "</svg>";

double zoomInLimit = 1.0e-10;
double zoomOutLimit = 1.0e10;

EmbString prompt_input_list[MAX_HISTORY];
int prompt_history_size = 0;
int prompt_history_position = 0;

NVGcontext *vg;

GLFWwindow* create_window(int32_t width, int32_t height, const char *title);

void draw_rect(NVGcontext *vg, EmbRect rect, EmbColor color);
void draw_button(NVGcontext *vg, Button button, float *bounds);
void draw_text(NVGcontext *vg, int x, int y, char *font, char *txt,
    EmbColor color, float *bounds);
void draw_view(NVGcontext *vg, EmbRect container);
void draw_mdiarea(NVGcontext *vg, EmbRect container);
void draw_dockarea(NVGcontext *vg, EmbRect container);
void draw_nvg_toolbar(NVGcontext *vg, EmbRect container);
void draw_prompt(NVGcontext *vg, EmbRect container);
void draw_statusbar(NVGcontext *vg, EmbRect container);

int icon_size = 16;

Button menu_buttons[10];
Button tool_buttons_a[100];

/* FIXME: this is very system-dependant. Could compile in somehow? */
const char *sans_font = "/usr/share/fonts/truetype/open-sans/OpenSans-Regular.ttf";
const char *mono_font = "/usr/share/fonts/truetype/freefont/FreeMono.ttf";
const char *icon_font = "/usr/share/fonts/truetype/open-sans/OpenSans-Regular.ttf";

EmbColor toolbar_bg_color = {.r=128, .g=128, .b=128};
EmbColor toolbar_button_color = {.r=208, .g=208, .b=208};
EmbColor toolbar_text = {.r=0, .g=0, .b=0};
EmbColor prompt_bg_color = {.r=255, .g=255, .b=255};
EmbColor prompt_color = {.r=0, .g=0, .b=0};
EmbColor scrollbar_color = {.r=100, .g=100, .b=100};
int window_width = 640;
int window_height = 480;
int toolbar_height = 40;
float button_padding = 5.0;
float prompt_divider = 405.0F;
float statusbar_depth = 40.0F;

char prompt_text[50][MAX_STRING_LENGTH] = {
    "prompt> ",
    "",
    "",
    ""
};

int n_prompt_lines = 3;

/* Check this. */
uint32_t
RGB(uint8_t r, uint8_t g, uint8_t b)
{
    uint32_t result = 0xFF000000;
    result += 0x10000 * r;
    result += 0x100 * g;
    result += b;
    return result;
}

/* . */
void
update_all_background_color(uint32_t color)
{
    setBackgroundColor(color);
}

void
preview_update(void)
{
    useBackgroundLogo(setting[GENERAL_MDI_BG_USE_LOGO].preview.b);
    useBackgroundColor(setting[GENERAL_MDI_BG_USE_COLOR].preview.b);
    useBackgroundTexture(setting[GENERAL_MDI_BG_USE_TEXTURE].preview.b);
    update_all_view_scroll_bars(setting[DISPLAY_SHOW_SCROLLBARS].preview.b);
}

/* . */
void
update_view(void)
{
    useBackgroundLogo(setting[GENERAL_MDI_BG_USE_LOGO].dialog.b);
    useBackgroundTexture(setting[GENERAL_MDI_BG_USE_TEXTURE].dialog.b);
    useBackgroundColor(setting[GENERAL_MDI_BG_USE_COLOR].dialog.b);
    setBackgroundLogo(setting[GENERAL_MDI_BG_LOGO].dialog.s);
    setBackgroundTexture(setting[GENERAL_MDI_BG_TEXTURE].dialog.s);
    setBackgroundColor(setting[GENERAL_MDI_BG_COLOR].dialog.i);

    icon_resize(setting[GENERAL_ICON_SIZE].dialog.i);

    update_all_view_scroll_bars(setting[DISPLAY_SHOW_SCROLLBARS].dialog.b);
    update_all_view_cross_hair_colors(setting[DISPLAY_CROSSHAIR_COLOR].dialog.i);
    update_all_view_background_colors(setting[DISPLAY_BG_COLOR].dialog.i);
    update_all_view_select_box_colors(
        setting[DISPLAY_SELECTBOX_LEFT_COLOR].dialog.i,
        setting[DISPLAY_SELECTBOX_LEFT_FILL].dialog.i,
        setting[DISPLAY_SELECTBOX_RIGHT_COLOR].dialog.i,
        setting[DISPLAY_SELECTBOX_RIGHT_FILL].dialog.i,
        setting[DISPLAY_SELECTBOX_ALPHA].dialog.i);
    update_all_view_grid_colors(setting[GRID_COLOR].dialog.i);
    update_all_view_ruler_colors(setting[RULER_COLOR].dialog.i);

    set_prompt_text_color(setting[PROMPT_TEXT_COLOR].dialog.i);
    set_prompt_background_color(setting[PROMPT_BG_COLOR].dialog.i);
    set_prompt_font_family(setting[PROMPT_FONT_FAMILY].dialog.s);
    set_prompt_font_style(setting[PROMPT_FONT_STYLE].dialog.s);
    set_prompt_font_size(setting[PROMPT_FONT_SIZE].dialog.i);

    statusbar_toggle("LWT", setting[LWT_SHOW_LWT].dialog.b);
    statusbar_toggle("REAL", setting[LWT_REAL_RENDER].dialog.b);

    update_pick_add_mode(setting[SELECTION_MODE_PICKADD].dialog.b);
}

/* . */
void
mdiarea_set_bg(uint32_t color)
{
    setBackgroundColor(color);
}

/* . */
void
draw_rect(NVGcontext *vg, EmbRect rect, EmbColor color)
{
    nvgFillColor(vg, nvgRGBA(color.r, color.g, color.b, 255));

    nvgBeginPath(vg);
    nvgRect(vg, rect.x, rect.y, rect.w, rect.h);
    nvgFill(vg);
}

/* . */
void
draw_button(NVGcontext *vg, Button button, float *bounds)
{
    draw_rect(vg, button.rect, button.color);
    draw_text(vg,
        button.rect.x + 3.0F,
        button.rect.y + button.rect.h * 0.5F,
        button.font,
        button.text,
        button.text_color,
        bounds);
}

/* . */
void
draw_text(NVGcontext *vg, int x, int y, char *font, char *txt, EmbColor text_color, float *bounds)
{
    nvgBeginPath(vg);
    nvgFontSize(vg, 14.0F);
    nvgFontFace(vg, font);
    nvgTextAlign(vg, NVG_ALIGN_CENTER | NVG_ALIGN_MIDDLE);
    nvgFillColor(vg, nvgRGBA(text_color.r, text_color.g, text_color.b, 255));
    nvgTextBounds(vg, x, y, txt, NULL, bounds);
    nvgTextAlign(vg, NVG_ALIGN_LEFT);
    nvgText(vg, x, y, txt, NULL);
    nvgFill(vg);
}

/* . */
Button
make_menubar_button(NVGcontext *vg, int x, int y, char *text, float *bounds)
{
    Button button;
    nvgTextBounds(vg, x, y, text, NULL, bounds);
    EmbReal w = bounds[2] - bounds[0] + button_padding;
    button.rect = emb_rect(x, y, w, 30);
    button.color = toolbar_button_color;
    button.text_color = toolbar_text;
    strcpy(button.text, text);
    strcpy(button.font, "sans");
    button.state = 0;
    return button;
}

/* . */
Button
make_toolbar_button(int x, int y, char *text)
{
    Button button;
    button.rect = emb_rect(x, y, icon_size, icon_size);
    button.color = toolbar_button_color;
    button.text_color = toolbar_text;
    strcpy(button.text, text);
    strcpy(button.font, "icons");
    button.state = 0;
    return button;
}

/* . */
void
draw_menubar(NVGcontext *vg, EmbRect container)
{
    EmbRect rect = emb_rect(0, 0, window_width, toolbar_height);
    if (window_height > 1000) {
        rect.h = 50;
    }

    draw_rect(vg, rect, toolbar_bg_color);

    float bounds[4] = {0.0F, 0.0F, 0.0F, 0.0F};
    float x_offset = -button_padding;
    for (int i=0; menubar_full_list[i] != TERMINATOR_SYMBOL; i++) {
        x_offset += (bounds[2] - bounds[0]) + 2*button_padding;
        menu_buttons[i] = make_menubar_button(vg, x_offset, 3,
            menu_list[menubar_full_list[i]], bounds);
        draw_button(vg, menu_buttons[i], bounds);
    }
}

/* . */
void
draw_nvg_toolbar(NVGcontext *vg, EmbRect container)
{
    EmbRect rect = emb_rect(toolbar_height + button_padding, 0,
        window_width, toolbar_height);

    //draw_rect(vg, rect, toolbar_bg_color);

    float bounds[4] = {0.0F, 0.0F, 0.0F, 0.0F};
    for (int i=0; i<10; i++) {
        tool_buttons_a[i] = make_toolbar_button(5+5*i+16*i, 50, "Test");
        draw_button(vg, tool_buttons_a[i], bounds);
    }
}

/* . */
void
draw_view(NVGcontext *vg, EmbRect container)
{
    EmbRect rect = emb_rect(0, 405, window_width, 75);
    draw_rect(vg, rect, prompt_bg_color);

}

/* . */
void
draw_mdiarea(NVGcontext *vg, EmbRect container)
{
    if (numOfDocs == 0) {
    }
    else {
    }
}

/* . */
void
draw_dockarea(NVGcontext *vg, EmbRect container)
{

}

/* . */
void
draw_prompt(NVGcontext *vg, EmbRect container)
{
    float bounds[5];
    EmbRect rect = container;
    draw_rect(vg, rect, prompt_bg_color);
    EmbRect sb_rect = emb_rect(window_width - 20, 405, 20, 75);
    draw_rect(vg, sb_rect, scrollbar_color);
    for (int i=0; i<n_prompt_lines; i++) {
        draw_text(vg, 10, 425+i*3, "mono", prompt_text[i], prompt_color, bounds);
    }
}

/* . */
void
draw_statusbar(NVGcontext *vg, EmbRect container)
{

}

/* . */
void
svg_test(void)
{
    EmbVector pos = emb_vector(100.0, 100.0);
    EmbVector scale = emb_vector(1.0, 1.0);

    char *data = load_file("draft.svg");
    NSVGimage *image = nsvgParse(data, "px", 96);
    free(data);
    NSVGshape *shape;
    NSVGpath *path;
    int i;

    FILE *f = fopen("parsed.svg", "w");
    fprintf(f, "<?xml version=\"1.0\" encoding=\"UTF-8\" standalone=\"no\"?>\n");
    fprintf(f, "<svg version=\"1.1\" width=\"128\" height=\"128\"\n");
    fprintf(f, "    viewBox=\"0 0 128 128\" xmlns=\"http://www.w3.org/2000/svg\"\n");
    fprintf(f, "    xmlns:svg=\"http://www.w3.org/2000/svg\">\n");

    for (shape = image->shapes; shape != NULL; shape = shape->next)
    for (path = shape->paths; path != NULL; path = path->next) {
        unsigned int c = shape->stroke.color;
        unsigned char r = (c & 0xFF);
        unsigned char g = (c & 0xFF00) >> 8;
        unsigned char b = (c & 0xFF0000) >> 16;
        NVGcolor color = nvgRGBA(r, g, b, 255);
        nvgStrokeColor(vg, color);
        nvgStrokeWidth(vg, shape->strokeWidth);
        fprintf(f, "<path color=\"%d,%d,%d\" width=\"%.2f\" d=\"",
            r, g, b, shape->strokeWidth);
        float* p = &path->pts[0];
        fprintf(f, "M %.2f %.2f ", p[0], p[1]);
        nvgMoveTo(vg, p[0], p[1]);
        for (i = 3; i < path->npts-1; i += 3) {
	        float* p = &path->pts[i*2];
            nvgBezierTo(vg, p[2], p[3], p[4], p[5], p[6], p[7]);
            fprintf(f, "C %.2f %.2f %.2f %.2f %.2f %.2f ",
                p[2], p[3], p[4], p[5], p[6], p[7]);
        }
        if (path->closed) {
            fprintf(f, "z");
        }
        fprintf(f, "\"/>\n");
    }
    fprintf(f, "</svg>\n");
    fclose(f);

    nsvgDelete(image);
}

/* . */
void
draw_interface(NVGcontext *vg)
{
    draw_menubar(vg, emb_rect(0, 405, window_width, 75));
    draw_nvg_toolbar(vg, emb_rect(0, 405, window_width, 75));
    draw_mdiarea(vg, emb_rect(0, prompt_divider, window_width, 75));
    draw_prompt(vg, emb_rect(0, prompt_divider, window_width, 75));
    draw_statusbar(vg, emb_rect(0, 405, window_width, 75));
    svg_test();
}

/* Set delta time, clear frame and resize. */
int
start_frame(GLFWwindow *window, EmbReal *prevt)
{
    EmbReal t = glfwGetTime();
    EmbReal dt = t - *prevt;

    /* Cap at 120 updates a second. */
    if (dt < 1.0/120) {
        return 0;
    }
    *prevt = t;

    glfwGetWindowSize(window, &window_width, &window_height);
    glClear(GL_COLOR_BUFFER_BIT);
    glViewport(0, 0, window_width, window_height);
    nvgBeginFrame(vg, window_width, window_height, 1.0);
    return 1;
}

/* Swap buffers and poll events. */
void
end_frame(GLFWwindow *window)
{
    nvgEndFrame(vg);

    glfwSwapBuffers(window);
    glfwPollEvents();
}

/* FIXME: error code
 *
 * NOTE: translation is the repsonisbility of the caller, because some reports
 * include parts that aren't translated. For example:
 *
 *     char message[MAX_STRING_LENGTH];
 *     sprintf(message, "%s: x > %f", translate("Value of X is too small"), x);
 *     messagebox("critical", translate("Out of Bounds"), message);
 */
void
messagebox(const char *logo, const char *title, const char *text)
{
    GLFWwindow* window = create_window(480, 200, title);

    EmbReal prevt = glfwGetTime();
    while (!glfwWindowShouldClose(window)) {
        if (!start_frame(window, &prevt)) {
            continue;
        }
        float bounds[5];
        EmbRect sb_rect = emb_rect(0, 0, window_width, window_height);
        draw_rect(vg, sb_rect, prompt_bg_color);
        draw_text(vg, 10, 40, "sans", title, prompt_color, bounds);
        draw_text(vg, 10, 80, "sans", text, prompt_color, bounds);
        end_frame(window);
    }
}

/* . */
GLFWwindow*
create_window(int32_t width, int32_t height, const char *title)
{
    GLFWwindow* window = glfwCreateWindow(width, height, title, NULL, NULL);
    if (!window) {
        glfwTerminate();
        return NULL;
    }

    glfwMakeContextCurrent(window);

    if (!gladLoadGL()) {
        puts("Failed load GL function pointers with glad.");
        return NULL;
    }

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 2);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 0);
    glfwWindowHint(GLFW_SAMPLES, 4);

    vg = nvgCreateGLES2(NVG_ANTIALIAS | NVG_STENCIL_STROKES | NVG_DEBUG);
    if (!vg) {
        puts("Failed to initialize NanoVG.");
        return NULL;
    }

    /* FIXME: only works on linux and only works if adobe fonts are installed. */
    int font = nvgCreateFont(vg, "sans", sans_font);
    if (font == -1) {
        puts("Font failed to load.");
        return NULL;
    }
    font = nvgCreateFont(vg, "icons", icon_font);
    if (font == -1) {
        puts("Font failed to load.");
        return NULL;
    }
    font = nvgCreateFont(vg, "mono", mono_font);
    if (font == -1) {
        puts("Font failed to load.");
        return NULL;
    }
    return window;
}

int32_t
free_glfw(void)
{
    nvgDeleteGLES2(vg);
    glfwTerminate();
    return 0;
}

/* . */
int
glfw_application(int argc, char *argv[])
{
    GLFWwindow* window = create_window(640, 480, "Embroidermodder 2 (GLFW)");

    EmbReal prevt = glfwGetTime();
    while (!glfwWindowShouldClose(window)) {
        if (!start_frame(window, &prevt)) {
            continue;
        }
        draw_interface(vg);
        end_frame(window);
    }
    return 0;
}

/* . */
void
settings_prompt(void)
{
    settings_dialog("Prompt");
}

/* . */
void
accept_interface_color(int32_t key, uint32_t color)
{
    setting[key].accept.i = color;
    switch (key) {
    case DISPLAY_CROSSHAIR_COLOR:
        update_all_view_cross_hair_colors(setting[key].accept.i);
        break;
    case DISPLAY_BG_COLOR:
        update_all_view_background_colors(setting[key].accept.i);
        break;
    case DISPLAY_SELECTBOX_LEFT_COLOR:
    case DISPLAY_SELECTBOX_LEFT_FILL:
    case DISPLAY_SELECTBOX_RIGHT_COLOR:
    case DISPLAY_SELECTBOX_RIGHT_FILL:
        update_all_view_select_box_colors(
            setting[DISPLAY_SELECTBOX_LEFT_COLOR].accept.i,
            setting[DISPLAY_SELECTBOX_LEFT_FILL].accept.i,
            setting[DISPLAY_SELECTBOX_RIGHT_COLOR].accept.i,
            setting[DISPLAY_SELECTBOX_RIGHT_FILL].accept.i,
            setting[DISPLAY_SELECTBOX_ALPHA].preview.i);
        break;
    case PROMPT_TEXT_COLOR:
        set_prompt_text_color(setting[key].accept.i);
        break;
    case PROMPT_BG_COLOR:
        set_prompt_background_color(setting[key].accept.i);
        break;
    case GRID_COLOR:
        update_all_view_grid_colors(setting[GRID_COLOR].accept.i);
        break;
    case RULER_COLOR:
        update_all_view_ruler_colors(setting[key].accept.i);
        break;
    case GENERAL_MDI_BG_COLOR:
        mdiarea_set_bg(setting[key].accept.i);
        break;
    default:
        break;
    }
}

/* . */
void
preview_interface_color(int32_t key, uint32_t color)
{
    setting[key].preview.i = color;
    switch (key) {
    case DISPLAY_CROSSHAIR_COLOR:
        update_all_view_cross_hair_colors(setting[key].preview.i);
        break;
    case DISPLAY_BG_COLOR:
        update_all_view_background_colors(setting[key].preview.i);
        break;
    case DISPLAY_SELECTBOX_LEFT_COLOR:
    case DISPLAY_SELECTBOX_LEFT_FILL:
    case DISPLAY_SELECTBOX_RIGHT_COLOR:
    case DISPLAY_SELECTBOX_RIGHT_FILL:
        update_all_view_select_box_colors(
            setting[DISPLAY_SELECTBOX_LEFT_COLOR].preview.i,
            setting[DISPLAY_SELECTBOX_LEFT_FILL].preview.i,
            setting[DISPLAY_SELECTBOX_RIGHT_COLOR].preview.i,
            setting[DISPLAY_SELECTBOX_RIGHT_FILL].preview.i,
            setting[DISPLAY_SELECTBOX_ALPHA].preview.i);
        break;
    case PROMPT_TEXT_COLOR:
        set_prompt_text_color(setting[key].preview.i);
        break;
    case PROMPT_BG_COLOR:
        set_prompt_background_color(setting[key].preview.i);
        break;
    case GRID_COLOR:
        update_all_view_grid_colors(setting[GRID_COLOR].preview.i);
        break;
    case RULER_COLOR:
        update_all_view_ruler_colors(setting[key].preview.i);
        break;
    case GENERAL_MDI_BG_COLOR:
        mdiarea_set_bg(setting[key].preview.i);
        break;
    default:
        break;
    }
}

/* . */
void
dialog_interface_color(int32_t key, uint32_t color)
{
    setting[key].dialog.i = color;
    switch (key) {
    case DISPLAY_CROSSHAIR_COLOR:
        update_all_view_cross_hair_colors(setting[key].dialog.i);
        break;
    case DISPLAY_BG_COLOR:
        update_all_view_background_colors(setting[key].dialog.i);
        break;
    case DISPLAY_SELECTBOX_LEFT_COLOR:
    case DISPLAY_SELECTBOX_LEFT_FILL:
    case DISPLAY_SELECTBOX_RIGHT_COLOR:
    case DISPLAY_SELECTBOX_RIGHT_FILL:
        update_all_view_select_box_colors(
            setting[DISPLAY_SELECTBOX_LEFT_COLOR].dialog.i,
            setting[DISPLAY_SELECTBOX_LEFT_FILL].dialog.i,
            setting[DISPLAY_SELECTBOX_RIGHT_COLOR].dialog.i,
            setting[DISPLAY_SELECTBOX_RIGHT_FILL].dialog.i,
            setting[DISPLAY_SELECTBOX_ALPHA].preview.i);
        break;
    case PROMPT_TEXT_COLOR:
        set_prompt_text_color(setting[key].dialog.i);
        break;
    case PROMPT_BG_COLOR:
        set_prompt_background_color(setting[key].dialog.i);
        break;
    case GRID_COLOR:
        update_all_view_grid_colors(setting[GRID_COLOR].dialog.i);
        break;
    case RULER_COLOR:
        update_all_view_ruler_colors(setting[key].dialog.i);
        break;
    case GENERAL_MDI_BG_COLOR:
        mdiarea_set_bg(setting[key].dialog.i);
        break;
    default:
        break;
    }
}

/* . */
void
prompt_history_appended(char *txt)
{
    sprintf(promptHistoryData, "%s<br/>%s", promptHistoryData, txt);
}

/* . */
void
log_prompt_input(char *txt)
{
    string_copy(prompt_input_list[prompt_history_size], txt);
    prompt_history_size++;
    prompt_history_position = prompt_history_size;
}

/* . */
void
prompt_input_previous(void)
{
    if (prompt_history_size <= 0) {
        messagebox("critical", translate("Prompt Previous Error"),
            translate("The prompt input is empty! Please report this as a bug!"));
        debug_message("The prompt input is empty! Please report this as a bug!");
    }
    else {
        prompt_history_position--;
        int maxNum = prompt_history_size;
        if (prompt_history_position < 0) {
            prompt_history_position = 0;
            prompt_set_current_text("");
        }
        else if (prompt_history_position >= maxNum) {
            prompt_history_position = maxNum;
            prompt_set_current_text("");
        }
        else {
            prompt_set_current_text(prompt_input_list[prompt_history_position]);
        }
    }
}

/* . */
void
prompt_input_next(void)
{
    if (prompt_history_size <= 0) {
        messagebox("critical", translate("Prompt Next Error"),
            translate("The prompt input is empty! Please report this as a bug!"));
        debug_message("The prompt input is empty! Please report this as a bug!");
    }
    else {
        prompt_history_position++;
        int maxNum = prompt_history_size;
        if (prompt_history_position < 0) {
            prompt_history_position = 0;
            prompt_set_current_text("");
        }
        else if (prompt_history_position >= maxNum) {
            prompt_history_position = maxNum;
            prompt_set_current_text("");
        }
        else {
            prompt_set_current_text(prompt_input_list[prompt_history_position]);
        }
    }
}

/* EmbVectorList */
EmbVectorList *
create_vector_list(void)
{
    EmbVectorList *list;
    list = (EmbVectorList*)malloc(sizeof(EmbVectorList));
    list->data = (EmbVector*)malloc(CHUNK_SIZE*sizeof(EmbVector));
    list->count = 0;
    list->size = CHUNK_SIZE;
    return list;
}

/* Checks if there is room to append a vector to the list, if there isn't then
 * resize it, then append the vector.
 */
void
append_vector_to_list(EmbVectorList *list, EmbVector v)
{
    if (list->count >= list->size - 1) {
        list->size += CHUNK_SIZE;
        list->data = (EmbVector*)realloc(list->data,
            (list->size)*sizeof(EmbVector));
    }
    list->data[list->count] = v;
    list->count++;
}

/* Test this */
void
remove_vector_from_list(EmbVectorList *list, int32_t position)
{
    if (list->size <= 0) {
        return;
    }
    memcpy(list->data + position + 1, list->data + position,
        (list->size - position)*sizeof(EmbVector));
    list->size--;
}

/* . */
void
free_vector_list(EmbVectorList *v)
{
    free(v->data);
    free(v);
}

/* EmbIdList */
EmbIdList *
create_id_list(void)
{
    EmbIdList *list;
    list = (EmbIdList*)malloc(sizeof(EmbIdList));
    list->data = (int32_t*)malloc(CHUNK_SIZE*sizeof(int32_t));
    list->count = 0;
    list->size = CHUNK_SIZE;
    return list;
}

/* Checks if there is room to append an id to the list, if there isn't then
 * resize it, then append the id.
 */
void
append_id_to_list(EmbIdList *list, int32_t i)
{
    if (list->count >= list->size - 1) {
        list->size += CHUNK_SIZE;
        list->data = (int32_t*)realloc(list->data,
            (list->size)*sizeof(EmbVector));
    }
    list->data[list->count] = i;
    list->count++;
}

bool
id_list_contains(EmbIdList *list, int32_t a)
{
    for (int i=0; i<list->count; i++) {
        if (list->data[i] == a) {
            return true;
        }
    }
    return false;
}

/* Test this. */
void
remove_id_from_list(EmbIdList *list, int32_t position)
{
    if (list->size <= 0) {
        return;
    }
    memcpy(list->data + position + 1, list->data + position,
        (list->size - position)*sizeof(int32_t));
    list->size --;
}

/* . */
void
free_id_list(EmbIdList *list)
{
    free(list->data);
    free(list);
}

/* Find index of integer in IntMap.
 * IntMaps have terminator symbols.
 */
int
find_int_map(IntMap *map, int key)
{
    for (int i=0; map[i].value != -1; i++) {
        if (map[i].key == key) {
            return map[i].value;
        }
    }
    return -1;
}

/* Find index of string in StringMap. */
int
find_in_map(StringMap *hash, int length, const char *key)
{
    for (int i=0; i<length; i++) {
        if (string_equal(hash[i].key, key)) {
            return i;
        }
    }
    return -1;
}

/* . */
void
nanosleep_(int time)
{
#if defined(WINDOWS)
    if (time/1000000 == 0) {
        Sleep(1);
    }
    else {
        Sleep(time/1000000);
    }
#elif defined(unix) || defined(__unix__) || defined(__APPLE__)
    usleep(time/1000);
#endif
}

/* . */
int
main(int argc, char* argv[])
{
    int i;
    int n_files = 0;
    char files_to_open[MAX_FILES][MAX_STRING_LENGTH];

    for (i = 1; i < argc; i++) {
        if (string_equal(argv[i], "-d") || string_equal(argv[i], "--debug")) {
            testing_mode = 1;
        }
        else if (string_equal(argv[i], "-h") || string_equal(argv[i], "--help")) {
            fprintf(stderr, help_msg);
            exitApp = true;
        }
        else if (string_equal(argv[i], "-v") || string_equal(argv[i], "--version")) {
            /* Print out version string and exit. */
            fprintf(stdout, "%s %s\n", _appName_, _appVer_);
            exitApp = true;
        }
        else if (argv[i][0] == '-') {
            fprintf(stderr, "ERROR: unrecognised flag \"%s\".\n", argv[i]);            
            exitApp = true;
        }
        else if (1 /* FIXME: QFile::exists(argv[i]) && emb_valid_file_format(argv[i])*/) {
            if (n_files >= MAX_FILES) {
                printf("ERROR: More files to open than MAX_FILES.");
                continue;
            }
            string_copy(files_to_open[n_files], argv[i]);
            n_files++;
        }
        else {
            fprintf(stderr, "ERROR: file \"%s\" not found.", argv[i]);
            exitApp = true;
        }
    }

    if (exitApp) {
        return 1;
    }

    if (!load_data()) {
        puts("Failed to load data.");
        return 2;
    }

    if (!glfwInit()) {
        puts("Failed to run glfwInit.");
        return 3;
    }

    global = create_script_env();
    int result = make_application(n_files, files_to_open);

    free_script_env(global);
    free(config);

    return result;
}

/* . */
void
run_testing(void)
{
    int i;
    nanosleep_(2000);
    int n = table_length(coverage_test);
    for (i=0; i<n; i++) {
        run_command_main(coverage_test[i]);
        nanosleep_(1000);
    }        
}

/* . */
ScriptValue
test_command(ScriptEnv *context)
{
    return script_true;
}

/* TODO: remove args command, use the command_data table
 */
int
argument_checks(ScriptEnv *context, int id)
{
    char s[200];
    int i;
    const char *function = command_data[id].command;
    const char *args = command_data[id].arguments;
    if (context->argumentCount != strlen(args)) {
        sprintf(s, "%s() requires %d arguments.", function, context->argumentCount);
        prompt_output(s);
        return 0;
    }
    for (i=0; args[i]; i++) {
        if (args[i] == 'r') {
            if (context->argument[i].type != SCRIPT_REAL) {
                sprintf(s,
                    "TYPE_ERROR, %s(): %s argument is not a real number.",
                    function, index_th_name[i]);
                prompt_output(s);
                return 0;
            }
            float variable = context->argument[i].r;
            if (isnan(variable)) {
                sprintf(s,
                    "TYPE_ERROR, %s(): %s argument is not a real number.",
                    function, index_th_name[i]);
                prompt_output(s);
                return 0;
            }
        }
        if (args[i] == 'i') {
            if (context->argument[i].type != SCRIPT_INT) {
                sprintf(s,
                    "TYPE_ERROR, %s(): %s argument is not an integer.",
                    function, index_th_name[i]);
                prompt_output(s);
                return 0;
            }
        }
        if (args[i] == 'b') {
            if (context->argument[i].type != SCRIPT_BOOL) {
                sprintf(s,
                    "TYPE_ERROR, %s(): %s argument is not a boolean.",
                    function, index_th_name[i]);
                prompt_output(s);
                return 0;
            }
        }
        if (args[i] == 's') {
            if (context->argument[i].type != SCRIPT_STRING) {
                sprintf(s,
                    "TYPE_ERROR, %s(): %s argument is not a string.",
                    function, index_th_name[i]);
                prompt_output(s);
                return 0;
            }
        }
    }
    return 1;
}

/* . */
char *
translate(const char *msg)
{
    return msg;
}

ScriptEnv *
create_script_env()
{
    ScriptEnv *context = (ScriptEnv*)malloc(sizeof(ScriptEnv));
    context->argumentCount = 0;
    return context;
}

/* . */
void
free_script_env(ScriptEnv* context)
{
    free(context);
}

/* . */
ScriptValue
script_bool(bool b)
{
    ScriptValue value;
    value.type = SCRIPT_BOOL;
    value.b = b;
    return value;
}

/* . */
ScriptValue
script_int(int i)
{
    ScriptValue value;
    value.type = SCRIPT_INT;
    value.i = i;
    return value;
}

/* . */
ScriptValue
script_real(EmbReal r)
{
    ScriptValue value;
    value.type = SCRIPT_REAL;
    value.r = r;
    return value;
}

/* . */
ScriptValue
script_string(char *s)
{
    ScriptValue value;
    value.type = SCRIPT_STRING;
    strncpy(value.s, s, MAX_STRING_LENGTH);
    return value;
}

/* Using stdarg we can pack arguments into the context using the above functions.
 *
 * https://pubs.opengroup.org/onlinepubs/009695399/basedefs/stdarg.h.html
 */
ScriptEnv *
pack(ScriptEnv *context, const char *fmt, ...)
{
    va_list a;
    int argno;
    context->argumentCount = 0;
    va_start(a, fmt);
    for (argno = 0; fmt[argno]; argno++) {
        switch (fmt[argno]) {
        case 's': {
            const char *s = va_arg(a, const char *);
            strncpy(context->argument[context->argumentCount].s, s, MAX_STRING_LENGTH);
            context->argument[context->argumentCount].type = SCRIPT_STRING;
            break;
        }
        case 'i': {
            int i = va_arg(a, int);
            context->argument[context->argumentCount].i = i;
            context->argument[context->argumentCount].type = SCRIPT_INT;
            break;
        }
        case 'r': {
            EmbReal r = va_arg(a, double);
            context->argument[context->argumentCount].r = r;
            context->argument[context->argumentCount].type = SCRIPT_REAL;
            break;
        }
        default:
            break;
        }
        context->argumentCount++;
    }
    va_end(a);
    return context;
}

/* Using a similar approach to pack we can now create a caller that also packs
 * the arguments.
 */
ScriptValue
call(ScriptEnv *context, char *cmd, ...)
{
    int id = get_command_id((char*)cmd);
    if (id < 0) {
        debug_message("ERROR: failed to find function %s", cmd);
        return script_false;
    }
    va_list a;
    int argno;
    context->argumentCount = 0;
    const char *fmt = command_data[id].arguments;
    va_start(a, cmd);
    for (argno = 0; fmt[argno]; argno++) {
        switch (fmt[argno]) {
        case 's': {
            const char *s = va_arg(a, const char *);
            strncpy(context->argument[context->argumentCount].s, s, MAX_STRING_LENGTH);
            context->argument[context->argumentCount].type = SCRIPT_STRING;
            break;
        }
        case 'i': {
            int i = va_arg(a, int);
            context->argument[context->argumentCount].i = i;
            context->argument[context->argumentCount].type = SCRIPT_INT;
            break;
        }
        case 'r': {
            EmbReal r = va_arg(a, double);
            context->argument[context->argumentCount].r = r;
            context->argument[context->argumentCount].type = SCRIPT_REAL;
            break;
        }
        default:
            break;
        }
        context->argumentCount++;
    }
    va_end(a);
    if (!argument_checks(context, id)) {
        debug_message("Failed argument checks.");
        return script_false;
    }
    return command_data[id].action(context);
}


/* . */
int
parse_floats(const char *line, float result[], int n)
{
    char substring[MAX_STRING_LENGTH];
    char *c;
    int i = 0;
    int pos = 0;
    for (c=line; *c; c++) {
        substring[pos] = *c;
        if (*c == ',' || *c == ' ') {
            substring[pos] = 0;
            result[i] = atof(substring);
            pos = 0;
            i++;
            if (i > n-1) {
                return -1;
            }
        }
        else {
            pos++;
        }
    }
    substring[pos] = 0;
    result[i] = atof(substring);
    return i+1;
}

int
parse_vector(const char *line, EmbVector *v)
{
    float v_[2];
    if (parse_floats(line, v_, 2) == 2) {
        return 0;
    }
    v->x = v_[0];
    v->y = v_[1];
    return 1;
}

/* . */
bool
valid_rgb(float r, float g, float b)
{
    if (isnan(r)) {
        return false;
    }
    if (isnan(g)) {
        return false;
    }
    if (isnan(b)) {
        return false;
    }
    if (r < 0 || r > 255) {
        return false;
    }
    if (g < 0 || g > 255) {
        return false;
    }
    if (b < 0 || b > 255) {
        return false;
    }
    return true;
}

/* This version of string_array_length does not protect against the END_SYMBOL
 * missing, because it is only to be used for compiled in constant tables.
 *
 * For string tables edited during run time, the END_SYMBOL is checked for during loops.
 */
int
table_length(char *s[])
{
    int i;
    for (i=0; ; i++) {
        if (s[i][0] == END_SYMBOL[0])
        if (s[i][1] == END_SYMBOL[1])
        if (s[i][2] == END_SYMBOL[2]) {
            break;
        }
    }
    return i;
}

/* . */
char *
load_file(char *fname)
{
    FILE *f = fopen(fname, "r");
    if (!f) {
        printf("ERROR: Failed to open \"%s\".\n", fname);
        return NULL;
    }
    fseek(f, 0, SEEK_END);
    size_t length = ftell(f);
    char *data = malloc(length+1);
    fseek(f, 0, SEEK_SET);
    int read_bytes = fread(data, 1, length, f);
    fclose(f);
    if (read_bytes != length) {
        printf("ERROR: Failed to read all the %ld bytes in the file \"%s\".\n",
            length,
            fname);
        return NULL;
    }
    return data;
}

/* . */
int
config_find(const char *key)
{
    int i;
    for (i=0; i<n_variables; i++) {
        if (string_equal(config[i].label, key)) {
            return i;
        }
    }
    return -1;
}

/* . */
char *
config_str(const char *key)
{
    int i = config_find(key);
    if (i >= 0) {
        return config[i].s;
    }
    return "ERROR: NOT FOUND";
}

/* . */
void
load_command_data(char *label)
{
    char key[MAX_STRING_LENGTH];
    sprintf(key, "%s.id", label);

    int id = atoi(config_str(key));

    sprintf(key, "%s.command", label);
    string_copy(command_data[id].command, config_str(key));

    sprintf(key, "%s.arguments", label);
    string_copy(command_data[id].arguments, config_str(key));

    sprintf(key, "%s.icon", label);
    string_copy(command_data[id].icon, config_str(key));

    sprintf(key, "%s.tooltip", label);
    string_copy(command_data[id].tooltip, config_str(key));

    sprintf(key, "%s.statustip", label);
    string_copy(command_data[id].statustip, config_str(key));

    sprintf(key, "%s.alias", label);
    string_copy(command_data[id].alias, config_str(key));

    sprintf(key, "%s.shortcut", label);
    string_copy(command_data[id].shortcut, config_str(key));

    sprintf(key, "%s.flags", label);
    int flags = atoi(config_str(key));
    command_data[id].flags = flags;
}

const char *config_table[] = {
    END_SYMBOL
};

/* Rather than loading necessary configuration data from file at load, it is
 * compiled into the program. However, the ability to change the UI as a
 * user, without re-compiling the program, can be preserved by overriding the string
 * tables from a custom "overrides.ini" file in the configuration directory.
 */
int
load_data(void)
{
    /* Load config_table into config ScriptValues. */
    for (n_variables=0; ; n_variables++) {
        if (string_equal(config_table[n_variables], END_SYMBOL)) {
            break;
        }
    }
    return 1;
    config = (ScriptValue*)malloc(n_variables * sizeof(ScriptValue));
    for (int line_no=0; line_no<n_variables; line_no++) {
        char line[200];
        string_copy(line, config_table[line_no]);
        int eq_pos = 0;
        for (int i=0; i<200; i++) {
            if (line[i] == '=') {
                eq_pos = i;
                break;
            }
        }
        line[eq_pos] = 0;
        string_copy(config[line_no].label, line);
        string_copy(config[line_no].s, line+eq_pos+1);
    }

    /* Identify the labels of commands. */
    for (int i=0; i<n_variables; i++) {
        int offset = strlen(config[i].label) - 4;
        if (string_equal(config[i].label + offset, "type")) {
            if (string_equal(config[i].s, "command")) {
                char label[MAX_STRING_LENGTH];
                snprintf(label, offset, "%s", config[i].label);
                load_command_data(label);
            }
        }
    }

    /* If the overrides file exists, load it on top of the default data. */
    FILE *file = fopen("overrides.ini", "r");
    if (file) {
        fclose(file);
        char *data = malloc(n_variables*100);
        data = load_file("overrides.ini");
        free(data);
    }
    return 1;
}

/* . */
int
load_settings(const char *appDir, const char *fname)
{
    return 1;
}

/* The file fname needs to be read from the users home directory to ensure it is writable.
 */
int
save_settings(char *appDir, char *fname)
{
    FILE *file = fopen(fname, "w");
    if (!file) {
        return 0;
    }

    for (int i=0; i<N_SETTINGS; i++) {
        switch (setting[i].setting.type) {
        case SCRIPT_INT:
            fprintf(file, "%s = %d\r\n", settings_data[6*i+0], setting[i].setting.i);
            break;
        case SCRIPT_REAL:
            fprintf(file, "%s = %f\r\n", settings_data[6*i+0], setting[i].setting.r);
            break;
        case SCRIPT_STRING:
            fprintf(file, "%s = \"%s\"\r\n", settings_data[6*i+0], setting[i].setting.s);
            break;
        case SCRIPT_BOOL:
            fprintf(file, "%s = %d\r\n", settings_data[6*i+0], setting[i].setting.b);
            break;
        default:
            break;
        }
    }

    fclose(file);
    return 1;
}

/* . */
int
get_command_id(char *name)
{
    int i;
    for (i = 0; i < N_ACTIONS; i++) {
        if (string_equal(command_data[i].command, name)) {
            return i;
        }
    }
    return -2;
}

/* TODO: Before saving to a stitch only format, Embroidermodder needs
 *       to calculate the optimal path to minimize jump stitches. Also
 *       based upon which layer needs to be stitched first,
 *       the path to the next object needs to be hidden beneath fills
 *       that will come later. When finding the optimal path, we need
 *       to take into account the color of the thread, as we do not want
 *       to try to hide dark colored stitches beneath light colored fills.
 */
bool
pattern_save(EmbPattern *pattern, char *fileName)
{
    char message[MAX_STRING_LENGTH];
    sprintf(message, "SaveObject save(%s)", fileName);
    prompt_output(message);

    bool writeSuccessful = false;

    int formatType = emb_identify_format(fileName);
    if (formatType <= 0) { /* EMBFORMAT_UNSUPPORTED */
        return false;
    }

    if (!pattern) {
        prompt_output("The EmbPattern has not been allocated.");
        return false;
    }

    /* TODO: handle EMBFORMAT_STCHANDOBJ also */
    if (formatType == EMBFORMAT_STITCHONLY) {
        /* emb_pattern_movePolylinesToStitchList(pattern); */
        /* TODO: handle all objects like this */
    }

    writeSuccessful = emb_pattern_write(pattern, fileName, formatType);
    if (!writeSuccessful) {
        sprintf(message, "Writing file %s was unsuccessful", fileName);
        prompt_output(message);
    }

    /* TODO: check the embLog for errors and if any exist, report them. */

    emb_pattern_free(pattern);

    return writeSuccessful;
}

/* . */
bool
int32_underflow(int64_t a, int64_t b)
{
    int64_t c;
    assert(LLONG_MAX>INT_MAX);
    c = (int64_t)a-b;
    if (c < INT_MIN || c > INT_MAX) {
        return true;
    }
    return false;
}

/* . */
bool
int32_overflow(int64_t a, int64_t b)
{
    int64_t c;
    assert(LLONG_MAX>INT_MAX);
    c = (int64_t)a+b;
    if (c < INT_MIN || c > INT_MAX) {
        return true;
    }
    return false;
}

/* . */
int
round_to_multiple(bool roundUp, int numToRound, int multiple)
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
    /* else round down */
    if (numToRound < 0 && !roundUp) {
        return numToRound - multiple - remainder;
    }
    return numToRound - remainder;
}

/* Formats each message with a timestamp. */
void
debug_message(const char *msg, ...)
{
    char buffer[MAX_STRING_LENGTH], fname[MAX_STRING_LENGTH];
    time_t t;
    struct tm* tm_info;
    sprintf(fname, "debug.log");
    FILE *f = fopen(fname, "a");
    if (!f) {
        printf("Failed to write to debug.log.");
        return;
    }
    t = time(NULL);
    tm_info = localtime(&t);
    strftime(buffer, 26, "%Y-%m-%d %H:%M:%S", tm_info);
    fprintf(f, "%s ", buffer);

    va_list args;
    va_start(args, msg);
    vfprintf(f, msg, args);
    va_end(args);

    fprintf(f, "\n");
    fclose(f);
}

/* . */
ScriptValue *
setting_ptr(int key, int mode)
{
    ScriptValue *set_mode;
    switch (mode) {
    case SETTING_DIALOG: {
        set_mode = &(setting[key].dialog);
        break;
    }
    case SETTING_PREVIEW: {
        set_mode = &(setting[key].preview);
        break;
    }
    case SETTING_ACCEPT: {
        set_mode = &(setting[key].dialog);
        break;
    }
    default:
    case SETTING_SETTING: {
        set_mode = &(setting[key].setting);
        break;
    }
    }
    return set_mode;
}

/* . */
void
copy_setting(int key, int dst, int src)
{
    ScriptValue *dst_set = setting_ptr(key, dst);
    ScriptValue *src_set = setting_ptr(key, src);
    switch (setting[src].setting.type) {
    case SCRIPT_INT:
        dst_set->i = src_set->i;
        break;
    case SCRIPT_REAL:
        dst_set->r = src_set->r;
        break;
    case SCRIPT_STRING:
        string_copy(dst_set->s, src_set->s);
        break;
    case SCRIPT_BOOL:
        dst_set->b = src_set->b;
        break;
    default:
        break;
    }
}

/* Tests if char * matches a fixed string, often from compiled-in program
 * data.
 */
bool
string_equal(char *a, const char *b)
{
    return (strncmp(a, b, MAX_STRING_LENGTH) == 0);
}

/* . */
const char *
platform_string(void)
{
    /* TODO: Append QSysInfo to string where applicable. */
    debug_message("Platform: %s", os);
    return os;
}

/* . */
bool
valid_file_format(char *fileName)
{
    if (emb_identify_format(fileName) >= 0) {
        return true;
    }
    return false;
}

/* . */
int
get_id(char *data[], char *label)
{
    int id;
    int n = table_length(data);
    for (id=0; id<n; id++) {
        if (string_equal(data[id], label)) {
            return id;
        }
    }
    return -1;
}

/* . */
void
update_editors(int32_t obj)
{
    ObjectCore *core = obj_get_core(obj);
    EmbGeometry *g = core->geometry;
    switch (g->type) {
    case EMB_ARC: {
        EmbVector center = obj_center(core);
        EmbVector start = g->object.arc.start;
        EmbVector end = g->object.arc.end;
        update_lineedit_num("ArcCenterX", center.x, false);
        update_lineedit_num("ArcCenterY", -center.y, false);
        update_lineedit_num("ArcRadius", obj_radius(core), false);
        update_lineedit_num("ArcStartAngle", emb_start_angle(g), true);
        update_lineedit_num("ArcEndAngle", emb_end_angle(g), true);
        update_lineedit_num("ArcStartX", start.x, false);
        update_lineedit_num("ArcStartY", -start.y, false);
        update_lineedit_num("ArcEndX", end.x, false);
        update_lineedit_num("ArcEndY", -end.y, false);
        update_lineedit_num("ArcArea", emb_area(g), false);
        update_lineedit_num("ArcLength", emb_arc_length(g), false);
        update_lineedit_num("ArcChord", emb_chord(g), false);
        update_lineedit_num("ArcIncAngle", emb_included_angle(g), true);
        update_lineedit_bool("ArcClockwise", emb_clockwise(g), true);
        break;
    }
    case OBJ_BLOCK: {
        /* TODO: load block data */
        break;
    }
    case EMB_CIRCLE: {
        EmbVector center = obj_center(core);
        update_lineedit_num("CircleCenterX", center.x, false);
        update_lineedit_num("CircleCenterY", -center.y, false);
        update_lineedit_num("CircleRadius", obj_radius(core), false);
        update_lineedit_num("CircleDiameter", obj_diameter(core), false);
        update_lineedit_num("CircleArea", emb_area(g), false);
        update_lineedit_num("CircleCircumference", obj_circumference(core), false);
        break;
    }
    case OBJ_DIMALIGNED: {
        /* TODO: load aligned dimension data */
        break;
    }
    case OBJ_DIMANGULAR: {
        /* TODO: load angular dimension data */
        break;
    }
    case OBJ_DIMARCLENGTH: {
        /* TODO: load arclength dimension data */
        break;
    }
    case OBJ_DIMDIAMETER: {
        /* TODO: load diameter dimension data */
        break;
    }
    case OBJ_DIMLEADER: {
        /* TODO: load leader dimension data */
        break;
    }
    case OBJ_DIMLINEAR: {
        /* TODO: load linear dimension data */
        break;
    }
    case OBJ_DIMORDINATE: {
        /* TODO: load ordinate dimension data */
        break;
    }
    case OBJ_DIMRADIUS: {
        /* TODO: load radius dimension data */
        break;
    }
    case EMB_ELLIPSE: {
        EmbVector center = obj_center(core);
        update_lineedit_num("EllipseCenterX", center.x, false);
        update_lineedit_num("EllipseCenterY", -center.y, false);
        update_lineedit_num("EllipseRadiusMajor", emb_radius_major(g), false);
        update_lineedit_num("EllipseRadiusMinor", emb_radius_minor(g), false);
        update_lineedit_num("EllipseDiameterMajor", emb_diameter_major(g), false);
        update_lineedit_num("EllipseDiameterMinor", emb_diameter_minor(g), false);
        break;
    }
    case EMB_IMAGE: {
        debug_message("TODO: load image data");
        break;
    }
    case OBJ_INFINITELINE: {
        debug_message("TODO: load infinite line data");
        break;
    }
    case EMB_LINE: {
        EmbVector point1 = obj_end_point_1(core);
        EmbVector point2 = obj_end_point_2(core);
        EmbVector delta = emb_vector_subtract(point2, point1);
        update_lineedit_num("LineStartX", point1.x, false);
        update_lineedit_num("LineStartY", -point1.y, false);
        update_lineedit_num("LineEndX", point2.x, false);
        update_lineedit_num("LineEndY", -point2.y, false);
        update_lineedit_num("LineDeltaX", delta.x, false);
        update_lineedit_num("LineDeltaY", -delta.y, false);
        update_lineedit_num("LineAngle", emb_angle(g), true);
        update_lineedit_num("LineLength", obj_length(core), false);
        break;
    }
    case EMB_PATH: {
        debug_message("TODO: load path data");
        break;
    }
    case EMB_POINT: {
        update_lineedit_num("PointX", obj_x(core), false);
        update_lineedit_num("PointY", -obj_y(core), false);
        break;
    }
    case EMB_POLYGON: {
        debug_message("TODO: load polygon data");
        break;
    }
    case EMB_POLYLINE: {
        debug_message("TODO: load polyline data");
        break;
    }
    case OBJ_RAY: {
        debug_message("TODO: load ray data");
        break;
    }
    case EMB_RECT: {
        EmbVector corn1 = obj_top_left(core);
        EmbVector corn2 = obj_top_right(core);
        EmbVector corn3 = obj_bottom_left(core);
        EmbVector corn4 = obj_bottom_right(core);

        update_lineedit_num("RectangleCorner1X", corn1.x, false);
        update_lineedit_num("RectangleCorner1Y", -corn1.y, false);
        update_lineedit_num("RectangleCorner2X", corn2.x, false);
        update_lineedit_num("RectangleCorner2Y", -corn2.y, false);
        update_lineedit_num("RectangleCorner3X", corn3.x, false);
        update_lineedit_num("RectangleCorner3Y", -corn3.y, false);
        update_lineedit_num("RectangleCorner4X", corn4.x, false);
        update_lineedit_num("RectangleCorner4Y", -corn4.y, false);
        update_lineedit_num("RectangleWidth", emb_width(g), false);
        update_lineedit_num("RectangleHeight", -emb_height(g), false);
        update_lineedit_num("RectangleArea", emb_area(g), false);
        break;
    }
    case EMB_TEXT_MULTI: {
        debug_message("TODO: load multiline text data");
        break;
    }
    case EMB_TEXT_SINGLE: {
        update_line_edit_str_if_varies("TextSingleContents", core->text);
        update_font_combo_box_str_if_varies(core->textFont);
        update_lineedit_str("TextSingleJustify", core->textJustify,
            objectTextJustifyList);
        update_lineedit_num("TextSingleHeight", core->textSize, false);
        update_lineedit_num("TextSingleRotation", -core->rotation, true);
        update_lineedit_num("TextSingleX", obj_x(core), false);
        update_lineedit_num("TextSingleY", -obj_y(core), false);
        update_lineedit_bool("TextSingleBackward", core->textBackward, true);
        update_lineedit_bool("TextSingleUpsideDown", core->textUpsideDown, true);
        break;
    }
    default:
        break;
    }
}

void
edit_field(int32_t id, const char *label, const char *text)
{
    ObjectCore *core = obj_get_core(id);
    EmbGeometry *g = core->geometry;
    switch(core->geometry->type) {
    case EMB_ARC: {
        if (!strcmp(label, "lineEditArcCenterX")) {
            EmbVector arc = obj_center(core);
            EmbVector center;
            center.x = atof(text);
            center.y = arc.y;
            obj_set_center(core, center);
        }
        if (!strcmp(label, "lineEditArcCenterY")) {
            EmbVector arc = obj_center(core);
            EmbVector center;
            center.x = arc.x;
            center.y = -atof(text);
            obj_set_center(core, center);
        }
        if (!strcmp(label, "lineEditArcRadius")) {
            emb_set_radius(g, atof(text));
        }
        if (!strcmp(label, "lineEditArcStartAngle")) {
            emb_set_start_angle(g, atof(text));
        }
        if (!strcmp(label, "lineEditArcEndAngle")) {
            emb_set_end_angle(g, atof(text));
        }
        break;
    }
    case OBJ_BLOCK: {
        /* TODO: field editing. */
        break;
    }
    case EMB_CIRCLE: {
        if (!strcmp(label, "lineEditCircleCenterX")) {
            obj_set_center_x(core, atof(text));
        }
        if (!strcmp(label, "lineEditCircleCenterY")) {
            obj_set_center_y(core, -atof(text));
        }
        if (!strcmp(label, "lineEditCircleRadius")) {
            emb_set_radius(core->geometry, atof(text));
        }
        if (!strcmp(label, "lineEditCircleDiameter")) {
            emb_set_diameter(core->geometry, atof(text));
        }
        if (!strcmp(label, "lineEditCircleArea")) {
            emb_set_area(core->geometry, atof(text));
        }
        if (!strcmp(label, "lineEditCircleCircumference")) {
            emb_set_circumference(core->geometry, atof(text));
        }
        break;
    }
    case OBJ_DIMALIGNED: {
        /* TODO: field editing. */
        break;
    }
    case OBJ_DIMANGULAR: {
        /* TODO: field editing. */
        break;
    }
    case OBJ_DIMARCLENGTH: {
        /* TODO: field editing. */
        break;
    }
    case OBJ_DIMDIAMETER: {
        /* TODO: field editing. */
        break;
    }
    case OBJ_DIMLEADER: {
        /* TODO: field editing. */
        break;
    }
    case OBJ_DIMLINEAR: {
        /* TODO: field editing. */
        break;
    }
    case OBJ_DIMORDINATE: {
        /* TODO: field editing. */
        break;
    }
    case OBJ_DIMRADIUS: {
        /* TODO: field editing. */
        break;
    }
    case OBJ_ELLIPSE: {
        if (!strcmp(label, "lineEditEllipseCenterX")) {
            obj_set_center_x(core, atof(text));
        }
        if (!strcmp(label, "lineEditEllipseCenterY")) {
            obj_set_center_y(core, -atof(text));
        }
        if (!strcmp(label, "lineEditEllipseRadiusMajor")) {
            emb_set_radius_major(g, atof(text));
        }
        if (!strcmp(label, "lineEditEllipseRadiusMinor")) {
            emb_set_radius_minor(g, atof(text));
        }
        if (!strcmp(label, "lineEditEllipseDiameterMajor")) {
            emb_set_diameter_major(g, atof(text));
        }
        if (!strcmp(label, "lineEditEllipseDiameterMinor")) {
            emb_set_diameter_minor(g, atof(text));
        }
        break;
    }
    case OBJ_IMAGE: {
        /* TODO: field editing. */
        break;
    }
    case OBJ_INFINITELINE: {
        /* TODO: field editing. */
        break;
    }
    case OBJ_LINE: { 
        if (!strcmp(label, "lineEditLineStartX")) {
            obj_set_x1(core, atof(text));
        }
        if (!strcmp(label, "lineEditLineStartY")) {
            obj_set_y1(core, -atof(text));
        }
        if (!strcmp(label, "lineEditLineEndX")) {
            obj_set_x2(core, atof(text));
        }
        if (!strcmp(label, "lineEditLineEndY")) {
            obj_set_y2(core, -atof(text));
        }
        break;
    }
    case OBJ_PATH: {
        /* TODO: field editing. */
        break;
    }
    case OBJ_POINT: {
        if (!strcmp(label, "lineEditPointX")) {
            obj_set_x(core, atof(text));
        }
        if (!strcmp(label, "lineEditPointY")) {
            obj_set_y(core, -atof(text));
        }
        break;
    }
    case OBJ_POLYGON: {
        /* TODO: field editing. */
        break;
    }
    case OBJ_POLYLINE: {
        /* TODO: field editing. */
        break;
    }
    case OBJ_RAY: {
        /* TODO: field editing. */
        break;
    }
    case OBJ_RECTANGLE: {
        /* TODO: field editing. */
        break;
    }
    case OBJ_TEXTMULTI: {
        /* TODO: field editing. */
        break;
    }
    case OBJ_TEXTSINGLE: {
        /* TODO: field editing */
        if (!strcmp(label, "lineEditTextSingleContents")) {
            obj_set_text(core, text);
            break;
        }
        if (!strcmp(label, "lineEditTextSingleHeight")) {
            obj_set_text_size(core, atof(text));
            break;
        }
        if (!strcmp(label, "lineEditTextSingleRotation")) {
            obj_set_rotation(core, atof(text));
            break;
        }
        if (!strcmp(label, "lineEditTextSingleX")) {
            obj_set_x(core, atof(text));
            break;
        }
        if (!strcmp(label, "lineEditTextSingleY")) {
            obj_set_y(core, atof(text));
            break;
        }
        break;
    }
    default:
        break;
    }
}

/* This needs a int to int table. */
void
show_groups(int objType)
{
    switch (objType) {
    case EMB_ARC:
        show_widget("GB_GEOM_ARC", WIDGET_GROUP_BOX);
        show_widget("GB_MISC_ARC", WIDGET_GROUP_BOX);
        break;
    case OBJ_BLOCK:
        show_widget("GB_GEOM_BLOCK", WIDGET_GROUP_BOX);
        break;
    case EMB_CIRCLE:
        show_widget("GB_GEOM_CIRCLE", WIDGET_GROUP_BOX);
        break;
    case OBJ_DIMALIGNED:
        show_widget("GB_GEOM_DIMALIGNED", WIDGET_GROUP_BOX);
        break;
    case OBJ_DIMANGULAR:
        show_widget("GB_GEOM_DIMANGULAR", WIDGET_GROUP_BOX);
        break;
    case OBJ_DIMARCLENGTH:
        show_widget("GB_GEOM_DIMARCLENGTH", WIDGET_GROUP_BOX);
        break;
    case OBJ_DIMDIAMETER:
        show_widget("GB_GEOM_DIMDIAMETER", WIDGET_GROUP_BOX);
        break;
    case OBJ_DIMLEADER:
        show_widget("GB_GEOM_DIMLEADER", WIDGET_GROUP_BOX);
        break;
    case OBJ_DIMLINEAR:
        show_widget("GB_GEOM_DIMLINEAR", WIDGET_GROUP_BOX);
        break;
    case OBJ_DIMORDINATE:
        show_widget("GB_GEOM_DIMORDINATE", WIDGET_GROUP_BOX);
        break;
    case OBJ_DIMRADIUS:
        show_widget("GB_GEOM_DIMRADIUS", WIDGET_GROUP_BOX);
        break;
    case EMB_ELLIPSE:
        show_widget("GB_GEOM_ELLIPSE", WIDGET_GROUP_BOX);
        break;
    case EMB_IMAGE:
        show_widget("GB_GEOM_IMAGE", WIDGET_GROUP_BOX);
        show_widget("GB_MISC_IMAGE", WIDGET_GROUP_BOX);
        break;
    case OBJ_INFINITELINE:
        show_widget("GB_GEOM_INFINITE_LINE", WIDGET_GROUP_BOX);
        break;
    case EMB_LINE:
        show_widget("GB_GEOM_LINE", WIDGET_GROUP_BOX);
        break;
    case EMB_PATH:
        show_widget("GB_GEOM_PATH", WIDGET_GROUP_BOX);
        show_widget("GB_MISC_PATH", WIDGET_GROUP_BOX);
        break;
    case EMB_POINT:
        show_widget("GB_GEOM_POINT", WIDGET_GROUP_BOX);
        break;
    case EMB_POLYGON:
        show_widget("GB_GEOM_POLYGON", WIDGET_GROUP_BOX);
        break;
    case EMB_POLYLINE:
        show_widget("GB_GEOM_POLYLINE", WIDGET_GROUP_BOX);
        show_widget("GB_MISC_POLYLINE", WIDGET_GROUP_BOX);
        break;
    case OBJ_RAY:
        show_widget("GB_GEOM_RAY", WIDGET_GROUP_BOX);
        break;
    case EMB_RECT:
        show_widget("GB_GEOM_RECT", WIDGET_GROUP_BOX);
        break;
    case EMB_TEXT_MULTI:
        show_widget("GB_GEOM_TEXT_MULTI", WIDGET_GROUP_BOX);
        break;
    case EMB_TEXT_SINGLE:
        show_widget("GB_TEXT_TEXT_SINGLE", WIDGET_GROUP_BOX);
        show_widget("GB_GEOM_TEXT_SINGLE", WIDGET_GROUP_BOX);
        show_widget("GB_MISC_TEXT_SINGLE", WIDGET_GROUP_BOX);
        break;
    default:
        break;
    }
}

void
spin_box_display_select_box_alpha_changed(int value)
{
    setting[DISPLAY_SELECTBOX_ALPHA].preview.i = value;
    update_all_view_select_box_colors(
        setting[DISPLAY_SELECTBOX_LEFT_COLOR].accept.i,
        setting[DISPLAY_SELECTBOX_LEFT_FILL].accept.i,
        setting[DISPLAY_SELECTBOX_RIGHT_COLOR].accept.i,
        setting[DISPLAY_SELECTBOX_RIGHT_FILL].accept.i,
        setting[DISPLAY_SELECTBOX_ALPHA].preview.i);
}

/* . */
void
combo_box_prompt_font_family_changed(EmbString family)
{
    string_copy(setting[PROMPT_FONT_FAMILY].preview.s, family);
    set_prompt_font_family(setting[PROMPT_FONT_FAMILY].preview.s);
}

/* . */
void
combo_box_prompt_font_style_changed(EmbString style)
{
    string_copy(setting[PROMPT_FONT_STYLE].preview.s, style);
    set_prompt_font_style(setting[PROMPT_FONT_STYLE].preview.s);
}

/* . */
void
spin_box_prompt_font_size_changed(int value)
{
    setting[PROMPT_FONT_SIZE].preview.i = value;
    set_prompt_font_size(setting[PROMPT_FONT_SIZE].preview.i);
}

/* . */
void
check_box_lwt_real_render_changed(int checked)
{
    setting[LWT_REAL_RENDER].preview.b = checked;
    statusbar_toggle("REAL", setting[LWT_REAL_RENDER].preview.b);
}

/* NOTE: General group will never be hidden. */
void
hide_all_groups(void)
{
    for (int i=0; group_box_list[i].id >= 0; i++) {
        if (group_box_list[i].id > GB_GENERAL) {
            hide_widget(group_box_list[i].key, WIDGET_GROUP_BOX);
        }
    }
}

/* . */
void
clear_all_fields(void)
{
    int n = table_length(editor_list);
    for (int i=0; i<n; i++) {
        widget_clear(editor_list[i], WIDGET_LINEEDIT);
    }
    n = table_length(combobox_list);
    for (int i=0; i<n; i++) {
        widget_clear(combobox_list[i], WIDGET_COMBOBOX);
    }
    clear_font_combobox();
}

/* . */
void
checkBoxGridCenterOnOriginStateChanged(int checked)
{
    setting[GRID_CENTER_ON_ORIGIN].dialog.b = checked;

    set_enabled_group(center_on_origin_group, !checked);
}

/* . */
void
checkBoxLwtShowLwtStateChanged(int checked)
{
    setting[LWT_SHOW_LWT].preview.b = checked;
    statusbar_toggle("LWT", setting[LWT_SHOW_LWT].preview.b);
    set_enabled("checkBoxRealRender", setting[LWT_SHOW_LWT].preview.b);
}

/* . */
void
accept_settings(void)
{
    if (setting[GRID_COLOR_MATCH_CROSSHAIR].dialog.b) {
        setting[GRID_COLOR].dialog.i = setting[DISPLAY_CROSSHAIR_COLOR].accept.i;
    }

    for (int i=0; preview_to_dialog[i] != TERMINATOR_SYMBOL; i++) {
        copy_setting(preview_to_dialog[i], SETTING_DIALOG, SETTING_PREVIEW);
    }

    for (int i=0; accept_to_dialog[i] != TERMINATOR_SYMBOL; i++) {
        copy_setting(accept_to_dialog[i], SETTING_DIALOG, SETTING_ACCEPT);
    }

    for (int i=0; i < N_SETTINGS; i++) {
        copy_setting(i, SETTING_SETTING, SETTING_DIALOG);
    }

    /* Make sure the user sees the changes applied immediately. */
    update_view();

    write_settings();
}

/* . */
void
comboBoxGridTypeCurrentIndexChanged(const char *type)
{
    string_copy(setting[GRID_TYPE].dialog.s, type);

    bool visibility = !strcmp(type, "Circular");
    set_visibility_group(rectangular_grid_group, !visibility);
    set_visibility_group(circular_grid_group, visibility);
}

/* . */
void
checkBoxGridColorMatchCrossHairStateChanged(int checked)
{
    setting[GRID_COLOR_MATCH_CROSSHAIR].dialog.b = checked;
    if (setting[GRID_COLOR_MATCH_CROSSHAIR].dialog.b) {
        update_all_view_grid_colors(setting[DISPLAY_CROSSHAIR_PERCENT].accept.i);
    }
    else {
        update_all_view_grid_colors(setting[GRID_COLOR].accept.i);
    }

    set_enabled("labelGridColor", !setting[GRID_COLOR_MATCH_CROSSHAIR].dialog.b);
    set_enabled("buttonGridColor", !setting[GRID_COLOR_MATCH_CROSSHAIR].dialog.b);
}

/* . */
void
checkBoxGridLoadFromFileStateChanged(int checked)
{
    setting[GRID_LOAD_FROM_FILE].dialog.b = checked;

    bool dont_load = !setting[GRID_LOAD_FROM_FILE].dialog.b;
    set_enabled_group(grid_load_from_file_group, dont_load);

    bool use_this_origin = !setting[GRID_LOAD_FROM_FILE].dialog.b && !setting[GRID_CENTER_ON_ORIGIN].dialog.b;
    set_enabled_group(defined_origin_group, use_this_origin);
}

/* . */
void
doc_init(int32_t doc)
{
    DocumentData *data = doc_data(doc);

    data->selectedItems = create_id_list();
    data->rubberRoomList = create_id_list();
    data->previewObjectList = create_id_list();
    data->spareRubberList = create_id_list();
    data->previewObjectItemGroup = create_id_list();
    data->pasteObjectItemGroup = create_id_list();
    data->hashDeletedObjects = create_id_list();

    data->selectedGripPoints = create_vector_list();

    /* NOTE: This has to be done before setting mouse tracking.
     * TODO: Review OpenGL for Qt5 later
     * if (get_bool(DISPLAY_USE_OPENGL)) {
     *     debug_message("Using OpenGL...");
     *     setViewport(new QGLWidget(QGLFormat(QGL::DoubleBuffer)));
     * }

     * TODO: Review RenderHints later
     * setRenderHint(QPainter::Antialiasing, get_bool(DISPLAY_RENDERHINT_AA));
     * setRenderHint(QPainter::TextAntialiasing, get_bool(DISPLAY_RENDERHINT_TEXT_AA));
     * setRenderHint(QPainter::SmoothPixmapTransform, get_bool(DISPLAY_RENDERHINT_SMOOTHPIX));
     * setRenderHint(QPainter::HighQualityAntialiasing, get_bool(DISPLAY_RENDERHINT_HIGH_AA));
     * setRenderHint(QPainter::NonCosmeticDefaultPen, get_bool(DISPLAY_RENDERHINT_NONCOSMETIC));

     * NOTE: FullViewportUpdate MUST be used for both the GL and Qt renderers.
     * NOTE: Qt renderer will not draw the foreground properly if it isnt set.
     */

    data->panDistance = 10; /* TODO: should there be a setting for this??? */

    data->properties[VIEW_GRIPPING] = false;
    data->properties[VIEW_RAPID_MOVING] = false;
    data->previewMode = PREVIEW_NULL;
    data->previewData = 0;
    data->properties[VIEW_PREVIEWING] = false;
    data->properties[VIEW_PASTING] = false;
    data->properties[VIEW_MOVING] = false;
    data->properties[VIEW_SELECTING] = false;
    data->properties[VIEW_ZOOMING] = false;
    data->properties[VIEW_PANNING_RT] = false;
    data->properties[VIEW_PANNING_POINT] = false;
    data->properties[VIEW_PANNING] = false;
    data->qSnapActive = false;
    data->qSnapToggle = false;

    data->qsnapLocatorColor = get_int(QSNAP_LOCATOR_COLOR);
    data->qsnapLocatorSize = get_int(QSNAP_LOCATOR_SIZE);
    data->qsnapApertureSize = get_int(QSNAP_APERTURE_SIZE);
    data->gripColorCool = get_int(SELECTION_COOLGRIP_COLOR);
    data->gripColorHot = get_int(SELECTION_HOTGRIP_COLOR);
    data->gripSize = get_int(SELECTION_GRIP_SIZE);
    data->pickBoxSize = get_int(SELECTION_PICKBOX_SIZE);

    /* TODO: set up config */
    data->properties[VIEW_RULER] = true;
    data->properties[VIEW_GRID] = true;
    data->properties[VIEW_ORTHO] = false;
    data->properties[VIEW_POLAR] = false;
    data->properties[VIEW_LWT] = false;
    data->properties[VIEW_RULER] = true;

    /* Randomize the hot grip location initially so it's not located at (0,0). */
    srand(time(NULL));
    data->sceneGripPoint = emb_vector(rand()*1000, rand()*1000);

    string_copy(data->curLayer, "0");
    data->curColor = 0; /* TODO: color ByLayer */
    string_copy(data->curLineType, "ByLayer");
    string_copy(data->curLineWeight, "ByLayer");

    data->pattern = emb_pattern_create();
    if (!data->pattern) {
        printf("Could not allocate memory for embroidery pattern\n");
        exit(1);
    }
}

/* TODO: review for memory leaks. */
void
free_doc(int32_t doc)
{
    DocumentData *data = doc_data(doc);

    document_memory[doc] = false;

    /* Prevent memory leaks by deleting any objects that were removed from the scene */
    free_objects(data->hashDeletedObjects);

    /* Prevent memory leaks by deleting any unused instances. */
    free_objects(data->previewObjectList);

    free_id_list(data->selectedItems);
    free_id_list(data->rubberRoomList);
    free_id_list(data->previewObjectList);
    free_id_list(data->spareRubberList);
    free_id_list(data->hashDeletedObjects);

    free_vector_list(data->selectedGripPoints);
}

/* . */
void
doc_clear_selection(int32_t doc)
{
    if (doc >= numOfDocs) {
        return;
    }
    DocumentData *data = doc_data(doc);
    data->selectedItems->count = 0;
}

/* . */
void
doc_zoom_window(int32_t doc)
{
    DocumentData *data = doc_data(doc);
    data->properties[VIEW_ZOOMING] = true;
    data->properties[VIEW_SELECTING] = false;
    doc_clear_selection(doc);
}

/* . */
void
doc_preview_on(int32_t doc, int clone, int mode, EmbReal x, EmbReal y, EmbReal data_)
{
    debug_message("View previewOn()");
    DocumentData *data = doc_data(doc);
    doc_preview_off(doc); /* Free the old objects before creating new ones */

    data->previewMode = mode;

    /* Create new objects and add them to the scene in an item group. */
    if (clone == PREVIEW_CLONE_SELECTED) {
        copy_object_list(data->previewObjectList, data->selectedItems);
    }
    else if (clone == PREVIEW_CLONE_RUBBER) {
        copy_object_list(data->previewObjectList, data->rubberRoomList);
    }
    else {
        return;
    }
    //FIXME: data->previewObjectItemGroup = documents[doc]->gscene->createItemGroup(documents[doc]->previewObjectList);

    if (data->previewMode == PREVIEW_MOVE ||
       data->previewMode == PREVIEW_ROTATE ||
       data->previewMode == PREVIEW_SCALE) {
        data->previewPoint.x = x;
        data->previewPoint.y = y; /* NOTE: Move: basePt; Rotate: basePt; Scale: basePt; */
        data->previewData = data_;           /* NOTE: Move: unused; Rotate: refAngle; Scale: refFactor; */
        data->properties[VIEW_PREVIEWING] = true;
    }
    else {
        data->previewMode = PREVIEW_NULL;
        data->previewPoint = emb_vector(0.0, 0.0);
        data->previewData = 0;
        data->properties[VIEW_PREVIEWING] = false;
    }

    doc_update(doc);
}

/* . */
void
doc_preview_off(int32_t doc)
{
    /* Prevent memory leaks by deleting any unused instances */
    DocumentData *data = doc_data(doc);
    free_objects(data->previewObjectList);
    data->previewObjectList->count = 0;

    if (data->previewObjectItemGroup->count > 0) {
        free_objects(data->previewObjectItemGroup);
        data->previewObjectList->count = 0;
    }

    data->properties[VIEW_PREVIEWING] = false;

    doc_update(doc);
}

/* . */
void
doc_spare_rubber(int32_t doc, int64_t id)
{
    DocumentData *data = doc_data(doc);
    append_id_to_list(data->spareRubberList, id);
}

/* . */
void
update_color_linetype_lineweight(void)
{
}

/* . */
void
doc_cut(int32_t doc)
{
    DocumentData *data = doc_data(doc);
    if (data->selectedItems->count <= 0) {
        messagebox("information", translate("Cut Preselect"),
            translate("Preselect objects before invoking the cut command."));
        return; /* TODO: Prompt to select objects if nothing is preselected */
    }

    doc_begin_macro(doc, "Cut");
    doc_copy_selected(doc);
    doc_delete_selected(doc);
    doc_end_macro(doc);
}

/* . */
void
doc_create_grid(int32_t doc, EmbString gridType)
{
    DocumentData *data = doc_data(doc);
    if (!strcmp(gridType, "Rectangular")) {
        doc_create_grid_rect(doc);
        data->properties[VIEW_GRID] = true;
    }
    else if (!strcmp(gridType, "Circular")) {
        doc_create_grid_polar(doc);
        data->properties[VIEW_GRID] = true;
    }
    else if (!strcmp(gridType, "Isometric")) {
        doc_create_grid_iso(doc);
        data->properties[VIEW_GRID] = true;
    }
    else {
        doc_empty_grid(doc);
        data->properties[VIEW_GRID] = false;
    }

    doc_create_origin(doc);

    doc_update(doc);
}

/* . */
void
open_recent_file(void)
{
    debug_message("open_recent_file()");

    /* Check to see if this from the recent files list. */
    /* FIXME: QAction* recentSender = qobject_cast<QAction*>(sender());
    if (recentSender) {
        open_file(true, recentSender->data().toString());
    }
    */
}

/* . */
void
statusbar_toggle(const char *key, bool on)
{
    debug_message("StatusBarButton toggleSnap()");
    int32_t doc = active_document();
    if (doc < 0) {
        return;
    }
    doc_toggle(doc, key, on);
}

/* . */
void
doc_copy(int32_t doc)
{
    DocumentData *data = doc_data(doc);
    if (data->selectedItems->count <= 0) {
        messagebox("information", translate("Copy Preselect"),
            translate("Preselect objects before invoking the copy command."));
        return; /* TODO: Prompt to select objects if nothing is preselected */
    }

    doc_copy_selected(doc);
    doc_clear_selection(doc);
}
/* . */
void
doc_set_rubber_mode(int32_t doc, int mode)
{
    DocumentData *data = doc_data(doc);
    for (int i=0; i<data->rubberRoomList->count; i++) {
        int32_t item = data->rubberRoomList->data[i];
        obj_set_rubber_mode(item, mode);
    }
    doc_update(doc);
}

/* . */
void
doc_set_rubber_point(int32_t doc, EmbString key, EmbVector point)
{
    DocumentData *data = doc_data(doc);
    for (int i=0; i<data->rubberRoomList->count; i++) {
        int32_t item = data->rubberRoomList->data[i];
        obj_set_rubber_point(item, key, point);
    }
    doc_update(doc);
}

/* . */
void
doc_set_rubber_text(int32_t doc, EmbString key, EmbString txt)
{
    DocumentData *data = doc_data(doc);
    for (int i=0; i<data->rubberRoomList->count; i++) {
        int32_t item = data->rubberRoomList->data[i];
        obj_set_rubber_text(item, key, txt);
    }
    doc_update(doc);
}

/* The caller is responsible for allocating the memory. */
void
copy_object_list(EmbIdList *dst, EmbIdList *src)
{
    dst->count = 0;
    for (int i = 0; i < src->count; i++) {
        uint32_t copyObj = copy_object(src->data[i]);
        append_id_to_list(dst, copyObj);
    }
}

/* . */
void
doc_copy_selected(int32_t doc)
{
    DocumentData *data = doc_data(doc);

    /* Prevent memory leaks by deleting any unpasted instances */
    free_objects(data->cutCopyObjectList);

    /* Create new objects but do not add them to the scene just yet.
     * By creating them now, ensures that pasting will still work
     * if the original objects are deleted before the paste occurs.
     */
    copy_object_list(data->cutCopyObjectList, data->selectedItems);
}

/* . */
void
doc_vulcanize_rubber_room(int32_t doc)
{
    DocumentData *data = doc_data(doc);
    for (int i=0; i<data->rubberRoomList->count; i++) {
        int32_t item = data->rubberRoomList->data[i];
        doc_vulcanize_object(doc, item);
    }
    data->rubberRoomList->count = 0;
    doc_update(doc);
}

/* . */
void
doc_center_at(int32_t doc, EmbVector centerPoint)
{
    /* centerOn also updates the scrollbars, which shifts things out of wack o_O */
    doc_center_on(doc, centerPoint);
    /* Reshift to the new center */
    EmbVector offset = emb_vector_subtract(centerPoint, doc_center(doc));
    EmbVector newCenter = emb_vector_add(centerPoint, offset);
    doc_center_on(doc, newCenter);
}

/* . */
bool
doc_allow_zoom_in(int32_t doc)
{
    EmbVector origin = doc_map_to_scene(doc, emb_vector(0.0, 0.0));
    EmbVector corner = doc_map_to_scene(doc,
        emb_vector(doc_width(doc), doc_height(doc)));
    EmbReal maxWidth = corner.x - origin.x;
    EmbReal maxHeight = corner.y - origin.y;

    if (EMB_MIN(maxWidth, maxHeight) < zoomInLimit) {
        debug_message("ZoomIn limit reached. (limit=%.10f)", zoomInLimit);
        return false;
    }

    return true;
}

/* . */
bool
doc_allow_zoom_out(int32_t doc)
{
    EmbVector origin = doc_map_to_scene(doc, emb_vector(0.0, 0.0));
    EmbVector corner = doc_map_to_scene(doc,
        emb_vector(doc_width(doc), doc_height(doc)));
    EmbReal maxWidth = corner.x - origin.x;
    EmbReal maxHeight = corner.y - origin.y;

    if (EMB_MAX(maxWidth, maxHeight) > zoomOutLimit) {
        debug_message("ZoomOut limit reached. (limit=%.1f)", zoomOutLimit);
        return false;
    }

    return true;
}

/* . */
void
doc_set_prop(int32_t doc, int key, bool value)
{
    DocumentData *data = doc_data(doc);
    data->properties[key] = value;
}

/* . */
bool
doc_allow_rubber(int32_t doc)
{
    /* if (!data->rubberRoomList.size()) */ /* TODO: this check should be removed later */
        return true;
    return false;
}

/* . */
void
doc_set_grid_color(int32_t doc, uint32_t color)
{
    DocumentData *data = doc_data(doc);
    data->gridColor = color;
    doc_update(doc);
}

/* . */
void
doc_set_ruler_color(int32_t doc, uint32_t color)
{
    DocumentData *data = doc_data(doc);
    data->rulerColor = color;
    doc_update(doc);
}

/* . */
void
doc_set_cross_hair_color(int32_t doc, uint32_t color)
{
    DocumentData *data = doc_data(doc);
    data->crosshairColor = color;
    doc_update(doc);
}

/* . */
void
doc_toggle(int32_t doc, const char *key, bool on)
{
    debug_message("doc_toggle(%d, %s, %d)", doc, key, on);
    DocumentData *data = doc_data(doc);
    wait_cursor();

    if (!strcmp(key, "GRID")) {
        if (on) {
            doc_create_grid(doc, get_str(GRID_TYPE));
        }
        else {
            doc_create_grid(doc, "");
        }
    }
    if (!strcmp(key, "RULER")) {
        data->properties[VIEW_RULER] = on;
        data->rulerMetric = get_bool(RULER_METRIC);
        data->rulerColor = get_int(RULER_COLOR);
        data->rulerPixelSize = get_int(RULER_PIXEL_SIZE);
    }
    if (!strcmp(key, "SNAP")) {    
        /* TODO: finish this */
        data->properties[VIEW_SNAP] = on;
    }
    if (!strcmp(key, "POLAR")) {    
        /* TODO: finish this */
        data->properties[VIEW_POLAR] = on;
    }
    if (!strcmp(key, "ORTHO")) {
        /* TODO: finish this */
        data->properties[VIEW_ORTHO] = on;
    }
    if (!strcmp(key, "QTRACK")) {
        /* TODO: finish this */
        data->properties[VIEW_QTRACK] = on;
    }
    if (!strcmp(key, "LWT")) {
        data->properties[VIEW_LWT] = on;
    }
    if (!strcmp(key, "REAL")) {
        data->properties[VIEW_REAL] = on;
    }
    if (!strcmp(key, "QSNAP")) {
        data->properties[VIEW_QSNAP] = on;
    }

    doc_update(doc);
    restore_cursor();
}

/* Stop any active interactions the user is engaged in. */
void
doc_stop(int32_t doc)
{
    DocumentData *data = doc_data(doc);
    if (data->properties[VIEW_PASTING]) {
        remove_paste_object_item_group(doc);
    }
    data->properties[VIEW_PASTING] = false;
    data->properties[VIEW_ZOOMING] = false;
    data->properties[VIEW_SELECTING] = false;
    hide_selectbox(doc);
    if (data->properties[VIEW_GRIPPING]) {
        doc_stop_gripping(doc, false);
    }
    else {
        doc_clear_selection(doc);
    }
}

/* . */
void
doc_delete_pressed(int32_t doc)
{
    debug_message("View delete_pressed()");
    doc_stop(doc);
    doc_delete_selected(doc);
}

/* . */
void
doc_escape_pressed(int32_t doc)
{
    debug_message("View escape_pressed()");
    doc_stop(doc);
}

/* . */
void
doc_delete_selected(int32_t doc)
{
    DocumentData *data = doc_data(doc);
    if (data->selectedItems->count > 1) {
        EmbString macro_name;
        sprintf(macro_name, "%s %d", translate("Delete"), data->selectedItems->count);
        doc_begin_macro(doc, macro_name);
    }
    for (int i = 0; i < data->selectedItems->count; i++) {
        ObjectCore* core = obj_get_core(data->selectedItems->data[i]);
        if (core->geometry->type != OBJ_NULL) {
            EmbString label;
            sprintf(label, "%s%s", translate("Delete 1 "), core->OBJ_NAME);
            undoable_delete(doc, data->selectedItems->data[i], label);
        }
    }
    if (data->selectedItems->count > 1) {
        doc_end_macro(doc);
    }
}

/* . */
void
doc_move_selected(int32_t doc, EmbVector delta)
{
    DocumentData *data = doc_data(doc);
    if (data->selectedItems->count > 1) {
        EmbString macro_name;
        sprintf(macro_name, "%s %d", translate("Move"), data->selectedItems->count);
        doc_begin_macro(doc, macro_name);
    }
    for (int i = 0; i < data->selectedItems->count; i++) {
        ObjectCore* core = obj_get_core(data->selectedItems->data[i]);
        char msg[400];
        sprintf(msg, "%s 1 %s", translate("Move"), core->OBJ_NAME);
        undoable_move(doc, data->selectedItems->data[i], delta, msg);
    }
    if (data->selectedItems->count > 1) {
        doc_end_macro(doc);
    }

    /* Always clear the selection after a move. */
    doc_clear_selection(doc);
}

/* . */
void
doc_rotate_selected(int32_t doc, EmbVector v, EmbReal rot)
{
    DocumentData *data = doc_data(doc);
    if (data->selectedItems->count > 1) {
        EmbString macro_name;
        sprintf(macro_name, "%s %d", translate("Rotate"), data->selectedItems->count);
        doc_begin_macro(doc, macro_name);
    }
    for (int i = 0; i < data->selectedItems->count; i++) {
        ObjectCore* core = obj_get_core(data->selectedItems->data[i]);
        char msg[300];
        sprintf(msg, "%s 1 %s", translate("Rotate"), core->OBJ_NAME);
        undoable_rotate(doc, data->selectedItems->data[i], v, msg);
    }
    if (data->selectedItems->count > 1) {
        doc_end_macro(doc);
    }

    /* Always clear the selection after a rotate. */
    doc_clear_selection(doc);
}

/* . */
void
doc_mirror_selected(int32_t doc, EmbReal x1, EmbReal y1, EmbReal x2, EmbReal y2)
{
    DocumentData *data = doc_data(doc);
    if (data->selectedItems->count > 1) {
        EmbString macro_name;
        sprintf(macro_name, "%s %d", translate("Mirror"), data->selectedItems->count);
        doc_begin_macro(doc, macro_name);
    }
    for (int i = 0; i < data->selectedItems->count; i++) {
        ObjectCore* core = obj_get_core(data->selectedItems->data[i]);
        char msg[300];
        sprintf(msg, "%s 1 %s", translate("Mirror"), core->OBJ_NAME);
        EmbVector start, end;
        start = emb_vector(x1, y1);
        end = emb_vector(x2, y2);
        undoable_mirror(doc, data->selectedItems->data[i], start, end, msg);
    }
    if (data->selectedItems->count > 1) {
        doc_end_macro(doc);
    }

    /* Always clear the selection after a mirror. */
    doc_clear_selection(doc);
}

/* . */
void
doc_scale_selected(int32_t doc, EmbReal x, EmbReal y, EmbReal factor)
{
    if (factor <= 0.0) {
        messagebox("critical", translate("ScaleFactor Error"),
            translate("Hi there. If you are not a developer, report this as a bug. "
            "If you are a developer, your code needs examined, and possibly your head too."));
        return;
    }

    DocumentData *data = doc_data(doc);
    if (data->selectedItems->count > 1) {
        EmbString macro_name;
        sprintf(macro_name, "%s %d", translate("Scale"), data->selectedItems->count);
        doc_begin_macro(doc, macro_name);
    }
    for (int i = 0; i < data->selectedItems->count; i++) {
        ObjectCore* core = obj_get_core(data->selectedItems->data[i]);
        EmbVector v = emb_vector(x, y);
        EmbString msg;
        sprintf(msg, "%s%s", translate("Scale 1 "), core->OBJ_NAME);
        undoable_scale(doc, data->selectedItems->data[i], v, factor, msg);
    }
    if (data->selectedItems->count > 1) {
        doc_end_macro(doc);
    }

    /* Always clear the selection after a scale. */
    doc_clear_selection(doc);
}

/* LALR Parsing? Check the Dragon book. */
int
string_replace(char *result, char *base, char *from, char *to)
{
    int replacements = 0;
    result[0] = 0;
    for (int i=0; i<strlen(base); i++) {
        int replaced = 0;
        if (base[i] == from[0]) {
            if (!strncmp(base + i, from, strlen(from))) {
                replaced = 1;
                i += strlen(from) - 1;
                strcat(result, to);
                replacements++;
            }
        }
        if (!replaced) {
            char a[] = {base[i], 0};
            strcat(result, a);
        }
    }
    return replacements;
}

/* . */
ViewData *
create_view_data(int32_t doc, int width, int height, int unit)
{
    ViewData *data = malloc(sizeof(ViewData));

    data->doc = doc;
    data->little = 0.20;
    data->medium = 0.40;
    data->width = width;
    data->height = height;
    data->lines = malloc(2000*sizeof(EmbLine));
    data->n_lines = 0;
    data->geometry = malloc(2000*sizeof(EmbGeometry));
    data->n_geometry = 0;
    data->unit = unit;
    data->feet = true;
    DocumentData *d = doc_data(doc);
    if (d->rulerMetric) {
        if (data->unit < 10) {
            data->unit = 10;
        }
        data->fraction = data->unit/10;
    }
    else {
        if (data->unit <= 1) {
            data->unit = 1;
            data->feet = false;
            data->fraction = (double)(data->unit/16);
        }
        else {
            data->unit = round_to_multiple(true, data->unit, 12);
            data->fraction = unit/12;
        }
    }
    
    EmbVector origin = doc_map_to_scene(doc, emb_vector(0.0, 0.0));
    int xFlow, yFlow;
    if (int32_underflow(origin.x, unit)) {
        return NULL;
    }
    xFlow = round_to_multiple(false, origin.x, unit);
    if (int32_underflow(xFlow, unit)) {
        return NULL;
    }
    data->xStart = xFlow - unit;
    if (int32_underflow(origin.y, unit)) {
        return NULL;
    }
    yFlow = round_to_multiple(false, origin.y, unit);
    if (int32_underflow(yFlow, unit)) {
        return NULL;
    }
    data->yStart = yFlow - unit;

    return data;
}

void
free_view_data(ViewData *vdata)
{
    free(vdata->lines);
    free(vdata);
}

void
add_line(ViewData *vdata, EmbVector start, EmbVector end)
{
    vdata->lines[vdata->n_lines].start = start;
    vdata->lines[vdata->n_lines].end = end;
    vdata->n_lines++;
}

/* . */
int
create_ruler_lines(ViewData *vdata)
{
    DocumentData *data = doc_data(vdata->doc);

    EmbVector origin = doc_map_to_scene(vdata->doc, emb_vector(0.0, 0.0));
    EmbVector rulerHoriz = doc_map_to_scene(vdata->doc,
        emb_vector(vdata->width, data->rulerPixelSize));
    EmbVector rulerVert = doc_map_to_scene(vdata->doc,
        emb_vector(data->rulerPixelSize, vdata->height));
    EmbVector ruler_h_cursor = emb_vector_subtract(rulerHoriz, origin);
    EmbVector ruler_v_cursor = emb_vector_subtract(rulerVert, origin);

    /* NOTE: Drawing ruler if zoomed out too far will cause an assertion failure. */
    /* We will limit the maximum size the ruler can be shown at. */
    uint16_t maxSize = -1; /* Intentional underflow */
    if (ruler_h_cursor.x >= maxSize || ruler_v_cursor.y >= maxSize) {
        return 0;
    }

    add_line(vdata, emb_vector(origin.x, rulerHoriz.y), rulerHoriz);
    add_line(vdata, emb_vector(rulerVert.x, origin.y), rulerVert);
    add_line(vdata,
        emb_vector(data->sceneMousePoint.x, rulerHoriz.y),
        emb_vector(data->sceneMousePoint.x, origin.y));
    add_line(vdata,
        emb_vector(rulerVert.x, data->sceneMousePoint.y),
        emb_vector(origin.x, data->sceneMousePoint.y));

    /* Ticks */
    for (int x = vdata->xStart; x < rulerHoriz.x; x += vdata->unit) {
        add_line(vdata, emb_vector(x, rulerHoriz.y), emb_vector(x, origin.y));

        if (data->rulerMetric) {
            for (int i=1; i<10; i++) {
                EmbReal xf = x + vdata->fraction * i;
                EmbReal tick = ruler_h_cursor.y * vdata->little;
                if (i == 5) {
                    tick = ruler_h_cursor.y * vdata->medium;
                }
                add_line(vdata,
                    emb_vector(xf, rulerHoriz.y),
                    emb_vector(xf, rulerHoriz.y-tick));
            }
        }
        else {
            if (vdata->feet) {
                for (int i = 0; i < 12; i++) {
                    EmbReal xf = x + vdata->fraction * i;
                    EmbReal tick = ruler_h_cursor.y * vdata->medium;
                    add_line(vdata,
                        emb_vector(xf, rulerHoriz.y),
                        emb_vector(xf, rulerHoriz.y - tick));
                }
            }
            else {
                for (int i=1; i<16; i++) {
                    EmbReal xf = x + vdata->fraction * i;
                    EmbReal tick = ruler_h_cursor.y * vdata->little;
                    if (i % 4 == 0) {
                        tick = ruler_h_cursor.y * vdata->medium;
                    }
                    add_line(vdata,
                        emb_vector(xf, rulerHoriz.y),
                        emb_vector(xf, rulerHoriz.y - tick));
                }
            }
        }
    }

    for (int y = vdata->yStart; y < rulerVert.y; y += vdata->unit) {
        add_line(vdata, emb_vector(rulerVert.x, y), emb_vector(origin.x, y));

        if (data->rulerMetric) {
            for (int i=1; i<10; i++) {
                EmbReal yf = y + vdata->fraction*i;
                EmbReal tick = ruler_v_cursor.x * vdata->little;
                if (i == 5) {
                    tick = ruler_v_cursor.x * vdata->medium;
                }
                add_line(vdata,
                    emb_vector(rulerVert.x, yf),
                    emb_vector(rulerVert.x - tick, yf));
            }
        }
        else {
            if (vdata->feet) {
                for (int i = 0; i < 12; ++i) {
                    EmbReal yf = y + vdata->fraction*i;
                    EmbReal tick = ruler_v_cursor.x * vdata->medium;
                    add_line(vdata,
                        emb_vector(rulerVert.x, yf),
                        emb_vector(rulerVert.x - tick, yf));
                }
            }
            else {
                for (int i=1; i<16; i++) {
                    EmbReal yf = y + vdata->fraction*i;
                    EmbReal tick = ruler_v_cursor.x * vdata->little;
                    if (i % 4 == 0) {
                        tick = ruler_v_cursor.x * vdata->medium;
                    }
                    add_line(vdata,
                        emb_vector(rulerVert.x, yf),
                        emb_vector(rulerVert.x - tick, yf));
                }
            }
        }
    }

    return 1;
}

/* . */
void
set_visibility_group(char *keylist[], bool visibility)
{
    int i;
    int n = table_length(keylist);
    for (i=0; i<n; i++) {
        set_visibility(keylist[i], visibility);
    }
}

/* . */
void
set_enabled_group(char *keylist[], bool enabled)
{
    int i;
    int n = table_length(keylist);
    for (i=0; i<n; i++) {
        set_enabled(keylist[i], enabled);
    }
}

/* FIXME: reconnect to new command system.
 */
void
run_command_main(const char *cmd)
{
    ScriptEnv *context = create_script_env();
    context->context = CONTEXT_MAIN;
    debug_message("run_command_main(%s)", cmd);
    /* TODO: Uncomment this when post-selection is available. */
    /*
    if (!get_bool(SELECTION_MODE_PICKFIRST)) {
        clear_selection();
    }
    */
    run_cmd(context, cmd);
    free_script_env(context);
}

/* FIXME: reconnect to new command system.
 */
void
run_command_click(const char *cmd, EmbReal x, EmbReal y)
{
    ScriptEnv *context = create_script_env();
    context->context = CONTEXT_CLICK;
    debug_message("run_command_click(%s, %.2f, %.2f)", cmd, x, y);
    /* engine->evaluate(cmd + "_click(" + QString().setNum(x) + "," + QString().setNum(-y) + ")", fileName); */
    run_cmd(context, cmd);
    free_script_env(context);
}

/* FIXME: reconnect to new command system.
 */
void
run_command_move(const char *cmd, EmbReal x, EmbReal y)
{
    ScriptEnv *context = create_script_env();
    context->context = CONTEXT_MOVE;
    debug_message("run_command_move(%s, %.2f, %.2f)", cmd, x, y);
    /* engine->evaluate(cmd + "_move(" + QString().setNum(x) + "," + QString().setNum(-y) + ")", fileName); */
    run_cmd(context, cmd);
    free_script_env(context);
}

/* FIXME: reconnect to new command system.
 */
void
run_command_context(const char *cmd, const char *str)
{
    ScriptEnv *context = create_script_env();
    context->context = CONTEXT_CONTEXT;
    debug_message("run_command_context(%s, %s)", cmd, str);
    /* engine->evaluate(cmd + "_context('" + str.toUpper() + "')", fileName); */
    run_cmd(context, cmd);
    free_script_env(context);
}

/* FIXME: reconnect to new command system.
 * NOTE: Replace any special characters that will cause a syntax error
 */
void
run_command_prompt(const char *cmd)
{
    ScriptEnv *context = create_script_env();
    debug_message("run_command_prompt(%s)", cmd);
    context->context = CONTEXT_PROMPT;
    if (rapidFireEnabled) {
        run_cmd(context, cmd);
    }
    else {
        /* FIXME: Both branches run the same. */
        run_cmd(context, cmd);
    }
    free_script_env(context);
}

/*
 * BUG: pan commands broke
 *
 * This is called when a command is issued.
 */
ScriptValue
run_cmd(ScriptEnv *context, const char *cmd)
{
    int id = get_command_id((char*)cmd);
    debug_message("run_cmd(%s) %d", cmd, id);

    if (id < 0) {
        debug_message("ERROR: %s not found in command_data.", cmd);
        return script_false;
    }

    if (!argument_checks(context, id)) {
        debug_message("Failed argument checks.");
        return script_false;
    }

    if (command_data[id].flags & REQUIRED_VIEW) {
        int doc_index = active_document();
        if (doc_index < 0) {
            return script_false;
        }
        /* If initialization is needed, a view is required implicitly. */
        if (!(command_data[id].flags & DONT_INITIALIZE)) {
            doc_clear_rubber_room(doc_index);
        }
        /* Selection only exists when the view exists. */
        if (command_data[id].flags & CLEAR_SELECTION) {
            //doc_clear_selection(doc_index);
        }
    }

    ScriptValue value = command_data[id].action(context);
    if (!(command_data[id].flags & DONT_END_COMMAND)) {
        end_command();
    }
    return value;
}

/*
 * Wrapper for system-specific UI features.
 * This is an alternative approach to using GLFW.
 */

void
draw_frame(void)
{
    glClearColor(1.0, 1.0, 1.0, 1.0);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glColor3f(1.0, 0.0, 0.0);
    glBegin(GL_QUADS);
    glVertex2f(1.0, 0.0);
    glVertex2f(1.0, 1.0);
    glVertex2f(0.0, 1.0);
    glVertex2f(0.0, 0.0);
    glEnd();
}

