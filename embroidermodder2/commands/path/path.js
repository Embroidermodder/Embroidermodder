//Command: Path

//TODO: The path command is currently broken

var global = {}; //Required
global.firstRun;
global.firstX;
global.firstY;
global.prevX;
global.prevY;

//NOTE: main() is run every time the command is started.
//      Use it to reset variables so they are ready to go.
function main()
{
    global.firstRun = true;
    global.firstX = NaN;
    global.firstY = NaN;
    global.prevX = NaN;
    global.prevY = NaN;
    setPromptPrefix("Specify start point: ");
}

//NOTE: click() is run only for left clicks.
//      Middle clicks are used for panning.
//      Right clicks bring up the context menu.
function click(x, y)
{
    if(global.firstRun)
    {
        global.firstRun = false;
        global.firstX = x;
        global.firstY = y;
        global.prevX = x;
        global.prevY = y;
        addPath(x,y);
        appendPromptHistory();
        setPromptPrefix("Specify next point or [Arc/Undo]: ");
    }
    else
    {
        appendPromptHistory();
        appendLineToPath(x,y);
        global.prevX = x;
        global.prevY = y;
    }
}

//NOTE: context() is run when a context menu entry is chosen.
function context(str)
{
    todo("PATH", "context()");
}

//NOTE: prompt() is run when Enter is pressed.
//      appendPromptHistory is automatically called before prompt()
//      is called so calling it is only needed for erroneous input.
//      Any text in the command prompt is sent as an uppercase string.
function prompt(str)
{
    if(str == "A" || str == "ARC")
    {
        todo("PATH", "prompt() for ARC");
    }
    else if(str == "U" || str == "UNDO")
    {
        todo("PATH", "prompt() for UNDO");
    }
    else
    {
        var strList = str.split(",");
        if(isNaN(strList[0]) || isNaN(strList[1]))
        {
            setPromptPrefix("Point or option keyword required.");
            appendPromptHistory();
            setPromptPrefix("Specify next point or [Arc/Undo]: ");
        }
        else
        {
            var x = Number(strList[0]);
            var y = Number(strList[1]);
            if(global.firstRun)
            {
                global.firstRun = false;
                global.firstX = x;
                global.firstY = y;
                global.prevX = x;
                global.prevY = y;
                addPath(x,y);
                setPromptPrefix("Specify next point or [Arc/Undo]: ");
            }
            else
            {
                appendLineToPath(x,y);
                global.prevX = x;
                global.prevY = y;
            }
        }
    }
}
