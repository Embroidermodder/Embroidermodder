-- Command: Platform

function platform_command ()
  -- NOTE: main() is run every time the command is started.
  --       Use it to reset variables so they are ready to go.
  if context == CONTEXT_MAIN then
    init_command()
    clear_selection()
  end
  setPromptPrefix(qsTr("Platform") + " = " + platformString())
  appendPromptHistory();
  end_command()
end

--[[
[Tips]
ToolTip=&Platform
StatusTip=List which platform is in use:  PLATFORM
--]]

PLATFORM = platform_command

