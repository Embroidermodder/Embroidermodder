#!/usr/bin/env python
# -*- coding: utf-8 -*-

"""
embdetails_dialog.py

TOWRITE
"""

## TODO List:
## 001. Remove c++ port imports when finally working.
## 002. Documentation.
## 003. PEP8
##
## #include "embdetails-dialog.h"
## #include "emb-pattern.h"
## #include "emb-reader-writer.h"


#-Imports.---------------------------------------------------------------------
#--Python Imports.
## import os

#--PySide/PyQt Imports.
try:
    ## from PySide import QtCore, QtGui
    # or... Improve performace with less dots...
    from PySide.QtCore import qDebug, Qt, SIGNAL, SLOT, QRect
    from PySide.QtGui import QApplication, QDialog, QGridLayout, QLabel, \
        QDialogButtonBox, QGroupBox, QScrollArea, QVBoxLayout, QWidget
    PYSIDE = True
    PYQT4 = False
except ImportError:
    raise
#    ## from PyQt4 import QtCore, QtGui
#    # or... Improve performace with less dots...
#    from PyQt4.QtCore import qDebug, Qt, SIGNAL, SLOT, QRect
#    from PyQt4.QtGui import QApplication, QDialog, QGridLayout, QLabel, \
#        QDialogButtonBox, QGroupBox, QScrollArea, QVBoxLayout, QWidget
#    PYSIDE = False
#    PYQT4 = True


class EmbDetailsDialog(QDialog):
    """
    Class implementing the Details dialog.
    """
    def __init__(self, theScene, parent):
        """
        Default class constructor.

        :param `theScene`: TOWRITE
        :type `theScene`: QGraphicsScene
        :param `parent`: parent widget instance of this dialog.
        :type `parent`: QWidget
        """
        super(EmbDetailsDialog, self).__init__(parent)

        self.setMinimumSize(750, 550)

        self.getInfo()
        mainWidget = self.createMainWidget()

        buttonBox = QDialogButtonBox(QDialogButtonBox.Ok)
        buttonBox.accepted.connect(self.accept)

        vboxLayoutMain = QVBoxLayout(self)
        vboxLayoutMain.addWidget(mainWidget)
        vboxLayoutMain.addWidget(buttonBox)
        self.setLayout(vboxLayoutMain)

        self.setWindowTitle(self.tr("Embroidery Design Details"))

        QApplication.setOverrideCursor(Qt.ArrowCursor)


    def __del__(self):
        """Class destructor"""
        QApplication.restoreOverrideCursor()

    def getInfo(self):
        """TOWRITE"""
        # TODO: generate a temporary pattern from the scene data.

        # TODO: grab this information from the pattern
        self.stitchesTotal = 5 # TODO: embStitchList_count(pattern->stitchList, TOTAL)
        self.stitchesReal  = 4 # TODO: embStitchList_count(pattern->stitchList, NORMAL)
        self.stitchesJump  = 3 # TODO: embStitchList_count(pattern->stitchList, JUMP)
        self.stitchesTrim  = 2 # TODO: embStitchList_count(pattern->stitchList, TRIM)
        self.colorTotal    = 1 # TODO: embThreadList_count(pattern->threadList, TOTAL)
        self.colorChanges  = 0 # TODO: embThreadList_count(pattern->threadList, CHANGES)

        self.boundingRect = QRect() #TODO/FIXME#
        self.boundingRect.setRect(0, 0, 50, 100) # TODO: embPattern_calcBoundingBox(pattern)

    def createMainWidget(self):
        """
        TOWRITE

        :return: TOWRITE
        :rtype: QScrollArea
        """
        widget = QWidget(self)

        # Misc
        groupBoxMisc = QGroupBox(self.tr("General Information"), widget)

        labelStitchesTotal = QLabel(self.tr("Total Stitches:"), self)
        labelStitchesReal  = QLabel(self.tr("Real Stitches:"),  self)
        labelStitchesJump  = QLabel(self.tr("Jump Stitches:"),  self)
        labelStitchesTrim  = QLabel(self.tr("Trim Stitches:"),  self)
        labelColorTotal    = QLabel(self.tr("Total Colors:"),   self)
        labelColorChanges  = QLabel(self.tr("Color Changes:"),  self)
        labelRectLeft      = QLabel(self.tr("Left:"),           self)
        labelRectTop       = QLabel(self.tr("Top:"),            self)
        labelRectRight     = QLabel(self.tr("Right:"),          self)
        labelRectBottom    = QLabel(self.tr("Bottom:"),         self)
        labelRectWidth     = QLabel(self.tr("Width:"),          self)
        labelRectHeight    = QLabel(self.tr("Height:"),         self)

        fieldStitchesTotal = QLabel(u'%s' % (self.stitchesTotal), self)
        fieldStitchesReal  = QLabel(u'%s' % (self.stitchesReal),  self)
        fieldStitchesJump  = QLabel(u'%s' % (self.stitchesJump),  self)
        fieldStitchesTrim  = QLabel(u'%s' % (self.stitchesTrim),  self)
        fieldColorTotal    = QLabel(u'%s' % (self.colorTotal),    self)
        fieldColorChanges  = QLabel(u'%s' % (self.colorChanges),  self)
        fieldRectLeft      = QLabel(u'%s' % (str(self.boundingRect.left())   + " mm"), self)
        fieldRectTop       = QLabel(u'%s' % (str(self.boundingRect.top())    + " mm"), self)
        fieldRectRight     = QLabel(u'%s' % (str(self.boundingRect.right())  + " mm"), self)
        fieldRectBottom    = QLabel(u'%s' % (str(self.boundingRect.bottom()) + " mm"), self)
        fieldRectWidth     = QLabel(u'%s' % (str(self.boundingRect.width())  + " mm"), self)
        fieldRectHeight    = QLabel(u'%s' % (str(self.boundingRect.height()) + " mm"), self)

        gridLayoutMisc = QGridLayout(groupBoxMisc)
        gridLayoutMisc.addWidget(labelStitchesTotal,  0, 0, Qt.AlignLeft)
        gridLayoutMisc.addWidget(labelStitchesReal,   1, 0, Qt.AlignLeft)
        gridLayoutMisc.addWidget(labelStitchesJump,   2, 0, Qt.AlignLeft)
        gridLayoutMisc.addWidget(labelStitchesTrim,   3, 0, Qt.AlignLeft)
        gridLayoutMisc.addWidget(labelColorTotal,     4, 0, Qt.AlignLeft)
        gridLayoutMisc.addWidget(labelColorChanges,   5, 0, Qt.AlignLeft)
        gridLayoutMisc.addWidget(labelRectLeft,       6, 0, Qt.AlignLeft)
        gridLayoutMisc.addWidget(labelRectTop,        7, 0, Qt.AlignLeft)
        gridLayoutMisc.addWidget(labelRectRight,      8, 0, Qt.AlignLeft)
        gridLayoutMisc.addWidget(labelRectBottom,     9, 0, Qt.AlignLeft)
        gridLayoutMisc.addWidget(labelRectWidth,     10, 0, Qt.AlignLeft)
        gridLayoutMisc.addWidget(labelRectHeight,    11, 0, Qt.AlignLeft)
        gridLayoutMisc.addWidget(fieldStitchesTotal,  0, 1, Qt.AlignLeft)
        gridLayoutMisc.addWidget(fieldStitchesReal,   1, 1, Qt.AlignLeft)
        gridLayoutMisc.addWidget(fieldStitchesJump,   2, 1, Qt.AlignLeft)
        gridLayoutMisc.addWidget(fieldStitchesTrim,   3, 1, Qt.AlignLeft)
        gridLayoutMisc.addWidget(fieldColorTotal,     4, 1, Qt.AlignLeft)
        gridLayoutMisc.addWidget(fieldColorChanges,   5, 1, Qt.AlignLeft)
        gridLayoutMisc.addWidget(fieldRectLeft,       6, 1, Qt.AlignLeft)
        gridLayoutMisc.addWidget(fieldRectTop,        7, 1, Qt.AlignLeft)
        gridLayoutMisc.addWidget(fieldRectRight,      8, 1, Qt.AlignLeft)
        gridLayoutMisc.addWidget(fieldRectBottom,     9, 1, Qt.AlignLeft)
        gridLayoutMisc.addWidget(fieldRectWidth,     10, 1, Qt.AlignLeft)
        gridLayoutMisc.addWidget(fieldRectHeight,    11, 1, Qt.AlignLeft)
        gridLayoutMisc.setColumnStretch(1,1)
        groupBoxMisc.setLayout(gridLayoutMisc)

        # TODO: Color Histogram

        # Stitch Distribution
        # groupBoxDist = QGroupBox(self.tr("Stitch Distribution"), widget)

        # TODO: Stitch Distribution Histogram

        # Widget Layout
        vboxLayoutMain = QVBoxLayout(widget)
        vboxLayoutMain.addWidget(groupBoxMisc)
        # vboxLayoutMain.addWidget(groupBoxDist)
        vboxLayoutMain.addStretch(1)
        widget.setLayout(vboxLayoutMain)

        scrollArea = QScrollArea(self)
        scrollArea.setWidgetResizable(True)
        scrollArea.setWidget(widget)
        return scrollArea


# kate: bom off; indent-mode cstyle; indent-width 4; replace-trailing-space-save on;
