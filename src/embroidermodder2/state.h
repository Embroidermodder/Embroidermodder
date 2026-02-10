#ifndef EMBROIDERMODDER_STATE_H
#define EMBROIDERMODDER_STATE_H

/*
 * TODO: Convert this file to C99 allowing for a core that only interacts with the state
 * that doesn't use C++.
 */

#include <QString>
#include <QStringList>
#include <QRgb>
#include <QHash>

#include <stdbool.h>
#include <inttypes.h>

#define MAX_ARGS                    10
#define MAX_ARG_LENGTH             500

typedef struct Settings_ {
    QString general_language;
    QString general_icon_theme;
    int general_icon_size;
    bool general_mdi_bg_use_logo;
    bool general_mdi_bg_use_texture;
    bool general_mdi_bg_use_color;
    QString general_mdi_bg_logo;
    QString general_mdi_bg_texture;
    QRgb general_mdi_bg_color;
    bool general_tip_of_the_day;
    uint16_t general_current_tip;
    bool general_system_help_browser;
    bool general_check_for_updates;
    bool display_use_opengl;
    bool display_renderhint_aa;
    bool display_renderhint_text_aa;
    bool display_renderhint_smooth_pix;
    bool display_renderhint_high_aa;
    bool display_renderhint_noncosmetic;
    bool display_show_scrollbars;
    int display_scrollbar_widget_num;
    QRgb display_crosshair_color;
    QRgb display_bg_color;
    QRgb display_selectbox_left_color;
    QRgb display_selectbox_left_fill;
    QRgb display_selectbox_right_color;
    QRgb display_selectbox_right_fill;
    uint8_t display_selectbox_alpha;
    double display_zoomscale_in;
    double display_zoomscale_out;
    uint8_t display_crosshair_percent;
    QString display_units;
    QRgb prompt_text_color;
    QRgb prompt_bg_color;
    QString prompt_font_family;
    QString prompt_font_style;
    uint8_t prompt_font_size;
    bool prompt_save_history;
    bool prompt_save_history_as_html;
    QString prompt_save_history_filename;
    QString opensave_custom_filter;
    QString opensave_open_format;
    bool opensave_open_thumbnail;
    QString opensave_save_format;
    bool opensave_save_thumbnail;
    uint8_t opensave_recent_max_files;
    QStringList opensave_recent_list_of_files;
    QString opensave_recent_directory;
    uint8_t opensave_trim_dst_num_jumps;
    QString printing_default_device;
    bool printing_use_last_device;
    bool printing_disable_bg;
    bool grid_show_on_load;
    bool grid_show_origin;
    bool grid_color_match_crosshair;
    QRgb grid_color;
    bool grid_load_from_file;
    QString grid_type;
    bool grid_center_on_origin;
    double grid_center_x;
    double grid_center_y;
    double grid_size_x;
    double grid_size_y;
    double grid_spacing_x;
    double grid_spacing_y;
    double grid_size_radius;
    double grid_spacing_radius;
    double grid_spacing_angle;
    bool ruler_show_on_load;
    bool ruler_metric;
    QRgb ruler_color;
    uint8_t ruler_pixel_size;
    bool qsnap_enabled;
    QRgb qsnap_locator_color;
    uint8_t qsnap_locator_size;
    uint8_t qsnap_aperture_size;
    bool qsnap_endpoint;
    bool qsnap_midpoint;
    bool qsnap_center;
    bool qsnap_node;
    bool qsnap_quadrant;
    bool qsnap_intersection;
    bool qsnap_extension;
    bool qsnap_insertion;
    bool qsnap_perpendicular;
    bool qsnap_tangent;
    bool qsnap_nearest;
    bool qsnap_apparent;
    bool qsnap_parallel;
    bool lwt_show_lwt;
    bool lwt_real_render;
    double lwt_default_lwt;
    bool selection_mode_pickfirst;
    bool selection_mode_pickadd;
    bool selection_mode_pickdrag;
    QRgb selection_coolgrip_color;
    QRgb selection_hotgrip_color;
    uint8_t selection_grip_size;
    uint8_t selection_pickbox_size;
    QString text_font;
    double text_size;
    double text_angle;
    bool text_style_bold;
    bool text_style_italic;
    bool text_style_underline;
    bool text_style_overline;
    bool text_style_strikeout;
} Settings;

typedef struct State_ {
    Settings settings;
    QHash<QString, QList<QString>> tables;

    char arguments[MAX_ARGS][MAX_ARG_LENGTH];
    int argument_count;
} State;

extern State state;

#endif
