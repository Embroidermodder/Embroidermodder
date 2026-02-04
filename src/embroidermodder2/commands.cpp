/*
 * Embroidermodder 2 -- Commands
 * Copyright 2011-2026 The Embroidermodder Team
 */

#include "commands.h"

int
about_cmd(State *state)
{
    state->mainWin->about();
    return 0;
}

int
do_nothing_cmd(State *state)
{
    return 0;
}

