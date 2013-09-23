#include "mainwindow.h"
#include "settings-dialog.h"

#include <QDebug>
#include <QSettings>

void MainWindow::readSettings()
{
    qDebug("Reading Settings...");
    QSettings settings("settings.ini", QSettings::IniFormat);
    QPoint pos = settings.value("Window/Position", QPoint(0, 0)).toPoint();
    QSize size = settings.value("Window/Size", QSize(800, 600)).toSize();

    layoutState = settings.value("LayoutState").toByteArray();
    if(!restoreState(layoutState))
    {
        qDebug("LayoutState NOT restored! Setting Default Layout...");
        //someToolBar->setVisible(true);
    }

    //General
    settings_general_icon_theme             = settings.value("IconTheme",                           "default").toString();
    settings_general_icon_size              = settings.value("IconSize",                                   16).toInt();
    settings_general_mdi_bg_use_logo        = settings.value("MdiBGUseLogo",                             true).toBool();
    settings_general_mdi_bg_use_texture     = settings.value("MdiBGUseTexture",                          true).toBool();
    settings_general_mdi_bg_use_color       = settings.value("MdiBGUseColor",                            true).toBool();
    settings_general_mdi_bg_logo            = settings.value("MdiBGLogo",           "images/logo-spirals.png").toString();
    settings_general_mdi_bg_texture         = settings.value("MdiBGTexture",     "images/texture-spirals.png").toString();
    settings_general_mdi_bg_color           = settings.value("MdiBGColor",                  qRgb(192,192,192)).toInt();
    settings_general_tip_of_the_day         = settings.value("TipOfTheDay",                              true).toBool();
    settings_general_current_tip            = settings.value("CurrentTip",                                  0).toInt();
    //Display
    settings_display_use_opengl             = settings.value("Display/UseOpenGL",                       false).toBool();
    settings_display_renderhint_aa          = settings.value("Display/RenderHintAntiAlias",             false).toBool();
    settings_display_renderhint_text_aa     = settings.value("Display/RenderHintTextAntiAlias",         false).toBool();
    settings_display_renderhint_smooth_pix  = settings.value("Display/RenderHintSmoothPixmap",          false).toBool();
    settings_display_renderhint_high_aa     = settings.value("Display/RenderHintHighQualityAntiAlias",  false).toBool();
    settings_display_renderhint_noncosmetic = settings.value("Display/RenderHintNonCosmetic",           false).toBool();
    settings_display_show_scrollbars        = settings.value("Display/ShowScrollBars",                   true).toBool();
    settings_display_scrollbar_widget_num   = settings.value("Display/ScrollBarWidgetNum",                  0).toInt();
    settings_display_crosshair_color        = settings.value("Display/CrossHairColor",      qRgb(  0,  0,  0)).toInt();
    settings_display_bg_color               = settings.value("Display/BackgroundColor",     qRgb(235,235,235)).toInt();
    settings_display_selectbox_left_color   = settings.value("Display/SelectBoxLeftColor",  qRgb(  0,128,  0)).toInt();
    settings_display_selectbox_left_fill    = settings.value("Display/SelectBoxLeftFill",   qRgb(  0,255,  0)).toInt();
    settings_display_selectbox_right_color  = settings.value("Display/SelectBoxRightColor", qRgb(  0,  0,128)).toInt();
    settings_display_selectbox_right_fill   = settings.value("Display/SelectBoxRightFill",  qRgb(  0,  0,255)).toInt();
    settings_display_selectbox_alpha        = settings.value("Display/SelectBoxAlpha",                     32).toInt();
    settings_display_zoomscale_in           = settings.value("Display/ZoomScaleIn",                       2.0).toFloat();
    settings_display_zoomscale_out          = settings.value("Display/ZoomScaleOut",                      0.5).toFloat();
    settings_display_crosshair_percent      = settings.value("Display/CrossHairPercent",                    5).toInt();
    settings_display_units                  = settings.value("Display/Units",                            "mm").toString();
    //Prompt
    settings_prompt_text_color              = settings.value("Prompt/TextColor",            qRgb(  0,  0,  0)).toInt();
    settings_prompt_bg_color                = settings.value("Prompt/BackgroundColor",      qRgb(255,255,255)).toInt();
    settings_prompt_font_family             = settings.value("Prompt/FontFamily",                 "Monospace").toString();
    settings_prompt_font_style              = settings.value("Prompt/FontStyle",                     "normal").toString();
    settings_prompt_font_size               = settings.value("Prompt/FontSize",                            12).toInt();
    settings_prompt_save_history            = settings.value("Prompt/SaveHistory",                       true).toBool();
    settings_prompt_save_history_as_html    = settings.value("Prompt/SaveHistoryAsHtml",                false).toBool();
    settings_prompt_save_history_filename   = settings.value("Prompt/SaveHistoryFilename",       "prompt.log").toString();
    //OpenSave
    settings_opensave_custom_filter         = settings.value("OpenSave/CustomFilter",             "supported").toString();
    settings_opensave_open_format           = settings.value("OpenSave/OpenFormat",                     "*.*").toString();
    settings_opensave_open_thumbnail        = settings.value("OpenSave/OpenThumbnail",                  false).toBool();
    settings_opensave_save_format           = settings.value("OpenSave/SaveFormat",                     "*.*").toString();
    settings_opensave_save_thumbnail        = settings.value("OpenSave/SaveThumbnail",                  false).toBool();
    //Recent
    settings_opensave_recent_max_files      = settings.value("OpenSave/RecentMax",                         10).toInt();
    settings_opensave_recent_list_of_files  = settings.value("OpenSave/RecentFiles")                          .toStringList();
    settings_opensave_recent_directory      = settings.value("OpenSave/RecentDirectory")                      .toString();
    //Trimming
    settings_opensave_trim_dst_num_jumps    = settings.value("OpenSave/TrimDstNumJumps",                    5).toInt();
    //Printing
    settings_printing_default_device        = settings.value("Printing/DefaultDevice",                     "").toString();
    settings_printing_use_last_device       = settings.value("Printing/UseLastDevice",                  false).toBool();
    settings_printing_disable_bg            = settings.value("Printing/DisableBG",                       true).toBool();
    //Grid
    settings_grid_show_on_load              = settings.value("Grid/ShowOnLoad",                          true).toBool();
    settings_grid_show_origin               = settings.value("Grid/ShowOrigin",                          true).toBool();
    settings_grid_color_match_crosshair     = settings.value("Grid/ColorMatchCrossHair",                 true).toBool();
    settings_grid_color                     = settings.value("Grid/Color",                  qRgb(  0,  0,  0)).toInt();
    settings_grid_load_from_file            = settings.value("Grid/LoadFromFile",                        true).toBool();
    settings_grid_type                      = settings.value("Grid/Type",                       "Rectangular").toString();
    settings_grid_center_on_origin          = settings.value("Grid/CenterOnOrigin",                      true).toBool();
    settings_grid_center_x                  = settings.value("Grid/CenterX",                              0.0).toFloat();
    settings_grid_center_y                  = settings.value("Grid/CenterY",                              0.0).toFloat();
    settings_grid_size_x                    = settings.value("Grid/SizeX",                              100.0).toFloat();
    settings_grid_size_y                    = settings.value("Grid/SizeY",                              100.0).toFloat();
    settings_grid_spacing_x                 = settings.value("Grid/SpacingX",                            25.0).toFloat();
    settings_grid_spacing_y                 = settings.value("Grid/SpacingY",                            25.0).toFloat();
    settings_grid_size_radius               = settings.value("Grid/SizeRadius",                          50.0).toFloat();
    settings_grid_spacing_radius            = settings.value("Grid/SpacingRadius",                       25.0).toFloat();
    settings_grid_spacing_angle             = settings.value("Grid/SpacingAngle",                        45.0).toFloat();
    //Ruler
    settings_ruler_show_on_load             = settings.value("Ruler/ShowOnLoad",                         true).toBool();
    settings_ruler_metric                   = settings.value("Ruler/Metric",                             true).toBool();
    settings_ruler_color                    = settings.value("Ruler/Color",                 qRgb(210,210, 50)).toInt();
    settings_ruler_pixel_size               = settings.value("Ruler/PixelSize",                            20).toInt();
    //Quick Snap
    settings_qsnap_enabled                  = settings.value("QuickSnap/Enabled",                        true).toBool();
    settings_qsnap_locator_color            = settings.value("QuickSnap/LocatorColor",      qRgb(255,255,  0)).toInt();
    settings_qsnap_locator_size             = settings.value("QuickSnap/LocatorSize",                       4).toInt();
    settings_qsnap_aperture_size            = settings.value("QuickSnap/ApertureSize",                     10).toInt();
    settings_qsnap_endpoint                 = settings.value("QuickSnap/EndPoint",                       true).toBool();
    settings_qsnap_midpoint                 = settings.value("QuickSnap/MidPoint",                       true).toBool();
    settings_qsnap_center                   = settings.value("QuickSnap/Center",                         true).toBool();
    settings_qsnap_node                     = settings.value("QuickSnap/Node",                           true).toBool();
    settings_qsnap_quadrant                 = settings.value("QuickSnap/Quadrant",                       true).toBool();
    settings_qsnap_intersection             = settings.value("QuickSnap/Intersection",                   true).toBool();
    settings_qsnap_extension                = settings.value("QuickSnap/Extension",                      true).toBool();
    settings_qsnap_insertion                = settings.value("QuickSnap/Insertion",                     false).toBool();
    settings_qsnap_perpendicular            = settings.value("QuickSnap/Perpendicular",                  true).toBool();
    settings_qsnap_tangent                  = settings.value("QuickSnap/Tangent",                        true).toBool();
    settings_qsnap_nearest                  = settings.value("QuickSnap/Nearest",                       false).toBool();
    settings_qsnap_apparent                 = settings.value("QuickSnap/Apparent",                      false).toBool();
    settings_qsnap_parallel                 = settings.value("QuickSnap/Parallel",                      false).toBool();
    //LineWeight
    settings_lwt_show_lwt                   = settings.value("LineWeight/ShowLineWeight",               false).toBool();
    settings_lwt_real_render                = settings.value("LineWeight/RealRender",                    true).toBool();
    settings_lwt_default_lwt                = settings.value("LineWeight/DefaultLineWeight",                0).toReal();
    //Selection
    settings_selection_mode_pickfirst       = settings.value("Selection/PickFirst",                      true).toBool();
    settings_selection_mode_pickadd         = settings.value("Selection/PickAdd",                        true).toBool();
    settings_selection_mode_pickdrag        = settings.value("Selection/PickDrag",                      false).toBool();
    settings_selection_coolgrip_color       = settings.value("Selection/CoolGripColor",     qRgb(  0,  0,255)).toInt();
    settings_selection_hotgrip_color        = settings.value("Selection/HotGripColor",      qRgb(255,  0,  0)).toInt();
    settings_selection_grip_size            = settings.value("Selection/GripSize",                          4).toInt();
    settings_selection_pickbox_size         = settings.value("Selection/PickBoxSize",                       4).toInt();
    //Text
    settings_text_font                      = settings.value("Text/Font",                             "Arial").toString();
    settings_text_size                      = settings.value("Text/Size",                                  12).toReal();
    settings_text_angle                     = settings.value("Text/Angle",                                  0).toReal();
    settings_text_style_bold                = settings.value("Text/StyleBold",                          false).toBool();
    settings_text_style_italic              = settings.value("Text/StyleItalic",                        false).toBool();
    settings_text_style_underline           = settings.value("Text/StyleUnderline",                     false).toBool();
    settings_text_style_strikeout           = settings.value("Text/StyleStrikeOut",                     false).toBool();
    settings_text_style_overline            = settings.value("Text/StyleOverline",                      false).toBool();

    move(pos);
    resize(size);
}

void MainWindow::writeSettings()
{
    qDebug("Writing Settings...");
    QSettings settings("settings.ini", QSettings::IniFormat);
    QString tmp;
    settings.setValue("Window/Position", pos());
    settings.setValue("Window/Size", size());

    //General
    settings.setValue("LayoutState",                               layoutState);
    settings.setValue("IconTheme",                                 settings_general_icon_theme);
    settings.setValue("IconSize",                       tmp.setNum(settings_general_icon_size));
    settings.setValue("MdiBGUseLogo",                              settings_general_mdi_bg_use_logo);
    settings.setValue("MdiBGUseTexture",                           settings_general_mdi_bg_use_texture);
    settings.setValue("MdiBGUseColor",                             settings_general_mdi_bg_use_color);
    settings.setValue("MdiBGLogo",                                 settings_general_mdi_bg_logo);
    settings.setValue("MdiBGTexture",                              settings_general_mdi_bg_texture);
    settings.setValue("MdiBGColor",                     tmp.setNum(settings_general_mdi_bg_color));
    settings.setValue("TipOfTheDay",                               settings_general_tip_of_the_day);
    settings.setValue("CurrentTip",                     tmp.setNum(settings_general_current_tip + 1));
    //Display
    settings.setValue("Display/UseOpenGL",                         settings_display_use_opengl);
    settings.setValue("Display/RenderHintAntiAlias",               settings_display_renderhint_aa);
    settings.setValue("Display/RenderHintTextAntiAlias",           settings_display_renderhint_text_aa);
    settings.setValue("Display/RenderHintSmoothPixmap",            settings_display_renderhint_smooth_pix);
    settings.setValue("Display/RenderHintHighQualityAntiAlias",    settings_display_renderhint_high_aa);
    settings.setValue("Display/RenderHintNonCosmetic",             settings_display_renderhint_noncosmetic);
    settings.setValue("Display/ShowScrollBars",                    settings_display_show_scrollbars);
    settings.setValue("Display/ScrollBarWidgetNum",     tmp.setNum(settings_display_scrollbar_widget_num));
    settings.setValue("Display/CrossHairColor",         tmp.setNum(settings_display_crosshair_color));
    settings.setValue("Display/BackgroundColor",        tmp.setNum(settings_display_bg_color));
    settings.setValue("Display/SelectBoxLeftColor",     tmp.setNum(settings_display_selectbox_left_color));
    settings.setValue("Display/SelectBoxLeftFill",      tmp.setNum(settings_display_selectbox_left_fill));
    settings.setValue("Display/SelectBoxRightColor",    tmp.setNum(settings_display_selectbox_right_color));
    settings.setValue("Display/SelectBoxRightFill",     tmp.setNum(settings_display_selectbox_right_fill));
    settings.setValue("Display/SelectBoxAlpha",         tmp.setNum(settings_display_selectbox_alpha));
    settings.setValue("Display/ZoomScaleIn",            tmp.setNum(settings_display_zoomscale_in));
    settings.setValue("Display/ZoomScaleOut",           tmp.setNum(settings_display_zoomscale_out));
    settings.setValue("Display/CrossHairPercent",       tmp.setNum(settings_display_crosshair_percent));
    settings.setValue("Display/Units",                             settings_display_units);
    //Prompt
    settings.setValue("Prompt/TextColor",               tmp.setNum(settings_prompt_text_color));
    settings.setValue("Prompt/BackgroundColor",         tmp.setNum(settings_prompt_bg_color));
    settings.setValue("Prompt/FontFamily",                         settings_prompt_font_family);
    settings.setValue("Prompt/FontStyle",                          settings_prompt_font_style);
    settings.setValue("Prompt/FontSize",                tmp.setNum(settings_prompt_font_size));
    settings.setValue("Prompt/SaveHistory",                        settings_prompt_save_history);
    settings.setValue("Prompt/SaveHistoryAsHtml",                  settings_prompt_save_history_as_html);
    settings.setValue("Prompt/SaveHistoryFilename",                settings_prompt_save_history_filename);
    //OpenSave
    settings.setValue("OpenSave/CustomFilter",                     settings_opensave_custom_filter);
    settings.setValue("OpenSave/OpenFormat",                       settings_opensave_open_format);
    settings.setValue("OpenSave/OpenThumbnail",                    settings_opensave_open_thumbnail);
    settings.setValue("OpenSave/SaveFormat",                       settings_opensave_save_format);
    settings.setValue("OpenSave/SaveThumbnail",                    settings_opensave_save_thumbnail);
    //Recent
    settings.setValue("OpenSave/RecentMax",             tmp.setNum(settings_opensave_recent_max_files));
    settings.setValue("OpenSave/RecentFiles",                      settings_opensave_recent_list_of_files);
    settings.setValue("OpenSave/RecentDirectory",                  settings_opensave_recent_directory);
    //Trimming
    settings.setValue("OpenSave/TrimDstNumJumps",       tmp.setNum(settings_opensave_trim_dst_num_jumps));
    //Printing
    settings.setValue("Printing/DefaultDevice",                    settings_printing_default_device);
    settings.setValue("Printing/UseLastDevice",                    settings_printing_use_last_device);
    settings.setValue("Printing/DisableBG",                        settings_printing_disable_bg);
    //Grid
    settings.setValue("Grid/ShowOnLoad",                           settings_grid_show_on_load);
    settings.setValue("Grid/ShowOrigin",                           settings_grid_show_origin);
    settings.setValue("Grid/ColorMatchCrossHair",                  settings_grid_color_match_crosshair);
    settings.setValue("Grid/Color",                     tmp.setNum(settings_grid_color));
    settings.setValue("Grid/LoadFromFile",                         settings_grid_load_from_file);
    settings.setValue("Grid/Type",                                 settings_grid_type);
    settings.setValue("Grid/CenterOnOrigin",                       settings_grid_center_on_origin);
    settings.setValue("Grid/CenterX",                   tmp.setNum(settings_grid_center_x));
    settings.setValue("Grid/CenterY",                   tmp.setNum(settings_grid_center_y));
    settings.setValue("Grid/SizeX",                     tmp.setNum(settings_grid_size_x));
    settings.setValue("Grid/SizeY",                     tmp.setNum(settings_grid_size_y));
    settings.setValue("Grid/SpacingX",                  tmp.setNum(settings_grid_spacing_x));
    settings.setValue("Grid/SpacingY",                  tmp.setNum(settings_grid_spacing_y));
    settings.setValue("Grid/SizeRadius",                tmp.setNum(settings_grid_size_radius));
    settings.setValue("Grid/SpacingRadius",             tmp.setNum(settings_grid_spacing_radius));
    settings.setValue("Grid/SpacingAngle",              tmp.setNum(settings_grid_spacing_angle));
    //Ruler
    settings.setValue("Ruler/ShowOnLoad",                          settings_ruler_show_on_load);
    settings.setValue("Ruler/Metric",                              settings_ruler_metric);
    settings.setValue("Ruler/Color",                    tmp.setNum(settings_ruler_color));
    settings.setValue("Ruler/PixelSize",                tmp.setNum(settings_ruler_pixel_size));
    //Quick Snap
    settings.setValue("QuickSnap/Enabled",                         settings_qsnap_enabled);
    settings.setValue("QuickSnap/LocatorColor",         tmp.setNum(settings_qsnap_locator_color));
    settings.setValue("QuickSnap/LocatorSize",          tmp.setNum(settings_qsnap_locator_size));
    settings.setValue("QuickSnap/ApertureSize",         tmp.setNum(settings_qsnap_aperture_size));
    settings.setValue("QuickSnap/EndPoint",                        settings_qsnap_endpoint);
    settings.setValue("QuickSnap/MidPoint",                        settings_qsnap_midpoint);
    settings.setValue("QuickSnap/Center",                          settings_qsnap_center);
    settings.setValue("QuickSnap/Node",                            settings_qsnap_node);
    settings.setValue("QuickSnap/Quadrant",                        settings_qsnap_quadrant);
    settings.setValue("QuickSnap/Intersection",                    settings_qsnap_intersection);
    settings.setValue("QuickSnap/Extension",                       settings_qsnap_extension);
    settings.setValue("QuickSnap/Insertion",                       settings_qsnap_insertion);
    settings.setValue("QuickSnap/Perpendicular",                   settings_qsnap_perpendicular);
    settings.setValue("QuickSnap/Tangent",                         settings_qsnap_tangent);
    settings.setValue("QuickSnap/Nearest",                         settings_qsnap_nearest);
    settings.setValue("QuickSnap/Apparent",                        settings_qsnap_apparent);
    settings.setValue("QuickSnap/Parallel",                        settings_qsnap_parallel);
    //LineWeight
    settings.setValue("LineWeight/ShowLineWeight",                 settings_lwt_show_lwt);
    settings.setValue("LineWeight/RealRender",                     settings_lwt_real_render);
    settings.setValue("LineWeight/DefaultLineWeight",   tmp.setNum(settings_lwt_default_lwt));
    //Selection
    settings.setValue("Selection/PickFirst",                       settings_selection_mode_pickfirst);
    settings.setValue("Selection/PickAdd",                         settings_selection_mode_pickadd);
    settings.setValue("Selection/PickDrag",                        settings_selection_mode_pickdrag);
    settings.setValue("Selection/CoolGripColor",        tmp.setNum(settings_selection_coolgrip_color));
    settings.setValue("Selection/HotGripColor",         tmp.setNum(settings_selection_hotgrip_color));
    settings.setValue("Selection/GripSize",             tmp.setNum(settings_selection_grip_size));
    settings.setValue("Selection/PickBoxSize",          tmp.setNum(settings_selection_pickbox_size));
    //Text
    settings.setValue("Text/Font",                                 settings_text_font);
    settings.setValue("Text/Size",                      tmp.setNum(settings_text_size));
    settings.setValue("Text/Angle",                     tmp.setNum(settings_text_angle));
    settings.setValue("Text/StyleBold",                            settings_text_style_bold);
    settings.setValue("Text/StyleItalic",                          settings_text_style_italic);
    settings.setValue("Text/StyleUnderline",                       settings_text_style_underline);
    settings.setValue("Text/StyleStrikeOut",                       settings_text_style_strikeout);
    settings.setValue("Text/StyleOverline",                        settings_text_style_overline);
}

void MainWindow::settingsDialog(const QString& showTab)
{
    Settings_Dialog dialog(this, showTab, this);
    dialog.exec();
}

/* kate: bom off; indent-mode cstyle; indent-width 4; replace-trailing-space-save on; */
