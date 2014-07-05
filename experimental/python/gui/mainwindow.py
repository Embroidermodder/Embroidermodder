#!/usr/bin/env python
# -*- coding: utf-8 -*-

"""
==============================
|module_summary| mainwindow.py
==============================

TOWRITE

Classes summary:
================

============================ ============================
:class:`~MainWindow`         TOWRITE
============================ ============================

---------------------------------------------------------

|

"""

#-Imports.----------------------------------------------------------------------
#--PySide/PyQt Imports.
try:
    from PySide.QtCore import (qDebug, QDate, QDir, QFile, QFileInfo, QLocale,
        QMetaObject, QLibraryInfo, QTextStream, QTranslator, SIGNAL, SLOT)
    from PySide.QtGui import (QIcon, QFrame, QVBoxLayout, QMenu, QMenuBar,
        QStatusBar, QMdiArea, QWidget, QMdiSubWindow, QMessageBox, QToolBar,
        QFileDialog, QApplication, QLabel, QComboBox, QCloseEvent, QMainWindow,
        QColor, QFontComboBox, QStyle)
    from PySide.QtScript import QScriptEngine
    from PySide.QtScriptTools import QScriptEngineDebugger
except ImportError:
    raise
#    from PyQt4.QtCore import (qDebug, QDate, QDir, QFile, QFileInfo, QLocale,
#        QMetaObject, QLibraryInfo, QTextStream, QTranslator, SIGNAL, SLOT)
#    from PyQt4.QtGui import (QIcon, QFrame, QVBoxLayout, QMenu, QMenuBar,
#        QStatusBar, QMdiArea, QWidget, QMdiSubWindow, QMessageBox, QToolBar,
#        QFileDialog, QApplication, QLabel, QComboBox, QCloseEvent, QMainWindow,
#        QColor, QFontComboBox, QStyle)
#    from PyQt4.QtScript import QScriptEngine
#    from PyQt4.QtScriptTools import QScriptEngineDebugger

# C++C++C++C++C++C++C++C++C++C++C++C++C++C++C++C++C++C++C++C++C++C++C++C++C++
#include "mainwindow.h"
#include "mainwindow-actions.h"

#include "statusbar.h"
#include "statusbar-button.h"

#include "view.h"
#include "cmdprompt.h"

#include "property-editor.h"
#include "undo-editor.h"

#include "native-scripting.h"
#include "native-javascript.h"

#include "preview-dialog.h"

#include "emb-format.h"

#include <stdlib.h>

#include <QDebug>
#include <QFrame>
#include <QVBoxLayout>
#include <QMenu>
#include <QMenuBar>
#include <QStatusBar>
#include <QMdiArea>
#include <QWidget>
#include <QMdiSubWindow>
#include <QMessageBox>
#include <QToolBar>
#include <QFileDialog>
#include <QApplication>
#include <QDate>
#include <QFileInfo>
#include <QLabel>
#include <QComboBox>
#include <QCloseEvent>
#include <QMetaObject>
#include <QLocale>
# C++C++C++C++C++C++C++C++C++C++C++C++C++C++C++C++C++C++C++C++C++C++C++C++C++


class MainWindow(QMainWindow):  # MainWindow::MainWindow() : QMainWindow(0)
    """
    Subclass of `QMainWindow`_

    Main Frame Window for Embroidermodder application.

    """
    def __init__(self):
        """Default class constructor."""
        super(EmbroidermodderMainWindow, self).__init__()

        # local optimization
        tr = self.tr
        connect = self.connect

        self.readSettings()

        lang = self.getSettingsGeneralLanguage()  # QString
        qDebug("language: %s", qPrintable(lang))
        if lang == "system":
            lang = QLocale.system().languageToString(QLocale.system().language()).toLower()

        # Load translations for the Embroidermodder 2 GUI.
        translatorEmb = QTranslator()
        translatorEmb.load("translations/" + lang + "/embroidermodder2_" + lang)
        qApp.installTranslator(translatorEmb)

        # Load translations for the commands.
        translatorCmd = QTranslator()
        translatorCmd.load("translations/" + lang + "/commands_" + lang)
        qApp.installTranslator(translatorCmd)

        # Load translations provided by Qt - this covers dialog buttons and other common things.
        translatorQt = QTranslator
        translatorQt.load("qt_" + QLocale.system().name(), QLibraryInfo.location(QLibraryInfo.TranslationsPath))
        qApp.installTranslator(translatorQt)

        # Init
        self.mainWin = mainWin = self
        # Menus
        self.fileMenu     = QMenu(tr("&File"),     self)
        self.editMenu     = QMenu(tr("&Edit"),     self)
        self.viewMenu     = QMenu(tr("&View"),     self)
        self.settingsMenu = QMenu(tr("&Settings"), self)
        self.windowMenu   = QMenu(tr("&Window"),   self)
        self.helpMenu     = QMenu(tr("&Help"),     self)
        # SubMenus
        self.recentMenu   = QMenu(tr("Open &Recent"), self)  #: `QMenu`_ for holding recently opened files paths.
        self.zoomMenu     = QMenu(tr("&Zoom"),        self)
        self.panMenu      = QMenu(tr("&Pan"),         self)
        # Toolbars
        self.toolbarFile       = self.addToolBar(tr("File"))
        self.toolbarEdit       = self.addToolBar(tr("Edit"))
        self.toolbarView       = self.addToolBar(tr("View"))
        self.toolbarZoom       = self.addToolBar(tr("Zoom"))
        self.toolbarPan        = self.addToolBar(tr("Pan"))
        self.toolbarIcon       = self.addToolBar(tr("Icon"))
        self.toolbarHelp       = self.addToolBar(tr("Help"))
        self.toolbarLayer      = self.addToolBar(tr("Layer"))
        self.toolbarProperties = self.addToolBar(tr("Properties"))
        self.toolbarText       = self.addToolBar(tr("Text"))
        self.toolbarPrompt     = self.addToolBar(tr("Command Prompt"))
        # Selectors
        self.layerSelector      = QComboBox(self)
        self.colorSelector      = QComboBox(self)
        self.linetypeSelector   = QComboBox(self)
        self.lineweightSelector = QComboBox(self)
        self.textFontSelector   = QFontComboBox(self)
        self.textSizeSelector   = QComboBox(self)

        self.numOfDocs = 0
        self.docIndex = 0

        self.shiftKeyPressedState = False

        self.setWindowIcon(QIcon("icons/" + self.getSettingsGeneralIconTheme() + "/" + "app" + ".png"))
        self.setMinimumSize(800, 480)  # Require Minimum WVGA

        #TODO/PORT# self.loadFormats()

        # create the mdiArea
        vbox = QFrame(self)  # QFrame*
        layout = QVBoxLayout(vbox)  # QVBoxLayout*
        layout.setMargin(0)
        vbox.setFrameStyle(QFrame.StyledPanel | QFrame.Sunken)
        self.mdiArea = mdiArea = MdiArea(self, vbox)
        mdiArea.useBackgroundLogo(self.getSettingsGeneralMdiBGUseLogo())
        mdiArea.useBackgroundTexture(self.getSettingsGeneralMdiBGUseTexture())
        mdiArea.useBackgroundColor(self.getSettingsGeneralMdiBGUseColor())
        mdiArea.setBackgroundLogo(self.getSettingsGeneralMdiBGLogo())
        mdiArea.setBackgroundTexture(self.getSettingsGeneralMdiBGTexture())
        mdiArea.setBackgroundColor(QColor(self.getSettingsGeneralMdiBGColor()))
        mdiArea.setViewMode(QMdiArea.TabbedView)
        mdiArea.setHorizontalScrollBarPolicy(Qt.ScrollBarAsNeeded)
        mdiArea.setVerticalScrollBarPolicy(Qt.ScrollBarAsNeeded)
        mdiArea.setActivationOrder(QMdiArea.ActivationHistoryOrder)
        layout.addWidget(mdiArea)
        self.setCentralWidget(vbox)

        # create the Command Prompt.
        self.prompt = prompt = CmdPrompt(self)
        prompt.setFocus(Qt.OtherFocusReason)
        self.setFocusProxy(prompt)
        mdiArea.setFocusProxy(prompt)

        prompt.setPromptTextColor(QColor(self.getSettingsPromptTextColor()))
        prompt.setPromptBackgroundColor(QColor(self.getSettingsPromptBGColor()))

        connect(prompt, SIGNAL("startCommand(QString)"), self, SLOT("logPromptInput(QString)"))

        connect(prompt, SIGNAL("startCommand(QString)"), self, SLOT("runCommandMain(QString)"))
        connect(prompt, SIGNAL("runCommand(QString, QString)"), self, SLOT("runCommandPrompt(QString, QString)"))

        connect(prompt, SIGNAL("deletePressed()"),    self, SLOT("deletePressed()"))
        # TODO: connect(prompt, SIGNAL("tabPressed()"),       self, SLOT("someUnknownSlot()"))
        connect(prompt, SIGNAL("escapePressed()"),    self, SLOT("escapePressed()"))
        connect(prompt, SIGNAL("upPressed()"),        self, SLOT("promptInputPrevious()"))
        connect(prompt, SIGNAL("downPressed()"),      self, SLOT("promptInputNext()"))
        connect(prompt, SIGNAL("F1Pressed()"),        self, SLOT("help()"))
        # TODO: connect(prompt, SIGNAL("F2Pressed()"),        self, SLOT("floatHistory()"))
        # TODO: connect(prompt, SIGNAL("F3Pressed()"),        self, SLOT("toggleQSNAP()"))
        connect(prompt, SIGNAL("F4Pressed()"),        self, SLOT("toggleLwt()"))  # TODO: typically self is toggleTablet(), make F-Keys customizable thru settings
        # TODO: connect(prompt, SIGNAL("F5Pressed()"),        self, SLOT("toggleISO()"))
        # TODO: connect(prompt, SIGNAL("F6Pressed()"),        self, SLOT("toggleCoords()"))
        connect(prompt, SIGNAL("F7Pressed()"),        self, SLOT("toggleGrid()"))
        # TODO: connect(prompt, SIGNAL("F8Pressed()"),        self, SLOT("toggleORTHO()"))
        # TODO: connect(prompt, SIGNAL("F9Pressed()"),        self, SLOT("toggleSNAP()"))
        # TODO: connect(prompt, SIGNAL("F10Pressed()"),       self, SLOT("togglePOLAR()"))
        # TODO: connect(prompt, SIGNAL("F11Pressed()"),       self, SLOT("toggleQTRACK()"))
        connect(prompt, SIGNAL("F12Pressed()"),       self, SLOT("toggleRuler()"))
        connect(prompt, SIGNAL("cutPressed()"),       self, SLOT("cut()"))
        connect(prompt, SIGNAL("copyPressed()"),      self, SLOT("copy()"))
        connect(prompt, SIGNAL("pastePressed()"),     self, SLOT("paste()"))
        connect(prompt, SIGNAL("selectAllPressed()"), self, SLOT("selectAll()"))
        connect(prompt, SIGNAL("undoPressed()"),      self, SLOT("undo()"))
        connect(prompt, SIGNAL("redoPressed()"),      self, SLOT("redo()"))

        connect(prompt, SIGNAL("shiftPressed()"),     self, SLOT("setShiftPressed()"))
        connect(prompt, SIGNAL("shiftReleased()"),    self, SLOT("setShiftReleased()"))

        connect(prompt, SIGNAL("showSettings()"),     self, SLOT("settingsPrompt()"))

        connect(prompt, SIGNAL("historyAppended(QString)"), self, SLOT("promptHistoryAppended(QString)"))

        # create the Object Property Editor.
        self.dockPropEdit = dockPropEdit = PropertyEditor("icons/" + self.getSettingsGeneralIconTheme(), self.getSettingsSelectionModePickAdd(), prompt, self)
        self.addDockWidget(Qt.LeftDockWidgetArea, dockPropEdit)
        connect(dockPropEdit, SIGNAL("pickAddModeToggled()"), self, SLOT("pickAddModeToggled()"))

        # create the Command History Undo Editor.
        self.dockUndoEdit = dockUndoEdit = UndoEditor("icons/" + getSettingsGeneralIconTheme(), prompt, self)
        self.addDockWidget(Qt.LeftDockWidgetArea, dockUndoEdit);

        ##setDockOptions(QMainWindow.AnimatedDocks | QMainWindow.AllowTabbedDocks | QMainWindow.VerticalTabs)  # TODO: Load these from settings
        ##tabifyDockWidget(dockPropEdit, dockUndoEdit)  # TODO: load this from settings

        # Javascript
        self.initMainWinPointer(self)

        self.engine = engine = QScriptEngine(self)
        engine.installTranslatorFunctions()
        self.debugger = QScriptEngineDebugger(self)
        self.debugger.attachTo(engine)
        self.javaInitNatives(engine)

        # Load all commands in a loop
        commandDir = QDir("commands")
        cmdList = commandDir.entryList(QDir.Dirs | QDir.NoDotAndDotDot)  # QStringList
        for cmdName in cmdList:  # foreach(QString cmdName, cmdList)
            self.javaLoadCommand(cmdName)

        self.statusbar = statusbar = StatusBar(self, self)
        self.setStatusBar(statusbar)

        self.createAllActions()
        self.createAllMenus()
        self.createAllToolbars()

        self.iconResize(self.getSettingsGeneralIconSize())
        self.updateMenuToolbarStatusbar()

        # Show date in statusbar after it has been updated.
        date = QDate.currentDate()  # QDate
        datestr = date.toString("MMMM d, yyyy")  # QString
        statusbar.showMessage(datestr)

        self.showNormal()

        # Load tips from external file.
        tipFile = QFile("tips.txt")
        #TODO/PORT# if tipFile.open(QFile.ReadOnly):
        #TODO/PORT#
        #TODO/PORT#     stream = QTextStream(tipFile)
        #TODO/PORT#     QString tipLine
        #TODO/PORT#     do
        #TODO/PORT#     {
        #TODO/PORT#         tipLine = stream.readLine();
        #TODO/PORT#         if(!tipLine.isEmpty())
        #TODO/PORT#             listTipOfTheDay << tipLine;
        #TODO/PORT#     }
        #TODO/PORT#     while(!tipLine.isNull());
        #TODO/PORT# }
        #TODO/PORT# if self.getSettingsGeneralTipOfTheDay():
        #TODO/PORT#     self.tipOfTheDay()


    def __del__(self):
        """Class destructor."""
        qDebug("MainWindow::Destructor()")

        # Prevent memory leaks by deleting any unpasted objects
        qDeleteAll(cutCopyObjectList.begin(), cutCopyObjectList.end())
        cutCopyObjectList.clear()

    def getAction(self, actionEnum):  #TODO/PORT# int?# QAction* MainWindow::getAction(int actionEnum)
        """
        Retrieve an action from the actionHash.

        :param `actionEnum`: TOWRITE
        :type `actionEnum`: str
        :rtype: `QAction`_
        """
        return actionHash["actionEnum"]

    def recentMenuAboutToShow(self):
        """
        Called when the ``aboutToShow()`` SIGNAL for :py:attr:`MainWindow.recentMenu` is emitted.
        """
        qDebug("MainWindow::recentMenuAboutToShow()")
        self.recentMenu.clear()

        recentFileInfo = QFileInfo()
        recentValue = ""  # QString
        #TODO/PORT# for i in range(0, len(self.settings_opensave_recent_list_of_files)):  # for(int i = 0; i < settings_opensave_recent_list_of_files.size(); ++i)
        #TODO/PORT#
        #TODO/PORT#     # If less than the max amount of entries add to menu.
        #TODO/PORT#     if i < self.settings_opensave_recent_max_files:
        #TODO/PORT#
        #TODO/PORT#         recentFileInfo = QFileInfo(self.settings_opensave_recent_list_of_files[i])
        #TODO/PORT#         if recentFileInfo.exists() and self.validFileFormat(recentFileInfo.fileName()):
        #TODO/PORT#
        #TODO/PORT#             recentValue.setNum(i+1)
        #TODO/PORT#             QAction* rAction;
        #TODO/PORT#             if     (recentValue.toInt() >= 1 && recentValue.toInt() <= 9) rAction = new QAction("&" + recentValue + " " + recentFileInfo.fileName(), this);
        #TODO/PORT#             else if(recentValue.toInt() == 10)                            rAction = new QAction("1&0 "                  + recentFileInfo.fileName(), this);
        #TODO/PORT#             else                                                          rAction = new QAction(      recentValue + " " + recentFileInfo.fileName(), this);
        #TODO/PORT#             rAction->setCheckable(false);
        #TODO/PORT#             rAction->setData(settings_opensave_recent_list_of_files.at(i));
        #TODO/PORT#             recentMenu->addAction(rAction);
        #TODO/PORT#             connect(rAction, SIGNAL(triggered()), this, SLOT(openrecentfile()));
        #TODO/PORT#
        #TODO/PORT# # Ensure the list only has max amount of entries.
        #TODO/PORT# while settings_opensave_recent_list_of_files.size() > settings_opensave_recent_max_files:
        #TODO/PORT#     settings_opensave_recent_list_of_files.removeLast()

    def windowMenuAboutToShow(self):
        """
        Called when the ``aboutToShow()`` SIGNAL for :py:attr:`MainWindow.windowMenu` is emitted.
        """
        windowMenu = self.windowMenu
        qDebug("MainWindow::windowMenuAboutToShow()")
        windowMenu.clear()
        windowMenu.addAction(actionHash.value(ACTION_windowclose))
        windowMenu.addAction(actionHash.value(ACTION_windowcloseall))
        windowMenu.addSeparator()
        windowMenu.addAction(actionHash.value(ACTION_windowcascade))
        windowMenu.addAction(actionHash.value(ACTION_windowtile))
        windowMenu.addSeparator()
        windowMenu.addAction(actionHash.value(ACTION_windownext))
        windowMenu.addAction(actionHash.value(ACTION_windowprevious))

        windowMenu.addSeparator()
        windows = mdiArea.subWindowList()  # QList<QMdiSubWindow*>
        for i in range(0, windows.count()):  # for(int i = 0; i < windows.count(); ++i)

            aAction = QAction(windows.at(i).windowTitle(), self)  # QAction*
            aAction.setCheckable(true)
            aAction.setData(i)
            windowMenu.addAction(aAction)
            self.connect(aAction, SIGNAL("toggled(bool)"), self, SLOT("windowMenuActivated(bool)"))
            aAction.setChecked(mdiArea.activeSubWindow() == windows.at(i))

    def windowMenuActivated(self, checked):
        """
        Called when the ``toggled(bool)`` SIGNAL for an `QAction`_ connected with a :py:attr:`MainWindow.windowMenu` menu item is emitted.

        :param `checked`: TOWRITE
        :type `checked`: bool
        """
        qDebug("MainWindow::windowMenuActivated()")
        aSender = self.sender()  # QAction* aSender = qobject_cast<QAction*>(sender());
        if not aSender:
            return
        w = self.mdiArea.subWindowList().at(aSender.data().toInt())  # QWidget*
        if w and checked:
            w.setFocus()

    def getMdiArea(self):
        """
        Get the `MdiArea` instance.

        :rtype: `MdiArea`
        """
        qDebug("MainWindow::getMdiArea()")
        return self.mdiArea

    def getApplication(self):
        """
        Get the `MainWindow` instance.

        :rtype: `MainWindow`
        """
        qDebug("MainWindow::getApplication()")
        return self.mainWin

    def newFile(self):
        """
        TOWRITE
        """
        qDebug("MainWindow::newFile()")
        self.docIndex += 1  # docIndex++;
        self.numOfDocs += 1  # numOfDocs++;
        mdiWin = MdiWindow(self.docIndex, self.mainWin, self.mdiArea, Qt.SubWindow)  # MdiWindow*
        self.connect(mdiWin, SIGNAL("sendCloseMdiWin(MdiWindow)"), self, SLOT("onCloseMdiWin(MdiWindow)"))
        self.connect(self.mdiArea, SIGNAL("subWindowActivated(QMdiSubWindow)"), self, SLOT("onWindowActivated(QMdiSubWindow)"))

        self.updateMenuToolbarStatusbar()
        self.windowMenuAboutToShow()

        v = mdiWin.getView()  # View*
        if v:
            v.recalculateLimits()
            v.zoomExtents()

    def openFile(self, recent, recentFile):
        """
        TOWRITE

        :param `recent`: TOWRITE
        :type `recent`: bool
        :param `recentFile`: TOWRITE
        :type `recentFile`: QString
        """
        qDebug("MainWindow::openFile()")

        QApplication.setOverrideCursor(Qt.ArrowCursor)

        files = []  # QStringList files
        preview = self.getSettingsOpenThumbnail()  # bool
        openFilesPath = self.settings_opensave_recent_directory

        # Check to see if this from the recent files list.
        if recent:
            files.append(recentFile)
            self.openFilesSelected(files)

        elif not preview:
            # TODO: set getOpenFileNames' selectedFilter parameter from settings_opensave_open_format
            files = QFileDialog.getOpenFileNames(self, self.tr("Open"), openFilesPath, self.formatFilterOpen)
            self.openFilesSelected(files)

        elif preview:
            openDialog = PreviewDialog(self, self.tr("Open w/Preview"), openFilesPath, self.formatFilterOpen)  # PreviewDialog*
            # TODO: set openDialog->selectNameFilter(const QString& filter) from settings_opensave_open_format
            self.connect(openDialog, SIGNAL("filesSelected(QStringList)"), self, SLOT("openFilesSelected(QStringList)"))
            openDialog.exec_()

        QApplication.restoreOverrideCursor()

    def openFilesSelected(self, filesToOpen):
        """
        TOWRITE

        :param `filesToOpen`: TOWRITE
        :type `filesToOpen`: QStringList
        """
        doOnce = True  # bool

        if filesToOpen.count():

            for i in range(0, filesToOpen.count()):  # for(int i = 0; i < filesToOpen.count(); i++)

                if not self.validFileFormat(filesToOpen[i]):
                    continue

                existing = self.findMdiWindow(filesToOpen[i])  # QMdiSubWindow*
                if existing:
                    mdiArea.setActiveSubWindow(existing)
                    continue

                # The docIndex doesn't need increased as it is only used for unnamed files
                self.numOfDocs += 1  # numOfDocs++;
                mdiWin = MdiWindow(self.docIndex, self.mainWin, self.mdiArea, Qt.SubWindow)  # MdiWindow*
                self.connect(mdiWin, SIGNAL("sendCloseMdiWin(MdiWindow)"), self, SLOT("onCloseMdiWin(MdiWindow)"));
                self.connect(self.mdiArea, SIGNAL("subWindowActivated(QMdiSubWindow)"), self, SLOT("onWindowActivated(QMdiSubWindow)"))

                # Make sure the toolbars/etc... are shown before doing their zoomExtents
                if doOnce:
                    self.updateMenuToolbarStatusbar()
                    doOnce = False

                if mdiWin.loadFile(filesToOpen.at(i)):

                    self.statusbar.showMessage(self.tr("File(s) loaded"), 2000)
                    mdiWin.show()
                    mdiWin.showMaximized()
                    # Prevent duplicate entries in the recent files list.
                    if not self.settings_opensave_recent_list_of_files.contains(filesToOpen.at(i), Qt.CaseInsensitive):
                        self.settings_opensave_recent_list_of_files.prepend(filesToOpen.at(i))

                    # Move the recent file to the top of the list.
                    else:
                        self.settings_opensave_recent_list_of_files.removeAll(filesToOpen.at(i))
                        self.settings_opensave_recent_list_of_files.prepend(filesToOpen.at(i))

                    self.settings_opensave_recent_directory = QFileInfo(filesToOpen.at(i)).absolutePath()

                    v = mdiWin.getView()  # View*
                    if v:
                        v.recalculateLimits()
                        v.zoomExtents()

                else:
                    mdiWin.close()

        self.windowMenuAboutToShow()

    def openrecentfile(self):
        """
        TOWRITE
        """
        qDebug("MainWindow::openrecentfile()")

        # Check to see if this from the recent files list.
        recentSender = self.sender()  # QAction* recentSender = qobject_cast<QAction*>(sender());
        if recentSender:
            self.openFile(True, recentSender.data().toString())

    def savefile(self):
        """
        TOWRITE
        """
        qDebug("MainWindow::savefile()")

    def saveasfile(self):
        """
        TOWRITE
        """
        qDebug("MainWindow::saveasfile()")
        # need to find the activeSubWindow before it loses focus to the FileDialog.
        mdiWin = self.mdiArea.activeSubWindow()  # MdiWindow* mdiWin = qobject_cast<MdiWindow*>(mdiArea->activeSubWindow());
        if not mdiWin:
            return

        file = ""  # QString file;
        openFilesPath = self.settings_opensave_recent_directory
        file = QFileDialog.getSaveFileName(self, self.tr("Save As"), openFilesPath, self.formatFilterSave)

        mdiWin.saveFile(file)

    def findMdiWindow(self, fileName):
        """
        TOWRITE

        :param `fileName`: TOWRITE
        :type `fileName`: QString
        :rtype: `QMdiSubWindow`_
        """
        qDebug("MainWindow::findMdiWindow(%s)", qPrintable(fileName))
        canonicalFilePath = QFileInfo(fileName).canonicalFilePath()  # QString

        for subWindow in self.mdiArea.subWindowList(): # foreach(QMdiSubWindow* subWindow, mdiArea->subWindowList())

            mdiWin = subWindow  # MdiWindow* mdiWin = qobject_cast<MdiWindow*>(subWindow);
            if mdiWin:

                if mdiWin.getCurrentFile() == canonicalFilePath:

                    return subWindow

        return 0

    def closeEvent(self, event):
        """
        Handles the ``closeEvent`` for :class:`MainWindow`.

        :param `event`: A `QCloseEvent`_ to be processed.
        """
        self.mdiArea.closeAllSubWindows()
        self.writeSettings()
        event.accept()

    def onCloseWindow(self):
        """
        TOWRITE
        """
        qDebug("MainWindow::onCloseWindow()")
        mdiWin = self.mdiArea.activeSubWindow()  # MdiWindow* mdiWin = qobject_cast<MdiWindow*>(mdiArea->activeSubWindow());
        if mdiWin:
            self.onCloseMdiWin(mdiWin)

    def onCloseMdiWin(self, theMdiWin):
        """
        TOWRITE

        :param `theMdiWin`: TOWRITE
        :type `theMdiWin`: `MdiWindow`
        """
        qDebug("MainWindow::onCloseMdiWin()")
        self.numOfDocs -= 1  # numOfDocs--;

        keepMaximized = False  # bool keepMaximized;
        if theMdiWin:
            keepMaximized = theMdiWin.isMaximized()

        self.mdiArea.removeSubWindow(theMdiWin)
        theMdiWin.deleteLater()

        self.updateMenuToolbarStatusbar()
        self.windowMenuAboutToShow()

        if keepMaximized:

            mdiWin = mdiArea.activeSubWindow()  # MdiWindow* mdiWin = qobject_cast<MdiWindow*>(mdiArea->activeSubWindow());
            if mdiWin:
                mdiWin.showMaximized()

    def onWindowActivated(self, w):
        """
        TOWRITE

        :param `w`: TOWRITE
        :type `w`: `QMdiSubWindow`_
        """
        qDebug("MainWindow::onWindowActivated()")
        mdiWin = w  # MdiWindow* mdiWin = qobject_cast<MdiWindow*>(w);
        if mdiWin:
            mdiWin.onWindowActivated()

    def resizeEvent(self, e):
        """
        TOWRITE

        :param `e`: TOWRITE
        :type `e`: `QResizeEvent`_
        """
        qDebug("MainWindow::resizeEvent()")
        QMainWindow.resizeEvent(e)
        self.statusBar().setSizeGripEnabled(not self.isMaximized())

    def getFileSeparator(self):
        """
        TOWRITE

        :rtype: `QAction`_
        """
        qDebug("MainWindow::getFileSeparator()")
        return self.myFileSeparator

    def updateMenuToolbarStatusbar(self):
        """
        TOWRITE
        """
        actionHash = self.actionHash
        numOfDocs = self.numOfDocs
        qDebug("MainWindow::updateMenuToolbarStatusbar()")

        actionHash["ACTION_print"].setEnabled(numOfDocs > 0)
        actionHash["ACTION_windowclose"].setEnabled(numOfDocs > 0)
        actionHash["ACTION_designdetails"].setEnabled(numOfDocs > 0)

        if numOfDocs:
            # Toolbars
            self.toolbarView.show()
            self.toolbarZoom.show()
            self.toolbarPan.show()
            self.toolbarIcon.show()
            self.toolbarHelp.show()
            self.toolbarLayer.show()
            self.toolbarText.show()
            self.toolbarProperties.show()
            self.toolbarPrompt.show()

            for tb in toolbarHash:  # foreach(QToolBar* tb, toolbarHash)
                tb.show()  #PORT/NOTE# Ummm why are you showing these twice? Redundant...?

            # DockWidgets
            self.dockPropEdit.show()
            self.dockUndoEdit.show()

            # Menus
            self.menuBar().clear();
            self.menuBar().addMenu(self.fileMenu)
            self.menuBar().addMenu(self.editMenu)
            self.menuBar().addMenu(self.viewMenu)

            for menu in menuHash:  # foreach(QMenu* menu, menuHash)
                self.menuBar().addMenu(menu)

            self.menuBar().addMenu(self.settingsMenu)
            self.menuBar().addMenu(self.windowMenu)
            self.menuBar().addMenu(self.helpMenu)

            self.windowMenu.setEnabled(True)

            # Statusbar
            statusbar = self.statusbar
            statusbar.clearMessage()
            statusbar.statusBarMouseCoord.show()
            statusbar.statusBarSnapButton.show()
            statusbar.statusBarGridButton.show()
            statusbar.statusBarRulerButton.show()
            statusbar.statusBarOrthoButton.show()
            statusbar.statusBarPolarButton.show()
            statusbar.statusBarQSnapButton.show()
            statusbar.statusBarQTrackButton.show()
            statusbar.statusBarLwtButton.show()

        else:
            # Toolbars
            self.toolbarView.hide()
            self.toolbarZoom.hide()
            self.toolbarPan.hide()
            self.toolbarIcon.hide()
            self.toolbarHelp.hide()
            self.toolbarLayer.hide()
            self.toolbarText.hide()
            self.toolbarProperties.hide()
            self.toolbarPrompt.hide()
            for tb in toolbarHash:  # foreach(QToolBar* tb, toolbarHash)
                tb.hide()

            # DockWidgets
            self.dockPropEdit.hide()
            self.dockUndoEdit.hide()

            # Menus
            self.menuBar().clear()
            self.menuBar().addMenu(self.fileMenu)
            self.menuBar().addMenu(self.editMenu)
            self.menuBar().addMenu(self.settingsMenu)
            self.menuBar().addMenu(self.windowMenu)
            self.menuBar().addMenu(self.helpMenu)

            self.windowMenu.setEnabled(False)

            # Statusbar
            statusbar = self.statusbar
            statusbar.clearMessage()
            statusbar.statusBarMouseCoord.hide()
            statusbar.statusBarSnapButton.hide()
            statusbar.statusBarGridButton.hide()
            statusbar.statusBarRulerButton.hide()
            statusbar.statusBarOrthoButton.hide()
            statusbar.statusBarPolarButton.hide()
            statusbar.statusBarQSnapButton.hide()
            statusbar.statusBarQTrackButton.hide()
            statusbar.statusBarLwtButton.hide()

        self.hideUnimplemented()

    def hideUnimplemented(self):
        """
        TOWRITE
        """
        qDebug("MainWindow::hideUnimplemented()")

    def validFileFormat(self, fileName):
        """
        TOWRITE

        :param `fileName`: TOWRITE
        :type `fileName`: QString
        :rtype: bool
        """
        #TODO/PORT# Check if this is right!
        unusedStr = 0       # char* unusedStr = 0;
        unusedChar = 0      # char unusedChar;
        unusedInt = 0       # int unusedInt;
        if embFormat_info(qPrintable(fileName), unusedStr, unusedStr, unusedChar, unusedChar, unusedInt):
            return True
        return False

    def loadFormats(self):
        """
        TOWRITE
        """
        pass
        #TODO/PORT# char stable, unstable;
        #TODO/PORT# QString supportedReaders  = "All Supported Files (";
        #TODO/PORT# QString individualReaders = "All Files (*);;";
        #TODO/PORT# QString supportedWriters  = "All Supported Files (";
        #TODO/PORT# QString individualWriters = "All Files (*);;";
        #TODO/PORT# QString supportedStr;
        #TODO/PORT# QString individualStr;
        #TODO/PORT#
        #TODO/PORT# //TODO: Stable Only (Settings Option)
        #TODO/PORT# //stable = 'S'; unstable = 'S';
        #TODO/PORT#
        #TODO/PORT# //Stable + Unstable
        #TODO/PORT# stable = 'S'; unstable = 'U';
        #TODO/PORT#
        #TODO/PORT# char* extension = 0;
        #TODO/PORT# char* description = 0;
        #TODO/PORT# char readerState;
        #TODO/PORT# char writerState;
        #TODO/PORT# int type;
        #TODO/PORT#
        #TODO/PORT# EmbFormatList* curFormat = 0;
        #TODO/PORT# EmbFormatList* formatList = embFormatList_create();
        #TODO/PORT# if(!formatList) { QMessageBox::critical(this, tr("Format Loading Error"), tr("Unable to load formats from libembroidery.")); return; }
        #TODO/PORT# curFormat = formatList;
        #TODO/PORT# while(curFormat)
        #TODO/PORT# {
        #TODO/PORT#     if(embFormat_info(curFormat->extension, &extension, &description, &readerState, &writerState, &type))
        #TODO/PORT#     {
        #TODO/PORT#         QString upperExt = QString(extension).toUpper();
        #TODO/PORT#         supportedStr = "*" + upperExt + " ";
        #TODO/PORT#         individualStr = upperExt.replace(".", "") + " - " + description + " (*" + extension + ");;";
        #TODO/PORT#         if(readerState == stable || readerState == unstable)
        #TODO/PORT#         {
        #TODO/PORT#             //Exclude color file formats from open dialogs
        #TODO/PORT#             if(upperExt != "COL" && upperExt != "EDR" && upperExt != "INF" && upperExt != "RGB")
        #TODO/PORT#             {
        #TODO/PORT#                 supportedReaders.append(supportedStr);
        #TODO/PORT#                 individualReaders.append(individualStr);
        #TODO/PORT#             }
        #TODO/PORT#         }
        #TODO/PORT#         if(writerState == stable || writerState == unstable)
        #TODO/PORT#         {
        #TODO/PORT#             supportedWriters.append(supportedStr);
        #TODO/PORT#             individualWriters.append(individualStr);
        #TODO/PORT#         }
        #TODO/PORT#     }
        #TODO/PORT#     curFormat = curFormat->next;
        #TODO/PORT# }
        #TODO/PORT# embFormatList_free(formatList);
        #TODO/PORT# formatList = 0;
        #TODO/PORT#
        #TODO/PORT# supportedReaders.append(");;");
        #TODO/PORT# supportedWriters.append(");;");
        #TODO/PORT#
        #TODO/PORT# formatFilterOpen = supportedReaders + individualReaders;
        #TODO/PORT# formatFilterSave = supportedWriters + individualWriters;
        #TODO/PORT#
        #TODO/PORT# //TODO: Fixup custom filter
        #TODO/PORT# /*
        #TODO/PORT# QString custom = getSettingsCustomFilter();
        #TODO/PORT# if(custom.contains("supported", Qt::CaseInsensitive))
        #TODO/PORT#     custom = ""; //This will hide it
        #TODO/PORT# else if(!custom.contains("*", Qt::CaseInsensitive))
        #TODO/PORT#     custom = ""; //This will hide it
        #TODO/PORT# else
        #TODO/PORT#     custom = "Custom Filter(" + custom + ");;";
        #TODO/PORT#
        #TODO/PORT# return tr(qPrintable(custom + supported + all));
        #TODO/PORT# */

    def closeToolBar(self, action):
        """
        TOWRITE

        :param `action`: TOWRITE
        :type `action`: `QAction`_
        """
        if action.objectName() == "toolbarclose":
            tb = self.sender()  # QToolBar* tb = qobject_cast<QToolBar*>(sender());
            if tb:
                qDebug("%s closed.", qPrintable(tb.objectName()))
                tb.hide()

    def floatingChangedToolBar(self, isFloating):
        """
        TOWRITE

        :param `isFloating`: TOWRITE
        :type `isFloating`: bool
        """
        tb = self.sender()  # QToolBar* tb = qobject_cast<QToolBar*>(sender());
        if tb:

            if isFloating:

                #TODO/PORT#  /*
                #TODO/PORT#  //TODO: Determine best suited close button on various platforms.
                #TODO/PORT#  QStyle::SP_DockWidgetCloseButton
                #TODO/PORT#  QStyle::SP_TitleBarCloseButton
                #TODO/PORT#  QStyle::SP_DialogCloseButton
                #TODO/PORT#  */
                ACTION = QAction(tb.style().standardIcon(QStyle.SP_DialogCloseButton), "Close", self)  # QAction *ACTION = new QAction(tb->style()->standardIcon(QStyle::SP_DialogCloseButton), "Close", this);
                ACTION.setStatusTip("Close the " + tb.windowTitle() + " Toolbar")
                ACTION.setObjectName("toolbarclose")
                tb.addAction(ACTION)
                self.connect(tb, SIGNAL("actionTriggered(QAction)"), self, SLOT("closeToolBar(QAction)"))

            else:
                actList = tb.actions()  # QList<QAction*>
                for i in range(0, len(actList)):  # for(int i = 0; i < actList.size(); ++i)

                    ACTION = actList[i]  # QAction* ACTION = actList.value(i);
                    if ACTION.objectName() == "toolbarclose":
                        tb.removeAction(ACTION)
                        self.disconnect(tb, SIGNAL("actionTriggered(QAction)"), self, SLOT("closeToolBar(QAction)"))
                        del ACTION


# kate: bom off; indent-mode python; indent-width 4; replace-trailing-space-save on;
