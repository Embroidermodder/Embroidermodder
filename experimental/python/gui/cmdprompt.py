#!/usr/bin/env python
# -*- coding: utf-8 -*-

"""
=============================
|module_summary| cmdprompt.py
=============================

TOWRITE

Classes summary:
================

============================== ============================
:class:`~CmdPrompt`            TOWRITE
:class:`~CmdPromptSplitter`    TOWRITE
:class:`~CmdPromptHandle`      TOWRITE
:class:`~CmdPromptHistory`     TOWRITE
:class:`~CmdPromptInput`       TOWRITE
============================== ============================

-----------------------------------------------------------------
"""

## TODO List:
## 001. Documentation.
## 002. Remove c++ port imports when finally working.
## 003. PEP8
##
## #include <QApplication>
## #include <QString>
## #include <QAction>
## #include <QMenu>
## #include <QFile>
## #include <QFrame>
## #include <QVBoxLayout>
## #include <QContextMenuEvent>
## #include <QSplitter>
## #include <QTextStream>
## #include <QTimer>

#-Imports.---------------------------------------------------------------------

#--PySide/PyQt Imports.
if PYSIDE:
    ## from PySide import QtCore, QtGui
    # or... Improve performace with less dots...
    # Only import what we need into the global namespace
    from PySide.QtCore import (qDebug, QFile, QTextStream, QTimer, Qt, SIGNAL,
        SLOT, Signal, Slot, QIODevice, QEvent, QObject, QCoreApplication)
    from PySide.QtGui import (QAction, QApplication, QFont, QFrame, QIcon,
        QKeySequence, QLineEdit, QMenu, QSplitter, QSplitterHandle,
        QTextBrowser, QVBoxLayout, QWidget, QTextCursor, QKeyEvent,
        QTextCharFormat, QTextLayout, QColor, qApp, QInputMethodEvent)
elif PYQT4:
    import sip
    sip.setapi('QString', 2)
    sip.setapi('QVariant', 2)
#    ## from PyQt4 import QtCore, QtGui
#    # or... Improve performace with less dots...
#    # Only import what we need into the global namespace
    from PyQt4.QtCore import (qDebug, QFile, QTextStream, QTimer, Qt, SIGNAL,
        SLOT, QIODevice, QEvent, QObject, QCoreApplication)
    from PyQt4.QtCore import pyqtSignal as Signal
    from PyQt4.QtCore import pyqtSlot as Slot
    from PyQt4.QtGui import (QAction, QApplication, QFont, QFrame, QIcon,
        QKeySequence, QLineEdit, QMenu, QSplitter, QSplitterHandle,
        QTextBrowser, QVBoxLayout, QWidget, QTextCursor, QKeyEvent,
        QTextCharFormat, QTextLayout, QColor, qApp, QInputMethodEvent)


class CmdPrompt(QWidget):
    """
    Subclass of `QWidget`_

    CmdPrompt for Embroidermodder.

    .. sphinx_generate_methods_summary::
       CmdPrompt
    """

    def QWIDGETSIZE_MAX(self):
        return (1 << 24) - 1

    def __init__(self, parent=None):
        """
        Default class constructor.

        :param `parent`: Pointer to a parent widget instance.
        :type `parent`: `QWidget`_
        """
        super(CmdPrompt, self).__init__(parent)

        qDebug("CmdPrompt Constructor")
        self.setObjectName("Command Prompt")

        self.promptInput = promptInput = CmdPromptInput(self)
        self.promptHistory = promptHistory = CmdPromptHistory()
        self.promptDivider = QFrame(self)
        promptVBoxLayout = QVBoxLayout(self)

        self.promptSplitter = CmdPromptSplitter(self)

        self.setFocusProxy(promptInput)
        self.promptHistory.setFocusProxy(promptInput)

        self.promptDivider.setLineWidth(1)
        self.promptDivider.setFrameStyle(QFrame.HLine)
        self.promptDivider.setMaximumSize(self.QWIDGETSIZE_MAX(), 1)

        promptVBoxLayout.addWidget(self.promptSplitter)
        promptVBoxLayout.addWidget(self.promptHistory)
        promptVBoxLayout.addWidget(self.promptDivider)
        promptVBoxLayout.addWidget(promptInput)

        promptVBoxLayout.setSpacing(0)
        promptVBoxLayout.setContentsMargins(0, 0, 0, 0)

        self.setLayout(promptVBoxLayout)

        self.styleHash = {}  # QHash<QString, QString>()
        self.styleHash["color"] =                      "#000000" # Match -------|
        self.styleHash["background-color"] =           "#FFFFFF" #              |
        self.styleHash["selection-color"] =            "#FFFFFF" #              |
        self.styleHash["selection-background-color"] = "#000000" # Match -------|
        self.styleHash["font-family"] =              "Monospace"
        self.styleHash["font-style"] =                  "normal"
        self.styleHash["font-size"] =                     "12px"

        self.updateStyle()

        self.blinkState = False
        self.blinkTimer = QTimer(self)
        self.blinkTimer.timeout.connect(self.blink)

        self.show()

        self.connect(promptInput, SIGNAL("stopBlinking()"), self, SLOT("stopBlinking()"))
        self.connect(promptInput, SIGNAL("appendHistory(QString, int)"), promptHistory, SLOT("appendHistory(const QString&, int)"))
        self.connect(self, SIGNAL("appendTheHistory(QString, int)"), promptHistory, SLOT("appendHistory(const QString&, int)"))

        # For use outside of command prompt
        self.connect(promptInput, SIGNAL("startCommand(QString)"), self, SIGNAL("startCommand(QString)"))
        self.connect(promptInput, SIGNAL("runCommand(QString, QString)"), self, SIGNAL("runCommand(QString, QString)"))
        self.connect(promptInput, SIGNAL("deletePressed()"),    self, SIGNAL("deletePressed()"))
        self.connect(promptInput, SIGNAL("tabPressed()"),       self, SIGNAL("tabPressed()"))
        self.connect(promptInput, SIGNAL("escapePressed()"),    self, SIGNAL("escapePressed()"))
        self.connect(promptInput, SIGNAL("upPressed()"),        self, SIGNAL("upPressed()"))
        self.connect(promptInput, SIGNAL("downPressed()"),      self, SIGNAL("downPressed()"))
        self.connect(promptInput, SIGNAL("F1Pressed()"),        self, SIGNAL("F1Pressed()"))
        self.connect(promptInput, SIGNAL("F2Pressed()"),        self, SIGNAL("F2Pressed()"))
        self.connect(promptInput, SIGNAL("F3Pressed()"),        self, SIGNAL("F3Pressed()"))
        self.connect(promptInput, SIGNAL("F4Pressed()"),        self, SIGNAL("F4Pressed()"))
        self.connect(promptInput, SIGNAL("F5Pressed()"),        self, SIGNAL("F5Pressed()"))
        self.connect(promptInput, SIGNAL("F6Pressed()"),        self, SIGNAL("F6Pressed()"))
        self.connect(promptInput, SIGNAL("F7Pressed()"),        self, SIGNAL("F7Pressed()"))
        self.connect(promptInput, SIGNAL("F8Pressed()"),        self, SIGNAL("F8Pressed()"))
        self.connect(promptInput, SIGNAL("F9Pressed()"),        self, SIGNAL("F9Pressed()"))
        self.connect(promptInput, SIGNAL("F10Pressed()"),       self, SIGNAL("F10Pressed()"))
        self.connect(promptInput, SIGNAL("F11Pressed()"),       self, SIGNAL("F11Pressed()"))
        self.connect(promptInput, SIGNAL("F12Pressed()"),       self, SIGNAL("F12Pressed()"))
        self.connect(promptInput, SIGNAL("cutPressed()"),       self, SIGNAL("cutPressed()"))
        self.connect(promptInput, SIGNAL("copyPressed()"),      self, SIGNAL("copyPressed()"))
        self.connect(promptInput, SIGNAL("pastePressed()"),     self, SIGNAL("pastePressed()"))
        self.connect(promptInput, SIGNAL("selectAllPressed()"), self, SIGNAL("selectAllPressed()"))
        self.connect(promptInput, SIGNAL("undoPressed()"),      self, SIGNAL("undoPressed()"))
        self.connect(promptInput, SIGNAL("redoPressed()"),      self, SIGNAL("redoPressed()"))

        self.connect(promptInput, SIGNAL("shiftPressed()"),     self, SIGNAL("shiftPressed()"))
        self.connect(promptInput, SIGNAL("shiftReleased()"),    self, SIGNAL("shiftReleased()"))

        self.connect(promptHistory, SIGNAL("historyAppended(QString)"), self, SIGNAL("historyAppended(QString)"))

    def updateStyle(self):
        """
        TOWRITE
        """
        style = ["QTextBrowser,QLineEdit{"]

        for key, value in self.styleHash.items():
            style.append(key + ":" + value + ";")

        style.append("}")
        style = "".join(style)

        self.setStyleSheet(style)

    # Slots ------------------------------------------------------------------

    @Slot(bool)
    def floatingChanged(self, isFloating):
        """
        TOWRITE

        :param `isFloating`: TOWRITE
        :type `isFloating`: bool
        """
        qDebug("CmdPrompt floatingChanged(%d)" % isFloating)
        if isFloating:
            self.promptSplitter.hide()
        else:
            self.promptSplitter.show()

    @Slot(str, bool)
    def saveHistory(self, fileName, html):
        """
        TOWRITE

        :param `fileName`: TOWRITE
        :type `fileName`: QString
        :param `html`: TOWRITE
        :type `html`: bool
        """
        qDebug("CmdPrompt saveHistory")
        file = QFile(fileName)
        if (not file.open(QIODevice.WriteOnly | QIODevice.Text)):
            return

        # TODO: save during input in case of crash
        output = QTextStream(file)
        if html:
            output << self.promptHistory.toHtml()
        else:
            output << self.promptHistory.toPlainText()

    @Slot(str)
    def alert(self, txt):
        """
        TOWRITE

        :param `txt`: TOWRITE
        :type `txt`: QString
        """
        alertTxt = "<font color=\"red\">" + txt + "</font>"  # TODO: Make the alert color customizable
        self.setPrefix(alertTxt)
        self.appendHistory('')

    @Slot()
    def startBlinking(self):
        """
        TOWRITE
        """
        self.blinkTimer.start(750)
        self.promptInput.isBlinking = True

    @Slot()
    def stopBlinking(self):
        """
        TOWRITE
        """
        self.blinkTimer.stop()
        self.promptInput.isBlinking = False

    @Slot()
    def blink(self):
        """
        TOWRITE
        """
        self.blinkState = not self.blinkState
        if self.blinkState:
            qDebug("CmdPrompt blink1")
        else:
            qDebug("CmdPrompt blink0")

    @Slot(QColor)
    def setPromptTextColor(self, color):
        """
        TOWRITE

        :param `color`: TOWRITE
        :type `color`: `QColor`_
        """
        self.styleHash["color"] = color.name()
        self.styleHash["selection-background-color"] = color.name()
        self.updateStyle()

    @Slot(QColor)
    def setPromptBackgroundColor(self, color):
        """
        TOWRITE

        :param `color`: TOWRITE
        :type `color`: `QColor`_
        """
        self.styleHash["background-color"] = color.name()
        self.styleHash["selection-color"] = color.name()
        self.updateStyle()

    @Slot(str)
    def setPromptFontFamily(self, family):
        """
        TOWRITE

        :param `family`: TOWRITE
        :type `family`: QString
        """
        self.styleHash["font-family"] = family
        self.updateStyle()

    @Slot(str)
    def setPromptFontStyle(self, style):
        """
        TOWRITE

        :param `style`: TOWRITE
        :type `style`: QString
        """
        self.styleHash["font-style"] = style
        self.updateStyle()

    @Slot(int)
    def setPromptFontSize(self, size):
        """
        TOWRITE

        :param `size`: TOWRITE
        :type `size`: int
        """
        self.styleHash["font-size"] = "%spx" % size  # QString().setNum(size).append("px"))
        self.updateStyle()

    @Slot(str)
    def appendHistory(self, txt):
        """
        TOWRITE

        :param `txt`: TOWRITE
        :type `txt`: QString
        """
        if not txt:

            self.appendTheHistory.emit(self.promptInput.curText, len(self.promptInput.prefix))
            return

        qDebug("CmdPrompt - appendHistory()")
        self.appendTheHistory(txt, len(self.promptInput.prefix))

    @Slot(str)
    def setPrefix(self, txt):
        """
        TOWRITE

        :param `txt`: TOWRITE
        :type `txt`: QString
        """
        self.promptInput.prefix = txt
        self.promptInput.curText = txt
        self.promptInput.setText(txt)

    # cmdprompt.hhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhh

    @Slot()
    def getHistory(self):
        """
        TOWRITE

        :rtype: QString
        """
        return self.promptHistory.toHtml()

    @Slot()
    def getPrefix(self):
        """
        TOWRITE

        :rtype: QString
        """
        return self.promptInput.prefix

    @Slot()
    def getCurrentText(self):
        """
        TOWRITE

        :rtype: QString
        """
        return self.promptInput.curText

    @Slot(str)
    def setCurrentText(self, txt):
        """
        TOWRITE

        :param `txt`: TOWRITE
        :type `txt`: QString
        """
        self.promptInput.curText = self.promptInput.prefix + txt
        self.promptInput.setText(self.promptInput.curText)

    @Slot(str)
    def setHistory(self, txt):
        """
        TOWRITE

        :param `txt`: TOWRITE
        :type `txt`: QString
        """
        self.promptHistory.setHtml(txt)
        self.promptHistory.moveCursor(QTextCursor.End, QTextCursor.MoveAnchor)

    @Slot(int)
    def startResizingTheHistory(self, y):
        """
        TOWRITE

        :param `y`: TOWRITE
        :type `y`: int
        """
        self.promptHistory.startResizeHistory(y)

    @Slot(int)
    def stopResizingTheHistory(self, y):
        """
        TOWRITE

        :param `y`: TOWRITE
        :type `y`: int
        """
        self.promptHistory.stopResizeHistory(y)

    @Slot(int)
    def resizeTheHistory(self, y):
        """
        TOWRITE

        :param `y`: TOWRITE
        :type `y`: int
        """
        self.promptHistory.resizeHistory(y)

    @Slot(str, str)
    def addCommand(self, alias, cmd):
        """
        TOWRITE

        :param `alias`: TOWRITE
        :type `alias`: QString
        :param `alias`: TOWRITE
        :type `alias`: QString
        """
        self.promptInput.addCommand(alias, cmd)

    @Slot()
    def endCommand(self):
        """
        TOWRITE
        """
        self.promptInput.endCommand()

    @Slot()
    def isCommandActive(self):
        """
        TOWRITE

        :rtype: bool
        """
        return self.promptInput.cmdActive

    @Slot()
    def activeCommand(self):
        """
        TOWRITE

        :rtype: QString
        """
        return self.promptInput.curCmd

    @Slot()
    def lastCommand(self):
        """
        TOWRITE

        :rtype: QString
        """
        return self.promptInput.lastCmd

    @Slot()
    def processInput(self):
        """
        TOWRITE
        """
        self.promptInput.processInput()

    @Slot()
    def enableRapidFire(self):
        """
        TOWRITE
        """
        self.promptInput.rapidFireEnabled = True

    @Slot()
    def disableRapidFire(self):
        """
        TOWRITE
        """
        self.promptInput.rapidFireEnabled = False

    @Slot()
    def isRapidFireEnabled(self):
        """
        TOWRITE

        :rtype: bool
        """
        return self.promptInput.rapidFireEnabled

    # Signals ----------------------------------------------------------------

    appendTheHistory = Signal(str, int)

    # For connecting outside of command prompt
    startCommand = Signal(str)
    runCommand = Signal(str, str)
    deletePressed = Signal()
    tabPressed = Signal()
    escapePressed = Signal()
    upPressed = Signal()
    downPressed = Signal()
    F1Pressed = Signal()
    F2Pressed = Signal()
    F3Pressed = Signal()
    F4Pressed = Signal()
    F5Pressed = Signal()
    F6Pressed = Signal()
    F7Pressed = Signal()
    F8Pressed = Signal()
    F9Pressed = Signal()
    F10Pressed = Signal()
    F11Pressed = Signal()
    F12Pressed = Signal()
    cutPressed = Signal()
    copyPressed = Signal()
    pastePressed = Signal()
    selectAllPressed = Signal()
    undoPressed = Signal()
    redoPressed = Signal()

    shiftPressed = Signal()
    shiftReleased = Signal()

    showSettings = Signal()

    historyAppended = Signal(str)


class CmdPromptSplitter(QSplitter):
    """
    Subclass of `QSplitter`_

    CmdPromptSplitter for :class:`CmdPrompt`.

    .. sphinx_generate_methods_summary::
       CmdPromptSplitter
    """
    def __init__(self, parent=None):
        """
        Default class constructor.

        :param `parent`: Pointer to a parent widget instance.
        :type `parent`: `QWidget`_
        """
        super(CmdPromptSplitter, self).__init__(parent)

        qDebug("CmdPromptSplitter Constructor")
        self.setObjectName("Command Prompt Splitter")

        self.setOrientation(Qt.Vertical)
        # NOTE: Add two empty widgets just so we have a handle to grab
        self.addWidget(QWidget(self))
        self.addWidget(QWidget(self))

        self.connect(self, SIGNAL("pressResizeHistory(int)"),   parent, SLOT("startResizingTheHistory(int)"))
        self.connect(self, SIGNAL("releaseResizeHistory(int)"), parent, SLOT("stopResizingTheHistory(int)"))
        self.connect(self, SIGNAL("moveResizeHistory(int)"),    parent, SLOT("resizeTheHistory(int)"))

    # Signals ----------------------------------------------------------------

    pressResizeHistory = Signal(int)
    releaseResizeHistory = Signal(int)
    moveResizeHistory = Signal(int)


class CmdPromptHandle(QSplitterHandle):
    """
    Subclass of `QSplitterHandle`_

    CmdPromptHandle for :class:`CmdPrompt`.

    .. sphinx_generate_methods_summary::
       CmdPromptHandle
    """
    def __init__(self, parent=None):
        """
        Default class constructor.

        :param `parent`: Pointer to a parent widget instance.
        :type `parent`: `QWidget`_
        """
        super(CmdPromptHandle, self).__init__(parent)

        qDebug("CmdPromptHandle Constructor")
        self.setObjectName("Command Prompt Handle")

        self.connect(self, SIGNAL("handlePressed(int)"),  parent, SIGNAL("pressResizeHistory(int)"))
        self.connect(self, SIGNAL("handleReleased(int)"), parent, SIGNAL("releaseResizeHistory(int)"))
        self.connect(self, SIGNAL("handleMoved(int)"),    parent, SIGNAL("moveResizeHistory(int)"))

        # initialize instance attributes ---
        self.pressY = self.releaseY = self.moveY = int()

    def mousePressEvent(self, event):
        """
        Handles the ``mousePressEvent`` event for :class:`CmdPromptHandle`.

        :param `event`: A `QMouseEvent`_ to be processed.
        """
        self.pressY = pressY = event.globalY()
        self.handlePressed.emit(pressY)

    def mouseReleaseEvent(self, event):
        """
        Handles the ``mouseReleaseEvent`` event for :class:`CmdPromptHandle`.

        :param `event`: A `QMouseEvent`_ to be processed.
        """
        self.releaseY = releaseY = event.globalY()
        self.handleReleased.emit(releaseY)

    def mouseMoveEvent(self, event):
        """
        Handles the ``mouseMoveEvent`` event for :class:`CmdPromptHandle`.

        :param `event`: A `QMouseEvent`_ to be processed.
        """
        self.moveY = event.globalY()
        dY = self.moveY - self.pressY
        self.handleMoved.emit(dY)

    # Signals ----------------------------------------------------------------

    handlePressed = Signal(int)
    handleReleased = Signal(int)
    handleMoved = Signal(int)


class CmdPromptHistory(QTextBrowser):
    """
    Subclass of `QTextBrowser`_

    CmdPromptHistory for :class:`CmdPrompt`.

    .. sphinx_generate_methods_summary::
       CmdPromptHistory
    """
    def __init__(self, parent=None):
        """
        Default class constructor.

        :param `parent`: Pointer to a parent widget instance.
        :type `parent`: `QWidget`_
        """
        super(CmdPromptHistory, self).__init__(parent)

        qDebug("CmdPromptHistory Constructor")
        self.setObjectName("Command Prompt History")

        initHeight = 57  # 19 * 3 (approximately three lines of text)

        self.setFrameStyle(QFrame.NoFrame)
        self.setMaximumHeight(initHeight)
        self.setMinimumWidth(200)  # TODO: use float/dock events to set minimum size so when floating, it isn't smooshed.

        self.setVerticalScrollBarPolicy(Qt.ScrollBarAlwaysOn)

        # initialize instance attributes
        self.tmpHeight = int()

    def contextMenuEvent(self, event): # TODO
        """
        Handles the ``contextMenuEvent`` event for :class:`CmdPromptHistory`.

        :param `event`: A `QContextMenuEvent`_ to be processed.
        """

        menu = self.createStandardContextMenu()
        menu.addSeparator()
        # TODO: Extra stuff
        menu.addAction(QAction("TODO: Add Extra stuff", self))
        menu.exec_(event.globalPos())
        del menu

    def applyFormatting(self, txt, prefixLength):
        """
        TOWRITE

        :param `txt`: TOWRITE
        :type `txt`: QString
        :param `prefixLength`: TOWRITE
        :type `prefixLength`: int
        """
        if prefixLength < 0:
            prefixLength = 0

        prefix = txt[:prefixLength]
        usrtxt = txt[prefixLength:]

        start = -1
        stop = -1

        # Bold Prefix
        prefix = "<b>%s</b>" % prefix

        # Keywords
        start = prefix.find('[')
        stop = prefix.rfind(']')
        if start != -1 and stop != -1 and start < stop:
            for i in range(stop, start - 1, -1):
                if prefix[i] == ']':
                    prefix = prefix[:i] + "</font>" + prefix[i:]
                if prefix[i] == '[':
                    prefix = prefix[:i + 1] + "<font color=\"#0095FF\">" + prefix[i + 1:]
                if prefix[i] == '/':
                    prefix = prefix[:i + 1] + "<font color=\"#0095FF\">" + prefix[i + 1:]
                    prefix = prefix[:i] + "</font>" + prefix[i:]

        # Default Values
        start = prefix.find('{')
        stop = prefix.rfind('}')
        if start != -1 and stop != -1 and start < stop:
            for i in reversed(range(start, stop + 1)):
                if prefix[i] == '}':
                    prefix = prefix[:i] + "</font>" + prefix[i:]
                if prefix[i] == '{':
                    prefix = prefix[:i + 1] + "<font color=\"#00AA00\">" + prefix[i + 1:]

        return prefix + usrtxt

    # Slots ------------------------------------------------------------------

    @Slot(str, int)
    def appendHistory(self, txt, prefixLength): #TODO
        """
        TOWRITE

        :param `txt`: TOWRITE
        :type `txt`: QString
        :param `prefixLength`: TOWRITE
        :type `prefixLength`: int
        """
        formatStr = self.applyFormatting(txt, prefixLength)
        self.append(formatStr)
        self.historyAppended.emit(formatStr)
        self.moveCursor(QTextCursor.End, QTextCursor.MoveAnchor)

    @Slot(int)
    def startResizeHistory(self, y):  #TODO/PORT/FIXME# int /*y*/
        self.tmpHeight = self.height()

    @Slot(int)
    def stopResizeHistory(self, y):  #TODO/PORT/FIXME# int /*y*/
        self.tmpHeight = self.height()

    @Slot(int)
    def resizeHistory(self, y):
        """
        TOWRITE

        :param `y`: TOWRITE
        :type `y`: int
        """
        newHeight = self.tmpHeight - y
        if (newHeight < 0):
            newHeight = 0
        self.setMaximumHeight(newHeight)

    # Signals ----------------------------------------------------------------

    historyAppended = Signal(str)


class CmdPromptInput(QLineEdit):
    """
    Subclass of `QLineEdit`_

    CmdPromptInput for :class:`CmdPrompt`.

    .. sphinx_generate_methods_summary::
       CmdPromptInput
    """
    def __init__(self, parent=None):
        """
        Default class constructor.

        :param `parent`: Pointer to a parent widget instance.
        :type `parent`: `QWidget`_
        """
        super(CmdPromptInput, self).__init__(parent)

        qDebug("CmdPromptInput Constructor")
        self.setObjectName("Command Prompt Input")

        self.defaultPrefix = self.tr("Command: ")
        self.prefix = self.defaultPrefix
        self.curText = self.prefix

        self.lastCmd = "help"
        self.curCmd = "help"
        self.cmdActive = False

        self.rapidFireEnabled = False

        self.isBlinking = False

        self.setText(self.prefix)
        self.setFrame(False)
        self.setMaxLength(266)
        self.setMaximumSize(5000, 25)
        self.setDragEnabled(False)

        self.connect(self, SIGNAL("cursorPositionChanged(int, int)"), self, SLOT("checkCursorPosition(int, int)"))
        self.connect(self, SIGNAL("textEdited(QString)"), self, SLOT("checkEditedText(QString)"))
        self.connect(self, SIGNAL("textChanged(QString)"), self, SLOT("checkChangedText(QString)"))
        self.connect(self, SIGNAL("selectionChanged()"), self, SLOT("checkSelection()"))

        self.aliasHash = {}  # new QHash<QString, QString>

        self.installEventFilter(self)
        self.setFocus(Qt.OtherFocusReason)

        self.applyFormatting()

        # self.setCompleter(EmbroiderCommanderAutoCompleter(self))

        #### self.completer = QCompleter(['LINE', 'HEART', 'DOLPHIN', 'CIRCLE', 'STAR', 'six', 'seven', 'eight', 'nine', 'ten'], self)
        #### self.completer.setCompletionMode(QCompleter.PopupCompletion)
        #### self.completer.setCaseSensitivity(Qt.CaseInsensitive)
        #### self.setCompleter(self.completer)

        self.curText          = str()   # QString curText;
        self.defaultPrefix    = str()   # QString defaultPrefix;
        self.prefix           = str()   # QString prefix;

        self.lastCmd          = str()   # QString lastCmd;
        self.curCmd           = str()   # QString curCmd;
        self.cmdActive        = str()   # bool cmdActive;

        self.rapidFireEnabled = bool()  # bool rapidFireEnabled;
        self.isBlinking       = bool()  # bool isBlinking;

    def changeFormatting(self, formats):
        """
        TOWRITE
        """
        attributes = []  # QList<QInputMethodEvent::Attribute>

        for range_ in formats:
            type_ = QInputMethodEvent.TextFormat
            start = range_.start - self.cursorPosition()
            length = range_.length
            value = range_.format
            attributes.append(QInputMethodEvent.Attribute(type_, start, length, value))

        event = QInputMethodEvent("", attributes)
        QCoreApplication.sendEvent(self, event)

    def clearFormatting(self):
        """
        TOWRITE
        """
        self.changeFormatting([])  # changeFormatting(QList<QTextLayout::FormatRange>());

    def applyFormatting(self):
        """
        TOWRITE
        """
        prefixLength = len(self.prefix)

        start = -1
        stop = -1

        formats = []  # QList<QTextLayout.FormatRange>

        # Bold Prefix
        formatPrefix = QTextCharFormat()
        formatPrefix.setFontWeight(QFont.Bold)
        rangePrefix = QTextLayout.FormatRange()
        rangePrefix.start = 0
        rangePrefix.length = prefixLength
        rangePrefix.format = formatPrefix
        formats.append(rangePrefix)

        # Keywords
        start = self.prefix.find('[')
        stop = self.prefix.rfind(']')
        if (start != -1 & stop != -1 & start < stop):

            formatKeyword = QTextCharFormat()
            formatKeyword.setFontWeight(QFont.Bold)
            formatKeyword.setForeground(QColor("#0095FF"))

            rangeStart = -1
            rangeStop = -1
            for i in reversed(range(start, stop + 1)):
                if self.prefix[i] == ']':
                    rangeStop = i

                if self.prefix[i] == '[' or self.prefix[i] == '/':
                    rangeStart = i

                    rangeKeyword = QTextLayout.FormatRange()
                    rangeKeyword.start = rangeStart + 1
                    rangeKeyword.length = rangeStop - rangeStart - 1
                    rangeKeyword.format = formatKeyword
                    formats.append(rangeKeyword)

                    rangeStop = i

        # Default Values
        start = self.prefix.find('{')
        stop = self.prefix.rfind('}')
        if start != -1 & stop != -1 & start < stop:

            formatKeyword = QTextCharFormat()
            formatKeyword.setFontWeight(QFont.Bold)
            formatKeyword.setForeground(QColor("#00AA00"))

            rangeStart = -1
            rangeStop = -1
            for i in reversed(range(start, stop + 1)):
                if self.prefix[i] == '}':
                    rangeStop = i

                if self.prefix[i] == '{':
                    rangeStart = i

                    rangeKeyword = QTextLayout.FormatRange()
                    rangeKeyword.start = rangeStart + 1
                    rangeKeyword.length = rangeStop - rangeStart - 1
                    rangeKeyword.format = formatKeyword
                    formats.append(rangeKeyword)

                    rangeStop = i

        self.changeFormatting(formats)

    def contextMenuEvent(self, event):
        """
        Handles the ``contextMenuEvent`` event for :class:`CmdPromptInput`.

        :param `event`: A `QContextMenuEvent`_ to be processed.
        """
        menu = QMenu(self)
        menu.addSeparator()
        # TODO: Extra stuff
        menu.addAction(QAction("TODO: Add Extra stuff", self))
        menu.exec_(event.globalPos())

    def eventFilter(self, obj, event):  #TODO/PORT/FIXME
        """
        TOWRITE

        :param `obj`: TOWRITE
        :type `obj`: `QObject`_
        :param `event`: TOWRITE
        :type `event`: `QEvent`_
        """
        if event.type() == QEvent.KeyPress:
            print('event.type() == QEvent.KeyPress')

    #        if (self.isBlinking):
    #            pass # REMOVEME tmp
    #            #TODO# emit stopBlinking()

            ## QKeyEvent* pressedKey = (QKeyEvent*)event
            pressedKey = event  # QKeyEvent
            # NOTE: These shortcuts need to be caught since QLineEdit uses them
            if pressedKey.matches(QKeySequence.Cut):
                pressedKey.accept()
                self.copyPressed.emit()
                print('CUT')
                self.cut()
                return True
            elif pressedKey.matches(QKeySequence.Copy):
                pressedKey.accept()
                self.copyPressed.emit()
                return True
            elif pressedKey.matches(QKeySequence.Paste):
                pressedKey.accept()
                self.pastePressed.emit()
                self.paste()
                return True
            elif pressedKey.matches(QKeySequence.SelectAll):
                pressedKey.accept()
                self.selectAllPressed.emit()
                self.selectAll()
                return True
            elif pressedKey.matches(QKeySequence.Undo):
                pressedKey.accept()
                self.undoPressed.emit()
                self.undo()
                return True
            elif pressedKey.matches(QKeySequence.Redo):
                pressedKey.accept()
                self.redoPressed.emit()
                self.redo()
                return True

            key = pressedKey.key()
            print('key = %s' % key)
            if key in (Qt.Key_Enter, Qt.Key_Return, Qt.Key_Space):
            # switch(key)

                pressedKey.accept()
                # self.processInput(QChar(key))
                self.processInput(key)

                ## print(self.text()[len(self.defaultPrefix):])
                ## exec(u'%s' % (self.text()[len(self.defaultPrefix):]))
                return True
            elif key == Qt.Key_Backspace:
                # print('Qt.Key_Backspace')
                pressedKey.ignore()
                return True
            elif key == Qt.Key_Delete:
                print('Qt.Key_Delete')
                pressedKey.accept()
                self.deletePressed.emit()
                return True
            elif key == Qt.Key_Tab:
                pressedKey.accept()
                self.tabPressed.emit()
                return True
            elif key == Qt.Key_Escape:
                pressedKey.accept()
                self.prefix = self.defaultPrefix
                self.clear()
                self.appendHistory.emit(self.curText + self.tr("*Cancel*"), len(self.prefix))
                self.escapePressed.emit()
                return True
            elif key == Qt.Key_Up:
                pressedKey.accept()
                self.upPressed.emit()
                return True
            elif key == Qt.Key_Down:
                pressedKey.accept()
                self.downPressed.emit()
                return True
            elif key == Qt.Key_F1:
                pressedKey.accept()
                self.F1Pressed.emit()
                return True
            elif key == Qt.Key_F2:
                pressedKey.accept()
                self.F2Pressed.emit()
                return True
            elif key == Qt.Key_F3:
                pressedKey.accept()
                self.F3Pressed.emit()
                return True
            elif key == Qt.Key_F4:
                pressedKey.accept()
                self.F4Pressed.emit()
                return True
            elif key == Qt.Key_F5:
                pressedKey.accept()
                self.F5Pressed.emit()
                return True
            elif key == Qt.Key_F6:
                pressedKey.accept()
                self.F6Pressed.emit()
                return True
            elif key == Qt.Key_F7:
                pressedKey.accept()
                self.F7Pressed.emit()
                return True
            elif key == Qt.Key_F8:
                pressedKey.accept()
                self.F8Pressed.emit()
                return True
            elif key == Qt.Key_F9:
                pressedKey.accept()
                self.F9Pressed.emit()
                return True
            elif key == Qt.Key_F10:
                pressedKey.accept()
                self.F10Pressed.emit()
                return True
            elif key == Qt.Key_F11:
                pressedKey.accept()
                self.F11Pressed.emit()
                return True
            elif key == Qt.Key_F12:
                pressedKey.accept()
                self.F12Pressed.emit()
                return True
            elif key == Qt.Key_Shift:
                pressedKey.ignore()  # we don't want to eat it, we just want to keep track of it
                self.shiftPressed.emit()
            else:
                pressedKey.ignore()

        if event.type() == QEvent.KeyRelease:
            # QKeyEvent* releasedKey = (QKeyEvent*)event
            releasedKey = event  # QKeyEvent
            key = releasedKey.key()
            if key == Qt.Key_Shift:
                releasedKey.ignore()  # we don't want to eat it, we just want to keep track of it
                # emit shiftReleased()
            else:
                releasedKey.ignore()

        # return QObject.eventFilter(obj, event)
        return QObject.eventFilter(self, obj, event)

    # Slots ------------------------------------------------------------------

    @Slot(str, str)
    def addCommand(self, alias, cmd):
        """
        TOWRITE

        :param `alias`: TOWRITE
        :type `alias`: QString
        :param `cmd`: TOWRITE
        :type `cmd`: QString
        """
        self.aliasHash[alias.lower()] = cmd.lower()
        qDebug("Command Added: %s, %s" % (str(alias), str(cmd)))

    @Slot()
    def endCommand(self):
        """
        TOWRITE
        """
        qDebug("CmdPromptInput endCommand")
        self.lastCmd = self.curCmd
        self.cmdActive = False
        self.rapidFireEnabled = False
        self.stopBlinking.emit()

        self.prefix = self.defaultPrefix
        self.clear()

    @Slot(str)
    def processInput(self, rapidChar=''):
        """
        TOWRITE

        :param `rapidChar`: TOWRITE
        :type `rapidChar`: QChar
        """
        qDebug("CmdPromptInput::processInput")

        self.updateCurrentText(self.curText)

        cmdtxt = self.curText  # QString
        cmdtxt = cmdtxt[len(self.prefix):]
        if not self.rapidFireEnabled:
            cmdtxt = cmdtxt.lower()

        if self.cmdActive:
            if self.rapidFireEnabled:
                if rapidChar == Qt.Key_Enter or rapidChar == Qt.Key_Return:
                    self.appendHistory.emit(self.curText, len(self.prefix))
                    self.runCommand.emit(self.curCmd, "RAPID_ENTER")
                    self.curText = ""  # .clear()
                    self.clear()
                    return
                elif rapidChar == Qt.Key_Space:
                    self.updateCurrentText(self.curText + " ")
                    self.runCommand.emit(self.curCmd, cmdtxt + " ")
                    return
                else:
                    self.runCommand.emit(self.curCmd, cmdtxt)
                    return
            else:
                self.appendHistory.emit(self.curText, len(self.prefix))
                self.runCommand.emit(self.curCmd, cmdtxt)
        else:
            if cmdtxt in self.aliasHash:
                self.cmdActive = True
                self.lastCmd = self.curCmd
                self.curCmd = self.aliasHash[cmdtxt]
                self.appendHistory.emit(self.curText, len(self.prefix))
                self.startCommand.emit(self.curCmd)
            elif not cmdtxt:
                self.cmdActive = True
                self.appendHistory.emit(self.curText, len(self.prefix))
                # Rerun the last successful command
                self.startCommand.emit(self.lastCmd)
            else:
                self.appendHistory.emit(self.curText + "<br/><font color=\"red\">Unknown command \"" + cmdtxt + "\". Press F1 for help.</font>", len(self.prefix))

        if not self.rapidFireEnabled:
            self.clear()

    @Slot()
    def checkSelection(self):
        """
        Handles the ``selectionChanged`` SIGNAL for :class:`CmdPromptInput`.
        """
        qDebug("CmdPromptInput::checkSelection")
        if self.hasSelectedText():
            self.deselect()

    @Slot(int, int)
    def checkCursorPosition(self, oldpos, newpos):  #TODO/PORT/FIXME# int /*oldpos*/, int newpos
        """
        TOWRITE

        :param `oldpos`: TOWRITE
        :type `oldpos`: int
        :param `newpos`: TOWRITE
        :type `newpos`: int
        """
        qDebug("CmdPromptInput::checkCursorPosition - %d %d" %(oldpos, newpos))
        if self.hasSelectedText():
            self.deselect()
        if newpos < len(self.prefix):
            self.setCursorPosition(len(self.prefix))

    @Slot(str)
    def updateCurrentText(self, txt):
        """
        TOWRITE

        :param `txt`: TOWRITE
        :type `txt`: QString
        """
        # QString to python str
        # print('self.prefix.toUtf8()', u'%s' % self.prefix.toUtf8())
        # print('self.prefix.toLocal8Bit()', str(self.prefix.toLocal8Bit()))
        # prefix = str(self.prefix.toUtf8())
        # prefix = str(self.prefix.toLocal8Bit())
        ### print('prefix', prefix)
        ### if prefix == self.tr("Command: "):
        ###     print('\a')
        ###     return

        cursorPos = self.cursorPosition()
        if not txt.startswith(self.prefix):
            if len(txt) < len(self.prefix):
                self.setText(self.prefix)
            elif len(txt) != len(self.prefix):
                self.setText(self.prefix + txt)
            else:
                self.setText(self.curText)
        else:
            # input is okay so update curText.
            self.curText = txt
            self.setText(self.curText)

        self.setCursorPosition(cursorPos)

        self.applyFormatting()

    @Slot(str)
    def checkEditedText(self, txt):
        """
        Handles the ``textEdited`` SIGNAL for :class:`CmdPromptInput`.

        :param `txt`: TOWRITE
        :type `txt`: QString
        """
        self.updateCurrentText(txt)

        if self.rapidFireEnabled:
            self.processInput()

    @Slot(str)
    def checkChangedText(self, txt):
        """
        Handles the ``textChanged`` SIGNAL for :class:`CmdPromptInput`.

        :param `txt`: TOWRITE
        :type `txt`: QString
        """
        self.updateCurrentText(txt)

    @Slot()
    def copyClip(self):
        copyText = self.curText[len(self.prefix):]
        qApp.clipboard().setText(copyText)

    @Slot()
    def pasteClip(self):
        self.paste()

    # Signals ----------------------------------------------------------------

    appendHistory = Signal(str, int)

    # These connect to the CmdPrompt signals
    startCommand = Signal(str)
    runCommand = Signal(str, str)
    deletePressed = Signal()
    tabPressed = Signal()
    escapePressed = Signal()
    upPressed = Signal()
    downPressed = Signal()
    F1Pressed = Signal()
    F2Pressed = Signal()
    F3Pressed = Signal()
    F4Pressed = Signal()
    F5Pressed = Signal()
    F6Pressed = Signal()
    F7Pressed = Signal()
    F8Pressed = Signal()
    F9Pressed = Signal()
    F10Pressed = Signal()
    F11Pressed = Signal()
    F12Pressed = Signal()
    cutPressed = Signal()
    copyPressed = Signal()
    pastePressed = Signal()
    selectAllPressed = Signal()
    undoPressed = Signal()
    redoPressed = Signal()

    shiftPressed = Signal()
    shiftReleased = Signal()

    showSettings = Signal()

    stopBlinking = Signal()


# kate: bom off; indent-mode python; indent-width 4; replace-trailing-space-save on;
