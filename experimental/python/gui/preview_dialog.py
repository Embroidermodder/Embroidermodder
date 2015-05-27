#!/usr/bin/env python
# -*- coding: utf-8 -*-

"""
==================================
|module_summary| preview_dialog.py
==================================

TOWRITE

Classes summary:
================

============================ ============================
:class:`~PreviewDialog`      TOWRITE
============================ ============================

---------------------------------------------------------

|

"""

#-Imports.---------------------------------------------------------------------
#--PySide/PyQt Imports.
if PYSIDE:
    ## from PySide import QtCore, QtGui
    # or... Improve performace with less dots...
    from PySide.QtCore import qDebug
    from PySide.QtGui import QFileDialog, QGridLayout, QLayout
elif PYQT4:
    import sip
    sip.setapi('QString', 2)
    sip.setapi('QVariant', 2)
    ## from PyQt4 import QtCore, QtGui
    # or... Improve performace with less dots...
    from PyQt4.QtCore import qDebug
    from PyQt4.QtGui import QFileDialog, QGridLayout, QLayout

# Local imports.
from imagewidget import ImageWidget


# C++C++C++C++C++C++C++C++C++C++C++C++C++C++C++C++C++C++C++C++C++C++C++C++C++
#include "preview-dialog.h"
#include "imagewidget.h"

#include <QDebug>
#include <QGridLayout>
# C++C++C++C++C++C++C++C++C++C++C++C++C++C++C++C++C++C++C++C++C++C++C++C++C++


class PreviewDialog(QFileDialog):
    """
    Subclass of `QFileDialog`_

    """
    def __init__(self, parent=None, caption="", dir="", filter=""):
        """
        Default class constructor.

        :param `parent`: Pointer to a parent widget instance.
        :type `parent`: `QWidget`_
        :param `caption`: The dialog caption.
        :type `caption`: QString
        :param `dir`: The dialog directory.
        :type `dir`: QString
        :param `filter`: The dialog filter.
        :type `filter`: QString
        """
        super(PreviewDialog, self).__init__(parent, caption, dir, filter)

        qDebug("PreviewDialog Constructor")

        # TODO: get actual thumbnail image from file, lets also use a size of 128x128 for now...
        # TODO: make thumbnail size adjustable thru settings dialog
        # TODO/PORT: Need access to gIconDir
        imgWidget = ImageWidget("icons/default/nopreview.png", self)

        lay = self.layout()
        if isinstance(lay, QGridLayout):  # if(qobject_cast<QGridLayout*>(lay))
            grid = lay  # QGridLayout* grid = qobject_cast<QGridLayout*>(lay);
            grid.addWidget(imgWidget, 0, grid.columnCount(), grid.rowCount(), 1)

        self.setModal(True)
        self.setOption(QFileDialog.DontUseNativeDialog)
        self.setViewMode(QFileDialog.Detail)
        self.setFileMode(QFileDialog.ExistingFiles)

        # TODO: connect the currentChanged signal to update the preview imgWidget.


    def __del__(self):
        """Class destructor"""
        qDebug("PreviewDialog Destructor")


# kate: bom off; indent-mode python; indent-width 4; replace-trailing-space-save on;
