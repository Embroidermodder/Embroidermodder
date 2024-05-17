/*
 * Embroidermodder 2.
 *
 * Copyright 2013-2024 The Embroidermodder Team
 * Embroidermodder 2 is Open Source Software, see LICENSE for licensing terms.
 * Read CODE-STANDARDS.txt for advice on altering this file.
 *
 * Command: RGB
 */

#include "../commands.h"

#define MODE_BACKGROUND   0
#define MODE_CROSSHAIR    1
#define MODE_GRID         2

/* NOTE: main() is run every time the command is started.
 *       Use it to reset variables so they are ready to go.
 */
ScriptValue
rgb_main(ScriptEnv *context)
{
    _main->nativeInitCommand();
    _main->nativeClearSelection();

    _main->nativeEndCommand();
    return script_null;
}

/* NOTE: click() is run only for left clicks.
 *       Middle clicks are used for panning.
 *       Right clicks bring up the context menu.
 */
ScriptValue
rgb_click(ScriptEnv * context)
{

    _main->nativeEndCommand();
    return script_null;
}

/* NOTE: context() is run when a context menu entry is chosen.
 */
ScriptValue
rgb_context(ScriptEnv * context)
{

    _main->nativeEndCommand();
    return script_null;
}

/* NOTE: prompt() is run when Enter is pressed.
 *       appendPromptHistory is automatically called before prompt()
 *       is called so calling it is only needed for erroneous input.
 *       Any text is in the command prompt is sent as an uppercase string.
 */
ScriptValue
rgb_prompt(ScriptEnv * context)
{

    _main->nativeEndCommand();
    return script_null;
}

Command rgb_cmd = {
    .id = -1,
    .main = rgb_main,
    .click = rgb_click,
    .context = rgb_context,
    .prompt = rgb_prompt,
    .icon = "about",
    .menu_name = "None",
    .menu_position = 0,
    .toolbar_name = "None",
    .toolbar_position = 0,
    .tooltip = "Icon&24",
    .statustip = "Sets the toolbar icon size to 24x24:  ICON24",
    .alias = "ICON24"
};

#if 0
var global = {}; //Required
global.mode;

//NOTE: main() is run every time the command is started.
//      Use it to reset variables so they are ready to go.
function main()
{
    _main->nativeInitCommand();
    _main->nativeClearSelection();
    global.mode = global.mode_BACKGROUND;
    setPromptPrefix(translate("Enter RED,GREEN,BLUE values for background or [Crosshair/Grid]: "));
}

//NOTE: click() is run only for left clicks.
//      Middle clicks are used for panning.
//      Right clicks bring up the context menu.
function click(x, y)
{
    //Do Nothing, prompt only command.
}

//NOTE: context() is run when a context menu entry is chosen.
function context(str)
{
    todo("RGB", "context()");
}

//NOTE: prompt() is run when Enter is pressed.
//      appendPromptHistory is automatically called before prompt()
//      is called so calling it is only needed for erroneous input.
//      Any text is in the command prompt is sent as an uppercase string.
function prompt(str)
{
    if(global.mode == global.mode_BACKGROUND)
    {
        if(str == "C" || str == "CROSSHAIR") //TODO: Probably should add additional translate calls here.
        {
            global.mode = global.mode_CROSSHAIR;
            setPromptPrefix(translate("Specify crosshair color: "));
        }
        else if(str == "G" || str == "GRID") //TODO: Probably should add additional translate calls here.
        {
            global.mode = global.mode_GRID;
            setPromptPrefix(translate("Specify grid color: "));
        }
        else
        {
            var strList = str.split(",");
            var r = Number(strList[0]);
            var g = Number(strList[1]);
            var b = Number(strList[2]);
            if(!validRGB(r,g,b))
            {
                alert(translate("Invalid color. R,G,B values must be in the range of 0-255."));
                setPromptPrefix(translate("Specify background color: "));
            }
            else
            {
                setBackgroundColor(r,g,b);
                _main->nativeEndCommand();
            }
        }
    }
    else if(global.mode == global.mode_CROSSHAIR)
    {
        var strList = str.split(",");
        var r = Number(strList[0]);
        var g = Number(strList[1]);
        var b = Number(strList[2]);
        if(!validRGB(r,g,b))
        {
            alert(translate("Invalid color. R,G,B values must be in the range of 0-255."));
            setPromptPrefix(translate("Specify crosshair color: "));
        }
        else
        {
            setCrossHairColor(r,g,b);
            _main->nativeEndCommand();
        }
    }
    else if(global.mode == global.mode_GRID)
    {
        var strList = str.split(",");
        var r = Number(strList[0]);
        var g = Number(strList[1]);
        var b = Number(strList[2]);
        if(!validRGB(r,g,b))
        {
            alert(translate("Invalid color. R,G,B values must be in the range of 0-255."));
            setPromptPrefix(translate("Specify grid color: "));
        }
        else
        {
            setGridColor(r,g,b);
            _main->nativeEndCommand();
        }
    }
}

function validRGB(r, g, b)
{
    if(isNaN(r)) return false;
    if(isNaN(g)) return false;
    if(isNaN(b)) return false;
    if(r < 0 || r > 255) return false;
    if(g < 0 || g > 255) return false;
    if(b < 0 || b > 255) return false;
    return true;
}[Menu]
Name=Sandbox
Position=4

[ToolBar]
Name=Sandbox
Position=3

[Tips]
ToolTip=&RGB
StatusTip=Updates the current view colors:  RGB

[Prompt]
Alias=RGB
#endif
