#ifndef SCRIPT_H
#define SCRIPT_H

#include <QString>
#include <QRgb>

#define CONTEXT_MAIN                   0
#define CONTEXT_CLICK                  1
#define CONTEXT_CONTEXT                2
#define CONTEXT_MOVE                   3
#define CONTEXT_PROMPT                 4

void run(const char *command);

extern QString settings_general_language;
extern QString settings_general_icon_theme;
extern int settings_general_icon_size;
extern bool settings_general_mdi_bg_use_logo;
extern bool settings_general_mdi_bg_use_texture;
extern bool settings_general_mdi_bg_use_color;
extern QString settings_general_mdi_bg_logo;
extern QString settings_general_mdi_bg_texture;
extern QRgb settings_general_mdi_bg_color;
extern bool settings_general_tip_of_the_day;
extern quint16 settings_general_current_tip;
extern bool settings_general_system_help_browser;
extern bool settings_general_check_for_updates;
extern bool settings_display_use_opengl;
extern bool settings_display_renderhint_aa;
extern bool settings_display_renderhint_text_aa;
extern bool settings_display_renderhint_smooth_pix;
extern bool settings_display_renderhint_high_aa;
extern bool settings_display_renderhint_noncosmetic;
extern bool settings_display_show_scrollbars;
extern int settings_display_scrollbar_widget_num;
extern QRgb settings_display_crosshair_color;
extern QRgb settings_display_bg_color;
extern QRgb settings_display_selectbox_left_color;
extern QRgb settings_display_selectbox_left_fill;
extern QRgb settings_display_selectbox_right_color;
extern QRgb settings_display_selectbox_right_fill;
extern quint8 settings_display_selectbox_alpha;
extern qreal settings_display_zoomscale_in;
extern qreal settings_display_zoomscale_out;
extern quint8 settings_display_crosshair_percent;
extern QString settings_display_units;
extern QRgb settings_prompt_text_color;
extern QRgb settings_prompt_bg_color;
extern QString settings_prompt_font_family;
extern QString settings_prompt_font_style;
extern quint8 settings_prompt_font_size;
extern bool settings_prompt_save_history;
extern bool settings_prompt_save_history_as_html;
extern QString settings_prompt_save_history_filename;
extern QString settings_opensave_custom_filter;
extern QString settings_opensave_open_format;
extern bool settings_opensave_open_thumbnail;
extern QString settings_opensave_save_format;
extern bool settings_opensave_save_thumbnail;
extern quint8 settings_opensave_recent_max_files;
extern QStringList settings_opensave_recent_list_of_files;
extern QString settings_opensave_recent_directory;
extern quint8 settings_opensave_trim_dst_num_jumps;
extern QString settings_printing_default_device;
extern bool settings_printing_use_last_device;
extern bool settings_printing_disable_bg;
extern bool settings_grid_show_on_load;
extern bool settings_grid_show_origin;
extern bool settings_grid_color_match_crosshair;
extern QRgb settings_grid_color;
extern bool settings_grid_load_from_file;
extern QString settings_grid_type;
extern bool settings_grid_center_on_origin;
extern qreal settings_grid_center_x;
extern qreal settings_grid_center_y;
extern qreal settings_grid_size_x;
extern qreal settings_grid_size_y;
extern qreal settings_grid_spacing_x;
extern qreal settings_grid_spacing_y;
extern qreal settings_grid_size_radius;
extern qreal settings_grid_spacing_radius;
extern qreal settings_grid_spacing_angle;
extern bool settings_ruler_show_on_load;
extern bool settings_ruler_metric;
extern QRgb settings_ruler_color;
extern quint8 settings_ruler_pixel_size;
extern bool settings_qsnap_enabled;
extern QRgb settings_qsnap_locator_color;
extern quint8 settings_qsnap_locator_size;
extern quint8 settings_qsnap_aperture_size;
extern bool settings_qsnap_endpoint;
extern bool settings_qsnap_midpoint;
extern bool settings_qsnap_center;
extern bool settings_qsnap_node;
extern bool settings_qsnap_quadrant;
extern bool settings_qsnap_intersection;
extern bool settings_qsnap_extension;
extern bool settings_qsnap_insertion;
extern bool settings_qsnap_perpendicular;
extern bool settings_qsnap_tangent;
extern bool settings_qsnap_nearest;
extern bool settings_qsnap_apparent;
extern bool settings_qsnap_parallel;
extern bool settings_lwt_show_lwt;
extern bool settings_lwt_real_render;
extern qreal settings_lwt_default_lwt;
extern bool settings_selection_mode_pickfirst;
extern bool settings_selection_mode_pickadd;
extern bool settings_selection_mode_pickdrag;
extern QRgb settings_selection_coolgrip_color;
extern QRgb settings_selection_hotgrip_color;
extern quint8 settings_selection_grip_size;
extern quint8 settings_selection_pickbox_size;
extern QString settings_text_font;
extern qreal settings_text_size;
extern qreal settings_text_angle;
extern bool settings_text_style_bold;
extern bool settings_text_style_italic;
extern bool settings_text_style_underline;
extern bool settings_text_style_overline;
extern bool settings_text_style_strikeout;

#endif

