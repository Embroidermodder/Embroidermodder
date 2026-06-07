/* Command: icon24 */

#include "commands.h"

int icon24_cmd(State *state)
{
    /* NOTE: main() is run every time the command is started.
     *       Use it to reset variables so they are ready to go.
     */
    if (state->context == CONTEXT_MAIN) {
        init_command();
        clear_selection();
    }
    icon_resize(24);
    end_command();
    return 0;
}

