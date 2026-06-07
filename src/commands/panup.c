/* Command: panup */

#include "commands.h"

int pan_up_cmd(State *state)
{
    /* NOTE: main() is run every time the command is started.
     *       Use it to reset variables so they are ready to go.
     */
    if (state->context == CONTEXT_MAIN) {
        init_command();
    }
    pan_up();
    end_command();
    return 0;
}

