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

#include "em2.h"

/*  DATA SECTION
 *  The data for configuring Embroidermodder 2,
 *  sets the default values should it not be present
 *  in configuration.
 */
TABLE(global_state);

EmbColor white_color = {255, 255, 255};

#if _WIN32
const char *os_seperator = "\\";
#else
/* This is is a guess since we don't know the system here. */
const char *os_seperator = "/";
#endif

#if ANDROID_VERSION
const char *em2_version = "Android version";
#elif IOS_VERSION
const char *em2_version = "iOS version";
#elif x11_version
const char *em2_version = "X11 version";
#elif __APPLE__
const char *em2_version = "Mac OS Cocoa version";
#elif _WIN32
const char *em2_version = "Win32 version";
#else
const char *em2_version = "SDL2 version";
#endif

int active_view = 0;
int active_scene = 0;
float dialog_qsnap_locator_size = 1.0;
float dialog_qsnap_aperture_size = 1.0;
float qsnap_aperture_size = 1.0;
int n_tips = 15;

/* Assets */
TABLE(tips);
TABLE(crash_test_script);
TABLE(tooltips);
TABLE(action_list);
TABLE(settings_state);
TABLE(shortcuts);

float max_distance = 1000000.0;
int general_icon_size = 16;

int dialog_grid_load_from_file = 1;

int n_windows = 0;

EmbColor black = {0x00, 0x00, 0x00};
EmbColor background_color = {0x44, 0x66, 0x88};
EmbColor interface_color = {0x66, 0x88, 0xAA};
EmbColor ruler_color = {0xFF, 0xFF, 0xFF};
EmbColor preview_ruler_color = {0xFF, 0xFF, 0xFF};
EmbColor qsnap_locator_color = {0xFF, 0xFF, 0xFF};

/* Grip settings. */
EmbColor selection_coolgrip_color = {0xFF, 0xFF, 0xFF};
EmbColor selection_hotgrip_color = {0xFF, 0xFF, 0xFF};

EmbColor button_background_color = {0x88, 0xAA, 0xFF};

EmbColor grip_color_cool = {0x00, 0x00, 0x00};
EmbColor grip_color_hot = {0x00, 0x00, 0x00};
EmbColor crosshair_color = {0x00, 0x00, 0x00};

int color_mode = 0;
int grid_type = RECTANGULAR_GRID;
int mdi_window = 0;

EmbVector paste_delta = {0.0, 0.0};
EmbVector scene_press_point = {0.0, 0.0};
EmbVector press_point = {0.0, 0.0};
EmbVector scene_move_point = {0.0, 0.0};
EmbVector move_point = {0.0, 0.0};
EmbVector scene_release_point = {0.0, 0.0};
EmbVector release_point = {0.0, 0.0};
EmbVector scene_grip_point = {0.0, 0.0};
EmbVector view_mouse_point = {0.0, 0.0};
EmbVector scene_mouse_point = {0.0, 0.0};

int precision_angle = 0;
int precision_length = 0;

int statusBarMouseCoord = 0;

/* Used when checking if fields vary */
int field_old_text = 0;
int field_new_text = 0;
int field_varies_text = 0;
int field_yes_text = 0;
int field_no_text = 0;
int field_on_text = 0;
int field_off_text = 0;

/* General settings */
char general_language[MAX_STRING_LENGTH] = "default";
char general_icon_theme[MAX_STRING_LENGTH] = "default";
char general_mdi_bg_logo[MAX_STRING_LENGTH] = "assets/icons/logo.png";
char general_mdi_bg_texture[MAX_STRING_LENGTH] = "assets/icons/texture.png";
EmbColor general_mdi_bg_color = {0xFF, 0xFF, 0xFF};
int general_current_tip = 0;
int general_tip_of_the_day = 0;
int general_system_help_browser = 1;
int general_check_for_updates = 0;
int general_mdi_bg_use_logo = 0;
int general_mdi_bg_use_texture = 0;
int general_mdi_bg_use_color = 0;

int preview_mdi_bg_use_logo = 0;
int preview_mdi_bg_use_texture = 0;
int preview_mdi_bg_use_color = 0;

unsigned char ttf_buffer[1<<25];

/* QSnap Properties */
int qsnap_endpoint = 0;
int qsnap_midpoint = 0;
int qsnap_center = 0;
int qsnap_node = 0;
int qsnap_quadrant = 0;
int qsnap_intersection = 0;
int qsnap_extension = 0;
int qsnap_insertion = 0;
int qsnap_perpendicular = 0;
int qsnap_tangent = 0;
int qsnap_nearest = 0;
int qsnap_apparent = 0;
int qsnap_parallel = 0;
int qsnap_enabled = 0;
float qsnap_locator_size = 1.0;
int qsnap_active = 0;
int qsnap_toggle = 0;

/* Line Weight Settings */
int lwt_show_lwt = 0;
int preview_lwt_show_lwt = 0;
int lwt_real_render = 0;
float lwt_default_lwt = 1.0;

int preview_lwt_real_render = 0;

char current_file_name[MAX_STRING_LENGTH];
int preview_mode = PREVIEW_MODE_OFF;

/* Ruler Settings */
int ruler_metric = 1;
int ruler_show_on_load = 1;
int ruler_pixel_size = 30;

/* Grid Settings */
float grid_size_radius = 10.0;
float grid_size_angle = 360.0;
int grid_center_on_origin = 0;
EmbVector grid_center = {0.5, 0.5};
EmbVector grid_size = {10.0, 10.0};
EmbVector grid_spacing = {10.0, 10.0};
float grid_spacing_radius = 10.0;
float grid_spacing_angle = 10.0;
int grid_show_on_load = 0;
int grid_show_origin = 1;
int grid_color_match_crosshair = 1;
int grid_load_from_file = 1;
EmbColor grid_color = {0x00, 0x00, 0x00};
EmbColor preview_grid_color = {0x00, 0x00, 0x00};

float symbol_scale = 0.01;

/* Display Settings */
int display_use_opengl = 1;
int display_renderhint_aa = 1;
int display_renderhint_text_aa = 1;
int display_renderhint_smooth_pix = 1;
int display_renderhint_high_aa = 1;
int display_renderhint_noncosmetic = 1;
int display_show_scrollbars = 1;
int display_scrollbar_widget_num = 1;
int display_zoomscale_in = 1;
int display_zoomscale_out = 1;
int display_selectbox_alpha = 1;
int preview_display_selectbox_alpha = 1;
int display_crosshair_percent = 10;

int preview_show_scrollbars = 1;
float display_zoom_scale_action_in = 0.0;
float display_zoom_scale_action_out = 0.0;
float zoom_in_limit = 0.00001;
float oom_out_limit = 10000.0;

/* Open/Save Settings */
char opensave_open_format[MAX_STRING_LENGTH] = "csv";
char opensave_save_format[MAX_STRING_LENGTH] = "csv";
char opensave_recent_directory[MAX_STRING_LENGTH] = "~";
char opensave_recent_files[MAX_RECENT][MAX_STRING_LENGTH] = {
    "new-file.csv"
};
char opensave_open_thumbnail[MAX_STRING_LENGTH] = "assets/icon/open_file_16.png";
char opensave_save_thumbnail[MAX_STRING_LENGTH] = "assets/icon/save_file_16.png";
int opensave_recent_max_files = 10;
int opensave_trim_dst_num_jumps = 6;

/* Printing Settings */
int printing_use_last_device = 0;
int printing_disable_bg = 0;

/* Selection Settings */
int selection_mode_pickfirst = 0;
int selection_mode_pickadd = 0;
int selection_mode_pickdrag = 0;
int selection_grip_size = 16;
int selection_pickbox_size = 16;

/* Panning Settings */
int panning_real_time_active = 0;
int panning_point_active = 0;
int panning_active = 0;

/* The Color class definition. */
int stitches_total;
int stitches_real;
int stitches_jump;
int stitches_trim;
int color_total;
int color_changes;

/* Display Settings */
EmbColor display_crosshair_color = {0x0, 0x0, 0x0};
EmbColor display_bg_color = {0xFF, 0xFF, 0xFF};
EmbColor display_selectbox_left_color = {0xFF, 0xFF, 0xFF};
EmbColor display_selectbox_left_fill = {0xFF, 0xFF, 0xFF};
EmbColor display_selectbox_right_color = {0xFF, 0xFF, 0xFF};
EmbColor display_selectbox_right_fill = {0xFF, 0xFF, 0xFF};

EmbColor preview_display_crosshair_color = {0x0, 0x0, 0x0};
EmbColor preview_display_bg_color = {0xFF, 0xFF, 0xFF};
EmbColor preview_display_selectbox_left_color = {0xFF, 0xFF, 0xFF};
EmbColor preview_display_selectbox_left_fill = {0xFF, 0xFF, 0xFF};
EmbColor preview_display_selectbox_right_color = {0xFF, 0xFF, 0xFF};
EmbColor preview_display_selectbox_right_fill = {0xFF, 0xFF, 0xFF};

int display_units = SCIENTIFIC;

int file_toolbar_visible = 1;
int edit_toolbar_visible = 1;
int view_toolbar_visible = 1;
int window_toolbar_visible = 1;

void load_csv(TABLE(table), char *fname);

int language = LANGUAGE_DEFAULT;
TABLE(translation_tables[N_LANGUAGES]);

char preview_modes[][MAX_STRING_LENGTH] = {
    "null",
    "move",
    "rotate",
    "scale"
};

TABLE(comboboxes);
TABLE(settings_tabs_data);
TABLE(obj_names);

char error_msg[MAX_STRING_LENGTH] = "None";
char uname_string[MAX_STRING_LENGTH] = "None";
char current_file_name[MAX_STRING_LENGTH] = "None";
char details_labels[MAX_STRING_LENGTH] = "None";

char general_language[MAX_STRING_LENGTH];
char general_icon_theme[MAX_STRING_LENGTH];
char general_mdi_bg_logo[MAX_STRING_LENGTH];
char general_mdi_bg_texture[MAX_STRING_LENGTH];
char opensave_open_format[MAX_STRING_LENGTH];
char opensave_save_format[MAX_STRING_LENGTH];
char opensave_recent_directory[MAX_STRING_LENGTH];
char printing_default_device[MAX_STRING_LENGTH];
float qsnap_locator_size;
float qsnap_aperture_size;
text_properties text_style = {16.0, 0.0, 0, 0, 0, 0, 0, 0, 0};

int qsnap_mode[20];
int state[20];
int toggle_status[20];
char prefix[100];
int shift_key_pressed_state;

/* Should be a ring buffer. */
TABLE(undo_history);
int undo_history_position = 0;

int text_cursor[2] = {0, 0};
TABLE(text_display);

char details_label_text[12][MAX_STRING_LENGTH];

int dialog_setting_int[100];
double dialog_setting_double[100];

TABLE(settings);
TABLE(preview);
TABLE(dialog);
TABLE(accepted_dialog);
char opensave_custom_filter[MAX_STRING_LENGTH];
char current_fname[MAX_STRING_LENGTH];
int line_n;

/* Program state */
int running = 1;
int debug_mode = 1;
int testing = 0;
int gripping_active = 0;
int preview_active = 0;
int moving_active = 0;
int rapid_move_active = 0;
int pasting_active = 0;
int selecting_active = 0;
int zoom_window_active = 0;
int shift_key_pressed_state = 0;
int track_mode = 0;
int n_patterns = 0;
int n_docs = 0;

/* Should be per view */
int show_grid = 1;
int show_lwt = 1;
int show_ortho = 1;
int show_ruler = 1;
int show_polar = 0;
int snap_mode = 0;
int real_render = 0;

/* Layout data */
int grip_size = 16;
int pick_box_size = 40;
int crosshair_size = 16;
int icon_size = 24;
int icon_padding = 4;
int toolbar_padding = 4;
int menu_item_height = 20;
int menubar_height = 24;
int menubar_padding = 2;
int menu_state = -1;

char statusbar_message[MAX_STRING_LENGTH]
    = "Embroidermodder 2.0.0-alpha";

EmbColor clear_color = {50, 50, 50};
EmbColor toolbar_bg_color = {150, 150, 200};
EmbColor bg_color = {100, 150, 210};
EmbColor menubar_color = {200, 200, 250};
EmbColor white = {255, 255, 255};

int selected_items[MAX_SELECTED];
int n_selected = 0;

TABLE(symbols_docstring);
    
/*
 *  Property box contents.
 */
TABLE(general_properties);

TABLE(arc_properties);
TABLE(block_properties);
TABLE(circle_properties);
TABLE(ellipse_properties);
TABLE(image_properties);
TABLE(infinite_line_properties);
TABLE(line_properties);
TABLE(rect_properties);
TABLE(polygon_properties);
TABLE(point_properties);
TABLE(general_properties);
TABLE(path_properties);
TABLE(polyline_properties);
TABLE(infinite_line_properties);
TABLE(path_properties);
TABLE(polyline_properties);
TABLE(ray_properties);
TABLE(text_single_properties);
TABLE(text_multi_properties);

float dialog_display_zoom_scale_action_in = 1.0;
float dialog_display_zoom_scale_action_out = 1.0;
int dialog_printing_disable_bg = 0;

EmbColor preview_display_selectbox_color_left;
EmbColor preview_display_selectbox_fill_left;
EmbColor preview_display_selectbox_color_right;
EmbColor preview_display_selectbox_fill_right;
int preview_display_selectbox_alpha;

EmbColor accept_display_selectbox_color_left;
EmbColor accept_display_selectbox_fill_left;
EmbColor accept_display_selectbox_color_right;
EmbColor accept_display_selectbox_fill_right;
int accept_display_selectbox_alpha;

void
load_state(EmbWindow *window)
{
    load_csv(global_state, "assets/global_state.csv");

    load_csv(tips, "assets/tips.csv");
    load_csv(crash_test_script, "assets/crash_test.csv");

    load_csv(tooltips, "assets/tooltips.csv");
    load_csv(action_list, "assets/action_list.csv");

    load_translations();

    load_csv(arc_properties, "assets/objects/arc.csv");
    load_csv(circle_properties, "assets/objects/circle.csv");
}

void
load_csv(TABLE(table), char *fname)
{
    int i, j;
    FILE *f;
    f = fopen(fname, "r");
    if (!f) {
        printf("Failed to read csv file: could not open file %s for reading.\n", fname);
        return;
    }

    for (i=0; i<MAX_CSV_ROWS; i++)
    for (j=0; j<MAX_CSV_COLUMNS; j++) {
        table[i][j][0] = 0;
    }

    int line_pos = 0;
    int row = 0;
    int column = 0;
    int escaped = 0;
    int inside_quote = 0;
    while (!feof(f)) {
        char c;
        fread(&c, 1, 1, f);
        /* Allows for quote symbols within quoted strings. */
        if (c=='\\') {
            escaped = 1;
            continue;
        }
        if (!escaped) {
            if (c=='"') {
                inside_quote = !inside_quote;
                continue;
            }
        }
        /* This isn't unicode compliant. If '\n', ','
         * or '"' occur as part of a multi-byte character
         * this breaks.
         */
        if (!inside_quote) {
            if (c==',') {
                table[row][column][line_pos] = 0;
                column++;
                line_pos = 0;
                continue;
            }
            if (c=='\n') {
                table[row][column][line_pos] = 0;
                row++;
                column = 0;
                line_pos = 0;
                continue;        
            }
        }
        table[row][column][line_pos] = c;
        line_pos++;
        escaped = 0;
    }
    fclose(f);

    /* Terminator symbol */
    for (i=0; i<MAX_CSV_COLUMNS; i++) {
        strcpy(table[row+1][i], "END");
    }
}

void
print_table(TABLE(table))
{
    int i, j;
    for (i=0; i<MAX_CSV_ROWS; i++) {
        if (!strcmp(table[i][0], "END")) {
            break;
        }
        for (j=0; j<MAX_CSV_COLUMNS; j++) {
            printf("\"%s\",", table[i][j]);
        }
        printf("\n");
    }
    printf("\n");
}

void
load_translations(EmbWindow *window)
{
    int i;
    TABLE(language_labels);
    load_csv(language_labels, "assets/languages.csv");

    for (i=0; i<N_LANGUAGES; i++) {
        /* TODO: not sure what this compiler warning is on about */
        load_csv(translation_tables[i], language_labels[i][0]);
    }
}

EmbColor
get_color(TABLE(state), char *key)
{
    EmbColor color;
    char *s = get_str(state, key);
    color.r = 16*char_to_int(s[0]) + char_to_int(s[1]);
    color.g = 16*char_to_int(s[2]) + char_to_int(s[3]);
    color.b = 16*char_to_int(s[4]) + char_to_int(s[5]);
    return color;
}

int
char_to_int(char a)
{
    if (a >= '0' && a <= '9') {
        return a-'0';
    }
    if (a >= 'A' && a <= 'F') {
        return a-'A'+10;
    }
    if (a >= 'a' && a <= 'f') {
        return a-'a'+10;
    }
    return 0;
}

char *
get_str(TABLE(state), char *key)
{
    int i;
    for (i=0; i<MAX_CSV_ROWS; i++) {
        if (!strcmp(state[i][0], key)) {
            return state[i][1];
        }
    }
    printf("Failed to find the variable: %s.\n", key);
    return "None";
}

int
get_int(TABLE(state), char *key)
{
    return atoi(get_str(state, key));
}

float
get_float(TABLE(state), char *key)
{
    return atof(get_str(state, key));
}

EmbVector
get_vector(TABLE(state), char *key)
{
    EmbVector v;
    char *value = get_str(state, key);
    v.x = atof(strtok(value, " "));
    v.y = atof(strtok(value, " "));
    return v;
}

int
load_to_buffer(EmbWindow *window)
{
    char buffer[4096];
    size_t i, j, length;
    FILE *fin;
    fin = fopen(current_fname, "r");
    if (!fin) {
        return 1;
    }
    fseek(fin, 0, SEEK_END);
    length = ftell(fin);
    fseek(fin, 0, SEEK_SET);
    if (fread(buffer, 1, length, fin) != length) {
        fclose(fin);
        return 1;
    }
    fclose(fin);

    for (i=0; i<100; i++) {
        text_display[i][0][0] = 0;
    }

    j = 0;
    for (i=0; i<length; i++) {
        buffer[j] = buffer[i];
        if (buffer[i] != '\r') {
            j++;
        }
    }
    buffer[j] = 0;

    line_n = 0;
    for (i=0; i<length; i++) {
        for (j=0; j<100; j++) {
            if (buffer[i+j] == '\n' || !buffer[i+j]) {
                break;
            }
        }
        strncpy(text_display[line_n][0], buffer+i, j);
        text_display[line_n][j][0] = 0;
        line_n++;
        i += j;
    }

    return 0;
}


int
save_from_buffer(EmbWindow *window)
{
    int i;
    FILE *fin;
    fin = fopen(current_fname, "w");
    if (!fin) {
        return 1;
    }
    for (i=0; i<line_n; i++) {
        fprintf(fin, "%s\r\n", text_display[i][0]);
    }

    fclose(fin);

    return 0;
}


