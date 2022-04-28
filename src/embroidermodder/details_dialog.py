#!/usr/bin/env python3

r"""
    Embroidermodder 2.

    -----

    Copyright 2013-2022 The Embroidermodder Team
    Embroidermodder 2 is Open Source Software.
    See LICENSE for licensing terms.

    -----

"""

import tkinter as tk

from libembroidery import Rect
from embroidermodder.utility import debug_message, translate


class DetailsDialog():
    """
    Creates a dialog showing key information about the pattern,
    ideally this will update as the pattern changes without any key presses
    or clicks.
    """
    def __init__(self, theScene, mainWindow):
        r"""
        .
        """
        self.dialog = tk.Tk()
        self.dialog.setMinimumSize(750, 550)
        self.dialog.title(translate("Embroidery Design Details"))

        self.stitches_total = 0
        self.stitches_real = 0
        self.stitches_jump = 0
        self.stitches_trim = 0
        self.color_total = 0
        self.color_changes = 0

        self.bounding_rect = Rect(0.0, 0.0, 0.1, 0.1)

        self.get_info()
        self.mw = mainWindow
        self.main_widget = self.create_main_widget()

        self.button_box = tk.ButtonBox(self.dialog, text="QDialogButtonBox_Ok")
        # connect(buttonBox, SIGNAL(accepted()), SLOT(accept()))

        vbox_layout_main = tk.VBoxLayout()
        vbox_layout_main.add_widget(self.main_widget)
        vbox_layout_main.add_widget(self.button_box)
        self.dialog.set_layout(vbox_layout_main)

        self.mw.setOverrideCursor("ArrowCursor")

    def __del__(self):
        r"""
        .
        """
        self.mw.restoreOverrideCursor()

    def createHistogram(self):
        r"""
        .
        """
        debug_message("TODO: createHistogram")

    def get_info(self):
        r"""
        .
        """
        # TODO: generate a temporary pattern from the scene data.

        # TODO: grab this information from the pattern
        self.stitches_total = 5
        # TODO: embStitchList_count(pattern.stitchList, TOTAL);
        self.stitches_real = 4
        # TODO: embStitchList_count(pattern.stitchList, NORMAL);
        self.stitches_jump = 3
        # TODO: embStitchList_count(pattern.stitchList, JUMP);
        self.stitches_trim = 2
        # TODO: embStitchList_count(pattern.stitchList, TRIM);
        self.color_total = 1
        # TODO: embThreadList_count(pattern.threadList, TOTAL);
        self.color_changes = 0
        # TODO: embThreadList_count(pattern.threadList, CHANGES);

        self.bounding_rect.set_rect(0, 0, 50, 100)
        # TODO: embPattern_calcBoundingBox(pattern);

    def create_main_widget(self):
        r"""
        .
        """
        widget = tk.Widget(self.dialog)

        # Misc
        group_box_misc = tk.GroupBox(translate("General Information"), widget)

        Labels = []
        fields = []

        for i in range(12):
            details_labels = self.mw.setting["details_label_text"]
            Labels[i] = tk.Label(translate(details_labels[i]))

        fields[0] = tk.Label(str(self.stitches_total))
        fields[1] = tk.Label(str(self.stitches_real))
        fields[2] = tk.Label(str(self.stitches_jump))
        fields[3] = tk.Label(str(self.stitches_trim))
        fields[4] = tk.Label(str(self.color_total))
        fields[5] = tk.Label(str(self.colorChanges))
        fields[6] = tk.Label(str(self.bounding_rect.left()) + " mm")
        fields[7] = tk.Label(str(self.bounding_rect.top()) + " mm")
        fields[8] = tk.Label(str(self.bounding_rect.right()) + " mm")
        fields[9] = tk.Label(str(self.bounding_rect.bottom()) + " mm")
        fields[10] = tk.Label(str(self.bounding_rect.width()) + " mm")
        fields[11] = tk.Label(str(self.bounding_rect.height()) + " mm")

        grid_layout_misc = GridLayout(group_box_misc)
        for i in range(12):
            grid_layout_misc.add_widget(tk.Labels[i], i, 0, "Qt_AlignLeft")
            grid_layout_misc.add_widget(fields[i], i, 1, "Qt_AlignLeft")

        grid_layout_misc.setColumnStretch(1, 1)
        group_box_misc.set_layout(grid_layout_misc)

        # TODO: Color Histogram

        # Stitch Distribution
        # groupBoxDist = QGroupBox(translate("Stitch Distribution"), widget);

        # TODO: Stitch Distribution Histogram

        # Widget Layout
        vbox_layout_main = tk.VBoxLayout(widget)
        vbox_layout_main.add_widget(group_box_misc)
        # vbox_layout_main.add_widget(groupBoxDist);
        vbox_layout_main.addStretch(1)
        widget.set_layout(vbox_layout_main)

        scroll_area = tk.scroll_area()
        scroll_area.set_widget_resizable(1)
        scroll_area.set_widget(widget)
        return scroll_area
