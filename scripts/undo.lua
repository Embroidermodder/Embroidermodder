-- Command: Undo

function undo_command ()
  -- NOTE: main() is run every time the command is started.
  --       Use it to reset variables so they are ready to go.
  if context == CONTEXT_MAIN then
    initCommand();
    clearSelection();
  end
  undo()
  endCommand()
end

--[[
[Tips]
ToolTip=&Undo
StatusTip=Reverses the most recent action:  UNDO

[Prompt]
Alias=U, UNDO
--]]
