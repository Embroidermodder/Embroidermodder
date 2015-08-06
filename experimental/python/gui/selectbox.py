#!/usr/bin/env python
# -*- coding: utf-8 -*-

"""
=============================
|module_summary| selectbox.py
=============================

TOWRITE

Classes summary:
================

============================ ============================
:class:`~SelectBox`          TOWRITE
============================ ============================

---------------------------------------------------------

|

"""

#-Imports.---------------------------------------------------------------------
#--PySide/PyQt Imports.
if PYSIDE:
    ## from PySide import QtCore, QtGui
    # or... Improve performace with less dots...
    from PySide.QtCore import qDebug, Qt, QSize, Slot
    from PySide.QtGui import QRubberBand, QColor, QPen, QBrush, QPainter
elif PYQT4:
    import sip
    sip.setapi('QString', 2)
    sip.setapi('QVariant', 2)
    ## from PyQt4 import QtCore, QtGui
    # or... Improve performace with less dots...
    from PyQt4.QtCore import qDebug, Qt, QSize
    from PyQt4.QtCore import pyqtSlot as Slot
    from PyQt4.QtGui import QRubberBand, QColor, QPen, QBrush, QPainter


# C++C++C++C++C++C++C++C++C++C++C++C++C++C++C++C++C++C++C++C++C++C++C++C++C++
#include "selectbox.h"
#include <QPainter>
# C++C++C++C++C++C++C++C++C++C++C++C++C++C++C++C++C++C++C++C++C++C++C++C++C++


class SelectBox(QRubberBand):
    """
    Subclass of `QRubberBand`_

    TOWRITE

    .. sphinx_generate_methods_summary::
       SelectBox
    """
    def __init__(self, s, parent=None):
        """
        Default class constructor.

        :param `s`: TOWRITE
        :type `s`: QRubberBand.Shape
        :param `parent`: Pointer to a parent widget instance.
        :type `parent`: `QWidget`_
        """
        super(SelectBox, self).__init__(s, parent)

        # private
        self._leftBrushColor = QColor()
        self._rightBrushColor = QColor()
        self._leftPenColor = QColor()
        self._rightPenColor = QColor()
        self._alpha = 255  # quint8  #: TODO: what is the initial int?

        self._dirBrush = QBrush()
        self._leftBrush = QBrush()
        self._rightBrush = QBrush()

        self._dirPen = QPen()
        self._leftPen = QPen()
        self._rightPen = QPen()

        self._boxDir = False  # TODO # is this initial bool value right?

        # Default values
        self.setColors(QColor(Qt.darkGreen), QColor(Qt.green), QColor(Qt.darkBlue), QColor(Qt.blue), 32)

    def paintEvent(self, event):
        """
        Handles the ``paintEvent`` event for :class:`SelectBox`.

        :param `event`: A `QPaintEvent`_ to be processed.
        """
        painter = QPainter(self)
        painter.setPen(self._dirPen)
        width, height = self.width(), self.height()
        painter.fillRect(0, 0, width - 1, height - 1, self._dirBrush)
        painter.drawRect(0, 0, width - 1, height - 1)

    def forceRepaint(self):
        """
        Force repaint the rubberband.

        .. NOTE:: HACK: Take that QRubberBand!
        """
        # HACK: Take that QRubberBand!
        hack = self.size()  # QSize
        self.resize(hack + QSize(1, 1))
        self.resize(hack)

    # Slots ------------------------------------------------------------------

    @Slot(int)
    def setDirection(self, dir):
        """
        TOWRITE

        :param `dir`: TOWRITE
        :type `dir`: int
        """
        if not dir:
            self._dirPen = self._leftPen
            self._dirBrush = self._leftBrush
        else:
            self._dirPen = self._rightPen
            self._dirBrush = self._rightBrush
        self._boxDir = dir

    @Slot(QColor, QColor, QColor, QColor, int)
    def setColors(self, colorL, fillL, colorR, fillR, newAlpha):
        """
        TOWRITE

        :param `colorL`: TOWRITE
        :type `colorL`: `QColor`_
        :param `fillL`: TOWRITE
        :type `fillL`: `QColor`_
        :param `colorR`: TOWRITE
        :type `colorR`: `QColor`_
        :param `fillR`: TOWRITE
        :type `fillR`: `QColor`_
        :param `newAlpha`: TOWRITE
        :type `newAlpha`: int
        """
        qDebug("SelectBox setColors()")
        self._alpha = newAlpha

        self._leftPenColor = colorL  # TODO: allow customization
        self._leftBrushColor = QColor(fillL.red(), fillL.green(), fillL.blue(), self._alpha)
        self._rightPenColor = colorR  # TODO: allow customization
        self._rightBrushColor = QColor(fillR.red(), fillR.green(), fillR.blue(), self._alpha)

        self._leftPen.setColor(self._leftPenColor)
        self._leftPen.setStyle(Qt.DashLine)
        self._leftBrush.setStyle(Qt.SolidPattern)
        self._leftBrush.setColor(self._leftBrushColor)

        self._rightPen.setColor(self._rightPenColor)
        self._rightPen.setStyle(Qt.SolidLine)
        self._rightBrush.setStyle(Qt.SolidPattern)
        self._rightBrush.setColor(self._rightBrushColor)

        if not self._boxDir:
            self._dirPen = self._leftPen
            self._dirBrush = self._leftBrush
        else:
            self._dirPen = self._rightPen
            self._dirBrush = self._rightBrush

        self.forceRepaint()

# kate: bom off; indent-mode python; indent-width 4; replace-trailing-space-save on;
