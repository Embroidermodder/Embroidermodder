-- Command: Icon24

function icon24_command()
  -- NOTE: main() is run every time the command is started.
  --       Use it to reset variables so they are ready to go.
  if context == CONTEXT_MAIN then
    init_command()
    clear_selection()
  end
  icon24()
  end_command()
end

icon24_data = {
  menu_name = "None",
  menu_position = 0,
  toolbar_name = "None",
  toolbar_position = 0,
  tooltip = "Icon&24",
  statustip = "Sets the toolbar icon size to 24x24.",
  alias = "ICON24",
  command = icon24_command
}

-- register_command(icon24_data)
