/* Command: zoomrealtime */

#include "commands.h"

int zoom_real_time_cmd(State *state)
{
    /* NOTE: main() is run every time the command is started.
     *       Use it to reset variables so they are ready to go.
     */
    if (state->context == CONTEXT_MAIN) {
        init_command();
        clear_selection();
    }
    zoom_real_time();
    end_command();
    return 0;
}

