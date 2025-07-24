/*!
 * \file data.cpp
 * \brief Data loading and TOML support for Embroidermodder.
 *
 * The command_names and settings_table tables are indexed by macro constants
 * so they are compiled in to ensure they stay in order.
 */

#include "embroidermodder.h"

/* Note that toml11 does not have interfacing outside of this file. */
#include "../extern/toml11/single_include/toml.hpp"

/*!
 * \brief Get an integer from a given toml table.
 *
 * Returns -1 if none is found.
 */
int32_t
get_toml_integer(const toml::value value, const char *key)
{
    if (value.contains(key)) {
        return value.at(key).as_integer();
    }
    return -1;
}

/*!
 * \brief Get a QString from a given toml table.
 *
 * Returns an empty string if the value is not found.
 */
const char *
get_toml_string(const toml::value value, const char *key)
{
    if (value.contains(key)) {
        return value.at(key).as_string().c_str();
    }
    return "";
}

/*!
 * \brief Defaults to empty QStringList when no value supplied.
 */
StringList
get_toml_string_table(const toml::value value, const char *key)
{
    StringList list;
    if (value.contains(key)) {
        toml::value table = value.at(key).as_array();
        for (size_t i=0; i<table.size(); i++) {
            list.push_back(table.at(i).as_string());
        }
    }
    return list;
}

/*!
 * \brief Defaults to 0.0 when no value supplied.
 */
bool
get_toml_float(const toml::value value, const char *key)
{
    if (value.contains(key)) {
        return value.at(key).as_floating();
    }
    return 0.0;
}

/*!
 * \brief Defaults to false when no value supplied.
 */
bool
get_toml_boolean(const toml::value value, const char *key)
{
    if (value.contains(key)) {
        return value.at(key).as_boolean();
    }
    return false;
}

/*!
 * \brief Load testing data.
 *
 * Currently just loads the main crash testing script, but could also load
 * other kinds of test.
 */
void
load_test_data(void)
{
    auto data = toml::parse("data/test.toml", toml::spec::v(1, 1, 0));

    string_tables["test_script"] = get_toml_string_table(data, "test_script");
}

/*!
 * \brief Load data associated with user actions.
 */
void
load_action_data(void)
{
    auto data = toml::parse("data/actions.toml", toml::spec::v(1, 1, 0));

    auto actions_table = data.at("actions").as_array();
    for (size_t i=0; i<actions_table.size(); i++) {
        auto current = actions_table.at(i);

        Command command;
        command.icon = get_toml_string(current, "icon");
        command.command = get_toml_string(current, "command");
        command.tooltip = get_toml_string(current, "tooltip");
        command.statustip = get_toml_string(current, "statustip");
        command.shortcut = get_toml_string(current, "shortcut");
        command.macos = get_toml_string(current, "macos");
        command.checkable = get_toml_boolean(current, "checkable");

        command_map.push_back(command);

        StringList alias = get_toml_string_table(current, "alias");
        for (int i=0; i<alias.size(); i++) {
            aliases[alias[i].c_str()] = command.command;
        }
    }
}

/*!
 * \brief Load the data needed to contruct the menus.
 */
void
load_menu_data(void)
{
    debug("Loading menus...");
    auto data = toml::parse("data/menus.toml", toml::spec::v(1, 1, 0));

    string_tables["menubar_order"] = get_toml_string_table(data, "menubar_order");

    auto menus_table = data.at("menus").as_array();
    for (size_t i=0; i<menus_table.size(); i++) {
        auto current = menus_table.at(i);

        MenuData data;
        data.label = get_toml_string(current, "label");
        data.mdi_only = get_toml_boolean(current, "mdi_only");
        data.top_level = get_toml_boolean(current, "top_level");
        data.entries = get_toml_string_table(current, "entries");

        std::string key = qPrintable(get_toml_string(current, "key"));
        menu_table[key] = data;
    }
}

/*!
 * \brief Load the data needed to contruct the toolbars.
 */
void
load_toolbar_data(void)
{
    debug("Loading toolbars...");
    auto data = toml::parse("data/toolbars.toml", toml::spec::v(1, 1, 0));

    string_tables["top_toolbar_order"] = get_toml_string_table(data, "top_toolbar_order");
    string_tables["bottom_toolbar_order"] = get_toml_string_table(data, "bottom_toolbar_order");
    string_tables["left_toolbar_order"] = get_toml_string_table(data, "left_toolbar_order");

    auto toolbars_table = data.at("toolbars").as_array();
    for (size_t i=0; i<toolbars_table.size(); i++) {
        auto current = toolbars_table.at(i);

        ToolbarData toolbar;
        toolbar.label = get_toml_string(current, "label");
        toolbar.mdi_only = get_toml_boolean(current, "mdi_only");
        toolbar.entries = get_toml_string_table(current, "entries");

        std::string key = qPrintable(get_toml_string(current, "key"));
        toolbar_table[key] = toolbar;
    }
}

/*!
 * \brief Load properties data into the struct `PropertiesData`.
 *
 * \note We set an order for each table, since we want the interface to
 *       be reliable.
 */
void
load_properties_data(void)
{
    debug("Loading properties...");
    auto data = toml::parse("data/properties.toml", toml::spec::v(1, 1, 0));

    string_tables["groupbox_order"] = get_toml_string_table(data, "groupbox_order");

    auto table = data.at("properties").as_array();
    for (size_t i=0; i<table.size(); i++) {
        auto current = table.at(i);
        String section = get_toml_string(current, "section");

        auto entries_table = current.at("entries").as_array();
        std::vector<PropertiesData> entries;
        for (size_t i=0; i<entries_table.size(); i++) {
            /* HACK: Not sure why this table only seems to load this way. */
            PropertiesData prop;
            prop.key = toml::find<std::string>(entries_table.at(i), "key");
            prop.label = toml::find<std::string>(entries_table.at(i), "label");
            prop.icon = toml::find<std::string>(entries_table.at(i), "icon");
            prop.type = toml::find<std::string>(entries_table.at(i), "type")[0];
            prop.editable = toml::find<bool>(entries_table.at(i), "editable");
            entries.push_back(prop);
        }
        properties_table[section] = entries;
    }
}

/*!
 * \brief Load settings data into the struct `SettingsData`.
 *
 * \note We set an order for each table, since it is indexed against the ST_* macros.
 */
void
load_settings_data(void)
{
    debug("Loading settings...");
    auto data = toml::parse("data/settings.toml", toml::spec::v(1, 1, 0));

    auto table = data.at("settings_table");
    for (size_t i=0; i<N_SETTINGS; i++) {
        auto current = table.at(i);

        settings_table[i].section = get_toml_string(current, "section");
        settings_table[i].key = get_toml_string(current, "key");
        settings_table[i].default_value = get_toml_string(current, "default_value");
        settings_table[i].type = get_toml_string(current, "type")[0];
        settings_table[i].enabled = get_toml_boolean(current, "enabled");
        settings_table[i].description = get_toml_string(current, "description");
        settings_table[i].icon = get_toml_string(current, "icon");
        settings_table[i].lower = get_toml_float(current, "lower");
        settings_table[i].upper = get_toml_float(current, "upper");
        settings_table[i].single_step = get_toml_float(current, "single_step");
    }

    /* Set up settings data. */
    for (size_t i=0; i<N_SETTINGS; i++) {
        const char *value = settings_table[i].default_value.c_str();
        switch (settings_table[i].type) {
        case 's':
            strncpy(st[i].s, value, 200);
            break;
        case 'i':
            st[i].i = atoi(value);
            break;
        case 'c':
            st[i].u = atoi(value);
            break;
        case 'r':
            st[i].r = atof(value);
            break;
        case 'b': {
            st[i].b = (value[0] == 't');
            break;
        }
        default:
            debug("ERROR: unknown settings type starting with the character %c.",
                settings_table[i].type);
            break;
        }
    }
}

/*!
 * \brief Load less structured data into string lists.
 */
void
load_general_data(void)
{
    auto data = toml::parse("data/general.toml", toml::spec::v(1, 1, 0));
    StringList string_table_list = get_toml_string_table(data, "string_tables");
    for (int i=0; i<string_table_list.size(); i++) {
        string_tables[string_table_list[i]] = get_toml_string_table(
            data, string_table_list[i].c_str());
    }
}

/*!
 * \brief Call all the specialised structured data loaders.
 */
void
load_data(void)
{
    load_test_data();
    load_action_data();
    load_menu_data();
    load_toolbar_data();
    load_properties_data();
    load_settings_data();
    load_general_data();
}

