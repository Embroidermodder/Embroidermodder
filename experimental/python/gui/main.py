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
#--PySide/PyQt Imports.
try:
    from PySide.QtCore import QObject, SIGNAL, SLOT
    from PySide.QtGui import QApplication
except ImportError:
    raise
#    from PyQt4.QtCore import QObject, SIGNAL, SLOT
#    from PyQt4.QtGui import QApplication

#--Local Imports.
from mainwindow import MainWindow

# C++C++C++C++C++C++C++C++C++C++C++C++C++C++C++C++C++C++C++C++C++C++C++C++C++
#include "mainwindow.h"
#include <QApplication>
# C++C++C++C++C++C++C++C++C++C++C++C++C++C++C++C++C++C++C++C++C++C++C++C++C++


_appName_ = "Embroidermodder"  # const char* 
_appVer_  = "v2.0 alpha"  # const char* 
exitApp = False  # bool

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
    "\n"
        ))
    exitApp = True


def version():  # static void version()
    """
    TOWRITE
    """
    fprintf(stdout, "%s %s\n", _appName_, _appVer_)
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

    app = QApplication(argv)
    app.setApplicationName(_appName_)
    app.setApplicationVersion(_appVer_)

    filesToOpen = []  # QStringList filesToOpen;

    #TODO/PORT# for(int i = 1; i < argc; i++)
    #TODO/PORT# {
    #TODO/PORT#     if     (!strcmp(argv[i], "-d") || !strcmp(argv[i], "--debug")  ) {  }
    #TODO/PORT#     else if(!strcmp(argv[i], "-h") || !strcmp(argv[i], "--help")   ) { usage(); }
    #TODO/PORT#     else if(!strcmp(argv[i], "-v") || !strcmp(argv[i], "--version")) { version(); }
    #TODO/PORT#     else if(QFile::exists(argv[i]) && MainWindow::validFileFormat(argv[i]))
    #TODO/PORT#         filesToOpen << argv[i];
    #TODO/PORT# 
    #TODO/PORT#     else:
    #TODO/PORT#         usage()
    #TODO/PORT# 
    #TODO/PORT# if exitApp:
    #TODO/PORT#     return 1

    mainWin = MainWindow()  # MainWindow* 

    QObject.connect(app, SIGNAL("lastWindowClosed()"), mainWin, SLOT("quit()"))

    mainWin.setWindowTitle(app.applicationName() + " " + app.applicationVersion())
    mainWin.show()

    # NOTE: If openFilesSelected() is called from within the mainWin constructor, slot commands wont work and the window menu will be screwed
    if filesToOpen:  # if(!filesToOpen.isEmpty())
        mainWin.openFilesSelected(filesToOpen)

    # return app.exec_()
    return sys.exit(app.exec_())

if __name__ == '__main__':
    import sys
    argv = sys.argv
    main(argv)


# kate: bom off; indent-mode python; indent-width 4; replace-trailing-space-save on;
