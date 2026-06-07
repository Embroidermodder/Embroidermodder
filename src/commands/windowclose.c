/* Command: windowclose */

#include "commands.h"

int window_close_cmd(State *state)
{
    /* NOTE: main() is run every time the command is started.
     *       Use it to reset variables so they are ready to go.
     */
    if (state->context == CONTEXT_MAIN) {
        init_command();
        clear_selection();
    }
    window_close();
    end_command();
    return 0;
}

