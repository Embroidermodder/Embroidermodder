#!/usr/bin/env python3

r"""
    Embroidermodder 2.

    -----

    Copyright 2013-2022 The Embroidermodder Team
    Embroidermodder 2 is Open Source Software.
    See LICENSE for licensing terms.

    -----

"""

from embroidermodder.image_widget import ImageWidget
from embroidermodder.utility import debug_message


class PreviewDialog():
    """
    .
    """
    def __init__(self, parent, caption, dir, filter):
        " . "
        debug_message("PreviewDialog Constructor")

        #TODO: get actual thumbnail image from file, lets also use a size of 128x128 for now...
        #TODO: make thumbnail size adjustable thru self.settings dialog
        self.img_widget = ImageWidget("icons/default/nopreview.png")

        lay = layout()
        if lay:
            columns = lay.columnCount()
            rows = lay.rowCount()
            lay.addWidget(self.img_widget, 0, columns, rows, 1)

        self.modal = 1
        self.option = "FileDialog_DontUseNativeDialog"
        self.view_mode = "FileDialog_Detail"
        self.file_mode = "FileDialog_ExistingFiles"

        #TODO: connect the currentChanged signal to update the preview img_widget.

    def toPolyline(self, pattern, obj_pos, obj_path, layer, color, line_type,  line_weight):
        " . "
        debug_message("Unused arguments:")
        debug_message(f"    {pattern}, {obj_pos}, {obj_path},")
        debug_message(f"    {layer}, {color}, {line_type}, {line_weight}.")
