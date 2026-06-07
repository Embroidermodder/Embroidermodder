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

    NAN_CHECK("scaleSelected", "first", x)
    NAN_CHECK("scaleSelected", "second", y)
    NAN_CHECK("scaleSelected", "third", factor)

    if (factor <= 0.0) {
        throwError(QJSValue::GenericError, "scaleSelected(): scale factor must be greater than zero");
        return 1;
    }

    scale_selected(x, y, factor);
*/
    end_command();
    return 0;
}

