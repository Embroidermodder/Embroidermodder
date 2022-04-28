#!/usr/bin/env python3

r"""
    Embroidermodder 2.

    ------------------------------------------------------------

    Copyright 2013-2022 The Embroidermodder Team
    Embroidermodder 2 is Open Source Software.
    See LICENSE for licensing terms.

    ------------------------------------------------------------

    The Color class definition.
"""

import math

from embroidermodder.config import settings
from embroidermodder.utility import (
    alert, translate, set_prompt_prefix
)


def valid_rgb(red, green, blue):
    " . "
    if math.isnan(red) or math.isnan(green) or math.isnan(blue):
        return False
    if red < 0 or red > 255:
        return False
    if green < 0 or green > 255:
        return False
    if blue < 0 or blue > 255:
        return False
    return True

class Color():
    r"""
    To manage thread colors and such, this class can call the settings
    JSON dict.
    """
    def __init__(self):
        " . "
        set_prompt_prefix("")
        self.mode = "BACKGROUND"
        self.modes = ["BACKGROUND","CROSSHAIR", "GRID"]
        self.red = math.nan
        self.green = math.nan
        self.blue = math.nan
        prefix = "Enter RED,GREEN,BLUE values for background or [Crosshair/Grid]: "
        set_prompt_prefix(translate(prefix))

    def prompt(self, cmd):
        " . "
        if self.mode == "BACKGROUND":
            if cmd[0] == "C" or cmd == "CROSSHAIR":
                self.mode = "CROSSHAIR"
                set_prompt_prefix(translate("Specify crosshair color: "))
            elif cmd[0] == "G" or cmd == "GRID":
                self.mode = "GRID"
                set_prompt_prefix(translate("Specify grid color: "))
            else:
                str_list = str.split(",")
                red = int(str_list[0])
                green = int(str_list[1])
                blue = int(str_list[2])
                if not valid_rgb(red, green, blue):
                    setBackgroundColor(red, green, blue)
                else:
                    alert(translate("Invalid color. R,G,B values must be in the range of 0-255."))
                    set_prompt_prefix(translate("Specify background color: "))

        elif self.mode == "CROSSHAIR":
            str_list = str.split(",")
            red = int(str_list[0])
            green = int(str_list[1])
            blue = int(str_list[2])
            if valid_rgb(red, green, blue):
                setCrossHairColor(red, green, blue)
            else:
                alert(translate("Invalid color. R,G,B values must be in the range of 0-255."))
                set_prompt_prefix(translate("Specify crosshair color: "))

        elif self.mode == "GRID":
            str_list = str.split(",")
            red = int(str_list[0])
            green = int(str_list[1])
            blue = int(str_list[2])
            if not valid_rgb(red, green, blue):
                setGridColor(red, green, blue)
            else:
                alert(translate("Invalid color. R,G,B values must be in the range of 0-255."))
                set_prompt_prefix(translate("Specify grid color: "))
