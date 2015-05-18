#!/usr/bin/env python
# -*- coding: utf-8 -*-

"""
====================================
|module_summary| native_scripting.py
====================================

TOWRITE

"""

## TODO List:
## 001. Check if this works/needed in python.
## 002. Documentation.


_mainWin = 0

# NOTE: qthelp://com.trolltech.qt.470/qdoc/scripting.html
# NOTE: Wrapping a Native Function
# NOTE: It is currently not possible to wrap member functions; i.e., methods of a C++ class that require a this object.
def initMainWinPointer(mw):
    global _mainWin
    _mainWin = mw


def mainWin():
    return _mainWin
