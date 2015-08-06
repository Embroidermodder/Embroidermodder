#!/usr/bin/env python
# -*- coding: utf-8 -*-

"""
=================================
|module_summary| layer_manager.py
=================================

TOWRITE

Classes summary:
================

============================ ============================
:class:`~LayerManager`       TOWRITE
============================ ============================

---------------------------------------------------------

|

"""

#-Imports.---------------------------------------------------------------------
#--PySide/PyQt Imports.
if PYSIDE:
    ## from PySide import QtCore, QtGui
    # or... Improve performace with less dots...
    from PySide.QtCore import Qt, QSize
    from PySide.QtGui import (qRgb, QApplication, QIcon, QImage, QVBoxLayout,
        QPixmap, QDialog, QColor, QStandardItemModel, QSortFilterProxyModel,
        QTreeView)
elif PYQT4:
#    ## from PyQt4 import QtCore, QtGui
#    # or... Improve performace with less dots...
    from PyQt4.QtCore import Qt, QSize
    from PyQt4.QtGui import (qRgb, QApplication, QIcon, QImage, QVBoxLayout,
        QPixmap, QDialog, QColor, QStandardItemModel, QSortFilterProxyModel,
        QTreeView)


#include <QtGui>
#include "layer-manager.h"
#include "mainwindow.h"

class LayerManager(QDialog):
    """
    Subclass of `QDialog`_

    TOWRITE

    .. sphinx_generate_methods_summary::
       LayerManager
    """
    def __init__(self, mw, parent=None):
        """
        Default class constructor.

        :param `mw`: The application's MainWindow instance.
        :type `mw`: `QMainWindow`_
        :param `parent`: Pointer to a parent widget instance.
        :type `parent`: `QWidget`_
        """
        super(LayerManager, self).__init__(parent)

        tr = self.tr

        self.layerModel = layerModel = QStandardItemModel(0, 8, self)

        self.layerModelSorted = layerModelSorted = QSortFilterProxyModel()
        layerModelSorted.setDynamicSortFilter(True)
        layerModelSorted.setSourceModel(layerModel)

        self.treeView = treeView = QTreeView()
        treeView.setRootIsDecorated(False)
        treeView.setAlternatingRowColors(True)
        treeView.setModel(layerModelSorted)
        treeView.setSortingEnabled(True)
        treeView.sortByColumn(0, Qt.AscendingOrder)

        mainLayout = QVBoxLayout()
        mainLayout.addWidget(treeView)
        self.setLayout(mainLayout)

        self.setWindowTitle(tr("Layer Manager"))
        self.setMinimumSize(750, 550)

        ## layerModel.setHeaderData(0, Qt.Horizontal, self.tr("Name"))
        ## layerModel.setHeaderData(1, Qt.Horizontal, self.tr("Visible"))
        ## layerModel.setHeaderData(2, Qt.Horizontal, self.tr("Frozen"))
        ## layerModel.setHeaderData(3, Qt.Horizontal, self.tr("Z Value"))
        ## layerModel.setHeaderData(4, Qt.Horizontal, self.tr("Color"))
        ## layerModel.setHeaderData(5, Qt.Horizontal, self.tr("Linetype"))
        ## layerModel.setHeaderData(6, Qt.Horizontal, self.tr("Lineweight"))
        ## layerModel.setHeaderData(7, Qt.Horizontal, self.tr("Print"))
        localMeth = layerModel.setHeaderData
        localQtHorizontal = Qt.Horizontal
        [localMeth(i, localQtHorizontal, tr(strng))
            for i, strng in ((0, "Name"),
                             (1, "Visible"),
                             (2, "Frozen"),
                             (3, "Z Value"),
                             (4, "Color"),
                             (5, "Linetype"),
                             (6, "Lineweight"),
                             (7, "Print")
                             )]

        ## addLayer = self.addLayer  # local optimization
        ## addLayer("0", True, False, 0.0, qRgb(0, 0, 0), "Continuous", "Default", True)
        ## addLayer("1", True, False, 1.0, qRgb(0, 0, 0), "Continuous", "Default", True)
        ## addLayer("2", True, False, 2.0, qRgb(0, 0, 0), "Continuous", "Default", True)
        ## addLayer("3", True, False, 3.0, qRgb(0, 0, 0), "Continuous", "Default", True)
        ## addLayer("4", True, False, 4.0, qRgb(0, 0, 0), "Continuous", "Default", True)
        ## addLayer("5", True, False, 5.0, qRgb(0, 0, 0), "Continuous", "Default", True)
        ## addLayer("6", True, False, 6.0, qRgb(0, 0, 0), "Continuous", "Default", True)
        ## addLayer("7", True, False, 7.0, qRgb(0, 0, 0), "Continuous", "Default", True)
        ## addLayer("8", True, False, 8.0, qRgb(0, 0, 0), "Continuous", "Default", True)
        ## addLayer("9", True, False, 9.0, qRgb(0, 0, 0), "Continuous", "Default", True)
        addLayer = self.addLayer
        [addLayer("%s" % i, True, False, float(i), qRgb(0, 0, 0), "Continuous", "Default", True)
            for i in range(0, 10)]

        ## for i in range(0, layerModel.columnCount()):
        ##     treeView.resizeColumnToContents(i)
        localtreeViewMeth = treeView.resizeColumnToContents
        [localtreeViewMeth(i)
            for i in range(0, layerModel.columnCount())]

        QApplication.setOverrideCursor(Qt.ArrowCursor)


    def __del__(self):
        """Class deconstructor."""
        QApplication.restoreOverrideCursor()

    def addLayer(self, name, visible, frozen, zValue, color, lineType, lineWeight, print_):
        """
        TOWRITE for :class:`LayerManager`.

        :param `name`: TOWRITE
        :type `name`: QString
        :param `visible`: TOWRITE
        :type `visible`: bool
        :param `frozen`: TOWRITE
        :type `frozen`: bool
        :param `zValue`: TOWRITE
        :type `zValue`: qreal
        :param `color`: TOWRITE
        :type `color`: QRgb
        :param `lineType`: TOWRITE
        :type `lineType`: QString
        :param `lineWeight`: TOWRITE
        :type `lineWeight`: QString
        :param `print_`: TOWRITE
        :type `print_`: bool
        """

        self.layerModel.insertRow(0)
        self.layerModel.setData(self.layerModel.index(0, 0), name)
        self.layerModel.setData(self.layerModel.index(0, 1), visible)
        self.layerModel.setData(self.layerModel.index(0, 2), frozen)
        self.layerModel.setData(self.layerModel.index(0, 3), zValue)

        colorPix = QPixmap(QSize(16, 16))
        colorPix.fill(QColor(color))
        self.layerModel.itemFromIndex(self.layerModel.index(0, 4)).setIcon(QIcon(colorPix))
        self.layerModel.setData(self.layerModel.index(0, 4), QColor(color))

        self.layerModel.setData(self.layerModel.index(0, 5), lineType)
        self.layerModel.setData(self.layerModel.index(0, 6), lineWeight)
        self.layerModel.setData(self.layerModel.index(0, 7), print_)


# kate: bom off; indent-mode python; indent-width 4; replace-trailing-space-save on;
