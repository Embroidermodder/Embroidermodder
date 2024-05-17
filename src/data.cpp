/*!
 * \file data.cpp
 * \brief TOML loading.
 *
 * Embroidermodder 2.
 *
 * Copyright 2011-2025 The Embroidermodder Team
 * Embroidermodder 2 is Open Source Software, see LICENSE.md for licensing terms.
 *
 * Read the reference manual (https://www.libembroidery.org/downloads/emrm.pdf)
 * for advice on altering this file.
 *
 * -----------------------------------------------------------------------------
 *
 * TOML++ documentation
 * https://marzer.github.io/tomlplusplus/
 */

#include <iostream>

#include "core.h"
#include "../extern/toml11/single_include/toml.hpp"

typedef struct std::vector<std::string> StrList;
Dictionary state;

const ScriptValue script_null = {
    .r = 0.0F,
    .i = 0,
    .b = false,
    .s = "",
    .label = "NULL",
    .type = SCRIPT_NULL
};

/*! The list of all the asset files used by Embroidermodder.
 *
 * The manifest is fixed: user overrides to the set files are available
 * by adding to "overrides.toml" in the configuration folder (as opposed to
 * the shared asset folder). For testing purposes only we can change the
 * parent assets folder containing all these using the --assets flag.
 */
static std::vector<std::string> manifest = {
    "data/versions.toml",
    "data/paths.toml",
    "data/settings.toml",
    "ui/toolbars.toml",
    "ui/menus.toml",
    "ui/widgets.toml",
    "tips.toml",
    "coverage_test.toml"
};

std::vector<std::string> variables = {
    "menu_list",

    "file_menu",
    "edit_menu",
    "view_menu",
    "zoom_menu",
    "pan_menu",
    "help_menu",
    "draw_menu",
    "tools_menu",
    "modify_menu",
    "dimension_menu",
    "sandbox_menu",
    "recent_menu",
    "window_menu",

    "toolbar_list",

    "file_toolbar",
    "edit_toolbar",
    "view_toolbar",
    "icon_toolbar",
    "help_toolbar",
    "draw_toolbar",
    "inquiry_toolbar",
    "modify_toolbar",
    "dimension_toolbar",
    "sandbox_toolbar",
    "layer_toolbar",
    "properties_toolbar",
    "text_toolbar",
    "prompt_toolbar",

    "settings_data",
    "tips",
    "coverage_test",
    
    "editor_list",
    "combobox_list",
    "layer_list",
    "color_list",
    "line_type_list",
    "line_weight_list",
    "text_size_list"
};

/*! \brief Load the entire file with path fname into the buffer passed in.
 *
 * Caller is responsible for reporting the error, as we use this to determine
 * if the file exists as well as if it can be read.
 */
size_t
load_file_to_buffer(char *fname, char *buffer)
{
    FILE *f = fopen(fname, "r");
    if (!f) {
        return 0;        
    }
    fseek(f, 0, SEEK_END);
    size_t size = ftell(f);
    fseek(f, 0, SEEK_SET);
    size_t bytes_read = fread(buffer, 1, size, f);
    fclose(f);
    if (bytes_read != size) {
        printf("ERROR: Failed to read configuration file \"%s\".\n", fname);
        return 0;
    }
    return 1;
}

/*! \brief Loads init_script 1 Mb buffer to chain together all of the
 *         strings in the init_script string table.
 *
 * \todo could reuse buffer safely between script loads.
 */
int
load_script(std::string fname)
{
    auto config = toml::parse(fname, toml::spec::v(1,1,0));
    for (size_t i=0; i<variables.size(); i++) {
        if (config.contains(variables[i])) {
            StrList a = toml::find<StrList>(config, variables[i]);
            /* TODO: Check for failure here . */
            state[variables[i]] = a;
        }
    }

    return 0;
}

/* . */
std::string
get_s(std::string key)
{
    auto result = state.find(key);
    if (result == state.end()) {
        std::string s;
        debug_message("ERROR: Failed to find %s.", key.c_str());
        return s;
    }
    return result->second.s;
}

/* . */
std::vector<std::string>
get_sl(std::string key)
{
    auto result = state.find(key);
    if (result == state.end()) {
        StrList empty;
        debug_message("ERROR: Failed to find %s.", key.c_str());
        return empty;
    }
    return result->second.sl;
}

/*! \brief Load our basic tree structure of data for all of our data structures
 *         above.
 *
 * EXAMPLE
 * We want access to the `os` string, it is a static in this file.
 * The command would be `get_env_var(global, "os").s`.
 *
 * To access something deeper in the tree we use the dot operator, which
 * allows searches to run faster by encapsulating data into structures.
 * Like "file_menu.4" to give the 4th element in the file_menu array.
 *
 * For accessing data at speed we can keep the parent node pointer available,
 * so for example:
 *
 *     ScriptValue *file_menu = get_var(root, "file_menu");
 *     for (int i=0; i<file_menu->n_leaves; i++) {
 *         printf("%s\n", file_menu->leaves[i].s);
 *     }
 *
 * To get the length of an array we .
 *
 * Rather than loading necessary configuration data from file at load, it is
 * compiled into the program. However, the ability to change the UI as a
 * user, without re-compiling the program, can be preserved by overriding the string
 * tables from a custom "overrides.ini" file in the configuration directory.
 */
int
load_global_state(char *asset_dir)
{
    std::string assets(asset_dir);

    for (size_t i=0; i<manifest.size(); i++) {
        std::string fname = "assets/" + manifest[i];
        std::cout << "Loading \"" << fname << "\"..." << std::endl;
        int error = load_script(fname);
        if (error) {
            return error;
        }
    }
    return 0;
}

