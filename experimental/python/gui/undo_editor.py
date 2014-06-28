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
try:
    from PySide.QtCore import qDebug, Qt
    from PySide.QtGui import QDockWidget, QIcon, QKeyEvent, \
                             QUndoGroup, QUndoStack, QUndoView
except ImportError:
    raise
    # from PyQt4.QtCore import qDebug, Qt
    # from PyQt4.QtGui import QDockWidget, QIcon, QKeyEvent, \
    #                         QUndoGroup, QUndoStack, QUndoView


class UndoEditor(QDockWidget):
    """
    Subclass of `QDockWidget`_

    TOWRITE

    .. sphinx_generate_methods_summary::
       UndoEditor
    """
    def __init__(self, iconDirectory, widgetToFocus, parent=None,
                 flags=Qt.WindowFlags()):
        """
        TOWRITE

        :param `iconDirectory`: TOWRITE
        :type `iconDirectory`: QString
        :param `widgetToFocus`: TOWRITE
        :type `widgetToFocus`: QWidget
        :param `parent`: TOWRITE
        :type `parent`: QWidget
        :param `flags`: TOWRITE
        :type `flags`: Qt.WindowFlags
        """
        super(UndoEditor, self).__init__(parent, flags)

        self.iconDir = iconDirectory
        self.iconSize = 16
        self.setMinimumSize(100, 100)

        self.undoGroup = QUndoGroup(self)
        self.undoView = QUndoView(self.undoGroup, self)
        self.undoView.setEmptyLabel(self.tr("New"))
        self.undoView.setCleanIcon(QIcon(self.iconDir + os.sep + "new.png")) # TODO: new.png for new drawings, open.png for opened drawings, save.png for saved/cleared drawings?

        self.setWidget(self.undoView)
        self.setWindowTitle(self.tr("History"))
        self.setAllowedAreas(Qt.LeftDockWidgetArea | Qt.RightDockWidgetArea)

        self.setFocusProxy(widgetToFocus)
        self.undoView.setFocusProxy(widgetToFocus)


    def addStack(self, stack):
        """
        TOWRITE

        :param `stack`: TOWRITE
        :type `stack`: QUndoStack
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

    def undo(self):
        """TOWRITE"""
        self.undoGroup.undo()

    def redo(self):
        """TOWRITE"""
        self.undoGroup.redo()


# kate: bom off; indent-mode cstyle; indent-width 4; replace-trailing-space-save on;
