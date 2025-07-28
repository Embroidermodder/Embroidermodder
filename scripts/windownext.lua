-- Command: WindowNext

function window_next ()
  -- NOTE: main() is run every time the command is started.
  --       Use it to reset variables so they are ready to go.
  if context == CONTEXT_MAIN then
    init_command()
    clear_selection()
  end
  window_next()
  end_command()
end

--[[
[Tips]
ToolTip=Ne&xt
StatusTip=Move the focus to the next window:  NEXT

[Prompt]
Alias=NEXT, WINDOWNEXT
--]]

