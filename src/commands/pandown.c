/* Command: pandown */

#include "commands.h"

int pan_down_cmd(State *state)
{
    /* NOTE: main() is run every time the command is started.
     *       Use it to reset variables so they are ready to go.
     */
    if (state->context == CONTEXT_MAIN) {
        init_command();
    }
    pan_down();
    end_command();
    return 0;
}

