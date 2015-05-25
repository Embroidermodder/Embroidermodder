#!/usr/bin/env python
# -*- coding: utf-8 -*-

"""
==========================================
|module_summary| native_javascript_init.py
==========================================

TOWRITE

"""

## TODO List:
## 001. Documentation.
## 002. Remove c++ port imports when finally working.
## 003. PEP8

#-Imports.---------------------------------------------------------------------
#--Python Imports
import os
qPrintable = str

#--PySide/PyQt Imports.
if PYSIDE:
    ## from PySide import QtCore, QtGui
    # or... Improve performace with less dots...
    from PySide.QtCore import qDebug, QFile, QIODevice, QSettings, Qt
    from PySide.QtGui import QToolBar, QMenu, qApp, QApplication
elif PYQT4:
    import sip
    sip.setapi('QString', 2)
    sip.setapi('QVariant', 2)
    ## from PyQt4 import QtCore, QtGui
    # or... Improve performace with less dots...
    from PyQt4.QtCore import qDebug, QFile, QIODevice, QSettings, Qt
    from PyQt4.QtGui import QToolBar, QMenu, qApp, QApplication

#--Local Imports.
from native_javascript import *
from native_javascript_include import javaInclude


# C++C++C++C++C++C++C++C++C++C++C++C++C++C++C++C++C++C++C++C++C++C++C++C++C++
#include "native-javascript.h"
# C++C++C++C++C++C++C++C++C++C++C++C++C++C++C++C++C++C++C++C++C++C++C++C++C++


try:
    gFileDir = os.path.dirname(os.path.abspath(__file__))
except:
    gFileDir = os.path.dirname(os.path.abspath(sys.argv[0]))

if os.path.basename(os.path.dirname(os.path.dirname(gFileDir))) == 'python':
    # From experimental/python/gui dir
    gAppDir = os.path.abspath(gFileDir + os.sep + '..' + os.sep + '..' + 
                              os.sep + '..' + os.sep + '..' + os.sep + 
                              'embroidermodder2')
elif os.path.basename(os.path.dirname(gFileDir)) == 'embroidermodder2':
    # From embroidermodder2 dir
    gAppDir = gFileDir


def javaInitNatives(self, engine):
    # return

    #TODO/PORT/FIXME# This doesn't work on PySide. Shiboken does not support function pointers.
    #                 either shiboken should be patched or find a workarround.
    scriptValDebug = engine.newFunction(javaDebug); engine.globalObject().setProperty("debug", scriptValDebug)
    scriptValError = engine.newFunction(javaError); engine.globalObject().setProperty("error", scriptValError)
    scriptValTodo = engine.newFunction(javaTodo); engine.globalObject().setProperty("todo", scriptValTodo)
    scriptValAlert = engine.newFunction(javaAlert); engine.globalObject().setProperty("alert", scriptValAlert)
    scriptValBlinkPrompt = engine.newFunction(javaBlinkPrompt); engine.globalObject().setProperty("blinkPrompt", scriptValBlinkPrompt)
    scriptValSetPromptPrefix = engine.newFunction(javaSetPromptPrefix); engine.globalObject().setProperty("setPromptPrefix", scriptValSetPromptPrefix)
    scriptValAppendPromptHistory = engine.newFunction(javaAppendPromptHistory); engine.globalObject().setProperty("appendPromptHistory", scriptValAppendPromptHistory)
    scriptValEnablePromptRapidFire = engine.newFunction(javaEnablePromptRapidFire); engine.globalObject().setProperty("enablePromptRapidFire", scriptValEnablePromptRapidFire)
    scriptValDisablePromptRapidFire = engine.newFunction(javaDisablePromptRapidFire); engine.globalObject().setProperty("disablePromptRapidFire", scriptValDisablePromptRapidFire)
    scriptValEnableMoveRapidFire = engine.newFunction(javaEnableMoveRapidFire); engine.globalObject().setProperty("enableMoveRapidFire", scriptValEnableMoveRapidFire)
    scriptValDisableMoveRapidFire = engine.newFunction(javaDisableMoveRapidFire); engine.globalObject().setProperty("disableMoveRapidFire", scriptValDisableMoveRapidFire)
    scriptValInitCommand = engine.newFunction(javaInitCommand); engine.globalObject().setProperty("initCommand", scriptValInitCommand)
    scriptValEndCommand = engine.newFunction(javaEndCommand); engine.globalObject().setProperty("endCommand", scriptValEndCommand)
    scriptValNewFile = engine.newFunction(javaNewFile); engine.globalObject().setProperty("newFile", scriptValNewFile)
    scriptValOpenFile = engine.newFunction(javaOpenFile); engine.globalObject().setProperty("openFile", scriptValOpenFile)
    scriptValExit = engine.newFunction(javaExit); engine.globalObject().setProperty("exit", scriptValExit)
    scriptValHelp = engine.newFunction(javaHelp); engine.globalObject().setProperty("help", scriptValHelp)
    scriptValAbout = engine.newFunction(javaAbout); engine.globalObject().setProperty("about", scriptValAbout)
    scriptValTipOfTheDay = engine.newFunction(javaTipOfTheDay); engine.globalObject().setProperty("tipOfTheDay", scriptValTipOfTheDay)
    scriptValWindowCascade = engine.newFunction(javaWindowCascade); engine.globalObject().setProperty("windowCascade", scriptValWindowCascade)
    scriptValWindowTile = engine.newFunction(javaWindowTile); engine.globalObject().setProperty("windowTile", scriptValWindowTile)
    scriptValWindowClose = engine.newFunction(javaWindowClose); engine.globalObject().setProperty("windowClose", scriptValWindowClose)
    scriptValWindowCloseAll = engine.newFunction(javaWindowCloseAll); engine.globalObject().setProperty("windowCloseAll", scriptValWindowCloseAll)
    scriptValWindowNext = engine.newFunction(javaWindowNext); engine.globalObject().setProperty("windowNext", scriptValWindowNext)
    scriptValWindowPrevious = engine.newFunction(javaWindowPrevious); engine.globalObject().setProperty("windowPrevious", scriptValWindowPrevious)
    scriptValPlatformString = engine.newFunction(javaPlatformString); engine.globalObject().setProperty("platformString", scriptValPlatformString)
    scriptValMessageBox = engine.newFunction(javaMessageBox); engine.globalObject().setProperty("messageBox", scriptValMessageBox)
    scriptValIsInt = engine.newFunction(javaIsInt); engine.globalObject().setProperty("isInt", scriptValIsInt)
    scriptValUndo = engine.newFunction(javaUndo); engine.globalObject().setProperty("undo", scriptValUndo)
    scriptValRedo = engine.newFunction(javaRedo); engine.globalObject().setProperty("redo", scriptValRedo)
    scriptValIcon16 = engine.newFunction(javaIcon16); engine.globalObject().setProperty("icon16", scriptValIcon16)
    scriptValIcon24 = engine.newFunction(javaIcon24); engine.globalObject().setProperty("icon24", scriptValIcon24)
    scriptValIcon32 = engine.newFunction(javaIcon32); engine.globalObject().setProperty("icon32", scriptValIcon32)
    scriptValIcon48 = engine.newFunction(javaIcon48); engine.globalObject().setProperty("icon48", scriptValIcon48)
    scriptValIcon64 = engine.newFunction(javaIcon64); engine.globalObject().setProperty("icon64", scriptValIcon64)
    scriptValIcon128 = engine.newFunction(javaIcon128); engine.globalObject().setProperty("icon128", scriptValIcon128)
    scriptValPanLeft = engine.newFunction(javaPanLeft); engine.globalObject().setProperty("panLeft", scriptValPanLeft)
    scriptValPanRight = engine.newFunction(javaPanRight); engine.globalObject().setProperty("panRight", scriptValPanRight)
    scriptValPanUp = engine.newFunction(javaPanUp); engine.globalObject().setProperty("panUp", scriptValPanUp)
    scriptValPanDown = engine.newFunction(javaPanDown); engine.globalObject().setProperty("panDown", scriptValPanDown)
    scriptValZoomIn = engine.newFunction(javaZoomIn); engine.globalObject().setProperty("zoomIn", scriptValZoomIn)
    scriptValZoomOut = engine.newFunction(javaZoomOut); engine.globalObject().setProperty("zoomOut", scriptValZoomOut)
    scriptValZoomExtents = engine.newFunction(javaZoomExtents); engine.globalObject().setProperty("zoomExtents", scriptValZoomExtents)
    scriptValPrintArea = engine.newFunction(javaPrintArea); engine.globalObject().setProperty("printArea", scriptValPrintArea)
    scriptValDayVision = engine.newFunction(javaDayVision); engine.globalObject().setProperty("dayVision", scriptValDayVision)
    scriptValNightVision = engine.newFunction(javaNightVision); engine.globalObject().setProperty("nightVision", scriptValNightVision)
    scriptValSetBackgroundColor = engine.newFunction(javaSetBackgroundColor); engine.globalObject().setProperty("setBackgroundColor", scriptValSetBackgroundColor)
    scriptValSetCrossHairColor = engine.newFunction(javaSetCrossHairColor); engine.globalObject().setProperty("setCrossHairColor", scriptValSetCrossHairColor)
    scriptValSetGridColor = engine.newFunction(javaSetGridColor); engine.globalObject().setProperty("setGridColor", scriptValSetGridColor)
    scriptValTextFont = engine.newFunction(javaTextFont); engine.globalObject().setProperty("textFont", scriptValTextFont)
    scriptValTextSize = engine.newFunction(javaTextSize); engine.globalObject().setProperty("textSize", scriptValTextSize)
    scriptValTextAngle = engine.newFunction(javaTextAngle); engine.globalObject().setProperty("textAngle", scriptValTextAngle)
    scriptValTextBold = engine.newFunction(javaTextBold); engine.globalObject().setProperty("textBold", scriptValTextBold)
    scriptValTextItalic = engine.newFunction(javaTextItalic); engine.globalObject().setProperty("textItalic", scriptValTextItalic)
    scriptValTextUnderline = engine.newFunction(javaTextUnderline); engine.globalObject().setProperty("textUnderline", scriptValTextUnderline)
    scriptValTextStrikeOut = engine.newFunction(javaTextStrikeOut); engine.globalObject().setProperty("textStrikeOut", scriptValTextStrikeOut)
    scriptValTextOverline = engine.newFunction(javaTextOverline); engine.globalObject().setProperty("textOverline", scriptValTextOverline)
    scriptValSetTextFont = engine.newFunction(javaSetTextFont); engine.globalObject().setProperty("setTextFont", scriptValSetTextFont)
    scriptValSetTextSize = engine.newFunction(javaSetTextSize); engine.globalObject().setProperty("setTextSize", scriptValSetTextSize)
    scriptValSetTextAngle = engine.newFunction(javaSetTextAngle); engine.globalObject().setProperty("setTextAngle", scriptValSetTextAngle)
    scriptValSetTextBold = engine.newFunction(javaSetTextBold); engine.globalObject().setProperty("setTextBold", scriptValSetTextBold)
    scriptValSetTextItalic = engine.newFunction(javaSetTextItalic); engine.globalObject().setProperty("setTextItalic", scriptValSetTextItalic)
    scriptValSetTextUnderline = engine.newFunction(javaSetTextUnderline); engine.globalObject().setProperty("setTextUnderline", scriptValSetTextUnderline)
    scriptValSetTextStrikeOut = engine.newFunction(javaSetTextStrikeOut); engine.globalObject().setProperty("setTextStrikeOut", scriptValSetTextStrikeOut)
    scriptValSetTextOverline = engine.newFunction(javaSetTextOverline); engine.globalObject().setProperty("setTextOverline", scriptValSetTextOverline)
    scriptValPreviewOn = engine.newFunction(javaPreviewOn); engine.globalObject().setProperty("previewOn", scriptValPreviewOn)
    scriptValPreviewOff = engine.newFunction(javaPreviewOff); engine.globalObject().setProperty("previewOff", scriptValPreviewOff)
    scriptValVulcanize = engine.newFunction(javaVulcanize); engine.globalObject().setProperty("vulcanize", scriptValVulcanize)
    scriptValAllowRubber = engine.newFunction(javaAllowRubber); engine.globalObject().setProperty("allowRubber", scriptValAllowRubber)
    scriptValSetRubberMode = engine.newFunction(javaSetRubberMode); engine.globalObject().setProperty("setRubberMode", scriptValSetRubberMode)
    scriptValSetRubberPoint = engine.newFunction(javaSetRubberPoint); engine.globalObject().setProperty("setRubberPoint", scriptValSetRubberPoint)
    scriptValSetRubberText = engine.newFunction(javaSetRubberText); engine.globalObject().setProperty("setRubberText", scriptValSetRubberText)
    scriptValAddRubber = engine.newFunction(javaAddRubber); engine.globalObject().setProperty("addRubber", scriptValAddRubber)
    scriptValClearRubber = engine.newFunction(javaClearRubber); engine.globalObject().setProperty("clearRubber", scriptValClearRubber)
    scriptValSpareRubber = engine.newFunction(javaSpareRubber); engine.globalObject().setProperty("spareRubber", scriptValSpareRubber)
    scriptValAddTextMulti = engine.newFunction(javaAddTextMulti); engine.globalObject().setProperty("addTextMulti", scriptValAddTextMulti)
    scriptValAddTextSingle = engine.newFunction(javaAddTextSingle); engine.globalObject().setProperty("addTextSingle", scriptValAddTextSingle)
    scriptValAddInfiniteLine = engine.newFunction(javaAddInfiniteLine); engine.globalObject().setProperty("addInfiniteLine", scriptValAddInfiniteLine)
    scriptValAddRay = engine.newFunction(javaAddRay); engine.globalObject().setProperty("addRay", scriptValAddRay)
    scriptValAddLine = engine.newFunction(javaAddLine); engine.globalObject().setProperty("addLine", scriptValAddLine)
    scriptValAddTriangle = engine.newFunction(javaAddTriangle); engine.globalObject().setProperty("addTriangle", scriptValAddTriangle)
    scriptValAddRectangle = engine.newFunction(javaAddRectangle); engine.globalObject().setProperty("addRectangle", scriptValAddRectangle)
    scriptValAddRoundedRectangle = engine.newFunction(javaAddRoundedRectangle); engine.globalObject().setProperty("addRoundedRectangle", scriptValAddRoundedRectangle)
    scriptValAddArc = engine.newFunction(javaAddArc); engine.globalObject().setProperty("addArc", scriptValAddArc)
    scriptValAddCircle = engine.newFunction(javaAddCircle); engine.globalObject().setProperty("addCircle", scriptValAddCircle)
    scriptValAddEllipse = engine.newFunction(javaAddEllipse); engine.globalObject().setProperty("addEllipse", scriptValAddEllipse)
    scriptValAddPoint = engine.newFunction(javaAddPoint); engine.globalObject().setProperty("addPoint", scriptValAddPoint)
    scriptValAddRegularPolygon = engine.newFunction(javaAddRegularPolygon); engine.globalObject().setProperty("addRegularPolygon", scriptValAddRegularPolygon)
    scriptValAddPolygon = engine.newFunction(javaAddPolygon); engine.globalObject().setProperty("addPolygon", scriptValAddPolygon)
    scriptValAddPolyline = engine.newFunction(javaAddPolyline); engine.globalObject().setProperty("addPolyline", scriptValAddPolyline)
    scriptValAddPath = engine.newFunction(javaAddPath); engine.globalObject().setProperty("addPath", scriptValAddPath)
    scriptValAddHorizontalDimension = engine.newFunction(javaAddHorizontalDimension); engine.globalObject().setProperty("addHorizontalDimension", scriptValAddHorizontalDimension)
    scriptValAddVerticalDimension = engine.newFunction(javaAddVerticalDimension); engine.globalObject().setProperty("addVerticalDimension", scriptValAddVerticalDimension)
    scriptValAddImage = engine.newFunction(javaAddImage); engine.globalObject().setProperty("addImage", scriptValAddImage)
    scriptValAddDimLeader = engine.newFunction(javaAddDimLeader); engine.globalObject().setProperty("addDimLeader", scriptValAddDimLeader)
    scriptValSetCursorShape = engine.newFunction(javaSetCursorShape); engine.globalObject().setProperty("setCursorShape", scriptValSetCursorShape)
    scriptValCalculateAngle = engine.newFunction(javaCalculateAngle); engine.globalObject().setProperty("calculateAngle", scriptValCalculateAngle)
    scriptValCalculateDistance = engine.newFunction(javaCalculateDistance); engine.globalObject().setProperty("calculateDistance", scriptValCalculateDistance)
    scriptValPerpendicularDistance = engine.newFunction(javaPerpendicularDistance); engine.globalObject().setProperty("perpendicularDistance", scriptValPerpendicularDistance)
    scriptValNumSelected = engine.newFunction(javaNumSelected); engine.globalObject().setProperty("numSelected", scriptValNumSelected)
    scriptValSelectAll = engine.newFunction(javaSelectAll); engine.globalObject().setProperty("selectAll", scriptValSelectAll)
    scriptValAddToSelection = engine.newFunction(javaAddToSelection); engine.globalObject().setProperty("addToSelection", scriptValAddToSelection)
    scriptValClearSelection = engine.newFunction(javaClearSelection); engine.globalObject().setProperty("clearSelection", scriptValClearSelection)
    scriptValDeleteSelected = engine.newFunction(javaDeleteSelected); engine.globalObject().setProperty("deleteSelected", scriptValDeleteSelected)
    scriptValCutSelected = engine.newFunction(javaCutSelected); engine.globalObject().setProperty("cutSelected", scriptValCutSelected)
    scriptValCopySelected = engine.newFunction(javaCopySelected); engine.globalObject().setProperty("copySelected", scriptValCopySelected)
    scriptValPasteSelected = engine.newFunction(javaPasteSelected); engine.globalObject().setProperty("pasteSelected", scriptValPasteSelected)
    scriptValMoveSelected = engine.newFunction(javaMoveSelected); engine.globalObject().setProperty("moveSelected", scriptValMoveSelected)
    scriptValScaleSelected = engine.newFunction(javaScaleSelected); engine.globalObject().setProperty("scaleSelected", scriptValScaleSelected)
    scriptValRotateSelected = engine.newFunction(javaRotateSelected); engine.globalObject().setProperty("rotateSelected", scriptValRotateSelected)
    scriptValMirrorSelected = engine.newFunction(javaMirrorSelected); engine.globalObject().setProperty("mirrorSelected", scriptValMirrorSelected)
    scriptValQSnapX = engine.newFunction(javaQSnapX); engine.globalObject().setProperty("qsnapX", scriptValQSnapX)
    scriptValQSnapY = engine.newFunction(javaQSnapY); engine.globalObject().setProperty("qsnapY", scriptValQSnapY)
    scriptValMouseX = engine.newFunction(javaMouseX); engine.globalObject().setProperty("mouseX", scriptValMouseX)
    scriptValMouseY = engine.newFunction(javaMouseY); engine.globalObject().setProperty("mouseY", scriptValMouseY)

    scriptValInclude = engine.newFunction(javaInclude); engine.globalObject().setProperty("include", scriptValInclude)


def javaLoadCommand(self, cmdName):
    qDebug("javaLoadCommand(%s)" % qPrintable(cmdName))
    #NOTE: Every QScriptProgram must have a unique function name to call. If every function was called main(), then
    #      the QScriptEngine would only call the last script evaluated (which happens to be main() in another script).
    #      Thus, adding the cmdName before main(), becomes line_main(), circle_main(), etc...
    #      Do not change self code unless you really know what you are doing. I mean it.
    #FIXME/PORT# appDir = QApplication.instance().applicationDirPath()  ##qApp.applicationDirPath()
    appDir = gAppDir
    ##file = QFile(appDir + "/commands/" + cmdName + "/" + cmdName + ".js")
    ##file.open(QIODevice.ReadOnly)
    ##script = str(file.readAll())  #FIXME/PORT#
    ##file.close()
    with open(appDir + "/commands/" + cmdName + "/" + cmdName + ".js") as f:
        script = f.read()

    done = False
    findNextIndex = True
    findFunc = "function"
    funcName = ""
    funcList = []  # QStringList
    index = 0
    funcNameIndex = 0
    while not done:
        if findNextIndex:
            index = script.find(findFunc, index)
            if index == -1:
                done = True
            else:
                index += len(findFunc)
                funcNameIndex = index
                findNextIndex = False
        else:
            ch = script[index]
            if ch == '(':
                funcName = script[funcNameIndex:index].strip()
                funcList.append(funcName)
                funcName = ""
                findNextIndex = True
            else:
                index += 1

    validBeforeChars = (  # QList<QChar>
        '\t', '\n', '\v', '\f', '\r', ' ', ';', '(', ')',
        '{', '}', 'not ', '=', '+', '-', '/', '*', '%',
        '<', '>', '&', '|', '?', ':', '^', '~')

    validAfterChars = ('\t', '\n', '\v', '\f', '\r', ' ', '(')  # QList<QChar>
    for functionName in funcList:
        findFunc = functionName
        funcName = cmdName + "_" + functionName

        found = 0
        done = False
        index = 0
        findNextIndex = True
        while not done:
            if findNextIndex:
                index = script.find(findFunc, index)
                if index == -1:
                    done = True
                else:
                    findNextIndex = False
            else:
                charBefore = script[index - 1]
                if charBefore in validBeforeChars:
                    i = 0
                    while True:
                        charAfter = script[index + i + len(findFunc)]  # QChar
                        if charAfter == '(':
                            found += 1
                            ##script.replace(index, len(findFunc), funcName)
                            #TODO/FIXME/PORT# inmprove this
                            script = script[:index] + funcName + script[index+len(findFunc):]
                            ##script.replace(findFunc, funcName)

                        i += 1

                        if charAfter not in validAfterChars:
                            break

                index += len(findFunc)
                findNextIndex = True

        qDebug("%s found: %d" % (qPrintable(findFunc), found))

    #TODO: low priority caveat: If a function name is within a string, is still replaced.

    script.replace("global = {};", "var " + cmdName + "_global = {};")
    script.replace("global.", cmdName + "_global.")

    self.engine.evaluate(script)

    #TODO/PORT/FIXME#
    settings = QSettings(appDir + "/commands/" + cmdName + "/" + cmdName + ".ini", QSettings.IniFormat)
    menuName = str(settings.value("Menu/Name", "Lost & Found"))  # .toString()
    menuPos = int(settings.value("Menu/Position", 0))  # .toInt()
    toolbarName = str(settings.value("ToolBar/Name", "Lost & Found"))  # .toString()
    toolbarPos = int(settings.value("ToolBar/Position", 0))  # .toInt()
    toolTip = str(settings.value("Tips/ToolTip", ""))  # .toString()
    statusTip = str(settings.value("Tips/StatusTip", ""))  # .toString()
    aliases = settings.value("Prompt/Alias") or []  # .toStringList()

    ACTION = self.createAction(cmdName, toolTip, statusTip, True)

    if toolbarName.upper() != "NONE":
        # If the toolbar doesn't exist, it.
        if toolbarName not in self.toolbarHash:
            tb = QToolBar(toolbarName, self)
            tb.setObjectName("toolbar" + toolbarName)
            tb.topLevelChanged.connect(self.floatingChangedToolBar)
            self.addToolBar(Qt.LeftToolBarArea, tb)
            self.addToolBarBreak(Qt.LeftToolBarArea)
            self.toolbarHash[toolbarName] = tb

        # TODO: order actions position in toolbar based on .ini setting
        self.toolbarHash[toolbarName].addAction(ACTION)

    if menuName.upper() != "NONE":
        # If the menu doesn't exist, it.
        if menuName not in self.menuHash:
            menu = QMenu(menuName, self)
            menu.setTearOffEnabled(True)
            self.menuBar().addMenu(menu)
            self.menuHash[menuName] = menu

        # TODO: order actions position in menu based on .ini setting
        self.menuHash[menuName].addAction(ACTION)

    for alias in aliases:
        self.prompt.addCommand(alias, cmdName)



# kate: bom off; indent-mode cstyle; indent-width 4; replace-trailing-space-save on;
