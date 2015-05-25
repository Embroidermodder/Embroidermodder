#!/usr/bin/env python
# -*- coding: utf-8 -*-

"""
====================================
|module_summary| statusbar_button.py
====================================

TOWRITE


Classes summary:
================

============================ ============================
:class:`~StatusBarButton`    A `QToolButton`_ GUI element to be injected into a `QStatusBar`_.
============================ ============================

---------------------------------------------------------
"""

#-Imports.---------------------------------------------------------------------
#--Python Imports.
import os

#--PySide/PyQt Imports.
if PYSIDE:
    from PySide.QtCore import qDebug, Qt, Slot
    from PySide.QtGui import QApplication, QAction, QIcon, QMenu, QToolButton
elif PYQT4:
    import sip
    sip.setapi('QString', 2)
    sip.setapi('QVariant', 2)
    from PyQt4.QtCore import qDebug, Qt
    from PyQt4.QtCore import pyqtSlot as Slot
    from PyQt4.QtGui import QApplication, QAction, QIcon, QMenu, QToolButton


class StatusBarButton(QToolButton):
    """
    Subclass of `QToolButton`_

    A `QToolButton`_ GUI element to be injected into a `QStatusBar`_.

    .. sphinx_generate_methods_summary::
       StatusBarButton
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
        :param `parent`: Pointer to a parent widget instance.
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
        themeDir = self.mainWin.gIconDir + os.sep + self.mainWin.getSettingsGeneralIconTheme() + os.sep
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

    # Slots ------------------------------------------------------------------

    @Slot()
    def settingsSnap(self):
        """
        Open the `Settings_Dialog` on the "Snap" notebook tab page.
        """
        self.mainWin.settingsDialog("Snap")

    @Slot()
    def settingsGrid(self):
        """
        Open the `Settings_Dialog` on the "Grid/Ruler" notebook tab page.
        """
        self.mainWin.settingsDialog("Grid/Ruler")

    @Slot()
    def settingsRuler(self):
        """
        Open the `Settings_Dialog` on the "Grid/Ruler" notebook tab page.
        """
        self.mainWin.settingsDialog("Grid/Ruler")

    @Slot()
    def settingsOrtho(self):
        """
        Open the `Settings_Dialog` on the "Ortho/Polar" notebook tab page.
        """
        self.mainWin.settingsDialog("Ortho/Polar")

    @Slot()
    def settingsPolar(self):
        """
        Open the `Settings_Dialog` on the "Ortho/Polar" notebook tab page.
        """
        self.mainWin.settingsDialog("Ortho/Polar")

    @Slot()
    def settingsQSnap(self):
        """
        Open the `Settings_Dialog` on the "QuickSnap" notebook tab page.
        """
        self.mainWin.settingsDialog("QuickSnap")

    @Slot()
    def settingsQTrack(self):
        """
        Open the `Settings_Dialog` on the "QuickTrack" notebook tab page.
        """
        self.mainWin.settingsDialog("QuickTrack")

    @Slot()
    def settingsLwt(self):
        """
        Open the `Settings_Dialog` on the "LineWeight" notebook tab page.
        """
        self.mainWin.settingsDialog("LineWeight")

    @Slot(bool)
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

    @Slot(bool)
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

    @Slot(bool)
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

    @Slot(bool)
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

    @Slot(bool)
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

    @Slot(bool)
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

    @Slot(bool)
    def toggleQTrack(self, on):
        """
        TOWRITE

        :param `on`: TOWRITE
        :type `on`: bool
        """
        qDebug("StatusBarButton toggleQTrack()")
        gview = self.mainWin.activeView()
        if gview:
            gview.toggleQTrack(on)

    @Slot(bool)
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

    @Slot()
    def enableLwt(self):
        """"""
        qDebug("StatusBarButton enableLwt()")
        gview = self.mainWin.activeView()
        if gview:
            if not gview.isLwtEnabled():
                gview.toggleLwt(True)

    @Slot()
    def disableLwt(self):
        """"""
        qDebug("StatusBarButton disableLwt()")
        gview = self.mainWin.activeView()
        if gview:
            if gview.isLwtEnabled():
                gview.toggleLwt(False)

    @Slot()
    def enableReal(self):
        """"""
        qDebug("StatusBarButton enableReal()")
        gview = self.mainWin.activeView()
        if gview:
            gview.toggleReal(True)

    @Slot()
    def disableReal(self):
        """"""
        qDebug("StatusBarButton disableReal()")
        gview = self.mainWin.activeView()
        if gview:
            gview.toggleReal(False)


# kate: bom off; indent-mode python; indent-width 4; replace-trailing-space-save on;
