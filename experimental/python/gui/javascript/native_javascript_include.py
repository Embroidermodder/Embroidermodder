#!/usr/bin/env python
# -*- coding: utf-8 -*-

"""
=============================================
|module_summary| native_javascript_include.py
=============================================

TOWRITE

"""

## TODO List:
## 001. Documentation.
## 002. Remove c++ port imports when finally working.
## 003. PEP8

#-Imports.---------------------------------------------------------------------
#--PySide/PyQt Imports.
if PYSIDE:
    ## from PySide import QtCore, QtGui
    # or... Improve performace with less dots...
    from PySide.QtCore import QFile, QTextStream, QIODevice
elif PYQT4:
    import sip
    sip.setapi('QString', 2)
    sip.setapi('QVariant', 2)
    ## from PyQt4 import QtCore, QtGui
    # or... Improve performace with less dots...
    from PyQt4.QtCore import QFile, QTextStream, QIODevice


# C++C++C++C++C++C++C++C++C++C++C++C++C++C++C++C++C++C++C++C++C++C++C++C++C++
#include "native-javascript.h"
# C++C++C++C++C++C++C++C++C++C++C++C++C++C++C++C++C++C++C++C++C++C++C++C++C++


#NOTE: http://www.qtcentre.org/threads/20432-Can-I-include-a-script-from-script
def javaInclude(context, engine):
    fileName = context.argument(0).toString()
    scriptFile = QFile("commands/" + fileName)

    if not scriptFile.open(QIODevice.ReadOnly):
        return -1

    stream = QTextStream(scriptFile)
    s = stream.readAll()  # QString
    scriptFile.close()

    parent = context.parentContext()  # QScriptContext*

    if parent != 0:
        context.setActivationObject(context.parentContext().activationObject())
        context.setThisObject(context.parentContext().thisObject())

    result = engine.evaluate(s)  #TODO/PORT/FIXME# what's this for?

    return 0


# kate: bom off; indent-mode python; indent-width 4; replace-trailing-space-save on;
