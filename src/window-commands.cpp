/*
 * Embroidermodder 2.
 *
 * Copyright 2011-2024 The Embroidermodder Team
 * Embroidermodder 2 is Open Source Software, see LICENSE.md for licensing terms.
 * Visit https://www.libembroidery.org/refman for advice on altering this file,
 * or read the markdown version in embroidermodder2/docs/refman.
 *
 * Window Commands
 */

#include "embroidermodder.h"

/* WINDOWCASCADE is not context-dependant. */
ScriptValue
windowcascade_command(ScriptEnv * context)
{
    if (!argument_checks(context, "windowcascade_command", "")) {
        return script_false;
    }
    init_command();
    clear_selection();
    _main->mdiArea->cascade();
    end_command();
    return script_null;
}

/* WINDOWCLOSE is not context-dependant. */
ScriptValue
windowclose_command(ScriptEnv * context)
{
    if (!argument_checks(context, "windowclose_command", "")) {
        return script_false;
    }
    init_command();
    clear_selection();
    _main->onCloseWindow();
    end_command();
    return script_null;
}

/* WINDOWCLOSEALL is not context-dependant. */
ScriptValue
windowcloseall_command(ScriptEnv * context)
{
    if (!argument_checks(context, "windowcloseall_command", "")) {
        return script_false;
    }
    init_command();
    clear_selection();
    _main->mdiArea->closeAllSubWindows();
    end_command();
    return script_null;
}

/* */
ScriptValue
windownext_command(ScriptEnv * context)
{
    if (!argument_checks(context, "windownext_command", "")) {
        return script_false;
    }
    init_command();
    clear_selection();
    _main->mdiArea->activateNextSubWindow();
    end_command();
    return script_null;
}

/* WINDOWPREVIOUS is not context-sensitive. */
ScriptValue
windowprevious_command(ScriptEnv * context)
{
    if (!argument_checks(context, "windowprevious_command", "")) {
        return script_false;
    }
    init_command();
    clear_selection();
    _main->mdiArea->activatePreviousSubWindow();
    end_command();
    return script_null;
}

/* WINDOWTILE is not context-dependant */
ScriptValue
windowtile_command(ScriptEnv *context)
{
    if (!argument_checks(context, "windowtile_command", "")) {
        return script_false;
    }
    init_command();
    clear_selection();
    _main->mdiArea->tile();
    end_command();
    return script_null;
}
