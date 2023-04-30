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

/**
 * \file mainwindow-settings.cpp
 */

#include "embroidermodder.h"

#include <string>
#include <iostream>
#include <fstream>

/**
 * .
 * Note: on Unix we include the trailing separator.
 * For Windows compatibility we omit it.
 */
QString SettingsDir()
{
#if defined(Q_OS_UNIX) || defined(Q_OS_MAC)
    QString homePath = QDir::homePath();
    return homePath + "/.embroidermodder2/";
#else
    return "";
#endif
}

/**
 * .
 */
QString SettingsPath()
{
    QString settingsPath = SettingsDir() + "settings.ini";
    return settingsPath;
}

/**
 * .
 */
std::vector<std::string>
to_string_vector(QStringList list)
{
    std::vector<std::string> a;
    for (QString s: list) {
        a.push_back(s.toStdString());
    }
    return a;
}

/**
 * .
 */
void
read_configuration()
{
    QString appDir = qApp->applicationDirPath();
    QSettings settings_file(appDir + "/config.ini", QSettings::IniFormat);
    QString file_toolbar_str = settings_file.value("file_toolbar", "").toString();
    file_toolbar = to_string_vector(file_toolbar_str.split(" "));
}

/**
 * @brief MainWindow::readSettings
 */
void
MainWindow::readSettings()
{
    qDebug("Reading Settings...");
    // This file needs to be read from the users home directory to ensure it is writable
    QString settingsPath = SettingsPath();
    QString settingsDir = SettingsDir();
    QString appDir = qApp->applicationDirPath();
    QSettings settings_file(settingsPath, QSettings::IniFormat);
    QPoint pos = settings_file.value("Window/Position", QPoint(0, 0)).toPoint();
    QSize size = settings_file.value("Window/Size", QSize(800, 600)).toSize();

    layoutState = settings_file.value("LayoutState").toByteArray();
    if(!restoreState(layoutState))
    {
        qDebug("LayoutState NOT restored! Setting Default Layout...");
        //someToolBar->setVisible(true);
    }

    //General
    settings_general_language = settings_file.value("Language", "default").toString();
    settings_general_icon_theme = settings_file.value("IconTheme", "default").toString();
    settings_general_icon_size = settings_file.value("IconSize", 16).toInt();
    settings_general_mdi_bg_use_logo = settings_file.value("MdiBGUseLogo", true).toBool();
    settings_general_mdi_bg_use_texture = settings_file.value("MdiBGUseTexture", true).toBool();
    settings_general_mdi_bg_use_color = settings_file.value("MdiBGUseColor", true).toBool();
    settings_general_mdi_bg_logo = settings_file.value("MdiBGLogo", appDir + "/images/logo-spirals.png").toString();
    settings_general_mdi_bg_texture = settings_file.value("MdiBGTexture", appDir + "/images/texture-spirals.png").toString();
    settings_general_mdi_bg_color = settings_file.value("MdiBGColor", qRgb(192,192,192)).toInt();
    settings_general_tip_of_the_day = settings_file.value("TipOfTheDay", true).toBool();
    settings_general_current_tip = settings_file.value("CurrentTip", 0).toInt();
    settings_general_system_help_browser = settings_file.value("SystemHelpBrowser", true).toBool();
    //Display
    settings_display_use_opengl = settings_file.value("Display/UseOpenGL", false).toBool();
    settings_display_renderhint_aa = settings_file.value("Display/RenderHintAntiAlias", false).toBool();
    settings_display_renderhint_text_aa = settings_file.value("Display/RenderHintTextAntiAlias", false).toBool();
    settings_display_renderhint_smooth_pix = settings_file.value("Display/RenderHintSmoothPixmap", false).toBool();
    settings_display_renderhint_high_aa = settings_file.value("Display/RenderHintHighQualityAntiAlias", false).toBool();
    settings_display_renderhint_noncosmetic = settings_file.value("Display/RenderHintNonCosmetic", false).toBool();
    settings_display_show_scrollbars = settings_file.value("Display/ShowScrollBars", true).toBool();
    settings_display_scrollbar_widget_num = settings_file.value("Display/ScrollBarWidgetNum", 0).toInt();
    settings_display_crosshair_color = settings_file.value("Display/CrossHairColor", qRgb(  0, 0, 0)).toInt();
    settings_display_bg_color = settings_file.value("Display/BackgroundColor", qRgb(235,235,235)).toInt();
    settings_display_selectbox_left_color = settings_file.value("Display/SelectBoxLeftColor", qRgb(  0,128, 0)).toInt();
    settings_display_selectbox_left_fill = settings_file.value("Display/SelectBoxLeftFill", qRgb(  0,255, 0)).toInt();
    settings_display_selectbox_right_color = settings_file.value("Display/SelectBoxRightColor", qRgb(  0, 0,128)).toInt();
    settings_display_selectbox_right_fill = settings_file.value("Display/SelectBoxRightFill", qRgb(  0, 0,255)).toInt();
    settings_display_selectbox_alpha = settings_file.value("Display/SelectBoxAlpha", 32).toInt();
    settings_display_zoomscale_in = settings_file.value("Display/ZoomScaleIn", 2.0).toFloat();
    settings_display_zoomscale_out = settings_file.value("Display/ZoomScaleOut", 0.5).toFloat();
    settings_display_crosshair_percent = settings_file.value("Display/CrossHairPercent", 5).toInt();
    settings_display_units = settings_file.value("Display/Units", "mm").toString();
    //Prompt
    settings_prompt_text_color = settings_file.value("Prompt/TextColor", qRgb(  0, 0, 0)).toInt();
    settings_prompt_bg_color = settings_file.value("Prompt/BackgroundColor", qRgb(255,255,255)).toInt();
    settings_prompt_font_family = settings_file.value("Prompt/FontFamily", "Monospace").toString();
    settings_prompt_font_style = settings_file.value("Prompt/FontStyle", "normal").toString();
    settings_prompt_font_size = settings_file.value("Prompt/FontSize", 12).toInt();
    settings_prompt_save_history = settings_file.value("Prompt/SaveHistory", true).toBool();
    settings_prompt_save_history_as_html = settings_file.value("Prompt/SaveHistoryAsHtml", false).toBool();
    settings_prompt_save_history_filename = settings_file.value("Prompt/SaveHistoryFilename", settingsDir + "prompt.log").toString();
    //OpenSave
    settings_opensave_custom_filter = settings_file.value("OpenSave/CustomFilter", "supported").toString();
    settings_opensave_open_format = settings_file.value("OpenSave/OpenFormat", "*.*").toString();
    settings_opensave_open_thumbnail = settings_file.value("OpenSave/OpenThumbnail", false).toBool();
    settings_opensave_save_format = settings_file.value("OpenSave/SaveFormat", "*.*").toString();
    settings_opensave_save_thumbnail = settings_file.value("OpenSave/SaveThumbnail", false).toBool();
    //Recent
    settings_opensave_recent_max_files = settings_file.value("OpenSave/RecentMax", 10).toInt();
    settings_opensave_recent_list_of_files = settings_file.value("OpenSave/RecentFiles")                                .toStringList();
    settings_opensave_recent_directory = settings_file.value("OpenSave/RecentDirectory", appDir + "/samples").toString();
    //Trimming
    settings_opensave_trim_dst_num_jumps = settings_file.value("OpenSave/TrimDstNumJumps", 5).toInt();
    //Printing
    settings_printing_default_device = settings_file.value("Printing/DefaultDevice", "").toString();
    settings_printing_use_last_device = settings_file.value("Printing/UseLastDevice", false).toBool();
    settings_printing_disable_bg = settings_file.value("Printing/DisableBG", true).toBool();
    //Grid
    settings_grid_show_on_load = settings_file.value("Grid/ShowOnLoad", true).toBool();
    settings_grid_show_origin = settings_file.value("Grid/ShowOrigin", true).toBool();
    settings_grid_color_match_crosshair = settings_file.value("Grid/ColorMatchCrossHair", true).toBool();
    settings_grid_color = settings_file.value("Grid/Color", qRgb(  0, 0, 0)).toInt();
    settings_grid_load_from_file = settings_file.value("Grid/LoadFromFile", true).toBool();
    settings_grid_type = settings_file.value("Grid/Type", "Rectangular").toString();
    settings_grid_center_on_origin = settings_file.value("Grid/CenterOnOrigin", true).toBool();
    settings_grid_center_x = settings_file.value("Grid/CenterX", 0.0).toFloat();
    settings_grid_center_y = settings_file.value("Grid/CenterY", 0.0).toFloat();
    settings_grid_size_x = settings_file.value("Grid/SizeX", 100.0).toFloat();
    settings_grid_size_y = settings_file.value("Grid/SizeY", 100.0).toFloat();
    settings_grid_spacing_x = settings_file.value("Grid/SpacingX", 25.0).toFloat();
    settings_grid_spacing_y = settings_file.value("Grid/SpacingY", 25.0).toFloat();
    settings_grid_size_radius = settings_file.value("Grid/SizeRadius", 50.0).toFloat();
    settings_grid_spacing_radius = settings_file.value("Grid/SpacingRadius", 25.0).toFloat();
    settings_grid_spacing_angle = settings_file.value("Grid/SpacingAngle", 45.0).toFloat();
    //Ruler
    settings_ruler_show_on_load = settings_file.value("Ruler/ShowOnLoad", true).toBool();
    settings_ruler_metric = settings_file.value("Ruler/Metric", true).toBool();
    settings_ruler_color = settings_file.value("Ruler/Color", qRgb(210,210, 50)).toInt();
    settings_ruler_pixel_size = settings_file.value("Ruler/PixelSize", 20).toInt();
    //Quick Snap
    settings_qsnap_enabled = settings_file.value("QuickSnap/Enabled", true).toBool();
    settings_qsnap_locator_color = settings_file.value("QuickSnap/LocatorColor", qRgb(255,255, 0)).toInt();
    settings_qsnap_locator_size = settings_file.value("QuickSnap/LocatorSize", 4).toInt();
    settings_qsnap_aperture_size = settings_file.value("QuickSnap/ApertureSize", 10).toInt();
    settings_qsnap_endpoint = settings_file.value("QuickSnap/EndPoint", true).toBool();
    settings_qsnap_midpoint = settings_file.value("QuickSnap/MidPoint", true).toBool();
    settings_qsnap_center = settings_file.value("QuickSnap/Center", true).toBool();
    settings_qsnap_node = settings_file.value("QuickSnap/Node", true).toBool();
    settings_qsnap_quadrant = settings_file.value("QuickSnap/Quadrant", true).toBool();
    settings_qsnap_intersection = settings_file.value("QuickSnap/Intersection", true).toBool();
    settings_qsnap_extension = settings_file.value("QuickSnap/Extension", true).toBool();
    settings_qsnap_insertion = settings_file.value("QuickSnap/Insertion", false).toBool();
    settings_qsnap_perpendicular = settings_file.value("QuickSnap/Perpendicular", true).toBool();
    settings_qsnap_tangent = settings_file.value("QuickSnap/Tangent", true).toBool();
    settings_qsnap_nearest = settings_file.value("QuickSnap/Nearest", false).toBool();
    settings_qsnap_apparent = settings_file.value("QuickSnap/Apparent", false).toBool();
    settings_qsnap_parallel = settings_file.value("QuickSnap/Parallel", false).toBool();
    //LineWeight
    settings_lwt_show_lwt = settings_file.value("LineWeight/ShowLineWeight", false).toBool();
    settings_lwt_real_render = settings_file.value("LineWeight/RealRender", true).toBool();
    settings_lwt_default_lwt = settings_file.value("LineWeight/DefaultLineWeight", 0).toReal();
    //Selection
    settings_selection_mode_pickfirst = settings_file.value("Selection/PickFirst", true).toBool();
    settings_selection_mode_pickadd = settings_file.value("Selection/PickAdd", true).toBool();
    settings_selection_mode_pickdrag = settings_file.value("Selection/PickDrag", false).toBool();
    settings_selection_coolgrip_color = settings_file.value("Selection/CoolGripColor", qRgb(  0, 0,255)).toInt();
    settings_selection_hotgrip_color = settings_file.value("Selection/HotGripColor", qRgb(255, 0, 0)).toInt();
    settings_selection_grip_size = settings_file.value("Selection/GripSize", 4).toInt();
    settings_selection_pickbox_size = settings_file.value("Selection/PickBoxSize", 4).toInt();
    //Text
    settings_text_font = settings_file.value("Text/Font", "Arial").toString();
    settings_text_size = settings_file.value("Text/Size", 12).toReal();
    settings_text_angle = settings_file.value("Text/Angle", 0).toReal();
    settings_text_style_bold = settings_file.value("Text/StyleBold", false).toBool();
    settings_text_style_italic = settings_file.value("Text/StyleItalic", false).toBool();
    settings_text_style_underline = settings_file.value("Text/StyleUnderline", false).toBool();
    settings_text_style_strikeout = settings_file.value("Text/StyleStrikeOut", false).toBool();
    settings_text_style_overline = settings_file.value("Text/StyleOverline", false).toBool();

    move(pos);
    resize(size);
}

std::string
write_setting(std::string label, int a)
{
    char s[200];
    sprintf(s, "%s=%d\n", label.c_str(), a);
    std::string str(s);
    return str;
}

std::string
write_setting(std::string label, QRgb a)
{
    char s[200];
    sprintf(s, "%s=%u\n", label.c_str(), a);
    std::string str(s);
    return str;
}

std::string
write_setting(std::string label, QString a)
{
    return label + "=" + a.toStdString() + "\n";
}

std::string
write_setting(std::string label, float a)
{
    char s[200];
    sprintf(s, "%s=%.6f\n", label.c_str(), a);
    std::string str(s);
    return str;
}

std::string
write_setting(std::string label, bool a)
{
    if (a) {
        return label + "=true\n";
    }
    return label  + "=false\n";
}

/**
 * @brief MainWindow::writeSettings
 *
 * This file needs to be read from the users home directory to ensure it is writable
 */
void MainWindow::writeSettings()
{
    qDebug("Writing Settings...");
    QString settingsPath = SettingsPath();
    std::ofstream settings_file;
    settings_file.open(settingsPath.toStdString());

    settings_file << "[General]" << std::endl;
    //settings_file << "LayoutState=" << layoutState;
    settings_file << write_setting("Language", settings_general_language);
    settings_file << write_setting("IconTheme", settings_general_icon_theme);
    settings_file << write_setting("IconSize", settings_general_icon_size);
    settings_file << write_setting("MdiBGUseLogo", settings_general_mdi_bg_use_logo);
    settings_file << write_setting("MdiBGUseTexture", settings_general_mdi_bg_use_texture);
    settings_file << write_setting("MdiBGUseColor", settings_general_mdi_bg_use_color);
    settings_file << write_setting("MdiBGLogo", settings_general_mdi_bg_logo);
    settings_file << write_setting("MdiBGTexture", settings_general_mdi_bg_texture);
    settings_file << write_setting("MdiBGColor", settings_general_mdi_bg_color);
    settings_file << write_setting("TipOfTheDay", settings_general_tip_of_the_day);
    settings_file << write_setting("CurrentTip", settings_general_current_tip + 1);
    settings_file << write_setting("SystemHelpBrowser", settings_general_system_help_browser);
    settings_file << std::endl;

    settings_file << "[Window]" << std::endl;
    //int pos_x = pos().x();
    //int pos_y = pos().y();
    //int size_x = size().x();
    //int size_y = size().y();
    //settings_file << "Position=@()" << std::endl;
    //settings_file << "Size=@()" << std::endl;
    settings_file << std::endl;

    settings_file << "[Display]" << std::endl;
    settings_file << write_setting("UseOpenGL", settings_display_use_opengl);
    settings_file << write_setting("RenderHintAntiAlias", settings_display_renderhint_aa);
    settings_file << write_setting("RenderHintTextAntiAlias", settings_display_renderhint_text_aa);
    settings_file << write_setting("RenderHintSmoothPixmap", settings_display_renderhint_smooth_pix);
    settings_file << write_setting("RenderHintHighQualityAntiAlias", settings_display_renderhint_high_aa);
    settings_file << write_setting("RenderHintNonCosmetic", settings_display_renderhint_noncosmetic);
    settings_file << write_setting("ShowScrollBars", settings_display_show_scrollbars);
    settings_file << write_setting("ScrollBarWidgetNum", settings_display_scrollbar_widget_num);
    settings_file << write_setting("CrossHairColor", settings_display_crosshair_color);
    settings_file << write_setting("BackgroundColor", settings_display_bg_color);
    settings_file << write_setting("SelectBoxLeftColor", settings_display_selectbox_left_color);
    settings_file << write_setting("SelectBoxLeftFill", settings_display_selectbox_left_fill);
    settings_file << write_setting("SelectBoxRightColor", settings_display_selectbox_right_color);
    settings_file << write_setting("SelectBoxRightFill", settings_display_selectbox_right_fill);
    settings_file << write_setting("SelectBoxAlpha", settings_display_selectbox_alpha);
    settings_file << write_setting("ZoomScaleIn", settings_display_zoomscale_in);
    settings_file << write_setting("ZoomScaleOut", settings_display_zoomscale_out);
    settings_file << write_setting("CrossHairPercent", settings_display_crosshair_percent);
    settings_file << write_setting("Units", settings_display_units);
    settings_file << std::endl;

    settings_file << "[Prompt]" << std::endl;
    settings_file << write_setting("TextColor", settings_prompt_text_color);
    settings_file << write_setting("BackgroundColor", settings_prompt_bg_color);
    settings_file << write_setting("FontFamily", settings_prompt_font_family);
    settings_file << write_setting("FontStyle", settings_prompt_font_style);
    settings_file << write_setting("FontSize", settings_prompt_font_size);
    settings_file << write_setting("SaveHistory", settings_prompt_save_history);
    settings_file << write_setting("SaveHistoryAsHtml", settings_prompt_save_history_as_html);
    settings_file << write_setting("SaveHistoryFilename", settings_prompt_save_history_filename);
    settings_file << std::endl;

    settings_file << "[OpenSave]" << std::endl;
    settings_file << write_setting("CustomFilter", settings_opensave_custom_filter);
    settings_file << write_setting("OpenFormat", settings_opensave_open_format);
    settings_file << write_setting("OpenThumbnail", settings_opensave_open_thumbnail);
    settings_file << write_setting("SaveFormat", settings_opensave_save_format);
    settings_file << write_setting("SaveThumbnail", settings_opensave_save_thumbnail);
    settings_file << write_setting("RecentMax", settings_opensave_recent_max_files);
    // settings_file << write_setting("RecentFiles", settings_opensave_recent_list_of_files);
    settings_file << write_setting("RecentDirectory", settings_opensave_recent_directory);
    settings_file << write_setting("TrimDstNumJumps", settings_opensave_trim_dst_num_jumps);
    settings_file << std::endl;

    settings_file << "[Printing]" << std::endl;
    settings_file << write_setting("DefaultDevice", settings_printing_default_device);
    settings_file << write_setting("UseLastDevice", settings_printing_use_last_device);
    settings_file << write_setting("DisableBG", settings_printing_disable_bg);
    settings_file << std::endl;

    settings_file << "[Grid]" << std::endl;
    settings_file << write_setting("ShowOnLoad", settings_grid_show_on_load);
    settings_file << write_setting("ShowOrigin", settings_grid_show_origin);
    settings_file << write_setting("ColorMatchCrossHair", settings_grid_color_match_crosshair);
    settings_file << write_setting("Color", settings_grid_color);
    settings_file << write_setting("LoadFromFile", settings_grid_load_from_file);
    settings_file << write_setting("Type", settings_grid_type);
    settings_file << write_setting("CenterOnOrigin", settings_grid_center_on_origin);
    settings_file << write_setting("CenterX", settings_grid_center_x);
    settings_file << write_setting("CenterY", settings_grid_center_y);
    settings_file << write_setting("SizeX", settings_grid_size_x);
    settings_file << write_setting("SizeY", settings_grid_size_y);
    settings_file << write_setting("SpacingX", settings_grid_spacing_x);
    settings_file << write_setting("SpacingY", settings_grid_spacing_y);
    settings_file << write_setting("SizeRadius", settings_grid_size_radius);
    settings_file << write_setting("SpacingRadius", settings_grid_spacing_radius);
    settings_file << write_setting("SpacingAngle", settings_grid_spacing_angle);
    settings_file << std::endl;

    settings_file << "[Ruler]" << std::endl;
    settings_file << write_setting("ShowOnLoad", settings_ruler_show_on_load);
    settings_file << write_setting("Metric", settings_ruler_metric);
    settings_file << write_setting("Color", settings_ruler_color);
    settings_file << write_setting("PixelSize", settings_ruler_pixel_size);
    settings_file << std::endl;

    settings_file << "[QuickSnap]" << std::endl;
    settings_file << write_setting("Enabled", settings_qsnap_enabled);
    settings_file << write_setting("LocatorColor", settings_qsnap_locator_color);
    settings_file << write_setting("LocatorSize", settings_qsnap_locator_size);
    settings_file << write_setting("ApertureSize", settings_qsnap_aperture_size);
    settings_file << write_setting("EndPoint", settings_qsnap_endpoint);
    settings_file << write_setting("MidPoint", settings_qsnap_midpoint);
    settings_file << write_setting("Center", settings_qsnap_center);
    settings_file << write_setting("Node", settings_qsnap_node);
    settings_file << write_setting("Quadrant", settings_qsnap_quadrant);
    settings_file << write_setting("Intersection", settings_qsnap_intersection);
    settings_file << write_setting("Extension", settings_qsnap_extension);
    settings_file << write_setting("Insertion", settings_qsnap_insertion);
    settings_file << write_setting("Perpendicular", settings_qsnap_perpendicular);
    settings_file << write_setting("Tangent", settings_qsnap_tangent);
    settings_file << write_setting("Nearest", settings_qsnap_nearest);
    settings_file << write_setting("Apparent", settings_qsnap_apparent);
    settings_file << write_setting("Parallel", settings_qsnap_parallel);
    settings_file << std::endl;

    settings_file << "[LineWeight]" << std::endl;
    settings_file << write_setting("ShowLineWeight", settings_lwt_show_lwt);
    settings_file << write_setting("RealRender", settings_lwt_real_render);
    settings_file << write_setting("DefaultLineWeight", settings_lwt_default_lwt);
    settings_file << std::endl;

    settings_file << "[Selection]" << std::endl;
    settings_file << write_setting("PickFirst", settings_selection_mode_pickfirst);
    settings_file << write_setting("PickAdd", settings_selection_mode_pickadd);
    settings_file << write_setting("PickDrag", settings_selection_mode_pickdrag);
    settings_file << write_setting("CoolGripColor", settings_selection_coolgrip_color);
    settings_file << write_setting("HotGripColor", settings_selection_hotgrip_color);
    settings_file << write_setting("GripSize", settings_selection_grip_size);
    settings_file << write_setting("PickBoxSize", settings_selection_pickbox_size);
    settings_file << std::endl;

    settings_file << "[Text]" << std::endl;
    settings_file << write_setting("Font", settings_text_font);
    settings_file << write_setting("Size", settings_text_size);
    settings_file << write_setting("Angle", settings_text_angle);
    settings_file << write_setting("StyleBold", settings_text_style_bold);
    settings_file << write_setting("StyleItalic", settings_text_style_italic);
    settings_file << write_setting("StyleUnderline", settings_text_style_underline);
    settings_file << write_setting("StyleStrikeOut", settings_text_style_strikeout);
    settings_file << write_setting("StyleOverline", settings_text_style_overline);
    settings_file << std::endl;

    settings_file.close();
}

void MainWindow::settingsPrompt()
{
    settingsDialog("Prompt");
}

void MainWindow::settingsDialog(const QString& showTab)
{
    Settings_Dialog dialog(this, showTab, this);
    dialog.exec();
}
