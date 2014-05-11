#!/usr/bin/env python
# -*- coding: utf-8 -*-

"""
statusbar_button.py
===================

TOWRITE
"""

#-Imports.---------------------------------------------------------------------
#--Python Imports.
import os

#--PySide/PyQt Imports.
try:
    from PySide.QtCore import qDebug, Qt
    from PySide.QtGui import QApplication, QAction, QIcon, QMenu, QToolButton
except ImportError:
    raise
    # from PyQt4.QtCore import qDebug, Qt, QApplication
    # from PyQt4.QtGui import QApplication, QAction, QIcon, QMenu, QToolButton


class StatusBarButton(QToolButton):
    """
    A `QToolButton`_ GUI element to be injected into a `QStatusBar`_.

    Subclass of `QToolButton`_
    """
    def __init__(self, buttonText, mw, statbar, parent=None):
        """
        Default class constructor.

        :param `buttonText`: the text for the button label.
        :type `buttonText`: QString
        :param `mw`: The application's MainWindow instance.
        :type `mw`: `QMainWindow`_
        :param `statbar`: The parent statusbar instance of this button.
        :type `statbar`: `QStatusBar`_
        :param `parent`: parent widget instance of this QToolButton.
        :type `parent`: `QWidget`_
        """
        super(StatusBarButton, self).__init__(parent)

        self.mainWin = mw
        self.statusbar = statbar

        self.setObjectName("StatusBarButton" + buttonText)

        self.setText(buttonText)
        self.setAutoRaise(True)
        self.setCheckable(True)

        objName = self.objectName()
        if   objName == "StatusBarButtonSNAP":
            self.toggled.connect(self.toggleSnap)
        elif objName == "StatusBarButtonGRID":
            self.toggled.connect(self.toggleGrid)
        elif objName == "StatusBarButtonRULER":
            self.toggled.connect(self.toggleRuler)
        elif objName == "StatusBarButtonORTHO":
            self.toggled.connect(self.toggleOrtho)
        elif objName == "StatusBarButtonPOLAR":
            self.toggled.connect(self.togglePolar)
        elif objName == "StatusBarButtonQSNAP":
            self.toggled.connect(self.toggleQSnap)
        elif objName == "StatusBarButtonQTRACK":
            self.toggled.connect(self.toggleQTrack)
        elif objName == "StatusBarButtonLWT":
            self.toggled.connect(self.toggleLwt)

    def contextMenuEvent(self, event):
        """
        Handles the ``contextMenuEvent`` event for :class:`StatusBarButton`.

        :param `event`: A `QContextMenuEvent`_ to be processed.
        """
        QApplication.setOverrideCursor(Qt.ArrowCursor)
        menu = QMenu(self)
        objName = self.objectName()
        themeDir = gIconDir + os.sep + '..' + os.sep + self.mainWin.getSettingsGeneralIconTheme() + os.sep
        s = "&Settings..."
        if objName == "StatusBarButtonSNAP":

            settingsSnapAction = QAction(QIcon(themeDir + "gridsnapsettings.png"), s, menu)
            settingsSnapAction.triggered.connect(self.settingsSnap)
            menu.addAction(settingsSnapAction)

        elif objName == "StatusBarButtonGRID":

            settingsGridAction = QAction(QIcon(themeDir + "gridsettings.png"), s, menu)
            settingsGridAction.triggered.connect(self.settingsGrid)
            menu.addAction(settingsGridAction)

        elif objName == "StatusBarButtonRULER":

            settingsRulerAction = QAction(QIcon(themeDir + "rulersettings.png"), s, menu)
            settingsRulerAction.triggered.connect(self.settingsRuler)
            menu.addAction(settingsRulerAction)

        elif objName == "StatusBarButtonORTHO":

            settingsOrthoAction = QAction(QIcon(themeDir + "orthosettings.png"), s, menu)
            settingsOrthoAction.triggered.connect(self.settingsOrtho)
            menu.addAction(settingsOrthoAction)

        elif objName == "StatusBarButtonPOLAR":

            settingsPolarAction = QAction(QIcon(themeDir + "polarsettings.png"), s, menu)
            settingsPolarAction.triggered.connect(self.settingsPolar)
            menu.addAction(settingsPolarAction)

        elif objName == "StatusBarButtonQSNAP":

            settingsQSnapAction = QAction(QIcon(themeDir + "qsnapsettings.png"), s, menu)
            settingsQSnapAction.triggered.connect(self.settingsQSnap)
            menu.addAction(settingsQSnapAction)

        elif objName == "StatusBarButtonQTRACK":

            settingsQTrackAction = QAction(QIcon(themeDir + "qtracksettings.png"), s, menu)
            settingsQTrackAction.triggered.connect(self.settingsQTrack)
            menu.addAction(settingsQTrackAction)

        elif objName == "StatusBarButtonLWT":

            gview = self.mainWin.activeView()
            if gview:

                enableRealAction = QAction(QIcon(themeDir + "realrender.png"), "&RealRender On", menu)
                enableRealAction.setEnabled(not gview.isRealEnabled())
                enableRealAction.triggered.connect(self.enableReal)
                menu.addAction(enableRealAction)

                disableRealAction = QAction(QIcon(themeDir + "realrender.png"), "&RealRender Off", menu)
                disableRealAction.setEnabled(gview.isRealEnabled())
                disableRealAction.triggered.connect(self.disableReal)
                menu.addAction(disableRealAction)

            settingsLwtAction = QAction(QIcon(themeDir + "lineweightsettings" + ".png"), s, menu)
            settingsLwtAction.triggered.connect(self.settingsLwt)
            menu.addAction(settingsLwtAction)

        menu.exec_(event.globalPos())
        QApplication.restoreOverrideCursor()
        self.statusbar.clearMessage()

    def settingsSnap(self):
        self.mainWin.settingsDialog("Snap")

    def settingsGrid(self):
        self.mainWin.settingsDialog("Grid/Ruler")

    def settingsRuler(self):
        self.mainWin.settingsDialog("Grid/Ruler")

    def settingsOrtho(self):
        self.mainWin.settingsDialog("Ortho/Polar")

    def settingsPolar(self):
        self.mainWin.settingsDialog("Ortho/Polar")

    def settingsQSnap(self):
        self.mainWin.settingsDialog("QuickSnap")

    def settingsQTrack(self):
        self.mainWin.settingsDialog("QuickTrack")

    def settingsLwt(self):
        self.mainWin.settingsDialog("LineWeight")

    def toggleSnap(self, on):
        """
        TOWRITE

        :param `on`: TOWRITE
        :type `on`: bool
        """
        qDebug("StatusBarButton toggleSnap()")
        gview = self.mainWin.activeView()
        if gview:
            gview.toggleSnap(on)

    def toggleGrid(self, on):
        """
        TOWRITE

        :param `on`: TOWRITE
        :type `on`: bool
        """
        qDebug("StatusBarButton toggleGrid()")
        gview = self.mainWin.activeView()
        if gview:
            gview.toggleGrid(on)

    def toggleRuler(self, on):
        """
        TOWRITE

        :param `on`: TOWRITE
        :type `on`: bool
        """
        qDebug("StatusBarButton toggleRuler()")
        gview = self.mainWin.activeView()
        if gview:
            gview.toggleRuler(on)

    def toggleOrtho(self, on):
        """
        TOWRITE

        :param `on`: TOWRITE
        :type `on`: bool
        """
        qDebug("StatusBarButton toggleOrtho()")
        gview = self.mainWin.activeView()
        if gview:
            gview.toggleOrtho(on)

    def togglePolar(self, on):
        """
        TOWRITE

        :param `on`: TOWRITE
        :type `on`: bool
        """
        qDebug("StatusBarButton togglePolar()")
        gview = self.mainWin.activeView()
        if gview:
            gview.togglePolar(on)

    def toggleQSnap(self, on):
        """
        TOWRITE

        :param `on`: TOWRITE
        :type `on`: bool
        """
        qDebug("StatusBarButton toggleQSnap()")
        gview = self.mainWin.activeView()
        if gview:
            gview.toggleQSnap(on)

    def toggleQTrack(self, on):
        """
        TOWRITE

        :param `on`: TOWRITE
        :type `on`: bool
        """
        qDebug("StatusBarButton toggleQTrack()")
        gview = mainWin.activeView()
        if gview:
            gview.toggleQTrack(on)

    def toggleLwt(self, on):
        """
        TOWRITE

        :param `on`: TOWRITE
        :type `on`: bool
        """
        qDebug("StatusBarButton toggleLwt()")
        gview = self.mainWin.activeView()
        if gview:
            gview.toggleLwt(on)

    def enableLwt(self):
        qDebug("StatusBarButton enableLwt()")
        gview = self.mainWin.activeView()
        if gview:
            if not gview.isLwtEnabled():
                gview.toggleLwt(True)

    def disableLwt(self):
        qDebug("StatusBarButton disableLwt()")
        gview = self.mainWin.activeView()
        if gview:
            if gview.isLwtEnabled():
                gview.toggleLwt(False)

    def enableReal(self):
        qDebug("StatusBarButton enableReal()")
        gview = self.mainWin.activeView()
        if gview:
            gview.toggleReal(True)

    def disableReal(self):
        qDebug("StatusBarButton disableReal()")
        gview = self.mainWin.activeView()
        if gview:
            gview.toggleReal(False)


# kate: bom off; indent-mode cstyle; indent-width 4; replace-trailing-space-save on;
