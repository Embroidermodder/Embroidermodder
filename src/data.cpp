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

#include <toml.hpp>

/*
#if defined(Q_OS_UNIX) || defined(Q_OS_MAC)
std::string settingsDir = QDir::homePath() + "/.embroidermodder2/";
#else
std::string settingsDir = "";
#endif
std::string defaultsPath = "./defaults.toml";
std::string settingsPath = settingsDir + "settings.toml";
*/

/* The actuator changes the program state via these global variables.
 */
float output;

Settings settings = {
    .running = true,
    .debug_mode = true,
    .show_about_dialog = false,
    .show_settings_editor = false,
    .show_editor = false,
    .icon_size = 16,
    .pattern_index = 0,
    .assets_dir = "../assets/",
    .use_translation = false,
    .language = "default",
    .mdi_bg_use_logo = true,
    .mdi_bg_use_texture = true,
    .mdi_bg_use_color = true,
    .tip_of_the_day = 0
};

string_matrix
load_string_matrix(toml::value config, std::string menu_name)
{
    string_matrix menu;
    for (const auto &i : config[menu_name].as_array()) {
        std::vector<std::string> entry;
        entry.push_back(toml::get<std::string>(i[0]));
        entry.push_back(toml::get<std::string>(i[1]));
        menu.push_back(entry);
    }
    return menu;
}

void
load_configuration(void)
{
    toml::value config = toml::parse(settings.assets_dir + "em2_config.toml");

    string_matrix menu = load_string_matrix(config, "menu_layout");
    for (const auto &i : menu) {
        menu_layout[i[0]] = load_string_matrix(config, i[1]);
    }

    /*
    string_matrix actions = load_string_matrix(config, "action_list");
    for (const auto &i : actions) {
        Action action;
        action.command = i[0];
        action.menu_label = i[1];
        action.description = i[2];
        action_list.push_back(action);
    }
    */

    for (const auto &[i, j] : config.as_table()) {
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
        }
    }
    for (const auto &[i, j] : config.as_table()) {
        if (!config[i].is_table()) {
            continue;
        }
        if (!config[i].contains("type")) {
            continue;
        }
        std::string s = toml::get<std::string>(config[i]["type"]);
        if (s == "menu-item") {
        }
    }
}

#if 0

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
            std::string menuName = toml::get<std::string>(config[i]["menu"]);
            std::string cmdName = toml::get<std::string>(config[i]["command"]);
            std::string icon = toml::get<std::string>(config[i]["icon"]);
            std::string toolTip = toml::get<std::string>(config[i]["tooltip"]);
            std::string statusTip = toml::get<std::string>(config[i]["statustip"]);
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
    settings.general_language = "default";
    settings.general_icon_theme = "default";
    settings.general_icon_size = 16;
    settings.general_mdi_bg_use_logo = true;
    settings.general_mdi_bg_use_texture = true;
    settings.general_mdi_bg_use_color = true;
    settings.general_mdi_bg_logo = "./images/logo-spirals.png";
    settings.general_mdi_bg_texture = "./images/texture-spirals.png";
    settings.general_mdi_bg_color = qRgb(192,192,192);
    settings.general_tip_of_the_day = true;
    settings.general_current_tip = 0;
    settings.general_system_help_browser = true;
    //Display
    settings.display_use_opengl = false;
    settings.display_renderhint_aa = false;
    settings.display_renderhint_text_aa = false;
    settings.display_renderhint_smooth_pix = false;
    settings.display_renderhint_high_aa = false;
    settings.display_renderhint_noncosmetic = false;
    settings.display_show_scrollbars = true;
    settings.display_scrollbar_widget_num = 0;
    settings.display_crosshair_color = qRgb(  0, 0, 0) ;
    settings.display_bg_color = qRgb(235,235,235) ;
    settings.display_selectbox_left_color = qRgb(  0,128, 0) ;
    settings.display_selectbox_left_fill = qRgb(  0,255, 0) ;
    settings.display_selectbox_right_color = qRgb(  0, 0,128) ;
    settings.display_selectbox_right_fill = qRgb(  0, 0,255) ;
    settings.display_selectbox_alpha = 32;
    settings.display_zoomscale_in = 2.0;
    settings.display_zoomscale_out = 0.5;
    settings.display_crosshair_percent = 5;
    settings.display_units = "mm";
    //Prompt
    settings.prompt_text_color = qRgb(  0, 0, 0) ;
    settings.prompt_bg_color = qRgb(255,255,255) ;
    settings.prompt_font_family = "Monospace";
    settings.prompt_font_style = "normal";
    settings.prompt_font_size = 12;
    settings.prompt_save_history = true;
    settings.prompt_save_history_as_html = false;
    settings.prompt_save_history_filename = settingsDir + "prompt.log";
    //OpenSave
    settings.opensave_custom_filter = "supported";
    settings.opensave_open_format = "*.*";
    settings.opensave_open_thumbnail = false;
    settings.opensave_save_format = "*.*";
    settings.opensave_save_thumbnail = false;
    //Recent
    settings.opensave_recent_max_files = 10;
    settings.opensave_recent_list_of_files = {};
    settings.opensave_recent_directory = "./samples";
    //Trimming
    settings.opensave_trim_dst_num_jumps = 5;
    //Printing
    settings.printing_default_device = "";
    settings.printing_use_last_device = false;
    settings.printing_disable_bg = true;
    //Grid
    settings.grid_show_on_load = true;
    settings.grid_show_origin = true;
    settings.grid_color_match_crosshair = true;
    settings.grid_color = qRgb(  0, 0, 0);
    settings.grid_load_from_file = true;
    settings.grid_type = "Rectangular";
    settings.grid_center_on_origin = true;
    settings.grid_center_x = 0.0;
    settings.grid_center_y = 0.0;
    settings.grid_size_x = 100.0;
    settings.grid_size_y = 100.0;
    settings.grid_spacing_x = 25.0;
    settings.grid_spacing_y = 25.0;
    settings.grid_size_radius = 50.0;
    settings.grid_spacing_radius = 25.0;
    settings.grid_spacing_angle = 45.0;
    //Ruler
    settings.ruler_show_on_load = true;
    settings.ruler_metric = true;
    settings.ruler_color = qRgb(210,210, 50);
    settings.ruler_pixel_size = 20;
    //Quick Snap
    settings.qsnap_enabled = true;
    settings.qsnap_locator_color = qRgb(255,255, 0);
    settings.qsnap_locator_size = 4;
    settings.qsnap_aperture_size = 10;
    settings.qsnap_endpoint = true;
    settings.qsnap_midpoint = true;
    settings.qsnap_center = true;
    settings.qsnap_node = true;
    settings.qsnap_quadrant = true;
    settings.qsnap_intersection = true;
    settings.qsnap_extension = true;
    settings.qsnap_insertion = false;
    settings.qsnap_perpendicular = true;
    settings.qsnap_tangent = true;
    settings.qsnap_nearest = false;
    settings.qsnap_apparent = false;
    settings.qsnap_parallel = false;
    //LineWeight
    settings.lwt_show_lwt = false;
    settings.lwt_real_render = true;
    settings.lwt_default_lwt = 0;
    //Selection
    settings.selection_mode_pickfirst = true;
    settings.selection_mode_pickadd = true;
    settings.selection_mode_pickdrag = false;
    settings.selection_coolgrip_color = qRgb(  0, 0,255);
    settings.selection_hotgrip_color = qRgb(255, 0, 0);
    settings.selection_grip_size = 4;
    settings.selection_pickbox_size = 4;
    //Text
    settings.text_font  = "Arial";
    settings.text_size = 12;
    settings.text_angle = 0;
    settings.text_style_bold = false;
    settings.text_style_italic = false;
    settings.text_style_underline = false;
    settings.text_style_strikeout = false;
    settings.text_style_overline = false;

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
        settings.general_language = std::string::fromStdString(toml::find<std::string>(table, "language"));
        settings.general_icon_theme = std::string::fromStdString(toml::find<std::string>(table, "icon_theme"));
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
    settings.setValue("Language", settings.general_language);
    settings.setValue("IconTheme", settings.general_icon_theme);
    settings.setValue("IconSize", tmp.setNum(settings.general_icon_size));
    settings.setValue("MdiBGUseLogo", settings.general_mdi_bg_use_logo);
    settings.setValue("MdiBGUseTexture", settings.general_mdi_bg_use_texture);
    settings.setValue("MdiBGUseColor", settings.general_mdi_bg_use_color);
    settings.setValue("MdiBGLogo", settings.general_mdi_bg_logo);
    settings.setValue("MdiBGTexture", settings.general_mdi_bg_texture);
    settings.setValue("MdiBGColor", tmp.setNum(settings.general_mdi_bg_color));
    settings.setValue("TipOfTheDay", settings.general_tip_of_the_day);
    settings.setValue("CurrentTip", tmp.setNum(settings.general_current_tip + 1));
    settings.setValue("SystemHelpBrowser", settings.general_system_help_browser);
    //Display
    settings.setValue("Display/UseOpenGL", settings.display_use_opengl);
    settings.setValue("Display/RenderHintAntiAlias", settings.display_renderhint_aa);
    settings.setValue("Display/RenderHintTextAntiAlias", settings.display_renderhint_text_aa);
    settings.setValue("Display/RenderHintSmoothPixmap", settings.display_renderhint_smooth_pix);
    settings.setValue("Display/RenderHintHighQualityAntiAlias", settings.display_renderhint_high_aa);
    settings.setValue("Display/RenderHintNonCosmetic", settings.display_renderhint_noncosmetic);
    settings.setValue("Display/ShowScrollBars", settings.display_show_scrollbars);
    settings.setValue("Display/ScrollBarWidgetNum", tmp.setNum(settings.display_scrollbar_widget_num));
    settings.setValue("Display/CrossHairColor", tmp.setNum(settings.display_crosshair_color));
    settings.setValue("Display/BackgroundColor", tmp.setNum(settings.display_bg_color));
    settings.setValue("Display/SelectBoxLeftColor", tmp.setNum(settings.display_selectbox_left_color));
    settings.setValue("Display/SelectBoxLeftFill", tmp.setNum(settings.display_selectbox_left_fill));
    settings.setValue("Display/SelectBoxRightColor", tmp.setNum(settings.display_selectbox_right_color));
    settings.setValue("Display/SelectBoxRightFill", tmp.setNum(settings.display_selectbox_right_fill));
    settings.setValue("Display/SelectBoxAlpha", tmp.setNum(settings.display_selectbox_alpha));
    settings.setValue("Display/ZoomScaleIn", tmp.setNum(settings.display_zoomscale_in));
    settings.setValue("Display/ZoomScaleOut", tmp.setNum(settings.display_zoomscale_out));
    settings.setValue("Display/CrossHairPercent", tmp.setNum(settings.display_crosshair_percent));
    settings.setValue("Display/Units", settings.display_units);
    //Prompt
    settings.setValue("Prompt/TextColor", tmp.setNum(settings.prompt_text_color));
    settings.setValue("Prompt/BackgroundColor", tmp.setNum(settings.prompt_bg_color));
    settings.setValue("Prompt/FontFamily", settings.prompt_font_family);
    settings.setValue("Prompt/FontStyle", settings.prompt_font_style);
    settings.setValue("Prompt/FontSize", tmp.setNum(settings.prompt_font_size));
    settings.setValue("Prompt/SaveHistory", settings.prompt_save_history);
    settings.setValue("Prompt/SaveHistoryAsHtml", settings.prompt_save_history_as_html);
    settings.setValue("Prompt/SaveHistoryFilename", settings.prompt_save_history_filename);
    //OpenSave
    settings.setValue("OpenSave/CustomFilter", settings.opensave_custom_filter);
    settings.setValue("OpenSave/OpenFormat", settings.opensave_open_format);
    settings.setValue("OpenSave/OpenThumbnail", settings.opensave_open_thumbnail);
    settings.setValue("OpenSave/SaveFormat", settings.opensave_save_format);
    settings.setValue("OpenSave/SaveThumbnail", settings.opensave_save_thumbnail);
    //Recent
    settings.setValue("OpenSave/RecentMax", tmp.setNum(settings.opensave_recent_max_files));
    settings.setValue("OpenSave/RecentFiles", settings.opensave_recent_list_of_files);
    settings.setValue("OpenSave/RecentDirectory", settings.opensave_recent_directory);
    //Trimming
    settings.setValue("OpenSave/TrimDstNumJumps", tmp.setNum(settings.opensave_trim_dst_num_jumps));
    //Printing
    settings.setValue("Printing/DefaultDevice", settings.printing_default_device);
    settings.setValue("Printing/UseLastDevice", settings.printing_use_last_device);
    settings.setValue("Printing/DisableBG", settings.printing_disable_bg);
    //Grid
    settings.setValue("Grid/ShowOnLoad", settings.grid_show_on_load);
    settings.setValue("Grid/ShowOrigin", settings.grid_show_origin);
    settings.setValue("Grid/ColorMatchCrossHair", settings.grid_color_match_crosshair);
    settings.setValue("Grid/Color", tmp.setNum(settings.grid_color));
    settings.setValue("Grid/LoadFromFile", settings.grid_load_from_file);
    settings.setValue("Grid/Type", settings.grid_type);
    settings.setValue("Grid/CenterOnOrigin", settings.grid_center_on_origin);
    settings.setValue("Grid/CenterX", tmp.setNum(settings.grid_center_x));
    settings.setValue("Grid/CenterY", tmp.setNum(settings.grid_center_y));
    settings.setValue("Grid/SizeX", tmp.setNum(settings.grid_size_x));
    settings.setValue("Grid/SizeY", tmp.setNum(settings.grid_size_y));
    settings.setValue("Grid/SpacingX", tmp.setNum(settings.grid_spacing_x));
    settings.setValue("Grid/SpacingY", tmp.setNum(settings.grid_spacing_y));
    settings.setValue("Grid/SizeRadius", tmp.setNum(settings.grid_size_radius));
    settings.setValue("Grid/SpacingRadius", tmp.setNum(settings.grid_spacing_radius));
    settings.setValue("Grid/SpacingAngle", tmp.setNum(settings.grid_spacing_angle));
    //Ruler
    settings.setValue("Ruler/ShowOnLoad", settings.ruler_show_on_load);
    settings.setValue("Ruler/Metric", settings.ruler_metric);
    settings.setValue("Ruler/Color", tmp.setNum(settings.ruler_color));
    settings.setValue("Ruler/PixelSize", tmp.setNum(settings.ruler_pixel_size));
    //Quick Snap
    settings.setValue("QuickSnap/Enabled", settings.qsnap_enabled);
    settings.setValue("QuickSnap/LocatorColor", tmp.setNum(settings.qsnap_locator_color));
    settings.setValue("QuickSnap/LocatorSize", tmp.setNum(settings.qsnap_locator_size));
    settings.setValue("QuickSnap/ApertureSize", tmp.setNum(settings.qsnap_aperture_size));
    settings.setValue("QuickSnap/EndPoint", settings.qsnap_endpoint);
    settings.setValue("QuickSnap/MidPoint", settings.qsnap_midpoint);
    settings.setValue("QuickSnap/Center", settings.qsnap_center);
    settings.setValue("QuickSnap/Node", settings.qsnap_node);
    settings.setValue("QuickSnap/Quadrant", settings.qsnap_quadrant);
    settings.setValue("QuickSnap/Intersection", settings.qsnap_intersection);
    settings.setValue("QuickSnap/Extension", settings.qsnap_extension);
    settings.setValue("QuickSnap/Insertion", settings.qsnap_insertion);
    settings.setValue("QuickSnap/Perpendicular", settings.qsnap_perpendicular);
    settings.setValue("QuickSnap/Tangent", settings.qsnap_tangent);
    settings.setValue("QuickSnap/Nearest", settings.qsnap_nearest);
    settings.setValue("QuickSnap/Apparent", settings.qsnap_apparent);
    settings.setValue("QuickSnap/Parallel", settings.qsnap_parallel);
    //LineWeight
    settings.setValue("LineWeight/ShowLineWeight", settings.lwt_show_lwt);
    settings.setValue("LineWeight/RealRender", settings.lwt_real_render);
    settings.setValue("LineWeight/DefaultLineWeight", tmp.setNum(settings.lwt_default_lwt));
    //Selection
    settings.setValue("Selection/PickFirst", settings.selection_mode_pickfirst);
    settings.setValue("Selection/PickAdd", settings.selection_mode_pickadd);
    settings.setValue("Selection/PickDrag", settings.selection_mode_pickdrag);
    settings.setValue("Selection/CoolGripColor", tmp.setNum(settings.selection_coolgrip_color));
    settings.setValue("Selection/HotGripColor", tmp.setNum(settings.selection_hotgrip_color));
    settings.setValue("Selection/GripSize", tmp.setNum(settings.selection_grip_size));
    settings.setValue("Selection/PickBoxSize", tmp.setNum(settings.selection_pickbox_size));
    //Text
    settings.setValue("Text/Font", settings.text_font);
    settings.setValue("Text/Size", tmp.setNum(settings.text_size));
    settings.setValue("Text/Angle", tmp.setNum(settings.text_angle));
    settings.setValue("Text/StyleBold", settings.text_style_bold);
    settings.setValue("Text/StyleItalic", settings.text_style_italic);
    settings.setValue("Text/StyleUnderline", settings.text_style_underline);
    settings.setValue("Text/StyleStrikeOut", settings.text_style_strikeout);
    settings.setValue("Text/StyleOverline", settings.text_style_overline);

}
#endif

std::string translate(std::string string)
{
    if (settings.use_translation) {
        return string;
    }
    for (auto i : translation_table) {
        if (i[0] == string) {
            return i[1];
        }
    }
    return string;
}

void debug_message(std::string string)
{
    if (settings.debug_mode) {
        std::cout << string << std::endl;
    }
}
