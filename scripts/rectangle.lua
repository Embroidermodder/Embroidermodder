-- Command: Rectangle

var global = {}-- -- Required
global.newRect;
global.x1;
global.y1;
global.x2;
global.y2;

-- NOTE: main() is run every time the command is started.
--       Use it to reset variables so they are ready to go.
function main()
{
    initCommand();
    clearSelection();
    global.newRect = true;
    global.x1 = NaN;
    global.y1 = NaN;
    global.x2 = NaN;
    global.y2 = NaN;
    setPromptPrefix(qsTr("Specify first corner point or [Chamfer/Fillet]: "));
}

-- NOTE: click() is run only for left clicks.
--       Middle clicks are used for panning.
--       Right clicks bring up the context menu.
function click(x, y)
{
    if(global.newRect)
    {
        global.newRect = false;
        global.x1 = x;
        global.y1 = y;
        addRubber("RECTANGLE");
        setRubberMode("RECTANGLE");
        setRubberPoint("RECTANGLE_START", x, y);
        setPromptPrefix(qsTr("Specify other corner point or [Dimensions]: "));
    }
    else
    {
        global.newRect = true;
        global.x2 = x;
        global.y2 = y;
        setRubberPoint("RECTANGLE_END", x, y);
        vulcanize();
        endCommand();
    }
}

-- NOTE: context() is run when a context menu entry is chosen.
function context(str)
{
    todo("RECTANGLE", "context()");
}

-- NOTE: prompt() is run when Enter is pressed.
--       appendPromptHistory is automatically called before prompt()
--       is called so calling it is only needed for erroneous input.
--       Any text in the command prompt is sent as an uppercase string.
function prompt(str)
{
    if(str == "C" || str == "CHAMFER") -- TODO: Probably should add additional qsTr calls here.
    {
        todo("RECTANGLE", "prompt() for CHAMFER");
    }
    else if(str == "D" || str == "DIMENSIONS") -- TODO: Probably should add additional qsTr calls here.
    {
        todo("RECTANGLE", "prompt() for DIMENSIONS");
    }
    else if(str == "F" || str == "FILLET") -- TODO: Probably should add additional qsTr calls here.
    {
        todo("RECTANGLE", "prompt() for FILLET");
    }
    else
    {
        var strList = str.split(",");
        if(isNaN(strList[0]) || isNaN(strList[1]))
        {
            alert(qsTr("Invalid point."));
            setPromptPrefix(qsTr("Specify first point: "));
        }
        else
        {
            var x = Number(strList[0]);
            var y = Number(strList[1]);
            if(global.newRect)
            {
                global.newRect = false;
                global.x1 = x;
                global.y1 = y;
                addRubber("RECTANGLE");
                setRubberMode("RECTANGLE");
                setRubberPoint("RECTANGLE_START", x, y);
                setPromptPrefix(qsTr("Specify other corner point or [Dimensions]: "));
            }
            else
                global.newRect = true
                global.x2 = x
                global.y2 = y
                setRubberPoint("RECTANGLE_END", x, y)
                vulcanize()
                endCommand()
            end
        end
    end
end
