/*
 * Embroidermodder 2.
 *
 * Copyright 2011-2024 The Embroidermodder Team
 * Embroidermodder 2 is Open Source Software, see LICENSE.md for licensing terms.
 * Visit https://www.libembroidery.org/refman for advice on altering this file,
 * or read the markdown version in embroidermodder2/docs/refman.
 *
 * View Commands
 */

#include "embroidermodder.h"

const char *settings_file = "settings.toml";

/* Note: on Unix we include the trailing separator. For Windows compatibility we
 * omit it.
 */
QString
SettingsDir()
{
#if defined(Q_OS_UNIX) || defined(Q_OS_MAC)
    return QDir::homePath() + "/.embroidermodder2/";
#else
    return "";
#endif
}

void
get_setting(QSettings *settings, const char *key, const char *value, StringSetting *s)
{
    QString k(key);
    QString v(value);
    strcpy(s->key, key);
    strcpy(s->setting, qPrintable(settings->value(k, v).toString()));
}

void
get_setting(QSettings *settings, const char *key, int value, IntSetting *i)
{
    strcpy(i->key, key);
    i->setting = settings->value(key, value).toInt();
}

void
get_setting(QSettings *settings, const char *key, bool value, BoolSetting *b)
{
    strcpy(b->key, key);
    b->setting = settings->value(key, value).toBool();
}

void
MainWindow::readSettings()
{
    qDebug("Reading Settings...");
    // This file needs to be read from the users home directory to ensure it is writable
    QString settingsDir = SettingsDir();
    QString appDir = qApp->applicationDirPath();
    //load_settings(qPrintable(appDir), qPrintable(SettingsDir()));

    QSettings settings(SettingsDir() + settings_file, QSettings::IniFormat);
    QPoint pos = settings.value("Window/Position", QPoint(0, 0)).toPoint();
    QSize size = settings.value("Window/Size", QSize(800, 600)).toSize();

    layoutState = settings.value("LayoutState").toByteArray();
    if (!restoreState(layoutState)) {
        qDebug("LayoutState NOT restored! Setting Default Layout...");
        //someToolBar->setVisible(true);
    }

    /* General */
    get_setting(&settings, "Language", "default", &general_language);
    get_setting(&settings, "IconTheme", "default", &general_icon_theme);
    get_setting(&settings, "IconSize", 16, &general_icon_size);
    get_setting(&settings, "MdiBGUseLogo", true, &general_mdi_bg_use_logo);
    get_setting(&settings, "MdiBGUseTexture", true, &general_mdi_bg_use_texture);
    get_setting(&settings, "MdiBGUseColor", true, &general_mdi_bg_use_color);
    get_setting(&settings, "MdiBGLogo", qPrintable(appDir + "/images/logo-spirals.png"), &general_mdi_bg_logo);
    get_setting(&settings, "MdiBGTexture", qPrintable(appDir + "/images/texture-spirals.png"), &general_mdi_bg_texture);
    get_setting(&settings, "MdiBGColor", qRgb(192,192,192), &general_mdi_bg_color);
    get_setting(&settings, "TipOfTheDay", true, &general_tip_of_the_day);
    get_setting(&settings, "CurrentTip", 0, &general_current_tip);
    get_setting(&settings, "SystemHelpBrowser", true, &general_system_help_browser);

    /* Display */
    get_setting(&settings, "Display/UseOpenGL", false, &display_use_opengl);
    get_setting(&settings, "Display/RenderHintAntiAlias", false, &display_renderhint_aa);
    display_renderhint_text_aa.setting = settings.value("Display/RenderHintTextAntiAlias", false).toBool();
    display_renderhint_smooth_pix.setting = settings.value("Display/RenderHintSmoothPixmap", false).toBool();
    display_renderhint_high_aa.setting = settings.value("Display/RenderHintHighQualityAntiAlias", false).toBool();
    display_renderhint_noncosmetic.setting = settings.value("Display/RenderHintNonCosmetic", false).toBool();
    display_show_scrollbars.setting = settings.value("Display/ShowScrollBars", true).toBool();
    display_scrollbar_widget_num.setting = settings.value("Display/ScrollBarWidgetNum", 0).toInt();
    display_crosshair_color.setting = settings.value("Display/CrossHairColor", qRgb(  0, 0, 0)).toInt();
    display_bg_color.setting = settings.value("Display/BackgroundColor", qRgb(235,235,235)).toInt();
    display_selectbox_left_color.setting = settings.value("Display/SelectBoxLeftColor", qRgb(  0,128, 0)).toInt();
    display_selectbox_left_fill.setting = settings.value("Display/SelectBoxLeftFill", qRgb(  0,255, 0)).toInt();
    display_selectbox_right_color.setting = settings.value("Display/SelectBoxRightColor", qRgb(  0, 0,128)).toInt();
    display_selectbox_right_fill.setting = settings.value("Display/SelectBoxRightFill", qRgb(  0, 0,255)).toInt();
    display_selectbox_alpha.setting = settings.value("Display/SelectBoxAlpha", 32).toInt();
    display_zoomscale_in.setting = settings.value("Display/ZoomScaleIn", 2.0).toFloat();
    display_zoomscale_out.setting = settings.value("Display/ZoomScaleOut", 0.5).toFloat();
    display_crosshair_percent.setting = settings.value("Display/CrossHairPercent", 5).toInt();
    strcpy(display_units.setting, qPrintable(settings.value("Display/Units", "mm").toString()));

    /* Prompt */
    prompt_text_color.setting = settings.value("Prompt/TextColor", qRgb(0, 0, 0)).toInt();
    prompt_bg_color.setting = settings.value("Prompt/BackgroundColor", qRgb(255, 255, 255)).toInt();
    strcpy(prompt_font_family.setting,
        qPrintable(settings.value("Prompt/FontFamily", "Monospace").toString()));
    strcpy(prompt_font_style.setting,
        qPrintable(settings.value("Prompt/FontStyle", "normal").toString()));
    prompt_font_size.setting = settings.value("Prompt/FontSize", 12).toInt();
    prompt_save_history.setting = settings.value("Prompt/SaveHistory", true).toBool();
    prompt_save_history_as_html.setting = settings.value("Prompt/SaveHistoryAsHtml", false).toBool();
    strcpy(prompt_save_history_filename.setting,
        qPrintable(settings.value("Prompt/SaveHistoryFilename", settingsDir + "prompt.log").toString()));

    /* OpenSave */
    strcpy(opensave_custom_filter.setting,
        qPrintable(settings.value("OpenSave/CustomFilter", "supported").toString()));
    strcpy(opensave_open_format.setting,
        qPrintable(settings.value("OpenSave/OpenFormat", "*.*").toString()));
    opensave_open_thumbnail.setting = settings.value("OpenSave/OpenThumbnail", false).toBool();
    strcpy(opensave_save_format.setting,
        qPrintable(settings.value("OpenSave/SaveFormat", "*.*").toString()));
    opensave_save_thumbnail.setting = settings.value("OpenSave/SaveThumbnail", false).toBool();

    /* Recent */
    opensave_recent_max_files.setting = settings.value("OpenSave/RecentMax", 10).toInt();
    QStringList sl = settings.value("OpenSave/RecentFiles").toStringList();
    /*
    int i;
    for (i=0; i<MAX_FILES-1; i++) {
        strncpy(opensave_recent_list_of_files.setting[i], qPrintable(sl[i]),
            MAX_STRING_LENGTH);
    }
    strncpy(opensave_recent_list_of_files.setting[i], "END", MAX_STRING_LENGTH);
    strncpy(opensave_recent_directory.setting,
        qPrintable(settings.value("OpenSave/RecentDirectory", appDir + "/samples").toString()),
        MAX_STRING_LENGTH);
    */
    strncpy(opensave_recent_list_of_files.setting[0], "END", MAX_STRING_LENGTH);

    /* Trimming */
    opensave_trim_dst_num_jumps.setting = settings.value("OpenSave/TrimDstNumJumps", 5).toInt();

    /* Printing */
    strcpy(printing_default_device.setting, qPrintable(settings.value("Printing/DefaultDevice", "").toString()));
    printing_use_last_device.setting = settings.value("Printing/UseLastDevice", false).toBool();
    printing_disable_bg.setting = settings.value("Printing/DisableBG", true).toBool();

    /* Grid */
    grid_show_on_load.setting = settings.value("Grid/ShowOnLoad", true).toBool();
    grid_show_origin.setting = settings.value("Grid/ShowOrigin", true).toBool();
    grid_color_match_crosshair.setting = settings.value("Grid/ColorMatchCrossHair", true).toBool();
    grid_color.setting = settings.value("Grid/Color", qRgb(  0, 0, 0)).toInt();
    grid_load_from_file.setting = settings.value("Grid/LoadFromFile", true).toBool();
    strcpy(grid_type.setting, qPrintable(settings.value("Grid/Type", "Rectangular").toString()));
    grid_center_on_origin.setting = settings.value("Grid/CenterOnOrigin", true).toBool();
    grid_center_x.setting = settings.value("Grid/CenterX", 0.0).toFloat();
    grid_center_y.setting = settings.value("Grid/CenterY", 0.0).toFloat();
    grid_size_x.setting = settings.value("Grid/SizeX", 100.0).toFloat();
    grid_size_y.setting = settings.value("Grid/SizeY", 100.0).toFloat();
    grid_spacing_x.setting = settings.value("Grid/SpacingX", 25.0).toFloat();
    grid_spacing_y.setting = settings.value("Grid/SpacingY", 25.0).toFloat();
    grid_size_radius.setting = settings.value("Grid/SizeRadius", 50.0).toFloat();
    grid_spacing_radius.setting = settings.value("Grid/SpacingRadius", 25.0).toFloat();
    grid_spacing_angle.setting = settings.value("Grid/SpacingAngle", 45.0).toFloat();

    /* Ruler */
    ruler_show_on_load.setting = settings.value("Ruler/ShowOnLoad", true).toBool();
    ruler_metric.setting = settings.value("Ruler/Metric", true).toBool();
    ruler_color.setting = settings.value("Ruler/Color", qRgb(210,210, 50)).toInt();
    ruler_pixel_size.setting = settings.value("Ruler/PixelSize", 20).toInt();

    /* Quick Snap */
    qsnap_enabled.setting = settings.value("QuickSnap/Enabled", true).toBool();
    qsnap_locator_color.setting = settings.value("QuickSnap/LocatorColor", qRgb(255,255, 0)).toInt();
    qsnap_locator_size.setting = settings.value("QuickSnap/LocatorSize", 4).toInt();
    qsnap_aperture_size.setting = settings.value("QuickSnap/ApertureSize", 10).toInt();
    qsnap_endpoint.setting = settings.value("QuickSnap/EndPoint", true).toBool();
    qsnap_midpoint.setting = settings.value("QuickSnap/MidPoint", true).toBool();
    qsnap_center.setting = settings.value("QuickSnap/Center", true).toBool();
    qsnap_node.setting = settings.value("QuickSnap/Node", true).toBool();
    qsnap_quadrant.setting = settings.value("QuickSnap/Quadrant", true).toBool();
    qsnap_intersection.setting = settings.value("QuickSnap/Intersection", true).toBool();
    qsnap_extension.setting = settings.value("QuickSnap/Extension", true).toBool();
    qsnap_insertion.setting = settings.value("QuickSnap/Insertion", false).toBool();
    qsnap_perpendicular.setting = settings.value("QuickSnap/Perpendicular", true).toBool();
    qsnap_tangent.setting = settings.value("QuickSnap/Tangent", true).toBool();
    qsnap_nearest.setting = settings.value("QuickSnap/Nearest", false).toBool();
    qsnap_apparent.setting = settings.value("QuickSnap/Apparent", false).toBool();
    qsnap_parallel.setting = settings.value("QuickSnap/Parallel", false).toBool();

    /* LineWeight */
    lwt_show_lwt.setting = settings.value("LineWeight/ShowLineWeight", false).toBool();
    lwt_real_render.setting = settings.value("LineWeight/RealRender", true).toBool();
    lwt_default_lwt.setting = settings.value("LineWeight/DefaultLineWeight", 0).toReal();

    /* Selection */
    selection_mode_pickfirst.setting = settings.value("Selection/PickFirst", true).toBool();
    selection_mode_pickadd.setting = settings.value("Selection/PickAdd", true).toBool();
    selection_mode_pickdrag.setting = settings.value("Selection/PickDrag", false).toBool();
    selection_coolgrip_color.setting = settings.value("Selection/CoolGripColor", qRgb(  0, 0,255)).toInt();
    selection_hotgrip_color.setting = settings.value("Selection/HotGripColor", qRgb(255, 0, 0)).toInt();
    selection_grip_size.setting = settings.value("Selection/GripSize", 4).toInt();
    selection_pickbox_size.setting = settings.value("Selection/PickBoxSize", 4).toInt();

    /* Text */
    strcpy(text_font.setting, qPrintable(settings.value("Text/Font", "Arial").toString()));
    text_size.setting = settings.value("Text/Size", 12).toReal();
    text_angle.setting = settings.value("Text/Angle", 0).toReal();
    text_style_bold.setting = settings.value("Text/StyleBold", false).toBool();
    text_style_italic.setting = settings.value("Text/StyleItalic", false).toBool();
    text_style_underline.setting = settings.value("Text/StyleUnderline", false).toBool();
    text_style_strikeout.setting = settings.value("Text/StyleStrikeOut", false).toBool();
    text_style_overline.setting = settings.value("Text/StyleOverline", false).toBool();

    move(pos);
    resize(size);
}

void
MainWindow::writeSettings()
{
    qDebug("Writing Settings...");
    save_settings("", qPrintable(SettingsDir() + settings_file));
}

void
MainWindow::settingsPrompt()
{
    settingsDialog("Prompt");
}

void
MainWindow::settingsDialog(const QString& showTab)
{
    Settings_Dialog dialog(this, showTab, this);
    dialog.exec();
}
