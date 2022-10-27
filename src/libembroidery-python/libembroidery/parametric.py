#!/usr/bin/env python3

r"""
    Embroidermodder 2.

    ------------------------------------------------------------

    Copyright 2013-2022 The Embroidermodder Team
    Embroidermodder 2 is Open Source Software.
    See LICENSE for licensing terms.

    ------------------------------------------------------------

    The support for all other geometric primatives.
"""

import math

from libembroidery.tools import Pen, Vector, clear_selection
from libembroidery.line import Line
from libembroidery.parser import list_processor


class Parametric():
    r"""
    The generic design primative class.

    To make a specific object use a call like this:

    dolphin = Parametric(settings["objects"]["dolphin"])
    """
    def __init__(self, desc, center=Vector(math.nan, math.nan),
                 mode="NUM_POINTS"):
        r" Create an object. "
        clear_selection()
        self.desc = desc
        self.num_points = desc["num_points"]
        # min:64 max:8192
        self.center = center
        self.scale = Vector(desc["scale"][0], desc["scale"][1])
        self.mode = mode
        self.rubber_points = {
            "POLYGON": self.center
        }
        self.rubber_mode = "POLYGON"
        self.spare_rubber = "POLYGON"
        self.radius_outer = 1.0
        self.radius_inner = 1.0
        self.rubber_text = {}
        self.update()

    def update(self):
        r" Update the object to represent the current data. "
        for i in range(self.num_points+1):
            t_parameter = (2*math.pi)/self.num_points*i
            rubber_point = self.center

            curve_x = self.desc["x"]
            exp = curve_x.replace("t", str(t_parameter))
            rubber_point.x += self.scale.x * list_processor(exp)

            curve_y = self.desc["y"]
            exp = curve_y.replace("t", str(t_parameter))
            rubber_point.y += self.scale.y * list_processor(exp)

            self.rubber_points[f"POLYGON_POINT_{i}"] = rubber_point
            self.rubber_text["POLYGON_NUM_POINTS"] = str(self.num_points)
