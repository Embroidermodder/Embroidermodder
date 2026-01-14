#include "mainwindow.h"
#include "settings-dialog.h"
#include "state.h"

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
    QSettings settings(settingsPath, QSettings::IniFormat);
    QPoint pos = settings.value("Window/Position", QPoint(0, 0)).toPoint();
    QSize size = settings.value("Window/Size", QSize(800, 600)).toSize();

    layoutState = settings.value("LayoutState").toByteArray();
    if(!restoreState(layoutState))
    {
        qDebug("LayoutState NOT restored! Setting Default Layout...");
        //someToolBar->setVisible(true);
    }

    //General
    state.settings.general_language               = settings.value("Language",                                  "default").toString();
    state.settings.general_icon_theme             = settings.value("IconTheme",                                 "default").toString();
    state.settings.general_icon_size              = settings.value("IconSize",                                         16).toInt();
    state.settings.general_mdi_bg_use_logo        = settings.value("MdiBGUseLogo",                                   true).toBool();
    state.settings.general_mdi_bg_use_texture     = settings.value("MdiBGUseTexture",                                true).toBool();
    state.settings.general_mdi_bg_use_color       = settings.value("MdiBGUseColor",                                  true).toBool();
    state.settings.general_mdi_bg_logo            = settings.value("MdiBGLogo",       appDir + "/images/logo-spirals.png").toString();
    state.settings.general_mdi_bg_texture         = settings.value("MdiBGTexture", appDir + "/images/texture-spirals.png").toString();
    state.settings.general_mdi_bg_color           = settings.value("MdiBGColor",                        qRgb(192,192,192)).toInt();
    state.settings.general_tip_of_the_day         = settings.value("TipOfTheDay",                                    true).toBool();
    state.settings.general_current_tip            = settings.value("CurrentTip",                                        0).toInt();
    state.settings.general_system_help_browser    = settings.value("SystemHelpBrowser",                              true).toBool();
    //Display
    state.settings.display_use_opengl             = settings.value("Display/UseOpenGL",                             false).toBool();
    state.settings.display_renderhint_aa          = settings.value("Display/RenderHintAntiAlias",                   false).toBool();
    state.settings.display_renderhint_text_aa     = settings.value("Display/RenderHintTextAntiAlias",               false).toBool();
    state.settings.display_renderhint_smooth_pix  = settings.value("Display/RenderHintSmoothPixmap",                false).toBool();
    state.settings.display_renderhint_high_aa     = settings.value("Display/RenderHintHighQualityAntiAlias",        false).toBool();
    state.settings.display_renderhint_noncosmetic = settings.value("Display/RenderHintNonCosmetic",                 false).toBool();
    state.settings.display_show_scrollbars        = settings.value("Display/ShowScrollBars",                         true).toBool();
    state.settings.display_scrollbar_widget_num   = settings.value("Display/ScrollBarWidgetNum",                        0).toInt();
    state.settings.display_crosshair_color        = settings.value("Display/CrossHairColor",            qRgb(  0,  0,  0)).toInt();
    state.settings.display_bg_color               = settings.value("Display/BackgroundColor",           qRgb(235,235,235)).toInt();
    state.settings.display_selectbox_left_color   = settings.value("Display/SelectBoxLeftColor",        qRgb(  0,128,  0)).toInt();
    state.settings.display_selectbox_left_fill    = settings.value("Display/SelectBoxLeftFill",         qRgb(  0,255,  0)).toInt();
    state.settings.display_selectbox_right_color  = settings.value("Display/SelectBoxRightColor",       qRgb(  0,  0,128)).toInt();
    state.settings.display_selectbox_right_fill   = settings.value("Display/SelectBoxRightFill",        qRgb(  0,  0,255)).toInt();
    state.settings.display_selectbox_alpha        = settings.value("Display/SelectBoxAlpha",                           32).toInt();
    state.settings.display_zoomscale_in           = settings.value("Display/ZoomScaleIn",                             2.0).toFloat();
    state.settings.display_zoomscale_out          = settings.value("Display/ZoomScaleOut",                            0.5).toFloat();
    state.settings.display_crosshair_percent      = settings.value("Display/CrossHairPercent",                          5).toInt();
    state.settings.display_units                  = settings.value("Display/Units",                                  "mm").toString();
    //Prompt
    state.settings.prompt_text_color              = settings.value("Prompt/TextColor",                  qRgb(  0,  0,  0)).toInt();
    state.settings.prompt_bg_color                = settings.value("Prompt/BackgroundColor",            qRgb(255,255,255)).toInt();
    state.settings.prompt_font_family             = settings.value("Prompt/FontFamily",                       "Monospace").toString();
    state.settings.prompt_font_style              = settings.value("Prompt/FontStyle",                           "normal").toString();
    state.settings.prompt_font_size               = settings.value("Prompt/FontSize",                                  12).toInt();
    state.settings.prompt_save_history            = settings.value("Prompt/SaveHistory",                             true).toBool();
    state.settings.prompt_save_history_as_html    = settings.value("Prompt/SaveHistoryAsHtml",                      false).toBool();
    state.settings.prompt_save_history_filename   = settings.value("Prompt/SaveHistoryFilename",    settingsDir + "prompt.log").toString();
    //OpenSave
    state.settings.opensave_custom_filter         = settings.value("OpenSave/CustomFilter",                   "supported").toString();
    state.settings.opensave_open_format           = settings.value("OpenSave/OpenFormat",                           "*.*").toString();
    state.settings.opensave_open_thumbnail        = settings.value("OpenSave/OpenThumbnail",                        false).toBool();
    state.settings.opensave_save_format           = settings.value("OpenSave/SaveFormat",                           "*.*").toString();
    state.settings.opensave_save_thumbnail        = settings.value("OpenSave/SaveThumbnail",                        false).toBool();
    //Recent
    state.settings.opensave_recent_max_files      = settings.value("OpenSave/RecentMax",                               10).toInt();
    state.settings.opensave_recent_list_of_files  = settings.value("OpenSave/RecentFiles")                                .toStringList();
    state.settings.opensave_recent_directory      = settings.value("OpenSave/RecentDirectory",        appDir + "/samples").toString();
    //Trimming
    state.settings.opensave_trim_dst_num_jumps    = settings.value("OpenSave/TrimDstNumJumps",                          5).toInt();
    //Printing
    state.settings.printing_default_device        = settings.value("Printing/DefaultDevice",                           "").toString();
    state.settings.printing_use_last_device       = settings.value("Printing/UseLastDevice",                        false).toBool();
    state.settings.printing_disable_bg            = settings.value("Printing/DisableBG",                             true).toBool();
    //Grid
    state.settings.grid_show_on_load              = settings.value("Grid/ShowOnLoad",                                true).toBool();
    state.settings.grid_show_origin               = settings.value("Grid/ShowOrigin",                                true).toBool();
    state.settings.grid_color_match_crosshair     = settings.value("Grid/ColorMatchCrossHair",                       true).toBool();
    state.settings.grid_color                     = settings.value("Grid/Color",                        qRgb(  0,  0,  0)).toInt();
    state.settings.grid_load_from_file            = settings.value("Grid/LoadFromFile",                              true).toBool();
    state.settings.grid_type                      = settings.value("Grid/Type",                             "Rectangular").toString();
    state.settings.grid_center_on_origin          = settings.value("Grid/CenterOnOrigin",                            true).toBool();
    state.settings.grid_center_x                  = settings.value("Grid/CenterX",                                    0.0).toFloat();
    state.settings.grid_center_y                  = settings.value("Grid/CenterY",                                    0.0).toFloat();
    state.settings.grid_size_x                    = settings.value("Grid/SizeX",                                    100.0).toFloat();
    state.settings.grid_size_y                    = settings.value("Grid/SizeY",                                    100.0).toFloat();
    state.settings.grid_spacing_x                 = settings.value("Grid/SpacingX",                                  25.0).toFloat();
    state.settings.grid_spacing_y                 = settings.value("Grid/SpacingY",                                  25.0).toFloat();
    state.settings.grid_size_radius               = settings.value("Grid/SizeRadius",                                50.0).toFloat();
    state.settings.grid_spacing_radius            = settings.value("Grid/SpacingRadius",                             25.0).toFloat();
    state.settings.grid_spacing_angle             = settings.value("Grid/SpacingAngle",                              45.0).toFloat();
    //Ruler
    state.settings.ruler_show_on_load             = settings.value("Ruler/ShowOnLoad",                               true).toBool();
    state.settings.ruler_metric                   = settings.value("Ruler/Metric",                                   true).toBool();
    state.settings.ruler_color                    = settings.value("Ruler/Color",                       qRgb(210,210, 50)).toInt();
    state.settings.ruler_pixel_size               = settings.value("Ruler/PixelSize",                                  20).toInt();
    //Quick Snap
    state.settings.qsnap_enabled                  = settings.value("QuickSnap/Enabled",                              true).toBool();
    state.settings.qsnap_locator_color            = settings.value("QuickSnap/LocatorColor",            qRgb(255,255,  0)).toInt();
    state.settings.qsnap_locator_size             = settings.value("QuickSnap/LocatorSize",                             4).toInt();
    state.settings.qsnap_aperture_size            = settings.value("QuickSnap/ApertureSize",                           10).toInt();
    state.settings.qsnap_endpoint                 = settings.value("QuickSnap/EndPoint",                             true).toBool();
    state.settings.qsnap_midpoint                 = settings.value("QuickSnap/MidPoint",                             true).toBool();
    state.settings.qsnap_center                   = settings.value("QuickSnap/Center",                               true).toBool();
    state.settings.qsnap_node                     = settings.value("QuickSnap/Node",                                 true).toBool();
    state.settings.qsnap_quadrant                 = settings.value("QuickSnap/Quadrant",                             true).toBool();
    state.settings.qsnap_intersection             = settings.value("QuickSnap/Intersection",                         true).toBool();
    state.settings.qsnap_extension                = settings.value("QuickSnap/Extension",                            true).toBool();
    state.settings.qsnap_insertion                = settings.value("QuickSnap/Insertion",                           false).toBool();
    state.settings.qsnap_perpendicular            = settings.value("QuickSnap/Perpendicular",                        true).toBool();
    state.settings.qsnap_tangent                  = settings.value("QuickSnap/Tangent",                              true).toBool();
    state.settings.qsnap_nearest                  = settings.value("QuickSnap/Nearest",                             false).toBool();
    state.settings.qsnap_apparent                 = settings.value("QuickSnap/Apparent",                            false).toBool();
    state.settings.qsnap_parallel                 = settings.value("QuickSnap/Parallel",                            false).toBool();
    //LineWeight
    state.settings.lwt_show_lwt                   = settings.value("LineWeight/ShowLineWeight",                     false).toBool();
    state.settings.lwt_real_render                = settings.value("LineWeight/RealRender",                          true).toBool();
    state.settings.lwt_default_lwt                = settings.value("LineWeight/DefaultLineWeight",                      0).toReal();
    //Selection
    state.settings.selection_mode_pickfirst       = settings.value("Selection/PickFirst",                            true).toBool();
    state.settings.selection_mode_pickadd         = settings.value("Selection/PickAdd",                              true).toBool();
    state.settings.selection_mode_pickdrag        = settings.value("Selection/PickDrag",                            false).toBool();
    state.settings.selection_coolgrip_color       = settings.value("Selection/CoolGripColor",           qRgb(  0,  0,255)).toInt();
    state.settings.selection_hotgrip_color        = settings.value("Selection/HotGripColor",            qRgb(255,  0,  0)).toInt();
    state.settings.selection_grip_size            = settings.value("Selection/GripSize",                                4).toInt();
    state.settings.selection_pickbox_size         = settings.value("Selection/PickBoxSize",                             4).toInt();
    //Text
    state.settings.text_font                      = settings.value("Text/Font",                                   "Arial").toString();
    state.settings.text_size                      = settings.value("Text/Size",                                        12).toReal();
    state.settings.text_angle                     = settings.value("Text/Angle",                                        0).toReal();
    state.settings.text_style_bold                = settings.value("Text/StyleBold",                                false).toBool();
    state.settings.text_style_italic              = settings.value("Text/StyleItalic",                              false).toBool();
    state.settings.text_style_underline           = settings.value("Text/StyleUnderline",                           false).toBool();
    state.settings.text_style_strikeout           = settings.value("Text/StyleStrikeOut",                           false).toBool();
    state.settings.text_style_overline            = settings.value("Text/StyleOverline",                            false).toBool();

    move(pos);
    resize(size);
}

void MainWindow::writeSettings()
{
    qDebug("Writing Settings...");
    QString settingsPath = SettingsPath();
    // This file needs to be read from the users home directory to ensure it is writable
    QSettings settings(settingsPath, QSettings::IniFormat);
    QString tmp;
    settings.setValue("Window/Position", pos());
    settings.setValue("Window/Size", size());

    //General
    settings.setValue("LayoutState",                               layoutState);
    settings.setValue("Language",                                  state.settings.general_language);
    settings.setValue("IconTheme",                                 state.settings.general_icon_theme);
    settings.setValue("IconSize",                       tmp.setNum(state.settings.general_icon_size));
    settings.setValue("MdiBGUseLogo",                              state.settings.general_mdi_bg_use_logo);
    settings.setValue("MdiBGUseTexture",                           state.settings.general_mdi_bg_use_texture);
    settings.setValue("MdiBGUseColor",                             state.settings.general_mdi_bg_use_color);
    settings.setValue("MdiBGLogo",                                 state.settings.general_mdi_bg_logo);
    settings.setValue("MdiBGTexture",                              state.settings.general_mdi_bg_texture);
    settings.setValue("MdiBGColor",                     tmp.setNum(state.settings.general_mdi_bg_color));
    settings.setValue("TipOfTheDay",                               state.settings.general_tip_of_the_day);
    settings.setValue("CurrentTip",                     tmp.setNum(state.settings.general_current_tip + 1));
    settings.setValue("SystemHelpBrowser",                         state.settings.general_system_help_browser);
    //Display
    settings.setValue("Display/UseOpenGL",                         state.settings.display_use_opengl);
    settings.setValue("Display/RenderHintAntiAlias",               state.settings.display_renderhint_aa);
    settings.setValue("Display/RenderHintTextAntiAlias",           state.settings.display_renderhint_text_aa);
    settings.setValue("Display/RenderHintSmoothPixmap",            state.settings.display_renderhint_smooth_pix);
    settings.setValue("Display/RenderHintHighQualityAntiAlias",    state.settings.display_renderhint_high_aa);
    settings.setValue("Display/RenderHintNonCosmetic",             state.settings.display_renderhint_noncosmetic);
    settings.setValue("Display/ShowScrollBars",                    state.settings.display_show_scrollbars);
    settings.setValue("Display/ScrollBarWidgetNum",     tmp.setNum(state.settings.display_scrollbar_widget_num));
    settings.setValue("Display/CrossHairColor",         tmp.setNum(state.settings.display_crosshair_color));
    settings.setValue("Display/BackgroundColor",        tmp.setNum(state.settings.display_bg_color));
    settings.setValue("Display/SelectBoxLeftColor",     tmp.setNum(state.settings.display_selectbox_left_color));
    settings.setValue("Display/SelectBoxLeftFill",      tmp.setNum(state.settings.display_selectbox_left_fill));
    settings.setValue("Display/SelectBoxRightColor",    tmp.setNum(state.settings.display_selectbox_right_color));
    settings.setValue("Display/SelectBoxRightFill",     tmp.setNum(state.settings.display_selectbox_right_fill));
    settings.setValue("Display/SelectBoxAlpha",         tmp.setNum(state.settings.display_selectbox_alpha));
    settings.setValue("Display/ZoomScaleIn",            tmp.setNum(state.settings.display_zoomscale_in));
    settings.setValue("Display/ZoomScaleOut",           tmp.setNum(state.settings.display_zoomscale_out));
    settings.setValue("Display/CrossHairPercent",       tmp.setNum(state.settings.display_crosshair_percent));
    settings.setValue("Display/Units",                             state.settings.display_units);
    //Prompt
    settings.setValue("Prompt/TextColor",               tmp.setNum(state.settings.prompt_text_color));
    settings.setValue("Prompt/BackgroundColor",         tmp.setNum(state.settings.prompt_bg_color));
    settings.setValue("Prompt/FontFamily",                         state.settings.prompt_font_family);
    settings.setValue("Prompt/FontStyle",                          state.settings.prompt_font_style);
    settings.setValue("Prompt/FontSize",                tmp.setNum(state.settings.prompt_font_size));
    settings.setValue("Prompt/SaveHistory",                        state.settings.prompt_save_history);
    settings.setValue("Prompt/SaveHistoryAsHtml",                  state.settings.prompt_save_history_as_html);
    settings.setValue("Prompt/SaveHistoryFilename",                state.settings.prompt_save_history_filename);
    //OpenSave
    settings.setValue("OpenSave/CustomFilter",                     state.settings.opensave_custom_filter);
    settings.setValue("OpenSave/OpenFormat",                       state.settings.opensave_open_format);
    settings.setValue("OpenSave/OpenThumbnail",                    state.settings.opensave_open_thumbnail);
    settings.setValue("OpenSave/SaveFormat",                       state.settings.opensave_save_format);
    settings.setValue("OpenSave/SaveThumbnail",                    state.settings.opensave_save_thumbnail);
    //Recent
    settings.setValue("OpenSave/RecentMax",             tmp.setNum(state.settings.opensave_recent_max_files));
    settings.setValue("OpenSave/RecentFiles",                      state.settings.opensave_recent_list_of_files);
    settings.setValue("OpenSave/RecentDirectory",                  state.settings.opensave_recent_directory);
    //Trimming
    settings.setValue("OpenSave/TrimDstNumJumps",       tmp.setNum(state.settings.opensave_trim_dst_num_jumps));
    //Printing
    settings.setValue("Printing/DefaultDevice",                    state.settings.printing_default_device);
    settings.setValue("Printing/UseLastDevice",                    state.settings.printing_use_last_device);
    settings.setValue("Printing/DisableBG",                        state.settings.printing_disable_bg);
    //Grid
    settings.setValue("Grid/ShowOnLoad",                           state.settings.grid_show_on_load);
    settings.setValue("Grid/ShowOrigin",                           state.settings.grid_show_origin);
    settings.setValue("Grid/ColorMatchCrossHair",                  state.settings.grid_color_match_crosshair);
    settings.setValue("Grid/Color",                     tmp.setNum(state.settings.grid_color));
    settings.setValue("Grid/LoadFromFile",                         state.settings.grid_load_from_file);
    settings.setValue("Grid/Type",                                 state.settings.grid_type);
    settings.setValue("Grid/CenterOnOrigin",                       state.settings.grid_center_on_origin);
    settings.setValue("Grid/CenterX",                   tmp.setNum(state.settings.grid_center_x));
    settings.setValue("Grid/CenterY",                   tmp.setNum(state.settings.grid_center_y));
    settings.setValue("Grid/SizeX",                     tmp.setNum(state.settings.grid_size_x));
    settings.setValue("Grid/SizeY",                     tmp.setNum(state.settings.grid_size_y));
    settings.setValue("Grid/SpacingX",                  tmp.setNum(state.settings.grid_spacing_x));
    settings.setValue("Grid/SpacingY",                  tmp.setNum(state.settings.grid_spacing_y));
    settings.setValue("Grid/SizeRadius",                tmp.setNum(state.settings.grid_size_radius));
    settings.setValue("Grid/SpacingRadius",             tmp.setNum(state.settings.grid_spacing_radius));
    settings.setValue("Grid/SpacingAngle",              tmp.setNum(state.settings.grid_spacing_angle));
    //Ruler
    settings.setValue("Ruler/ShowOnLoad",                          state.settings.ruler_show_on_load);
    settings.setValue("Ruler/Metric",                              state.settings.ruler_metric);
    settings.setValue("Ruler/Color",                    tmp.setNum(state.settings.ruler_color));
    settings.setValue("Ruler/PixelSize",                tmp.setNum(state.settings.ruler_pixel_size));
    //Quick Snap
    settings.setValue("QuickSnap/Enabled",                         state.settings.qsnap_enabled);
    settings.setValue("QuickSnap/LocatorColor",         tmp.setNum(state.settings.qsnap_locator_color));
    settings.setValue("QuickSnap/LocatorSize",          tmp.setNum(state.settings.qsnap_locator_size));
    settings.setValue("QuickSnap/ApertureSize",         tmp.setNum(state.settings.qsnap_aperture_size));
    settings.setValue("QuickSnap/EndPoint",                        state.settings.qsnap_endpoint);
    settings.setValue("QuickSnap/MidPoint",                        state.settings.qsnap_midpoint);
    settings.setValue("QuickSnap/Center",                          state.settings.qsnap_center);
    settings.setValue("QuickSnap/Node",                            state.settings.qsnap_node);
    settings.setValue("QuickSnap/Quadrant",                        state.settings.qsnap_quadrant);
    settings.setValue("QuickSnap/Intersection",                    state.settings.qsnap_intersection);
    settings.setValue("QuickSnap/Extension",                       state.settings.qsnap_extension);
    settings.setValue("QuickSnap/Insertion",                       state.settings.qsnap_insertion);
    settings.setValue("QuickSnap/Perpendicular",                   state.settings.qsnap_perpendicular);
    settings.setValue("QuickSnap/Tangent",                         state.settings.qsnap_tangent);
    settings.setValue("QuickSnap/Nearest",                         state.settings.qsnap_nearest);
    settings.setValue("QuickSnap/Apparent",                        state.settings.qsnap_apparent);
    settings.setValue("QuickSnap/Parallel",                        state.settings.qsnap_parallel);
    //LineWeight
    settings.setValue("LineWeight/ShowLineWeight",                 state.settings.lwt_show_lwt);
    settings.setValue("LineWeight/RealRender",                     state.settings.lwt_real_render);
    settings.setValue("LineWeight/DefaultLineWeight",   tmp.setNum(state.settings.lwt_default_lwt));
    //Selection
    settings.setValue("Selection/PickFirst",                       state.settings.selection_mode_pickfirst);
    settings.setValue("Selection/PickAdd",                         state.settings.selection_mode_pickadd);
    settings.setValue("Selection/PickDrag",                        state.settings.selection_mode_pickdrag);
    settings.setValue("Selection/CoolGripColor",        tmp.setNum(state.settings.selection_coolgrip_color));
    settings.setValue("Selection/HotGripColor",         tmp.setNum(state.settings.selection_hotgrip_color));
    settings.setValue("Selection/GripSize",             tmp.setNum(state.settings.selection_grip_size));
    settings.setValue("Selection/PickBoxSize",          tmp.setNum(state.settings.selection_pickbox_size));
    //Text
    settings.setValue("Text/Font",                                 state.settings.text_font);
    settings.setValue("Text/Size",                      tmp.setNum(state.settings.text_size));
    settings.setValue("Text/Angle",                     tmp.setNum(state.settings.text_angle));
    settings.setValue("Text/StyleBold",                            state.settings.text_style_bold);
    settings.setValue("Text/StyleItalic",                          state.settings.text_style_italic);
    settings.setValue("Text/StyleUnderline",                       state.settings.text_style_underline);
    settings.setValue("Text/StyleStrikeOut",                       state.settings.text_style_strikeout);
    settings.setValue("Text/StyleOverline",                        state.settings.text_style_overline);
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

