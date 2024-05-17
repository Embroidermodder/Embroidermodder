/*
 * Embroidermodder 2.
 *
 * Copyright 2013-2024 The Embroidermodder Team
 * Embroidermodder 2 is Open Source Software, see LICENSE for licensing terms.
 * Read CODE-STANDARDS.txt for advice on altering this file.
 *
 * Command: icon24
 */

#include "../commands.h"

/* NOTE: main() is run every time the command is started.
 *       Use it to reset variables so they are ready to go.
 */
ScriptValue
locatepoint_main(ScriptEnv * context)
{
    return script_null;
}

/* NOTE: click() is run only for left clicks.
 *       Middle clicks are used for panning.
 *       Right clicks bring up the context menu.
 */
ScriptValue
locatepoint_click(ScriptEnv * context)
{
    return script_null;
}

/* NOTE: context() is run when a context menu entry is chosen.
 */
ScriptValue
locatepoint_context(ScriptEnv * context)
{
    return script_null;
}

/* NOTE: prompt() is run when Enter is pressed.
 *       appendPromptHistory is automatically called before prompt()
 *       is called so calling it is only needed for erroneous input.
 *       Any text is in the command prompt is sent as an uppercase string.
 */
ScriptValue
locatepoint_prompt(ScriptEnv * context)
{
    return script_null;
}

Command locatepoint_cmd = {
    .id = -1,
    .main = locatepoint_main,
    .click = locatepoint_click,
    .context = locatepoint_context,
    .prompt = locatepoint_prompt,
    .icon = "about",
    .menu_name = "Tools",
    .menu_position = 11,
    .toolbar_name = "Inquiry",
    .toolbar_position = 0,
    .tooltip = "&Locate Point",
    .statustip = "Displays the coordinate values of a location:  ID",
    .alias = "ID, LOCATEPOINT"
};

#if 0
//Command: Locate Point

//NOTE: main() is run every time the command is started.
//      Use it to reset variables so they are ready to go.
function main()
{
    _main->nativeInitCommand();
    _main->nativeClearSelection();
    setPromptPrefix(qsTr("Specify point: "));
}

//NOTE: click() is run only for left clicks.
//      Middle clicks are used for panning.
//      Right clicks bring up the context menu.
function click(x, y)
{
    appendPromptHistory();
    setPromptPrefix("X = " + x.toString() + ", Y = " + y.toString());
    appendPromptHistory();
    _main->nativeEndCommand();
}

//NOTE: context() is run when a context menu entry is chosen.
function context(str)
{
    todo("LOCATEPOINT", "context()");
}

//NOTE: prompt() is run when Enter is pressed.
//      appendPromptHistory is automatically called before prompt()
//      is called so calling it is only needed for erroneous input.
//      Any text in the command prompt is sent as an uppercase string.
function prompt(str)
{
    var strList = str.split(",");
    if(isNaN(strList[0]) || isNaN(strList[1]))
    {
        alert(qsTr("Invalid point."));
        setPromptPrefix(qsTr("Specify point: "));
    }
    else
    {
        appendPromptHistory();
        setPromptPrefix("X = " + strList[0].toString() + ", Y = " + strList[1].toString());
        appendPromptHistory();
        _main->nativeEndCommand();
    }
}
#endif
