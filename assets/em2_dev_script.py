#!/usr/bin/env python3

r"""
Embroidermodder 2.

------------------------------------------------------------

Copyright 2013-2023 The Embroidermodder Team
Embroidermodder 2 is Open Source Software.
See LICENSE for licensing terms.

------------------------------------------------------------

In order to improve the config.toml configuration file, we
can't rely on Embroidermodder2 itself since it will crash
on attempting to load poorly formed data. Instead, we
run checks with this script to see that config.toml is
well-formed as toml and it can make recommendations
to the developers on what to do about missing data like
a compiler would.

"""

header = """
# Embroidermodder 2.
#
# ------------------------------------------------------------
#
# Copyright 2013-2023 The Embroidermodder Team
# Embroidermodder 2 is Open Source Software.
# See LICENSE for licensing terms.
#
# ------------------------------------------------------------

"""

import toml

d = {}
with open("config.toml", "r") as f:
    s = f.read()
    d = toml.loads(s)

print(d)

