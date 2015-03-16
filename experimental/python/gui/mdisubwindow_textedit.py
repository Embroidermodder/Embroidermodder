#!/usr/bin/env python
# -*- coding: utf-8 -*-

"""
=========================================
|module_summary| mdisubwindow_textedit.py
=========================================

TOWRITE

Classes summary:
================

================================ ============================
:class:`~MDISubWindow_TextEdit`  TOWRITE
================================ ============================

-------------------------------------------------------------
"""

#-Imports.----------------------------------------------------------------------

#--Python Imports.
import os

#--PySide/PyQt Imports.
try:
    ## from PySide import QtCore, QtGui
    # or... Improve performace with less dots...
    # Only import what we need into the global namespace
    from PySide.QtCore import qDebug, Qt, QTextStream, QFile
    from PySide.QtGui import QApplication, QAbstractSlider, QAction, QBrush, QColor, \
        QFileDialog, QFont, QIcon, QKeySequence, QMenu, QMessageBox, \
        QPalette, QPixmap, QPrinter, QPrintDialog, QTextEdit, QTextOption, QToolButton, QToolTip
    PYSIDE = True
    PYQT4 = False
except ImportError:
    raise
#    ## from PyQt4 import QtCore, QtGui
#    # or... Improve performace with less dots...
#    # Only import what we need into the global namespace
#    from PyQt4.QtCore import qDebug, Qt, QTextStream, QFile
#    from PyQt4.QtGui import QApplication, QAbstractSlider, QAction, QBrush, QColor, \
#        QFileDialog, QFont, QIcon, QKeySequence, QMenu, QMessageBox, \
#        QPalette, QPixmap, QPrinter, QPrintDialog, QTextEdit, QTextOption, QToolButton, QToolTip
#    PYSIDE = False
#    PYQT4 = True


#--Local Imports.
from KeyboardShortcuts import *
from emb_globals import *


def initGlobals(MainWin):
    """"""
    global gMainWin
    gMainWin = MainWin

    global gIconDir
    gIconDir = gMainWin.gIconDir

    global gImgDir
    gImgDir = gMainWin.gImgDir


class MDISubWindow_TextEdit(QTextEdit):
    """
    Subclass of `QTextEdit`_

    Text Editor MDISubwindow for Embroidermodder.

    .. sphinx_generate_methods_summary::
       MDISubWindow_TextEdit
    """
    documentCount = 1
    def __init__(self, parent=None, filePath=''):
        """
        Default class constructor.

        :param `parent`: Pointer to a parent widget instance.
        :type `parent`: `QWidget`_
        :param `filePath`: Filepath to open in the text editor.
        :type `filePath`: str
        """
        super(MDISubWindow_TextEdit, self).__init__(parent)

        qDebug(filePath)
        self.filePath = filePath
        self.fileName = os.path.basename(os.path.abspath(filePath))
        self.fileExt = os.path.splitext(self.fileName)[1]

        if not filePath:
            self.setWindowTitle('Untitled[*]')
            parent.setWindowIcon(QIcon(gIconDir + os.sep + 'new.png'))
        else:
            f = open(filePath, 'r')
            txt = f.read()
            f.close()
            self.setText(txt)

            self.setWindowTitle('%s[*]' % self.fileName)

            # TODO: This is changing all the subwindows icons. We want individual icons based on filetype...?
            if self.fileExt in ('.py', '.pyw'):
                parent.setWindowIcon(QIcon(gIconDir + os.sep + 'pyscript.png'))
            else:
                parent.setWindowIcon(QIcon(gIconDir + os.sep + 'new.png'))

        monoFont = QFont('Courier New')
        monoFont.setFixedPitch(True)
        self.setFont(monoFont)
        self.setWordWrapMode(QTextOption.NoWrap)
        ## self.setTextBackgroundColor(QColor('#000000'))
        ## self.setTextColor(QColor('#FFFFFF'))
        self.setCursorWidth(2)
        self.CreateActions()

        self.zoomLevel = 0

        gMainWin.action_Edit_Cut.setEnabled(False)
        gMainWin.action_Edit_Copy.setEnabled(False)
        self.copyAvailable.connect(gMainWin.action_Edit_Cut.setEnabled)
        self.copyAvailable.connect(gMainWin.action_Edit_Copy.setEnabled)
        # Handle the Context Menu Entries Also.
        self.action_Edit_Undo.setEnabled(False)
        self.action_Edit_Redo.setEnabled(False)
        self.action_Edit_Cut.setEnabled(False)
        self.action_Edit_Copy.setEnabled(False)
        self.undoAvailable.connect(self.action_Edit_Undo.setEnabled)
        self.redoAvailable.connect(self.action_Edit_Redo.setEnabled)
        self.copyAvailable.connect(self.action_Edit_Cut.setEnabled)
        self.copyAvailable.connect(self.action_Edit_Copy.setEnabled)

        self.document().contentsChanged.connect(self.TheDocumentWasModified)

        # self.show()
        # self.showMaximized()
        # self.setFocusPolicy(Qt.WheelFocus)
        ##### self.setFocus() # This makes the MDIArea go into SubWindowView...?

        self.gCornerWidget = QToolButton(self)
        # self.gCornerWidgetToolTip = QToolTip()
        QToolTip.setFont(QFont('SansSerif', 10))
        br = QBrush(QPixmap(gImgDir + os.sep + 'texture-spirals.png'))
        pal = QPalette()
        # pal.setBrush(QPalette.Active, QPalette.Base, br)
        # pal.setColor(QPalette.Background, QColor('#FF8000'))
        pal.setColor(QPalette.ColorGroup.Inactive,
                     QPalette.ColorRole.ToolTipBase,
                     QColor(EMBROIDERBLUE2))
        QToolTip.setPalette(QPalette(pal))
        self.action_Edit_SelectAll.setToolTip('This is a <b>QWidget</b> widget')

        self.gCornerWidget.setDefaultAction(self.action_Edit_SelectAll)
        self.setCornerWidget(self.gCornerWidget)
        # We want to show the corner widget no matter what so...
        self.setHorizontalScrollBarPolicy(Qt.ScrollBarAlwaysOn)
        self.setVerticalScrollBarPolicy(Qt.ScrollBarAlwaysOn)



    def CreateActions(self):
        """"""
        self.action_Edit_Undo = QAction(QIcon(gIconDir + os.sep + 'undo.png'),
                self.tr('&Undo'), self,
                shortcut=UndoShortcut, # QKeySequence.Undo
                statusTip=self.tr('Reverses the most recent action.'),
                triggered=self.undo)

        self.action_Edit_Redo = QAction(QIcon(gIconDir + os.sep + 'redo.png'),
                self.tr('&Redo'), self,
                shortcut=RedoShortcut, # QKeySequence.Redo
                statusTip=self.tr('Reverses the effects of the previous undo action.'),
                triggered=self.redo)

        self.action_Edit_Cut = QAction(QIcon(gIconDir + os.sep + 'cut.png'),
                self.tr('Cu&t'), self,
                shortcut=CutShortcut, # QKeySequence.Cut
                statusTip=self.tr('Cut the current selection\'s contents to the clipboard.'),
                triggered=self.cut)

        self.action_Edit_Copy = QAction(QIcon(gIconDir + os.sep + 'copy.png'),
                self.tr('&Copy'), self,
                shortcut=CopyShortcut, # QKeySequence.Copy
                statusTip=self.tr('Copy the current selection\'s contents to the clipboard.'),
                triggered=self.copy)

        self.action_Edit_Paste = QAction(QIcon(gIconDir + os.sep + 'paste.png'),
                self.tr('&Paste'), self,
                shortcut=PasteShortcut, # QKeySequence.Paste
                statusTip=self.tr('Paste the clipboard\'s contents into the current selection.'),
                triggered=self.paste)

        self.action_Edit_Delete = QAction(QIcon(gIconDir + os.sep + 'delete.png'),
                self.tr('&Delete'), self,
                # shortcut=DeleteShortcut, # QKeySequence.Delete
                statusTip=self.tr('Delete selection.'),
                triggered=gMainWin.OnTODO)
                # triggered=self.delete)

        self.action_Edit_SelectAll = QAction(QIcon(gIconDir + os.sep + 'selectall.png'),
                self.tr('&Select All'), self,
                # shortcut=SelectAllShortcut, # QKeySequence.SelectAll
                statusTip=self.tr('Select all text in the document.'),
                # toolTip=QToolTip.text('Select All'),
                triggered=self.selectAll)

        self.action_Edit_ZoomIn = QAction(QIcon(gIconDir + os.sep + 'zoomin.png'),
                self.tr('Zoom &In'), self,
                shortcut=QKeySequence.ZoomIn, # ZoomInShortcut,
                statusTip=self.tr('Zoom In'),
                triggered=self.DoZoomIn)

        self.action_Edit_ZoomOut = QAction(QIcon(gIconDir + os.sep + 'zoomout.png'),
                self.tr('Zoom &Out'), self,
                shortcut=QKeySequence(Qt.CTRL + Qt.Key_Plus), #QKeySequence.ZoomOut, # ZoomOutShortcut,
                statusTip=self.tr('Zoom Out'),
                triggered=self.DoZoomOut)

        self.action_Edit_ZoomDefault = QAction(QIcon(gIconDir + os.sep + 'zoom.png'),
                self.tr('Zoom &Default'), self,
                shortcut=ZoomDefaultShortcut,
                statusTip=self.tr('Zoom Default'),
                triggered=self.DoZoomDefault)



    def TheDocumentWasModified(self):
        """"""
        self.setWindowModified(self.document().isModified())

    # def focusInEvent(self, event):
        # event.accept()
        # event.ignore()
        # print('focusInEvent - ')

    def AskToSaveBeforeClosing(self):
        """"""
        if self.document().isModified():
            ret = QMessageBox.warning(self, 'Warning',
                    'The document has been modified.' '\n'
                    'Do you want to save your changes?',
                    QMessageBox.Save | QMessageBox.Discard |
                    QMessageBox.Cancel)
            if ret == QMessageBox.Save:
                return self.OnSave()
            elif ret == QMessageBox.Cancel:
                if gMainWin.isEmbroidermodderMainWindowClosing:
                    gMainWin.isEmbroidermodderMainWindowClosing = False # Cancel closing the app.
                return False
        return True

    def OnSave(self):
        """"""
        if self.filePath:
            return self.OnSaveFilePath(self.filePath)

        return self.OnSaveAs()

    def OnSaveAs(self):
        """"""
        filePath, filtr = QFileDialog.getSaveFileName(self,
                                                      self.tr('Save As...'),
                                                      os.getcwd(),
                                                      WILDCARD_ALL)
        if filePath:
            return self.OnSaveFilePath(filePath)

        return False

    def OnSaveFilePath(self, filePath):
        """"""
        file = QFile(filePath)
        if not file.open(QFile.WriteOnly | QFile.Text):
            QMessageBox.warning(self, self.tr('Warning'),
                    self.tr('Cannot write file') + ' %s:\n%s.' % (filePath, file.errorString()))
            return False

        outf = QTextStream(file)
        QApplication.setOverrideCursor(Qt.WaitCursor)
        outf << self.toPlainText()
        QApplication.restoreOverrideCursor()

        self.DoSetCurrentFilePath(filePath)

        # Clear the Modified Flag.
        self.document().setModified(False)
        self.setWindowModified(False)
        self.setWindowTitle('%s[*]' % self.fileName)

        return True

    def OnPrint(self):
        """"""
        document = self.document()
        printer = QPrinter()

        dlg = QPrintDialog(printer, self)
        if dlg.exec_() != QDialog.Accepted:
            return

        document.print_(printer)

    def contextMenuEvent(self, event):
        """
        Handles the ``contextMenuEvent`` event for :class:`MDISubWindow_TextEdit`.

        :param `event`: A `QContextMenuEvent`_ to be processed.
        """
        menu = QMenu(self)
        menu.addAction(self.action_Edit_Undo)
        menu.addAction(self.action_Edit_Redo)
        menu.addSeparator()
        menu.addAction(self.action_Edit_Cut)
        menu.addAction(self.action_Edit_Copy)
        menu.addAction(self.action_Edit_Paste)
        menu.addAction(self.action_Edit_Delete)
        menu.addSeparator()
        menu.addAction(self.action_Edit_SelectAll)
        menu.popup(self.mapToGlobal(event.pos()))
        # menu.exec_(self.mapToGlobal(event.pos()))
        event.accept()
        print("MDISubWindow_TextEdit contextMenuEvent")

    def DoZoomOut(self):
        """"""
        self.zoomOut()
        self.zoomLevel = self.zoomLevel - 1

    def DoZoomIn(self):
        """"""
        self.zoomIn()
        self.zoomLevel = self.zoomLevel + 1

    def GetZoomLevel(self):
        """"""
        return self.zoomLevel

    def DoZoomDefault(self):
        """"""
        if self.zoomLevel: # not == 0
            if self.zoomLevel < 0:
                while self.GetZoomLevel():
                    self.DoZoomIn()
            else:
                while self.GetZoomLevel():
                    self.DoZoomOut()

    def OnDay(self):
        """"""
        self.setTextBackgroundColor(QColor('#FFFFFF'))
        self.setTextColor(QColor('#000000'))
        print('OnDay')

    def OnNight(self):
        """"""
        self.setTextBackgroundColor(QColor('#000000'))
        # self.setTextColor(QColor('#FFFFFF'))

        # sample_palette = QPalette()
        # sample_palette.setColor(QPalette.Window, Qt.white)
        # sample_palette.setColor(QPalette.WindowText, Qt.blue)

        # self.setAutoFillBackground(True)
        # self.setPalette(sample_palette)
        self.document().setDefaultStyleSheet("QTextEdit {color: white; background-color: black}")
        # self.setStyleSheet("color: white; background-color: black")
        # self.setStyleSheet("color: white;"
                           # "background-color: black;"
                           # "selection-color: black;"
                           # "selection-background-color: white;")
        # self.setExtraSelections([(0, 1)])

        # self.setAutoFillBackground(True)
        # self.setBackgroundRole(QPalette.Background)
        self.update()
        print('OnNight')

    def wheelEvent(self, event):
        """
        Handles the ``wheelEvent`` event for :class:`MDISubWindow_TextEdit`.

        :param `event`: A `QWheelEvent`_ to be processed.
        """
        evtDelta = event.delta()
        evtModifiers = event.modifiers()
        evtAltDown = evtModifiers & Qt.AltModifier
        evtShiftDown = evtModifiers & Qt.ShiftModifier
        evtCtrlDown = evtModifiers & Qt.ControlModifier
        ## print('evtModifiers = %s' % evtModifiers)
        ## print('evtAltDown = %s' % evtAltDown)
        ## print('evtShiftDown = %s' % evtShiftDown)
        ## print('evtCtrlDown = %s' % evtCtrlDown)
        ## print('event.delta() = %s' % evtDelta)
        # Ctrl+MouseWheel Zooming.
        if (evtDelta < 0) and evtCtrlDown and not evtShiftDown and not evtAltDown:
            if self.zoomLevel > -7:
                self.DoZoomOut()
                print('zoomLevel = %d' % self.zoomLevel)
        elif (evtDelta > 0) and evtCtrlDown and not evtShiftDown and not evtAltDown:
            if self.zoomLevel < 20:
                self.DoZoomIn()
                print('zoomLevel = %d' % self.zoomLevel)
        # Shift+MouseWheel Horizontal Scrolling.
        elif (evtDelta < 0) and not evtCtrlDown and evtShiftDown and not evtAltDown:
            self.horizontalScrollBar().triggerAction(QAbstractSlider.SliderSingleStepAdd)
            self.horizontalScrollBar().triggerAction(QAbstractSlider.SliderSingleStepAdd)
        elif (evtDelta > 0) and not evtCtrlDown and evtShiftDown and not evtAltDown:
            self.horizontalScrollBar().triggerAction(QAbstractSlider.SliderSingleStepSub)
            self.horizontalScrollBar().triggerAction(QAbstractSlider.SliderSingleStepSub)
        # Reproduce Regular Scrolling.
        elif evtDelta < 0:
            self.verticalScrollBar().triggerAction(QAbstractSlider.SliderSingleStepAdd)
            self.verticalScrollBar().triggerAction(QAbstractSlider.SliderSingleStepAdd)
        elif evtDelta > 0:
            self.verticalScrollBar().triggerAction(QAbstractSlider.SliderSingleStepSub)
            self.verticalScrollBar().triggerAction(QAbstractSlider.SliderSingleStepSub)

        event.accept()
        # event.ignore()

        # numDegrees = event.delta() / 8
        # numSteps = numDegrees / 15
        # print('numDegrees = %s' % numDegrees)
        # print('numSteps = %s' % numSteps)

        #if event.orientation() == Qt.Horizontal:
        #    self.scroll(numSteps, 0)
        #    # self.scrollHorizontally(numSteps)
        #else:
        #    self.scroll(0, numSteps)
        #    # self.scrollVertically(numSteps)
        #event.accept()

    def DoSetCurrentFilePath(self, filePath):
        """"""
        self.filePath = str(filePath)
        self.fileName = os.path.basename(os.path.abspath(filePath))
        self.fileExt = os.path.splitext(self.fileName)[1]

    def closeEvent(self, event):
        """
        Handles the ``closeEvent`` event for :class:`MDISubWindow_TextEdit`.

        :param `event`: A `QCloseEvent`_ to be processed.
        """
        if self.AskToSaveBeforeClosing():
            # self.DoWriteSettings()
            event.accept()
        else:
            event.ignore()

        # reply = QMessageBox.question(self, 'Message',
        #     "Are you sure to close this Window?", QMessageBox.Yes, QMessageBox.No)
        #
        # if reply == QMessageBox.Yes:
        #     event.accept()
        # else:
        #     event.ignore()


# kate: bom off; indent-mode python; indent-width 4; replace-trailing-space-save on;
