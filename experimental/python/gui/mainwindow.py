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
#--Python Imports.
import os
import sys


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

try:
    if sys.version_info[0] == 2:
        import cPickle as pickle
        import ConfigParser as configparser  # Saving of ini settings
    elif sys.version_info[0] == 3:
        import pickle
        import configparser
except ImportError as exc:
    raise exc
gSettings = configparser.ConfigParser()


#--PySide/PyQt Imports.
try:
    from PySide.QtCore import (qDebug, Qt, QDate, QDir, QFile, QFileInfo, QLocale, QSize, QUrl, QPoint, QSettings, QByteArray,
        QMetaObject, QLibraryInfo, QTextStream, QTranslator, SIGNAL, SLOT, QCoreApplication, QMargins)
    from PySide.QtGui import (qRgb, qApp, QIcon, QFrame, QVBoxLayout, QMenu, QMenuBar, QAction,
        QStatusBar, QMdiArea, QWidget, QMdiSubWindow, QMessageBox, QToolBar,
        QFileDialog, QApplication, QLabel, QComboBox, QCloseEvent, QMainWindow,
        QColor, QFontComboBox, QStyle, QWizard, QWizardPage, QDialog,
        QCheckBox, QCursor, QFont, QKeySequence,
        QDesktopServices, QDialogButtonBox,
        QPushButton, QGraphicsScene, QWhatsThis
        )
    from PySide.QtScript import QScriptEngine
    from PySide.QtScriptTools import QScriptEngineDebugger
except ImportError:
    raise
#    from PyQt4.QtCore import (qDebug, Qt, QDate, QDir, QFile, QFileInfo, QLocale, QSize, QUrl, QPoint, QSettings, QByteArray,
#        QMetaObject, QLibraryInfo, QTextStream, QTranslator, SIGNAL, SLOT, QCoreApplication, QMargins)
#    from PyQt4.QtGui import (qRgb, qApp, QIcon, QFrame, QVBoxLayout, QMenu, QMenuBar,  QAction,
#        QStatusBar, QMdiArea, QWidget, QMdiSubWindow, QMessageBox, QToolBar,
#        QFileDialog, QApplication, QLabel, QComboBox, QCloseEvent, QMainWindow,
#        QColor, QFontComboBox, QStyle, QWizard, QWizardPage, QDialog,
#        QCheckBox, QCursor, QFont, QKeySequence,
#        QDesktopServices, QDialogButtonBox,
#        QPushButton, QGraphicsScene, QWhatsThis
#        )
#    from PyQt4.QtScript import QScriptEngine
#    from PyQt4.QtScriptTools import QScriptEngineDebugger

#--Local Imports.
# from mainwindow_settings import DoReadSettings, DoWriteSettings, settingsPrompt, settingsDialog
from mdiarea import MdiArea
from cmdprompt import CmdPrompt
from property_editor import PropertyEditor
from undo_editor import UndoEditor
from statusbar import StatusBar
# from mainwindow_actions import createAllActions
# from mainwindow_menus import createAllMenus
# from mainwindow_toolbars import createAllToolbars
from mdiwindow import MdiWindow
from tipoftheday_dialog import TipOfTheDayDialog
from dialog_about import AboutDialog
from embdetails_dialog import EmbDetailsDialog
from settings_dialog import Settings_Dialog
from undo_commands import UndoableAddCommand, UndoableNavCommand


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


#-Globals.----------------------------------------------------------------------
# Application Directories.

gImgDir = gAppDir + os.sep + 'images'
gIconDir = gAppDir + os.sep + 'icons'
gTrDir = gAppDir + os.sep + 'translations'

gSpiralsImgPath = gImgDir + os.sep + 'texture-spirals.png'
gLogoSpiralsImgPath = gImgDir + os.sep + 'logo-spirals.png'

major, minor, micro, release = sys.version_info[0:-1]
pythonVersion = 'Python %d.%d.%d-%s' % (major, minor, micro, release)


class MainWindow(QMainWindow):  # MainWindow::MainWindow() : QMainWindow(0)
    """
    Subclass of `QMainWindow`_

    Main Frame Window for Embroidermodder application.

    """
    def __init__(self):
        """Default class constructor."""
        super(MainWindow, self).__init__()

        # Init
        global mainWin
        self.mainWin = mainWin = self

        # Application directories.
        self.gAppDir = gAppDir
        self.gImgDir = gImgDir
        self.gIconDir = gIconDir
        self.gTrDir = gTrDir
        # local optimization.
        tr = self.tr
        connect = self.connect

        #self.settings_general_language =                            # QString
        #self.settings_general_icon_theme =                          # QString
        #self.settings_general_icon_size =                           # int
        #self.settings_general_mdi_bg_use_logo =                     # bool
        #self.settings_general_mdi_bg_use_texture =                  # bool
        #self.settings_general_mdi_bg_use_color =                    # bool
        #self.settings_general_mdi_bg_logo =                         # QString
        #self.settings_general_mdi_bg_texture =                      # QString
        #self.settings_general_mdi_bg_color =                        # QRgb
        #self.settings_general_tip_of_the_day =                      # bool
        #self.settings_general_current_tip =                         # quint16
        self.settings_general_system_help_browser = False           # bool
        self.settings_general_check_for_updates = False             # bool
        #self.settings_display_use_opengl =                          # bool
        #self.settings_display_renderhint_aa =                       # bool
        #self.settings_display_renderhint_text_aa =                  # bool
        #self.settings_display_renderhint_smooth_pix =               # bool
        #self.settings_display_renderhint_high_aa =                  # bool
        #self.settings_display_renderhint_noncosmetic =              # bool
        #self.settings_display_show_scrollbars =                     # bool
        #self.settings_display_scrollbar_widget_num =                # int
        #self.settings_display_crosshair_color =                     # QRgb
        # self.settings_display_bg_color =                            # QRgb
        # self.settings_display_selectbox_left_color =                # QRgb
        # self.settings_display_selectbox_left_fill =                 # QRgb
        # self.settings_display_selectbox_right_color =               # QRgb
        # self.settings_display_selectbox_right_fill =                # QRgb
        # self.settings_display_selectbox_alpha =                     # quint8
        # self.settings_display_zoomscale_in =                        # qreal
        # self.settings_display_zoomscale_out =                       # qreal
        # self.settings_display_crosshair_percent =                   # quint8
        # self.settings_display_units =                               # QString
        # self.settings_prompt_text_color =                           # QRgb
        # self.settings_prompt_bg_color =                             # QRgb
        # self.settings_prompt_font_family =                          # QString
        # self.settings_prompt_font_style =                           # QString
        # self.settings_prompt_font_size =                            # quint8
        # self.settings_prompt_save_history =                         # bool
        # self.settings_prompt_save_history_as_html =                 # bool
        # self.settings_prompt_save_history_filename =                # QString
        # self.settings_opensave_custom_filter =                      # QString
        # self.settings_opensave_open_format =                        # QString
        # self.settings_opensave_open_thumbnail =                     # bool
        # self.settings_opensave_save_format =                        # QString
        # self.settings_opensave_save_thumbnail =                     # bool
        # self.settings_opensave_recent_max_files =                   # quint8
        # self.settings_opensave_recent_list_of_files =               # QStringList
        # self.settings_opensave_recent_directory =                   # QString
        # self.settings_opensave_trim_dst_num_jumps =                 # quint8
        # self.settings_printing_default_device =                     # QString
        # self.settings_printing_use_last_device =                    # bool
        # self.settings_printing_disable_bg =                         # bool
        # self.settings_grid_show_on_load =                           # bool
        # self.settings_grid_show_origin =                            # bool
        # self.settings_grid_color_match_crosshair =                  # bool
        # self.settings_grid_color =                                  # QRgb
        # self.settings_grid_load_from_file =                         # bool
        # self.settings_grid_type =                                   # QString
        # self.settings_grid_center_on_origin =                       # bool
        # self.settings_grid_center_x =                               # qreal
        # self.settings_grid_center_y =                               # qreal
        # self.settings_grid_size_x =                                 # qreal
        # self.settings_grid_size_y =                                 # qreal
        # self.settings_grid_spacing_x =                              # qreal
        # self.settings_grid_spacing_y =                              # qreal
        # self.settings_grid_size_radius =                            # qreal
        # self.settings_grid_spacing_radius =                         # qreal
        # self.settings_grid_spacing_angle =                          # qreal
        # self.settings_ruler_show_on_load =                          # bool
        # self.settings_ruler_metric =                                # bool
        # self.settings_ruler_color =                                 # QRgb
        # self.settings_ruler_pixel_size =                            # quint8
        # self.settings_qsnap_enabled =                               # bool
        # self.settings_qsnap_locator_color =                         # QRgb
        # self.settings_qsnap_locator_size =                          # quint8
        # self.settings_qsnap_aperture_size =                         # quint8
        # self.settings_qsnap_endpoint =                              # bool
        # self.settings_qsnap_midpoint =                              # bool
        # self.settings_qsnap_center =                                # bool
        # self.settings_qsnap_node =                                  # bool
        # self.settings_qsnap_quadrant =                              # bool
        # self.settings_qsnap_intersection =                          # bool
        # self.settings_qsnap_extension =                             # bool
        # self.settings_qsnap_insertion =                             # bool
        # self.settings_qsnap_perpendicular =                         # bool
        # self.settings_qsnap_tangent =                               # bool
        # self.settings_qsnap_nearest =                               # bool
        # self.settings_qsnap_apparent =                              # bool
        # self.settings_qsnap_parallel =                              # bool
        # self.settings_lwt_show_lwt =                                # bool
        # self.settings_lwt_real_render =                             # bool
        # self.settings_lwt_default_lwt =                             # qreal
        # self.settings_selection_mode_pickfirst =                    # bool
        # self.settings_selection_mode_pickadd =                      # bool
        # self.settings_selection_mode_pickdrag =                     # bool
        # self.settings_selection_coolgrip_color =                    # QRgb
        # self.settings_selection_hotgrip_color =                     # QRgb
        # self.settings_selection_grip_size =                         # quint8
        # self.settings_selection_pickbox_size =                      # quint8
        #self.settings_text_font =                                   # QString
        #self.settings_text_size =                                   # qreal
        #self.settings_text_angle =                                  # qreal
        #self.settings_text_style_bold =                             # bool
        #self.settings_text_style_italic =                           # bool
        #self.settings_text_style_underline =                        # bool
        #self.settings_text_style_overline =                         # bool
        #self.settings_text_style_strikeout =                        # bool


        self.readSettings()  # self.readSettings()

        lang = self.getSettingsGeneralLanguage()  # QString
        qDebug("language: %s" % lang)
        if lang == "system":
            lang = QLocale.system().languageToString(QLocale.system().language()).toLower()

        # Load translations for the Embroidermodder 2 GUI.
        translatorEmb = QTranslator()
        translatorEmb.load("translations/" + lang + "/embroidermodder2_" + lang)
        QCoreApplication.installTranslator(translatorEmb)

        # Load translations for the commands.
        translatorCmd = QTranslator()
        translatorCmd.load("translations/" + lang + "/commands_" + lang)
        QCoreApplication.installTranslator(translatorCmd)

        # Load translations provided by Qt - this covers dialog buttons and other common things.
        translatorQt = QTranslator()
        translatorQt.load("qt_" + QLocale.system().name(), QLibraryInfo.location(QLibraryInfo.TranslationsPath))
        QCoreApplication.installTranslator(translatorQt)

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

        self.setWindowIcon(QIcon(self.gIconDir + os.sep + self.getSettingsGeneralIconTheme() + "/" + "app" + ".png"))
        self.setMinimumSize(800, 480)  # Require Minimum WVGA

        #TODO/PORT# self.loadFormats()

        # create the mdiArea.
        vbox = QFrame(self)  # QFrame*
        layout = QVBoxLayout(vbox)  # QVBoxLayout*
        layout.setContentsMargins(QMargins(0, 0, 0, 0))  # layout.setMargin(0)
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

        #TODO/PORT styleHash# prompt.setPromptTextColor(QColor(self.getSettingsPromptTextColor()))
        #TODO/PORT styleHash# prompt.setPromptBackgroundColor(QColor(self.getSettingsPromptBGColor()))

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
        self.dockPropEdit = dockPropEdit = PropertyEditor(gIconDir + os.sep + self.getSettingsGeneralIconTheme(), self.getSettingsSelectionModePickAdd(), prompt, self)
        self.addDockWidget(Qt.LeftDockWidgetArea, dockPropEdit)
        connect(dockPropEdit, SIGNAL("pickAddModeToggled()"), self, SLOT("pickAddModeToggled()"))

        # create the Command History Undo Editor.
        self.dockUndoEdit = dockUndoEdit = UndoEditor(gIconDir + os.sep + self.getSettingsGeneralIconTheme(), prompt, self)
        self.addDockWidget(Qt.LeftDockWidgetArea, dockUndoEdit);

        ##setDockOptions(QMainWindow.AnimatedDocks | QMainWindow.AllowTabbedDocks | QMainWindow.VerticalTabs)  # TODO: Load these from settings
        ##tabifyDockWidget(dockPropEdit, dockUndoEdit)  # TODO: load this from settings

        # Javascript
        #TODO/PORT# self.initMainWinPointer(self)

        self.engine = engine = QScriptEngine(self)
        engine.installTranslatorFunctions()
        self.debugger = QScriptEngineDebugger(self)
        self.debugger.attachTo(engine)
        #TODO/PORT# self.javaInitNatives(engine)

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
        
        if self.getSettingsGeneralTipOfTheDay():
            self.tipOfTheDay()


    def __del__(self):
        """Class destructor."""
        qDebug("MainWindow::Destructor()")

        # Prevent memory leaks by deleting any unpasted objects
        #TODO/PORT# qDeleteAll(cutCopyObjectList.begin(), cutCopyObjectList.end())
        #TODO/PORT# cutCopyObjectList.clear()

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
        actionHash = self.actionHash
        qDebug("MainWindow::windowMenuAboutToShow()")
        windowMenu.clear()
        windowMenu.addAction(actionHash["ACTION_windowclose"])
        windowMenu.addAction(actionHash["ACTION_windowcloseall"])
        windowMenu.addSeparator()
        windowMenu.addAction(actionHash["ACTION_windowcascade"])
        windowMenu.addAction(actionHash["ACTION_windowtile"])
        windowMenu.addSeparator()
        windowMenu.addAction(actionHash["ACTION_windownext"])
        windowMenu.addAction(actionHash["ACTION_windowprevious"])

        windowMenu.addSeparator()
        windows = self.mdiArea.subWindowList()  # QList<QMdiSubWindow*>
        for i in range(0, len(windows)):  # for(int i = 0; i < windows.count(); ++i)

            aAction = QAction(windows[i].windowTitle(), self)  # QAction*
            aAction.setCheckable(True)
            aAction.setData(i)
            windowMenu.addAction(aAction)
            self.connect(aAction, SIGNAL("toggled(bool)"), self, SLOT("windowMenuActivated(bool)"))
            aAction.setChecked(self.mdiArea.activeSubWindow() == windows[i])

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
        w = self.mdiArea.subWindowList()[aSender.data()]  # QWidget*
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
        event.accept()
        self.writeSettings()

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

    #TODO/PORT# def resizeEvent(self, e):
    #TODO/PORT#     """
    #TODO/PORT#     TOWRITE
    #TODO/PORT#
    #TODO/PORT#     :param `e`: TOWRITE
    #TODO/PORT#     :type `e`: `QResizeEvent`_
    #TODO/PORT#     """
    #TODO/PORT#     qDebug("MainWindow::resizeEvent()")
    #TODO/PORT#     self.resizeEvent(e)
    #TODO/PORT#     self.statusBar().setSizeGripEnabled(not self.isMaximized())

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

            #TODO/PORT# for tb in toolbarHash:  # foreach(QToolBar* tb, toolbarHash)
            #TODO/PORT#     tb.show()  #PORT/NOTE# Ummm why are you showing these twice? Redundant...?

            # DockWidgets
            self.dockPropEdit.show()
            self.dockUndoEdit.show()

            # Menus
            self.menuBar().clear();
            self.menuBar().addMenu(self.fileMenu)
            self.menuBar().addMenu(self.editMenu)
            self.menuBar().addMenu(self.viewMenu)

            #TODO/PORT# for menu in menuHash:  # foreach(QMenu* menu, menuHash)
            #TODO/PORT#     self.menuBar().addMenu(menu)

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
            #TODO/PORT# for tb in toolbarHash:  # foreach(QToolBar* tb, toolbarHash)
            #TODO/PORT#     tb.hide()

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


    # mainwindow.hhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhh

    def setUndoCleanIcon(self, opened):
        """
        TOWRITE

        :param `opened`: TOWRITE
        :type `opened`: bool
        """
        pass #TODO/PORT# Is this supposed to be empty?

    def isCommandActive(self):
        """
        TOWRITE

        :rtype: bool
        """
        return self.prompt.isCommandActive()

    def activeCommand(self):
        """
        TOWRITE

        :rtype: QString
        """
        return self.prompt.activeCommand()


    def getSettingsGeneralLanguage(self):              return self.settings_general_language               # :rtype: QString
    def getSettingsGeneralIconTheme(self):             return self.settings_general_icon_theme             # :rtype: QString
    def getSettingsGeneralIconSize(self):              return self.settings_general_icon_size              # :rtype: int
    def getSettingsGeneralMdiBGUseLogo(self):          return self.settings_general_mdi_bg_use_logo        # :rtype: bool
    def getSettingsGeneralMdiBGUseTexture(self):       return self.settings_general_mdi_bg_use_texture     # :rtype: bool
    def getSettingsGeneralMdiBGUseColor(self):         return self.settings_general_mdi_bg_use_color       # :rtype: bool
    def getSettingsGeneralMdiBGLogo(self):             return self.settings_general_mdi_bg_logo            # :rtype: QString
    def getSettingsGeneralMdiBGTexture(self):          return self.settings_general_mdi_bg_texture         # :rtype: QString
    def getSettingsGeneralMdiBGColor(self):            return self.settings_general_mdi_bg_color           # :rtype: QRgb
    def getSettingsGeneralTipOfTheDay(self):           return self.settings_general_tip_of_the_day         # :rtype: bool
    def getSettingsGeneralCurrentTip(self):            return self.settings_general_current_tip            # :rtype: int
    def getSettingsGeneralSystemHelpBrowser(self):     return self.settings_general_system_help_browser    # :rtype: bool
    def getSettingsGeneralCheckForUpdates(self):       return self.settings_general_check_for_updates      # :rtype: bool
    def getSettingsDisplayUseOpenGL(self):             return self.settings_display_use_opengl             # :rtype: bool
    def getSettingsDisplayRenderHintAA(self):          return self.settings_display_renderhint_aa          # :rtype: bool
    def getSettingsDisplayRenderHintTextAA(self):      return self.settings_display_renderhint_text_aa     # :rtype: bool
    def getSettingsDisplayRenderHintSmoothPix(self):   return self.settings_display_renderhint_smooth_pix  # :rtype: bool
    def getSettingsDisplayRenderHintHighAA(self):      return self.settings_display_renderhint_high_aa     # :rtype: bool
    def getSettingsDisplayRenderHintNonCosmetic(self): return self.settings_display_renderhint_noncosmetic # :rtype: bool
    def getSettingsDisplayShowScrollBars(self):        return self.settings_display_show_scrollbars        # :rtype: bool
    def getSettingsDisplayScrollBarWidgetNum(self):    return self.settings_display_scrollbar_widget_num   # :rtype: int
    def getSettingsDisplayCrossHairColor(self):        return self.settings_display_crosshair_color        # :rtype: QRgb
    def getSettingsDisplayBGColor(self):               return self.settings_display_bg_color               # :rtype: QRgb
    def getSettingsDisplaySelectBoxLeftColor(self):    return self.settings_display_selectbox_left_color   # :rtype: QRgb
    def getSettingsDisplaySelectBoxLeftFill(self):     return self.settings_display_selectbox_left_fill    # :rtype: QRgb
    def getSettingsDisplaySelectBoxRightColor(self):   return self.settings_display_selectbox_right_color  # :rtype: QRgb
    def getSettingsDisplaySelectBoxRightFill(self):    return self.settings_display_selectbox_right_fill   # :rtype: QRgb
    def getSettingsDisplaySelectBoxAlpha(self):        return self.settings_display_selectbox_alpha        # :rtype: quint8
    def getSettingsDisplayZoomScaleIn(self):           return self.settings_display_zoomscale_in           # :rtype: qreal
    def getSettingsDisplayZoomScaleOut(self):          return self.settings_display_zoomscale_out          # :rtype: qreal
    def getSettingsDisplayCrossHairPercent(self):      return self.settings_display_crosshair_percent      # :rtype: quint8
    def getSettingsDisplayUnits(self):                 return self.settings_display_units                  # :rtype: QString
    def getSettingsPromptTextColor(self):              return self.settings_prompt_text_color              # :rtype: QRgb
    def getSettingsPromptBGColor(self):                return self.settings_prompt_bg_color                # :rtype: QRgb
    def getSettingsPromptFontFamily(self):             return self.settings_prompt_font_family             # :rtype: QString
    def getSettingsPromptFontStyle(self):              return self.settings_prompt_font_style              # :rtype: QString
    def getSettingsPromptFontSize(self):               return self.settings_prompt_font_size               # :rtype: quint8
    def getSettingsPromptSaveHistory(self):            return self.settings_prompt_save_history            # :rtype: bool
    def getSettingsPromptSaveHistoryAsHtml(self):      return self.settings_prompt_save_history_as_html    # :rtype: bool
    def getSettingsPromptSaveHistoryFilename(self):    return self.settings_prompt_save_history_filename   # :rtype: QString
    def getSettingsCustomFilter(self):                 return self.settings_opensave_custom_filter         # :rtype: QString
    def getSettingsOpenFormat(self):                   return self.settings_opensave_open_format           # :rtype: QString
    def getSettingsOpenThumbnail(self):                return self.settings_opensave_open_thumbnail        # :rtype: bool
    def getSettingsSaveFormat(self):                   return self.settings_opensave_save_format           # :rtype: QString
    def getSettingsSaveThumbnail(self):                return self.settings_opensave_save_thumbnail        # :rtype: bool
    def getSettingsRecentMaxFiles(self):               return self.settings_opensave_recent_max_files      # :rtype: quint8
    def getSettingsOpenSaveTrimDstNumJumps(self):      return self.settings_opensave_trim_dst_num_jumps    # :rtype: quint8
    def getSettingsPrintingDefaultDevice(self):        return self.settings_printing_default_device        # :rtype: QString
    def getSettingsPrintingUseLastDevice(self):        return self.settings_printing_use_last_device       # :rtype: bool
    def getSettingsPrintingDisableBG(self):            return self.settings_printing_disable_bg            # :rtype: bool
    def getSettingsGridShowOnLoad(self):               return self.settings_grid_show_on_load              # :rtype: bool
    def getSettingsGridShowOrigin(self):               return self.settings_grid_show_origin               # :rtype: bool
    def getSettingsGridColorMatchCrossHair(self):      return self.settings_grid_color_match_crosshair     # :rtype: bool
    def getSettingsGridColor(self):                    return self.settings_grid_color                     # :rtype: QRgb
    def getSettingsGridLoadFromFile(self):             return self.settings_grid_load_from_file            # :rtype: bool
    def getSettingsGridType(self):                     return self.settings_grid_type                      # :rtype: QString
    def getSettingsGridCenterOnOrigin(self):           return self.settings_grid_center_on_origin          # :rtype: bool
    def getSettingsGridCenterX(self):                  return self.settings_grid_center_x                  # :rtype: qreal
    def getSettingsGridCenterY(self):                  return self.settings_grid_center_y                  # :rtype: qreal
    def getSettingsGridSizeX(self):                    return self.settings_grid_size_x                    # :rtype: qreal
    def getSettingsGridSizeY(self):                    return self.settings_grid_size_y                    # :rtype: qreal
    def getSettingsGridSpacingX(self):                 return self.settings_grid_spacing_x                 # :rtype: qreal
    def getSettingsGridSpacingY(self):                 return self.settings_grid_spacing_y                 # :rtype: qreal
    def getSettingsGridSizeRadius(self):               return self.settings_grid_size_radius               # :rtype: qreal
    def getSettingsGridSpacingRadius(self):            return self.settings_grid_spacing_radius            # :rtype: qreal
    def getSettingsGridSpacingAngle(self):             return self.settings_grid_spacing_angle             # :rtype: qreal
    def getSettingsRulerShowOnLoad(self):              return self.settings_ruler_show_on_load             # :rtype: bool
    def getSettingsRulerMetric(self):                  return self.settings_ruler_metric                   # :rtype: bool
    def getSettingsRulerColor(self):                   return self.settings_ruler_color                    # :rtype: QRgb
    def getSettingsRulerPixelSize(self):               return self.settings_ruler_pixel_size               # :rtype: quint8
    def getSettingsQSnapEnabled(self):                 return self.settings_qsnap_enabled                  # :rtype: bool
    def getSettingsQSnapLocatorColor(self):            return self.settings_qsnap_locator_color            # :rtype: QRgb
    def getSettingsQSnapLocatorSize(self):             return self.settings_qsnap_locator_size             # :rtype: quint8
    def getSettingsQSnapApertureSize(self):            return self.settings_qsnap_aperture_size            # :rtype: quint8
    def getSettingsQSnapEndPoint(self):                return self.settings_qsnap_endpoint                 # :rtype: bool
    def getSettingsQSnapMidPoint(self):                return self.settings_qsnap_midpoint                 # :rtype: bool
    def getSettingsQSnapCenter(self):                  return self.settings_qsnap_center                   # :rtype: bool
    def getSettingsQSnapNode(self):                    return self.settings_qsnap_node                     # :rtype: bool
    def getSettingsQSnapQuadrant(self):                return self.settings_qsnap_quadrant                 # :rtype: bool
    def getSettingsQSnapIntersection(self):            return self.settings_qsnap_intersection             # :rtype: bool
    def getSettingsQSnapExtension(self):               return self.settings_qsnap_extension                # :rtype: bool
    def getSettingsQSnapInsertion(self):               return self.settings_qsnap_insertion                # :rtype: bool
    def getSettingsQSnapPerpendicular(self):           return self.settings_qsnap_perpendicular            # :rtype: bool
    def getSettingsQSnapTangent(self):                 return self.settings_qsnap_tangent                  # :rtype: bool
    def getSettingsQSnapNearest(self):                 return self.settings_qsnap_nearest                  # :rtype: bool
    def getSettingsQSnapApparent(self):                return self.settings_qsnap_apparent                 # :rtype: bool
    def getSettingsQSnapParallel(self):                return self.settings_qsnap_parallel                 # :rtype: bool
    def getSettingsLwtShowLwt(self):                   return self.settings_lwt_show_lwt                   # :rtype: bool
    def getSettingsLwtRealRender(self):                return self.settings_lwt_real_render                # :rtype: bool
    def getSettingsLwtDefaultLwt(self):                return self.settings_lwt_default_lwt                # :rtype: qreal
    def getSettingsSelectionModePickFirst(self):       return self.settings_selection_mode_pickfirst       # :rtype: bool
    def getSettingsSelectionModePickAdd(self):         return self.settings_selection_mode_pickadd         # :rtype: bool
    def getSettingsSelectionModePickDrag(self):        return self.settings_selection_mode_pickdrag        # :rtype: bool
    def getSettingsSelectionCoolGripColor(self):       return self.settings_selection_coolgrip_color       # :rtype: QRgb
    def getSettingsSelectionHotGripColor(self):        return self.settings_selection_hotgrip_color        # :rtype: QRgb
    def getSettingsSelectionGripSize(self):            return self.settings_selection_grip_size            # :rtype: quint8
    def getSettingsSelectionPickBoxSize(self):         return self.settings_selection_pickbox_size         # :rtype: quint8
    def getSettingsTextFont(self):                     return self.settings_text_font                      # :rtype: QString
    def getSettingsTextSize(self):                     return self.settings_text_size                      # :rtype: qreal
    def getSettingsTextAngle(self):                    return self.settings_text_angle                     # :rtype: qreal
    def getSettingsTextStyleBold(self):                return self.settings_text_style_bold                # :rtype: bool
    def getSettingsTextStyleItalic(self):              return self.settings_text_style_italic              # :rtype: bool
    def getSettingsTextStyleUnderline(self):           return self.settings_text_style_underline           # :rtype: bool
    def getSettingsTextStyleStrikeOut(self):           return self.settings_text_style_strikeout           # :rtype: bool
    def getSettingsTextStyleOverline(self):            return self.settings_text_style_overline            # :rtype: bool

    def setSettingsGeneralLanguage(self, newValue):              self.settings_general_language               = newValue  # :param QString `newValue`:
    def setSettingsGeneralIconTheme(self, newValue):             self.settings_general_icon_theme             = newValue  # :param QString `newValue`:
    def setSettingsGeneralIconSize(self, newValue):              self.settings_general_icon_size              = newValue  # :param int     `newValue`:
    def setSettingsGeneralMdiBGUseLogo(self, newValue):          self.settings_general_mdi_bg_use_logo        = newValue  # :param bool    `newValue`:
    def setSettingsGeneralMdiBGUseTexture(self, newValue):       self.settings_general_mdi_bg_use_texture     = newValue  # :param bool    `newValue`:
    def setSettingsGeneralMdiBGUseColor(self, newValue):         self.settings_general_mdi_bg_use_color       = newValue  # :param bool    `newValue`:
    def setSettingsGeneralMdiBGLogo(self, newValue):             self.settings_general_mdi_bg_logo            = newValue  # :param QString `newValue`:
    def setSettingsGeneralMdiBGTexture(self, newValue):          self.settings_general_mdi_bg_texture         = newValue  # :param QString `newValue`:
    def setSettingsGeneralMdiBGColor(self, newValue):            self.settings_general_mdi_bg_color           = newValue  # :param QRgb    `newValue`:
    def setSettingsGeneralTipOfTheDay(self, newValue):           self.settings_general_tip_of_the_day         = newValue  # :param bool    `newValue`:
    def setSettingsGeneralCurrentTip(self, newValue):            self.settings_general_current_tip            = newValue  # :param int     `newValue`:
    def setSettingsGeneralSystemHelpBrowser(self, newValue):     self.settings_general_system_help_browser    = newValue  # :param bool    `newValue`:
    def setSettingsGeneralCheckForUpdates(self, newValue):       self.settings_general_check_for_updates      = newValue  # :param bool    `newValue`:
    def setSettingsDisplayUseOpenGL(self, newValue):             self.settings_display_use_opengl             = newValue  # :param bool    `newValue`:
    def setSettingsDisplayRenderHintAA(self, newValue):          self.settings_display_renderhint_aa          = newValue  # :param bool    `newValue`:
    def setSettingsDisplayRenderHintTextAA(self, newValue):      self.settings_display_renderhint_text_aa     = newValue  # :param bool    `newValue`:
    def setSettingsDisplayRenderHintSmoothPix(self, newValue):   self.settings_display_renderhint_smooth_pix  = newValue  # :param bool    `newValue`:
    def setSettingsDisplayRenderHintHighAA(self, newValue):      self.settings_display_renderhint_high_aa     = newValue  # :param bool    `newValue`:
    def setSettingsDisplayRenderHintNonCosmetic(self, newValue): self.settings_display_renderhint_noncosmetic = newValue  # :param bool    `newValue`:
    def setSettingsDisplayShowScrollBars(self, newValue):        self.settings_display_show_scrollbars        = newValue  # :param bool    `newValue`:
    def setSettingsDisplayScrollBarWidgetNum(self, newValue):    self.settings_display_scrollbar_widget_num   = newValue  # :param int     `newValue`:
    def setSettingsDisplayCrossHairColor(self, newValue):        self.settings_display_crosshair_color        = newValue  # :param QRgb    `newValue`:
    def setSettingsDisplayBGColor(self, newValue):               self.settings_display_bg_color               = newValue  # :param QRgb    `newValue`:
    def setSettingsDisplaySelectBoxLeftColor(self, newValue):    self.settings_display_selectbox_left_color   = newValue  # :param QRgb    `newValue`:
    def setSettingsDisplaySelectBoxLeftFill(self, newValue):     self.settings_display_selectbox_left_fill    = newValue  # :param QRgb    `newValue`:
    def setSettingsDisplaySelectBoxRightColor(self, newValue):   self.settings_display_selectbox_right_color  = newValue  # :param QRgb    `newValue`:
    def setSettingsDisplaySelectBoxRightFill(self, newValue):    self.settings_display_selectbox_right_fill   = newValue  # :param QRgb    `newValue`:
    def setSettingsDisplaySelectBoxAlpha(self, newValue):        self.settings_display_selectbox_alpha        = newValue  # :param quint8  `newValue`:
    def setSettingsDisplayZoomScaleIn(self, newValue):           self.settings_display_zoomscale_in           = newValue  # :param qreal   `newValue`:
    def setSettingsDisplayZoomScaleOut(self, newValue):          self.settings_display_zoomscale_out          = newValue  # :param qreal   `newValue`:
    def setSettingsDisplayCrossHairPercent(self, newValue):      self.settings_display_crosshair_percent      = newValue  # :param quint8  `newValue`:
    def setSettingsDisplayUnits(self, newValue):                 self.settings_display_units                  = newValue  # :param QString `newValue`:
    def setSettingsPromptTextColor(self, newValue):              self.settings_prompt_text_color              = newValue  # :param QRgb    `newValue`:
    def setSettingsPromptBGColor(self, newValue):                self.settings_prompt_bg_color                = newValue  # :param QRgb    `newValue`:
    def setSettingsPromptFontFamily(self, newValue):             self.settings_prompt_font_family             = newValue  # :param QString `newValue`:
    def setSettingsPromptFontStyle(self, newValue):              self.settings_prompt_font_style              = newValue  # :param QString `newValue`:
    def setSettingsPromptFontSize(self, newValue):               self.settings_prompt_font_size               = newValue  # :param quint8  `newValue`:
    def setSettingsPromptSaveHistory(self, newValue):            self.settings_prompt_save_history            = newValue  # :param bool    `newValue`:
    def setSettingsPromptSaveHistoryAsHtml(self, newValue):      self.settings_prompt_save_history_as_html    = newValue  # :param bool    `newValue`:
    def setSettingsPromptSaveHistoryFilename(self, newValue):    self.settings_prompt_save_history_filename   = newValue  # :param QString `newValue`:
    def setSettingsCustomFilter(self, newValue):                 self.settings_opensave_custom_filter         = newValue  # :param QString `newValue`:
    def setSettingsOpenFormat(self, newValue):                   self.settings_opensave_open_format           = newValue  # :param QString `newValue`:
    def setSettingsOpenThumbnail(self, newValue):                self.settings_opensave_open_thumbnail        = newValue  # :param bool    `newValue`:
    def setSettingsSaveFormat(self, newValue):                   self.settings_opensave_save_format           = newValue  # :param QString `newValue`:
    def setSettingsSaveThumbnail(self, newValue):                self.settings_opensave_save_thumbnail        = newValue  # :param bool    `newValue`:
    def setSettingsRecentMaxFiles(self, newValue):               self.settings_opensave_recent_max_files      = newValue  # :param quint8  `newValue`:
    def setSettingsOpenSaveTrimDstNumJumps(self, newValue):      self.settings_opensave_trim_dst_num_jumps    = newValue  # :param quint8  `newValue`:
    def setSettingsPrintingDefaultDevice(self, newValue):        self.settings_printing_default_device        = newValue  # :param QString `newValue`:
    def setSettingsPrintingUseLastDevice(self, newValue):        self.settings_printing_use_last_device       = newValue  # :param bool    `newValue`:
    def setSettingsPrintingDisableBG(self, newValue):            self.settings_printing_disable_bg            = newValue  # :param bool    `newValue`:
    def setSettingsGridShowOnLoad(self, newValue):               self.settings_grid_show_on_load              = newValue  # :param bool    `newValue`:
    def setSettingsGridShowOrigin(self, newValue):               self.settings_grid_show_origin               = newValue  # :param bool    `newValue`:
    def setSettingsGridColorMatchCrossHair(self, newValue):      self.settings_grid_color_match_crosshair     = newValue  # :param bool    `newValue`:
    def setSettingsGridColor(self, newValue):                    self.settings_grid_color                     = newValue  # :param QRgb    `newValue`:
    def setSettingsGridLoadFromFile(self, newValue):             self.settings_grid_load_from_file            = newValue  # :param bool    `newValue`:
    def setSettingsGridType(self, newValue):                     self.settings_grid_type                      = newValue  # :param QString `newValue`:
    def setSettingsGridCenterOnOrigin(self, newValue):           self.settings_grid_center_on_origin          = newValue  # :param bool    `newValue`:
    def setSettingsGridCenterX(self, newValue):                  self.settings_grid_center_x                  = newValue  # :param qreal   `newValue`:
    def setSettingsGridCenterY(self, newValue):                  self.settings_grid_center_y                  = newValue  # :param qreal   `newValue`:
    def setSettingsGridSizeX(self, newValue):                    self.settings_grid_size_x                    = newValue  # :param qreal   `newValue`:
    def setSettingsGridSizeY(self, newValue):                    self.settings_grid_size_y                    = newValue  # :param qreal   `newValue`:
    def setSettingsGridSpacingX(self, newValue):                 self.settings_grid_spacing_x                 = newValue  # :param qreal   `newValue`:
    def setSettingsGridSpacingY(self, newValue):                 self.settings_grid_spacing_y                 = newValue  # :param qreal   `newValue`:
    def setSettingsGridSizeRadius(self, newValue):               self.settings_grid_size_radius               = newValue  # :param qreal   `newValue`:
    def setSettingsGridSpacingRadius(self, newValue):            self.settings_grid_spacing_radius            = newValue  # :param qreal   `newValue`:
    def setSettingsGridSpacingAngle(self, newValue):             self.settings_grid_spacing_angle             = newValue  # :param qreal   `newValue`:
    def setSettingsRulerShowOnLoad(self, newValue):              self.settings_ruler_show_on_load             = newValue  # :param bool    `newValue`:
    def setSettingsRulerMetric(self, newValue):                  self.settings_ruler_metric                   = newValue  # :param bool    `newValue`:
    def setSettingsRulerColor(self, newValue):                   self.settings_ruler_color                    = newValue  # :param QRgb    `newValue`:
    def setSettingsRulerPixelSize(self, newValue):               self.settings_ruler_pixel_size               = newValue  # :param quint8  `newValue`:
    def setSettingsQSnapEnabled(self, newValue):                 self.settings_qsnap_enabled                  = newValue  # :param bool    `newValue`:
    def setSettingsQSnapLocatorColor(self, newValue):            self.settings_qsnap_locator_color            = newValue  # :param QRgb    `newValue`:
    def setSettingsQSnapLocatorSize(self, newValue):             self.settings_qsnap_locator_size             = newValue  # :param quint8  `newValue`:
    def setSettingsQSnapApertureSize(self, newValue):            self.settings_qsnap_aperture_size            = newValue  # :param quint8  `newValue`:
    def setSettingsQSnapEndPoint(self, newValue):                self.settings_qsnap_endpoint                 = newValue  # :param bool    `newValue`:
    def setSettingsQSnapMidPoint(self, newValue):                self.settings_qsnap_midpoint                 = newValue  # :param bool    `newValue`:
    def setSettingsQSnapCenter(self, newValue):                  self.settings_qsnap_center                   = newValue  # :param bool    `newValue`:
    def setSettingsQSnapNode(self, newValue):                    self.settings_qsnap_node                     = newValue  # :param bool    `newValue`:
    def setSettingsQSnapQuadrant(self, newValue):                self.settings_qsnap_quadrant                 = newValue  # :param bool    `newValue`:
    def setSettingsQSnapIntersection(self, newValue):            self.settings_qsnap_intersection             = newValue  # :param bool    `newValue`:
    def setSettingsQSnapExtension(self, newValue):               self.settings_qsnap_extension                = newValue  # :param bool    `newValue`:
    def setSettingsQSnapInsertion(self, newValue):               self.settings_qsnap_insertion                = newValue  # :param bool    `newValue`:
    def setSettingsQSnapPerpendicular(self, newValue):           self.settings_qsnap_perpendicular            = newValue  # :param bool    `newValue`:
    def setSettingsQSnapTangent(self, newValue):                 self.settings_qsnap_tangent                  = newValue  # :param bool    `newValue`:
    def setSettingsQSnapNearest(self, newValue):                 self.settings_qsnap_nearest                  = newValue  # :param bool    `newValue`:
    def setSettingsQSnapApparent(self, newValue):                self.settings_qsnap_apparent                 = newValue  # :param bool    `newValue`:
    def setSettingsQSnapParallel(self, newValue):                self.settings_qsnap_parallel                 = newValue  # :param bool    `newValue`:
    def setSettingsLwtShowLwt(self, newValue):                   self.settings_lwt_show_lwt                   = newValue  # :param bool    `newValue`:
    def setSettingsLwtRealRender(self, newValue):                self.settings_lwt_real_render                = newValue  # :param bool    `newValue`:
    def setSettingsLwtDefaultLwt(self, newValue):                self.settings_lwt_default_lwt                = newValue  # :param qreal   `newValue`:
    def setSettingsSelectionModePickFirst(self, newValue):       self.settings_selection_mode_pickfirst       = newValue  # :param bool    `newValue`:
    def setSettingsSelectionModePickAdd(self, newValue):         self.settings_selection_mode_pickadd         = newValue  # :param bool    `newValue`:
    def setSettingsSelectionModePickDrag(self, newValue):        self.settings_selection_mode_pickdrag        = newValue  # :param bool    `newValue`:
    def setSettingsSelectionCoolGripColor(self, newValue):       self.settings_selection_coolgrip_color       = newValue  # :param QRgb    `newValue`:
    def setSettingsSelectionHotGripColor(self, newValue):        self.settings_selection_hotgrip_color        = newValue  # :param QRgb    `newValue`:
    def setSettingsSelectionGripSize(self, newValue):            self.settings_selection_grip_size            = newValue  # :param quint8  `newValue`:
    def setSettingsSelectionPickBoxSize(self, newValue):         self.settings_selection_pickbox_size         = newValue  # :param quint8  `newValue`:
    def setSettingsTextFont(self, newValue):                     self.settings_text_font                      = newValue  # :param QString `newValue`:
    def setSettingsTextSize(self, newValue):                     self.settings_text_size                      = newValue  # :param qreal   `newValue`:
    def setSettingsTextAngle(self, newValue):                    self.settings_text_angle                     = newValue  # :param qreal   `newValue`:
    def setSettingsTextStyleBold(self, newValue):                self.settings_text_style_bold                = newValue  # :param bool    `newValue`:
    def setSettingsTextStyleItalic(self, newValue):              self.settings_text_style_italic              = newValue  # :param bool    `newValue`:
    def setSettingsTextStyleUnderline(self, newValue):           self.settings_text_style_underline           = newValue  # :param bool    `newValue`:
    def setSettingsTextStyleStrikeOut(self, newValue):           self.settings_text_style_strikeout           = newValue  # :param bool    `newValue`:
    def setSettingsTextStyleOverline(self, newValue):            self.settings_text_style_overline            = newValue  # :param bool    `newValue`:


    ##########################################################################
    # mainwindow_settings.py
    ##########################################################################

    def readSettings(self): # void MainWindow::
        """"""
        qDebug("Reading Settings...")
        theSettingsIniPath = self.gAppDir + os.sep + "settings-py.ini"

        # self.gSettings = gSettings = QSettings(theSettingsIniPath, QSettings.IniFormat)
        # pos = gSettings.value("Window/Position", QPoint(0, 0))#.toPoint()
        # size = gSettings.value("Window/Size", QSize(800, 600))#.toSize()

        # self.layoutState = gSettings.value("LayoutState")#.toByteArray()
        # from array import array
        # self.layoutState = array("B", gSettings.get("General", "LayoutState")) #.toByteArray()
        # if not self.layoutState: # None
        #     qDebug("LayoutState NOT restored! Setting Default Layout...")
        #     # someToolBar.setVisible(True)
        # else:
        #     self.restoreState(self.layoutState)

        ## # PySide has this bad habit *sometimes* of all settings being strings on reloading.
        ## # This is a pain in the ass to not have a clear way to retrieve values as they where
        ## # upon saving.
        ## # Maybe should just switch to pythons built in configparser module for ini.
        ## # At least it has config.getboolean(section, option)
        ## isBoolStr = ('true', 'True')
        ## # General
        ## self.settings_general_language               =       gSettings.value("Language",                            "default")               #.toString()
        ## self.settings_general_icon_theme             =       gSettings.value("IconTheme",                           "default")               #.toString()
        ## self.settings_general_icon_size              =   int(gSettings.value("IconSize",                                   16))              #.toInt()
        ## self.settings_general_mdi_bg_use_logo        =       gSettings.value("MdiBGUseLogo",                             True) in isBoolStr  #.toBool()
        ## self.settings_general_mdi_bg_use_texture     =       gSettings.value("MdiBGUseTexture",                          True) in isBoolStr  #.toBool()
        ## self.settings_general_mdi_bg_use_color       =       gSettings.value("MdiBGUseColor",                            True) in isBoolStr  #.toBool()
        ## self.settings_general_mdi_bg_logo            =       gSettings.value("MdiBGLogo",           "images/logo-spirals.png")               #.toString()
        ## self.settings_general_mdi_bg_texture         =       gSettings.value("MdiBGTexture",     "images/texture-spirals.png")               #.toString()
        ## self.settings_general_mdi_bg_color           =   int(gSettings.value("MdiBGColor",                  qRgb(192,192,192)))              #.toInt()
        ## self.settings_general_tip_of_the_day         =       gSettings.value("TipOfTheDay",                                 1) in isBoolStr  #.toBool()
        ## self.settings_general_current_tip            =   int(gSettings.value("CurrentTip",                                  0))              #.toInt()
        ## # Display
        ## self.settings_display_use_opengl             =       gSettings.value("Display/UseOpenGL",                       False) in isBoolStr  #.toBool()
        ## self.settings_display_renderhint_aa          =       gSettings.value("Display/RenderHintAntiAlias",             False) in isBoolStr  #.toBool()
        ## self.settings_display_renderhint_text_aa     =       gSettings.value("Display/RenderHintTextAntiAlias",         False) in isBoolStr  #.toBool()
        ## self.settings_display_renderhint_smooth_pix  =       gSettings.value("Display/RenderHintSmoothPixmap",          False) in isBoolStr  #.toBool()
        ## self.settings_display_renderhint_high_aa     =       gSettings.value("Display/RenderHintHighQualityAntiAlias",  False) in isBoolStr  #.toBool()
        ## self.settings_display_renderhint_noncosmetic =       gSettings.value("Display/RenderHintNonCosmetic",           False) in isBoolStr  #.toBool()
        ## self.settings_display_show_scrollbars        =       gSettings.value("Display/ShowScrollBars",                   True) in isBoolStr  #.toBool()
        ## self.settings_display_scrollbar_widget_num   =   int(gSettings.value("Display/ScrollBarWidgetNum",                  0))              #.toInt()
        ## self.settings_display_crosshair_color        =   int(gSettings.value("Display/CrossHairColor",      qRgb(  0,  0,  0)))              #.toInt()
        ## self.settings_display_bg_color               =   int(gSettings.value("Display/BackgroundColor",     qRgb(235,235,235)))              #.toInt()
        ## self.settings_display_selectbox_left_color   =   int(gSettings.value("Display/SelectBoxLeftColor",  qRgb(  0,128,  0)))              #.toInt()
        ## self.settings_display_selectbox_left_fill    =   int(gSettings.value("Display/SelectBoxLeftFill",   qRgb(  0,255,  0)))              #.toInt()
        ## self.settings_display_selectbox_right_color  =   int(gSettings.value("Display/SelectBoxRightColor", qRgb(  0,  0,128)))              #.toInt()
        ## self.settings_display_selectbox_right_fill   =   int(gSettings.value("Display/SelectBoxRightFill",  qRgb(  0,  0,255)))              #.toInt()
        ## self.settings_display_selectbox_alpha        =   int(gSettings.value("Display/SelectBoxAlpha",                     32))              #.toInt()
        ## self.settings_display_zoomscale_in           = float(gSettings.value("Display/ZoomScaleIn",                       2.0))              #.toFloat()
        ## self.settings_display_zoomscale_out          = float(gSettings.value("Display/ZoomScaleOut",                      0.5))              #.toFloat()
        ## self.settings_display_crosshair_percent      =   int(gSettings.value("Display/CrossHairPercent",                    5))              #.toInt()
        ## self.settings_display_units                  =       gSettings.value("Display/Units",                            "mm")               #.toString()
        ## # Prompt
        ## self.settings_prompt_text_color              =   int(gSettings.value("Prompt/TextColor",            qRgb(  0,  0,  0)))              #.toInt()
        ## self.settings_prompt_bg_color                =   int(gSettings.value("Prompt/BackgroundColor",      qRgb(255,255,255)))              #.toInt()
        ## self.settings_prompt_font_family             =       gSettings.value("Prompt/FontFamily",                 "Monospace")               #.toString()
        ## self.settings_prompt_font_style              =       gSettings.value("Prompt/FontStyle",                     "normal")               #.toString()
        ## self.settings_prompt_font_size               =   int(gSettings.value("Prompt/FontSize",                            12))              #.toInt()
        ## self.settings_prompt_save_history            =       gSettings.value("Prompt/SaveHistory",                       True) in isBoolStr  #.toBool()
        ## self.settings_prompt_save_history_as_html    =       gSettings.value("Prompt/SaveHistoryAsHtml",                False) in isBoolStr  #.toBool()
        ## self.settings_prompt_save_history_filename   =       gSettings.value("Prompt/SaveHistoryFilename",       "prompt.log")               #.toString()
        ## # OpenSave
        ## self.settings_opensave_custom_filter         =       gSettings.value("OpenSave/CustomFilter",             "supported")               #.toString()
        ## self.settings_opensave_open_format           =       gSettings.value("OpenSave/OpenFormat",                     "*.*")               #.toString()
        ## self.settings_opensave_open_thumbnail        =       gSettings.value("OpenSave/OpenThumbnail",                  False) in isBoolStr  #.toBool()
        ## self.settings_opensave_save_format           =       gSettings.value("OpenSave/SaveFormat",                     "*.*")               #.toString()
        ## self.settings_opensave_save_thumbnail        =       gSettings.value("OpenSave/SaveThumbnail",                  False) in isBoolStr  #.toBool()
        ## # Recent
        ## self.settings_opensave_recent_max_files      =   int(gSettings.value("OpenSave/RecentMax",                         10))              #.toInt()
        ## self.settings_opensave_recent_list_of_files  =       gSettings.value("OpenSave/RecentFiles")                                         #.toStringList()
        ## self.settings_opensave_recent_directory      =       gSettings.value("OpenSave/RecentDirectory")                                     #.toString()
        ## # Trimming
        ## self.settings_opensave_trim_dst_num_jumps    =   int(gSettings.value("OpenSave/TrimDstNumJumps",                    5))              #.toInt()
        ## # Printing
        ## self.settings_printing_default_device        =       gSettings.value("Printing/DefaultDevice",                     "")               #.toString()
        ## self.settings_printing_use_last_device       =       gSettings.value("Printing/UseLastDevice",                  False) in isBoolStr  #.toBool()
        ## self.settings_printing_disable_bg            =       gSettings.value("Printing/DisableBG",                       True) in isBoolStr  #.toBool()
        ## # Grid
        ## self.settings_grid_show_on_load              =       gSettings.value("Grid/ShowOnLoad",                          True) in isBoolStr  #.toBool()
        ## self.settings_grid_show_origin               =       gSettings.value("Grid/ShowOrigin",                          True) in isBoolStr  #.toBool()
        ## self.settings_grid_color_match_crosshair     =       gSettings.value("Grid/ColorMatchCrossHair",                 True) in isBoolStr  #.toBool()
        ## self.settings_grid_color                     =   int(gSettings.value("Grid/Color",                  qRgb(  0,  0,  0)))              #.toInt()
        ## self.settings_grid_load_from_file            =       gSettings.value("Grid/LoadFromFile",                        True) in isBoolStr  #.toBool()
        ## self.settings_grid_type                      =       gSettings.value("Grid/Type",                       "Rectangular")               #.toString()
        ## self.settings_grid_center_on_origin          =       gSettings.value("Grid/CenterOnOrigin",                      True) in isBoolStr  #.toBool()
        ## self.settings_grid_center_x                  = float(gSettings.value("Grid/CenterX",                              0.0))              #.toFloat()
        ## self.settings_grid_center_y                  = float(gSettings.value("Grid/CenterY",                              0.0))              #.toFloat()
        ## self.settings_grid_size_x                    = float(gSettings.value("Grid/SizeX",                              100.0))              #.toFloat()
        ## self.settings_grid_size_y                    = float(gSettings.value("Grid/SizeY",                              100.0))              #.toFloat()
        ## self.settings_grid_spacing_x                 = float(gSettings.value("Grid/SpacingX",                            25.0))              #.toFloat()
        ## self.settings_grid_spacing_y                 = float(gSettings.value("Grid/SpacingY",                            25.0))              #.toFloat()
        ## self.settings_grid_size_radius               = float(gSettings.value("Grid/SizeRadius",                          50.0))              #.toFloat()
        ## self.settings_grid_spacing_radius            = float(gSettings.value("Grid/SpacingRadius",                       25.0))              #.toFloat()
        ## self.settings_grid_spacing_angle             = float(gSettings.value("Grid/SpacingAngle",                        45.0))              #.toFloat()
        ## # Ruler
        ## self.settings_ruler_show_on_load             =       gSettings.value("Ruler/ShowOnLoad",                         True) in isBoolStr  #.toBool()
        ## self.settings_ruler_metric                   =       gSettings.value("Ruler/Metric",                             True) in isBoolStr  #.toBool()
        ## self.settings_ruler_color                    =   int(gSettings.value("Ruler/Color",                 qRgb(210,210, 50)))              #.toInt()
        ## self.settings_ruler_pixel_size               =   int(gSettings.value("Ruler/PixelSize",                            20))              #.toInt()
        ## # Quick Snap
        ## self.settings_qsnap_enabled                  =       gSettings.value("QuickSnap/Enabled",                        True) in isBoolStr  #.toBool()
        ## self.settings_qsnap_locator_color            =   int(gSettings.value("QuickSnap/LocatorColor",      qRgb(255,255,  0)))              #.toInt()
        ## self.settings_qsnap_locator_size             =   int(gSettings.value("QuickSnap/LocatorSize",                       4))              #.toInt()
        ## self.settings_qsnap_aperture_size            =   int(gSettings.value("QuickSnap/ApertureSize",                     10))              #.toInt()
        ## self.settings_qsnap_endpoint                 =       gSettings.value("QuickSnap/EndPoint",                       True) in isBoolStr  #.toBool()
        ## self.settings_qsnap_midpoint                 =       gSettings.value("QuickSnap/MidPoint",                       True) in isBoolStr  #.toBool()
        ## self.settings_qsnap_center                   =       gSettings.value("QuickSnap/Center",                         True) in isBoolStr  #.toBool()
        ## self.settings_qsnap_node                     =       gSettings.value("QuickSnap/Node",                           True) in isBoolStr  #.toBool()
        ## self.settings_qsnap_quadrant                 =       gSettings.value("QuickSnap/Quadrant",                       True) in isBoolStr  #.toBool()
        ## self.settings_qsnap_intersection             =       gSettings.value("QuickSnap/Intersection",                   True) in isBoolStr  #.toBool()
        ## self.settings_qsnap_extension                =       gSettings.value("QuickSnap/Extension",                      True) in isBoolStr  #.toBool()
        ## self.settings_qsnap_insertion                =       gSettings.value("QuickSnap/Insertion",                     False) in isBoolStr  #.toBool()
        ## self.settings_qsnap_perpendicular            =       gSettings.value("QuickSnap/Perpendicular",                  True) in isBoolStr  #.toBool()
        ## self.settings_qsnap_tangent                  =       gSettings.value("QuickSnap/Tangent",                        True) in isBoolStr  #.toBool()
        ## self.settings_qsnap_nearest                  =       gSettings.value("QuickSnap/Nearest",                       False) in isBoolStr  #.toBool()
        ## self.settings_qsnap_apparent                 =       gSettings.value("QuickSnap/Apparent",                      False) in isBoolStr  #.toBool()
        ## self.settings_qsnap_parallel                 =       gSettings.value("QuickSnap/Parallel",                      False) in isBoolStr  #.toBool()
        ## # LineWeight
        ## self.settings_lwt_show_lwt                   =       gSettings.value("LineWeight/ShowLineWeight",               False) in isBoolStr  #.toBool()
        ## self.settings_lwt_real_render                =       gSettings.value("LineWeight/RealRender",                    True) in isBoolStr  #.toBool()
        ## self.settings_lwt_default_lwt                = float(gSettings.value("LineWeight/DefaultLineWeight",                0))              #.toReal()
        ## # Selection
        ## self.settings_selection_mode_pickfirst       =       gSettings.value("Selection/PickFirst",                      True) in isBoolStr  #.toBool()
        ## self.settings_selection_mode_pickadd         =       gSettings.value("Selection/PickAdd",                        True) in isBoolStr  #.toBool()
        ## self.settings_selection_mode_pickdrag        =       gSettings.value("Selection/PickDrag",                      False) in isBoolStr  #.toBool()
        ## self.settings_selection_coolgrip_color       =   int(gSettings.value("Selection/CoolGripColor",     qRgb(  0,  0,255)))              #.toInt()
        ## self.settings_selection_hotgrip_color        =   int(gSettings.value("Selection/HotGripColor",      qRgb(255,  0,  0)))              #.toInt()
        ## self.settings_selection_grip_size            =   int(gSettings.value("Selection/GripSize",                          4))              #.toInt()
        ## self.settings_selection_pickbox_size         =   int(gSettings.value("Selection/PickBoxSize",                       4))              #.toInt()
        ## # Text
        ## self.settings_text_font                      =       gSettings.value("Text/Font",                             "Arial")               #.toString()
        ## self.settings_text_size                      = float(gSettings.value("Text/Size",                                  12))              #.toReal()
        ## self.settings_text_angle                     = float(gSettings.value("Text/Angle",                                  0))              #.toReal()
        ## self.settings_text_style_bold                =       gSettings.value("Text/StyleBold",                          False) in isBoolStr  #.toBool()
        ## self.settings_text_style_italic              =       gSettings.value("Text/StyleItalic",                        False) in isBoolStr  #.toBool()
        ## self.settings_text_style_underline           =       gSettings.value("Text/StyleUnderline",                     False) in isBoolStr  #.toBool()
        ## self.settings_text_style_strikeout           =       gSettings.value("Text/StyleStrikeOut",                     False) in isBoolStr  #.toBool()
        ## self.settings_text_style_overline            =       gSettings.value("Text/StyleOverline",                      False) in isBoolStr  #.toBool()

        ############

        ## # General
        ## self.settings_general_language               = gSettings.value("Language",                            "default")#.toString()
        ## self.settings_general_icon_theme             = gSettings.value("IconTheme",                           "default")#.toString()
        ## self.settings_general_icon_size              = gSettings.value("IconSize",                                   16)#.toInt()
        ## self.settings_general_mdi_bg_use_logo        = gSettings.value("MdiBGUseLogo",                             True)#.toBool()
        ## self.settings_general_mdi_bg_use_texture     = gSettings.value("MdiBGUseTexture",                          True)#.toBool()
        ## self.settings_general_mdi_bg_use_color       = gSettings.value("MdiBGUseColor",                            True)#.toBool()
        ## self.settings_general_mdi_bg_logo            = gSettings.value("MdiBGLogo",           "images/logo-spirals.png")#.toString()
        ## self.settings_general_mdi_bg_texture         = gSettings.value("MdiBGTexture",     "images/texture-spirals.png")#.toString()
        ## self.settings_general_mdi_bg_color           = gSettings.value("MdiBGColor",                  qRgb(192,192,192))#.toInt()
        ## self.settings_general_tip_of_the_day         = gSettings.value("TipOfTheDay",                                 1)#.toBool()
        ## self.settings_general_current_tip            = gSettings.value("CurrentTip",                                  0)#.toInt()
        ## # Display
        ## self.settings_display_use_opengl             = gSettings.value("Display/UseOpenGL",                       False)#.toBool()
        ## self.settings_display_renderhint_aa          = gSettings.value("Display/RenderHintAntiAlias",             False)#.toBool()
        ## self.settings_display_renderhint_text_aa     = gSettings.value("Display/RenderHintTextAntiAlias",         False)#.toBool()
        ## self.settings_display_renderhint_smooth_pix  = gSettings.value("Display/RenderHintSmoothPixmap",          False)#.toBool()
        ## self.settings_display_renderhint_high_aa     = gSettings.value("Display/RenderHintHighQualityAntiAlias",  False)#.toBool()
        ## self.settings_display_renderhint_noncosmetic = gSettings.value("Display/RenderHintNonCosmetic",           False)#.toBool()
        ## self.settings_display_show_scrollbars        = gSettings.value("Display/ShowScrollBars",                   True)#.toBool()
        ## self.settings_display_scrollbar_widget_num   = gSettings.value("Display/ScrollBarWidgetNum",                  0)#.toInt()
        ## self.settings_display_crosshair_color        = gSettings.value("Display/CrossHairColor",      qRgb(  0,  0,  0))#.toInt()
        ## self.settings_display_bg_color               = gSettings.value("Display/BackgroundColor",     qRgb(235,235,235))#.toInt()
        ## self.settings_display_selectbox_left_color   = gSettings.value("Display/SelectBoxLeftColor",  qRgb(  0,128,  0))#.toInt()
        ## self.settings_display_selectbox_left_fill    = gSettings.value("Display/SelectBoxLeftFill",   qRgb(  0,255,  0))#.toInt()
        ## self.settings_display_selectbox_right_color  = gSettings.value("Display/SelectBoxRightColor", qRgb(  0,  0,128))#.toInt()
        ## self.settings_display_selectbox_right_fill   = gSettings.value("Display/SelectBoxRightFill",  qRgb(  0,  0,255))#.toInt()
        ## self.settings_display_selectbox_alpha        = gSettings.value("Display/SelectBoxAlpha",                     32)#.toInt()
        ## self.settings_display_zoomscale_in           = gSettings.value("Display/ZoomScaleIn",                       2.0)#.toFloat()
        ## self.settings_display_zoomscale_out          = gSettings.value("Display/ZoomScaleOut",                      0.5)#.toFloat()
        ## self.settings_display_crosshair_percent      = gSettings.value("Display/CrossHairPercent",                    5)#.toInt()
        ## self.settings_display_units                  = gSettings.value("Display/Units",                            "mm")#.toString()
        ## # Prompt
        ## self.settings_prompt_text_color              = gSettings.value("Prompt/TextColor",            qRgb(  0,  0,  0))#.toInt()
        ## self.settings_prompt_bg_color                = gSettings.value("Prompt/BackgroundColor",      qRgb(255,255,255))#.toInt()
        ## self.settings_prompt_font_family             = gSettings.value("Prompt/FontFamily",                 "Monospace")#.toString()
        ## self.settings_prompt_font_style              = gSettings.value("Prompt/FontStyle",                     "normal")#.toString()
        ## self.settings_prompt_font_size               = gSettings.value("Prompt/FontSize",                            12)#.toInt()
        ## self.settings_prompt_save_history            = gSettings.value("Prompt/SaveHistory",                       True)#.toBool()
        ## self.settings_prompt_save_history_as_html    = gSettings.value("Prompt/SaveHistoryAsHtml",                False)#.toBool()
        ## self.settings_prompt_save_history_filename   = gSettings.value("Prompt/SaveHistoryFilename",       "prompt.log")#.toString()
        ## # OpenSave
        ## self.settings_opensave_custom_filter         = gSettings.value("OpenSave/CustomFilter",             "supported")#.toString()
        ## self.settings_opensave_open_format           = gSettings.value("OpenSave/OpenFormat",                     "*.*")#.toString()
        ## self.settings_opensave_open_thumbnail        = gSettings.value("OpenSave/OpenThumbnail",                  False)#.toBool()
        ## self.settings_opensave_save_format           = gSettings.value("OpenSave/SaveFormat",                     "*.*")#.toString()
        ## self.settings_opensave_save_thumbnail        = gSettings.value("OpenSave/SaveThumbnail",                  False)#.toBool()
        ## # Recent
        ## self.settings_opensave_recent_max_files      = gSettings.value("OpenSave/RecentMax",                         10)#.toInt()
        ## self.settings_opensave_recent_list_of_files  = gSettings.value("OpenSave/RecentFiles")                          #.toStringList()
        ## self.settings_opensave_recent_directory      = gSettings.value("OpenSave/RecentDirectory")                      #.toString()
        ## # Trimming
        ## self.settings_opensave_trim_dst_num_jumps    = gSettings.value("OpenSave/TrimDstNumJumps",                    5)#.toInt()
        ## # Printing
        ## self.settings_printing_default_device        = gSettings.value("Printing/DefaultDevice",                     "")#.toString()
        ## self.settings_printing_use_last_device       = gSettings.value("Printing/UseLastDevice",                  False)#.toBool()
        ## self.settings_printing_disable_bg            = gSettings.value("Printing/DisableBG",                       True)#.toBool()
        ## # Grid
        ## self.settings_grid_show_on_load              = gSettings.value("Grid/ShowOnLoad",                          True)#.toBool()
        ## self.settings_grid_show_origin               = gSettings.value("Grid/ShowOrigin",                          True)#.toBool()
        ## self.settings_grid_color_match_crosshair     = gSettings.value("Grid/ColorMatchCrossHair",                 True)#.toBool()
        ## self.settings_grid_color                     = gSettings.value("Grid/Color",                  qRgb(  0,  0,  0))#.toInt()
        ## self.settings_grid_load_from_file            = gSettings.value("Grid/LoadFromFile",                        True)#.toBool()
        ## self.settings_grid_type                      = gSettings.value("Grid/Type",                       "Rectangular")#.toString()
        ## self.settings_grid_center_on_origin          = gSettings.value("Grid/CenterOnOrigin",                      True)#.toBool()
        ## self.settings_grid_center_x                  = gSettings.value("Grid/CenterX",                              0.0)#.toFloat()
        ## self.settings_grid_center_y                  = gSettings.value("Grid/CenterY",                              0.0)#.toFloat()
        ## self.settings_grid_size_x                    = gSettings.value("Grid/SizeX",                              100.0)#.toFloat()
        ## self.settings_grid_size_y                    = gSettings.value("Grid/SizeY",                              100.0)#.toFloat()
        ## self.settings_grid_spacing_x                 = gSettings.value("Grid/SpacingX",                            25.0)#.toFloat()
        ## self.settings_grid_spacing_y                 = gSettings.value("Grid/SpacingY",                            25.0)#.toFloat()
        ## self.settings_grid_size_radius               = gSettings.value("Grid/SizeRadius",                          50.0)#.toFloat()
        ## self.settings_grid_spacing_radius            = gSettings.value("Grid/SpacingRadius",                       25.0)#.toFloat()
        ## self.settings_grid_spacing_angle             = gSettings.value("Grid/SpacingAngle",                        45.0)#.toFloat()
        ## # Ruler
        ## self.settings_ruler_show_on_load             = gSettings.value("Ruler/ShowOnLoad",                         True)#.toBool()
        ## self.settings_ruler_metric                   = gSettings.value("Ruler/Metric",                             True)#.toBool()
        ## self.settings_ruler_color                    = gSettings.value("Ruler/Color",                 qRgb(210,210, 50))#.toInt()
        ## self.settings_ruler_pixel_size               = gSettings.value("Ruler/PixelSize",                            20)#.toInt()
        ## # Quick Snap
        ## self.settings_qsnap_enabled                  = gSettings.value("QuickSnap/Enabled",                        True)#.toBool()
        ## self.settings_qsnap_locator_color            = gSettings.value("QuickSnap/LocatorColor",      qRgb(255,255,  0))#.toInt()
        ## self.settings_qsnap_locator_size             = gSettings.value("QuickSnap/LocatorSize",                       4)#.toInt()
        ## self.settings_qsnap_aperture_size            = gSettings.value("QuickSnap/ApertureSize",                     10)#.toInt()
        ## self.settings_qsnap_endpoint                 = gSettings.value("QuickSnap/EndPoint",                       True)#.toBool()
        ## self.settings_qsnap_midpoint                 = gSettings.value("QuickSnap/MidPoint",                       True)#.toBool()
        ## self.settings_qsnap_center                   = gSettings.value("QuickSnap/Center",                         True)#.toBool()
        ## self.settings_qsnap_node                     = gSettings.value("QuickSnap/Node",                           True)#.toBool()
        ## self.settings_qsnap_quadrant                 = gSettings.value("QuickSnap/Quadrant",                       True)#.toBool()
        ## self.settings_qsnap_intersection             = gSettings.value("QuickSnap/Intersection",                   True)#.toBool()
        ## self.settings_qsnap_extension                = gSettings.value("QuickSnap/Extension",                      True)#.toBool()
        ## self.settings_qsnap_insertion                = gSettings.value("QuickSnap/Insertion",                     False)#.toBool()
        ## self.settings_qsnap_perpendicular            = gSettings.value("QuickSnap/Perpendicular",                  True)#.toBool()
        ## self.settings_qsnap_tangent                  = gSettings.value("QuickSnap/Tangent",                        True)#.toBool()
        ## self.settings_qsnap_nearest                  = gSettings.value("QuickSnap/Nearest",                       False)#.toBool()
        ## self.settings_qsnap_apparent                 = gSettings.value("QuickSnap/Apparent",                      False)#.toBool()
        ## self.settings_qsnap_parallel                 = gSettings.value("QuickSnap/Parallel",                      False)#.toBool()
        ## # LineWeight
        ## self.settings_lwt_show_lwt                   = gSettings.value("LineWeight/ShowLineWeight",               False)#.toBool()
        ## self.settings_lwt_real_render                = gSettings.value("LineWeight/RealRender",                    True)#.toBool()
        ## self.settings_lwt_default_lwt                = gSettings.value("LineWeight/DefaultLineWeight",                0)#.toReal()
        ## # Selection
        ## self.settings_selection_mode_pickfirst       = gSettings.value("Selection/PickFirst",                      True)#.toBool()
        ## self.settings_selection_mode_pickadd         = gSettings.value("Selection/PickAdd",                        True)#.toBool()
        ## self.settings_selection_mode_pickdrag        = gSettings.value("Selection/PickDrag",                      False)#.toBool()
        ## self.settings_selection_coolgrip_color       = gSettings.value("Selection/CoolGripColor",     qRgb(  0,  0,255))#.toInt()
        ## self.settings_selection_hotgrip_color        = gSettings.value("Selection/HotGripColor",      qRgb(255,  0,  0))#.toInt()
        ## self.settings_selection_grip_size            = gSettings.value("Selection/GripSize",                          4)#.toInt()
        ## self.settings_selection_pickbox_size         = gSettings.value("Selection/PickBoxSize",                       4)#.toInt()
        ## # Text
        ## self.settings_text_font                      = gSettings.value("Text/Font",                             "Arial")#.toString()
        ## self.settings_text_size                      = gSettings.value("Text/Size",                                  12)#.toReal()
        ## self.settings_text_angle                     = gSettings.value("Text/Angle",                                  0)#.toReal()
        ## self.settings_text_style_bold                = gSettings.value("Text/StyleBold",                          False)#.toBool()
        ## self.settings_text_style_italic              = gSettings.value("Text/StyleItalic",                        False)#.toBool()
        ## self.settings_text_style_underline           = gSettings.value("Text/StyleUnderline",                     False)#.toBool()
        ## self.settings_text_style_strikeout           = gSettings.value("Text/StyleStrikeOut",                     False)#.toBool()
        ## self.settings_text_style_overline            = gSettings.value("Text/StyleOverline",                      False)#.toBool()

        ########################


        if not os.path.exists(gAppDir + os.sep + 'settings-py.ini'):  # Default
            qDebug('gSettings-Default')
            self.layoutState = QByteArray()
            pos = QPoint(0, 0)      #.toPoint()
            size = QSize(800, 600)  #.toSize()

            # General
            self.settings_general_language               =                    "default"  #.toString()
            self.settings_general_icon_theme             =                    "default"  #.toString()
            self.settings_general_icon_size              =                           16  #.toInt()
            self.settings_general_mdi_bg_use_logo        =                         True  #.toBool()
            self.settings_general_mdi_bg_use_texture     =                         True  #.toBool()
            self.settings_general_mdi_bg_use_color       =                         True  #.toBool()
            self.settings_general_mdi_bg_logo            =    "images/logo-spirals.png"  #.toString()
            self.settings_general_mdi_bg_texture         = "images/texture-spirals.png"  #.toString()
            self.settings_general_mdi_bg_color           =            qRgb(192,192,192)  #.toInt()
            self.settings_general_tip_of_the_day         =                            1  #.toBool()
            self.settings_general_current_tip            =                            0  #.toInt()
            # Display
            self.settings_display_use_opengl             =             False  #.toBool()
            self.settings_display_renderhint_aa          =             False  #.toBool()
            self.settings_display_renderhint_text_aa     =             False  #.toBool()
            self.settings_display_renderhint_smooth_pix  =             False  #.toBool()
            self.settings_display_renderhint_high_aa     =             False  #.toBool()
            self.settings_display_renderhint_noncosmetic =             False  #.toBool()
            self.settings_display_show_scrollbars        =              True  #.toBool()
            self.settings_display_scrollbar_widget_num   =                 0  #.toInt()
            self.settings_display_crosshair_color        = qRgb(  0,  0,  0)  #.toInt()
            self.settings_display_bg_color               = qRgb(235,235,235)  #.toInt()
            self.settings_display_selectbox_left_color   = qRgb(  0,128,  0)  #.toInt()
            self.settings_display_selectbox_left_fill    = qRgb(  0,255,  0)  #.toInt()
            self.settings_display_selectbox_right_color  = qRgb(  0,  0,128)  #.toInt()
            self.settings_display_selectbox_right_fill   = qRgb(  0,  0,255)  #.toInt()
            self.settings_display_selectbox_alpha        =                32  #.toInt()
            self.settings_display_zoomscale_in           =               2.0  #.toFloat()
            self.settings_display_zoomscale_out          =               0.5  #.toFloat()
            self.settings_display_crosshair_percent      =                 5  #.toInt()
            self.settings_display_units                  =              "mm"  #.toString()
            # Prompt
            self.settings_prompt_text_color              = qRgb(  0,  0,  0)  #.toInt()
            self.settings_prompt_bg_color                = qRgb(255,255,255)  #.toInt()
            self.settings_prompt_font_family             =       "Monospace"  #.toString()
            self.settings_prompt_font_style              =          "normal"  #.toString()
            self.settings_prompt_font_size               =                12  #.toInt()
            self.settings_prompt_save_history            =              True  #.toBool()
            self.settings_prompt_save_history_as_html    =             False  #.toBool()
            self.settings_prompt_save_history_filename   =      "prompt.log"  #.toString()
            # OpenSave
            self.settings_opensave_custom_filter         =       "supported"  #.toString()
            self.settings_opensave_open_format           =             "*.*"  #.toString()
            self.settings_opensave_open_thumbnail        =             False  #.toBool()
            self.settings_opensave_save_format           =             "*.*"  #.toString()
            self.settings_opensave_save_thumbnail        =             False  #.toBool()
            # Recent
            self.settings_opensave_recent_max_files      =  10                #.toInt()
            self.settings_opensave_recent_list_of_files  =  []                #.toStringList()
            self.settings_opensave_recent_directory      =  ""                #.toString()
            # Trimming
            self.settings_opensave_trim_dst_num_jumps    =   5                #.toInt()
            # Printing
            self.settings_printing_default_device        =    ""              #.toString()
            self.settings_printing_use_last_device       = False              #.toBool()
            self.settings_printing_disable_bg            =  True              #.toBool()
            # Grid
            self.settings_grid_show_on_load              =              True  #.toBool()
            self.settings_grid_show_origin               =              True  #.toBool()
            self.settings_grid_color_match_crosshair     =              True  #.toBool()
            self.settings_grid_color                     = qRgb(  0,  0,  0)  #.toInt()
            self.settings_grid_load_from_file            =              True  #.toBool()
            self.settings_grid_type                      =     "Rectangular"  #.toString()
            self.settings_grid_center_on_origin          =              True  #.toBool()
            self.settings_grid_center_x                  =               0.0  #.toFloat()
            self.settings_grid_center_y                  =               0.0  #.toFloat()
            self.settings_grid_size_x                    =             100.0  #.toFloat()
            self.settings_grid_size_y                    =             100.0  #.toFloat()
            self.settings_grid_spacing_x                 =              25.0  #.toFloat()
            self.settings_grid_spacing_y                 =              25.0  #.toFloat()
            self.settings_grid_size_radius               =              50.0  #.toFloat()
            self.settings_grid_spacing_radius            =              25.0  #.toFloat()
            self.settings_grid_spacing_angle             =              45.0  #.toFloat()
            # Ruler
            self.settings_ruler_show_on_load             =              True  #.toBool()
            self.settings_ruler_metric                   =              True  #.toBool()
            self.settings_ruler_color                    = qRgb(210,210, 50)  #.toInt()
            self.settings_ruler_pixel_size               =                20  #.toInt()
            # Quick Snap
            self.settings_qsnap_enabled                  =              True  #.toBool()
            self.settings_qsnap_locator_color            = qRgb(255,255,  0)  #.toInt()
            self.settings_qsnap_locator_size             =                 4  #.toInt()
            self.settings_qsnap_aperture_size            =                10  #.toInt()
            self.settings_qsnap_endpoint                 =              True  #.toBool()
            self.settings_qsnap_midpoint                 =              True  #.toBool()
            self.settings_qsnap_center                   =              True  #.toBool()
            self.settings_qsnap_node                     =              True  #.toBool()
            self.settings_qsnap_quadrant                 =              True  #.toBool()
            self.settings_qsnap_intersection             =              True  #.toBool()
            self.settings_qsnap_extension                =              True  #.toBool()
            self.settings_qsnap_insertion                =             False  #.toBool()
            self.settings_qsnap_perpendicular            =              True  #.toBool()
            self.settings_qsnap_tangent                  =              True  #.toBool()
            self.settings_qsnap_nearest                  =             False  #.toBool()
            self.settings_qsnap_apparent                 =             False  #.toBool()
            self.settings_qsnap_parallel                 =             False  #.toBool()
            # LineWeight
            self.settings_lwt_show_lwt                   = False              #.toBool()
            self.settings_lwt_real_render                =  True              #.toBool()
            self.settings_lwt_default_lwt                =     0              #.toReal()
            # Selection
            self.settings_selection_mode_pickfirst       =              True  #.toBool()
            self.settings_selection_mode_pickadd         =              True  #.toBool()
            self.settings_selection_mode_pickdrag        =             False  #.toBool()
            self.settings_selection_coolgrip_color       = qRgb(  0,  0,255)  #.toInt()
            self.settings_selection_hotgrip_color        = qRgb(255,  0,  0)  #.toInt()
            self.settings_selection_grip_size            =                 4  #.toInt()
            self.settings_selection_pickbox_size         =                 4  #.toInt()
            # Text
            self.settings_text_font                      = "Arial"  #.toString()
            self.settings_text_size                      =      12  #.toReal()
            self.settings_text_angle                     =       0  #.toReal()
            self.settings_text_style_bold                =   False  #.toBool()
            self.settings_text_style_italic              =   False  #.toBool()
            self.settings_text_style_underline           =   False  #.toBool()
            self.settings_text_style_strikeout           =   False  #.toBool()
            self.settings_text_style_overline            =   False  #.toBool()

        else:
            gSettings.read(gAppDir + os.sep + 'settings-py.ini')
            qDebug('gSettings-Saved')

            # from array import array
            # self.layoutState = array("B", gSettings.get("General", "LayoutState")) #.toByteArray()
            self.layoutState = QByteArray(gSettings.get("General", "LayoutState")) #.toByteArray()
            if not self.layoutState: # None
                qDebug("LayoutState NOT restored! Setting Default Layout...")
                # someToolBar.setVisible(True)
            else:
                self.restoreState(self.layoutState)

            try:
                pos = QPoint(gSettings.getint("Window", "PositionX"), gSettings.getint("Window", "PositionY"))              #.toPoint()
                size = QSize(gSettings.getint("Window", "SizeX"), gSettings.getint("Window", "SizeY"))                      #.toSize()
            except (configparser.NoOptionError, ValueError):
                pointStr = gSettings.getint("Window", "Position")
                pos = QPoint(int(pointStr[pointStr.find('('): pointStr.find(' ')]), int(pointStr[pointStr.find(' ') + 1: len(pointStr) - 1]))
                sizeStr = gSettings.getint("Window", "Position")
                size = QSize(int(pointStr[sizeStr.find('('): sizeStr.find(' ')]), int(pointStr[sizeStr.find(' ') + 1: len(sizeStr) - 1]))

            # General
            self.settings_general_language               = gSettings.get("General", "Language")                       #.toString()
            self.settings_general_icon_theme             = gSettings.get("General", "IconTheme")                      #.toString()
            self.settings_general_icon_size              = gSettings.getint("General", "IconSize")                    #.toInt()
            self.settings_general_mdi_bg_use_logo        = gSettings.getboolean("General", "MdiBGUseLogo")            #.toBool()
            self.settings_general_mdi_bg_use_texture     = gSettings.getboolean("General", "MdiBGUseTexture")         #.toBool()
            self.settings_general_mdi_bg_use_color       = gSettings.getboolean("General", "MdiBGUseColor")           #.toBool()
            self.settings_general_mdi_bg_logo            = gSettings.get("General", "MdiBGLogo")                      #.toString()
            self.settings_general_mdi_bg_texture         = gSettings.get("General", "MdiBGTexture")                   #.toString()
            self.settings_general_mdi_bg_color           = gSettings.getint("General", "MdiBGColor")                  #.toInt()
            self.settings_general_tip_of_the_day         = gSettings.getboolean("General", "TipOfTheDay")             #.toBool()
            self.settings_general_current_tip            = gSettings.getint("General", "CurrentTip")                  #.toInt()
            # Display
            self.settings_display_use_opengl             = gSettings.getboolean("Display", "UseOpenGL")               #.toBool()
            self.settings_display_renderhint_aa          = gSettings.getboolean("Display", "RenderHintAntiAlias")     #.toBool()
            self.settings_display_renderhint_text_aa     = gSettings.getboolean("Display", "RenderHintTextAntiAlias") #.toBool()
            self.settings_display_renderhint_smooth_pix  = gSettings.getboolean("Display", "RenderHintSmoothPixmap")  #.toBool()
            self.settings_display_renderhint_high_aa     = gSettings.getboolean("Display", "RenderHintHighQualityAntiAlias")#.toBool()
            self.settings_display_renderhint_noncosmetic = gSettings.getboolean("Display", "RenderHintNonCosmetic")   #.toBool()
            self.settings_display_show_scrollbars        = gSettings.getboolean("Display", "ShowScrollBars")          #.toBool()
            self.settings_display_scrollbar_widget_num   = gSettings.getint("Display", "ScrollBarWidgetNum")          #.toInt()
            self.settings_display_crosshair_color        = gSettings.getint("Display", "CrossHairColor")              #.toInt()
            self.settings_display_bg_color               = gSettings.getint("Display", "BackgroundColor")             #.toInt()
            self.settings_display_selectbox_left_color   = gSettings.getint("Display", "SelectBoxLeftColor")          #.toInt()
            self.settings_display_selectbox_left_fill    = gSettings.getint("Display", "SelectBoxLeftFill")           #.toInt()
            self.settings_display_selectbox_right_color  = gSettings.getint("Display", "SelectBoxRightColor")         #.toInt()
            self.settings_display_selectbox_right_fill   = gSettings.getint("Display", "SelectBoxRightFill")          #.toInt()
            self.settings_display_selectbox_alpha        = gSettings.getint("Display", "SelectBoxAlpha")              #.toInt()
            self.settings_display_zoomscale_in           = gSettings.getfloat("Display", "ZoomScaleIn")               #.toFloat()
            self.settings_display_zoomscale_out          = gSettings.getfloat("Display", "ZoomScaleOut")              #.toFloat()
            self.settings_display_crosshair_percent      = gSettings.getint("Display", "CrossHairPercent")            #.toInt()
            self.settings_display_units                  = gSettings.get("Display", "Units")                          #.toString()
            # Prompt
            self.settings_prompt_text_color              = gSettings.getint("Prompt", "TextColor")                    #.toInt()
            self.settings_prompt_bg_color                = gSettings.getint("Prompt", "BackgroundColor")              #.toInt()
            self.settings_prompt_font_family             = gSettings.get("Prompt", "FontFamily")                      #.toString()
            self.settings_prompt_font_style              = gSettings.get("Prompt", "FontStyle")                       #.toString()
            self.settings_prompt_font_size               = gSettings.getint("Prompt", "FontSize")                     #.toInt()
            self.settings_prompt_save_history            = gSettings.getboolean("Prompt", "SaveHistory")              #.toBool()
            self.settings_prompt_save_history_as_html    = gSettings.getboolean("Prompt", "SaveHistoryAsHtml")        #.toBool()
            self.settings_prompt_save_history_filename   = gSettings.get("Prompt", "SaveHistoryFilename")             #.toString()
            # OpenSave
            self.settings_opensave_custom_filter         = gSettings.get("OpenSave", "CustomFilter")                  #.toString()
            self.settings_opensave_open_format           = gSettings.get("OpenSave", "OpenFormat")                    #.toString()
            self.settings_opensave_open_thumbnail        = gSettings.getboolean("OpenSave", "OpenThumbnail")          #.toBool()
            self.settings_opensave_save_format           = gSettings.get("OpenSave", "SaveFormat")                    #.toString()
            self.settings_opensave_save_thumbnail        = gSettings.getboolean("OpenSave", "SaveThumbnail")          #.toBool()
            # Recent
            self.settings_opensave_recent_max_files      = gSettings.getint("OpenSave", "RecentMax")                  #.toInt()
            self.settings_opensave_recent_list_of_files  = eval(gSettings.get("OpenSave", "RecentFiles"))             #.toStringList()
            self.settings_opensave_recent_directory      = gSettings.get("OpenSave", "RecentDirectory")               #.toString()
            # Trimming
            self.settings_opensave_trim_dst_num_jumps    = gSettings.getint("OpenSave", "TrimDstNumJumps")            #.toInt()
            # Printing
            self.settings_printing_default_device        = gSettings.get("Printing", "DefaultDevice")                 #.toString()
            self.settings_printing_use_last_device       = gSettings.getboolean("Printing", "UseLastDevice")          #.toBool()
            self.settings_printing_disable_bg            = gSettings.getboolean("Printing", "DisableBG")              #.toBool()
            # Grid
            self.settings_grid_show_on_load              = gSettings.getboolean("Grid", "ShowOnLoad")                 #.toBool()
            self.settings_grid_show_origin               = gSettings.getboolean("Grid", "ShowOrigin")                 #.toBool()
            self.settings_grid_color_match_crosshair     = gSettings.getboolean("Grid", "ColorMatchCrossHair")        #.toBool()
            self.settings_grid_color                     = gSettings.getint("Grid", "Color")                          #.toInt()
            self.settings_grid_load_from_file            = gSettings.getboolean("Grid", "LoadFromFile")               #.toBool()
            self.settings_grid_type                      = gSettings.get("Grid", "Type")                              #.toString()
            self.settings_grid_center_on_origin          = gSettings.getboolean("Grid", "CenterOnOrigin")             #.toBool()
            self.settings_grid_center_x                  = gSettings.getfloat("Grid", "CenterX")                      #.toFloat()
            self.settings_grid_center_y                  = gSettings.getfloat("Grid", "CenterY")                      #.toFloat()
            self.settings_grid_size_x                    = gSettings.getfloat("Grid", "SizeX")                        #.toFloat()
            self.settings_grid_size_y                    = gSettings.getfloat("Grid", "SizeY")                        #.toFloat()
            self.settings_grid_spacing_x                 = gSettings.getfloat("Grid", "SpacingX")                     #.toFloat()
            self.settings_grid_spacing_y                 = gSettings.getfloat("Grid", "SpacingY")                     #.toFloat()
            self.settings_grid_size_radius               = gSettings.getfloat("Grid", "SizeRadius")                   #.toFloat()
            self.settings_grid_spacing_radius            = gSettings.getfloat("Grid", "SpacingRadius")                #.toFloat()
            self.settings_grid_spacing_angle             = gSettings.getfloat("Grid", "SpacingAngle")                 #.toFloat()
            # Ruler
            self.settings_ruler_show_on_load             = gSettings.getboolean("Ruler", "ShowOnLoad")                #.toBool()
            self.settings_ruler_metric                   = gSettings.getboolean("Ruler", "Metric")                    #.toBool()
            self.settings_ruler_color                    = gSettings.getint("Ruler", "Color")                         #.toInt()
            self.settings_ruler_pixel_size               = gSettings.getint("Ruler", "PixelSize")                     #.toInt()
            # Quick Snap
            self.settings_qsnap_enabled                  = gSettings.getboolean("QuickSnap", "Enabled")               #.toBool()
            self.settings_qsnap_locator_color            = gSettings.getint("QuickSnap", "LocatorColor")              #.toInt()
            self.settings_qsnap_locator_size             = gSettings.getint("QuickSnap", "LocatorSize")               #.toInt()
            self.settings_qsnap_aperture_size            = gSettings.getint("QuickSnap", "ApertureSize")              #.toInt()
            self.settings_qsnap_endpoint                 = gSettings.getboolean("QuickSnap", "EndPoint")              #.toBool()
            self.settings_qsnap_midpoint                 = gSettings.getboolean("QuickSnap", "MidPoint")              #.toBool()
            self.settings_qsnap_center                   = gSettings.getboolean("QuickSnap", "Center")                #.toBool()
            self.settings_qsnap_node                     = gSettings.getboolean("QuickSnap", "Node")                  #.toBool()
            self.settings_qsnap_quadrant                 = gSettings.getboolean("QuickSnap", "Quadrant")              #.toBool()
            self.settings_qsnap_intersection             = gSettings.getboolean("QuickSnap", "Intersection")          #.toBool()
            self.settings_qsnap_extension                = gSettings.getboolean("QuickSnap", "Extension")             #.toBool()
            self.settings_qsnap_insertion                = gSettings.getboolean("QuickSnap", "Insertion")             #.toBool()
            self.settings_qsnap_perpendicular            = gSettings.getboolean("QuickSnap", "Perpendicular")         #.toBool()
            self.settings_qsnap_tangent                  = gSettings.getboolean("QuickSnap", "Tangent")               #.toBool()
            self.settings_qsnap_nearest                  = gSettings.getboolean("QuickSnap", "Nearest")               #.toBool()
            self.settings_qsnap_apparent                 = gSettings.getboolean("QuickSnap", "Apparent")              #.toBool()
            self.settings_qsnap_parallel                 = gSettings.getboolean("QuickSnap", "Parallel")              #.toBool()
            # LineWeight
            self.settings_lwt_show_lwt                   = gSettings.getboolean("LineWeight", "ShowLineWeight")       #.toBool()
            self.settings_lwt_real_render                = gSettings.getboolean("LineWeight", "RealRender")           #.toBool()
            self.settings_lwt_default_lwt                = gSettings.getfloat("LineWeight", "DefaultLineWeight")      #.toReal()
            # Selection
            self.settings_selection_mode_pickfirst       = gSettings.getboolean("Selection", "PickFirst")             #.toBool()
            self.settings_selection_mode_pickadd         = gSettings.getboolean("Selection", "PickAdd")               #.toBool()
            self.settings_selection_mode_pickdrag        = gSettings.getboolean("Selection", "PickDrag")              #.toBool()
            self.settings_selection_coolgrip_color       = gSettings.getint("Selection", "CoolGripColor")             #.toInt()
            self.settings_selection_hotgrip_color        = gSettings.getint("Selection", "HotGripColor")              #.toInt()
            self.settings_selection_grip_size            = gSettings.getint("Selection", "GripSize")                  #.toInt()
            self.settings_selection_pickbox_size         = gSettings.getint("Selection", "PickBoxSize")               #.toInt()
            # Text
            self.settings_text_font                      = gSettings.get("Text", "Font")                              #.toString()
            self.settings_text_size                      = gSettings.getfloat("Text", "Size")                         #.toReal()
            self.settings_text_angle                     = gSettings.getfloat("Text", "Angle")                        #.toReal()
            self.settings_text_style_bold                = gSettings.getboolean("Text", "StyleBold")                  #.toBool()
            self.settings_text_style_italic              = gSettings.getboolean("Text", "StyleItalic")                #.toBool()
            self.settings_text_style_underline           = gSettings.getboolean("Text", "StyleUnderline")             #.toBool()
            self.settings_text_style_strikeout           = gSettings.getboolean("Text", "StyleStrikeOut")             #.toBool()
            self.settings_text_style_overline            = gSettings.getboolean("Text", "StyleOverline")              #.toBool()


        self.move(pos.x(), pos.y())
        self.resize(size.width(), size.height())


    def writeSettings(self): # void MainWindow::

        qDebug("Writing Settings...")
        ## settings = QSettings("settings-py.ini", QSettings.IniFormat)
        # gSettings = self.gSettings
        #
        # gSettings.setValue("Window/Position", self.pos())
        # gSettings.setValue("Window/Size", self.size())
        #
        # # General
        # gSettings.setValue("LayoutState",                               self.layoutState)
        # gSettings.setValue("Language",                                  self.settings_general_language)
        # gSettings.setValue("IconTheme",                                 self.settings_general_icon_theme)
        # gSettings.setValue("IconSize",                              int(self.settings_general_icon_size))
        # gSettings.setValue("MdiBGUseLogo",                              self.settings_general_mdi_bg_use_logo)
        # gSettings.setValue("MdiBGUseTexture",                           self.settings_general_mdi_bg_use_texture)
        # gSettings.setValue("MdiBGUseColor",                             self.settings_general_mdi_bg_use_color)
        # gSettings.setValue("MdiBGLogo",                                 self.settings_general_mdi_bg_logo)
        # gSettings.setValue("MdiBGTexture",                              self.settings_general_mdi_bg_texture)
        # gSettings.setValue("MdiBGColor",                            str(self.settings_general_mdi_bg_color))
        # gSettings.setValue("TipOfTheDay",                               self.settings_general_tip_of_the_day)
        # gSettings.setValue("CurrentTip",                            str(int(self.settings_general_current_tip) + 1))
        # # Display
        # gSettings.setValue("Display/UseOpenGL",                         self.settings_display_use_opengl)
        # gSettings.setValue("Display/RenderHintAntiAlias",               self.settings_display_renderhint_aa)
        # gSettings.setValue("Display/RenderHintTextAntiAlias",           self.settings_display_renderhint_text_aa)
        # gSettings.setValue("Display/RenderHintSmoothPixmap",            self.settings_display_renderhint_smooth_pix)
        # gSettings.setValue("Display/RenderHintHighQualityAntiAlias",    self.settings_display_renderhint_high_aa)
        # gSettings.setValue("Display/RenderHintNonCosmetic",             self.settings_display_renderhint_noncosmetic)
        # gSettings.setValue("Display/ShowScrollBars",                    self.settings_display_show_scrollbars)
        # gSettings.setValue("Display/ScrollBarWidgetNum",            str(self.settings_display_scrollbar_widget_num))
        # gSettings.setValue("Display/CrossHairColor",                str(self.settings_display_crosshair_color))
        # gSettings.setValue("Display/BackgroundColor",               str(self.settings_display_bg_color))
        # gSettings.setValue("Display/SelectBoxLeftColor",            str(self.settings_display_selectbox_left_color))
        # gSettings.setValue("Display/SelectBoxLeftFill",             str(self.settings_display_selectbox_left_fill))
        # gSettings.setValue("Display/SelectBoxRightColor",           str(self.settings_display_selectbox_right_color))
        # gSettings.setValue("Display/SelectBoxRightFill",            str(self.settings_display_selectbox_right_fill))
        # gSettings.setValue("Display/SelectBoxAlpha",                str(self.settings_display_selectbox_alpha))
        # gSettings.setValue("Display/ZoomScaleIn",                   str(self.settings_display_zoomscale_in))
        # gSettings.setValue("Display/ZoomScaleOut",                  str(self.settings_display_zoomscale_out))
        # gSettings.setValue("Display/CrossHairPercent",              str(self.settings_display_crosshair_percent))
        # gSettings.setValue("Display/Units",                             self.settings_display_units)
        # # Prompt
        # gSettings.setValue("Prompt/TextColor",                      str(self.settings_prompt_text_color))
        # gSettings.setValue("Prompt/BackgroundColor",                str(self.settings_prompt_bg_color))
        # gSettings.setValue("Prompt/FontFamily",                         self.settings_prompt_font_family)
        # gSettings.setValue("Prompt/FontStyle",                          self.settings_prompt_font_style)
        # gSettings.setValue("Prompt/FontSize",                       str(self.settings_prompt_font_size))
        # gSettings.setValue("Prompt/SaveHistory",                        self.settings_prompt_save_history)
        # gSettings.setValue("Prompt/SaveHistoryAsHtml",                  self.settings_prompt_save_history_as_html)
        # gSettings.setValue("Prompt/SaveHistoryFilename",                self.settings_prompt_save_history_filename)
        # # OpenSave
        # gSettings.setValue("OpenSave/CustomFilter",                     self.settings_opensave_custom_filter)
        # gSettings.setValue("OpenSave/OpenFormat",                       self.settings_opensave_open_format)
        # gSettings.setValue("OpenSave/OpenThumbnail",                    self.settings_opensave_open_thumbnail)
        # gSettings.setValue("OpenSave/SaveFormat",                       self.settings_opensave_save_format)
        # gSettings.setValue("OpenSave/SaveThumbnail",                    self.settings_opensave_save_thumbnail)
        # # Recent
        # gSettings.setValue("OpenSave/RecentMax",                    str(self.settings_opensave_recent_max_files))
        # gSettings.setValue("OpenSave/RecentFiles",                      self.settings_opensave_recent_list_of_files)
        # gSettings.setValue("OpenSave/RecentDirectory",                  self.settings_opensave_recent_directory)
        # # Trimming
        # gSettings.setValue("OpenSave/TrimDstNumJumps",              str(self.settings_opensave_trim_dst_num_jumps))
        # # Printing
        # gSettings.setValue("Printing/DefaultDevice",                    self.settings_printing_default_device)
        # gSettings.setValue("Printing/UseLastDevice",                    self.settings_printing_use_last_device)
        # gSettings.setValue("Printing/DisableBG",                        self.settings_printing_disable_bg)
        # # Grid
        # gSettings.setValue("Grid/ShowOnLoad",                           self.settings_grid_show_on_load)
        # gSettings.setValue("Grid/ShowOrigin",                           self.settings_grid_show_origin)
        # gSettings.setValue("Grid/ColorMatchCrossHair",                  self.settings_grid_color_match_crosshair)
        # gSettings.setValue("Grid/Color",                            str(self.settings_grid_color))
        # gSettings.setValue("Grid/LoadFromFile",                         self.settings_grid_load_from_file)
        # gSettings.setValue("Grid/Type",                                 self.settings_grid_type)
        # gSettings.setValue("Grid/CenterOnOrigin",                       self.settings_grid_center_on_origin)
        # gSettings.setValue("Grid/CenterX",                          str(self.settings_grid_center_x))
        # gSettings.setValue("Grid/CenterY",                          str(self.settings_grid_center_y))
        # gSettings.setValue("Grid/SizeX",                            str(self.settings_grid_size_x))
        # gSettings.setValue("Grid/SizeY",                            str(self.settings_grid_size_y))
        # gSettings.setValue("Grid/SpacingX",                         str(self.settings_grid_spacing_x))
        # gSettings.setValue("Grid/SpacingY",                         str(self.settings_grid_spacing_y))
        # gSettings.setValue("Grid/SizeRadius",                       str(self.settings_grid_size_radius))
        # gSettings.setValue("Grid/SpacingRadius",                    str(self.settings_grid_spacing_radius))
        # gSettings.setValue("Grid/SpacingAngle",                     str(self.settings_grid_spacing_angle))
        # # Ruler
        # gSettings.setValue("Ruler/ShowOnLoad",                          self.settings_ruler_show_on_load)
        # gSettings.setValue("Ruler/Metric",                              self.settings_ruler_metric)
        # gSettings.setValue("Ruler/Color",                           str(self.settings_ruler_color))
        # gSettings.setValue("Ruler/PixelSize",                       str(self.settings_ruler_pixel_size))
        # # Quick Snap
        # gSettings.setValue("QuickSnap/Enabled",                         self.settings_qsnap_enabled)
        # gSettings.setValue("QuickSnap/LocatorColor",                str(self.settings_qsnap_locator_color))
        # gSettings.setValue("QuickSnap/LocatorSize",                 str(self.settings_qsnap_locator_size))
        # gSettings.setValue("QuickSnap/ApertureSize",                str(self.settings_qsnap_aperture_size))
        # gSettings.setValue("QuickSnap/EndPoint",                        self.settings_qsnap_endpoint)
        # gSettings.setValue("QuickSnap/MidPoint",                        self.settings_qsnap_midpoint)
        # gSettings.setValue("QuickSnap/Center",                          self.settings_qsnap_center)
        # gSettings.setValue("QuickSnap/Node",                            self.settings_qsnap_node)
        # gSettings.setValue("QuickSnap/Quadrant",                        self.settings_qsnap_quadrant)
        # gSettings.setValue("QuickSnap/Intersection",                    self.settings_qsnap_intersection)
        # gSettings.setValue("QuickSnap/Extension",                       self.settings_qsnap_extension)
        # gSettings.setValue("QuickSnap/Insertion",                       self.settings_qsnap_insertion)
        # gSettings.setValue("QuickSnap/Perpendicular",                   self.settings_qsnap_perpendicular)
        # gSettings.setValue("QuickSnap/Tangent",                         self.settings_qsnap_tangent)
        # gSettings.setValue("QuickSnap/Nearest",                         self.settings_qsnap_nearest)
        # gSettings.setValue("QuickSnap/Apparent",                        self.settings_qsnap_apparent)
        # gSettings.setValue("QuickSnap/Parallel",                        self.settings_qsnap_parallel)
        # # LineWeight
        # gSettings.setValue("LineWeight/ShowLineWeight",                 self.settings_lwt_show_lwt)
        # gSettings.setValue("LineWeight/RealRender",                     self.settings_lwt_real_render)
        # gSettings.setValue("LineWeight/DefaultLineWeight",          str(self.settings_lwt_default_lwt))
        # # Selection
        # gSettings.setValue("Selection/PickFirst",                       self.settings_selection_mode_pickfirst)
        # gSettings.setValue("Selection/PickAdd",                         self.settings_selection_mode_pickadd)
        # gSettings.setValue("Selection/PickDrag",                        self.settings_selection_mode_pickdrag)
        # gSettings.setValue("Selection/CoolGripColor",               str(self.settings_selection_coolgrip_color))
        # gSettings.setValue("Selection/HotGripColor",                str(self.settings_selection_hotgrip_color))
        # gSettings.setValue("Selection/GripSize",                    str(self.settings_selection_grip_size))
        # gSettings.setValue("Selection/PickBoxSize",                 str(self.settings_selection_pickbox_size))
        # # Text
        # gSettings.setValue("Text/Font",                                 self.settings_text_font)
        # gSettings.setValue("Text/Size",                             str(self.settings_text_size))
        # gSettings.setValue("Text/Angle",                            str(self.settings_text_angle))
        # gSettings.setValue("Text/StyleBold",                            self.settings_text_style_bold)
        # gSettings.setValue("Text/StyleItalic",                          self.settings_text_style_italic)
        # gSettings.setValue("Text/StyleUnderline",                       self.settings_text_style_underline)
        # gSettings.setValue("Text/StyleStrikeOut",                       self.settings_text_style_strikeout)
        # gSettings.setValue("Text/StyleOverline",                        self.settings_text_style_overline)


        if not gSettings.has_section("Window"):
            gSettings.add_section("Window")
        gSettings.set("Window", "PositionX", self.pos().x())
        gSettings.set("Window", "PositionY", self.pos().y())
        gSettings.set("Window", "SizeX", self.size().width())
        gSettings.set("Window", "SizeY", self.size().height())

        # General
        if not gSettings.has_section("General"):
            gSettings.add_section("General")
        gSettings.set("General", "LayoutState",                     str(self.layoutState))
        gSettings.set("General", "Language",                        str(self.settings_general_language))
        gSettings.set("General", "IconTheme",                       str(self.settings_general_icon_theme))
        gSettings.set("General", "IconSize",                        str(self.settings_general_icon_size))
        gSettings.set("General", "MdiBGUseLogo",                    str(self.settings_general_mdi_bg_use_logo))
        gSettings.set("General", "MdiBGUseTexture",                 str(self.settings_general_mdi_bg_use_texture))
        gSettings.set("General", "MdiBGUseColor",                   str(self.settings_general_mdi_bg_use_color))
        gSettings.set("General", "MdiBGLogo",                       str(self.settings_general_mdi_bg_logo))
        gSettings.set("General", "MdiBGTexture",                    str(self.settings_general_mdi_bg_texture))
        gSettings.set("General", "MdiBGColor",                      str(self.settings_general_mdi_bg_color))
        gSettings.set("General", "TipOfTheDay",                     str(self.settings_general_tip_of_the_day))
        gSettings.set("General", "CurrentTip",                  str(int(self.settings_general_current_tip) + 1))
        # Display
        if not gSettings.has_section("Display"):
            gSettings.add_section("Display")
        gSettings.set("Display", "UseOpenGL",                       str(self.settings_display_use_opengl))
        gSettings.set("Display", "RenderHintAntiAlias",             str(self.settings_display_renderhint_aa))
        gSettings.set("Display", "RenderHintTextAntiAlias",         str(self.settings_display_renderhint_text_aa))
        gSettings.set("Display", "RenderHintSmoothPixmap",          str(self.settings_display_renderhint_smooth_pix))
        gSettings.set("Display", "RenderHintHighQualityAntiAlias",  str(self.settings_display_renderhint_high_aa))
        gSettings.set("Display", "RenderHintNonCosmetic",           str(self.settings_display_renderhint_noncosmetic))
        gSettings.set("Display", "ShowScrollBars",                  str(self.settings_display_show_scrollbars))
        gSettings.set("Display", "ScrollBarWidgetNum",              str(self.settings_display_scrollbar_widget_num))
        gSettings.set("Display", "CrossHairColor",                  str(self.settings_display_crosshair_color))
        gSettings.set("Display", "BackgroundColor",                 str(self.settings_display_bg_color))
        gSettings.set("Display", "SelectBoxLeftColor",              str(self.settings_display_selectbox_left_color))
        gSettings.set("Display", "SelectBoxLeftFill",               str(self.settings_display_selectbox_left_fill))
        gSettings.set("Display", "SelectBoxRightColor",             str(self.settings_display_selectbox_right_color))
        gSettings.set("Display", "SelectBoxRightFill",              str(self.settings_display_selectbox_right_fill))
        gSettings.set("Display", "SelectBoxAlpha",                  str(self.settings_display_selectbox_alpha))
        gSettings.set("Display", "ZoomScaleIn",                     str(self.settings_display_zoomscale_in))
        gSettings.set("Display", "ZoomScaleOut",                    str(self.settings_display_zoomscale_out))
        gSettings.set("Display", "CrossHairPercent",                str(self.settings_display_crosshair_percent))
        gSettings.set("Display", "Units",                           str(self.settings_display_units))
        # Prompt
        if not gSettings.has_section("Prompt"):
            gSettings.add_section("Prompt")
        gSettings.set("Prompt", "TextColor",                        str(self.settings_prompt_text_color))
        gSettings.set("Prompt", "BackgroundColor",                  str(self.settings_prompt_bg_color))
        gSettings.set("Prompt", "FontFamily",                       str(self.settings_prompt_font_family))
        gSettings.set("Prompt", "FontStyle",                        str(self.settings_prompt_font_style))
        gSettings.set("Prompt", "FontSize",                         str(self.settings_prompt_font_size))
        gSettings.set("Prompt", "SaveHistory",                      str(self.settings_prompt_save_history))
        gSettings.set("Prompt", "SaveHistoryAsHtml",                str(self.settings_prompt_save_history_as_html))
        gSettings.set("Prompt", "SaveHistoryFilename",              str(self.settings_prompt_save_history_filename))
        # OpenSave
        if not gSettings.has_section("OpenSave"):
            gSettings.add_section("OpenSave")
        gSettings.set("OpenSave", "CustomFilter",                   str(self.settings_opensave_custom_filter))
        gSettings.set("OpenSave", "OpenFormat",                     str(self.settings_opensave_open_format))
        gSettings.set("OpenSave", "OpenThumbnail",                  str(self.settings_opensave_open_thumbnail))
        gSettings.set("OpenSave", "SaveFormat",                     str(self.settings_opensave_save_format))
        gSettings.set("OpenSave", "SaveThumbnail",                  str(self.settings_opensave_save_thumbnail))
        # Recent
        gSettings.set("OpenSave", "RecentMax",                      str(self.settings_opensave_recent_max_files))
        gSettings.set("OpenSave", "RecentFiles",                    str(self.settings_opensave_recent_list_of_files))
        gSettings.set("OpenSave", "RecentDirectory",                str(self.settings_opensave_recent_directory))
        # Trimming
        gSettings.set("OpenSave", "TrimDstNumJumps",                str(self.settings_opensave_trim_dst_num_jumps))
        # Printing
        if not gSettings.has_section("Printing"):
            gSettings.add_section("Printing")
        gSettings.set("Printing", "DefaultDevice",                  str(self.settings_printing_default_device))
        gSettings.set("Printing", "UseLastDevice",                  str(self.settings_printing_use_last_device))
        gSettings.set("Printing", "DisableBG",                      str(self.settings_printing_disable_bg))
        # Grid
        if not gSettings.has_section("Grid"):
            gSettings.add_section("Grid")
        gSettings.set("Grid", "ShowOnLoad",                         str(self.settings_grid_show_on_load))
        gSettings.set("Grid", "ShowOrigin",                         str(self.settings_grid_show_origin))
        gSettings.set("Grid", "ColorMatchCrossHair",                str(self.settings_grid_color_match_crosshair))
        gSettings.set("Grid", "Color",                              str(self.settings_grid_color))
        gSettings.set("Grid", "LoadFromFile",                       str(self.settings_grid_load_from_file))
        gSettings.set("Grid", "Type",                               str(self.settings_grid_type))
        gSettings.set("Grid", "CenterOnOrigin",                     str(self.settings_grid_center_on_origin))
        gSettings.set("Grid", "CenterX",                           str(self.settings_grid_center_x))
        gSettings.set("Grid", "CenterY",                           str(self.settings_grid_center_y))
        gSettings.set("Grid", "SizeX",                             str(self.settings_grid_size_x))
        gSettings.set("Grid", "SizeY",                             str(self.settings_grid_size_y))
        gSettings.set("Grid", "SpacingX",                          str(self.settings_grid_spacing_x))
        gSettings.set("Grid", "SpacingY",                          str(self.settings_grid_spacing_y))
        gSettings.set("Grid", "SizeRadius",                        str(self.settings_grid_size_radius))
        gSettings.set("Grid", "SpacingRadius",                     str(self.settings_grid_spacing_radius))
        gSettings.set("Grid", "SpacingAngle",                      str(self.settings_grid_spacing_angle))
        # Ruler
        if not gSettings.has_section("Ruler"):
            gSettings.add_section("Ruler")
        gSettings.set("Ruler", "ShowOnLoad",                       str(self.settings_ruler_show_on_load))
        gSettings.set("Ruler", "Metric",                           str(self.settings_ruler_metric))
        gSettings.set("Ruler", "Color",                            str(self.settings_ruler_color))
        gSettings.set("Ruler", "PixelSize",                        str(self.settings_ruler_pixel_size))
        # Quick Snap
        if not gSettings.has_section("QuickSnap"):
            gSettings.add_section("QuickSnap")
        gSettings.set("QuickSnap", "Enabled",                      str(self.settings_qsnap_enabled))
        gSettings.set("QuickSnap", "LocatorColor",                 str(self.settings_qsnap_locator_color))
        gSettings.set("QuickSnap", "LocatorSize",                  str(self.settings_qsnap_locator_size))
        gSettings.set("QuickSnap", "ApertureSize",                 str(self.settings_qsnap_aperture_size))
        gSettings.set("QuickSnap", "EndPoint",                     str(self.settings_qsnap_endpoint))
        gSettings.set("QuickSnap", "MidPoint",                     str(self.settings_qsnap_midpoint))
        gSettings.set("QuickSnap", "Center",                       str(self.settings_qsnap_center))
        gSettings.set("QuickSnap", "Node",                         str(self.settings_qsnap_node))
        gSettings.set("QuickSnap", "Quadrant",                     str(self.settings_qsnap_quadrant))
        gSettings.set("QuickSnap", "Intersection",                 str(self.settings_qsnap_intersection))
        gSettings.set("QuickSnap", "Extension",                    str(self.settings_qsnap_extension))
        gSettings.set("QuickSnap", "Insertion",                    str(self.settings_qsnap_insertion))
        gSettings.set("QuickSnap", "Perpendicular",                str(self.settings_qsnap_perpendicular))
        gSettings.set("QuickSnap", "Tangent",                      str(self.settings_qsnap_tangent))
        gSettings.set("QuickSnap", "Nearest",                      str(self.settings_qsnap_nearest))
        gSettings.set("QuickSnap", "Apparent",                     str(self.settings_qsnap_apparent))
        gSettings.set("QuickSnap", "Parallel",                     str(self.settings_qsnap_parallel))
        # LineWeight
        if not gSettings.has_section("LineWeight"):
            gSettings.add_section("LineWeight")
        gSettings.set("LineWeight", "ShowLineWeight",              str(self.settings_lwt_show_lwt))
        gSettings.set("LineWeight", "RealRender",                  str(self.settings_lwt_real_render))
        gSettings.set("LineWeight", "DefaultLineWeight",           str(self.settings_lwt_default_lwt))
        # Selection
        if not gSettings.has_section("Selection"):
            gSettings.add_section("Selection")
        gSettings.set("Selection", "PickFirst",                    str(self.settings_selection_mode_pickfirst))
        gSettings.set("Selection", "PickAdd",                      str(self.settings_selection_mode_pickadd))
        gSettings.set("Selection", "PickDrag",                     str(self.settings_selection_mode_pickdrag))
        gSettings.set("Selection", "CoolGripColor",                str(self.settings_selection_coolgrip_color))
        gSettings.set("Selection", "HotGripColor",                 str(self.settings_selection_hotgrip_color))
        gSettings.set("Selection", "GripSize",                     str(self.settings_selection_grip_size))
        gSettings.set("Selection", "PickBoxSize",                  str(self.settings_selection_pickbox_size))
        # Text
        if not gSettings.has_section("Text"):
            gSettings.add_section("Text")
        gSettings.set("Text", "Font",                              str(self.settings_text_font))
        gSettings.set("Text", "Size",                              str(self.settings_text_size))
        gSettings.set("Text", "Angle",                             str(self.settings_text_angle))
        gSettings.set("Text", "StyleBold",                         str(self.settings_text_style_bold))
        gSettings.set("Text", "StyleItalic",                       str(self.settings_text_style_italic))
        gSettings.set("Text", "StyleUnderline",                    str(self.settings_text_style_underline))
        gSettings.set("Text", "StyleStrikeOut",                    str(self.settings_text_style_strikeout))
        gSettings.set("Text", "StyleOverline",                     str(self.settings_text_style_overline))

        # Write the UserConfigurableSettings.ini file
        cfgfile = open(gAppDir + os.sep + 'settings-py.ini', 'w')
        gSettings.write(cfgfile)
        cfgfile.close()

        qDebug("Done writing settings.")


    def settingsPrompt(self):
        """
        Show the Settings Dialog and open on the "Prompt" tab page.
        """
        self.settingsDialog("Prompt")


    def settingsDialog(self, showTab="General"): # void MainWindow::
        """
        Show the Settings Dialog.

        :param `showTab`:
        :type `showTab`: QString
        """
        dialog = Settings_Dialog(self, showTab, self)
        dialog.exec_()

    ##########################################################################
    # mainwindow_actions.py
    ##########################################################################

    def createAllActions(self):
        """
        TOWRITE

        .. TODO:: The c++ code is ugly. If you are going to line things up, line everything up the same so everything looks and reads nicely.

        """
        self.actionHash = actionHash = {}
        createAction = self.createAction
        tr = self.tr

        qDebug("Creating All Actions...")
        appName = QApplication.applicationName()  # QString

        actionHash["ACTION_donothing"] = createAction("donothing", tr("&Do Nothing"), tr("Does Nothing"))

        actionHash["ACTION_windowcascade"] =      createAction("windowcascade",  tr("&Cascade"),   tr("Cascade the windows."))
        actionHash["ACTION_windowtile"] =         createAction("windowtile",     tr("&Tile"),      tr("Tile the windows."))
        actionHash["ACTION_windowclose"] =        createAction("windowclose",    tr("Cl&ose"),     tr("Close the active window."))
        actionHash["ACTION_windowcloseall"] =     createAction("windowcloseall", tr("Close &All"), tr("Close all the windows."))
        actionHash["ACTION_windownext"] =         createAction("windownext",     tr("Ne&xt"),      tr("Move the focus to the next window."))
        actionHash["ACTION_windowprevious"] =     createAction("windowprevious", tr("Pre&vious"),  tr("Move the focus to the previous window."))

        actionHash["ACTION_new"] =                createAction("new",           tr("&New"),     tr("Create a new file."))
        actionHash["ACTION_open"] =               createAction("open",          tr("&Open"),    tr("Open an existing file."))
        actionHash["ACTION_save"] =               createAction("save",          tr("&Save"),    tr("Save the design to disk."))
        actionHash["ACTION_saveas"] =             createAction("saveas",        tr("Save &As"), tr("Save the design under a new name."))
        actionHash["ACTION_print"] =              createAction("print",         tr("&Print"),   tr("Print the design."))
        actionHash["ACTION_designdetails"] =      createAction("designdetails", tr("&Details"), tr("Details of the current design."))
        actionHash["ACTION_exit"] =               createAction("exit",          tr("E&xit"),    tr("Exit the application."))

        actionHash["ACTION_cut"] =                createAction("cut",   tr("Cu&t"),   tr("Cut the current selection's contents to the clipboard."))
        actionHash["ACTION_copy"] =               createAction("copy",  tr("&Copy"),  tr("Copy the current selection's contents to the clipboard."))
        actionHash["ACTION_paste"] =              createAction("paste", tr("&Paste"), tr("Paste the clipboard's contents into the current selection."))

        actionHash["ACTION_help"] =               createAction("help",        tr("&Help"),             tr("Displays help."))
        actionHash["ACTION_changelog"] =          createAction("changelog",   tr("&Changelog"),        tr("Describes new features in this product."))
        actionHash["ACTION_tipoftheday"] =        createAction("tipoftheday", tr("&Tip Of The Day"),   tr("Displays a dialog with useful tips"))
        actionHash["ACTION_about"] =              createAction("about",       tr("&About ") + appName, tr("Displays information about this product."))
        actionHash["ACTION_whatsthis"] =          createAction("whatsthis",   tr("&What's This?"),     tr("What's This? Context Help!"))

        actionHash["ACTION_undo"] =               createAction("undo", tr("&Undo"),  tr("Reverses the most recent action."))
        actionHash["ACTION_redo"] =               createAction("redo", tr("&Redo"),  tr("Reverses the effects of the previous undo action."))

        actionHash["ACTION_icon16"] =             createAction("icon16",  tr("Icon&16"),  tr("Sets the toolbar icon size to 16x16."))
        actionHash["ACTION_icon24"] =             createAction("icon24",  tr("Icon&24"),  tr("Sets the toolbar icon size to 24x24."))
        actionHash["ACTION_icon32"] =             createAction("icon32",  tr("Icon&32"),  tr("Sets the toolbar icon size to 32x32."))
        actionHash["ACTION_icon48"] =             createAction("icon48",  tr("Icon&48"),  tr("Sets the toolbar icon size to 48x48."))
        actionHash["ACTION_icon64"] =             createAction("icon64",  tr("Icon&64"),  tr("Sets the toolbar icon size to 64x64."))
        actionHash["ACTION_icon128"] =            createAction("icon128", tr("Icon12&8"), tr("Sets the toolbar icon size to 128x128."))

        actionHash["ACTION_settingsdialog"] =     createAction("settingsdialog", tr("&Settings"), tr("Configure settings specific to this product."))

        actionHash["ACTION_makelayercurrent"] =   createAction("makelayercurrent",   tr("&Make Layer Active"),    tr("Makes the layer of a selected object the active layer"))
        actionHash["ACTION_layers"] =             createAction("layers",             tr("&Layers"),                tr("Manages layers and layer properties:  LAYER"))
        actionHash["ACTION_layerselector"] =      createAction("layerselector",      tr("&Layer Selector"),        tr("Dropdown selector for changing the current layer"))
        actionHash["ACTION_layerprevious"] =      createAction("layerprevious",      tr("&Layer Previous"),        tr("Restores the previous layer settings:  LAYERP"))
        actionHash["ACTION_colorselector"] =      createAction("colorselector",      tr("&Color Selector"),        tr("Dropdown selector for changing the current thread color"))
        actionHash["ACTION_linetypeselector"] =   createAction("linetypeselector",   tr("&Stitchtype Selector"),   tr("Dropdown selector for changing the current stitch type"))
        actionHash["ACTION_lineweightselector"] = createAction("lineweightselector", tr("&Threadweight Selector"), tr("Dropdown selector for changing the current thread weight"))
        actionHash["ACTION_hidealllayers"] =      createAction("hidealllayers",      tr("&Hide All Layers"),       tr("Turns the visibility off for all layers in the current drawing:  HIDEALL"))
        actionHash["ACTION_showalllayers"] =      createAction("showalllayers",      tr("&Show All Layers"),       tr("Turns the visibility on for all layers in the current drawing:  SHOWALL"))
        actionHash["ACTION_freezealllayers"] =    createAction("freezealllayers",    tr("&Freeze All Layers"),     tr("Freezes all layers in the current drawing:  FREEZEALL"))
        actionHash["ACTION_thawalllayers"] =      createAction("thawalllayers",      tr("&Thaw All Layers"),       tr("Thaws all layers in the current drawing:  THAWALL"))
        actionHash["ACTION_lockalllayers"] =      createAction("lockalllayers",      tr("&Lock All Layers"),       tr("Locks all layers in the current drawing:  LOCKALL"))
        actionHash["ACTION_unlockalllayers"] =    createAction("unlockalllayers",    tr("&Unlock All Layers"),     tr("Unlocks all layers in the current drawing:  UNLOCKALL"))

        actionHash["ACTION_textbold"] =           createAction("textbold",      tr("&Bold Text"),      tr("Sets text to be bold."))
        actionHash["ACTION_textitalic"] =         createAction("textitalic",    tr("&Italic Text"),    tr("Sets text to be italic."))
        actionHash["ACTION_textunderline"] =      createAction("textunderline", tr("&Underline Text"), tr("Sets text to be underlined."))
        actionHash["ACTION_textstrikeout"] =      createAction("textstrikeout", tr("&StrikeOut Text"), tr("Sets text to be striked out."))
        actionHash["ACTION_textoverline"] =       createAction("textoverline",  tr("&Overline Text"),  tr("Sets text to be overlined."))

        actionHash["ACTION_zoomrealtime"] =       createAction("zoomrealtime", tr("Zoom &Realtime"), tr("Zooms to increase or decrease the apparent size of objects in the current viewport."))
        actionHash["ACTION_zoomprevious"] =       createAction("zoomprevious", tr("Zoom &Previous"), tr("Zooms to display the previous view."))
        actionHash["ACTION_zoomwindow"] =         createAction("zoomwindow",   tr("Zoom &Window"),   tr("Zooms to display an area specified by a rectangular window."))
        actionHash["ACTION_zoomdynamic"] =        createAction("zoomdynamic",  tr("Zoom &Dynamic"),  tr("Zooms to display the generated portion of the drawing."))
        actionHash["ACTION_zoomscale"] =          createAction("zoomscale",    tr("Zoom &Scale"),    tr("Zooms the display using a specified scale factor."))
        actionHash["ACTION_zoomcenter"] =         createAction("zoomcenter",   tr("Zoom &Center"),   tr("Zooms to display a view specified by a center point and magnification or height."))
        actionHash["ACTION_zoomin"] =             createAction("zoomin",       tr("Zoom &In"),       tr("Zooms to increase the apparent size of objects."))
        actionHash["ACTION_zoomout"] =            createAction("zoomout",      tr("Zoom &Out"),      tr("Zooms to decrease the apparent size of objects."))
        actionHash["ACTION_zoomselected"] =       createAction("zoomselected", tr("Zoom Selec&ted"), tr("Zooms to display the selected objects."))
        actionHash["ACTION_zoomall"] =            createAction("zoomall",      tr("Zoom &All"),      tr("Zooms to display the drawing extents or the grid limits."))
        actionHash["ACTION_zoomextents"] =        createAction("zoomextents",  tr("Zoom &Extents"),  tr("Zooms to display the drawing extents."))

        actionHash["ACTION_panrealtime"] =        createAction("panrealtime", tr("&Pan Realtime"), tr("Moves the view in the current viewport."))
        actionHash["ACTION_panpoint"] =           createAction("panpoint",    tr("&Pan Point"),    tr("Moves the view by the specified distance."))
        actionHash["ACTION_panleft"] =            createAction("panleft",     tr("&Pan Left"),     tr("Moves the view to the left."))
        actionHash["ACTION_panright"] =           createAction("panright",    tr("&Pan Right"),    tr("Moves the view to the right."))
        actionHash["ACTION_panup"] =              createAction("panup",       tr("&Pan Up"),       tr("Moves the view up."))
        actionHash["ACTION_pandown"] =            createAction("pandown",     tr("&Pan Down"),     tr("Moves the view down."))

        actionHash["ACTION_day"] =                createAction("day",   tr("&Day"),   tr("Updates the current view using day vision settings."))
        actionHash["ACTION_night"] =              createAction("night", tr("&Night"), tr("Updates the current view using night vision settings."))

        actionHash["ACTION_windowclose"].setEnabled(self.numOfDocs > 0)
        actionHash["ACTION_designdetails"].setEnabled(self.numOfDocs > 0)


    def createAction(self, icon, toolTip, statusTip, scripted=False):
        """
        TOWRITE

        :param `icon`: TOWRITE
        :type `icon`: QString
        :param `toolTip`: TOWRITE
        :type `toolTip`: QString
        :param `statusTip`: TOWRITE
        :type `statusTip`: QString
        :param `scripted`: TOWRITE
        :type `scripted`: bool
        :rtype: `QAction`_

        .. TODO:: send the global Icon, Image, Commands Dirs in to be used.

        """
        connect = self.connect # NOT local optimization, but for ease of the code looking similar to the C++ whash
        mdiArea = self.mdiArea # ditto

        ACTION = QAction(QIcon(self.gIconDir + os.sep + self.getSettingsGeneralIconTheme() + "/" + icon + ".png"), toolTip, self)  # QAction *  # TODO: Qt4.7 wont load icons without an extension...
        ACTION.setStatusTip(statusTip)
        ACTION.setObjectName(icon)
        # TODO: Set What's This Context Help to statusTip for now so there is some infos there.
        # Make custom whats this context help popup with more descriptive help than just
        # the status bar/tip one liner(short but not real long) with a hyperlink in the custom popup
        # at the bottom to open full help file description. Ex: like wxPython AGW's SuperToolTip.
        ACTION.setWhatsThis(statusTip)
        # TODO: Finish All Commands ... <.<

        if   icon == "donothing":                  connect(ACTION, SIGNAL("triggered()"), self, SLOT("doNothing()"))
        elif icon == "new":                        ACTION.setShortcut(QKeySequence.New);       connect(ACTION, SIGNAL("triggered()"), self, SLOT("newFile()"))
        elif icon == "open":                       ACTION.setShortcut(QKeySequence.Open);      connect(ACTION, SIGNAL("triggered()"), self, SLOT("openFile()"))
        elif icon == "save":                       ACTION.setShortcut(QKeySequence.Save);      connect(ACTION, SIGNAL("triggered()"), self, SLOT("savefile()"))
        elif icon == "saveas":                     ACTION.setShortcut(QKeySequence.SaveAs);    connect(ACTION, SIGNAL("triggered()"), self, SLOT("saveasfile()"))
        elif icon == "print":                      ACTION.setShortcut(QKeySequence.Print);     connect(ACTION, SIGNAL("triggered()"), self, SLOT("print_()"))
        elif icon == "designdetails":              ACTION.setShortcut(QKeySequence("Ctrl+D")); connect(ACTION, SIGNAL("triggered()"), self, SLOT("designDetails()"))
        elif icon == "exit":                       ACTION.setShortcut(QKeySequence("Ctrl+Q")); connect(ACTION, SIGNAL("triggered()"), self, SLOT("exit()"))

        elif icon == "cut":                        ACTION.setShortcut(QKeySequence.Cut);   connect(ACTION, SIGNAL("triggered()"), self, SLOT("cut()"))
        elif icon == "copy":                       ACTION.setShortcut(QKeySequence.Copy);  connect(ACTION, SIGNAL("triggered()"), self, SLOT("copy()"))
        elif icon == "paste":                      ACTION.setShortcut(QKeySequence.Paste); connect(ACTION, SIGNAL("triggered()"), self, SLOT("paste()"))

        elif icon == "windowcascade":              connect(ACTION, SIGNAL("triggered()"), mdiArea, SLOT("cascade()"))
        elif icon == "windowtile":                 connect(ACTION, SIGNAL("triggered()"), mdiArea, SLOT("tile()"))
        elif icon == "windowclose":                ACTION.setShortcut(QKeySequence.Close);         connect(ACTION, SIGNAL("triggered()"), self, SLOT("onCloseWindow()"))
        elif icon == "windowcloseall":             connect(ACTION, SIGNAL("triggered()"), mdiArea, SLOT("closeAllSubWindows()"))
        elif icon == "windownext":                 ACTION.setShortcut(QKeySequence.NextChild);     connect(ACTION, SIGNAL("triggered()"), mdiArea, SLOT("activateNextSubWindow()"))
        elif icon == "windowprevious":             ACTION.setShortcut(QKeySequence.PreviousChild); connect(ACTION, SIGNAL("triggered()"), mdiArea, SLOT("activatePreviousSubWindow()"))

        elif icon == "help":                       connect(ACTION, SIGNAL("triggered()"), self, SLOT("help()"))
        elif icon == "changelog":                  connect(ACTION, SIGNAL("triggered()"), self, SLOT("changelog()"))
        elif icon == "tipoftheday":                connect(ACTION, SIGNAL("triggered()"), self, SLOT("tipOfTheDay()"))
        elif icon == "about":                      connect(ACTION, SIGNAL("triggered()"), self, SLOT("about()"))
        elif icon == "whatsthis":                  connect(ACTION, SIGNAL("triggered()"), self, SLOT("whatsThisContextHelp()"))

        elif icon == "icon16":                     connect(ACTION, SIGNAL("triggered()"), self, SLOT("icon16()"))
        elif icon == "icon24":                     connect(ACTION, SIGNAL("triggered()"), self, SLOT("icon24()"))
        elif icon == "icon32":                     connect(ACTION, SIGNAL("triggered()"), self, SLOT("icon32()"))
        elif icon == "icon48":                     connect(ACTION, SIGNAL("triggered()"), self, SLOT("icon48()"))
        elif icon == "icon64":                     connect(ACTION, SIGNAL("triggered()"), self, SLOT("icon64()"))
        elif icon == "icon128":                    connect(ACTION, SIGNAL("triggered()"), self, SLOT("icon128()"))

        elif icon == "settingsdialog":             connect(ACTION, SIGNAL("triggered()"), self, SLOT("settingsDialog()"))

        elif icon == "undo":                       connect(ACTION, SIGNAL("triggered()"), self, SLOT("undo()"))
        elif icon == "redo":                       connect(ACTION, SIGNAL("triggered()"), self, SLOT("redo()"))

        elif icon == "makelayercurrent":           connect(ACTION, SIGNAL("triggered()"), self, SLOT("makeLayerActive()"))
        elif icon == "layers":                     connect(ACTION, SIGNAL("triggered()"), self, SLOT("layerManager()"))
        elif icon == "layerprevious":              connect(ACTION, SIGNAL("triggered()"), self, SLOT("layerPrevious()"))

        elif icon == "textbold":                   ACTION.setCheckable(True); connect(ACTION, SIGNAL("toggled(bool)"), self, SLOT("setTextBold(bool)"))
        elif icon == "textitalic":                 ACTION.setCheckable(True); connect(ACTION, SIGNAL("toggled(bool)"), self, SLOT("setTextItalic(bool)"))
        elif icon == "textunderline":              ACTION.setCheckable(True); connect(ACTION, SIGNAL("toggled(bool)"), self, SLOT("setTextUnderline(bool)"))
        elif icon == "textstrikeout":              ACTION.setCheckable(True); connect(ACTION, SIGNAL("toggled(bool)"), self, SLOT("setTextStrikeOut(bool)"))
        elif icon == "textoverline":               ACTION.setCheckable(True); connect(ACTION, SIGNAL("toggled(bool)"), self, SLOT("setTextOverline(bool)"))

        elif icon == "zoomrealtime":               connect(ACTION, SIGNAL("triggered()"), self, SLOT("zoomRealtime()"))
        elif icon == "zoomprevious":               connect(ACTION, SIGNAL("triggered()"), self, SLOT("zoomPrevious()"))
        elif icon == "zoomwindow":                 connect(ACTION, SIGNAL("triggered()"), self, SLOT("zoomWindow()"))
        elif icon == "zoomdynamic":                connect(ACTION, SIGNAL("triggered()"), self, SLOT("zoomDynamic()"))
        elif icon == "zoomscale":                  connect(ACTION, SIGNAL("triggered()"), self, SLOT("zoomScale()"))
        elif icon == "zoomcenter":                 connect(ACTION, SIGNAL("triggered()"), self, SLOT("zoomCenter()"))
        elif icon == "zoomin":                     connect(ACTION, SIGNAL("triggered()"), self, SLOT("zoomIn()"))
        elif icon == "zoomout":                    connect(ACTION, SIGNAL("triggered()"), self, SLOT("zoomOut()"))
        elif icon == "zoomselected":               connect(ACTION, SIGNAL("triggered()"), self, SLOT("zoomSelected()"))
        elif icon == "zoomall":                    connect(ACTION, SIGNAL("triggered()"), self, SLOT("zoomAll()"))
        elif icon == "zoomextents":                connect(ACTION, SIGNAL("triggered()"), self, SLOT("zoomExtents()"))

        elif icon == "panrealtime":                connect(ACTION, SIGNAL("triggered()"), self, SLOT("panrealtime()"))
        elif icon == "panpoint":                   connect(ACTION, SIGNAL("triggered()"), self, SLOT("panpoint()"))
        elif icon == "panleft":                    connect(ACTION, SIGNAL("triggered()"), self, SLOT("panLeft()"))
        elif icon == "panright":                   connect(ACTION, SIGNAL("triggered()"), self, SLOT("panRight()"))
        elif icon == "panup":                      connect(ACTION, SIGNAL("triggered()"), self, SLOT("panUp()"))
        elif icon == "pandown":                    connect(ACTION, SIGNAL("triggered()"), self, SLOT("panDown()"))

        elif icon == "day":                        connect(ACTION, SIGNAL("triggered()"), self, SLOT("dayVision()"))
        elif icon == "night":                      connect(ACTION, SIGNAL("triggered()"), self, SLOT("nightVision()"))

        elif scripted:
            ACTION.setIcon(QIcon("commands/" + icon + "/" + icon + ".png"))
            connect(ACTION, SIGNAL("triggered()"), self, SLOT("runCommand()"))

        else:
            ACTION.setEnabled(False)
            connect(ACTION, SIGNAL("triggered()"), self, SLOT("stub_implement()"))

        return ACTION

    ##########################################################################
    # mainwindow_menus.py
    ##########################################################################

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

    ##########################################################################
    # mainwindow_toolbars.py
    ##########################################################################

    def createFileToolbar(self):
        """
        TOWRITE
        """
        toolbarFile = self.toolbarFile
        actionHash = self.actionHash
        qDebug("MainWindow createFileToolbar()")

        toolbarFile.setObjectName("toolbarFile")
        toolbarFile.addAction(actionHash["ACTION_new"])
        toolbarFile.addAction(actionHash["ACTION_open"])
        toolbarFile.addAction(actionHash["ACTION_save"])
        toolbarFile.addAction(actionHash["ACTION_saveas"])
        toolbarFile.addAction(actionHash["ACTION_print"])
        toolbarFile.addAction(actionHash["ACTION_designdetails"])

        toolbarFile.addSeparator()
        toolbarFile.addAction(actionHash["ACTION_undo"])
        toolbarFile.addAction(actionHash["ACTION_redo"])
        toolbarFile.addSeparator()
        toolbarFile.addAction(actionHash["ACTION_help"])

        self.connect(toolbarFile, SIGNAL("topLevelChanged(bool)"), self, SLOT("floatingChangedToolBar(bool)"))


    def createEditToolbar(self):
        """
        TOWRITE
        """
        toolbarEdit = self.toolbarEdit
        actionHash = self.actionHash
        qDebug("MainWindow createEditToolbar()")

        toolbarEdit.setObjectName("toolbarEdit")
        toolbarEdit.addAction(actionHash["ACTION_cut"])
        toolbarEdit.addAction(actionHash["ACTION_copy"])
        toolbarEdit.addAction(actionHash["ACTION_paste"])

        self.connect(toolbarEdit, SIGNAL("topLevelChanged(bool)"), self, SLOT("floatingChangedToolBar(bool)"))


    def createViewToolbar(self):
        """
        TOWRITE
        """
        toolbarView = self.toolbarView
        actionHash = self.actionHash
        qDebug("MainWindow createViewToolbar()")

        toolbarView.setObjectName("toolbarView")
        toolbarView.addAction(actionHash["ACTION_day"])
        toolbarView.addAction(actionHash["ACTION_night"])

        self.connect(toolbarView, SIGNAL("topLevelChanged(bool)"), self, SLOT("floatingChangedToolBar(bool)"))


    def createZoomToolbar(self):
        """
        TOWRITE
        """
        toolbarZoom = self.toolbarZoom
        actionHash = self.actionHash
        qDebug("MainWindow createZoomToolbar()")

        toolbarZoom.setObjectName("toolbarZoom")
        toolbarZoom.addAction(actionHash["ACTION_zoomwindow"])
        toolbarZoom.addAction(actionHash["ACTION_zoomdynamic"])
        toolbarZoom.addAction(actionHash["ACTION_zoomscale"])
        toolbarZoom.addSeparator()
        toolbarZoom.addAction(actionHash["ACTION_zoomcenter"])
        toolbarZoom.addAction(actionHash["ACTION_zoomin"])
        toolbarZoom.addAction(actionHash["ACTION_zoomout"])
        toolbarZoom.addSeparator()
        toolbarZoom.addAction(actionHash["ACTION_zoomselected"])
        toolbarZoom.addAction(actionHash["ACTION_zoomall"])
        toolbarZoom.addAction(actionHash["ACTION_zoomextents"])

        self.connect(toolbarZoom, SIGNAL("topLevelChanged(bool)"), self, SLOT("floatingChangedToolBar(bool)"))


    def createPanToolbar(self):
        """
        TOWRITE
        """
        toolbarPan = self.toolbarPan
        actionHash = self.actionHash
        qDebug("MainWindow createPanToolbar()")

        toolbarPan.setObjectName("toolbarPan")
        toolbarPan.addAction(actionHash["ACTION_panrealtime"])
        toolbarPan.addAction(actionHash["ACTION_panpoint"])
        toolbarPan.addSeparator()
        toolbarPan.addAction(actionHash["ACTION_panleft"])
        toolbarPan.addAction(actionHash["ACTION_panright"])
        toolbarPan.addAction(actionHash["ACTION_panup"])
        toolbarPan.addAction(actionHash["ACTION_pandown"])

        self.connect(toolbarPan, SIGNAL("topLevelChanged(bool)"), self, SLOT("floatingChangedToolBar(bool)"))


    def createIconToolbar(self):
        """
        TOWRITE
        """
        toolbarIcon = self.toolbarIcon
        actionHash = self.actionHash
        qDebug("MainWindow createIconToolbar()")

        toolbarIcon.setObjectName("toolbarIcon")
        toolbarIcon.addAction(actionHash["ACTION_icon16"])
        toolbarIcon.addAction(actionHash["ACTION_icon24"])
        toolbarIcon.addAction(actionHash["ACTION_icon32"])
        toolbarIcon.addAction(actionHash["ACTION_icon48"])
        toolbarIcon.addAction(actionHash["ACTION_icon64"])
        toolbarIcon.addAction(actionHash["ACTION_icon128"])

        self.connect(toolbarIcon, SIGNAL("topLevelChanged(bool)"), self, SLOT("floatingChangedToolBar(bool)"))


    def createHelpToolbar(self):
        """
        TOWRITE
        """
        toolbarHelp = self.toolbarHelp
        actionHash = self.actionHash
        qDebug("MainWindow createHelpToolbar()")

        toolbarHelp.setObjectName("toolbarHelp")
        toolbarHelp.addAction(actionHash["ACTION_help"])
        toolbarHelp.addSeparator()
        toolbarHelp.addAction(actionHash["ACTION_changelog"])
        toolbarHelp.addSeparator()
        toolbarHelp.addAction(actionHash["ACTION_about"])
        toolbarHelp.addSeparator()
        toolbarHelp.addAction(actionHash["ACTION_whatsthis"])

        self.connect(toolbarHelp, SIGNAL("topLevelChanged(bool)"), self, SLOT("floatingChangedToolBar(bool)"))


    def createLayerToolbar(self):
        """
        TOWRITE
        """
        toolbarLayer = self.toolbarLayer
        actionHash = self.actionHash
        layerSelector = self.layerSelector
        qDebug("MainWindow createLayerToolbar()")

        toolbarLayer.setObjectName("toolbarLayer")
        toolbarLayer.addAction(actionHash["ACTION_makelayercurrent"])
        toolbarLayer.addAction(actionHash["ACTION_layers"])

        icontheme = self.getSettingsGeneralIconTheme()  # QString

        layerSelector.setFocusProxy(self.prompt)
        # NOTE: Qt4.7 wont load icons without an extension...
        # TODO: Create layer pixmaps by concatenating several icons
        layerSelector.addItem(QIcon("icons/" + icontheme + "/" + "linetypebylayer" + ".png"), "0")
        layerSelector.addItem(QIcon("icons/" + icontheme + "/" + "linetypebylayer" + ".png"), "1")
        layerSelector.addItem(QIcon("icons/" + icontheme + "/" + "linetypebylayer" + ".png"), "2")
        layerSelector.addItem(QIcon("icons/" + icontheme + "/" + "linetypebylayer" + ".png"), "3")
        layerSelector.addItem(QIcon("icons/" + icontheme + "/" + "linetypebylayer" + ".png"), "4")
        layerSelector.addItem(QIcon("icons/" + icontheme + "/" + "linetypebylayer" + ".png"), "5")
        layerSelector.addItem(QIcon("icons/" + icontheme + "/" + "linetypebylayer" + ".png"), "6")
        layerSelector.addItem(QIcon("icons/" + icontheme + "/" + "linetypebylayer" + ".png"), "7")
        layerSelector.addItem(QIcon("icons/" + icontheme + "/" + "linetypebylayer" + ".png"), "8")
        layerSelector.addItem(QIcon("icons/" + icontheme + "/" + "linetypebylayer" + ".png"), "9")
        toolbarLayer.addWidget(layerSelector)
        self.connect(layerSelector, SIGNAL("currentIndexChanged(int)"), self, SLOT("layerSelectorIndexChanged(int)"))

        toolbarLayer.addAction(actionHash["ACTION_layerprevious"])

        self.connect(toolbarLayer, SIGNAL("topLevelChanged(bool)"), self, SLOT("floatingChangedToolBar(bool)"))


    def createPropertiesToolbar(self):
        """
        TOWRITE
        """
        toolbarProperties = self.toolbarProperties
        prompt = self.prompt
        colorSelector = self.colorSelector
        linetypeSelector = self.linetypeSelector
        lineweightSelector = self.lineweightSelector
        tr = self.tr
        qDebug("MainWindow createPropertiesToolbar()")

        toolbarProperties.setObjectName("toolbarProperties")

        icontheme = self.getSettingsGeneralIconTheme()  # QString

        colorSelector.setFocusProxy(prompt)
        # NOTE: Qt4.7 wont load icons without an extension...
        colorSelector.addItem(QIcon("icons/" + icontheme + "/" + "colorbylayer" + ".png"), "ByLayer")
        colorSelector.addItem(QIcon("icons/" + icontheme + "/" + "colorbyblock" + ".png"), "ByBlock")
        colorSelector.addItem(QIcon("icons/" + icontheme + "/" + "colorred"     + ".png"), tr("Red"),     qRgb(255,   0,   0))
        colorSelector.addItem(QIcon("icons/" + icontheme + "/" + "coloryellow"  + ".png"), tr("Yellow"),  qRgb(255, 255,   0))
        colorSelector.addItem(QIcon("icons/" + icontheme + "/" + "colorgreen"   + ".png"), tr("Green"),   qRgb(  0, 255,   0))
        colorSelector.addItem(QIcon("icons/" + icontheme + "/" + "colorcyan"    + ".png"), tr("Cyan"),    qRgb(  0, 255, 255))
        colorSelector.addItem(QIcon("icons/" + icontheme + "/" + "colorblue"    + ".png"), tr("Blue"),    qRgb(  0,   0, 255))
        colorSelector.addItem(QIcon("icons/" + icontheme + "/" + "colormagenta" + ".png"), tr("Magenta"), qRgb(255,   0, 255))
        colorSelector.addItem(QIcon("icons/" + icontheme + "/" + "colorwhite"   + ".png"), tr("White"),   qRgb(255, 255, 255))
        colorSelector.addItem(QIcon("icons/" + icontheme + "/" + "colorother"   + ".png"), tr("Other..."))
        toolbarProperties.addWidget(colorSelector)
        self.connect(colorSelector, SIGNAL("currentIndexChanged(int)"), self, SLOT("colorSelectorIndexChanged(int)"))

        toolbarProperties.addSeparator()
        linetypeSelector.setFocusProxy(prompt)
        # NOTE: Qt4.7 wont load icons without an extension...
        linetypeSelector.addItem(QIcon("icons/" + icontheme + "/" + "linetypebylayer"    + ".png"), "ByLayer")
        linetypeSelector.addItem(QIcon("icons/" + icontheme + "/" + "linetypebyblock"    + ".png"), "ByBlock")
        linetypeSelector.addItem(QIcon("icons/" + icontheme + "/" + "linetypecontinuous" + ".png"), "Continuous")
        linetypeSelector.addItem(QIcon("icons/" + icontheme + "/" + "linetypehidden"     + ".png"), "Hidden")
        linetypeSelector.addItem(QIcon("icons/" + icontheme + "/" + "linetypecenter"     + ".png"), "Center")
        linetypeSelector.addItem(QIcon("icons/" + icontheme + "/" + "linetypeother"      + ".png"), "Other...")
        toolbarProperties.addWidget(linetypeSelector)
        self.connect(linetypeSelector, SIGNAL("currentIndexChanged(int)"), self, SLOT("linetypeSelectorIndexChanged(int)"))

        toolbarProperties.addSeparator()
        lineweightSelector.setFocusProxy(prompt)
        # NOTE: Qt4.7 wont load icons without an extension...
        lineweightSelector.addItem(QIcon("icons/" + icontheme + "/" + "lineweightbylayer" + ".png"), "ByLayer", -2.00)
        lineweightSelector.addItem(QIcon("icons/" + icontheme + "/" + "lineweightbyblock" + ".png"), "ByBlock", -1.00)
        lineweightSelector.addItem(QIcon("icons/" + icontheme + "/" + "lineweightdefault" + ".png"), "Default",  0.00)
        # TODO: Thread weight is weird. See http://en.wikipedia.org/wiki/Thread_(yarn)#Weight
        lineweightSelector.addItem(QIcon("icons/" + icontheme + "/" + "lineweight01" + ".png"), "0.00 mm", 0.00)
        lineweightSelector.addItem(QIcon("icons/" + icontheme + "/" + "lineweight02" + ".png"), "0.05 mm", 0.05)
        lineweightSelector.addItem(QIcon("icons/" + icontheme + "/" + "lineweight03" + ".png"), "0.15 mm", 0.15)
        lineweightSelector.addItem(QIcon("icons/" + icontheme + "/" + "lineweight04" + ".png"), "0.20 mm", 0.20)
        lineweightSelector.addItem(QIcon("icons/" + icontheme + "/" + "lineweight05" + ".png"), "0.25 mm", 0.25)
        lineweightSelector.addItem(QIcon("icons/" + icontheme + "/" + "lineweight06" + ".png"), "0.30 mm", 0.30)
        lineweightSelector.addItem(QIcon("icons/" + icontheme + "/" + "lineweight07" + ".png"), "0.35 mm", 0.35)
        lineweightSelector.addItem(QIcon("icons/" + icontheme + "/" + "lineweight08" + ".png"), "0.40 mm", 0.40)
        lineweightSelector.addItem(QIcon("icons/" + icontheme + "/" + "lineweight09" + ".png"), "0.45 mm", 0.45)
        lineweightSelector.addItem(QIcon("icons/" + icontheme + "/" + "lineweight10" + ".png"), "0.50 mm", 0.50)
        lineweightSelector.addItem(QIcon("icons/" + icontheme + "/" + "lineweight11" + ".png"), "0.55 mm", 0.55)
        lineweightSelector.addItem(QIcon("icons/" + icontheme + "/" + "lineweight12" + ".png"), "0.60 mm", 0.60)
        lineweightSelector.addItem(QIcon("icons/" + icontheme + "/" + "lineweight13" + ".png"), "0.65 mm", 0.65)
        lineweightSelector.addItem(QIcon("icons/" + icontheme + "/" + "lineweight14" + ".png"), "0.70 mm", 0.70)
        lineweightSelector.addItem(QIcon("icons/" + icontheme + "/" + "lineweight15" + ".png"), "0.75 mm", 0.75)
        lineweightSelector.addItem(QIcon("icons/" + icontheme + "/" + "lineweight16" + ".png"), "0.80 mm", 0.80)
        lineweightSelector.addItem(QIcon("icons/" + icontheme + "/" + "lineweight17" + ".png"), "0.85 mm", 0.85)
        lineweightSelector.addItem(QIcon("icons/" + icontheme + "/" + "lineweight18" + ".png"), "0.90 mm", 0.90)
        lineweightSelector.addItem(QIcon("icons/" + icontheme + "/" + "lineweight19" + ".png"), "0.95 mm", 0.95)
        lineweightSelector.addItem(QIcon("icons/" + icontheme + "/" + "lineweight20" + ".png"), "1.00 mm", 1.00)
        lineweightSelector.addItem(QIcon("icons/" + icontheme + "/" + "lineweight21" + ".png"), "1.05 mm", 1.05)
        lineweightSelector.addItem(QIcon("icons/" + icontheme + "/" + "lineweight22" + ".png"), "1.10 mm", 1.10)
        lineweightSelector.addItem(QIcon("icons/" + icontheme + "/" + "lineweight23" + ".png"), "1.15 mm", 1.15)
        lineweightSelector.addItem(QIcon("icons/" + icontheme + "/" + "lineweight24" + ".png"), "1.20 mm", 1.20)
        lineweightSelector.setMinimumContentsLength(8)  # Prevent dropdown text readability being squish...d.
        toolbarProperties.addWidget(lineweightSelector)
        self.connect(lineweightSelector, SIGNAL("currentIndexChanged(int)"), self, SLOT("lineweightSelectorIndexChanged(int)"))

        self.connect(toolbarProperties, SIGNAL("topLevelChanged(bool)"), self, SLOT("floatingChangedToolBar(bool)"))


    def createTextToolbar(self):
        """
        TOWRITE
        """
        toolbarText = self.toolbarText
        actionHash = self.actionHash
        prompt = self.prompt
        textSizeSelector = self.textSizeSelector
        textFontSelector = self.textFontSelector
        qDebug("MainWindow createTextToolbar()")

        toolbarText.setObjectName("toolbarText")

        toolbarText.addWidget(textFontSelector)
        textFontSelector.setCurrentFont(QFont(self.getSettingsTextFont()))
        self.connect(textFontSelector, SIGNAL("currentFontChanged(QFont)"), self, SLOT("textFontSelectorCurrentFontChanged(QFont)"))

        toolbarText.addAction(actionHash["ACTION_textbold"])
        ## print('DEBUG', self.getSettingsTextStyleBold())
        #TODO/PORT TypeError# actionHash["ACTION_textbold"].setChecked(self.getSettingsTextStyleBold())
        toolbarText.addAction(actionHash["ACTION_textitalic"])
        #TODO/PORT TypeError# actionHash["ACTION_textitalic"].setChecked(self.getSettingsTextStyleItalic())
        toolbarText.addAction(actionHash["ACTION_textunderline"])
        #TODO/PORT TypeError# actionHash["ACTION_textunderline"].setChecked(self.getSettingsTextStyleUnderline())
        toolbarText.addAction(actionHash["ACTION_textstrikeout"])
        #TODO/PORT TypeError# actionHash["ACTION_textstrikeout"].setChecked(self.getSettingsTextStyleStrikeOut())
        toolbarText.addAction(actionHash["ACTION_textoverline"])
        #TODO/PORT TypeError# actionHash["ACTION_textoverline"].setChecked(self.getSettingsTextStyleOverline())

        textSizeSelector.setFocusProxy(prompt)
        textSizeSelector.addItem("6 pt",   6)
        textSizeSelector.addItem("8 pt",   8)
        textSizeSelector.addItem("9 pt",   9)
        textSizeSelector.addItem("10 pt", 10)
        textSizeSelector.addItem("11 pt", 11)
        textSizeSelector.addItem("12 pt", 12)
        textSizeSelector.addItem("14 pt", 14)
        textSizeSelector.addItem("18 pt", 18)
        textSizeSelector.addItem("24 pt", 24)
        textSizeSelector.addItem("30 pt", 30)
        textSizeSelector.addItem("36 pt", 36)
        textSizeSelector.addItem("48 pt", 48)
        textSizeSelector.addItem("60 pt", 60)
        textSizeSelector.addItem("72 pt", 72)
        #TODO/PORT# setTextSize(self.getSettingsTextSize())
        toolbarText.addWidget(textSizeSelector)
        self.connect(textSizeSelector, SIGNAL("currentIndexChanged(int)"), self, SLOT("textSizeSelectorIndexChanged(int)"))

        self.connect(toolbarText, SIGNAL("topLevelChanged(bool)"), self, SLOT("floatingChangedToolBar(bool)"))


    def createPromptToolbar(self):
        """
        TOWRITE
        """
        toolbarPrompt = self.toolbarPrompt
        prompt = self.prompt
        qDebug("MainWindow createPromptToolbar()")

        toolbarPrompt.setObjectName("toolbarPrompt")
        toolbarPrompt.addWidget(prompt)
        toolbarPrompt.setAllowedAreas(Qt.TopToolBarArea | Qt.BottomToolBarArea)
        self.connect(toolbarPrompt, SIGNAL("topLevelChanged(bool)"), prompt, SLOT("floatingChanged(bool)"))


    def createAllToolbars(self):
        """
        TOWRITE
        """
        qDebug("MainWindow createAllToolbars()")

        self.createFileToolbar()
        self.createEditToolbar()
        self.createViewToolbar()
        self.createZoomToolbar()
        self.createPanToolbar()
        self.createIconToolbar()
        self.createHelpToolbar()
        self.createLayerToolbar()
        self.createPropertiesToolbar()
        self.createTextToolbar()
        self.createPromptToolbar()

        # Horizontal
        self.toolbarView.setOrientation(Qt.Horizontal)
        self.toolbarZoom.setOrientation(Qt.Horizontal)
        self.toolbarLayer.setOrientation(Qt.Horizontal)
        self.toolbarProperties.setOrientation(Qt.Horizontal)
        self.toolbarText.setOrientation(Qt.Horizontal)
        self.toolbarPrompt.setOrientation(Qt.Horizontal)
        # Top
        self.addToolBarBreak(Qt.TopToolBarArea)
        self.addToolBar(Qt.TopToolBarArea, self.toolbarFile)
        self.addToolBar(Qt.TopToolBarArea, self.toolbarEdit)
        self.addToolBar(Qt.TopToolBarArea, self.toolbarHelp)
        self.addToolBar(Qt.TopToolBarArea, self.toolbarIcon)
        self.addToolBarBreak(Qt.TopToolBarArea)
        self.addToolBar(Qt.TopToolBarArea, self.toolbarZoom)
        self.addToolBar(Qt.TopToolBarArea, self.toolbarPan)
        self.addToolBar(Qt.TopToolBarArea, self.toolbarView)
        self.addToolBarBreak(Qt.TopToolBarArea)
        self.addToolBar(Qt.TopToolBarArea, self.toolbarLayer)
        self.addToolBar(Qt.TopToolBarArea, self.toolbarProperties)
        self.addToolBarBreak(Qt.TopToolBarArea)
        self.addToolBar(Qt.TopToolBarArea, self.toolbarText)
        # Bottom
        self.addToolBar(Qt.BottomToolBarArea, self.toolbarPrompt)

        #self.zoomToolBar.setToolButtonStyle(Qt.ToolButtonTextOnly)


    ##########################################################################
    # mainwindow_commands.py
    ##########################################################################


    def stub_implement(self, txt):
        """
        TOWRITE

        :param `txt`: TOWRITE
        :type `txt`: QString
        """
        qDebug("TODO: %s" % txt)

    def stub_testing(self):
        """
        TOWRITE
        """
        QMessageBox.warning(self, self.tr("Testing Feature"), self.tr("<b>This feature is in testing.</b>"))

    def exit(self):
        """
        TOWRITE
        """
        qDebug("exit()")
        if self.getSettingsPromptSaveHistory():

            self.prompt.saveHistory("prompt.log", self.getSettingsPromptSaveHistoryAsHtml())  # TODO: get filename from settings

        QApplication.closeAllWindows()
        self.deleteLater()  # Force the MainWindow destructor to run before exiting. Makes Valgrind "still reachable" happy :)

    def quit(self):
        """
        TOWRITE
        """
        qDebug("quit()")
        self.exit()

    def checkForUpdates(self):
        """
        TOWRITE
        """
        qDebug("checkForUpdates()")
        #  TODO: Check website for new versions, commands, etc...

    def cut(self):
        """
        TOWRITE
        """
        qDebug("cut()")
        self.gview = self.activeView()  # View*
        if self.gview:
            self.gview.cut()

    def copy(self):
        """
        TOWRITE
        """
        qDebug("copy()")
        self.gview = self.activeView()  # View*
        if self.gview:
            self.gview.copy()

    def paste(self):
        """
        TOWRITE
        """
        qDebug("paste()")
        self.gview = self.activeView()  # View*
        if self.gview:
            self.gview.paste()

    def selectAll(self):
        """
        TOWRITE
        """
        qDebug("selectAll()")
        self.gview = self.activeView()  # View*
        if self.gview:
            self.gview.selectAll()

    def platformString(self):
        """
        TOWRITE

        :rtype: QString
        """
        # TODO: Append QSysInfo to string where applicable.
        os = ""  # QString
        #if   defined(Q_OS_AIX)
        os = "AIX"
        #elif defined(Q_OS_BSD4)
        os = "BSD 4.4"
        #elif defined(Q_OS_BSDI)
        os = "BSD/OS"
        #elif defined(Q_OS_CYGWIN)
        os = "Cygwin"
        #elif defined(Q_OS_DARWIN)
        os = "Mac OS"
        #elif defined(Q_OS_DGUX)
        os = "DG/UX"
        #elif defined(Q_OS_DYNIX)
        os = "DYNIX/ptx"
        #elif defined(Q_OS_FREEBSD)
        os = "FreeBSD"
        #elif defined(Q_OS_HPUX)
        os = "HP-UX"
        #elif defined(Q_OS_HURD)
        os = "GNU Hurd"
        #elif defined(Q_OS_IRIX)
        os = "SGI Irix"
        #elif defined(Q_OS_LINUX)
        os = "Linux"
        #elif defined(Q_OS_LYNX)
        os = "LynxOS"
        #elif defined(Q_OS_MAC)
        os = "Mac OS"
        #elif defined(Q_OS_MSDOS)
        os = "MS-DOS"
        #elif defined(Q_OS_NETBSD)
        os = "NetBSD"
        #elif defined(Q_OS_OS2)
        os = "OS/2"
        #elif defined(Q_OS_OPENBSD)
        os = "OpenBSD"
        #elif defined(Q_OS_OS2EMX)
        os = "XFree86 on OS/2"
        #elif defined(Q_OS_OSF)
        os = "HP Tru64 UNIX"
        #elif defined(Q_OS_QNX)
        os = "QNX Neutrino"
        #elif defined(Q_OS_RELIANT)
        os = "Reliant UNIX"
        #elif defined(Q_OS_SCO)
        os = "SCO OpenServer 5"
        #elif defined(Q_OS_SOLARIS)
        os = "Sun Solaris"
        #elif defined(Q_OS_SYMBIAN)
        os = "Symbian"
        #elif defined(Q_OS_ULTRIX)
        os = "DEC Ultrix"
        #elif defined(Q_OS_UNIX)
        os = "UNIX BSD/SYSV"
        #elif defined(Q_OS_UNIXWARE)
        os = "UnixWare"
        #elif defined(Q_OS_WIN32)
        os = "Windows"
        #elif defined(Q_OS_WINCE)
        os = "Windows CE"
        #endif
        qDebug("Platform: %s" % os)
        return os

    def designDetails(self):
        """
        TOWRITE
        """
        self.scene = self.activeScene()  # QGraphicsScene*
        if self.scene:
            dialog = EmbDetailsDialog(self.scene, self)
            dialog.exec_()

    def about(self):
        """
        Show the Embroidermodder About Dialog.
        """
        dialog = AboutDialog(self)
        dialog.show()

        # TODO: QTabWidget for about dialog
        ## QApplication.setOverrideCursor(Qt.ArrowCursor);
        ## qDebug("about()");
        ## appName = QApplication.applicationName()  # QString
        ## title = "About " + appName  # QString
        ##
        ## dialog = QDialog(self)
        ## img = ImageWidget("images/logo-small")
        ## text = QLabel(appName +
        ##               self.tr("\n\n") +
        ##               self.tr("http://embroidermodder.github.io") +
        ##               self.tr("\n\n") +
        ##               self.tr("Available Platforms: GNU/Linux, Windows, Mac OSX, Raspberry Pi") +
        ##               self.tr("\n\n") +
        ##               self.tr("Embroidery formats by Josh Varga.") +
        ##               self.tr("\n") +
        ##               self.tr("User Interface by Jonathan Greig.") +
        ##               self.tr("\n\n") +
        ##               self.tr("Free under the zlib/libpng license."))
        ## text.setWordWrap(True)
        ##
        ## buttonbox = QDialogButtonBox(Qt.Horizontal, dialog)
        ## button = QPushButton(dialog)
        ## button.setText("Oh, Yeah!")
        ## buttonbox.addButton(button, QDialogButtonBox.AcceptRole)
        ## buttonbox.setCenterButtons(True)
        ## buttonbox.accepted.connect("accept()")  # connect(&buttonbox, SIGNAL(accepted()), &dialog, SLOT(accept()))
        ##
        ## layout = QVBoxLayout()
        ## layout.setAlignment(Qt.AlignCenter)
        ## layout.addWidget(img)
        ## layout.addWidget(text)
        ## layout.addWidget(buttonbox)
        ##
        ## dialog.setWindowTitle(title)
        ## dialog.setMinimumWidth(img.minimumWidth() + 30)
        ## dialog.setMinimumHeight(img.minimumHeight() + 50)
        ## dialog.setLayout(layout)
        ## dialog.exec_()
        ## QApplication.restoreOverrideCursor()

    def whatsThisContextHelp(self):
        """
        TOWRITE
        """
        qDebug("whatsThisContextHelp()")
        QWhatsThis.enterWhatsThisMode()

    def print_(self):
        """
        TOWRITE
        """
        qDebug("print()")
        mdiWin = self.mdiArea.activeSubWindow()  # MdiWindow* mdiWin = qobject_cast<MdiWindow*>(mdiArea->activeSubWindow());
        if mdiWin:
            mdiWin.print_()

    def tipOfTheDay(self):
        """Show the :class:`TipOfTheDayDialog`"""
        dialog = TipOfTheDayDialog(self)
        dialog.show()

        ## qDebug("tipOfTheDay()")
        ##
        ## wizardTipOfTheDay = QWizard(self)
        ## wizardTipOfTheDay.setAttribute(Qt.WA_DeleteOnClose)
        ## wizardTipOfTheDay.setWizardStyle(QWizard.ModernStyle)
        ## wizardTipOfTheDay.setMinimumSize(550, 400)
        ##
        ## page = QWizardPage(wizardTipOfTheDay)  # QWizardPage*
        ##
        ## imgBanner = ImageWidget("images/did-you-know.png", wizardTipOfTheDay)  # ImageWidget*
        ##
        ## if self.settings_general_current_tip >= len(self.listTipOfTheDay):
        ##     self.settings_general_current_tip = 0
        ## labelTipOfTheDay = QLabel(self.listTipOfTheDay[settings_general_current_tip], wizardTipOfTheDay)
        ## labelTipOfTheDay.setWordWrap(True)
        ##
        ## checkBoxTipOfTheDay = QCheckBox(tr("&Show tips on startup"), wizardTipOfTheDay)  # QCheckBox*
        ## settings_general_tip_of_the_day = mainWin.getSettingsGeneralTipOfTheDay()
        ## checkBoxTipOfTheDay.setChecked(settings_general_tip_of_the_day);
        ## #TODO/PORT# connect(checkBoxTipOfTheDay, SIGNAL(stateChanged(int)), this, SLOT(checkBoxTipOfTheDayStateChanged(int)))
        ##
        ## layout = QVBoxLayout(wizardTipOfTheDay)  # QVBoxLayout*
        ## layout.addWidget(imgBanner)
        ## layout.addStrut(1)
        ## layout.addWidget(labelTipOfTheDay)
        ## layout.addStretch(1)
        ## layout.addWidget(checkBoxTipOfTheDay)
        ## page.setLayout(layout)
        ## wizardTipOfTheDay.addPage(page)
        ##
        ## wizardTipOfTheDay.setWindowTitle("Tip of the Day")
        ##
        ## # TODO: Add icons to buttons by using wizardTipOfTheDay->setButton(QWizard::CustomButton1, buttonPrevious)
        ## # TODO: Add icons to buttons by using wizardTipOfTheDay->setButton(QWizard::CustomButton1, buttonNext)
        ## # TODO: Add icons to buttons by using wizardTipOfTheDay->setButton(QWizard::CustomButton1, buttonClose)
        ## wizardTipOfTheDay.setButtonText(QWizard.CustomButton1, tr("&Previous"))
        ## wizardTipOfTheDay.setButtonText(QWizard.CustomButton2, tr("&Next"))
        ## wizardTipOfTheDay.setButtonText(QWizard.CustomButton3, tr("&Close"))
        ## wizardTipOfTheDay.setOption(QWizard.HaveCustomButton1, True)
        ## wizardTipOfTheDay.setOption(QWizard.HaveCustomButton2, True)
        ## wizardTipOfTheDay.setOption(QWizard.HaveCustomButton3, True)
        ## #TODO/PORT# connect(wizardTipOfTheDay, SIGNAL(customButtonClicked(int)), this, SLOT(buttonTipOfTheDayClicked(int)));
        ##
        ## listTipOfTheDayButtons = []  # QList<QWizard::WizardButton> listTipOfTheDayButtons;
        ## ## listTipOfTheDayButtons << QWizard::Stretch << QWizard::CustomButton1 << QWizard::CustomButton2 << QWizard::CustomButton3;
        ## listTipOfTheDayButtons = [QWizard.Stretch, QWizard.CustomButton1, QWizard.CustomButton2, QWizard.CustomButton3]
        ## wizardTipOfTheDay.setButtonLayout(listTipOfTheDayButtons)
        ##
        ## wizardTipOfTheDay.exec_()

    def checkBoxTipOfTheDayStateChanged(self, checked):
        """
        TOWRITE

        :param `checked`: TOWRITE
        :type `checked`: int
        """
        self.settings_general_tip_of_the_day = checked

    def buttonTipOfTheDayClicked(self, button):
        """
        TOWRITE

        :param `button`: TOWRITE
        :type `button`: int
        """
        qDebug("buttonTipOfTheDayClicked(%d)", button)
        if button == QWizard.CustomButton1:
            if self.settings_general_current_tip > 0:
                self.settings_general_current_tip -= 1
            else:
                self.settings_general_current_tip = len(self.listTipOfTheDay) - 1
            labelTipOfTheDay.setText(self.listTipOfTheDay[settings_general_current_tip])

        elif button == QWizard.CustomButton2:
            self.settings_general_current_tip += 1
            if self.settings_general_current_tip >= len(listTipOfTheDay):
                self.settings_general_current_tip = 0
            labelTipOfTheDay.setText(listTipOfTheDay[settings_general_current_tip])

        elif button == QWizard.CustomButton3:
            wizardTipOfTheDay.close()

    def help(self):
        """
        TOWRITE
        """
        qDebug("help()")

        # Open the HTML Help in the default browser
        ## qDebug('QCoreApplication.applicationDirPath() = %s' % QCoreApplication.applicationDirPath())
        qDebug('self.gAppDir = %s' % self.gAppDir)
        # helpURL = QUrl("file:///" + QCoreApplication.applicationDirPath() + "/help/doc-index.html")
        helpURL = QUrl("file:///" + self.gAppDir + os.sep + "help" + os.sep + "doc-index.html")
        QDesktopServices.openUrl(helpURL)

        # TODO: This is how to start an external program. Use this elsewhere...
        # QString program = "firefox";
        # QStringList arguments;
        # arguments << "help/commands.html";
        # QProcess *myProcess = new QProcess(this);
        # myProcess->start(program, arguments);

    def changelog(self):
        """
        TOWRITE
        """
        qDebug("changelog()")

        changelogURL = QUrl("help/changelog.html")
        QDesktopServices.openUrl(changelogURL)


    # Standard Slots
    def undo(self):
        """
        TOWRITE
        """
        qDebug("undo()")
        prefix = self.prompt.getPrefix()  # QString
        if dockUndoEdit.canUndo():
            self.prompt.setPrefix("Undo " + dockUndoEdit.undoText())
            self.prompt.appendHistory("")
            self.dockUndoEdit.undo()
            self.prompt.setPrefix(prefix)

        else:
            self.prompt.alert("Nothing to undo")
            self.prompt.setPrefix(prefix)

    def redo(self):
        """
        TOWRITE
        """
        qDebug("redo()")
        prefix = self.prompt.getPrefix()  # QString
        if dockUndoEdit.canRedo():
            self.prompt.setPrefix("Redo " + dockUndoEdit.redoText())
            self.prompt.appendHistory("")
            self.dockUndoEdit.redo()
            self.prompt.setPrefix(prefix)

        else:
            self.prompt.alert("Nothing to redo")
            self.prompt.setPrefix(prefix)

    def isShiftPressed(self):
        """
        Return whether or not the "Shift" key is currently pressed.

        :rtype: bool
        """
        return self.shiftKeyPressedState

    def setShiftPressed(self):
        """
        Set the internal "Shift" key state to pressed.(True)
        """
        self.shiftKeyPressedState = True

    def setShiftReleased(self):
        """
        Set the internal "Shift" key state to unpressed.(False)
        """
        self.shiftKeyPressedState = False


    # Icons
    def iconResize(self, iconSize):
        """
        TOWRITE

        :param `iconSize`: TOWRITE
        :type `iconSize`: int
        """
        self.setIconSize(QSize(iconSize, iconSize))
        self.layerSelector.setIconSize(QSize(iconSize * 4, iconSize))
        self.colorSelector.setIconSize(QSize(iconSize, iconSize))
        self.linetypeSelector.setIconSize(QSize(iconSize * 4, iconSize))
        self.lineweightSelector.setIconSize(QSize(iconSize * 4, iconSize))
        # set the minimum combobox width so the text is always readable
        self.layerSelector.setMinimumWidth(iconSize * 4)
        self.colorSelector.setMinimumWidth(iconSize * 2)
        self.linetypeSelector.setMinimumWidth(iconSize * 4)
        self.lineweightSelector.setMinimumWidth(iconSize * 4)

        # TODO: low-priority: open app with iconSize set to 128. resize the icons to a smaller size.

        self.setSettingsGeneralIconSize(iconSize)

    def icon16(self):
        """
        TOWRITE
        """
        qDebug("icon16()")
        self.iconResize(16)

    def icon24(self):
        """
        TOWRITE
        """
        qDebug("icon24()")
        self.iconResize(24)

    def icon32(self):
        """
        TOWRITE
        """
        qDebug("icon32()")
        self.iconResize(32)

    def icon48(self):
        """
        TOWRITE
        """
        qDebug("icon48()")
        self.iconResize(48)

    def icon64(self):
        """
        TOWRITE
        """
        qDebug("icon64()")
        self.iconResize(64)

    def icon128(self):
        """
        TOWRITE
        """
        qDebug("icon128()")
        self.iconResize(128)

    def activeMdiWindow(self):
        """
        Get the active MDI Window

        :rtype: `MdiWindow`
        """
        qDebug("activeMdiWindow()")
        mdiWin = self.mdiArea.activeSubWindow()  # MdiWindow* mdiWin = qobject_cast<MdiWindow*>(mdiArea->activeSubWindow());
        return mdiWin

    def activeView(self):
        """
        Get the active View

        :rtype: `View`
        """
        qDebug("activeView()");
        mdiWin = self.mdiArea.activeSubWindow()  # MdiWindow* mdiWin = qobject_cast<MdiWindow*>(mdiArea->activeSubWindow());
        if mdiWin:

            v = mdiWin.getView()  # View*
            return v

        return 0

    def activeScene(self):
        """
        Get the active Scene.

        :rtype: `QGraphicsScene`_
        """
        qDebug("activeScene()")
        mdiWin = self.mdiArea.activeSubWindow()  # MdiWindow* mdiWin = qobject_cast<MdiWindow*>(mdiArea->activeSubWindow());
        if mdiWin:
            s = mdiWin.getScene()  # QGraphicsScene*
            return s

        return 0

    def activeUndoStack(self):
        """
        Get the active UndoStack.

        :rtype: `QUndoStack`_
        """
        qDebug("activeUndoStack()")
        v = self.activeView()  # View*
        if v:
            u = v.getUndoStack()  # QUndoStack*
            return u

        return 0

    def setUndoCleanIcon(self, opened):
        """
        TOWRITE

        :param `opened`: TOWRITE
        :type `opened`: bool
        """
        self.dockUndoEdit.updateCleanIcon(opened)

    def updateAllViewScrollBars(self, val):
        """
        TOWRITE

        :param `val`: TOWRITE
        :type `val`: bool
        """
        windowList = self.mdiArea.subWindowList()  # QList<QMdiSubWindow*>
        for i in range(0, len(windowList)):  # for(int i = 0; i < windowList.count(); ++i)
            mdiWin = windowList[i]  # MdiWindow* mdiWin = qobject_cast<MdiWindow*>(windowList.at(i));
            if mdiWin:
                mdiWin.showViewScrollBars(val)

    def updateAllViewCrossHairColors(self, color):
        """
        TOWRITE

        :param `color`: TOWRITE
        :type `color`: QRgb
        """
        windowList = self.mdiArea.subWindowList()  # QList<QMdiSubWindow*> windowList = mdiArea->subWindowList();
        for i in range(0, len(windowList)):  # for(int i = 0; i < windowList.count(); ++i)
            mdiWin = windowList[i]  # MdiWindow* mdiWin = qobject_cast<MdiWindow*>(windowList.at(i));
            if mdiWin:
                mdiWin.setViewCrossHairColor(color)

    def updateAllViewBackgroundColors(self, color):
        """
        TOWRITE

        :param `color`: TOWRITE
        :type `color`: QRgb
        """
        windowList = self.mdiArea.subWindowList()  # QList<QMdiSubWindow*>
        for i in range(0, len(windowList)):  # for(int i = 0; i < windowList.count(); ++i)
            mdiWin = windowList[i]  # MdiWindow* mdiWin = qobject_cast<MdiWindow*>(windowList.at(i));
            if mdiWin:
                mdiWin.setViewBackgroundColor(color)

    def updateAllViewSelectBoxColors(self, colorL, fillL, colorR, fillR, alpha):
        """
        TOWRITE

        :param `colorL`: TOWRITE
        :type `colorL`: QRgb
        :param `fillL`: TOWRITE
        :type `fillL`: QRgb
        :param `colorR`: TOWRITE
        :type `colorR`: QRgb
        :param `fillR`: TOWRITE
        :type `fillR`: QRgb
        :param `alpha`: TOWRITE
        :type `alpha`: int
        """
        windowList = self.mdiArea.subWindowList()  # QList<QMdiSubWindow*> windowList = mdiArea->subWindowList();
        for i in range(0, len(windowList)):  # for(int i = 0; i < windowList.count(); ++i)
            mdiWin = windowList[i]  # MdiWindow* mdiWin = qobject_cast<MdiWindow*>(windowList.at(i));
            if mdiWin:
                mdiWin.setViewSelectBoxColors(colorL, fillL, colorR, fillR, alpha)

    def updateAllViewGridColors(self, color):
        """
        TOWRITE

        :param `color`: TOWRITE
        :type `color`: QRgb
        """
        windowList = self.mdiArea.subWindowList()  # QList<QMdiSubWindow*>
        for i in range(0, len(windowList)):  # for(int i = 0; i < windowList.count(); ++i)
            mdiWin = windowList[i]  # MdiWindow* mdiWin = qobject_cast<MdiWindow*>(windowList.at(i));
            if mdiWin:
                mdiWin.setViewGridColor(color)

    def updateAllViewRulerColors(self, color):
        """
        TOWRITE

        :param `color`: TOWRITE
        :type `color`: QRgb
        """
        windowList = self.mdiArea.subWindowList()  # QList<QMdiSubWindow*> windowList = mdiArea->subWindowList();
        for i in range(0, len(windowList)):  #for(int i = 0; i < windowList.count(); ++i)
            mdiWin = windowList[i]  # MdiWindow* mdiWin = qobject_cast<MdiWindow*>(windowList.at(i));
            if mdiWin:
                mdiWin.setViewRulerColor(color)

    def updatePickAddMode(self, val):
        """
        TOWRITE

        :param `val`: TOWRITE
        :type `val`: bool
        """
        self.setSettingsSelectionModePickAdd(val)
        self.dockPropEdit.updatePickAddModeButton(val)

    def pickAddModeToggled(self):
        """
        TOWRITE
        """
        val = not self.getSettingsSelectionModePickAdd()  # bool
        self.updatePickAddMode(val)


    # Layer ToolBar
    def makeLayerActive(self):
        """
        TOWRITE
        """
        qDebug("makeLayerActive()")
        self.stub_implement("Implement makeLayerActive.")


    def layerManager(self):
        """
        TOWRITE
        """
        qDebug("layerManager()")
        self.stub_implement("Implement layerManager.")
        layman = LayerManager(self, self)
        layman.exec_()

    def layerPrevious(self):
        """
        TOWRITE
        """
        qDebug("layerPrevious()")
        self.stub_implement("Implement layerPrevious.")


    # Zoom ToolBar
    def zoomRealtime(self):
        """
        TOWRITE
        """
        qDebug("zoomRealtime()")
        self.stub_implement("Implement zoomRealtime.")

    def zoomPrevious(self):
        """
        TOWRITE
        """
        qDebug("zoomPrevious()")
        self.stub_implement("Implement zoomPrevious.")

    def zoomWindow(self):
        """
        TOWRITE
        """
        qDebug("zoomWindow()")
        gview = self.activeView()  # View*
        if gview:
            gview.zoomWindow()

    def zoomDynamic(self):
        """
        TOWRITE
        """
        qDebug("zoomDynamic()")
        self.stub_implement("Implement zoomDynamic.")

    def zoomScale(self):
        """
        TOWRITE
        """
        qDebug("zoomScale()")
        self.stub_implement("Implement zoomScale.")

    def zoomCenter(self):
        """
        TOWRITE
        """
        qDebug("zoomCenter()")
        self.stub_implement("Implement zoomCenter.")

    def zoomIn(self):
        """
        TOWRITE
        """
        qDebug("zoomIn()")
        gview = self.activeView()  # View*
        if gview:
            gview.zoomIn()

    def zoomOut(self):
        """
        TOWRITE
        """
        qDebug("zoomOut()")
        gview = self.activeView()  # View*
        if gview:
            gview.zoomOut()

    def zoomSelected(self):
        """
        TOWRITE
        """
        qDebug("zoomSelected()")
        gview = self.activeView()  # View*
        stack = gview.getUndoStack()  # QUndoStack*
        if gview and stack:
            cmd = UndoableNavCommand("ZoomSelected", gview, None)
            stack.push(cmd)

    def zoomAll(self):
        """
        TOWRITE
        """
        qDebug("zoomAll()")
        self.stub_implement("Implement zoomAll.")

    def zoomExtents(self):
        """
        TOWRITE
        """
        qDebug("zoomExtents()")
        gview = self.activeView()  # View*
        stack = gview.getUndoStack()  # QUndoStack*
        if gview and stack:
            cmd = UndoableNavCommand("ZoomExtents", gview, None)
            stack.push(cmd)


    # Pan SubMenu
    def panrealtime(self):
        """
        TOWRITE
        """
        qDebug("panrealtime()")
        gview = self.activeView()  # View*
        if gview:
            gview.panRealTime()

    def panpoint(self):
        """
        TOWRITE
        """
        qDebug("panpoint()")
        gview = self.activeView()  # View*
        if gview:
            gview.panPoint()

    def panLeft(self):
        """
        TOWRITE
        """
        qDebug("panLeft()")
        gview = self.activeView()  # View*
        stack = gview.getUndoStack()  # QUndoStack*
        if gview and stack:
            cmd = UndoableNavCommand("PanLeft", gview, None)
            stack.push(cmd)

    def panRight(self):
        """
        TOWRITE
        """
        qDebug("panRight()")
        gview = self.activeView()  # View*
        stack = gview.getUndoStack()  # QUndoStack*
        if gview and stack:
            cmd = UndoableNavCommand("PanRight", gview, None)
            stack.push(cmd)

    def panUp(self):
        """
        TOWRITE
        """
        qDebug("panUp()")
        gview = self.activeView()  # View*
        stack = gview.getUndoStack()  # QUndoStack*
        if gview and stack:
            cmd = UndoableNavCommand("PanUp", gview, None)
            stack.push(cmd)

    def panDown(self):
        """
        TOWRITE
        """
        qDebug("panDown()")
        gview = self.activeView()  # View*
        stack = gview.getUndoStack()  # QUndoStack*
        if gview and stack:
            cmd = UndoableNavCommand("PanDown", gview, None)
            stack.push(cmd)

    def dayVision(self):
        """
        TOWRITE
        """
        gview = self.activeView()  # View*
        if gview:
            gview.setBackgroundColor(qRgb(255, 255, 255))  # TODO: Make day vision color settings.
            gview.setCrossHairColor(qRgb(0, 0, 0))         # TODO: Make day vision color settings.
            gview.setGridColor(qRgb(0, 0, 0))              # TODO: Make day vision color settings.

    def nightVision(self):
        """
        TOWRITE
        """
        gview = self.activeView()  # View*
        if gview:
            gview.setBackgroundColor(qRgb(0, 0, 0))       # TODO: Make night vision color settings.
            gview.setCrossHairColor(qRgb(255, 255, 255))  # TODO: Make night vision color settings.
            gview.setGridColor(qRgb(255, 255, 255))       # TODO: Make night vision color settings.

    def doNothing(self):
        """
        This function intentionally does nothing.
        """
        qDebug("doNothing()")

    def layerSelectorIndexChanged(self, index):
        """
        TOWRITE

        :param `index`: TOWRITE
        :type `index`: int
        """
        qDebug("layerSelectorIndexChanged(%d)", index)

    def colorSelectorIndexChanged(self, index):
        """
        TOWRITE

        :param `index`: TOWRITE
        :type `index`: int
        """
        qDebug("colorSelectorIndexChanged(%d)", index)

        comboBox = self.sender()  # QComboBox* comboBox = qobject_cast<QComboBox*>(sender());
        newColor = QRgb()
        if comboBox:

            ok = 0  # bool
            # TODO: Handle ByLayer and ByBlock and Other...
            newColor = comboBox.itemData(index).toUInt(ok)
            if not ok:
                QMessageBox.warning(self, self.tr("Color Selector Conversion Error"), self.tr("<b>An error has occured while changing colors.</b>"))

        else:
            QMessageBox.warning(self, self.tr("Color Selector Pointer Error"), self.tr("<b>An error has occured while changing colors.</b>"))

        mdiWin = self.mdiArea.activeSubWindow()  # MdiWindow* mdiWin = qobject_cast<MdiWindow*>(mdiArea->activeSubWindow());
        if mdiWin:
            mdiWin.currentColorChanged(newColor)

    def linetypeSelectorIndexChanged(self, index):
        """
        TOWRITE

        :param `index`: TOWRITE
        :type `index`: int
        """
        qDebug("linetypeSelectorIndexChanged(%d)", index)

    def lineweightSelectorIndexChanged(self, index):
        """
        TOWRITE

        :param `index`: TOWRITE
        :type `index`: int
        """
        qDebug("lineweightSelectorIndexChanged(%d)", index)

    def textFontSelectorCurrentFontChanged(self, font):
        """
        TOWRITE

        :param `font`: TOWRITE
        :type `font`: `QFont`_
        """
        qDebug("textFontSelectorCurrentFontChanged()")
        self.setTextFont(font.family())

    def textSizeSelectorIndexChanged(self, index):
        """
        TOWRITE

        :param `index`: TOWRITE
        :type `index`: int
        """
        qDebug("textSizeSelectorIndexChanged(%d)", index)
        self.setSettingsTextSize(qFabs(textSizeSelector.itemData(index).toReal()))  # TODO: check that the toReal() conversion is ok

    def textFont(self):
        """
        TOWRITE

        :rtype: QString
        """
        return self.getSettingsTextFont()

    def textSize(self):
        """
        TOWRITE

        :rtype: qreal
        """
        return self.getSettingsTextSize()

    def textAngle(self):
        """
        TOWRITE

        :rtype: qreal
        """
        return self.getSettingsTextAngle()

    def textBold(self):
        """
        TOWRITE

        :rtype: bool
        """
        return self.getSettingsTextStyleBold()

    def textItalic(self):
        """
        TOWRITE

        :rtype: bool
        """
        return self.getSettingsTextStyleItalic()

    def textUnderline(self):
        """
        TOWRITE

        :rtype: bool
        """
        return self.getSettingsTextStyleUnderline()

    def textStrikeOut(self):
        """
        TOWRITE

        :rtype: bool
        """
        return self.getSettingsTextStyleStrikeOut()

    def textOverline(self):
        """
        TOWRITE

        :rtype: bool
        """
        return self.getSettingsTextStyleOverline()

    def setTextFont(self, strng):
        """
        TOWRITE

        :param `strng`: TOWRITE
        :type `strng`: QString
        """
        self.textFontSelector.setCurrentFont(QFont(strng))
        self.setSettingsTextFont(strng)

    def setTextSize(self, num):
        """
        TOWRITE

        :param `num`: TOWRITE
        :type `num`: qreal
        """
        self.setSettingsTextSize(qFabs(num))
        index = self.textSizeSelector.findText("Custom", Qt.MatchContains)  # int
        if index != -1:
            self.textSizeSelector.removeItem(index)
        self.textSizeSelector.addItem("Custom " + QString().setNum(num, 'f', 2) + " pt", num)
        index = self.textSizeSelector.findText("Custom", Qt.MatchContains)
        if index != -1:
            self.textSizeSelector.setCurrentIndex(index)

    def setTextAngle(self, num):
        """
        TOWRITE

        :param `num`: TOWRITE
        :type `num`: qreal
        """
        self.setSettingsTextAngle(num)

    def setTextBold(self, val):
        """
        TOWRITE

        :param `val`: TOWRITE
        :type `val`: bool
        """
        self.setSettingsTextStyleBold(val)

    def setTextItalic(self, val):
        """
        TOWRITE

        :param `val`: TOWRITE
        :type `val`: bool
        """
        self.setSettingsTextStyleItalic(val)

    def setTextUnderline(self, val):
        """
        TOWRITE

        :param `val`: TOWRITE
        :type `val`: bool
        """
        self.setSettingsTextStyleUnderline(val)

    def setTextStrikeOut(self, val):
        """
        TOWRITE

        :param `val`: TOWRITE
        :type `val`: bool
        """
        self.setSettingsTextStyleStrikeOut(val)

    def setTextOverline(self, val):
        """
        TOWRITE

        :param `val`: TOWRITE
        :type `val`: bool
        """
        self.setSettingsTextStyleOverline(val)

    def getCurrentLayer(self):
        """
        TOWRITE

        :rtype: QString
        """
        mdiWin = self.mdiArea.activeSubWindow()  # MdiWindow* mdiWin = qobject_cast<MdiWindow*>(mdiArea->activeSubWindow());
        if mdiWin:
            return mdiWin.getCurrentLayer()
        return "0"

    def getCurrentColor(self):
        """
        TOWRITE

        :rtype: QRgb
        """
        mdiWin = self.mdiArea.activeSubWindow()  # MdiWindow* mdiWin = qobject_cast<MdiWindow*>(mdiArea->activeSubWindow());
        if mdiWin:
            return mdiWin.getCurrentColor()
        return 0  # TODO: return color ByLayer

    def getCurrentLineType(self):
        """
        TOWRITE

        :rtype: QString
        """
        mdiWin = self.mdiArea.activeSubWindow()  # MdiWindow* mdiWin = qobject_cast<MdiWindow*>(mdiArea->activeSubWindow());
        if mdiWin:
            return mdiWin.getCurrentLineType()
        return "ByLayer"

    def getCurrentLineWeight(self):
        """
        TOWRITE

        :rtype: QString
        """
        mdiWin = self.mdiArea.activeSubWindow()  # MdiWindow* mdiWin = qobject_cast<MdiWindow*>(mdiArea->activeSubWindow());
        if mdiWin:
            return mdiWin.getCurrentLineWeight()
        return "ByLayer"

    def deletePressed(self):
        """
        TOWRITE
        """
        qDebug("deletePressed()")
        QApplication.setOverrideCursor(Qt.WaitCursor)
        mdiWin = self.mdiArea.activeSubWindow()  # MdiWindow* mdiWin = qobject_cast<MdiWindow*>(mdiArea->activeSubWindow());
        if mdiWin:
            mdiWin.deletePressed()
        QApplication.restoreOverrideCursor()

    def escapePressed(self):
        """
        TOWRITE
        """
        qDebug("escapePressed()")
        QApplication.setOverrideCursor(Qt.WaitCursor)
        mdiWin = self.mdiArea.activeSubWindow()  # MdiWindow* mdiWin = qobject_cast<MdiWindow*>(mdiArea->activeSubWindow());
        if mdiWin:
            mdiWin.escapePressed()
        QApplication.restoreOverrideCursor()

        self.nativeEndCommand()

    def toggleGrid(self):
        """
        TOWRITE
        """
        qDebug("toggleGrid()")
        self.statusbar.statusBarGridButton.toggle()

    def toggleRuler(self):
        """
        TOWRITE
        """
        qDebug("toggleRuler()")
        self.statusbar.statusBarRulerButton.toggle()

    def toggleLwt(self):
        """
        TOWRITE
        """
        qDebug("toggleLwt()")
        self.statusbar.statusBarLwtButton.toggle()

    def enablePromptRapidFire(self):
        """
        TOWRITE
        """
        self.prompt.enableRapidFire()

    def disablePromptRapidFire(self):
        """
        TOWRITE
        """
        self.prompt.disableRapidFire()

    def enableMoveRapidFire(self):
        """
        TOWRITE
        """
        gview = self.activeView()  # View*
        if gview:
            gview.enableMoveRapidFire()

    def disableMoveRapidFire(self):
        """
        TOWRITE
        """
        gview = self.activeView()  # View*
        if gview:
            gview.disableMoveRapidFire()

    def promptHistoryAppended(self, txt):
        """
        TOWRITE

        :param `txt`: TOWRITE
        :type `txt`: QString
        """
        mdiWin = self.activeMdiWindow()  #  MdiWindow*
        if mdiWin:
            mdiWin.promptHistoryAppended(txt)

    def logPromptInput(self, txt):
        """
        TOWRITE

        :param `txt`: TOWRITE
        :type `txt`: QString
        """
        mdiWin = self.activeMdiWindow()  # MdiWindow*
        if mdiWin:
            mdiWin.logPromptInput(txt)

    def promptInputPrevious(self):
        """
        TOWRITE
        """
        mdiWin = self.activeMdiWindow()  # MdiWindow*
        if mdiWin:
            mdiWin.promptInputPrevious()

    def promptInputNext(self):
        """
        TOWRITE
        """
        mdiWin = self.activeMdiWindow()  # MdiWindow*
        if mdiWin:
            mdiWin.promptInputNext()

    def runCommand(self):
        """
        TOWRITE
        """
        act = self.sender()  # QAction* act = qobject_cast<QAction*>(sender());
        if act:
            qDebug("runCommand(%s)", qPrintable(act.objectName()))
            prompt.endCommand()
            prompt.setCurrentText(act.objectName())
            prompt.processInput()

    def runCommandMain(self, cmd):
        """
        TOWRITE

        :param `cmd`: TOWRITE
        :type `cmd`: QString
        """
        qDebug("runCommandMain(%s)", qPrintable(cmd))
        fileName = "commands/" + cmd + "/" + cmd + ".js"  # QString
        # if not self.getSettingsSelectionModePickFirst(): self.nativeClearSelection()  # TODO: Uncomment this line when post-selection is available
        self.engine.evaluate(cmd + "_main()", fileName)

    def runCommandClick(self, cmd, x, y):
        """
        TOWRITE

        :param `cmd`: TOWRITE
        :type `cmd`: QString
        :param `x`: TOWRITE
        :type `x`: qreal
        :param `y`: TOWRITE
        :type `y`: qreal
        """
        qDebug("runCommandClick(%s, %.2f, %.2f)", qPrintable(cmd), x, y)
        fileName = "commands/" + cmd + "/" + cmd + ".js"  # QString
        self.engine.evaluate(cmd + "_click(" + QString().setNum(x) + "," + QString().setNum(-y) + ")", fileName)

    def runCommandMove(self, cmd, x, y):
        """
        TOWRITE

        :param `cmd`: TOWRITE
        :type `cmd`: QString
        :param `x`: TOWRITE
        :type `x`: qreal
        :param `y`: TOWRITE
        :type `y`: qreal
        """
        qDebug("runCommandMove(%s, %.2f, %.2f)", qPrintable(cmd), x, y)
        fileName = "commands/" + cmd + "/" + cmd + ".js"  # QString
        self.engine.evaluate(cmd + "_move(" + QString().setNum(x) + "," + QString().setNum(-y) + ")", fileName)

    def runCommandContext(self, cmd, strng):
        """
        TOWRITE

        :param `cmd`: TOWRITE
        :type `cmd`: QString
        :param `strng`: TOWRITE
        :type `strng`: QString
        """
        qDebug("runCommandContext(%s, %s)", qPrintable(cmd), qPrintable(strng))
        fileName = "commands/" + cmd + "/" + cmd + ".js"  # QString
        self.engine.evaluate(cmd + "_context('" + strng.toUpper() + "')", fileName)

    def runCommandPrompt(self, cmd, strng):
        """
        TOWRITE

        :param `cmd`: TOWRITE
        :type `cmd`: QString
        :param `strng`: TOWRITE
        :type `strng`: QString
        """
        qDebug("runCommandPrompt(%s, %s)", qPrintable(cmd), qPrintable(strng));
        fileName = "commands/" + cmd + "/" + cmd + ".js"  # QString
        # NOTE: Replace any special characters that will cause a syntax error
        safeStr = strng  # QString
        safeStr.replace("\\", "\\\\")
        safeStr.replace("\'", "\\\'")
        if self.prompt.isRapidFireEnabled():
            self.engine.evaluate(cmd + "_prompt('" + safeStr + "')", fileName)
        else:
            self.engine.evaluate(cmd + "_prompt('" + safeStr.toUpper() + "')", fileName)

    def nativeAlert(self, txt):
        """
        TOWRITE

        :param `txt`: TOWRITE
        :type `txt`: QString
        """
        self.prompt.alert(txt)

    def nativeBlinkPrompt(self):
        """
        TOWRITE
        """
        self.prompt.startBlinking()

    def nativeSetPromptPrefix(self, txt):
        """
        TOWRITE

        :param `txt`: TOWRITE
        :type `txt`: QString
        """
        self.prompt.setPrefix(txt)

    def nativeAppendPromptHistory(self, txt):
        """
        TOWRITE

        :param `txt`: TOWRITE
        :type `txt`: QString
        """
        self.prompt.appendHistory(txt)

    def nativeEnablePromptRapidFire(self):
        """
        TOWRITE
        """
        self.enablePromptRapidFire()

    def nativeDisablePromptRapidFire(self):
        """
        TOWRITE
        """
        self.disablePromptRapidFire()

    def nativeEnableMoveRapidFire(self):
        """
        TOWRITE
        """
        self.enableMoveRapidFire()

    def nativeDisableMoveRapidFire(self):
        """
        TOWRITE
        """
        self.disableMoveRapidFire()

    def nativeInitCommand(self):
        """
        TOWRITE
        """
        gview = self.activeView()  # View*
        if gview:
            gview.clearRubberRoom()

    def nativeEndCommand(self):
        """
        TOWRITE
        """
        gview = self.activeView()  # View*
        if gview:
            gview.clearRubberRoom()
            gview.previewOff()
            gview.disableMoveRapidFire()

        self.prompt.endCommand()

    def nativeNewFile(self):
        """
        TOWRITE
        """
        self.newFile()

    def nativeOpenFile(self):
        """
        TOWRITE
        """
        self.openFile()

    def nativeExit(self):
        """
        TOWRITE
        """
        self.exit()

    def nativeHelp(self):
        """
        TOWRITE
        """
        self.help()

    def nativeAbout(self):
        """
        TOWRITE
        """
        self.about()

    def nativeTipOfTheDay(self):
        """
        TOWRITE
        """
        self.tipOfTheDay()

    def nativeWindowCascade(self):
        """
        TOWRITE
        """
        self.mdiArea.cascade()

    def nativeWindowTile(self):
        """
        TOWRITE
        """
        self.mdiArea.tile()

    def nativeWindowClose(self):
        """
        TOWRITE
        """
        self.onCloseWindow()

    def nativeWindowCloseAll(self):
        """
        TOWRITE
        """
        self.mdiArea.closeAllSubWindows()

    def nativeWindowNext(self):
        """
        TOWRITE
        """
        self.mdiArea.activateNextSubWindow()

    def nativeWindowPrevious(self):
        """
        TOWRITE
        """
        self.mdiArea.activatePreviousSubWindow()

    def nativePlatformString(self):
        """
        TOWRITE

        :rtype: QString
        """
        return self.platformString()

    def nativeMessageBox(self, type, title, text):
        """
        Construct and show a message box. `QMessageBox`_ wrapper.

        :param `type`: Can be one of "critical", "information", "question", or "warning"
        :type `type`: QString
        :param `title`: The title of the message box to display.
        :type `title`: QString
        :param `text`: The body text of the message box to display.
        :type `text`: QString
        """
        msgType = type.lower()  # QString msgType = type.toLower()
        if   msgType == "critical":    QMessageBox.critical   (self, self.tr(qPrintable(title)), self.tr(qPrintable(text)))
        elif msgType == "information": QMessageBox.information(self, self.tr(qPrintable(title)), self.tr(qPrintable(text)))
        elif msgType == "question":    QMessageBox.question   (self, self.tr(qPrintable(title)), self.tr(qPrintable(text)))
        elif msgType == "warning":     QMessageBox.warning    (self, self.tr(qPrintable(title)), self.tr(qPrintable(text)))
        else:                          QMessageBox.critical   (self, self.tr("Native MessageBox Error"), self.tr("Incorrect use of the native messageBox function."))

    def nativeUndo(self):
        """
        TOWRITE
        """
        self.undo()

    def nativeRedo(self):
        """
        TOWRITE
        """
        self.redo()

    def nativeIcon16(self):
        """
        TOWRITE
        """
        self.icon16()

    def nativeIcon24(self):
        """
        TOWRITE
        """
        self.icon24()

    def nativeIcon32(self):
        """
        TOWRITE
        """
        self.icon32()

    def nativeIcon48(self):
        """
        TOWRITE
        """
        self.icon48()

    def nativeIcon64(self):
        """
        TOWRITE
        """
        self.icon64()

    def nativeIcon128(self):
        """
        TOWRITE
        """
        icon128()

    def nativePanLeft(self):
        """
        TOWRITE
        """
        self.panLeft()

    def nativePanRight(self):
        """
        TOWRITE
        """
        self.panRight()

    def nativePanUp(self):
        """
        TOWRITE
        """
        self.panUp()

    def nativePanDown(self):
        """
        TOWRITE
        """
        self.panDown()

    def nativeZoomIn(self):
        """
        TOWRITE
        """
        self.zoomIn()

    def nativeZoomOut(self):
        """
        TOWRITE
        """
        self.zoomOut()

    def nativeZoomExtents(self):
        """
        TOWRITE
        """
        self.zoomExtents()

    def nativePrintArea(self, x, y, w, h):
        """
        TOWRITE

        :param `x`: TOWRITE
        :type `x`: qreal
        :param `y`: TOWRITE
        :type `y`: qreal
        :param `w`: TOWRITE
        :type `w`: qreal
        :param `h`: TOWRITE
        :type `h`: qreal
        """
        qDebug("nativePrintArea(%.2f, %.2f, %.2f, %.2f)", x, y, w, h)
        # TODO: Print Setup Stuff
        self.print_()

    def nativeDayVision(self):
        """
        TOWRITE
        """
        self.dayVision()

    def nativeNightVision(self):
        """
        TOWRITE
        """
        self.nightVision()

    def nativeSetBackgroundColor(self, r, g, b):
        """
        TOWRITE

        :param `r`: Red value
        :type `r`: quint8
        :param `g`: Green value
        :type `g`: quint8
        :param `b`: Blue value
        :type `b`: quint8
        """
        self.setSettingsDisplayBGColor(qRgb(r, g, b))
        self.updateAllViewBackgroundColors(qRgb(r, g, b))

    def nativeSetCrossHairColor(self, r, g, b):
        """
        TOWRITE

        :param `r`: Red value
        :type `r`: quint8
        :param `g`: Green value
        :type `g`: quint8
        :param `b`: Blue value
        :type `b`: quint8
        """
        self.setSettingsDisplayCrossHairColor(qRgb(r, g, b))
        self.updateAllViewCrossHairColors(qRgb(r, g, b))

    def nativeSetGridColor(self, r, g, b):
        """
        TOWRITE

        :param `r`: Red value
        :type `r`: quint8
        :param `g`: Green value
        :type `g`: quint8
        :param `b`: Blue value
        :type `b`: quint8
        """
        self.setSettingsGridColor(qRgb(r, g, b))
        self.updateAllViewGridColors(qRgb(r, g, b))

    def nativeTextFont(self):
        """
        TOWRITE

        :rtype: QString
        """
        return self.textFont()

    def nativeTextSize(self):
        """
        TOWRITE

        :rtype: qreal
        """
        return self.textSize()

    def nativeTextAngle(self):
        """
        TOWRITE

        :rtype: qreal
        """
        return self.textAngle()

    def nativeTextBold(self):
        """
        TOWRITE

        :rtype: bool
        """
        return self.textBold()

    def nativeTextItalic(self):
        """
        TOWRITE

        :rtype: bool
        """
        return self.textItalic()

    def nativeTextUnderline(self):
        """
        TOWRITE

        :rtype: bool
        """
        return self.textUnderline()

    def nativeTextStrikeOut(self):
        """
        TOWRITE

        :rtype: bool
        """
        return self.textStrikeOut()

    def nativeTextOverline(self):
        """
        TOWRITE

        :rtype: bool
        """
        return self.textOverline()

    def nativeSetTextFont(self, strng):
        """
        TOWRITE

        :param `strng`: TOWRITE
        :type `strng`: QString
        """
        self.setTextFont(strng)

    def nativeSetTextSize(self, num):
        """
        TOWRITE

        :param `num`: TOWRITE
        :type `num`: qreal
        """
        self.setTextSize(num)

    def nativeSetTextAngle(self, num):
        """
        TOWRITE

        :param `num`: TOWRITE
        :type `num`: qreal
        """
        self.setTextAngle(num)

    def nativeSetTextBold(self, val):
        """
        TOWRITE

        :param `val`: TOWRITE
        :type `val`: bool
        """
        self.setTextBold(val)

    def nativeSetTextItalic(self, val):
        """
        TOWRITE

        :param `val`: TOWRITE
        :type `val`: bool
        """
        self.setTextItalic(val)

    def nativeSetTextUnderline(self, val):
        """
        TOWRITE

        :param `val`: TOWRITE
        :type `val`: bool
        """
        self.setTextUnderline(val)

    def nativeSetTextStrikeOut(self, val):
        """
        TOWRITE

        :param `val`: TOWRITE
        :type `val`: bool
        """
        self.setTextStrikeOut(val)

    def nativeSetTextOverline(self, val):
        """
        TOWRITE

        :param `val`: TOWRITE
        :type `val`: bool
        """
        self.setTextOverline(val)

    def nativePreviewOn(self, clone, mode, x, y, data):
        """
        TOWRITE

        :param `clone`: TOWRITE
        :type `clone`: int
        :param `mode`: TOWRITE
        :type `mode`: int
        :param `x`: TOWRITE
        :type `x`: qreal
        :param `y`: TOWRITE
        :type `y`: qreal
        :param `data`: TOWRITE
        :type `data`: qreal
        """
        gview = self.activeView()  # View*
        if gview:
            gview.previewOn(clone, mode, x, -y, data)

    def nativePreviewOff(self):
        """
        TOWRITE
        """
        gview = self.activeView()  # View*
        if gview:
            gview.previewOff()

    def nativeVulcanize(self):
        """
        TOWRITE
        """
        gview = self.activeView()  # View*
        if gview:
            gview.vulcanizeRubberRoom()

    def nativeClearRubber(self):
        """
        TOWRITE
        """
        gview = self.activeView()  # View*
        if gview:
            gview.clearRubberRoom()

    def nativeAllowRubber(self):
        """
        TOWRITE

        :rtype: bool
        """
        gview = self.activeView()  # View*
        if gview:
            return gview.allowRubber()
        return False

    def nativeSpareRubber(self, id):
        """
        TOWRITE

        :param `id`: TOWRITE
        :type `id`: qint64
        """
        gview = self.activeView()  # View*
        if gview:
            gview.spareRubber(id)

    def nativeSetRubberMode(self, mode):
        """
        TOWRITE

        :param `mode`: TOWRITE
        :type `mode`: int
        """
        gview = self.activeView()  # View*
        if gview:
            gview.setRubberMode(mode)

    def nativeSetRubberPoint(self, key, x, y):
        """
        TOWRITE

        :param `key`: TOWRITE
        :type `key`: QString
        :param `x`: TOWRITE
        :type `x`: qreal
        :param `y`: TOWRITE
        :type `y`: qreal
        """
        gview = self.activeView()  # View*
        if gview:
            gview.setRubberPoint(key, QPointF(x, -y))

    def nativeSetRubberText(self, key, txt):
        """
        TOWRITE

        :param `key`: TOWRITE
        :type `key`: QString
        :param `txt`: TOWRITE
        :type `txt`: QString
        """
        gview = self.activeView()  # View*
        if gview:
            gview.setRubberText(key, txt)

    def nativeAddTextMulti(self, strng, x, y, rot, fill, rubberMode):
        """
        TOWRITE

        :param `strng`: TOWRITE
        :type `strng`: QString
        :param `x`: TOWRITE
        :type `x`: qreal
        :param `y`: TOWRITE
        :type `y`: qreal
        :param `rot`: TOWRITE
        :type `rot`: qreal
        :param `fill`: TOWRITE
        :type `fill`: bool
        :param `rubberMode`: TOWRITE
        :type `rubberMode`: int
        """
        pass #TODO/PORT# This is empty

    def nativeAddTextSingle(self, strng, x, y, rot, fill, rubberMode):
        """
        TOWRITE

        :param `strng`: TOWRITE
        :type `strng`: QString
        :param `x`: TOWRITE
        :type `x`: qreal
        :param `y`: TOWRITE
        :type `y`: qreal
        :param `rot`: TOWRITE
        :type `rot`: qreal
        :param `fill`: TOWRITE
        :type `fill`: bool
        :param `rubberMode`: TOWRITE
        :type `rubberMode`: int
        """
        gview = self.activeView()  # View*
        gscene = gview.scene()  # QGraphicsScene*
        stack = gview.getUndoStack()  # QUndoStack*
        if gview and gscene and stack:

            obj = TextSingleObject(strng, x, -y, getCurrentColor())  # TextSingleObject*
            obj.setObjectTextFont(self.getSettingsTextFont())
            obj.setObjectTextSize(self.getSettingsTextSize())
            obj.setObjectTextStyle(self.getSettingsTextStyleBold(),
                                   self.getSettingsTextStyleItalic(),
                                   self.getSettingsTextStyleUnderline(),
                                   self.getSettingsTextStyleStrikeOut(),
                                   self.getSettingsTextStyleOverline())
            obj.setObjectTextBackward(False)
            obj.setObjectTextUpsideDown(False)
            obj.setRotation(-rot)
            # TODO: single line text fill
            obj.setObjectRubberMode(rubberMode)
            if rubberMode:
                gview.addToRubberRoom(obj)
                gscene.addItem(obj)
                gscene.update()

            else:
                cmd = UndoableAddCommand(obj.data("OBJ_NAME"), obj, gview, None)
                stack.push(cmd)

    def nativeAddInfiniteLine(self, x1, y1, x2, y2, rot):
        """
        TOWRITE

        :param `x1`: TOWRITE
        :type `x1`: qreal
        :param `y1`: TOWRITE
        :type `y1`: qreal
        :param `x2`: TOWRITE
        :type `x2`: qreal
        :param `y2`: TOWRITE
        :type `y2`: qreal
        :param `rot`: TOWRITE
        :type `rot`: qreal
        """
        pass #TODO/PORT# This is empty

    def nativeAddRay(self, x1, y1, x2, y2, rot):
        """
        TOWRITE

        :param `x1`: TOWRITE
        :type `x1`: qreal
        :param `y1`: TOWRITE
        :type `y1`: qreal
        :param `x2`: TOWRITE
        :type `x2`: qreal
        :param `y2`: TOWRITE
        :type `y2`: qreal
        :param `rot`: TOWRITE
        :type `rot`: qreal
        """
        pass #TODO/PORT# This is empty

    def nativeAddLine(self, x1, y1, x2, y2, rot, rubberMode):
        """
        TOWRITE

        :param `x1`: TOWRITE
        :type `x1`: qreal
        :param `y1`: TOWRITE
        :type `y1`: qreal
        :param `x2`: TOWRITE
        :type `x2`: qreal
        :param `y2`: TOWRITE
        :type `y2`: qreal
        :param `rot`: TOWRITE
        :type `rot`: qreal
        :param `rubberMode`: TOWRITE
        :type `rubberMode`: int
        """
        gview = self.activeView()  # View*
        gscene = gview.scene()  # QGraphicsScene*
        stack = gview.getUndoStack()  # QUndoStack*
        if gview and gscene and stack:

            obj = LineObject(x1, -y1, x2, -y2, self.getCurrentColor())  # LineObject*
            obj.setRotation(-rot)
            obj.setObjectRubberMode(rubberMode)
            if rubberMode:
                gview.addToRubberRoom(obj)
                gscene.addItem(obj)
                gscene.update()

            else:
                cmd = UndoableAddCommand(obj.data("OBJ_NAME"), obj, gview, None)
                stack.push(cmd)

    def nativeAddTriangle(self, x1, y1, x2, y2, x3, y3, rot, fill):
        """
        TOWRITE

        :param `x1`: TOWRITE
        :type `x1`: qreal
        :param `y1`: TOWRITE
        :type `y1`: qreal
        :param `x2`: TOWRITE
        :type `x2`: qreal
        :param `y2`: TOWRITE
        :type `y2`: qreal
        :param `x3`: TOWRITE
        :type `x3`: qreal
        :param `y3`: TOWRITE
        :type `y3`: qreal
        :param `rot`: TOWRITE
        :type `rot`: qreal
        :param `fill`: TOWRITE
        :type `fill`: bool
        """
        pass #TODO/PORT# This is empty

    def nativeAddRectangle(self, x, y, w, h, rot, fill, rubberMode):
        """
        TOWRITE

        :param `x`: TOWRITE
        :type `x`: qreal
        :param `y`: TOWRITE
        :type `y`: qreal
        :param `w`: TOWRITE
        :type `w`: qreal
        :param `h`: TOWRITE
        :type `h`: qreal
        :param `rot`: TOWRITE
        :type `rot`: qreal
        :param `fill`: TOWRITE
        :type `fill`: bool
        :param `rubberMode`: TOWRITE
        :type `rubberMode`: int
        """
        gview = self.activeView()  # View*
        gscene = gview.scene()  # QGraphicsScene*
        stack = gview.getUndoStack()  # QUndoStack*
        if gview and gscene and stack:

            obj = RectObject(x, -y, w, -h, self.getCurrentColor())  # RectObject*
            obj.setRotation(-rot)
            obj.setObjectRubberMode(rubberMode)
            # TODO: rect fill
            if rubberMode:
                gview.addToRubberRoom(obj)
                gscene.addItem(obj)
                gscene.update()

            else:
                cmd = UndoableAddCommand(obj.data("OBJ_NAME"), obj, gview, None)
                stack.push(cmd)

    def nativeAddRoundedRectangle(self, x, y, w, h, rad, rot, fill):
        """
        TOWRITE

        :param `x`: TOWRITE
        :type `x`: qreal
        :param `y`: TOWRITE
        :type `y`: qreal
        :param `w`: TOWRITE
        :type `w`: qreal
        :param `h`: TOWRITE
        :type `h`: qreal
        :param `rad`: TOWRITE
        :type `rad`: qreal
        :param `rot`: TOWRITE
        :type `rot`: qreal
        :param `fill`: TOWRITE
        :type `fill`: bool
        """
        pass #TODO/PORT# This is empty

    def nativeAddArc(self, startX, startY, midX, midY, endX, endY, rubberMode):
        """
        TOWRITE

        :param `startX`: TOWRITE
        :type `startX`: qreal
        :param `startY`: TOWRITE
        :type `startY`: qreal
        :param `midX`: TOWRITE
        :type `midX`: qreal
        :param `midY`: TOWRITE
        :type `midY`: qreal
        :param `endX`: TOWRITE
        :type `endX`: qreal
        :param `endY`: TOWRITE
        :type `endY`: qreal
        :param `rubberMode`: TOWRITE
        :type `rubberMode`: int
        """
        gview = self.activeView()  # View*
        scene = self.activeScene()  # QGraphicsScene*
        if gview and scene:
            arcObj = ArcObject(startX, -startY, midX, -midY, endX, -endY, self.getCurrentColor())  # ArcObject*
            arcObj.setObjectRubberMode(rubberMode)
            if rubberMode:
                gview.addToRubberRoom(arcObj)
            scene.addItem(arcObj)
            scene.update()

    def nativeAddCircle(self, centerX, centerY, radius, fill, rubberMode):
        """
        TOWRITE

        :param `centerX`: TOWRITE
        :type `centerX`: qreal
        :param `centerY`: TOWRITE
        :type `centerY`: qreal
        :param `radius`: TOWRITE
        :type `radius`: qreal
        :param `fill`: TOWRITE
        :type `fill`: bool
        :param `rubberMode`: TOWRITE
        :type `rubberMode`: int
        """
        gview = self.activeView()  # View*
        gscene = gview.scene()  # QGraphicsScene*
        stack = gview.getUndoStack()  # QUndoStack*
        if gview and gscene and stack:
            obj = CircleObject(centerX, -centerY, radius, self.getCurrentColor())  # CircleObject*
            obj.setObjectRubberMode(rubberMode)
            # TODO: circle fill
            if rubberMode:
                gview.addToRubberRoom(obj)
                gscene.addItem(obj)
                gscene.update()

            else:
                cmd = UndoableAddCommand(obj.data("OBJ_NAME"), obj, gview, None)
                stack.push(cmd)

    def nativeAddSlot(self, centerX, centerY, diameter, length, rot, fill, rubberMode):
        """
        TOWRITE

        :param `centerX`: TOWRITE
        :type `centerX`: qreal
        :param `centerY`: TOWRITE
        :type `centerY`: qreal
        :param `diameter`: TOWRITE
        :type `diameter`: qreal
        :param `length`: TOWRITE
        :type `length`: qreal
        :param `rot`: TOWRITE
        :type `rot`: qreal
        :param `fill`: TOWRITE
        :type `fill`: bool
        :param `rubberMode`: TOWRITE
        :type `rubberMode`: int
        """
        pass #TODO/PORT#
        # //TODO: Use UndoableAddCommand for slots
        # /*
        # SlotObject* slotObj = new SlotObject(centerX, -centerY, diameter, length, getCurrentColor());
        # slotObj->setRotation(-rot);
        # slotObj->setObjectRubberMode(rubberMode);
        # if(rubberMode) gview->addToRubberRoom(slotObj);
        # scene->addItem(slotObj);
        # //TODO: slot fill
        # scene->update();
        # */

    def nativeAddEllipse(self, centerX, centerY, width, height, rot, fill, rubberMode):
        """
        TOWRITE

        :param `centerX`: TOWRITE
        :type `centerX`: qreal
        :param `centerY`: TOWRITE
        :type `centerY`: qreal
        :param `width`: TOWRITE
        :type `width`: qreal
        :param `height`: TOWRITE
        :type `height`: qreal
        :param `rot`: TOWRITE
        :type `rot`: qreal
        :param `fill`: TOWRITE
        :type `fill`: bool
        :param `rubberMode`: TOWRITE
        :type `rubberMode`: int
        """
        gview = self.activeView()  # View*
        gscene = gview.scene()  # QGraphicsScene*
        stack = gview.getUndoStack()  # QUndoStack*
        if gview and gscene and stack:
            obj = EllipseObject(centerX, -centerY, width, height, self.getCurrentColor())  # EllipseObject*
            obj.setRotation(-rot)
            obj.setObjectRubberMode(rubberMode)
            # TODO: ellipse fill
            if rubberMode:
                gview.addToRubberRoom(obj)
                gscene.addItem(obj)
                gscene.update()

            else:
                cmd = UndoableAddCommand(obj.data("OBJ_NAME"), obj, gview, None)
                stack.push(cmd)

    def nativeAddPoint(self, x, y):
        """
        TOWRITE

        :param `x`: TOWRITE
        :type `x`: qreal
        :param `y`: TOWRITE
        :type `y`: qreal
        """
        gview = self.activeView()  # View*
        stack = gview.getUndoStack()  # QUndoStack*
        if gview and stack:
            obj = PointObject(x, -y, self.getCurrentColor())  # PointObject*
            cmd = UndoableAddCommand(obj.data("OBJ_NAME"), obj, gview, None)
            stack.push(cmd)

    def nativeAddRegularPolygon(self, centerX, centerY, sides, mode, rad, rot, fill):
        """
        TOWRITE

        :param `centerX`: TOWRITE
        :type `centerX`: qreal
        :param `centerY`: TOWRITE
        :type `centerY`: qreal
        :param `sides`: TOWRITE
        :type `sides`: quint16
        :param `mode`: TOWRITE
        :type `mode`: quint8
        :param `rad`: TOWRITE
        :type `rad`: qreal
        :param `rot`: TOWRITE
        :type `rot`: qreal
        :param `fill`: TOWRITE
        :type `fill`: bool
        """
        pass #TODO/PORT# This is empty

    def nativeAddPolygon(self, startX, startY, p, rubberMode):
        """
        TOWRITE

        :param `startX`: TOWRITE
        :type `startX`: qreal
        :param `startY`: TOWRITE
        :type `startY`: qreal
        :param `p`: TOWRITE
        :type `p`: `QPainterPath`_
        :param `rubberMode`: TOWRITE
        :type `rubberMode`: int

        .. NOTE:: This native is different than the rest in that the Y+ is down (scripters need not worry about this)
        """
        gview = self.activeView()  # View*
        gscene = gview.scene()  # QGraphicsScene*
        stack = gview.getUndoStack()  # QUndoStack*
        if gview and gscene and stack:
            obj = PolygonObject(startX, startY, p, self.getCurrentColor())
            obj.setObjectRubberMode(rubberMode)
            if rubberMode:
                gview.addToRubberRoom(obj)
                gscene.addItem(obj)
                gscene.update()

            else:
                cmd = UndoableAddCommand(obj.data("OBJ_NAME"), obj, gview, None)
                stack.push(cmd)

    def nativeAddPolyline(self, startX, startY, p, rubberMode):
        """
        TOWRITE

        :param `startX`: TOWRITE
        :type `startX`: qreal
        :param `startY`: TOWRITE
        :type `startY`: qreal
        :param `p`: TOWRITE
        :type `p`: `QPainterPath`_
        :param `rubberMode`: TOWRITE
        :type `rubberMode`: int

        .. NOTE:: This native is different than the rest in that the Y+ is down (scripters need not worry about this)
        """
        gview = self.activeView()  # View*
        gscene = gview.scene()  # QGraphicsScene*
        stack = gview.getUndoStack()  # QUndoStack*
        if gview and gscene and stack:
            obj = PolylineObject(startX, startY, p, self.getCurrentColor())
            obj.setObjectRubberMode(rubberMode)
            if rubberMode:
                gview.addToRubberRoom(obj)
                gscene.addItem(obj)
                gscene.update()

            else:
                cmd = UndoableAddCommand(obj.data("OBJ_NAME"), obj, gview, None)
                stack.push(cmd)

    def nativeAddPath(self, startX, startY, p, rubberMode):
        """
        TOWRITE

        :param `startX`: TOWRITE
        :type `startX`: qreal
        :param `startY`: TOWRITE
        :type `startY`: qreal
        :param `p`: TOWRITE
        :type `p`: `QPainterPath`_
        :param `rubberMode`: TOWRITE
        :type `rubberMode`: int

        .. NOTE:: This native is different than the rest in that the Y+ is down (scripters need not worry about this)
        """
        pass #TODO/PORT# This is empty

    def nativeAddHorizontalDimension(self, x1, y1, x2, y2, legHeight):
        """
        TOWRITE

        :param `x1`: TOWRITE
        :type `x1`: qreal
        :param `y1`: TOWRITE
        :type `y1`: qreal
        :param `x2`: TOWRITE
        :type `x2`: qreal
        :param `y2`: TOWRITE
        :type `y2`: qreal
        :param `legHeight`: TOWRITE
        :type `legHeight`: qreal
        """
        pass #TODO/PORT# This is empty

    def nativeAddVerticalDimension(self, x1, y1, x2, y2, legHeight):
        """
        TOWRITE

        :param `x1`: TOWRITE
        :type `x1`: qreal
        :param `y1`: TOWRITE
        :type `y1`: qreal
        :param `x2`: TOWRITE
        :type `x2`: qreal
        :param `y2`: TOWRITE
        :type `y2`: qreal
        :param `legHeight`: TOWRITE
        :type `legHeight`: qreal
        """
        pass #TODO/PORT# This is empty

    def nativeAddImage(self, img, x, y, w, h, rot):
        """
        TOWRITE

        :param `img`: TOWRITE
        :type `img`: QString
        :param `x`: TOWRITE
        :type `x`: qreal
        :param `y`: TOWRITE
        :type `y`: qreal
        :param `w`: TOWRITE
        :type `w`: qreal
        :param `h`: TOWRITE
        :type `h`: qreal
        :param `rot`: TOWRITE
        :type `rot`: qreal
        """
        pass #TODO/PORT# This is empty

    def nativeAddDimLeader(self, x1, y1, x2, y2, rot, rubberMode):
        """
        TOWRITE

        :param `x1`: TOWRITE
        :type `x1`: qreal
        :param `y1`: TOWRITE
        :type `y1`: qreal
        :param `x2`: TOWRITE
        :type `x2`: qreal
        :param `y2`: TOWRITE
        :type `y2`: qreal
        :param `rot`: TOWRITE
        :type `rot`: qreal
        :param `rubberMode`: TOWRITE
        :type `rubberMode`: int
        """
        gview = self.activeView()  # View*
        gscene = gview.scene()  # QGraphicsScene*
        stack = gview.getUndoStack()  # QUndoStack*
        if gview and gscene and stack:
            obj = DimLeaderObject(x1, -y1, x2, -y2, self.getCurrentColor())
            obj.setRotation(-rot)
            obj.setObjectRubberMode(rubberMode)
            if rubberMode:
                gview.addToRubberRoom(obj)
                gscene.addItem(obj)
                gscene.update()

            else:
                cmd = UndoableAddCommand(obj.data("OBJ_NAME"), obj, gview, None)
                stack.push(cmd)

    def nativeSetCursorShape(self, strng):
        """
        TOWRITE

        :param `strng`: TOWRITE
        :type `strng`: QString
        """
        gview = self.activeView()  # View*
        if gview:

            shape = strng.lower()  # QString shape = str.toLower();
            if   shape == "arrow":           gview.setCursor(QCursor(Qt.ArrowCursor))
            elif shape == "uparrow":         gview.setCursor(QCursor(Qt.UpArrowCursor))
            elif shape == "cross":           gview.setCursor(QCursor(Qt.CrossCursor))
            elif shape == "wait":            gview.setCursor(QCursor(Qt.WaitCursor))
            elif shape == "ibeam":           gview.setCursor(QCursor(Qt.IBeamCursor))
            elif shape == "resizevert":      gview.setCursor(QCursor(Qt.SizeVerCursor))
            elif shape == "resizehoriz":     gview.setCursor(QCursor(Qt.SizeHorCursor))
            elif shape == "resizediagleft":  gview.setCursor(QCursor(Qt.SizeBDiagCursor))
            elif shape == "resizediagright": gview.setCursor(QCursor(Qt.SizeFDiagCursor))
            elif shape == "move":            gview.setCursor(QCursor(Qt.SizeAllCursor))
            elif shape == "blank":           gview.setCursor(QCursor(Qt.BlankCursor))
            elif shape == "splitvert":       gview.setCursor(QCursor(Qt.SplitVCursor))
            elif shape == "splithoriz":      gview.setCursor(QCursor(Qt.SplitHCursor))
            elif shape == "handpointing":    gview.setCursor(QCursor(Qt.PointingHandCursor))
            elif shape == "forbidden":       gview.setCursor(QCursor(Qt.ForbiddenCursor))
            elif shape == "handopen":        gview.setCursor(QCursor(Qt.OpenHandCursor))
            elif shape == "handclosed":      gview.setCursor(QCursor(Qt.ClosedHandCursor))
            elif shape == "whatsthis":       gview.setCursor(QCursor(Qt.WhatsThisCursor))
            elif shape == "busy":            gview.setCursor(QCursor(Qt.BusyCursor))
            #if QT_VERSION >= 0x040700
            elif shape == "dragmove":        gview.setCursor(QCursor(Qt.DragMoveCursor))
            elif shape == "dragcopy":        gview.setCursor(QCursor(Qt.DragCopyCursor))
            elif shape == "draglink":        gview.setCursor(QCursor(Qt.DragLinkCursor))
            #endif


    def nativeCalculateAngle(self, x1, y1, x2, y2):
        """
        TOWRITE

        :param `x1`: TOWRITE
        :type `x1`: qreal
        :param `y1`: TOWRITE
        :type `y1`: qreal
        :param `x2`: TOWRITE
        :type `x2`: qreal
        :param `y2`: TOWRITE
        :type `y2`: qreal
        :rtype: qreal
        """
        return QLineF(x1, -y1, x2, -y2).angle()

    def nativeCalculateDistance(self, x1, y1, x2, y2):
        """
        TOWRITE

        :param `x1`: TOWRITE
        :type `x1`: qreal
        :param `y1`: TOWRITE
        :type `y1`: qreal
        :param `x2`: TOWRITE
        :type `x2`: qreal
        :param `y2`: TOWRITE
        :type `y2`: qreal
        :rtype: qreal
        """
        return QLineF(x1, y1, x2, y2).length()

    def nativePerpendicularDistance(self, px, py, x1, y1, x2, y2):
        """
        TOWRITE

        :param `px`: TOWRITE
        :type `px`: qreal
        :param `py`: TOWRITE
        :type `py`: qreal
        :param `x1`: TOWRITE
        :type `x1`: qreal
        :param `y1`: TOWRITE
        :type `y1`: qreal
        :param `x2`: TOWRITE
        :type `x2`: qreal
        :param `y2`: TOWRITE
        :type `y2`: qreal
        :rtype: qreal
        """
        line = QLineF(x1, y1, x2, y2)
        norm = line.normalVector()  # QLineF
        dx = px - x1  # qreal
        dy = py - y1  # qreal
        norm.translate(dx, dy)
        iPoint = QPointF()
        norm.intersect(line, iPoint)
        return QLineF(px, py, iPoint.x(), iPoint.y()).length()

    def nativeNumSelected(self):
        """
        TOWRITE

        :rtype: int
        """
        gview = self.activeView()  # View*
        if gview:
            return gview.numSelected()
        return 0

    def nativeSelectAll(self):
        """
        TOWRITE
        """
        gview = self.activeView()  # View*
        if gview:
            gview.selectAll()

    def nativeAddToSelection(self, path, mode):
        """
        TOWRITE

        :param `path`: TOWRITE
        :type `path`: `QPainterPath`_
        :param `mode`: TOWRITE
        :type `mode`: Qt.ItemSelectionMode
        """
        pass #TODO/PORT# This is empty

    def nativeClearSelection(self):
        """
        TOWRITE
        """
        gview = self.activeView()  # View*
        if gview:
            gview.clearSelection()

    def nativeDeleteSelected(self):
        """
        TOWRITE
        """
        gview = self.activeView()  # View*
        if gview:
            gview.deleteSelected()

    def nativeCutSelected(self, x, y):
        """
        TOWRITE

        :param `x`: TOWRITE
        :type `x`: qreal
        :param `y`: TOWRITE
        :type `y`: qreal
        """
        pass #TODO/PORT# This is empty

    def nativeCopySelected(self, x, y):
        """
        TOWRITE

        :param `x`: TOWRITE
        :type `x`: qreal
        :param `y`: TOWRITE
        :type `y`: qreal
        """
        pass #TODO/PORT# This is empty

    def nativePasteSelected(self, x, y):
        """
        TOWRITE

        :param `x`: TOWRITE
        :type `x`: qreal
        :param `y`: TOWRITE
        :type `y`: qreal
        """
        pass #TODO/PORT# This is empty

    def nativeMoveSelected(self, dx, dy):
        """
        TOWRITE

        :param `dx`: TOWRITE
        :type `dx`: qreal
        :param `dy`: TOWRITE
        :type `dy`: qreal
        """
        gview = self.activeView()  # View*
        if gview:
            gview.moveSelected(dx, -dy)

    def nativeScaleSelected(self, x, y, factor):
        """
        TOWRITE

        :param `x`: TOWRITE
        :type `x`: qreal
        :param `y`: TOWRITE
        :type `y`: qreal
        :param `factor`: TOWRITE
        :type `factor`: qreal
        """
        if factor <= 0.0:
            QMessageBox.critical(self, self.tr("ScaleFactor Error"),
                                 self.tr("Hi there. If you are not a developer, report this as a bug. "
                                 "If you are a developer, your code needs examined, and possibly your head too."))

        gview = self.activeView()  # View*
        if gview:
            gview.scaleSelected(x, -y, factor)

    def nativeRotateSelected(self, x, y, rot):
        """
        TOWRITE

        :param `x`: TOWRITE
        :type `x`: qreal
        :param `y`: TOWRITE
        :type `y`: qreal
        :param `rot`: TOWRITE
        :type `rot`: qreal
        """
        gview = self.activeView()  # View*
        if gview:
            gview.rotateSelected(x, -y, -rot)

    def nativeMirrorSelected(self, x1, y1, x2, y2):
        """
        TOWRITE

        :param `x1`: TOWRITE
        :type `x1`: qreal
        :param `y1`: TOWRITE
        :type `y1`: qreal
        :param `x2`: TOWRITE
        :type `x2`: qreal
        :param `y2`: TOWRITE
        :type `y2`: qreal
        """
        gview = self.activeView()  # View*
        if gview:
            gview.mirrorSelected(x1, -y1, x2, -y2)

    def nativeQSnapX(self):
        """
        TOWRITE

        :rtype: qreal
        """
        scene = self.activeScene()  # QGraphicsScene*
        if scene:
            return scene.property("SCENE_QSNAP_POINT").toPointF().x()
        return 0.0

    def nativeQSnapY(self):
        """
        TOWRITE

        :rtype: qreal
        """
        scene = self.activeScene()  # QGraphicsScene*
        if scene:
            return -scene.property("SCENE_QSNAP_POINT").toPointF().y()
        return 0.0

    def nativeMouseX(self):
        """
        TOWRITE

        :rtype: qreal
        """
        scene = self.activeScene()  # QGraphicsScene*
        if scene:
            qDebug("mouseX: %.50f", scene.property("SCENE_MOUSE_POINT").toPointF().x())
        if scene:
            return scene.property("SCENE_MOUSE_POINT").toPointF().x()
        return 0.0

    def nativeMouseY(self):
        """
        TOWRITE

        :rtype: qreal
        """
        scene = self.activeScene()  # QGraphicsScene*
        if scene:
            qDebug("mouseY: %.50f", -scene.property("SCENE_MOUSE_POINT").toPointF().y())
        if scene:
            return -scene.property("SCENE_MOUSE_POINT").toPointF().y()
        return 0.0







# kate: bom off; indent-mode python; indent-width 4; replace-trailing-space-save on;
