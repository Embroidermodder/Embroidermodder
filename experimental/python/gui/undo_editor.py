#!/usr/bin/env python
# -*- coding: utf-8 -*-

"""
===============================
|module_summary| undo_editor.py
===============================

TOWRITE
"""

## TODO List:
## 001. Remove c++ port imports when finally working.
## 002. Documentation.
## 003. PEP8
##
## #include <QUndoGroup>
## #include <QUndoStack>
## #include <QUndoView>
## #include <QKeyEvent>
##
## #include "undo-editor.h"
## #include "undo-commands.h"

#-Imports.---------------------------------------------------------------------
#--Python Imports.
import os

#--PySide/PyQt Imports.
if PYSIDE:
    from PySide.QtCore import qDebug, Qt, Slot
    from PySide.QtGui import (QDockWidget, QIcon, QKeyEvent,
                              QUndoGroup, QUndoStack, QUndoView)
elif PYQT4:
    import sip
    sip.setapi('QString', 2)
    sip.setapi('QVariant', 2)
    from PyQt4.QtCore import qDebug, Qt
    from PyQt4.QtCore import pyqtSlot as Slot
    from PyQt4.QtGui import (QDockWidget, QIcon, QKeyEvent,
                             QUndoGroup, QUndoStack, QUndoView)


class UndoEditor(QDockWidget):
    """
    Subclass of `QDockWidget`_

    TOWRITE

    .. sphinx_generate_methods_summary::
       UndoEditor
    """
    def __init__(self, iconDirectory="", widgetToFocus=None, parent=None,
                 flags=Qt.WindowFlags()):
        """
        TOWRITE

        :param `iconDirectory`: TOWRITE
        :type `iconDirectory`: QString
        :param `widgetToFocus`: TOWRITE
        :type `widgetToFocus`: `QWidget`_
        :param `parent`: Pointer to a parent widget instance.
        :type `parent`: `QWidget`_
        :param `flags`: TOWRITE
        :type `flags`: Qt.WindowFlags
        """
        super(UndoEditor, self).__init__(parent, flags)

        self.iconDir = iconDirectory
        self.iconSize = 16
        self.setMinimumSize(100, 100)

        self.undoGroup = undoGroup = QUndoGroup(self)
        self.undoView = undoView = QUndoView(undoGroup, self)
        undoView.setEmptyLabel(self.tr("New"))
        undoView.setCleanIcon(QIcon(self.iconDir + os.sep + "new.png")) # TODO: new.png for new drawings, open.png for opened drawings, save.png for saved/cleared drawings?

        self.setWidget(undoView)
        self.setWindowTitle(self.tr("History"))
        self.setAllowedAreas(Qt.LeftDockWidgetArea | Qt.RightDockWidgetArea)

        self.setFocusProxy(widgetToFocus)
        undoView.setFocusProxy(widgetToFocus)


    def __del__(self):
        """Class destructor."""
        qDebug("UndoEditor Destructor()")

    def addStack(self, stack):
        """
        TOWRITE

        :param `stack`: TOWRITE
        :type `stack`: `QUndoStack`_
        """
        self.undoGroup.addStack(stack)

    def canUndo(self):
        """TOWRITE"""
        return self.undoGroup.canUndo()

    def canRedo(self):
        """TOWRITE"""
        return self.undoGroup.canRedo()

    def undoText(self):
        """TOWRITE"""
        return self.undoGroup.undoText()

    def redoText(self):
        """TOWRITE"""
        return self.undoGroup.redoText()

    # Slots ------------------------------------------------------------------

    @Slot()
    def undo(self):
        """TOWRITE"""
        self.undoGroup.undo()

    @Slot()
    def redo(self):
        """TOWRITE"""
        self.undoGroup.redo()

    @Slot(bool)
    def updateCleanIcon(self, opened):
        """
        TOWRITE

        :param `opened`: TOWRITE
        :type `opened`: bool
        """
        undoView = self.undoView
        if opened:
            undoView.setEmptyLabel(self.tr("Open"))
            undoView.setCleanIcon(QIcon(self.iconDir + os.sep + "open.png"))
        else:
            undoView.setEmptyLabel(self.tr("New"))
            undoView.setCleanIcon(QIcon(self.iconDir + os.sep + "new.png"))


# kate: bom off; indent-mode python; indent-width 4; replace-trailing-space-save on;
