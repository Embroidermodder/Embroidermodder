-- Command: WindowClose

function window_close ()
  -- NOTE: main() is run every time the command is started.
  --       Use it to reset variables so they are ready to go.
  if context == CONTEXT_MAIN then
    init_command()
    clear_selection()
  end
  window_close()
  end_command()
end

--[[
[Tips]
ToolTip=Cl&ose
StatusTip=Close the active window:  CLOSE

[Prompt]
Alias=CLOSE, WINDOWCLOSE
--]]
