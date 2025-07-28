-- Command: WindowCascade

function window_cascade ()
  if context == CONTEXT_MAIN then
    init_command();
    clear_selection();
  end
  windowCascade();
  end_command();
end

[Tips]
ToolTip=&Cascade
StatusTip=Cascade the windows:  CASCADE

[Prompt]
Alias=CASCADE, WINDOWCASCADE
