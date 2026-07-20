-- NOTE: This should be able to be reloaded during runtime.

print("Running boot script...")

-- Wrappers for standard C commands.
--
-- The command system should work in an interpretable way with
-- or without a lua script running. If all lua scripts emit a
-- sequence of calls to cmd, get and set then tracing the behaviour
-- of a script or working with undo grouping is exactly as we
-- would expect given a command written in C in the "src/commands"
-- folder.

function icon16()
  cmd("icon16")
end

function icon24()
  cmd("icon24")
end

function icon32()
  cmd("icon32")
end

function icon48()
  cmd("icon48")
end

function icon64()
  cmd("icon64")
end

function icon128()
  cmd("icon128")
end

test = {
  nested = "this is the string",
  other = "this is another string"
}

require "scripts.config"

print("Finished boot script.")

