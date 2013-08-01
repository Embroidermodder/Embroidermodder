//Command: Point

var global = {}; //Required
global.firstRun;

//NOTE: main() is run every time the command is started.
//      Use it to reset variables so they are ready to go.
function main()
{
    initCommand();
    global.firstRun = true;
    setPromptPrefix("TODO: Current point settings: PDMODE=?  PDSIZE=?");
    appendPromptHistory();
    setPromptPrefix("Specify first point: ");
}

//NOTE: click() is run only for left clicks.
//      Middle clicks are used for panning.
//      Right clicks bring up the context menu.
function click(x, y)
{
    if(global.firstRun)
    {
        global.firstRun = false;
        appendPromptHistory();
        setPromptPrefix("Specify next point: ");
        addPoint(x,y);
    }
    else
    {
        appendPromptHistory();
        addPoint(x,y);
    }
}

//NOTE: context() is run when a context menu entry is chosen.
function context(str)
{
    todo("POINT", "context()");
}

//NOTE: prompt() is run when Enter is pressed.
//      appendPromptHistory is automatically called before prompt()
//      is called so calling it is only needed for erroneous input.
//      Any text in the command prompt is sent as an uppercase string.
function prompt(str)
{
    if(global.firstRun)
    {
        if(str == "M" || str == "MODE")
        {
            todo("POINT", "prompt() for PDMODE");
        }
        else if(str == "S" || str == "SIZE")
        {
            todo("POINT", "prompt() for PDSIZE");
        }
        var strList = str.split(",");
        if(isNaN(strList[0]) || isNaN(strList[1]))
        {
            setPromptPrefix("Invalid point.");
            appendPromptHistory();
            setPromptPrefix("Specify first point: ");
        }
        else
        {
            global.firstRun = false;
            var x = Number(strList[0]);
            var y = Number(strList[1]);
            setPromptPrefix("Specify next point: ");
            addPoint(x,y);
        }
    }
    else
    {
        var strList = str.split(",");
        if(isNaN(strList[0]) || isNaN(strList[1]))
        {
            setPromptPrefix("Invalid point.");
            appendPromptHistory();
            setPromptPrefix("Specify next point: ");
        }
        else
        {
            var x = Number(strList[0]);
            var y = Number(strList[1]);
            setPromptPrefix("Specify next point: ");
            addPoint(x,y);
        }
     
    }
}
