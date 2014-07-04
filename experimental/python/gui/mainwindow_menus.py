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
#--PySide/PyQt Imports.
try:
    from PySide.QtCore import qDebug, SIGNAL, SLOT
    from PySide.QtGui import QIcon
except ImportError:
    raise
#    from PyQt4.QtCore import qDebug, SIGNAL, SLOT
#    from PyQt4.QtGui import QIcon


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
    qDebug("MainWindow createFileMenu()")
    self.menuBar().addMenu(self.fileMenu)
    self.fileMenu.addAction(actionHash.value(ACTION_new))
    self.fileMenu.addSeparator()
    self.fileMenu.addAction(actionHash.value(ACTION_open))

    self.fileMenu.addMenu(self.recentMenu)
    self.connect(self.recentMenu, SIGNAL("aboutToShow()"), self, SLOT("recentMenuAboutToShow()"))
    # Do not allow the Recent Menu to be torn off. It's a pain in the ass to maintain.
    self.recentMenu.setTearOffEnabled(False)

    self.fileMenu.addSeparator()
    self.fileMenu.addAction(actionHash.value(ACTION_save))
    self.fileMenu.addAction(actionHash.value(ACTION_saveas))
    self.fileMenu.addSeparator()
    self.fileMenu.addAction(actionHash.value(ACTION_print))
    self.fileMenu.addSeparator()
    self.fileMenu.addAction(actionHash.value(ACTION_windowclose))
    self.fileMenu.addSeparator()
    self.fileMenu.addAction(actionHash.value(ACTION_designdetails))
    self.fileMenu.addSeparator()

    self.fileMenu.addAction(actionHash.value(ACTION_exit))
    self.fileMenu.setTearOffEnabled(False)

def createEditMenu(self):  # void MainWindow::createEditMenu()
    """
    TOWRITE
    """
    qDebug("MainWindow createEditMenu()")
    self.menuBar().addMenu(self.editMenu)
    self.editMenu.addAction(actionHash.value(ACTION_undo))
    self.editMenu.addAction(actionHash.value(ACTION_redo))
    self.editMenu.addSeparator()
    self.editMenu.addAction(actionHash.value(ACTION_cut))
    self.editMenu.addAction(actionHash.value(ACTION_copy))
    self.editMenu.addAction(actionHash.value(ACTION_paste))
    self.editMenu.addSeparator()
    self.editMenu.setTearOffEnabled(True)

def createViewMenu(self):  # void MainWindow::createViewMenu()
    """
    TOWRITE
    """
    qDebug("MainWindow createViewMenu()")

    icontheme = self.getSettingsGeneralIconTheme()  # QString

    self.menuBar().addMenu(self.viewMenu)
    self.viewMenu.addSeparator()
    self.viewMenu.addMenu(self.zoomMenu)
    self.zoomMenu.setIcon(QIcon("icons/" + icontheme + "/zoom" + ".png"))
    self.zoomMenu.addAction(actionHash.value(ACTION_zoomrealtime))
    self.zoomMenu.addAction(actionHash.value(ACTION_zoomprevious))
    self.zoomMenu.addSeparator()
    self.zoomMenu.addAction(actionHash.value(ACTION_zoomwindow))
    self.zoomMenu.addAction(actionHash.value(ACTION_zoomdynamic))
    self.zoomMenu.addAction(actionHash.value(ACTION_zoomscale))
    self.zoomMenu.addAction(actionHash.value(ACTION_zoomcenter))
    self.zoomMenu.addSeparator()
    self.zoomMenu.addAction(actionHash.value(ACTION_zoomin))
    self.zoomMenu.addAction(actionHash.value(ACTION_zoomout))
    self.zoomMenu.addSeparator()
    self.zoomMenu.addAction(actionHash.value(ACTION_zoomselected))
    self.zoomMenu.addAction(actionHash.value(ACTION_zoomall))
    self.zoomMenu.addAction(actionHash.value(ACTION_zoomextents))
    self.viewMenu.addMenu(panMenu)
    self.panMenu.setIcon(QIcon("icons/" + icontheme + "/pan" + ".png"))
    self.panMenu.addAction(actionHash.value(ACTION_panrealtime))
    self.panMenu.addAction(actionHash.value(ACTION_panpoint))
    self.panMenu.addSeparator()
    self.panMenu.addAction(actionHash.value(ACTION_panleft))
    self.panMenu.addAction(actionHash.value(ACTION_panright))
    self.panMenu.addAction(actionHash.value(ACTION_panup))
    self.panMenu.addAction(actionHash.value(ACTION_pandown))
    self.viewMenu.addSeparator()
    self.viewMenu.addAction(actionHash.value(ACTION_day))
    self.viewMenu.addAction(actionHash.value(ACTION_night))
    self.viewMenu.addSeparator()

    self.viewMenu.setTearOffEnabled(True)
    self.zoomMenu.setTearOffEnabled(True)
    self.panMenu.setTearOffEnabled(True)

def createSettingsMenu(self):  # void MainWindow::createSettingsMenu()
    """
    TOWRITE
    """
    qDebug("MainWindow createSettingsMenu()")
    self.menuBar().addMenu(self.settingsMenu)
    self.settingsMenu.addAction(actionHash.value(ACTION_settingsdialog))
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
    qDebug("MainWindow createHelpMenu()")
    self.menuBar().addMenu(self.helpMenu)
    self.helpMenu.addAction(actionHash.value(ACTION_help))
    self.helpMenu.addSeparator()
    self.helpMenu.addAction(actionHash.value(ACTION_changelog))
    self.helpMenu.addSeparator()
    self.helpMenu.addAction(actionHash.value(ACTION_tipoftheday))
    self.helpMenu.addSeparator()
    self.helpMenu.addAction(actionHash.value(ACTION_about))
    self.helpMenu.addSeparator()
    self.helpMenu.addAction(actionHash.value(ACTION_whatsthis))
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
