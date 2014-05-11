#!/usr/bin/env python
# -*- coding: utf-8 -*-

"""
Embroidermodder_PySide_PyQt_GUI.py
==================================

"""

#-Imports.----------------------------------------------------------------------
#--Python Imports.
import os
import sys
import webbrowser

if sys.version_info[0] == 2:
    PY2 = True
    PY3 = False
elif sys.version_info[0] == 3:
    PY2 = False
    PY3 = True

## if PY2:
##     # This is only needed for Python v2 but is harmless for Python v3.
##     import sip
##     sip.setapi('QVariant', 2)

#--PySide/PyQt Imports.
try:
    ## from PySide import QtCore, QtGui
    # or... Improve performace with less dots...
    # Only import what we need into the global namespace
    ## import causeIntentionalError
    from PySide.QtCore import __version_info__ as QtCore__version_info__
    from PySide.QtCore import __version__ as QtCore__version__
    from PySide.QtCore import qDebug, QFile, QFileInfo, QPoint, QPointF, QSettings, QSize, \
        QTextStream, SIGNAL, QSignalMapper, QTimer, QTranslator, QIODevice, QEvent, QObject, Qt
    from PySide.QtGui import qRgb, QAbstractSlider, QAction, QActionGroup, QApplication, QBitmap, QBrush, \
        QColor, QComboBox, QDialog, QDockWidget, QFileDialog, \
        QFont, QFontComboBox, QFrame, QGradient, QLinearGradient, QHBoxLayout, QIcon, QKeySequence, QLabel, \
        QLineEdit, QListWidget, QMainWindow, QMdiArea, QMdiSubWindow, QMenu, QMessageBox, \
        QPainter, QPixmap, QPrintDialog, QSplitter, QSplitterHandle, \
        QTabWidget, QTextBrowser, QTextEdit, QTextOption, QToolBar, QToolButton, \
        QStatusBar, QVBoxLayout, QWidget, QWidgetAction, QWhatsThis, \
        QKeyEvent, QUndoGroup, QUndoStack, QUndoView, QTextCharFormat, QTextLayout, QCompleter
    PYSIDE = True
    PYQT4 = False
except ImportError:
    raise
#    ## from PyQt4 import QtCore, QtGui
#    # or... Improve performace with less dots...
#    # Only import what we need into the global namespace
#    from PyQt4.QtCore import QT_VERSION_STR as QtCore_QT_VERSION_STR
#    from PyQt4.QtCore import QT_VERSION as QtCore_QT_VERSION
#    from PyQt4.QtCore import qDebug, QFile, QFileInfo, QPoint, QPointF, QSettings, QSize, \
#        QTextStream, SIGNAL, QSignalMapper, QTimer, QTranslator, QIODevice, QEvent, QObject, Qt
#    from PyQt4.QtGui import qRgb, QAbstractSlider, QAction, QActionGroup, QApplication, QBitmap, QBrush, \
#        QColor, QComboBox, QDialog, QDockWidget, QFileDialog, \
#        QFont, QFontComboBox, QFrame, QGradient, QLinearGradient, QHBoxLayout, QIcon, QKeySequence, QLabel, \
#        QLineEdit, QListWidget, QMainWindow, QMdiArea, QMdiSubWindow, QMenu, QMessageBox, \
#        QPainter, QPixmap, QPrintDialog, QSplitter, QSplitterHandle, \
#        QTabWidget, QTextBrowser, QTextEdit, QTextOption, QToolBar, QToolButton, \
#        QStatusBar, QVBoxLayout, QWidget, QWidgetAction, QWhatsThis, \
#        QKeyEvent, QUndoGroup, QUndoStack, QUndoView, QTextCharFormat, QTextLayout, QCompleter
#    PYSIDE = False
#    PYQT4 = True


#--Local Imports.
from emb_globals import *
from KeyboardShortcuts import *

from mainwindow_settings import DoReadSettings, DoWriteSettings, settingsDialog

import mdisubwindow_textedit
from mdisubwindow_textedit import MDISubWindow_TextEdit

from cmdprompt import CmdPrompt
from property_editor import PropertyEditor
from undo_editor import UndoEditor
from statusbar import StatusBar
from settings_dialog import Settings_Dialog
from dialog_about import AboutDialog
from embdetails_dialog import EmbDetailsDialog
from tipoftheday_dialog import TipOfTheDayDialog
from hacks import ForceRepaint

#-Globals-----------------------------------------------------------------------
# Application Directories.
try:
    gFileDir = os.path.dirname(os.path.abspath(__file__))
except:
    gFileDir = os.path.dirname(os.path.abspath(sys.argv[0]))

## print(gFileDir)
## print(os.path.dirname(gFileDir))
if os.path.basename(os.path.dirname(gFileDir)) == 'python':
    # From experimental/python/gui dir
    gAppDir = os.path.abspath(gFileDir + os.sep + '..' + os.sep + '..' + os.sep + '..' + os.sep + 'embroidermodder2')
elif os.path.basename(gFileDir) == 'embroidermodder2':
    # From embroidermodder2 dir
    gAppDir = gFileDir
## print(gAppDir)
## print(os.path.basename(gAppDir))


gImgDir = gAppDir + os.sep + 'images'
gIconDir = gAppDir + os.sep + 'icons' + os.sep + 'default'
gTrDir = gAppDir + os.sep + 'translations'

gSpiralsImgPath = gImgDir + os.sep + 'texture-spirals.png'
gLogoSpiralsImgPath = gImgDir + os.sep + 'logo-spirals.png'

major, minor, micro, release = sys.version_info[0:-1]
pythonVersion = 'Python %d.%d.%d-%s'%(major, minor, micro, release)


class MDIArea(QMdiArea):
    """
    Multiple Document Interface(MDI) Area for Embroidermodder.

    Subclass of `QMdiArea`_
    """
    def __init__(self):
        """Default class constructor."""
        super(MDIArea, self).__init__()

        if PYSIDE:
            if QtCore__version_info__ >= (4, 8, 0):
                self.setTabsClosable(True)
        elif PYQT4:
            try:
                self.setTabsClosable(True)
            except Exception as exc:
                print("Riverbank didn't use standard python __version__ magic")
                raise exc

        self.useLogo = False
        self.useTexture = False
        self.useColor = False

        ## self.bgLogo = QPixmap()
        self.bgTexture = QPixmap()
        self.bgColor = QColor()

        # Brushes
        self.colorBrush = QBrush(QColor(EMBROIDERBLUE1))
        self.backgroundBrush = QBrush(QPixmap(gSpiralsImgPath))
        linearGrad = QLinearGradient(QPointF(0, 0), QPointF(400, 400))
        linearGrad.setColorAt(0, QColor(EMBROIDERBLUE1))
        linearGrad.setColorAt(1, QColor(EMBROIDERBLUE2))
        self.gradientBrush = QBrush(linearGrad)

        # Logo
        self.bgLogo = QPixmap(gLogoSpiralsImgPath)

        self.setHorizontalScrollBarPolicy(Qt.ScrollBarAsNeeded)
        self.setVerticalScrollBarPolicy(Qt.ScrollBarAsNeeded)
        self.setActivationOrder(QMdiArea.ActivationHistoryOrder)

        # self.setBackground(QBrush(Qt.cyan))

        # self.setBackground(QBrush(QPixmap(gSpiralsImgPath)))
        # self.setBackground(QBrush(QPixmap(gSpiralsImgPath).scaled(48, 48, Qt.IgnoreAspectRatio)))
        # self.setBackground(QBrush(QPixmap(gSpiralsImgPath).scaledToWidth(100)))

        # linearGrad = QLinearGradient(QPointF(0, 0), QPointF(400, 400))
        # linearGrad.setColorAt(0, QColor(EMBROIDERBLUE1))
        # linearGrad.setColorAt(1, QColor(EMBROIDERBLUE2))
        # self.setBackground(QBrush(linearGrad))

        self.DoSetBackground()
        self.DoSetTabPosition()
        self.DoSetTabShape()
        self.DoSetViewMode()
        self.DoSetDocumentMode()

        # self.setFocusPolicy(Qt.WheelFocus)
        # self.setFocus()


        # print('updatesEnabled()', self.updatesEnabled())

        self.setAcceptDrops(True)

    def useBackgroundLogo(self, use):
        """
        TOWRITE

        :param `use`: TOWRITE
        :type `use`: bool
        """
        self.useLogo = use
        ForceRepaint(self)

    def useBackgroundTexture(self, use):
        """
        TOWRITE

        :param `use`: TOWRITE
        :type `use`: bool
        """
        self.useTexture = use
        ForceRepaint(self)

    def useBackgroundColor(self, use):
        """
        TOWRITE

        :param `use`: TOWRITE
        :type `use`: bool
        """
        self.useColor = use
        ForceRepaint(self)

    def setBackgroundLogo(self, fileName):
        """
        TOWRITE

        :param `fileName`: TOWRITE
        :type `fileName`: QString
        """
        self.bgLogo.load(fileName)

        ForceRepaint(self)

    def setBackgroundTexture(self, fileName):
        """
        TOWRITE

        :param `fileName`: TOWRITE
        :type `fileName`: QString
        """
        self.bgTexture.load(fileName)

        ForceRepaint(self)

    def setBackgroundColor(self, color):
        """
        TOWRITE

        :param `color`: TOWRITE
        :type `color`: `QColor`_
        """
        if not color.isValid():
            self.bgColor = background().color()
        else:
            self.bgColor = color

        ForceRepaint(self)


    # Other defs----------------------------------------------------------------
    def dragEnterEvent(self, event):
        """
        Handles the ``dragEnterEvent`` event for :class:`MDIArea`.

        :param `event`: a `QDragEnterEvent`_ to be processed.
        """
        print('event.mimeData().hasText()  = %s' % event.mimeData().hasText())
        print('event.mimeData().hasHtml()  = %s' % event.mimeData().hasHtml())
        print('event.mimeData().hasUrls()  = %s' % event.mimeData().hasUrls())
        print('event.mimeData().hasImage() = %s' % event.mimeData().hasImage())
        print('event.mimeData().hasColor() = %s' % event.mimeData().hasColor())

        print('event.mimeData().text()  = %s' % event.mimeData().text())
        print('event.mimeData().html()  = %s' % event.mimeData().html())
        print('event.mimeData().urls()  = %s' % event.mimeData().urls())
        print('event.mimeData().imageData() = %s' % event.mimeData().imageData())
        print('event.mimeData().colorData() = %s' % event.mimeData().colorData())
        # print(event.mimeData().hasText())
        if event.mimeData().hasFormat('text/plain'):
            event.accept()
        else:
            event.ignore()

    def dropEvent(self, event):
        """
        Handles the ``dropEvent`` event for :class:`MDIArea`.

        :param `event`: a `QDropEvent`_ to be processed.
        """
        print(event.mimeData().text())
        # self.setText(event.mimeData().text())

    def paintEvent(self, event):
        """
        Handles the ``paintEvent`` event for :class:`MDIArea`.

        :param `event`: a `QPaintEvent`_ to be processed.
        """
        vport = self.viewport()
        rect = vport.rect()

        painter = QPainter(vport)
        painter.setRenderHint(painter.SmoothPixmapTransform)

        # Always fill with a solid color first
        if self.useColor:
            painter.fillRect(rect, self.colorBrush)
        else:
            painter.fillRect(rect, self.background())

        # Then overlay the texture
        if self.useTexture:
            painter.fillRect(rect, self.backgroundBrush)

        # Overlay the logo last
        if self.useLogo:
            if not len(self.subWindowList()): # Nothing is open.
                cSizeW, cSizeH = rect.width(), rect.height()
                bgLogoW, bgLogoH = self.bgLogo.width(), self.bgLogo.height()
                if bgLogoW > cSizeW:
                    # Proportional Scaling an Image.
                    newHeight = bgLogoH * cSizeW//bgLogoW
                    scaledLogo = self.bgLogo.scaled(cSizeW, newHeight)
                    painter.drawPixmap(0, cSizeH // 2 - scaledLogo.height()//2, scaledLogo)
                else:
                    painter.drawPixmap((cSizeW - bgLogoW)//2, (cSizeH - bgLogoH)//2, self.bgLogo)
            else: #
                # Center the pixmap
                dx = (rect.width() - self.bgLogo.width()) / 2
                dy = (rect.height() - self.bgLogo.height()) / 2
                painter.drawPixmap(dx, dy, self.bgLogo.width(), self.bgLogo.height(), self.bgLogo)

    def mouseDoubleClickEvent(self, event):
        """
        Handles the ``mouseDoubleClickEvent`` event for :class:`MDIArea`.

        :param `event`: A `QMouseEvent`_ to be processed.
        """
        print(event.button())
        evtBtn = event.button()
        if evtBtn == Qt.LeftButton: # return 1
            gMainWin.OnOpen()
        elif evtBtn == Qt.RightButton: # return 2
            print('DoubleRightClick')
        elif evtBtn == Qt.MiddleButton: # return 4
            print('DoubleMiddleClick')
        elif evtBtn == Qt.XButton1: # Aux1 return 8
            print('DoubleAux1Click')
        elif evtBtn == Qt.XButton2: # Aux2 return 16
            print('DoubleAux2Click')

    def mousePressEvent(self, event):
        """
        Handles the ``mousePressEvent`` event for :class:`MDIArea`.

        :param `event`: A `QMouseEvent`_ to be processed.
        """
        # print(event.button())
        evtBtn = event.button()
        if evtBtn == Qt.LeftButton: # return 1
            print('LeftClick')
        elif evtBtn == Qt.RightButton: # return 2
            print('RightClick')
        elif evtBtn == Qt.MiddleButton: # return 4
            print('MiddleClick')
        elif evtBtn == Qt.XButton1: # Aux1 return 8
            self.activatePreviousSubWindow()
            print('Aux1Click')
        elif evtBtn == Qt.XButton2: # Aux2 return 16
            self.activateNextSubWindow()
            print('Aux2Click')
        print("QMdiArea mousePressEvent")

    def mouseMoveEvent(self, event):
        """
        Handles the ``mouseMoveEvent`` event for :class:`MDIArea`.

        :param `event`: A `QMouseEvent`_ to be processed.
        """
        # Dragging while MouseButton is down.
        print("QMdiArea mouseMoveEvent")

    def mouseReleaseEvent(self, event):
        """
        Handles the ``mouseReleaseEvent`` event for :class:`MDIArea`.

        :param `event`: A `QMouseEvent`_ to be processed.
        """
        print("QMdiArea mouseReleaseEvent")

    def moveEvent(self, event):
        """
        Handles the ``moveEvent`` event for :class:`MDIArea`.

        :param `event`: A `QMoveEvent`_ to be processed.
        """
        # Dragging while MouseButton is down.
        print("QMdiArea moveEvent")

    def contextMenuEvent(self, event):
        """
        Handles the ``contextMenuEvent`` event for :class:`MDIArea`.

        :param `event`: A `QContextMenuEvent`_ to be processed.
        """

        if not len(self.subWindowList()): # Nothing is open.
            # Build a menu suitable for the startup screen.
            menu = QMenu(self)
            menu.addAction(gMainWin.action_File_New)
            menu.addAction(gMainWin.action_File_Open)
            menu.addAction(gMainWin.action_Settings_Settings)
            menu.addAction(gMainWin.action_Help_Help)
            menu.addAction(gMainWin.action_Help_About)
            menu.addAction(gMainWin.action_File_Exit)
            menu.popup(self.mapToGlobal(event.pos()))
            # menu.exec_(self.mapToGlobal(event.pos()))
        else:
            # Build a menu suitable for when the mdi workspace is open.
            gMainWin.gMenu_File.popup(self.mapToGlobal(event.pos()))

        event.accept()
        print("QMdiArea contextMenuEvent")

    def DoSetBackground(self, brush=None):
        """
        Set the background brush for :class:`MDIArea` workspace.

        :param `brush`: By default, it is a the Embroidermodder Spirals Theme Pixmap,
         but can be any brush (e.g., colors, gradients or pixmaps).
        :type `brush`: `QBrush`_
        """
        if not brush:
            self.backgroundBrush = QBrush(QPixmap(gSpiralsImgPath))
        else:
            self.backgroundBrush = brush
        self.setBackground(self.backgroundBrush)

    def DoSetTabPosition(self, position=QTabWidget.North):
        """
        Set the tab position for :class:`MDIArea`.

        :param `position`: The position of the tabs in tabbed view mode.
         Possible values for this property are QTabWidget.North (default),
         QTabWidget.South, QTabWidget.East or QTabWidget.West
        :type `position`: QTabWidget.TabPosition
        """
        self.setTabPosition(position)

    def DoSetTabShape(self, shape=QTabWidget.Rounded):
        """
        Set the tab shape for :class:`MDIArea`.

        :param `shape`: The shape of the tabs in tabbed view mode.
         Possible values for this property are QTabWidget.Rounded (default)
         or QTabWidget.Triangular
        :type `shape`: QtGui.QTabWidget.TabShape
        """
        self.setTabShape(shape)

    def DoSetViewMode(self, mode=QMdiArea.TabbedView):
        """
        Set the view mode for :class:`MDIArea`.

        :param `mode`: The way sub-windows are displayed in the QtGui.QMdiArea.
         By default, the SubWindowView is used to display tabbed-windows.
        :type `mode`: QtGui.QMdiArea.ViewMode
        """
        # QMdiArea.TabbedView
        # QMdiArea.SubWindowView
        self.setViewMode(mode)

    def DoSetDocumentMode(self, enabled=False):
        """
        Set the document mode for :class:`MDIArea`.

        :param `enabled`: Whether the tab bar is set to document mode in tabbed view mode.
         Document mode is disabled by default.
        :type `enabled`: bool
        """
        self.setDocumentMode(enabled)


# def _(translatableString):
    # return gTranslator.tr(translatableString)


class EmbroidermodderMainWindow(QMainWindow):
    """Main Frame Window for Embroidermodder application."""
    def __init__(self):
        """Default class constructor."""
        super(EmbroidermodderMainWindow, self).__init__()

        global gMainWin
        gMainWin = self

        self.gAppDir = gAppDir
        self.gIconDir = gIconDir
        self.gImgDir = gImgDir

        DoReadSettings(self)

        mdisubwindow_textedit.initGlobals(self)

        # Define a translation function.
        # TODO figure out how to set this up right. This is causing a crash on exit...?
        # global _
        # _ = self.tr
        # _ = gTranslator.tr

        self.isEmbroidermodderMainWindowClosing = False

        self.activeSTC = ''

        # self.gMDIArea = QMdiArea()
        self.gMDIArea = MDIArea()
        self.gMDIArea.show()
        self.setCentralWidget(self.gMDIArea)

        self.gMDIArea.subWindowActivated.connect(self.DoMenuUpdates)
        self.windowMapper = QSignalMapper(self)
        self.windowMapper.mapped.connect(self.DoSetActiveSubWindow)

        self.CreateActions()
        self.CreateMenus()
        self.CreateToolBars()
        self.CreateStatusBar()
        self.CreateDockWindows()

        self.setUnifiedTitleAndToolBarOnMac(True)

        self.setWindowIcon(QIcon(gIconDir + os.sep + 'app.png'))

        # Set the initial frame size
        # screenRect = gApp.desktop().screenGeometry().getRect()
        # # print(screenRect)
        # # 2/3 w & h
        # self.setGeometry(0, 0, screenRect[2] / 3 * 2, screenRect[3] / 3 * 2)
        # self.CenterOnScreen()

        self.action_Edit_Cut.setEnabled(False)
        self.action_Edit_Copy.setEnabled(False)
        self.SetStatusText(pythonVersion)


        # # Load tips from external file
        # tipFile = QFile("tips.txt")
        # if tipFile.open(QFile.ReadOnly):
        #
        #     stream = QTextStream(tipFile)
        #     tipLine = QString()
        #     while not tipLine.isNull():
        #
        #         tipLine = stream.readLine()
        #         if not tipLine.isEmpty():
        #             listTipOfTheDay << tipLine
        #
        # if getSettingsGeneralTipOfTheDay():
        #     tipOfTheDay()

        if bool(int(self.settings_general_tip_of_the_day)):
            self.OnTipOfTheDay()


    # Development/Testing/TODO defs start ######################################

    # Developers Convienience OnStartup File/Specs.
    # def showEvent(self, event):
    #     # Developers Convienience OnStartup File/Specs.
    #     self.gMDIArea.setViewMode(QMdiArea.TabbedView)
    #     self.DoOpenFilePath('TODO.txt')

    def OnTest01(self):
        # Set Individual ToolBar Size
        self.gToolBar_Help.setIconSize(QSize(16, 16))
        print('OnTest01')

    def OnTest02(self):
        # self.gMenu_Help.setIconSize(QSize(32, 32))
        print('self.sender().text() = %s' % self.sender().text())
        print('self.sender().parentWidget() = %s' % self.sender().parentWidget())
        print('self.sender().objectName() = %s' % self.sender().objectName())
        print('OnTest02')

    def OnTest03(self):
        self.gMDIArea.DoSetBackground(QBrush(Qt.cyan))
        ForceRepaint(self.gMDIArea)
        print('OnTest03')

    def OnTest04(self):
        print('OnTest04')

    def OnTest05(self):
        # editor.showSystemMenu()
        # print(Qt.WindowStaysOnTopHint in self.windowFlags())
        windowFlags = self.windowFlags()
        flag_type = (windowFlags & Qt.WindowType_Mask)

        print(self.windowFlags())
        print(flag_type)

        if flag_type == Qt.Window:
            text = "QtCore.Qt.Window"
        elif flag_type == Qt.Dialog:
            text = "QtCore.Qt.Dialog"
        elif flag_type == Qt.Sheet:
            text = "QtCore.Qt.Sheet"
        elif flag_type == Qt.Drawer:
            text = "QtCore.Qt.Drawer"
        elif flag_type == Qt.Popup:
            text = "QtCore.Qt.Popup"
        elif flag_type == Qt.Tool:
            text = "QtCore.Qt.Tool"
        elif flag_type == Qt.ToolTip:
            text = "QtCore.Qt.ToolTip"
        elif flag_type == Qt.SplashScreen:
            text = "QtCore.Qt.SplashScreen"

        if windowFlags & Qt.MSWindowsFixedSizeDialogHint:
            text += "\n| QtCore.Qt.MSWindowsFixedSizeDialogHint"
        if windowFlags & Qt.X11BypassWindowManagerHint:
            text += "\n| QtCore.Qt.X11BypassWindowManagerHint"
        if windowFlags & Qt.FramelessWindowHint:
            text += "\n| QtCore.Qt.FramelessWindowHint"
        if windowFlags & Qt.WindowTitleHint:
            text += "\n| QtCore.Qt.WindowTitleHint"
        if windowFlags & Qt.WindowSystemMenuHint:
            text += "\n| QtCore.Qt.WindowSystemMenuHint"
        if windowFlags & Qt.WindowMinimizeButtonHint:
            text += "\n| QtCore.Qt.WindowMinimizeButtonHint"
        if windowFlags & Qt.WindowMaximizeButtonHint:
            text += "\n| QtCore.Qt.WindowMaximizeButtonHint"
        if windowFlags & Qt.WindowContextHelpButtonHint:
            text += "\n| QtCore.Qt.WindowContextHelpButtonHint"
        if windowFlags & Qt.WindowShadeButtonHint:
            text += "\n| QtCore.Qt.WindowShadeButtonHint"
        if windowFlags & Qt.WindowStaysOnTopHint:
            text += "\n| QtCore.Qt.WindowStaysOnTopHint"
        print(text)

        if windowFlags & Qt.WindowStaysOnTopHint:
            self.setWindowFlags(self.windowFlags() & ~Qt.WindowStaysOnTopHint)
        else:
            self.setWindowFlags(self.windowFlags() | Qt.WindowStaysOnTopHint)

        self.show() # For some odd reason, changing the flags makes the window hide, so show it again.

        # self.GetActiveMDISubWindow().parent().showShaded()
        # print(self.sender())
        print('OnTest05')

    def OnTODO(self):
        print('OnTODO')
        print('self.sender().text() = %s' % self.sender().text())
        print('self.sender().parentWidget() = %s' % self.sender().parentWidget())

    def OnHovered(self):
        print('OnHovered')
        print('self.sender().text() = %s' % self.sender().text())
        print('self.sender().parentWidget() = %s' % self.sender().parentWidget())

    # Development/Testing/TODO defs end ########################################

    def DoMenuUpdates(self):
        """"""
        # print('len(subWindowList) = %s' % len(self.gMDIArea.subWindowList()))
        if not len(self.gMDIArea.subWindowList()): # Nothing is open.
            # Hide everything. Go back to startup layout.
            gMainWin.gMenuBar.clear()
            self.DoAddMenusToMenuBar(menus=(self.gMenu_File, self.gMenu_Settings, self.gMenu_Help))
            self.DoHideAllToolBars()
            self.DoHideAllDockWindows()
            self.gStatusBar.hide()

        activeSubWindow = self.GetActiveMDISubWindow()
        print('self.activeMdiChild() = %s' % activeSubWindow)

        #TODO# self.gDockWindow_History.setFocusProxy(activeSubWindow)
        #TODO# self.gDockWindow_History.undoView.setFocusProxy(activeSubWindow)
        # print('self.activeMdiChild().filePath = %s' % self.GetActiveMDISubWindow().filePath)
        print('DoMenuUpdates')

    def GetActiveMDISubWindow(self):
        """"""
        activeMDISubWindow = self.gMDIArea.activeSubWindow()
        if activeMDISubWindow:
            return activeMDISubWindow.widget()
        return None

    def DoSetActiveSubWindow(self, window):
        """"""
        if window:
            self.gMDIArea.setActiveSubWindow(window)
        print('DoSetActiveSubWindow')

    def CenterOnScreen(self): # wxPython MethodName
        """
        Center the EmbroidermodderMainWindow on the screen.
        """
        screen = gApp.desktop().screenGeometry()
        size = self.geometry()
        # print("Window Width: %d" % size.width())
        # print("Window Height: %d" % size.height())
        # print("Screen Width: %d" % screen.width())
        # print("Screen Height: %d" % screen.height())
        self.move((screen.width() - size.width()) / 2, (screen.height() - size.height()) / 2)

    def DoCascadeMdiWindows(self):
        """"""
        # self.gMDIArea.setViewMode(QMdiArea.SubWindowView)
        self.gMDIArea.cascadeSubWindows()

    def DoTileMdiWindows(self):
        """"""
        # self.gMDIArea.setViewMode(QMdiArea.SubWindowView)
        self.gMDIArea.tileSubWindows()

    def DoTabMdiWindows(self):
        """"""
        self.gMDIArea.setViewMode(QMdiArea.TabbedView)

    def closeEvent(self, event):
        """"""
        # self.gMDIArea.closeAllSubWindows()

        self.isEmbroidermodderMainWindowClosing = True
        for child in self.gMDIArea.subWindowList():
            self.gMDIArea.closeActiveSubWindow()
            if not self.isEmbroidermodderMainWindowClosing: # User has clicked cancel, so veto the event.
                event.ignore()
                return

        event.accept()
        DoWriteSettings(self)

    def OnNewFile(self):
        """"""
        if not len(self.gMDIArea.subWindowList()): # Nothing is open.
            self.gMenuBar.clear()
            self.DoAddMenusToMenuBar(menus=self._allTopLevelMenus)
            self.ShowAllToolBars()
            self.ShowAllDockWindows()
            self.gStatusBar.show()
        editor = MDISubWindow_TextEdit(self.gMDIArea)
        self.gMDIArea.addSubWindow(editor)
        editor.show()

    def OnOpen(self):
        """"""
        filePath, filtr = QFileDialog.getOpenFileName(self, self.tr('Open file...'), os.getcwd(), WILDCARD_ALL)
        if filePath:
            self.DoOpenFilePath(filePath)

    def DoOpenFilePath(self, filePath):
        """"""
        if not len(self.gMDIArea.subWindowList()): # Nothing is open.
            self.gMenuBar.clear()
            self.DoAddMenusToMenuBar(menus=self._allTopLevelMenus)
            self.ShowAllToolBars()
            self.ShowAllDockWindows()
            self.gStatusBar.show()
        editor = MDISubWindow_TextEdit(self.gMDIArea, filePath)
        self.gMDIArea.addSubWindow(editor)
        editor.show()

    def OnSave(self):
        """"""
        if self.GetActiveMDISubWindow() and self.GetActiveMDISubWindow().OnSave():
            self.statusBar().showMessage(self.tr('File saved'), 2000)

        # if self.GetActiveMDISubWindow().isModified():
            # self.GetActiveMDISubWindow().save()
        # if self.activeSTC:
            # return self.OnSaveFilePath(self.activeSTC)

        # return self.OnSaveAs()

    def OnSaveAs(self):
        """"""
        if self.GetActiveMDISubWindow() and self.GetActiveMDISubWindow().OnSaveAs():
            self.statusBar().showMessage(self.tr('File saved'), 2000)

    def OnPrint(self):
        """"""
        print('OnPrint')

    def OnWindowClose(self):
        """"""
        self.gMDIArea.closeActiveSubWindow()

    def OnWindowCloseAll(self):
        """"""
        self.gMDIArea.closeAllSubWindows()

    def OnWindowNext(self):
        """"""
        self.gMDIArea.activateNextSubWindow()

    def OnWindowPrevious(self):
        """"""
        self.gMDIArea.activatePreviousSubWindow()

    def OnDetails(self):
        """
        Show the Embroidermodder Details Dialog.
        """
        dialog = EmbDetailsDialog(None, self)
        dialog.show()

        print('OnDetails')

    def OnDay(self):
        """"""
        self.GetActiveMDISubWindow().OnDay()
        print('OnDay')

    def OnNight(self):
        """"""
        self.GetActiveMDISubWindow().OnNight()
        print('OnNight')

    def OnToggleFullscreenMode(self):
        """
        Toggle Fullscreen Mode On/Off.
        """
        if not self.isFullScreen():
            self.showFullScreen()
            self.action_View_Fullscreen.setIcon(QIcon(gIconDir + os.sep + 'check.png'))
        else:
            self.showNormal()
            self.action_View_Fullscreen.setIcon(QIcon(gIconDir + os.sep + 'blank.png'))

        ## if self.windowState() & Qt.WindowFullScreen:
        ##     self.showNormal()
        ## else:
        ##     self.showFullScreen()

    def OnToggleMaximize(self):
        """"""
        if not self.isMaximized():
            self.showMaximized()
        else:
            self.showNormal()

    def OnToggleMinimize(self):
        """"""
        if not self.isMinimized():
            self.showMinimized()
        else:
            self.showNormal()

    def OnToggleStaysOnTopFlag(self):
        """"""
        windowFlags = self.windowFlags()
        # flag_type = (windowFlags & Qt.WindowType_Mask)

        if windowFlags & Qt.WindowStaysOnTopHint:
            self.setWindowFlags(self.windowFlags() & ~Qt.WindowStaysOnTopHint)
            self.action_View_StaysOnTop.setIcon(QIcon(gIconDir + os.sep + 'blank.png'))
        else:
            self.setWindowFlags(self.windowFlags() | Qt.WindowStaysOnTopHint)
            self.action_View_StaysOnTop.setIcon(QIcon(gIconDir + os.sep + 'check.png'))

        self.show() # For some odd reason, changing the flags makes the window hide, so show it again.

    def OnChangelog(self):
        """"""
        print('OnChangelog')

    def OnTipOfTheDay(self):
        """"""
        dialog = TipOfTheDayDialog(self)
        dialog.show()

        print('OnTipOfTheDay')

    def OnHelp(self):
        """"""
        localUrl = 'file:///'
        localPath = gAppDir + os.sep + 'help' + os.sep + 'doc-index.html'
        webbrowser.open_new_tab(localUrl + localPath)

    def OnWhatsThisContextHelp(self):
        """"""
        # QWhatsThis.createAction()
        QWhatsThis.enterWhatsThisMode()

    def OnSetToolbarIconSize16(self):
        """"""
        print(self.sender().text())
        print(self.sender().parentWidget())
        print(self.sender().actionGroup())
        [tb.setIconSize(QSize(16, 16)) for tb in self._allToolBars]
        self.undoToolButton.setIconSize(QSize(16, 16))
        self.redoToolButton.setIconSize(QSize(16, 16))
    def OnSetToolbarIconSize24(self):
        """"""
        [tb.setIconSize(QSize(24, 24)) for tb in self._allToolBars]
        self.undoToolButton.setIconSize(QSize(24, 24))
        self.redoToolButton.setIconSize(QSize(24, 24))
    def OnSetToolbarIconSize32(self):
        """"""
        [tb.setIconSize(QSize(32, 32)) for tb in self._allToolBars]
        self.undoToolButton.setIconSize(QSize(32, 32))
        self.redoToolButton.setIconSize(QSize(32, 32))
    def OnSetToolbarIconSize48(self):
        """"""
        [tb.setIconSize(QSize(48, 48)) for tb in self._allToolBars]
        self.undoToolButton.setIconSize(QSize(48, 48))
        self.redoToolButton.setIconSize(QSize(48, 48))
    def OnSetToolbarIconSize64(self):
        """"""
        [tb.setIconSize(QSize(64, 64)) for tb in self._allToolBars]
        self.undoToolButton.setIconSize(QSize(64, 64))
        self.redoToolButton.setIconSize(QSize(64, 64))
    def OnSetToolbarIconSize128(self):
        """"""
        [tb.setIconSize(QSize(128, 128)) for tb in self._allToolBars]
        self.undoToolButton.setIconSize(QSize(128, 128))
        self.redoToolButton.setIconSize(QSize(128, 128))

    def SetStatusText(self, msg):
        """"""
        self.gStatusBar.showMessage(msg)

    def CreateActions(self):
        """"""
        self.action_File_New = QAction(QIcon(gIconDir + os.sep + 'new.png'),
                '&New', self,
                shortcut=NewShortcut, # QKeySequence.New
                statusTip='Create a new file.',
                triggered=self.OnNewFile,
                whatsThis='Create a new file.')

        self.action_File_Open = QAction(QIcon(gIconDir + os.sep + 'open.png'),
                '&Open...', self,
                shortcut=OpenShortcut, # QKeySequence.Open
                statusTip='Open an existing file.',
                triggered=self.OnOpen)

        self.action_File_Save = QAction(QIcon(gIconDir + os.sep + 'save.png'),
                '&Save', self,
                shortcut=SaveShortcut, # QKeySequence.Save
                statusTip='Save the design to disk.',
                triggered=self.OnSave)

        self.action_File_SaveAs = QAction(QIcon(gIconDir + os.sep + 'saveas.png'),
                'Save &As...', self,
                shortcut=SaveAsShortcut, # QKeySequence.SaveAs
                statusTip='Save the design under a new name.',
                triggered=self.OnSaveAs)

        self.action_File_Print = QAction(QIcon(gIconDir + os.sep + 'print.png'),
                '&Print...', self,
                shortcut=PrintShortcut, # QtGui.QKeySequence.Print
                statusTip='Print the design',
                triggered=self.OnPrint)

        # self.action_File_Close = QAction(QIcon(gIconDir + os.sep + 'windowclose.png'),
                # '&Details...', self,
                # shortcut=WindowCloseShortcut,
                # statusTip='Details of the current design.',
                # triggered=self.OnWindowClose)

        self.action_File_Details = QAction(QIcon(gIconDir + os.sep + 'histogram.png'),
                '&Details...', self,
                shortcut=DetailsShortcut,
                statusTip='Print the design.',
                triggered=self.OnDetails)

        self.action_File_Exit = QAction(QIcon(gIconDir + os.sep + 'exit.png'),
                'E&xit', self,
                shortcut=ExitShortcut,
                statusTip='Exit the application.',
                triggered=self.close)

        self.action_Edit_Undo = QAction(QIcon(gIconDir + os.sep + 'undo.png'),
                '&Undo', self,
                shortcut=UndoShortcut, # QKeySequence.Undo
                statusTip='Reverses the most recent action.',
                triggered=self.OnTODO)

        self.action_Edit_Redo = QAction(QIcon(gIconDir + os.sep + 'redo.png'),
                '&Redo', self,
                shortcut=RedoShortcut, # QKeySequence.Redo
                statusTip='Reverses the effects of the previous undo action.',
                triggered=self.OnTODO)

        self.action_Edit_Cut = QAction(QIcon(gIconDir + os.sep + 'cut.png'),
                'Cu&t', self,
                shortcut=CutShortcut, # QKeySequence.Cut
                statusTip='Cut the current selection\'s contents to the clipboard.',
                triggered=self.OnTODO)

        self.action_Edit_Copy = QAction(QIcon(gIconDir + os.sep + 'copy.png'),
                '&Copy', self,
                shortcut=CopyShortcut, # QKeySequence.Copy
                statusTip='Copy the current selection\'s contents to the clipboard.',
                triggered=self.OnTODO)

        self.action_Edit_Paste = QAction(QIcon(gIconDir + os.sep + 'paste.png'),
                '&Paste', self,
                shortcut=PasteShortcut, # QKeySequence.Paste
                statusTip='Paste the clipboard\'s contents into the current selection.',
                triggered=self.OnTODO)

        self.action_View_Zoom_ZoomRealtime = QAction(QIcon(gIconDir + os.sep + 'zoomrealtime.png'),
                'Zoom Realtime', self,
                shortcut=ZoomRealtimeShortcut,
                statusTip='Zooms to increase or decrease the apparent size of objects in the current viewport.',
                triggered=self.OnTODO)

        self.action_View_Zoom_ZoomPrevious = QAction(QIcon(gIconDir + os.sep + 'zoomprevious.png'),
                'Zoom Previous', self,
                shortcut=ZoomPreviousShortcut,
                statusTip='Zooms to display the previous view.',
                triggered=self.OnTODO)

        self.action_View_Zoom_ZoomWindow = QAction(QIcon(gIconDir + os.sep + 'zoomwindow.png'),
                'Zoom Window', self,
                shortcut=ZoomWindowShortcut,
                statusTip='Zooms to display an area specified by a rectangular window.',
                triggered=self.OnTODO)

        self.action_View_Zoom_ZoomDynamic = QAction(QIcon(gIconDir + os.sep + 'zoomdynamic.png'),
                'Zoom Dynamic', self,
                shortcut=ZoomDynamicShortcut,
                statusTip='Zooms to display the generated portion of the drawing.',
                triggered=self.OnTODO)

        self.action_View_Zoom_ZoomScale = QAction(QIcon(gIconDir + os.sep + 'zoomscale.png'),
                'Zoom Scale', self,
                shortcut=ZoomScaleShortcut,
                statusTip='Zooms the display using a specified scale factor.',
                triggered=self.OnTODO)

        self.action_View_Zoom_ZoomCenter = QAction(QIcon(gIconDir + os.sep + 'zoomcenter.png'),
                'Zoom Center', self,
                shortcut=ZoomCenterShortcut,
                statusTip='Zooms to display a view specified by a center point and magnification or height.',
                triggered=self.OnTODO)

        self.action_View_Zoom_ZoomIn = QAction(QIcon(gIconDir + os.sep + 'zoomin.png'),
                'Zoom In', self,
                shortcut=ZoomInShortcut,
                statusTip='Zooms to display the selected objects.',
                triggered=self.OnTODO)

        self.action_View_Zoom_ZoomOut = QAction(QIcon(gIconDir + os.sep + 'zoomout.png'),
                'Zoom Out', self,
                shortcut=ZoomInShortcut,
                statusTip='Zooms to display the drawing extents or the grid limits.',
                triggered=self.OnTODO)

        self.action_View_Zoom_ZoomSelected = QAction(QIcon(gIconDir + os.sep + 'zoomselected.png'),
                'Zoom Selected', self,
                shortcut=ZoomSelectedShortcut,
                statusTip='Zooms to display the drawing extents.',
                triggered=self.OnTODO)

        self.action_View_Zoom_ZoomAll = QAction(QIcon(gIconDir + os.sep + 'zoomall.png'),
                'Zoom All', self,
                shortcut=ZoomAllShortcut,
                statusTip='Zooms to display the drawing extents or the grid limits.',
                triggered=self.OnTODO)

        self.action_View_Zoom_ZoomExtents = QAction(QIcon(gIconDir + os.sep + 'zoomextents.png'),
                'Zoom Extents', self,
                shortcut=ZoomExtentsShortcut,
                statusTip='Zooms to display the drawing extents.',
                triggered=self.OnTODO)

        self.action_View_Pan_PanRealtime = QAction(QIcon(gIconDir + os.sep + 'panrealtime.png'),
                'Pan Realtime', self,
                shortcut=PanRealtimeShortcut,
                statusTip='Moves the view in the current viewport.',
                triggered=self.OnTODO)

        self.action_View_Pan_PanPoint = QAction(QIcon(gIconDir + os.sep + 'panpoint.png'),
                'Pan Point', self,
                shortcut=PanPointShortcut,
                statusTip='Moves the view by the specified distance.',
                triggered=self.OnTODO)

        self.action_View_Pan_PanLeft = QAction(QIcon(gIconDir + os.sep + 'panleft.png'),
                'Pan Left', self,
                shortcut=PanLeftShortcut,
                statusTip='Moves the view to the left.',
                triggered=self.OnTODO)

        self.action_View_Pan_PanRight = QAction(QIcon(gIconDir + os.sep + 'panright.png'),
                'Pan Right', self,
                shortcut=PanRightShortcut,
                statusTip='Moves the view to the right.',
                triggered=self.OnTODO)

        self.action_View_Pan_PanUp = QAction(QIcon(gIconDir + os.sep + 'panup.png'),
                'Pan Up', self,
                shortcut=PanUpShortcut,
                statusTip='Moves the view up.',
                triggered=self.OnTODO)

        self.action_View_Pan_PanDown = QAction(QIcon(gIconDir + os.sep + 'pandown.png'),
                'Pan Down', self,
                shortcut=PanDownShortcut,
                statusTip='Moves the view down.',
                triggered=self.OnTODO)

        self.action_View_Day = QAction(QIcon(gIconDir + os.sep + 'day.png'),
                '&Day', self,
                shortcut=ViewDayShortcut,
                statusTip='Updates the current view using day vision settings.',
                triggered=self.OnDay)

        self.action_View_Night = QAction(QIcon(gIconDir + os.sep + 'night.png'),
                '&Night', self,
                shortcut=ViewNightShortcut,
                statusTip='Updates the current view using night vision settings.',
                triggered=self.OnNight)

        self.action_View_Fullscreen = QAction(QIcon(gIconDir + os.sep + 'fullscreen.png'),
                '&Fullscreen', self,
                shortcut=ToggleFullscreenShortcut,
                statusTip='Toggle fullscreen mode.',
                triggered=self.OnToggleFullscreenMode)

        self.action_View_Maximize = QAction(QIcon(gIconDir + os.sep + 'windowmaximize.png'),
                '&Maximize', self,
                shortcut=MaximizeShortcut,
                statusTip='Maximize the window.',
                triggered=self.OnToggleMaximize)

        self.action_View_Minimize = QAction(QIcon(gIconDir + os.sep + 'windowminimize.png'),
                '&Minimize', self,
                shortcut=MaximizeShortcut,
                statusTip='Minimize the window.',
                triggered=self.OnToggleMinimize)

        self.action_View_StaysOnTop = QAction(QIcon(gIconDir + os.sep + 'blank.png'),
                '&Always Stays On Top', self,
                shortcut=MaximizeShortcut,
                statusTip='Toggle Window Always Stays On Top On/Off.',
                triggered=self.OnToggleStaysOnTopFlag)

        self.action_Tools_Distance = QAction(QIcon(gIconDir + os.sep + 'distance.png'),
                '&Distance', self,
                shortcut=DistanceShortcut,
                statusTip='Measures the distance and angle between two points: DIST',
                triggered=self.OnTODO)

        self.action_Tools_LocatePoint = QAction(QIcon(gIconDir + os.sep + 'locatepoint.png'),
                '&Locate Point', self,
                shortcut=LocatePointShortcut,
                statusTip='Displays the coordinate values of a location: ID',
                triggered=self.OnTODO)

        self.action_Modify_Delete = QAction(QIcon(gIconDir + os.sep + 'erase.png'),
                '&Delete', self,
                shortcut=DeleteShortcut,
                statusTip='Removes objects from a drawing: DELETE',
                triggered=self.OnTODO)

        self.action_Modify_Move = QAction(QIcon(gIconDir + os.sep + 'move.png'),
                '&Move', self,
                shortcut=MoveShortcut,
                statusTip='Displaces objects a specified distance in a specified direction: MOVE',
                triggered=self.OnTODO)

        self.action_Modify_Rotate = QAction(QIcon(gIconDir + os.sep + 'rotate.png'),
                '&Rotate', self,
                shortcut=RotateShortcut,
                statusTip='Rotates objects around a base point: ROTATE',
                triggered=self.OnTODO)

        self.action_Modify_Scale = QAction(QIcon(gIconDir + os.sep + 'scale.png'),
                '&Scale', self,
                shortcut=ScaleShortcut,
                statusTip='TODO?',
                triggered=self.OnTODO)

        self.action_Dimension_QuickLeader = QAction(QIcon(gIconDir + os.sep + 'quickleader.png'),
                '&QuickLeader', self,
                shortcut=QuickLeaderShortcut,
                statusTip='Creates a leader and annotation: QUICKLEADER',
                triggered=self.OnTODO)

        self.action_Sandbox_RGB = QAction(QIcon(gIconDir + os.sep + 'rgb.png'),
                '&RGB', self,
                shortcut=RGBShortcut,
                statusTip='Updates the current view colors: RGB',
                triggered=self.OnTODO)

        self.action_Sandbox_Sandbox = QAction(QIcon(gIconDir + os.sep + 'sandbox.png'),
                '&Sandbox', self,
                shortcut=SandboxShortcut,
                statusTip='A sandbox to play in: SANDBOX',
                triggered=self.OnTODO)

        self.action_Draw_Circle = QAction(QIcon(gIconDir + os.sep + 'circle.png'),
                '&Circle', self,
                shortcut=CircleShortcut,
                statusTip='Creates a circle: CIRCLE',
                triggered=self.OnTODO)

        self.action_Draw_Dolphin = QAction(QIcon(gIconDir + os.sep + 'dolphin.png'),
                '&Dolphin', self,
                shortcut=DolphinShortcut,
                statusTip='Creates a dolphin: DOLPHIN',
                triggered=self.OnTODO)

        self.action_Draw_Ellipse = QAction(QIcon(gIconDir + os.sep + 'ellipse.png'),
                '&Ellipse', self,
                shortcut=EllipseShortcut,
                statusTip='Creates a ellipse: ELLIPSE',
                triggered=self.OnTODO)

        self.action_Draw_Heart = QAction(QIcon(gIconDir + os.sep + 'heart.png'),
                '&Heart', self,
                shortcut=HeartShortcut,
                statusTip='Creates a heart: HEART',
                triggered=self.OnTODO)

        self.action_Draw_Line = QAction(QIcon(gIconDir + os.sep + 'line.png'),
                '&Line', self,
                shortcut=LineShortcut,
                statusTip='Creates straight line segments: LINE',
                triggered=self.OnTODO)

        self.action_Draw_Path = QAction(QIcon(gIconDir + os.sep + 'path.png'),
                '&Path', self,
                shortcut=PathShortcut,
                statusTip='Creates a 2D path: PATH',
                triggered=self.OnTODO)

        self.action_Draw_Point = QAction(QIcon(gIconDir + os.sep + 'point.png'),
                '&Point', self,
                shortcut=PointShortcut,
                statusTip='Creates multiple points: POINT',
                triggered=self.OnTODO)

        self.action_Draw_Polygon = QAction(QIcon(gIconDir + os.sep + 'polygon.png'),
                '&Polygon', self,
                shortcut=PolygonShortcut,
                statusTip='Creates a rectangular polygon: POLYGON',
                triggered=self.OnTODO)

        self.action_Draw_Polyline = QAction(QIcon(gIconDir + os.sep + 'polyline.png'),
                '&Polyline', self,
                shortcut=PolylineShortcut,
                statusTip='Creates a 2D polyline: PLINE',
                triggered=self.OnTODO)

        self.action_Draw_Rectangle = QAction(QIcon(gIconDir + os.sep + 'rectangle.png'),
                '&Rectangle', self,
                shortcut=RectangleShortcut,
                statusTip='Creates a rectangular polyline: RECTANGLE',
                triggered=self.OnTODO)

        self.action_Draw_SingleLineText = QAction(QIcon(gIconDir + os.sep + 'singlelinetext.png'),
                '&Single Line Text', self,
                shortcut=SingleLineTextShortcut,
                statusTip='Creates single-line text objects: TEXT',
                triggered=self.OnTODO)

        self.action_Draw_Snowflake = QAction(QIcon(gIconDir + os.sep + 'snowflake.png'),
                '&Snowflake', self,
                shortcut=SnowflakeShortcut,
                statusTip='Creates a snowflake: SNOWFLAKE',
                triggered=self.OnTODO)

        self.action_Draw_Star = QAction(QIcon(gIconDir + os.sep + 'star.png'),
                '&Star', self,
                shortcut=StarShortcut,
                statusTip='Creates a star: STAR',
                triggered=self.OnTODO)

        self.action_Settings_Settings = QAction(QIcon(gIconDir + os.sep + 'settingsdialog.png'),
                '&Settings', self,
                shortcut=SettingsShortcut,
                statusTip='Configure settings specific to this project.',
                triggered=self.OnShowSettingsDialog)

        self.action_Window_Close = QAction(QIcon(gIconDir + os.sep + 'windowclose.png'),
                '&Close', self,
                shortcut=WindowCloseShortcut,
                statusTip='',
                triggered=self.OnWindowClose)

        self.action_Window_CloseAll = QAction(QIcon(gIconDir + os.sep + 'windowcloseall.png'),
                '&Close All', self,
                shortcut=WindowCloseAllShortcut,
                statusTip='',
                triggered=self.OnWindowCloseAll)

        self.action_Window_Cascade = QAction(QIcon(gIconDir + os.sep + 'windowcascade.png'),
                '&Cascade', self,
                shortcut=WindowCascadeShortcut,
                statusTip='',
                triggered=self.DoCascadeMdiWindows)

        self.action_Window_Tile = QAction(QIcon(gIconDir + os.sep + 'windowtile.png'),
                '&Tile', self,
                shortcut=WindowTileShortcut,
                statusTip='',
                triggered=self.DoTileMdiWindows)

        self.action_Window_Next = QAction(QIcon(gIconDir + os.sep + 'windownext.png'),
                '&Next', self,
                shortcut=WindowNextShortcut,
                statusTip='',
                triggered=self.OnWindowNext)

        self.action_Window_Previous = QAction(QIcon(gIconDir + os.sep + 'windowprevious.png'),
                '&Previous', self,
                shortcut=WindowPreviousShortcut,
                statusTip='',
                triggered=self.OnWindowPrevious)

#         from Strings import *
#         for key, value in list(gHelpStringsDict.items()):
#             execStr = """\
# self.action_Help_%s = QAction(QIcon(gIconDir + os.sep + '%s'),
#         self.tr('%s'), self,
#         shortcut=self.tr('%s'),
#         statusTip=self.tr('%s'),
#         triggered=self.%s)
# """ % (key.replace('&', '').replace(' ', ''), value[2], key, value[0], value[2], value[1])
#             exec(execStr)

        self.action_Help_Help = QAction(QIcon(gIconDir + os.sep + 'help.png'),
                self.tr('&Help'), self,
                shortcut=HelpShortcut,
                statusTip='Displays help.',
                triggered=self.OnHelp,
                whatsThis='Opens the Embroidermodder help files up in your default web browser.')

        self.action_Help_Changelog = QAction(QIcon(gIconDir + os.sep + 'changelog.png'),
                '&Changelog', self,
                shortcut=ChangelogShortcut,
                statusTip='Describes new features in this product.',
                triggered=self.OnChangelog,
                whatsThis=('Opens the Changelog dialog that displays information' '\n'
                           'about changes that have been done thoughout the various' '\n'
                           'versions of Embroidermodder.'))

        self.action_Help_TipOfTheDay = QAction(QIcon(gIconDir + os.sep + 'tipoftheday.png'),
                '&Tip Of The Day', self,
                shortcut=TipOfTheDayShortcut,
                statusTip='Displays a dialog with useful tips.',
                triggered=self.OnTipOfTheDay,
                whatsThis='Opens the Tip Of The Day dialog that displays useful tips.')

        self.action_Help_About = QAction(QIcon(gIconDir + os.sep + 'about.png'),
                '&About', self,
                shortcut=AboutEmbroidermodderShortcut,
                statusTip='Displays information about this product.',
                triggered=self.OnAbout,
                whatsThis='Opens the About dialog that displays information about this product.')

        # self.action_Help_ContextHelp = QAction(QIcon(gIconDir + os.sep + 'lifesaver_help16.png'),
        self.action_Help_ContextHelp = QAction(QIcon(gIconDir + os.sep + 'lifesaver_help128.png'),
                '&What\'s This?', self,
                shortcut=WhatsThisContextHelpShortcut,
                statusTip='What\'s This? Context Help!',
                triggered=self.OnWhatsThisContextHelp,
                whatsThis=('I\'m a context help button.' '\n'
                           'Click on me and then on another widget to get' '\n'
                           'a more detailed message of what it does!'))


        self.action_Text_Bold = QAction(QIcon(gIconDir + os.sep + 'textbold.png'),
                '&Bold Text', self,
                shortcut=TextBoldShortcut,
                statusTip='',
                triggered=self.OnTODO)

        self.action_Text_Italic = QAction(QIcon(gIconDir + os.sep + 'textitalic.png'),
                '&Italic Text', self,
                shortcut=TextItalicShortcut,
                statusTip='',
                triggered=self.OnTODO)

        self.action_Text_Underline = QAction(QIcon(gIconDir + os.sep + 'textunderline.png'),
                '&Underline Text', self,
                shortcut=TextUnderlineShortcut,
                statusTip='',
                triggered=self.OnTODO)

        self.action_Text_StrikeOut = QAction(QIcon(gIconDir + os.sep + 'textstrikeout.png'),
                '&StrikeOut Text', self,
                shortcut=TextStrikeOutShortcut,
                statusTip='',
                triggered=self.OnTODO)

        self.action_Text_Overline = QAction(QIcon(gIconDir + os.sep + 'textoverline.png'),
                '&Overline Text', self,
                shortcut=TextOverlineShortcut,
                statusTip='',
                triggered=self.OnTODO)

        self.action_Layer_MakeLayerActive = QAction(QIcon(gIconDir + os.sep + 'makelayercurrent.png'),
                'Make Layer &Active', self,
                shortcut=MakeLayerActiveShortcut,
                statusTip='Makes the layer of a selected object the active layer.',
                triggered=self.OnTODO)

        self.action_Layer_Layers = QAction(QIcon(gIconDir + os.sep + 'layers.png'),
                '&Layers', self,
                shortcut=LayersShortcut,
                statusTip='Manages layers and layer properties: LAYER',
                triggered=self.OnTODO)

        self.action_Layer_Previous = QAction(QIcon(gIconDir + os.sep + 'layerprevious.png'),
                'Layer &Previous', self,
                shortcut=LayerPreviousShortcut,
                statusTip='Restores the previous layer settings: LAYERP',
                hovered=self.OnHovered,
                triggered=self.OnTODO)


        self.action_TEST01 = QAction(QIcon(gIconDir + os.sep + 'pyscript.png'),
                '&OnTest01', self,
                shortcut=LayerPreviousShortcut,
                statusTip='OnTest01',
                hovered=self.OnHovered,
                triggered=self.OnTest01)
        self.action_TEST02 = QAction(QIcon(gIconDir + os.sep + 'spoolofthread.png'),
                '&OnTest02', self,
                shortcut=LayerPreviousShortcut,
                statusTip='OnTest02',
                hovered=self.OnHovered,
                triggered=self.OnTest02)
        self.action_TEST03 = QAction(QIcon(gIconDir + os.sep + 'pyscript.png'),
                '&OnTest03', self,
                shortcut=LayerPreviousShortcut,
                statusTip='OnTest03',
                hovered=self.OnHovered,
                triggered=self.OnTest03)
        self.action_TEST04 = QAction(QIcon(gIconDir + os.sep + 'pyscript.png'),
                '&OnTest04', self,
                shortcut=LayerPreviousShortcut,
                statusTip='OnTest04',
                hovered=self.OnHovered,
                triggered=self.OnTest04)

        self.actionGroup_ToolBarIconSize = QActionGroup(self)
        for i in (16, 24, 32, 48, 64, 128):
            execStr = """\
self.action_Icon%d = QAction(QIcon(gIconDir + os.sep + 'icon%d.png'),
        'Icon%d', self,
        shortcut='',
        statusTip='Sets the toolbar icon size to %dx%d',
        triggered=self.OnSetToolbarIconSize%d)
self.actionGroup_ToolBarIconSize.addAction(self.action_Icon%d)
""" % (i, i, i, i, i, i, i)
            exec(execStr)

        # self.action_Edit_Cut.setEnabled(False)
        # self.action_Edit_Copy.setEnabled(False)
        # self.textEdit.copyAvailable.connect(self.action_Edit_Cut.setEnabled)
        # self.textEdit.copyAvailable.connect(self.action_Edit_Copy.setEnabled)

    def CreateMenus(self):
        """"""
        self.gMenuBar = gMenuBar = self.menuBar()

        self.gMenu_File = gMenu_File = QMenu('&File')
        gMenu_File.setTearOffEnabled(False)
        gMenu_File.addAction(self.action_File_New)
        gMenu_File.addAction(self.action_File_Open)
        gMenu_File.addAction(self.action_File_Save)
        gMenu_File.addAction(self.action_File_SaveAs)
        gMenu_File.addSeparator()
        gMenu_File.addAction(self.action_File_Print)
        gMenu_File.addSeparator()
        gMenu_File.addAction(self.action_Window_Close)
        gMenu_File.addSeparator()
        gMenu_File.addAction(self.action_File_Details)
        gMenu_File.addSeparator()
        gMenu_File.addAction(self.action_File_Exit)

        self.gMenu_Edit = gMenu_Edit = QMenu('&Edit')
        gMenu_Edit.setTearOffEnabled(True)
        gMenu_Edit.addAction(self.action_Edit_Undo)
        gMenu_Edit.addAction(self.action_Edit_Redo)
        gMenu_Edit.addSeparator()
        gMenu_Edit.addAction(self.action_Edit_Cut)
        gMenu_Edit.addAction(self.action_Edit_Copy)
        gMenu_Edit.addAction(self.action_Edit_Paste)

        self.gMenu_View = gMenu_View = QMenu('&View')
        gMenu_View.setTearOffEnabled(True)

        self.gMenu_View_Zoom = gMenu_View_Zoom = QMenu('&Zoom')
        gMenu_View_Zoom.setTearOffEnabled(True)
        gMenu_View_Zoom.addAction(self.action_View_Zoom_ZoomRealtime)
        gMenu_View_Zoom.addAction(self.action_View_Zoom_ZoomPrevious)
        gMenu_View_Zoom.addSeparator()
        gMenu_View_Zoom.addAction(self.action_View_Zoom_ZoomWindow)
        gMenu_View_Zoom.addAction(self.action_View_Zoom_ZoomDynamic)
        gMenu_View_Zoom.addAction(self.action_View_Zoom_ZoomScale)
        gMenu_View_Zoom.addAction(self.action_View_Zoom_ZoomCenter)
        gMenu_View_Zoom.addSeparator()
        gMenu_View_Zoom.addAction(self.action_View_Zoom_ZoomIn)
        gMenu_View_Zoom.addAction(self.action_View_Zoom_ZoomOut)
        gMenu_View_Zoom.addSeparator()
        gMenu_View_Zoom.addAction(self.action_View_Zoom_ZoomSelected)
        gMenu_View_Zoom.addAction(self.action_View_Zoom_ZoomAll)
        gMenu_View_Zoom.addAction(self.action_View_Zoom_ZoomExtents)

        self.gMenu_View_Pan = gMenu_View_Pan = QMenu('&Pan')
        gMenu_View_Pan.setTearOffEnabled(True)
        gMenu_View_Pan.addAction(self.action_View_Pan_PanRealtime)
        gMenu_View_Pan.addAction(self.action_View_Pan_PanPoint)
        gMenu_View_Pan.addSeparator()
        gMenu_View_Pan.addAction(self.action_View_Pan_PanLeft)
        gMenu_View_Pan.addAction(self.action_View_Pan_PanRight)
        gMenu_View_Pan.addAction(self.action_View_Pan_PanUp)
        gMenu_View_Pan.addAction(self.action_View_Pan_PanDown)

        gMenu_View.addMenu(gMenu_View_Zoom)
        gMenu_View.addMenu(gMenu_View_Pan)
        gMenu_View.addSeparator()
        gMenu_View.addAction(self.action_View_Day)
        gMenu_View.addAction(self.action_View_Night)
        gMenu_View.addSeparator()
        gMenu_View.addAction(self.action_View_Fullscreen)
        gMenu_View.addAction(self.action_View_Maximize)
        gMenu_View.addAction(self.action_View_Minimize)
        gMenu_View.addAction(self.action_View_StaysOnTop)

        self.gMenu_Tools = gMenu_Tools = QMenu('&Tools')
        gMenu_Tools.setTearOffEnabled(True)
        gMenu_Tools.addAction(self.action_Tools_Distance)
        gMenu_Tools.addAction(self.action_Tools_LocatePoint)

        self.gMenu_Modify = gMenu_Modify = QMenu('&Modify')
        gMenu_Modify.setTearOffEnabled(True)
        gMenu_Modify.addAction(self.action_Modify_Delete)
        gMenu_Modify.addAction(self.action_Modify_Move)
        gMenu_Modify.addAction(self.action_Modify_Rotate)
        gMenu_Modify.addAction(self.action_Modify_Scale)

        self.gMenu_Dimension = gMenu_Dimension = QMenu('&Dimension')
        gMenu_Dimension.setTearOffEnabled(True)
        gMenu_Dimension.addAction(self.action_Dimension_QuickLeader)

        self.gMenu_Sandbox = gMenu_Sandbox = QMenu('&Sandbox')
        gMenu_Sandbox.setTearOffEnabled(True)
        gMenu_Sandbox.addAction(self.action_Sandbox_RGB)
        gMenu_Sandbox.addAction(self.action_Sandbox_Sandbox)

        self.gMenu_Draw = gMenu_Draw = QMenu('&Draw')
        gMenu_Draw.setTearOffEnabled(True)
        gMenu_Draw.addAction(self.action_Draw_Circle)
        gMenu_Draw.addAction(self.action_Draw_Dolphin)
        gMenu_Draw.addAction(self.action_Draw_Ellipse)
        gMenu_Draw.addAction(self.action_Draw_Heart)
        gMenu_Draw.addAction(self.action_Draw_Line)
        gMenu_Draw.addAction(self.action_Draw_Path)
        gMenu_Draw.addAction(self.action_Draw_Point)
        gMenu_Draw.addAction(self.action_Draw_Polygon)
        gMenu_Draw.addAction(self.action_Draw_Polyline)
        gMenu_Draw.addAction(self.action_Draw_Rectangle)
        gMenu_Draw.addAction(self.action_Draw_SingleLineText)
        gMenu_Draw.addAction(self.action_Draw_Snowflake)
        gMenu_Draw.addAction(self.action_Draw_Star)

        self.gMenu_Settings = gMenu_Settings = QMenu('&Settings')
        gMenu_Settings.setTearOffEnabled(True)
        gMenu_Settings.addAction(self.action_Settings_Settings)

        self.gMenu_Window = gMenu_Window = QMenu('&Window')
        gMenu_Window.setTearOffEnabled(False)
        gMenu_Window.addAction(self.action_Window_Close)
        gMenu_Window.addAction(self.action_Window_CloseAll)
        gMenu_Window.addSeparator()
        gMenu_Window.addAction(self.action_Window_Cascade)
        gMenu_Window.addAction(self.action_Window_Tile)
        gMenu_Window.addSeparator()
        gMenu_Window.addAction(self.action_Window_Next)
        gMenu_Window.addAction(self.action_Window_Previous)
        gMenu_Window.addSeparator()


        self.gMenu_Help = gMenu_Help = QMenu('&Help')
        gMenu_Help.setTearOffEnabled(True)
        gMenu_Help.addAction(self.action_Help_Help)
        gMenu_Help.addAction(self.action_Help_Changelog)
        gMenu_Help.addAction(self.action_Help_TipOfTheDay)
        gMenu_Help.addAction(self.action_Help_About)
        gMenu_Help.addAction(self.action_Help_ContextHelp)

        self._allTopLevelMenus = (
            gMenu_File,
            gMenu_Edit,
            gMenu_View,
            gMenu_Tools,
            gMenu_Modify,
            gMenu_Dimension,
            gMenu_Sandbox,
            gMenu_Dimension,
            gMenu_Settings,
            gMenu_Window,
            gMenu_Help
            )

        gMenuBar.addMenu(gMenu_File)
        gMenuBar.addMenu(gMenu_Settings)
        gMenuBar.addMenu(gMenu_Help)

        ## self.DoAddMenusToMenuBar(self._allTopLevelMenus)

        # Hidden or Non-Shown Menu
        # self.gMenu_Icon = gMenu_Icon = QMenu('&Icon')
        # ## gMenuBar.addMenu(gMenu_Icon)
        # gMenu_Icon.setTearOffEnabled(True)
        # for i in (16, 24, 32, 48, 64, 128):
        #     execStr = "gMenu_Icon.addAction(self.action_Icon%d)" %(i)
        #     exec(execStr)

    def DoAddMenusToMenuBar(self, menus):
        """
        Add menus to the MenuBar.

        :param `menus`: A list of menu instances.
        :type `menus`: tuple or list
        """
        # Add the Menus to the MenuBar
        [self.gMenuBar.addMenu(menu) for menu in menus]

    def CreateToolBars(self):
        """"""
        self.gToolBar_File = gToolBar_File = QToolBar('File')
        gToolBar_File.addAction(self.action_File_New)
        gToolBar_File.addAction(self.action_File_Open)
        gToolBar_File.addAction(self.action_File_Save)
        gToolBar_File.addAction(self.action_File_SaveAs)
        gToolBar_File.addAction(self.action_File_Print)
        gToolBar_File.addAction(self.action_File_Details)

        self.gToolBar_Edit = gToolBar_Edit = QToolBar('Edit')

        # Undo/Redo History Dropdown ToolButtons.
        self.undoToolButton = QToolButton()
        self.undoToolButton.setPopupMode(QToolButton.MenuButtonPopup)
        self.undoToolButton.setMenu(QMenu(self.undoToolButton))
        self.undoToolButton.setIcon(QIcon(gIconDir + os.sep + 'undo.png'))
        # self.undoToolButton.setIconSize(QSize(128, 128))
        self.undoToolButton.setDefaultAction(self.action_Edit_Undo)

        self.undoHistoryListBox = QListWidget(self)
        self.undoHistoryListBox.addItems(['Undo Item %d' % i for i in range(10)])
        action = QWidgetAction(self.undoToolButton)
        action.setDefaultWidget(self.undoHistoryListBox)
        self.undoToolButton.menu().addAction(action)

        gToolBar_Edit.addWidget(self.undoToolButton)

        self.redoToolButton = QToolButton()
        self.redoToolButton.setPopupMode(QToolButton.MenuButtonPopup)
        self.redoToolButton.setMenu(QMenu(self.redoToolButton))
        self.redoToolButton.setIcon(QIcon(gIconDir + os.sep + 'redo.png'))
        # self.redoToolButton.setIconSize(QSize(128, 128))
        self.redoToolButton.setDefaultAction(self.action_Edit_Redo)

        self.redoHistoryListBox = QListWidget(self)
        self.redoHistoryListBox.addItems(['Redo Item %d' % i for i in range(10)])
        action = QWidgetAction(self.redoToolButton)
        action.setDefaultWidget(self.redoHistoryListBox)
        self.redoToolButton.menu().addAction(action)
        gToolBar_Edit.addWidget(self.redoToolButton)

        # gToolBar_Edit.addAction(self.action_Edit_Undo)
        # gToolBar_Edit.addAction(self.action_Edit_Redo)

        gToolBar_Edit.addSeparator()
        gToolBar_Edit.addAction(self.action_Edit_Cut)
        gToolBar_Edit.addAction(self.action_Edit_Copy)
        gToolBar_Edit.addAction(self.action_Edit_Paste)

        self.gToolBar_View = gToolBar_View = QToolBar('View')
        gToolBar_View.addAction(self.action_View_Day)
        gToolBar_View.addAction(self.action_View_Night)
        gToolBar_View.addSeparator()
        gToolBar_View.addAction(self.action_View_Fullscreen)
        gToolBar_View.addAction(self.action_View_Maximize)
        gToolBar_View.addAction(self.action_View_Minimize)

        self.gToolBar_Draw = gToolBar_Draw = QToolBar('Draw')
        gToolBar_Draw.addAction(self.action_Draw_Circle)
        gToolBar_Draw.addAction(self.action_Draw_Dolphin)
        gToolBar_Draw.addAction(self.action_Draw_Ellipse)
        gToolBar_Draw.addAction(self.action_Draw_Heart)
        gToolBar_Draw.addAction(self.action_Draw_Line)
        gToolBar_Draw.addAction(self.action_Draw_Path)
        gToolBar_Draw.addAction(self.action_Draw_Point)
        gToolBar_Draw.addAction(self.action_Draw_Polygon)
        gToolBar_Draw.addAction(self.action_Draw_Polyline)
        gToolBar_Draw.addAction(self.action_Draw_Rectangle)
        gToolBar_Draw.addAction(self.action_Draw_SingleLineText)
        gToolBar_Draw.addAction(self.action_Draw_Snowflake)
        gToolBar_Draw.addAction(self.action_Draw_Star)

        self.gToolBar_Tools = gToolBar_Tools = QToolBar('Tools')
        gToolBar_Tools.addAction(self.action_Tools_Distance)
        gToolBar_Tools.addAction(self.action_Tools_LocatePoint)

        self.gToolBar_Modify = gToolBar_Modify = QToolBar('Modify')
        gToolBar_Modify.addAction(self.action_Modify_Delete)
        gToolBar_Modify.addAction(self.action_Modify_Move)
        gToolBar_Modify.addAction(self.action_Modify_Rotate)
        gToolBar_Modify.addAction(self.action_Modify_Scale)

        self.gToolBar_Dimension = gToolBar_Dimension = QToolBar('Dimension')
        gToolBar_Dimension.addAction(self.action_Dimension_QuickLeader)

        self.gToolBar_Sandbox = gToolBar_Sandbox = QToolBar('Sandbox')
        gToolBar_Sandbox.addAction(self.action_Sandbox_RGB)
        gToolBar_Sandbox.addAction(self.action_Sandbox_Sandbox)

        self.gToolBar_View_Zoom = gToolBar_View_Zoom = QToolBar('Zoom')
        # gToolBar_View_Zoom.addAction(self.action_View_Zoom_ZoomRealtime)
        # gToolBar_View_Zoom.addAction(self.action_View_Zoom_ZoomPrevious)
        # gToolBar_View_Zoom.addSeparator()
        gToolBar_View_Zoom.addAction(self.action_View_Zoom_ZoomWindow)
        gToolBar_View_Zoom.addAction(self.action_View_Zoom_ZoomDynamic)
        gToolBar_View_Zoom.addAction(self.action_View_Zoom_ZoomScale)
        gToolBar_View_Zoom.addSeparator()
        gToolBar_View_Zoom.addAction(self.action_View_Zoom_ZoomCenter)
        gToolBar_View_Zoom.addAction(self.action_View_Zoom_ZoomIn)
        gToolBar_View_Zoom.addAction(self.action_View_Zoom_ZoomOut)
        gToolBar_View_Zoom.addSeparator()
        gToolBar_View_Zoom.addAction(self.action_View_Zoom_ZoomSelected)
        gToolBar_View_Zoom.addAction(self.action_View_Zoom_ZoomAll)
        gToolBar_View_Zoom.addAction(self.action_View_Zoom_ZoomExtents)

        self.gToolBar_View_Pan = gToolBar_View_Pan = QToolBar('Pan')
        gToolBar_View_Pan.addAction(self.action_View_Pan_PanRealtime)
        gToolBar_View_Pan.addAction(self.action_View_Pan_PanPoint)
        gToolBar_View_Pan.addSeparator()
        gToolBar_View_Pan.addAction(self.action_View_Pan_PanLeft)
        gToolBar_View_Pan.addAction(self.action_View_Pan_PanRight)
        gToolBar_View_Pan.addAction(self.action_View_Pan_PanUp)
        gToolBar_View_Pan.addAction(self.action_View_Pan_PanDown)

        self.gToolBar_Help = gToolBar_Help = QToolBar('Help')
        gToolBar_Help.addAction(self.action_Help_Help)
        gToolBar_Help.addAction(self.action_Help_Changelog)
        gToolBar_Help.addAction(self.action_Help_TipOfTheDay)
        gToolBar_Help.addAction(self.action_Help_About)
        gToolBar_Help.addAction(self.action_Help_ContextHelp)

        self.CreateToolbar_Text()
        # self.gToolBar_Text = gToolBar_Text = QToolBar('Text')
        # gToolBar_Text.addAction(self.action_Text_Bold)
        # gToolBar_Text.addAction(self.action_Text_Italic)
        # gToolBar_Text.addAction(self.action_Text_Underline)
        # gToolBar_Text.addAction(self.action_Text_StrikeOut)
        # gToolBar_Text.addAction(self.action_Text_Overline)

        self.CreateToolbar_Layer()
        # self.gToolBar_Layer = gToolBar_Layer = QToolBar('Layer')
        # gToolBar_Layer.addAction(self.action_Layer_MakeLayerActive)
        # gToolBar_Layer.addAction(self.action_Layer_Layers)
        # gToolBar_Layer.addAction(self.action_Layer_Previous)

        # self.gToolBar_Properties = gToolBar_Properties = QToolBar('Properties')
        self.CreateToolbar_Properties()

        self.CreateToolbar_Prompt()

        # self.gToolBar_Test = gToolBar_Test = QToolBar('Test')
        # gToolBar_Test.setObjectName('TestToolbar')
        # gToolBar_Test.addAction(self.action_TEST01)
        # gToolBar_Test.addAction(self.action_TEST02)
        # gToolBar_Test.addAction(self.action_TEST03)
        # gToolBar_Test.addAction(self.action_TEST04)


        self.gToolBar_Icon = gToolBar_Icon = QToolBar('Icon')
        for i in (16, 24, 32, 48, 64, 128):
            execStr = """\
gToolBar_Icon.addAction(self.action_Icon%d)
""" %(i)
            exec(execStr)

        self._allToolBars = (
            gToolBar_File,
            gToolBar_Edit,
            gToolBar_View,
            gToolBar_View_Pan,
            gToolBar_View_Zoom,
            gToolBar_Dimension,
            gToolBar_Draw,
            gToolBar_Icon,
            gToolBar_Modify,
            gToolBar_Sandbox,
            gToolBar_Tools,
            gToolBar_Help,
            self.gToolBar_Text,
            self.gToolBar_Layer,
            self.gToolBar_Properties,
            self.gToolBar_Prompt,
            # gToolBar_Test
            )

        # Add the Toolbars.
        if PYQT4:
            Qt.ToolBarArea.BottomToolBarArea = 8
            Qt.ToolBarArea.TopToolBarArea = 4
            Qt.ToolBarArea.RightToolBarArea = 2
            Qt.ToolBarArea.LeftToolBarArea = 1
        self.addToolBar(Qt.ToolBarArea.TopToolBarArea, gToolBar_File)
        self.addToolBar(Qt.ToolBarArea.TopToolBarArea, gToolBar_Edit)
        self.addToolBar(Qt.ToolBarArea.TopToolBarArea, gToolBar_View)
        self.addToolBar(Qt.ToolBarArea.TopToolBarArea, gToolBar_Icon)
        self.addToolBar(Qt.ToolBarArea.LeftToolBarArea, gToolBar_Draw)
        self.addToolBarBreak(Qt.LeftToolBarArea)
        self.addToolBar(Qt.ToolBarArea.LeftToolBarArea, gToolBar_Tools)
        # self.addToolBarBreak(Qt.LeftToolBarArea)
        self.addToolBar(Qt.ToolBarArea.LeftToolBarArea, gToolBar_Modify)
        # self.addToolBarBreak(Qt.LeftToolBarArea)
        self.addToolBar(Qt.ToolBarArea.LeftToolBarArea, gToolBar_Dimension)
        # self.addToolBarBreak(Qt.LeftToolBarArea)
        self.addToolBar(Qt.ToolBarArea.LeftToolBarArea, gToolBar_Sandbox)
        self.addToolBarBreak(Qt.TopToolBarArea)
        self.addToolBar(Qt.ToolBarArea.TopToolBarArea, gToolBar_View_Zoom)
        self.addToolBar(Qt.ToolBarArea.TopToolBarArea, gToolBar_View_Pan)
        self.addToolBar(Qt.ToolBarArea.TopToolBarArea, gToolBar_Help)
        self.addToolBarBreak(Qt.TopToolBarArea)
        self.addToolBar(Qt.ToolBarArea.TopToolBarArea, self.gToolBar_Properties)
        self.addToolBar(Qt.ToolBarArea.TopToolBarArea, self.gToolBar_Layer)
        self.addToolBar(Qt.ToolBarArea.TopToolBarArea, self.gToolBar_Text)
        # self.addToolBar(Qt.ToolBarArea.RightToolBarArea, gToolBar_Test)
        self.addToolBar(Qt.ToolBarArea.BottomToolBarArea, self.gToolBar_Prompt)

        self.DoHideAllToolBars()

    def CreateToolbar_Layer(self):
        """"""
        self.gToolBar_Layer = gToolBar_Layer = QToolBar('Layer')

        gToolBar_Layer.setObjectName("toolbarLayer")
        gToolBar_Layer.addAction(self.action_Layer_MakeLayerActive)
        gToolBar_Layer.addAction(self.action_Layer_Layers)

        icontheme = self.getSettingsGeneralIconTheme()

        layerSelector = QComboBox(self)
        #TODO# layerSelector.setFocusProxy(prompt)
        # NOTE: Qt4.7 wont load icons without an extension...
        # TODO: Create layer pixmaps by concatenating several icons
        iconThemeDir = os.path.abspath(gIconDir + os.sep + '..' + os.sep + icontheme + os.sep)
        layerSelector.addItem(QIcon(iconThemeDir + "linetypebylayer" + ".png"), "0")
        layerSelector.addItem(QIcon(iconThemeDir + "linetypebylayer" + ".png"), "1")
        layerSelector.addItem(QIcon(iconThemeDir + "linetypebylayer" + ".png"), "2")
        layerSelector.addItem(QIcon(iconThemeDir + "linetypebylayer" + ".png"), "3")
        layerSelector.addItem(QIcon(iconThemeDir + "linetypebylayer" + ".png"), "4")
        layerSelector.addItem(QIcon(iconThemeDir + "linetypebylayer" + ".png"), "5")
        layerSelector.addItem(QIcon(iconThemeDir + "linetypebylayer" + ".png"), "6")
        layerSelector.addItem(QIcon(iconThemeDir + "linetypebylayer" + ".png"), "7")
        layerSelector.addItem(QIcon(iconThemeDir + "linetypebylayer" + ".png"), "8")
        layerSelector.addItem(QIcon(iconThemeDir + "linetypebylayer" + ".png"), "9")
        gToolBar_Layer.addWidget(layerSelector)
        #TODO# connect(layerSelector, SIGNAL(currentIndexChanged(int)), this, SLOT(layerSelectorIndexChanged(int)))

        gToolBar_Layer.addAction(self.action_Layer_Previous)

        #TODO# connect(toolbarLayer, SIGNAL(topLevelChanged(bool)), this, SLOT(floatingChangedToolBar(bool)))

    def CreateToolbar_Properties(self):
        """"""
        self.gToolBar_Properties = gToolBar_Properties = QToolBar('Properties')
        gToolBar_Properties.setObjectName("toolbarProperties")

        colorSelector = QComboBox(self)
        # NOTE: Qt4.7 wont load icons without an extension...
        colorSelector.addItem(QIcon(gIconDir + os.sep + "colorbylayer.png"), "ByLayer")
        colorSelector.addItem(QIcon(gIconDir + os.sep + "colorbyblock.png"), "ByBlock")
        colorSelector.addItem(QIcon(gIconDir + os.sep + "colorred.png"),     self.tr("Red"),     qRgb(255,   0,   0))
        colorSelector.addItem(QIcon(gIconDir + os.sep + "coloryellow.png"),  self.tr("Yellow"),  qRgb(255, 255,   0))
        colorSelector.addItem(QIcon(gIconDir + os.sep + "colorgreen.png"),   self.tr("Green"),   qRgb(  0, 255,   0))
        colorSelector.addItem(QIcon(gIconDir + os.sep + "colorcyan.png"),    self.tr("Cyan"),    qRgb(  0, 255, 255))
        colorSelector.addItem(QIcon(gIconDir + os.sep + "colorblue.png"),    self.tr("Blue"),    qRgb(  0,   0, 255))
        colorSelector.addItem(QIcon(gIconDir + os.sep + "colormagenta.png"), self.tr("Magenta"), qRgb(255,   0, 255))
        colorSelector.addItem(QIcon(gIconDir + os.sep + "colorwhite.png"),   self.tr("White"),   qRgb(255, 255, 255))
        colorSelector.addItem(QIcon(gIconDir + os.sep + "colorother.png"),   self.tr("Other..."))
        gToolBar_Properties.addWidget(colorSelector)

        gToolBar_Properties.addSeparator()

        linetypeSelector = QComboBox(self)
        #TODO# linetypeSelector.setFocusProxy(prompt)
        linetypeSelector.addItem(QIcon(gIconDir + os.sep + "linetypebylayer.png"),    "ByLayer")
        linetypeSelector.addItem(QIcon(gIconDir + os.sep + "linetypebyblock.png"),    "ByBlock")
        linetypeSelector.addItem(QIcon(gIconDir + os.sep + "linetypecontinuous.png"), "Continuous")
        linetypeSelector.addItem(QIcon(gIconDir + os.sep + "linetypehidden.png"),     "Hidden")
        linetypeSelector.addItem(QIcon(gIconDir + os.sep + "linetypecenter.png"),     "Center")
        linetypeSelector.addItem(QIcon(gIconDir + os.sep + "linetypeother.png"),      "Other...")
        gToolBar_Properties.addWidget(linetypeSelector)
        #TODO# connect(linetypeSelector, SIGNAL(currentIndexChanged(int)), this, SLOT(linetypeSelectorIndexChanged(int)))

        gToolBar_Properties.addSeparator()

        lineweightSelector = QComboBox(self)
        #TODO# lineweightSelector.setFocusProxy(prompt)
        lineweightSelector.addItem(QIcon(gIconDir + os.sep + "lineweightbylayer.png"), "ByLayer", -2.00)
        lineweightSelector.addItem(QIcon(gIconDir + os.sep + "lineweightbyblock.png"), "ByBlock", -1.00)
        lineweightSelector.addItem(QIcon(gIconDir + os.sep + "lineweightdefault.png"), "Default",  0.00)
        # TODO: Thread weight is weird. See http://en.wikipedia.org/wiki/Thread_(yarn)#Weight
        lineweightSelector.addItem(QIcon(gIconDir + os.sep + "lineweight01.png"), "0.00 mm", 0.00)
        lineweightSelector.addItem(QIcon(gIconDir + os.sep + "lineweight02.png"), "0.05 mm", 0.05)
        lineweightSelector.addItem(QIcon(gIconDir + os.sep + "lineweight03.png"), "0.15 mm", 0.15)
        lineweightSelector.addItem(QIcon(gIconDir + os.sep + "lineweight04.png"), "0.20 mm", 0.20)
        lineweightSelector.addItem(QIcon(gIconDir + os.sep + "lineweight05.png"), "0.25 mm", 0.25)
        lineweightSelector.addItem(QIcon(gIconDir + os.sep + "lineweight06.png"), "0.30 mm", 0.30)
        lineweightSelector.addItem(QIcon(gIconDir + os.sep + "lineweight07.png"), "0.35 mm", 0.35)
        lineweightSelector.addItem(QIcon(gIconDir + os.sep + "lineweight08.png"), "0.40 mm", 0.40)
        lineweightSelector.addItem(QIcon(gIconDir + os.sep + "lineweight09.png"), "0.45 mm", 0.45)
        lineweightSelector.addItem(QIcon(gIconDir + os.sep + "lineweight10.png"), "0.50 mm", 0.50)
        lineweightSelector.addItem(QIcon(gIconDir + os.sep + "lineweight11.png"), "0.55 mm", 0.55)
        lineweightSelector.addItem(QIcon(gIconDir + os.sep + "lineweight12.png"), "0.60 mm", 0.60)
        lineweightSelector.addItem(QIcon(gIconDir + os.sep + "lineweight13.png"), "0.65 mm", 0.65)
        lineweightSelector.addItem(QIcon(gIconDir + os.sep + "lineweight14.png"), "0.70 mm", 0.70)
        lineweightSelector.addItem(QIcon(gIconDir + os.sep + "lineweight15.png"), "0.75 mm", 0.75)
        lineweightSelector.addItem(QIcon(gIconDir + os.sep + "lineweight16.png"), "0.80 mm", 0.80)
        lineweightSelector.addItem(QIcon(gIconDir + os.sep + "lineweight17.png"), "0.85 mm", 0.85)
        lineweightSelector.addItem(QIcon(gIconDir + os.sep + "lineweight18.png"), "0.90 mm", 0.90)
        lineweightSelector.addItem(QIcon(gIconDir + os.sep + "lineweight19.png"), "0.95 mm", 0.95)
        lineweightSelector.addItem(QIcon(gIconDir + os.sep + "lineweight20.png"), "1.00 mm", 1.00)
        lineweightSelector.addItem(QIcon(gIconDir + os.sep + "lineweight21.png"), "1.05 mm", 1.05)
        lineweightSelector.addItem(QIcon(gIconDir + os.sep + "lineweight22.png"), "1.10 mm", 1.10)
        lineweightSelector.addItem(QIcon(gIconDir + os.sep + "lineweight23.png"), "1.15 mm", 1.15)
        lineweightSelector.addItem(QIcon(gIconDir + os.sep + "lineweight24.png"), "1.20 mm", 1.20)
        lineweightSelector.setMinimumWidth(lineweightSelector.iconSize().width() * 6) # Fix dropdown readability
        gToolBar_Properties.addWidget(lineweightSelector)
        #TODO# connect(lineweightSelector, SIGNAL(currentIndexChanged(int)), this, SLOT(lineweightSelectorIndexChanged(int)))

        #TODO# connect(toolbarProperties, SIGNAL(topLevelChanged(bool)), this, SLOT(floatingChangedToolBar(bool)))

    def CreateToolbar_Text(self):
        """"""
        self.gToolBar_Text = gToolBar_Text = QToolBar('Text')

        gToolBar_Text.setObjectName("toolbarText")

        textFontSelector = QFontComboBox(self)
        gToolBar_Text.addWidget(textFontSelector)
        #TODO# textFontSelector.setCurrentFont(QFont(getSettingsTextFont()))
        #TODO# connect(textFontSelector, SIGNAL(currentFontChanged(const QFont&)), this, SLOT(textFontSelectorCurrentFontChanged(const QFont&)))

        gToolBar_Text.addAction(self.action_Text_Bold)
        gToolBar_Text.addAction(self.action_Text_Italic)
        gToolBar_Text.addAction(self.action_Text_Underline)
        gToolBar_Text.addAction(self.action_Text_StrikeOut)
        gToolBar_Text.addAction(self.action_Text_Overline)

        textSizeSelector = QComboBox(self)
        #TODO# textSizeSelector.setFocusProxy(prompt)
        textSizeSelector.addItem("6 pt",   6);
        textSizeSelector.addItem("8 pt",   8);
        textSizeSelector.addItem("9 pt",   9);
        textSizeSelector.addItem("10 pt", 10);
        textSizeSelector.addItem("11 pt", 11);
        textSizeSelector.addItem("12 pt", 12);
        textSizeSelector.addItem("14 pt", 14);
        textSizeSelector.addItem("18 pt", 18);
        textSizeSelector.addItem("24 pt", 24);
        textSizeSelector.addItem("30 pt", 30);
        textSizeSelector.addItem("36 pt", 36);
        textSizeSelector.addItem("48 pt", 48);
        textSizeSelector.addItem("60 pt", 60);
        textSizeSelector.addItem("72 pt", 72);
        #TODO# setTextSize(getSettingsTextSize())
        gToolBar_Text.addWidget(textSizeSelector)
        #TODO# connect(textSizeSelector, SIGNAL(currentIndexChanged(int)), this, SLOT(textSizeSelectorIndexChanged(int)))

        #TODO# connect(toolbarText, SIGNAL(topLevelChanged(bool)), this, SLOT(floatingChangedToolBar(bool)))

    def CreateToolbar_Prompt(self):
        """"""
        self.gToolBar_Prompt = gToolBar_Prompt = QToolBar('Prompt')

        prompt = CmdPrompt(self)
        gToolBar_Prompt.setObjectName("toolbarPrompt")
        gToolBar_Prompt.addWidget(prompt)
        gToolBar_Prompt.setAllowedAreas(Qt.TopToolBarArea | Qt.BottomToolBarArea)
        #TODO# connect(gToolBar_Prompt, SIGNAL(topLevelChanged(bool)), prompt, SLOT(floatingChanged(bool)))


    def DoHideAllToolBars(self):
        """"""
        [toolBar.hide() for toolBar in self._allToolBars]

    def ShowAllToolBars(self):
        """"""
        [toolBar.show() for toolBar in self._allToolBars]

    def CreateStatusBar(self):
        """"""
        # self.gStatusBar = StatusBar(self, self)
        self.gStatusBar = StatusBar(self)
        self.setStatusBar(self.gStatusBar)
        self.gStatusBar.hide()

        # self.gStatusBar = self.statusBar()
        # self.gStatusBar.showMessage('Welcome to Embroidermodder v2.0 build/rev #-----')

    def CreateDockWindows(self):
        """"""
        self.gDockWindow_Properties = gDockWindow_Properties = PropertyEditor(
            iconDirectory=gIconDir, pickAddMode=False, widgetToFocus=None, title=self.tr('Properties'), parent=self)
        # self.gDockWindow_Properties = gDockWindow_Properties = QDockWidget("Properties", self)
        # gDockWindow_Properties.setAllowedAreas(Qt.LeftDockWidgetArea | Qt.RightDockWidgetArea)
        # self.propertiesList = QListWidget(gDockWindow_Properties)
        # self.propertiesList.addItems(['Properties Item %d' % i for i in range(10)])
        # gDockWindow_Properties.setWidget(self.propertiesList)
        self.addDockWidget(Qt.LeftDockWidgetArea, gDockWindow_Properties)


        self.gDockWindow_History = gDockWindow_History = UndoEditor(gIconDir, self.gMDIArea, self)
        # gDockWindow_History.setTitle(self.tr("History"))
        # self.gDockWindow_History = gDockWindow_History = QDockWidget("History", self)
        # gDockWindow_History.setAllowedAreas(Qt.LeftDockWidgetArea | Qt.RightDockWidgetArea)
        # self.historyList = QListWidget(gDockWindow_History)
        # self.historyList.addItems(['History Item %d' % i for i in range(10)])
        # gDockWindow_History.setWidget(self.historyList)
        self.addDockWidget(Qt.LeftDockWidgetArea, gDockWindow_History)

        self._allDockWindows = (
            gDockWindow_Properties,
            gDockWindow_History
            )

        self.DoHideAllDockWindows()

    def DoHideAllDockWindows(self):
        """"""
        [dockWindow.hide() for dockWindow in self._allDockWindows]

    def ShowAllDockWindows(self):
        """"""
        [dockWindow.show() for dockWindow in self._allDockWindows]

    # def DoReadSettings(self):
    #     """
    #     QSettings(organization[, application=''[, parent=None]])
    #     """
    #     settings = QSettings('Embroidermodder',
    #                          'Embroidermodder - PySide/PyQt GUI')
    #     pos = settings.value('pos', QPoint(200, 200))
    #     size = settings.value('size', QSize(400, 400))
    #     self.resize(size)
    #     self.move(pos)
    #
    # def DoWriteSettings(self):
    #     settings = QSettings('Embroidermodder',
    #                          'Embroidermodder - PySide GUI')
    #     settings.setValue('pos', self.pos())
    #     settings.setValue('size', self.size())

    def OnShowSettingsDialog(self, showTab=None):
        """"""
        openingTab = showTab or 'General'
        dialog = Settings_Dialog(self, openingTab, self)
        dialog.show()

    def OnAbout(self):
        """
        Show the Embroidermodder About Dialog.
        """
        dialog = AboutDialog(self)
        dialog.show()

    def getSettingsGeneralIconTheme(self): #TODO
        return self.settings_general_icon_theme
    def getSettingsGeneralIconSize(self): #TODO
        return self.settings_general_icon_size
    def getSettingsGeneralLanguage(self): #TODO
        return 'Default'
    def getSettingsGeneralMdiBGUseLogo(self): #TODO
        return True
    def getSettingsGeneralMdiBGLogo(self): #TODO
        return True
    def getSettingsGeneralMdiBGUseTexture(self): #TODO
        return True
    def getSettingsGeneralMdiBGTexture(self): #TODO
        return True
    def getSettingsGeneralMdiBGUseColor(self): #TODO
        return False
    def getSettingsGeneralMdiBGColor(self): #TODO
        return QColor(Qt.white)
    def getSettingsGeneralTipOfTheDay(self): #TODO
        return True
    def getSettingsGeneralSystemHelpBrowser(self): #TODO
        return True

    def getSettingsDisplayShowScrollBars(self): #TODO
        return True
    def getSettingsDisplayScrollBarWidgetNum(self): #TODO
        return 0
    def getSettingsDisplayCrossHairColor(self): #TODO
        return QColor(Qt.black)
    def getSettingsDisplayBGColor(self): #TODO
        return QColor(Qt.white)
    def getSettingsDisplaySelectBoxLeftColor(self): #TODO
        return QColor(Qt.white)
    def getSettingsDisplaySelectBoxLeftFill(self): #TODO
        return QColor(Qt.white)
    def getSettingsDisplaySelectBoxRightColor(self): #TODO
        return QColor(Qt.white)
    def getSettingsDisplaySelectBoxRightFill(self): #TODO
        return QColor(Qt.white)
    def getSettingsDisplaySelectBoxAlpha(self): #TODO
        return 255
    def getSettingsDisplayZoomScaleIn(self): #TODO
        return 1.0
    def getSettingsDisplayZoomScaleOut(self): #TODO
        return 1.0

    def getSettingsPromptTextColor(self): #TODO
        return QColor(Qt.white)
    def getSettingsPromptBGColor(self): #TODO
        return QColor(Qt.white)
    def getSettingsPromptFontFamily(self): #TODO
        return 'Normal'
    def getSettingsPromptFontSize(self): #TODO
        return 12
    def getSettingsPromptSaveHistory(self): #TODO
        return True
    def getSettingsPromptSaveHistoryAsHtml(self): #TODO
        return True

    def getSettingsCustomFilter(self): #TODO
        # return settings_opensave_custom_filter
        return WILDCARD_SUPPORTED
    def getSettingsRecentMaxFiles(self): #TODO
        return 10
    def getSettingsOpenSaveTrimDstNumJumps(self): #TODO
        return True

    def getSettingsPrintingUseLastDevice(self): #TODO
        return True
    def getSettingsPrintingDisableBG(self): #TODO
        return True

    def getSettingsGridShowOnLoad(self): #TODO
        return True
    def getSettingsGridShowOrigin(self): #TODO
        return True
    def getSettingsGridColorMatchCrossHair(self): #TODO
        return True
    def getSettingsDisplayCrossHairColor(self): #TODO
        return QColor(Qt.white)
    def getSettingsGridColor(self): #TODO
        return QColor(Qt.white)
    def getSettingsGridLoadFromFile(self): #TODO
        return True
    def getSettingsGridType(self): #TODO
        return "Rectangular"
    def getSettingsGridCenterOnOrigin(self): #TODO
        return True
    def getSettingsGridCenterX(self): #TODO
        return 0
    def getSettingsGridCenterY(self): #TODO
        return 0
    def getSettingsGridSizeX(self): #TODO
        return True
    def getSettingsGridSizeY(self): #TODO
        return True
    def getSettingsGridSpacingX(self): #TODO
        return True
    def getSettingsGridSpacingY(self): #TODO
        return True
    def getSettingsGridSizeRadius(self): #TODO
        return True
    def getSettingsGridSpacingRadius(self): #TODO
        return True
    def getSettingsGridSpacingAngle(self): #TODO
        return True
    def getSettingsRulerShowOnLoad(self): #TODO
        return True
    def getSettingsRulerMetric(self): #TODO
        return True
    def getSettingsRulerColor(self): #TODO
        return QColor(Qt.white)
    def getSettingsRulerPixelSize(self): #TODO
        return 1

    def getSettingsQSnapEndPoint(self): #TODO
        return True
    def getSettingsQSnapMidPoint(self): #TODO
        return True
    def getSettingsQSnapCenter(self): #TODO
        return True
    def getSettingsQSnapNode(self): #TODO
        return True
    def getSettingsQSnapQuadrant(self): #TODO
        return True
    def getSettingsQSnapIntersection(self): #TODO
        return True
    def getSettingsQSnapExtension(self): #TODO
        return True
    def getSettingsQSnapInsertion(self): #TODO
        return True
    def getSettingsQSnapPerpendicular(self): #TODO
        return True
    def getSettingsQSnapTangent(self): #TODO
        return True
    def getSettingsQSnapNearest(self): #TODO
        return True
    def getSettingsQSnapApparent(self): #TODO
        return True
    def getSettingsQSnapParallel(self): #TODO
        return True
    def getSettingsQSnapLocatorColor(self): #TODO
        return QColor(Qt.white)
    def getSettingsQSnapLocatorSize(self): #TODO
        return True
    def getSettingsQSnapApertureSize(self): #TODO
        return True

    def getSettingsLwtShowLwt(self): #TODO
        return True
    def getSettingsLwtRealRender(self): #TODO
        return True
    def getSettingsLwtDefaultLwt(self): #TODO
        return True

    def getSettingsSelectionModePickFirst(self): #TODO
        return True
    def getSettingsSelectionModePickAdd(self): #TODO
        return True
    def getSettingsSelectionModePickDrag(self): #TODO
        return True
    def getSettingsSelectionCoolGripColor(self): #TODO
        return QColor(Qt.white)
    def getSettingsSelectionHotGripColor(self): #TODO
        return QColor(Qt.white)
    def getSettingsSelectionGripSize(self): #TODO
        return True
    def getSettingsSelectionPickBoxSize(self): #TODO
        return True

    # def (self): #TODO
        # return True
    # def (self): #TODO
        # return True
    # def (self): #TODO
        # return True
    # def (self): #TODO
        # return True
    # def (self): #TODO
        # return True
    # def (self): #TODO
        # return True
    # def (self): #TODO
        # return True
    # def (self): #TODO
        # return True
    # def (self): #TODO
        # return True

################################################################################
################################################################################
################################################################################
################################################################################

## class EmbroiderCommanderAutoCompleter(QCompleter):
##     def __init__(self, parent=None, completions=['LINE', 'HEART', 'DOLPHIN', 'CIRCLE', 'STAR', 'six', 'seven', 'eight', 'nine', 'ten']):
##         super(EmbroiderCommanderAutoCompleter, self).__init__(completions, parent)
##
##         # self.completerList = QtCore.QStringList(stringList)
##         # for content in completerContents:
##             # self.completerList.append(QtCore.QString(content))
##
##         # self.setModel(QStringList(completions))
##         # self.completer = QtGui.QCompleter(self.completerList, self)
##         self.setCompletionMode(QCompleter.PopupCompletion)
##         self.setCaseSensitivity(Qt.CaseInsensitive)
##         # self.setCompleter(self.completer)




if __name__ == '__main__':

    import sys

    gApp = QApplication(sys.argv)

    gTranslator = QTranslator()
    gTranslator.load(gTrDir + os.sep + 'english' + os.sep + 'embroidermodder2_english.qm')
    # gTranslator.load(gTrDir + os.sep + 'french' + os.sep + 'embroidermodder2_french.qm')
    gApp.installTranslator(gTranslator)

    gMainWin = EmbroidermodderMainWindow()
    gMainWin.setWindowTitle("Embroidermodder - PySide/PyQt GUI[*]")
    gMainWin.show()
    sys.exit(gApp.exec_())


# kate: bom off; indent-mode cstyle; indent-width 4; replace-trailing-space-save on;
