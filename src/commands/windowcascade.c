/* Command: windowcascade */

#include "commands.h"

int window_cascade_cmd(State *state)
{
    /* NOTE: main() is run every time the command is started.
     *       Use it to reset variables so they are ready to go.
     */
    if (state->context == CONTEXT_MAIN) {
        init_command();
        clear_selection();
    }
    window_cascade();
    end_command();
    return 0;
}

