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

#include "embroidermodder.h"
#include <iostream>

#if 0

#if defined(Q_OS_UNIX) || defined(Q_OS_MAC)
std::string settingsDir = QDir::homePath() + "/.embroidermodder2/";
#else
std::string settingsDir = "";
#endif
std::string defaultsPath = "./defaults.toml";
std::string settingsPath = settingsDir + "settings.toml";

std::vector<std::string> ui_script;
std::unordered_map<std::string, Setting> settings;
std::unordered_map<std::string, std::string> settings_str;
std::unordered_map<std::string, QAction *> action_list_;

std::string load_string(std::string s)
{
    std::unordered_map<std::string, std::string>::const_iterator result = settings_str.find(s);
    if (settings_str.end() == result) {
        std::cout << "Failed to load: " << s << std::endl;
        return "";
    }
    return settings_str[s];
}

double load_double(std::string s)
{
    std::unordered_map<std::string, Setting>::const_iterator result = settings.find(s);
    if (settings.end() == result) {
        std::cout << "Failed to load: " << s << std::endl;
        return 0.0;
    }
    return settings[s].real_value;
}

int load_int(std::string s)
{
    std::unordered_map<std::string, Setting>::const_iterator result = settings.find(s);
    if (settings.end() == result) {
        std::cout << "Failed to load: " << s << std::endl;
        return 0;
    }
    return settings[s].int_value;
}

bool load_bool(std::string s)
{
    std::unordered_map<std::string, Setting>::const_iterator result = settings.find(s);
    if (settings.end() == result) {
        std::cout << "Failed to load: " << s << std::endl;
        return false;
    }
    return settings[s].bool_value;
}

void store_string(std::string key, std::string value)
{
    std::unordered_map<std::string, Setting>::const_iterator result = settings.find(key);
    if (settings.end() != result) {
        settings[key].string_value = value;
    }
    else {
        std::cout << "Failed to store: " << key << std::endl;
    }
}

void store_double(std::string key, double value)
{
    std::unordered_map<std::string, Setting>::const_iterator result = settings.find(key);
    if (settings.end() != result) {
        settings[key].real_value = value;
    }
    else {
        std::cout << "Failed to store: " << key << std::endl;
    }
}

void store_int(std::string key, int value)
{
    std::unordered_map<std::string, Setting>::const_iterator result = settings.find(key);
    if (settings.end() != result) {
        settings[key].int_value = value;
    }
    else {
        std::cout << "Failed to store: " << key << std::endl;
    }
}

void store_bool(std::string key, bool value)
{
    std::unordered_map<std::string, Setting>::const_iterator result = settings.find(key);
    if (settings.end() != result) {
        settings[key].bool_value = value;
    }
    else {
        std::cout << "Failed to store: " << key << std::endl;
    }
}

std::string str_from_toml(toml::value value)
{
    return std::string::fromLocal8Bit(toml::get<std::string>(value));
}

void load_configuration(void)
{
    auto config = toml::parse("config.toml");

    for (auto &[i, j] : config.as_table()) {
        if (!config[i].is_table()) {
            continue;
        }
        if (!config[i].contains("type")) {
            continue;
        }
        std::string s = toml::get<std::string>(config[i]["type"]);
        if (s == "top-menu") {
            // If the menu doesn't exist, create it.
            std::string s_label = toml::get<std::string>(config[i]["label"]);
            std::string label = std::string::fromLocal8Bit(s_label);
            if (!menuHash.value(label)) {
                QMenu* menu = new QMenu(label, this);
                menu->setTearOffEnabled(true);
                menuBar()->addMenu(menu);
                menuHash.insert(label, menu);
            }
        }
    }
    for (auto &[i, j] : config.as_table()) {
        if (!config[i].is_table()) {
            continue;
        }
        if (!config[i].contains("type")) {
            continue;
        }
        std::string s = toml::get<std::string>(config[i]["type"]);
        if (s == "menu-item") {
            std::string menuName = str_from_toml(config[i]["menu"]);
            std::string cmdName = str_from_toml(config[i]["command"]);
            std::string icon = str_from_toml(config[i]["icon"]);
            std::string toolTip = str_from_toml(config[i]["tooltip"]);
            std::string statusTip = str_from_toml(config[i]["statustip"]);
            QAction *ACTION = new QAction(load_icon(icon), toolTip, this);
            ACTION->setStatusTip(statusTip);
            ACTION->setObjectName(cmdName);
            ACTION->setWhatsThis(statusTip);
            connect(ACTION, &QAction::triggered, this, [=](){
                actuator(ACTION->objectName().toLower().toStdString());
            });
            menuHash.value(menuName)->addAction(ACTION);
        }
    }

    /*

    if (toolbarName.toUpper() != "NONE") {
        //If the toolbar doesn't exist, create it.
        if (!toolbarHash.value(toolbarName)) {
            QToolBar* tb = new QToolBar(toolbarName, this);
            tb->setObjectName("toolbar" + toolbarName);
            connect(tb, SIGNAL(topLevelChanged(bool)), this, SLOT(floatingChangedToolBar(bool)));
            addToolBar(Qt::LeftToolBarArea, tb);
            addToolBarBreak(Qt::LeftToolBarArea);
            toolbarHash.insert(toolbarName, tb);
        }

        //TODO: order actions position in toolbar based on .ini setting
        toolbarHash.value(toolbarName)->addAction(ACTION);
    }
    */
}

void readSettings()
{
    load_configuration();

    debug_message("Reading Settings...");
    // This file needs to be read from the users home directory to ensure it is writable
    EmbVector pos(0,0);
    QSize size(800,600);

    /* some layout state recording? */
    settings_general_language = "default";
    settings_general_icon_theme = "default";
    settings_general_icon_size = 16;
    settings_general_mdi_bg_use_logo = true;
    settings_general_mdi_bg_use_texture = true;
    settings_general_mdi_bg_use_color = true;
    settings_general_mdi_bg_logo = "./images/logo-spirals.png";
    settings_general_mdi_bg_texture = "./images/texture-spirals.png";
    settings_general_mdi_bg_color = qRgb(192,192,192);
    settings_general_tip_of_the_day = true;
    settings_general_current_tip = 0;
    settings_general_system_help_browser = true;
    //Display
    settings_display_use_opengl = false;
    settings_display_renderhint_aa = false;
    settings_display_renderhint_text_aa = false;
    settings_display_renderhint_smooth_pix = false;
    settings_display_renderhint_high_aa = false;
    settings_display_renderhint_noncosmetic = false;
    settings_display_show_scrollbars = true;
    settings_display_scrollbar_widget_num = 0;
    settings_display_crosshair_color = qRgb(  0, 0, 0) ;
    settings_display_bg_color = qRgb(235,235,235) ;
    settings_display_selectbox_left_color = qRgb(  0,128, 0) ;
    settings_display_selectbox_left_fill = qRgb(  0,255, 0) ;
    settings_display_selectbox_right_color = qRgb(  0, 0,128) ;
    settings_display_selectbox_right_fill = qRgb(  0, 0,255) ;
    settings_display_selectbox_alpha = 32;
    settings_display_zoomscale_in = 2.0;
    settings_display_zoomscale_out = 0.5;
    settings_display_crosshair_percent = 5;
    settings_display_units = "mm";
    //Prompt
    settings_prompt_text_color = qRgb(  0, 0, 0) ;
    settings_prompt_bg_color = qRgb(255,255,255) ;
    settings_prompt_font_family = "Monospace";
    settings_prompt_font_style = "normal";
    settings_prompt_font_size = 12;
    settings_prompt_save_history = true;
    settings_prompt_save_history_as_html = false;
    settings_prompt_save_history_filename = settingsDir + "prompt.log";
    //OpenSave
    settings_opensave_custom_filter = "supported";
    settings_opensave_open_format = "*.*";
    settings_opensave_open_thumbnail = false;
    settings_opensave_save_format = "*.*";
    settings_opensave_save_thumbnail = false;
    //Recent
    settings_opensave_recent_max_files = 10;
    settings_opensave_recent_list_of_files = {};
    settings_opensave_recent_directory = "./samples";
    //Trimming
    settings_opensave_trim_dst_num_jumps = 5;
    //Printing
    settings_printing_default_device = "";
    settings_printing_use_last_device = false;
    settings_printing_disable_bg = true;
    //Grid
    settings_grid_show_on_load = true;
    settings_grid_show_origin = true;
    settings_grid_color_match_crosshair = true;
    settings_grid_color = qRgb(  0, 0, 0);
    settings_grid_load_from_file = true;
    settings_grid_type = "Rectangular";
    settings_grid_center_on_origin = true;
    settings_grid_center_x = 0.0;
    settings_grid_center_y = 0.0;
    settings_grid_size_x = 100.0;
    settings_grid_size_y = 100.0;
    settings_grid_spacing_x = 25.0;
    settings_grid_spacing_y = 25.0;
    settings_grid_size_radius = 50.0;
    settings_grid_spacing_radius = 25.0;
    settings_grid_spacing_angle = 45.0;
    //Ruler
    settings_ruler_show_on_load = true;
    settings_ruler_metric = true;
    settings_ruler_color = qRgb(210,210, 50);
    settings_ruler_pixel_size = 20;
    //Quick Snap
    settings_qsnap_enabled = true;
    settings_qsnap_locator_color = qRgb(255,255, 0);
    settings_qsnap_locator_size = 4;
    settings_qsnap_aperture_size = 10;
    settings_qsnap_endpoint = true;
    settings_qsnap_midpoint = true;
    settings_qsnap_center = true;
    settings_qsnap_node = true;
    settings_qsnap_quadrant = true;
    settings_qsnap_intersection = true;
    settings_qsnap_extension = true;
    settings_qsnap_insertion = false;
    settings_qsnap_perpendicular = true;
    settings_qsnap_tangent = true;
    settings_qsnap_nearest = false;
    settings_qsnap_apparent = false;
    settings_qsnap_parallel = false;
    //LineWeight
    settings_lwt_show_lwt = false;
    settings_lwt_real_render = true;
    settings_lwt_default_lwt = 0;
    //Selection
    settings_selection_mode_pickfirst = true;
    settings_selection_mode_pickadd = true;
    settings_selection_mode_pickdrag = false;
    settings_selection_coolgrip_color = qRgb(  0, 0,255);
    settings_selection_hotgrip_color = qRgb(255, 0, 0);
    settings_selection_grip_size = 4;
    settings_selection_pickbox_size = 4;
    //Text
    settings_text_font  = "Arial";
    settings_text_size = 12;
    settings_text_angle = 0;
    settings_text_style_bold = false;
    settings_text_style_italic = false;
    settings_text_style_underline = false;
    settings_text_style_strikeout = false;
    settings_text_style_overline = false;

    auto setting_toml = toml::parse(defaultsPath);
    if (setting_toml.contains("window")) {
        toml::value table = toml::find<toml::value>(setting_toml, "window");
        if (table.contains("position-x")) {
            pos.setX(toml::find<int>(table, "position_x"));
        }
        if (table.contains("position-y")) {
            pos.setY(toml::find<int>(table, "position_y"));
        }
        if (table.contains("size-x")) {
            size.setWidth(toml::find<int>(table, "size_x"));
        }
        if (table.contains("size-y")) {
            size.setHeight(toml::find<int>(table, "size_y"));
        }
    }

    if (setting_toml.contains("general")) {
        toml::value table = toml::find<toml::value>(setting_toml, "general");
        settings_general_language = std::string::fromStdString(toml::find<std::string>(table, "language"));
        settings_general_icon_theme = std::string::fromStdString(toml::find<std::string>(table, "icon_theme"));
    }

    move(pos);
    resize(size);
}

void writeSettings()
{
    debug_message("Writing Settings...");
    // This file needs to be read from the users home directory to ensure it is writable
    QSettings settings(settingsPath, QSettings::IniFormat);
    std::string tmp;
    settings.setValue("Window/Position", pos());
    settings.setValue("Window/Size", size());

    //General
    settings.setValue("LayoutState", layoutState);
    settings.setValue("Language", settings_general_language);
    settings.setValue("IconTheme", settings_general_icon_theme);
    settings.setValue("IconSize", tmp.setNum(settings_general_icon_size));
    settings.setValue("MdiBGUseLogo", settings_general_mdi_bg_use_logo);
    settings.setValue("MdiBGUseTexture", settings_general_mdi_bg_use_texture);
    settings.setValue("MdiBGUseColor", settings_general_mdi_bg_use_color);
    settings.setValue("MdiBGLogo", settings_general_mdi_bg_logo);
    settings.setValue("MdiBGTexture", settings_general_mdi_bg_texture);
    settings.setValue("MdiBGColor", tmp.setNum(settings_general_mdi_bg_color));
    settings.setValue("TipOfTheDay", settings_general_tip_of_the_day);
    settings.setValue("CurrentTip", tmp.setNum(settings_general_current_tip + 1));
    settings.setValue("SystemHelpBrowser", settings_general_system_help_browser);
    //Display
    settings.setValue("Display/UseOpenGL", settings_display_use_opengl);
    settings.setValue("Display/RenderHintAntiAlias", settings_display_renderhint_aa);
    settings.setValue("Display/RenderHintTextAntiAlias", settings_display_renderhint_text_aa);
    settings.setValue("Display/RenderHintSmoothPixmap", settings_display_renderhint_smooth_pix);
    settings.setValue("Display/RenderHintHighQualityAntiAlias", settings_display_renderhint_high_aa);
    settings.setValue("Display/RenderHintNonCosmetic", settings_display_renderhint_noncosmetic);
    settings.setValue("Display/ShowScrollBars", settings_display_show_scrollbars);
    settings.setValue("Display/ScrollBarWidgetNum", tmp.setNum(settings_display_scrollbar_widget_num));
    settings.setValue("Display/CrossHairColor", tmp.setNum(settings_display_crosshair_color));
    settings.setValue("Display/BackgroundColor", tmp.setNum(settings_display_bg_color));
    settings.setValue("Display/SelectBoxLeftColor", tmp.setNum(settings_display_selectbox_left_color));
    settings.setValue("Display/SelectBoxLeftFill", tmp.setNum(settings_display_selectbox_left_fill));
    settings.setValue("Display/SelectBoxRightColor", tmp.setNum(settings_display_selectbox_right_color));
    settings.setValue("Display/SelectBoxRightFill", tmp.setNum(settings_display_selectbox_right_fill));
    settings.setValue("Display/SelectBoxAlpha", tmp.setNum(settings_display_selectbox_alpha));
    settings.setValue("Display/ZoomScaleIn", tmp.setNum(settings_display_zoomscale_in));
    settings.setValue("Display/ZoomScaleOut", tmp.setNum(settings_display_zoomscale_out));
    settings.setValue("Display/CrossHairPercent", tmp.setNum(settings_display_crosshair_percent));
    settings.setValue("Display/Units", settings_display_units);
    //Prompt
    settings.setValue("Prompt/TextColor", tmp.setNum(settings_prompt_text_color));
    settings.setValue("Prompt/BackgroundColor", tmp.setNum(settings_prompt_bg_color));
    settings.setValue("Prompt/FontFamily", settings_prompt_font_family);
    settings.setValue("Prompt/FontStyle", settings_prompt_font_style);
    settings.setValue("Prompt/FontSize", tmp.setNum(settings_prompt_font_size));
    settings.setValue("Prompt/SaveHistory", settings_prompt_save_history);
    settings.setValue("Prompt/SaveHistoryAsHtml", settings_prompt_save_history_as_html);
    settings.setValue("Prompt/SaveHistoryFilename", settings_prompt_save_history_filename);
    //OpenSave
    settings.setValue("OpenSave/CustomFilter", settings_opensave_custom_filter);
    settings.setValue("OpenSave/OpenFormat", settings_opensave_open_format);
    settings.setValue("OpenSave/OpenThumbnail", settings_opensave_open_thumbnail);
    settings.setValue("OpenSave/SaveFormat", settings_opensave_save_format);
    settings.setValue("OpenSave/SaveThumbnail", settings_opensave_save_thumbnail);
    //Recent
    settings.setValue("OpenSave/RecentMax", tmp.setNum(settings_opensave_recent_max_files));
    settings.setValue("OpenSave/RecentFiles", settings_opensave_recent_list_of_files);
    settings.setValue("OpenSave/RecentDirectory", settings_opensave_recent_directory);
    //Trimming
    settings.setValue("OpenSave/TrimDstNumJumps", tmp.setNum(settings_opensave_trim_dst_num_jumps));
    //Printing
    settings.setValue("Printing/DefaultDevice", settings_printing_default_device);
    settings.setValue("Printing/UseLastDevice", settings_printing_use_last_device);
    settings.setValue("Printing/DisableBG", settings_printing_disable_bg);
    //Grid
    settings.setValue("Grid/ShowOnLoad", settings_grid_show_on_load);
    settings.setValue("Grid/ShowOrigin", settings_grid_show_origin);
    settings.setValue("Grid/ColorMatchCrossHair", settings_grid_color_match_crosshair);
    settings.setValue("Grid/Color", tmp.setNum(settings_grid_color));
    settings.setValue("Grid/LoadFromFile", settings_grid_load_from_file);
    settings.setValue("Grid/Type", settings_grid_type);
    settings.setValue("Grid/CenterOnOrigin", settings_grid_center_on_origin);
    settings.setValue("Grid/CenterX", tmp.setNum(settings_grid_center_x));
    settings.setValue("Grid/CenterY", tmp.setNum(settings_grid_center_y));
    settings.setValue("Grid/SizeX", tmp.setNum(settings_grid_size_x));
    settings.setValue("Grid/SizeY", tmp.setNum(settings_grid_size_y));
    settings.setValue("Grid/SpacingX", tmp.setNum(settings_grid_spacing_x));
    settings.setValue("Grid/SpacingY", tmp.setNum(settings_grid_spacing_y));
    settings.setValue("Grid/SizeRadius", tmp.setNum(settings_grid_size_radius));
    settings.setValue("Grid/SpacingRadius", tmp.setNum(settings_grid_spacing_radius));
    settings.setValue("Grid/SpacingAngle", tmp.setNum(settings_grid_spacing_angle));
    //Ruler
    settings.setValue("Ruler/ShowOnLoad", settings_ruler_show_on_load);
    settings.setValue("Ruler/Metric", settings_ruler_metric);
    settings.setValue("Ruler/Color", tmp.setNum(settings_ruler_color));
    settings.setValue("Ruler/PixelSize", tmp.setNum(settings_ruler_pixel_size));
    //Quick Snap
    settings.setValue("QuickSnap/Enabled", settings_qsnap_enabled);
    settings.setValue("QuickSnap/LocatorColor", tmp.setNum(settings_qsnap_locator_color));
    settings.setValue("QuickSnap/LocatorSize", tmp.setNum(settings_qsnap_locator_size));
    settings.setValue("QuickSnap/ApertureSize", tmp.setNum(settings_qsnap_aperture_size));
    settings.setValue("QuickSnap/EndPoint", settings_qsnap_endpoint);
    settings.setValue("QuickSnap/MidPoint", settings_qsnap_midpoint);
    settings.setValue("QuickSnap/Center", settings_qsnap_center);
    settings.setValue("QuickSnap/Node", settings_qsnap_node);
    settings.setValue("QuickSnap/Quadrant", settings_qsnap_quadrant);
    settings.setValue("QuickSnap/Intersection", settings_qsnap_intersection);
    settings.setValue("QuickSnap/Extension", settings_qsnap_extension);
    settings.setValue("QuickSnap/Insertion", settings_qsnap_insertion);
    settings.setValue("QuickSnap/Perpendicular", settings_qsnap_perpendicular);
    settings.setValue("QuickSnap/Tangent", settings_qsnap_tangent);
    settings.setValue("QuickSnap/Nearest", settings_qsnap_nearest);
    settings.setValue("QuickSnap/Apparent", settings_qsnap_apparent);
    settings.setValue("QuickSnap/Parallel", settings_qsnap_parallel);
    //LineWeight
    settings.setValue("LineWeight/ShowLineWeight", settings_lwt_show_lwt);
    settings.setValue("LineWeight/RealRender", settings_lwt_real_render);
    settings.setValue("LineWeight/DefaultLineWeight", tmp.setNum(settings_lwt_default_lwt));
    //Selection
    settings.setValue("Selection/PickFirst", settings_selection_mode_pickfirst);
    settings.setValue("Selection/PickAdd", settings_selection_mode_pickadd);
    settings.setValue("Selection/PickDrag", settings_selection_mode_pickdrag);
    settings.setValue("Selection/CoolGripColor", tmp.setNum(settings_selection_coolgrip_color));
    settings.setValue("Selection/HotGripColor", tmp.setNum(settings_selection_hotgrip_color));
    settings.setValue("Selection/GripSize", tmp.setNum(settings_selection_grip_size));
    settings.setValue("Selection/PickBoxSize", tmp.setNum(settings_selection_pickbox_size));
    //Text
    settings.setValue("Text/Font", settings_text_font);
    settings.setValue("Text/Size", tmp.setNum(settings_text_size));
    settings.setValue("Text/Angle", tmp.setNum(settings_text_angle));
    settings.setValue("Text/StyleBold", settings_text_style_bold);
    settings.setValue("Text/StyleItalic", settings_text_style_italic);
    settings.setValue("Text/StyleUnderline", settings_text_style_underline);
    settings.setValue("Text/StyleStrikeOut", settings_text_style_strikeout);
    settings.setValue("Text/StyleOverline", settings_text_style_overline);

}

void settingsPrompt()
{
    settingsDialog("Prompt");
}

void settingsDialog(std::string& showTab)
{
    Settings_Dialog dialog(this, showTab, this);
    dialog.exec();
}

#endif
