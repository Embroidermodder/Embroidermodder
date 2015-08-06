#!/usr/bin/env python
# -*- coding: utf-8 -*-

"""
=====================================
|module_summary| native_javascript.py
=====================================

TOWRITE

"""

## TODO List:
## 001. Documentation.
## 002. Remove c++ port imports when finally working.
## 003. PEP8

#-Imports.---------------------------------------------------------------------
#--Python Imports
from math import isnan as qIsNaN
qPrintable = str

#--PySide/PyQt Imports.
if PYSIDE:
    ## from PySide import QtCore, QtGui
    # or... Improve performace with less dots...
    from PySide.QtScript import QScriptContext, QScriptValue
    from PySide.QtCore import qDebug
    from PySide.QtGui import QPainterPath
elif PYQT4:
    import sip
    sip.setapi('QString', 2)
    sip.setapi('QVariant', 2)
    ## from PyQt4 import QtCore, QtGui
    # or... Improve performace with less dots...
    from PyQt4.QtScript import QScriptContext, QScriptValue
    from PyQt4.QtCore import qDebug
    from PyQt4.QtGui import QPainterPath

#--Local Imports.
from native_scripting import mainWin
from object_data import (PREVIEW_CLONE_NULL, PREVIEW_MODE_NULL,
    PREVIEW_CLONE_SELECTED, PREVIEW_CLONE_RUBBER, PREVIEW_MODE_MOVE,
    PREVIEW_MODE_ROTATE, PREVIEW_MODE_SCALE, SPARE_RUBBER_PATH,
    SPARE_RUBBER_POLYGON, SPARE_RUBBER_POLYLINE, OBJ_RUBBER_CIRCLE_1P_RAD,
    OBJ_RUBBER_CIRCLE_1P_DIA, OBJ_RUBBER_CIRCLE_2P, OBJ_RUBBER_CIRCLE_3P,
    OBJ_RUBBER_CIRCLE_TTR, OBJ_RUBBER_CIRCLE_TTT, OBJ_RUBBER_DIMLEADER_LINE,
    OBJ_RUBBER_ELLIPSE_LINE, OBJ_RUBBER_ELLIPSE_MAJORDIAMETER_MINORRADIUS,
    OBJ_RUBBER_ELLIPSE_MAJORRADIUS_MINORRADIUS, OBJ_RUBBER_ELLIPSE_ROTATION,
    OBJ_RUBBER_LINE, OBJ_RUBBER_POLYGON, OBJ_RUBBER_POLYGON_INSCRIBE,
    OBJ_RUBBER_POLYGON_CIRCUMSCRIBE, OBJ_RUBBER_POLYLINE, OBJ_RUBBER_RECTANGLE,
    OBJ_RUBBER_TEXTSINGLE, OBJ_RUBBER_ON, OBJ_RUBBER_OFF)


# C++C++C++C++C++C++C++C++C++C++C++C++C++C++C++C++C++C++C++C++C++C++C++C++C++
#include "native-javascript.h"
#include "object-data.h"
# C++C++C++C++C++C++C++C++C++C++C++C++C++C++C++C++C++C++C++C++C++C++C++C++C++


def javaDebug(context, engine):
    if context.argumentCount() != 1:
        return context.throwError("debug() requires one argument")

    if not context.argument(0).isString():
        return context.throwError(QScriptContext.TypeError, "debug(): first argument is not a string")

    qDebug("%s" % qPrintable(context.argument(0).toString()))
    return QScriptValue()


def javaError(context, engine):
    if context.argumentCount() != 2:
        return context.throwError("error() requires two arguments")
    if not context.argument(0).isString():
        return context.throwError(QScriptContext.TypeError, "error(): first argument is not a string")
    if not context.argument(1).isString():
        return context.throwError(QScriptContext.TypeError, "error(): second argument is not a string")

    strCmd = context.argument(0).toString()
    strErr = context.argument(1).toString()

    mainWin().nativeSetPromptPrefix("ERROR: (" + strCmd + ") " + strErr)
    mainWin().nativeAppendPromptHistory(str())
    mainWin().nativeEndCommand()
    return QScriptValue()


def javaTodo(context, engine):
    if context.argumentCount() != 2:
        return context.throwError("todo() requires two arguments")

    if not context.argument(0).isString():
        return context.throwError(QScriptContext.TypeError, "todo(): first argument is not a string")

    if not context.argument(1).isString():
        return context.throwError(QScriptContext.TypeError, "todo(): second argument is not a string")

    strCmd = context.argument(0).toString()
    strTodo = context.argument(1).toString()

    mainWin().nativeAlert("TODO: (" + strCmd + ") " + strTodo)
    mainWin().nativeEndCommand()
    return QScriptValue()


def javaAlert(context, engine):
    if context.argumentCount() != 1:
        return context.throwError("alert() requires one argument")

    if not context.argument(0).isString():
        return context.throwError(QScriptContext.TypeError, "alert(): first argument is not a string")

    mainWin().nativeAlert(context.argument(0).toString())
    return QScriptValue()


def javaBlinkPrompt(context, engine):
    if context.argumentCount() != 0:
        return context.throwError("blinkPrompt() requires zero arguments")

    mainWin().nativeBlinkPrompt()
    return QScriptValue()


def javaSetPromptPrefix(context, engine):
    if context.argumentCount() != 1:
        return context.throwError("setPromptPrefix() requires one argument")

    if not context.argument(0).isString():
        return context.throwError(QScriptContext.TypeError, "setPromptPrefix(): first argument is not a string")

    mainWin().nativeSetPromptPrefix(context.argument(0).toString())
    return QScriptValue()


def javaAppendPromptHistory(context, engine):
    args = context.argumentCount()
    if args == 0:
        mainWin().nativeAppendPromptHistory(str())
    elif args == 1:
        mainWin().nativeAppendPromptHistory(context.argument(0).toString())
    else:
        return context.throwError("appendPromptHistory() requires one or zero arguments")

    return QScriptValue()


def javaEnablePromptRapidFire(context, engine):
    if context.argumentCount() != 0:
        return context.throwError("enablePromptRapidFire() requires zero arguments")

    mainWin().nativeEnablePromptRapidFire()
    return QScriptValue()


def javaDisablePromptRapidFire(context, engine):
    if context.argumentCount() != 0:
        return context.throwError("disablePromptRapidFire() requires zero arguments")

    mainWin().nativeDisablePromptRapidFire()
    return QScriptValue()


def javaEnableMoveRapidFire(context, engine):
    if context.argumentCount() != 0:
        return context.throwError("enableMoveRapidFire() requires zero arguments")

    mainWin().nativeEnableMoveRapidFire()
    return QScriptValue()


def javaDisableMoveRapidFire(context, engine):
    if context.argumentCount() != 0:
        return context.throwError("disableMoveRapidFire() requires zero arguments")

    mainWin().nativeDisableMoveRapidFire()
    return QScriptValue()


def javaInitCommand(context, engine):
    if context.argumentCount() != 0:
        return context.throwError("initCommand() requires zero arguments")

    mainWin().nativeInitCommand()
    return QScriptValue()


def javaEndCommand(context, engine):
    if context.argumentCount() != 0:
        return context.throwError("endCommand() requires zero arguments")

    mainWin().nativeEndCommand()
    return QScriptValue()


def javaNewFile(context, engine):
    if context.argumentCount() != 0:
        return context.throwError("newFile() requires zero arguments")

    mainWin().nativeNewFile()
    return QScriptValue()


def javaOpenFile(context, engine):
    if context.argumentCount() != 0:
        return context.throwError("openFile() requires zero arguments")

    mainWin().nativeOpenFile()
    return QScriptValue()


def javaExit(context, engine):
    if context.argumentCount() != 0:
        return context.throwError("exit() requires zero arguments")

    mainWin().nativeExit()
    return QScriptValue()


def javaHelp(context, engine):
    if context.argumentCount() != 0:
        return context.throwError("help() requires zero arguments")

    mainWin().nativeHelp()
    return QScriptValue()


def javaAbout(context, engine):
    if context.argumentCount() != 0:
        return context.throwError("about() requires zero arguments")

    mainWin().nativeAbout()
    return QScriptValue()


def javaTipOfTheDay(context, engine):
    if context.argumentCount() != 0:
        return context.throwError("tipOfTheDay() requires zero arguments")

    mainWin().nativeTipOfTheDay()
    return QScriptValue()


def javaWindowCascade(context, engine):
    if context.argumentCount() != 0:
        return context.throwError("windowCascade() requires zero arguments")

    mainWin().nativeWindowCascade()
    return QScriptValue()


def javaWindowTile(context, engine):
    if context.argumentCount() != 0:
        return context.throwError("windowTile() requires zero arguments")

    mainWin().nativeWindowTile()
    return QScriptValue()


def javaWindowClose(context, engine):
    if context.argumentCount() != 0:
        return context.throwError("windowClose() requires zero arguments")

    mainWin().nativeWindowClose()
    return QScriptValue()


def javaWindowCloseAll(context, engine):
    if context.argumentCount() != 0:
        return context.throwError("windowCloseAll() requires zero arguments")

    mainWin().nativeWindowCloseAll()
    return QScriptValue()


def javaWindowNext(context, engine):
    if context.argumentCount() != 0:
        return context.throwError("windowNext() requires zero arguments")

    mainWin().nativeWindowNext()
    return QScriptValue()


def javaWindowPrevious(context, engine):
    if context.argumentCount() != 0:
        return context.throwError("windowPrevious() requires zero arguments")

    mainWin().nativeWindowPrevious()
    return QScriptValue()


def javaPlatformString(context, engine):
    if context.argumentCount() != 0:
        return context.throwError("platformString() requires zero arguments")

    return QScriptValue(mainWin().nativePlatformString())


def javaMessageBox(context, engine):
    if context.argumentCount() != 3:
        return context.throwError("messageBox() requires three arguments")

    if not context.argument(0).isString():
        return context.throwError(QScriptContext.TypeError, "messageBox(): first argument is not a string")

    if not context.argument(1).isString():
        return context.throwError(QScriptContext.TypeError, "messageBox(): second argument is not a string")

    if not context.argument(2).isString():
        return context.throwError(QScriptContext.TypeError, "messageBox(): third argument is not a string")

    type_ = context.argument(0).toString().lower()
    title = context.argument(1).toString()
    text = context.argument(2).toString()

    ##if type_ != "critical" and type_ != "information" and type_ != "question" and type_ != "warning":
    if type_ not in ("critical", "information", "question", "warning"):
        return context.throwError(QScriptContext.UnknownError, "messageBox(): first argument must be \"critical\", \"information\", \"question\" or \"warning\".")

    mainWin().nativeMessageBox(type_, title, text)
    return QScriptValue()


def javaIsInt(context, engine):
    if context.argumentCount() != 1:
        return context.throwError("isInt() requires one argument")

    if not context.argument(0).isNumber():
        return context.throwError(QScriptContext.TypeError, "isInt(): first argument is not a number")

    num = context.argument(0).toNumber()

    #isNaN check
    if qIsNaN(num):
        return context.throwError(QScriptContext.TypeError, "isInt(): first argument failed isNaN check. There is an error in your code.")

    if num % 1 == 0:
        return QScriptValue(True)

    return QScriptValue(False)


def javaUndo(context, engine):
    if context.argumentCount() != 0:
        return context.throwError("undo() requires zero arguments")

    mainWin().nativeUndo()
    return QScriptValue()


def javaRedo(context, engine):
    if context.argumentCount() != 0:
        return context.throwError("redo() requires zero arguments")

    mainWin().nativeRedo()
    return QScriptValue()


def javaIcon16(context, engine):
    if context.argumentCount() != 0:
        return context.throwError("icon16() requires zero arguments")

    mainWin().nativeIcon16()
    return QScriptValue()


def javaIcon24(context, engine):
    if context.argumentCount() != 0:
        return context.throwError("icon24() requires zero arguments")

    mainWin().nativeIcon24()
    return QScriptValue()


def javaIcon32(context, engine):
    if context.argumentCount() != 0:
        return context.throwError("icon32() requires zero arguments")

    mainWin().nativeIcon32()
    return QScriptValue()


def javaIcon48(context, engine):
    if context.argumentCount() != 0:
        return context.throwError("icon48() requires zero arguments")

    mainWin().nativeIcon48()
    return QScriptValue()


def javaIcon64(context, engine):
    if context.argumentCount() != 0:
        return context.throwError("icon64() requires zero arguments")

    mainWin().nativeIcon64()
    return QScriptValue()


def javaIcon128(context, engine):
    if context.argumentCount() != 0:
        return context.throwError("icon128() requires zero arguments")

    mainWin().nativeIcon128()
    return QScriptValue()


def javaPanLeft(context, engine):
    if context.argumentCount() != 0:
        return context.throwError("panLeft() requires zero arguments")

    mainWin().nativePanLeft()
    return QScriptValue()


def javaPanRight(context, engine):
    if context.argumentCount() != 0:
        return context.throwError("panRight() requires zero arguments")

    mainWin().nativePanRight()
    return QScriptValue()


def javaPanUp(context, engine):
    if context.argumentCount() != 0:
        return context.throwError("panUp() requires zero arguments")

    mainWin().nativePanUp()
    return QScriptValue()


def javaPanDown(context, engine):
    if context.argumentCount() != 0:
        return context.throwError("panDown() requires zero arguments")

    mainWin().nativePanDown()
    return QScriptValue()


def javaZoomIn(context, engine):
    if context.argumentCount() != 0:
        return context.throwError("zoomIn() requires zero arguments")

    mainWin().nativeZoomIn()
    return QScriptValue()


def javaZoomOut(context, engine):
    if context.argumentCount() != 0:
        return context.throwError("zoomOut() requires zero arguments")

    mainWin().nativeZoomOut()
    return QScriptValue()


def javaZoomExtents(context, engine):
    if context.argumentCount() != 0:
        return context.throwError("zoomExtents() requires zero arguments")

    mainWin().nativeZoomExtents()
    return QScriptValue()


def javaPrintArea(context, engine):
    if context.argumentCount() != 4:
        return context.throwError("printArea() requires four arguments")

    if not context.argument(0).isNumber():
        return context.throwError(QScriptContext.TypeError, "printArea(): first argument is not a number")

    if not context.argument(1).isNumber():
        return context.throwError(QScriptContext.TypeError, "printArea(): second argument is not a number")

    if not context.argument(2).isNumber():
        return context.throwError(QScriptContext.TypeError, "printArea(): third argument is not a number")

    if not context.argument(3).isNumber():
        return context.throwError(QScriptContext.TypeError, "printArea(): fourth argument is not a number")

    x = context.argument(0).toNumber()
    y = context.argument(1).toNumber()
    w = context.argument(2).toNumber()
    h = context.argument(3).toNumber()

    #isNaN check
    if qIsNaN(x):
        return context.throwError(QScriptContext.TypeError, "printArea(): first argument failed isNaN check. There is an error in your code.")

    if qIsNaN(y):
        return context.throwError(QScriptContext.TypeError, "printArea(): second argument failed isNaN check. There is an error in your code.")

    if qIsNaN(w):
        return context.throwError(QScriptContext.TypeError, "printArea(): third argument failed isNaN check. There is an error in your code.")

    if qIsNaN(h):
        return context.throwError(QScriptContext.TypeError, "printArea(): fourth argument failed isNaN check. There is an error in your code.")

    mainWin().nativePrintArea(x, y, w, h)
    return QScriptValue()


def javaDayVision(context, engine):
    if context.argumentCount() != 0:
        return context.throwError("dayVision() requires zero arguments")

    mainWin().nativeDayVision()
    return QScriptValue()


def javaNightVision(context, engine):
    if context.argumentCount() != 0:
        return context.throwError("nightVision() requires zero arguments")

    mainWin().nativeNightVision()
    return QScriptValue()


def javaSetBackgroundColor(context, engine):
    if context.argumentCount() != 3:
        return context.throwError("setBackgroundColor() requires three arguments")

    if not context.argument(0).isNumber():
        return context.throwError(QScriptContext.TypeError, "setBackgroundColor(): first argument is not a number")

    if not context.argument(1).isNumber():
        return context.throwError(QScriptContext.TypeError, "setBackgroundColor(): second argument is not a number")

    if not context.argument(2).isNumber():
        return context.throwError(QScriptContext.TypeError, "setBackgroundColor(): third argument is not a number")

    r = context.argument(0).toNumber()
    g = context.argument(1).toNumber()
    b = context.argument(2).toNumber()

    #isNaN check
    if qIsNaN(r):
        return context.throwError(QScriptContext.TypeError, "setBackgroundColor(): first argument failed isNaN check. There is an error in your code.")

    if qIsNaN(g):
        return context.throwError(QScriptContext.TypeError, "setBackgroundColor(): second argument failed isNaN check. There is an error in your code.")

    if qIsNaN(b):
        return context.throwError(QScriptContext.TypeError, "setBackgroundColor(): third argument failed isNaN check. There is an error in your code.")

    if r < 0 or r > 255:
        return context.throwError(QScriptContext.UnknownError, "setBackgroundColor(): r value must be in range 0-255")

    if g < 0 or g > 255:
        return context.throwError(QScriptContext.UnknownError, "setBackgroundColor(): g value must be in range 0-255")

    if b < 0 or b > 255:
        return context.throwError(QScriptContext.UnknownError, "setBackgroundColor(): b value must be in range 0-255")

    mainWin().nativeSetBackgroundColor(r, g, b)
    return QScriptValue()


def javaSetCrossHairColor(context, engine):
    if context.argumentCount() != 3:
        return context.throwError("setCrossHairColor() requires three arguments")

    if not context.argument(0).isNumber():
        return context.throwError(QScriptContext.TypeError, "setCrossHairColor(): first argument is not a number")

    if not context.argument(1).isNumber():
        return context.throwError(QScriptContext.TypeError, "setCrossHairColor(): second argument is not a number")

    if not context.argument(2).isNumber():
        return context.throwError(QScriptContext.TypeError, "setCrossHairColor(): third argument is not a number")

    r = context.argument(0).toNumber()
    g = context.argument(1).toNumber()
    b = context.argument(2).toNumber()

    #isNaN check
    if qIsNaN(r): return context.throwError(QScriptContext.TypeError, "setCrossHairColor(): first argument failed isNaN check. There is an error in your code.")
    if qIsNaN(g): return context.throwError(QScriptContext.TypeError, "setCrossHairColor(): second argument failed isNaN check. There is an error in your code.")
    if qIsNaN(b): return context.throwError(QScriptContext.TypeError, "setCrossHairColor(): third argument failed isNaN check. There is an error in your code.")

    if r < 0 or r > 255: return context.throwError(QScriptContext.UnknownError, "setCrossHairColor(): r value must be in range 0-255");
    if g < 0 or g > 255: return context.throwError(QScriptContext.UnknownError, "setCrossHairColor(): g value must be in range 0-255");
    if b < 0 or b > 255: return context.throwError(QScriptContext.UnknownError, "setCrossHairColor(): b value must be in range 0-255");

    mainWin().nativeSetCrossHairColor(r, g, b)
    return QScriptValue()


def javaSetGridColor(context, engine):
    if context.argumentCount() != 3:
        return context.throwError("setGridColor() requires three arguments")

    if not context.argument(0).isNumber():
        context.throwError(QScriptContext.TypeError, "setGridColor(): first argument is not a number")

    if not context.argument(1).isNumber():
        context.throwError(QScriptContext.TypeError, "setGridColor(): second argument is not a number")

    if not context.argument(2).isNumber():
        context.throwError(QScriptContext.TypeError, "setGridColor(): third argument is not a number")


    r = context.argument(0).toNumber()
    g = context.argument(1).toNumber()
    b = context.argument(2).toNumber()

    #isNaN check
    if qIsNaN(r): return context.throwError(QScriptContext.TypeError, "setGridColor(): first argument failed isNaN check. There is an error in your code.")
    if qIsNaN(g): return context.throwError(QScriptContext.TypeError, "setGridColor(): second argument failed isNaN check. There is an error in your code.")
    if qIsNaN(b): return context.throwError(QScriptContext.TypeError, "setGridColor(): third argument failed isNaN check. There is an error in your code.")

    if r < 0 or r > 255: return context.throwError(QScriptContext.UnknownError, "setGridColor(): r value must be in range 0-255")
    if g < 0 or g > 255: return context.throwError(QScriptContext.UnknownError, "setGridColor(): g value must be in range 0-255")
    if b < 0 or b > 255: return context.throwError(QScriptContext.UnknownError, "setGridColor(): b value must be in range 0-255")

    mainWin().nativeSetGridColor(r, g, b)
    return QScriptValue()


def javaTextFont(context, engine):
    if context.argumentCount() != 0:
        return context.throwError("textFont() requires zero arguments")
    return QScriptValue(mainWin().nativeTextFont())


def javaTextSize(context, engine):
    if context.argumentCount() != 0:
        return context.throwError("textSize() requires zero arguments")
    return QScriptValue(mainWin().nativeTextSize())


def javaTextAngle(context, engine):
    if context.argumentCount() != 0:
        return context.throwError("textAngle() requires zero arguments")
    return QScriptValue(mainWin().nativeTextAngle())


def javaTextBold(context, engine):
    if context.argumentCount() != 0:
        return context.throwError("textBold() requires zero arguments")
    return QScriptValue(mainWin().nativeTextBold())


def javaTextItalic(context, engine):
    if context.argumentCount() != 0:
        return context.throwError("textItalic() requires zero arguments")
    return QScriptValue(mainWin().nativeTextItalic())


def javaTextUnderline(context, engine):
    if context.argumentCount() != 0:
        return context.throwError("textUnderline() requires zero arguments")
    return QScriptValue(mainWin().nativeTextUnderline())


def javaTextStrikeOut(context, engine):
    if context.argumentCount() != 0:
        return context.throwError("textStrikeOut() requires zero arguments")
    return QScriptValue(mainWin().nativeTextStrikeOut())


def javaTextOverline(context, engine):
    if context.argumentCount() != 0:
        return context.throwError("textOverline() requires zero arguments")
    return QScriptValue(mainWin().nativeTextOverline())


def javaSetTextFont(context, engine):
    if context.argumentCount() != 1:
        return context.throwError("setTextFont() requires one argument")
    if not context.argument(0).isString():
        return context.throwError(QScriptContext.TypeError, "setTextFont(): first argument is not a string")

    mainWin().nativeSetTextFont(context.argument(0).toString())
    return QScriptValue()


def javaSetTextSize(context, engine):
    if context.argumentCount() != 1:
        return context.throwError("setTextSize() requires one argument")
    if not context.argument(0).isNumber():
        context.throwError(QScriptContext.TypeError, "setTextSize(): first argument is not a number")


    num = context.argument(0).toNumber()

    #isNaN check
    if qIsNaN(num):
        return context.throwError(QScriptContext.TypeError, "setTextSize(): first argument failed isNaN check. There is an error in your code.")

    mainWin().nativeSetTextSize(num)
    return QScriptValue()


def javaSetTextAngle(context, engine):
    if context.argumentCount() != 1:
        return context.throwError("setTextAngle() requires one argument")
    if not context.argument(0).isNumber():
        context.throwError(QScriptContext.TypeError, "setTextAngle(): first argument is not a number")


    num = context.argument(0).toNumber()

    #isNaN check
    if qIsNaN(num):
        return context.throwError(QScriptContext.TypeError, "setTextAngle(): first argument failed isNaN check. There is an error in your code.")

    mainWin().nativeSetTextAngle(num)
    return QScriptValue()


def javaSetTextBold(context, engine):
    if context.argumentCount() != 1:
        return context.throwError("setTextBold() requires one argument")
    if not context.argument(0).isBool():
        return context.throwError(QScriptContext.TypeError, "setTextBold(): first argument is not a bool")

    mainWin().nativeSetTextBold(context.argument(0).toBool())
    return QScriptValue()


def javaSetTextItalic(context, engine):
    if context.argumentCount() != 1:
        return context.throwError("setTextItalic() requires one argument")
    if not context.argument(0).isBool():
        return context.throwError(QScriptContext.TypeError, "setTextItalic(): first argument is not a bool")

    mainWin().nativeSetTextItalic(context.argument(0).toBool())
    return QScriptValue()


def javaSetTextUnderline(context, engine):
    if context.argumentCount() != 1:
        return context.throwError("setTextUnderline() requires one argument")
    if not context.argument(0).isBool():
        return context.throwError(QScriptContext.TypeError, "setTextUnderline(): first argument is not a bool")

    mainWin().nativeSetTextUnderline(context.argument(0).toBool())
    return QScriptValue()


def javaSetTextStrikeOut(context, engine):
    if context.argumentCount() != 1:
        return context.throwError("setTextStrikeOut() requires one argument")
    if not context.argument(0).isBool():
        return context.throwError(QScriptContext.TypeError, "setTextStrikeOut(): first argument is not a bool")

    mainWin().nativeSetTextStrikeOut(context.argument(0).toBool())
    return QScriptValue()


def javaSetTextOverline(context, engine):
    if context.argumentCount() != 1:
        return context.throwError("setTextOverline() requires one argument")
    if not context.argument(0).isBool():
        return context.throwError(QScriptContext.TypeError, "setTextOverline(): first argument is not a bool")

    mainWin().nativeSetTextOverline(context.argument(0).toBool())
    return QScriptValue()


def javaPreviewOn(context, engine):
    if context.argumentCount() != 5:
        return context.throwError("previewOn() requires five arguments")
    if not context.argument(0).isString():
        return context.throwError(QScriptContext.TypeError, "previewOn(): first argument is not a string")
    if not context.argument(1).isString():
        return context.throwError(QScriptContext.TypeError, "previewOn(): second argument is not a string")
    if not context.argument(2).isNumber():
        context.throwError(QScriptContext.TypeError, "previewOn(): third argument is not a number")

    if not context.argument(3).isNumber():
        context.throwError(QScriptContext.TypeError, "previewOn(): fourth argument is not a number")

    if not context.argument(4).isNumber():
        context.throwError(QScriptContext.TypeError, "previewOn(): fifth argument is not a number")

    cloneStr = context.argument(0).toString().upper()
    modeStr = context.argument(1).toString().upper()
    x = context.argument(2).toNumber()
    y = context.argument(3).toNumber()
    data = context.argument(4).toNumber()

    clone = PREVIEW_CLONE_NULL
    mode = PREVIEW_MODE_NULL
    if cloneStr == "SELECTED": clone = PREVIEW_CLONE_SELECTED
    elif cloneStr == "RUBBER": clone = PREVIEW_CLONE_RUBBER
    else:
        return context.throwError(QScriptContext.UnknownError, "previewOn(): first argument must be \"SELECTED\" or \"RUBBER\".")

    if modeStr == "MOVE":       mode = PREVIEW_MODE_MOVE
    elif modeStr == "ROTATE":   mode = PREVIEW_MODE_ROTATE
    elif modeStr == "SCALE":    mode = PREVIEW_MODE_SCALE
    else:
        return context.throwError(QScriptContext.UnknownError, "previewOn(): second argument must be \"MOVE\", \"ROTATE\" or \"SCALE\".")

    #isNaN check
    if qIsNaN(x):    return context.throwError(QScriptContext.TypeError, "previewOn(): third argument failed isNaN check. There is an error in your code.")
    if qIsNaN(y):    return context.throwError(QScriptContext.TypeError, "previewOn(): fourth argument failed isNaN check. There is an error in your code.")
    if qIsNaN(data): return context.throwError(QScriptContext.TypeError, "previewOn(): fifth argument failed isNaN check. There is an error in your code.")

    mainWin().nativePreviewOn(clone, mode, x, y, data)
    return QScriptValue()


def javaPreviewOff(context, engine):
    if context.argumentCount() != 0:
        return context.throwError("previewOff() requires zero arguments")

    mainWin().nativePreviewOff()
    return QScriptValue()


def javaVulcanize(context, engine):
    if context.argumentCount() != 0:
        return context.throwError("vulcanize() requires zero arguments")

    mainWin().nativeVulcanize()
    return QScriptValue()


def javaAllowRubber(context, engine):
    if context.argumentCount() != 0:
        return context.throwError("allowRubber() requires zero arguments")

    return QScriptValue(mainWin().nativeAllowRubber())


def javaSetRubberMode(context, engine):
    if context.argumentCount() != 1:
        return context.throwError("setRubberMode() requires one argument")
    if not context.argument(0).isString():
        return context.throwError(QScriptContext.TypeError, "setRubberMode(): first argument is not a string")

    mode = context.argument(0).toString().upper()

    if mode == "CIRCLE_1P_RAD":                       mainWin().nativeSetRubberMode(OBJ_RUBBER_CIRCLE_1P_RAD)
    elif mode == "CIRCLE_1P_DIA":                     mainWin().nativeSetRubberMode(OBJ_RUBBER_CIRCLE_1P_DIA)
    elif mode == "CIRCLE_2P":                         mainWin().nativeSetRubberMode(OBJ_RUBBER_CIRCLE_2P)
    elif mode == "CIRCLE_3P":                         mainWin().nativeSetRubberMode(OBJ_RUBBER_CIRCLE_3P)
    elif mode == "CIRCLE_TTR":                        mainWin().nativeSetRubberMode(OBJ_RUBBER_CIRCLE_TTR)
    elif mode == "CIRCLE_TTR":                        mainWin().nativeSetRubberMode(OBJ_RUBBER_CIRCLE_TTT)

    elif mode == "DIMLEADER_LINE":                    mainWin().nativeSetRubberMode(OBJ_RUBBER_DIMLEADER_LINE)

    elif mode == "ELLIPSE_LINE":                      mainWin().nativeSetRubberMode(OBJ_RUBBER_ELLIPSE_LINE)
    elif mode == "ELLIPSE_MAJORDIAMETER_MINORRADIUS": mainWin().nativeSetRubberMode(OBJ_RUBBER_ELLIPSE_MAJORDIAMETER_MINORRADIUS)
    elif mode == "ELLIPSE_MAJORRADIUS_MINORRADIUS":   mainWin().nativeSetRubberMode(OBJ_RUBBER_ELLIPSE_MAJORRADIUS_MINORRADIUS)
    elif mode == "ELLIPSE_ROTATION":                  mainWin().nativeSetRubberMode(OBJ_RUBBER_ELLIPSE_ROTATION)

    elif mode == "LINE":                              mainWin().nativeSetRubberMode(OBJ_RUBBER_LINE)

    elif mode == "POLYGON":                           mainWin().nativeSetRubberMode(OBJ_RUBBER_POLYGON)
    elif mode == "POLYGON_INSCRIBE":                  mainWin().nativeSetRubberMode(OBJ_RUBBER_POLYGON_INSCRIBE)
    elif mode == "POLYGON_CIRCUMSCRIBE":              mainWin().nativeSetRubberMode(OBJ_RUBBER_POLYGON_CIRCUMSCRIBE)

    elif mode == "POLYLINE":                          mainWin().nativeSetRubberMode(OBJ_RUBBER_POLYLINE)

    elif mode == "RECTANGLE":                         mainWin().nativeSetRubberMode(OBJ_RUBBER_RECTANGLE)

    elif mode == "TEXTSINGLE":                        mainWin().nativeSetRubberMode(OBJ_RUBBER_TEXTSINGLE)

    else:
        context.throwError(QScriptContext.UnknownError, "setRubberMode(): unknown rubberMode value")

    return QScriptValue()


def javaSetRubberPoint(context, engine):
    if context.argumentCount() != 3:
        return context.throwError("setRubberPoint() requires three arguments")

    if not context.argument(0).isString():
        return context.throwError(QScriptContext.TypeError, "setRubberPoint(): first argument is not a string")

    if not context.argument(1).isNumber():
        context.throwError(QScriptContext.TypeError, "setRubberPoint(): second argument is not a number")

    if not context.argument(2).isNumber():
        context.throwError(QScriptContext.TypeError, "setRubberPoint(): third argument is not a number")

    key = context.argument(0).toString().upper()
    x = context.argument(1).toNumber()
    y = context.argument(2).toNumber()

    #isNaN check
    if qIsNaN(x): return context.throwError(QScriptContext.TypeError, "setRubberPoint(): second argument failed isNaN check. There is an error in your code.")
    if qIsNaN(y): return context.throwError(QScriptContext.TypeError, "setRubberPoint(): third argument failed isNaN check. There is an error in your code.")

    mainWin().nativeSetRubberPoint(key, x, y)
    return QScriptValue()


def javaSetRubberText(context, engine):
    if context.argumentCount() != 2:
        return context.throwError("setRubberText() requires two arguments")

    if not context.argument(0).isString():
        return context.throwError(QScriptContext.TypeError, "setRubberText(): first argument is not a string")

    if not context.argument(1).isString():
        return context.throwError(QScriptContext.TypeError, "setRubberText(): second argument is not a string")

    key = context.argument(0).toString().upper()
    txt = context.argument(1).toString()

    mainWin().nativeSetRubberText(key, txt)
    return QScriptValue()


def javaAddRubber(context, engine):
    if context.argumentCount() != 1:
        return context.throwError("addRubber() requires one argument")

    if not context.argument(0).isString():
        return context.throwError(QScriptContext.TypeError, "addRubber(): first argument is not a string")

    objType = context.argument(0).toString().upper()

    if not mainWin().nativeAllowRubber():
        return context.throwError(QScriptContext.UnknownError, "addRubber(): You must use vulcanize() before you can add another rubber object.")

    mx = mainWin().nativeMouseX()
    my = mainWin().nativeMouseY()

    if objType == "ARC":            pass  #TODO: handle self type
    elif objType == "BLOCK":        pass  #TODO: handle self type
    elif objType == "CIRCLE":       mainWin().nativeAddCircle(mx, my, 0, False, OBJ_RUBBER_ON)
    elif objType == "DIMALIGNED":   pass  #TODO: handle self type
    elif objType == "DIMANGULAR":   pass  #TODO: handle self type
    elif objType == "DIMARCLENGTH": pass  #TODO: handle self type
    elif objType == "DIMDIAMETER":  pass  #TODO: handle self type
    elif objType == "DIMLEADER":    mainWin().nativeAddDimLeader(mx, my, mx, my, 0, OBJ_RUBBER_ON)
    elif objType == "DIMLINEAR":    pass  #TODO: handle self type
    elif objType == "DIMORDINATE":  pass  #TODO: handle self type
    elif objType == "DIMRADIUS":    pass  #TODO: handle self type
    elif objType == "ELLIPSE":      mainWin().nativeAddEllipse(mx, my, 0, 0, 0, 0, OBJ_RUBBER_ON)
    elif objType == "ELLIPSEARC":   pass  #TODO: handle self type
    elif objType == "HATCH":        pass  #TODO: handle self type
    elif objType == "IMAGE":        pass  #TODO: handle self type
    elif objType == "INFINITELINE": pass  #TODO: handle self type
    elif objType == "LINE":         mainWin().nativeAddLine(mx, my, mx, my, 0, OBJ_RUBBER_ON)
    elif objType == "PATH":         pass  #TODO: handle self type
    elif objType == "POINT":        pass  #TODO: handle self type
    elif objType == "POLYGON":      mainWin().nativeAddPolygon(mx, my, QPainterPath(), OBJ_RUBBER_ON)
    elif objType == "POLYLINE":     mainWin().nativeAddPolyline(mx, my, QPainterPath(), OBJ_RUBBER_ON)
    elif objType == "RAY":          pass  #TODO: handle self type
    elif objType == "RECTANGLE":    mainWin().nativeAddRectangle(mx, my, mx, my, 0, 0, OBJ_RUBBER_ON)
    elif objType == "SPLINE":       pass  #TODO: handle self type
    elif objType == "TEXTMULTI":    pass  #TODO: handle self type
    elif objType == "TEXTSINGLE":   mainWin().nativeAddTextSingle("", mx, my, 0, False, OBJ_RUBBER_ON)

    return QScriptValue()


def javaClearRubber(context, engine):
    if context.argumentCount() != 0:
        return context.throwError("clearRubber() requires zero arguments")

    mainWin().nativeClearRubber()
    return QScriptValue()


def javaSpareRubber(context, engine):
    if context.argumentCount() != 1:
        return context.throwError("spareRubber() requires one argument")
    if not context.argument(0).isString():
        return context.throwError(QScriptContext.TypeError, "spareRubber(): first argument is not a string")

    objID = context.argument(0).toString().upper()

    if objID == "PATH":       mainWin().nativeSpareRubber(SPARE_RUBBER_PATH)
    elif objID == "POLYGON":  mainWin().nativeSpareRubber(SPARE_RUBBER_POLYGON)
    elif objID == "POLYLINE": mainWin().nativeSpareRubber(SPARE_RUBBER_POLYLINE)
    else:
        ## ok = False
        ## id = objID.toLongLong(&ok)
        ## if not ok:
        ##     return context.throwError(QScriptContext.TypeError, "spareRubber(): error converting object ID into an int64")
        try:
            id_ = int(objID)
        except ValueError:
            return context.throwError(QScriptContext.TypeError, "spareRubber(): error converting object ID into an int64")

        mainWin().nativeSpareRubber(id_)

    return QScriptValue()


def javaAddTextMulti(context, engine):
    if context.argumentCount() != 5:
        return context.throwError("addTextMulti() requires five arguments")
    if not context.argument(0).isString():
        return context.throwError(QScriptContext.TypeError, "addTextMulti(): first argument is not a string")
    if not context.argument(1).isNumber():
        context.throwError(QScriptContext.TypeError, "addTextMulti(): second argument is not a number")

    if not context.argument(2).isNumber():
        context.throwError(QScriptContext.TypeError, "addTextMulti(): third argument is not a number")

    if not context.argument(3).isNumber():
        context.throwError(QScriptContext.TypeError, "addTextMulti(): fourth argument is not a number")

    if not context.argument(4).isBool():
        return context.throwError(QScriptContext.TypeError, "addTextMulti(): fifth argument is not a bool")

    strng = context.argument(0).toString()
    x = context.argument(1).toNumber()
    y = context.argument(2).toNumber()
    rot = context.argument(3).toNumber()
    fill = context.argument(4).toBool()

    #isNaN check
    if qIsNaN(x):   return context.throwError(QScriptContext.TypeError, "addTextMulti(): second argument failed isNaN check. There is an error in your code.")
    if qIsNaN(y):   return context.throwError(QScriptContext.TypeError, "addTextMulti(): third argument failed isNaN check. There is an error in your code.")
    if qIsNaN(rot): return context.throwError(QScriptContext.TypeError, "addTextMulti(): fourth argument failed isNaN check. There is an error in your code.")

    mainWin().nativeAddTextMulti(strng, x, y, rot, fill, OBJ_RUBBER_OFF)
    return QScriptValue()


def javaAddTextSingle(context, engine):
    if context.argumentCount() != 5:
        return context.throwError("addTextSingle() requires five arguments")
    if not context.argument(0).isString():
        return context.throwError(QScriptContext.TypeError, "addTextSingle(): first argument is not a string")
    if not context.argument(1).isNumber():
        context.throwError(QScriptContext.TypeError, "addTextSingle(): second argument is not a number")

    if not context.argument(2).isNumber():
        context.throwError(QScriptContext.TypeError, "addTextSingle(): third argument is not a number")

    if not context.argument(3).isNumber():
        context.throwError(QScriptContext.TypeError, "addTextSingle(): fourth argument is not a number")

    if not context.argument(4).isBool():
        return context.throwError(QScriptContext.TypeError, "addTextSingle(): fifth argument is not a bool")

    strng = context.argument(0).toString()
    x = context.argument(1).toNumber()
    y = context.argument(2).toNumber()
    rot = context.argument(3).toNumber()
    fill = context.argument(4).toBool()

    #isNaN check
    if qIsNaN(x):   return context.throwError(QScriptContext.TypeError, "addTextSingle(): second argument failed isNaN check. There is an error in your code.")
    if qIsNaN(y):   return context.throwError(QScriptContext.TypeError, "addTextSingle(): third argument failed isNaN check. There is an error in your code.")
    if qIsNaN(rot): return context.throwError(QScriptContext.TypeError, "addTextSingle(): fourth argument failed isNaN check. There is an error in your code.")

    mainWin().nativeAddTextSingle(strng, x, y, rot, fill, OBJ_RUBBER_OFF)
    return QScriptValue()


def javaAddInfiniteLine(context, engine):
    #TODO: parameter error checking
    qDebug("TODO: finish addInfiniteLine command")
    return QScriptValue()


def javaAddRay(context, engine):
    #TODO: parameter error checking
    qDebug("TODO: finish addRay command")
    return QScriptValue()


def javaAddLine(context, engine):
    if context.argumentCount() != 5:
        return context.throwError("addLine() requires five arguments")
    if not context.argument(0).isNumber():
        context.throwError(QScriptContext.TypeError, "addLine(): first argument is not a number")

    if not context.argument(1).isNumber():
        context.throwError(QScriptContext.TypeError, "addLine(): second argument is not a number")

    if not context.argument(2).isNumber():
        context.throwError(QScriptContext.TypeError, "addLine(): third argument is not a number")

    if not context.argument(3).isNumber():
        context.throwError(QScriptContext.TypeError, "addLine(): fourth argument is not a number")

    if not context.argument(4).isNumber():
        context.throwError(QScriptContext.TypeError, "addLine(): fifth argument is not a number")

    x1 = context.argument(0).toNumber()
    y1 = context.argument(1).toNumber()
    x2 = context.argument(2).toNumber()
    y2 = context.argument(3).toNumber()
    rot = context.argument(4).toNumber()

    #isNaN check
    if qIsNaN(x1):  return context.throwError(QScriptContext.TypeError, "addLine(): first argument failed isNaN check. There is an error in your code.")
    if qIsNaN(y1):  return context.throwError(QScriptContext.TypeError, "addLine(): second argument failed isNaN check. There is an error in your code.")
    if qIsNaN(x2):  return context.throwError(QScriptContext.TypeError, "addLine(): third argument failed isNaN check. There is an error in your code.")
    if qIsNaN(y2):  return context.throwError(QScriptContext.TypeError, "addLine(): fourth argument failed isNaN check. There is an error in your code.")
    if qIsNaN(rot): return context.throwError(QScriptContext.TypeError, "addLine(): fifth argument failed isNaN check. There is an error in your code.")

    mainWin().nativeAddLine(x1, y1, x2, y2, rot, OBJ_RUBBER_OFF)
    return QScriptValue()


def javaAddTriangle(context, engine):
    if context.argumentCount() != 8:
        return context.throwError("addTriangle() requires eight arguments")
    if not context.argument(0).isNumber():
        context.throwError(QScriptContext.TypeError, "addTriangle(): first argument is not a number")

    if not context.argument(1).isNumber():
        context.throwError(QScriptContext.TypeError, "addTriangle(): second argument is not a number")

    if not context.argument(2).isNumber():
        context.throwError(QScriptContext.TypeError, "addTriangle(): third argument is not a number")

    if not context.argument(3).isNumber():
        context.throwError(QScriptContext.TypeError, "addTriangle(): fourth argument is not a number")

    if not context.argument(4).isNumber():
        context.throwError(QScriptContext.TypeError, "addTriangle(): fifth argument is not a number")

    if not context.argument(5).isNumber():
        context.throwError(QScriptContext.TypeError, "addTriangle(): sixth argument is not a number")

    if not context.argument(6).isNumber():
        context.throwError(QScriptContext.TypeError, "addTriangle(): seventh argument is not a number")

    if not context.argument(7).isBool():
        return context.throwError(QScriptContext.TypeError, "addTriangle(): eighth argument is not a bool")

    x1 = context.argument(0).toNumber()
    y1 = context.argument(1).toNumber()
    x2 = context.argument(2).toNumber()
    y2 = context.argument(3).toNumber()
    x3 = context.argument(4).toNumber()
    y3 = context.argument(5).toNumber()
    rot = context.argument(6).toNumber()
    fill = context.argument(7).toBool()

    #isNaN check
    if qIsNaN(x1):  return context.throwError(QScriptContext.TypeError, "addTriangle(): first argument failed isNaN check. There is an error in your code.")
    if qIsNaN(y1):  return context.throwError(QScriptContext.TypeError, "addTriangle(): second argument failed isNaN check. There is an error in your code.")
    if qIsNaN(x2):  return context.throwError(QScriptContext.TypeError, "addTriangle(): third argument failed isNaN check. There is an error in your code.")
    if qIsNaN(y2):  return context.throwError(QScriptContext.TypeError, "addTriangle(): fourth argument failed isNaN check. There is an error in your code.")
    if qIsNaN(x3):  return context.throwError(QScriptContext.TypeError, "addTriangle(): fifth argument failed isNaN check. There is an error in your code.")
    if qIsNaN(y3):  return context.throwError(QScriptContext.TypeError, "addTriangle(): sixth argument failed isNaN check. There is an error in your code.")
    if qIsNaN(rot): return context.throwError(QScriptContext.TypeError, "addTriangle(): seventh argument failed isNaN check. There is an error in your code.")

    mainWin().nativeAddTriangle(x1, y1, x2, y2, x3, y3, rot, fill)
    return QScriptValue()


def javaAddRectangle(context, engine):
    if context.argumentCount() != 6:
        return context.throwError("addRectangle() requires six arguments")
    if not context.argument(0).isNumber():
        context.throwError(QScriptContext.TypeError, "addRectangle(): first argument is not a number")

    if not context.argument(1).isNumber():
        context.throwError(QScriptContext.TypeError, "addRectangle(): second argument is not a number")

    if not context.argument(2).isNumber():
        context.throwError(QScriptContext.TypeError, "addRectangle(): third argument is not a number")

    if not context.argument(3).isNumber():
        context.throwError(QScriptContext.TypeError, "addRectangle(): fourth argument is not a number")

    if not context.argument(4).isNumber():
        context.throwError(QScriptContext.TypeError, "addRectangle(): fifth argument is not a number")

    if not context.argument(5).isBool():
        return context.throwError(QScriptContext.TypeError, "addRectangle(): sixth argument is not a bool")

    x = context.argument(0).toNumber()
    y = context.argument(1).toNumber()
    w = context.argument(2).toNumber()
    h = context.argument(3).toNumber()
    rot = context.argument(4).toNumber()
    fill = context.argument(5).toBool()

    #isNaN check
    if qIsNaN(x):   return context.throwError(QScriptContext.TypeError, "addRectangle(): first argument failed isNaN check. There is an error in your code.")
    if qIsNaN(y):   return context.throwError(QScriptContext.TypeError, "addRectangle(): second argument failed isNaN check. There is an error in your code.")
    if qIsNaN(w):   return context.throwError(QScriptContext.TypeError, "addRectangle(): third argument failed isNaN check. There is an error in your code.")
    if qIsNaN(h):   return context.throwError(QScriptContext.TypeError, "addRectangle(): fourth argument failed isNaN check. There is an error in your code.")
    if qIsNaN(rot): return context.throwError(QScriptContext.TypeError, "addRectangle(): fifth argument failed isNaN check. There is an error in your code.")

    mainWin().nativeAddRectangle(x, y, w, h, rot, fill, OBJ_RUBBER_OFF)
    return QScriptValue()


def javaAddRoundedRectangle(context, engine):
    if context.argumentCount() != 7:
        return context.throwError("addRoundedRectangle() requires seven arguments")
    if not context.argument(0).isNumber():
        context.throwError(QScriptContext.TypeError, "addRoundedRectangle(): first argument is not a number")

    if not context.argument(1).isNumber():
        context.throwError(QScriptContext.TypeError, "addRoundedRectangle(): second argument is not a number")

    if not context.argument(2).isNumber():
        context.throwError(QScriptContext.TypeError, "addRoundedRectangle(): third argument is not a number")

    if not context.argument(3).isNumber():
        context.throwError(QScriptContext.TypeError, "addRoundedRectangle(): fourth argument is not a number")

    if not context.argument(4).isNumber():
        context.throwError(QScriptContext.TypeError, "addRoundedRectangle(): fifth argument is not a number")

    if not context.argument(5).isNumber():
        context.throwError(QScriptContext.TypeError, "addRoundedRectangle(): sixth argument is not a number")

    if not context.argument(6).isBool():
        return context.throwError(QScriptContext.TypeError, "addRoundedRectangle(): seventh argument is not a bool")

    x = context.argument(0).toNumber()
    y = context.argument(1).toNumber()
    w = context.argument(2).toNumber()
    h = context.argument(3).toNumber()
    rad = context.argument(4).toNumber()
    rot = context.argument(5).toNumber()
    fill = context.argument(6).toBool()

    #isNaN check
    if qIsNaN(x):   return context.throwError(QScriptContext.TypeError, "addRoundedRectangle(): first argument failed isNaN check. There is an error in your code.")
    if qIsNaN(y):   return context.throwError(QScriptContext.TypeError, "addRoundedRectangle(): second argument failed isNaN check. There is an error in your code.")
    if qIsNaN(w):   return context.throwError(QScriptContext.TypeError, "addRoundedRectangle(): third argument failed isNaN check. There is an error in your code.")
    if qIsNaN(h):   return context.throwError(QScriptContext.TypeError, "addRoundedRectangle(): fourth argument failed isNaN check. There is an error in your code.")
    if qIsNaN(rad): return context.throwError(QScriptContext.TypeError, "addRoundedRectangle(): fifth argument failed isNaN check. There is an error in your code.")
    if qIsNaN(rot): return context.throwError(QScriptContext.TypeError, "addRoundedRectangle(): sixth argument failed isNaN check. There is an error in your code.")

    mainWin().nativeAddRoundedRectangle(x, y, w, h, rad, rot, fill)
    return QScriptValue()


def javaAddArc(context, engine):
    if context.argumentCount() != 6:
        return context.throwError("addArc() requires six arguments")
    if not context.argument(0).isNumber():
        context.throwError(QScriptContext.TypeError, "addArc(): first argument is not a number")

    if not context.argument(1).isNumber():
        context.throwError(QScriptContext.TypeError, "addArc(): second argument is not a number")

    if not context.argument(2).isNumber():
        context.throwError(QScriptContext.TypeError, "addArc(): third argument is not a number")

    if not context.argument(3).isNumber():
        context.throwError(QScriptContext.TypeError, "addArc(): fourth argument is not a number")

    if not context.argument(4).isNumber():
        context.throwError(QScriptContext.TypeError, "addArc(): fifth argument is not a number")

    if not context.argument(5).isNumber():
        context.throwError(QScriptContext.TypeError, "addArc(): sixth argument is not a number")


    startX = context.argument(0).toNumber()
    startY = context.argument(1).toNumber()
    midX = context.argument(2).toNumber()
    midY = context.argument(3).toNumber()
    endX = context.argument(4).toNumber()
    endY = context.argument(5).toNumber()

    #isNaN check
    if qIsNaN(startX): return context.throwError(QScriptContext.TypeError, "addArc(): first argument failed isNaN check. There is an error in your code.")
    if qIsNaN(startY): return context.throwError(QScriptContext.TypeError, "addArc(): second argument failed isNaN check. There is an error in your code.")
    if qIsNaN(midX):   return context.throwError(QScriptContext.TypeError, "addArc(): third argument failed isNaN check. There is an error in your code.")
    if qIsNaN(midY):   return context.throwError(QScriptContext.TypeError, "addArc(): fourth argument failed isNaN check. There is an error in your code.")
    if qIsNaN(endX):   return context.throwError(QScriptContext.TypeError, "addArc(): fifth argument failed isNaN check. There is an error in your code.")
    if qIsNaN(endY):   return context.throwError(QScriptContext.TypeError, "addArc(): sixth argument failed isNaN check. There is an error in your code.")

    mainWin().nativeAddArc(startX, startY, midX, midY, endX, endY, OBJ_RUBBER_OFF)
    return QScriptValue()


def javaAddCircle(context, engine):
    if context.argumentCount() != 4:
        return context.throwError("addCircle() requires four arguments")
    if not context.argument(0).isNumber():
        context.throwError(QScriptContext.TypeError, "addCircle(): first argument is not a number")

    if not context.argument(1).isNumber():
        context.throwError(QScriptContext.TypeError, "addCircle(): second argument is not a number")

    if not context.argument(2).isNumber():
        context.throwError(QScriptContext.TypeError, "addCircle(): third argument is not a number")

    if not context.argument(3).isBool():
        return context.throwError(QScriptContext.TypeError, "addCircle(): fourth argument is not a bool")

    centerX = context.argument(0).toNumber()
    centerY = context.argument(1).toNumber()
    radius = context.argument(2).toNumber()
    fill = context.argument(3).toBool()

    #isNaN check
    if qIsNaN(centerX): return context.throwError(QScriptContext.TypeError, "addCircle(): first argument failed isNaN check. There is an error in your code.")
    if qIsNaN(centerY): return context.throwError(QScriptContext.TypeError, "addCircle(): second argument failed isNaN check. There is an error in your code.")
    if qIsNaN(radius):  return context.throwError(QScriptContext.TypeError, "addCircle(): third argument failed isNaN check. There is an error in your code.")

    mainWin().nativeAddCircle(centerX, centerY, radius, fill, OBJ_RUBBER_OFF)
    return QScriptValue()


def javaAddSlot(context, engine):
    if context.argumentCount() != 6:
        return context.throwError("addSlot() requires six arguments")
    if not context.argument(0).isNumber():
        context.throwError(QScriptContext.TypeError, "addSlot(): first argument is not a number")

    if not context.argument(1).isNumber():
        context.throwError(QScriptContext.TypeError, "addSlot(): second argument is not a number")

    if not context.argument(2).isNumber():
        context.throwError(QScriptContext.TypeError, "addSlot(): third argument is not a number")

    if not context.argument(3).isNumber():
        context.throwError(QScriptContext.TypeError, "addSlot(): fourth argument is not a number")

    if not context.argument(4).isNumber():
        context.throwError(QScriptContext.TypeError, "addSlot(): fifth argument is not a number")

    if not context.argument(5).isBool():
        return context.throwError(QScriptContext.TypeError, "addSlot(): sixth argument is not a bool")

    centerX = context.argument(0).toNumber()
    centerY = context.argument(1).toNumber()
    diameter = context.argument(2).toNumber()
    length = context.argument(3).toNumber()
    rot = context.argument(4).toNumber()
    fill = context.argument(5).toBool()

    #isNaN check
    if qIsNaN(centerX):  return context.throwError(QScriptContext.TypeError, "addSlot(): first argument failed isNaN check. There is an error in your code.")
    if qIsNaN(centerY):  return context.throwError(QScriptContext.TypeError, "addSlot(): second argument failed isNaN check. There is an error in your code.")
    if qIsNaN(diameter): return context.throwError(QScriptContext.TypeError, "addSlot(): third argument failed isNaN check. There is an error in your code.")
    if qIsNaN(length):   return context.throwError(QScriptContext.TypeError, "addSlot(): fourth argument failed isNaN check. There is an error in your code.")
    if qIsNaN(rot):      return context.throwError(QScriptContext.TypeError, "addSlot(): fifth argument failed isNaN check. There is an error in your code.")

    mainWin().nativeAddSlot(centerX, centerY, diameter, length, rot, fill, OBJ_RUBBER_OFF)
    return QScriptValue()


def javaAddEllipse(context, engine):
    if context.argumentCount() != 6:
        return context.throwError("addEllipse() requires six arguments")
    if not context.argument(0).isNumber():
        context.throwError(QScriptContext.TypeError, "addEllipse(): first argument is not a number")

    if not context.argument(1).isNumber():
        context.throwError(QScriptContext.TypeError, "addEllipse(): second argument is not a number")

    if not context.argument(2).isNumber():
        context.throwError(QScriptContext.TypeError, "addEllipse(): third argument is not a number")

    if not context.argument(3).isNumber():
        context.throwError(QScriptContext.TypeError, "addEllipse(): fourth argument is not a number")

    if not context.argument(4).isNumber():
        context.throwError(QScriptContext.TypeError, "addEllipse(): fifth argument is not a number")

    if not context.argument(5).isBool():
        return context.throwError(QScriptContext.TypeError, "addEllipse(): sixth argument is not a bool")

    centerX = context.argument(0).toNumber()
    centerY = context.argument(1).toNumber()
    radX = context.argument(2).toNumber()
    radY = context.argument(3).toNumber()
    rot = context.argument(4).toNumber()
    fill = context.argument(5).toBool()

    #isNaN check
    if qIsNaN(centerX): return context.throwError(QScriptContext.TypeError, "addEllipse(): first argument failed isNaN check. There is an error in your code.")
    if qIsNaN(centerY): return context.throwError(QScriptContext.TypeError, "addEllipse(): second argument failed isNaN check. There is an error in your code.")
    if qIsNaN(radX):    return context.throwError(QScriptContext.TypeError, "addEllipse(): third argument failed isNaN check. There is an error in your code.")
    if qIsNaN(radY):    return context.throwError(QScriptContext.TypeError, "addEllipse(): fourth argument failed isNaN check. There is an error in your code.")
    if qIsNaN(rot):     return context.throwError(QScriptContext.TypeError, "addEllipse(): fifth argument failed isNaN check. There is an error in your code.")

    mainWin().nativeAddEllipse(centerX, centerY, radX, radY, rot, fill, OBJ_RUBBER_OFF)
    return QScriptValue()


def javaAddPoint(context, engine):
    if context.argumentCount() != 2:
        return context.throwError("addPoint() requires two arguments")
    if not context.argument(0).isNumber():
        context.throwError(QScriptContext.TypeError, "addPoint(): first argument is not a number")

    if not context.argument(1).isNumber():
        context.throwError(QScriptContext.TypeError, "addPoint(): second argument is not a number")


    x = context.argument(0).toNumber()
    y = context.argument(1).toNumber()

    #isNaN check
    if qIsNaN(x): return context.throwError(QScriptContext.TypeError, "addPoint(): first argument failed isNaN check. There is an error in your code.")
    if qIsNaN(y): return context.throwError(QScriptContext.TypeError, "addPoint(): second argument failed isNaN check. There is an error in your code.")

    mainWin().nativeAddPoint(x,y)
    return QScriptValue()


def javaAddRegularPolygon(context, engine):
    #TODO: parameter error checking
    qDebug("TODO: finish addRegularPolygon command")
    return QScriptValue()


def javaAddPolygon(context, engine):
    if context.argumentCount() != 1:
        return context.throwError("addPolygon() requires one argument")
    if not context.argument(0).isArray():
        return context.throwError(QScriptContext.TypeError, "addPolygon(): first argument is not an array")

    varList = context.argument(0).toVariant()  # .toList()
    varSize = len(varList)  # .size()
    if varSize < 2:
        return context.throwError(QScriptContext.TypeError, "addPolygon(): array must contain at least two elements")
    if varSize % 2:
        return context.throwError(QScriptContext.TypeError, "addPolygon(): array cannot contain an odd number of elements")

    lineTo = False
    xCoord = True
    x = 0
    y = 0
    startX = 0
    startY = 0
    path = QPainterPath()
    for var in varList:
        ## if var.canConvert(QVariant.Double):
        ##     if xCoord:
        ##         xCoord = False
        ##         x = var.toReal()
        ##     else:
        ##         xCoord = True
        ##         y = -var.toReal()

        ##         if lineTo: path.lineTo(x,y)
        ##         else     : path.moveTo(x,y); lineTo = True; startX = x; startY = y
        ## else:
        ##     return context.throwError(QScriptContext.TypeError, "addPolygon(): array contains one or more invalid elements")
        try:
            float(var)
        except ValueError:
            return context.throwError(QScriptContext.TypeError, "addPolygon(): array contains one or more invalid elements")
        else:
            if xCoord:
                xCoord = False
                x = float(var)
            else:
                xCoord = True
                y = -float(var)

                if lineTo:
                    path.lineTo(x,y)
                else     :
                    path.moveTo(x,y); lineTo = True; startX = x; startY = y

    #Close the polygon
    path.closeSubpath()

    path.translate(-startX, -startY)

    mainWin().nativeAddPolygon(startX, startY, path, OBJ_RUBBER_OFF)
    return QScriptValue()


def javaAddPolyline(context, engine):
    if context.argumentCount() != 1:
        return context.throwError("addPolyline() requires one argument")
    if not context.argument(0).isArray():
        return context.throwError(QScriptContext.TypeError, "addPolyline(): first argument is not an array")

    varList = context.argument(0).toVariant()  # .toList()
    varSize = len(varList)  # .size()
    if varSize < 2:
        return context.throwError(QScriptContext.TypeError, "addPolyline(): array must contain at least two elements")
    if varSize % 2:
        return context.throwError(QScriptContext.TypeError, "addPolyline(): array cannot contain an odd number of elements")

    lineTo = False
    xCoord = True
    x = 0
    y = 0
    startX = 0
    startY = 0
    path = QPainterPath()
    for var in varList:
        try:
            float(var)
        except ValueError:
            return context.throwError(QScriptContext.TypeError, "addPolyline(): array contains one or more invalid elements")
        else:
            if xCoord:
                xCoord = False
                x = float(var)

            else:
                xCoord = True
                y = -float(var)

                if lineTo:
                    path.lineTo(x,y)
                else     :
                    path.moveTo(x,y); lineTo = True; startX = x; startY = y

    path.translate(-startX, -startY)

    mainWin().nativeAddPolyline(startX, startY, path, OBJ_RUBBER_OFF)
    return QScriptValue()


def javaAddPath(context, engine):
    #TODO: parameter error checking
    qDebug("TODO: finish addPath command")
    return QScriptValue()


def javaAddHorizontalDimension(context, engine):
    #TODO: parameter error checking
    qDebug("TODO: finish addHorizontalDimension command")
    return QScriptValue()


def javaAddVerticalDimension(context, engine):
    #TODO: parameter error checking
    qDebug("TODO: finish addVerticalDimension command")
    return QScriptValue()


def javaAddImage(context, engine):
    #TODO: parameter error checking
    qDebug("TODO: finish addImage command")
    return QScriptValue()


def javaAddDimLeader(context, engine):
    if context.argumentCount() != 5:
        return context.throwError("addDimLeader() requires five arguments")
    if not context.argument(0).isNumber():
        context.throwError(QScriptContext.TypeError, "addDimLeader(): first argument is not a number")

    if not context.argument(1).isNumber():
        context.throwError(QScriptContext.TypeError, "addDimLeader(): second argument is not a number")

    if not context.argument(2).isNumber():
        context.throwError(QScriptContext.TypeError, "addDimLeader(): third argument is not a number")

    if not context.argument(3).isNumber():
        context.throwError(QScriptContext.TypeError, "addDimLeader(): fourth argument is not a number")

    if not context.argument(4).isNumber():
        context.throwError(QScriptContext.TypeError, "addDimLeader(): fifth argument is not a number")


    x1 = context.argument(0).toNumber()
    y1 = context.argument(1).toNumber()
    x2 = context.argument(2).toNumber()
    y2 = context.argument(3).toNumber()
    rot = context.argument(4).toNumber()

    #isNaN check
    if qIsNaN(x1):  return context.throwError(QScriptContext.TypeError, "addDimLeader(): first argument failed isNaN check. There is an error in your code.")
    if qIsNaN(y1):  return context.throwError(QScriptContext.TypeError, "addDimLeader(): second argument failed isNaN check. There is an error in your code.")
    if qIsNaN(x2):  return context.throwError(QScriptContext.TypeError, "addDimLeader(): third argument failed isNaN check. There is an error in your code.")
    if qIsNaN(y2):  return context.throwError(QScriptContext.TypeError, "addDimLeader(): fourth argument failed isNaN check. There is an error in your code.")
    if qIsNaN(rot): return context.throwError(QScriptContext.TypeError, "addDimLeader(): fifth argument failed isNaN check. There is an error in your code.")

    mainWin().nativeAddDimLeader(x1, y1, x2, y2, rot, OBJ_RUBBER_OFF)
    return QScriptValue()


def javaSetCursorShape(context, engine):
    if context.argumentCount() != 1:
        return context.throwError("setCursorShape() requires one argument")
    if not context.argument(0).isString():
        return context.throwError(QScriptContext.TypeError, "setCursorShape(): first argument is not a string")

    shape = context.argument(0).toString()
    mainWin().nativeSetCursorShape(shape)
    return QScriptValue()


def javaCalculateAngle(context, engine):
    if context.argumentCount() != 4:
        return context.throwError("calculateAngle() requires four arguments")
    if not context.argument(0).isNumber():
        context.throwError(QScriptContext.TypeError, "calculateAngle(): first argument is not a number")

    if not context.argument(1).isNumber():
        context.throwError(QScriptContext.TypeError, "calculateAngle(): second argument is not a number")

    if not context.argument(2).isNumber():
        context.throwError(QScriptContext.TypeError, "calculateAngle(): third argument is not a number")

    if not context.argument(3).isNumber():
        context.throwError(QScriptContext.TypeError, "calculateAngle(): fourth argument is not a number")


    x1 = context.argument(0).toNumber()
    y1 = context.argument(1).toNumber()
    x2 = context.argument(2).toNumber()
    y2 = context.argument(3).toNumber()

    #isNaN check
    if qIsNaN(x1): return context.throwError(QScriptContext.TypeError, "calculateAngle(): first argument failed isNaN check. There is an error in your code.")
    if qIsNaN(y1): return context.throwError(QScriptContext.TypeError, "calculateAngle(): second argument failed isNaN check. There is an error in your code.")
    if qIsNaN(x2): return context.throwError(QScriptContext.TypeError, "calculateAngle(): third argument failed isNaN check. There is an error in your code.")
    if qIsNaN(y2): return context.throwError(QScriptContext.TypeError, "calculateAngle(): fourth argument failed isNaN check. There is an error in your code.")

    return QScriptValue(mainWin().nativeCalculateAngle(x1, y1, x2, y2))


def javaCalculateDistance(context, engine):
    if context.argumentCount() != 4:
        return context.throwError("calculateDistance() requires four arguments")
    if not context.argument(0).isNumber():
        context.throwError(QScriptContext.TypeError, "calculateDistance(): first argument is not a number")

    if not context.argument(1).isNumber():
        context.throwError(QScriptContext.TypeError, "calculateDistance(): second argument is not a number")

    if not context.argument(2).isNumber():
        context.throwError(QScriptContext.TypeError, "calculateDistance(): third argument is not a number")

    if not context.argument(3).isNumber():
        context.throwError(QScriptContext.TypeError, "calculateDistance(): fourth argument is not a number")


    x1 = context.argument(0).toNumber()
    y1 = context.argument(1).toNumber()
    x2 = context.argument(2).toNumber()
    y2 = context.argument(3).toNumber()

    #isNaN check
    if qIsNaN(x1): return context.throwError(QScriptContext.TypeError, "calculateDistance(): first argument failed isNaN check. There is an error in your code.")
    if qIsNaN(y1): return context.throwError(QScriptContext.TypeError, "calculateDistance(): second argument failed isNaN check. There is an error in your code.")
    if qIsNaN(x2): return context.throwError(QScriptContext.TypeError, "calculateDistance(): third argument failed isNaN check. There is an error in your code.")
    if qIsNaN(y2): return context.throwError(QScriptContext.TypeError, "calculateDistance(): fourth argument failed isNaN check. There is an error in your code.")

    return QScriptValue(mainWin().nativeCalculateDistance(x1, y1, x2, y2))


def javaPerpendicularDistance(context, engine):
    if context.argumentCount() != 6:
        return context.throwError("perpendicularDistance() requires six arguments")
    if not context.argument(0).isNumber():
        context.throwError(QScriptContext.TypeError, "perpendicularDistance(): first argument is not a number")

    if not context.argument(1).isNumber():
        context.throwError(QScriptContext.TypeError, "perpendicularDistance(): second argument is not a number")

    if not context.argument(2).isNumber():
        context.throwError(QScriptContext.TypeError, "perpendicularDistance(): third argument is not a number")

    if not context.argument(3).isNumber():
        context.throwError(QScriptContext.TypeError, "perpendicularDistance(): fourth argument is not a number")

    if not context.argument(4).isNumber():
        context.throwError(QScriptContext.TypeError, "perpendicularDistance(): fifth argument is not a number")

    if not context.argument(5).isNumber():
        context.throwError(QScriptContext.TypeError, "perpendicularDistance(): sixth argument is not a number")


    px = context.argument(0).toNumber()
    py = context.argument(1).toNumber()
    x1 = context.argument(2).toNumber()
    y1 = context.argument(3).toNumber()
    x2 = context.argument(4).toNumber()
    y2 = context.argument(5).toNumber()

    #isNaN check
    if qIsNaN(px): return context.throwError(QScriptContext.TypeError, "perpendicularDistance(): first argument failed isNaN check. There is an error in your code.")
    if qIsNaN(py): return context.throwError(QScriptContext.TypeError, "perpendicularDistance(): second argument failed isNaN check. There is an error in your code.")
    if qIsNaN(x1): return context.throwError(QScriptContext.TypeError, "perpendicularDistance(): third argument failed isNaN check. There is an error in your code.")
    if qIsNaN(y1): return context.throwError(QScriptContext.TypeError, "perpendicularDistance(): fourth argument failed isNaN check. There is an error in your code.")
    if qIsNaN(x2): return context.throwError(QScriptContext.TypeError, "perpendicularDistance(): fifth argument failed isNaN check. There is an error in your code.")
    if qIsNaN(y2): return context.throwError(QScriptContext.TypeError, "perpendicularDistance(): sixth argument failed isNaN check. There is an error in your code.")

    return QScriptValue(mainWin().nativePerpendicularDistance(px, py, x1, y1, x2, y2))


def javaNumSelected(context, engine):
    if context.argumentCount() != 0:
        return context.throwError("numSelected() requires zero arguments")
    return QScriptValue(mainWin().nativeNumSelected())


def javaSelectAll(context, engine):
    if context.argumentCount() != 0:
        return context.throwError("selectAll() requires zero arguments")

    mainWin().nativeSelectAll()
    return QScriptValue()


def javaAddToSelection(context, engine):
    #TODO: finish
    return QScriptValue()


def javaClearSelection(context, engine):
    if context.argumentCount() != 0:
        return context.throwError("clearSelection() requires zero arguments")

    mainWin().nativeClearSelection()
    return QScriptValue()


def javaDeleteSelected(context, engine):
    if context.argumentCount() != 0:
        return context.throwError("deleteSelected() requires zero arguments")

    mainWin().nativeDeleteSelected()
    return QScriptValue()


def javaCutSelected(context, engine):
    if context.argumentCount() != 2:
        return context.throwError("cutSelected() requires two arguments")
    if not context.argument(0).isNumber():
        context.throwError(QScriptContext.TypeError, "cutSelected(): first argument is not a number")

    if not context.argument(1).isNumber():
        context.throwError(QScriptContext.TypeError, "cutSelected(): second argument is not a number")


    x = context.argument(0).toNumber()
    y = context.argument(1).toNumber()

    #isNaN check
    if qIsNaN(x): return context.throwError(QScriptContext.TypeError, "cutSelected(): first argument failed isNaN check. There is an error in your code.")
    if qIsNaN(y): return context.throwError(QScriptContext.TypeError, "cutSelected(): second argument failed isNaN check. There is an error in your code.")

    mainWin().nativeCutSelected(x, y)
    return QScriptValue()


def javaCopySelected(context, engine):
    if context.argumentCount() != 2:
        return context.throwError("copySelected() requires two arguments")
    if not context.argument(0).isNumber():
        context.throwError(QScriptContext.TypeError, "copySelected(): first argument is not a number")

    if not context.argument(1).isNumber():
        context.throwError(QScriptContext.TypeError, "copySelected(): second argument is not a number")


    x = context.argument(0).toNumber()
    y = context.argument(1).toNumber()

    #isNaN check
    if qIsNaN(x): return context.throwError(QScriptContext.TypeError, "copySelected(): first argument failed isNaN check. There is an error in your code.")
    if qIsNaN(y): return context.throwError(QScriptContext.TypeError, "copySelected(): second argument failed isNaN check. There is an error in your code.")

    mainWin().nativeCopySelected(x, y)
    return QScriptValue()


def javaPasteSelected(context, engine):
    if context.argumentCount() != 2:
        return context.throwError("pasteSelected() requires two arguments")
    if not context.argument(0).isNumber():
        context.throwError(QScriptContext.TypeError, "pasteSelected(): first argument is not a number")

    if not context.argument(1).isNumber():
        context.throwError(QScriptContext.TypeError, "pasteSelected(): second argument is not a number")


    x = context.argument(0).toNumber()
    y = context.argument(1).toNumber()

    #isNaN check
    if qIsNaN(x): return context.throwError(QScriptContext.TypeError, "pasteSelected(): first argument failed isNaN check. There is an error in your code.")
    if qIsNaN(y): return context.throwError(QScriptContext.TypeError, "pasteSelected(): second argument failed isNaN check. There is an error in your code.")

    mainWin().nativePasteSelected(x, y)
    return QScriptValue()


def javaMoveSelected(context, engine):
    if context.argumentCount() != 2:
        return context.throwError("moveSelected() requires two arguments")
    if not context.argument(0).isNumber():
        context.throwError(QScriptContext.TypeError, "moveSelected(): first argument is not a number")

    if not context.argument(1).isNumber():
        context.throwError(QScriptContext.TypeError, "moveSelected(): second argument is not a number")


    dx = context.argument(0).toNumber()
    dy = context.argument(1).toNumber()

    #isNaN check
    if qIsNaN(dx): return context.throwError(QScriptContext.TypeError, "moveSelected(): first argument failed isNaN check. There is an error in your code.")
    if qIsNaN(dy): return context.throwError(QScriptContext.TypeError, "moveSelected(): second argument failed isNaN check. There is an error in your code.")

    mainWin().nativeMoveSelected(dx, dy)
    return QScriptValue()


def javaScaleSelected(context, engine):
    if context.argumentCount() != 3:
        return context.throwError("scaleSelected() requires three arguments")

    if not context.argument(0).isNumber():
        context.throwError(QScriptContext.TypeError, "scaleSelected(): first argument is not a number")

    if not context.argument(1).isNumber():
        context.throwError(QScriptContext.TypeError, "scaleSelected(): second argument is not a number")

    if not context.argument(2).isNumber():
        context.throwError(QScriptContext.TypeError, "scaleSelected(): third argument is not a number")


    x = context.argument(0).toNumber()
    y = context.argument(1).toNumber()
    factor = context.argument(2).toNumber()

    #isNaN check
    if qIsNaN(x):      return context.throwError(QScriptContext.TypeError, "scaleSelected(): first argument failed isNaN check. There is an error in your code.")
    if qIsNaN(y):      return context.throwError(QScriptContext.TypeError, "scaleSelected(): second argument failed isNaN check. There is an error in your code.")
    if qIsNaN(factor): return context.throwError(QScriptContext.TypeError, "scaleSelected(): third argument failed isNaN check. There is an error in your code.")

    if factor <= 0.0:
        return context.throwError(QScriptContext.UnknownError, "scaleSelected(): scale factor must be greater than zero")

    mainWin().nativeScaleSelected(x, y, factor)
    return QScriptValue()


def javaRotateSelected(context, engine):
    if context.argumentCount() != 3:
        return context.throwError("rotateSelected() requires three arguments")

    if not context.argument(0).isNumber():
        context.throwError(QScriptContext.TypeError, "rotateSelected(): first argument is not a number")

    if not context.argument(1).isNumber():
        context.throwError(QScriptContext.TypeError, "rotateSelected(): second argument is not a number")

    if not context.argument(2).isNumber():
        context.throwError(QScriptContext.TypeError, "rotateSelected(): third argument is not a number")


    x = context.argument(0).toNumber()
    y = context.argument(1).toNumber()
    rot = context.argument(2).toNumber()

    #isNaN check
    if qIsNaN(x):   return context.throwError(QScriptContext.TypeError, "rotateSelected(): first argument failed isNaN check. There is an error in your code.")
    if qIsNaN(y):   return context.throwError(QScriptContext.TypeError, "rotateSelected(): second argument failed isNaN check. There is an error in your code.")
    if qIsNaN(rot): return context.throwError(QScriptContext.TypeError, "rotateSelected(): third argument failed isNaN check. There is an error in your code.")

    mainWin().nativeRotateSelected(x, y, rot)
    return QScriptValue()


def javaMirrorSelected(context, engine):
    if context.argumentCount() != 4:
        return context.throwError("mirrorSelected() requires four arguments")
    if not context.argument(0).isNumber():
        context.throwError(QScriptContext.TypeError, "mirrorSelected(): first argument is not a number")

    if not context.argument(1).isNumber():
        context.throwError(QScriptContext.TypeError, "mirrorSelected(): second argument is not a number")

    if not context.argument(2).isNumber():
        context.throwError(QScriptContext.TypeError, "mirrorSelected(): third argument is not a number")

    if not context.argument(3).isNumber():
        context.throwError(QScriptContext.TypeError, "mirrorSelected(): fourth argument is not a number")


    x1 = context.argument(0).toNumber()
    y1 = context.argument(1).toNumber()
    x2 = context.argument(2).toNumber()
    y2 = context.argument(3).toNumber()

    #isNaN check
    if qIsNaN(x1): return context.throwError(QScriptContext.TypeError, "mirrorSelected(): first argument failed isNaN check. There is an error in your code.")
    if qIsNaN(y1): return context.throwError(QScriptContext.TypeError, "mirrorSelected(): second argument failed isNaN check. There is an error in your code.")
    if qIsNaN(x2): return context.throwError(QScriptContext.TypeError, "mirrorSelected(): third argument failed isNaN check. There is an error in your code.")
    if qIsNaN(y2): return context.throwError(QScriptContext.TypeError, "mirrorSelected(): fourth argument failed isNaN check. There is an error in your code.")

    mainWin().nativeMirrorSelected(x1, y1, x2, y2)
    return QScriptValue()


def javaQSnapX(context, engine):
    if context.argumentCount() != 0:
        return context.throwError("qsnapX() requires zero arguments")

    return QScriptValue(mainWin().nativeQSnapX())


def javaQSnapY(context, engine):
    if context.argumentCount() != 0:
        return context.throwError("qsnapY() requires zero arguments")

    return QScriptValue(mainWin().nativeQSnapY())


def javaMouseX(context, engine):
    if context.argumentCount() != 0:
        return context.throwError("mouseX() requires zero arguments")

    return QScriptValue(mainWin().nativeMouseX())


def javaMouseY(context, engine):
    if context.argumentCount() != 0:
        return context.throwError("mouseY() requires zero arguments")

    return QScriptValue(mainWin().nativeMouseY())


# kate: bom off; indent-mode python; indent-width 4; replace-trailing-space-save on;
