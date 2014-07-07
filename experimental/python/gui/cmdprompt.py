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
## 001. Remove c++ port imports when finally working.
## 002. Documentation.
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
try:
    ## from PySide import QtCore, QtGui
    # or... Improve performace with less dots...
    # Only import what we need into the global namespace
    from PySide.QtCore import qDebug, QFile, QTextStream, QTimer, Qt, SIGNAL, SLOT, QIODevice, QEvent, QObject
    from PySide.QtGui import (QAction, QApplication, QFont, QFrame, QIcon,
        QKeySequence, QLineEdit, QMenu, QSplitter, QSplitterHandle,
        QTextBrowser, QVBoxLayout, QWidget, QTextCursor,
        QKeyEvent)
    PYSIDE = True
    PYQT4 = False
except ImportError:
    raise
#    ## from PyQt4 import QtCore, QtGui
#    # or... Improve performace with less dots...
#    # Only import what we need into the global namespace
#    from PyQt4.QtCore import qDebug, QFile, QTextStream, QTimer, Qt, SIGNAL, SLOT, QIODevice, QEvent, QObject
#    from PyQt4.QtGui import (QAction, QApplication, QFont, QFrame, QIcon,
#        QKeySequence, QLineEdit, QMenu, QSplitter, QSplitterHandle,
#        QTextBrowser, QVBoxLayout, QWidget, QTextCursor,
#        QKeyEvent)
#    PYSIDE = False
#    PYQT4 = True



class CmdPrompt(QWidget):
    """
    Subclass of `QWidget`_

    CmdPrompt for Embroidermodder.

    .. sphinx_generate_methods_summary::
       CmdPrompt
    """
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
        QWIDGETSIZE_MAX = 1 # TODO/FIXME. What is QWIDGETSIZE_MAX???
        self.promptDivider.setMaximumSize(QWIDGETSIZE_MAX, 1)

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

        # self.updateStyle()

        self.blinkState = False
        self.blinkTimer = QTimer(self)
        self.blinkTimer.timeout.connect(self.blink)

        self.show()

        #### stopBlinking = SIGNAL(str)
        #TODO# I'm guessing these are custom Signals...
        #TODO# promptInput.stopBlinking.connect(self.stopBlinking)
        #TODO# connect(promptInput, SIGNAL(appendHistory(const QString&, int)), promptHistory, SLOT(appendHistory(const QString&, int)));
        #TODO# connect(this, SIGNAL(appendTheHistory(const QString&, int)), promptHistory, SLOT(appendHistory(const QString&, int)));

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

    def floatingChanged(self, isFloating): #TODO
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

    def saveHistory(self, fileName, html): #TODO
        """
        TOWRITE

        :param `fileName`: TOWRITE
        :type `fileName`: QString
        :param `html`: TOWRITE
        :type `html`: bool
        """
        qDebug("CmdPrompt saveHistory");
        file = QFile(fileName)
        if (not file.open(QIODevice.WriteOnly | QIODevice.Text)):
            return

        # TODO: save during input in case of crash
        output = QTextStream(file)
        if html:
            output << self.promptHistory.toHtml()
        else:
            output << self.promptHistory.toPlainText()

    def alert(self, txt):
        """
        TOWRITE

        :param `txt`: TOWRITE
        :type `txt`: QString
        """
        alertTxt = "<font color=\"red\">" + txt + "</font>" # TODO: Make the alert color customizable
        self.setPrefix(alertTxt)
        #TODO# appendHistory(QString())

    def startBlinking(self):
        """
        TOWRITE
        """
        self.blinkTimer.start(750)
        self.promptInput.isBlinking = True

    def stopBlinking(self):
        """
        TOWRITE
        """
        self.blinkTimer.stop()
        self.promptInput.isBlinking = False

    def blink(self):
        """
        TOWRITE
        """
        self.blinkState = not self.blinkState
        if self.blinkState:
            qDebug("CmdPrompt blink1")
        else:
            qDebug("CmdPrompt blink0")

    def setPromptTextColor(self, color):
        """
        TOWRITE

        :param `color`: TOWRITE
        :type `color`: `QColor`_
        """
        self.styleHash["color"] = color.name()
        self.styleHash["selection-background-color"] = color.name()
        self.updateStyle()

    def setPromptBackgroundColor(self, color):
        """
        TOWRITE

        :param `color`: TOWRITE
        :type `color`: `QColor`_
        """
        self.styleHash["background-color"] = color.name()
        self.styleHash["selection-color"] = color.name()
        self.updateStyle()

    def setPromptFontFamily(self, family):
        """
        TOWRITE

        :param `family`: TOWRITE
        :type `family`: QString
        """
        self.styleHash["font-family"] = family
        self.updateStyle()

    def setPromptFontStyle(self, style):
        """
        TOWRITE

        :param `style`: TOWRITE
        :type `style`: QString
        """
        self.styleHash["font-style"] = style
        self.updateStyle()

    def setPromptFontSize(self, size):
        """
        TOWRITE

        :param `size`: TOWRITE
        :type `size`: int
        """
        self.styleHash["font-size"] = "%spx" % size  # QString().setNum(size).append("px"))
        self.updateStyle()

    def updateStyle(self): # TODO
        """
        TOWRITE
        """
        style = "QTextBrowser,QLineEdit{"

        QHashIterator<QString, QString> i(*styleHash);
        while (i.hasNext()):

            i.next()
            style.append(i.key() + ":" + i.value() + ";")

        style.append("}")

        self.setStyleSheet(style)

    def appendHistory(self, txt): # TODO
        """
        TOWRITE

        :param `txt`: TOWRITE
        :type `txt`: QString
        """
        if (txt.isNull()): #TODO#

            #TODO# emit appendTheHistory(self.promptInput.curText, self.promptInput.prefix.length())
            return

        qDebug("CmdPrompt - appendHistory()")
        #TODO# emit appendTheHistory(txt, self.promptInput.prefix.length())

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

    def getHistory(self):
        """
        TOWRITE

        :rtype: QString
        """
        return self.promptHistory.toHtml()

    def getPrefix(self):
        """
        TOWRITE

        :rtype: QString
        """
        return self.promptInput.prefix

    def getCurrentText(self):
        """
        TOWRITE

        :rtype: QString
        """
        return self.promptInput.curText

    def setCurrentText(self, txt):
        """
        TOWRITE

        :param `txt`: TOWRITE
        :type `txt`: QString
        """
        self.promptInput.curText = self.promptInput.prefix + txt
        self.promptInput.setText(self.promptInput.curText)

    def setHistory(self, txt):
        """
        TOWRITE

        :param `txt`: TOWRITE
        :type `txt`: QString
        """
        self.promptHistory.setHtml(txt)
        self.promptHistory.moveCursor(QTextCursor.End, QTextCursor.MoveAnchor)

    def startResizingTheHistory(self, y):
        """
        TOWRITE

        :param `y`: TOWRITE
        :type `y`: int
        """
        self.promptHistory.startResizeHistory(y)

    def stopResizingTheHistory(self, y):
        """
        TOWRITE

        :param `y`: TOWRITE
        :type `y`: int
        """
        self.promptHistory.stopResizeHistory(y)

    def resizeTheHistory(self, y):
        """
        TOWRITE

        :param `y`: TOWRITE
        :type `y`: int
        """
        self.promptHistory.resizeHistory(y)

    def addCommand(self, alias, cmd):
        """
        TOWRITE

        :param `alias`: TOWRITE
        :type `alias`: QString
        :param `alias`: TOWRITE
        :type `alias`: QString
        """
        self.promptInput.addCommand(alias, cmd)

    def endCommand(self):
        """
        TOWRITE
        """
        self.promptInput.endCommand()

    def isCommandActive(self):
        """
        TOWRITE

        :rtype: bool
        """
        return self.promptInput.cmdActive

    def activeCommand(self):
        """
        TOWRITE

        :rtype: QString
        """
        return self.promptInput.curCmd

    def lastCommand(self):
        """
        TOWRITE

        :rtype: QString
        """
        return self.promptInput.lastCmd

    def processInput(self):
        """
        TOWRITE
        """
        self.promptInput.processInput()

    def enableRapidFire(self):
        """
        TOWRITE
        """
        self.promptInput.rapidFireEnabled = True

    def disableRapidFire(self):
        """
        TOWRITE
        """
        self.promptInput.rapidFireEnabled = False

    def isRapidFireEnabled(self):
        """
        TOWRITE

        :rtype: bool
        """
        return self.promptInput.rapidFireEnabled


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

        #TODO# self.pressResizeHistory.connect(parent.startResizingTheHistory) # (int)
        #TODO# self.releaseResizeHistory.connect(parent.stopResizingTheHistory) # (int)
        #TODO# self.moveResizeHistory.connect(parent.resizeTheHistory) # (int)


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

        self.handlePressed.connect(parent.pressResizeHistory) # (int)
        self.handleReleased.connect(parent.releaseResizeHistory) # (int)
        self.handleMoved.connect(parent.moveResizeHistory) # (int)


    def mousePressEvent(self, event):
        """
        Handles the ``mousePressEvent`` event for :class:`CmdPromptHandle`.

        :param `event`: A `QMouseEvent`_ to be processed.
        """
        pressY = event.globalY()
        #TODO# emit handlePressed(pressY)

    def mouseReleaseEvent(self, event):
        """
        Handles the ``mouseReleaseEvent`` event for :class:`CmdPromptHandle`.

        :param `event`: A `QMouseEvent`_ to be processed.
        """
        releaseY = event.globalY()
        #TODO# emit handleReleased(releaseY)

    def mouseMoveEvent(self, event):
        """
        Handles the ``mouseMoveEvent`` event for :class:`CmdPromptHandle`.

        :param `event`: A `QMouseEvent`_ to be processed.
        """
        moveY = event.globalY()
        dY = moveY - pressY
        #TODO# emit handleMoved(dY)


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

        initHeight = 57 # 19 * 3 (approximately three lines of text)

        self.setFrameStyle(QFrame.NoFrame)
        self.setMaximumHeight(initHeight)
        self.setMinimumWidth(200) # TODO: use float/dock events to set minimum size so when floating, it isn't smooshed.

        self.setVerticalScrollBarPolicy(Qt.ScrollBarAlwaysOn)

    def applyFormatting(self, txt, prefixLength): #TODO
        """
        TOWRITE

        :param `txt`: TOWRITE
        :type `txt`: QString
        :param `prefixLength`: TOWRITE
        :type `prefixLength`: int
        """

        prefix = txt.left(prefixLength)
        usrtxt = txt.right(txt.length() - prefixLength)

        start = -1
        stop = -1

        # Bold Prefix
        prefix.prepend("<b>")
        prefix.append("</b>")

        # Keywords
        start = prefix.indexOf('[')
        stop = prefix.lastIndexOf(']')
        if (start != -1 & stop != -1 & start < stop):

            #TODO# for (int i = stop; i >= start; i--):

                if (prefix.at(i) == ']'):
                    prefix.insert(i, "</font>")
                if (prefix.at(i) == '['):
                    prefix.insert(i + 1, "<font color=\"#0095FF\">");
                if (prefix.at(i) == '/'):

                    prefix.insert(i+1, "<font color=\"#0095FF\">");
                    prefix.insert(i, "</font>")

        # Default Values
        start = prefix.indexOf('{')
        stop = prefix.lastIndexOf('}')
        if (start != -1 & stop != -1 & start < stop):

            #TODO# for (int i = stop; i >= start; i--):

                if (prefix.at(i) == '}'):
                    prefix.insert(i, "</font>")
                if (prefix.at(i) == '{'):
                    prefix.insert(i + 1, "<font color=\"#00AA00\">")

        return prefix + usrtxt

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
        #TODO# emit historyAppended(formatStr)
        #TODO# self.moveCursor(QTextCursor.End, QTextCursor.MoveAnchor)

    # def startResizeHistory(int /*y*/): #TODO
    #     tmpHeight = height()

    # def stopResizeHistory(int /*y*/): #TODO
    #     tmpHeight = height()

    def resizeHistory(self, y): #TODO what is tmpHeight? a Global/Local attr???
        """
        TOWRITE

        :param `y`: TOWRITE
        :type `y`: int
        """
        newHeight = tmpHeight - y
        if (newHeight < 0):
            newHeight = 0
        self.setMaximumHeight(newHeight)

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

        # self.cursorPositionChanged.connect(self.checkCursorPosition) # (int, int)
        # self.textEdited.connect(self.checkEditedText) # (str)
        # self.textChanged.connect(self.checkChangedText)
        # self.selectionChanged.connect(self.checkSelection)

        #TODO# self.aliasHash = new QHash<QString, QString>

        self.installEventFilter(self)
        self.setFocus(Qt.OtherFocusReason)

        #TODO# self.applyFormatting()

        # self.setCompleter(EmbroiderCommanderAutoCompleter(self))

        #### self.completer = QCompleter(['LINE', 'HEART', 'DOLPHIN', 'CIRCLE', 'STAR', 'six', 'seven', 'eight', 'nine', 'ten'], self)
        #### self.completer.setCompletionMode(QCompleter.PopupCompletion)
        #### self.completer.setCaseSensitivity(Qt.CaseInsensitive)
        #### self.setCompleter(self.completer)

    def addCommand(self, alias, cmd):
        """
        TOWRITE

        :param `alias`: TOWRITE
        :type `alias`: QString
        :param `cmd`: TOWRITE
        :type `cmd`: QString
        """
        aliasHash.insert(alias.toLower(), cmd.toLower())
        qDebug("Command Added: %s, %s" % (qPrintable(alias), qPrintable(cmd)))

    def endCommand(self):
        """
        TOWRITE
        """
        qDebug("CmdPromptInput endCommand")
        self.lastCmd = self.curCmd
        self.cmdActive = False
        self.rapidFireEnabled = False
        #TODO# emit stopBlinking()

        self.prefix = self.defaultPrefix
        self.clear()

    def processInput(self, rapidChar):
        """
        TOWRITE

        :param `rapidChar`: TOWRITE
        :type `rapidChar`: QChar
        """
        qDebug("CmdPromptInput::processInput")

        self.updateCurrentText(self.curText)

        cmdtxt = self.curText  # QString
        cmdtxt.replace(0, self.prefix.length(), "")
        if not self.rapidFireEnabled:
            cmdtxt = cmdtxt.toLower()

        if (cmdActive):

            if self.rapidFireEnabled:

                if (rapidChar == Qt.Key_Enter or rapidChar == Qt.Key_Return):

                    #TODO# emit appendHistory(curText, prefix.length())
                    #TODO# emit runCommand(curCmd, "RAPID_ENTER")
                    curText.clear()
                    clear()
                    return

                elif (rapidChar == Qt.Key_Space):

                    updateCurrentText(curText + " ")
                    #TODO# emit runCommand(curCmd, cmdtxt + " ")
                    return

                else:

                    #TODO# emit runCommand(curCmd, cmdtxt)
                    return

            else:
                pass # TODO/PORT/REMOVEME tmp
                #TODO# emit appendHistory(curText, prefix.length())
                #TODO# emit runCommand(curCmd, cmdtxt)

        else:

            if (aliasHash.contains(cmdtxt)):

                cmdActive = true
                lastCmd = curCmd
                curCmd = aliasHash.value(cmdtxt)
                #TODO# emit appendHistory(curText, prefix.length())
                #TODO# emit startCommand(curCmd)

            elif (cmdtxt.isEmpty()):

                cmdActive = True
                #TODO# emit appendHistory(curText, prefix.length())
                # Rerun the last successful command
                #TODO# emit startCommand(lastCmd)

            else:
                pass # TODO/PORT/REMOVEME tmp
                #TODO# emit appendHistory(curText + "<br/><font color=\"red\">Unknown command \"" + cmdtxt + "\". Press F1 for help.</font>", prefix.length())

        if not self.rapidFireEnabled:
            self.clear()

    def checkSelection(self):
        """
        Handles the ``selectionChanged`` SIGNAL for :class:`CmdPromptInput`.
        """
        qDebug("CmdPromptInput::checkSelection")
        if self.hasSelectedText():
            self.deselect()

    def checkCursorPosition(self, oldpos, newpos): #TODO/PORT/FIXME# int /*oldpos*/, int newpos
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
        if newpos < self.prefix.length():
            self.setCursorPosition(self.prefix.length())

    def changeFormatting(self, FIXME): #TODO/PORT/FIXME# const QList<QTextLayout::FormatRange>& formats
        """
        TOWRITE
        """
        pass
        # QList<QInputMethodEvent::Attribute> attributes;
        # foreach(const QTextLayout::FormatRange& range, formats)
        # {
        #     QInputMethodEvent::AttributeType type = QInputMethodEvent::TextFormat;
        #     int start = range.start - this->cursorPosition();
        #     int length = range.length;
        #     QVariant value = range.format;
        #     attributes.append(QInputMethodEvent::Attribute(type, start, length, value));
        # }
        # QInputMethodEvent event(QString(), attributes);
        # QCoreApplication::sendEvent(this, &event);

    def clearFormatting(self):
        """
        TOWRITE
        """
        pass #TODO/PORT/FIXME#
        # self.changeFormatting(QList<QTextLayout::FormatRange>())

    def applyFormatting(self): # TODO
        """
        TOWRITE
        """
        prefixLength = self.prefix.length()

        start = -1
        stop = -1

        #TODO# QList<QTextLayout.FormatRange> formats
        #TODO/UMMM/PORT# formats = QTextLayout.FormatRange()

        # Bold Prefix
        formatPrefix = QTextCharFormat()
        formatPrefix.setFontWeight(QFont.Bold)
        rangePrefix = QTextLayout.FormatRange()
        rangePrefix.start = 0
        rangePrefix.length = prefixLength
        rangePrefix.format = formatPrefix
        formats.append(rangePrefix)

        # Keywords
        start = self.prefix.indexOf('[')
        stop = self.prefix.lastIndexOf(']')
        if (start != -1 & stop != -1 & start < stop):

            formatKeyword = QTextCharFormat()
            formatKeyword.setFontWeight(QFont.Bold)
            formatKeyword.setForeground(QColor("#0095FF"))

            rangeStart = -1
            rangeStop = -1
            for i in reversed(range(stop, start)): #TODO/PORT/FIXME# for (int i = stop; i >= start; i--)

                if (self.prefix.at(i) == ']'):

                    rangeStop = i

                if (self.prefix.at(i) == '[' or self.prefix.at(i) == '/'):

                    rangeStart = i

                    rangeKeyword = QTextLayout.FormatRange()
                    rangeKeyword.start = rangeStart + 1
                    rangeKeyword.length = rangeStop - rangeStart - 1
                    rangeKeyword.format = formatKeyword
                    formats.append(rangeKeyword)

                    rangeStop = i

        # Default Values
        start = self.prefix.indexOf('{')
        stop = self.prefix.lastIndexOf('}')
        if (start != -1 & stop != -1 & start < stop):

            formatKeyword = QTextCharFormat()
            formatKeyword.setFontWeight(QFont.Bold)
            formatKeyword.setForeground(QColor("#00AA00"))

            rangeStart = -1
            rangeStop = -1
            for FIXME in range(FIXME): #TODO/PORT/FIXME# for (int i = stop; i >= start; i--):

                if (self.prefix.at(i) == '}'):

                    rangeStop = i

                if (self.prefix.at(i) == '{'):

                    rangeStart = i

                    rangeKeyword = QTextLayout.FormatRange()
                    rangeKeyword.start = rangeStart + 1
                    rangeKeyword.length = rangeStop - rangeStart - 1
                    rangeKeyword.format = formatKeyword
                    formats.append(rangeKeyword)

                    rangeStop = i

        self.changeFormatting(formats)

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

        if (not txt.startswith(self.prefix)):

            if (len(txt) < self.prefix.length()):
                self.setText(self.prefix)
            elif (len(txt) != self.prefix.length()):
                self.setText(self.prefix + txt)
            else:
                self.setText(self.curText)

        else:
            # input is okay so update curText.
            selfcurText = txt
            self.setText(selfcurText)

        self.applyFormatting()

    def checkEditedText(self, txt):
        """
        Handles the ``textEdited`` SIGNAL for :class:`CmdPromptInput`.

        :param `txt`: TOWRITE
        :type `txt`: QString
        """
        self.updateCurrentText(txt)

        if self.rapidFireEnabled:
            self.processInput()

    def checkChangedText(self, txt):
        """
        Handles the ``textChanged`` SIGNAL for :class:`CmdPromptInput`.

        :param `txt`: TOWRITE
        :type `txt`: QString
        """
        self.updateCurrentText(txt)

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

    def eventFilter(self, obj, event): #TODO/PORT/FIXME
        """
        TOWRITE

        :param `obj`: TOWRITE
        :type `obj`: `QObject`_
        :param `event`: TOWRITE
        :type `event`: `QEvent`_
        """
        if (event.type() == QEvent.KeyPress):
            print('event.type() == QEvent.KeyPress')

    #        if (self.isBlinking):
    #            pass # REMOVEME tmp
    #            #TODO# emit stopBlinking()

            ## QKeyEvent* pressedKey = (QKeyEvent*)event
            pressedKey = event  # QKeyEvent
            # NOTE: These shortcuts need to be caught since QLineEdit uses them
            if (pressedKey.matches(QKeySequence.Cut)):
                pressedKey.accept()
                self.copyPressed.emit()
                print('CUT')
                self.cut()
                return True
            elif (pressedKey.matches(QKeySequence.Copy)):
                pressedKey.accept()
                self.copyPressed.emit()
                return True
            elif(pressedKey.matches(QKeySequence.Paste)):
                pressedKey.accept()
                self.pastePressed.emit()
                self.paste()
                return True
            elif(pressedKey.matches(QKeySequence.SelectAll)):
                pressedKey.accept()
                self.selectAllPressed.emit()
                self.selectAll()
                return True
            elif(pressedKey.matches(QKeySequence.Undo)):
                pressedKey.accept()
                self.undoPressed.emit()
                self.undo()
                return True
            elif (pressedKey.matches(QKeySequence.Redo)):
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
                # emit appendHistory(self.curText + tr("*Cancel*"), prefix.length())
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
                pressedKey.ignore() # we don't want to eat it, we just want to keep track of it
                self.shiftPressed.emit()
            else:
                pressedKey.ignore()

        if (event.type() == QEvent.KeyRelease):

            # QKeyEvent* releasedKey = (QKeyEvent*)event
            releasedKey = event  # QKeyEvent
            key = releasedKey.key()
            if key == Qt.Key_Shift:
                releasedKey.ignore() # we don't want to eat it, we just want to keep track of it
                # emit shiftReleased()
            else:
                releasedKey.ignore()

        # return QObject.eventFilter(obj, event)
        return QObject.eventFilter(self, obj, event)


# kate: bom off; indent-mode python; indent-width 4; replace-trailing-space-save on;
