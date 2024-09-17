/*
 * Embroidermodder 2 (GLFW version)
 *
 * Copyright 2011-2024 The Embroidermodder Team
 * Embroidermodder 2 is Open Source Software, see LICENSE.md for licensing terms.
 *
 * Read the reference manual (https://www.libembroidery.org/downloads/emrm.pdf)
 * for advice on altering this file.
 *
 * Main window wrangling and drawing functions.
 */

#include <stdio.h>

#include <GL/glew.h>

#define GLFW_INCLUDE_GLEXT
#include "GLFW/glfw3.h"

#include "nanovg.h"
#define NANOVG_GL2_IMPLEMENTATION
#include "nanovg_gl.h"

#include "core.h"

typedef struct Button_ {
    EmbRect rect;
    EmbColor color;
    EmbColor highlight_color;
    EmbString text;
    EmbString font;
    EmbColor text_color;
    int state;
} Button;

typedef struct Tab_ {
    int state;
} Tab;

extern int icon_size;
extern Button menu_buttons[10];
extern Button tool_buttons[100];
extern EmbColor toolbar_bg_color;
extern EmbColor toolbar_button_color;
extern EmbColor toolbar_text;
extern int window_width;
extern int window_height;
extern int toolbar_height;
extern float button_padding;

void draw_rect(NVGcontext *vg, EmbRect rect, EmbColor color);
void draw_button(NVGcontext *vg, Button button, float *bounds);
void draw_text(NVGcontext *vg, int x, int y, char *font, char *txt, EmbColor color, float *bounds);
void draw_view(NVGcontext *vg);
void draw_mdiarea(NVGcontext *vg);
void draw_dockarea(NVGcontext *vg);
void draw_nvg_toolbar(NVGcontext *vg);
void draw_prompt(NVGcontext *vg);
void draw_statusbar(NVGcontext *vg);

int icon_size = 16;

Button menu_buttons[10];
Button tool_buttons[100];

/* FIXME: this is very system-dependant. Could compile in somehow? */
char *sans_font = "/usr/share/fonts/truetype/open-sans/OpenSans-Regular.ttf";
char *icon_font = "/usr/share/fonts/truetype/open-sans/OpenSans-Regular.ttf";

EmbColor toolbar_bg_color = {.r=128, .g=128, .b=128};
EmbColor toolbar_button_color = {.r=208, .g=208, .b=208};
EmbColor toolbar_text = {.r=0, .g=0, .b=0};
int window_width = 640;
int window_height = 480;
int toolbar_height = 40;
float button_padding = 5.0;

/* Dummy functions. */
void nativeSetCursorShape(char *) {}
void nativeSetBackgroundColor(uint8_t, uint8_t, uint8_t) {}
void nativeSetCrossHairColor(uint8_t, uint8_t, uint8_t) {}
void nativeSetGridColor(uint8_t, uint8_t, uint8_t) {}
void nativeAddTextSingle(char *, double, double, double, bool, int) {}
void nativeAddArc(double, double, double, double, double, double, int) {}
void nativeAddSlot(double, double, double, double, double, bool, int) {}
void nativeAddPoint(double, double) {}
void question_box(const char *, const char *) {}
void warning_box(const char *, const char *) {}
void critical_box(const char *, const char *) {}
void information_box(const char *, const char *) {}
void clear_selection(void) {}
void end_command(void) {}
void start_blinking(void) {}
void nativeSetRubberMode(int) {}
void nativeSetRubberPoint(const EmbString, double, double) {}
void nativeScaleSelected(double, double, double) {}
bool nativeAllowRubber() {}
void nativeAddCircle(double centerX, double centerY, double radius, bool fill, int rubberMode) {}
void nativeAddEllipse(double centerX, double centerY, double width, double height, double rot, bool fill, int rubberMode) {}
void nativeAddDimLeader(double x1, double y1, double x2, double y2, double rot, int rubberMode) {}
void nativeAddLine(double x1, double y1, double x2, double y2, double rot, int rubberMode) {}
void nativeAddRectangle(double x, double y, double w, double h, double rot, bool fill, int rubberMode) {}
void nativeSpareRubber(int64_t) {}

/* . */
ScriptValue
run_command_core(int id, const EmbString cmd, ScriptEnv *context)
{
    return script_false;
}

/* . */
void
prompt_output(const EmbString line)
{
    printf("%s\n", line);
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
        button.rect.x + button.rect.w * 0.5,
        button.rect.y + button.rect.h * 0.5,
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
    nvgFontSize(vg, 14.0f);
    nvgFontFace(vg, font);
    nvgTextAlign(vg, NVG_ALIGN_CENTER | NVG_ALIGN_MIDDLE);
    nvgFillColor(vg, nvgRGBA(text_color.r, text_color.g, text_color.b, 255));
    nvgTextBounds(vg, x, y, txt, NULL, bounds);
    nvgText(vg, x, y, txt, NULL);
    nvgFill(vg);
}

/* . */
Button
make_menubar_button(NVGcontext *vg, int x, int y, char *text, float *bounds)
{
    Button button;
    nvgTextBounds(vg, x, y, text, NULL, bounds);
    button.rect = emb_rect(x, y, bounds[2] - bounds[0] + button_padding, 30);
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
draw_menubar(NVGcontext *vg)
{
    EmbRect rect = emb_rect(0, 0, window_width, toolbar_height);
    if (window_height > 1000) {
        rect.h = 50;
    }

    draw_rect(vg, rect, toolbar_bg_color);

    float bounds[4] = {0.0, 0.0, 0.0, 0.0};
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
draw_nvg_toolbar(NVGcontext *vg)
{
    EmbRect rect = emb_rect(toolbar_height + button_padding, 0,
        window_width, toolbar_height);

    //draw_rect(vg, rect, toolbar_bg_color);

    float bounds[4] = {0.0, 0.0, 0.0, 0.0};
    for (int i=0; i<10; i++) {
        tool_buttons[i] = make_toolbar_button(5+5*i+16*i, 50, "Test");
        draw_button(vg, tool_buttons[i], bounds);
    }
}

/* . */
void
draw_view(NVGcontext *vg)
{

}

/* . */
void
draw_mdiarea(NVGcontext *vg)
{
    if (numOfDocs == 0) {
    }
    else {
    }
}

/* . */
void
draw_dockarea(NVGcontext *vg)
{

}

/* . */
void
draw_prompt(NVGcontext *vg)
{

}

/* . */
void
draw_statusbar(NVGcontext *vg)
{

}

/* . */
void
draw_interface(NVGcontext *vg)
{
    draw_menubar(vg);
    draw_nvg_toolbar(vg);
    draw_mdiarea(vg);
    draw_prompt(vg);
    draw_statusbar(vg);
}

/* . */
int
make_application(int argc, char *argv[])
{
    GLFWwindow* window;
    GLFWvidmode vidmode;

    if (!load_data()) {
        return 1;
    }

    if (!glfwInit()) {
        return 2;
    }

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 2);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 0);
    glfwWindowHint(GLFW_SAMPLES, 4);

    window = glfwCreateWindow(640, 480, "Embroidermodder 2 (GLFW)", NULL, NULL);
    if (!window) {
        glfwTerminate();
        return 3;
    }

    glfwMakeContextCurrent(window);

    if (glewInit() != GLEW_OK) {
        glfwTerminate();
        return 4;
    }

    NVGcontext *vg = nvgCreateGL2(NVG_STENCIL_STROKES | NVG_DEBUG);
    if (!vg) {
        puts("Failed to initialize NanoVG.");
        return 5;
    }

    double prevt = glfwGetTime();

    /* FIXME: only works on linux and only works if adobe fonts are installed. */
    int font = nvgCreateFont(vg, "sans", sans_font);
    if (font == -1) {
        puts("Font failed to load.");
        return 6;
    }
    font = nvgCreateFont(vg, "icons", icon_font);
    if (font == -1) {
        puts("Font failed to load.");
        return 6;
    }

    while (!glfwWindowShouldClose(window)) {
        double t = glfwGetTime();
        double dt = t - prevt;
        printf("%f %f\n", t, dt);

        /* Cap at 120 updates a second. */
        if (dt < 1.0/120) {
            continue;
        }
        prevt = t;

        glfwGetWindowSize(window, &window_width, &window_height);

        glClear(GL_COLOR_BUFFER_BIT);

        glViewport(0, 0, window_width, window_height);

        nvgBeginFrame(vg, window_width, window_height, 1.0);

        draw_interface(vg);
        
        nvgEndFrame(vg);

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    nvgDeleteGL2(vg);
    glfwTerminate();
    return 0;
}

