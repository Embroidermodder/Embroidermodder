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
        if (!strcmp(token, "..")) {
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
