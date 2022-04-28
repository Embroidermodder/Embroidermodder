#!/usr/bin/env python3

r"""
    Embroidermodder 2.

    ------------------------------------------------------------

    Copyright 2013-2022 The Embroidermodder Team
    Embroidermodder 2 is Open Source Software.
    See LICENSE for licensing terms.

    ------------------------------------------------------------

    Same as __init__.py, allows us to boot the development version
    with the command:

        $ python3 -m embroidermodder

    or

        > py -m embroidermodder

    on Windows.

    We still need __init__ for anything that wants to interact with
    embroidermodder from Python, it makes the syntax nicer.
"""

from embroidermodder.main_window import MainWindow as main

window = main()
window.root.mainloop()
