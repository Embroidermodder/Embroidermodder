/*
 * TODO: Convert this to C.
 */

#include "state.h"

/* FIXME: Switch over to the returning a pointer convention. (See the style section of CONTRIBUTING.md) */
void
settings_create(Settings *settings)
{
     
}

/* Make sure that all the settings are within acceptable parameters. */
void
settings_validate(Settings *settings)
{
    settings->general_language = settings->general_language.toLower();
    if (settings->grid_color_match_crosshair) {
        settings->grid_color = settings->display_crosshair_color;
    }
}

/* Please keep this in the same order as the struct itself. */
void
settings_copy(Settings *dest, Settings *src)
{
    /* We only want to copy valid settings, so validate them first. */
    settings_validate(src);

    dest->general_language = src->general_language;
    dest->general_icon_theme = src->general_icon_theme;
    dest->general_icon_size = src->general_icon_size;
    dest->general_mdi_bg_use_logo = src->general_mdi_bg_use_logo;
    dest->general_mdi_bg_use_texture = src->general_mdi_bg_use_texture;
    dest->general_mdi_bg_use_color = src->general_mdi_bg_use_color;
    dest->general_mdi_bg_logo = src->general_mdi_bg_logo;
    dest->general_mdi_bg_texture = src->general_mdi_bg_texture;
    dest->general_mdi_bg_color = src->general_mdi_bg_color;
    dest->general_tip_of_the_day = src->general_tip_of_the_day;
    dest->general_current_tip = src->general_current_tip;
    dest->general_system_help_browser = src->general_system_help_browser;
    dest->general_check_for_updates = src->general_check_for_updates;
    dest->display_use_opengl = src->display_use_opengl;
    dest->display_renderhint_aa = src->display_renderhint_aa;
    dest->display_renderhint_text_aa = src->display_renderhint_text_aa;
    dest->display_renderhint_smooth_pix = src->display_renderhint_smooth_pix;
    dest->display_renderhint_high_aa = src->display_renderhint_high_aa;
    dest->display_renderhint_noncosmetic = src->display_renderhint_noncosmetic;
    dest->display_show_scrollbars = src->display_show_scrollbars;
    dest->display_scrollbar_widget_num = src->display_scrollbar_widget_num;
    dest->display_crosshair_color = src->display_crosshair_color;
    dest->display_bg_color = src->display_bg_color;
    dest->display_selectbox_left_color = src->display_selectbox_left_color;
    dest->display_selectbox_left_fill = src->display_selectbox_left_fill;
    dest->display_selectbox_right_color = src->display_selectbox_right_color;
    dest->display_selectbox_right_fill = src->display_selectbox_right_fill;
    dest->display_selectbox_alpha = src->display_selectbox_alpha;
    dest->display_zoomscale_in = src->display_zoomscale_in;
    dest->display_zoomscale_out = src->display_zoomscale_out;
    dest->display_crosshair_percent = src->display_crosshair_percent;
    dest->display_units = src->display_units;
    dest->prompt_text_color = src->prompt_text_color;
    dest->prompt_bg_color = src->prompt_bg_color;
    dest->prompt_font_family = src->prompt_font_family;
    dest->prompt_font_style = src->prompt_font_style;
    dest->prompt_font_size = src->prompt_font_size;
    dest->prompt_save_history = src->prompt_save_history;
    dest->prompt_save_history_as_html = src->prompt_save_history_as_html;
    dest->prompt_save_history_filename = src->prompt_save_history_filename;
    dest->opensave_custom_filter = src->opensave_custom_filter;
    dest->opensave_open_format = src->opensave_open_format;
    dest->opensave_open_thumbnail = src->opensave_open_thumbnail;
    dest->opensave_save_format = src->opensave_save_format;
    dest->opensave_save_thumbnail = src->opensave_save_thumbnail;
    dest->opensave_recent_max_files = src->opensave_recent_max_files;
    dest->opensave_recent_list_of_files = src->opensave_recent_list_of_files;
    dest->opensave_recent_directory = src->opensave_recent_directory;
    dest->opensave_trim_dst_num_jumps = src->opensave_trim_dst_num_jumps;
    dest->printing_default_device = src->printing_default_device;
    dest->printing_use_last_device = src->printing_use_last_device;
    dest->printing_disable_bg = src->printing_disable_bg;
    dest->grid_show_on_load = src->grid_show_on_load;
    dest->grid_show_origin = src->grid_show_origin;
    dest->grid_color_match_crosshair = src->grid_color_match_crosshair;
    dest->grid_color = src->grid_color;
    dest->grid_load_from_file = src->grid_load_from_file;
    dest->grid_type = src->grid_type;
    dest->grid_center_on_origin = src->grid_center_on_origin;
    dest->grid_center_x = src->grid_center_x;
    dest->grid_center_y = src->grid_center_y;
    dest->grid_size_x = src->grid_size_x;
    dest->grid_size_y = src->grid_size_y;
    dest->grid_spacing_x = src->grid_spacing_x;
    dest->grid_spacing_y = src->grid_spacing_y;
    dest->grid_size_radius = src->grid_size_radius;
    dest->grid_spacing_radius = src->grid_spacing_radius;
    dest->grid_spacing_angle = src->grid_spacing_angle;
    dest->ruler_show_on_load = src->ruler_show_on_load;
    dest->ruler_metric = src->ruler_metric;
    dest->ruler_color = src->ruler_color;
    dest->ruler_pixel_size = src->ruler_pixel_size;
    dest->qsnap_enabled = src->qsnap_enabled;
    dest->qsnap_locator_color = src->qsnap_locator_color;
    dest->qsnap_locator_size = src->qsnap_locator_size;
    dest->qsnap_aperture_size = src->qsnap_aperture_size;
    dest->qsnap_endpoint = src->qsnap_endpoint;
    dest->qsnap_midpoint = src->qsnap_midpoint;
    dest->qsnap_center = src->qsnap_center;
    dest->qsnap_node = src->qsnap_node;
    dest->qsnap_quadrant = src->qsnap_quadrant;
    dest->qsnap_intersection = src->qsnap_intersection;
    dest->qsnap_extension = src->qsnap_extension;
    dest->qsnap_insertion = src->qsnap_insertion;
    dest->qsnap_perpendicular = src->qsnap_perpendicular;
    dest->qsnap_tangent = src->qsnap_tangent;
    dest->qsnap_nearest = src->qsnap_nearest;
    dest->qsnap_apparent = src->qsnap_apparent;
    dest->qsnap_parallel = src->qsnap_parallel;
    dest->lwt_show_lwt = src->lwt_show_lwt;
    dest->lwt_real_render = src->lwt_real_render;
    dest->lwt_default_lwt = src->lwt_default_lwt;
    dest->selection_mode_pickfirst = src->selection_mode_pickfirst;
    dest->selection_mode_pickadd = src->selection_mode_pickadd;
    dest->selection_mode_pickdrag = src->selection_mode_pickdrag;
    dest->selection_coolgrip_color = src->selection_coolgrip_color;
    dest->selection_hotgrip_color = src->selection_hotgrip_color;
    dest->selection_grip_size = src->selection_grip_size;
    dest->selection_pickbox_size = src->selection_pickbox_size;
    dest->text_font = src->text_font;
    dest->text_size = src->text_size;
    dest->text_angle = src->text_angle;
    dest->text_style_bold = src->text_style_bold;
    dest->text_style_italic = src->text_style_italic;
    dest->text_style_underline = src->text_style_underline;
    dest->text_style_overline = src->text_style_overline;
    dest->text_style_strikeout = src->text_style_strikeout;
}

/* FIXME: This is currently unused */
void
settings_free(Settings *settings)
{

}

