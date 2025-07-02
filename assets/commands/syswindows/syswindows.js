//Command: SysWindows

//NOTE: main() is run every time the command is started.
//      Use it to reset variables so they are ready to go.
function main()
{
    initCommand();
    clearSelection();
    setPromptPrefix(qsTr("Enter an option [Cascade/Tile]: "));
}

//NOTE: click() is run only for left clicks.
//      Middle clicks are used for panning.
//      Right clicks bring up the context menu.
function click(x, y)
{
    //Do Nothing
}

//NOTE: context() is run when a context menu entry is chosen.
function context(str)
{
    //Do Nothing
}

//NOTE: prompt() is run when Enter is pressed.
//      appendPromptHistory is automatically called before prompt()
//      is called so calling it is only needed for erroneous input.
//      Any text in the command prompt is sent as an uppercase string.
function prompt(str)
{
    if(str == "C" || str == "CASCADE") //TODO: Probably should add additional qsTr calls here.
    {
        windowCascade();
        endCommand();
    }
    else if(str == "T" || str == "TILE") //TODO: Probably should add additional qsTr calls here.
    {
        windowTile();
        endCommand();
    }
    else
    {
        alert(qsTr("Invalid option keyword."));
        setPromptPrefix(qsTr("Enter an option [Cascade/Tile]: "));
    }
}
