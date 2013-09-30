//Command: RGB

var global = {}; //Required
global.mode;

//enums
global.mode_BACKGROUND = 0;
global.mode_CROSSHAIR  = 1;
global.mode_GRID       = 2;

//NOTE: main() is run every time the command is started.
//      Use it to reset variables so they are ready to go.
function main()
{
    initCommand();
    clearSelection();
    global.mode = global.mode_BACKGROUND;
    setPromptPrefix("Enter RED,GREEN,BLUE values for background or [Crosshair/Grid]: ");
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
        if(str == "C" || str == "CROSSHAIR")
        {
            global.mode = global.mode_CROSSHAIR;
            setPromptPrefix("Specify crosshair color: ");
        }
        else if(str == "G" || str == "GRID")
        {
            global.mode = global.mode_GRID;
            setPromptPrefix("Specify grid color: ");
        }
        else
        {
            var strList = str.split(",");
            var r = Number(strList[0]);
            var g = Number(strList[1]);
            var b = Number(strList[2]);
            if(!validRGB(r,g,b))
            {
                setPromptPrefix("Invalid color. R,G,B values must be in the range of 0-255.");
                appendPromptHistory();
                setPromptPrefix("Specify background color: ");
            }
            else
            {
                setBackgroundColor(r,g,b);
                endCommand();
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
            setPromptPrefix("Invalid color. R,G,B values must be in the range of 0-255.");
            appendPromptHistory();
            setPromptPrefix("Specify crosshair color: ");
        }
        else
        {
            setCrossHairColor(r,g,b);
            endCommand();
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
            setPromptPrefix("Invalid color. R,G,B values must be in the range of 0-255.");
            appendPromptHistory();
            setPromptPrefix("Specify grid color: ");
        }
        else
        {
            setGridColor(r,g,b);
            endCommand();
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
}