/* Command: SelectAll */

#include "commands.h"

int select_all_cmd(State *state)
{
    /* NOTE: main() is run every time the command is started.
     *       Use it to reset variables so they are ready to go.
     */
    if (state->context == CONTEXT_MAIN) {
        init_command();
    }
    select_all();
    end_command();
    return 0;
}

