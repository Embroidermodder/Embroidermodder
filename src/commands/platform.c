/*
 * Embroidermodder 2
 * Copyright 2011-2025 The Embroidermodder Team
 *
 * Embroidermodder 2 is free and open software under the zlib license:
 * see LICENSE.md for details.
 *
 * ----------------------------------------------------------------------------
 *
 * Command to report the platform the program is running on.
 */

#include "../core.h"

int
platform_command(State *state)
{
    return 0;
}

#if 0
-- Command: Platform

function platform_command ()
  -- NOTE: main() is run every time the command is started.
  --       Use it to reset variables so they are ready to go.
  if context == CONTEXT_MAIN then
    init_command()
    clear_selection()
  end
  setPromptPrefix(qsTr("Platform") + " = " + platformString())
  appendPromptHistory()
  end_command()
end

#endif

