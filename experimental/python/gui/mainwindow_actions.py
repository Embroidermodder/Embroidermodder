#!/usr/bin/env python
# -*- coding: utf-8 -*-

"""
======================================
|module_summary| mainwindow_actions.py
======================================

TOWRITE

These methods are to be imported into the MainWindow class directly.

"""

#-Imports.----------------------------------------------------------------------
#--Python Imports.
import os

#--PySide/PyQt Imports.
try:
    from PySide.QtCore import qDebug, SIGNAL, SLOT
    from PySide.QtGui import QApplication, QIcon, QKeySequence, QAction
except ImportError:
    raise
#    from PyQt4.QtCore import qDebug, SIGNAL, SLOT
#    from PyQt4.QtGui import QApplication, QIcon, QKeySequence, QAction


# C++C++C++C++C++C++C++C++C++C++C++C++C++C++C++C++C++C++C++C++C++C++C++C++C++
#include "mainwindow.h"
#include <QAction>
#include <QApplication>
#include <QMdiArea>
# C++C++C++C++C++C++C++C++C++C++C++C++C++C++C++C++C++C++C++C++C++C++C++C++C++

# meth MainWindow::
# These methods are to be imported into the MainWindow class.
# Just imagine the methods are right inside it.


def createAllActions(self):
    """
    TOWRITE

    .. TODO:: The c++ code is ugly. If you are going to line things up, line everything up the same so everything looks and reads nicely.

    """
    self.actionHash = actionHash = {}
    tr = self.tr

    qDebug("Creating All Actions...")
    appName = QApplication.applicationName()  # QString

    actionHash["ACTION_donothing"] = createAction(self, "donothing", tr("&Do Nothing"), tr("Does Nothing"))

    actionHash["ACTION_windowcascade"] =  createAction(self, "windowcascade",  tr("&Cascade"),   tr("Cascade the windows."))
    actionHash["ACTION_windowtile"] =     createAction(self, "windowtile",     tr("&Tile"),      tr("Tile the windows."))
    actionHash["ACTION_windowclose"] =    createAction(self, "windowclose",    tr("Cl&ose"),     tr("Close the active window."))
    actionHash["ACTION_windowcloseall"] = createAction(self, "windowcloseall", tr("Close &All"), tr("Close all the windows."))
    actionHash["ACTION_windownext"] =     createAction(self, "windownext",     tr("Ne&xt"),      tr("Move the focus to the next window."))
    actionHash["ACTION_windowprevious"] = createAction(self, "windowprevious", tr("Pre&vious"),  tr("Move the focus to the previous window."))

    actionHash["ACTION_new"] =           createAction(self, "new",           tr("&New"),     tr("Create a new file."))
    actionHash["ACTION_open"] =          createAction(self, "open",          tr("&Open"),    tr("Open an existing file."))
    actionHash["ACTION_save"] =          createAction(self, "save",          tr("&Save"),    tr("Save the design to disk."))
    actionHash["ACTION_saveas"] =        createAction(self, "saveas",        tr("Save &As"), tr("Save the design under a new name."))
    actionHash["ACTION_print"] =         createAction(self, "print",         tr("&Print"),   tr("Print the design."))
    actionHash["ACTION_designdetails"] = createAction(self, "designdetails", tr("&Details"), tr("Details of the current design."))
    actionHash["ACTION_exit"] =          createAction(self, "exit",          tr("E&xit"),    tr("Exit the application."))

    actionHash["ACTION_cut"] =   createAction(self, "cut",   tr("Cu&t"),   tr("Cut the current selection's contents to the clipboard."))
    actionHash["ACTION_copy"] =  createAction(self, "copy",  tr("&Copy"),  tr("Copy the current selection's contents to the clipboard."))
    actionHash["ACTION_paste"] = createAction(self, "paste", tr("&Paste"), tr("Paste the clipboard's contents into the current selection."))

    actionHash["ACTION_help"] =        createAction(self, "help",        tr("&Help"),             tr("Displays help."))
    actionHash["ACTION_changelog"] =   createAction(self, "changelog",   tr("&Changelog"),        tr("Describes new features in this product."))
    actionHash["ACTION_tipoftheday"] = createAction(self, "tipoftheday", tr("&Tip Of The Day"),   tr("Displays a dialog with useful tips"))
    actionHash["ACTION_about"] =       createAction(self, "about",       tr("&About ") + appName, tr("Displays information about this product."))
    actionHash["ACTION_whatsthis"] =   createAction(self, "whatsthis",   tr("&What's This?"),     tr("What's This? Context Help!"))

    actionHash["ACTION_undo"] = createAction(self, "undo", tr("&Undo"),  tr("Reverses the most recent action."))
    actionHash["ACTION_redo"] = createAction(self, "redo", tr("&Redo"),  tr("Reverses the effects of the previous undo action."))

    actionHash["ACTION_icon16"] =  createAction(self, "icon16",  tr("Icon&16"),  tr("Sets the toolbar icon size to 16x16."))
    actionHash["ACTION_icon24"] =  createAction(self, "icon24",  tr("Icon&24"),  tr("Sets the toolbar icon size to 24x24."))
    actionHash["ACTION_icon32"] =  createAction(self, "icon32",  tr("Icon&32"),  tr("Sets the toolbar icon size to 32x32."))
    actionHash["ACTION_icon48"] =  createAction(self, "icon48",  tr("Icon&48"),  tr("Sets the toolbar icon size to 48x48."))
    actionHash["ACTION_icon64"] =  createAction(self, "icon64",  tr("Icon&64"),  tr("Sets the toolbar icon size to 64x64."))
    actionHash["ACTION_icon128"] = createAction(self, "icon128", tr("Icon12&8"), tr("Sets the toolbar icon size to 128x128."))

    actionHash["ACTION_settingsdialog"] = createAction(self, "settingsdialog", tr("&Settings"), tr("Configure settings specific to this product."))

    actionHash["ACTION_makelayercurrent"] =   createAction(self, "makelayercurrent",   tr("&Make Layer Active"),    tr("Makes the layer of a selected object the active layer"))
    actionHash["ACTION_layers"] =             createAction(self, "layers",             tr("&Layers"),                tr("Manages layers and layer properties:  LAYER"))
    actionHash["ACTION_layerselector"] =      createAction(self, "layerselector",      tr("&Layer Selector"),        tr("Dropdown selector for changing the current layer"))
    actionHash["ACTION_layerprevious"] =      createAction(self, "layerprevious",      tr("&Layer Previous"),        tr("Restores the previous layer settings:  LAYERP"))
    actionHash["ACTION_colorselector"] =      createAction(self, "colorselector",      tr("&Color Selector"),        tr("Dropdown selector for changing the current thread color"))
    actionHash["ACTION_linetypeselector"] =   createAction(self, "linetypeselector",   tr("&Stitchtype Selector"),   tr("Dropdown selector for changing the current stitch type"))
    actionHash["ACTION_lineweightselector"] = createAction(self, "lineweightselector", tr("&Threadweight Selector"), tr("Dropdown selector for changing the current thread weight"))
    actionHash["ACTION_hidealllayers"] =      createAction(self, "hidealllayers",      tr("&Hide All Layers"),       tr("Turns the visibility off for all layers in the current drawing:  HIDEALL"))
    actionHash["ACTION_showalllayers"] =      createAction(self, "showalllayers",      tr("&Show All Layers"),       tr("Turns the visibility on for all layers in the current drawing:  SHOWALL"))
    actionHash["ACTION_freezealllayers"] =    createAction(self, "freezealllayers",    tr("&Freeze All Layers"),     tr("Freezes all layers in the current drawing:  FREEZEALL"))
    actionHash["ACTION_thawalllayers"] =      createAction(self, "thawalllayers",      tr("&Thaw All Layers"),       tr("Thaws all layers in the current drawing:  THAWALL"))
    actionHash["ACTION_lockalllayers"] =      createAction(self, "lockalllayers",      tr("&Lock All Layers"),       tr("Locks all layers in the current drawing:  LOCKALL"))
    actionHash["ACTION_unlockalllayers"] =    createAction(self, "unlockalllayers",    tr("&Unlock All Layers"),     tr("Unlocks all layers in the current drawing:  UNLOCKALL"))

    actionHash["ACTION_textbold"] =      createAction(self, "textbold",      tr("&Bold Text"),      tr("Sets text to be bold."))
    actionHash["ACTION_textitalic"] =    createAction(self, "textitalic",    tr("&Italic Text"),    tr("Sets text to be italic."))
    actionHash["ACTION_textunderline"] = createAction(self, "textunderline", tr("&Underline Text"), tr("Sets text to be underlined."))
    actionHash["ACTION_textstrikeout"] = createAction(self, "textstrikeout", tr("&StrikeOut Text"), tr("Sets text to be striked out."))
    actionHash["ACTION_textoverline"] =  createAction(self, "textoverline",  tr("&Overline Text"),  tr("Sets text to be overlined."))

    actionHash["ACTION_zoomrealtime"] = createAction(self, "zoomrealtime", tr("Zoom &Realtime"), tr("Zooms to increase or decrease the apparent size of objects in the current viewport."))
    actionHash["ACTION_zoomprevious"] = createAction(self, "zoomprevious", tr("Zoom &Previous"), tr("Zooms to display the previous view."))
    actionHash["ACTION_zoomwindow"] =   createAction(self, "zoomwindow",   tr("Zoom &Window"),   tr("Zooms to display an area specified by a rectangular window."))
    actionHash["ACTION_zoomdynamic"] =  createAction(self, "zoomdynamic",  tr("Zoom &Dynamic"),  tr("Zooms to display the generated portion of the drawing."))
    actionHash["ACTION_zoomscale"] =    createAction(self, "zoomscale",    tr("Zoom &Scale"),    tr("Zooms the display using a specified scale factor."))
    actionHash["ACTION_zoomcenter"] =   createAction(self, "zoomcenter",   tr("Zoom &Center"),   tr("Zooms to display a view specified by a center point and magnification or height."))
    actionHash["ACTION_zoomin"] =       createAction(self, "zoomin",       tr("Zoom &In"),       tr("Zooms to increase the apparent size of objects."))
    actionHash["ACTION_zoomout"] =      createAction(self, "zoomout",      tr("Zoom &Out"),      tr("Zooms to decrease the apparent size of objects."))
    actionHash["ACTION_zoomselected"] = createAction(self, "zoomselected", tr("Zoom Selec&ted"), tr("Zooms to display the selected objects."))
    actionHash["ACTION_zoomall"] =      createAction(self, "zoomall",      tr("Zoom &All"),      tr("Zooms to display the drawing extents or the grid limits."))
    actionHash["ACTION_zoomextents"] =  createAction(self, "zoomextents",  tr("Zoom &Extents"),  tr("Zooms to display the drawing extents."))

    actionHash["ACTION_panrealtime"] = createAction(self, "panrealtime", tr("&Pan Realtime"), tr("Moves the view in the current viewport."))
    actionHash["ACTION_panpoint"] =    createAction(self, "panpoint",    tr("&Pan Point"),    tr("Moves the view by the specified distance."))
    actionHash["ACTION_panleft"] =     createAction(self, "panleft",     tr("&Pan Left"),     tr("Moves the view to the left."))
    actionHash["ACTION_panright"] =    createAction(self, "panright",    tr("&Pan Right"),    tr("Moves the view to the right."))
    actionHash["ACTION_panup"] =       createAction(self, "panup",       tr("&Pan Up"),       tr("Moves the view up."))
    actionHash["ACTION_pandown"] =     createAction(self, "pandown",     tr("&Pan Down"),     tr("Moves the view down."))

    actionHash["ACTION_day"] =   createAction(self, "day",   tr("&Day"),   tr("Updates the current view using day vision settings."))
    actionHash["ACTION_night"] = createAction(self, "night", tr("&Night"), tr("Updates the current view using night vision settings."))

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


# kate: bom off; indent-mode python; indent-width 4; replace-trailing-space-save on;
