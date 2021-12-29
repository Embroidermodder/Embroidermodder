#!/usr/bin/env python3

r"""
    Embroidermodder 2.

    -----

    Copyright 2013-2022 The Embroidermodder Team
    Embroidermodder 2 is Open Source Software.
    See LICENSE for licensing terms.

    -----

    Another attempt at a graphical user interface that runs on
    lots of machines without a complex build or fragile dependencies.

    This is a translation of some of the ideas we came up with for other
    attempts.
"""

from embroidermodder.main_window import alert, clearSelection, translate

class SysWindows():
    r"""
    "menu-name": "None",
    "menu-position": 0,
    "toolbar-name": "None",
    "toolbar-position": 0,
    "tooltip": "&SysWindows",
    "statustip": "Arrange the windows:  SYSWINDOWS",
    "alias": "WINDOWS, SYSWINDOWS"
    """
    def __init__(self):
        clearSelection()
        set_prompt_prefix(translate("Enter an option [Cascade/Tile]: "))

    def prompt(self, cmd):
        if cmd == "C" or cmd == "CASCADE":
            # TODO: Probably should add additional qsTr calls here.
            windowCascade()
        elif cmd == "T" or cmd == "TILE":
            #TODO: Probably should add additional qsTr calls here.
            windowTile()
        else:
            alert(translate("Invalid option keyword."))
            set_prompt_prefix(translate("Enter an option [Cascade/Tile]: "))

