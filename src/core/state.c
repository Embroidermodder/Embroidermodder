/*
 * Embroidermodder 2 -- State Management
 * Copyright 2011-2026 The Embroidermodder Team
 */

#include "core.h"

State state;

void state_create(char *settings_dir, char *app_dir)
{
    state.settings_dir = str_create(settings_dir);
    state.settings_path = str_create(settings_dir);
    str_concat(state.settings_path, "settings.ini");
    state.app_dir = str_create(app_dir);

    state.shift = false;
    state.num_docs = 0;
    state.doc_index = 0;

    /* Command */
    state.command_active = false;
    state.rapid_fire = false;
    state.is_blinking = false;
    state.blink_state = false;
    state.command_line = str_create("");
    state.prefix = str_create("> ");
    state.current_command = str_create("donothing");
    state.last_command = str_create("donothing");
    state.arguments = strarray_create();

    state.manifest = strarray_create();
    state.tips = strarray_create();
    state.aliases = strarray_create();

    state.file_menu = strarray_create();
    state.edit_menu = strarray_create();
    state.view_menu = strarray_create();
    state.window_menu = strarray_create();
    state.help_menu = strarray_create();
    state.recent_menu = strarray_create();
    state.zoom_menu = strarray_create();
    state.pan_menu = strarray_create();
    state.draw_menu = strarray_create();
    state.tools_menu = strarray_create();
    state.dimension_menu = strarray_create();
    state.modify_menu = strarray_create();

    state.file_toolbar = strarray_create();
    state.edit_toolbar = strarray_create();
    state.view_toolbar = strarray_create();
    state.zoom_toolbar = strarray_create();
    state.pan_toolbar = strarray_create();
    state.icon_toolbar = strarray_create();
    state.help_toolbar = strarray_create();
    state.layer_toolbar = strarray_create();
    state.text_toolbar = strarray_create();
    state.properties_toolbar = strarray_create();
    state.prompt_toolbar = strarray_create();
    state.draw_toolbar = strarray_create();
    state.dimension_toolbar = strarray_create();
    state.inquiry_toolbar = strarray_create();
    state.modify_toolbar = strarray_create();

    /* Note that these are created after state.settings_dir so the defaults are corrent. */
    settings_create(&state.settings);
    settings_create(&state.accept);
    settings_create(&state.preview);
    settings_create(&state.dialog);
}

int load_menu(Node *config, const char *key, StrArray *array)
{
    if (!strarray_from_tree(config, key, array)) {
        printf("ERROR: failed to load %s.", key);
        return 0;
    }
    return 1;
}

/* The main loader for all of the configuration.
 */
int state_load(void)
{
    Node *config = load_xml("config.xml");
    if (config == NULL) {
        printf("ERROR: failed to load XML.\n");
        return 0;
    }

    if (!strarray_from_tree(config, "config.tips", state.tips)) {
        printf("ERROR: failed to parse strarray from tree.\n");
        return 0;
    }

    if (!load_menu(config, "config.filemenu", state.file_menu)) {
        return 0;
    }
    if (!load_menu(config, "config.editmenu", state.edit_menu)) {
        return 0;
    }
    if (!load_menu(config, "config.viewmenu", state.view_menu)) {
        return 0;
    }
    if (!load_menu(config, "config.helpmenu", state.help_menu)) {
        return 0;
    }
    if (!load_menu(config, "config.zoommenu", state.zoom_menu)) {
        return 0;
    }
    if (!load_menu(config, "config.panmenu", state.pan_menu)) {
        return 0;
    }
    if (!load_menu(config, "config.drawmenu", state.draw_menu)) {
        return 0;
    }
    if (!load_menu(config, "config.toolsmenu", state.tools_menu)) {
        return 0;
    }
    if (!load_menu(config, "config.dimensionmenu", state.dimension_menu)) {
        return 0;
    }
    if (!load_menu(config, "config.modifymenu", state.modify_menu)) {
        return 0;
    }

    if (!load_menu(config, "config.filetoolbar", state.file_toolbar)) {
        return 0;
    }
    if (!load_menu(config, "config.edittoolbar", state.edit_toolbar)) {
        return 0;
    }
    if (!load_menu(config, "config.viewtoolbar", state.view_toolbar)) {
        return 0;
    }
    if (!load_menu(config, "config.zoomtoolbar", state.zoom_toolbar)) {
        return 0;
    }
    if (!load_menu(config, "config.pantoolbar", state.pan_toolbar)) {
        return 0;
    }
    if (!load_menu(config, "config.icontoolbar", state.icon_toolbar)) {
        return 0;
    }
    if (!load_menu(config, "config.helptoolbar", state.help_toolbar)) {
        return 0;
    }
    if (!load_menu(config, "config.drawtoolbar", state.draw_toolbar)) {
        return 0;
    }
    if (!load_menu(config, "config.dimensiontoolbar", state.dimension_toolbar)) {
        return 0;
    }
    if (!load_menu(config, "config.inquirytoolbar", state.inquiry_toolbar)) {
        return 0;
    }
    if (!load_menu(config, "config.modifytoolbar", state.modify_toolbar)) {
        return 0;
    }

    return 1;
}

void state_free(void)
{
    settings_free(&state.settings);
    settings_free(&state.accept);
    settings_free(&state.preview);
    settings_free(&state.dialog);

    str_free(state.settings_dir);
    str_free(state.settings_path);
    str_free(state.app_dir);

    str_free(state.command_line);
    str_free(state.prefix);
    str_free(state.current_command);
    str_free(state.last_command);
    strarray_free(state.arguments);

    strarray_free(state.manifest);
    strarray_free(state.tips);
    strarray_free(state.aliases);

    strarray_free(state.file_menu);
    strarray_free(state.edit_menu);
    strarray_free(state.view_menu);
    strarray_free(state.window_menu);
    strarray_free(state.help_menu);
    strarray_free(state.recent_menu);
    strarray_free(state.zoom_menu);
    strarray_free(state.pan_menu);
    strarray_free(state.draw_menu);
    strarray_free(state.tools_menu);
    strarray_free(state.dimension_menu);
    strarray_free(state.modify_menu);

    strarray_free(state.file_toolbar);
    strarray_free(state.edit_toolbar);
    strarray_free(state.view_toolbar);
    strarray_free(state.zoom_toolbar);
    strarray_free(state.pan_toolbar);
    strarray_free(state.icon_toolbar);
    strarray_free(state.help_toolbar);
    strarray_free(state.layer_toolbar);
    strarray_free(state.text_toolbar);
    strarray_free(state.properties_toolbar);
    strarray_free(state.prompt_toolbar);
    strarray_free(state.draw_toolbar);
    strarray_free(state.dimension_toolbar);
    strarray_free(state.inquiry_toolbar);
    strarray_free(state.modify_toolbar);
}

