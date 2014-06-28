#!/usr/bin/env python
# -*- coding: utf-8 -*-

"""
=============================
|module_summary| statusbar.py
=============================

TOWRITE


Classes summary:
================

============================ ============================
:class:`~StatusBar`          StatusBar GUI element for Embroidermodder application.
============================ ============================

---------------------------------------------------------
"""

#-Imports.---------------------------------------------------------------------
#--Python Imports.
# import os

#--PySide/PyQt Imports.
try:
    # from PySide.QtCore import qDebug, Qt
    from PySide.QtGui import QStatusBar, QLabel
except ImportError:
    raise
    # from PyQt4.QtCore import qDebug, Qt,
    # from PyQt4.QtGui import QStatusBar, QLabel

#--Local Imports.
from statusbar_button import StatusBarButton


class StatusBar(QStatusBar):
    """
    Subclass of `QStatusBar`_

    StatusBar GUI element for Embroidermodder application.

    .. sphinx_generate_methods_summary::
       StatusBar
    """
    def __init__(self, parent=None):
        """
        Default class constructor.

        :param `parent`: parent widget instance of this QStatusBar.
        :type `parent`: `QWidget`_
        """
        super(StatusBar, self).__init__(parent)

        mw = parent

        self.setObjectName("StatusBar")

        self.statusBarSnapButton   = StatusBarButton("SNAP", mw, self, self)
        self.statusBarGridButton   = StatusBarButton("GRID", mw, self, self)
        self.statusBarRulerButton  = StatusBarButton("RULER", mw, self, self)
        self.statusBarOrthoButton  = StatusBarButton("ORTHO", mw, self, self)
        self.statusBarPolarButton  = StatusBarButton("POLAR", mw, self, self)
        self.statusBarQSnapButton  = StatusBarButton("QSNAP", mw, self, self)
        self.statusBarQTrackButton = StatusBarButton("QTRACK", mw, self, self)
        self.statusBarLwtButton    = StatusBarButton("LWT", mw, self, self)
        self.statusBarMouseCoord   = QLabel(self)

        self.statusBarMouseCoord.setMinimumWidth(300) # Must fit this text always
        self.statusBarMouseCoord.setMaximumWidth(300) # "+1.2345E+99, +1.2345E+99, +1.2345E+99"

        self.addWidget(self.statusBarMouseCoord)
        self.addWidget(self.statusBarSnapButton)
        self.addWidget(self.statusBarGridButton)
        self.addWidget(self.statusBarRulerButton)
        self.addWidget(self.statusBarOrthoButton)
        self.addWidget(self.statusBarPolarButton)
        self.addWidget(self.statusBarQSnapButton)
        self.addWidget(self.statusBarQTrackButton)
        self.addWidget(self.statusBarLwtButton)


# kate: bom off; indent-mode cstyle; indent-width 4; replace-trailing-space-save on;
