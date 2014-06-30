#!/usr/bin/env python
# -*- coding: utf-8 -*-

"""
===============================
|module_summary| imagewidget.py
===============================

TOWRITE

Classes summary:
================

============================ ============================
:class:`~ImageWidget`        TOWRITE
============================ ============================

---------------------------------------------------------

|

"""

#-Imports.---------------------------------------------------------------------
#--PySide/PyQt Imports.
try:
    ## from PySide import QtCore, QtGui
    # or... Improve performace with less dots...
    from PySide.QtCore import qDebug
    from PySide.QtGui import QImage, QPainter, QWidget
    PYSIDE = True
    PYQT4 = False
except ImportError:
    raise
#    ## from PyQt4 import QtCore, QtGui
#    # or... Improve performace with less dots...
#    from PyQt4.QtCore import qDebug
#    from PyQt4.QtGui import QImage, QPainter, QWidget
#    PYSIDE = False
#    PYQT4 = True


# C++C++C++C++C++C++C++C++C++C++C++C++C++C++C++C++C++C++C++C++C++C++C++C++C++
#include <QDebug>
#include "imagewidget.h"
# C++C++C++C++C++C++C++C++C++C++C++C++C++C++C++C++C++C++C++C++C++C++C++C++C++


class ImageWidget(QWidget):
    """
    Subclass of `QWidget`_

    """
    def __init__(self, filename, parent):
        """
        Default class constructor.

        :param `filename`: The filename path.
        :type `filename`: QString
        :param `parent`: Pointer to a parent widget instance.
        :type `parent`: `QWidget`_
        """
        super(ImageWidget, self).__init__(parent)

        qDebug("ImageWidget Constructor")

        self.img = img = QImage()

        self.load(filename)

        self.setMinimumWidth(img.width())
        self.setMinimumHeight(img.height())
        self.setMaximumWidth(img.width())
        self.setMaximumHeight(img.height())

        self.show()


    def load(self, fileName):
        """
        TOWRITE

        :param `fileName`: The filename to load.
        :type `fileName`: QString
        :returns: Whether or not the load was successful.
        :rtype: bool

        .. TODO:: Make sure this returns False if failure.
        """

        self.img.load(fileName)
        return True

    def save(self, fileName):
        """
        TOWRITE

        :param `fileName`: The filename to save.
        :type `fileName`: QString
        :returns: Whether or not the save was successful.
        :rtype: bool

        .. TODO:: Make sure this returns False if failure.
        """
        self.img.save(fileName, "PNG")
        return True

    def __del__(self):
        """Class destructor."""
        qDebug("ImageWidget Destructor")

    def paintEvent(self, event):
        """
        Handles the ``paintEvent`` event for :class:`ImageWidget`.

        :param `event`: A `QPaintEvent`_ to be processed.
        """
        painter = QPainter(self)
        img = self.img
        width, height = img.width(), img.height()
        painter.setViewport(0, 0, width, height)
        painter.setWindow(0, 0, width, height)
        painter.drawImage(0, 0, img)


# kate: bom off; indent-mode cstyle; indent-width 4; replace-trailing-space-save on;
