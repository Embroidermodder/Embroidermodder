//Command: Polyline

var global = {}; //Required
global.firstRun;
global.firstX;
global.firstY;
global.prevX;
global.prevY;
global.num;

//NOTE: main() is run every time the command is started.
//      Use it to reset variables so they are ready to go.
function main()
{
    initCommand();
    clearSelection();
    global.firstRun = true;
    global.firstX = NaN;
    global.firstY = NaN;
    global.prevX = NaN;
    global.prevY = NaN;
    global.num = 0;
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
        global.firstX = x;
        global.firstY = y;
        global.prevX = x;
        global.prevY = y;
        addRubber("POLYLINE");
        setRubberMode("POLYLINE");
        setRubberPoint("POLYLINE_POINT_0", global.firstX, global.firstY);
        appendPromptHistory();
        setPromptPrefix("Specify next point or [Undo]: ");
    }
    else
    {
        global.num++;
        setRubberPoint("POLYLINE_POINT_" + global.num.toString(), x, y);
        setRubberText("POLYLINE_NUM_POINTS", global.num.toString());
        spareRubber("POLYLINE");
        appendPromptHistory();
        global.prevX = x;
        global.prevY = y;
    }
}

//NOTE: context() is run when a context menu entry is chosen.
function context(str)
{
    todo("POLYLINE", "context()");
}

//NOTE: prompt() is run when Enter is pressed.
//      appendPromptHistory is automatically called before prompt()
//      is called so calling it is only needed for erroneous input.
//      Any text in the command prompt is sent as an uppercase string.
function prompt(str)
{
    if(global.firstRun)
    {
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
            global.firstX = Number(strList[0]);
            global.firstY = Number(strList[1]);
            global.prevX = global.firstX;
            global.prevY = global.firstY;
            addRubber("POLYLINE");
            setRubberMode("POLYLINE");
            setRubberPoint("POLYLINE_POINT_0", global.firstX, global.firstY);
            setPromptPrefix("Specify next point or [Undo]: ");
        }
    }
    else
    {
        if(str == "U" || str == "UNDO")
        {
            todo("POLYLINE", "prompt() for UNDO");
        }
        else
        {
            var strList = str.split(",");
            if(isNaN(strList[0]) || isNaN(strList[1]))
            {
                setPromptPrefix("Point or option keyword required.");
                appendPromptHistory();
                setPromptPrefix("Specify next point or [Undo]: ");
            }
            else
            {
                var x = Number(strList[0]);
                var y = Number(strList[1]);
                global.num++;
                setRubberPoint("POLYLINE_POINT_" + global.num.toString(), x, y);
                setRubberText("POLYLINE_NUM_POINTS", global.num.toString());
                spareRubber("POLYLINE");
                global.prevX = x;
                global.prevY = y;
                setPromptPrefix("Specify next point or [Undo]: ");
            }
        }
    }
}
