/* Command: panleft */

#include "commands.h"

int pan_left_cmd(State *state)
{
    /* NOTE: main() is run every time the command is started.
     *       Use it to reset variables so they are ready to go.
     */
    if (state->context == CONTEXT_MAIN) {
        init_command();
    }
    pan_left();
    end_command();
    return 0;
}

