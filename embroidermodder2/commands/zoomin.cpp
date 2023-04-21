//Command: ZoomIn

//NOTE: main() is run every time the command is started.
//      Use it to reset variables so they are ready to go.
function main()
{
    initCommand();
    clearSelection();
    zoomIn();
    endCommand();
}

//NOTE: click() is run only for left clicks.
//      Middle clicks are used for panning.
//      Right clicks bring up the context menu.
function click(x, y)
{
    zoomIn();
    endCommand();
}

//NOTE: context() is run when a context menu entry is chosen.
function context(str)
{
    zoomIn();
    endCommand();
}

//NOTE: prompt() is run when Enter is pressed.
//      appendPromptHistory is automatically called before prompt()
//      is called so calling it is only needed for erroneous input.
//      Any text is in the command prompt is sent as an uppercase string.
function prompt(str)
{
    zoomIn();
    endCommand();
}
[Menu]
Name=None
Position=0

[ToolBar]
Name=None
Position=0

[Tips]
ToolTip=Zoom &In
StatusTip=Zooms to increase the apparent size of objects:  ZOOMIN

[Prompt]
Alias=ZOOMIN
