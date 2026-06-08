/*
 * Embroidermodder 2 -- Nuklear frontend
 * Copyright 2011-2026 The Embroidermodder Team
 *
 * ------------------------------------------------------------------
 *
 * This file is based on the examples from the Nuklear source code.
 * See "extern/Nuklear".
 *
 * nuklear - v1.32.0 - public domain
 */

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdarg.h>
#include <string.h>
#include <math.h>
#include <assert.h>
#include <limits.h>
#include <time.h>

#include <GLFW/glfw3.h>

#define NK_INCLUDE_FIXED_TYPES
#define NK_INCLUDE_STANDARD_IO
#define NK_INCLUDE_STANDARD_VARARGS
#define NK_INCLUDE_DEFAULT_ALLOCATOR
#define NK_INCLUDE_VERTEX_BUFFER_OUTPUT
#define NK_INCLUDE_FONT_BAKING
#define NK_INCLUDE_DEFAULT_FONT
#define NK_IMPLEMENTATION
#define NK_GLFW_GL2_IMPLEMENTATION
#include "nuklear.h"
#include "demo/glfw_opengl2/nuklear_glfw_gl2.h"

#define STB_IMAGE_IMPLEMENTATION
#include "demo/common/filebrowser/stb_image.h"

#define WINDOW_WIDTH 1200
#define WINDOW_HEIGHT 800

#define TOP_LEFT       NK_TEXT_ALIGN_TOP|NK_TEXT_ALIGN_LEFT
#define TOP_CENTER     NK_TEXT_ALIGN_TOP|NK_TEXT_ALIGN_CENTERED
#define TOP_RIGHT      NK_TEXT_ALIGN_TOP|NK_TEXT_ALIGN_RIGHT
#define BOTTOM_LEFT    NK_TEXT_ALIGN_BOTTOM|NK_TEXT_ALIGN_LEFT
#define BOTTOM_CENTER  NK_TEXT_ALIGN_BOTTOM|NK_TEXT_ALIGN_CENTERED
#define BOTTOM_RIGHT   NK_TEXT_ALIGN_BOTTOM|NK_TEXT_ALIGN_RIGHT

const char* alignments[] = {
    "LEFT",
    "CENTERED",
    "RIGHT",
    "TOP LEFT",
    "TOP CENTERED",
    "TOP_RIGHT",
    "BOTTOM LEFT",
    "BOTTOM CENTERED",
    "BOTTOM RIGHT"
};

unsigned int aligns[] = {
    NK_TEXT_LEFT,
    NK_TEXT_CENTERED,
    NK_TEXT_RIGHT,
    TOP_LEFT,
    TOP_CENTER,
    TOP_RIGHT,
    BOTTOM_LEFT,
    BOTTOM_CENTER,
    BOTTOM_RIGHT
};

enum theme {
    THEME_BLACK,
    THEME_WHITE,
    THEME_RED,
    THEME_BLUE,
    THEME_DARK,
    THEME_DRACULA,
    THEME_CATPPUCCIN_LATTE,
    THEME_CATPPUCCIN_FRAPPE,
    THEME_CATPPUCCIN_MACCHIATO,
    THEME_CATPPUCCIN_MOCHA
};

/* ===============================================================
 *
 *                          EXAMPLE
 *
 * ===============================================================*/
/* This are some code examples to provide a small overview of what can be
 * done with this library. To try out an example uncomment the defines */
/* #define INCLUDE_FILE_BROWSER */
#ifdef INCLUDE_FILE_BROWSER
  #include "demo/common/file_browser.c"
#endif

struct nk_canvas {
    struct nk_command_buffer *painter;
    struct nk_vec2 item_spacing;
    struct nk_vec2 panel_padding;
    struct nk_style_item window_background;
};

static void set_style(struct nk_context *ctx, enum theme theme) {
  struct nk_color table[NK_COLOR_COUNT];
  if (theme == THEME_WHITE) {
    table[NK_COLOR_TEXT] = nk_rgba(70, 70, 70, 255);
    table[NK_COLOR_WINDOW] = nk_rgba(175, 175, 175, 255);
    table[NK_COLOR_HEADER] = nk_rgba(175, 175, 175, 255);
    table[NK_COLOR_BORDER] = nk_rgba(0, 0, 0, 255);
    table[NK_COLOR_BUTTON] = nk_rgba(185, 185, 185, 255);
    table[NK_COLOR_BUTTON_HOVER] = nk_rgba(170, 170, 170, 255);
    table[NK_COLOR_BUTTON_ACTIVE] = nk_rgba(160, 160, 160, 255);
    table[NK_COLOR_TOGGLE] = nk_rgba(150, 150, 150, 255);
    table[NK_COLOR_TOGGLE_HOVER] = nk_rgba(120, 120, 120, 255);
    table[NK_COLOR_TOGGLE_CURSOR] = nk_rgba(175, 175, 175, 255);
    table[NK_COLOR_SELECT] = nk_rgba(190, 190, 190, 255);
    table[NK_COLOR_SELECT_ACTIVE] = nk_rgba(175, 175, 175, 255);
    table[NK_COLOR_SLIDER] = nk_rgba(190, 190, 190, 255);
    table[NK_COLOR_SLIDER_CURSOR] = nk_rgba(80, 80, 80, 255);
    table[NK_COLOR_SLIDER_CURSOR_HOVER] = nk_rgba(70, 70, 70, 255);
    table[NK_COLOR_SLIDER_CURSOR_ACTIVE] = nk_rgba(60, 60, 60, 255);
    table[NK_COLOR_PROPERTY] = nk_rgba(175, 175, 175, 255);
    table[NK_COLOR_EDIT] = nk_rgba(150, 150, 150, 255);
    table[NK_COLOR_EDIT_CURSOR] = nk_rgba(0, 0, 0, 255);
    table[NK_COLOR_COMBO] = nk_rgba(175, 175, 175, 255);
    table[NK_COLOR_CHART] = nk_rgba(160, 160, 160, 255);
    table[NK_COLOR_CHART_COLOR] = nk_rgba(45, 45, 45, 255);
    table[NK_COLOR_CHART_COLOR_HIGHLIGHT] = nk_rgba(255, 0, 0, 255);
    table[NK_COLOR_SCROLLBAR] = nk_rgba(180, 180, 180, 255);
    table[NK_COLOR_SCROLLBAR_CURSOR] = nk_rgba(140, 140, 140, 255);
    table[NK_COLOR_SCROLLBAR_CURSOR_HOVER] = nk_rgba(150, 150, 150, 255);
    table[NK_COLOR_SCROLLBAR_CURSOR_ACTIVE] = nk_rgba(160, 160, 160, 255);
    table[NK_COLOR_TAB_HEADER] = nk_rgba(180, 180, 180, 255);
    table[NK_COLOR_KNOB] = table[NK_COLOR_SLIDER];
    table[NK_COLOR_KNOB_CURSOR] = table[NK_COLOR_SLIDER_CURSOR];
    table[NK_COLOR_KNOB_CURSOR_HOVER] = table[NK_COLOR_SLIDER_CURSOR_HOVER];
    table[NK_COLOR_KNOB_CURSOR_ACTIVE] = table[NK_COLOR_SLIDER_CURSOR_ACTIVE];
    nk_style_from_table(ctx, table);
  } else if (theme == THEME_RED) {
    table[NK_COLOR_TEXT] = nk_rgba(190, 190, 190, 255);
    table[NK_COLOR_WINDOW] = nk_rgba(30, 33, 40, 215);
    table[NK_COLOR_HEADER] = nk_rgba(181, 45, 69, 220);
    table[NK_COLOR_BORDER] = nk_rgba(51, 55, 67, 255);
    table[NK_COLOR_BUTTON] = nk_rgba(181, 45, 69, 255);
    table[NK_COLOR_BUTTON_HOVER] = nk_rgba(190, 50, 70, 255);
    table[NK_COLOR_BUTTON_ACTIVE] = nk_rgba(195, 55, 75, 255);
    table[NK_COLOR_TOGGLE] = nk_rgba(51, 55, 67, 255);
    table[NK_COLOR_TOGGLE_HOVER] = nk_rgba(45, 60, 60, 255);
    table[NK_COLOR_TOGGLE_CURSOR] = nk_rgba(181, 45, 69, 255);
    table[NK_COLOR_SELECT] = nk_rgba(51, 55, 67, 255);
    table[NK_COLOR_SELECT_ACTIVE] = nk_rgba(181, 45, 69, 255);
    table[NK_COLOR_SLIDER] = nk_rgba(51, 55, 67, 255);
    table[NK_COLOR_SLIDER_CURSOR] = nk_rgba(181, 45, 69, 255);
    table[NK_COLOR_SLIDER_CURSOR_HOVER] = nk_rgba(186, 50, 74, 255);
    table[NK_COLOR_SLIDER_CURSOR_ACTIVE] = nk_rgba(191, 55, 79, 255);
    table[NK_COLOR_PROPERTY] = nk_rgba(51, 55, 67, 255);
    table[NK_COLOR_EDIT] = nk_rgba(51, 55, 67, 225);
    table[NK_COLOR_EDIT_CURSOR] = nk_rgba(190, 190, 190, 255);
    table[NK_COLOR_COMBO] = nk_rgba(51, 55, 67, 255);
    table[NK_COLOR_CHART] = nk_rgba(51, 55, 67, 255);
    table[NK_COLOR_CHART_COLOR] = nk_rgba(170, 40, 60, 255);
    table[NK_COLOR_CHART_COLOR_HIGHLIGHT] = nk_rgba(255, 0, 0, 255);
    table[NK_COLOR_SCROLLBAR] = nk_rgba(30, 33, 40, 255);
    table[NK_COLOR_SCROLLBAR_CURSOR] = nk_rgba(64, 84, 95, 255);
    table[NK_COLOR_SCROLLBAR_CURSOR_HOVER] = nk_rgba(70, 90, 100, 255);
    table[NK_COLOR_SCROLLBAR_CURSOR_ACTIVE] = nk_rgba(75, 95, 105, 255);
    table[NK_COLOR_TAB_HEADER] = nk_rgba(181, 45, 69, 220);
    table[NK_COLOR_KNOB] = table[NK_COLOR_SLIDER];
    table[NK_COLOR_KNOB_CURSOR] = table[NK_COLOR_SLIDER_CURSOR];
    table[NK_COLOR_KNOB_CURSOR_HOVER] = table[NK_COLOR_SLIDER_CURSOR_HOVER];
    table[NK_COLOR_KNOB_CURSOR_ACTIVE] = table[NK_COLOR_SLIDER_CURSOR_ACTIVE];
    nk_style_from_table(ctx, table);
  } else if (theme == THEME_BLUE) {
    table[NK_COLOR_TEXT] = nk_rgba(20, 20, 20, 255);
    table[NK_COLOR_WINDOW] = nk_rgba(202, 212, 214, 215);
    table[NK_COLOR_HEADER] = nk_rgba(137, 182, 224, 220);
    table[NK_COLOR_BORDER] = nk_rgba(140, 159, 173, 255);
    table[NK_COLOR_BUTTON] = nk_rgba(137, 182, 224, 255);
    table[NK_COLOR_BUTTON_HOVER] = nk_rgba(142, 187, 229, 255);
    table[NK_COLOR_BUTTON_ACTIVE] = nk_rgba(147, 192, 234, 255);
    table[NK_COLOR_TOGGLE] = nk_rgba(177, 210, 210, 255);
    table[NK_COLOR_TOGGLE_HOVER] = nk_rgba(182, 215, 215, 255);
    table[NK_COLOR_TOGGLE_CURSOR] = nk_rgba(137, 182, 224, 255);
    table[NK_COLOR_SELECT] = nk_rgba(177, 210, 210, 255);
    table[NK_COLOR_SELECT_ACTIVE] = nk_rgba(137, 182, 224, 255);
    table[NK_COLOR_SLIDER] = nk_rgba(177, 210, 210, 255);
    table[NK_COLOR_SLIDER_CURSOR] = nk_rgba(137, 182, 224, 245);
    table[NK_COLOR_SLIDER_CURSOR_HOVER] = nk_rgba(142, 188, 229, 255);
    table[NK_COLOR_SLIDER_CURSOR_ACTIVE] = nk_rgba(147, 193, 234, 255);
    table[NK_COLOR_PROPERTY] = nk_rgba(210, 210, 210, 255);
    table[NK_COLOR_EDIT] = nk_rgba(210, 210, 210, 225);
    table[NK_COLOR_EDIT_CURSOR] = nk_rgba(20, 20, 20, 255);
    table[NK_COLOR_COMBO] = nk_rgba(210, 210, 210, 255);
    table[NK_COLOR_CHART] = nk_rgba(210, 210, 210, 255);
    table[NK_COLOR_CHART_COLOR] = nk_rgba(137, 182, 224, 255);
    table[NK_COLOR_CHART_COLOR_HIGHLIGHT] = nk_rgba(255, 0, 0, 255);
    table[NK_COLOR_SCROLLBAR] = nk_rgba(190, 200, 200, 255);
    table[NK_COLOR_SCROLLBAR_CURSOR] = nk_rgba(64, 84, 95, 255);
    table[NK_COLOR_SCROLLBAR_CURSOR_HOVER] = nk_rgba(70, 90, 100, 255);
    table[NK_COLOR_SCROLLBAR_CURSOR_ACTIVE] = nk_rgba(75, 95, 105, 255);
    table[NK_COLOR_TAB_HEADER] = nk_rgba(156, 193, 220, 255);
    table[NK_COLOR_KNOB] = table[NK_COLOR_SLIDER];
    table[NK_COLOR_KNOB_CURSOR] = table[NK_COLOR_SLIDER_CURSOR];
    table[NK_COLOR_KNOB_CURSOR_HOVER] = table[NK_COLOR_SLIDER_CURSOR_HOVER];
    table[NK_COLOR_KNOB_CURSOR_ACTIVE] = table[NK_COLOR_SLIDER_CURSOR_ACTIVE];
    nk_style_from_table(ctx, table);
  } else if (theme == THEME_DARK) {
    table[NK_COLOR_TEXT] = nk_rgba(210, 210, 210, 255);
    table[NK_COLOR_WINDOW] = nk_rgba(57, 67, 71, 215);
    table[NK_COLOR_HEADER] = nk_rgba(51, 51, 56, 220);
    table[NK_COLOR_BORDER] = nk_rgba(46, 46, 46, 255);
    table[NK_COLOR_BUTTON] = nk_rgba(48, 83, 111, 255);
    table[NK_COLOR_BUTTON_HOVER] = nk_rgba(58, 93, 121, 255);
    table[NK_COLOR_BUTTON_ACTIVE] = nk_rgba(63, 98, 126, 255);
    table[NK_COLOR_TOGGLE] = nk_rgba(50, 58, 61, 255);
    table[NK_COLOR_TOGGLE_HOVER] = nk_rgba(45, 53, 56, 255);
    table[NK_COLOR_TOGGLE_CURSOR] = nk_rgba(48, 83, 111, 255);
    table[NK_COLOR_SELECT] = nk_rgba(57, 67, 61, 255);
    table[NK_COLOR_SELECT_ACTIVE] = nk_rgba(48, 83, 111, 255);
    table[NK_COLOR_SLIDER] = nk_rgba(50, 58, 61, 255);
    table[NK_COLOR_SLIDER_CURSOR] = nk_rgba(48, 83, 111, 245);
    table[NK_COLOR_SLIDER_CURSOR_HOVER] = nk_rgba(53, 88, 116, 255);
    table[NK_COLOR_SLIDER_CURSOR_ACTIVE] = nk_rgba(58, 93, 121, 255);
    table[NK_COLOR_PROPERTY] = nk_rgba(50, 58, 61, 255);
    table[NK_COLOR_EDIT] = nk_rgba(50, 58, 61, 225);
    table[NK_COLOR_EDIT_CURSOR] = nk_rgba(210, 210, 210, 255);
    table[NK_COLOR_COMBO] = nk_rgba(50, 58, 61, 255);
    table[NK_COLOR_CHART] = nk_rgba(50, 58, 61, 255);
    table[NK_COLOR_CHART_COLOR] = nk_rgba(48, 83, 111, 255);
    table[NK_COLOR_CHART_COLOR_HIGHLIGHT] = nk_rgba(255, 0, 0, 255);
    table[NK_COLOR_SCROLLBAR] = nk_rgba(50, 58, 61, 255);
    table[NK_COLOR_SCROLLBAR_CURSOR] = nk_rgba(48, 83, 111, 255);
    table[NK_COLOR_SCROLLBAR_CURSOR_HOVER] = nk_rgba(53, 88, 116, 255);
    table[NK_COLOR_SCROLLBAR_CURSOR_ACTIVE] = nk_rgba(58, 93, 121, 255);
    table[NK_COLOR_TAB_HEADER] = nk_rgba(48, 83, 111, 255);
    table[NK_COLOR_KNOB] = table[NK_COLOR_SLIDER];
    table[NK_COLOR_KNOB_CURSOR] = table[NK_COLOR_SLIDER_CURSOR];
    table[NK_COLOR_KNOB_CURSOR_HOVER] = table[NK_COLOR_SLIDER_CURSOR_HOVER];
    table[NK_COLOR_KNOB_CURSOR_ACTIVE] = table[NK_COLOR_SLIDER_CURSOR_ACTIVE];
    nk_style_from_table(ctx, table);
  } else if (theme == THEME_DRACULA) {
    struct nk_color background = nk_rgba(40, 42, 54, 255);
    struct nk_color currentline = nk_rgba(68, 71, 90, 255);
    struct nk_color foreground = nk_rgba(248, 248, 242, 255);
    struct nk_color comment = nk_rgba(98, 114, 164, 255);
    /* struct nk_color cyan = nk_rgba(139, 233, 253, 255); */
    /* struct nk_color green = nk_rgba(80, 250, 123, 255); */
    /* struct nk_color orange = nk_rgba(255, 184, 108, 255); */
    struct nk_color pink = nk_rgba(255, 121, 198, 255);
    struct nk_color purple = nk_rgba(189, 147, 249, 255);
    /* struct nk_color red = nk_rgba(255, 85, 85, 255); */
    /* struct nk_color yellow = nk_rgba(241, 250, 140, 255); */
    table[NK_COLOR_TEXT] = foreground;
    table[NK_COLOR_WINDOW] = background;
    table[NK_COLOR_HEADER] = currentline;
    table[NK_COLOR_BORDER] = currentline;
    table[NK_COLOR_BUTTON] = currentline;
    table[NK_COLOR_BUTTON_HOVER] = comment;
    table[NK_COLOR_BUTTON_ACTIVE] = purple;
    table[NK_COLOR_TOGGLE] = currentline;
    table[NK_COLOR_TOGGLE_HOVER] = comment;
    table[NK_COLOR_TOGGLE_CURSOR] = pink;
    table[NK_COLOR_SELECT] = currentline;
    table[NK_COLOR_SELECT_ACTIVE] = comment;
    table[NK_COLOR_SLIDER] = background;
    table[NK_COLOR_SLIDER_CURSOR] = currentline;
    table[NK_COLOR_SLIDER_CURSOR_HOVER] = comment;
    table[NK_COLOR_SLIDER_CURSOR_ACTIVE] = comment;
    table[NK_COLOR_PROPERTY] = currentline;
    table[NK_COLOR_EDIT] = currentline;
    table[NK_COLOR_EDIT_CURSOR] = foreground;
    table[NK_COLOR_COMBO] = currentline;
    table[NK_COLOR_CHART] = currentline;
    table[NK_COLOR_CHART_COLOR] = comment;
    table[NK_COLOR_CHART_COLOR_HIGHLIGHT] = purple;
    table[NK_COLOR_SCROLLBAR] = background;
    table[NK_COLOR_SCROLLBAR_CURSOR] = currentline;
    table[NK_COLOR_SCROLLBAR_CURSOR_HOVER] = comment;
    table[NK_COLOR_SCROLLBAR_CURSOR_ACTIVE] = purple;
    table[NK_COLOR_TAB_HEADER] = currentline;
    table[NK_COLOR_KNOB] = table[NK_COLOR_SLIDER];
    table[NK_COLOR_KNOB_CURSOR] = table[NK_COLOR_SLIDER_CURSOR];
    table[NK_COLOR_KNOB_CURSOR_HOVER] = table[NK_COLOR_SLIDER_CURSOR_HOVER];
    table[NK_COLOR_KNOB_CURSOR_ACTIVE] = table[NK_COLOR_SLIDER_CURSOR_ACTIVE];
    nk_style_from_table(ctx, table);
  } else if (theme == THEME_CATPPUCCIN_LATTE) {
    /*struct nk_color rosewater = nk_rgba(220, 138, 120, 255);*/
    /*struct nk_color flamingo = nk_rgba(221, 120, 120, 255);*/
    struct nk_color pink = nk_rgba(234, 118, 203, 255);
    struct nk_color mauve = nk_rgba(136, 57, 239, 255);
    /*struct nk_color red = nk_rgba(210, 15, 57, 255);*/
    /*struct nk_color maroon = nk_rgba(230, 69, 83, 255);*/
    /*struct nk_color peach = nk_rgba(254, 100, 11, 255);*/
    struct nk_color yellow = nk_rgba(223, 142, 29, 255);
    /*struct nk_color green = nk_rgba(64, 160, 43, 255);*/
    struct nk_color teal = nk_rgba(23, 146, 153, 255);
    /*struct nk_color sky = nk_rgba(4, 165, 229, 255);*/
    /*struct nk_color sapphire = nk_rgba(32, 159, 181, 255);*/
    /*struct nk_color blue = nk_rgba(30, 102, 245, 255);*/
    /*struct nk_color lavender = nk_rgba(114, 135, 253, 255);*/
    struct nk_color text = nk_rgba(76, 79, 105, 255);
    /*struct nk_color subtext1 = nk_rgba(92, 95, 119, 255);*/
    /*struct nk_color subtext0 = nk_rgba(108, 111, 133, 255);*/
    struct nk_color overlay2 = nk_rgba(124, 127, 147, 55);
    /*struct nk_color overlay1 = nk_rgba(140, 143, 161, 255);*/
    struct nk_color overlay0 = nk_rgba(156, 160, 176, 255);
    struct nk_color surface2 = nk_rgba(172, 176, 190, 255);
    struct nk_color surface1 = nk_rgba(188, 192, 204, 255);
    struct nk_color surface0 = nk_rgba(204, 208, 218, 255);
    struct nk_color base = nk_rgba(239, 241, 245, 255);
    struct nk_color mantle = nk_rgba(230, 233, 239, 255);
    /*struct nk_color crust = nk_rgba(220, 224, 232, 255);*/
    table[NK_COLOR_TEXT] = text;
    table[NK_COLOR_WINDOW] = base;
    table[NK_COLOR_HEADER] = mantle;
    table[NK_COLOR_BORDER] = mantle;
    table[NK_COLOR_BUTTON] = surface0;
    table[NK_COLOR_BUTTON_HOVER] = overlay2;
    table[NK_COLOR_BUTTON_ACTIVE] = overlay0;
    table[NK_COLOR_TOGGLE] = surface2;
    table[NK_COLOR_TOGGLE_HOVER] = overlay2;
    table[NK_COLOR_TOGGLE_CURSOR] = yellow;
    table[NK_COLOR_SELECT] = surface0;
    table[NK_COLOR_SELECT_ACTIVE] = overlay0;
    table[NK_COLOR_SLIDER] = surface1;
    table[NK_COLOR_SLIDER_CURSOR] = teal;
    table[NK_COLOR_SLIDER_CURSOR_HOVER] = teal;
    table[NK_COLOR_SLIDER_CURSOR_ACTIVE] = teal;
    table[NK_COLOR_PROPERTY] = surface0;
    table[NK_COLOR_EDIT] = surface0;
    table[NK_COLOR_EDIT_CURSOR] = mauve;
    table[NK_COLOR_COMBO] = surface0;
    table[NK_COLOR_CHART] = surface0;
    table[NK_COLOR_CHART_COLOR] = teal;
    table[NK_COLOR_CHART_COLOR_HIGHLIGHT] = mauve;
    table[NK_COLOR_SCROLLBAR] = surface0;
    table[NK_COLOR_SCROLLBAR_CURSOR] = overlay0;
    table[NK_COLOR_SCROLLBAR_CURSOR_HOVER] = mauve;
    table[NK_COLOR_SCROLLBAR_CURSOR_ACTIVE] = mauve;
    table[NK_COLOR_TAB_HEADER] = surface0;
    table[NK_COLOR_KNOB] = table[NK_COLOR_SLIDER];
    table[NK_COLOR_KNOB_CURSOR] = pink;
    table[NK_COLOR_KNOB_CURSOR_HOVER] = pink;
    table[NK_COLOR_KNOB_CURSOR_ACTIVE] = pink;
    nk_style_from_table(ctx, table);
  } else if (theme == THEME_CATPPUCCIN_FRAPPE) {
    /*struct nk_color rosewater = nk_rgba(242, 213, 207, 255);*/
    /*struct nk_color flamingo = nk_rgba(238, 190, 190, 255);*/
    struct nk_color pink = nk_rgba(244, 184, 228, 255);
    /*struct nk_color mauve = nk_rgba(202, 158, 230, 255);*/
    /*struct nk_color red = nk_rgba(231, 130, 132, 255);*/
    /*struct nk_color maroon = nk_rgba(234, 153, 156, 255);*/
    /*struct nk_color peach = nk_rgba(239, 159, 118, 255);*/
    /*struct nk_color yellow = nk_rgba(229, 200, 144, 255);*/
    struct nk_color green = nk_rgba(166, 209, 137, 255);
    /*struct nk_color teal = nk_rgba(129, 200, 190, 255);*/
    /*struct nk_color sky = nk_rgba(153, 209, 219, 255);*/
    /*struct nk_color sapphire = nk_rgba(133, 193, 220, 255);*/
    /*struct nk_color blue = nk_rgba(140, 170, 238, 255);*/
    struct nk_color lavender = nk_rgba(186, 187, 241, 255);
    struct nk_color text = nk_rgba(198, 208, 245, 255);
    /*struct nk_color subtext1 = nk_rgba(181, 191, 226, 255);*/
    /*struct nk_color subtext0 = nk_rgba(165, 173, 206, 255);*/
    struct nk_color overlay2 = nk_rgba(148, 156, 187, 255);
    struct nk_color overlay1 = nk_rgba(131, 139, 167, 255);
    struct nk_color overlay0 = nk_rgba(115, 121, 148, 255);
    struct nk_color surface2 = nk_rgba(98, 104, 128, 255);
    struct nk_color surface1 = nk_rgba(81, 87, 109, 255);
    struct nk_color surface0 = nk_rgba(65, 69, 89, 255);
    struct nk_color base = nk_rgba(48, 52, 70, 255);
    struct nk_color mantle = nk_rgba(41, 44, 60, 255);
    /*struct nk_color crust = nk_rgba(35, 38, 52, 255);*/
    table[NK_COLOR_TEXT] = text;
    table[NK_COLOR_WINDOW] = base;
    table[NK_COLOR_HEADER] = mantle;
    table[NK_COLOR_BORDER] = mantle;
    table[NK_COLOR_BUTTON] = surface0;
    table[NK_COLOR_BUTTON_HOVER] = overlay1;
    table[NK_COLOR_BUTTON_ACTIVE] = overlay0;
    table[NK_COLOR_TOGGLE] = surface2;
    table[NK_COLOR_TOGGLE_HOVER] = overlay2;
    table[NK_COLOR_TOGGLE_CURSOR] = pink;
    table[NK_COLOR_SELECT] = surface0;
    table[NK_COLOR_SELECT_ACTIVE] = overlay0;
    table[NK_COLOR_SLIDER] = surface1;
    table[NK_COLOR_SLIDER_CURSOR] = green;
    table[NK_COLOR_SLIDER_CURSOR_HOVER] = green;
    table[NK_COLOR_SLIDER_CURSOR_ACTIVE] = green;
    table[NK_COLOR_PROPERTY] = surface0;
    table[NK_COLOR_EDIT] = surface0;
    table[NK_COLOR_EDIT_CURSOR] = pink;
    table[NK_COLOR_COMBO] = surface0;
    table[NK_COLOR_CHART] = surface0;
    table[NK_COLOR_CHART_COLOR] = lavender;
    table[NK_COLOR_CHART_COLOR_HIGHLIGHT] = pink;
    table[NK_COLOR_SCROLLBAR] = surface0;
    table[NK_COLOR_SCROLLBAR_CURSOR] = overlay0;
    table[NK_COLOR_SCROLLBAR_CURSOR_HOVER] = lavender;
    table[NK_COLOR_SCROLLBAR_CURSOR_ACTIVE] = lavender;
    table[NK_COLOR_TAB_HEADER] = surface0;
    table[NK_COLOR_KNOB] = table[NK_COLOR_SLIDER];
    table[NK_COLOR_KNOB_CURSOR] = pink;
    table[NK_COLOR_KNOB_CURSOR_HOVER] = pink;
    table[NK_COLOR_KNOB_CURSOR_ACTIVE] = pink;
    nk_style_from_table(ctx, table);
  } else if (theme == THEME_CATPPUCCIN_MACCHIATO) {
    /*struct nk_color rosewater = nk_rgba(244, 219, 214, 255);*/
    /*struct nk_color flamingo = nk_rgba(240, 198, 198, 255);*/
    struct nk_color pink = nk_rgba(245, 189, 230, 255);
    /*struct nk_color mauve = nk_rgba(198, 160, 246, 255);*/
    /*struct nk_color red = nk_rgba(237, 135, 150, 255);*/
    /*struct nk_color maroon = nk_rgba(238, 153, 160, 255);*/
    /*struct nk_color peach = nk_rgba(245, 169, 127, 255);*/
    struct nk_color yellow = nk_rgba(238, 212, 159, 255);
    struct nk_color green = nk_rgba(166, 218, 149, 255);
    /*struct nk_color teal = nk_rgba(139, 213, 202, 255);*/
    /*struct nk_color sky = nk_rgba(145, 215, 227, 255);*/
    /*struct nk_color sapphire = nk_rgba(125, 196, 228, 255);*/
    /*struct nk_color blue = nk_rgba(138, 173, 244, 255);*/
    struct nk_color lavender = nk_rgba(183, 189, 248, 255);
    struct nk_color text = nk_rgba(202, 211, 245, 255);
    /*struct nk_color subtext1 = nk_rgba(184, 192, 224, 255);*/
    /*struct nk_color subtext0 = nk_rgba(165, 173, 203, 255);*/
    struct nk_color overlay2 = nk_rgba(147, 154, 183, 255);
    struct nk_color overlay1 = nk_rgba(128, 135, 162, 255);
    struct nk_color overlay0 = nk_rgba(110, 115, 141, 255);
    struct nk_color surface2 = nk_rgba(91, 96, 120, 255);
    struct nk_color surface1 = nk_rgba(73, 77, 100, 255);
    struct nk_color surface0 = nk_rgba(54, 58, 79, 255);
    struct nk_color base = nk_rgba(36, 39, 58, 255);
    struct nk_color mantle = nk_rgba(30, 32, 48, 255);
    /*struct nk_color crust = nk_rgba(24, 25, 38, 255);*/
    table[NK_COLOR_TEXT] = text;
    table[NK_COLOR_WINDOW] = base;
    table[NK_COLOR_HEADER] = mantle;
    table[NK_COLOR_BORDER] = mantle;
    table[NK_COLOR_BUTTON] = surface0;
    table[NK_COLOR_BUTTON_HOVER] = overlay1;
    table[NK_COLOR_BUTTON_ACTIVE] = overlay0;
    table[NK_COLOR_TOGGLE] = surface2;
    table[NK_COLOR_TOGGLE_HOVER] = overlay2;
    table[NK_COLOR_TOGGLE_CURSOR] = yellow;
    table[NK_COLOR_SELECT] = surface0;
    table[NK_COLOR_SELECT_ACTIVE] = overlay0;
    table[NK_COLOR_SLIDER] = surface1;
    table[NK_COLOR_SLIDER_CURSOR] = green;
    table[NK_COLOR_SLIDER_CURSOR_HOVER] = green;
    table[NK_COLOR_SLIDER_CURSOR_ACTIVE] = green;
    table[NK_COLOR_PROPERTY] = surface0;
    table[NK_COLOR_EDIT] = surface0;
    table[NK_COLOR_EDIT_CURSOR] = pink;
    table[NK_COLOR_COMBO] = surface0;
    table[NK_COLOR_CHART] = surface0;
    table[NK_COLOR_CHART_COLOR] = lavender;
    table[NK_COLOR_CHART_COLOR_HIGHLIGHT] = yellow;
    table[NK_COLOR_SCROLLBAR] = surface0;
    table[NK_COLOR_SCROLLBAR_CURSOR] = overlay0;
    table[NK_COLOR_SCROLLBAR_CURSOR_HOVER] = lavender;
    table[NK_COLOR_SCROLLBAR_CURSOR_ACTIVE] = lavender;
    table[NK_COLOR_TAB_HEADER] = surface0;
    table[NK_COLOR_KNOB] = table[NK_COLOR_SLIDER];
    table[NK_COLOR_KNOB_CURSOR] = pink;
    table[NK_COLOR_KNOB_CURSOR_HOVER] = pink;
    table[NK_COLOR_KNOB_CURSOR_ACTIVE] = pink;
    nk_style_from_table(ctx, table); 
  } else if (theme == THEME_CATPPUCCIN_MOCHA) {
    /*struct nk_color rosewater = nk_rgba(245, 224, 220, 255);*/
    /*struct nk_color flamingo = nk_rgba(242, 205, 205, 255);*/
    struct nk_color pink = nk_rgba(245, 194, 231, 255);
    /*struct nk_color mauve = nk_rgba(203, 166, 247, 255);*/
    /*struct nk_color red = nk_rgba(243, 139, 168, 255);*/
    /*struct nk_color maroon = nk_rgba(235, 160, 172, 255);*/
    /*struct nk_color peach = nk_rgba(250, 179, 135, 255);*/
    /*struct nk_color yellow = nk_rgba(249, 226, 175, 255);*/
    struct nk_color green = nk_rgba(166, 227, 161, 255);
    /*struct nk_color teal = nk_rgba(148, 226, 213, 255);*/
    /*struct nk_color sky = nk_rgba(137, 220, 235, 255);*/
    /*struct nk_color sapphire = nk_rgba(116, 199, 236, 255);*/
    /*struct nk_color blue = nk_rgba(137, 180, 250, 255);*/
    struct nk_color lavender = nk_rgba(180, 190, 254, 255);
    struct nk_color text = nk_rgba(205, 214, 244, 255);
    /*struct nk_color subtext1 = nk_rgba(186, 194, 222, 255);*/
    /*struct nk_color subtext0 = nk_rgba(166, 173, 200, 255);*/
    struct nk_color overlay2 = nk_rgba(147, 153, 178, 255);
    struct nk_color overlay1 = nk_rgba(127, 132, 156, 255);
    struct nk_color overlay0 = nk_rgba(108, 112, 134, 255);
    struct nk_color surface2 = nk_rgba(88, 91, 112, 255);
    struct nk_color surface1 = nk_rgba(69, 71, 90, 255);
    struct nk_color surface0 = nk_rgba(49, 50, 68, 255);
    struct nk_color base = nk_rgba(30, 30, 46, 255);
    struct nk_color mantle = nk_rgba(24, 24, 37, 255);
    /*struct nk_color crust = nk_rgba(17, 17, 27, 255);*/
    table[NK_COLOR_TEXT] = text;
    table[NK_COLOR_WINDOW] = base;
    table[NK_COLOR_HEADER] = mantle;
    table[NK_COLOR_BORDER] = mantle;
    table[NK_COLOR_BUTTON] = surface0;
    table[NK_COLOR_BUTTON_HOVER] = overlay1;
    table[NK_COLOR_BUTTON_ACTIVE] = overlay0;
    table[NK_COLOR_TOGGLE] = surface2;
    table[NK_COLOR_TOGGLE_HOVER] = overlay2;
    table[NK_COLOR_TOGGLE_CURSOR] = lavender;
    table[NK_COLOR_SELECT] = surface0;
    table[NK_COLOR_SELECT_ACTIVE] = overlay0;
    table[NK_COLOR_SLIDER] = surface1;
    table[NK_COLOR_SLIDER_CURSOR] = green;
    table[NK_COLOR_SLIDER_CURSOR_HOVER] = green;
    table[NK_COLOR_SLIDER_CURSOR_ACTIVE] = green;
    table[NK_COLOR_PROPERTY] = surface0;
    table[NK_COLOR_EDIT] = surface0;
    table[NK_COLOR_EDIT_CURSOR] = lavender;
    table[NK_COLOR_COMBO] = surface0;
    table[NK_COLOR_CHART] = surface0;
    table[NK_COLOR_CHART_COLOR] = lavender;
    table[NK_COLOR_CHART_COLOR_HIGHLIGHT] = pink;
    table[NK_COLOR_SCROLLBAR] = surface0;
    table[NK_COLOR_SCROLLBAR_CURSOR] = overlay0;
    table[NK_COLOR_SCROLLBAR_CURSOR_HOVER] = lavender;
    table[NK_COLOR_SCROLLBAR_CURSOR_ACTIVE] = pink;
    table[NK_COLOR_TAB_HEADER] = surface0;
    table[NK_COLOR_KNOB] = table[NK_COLOR_SLIDER];
    table[NK_COLOR_KNOB_CURSOR] = pink;
    table[NK_COLOR_KNOB_CURSOR_HOVER] = pink;
    table[NK_COLOR_KNOB_CURSOR_ACTIVE] = pink;
    nk_style_from_table(ctx, table);   
  } else {
    nk_style_default(ctx);
  }
}

static nk_bool
canvas_begin(struct nk_context *ctx, struct nk_canvas *canvas, nk_flags flags,
    int x, int y, int width, int height, struct nk_color background_color)
{
    /* save style properties which will be overwritten */
    canvas->panel_padding = ctx->style.window.padding;
    canvas->item_spacing = ctx->style.window.spacing;
    canvas->window_background = ctx->style.window.fixed_background;

    /* use the complete window space and set background */
    ctx->style.window.spacing = nk_vec2(0,0);
    ctx->style.window.padding = nk_vec2(0,0);
    ctx->style.window.fixed_background = nk_style_item_color(background_color);

    /* create/update window and set position + size */
    if (!nk_begin(ctx, "Canvas", nk_rect(x, y, width, height), NK_WINDOW_NO_SCROLLBAR|flags))
        return nk_false;

    /* allocate the complete window space for drawing */
    {
        struct nk_rect total_space;
        total_space = nk_window_get_content_region(ctx);
        nk_layout_row_dynamic(ctx, total_space.h, 1);
        nk_widget(&total_space, ctx);
        canvas->painter = nk_window_get_canvas(ctx);
    }

    return nk_true;
}

static void
canvas_end(struct nk_context *ctx, struct nk_canvas *canvas)
{
    nk_end(ctx);
    ctx->style.window.spacing = canvas->panel_padding;
    ctx->style.window.padding = canvas->item_spacing;
    ctx->style.window.fixed_background = canvas->window_background;
}

static void draw_rulers(struct nk_context *ctx, struct nk_canvas canvas, struct nk_rect rect)
{
    nk_fill_rect(canvas.painter, nk_rect(rect.x, rect.y, rect.w, 10), 5, nk_rgb(247, 230, 154));
}

static void
canvas(struct nk_context *ctx, struct nk_rect rect)
{
    struct nk_canvas canvas;
    if (canvas_begin(ctx, &canvas, NK_WINDOW_BORDER|NK_WINDOW_CLOSABLE|NK_WINDOW_TITLE, rect.x, rect.y, rect.w, rect.h, nk_rgb(250,250,250))) {
        float x = canvas.painter->clip.x, y = canvas.painter->clip.y;

        nk_fill_rect(canvas.painter, nk_rect(x + 15, y + 15, 210, 210), 5, nk_rgb(247, 230, 154));
        nk_fill_rect(canvas.painter, nk_rect(x + 20, y + 20, 200, 200), 5, nk_rgb(188, 174, 118));
        /* nk_draw_text(canvas.painter, nk_rect(x + 30, y + 30, 150, 20), "Text to draw", 12, &font->handle, nk_rgb(188,174,118), nk_rgb(0,0,0)); */
        nk_fill_rect(canvas.painter, nk_rect(x + 250, y + 20, 100, 100), 0, nk_rgb(0,0,255));
        nk_fill_circle(canvas.painter, nk_rect(x + 20, y + 250, 100, 100), nk_rgb(255,0,0));
        nk_fill_triangle(canvas.painter, x + 250, y + 250, x + 350, y + 250, x + 300, y + 350, nk_rgb(0,255,0));
        nk_fill_arc(canvas.painter, x + 300, y + 420, 50, 0, 3.141592654f * 3.0f / 4.0f, nk_rgb(255,255,0));

        {
            float points[12];
            points[0]  = x + 200; points[1]  = y + 250;
            points[2]  = x + 250; points[3]  = y + 350;
            points[4]  = x + 225; points[5]  = y + 350;
            points[6]  = x + 200; points[7]  = y + 300;
            points[8]  = x + 175; points[9]  = y + 350;
            points[10] = x + 150; points[11] = y + 350;
            nk_fill_polygon(canvas.painter, points, 6, nk_rgb(0,0,0));
        }

        {
            float points[12];
            points[0]  = x + 200; points[1]  = y + 370;
            points[2]  = x + 250; points[3]  = y + 470;
            points[4]  = x + 225; points[5]  = y + 470;
            points[6]  = x + 200; points[7]  = y + 420;
            points[8]  = x + 175; points[9]  = y + 470;
            points[10] = x + 150; points[11] = y + 470;
            nk_stroke_polygon(canvas.painter, points, 6, 4, nk_rgb(0,0,0));
        }

        {
            float points[8];
            points[0]  = x + 250; points[1]  = y + 200;
            points[2]  = x + 275; points[3]  = y + 220;
            points[4]  = x + 325; points[5]  = y + 170;
            points[6]  = x + 350; points[7]  = y + 200;
            nk_stroke_polyline(canvas.painter, points, 4, 2, nk_rgb(255,128,0));
        }

        nk_stroke_line(canvas.painter, x + 15, y + 10, x + 200, y + 10, 2.0f, nk_rgb(189,45,75));
        nk_stroke_rect(canvas.painter, nk_rect(x + 370, y + 20, 100, 100), 10, 3, nk_rgb(0,0,255));
        nk_stroke_curve(canvas.painter, x + 380, y + 200, x + 405, y + 270, x + 455, y + 120, x + 480, y + 200, 2, nk_rgb(0,150,220));
        nk_stroke_circle(canvas.painter, nk_rect(x + 20, y + 370, 100, 100), 5, nk_rgb(0,255,120));
        nk_stroke_triangle(canvas.painter, x + 370, y + 250, x + 470, y + 250, x + 420, y + 350, 6, nk_rgb(255,0,143));
        nk_stroke_arc(canvas.painter, x + 420, y + 420, 50, 0, 3.141592654f * 3.0f / 4.0f, 5, nk_rgb(0,255,255));

        draw_rulers(ctx, canvas, rect);
    }
    canvas_end(ctx, &canvas);
}

static void
calculator(struct nk_context *ctx)
{
    if (nk_begin(ctx, "Calculator", nk_rect(10, 10, 180, 250),
        NK_WINDOW_BORDER|NK_WINDOW_NO_SCROLLBAR|NK_WINDOW_MOVABLE)) {
        static int set = 0, prev = 0, op = 0;
        static const char numbers[] = "789456123";
        static const char ops[] = "+-*/";
        static double a = 0, b = 0;
        static double *current = &a;

        size_t i = 0;
        int solve = 0;
        {int len; char buffer[256];
        nk_layout_row_dynamic(ctx, 35, 1);
        len = snprintf(buffer, 256, "%.2f", *current);
        nk_edit_string(ctx, NK_EDIT_SIMPLE, buffer, &len, 255, nk_filter_float);
        buffer[len] = 0;
        *current = atof(buffer);}

        nk_layout_row_dynamic(ctx, 35, 4);
        for (i = 0; i < 16; ++i) {
            if (i >= 12 && i < 15) {
                if (i > 12) continue;
                if (nk_button_label(ctx, "C")) {
                    a = b = op = 0; current = &a; set = 0;
                } if (nk_button_label(ctx, "0")) {
                    *current = *current*10.0f; set = 0;
                } if (nk_button_label(ctx, "=")) {
                    solve = 1; prev = op; op = 0;
                }
            } else if (((i+1) % 4)) {
                if (nk_button_text(ctx, &numbers[(i/4)*3+i%4], 1)) {
                    *current = *current * 10.0f + numbers[(i/4)*3+i%4] - '0';
                    set = 0;
                }
            } else if (nk_button_text(ctx, &ops[i/4], 1)) {
                if (!set) {
                    if (current != &b) {
                        current = &b;
                    } else {
                        prev = op;
                        solve = 1;
                    }
                }
                op = ops[i/4];
                set = 1;
            }
        }
        if (solve) {
            if (prev == '+') a = a + b;
            if (prev == '-') a = a - b;
            if (prev == '*') a = a * b;
            if (prev == '/') a = a / b;
            current = &a;
            if (set) current = &b;
            b = 0; set = 0;
        }
    }
    nk_end(ctx);
}

#define INCLUDE_STYLE

static int
overview(struct nk_context *ctx, struct nk_rect rect)
{
    /* window flags */
    static nk_flags window_flags = NK_WINDOW_TITLE|NK_WINDOW_BORDER/*|NK_WINDOW_SCALABLE|NK_WINDOW_MOVABLE|NK_WINDOW_MINIMIZABLE*/|NK_WINDOW_SCROLL_AUTO_HIDE;
    nk_flags actual_window_flags = 0;

    /* widget flags */
    static nk_bool disable_widgets = nk_false;

    /* popups */
    static enum nk_style_header_align header_align = NK_HEADER_RIGHT;
    static nk_bool show_app_about = nk_false;

#ifdef INCLUDE_STYLE
    /* styles */
    static const char* themes[] = {"Black", "White", "Red", "Blue", "Dark", "Dracula",
      "Catppucin Latte", "Catppucin Frappe", "Catppucin Macchiato", "Catppucin Mocha"};
    static int current_theme = 0;
#endif

    /* window flags */
    ctx->style.window.header.align = header_align;

    actual_window_flags = window_flags;
    if (!(actual_window_flags & NK_WINDOW_TITLE))
        actual_window_flags &= ~(NK_WINDOW_MINIMIZABLE|NK_WINDOW_CLOSABLE);
    if (nk_begin(ctx, "Overview", rect, actual_window_flags)) {
#ifdef INCLUDE_STYLE
        /* style selector */
        nk_layout_row_dynamic(ctx, 0, 2);
        {
            int new_theme;
            nk_label(ctx, "Style:", NK_TEXT_LEFT);
            new_theme = nk_combo(ctx, themes, NK_LEN(themes), current_theme, 25, nk_vec2(200, 200));
            if (new_theme != current_theme) {
                current_theme = new_theme;
                set_style(ctx, current_theme);
            }
        }
#endif

        /* window flags */
        if (nk_tree_push(ctx, NK_TREE_TAB, "Window", NK_MINIMIZED)) {
            nk_layout_row_dynamic(ctx, 30, 2);
//            nk_checkbox_label(ctx, "Menu", &show_menu);
            nk_checkbox_flags_label(ctx, "Titlebar", &window_flags, NK_WINDOW_TITLE);
            nk_checkbox_flags_label(ctx, "Border", &window_flags, NK_WINDOW_BORDER);
            nk_checkbox_flags_label(ctx, "Resizable", &window_flags, NK_WINDOW_SCALABLE);
            nk_checkbox_flags_label(ctx, "Movable", &window_flags, NK_WINDOW_MOVABLE);
            nk_checkbox_flags_label(ctx, "No Scrollbar", &window_flags, NK_WINDOW_NO_SCROLLBAR);
            nk_checkbox_flags_label(ctx, "Minimizable", &window_flags, NK_WINDOW_MINIMIZABLE);
            nk_checkbox_flags_label(ctx, "Scale Left", &window_flags, NK_WINDOW_SCALE_LEFT);
            nk_checkbox_label(ctx, "Disable widgets", &disable_widgets);
            nk_tree_pop(ctx);
        }

        if (disable_widgets)
            nk_widget_disable_begin(ctx);

        if (nk_tree_push(ctx, NK_TREE_TAB, "Widgets", NK_MINIMIZED)) {
            enum options {A,B,C};
            static nk_bool checkbox_left_text_left;
            static nk_bool checkbox_centered_text_right;
            static nk_bool checkbox_right_text_right;
            static nk_bool checkbox_right_text_left;
            static int option_left;
            static int option_right;
            if (nk_tree_push(ctx, NK_TREE_NODE, "Text", NK_MINIMIZED)) {
                /* Text Widgets */
                nk_layout_row_dynamic(ctx, 20, 1);
                nk_label(ctx, "Label aligned left", NK_TEXT_LEFT);
                nk_label(ctx, "Label aligned centered", NK_TEXT_CENTERED);
                nk_label(ctx, "Label aligned right", NK_TEXT_RIGHT);
                nk_label_colored(ctx, "Blue text", NK_TEXT_LEFT, nk_rgb(0,0,255));
                nk_label_colored(ctx, "Yellow text", NK_TEXT_LEFT, nk_rgb(255,255,0));
                nk_text(ctx, "Text without /0", 15, NK_TEXT_RIGHT);

                nk_layout_row_static(ctx, 100, 200, 1);
                nk_label_wrap(ctx, "This is a very long line to hopefully get this text to be wrapped into multiple lines to show line wrapping");
                nk_layout_row_dynamic(ctx, 100, 1);
                nk_label_wrap(ctx, "This is another long text to show dynamic window changes on multiline text");
                nk_tree_pop(ctx);
            }

            if (nk_tree_push(ctx, NK_TREE_NODE, "Button", NK_MINIMIZED)) {
                /* Buttons Widgets */
                nk_layout_row_static(ctx, 30, 100, 3);
                if (nk_button_label(ctx, "Button"))
                    fprintf(stdout, "Button pressed!\n");
                nk_button_set_behavior(ctx, NK_BUTTON_REPEATER);
                if (nk_button_label(ctx, "Repeater"))
                    fprintf(stdout, "Repeater is being pressed!\n");
                nk_button_set_behavior(ctx, NK_BUTTON_DEFAULT);
                nk_button_color(ctx, nk_rgb(0,0,255));

                nk_layout_row_static(ctx, 30, 100, 2);
                nk_button_symbol_label(ctx, NK_SYMBOL_TRIANGLE_LEFT, "prev", NK_TEXT_RIGHT);
                nk_button_symbol_label(ctx, NK_SYMBOL_TRIANGLE_RIGHT, "next", NK_TEXT_LEFT);

                nk_tree_pop(ctx);
            }

            if (nk_tree_push(ctx, NK_TREE_NODE, "Basic", NK_MINIMIZED)) {
                /* Basic widgets */
                static int int_slider = 5;
                static float float_slider = 2.5f;
                static int int_knob = 5;
                static float float_knob = 2.5f;
                static nk_size prog_value = 40;
                static float property_float = 2;
                static int property_int = 10;
                static int property_neg = 10;

                static float range_float_min = 0;
                static float range_float_max = 100;
                static float range_float_value = 50;
                static int range_int_min = 0;
                static int range_int_value = 2048;
                static int range_int_max = 4096;
                static const float ratio[] = {120, 150};
                static int range_int_value_hidden = 2048;

                nk_layout_row_dynamic(ctx, 0, 1);
                nk_checkbox_label(ctx, "CheckLeft TextLeft", &checkbox_left_text_left);
                nk_checkbox_label_align(ctx, "CheckCenter TextRight", &checkbox_centered_text_right, NK_WIDGET_ALIGN_CENTERED | NK_WIDGET_ALIGN_MIDDLE, NK_TEXT_RIGHT);
                nk_checkbox_label_align(ctx, "CheckRight TextRight", &checkbox_right_text_right, NK_WIDGET_LEFT, NK_TEXT_RIGHT);
                nk_checkbox_label_align(ctx, "CheckRight TextLeft", &checkbox_right_text_left, NK_WIDGET_RIGHT, NK_TEXT_LEFT);

                nk_layout_row_static(ctx, 30, 80, 3);
                option_left = nk_option_label(ctx, "optionA", option_left == A) ? A : option_left;
                option_left = nk_option_label(ctx, "optionB", option_left == B) ? B : option_left;
                option_left = nk_option_label(ctx, "optionC", option_left == C) ? C : option_left;

                nk_layout_row_static(ctx, 30, 80, 3);
                option_right = nk_option_label_align(ctx, "optionA", option_right == A, NK_WIDGET_RIGHT, NK_TEXT_RIGHT) ? A : option_right;
                option_right = nk_option_label_align(ctx, "optionB", option_right == B, NK_WIDGET_RIGHT, NK_TEXT_RIGHT) ? B : option_right;
                option_right = nk_option_label_align(ctx, "optionC", option_right == C, NK_WIDGET_RIGHT, NK_TEXT_RIGHT) ? C : option_right;

                nk_layout_row(ctx, NK_STATIC, 30, 2, ratio);
                nk_labelf(ctx, NK_TEXT_LEFT, "Slider int");
                nk_slider_int(ctx, 0, &int_slider, 10, 1);

                nk_label(ctx, "Slider float", NK_TEXT_LEFT);
                nk_slider_float(ctx, 0, &float_slider, 5.0, 0.5f);
                nk_labelf(ctx, NK_TEXT_LEFT, "Progressbar: %u" , (int)prog_value);
                nk_progress(ctx, &prog_value, 100, NK_MODIFIABLE);

                nk_layout_row(ctx, NK_STATIC, 40, 2, ratio);
                nk_labelf(ctx, NK_TEXT_LEFT, "Knob int: %d", int_knob);
                nk_knob_int(ctx, 0, &int_knob, 10, 1, NK_DOWN, 60.0f);
                nk_labelf(ctx, NK_TEXT_LEFT, "Knob float: %.2f", float_knob);
                nk_knob_float(ctx, 0, &float_knob, 5.0, 0.5f, NK_DOWN, 60.0f);

                nk_layout_row(ctx, NK_STATIC, 25, 2, ratio);
                nk_label(ctx, "Property float:", NK_TEXT_LEFT);
                nk_property_float(ctx, "Float:", 0, &property_float, 64.0f, 0.1f, 0.2f);
                nk_label(ctx, "Property int:", NK_TEXT_LEFT);
                nk_property_int(ctx, "Int:", 0, &property_int, 100, 1, 1);
                nk_label(ctx, "Property neg:", NK_TEXT_LEFT);
                nk_property_int(ctx, "Neg:", -10, &property_neg, 10, 1, 1);

                nk_layout_row_dynamic(ctx, 25, 1);
                nk_label(ctx, "Range:", NK_TEXT_LEFT);
                nk_layout_row_dynamic(ctx, 25, 3);
                nk_property_float(ctx, "#min:", 0, &range_float_min, range_float_max, 1.0f, 0.2f);
                nk_property_float(ctx, "#float:", range_float_min, &range_float_value, range_float_max, 1.0f, 0.2f);
                nk_property_float(ctx, "#max:", range_float_min, &range_float_max, 100, 1.0f, 0.2f);

                nk_property_int(ctx, "#min:", INT_MIN, &range_int_min, range_int_max, 1, 10);
                nk_property_int(ctx, "#neg:", range_int_min, &range_int_value, range_int_max, 1, 10);
                nk_property_int(ctx, "#max:", range_int_min, &range_int_max, INT_MAX, 1, 10);

                nk_layout_row_dynamic(ctx, 0, 2);
                nk_label(ctx, "Hidden Label:", NK_TEXT_LEFT);
                nk_property_int(ctx, "##Hidden Label", range_int_min, &range_int_value_hidden, INT_MAX, 1, 10);

                nk_tree_pop(ctx);
            }

            if (nk_tree_push(ctx, NK_TREE_NODE, "Inactive", NK_MINIMIZED)) {
                static nk_bool inactive = 1;
                nk_layout_row_dynamic(ctx, 30, 1);
                nk_checkbox_label(ctx, "Inactive", &inactive);

                nk_layout_row_static(ctx, 30, 80, 1);
                if (inactive) {
                    nk_widget_disable_begin(ctx);
                }

                if (nk_button_label(ctx, "button"))
                    fprintf(stdout, "button pressed\n");

                nk_widget_disable_end(ctx);

                nk_tree_pop(ctx);
            }

            if (nk_tree_push(ctx, NK_TREE_NODE, "Selectable", NK_MINIMIZED)) {
                if (nk_tree_push(ctx, NK_TREE_NODE, "List", NK_MINIMIZED))
                {
                    static nk_bool selected[4] = {nk_false, nk_false, nk_true, nk_false};
                    nk_layout_row_static(ctx, 18, 100, 1);
                    nk_selectable_label(ctx, "Selectable", NK_TEXT_LEFT, &selected[0]);
                    nk_selectable_label(ctx, "Selectable", NK_TEXT_LEFT, &selected[1]);
                    nk_label(ctx, "Not Selectable", NK_TEXT_LEFT);
                    nk_selectable_label(ctx, "Selectable", NK_TEXT_LEFT, &selected[2]);
                    nk_selectable_label(ctx, "Selectable", NK_TEXT_LEFT, &selected[3]);
                    nk_tree_pop(ctx);
                }
                if (nk_tree_push(ctx, NK_TREE_NODE, "Grid", NK_MINIMIZED))
                {
                    int i;
                    static nk_bool selected[16] = {1,0,0,0, 0,1,0,0, 0,0,1,0, 0,0,0,1};
                    nk_layout_row_static(ctx, 50, 50, 4);
                    for (i = 0; i < 16; ++i) {
                        if (nk_selectable_label(ctx, "Z", NK_TEXT_CENTERED, &selected[i])) {
                            int x = (i % 4), y = i / 4;
                            if (x > 0) selected[i - 1] ^= 1;
                            if (x < 3) selected[i + 1] ^= 1;
                            if (y > 0) selected[i - 4] ^= 1;
                            if (y < 3) selected[i + 4] ^= 1;
                        }
                    }
                    nk_tree_pop(ctx);
                }
                nk_tree_pop(ctx);
            }

            if (nk_tree_push(ctx, NK_TREE_NODE, "Combo", NK_MINIMIZED)) {
                /* Combobox Widgets
                 * In this library comboboxes are not limited to being a popup
                 * list of selectable text. Instead it is a abstract concept of
                 * having something that is *selected* or displayed, a popup window
                 * which opens if something needs to be modified and the content
                 * of the popup which causes the *selected* or displayed value to
                 * change or if wanted close the combobox.
                 *
                 * While strange at first handling comboboxes in a abstract way
                 * solves the problem of overloaded window content. For example
                 * changing a color value requires 4 value modifier (slider, property,...)
                 * for RGBA then you need a label and ways to display the current color.
                 * If you want to go fancy you even add rgb and hsv ratio boxes.
                 * While fine for one color if you have a lot of them it because
                 * tedious to look at and quite wasteful in space. You could add
                 * a popup which modifies the color but this does not solve the
                 * fact that it still requires a lot of cluttered space to do.
                 *
                 * In these kind of instance abstract comboboxes are quite handy. All
                 * value modifiers are hidden inside the combobox popup and only
                 * the color is shown if not open. This combines the clarity of the
                 * popup with the ease of use of just using the space for modifiers.
                 *
                 * Other instances are for example time and especially date picker,
                 * which only show the currently activated time/data and hide the
                 * selection logic inside the combobox popup.
                 */
                static float chart_selection = 8.0f;
                static int current_weapon = 0;
                static nk_bool check_values[5];
                static float position[3];
                static struct nk_color combo_color = {130, 50, 50, 255};
                static struct nk_colorf combo_color2 = {0.509f, 0.705f, 0.2f, 1.0f};
                static size_t prog_a =  20, prog_b = 40, prog_c = 10, prog_d = 90;
                static const char *weapons[] = {"Fist","Pistol","Shotgun","Plasma","BFG"};

                char buffer[64];
                size_t sum = 0;

                /* default combobox */
                nk_layout_row_static(ctx, 25, 200, 1);
                current_weapon = nk_combo(ctx, weapons, NK_LEN(weapons), current_weapon, 25, nk_vec2(200,200));

                /* slider color combobox */
                if (nk_combo_begin_color(ctx, combo_color, nk_vec2(200,200))) {
                    float ratios[] = {0.15f, 0.85f};
                    nk_layout_row(ctx, NK_DYNAMIC, 30, 2, ratios);
                    nk_label(ctx, "R:", NK_TEXT_LEFT);
                    combo_color.r = (nk_byte)nk_slide_int(ctx, 0, combo_color.r, 255, 5);
                    nk_label(ctx, "G:", NK_TEXT_LEFT);
                    combo_color.g = (nk_byte)nk_slide_int(ctx, 0, combo_color.g, 255, 5);
                    nk_label(ctx, "B:", NK_TEXT_LEFT);
                    combo_color.b = (nk_byte)nk_slide_int(ctx, 0, combo_color.b, 255, 5);
                    nk_label(ctx, "A:", NK_TEXT_LEFT);
                    combo_color.a = (nk_byte)nk_slide_int(ctx, 0, combo_color.a , 255, 5);
                    nk_combo_end(ctx);
                }
                /* complex color combobox */
                if (nk_combo_begin_color(ctx, nk_rgb_cf(combo_color2), nk_vec2(200,400))) {
                    enum color_mode {COL_RGB, COL_HSV};
                    static int col_mode = COL_RGB;
                    #ifndef DEMO_DO_NOT_USE_COLOR_PICKER
                    nk_layout_row_dynamic(ctx, 120, 1);
                    combo_color2 = nk_color_picker(ctx, combo_color2, NK_RGBA);
                    #endif

                    nk_layout_row_dynamic(ctx, 25, 2);
                    col_mode = nk_option_label(ctx, "RGB", col_mode == COL_RGB) ? COL_RGB : col_mode;
                    col_mode = nk_option_label(ctx, "HSV", col_mode == COL_HSV) ? COL_HSV : col_mode;

                    nk_layout_row_dynamic(ctx, 25, 1);
                    if (col_mode == COL_RGB) {
                        combo_color2.r = nk_propertyf(ctx, "#R:", 0, combo_color2.r, 1.0f, 0.01f,0.005f);
                        combo_color2.g = nk_propertyf(ctx, "#G:", 0, combo_color2.g, 1.0f, 0.01f,0.005f);
                        combo_color2.b = nk_propertyf(ctx, "#B:", 0, combo_color2.b, 1.0f, 0.01f,0.005f);
                        combo_color2.a = nk_propertyf(ctx, "#A:", 0, combo_color2.a, 1.0f, 0.01f,0.005f);
                    } else {
                        float hsva[4];
                        nk_colorf_hsva_fv(hsva, combo_color2);
                        hsva[0] = nk_propertyf(ctx, "#H:", 0, hsva[0], 1.0f, 0.01f,0.05f);
                        hsva[1] = nk_propertyf(ctx, "#S:", 0, hsva[1], 1.0f, 0.01f,0.05f);
                        hsva[2] = nk_propertyf(ctx, "#V:", 0, hsva[2], 1.0f, 0.01f,0.05f);
                        hsva[3] = nk_propertyf(ctx, "#A:", 0, hsva[3], 1.0f, 0.01f,0.05f);
                        combo_color2 = nk_hsva_colorfv(hsva);
                    }
                    nk_combo_end(ctx);
                }
                /* progressbar combobox */
                sum = prog_a + prog_b + prog_c + prog_d;
                sprintf(buffer, "%lu", sum);
                if (nk_combo_begin_label(ctx, buffer, nk_vec2(200,200))) {
                    nk_layout_row_dynamic(ctx, 30, 1);
                    nk_progress(ctx, &prog_a, 100, NK_MODIFIABLE);
                    nk_progress(ctx, &prog_b, 100, NK_MODIFIABLE);
                    nk_progress(ctx, &prog_c, 100, NK_MODIFIABLE);
                    nk_progress(ctx, &prog_d, 100, NK_MODIFIABLE);
                    nk_combo_end(ctx);
                }

                /* checkbox combobox */
                sum = (size_t)(check_values[0] + check_values[1] + check_values[2] + check_values[3] + check_values[4]);
                sprintf(buffer, "%lu", sum);
                if (nk_combo_begin_label(ctx, buffer, nk_vec2(200,200))) {
                    nk_layout_row_dynamic(ctx, 30, 1);
                    nk_checkbox_label(ctx, weapons[0], &check_values[0]);
                    nk_checkbox_label(ctx, weapons[1], &check_values[1]);
                    nk_checkbox_label(ctx, weapons[2], &check_values[2]);
                    nk_checkbox_label(ctx, weapons[3], &check_values[3]);
                    nk_combo_end(ctx);
                }

                /* complex text combobox */
                sprintf(buffer, "%.2f, %.2f, %.2f", position[0], position[1],position[2]);
                if (nk_combo_begin_label(ctx, buffer, nk_vec2(200,200))) {
                    nk_layout_row_dynamic(ctx, 25, 1);
                    nk_property_float(ctx, "#X:", -1024.0f, &position[0], 1024.0f, 1,0.5f);
                    nk_property_float(ctx, "#Y:", -1024.0f, &position[1], 1024.0f, 1,0.5f);
                    nk_property_float(ctx, "#Z:", -1024.0f, &position[2], 1024.0f, 1,0.5f);
                    nk_combo_end(ctx);
                }

                /* chart combobox */
                sprintf(buffer, "%.1f", chart_selection);
                if (nk_combo_begin_label(ctx, buffer, nk_vec2(200,250))) {
                    size_t i = 0;
                    static const float values[]={26.0f,13.0f,30.0f,15.0f,25.0f,10.0f,20.0f,40.0f, 12.0f, 8.0f, 22.0f, 28.0f, 5.0f};
                    nk_layout_row_dynamic(ctx, 150, 1);
                    nk_chart_begin(ctx, NK_CHART_COLUMN, NK_LEN(values), 0, 50);
                    for (i = 0; i < NK_LEN(values); ++i) {
                        nk_flags res = nk_chart_push(ctx, values[i]);
                        if (res & NK_CHART_CLICKED) {
                            chart_selection = values[i];
                            nk_combo_close(ctx);
                        }
                    }
                    nk_chart_end(ctx);
                    nk_combo_end(ctx);
                }

                {
                    static int time_selected = 0;
                    static int date_selected = 0;
                    static struct tm sel_time;
                    static struct tm sel_date;
                    if (!time_selected || !date_selected) {
                        /* keep time and date updated if nothing is selected */
                        time_t cur_time = time(0);
                        struct tm *n = localtime(&cur_time);
                        if (!time_selected)
                            memcpy(&sel_time, n, sizeof(struct tm));
                        if (!date_selected)
                            memcpy(&sel_date, n, sizeof(struct tm));
                    }

                    /* time combobox */
                    sprintf(buffer, "%02d:%02d:%02d", sel_time.tm_hour, sel_time.tm_min, sel_time.tm_sec);
                    if (nk_combo_begin_label(ctx, buffer, nk_vec2(200,250))) {
                        time_selected = 1;
                        nk_layout_row_dynamic(ctx, 25, 1);
                        sel_time.tm_sec = nk_propertyi(ctx, "#S:", 0, sel_time.tm_sec, 60, 1, 1);
                        sel_time.tm_min = nk_propertyi(ctx, "#M:", 0, sel_time.tm_min, 60, 1, 1);
                        sel_time.tm_hour = nk_propertyi(ctx, "#H:", 0, sel_time.tm_hour, 23, 1, 1);
                        nk_combo_end(ctx);
                    }

                    /* date combobox */
                    sprintf(buffer, "%02d-%02d-%02d", sel_date.tm_mday, sel_date.tm_mon+1, sel_date.tm_year+1900);
                    if (nk_combo_begin_label(ctx, buffer, nk_vec2(350,400)))
                    {
                        int i = 0;
                        const char *month[] = {"January", "February", "March",
                            "April", "May", "June", "July", "August", "September",
                            "October", "November", "December"};
                        const char *week_days[] = {"SUN", "MON", "TUE", "WED", "THU", "FRI", "SAT"};
                        const int month_days[] = {31,28,31,30,31,30,31,31,30,31,30,31};
                        int year = sel_date.tm_year+1900;
                        int leap_year = (!(year % 4) && ((year % 100))) || !(year % 400);
                        int days = (sel_date.tm_mon == 1) ?
                            month_days[sel_date.tm_mon] + leap_year:
                            month_days[sel_date.tm_mon];

                        /* header with month and year */
                        date_selected = 1;
                        nk_layout_row_begin(ctx, NK_DYNAMIC, 20, 3);
                        nk_layout_row_push(ctx, 0.05f);
                        if (nk_button_symbol(ctx, NK_SYMBOL_TRIANGLE_LEFT)) {
                            if (sel_date.tm_mon == 0) {
                                sel_date.tm_mon = 11;
                                sel_date.tm_year = NK_MAX(0, sel_date.tm_year-1);
                            } else sel_date.tm_mon--;
                        }
                        nk_layout_row_push(ctx, 0.9f);
                        sprintf(buffer, "%s %d", month[sel_date.tm_mon], year);
                        nk_label(ctx, buffer, NK_TEXT_CENTERED);
                        nk_layout_row_push(ctx, 0.05f);
                        if (nk_button_symbol(ctx, NK_SYMBOL_TRIANGLE_RIGHT)) {
                            if (sel_date.tm_mon == 11) {
                                sel_date.tm_mon = 0;
                                sel_date.tm_year++;
                            } else sel_date.tm_mon++;
                        }
                        nk_layout_row_end(ctx);

                        /* good old week day formula (double because precision) */
                        {int year_n = (sel_date.tm_mon < 2) ? year-1: year;
                        int y = year_n % 100;
                        int c = year_n / 100;
                        int y4 = (int)((float)y / 4);
                        int c4 = (int)((float)c / 4);
                        int m = (int)(2.6 * (double)(((sel_date.tm_mon + 10) % 12) + 1) - 0.2);
                        int week_day = (((1 + m + y + y4 + c4 - 2 * c) % 7) + 7) % 7;

                        /* weekdays  */
                        nk_layout_row_dynamic(ctx, 35, 7);
                        for (i = 0; i < (int)NK_LEN(week_days); ++i)
                            nk_label(ctx, week_days[i], NK_TEXT_CENTERED);

                        /* days  */
                        if (week_day > 0) nk_spacing(ctx, week_day);
                        for (i = 1; i <= days; ++i) {
                            sprintf(buffer, "%d", i);
                            if (nk_button_label(ctx, buffer)) {
                                sel_date.tm_mday = i;
                                nk_combo_close(ctx);
                            }
                        }}
                        nk_combo_end(ctx);
                    }
                }

                nk_tree_pop(ctx);
            }

            if (nk_tree_push(ctx, NK_TREE_NODE, "Input", NK_MINIMIZED)) {
                static const float ratio[] = {120, 150};
                static char field_buffer[64];
                static char field_w_overwrite_buf[64];
                static char text[9][64];
                static int text_len[9];
                static char box_buffer[512];
                static int field_len;
                static int field_ow_len;
                static int box_len;
                nk_flags active;

                nk_layout_row(ctx, NK_STATIC, 25, 2, ratio);
                nk_label(ctx, "Default:", NK_TEXT_LEFT);

                nk_edit_string(ctx, NK_EDIT_SIMPLE, text[0], &text_len[0], 64, nk_filter_default);
                nk_label(ctx, "Int:", NK_TEXT_LEFT);
                nk_edit_string(ctx, NK_EDIT_SIMPLE, text[1], &text_len[1], 64, nk_filter_decimal);
                nk_label(ctx, "Float:", NK_TEXT_LEFT);
                nk_edit_string(ctx, NK_EDIT_SIMPLE, text[2], &text_len[2], 64, nk_filter_float);
                nk_label(ctx, "Hex:", NK_TEXT_LEFT);
                nk_edit_string(ctx, NK_EDIT_SIMPLE, text[4], &text_len[4], 64, nk_filter_hex);
                nk_label(ctx, "Octal:", NK_TEXT_LEFT);
                nk_edit_string(ctx, NK_EDIT_SIMPLE, text[5], &text_len[5], 64, nk_filter_oct);
                nk_label(ctx, "Binary:", NK_TEXT_LEFT);
                nk_edit_string(ctx, NK_EDIT_SIMPLE, text[6], &text_len[6], 64, nk_filter_binary);

                nk_label(ctx, "Password:", NK_TEXT_LEFT);
                {
                    int i = 0;
                    int old_len = text_len[8];
                    char buffer[64];
                    for (i = 0; i < text_len[8]; ++i) buffer[i] = '*';
                    nk_edit_string(ctx, NK_EDIT_FIELD, buffer, &text_len[8], 64, nk_filter_default);
                    if (old_len < text_len[8])
                        memcpy(&text[8][old_len], &buffer[old_len], (nk_size)(text_len[8] - old_len));
                }

                nk_label(ctx, "Field:", NK_TEXT_LEFT);
                nk_edit_string(ctx, NK_EDIT_FIELD, field_buffer, &field_len, 64, nk_filter_default);

                nk_label(ctx, "Field 2:", NK_TEXT_LEFT);
                nk_edit_string(ctx, NK_EDIT_SELECTABLE|NK_EDIT_CLIPBOARD, field_w_overwrite_buf, &field_ow_len, 64, nk_filter_default);

                nk_label(ctx, "Box:", NK_TEXT_LEFT);
                nk_layout_row_static(ctx, 180, 278, 1);
                nk_edit_string(ctx, NK_EDIT_BOX, box_buffer, &box_len, 512, nk_filter_default);

                nk_layout_row(ctx, NK_STATIC, 25, 2, ratio);
                active = nk_edit_string(ctx, NK_EDIT_FIELD|NK_EDIT_SIG_ENTER, text[7], &text_len[7], 64,  nk_filter_ascii);
                if (nk_button_label(ctx, "Submit") ||
                    (active & NK_EDIT_COMMITTED))
                {
                    text[7][text_len[7]] = '\n';
                    text_len[7]++;
                    memcpy(&box_buffer[box_len], &text[7], (nk_size)text_len[7]);
                    box_len += text_len[7];
                    text_len[7] = 0;
                }
                nk_tree_pop(ctx);
            }

            if (nk_tree_push(ctx, NK_TREE_NODE, "Horizontal Rule", NK_MINIMIZED)) {
                nk_layout_row_dynamic(ctx, 12, 1);
                nk_label(ctx, "Use this to subdivide spaces visually", NK_TEXT_LEFT);
                nk_layout_row_dynamic(ctx, 4, 1);
                nk_rule_horizontal(ctx, nk_white, nk_true);
                nk_layout_row_dynamic(ctx, 75, 1);
                nk_label_wrap(ctx, "Best used in 'Card'-like layouts, with a bigger title font on top. Takes on the size of the previous layout definition. Rounding optional.");
                nk_tree_pop(ctx);
            }

            nk_tree_pop(ctx);
        }

        if (nk_tree_push(ctx, NK_TREE_TAB, "Chart", NK_MINIMIZED)) {
            /* Chart Widgets
             * This library has two different rather simple charts. The line and the
             * column chart. Both provide a simple way of visualizing values and
             * have a retained mode and immediate mode API version. For the retain
             * mode version `nk_plot` and `nk_plot_function` you either provide
             * an array or a callback to call to handle drawing the graph.
             * For the immediate mode version you start by calling `nk_chart_begin`
             * and need to provide min and max values for scaling on the Y-axis.
             * and then call `nk_chart_push` to push values into the chart.
             * Finally `nk_chart_end` needs to be called to end the process. */
            float id = 0;
            static int col_index = -1;
            static int line_index = -1;
            static nk_bool show_markers = nk_true;
            float step = (2*3.141592654f) / 32;

            int i;
            int index = -1;

            /* line chart */
            id = 0;
            index = -1;
            nk_layout_row_dynamic(ctx, 15, 1);
            nk_checkbox_label(ctx, "Show markers", &show_markers);
            nk_layout_row_dynamic(ctx, 100, 1);
            ctx->style.chart.show_markers = show_markers;
            if (nk_chart_begin(ctx, NK_CHART_LINES, 32, -1.0f, 1.0f)) {
                for (i = 0; i < 32; ++i) {
                    nk_flags res = nk_chart_push(ctx, (float)cos(id));
                    if (res & NK_CHART_HOVERING)
                        index = (int)i;
                    if (res & NK_CHART_CLICKED)
                        line_index = (int)i;
                    id += step;
                }
                nk_chart_end(ctx);
            }

            if (index != -1)
                nk_tooltipf(ctx, "Value: %.2f", (float)cos((float)index*step));
            if (line_index != -1) {
                nk_layout_row_dynamic(ctx, 20, 1);
                nk_labelf(ctx, NK_TEXT_LEFT, "Selected value: %.2f", (float)cos((float)index*step));
            }

            /* column chart */
            nk_layout_row_dynamic(ctx, 100, 1);
            if (nk_chart_begin(ctx, NK_CHART_COLUMN, 32, 0.0f, 1.0f)) {
                for (i = 0; i < 32; ++i) {
                    nk_flags res = nk_chart_push(ctx, (float)fabs(sin(id)));
                    if (res & NK_CHART_HOVERING)
                        index = (int)i;
                    if (res & NK_CHART_CLICKED)
                        col_index = (int)i;
                    id += step;
                }
                nk_chart_end(ctx);
            }
            if (index != -1)
                nk_tooltipf(ctx, "Value: %.2f", (float)fabs(sin(step * (float)index)));
            if (col_index != -1) {
                nk_layout_row_dynamic(ctx, 20, 1);
                nk_labelf(ctx, NK_TEXT_LEFT, "Selected value: %.2f", (float)fabs(sin(step * (float)col_index)));
            }

            /* mixed chart */
            nk_layout_row_dynamic(ctx, 100, 1);
            if (nk_chart_begin(ctx, NK_CHART_COLUMN, 32, 0.0f, 1.0f)) {
                nk_chart_add_slot(ctx, NK_CHART_LINES, 32, -1.0f, 1.0f);
                nk_chart_add_slot(ctx, NK_CHART_LINES, 32, -1.0f, 1.0f);
                for (id = 0, i = 0; i < 32; ++i) {
                    nk_chart_push_slot(ctx, (float)fabs(sin(id)), 0);
                    nk_chart_push_slot(ctx, (float)cos(id), 1);
                    nk_chart_push_slot(ctx, (float)sin(id), 2);
                    id += step;
                }
            }
            nk_chart_end(ctx);

            /* mixed colored chart */
            nk_layout_row_dynamic(ctx, 100, 1);
            if (nk_chart_begin_colored(ctx, NK_CHART_LINES, nk_rgb(255,0,0), nk_rgb(150,0,0), 32, 0.0f, 1.0f)) {
                nk_chart_add_slot_colored(ctx, NK_CHART_LINES, nk_rgb(0,0,255), nk_rgb(0,0,150),32, -1.0f, 1.0f);
                nk_chart_add_slot_colored(ctx, NK_CHART_LINES, nk_rgb(0,255,0), nk_rgb(0,150,0), 32, -1.0f, 1.0f);
                for (id = 0, i = 0; i < 32; ++i) {
                    nk_chart_push_slot(ctx, (float)fabs(sin(id)), 0);
                    nk_chart_push_slot(ctx, (float)cos(id), 1);
                    nk_chart_push_slot(ctx, (float)sin(id), 2);
                    id += step;
                }
            }
            nk_chart_end(ctx);
            nk_tree_pop(ctx);
        }

        if (nk_tree_push(ctx, NK_TREE_TAB, "Popup", NK_MINIMIZED)) {
            static struct nk_color color = {255,0,0, 255};
            static nk_bool select[4];
            static nk_bool popup_active;
            const struct nk_input *in = &ctx->input;
            struct nk_rect bounds;

            /* seconds */
            static float delay_timer = 0.0;
            static nk_bool clicked = nk_false;

            /* menu contextual */
            nk_layout_row_static(ctx, 30, 160, 1);
            bounds = nk_widget_bounds(ctx);
            nk_label(ctx, "Right click me for menu", NK_TEXT_LEFT);

            if (nk_contextual_begin(ctx, 0, nk_vec2(100, 300), bounds)) {
                static size_t prog = 40;
                static int slider = 10;

                nk_layout_row_dynamic(ctx, 25, 1);
//                nk_checkbox_label(ctx, "Menu", &show_menu);
                nk_progress(ctx, &prog, 100, NK_MODIFIABLE);
                nk_slider_int(ctx, 0, &slider, 16, 1);
                if (nk_contextual_item_label(ctx, "About", NK_TEXT_CENTERED))
                    show_app_about = nk_true;
                nk_selectable_label(ctx, select[0]?"Unselect":"Select", NK_TEXT_LEFT, &select[0]);
                nk_selectable_label(ctx, select[1]?"Unselect":"Select", NK_TEXT_LEFT, &select[1]);
                nk_selectable_label(ctx, select[2]?"Unselect":"Select", NK_TEXT_LEFT, &select[2]);
                nk_selectable_label(ctx, select[3]?"Unselect":"Select", NK_TEXT_LEFT, &select[3]);
                nk_contextual_end(ctx);
            }

            /* color contextual */
            nk_layout_row_begin(ctx, NK_STATIC, 30, 2);
            nk_layout_row_push(ctx, 120);
            nk_label(ctx, "Right Click here:", NK_TEXT_LEFT);
            nk_layout_row_push(ctx, 50);
            bounds = nk_widget_bounds(ctx);
            nk_button_color(ctx, color);
            nk_layout_row_end(ctx);

            if (nk_contextual_begin(ctx, 0, nk_vec2(350, 60), bounds)) {
                nk_layout_row_dynamic(ctx, 30, 4);
                color.r = (nk_byte)nk_propertyi(ctx, "#r", 0, color.r, 255, 1, 1);
                color.g = (nk_byte)nk_propertyi(ctx, "#g", 0, color.g, 255, 1, 1);
                color.b = (nk_byte)nk_propertyi(ctx, "#b", 0, color.b, 255, 1, 1);
                color.a = (nk_byte)nk_propertyi(ctx, "#a", 0, color.a, 255, 1, 1);
                nk_contextual_end(ctx);
            }

            /* popup */
            nk_layout_row_begin(ctx, NK_STATIC, 30, 2);
            nk_layout_row_push(ctx, 120);
            nk_label(ctx, "Popup:", NK_TEXT_LEFT);
            nk_layout_row_push(ctx, 50);
            if (nk_button_label(ctx, "Popup"))
                popup_active = 1;
            nk_layout_row_end(ctx);

            if (popup_active) {
                static struct nk_rect s = {20, 100, 220, 90};
                if (nk_popup_begin(ctx, NK_POPUP_STATIC, "Error", 0, s))
                {
                    nk_layout_row_dynamic(ctx, 25, 1);
                    nk_label(ctx, "A terrible error as occurred", NK_TEXT_LEFT);
                    nk_layout_row_dynamic(ctx, 25, 2);
                    if (nk_button_label(ctx, "OK")) {
                        popup_active = 0;
                        nk_popup_close(ctx);
                    }
                    if (nk_button_label(ctx, "Cancel")) {
                        popup_active = 0;
                        nk_popup_close(ctx);
                    }
                    nk_popup_end(ctx);
                } else popup_active = nk_false;
            }

            /* tooltips */
            nk_layout_row_static(ctx, 30, 400, 1);
            bounds = nk_widget_bounds(ctx);
            nk_label(ctx, "Hover for default tooltip", NK_TEXT_LEFT);
            if (nk_input_is_mouse_hovering_rect(in, bounds)) {
                nk_tooltip(ctx, "This is a default tooltip");
            }

            bounds = nk_widget_bounds(ctx);
            nk_label(ctx, "Hover motionless for a default delayed tooltip", NK_TEXT_LEFT);
            nk_do_tooltip_delay(ctx, "This is a delayed tooltip", bounds, &delay_timer);

            bounds = nk_widget_bounds(ctx);
            nk_label(ctx, "Hover motionless longer a custom delayed tooltip", NK_TEXT_LEFT);
            if (nk_input_is_mouse_hovering_still_delay_rect(ctx, bounds, &delay_timer, 1.5)) {
                nk_tooltip(ctx, "This is a custom delayed tooltip");
            }

            bounds = nk_widget_bounds(ctx);
            if (nk_button_label(ctx, "Delayed tooltip with click sensitivity")) {
                clicked = nk_true;
            }
            nk_do_tooltip_delay_clicked(ctx, "disappears when clicked, timer starts when you move again", bounds, &delay_timer, &clicked);

            bounds = nk_widget_bounds(ctx);
            nk_label(ctx, "Hover for Gnome-like tooltip", NK_TEXT_LEFT);
            if (nk_input_is_mouse_hovering_rect(in, bounds)) {
                struct nk_vec2 offset = { 0, -15 };
                nk_tooltip_offset(ctx, "Gnome bottom centers plus a -y offset", NK_BOTTOM_CENTER, offset);
            }
            bounds = nk_widget_bounds(ctx);
            nk_label(ctx, "Hover for a bottom left tooltip", NK_TEXT_LEFT);
            if (nk_input_is_mouse_hovering_rect(in, bounds)) {
                struct nk_vec2 offset = { 0, 0 };
                nk_tooltip_offset(ctx, "Bottom left positioning", NK_BOTTOM_LEFT, offset);
            }
            bounds = nk_widget_bounds(ctx);
            nk_label(ctx, "Hover for MAGIC!", NK_TEXT_LEFT);
            if (nk_input_is_mouse_hovering_rect(in, bounds)) {
                static double accum_time_seconds = 0.0;
                const double speed = 3.0, radius = 50.0;
                struct nk_vec2 offset;
                offset.x = radius * cos(accum_time_seconds * speed);
                offset.y = radius * sin(accum_time_seconds * speed);
                nk_tooltip_offset(ctx, "WOW!", NK_MIDDLE_CENTER, offset);
                accum_time_seconds += (double)(ctx->delta_time_seconds);
            }

            /* editor for custom tooltip */
            {
                static char text_buf[64] = {0};
                static int text_len = 0;
                static int text_initialized = 0;
                static struct nk_vec2 offset = {0};
                static const char* tooltip_positions[] =
                {
                    "TOP_LEFT",
                    "TOP_CENTER",
                    "TOP_RIGHT",

                    "MIDDLE_LEFT",
                    "MIDDLE_CENTER",
                    "MIDDLE_RIGHT",

                    "BOTTOM_LEFT",
                    "BOTTOM_CENTER",
                    "BOTTOM_RIGHT"
                };
                int cur_pos = NK_TOP_LEFT;

                if (!text_initialized) {
                    const char text_default[] = "you can customize this!";
                    NK_ASSERT(sizeof(text_default) < sizeof(text_buf));
                    memcpy(text_buf, text_default, sizeof(text_default));
                    text_len = sizeof(text_default) - 1;
                    text_initialized = 1;
                }
                bounds = nk_widget_bounds(ctx);
                nk_label(ctx, "Hover for custom tooltip (you can customize it below)", NK_TEXT_LEFT);
                if (nk_input_is_mouse_hovering_rect(in, bounds)) {
                    nk_tooltip_offset(ctx, text_buf, (enum nk_tooltip_pos)cur_pos, offset);
                }
                nk_layout_row_dynamic(ctx, 1, 1);
                nk_rule_horizontal(ctx, nk_white, nk_true);
                nk_layout_row_dynamic(ctx, 30, 2);
                nk_label(ctx, "custom tooltip text:", NK_TEXT_LEFT);
                nk_edit_string(ctx, NK_EDIT_FIELD, text_buf, &text_len, sizeof(text_buf), nk_filter_default);
                text_buf[text_len] = '\0';  /* TODO: why nk_edit_string is NOT setting this on its own? */
                nk_layout_row_dynamic(ctx, 30, 1);
                cur_pos = nk_combo(ctx, tooltip_positions, NK_LEN(tooltip_positions), cur_pos, 25, nk_vec2(200, 200));

                nk_layout_row_dynamic(ctx, 30, 2);
                nk_label(ctx, "custom tooltip offset", NK_TEXT_LEFT);
                nk_property_float(ctx, "x", -100.0f, &offset.x, 100.0f, 5.0f, 0.5f);
                nk_label(ctx, "custom tooltip offset", NK_TEXT_LEFT);
                nk_property_float(ctx, "y", -100.0f, &offset.y, 100.0f, 5.0f, 0.5f);
            }

            nk_tree_pop(ctx);
        }

        if (nk_tree_push(ctx, NK_TREE_TAB, "Layout", NK_MINIMIZED)) {
            if (nk_tree_push(ctx, NK_TREE_NODE, "Widget", NK_MINIMIZED)) {
                float ratio_two[] = {0.2f, 0.6f, 0.2f};
                float width_two[] = {100, 200, 50};

                nk_layout_row_dynamic(ctx, 30, 1);
                nk_label(ctx, "Dynamic fixed column layout with generated position and size:", NK_TEXT_LEFT);
                nk_layout_row_dynamic(ctx, 30, 3);
                nk_button_label(ctx, "button");
                nk_button_label(ctx, "button");
                nk_button_label(ctx, "button");

                nk_layout_row_dynamic(ctx, 30, 1);
                nk_label(ctx, "static fixed column layout with generated position and size:", NK_TEXT_LEFT);
                nk_layout_row_static(ctx, 30, 100, 3);
                nk_button_label(ctx, "button");
                nk_button_label(ctx, "button");
                nk_button_label(ctx, "button");

                nk_layout_row_dynamic(ctx, 30, 1);
                nk_label(ctx, "Dynamic array-based custom column layout with generated position and custom size:",NK_TEXT_LEFT);
                nk_layout_row(ctx, NK_DYNAMIC, 30, 3, ratio_two);
                nk_button_label(ctx, "button");
                nk_button_label(ctx, "button");
                nk_button_label(ctx, "button");

                nk_layout_row_dynamic(ctx, 30, 1);
                nk_label(ctx, "Static array-based custom column layout with generated position and custom size:",NK_TEXT_LEFT );
                nk_layout_row(ctx, NK_STATIC, 30, 3, width_two);
                nk_button_label(ctx, "button");
                nk_button_label(ctx, "button");
                nk_button_label(ctx, "button");

                nk_layout_row_dynamic(ctx, 30, 1);
                nk_label(ctx, "Dynamic immediate mode custom column layout with generated position and custom size:",NK_TEXT_LEFT);
                nk_layout_row_begin(ctx, NK_DYNAMIC, 30, 3);
                nk_layout_row_push(ctx, 0.2f);
                nk_button_label(ctx, "button");
                nk_layout_row_push(ctx, 0.6f);
                nk_button_label(ctx, "button");
                nk_layout_row_push(ctx, 0.2f);
                nk_button_label(ctx, "button");
                nk_layout_row_end(ctx);

                nk_layout_row_dynamic(ctx, 30, 1);
                nk_label(ctx, "Static immediate mode custom column layout with generated position and custom size:", NK_TEXT_LEFT);
                nk_layout_row_begin(ctx, NK_STATIC, 30, 3);
                nk_layout_row_push(ctx, 100);
                nk_button_label(ctx, "button");
                nk_layout_row_push(ctx, 200);
                nk_button_label(ctx, "button");
                nk_layout_row_push(ctx, 50);
                nk_button_label(ctx, "button");
                nk_layout_row_end(ctx);

                nk_layout_row_dynamic(ctx, 30, 1);
                nk_label(ctx, "Static free space with custom position and custom size:", NK_TEXT_LEFT);
                nk_layout_space_begin(ctx, NK_STATIC, 60, 4);
                nk_layout_space_push(ctx, nk_rect(100, 0, 100, 30));
                nk_button_label(ctx, "button");
                nk_layout_space_push(ctx, nk_rect(0, 15, 100, 30));
                nk_button_label(ctx, "button");
                nk_layout_space_push(ctx, nk_rect(200, 15, 100, 30));
                nk_button_label(ctx, "button");
                nk_layout_space_push(ctx, nk_rect(100, 30, 100, 30));
                nk_button_label(ctx, "button");
                nk_layout_space_end(ctx);

                nk_layout_row_dynamic(ctx, 30, 1);
                nk_label(ctx, "Row template:", NK_TEXT_LEFT);
                nk_layout_row_template_begin(ctx, 30);
                nk_layout_row_template_push_dynamic(ctx);
                nk_layout_row_template_push_variable(ctx, 80);
                nk_layout_row_template_push_static(ctx, 80);
                nk_layout_row_template_end(ctx);
                nk_button_label(ctx, "button");
                nk_button_label(ctx, "button");
                nk_button_label(ctx, "button");

                nk_tree_pop(ctx);
            }

            if (nk_tree_push(ctx, NK_TREE_NODE, "Group", NK_MINIMIZED)) {
                static nk_bool group_titlebar = nk_false;
                static nk_bool group_border = nk_true;
                static nk_bool group_no_scrollbar = nk_false;
                static int group_width = 320;
                static int group_height = 200;

                nk_flags group_flags = 0;
                if (group_border) group_flags |= NK_WINDOW_BORDER;
                if (group_no_scrollbar) group_flags |= NK_WINDOW_NO_SCROLLBAR;
                if (group_titlebar) group_flags |= NK_WINDOW_TITLE;

                nk_layout_row_dynamic(ctx, 30, 3);
                nk_checkbox_label(ctx, "Titlebar", &group_titlebar);
                nk_checkbox_label(ctx, "Border", &group_border);
                nk_checkbox_label(ctx, "No Scrollbar", &group_no_scrollbar);

                nk_layout_row_begin(ctx, NK_STATIC, 22, 3);
                nk_layout_row_push(ctx, 50);
                nk_label(ctx, "size:", NK_TEXT_LEFT);
                nk_layout_row_push(ctx, 130);
                nk_property_int(ctx, "#Width:", 100, &group_width, 500, 10, 1);
                nk_layout_row_push(ctx, 130);
                nk_property_int(ctx, "#Height:", 100, &group_height, 500, 10, 1);
                nk_layout_row_end(ctx);

                nk_layout_row_static(ctx, (float)group_height, group_width, 2);
                if (nk_group_begin(ctx, "Group", group_flags)) {
                    int i = 0;
                    static nk_bool selected[16];
                    nk_layout_row_static(ctx, 18, 100, 1);
                    for (i = 0; i < 16; ++i)
                        nk_selectable_label(ctx, (selected[i]) ? "Selected": "Unselected", NK_TEXT_CENTERED, &selected[i]);
                    nk_group_end(ctx);
                }
                nk_tree_pop(ctx);
            }
            if (nk_tree_push(ctx, NK_TREE_NODE, "Tree", NK_MINIMIZED)) {
                static nk_bool root_selected = 0;
                nk_bool sel = root_selected;
                if (nk_tree_element_push(ctx, NK_TREE_NODE, "Root", NK_MINIMIZED, &sel)) {
                    static nk_bool selected[8];
                    int i = 0;
                    nk_bool node_select = selected[0];
                    if (sel != root_selected) {
                        root_selected = sel;
                        for (i = 0; i < 8; ++i)
                            selected[i] = sel;
                    }
                    if (nk_tree_element_push(ctx, NK_TREE_NODE, "Node", NK_MINIMIZED, &node_select)) {
                        int j = 0;
                        static nk_bool sel_nodes[4];
                        if (node_select != selected[0]) {
                            selected[0] = node_select;
                            for (i = 0; i < 4; ++i)
                                sel_nodes[i] = node_select;
                        }
                        nk_layout_row_static(ctx, 18, 100, 1);
                        for (j = 0; j < 4; ++j)
                            nk_selectable_symbol_label(ctx, NK_SYMBOL_CIRCLE_SOLID, (sel_nodes[j]) ? "Selected": "Unselected", NK_TEXT_RIGHT, &sel_nodes[j]);
                        nk_tree_element_pop(ctx);
                    }
                    nk_layout_row_static(ctx, 18, 100, 1);
                    for (i = 1; i < 8; ++i)
                        nk_selectable_symbol_label(ctx, NK_SYMBOL_CIRCLE_SOLID, (selected[i]) ? "Selected": "Unselected", NK_TEXT_RIGHT, &selected[i]);
                    nk_tree_element_pop(ctx);
                }
                nk_tree_pop(ctx);
            }
            if (nk_tree_push(ctx, NK_TREE_NODE, "Notebook", NK_MINIMIZED)) {
                static int current_tab = 0;
                float step = (2*3.141592654f) / 32;
                enum chart_type {CHART_LINE, CHART_HISTO, CHART_MIXED};
                const char *names[] = {"Lines", "Columns", "Mixed"};
                float id = 0;
                int i;

                /* Header */
                nk_style_push_vec2(ctx, &ctx->style.window.spacing, nk_vec2(0,0));
                nk_style_push_float(ctx, &ctx->style.button.rounding, 0);
                nk_layout_row_begin(ctx, NK_STATIC, 20, 3);
                for (i = 0; i < 3; ++i) {
                    /* make sure button perfectly fits text */
                    const struct nk_user_font *f = ctx->style.font;
                    float text_width = f->width(f->userdata, f->height, names[i], nk_strlen(names[i]));
                    float widget_width = text_width + 3 * ctx->style.button.padding.x;
                    nk_layout_row_push(ctx, widget_width);
                    if (current_tab == i) {
                        /* active tab gets highlighted */
                        struct nk_style_item button_color = ctx->style.button.normal;
                        ctx->style.button.normal = ctx->style.button.active;
                        current_tab = nk_button_label(ctx, names[i]) ? i: current_tab;
                        ctx->style.button.normal = button_color;
                    } else current_tab = nk_button_label(ctx, names[i]) ? i: current_tab;
                }
                nk_style_pop_float(ctx);

                /* Body */
                nk_layout_row_dynamic(ctx, 140, 1);
                if (nk_group_begin(ctx, "Notebook", NK_WINDOW_BORDER))
                {
                    nk_style_pop_vec2(ctx);
                    switch (current_tab) {
                    default: break;
                    case CHART_LINE:
                        nk_layout_row_dynamic(ctx, 100, 1);
                        if (nk_chart_begin_colored(ctx, NK_CHART_LINES, nk_rgb(255,0,0), nk_rgb(150,0,0), 32, 0.0f, 1.0f)) {
                            nk_chart_add_slot_colored(ctx, NK_CHART_LINES, nk_rgb(0,0,255), nk_rgb(0,0,150),32, -1.0f, 1.0f);
                            for (i = 0, id = 0; i < 32; ++i) {
                                nk_chart_push_slot(ctx, (float)fabs(sin(id)), 0);
                                nk_chart_push_slot(ctx, (float)cos(id), 1);
                                id += step;
                            }
                        }
                        nk_chart_end(ctx);
                        break;
                    case CHART_HISTO:
                        nk_layout_row_dynamic(ctx, 100, 1);
                        if (nk_chart_begin_colored(ctx, NK_CHART_COLUMN, nk_rgb(255,0,0), nk_rgb(150,0,0), 32, 0.0f, 1.0f)) {
                            for (i = 0, id = 0; i < 32; ++i) {
                                nk_chart_push_slot(ctx, (float)fabs(sin(id)), 0);
                                id += step;
                            }
                        }
                        nk_chart_end(ctx);
                        break;
                    case CHART_MIXED:
                        nk_layout_row_dynamic(ctx, 100, 1);
                        if (nk_chart_begin_colored(ctx, NK_CHART_LINES, nk_rgb(255,0,0), nk_rgb(150,0,0), 32, 0.0f, 1.0f)) {
                            nk_chart_add_slot_colored(ctx, NK_CHART_LINES, nk_rgb(0,0,255), nk_rgb(0,0,150),32, -1.0f, 1.0f);
                            nk_chart_add_slot_colored(ctx, NK_CHART_COLUMN, nk_rgb(0,255,0), nk_rgb(0,150,0), 32, 0.0f, 1.0f);
                            for (i = 0, id = 0; i < 32; ++i) {
                                nk_chart_push_slot(ctx, (float)fabs(sin(id)), 0);
                                nk_chart_push_slot(ctx, (float)fabs(cos(id)), 1);
                                nk_chart_push_slot(ctx, (float)fabs(sin(id)), 2);
                                id += step;
                            }
                        }
                        nk_chart_end(ctx);
                        break;
                    }
                    nk_group_end(ctx);
                } else nk_style_pop_vec2(ctx);
                nk_tree_pop(ctx);
            }

            if (nk_tree_push(ctx, NK_TREE_NODE, "Simple", NK_MINIMIZED)) {
                nk_layout_row_dynamic(ctx, 300, 2);
                if (nk_group_begin(ctx, "Group_Without_Border", 0)) {
                    int i = 0;
                    char buffer[64];
                    nk_layout_row_static(ctx, 18, 150, 1);
                    for (i = 0; i < 64; ++i) {
                        sprintf(buffer, "0x%02x", i);
                        nk_labelf(ctx, NK_TEXT_LEFT, "%s: scrollable region", buffer);
                    }
                    nk_group_end(ctx);
                }
                if (nk_group_begin(ctx, "Group_With_Border", NK_WINDOW_BORDER)) {
                    int i = 0;
                    char buffer[64];
                    nk_layout_row_dynamic(ctx, 25, 2);
                    for (i = 0; i < 64; ++i) {
                        sprintf(buffer, "%08d", ((((i%7)*10)^32))+(64+(i%2)*2));
                        nk_button_label(ctx, buffer);
                    }
                    nk_group_end(ctx);
                }
                nk_tree_pop(ctx);
            }

            if (nk_tree_push(ctx, NK_TREE_NODE, "Complex", NK_MINIMIZED)) {
                int i;
                nk_layout_space_begin(ctx, NK_STATIC, 500, 64);
                nk_layout_space_push(ctx, nk_rect(0,0,150,500));
                if (nk_group_begin(ctx, "Group_left", NK_WINDOW_BORDER)) {
                    static nk_bool selected[32];
                    nk_layout_row_static(ctx, 18, 100, 1);
                    for (i = 0; i < 32; ++i)
                        nk_selectable_label(ctx, (selected[i]) ? "Selected": "Unselected", NK_TEXT_CENTERED, &selected[i]);
                    nk_group_end(ctx);
                }

                nk_layout_space_push(ctx, nk_rect(160,0,150,240));
                if (nk_group_begin(ctx, "Group_top", NK_WINDOW_BORDER)) {
                    nk_layout_row_dynamic(ctx, 25, 1);
                    nk_button_label(ctx, "#FFAA");
                    nk_button_label(ctx, "#FFBB");
                    nk_button_label(ctx, "#FFCC");
                    nk_button_label(ctx, "#FFDD");
                    nk_button_label(ctx, "#FFEE");
                    nk_button_label(ctx, "#FFFF");
                    nk_group_end(ctx);
                }

                nk_layout_space_push(ctx, nk_rect(160,250,150,250));
                if (nk_group_begin(ctx, "Group_buttom", NK_WINDOW_BORDER)) {
                    nk_layout_row_dynamic(ctx, 25, 1);
                    nk_button_label(ctx, "#FFAA");
                    nk_button_label(ctx, "#FFBB");
                    nk_button_label(ctx, "#FFCC");
                    nk_button_label(ctx, "#FFDD");
                    nk_button_label(ctx, "#FFEE");
                    nk_button_label(ctx, "#FFFF");
                    nk_group_end(ctx);
                }

                nk_layout_space_push(ctx, nk_rect(320,0,150,150));
                if (nk_group_begin(ctx, "Group_right_top", NK_WINDOW_BORDER)) {
                    static nk_bool selected[4];
                    nk_layout_row_static(ctx, 18, 100, 1);
                    for (i = 0; i < 4; ++i)
                        nk_selectable_label(ctx, (selected[i]) ? "Selected": "Unselected", NK_TEXT_CENTERED, &selected[i]);
                    nk_group_end(ctx);
                }

                nk_layout_space_push(ctx, nk_rect(320,160,150,150));
                if (nk_group_begin(ctx, "Group_right_center", NK_WINDOW_BORDER)) {
                    static nk_bool selected[4];
                    nk_layout_row_static(ctx, 18, 100, 1);
                    for (i = 0; i < 4; ++i)
                        nk_selectable_label(ctx, (selected[i]) ? "Selected": "Unselected", NK_TEXT_CENTERED, &selected[i]);
                    nk_group_end(ctx);
                }

                nk_layout_space_push(ctx, nk_rect(320,320,150,150));
                if (nk_group_begin(ctx, "Group_right_bottom", NK_WINDOW_BORDER)) {
                    static nk_bool selected[4];
                    nk_layout_row_static(ctx, 18, 100, 1);
                    for (i = 0; i < 4; ++i)
                        nk_selectable_label(ctx, (selected[i]) ? "Selected": "Unselected", NK_TEXT_CENTERED, &selected[i]);
                    nk_group_end(ctx);
                }
                nk_layout_space_end(ctx);
                nk_tree_pop(ctx);
            }

            if (nk_tree_push(ctx, NK_TREE_NODE, "Splitter", NK_MINIMIZED)) {
                const struct nk_input *in = &ctx->input;
                nk_layout_row_static(ctx, 20, 320, 1);
                nk_label(ctx, "Use slider and spinner to change tile size", NK_TEXT_LEFT);
                nk_label(ctx, "Drag the space between tiles to change tile ratio", NK_TEXT_LEFT);

                if (nk_tree_push(ctx, NK_TREE_NODE, "Vertical", NK_MINIMIZED))
                {
                    static float a = 100, b = 100, c = 100;
                    struct nk_rect bounds;

                    float row_layout[5];
                    row_layout[0] = a;
                    row_layout[1] = 8;
                    row_layout[2] = b;
                    row_layout[3] = 8;
                    row_layout[4] = c;

                    /* header */
                    nk_layout_row_static(ctx, 30, 100, 2);
                    nk_label(ctx, "left:", NK_TEXT_LEFT);
                    nk_slider_float(ctx, 10.0f, &a, 200.0f, 10.0f);

                    nk_label(ctx, "middle:", NK_TEXT_LEFT);
                    nk_slider_float(ctx, 10.0f, &b, 200.0f, 10.0f);

                    nk_label(ctx, "right:", NK_TEXT_LEFT);
                    nk_slider_float(ctx, 10.0f, &c, 200.0f, 10.0f);

                    /* tiles */
                    nk_layout_row(ctx, NK_STATIC, 200, 5, row_layout);

                    /* left space */
                    if (nk_group_begin(ctx, "left", NK_WINDOW_NO_SCROLLBAR|NK_WINDOW_BORDER|NK_WINDOW_NO_SCROLLBAR)) {
                        nk_layout_row_dynamic(ctx, 25, 1);
                        nk_button_label(ctx, "#FFAA");
                        nk_button_label(ctx, "#FFBB");
                        nk_button_label(ctx, "#FFCC");
                        nk_button_label(ctx, "#FFDD");
                        nk_button_label(ctx, "#FFEE");
                        nk_button_label(ctx, "#FFFF");
                        nk_group_end(ctx);
                    }

                    /* scaler */
                    bounds = nk_widget_bounds(ctx);
                    nk_spacing(ctx, 1);
                    if ((nk_input_is_mouse_hovering_rect(in, bounds) ||
                        nk_input_is_mouse_prev_hovering_rect(in, bounds)) &&
                        nk_input_is_mouse_down(in, NK_BUTTON_LEFT))
                    {
                        a = row_layout[0] + in->mouse.delta.x;
                        b = row_layout[2] - in->mouse.delta.x;
                    }

                    /* middle space */
                    if (nk_group_begin(ctx, "center", NK_WINDOW_BORDER|NK_WINDOW_NO_SCROLLBAR)) {
                        nk_layout_row_dynamic(ctx, 25, 1);
                        nk_button_label(ctx, "#FFAA");
                        nk_button_label(ctx, "#FFBB");
                        nk_button_label(ctx, "#FFCC");
                        nk_button_label(ctx, "#FFDD");
                        nk_button_label(ctx, "#FFEE");
                        nk_button_label(ctx, "#FFFF");
                        nk_group_end(ctx);
                    }

                    /* scaler */
                    bounds = nk_widget_bounds(ctx);
                    nk_spacing(ctx, 1);
                    if ((nk_input_is_mouse_hovering_rect(in, bounds) ||
                        nk_input_is_mouse_prev_hovering_rect(in, bounds)) &&
                        nk_input_is_mouse_down(in, NK_BUTTON_LEFT))
                    {
                        b = (row_layout[2] + in->mouse.delta.x);
                        c = (row_layout[4] - in->mouse.delta.x);
                    }

                    /* right space */
                    if (nk_group_begin(ctx, "right", NK_WINDOW_BORDER|NK_WINDOW_NO_SCROLLBAR)) {
                        nk_layout_row_dynamic(ctx, 25, 1);
                        nk_button_label(ctx, "#FFAA");
                        nk_button_label(ctx, "#FFBB");
                        nk_button_label(ctx, "#FFCC");
                        nk_button_label(ctx, "#FFDD");
                        nk_button_label(ctx, "#FFEE");
                        nk_button_label(ctx, "#FFFF");
                        nk_group_end(ctx);
                    }

                    nk_tree_pop(ctx);
                }

                if (nk_tree_push(ctx, NK_TREE_NODE, "Horizontal", NK_MINIMIZED))
                {
                    static float a = 100, b = 100, c = 100;
                    struct nk_rect bounds;

                    /* header */
                    nk_layout_row_static(ctx, 30, 100, 2);
                    nk_label(ctx, "top:", NK_TEXT_LEFT);
                    nk_slider_float(ctx, 10.0f, &a, 200.0f, 10.0f);

                    nk_label(ctx, "middle:", NK_TEXT_LEFT);
                    nk_slider_float(ctx, 10.0f, &b, 200.0f, 10.0f);

                    nk_label(ctx, "bottom:", NK_TEXT_LEFT);
                    nk_slider_float(ctx, 10.0f, &c, 200.0f, 10.0f);

                    /* top space */
                    nk_layout_row_dynamic(ctx, a, 1);
                    if (nk_group_begin(ctx, "top", NK_WINDOW_NO_SCROLLBAR|NK_WINDOW_BORDER)) {
                        nk_layout_row_dynamic(ctx, 25, 3);
                        nk_button_label(ctx, "#FFAA");
                        nk_button_label(ctx, "#FFBB");
                        nk_button_label(ctx, "#FFCC");
                        nk_button_label(ctx, "#FFDD");
                        nk_button_label(ctx, "#FFEE");
                        nk_button_label(ctx, "#FFFF");
                        nk_group_end(ctx);
                    }

                    /* scaler */
                    nk_layout_row_dynamic(ctx, 8, 1);
                    bounds = nk_widget_bounds(ctx);
                    nk_spacing(ctx, 1);
                    if ((nk_input_is_mouse_hovering_rect(in, bounds) ||
                        nk_input_is_mouse_prev_hovering_rect(in, bounds)) &&
                        nk_input_is_mouse_down(in, NK_BUTTON_LEFT))
                    {
                        a = a + in->mouse.delta.y;
                        b = b - in->mouse.delta.y;
                    }

                    /* middle space */
                    nk_layout_row_dynamic(ctx, b, 1);
                    if (nk_group_begin(ctx, "middle", NK_WINDOW_NO_SCROLLBAR|NK_WINDOW_BORDER)) {
                        nk_layout_row_dynamic(ctx, 25, 3);
                        nk_button_label(ctx, "#FFAA");
                        nk_button_label(ctx, "#FFBB");
                        nk_button_label(ctx, "#FFCC");
                        nk_button_label(ctx, "#FFDD");
                        nk_button_label(ctx, "#FFEE");
                        nk_button_label(ctx, "#FFFF");
                        nk_group_end(ctx);
                    }

                    {
                        /* scaler */
                        nk_layout_row_dynamic(ctx, 8, 1);
                        bounds = nk_widget_bounds(ctx);
                        if ((nk_input_is_mouse_hovering_rect(in, bounds) ||
                            nk_input_is_mouse_prev_hovering_rect(in, bounds)) &&
                            nk_input_is_mouse_down(in, NK_BUTTON_LEFT))
                        {
                            b = b + in->mouse.delta.y;
                            c = c - in->mouse.delta.y;
                        }
                    }

                    /* bottom space */
                    nk_layout_row_dynamic(ctx, c, 1);
                    if (nk_group_begin(ctx, "bottom", NK_WINDOW_NO_SCROLLBAR|NK_WINDOW_BORDER)) {
                        nk_layout_row_dynamic(ctx, 25, 3);
                        nk_button_label(ctx, "#FFAA");
                        nk_button_label(ctx, "#FFBB");
                        nk_button_label(ctx, "#FFCC");
                        nk_button_label(ctx, "#FFDD");
                        nk_button_label(ctx, "#FFEE");
                        nk_button_label(ctx, "#FFFF");
                        nk_group_end(ctx);
                    }
                    nk_tree_pop(ctx);
                }
                nk_tree_pop(ctx);
            }
            nk_tree_pop(ctx);
        }

        /* Input */
        if (nk_tree_push(ctx, NK_TREE_TAB, "Input", NK_MINIMIZED)) {
            const struct nk_input *in = &ctx->input;
            static const char *button_names[NK_BUTTON_MAX] = {
                "Left", "Middle", "Right", "Double Click", "X1", "X2"
            };
            int i;

            /* Mouse Buttons*/
            nk_layout_row_dynamic(ctx, 30, 1);
            nk_label(ctx, "Mouse Buttons", NK_TEXT_LEFT);
            nk_layout_row_dynamic(ctx, 20, 2);
            for (i = 0; i < NK_BUTTON_MAX; i++) {
                nk_label(ctx, button_names[i], NK_TEXT_LEFT);
                if (nk_input_is_mouse_pressed(in, (enum nk_buttons)i))
                    nk_label(ctx, "Pressed", NK_TEXT_LEFT);
                else if (nk_input_is_mouse_down(in, (enum nk_buttons)i))
                    nk_label(ctx, "Down", NK_TEXT_LEFT);
                else if (nk_input_is_mouse_released(in, (enum nk_buttons)i))
                    nk_label(ctx, "Released", NK_TEXT_LEFT);
                else
                    nk_label(ctx, "Up", NK_TEXT_LEFT);
            }

            /* Mouse Wheel */
            nk_layout_row_dynamic(ctx, 30, 1);
            nk_label(ctx, "Mouse Wheel", NK_TEXT_LEFT);
            nk_layout_row_dynamic(ctx, 20, 2);
            nk_labelf(ctx, NK_TEXT_LEFT, "X: %.2f", in->mouse.scroll_delta.x);
            nk_labelf(ctx, NK_TEXT_LEFT, "Y: %.2f", in->mouse.scroll_delta.y);
            nk_tree_pop(ctx);
        }

        if (disable_widgets)
             nk_widget_disable_end(ctx);
    }
    nk_end(ctx);
    return !nk_window_is_closed(ctx, "Overview");
}

/*
 TODO design decisions
 plural or not?  ie style_button or style_buttons?
 use the duplicate array method, or just let the user
 manually set those after calling the function by accessing ctx->style->*?
*/

static const char* symbols[NK_SYMBOL_MAX] =
{
    "NONE",
    "X",
    "UNDERSCORE",
    "CIRCLE_SOLID",
    "CIRCLE_OUTLINE",
    "RECT_SOLID",
    "RECT_OUTLINE",
    "TRIANGLE_UP",
    "TRIANGLE_DOWN",
    "TRIANGLE_LEFT",
    "TRIANGLE_RIGHT",
    "PLUS",
    "MINUS",
    "TRIANGLE_UP_OUTLINE",
    "TRIANGLE_DOWN_OUTLINE",
    "TRIANGLE_LEFT_OUTLINE",
    "TRIANGLE_RIGHT_OUTLINE"
};

static int
style_rgb(struct nk_context* ctx, const char* name, struct nk_color* color)
{
    struct nk_colorf colorf;
    nk_label(ctx, name, NK_TEXT_LEFT);
    if (nk_combo_begin_color(ctx, *color, nk_vec2(nk_widget_width(ctx), 400))) {
        nk_layout_row_dynamic(ctx, 120, 1);
        colorf = nk_color_picker(ctx, nk_color_cf(*color), NK_RGB);
        nk_layout_row_dynamic(ctx, 25, 1);
        colorf.r = nk_propertyf(ctx, "#R:", 0, colorf.r, 1.0f, 0.01f,0.005f);
        colorf.g = nk_propertyf(ctx, "#G:", 0, colorf.g, 1.0f, 0.01f,0.005f);
        colorf.b = nk_propertyf(ctx, "#B:", 0, colorf.b, 1.0f, 0.01f,0.005f);

        *color = nk_rgb_cf(colorf);

        nk_combo_end(ctx);
        return 1;
    }
    return 0;
}

/* TODO style_style_item?  how to handle images if at all? */
static void
style_item_color(struct nk_context* ctx, const char* name, struct nk_style_item* item)
{
    style_rgb(ctx, name, &item->data.color);

}

static void
style_vec2(struct nk_context* ctx, const char* name, struct nk_vec2* vec)
{
    char buffer[64];
    nk_label(ctx, name, NK_TEXT_LEFT);
    sprintf(buffer, "%.2f, %.2f", vec->x, vec->y);
    if (nk_combo_begin_label(ctx, buffer, nk_vec2(200,200))) {
        nk_layout_row_dynamic(ctx, 25, 1);
        nk_property_float(ctx, "#X:", -100.0f, &vec->x, 100.0f, 1,0.5f);
        nk_property_float(ctx, "#Y:", -100.0f, &vec->y, 100.0f, 1,0.5f);
        nk_combo_end(ctx);
    }
}

/* style_general? pass array in instead of static? */
static void
style_global_colors(struct nk_context* ctx, struct nk_color color_table[NK_COLOR_COUNT])
{
    const char* color_labels[NK_COLOR_COUNT] =
    {
        "COLOR_TEXT:",
        "COLOR_WINDOW:",
        "COLOR_HEADER:",
        "COLOR_BORDER:",
        "COLOR_BUTTON:",
        "COLOR_BUTTON_HOVER:",
        "COLOR_BUTTON_ACTIVE:",
        "COLOR_TOGGLE:",
        "COLOR_TOGGLE_HOVER:",
        "COLOR_TOGGLE_CURSOR:",
        "COLOR_SELECT:",
        "COLOR_SELECT_ACTIVE:",
        "COLOR_SLIDER:",
        "COLOR_SLIDER_CURSOR:",
        "COLOR_SLIDER_CURSOR_HOVER:",
        "COLOR_SLIDER_CURSOR_ACTIVE:",
        "COLOR_PROPERTY:",
        "COLOR_EDIT:",
        "COLOR_EDIT_CURSOR:",
        "COLOR_COMBO:",
        "COLOR_CHART:",
        "COLOR_CHART_COLOR:",
        "COLOR_CHART_COLOR_HIGHLIGHT:",
        "COLOR_SCROLLBAR:",
        "COLOR_SCROLLBAR_CURSOR:",
        "COLOR_SCROLLBAR_CURSOR_HOVER:",
        "COLOR_SCROLLBAR_CURSOR_ACTIVE:",
        "COLOR_TAB_HEADER:",
        "COLOR_KNOB:",
        "COLOR_KNOB_CURSOR:",
        "COLOR_KNOB_CURSOR_HOVER:",
        "COLOR_KNOB_CURSOR_ACTIVE:"
    };

    int clicked = 0;
    int i;

    nk_layout_row_dynamic(ctx, 30, 2);
    for (i=0; i<NK_COLOR_COUNT; ++i) {
        clicked |= style_rgb(ctx, color_labels[i], &color_table[i]);
    }

    if (clicked) {
        nk_style_from_table(ctx, color_table);
    }
}

static void
style_text(struct nk_context* ctx, struct nk_style_text* out_style)
{
    struct nk_style_text text = *out_style;

    nk_layout_row_dynamic(ctx, 30, 2);
    style_rgb(ctx, "Color:", &text.color);

    style_vec2(ctx, "Padding:", &text.padding);
    nk_property_float(ctx, "#Color Factor:", 0.0f, &text.color_factor, 1.0f, 0.1, 0.0025f);
    nk_property_float(ctx, "#Disabled Factor:", 0.0f, &text.disabled_factor, 1.0f, 0.1, 0.0025f);

    *out_style = text;
}

static void
style_button(struct nk_context* ctx, struct nk_style_button* out_style, struct nk_style_button** duplicate_styles, int n_dups)
{
    struct nk_style_button button = *out_style;

    nk_layout_row_dynamic(ctx, 30, 2);
    style_item_color(ctx, "Normal:", &button.normal);
    style_item_color(ctx, "Hover:", &button.hover);
    style_item_color(ctx, "Active:", &button.active);
    style_rgb(ctx, "Border:", &button.border_color);

    style_rgb(ctx, "Text Background:", &button.text_background);
    style_rgb(ctx, "Text Normal:", &button.text_normal);
    style_rgb(ctx, "Text Hover:", &button.text_hover);
    style_rgb(ctx, "Text Active:", &button.text_active);

    style_vec2(ctx, "Padding:", &button.padding);
    style_vec2(ctx, "Image Padding:", &button.image_padding);
    style_vec2(ctx, "Touch Padding:", &button.touch_padding);

    int cur_align = button.text_alignment-NK_TEXT_LEFT;
    int i;
    for (i=0; i<(int)NK_LEN(aligns); ++i) {
        if (button.text_alignment == aligns[i]) {
            cur_align = i;
            break;
        }
    }
    nk_label(ctx, "Text Alignment:", NK_TEXT_LEFT);
    cur_align = nk_combo(ctx, alignments, NK_LEN(alignments), cur_align, 25, nk_vec2(200,200));
    button.text_alignment = aligns[cur_align];

    nk_property_float(ctx, "#Border:", -100.0f, &button.border, 100.0f, 1,0.5f);
    nk_property_float(ctx, "#Rounding:", -100.0f, &button.rounding, 100.0f, 1,0.5f);

    nk_layout_row_dynamic(ctx, 30, 1);
    nk_property_float(ctx, "#Color Factor Background:", 0.0f, &button.color_factor_background, 1.0f, 0.1, 0.0025f);
    nk_property_float(ctx, "#Color Factor Text:", 0.0f, &button.color_factor_text, 1.0f, 0.1, 0.0025f);
    nk_property_float(ctx, "#Disabled Factor:", 0.0f, &button.disabled_factor, 1.0f, 0.1, 0.0025f);

    *out_style = button;
    if (duplicate_styles) {
        int i;
        for (i=0; i<n_dups; ++i) {
            *duplicate_styles[i] = button;
        }
    }
}

static void style_toggle(struct nk_context* ctx, struct nk_style_toggle* out_style)
{
    struct nk_style_toggle toggle = *out_style;

    nk_layout_row_dynamic(ctx, 30, 2);

    style_item_color(ctx, "Normal:", &toggle.normal);
    style_item_color(ctx, "Hover:", &toggle.hover);
    style_item_color(ctx, "Active:", &toggle.active);
    style_item_color(ctx, "Cursor Normal:", &toggle.cursor_normal);
    style_item_color(ctx, "Cursor Hover:", &toggle.cursor_hover);

    style_rgb(ctx, "Border:", &toggle.border_color);
    style_rgb(ctx, "Text Background:", &toggle.text_background);
    style_rgb(ctx, "Text Normal:", &toggle.text_normal);
    style_rgb(ctx, "Text Hover:", &toggle.text_hover);
    style_rgb(ctx, "Text Active:", &toggle.text_active);

    style_vec2(ctx, "Padding:", &toggle.padding);
    style_vec2(ctx, "Touch Padding:", &toggle.touch_padding);

    nk_property_float(ctx, "#Border:", -100.0f, &toggle.border, 100.0f, 1,0.5f);
    nk_property_float(ctx, "#Spacing:", -100.0f, &toggle.spacing, 100.0f, 1,0.5f);
    nk_property_float(ctx, "#Color Factor:", 0.0f, &toggle.color_factor, 1.0f, 0.1, 0.0025f);
    nk_property_float(ctx, "#Disabled Factor:", 0.0f, &toggle.disabled_factor, 1.0f, 0.1, 0.0025f);

    *out_style = toggle;

}

static void
style_selectable(struct nk_context* ctx, struct nk_style_selectable* out_style)
{
    struct nk_style_selectable select = *out_style;

    nk_layout_row_dynamic(ctx, 30, 2);

    style_item_color(ctx, "Normal:", &select.normal);
    style_item_color(ctx, "Hover:", &select.hover);
    style_item_color(ctx, "Pressed:", &select.pressed);
    style_item_color(ctx, "Normal Active:", &select.normal_active);
    style_item_color(ctx, "Hover Active:", &select.hover_active);
    style_item_color(ctx, "Pressed Active:", &select.pressed_active);

    style_rgb(ctx, "Text Normal:", &select.text_normal);
    style_rgb(ctx, "Text Hover:", &select.text_hover);
    style_rgb(ctx, "Text Pressed:", &select.text_pressed);
    style_rgb(ctx, "Text Normal Active:", &select.text_normal_active);
    style_rgb(ctx, "Text Hover Active:", &select.text_hover_active);
    style_rgb(ctx, "Text Pressed Active:", &select.text_pressed_active);

    style_vec2(ctx, "Padding:", &select.padding);
    style_vec2(ctx, "Image Padding:", &select.image_padding);
    style_vec2(ctx, "Touch Padding:", &select.touch_padding);

    nk_property_float(ctx, "#Rounding:", -100.0f, &select.rounding, 100.0f, 1,0.5f);
    nk_property_float(ctx, "#Color Factor:", 0.0f, &select.color_factor, 1.0f, 0.1, 0.0025f);
    nk_property_float(ctx, "#Disabled Factor:", 0.0f, &select.disabled_factor, 1.0f, 0.1, 0.0025f);

    *out_style = select;
}

static void
style_slider(struct nk_context* ctx, struct nk_style_slider* out_style)
{
    struct nk_style_slider slider = *out_style;
    struct nk_style_button* dups[1];
    nk_bool show_buttons_b;

    nk_layout_row_dynamic(ctx, 30, 2);

    style_item_color(ctx, "Normal:", &slider.normal);
    style_item_color(ctx, "Hover:", &slider.hover);
    style_item_color(ctx, "Active:", &slider.active);

    style_rgb(ctx, "Bar Normal:", &slider.bar_normal);
    style_rgb(ctx, "Bar Hover:", &slider.bar_hover);
    style_rgb(ctx, "Bar Active:", &slider.bar_active);
    style_rgb(ctx, "Bar Filled:", &slider.bar_filled);

    style_item_color(ctx, "Cursor Normal:", &slider.cursor_normal);
    style_item_color(ctx, "Cursor Hover:", &slider.cursor_hover);
    style_item_color(ctx, "Cursor Active:", &slider.cursor_active);

    style_vec2(ctx, "Cursor Size:", &slider.cursor_size);
    style_vec2(ctx, "Padding:", &slider.padding);
    style_vec2(ctx, "Spacing:", &slider.spacing);

    nk_property_float(ctx, "#Bar Height:", -100.0f, &slider.bar_height, 100.0f, 1,0.5f);
    nk_property_float(ctx, "#Rounding:", -100.0f, &slider.rounding, 100.0f, 1,0.5f);
    nk_property_float(ctx, "#Color Factor:", 0.0f, &slider.color_factor, 1.0f, 0.1, 0.0025f);
    nk_property_float(ctx, "#Disabled Factor:", 0.0f, &slider.disabled_factor, 1.0f, 0.1, 0.0025f);

    nk_layout_row_dynamic(ctx, 30, 1);
    show_buttons_b = (nk_bool)slider.show_buttons;
    nk_checkbox_label(ctx, "Show Buttons", &show_buttons_b);
    slider.show_buttons = (int)show_buttons_b;

    if (slider.show_buttons) {
        nk_layout_row_dynamic(ctx, 30, 2);
        nk_label(ctx, "Inc Symbol:", NK_TEXT_LEFT);
        slider.inc_symbol = nk_combo(ctx, symbols, NK_SYMBOL_MAX, slider.inc_symbol, 25, nk_vec2(200,200));
        nk_label(ctx, "Dec Symbol:", NK_TEXT_LEFT);
        slider.dec_symbol = nk_combo(ctx, symbols, NK_SYMBOL_MAX, slider.dec_symbol, 25, nk_vec2(200,200));

        /* necessary or do tree's always take the whole width? */
        /* nk_layout_row_dynamic(ctx, 30, 1); */
        if (nk_tree_push(ctx, NK_TREE_TAB, "Slider Buttons", NK_MINIMIZED)) {
            dups[0] = &ctx->style.slider.dec_button;
            style_button(ctx, &ctx->style.slider.inc_button, dups, 1);
            nk_tree_pop(ctx);
        }

    }

    *out_style = slider;
}

static void
style_knob(struct nk_context* ctx, struct nk_style_knob* out_style)
{
    struct nk_style_knob knob = *out_style;

    nk_layout_row_dynamic(ctx, 30, 2);

    /* background */
    style_item_color(ctx, "Normal:", &knob.normal);
    style_item_color(ctx, "Hover:", &knob.hover);
    style_item_color(ctx, "Active:", &knob.active);
    style_rgb(ctx, "Border:", &knob.border_color);

    /* knob */
    style_rgb(ctx, "Knob Normal:", &knob.knob_normal);
    style_rgb(ctx, "Knob Hover:", &knob.knob_hover);
    style_rgb(ctx, "Knob Active:", &knob.knob_active);
    style_rgb(ctx, "Knob Border:", &knob.knob_border_color);

    /* cursor */
    style_rgb(ctx, "Cursor Normal:", &knob.cursor_normal);
    style_rgb(ctx, "Cursor Hover:", &knob.cursor_hover);
    style_rgb(ctx, "Cursor Active:", &knob.cursor_active);

    /* properties */
    style_vec2(ctx, "Padding:", &knob.padding);
    style_vec2(ctx, "Spacing:", &knob.spacing);
    nk_property_float(ctx, "#Border:", -100.0f, &knob.border, 100.0f, 1,0.5f);
    nk_property_float(ctx, "#Knob Border:", -100.0f, &knob.knob_border, 100.0f, 1,0.5f);
    nk_property_float(ctx, "#Cursor Width:", -100.0f, &knob.cursor_width, 100.0f, 1,0.5f);
    nk_property_float(ctx, "#Color Factor:", 0.0f, &knob.color_factor, 1.0f, 0.1, 0.0025f);
    nk_property_float(ctx, "#Disabled Factor:", 0.0f, &knob.disabled_factor, 1.0f, 0.1, 0.0025f);

    *out_style = knob;
}

static void
style_progress(struct nk_context* ctx, struct nk_style_progress* out_style)
{
    struct nk_style_progress prog = *out_style;

    nk_layout_row_dynamic(ctx, 30, 2);

    style_item_color(ctx, "Normal:", &prog.normal);
    style_item_color(ctx, "Hover:", &prog.hover);
    style_item_color(ctx, "Active:", &prog.active);
    style_item_color(ctx, "Cursor Normal:", &prog.cursor_normal);
    style_item_color(ctx, "Cursor Hover:", &prog.cursor_hover);
    style_item_color(ctx, "Cursor Active:", &prog.cursor_active);

    /* TODO rgba? */
    style_rgb(ctx, "Border Color:", &prog.border_color);
    style_rgb(ctx, "Cursor Border Color:", &prog.cursor_border_color);

    style_vec2(ctx, "Padding:", &prog.padding);

    nk_property_float(ctx, "#Rounding:", -100.0f, &prog.rounding, 100.0f, 1,0.5f);
    nk_property_float(ctx, "#Border:", -100.0f, &prog.border, 100.0f, 1,0.5f);
    nk_property_float(ctx, "#Cursor Border:", -100.0f, &prog.cursor_border, 100.0f, 1,0.5f);
    nk_property_float(ctx, "#Cursor Rounding:", -100.0f, &prog.cursor_rounding, 100.0f, 1,0.5f);
    nk_property_float(ctx, "#Color Factor:", 0.0f, &prog.color_factor, 1.0f, 0.1, 0.0025f);
    nk_property_float(ctx, "#Disabled Factor:", 0.0f, &prog.disabled_factor, 1.0f, 0.1, 0.0025f);

    *out_style = prog;
}

static void
style_scrollbars(struct nk_context* ctx, struct nk_style_scrollbar* out_style, struct nk_style_scrollbar** duplicate_styles, int n_dups)
{
    struct nk_style_scrollbar scroll = *out_style;
    struct nk_style_button* dups[3];
    nk_bool show_buttons_b;

    nk_layout_row_dynamic(ctx, 30, 2);

    style_item_color(ctx, "Normal:", &scroll.normal);
    style_item_color(ctx, "Hover:", &scroll.hover);
    style_item_color(ctx, "Active:", &scroll.active);
    style_item_color(ctx, "Cursor Normal:", &scroll.cursor_normal);
    style_item_color(ctx, "Cursor Hover:", &scroll.cursor_hover);
    style_item_color(ctx, "Cursor Active:", &scroll.cursor_active);

    /* TODO rgba? */
    style_rgb(ctx, "Border Color:", &scroll.border_color);
    style_rgb(ctx, "Cursor Border Color:", &scroll.cursor_border_color);

    style_vec2(ctx, "Padding:", &scroll.padding);

    nk_property_float(ctx, "#Border:", -100.0f, &scroll.border, 100.0f, 1,0.5f);
    nk_property_float(ctx, "#Rounding:", -100.0f, &scroll.rounding, 100.0f, 1,0.5f);

    /* TODO naming inconsistency with style_scrollress? */
    nk_property_float(ctx, "#Cursor Border:", -100.0f, &scroll.border_cursor, 100.0f, 1,0.5f);
    nk_property_float(ctx, "#Cursor Rounding:", -100.0f, &scroll.rounding_cursor, 100.0f, 1,0.5f);
    nk_property_float(ctx, "#Color Factor:", 0.0f, &scroll.color_factor, 1.0f, 0.1, 0.0025f);
    nk_property_float(ctx, "#Disabled Factor:", 0.0f, &scroll.disabled_factor, 1.0f, 0.1, 0.0025f);

    /* TODO what is wrong with scrollbar buttons?  Also look into controlling the total width (and height) of scrollbars */
    nk_layout_row_dynamic(ctx, 30, 1);
    show_buttons_b = (nk_bool)scroll.show_buttons;
    nk_checkbox_label(ctx, "Show Buttons", &show_buttons_b);
    scroll.show_buttons = (int)show_buttons_b;

    if (scroll.show_buttons) {
        nk_layout_row_dynamic(ctx, 30, 2);
        nk_label(ctx, "Inc Symbol:", NK_TEXT_LEFT);
        scroll.inc_symbol = nk_combo(ctx, symbols, NK_SYMBOL_MAX, scroll.inc_symbol, 25, nk_vec2(200,200));
        nk_label(ctx, "Dec Symbol:", NK_TEXT_LEFT);
        scroll.dec_symbol = nk_combo(ctx, symbols, NK_SYMBOL_MAX, scroll.dec_symbol, 25, nk_vec2(200,200));

        /* nk_layout_row_dynamic(ctx, 30, 1); */
        if (nk_tree_push(ctx, NK_TREE_TAB, "Scrollbar Buttons", NK_MINIMIZED)) {
            dups[0] = &ctx->style.scrollh.dec_button;
            dups[1] = &ctx->style.scrollv.inc_button;
            dups[2] = &ctx->style.scrollv.dec_button;
            style_button(ctx, &ctx->style.scrollh.inc_button, dups, 3);
            nk_tree_pop(ctx);
        }
    }

    *out_style = scroll;
    if (duplicate_styles) {
        int i;
        for (i=0; i<n_dups; ++i) {
            *duplicate_styles[i] = scroll;
        }
    }
}

static void
style_edit(struct nk_context* ctx, struct nk_style_edit* out_style)
{
    struct nk_style_edit edit = *out_style;

    nk_layout_row_dynamic(ctx, 30, 2);

    style_item_color(ctx, "Normal:", &edit.normal);
    style_item_color(ctx, "Hover:", &edit.hover);
    style_item_color(ctx, "Active:", &edit.active);

    style_rgb(ctx, "Cursor Normal:", &edit.cursor_normal);
    style_rgb(ctx, "Cursor Hover:", &edit.cursor_hover);
    style_rgb(ctx, "Cursor Text Normal:", &edit.cursor_text_normal);
    style_rgb(ctx, "Cursor Text Hover:", &edit.cursor_text_hover);
    style_rgb(ctx, "Border:", &edit.border_color);
    style_rgb(ctx, "Text Normal:", &edit.text_normal);
    style_rgb(ctx, "Text Hover:", &edit.text_hover);
    style_rgb(ctx, "Text Active:", &edit.text_active);
    style_rgb(ctx, "Selected Normal:", &edit.selected_normal);
    style_rgb(ctx, "Selected Hover:", &edit.selected_hover);
    style_rgb(ctx, "Selected Text Normal:", &edit.selected_text_normal);
    style_rgb(ctx, "Selected Text Hover:", &edit.selected_text_hover);

    style_vec2(ctx, "Scrollbar Size:", &edit.scrollbar_size);
    style_vec2(ctx, "Padding:", &edit.padding);

    nk_property_float(ctx, "#Border:", -100.0f, &edit.border, 100.0f, 1,0.5f);
    nk_property_float(ctx, "#Rounding:", -100.0f, &edit.rounding, 100.0f, 1,0.5f);
    nk_property_float(ctx, "#Cursor Size:", -100.0f, &edit.cursor_size, 100.0f, 1,0.5f);
    nk_property_float(ctx, "#Row Padding:", -100.0f, &edit.row_padding, 100.0f, 1,0.5f);
    nk_property_float(ctx, "#Color Factor:", 0.0f, &edit.color_factor, 1.0f, 0.1, 0.0025f);
    nk_property_float(ctx, "#Disabled Factor:", 0.0f, &edit.disabled_factor, 1.0f, 0.1, 0.0025f);

    *out_style = edit;
}

static void
style_property(struct nk_context* ctx, struct nk_style_property* out_style)
{
    struct nk_style_property property = *out_style;
    struct nk_style_button* dups[1];

    nk_layout_row_dynamic(ctx, 30, 2);

    style_item_color(ctx, "Normal:", &property.normal);
    style_item_color(ctx, "Hover:", &property.hover);
    style_item_color(ctx, "Active:", &property.active);

    style_rgb(ctx, "Border:", &property.border_color);
    style_rgb(ctx, "Label Normal:", &property.label_normal);
    style_rgb(ctx, "Label Hover:", &property.label_hover);
    style_rgb(ctx, "Label Active:", &property.label_active);

    style_vec2(ctx, "Padding:", &property.padding);

    /* TODO check weird hover bug with properties, happens in overview basic section too */
    nk_property_float(ctx, "#Border:", -100.0f, &property.border, 100.0f, 1,0.5f);
    nk_property_float(ctx, "#Rounding:", -100.0f, &property.rounding, 100.0f, 1,0.5f);
    nk_property_float(ctx, "#Color Factor:", 0.0f, &property.color_factor, 1.0f, 0.1, 0.0025f);
    nk_property_float(ctx, "#Disabled Factor:", 0.0f, &property.disabled_factor, 1.0f, 0.1, 0.0025f);

    /* there is no property.show_buttons, they're always there */

    nk_label(ctx, "Left Symbol:", NK_TEXT_LEFT);
    property.sym_left = nk_combo(ctx, symbols, NK_SYMBOL_MAX, property.sym_left, 25, nk_vec2(200,200));
    nk_label(ctx, "Right Symbol:", NK_TEXT_LEFT);
    property.sym_right = nk_combo(ctx, symbols, NK_SYMBOL_MAX, property.sym_right, 25, nk_vec2(200,200));

    if (nk_tree_push(ctx, NK_TREE_TAB, "Property Buttons", NK_MINIMIZED)) {
        dups[0] = &property.dec_button;
        style_button(ctx, &property.inc_button, dups, 1);
        nk_tree_pop(ctx);
    }

    if (nk_tree_push(ctx, NK_TREE_TAB, "Property Edit", NK_MINIMIZED)) {
        style_edit(ctx, &ctx->style.property.edit);
        nk_tree_pop(ctx);
    }

    *out_style = property;
}

static void
style_chart(struct nk_context* ctx, struct nk_style_chart* out_style)
{
    struct nk_style_chart chart = *out_style;

    nk_layout_row_dynamic(ctx, 30, 2);

    style_item_color(ctx, "Background:", &chart.background);

    style_rgb(ctx, "Border:", &chart.border_color);
    style_rgb(ctx, "Selected Color:", &chart.selected_color);
    style_rgb(ctx, "Color:", &chart.color);

    style_vec2(ctx, "Padding:", &chart.padding);

    nk_property_float(ctx, "#Border:", -100.0f, &chart.border, 100.0f, 1,0.5f);
    nk_property_float(ctx, "#Rounding:", -100.0f, &chart.rounding, 100.0f, 1,0.5f);
    nk_property_float(ctx, "#Color Factor:", 0.0f, &chart.color_factor, 1.0f, 0.1, 0.0025f);
    nk_property_float(ctx, "#Disabled Factor:", 0.0f, &chart.disabled_factor, 1.0f, 0.1, 0.0025f);

    *out_style = chart;
}

static void
style_combo(struct nk_context* ctx, struct nk_style_combo* out_style)
{
    struct nk_style_combo combo = *out_style;

    nk_layout_row_dynamic(ctx, 30, 2);

    style_item_color(ctx, "Normal:", &combo.normal);
    style_item_color(ctx, "Hover:", &combo.hover);
    style_item_color(ctx, "Active:", &combo.active);

    style_rgb(ctx, "Border:", &combo.border_color);
    style_rgb(ctx, "Label Normal:", &combo.label_normal);
    style_rgb(ctx, "Label Hover:", &combo.label_hover);
    style_rgb(ctx, "Label Active:", &combo.label_active);

    nk_label(ctx, "Normal Symbol:", NK_TEXT_LEFT);
    combo.sym_normal = nk_combo(ctx, symbols, NK_SYMBOL_MAX, combo.sym_normal, 25, nk_vec2(200,200));
    nk_label(ctx, "Hover Symbol:", NK_TEXT_LEFT);
    combo.sym_hover = nk_combo(ctx, symbols, NK_SYMBOL_MAX, combo.sym_hover, 25, nk_vec2(200,200));
    nk_label(ctx, "Active Symbol:", NK_TEXT_LEFT);
    combo.sym_active = nk_combo(ctx, symbols, NK_SYMBOL_MAX, combo.sym_active, 25, nk_vec2(200,200));

    style_vec2(ctx, "Content Padding:", &combo.content_padding);
    style_vec2(ctx, "Button Padding:", &combo.button_padding);
    style_vec2(ctx, "Spacing:", &combo.spacing);

    nk_property_float(ctx, "#Border:", -100.0f, &combo.border, 100.0f, 1,0.5f);
    nk_property_float(ctx, "#Rounding:", -100.0f, &combo.rounding, 100.0f, 1,0.5f);
    nk_property_float(ctx, "#Color Factor:", 0.0f, &combo.color_factor, 1.0f, 0.1, 0.0025f);
    nk_property_float(ctx, "#Disabled Factor:", 0.0f, &combo.disabled_factor, 1.0f, 0.1, 0.0025f);

    *out_style = combo;
}

static void
style_tab(struct nk_context* ctx, struct nk_style_tab* out_style)
{
    struct nk_style_tab tab = *out_style;

    nk_layout_row_dynamic(ctx, 30, 2);

    style_item_color(ctx, "Background:", &tab.background);

    style_rgb(ctx, "Border:", &tab.border_color);
    style_rgb(ctx, "Text:", &tab.text);

    /*
     * FTR, I feel these fields are misnamed and should be sym_minimized and sym_maximized since they are
     * what show in that state, not the button to push to get to that state
     */
    nk_label(ctx, "Minimized Symbol:", NK_TEXT_LEFT);
    tab.sym_minimize = nk_combo(ctx, symbols, NK_SYMBOL_MAX, tab.sym_minimize, 25, nk_vec2(200,200));
    nk_label(ctx, "Maxmized Symbol:", NK_TEXT_LEFT);
    tab.sym_maximize = nk_combo(ctx, symbols, NK_SYMBOL_MAX, tab.sym_maximize, 25, nk_vec2(200,200));

    style_vec2(ctx, "Padding:", &tab.padding);
    style_vec2(ctx, "Spacing:", &tab.spacing);

    nk_property_float(ctx, "#Indent:", -100.0f, &tab.indent, 100.0f, 1,0.5f);
    nk_property_float(ctx, "#Border:", -100.0f, &tab.border, 100.0f, 1,0.5f);
    nk_property_float(ctx, "#Rounding:", -100.0f, &tab.rounding, 100.0f, 1,0.5f);
    nk_property_float(ctx, "#Color Factor:", 0.0f, &tab.color_factor, 1.0f, 0.1, 0.0025f);
    nk_property_float(ctx, "#Disabled Factor:", 0.0f, &tab.disabled_factor, 1.0f, 0.1, 0.0025f);

    *out_style = tab;
}

static void
style_window_header(struct nk_context* ctx, struct nk_style_window_header* out_style)
{
    struct nk_style_window_header header = *out_style;
    struct nk_style_button* dups[1];

    nk_layout_row_dynamic(ctx, 30, 2);

    style_item_color(ctx, "Normal:", &header.normal);
    style_item_color(ctx, "Hover:", &header.hover);
    style_item_color(ctx, "Active:", &header.active);

    style_rgb(ctx, "Label Normal:", &header.label_normal);
    style_rgb(ctx, "Label Hover:", &header.label_hover);
    style_rgb(ctx, "Label Active:", &header.label_active);

    style_vec2(ctx, "Label Padding:", &header.label_padding);
    style_vec2(ctx, "Padding:", &header.padding);
    style_vec2(ctx, "Spacing:", &header.spacing);

#define NUM_ALIGNS 2
    {
    const char* alignments[NUM_ALIGNS] = { "LEFT", "RIGHT" };

    nk_layout_row_dynamic(ctx, 30, 2);
    nk_label(ctx, "Button Alignment:", NK_TEXT_LEFT);
    header.align = nk_combo(ctx, alignments, NUM_ALIGNS, header.align, 25, nk_vec2(200,200));
    }
#undef NUM_ALIGNS

    nk_label(ctx, "Close Symbol:", NK_TEXT_LEFT);
    header.close_symbol = nk_combo(ctx, symbols, NK_SYMBOL_MAX, header.close_symbol, 25, nk_vec2(200,200));
    nk_label(ctx, "Minimize Symbol:", NK_TEXT_LEFT);
    header.minimize_symbol = nk_combo(ctx, symbols, NK_SYMBOL_MAX, header.minimize_symbol, 25, nk_vec2(200,200));
    nk_label(ctx, "Maximize Symbol:", NK_TEXT_LEFT);
    header.maximize_symbol = nk_combo(ctx, symbols, NK_SYMBOL_MAX, header.maximize_symbol, 25, nk_vec2(200,200));

    /* necessary or do tree's always take the whole width? */
    /* nk_layout_row_dynamic(ctx, 30, 1); */
    if (nk_tree_push(ctx, NK_TREE_TAB, "Close and Minimize Button", NK_MINIMIZED)) {
        dups[0] = &header.minimize_button;
        style_button(ctx, &header.close_button, dups, 1);
        nk_tree_pop(ctx);
    }

    *out_style = header;
}

static const char* tooltip_positions[] = {
    "TOP_LEFT",
    "TOP_CENTER",
    "TOP_RIGHT",
    "MIDDLE_LEFT",
    "MIDDLE_CENTER",
    "MIDDLE_RIGHT",
    "BOTTOM_LEFT",
    "BOTTOM_CENTER",
    "BOTTOM_RIGHT"
};

static void
style_window(struct nk_context* ctx, struct nk_style_window* out_style)
{
    /*static int cur_tooltip_pos = NK_TOP_LEFT;*/

    struct nk_style_window win = *out_style;

    nk_layout_row_dynamic(ctx, 30, 2);

    style_rgb(ctx, "Background:", &win.background);

    style_item_color(ctx, "Fixed Background:", &win.fixed_background);

    style_rgb(ctx, "Border:", &win.border_color);
    style_rgb(ctx, "Popup Border:", &win.popup_border_color);
    style_rgb(ctx, "Combo Border:", &win.combo_border_color);
    style_rgb(ctx, "Contextual Border:", &win.contextual_border_color);
    style_rgb(ctx, "Menu Border:", &win.menu_border_color);
    style_rgb(ctx, "Group Border:", &win.group_border_color);
    style_rgb(ctx, "Tooltip Border:", &win.tooltip_border_color);

    style_item_color(ctx, "Scaler:", &win.scaler);

    style_vec2(ctx, "Spacing:", &win.spacing);
    style_vec2(ctx, "Scrollbar Size:", &win.scrollbar_size);
    style_vec2(ctx, "Min Size:", &win.min_size);
    style_vec2(ctx, "Padding:", &win.padding);
    style_vec2(ctx, "Group Padding:", &win.group_padding);
    style_vec2(ctx, "Popup Padding:", &win.popup_padding);
    style_vec2(ctx, "Combo Padding:", &win.combo_padding);
    style_vec2(ctx, "Contextual Padding:", &win.contextual_padding);
    style_vec2(ctx, "Menu Padding:", &win.menu_padding);
    style_vec2(ctx, "Tooltip Padding:", &win.tooltip_padding);

    nk_label(ctx, "Tooltip Origin", NK_TEXT_LEFT);
    win.tooltip_origin = nk_combo(ctx, tooltip_positions, NK_LEN(tooltip_positions), win.tooltip_origin, 25, nk_vec2(200, 200));
    style_vec2(ctx, "Tooltip offset:", &win.tooltip_offset);

    nk_property_float(ctx, "#Rounding:", -100.0f, &win.rounding, 100.0f, 1,0.5f);
    nk_property_float(ctx, "#Combo Border:", -100.0f, &win.combo_border, 100.0f, 1,0.5f);
    nk_property_float(ctx, "#Contextual Border:", -100.0f, &win.contextual_border, 100.0f, 1,0.5f);
    nk_property_float(ctx, "#Menu Border:", -100.0f, &win.menu_border, 100.0f, 1,0.5f);
    nk_property_float(ctx, "#Group Border:", -100.0f, &win.group_border, 100.0f, 1,0.5f);
    nk_property_float(ctx, "#Tooltip Border:", -100.0f, &win.tooltip_border, 100.0f, 1,0.5f);
    nk_property_float(ctx, "#Popup Border:", -100.0f, &win.popup_border, 100.0f, 1,0.5f);
    nk_property_float(ctx, "#Border:", -100.0f, &win.border, 100.0f, 1,0.5f);

    nk_layout_row_dynamic(ctx, 30, 1);
    nk_property_float(ctx, "#Min Row Height Padding:", -100.0f, &win.min_row_height_padding, 100.0f, 1,0.5f);

    if (nk_tree_push(ctx, NK_TREE_TAB, "Window Header", NK_MINIMIZED)) {
        style_window_header(ctx, &win.header);
        nk_tree_pop(ctx);
    }

    *out_style = win;
}

static int
style_configurator(struct nk_context *ctx, struct nk_color color_table[NK_COLOR_COUNT], struct nk_rect rect)
{
    /* window flags */
    int border = nk_true;
    int no_scrollbar = nk_false;
    int scale_left = nk_false;
    nk_flags window_flags = 0;
    struct nk_style *style = NULL;
    struct nk_style_button* dups[1];

    /* window flags */
    window_flags = 0;
    if (border) window_flags |= NK_WINDOW_BORDER;
    if (no_scrollbar) window_flags |= NK_WINDOW_NO_SCROLLBAR;
    if (scale_left) window_flags |= NK_WINDOW_SCALE_LEFT;

    style = &ctx->style;

    if (nk_begin(ctx, "Configurator", rect, window_flags)) {
        if (nk_tree_push(ctx, NK_TREE_TAB, "Global Colors", NK_MINIMIZED)) {
            style_global_colors(ctx, color_table);
            nk_tree_pop(ctx);
        }

        if (nk_tree_push(ctx, NK_TREE_TAB, "Text", NK_MINIMIZED)) {
            style_text(ctx, &style->text);
            nk_tree_pop(ctx);
        }

        if (nk_tree_push(ctx, NK_TREE_TAB, "Button", NK_MINIMIZED)) {
            style_button(ctx, &style->button, NULL, 0);
            nk_tree_pop(ctx);
        }

        if (nk_tree_push(ctx, NK_TREE_TAB, "Contextual Button", NK_MINIMIZED)) {
            style_button(ctx, &style->contextual_button, NULL, 0);
            nk_tree_pop(ctx);
        }

        if (nk_tree_push(ctx, NK_TREE_TAB, "Menu Button", NK_MINIMIZED)) {
            style_button(ctx, &style->menu_button, NULL, 0);
            nk_tree_pop(ctx);
        }

        if (nk_tree_push(ctx, NK_TREE_TAB, "Combo Buttons", NK_MINIMIZED)) {
            style_button(ctx, &style->combo.button, NULL, 0);
            nk_tree_pop(ctx);
        }

        if (nk_tree_push(ctx, NK_TREE_TAB, "Tab Min/Max Buttons", NK_MINIMIZED)) {
            dups[0] = &style->tab.tab_maximize_button;
            style_button(ctx, &style->tab.tab_minimize_button, dups, 1);
            nk_tree_pop(ctx);
        }
        if (nk_tree_push(ctx, NK_TREE_TAB, "Node Min/Max Buttons", NK_MINIMIZED)) {
            dups[0] = &style->tab.node_maximize_button;
            style_button(ctx, &style->tab.node_minimize_button, dups, 1);
            nk_tree_pop(ctx);
        }

        if (nk_tree_push(ctx, NK_TREE_TAB, "Window Header Close Buttons", NK_MINIMIZED)) {
            style_button(ctx, &style->window.header.close_button, NULL, 0);
            nk_tree_pop(ctx);
        }

        if (nk_tree_push(ctx, NK_TREE_TAB, "Window Header Minimize Buttons", NK_MINIMIZED)) {
            style_button(ctx, &style->window.header.minimize_button, NULL, 0);
            nk_tree_pop(ctx);
        }

        if (nk_tree_push(ctx, NK_TREE_TAB, "Checkbox", NK_MINIMIZED)) {
            style_toggle(ctx, &style->checkbox);
            nk_tree_pop(ctx);
        }

        if (nk_tree_push(ctx, NK_TREE_TAB, "Option", NK_MINIMIZED)) {
            style_toggle(ctx, &style->option);
            nk_tree_pop(ctx);
        }

        if (nk_tree_push(ctx, NK_TREE_TAB, "Selectable", NK_MINIMIZED)) {
            style_selectable(ctx, &style->selectable);
            nk_tree_pop(ctx);
        }

        if (nk_tree_push(ctx, NK_TREE_TAB, "Slider", NK_MINIMIZED)) {
            style_slider(ctx, &style->slider);
            nk_tree_pop(ctx);
        }

        if (nk_tree_push(ctx, NK_TREE_TAB, "Knob", NK_MINIMIZED)) {
            style_knob(ctx, &style->knob);
            nk_tree_pop(ctx);
        }

        if (nk_tree_push(ctx, NK_TREE_TAB, "Progress", NK_MINIMIZED)) {
            style_progress(ctx, &style->progress);
            nk_tree_pop(ctx);
        }

        if (nk_tree_push(ctx, NK_TREE_TAB, "Scrollbars", NK_MINIMIZED)) {
            struct nk_style_scrollbar* dups[1];
            dups[0] = &style->scrollv;
            style_scrollbars(ctx, &style->scrollh, dups, 1);
            nk_tree_pop(ctx);
        }

        if (nk_tree_push(ctx, NK_TREE_TAB, "Edit", NK_MINIMIZED)) {
            style_edit(ctx, &style->edit);
            nk_tree_pop(ctx);
        }

        if (nk_tree_push(ctx, NK_TREE_TAB, "Property", NK_MINIMIZED)) {
            style_property(ctx, &style->property);
            nk_tree_pop(ctx);
        }

        if (nk_tree_push(ctx, NK_TREE_TAB, "Chart", NK_MINIMIZED)) {
            style_chart(ctx, &style->chart);
            nk_tree_pop(ctx);
        }

        if (nk_tree_push(ctx, NK_TREE_TAB, "Combo", NK_MINIMIZED)) {
            style_combo(ctx, &style->combo);
            nk_tree_pop(ctx);
        }

        if (nk_tree_push(ctx, NK_TREE_TAB, "Tab", NK_MINIMIZED)) {
            style_tab(ctx, &style->tab);
            nk_tree_pop(ctx);
        }

        if (nk_tree_push(ctx, NK_TREE_TAB, "Window", NK_MINIMIZED)) {
            style_window(ctx, &style->window);
            nk_tree_pop(ctx);
        }

        nk_layout_row_dynamic(ctx, 30, 1);
        if (nk_button_label(ctx, "Reset all styles to defaults")) {
            memcpy(color_table, nk_default_color_style, sizeof(nk_default_color_style));
            nk_style_default(ctx);
        }

    }

    nk_end(ctx);
    return !nk_window_is_closed(ctx, "Configurator");
}

/* This is a simple node editor just to show a simple implementation and that
 * it is possible to achieve it with this library. While all nodes inside this
 * example use a simple color modifier as content you could change them
 * to have your custom content depending on the node time.
 * Biggest difference to most usual implementation is that this example does
 * not have connectors on the right position of the property that it links.
 * This is mainly done out of laziness and could be implemented as well but
 * requires calculating the position of all rows and add connectors.
 * In addition adding and removing nodes is quite limited at the
 * moment since it is based on a simple fixed array. If this is to be converted
 * into something more serious it is probably best to extend it.*/
struct node {
    int ID;
    char name[32];
    struct nk_rect bounds;
    float value;
    struct nk_color color;
    int input_count;
    int output_count;
    struct node *next;
    struct node *prev;
};

struct node_link {
    int input_id;
    int input_slot;
    int output_id;
    int output_slot;
    struct nk_vec2 in;
    struct nk_vec2 out;
};

struct node_linking {
    int active;
    struct node *node;
    int input_id;
    int input_slot;
};

struct node_editor {
    int initialized;
    struct node node_buf[32];
    struct node_link links[64];
    struct node *begin;
    struct node *end;
    int node_count;
    int link_count;
    struct nk_rect bounds;
    struct node *selected;
    int show_grid;
    struct nk_vec2 scrolling;
    struct node_linking linking;
};
static struct node_editor nodeEditor;

static void
node_editor_push(struct node_editor *editor, struct node *node)
{
    if (!editor->begin) {
        node->next = NULL;
        node->prev = NULL;
        editor->begin = node;
        editor->end = node;
    } else {
        node->prev = editor->end;
        if (editor->end)
            editor->end->next = node;
        node->next = NULL;
        editor->end = node;
    }
}

static void
node_editor_pop(struct node_editor *editor, struct node *node)
{
    if (node->next)
        node->next->prev = node->prev;
    if (node->prev)
        node->prev->next = node->next;
    if (editor->end == node)
        editor->end = node->prev;
    if (editor->begin == node)
        editor->begin = node->next;
    node->next = NULL;
    node->prev = NULL;
}

static struct node*
node_editor_find(struct node_editor *editor, int ID)
{
    struct node *iter = editor->begin;
    while (iter) {
        if (iter->ID == ID)
            return iter;
        iter = iter->next;
    }
    return NULL;
}

static void
node_editor_add(struct node_editor *editor, const char *name, struct nk_rect bounds,
    struct nk_color col, int in_count, int out_count)
{
    static int IDs = 0;
    struct node *node;
    NK_ASSERT((nk_size)editor->node_count < NK_LEN(editor->node_buf));
    node = &editor->node_buf[editor->node_count++];
    node->ID = IDs++;
    node->value = 0;
    node->color = nk_rgb(255, 0, 0);
    node->input_count = in_count;
    node->output_count = out_count;
    node->color = col;
    node->bounds = bounds;
    strcpy(node->name, name);
    node_editor_push(editor, node);
}

static void
node_editor_link(struct node_editor *editor, int in_id, int in_slot,
    int out_id, int out_slot)
{
    struct node_link *link;
    NK_ASSERT((nk_size)editor->link_count < NK_LEN(editor->links));
    link = &editor->links[editor->link_count++];
    link->input_id = in_id;
    link->input_slot = in_slot;
    link->output_id = out_id;
    link->output_slot = out_slot;
}

static void
node_editor_init(struct node_editor *editor)
{
    memset(editor, 0, sizeof(*editor));
    editor->begin = NULL;
    editor->end = NULL;
    node_editor_add(editor, "Source", nk_rect(40, 10, 180, 220), nk_rgb(255, 0, 0), 0, 1);
    node_editor_add(editor, "Source", nk_rect(40, 260, 180, 220), nk_rgb(0, 255, 0), 0, 1);
    node_editor_add(editor, "Combine", nk_rect(400, 100, 180, 220), nk_rgb(0,0,255), 2, 2);
    node_editor_link(editor, 0, 0, 2, 0);
    node_editor_link(editor, 1, 0, 2, 1);
    editor->show_grid = nk_true;
}

static int
node_editor(struct nk_context *ctx, struct nk_rect rect)
{
    int n = 0;
    struct nk_rect total_space;
    const struct nk_input *in = &ctx->input;
    struct nk_command_buffer *canvas;
    struct node *updated = 0;
    struct node_editor *nodedit = &nodeEditor;

    if (!nodeEditor.initialized) {
        node_editor_init(&nodeEditor);
        nodeEditor.initialized = 1;
    }

    if (nk_begin(ctx, "NodeEdit", rect,
        NK_WINDOW_BORDER|NK_WINDOW_NO_SCROLLBAR|NK_WINDOW_MOVABLE|NK_WINDOW_CLOSABLE)) {
        /* allocate complete window space */
        canvas = nk_window_get_canvas(ctx);
        total_space = nk_window_get_content_region(ctx);
        nk_layout_space_begin(ctx, NK_STATIC, total_space.h, nodedit->node_count);
        {
            struct node *it = nodedit->begin;
            struct nk_rect size = nk_layout_space_bounds(ctx);
            struct nk_panel *node = 0;

            if (nodedit->show_grid) {
                /* display grid */
                float x, y;
                const float grid_size = 32.0f;
                const struct nk_color grid_color = nk_rgb(50, 50, 50);
                for (x = (float)fmod(size.x - nodedit->scrolling.x, grid_size); x < size.w; x += grid_size)
                    nk_stroke_line(canvas, x+size.x, size.y, x+size.x, size.y+size.h, 1.0f, grid_color);
                for (y = (float)fmod(size.y - nodedit->scrolling.y, grid_size); y < size.h; y += grid_size)
                    nk_stroke_line(canvas, size.x, y+size.y, size.x+size.w, y+size.y, 1.0f, grid_color);
            }

            /* execute each node as a movable group */
            while (it) {
                /* calculate scrolled node window position and size */
                nk_layout_space_push(ctx, nk_rect(it->bounds.x - nodedit->scrolling.x,
                    it->bounds.y - nodedit->scrolling.y, it->bounds.w, it->bounds.h));

                /* execute node window */
                if (nk_group_begin(ctx, it->name, NK_WINDOW_MOVABLE|NK_WINDOW_NO_SCROLLBAR|NK_WINDOW_BORDER|NK_WINDOW_TITLE))
                {
                    /* always have last selected node on top */

                    node = nk_window_get_panel(ctx);
                    if (nk_input_mouse_clicked(in, NK_BUTTON_LEFT, node->bounds) &&
                        (!(it->prev && nk_input_mouse_clicked(in, NK_BUTTON_LEFT,
                        nk_layout_space_rect_to_screen(ctx, node->bounds)))) &&
                        nodedit->end != it)
                    {
                        updated = it;
                    }

                    /* ================= NODE CONTENT =====================*/
                    nk_layout_row_dynamic(ctx, 25, 1);
                    nk_button_color(ctx, it->color);
                    it->color.r = (nk_byte)nk_propertyi(ctx, "#R:", 0, it->color.r, 255, 1,1);
                    it->color.g = (nk_byte)nk_propertyi(ctx, "#G:", 0, it->color.g, 255, 1,1);
                    it->color.b = (nk_byte)nk_propertyi(ctx, "#B:", 0, it->color.b, 255, 1,1);
                    it->color.a = (nk_byte)nk_propertyi(ctx, "#A:", 0, it->color.a, 255, 1,1);
                    /* ====================================================*/
                    nk_group_end(ctx);
                }
                {
                    /* node connector and linking */
                    float space;
                    struct nk_rect bounds;
                    bounds = nk_layout_space_rect_to_local(ctx, node->bounds);
                    bounds.x += nodedit->scrolling.x;
                    bounds.y += nodedit->scrolling.y;
                    it->bounds = bounds;

                    /* output connector */
                    space = node->bounds.h / (float)((it->output_count) + 1);
                    for (n = 0; n < it->output_count; ++n) {
                        struct nk_rect circle;
                        circle.x = node->bounds.x + node->bounds.w-4;
                        circle.y = node->bounds.y + space * (float)(n+1);
                        circle.w = 8; circle.h = 8;
                        nk_fill_circle(canvas, circle, nk_rgb(100, 100, 100));

                        /* start linking process */
                        if (nk_input_has_mouse_click_down_in_rect(in, NK_BUTTON_LEFT, circle, nk_true)) {
                            nodedit->linking.active = nk_true;
                            nodedit->linking.node = it;
                            nodedit->linking.input_id = it->ID;
                            nodedit->linking.input_slot = n;
                        }

                        /* draw curve from linked node slot to mouse position */
                        if (nodedit->linking.active && nodedit->linking.node == it &&
                            nodedit->linking.input_slot == n) {
                            struct nk_vec2 l0 = nk_vec2(circle.x + 3, circle.y + 3);
                            struct nk_vec2 l1 = in->mouse.pos;
                            nk_stroke_curve(canvas, l0.x, l0.y, l0.x + 50.0f, l0.y,
                                l1.x - 50.0f, l1.y, l1.x, l1.y, 1.0f, nk_rgb(100, 100, 100));
                        }
                    }

                    /* input connector */
                    space = node->bounds.h / (float)((it->input_count) + 1);
                    for (n = 0; n < it->input_count; ++n) {
                        struct nk_rect circle;
                        circle.x = node->bounds.x-4;
                        circle.y = node->bounds.y + space * (float)(n+1);
                        circle.w = 8; circle.h = 8;
                        nk_fill_circle(canvas, circle, nk_rgb(100, 100, 100));
                        if (nk_input_is_mouse_released(in, NK_BUTTON_LEFT) &&
                            nk_input_is_mouse_hovering_rect(in, circle) &&
                            nodedit->linking.active && nodedit->linking.node != it) {
                            nodedit->linking.active = nk_false;
                            node_editor_link(nodedit, nodedit->linking.input_id,
                                nodedit->linking.input_slot, it->ID, n);
                        }
                    }
                }
                it = it->next;
            }

            /* reset linking connection */
            if (nodedit->linking.active && nk_input_is_mouse_released(in, NK_BUTTON_LEFT)) {
                nodedit->linking.active = nk_false;
                nodedit->linking.node = NULL;
                fprintf(stdout, "linking failed\n");
            }

            /* draw each link */
            for (n = 0; n < nodedit->link_count; ++n) {
                struct node_link *link = &nodedit->links[n];
                struct node *ni = node_editor_find(nodedit, link->input_id);
                struct node *no = node_editor_find(nodedit, link->output_id);
                float spacei = node->bounds.h / (float)((ni->output_count) + 1);
                float spaceo = node->bounds.h / (float)((no->input_count) + 1);
                struct nk_vec2 l0 = nk_layout_space_to_screen(ctx,
                    nk_vec2(ni->bounds.x + ni->bounds.w, 3.0f + ni->bounds.y + spacei * (float)(link->input_slot+1)));
                struct nk_vec2 l1 = nk_layout_space_to_screen(ctx,
                    nk_vec2(no->bounds.x, 3.0f + no->bounds.y + spaceo * (float)(link->output_slot+1)));

                l0.x -= nodedit->scrolling.x;
                l0.y -= nodedit->scrolling.y;
                l1.x -= nodedit->scrolling.x;
                l1.y -= nodedit->scrolling.y;
                nk_stroke_curve(canvas, l0.x, l0.y, l0.x + 50.0f, l0.y,
                    l1.x - 50.0f, l1.y, l1.x, l1.y, 1.0f, nk_rgb(100, 100, 100));
            }

            if (updated) {
                /* reshuffle nodes to have least recently selected node on top */
                node_editor_pop(nodedit, updated);
                node_editor_push(nodedit, updated);
            }

            /* node selection */
            if (nk_input_mouse_clicked(in, NK_BUTTON_LEFT, nk_layout_space_bounds(ctx))) {
                it = nodedit->begin;
                nodedit->selected = NULL;
                nodedit->bounds = nk_rect(in->mouse.pos.x, in->mouse.pos.y, 100, 200);
                while (it) {
                    struct nk_rect b = nk_layout_space_rect_to_screen(ctx, it->bounds);
                    b.x -= nodedit->scrolling.x;
                    b.y -= nodedit->scrolling.y;
                    if (nk_input_is_mouse_hovering_rect(in, b))
                        nodedit->selected = it;
                    it = it->next;
                }
            }

            /* contextual menu */
            if (nk_contextual_begin(ctx, 0, nk_vec2(100, 220), nk_window_get_bounds(ctx))) {
                const char *grid_option[] = {"Show Grid", "Hide Grid"};
                nk_layout_row_dynamic(ctx, 25, 1);
                if (nk_contextual_item_label(ctx, "New", NK_TEXT_CENTERED))
                    node_editor_add(nodedit, "New", nk_rect(400, 260, 180, 220),
                            nk_rgb(255, 255, 255), 1, 2);
                if (nk_contextual_item_label(ctx, grid_option[nodedit->show_grid],NK_TEXT_CENTERED))
                    nodedit->show_grid = !nodedit->show_grid;
                nk_contextual_end(ctx);
            }
        }
        nk_layout_space_end(ctx);

        /* window content scrolling */
        if (nk_input_is_mouse_hovering_rect(in, nk_window_get_bounds(ctx)) &&
            nk_input_is_mouse_down(in, NK_BUTTON_MIDDLE)) {
            nodedit->scrolling.x += in->mouse.delta.x;
            nodedit->scrolling.y += in->mouse.delta.y;
        }
    }
    nk_end(ctx);
    return !nk_window_is_closed(ctx, "NodeEdit");
}

static void menu_and_toolbar(struct nk_context *ctx, struct nk_rect rect, struct nk_colorf bg)
{
    static nk_bool show_app_about = nk_false;
    if (nk_begin(ctx, "Toolbar", rect, NK_WINDOW_BORDER)) {
        /* menubar */
        enum menu_states {MENU_DEFAULT, MENU_WINDOWS};
        static nk_size mprog = 60;
        static int mslider = 10;
        static nk_bool mcheck = nk_true;
        nk_menubar_begin(ctx);

        /* menu #1 */
        nk_layout_row_begin(ctx, NK_STATIC, 25, 5);
        /* menu #2 */
        nk_layout_row_push(ctx, 60);
        if (nk_menu_begin_label(ctx, "MENU", NK_TEXT_LEFT, nk_vec2(200, 600))) {
            enum menu_state {MENU_NONE,MENU_FILE, MENU_EDIT,MENU_VIEW,MENU_CHART};
            static enum menu_state menu_state = MENU_NONE;
            enum nk_collapse_states state;

            state = (menu_state == MENU_FILE) ? NK_MAXIMIZED: NK_MINIMIZED;
            if (nk_tree_state_push(ctx, NK_TREE_TAB, "FILE", &state)) {
                menu_state = MENU_FILE;
                nk_menu_item_label(ctx, "New", NK_TEXT_LEFT);
                nk_menu_item_label(ctx, "Open", NK_TEXT_LEFT);
                nk_menu_item_label(ctx, "Save", NK_TEXT_LEFT);
                nk_menu_item_label(ctx, "Close", NK_TEXT_LEFT);
                nk_menu_item_label(ctx, "Exit", NK_TEXT_LEFT);
                nk_tree_pop(ctx);
            } else {
                menu_state = (menu_state == MENU_FILE) ? MENU_NONE: menu_state;
            }

            state = (menu_state == MENU_EDIT) ? NK_MAXIMIZED: NK_MINIMIZED;
            if (nk_tree_state_push(ctx, NK_TREE_TAB, "EDIT", &state)) {
                menu_state = MENU_EDIT;
                nk_menu_item_label(ctx, "Copy", NK_TEXT_LEFT);
                nk_menu_item_label(ctx, "Delete", NK_TEXT_LEFT);
                nk_menu_item_label(ctx, "Cut", NK_TEXT_LEFT);
                nk_menu_item_label(ctx, "Paste", NK_TEXT_LEFT);
                nk_tree_pop(ctx);
            } else menu_state = (menu_state == MENU_EDIT) ? MENU_NONE: menu_state;

            state = (menu_state == MENU_VIEW) ? NK_MAXIMIZED: NK_MINIMIZED;
            if (nk_tree_state_push(ctx, NK_TREE_TAB, "VIEW", &state)) {
                menu_state = MENU_VIEW;
                nk_menu_item_label(ctx, "About", NK_TEXT_LEFT);
                nk_menu_item_label(ctx, "Options", NK_TEXT_LEFT);
                nk_menu_item_label(ctx, "Customize", NK_TEXT_LEFT);
                nk_tree_pop(ctx);
            } else menu_state = (menu_state == MENU_VIEW) ? MENU_NONE: menu_state;

            state = (menu_state == MENU_CHART) ? NK_MAXIMIZED: NK_MINIMIZED;
            if (nk_tree_state_push(ctx, NK_TREE_TAB, "CHART", &state)) {
                size_t i = 0;
                const float values[]={26.0f,13.0f,30.0f,15.0f,25.0f,10.0f,20.0f,40.0f,12.0f,8.0f,22.0f,28.0f};
                menu_state = MENU_CHART;
                nk_layout_row_dynamic(ctx, 150, 1);
                nk_chart_begin(ctx, NK_CHART_COLUMN, NK_LEN(values), 0, 50);
                for (i = 0; i < NK_LEN(values); ++i)
                    nk_chart_push(ctx, values[i]);
                nk_chart_end(ctx);
                nk_tree_pop(ctx);
            } else menu_state = (menu_state == MENU_CHART) ? MENU_NONE: menu_state;
            nk_menu_end(ctx);
        }
        /* menu widgets */
        nk_layout_row_push(ctx, 70);
        /*
        nk_progress(ctx, &mprog, 100, NK_MODIFIABLE);
        nk_slider_int(ctx, 0, &mslider, 16, 1);
        nk_checkbox_label(ctx, "check", &mcheck);
        */
        nk_menubar_end(ctx);

        if (show_app_about) {
            /* about popup */
            static struct nk_rect s = {20, 100, 300, 190};
            if (nk_popup_begin(ctx, NK_POPUP_STATIC, "About", NK_WINDOW_CLOSABLE, s)) {
                nk_layout_row_dynamic(ctx, 20, 1);
                nk_label(ctx, "Nuklear", NK_TEXT_LEFT);
                nk_label(ctx, "By Micha Mettke", NK_TEXT_LEFT);
                nk_label(ctx, "nuklear is licensed under the public domain License.",  NK_TEXT_LEFT);
                nk_popup_end(ctx);
            } else show_app_about = nk_false;
        }

        nk_layout_row_static(ctx, 25, 25, 20);
        nk_button_symbol(ctx, NK_SYMBOL_CIRCLE_SOLID);
        nk_button_symbol(ctx, NK_SYMBOL_RECT_SOLID);
        nk_button_symbol(ctx, NK_SYMBOL_CIRCLE_OUTLINE);
        nk_button_symbol(ctx, NK_SYMBOL_RECT_OUTLINE);
        nk_button_symbol(ctx, NK_SYMBOL_TRIANGLE_UP);
        nk_button_symbol(ctx, NK_SYMBOL_TRIANGLE_RIGHT);
        nk_button_symbol(ctx, NK_SYMBOL_TRIANGLE_DOWN);
        nk_button_symbol(ctx, NK_SYMBOL_TRIANGLE_LEFT);
        nk_button_symbol(ctx, NK_SYMBOL_TRIANGLE_UP_OUTLINE);
        nk_button_symbol(ctx, NK_SYMBOL_TRIANGLE_RIGHT_OUTLINE);
        nk_button_symbol(ctx, NK_SYMBOL_TRIANGLE_DOWN_OUTLINE);
        nk_button_symbol(ctx, NK_SYMBOL_TRIANGLE_LEFT_OUTLINE);
        nk_button_symbol(ctx, NK_SYMBOL_CHEVRON_UP);
        nk_button_symbol(ctx, NK_SYMBOL_CHEVRON_RIGHT);
        nk_button_symbol(ctx, NK_SYMBOL_CHEVRON_DOWN);
        nk_button_symbol(ctx, NK_SYMBOL_CHEVRON_LEFT);
        nk_button_symbol(ctx, NK_SYMBOL_HAMBURGER);
        nk_button_symbol(ctx, NK_SYMBOL_X);
        nk_button_symbol(ctx, NK_SYMBOL_UNDERSCORE);
        nk_button_symbol(ctx, NK_SYMBOL_PLUS);
        nk_button_symbol(ctx, NK_SYMBOL_MINUS);

        static int property = 20;
        nk_layout_row_static(ctx, 30, 80, 1);
        if (nk_button_label(ctx, "button")) {
            fprintf(stdout, "button pressed\n");
            show_app_about = nk_true;
        }
    }
    nk_end(ctx);
}

/* ===============================================================
 *
 *                          DEMO
 *
 * ===============================================================*/
static void error_callback(int e, const char *d)
{printf("Error %d: %s\n", e, d);}

int nuklear_frontend(void)
{
    /* Platform */
    static GLFWwindow *win;
    int width = 0, height = 0;

    /* GUI */
    struct nk_context *ctx;
    struct nk_colorf bg;
#ifdef INCLUDE_FILE_BROWSER
    struct file_browser browser;
    struct media media;
#endif

    static struct nk_color color_table[NK_COLOR_COUNT];
    memcpy(color_table, nk_default_color_style, sizeof(color_table));

    /* GLFW */
    glfwSetErrorCallback(error_callback);
    if (!glfwInit()) {
        fprintf(stdout, "[GFLW] failed to init!\n");
        exit(1);
    }
    win = glfwCreateWindow(WINDOW_WIDTH, WINDOW_HEIGHT, "Embroidermodder 2.0.0 (alpha)", NULL, NULL);
    glfwMakeContextCurrent(win);
    glfwGetFramebufferSize(win, &width, &height);

    /* GUI */
    ctx = nk_glfw3_init(win, NK_GLFW3_INSTALL_CALLBACKS);
    /* Load Fonts: if none of these are loaded a default font will be used  */
    /* Load Cursor: if you uncomment cursor loading please hide the cursor */
    {struct nk_font_atlas *atlas;
    nk_glfw3_font_stash_begin(&atlas);
    /*struct nk_font *droid = nk_font_atlas_add_from_file(atlas, "../../../extra_font/DroidSans.ttf", 14, 0);*/
    /*struct nk_font *roboto = nk_font_atlas_add_from_file(atlas, "../../../extra_font/Roboto-Regular.ttf", 14, 0);*/
    /*struct nk_font *future = nk_font_atlas_add_from_file(atlas, "../../../extra_font/kenvector_future_thin.ttf", 13, 0);*/
    /*struct nk_font *clean = nk_font_atlas_add_from_file(atlas, "../../../extra_font/ProggyClean.ttf", 12, 0);*/
    /*struct nk_font *tiny = nk_font_atlas_add_from_file(atlas, "../../../extra_font/ProggyTiny.ttf", 10, 0);*/
    /*struct nk_font *cousine = nk_font_atlas_add_from_file(atlas, "../../../extra_font/Cousine-Regular.ttf", 13, 0);*/
    nk_glfw3_font_stash_end();
    /*nk_style_load_all_cursors(ctx, atlas->cursors);*/
    /*nk_style_set_font(ctx, &droid->handle);*/}

    bg.r = 0.10f, bg.g = 0.18f, bg.b = 0.24f, bg.a = 1.0f;

    #ifdef INCLUDE_FILE_BROWSER
    /* icons */
    glEnable(GL_TEXTURE_2D);
    media.icons.home = icon_load("icons/filebrowser/home.png");
    media.icons.directory = icon_load("icons/filebrowser/directory.png");
    media.icons.computer = icon_load("icons/filebrowser/computer.png");
    media.icons.desktop = icon_load("icons/filebrowser/desktop.png");
    media.icons.default_file = icon_load("icons/filebrowser/default.png");
    media.icons.text_file = icon_load("icons/filebrowser/text.png");
    media.icons.music_file = icon_load("icons/filebrowser/music.png");
    media.icons.font_file =  icon_load("icons/filebrowser/font.png");
    media.icons.img_file = icon_load("icons/filebrowser/img.png");
    media.icons.movie_file = icon_load("icons/filebrowser/movie.png");
    media_init(&media);

    file_browser_init(&browser, &media);
    #endif

    while (!glfwWindowShouldClose(win)) {
        /* Input */
        glfwPollEvents();
        if (glfwGetKey(win, GLFW_KEY_Q) == GLFW_PRESS &&
             (glfwGetKey(win, GLFW_KEY_LEFT_CONTROL) == GLFW_PRESS ||
              glfwGetKey(win, GLFW_KEY_RIGHT_CONTROL) == GLFW_PRESS)) {
            glfwSetWindowShouldClose(win, nk_true);
        }
        nk_glfw3_new_frame();

        /* GUI */
        int prompt_depth = 100;
        int toolbar_depth = 140;
        int overview_width = 400;
        int overview_height = height-toolbar_depth-prompt_depth;

        struct nk_rect toolbar_rect = nk_rect(0, 0, width, toolbar_depth);
        struct nk_rect overview_rect = nk_rect(0, toolbar_depth, overview_width, overview_height);
        struct nk_rect canvas_rect = nk_rect(overview_width, toolbar_depth, width-overview_width, overview_height);
        struct nk_rect prompt_rect = nk_rect(0, height-prompt_depth, width, prompt_depth);

        menu_and_toolbar(ctx, toolbar_rect, bg);
        canvas(ctx, canvas_rect);
        overview(ctx, overview_rect);
        style_configurator(ctx, color_table, prompt_rect);
        /* calculator(ctx);
        #ifdef INCLUDE_FILE_BROWSER
            file_browser_run(&browser, ctx);
        #endif
        node_editor(ctx, toolbar_rect); */

        /* Draw */
        glfwGetFramebufferSize(win, &width, &height);
        glViewport(0, 0, width, height);
        glClear(GL_COLOR_BUFFER_BIT);
        glClearColor(bg.r, bg.g, bg.b, bg.a);
        /* IMPORTANT: `nk_glfw_render` modifies some global OpenGL state
         * with blending, scissor, face culling and depth test and defaults everything
         * back into a default state. Make sure to either save and restore or
         * reset your own state after drawing rendering the UI. */
        nk_glfw3_render(NK_ANTI_ALIASING_ON);
        glfwSwapBuffers(win);
    }

    #ifdef INCLUDE_FILE_BROWSER
    glDeleteTextures(1,(const GLuint*)&media.icons.home.handle.id);
    glDeleteTextures(1,(const GLuint*)&media.icons.directory.handle.id);
    glDeleteTextures(1,(const GLuint*)&media.icons.computer.handle.id);
    glDeleteTextures(1,(const GLuint*)&media.icons.desktop.handle.id);
    glDeleteTextures(1,(const GLuint*)&media.icons.default_file.handle.id);
    glDeleteTextures(1,(const GLuint*)&media.icons.text_file.handle.id);
    glDeleteTextures(1,(const GLuint*)&media.icons.music_file.handle.id);
    glDeleteTextures(1,(const GLuint*)&media.icons.font_file.handle.id);
    glDeleteTextures(1,(const GLuint*)&media.icons.img_file.handle.id);
    glDeleteTextures(1,(const GLuint*)&media.icons.movie_file.handle.id);

    file_browser_free(&browser);
    #endif

    nk_glfw3_shutdown();
    glfwTerminate();
    return 0;
}

