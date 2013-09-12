//Command: Rotate

var global = {}; //Required
global.firstRun;
global.baseX;
global.baseY;
global.destX;
global.destY;
global.angle;

global.baseRX;
global.baseRY;
global.destRX;
global.destRY;
global.angleRef;
global.angleNew;

global.mode;

//enums
global.mode_NORMAL    = 0;
global.mode_REFERENCE = 1;


//NOTE: main() is run every time the command is started.
//      Use it to reset variables so they are ready to go.
function main()
{
    initCommand();
    global.mode = global.mode_NORMAL;
    global.firstRun = true;
    global.baseX = NaN;
    global.baseY = NaN;
    global.destX = NaN;
    global.destY = NaN;
    global.angle = NaN;

    global.baseRX   = NaN;
    global.baseRY   = NaN;
    global.destRX   = NaN;
    global.destRY   = NaN;
    global.angleRef = NaN;
    global.angleNew = NaN;

    if(numSelected() <= 0)
    {
        //TODO: Prompt to select objects if nothing is preselected
        setPromptPrefix("Preselect objects before invoking the rotate command.");
        appendPromptHistory();
        endCommand();
        messageBox("information", "Rotate Preselect", "Preselect objects before invoking the rotate command.");
    }
    else
    {
        setPromptPrefix("Specify base point: ");
    }
}

//NOTE: click() is run only for left clicks.
//      Middle clicks are used for panning.
//      Right clicks bring up the context menu.
function click(x, y)
{
    if(global.mode == global.mode_NORMAL)
    {
        if(global.firstRun)
        {
            global.firstRun = false;
            global.baseX = x;
            global.baseY = y;
            addRubber("LINE");
            setRubberMode("LINE");
            setRubberPoint("LINE_START", global.baseX, global.baseY);
            appendPromptHistory();
            setPromptPrefix("Specify rotation angle or [Reference]: ");
        }
        else
        {
            global.destX = x;
            global.destY = y;
            global.angle = calculateAngle(global.baseX, global.baseY, global.destX, global.destY);
            rotateSelected(global.baseX, global.baseY, global.angle);
            endCommand();
        }
    }
    else if(global.mode == global.mode_REFERENCE)
    {
        if(isNaN(global.baseRX))
        {
            global.baseRX = x;
            global.baseRY = y;
            appendPromptHistory();
            setPromptPrefix("Specify second point: ");
        }
        else if(isNaN(global.destRX))
        {
            global.destRX = x;
            global.destRY = y;
            global.angleRef = calculateAngle(global.baseRX, global.baseRY, global.destRX, global.destRY);
            appendPromptHistory();
            setPromptPrefix("Specify the new angle: ");
        }
        else if(isNaN(global.angleNew))
        {
            global.destRX = x;
            global.destRY = y;
            global.angleNew = calculateAngle(global.baseX, global.baseY, x, y);
            rotateSelected(global.baseX, global.baseY, global.angleNew - global.angleRef);
            endCommand();
        }
    }
}

//NOTE: context() is run when a context menu entry is chosen.
function context(str)
{
    todo("ROTATE", "context()");
}

//NOTE: prompt() is run when Enter is pressed.
//      appendPromptHistory is automatically called before prompt()
//      is called so calling it is only needed for erroneous input.
//      Any text in the command prompt is sent as an uppercase string.
function prompt(str)
{
    if(global.mode == global.mode_NORMAL)
    {
        if(global.firstRun)
        {
            var strList = str.split(",");
            if(isNaN(strList[0]) || isNaN(strList[1]))
            {
                setPromptPrefix("Invalid point.");
                appendPromptHistory();
                setPromptPrefix("Specify base point: ");
            }
            else
            {
                global.firstRun = false;
                global.baseX = Number(strList[0]);
                global.baseY = Number(strList[1]);
                addRubber("LINE");
                setRubberMode("LINE");
                setRubberPoint("LINE_START", global.baseX, global.baseY);
                setPromptPrefix("Specify rotation angle or [Reference]: ");
            }
        }
        else
        {
            if(str == "R" || str == "REFERENCE")
            {
                global.mode = global.mode_REFERENCE;
                setPromptPrefix("Specify the reference angle <0.00>: ");
                clearRubber();
            }
            else
            {
                if(isNaN(str))
                {
                    setPromptPrefix("Requires valid numeric angle, second point, or option keyword.");
                    appendPromptHistory();
                    setPromptPrefix("Specify rotation angle or [Reference]: ");
                }
                else
                {
                    global.angle = Number(str);
                    rotateSelected(global.baseX, global.baseY, global.angle);
                    endCommand();
                }
            }
        }
    }
    else if(global.mode == global.mode_REFERENCE)
    {
        todo("ROTATE", "Reference mode for prompt");
    }
}
