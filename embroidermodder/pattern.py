#!/usr/bin/env python3

r"""
    Embroidermodder 2.

    -----

    Copyright 2013-2022 The Embroidermodder Team
    Embroidermodder 2 is Open Source Software.
    See LICENSE for licensing terms.

    -----

    The Pattern class definition file.
"""

from libembroidery import convert

import embroidermodder.color as color
import embroidermodder.geometry as geometry
import embroidermodder.utility as utility
from embroidermodder.utility import debug_message

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
