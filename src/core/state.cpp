/*
 * TODO: Convert this to C.
 */

#include "state.h"

void
state_create(void)
{
    settings_create(&state.settings);
    settings_create(&state.accept);
    settings_create(&state.preview);
    settings_create(&state.dialog);
    state.manifest = sdsarray_create();
    state.tips = sdsarray_create();
    state.arguments = sdsarray_create();
}

void
state_free(void)
{
    settings_free(&state.settings);
    settings_free(&state.accept);
    settings_free(&state.preview);
    settings_free(&state.dialog);
    sdsarray_free(state.manifest);
    sdsarray_free(state.tips);
    sdsarray_free(state.arguments);
}

