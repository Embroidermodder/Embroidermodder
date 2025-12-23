/*
 * Embroidermodder 2
 * Copyright 2011-2025 The Embroidermodder Team
 *
 * Embroidermodder 2 is free and open software under the zlib license:
 * see LICENSE.md for details.
 *
 * ----------------------------------------------------------------------------
 *
 * Command that adds a point to the current pattern.
 */

#include "../core.h"

int point_main(State *state);
int point_click(State *state);
int point_context(State *state);
int point_prompt(State *state);

int
point_command(State *state)
{
    int result = 0;
    switch (state->context) {
    case CONTEXT_CLICK:
        result = point_click(state);
        break;
    case CONTEXT_MENU:
        result = point_context(state);
        break;
    case CONTEXT_PROMPT:
        result = point_prompt(state);
        break;
    case CONTEXT_MAIN:
    default:
        result = point_main(state);
        break;
    }
    return result;
}

/*
 * NOTE: point_main() is run every time the command is started.
 *       Use it to reset variables so they are ready to go.
 */
int
point_main(State *state)
{
    // FIXME: init_command_c();
    clear_selection_c();
    state->first_run = true;
    prompt_output("TODO: Current point settings: PDMODE=?  PDSIZE=?");
    /* TODO: translate needed here when complete */
    prompt_output(translate("Specify first point: "));
    return 0;
}

/*
 * NOTE: point_click() is run only for left clicks.
 *       Middle clicks are used for panning.
 *       Right clicks bring up the context menu.
 */
int
point_click(State *state)
{
    if (state->first_run) {
        state->first_run = false;
        prompt_output(translate("Specify next point: "));
        point_create(state->mouse_position);
    }
    else {
        point_create(state->mouse_position);
    }
    return 0;
}

/*
 * NOTE: point_context() is run when a context menu entry is chosen.
 */
int
point_context(State *state)
{
    debug("TODO: POINT context()");
    return 0;
}

/*
 * NOTE: point_prompt() is run when Enter is pressed.
 *       appendPromptHistory is automatically called before prompt()
 *       is called so calling it is only needed for erroneous input.
 *       Any text in the command prompt is sent as an uppercase string.
 */
int
point_prompt(State *state)
{
    if (state->first_run) {
        /*
        if (str == "M" || (!strcmp(str, "MODE")) {
            debug("TODO: POINT prompt() for PDMODE");
        }
        else if (str == "S" || (!strcmp(str, "SIZE")) {
            debug("TODO: POINT prompt() for PDSIZE");
        }
        */
        if (state->argument_count < 2) {
            //alert(translate("Invalid point."));
            prompt_output(translate("Specify first point: "));
        }
        else {
            state->first_run = false;
            EmbVector position = get_vector(state, 0);
            prompt_output(translate("Specify next point: "));
            point_create(position);
        }
    }
    else {
        if (state->argument_count < 2) {
            //alert(translate("Invalid point."));
            prompt_output(translate("Specify next point: "));
        }
        else {
            EmbVector position = get_vector(state, 0);
            prompt_output(translate("Specify next point: "));
            point_create(position);
        }
    }
    return 0;
}

