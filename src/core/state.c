/*
 * TODO: Convert this to C.
 */

#include "core.h"

void
state_create(char *settings_dir, char *app_dir)
{
    state.settings_dir = sdsnew(settings_dir);
    state.settings_path = sdsnew(settings_dir);
    state.settings_path = sdscat(state.settings_path, "settings.ini");
    state.app_dir = sdsnew(app_dir);
    state.manifest = sdsarray_create();
    state.tips = sdsarray_create();
    state.arguments = sdsarray_create();

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
    sdsarray_free(state.manifest);
    sdsarray_free(state.tips);
    sdsarray_free(state.arguments);
}

