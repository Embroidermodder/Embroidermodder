#!/usr/bin/env python
# -*- coding: utf-8 -*-

"""
===========================
|module_summary| mdiarea.py
===========================

TOWRITE

Classes summary:
================

================================ ============================
:class:`~MdiArea`                TOWRITE
================================ ============================

-------------------------------------------------------------
"""

#-Imports.----------------------------------------------------------------------

#--Python Imports.
import os

#--PySide/PyQt Imports.
try:
    from PySide.QtCore import qDebug, Qt, QSize, QPointF
    from PySide.QtGui import QMdiArea, QColor, QPainter, QBrush, QPixmap, QLinearGradient, QMenu
    PYSIDE = True
    PYQT4 = False
except ImportError:
    raise
#    from PyQt4.QtCore import qDebug, Qt, QSize
#    from PyQt4.QtGui import QMdiArea, QColor, QPainter, QBrush, QPixmap, QLinearGradient, QMenu
#    PYSIDE = False
#    PYQT4 = True


EMBROIDERBLUE1 = '#0C6AB0' # (12, 106, 176)
EMBROIDERBLUE2 = '#55C4E6' # (85, 196, 230)


class MdiArea(QMdiArea):  # MdiArea::MdiArea(MainWindow* mw, QWidget *parent) : QMdiArea(parent), mainWin(mw)
    """
    Subclass of `QMdiArea`_

    TOWRITE
    """

    def __init__(self, mw, parent=None):
        """
        Default class constructor.

        :param `mw`: Pointer to a application main window instance.
        :type `mw`: `MainWindow`_
        :param `parent`: Pointer to a parent widget instance.
        :type `parent`: `QWidget`_
        """
        super(MdiArea, self).__init__(parent)

        self.mainWin = mw
        self.gSpiralsImgPath = mw.gImgDir + os.sep + 'texture-spirals.png'
        self.gLogoSpiralsImgPath = mw.gImgDir + os.sep + 'logo-spirals.png'

        try:  #if QT_VERSION >= 0x040800
            self.setTabsClosable(True)
        except AttributeError:
            pass

        self.useLogo = False
        self.useTexture = False
        self.useColor = False

        self.bgLogo = QPixmap()
        self.bgTexture = QPixmap(self.gSpiralsImgPath)
        self.bgColor = QColor()

        self.bgLogo = QPixmap(self.gLogoSpiralsImgPath)

        # Brushes
        self.colorBrush = QBrush(QColor(EMBROIDERBLUE1))
        self.backgroundBrush = QBrush(QPixmap(self.gSpiralsImgPath))
        linearGrad = QLinearGradient(QPointF(0, 0), QPointF(400, 400))
        linearGrad.setColorAt(0, QColor(EMBROIDERBLUE1))
        linearGrad.setColorAt(1, QColor(EMBROIDERBLUE2))
        self.gradientBrush = QBrush(linearGrad)

        self.setHorizontalScrollBarPolicy(Qt.ScrollBarAsNeeded)
        self.setVerticalScrollBarPolicy(Qt.ScrollBarAsNeeded)
        self.setActivationOrder(QMdiArea.ActivationHistoryOrder)

        self.setFocusPolicy(Qt.WheelFocus)
        self.setFocus()

        self.setAcceptDrops(True)
        self.doSetDocumentMode(True)

    def __del__(self):
        """Class destructor."""
        qDebug("MdiArea Destructor")

    def useBackgroundLogo(self, use):
        """
        TOWRITE

        :param `use`: TOWRITE
        :type `use`: bool
        """
        self.useLogo = use
        self.forceRepaint()

    def useBackgroundTexture(self, use):
        """
        TOWRITE

        :param `use`: TOWRITE
        :type `use`: bool
        """
        self.useTexture = use
        self.forceRepaint()

    def useBackgroundColor(self, use):
        """
        TOWRITE

        :param `use`: TOWRITE
        :type `use`: bool
        """
        self.useColor = use
        self.forceRepaint()

    def setBackgroundLogo(self, fileName):
        """
        TOWRITE

        :param `fileName`: TOWRITE
        :type `fileName`: QString
        """
        self.bgLogo.load(fileName)
        self.forceRepaint()

    def setBackgroundTexture(self, fileName):
        """
        TOWRITE

        :param `fileName`: TOWRITE
        :type `fileName`: QString
        """
        self.bgTexture.load(fileName)
        self.forceRepaint()

    def setBackgroundColor(self, color):
        """
        TOWRITE

        :param `color`: TOWRITE
        :type `color`: `QColor`_
        """
        if not color:  # .isValid()
            self.bgColor = self.background().color()
        else:
            self.bgColor = color

        self.forceRepaint()

    def mouseDoubleClickEvent(self, event):
        """
        Handles the ``mouseDoubleClickEvent`` event for :class:`MdiArea`.

        :param `event`: A `QMouseEvent`_ to be processed.
        """
        qDebug('%s' % event.button())
        evtBtn = event.button()
        if evtBtn == Qt.LeftButton: # return 1
            self.mainWin.openFile()
        elif evtBtn == Qt.RightButton: # return 2
            qDebug('DoubleRightClick')
        elif evtBtn == Qt.MiddleButton: # return 4
            qDebug('DoubleMiddleClick')
        elif evtBtn == Qt.XButton1: # Aux1 return 8
            qDebug('DoubleAux1Click')
        elif evtBtn == Qt.XButton2: # Aux2 return 16
            qDebug('DoubleAux2Click')

    ## def paintEvent(self, event):
    ##     """
    ##     Handles the ``paintEvent`` event for :class:`MdiArea`.
    ##
    ##     :param `event`: A `QPaintEvent`_ to be processed.
    ##     """
    ##     vport = self.viewport()  # QWidget*
    ##     rect = vport.rect()  # QRect
    ##
    ##     painter = QPainter(vport)
    ##     painter.setRenderHint(QPainter.SmoothPixmapTransform)
    ##
    ##     # Always fill with a solid color first.
    ##     if self.useColor:
    ##         painter.fillRect(rect, self.bgColor)
    ##     else:
    ##         painter.fillRect(rect, self.background())
    ##
    ##     # Then overlay the texture.
    ##     if self.useTexture:
    ##         bgBrush = QBrush(self.bgTexture)
    ##         painter.fillRect(rect, bgBrush)
    ##
    ##     # Overlay the logo last.
    ##     if self.useLogo:
    ##         bgLogo = self.bgLogo
    ##         # Center the pixmap.
    ##         dx = (rect.width() - bgLogo.width()) / 2     # int
    ##         dy = (rect.height() - bgLogo.height()) / 2   # int
    ##         painter.drawPixmap(dx, dy, bgLogo.width(), bgLogo.height(), bgLogo)

    def paintEvent(self, event):
        """
        Handles the ``paintEvent`` event for :class:`MdiArea`.

        :param `event`: A `QPaintEvent`_ to be processed.
        """
        vport = self.viewport()
        rect = vport.rect()

        painter = QPainter(vport)
        painter.setRenderHint(painter.SmoothPixmapTransform)

        # Always fill with a solid color first
        if self.useColor:
            # painter.fillRect(rect, self.colorBrush)
            painter.fillRect(rect, self.bgColor)
        else:
            painter.fillRect(rect, self.background())

        # Then overlay the texture
        if self.useTexture:
            # painter.fillRect(rect, self.backgroundBrush)
            bgBrush = QBrush(self.bgTexture)
            painter.fillRect(rect, bgBrush)
            

        # Overlay the logo last
        if self.useLogo:
            if not len(self.subWindowList()):  # Nothing is open.
                cSizeW, cSizeH = rect.width(), rect.height()
                bgLogoW, bgLogoH = self.bgLogo.width(), self.bgLogo.height()
                if bgLogoW > cSizeW:
                    # Proportional Scaling an Image.
                    newHeight = bgLogoH * cSizeW // bgLogoW
                    scaledLogo = self.bgLogo.scaled(cSizeW, newHeight)
                    painter.drawPixmap(0,
                                       cSizeH // 2 - scaledLogo.height() // 2,
                                       scaledLogo)
                else:
                    painter.drawPixmap((cSizeW - bgLogoW) // 2,
                                       (cSizeH - bgLogoH) // 2,
                                       self.bgLogo)
            else:
                # Center the pixmap
                dx = (rect.width() - self.bgLogo.width()) / 2
                dy = (rect.height() - self.bgLogo.height()) / 2
                painter.drawPixmap(dx, dy,
                                   self.bgLogo.width(), self.bgLogo.height(),
                                   self.bgLogo)

    def cascade(self):
        """
        TOWRITE
        """
        self.cascadeSubWindows()
        self.zoomExtentsAllSubWindows()

    def tile(self):
        """
        TOWRITE
        """
        self.tileSubWindows()
        self.zoomExtentsAllSubWindows()

    def zoomExtentsAllSubWindows(self):
        """
        TOWRITE
        """
        for window in self.subWindowList():  # foreach(QMdiSubWindow* window, subWindowList())
            mdiWin = window  # MdiWindow* mdiWin = qobject_cast<MdiWindow*>(window);
            if mdiWin:
                v = mdiWin.getView()  # View*
                if v:
                    v.recalculateLimits()
                    v.zoomExtents()

    def forceRepaint(self):
        """
        TOWRITE
        """
        # HACK: Take that QMdiArea!
        hack = self.size()  # QSize
        self.resize(hack + QSize(1, 1))
        self.resize(hack)

    def moveEvent(self, event):
        """
        Handles the ``moveEvent`` event for :class:`MDIArea`.

        :param `event`: A `QMoveEvent`_ to be processed.
        """
        # Dragging while MouseButton is down.
        qDebug("QMdiArea moveEvent")

    def contextMenuEvent(self, event):
        """
        Handles the ``contextMenuEvent`` event for :class:`MDIArea`.

        :param `event`: A `QContextMenuEvent`_ to be processed.
        """
        mainWin = self.mainWin
        if not len(self.subWindowList()): # Nothing is open.
            # Build a menu suitable for the startup screen.
            menu = QMenu(self)
            menu.addAction(mainWin.actionHash["ACTION_new"])
            menu.addAction(mainWin.actionHash["ACTION_open"])
            menu.addAction(mainWin.actionHash["ACTION_settingsdialog"])
            menu.addAction(mainWin.actionHash["ACTION_help"])
            menu.addAction(mainWin.actionHash["ACTION_about"])
            menu.addAction(mainWin.actionHash["ACTION_exit"])
            menu.popup(self.mapToGlobal(event.pos()))
            # menu.exec_(self.mapToGlobal(event.pos()))
        else:
            # Build a menu suitable for when the mdi workspace is open.
            mainWin.file_menu.popup(self.mapToGlobal(event.pos()))

        event.accept()
        qDebug("QMdiArea contextMenuEvent")

    def doSetDocumentMode(self, enabled=False):
        """
        Set the document mode for :class:`MDIArea`.

        :param `enabled`: Whether the tab bar is set to document mode in tabbed view mode.
         Document mode is disabled by default.
        :type `enabled`: bool
        """
        self.setDocumentMode(enabled)


# kate: bom off; indent-mode python; indent-width 4; replace-trailing-space-save on;
