/* Command: zoomin */

#include "commands.h"

int zoom_in_cmd(State *state)
{
    /* NOTE: main() is run every time the command is started.
     *       Use it to reset variables so they are ready to go.
     */
    if (state->context == CONTEXT_MAIN) {
        init_command();
        clear_selection();
    }
    zoom_in();
    end_command();
    return 0;
}

