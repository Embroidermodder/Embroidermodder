#include "mainwindow.h"
#include "settings-dialog.h"

#include <QDebug>
#include <QtGlobal>
#include <QSettings>

namespace {

// Note: on Unix we include the trailing separator. For Windows compatibility we omit it.
QString SettingsDir()
{
#if defined(Q_OS_UNIX) || defined(Q_OS_MAC)
  QString homePath = QDir::homePath();
  return homePath + "/.embroidermodder2/";
#else
  return "";
#endif
}

QString SettingsPath()
{
  QString settingsPath = SettingsDir() + "settings.ini";
  return settingsPath;
}

} // end anonymous namespace

void MainWindow::readSettings()
{
    qDebug("Reading Settings...");
    // This file needs to be read from the users home directory to ensure it is writable
    QString settingsPath = SettingsPath();
    QString settingsDir = SettingsDir();
    QString appDir = qApp->applicationDirPath();
    QSettings settings_file(settingsPath, QSettings::IniFormat);
    QPoint pos =  settings_file.value("Window/Position", QPoint(0, 0)).toPoint();
    QSize size =  settings_file.value("Window/Size", QSize(800, 600)).toSize();

    layoutState =  settings_file.value("LayoutState").toByteArray();
    if (!restoreState(layoutState)) {
        qDebug("LayoutState NOT restored! Setting Default Layout...");
        //someToolBar->setVisible(true);
    }

    //General
    settings.general_language               =  settings_file.value("Language",                                  "default").toString();
    settings.general_icon_theme             =  settings_file.value("IconTheme",                                 "default").toString();
    settings.general_icon_size              =  settings_file.value("IconSize",                                         16).toInt();
    settings.general_mdi_bg_use_logo        =  settings_file.value("MdiBGUseLogo",                                   true).toBool();
    settings.general_mdi_bg_use_texture     =  settings_file.value("MdiBGUseTexture",                                true).toBool();
    settings.general_mdi_bg_use_color       =  settings_file.value("MdiBGUseColor",                                  true).toBool();
    settings.general_mdi_bg_logo            =  settings_file.value("MdiBGLogo",       appDir + "/images/logo-spirals.png").toString();
    settings.general_mdi_bg_texture         =  settings_file.value("MdiBGTexture", appDir + "/images/texture-spirals.png").toString();
    settings.general_mdi_bg_color           =  settings_file.value("MdiBGColor",                        qRgb(192,192,192)).toInt();
    settings.general_tip_of_the_day         =  settings_file.value("TipOfTheDay",                                    true).toBool();
    settings.general_current_tip            =  settings_file.value("CurrentTip",                                        0).toInt();
    settings.general_system_help_browser    =  settings_file.value("SystemHelpBrowser",                              true).toBool();
    //Display
    settings.display_use_opengl             =  settings_file.value("Display/UseOpenGL",                             false).toBool();
    settings.display_renderhint_aa          =  settings_file.value("Display/RenderHintAntiAlias",                   false).toBool();
    settings.display_renderhint_text_aa     =  settings_file.value("Display/RenderHintTextAntiAlias",               false).toBool();
    settings.display_renderhint_smooth_pix  =  settings_file.value("Display/RenderHintSmoothPixmap",                false).toBool();
    settings.display_renderhint_high_aa     =  settings_file.value("Display/RenderHintHighQualityAntiAlias",        false).toBool();
    settings.display_renderhint_noncosmetic =  settings_file.value("Display/RenderHintNonCosmetic",                 false).toBool();
    settings.display_show_scrollbars        =  settings_file.value("Display/ShowScrollBars",                         true).toBool();
    settings.display_scrollbar_widget_num   =  settings_file.value("Display/ScrollBarWidgetNum",                        0).toInt();
    settings.display_crosshair_color        =  settings_file.value("Display/CrossHairColor",            qRgb(  0,  0,  0)).toInt();
    settings.display_bg_color               =  settings_file.value("Display/BackgroundColor",           qRgb(235,235,235)).toInt();
    settings.display_selectbox_left_color   =  settings_file.value("Display/SelectBoxLeftColor",        qRgb(  0,128,  0)).toInt();
    settings.display_selectbox_left_fill    =  settings_file.value("Display/SelectBoxLeftFill",         qRgb(  0,255,  0)).toInt();
    settings.display_selectbox_right_color  =  settings_file.value("Display/SelectBoxRightColor",       qRgb(  0,  0,128)).toInt();
    settings.display_selectbox_right_fill   =  settings_file.value("Display/SelectBoxRightFill",        qRgb(  0,  0,255)).toInt();
    settings.display_selectbox_alpha        =  settings_file.value("Display/SelectBoxAlpha",                           32).toInt();
    settings.display_zoomscale_in           =  settings_file.value("Display/ZoomScaleIn",                             2.0).toFloat();
    settings.display_zoomscale_out          =  settings_file.value("Display/ZoomScaleOut",                            0.5).toFloat();
    settings.display_crosshair_percent      =  settings_file.value("Display/CrossHairPercent",                          5).toInt();
    settings.display_units                  =  settings_file.value("Display/Units",                                  "mm").toString();
    //Prompt
    settings.prompt_text_color              =  settings_file.value("Prompt/TextColor",                  qRgb(  0,  0,  0)).toInt();
    settings.prompt_bg_color                =  settings_file.value("Prompt/BackgroundColor",            qRgb(255,255,255)).toInt();
    settings.prompt_font_family             =  settings_file.value("Prompt/FontFamily",                       "Monospace").toString();
    settings.prompt_font_style              =  settings_file.value("Prompt/FontStyle",                           "normal").toString();
    settings.prompt_font_size               =  settings_file.value("Prompt/FontSize",                                  12).toInt();
    settings.prompt_save_history            =  settings_file.value("Prompt/SaveHistory",                             true).toBool();
    settings.prompt_save_history_as_html    =  settings_file.value("Prompt/SaveHistoryAsHtml",                      false).toBool();
    settings.prompt_save_history_filename   =  settings_file.value("Prompt/SaveHistoryFilename",    settingsDir + "prompt.log").toString();
    //OpenSave
    settings.opensave_custom_filter         =  settings_file.value("OpenSave/CustomFilter",                   "supported").toString();
    settings.opensave_open_format           =  settings_file.value("OpenSave/OpenFormat",                           "*.*").toString();
    settings.opensave_open_thumbnail        =  settings_file.value("OpenSave/OpenThumbnail",                        false).toBool();
    settings.opensave_save_format           =  settings_file.value("OpenSave/SaveFormat",                           "*.*").toString();
    settings.opensave_save_thumbnail        =  settings_file.value("OpenSave/SaveThumbnail",                        false).toBool();
    //Recent
    settings.opensave_recent_max_files      =  settings_file.value("OpenSave/RecentMax",                               10).toInt();
    settings.opensave_recent_list_of_files  =  settings_file.value("OpenSave/RecentFiles")                                .toStringList();
    settings.opensave_recent_directory      =  settings_file.value("OpenSave/RecentDirectory",        appDir + "/samples").toString();
    //Trimming
    settings.opensave_trim_dst_num_jumps    =  settings_file.value("OpenSave/TrimDstNumJumps",                          5).toInt();
    //Printing
    settings.printing_default_device        =  settings_file.value("Printing/DefaultDevice",                           "").toString();
    settings.printing_use_last_device       =  settings_file.value("Printing/UseLastDevice",                        false).toBool();
    settings.printing_disable_bg            =  settings_file.value("Printing/DisableBG",                             true).toBool();
    //Grid
    settings.grid_show_on_load              =  settings_file.value("Grid/ShowOnLoad",                                true).toBool();
    settings.grid_show_origin               =  settings_file.value("Grid/ShowOrigin",                                true).toBool();
    settings.grid_color_match_crosshair     =  settings_file.value("Grid/ColorMatchCrossHair",                       true).toBool();
    settings.grid_color                     =  settings_file.value("Grid/Color",                        qRgb(  0,  0,  0)).toInt();
    settings.grid_load_from_file            =  settings_file.value("Grid/LoadFromFile",                              true).toBool();
    settings.grid_type                      =  settings_file.value("Grid/Type",                             "Rectangular").toString();
    settings.grid_center_on_origin          =  settings_file.value("Grid/CenterOnOrigin",                            true).toBool();
    settings.grid_center_x                  =  settings_file.value("Grid/CenterX",                                    0.0).toFloat();
    settings.grid_center_y                  =  settings_file.value("Grid/CenterY",                                    0.0).toFloat();
    settings.grid_size_x                    =  settings_file.value("Grid/SizeX",                                    100.0).toFloat();
    settings.grid_size_y                    =  settings_file.value("Grid/SizeY",                                    100.0).toFloat();
    settings.grid_spacing_x                 =  settings_file.value("Grid/SpacingX",                                  25.0).toFloat();
    settings.grid_spacing_y                 =  settings_file.value("Grid/SpacingY",                                  25.0).toFloat();
    settings.grid_size_radius               =  settings_file.value("Grid/SizeRadius",                                50.0).toFloat();
    settings.grid_spacing_radius            =  settings_file.value("Grid/SpacingRadius",                             25.0).toFloat();
    settings.grid_spacing_angle             =  settings_file.value("Grid/SpacingAngle",                              45.0).toFloat();
    //Ruler
    settings.ruler_show_on_load             =  settings_file.value("Ruler/ShowOnLoad",                               true).toBool();
    settings.ruler_metric                   =  settings_file.value("Ruler/Metric",                                   true).toBool();
    settings.ruler_color                    =  settings_file.value("Ruler/Color",                       qRgb(210,210, 50)).toInt();
    settings.ruler_pixel_size               =  settings_file.value("Ruler/PixelSize",                                  20).toInt();
    //Quick Snap
    settings.qsnap_enabled                  =  settings_file.value("QuickSnap/Enabled",                              true).toBool();
    settings.qsnap_locator_color            =  settings_file.value("QuickSnap/LocatorColor",            qRgb(255,255,  0)).toInt();
    settings.qsnap_locator_size             =  settings_file.value("QuickSnap/LocatorSize",                             4).toInt();
    settings.qsnap_aperture_size            =  settings_file.value("QuickSnap/ApertureSize",                           10).toInt();
    settings.qsnap_endpoint                 =  settings_file.value("QuickSnap/EndPoint",                             true).toBool();
    settings.qsnap_midpoint                 =  settings_file.value("QuickSnap/MidPoint",                             true).toBool();
    settings.qsnap_center                   =  settings_file.value("QuickSnap/Center",                               true).toBool();
    settings.qsnap_node                     =  settings_file.value("QuickSnap/Node",                                 true).toBool();
    settings.qsnap_quadrant                 =  settings_file.value("QuickSnap/Quadrant",                             true).toBool();
    settings.qsnap_intersection             =  settings_file.value("QuickSnap/Intersection",                         true).toBool();
    settings.qsnap_extension                =  settings_file.value("QuickSnap/Extension",                            true).toBool();
    settings.qsnap_insertion                =  settings_file.value("QuickSnap/Insertion",                           false).toBool();
    settings.qsnap_perpendicular            =  settings_file.value("QuickSnap/Perpendicular",                        true).toBool();
    settings.qsnap_tangent                  =  settings_file.value("QuickSnap/Tangent",                              true).toBool();
    settings.qsnap_nearest                  =  settings_file.value("QuickSnap/Nearest",                             false).toBool();
    settings.qsnap_apparent                 =  settings_file.value("QuickSnap/Apparent",                            false).toBool();
    settings.qsnap_parallel                 =  settings_file.value("QuickSnap/Parallel",                            false).toBool();
    //LineWeight
    settings.lwt_show_lwt                   =  settings_file.value("LineWeight/ShowLineWeight",                     false).toBool();
    settings.lwt_real_render                =  settings_file.value("LineWeight/RealRender",                          true).toBool();
    settings.lwt_default_lwt                =  settings_file.value("LineWeight/DefaultLineWeight",                      0).toReal();
    //Selection
    settings.selection_mode_pickfirst       =  settings_file.value("Selection/PickFirst",                            true).toBool();
    settings.selection_mode_pickadd         =  settings_file.value("Selection/PickAdd",                              true).toBool();
    settings.selection_mode_pickdrag        =  settings_file.value("Selection/PickDrag",                            false).toBool();
    settings.selection_coolgrip_color       =  settings_file.value("Selection/CoolGripColor",           qRgb(  0,  0,255)).toInt();
    settings.selection_hotgrip_color        =  settings_file.value("Selection/HotGripColor",            qRgb(255,  0,  0)).toInt();
    settings.selection_grip_size            =  settings_file.value("Selection/GripSize",                                4).toInt();
    settings.selection_pickbox_size         =  settings_file.value("Selection/PickBoxSize",                             4).toInt();
    //Text
    settings.text_font                      =  settings_file.value("Text/Font",                                   "Arial").toString();
    settings.text_size                      =  settings_file.value("Text/Size",                                        12).toReal();
    settings.text_angle                     =  settings_file.value("Text/Angle",                                        0).toReal();
    settings.text_style_bold                =  settings_file.value("Text/StyleBold",                                false).toBool();
    settings.text_style_italic              =  settings_file.value("Text/StyleItalic",                              false).toBool();
    settings.text_style_underline           =  settings_file.value("Text/StyleUnderline",                           false).toBool();
    settings.text_style_strikeout           =  settings_file.value("Text/StyleStrikeOut",                           false).toBool();
    settings.text_style_overline            =  settings_file.value("Text/StyleOverline",                            false).toBool();

    move(pos);
    resize(size);
}

void MainWindow::writeSettings()
{
    qDebug("Writing Settings...");
    QString settingsPath = SettingsPath();
    // This file needs to be read from the users home directory to ensure it is writable
    QSettings settings_file(settingsPath, QSettings::IniFormat);
    QString tmp;
     settings_file.setValue("Window/Position", pos());
     settings_file.setValue("Window/Size", size());

    //General
     settings_file.setValue("LayoutState", layoutState);
     settings_file.setValue("Language", settings.general_language);
     settings_file.setValue("IconTheme", settings.general_icon_theme);
     settings_file.setValue("IconSize",                       tmp.setNum(settings.general_icon_size));
     settings_file.setValue("MdiBGUseLogo",                              settings.general_mdi_bg_use_logo);
     settings_file.setValue("MdiBGUseTexture",                           settings.general_mdi_bg_use_texture);
     settings_file.setValue("MdiBGUseColor",                             settings.general_mdi_bg_use_color);
     settings_file.setValue("MdiBGLogo",                                 settings.general_mdi_bg_logo);
     settings_file.setValue("MdiBGTexture",                              settings.general_mdi_bg_texture);
     settings_file.setValue("MdiBGColor",                     tmp.setNum(settings.general_mdi_bg_color));
     settings_file.setValue("TipOfTheDay",                               settings.general_tip_of_the_day);
     settings_file.setValue("CurrentTip",                     tmp.setNum(settings.general_current_tip + 1));
     settings_file.setValue("SystemHelpBrowser",                         settings.general_system_help_browser);
    //Display
     settings_file.setValue("Display/UseOpenGL",                         settings.display_use_opengl);
     settings_file.setValue("Display/RenderHintAntiAlias",               settings.display_renderhint_aa);
     settings_file.setValue("Display/RenderHintTextAntiAlias",           settings.display_renderhint_text_aa);
     settings_file.setValue("Display/RenderHintSmoothPixmap",            settings.display_renderhint_smooth_pix);
     settings_file.setValue("Display/RenderHintHighQualityAntiAlias",    settings.display_renderhint_high_aa);
     settings_file.setValue("Display/RenderHintNonCosmetic",             settings.display_renderhint_noncosmetic);
     settings_file.setValue("Display/ShowScrollBars",                    settings.display_show_scrollbars);
     settings_file.setValue("Display/ScrollBarWidgetNum",     tmp.setNum(settings.display_scrollbar_widget_num));
     settings_file.setValue("Display/CrossHairColor",         tmp.setNum(settings.display_crosshair_color));
     settings_file.setValue("Display/BackgroundColor",        tmp.setNum(settings.display_bg_color));
     settings_file.setValue("Display/SelectBoxLeftColor",     tmp.setNum(settings.display_selectbox_left_color));
     settings_file.setValue("Display/SelectBoxLeftFill",      tmp.setNum(settings.display_selectbox_left_fill));
     settings_file.setValue("Display/SelectBoxRightColor",    tmp.setNum(settings.display_selectbox_right_color));
     settings_file.setValue("Display/SelectBoxRightFill",     tmp.setNum(settings.display_selectbox_right_fill));
     settings_file.setValue("Display/SelectBoxAlpha",         tmp.setNum(settings.display_selectbox_alpha));
     settings_file.setValue("Display/ZoomScaleIn",            tmp.setNum(settings.display_zoomscale_in));
     settings_file.setValue("Display/ZoomScaleOut",           tmp.setNum(settings.display_zoomscale_out));
     settings_file.setValue("Display/CrossHairPercent",       tmp.setNum(settings.display_crosshair_percent));
     settings_file.setValue("Display/Units",                             settings.display_units);
    //Prompt
     settings_file.setValue("Prompt/TextColor",               tmp.setNum(settings.prompt_text_color));
     settings_file.setValue("Prompt/BackgroundColor",         tmp.setNum(settings.prompt_bg_color));
     settings_file.setValue("Prompt/FontFamily",                         settings.prompt_font_family);
     settings_file.setValue("Prompt/FontStyle",                          settings.prompt_font_style);
     settings_file.setValue("Prompt/FontSize",                tmp.setNum(settings.prompt_font_size));
     settings_file.setValue("Prompt/SaveHistory",                        settings.prompt_save_history);
     settings_file.setValue("Prompt/SaveHistoryAsHtml",                  settings.prompt_save_history_as_html);
     settings_file.setValue("Prompt/SaveHistoryFilename",                settings.prompt_save_history_filename);
    //OpenSave
     settings_file.setValue("OpenSave/CustomFilter",                     settings.opensave_custom_filter);
     settings_file.setValue("OpenSave/OpenFormat",                       settings.opensave_open_format);
     settings_file.setValue("OpenSave/OpenThumbnail",                    settings.opensave_open_thumbnail);
     settings_file.setValue("OpenSave/SaveFormat",                       settings.opensave_save_format);
     settings_file.setValue("OpenSave/SaveThumbnail",                    settings.opensave_save_thumbnail);
    //Recent
     settings_file.setValue("OpenSave/RecentMax",             tmp.setNum(settings.opensave_recent_max_files));
     settings_file.setValue("OpenSave/RecentFiles",                      settings.opensave_recent_list_of_files);
     settings_file.setValue("OpenSave/RecentDirectory",                  settings.opensave_recent_directory);
    //Trimming
     settings_file.setValue("OpenSave/TrimDstNumJumps",       tmp.setNum(settings.opensave_trim_dst_num_jumps));
    //Printing
     settings_file.setValue("Printing/DefaultDevice",                    settings.printing_default_device);
     settings_file.setValue("Printing/UseLastDevice",                    settings.printing_use_last_device);
     settings_file.setValue("Printing/DisableBG",                        settings.printing_disable_bg);
    //Grid
     settings_file.setValue("Grid/ShowOnLoad",                           settings.grid_show_on_load);
     settings_file.setValue("Grid/ShowOrigin",                           settings.grid_show_origin);
     settings_file.setValue("Grid/ColorMatchCrossHair",                  settings.grid_color_match_crosshair);
     settings_file.setValue("Grid/Color",                     tmp.setNum(settings.grid_color));
     settings_file.setValue("Grid/LoadFromFile",                         settings.grid_load_from_file);
     settings_file.setValue("Grid/Type",                                 settings.grid_type);
     settings_file.setValue("Grid/CenterOnOrigin",                       settings.grid_center_on_origin);
     settings_file.setValue("Grid/CenterX",                   tmp.setNum(settings.grid_center_x));
     settings_file.setValue("Grid/CenterY",                   tmp.setNum(settings.grid_center_y));
     settings_file.setValue("Grid/SizeX",                     tmp.setNum(settings.grid_size_x));
     settings_file.setValue("Grid/SizeY",                     tmp.setNum(settings.grid_size_y));
     settings_file.setValue("Grid/SpacingX",                  tmp.setNum(settings.grid_spacing_x));
     settings_file.setValue("Grid/SpacingY",                  tmp.setNum(settings.grid_spacing_y));
     settings_file.setValue("Grid/SizeRadius",                tmp.setNum(settings.grid_size_radius));
     settings_file.setValue("Grid/SpacingRadius",             tmp.setNum(settings.grid_spacing_radius));
     settings_file.setValue("Grid/SpacingAngle",              tmp.setNum(settings.grid_spacing_angle));
    //Ruler
     settings_file.setValue("Ruler/ShowOnLoad",                          settings.ruler_show_on_load);
     settings_file.setValue("Ruler/Metric",                              settings.ruler_metric);
     settings_file.setValue("Ruler/Color",                    tmp.setNum(settings.ruler_color));
     settings_file.setValue("Ruler/PixelSize",                tmp.setNum(settings.ruler_pixel_size));
    //Quick Snap
     settings_file.setValue("QuickSnap/Enabled",                         settings.qsnap_enabled);
     settings_file.setValue("QuickSnap/LocatorColor",         tmp.setNum(settings.qsnap_locator_color));
     settings_file.setValue("QuickSnap/LocatorSize",          tmp.setNum(settings.qsnap_locator_size));
     settings_file.setValue("QuickSnap/ApertureSize",         tmp.setNum(settings.qsnap_aperture_size));
     settings_file.setValue("QuickSnap/EndPoint",                        settings.qsnap_endpoint);
     settings_file.setValue("QuickSnap/MidPoint",                        settings.qsnap_midpoint);
     settings_file.setValue("QuickSnap/Center",                          settings.qsnap_center);
     settings_file.setValue("QuickSnap/Node",                            settings.qsnap_node);
     settings_file.setValue("QuickSnap/Quadrant",                        settings.qsnap_quadrant);
     settings_file.setValue("QuickSnap/Intersection",                    settings.qsnap_intersection);
     settings_file.setValue("QuickSnap/Extension",                       settings.qsnap_extension);
     settings_file.setValue("QuickSnap/Insertion",                       settings.qsnap_insertion);
     settings_file.setValue("QuickSnap/Perpendicular",                   settings.qsnap_perpendicular);
     settings_file.setValue("QuickSnap/Tangent",                         settings.qsnap_tangent);
     settings_file.setValue("QuickSnap/Nearest",                         settings.qsnap_nearest);
     settings_file.setValue("QuickSnap/Apparent",                        settings.qsnap_apparent);
     settings_file.setValue("QuickSnap/Parallel",                        settings.qsnap_parallel);
    //LineWeight
     settings_file.setValue("LineWeight/ShowLineWeight",                 settings.lwt_show_lwt);
     settings_file.setValue("LineWeight/RealRender",                     settings.lwt_real_render);
     settings_file.setValue("LineWeight/DefaultLineWeight",   tmp.setNum(settings.lwt_default_lwt));
    //Selection
     settings_file.setValue("Selection/PickFirst",                       settings.selection_mode_pickfirst);
     settings_file.setValue("Selection/PickAdd",                         settings.selection_mode_pickadd);
     settings_file.setValue("Selection/PickDrag",                        settings.selection_mode_pickdrag);
     settings_file.setValue("Selection/CoolGripColor",        tmp.setNum(settings.selection_coolgrip_color));
     settings_file.setValue("Selection/HotGripColor",         tmp.setNum(settings.selection_hotgrip_color));
     settings_file.setValue("Selection/GripSize",             tmp.setNum(settings.selection_grip_size));
     settings_file.setValue("Selection/PickBoxSize",          tmp.setNum(settings.selection_pickbox_size));
    //Text
     settings_file.setValue("Text/Font",                                 settings.text_font);
     settings_file.setValue("Text/Size",                      tmp.setNum(settings.text_size));
     settings_file.setValue("Text/Angle",                     tmp.setNum(settings.text_angle));
     settings_file.setValue("Text/StyleBold",                            settings.text_style_bold);
     settings_file.setValue("Text/StyleItalic",                          settings.text_style_italic);
     settings_file.setValue("Text/StyleUnderline",                       settings.text_style_underline);
     settings_file.setValue("Text/StyleStrikeOut",                       settings.text_style_strikeout);
     settings_file.setValue("Text/StyleOverline",                        settings.text_style_overline);
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

