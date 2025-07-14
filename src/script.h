/*!
 * \file script.h
 * \brief C core of the scripting environment.
 */

#ifndef SCRIPT_H
#define SCRIPT_H

#include <stdbool.h>
#include <stdint.h>

#include "scheme.h"
#include "scheme-private.h"
#include "dynload.h"

#define TypeError                 0
#define NotEnoughArgumentsError   1
#define GeneralError              2

/*!
 * NOTE: main() is run every time the command is started.
 *       Use it to reset variables so they are ready to go.
 *
 * NOTE: click() is run only for left clicks.
 *       Middle clicks are used for panning.
 *       Right clicks bring up the context menu.
 *
 * NOTE: context() is run when a context menu entry is chosen.
 *
 * NOTE: prompt() is run when Enter is pressed.
 *       appendPromptHistory is automatically called before prompt()
 *       is called so calling it is only needed for erroneous input.
 *       Any text is in the command prompt is sent as an uppercase string.
 */
#define CONTEXT_MAIN              0
#define CONTEXT_CLICK             1
#define CONTEXT_CONTEXT           2
#define CONTEXT_PROMPT            3

#define MAX_ARGS                 50

int load_scheme(void);
int run_command(const char *line);

int streq(char *, const char *);

/* Main Window functions */
void debug(const char *s);

void alert(const char *s);
void blink_prompt(void);
void set_prompt_prefix(const char *s);
void append_prompt_history(const char *s);

void init_command(void);
void clear_selection(void);
void end_command(void);

const char *platform_string(void);

void message_box(const char *type, const char *title, const char *text);

void print_area(double x, double y, double w, double h);

void set_background_color(uint8_t r, uint8_t g, uint8_t b);
void set_crosshair_color(uint8_t r, uint8_t g, uint8_t b);
void set_grid_color(uint8_t r, uint8_t g, uint8_t b);

const char *text_font(void);
double text_size(void);
double text_angle(void);
bool text_bold(void);
bool text_italic(void);
bool text_underline(void);
bool text_strikeout(void);
bool text_overline(void);

void set_text_font(const char *s);
void set_text_size(double size);
void set_text_angle(double angle);
void set_text_bold(bool bold);
void set_text_italic(bool italic);
void set_text_underline(bool underline);
void set_text_strikeout(bool strikeout);
void set_text_overline(bool overline);

void preview_on(int clone, int mode, double x, double y, double data);
void preview_off(void);

void vulcanize(void);
void clear_rubber(void);
bool allow_rubber(void);
void spare_rubber(int64_t id);
void set_rubber_filter(int64_t id);
void set_rubber_mode(int mode);
void set_rubber_point(const char *key, double x, double y);
void set_rubber_text(const char *key, const char *txt);

void add_text_multi(const char *s, double x, double y, double rot, bool fill, int rubberMode);
void add_text_single(const char *s, double x, double y, double roy, bool fill, int rubberMode);
void add_infinite_line(double x1, double y1, double x2, double y2, double rot);
void add_ray(double x1, double y1, double x2, double y2, double rot);
void add_line(double x1, double y1, double x2, double y2, double rot, int rubberMode);
void add_triangle(double x1, double y1, double x2, double y2, double x3, double y3, double rot, bool fill);
void add_rectangle(double x, double y, double w, double h, double rot, bool fill, int rubberMode);
void add_rounded_rectangle(double x, double y, double w, double h, double rad, double rot, bool fill);
void add_arc(double startX, double startY, double midX, double midY, double endX, double endY, int rubberMode);
void add_circle(double centerX, double centerY, double radius, bool fill, int rubberMode);
void add_slot(double centerX, double centerY, double diameter, double length, double rot, bool fill, int rubberMode);
void add_ellipse(double centerX, double centerY, double width, double height, double rot, bool fill, int rubberMode);
void add_point(double x, double y);
void add_regular_polygon(double centerX, double centerY, int sides, int mode, double rad, double rot, bool fill);
void add_polygon(double startX, double startY, int rubberMode);
void add_polyline(double startX, double startY, int rubberMode);
void add_path(double startX, double startY, int rubberMode);
void add_horizontal_dimension(double x1, double y1, double x2, double y2, double legHeight);
void add_veritical_dimension(double x1, double y1, double x2, double y2, double legHeight);
void add_image(const char *img, double x, double y, double w, double h, double rot);
void add_dimleader(double x1, double y1, double x2, double y2, double rot, int rubberMode);

void set_cursor_shape(const char *shape);

double calculate_angle(double x1, double y1, double x2, double y2);
double calculate_distance(double x1, double y1, double x2, double y2);
double perpendicular_distance(double px, double py, double x1, double y1, double x2, double y2);

int num_selected(void);
void select_all(void);
void add_to_selection(void);
void delete_selected(void);
void cut_selected(double x, double y);
void copy_selected(double x, double y);
void paste_selected(double x, double y);
void move_selected(double dx, double dy);
void scale_selected(double x, double y, double factor);
void rotate_selected(double x, double y, double rot);
void mirror_selected(double x1, double y1, double x2, double y2);

double qsnapx(void);
double qsnapy(void);
double mousex(void);
double mousey(void);

#endif // SCRIPT_H

