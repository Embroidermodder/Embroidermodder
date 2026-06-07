/* Command: moveselected */

#include "commands.h"

int move_selected_cmd(State *state)
{
    /* NOTE: main() is run every time the command is started.
     *       Use it to reset variables so they are ready to go.
     */
    if (state->context == CONTEXT_MAIN) {
        init_command();
    }
/* FIXME: argument parsing

    NAN_CHECK("moveSelected", "first", dx)
    NAN_CHECK("moveSelected", "second", dy)

    move_selected(dx, dy);
*/
    end_command();
    return 0;
}

