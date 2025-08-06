-- Command: WindowCloseAll

function window_close_all_command ()
  -- NOTE: main() is run every time the command is started.
  --       Use it to reset variables so they are ready to go.
  if context == CONTEXT_MAIN then
    initCommand()
    clearSelection()
  end
  window_close_all()
  end_command()
}

--[[
[Tips]
ToolTip=Close &All
StatusTip=Close all the windows:  CLOSEALL

[Prompt]
Alias=CLOSEALL, WINDOWCLOSEALL
--]]
