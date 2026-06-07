//Command: Open

#include "commands.h"

int open_cmd(State *state)
{
    //NOTE: main() is run every time the command is started.
    //      Use it to reset variables so they are ready to go.
    if (state->context == CONTEXT_MAIN) {
        init_command();
        clear_selection();
    }
    open_file();
    end_command();
    return 0;
}

