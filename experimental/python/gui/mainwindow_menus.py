#!/usr/bin/env python
# -*- coding: utf-8 -*-

"""
====================================
|module_summary| mainwindow_menus.py
====================================

TOWRITE

These methods are to be imported into the MainWindow class directly.

"""

#-Imports.----------------------------------------------------------------------
#--Python Imports.
import os

#--PySide/PyQt Imports.
if PYSIDE:
    from PySide.QtCore import qDebug, SIGNAL, SLOT
    from PySide.QtGui import QIcon
elif PYQT4:
    import sip
    sip.setapi('QString', 2)
    sip.setapi('QVariant', 2)
    from PyQt4.QtCore import qDebug, SIGNAL, SLOT
    from PyQt4.QtGui import QIcon


# C++C++C++C++C++C++C++C++C++C++C++C++C++C++C++C++C++C++C++C++C++C++C++C++C++
#include "mainwindow.h"
#include <QDebug>
#include <QMenu>
#include <QMenuBar>
#include <QAction>
# C++C++C++C++C++C++C++C++C++C++C++C++C++C++C++C++C++C++C++C++C++C++C++C++C++

# meth MainWindow::
# These methods are to be imported into the MainWindow class.
# Just imagine the methods are right inside it.


def createFileMenu(self):  # void MainWindow::createFileMenu()
    """
    TOWRITE
    """
    actionHash = self.actionHash
    qDebug("MainWindow createFileMenu()")
    self.menuBar().addMenu(self.fileMenu)
    self.fileMenu.addAction(actionHash["ACTION_new"])
    self.fileMenu.addSeparator()
    self.fileMenu.addAction(actionHash["ACTION_open"])

    self.fileMenu.addMenu(self.recentMenu)
    self.connect(self.recentMenu, SIGNAL("aboutToShow()"), self, SLOT("recentMenuAboutToShow()"))
    # Do not allow the Recent Menu to be torn off. It's a pain in the ass to maintain.
    self.recentMenu.setTearOffEnabled(False)

    self.fileMenu.addSeparator()
    self.fileMenu.addAction(actionHash["ACTION_save"])
    self.fileMenu.addAction(actionHash["ACTION_saveas"])
    self.fileMenu.addSeparator()
    self.fileMenu.addAction(actionHash["ACTION_print"])
    self.fileMenu.addSeparator()
    self.fileMenu.addAction(actionHash["ACTION_windowclose"])
    self.fileMenu.addSeparator()
    self.fileMenu.addAction(actionHash["ACTION_designdetails"])
    self.fileMenu.addSeparator()

    self.fileMenu.addAction(actionHash["ACTION_exit"])
    self.fileMenu.setTearOffEnabled(False)

def createEditMenu(self):  # void MainWindow::createEditMenu()
    """
    TOWRITE
    """
    actionHash = self.actionHash
    qDebug("MainWindow createEditMenu()")
    self.menuBar().addMenu(self.editMenu)
    self.editMenu.addAction(actionHash["ACTION_undo"])
    self.editMenu.addAction(actionHash["ACTION_redo"])
    self.editMenu.addSeparator()
    self.editMenu.addAction(actionHash["ACTION_cut"])
    self.editMenu.addAction(actionHash["ACTION_copy"])
    self.editMenu.addAction(actionHash["ACTION_paste"])
    self.editMenu.addSeparator()
    self.editMenu.setTearOffEnabled(True)

def createViewMenu(self):  # void MainWindow::createViewMenu()
    """
    TOWRITE
    """
    actionHash = self.actionHash
    qDebug("MainWindow createViewMenu()")

    icontheme = self.getSettingsGeneralIconTheme()  # QString

    self.menuBar().addMenu(self.viewMenu)
    self.viewMenu.addSeparator()
    self.viewMenu.addMenu(self.zoomMenu)
    self.zoomMenu.setIcon(QIcon(self.gIconDir + os.sep + icontheme + "/zoom" + ".png"))
    self.zoomMenu.addAction(actionHash["ACTION_zoomrealtime"])
    self.zoomMenu.addAction(actionHash["ACTION_zoomprevious"])
    self.zoomMenu.addSeparator()
    self.zoomMenu.addAction(actionHash["ACTION_zoomwindow"])
    self.zoomMenu.addAction(actionHash["ACTION_zoomdynamic"])
    self.zoomMenu.addAction(actionHash["ACTION_zoomscale"])
    self.zoomMenu.addAction(actionHash["ACTION_zoomcenter"])
    self.zoomMenu.addSeparator()
    self.zoomMenu.addAction(actionHash["ACTION_zoomin"])
    self.zoomMenu.addAction(actionHash["ACTION_zoomout"])
    self.zoomMenu.addSeparator()
    self.zoomMenu.addAction(actionHash["ACTION_zoomselected"])
    self.zoomMenu.addAction(actionHash["ACTION_zoomall"])
    self.zoomMenu.addAction(actionHash["ACTION_zoomextents"])
    self.viewMenu.addMenu(self.panMenu)
    self.panMenu.setIcon(QIcon(self.gIconDir + os.sep + icontheme + "/pan" + ".png"))
    self.panMenu.addAction(actionHash["ACTION_panrealtime"])
    self.panMenu.addAction(actionHash["ACTION_panpoint"])
    self.panMenu.addSeparator()
    self.panMenu.addAction(actionHash["ACTION_panleft"])
    self.panMenu.addAction(actionHash["ACTION_panright"])
    self.panMenu.addAction(actionHash["ACTION_panup"])
    self.panMenu.addAction(actionHash["ACTION_pandown"])
    self.viewMenu.addSeparator()
    self.viewMenu.addAction(actionHash["ACTION_day"])
    self.viewMenu.addAction(actionHash["ACTION_night"])
    self.viewMenu.addSeparator()

    self.viewMenu.setTearOffEnabled(True)
    self.zoomMenu.setTearOffEnabled(True)
    self.panMenu.setTearOffEnabled(True)

def createSettingsMenu(self):  # void MainWindow::createSettingsMenu()
    """
    TOWRITE
    """
    actionHash = self.actionHash
    qDebug("MainWindow createSettingsMenu()")
    self.menuBar().addMenu(self.settingsMenu)
    self.settingsMenu.addAction(actionHash["ACTION_settingsdialog"])
    self.settingsMenu.addSeparator()
    self.settingsMenu.setTearOffEnabled(True)

def createWindowMenu(self):  #void MainWindow::createWindowMenu()
    """
    TOWRITE
    """
    qDebug("MainWindow createWindowMenu()")
    self.menuBar().addMenu(self.windowMenu)
    self.connect(self.windowMenu, SIGNAL("aboutToShow()"), self, SLOT("windowMenuAboutToShow()"))
    # Do not allow the Window Menu to be torn off. It's a pain in the ass to maintain.
    self.windowMenu.setTearOffEnabled(False)

def createHelpMenu(self):  # void MainWindow::createHelpMenu()
    """
    TOWRITE
    """
    actionHash = self.actionHash
    qDebug("MainWindow createHelpMenu()")
    self.menuBar().addMenu(self.helpMenu)
    self.helpMenu.addAction(actionHash["ACTION_help"])
    self.helpMenu.addSeparator()
    self.helpMenu.addAction(actionHash["ACTION_changelog"])
    self.helpMenu.addSeparator()
    self.helpMenu.addAction(actionHash["ACTION_tipoftheday"])
    self.helpMenu.addSeparator()
    self.helpMenu.addAction(actionHash["ACTION_about"])
    self.helpMenu.addSeparator()
    self.helpMenu.addAction(actionHash["ACTION_whatsthis"])
    self.helpMenu.setTearOffEnabled(True)

def createAllMenus(self):  # void MainWindow::createAllMenus()
    """
    TOWRITE
    """
    qDebug("MainWindow createAllMenus()")
    self.createFileMenu()
    self.createEditMenu()
    self.createViewMenu()
    self.createSettingsMenu()
    self.createWindowMenu()
    self.createHelpMenu()


# kate: bom off; indent-mode python; indent-width 4; replace-trailing-space-save on;
