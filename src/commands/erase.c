//Command: Erase/Delete

#include "commands.h"

int erase_cmd(State *state)
{
    // FIXME
    //NOTE: main() is run every time the command is started.
    //      Use it to reset variables so they are ready to go.
#if 0
    if (state->context == CONTEXT_MAIN) {
        init_command();

        if (numSelected() <= 0) {
            //TODO: Prompt to select objects if nothing is preselected
            alert(qsTr("Preselect objects before invoking the delete command."));
            end_command();
            messageBox("information", qsTr("Delete Preselect"), qsTr("Preselect objects before invoking the delete command."));
            return 1;
        }
    }
#endif
    delete_selected();
    end_command();
    return 0;
}

