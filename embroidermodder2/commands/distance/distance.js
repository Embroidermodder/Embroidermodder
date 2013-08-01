//Command: Distance

var global = {}; //Required
global.x1;
global.y1;
global.x2;
global.y2;

//NOTE: main() is run every time the command is started.
//      Use it to reset variables so they are ready to go.
function main()
{
    initCommand();
    global.x1 = NaN;
    global.y1 = NaN;
    global.x2 = NaN;
    global.y2 = NaN;
    setPromptPrefix("Specify first point: ");
}

//NOTE: click() is run only for left clicks.
//      Middle clicks are used for panning.
//      Right clicks bring up the context menu.
function click(x, y)
{
    if(isNaN(global.x1))
    {
        global.x1 = x;
        global.y1 = y;
        appendPromptHistory();
        setPromptPrefix("Specify second point: ");
    }
    else
    {
        appendPromptHistory();
        global.x2 = x;
        global.y2 = y;
        reportDistance();
        endCommand();
    }
}

//NOTE: context() is run when a context menu entry is chosen.
function context(str)
{
    todo("DISTANCE", "context()");
}

//NOTE: prompt() is run when Enter is pressed.
//      appendPromptHistory is automatically called before prompt()
//      is called so calling it is only needed for erroneous input.
//      Any text in the command prompt is sent as an uppercase string.
function prompt(str)
{
    var strList = str.split(",");
    if(isNaN(global.x1))
    {
        if(isNaN(strList[0]) || isNaN(strList[1]))
        {
            setPromptPrefix("Requires numeric distance or two points.");
            appendPromptHistory();
            setPromptPrefix("Specify first point: ");
        }
        else
        {
            global.x1 = Number(strList[0]);
            global.y1 = Number(strList[1]);
            setPromptPrefix("Specify second point: ");
        }
    }
    else
    {
        if(isNaN(strList[0]) || isNaN(strList[1]))
        {
            setPromptPrefix("Requires numeric distance or two points.");
            appendPromptHistory();
            setPromptPrefix("Specify second point: ");
        }
        else
        {
            global.x2 = Number(strList[0]);
            global.y2 = Number(strList[1]);
            reportDistance();
            endCommand();
        }
    }
}

//Cartesian Coordinate System reported:
//
//               (+)
//               90
//               |
//      (-) 180__|__0 (+)
//               |
//              270
//              (-)

function reportDistance()
{
    var dx = global.x2 - global.x1;
    var dy = global.y2 - global.y1;

    var dist = calculateDistance(global.x1,global.y1,global.x2, global.y2);
    var angle = calculateAngle(global.x1,global.y1,global.x2, global.y2);

    setPromptPrefix("Distance = " + dist.toString() + ", Angle = " + angle.toString());
    appendPromptHistory();
    setPromptPrefix("Delta X = " + dx.toString() + ", Delta Y = " + dy.toString());
    appendPromptHistory();
}
