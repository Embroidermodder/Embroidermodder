print("Running tests...")

import time

def pause():
    time.sleep(0.3)

# Create a file for testing.
new()
pause()

# Try all icon sizes.
icon16()
pause()
icon24()
pause()
icon32()
pause()
icon48()
pause()
icon64()
pause()
icon128()
pause()
icon16()
pause()

panup()
pause()
pandown()
pause()
panleft()
pause()
panright()
pause()
panup()
pause()
panup()
pause()

# FIXME:
#messagebox("information", "Test Script",
#    "The test script has managed to create a messagebox.")

print("Tests complete.")
