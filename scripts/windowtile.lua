-- Command: WindowTile

-- NOTE: main() is run every time the command is started.
--       Use it to reset variables so they are ready to go.
function main()
{
    initCommand();
    clearSelection();
    windowTile();
    endCommand();
}

-- NOTE: click() is run only for left clicks.
--       Middle clicks are used for panning.
--       Right clicks bring up the context menu.
function click(x, y)
{
    windowTile();
    endCommand();
}

-- NOTE: context() is run when a context menu entry is chosen.
function context(str)
{
    windowTile();
    endCommand();
}

-- NOTE: prompt() is run when Enter is pressed.
--       appendPromptHistory is automatically called before prompt()
--       is called so calling it is only needed for erroneous input.
--       Any text is in the command prompt is sent as an uppercase string.
function prompt(str)
{
    windowTile();
    endCommand();
}
[Menu]
Name=None
Position=0

[ToolBar]
Name=None
Position=0

[Tips]
ToolTip=&Tile
StatusTip=Tile the windows:  TILE

[Prompt]
Alias=TILE, WINDOWTILE
