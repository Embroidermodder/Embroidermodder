/**
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

#include "embroidermodder.h"

#include <fstream>

/**
 * The actuator changes the program state via these global variables.
 */
Settings settings;

/**
 * These copies of the settings struct are for restoring the state if
 * the user doesn't want to accept their changes in the settings dialog.
 */
Settings dialog, preview, accept_;
Dictionary settings_, dialog_, preview_, accept__;

bool qsnap_endpoint_cb, qsnap_midpoint_cb, qsnap_center_cb,
    qsnap_node_cb, qsnap_quadrant_cb, qsnap_intersection_cb,
    qsnap_extension_cb, qsnap_insertion_cb, qsnap_perpendicular_cb,
    qsnap_tangent_cb, qsnap_nearest_cb, qsnap_apparent_cb,
    qsnap_parallel_cb;

/**
 * .
 */
StringList extensions = {
    "100", "10o", "ART", "BMC", "BRO",
    "CND", "COL", "CSD", "CSV", "DAT",
    "DEM", "DSB", "DST", "DSZ", "DXF",
    "EDR", "EMD", "EXP", "EXY", "EYS",
    "FXY", "GNC", "GT", "HUS", "INB",
    "JEF", "KSM", "PCD", "PCM", "PCQ",
    "PCS", "PEC", "PEL", "PEM", "PES",
    "PHB", "PHC", "RGB", "SEW", "SHV",
    "SST", "STX", "SVG", "T09", "TAP",
    "THR", "TXT", "U00", "U01", "VIP",
    "VP3", "XXX", "ZSK"
};

/**
 * @brief read_settings
 *
 * This file needs to be read from the users home directory to ensure it is writable.
 */
void
read_settings(void)
{
    debug_message("Reading Settings...");

    QString appDir = qApp->applicationDirPath();
#if defined(Q_OS_UNIX) || defined(Q_OS_MAC)
    QString settingsPath = QDir::homePath() + "/.embroidermodder2/settings.ini";
#else
    QString settingsPath = appDir + "/settings.ini";
#endif

    QSettings settings_file(settingsPath, QSettings::IniFormat);

    /*
    layoutState = settings_file.value("LayoutState").toByteArray();
    if (!restoreState(layoutState)) {
        debug_message("LayoutState NOT restored! Setting Default Layout...");
        //someToolBar->setVisible(true);
     }
    */

    //General
    settings.general_language = settings_file.value("Language", "default").toString().toStdString();
    settings.general_icon_theme = settings_file.value("IconTheme", "default").toString().toStdString();
    settings.general_icon_size = settings_file.value("IconSize", 16).toInt();
    settings.general_mdi_bg_use_logo = settings_file.value("MdiBGUseLogo", true).toBool();
    settings.general_mdi_bg_use_texture = settings_file.value("MdiBGUseTexture", true).toBool();
    settings.general_mdi_bg_use_color = settings_file.value("MdiBGUseColor", true).toBool();
    settings_["general_mdi_bg_logo"] = node(settings_file.value("MdiBGLogo", appDir + "/images/logo-spirals.png").toString().toStdString());
    settings.general_mdi_bg_texture = settings_file.value("MdiBGTexture", appDir + "/images/texture-spirals.png").toString().toStdString();
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
    settings_["display_units"] = node(settings_file.value("Display/Units", "mm").toString().toStdString());

    //Prompt
    settings.prompt_text_color = settings_file.value("Prompt/TextColor", qRgb(  0, 0, 0)).toInt();
    settings.prompt_bg_color = settings_file.value("Prompt/BackgroundColor", qRgb(255,255,255)).toInt();
    settings.prompt_font_family = settings_file.value("Prompt/FontFamily", "Monospace").toString().toStdString();
    settings.prompt_font_style = settings_file.value("Prompt/FontStyle", "normal").toString().toStdString();
    settings.prompt_font_size = settings_file.value("Prompt/FontSize", 12).toInt();
    settings.prompt_save_history = settings_file.value("Prompt/SaveHistory", true).toBool();
    settings.prompt_save_history_as_html = settings_file.value("Prompt/SaveHistoryAsHtml", false).toBool();

#if defined(Q_OS_UNIX) || defined(Q_OS_MAC)
    settings.prompt_save_history_filename = settings_file.value("Prompt/SaveHistoryFilename", QDir::homePath() + "/.embroidermodder2/prompt.log").toString().toStdString();
#else
    settings.prompt_save_history_filename = settings_file.value("Prompt/SaveHistoryFilename", appDir + "prompt.log").toString().toStdString();
#endif

    //OpenSave
    settings.opensave_custom_filter = settings_file.value("OpenSave/CustomFilter", "supported").toString();
    settings.opensave_open_format = settings_file.value("OpenSave/OpenFormat", "*.*").toString().toStdString();
    settings.opensave_open_thumbnail = settings_file.value("OpenSave/OpenThumbnail", false).toBool();
    settings.opensave_save_format = settings_file.value("OpenSave/SaveFormat", "*.*").toString().toStdString();
    settings.opensave_save_thumbnail = settings_file.value("OpenSave/SaveThumbnail", false).toBool();

    //Recent
    settings.opensave_recent_max_files = settings_file.value("OpenSave/RecentMax", 10).toInt();
    settings.opensave_recent_list_of_files = settings_file.value("OpenSave/RecentFiles").toStringList();
    settings.opensave_recent_directory = settings_file.value("OpenSave/RecentDirectory", appDir + "/samples").toString().toStdString();

    //Trimming
    settings.opensave_trim_dst_num_jumps = settings_file.value("OpenSave/TrimDstNumJumps", 5).toInt();

    //Printing
    settings.printing_default_device = settings_file.value("Printing/DefaultDevice", "").toString().toStdString();
    settings.printing_use_last_device = settings_file.value("Printing/UseLastDevice", false).toBool();
    settings.printing_disable_bg = settings_file.value("Printing/DisableBG", true).toBool();

    //Grid
    settings.grid_show_on_load = settings_file.value("Grid/ShowOnLoad", true).toBool();
    settings.grid_show_origin = settings_file.value("Grid/ShowOrigin", true).toBool();
    settings.grid_color_match_crosshair = settings_file.value("Grid/ColorMatchCrossHair", true).toBool();
    settings.grid_color = settings_file.value("Grid/Color", qRgb(  0, 0, 0)).toInt();
    settings.grid_load_from_file = settings_file.value("Grid/LoadFromFile", true).toBool();
    settings_["grid_type"] = node(settings_file.value("Grid/Type", "Rectangular").toString().toStdString());
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
    settings.text_font = settings_file.value("Text/Font", "Arial").toString().toStdString();
    settings.text_size = settings_file.value("Text/Size", 12).toReal();
    settings.text_angle = settings_file.value("Text/Angle", 0).toReal();
    settings.text_style_bold = settings_file.value("Text/StyleBold", false).toBool();
    settings.text_style_italic = settings_file.value("Text/StyleItalic", false).toBool();
    settings.text_style_underline = settings_file.value("Text/StyleUnderline", false).toBool();
    settings.text_style_strikeout = settings_file.value("Text/StyleStrikeOut", false).toBool();
    settings.text_style_overline = settings_file.value("Text/StyleOverline", false).toBool();

#if defined(Q_OS_UNIX) || defined(Q_OS_MAC)
    QString fname = QDir::homePath() + "/.embroidermodder2/settings.toml";
#else
    QString fname = qApp->applicationDirPath() + "/settings.toml";
#endif

    char error_buffer[200];
    FILE *f = fopen(fname.toStdString().c_str(), "r");
    if (!f) {
        puts("ERROR: Failed to open settings file:");
        printf("%s", fname.toStdString().c_str());
        return;
    }
    toml_table_t *table = toml_parse_file(f, error_buffer, sizeof(error_buffer));
    fclose(f);

    if (!table) {
        puts("ERROR: failed to parse settings.ini, continuing with defaults.");
        return;
    }

    for (int i=0; ; i++) {
        const char *key = toml_key_in(table, i);
        if (!key) {
            break;
        }
        std::string k(key);
        toml_datum_t string_in = toml_string_in(table, key);
        if (string_in.ok) {
            settings_[k] = node(read_string_setting(table, key));
            continue;
        }
        toml_datum_t int_in = toml_int_in(table, key);
        if (int_in.ok) {
            settings_[k] = node((int)int_in.u.i);
            continue;
        }
        toml_datum_t float_in = toml_double_in(table, key);
        if (float_in.ok) {
            settings_[k] = node((EmbReal)float_in.u.d);
            continue;
        }
        toml_datum_t bool_in = toml_bool_in(table, key);
        if (bool_in.ok) {
            settings_[k] = node(bool_in.u.b);
        }
    }
}

/**
 * @brief MainWindow::writeSettings
 *
 * This file needs to be read from the users home directory to ensure it is writable
 */
void
write_settings(void)
{
    debug_message("Writing Settings...");
    QString appDir = qApp->applicationDirPath();
#if defined(Q_OS_UNIX) || defined(Q_OS_MAC)
    String settingsPath = QDir::homePath().toStdString() + "/.embroidermodder2/settings.ini";
#else
    String settingsPath = appDir + "/settings.ini";
#endif
    std::ofstream settings_file;
    settings_file.open(settingsPath);

    settings_["window_position_x"] = node((int)_mainWin->pos().x());
    settings_["window_position_y"] = node((int)_mainWin->pos().y());
    settings_["window_size_x"] = node((int)_mainWin->size().width());
    settings_["window_size_y"] = node((int)_mainWin->size().height());

    settings_file << "[General]" << std::endl;
    //settings_file << "LayoutState=" << layoutState << std::endl;
    settings_file << "Language=" << settings.general_language << std::endl;
    settings_file << "IconTheme=" << settings.general_icon_theme << std::endl;
    settings_file << "IconSize=" << settings.general_icon_size << std::endl;
    settings_file << "MdiBGUseLogo=" << settings.general_mdi_bg_use_logo << std::endl;
    settings_file << "MdiBGUseTexture=" << settings.general_mdi_bg_use_texture << std::endl;
    settings_file << "MdiBGUseColor=" << settings.general_mdi_bg_use_color << std::endl;
    settings_file << "MdiBGLogo=" << get_str(settings_, "general_mdi_bg_logo") << std::endl;
    settings_file << "MdiBGTexture=" << settings.general_mdi_bg_texture << std::endl;
    settings_file << "MdiBGColor=" << settings.general_mdi_bg_color << std::endl;
    settings_file << "TipOfTheDay=" << settings.general_tip_of_the_day << std::endl;
    settings_file << "CurrentTip=" << settings.general_current_tip + 1 << std::endl;
    settings_file << "SystemHelpBrowser=" << settings.general_system_help_browser << std::endl;
    settings_file << std::endl;

    settings_file << "[Display]" << std::endl;
    settings_file << "UseOpenGL=" << settings.display_use_opengl << std::endl;
    settings_file << "RenderHintAntiAlias=" << settings.display_renderhint_aa << std::endl;
    settings_file << "RenderHintTextAntiAlias=" << settings.display_renderhint_text_aa << std::endl;
    settings_file << "RenderHintSmoothPixmap=" << settings.display_renderhint_smooth_pix << std::endl;
    settings_file << "RenderHintHighQualityAntiAlias=" << settings.display_renderhint_high_aa << std::endl;
    settings_file << "RenderHintNonCosmetic=" << settings.display_renderhint_noncosmetic << std::endl;
    settings_file << "ShowScrollBars=" << settings.display_show_scrollbars << std::endl;
    settings_file << "ScrollBarWidgetNum=" << settings.display_scrollbar_widget_num << std::endl;
    settings_file << "CrossHairColor=" << settings.display_crosshair_color << std::endl;
    settings_file << "BackgroundColor=" << settings.display_bg_color << std::endl;
    settings_file << "SelectBoxLeftColor=" << settings.display_selectbox_left_color << std::endl;
    settings_file << "SelectBoxLeftFill=" << settings.display_selectbox_left_fill << std::endl;
    settings_file << "SelectBoxRightColor=" << settings.display_selectbox_right_color << std::endl;
    settings_file << "SelectBoxRightFill=" << settings.display_selectbox_right_fill << std::endl;
    settings_file << "SelectBoxAlpha=" << settings.display_selectbox_alpha << std::endl;
    settings_file << "ZoomScaleIn=" << settings.display_zoomscale_in << std::endl;
    settings_file << "ZoomScaleOut=" << settings.display_zoomscale_out << std::endl;
    settings_file << "CrossHairPercent=" << settings.display_crosshair_percent << std::endl;
    settings_file << "Units=" << settings_["display_units"].s << std::endl;
    settings_file << std::endl;

    settings_file << "[Prompt]" << std::endl;
    settings_file << "TextColor=" << settings.prompt_text_color << std::endl;
    settings_file << "BackgroundColor=" << (unsigned int)settings.prompt_bg_color << std::endl;
    settings_file << "FontFamily=" << settings.prompt_font_family << std::endl;
    settings_file << "FontStyle=" << settings.prompt_font_style << std::endl;
    settings_file << "FontSize=" << settings.prompt_font_size << std::endl;
    settings_file << "SaveHistory=" << settings.prompt_save_history << std::endl;
    settings_file << "SaveHistoryAsHtml=" << settings.prompt_save_history_as_html << std::endl;
    settings_file << "SaveHistoryFilename=" << settings.prompt_save_history_filename  << std::endl;
    settings_file << std::endl;

    settings_file << "[OpenSave]" << std::endl;
    settings_file << "CustomFilter=" << settings.opensave_custom_filter.toStdString() << std::endl;
    settings_file << "OpenFormat=" << settings.opensave_open_format << std::endl;
    settings_file << "OpenThumbnail=" << settings.opensave_open_thumbnail << std::endl;
    settings_file << "SaveFormat=" << settings.opensave_save_format << std::endl;
    settings_file << "SaveThumbnail=" << settings.opensave_save_thumbnail << std::endl;
    settings_file << "RecentMax=" << settings.opensave_recent_max_files << std::endl;
    // settings_file << "RecentFiles=" << settings.opensave_recent_list_of_files << std::endl;
    settings_file << "RecentDirectory=" << settings.opensave_recent_directory << std::endl;
    settings_file << "TrimDstNumJumps=" << settings.opensave_trim_dst_num_jumps << std::endl;
    settings_file << std::endl;

    settings_file << "[Printing]" << std::endl;
    settings_file << "DefaultDevice=" << settings.printing_default_device << std::endl;
    settings_file << "UseLastDevice=" << settings.printing_use_last_device << std::endl;
    settings_file << "DisableBG=" << settings.printing_disable_bg << std::endl;
    settings_file << std::endl;

    settings_file << "[Grid]" << std::endl;
    settings_file << "ShowOnLoad=" << settings.grid_show_on_load << std::endl;
    settings_file << "ShowOrigin=" << settings.grid_show_origin << std::endl;
    settings_file << "ColorMatchCrossHair=" << settings.grid_color_match_crosshair << std::endl;
    settings_file << "Color=" << settings.grid_color << std::endl;
    settings_file << "LoadFromFile=" << settings.grid_load_from_file << std::endl;
    settings_file << "Type=" << settings_["grid_type"].s << std::endl;;
    settings_file << "CenterOnOrigin=" << settings.grid_center_on_origin << std::endl;
    settings_file << "CenterX=" << settings.grid_center.x << std::endl;
    settings_file << "CenterY=" << settings.grid_center.y << std::endl;
    settings_file << "SizeX=" << settings.grid_size.x << std::endl;
    settings_file << "SizeY=" << settings.grid_size.y << std::endl;
    settings_file << "SpacingX=" << settings.grid_spacing.x << std::endl;
    settings_file << "SpacingY=" << settings.grid_spacing.y << std::endl;
    settings_file << "SizeRadius=" << settings.grid_size_radius << std::endl;
    settings_file << "SpacingRadius=" << settings.grid_spacing_radius << std::endl;
    settings_file << "SpacingAngle=" << settings.grid_spacing_angle << std::endl;
    settings_file << std::endl;

    settings_file << "[Ruler]" << std::endl;
    settings_file << "ShowOnLoad=" << settings.ruler_show_on_load << std::endl;
    settings_file << "Metric=" << settings.ruler_metric << std::endl;
    settings_file << "Color=" << (unsigned int)settings.ruler_color << std::endl;
    settings_file << "PixelSize=" << settings.ruler_pixel_size << std::endl;
    settings_file << std::endl;

    settings_file << "[QuickSnap]" << std::endl;
    settings_file << "Enabled=" << settings.qsnap_enabled << std::endl;
    settings_file << "LocatorColor=" << settings.qsnap_locator_color << std::endl;
    settings_file << "LocatorSize=" << settings.qsnap_locator_size << std::endl;
    settings_file << "ApertureSize=" << settings.qsnap_aperture_size << std::endl;
    settings_file << std::endl;

    settings_file << "[LineWeight]" << std::endl;
    settings_file << "ShowLineWeight=" << settings.lwt_show_lwt << std::endl;
    settings_file << "RealRender=" << settings.lwt_real_render << std::endl;
    settings_file << "DefaultLineWeight=" << settings.lwt_default_lwt << std::endl;
    settings_file << std::endl;

    settings_file << "[Selection]" << std::endl;
    settings_file << "PickFirst=" << settings.selection_mode_pickfirst << std::endl;
    settings_file << "PickAdd=" << settings.selection_mode_pickadd << std::endl;
    settings_file << "PickDrag=" << settings.selection_mode_pickdrag << std::endl;
    settings_file << "CoolGripColor=" << settings.selection_coolgrip_color << std::endl;
    settings_file << "HotGripColor=" << settings.selection_hotgrip_color << std::endl;
    settings_file << "GripSize=" << settings.selection_grip_size << std::endl;
    settings_file << "PickBoxSize=" << settings.selection_pickbox_size << std::endl;
    settings_file << std::endl;

    settings_file << "[Text]" << std::endl;
    settings_file << "Font=" << settings.text_font << std::endl;
    settings_file << "Size=" << settings.text_size << std::endl;
    settings_file << "Angle=" << settings.text_angle << std::endl;
    settings_file << "StyleBold=" << settings.text_style_bold << std::endl;
    settings_file << "StyleItalic=" << settings.text_style_italic << std::endl;
    settings_file << "StyleUnderline=" << settings.text_style_underline << std::endl;
    settings_file << "StyleStrikeOut=" << settings.text_style_strikeout << std::endl;
    settings_file << "StyleOverline=" << settings.text_style_overline << std::endl;
    settings_file << std::endl;
    
    settings_file.close();

#if defined(Q_OS_UNIX) || defined(Q_OS_MAC)
    settingsPath = QDir::homePath().toStdString() + "/.embroidermodder2/settings.toml";
#else
    settingsPath = qApp->applicationDirPath() + "/settings.toml";
#endif
    std::ofstream file;
    file.open(settingsPath);

    for (auto iter = settings_.begin(); iter != settings_.end(); iter++) {
        file << iter->first << "=";
        switch (iter->second.type) {
        case INT_TYPE:
            file << iter->second.i;
            break;
        case REAL_TYPE:
            file << iter->second.r;
            break;
        case STRING_TYPE:
            file << "\"" << iter->second.s << "\"";
            break;
        /*
        case STRING_LIST_TYPE: {
            file << "[" << std::endl;
            for (int i=0; i<(int)iter->second.sl.size(); i++) {
                file << "    \"" << iter->second.sl[i] << "\"";
                if (i < (int)iter->second.sl.size() - 1) {
                    file << ",";
                }
            }
            file << "]";
            break;
        }
        */
        default:
            break;
        }
        file << std::endl;
    }
    file.close();
}

/**
 * .
 */
QCheckBox *
Settings_Dialog::make_checkbox(QGroupBox *gb, const char *label, const char *icon, bool *ptr)
{
    QCheckBox *checkBox = new QCheckBox(tr(label), gb);
    checkBox->setChecked(*ptr);
    checkBox->setIcon(_mainWin->create_icon(icon));
    connect(checkBox, &QCheckBox::stateChanged, this, [=](int x) { *ptr = (x != 0); });
    return checkBox;
}

/**
 *
 */
Settings_Dialog::Settings_Dialog(QString  showTab, QWidget* parent) : QDialog(parent)
{
    setMinimumSize(750,550);

    tabWidget = new QTabWidget(this);

    //TODO: Add icons to tabs
    tabWidget->addTab(createTabGeneral(), tr("General"));
    tabWidget->addTab(createTabFilesPaths(), tr("Files/Paths"));
    tabWidget->addTab(createTabDisplay(), tr("Display"));
    tabWidget->addTab(createTabPrompt(), tr("Prompt"));
    tabWidget->addTab(createTabOpenSave(), tr("Open/Save"));
    tabWidget->addTab(createTabPrinting(), tr("Printing"));
    tabWidget->addTab(createTabSnap(), tr("Snap"));
    tabWidget->addTab(createTabGridRuler(), tr("Grid/Ruler"));
    tabWidget->addTab(createTabOrthoPolar(), tr("Ortho/Polar"));
    tabWidget->addTab(createTabQuickSnap(), tr("QuickSnap"));
    tabWidget->addTab(createTabQuickTrack(), tr("QuickTrack"));
    tabWidget->addTab(createTabLineWeight(), tr("LineWeight"));
    tabWidget->addTab(createTabSelection(), tr("Selection"));

    if     (showTab == "General")     tabWidget->setCurrentIndex( 0);
    else if (showTab == "Files/Path")  tabWidget->setCurrentIndex( 1);
    else if (showTab == "Display")     tabWidget->setCurrentIndex( 2);
    else if (showTab == "Prompt")      tabWidget->setCurrentIndex( 3);
    else if (showTab == "Open/Save")   tabWidget->setCurrentIndex( 4);
    else if (showTab == "Printing")    tabWidget->setCurrentIndex( 5);
    else if (showTab == "Snap")        tabWidget->setCurrentIndex( 6);
    else if (showTab == "Grid/Ruler")  tabWidget->setCurrentIndex( 7);
    else if (showTab == "Ortho/Polar") tabWidget->setCurrentIndex( 8);
    else if (showTab == "QuickSnap")   tabWidget->setCurrentIndex( 9);
    else if (showTab == "QuickTrack")  tabWidget->setCurrentIndex(10);
    else if (showTab == "LineWeight")  tabWidget->setCurrentIndex(11);
    else if (showTab == "Selection")   tabWidget->setCurrentIndex(12);

    buttonBox = new QDialogButtonBox(QDialogButtonBox::Ok | QDialogButtonBox::Cancel);

    connect(buttonBox, SIGNAL(accepted()), this, SLOT(acceptChanges()));
    connect(buttonBox, SIGNAL(rejected()), this, SLOT(rejectChanges()));

    QVBoxLayout* vboxLayoutMain = new QVBoxLayout(this);
    vboxLayoutMain->addWidget(tabWidget);
    vboxLayoutMain->addWidget(buttonBox);
    setLayout(vboxLayoutMain);

    setWindowTitle(tr("Settings"));

    QApplication::setOverrideCursor(Qt::ArrowCursor);
}

Settings_Dialog::~Settings_Dialog()
{
    QApplication::restoreOverrideCursor();
}

QWidget* Settings_Dialog::createTabGeneral()
{
    QWidget* widget = new QWidget(this);

    //Language
    QGroupBox* groupBoxLanguage = new QGroupBox(tr("Language"), widget);

    QLabel* labelLanguage = new QLabel(tr("Language (Requires Restart)"), groupBoxLanguage);
    QComboBox* comboBoxLanguage = new QComboBox(groupBoxLanguage);
    dialog.general_language = QString::fromStdString(settings.general_language).toLower().toStdString();
    comboBoxLanguage->addItem("Default");
    comboBoxLanguage->addItem("System");
    comboBoxLanguage->insertSeparator(2);
    QDir trDir(qApp->applicationDirPath());
    trDir.cd("translations");
    foreach(QString dirName, trDir.entryList(QDir::Dirs | QDir::NoDotAndDotDot)) {
        dirName[0] = dirName[0].toUpper();
        comboBoxLanguage->addItem(dirName);
    }
    QString current = QString::fromStdString(dialog.general_language);
    current[0] = current[0].toUpper();
    comboBoxLanguage->setCurrentIndex(comboBoxLanguage->findText(current));
    /*
    TODO: FIX
    connect(
        comboBoxLanguage,
        SIGNAL(currentIndexChanged(QString)),
        this,
        [=](const char *lang) { dialog.general_language = QString::fromStdString(lang).toLower().toStdString(); }
    );
    */

    QVBoxLayout* vboxLayoutLanguage = new QVBoxLayout(groupBoxLanguage);
    vboxLayoutLanguage->addWidget(labelLanguage);
    vboxLayoutLanguage->addWidget(comboBoxLanguage);
    groupBoxLanguage->setLayout(vboxLayoutLanguage);

    //Icons
    QGroupBox* groupBoxIcon = new QGroupBox(tr("Icons"), widget);

    QLabel* labelIconTheme = new QLabel(tr("Icon Theme"), groupBoxIcon);
    QComboBox* comboBoxIconTheme = new QComboBox(groupBoxIcon);
    QDir dir(qApp->applicationDirPath());
    dir.cd("icons");
    dialog.general_icon_theme = settings.general_icon_theme;
    foreach(QString dirName, dir.entryList(QDir::Dirs | QDir::NoDotAndDotDot)) {
        comboBoxIconTheme->addItem(QIcon("icons/" + dirName + "/" + "theme" + ".png"), dirName);
    }
    comboBoxIconTheme->setCurrentIndex(comboBoxIconTheme->findText(QString::fromStdString(dialog.general_icon_theme)));
    /*
    TODO: FIX
    connect(
        comboBoxIconTheme,
        &QComboBox::currentIndexChanged,
        this,
        [=](int i) { dialog.general_icon_theme = theme.toStdString(); }
    );
    */

    QLabel* labelIconSize = new QLabel(tr("Icon Size"), groupBoxIcon);
    QComboBox* comboBoxIconSize = new QComboBox(groupBoxIcon);
    comboBoxIconSize->addItem(QIcon("icons/" + QString::fromStdString(dialog.general_icon_theme) + "/" + "icon16"  + ".png"), "Very Small", 16);
    comboBoxIconSize->addItem(QIcon("icons/" + QString::fromStdString(dialog.general_icon_theme) + "/" + "icon24"  + ".png"), "Small", 24);
    comboBoxIconSize->addItem(QIcon("icons/" + QString::fromStdString(dialog.general_icon_theme) + "/" + "icon32"  + ".png"), "Medium", 32);
    comboBoxIconSize->addItem(QIcon("icons/" + QString::fromStdString(dialog.general_icon_theme) + "/" + "icon48"  + ".png"), "Large", 48);
    comboBoxIconSize->addItem(QIcon("icons/" + QString::fromStdString(dialog.general_icon_theme) + "/" + "icon64"  + ".png"), "Very Large", 64);
    comboBoxIconSize->addItem(QIcon("icons/" + QString::fromStdString(dialog.general_icon_theme) + "/" + "icon128" + ".png"), "I'm Blind", 128);
    dialog.general_icon_size = settings.general_icon_size;
    comboBoxIconSize->setCurrentIndex(comboBoxIconSize->findData(dialog.general_icon_size));
    connect(comboBoxIconSize, SIGNAL(currentIndexChanged(int)), this, SLOT(comboBoxIconSizeCurrentIndexChanged(int)));

    QVBoxLayout* vboxLayoutIcon = new QVBoxLayout(groupBoxIcon);
    vboxLayoutIcon->addWidget(labelIconTheme);
    vboxLayoutIcon->addWidget(comboBoxIconTheme);
    vboxLayoutIcon->addWidget(labelIconSize);
    vboxLayoutIcon->addWidget(comboBoxIconSize);
    groupBoxIcon->setLayout(vboxLayoutIcon);

    //Mdi Background
    QGroupBox* groupBoxMdiBG = new QGroupBox(tr("Background"), widget);


    QCheckBox* checkBoxMdiBGUseLogo = new QCheckBox(tr("Use Logo"), groupBoxMdiBG);
    dialog.general_mdi_bg_use_logo = settings.general_mdi_bg_use_logo;
    preview.general_mdi_bg_use_logo = dialog.general_mdi_bg_use_logo;
    checkBoxMdiBGUseLogo->setChecked(preview.general_mdi_bg_use_logo);
    connect(checkBoxMdiBGUseLogo, SIGNAL(stateChanged(int)), this, SLOT(checkBoxGeneralMdiBGUseLogoStateChanged(int)));

    QPushButton* buttonMdiBGLogo = new QPushButton(tr("Choose"), groupBoxMdiBG);
    buttonMdiBGLogo->setEnabled(dialog.general_mdi_bg_use_logo);
    dialog_["general_mdi_bg_logo"] = node(get_str(settings_, "general_mdi_bg_logo"));
    accept__["general_mdi_bg_logo"] = dialog_["general_mdi_bg_logo"];
    connect(buttonMdiBGLogo, SIGNAL(clicked()), this, SLOT(chooseGeneralMdiBackgroundLogo()));
    connect(checkBoxMdiBGUseLogo, SIGNAL(toggled(bool)), buttonMdiBGLogo, SLOT(setEnabled(bool)));

    QCheckBox* checkBoxMdiBGUseTexture = new QCheckBox(tr("Use Texture"), groupBoxMdiBG);
    dialog.general_mdi_bg_use_texture = settings.general_mdi_bg_use_texture;
    preview.general_mdi_bg_use_texture = dialog.general_mdi_bg_use_texture;
    checkBoxMdiBGUseTexture->setChecked(preview.general_mdi_bg_use_texture);
    connect(checkBoxMdiBGUseTexture, SIGNAL(stateChanged(int)), this, SLOT(checkBoxGeneralMdiBGUseTextureStateChanged(int)));

    QPushButton* buttonMdiBGTexture = new QPushButton(tr("Choose"), groupBoxMdiBG);
    buttonMdiBGTexture->setEnabled(dialog.general_mdi_bg_use_texture);
    dialog.general_mdi_bg_texture  = settings.general_mdi_bg_texture;
    accept_.general_mdi_bg_texture  = dialog.general_mdi_bg_texture;
    connect(buttonMdiBGTexture, SIGNAL(clicked()), this, SLOT(chooseGeneralMdiBackgroundTexture()));
    connect(checkBoxMdiBGUseTexture, SIGNAL(toggled(bool)), buttonMdiBGTexture, SLOT(setEnabled(bool)));

    QCheckBox* checkBoxMdiBGUseColor = new QCheckBox(tr("Use Color"), groupBoxMdiBG);
    dialog.general_mdi_bg_use_color = settings.general_mdi_bg_use_color;
    preview.general_mdi_bg_use_color = dialog.general_mdi_bg_use_color;
    checkBoxMdiBGUseColor->setChecked(preview.general_mdi_bg_use_color);
    connect(checkBoxMdiBGUseColor, SIGNAL(stateChanged(int)), this, SLOT(checkBoxGeneralMdiBGUseColorStateChanged(int)));

    QPushButton* buttonMdiBGColor = new QPushButton(tr("Choose"), groupBoxMdiBG);
    buttonMdiBGColor->setEnabled(dialog.general_mdi_bg_use_color);
    dialog.general_mdi_bg_color = settings.general_mdi_bg_color;
    preview.general_mdi_bg_color = dialog.general_mdi_bg_color;
    accept_.general_mdi_bg_color = dialog.general_mdi_bg_color;
    QPixmap mdiBGPix(16,16);
    mdiBGPix.fill(QColor(preview.general_mdi_bg_color));
    buttonMdiBGColor->setIcon(QIcon(mdiBGPix));
    connect(buttonMdiBGColor, SIGNAL(clicked()), this, SLOT(chooseGeneralMdiBackgroundColor()));
    connect(checkBoxMdiBGUseColor, SIGNAL(toggled(bool)), buttonMdiBGColor, SLOT(setEnabled(bool)));

    QGridLayout* gridLayoutMdiBG = new QGridLayout(widget);
    gridLayoutMdiBG->addWidget(checkBoxMdiBGUseLogo, 0, 0, Qt::AlignLeft);
    gridLayoutMdiBG->addWidget(buttonMdiBGLogo, 0, 1, Qt::AlignRight);
    gridLayoutMdiBG->addWidget(checkBoxMdiBGUseTexture, 1, 0, Qt::AlignLeft);
    gridLayoutMdiBG->addWidget(buttonMdiBGTexture, 1, 1, Qt::AlignRight);
    gridLayoutMdiBG->addWidget(checkBoxMdiBGUseColor, 2, 0, Qt::AlignLeft);
    gridLayoutMdiBG->addWidget(buttonMdiBGColor, 2, 1, Qt::AlignRight);
    groupBoxMdiBG->setLayout(gridLayoutMdiBG);

    //Tips
    QGroupBox* groupBoxTips = new QGroupBox(tr("Tips"), widget);

    dialog.general_tip_of_the_day = settings.general_tip_of_the_day;

    QCheckBox* checkBoxTipOfTheDay = make_checkbox(groupBoxTips,
        "Show Tip of the Day on startup", "blank", &(dialog.general_tip_of_the_day));

    QVBoxLayout* vboxLayoutTips = new QVBoxLayout(groupBoxTips);
    vboxLayoutTips->addWidget(checkBoxTipOfTheDay);
    groupBoxTips->setLayout(vboxLayoutTips);

    //Help Browser
    QGroupBox* groupBoxHelpBrowser = new QGroupBox(tr("Help Browser"), widget);

    QRadioButton* radioButtonSystemHelpBrowser = new QRadioButton(tr("System"), groupBoxHelpBrowser);
    radioButtonSystemHelpBrowser->setChecked(settings.general_system_help_browser);
    QRadioButton* radioButtonCustomHelpBrowser = new QRadioButton(tr("Custom"), groupBoxHelpBrowser);
    radioButtonCustomHelpBrowser->setChecked(!settings.general_system_help_browser);
    radioButtonCustomHelpBrowser->setEnabled(false); //TODO: finish this

    QVBoxLayout* vboxLayoutHelpBrowser = new QVBoxLayout(groupBoxHelpBrowser);
    vboxLayoutHelpBrowser->addWidget(radioButtonSystemHelpBrowser);
    vboxLayoutHelpBrowser->addWidget(radioButtonCustomHelpBrowser);
    groupBoxHelpBrowser->setLayout(vboxLayoutHelpBrowser);

    //Widget Layout
    QVBoxLayout* vboxLayoutMain = new QVBoxLayout(widget);
    vboxLayoutMain->addWidget(groupBoxLanguage);
    vboxLayoutMain->addWidget(groupBoxIcon);
    vboxLayoutMain->addWidget(groupBoxMdiBG);
    vboxLayoutMain->addWidget(groupBoxTips);
    vboxLayoutMain->addWidget(groupBoxHelpBrowser);
    vboxLayoutMain->addStretch(1);
    widget->setLayout(vboxLayoutMain);

    QScrollArea* scrollArea = new QScrollArea(this);
    scrollArea->setWidgetResizable(true);
    scrollArea->setWidget(widget);
    return scrollArea;
}

QWidget* Settings_Dialog::createTabFilesPaths()
{
    QWidget* widget = new QWidget(this);

    QScrollArea* scrollArea = new QScrollArea(this);
    scrollArea->setWidgetResizable(true);
    scrollArea->setWidget(widget);
    return scrollArea;
}

QWidget* Settings_Dialog::createTabDisplay()
{
    QWidget* widget = new QWidget(this);

    //Rendering
    //TODO: Review OpenGL and Rendering settings for future inclusion
    QGroupBox* groupBoxRender = new QGroupBox(tr("Rendering"), widget);

    dialog.display_use_opengl = settings.display_use_opengl;
    dialog.display_renderhint_aa = settings.display_renderhint_aa;
    dialog.display_renderhint_text_aa = settings.display_renderhint_text_aa;
    dialog.display_renderhint_smooth_pix = settings.display_renderhint_smooth_pix;
    dialog.display_renderhint_high_aa = settings.display_renderhint_high_aa;
    dialog.display_renderhint_noncosmetic = settings.display_renderhint_noncosmetic;

    QCheckBox* checkBoxUseOpenGL = make_checkbox(groupBoxRender, "Use OpenGL", "blank", &(dialog.display_use_opengl));
    QCheckBox* checkBoxRenderHintAA = make_checkbox(groupBoxRender, "Antialias", "blank", &(dialog.display_renderhint_aa));
    QCheckBox* checkBoxRenderHintTextAA = make_checkbox(groupBoxRender, "Antialias Text", "blank", &(dialog.display_renderhint_text_aa));
    QCheckBox* checkBoxRenderHintSmoothPix = make_checkbox(groupBoxRender, "Smooth Pixmap", "blank", &(dialog.display_renderhint_smooth_pix));
    QCheckBox* checkBoxRenderHintHighAA = make_checkbox(groupBoxRender, "High Quality Antialiasing (OpenGL)", "blank", &(dialog.display_renderhint_high_aa));
    QCheckBox* checkBoxRenderHintNonCosmetic = make_checkbox(groupBoxRender, "Non Cosmetic", "blank", &(dialog.display_renderhint_noncosmetic));

    QVBoxLayout* vboxLayoutRender = new QVBoxLayout(groupBoxRender);
    vboxLayoutRender->addWidget(checkBoxUseOpenGL);
    vboxLayoutRender->addWidget(checkBoxRenderHintAA);
    vboxLayoutRender->addWidget(checkBoxRenderHintTextAA);
    vboxLayoutRender->addWidget(checkBoxRenderHintSmoothPix);
    vboxLayoutRender->addWidget(checkBoxRenderHintHighAA);
    vboxLayoutRender->addWidget(checkBoxRenderHintNonCosmetic);
    groupBoxRender->setLayout(vboxLayoutRender);

    //ScrollBars
    QGroupBox* groupBoxScrollBars = new QGroupBox(tr("ScrollBars"), widget);

    QCheckBox* checkBoxShowScrollBars = new QCheckBox(tr("Show ScrollBars"), groupBoxScrollBars);
    dialog.display_show_scrollbars = settings.display_show_scrollbars;
    preview.display_show_scrollbars = dialog.display_show_scrollbars;
    checkBoxShowScrollBars->setChecked(preview.display_show_scrollbars);
    connect(checkBoxShowScrollBars, SIGNAL(stateChanged(int)), this, SLOT(checkBoxShowScrollBarsStateChanged(int)));

    QLabel* labelScrollBarWidget = new QLabel(tr("Perform action when clicking corner widget"), groupBoxScrollBars);
    QComboBox* comboBoxScrollBarWidget = new QComboBox(groupBoxScrollBars);
    dialog.display_scrollbar_widget_num = settings.display_scrollbar_widget_num;
    for (auto iter = actionHash.begin(); iter!=actionHash.end(); iter++) {
        QAction* action = iter->second;
        if (action) {
            comboBoxScrollBarWidget->addItem(action->icon(), action->text().replace("&", ""));
        }
    }
    comboBoxScrollBarWidget->setCurrentIndex(dialog.display_scrollbar_widget_num);
    connect(comboBoxScrollBarWidget, SIGNAL(currentIndexChanged(int)), this, SLOT(comboBoxScrollBarWidgetCurrentIndexChanged(int)));

    QVBoxLayout* vboxLayoutScrollBars = new QVBoxLayout(groupBoxScrollBars);
    vboxLayoutScrollBars->addWidget(checkBoxShowScrollBars);
    vboxLayoutScrollBars->addWidget(labelScrollBarWidget);
    vboxLayoutScrollBars->addWidget(comboBoxScrollBarWidget);
    groupBoxScrollBars->setLayout(vboxLayoutScrollBars);

    //Colors
    QGroupBox* groupBoxColor = new QGroupBox(tr("Colors"), widget);

    QLabel* labelCrossHairColor = new QLabel(tr("Crosshair Color"), groupBoxColor);
    QPushButton* buttonCrossHairColor = new QPushButton(tr("Choose"), groupBoxColor);
    dialog.display_crosshair_color  = settings.display_crosshair_color;
    preview.display_crosshair_color = dialog.display_crosshair_color;
    accept_.display_crosshair_color  = dialog.display_crosshair_color;
    QPixmap crosshairPix(16,16);
    crosshairPix.fill(QColor(preview.display_crosshair_color));
    buttonCrossHairColor->setIcon(QIcon(crosshairPix));
    connect(buttonCrossHairColor, SIGNAL(clicked()), this, SLOT(chooseDisplayCrossHairColor()));

    QLabel* labelBGColor = new QLabel(tr("Background Color"), groupBoxColor);
    QPushButton* buttonBGColor = new QPushButton(tr("Choose"), groupBoxColor);
    dialog.display_bg_color  = settings.display_bg_color;
    preview.display_bg_color = dialog.display_bg_color;
    accept_.display_bg_color  = dialog.display_bg_color;
    QPixmap bgPix(16,16);
    bgPix.fill(QColor(preview.display_bg_color));
    buttonBGColor->setIcon(QIcon(bgPix));
    connect(buttonBGColor, SIGNAL(clicked()), this, SLOT(chooseDisplayBackgroundColor()));

    QLabel* labelSelectBoxLeftColor = new QLabel(tr("Selection Box Color (Crossing)"), groupBoxColor);
    QPushButton* buttonSelectBoxLeftColor = new QPushButton(tr("Choose"), groupBoxColor);
    dialog.display_selectbox_left_color  = settings.display_selectbox_left_color;
    preview.display_selectbox_left_color = dialog.display_selectbox_left_color;
    accept_.display_selectbox_left_color  = dialog.display_selectbox_left_color;
    QPixmap sBoxLCPix(16,16);
    sBoxLCPix.fill(QColor(preview.display_selectbox_left_color));
    buttonSelectBoxLeftColor->setIcon(QIcon(sBoxLCPix));
    connect(buttonSelectBoxLeftColor, SIGNAL(clicked()), this, SLOT(chooseDisplaySelectBoxLeftColor()));

    QLabel* labelSelectBoxLeftFill = new QLabel(tr("Selection Box Fill (Crossing)"), groupBoxColor);
    QPushButton* buttonSelectBoxLeftFill = new QPushButton(tr("Choose"), groupBoxColor);
    dialog.display_selectbox_left_fill  = settings.display_selectbox_left_fill;
    preview.display_selectbox_left_fill = dialog.display_selectbox_left_fill;
    accept_.display_selectbox_left_fill  = dialog.display_selectbox_left_fill;
    QPixmap sBoxLFPix(16,16);
    sBoxLFPix.fill(QColor(preview.display_selectbox_left_fill));
    buttonSelectBoxLeftFill->setIcon(QIcon(sBoxLFPix));
    connect(buttonSelectBoxLeftFill, SIGNAL(clicked()), this, SLOT(chooseDisplaySelectBoxLeftFill()));

    QLabel* labelSelectBoxRightColor = new QLabel(tr("Selection Box Color (Window)"), groupBoxColor);
    QPushButton* buttonSelectBoxRightColor = new QPushButton(tr("Choose"), groupBoxColor);
    dialog.display_selectbox_right_color  = settings.display_selectbox_right_color;
    preview.display_selectbox_right_color = dialog.display_selectbox_right_color;
    accept_.display_selectbox_right_color  = dialog.display_selectbox_right_color;
    QPixmap sBoxRCPix(16,16);
    sBoxRCPix.fill(QColor(preview.display_selectbox_right_color));
    buttonSelectBoxRightColor->setIcon(QIcon(sBoxRCPix));
    connect(buttonSelectBoxRightColor, SIGNAL(clicked()), this, SLOT(chooseDisplaySelectBoxRightColor()));

    QLabel* labelSelectBoxRightFill = new QLabel(tr("Selection Box Fill (Window)"), groupBoxColor);
    QPushButton* buttonSelectBoxRightFill = new QPushButton(tr("Choose"), groupBoxColor);
    dialog.display_selectbox_right_fill  = settings.display_selectbox_right_fill;
    preview.display_selectbox_right_fill = dialog.display_selectbox_right_fill;
    accept_.display_selectbox_right_fill  = dialog.display_selectbox_right_fill;
    QPixmap sBoxRFPix(16,16);
    sBoxRFPix.fill(QColor(preview.display_selectbox_right_fill));
    buttonSelectBoxRightFill->setIcon(QIcon(sBoxRFPix));
    connect(buttonSelectBoxRightFill, SIGNAL(clicked()), this, SLOT(chooseDisplaySelectBoxRightFill()));

    QLabel* labelSelectBoxAlpha = new QLabel(tr("Selection Box Fill Alpha"), groupBoxColor);
    QSpinBox* spinBoxSelectBoxAlpha = new QSpinBox(groupBoxColor);
    spinBoxSelectBoxAlpha->setRange(0, 255);
    dialog.display_selectbox_alpha = settings.display_selectbox_alpha;
    preview.display_selectbox_alpha = dialog.display_selectbox_alpha;
    spinBoxSelectBoxAlpha->setValue(preview.display_selectbox_alpha);
    connect(spinBoxSelectBoxAlpha, SIGNAL(valueChanged(int)), this, SLOT(spinBoxDisplaySelectBoxAlphaValueChanged(int)));

    QGridLayout* gridLayoutColor = new QGridLayout(widget);
    gridLayoutColor->addWidget(labelCrossHairColor, 0, 0, Qt::AlignLeft);
    gridLayoutColor->addWidget(buttonCrossHairColor, 0, 1, Qt::AlignRight);
    gridLayoutColor->addWidget(labelBGColor, 1, 0, Qt::AlignLeft);
    gridLayoutColor->addWidget(buttonBGColor, 1, 1, Qt::AlignRight);
    gridLayoutColor->addWidget(labelSelectBoxLeftColor, 2, 0, Qt::AlignLeft);
    gridLayoutColor->addWidget(buttonSelectBoxLeftColor, 2, 1, Qt::AlignRight);
    gridLayoutColor->addWidget(labelSelectBoxLeftFill, 3, 0, Qt::AlignLeft);
    gridLayoutColor->addWidget(buttonSelectBoxLeftFill, 3, 1, Qt::AlignRight);
    gridLayoutColor->addWidget(labelSelectBoxRightColor, 4, 0, Qt::AlignLeft);
    gridLayoutColor->addWidget(buttonSelectBoxRightColor, 4, 1, Qt::AlignRight);
    gridLayoutColor->addWidget(labelSelectBoxRightFill, 5, 0, Qt::AlignLeft);
    gridLayoutColor->addWidget(buttonSelectBoxRightFill, 5, 1, Qt::AlignRight);
    gridLayoutColor->addWidget(labelSelectBoxAlpha, 6, 0, Qt::AlignLeft);
    gridLayoutColor->addWidget(spinBoxSelectBoxAlpha, 6, 1, Qt::AlignRight);
    groupBoxColor->setLayout(gridLayoutColor);

    //Zoom
    QGroupBox* groupBoxZoom = new QGroupBox(tr("Zoom"), widget);

    QLabel* labelZoomScaleIn = new QLabel(tr("Zoom In Scale"), groupBoxZoom);
    QDoubleSpinBox* spinBoxZoomScaleIn = new QDoubleSpinBox(groupBoxZoom);
    dialog.display_zoomscale_in = settings.display_zoomscale_in;
    spinBoxZoomScaleIn->setValue(dialog.display_zoomscale_in);
    spinBoxZoomScaleIn->setSingleStep(0.01);
    spinBoxZoomScaleIn->setRange(1.01, 10.00);
    connect(
        spinBoxZoomScaleIn,
        &QDoubleSpinBox::valueChanged,
        this,
        [=](double value) { dialog.display_zoomscale_in = value; }
    );

    QLabel* labelZoomScaleOut = new QLabel(tr("Zoom Out Scale"), groupBoxZoom);
    QDoubleSpinBox* spinBoxZoomScaleOut = new QDoubleSpinBox(groupBoxZoom);
    dialog.display_zoomscale_out = settings.display_zoomscale_out;
    spinBoxZoomScaleOut->setValue(dialog.display_zoomscale_out);
    spinBoxZoomScaleOut->setSingleStep(0.01);
    spinBoxZoomScaleOut->setRange(0.01, 0.99);
    connect(
        spinBoxZoomScaleOut,
        &QDoubleSpinBox::valueChanged,
        this,
        [=](double value) { dialog.display_zoomscale_out = value; }
    );

    QGridLayout* gridLayoutZoom = new QGridLayout(groupBoxZoom);
    gridLayoutZoom->addWidget(labelZoomScaleIn, 0, 0, Qt::AlignLeft);
    gridLayoutZoom->addWidget(spinBoxZoomScaleIn, 0, 1, Qt::AlignRight);
    gridLayoutZoom->addWidget(labelZoomScaleOut, 1, 0, Qt::AlignLeft);
    gridLayoutZoom->addWidget(spinBoxZoomScaleOut, 1, 1, Qt::AlignRight);
    groupBoxZoom->setLayout(gridLayoutZoom);

    //Widget Layout
    QVBoxLayout *vboxLayoutMain = new QVBoxLayout(widget);
    //vboxLayoutMain->addWidget(groupBoxRender); //TODO: Review OpenGL and Rendering settings for future inclusion
    vboxLayoutMain->addWidget(groupBoxScrollBars);
    vboxLayoutMain->addWidget(groupBoxColor);
    vboxLayoutMain->addWidget(groupBoxZoom);
    vboxLayoutMain->addStretch(1);
    widget->setLayout(vboxLayoutMain);

    QScrollArea* scrollArea = new QScrollArea(this);
    scrollArea->setWidgetResizable(true);
    scrollArea->setWidget(widget);
    return scrollArea;
}

//TODO: finish prompt options
QWidget* Settings_Dialog::createTabPrompt()
{
    QWidget* widget = new QWidget(this);

    //Colors
    QGroupBox* groupBoxColor = new QGroupBox(tr("Colors"), widget);

    QLabel* labelTextColor = new QLabel(tr("Text Color"), groupBoxColor);
    QPushButton* buttonTextColor = new QPushButton(tr("Choose"), groupBoxColor);
    dialog.prompt_text_color  = settings.prompt_text_color;
    preview.prompt_text_color = dialog.prompt_text_color;
    accept_.prompt_text_color  = dialog.prompt_text_color;
    QPixmap pix(16,16);
    pix.fill(QColor(preview.prompt_text_color));
    buttonTextColor->setIcon(QIcon(pix));
    connect(buttonTextColor, SIGNAL(clicked()), this, SLOT(choosePromptTextColor()));

    QLabel* labelBGColor = new QLabel(tr("Background Color"), groupBoxColor);
    QPushButton* buttonBGColor = new QPushButton(tr("Choose"), groupBoxColor);
    dialog.prompt_bg_color  = settings.prompt_bg_color;
    preview.prompt_bg_color = dialog.prompt_bg_color;
    accept_.prompt_bg_color  = dialog.prompt_bg_color;
    QPixmap pixx(16,16);
    pixx.fill(QColor(preview.prompt_bg_color));
    buttonBGColor->setIcon(QIcon(pixx));
    connect(buttonBGColor, SIGNAL(clicked()), this, SLOT(choosePromptBackgroundColor()));

    QGridLayout* gridLayoutColor = new QGridLayout(widget);
    gridLayoutColor->addWidget(labelTextColor, 0, 0, Qt::AlignLeft);
    gridLayoutColor->addWidget(buttonTextColor, 0, 1, Qt::AlignRight);
    gridLayoutColor->addWidget(labelBGColor, 1, 0, Qt::AlignLeft);
    gridLayoutColor->addWidget(buttonBGColor, 1, 1, Qt::AlignRight);
    groupBoxColor->setLayout(gridLayoutColor);

    //TODO: Tweak the Prompt Font ComboBoxes so they work properly
    //Font
    QGroupBox* groupBoxFont = new QGroupBox(tr("Font"), widget);

    QLabel* labelFontFamily = new QLabel(tr("Font Family"), groupBoxFont);
    QFontComboBox* comboBoxFontFamily = new QFontComboBox(groupBoxFont);
    dialog.prompt_font_family = settings.prompt_font_family;
    preview.prompt_font_family = dialog.prompt_font_family;
    comboBoxFontFamily->setCurrentFont(QFont(QString::fromStdString(preview.prompt_font_family)));
    connect(comboBoxFontFamily, SIGNAL(currentIndexChanged(QString)), this, SLOT(comboBoxPromptFontFamilyCurrentIndexChanged(QString)));
    QLabel* labelFontStyle = new QLabel(tr("Font Style"), groupBoxFont);
    QComboBox* comboBoxFontStyle = new QComboBox(groupBoxFont);
    comboBoxFontStyle->addItem("Normal");
    comboBoxFontStyle->addItem("Italic");
    comboBoxFontStyle->setEditText(QString::fromStdString(preview.prompt_font_style));
    connect(comboBoxFontStyle, SIGNAL(currentIndexChanged(QString)), this, SLOT(comboBoxPromptFontStyleCurrentIndexChanged(QString)));
    QLabel* labelFontSize = new QLabel(tr("Font Size"), groupBoxFont);
    QSpinBox* spinBoxFontSize = new QSpinBox(groupBoxFont);
    spinBoxFontSize->setRange(4, 64);
    dialog.prompt_font_size = settings.prompt_font_size;
    preview.prompt_font_size = dialog.prompt_font_size;
    spinBoxFontSize->setValue(preview.prompt_font_size);
    connect(spinBoxFontSize, SIGNAL(valueChanged(int)), this, SLOT(spinBoxPromptFontSizeValueChanged(int)));

    QGridLayout* gridLayoutFont = new QGridLayout(groupBoxFont);
    gridLayoutFont->addWidget(labelFontFamily, 0, 0, Qt::AlignLeft);
    gridLayoutFont->addWidget(comboBoxFontFamily, 0, 1, Qt::AlignRight);
    gridLayoutFont->addWidget(labelFontStyle, 1, 0, Qt::AlignLeft);
    gridLayoutFont->addWidget(comboBoxFontStyle, 1, 1, Qt::AlignRight);
    gridLayoutFont->addWidget(labelFontSize, 2, 0, Qt::AlignLeft);
    gridLayoutFont->addWidget(spinBoxFontSize, 2, 1, Qt::AlignRight);
    groupBoxFont->setLayout(gridLayoutFont);

    //History
    QGroupBox* groupBoxHistory = new QGroupBox(tr("History"), widget);

    QCheckBox* checkBoxPromptSaveHistory = new QCheckBox(tr("Save History"), groupBoxHistory);
    dialog.prompt_save_history = settings.prompt_save_history;
    checkBoxPromptSaveHistory->setChecked(dialog.prompt_save_history);
    connect(
        checkBoxPromptSaveHistory,
        &QCheckBox::stateChanged,
        this,
        [=](int checked) { dialog.prompt_save_history = checked; }
    );

    QCheckBox* checkBoxPromptSaveHistoryAsHtml = new QCheckBox(tr("Save As HTML"), groupBoxHistory);
    dialog.prompt_save_history_as_html = settings.prompt_save_history_as_html;
    checkBoxPromptSaveHistoryAsHtml->setChecked(dialog.prompt_save_history_as_html);
    connect(checkBoxPromptSaveHistoryAsHtml, SIGNAL(stateChanged(int)), this, SLOT(checkBoxPromptSaveHistoryAsHtmlStateChanged(int)));

    QVBoxLayout* vboxLayoutHistory = new QVBoxLayout(groupBoxHistory);
    vboxLayoutHistory->addWidget(checkBoxPromptSaveHistory);
    vboxLayoutHistory->addWidget(checkBoxPromptSaveHistoryAsHtml);
    groupBoxHistory->setLayout(vboxLayoutHistory);

    //Widget Layout
    QVBoxLayout *vboxLayoutMain = new QVBoxLayout(widget);
    vboxLayoutMain->addWidget(groupBoxColor);
    vboxLayoutMain->addWidget(groupBoxFont);
    vboxLayoutMain->addWidget(groupBoxHistory);
    vboxLayoutMain->addStretch(1);
    widget->setLayout(vboxLayoutMain);

    QScrollArea* scrollArea = new QScrollArea(this);
    scrollArea->setWidgetResizable(true);
    scrollArea->setWidget(widget);
    return scrollArea;
}

/**
 * .
 */
QCheckBox*
Settings_Dialog::create_checkbox(QGroupBox *groupbox, std::string label)
{
    QCheckBox* checkbox = new QCheckBox(label.c_str(), groupbox);
    std::string filter = "*." + label;
    checkbox->setChecked(dialog.opensave_custom_filter.contains(filter.c_str(), Qt::CaseInsensitive));
    connect(checkbox, SIGNAL(stateChanged(int)), this, SLOT(checkBoxCustomFilterStateChanged(int)));
    return checkbox;
}

//TODO: finish open/save options
QWidget* Settings_Dialog::createTabOpenSave()
{
    QWidget* widget = new QWidget(this);

    //Custom Filter
    QGroupBox* groupBoxCustomFilter = new QGroupBox(tr("Custom Filter"), widget);
    groupBoxCustomFilter->setEnabled(false); //TODO: Fixup custom filter

    dialog.opensave_custom_filter = settings.opensave_custom_filter;

    std::unordered_map<std::string, QCheckBox*> checkBoxCustomFilter;
    int n_extensions = (int)extensions.size();

    for (int i=0; i<n_extensions; i++) {
        checkBoxCustomFilter[extensions[i]] = create_checkbox(groupBoxCustomFilter, extensions[i]);
    }

    QPushButton* buttonCustomFilterSelectAll = new QPushButton(tr("Select All"), groupBoxCustomFilter);
    connect(buttonCustomFilterSelectAll, SIGNAL(clicked()), this, SLOT(buttonCustomFilterSelectAllClicked()));
    for (int i=0; i<n_extensions; i++) {
        connect(
            this,
            SIGNAL(buttonCustomFilterSelectAll(bool)),
            checkBoxCustomFilter[extensions[i]],
            SLOT(setChecked(bool))
        );
    }

    QPushButton* buttonCustomFilterClearAll = new QPushButton("Clear All", groupBoxCustomFilter);
    connect(buttonCustomFilterClearAll, SIGNAL(clicked()), this, SLOT(buttonCustomFilterClearAllClicked()));
    for (int i=0; i<n_extensions; i++) {
        connect(
            this,
            SIGNAL(buttonCustomFilterClearAll(bool)),
            checkBoxCustomFilter[extensions[i]],
            SLOT(setChecked(bool))
        );
    }

    QGridLayout* gridLayoutCustomFilter = new QGridLayout(groupBoxCustomFilter);
    for (int j=0; j<10; j++)
    for (int i=0; i<10; i++) {
        if (10*j+i >= n_extensions) {
            break;
        }
        std::string ext = extensions[10*j+i];
        gridLayoutCustomFilter->addWidget(checkBoxCustomFilter[ext], i, j, Qt::AlignLeft);
    }
    gridLayoutCustomFilter->addWidget(buttonCustomFilterSelectAll, 0, 6, Qt::AlignLeft);
    gridLayoutCustomFilter->addWidget(buttonCustomFilterClearAll, 1, 6, Qt::AlignLeft);
    gridLayoutCustomFilter->setColumnStretch(6,1);
    groupBoxCustomFilter->setLayout(gridLayoutCustomFilter);

    if (dialog.opensave_custom_filter.contains("supported", Qt::CaseInsensitive)) buttonCustomFilterSelectAllClicked();

    //Opening
    QGroupBox* groupBoxOpening = new QGroupBox(tr("File Open"), widget);

    QComboBox* comboBoxOpenFormat = new QComboBox(groupBoxOpening);

    QCheckBox* checkBoxOpenThumbnail = new QCheckBox(tr("Preview Thumbnails"), groupBoxOpening);
    checkBoxOpenThumbnail->setChecked(false);

    //TODO: Add a button to clear the recent history.

    QLabel* labelRecentMaxFiles = new QLabel(tr("Number of recently accessed files to show"), groupBoxOpening);
    QSpinBox* spinBoxRecentMaxFiles = new QSpinBox(groupBoxOpening);
    spinBoxRecentMaxFiles->setRange(0, 10);
    dialog.opensave_recent_max_files = settings.opensave_recent_max_files;
    spinBoxRecentMaxFiles->setValue(dialog.opensave_recent_max_files);
    connect(
        spinBoxRecentMaxFiles,
        &QSpinBox::valueChanged,
        this,
        [=](int value) { dialog.opensave_recent_max_files = value; }
    );

    QFrame* frameRecent = new QFrame(groupBoxOpening);
    QGridLayout* gridLayoutRecent = new QGridLayout(frameRecent);
    gridLayoutRecent->addWidget(labelRecentMaxFiles, 0, 0, Qt::AlignLeft);
    gridLayoutRecent->addWidget(spinBoxRecentMaxFiles, 0, 1, Qt::AlignRight);
    frameRecent->setLayout(gridLayoutRecent);

    QVBoxLayout* vboxLayoutOpening = new QVBoxLayout(groupBoxOpening);
    vboxLayoutOpening->addWidget(comboBoxOpenFormat);
    vboxLayoutOpening->addWidget(checkBoxOpenThumbnail);
    vboxLayoutOpening->addWidget(frameRecent);
    groupBoxOpening->setLayout(vboxLayoutOpening);

    //Saving
    QGroupBox* groupBoxSaving = new QGroupBox(tr("File Save"), widget);

    QComboBox* comboBoxSaveFormat = new QComboBox(groupBoxSaving);

    QCheckBox* checkBoxSaveThumbnail = new QCheckBox(tr("Save Thumbnails"), groupBoxSaving);
    checkBoxSaveThumbnail->setChecked(false);

    QCheckBox* checkBoxAutoSave = new QCheckBox(tr("AutoSave"), groupBoxSaving);
    checkBoxAutoSave->setChecked(false);

    QVBoxLayout* vboxLayoutSaving = new QVBoxLayout(groupBoxSaving);
    vboxLayoutSaving->addWidget(comboBoxSaveFormat);
    vboxLayoutSaving->addWidget(checkBoxSaveThumbnail);
    vboxLayoutSaving->addWidget(checkBoxAutoSave);
    groupBoxSaving->setLayout(vboxLayoutSaving);

    //Trimming
    QGroupBox* groupBoxTrim = new QGroupBox(tr("Trimming"), widget);

    QLabel* labelTrimDstNumJumps = new QLabel(tr("DST Only: Minimum number of jumps to trim"), groupBoxTrim);
    QSpinBox* spinBoxTrimDstNumJumps = new QSpinBox(groupBoxTrim);
    spinBoxTrimDstNumJumps->setRange(1, 20);
    dialog.opensave_trim_dst_num_jumps = settings.opensave_trim_dst_num_jumps;
    spinBoxTrimDstNumJumps->setValue(dialog.opensave_trim_dst_num_jumps);
    connect(
        spinBoxTrimDstNumJumps,
        &QSpinBox::valueChanged,
        this,
        [=](int value){ dialog.opensave_trim_dst_num_jumps = value; }
    );

    QFrame* frameTrimDstNumJumps = new QFrame(groupBoxTrim);
    QGridLayout* gridLayoutTrimDstNumJumps = new QGridLayout(frameTrimDstNumJumps);
    gridLayoutTrimDstNumJumps->addWidget(labelTrimDstNumJumps, 0, 0, Qt::AlignLeft);
    gridLayoutTrimDstNumJumps->addWidget(spinBoxTrimDstNumJumps, 0, 1, Qt::AlignRight);
    frameTrimDstNumJumps->setLayout(gridLayoutTrimDstNumJumps);

    QVBoxLayout* vboxLayoutTrim = new QVBoxLayout(groupBoxTrim);
    vboxLayoutTrim->addWidget(frameTrimDstNumJumps);
    groupBoxTrim->setLayout(vboxLayoutTrim);

    //Widget Layout
    QVBoxLayout* vboxLayoutMain = new QVBoxLayout(widget);
    vboxLayoutMain->addWidget(groupBoxCustomFilter);
    vboxLayoutMain->addWidget(groupBoxOpening);
    vboxLayoutMain->addWidget(groupBoxSaving);
    vboxLayoutMain->addWidget(groupBoxTrim);
    vboxLayoutMain->addStretch(1);
    widget->setLayout(vboxLayoutMain);

    QScrollArea* scrollArea = new QScrollArea(this);
    scrollArea->setWidgetResizable(true);
    scrollArea->setWidget(widget);
    return scrollArea;
}

QWidget* Settings_Dialog::createTabPrinting()
{
    QWidget* widget = new QWidget(this);

    //Default Printer
    QGroupBox* groupBoxDefaultPrinter = new QGroupBox(tr("Default Printer"), widget);

    QRadioButton* radioButtonUseSame = new QRadioButton(tr("Use as default device"), groupBoxDefaultPrinter);
    radioButtonUseSame->setChecked(!settings.printing_use_last_device);
    QRadioButton* radioButtonUseLast = new QRadioButton(tr("Use last used device"), groupBoxDefaultPrinter);
    radioButtonUseLast->setChecked(settings.printing_use_last_device);

    QComboBox* comboBoxDefaultDevice = new QComboBox(groupBoxDefaultPrinter);
    QList<QPrinterInfo> listAvailPrinters = QPrinterInfo::availablePrinters();
    foreach (QPrinterInfo info, listAvailPrinters) {
        comboBoxDefaultDevice->addItem(_mainWin->create_icon("print"), info.printerName());
    }

    QVBoxLayout* vboxLayoutDefaultPrinter = new QVBoxLayout(groupBoxDefaultPrinter);
    vboxLayoutDefaultPrinter->addWidget(radioButtonUseSame);
    vboxLayoutDefaultPrinter->addWidget(comboBoxDefaultDevice);
    vboxLayoutDefaultPrinter->addWidget(radioButtonUseLast);
    groupBoxDefaultPrinter->setLayout(vboxLayoutDefaultPrinter);

    //Save Ink
    QGroupBox* groupBoxSaveInk = new QGroupBox(tr("Save Ink"), widget);

    QCheckBox* checkBoxDisableBG = new QCheckBox(tr("Disable Background"), groupBoxSaveInk);
    dialog.printing_disable_bg = settings.printing_disable_bg;
    checkBoxDisableBG->setChecked(dialog.printing_disable_bg);
    connect(
        checkBoxDisableBG,
        &QCheckBox::stateChanged,
        this,
        [=](int checked) { dialog.printing_disable_bg = checked; }
    );

    QVBoxLayout* vboxLayoutSaveInk = new QVBoxLayout(groupBoxSaveInk);
    vboxLayoutSaveInk->addWidget(checkBoxDisableBG);
    groupBoxSaveInk->setLayout(vboxLayoutSaveInk);

    //Widget Layout
    QVBoxLayout* vboxLayoutMain = new QVBoxLayout(widget);
    vboxLayoutMain->addWidget(groupBoxDefaultPrinter);
    vboxLayoutMain->addWidget(groupBoxSaveInk);
    vboxLayoutMain->addStretch(1);
    widget->setLayout(vboxLayoutMain);

    QScrollArea* scrollArea = new QScrollArea(this);
    scrollArea->setWidgetResizable(true);
    scrollArea->setWidget(widget);
    return scrollArea;
}

QWidget* Settings_Dialog::createTabSnap()
{
    QWidget* widget = new QWidget(this);

    //TODO: finish this

    QScrollArea* scrollArea = new QScrollArea(this);
    scrollArea->setWidgetResizable(true);
    scrollArea->setWidget(widget);
    return scrollArea;
}

void
Settings_Dialog::create_float_spinbox(
    QGroupBox *gb,
    QGridLayout* gridLayout,
    const char *label_in,
    EmbReal single_step,
    EmbReal lower,
    EmbReal upper,
    EmbReal *ptr,
    int row)
{
    QString label_object_name(label_in);
    QString spinbox_object_name(label_in);
    label_object_name = "label" + label_object_name.simplified().remove(' ');
    spinbox_object_name = "spinBox" + spinbox_object_name.simplified().remove(' ');
    QLabel* label = new QLabel(tr(label_in), gb);
    label->setObjectName(label_object_name);
    QDoubleSpinBox* spinBox = new QDoubleSpinBox(gb);
    spinBox->setObjectName(spinbox_object_name);
    spinBox->setSingleStep(single_step);
    spinBox->setRange(lower, upper);
    spinBox->setValue(*ptr);
    auto f = [=](EmbReal x){ *ptr = x; };
    connect(spinBox, SIGNAL(valueChanged(double)), this, SLOT(f));

    label->setEnabled(!dialog.grid_load_from_file);
    spinBox->setEnabled(!dialog.grid_load_from_file);

    bool visibility = (dialog_["grid_type"].s == "Circular");
    label->setVisible(!visibility);
    spinBox->setVisible(!visibility);

    gridLayout->addWidget(label, row, 0, Qt::AlignLeft);
    gridLayout->addWidget(spinBox, row, 1, Qt::AlignRight);
}

QWidget* Settings_Dialog::createTabGridRuler()
{
    QWidget* widget = new QWidget(this);

    //Grid Misc
    QGroupBox* groupBoxGridMisc = new QGroupBox(tr("Grid Misc"), widget);

    dialog.grid_show_on_load = settings.grid_show_on_load;
    dialog.grid_show_origin = settings.grid_show_origin;

    QCheckBox* checkBoxGridShowOnLoad = make_checkbox(groupBoxGridMisc,
        "Initially show grid when loading a file", "blank", &(dialog.grid_show_on_load));
    QCheckBox* checkBoxGridShowOrigin = make_checkbox(groupBoxGridMisc,
        "Show the origin when the grid is enabled", "blank", &(dialog.grid_show_origin));

    QGridLayout* gridLayoutGridMisc = new QGridLayout(widget);
    gridLayoutGridMisc->addWidget(checkBoxGridShowOnLoad, 0, 0, Qt::AlignLeft);
    gridLayoutGridMisc->addWidget(checkBoxGridShowOrigin, 1, 0, Qt::AlignLeft);
    groupBoxGridMisc->setLayout(gridLayoutGridMisc);

    //Grid Color
    QGroupBox* groupBoxGridColor = new QGroupBox(tr("Grid Color"), widget);

    QCheckBox* checkBoxGridColorMatchCrossHair = new QCheckBox(tr("Match grid color to crosshair color"), groupBoxGridColor);
    dialog.grid_color_match_crosshair = settings.grid_color_match_crosshair;
    checkBoxGridColorMatchCrossHair->setChecked(dialog.grid_color_match_crosshair);
    connect(checkBoxGridColorMatchCrossHair, SIGNAL(stateChanged(int)), this, SLOT(checkBoxGridColorMatchCrossHairStateChanged(int)));

    QLabel* labelGridColor = new QLabel(tr("Grid Color"), groupBoxGridColor);
    labelGridColor->setObjectName("labelGridColor");
    QPushButton* buttonGridColor = new QPushButton(tr("Choose"), groupBoxGridColor);
    buttonGridColor->setObjectName("buttonGridColor");
    if (dialog.grid_color_match_crosshair) {
        dialog.grid_color = settings.display_crosshair_color;
    }
    else {
        dialog.grid_color = settings.grid_color;
    }
    preview.grid_color = dialog.grid_color;
    accept_.grid_color  = dialog.grid_color;
    QPixmap gridPix(16,16);
    gridPix.fill(QColor(preview.grid_color));
    buttonGridColor->setIcon(QIcon(gridPix));
    connect(buttonGridColor, SIGNAL(clicked()), this, SLOT(chooseGridColor()));

    labelGridColor->setEnabled(!dialog.grid_color_match_crosshair);
    buttonGridColor->setEnabled(!dialog.grid_color_match_crosshair);

    QGridLayout* gridLayoutGridColor = new QGridLayout(widget);
    gridLayoutGridColor->addWidget(checkBoxGridColorMatchCrossHair, 0, 0, Qt::AlignLeft);
    gridLayoutGridColor->addWidget(labelGridColor, 1, 0, Qt::AlignLeft);
    gridLayoutGridColor->addWidget(buttonGridColor, 1, 1, Qt::AlignRight);
    groupBoxGridColor->setLayout(gridLayoutGridColor);

    //Grid Geometry
    QGroupBox* groupBoxGridGeom = new QGroupBox(tr("Grid Geometry"), widget);

    dialog.grid_load_from_file = settings.grid_load_from_file;
    dialog_["grid_type"] = settings_["grid_type"];
    dialog.grid_center_on_origin = settings.grid_center_on_origin;
    dialog.grid_center = settings.grid_center;
    dialog.grid_size = settings.grid_size;
    dialog.grid_spacing = settings.grid_spacing;

    QCheckBox* checkBoxGridLoadFromFile = new QCheckBox(tr("Set grid size from opened file"), groupBoxGridGeom);
    checkBoxGridLoadFromFile->setChecked(dialog.grid_load_from_file);
    connect(checkBoxGridLoadFromFile, SIGNAL(stateChanged(int)), this, SLOT(checkBoxGridLoadFromFileStateChanged(int)));

    QLabel* labelGridType = new QLabel(tr("Grid Type"), groupBoxGridGeom);
    labelGridType->setObjectName("labelGridType");
    QComboBox* comboBoxGridType = new QComboBox(groupBoxGridGeom);
    comboBoxGridType->setObjectName("comboBoxGridType");
    comboBoxGridType->addItem("Rectangular");
    comboBoxGridType->addItem("Circular");
    comboBoxGridType->addItem("Isometric");
    comboBoxGridType->setCurrentIndex(comboBoxGridType->findText(QString::fromStdString(dialog_["grid_type"].s)));
    connect(comboBoxGridType, SIGNAL(currentIndexChanged(QString)), this, SLOT(comboBoxGridTypeCurrentIndexChanged(QString)));

    QCheckBox* checkBoxGridCenterOnOrigin = new QCheckBox(tr("Center the grid on the origin"), groupBoxGridGeom);
    checkBoxGridCenterOnOrigin->setObjectName("checkBoxGridCenterOnOrigin");
    checkBoxGridCenterOnOrigin->setChecked(dialog.grid_center_on_origin);
    connect(checkBoxGridCenterOnOrigin, SIGNAL(stateChanged(int)), this, SLOT(checkBoxGridCenterOnOriginStateChanged(int)));

    labelGridType->setEnabled(!dialog.grid_load_from_file);
    comboBoxGridType->setEnabled(!dialog.grid_load_from_file);
    checkBoxGridCenterOnOrigin->setEnabled(!dialog.grid_load_from_file);

    QGridLayout* gridLayoutGridGeom = new QGridLayout(groupBoxGridGeom);
    gridLayoutGridGeom->addWidget(checkBoxGridLoadFromFile, 0, 0, Qt::AlignLeft);
    gridLayoutGridGeom->addWidget(labelGridType, 1, 0, Qt::AlignLeft);
    gridLayoutGridGeom->addWidget(comboBoxGridType, 1, 1, Qt::AlignRight);
    gridLayoutGridGeom->addWidget(checkBoxGridCenterOnOrigin, 2, 0, Qt::AlignLeft);
    create_float_spinbox(groupBoxGridGeom, gridLayoutGridGeom,
        "Grid Center X", 1.0, -1000.0, 1000.0, &(dialog.grid_center.x), 3);
    create_float_spinbox(groupBoxGridGeom, gridLayoutGridGeom,
        "Grid Center Y", 1.0, -1000.0, 1000.0, &(dialog.grid_center.y), 4);
    create_float_spinbox(groupBoxGridGeom, gridLayoutGridGeom,
        "Grid Size X", 1.0, 1.0, 1000.0, &(dialog.grid_size.x), 5);
    create_float_spinbox(groupBoxGridGeom, gridLayoutGridGeom,
        "Grid Size Y", 1.0, 1.0, 1000.0, &(dialog.grid_size.y), 6);
    create_float_spinbox(groupBoxGridGeom, gridLayoutGridGeom,
        "Grid Spacing X", 1.0, 0.001, 1000.0, &(dialog.grid_spacing.x), 7);
    create_float_spinbox(groupBoxGridGeom, gridLayoutGridGeom,
        "Grid Spacing Y", 1.0, 0.001, 1000.0, &(dialog.grid_spacing.y), 8);
    create_float_spinbox(groupBoxGridGeom, gridLayoutGridGeom,
        "Grid Size Radius", 1.0, 1.0, 1000.0, &(dialog.grid_size_radius), 9);
    create_float_spinbox(groupBoxGridGeom, gridLayoutGridGeom,
        "Grid Spacing Radius", 1.0, 1.0, 1000.0, &(dialog.grid_spacing_radius), 10);
    create_float_spinbox(groupBoxGridGeom, gridLayoutGridGeom,
        "Grid Spacing Angle", 1.0, 0.001, 1000.0, &(dialog.grid_spacing_angle), 11);
    groupBoxGridGeom->setLayout(gridLayoutGridGeom);

    //Ruler Misc
    QGroupBox* groupBoxRulerMisc = new QGroupBox(tr("Ruler Misc"), widget);

    QCheckBox* checkBoxRulerShowOnLoad = new QCheckBox(tr("Initially show ruler when loading a file"), groupBoxRulerMisc);
    dialog.ruler_show_on_load = settings.ruler_show_on_load;
    checkBoxRulerShowOnLoad->setChecked(dialog.ruler_show_on_load);
    connect(checkBoxRulerShowOnLoad, SIGNAL(stateChanged(int)), this, SLOT(checkBoxRulerShowOnLoadStateChanged(int)));

    QLabel* labelRulerMetric = new QLabel(tr("Ruler Units"), groupBoxRulerMisc);
    QComboBox* comboBoxRulerMetric = new QComboBox(groupBoxRulerMisc);
    comboBoxRulerMetric->addItem("Imperial", false);
    comboBoxRulerMetric->addItem("Metric", true);
    dialog.ruler_metric = settings.ruler_metric;
    comboBoxRulerMetric->setCurrentIndex(comboBoxRulerMetric->findData(dialog.ruler_metric));
    connect(comboBoxRulerMetric, SIGNAL(currentIndexChanged(int)), this, SLOT(comboBoxRulerMetricCurrentIndexChanged(int)));

    QGridLayout* gridLayoutRulerMisc = new QGridLayout(widget);
    gridLayoutRulerMisc->addWidget(checkBoxRulerShowOnLoad, 0, 0, Qt::AlignLeft);
    gridLayoutRulerMisc->addWidget(labelRulerMetric, 1, 0, Qt::AlignLeft);
    gridLayoutRulerMisc->addWidget(comboBoxRulerMetric, 1, 1, Qt::AlignRight);
    groupBoxRulerMisc->setLayout(gridLayoutRulerMisc);

    //Ruler Color
    QGroupBox* groupBoxRulerColor = new QGroupBox(tr("Ruler Color"), widget);

    QLabel* labelRulerColor = new QLabel(tr("Ruler Color"), groupBoxRulerColor);
    labelRulerColor->setObjectName("labelRulerColor");
    QPushButton* buttonRulerColor = new QPushButton(tr("Choose"), groupBoxRulerColor);
    buttonRulerColor->setObjectName("buttonRulerColor");
    dialog.ruler_color = settings.ruler_color;
    preview.ruler_color = dialog.ruler_color;
    accept_.ruler_color  = dialog.ruler_color;
    QPixmap rulerPix(16,16);
    rulerPix.fill(QColor(preview.ruler_color));
    buttonRulerColor->setIcon(QIcon(rulerPix));
    connect(buttonRulerColor, SIGNAL(clicked()), this, SLOT(chooseRulerColor()));

    QGridLayout* gridLayoutRulerColor = new QGridLayout(widget);
    gridLayoutRulerColor->addWidget(labelRulerColor, 1, 0, Qt::AlignLeft);
    gridLayoutRulerColor->addWidget(buttonRulerColor, 1, 1, Qt::AlignRight);
    groupBoxRulerColor->setLayout(gridLayoutRulerColor);

    //Ruler Geometry
    QGroupBox* groupBoxRulerGeom = new QGroupBox(tr("Ruler Geometry"), widget);

    QLabel* labelRulerPixelSize = new QLabel(tr("Ruler Pixel Size"), groupBoxRulerGeom);
    labelRulerPixelSize->setObjectName("labelRulerPixelSize");
    QDoubleSpinBox* spinBoxRulerPixelSize = new QDoubleSpinBox(groupBoxRulerGeom);
    spinBoxRulerPixelSize->setObjectName("spinBoxRulerPixelSize");
    dialog.ruler_pixel_size = settings.ruler_pixel_size;
    spinBoxRulerPixelSize->setSingleStep(1.000);
    spinBoxRulerPixelSize->setRange(20.000, 100.000);
    spinBoxRulerPixelSize->setValue(dialog.ruler_pixel_size);
    connect(spinBoxRulerPixelSize, SIGNAL(valueChanged(double)), this, SLOT(spinBoxRulerPixelSizeValueChanged(double)));

    QGridLayout* gridLayoutRulerGeom = new QGridLayout(groupBoxRulerGeom);
    gridLayoutRulerGeom->addWidget(labelRulerPixelSize, 0, 0, Qt::AlignLeft);
    gridLayoutRulerGeom->addWidget(spinBoxRulerPixelSize, 0, 1, Qt::AlignRight);
    groupBoxRulerGeom->setLayout(gridLayoutRulerGeom);

    //Widget Layout
    QVBoxLayout *vboxLayoutMain = new QVBoxLayout(widget);
    vboxLayoutMain->addWidget(groupBoxGridMisc);
    vboxLayoutMain->addWidget(groupBoxGridColor);
    vboxLayoutMain->addWidget(groupBoxGridGeom);
    vboxLayoutMain->addWidget(groupBoxRulerMisc);
    vboxLayoutMain->addWidget(groupBoxRulerColor);
    vboxLayoutMain->addWidget(groupBoxRulerGeom);
    vboxLayoutMain->addStretch(1);
    widget->setLayout(vboxLayoutMain);

    QScrollArea* scrollArea = new QScrollArea(this);
    scrollArea->setWidgetResizable(true);
    scrollArea->setWidget(widget);
    return scrollArea;
}

QWidget* Settings_Dialog::createTabOrthoPolar()
{
    QWidget* widget = new QWidget(this);

    //TODO: finish this

    QScrollArea* scrollArea = new QScrollArea(this);
    scrollArea->setWidgetResizable(true);
    scrollArea->setWidget(widget);
    return scrollArea;
}

QWidget* Settings_Dialog::createTabQuickSnap()
{
    QWidget* widget = new QWidget(this);

    //QSnap Locators
    QGroupBox* groupBoxQSnapLoc = new QGroupBox(tr("Locators Used"), widget);

    qsnap_endpoint_cb = get_bool(settings_, "qsnap_endpoint");
    qsnap_midpoint_cb = get_bool(settings_, "qsnap_midpoint");
    qsnap_center_cb = get_bool(settings_, "qsnap_center");
    qsnap_node_cb = get_bool(settings_, "qsnap_node");
    qsnap_quadrant_cb = get_bool(settings_, "qsnap_quadrant");
    qsnap_intersection_cb = get_bool(settings_, "qsnap_intersection");
    qsnap_extension_cb = get_bool(settings_, "qsnap_extension");
    qsnap_insertion_cb = get_bool(settings_, "qsnap_insertion");
    qsnap_perpendicular_cb = get_bool(settings_, "qsnap_perpendicular");
    qsnap_tangent_cb = get_bool(settings_, "qsnap_tangent");
    qsnap_nearest_cb = get_bool(settings_, "qsnap_nearest");
    qsnap_apparent_cb = get_bool(settings_, "qsnap_apparent");
    qsnap_parallel_cb = get_bool(settings_, "qsnap_parallel");

    std::vector<QCheckBox*> checkboxes = {
        make_checkbox(groupBoxQSnapLoc, "Endpoint", "locator-snaptoendpoint", &qsnap_endpoint_cb),
        make_checkbox(groupBoxQSnapLoc, "Midpoint", "locator-snaptomidpoint", &qsnap_midpoint_cb),
        make_checkbox(groupBoxQSnapLoc, "Center", "locator-snaptocenter", &qsnap_center_cb),
        make_checkbox(groupBoxQSnapLoc, "Node", "locator-snaptonode", &qsnap_node_cb),
        make_checkbox(groupBoxQSnapLoc, "Quadrant", "locator-snaptoquadrant", &qsnap_quadrant_cb),
        make_checkbox(groupBoxQSnapLoc, "Intersection", "locator-snaptointersection", &qsnap_intersection_cb),
        make_checkbox(groupBoxQSnapLoc, "Extension", "locator-snaptoextension", &qsnap_extension_cb),
        make_checkbox(groupBoxQSnapLoc, "Insertion", "locator-snaptoinsert", &qsnap_insertion_cb),
        make_checkbox(groupBoxQSnapLoc, "Perpendicular", "locator-snaptoperpendicular", &qsnap_perpendicular_cb),
        make_checkbox(groupBoxQSnapLoc, "Tangent", "locator-snaptotangent", &qsnap_tangent_cb),
        make_checkbox(groupBoxQSnapLoc, "Nearest", "locator-snaptonearest", &qsnap_nearest_cb),
        make_checkbox(groupBoxQSnapLoc, "Apparent Intersection", "locator-snaptoapparentintersection", &qsnap_apparent_cb),
        make_checkbox(groupBoxQSnapLoc, "Parallel", "locator-snaptoparallel", &qsnap_parallel_cb)
    };
    int n_checkboxes = (int)checkboxes.size();

    QPushButton* buttonQSnapSelectAll = new QPushButton(tr("Select All"), groupBoxQSnapLoc);
    connect(buttonQSnapSelectAll, SIGNAL(clicked()), this, SLOT(buttonQSnapSelectAllClicked()));
    for (int i=0; i<n_checkboxes; i++) {
        connect(this, SIGNAL(buttonQSnapSelectAll(bool)), checkboxes[i], SLOT(setChecked(bool)));
    }

    QPushButton* buttonQSnapClearAll = new QPushButton(tr("Clear All"), groupBoxQSnapLoc);
    connect(buttonQSnapClearAll, SIGNAL(clicked()), this, SLOT(buttonQSnapClearAllClicked()));
    for (int i=0; i<n_checkboxes; i++) {
        connect(this, SIGNAL(buttonQSnapClearAll(bool)), checkboxes[i], SLOT(setChecked(bool)));
    }

    QGridLayout* gridLayoutQSnap = new QGridLayout(groupBoxQSnapLoc);
    for (int i=0; i<n_checkboxes; i++) {
        gridLayoutQSnap->addWidget(checkboxes[i], i%7, i/7, Qt::AlignLeft);
    }
    gridLayoutQSnap->addWidget(buttonQSnapSelectAll, 0, 2, Qt::AlignLeft);
    gridLayoutQSnap->addWidget(buttonQSnapClearAll, 1, 2, Qt::AlignLeft);
    gridLayoutQSnap->setColumnStretch(2,1);
    groupBoxQSnapLoc->setLayout(gridLayoutQSnap);

    //QSnap Visual Config
    QGroupBox* groupBoxQSnapVisual = new QGroupBox(tr("Visual Configuration"), widget);

    QLabel* labelQSnapLocColor = new QLabel(tr("Locator Color"), groupBoxQSnapVisual);
    QComboBox* comboBoxQSnapLocColor = new QComboBox(groupBoxQSnapVisual);
    addColorsToComboBox(comboBoxQSnapLocColor);
    dialog.qsnap_locator_color = settings.qsnap_locator_color;
    comboBoxQSnapLocColor->setCurrentIndex(comboBoxQSnapLocColor->findData(dialog.qsnap_locator_color));
    connect(comboBoxQSnapLocColor, SIGNAL(currentIndexChanged(int)), this, SLOT(comboBoxQSnapLocatorColorCurrentIndexChanged(int)));

    QLabel* labelQSnapLocSize = new QLabel(tr("Locator Size"), groupBoxQSnapVisual);
    QSlider* sliderQSnapLocSize = new QSlider(Qt::Horizontal, groupBoxQSnapVisual);
    sliderQSnapLocSize->setRange(1,20);
    dialog.qsnap_locator_size = settings.qsnap_locator_size;
    sliderQSnapLocSize->setValue(dialog.qsnap_locator_size);
    auto f = [=](int x) { dialog.qsnap_locator_size = x; };
    connect(sliderQSnapLocSize, SIGNAL(valueChanged(int)), this, SLOT(f));

    QVBoxLayout* vboxLayoutQSnapVisual = new QVBoxLayout(groupBoxQSnapVisual);
    vboxLayoutQSnapVisual->addWidget(labelQSnapLocColor);
    vboxLayoutQSnapVisual->addWidget(comboBoxQSnapLocColor);
    vboxLayoutQSnapVisual->addWidget(labelQSnapLocSize);
    vboxLayoutQSnapVisual->addWidget(sliderQSnapLocSize);
    groupBoxQSnapVisual->setLayout(vboxLayoutQSnapVisual);

    //QSnap Sensitivity Config
    QGroupBox* groupBoxQSnapSensitivity = new QGroupBox(tr("Sensitivity"), widget);

    QLabel* labelQSnapApertureSize = new QLabel(tr("Aperture Size"), groupBoxQSnapSensitivity);
    QSlider* sliderQSnapApertureSize = new QSlider(Qt::Horizontal, groupBoxQSnapSensitivity);
    sliderQSnapApertureSize->setRange(1,20);
    dialog.qsnap_aperture_size = settings.qsnap_aperture_size;
    sliderQSnapApertureSize->setValue(dialog.qsnap_aperture_size);
    connect(sliderQSnapApertureSize, SIGNAL(valueChanged(int)), this,
        SLOT([=](int x) { dialog.qsnap_aperture_size = x; } ));

    QVBoxLayout* vboxLayoutQSnapSensitivity = new QVBoxLayout(groupBoxQSnapSensitivity);
    vboxLayoutQSnapSensitivity->addWidget(labelQSnapApertureSize);
    vboxLayoutQSnapSensitivity->addWidget(sliderQSnapApertureSize);
    groupBoxQSnapSensitivity->setLayout(vboxLayoutQSnapSensitivity);

    //Widget Layout
    QVBoxLayout *vboxLayoutMain = new QVBoxLayout(widget);
    vboxLayoutMain->addWidget(groupBoxQSnapLoc);
    vboxLayoutMain->addWidget(groupBoxQSnapVisual);
    vboxLayoutMain->addWidget(groupBoxQSnapSensitivity);
    vboxLayoutMain->addStretch(1);
    widget->setLayout(vboxLayoutMain);

    QScrollArea* scrollArea = new QScrollArea(this);
    scrollArea->setWidgetResizable(true);
    scrollArea->setWidget(widget);
    return scrollArea;
}

QWidget* Settings_Dialog::createTabQuickTrack()
{
    QWidget* widget = new QWidget(this);

    //TODO: finish this

    QScrollArea* scrollArea = new QScrollArea(this);
    scrollArea->setWidgetResizable(true);
    scrollArea->setWidget(widget);
    return scrollArea;
}

QWidget* Settings_Dialog::createTabLineWeight()
{
    QWidget* widget = new QWidget(this);

    //TODO: finish this

    //Misc
    QGroupBox* groupBoxLwtMisc = new QGroupBox(tr("LineWeight Misc"), widget);

    QGraphicsScene* s = activeScene();

    QCheckBox* checkBoxShowLwt = new QCheckBox(tr("Show LineWeight"), groupBoxLwtMisc);
    if (s) {
        dialog.lwt_show_lwt = s->property("ENABLE_LWT").toBool();
    }
    else {
        dialog.lwt_show_lwt = settings.lwt_show_lwt;
    }
    preview.lwt_show_lwt = dialog.lwt_show_lwt;
    checkBoxShowLwt->setChecked(preview.lwt_show_lwt);
    connect(checkBoxShowLwt, SIGNAL(stateChanged(int)), this, SLOT(checkBoxLwtShowLwtStateChanged(int)));

    QCheckBox* checkBoxRealRender = new QCheckBox(tr("RealRender"), groupBoxLwtMisc);
    checkBoxRealRender->setObjectName("checkBoxRealRender");
    if (s) {
        dialog.lwt_real_render = s->property("ENABLE_REAL").toBool();
    }
    else  {
        dialog.lwt_real_render = settings.lwt_real_render;
    }
    preview.lwt_real_render = dialog.lwt_real_render;
    checkBoxRealRender->setChecked(preview.lwt_real_render);
    connect(checkBoxRealRender, SIGNAL(stateChanged(int)), this, SLOT(checkBoxLwtRealRenderStateChanged(int)));
    checkBoxRealRender->setEnabled(dialog.lwt_show_lwt);

    QLabel* labelDefaultLwt = new QLabel(tr("Default weight"), groupBoxLwtMisc);
    labelDefaultLwt->setEnabled(false); //TODO: remove later
    QComboBox* comboBoxDefaultLwt = new QComboBox(groupBoxLwtMisc);
    dialog.lwt_default_lwt = settings.lwt_default_lwt;
    //TODO: populate the comboBox and set the initial value
    comboBoxDefaultLwt->addItem(QString().setNum(dialog.lwt_default_lwt, 'F', 2).append(" mm"), dialog.lwt_default_lwt);
    comboBoxDefaultLwt->setEnabled(false); //TODO: remove later

    QVBoxLayout* vboxLayoutLwtMisc = new QVBoxLayout(groupBoxLwtMisc);
    vboxLayoutLwtMisc->addWidget(checkBoxShowLwt);
    vboxLayoutLwtMisc->addWidget(checkBoxRealRender);
    vboxLayoutLwtMisc->addWidget(labelDefaultLwt);
    vboxLayoutLwtMisc->addWidget(comboBoxDefaultLwt);
    groupBoxLwtMisc->setLayout(vboxLayoutLwtMisc);

    //Widget Layout
    QVBoxLayout *vboxLayoutMain = new QVBoxLayout(widget);
    vboxLayoutMain->addWidget(groupBoxLwtMisc);
    vboxLayoutMain->addStretch(1);
    widget->setLayout(vboxLayoutMain);

    QScrollArea* scrollArea = new QScrollArea(this);
    scrollArea->setWidgetResizable(true);
    scrollArea->setWidget(widget);
    return scrollArea;
}

QWidget* Settings_Dialog::createTabSelection()
{
    QWidget* widget = new QWidget(this);

    //Selection Modes
    QGroupBox* groupBoxSelectionModes = new QGroupBox(tr("Modes"), widget);

    QCheckBox* checkBoxSelectionModePickFirst = new QCheckBox(tr("Allow Preselection (PickFirst)"), groupBoxSelectionModes);
    dialog.selection_mode_pickfirst = settings.selection_mode_pickfirst;
    checkBoxSelectionModePickFirst->setChecked(dialog.selection_mode_pickfirst);
    checkBoxSelectionModePickFirst->setChecked(true);
    checkBoxSelectionModePickFirst->setEnabled(false); //TODO: Remove this line when Post-selection is available
    connect(checkBoxSelectionModePickFirst, SIGNAL(stateChanged(int)), this,
        SLOT([=](int x) {dialog.selection_mode_pickfirst = x; } ));

    QCheckBox* checkBoxSelectionModePickAdd = new QCheckBox(tr("Add to Selection (PickAdd)"), groupBoxSelectionModes);
    dialog.selection_mode_pickadd = settings.selection_mode_pickadd;
    checkBoxSelectionModePickAdd->setChecked(dialog.selection_mode_pickadd);
    connect(checkBoxSelectionModePickAdd, SIGNAL(stateChanged(int)), this,
        SLOT([=](int x) { dialog.selection_mode_pickadd = x; } ));

    QCheckBox* checkBoxSelectionModePickDrag = new QCheckBox(tr("Drag to Select (PickDrag)"), groupBoxSelectionModes);
    dialog.selection_mode_pickdrag = settings.selection_mode_pickdrag;
    checkBoxSelectionModePickDrag->setChecked(dialog.selection_mode_pickdrag);
    checkBoxSelectionModePickDrag->setChecked(false);
    checkBoxSelectionModePickDrag->setEnabled(false); //TODO: Remove this line when this functionality is available
    connect(checkBoxSelectionModePickDrag, SIGNAL(stateChanged(int)), this,    SLOT([=](int x) {dialog.selection_mode_pickdrag = x; } ));

    QVBoxLayout* vboxLayoutSelectionModes = new QVBoxLayout(groupBoxSelectionModes);
    vboxLayoutSelectionModes->addWidget(checkBoxSelectionModePickFirst);
    vboxLayoutSelectionModes->addWidget(checkBoxSelectionModePickAdd);
    vboxLayoutSelectionModes->addWidget(checkBoxSelectionModePickDrag);
    groupBoxSelectionModes->setLayout(vboxLayoutSelectionModes);

    //Selection Colors
    QGroupBox* groupBoxSelectionColors = new QGroupBox(tr("Colors"), widget);

    QLabel* labelCoolGripColor = new QLabel(tr("Cool Grip (Unselected)"), groupBoxSelectionColors);
    QComboBox* comboBoxCoolGripColor = new QComboBox(groupBoxSelectionColors);
    addColorsToComboBox(comboBoxCoolGripColor);
    dialog.selection_coolgrip_color = settings.selection_coolgrip_color;
    comboBoxCoolGripColor->setCurrentIndex(comboBoxCoolGripColor->findData(dialog.selection_coolgrip_color));
    connect(comboBoxCoolGripColor, SIGNAL(currentIndexChanged(int)), this,
        SLOT([=](int x) { dialog.selection_coolgrip_color = x; } ));

    QLabel* labelHotGripColor = new QLabel(tr("Hot Grip (Selected)"), groupBoxSelectionColors);
    QComboBox* comboBoxHotGripColor = new QComboBox(groupBoxSelectionColors);
    addColorsToComboBox(comboBoxHotGripColor);
    dialog.selection_hotgrip_color = settings.selection_hotgrip_color;
    comboBoxHotGripColor->setCurrentIndex(comboBoxHotGripColor->findData(dialog.selection_hotgrip_color));
    connect(comboBoxHotGripColor, SIGNAL(currentIndexChanged(int)), this,
        SLOT([=](int x) { dialog.selection_hotgrip_color = x; } ));

    QVBoxLayout* vboxLayoutSelectionColors = new QVBoxLayout(groupBoxSelectionColors);
    vboxLayoutSelectionColors->addWidget(labelCoolGripColor);
    vboxLayoutSelectionColors->addWidget(comboBoxCoolGripColor);
    vboxLayoutSelectionColors->addWidget(labelHotGripColor);
    vboxLayoutSelectionColors->addWidget(comboBoxHotGripColor);
    groupBoxSelectionColors->setLayout(vboxLayoutSelectionColors);

    //Selection Sizes
    QGroupBox* groupBoxSelectionSizes = new QGroupBox(tr("Sizes"), widget);

    QLabel* labelSelectionGripSize = new QLabel(tr("Grip Size"), groupBoxSelectionSizes);
    QSlider* sliderSelectionGripSize = new QSlider(Qt::Horizontal, groupBoxSelectionSizes);
    sliderSelectionGripSize->setRange(1,20);
    dialog.selection_grip_size = settings.selection_grip_size;
    sliderSelectionGripSize->setValue(dialog.selection_grip_size);
    connect(sliderSelectionGripSize, SIGNAL(valueChanged(int)), this,
        SLOT([=](int x) { dialog.selection_grip_size = x; } ));

    QLabel* labelSelectionPickBoxSize = new QLabel(tr("Pickbox Size"), groupBoxSelectionSizes);
    QSlider* sliderSelectionPickBoxSize = new QSlider(Qt::Horizontal, groupBoxSelectionSizes);
    sliderSelectionPickBoxSize->setRange(1,20);
    dialog.selection_pickbox_size = settings.selection_pickbox_size;
    sliderSelectionPickBoxSize->setValue(dialog.selection_pickbox_size);
    connect(sliderSelectionPickBoxSize, SIGNAL(valueChanged(int)), this,
        SLOT([=](int x) { dialog.selection_pickbox_size = x; } ));

    QVBoxLayout* vboxLayoutSelectionSizes = new QVBoxLayout(groupBoxSelectionSizes);
    vboxLayoutSelectionSizes->addWidget(labelSelectionGripSize);
    vboxLayoutSelectionSizes->addWidget(sliderSelectionGripSize);
    vboxLayoutSelectionSizes->addWidget(labelSelectionPickBoxSize);
    vboxLayoutSelectionSizes->addWidget(sliderSelectionPickBoxSize);
    groupBoxSelectionSizes->setLayout(vboxLayoutSelectionSizes);

    //Widget Layout
    QVBoxLayout *vboxLayoutMain = new QVBoxLayout(widget);
    vboxLayoutMain->addWidget(groupBoxSelectionModes);
    vboxLayoutMain->addWidget(groupBoxSelectionColors);
    vboxLayoutMain->addWidget(groupBoxSelectionSizes);
    vboxLayoutMain->addStretch(1);
    widget->setLayout(vboxLayoutMain);

    QScrollArea* scrollArea = new QScrollArea(this);
    scrollArea->setWidgetResizable(true);
    scrollArea->setWidget(widget);
    return scrollArea;
}

void Settings_Dialog::addColorsToComboBox(QComboBox* comboBox)
{
    comboBox->addItem(_mainWin->create_icon("colorred"), tr("Red"), qRgb(255, 0, 0));
    comboBox->addItem(_mainWin->create_icon("coloryellow"), tr("Yellow"), qRgb(255,255, 0));
    comboBox->addItem(_mainWin->create_icon("colorgreen"), tr("Green"), qRgb(0, 255, 0));
    comboBox->addItem(_mainWin->create_icon("colorcyan"), tr("Cyan"), qRgb(0, 255, 255));
    comboBox->addItem(_mainWin->create_icon("colorblue"), tr("Blue"), qRgb(0, 0, 255));
    comboBox->addItem(_mainWin->create_icon("colormagenta"), tr("Magenta"), qRgb(255, 0,255));
    comboBox->addItem(_mainWin->create_icon("colorwhite"), tr("White"), qRgb(255,255,255));
    //TODO: Add Other... so the user can select custom colors
}

/**
 * @brief Settings_Dialog::comboBoxIconSizeCurrentIndexChanged
 * @param index
 */
void
Settings_Dialog::comboBoxIconSizeCurrentIndexChanged(int index)
{
    QComboBox* comboBox = qobject_cast<QComboBox*>(sender());
    if (comboBox) {
        bool ok = 0;
        dialog.general_icon_size = comboBox->itemData(index).toUInt(&ok);
        if (!ok) {
            dialog.general_icon_size = 16;
        }
    }
    else {
        dialog.general_icon_size = 16;
    }
}

void Settings_Dialog::checkBoxGeneralMdiBGUseLogoStateChanged(int checked)
{
    preview.general_mdi_bg_use_logo = checked;
    mdiArea->useBackgroundLogo(checked);
}

void Settings_Dialog::chooseGeneralMdiBackgroundLogo()
{
    QPushButton* button = qobject_cast<QPushButton*>(sender());
    if (button) {
        QString selectedImage;
        selectedImage = QFileDialog::getOpenFileName(this, tr("Open File"),
                        QStandardPaths::writableLocation(QStandardPaths::PicturesLocation),
                        tr("Images (*.bmp *.png *.jpg)"));

        if (!selectedImage.isNull()) {
            accept__["general_mdi_bg_logo"] = node(selectedImage.toStdString());
        }

        //Update immediately so it can be previewed
        mdiArea->setBackgroundLogo(QString::fromStdString(get_str(accept__, "general_mdi_bg_logo")));
    }
}

/**
 * @brief Settings_Dialog::checkBoxGeneralMdiBGUseTextureStateChanged
 * @param checked
 */
void
Settings_Dialog::checkBoxGeneralMdiBGUseTextureStateChanged(int checked)
{
    preview.general_mdi_bg_use_texture = checked;
    mdiArea->useBackgroundTexture(checked);
}

void Settings_Dialog::chooseGeneralMdiBackgroundTexture()
{
    QPushButton* button = qobject_cast<QPushButton*>(sender());
    if (button) {
        QString selectedImage;
        selectedImage = QFileDialog::getOpenFileName(this, tr("Open File"),
                        QStandardPaths::writableLocation(QStandardPaths::PicturesLocation),
                        tr("Images (*.bmp *.png *.jpg)"));

        if (!selectedImage.isNull()) {
            accept_.general_mdi_bg_texture = selectedImage.toStdString();
        }

        //Update immediately so it can be previewed
        mdiArea->setBackgroundTexture(QString::fromStdString(accept_.general_mdi_bg_texture));
    }
}

void Settings_Dialog::checkBoxGeneralMdiBGUseColorStateChanged(int checked)
{
    preview.general_mdi_bg_use_color = checked;
    mdiArea->useBackgroundColor(checked);
}

void Settings_Dialog::chooseGeneralMdiBackgroundColor()
{
    QPushButton* button = qobject_cast<QPushButton*>(sender());
    if (button) {
        QColorDialog* colorDialog = new QColorDialog(QColor(accept_.general_mdi_bg_color), this);
        connect(colorDialog, SIGNAL(currentColorChanged(const QColor&)), this, SLOT(currentGeneralMdiBackgroundColorChanged(const QColor&)));
        colorDialog->exec();

        if (colorDialog->result() == QDialog::Accepted) {
            accept_.general_mdi_bg_color = colorDialog->selectedColor().rgb();
            QPixmap pix(16,16);
            pix.fill(QColor(accept_.general_mdi_bg_color));
            button->setIcon(QIcon(pix));
            mdiArea->setBackgroundColor(QColor(accept_.general_mdi_bg_color));
        }
        else {
            mdiArea->setBackgroundColor(QColor(dialog.general_mdi_bg_color));
        }
    }
}

void Settings_Dialog::currentGeneralMdiBackgroundColorChanged(const QColor& color)
{
    preview.general_mdi_bg_color = color.rgb();
    mdiArea->setBackgroundColor(QColor(preview.general_mdi_bg_color));
}

void Settings_Dialog::checkBoxShowScrollBarsStateChanged(int checked)
{
    preview.display_show_scrollbars = checked;
    _mainWin->updateAllViewScrollBars(preview.display_show_scrollbars);
}

void Settings_Dialog::comboBoxScrollBarWidgetCurrentIndexChanged(int index)
{
    dialog.display_scrollbar_widget_num = index;
}

void Settings_Dialog::chooseDisplayCrossHairColor()
{
    QPushButton* button = qobject_cast<QPushButton*>(sender());
    if (button) {
        QColorDialog* colorDialog = new QColorDialog(QColor(accept_.display_crosshair_color), this);
        connect(colorDialog, SIGNAL(currentColorChanged(const QColor&)), this, SLOT(currentDisplayCrossHairColorChanged(const QColor&)));
        colorDialog->exec();

        if (colorDialog->result() == QDialog::Accepted) {
            accept_.display_crosshair_color = colorDialog->selectedColor().rgb();
            QPixmap pix(16,16);
            pix.fill(QColor(accept_.display_crosshair_color));
            button->setIcon(QIcon(pix));
            _mainWin->updateAllViewCrossHairColors(accept_.display_crosshair_color);
        }
        else {
            _mainWin->updateAllViewCrossHairColors(dialog.display_crosshair_color);
        }
    }
}

void Settings_Dialog::currentDisplayCrossHairColorChanged(const QColor& color)
{
    preview.display_crosshair_color = color.rgb();
    _mainWin->updateAllViewCrossHairColors(preview.display_crosshair_color);
}

void Settings_Dialog::chooseDisplayBackgroundColor()
{
    QPushButton* button = qobject_cast<QPushButton*>(sender());
    if (button) {
        QColorDialog* colorDialog = new QColorDialog(QColor(accept_.display_bg_color), this);
        connect(colorDialog, SIGNAL(currentColorChanged(const QColor&)), this, SLOT(currentDisplayBackgroundColorChanged(const QColor&)));
        colorDialog->exec();

        if (colorDialog->result() == QDialog::Accepted) {
            accept_.display_bg_color = colorDialog->selectedColor().rgb();
            QPixmap pix(16,16);
            pix.fill(QColor(accept_.display_bg_color));
            button->setIcon(QIcon(pix));
            _mainWin->updateAllViewBackgroundColors(accept_.display_bg_color);
        }
        else {
            _mainWin->updateAllViewBackgroundColors(dialog.display_bg_color);
        }
    }
}

void Settings_Dialog::currentDisplayBackgroundColorChanged(const QColor& color)
{
    preview.display_bg_color = color.rgb();
    _mainWin->updateAllViewBackgroundColors(preview.display_bg_color);
}

void Settings_Dialog::chooseDisplaySelectBoxLeftColor()
{
    QPushButton* button = qobject_cast<QPushButton*>(sender());
    if (button) {
        QColorDialog* colorDialog = new QColorDialog(QColor(accept_.display_selectbox_left_color), this);
        connect(colorDialog, SIGNAL(currentColorChanged(const QColor&)), this, SLOT(currentDisplaySelectBoxLeftColorChanged(const QColor&)));
        colorDialog->exec();

        if (colorDialog->result() == QDialog::Accepted) {
            accept_.display_selectbox_left_color = colorDialog->selectedColor().rgb();
            QPixmap pix(16,16);
            pix.fill(QColor(accept_.display_selectbox_left_color));
            button->setIcon(QIcon(pix));
            _mainWin->updateAllViewSelectBoxColors(
                accept_.display_selectbox_left_color,
                accept_.display_selectbox_left_fill,
                accept_.display_selectbox_right_color,
                accept_.display_selectbox_right_fill,
                preview.display_selectbox_alpha);
        }
        else {
            _mainWin->updateAllViewSelectBoxColors(
                dialog.display_selectbox_left_color,
                dialog.display_selectbox_left_fill,
                dialog.display_selectbox_right_color,
                dialog.display_selectbox_right_fill,
                preview.display_selectbox_alpha);
        }
    }
}

void Settings_Dialog::currentDisplaySelectBoxLeftColorChanged(const QColor& color)
{
    preview.display_selectbox_left_color = color.rgb();
    _mainWin->updateAllViewSelectBoxColors(
        preview.display_selectbox_left_color,
        preview.display_selectbox_left_fill,
        preview.display_selectbox_right_color,
        preview.display_selectbox_right_fill,
        preview.display_selectbox_alpha);
}

void Settings_Dialog::chooseDisplaySelectBoxLeftFill()
{
    QPushButton* button = qobject_cast<QPushButton*>(sender());
    if (button) {
        QColorDialog* colorDialog = new QColorDialog(QColor(accept_.display_selectbox_left_fill), this);
        connect(colorDialog, SIGNAL(currentColorChanged(const QColor&)), this, SLOT(currentDisplaySelectBoxLeftFillChanged(const QColor&)));
        colorDialog->exec();

        if (colorDialog->result() == QDialog::Accepted) {
            accept_.display_selectbox_left_fill = colorDialog->selectedColor().rgb();
            QPixmap pix(16,16);
            pix.fill(QColor(accept_.display_selectbox_left_fill));
            button->setIcon(QIcon(pix));
            _mainWin->updateAllViewSelectBoxColors(
                accept_.display_selectbox_left_color,
                accept_.display_selectbox_left_fill,
                accept_.display_selectbox_right_color,
                accept_.display_selectbox_right_fill,
                preview.display_selectbox_alpha);
        }
        else {
            _mainWin->updateAllViewSelectBoxColors(
                dialog.display_selectbox_left_color,
                dialog.display_selectbox_left_fill,
                dialog.display_selectbox_right_color,
                dialog.display_selectbox_right_fill,
                preview.display_selectbox_alpha);
        }
    }
}

void Settings_Dialog::currentDisplaySelectBoxLeftFillChanged(const QColor& color)
{
    preview.display_selectbox_left_fill = color.rgb();
    _mainWin->updateAllViewSelectBoxColors(
        preview.display_selectbox_left_color,
        preview.display_selectbox_left_fill,
        preview.display_selectbox_right_color,
        preview.display_selectbox_right_fill,
        preview.display_selectbox_alpha);
}

void Settings_Dialog::chooseDisplaySelectBoxRightColor()
{
    QPushButton* button = qobject_cast<QPushButton*>(sender());
    if (button)
    {
        QColorDialog* colorDialog = new QColorDialog(QColor(accept_.display_selectbox_right_color), this);
        connect(colorDialog, SIGNAL(currentColorChanged(const QColor&)), this, SLOT(currentDisplaySelectBoxRightColorChanged(const QColor&)));
        colorDialog->exec();

        if (colorDialog->result() == QDialog::Accepted) {
            accept_.display_selectbox_right_color = colorDialog->selectedColor().rgb();
            QPixmap pix(16,16);
            pix.fill(QColor(accept_.display_selectbox_right_color));
            button->setIcon(QIcon(pix));
            _mainWin->updateAllViewSelectBoxColors(
                accept_.display_selectbox_left_color,
                accept_.display_selectbox_left_fill,
                accept_.display_selectbox_right_color,
                accept_.display_selectbox_right_fill,
                preview.display_selectbox_alpha);
        }
        else {
            _mainWin->updateAllViewSelectBoxColors(
                dialog.display_selectbox_left_color,
                dialog.display_selectbox_left_fill,
                dialog.display_selectbox_right_color,
                dialog.display_selectbox_right_fill,
                preview.display_selectbox_alpha);
        }
    }
}

void Settings_Dialog::currentDisplaySelectBoxRightColorChanged(const QColor& color)
{
    preview.display_selectbox_right_color = color.rgb();
    _mainWin->updateAllViewSelectBoxColors(
        preview.display_selectbox_left_color,
        preview.display_selectbox_left_fill,
        preview.display_selectbox_right_color,
        preview.display_selectbox_right_fill,
        preview.display_selectbox_alpha);
}

void Settings_Dialog::chooseDisplaySelectBoxRightFill()
{
    QPushButton* button = qobject_cast<QPushButton*>(sender());
    if (button) {
        QColorDialog* colorDialog = new QColorDialog(QColor(accept_.display_selectbox_right_fill), this);
        connect(colorDialog, SIGNAL(currentColorChanged(const QColor&)), this, SLOT(currentDisplaySelectBoxRightFillChanged(const QColor&)));
        colorDialog->exec();

        if (colorDialog->result() == QDialog::Accepted) {
            accept_.display_selectbox_right_fill = colorDialog->selectedColor().rgb();
            QPixmap pix(16,16);
            pix.fill(QColor(accept_.display_selectbox_right_fill));
            button->setIcon(QIcon(pix));
            _mainWin->updateAllViewSelectBoxColors(
                accept_.display_selectbox_left_color,
                accept_.display_selectbox_left_fill,
                accept_.display_selectbox_right_color,
                accept_.display_selectbox_right_fill,
                preview.display_selectbox_alpha);
        }
        else {
            _mainWin->updateAllViewSelectBoxColors(
                dialog.display_selectbox_left_color,
                dialog.display_selectbox_left_fill,
                dialog.display_selectbox_right_color,
                dialog.display_selectbox_right_fill,
                preview.display_selectbox_alpha);
        }
    }
}

void Settings_Dialog::currentDisplaySelectBoxRightFillChanged(const QColor& color)
{
    preview.display_selectbox_right_fill = color.rgb();
    _mainWin->updateAllViewSelectBoxColors(
        preview.display_selectbox_left_color,
        preview.display_selectbox_left_fill,
        preview.display_selectbox_right_color,
        preview.display_selectbox_right_fill,
        preview.display_selectbox_alpha);
}

void Settings_Dialog::spinBoxDisplaySelectBoxAlphaValueChanged(int value)
{
    preview.display_selectbox_alpha = value;
    _mainWin->updateAllViewSelectBoxColors(
        accept_.display_selectbox_left_color,
        accept_.display_selectbox_left_fill,
        accept_.display_selectbox_right_color,
        accept_.display_selectbox_right_fill,
        preview.display_selectbox_alpha);
}

void Settings_Dialog::choosePromptTextColor()
{
    QPushButton* button = qobject_cast<QPushButton*>(sender());
    if (button) {
        QColorDialog* colorDialog = new QColorDialog(QColor(accept_.prompt_text_color), this);
        connect(colorDialog, SIGNAL(currentColorChanged(const QColor&)), this, SLOT(currentPromptTextColorChanged(const QColor&)));
        colorDialog->exec();

        if (colorDialog->result() == QDialog::Accepted) {
            accept_.prompt_text_color = colorDialog->selectedColor().rgb();
            QPixmap pix(16,16);
            pix.fill(QColor(accept_.prompt_text_color));
            button->setIcon(QIcon(pix));
            prompt->setPromptTextColor(QColor(accept_.prompt_text_color));
        }
        else {
            prompt->setPromptTextColor(QColor(dialog.prompt_text_color));
        }
    }
}

void Settings_Dialog::currentPromptTextColorChanged(const QColor& color)
{
    preview.prompt_text_color = color.rgb();
    prompt->setPromptTextColor(QColor(preview.prompt_text_color));
}

void Settings_Dialog::choosePromptBackgroundColor()
{
    QPushButton* button = qobject_cast<QPushButton*>(sender());
    if (button) {
        QColorDialog* colorDialog = new QColorDialog(QColor(accept_.prompt_bg_color), this);
        connect(colorDialog, SIGNAL(currentColorChanged(QColor)), this, SLOT(currentPromptBackgroundColorChanged(QColor)));
        colorDialog->exec();

        if (colorDialog->result() == QDialog::Accepted) {
            accept_.prompt_bg_color = colorDialog->selectedColor().rgb();
            QPixmap pix(16,16);
            pix.fill(QColor(accept_.prompt_bg_color));
            button->setIcon(QIcon(pix));
            prompt->setPromptBackgroundColor(QColor(accept_.prompt_bg_color));
        }
        else {
            prompt->setPromptBackgroundColor(QColor(dialog.prompt_bg_color));
        }
    }
}

void Settings_Dialog::currentPromptBackgroundColorChanged(const QColor& color)
{
    preview.prompt_bg_color = color.rgb();
    prompt->setPromptBackgroundColor(QColor(preview.prompt_bg_color));
}

void Settings_Dialog::comboBoxPromptFontFamilyCurrentIndexChanged(QString  family)
{
    preview.prompt_font_family = family.toStdString();
    prompt->setPromptFontFamily(QString::fromStdString(preview.prompt_font_family));
}

void Settings_Dialog::comboBoxPromptFontStyleCurrentIndexChanged(QString  style)
{
    preview.prompt_font_style = style.toStdString();
    prompt->setPromptFontStyle(QString::fromStdString(preview.prompt_font_style));
}

void Settings_Dialog::spinBoxPromptFontSizeValueChanged(int value)
{
    preview.prompt_font_size = value;
    prompt->setPromptFontSize(preview.prompt_font_size);
}

void Settings_Dialog::checkBoxPromptSaveHistoryAsHtmlStateChanged(int checked)
{
    dialog.prompt_save_history_as_html = checked;
}

void Settings_Dialog::checkBoxCustomFilterStateChanged(int checked)
{
    QCheckBox* checkBox = qobject_cast<QCheckBox*>(sender());
    if (checkBox) {
        QString format = checkBox->text();
        qDebug("CustomFilter: %s %d", qPrintable(format), checked);
        if (checked)
            dialog.opensave_custom_filter.append(" *." + format.toLower());
        else
            dialog.opensave_custom_filter.remove("*." + format, Qt::CaseInsensitive);
        //dialog.opensave_custom_filter = checked; //TODO
    }
}

void Settings_Dialog::buttonCustomFilterSelectAllClicked()
{
    emit buttonCustomFilterSelectAll(true);
    dialog.opensave_custom_filter = "supported";
}

void Settings_Dialog::buttonCustomFilterClearAllClicked()
{
    emit buttonCustomFilterClearAll(false);
    dialog.opensave_custom_filter.clear();
}

void Settings_Dialog::checkBoxGridColorMatchCrossHairStateChanged(int checked)
{
    dialog.grid_color_match_crosshair = checked;
    if (dialog.grid_color_match_crosshair) {
        _mainWin->updateAllViewGridColors(accept_.display_crosshair_color);
    }
    else {
        _mainWin->updateAllViewGridColors(accept_.grid_color);
    }

    QObject* senderObj = sender();
    if (senderObj) {
        QObject* parent = senderObj->parent();
        if (parent) {
            set_enabled(parent, "labelGridColor", !dialog.grid_color_match_crosshair);
            set_enabled(parent, "buttonGridColor", !dialog.grid_color_match_crosshair);
        }
    }
}

void Settings_Dialog::chooseGridColor()
{
    QPushButton* button = qobject_cast<QPushButton*>(sender());
    if (button)
    {
        QColorDialog* colorDialog = new QColorDialog(QColor(accept_.grid_color), this);
        connect(colorDialog, SIGNAL(currentColorChanged(const QColor&)), this, SLOT(currentGridColorChanged(const QColor&)));
        colorDialog->exec();

        if (colorDialog->result() == QDialog::Accepted)
        {
            accept_.grid_color = colorDialog->selectedColor().rgb();
            QPixmap pix(16,16);
            pix.fill(QColor(accept_.grid_color));
            button->setIcon(QIcon(pix));
            _mainWin->updateAllViewGridColors(accept_.grid_color);
        }
        else
        {
            _mainWin->updateAllViewGridColors(dialog.grid_color);
        }
    }
}

void Settings_Dialog::currentGridColorChanged(const QColor& color)
{
    preview.grid_color = color.rgb();
    _mainWin->updateAllViewGridColors(preview.grid_color);
}

void Settings_Dialog::checkBoxGridLoadFromFileStateChanged(int checked)
{
    dialog.grid_load_from_file = checked;

    QObject* senderObj = sender();
    if (!senderObj) {
        return;
    }

    QObject* parent = senderObj->parent();
    if (parent) {
        return;
    }

    set_enabled(parent, "labelGridType", !dialog.grid_load_from_file);
    set_enabled(parent, "comboBoxGridType", !dialog.grid_load_from_file);
    set_enabled(parent, "checkBoxGridCenterOnOrigin", !dialog.grid_load_from_file);

    set_enabled(parent, "labelGridCenterX",
        !dialog.grid_load_from_file && !dialog.grid_center_on_origin);
    set_enabled(parent, "spinBoxGridCenterX",
        !dialog.grid_load_from_file && !dialog.grid_center_on_origin);

    set_enabled(parent, "labelGridCenterY",
        !dialog.grid_load_from_file && !dialog.grid_center_on_origin);
    set_enabled(parent, "spinBoxGridCenterY",
        !dialog.grid_load_from_file && !dialog.grid_center_on_origin);

    set_enabled(parent, "labelGridSizeX", !dialog.grid_load_from_file);
    set_enabled(parent, "spinBoxGridSizeX", !dialog.grid_load_from_file);

    set_enabled(parent, "labelGridSizeY", !dialog.grid_load_from_file);
    set_enabled(parent, "spinBoxGridSizeY", !dialog.grid_load_from_file);

    set_enabled(parent, "labelGridSpacingX", !dialog.grid_load_from_file);
    set_enabled(parent, "spinBoxGridSpacingX", !dialog.grid_load_from_file);

    set_enabled(parent, "labelGridSpacingY", !dialog.grid_load_from_file);
    set_enabled(parent, "spinBoxGridSpacingY", !dialog.grid_load_from_file);

    set_enabled(parent, "labelGridSizeRadius", !dialog.grid_load_from_file);
    set_enabled(parent, "spinBoxGridSizeRadius", !dialog.grid_load_from_file);

    set_enabled(parent, "labelGridSpacingRadius", !dialog.grid_load_from_file);
    set_enabled(parent, "spinBoxGridSpacingRadius", !dialog.grid_load_from_file);

    set_enabled(parent, "labelGridSpacingAngle", !dialog.grid_load_from_file);
    set_enabled(parent, "spinBoxGridSpacingAngle", !dialog.grid_load_from_file);
}

/**
 * @brief Settings_Dialog::comboBoxGridTypeCurrentIndexChanged
 * @param type
 */
void Settings_Dialog::comboBoxGridTypeCurrentIndexChanged(QString  type)
{
    dialog_["grid_type"].s = type.toStdString();

    QObject* senderObj = sender();
    if (!senderObj) {
        return;
    }
    QObject* parent = senderObj->parent();
    if (!parent) {
        return;
    }
    bool visibility = (type == "Circular");

    set_visibility(parent, "labelGridSizeX", !visibility);
    set_visibility(parent, "spinBoxGridSizeX", !visibility);

    set_visibility(parent, "labelGridSizeY", !visibility);
    set_visibility(parent, "spinBoxGridSizeY", !visibility);

    set_visibility(parent, "labelGridSpacingX", !visibility);
    set_visibility(parent, "spinBoxGridSpacingX", !visibility);

    set_visibility(parent, "labelGridSpacingY", !visibility);
    set_visibility(parent, "spinBoxGridSpacingY", !visibility);

    set_visibility(parent, "labelGridSizeRadius", visibility);
    set_visibility(parent, "spinBoxGridSizeRadius", visibility);

    set_visibility(parent, "labelGridSpacingRadius", visibility);
    set_visibility(parent, "spinBoxGridSpacingRadius", visibility);

    set_visibility(parent, "labelGridSpacingAngle", visibility);
    set_visibility(parent, "spinBoxGridSpacingAngle", visibility);
}

void Settings_Dialog::checkBoxGridCenterOnOriginStateChanged(int checked)
{
    dialog.grid_center_on_origin = checked;

    QObject* senderObj = sender();
    if (senderObj) {
        return;
    }

    QObject* parent = senderObj->parent();
    if (parent) {
        return;
    }

    set_enabled(parent, "labelGridCenterX", !dialog.grid_center_on_origin);
    set_enabled(parent, "spinBoxGridCenterX", !dialog.grid_center_on_origin);

    set_enabled(parent, "labelGridCenterY", !dialog.grid_center_on_origin);
    set_enabled(parent, "spinBoxGridCenterY", !dialog.grid_center_on_origin);
}

void Settings_Dialog::checkBoxRulerShowOnLoadStateChanged(int checked)
{
    dialog.ruler_show_on_load = checked;
}

void Settings_Dialog::comboBoxRulerMetricCurrentIndexChanged(int index)
{
    QComboBox* comboBox = qobject_cast<QComboBox*>(sender());
    if (comboBox) {
        bool ok = 0;
        dialog.ruler_metric = comboBox->itemData(index).toBool();
    }
    else {
        dialog.ruler_metric = true;
    }
}

void Settings_Dialog::chooseRulerColor()
{
    QPushButton* button = qobject_cast<QPushButton*>(sender());
    if (button) {
        QColorDialog* colorDialog = new QColorDialog(QColor(accept_.ruler_color), this);
        connect(colorDialog, SIGNAL(currentColorChanged(QColor)), this, SLOT(currentRulerColorChanged(QColor)));
        colorDialog->exec();

        if (colorDialog->result() == QDialog::Accepted) {
            accept_.ruler_color = colorDialog->selectedColor().rgb();
            QPixmap pix(16,16);
            pix.fill(QColor(accept_.ruler_color));
            button->setIcon(QIcon(pix));
            _mainWin->updateAllViewRulerColors(accept_.ruler_color);
        }
        else {
            _mainWin->updateAllViewRulerColors(dialog.ruler_color);
        }
    }
}

void Settings_Dialog::currentRulerColorChanged(const QColor& color)
{
    preview.ruler_color = color.rgb();
    _mainWin->updateAllViewRulerColors(preview.ruler_color);
}

void Settings_Dialog::spinBoxRulerPixelSizeValueChanged(double value)
{
    dialog.ruler_pixel_size = value;
}

void Settings_Dialog::buttonQSnapSelectAllClicked()
{
    emit buttonQSnapSelectAll(true);
}

void Settings_Dialog::buttonQSnapClearAllClicked()
{
    emit buttonQSnapClearAll(false);
}

/**
 *
 */
void Settings_Dialog::comboBoxQSnapLocatorColorCurrentIndexChanged(int index)
{
    //TODO: Alert user if color matched the display bg color
    QComboBox* comboBox = qobject_cast<QComboBox*>(sender());
    QRgb defaultColor = qRgb(255,255,0); //Yellow
    if (comboBox) {
        bool ok = 0;
        dialog.qsnap_locator_color = comboBox->itemData(index).toUInt(&ok);
        if (!ok)
            dialog.qsnap_locator_color = defaultColor;
    }
    else {
        dialog.qsnap_locator_color = defaultColor;
    }
}

void Settings_Dialog::checkBoxLwtShowLwtStateChanged(int checked)
{
    preview.lwt_show_lwt = checked;
    statusbar->toggle("LWT", preview.lwt_show_lwt);

    QObject* senderObj = sender();
    if (senderObj) {
        QObject* parent = senderObj->parent();
        if (parent) {
            QCheckBox* checkBoxRealRender = parent->findChild<QCheckBox*>("checkBoxRealRender");
            if (checkBoxRealRender) checkBoxRealRender->setEnabled(preview.lwt_show_lwt);
        }
    }
}

void Settings_Dialog::checkBoxLwtRealRenderStateChanged(int checked)
{
    preview.lwt_real_render = checked;
    statusbar->toggle("LWT", preview.lwt_real_render);
}

void Settings_Dialog::comboBoxSelectionCoolGripColorCurrentIndexChanged(int index)
{
    //TODO: Alert user if color matched the display bg color
    QComboBox* comboBox = qobject_cast<QComboBox*>(sender());
    QRgb defaultColor = qRgb(0,0,255); //Blue
    if (comboBox) {
        bool ok = 0;
        dialog.selection_coolgrip_color = comboBox->itemData(index).toUInt(&ok);
        if (!ok) {
            dialog.selection_coolgrip_color = defaultColor;
        }
    }
    else {
        dialog.selection_coolgrip_color = defaultColor;
    }
}

void Settings_Dialog::comboBoxSelectionHotGripColorCurrentIndexChanged(int index)
{
    //TODO: Alert user if color matched the display bg color
    QComboBox* comboBox = qobject_cast<QComboBox*>(sender());
    QRgb defaultColor = qRgb(255,0,0); //Red
    if (comboBox) {
        bool ok = 0;
        dialog.selection_hotgrip_color = comboBox->itemData(index).toUInt(&ok);
        if (!ok) {
            dialog.selection_hotgrip_color = defaultColor;
        }
    }
    else {
        dialog.selection_hotgrip_color = defaultColor;
    }
}

/**
 * .
 */
void Settings_Dialog::acceptChanges()
{
    dialog.general_mdi_bg_use_logo = preview.general_mdi_bg_use_logo;
    dialog.general_mdi_bg_use_texture = preview.general_mdi_bg_use_texture;
    dialog.general_mdi_bg_use_color = preview.general_mdi_bg_use_color;
    dialog_["general_mdi_bg_logo"] = node(get_str(accept__, "general_mdi_bg_logo"));
    dialog.general_mdi_bg_texture = accept_.general_mdi_bg_texture;
    dialog.general_mdi_bg_color = accept_.general_mdi_bg_color;
    dialog.display_show_scrollbars = preview.display_show_scrollbars;
    dialog.display_crosshair_color = accept_.display_crosshair_color;
    dialog.display_bg_color = accept_.display_bg_color;
    dialog.display_selectbox_left_color = accept_.display_selectbox_left_color;
    dialog.display_selectbox_left_fill = accept_.display_selectbox_left_fill;
    dialog.display_selectbox_right_color = accept_.display_selectbox_right_color;
    dialog.display_selectbox_right_fill = accept_.display_selectbox_right_fill;
    dialog.display_selectbox_alpha = preview.display_selectbox_alpha;
    dialog.prompt_text_color = accept_.prompt_text_color;
    dialog.prompt_bg_color = accept_.prompt_bg_color;
    dialog.prompt_font_family = preview.prompt_font_family;
    dialog.prompt_font_style = preview.prompt_font_style;
    dialog.prompt_font_size = preview.prompt_font_size;
    if (dialog.grid_color_match_crosshair) {
        dialog.grid_color = accept_.display_crosshair_color;
    }
    else {
        dialog.grid_color = accept_.grid_color;
    }
    dialog.ruler_color = accept_.ruler_color;
    dialog.lwt_show_lwt = preview.lwt_show_lwt;
    dialog.lwt_real_render = preview.lwt_real_render;

    settings = dialog;

    //Make sure the user sees the changes applied immediately
    mdiArea->useBackgroundLogo(dialog.general_mdi_bg_use_logo);
    mdiArea->useBackgroundTexture(dialog.general_mdi_bg_use_texture);
    mdiArea->useBackgroundColor(dialog.general_mdi_bg_use_color);
    mdiArea->setBackgroundLogo(QString::fromStdString(get_str(dialog_, "general_mdi_bg_logo")));
    mdiArea->setBackgroundTexture(QString::fromStdString(dialog.general_mdi_bg_texture));
    mdiArea->setBackgroundColor(dialog.general_mdi_bg_color);
    _mainWin->iconResize(dialog.general_icon_size);
    _mainWin->updateAllViewScrollBars(dialog.display_show_scrollbars);
    _mainWin->updateAllViewCrossHairColors(dialog.display_crosshair_color);
    _mainWin->updateAllViewBackgroundColors(dialog.display_bg_color);
    _mainWin->updateAllViewSelectBoxColors(
        dialog.display_selectbox_left_color,
        dialog.display_selectbox_left_fill,
        dialog.display_selectbox_right_color,
        dialog.display_selectbox_right_fill,
        dialog.display_selectbox_alpha);
    prompt->setPromptTextColor(QColor(dialog.prompt_text_color));
    prompt->setPromptBackgroundColor(QColor(dialog.prompt_bg_color));
    prompt->setPromptFontFamily(QString::fromStdString(dialog.prompt_font_family));
    prompt->setPromptFontStyle(QString::fromStdString(dialog.prompt_font_style));
    prompt->setPromptFontSize(dialog.prompt_font_size);
    _mainWin->updateAllViewGridColors(dialog.grid_color);
    _mainWin->updateAllViewRulerColors(dialog.ruler_color);
    statusbar->toggle("LWT", dialog.lwt_show_lwt);
    /*
    statusbar->toggle("REAL", dialog.lwt_real_render); ?
    */
    _mainWin->updatePickAddMode(dialog.selection_mode_pickadd);

    write_settings();
    accept();
}

/**
 * @brief Settings_Dialog::rejectChanges
 */
void
Settings_Dialog::rejectChanges()
{
    //TODO: inform the user if they have changed settings

    //Update the view since the user must accept the preview
    mdiArea->useBackgroundLogo(dialog.general_mdi_bg_use_logo);
    mdiArea->useBackgroundTexture(dialog.general_mdi_bg_use_texture);
    mdiArea->useBackgroundColor(dialog.general_mdi_bg_use_color);
    mdiArea->setBackgroundLogo(QString::fromStdString(get_str(dialog_, "general_mdi_bg_logo")));
    mdiArea->setBackgroundTexture(QString::fromStdString(dialog.general_mdi_bg_texture));
    mdiArea->setBackgroundColor(dialog.general_mdi_bg_color);
    _mainWin->updateAllViewScrollBars(dialog.display_show_scrollbars);
    _mainWin->updateAllViewCrossHairColors(dialog.display_crosshair_color);
    _mainWin->updateAllViewBackgroundColors(dialog.display_bg_color);
    _mainWin->updateAllViewSelectBoxColors(
        dialog.display_selectbox_left_color,
        dialog.display_selectbox_left_fill,
        dialog.display_selectbox_right_color,
        dialog.display_selectbox_right_fill,
        dialog.display_selectbox_alpha);
    prompt->setPromptTextColor(QColor(dialog.prompt_text_color));
    prompt->setPromptBackgroundColor(QColor(dialog.prompt_bg_color));
    prompt->setPromptFontFamily(QString::fromStdString(dialog.prompt_font_family));
    prompt->setPromptFontStyle(QString::fromStdString(dialog.prompt_font_style));
    prompt->setPromptFontSize(dialog.prompt_font_size);
    _mainWin->updateAllViewGridColors(dialog.grid_color);
    _mainWin->updateAllViewRulerColors(dialog.ruler_color);
    statusbar->toggle("LWT", settings.lwt_show_lwt);
    /*
    statusbar->toggle("REAL", settings.lwt_real_render); ?
    */

    reject();
}

