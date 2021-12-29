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

from embroidermodder.geometry import Rect
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
        self.dialog.setMinimumSize(750,550)
        self.dialog.title(translate("Embroidery Design Details"))

        self.stitchesTotal = 0
        self.stitchesReal = 0
        self.stitchesJump = 0
        self.stitchesTrim = 0
        self.colorTotal = 0
        self.colorChanges = 0

        self.boundingRect = Rect(0.0, 0.0, 0.1, 0.1)

        self.get_info()
        self.mw = mainWindow
        self.main_widget = self.create_main_widget()

        self.button_box = tk.ButtonBox(self.dialog, text="QDialogButtonBox_Ok")
        #connect(buttonBox, SIGNAL(accepted()), SLOT(accept()))

        vboxLayoutMain = tk.VBoxLayout()
        vboxLayoutMain.addWidget(self.main_widget)
        vboxLayoutMain.addWidget(self.button_box)
        self.dialog.setLayout(vboxLayoutMain)

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
        #TODO: generate a temporary pattern from the scene data.

        #TODO: grab this information from the pattern
        self.stitchesTotal = 5
        #TODO: embStitchList_count(pattern.stitchList, TOTAL);
        self.stitchesReal = 4
        #TODO: embStitchList_count(pattern.stitchList, NORMAL);
        self.stitchesJump = 3
        #TODO: embStitchList_count(pattern.stitchList, JUMP);
        self.stitchesTrim = 2
        #TODO: embStitchList_count(pattern.stitchList, TRIM);
        self.colorTotal = 1
        #TODO: embThreadList_count(pattern.threadList, TOTAL);
        self.colorChanges = 0
        #TODO: embThreadList_count(pattern.threadList, CHANGES);

        self.boundingRect.set_rect(0, 0, 50, 100)
        #TODO: embPattern_calcBoundingBox(pattern);

    def create_main_widget(self):
        r"""
        .
        """
        widget = tk.Widget(self.dialog)

        #Misc
        groupBoxMisc = tk.GroupBox(translate("General Information"), widget)

        Labels = []
        fields = []

        for i in range(12):
            details_labels = self.mw.setting["details_label_text"]
            Labels[i] = tk.Label(translate(details_labels[i]))

        fields[0] = tk.Label(str(self.stitchesTotal))
        fields[1] = tk.Label(str(self.stitchesReal))
        fields[2] = tk.Label(str(self.stitchesJump))
        fields[3] = tk.Label(str(self.stitchesTrim))
        fields[4] = tk.Label(str(self.colorTotal))
        fields[5] = tk.Label(str(self.colorChanges))
        fields[6] = tk.Label(str(self.boundingRect.left()) + " mm")
        fields[7] = tk.Label(str(self.boundingRect.top()) + " mm")
        fields[8] = tk.Label(str(self.boundingRect.right()) + " mm")
        fields[9] = tk.Label(str(self.boundingRect.bottom()) + " mm")
        fields[10] = tk.Label(str(self.boundingRect.width()) + " mm")
        fields[11] = tk.Label(str(self.boundingRect.height()) + " mm")

        gridLayoutMisc = GridLayout(groupBoxMisc)
        for i in range(12):
            gridLayoutMisc.addWidget(tk.Labels[i], i, 0, "Qt_AlignLeft")
            gridLayoutMisc.addWidget(fields[i], i, 1, "Qt_AlignLeft")

        gridLayoutMisc.setColumnStretch(1,1)
        groupBoxMisc.setLayout(gridLayoutMisc)

        # TODO: Color Histogram

        #Stitch Distribution
        #QGroupBox* groupBoxDist = QGroupBox(translate("Stitch Distribution"), widget);

        # TODO: Stitch Distribution Histogram

        #Widget Layout
        vboxLayoutMain = tk.VBoxLayout(widget)
        vboxLayoutMain.addWidget(groupBoxMisc)
        #vboxLayoutMain.addWidget(groupBoxDist);
        vboxLayoutMain.addStretch(1)
        widget.setLayout(vboxLayoutMain)

        scrollArea = tk.ScrollArea()
        scrollArea.setWidgetResizable(1)
        scrollArea.setWidget(widget)
        return scrollArea
