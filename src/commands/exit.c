//Command: Exit

#include "commands.h"

int exit_cmd(State *state)
{
    if (state->context == CONTEXT_MAIN) {
        init_command();
        clear_selection();
    }
    exit_program();
    end_command();
    return 0;
}

