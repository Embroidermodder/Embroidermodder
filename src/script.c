/*
 * Embroidermodder 2.
 *
 * Copyright 2011-2024 The Embroidermodder Team
 * Embroidermodder 2 is Open Source Software, see LICENSE.md for licensing terms.
 *
 * Read the reference manual (https://www.libembroidery.org/downloads/emrm.pdf)
 * for advice on altering this file.
 *
 * Script
 */

#include <string.h>
#include <stdlib.h>
#include <stdarg.h>
#include <errno.h>

#include "core.h"

#include "toml.h"

ScriptValue state[MAX_STATE_VARIABLES];
int state_length = 0;

StringSetting general_language;
StringSetting general_icon_theme;
IntSetting general_icon_size;
BoolSetting general_mdi_bg_use_logo;
BoolSetting general_mdi_bg_use_texture;
BoolSetting general_mdi_bg_use_color;
StringSetting general_mdi_bg_logo;
StringSetting general_mdi_bg_texture;
IntSetting general_mdi_bg_color;
BoolSetting general_tip_of_the_day;
IntSetting general_current_tip;
BoolSetting general_system_help_browser;
BoolSetting general_check_for_updates;

BoolSetting display_use_opengl;
BoolSetting display_renderhint_aa;
BoolSetting display_renderhint_text_aa;
BoolSetting display_renderhint_smooth_pix;
BoolSetting display_renderhint_high_aa;
BoolSetting display_renderhint_noncosmetic;
BoolSetting display_show_scrollbars;
IntSetting display_scrollbar_widget_num;
IntSetting display_crosshair_color;
IntSetting display_bg_color;
IntSetting display_selectbox_left_color;
IntSetting display_selectbox_left_fill;
IntSetting display_selectbox_right_color;
IntSetting display_selectbox_right_fill;
IntSetting display_selectbox_alpha;
RealSetting display_zoomscale_in;
RealSetting display_zoomscale_out;
IntSetting display_crosshair_percent;
StringSetting display_units;

IntSetting prompt_text_color;
IntSetting prompt_bg_color;
StringSetting prompt_font_family;
StringSetting prompt_font_style;
IntSetting prompt_font_size;
BoolSetting prompt_save_history;
BoolSetting prompt_save_history_as_html;
StringSetting prompt_save_history_filename;

StringSetting opensave_custom_filter;
StringSetting opensave_open_format;
BoolSetting opensave_open_thumbnail;
StringSetting opensave_save_format;
BoolSetting opensave_save_thumbnail;
IntSetting opensave_recent_max_files;
StringTableSetting opensave_recent_list_of_files;
StringSetting opensave_recent_directory;
IntSetting opensave_trim_dst_num_jumps;

StringSetting printing_default_device;
BoolSetting printing_use_last_device;
BoolSetting printing_disable_bg;

BoolSetting grid_show_on_load;
BoolSetting grid_show_origin;
BoolSetting grid_color_match_crosshair;
IntSetting grid_color;
BoolSetting grid_load_from_file;
StringSetting grid_type;
BoolSetting grid_center_on_origin;
RealSetting grid_center_x;
RealSetting grid_center_y;
RealSetting grid_size_x;
RealSetting grid_size_y;
RealSetting grid_spacing_x;
RealSetting grid_spacing_y;
RealSetting grid_size_radius;
RealSetting grid_spacing_radius;
RealSetting grid_spacing_angle;

BoolSetting ruler_show_on_load;
BoolSetting ruler_metric;
IntSetting ruler_color;
IntSetting ruler_pixel_size;

BoolSetting qsnap_enabled;
IntSetting qsnap_locator_color;
IntSetting qsnap_locator_size;
IntSetting qsnap_aperture_size;
BoolSetting qsnap_endpoint;
BoolSetting qsnap_midpoint;
BoolSetting qsnap_center;
BoolSetting qsnap_node;
BoolSetting qsnap_quadrant;
BoolSetting qsnap_intersection;
BoolSetting qsnap_extension;
BoolSetting qsnap_insertion;
BoolSetting qsnap_perpendicular;
BoolSetting qsnap_tangent;
BoolSetting qsnap_nearest;
BoolSetting qsnap_apparent;
BoolSetting qsnap_parallel;

BoolSetting lwt_show_lwt;
BoolSetting lwt_real_render;
RealSetting lwt_default_lwt;

BoolSetting selection_mode_pickfirst;
BoolSetting selection_mode_pickadd;
BoolSetting selection_mode_pickdrag;
IntSetting selection_coolgrip_color;
IntSetting selection_hotgrip_color;
IntSetting selection_grip_size;
IntSetting selection_pickbox_size;

StringSetting text_font;
RealSetting text_size;
RealSetting text_angle;
BoolSetting text_style_bold;
BoolSetting text_style_italic;
BoolSetting text_style_underline;
BoolSetting text_style_overline;
BoolSetting text_style_strikeout;

const char *index_name[] = {
    "one",
    "two",
    "three",
    "four",
    "five",
    "six",
    "seven"
};

const char *index_th_name[] = {
    "first",
    "second",
    "third",
    "fourth",
    "fifth",
    "sixth",
    "seventh"
};

ScriptValue script_null = {
    .r = 0.0f,
    .i = 0,
    .b = false,
    .s = "",
    .label = "NULL",
    .type = SCRIPT_NULL
};

ScriptValue script_true = {
    .r = 0.0f,
    .i = 1,
    .b = true,
    .s = "",
    .label = "true",
    .type = SCRIPT_BOOL
};

ScriptValue script_false = {
    .r = 0.0f,
    .i = 0,
    .b = false,
    .s = "",
    .label = "false",
    .type = SCRIPT_BOOL
};

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
    context->n_variables = 0;
    return context;
}

void
add_string_variable(ScriptEnv *context, const char *label, const char *s)
{
    strcpy(context->variable[context->n_variables].label, label);
    strcpy(context->variable[context->n_variables].s, s);
    context->variable[context->n_variables].type = SCRIPT_STRING;
    context->n_variables++;
}

void
add_int_variable(ScriptEnv *context, const char *label, int i)
{
    strcpy(context->variable[context->n_variables].label, label);
    context->variable[context->n_variables].i = i;
    context->variable[context->n_variables].type = SCRIPT_INT;
    context->n_variables++;
}

void
add_real_variable(ScriptEnv *context, const char *label, double r)
{
    strcpy(context->variable[context->n_variables].label, label);
    context->variable[context->n_variables].r = r;
    context->variable[context->n_variables].type = SCRIPT_REAL;
    context->n_variables++;
}

const char *
script_get_string(ScriptEnv *context, const char *label)
{
    int i;
    for (i=0; i<context->n_variables; i++) {
        if (!strcmp(context->variable[i].label, label)) {
            return context->variable[i].s;
        }
    }
    return "ERROR: string not found.";
}

int
script_get_int(ScriptEnv *context, const char *label)
{
    int i;
    for (i=0; i<context->n_variables; i++) {
        if (!strcmp(context->variable[i].label, label)) {
            return context->variable[i].i;
        }
    }
    return -1;
}

double
script_get_real(ScriptEnv *context, const char *label)
{
    int i;
    for (i=0; i<context->n_variables; i++) {
        if (!strcmp(context->variable[i].label, label)) {
            return context->variable[i].r;
        }
    }
    return -1.0;
}

int
script_set_string(ScriptEnv *context, const char *label, const char *s)
{
    int i;
    for (i=0; i<context->n_variables; i++) {
        if (!strcmp(context->variable[i].label, label)) {
            strcpy(context->variable[i].s, s);
            return 1;
        }
    }
    return 0;
}

int
script_set_int(ScriptEnv *context, const char *label, int x)
{
    int i;
    for (i=0; i<context->n_variables; i++) {
        if (!strcmp(context->variable[i].label, label)) {
            context->variable[i].i = x;
            return 1;
        }
    }
    return 0;
}

/* . */
int
script_set_real(ScriptEnv *context, const char *label, double r)
{
    int i;
    for (i=0; i<context->n_variables; i++) {
        if (!strcmp(context->variable[i].label, label)) {
            context->variable[i].r = r;
            return 1;
        }
    }
    return 0;
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
script_real(double r)
{
    ScriptValue value;
    value.type = SCRIPT_REAL;
    value.r = r;
    return value;
}

/* . */
ScriptValue
script_string(const char *s)
{
    ScriptValue value;
    value.type = SCRIPT_STRING;
    strcpy(value.s, s);
    return value;
}

/* These pack the arguments for function calls in the command environment. */
ScriptEnv *
add_string_argument(ScriptEnv *context, const char *s)
{
    strcpy(context->argument[context->argumentCount].s, s);
    context->argument[context->argumentCount].type = SCRIPT_STRING;
    context->argumentCount++;
    return context;
}

ScriptEnv *
add_real_argument(ScriptEnv *context, double r)
{
    context->argument[context->argumentCount].r = r;
    context->argument[context->argumentCount].type = SCRIPT_REAL;
    context->argumentCount++;
    return context;
}

ScriptEnv *
add_int_argument(ScriptEnv *context, int i)
{
    context->argument[context->argumentCount].i = i;
    context->argument[context->argumentCount].type = SCRIPT_INT;
    context->argumentCount++;
    return context;
}

int
parse_floats(char *line, float result[], int n)
{
    char substring[100];
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
parse_vector(char *line, EmbVector *v)
{
    float v_[2];
    if (parse_floats(line, v_, 2) == 2) {
        return 0;
    }
    v->x = v_[0];
    v->y = v_[1];
    return 1;
}

/* FIXME: parse strings arguments with quotes.
 *
 * There are 5 steps to the parsing algorithm:
 *     1. Split the string into string arguments in the style of "argc, argv".
 *     2. Identify the command by the first argument.
 *     3. Process arguments into ScriptValues.
 *     4. Check that the arguments parsed matched the required arguments for that
 *        commands.
 *     5. Call the main function.
 */
 #if 0
ScriptValue
command_prompt(ScriptEnv *context, const char *line)
{
    /* Split arguments into seperate strings. */
    int i;
    char args[10][MAX_STRING_LENGTH];
    char *c;
    int n_args = 0;
    int str_pos = 0;
    for (c=(char*)line; *c; c++) {
        args[n_args][str_pos] = *c;
        if (*c == ' ') {
            args[n_args][str_pos] = 0;
            n_args++;
            str_pos = 0;
        }
        else {
            str_pos++;
        }
    }
    args[n_args][str_pos] = 0;

    /* Identify function. */
    int function = -1;
    for (i=0; command_data[i].id != -2; i++) {
        if (!strcmp(command_data[i].icon, args[0])) {
            function = i;
            break;
        }
    }
    if (function < 0) {
        return script_false;
    }

    /* Check there are enough arguments. */
    if (strlen(command_data[function].arguments) != n_args) {
        return script_false;
    }

    /* Parse arguments into ScriptValues. */
    if (n_args > 1) {
        for (i=1; i<n_args; i++) {
            switch (command_data[function].arguments[i]) {
            case 'i':
            case 'b':
                context = add_int_argument(context, atoi(args[i-1]));
                break;
            case 's':
                context = add_string_argument(context, args[i-1]);
                break;
            case 'r':
                context = add_real_argument(context, atof(args[i-1]));
                break;
            default:
                break;
            }
        }
    }

    /* Call command's main. */
    return command_data[function].main(context);
}
#endif

/* . */
bool
validRGB(float r, float g, float b)
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

/* Within the state, if a table starts on index i then the end of the table
 * is at index i + state[i].n_leaves - 1.
 *
 * For regular lookups that are expensive we can keep the state variable's index
 * in another variable.
 */
int
string_array_length(const char *s)
{
    int key = get_state_variable(s);
    /* printf("array_length call: %s %d\n", s, state[key].n_leaves); */
    return state[key].n_leaves;
}

/* . */
int
get_state_variable(const char *key)
{
    int i;
    for (i=0; i<state_length; i++) {
        if (!strcmp(state[i].label, key)) {
            /* printf("get_state_variable call: %s %s\n", state[i].label, state[i].s); */
            return i;
        }
    }
    return -1;
}

/* TODO: error reporting. */
void
add_state_int_variable(char *label, int i)
{
    strncpy(state[state_length].label, label, MAX_LABEL_LENGTH);
    state[state_length].i = i;
    state[state_length].type = SCRIPT_INT;
    state_length++;
}

/* TODO: error reporting. */
void
add_state_string_variable(char *label, char *s)
{
    strncpy(state[state_length].label, label, MAX_LABEL_LENGTH);
    strncpy(state[state_length].s, s, MAX_STATE_STRING_LENGTH);
    state[state_length].type = SCRIPT_STRING;
    state_length++;
}

/* TODO: error reporting. */
void
add_state_real_variable(char *label, double r)
{
    strncpy(state[state_length].label, label, MAX_LABEL_LENGTH);
    state[state_length].r = r;
    state[state_length].type = SCRIPT_REAL;
    state_length++;
}

/* . */
int
get_state_element_from_table(char *table_name, int position)
{
    char key[MAX_STRING_LENGTH];
    sprintf(key, "%s.%d", table_name, position);
    return get_state_variable(key);
}

/* . */
char *
get_state_string_from_table(char *table_name, int position)
{
    int i = get_state_element_from_table(table_name, position);
    if (i>=0) {
        return state[i].s;
    }
    return "END";
}

/* . */
char *
get_state_int_from_table(char *table_name, int position)
{
    int i = get_state_element_from_table(table_name, position);
    if (i>=0) {
        return state[i].i;
    }
    return -1;
}

/* table_name is stored at global scope in state,
 * so in order to access the 3rd element of table_name="array"
 *
 */
int
load_string_table(toml_table_t* conf, char *table_name)
{
    int table_index = state_length;
    toml_array_t* str_table = toml_array_in(conf, table_name);
    toml_datum_t str;
    for (int i=0; ; i++) {
        str = toml_string_at(str_table, i);
        if (!str.ok) {
            state[table_index].n_leaves = i;
            break;
        }
        else {
            if (i>0) {
                char label[MAX_STRING_LENGTH];
                sprintf(label, "%s.%d", table_name, i);
                add_state_string_variable(label, str.u.s);
            }
            else {
                add_state_string_variable(table_name, str.u.s);
            }
        }
    }

    free(str.u.s);
    return 1;
}

/* . */
int
load_file(char *fname)
{
    int i;
    FILE* file;
    char error_buffer[200];
    toml_table_t *conf;

    file = fopen(fname, "r");
    if (!file) {
        printf("ERROR: Failed to open \"%s\".\n", fname);
        return 0;
    }

    conf = toml_parse_file(file, error_buffer, sizeof(error_buffer));
    fclose(file);

    if (!conf) {
        printf("ERROR: Failed to parse \"%s\".\n", fname);
        printf("    %s\n", error_buffer);
        return 0;
    }

    for (i=0; ; i++) {
        const char *key = toml_key_in(conf, i);
        if (!key) {
            break;
        }
        if (!load_string_table(conf, key)) {
            printf("ERROR: failed to load string table %s\n", key);
            return 0;
        }
    }
    toml_free(conf);
    return 1;
}

/* . */
int
load_data(void)
{
    int i;
    /* load manifest */
    if (!load_file("data/toolbars.toml")) {
        printf("ERROR: failed to load file %s\n", "data/toolbars.toml");
        return 0;
    }
    if (!load_file("data/testing.toml")) {
        return 0;
    }
    if (!load_file("data/commands.toml")) {
        return 0;
    }
    if (!load_file("data/menus.toml")) {
        return 0;
    }
    if (!load_file("data/settings.toml")) {
        return 0;
    }

    /* Confirm load. */
    for (i=0; i<state_length; i++) {
        printf("%d: %s %d\n", i, state[i].label, state[i].type);
    }
    return 1;
}

/* . */
void
read_toml_str(toml_table_t *table, char *label, StringSetting *s)
{
}

/* . */
void
read_toml_int(toml_table_t *table, char *label, IntSetting *s)
{
}

/* . */
void
read_toml_strtable(toml_table_t *table, char *label, StringTableSetting *s)
{
}

/* . */
void
read_toml_bool(toml_table_t *table, char *label, BoolSetting *s)
{
}

/* . */
void
read_toml_real(FILE *file, char *label, RealSetting r)
{
}

/* . */
int
load_settings(char *appDir, char *fname)
{
    return 1;
}

/* . */
void
write_toml_str(FILE *file, char *label, StringSetting s)
{
    fprintf(file, "%s = \"%s\"\r\n", label, s.setting);
}

/* . */
void
write_toml_int(FILE *file, char *label, IntSetting i)
{
    fprintf(file, "%s = %d\r\n", label, i.setting);
}

/* . */
void
write_toml_strtable(FILE *file, char *label, StringTableSetting table)
{
    fprintf(file, "%s = [", label);
    int i;
    for (i=0; i<MAX_FILES; i++) {
        if (!strcmp(table.setting[i], "END")) {
            break;
        }
        fprintf(file, "\"%s\"", table.setting[i]);
        if (strcmp(table.setting[i+1], "END")) {
            fprintf(file, ", ");
        }
    }
    fprintf(file, "]\r\n");
}

/* . */
void
write_toml_bool(FILE *file, char *label, BoolSetting i)
{
    fprintf(file, "%s = %d\r\n", label, i.setting);
}

/* . */
void
write_toml_real(FILE *file, char *label, RealSetting r)
{
    fprintf(file, "%s = %f\r\n", label, r.setting);
}

/* The file fname needs to be read from the users home directory to ensure it is writable.
 */
int
save_settings(const char *appDir, const char *fname)
{
    FILE *file = fopen(fname, "w");
    if (!file) {
        return 0;
    }

    /*
    QSettings settings(settingsPath, QSettings::IniFormat);
    QString tmp;
    //save_settings(qPrintable(appDir), qPrintable(SettingsDir()));

    fprintf(file, "\r\n[Window]\r\n");
    write_toml_bool(file, "Window/Position", pos());
    write_toml_int(file, "PositionX", pos.x);
    write_toml_int(file, "PositionY", pos.y);
    write_toml_bool(file, "Window/Size", size());
    write_toml_int(file, "SizeX", size.x);
    write_toml_int(file, "SizeY", size.y);
    */

    /* General */
    /* write_toml_int(file, "LayoutState", layoutState); */
    write_toml_str(file, "Language", general_language);
    write_toml_str(file, "IconTheme", general_icon_theme);
    write_toml_int(file, "IconSize", general_icon_size);
    write_toml_bool(file, "MdiBGUseLogo", general_mdi_bg_use_logo);
    write_toml_bool(file, "MdiBGUseTexture", general_mdi_bg_use_texture);
    write_toml_bool(file, "MdiBGUseColor", general_mdi_bg_use_color);
    write_toml_str(file, "MdiBGLogo", general_mdi_bg_logo);
    write_toml_str(file, "MdiBGTexture", general_mdi_bg_texture);
    write_toml_int(file, "MdiBGColor", general_mdi_bg_color);
    write_toml_bool(file, "TipOfTheDay", general_tip_of_the_day);
    general_current_tip.setting++;
    write_toml_int(file, "CurrentTip", general_current_tip);
    write_toml_bool(file, "SystemHelpBrowser", general_system_help_browser);

    /* Display */
    fprintf(file, "\r\n[Display]\r\n");
    write_toml_bool(file, "UseOpenGL", display_use_opengl);
    write_toml_bool(file, "RenderHintAntiAlias", display_renderhint_aa);
    write_toml_bool(file, "RenderHintTextAntiAlias", display_renderhint_text_aa);
    write_toml_bool(file, "RenderHintSmoothPixmap", display_renderhint_smooth_pix);
    write_toml_bool(file, "RenderHintHighQualityAntiAlias", display_renderhint_high_aa);
    write_toml_bool(file, "RenderHintNonCosmetic", display_renderhint_noncosmetic);
    write_toml_bool(file, "ShowScrollBars", display_show_scrollbars);
    write_toml_int(file, "ScrollBarWidgetNum", display_scrollbar_widget_num);
    write_toml_int(file, "CrossHairColor", display_crosshair_color);
    write_toml_int(file, "BackgroundColor", display_bg_color);
    write_toml_int(file, "SelectBoxLeftColor", display_selectbox_left_color);
    write_toml_int(file, "SelectBoxLeftFill", display_selectbox_left_fill);
    write_toml_int(file, "SelectBoxRightColor", display_selectbox_right_color);
    write_toml_int(file, "SelectBoxRightFill", display_selectbox_right_fill);
    write_toml_int(file, "SelectBoxAlpha", display_selectbox_alpha);
    write_toml_real(file, "ZoomScaleIn", display_zoomscale_in);
    write_toml_real(file, "ZoomScaleOut", display_zoomscale_out);
    write_toml_int(file, "CrossHairPercent", display_crosshair_percent);
    write_toml_str(file, "Units", display_units);

    /* Prompt */
    fprintf(file, "\r\n[Prompt]\r\n");
    write_toml_int(file, "TextColor", prompt_text_color);
    write_toml_int(file, "BackgroundColor", prompt_bg_color);
    write_toml_str(file, "FontFamily", prompt_font_family);
    write_toml_str(file, "FontStyle", prompt_font_style);
    write_toml_int(file, "FontSize", prompt_font_size);
    write_toml_bool(file, "SaveHistory", prompt_save_history);
    write_toml_bool(file, "SaveHistoryAsHtml", prompt_save_history_as_html);
    write_toml_str(file, "SaveHistoryFilename", prompt_save_history_filename);

    /* OpenSave */
    fprintf(file, "\r\n[OpenSave]\r\n");
    write_toml_str(file, "CustomFilter", opensave_custom_filter);
    write_toml_str(file, "OpenFormat", opensave_open_format);
    write_toml_bool(file, "OpenThumbnail", opensave_open_thumbnail);
    write_toml_str(file, "SaveFormat", opensave_save_format);
    write_toml_bool(file, "SaveThumbnail", opensave_save_thumbnail);

    /* Recent */
    fprintf(file, "\r\n[Recent]\r\n");
    write_toml_int(file, "RecentMax", opensave_recent_max_files);
    write_toml_strtable(file, "RecentFiles", opensave_recent_list_of_files);
    write_toml_str(file, "RecentDirectory", opensave_recent_directory);

    /* Trimming */
    fprintf(file, "\r\n[Trimming]\r\n");
    write_toml_int(file, "TrimDstNumJumps", opensave_trim_dst_num_jumps);

    /* Printing */
    fprintf(file, "\r\n[Printing]\r\n");
    write_toml_str(file, "DefaultDevice", printing_default_device);
    write_toml_bool(file, "UseLastDevice", printing_use_last_device);
    write_toml_bool(file, "DisableBG", printing_disable_bg);

    /* Grid */
    fprintf(file, "\r\n[Grid]\r\n");
    write_toml_bool(file, "ShowOnLoad", grid_show_on_load);
    write_toml_bool(file, "ShowOrigin", grid_show_origin);
    write_toml_bool(file, "ColorMatchCrossHair", grid_color_match_crosshair);
    write_toml_int(file, "Color", grid_color);
    write_toml_bool(file, "LoadFromFile", grid_load_from_file);
    write_toml_str(file, "Type", grid_type);
    write_toml_bool(file, "CenterOnOrigin", grid_center_on_origin);
    write_toml_real(file, "CenterX", grid_center_x);
    write_toml_real(file, "CenterY", grid_center_y);
    write_toml_real(file, "SizeX", grid_size_x);
    write_toml_real(file, "SizeY", grid_size_y);
    write_toml_real(file, "SpacingX", grid_spacing_x);
    write_toml_real(file, "SpacingY", grid_spacing_y);
    write_toml_real(file, "SizeRadius", grid_size_radius);
    write_toml_real(file, "SpacingRadius", grid_spacing_radius);
    write_toml_real(file, "SpacingAngle", grid_spacing_angle);

    /* Ruler */
    fprintf(file, "\r\n[Ruler]\r\n");
    write_toml_bool(file, "ShowOnLoad", ruler_show_on_load);
    write_toml_bool(file, "Metric", ruler_metric);
    write_toml_int(file, "Color", ruler_color);
    write_toml_int(file, "PixelSize", ruler_pixel_size);

    /* Quick Snap */
    fprintf(file, "\r\n[QuickSnap]\r\n");
    write_toml_bool(file, "Enabled", qsnap_enabled);
    write_toml_int(file, "LocatorColor", qsnap_locator_color);
    write_toml_int(file, "LocatorSize", qsnap_locator_size);
    write_toml_int(file, "ApertureSize", qsnap_aperture_size);
    write_toml_bool(file, "EndPoint", qsnap_endpoint);
    write_toml_bool(file, "MidPoint", qsnap_midpoint);
    write_toml_bool(file, "Center", qsnap_center);
    write_toml_bool(file, "Node", qsnap_node);
    write_toml_bool(file, "Quadrant", qsnap_quadrant);
    write_toml_bool(file, "Intersection", qsnap_intersection);
    write_toml_bool(file, "Extension", qsnap_extension);
    write_toml_bool(file, "Insertion", qsnap_insertion);
    write_toml_bool(file, "Perpendicular", qsnap_perpendicular);
    write_toml_bool(file, "Tangent", qsnap_tangent);
    write_toml_bool(file, "Nearest", qsnap_nearest);
    write_toml_bool(file, "Apparent", qsnap_apparent);
    write_toml_bool(file, "Parallel", qsnap_parallel);

    /* LineWeight */
    fprintf(file, "\r\n[LineWeight]\r\n");
    write_toml_bool(file, "ShowLineWeight", lwt_show_lwt);
    write_toml_bool(file, "RealRender", lwt_real_render);
    write_toml_real(file, "DefaultLineWeight", lwt_default_lwt);

    /* Selection */
    fprintf(file, "\r\n[Selection]\r\n");
    write_toml_bool(file, "PickFirst", selection_mode_pickfirst);
    write_toml_bool(file, "PickAdd", selection_mode_pickadd);
    write_toml_bool(file, "PickDrag", selection_mode_pickdrag);
    write_toml_int(file, "CoolGripColor", selection_coolgrip_color);
    write_toml_int(file, "HotGripColor", selection_hotgrip_color);
    write_toml_int(file, "GripSize", selection_grip_size);
    write_toml_int(file, "PickBoxSize", selection_pickbox_size);

    /* Text */
    fprintf(file, "\r\n[Text]\r\n");
    write_toml_str(file, "Font", text_font);
    write_toml_real(file, "Size", text_size);
    write_toml_real(file, "Angle", text_angle);
    write_toml_bool(file, "StyleBold", text_style_bold);
    write_toml_bool(file, "StyleItalic", text_style_italic);
    write_toml_bool(file, "StyleUnderline", text_style_underline);
    write_toml_bool(file, "StyleStrikeOut", text_style_strikeout);
    write_toml_bool(file, "StyleOverline", text_style_overline);

    fclose(file);
    return 1;
}

/* . */
int
get_command_id(const char *name)
{
    for (int i=0; i<MAX_COMMANDS; i++) {
        if (command_data[i].id == -2) {
            break;
        }
        if (!strcmp(command_data[i].icon, name)) {
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
bool pattern_save(EmbPattern *pattern, const char *fileName)
{
    char message[200];
    sprintf(message, "SaveObject save(%s)", fileName);
    prompt_output(message);

    bool writeSuccessful = false;
    int i;

    int formatType = emb_identify_format(fileName);
    if (formatType <= 0) { /* EMBFORMAT_UNSUPPORTED */
        return false;
    }

    if (!pattern) {
        prompt_output("The EmbPattern has not been allocated.");
        return false;
    }

    // TODO: handle EMBFORMAT_STCHANDOBJ also
    if (formatType == EMBFORMAT_STITCHONLY) {
        // emb_pattern_movePolylinesToStitchList(pattern);
        // TODO: handle all objects like this
    }

    writeSuccessful = emb_pattern_write(pattern, fileName, formatType);
    if (!writeSuccessful) {
        sprintf(message, "Writing file %s was unsuccessful", fileName);
        prompt_output(message);
    }

    //TODO: check the embLog for errors and if any exist, report them.

    emb_pattern_free(pattern);

    return writeSuccessful;
}

/* GEOMETRY */

/* . */
EmbVector
quadrant(EmbGeometry geometry, int degrees)
{
    EmbVector v;
    EmbReal radius;
    v.x = 0.0;
    v.y = 0.0;
    switch (geometry.type) {
    case EMB_CIRCLE: {
        v = geometry.object.circle.center;
        radius = geometry.object.circle.radius;
    }
    case EMB_ELLIPSE: {
        v = geometry.object.ellipse.center;
        if (degrees % 180 == 0) {
            radius = geometry.object.ellipse.radius.x;
        }
        else {
            radius = geometry.object.ellipse.radius.y;
        }
    }
    default:
        break;
    }
    double rot = radians(/* rotation() + */ degrees);
    v.x += radius * cos(rot);
    v.y += radius * sin(rot);
    return v;
}

/* . */
void
set_start_angle(EmbGeometry *geometry, double angle)
{
    switch (geometry->type) {
    case EMB_ARC: {
        //TODO: ArcObject setObjectStartAngle
        break;
    }
    default:
        break;
    }
}

/* . */
void
set_end_angle(EmbGeometry *geometry, double angle)
{
    switch (geometry->type) {
    case EMB_ARC: {
        //TODO: ArcObject setObjectEndAngle
        break;
    }
    default:
        break;
    }
}

/* . */
void
set_start_point(EmbGeometry *geometry, EmbVector point)
{
    switch (geometry->type) {
    case EMB_ARC: {
        geometry->object.arc.start = point;
        //calculateData();
        break;
    }
    default:
        break;
    }
}

/* . */
void
set_mid_point(EmbGeometry *geometry, EmbVector point)
{
    switch (geometry->type) {
    case EMB_ARC: {
        geometry->object.arc.mid = point;
        //calculateData();
        break;
    }
    default:
        break;
    }
}

/* . */
void
set_end_point(EmbGeometry *geometry, EmbVector point)
{
    switch (geometry->type) {
    case EMB_ARC: {
        geometry->object.arc.end = point;
        //calculateData();
        break;
    }
    default:
        break;
    }
}

/* . */
void
set_radius(EmbGeometry *geometry, double radius)
{
    switch (geometry->type) {
    case EMB_ARC: {
        geometry->object.arc = emb_arc_set_radius(geometry->object.arc, radius);
        break;
    }
    case EMB_CIRCLE:
        geometry->object.circle.radius = radius;
        break;
    default:
        break;
    }
}

/* . */
void
set_diameter(EmbGeometry *geometry, double diameter)
{
    switch (geometry->type) {
    case EMB_CIRCLE: {
        geometry->object.circle.radius = diameter / 2.0;
        //FIXME: updatePath();
        break;
    }
    default:
        break;
    }
}

/* . */
void
set_area(EmbGeometry *geometry, double area)
{
    switch (geometry->type) {
    case EMB_CIRCLE: {
        double radius = sqrt(area / embConstantPi);
        set_radius(geometry, radius);
        break;
    }
    default:
        break;
    }
}

/* . */
void
set_circumference(EmbGeometry *geometry, double circumference)
{
    switch (geometry->type) {
    case EMB_CIRCLE: {
        double diameter = circumference / embConstantPi;
        set_diameter(geometry, diameter);
        break;
    }
    default:
        break;
    }
}

/* . */
void
set_radius_major(EmbGeometry *geometry, double radius)
{
    set_diameter_major(geometry, radius*2.0);
}

/* . */
void
set_radius_minor(EmbGeometry *geometry, double radius)
{
    set_diameter_minor(geometry, radius*2.0);
}

/* . */
void
set_diameter_major(EmbGeometry *geometry, double diameter)
{
    switch (geometry->type) {
    case EMB_ELLIPSE:
        /* FIXME: Identify longer axis and replace. */
        geometry->object.ellipse.radius.x = diameter;
        break;
    default:
        break;
    }
}

/* . */
void
set_diameter_minor(EmbGeometry *geometry, double diameter)
{
    switch (geometry->type) {
    case EMB_ELLIPSE:
        /* FIXME: Identify longer axis and replace. */
        geometry->object.ellipse.radius.x = diameter;
        break;
    default:
        break;
    }
}

