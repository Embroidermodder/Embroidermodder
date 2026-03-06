/*
 * Embroidermodder 2 -- State Management
 * Copyright 2011-2026 The Embroidermodder Team
 */

#include "core.h"

void
state_create(char *settings_dir, char *app_dir)
{
    state.settings_dir = sdsnew(settings_dir);
    state.settings_path = sdsnew(settings_dir);
    state.settings_path = sdscat(state.settings_path, "settings.ini");
    state.app_dir = sdsnew(app_dir);

    state.shift = false;
    state.num_docs = 0;
    state.doc_index = 0;

    /* Command */
    state.command_active = false;
    state.rapid_fire = false;
    state.is_blinking = false;
    state.blink_state = false;
    state.command_line = sdsnew("");
    state.prefix = sdsnew("> ");
    state.current_command = sdsnew("donothing");
    state.last_command = sdsnew("donothing");
    state.arguments = sdsarray_create();

    state.manifest = sdsarray_create();
    state.tips = sdsarray_create();
    state.aliases = sdsarray_create();

    state.file_menu = sdsarray_create();
    state.edit_menu = sdsarray_create();
    state.view_menu = sdsarray_create();
    state.window_menu = sdsarray_create();
    state.help_menu = sdsarray_create();
    state.recent_menu = sdsarray_create();
    state.zoom_menu = sdsarray_create();
    state.pan_menu = sdsarray_create();

    state.file_toolbar = sdsarray_create();
    state.edit_toolbar = sdsarray_create();
    state.view_toolbar = sdsarray_create();
    state.zoom_toolbar = sdsarray_create();
    state.pan_toolbar = sdsarray_create();
    state.icon_toolbar = sdsarray_create();
    state.help_toolbar = sdsarray_create();
    state.layer_toolbar = sdsarray_create();
    state.text_toolbar = sdsarray_create();
    state.properties_toolbar = sdsarray_create();
    state.prompt_toolbar = sdsarray_create();

    /* Note that these are created after state.settings_dir so the defaults are corrent. */
    settings_create(&state.settings);
    settings_create(&state.accept);
    settings_create(&state.preview);
    settings_create(&state.dialog);
}

void
state_free(void)
{
    settings_free(&state.settings);
    settings_free(&state.accept);
    settings_free(&state.preview);
    settings_free(&state.dialog);

    sdsfree(state.settings_dir);
    sdsfree(state.settings_path);
    sdsfree(state.app_dir);

    sdsfree(state.command_line);
    sdsfree(state.prefix);
    sdsfree(state.current_command);
    sdsfree(state.last_command);
    sdsarray_free(state.arguments);

    sdsarray_free(state.manifest);
    sdsarray_free(state.tips);
    sdsarray_free(state.aliases);

    sdsarray_free(state.file_menu);
    sdsarray_free(state.edit_menu);
    sdsarray_free(state.view_menu);
    sdsarray_free(state.window_menu);
    sdsarray_free(state.help_menu);
    sdsarray_free(state.recent_menu);
    sdsarray_free(state.zoom_menu);
    sdsarray_free(state.pan_menu);

    sdsarray_free(state.file_toolbar);
    sdsarray_free(state.edit_toolbar);
    sdsarray_free(state.view_toolbar);
    sdsarray_free(state.zoom_toolbar);
    sdsarray_free(state.pan_toolbar);
    sdsarray_free(state.icon_toolbar);
    sdsarray_free(state.help_toolbar);
    sdsarray_free(state.layer_toolbar);
    sdsarray_free(state.text_toolbar);
    sdsarray_free(state.properties_toolbar);
    sdsarray_free(state.prompt_toolbar);
}

