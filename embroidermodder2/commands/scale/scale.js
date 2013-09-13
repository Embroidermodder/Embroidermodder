//Command: Scale

var global = {}; //Required
global.firstRun;
global.baseX;
global.baseY;
global.destX;
global.destY;
global.factor;

global.baseRX;
global.baseRY;
global.destRX;
global.destRY;
global.factorRef;
global.factorNew;

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
    global.baseX  = NaN;
    global.baseY  = NaN;
    global.destX  = NaN;
    global.destY  = NaN;
    global.factor = NaN;

    global.baseRX    = NaN;
    global.baseRY    = NaN;
    global.destRX    = NaN;
    global.destRY    = NaN;
    global.factorRef = NaN;
    global.factorNew = NaN;

    if(numSelected() <= 0)
    {
        //TODO: Prompt to select objects if nothing is preselected
        setPromptPrefix("Preselect objects before invoking the scale command.");
        appendPromptHistory();
        endCommand();
        messageBox("information", "Scale Preselect", "Preselect objects before invoking the scale command.");
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
            setPromptPrefix("Specify scale factor or [Reference]: ");
        }
        else
        {
            global.destX = x;
            global.destY = y;
            global.factor = calculateDistance(global.baseX, global.baseY, global.destX, global.destY);
            appendPromptHistory();
            scaleSelected(global.baseX, global.baseY, global.factor);
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
            global.factorRef = calculateDistance(global.baseRX, global.baseRY, global.destRX, global.destRY);
            if(global.factorRef <= 0.0)
            {
                global.destRX    = NaN;
                global.destRY    = NaN;
                global.factorRef = NaN;
                setPromptPrefix("Value must be positive and nonzero.");
                appendPromptHistory();
                setPromptPrefix("Specify second point: ");
            }
            else
            {
                appendPromptHistory();
                setPromptPrefix("Specify new length: ");
            }
        }
        else if(isNaN(global.factorNew))
        {
            global.factorNew = calculateDistance(global.baseX, global.baseY, x, y);
            if(global.factorNew <= 0.0)
            {
                global.factorNew = NaN;
                setPromptPrefix("Value must be positive and nonzero.");
                appendPromptHistory();
                setPromptPrefix("Specify new length: ");
            }
            else
            {
                appendPromptHistory();
                scaleSelected(global.baseX, global.baseY, global.factorNew/global.factorRef);
                endCommand();
            }
        }
    }
}

//NOTE: context() is run when a context menu entry is chosen.
function context(str)
{
    todo("SCALE", "context()");
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
                setPromptPrefix("Specify scale factor or [Reference]: ");
            }
        }
        else
        {
            if(str == "R" || str == "REFERENCE")
            {
                global.mode = global.mode_REFERENCE;
                setPromptPrefix("Specify reference length <1>: ");
                clearRubber();
            }
            else
            {
                if(isNaN(str))
                {
                    setPromptPrefix("Requires valid numeric distance, second point, or option keyword.");
                    appendPromptHistory();
                    setPromptPrefix("Specify scale factor or [Reference]: ");
                }
                else
                {
                    global.factor = Number(str);
                    scaleSelected(global.baseX, global.baseY, global.factor);
                    endCommand();
                }
            }
        }
    }
    else if(global.mode == global.mode_REFERENCE)
    {
        todo("SCALE", "Reference mode for prompt");
    }
}
