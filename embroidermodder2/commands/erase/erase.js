//Command: Erase/Delete

//NOTE: main() is run every time the command is started.
//      Use it to reset variables so they are ready to go.
function main()
{
    initCommand();

    if(numSelected() <= 0)
    {
        //TODO: Prompt to select objects if nothing is preselected
        setPromptPrefix("Preselect objects before invoking the delete command.");
        appendPromptHistory();
        endCommand();
        messageBox("information", "Delete Preselect", "Preselect objects before invoking the delete command.");
    }
    else
    {
        deleteSelected();
        endCommand();
    }
}

//NOTE: click() is run only for left clicks.
//      Middle clicks are used for panning.
//      Right clicks bring up the context menu.
function click(x, y)
{
    deleteSelected();
    endCommand();
}

//NOTE: context() is run when a context menu entry is chosen.
function context(str)
{
    deleteSelected();
    endCommand();
}

//NOTE: prompt() is run when Enter is pressed.
//      appendPromptHistory is automatically called before prompt()
//      is called so calling it is only needed for erroneous input.
//      Any text is in the command prompt is sent as an uppercase string.
function prompt(str)
{
    deleteSelected();
    endCommand();
}
