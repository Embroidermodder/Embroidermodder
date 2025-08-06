-- Command: Icon16
--

function icon16_command ()
  -- NOTE: main() is run every time the command is started.
  --       Use it to reset variables so they are ready to go.
  if context == CONTEXT_MAIN then
    init_command()
    clear_selection()
  end
  icon16()
  end_command()
end

-- menu-name=None
-- menu-position=0
--
-- toolbar-name=None
-- toolbar-position=0
--
-- ToolTip=Icon&16
-- StatusTip=Sets the toolbar icon size to 16x16:  ICON16
--
-- alias=ICON16
