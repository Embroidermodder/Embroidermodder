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
    toml::value config = toml::parse(assets_dir + "em2_config.toml");

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
