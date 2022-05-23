#!/usr/bin/env python3


r"""
    Libembroidery.

    ---------------------------------------------

    Copyright 2018-2022 The Embroidermodder Team
    Libembroidery is Open Source Software.
    See LICENSE for licensing terms.

    ---------------------------------------------

    The Pattern class definition file.

    More Pythonic version than generated bindings,
    a hand translation of the C version.

    All of the object orientation happens here.
"""

from libembroidery.tools import (
    Pen, Vector, vector_from_str, closest_vector, load_data
)
from libembroidery.geometry import (
    Line, Rect, Arc, Circle, DimLeader, Ellipse,
    Polygon, Polyline, PolylineArray, Path
)
from libembroidery.parametric import Parametric
from libembroidery.text import TextSingle, Text

NORMAL = 0
JUMP = 1
END = 16
format_table = load_data("formats.json")
number_of_formats = len(format_table)


class Pattern():
    r"""
    While we're working out how to interface libembroidery
    and embroidermodder the Pattern is almost completely detatched from
    the python binding.

    We only interact with it by calling the libembroidery convert function
    to make it so we only need to import formats other than SVG and export
    to formats other than SVG.

    This leads to a few areas of repeated code that will have to do for now.
    The most important is that SVG needs to be parsable by both embroidermodder
    in Python and libembroidery in C.
    """
    def __init__(self, fname=""):
        r"""
        .
        """
        debug_message("")
        if len(fname) > 0:
            self.open(fname)

        self.geometry = []
        self.stitches = []

    def open(self, fname):
        r"""
        Open the file called fname and load the data to
        .
        """
        debug_message("")

    def save(self, fname):
        r"""
        .
        """
        debug_message("")

    def save_as(self, fname):
        r"""
        .
        """
        self.save("buffer_QFXZ.svg")
        convert("buffer_QFXZ.svg", fname)

    def add_circle(self, center, radius):
        r"."
        self.geometry += [Circle(center, radius)]

    def add_dimleader(self, dimleader):
        r"."
        self.geometry += [DimLeader()]

    def add_ellipse(self, ellipse):
        r"."
        self.geometry += [Ellipse()]

    def add_image(self, image):
        r"."
        self.geometry += [Image()]

    def add_line(self, start, end):
        r"."
        self.geometry += [Line(start, end)]

    def add_stitch(self, x, y, flags=0, color=0, relative=False):
        r"."
        stitch = {
            "position": Vector(x, y),
            "flags": flags,
            "color": color
        }
        self.stitches += [stitch]

    def convert_to_stitches(self):
        r"."
        debug_message("convert_to_stitches")
