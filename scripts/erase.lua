-- Command: Erase/Delete

-- NOTE: main() is run every time the command is started.
--       Use it to reset variables so they are ready to go.
function main()
{
    initCommand();

    if(numSelected() <= 0)
    {
        -- TODO: Prompt to select objects if nothing is preselected
        alert(qsTr("Preselect objects before invoking the delete command."));
        endCommand();
        messageBox("information", qsTr("Delete Preselect"), qsTr("Preselect objects before invoking the delete command."));
    }
    else
    {
        deleteSelected();
        endCommand();
    }
}

-- NOTE: click() is run only for left clicks.
--       Middle clicks are used for panning.
--       Right clicks bring up the context menu.
function click(x, y)
{
    deleteSelected();
    endCommand();
}

-- NOTE: context() is run when a context menu entry is chosen.
function context(str)
{
    deleteSelected();
    endCommand();
}

-- NOTE: prompt() is run when Enter is pressed.
--       appendPromptHistory is automatically called before prompt()
--       is called so calling it is only needed for erroneous input.
--       Any text is in the command prompt is sent as an uppercase string.
function prompt(str)
{
    deleteSelected();
    endCommand();
}
[Menu]
Name=Modify
Position=5

[ToolBar]
Name=Modify
Position=0

[Tips]
ToolTip=D&elete
StatusTip=Removes objects from a drawing:  DELETE

[Prompt]
Alias=E, ERASE, DEL, DELETE
