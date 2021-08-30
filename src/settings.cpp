/* This file is part of Embroidermodder 2.
 * ------------------------------------------------------------
 * Copyright 2021 The Embroidermodder Team
 * Embroidermodder 2 is Open Source Software.
 * See LICENSE.txt for licensing terms.
 * ------------------------------------------------------------
 * This file is for the in-progress translation of the settings
 * system into C without dependencies.
 */

#include "embroidermodder.h"

#include <QSettings>

extern settings_wrapper settings;

char assets_dir[1000];
char settings_fname[1000];
char settings_data[5000];
char value_out[1000];
char *settings_positions[1000];
int settings_data_length;

char * copy_ini_block(int i, char *data, char *section);
int find_ini_value(char *section, char *key, char *out);
int get_ini_int(char *section, char *key, int default_value);
float get_ini_float(char *section, char *key, float default_value);
int embClamp(int lower, int x, int upper);

int
find_ini_value(char *section, char *key, char *out)
{
    int i;
    /* find the block start */
    for (i=0; i<settings_data_length; i++) {
        if (settings_data[i] == '[') {
            if (!strncmp(settings_data+i, section, strlen(section))) {
                break;
            }
        }
    }

    /* find line starts, check for key */
    for (; i<settings_data_length; i++) {
        if (settings_data[i] == '\n' || settings_data[i] == '\r') {
            if (!strncmp(settings_data+i+1, key, strlen(key))) {
                int j;
                strcpy(out, settings_data+i+2+strlen(key));
                for (j=0; j<strlen(out); j++) {
                    if (out[j] == '\n' || out[j] == '\r') {
                        out[j] = 0;
                        break;
                    }
                }
                return 1;
            }
        }
        /* reached next section, break */
        if (settings_data[i] == '[') {
            break;
        }
    }
    return 0;
}

int
get_ini_int(char *section, char *key, int default_value)
{
    int r;
    r = find_ini_value(section, key, value_out);
    if (r) {
        return atoi(value_out);
    }
    return default_value;
}

float
get_ini_float(char *section, char *key, float default_value)
{
    int r;
    r = find_ini_value(section, key, value_out);
    if (r) {
        return atof(value_out);
    }
    return default_value;
}

char *
get_ini_str(char *section, char *key, char *default_value)
{
    int r;
    r = find_ini_value(section, key, value_out);
    if (r) {
        return value_out;
    }
    return default_value;
}

int
load_settings(void)
{
    FILE *f;
    int i;
    app_dir(assets_dir, 0);
    strcpy(settings_fname, assets_dir);
    strcat(settings_fname, "settings.ini");

    puts("Embroidermodder sandbox.");

    /* Step zero: load all of the ini file into a char buffer. */
    f = fopen(settings_fname, "rb");
    fseek(f, 0, SEEK_END);
    settings_data_length = ftell(f);
    fseek(f, 0, SEEK_SET);
    fread(settings_data, 1, settings_data_length, f);
    fclose(f);

    settings.window_width = get_ini_int("[Window]", "Width", 640);
    settings.window_height = get_ini_int("[Window]", "Height", 480);
    settings.window_width = embClamp(640, settings.window_width, 10000);
    settings.window_height = embClamp(480, settings.window_height, 10000);
    settings.window_x = get_ini_int("[Window]", "PositionX", 0);
    settings.window_y = get_ini_int("[Window]", "PositionY", 0);
    settings.window_x = embClamp(0, settings.window_x, 1000);
    settings.window_y = embClamp(0, settings.window_y, 1000);

    settings.general_language = QString(get_ini_str("[General]", "Language", "default"));
    settings.general_icon_theme = QString(get_ini_str("[General]", "IconTheme", "default"));
    settings.general_icon_size = get_ini_int("[General]", "IconSize", 16);
    settings.general_mdi_bg_use_logo = get_ini_int("[General]", "MdiBGUseLogo", 1);
    settings.general_mdi_bg_use_texture = get_ini_int("[General]", "MdiBGUseTexture", 1);
    settings.general_mdi_bg_use_color = get_ini_int("[General]", "MdiBGUseColor", 1);
    return 0;
}

int
save_settings(void)
{
    app_dir(assets_dir, 0);
    strcpy(settings_fname, assets_dir);
    /* This file needs to be read from the users home directory to ensure it is writable. */
    strcat(settings_fname, "settings.ini");
}

QString
load_setting_str(char *s, char* default_value)
{
    QSettings settings_file(QString(settings_fname), QSettings::IniFormat);
    return settings_file.value(s, default_value).toString();
}

QString
load_setting_str(char *s, QString default_value)
{
    QSettings settings_file(QString(settings_fname), QSettings::IniFormat);
    return settings_file.value(s, default_value).toString();
}

int embClamp(int lower, int x, int upper)
{
    x = embMinInt(upper, x);
    x = embMaxInt(lower, x);
    return x;
}

void MainWindow::readSettings()
{
    debug_message("Reading Settings...");

    // This file needs to be read from the users home directory to ensure it is writable
    app_dir(assets_dir, 0);
    strcpy(settings_fname, assets_dir);
    strcat(settings_fname, "settings.ini");

    load_settings();

    QSettings settings_file(QString(assets_dir)+"settings.ini", QSettings::IniFormat);
    QPoint pos(settings.window_x, settings.window_y);
    QSize size(settings.window_width, settings.window_height);

    layoutState = settings_file.value("LayoutState").toByteArray();
    if(!restoreState(layoutState))
    {
        debug_message("LayoutState NOT restored! Setting Default Layout...");
        //someToolBar->setVisible(true);
    }

    //General
    settings.general_mdi_bg_logo = load_setting_str("MdiBGLogo", QString(assets_dir) + "/images/logo-spirals.png");
    settings.general_mdi_bg_texture = load_setting_str("MdiBGTexture", QString(assets_dir) + "/images/texture-spirals.png");
    settings.general_mdi_bg_color = settings_file.value("MdiBGColor", qRgb(192,192,192)).toInt();
    settings.general_tip_of_the_day = settings_file.value("TipOfTheDay", true).toBool();
    settings.general_current_tip = settings_file.value("CurrentTip", 0).toInt();
    settings.general_system_help_browser = settings_file.value("SystemHelpBrowser", true).toBool();
    //Display
    settings.display_use_opengl = settings_file.value("Display/UseOpenGL", false).toBool();
    settings.display_renderhint_aa = settings_file.value("Display/RenderHintAntiAlias", false).toBool();
    settings.display_renderhint_text_aa = settings_file.value("Display/RenderHintTextAntiAlias", false).toBool();
    settings.display_renderhint_smooth_pix = settings_file.value("Display/RenderHintSmoothPixmap", false).toBool();
    settings.display_renderhint_high_aa = settings_file.value("Display/RenderHintHighQualityAntiAlias", false).toBool();
    settings.display_renderhint_noncosmetic = settings_file.value("Display/RenderHintNonCosmetic", false).toBool();
    settings.display_show_scrollbars = settings_file.value("Display/ShowScrollBars", true).toBool();
    settings.display_scrollbar_widget_num = settings_file.value("Display/ScrollBarWidgetNum", 0).toInt();
    settings.display_crosshair_color = settings_file.value("Display/CrossHairColor", qRgb(  0, 0, 0)).toInt();
    settings.display_bg_color = settings_file.value("Display/BackgroundColor", qRgb(235,235,235)).toInt();
    settings.display_selectbox_left_color = settings_file.value("Display/SelectBoxLeftColor", qRgb(  0,128, 0)).toInt();
    settings.display_selectbox_left_fill = settings_file.value("Display/SelectBoxLeftFill", qRgb(  0,255, 0)).toInt();
    settings.display_selectbox_right_color = settings_file.value("Display/SelectBoxRightColor", qRgb(  0, 0,128)).toInt();
    settings.display_selectbox_right_fill = settings_file.value("Display/SelectBoxRightFill", qRgb(  0, 0,255)).toInt();
    settings.display_selectbox_alpha = settings_file.value("Display/SelectBoxAlpha", 32).toInt();
    settings.display_zoomscale_in = settings_file.value("Display/ZoomScaleIn", 2.0).toFloat();
    settings.display_zoomscale_out = settings_file.value("Display/ZoomScaleOut", 0.5).toFloat();
    settings.display_crosshair_percent = settings_file.value("Display/CrossHairPercent", 5).toInt();
    settings.display_units = settings_file.value("Display/Units", "mm").toString();
    //Prompt
    //OpenSave
    settings.opensave_custom_filter = settings_file.value("OpenSave/CustomFilter", "supported").toString();
    settings.opensave_open_format = settings_file.value("OpenSave/OpenFormat", "*.*").toString();
    settings.opensave_open_thumbnail = settings_file.value("OpenSave/OpenThumbnail", false).toBool();
    settings.opensave_save_format = settings_file.value("OpenSave/SaveFormat", "*.*").toString();
    settings.opensave_save_thumbnail = settings_file.value("OpenSave/SaveThumbnail", false).toBool();
    //Recent
    settings.opensave_recent_max_files = settings_file.value("OpenSave/RecentMax", 10).toInt();
    settings.opensave_recent_list_of_files = settings_file.value("OpenSave/RecentFiles")                                .toStringList();
    settings.opensave_recent_directory = settings_file.value("OpenSave/RecentDirectory", QString(assets_dir) + "/samples").toString();
    //Trimming
    settings.opensave_trim_dst_num_jumps = settings_file.value("OpenSave/TrimDstNumJumps", 5).toInt();
    //Printing
    settings.printing_default_device = settings_file.value("Printing/DefaultDevice", "").toString();
    settings.printing_use_last_device = settings_file.value("Printing/UseLastDevice", false).toBool();
    settings.printing_disable_bg = settings_file.value("Printing/DisableBG", true).toBool();
    //Grid
    settings.grid_show_on_load = settings_file.value("Grid/ShowOnLoad", true).toBool();
    settings.grid_show_origin = settings_file.value("Grid/ShowOrigin", true).toBool();
    settings.grid_color_match_crosshair = settings_file.value("Grid/ColorMatchCrossHair", true).toBool();
    int red = settings_file.value("Grid/ColorR", 0).toInt();
    int green = settings_file.value("Grid/ColorG", 0).toInt();
    int blue = settings_file.value("Grid/ColorB", 0).toInt();
    settings.grid_color = QColor(red, green, blue).rgb();
    settings.grid_load_from_file = settings_file.value("Grid/LoadFromFile", true).toBool();
    settings.grid_type = settings_file.value("Grid/Type", "Rectangular").toString();
    settings.grid_center_on_origin = settings_file.value("Grid/CenterOnOrigin", true).toBool();
    settings.grid_center_x = settings_file.value("Grid/CenterX", 0.0).toFloat();
    settings.grid_center_y = settings_file.value("Grid/CenterY", 0.0).toFloat();
    settings.grid_size_x = settings_file.value("Grid/SizeX", 100.0).toFloat();
    settings.grid_size_y = settings_file.value("Grid/SizeY", 100.0).toFloat();
    settings.grid_spacing_x = settings_file.value("Grid/SpacingX", 25.0).toFloat();
    settings.grid_spacing_y = settings_file.value("Grid/SpacingY", 25.0).toFloat();
    settings.grid_size_radius = settings_file.value("Grid/SizeRadius", 50.0).toFloat();
    settings.grid_spacing_radius = settings_file.value("Grid/SpacingRadius", 25.0).toFloat();
    settings.grid_spacing_angle = settings_file.value("Grid/SpacingAngle", 45.0).toFloat();
    //Ruler
    settings.ruler_show_on_load = settings_file.value("Ruler/ShowOnLoad", true).toBool();
    settings.ruler_metric = settings_file.value("Ruler/Metric", true).toBool();
    settings.ruler_color = settings_file.value("Ruler/Color", qRgb(210,210, 50)).toInt();
    settings.ruler_pixel_size = settings_file.value("Ruler/PixelSize", 20).toInt();
    //Quick Snap
    settings.qsnap_enabled = settings_file.value("QuickSnap/Enabled", true).toBool();
    settings.qsnap_locator_color = settings_file.value("QuickSnap/LocatorColor", qRgb(255,255, 0)).toInt();
    settings.qsnap_locator_size = settings_file.value("QuickSnap/LocatorSize", 4).toInt();
    settings.qsnap_aperture_size = settings_file.value("QuickSnap/ApertureSize", 10).toInt();
    settings.qsnap_endpoint = settings_file.value("QuickSnap/EndPoint", true).toBool();
    settings.qsnap_midpoint = settings_file.value("QuickSnap/MidPoint", true).toBool();
    settings.qsnap_center = settings_file.value("QuickSnap/Center", true).toBool();
    settings.qsnap_node = settings_file.value("QuickSnap/Node", true).toBool();
    settings.qsnap_quadrant = settings_file.value("QuickSnap/Quadrant", true).toBool();
    settings.qsnap_intersection = settings_file.value("QuickSnap/Intersection", true).toBool();
    settings.qsnap_extension = settings_file.value("QuickSnap/Extension", true).toBool();
    settings.qsnap_insertion = settings_file.value("QuickSnap/Insertion", false).toBool();
    settings.qsnap_perpendicular = settings_file.value("QuickSnap/Perpendicular", true).toBool();
    settings.qsnap_tangent = settings_file.value("QuickSnap/Tangent", true).toBool();
    settings.qsnap_nearest = settings_file.value("QuickSnap/Nearest", false).toBool();
    settings.qsnap_apparent = settings_file.value("QuickSnap/Apparent", false).toBool();
    settings.qsnap_parallel = settings_file.value("QuickSnap/Parallel", false).toBool();
    //LineWeight
    settings.lwt_show_lwt = settings_file.value("LineWeight/ShowLineWeight", false).toBool();
    settings.lwt_real_render = settings_file.value("LineWeight/RealRender", true).toBool();
    settings.lwt_default_lwt = settings_file.value("LineWeight/DefaultLineWeight", 0).toReal();
    //Selection
    settings.selection_mode_pickfirst = settings_file.value("Selection/PickFirst", true).toBool();
    settings.selection_mode_pickadd = settings_file.value("Selection/PickAdd", true).toBool();
    settings.selection_mode_pickdrag = settings_file.value("Selection/PickDrag", false).toBool();
    settings.selection_coolgrip_color = settings_file.value("Selection/CoolGripColor", qRgb(  0, 0,255)).toInt();
    settings.selection_hotgrip_color = settings_file.value("Selection/HotGripColor", qRgb(255, 0, 0)).toInt();
    settings.selection_grip_size = settings_file.value("Selection/GripSize", 4).toInt();
    settings.selection_pickbox_size = settings_file.value("Selection/PickBoxSize", 4).toInt();
    //Text
    settings.text_font = settings_file.value("Text/Font", "Arial").toString();
    settings.text_size = settings_file.value("Text/Size", 12).toReal();
    settings.text_angle = settings_file.value("Text/Angle", 0).toReal();
    settings.text_style_bold = settings_file.value("Text/StyleBold", false).toBool();
    settings.text_style_italic = settings_file.value("Text/StyleItalic", false).toBool();
    settings.text_style_underline = settings_file.value("Text/StyleUnderline", false).toBool();
    settings.text_style_strikeout = settings_file.value("Text/StyleStrikeOut", false).toBool();
    settings.text_style_overline = settings_file.value("Text/StyleOverline", false).toBool();

    move(pos);
    resize(size);
}

void MainWindow::writeSettings()
{
    debug_message("Writing Settings...");

    save_settings();

    QSettings settings_file(QString(settings_fname), QSettings::IniFormat);
    QString tmp;
    settings_file.setValue("Window/PositionX", pos().x());
    settings_file.setValue("Window/PositionY", pos().y());
    settings_file.setValue("Window/Width", size().width());
    settings_file.setValue("Window/Height", size().height());

    //General
    settings_file.setValue("LayoutState", layoutState);
    settings_file.setValue("Language", settings.general_language);
    settings_file.setValue("IconTheme", settings.general_icon_theme);
    settings_file.setValue("IconSize", tmp.setNum(settings.general_icon_size));
    settings_file.setValue("MdiBGUseLogo", settings.general_mdi_bg_use_logo);
    settings_file.setValue("MdiBGUseTexture", settings.general_mdi_bg_use_texture);
    settings_file.setValue("MdiBGUseColor", settings.general_mdi_bg_use_color);
    settings_file.setValue("MdiBGLogo", settings.general_mdi_bg_logo);
    settings_file.setValue("MdiBGTexture", settings.general_mdi_bg_texture);
    settings_file.setValue("MdiBGColor", tmp.setNum(settings.general_mdi_bg_color));
    settings_file.setValue("TipOfTheDay", settings.general_tip_of_the_day);
    settings_file.setValue("CurrentTip", tmp.setNum(settings.general_current_tip + 1));
    settings_file.setValue("SystemHelpBrowser", settings.general_system_help_browser);
    //Display
    settings_file.setValue("Display/UseOpenGL", settings.display_use_opengl);
    settings_file.setValue("Display/RenderHintAntiAlias", settings.display_renderhint_aa);
    settings_file.setValue("Display/RenderHintTextAntiAlias", settings.display_renderhint_text_aa);
    settings_file.setValue("Display/RenderHintSmoothPixmap", settings.display_renderhint_smooth_pix);
    settings_file.setValue("Display/RenderHintHighQualityAntiAlias", settings.display_renderhint_high_aa);
    settings_file.setValue("Display/RenderHintNonCosmetic", settings.display_renderhint_noncosmetic);
    settings_file.setValue("Display/ShowScrollBars", settings.display_show_scrollbars);
    settings_file.setValue("Display/ScrollBarWidgetNum", tmp.setNum(settings.display_scrollbar_widget_num));
    settings_file.setValue("Display/CrossHairColor", tmp.setNum(settings.display_crosshair_color));
    settings_file.setValue("Display/BackgroundColor", tmp.setNum(settings.display_bg_color));
    settings_file.setValue("Display/SelectBoxLeftColor", tmp.setNum(settings.display_selectbox_left_color));
    settings_file.setValue("Display/SelectBoxLeftFill", tmp.setNum(settings.display_selectbox_left_fill));
    settings_file.setValue("Display/SelectBoxRightColor", tmp.setNum(settings.display_selectbox_right_color));
    settings_file.setValue("Display/SelectBoxRightFill", tmp.setNum(settings.display_selectbox_right_fill));
    settings_file.setValue("Display/SelectBoxAlpha", tmp.setNum(settings.display_selectbox_alpha));
    settings_file.setValue("Display/ZoomScaleIn", tmp.setNum(settings.display_zoomscale_in));
    settings_file.setValue("Display/ZoomScaleOut", tmp.setNum(settings.display_zoomscale_out));
    settings_file.setValue("Display/CrossHairPercent", tmp.setNum(settings.display_crosshair_percent));
    settings_file.setValue("Display/Units", settings.display_units);
    //Prompt
    //OpenSave
    settings_file.setValue("OpenSave/CustomFilter", settings.opensave_custom_filter);
    settings_file.setValue("OpenSave/OpenFormat", settings.opensave_open_format);
    settings_file.setValue("OpenSave/OpenThumbnail", settings.opensave_open_thumbnail);
    settings_file.setValue("OpenSave/SaveFormat", settings.opensave_save_format);
    settings_file.setValue("OpenSave/SaveThumbnail", settings.opensave_save_thumbnail);
    //Recent
    settings_file.setValue("OpenSave/RecentMax", tmp.setNum(settings.opensave_recent_max_files));
    settings_file.setValue("OpenSave/RecentFiles", settings.opensave_recent_list_of_files);
    settings_file.setValue("OpenSave/RecentDirectory", settings.opensave_recent_directory);
    //Trimming
    settings_file.setValue("OpenSave/TrimDstNumJumps", tmp.setNum(settings.opensave_trim_dst_num_jumps));
    //Printing
    settings_file.setValue("Printing/DefaultDevice", settings.printing_default_device);
    settings_file.setValue("Printing/UseLastDevice", settings.printing_use_last_device);
    settings_file.setValue("Printing/DisableBG", settings.printing_disable_bg);
    //Grid
    settings_file.setValue("Grid/ShowOnLoad", settings.grid_show_on_load);
    settings_file.setValue("Grid/ShowOrigin", settings.grid_show_origin);
    settings_file.setValue("Grid/ColorMatchCrossHair", settings.grid_color_match_crosshair);
    settings_file.setValue("Grid/Color", tmp.setNum(settings.grid_color));
    settings_file.setValue("Grid/ColorR", QColor(settings.grid_color).red());
    settings_file.setValue("Grid/ColorG", QColor(settings.grid_color).green());
    settings_file.setValue("Grid/ColorB", QColor(settings.grid_color).blue());
    settings_file.setValue("Grid/LoadFromFile", settings.grid_load_from_file);
    settings_file.setValue("Grid/Type", settings.grid_type);
    settings_file.setValue("Grid/CenterOnOrigin", settings.grid_center_on_origin);
    settings_file.setValue("Grid/CenterX", tmp.setNum(settings.grid_center_x));
    settings_file.setValue("Grid/CenterY", tmp.setNum(settings.grid_center_y));
    settings_file.setValue("Grid/SizeX", tmp.setNum(settings.grid_size_x));
    settings_file.setValue("Grid/SizeY", tmp.setNum(settings.grid_size_y));
    settings_file.setValue("Grid/SpacingX", tmp.setNum(settings.grid_spacing_x));
    settings_file.setValue("Grid/SpacingY", tmp.setNum(settings.grid_spacing_y));
    settings_file.setValue("Grid/SizeRadius", tmp.setNum(settings.grid_size_radius));
    settings_file.setValue("Grid/SpacingRadius", tmp.setNum(settings.grid_spacing_radius));
    settings_file.setValue("Grid/SpacingAngle", tmp.setNum(settings.grid_spacing_angle));
    //Ruler
    settings_file.setValue("Ruler/ShowOnLoad", settings.ruler_show_on_load);
    settings_file.setValue("Ruler/Metric", settings.ruler_metric);
    settings_file.setValue("Ruler/Color", tmp.setNum(settings.ruler_color));
    settings_file.setValue("Ruler/PixelSize", tmp.setNum(settings.ruler_pixel_size));
    //Quick Snap
    settings_file.setValue("QuickSnap/Enabled", settings.qsnap_enabled);
    settings_file.setValue("QuickSnap/LocatorColor", tmp.setNum(settings.qsnap_locator_color));
    settings_file.setValue("QuickSnap/LocatorSize", tmp.setNum(settings.qsnap_locator_size));
    settings_file.setValue("QuickSnap/ApertureSize", tmp.setNum(settings.qsnap_aperture_size));
    settings_file.setValue("QuickSnap/EndPoint", settings.qsnap_endpoint);
    settings_file.setValue("QuickSnap/MidPoint", settings.qsnap_midpoint);
    settings_file.setValue("QuickSnap/Center", settings.qsnap_center);
    settings_file.setValue("QuickSnap/Node", settings.qsnap_node);
    settings_file.setValue("QuickSnap/Quadrant", settings.qsnap_quadrant);
    settings_file.setValue("QuickSnap/Intersection", settings.qsnap_intersection);
    settings_file.setValue("QuickSnap/Extension", settings.qsnap_extension);
    settings_file.setValue("QuickSnap/Insertion", settings.qsnap_insertion);
    settings_file.setValue("QuickSnap/Perpendicular", settings.qsnap_perpendicular);
    settings_file.setValue("QuickSnap/Tangent", settings.qsnap_tangent);
    settings_file.setValue("QuickSnap/Nearest", settings.qsnap_nearest);
    settings_file.setValue("QuickSnap/Apparent", settings.qsnap_apparent);
    settings_file.setValue("QuickSnap/Parallel", settings.qsnap_parallel);
    //LineWeight
    settings_file.setValue("LineWeight/ShowLineWeight", settings.lwt_show_lwt);
    settings_file.setValue("LineWeight/RealRender", settings.lwt_real_render);
    settings_file.setValue("LineWeight/DefaultLineWeight", tmp.setNum(settings.lwt_default_lwt));
    //Selection
    settings_file.setValue("Selection/PickFirst", settings.selection_mode_pickfirst);
    settings_file.setValue("Selection/PickAdd", settings.selection_mode_pickadd);
    settings_file.setValue("Selection/PickDrag", settings.selection_mode_pickdrag);
    settings_file.setValue("Selection/CoolGripColor", tmp.setNum(settings.selection_coolgrip_color));
    settings_file.setValue("Selection/HotGripColor", tmp.setNum(settings.selection_hotgrip_color));
    settings_file.setValue("Selection/GripSize", tmp.setNum(settings.selection_grip_size));
    settings_file.setValue("Selection/PickBoxSize", tmp.setNum(settings.selection_pickbox_size));
    //Text
    settings_file.setValue("Text/Font", settings.text_font);
    settings_file.setValue("Text/Size", tmp.setNum(settings.text_size));
    settings_file.setValue("Text/Angle", tmp.setNum(settings.text_angle));
    settings_file.setValue("Text/StyleBold", settings.text_style_bold);
    settings_file.setValue("Text/StyleItalic", settings.text_style_italic);
    settings_file.setValue("Text/StyleUnderline", settings.text_style_underline);
    settings_file.setValue("Text/StyleStrikeOut", settings.text_style_strikeout);
    settings_file.setValue("Text/StyleOverline", settings.text_style_overline);
}

void MainWindow::settingsDialog(const QString& showTab)
{
    Settings_Dialog dialog_(this, showTab, this);
    dialog_.exec();
}

