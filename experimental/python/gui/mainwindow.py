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
from collections import deque
qPrintable = str
qFabs = abs

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
        import cPickle as pickle  #TODO# what's this for?
        import ConfigParser as configparser  # Saving of ini settings
    elif sys.version_info[0] == 3:
        import pickle
        import configparser
except ImportError as exc:
    raise exc
gSettings = configparser.ConfigParser()


#--PySide/PyQt Imports.
if PYSIDE:
    from PySide.QtCore import (qDebug, Qt, QDate, QDir, QFile, QFileInfo, QLocale, QSize, QUrl, QPoint, QSettings, QByteArray,
        QMetaObject, QLibraryInfo, QTextStream, QTranslator, SIGNAL, SLOT, QCoreApplication, QMargins, QPointF, QLineF,
        Slot)
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
elif PYQT4:
    import sip
    sip.setapi('QString', 2)
    sip.setapi('QVariant', 2)
    from PyQt4.QtCore import (qDebug, Qt, QDate, QDir, QFile, QFileInfo, QLocale, QSize, QUrl, QPoint, QSettings, QByteArray,
        QMetaObject, QLibraryInfo, QTextStream, QTranslator, SIGNAL, SLOT, QCoreApplication, QMargins, QPointF, QLineF)
    from PyQt4.QtCore import pyqtSlot as Slot
    from PyQt4.QtGui import (qRgb, qApp, QIcon, QFrame, QVBoxLayout, QMenu, QMenuBar, QAction,
        QStatusBar, QMdiArea, QWidget, QMdiSubWindow, QMessageBox, QToolBar,
        QFileDialog, QApplication, QLabel, QComboBox, QCloseEvent, QMainWindow,
        QColor, QFontComboBox, QStyle, QWizard, QWizardPage, QDialog,
        QCheckBox, QCursor, QFont, QKeySequence,
        QDesktopServices, QDialogButtonBox,
        QPushButton, QGraphicsScene, QWhatsThis
        )
    from PyQt4.QtScript import QScriptEngine
    from PyQt4.QtScriptTools import QScriptEngineDebugger

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
from preview_dialog import PreviewDialog
from layer_manager import LayerManager
from object_data import OBJ_NAME
from object_textsingle import TextSingleObject
from object_line import LineObject
from object_arc import ArcObject
from object_circle import CircleObject
from object_rect import RectObject
from object_point import PointObject
from object_polygon import PolygonObject
from object_ellipse import EllipseObject
from object_polyline import PolylineObject
from object_dimleader import DimLeaderObject
from native_scripting import initMainWinPointer

#--libembroidery Imports.
if not 'BUILDING_SPHINX_DOCS' in __builtins__:  # TEMP # Avoid sphinx docs error if bindings are not built yet.
    from libembroidery import embFormat_typeFromName, embFormatList_create, embFormatList_free, \
        embFormat_extension, embFormat_description, embFormat_readerState, embFormat_writerState

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

    .. sphinx_generate_methods_summary::
       MainWindow
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

        self.settings_general_language =               str()    # QString
        self.settings_general_icon_theme =             str()    # QString
        self.settings_general_icon_size =              int()    # int
        self.settings_general_mdi_bg_use_logo =        bool()   # bool
        self.settings_general_mdi_bg_use_texture =     bool()   # bool
        self.settings_general_mdi_bg_use_color =       bool()   # bool
        self.settings_general_mdi_bg_logo =            str()    # QString
        self.settings_general_mdi_bg_texture =         str()    # QString
        self.settings_general_mdi_bg_color =           int()    # QRgb
        self.settings_general_tip_of_the_day =         bool()   # bool
        self.settings_general_current_tip =            int()    # quint16
        self.settings_general_system_help_browser =    bool()   # bool
        self.settings_general_check_for_updates =      bool()   # bool
        self.settings_display_use_opengl =             bool()   # bool
        self.settings_display_renderhint_aa =          bool()   # bool
        self.settings_display_renderhint_text_aa =     bool()   # bool
        self.settings_display_renderhint_smooth_pix =  bool()   # bool
        self.settings_display_renderhint_high_aa =     bool()   # bool
        self.settings_display_renderhint_noncosmetic = bool()   # bool
        self.settings_display_show_scrollbars =        bool()   # bool
        self.settings_display_scrollbar_widget_num =   int()    # int
        self.settings_display_crosshair_color =        int()    # QRgb
        self.settings_display_bg_color =               int()    # QRgb
        self.settings_display_selectbox_left_color =   int()    # QRgb
        self.settings_display_selectbox_left_fill =    int()    # QRgb
        self.settings_display_selectbox_right_color =  int()    # QRgb
        self.settings_display_selectbox_right_fill =   int()    # QRgb
        self.settings_display_selectbox_alpha =        int()    # quint8
        self.settings_display_zoomscale_in =           float()  # qreal
        self.settings_display_zoomscale_out =          float()  # qreal
        self.settings_display_crosshair_percent =      int()    # quint8
        self.settings_display_units =                  str()    # QString
        self.settings_prompt_text_color =              int()    # QRgb
        self.settings_prompt_bg_color =                int()    # QRgb
        self.settings_prompt_font_family =             str()    # QString
        self.settings_prompt_font_style =              str()    # QString
        self.settings_prompt_font_size =               int()    # quint8
        self.settings_prompt_save_history =            bool()   # bool
        self.settings_prompt_save_history_as_html =    bool()   # bool
        self.settings_prompt_save_history_filename =   str()    # QString
        self.settings_opensave_custom_filter =         str()    # QString
        self.settings_opensave_open_format =           str()    # QString
        self.settings_opensave_open_thumbnail =        bool()   # bool
        self.settings_opensave_save_format =           str()    # QString
        self.settings_opensave_save_thumbnail =        bool()   # bool
        self.settings_opensave_recent_max_files =      int(10)  # quint8
        self.settings_opensave_recent_list_of_files =  deque()  # QStringList
        self.settings_opensave_recent_directory =      str()    # QString
        self.settings_opensave_trim_dst_num_jumps =    int()    # quint8
        self.settings_printing_default_device =        str()    # QString
        self.settings_printing_use_last_device =       bool()   # bool
        self.settings_printing_disable_bg =            bool()   # bool
        self.settings_grid_show_on_load =              bool()   # bool
        self.settings_grid_show_origin =               bool()   # bool
        self.settings_grid_color_match_crosshair =     bool()   # bool
        self.settings_grid_color =                     int()    # QRgb
        self.settings_grid_load_from_file =            bool()   # bool
        self.settings_grid_type =                      str()    # QString
        self.settings_grid_center_on_origin =          bool()   # bool
        self.settings_grid_center_x =                  float()  # qreal
        self.settings_grid_center_y =                  float()  # qreal
        self.settings_grid_size_x =                    float()  # qreal
        self.settings_grid_size_y =                    float()  # qreal
        self.settings_grid_spacing_x =                 float()  # qreal
        self.settings_grid_spacing_y =                 float()  # qreal
        self.settings_grid_size_radius =               float()  # qreal
        self.settings_grid_spacing_radius =            float()  # qreal
        self.settings_grid_spacing_angle =             float()  # qreal
        self.settings_ruler_show_on_load =             bool()   # bool
        self.settings_ruler_metric =                   bool()   # bool
        self.settings_ruler_color =                    int()    # QRgb
        self.settings_ruler_pixel_size =               int()    # quint8
        self.settings_qsnap_enabled =                  bool()   # bool
        self.settings_qsnap_locator_color =            int()    # QRgb
        self.settings_qsnap_locator_size =             int()    # quint8
        self.settings_qsnap_aperture_size =            int()    # quint8
        self.settings_qsnap_endpoint =                 bool()   # bool
        self.settings_qsnap_midpoint =                 bool()   # bool
        self.settings_qsnap_center =                   bool()   # bool
        self.settings_qsnap_node =                     bool()   # bool
        self.settings_qsnap_quadrant =                 bool()   # bool
        self.settings_qsnap_intersection =             bool()   # bool
        self.settings_qsnap_extension =                bool()   # bool
        self.settings_qsnap_insertion =                bool()   # bool
        self.settings_qsnap_perpendicular =            bool()   # bool
        self.settings_qsnap_tangent =                  bool()   # bool
        self.settings_qsnap_nearest =                  bool()   # bool
        self.settings_qsnap_apparent =                 bool()   # bool
        self.settings_qsnap_parallel =                 bool()   # bool
        self.settings_lwt_show_lwt =                   bool()   # bool
        self.settings_lwt_real_render =                bool()   # bool
        self.settings_lwt_default_lwt =                float()  # qreal
        self.settings_selection_mode_pickfirst =       bool()   # bool
        self.settings_selection_mode_pickadd =         bool()   # bool
        self.settings_selection_mode_pickdrag =        bool()   # bool
        self.settings_selection_coolgrip_color =       int()    # QRgb
        self.settings_selection_hotgrip_color =        int()    # QRgb
        self.settings_selection_grip_size =            int()    # quint8
        self.settings_selection_pickbox_size =         int()    # quint8
        self.settings_text_font =                      str()    # QString
        self.settings_text_size =                      float()  # qreal
        self.settings_text_angle =                     float()  # qreal
        self.settings_text_style_bold =                bool()   # bool
        self.settings_text_style_italic =              bool()   # bool
        self.settings_text_style_underline =           bool()   # bool
        self.settings_text_style_overline =            bool()   # bool
        self.settings_text_style_strikeout =           bool()   # bool

        self.actionHash = {}                    # QHash<int, QAction*>
        self.toolbarHash = {}                   # QHash<QString, QToolBar*>
        self.menuHash = {}                      # QHash<QString, QMenu*>

        self.formatFilterOpen = str()           # QString
        self.formatFilterSave = str()           # QString

        self.layoutState = QByteArray()         # QByteArray

        self.listMdiWin = list()                # QList<MdiWindow*>
        self.openFilesPath = str()              # QString

        self.myFileSeparator = QAction(self)    # QAction*

        self.wizardTipOfTheDay = QWizard()      # QWizard*
        self.labelTipOfTheDay = QLabel()        # QLabel*
        self.checkBoxTipOfTheDay = QCheckBox()  # QCheckBox*
        self.listTipOfTheDay = list()           # QStringList

        self.cutCopyObjectList = list()         # QList<QGraphicsItem*>

        self.readSettings()

        lang = self.getSettingsGeneralLanguage()  # QString
        qDebug("language: %s" % lang)
        if lang == "system":
            lang = QLocale.system().languageToString(QLocale.system().language()).lower()

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

        self.loadFormats()

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
        self.dockPropEdit = dockPropEdit = PropertyEditor(gIconDir + os.sep + self.getSettingsGeneralIconTheme(), self.getSettingsSelectionModePickAdd(), prompt, self)
        self.addDockWidget(Qt.LeftDockWidgetArea, dockPropEdit)
        connect(dockPropEdit, SIGNAL("pickAddModeToggled()"), self, SLOT("pickAddModeToggled()"))

        # create the Command History Undo Editor.
        self.dockUndoEdit = dockUndoEdit = UndoEditor(gIconDir + os.sep + self.getSettingsGeneralIconTheme(), prompt, self)
        self.addDockWidget(Qt.LeftDockWidgetArea, dockUndoEdit)

        ##setDockOptions(QMainWindow.AnimatedDocks | QMainWindow.AllowTabbedDocks | QMainWindow.VerticalTabs)  # TODO: Load these from settings
        ##tabifyDockWidget(dockPropEdit, dockUndoEdit)  # TODO: load this from settings

        # Javascript
        initMainWinPointer(self)  #TODO/PORT/FIXME# check if this works

        self.engine = engine = QScriptEngine(self)
        engine.installTranslatorFunctions()
        self.debugger = QScriptEngineDebugger(self)
        self.debugger.attachTo(engine)
        self.javaInitNatives(engine)

        # Load all commands in a loop
        commandDir = QDir(gAppDir + os.sep + "commands")
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
        try:
            tipFile = open("tips.txt", 'r')
        except IOError:
            pass
        else:
            for tipLine in tipFile:
                if tipLine.strip():
                    self.listTipOfTheDay.append(tipLine)

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
        return self.actionHash["actionEnum"]

    @Slot()
    def recentMenuAboutToShow(self):
        """
        Called when the ``aboutToShow()`` SIGNAL for :py:attr:`MainWindow.recentMenu` is emitted.
        """
        qDebug("MainWindow::recentMenuAboutToShow()")
        self.recentMenu.clear()

        for i, recentFile in enumerate(self.settings_opensave_recent_list_of_files):  # for(int i = 0; i < settings_opensave_recent_list_of_files.size(); ++i)

            # If less than the max amount of entries add to menu.
            if i >= self.settings_opensave_recent_max_files:
                break

            recentFileInfo = QFileInfo(recentFile)
            if recentFileInfo.exists() and self.validFileFormat(recentFileInfo.fileName()):

                recentValue = str(i+1)

                if int(recentValue) >= 1 and int(recentValue) <= 9:
                    rAction = QAction("&" + recentValue + " " + recentFileInfo.fileName(), self)
                elif int(recentValue) == 10:
                    rAction = QAction("1&0 " + recentFileInfo.fileName(), self)
                else:
                    rAction = QAction(recentValue + " " + recentFileInfo.fileName(), self)

                rAction.setCheckable(False)
                rAction.setData(recentFile)
                self.recentMenu.addAction(rAction)
                self.connect(rAction, SIGNAL("triggered()"), self, SLOT("openrecentfile()"))

        # Ensure the list only has max amount of entries.
        # Note: deque takes care of it.
        ## list_of_files = self.settings_opensave_recent_list_of_files
        ## max_files = self.settings_opensave_recent_max_files
        ## if len(list_of_files) > max_files:
        ##     self.settings_opensave_recent_list_of_files     = list_of_files[:max_files]

    @Slot()
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

    @Slot(bool)
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

    @Slot()
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

    @Slot(bool, str)
    def openFile(self, recent=False, recentFile=""):
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

        if filesToOpen:

            for fileToOpen in filesToOpen:  # for(int i = 0; i < filesToOpen.count(); i++)

                if not self.validFileFormat(fileToOpen):
                    continue

                existing = self.findMdiWindow(fileToOpen)  # QMdiSubWindow*
                if existing:
                    self.mdiArea.setActiveSubWindow(existing)
                    continue

                # The docIndex doesn't need increased as it is only used for unnamed files
                self.numOfDocs += 1  # numOfDocs++;
                mdiWin = MdiWindow(self.docIndex, self.mainWin, self.mdiArea, Qt.SubWindow)  # MdiWindow*
                self.connect(mdiWin, SIGNAL("sendCloseMdiWin(MdiWindow)"), self, SLOT("onCloseMdiWin(MdiWindow)"))
                self.connect(self.mdiArea, SIGNAL("subWindowActivated(QMdiSubWindow)"), self, SLOT("onWindowActivated(QMdiSubWindow)"))

                # Make sure the toolbars/etc... are shown before doing their zoomExtents
                if doOnce:
                    self.updateMenuToolbarStatusbar()
                    doOnce = False

                if mdiWin.loadFile(fileToOpen):

                    self.statusbar.showMessage(self.tr("File(s) loaded"), 2000)
                    mdiWin.show()
                    mdiWin.showMaximized()
                    # Prevent duplicate entries in the recent files list.
                    if fileToOpen not in self.settings_opensave_recent_list_of_files:
                        self.settings_opensave_recent_list_of_files.appendleft(fileToOpen)

                    # Move the recent file to the top of the list.
                    else:
                        while fileToOpen in self.settings_opensave_recent_list_of_files:
                            self.settings_opensave_recent_list_of_files.remove(fileToOpen)

                        self.settings_opensave_recent_list_of_files.appendleft(fileToOpen)

                    self.settings_opensave_recent_directory = QFileInfo(fileToOpen).absolutePath()

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

    @Slot()
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
        qDebug("MainWindow::findMdiWindow(%s)" % qPrintable(fileName))
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

    @Slot()
    def onCloseWindow(self):
        """
        TOWRITE
        """
        qDebug("MainWindow::onCloseWindow()")
        mdiWin = self.mdiArea.activeSubWindow()  # MdiWindow* mdiWin = qobject_cast<MdiWindow*>(mdiArea->activeSubWindow());
        if mdiWin:
            self.onCloseMdiWin(mdiWin)

    @Slot(MdiWindow)
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

            mdiWin = self.mdiArea.activeSubWindow()  # MdiWindow* mdiWin = qobject_cast<MdiWindow*>(mdiArea->activeSubWindow());
            if mdiWin:
                mdiWin.showMaximized()

    @Slot(QMdiSubWindow)
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
            self.menuBar().clear()
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

    @Slot()
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
        if embFormat_typeFromName(qPrintable(fileName)):
            return True
        return False

    def loadFormats(self):
        """
        TOWRITE
        """
        stale = unstable = ''  # char stable, unstable;
        supportedReaders  = "All Supported Files ("  # QString
        individualReaders = "All Files (*);;"        # QString
        supportedWriters  = "All Supported Files ("  # QString
        individualWriters = "All Files (*);;"        # QString
        supportedStr = ""                            # QString
        individualStr = ""                           # QString

        # TODO: Stable Only (Settings Option)
        # stable = 'S'; unstable = 'S'

        # Stable + Unstable
        stable = 'S'; unstable = 'U'

        extension = ""    # const char*
        description = ""  # const char*
        readerState = ""  # char   
        writerState = ""  # char

        curFormat = None                     # EmbFormatList*
        formatList = embFormatList_create()  # EmbFormatList*
        if not formatList:
            QMessageBox.critical(self, self.tr("Format Loading Error"), self.tr("Unable to load formats from libembroidery."))
            return

        curFormat = formatList
        while curFormat:
            extension = embFormat_extension(curFormat)
            description = embFormat_description(curFormat)
            readerState = embFormat_readerState(curFormat)
            writerState = embFormat_writerState(curFormat)

            upperExt = str(extension).upper()  # QString
            supportedStr = "*" + upperExt + " "
            individualStr = upperExt.replace(".", "") + " - " + description + " (*" + extension + ");;"
            if readerState == stable or readerState == unstable:
                # Exclude color file formats from open dialogs
                ## if upperExt != "COL" and upperExt != "EDR" and upperExt != "INF" and upperExt != "RGB":
                if upperExt not in ("COL", "EDR", "INF", "RGB"):
                    supportedReaders += supportedStr
                    individualReaders += individualStr

            if writerState == stable or writerState == unstable:
                supportedWriters += supportedStr
                individualWriters += individualStr
    
            curFormat = curFormat.next

        embFormatList_free(formatList)
        formatList = None
    
        supportedReaders += ");;"
        supportedWriters += ");;"
    
        self.formatFilterOpen = supportedReaders + individualReaders
        self.formatFilterSave = supportedWriters + individualWriters
    
        # TODO: Fixup custom filter
        # /*
        # QString custom = getSettingsCustomFilter();
        # if(custom.contains("supported", Qt::CaseInsensitive))
        #     custom = ""; //This will hide it
        # else if(!custom.contains("*", Qt::CaseInsensitive))
        #     custom = ""; //This will hide it
        # else
        #     custom = "Custom Filter(" + custom + ");;";
    
        # return tr(qPrintable(custom + supported + all));
        # */

    def closeToolBar(self, action):
        """
        TOWRITE

        :param `action`: TOWRITE
        :type `action`: `QAction`_
        """
        if action.objectName() == "toolbarclose":
            tb = self.sender()  # QToolBar* tb = qobject_cast<QToolBar*>(sender());
            if tb:
                qDebug("%s closed." % qPrintable(tb.objectName()))
                tb.hide()

    @Slot(bool)
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

    # def setUndoCleanIcon(self, opened):
    #     """
    #     TOWRITE
    # 
    #     :param `opened`: TOWRITE
    #     :type `opened`: bool
    #     """
    #     pass #TODO/PORT# Is this supposed to be empty?

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
    # -- mainwindow_settings.py ----------------------------------------------
    ##########################################################################

    from mainwindow_settings import readSettings, writeSettings, \
        settingsPrompt, settingsDialog

    ##########################################################################
    # -- mainwindow_actions.py -----------------------------------------------
    ##########################################################################

    from mainwindow_actions import createAllActions, createAction

    ##########################################################################
    # -- mainwindow_menus.py -------------------------------------------------
    ##########################################################################

    from mainwindow_menus import createFileMenu, createEditMenu, \
        createViewMenu, createSettingsMenu, createWindowMenu, createHelpMenu, \
        createAllMenus

    ##########################################################################
    # -- mainwindow_toolbars.py ----------------------------------------------
    ##########################################################################

    from mainwindow_toolbars import createFileToolbar, createEditToolbar, \
        createViewToolbar, createZoomToolbar, createPanToolbar, \
        createIconToolbar, createHelpToolbar, createLayerToolbar, \
        createPropertiesToolbar, createTextToolbar, createPromptToolbar, \
        createAllToolbars

    ##########################################################################
    # -- mainwindow_commands.py ----------------------------------------------
    ##########################################################################

    from mainwindow_commands import stub_implement, stub_testing, exit, \
        quit, checkForUpdates, cut, copy, paste, selectAll, platformString, \
        designDetails, about, whatsThisContextHelp, print_, tipOfTheDay, \
        checkBoxTipOfTheDayStateChanged, buttonTipOfTheDayClicked, help, \
        changelog, undo, redo, isShiftPressed, setShiftPressed, \
        setShiftReleased, iconResize, icon16, icon24, icon32, icon48, icon64, \
        icon128, activeMdiWindow, activeView, activeScene, activeUndoStack, \
        setUndoCleanIcon, updateAllViewScrollBars, updateAllViewCrossHairColors, \
        updateAllViewBackgroundColors, updateAllViewSelectBoxColors, \
        updateAllViewGridColors, updateAllViewRulerColors, updatePickAddMode, \
        pickAddModeToggled, makeLayerActive, layerManager, layerPrevious, \
        zoomRealtime, zoomPrevious, zoomWindow, zoomDynamic, zoomScale, \
        zoomCenter, zoomIn, zoomOut, zoomSelected, zoomAll, zoomExtents, \
        panrealtime, panpoint, panLeft, panRight, panUp, panDown, dayVision, \
        nightVision, doNothing, layerSelectorIndexChanged, colorSelectorIndexChanged, \
        linetypeSelectorIndexChanged, lineweightSelectorIndexChanged, \
        textFontSelectorCurrentFontChanged, textSizeSelectorIndexChanged, \
        textFont, textSize, textAngle, textBold, textItalic, textUnderline, \
        textStrikeOut, textOverline, setTextFont, setTextSize, setTextAngle, \
        setTextBold, setTextItalic, setTextUnderline, setTextStrikeOut, \
        setTextOverline, getCurrentLayer, getCurrentColor, getCurrentLineType, \
        getCurrentLineWeight, deletePressed, escapePressed, toggleGrid, \
        toggleRuler, toggleLwt, enablePromptRapidFire, disablePromptRapidFire, \
        enableMoveRapidFire, disableMoveRapidFire, promptHistoryAppended, \
        logPromptInput, promptInputPrevious, promptInputNext, runCommand, \
        runCommandMain, runCommandClick, runCommandMove, runCommandContext, \
        runCommandPrompt, nativeAlert, nativeBlinkPrompt, nativeSetPromptPrefix, \
        nativeAppendPromptHistory, nativeEnablePromptRapidFire, \
        nativeDisablePromptRapidFire, nativeEnableMoveRapidFire, \
        nativeDisableMoveRapidFire, nativeInitCommand, nativeEndCommand, \
        nativeNewFile, nativeOpenFile, nativeExit, nativeHelp, nativeAbout, \
        nativeTipOfTheDay, nativeWindowCascade, nativeWindowTile, nativeWindowClose, \
        nativeWindowCloseAll, nativeWindowNext, nativeWindowPrevious, \
        nativePlatformString, nativeMessageBox, nativeUndo, nativeRedo, \
        nativeIcon16, nativeIcon24, nativeIcon32, nativeIcon48, nativeIcon64, \
        nativeIcon128, nativePanLeft, nativePanRight, nativePanUp, nativePanDown, \
        nativeZoomIn, nativeZoomOut, nativeZoomExtents, nativePrintArea, nativeDayVision, \
        nativeNightVision, nativeSetBackgroundColor, nativeSetCrossHairColor, \
        nativeSetGridColor, nativeTextFont, nativeTextSize, nativeTextAngle, \
        nativeTextBold, nativeTextItalic, nativeTextUnderline, nativeTextStrikeOut, \
        nativeTextOverline, nativeSetTextFont, nativeSetTextSize, nativeSetTextAngle, \
        nativeSetTextBold, nativeSetTextItalic, nativeSetTextUnderline, \
        nativeSetTextStrikeOut, nativeSetTextOverline, nativePreviewOn, nativePreviewOff, \
        nativeVulcanize, nativeClearRubber, nativeAllowRubber, nativeSpareRubber, \
        nativeSetRubberMode, nativeSetRubberPoint, nativeSetRubberText, nativeAddTextMulti, \
        nativeAddTextSingle, nativeAddInfiniteLine, nativeAddRay, nativeAddLine, \
        nativeAddTriangle, nativeAddRectangle, nativeAddRoundedRectangle, nativeAddArc, \
        nativeAddCircle, nativeAddSlot, nativeAddEllipse, nativeAddPoint, \
        nativeAddRegularPolygon, nativeAddPolygon, nativeAddPolyline, nativeAddPath, \
        nativeAddHorizontalDimension, nativeAddVerticalDimension, nativeAddImage, \
        nativeAddDimLeader, nativeSetCursorShape, nativeCalculateAngle, \
        nativeCalculateDistance, nativePerpendicularDistance, nativeNumSelected, \
        nativeSelectAll, nativeAddToSelection, nativeClearSelection, \
        nativeDeleteSelected, nativeCutSelected, nativeCopySelected, nativePasteSelected, \
        nativeMoveSelected, nativeScaleSelected, nativeRotateSelected, \
        nativeMirrorSelected, nativeQSnapX, nativeQSnapY, nativeMouseX, nativeMouseY

    ##########################################################################
    # -- native_javascript_init.py -------------------------------------------
    ##########################################################################

    from javascript.native_javascript_init import javaLoadCommand, javaInitNatives





# kate: bom off; indent-mode python; indent-width 4; replace-trailing-space-save on;
