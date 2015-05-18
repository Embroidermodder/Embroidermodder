#!/usr/bin/env python
# -*- coding: utf-8 -*-

"""
========================
|module_summary| main.py
========================

TOWRITE


---------------------------------------------------------

|

"""

#-Imports.----------------------------------------------------------------------
#--Python Imports.
import sys

if sys.version_info[0] == 2:
    import __builtin__ as builtins
else:
    import builtins
#--PySide/PyQt Imports.
# see further down.
#--Local Imports.
# see further down.

# C++C++C++C++C++C++C++C++C++C++C++C++C++C++C++C++C++C++C++C++C++C++C++C++C++
#include "mainwindow.h"
#include <QApplication>
# C++C++C++C++C++C++C++C++C++C++C++C++C++C++C++C++C++C++C++C++C++C++C++C++C++


_appName_ = "Embroidermodder"  # const char* 
_appVer_  = "v2.0 alpha"  # const char* 
exitApp = False  # bool
defaultBindings = "pyside"

def usage():  # static void usage(void)

    print((
    " ___ _____ ___  ___   __  _ ___  ___ ___   _____  __  ___  ___  ___ ___    ___ "           "\n"
    "| __|     | _ \\| _ \\ /  \\| |   \\| __| _ \\ |     |/  \\|   \\|   \\| __| _ \\  |__ \\" "\n"
    "| __| | | | _ <|   /| () | | |) | __|   / | | | | () | |) | |) | __|   /  / __/"           "\n"
    "|___|_|_|_|___/|_|\\_\\\\__/|_|___/|___|_|\\_\\|_|_|_|\\__/|___/|___/|___|_|\\_\\ |___|"   "\n"
    " _____________________________________________________________________________ "           "\n"
    "|                                                                             | "          "\n"
    "|                   http://embroidermodder.github.io                          | "          "\n"
    "|_____________________________________________________________________________| "          "\n"
    "                                                                               "           "\n"
    "Usage: embroidermodder [options] files ..."                                      "\n"
    #80CHARS======================================================================MAX
    "Options:"                                                                        "\n"
    "  -d, --debug      Print lots of debugging information."                         "\n"
    "  -h, --help       Print this message and exit."                                 "\n"
    "  -v, --version    Print the version number of embroidermodder and exit."        "\n"
    "  -b, --bindings {pyside,pyqt4}"                                                 "\n"
    "                   Set which qt python bindings to use."                         "\n"
    "\n"
        ))
    global exitApp
    exitApp = True


def version():  # static void version()
    """
    TOWRITE
    """
    print("%s %s\n" % (_appName_, _appVer_))

    global exitApp
    exitApp = True


def main(argv):
    """
    TOWRITE

    :param `argc`: TOWRITE
    :type `argc`: int
    :param `argv`: TOWRITE
    :type `argv`: char
    :rtype: int
    """

    filesToOpen = []  # QStringList filesToOpen;
    filesToCheck = []

    bindings = defaultBindings
    bindings_flag = False
    for arg in argv[1:]:
        if bindings_flag:
            bindings_flag = False
            if not arg in ("pyside", "pyqt4"):
                usage()
            else:
                bindings = arg
        elif arg == "-d" or arg == "--debug":
            pass
        elif arg == "-h" or arg == "--help":
            usage()
        elif arg == "-v" or arg == "--version":
            version()
        elif arg == "-b" or arg == "--bindings":
            bindings_flag = True
        else:
            filesToCheck.append(arg)

    if exitApp:
        return 1

    # make global to all modules
    if bindings == "pyside":
        builtins.PYSIDE = True
        builtins.PYQT4 = False
    elif bindings == "pyqt4":
        builtins.PYSIDE = False
        builtins.PYQT4 = True

    if PYSIDE:
        from PySide.QtScript import QScriptEngine
        if not hasattr(QScriptEngine, "newFunction"):
            raise ImportError(
                'You need a patched version of PySide.\n'
                'see: https://codereview.qt-project.org/112333')

    #--PySide Imports.
    if PYSIDE:
        from PySide.QtCore import QObject, SIGNAL, SLOT, QFile
        from PySide.QtGui import QApplication
    elif PYQT4:
        import sip
        sip.setapi('QString', 2)
        sip.setapi('QVariant', 2)
        from PyQt4.QtCore import QObject, SIGNAL, SLOT, QFile
        from PyQt4.QtGui import QApplication

    from mainwindow import MainWindow

    for file_ in filesToCheck:
        if QFile.exists(file_) and MainWindow.validFileFormat(file_):
            filesToOpen.append(file_)
        else:
            usage()
            break

    if exitApp:
        return 1

    app = QApplication(argv)
    app.setApplicationName(_appName_)
    app.setApplicationVersion(_appVer_)

    mainWin = MainWindow()  # MainWindow* 

    QObject.connect(app, SIGNAL("lastWindowClosed()"), mainWin, SLOT("quit()"))

    mainWin.setWindowTitle(app.applicationName() + " " + app.applicationVersion())
    mainWin.show()

    # NOTE: If openFilesSelected() is called from within the mainWin constructor, slot commands wont work and the window menu will be screwed
    if filesToOpen:  # if(!filesToOpen.isEmpty())
        mainWin.openFilesSelected(filesToOpen)

    # return app.exec_()
    return app.exec_()

if __name__ == '__main__':
    import sys
    argv = sys.argv
    sys.exit(main(argv))


# kate: bom off; indent-mode python; indent-width 4; replace-trailing-space-save on;
