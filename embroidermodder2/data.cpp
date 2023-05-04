/*
 *  Embroidermodder 2.
 *
 *  ------------------------------------------------------------
 *
 *  Copyright 2013-2023 The Embroidermodder Team
 *  Embroidermodder 2 is Open Source Software.
 *  See LICENSE for licensing terms.
 *
 *  ------------------------------------------------------------
 *
 *  Use Python's PEP7 style guide.
 *      https://peps.python.org/pep-0007/
 */

/**
 * \file data.cpp For all global data.
 */

#include "embroidermodder.h"

std::unordered_map<std::string, QGroupBox *> groupBoxes;
std::unordered_map<std::string, QComboBox *> comboBoxes;
std::unordered_map<std::string, QLineEdit *> lineEdits;
std::unordered_map<std::string, QToolButton *> toolButtons;
QFontComboBox* comboBoxTextSingleFont;

std::vector<std::string> group_box_list = {
    "general",
    "geometry_arc",
    "misc_arc",
    "geometry_block",
    "geometry_circle",
    "geometry_dim_aligned",
    "geometry_dim_angular",
    "geometry_dim_arc_length",
    "geometry_dim_diameter",
    "geometry_dim_leader",
    "geometry_dim_linear",
    "geometry_dim_ordinate",
    "geometry_dim_radius",
    "geometry_ellipse",
    "geometry_image",
    "misc_image",
    "geometry_infinite_line",
    "geometry_line",
    "geometry_path",
    "misc_path",
    "geometry_point",
    "geometry_polygon",
    "geometry_polyline",
    "misc_polyline",
    "geometry_ray",
    "geometry_rectangle",
    "geometry_text_multi",
    "text_text_single",
    "geometry_text_single",
    "misc_text_single"
};

GroupBoxData group_box_arc_geometry[] = {
    {
        .object = OBJ_TYPE_ARC,
        .key = "arc_center_x",
        .icon_name = "blank",
        .label = "Center X",
        .type = "double",
        .map_signal = "lineEditArcCenterX"
    },
    {
        .object = OBJ_TYPE_ARC,
        .key = "arc_center_y",
        .icon_name = "blank",
        .label = "Center Y",
        .type = "double",
        .map_signal = "lineEditArcCenterY"
    },
    {
        .object = OBJ_TYPE_ARC,
        .key = "arc_radius",
        .icon_name = "blank",
        .label = "Radius",
        .type = "double",
        .map_signal = "lineEditArcRadius"
    },
    {
        .object = OBJ_TYPE_ARC,
        .key = "arc_start_angle",
        .icon_name = "blank",
        .label = "Start Angle",
        .type = "double",
        .map_signal = "lineEditArcStartAngle"
    },
    {
        .object = OBJ_TYPE_ARC,
        .key = "arc_end_angle",
        .icon_name = "blank",
        .label = "End Angle",
        .type = "double",
        .map_signal = "lineEditArcEndAngle"
    },
    {
        .object = OBJ_TYPE_ARC,
        .key = "arc_start_x",
        .icon_name = "blank",
        .label = "Start X",
        .type = "double",
        .map_signal = ""
    },
    {
        .object = OBJ_TYPE_ARC,
        .key = "arc_start_y",
        .icon_name = "blank",
        .label = "Start Y",
        .type = "double",
        .map_signal = ""
    },
    {
        .object = OBJ_TYPE_ARC,
        .key = "arc_end_x",
        .icon_name = "blank",
        .label = "End X",
        .type = "double",
        .map_signal = ""
    },
    {
        .object = OBJ_TYPE_ARC,
        .key = "arc_end_y",
        .icon_name = "blank",
        .label = "End Y",
        .type = "double",
        .map_signal = ""
    },
    {
        .object = OBJ_TYPE_ARC,
        .key = "arc_area",
        .icon_name = "blank",
        .label = "Area",
        .type = "double",
        .map_signal = ""
    },
    {
        .object = OBJ_TYPE_ARC,
        .key = "arc_length",
        .icon_name = "blank",
        .label = "Arc Length",
        .type = "double",
        .map_signal = ""
    },
    {
        .object = OBJ_TYPE_ARC,
        .key = "arc_chord",
        .icon_name = "blank",
        .label = "Chord",
        .type = "double",
        .map_signal = ""
    },
    {
        .object = OBJ_TYPE_ARC,
        .key = "arc_inc_angle",
        .icon_name = "blank",
        .label = "Included Angle",
        .type = "double",
        .map_signal = ""
    }
};
const int group_box_arc_geometry_entries = 13;

/**
 * \todo use proper icons for toolButtons
 */
GroupBoxData group_box_ellipse_geometry[] = {
    {
        .object = OBJ_TYPE_ELLIPSE,
        .key = "ellipse_center_x",
        .icon_name = "blank",
        .label = "Center X",
        .type = "double",
        .map_signal = "lineEditEllipseCenterX"
    },
    {
        .object = OBJ_TYPE_ELLIPSE,
        .key = "ellipse_center_y",
        .icon_name = "blank",
        .label = "Center Y",
        .type = "double",
        .map_signal = "lineEditEllipseCenterY"
    },
    {
        .object = OBJ_TYPE_ELLIPSE,
        .key = "ellipse_semi_major_axis",
        .icon_name = "blank",
        .label = "Semi-Major Axis",
        .type = "double",
        .map_signal = "lineEditEllipseSemiMajorAxis"
    },
    {
        .object = OBJ_TYPE_ELLIPSE,
        .key = "ellipse_semi_minor_axis",
        .icon_name = "blank",
        .label = "Semi-Minor Axis",
        .type = "double",
        .map_signal = "lineEditEllipseSemiMinorAxis"
    },
    {
        .object = OBJ_TYPE_ELLIPSE,
        .key = "ellipse_major_axis",
        .icon_name = "blank",
        .label = "Major Axis",
        .type = "double",
        .map_signal = "lineEditEllipseMajorAxis"
    },
    {
        .object = OBJ_TYPE_ELLIPSE,
        .key = "ellipse_minor_axis",
        .icon_name = "blank",
        .label = "Minor Axis",
        .type = "double",
        .map_signal = "lineEditEllipseMinorAxis"
    }
};
const int group_box_ellipse_geometry_entries = 6;

std::unordered_map<std::string, GroupBoxData*> group_box_data;

QString settings_general_language;
QString settings_general_icon_theme;
int settings_general_icon_size;
bool settings_general_mdi_bg_use_logo;
bool settings_general_mdi_bg_use_texture;
bool settings_general_mdi_bg_use_color;
QString settings_general_mdi_bg_logo;
QString settings_general_mdi_bg_texture;
QRgb settings_general_mdi_bg_color;
bool settings_general_tip_of_the_day;
quint16 settings_general_current_tip;
bool settings_general_system_help_browser;
bool settings_general_check_for_updates;
bool settings_display_use_opengl;
bool settings_display_renderhint_aa;
bool settings_display_renderhint_text_aa;
bool settings_display_renderhint_smooth_pix;
bool settings_display_renderhint_high_aa;
bool settings_display_renderhint_noncosmetic;
bool settings_display_show_scrollbars;
int settings_display_scrollbar_widget_num;
QRgb settings_display_crosshair_color;
QRgb settings_display_bg_color;
QRgb settings_display_selectbox_left_color;
QRgb settings_display_selectbox_left_fill;
QRgb settings_display_selectbox_right_color;
QRgb settings_display_selectbox_right_fill;
uint8_t settings_display_selectbox_alpha;
EmbReal settings_display_zoomscale_in;
EmbReal settings_display_zoomscale_out;
uint8_t settings_display_crosshair_percent;
QString settings_display_units;
QRgb settings_prompt_text_color;
QRgb settings_prompt_bg_color;
QString settings_prompt_font_family;
QString settings_prompt_font_style;
uint8_t settings_prompt_font_size;
bool settings_prompt_save_history;
bool settings_prompt_save_history_as_html;
QString settings_prompt_save_history_filename;
QString settings_opensave_custom_filter;
QString settings_opensave_open_format;
bool settings_opensave_open_thumbnail;
QString settings_opensave_save_format;
bool settings_opensave_save_thumbnail;
uint8_t settings_opensave_recent_max_files;
QStringList settings_opensave_recent_list_of_files;
QString settings_opensave_recent_directory;
uint8_t settings_opensave_trim_dst_num_jumps;
QString settings_printing_default_device;
bool settings_printing_use_last_device;
bool settings_printing_disable_bg;
bool settings_grid_show_on_load;
bool settings_grid_show_origin;
bool settings_grid_color_match_crosshair;
QRgb settings_grid_color;
bool settings_grid_load_from_file;
QString settings_grid_type;
bool settings_grid_center_on_origin;
EmbReal settings_grid_center_x;
EmbReal settings_grid_center_y;
EmbReal settings_grid_size_x;
EmbReal settings_grid_size_y;
EmbReal settings_grid_spacing_x;
EmbReal settings_grid_spacing_y;
EmbReal settings_grid_size_radius;
EmbReal settings_grid_spacing_radius;
EmbReal settings_grid_spacing_angle;
bool settings_ruler_show_on_load;
bool settings_ruler_metric;
QRgb settings_ruler_color;
uint8_t settings_ruler_pixel_size;
bool settings_qsnap_enabled;
QRgb settings_qsnap_locator_color;
uint8_t settings_qsnap_locator_size;
uint8_t settings_qsnap_aperture_size;
bool settings_qsnap_endpoint;
bool settings_qsnap_midpoint;
bool settings_qsnap_center;
bool settings_qsnap_node;
bool settings_qsnap_quadrant;
bool settings_qsnap_intersection;
bool settings_qsnap_extension;
bool settings_qsnap_insertion;
bool settings_qsnap_perpendicular;
bool settings_qsnap_tangent;
bool settings_qsnap_nearest;
bool settings_qsnap_apparent;
bool settings_qsnap_parallel;
bool settings_lwt_show_lwt;
bool settings_lwt_real_render;
EmbReal settings_lwt_default_lwt;
bool settings_selection_mode_pickfirst;
bool settings_selection_mode_pickadd;
bool settings_selection_mode_pickdrag;
QRgb settings_selection_coolgrip_color;
QRgb settings_selection_hotgrip_color;
uint8_t settings_selection_grip_size;
uint8_t settings_selection_pickbox_size;
QString settings_text_font;
EmbReal settings_text_size;
EmbReal settings_text_angle;
bool settings_text_style_bold;
bool settings_text_style_italic;
bool settings_text_style_underline;
bool settings_text_style_overline;
bool settings_text_style_strikeout;

//Temporary for instant preview
bool preview_general_mdi_bg_use_logo;
bool preview_general_mdi_bg_use_texture;
bool preview_general_mdi_bg_use_color;

QString accept_general_mdi_bg_logo;
QString accept_general_mdi_bg_texture;
QRgb preview_general_mdi_bg_color;
QRgb accept_general_mdi_bg_color;

bool preview_display_show_scrollbars;

QRgb preview_display_crosshair_color;
QRgb accept_display_crosshair_color;
QRgb preview_display_bg_color;
QRgb accept_display_bg_color;

QRgb preview_display_selectbox_left_color;
QRgb accept_display_selectbox_left_color;
QRgb preview_display_selectbox_left_fill;
QRgb accept_display_selectbox_left_fill;
QRgb preview_display_selectbox_right_color;
QRgb accept_display_selectbox_right_color;
QRgb preview_display_selectbox_right_fill;
QRgb accept_display_selectbox_right_fill;
quint8  preview_display_selectbox_alpha;

QRgb preview_prompt_text_color;
QRgb accept_prompt_text_color;

QRgb preview_prompt_bg_color;
QRgb accept_prompt_bg_color;

QString preview_prompt_font_family;
QString preview_prompt_font_style;
quint8  preview_prompt_font_size;

QRgb preview_grid_color;
QRgb accept_grid_color;

QRgb preview_ruler_color;
QRgb accept_ruler_color;

bool preview_lwt_show_lwt;
bool preview_lwt_real_render;

//Temporary until changes are accepted
QString dialog_general_language;
QString dialog_general_icon_theme;
int dialog_general_icon_size;
bool dialog_general_mdi_bg_use_logo;
bool dialog_general_mdi_bg_use_texture;
bool dialog_general_mdi_bg_use_color;
QString dialog_general_mdi_bg_logo;
QString dialog_general_mdi_bg_texture;
QRgb dialog_general_mdi_bg_color;
bool dialog_general_tip_of_the_day;
bool dialog_general_system_help_browser;
bool dialog_display_use_opengl;
bool dialog_display_renderhint_aa;
bool dialog_display_renderhint_text_aa;
bool dialog_display_renderhint_smooth_pix;
bool dialog_display_renderhint_high_aa;
bool dialog_display_renderhint_noncosmetic;
bool dialog_display_show_scrollbars;
int dialog_display_scrollbar_widget_num;
QRgb dialog_display_crosshair_color;
QRgb dialog_display_bg_color;
QRgb dialog_display_selectbox_left_color;
QRgb dialog_display_selectbox_left_fill;
QRgb dialog_display_selectbox_right_color;
QRgb dialog_display_selectbox_right_fill;
quint8  dialog_display_selectbox_alpha;
EmbReal dialog_display_zoomscale_in;
EmbReal dialog_display_zoomscale_out;
quint8  dialog_display_crosshair_percent;
QString dialog_display_units;
QRgb dialog_prompt_text_color;
QRgb dialog_prompt_bg_color;
QString dialog_prompt_font_family;
QString dialog_prompt_font_style;
quint8  dialog_prompt_font_size;
bool dialog_prompt_save_history;
bool dialog_prompt_save_history_as_html;
QString dialog_prompt_save_history_filename;
QString dialog_opensave_custom_filter;
QString dialog_opensave_open_format;
bool dialog_opensave_open_thumbnail;
QString dialog_opensave_save_format;
bool dialog_opensave_save_thumbnail;
quint8  dialog_opensave_recent_max_files;
quint8  dialog_opensave_trim_dst_num_jumps;
QString dialog_printing_default_device;
bool dialog_printing_use_last_device;
bool dialog_printing_disable_bg;
bool dialog_grid_show_on_load;
bool dialog_grid_show_origin;
bool dialog_grid_color_match_crosshair;
QRgb dialog_grid_color;
bool dialog_grid_load_from_file;
QString dialog_grid_type;
bool dialog_grid_center_on_origin;
EmbReal dialog_grid_center_x;
EmbReal dialog_grid_center_y;
EmbReal dialog_grid_size_x;
EmbReal dialog_grid_size_y;
EmbReal dialog_grid_spacing_x;
EmbReal dialog_grid_spacing_y;
EmbReal dialog_grid_size_radius;
EmbReal dialog_grid_spacing_radius;
EmbReal dialog_grid_spacing_angle;
bool dialog_ruler_show_on_load;
bool dialog_ruler_metric;
QRgb dialog_ruler_color;
quint8  dialog_ruler_pixel_size;
bool dialog_qsnap_enabled;
QRgb dialog_qsnap_locator_color;
quint8  dialog_qsnap_locator_size;
quint8  dialog_qsnap_aperture_size;
bool dialog_qsnap_endpoint;
bool dialog_qsnap_midpoint;
bool dialog_qsnap_center;
bool dialog_qsnap_node;
bool dialog_qsnap_quadrant;
bool dialog_qsnap_intersection;
bool dialog_qsnap_extension;
bool dialog_qsnap_insertion;
bool dialog_qsnap_perpendicular;
bool dialog_qsnap_tangent;
bool dialog_qsnap_nearest;
bool dialog_qsnap_apparent;
bool dialog_qsnap_parallel;
bool dialog_lwt_show_lwt;
bool dialog_lwt_real_render;
EmbReal dialog_lwt_default_lwt;
bool dialog_selection_mode_pickfirst;
bool dialog_selection_mode_pickadd;
bool dialog_selection_mode_pickdrag;
QRgb dialog_selection_coolgrip_color;
QRgb dialog_selection_hotgrip_color;
quint8  dialog_selection_grip_size;
quint8  dialog_selection_pickbox_size;
