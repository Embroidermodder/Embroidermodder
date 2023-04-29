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

#ifndef GUI_BACKEND_H
#define GUI_BACKEND_H

#ifdef __cplusplus
extern "C" {
#endif

#include <stdio.h>
#include <stdbool.h>
#include <time.h>

#include <embroidery.h>

#include <GL/glew.h>
#include <GLFW/glfw3.h>

/**
 * @brief 
 * 
 */
typedef struct DictionaryEntry_ {
    char key[200];
    char value[200];
} DictionaryEntry;

/**
 * @brief 
 * 
 */
typedef struct Dictionary_ {
    DictionaryEntry *data;
    int length;
    int max_length;
} Dictionary;


void log_debug(const char *s);
const char *translate(const char *);
void set_style(char fname[200]);
bool menu_item(const char *);
unsigned int embColor_to_int(EmbColor c, int alpha);
void draw_line(EmbVector start, EmbVector end, int color);
void draw_circle(EmbVector position, float radius, int color, bool filled);
void draw_rect(EmbVector bottom_left, EmbVector top_right, int color, bool filled);
void draw_quad(EmbVector c1, EmbVector c2, EmbVector c3, EmbVector c4, int color);
void draw_text(EmbVector position, int color, const char *text);
void begin_ui(const char *, int, int);
void end_ui(void);
void begin_child(const char *label);
void end_child(void);
bool begin_tab_bar(const char *label);
void end_tab_bar(void);
bool begin_combo(const char *label, char *data);
bool selectable(const char *label, bool);
void end_combo(void);
bool collapsing_header(const char *label);
bool button(const char *);
bool begin_tab_item(const char *str);
void edit_int(const char *str, int *data);
void end_tab_item(void);
void show_real(const char *label, EmbReal data);
void show_int(const char *label, int data);
void translated_text(const char *str);
void checkbox_data(const char *str, bool *stor, bool *value);
void edit_real(const char *str, EmbReal *data);
void same_line(void);
bool begin_menu_bar(void);
void end_menu_bar(void);
void get_current_directory(char directory[200]);
bool image_button(char* texture_label, EmbVector size);
void menu_separator(void);
EmbVector get_window_pos(void);
EmbVector get_screen_size(void);
GLuint gen_gl_texture(uint8_t* data, int w, int h, int wrap);
GLuint load_texture(char fname[200]);
bool begin_menu(const char *label);
void end_menu();
void setup_imgui(GLFWwindow *window);
void imgui_render(GLFWwindow *window);
void imgui_new_frame(void);
int load_textures(Dictionary *texture_list);
void view_tab(int);
void setup_main_widget(void);
void columns(int n, char *label);
void next_column(void);
void end_columns(void);
void display_text(const char *msg);
void header(const char *title);
void set_column_width(int, int);
void get_display_size(int *width, int *height);

#ifdef __cplusplus
}
#endif

#endif /* EMBROIDERMODDER_H */
