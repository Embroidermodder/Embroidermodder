/*!
 * \file script.h
 * \brief C core of the scripting environment.
 */

#ifndef SCRIPT_H
#define SCRIPT_H

#include <stdbool.h>
#include <stdint.h>

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

#ifdef __cplusplus
extern "C" {
#endif

typedef struct ScriptValue_ ScriptValue;
typedef struct ScriptContext_ ScriptContext;
typedef ScriptValue CommandPtr(ScriptContext *context);

/*!
 * \brief General script value to allow for argument packing in scripts.
 *
 * For strings longer than 1000 characters, we require a list of strings.
 */
struct ScriptValue_ {
    char s[1000];
    int i;
    double r;
    bool b;
    CommandPtr *c;
    unsigned char type;
};

/*!
 * \brief
 */
struct ScriptContext_ {
    ScriptValue arguments[MAX_ARGS];
    int argument_count;
    ScriptValue *command_list;
    int command_count;
    int context;
    int mode;
};

extern ScriptValue script_null;
extern ScriptValue script_true;
extern ScriptValue script_false;

ScriptValue script_bool(bool b);
ScriptValue script_int(int i);
ScriptValue script_real(double r);
ScriptValue script_command(CommandPtr *c);
ScriptValue script_str(const char *s);

int is_string(ScriptValue v);
int is_int(ScriptValue v);
int is_real(ScriptValue v);
int is_bool(ScriptValue v);
int is_command(ScriptValue v);
int is_null(ScriptValue v);

char *as_string(ScriptValue *v);
int as_int(ScriptValue v);
double as_real(ScriptValue v);
bool as_bool(ScriptValue v);

ScriptValue throwError(int type, const char *);

int streq(char *, const char *);

/* Main Window functions */
void debug(const char *s);

void alert(const char *s);
void blink_prompt(void);
void set_prompt_prefix(const char *s);
void append_prompt_history(const char *s);
void enable_prompt_rapid_fire(void);
void disable_prompt_rapid_fire(void);

void init_command(void);
void end_command(void);

void enable_move_rapid_fire(void);
void disable_move_rapid_fire(void);
void new_file(void);
void open_file(void);
void exit_program(void);
void help(void);

void about(void);
void tip_of_the_day(void);

void window_cascade(void);
void window_tile(void);
void window_close(void);
void window_close_all(void);
void window_next(void);
void window_previous(void);

const char *platform_string(void);

void message_box(const char *type, const char *title, const char *text);

void undo(void);
void redo(void);

void icon16(void);
void icon24(void);
void icon32(void);
void icon48(void);
void icon64(void);
void icon128(void);

void pan_left(void);
void pan_right(void);
void pan_up(void);
void pan_down(void);

void zoom_in(void);
void zoom_out(void);
void zoom_extents(void);

void print_area(double x, double y, double w, double h);

void day(void);
void night(void);

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
void clear_selection(void);
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

/*
ScriptValue alert_command(ScriptContext* context);
ScriptValue debug_command(ScriptContext* context);
ScriptValue error_command(ScriptContext* context);
ScriptValue todo_command(ScriptContext* context);
ScriptValue BlinkPrompt(ScriptContext* context);
ScriptValue SetPromptPrefix(ScriptContext* context);
ScriptValue AppendPromptHistory(ScriptContext* context);
ScriptValue EnablePromptRapidFire(ScriptContext* context);
ScriptValue DisablePromptRapidFire(ScriptContext* context);
ScriptValue EnableMoveRapidFire(ScriptContext* context);
ScriptValue DisableMoveRapidFire(ScriptContext* context);
ScriptValue InitCommand(ScriptContext* context);
ScriptValue EndCommand(ScriptContext* context);
ScriptValue NewFile(ScriptContext* context);
ScriptValue OpenFile(ScriptContext* context);
ScriptValue Exit(ScriptContext* context);
ScriptValue Help(ScriptContext* context);
ScriptValue About(ScriptContext* context);
ScriptValue TipOfTheDay(ScriptContext* context);
ScriptValue WindowCascade(ScriptContext* context);
ScriptValue WindowTile(ScriptContext* context);
ScriptValue WindowClose(ScriptContext* context);
ScriptValue WindowCloseAll(ScriptContext* context);
ScriptValue WindowNext(ScriptContext* context);
ScriptValue WindowPrevious(ScriptContext* context);
ScriptValue PlatformString(ScriptContext* context);
ScriptValue MessageBox(ScriptContext* context);
ScriptValue IsInt(ScriptContext* context);
ScriptValue Undo(ScriptContext* context);
ScriptValue Redo(ScriptContext* context);
ScriptValue Icon16(ScriptContext* context);
ScriptValue Icon24(ScriptContext* context);
ScriptValue Icon32(ScriptContext* context);
ScriptValue Icon48(ScriptContext* context);
ScriptValue Icon64(ScriptContext* context);
ScriptValue Icon128(ScriptContext* context);
ScriptValue PanLeft(ScriptContext* context);
ScriptValue PanRight(ScriptContext* context);
ScriptValue PanUp(ScriptContext* context);
ScriptValue PanDown(ScriptContext* context);
ScriptValue ZoomIn(ScriptContext* context);
ScriptValue ZoomOut(ScriptContext* context);
ScriptValue ZoomExtents(ScriptContext* context);
ScriptValue PrintArea(ScriptContext* context);
ScriptValue DayVision(ScriptContext* context);
ScriptValue NightVision(ScriptContext* context);
ScriptValue SetBackgroundColor(ScriptContext* context);
ScriptValue SetCrossHairColor(ScriptContext* context);
ScriptValue SetGridColor(ScriptContext* context);
ScriptValue TextFont(ScriptContext* context);
ScriptValue TextSize(ScriptContext* context);
ScriptValue TextAngle(ScriptContext* context);
ScriptValue TextBold(ScriptContext* context);
ScriptValue TextItalic(ScriptContext* context);
ScriptValue TextUnderline(ScriptContext* context);
ScriptValue TextStrikeOut(ScriptContext* context);
ScriptValue TextOverline(ScriptContext* context);
ScriptValue SetTextFont(ScriptContext* context);
ScriptValue SetTextSize(ScriptContext* context);
ScriptValue SetTextAngle(ScriptContext* context);
ScriptValue SetTextBold(ScriptContext* context);
ScriptValue SetTextItalic(ScriptContext* context);
ScriptValue SetTextUnderline(ScriptContext* context);
ScriptValue SetTextStrikeOut(ScriptContext* context);
ScriptValue SetTextOverline(ScriptContext* context);
ScriptValue PreviewOn(ScriptContext* context);
ScriptValue PreviewOff(ScriptContext* context);
ScriptValue Vulcanize(ScriptContext* context);
ScriptValue AllowRubber(ScriptContext* context);
ScriptValue SetRubberMode(ScriptContext* context);
ScriptValue SetRubberPoint(ScriptContext* context);
ScriptValue SetRubberText(ScriptContext* context);
ScriptValue AddRubber(ScriptContext* context);
ScriptValue ClearRubber(ScriptContext* context);
ScriptValue SpareRubber(ScriptContext* context);
ScriptValue AddTextMulti(ScriptContext* context);
ScriptValue AddTextSingle(ScriptContext* context);
ScriptValue AddInfiniteLine(ScriptContext* context);
ScriptValue AddRay(ScriptContext* context);
ScriptValue AddLine(ScriptContext* context);
ScriptValue AddTriangle(ScriptContext* context);
ScriptValue AddRectangle(ScriptContext* context);
ScriptValue AddRoundedRectangle(ScriptContext* context);
ScriptValue AddArc(ScriptContext* context);
ScriptValue AddCircle(ScriptContext* context);
ScriptValue AddSlot(ScriptContext* context);
ScriptValue AddEllipse(ScriptContext* context);
ScriptValue AddPoint(ScriptContext* context);
ScriptValue AddRegularPolygon(ScriptContext* context);
ScriptValue AddPolygon(ScriptContext* context);
ScriptValue AddPolyline(ScriptContext* context);
ScriptValue AddPath(ScriptContext* context);
ScriptValue AddHorizontalDimension(ScriptContext* context);
ScriptValue AddVerticalDimension(ScriptContext* context);
ScriptValue AddImage(ScriptContext* context);
ScriptValue AddDimLeader(ScriptContext* context);
ScriptValue SetCursorShape(ScriptContext* context);
ScriptValue CalculateAngle(ScriptContext* context);
ScriptValue CalculateDistance(ScriptContext* context);
ScriptValue PerpendicularDistance(ScriptContext* context);
ScriptValue NumSelected(ScriptContext* context);
ScriptValue SelectAll(ScriptContext* context);
ScriptValue AddToSelection(ScriptContext* context);
ScriptValue ClearSelection(ScriptContext* context);
ScriptValue DeleteSelected(ScriptContext* context);
ScriptValue CutSelected(ScriptContext* context);
ScriptValue CopySelected(ScriptContext* context);
ScriptValue PasteSelected(ScriptContext* context);
ScriptValue MoveSelected(ScriptContext* context);
ScriptValue ScaleSelected(ScriptContext* context);
ScriptValue RotateSelected(ScriptContext* context);
ScriptValue MirrorSelected(ScriptContext* context);
ScriptValue QSnapX(ScriptContext* context);
ScriptValue QSnapY(ScriptContext* context);
ScriptValue MouseX(ScriptContext* context);
ScriptValue MouseY(ScriptContext* context);
ScriptValue Include(ScriptContext* context);
*/

#ifdef __cplusplus
}
#endif

#endif // SCRIPT_H

