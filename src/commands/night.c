/* Command: Night */

#include "commands.h"

int night_cmd(State *state)
{
    /* NOTE: main() is run every time the command is started.
     *       Use it to reset variables so they are ready to go.
     */
    if (state->context == CONTEXT_MAIN) {
        init_command();
        clear_selection();
    }
    night_vision();
    end_command();
    return 0;
}

