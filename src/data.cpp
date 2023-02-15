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

#include <fstream>

#include <toml.hpp>

/*
std::string settingsDir = QDir::homePath() + "/.embroidermodder2/";
*/

// This file needs to be read from the users home directory to ensure it is writable
std::string settings_dir = "./";
std::string settings_file = settings_dir + "settings.toml";

/* The actuator changes the program state via these global variables.
 */
Settings settings = {
    .running = true,
    .debug_mode = true,
    .show_about_dialog = false,
    .show_settings_editor = false,
    .show_editor = false,
    .icon_size = 16,
    .icon_theme = "default",
    .pattern_index = 0,
    .assets_dir = "assets/",
    .use_translation = false,
    .language = "default",
    .mdi_bg_use_logo = true,
    .mdi_bg_use_texture = true,
    .mdi_bg_use_color = true,
    .general_mdi_bg_logo = "./images/logo-spirals.png",
    .general_mdi_bg_texture = "./images/texture-spirals.png",
    .general_mdi_bg_color = 0xAAAAAAFF,
    .tip_of_the_day = true,
    .general_current_tip = 0,
    .general_system_help_browser = true,
    .display_use_opengl = false,
    .display_renderhint_aa = false,
    .display_renderhint_text_aa = false,
    .display_renderhint_smooth_pix = false,
    .display_renderhint_high_aa = false,
    .display_renderhint_noncosmetic = false,
    .display_show_scrollbars = true,
    .display_scrollbar_widget_num = 0,
    .display_crosshair_color = 0x000000FF,
    .display_bg_color = 0xEEEEEEFF,
    .display_selectbox_left_color = 0x008000FF,
    .display_selectbox_left_fill = 0x00FF00FF,
    .display_selectbox_right_color = 0x000080FF,
    .display_selectbox_right_fill = 0x0000FFFF,
    .display_selectbox_alpha = 32,
    .display_zoomscale_in = 2.0,
    .display_zoomscale_out = 0.5,
    .display_crosshair_percent = 5,
    .display_units = "mm",
    .prompt_text_color = 0x000000FF,
    .prompt_bg_color = 0xFFFFFFFF,
    .prompt_font_family = "Monospace",
    .prompt_font_style = "normal",
    .prompt_font_size = 12,
    .prompt_save_history = true,
    .prompt_save_history_as_html = false,
    .prompt_save_history_filename = settings_dir + "prompt.log",
    .opensave_custom_filter = "supported",
    .opensave_open_format = "*.*",
    .opensave_open_thumbnail = false,
    .opensave_save_format = "*.*",
    .opensave_save_thumbnail = false,
    .opensave_recent_max_files = 10,
    // .opensave_recent_list_of_files = {},
    .opensave_recent_directory = "./samples",
    .opensave_trim_dst_num_jumps = 5,
    .printing_default_device = "",
    .printing_use_last_device = false,
    .printing_disable_bg = true,
    .grid_show_on_load = true,
    .grid_show_origin = true,
    .grid_color_match_crosshair = true,
    .grid_color = 0x000000FF,
    .grid_load_from_file = true,
    .grid_type = "Rectangular",
    .grid_center_on_origin = true,
    .grid_center_x = 0.0,
    .grid_center_y = 0.0,
    .grid_size_x = 100.0,
    .grid_size_y = 100.0,
    .grid_spacing_x = 25.0,
    .grid_spacing_y = 25.0,
    .grid_size_radius = 50.0,
    .grid_spacing_radius = 25.0,
    .grid_spacing_angle = 45.0,
    .ruler_show_on_load = true,
    .ruler_metric = true,
    .ruler_color = 0xAAAA55FF, //qRgb(210,210, 50);
    .ruler_pixel_size = 20,
    .qsnap_enabled = true,
    .qsnap_locator_color = 0xFFFF00FF,
    .qsnap_locator_size = 4,
    .qsnap_aperture_size = 10,
    .qsnap_endpoint = true,
    .qsnap_midpoint = true,
    .qsnap_center = true,
    .qsnap_node = true,
    .qsnap_quadrant = true,
    .qsnap_intersection = true,
    .qsnap_extension = true,
    .qsnap_insertion = false,
    .qsnap_perpendicular = true,
    .qsnap_tangent = true,
    .qsnap_nearest = false,
    .qsnap_apparent = false,
    .qsnap_parallel = false,
    .lwt_show_lwt = false,
    .lwt_real_render = true,
    .lwt_default_lwt = 0,
    .selection_mode_pickfirst = true,
    .selection_mode_pickadd = true,
    .selection_mode_pickdrag = false,
    .selection_coolgrip_color = 0x0000FFFF,
    .selection_hotgrip_color = 0xFF0000FF,
    .selection_grip_size = 4,
    .selection_pickbox_size = 4,
    .text_font  = "Arial",
    .text_size = 12,
    .text_angle = 0,
    .text_style_bold = false,
    .text_style_italic = false,
    .text_style_underline = false,
    .text_style_overline = false,
    .text_style_strikeout = false
};

bool show_details_dialog = false;

void read_setting(toml::value table, std::string key, std::string *s)
{
    if (table.contains(key)) {
        *s = toml::find<std::string>(table, key);
    }
}

void read_setting(toml::value table, std::string key, int *i)
{
    if (table.contains(key)) {
        *i = toml::find<int>(table, key);
    }
}

void read_setting(toml::value table, std::string key, bool *b)
{
    if (table.contains(key)) {
        *b = toml::find<bool>(table, key);
    }
}

void write_setting(FILE *file, std::string key, float value)
{
    fprintf(file, "%s=%f\n", key.c_str(), value);
}

void write_setting(FILE *file, std::string key, int value)
{
    fprintf(file, "%s=%d\n", key.c_str(), value);
}

void write_setting(FILE *file, std::string key, unsigned int value)
{
    fprintf(file, "%s=%u\n", key.c_str(), value);
}

void write_setting(FILE *file, std::string key, std::string value)
{
    fprintf(file, "%s=\"%s\"\n", key.c_str(), value.c_str());
}

void write_setting(FILE *file, std::string key, bool value)
{
    if (value) {
        fprintf(file, "%s=true\n", key.c_str());
    }
    else {
        fprintf(file, "%s=false\n", key.c_str());
    }    
}

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

    /*
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

int file_exists(std::string s)
{
    std::ifstream f(s);
    return f.good();
}


void read_settings(void)
{
    debug_message("Reading Settings...");
    /*
    EmbVector pos(0,0);
    QSize size(800,600);

    some layout state recording? */
    if (!file_exists(settings_file)) {
        return;
    }

    toml::value setting_toml = toml::parse(settings_file);
    /*
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
    */

    if (setting_toml.contains("general")) {
        toml::value table = toml::find<toml::value>(setting_toml, "general");
        read_setting(table, "language", &(settings.language));
        read_setting(table, "icon_size", &(settings.icon_size));
        read_setting(table, "icon_theme", &(settings.icon_theme));
        read_setting(table, "use_translation", &(settings.use_translation));
        read_setting(table, "mdi_bg_use_logo", &(settings.mdi_bg_use_logo));
        read_setting(table, "mdi_bg_use_texture", &(settings.mdi_bg_use_texture));
        read_setting(table, "mdi_bg_use_color", &(settings.mdi_bg_use_color));
        /*
        settings.general_mdi_bg_logo = toml::find<std::string>(table, "mdi_bg_logo");
        settings.general_mdi_bg_texture = toml::find<std::string>(table, "mdi_bg_texture");
        settings.general_mdi_bg_color = toml::find<int>(table, "mdi_bg_color");
        settings.tip_of_the_day = toml::find<bool>(table, "tip_of_the_day");
        settings.general_current_tip = toml::find<bool>(table, "current_tip");
        settings.general_system_help_browser = toml::find<bool>(table, "general_system_help_browser");
        */
    }

    /*
    if (setting_toml.contains("display")) {
        toml::value table = toml::find<toml::value>(setting_toml, "display");
    }
    */

    /*
    .display_use_opengl = false,
    .display_renderhint_aa = false,
    .display_renderhint_text_aa = false,
    .display_renderhint_smooth_pix = false,
    .display_renderhint_high_aa = false,
    .display_renderhint_noncosmetic = false,
    .display_show_scrollbars = true,
    .display_scrollbar_widget_num = 0,
    .display_crosshair_color = 0x000000FF,
    .display_bg_color = 0xEEEEEEFF,
    .display_selectbox_left_color = 0x008000FF,
    .display_selectbox_left_fill = 0x00FF00FF,
    .display_selectbox_right_color = 0x000080FF,
    .display_selectbox_right_fill = 0x0000FFFF,
    .display_selectbox_alpha = 32,
    .display_zoomscale_in = 2.0,
    .display_zoomscale_out = 0.5,
    .display_crosshair_percent = 5,
    .display_units = "mm",
    .prompt_text_color = 0x000000FF,
    .prompt_bg_color = 0xFFFFFFFF,
    .prompt_font_family = "Monospace",
    .prompt_font_style = "normal",
    .prompt_font_size = 12,
    .prompt_save_history = true,
    .prompt_save_history_as_html = false,
    .prompt_save_history_filename = settings_dir + "prompt.log",
    .opensave_custom_filter = "supported",
    .opensave_open_format = "*.*",
    .opensave_open_thumbnail = false,
    .opensave_save_format = "*.*",
    .opensave_save_thumbnail = false,
    .opensave_recent_max_files = 10,
    // .opensave_recent_list_of_files = {},
    .opensave_recent_directory = "./samples",
    .opensave_trim_dst_num_jumps = 5,
    .printing_default_device = "",
    .printing_use_last_device = false,
    .printing_disable_bg = true,
    .grid_show_on_load = true,
    .grid_show_origin = true,
    .grid_color_match_crosshair = true,
    .grid_color = 0x000000FF,
    .grid_load_from_file = true,
    .grid_type = "Rectangular",
    .grid_center_on_origin = true,
    .grid_center_x = 0.0,
    .grid_center_y = 0.0,
    .grid_size_x = 100.0,
    .grid_size_y = 100.0,
    .grid_spacing_x = 25.0,
    .grid_spacing_y = 25.0,
    .grid_size_radius = 50.0,
    .grid_spacing_radius = 25.0,
    .grid_spacing_angle = 45.0,
    .ruler_show_on_load = true,
    .ruler_metric = true,
    .ruler_color = 0xAAAA55FF, //qRgb(210,210, 50);
    .ruler_pixel_size = 20,
    .qsnap_enabled = true,
    .qsnap_locator_color = 0xFFFF00FF,
    .qsnap_locator_size = 4,
    .qsnap_aperture_size = 10,
    .qsnap_endpoint = true,
    .qsnap_midpoint = true,
    .qsnap_center = true,
    .qsnap_node = true,
    .qsnap_quadrant = true,
    .qsnap_intersection = true,
    .qsnap_extension = true,
    .qsnap_insertion = false,
    .qsnap_perpendicular = true,
    .qsnap_tangent = true,
    .qsnap_nearest = false,
    .qsnap_apparent = false,
    .qsnap_parallel = false,
    .lwt_show_lwt = false,
    .lwt_real_render = true,
    .lwt_default_lwt = 0,
    .selection_mode_pickfirst = true,
    .selection_mode_pickadd = true,
    .selection_mode_pickdrag = false,
    .selection_coolgrip_color = 0x0000FFFF,
    .selection_hotgrip_color = 0xFF0000FF,
    .selection_grip_size = 4,
    .selection_pickbox_size = 4,
    .text_font  = "Arial",
    .text_size = 12,
    .text_angle = 0,
    .text_style_bold = false,
    .text_style_italic = false,
    .text_style_underline = false,
    .text_style_overline = false,
    .text_style_strikeout = false

    move(pos);
    resize(size);*/
}

void write_settings(void)
{
    debug_message("Writing Settings...");
    FILE *file = fopen(settings_file.c_str(), "w");

    fprintf(file, "[Window]\n");
    /*
    write_setting(file, "PositionX", pos().x);
    write_setting(file, "PositionY", pos().y);
    write_setting(file, "SizeX", size().x);
    write_setting(file, "SizeX", size().y);
    */

    fprintf(file, "[general]\n");
    //write_setting(file, "LayoutState", layoutState);
    write_setting(file, "language", settings.language);
    write_setting(file, "icon_theme", settings.icon_theme);
    write_setting(file, "icon_size", settings.icon_size);
    write_setting(file, "mdi_bg_use_logo", settings.mdi_bg_use_logo);
    write_setting(file, "mdi_bg_use_texture", settings.mdi_bg_use_texture);
    write_setting(file, "mdi_bg_use_color", settings.mdi_bg_use_color);
    write_setting(file, "mdi_bg_logo", settings.general_mdi_bg_logo);
    write_setting(file, "mdi_bg_texture", settings.general_mdi_bg_texture);
    write_setting(file, "mdi_bg_color", settings.general_mdi_bg_color);
    write_setting(file, "tip_of_the_day", settings.tip_of_the_day);
    write_setting(file, "current_tip", settings.general_current_tip + 1);
    write_setting(file, "system_help_browser", settings.general_system_help_browser);
    fprintf(file, "\n");

    fprintf(file, "[display]\n");
    write_setting(file, "UseOpenGL", settings.display_use_opengl);
    write_setting(file, "RenderHintAntiAlias", settings.display_renderhint_aa);
    write_setting(file, "RenderHintTextAntiAlias", settings.display_renderhint_text_aa);
    write_setting(file, "RenderHintSmoothPixmap", settings.display_renderhint_smooth_pix);
    write_setting(file, "RenderHintHighQualityAntiAlias", settings.display_renderhint_high_aa);
    write_setting(file, "RenderHintNonCosmetic", settings.display_renderhint_noncosmetic);
    write_setting(file, "ShowScrollBars", settings.display_show_scrollbars);
    write_setting(file, "ScrollBarWidgetNum", settings.display_scrollbar_widget_num);
    write_setting(file, "CrossHairColor", settings.display_crosshair_color);
    write_setting(file, "BackgroundColor", settings.display_bg_color);
    write_setting(file, "SelectBoxLeftColor", settings.display_selectbox_left_color);
    write_setting(file, "SelectBoxLeftFill", settings.display_selectbox_left_fill);
    write_setting(file, "SelectBoxRightColor", settings.display_selectbox_right_color);
    write_setting(file, "SelectBoxRightFill", settings.display_selectbox_right_fill);
    write_setting(file, "SelectBoxAlpha", settings.display_selectbox_alpha);
    write_setting(file, "ZoomScaleIn", settings.display_zoomscale_in);
    write_setting(file, "ZoomScaleOut", settings.display_zoomscale_out);
    write_setting(file, "CrossHairPercent", settings.display_crosshair_percent);
    write_setting(file, "Units", settings.display_units);
    fprintf(file, "\n");
 
    fprintf(file, "[Prompt]\n");
    write_setting(file, "TextColor", settings.prompt_text_color);
    write_setting(file, "BackgroundColor", settings.prompt_bg_color);
    write_setting(file, "FontFamily", settings.prompt_font_family);
    write_setting(file, "FontStyle", settings.prompt_font_style);
    write_setting(file, "FontSize", settings.prompt_font_size);
    write_setting(file, "SaveHistory", settings.prompt_save_history);
    write_setting(file, "SaveHistoryAsHtml", settings.prompt_save_history_as_html);
    write_setting(file, "SaveHistoryFilename", settings.prompt_save_history_filename);
    fprintf(file, "\n");

    fprintf(file, "[OpenSave]\n");
    write_setting(file, "CustomFilter", settings.opensave_custom_filter);
    write_setting(file, "OpenFormat", settings.opensave_open_format);
    write_setting(file, "OpenThumbnail", settings.opensave_open_thumbnail);
    write_setting(file, "SaveFormat", settings.opensave_save_format);
    write_setting(file, "SaveThumbnail", settings.opensave_save_thumbnail);
    write_setting(file, "RecentMax", settings.opensave_recent_max_files);
    //write_setting(file, "RecentFiles", settings.opensave_recent_list_of_files);
    write_setting(file, "RecentDirectory", settings.opensave_recent_directory);
    write_setting(file, "TrimDstNumJumps", settings.opensave_trim_dst_num_jumps);
    fprintf(file, "\n");

    fprintf(file, "[Printing]\n");
    write_setting(file, "DefaultDevice", settings.printing_default_device);
    write_setting(file, "UseLastDevice", settings.printing_use_last_device);
    write_setting(file, "DisableBG", settings.printing_disable_bg);
    fprintf(file, "\n");

    fprintf(file, "[Grid]\n");
    write_setting(file, "ShowOnLoad", settings.grid_show_on_load);
    write_setting(file, "ShowOrigin", settings.grid_show_origin);
    write_setting(file, "ColorMatchCrossHair", settings.grid_color_match_crosshair);
    write_setting(file, "Color", settings.grid_color);
    write_setting(file, "LoadFromFile", settings.grid_load_from_file);
    write_setting(file, "Type", settings.grid_type);
    write_setting(file, "CenterOnOrigin", settings.grid_center_on_origin);
    write_setting(file, "CenterX", settings.grid_center_x);
    write_setting(file, "CenterY", settings.grid_center_y);
    write_setting(file, "SizeX", settings.grid_size_x);
    write_setting(file, "SizeY", settings.grid_size_y);
    write_setting(file, "SpacingX", settings.grid_spacing_x);
    write_setting(file, "SpacingY", settings.grid_spacing_y);
    write_setting(file, "SizeRadius", settings.grid_size_radius);
    write_setting(file, "SpacingRadius", settings.grid_spacing_radius);
    write_setting(file, "SpacingAngle", settings.grid_spacing_angle);
    fprintf(file, "\n");

    fprintf(file, "[Ruler]\n");
    write_setting(file, "ShowOnLoad", settings.ruler_show_on_load);
    write_setting(file, "Metric", settings.ruler_metric);
    write_setting(file, "Color", settings.ruler_color);
    write_setting(file, "PixelSize", settings.ruler_pixel_size);
    fprintf(file, "\n");

    fprintf(file, "[QuickSnap]\n");
    write_setting(file, "Enabled", settings.qsnap_enabled);
    write_setting(file, "LocatorColor", settings.qsnap_locator_color);
    write_setting(file, "LocatorSize", settings.qsnap_locator_size);
    write_setting(file, "ApertureSize", settings.qsnap_aperture_size);
    write_setting(file, "EndPoint", settings.qsnap_endpoint);
    write_setting(file, "MidPoint", settings.qsnap_midpoint);
    write_setting(file, "Center", settings.qsnap_center);
    write_setting(file, "Node", settings.qsnap_node);
    write_setting(file, "Quadrant", settings.qsnap_quadrant);
    write_setting(file, "Intersection", settings.qsnap_intersection);
    write_setting(file, "Extension", settings.qsnap_extension);
    write_setting(file, "Insertion", settings.qsnap_insertion);
    write_setting(file, "Perpendicular", settings.qsnap_perpendicular);
    write_setting(file, "Tangent", settings.qsnap_tangent);
    write_setting(file, "Nearest", settings.qsnap_nearest);
    write_setting(file, "Apparent", settings.qsnap_apparent);
    write_setting(file, "Parallel", settings.qsnap_parallel);
    fprintf(file, "\n");

    fprintf(file, "[LineWeight]\n");
    write_setting(file, "ShowLineWeight", settings.lwt_show_lwt);
    write_setting(file, "RealRender", settings.lwt_real_render);
    write_setting(file, "DefaultLineWeight", settings.lwt_default_lwt);
    fprintf(file, "\n");

    fprintf(file, "[Selection]\n");
    write_setting(file, "PickFirst", settings.selection_mode_pickfirst);
    write_setting(file, "PickAdd", settings.selection_mode_pickadd);
    write_setting(file, "PickDrag", settings.selection_mode_pickdrag);
    write_setting(file, "CoolGripColor", settings.selection_coolgrip_color);
    write_setting(file, "HotGripColor", settings.selection_hotgrip_color);
    write_setting(file, "GripSize", settings.selection_grip_size);
    write_setting(file, "PickBoxSize", settings.selection_pickbox_size);
    fprintf(file, "\n");

    fprintf(file, "[Text]\n");
    write_setting(file, "Font", settings.text_font);
    write_setting(file, "Size", settings.text_size);
    write_setting(file, "Angle", settings.text_angle);
    write_setting(file, "StyleBold", settings.text_style_bold);
    write_setting(file, "StyleItalic", settings.text_style_italic);
    write_setting(file, "StyleUnderline", settings.text_style_underline);
    write_setting(file, "StyleStrikeOut", settings.text_style_strikeout);
    write_setting(file, "StyleOverline", settings.text_style_overline);
    fclose(file);
}

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
