/* Command: windowcloseall */

#include "commands.h"

int window_close_all_cmd(State *state)
{
    /* NOTE: main() is run every time the command is started.
     *       Use it to reset variables so they are ready to go.
     */
    if (state->context == CONTEXT_MAIN) {
        init_command();
        clear_selection();
    }
    window_close_all();
    end_command();
    return 0;
}

