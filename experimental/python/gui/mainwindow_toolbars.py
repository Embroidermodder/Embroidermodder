#!/usr/bin/env python
# -*- coding: utf-8 -*-

"""
=======================================
|module_summary| mainwindow_toolbars.py
=======================================

TOWRITE

These methods are to be imported into the MainWindow class directly.

"""

#-Imports.----------------------------------------------------------------------
#--PySide/PyQt Imports.
if PYSIDE:
    from PySide.QtCore import qDebug, Qt, SIGNAL, SLOT
    from PySide.QtGui import qRgb, QIcon, QFont
elif PYQT4:
    import sip
    sip.setapi('QString', 2)
    sip.setapi('QVariant', 2)
    from PyQt4.QtCore import qDebug, Qt, SIGNAL, SLOT
    from PyQt4.QtGui import qRgb, QIcon, QFont


# C++C++C++C++C++C++C++C++C++C++C++C++C++C++C++C++C++C++C++C++C++C++C++C++C++
#include "mainwindow.h"
#include <QAction>
#include <QToolBar>
# C++C++C++C++C++C++C++C++C++C++C++C++C++C++C++C++C++C++C++C++C++C++C++C++C++

# meth MainWindow::
# These methods are to be imported into the MainWindow class.
# Just imagine the methods are right inside it.


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
    self.setTextSize(self.getSettingsTextSize())
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



# kate: bom off; indent-mode python; indent-width 4; replace-trailing-space-save on;
