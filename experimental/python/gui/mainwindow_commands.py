#!/usr/bin/env python
# -*- coding: utf-8 -*-

"""
=======================================
|module_summary| mainwindow_commands.py
=======================================

TOWRITE

These methods are to be imported into the `MainWindow` class directly.

"""


#-Imports.---------------------------------------------------------------------
#--Python Imports.
import os
qPrintable = str
qFabs = abs

#--PySide/PyQt Imports.
if PYSIDE:
    from PySide.QtCore import (qDebug, Qt, QUrl, QProcess, QSize, QPointF,
        QLineF, Slot)
    from PySide.QtGui import (QMessageBox, QApplication, QCheckBox, QCursor,
            QLabel, QDesktopServices, QDialogButtonBox, qRgb, QFont,
            QPushButton, QMdiArea, QGraphicsScene, QComboBox, QWhatsThis,
            QWizard)
elif PYQT4:
    from PyQt4.QtCore import (qDebug, Qt, QUrl, QProcess, QSize, QPointF,
        QLineF)
    from PyQt4.QtCore import pyqtSlot as Slot
    from PyQt4.QtGui import (QMessageBox, QApplication, QCheckBox, QCursor,
            QLabel, QDesktopServices, QDialogButtonBox, qRgb, QFont,
            QPushButton, QMdiArea, QGraphicsScene, QComboBox, QWhatsThis,
            QWizard)

#--Local Imports.
from embdetails_dialog import EmbDetailsDialog
from dialog_about import AboutDialog
from tipoftheday_dialog import TipOfTheDayDialog
from layer_manager import LayerManager
from undo_commands import UndoableAddCommand, UndoableNavCommand
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


# C++C++C++C++C++C++C++C++C++C++C++C++C++C++C++C++C++C++C++C++C++C++C++C++C++
#include "mainwindow.h"
#include "view.h"
#include "statusbar.h"
#include "statusbar-button.h"
#include "imagewidget.h"
#include "layer-manager.h"
#include "object-data.h"
#include "object-arc.h"
#include "object-circle.h"
#include "object-dimleader.h"
#include "object-ellipse.h"
#include "object-image.h"
#include "object-line.h"
#include "object-path.h"
#include "object-point.h"
#include "object-polygon.h"
#include "object-polyline.h"
#include "object-rect.h"
#include "object-textsingle.h"
#include "emb-rect.h"
#include "property-editor.h"
#include "undo-editor.h"
#include "undo-commands.h"
#include "embdetails-dialog.h"

#include <QLabel>
#include <QDesktopServices>
#include <QApplication>
#include <QUrl>
#include <QProcess>
#include <QMessageBox>
#include <QDialogButtonBox>
#include <QPushButton>
#include <QMdiArea>
#include <QGraphicsScene>
#include <QComboBox>
#include <QWhatsThis>
# C++C++C++C++C++C++C++C++C++C++C++C++C++C++C++C++C++C++C++C++C++C++C++C++C++


# meth MainWindow::
# These methods are to be imported into the MainWindow class.
# Just imagine the methods are right inside it.

@Slot(str)
def stub_implement(self, txt):
    """
    TOWRITE

    :param `txt`: TOWRITE
    :type `txt`: QString
    """
    qDebug("TODO: %s" % qPrintable(txt))

@Slot()
def stub_testing(self):
    """
    TOWRITE
    """
    QMessageBox.warning(self, self.tr("Testing Feature"), self.tr("<b>This feature is in testing.</b>"))

@Slot()
def exit(self):
    """
    TOWRITE
    """
    qDebug("exit()")
    if self.getSettingsPromptSaveHistory():

        self.prompt.saveHistory("prompt.log", self.getSettingsPromptSaveHistoryAsHtml())  # TODO: get filename from settings

    QApplication.closeAllWindows()
    self.deleteLater()  # Force the MainWindow destructor to run before exiting. Makes Valgrind "still reachable" happy :)

@Slot()
def quit(self):
    """
    TOWRITE
    """
    qDebug("quit()")
    self.exit()

@Slot()
def checkForUpdates(self):
    """
    TOWRITE
    """
    qDebug("checkForUpdates()")
    #  TODO: Check website for new versions, commands, etc...

@Slot()
def cut(self):
    """
    TOWRITE
    """
    qDebug("cut()")
    gview = self.activeView()  # View*
    if gview:
        gview.cut()

@Slot()
def copy(self):
    """
    TOWRITE
    """
    qDebug("copy()")
    gview = self.activeView()  # View*
    if gview:
        gview.copy()

@Slot()
def paste(self):
    """
    TOWRITE
    """
    qDebug("paste()")
    gview = self.activeView()  # View*
    if gview:
        gview.paste()

@Slot()
def selectAll(self):
    """
    TOWRITE
    """
    qDebug("selectAll()")
    gview = self.activeView()  # View*
    if gview:
        gview.selectAll()

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
    qDebug("Platform: %s" % qPrintable(os))
    return os

@Slot()
def designDetails(self):
    """
    TOWRITE
    """
    scene = self.activeScene()  # QGraphicsScene*
    if scene:
        dialog = EmbDetailsDialog(scene, self)
        dialog.exec_()

@Slot()
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

@Slot()
def whatsThisContextHelp(self):
    """
    TOWRITE
    """
    qDebug("whatsThisContextHelp()")
    QWhatsThis.enterWhatsThisMode()

@Slot()
def print_(self):
    """
    TOWRITE
    """
    qDebug("print()")
    mdiWin = self.mdiArea.activeSubWindow()  # MdiWindow* mdiWin = qobject_cast<MdiWindow*>(mdiArea->activeSubWindow());
    if mdiWin:
        mdiWin.print_()

@Slot()
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

@Slot(int)
def checkBoxTipOfTheDayStateChanged(self, checked):
    """
    TOWRITE

    :param `checked`: TOWRITE
    :type `checked`: int
    """
    self.settings_general_tip_of_the_day = checked

@Slot(int)
def buttonTipOfTheDayClicked(self, button):
    """
    TOWRITE

    :param `button`: TOWRITE
    :type `button`: int
    """
    qDebug("buttonTipOfTheDayClicked(%d)" % button)
    if button == QWizard.CustomButton1:
        if self.settings_general_current_tip > 0:
            self.settings_general_current_tip -= 1
        else:
            self.settings_general_current_tip = len(self.listTipOfTheDay) - 1
        self.labelTipOfTheDay.setText(self.listTipOfTheDay[self.settings_general_current_tip])

    elif button == QWizard.CustomButton2:
        self.settings_general_current_tip += 1
        if self.settings_general_current_tip >= len(self.listTipOfTheDay):
            self.settings_general_current_tip = 0
        self.labelTipOfTheDay.setText(self.listTipOfTheDay[self.settings_general_current_tip])

    elif button == QWizard.CustomButton3:
        self.wizardTipOfTheDay.close()

@Slot()
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

@Slot()
def changelog(self):
    """
    TOWRITE
    """
    qDebug("changelog()")

    changelogURL = QUrl("help/changelog.html")
    QDesktopServices.openUrl(changelogURL)


# Standard Slots
@Slot()
def undo(self):
    """
    TOWRITE
    """
    qDebug("undo()")
    prefix = self.prompt.getPrefix()  # QString
    if self.dockUndoEdit.canUndo():
        self.prompt.setPrefix("Undo " + self.dockUndoEdit.undoText())
        self.prompt.appendHistory("")
        self.dockUndoEdit.undo()
        self.prompt.setPrefix(prefix)

    else:
        self.prompt.alert("Nothing to undo")
        self.prompt.setPrefix(prefix)

@Slot()
def redo(self):
    """
    TOWRITE
    """
    qDebug("redo()")
    prefix = self.prompt.getPrefix()  # QString
    if self.dockUndoEdit.canRedo():
        self.prompt.setPrefix("Redo " + self.dockUndoEdit.redoText())
        self.prompt.appendHistory("")
        self.dockUndoEdit.redo()
        self.prompt.setPrefix(prefix)

    else:
        self.prompt.alert("Nothing to redo")
        self.prompt.setPrefix(prefix)

@Slot()
def isShiftPressed(self):
    """
    Return whether or not the "Shift" key is currently pressed.

    :rtype: bool
    """
    return self.shiftKeyPressedState

@Slot()
def setShiftPressed(self):
    """
    Set the internal "Shift" key state to pressed.(True)
    """
    self.shiftKeyPressedState = True

@Slot()
def setShiftReleased(self):
    """
    Set the internal "Shift" key state to unpressed.(False)
    """
    self.shiftKeyPressedState = False


# Icons
@Slot(int)
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

@Slot()
def icon16(self):
    """
    TOWRITE
    """
    qDebug("icon16()")
    self.iconResize(16)

@Slot()
def icon24(self):
    """
    TOWRITE
    """
    qDebug("icon24()")
    self.iconResize(24)

@Slot()
def icon32(self):
    """
    TOWRITE
    """
    qDebug("icon32()")
    self.iconResize(32)

@Slot()
def icon48(self):
    """
    TOWRITE
    """
    qDebug("icon48()")
    self.iconResize(48)

@Slot()
def icon64(self):
    """
    TOWRITE
    """
    qDebug("icon64()")
    self.iconResize(64)

@Slot()
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
    qDebug("activeView()")
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

@Slot(bool)
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

@Slot(int)
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

@Slot(int)
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

@Slot(int, int, int, int, int)
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

@Slot(int)
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

@Slot(int)
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

@Slot(bool)
def updatePickAddMode(self, val):
    """
    TOWRITE

    :param `val`: TOWRITE
    :type `val`: bool
    """
    self.setSettingsSelectionModePickAdd(val)
    self.dockPropEdit.updatePickAddModeButton(val)

@Slot()
def pickAddModeToggled(self):
    """
    TOWRITE
    """
    val = not self.getSettingsSelectionModePickAdd()  # bool
    self.updatePickAddMode(val)


# Layer ToolBar
@Slot()
def makeLayerActive(self):
    """
    TOWRITE
    """
    qDebug("makeLayerActive()")
    self.stub_implement("Implement makeLayerActive.")


@Slot()
def layerManager(self):
    """
    TOWRITE
    """
    qDebug("layerManager()")
    self.stub_implement("Implement layerManager.")
    layman = LayerManager(self, self)
    layman.exec_()

@Slot()
def layerPrevious(self):
    """
    TOWRITE
    """
    qDebug("layerPrevious()")
    self.stub_implement("Implement layerPrevious.")


# Zoom ToolBar
@Slot()
def zoomRealtime(self):
    """
    TOWRITE
    """
    qDebug("zoomRealtime()")
    self.stub_implement("Implement zoomRealtime.")

@Slot()
def zoomPrevious(self):
    """
    TOWRITE
    """
    qDebug("zoomPrevious()")
    self.stub_implement("Implement zoomPrevious.")

@Slot()
def zoomWindow(self):
    """
    TOWRITE
    """
    qDebug("zoomWindow()")
    gview = self.activeView()  # View*
    if gview:
        gview.zoomWindow()

@Slot()
def zoomDynamic(self):
    """
    TOWRITE
    """
    qDebug("zoomDynamic()")
    self.stub_implement("Implement zoomDynamic.")

@Slot()
def zoomScale(self):
    """
    TOWRITE
    """
    qDebug("zoomScale()")
    self.stub_implement("Implement zoomScale.")

@Slot()
def zoomCenter(self):
    """
    TOWRITE
    """
    qDebug("zoomCenter()")
    self.stub_implement("Implement zoomCenter.")

@Slot()
def zoomIn(self):
    """
    TOWRITE
    """
    qDebug("zoomIn()")
    gview = self.activeView()  # View*
    if gview:
        gview.zoomIn()

@Slot()
def zoomOut(self):
    """
    TOWRITE
    """
    qDebug("zoomOut()")
    gview = self.activeView()  # View*
    if gview:
        gview.zoomOut()

@Slot()
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

@Slot()
def zoomAll(self):
    """
    TOWRITE
    """
    qDebug("zoomAll()")
    self.stub_implement("Implement zoomAll.")

@Slot()
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
@Slot()
def panrealtime(self):
    """
    TOWRITE
    """
    qDebug("panrealtime()")
    gview = self.activeView()  # View*
    if gview:
        gview.panRealTime()

@Slot()
def panpoint(self):
    """
    TOWRITE
    """
    qDebug("panpoint()")
    gview = self.activeView()  # View*
    if gview:
        gview.panPoint()

@Slot()
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

@Slot()
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

@Slot()
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

@Slot()
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

@Slot()
def dayVision(self):
    """
    TOWRITE
    """
    gview = self.activeView()  # View*
    if gview:
        gview.setBackgroundColor(qRgb(255, 255, 255))  # TODO: Make day vision color settings.
        gview.setCrossHairColor(qRgb(0, 0, 0))         # TODO: Make day vision color settings.
        gview.setGridColor(qRgb(0, 0, 0))              # TODO: Make day vision color settings.

@Slot()
def nightVision(self):
    """
    TOWRITE
    """
    gview = self.activeView()  # View*
    if gview:
        gview.setBackgroundColor(qRgb(0, 0, 0))       # TODO: Make night vision color settings.
        gview.setCrossHairColor(qRgb(255, 255, 255))  # TODO: Make night vision color settings.
        gview.setGridColor(qRgb(255, 255, 255))       # TODO: Make night vision color settings.

@Slot()
def doNothing(self):
    """
    This function intentionally does nothing.
    """
    qDebug("doNothing()")

@Slot(int)
def layerSelectorIndexChanged(self, index):
    """
    TOWRITE

    :param `index`: TOWRITE
    :type `index`: int
    """
    qDebug("layerSelectorIndexChanged(%d)" % index)

@Slot(int)
def colorSelectorIndexChanged(self, index):
    """
    TOWRITE

    :param `index`: TOWRITE
    :type `index`: int
    """
    qDebug("colorSelectorIndexChanged(%d)" % index)

    comboBox = self.sender()  # QComboBox* comboBox = qobject_cast<QComboBox*>(sender());
    newColor = int()  # QRgb
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

@Slot(int)
def linetypeSelectorIndexChanged(self, index):
    """
    TOWRITE

    :param `index`: TOWRITE
    :type `index`: int
    """
    qDebug("linetypeSelectorIndexChanged(%d)" % index)

@Slot(int)
def lineweightSelectorIndexChanged(self, index):
    """
    TOWRITE

    :param `index`: TOWRITE
    :type `index`: int
    """
    qDebug("lineweightSelectorIndexChanged(%d)" % index)

@Slot(QFont)
def textFontSelectorCurrentFontChanged(self, font):
    """
    TOWRITE

    :param `font`: TOWRITE
    :type `font`: `QFont`_
    """
    qDebug("textFontSelectorCurrentFontChanged()")
    self.setTextFont(font.family())

@Slot(int)
def textSizeSelectorIndexChanged(self, index):
    """
    TOWRITE

    :param `index`: TOWRITE
    :type `index`: int
    """
    qDebug("textSizeSelectorIndexChanged(%d)" % index)
    self.setSettingsTextSize(qFabs(float(self.textSizeSelector.itemData(index))))  # TODO: check that the toReal() conversion is ok

@Slot()
def textFont(self):
    """
    TOWRITE

    :rtype: QString
    """
    return self.getSettingsTextFont()

@Slot()
def textSize(self):
    """
    TOWRITE

    :rtype: qreal
    """
    return self.getSettingsTextSize()

@Slot()
def textAngle(self):
    """
    TOWRITE

    :rtype: qreal
    """
    return self.getSettingsTextAngle()

@Slot()
def textBold(self):
    """
    TOWRITE

    :rtype: bool
    """
    return self.getSettingsTextStyleBold()

@Slot()
def textItalic(self):
    """
    TOWRITE

    :rtype: bool
    """
    return self.getSettingsTextStyleItalic()

@Slot()
def textUnderline(self):
    """
    TOWRITE

    :rtype: bool
    """
    return self.getSettingsTextStyleUnderline()

@Slot()
def textStrikeOut(self):
    """
    TOWRITE

    :rtype: bool
    """
    return self.getSettingsTextStyleStrikeOut()

@Slot()
def textOverline(self):
    """
    TOWRITE

    :rtype: bool
    """
    return self.getSettingsTextStyleOverline()

@Slot(str)
def setTextFont(self, strng):
    """
    TOWRITE

    :param `strng`: TOWRITE
    :type `strng`: QString
    """
    self.textFontSelector.setCurrentFont(QFont(strng))
    self.setSettingsTextFont(strng)

@Slot(float)
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
    self.textSizeSelector.addItem("Custom %.2f pt" % num, num)
    index = self.textSizeSelector.findText("Custom", Qt.MatchContains)
    if index != -1:
        self.textSizeSelector.setCurrentIndex(index)

@Slot(float)
def setTextAngle(self, num):
    """
    TOWRITE

    :param `num`: TOWRITE
    :type `num`: qreal
    """
    self.setSettingsTextAngle(num)

@Slot(bool)
def setTextBold(self, val):
    """
    TOWRITE

    :param `val`: TOWRITE
    :type `val`: bool
    """
    self.setSettingsTextStyleBold(val)

@Slot(bool)
def setTextItalic(self, val):
    """
    TOWRITE

    :param `val`: TOWRITE
    :type `val`: bool
    """
    self.setSettingsTextStyleItalic(val)

@Slot(bool)
def setTextUnderline(self, val):
    """
    TOWRITE

    :param `val`: TOWRITE
    :type `val`: bool
    """
    self.setSettingsTextStyleUnderline(val)

@Slot(bool)
def setTextStrikeOut(self, val):
    """
    TOWRITE

    :param `val`: TOWRITE
    :type `val`: bool
    """
    self.setSettingsTextStyleStrikeOut(val)

@Slot(bool)
def setTextOverline(self, val):
    """
    TOWRITE

    :param `val`: TOWRITE
    :type `val`: bool
    """
    self.setSettingsTextStyleOverline(val)

@Slot()
def getCurrentLayer(self):
    """
    TOWRITE

    :rtype: QString
    """
    mdiWin = self.mdiArea.activeSubWindow()  # MdiWindow* mdiWin = qobject_cast<MdiWindow*>(mdiArea->activeSubWindow());
    if mdiWin:
        return mdiWin.getCurrentLayer()
    return "0"

@Slot()
def getCurrentColor(self):
    """
    TOWRITE

    :rtype: QRgb
    """
    mdiWin = self.mdiArea.activeSubWindow()  # MdiWindow* mdiWin = qobject_cast<MdiWindow*>(mdiArea->activeSubWindow());
    if mdiWin:
        return mdiWin.getCurrentColor()
    return 0  # TODO: return color ByLayer

@Slot()
def getCurrentLineType(self):
    """
    TOWRITE

    :rtype: QString
    """
    mdiWin = self.mdiArea.activeSubWindow()  # MdiWindow* mdiWin = qobject_cast<MdiWindow*>(mdiArea->activeSubWindow());
    if mdiWin:
        return mdiWin.getCurrentLineType()
    return "ByLayer"

@Slot()
def getCurrentLineWeight(self):
    """
    TOWRITE

    :rtype: QString
    """
    mdiWin = self.mdiArea.activeSubWindow()  # MdiWindow* mdiWin = qobject_cast<MdiWindow*>(mdiArea->activeSubWindow());
    if mdiWin:
        return mdiWin.getCurrentLineWeight()
    return "ByLayer"

@Slot()
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

@Slot()
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

@Slot()
def toggleGrid(self):
    """
    TOWRITE
    """
    qDebug("toggleGrid()")
    self.statusbar.statusBarGridButton.toggle()

@Slot()
def toggleRuler(self):
    """
    TOWRITE
    """
    qDebug("toggleRuler()")
    self.statusbar.statusBarRulerButton.toggle()

@Slot()
def toggleLwt(self):
    """
    TOWRITE
    """
    qDebug("toggleLwt()")
    self.statusbar.statusBarLwtButton.toggle()

@Slot()
def enablePromptRapidFire(self):
    """
    TOWRITE
    """
    self.prompt.enableRapidFire()

@Slot()
def disablePromptRapidFire(self):
    """
    TOWRITE
    """
    self.prompt.disableRapidFire()

@Slot()
def enableMoveRapidFire(self):
    """
    TOWRITE
    """
    gview = self.activeView()  # View*
    if gview:
        gview.enableMoveRapidFire()

@Slot()
def disableMoveRapidFire(self):
    """
    TOWRITE
    """
    gview = self.activeView()  # View*
    if gview:
        gview.disableMoveRapidFire()

@Slot(str)
def promptHistoryAppended(self, txt):
    """
    TOWRITE

    :param `txt`: TOWRITE
    :type `txt`: QString
    """
    mdiWin = self.activeMdiWindow()  #  MdiWindow*
    if mdiWin:
        mdiWin.promptHistoryAppended(txt)

@Slot(str)
def logPromptInput(self, txt):
    """
    TOWRITE

    :param `txt`: TOWRITE
    :type `txt`: QString
    """
    mdiWin = self.activeMdiWindow()  # MdiWindow*
    if mdiWin:
        mdiWin.logPromptInput(txt)

@Slot()
def promptInputPrevious(self):
    """
    TOWRITE
    """
    mdiWin = self.activeMdiWindow()  # MdiWindow*
    if mdiWin:
        mdiWin.promptInputPrevious()

@Slot()
def promptInputNext(self):
    """
    TOWRITE
    """
    mdiWin = self.activeMdiWindow()  # MdiWindow*
    if mdiWin:
        mdiWin.promptInputNext()

@Slot()
def runCommand(self):
    """
    TOWRITE
    """
    act = self.sender()  # QAction* act = qobject_cast<QAction*>(sender());
    if act:
        qDebug("runCommand(%s)" % qPrintable(act.objectName()))
        self.prompt.endCommand()
        self.prompt.setCurrentText(act.objectName())
        self.prompt.processInput()

@Slot(str)
def runCommandMain(self, cmd):
    """
    TOWRITE

    :param `cmd`: TOWRITE
    :type `cmd`: QString
    """
    qDebug("runCommandMain(%s)" % qPrintable(cmd))
    fileName = "commands/" + cmd + "/" + cmd + ".js"  # QString
    # if not self.getSettingsSelectionModePickFirst(): self.nativeClearSelection()  # TODO: Uncomment this line when post-selection is available
    self.engine.evaluate(cmd + "_main()", fileName)

@Slot(str, float, float)
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
    qDebug("runCommandClick(%s, %.2f, %.2f)" % (qPrintable(cmd), x, y))
    fileName = "commands/" + cmd + "/" + cmd + ".js"  # QString
    self.engine.evaluate("%s_click(%d,%d)" % (cmd, x, -y), fileName)

@Slot(str, float, float)
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
    qDebug("runCommandMove(%s, %.2f, %.2f)" % (qPrintable(cmd), x, y))
    fileName = "commands/" + cmd + "/" + cmd + ".js"  # QString
    self.engine.evaluate("%s_move(%d,%d)" % (cmd, x, -y), fileName)

@Slot(str, str)
def runCommandContext(self, cmd, strng):
    """
    TOWRITE

    :param `cmd`: TOWRITE
    :type `cmd`: QString
    :param `strng`: TOWRITE
    :type `strng`: QString
    """
    qDebug("runCommandContext(%s, %s)" % (qPrintable(cmd), qPrintable(strng)))
    fileName = "commands/" + cmd + "/" + cmd + ".js"  # QString
    self.engine.evaluate("%s_context('%s')" % (cmd, strng.upper()), fileName)

@Slot(str ,str)
def runCommandPrompt(self, cmd, strng):
    """
    TOWRITE

    :param `cmd`: TOWRITE
    :type `cmd`: QString
    :param `strng`: TOWRITE
    :type `strng`: QString
    """
    qDebug("runCommandPrompt(%s, %s)" % (qPrintable(cmd), qPrintable(strng)))
    fileName = "commands/" + cmd + "/" + cmd + ".js"  # QString
    # NOTE: Replace any special characters that will cause a syntax error
    safeStr = strng  # QString
    safeStr.replace("\\", "\\\\")
    safeStr.replace("\'", "\\\'")
    if self.prompt.isRapidFireEnabled():
        self.engine.evaluate(cmd + "_prompt('" + safeStr + "')", fileName)
    else:
        self.engine.evaluate(cmd + "_prompt('" + safeStr.upper() + "')", fileName)

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
    self.icon128()

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
    qDebug("nativePrintArea(%.2f, %.2f, %.2f, %.2f)" % (x, y, w, h))
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

        obj = TextSingleObject(strng, x, -y, self.getCurrentColor())  # TextSingleObject*
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
            cmd = UndoableAddCommand(obj.data(OBJ_NAME), obj, gview, None)
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
            cmd = UndoableAddCommand(obj.data(OBJ_NAME), obj, gview, None)
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
            cmd = UndoableAddCommand(obj.data(OBJ_NAME), obj, gview, None)
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
            cmd = UndoableAddCommand(obj.data(OBJ_NAME), obj, gview, None)
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
            cmd = UndoableAddCommand(obj.data(OBJ_NAME), obj, gview, None)
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
        cmd = UndoableAddCommand(obj.data(OBJ_NAME), obj, gview, None)
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
            cmd = UndoableAddCommand(obj.data(OBJ_NAME), obj, gview, None)
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
            cmd = UndoableAddCommand(obj.data(OBJ_NAME), obj, gview, None)
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
            cmd = UndoableAddCommand(obj.data(OBJ_NAME), obj, gview, None)
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
    # iPoint = QPointF()
    _, iPoint = norm.intersect(line)
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
        return scene.property("SCENE_QSNAP_POINT").x()  # .toPointF().x()
    return 0.0

def nativeQSnapY(self):
    """
    TOWRITE

    :rtype: qreal
    """
    scene = self.activeScene()  # QGraphicsScene*
    if scene:
        return -scene.property("SCENE_QSNAP_POINT").y()  # .toPointF().y()
    return 0.0

def nativeMouseX(self):
    """
    TOWRITE

    :rtype: qreal
    """
    scene = self.activeScene()  # QGraphicsScene*
    if scene:
        qDebug("mouseX: %.50f" % scene.property("SCENE_MOUSE_POINT").x())  # .toPointF().x())
    if scene:
        return scene.property("SCENE_MOUSE_POINT").x()  # .toPointF().x()
    return 0.0

def nativeMouseY(self):
    """
    TOWRITE

    :rtype: qreal
    """
    scene = self.activeScene()  # QGraphicsScene*
    if scene:
        qDebug("mouseY: %.50f" % -scene.property("SCENE_MOUSE_POINT").y())  # .toPointF().y())
    if scene:
        return -scene.property("SCENE_MOUSE_POINT").y()  # .toPointF().y()
    return 0.0

