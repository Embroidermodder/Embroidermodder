/*
 * Embroidermodder 2.
 *
 * ------------------------------------------------------------
 *
 * Copyright 2013-2023 The Embroidermodder Team
 * Embroidermodder 2 is Open Source Software.
 * See LICENSE for licensing terms.
 *
 * ------------------------------------------------------------
 *
 * Use Python's PEP7 style guide.
 *     https://peps.python.org/pep-0007/
 *
 * ------------------------------------------------------------
 *
 * Since we use a lot of switch tables and a generally
 * procedural style, most of these constants are for identifying
 * our internal types. More general constants belong
 * in "embroidery.h" which does all the grunt work for our geometry.
 */

#ifndef __EMBROIDERMODDER_CONSTANTS__
#define __EMBROIDERMODDER_CONSTANTS__

#include "macros.h"

#ifdef __cplusplus
extern "C" {
#endif

#include <stdio.h>
#include <stdbool.h>
#include <inttypes.h>

#include "../extern/libembroidery/embroidery.h"

typedef struct String_ {
	char *data;
	int max_length;
} String;

typedef struct StringTable_ {
    String **data;
	int entries;
	int max_length;
} StringTable;

/* . */
typedef struct GeometryData_ {
    int32_t mode;
    int32_t numPoints;
    int8_t preview;
    unsigned char firstRun;

    EmbArc arc;
    EmbCircle circle;
    EmbEllipse ellipse;
    EmbLine line;
    EmbPoint point;
    EmbRect rect;
    EmbVector center;
    EmbVector pointI;
    EmbVector pointC;
    char polyType[MAX_STRING_LENGTH];

    EmbVector point1;
    EmbVector point2;
    EmbVector first;
    EmbVector prev;
    EmbVector scale;

    EmbReal arrowStyleAngle;
    EmbReal arrowStyleLength;
    EmbReal lineStyleAngle;
    EmbReal lineStyleLength;

    EmbReal text_size;
} GeometryData;

/* . */
typedef struct MenuData_ {
    int32_t id;
    const char key[MAX_STRING_LENGTH];
    int32_t entries[MAX_TOOLBAR_LENGTH];
} MenuData;

/* . */
typedef struct ToolbarData_ {
    int32_t id;
    const char key[MAX_STRING_LENGTH];
    int32_t entries[MAX_TOOLBAR_LENGTH];
    char horizontal;
} ToolbarData;

/* . */
typedef struct CommandData_ {
    int32_t id;
    char icon[MAX_STRING_LENGTH];
    char command[MAX_STRING_LENGTH];
    char tooltip[MAX_STRING_LENGTH];
    char statustip[MAX_STRING_LENGTH];
    char shortcut[MAX_STRING_LENGTH];
    int8_t min_args;
    int8_t gview;
    int8_t gscene;
    int8_t undo;
    int32_t flags;
} CommandData;

/* . */
typedef struct EditorData_ {
    int32_t groupbox;
    int32_t key;
    char icon[MAX_STRING_LENGTH];
    char label[MAX_STRING_LENGTH];
    int32_t type;
    char map_signal[MAX_STRING_LENGTH];
} EditorData;

/* . */
typedef struct Setting_ {
    int32_t id;
    char key[MAX_STRING_LENGTH];
    char value[MAX_STRING_LENGTH];
    int type;
} Setting;

/* . */
typedef struct ViewData_ {
    uint8_t grippingActive;
    uint8_t rapidMoveActive;
    uint8_t previewActive;
    uint8_t pastingActive;
    uint8_t movingActive;
    uint8_t selectingActive;
    uint8_t zoomWindowActive;
    uint8_t panningRealTimeActive;
    uint8_t panningPointActive;
    uint8_t panningActive;
    uint8_t qSnapActive;
} ViewData;

/* . */
typedef struct RubberPoint_ {
    char key[MAX_STRING_LENGTH];
    char text[MAX_STRING_LENGTH];
    EmbVector position;
} RubberPoint;

/* . */
typedef struct UndoData_ {
    EmbVector pivot;
    EmbVector before;
} UndoData;

/* . */
typedef struct Node_ {
    char s[MAX_STRING_LENGTH];
    int32_t i;
    EmbReal r;
} Node;

/* . */
typedef struct WidgetData_ {
    int type;
    char label[MAX_STRING_LENGTH];
    char icon[MAX_STRING_LENGTH];
    EmbReal single_step;
    EmbReal lower;
    EmbReal upper;
    int key;
    int position[2];
} WidgetData;

GeometryData *geometry_init(int type);
void geometry_free(GeometryData *g);
void geometry_left_click(GeometryData *geometry, EmbVector v);
void geometry_prompt(
    GeometryData *geometry,
    char input[MAX_STRING_LENGTH],
    char output[MAX_STRING_LENGTH]);
void geometry_context(
    void *m,
    GeometryData *geometry,
    char output[MAX_STRING_LENGTH]);
void geometry_update(GeometryData *g);
void geometry_set_flag(GeometryData *g, uint64_t flag);
void geometry_unset_flag(GeometryData *g, uint64_t flag);

EmbVector get_vector(GeometryData *g, int64_t id);
EmbReal get_real(GeometryData *g, int64_t id);
int32_t get_int(GeometryData *g, int64_t id);
void get_str(GeometryData *g, int64_t id, char *s);

void set_vector(GeometryData *g, int64_t id, EmbVector v);
void set_real(GeometryData *g, int64_t id, EmbReal r);
void set_int(GeometryData *g, int64_t id, int32_t i);
void set_str(GeometryData *g, int64_t id, char *str);

const char *add_geometry(char argv[MAX_ARGS][MAX_STRING_LENGTH], int argc);

/* Utility */
void debug_message(char *msg, ...);
int read_settings(void);
void write_settings(void);
EmbVector rotate_vector(EmbVector v, EmbReal alpha);
const char *actuator(const char *string);
int string_equal(const char *a, const char *b);
void emb_sleep(int seconds);
int string_array_length(const char *list[]);
bool save_current_file(const char *fileName);
int find_command(const char *command);

const char *run_script_file(char *fname);
const char *run_script(char **script);
const char *translate(char *str);

/* Utility Functions. */
unsigned char validRGB(int r, int g, int b);
int str_contains(char *s, char c);
EmbReal fourier_series(EmbReal arg, EmbReal *terms, int n_terms);
bool willUnderflowInt32(int64_t a, int64_t b);
bool willOverflowInt32(int64_t a, int64_t b);
int roundToMultiple(bool roundUp, int numToRound, int multiple);
int tokenize(char **argv, char *str, const char delim);
void emb_sleep(int seconds);
char *platformString(void);
void get_n_reals(float result[], char *argv[], int n, int offset);
void clear_selection(void);

String *create_string(int length);
void resize_string(String *s, int length);
void free_string(String *s);

StringTable *create_string_table(int, int);
void resize_string_table(StringTable *t, int length);
void add_string_to_table(String *s, StringTable *t);
void free_string_table(StringTable *t);

int load_ui(const char *appDir);

/* The Settings System
 *
 * Rather than pollute the global namespace, we collect together all the global
 * settings into a structure that stores them. This also allows us to create a
 * complete copy of the settings for the purpose of restoring them if the user
 * cancels out of the Settings Dialog.
 */
extern const char *settings_labels[];
extern Node settings[SETTINGS_TOTAL], dialog[SETTINGS_TOTAL],
    preview[SETTINGS_TOTAL], accept_[SETTINGS_TOTAL];

extern CommandData command_table[MAX_COMMANDS];
extern EditorData all_line_editors[MAX_EDITORS];
extern EditorData all_spinbox_editors[MAX_EDITORS];
extern Setting settings_data[];

/* Properties */
extern int general_props[];
extern int display_props[];
extern int prompt_props[];
extern int quick_snap_props[];
extern int opensave_props[];
extern int selection_props[];
extern int grid_ruler_props[];
extern int accept_preview_props[];
extern int accept_accept_props[];
extern const char *load_from_file_enabled[];
extern const char *grid_type_visibility_lattice[];
extern const char *grid_type_visibility_circular[];
extern const char *tab_names[];

/* Other strings and string tables. */
#define STR(A) extern String *A;
#define TABLE(A) extern StringTable *A;
#include "tables.h"
#undef TABLE
#undef STR

extern const char *usage_msg;
extern const char *extensions[];
extern const char *default_prompt_style[];
extern const char *details_labels[];
extern const char *command_labels[];
extern const char *button_list[];
extern const char *tips[];

/* Menus data */
extern MenuData menu_data[MAX_MENUS];
extern int32_t menubar_order[];

/* Toolbar data */
extern ToolbarData toolbar_data[MAX_TOOLBARS];
extern int32_t top_toolbar_layout[];
extern int32_t bottom_toolbar_layout[];
extern int32_t side_toolbar_layout[];

/* Property editor data */
extern const char *group_box_data[];
extern const int32_t group_box_ids[];

extern const char *geometry_subcommands[];

/* Widget data */
extern WidgetData grid_geometry_widgets[];
extern WidgetData qsnap_mode_checkboxes[];

#ifdef __cplusplus
}
#endif

#endif
