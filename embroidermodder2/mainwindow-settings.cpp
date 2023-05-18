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
 * @brief SettingsPath
 * @return
 */
QString SettingsPath()
{
    QString settingsPath = SettingsDir() + "settings.ini";
    return settingsPath;
}

/**
 * @brief to_string_vector
 * @param list
 * @return
 */
std::vector<std::string>
to_string_vector(QStringList list)
{
    std::vector<std::string> a;
    for (int i=0; i<(int)list.size(); i++) {
        a.push_back(list[i].toStdString());
    }
    return a;
}

/**
 * @brief read_configuration
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
    if (!restoreState(layoutState))
    {
        qDebug("LayoutState NOT restored! Setting Default Layout...");
        //someToolBar->setVisible(true);
    }

    //General
    settings.general_language = settings_file.value("Language", "default").toString();
    settings.general_icon_theme = settings_file.value("IconTheme", "default").toString();
    settings.general_icon_size = settings_file.value("IconSize", 16).toInt();
    settings.general_mdi_bg_use_logo = settings_file.value("MdiBGUseLogo", true).toBool();
    settings.general_mdi_bg_use_texture = settings_file.value("MdiBGUseTexture", true).toBool();
    settings.general_mdi_bg_use_color = settings_file.value("MdiBGUseColor", true).toBool();
    settings.general_mdi_bg_logo = settings_file.value("MdiBGLogo", appDir + "/images/logo-spirals.png").toString();
    settings.general_mdi_bg_texture = settings_file.value("MdiBGTexture", appDir + "/images/texture-spirals.png").toString();
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
    settings.prompt_text_color = settings_file.value("Prompt/TextColor", qRgb(  0, 0, 0)).toInt();
    settings.prompt_bg_color = settings_file.value("Prompt/BackgroundColor", qRgb(255,255,255)).toInt();
    settings.prompt_font_family = settings_file.value("Prompt/FontFamily", "Monospace").toString();
    settings.prompt_font_style = settings_file.value("Prompt/FontStyle", "normal").toString();
    settings.prompt_font_size = settings_file.value("Prompt/FontSize", 12).toInt();
    settings.prompt_save_history = settings_file.value("Prompt/SaveHistory", true).toBool();
    settings.prompt_save_history_as_html = settings_file.value("Prompt/SaveHistoryAsHtml", false).toBool();
    settings.prompt_save_history_filename = settings_file.value("Prompt/SaveHistoryFilename", settingsDir + "prompt.log").toString();
    //OpenSave
    settings.opensave_custom_filter = settings_file.value("OpenSave/CustomFilter", "supported").toString();
    settings.opensave_open_format = settings_file.value("OpenSave/OpenFormat", "*.*").toString();
    settings.opensave_open_thumbnail = settings_file.value("OpenSave/OpenThumbnail", false).toBool();
    settings.opensave_save_format = settings_file.value("OpenSave/SaveFormat", "*.*").toString();
    settings.opensave_save_thumbnail = settings_file.value("OpenSave/SaveThumbnail", false).toBool();
    //Recent
    settings.opensave_recent_max_files = settings_file.value("OpenSave/RecentMax", 10).toInt();
    settings.opensave_recent_list_of_files = settings_file.value("OpenSave/RecentFiles")                                .toStringList();
    settings.opensave_recent_directory = settings_file.value("OpenSave/RecentDirectory", appDir + "/samples").toString();
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
    settings.grid_color = settings_file.value("Grid/Color", qRgb(  0, 0, 0)).toInt();
    settings.grid_load_from_file = settings_file.value("Grid/LoadFromFile", true).toBool();
    settings.grid_type = settings_file.value("Grid/Type", "Rectangular").toString();
    settings.grid_center_on_origin = settings_file.value("Grid/CenterOnOrigin", true).toBool();
    settings.grid_center.x = settings_file.value("Grid/CenterX", 0.0).toFloat();
    settings.grid_center.y = settings_file.value("Grid/CenterY", 0.0).toFloat();
    settings.grid_size.x = settings_file.value("Grid/SizeX", 100.0).toFloat();
    settings.grid_size.y = settings_file.value("Grid/SizeY", 100.0).toFloat();
    settings.grid_spacing.x = settings_file.value("Grid/SpacingX", 25.0).toFloat();
    settings.grid_spacing.y = settings_file.value("Grid/SpacingY", 25.0).toFloat();
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

/**
 * @brief write_setting
 * @param label
 * @param a
 * @return
 */
std::string
write_setting(std::string label, int a)
{
    char s[200];
    sprintf(s, "%s=%d\n", label.c_str(), a);
    std::string str(s);
    return str;
}

/**
 * @brief write_setting
 * @param label
 * @param a
 * @return
 */
std::string
write_setting(std::string label, QRgb a)
{
    char s[200];
    sprintf(s, "%s=%u\n", label.c_str(), a);
    std::string str(s);
    return str;
}

/**
 * @brief write_setting
 * @param label
 * @param a
 * @return
 */
std::string
write_setting(std::string label, QString a)
{
    return label + "=" + a.toStdString() + "\n";
}

/**
 * @brief write_setting
 * @param label
 * @param a
 * @return
 */
std::string
write_setting(std::string label, float a)
{
    char s[200];
    sprintf(s, "%s=%.6f\n", label.c_str(), a);
    std::string str(s);
    return str;
}

/**
 * @brief write_setting
 * @param label
 * @param a
 * @return
 */
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
    settings_file << write_setting("Language", settings.general_language);
    settings_file << write_setting("IconTheme", settings.general_icon_theme);
    settings_file << write_setting("IconSize", settings.general_icon_size);
    settings_file << write_setting("MdiBGUseLogo", settings.general_mdi_bg_use_logo);
    settings_file << write_setting("MdiBGUseTexture", settings.general_mdi_bg_use_texture);
    settings_file << write_setting("MdiBGUseColor", settings.general_mdi_bg_use_color);
    settings_file << write_setting("MdiBGLogo", settings.general_mdi_bg_logo);
    settings_file << write_setting("MdiBGTexture", settings.general_mdi_bg_texture);
    settings_file << write_setting("MdiBGColor", settings.general_mdi_bg_color);
    settings_file << write_setting("TipOfTheDay", settings.general_tip_of_the_day);
    settings_file << write_setting("CurrentTip", settings.general_current_tip + 1);
    settings_file << write_setting("SystemHelpBrowser", settings.general_system_help_browser);
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
    settings_file << write_setting("UseOpenGL", settings.display_use_opengl);
    settings_file << write_setting("RenderHintAntiAlias", settings.display_renderhint_aa);
    settings_file << write_setting("RenderHintTextAntiAlias", settings.display_renderhint_text_aa);
    settings_file << write_setting("RenderHintSmoothPixmap", settings.display_renderhint_smooth_pix);
    settings_file << write_setting("RenderHintHighQualityAntiAlias", settings.display_renderhint_high_aa);
    settings_file << write_setting("RenderHintNonCosmetic", settings.display_renderhint_noncosmetic);
    settings_file << write_setting("ShowScrollBars", settings.display_show_scrollbars);
    settings_file << write_setting("ScrollBarWidgetNum", settings.display_scrollbar_widget_num);
    settings_file << write_setting("CrossHairColor", settings.display_crosshair_color);
    settings_file << write_setting("BackgroundColor", settings.display_bg_color);
    settings_file << write_setting("SelectBoxLeftColor", settings.display_selectbox_left_color);
    settings_file << write_setting("SelectBoxLeftFill", settings.display_selectbox_left_fill);
    settings_file << write_setting("SelectBoxRightColor", settings.display_selectbox_right_color);
    settings_file << write_setting("SelectBoxRightFill", settings.display_selectbox_right_fill);
    settings_file << write_setting("SelectBoxAlpha", settings.display_selectbox_alpha);
    settings_file << write_setting("ZoomScaleIn", settings.display_zoomscale_in);
    settings_file << write_setting("ZoomScaleOut", settings.display_zoomscale_out);
    settings_file << write_setting("CrossHairPercent", settings.display_crosshair_percent);
    settings_file << write_setting("Units", settings.display_units);
    settings_file << std::endl;

    settings_file << "[Prompt]" << std::endl;
    settings_file << write_setting("TextColor", settings.prompt_text_color);
    settings_file << write_setting("BackgroundColor", settings.prompt_bg_color);
    settings_file << write_setting("FontFamily", settings.prompt_font_family);
    settings_file << write_setting("FontStyle", settings.prompt_font_style);
    settings_file << write_setting("FontSize", settings.prompt_font_size);
    settings_file << write_setting("SaveHistory", settings.prompt_save_history);
    settings_file << write_setting("SaveHistoryAsHtml", settings.prompt_save_history_as_html);
    settings_file << write_setting("SaveHistoryFilename", settings.prompt_save_history_filename);
    settings_file << std::endl;

    settings_file << "[OpenSave]" << std::endl;
    settings_file << write_setting("CustomFilter", settings.opensave_custom_filter);
    settings_file << write_setting("OpenFormat", settings.opensave_open_format);
    settings_file << write_setting("OpenThumbnail", settings.opensave_open_thumbnail);
    settings_file << write_setting("SaveFormat", settings.opensave_save_format);
    settings_file << write_setting("SaveThumbnail", settings.opensave_save_thumbnail);
    settings_file << write_setting("RecentMax", settings.opensave_recent_max_files);
    // settings_file << write_setting("RecentFiles", settings.opensave_recent_list_of_files);
    settings_file << write_setting("RecentDirectory", settings.opensave_recent_directory);
    settings_file << write_setting("TrimDstNumJumps", settings.opensave_trim_dst_num_jumps);
    settings_file << std::endl;

    settings_file << "[Printing]" << std::endl;
    settings_file << write_setting("DefaultDevice", settings.printing_default_device);
    settings_file << write_setting("UseLastDevice", settings.printing_use_last_device);
    settings_file << write_setting("DisableBG", settings.printing_disable_bg);
    settings_file << std::endl;

    settings_file << "[Grid]" << std::endl;
    settings_file << write_setting("ShowOnLoad", settings.grid_show_on_load);
    settings_file << write_setting("ShowOrigin", settings.grid_show_origin);
    settings_file << write_setting("ColorMatchCrossHair", settings.grid_color_match_crosshair);
    settings_file << write_setting("Color", settings.grid_color);
    settings_file << write_setting("LoadFromFile", settings.grid_load_from_file);
    settings_file << write_setting("Type", settings.grid_type);
    settings_file << write_setting("CenterOnOrigin", settings.grid_center_on_origin);
    settings_file << write_setting("CenterX", settings.grid_center.x);
    settings_file << write_setting("CenterY", settings.grid_center.y);
    settings_file << write_setting("SizeX", settings.grid_size.x);
    settings_file << write_setting("SizeY", settings.grid_size.y);
    settings_file << write_setting("SpacingX", settings.grid_spacing.x);
    settings_file << write_setting("SpacingY", settings.grid_spacing.y);
    settings_file << write_setting("SizeRadius", settings.grid_size_radius);
    settings_file << write_setting("SpacingRadius", settings.grid_spacing_radius);
    settings_file << write_setting("SpacingAngle", settings.grid_spacing_angle);
    settings_file << std::endl;

    settings_file << "[Ruler]" << std::endl;
    settings_file << write_setting("ShowOnLoad", settings.ruler_show_on_load);
    settings_file << write_setting("Metric", settings.ruler_metric);
    settings_file << write_setting("Color", settings.ruler_color);
    settings_file << write_setting("PixelSize", settings.ruler_pixel_size);
    settings_file << std::endl;

    settings_file << "[QuickSnap]" << std::endl;
    settings_file << write_setting("Enabled", settings.qsnap_enabled);
    settings_file << write_setting("LocatorColor", settings.qsnap_locator_color);
    settings_file << write_setting("LocatorSize", settings.qsnap_locator_size);
    settings_file << write_setting("ApertureSize", settings.qsnap_aperture_size);
    settings_file << write_setting("EndPoint", settings.qsnap_endpoint);
    settings_file << write_setting("MidPoint", settings.qsnap_midpoint);
    settings_file << write_setting("Center", settings.qsnap_center);
    settings_file << write_setting("Node", settings.qsnap_node);
    settings_file << write_setting("Quadrant", settings.qsnap_quadrant);
    settings_file << write_setting("Intersection", settings.qsnap_intersection);
    settings_file << write_setting("Extension", settings.qsnap_extension);
    settings_file << write_setting("Insertion", settings.qsnap_insertion);
    settings_file << write_setting("Perpendicular", settings.qsnap_perpendicular);
    settings_file << write_setting("Tangent", settings.qsnap_tangent);
    settings_file << write_setting("Nearest", settings.qsnap_nearest);
    settings_file << write_setting("Apparent", settings.qsnap_apparent);
    settings_file << write_setting("Parallel", settings.qsnap_parallel);
    settings_file << std::endl;

    settings_file << "[LineWeight]" << std::endl;
    settings_file << write_setting("ShowLineWeight", settings.lwt_show_lwt);
    settings_file << write_setting("RealRender", settings.lwt_real_render);
    settings_file << write_setting("DefaultLineWeight", settings.lwt_default_lwt);
    settings_file << std::endl;

    settings_file << "[Selection]" << std::endl;
    settings_file << write_setting("PickFirst", settings.selection_mode_pickfirst);
    settings_file << write_setting("PickAdd", settings.selection_mode_pickadd);
    settings_file << write_setting("PickDrag", settings.selection_mode_pickdrag);
    settings_file << write_setting("CoolGripColor", settings.selection_coolgrip_color);
    settings_file << write_setting("HotGripColor", settings.selection_hotgrip_color);
    settings_file << write_setting("GripSize", settings.selection_grip_size);
    settings_file << write_setting("PickBoxSize", settings.selection_pickbox_size);
    settings_file << std::endl;

    settings_file << "[Text]" << std::endl;
    settings_file << write_setting("Font", settings.text_font);
    settings_file << write_setting("Size", settings.text_size);
    settings_file << write_setting("Angle", settings.text_angle);
    settings_file << write_setting("StyleBold", settings.text_style_bold);
    settings_file << write_setting("StyleItalic", settings.text_style_italic);
    settings_file << write_setting("StyleUnderline", settings.text_style_underline);
    settings_file << write_setting("StyleStrikeOut", settings.text_style_strikeout);
    settings_file << write_setting("StyleOverline", settings.text_style_overline);
    settings_file << std::endl;

    settings_file.close();
}

/**
 * @brief MainWindow::settingsPrompt
 */
void
MainWindow::settingsPrompt()
{
    settingsDialog("Prompt");
}

/**
 * @brief MainWindow::settingsDialog
 * @param showTab
 */
void
MainWindow::settingsDialog(const QString& showTab)
{
    Settings_Dialog dialog(showTab);
    dialog.exec();
}
