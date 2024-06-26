/*
 * Embroidermodder 2.
 *
 * Copyright 2011-2024 The Embroidermodder Team
 * Embroidermodder 2 is Open Source Software, see LICENSE.md for licensing terms.
 * Visit https://www.libembroidery.org/refman for advice on altering this file,
 * or read the markdown version in embroidermodder2/docs/refman.
 *
 * View Commands
 */

#include "embroidermodder.h"

/* DAY is not context-dependant. */
ScriptValue
day_command(ScriptEnv *context)
{
    if (!argument_checks(context, "day_command", "")) {
        return script_false;
    }
    init_command();
    clear_selection();
    _main->dayVision();
    end_command();
    return script_null;
}

/* NIGHT is not context-sensitive. */
ScriptValue
night_command(ScriptEnv * context)
{
    if (!argument_checks(context, "night_command", "")) {
        return script_false;
    }
    init_command();
    clear_selection();
    _main->nightVision();
    end_command();
    return script_null;
}

/* PANPOINT. */
ScriptValue
panpoint_command(ScriptEnv *context)
{
    if (!argument_checks(context, "panpoint_command", "")) {
        return script_false;
    }
    init_command();

    end_command();
    return script_null;
}

/* PANREALTIME. */
ScriptValue
panrealtime_command(ScriptEnv *context)
{
    if (!argument_checks(context, "panrealtime_command", "")) {
        return script_false;
    }
    init_command();

    end_command();
    return script_null;
}

/* PANDOWN is context-independant. */
ScriptValue
pandown_command(ScriptEnv *context)
{
    if (!argument_checks(context, "pandown_command", "")) {
        return script_false;
    }
    init_command();
    _main->panDown();
    end_command();
    return script_null;
}

/* PANLEFT */
ScriptValue
panleft_command(ScriptEnv *context)
{
    if (!argument_checks(context, "panleft_command", "")) {
        return script_false;
    }
    init_command();
    _main->panLeft();
    end_command();
    return script_null;
}

/* PANRIGHT */
ScriptValue
panright_command(ScriptEnv *context)
{
    if (!argument_checks(context, "panright_command", "")) {
        return script_false;
    }
    init_command();
    _main->panRight();
    end_command();
    return script_null;
}

/* PANUP */
ScriptValue
panup_command(ScriptEnv *context)
{
    _main->debug_message("panUp()");
    if (!argument_checks(context, "panup_command", "")) {
        return script_false;
    }
    init_command();
    View* gview = activeView();
    QUndoStack* stack = gview->getUndoStack();
    if (gview && stack) {
        UndoableNavCommand* cmd = new UndoableNavCommand("PanUp", gview, 0);
        stack->push(cmd);
    }
    end_command();
    return script_null;
}

/* ZOOMALL is not context-dependant */
ScriptValue
zoom_all_command(ScriptEnv *context)
{
    if (!argument_checks(context, "zoomextents_command", "")) {
        return script_false;
    }
    init_command();
    clear_selection();
    _main->zoomExtents();
    end_command();
    return script_null;
}

/* ZOOMDYNAMIC is not context-dependant */
ScriptValue
zoom_dynamic_command(ScriptEnv *context)
{
    if (!argument_checks(context, "zoomextents_command", "")) {
        return script_false;
    }
    init_command();
    _main->zoomExtents();
    end_command();
    return script_null;
}

/* ZOOMCENTER is not context-dependant */
ScriptValue
zoom_center_command(ScriptEnv *context)
{
    if (!argument_checks(context, "zoomextents_command", "")) {
        return script_false;
    }
    init_command();
    _main->zoomExtents();
    end_command();
    return script_null;
}

/* ZOOMEXTENTS is not context-dependant */
ScriptValue
zoom_extents_command(ScriptEnv *context)
{
    if (!argument_checks(context, "zoomextents_command", "")) {
        return script_false;
    }
    init_command();
    _main->zoomExtents();
    end_command();
    return script_null;
}

/* ZOOMIN is not context-dependant */
ScriptValue
zoom_in_command(ScriptEnv *context)
{
    _main->debug_message("zoomIn()");
    if (!argument_checks(context, "zoom_in_command", "")) {
        return script_false;
    }
    init_command();

    View* gview = activeView();
    if (gview) {
        gview->zoomIn();
    }

    end_command();
    return script_null;
}

/* ZOOMPREVIOUS is not context-dependant */
ScriptValue
zoom_previous_command(ScriptEnv *context)
{
    if (!argument_checks(context, "zoomin_command", "")) {
        return script_false;
    }
    init_command();

    end_command();
    return script_null;
}

/* ZOOMREALTIME is not context-dependant */
ScriptValue
zoom_real_time_command(ScriptEnv *context)
{
    _main->debug_message("zoomRealtime()");
    if (!argument_checks(context, "zoom_real_time_command", "")) {
        return script_false;
    }
    init_command();
    _main->stub_implement("Implement zoomRealtime.");
    end_command();
    return script_null;
}

/* ZOOMOUT is not context-dependant */
ScriptValue
zoom_out_command(ScriptEnv *context)
{
    if (!argument_checks(context, "zoomout_command", "")) {
        return script_false;
    }
    init_command();

    View* gview = activeView();
    if (gview) {
        gview->zoomOut();
    }

    end_command();
    return script_null;
}

/* ZOOMSCALE is not context-dependant */
ScriptValue
zoom_scale_command(ScriptEnv *context)
{
    if (!argument_checks(context, "zoomin_command", "")) {
        return script_false;
    }
    init_command();

    end_command();
    return script_null;
}

/* ZOOMSELECTED is not context-dependant */
ScriptValue
zoom_selected_command(ScriptEnv *context)
{
    if (!argument_checks(context, "zoomin_command", "")) {
        return script_false;
    }
    init_command();

    end_command();
    return script_null;
}

/* ZOOMWINDOW is not context-dependant */
ScriptValue
zoom_window_command(ScriptEnv *context)
{
    if (!argument_checks(context, "zoomin_command", "")) {
        return script_false;
    }
    init_command();

    end_command();
    return script_null;
}
