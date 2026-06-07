//Command: platform

#include "commands.h"

int platform_cmd(State *state)
{
    /* NOTE: main() is run every time the command is started.
     *       Use it to reset variables so they are ready to go.
     */
    if (state->context == CONTEXT_MAIN) {
        init_command();
        clear_selection();
    }
    /* FIXME:
    setPromptPrefix(qsTr("Platform") + " = " + platformString());
    appendPromptHistory();
    */
    end_command();
    return 0;
}

