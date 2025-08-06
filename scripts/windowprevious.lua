-- Command: WindowPrevious

function window_previous_command ()
  -- NOTE: main() is run every time the command is started.
  --       Use it to reset variables so they are ready to go.
  if context == CONTEXT_MAIN then
    init_command()
    clear_selection()
  end
  window_previous()
  end_command()
end

--[[
[Tips]
ToolTip=Pre&vious
StatusTip=Move the focus to the previous window:  PREVIOUS

[Prompt]
Alias=PREV, PREVIOUS, WINDOWPREVIOUS
--]]

