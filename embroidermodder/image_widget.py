#!/usr/bin/env python3

r"""
    Embroidermodder 2.

    ------------------------------------------------------------

    Copyright 2013-2022 The Embroidermodder Team
    Embroidermodder 2 is Open Source Software.
    See LICENSE for licensing terms.

    ------------------------------------------------------------

    To make the undo history easier to manage we use a dict for
    keeping all the action information together.
"""

from embroidermodder.main_window import debug_message

class ImageWidget():
    """
    To display an embedded image as a widget in tk.
    """
    def __init__(self, filename, mw):
        " . "
        debug_message("ImageWidget Constructor")
        self.mw = mw

        self.img = img_load(filename)

        self.min_width = self.img.width()
        self.min_height = self.img.height()
        self.max_width = self.img.width()
        self.max_height = self.img.height()

        self.img.show()

    def load(self, fileName):
        " . "
        return self.img.load(fileName)

    def save(self, fileName):
        " . "
        return self.img.save(fileName, "PNG")

    def paintEvent(self, event):
        " . "
        painter = Painter()
        painter.setViewport(0, 0, self.img.width(), self.img.height())
        painter.setWindow(0, 0, self.img.width(), self.img.height())
        painter.drawImage(0, 0, self.img)
