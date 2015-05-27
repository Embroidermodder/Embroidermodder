#!/usr/bin/env python
# -*- coding: utf-8 -*-

"""
=============================
|module_summary| mdiwindow.py
=============================

TOWRITE

Classes summary:
================

================================ ============================
:class:`~MdiWindow`              TOWRITE
================================ ============================

-------------------------------------------------------------
"""

#-Imports.----------------------------------------------------------------------

#--Python Imports.
import os
qPrintable = str

#--PySide/PyQt Imports.
if PYSIDE:
    from PySide.QtCore import qDebug, Qt, QFile, QSize, QRectF, \
        QFileInfo, Slot, Signal
    from PySide.QtGui import (QMdiSubWindow, QGraphicsScene, QGraphicsItem,
        QApplication, QBrush, QColor, QDialog, QPainterPath, QPen, QImage,
        QIcon, QMessageBox, QPrinter, QPrintDialog, QPainter, qRgb)
elif PYQT4:
    import sip
    sip.setapi('QString', 2)
    sip.setapi('QVariant', 2)
    from PyQt4.QtCore import qDebug, Qt, QFile, QSize, QRectF, QFileInfo
    from PyQt4.QtCore import pyqtSlot as Slot
    from PyQt4.QtCore import pyqtSignal as Signal
    from PyQt4.QtGui import (QMdiSubWindow, QGraphicsScene, QGraphicsItem,
        QApplication, QBrush, QColor, QDialog, QPainterPath, QPen, QImage,
        QIcon, QMessageBox, QPrinter, QPrintDialog, QPainter, qRgb)


#--Local Imports.
from view import View
from object_path import PathObject
from object_save import SaveObject
from object_data import OBJ_RUBBER_OFF, ENABLE_SNAP, ENABLE_GRID, ENABLE_RULER, \
    ENABLE_ORTHO, ENABLE_POLAR, ENABLE_QSNAP, ENABLE_QTRACK, ENABLE_LWT

#--libembroidery Imports.
from libembroidery import (embPattern_create, embReaderWriter_getByFileName,
    embStitchList_count, embCircle_centerX, embCircle_centerY,
    embCircle_radius, embEllipse_centerY, embRect_width, embEllipse_width,
    embEllipse_height, embLine_x1, embLine_y1, embLine_x2, embLine_y2,
    embPoint_x, embPoint_y, embRect_x, embRect_y, embRect_height, 
    embEllipse_centerX, embPattern_moveStitchListToPolylines)

# C++C++C++C++C++C++C++C++C++C++C++C++C++C++C++C++C++C++C++C++C++C++C++C++C++
#include <QMdiSubWindow>
#include <QStatusBar>
#include <QColor>
#include <QUndoStack>

#include <QGraphicsScene>
#include <QGraphicsView>
#include <QGraphicsItem>

#include "emb-reader-writer.h"
# C++C++C++C++C++C++C++C++C++C++C++C++C++C++C++C++C++C++C++C++C++C++C++C++C++


class MdiWindow(QMdiSubWindow):
    """
    Subclass of `QMdiSubWindow`_

    TOWRITE
    """
    def __init__(self, theIndex, mw, parent=None, wflags=Qt.WindowFlags):
        """
        Default class constructor.

        :param `theIndex`: TOWRITE
        :type `theIndex`: int
        :param `mw`: Pointer to the applications main window instance.
        :type `mw`: `MainWindow`_
        :param `parent`: Pointer to a parent widget instance.
        :type `parent`: `QMdiArea`_
        :param `wflags`: TOWRITE
        :type `wflags`: Qt.WindowFlags
        """
        super(MdiWindow, self).__init__(parent, wflags)

        self.printer = QPrinter()


        self.mainWin = mainWin = mw
        self.mdiArea = parent


        self.myIndex = myIndex = theIndex

        self.fileWasLoaded = False

        self.setAttribute(Qt.WA_DeleteOnClose)

        # QString aName;
        self.curFile = curFile = "Untitled%d.dst" % myIndex # curFile = aName.sprintf("Untitled%d.dst", myIndex);
        self.setWindowTitle(curFile)

        self.setWindowIcon(QIcon(mainWin.gIconDir + os.sep + mainWin.getSettingsGeneralIconTheme() + "/" + "app" + ".png"))

        self.gscene = gscene = QGraphicsScene(0, 0, 0, 0, self)
        self.gview = gview = View(mainWin, gscene, self)

        self.setWidget(gview)

        # WARNING: DO NOT SET THE QMDISUBWINDOW (this) FOCUSPROXY TO THE PROMPT
        # WARNING: AS IT WILL CAUSE THE WINDOW MENU TO NOT SWITCH WINDOWS PROPERLY!
        # WARNING: ALTHOUGH IT SEEMS THAT SETTING INTERNAL WIDGETS FOCUSPROXY IS OK.
        gview.setFocusProxy(mainWin.prompt)

        self.resize(self.sizeHint())

        self.promptHistory = promptHistory = "Welcome to Embroidermodder 2!<br/>Open some of our sample files. Many formats are supported.<br/>For help, press F1."
        mainWin.prompt.setHistory(promptHistory)
        self.promptInputList = [""]
        self.promptInputNum = 0

        self.curLayer = "0"
        self.curColor = 0  # TODO: color ByLayer
        self.curLineType = "ByLayer"
        self.curLineWeight = "ByLayer"

        # Due to strange Qt4.2.3 feature the child window icon is not drawn
        # in the main menu if showMaximized() is called for a non-visible child window
        # Therefore calling show() first...
        self.show()
        self.showMaximized()

        self.setFocusPolicy(Qt.WheelFocus)
        self.setFocus()

        self.onWindowActivated()


    def __del__(self):
        """Class destructor."""
        qDebug("MdiWindow Destructor()")

    def saveFile(self, fileName):
        """
        TOWRITE

        :param `fileName`: TOWRITE
        :type `fileName`: QString
        :rtype: bool
        """
        saveObj = SaveObject(self.gscene, self)
        return saveObj.save(fileName)

    def loadFile(self, fileName):
        """
        TOWRITE

        :param `fileName`: TOWRITE
        :type `fileName`: QString
        :rtype: bool
        """
        qDebug("MdiWindow loadFile()")

        tmpColor = self.getCurrentColor()  # QRgb

        file = QFile(fileName)
        if not file.open(QFile.ReadOnly | QFile.Text):
            QMessageBox.warning(
                self,
                self.tr("Error reading file"),
                self.tr("Cannot read file %s:\n%s." % (fileName, file.errorString()))
                )
            return False

        QApplication.setOverrideCursor(Qt.WaitCursor)

        ext = self.fileExtension(fileName)  # QString
        qDebug("ext: %s" % qPrintable(ext))

        # Read
        p = embPattern_create()  # EmbPattern*
        if not p:
            print("Could not allocate memory for embroidery pattern\n")
            exit(1)
        readSuccessful = 0  # int
        ## QString readError
        reader = embReaderWriter_getByFileName(qPrintable(fileName))  # EmbReaderWriter*
        if not reader:
            readSuccessful = 0
            readError = "Unsupported read file type: " + fileName
            qDebug("Unsupported read file type: %s\n" % qPrintable(fileName))
        else:
            readSuccessful = reader.reader(p, qPrintable(fileName))
            if not readSuccessful:
                readError = "Reading file was unsuccessful: " + fileName
                qDebug("Reading file was unsuccessful: %s\n" % qPrintable(fileName))

        ## free(reader)  #TODO/REMOVE# not needed in python
        if not readSuccessful:
            QMessageBox.warning(self, self.tr("Error reading pattern"), self.tr(qPrintable(readError)))

        if readSuccessful:
            embPattern_moveStitchListToPolylines(p)  # TODO: Test more
            stitchCount = embStitchList_count(p.stitchList)  # int
            path = QPainterPath()

            if p.circleObjList:
                curCircleObj = p.circleObjList  # EmbCircleObjectList*
                while curCircleObj:
                    c = curCircleObj.circleObj.circle  # EmbCircle
                    thisColor = curCircleObj.circleObj.color  # EmbColor
                    self.setCurrentColor(qRgb(thisColor.r, thisColor.g, thisColor.b))
                    # NOTE: With natives, the Y+ is up and libembroidery Y+ is up, so inverting the Y is NOT needed.
                    self.mainWin.nativeAddCircle(embCircle_centerX(c), embCircle_centerY(c), embCircle_radius(c), False, OBJ_RUBBER_OFF)  # TODO: fill
                    curCircleObj = curCircleObj.next

            if p.ellipseObjList:
                curEllipseObj = p.ellipseObjList  # EmbEllipseObjectList*
                while curEllipseObj:
                    e = curEllipseObj.ellipseObj.ellipse  # EmbEllipse
                    thisColor = curEllipseObj.ellipseObj.color  # EmbColor
                    self.setCurrentColor(qRgb(thisColor.r, thisColor.g, thisColor.b))
                    # NOTE: With natives, the Y+ is up and libembroidery Y+ is up, so inverting the Y is NOT needed.
                    self.mainWin.nativeAddEllipse(embEllipse_centerX(e), embEllipse_centerY(e), embEllipse_width(e), embEllipse_height(e), 0, False, OBJ_RUBBER_OFF)  # TODO: rotation and fill
                    curEllipseObj = curEllipseObj.next

            if p.lineObjList:
                curLineObj = p.lineObjList  # EmbLineObjectList*
                while curLineObj:
                    li = curLineObj.lineObj.line  # EmbLine
                    thisColor = curLineObj.lineObj.color  # EmbColor
                    self.setCurrentColor(qRgb(thisColor.r, thisColor.g, thisColor.b))
                    # NOTE: With natives, the Y+ is up and libembroidery Y+ is up, so inverting the Y is NOT needed.
                    self.mainWin.nativeAddLine(embLine_x1(li), embLine_y1(li), embLine_x2(li), embLine_y2(li), 0, OBJ_RUBBER_OFF)  # TODO: rotation
                    curLineObj = curLineObj.next

            if p.pathObjList:
                # TODO: This is unfinished. It needs more work
                curPathObjList = p.pathObjList  # EmbPathObjectList*
                while curPathObjList:
                    pathPath = QPainterPath()
                    curPointList = curPathObjList.pathObj.pointList  # EmbPointList*
                    thisColor = curPathObjList.pathObj.color  # EmbColor
                    if curPointList:
                        pp = curPointList.point  # EmbPoint
                        pathPath.moveTo(embPoint_x(pp), -embPoint_y(pp))  # NOTE: Qt Y+ is down and libembroidery Y+ is up, so inverting the Y is needed.
                        curPointList = curPointList.next

                    while curPointList:
                        pp = curPointList.point  # EmbPoint
                        pathPath.lineTo(embPoint_x(pp), -embPoint_y(pp))  # NOTE: Qt Y+ is down and libembroidery Y+ is up, so inverting the Y is needed.
                        curPointList = curPointList.next

                    loadPen = QPen(qRgb(thisColor.r, thisColor.g, thisColor.b))
                    loadPen.setWidthF(0.35)
                    loadPen.setCapStyle(Qt.RoundCap)
                    loadPen.setJoinStyle(Qt.RoundJoin)

                    obj = PathObject(0, 0, pathPath, loadPen.color().rgb())  # PathObject*
                    obj.setObjectRubberMode(OBJ_RUBBER_OFF)
                    self.gscene.addItem(obj)

                    curPathObjList = curPathObjList.next

            if p.pointObjList:
                curPointObj = p.pointObjList  # EmbPointObjectList*
                while curPointObj:
                    po = curPointObj.pointObj.point  # EmbPoint
                    thisColor = curPointObj.pointObj.color  # EmbColor
                    self.setCurrentColor(qRgb(thisColor.r, thisColor.g, thisColor.b))
                    # NOTE: With natives, the Y+ is up and libembroidery Y+ is up, so inverting the Y is NOT needed.
                    self.mainWin.nativeAddPoint(embPoint_x(po), embPoint_y(po))
                    curPointObj = curPointObj.next

            if p.polygonObjList:
                curPolygonObjList = p.polygonObjList  # EmbPolygonObjectList*
                while curPolygonObjList:
                    polygonPath = QPainterPath()
                    firstPoint = False  # bool
                    startX = 0; startY = 0  # qreal
                    x = 0; y = 0  # qreal
                    curPointList = curPolygonObjList.polygonObj.pointList  # EmbPointList*
                    thisColor = curPolygonObjList.polygonObj.color  # EmbColor
                    self.setCurrentColor(qRgb(thisColor.r, thisColor.g, thisColor.b))
                    while curPointList:
                        pp = curPointList.point  # EmbPoint
                        x = embPoint_x(pp)
                        y = -embPoint_y(pp)  # NOTE: Qt Y+ is down and libembroidery Y+ is up, so inverting the Y is needed.

                        if firstPoint:
                            polygonPath.lineTo(x,y)
                        else:
                            polygonPath.moveTo(x,y)
                            firstPoint = True
                            startX = x
                            startY = y

                        curPointList = curPointList.next

                    polygonPath.translate(-startX, -startY)
                    self.mainWin.nativeAddPolygon(startX, startY, polygonPath, OBJ_RUBBER_OFF)

                    curPolygonObjList = curPolygonObjList.next

            # NOTE: Polylines should only contain NORMAL stitches.
            if p.polylineObjList:
                curPolylineObjList = p.polylineObjList  # EmbPolylineObjectList*
                while curPolylineObjList:
                    polylinePath = QPainterPath()
                    firstPoint = False  # bool
                    startX = 0; startY = 0  # qreal
                    x = 0; y = 0  # qreal
                    curPointList = curPolylineObjList.polylineObj.pointList  # EmbPointList*
                    thisColor = curPolylineObjList.polylineObj.color  # EmbColor
                    self.setCurrentColor(qRgb(thisColor.r, thisColor.g, thisColor.b))
                    while curPointList:
                        pp = curPointList.point  # EmbPoint
                        x = embPoint_x(pp)
                        y = -embPoint_y(pp) # NOTE: Qt Y+ is down and libembroidery Y+ is up, so inverting the Y is needed.

                        if firstPoint:
                            polylinePath.lineTo(x,y)
                        else:
                            polylinePath.moveTo(x,y)
                            firstPoint = True
                            startX = x
                            startY = y

                        curPointList = curPointList.next

                    polylinePath.translate(-startX, -startY)
                    self.mainWin.nativeAddPolyline(startX, startY, polylinePath, OBJ_RUBBER_OFF)

                    curPolylineObjList = curPolylineObjList.next


            if p.rectObjList:
                curRectObj = p.rectObjList  # EmbRectObjectList*
                while curRectObj:
                    r = curRectObj.rectObj.rect  # EmbRect
                    thisColor = curRectObj.rectObj.color  # EmbColor
                    self.setCurrentColor(qRgb(thisColor.r, thisColor.g, thisColor.b))
                    # NOTE: With natives, the Y+ is up and libembroidery Y+ is up, so inverting the Y is NOT needed.
                    self.mainWin.nativeAddRectangle(embRect_x(r), embRect_y(r), embRect_width(r), embRect_height(r), 0, False, OBJ_RUBBER_OFF)  # TODO: rotation and fill
                    curRectObj = curRectObj.next

            self.setCurrentFile(fileName)
            self.mainWin.statusbar.showMessage("File loaded.")
            stitches = str(stitchCount)  # QString: stitches.setNum(stitchCount)

            if self.mainWin.getSettingsGridLoadFromFile():
                # TODO: Josh, provide me a hoop size and/or grid spacing from the pattern.
                pass

            QApplication.restoreOverrideCursor()

        else:  #TODO/PORT# warning shown twice?! redundant ?!
            QApplication.restoreOverrideCursor()
            QMessageBox.warning(self, self.tr("Error reading pattern"), self.tr("Cannot read pattern"))

        ## embPattern_free(p)  #TODO/REMOVE# not needed in python

        # Clear the undo stack so it is not possible to undo past this point.
        self.gview.getUndoStack().clear()

        self.setCurrentColor(tmpColor)

        fileWasLoaded = True
        self.mainWin.setUndoCleanIcon(fileWasLoaded)
        return fileWasLoaded

    def setCurrentFile(self, fileName):
        """
        TOWRITE

        :param `fileName`: TOWRITE
        :type `fileName`: QString
        """
        self.curFile = QFileInfo(fileName).canonicalFilePath()
        self.setWindowModified(False)
        self.setWindowTitle(self.getShortCurrentFile())

    def getShortCurrentFile(self):
        """
        TOWRITE

        :rtype: QString
        """
        return QFileInfo(self.curFile).fileName()

    def fileExtension(self, fileName):
        """
        TOWRITE

        :param `fileName`: TOWRITE
        :type `fileName`: QString
        :rtype: QString
        """
        return QFileInfo(fileName).suffix().lower()

    def sizeHint(self):
        """
        TOWRITE

        :rtype: QSize
        """
        qDebug("MdiWindow sizeHint()")
        return QSize(450, 300)

    def promptInputPrevNext(self, prev):
        """
        TOWRITE

        :param `prev`: TOWRITE
        :type `prev`: bool
        """
        if self.promptInputList:  # if(promptInputList.isEmpty())
            if prev:
                QMessageBox.critical(self, self.tr("Prompt Previous Error"), self.tr("The prompt input is empty! Please report this as a bug!"))
            else:
                QMessageBox.critical(self, self.tr("Prompt Next Error"), self.tr("The prompt input is empty! Please report this as a bug!"))
            qDebug("The prompt input is empty! Please report this as a bug!")

        else:
            if prev:
                self.promptInputNum -= 1  # promptInputNum--;
            else:
                self.promptInputNum += 1  # promptInputNum++;
            maxNum = len(self.promptInputList)  # int maxNum = promptInputList.size();
            if self.promptInputNum < 0:
                self.promptInputNum = 0
                self.mainWin.prompt.setCurrentText("")
            elif self.promptInputNum >= maxNum:
                self.promptInputNum = maxNum
                self.mainWin.prompt.setCurrentText("")
            else:
                self.mainWin.prompt.setCurrentText(self.promptInputList[self.promptInputNum])

    # mdiwindow.hhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhh

    def getCurrentFile(self):
        """
        TOWRITE

        :rtype: QString
        """
        return self.curFile

    def getView(self):
        """
        TOWRITE

        :rtype: `View`
        """
        return self.gview

    def getScene(self):
        """
        TOWRITE

        :rtype: `QGraphicsScene`_
        """
        return self.gscene

    def getCurrentLayer(self):
        """
        TOWRITE

        :rtype: QString
        """
        return self.curLayer

    def getCurrentColor(self):
        """
        TOWRITE

        :rtype: QRgb
        """
        return self.curColor

    def getCurrentLineType(self):
        """
        TOWRITE

        :rtype: QString
        """
        return self.curLineType

    def getCurrentLineWeight(self):
        """
        TOWRITE

        :rtype: QString
        """
        return self.curLineWeight

    def setCurrentLayer(self, layer):
        """
        TOWRITE

        :param `layer`: TOWRITE
        :type `layer`: QString
        """
        self.curLayer = layer

    def setCurrentColor(self, color):
        """
        TOWRITE

        :param `color`: TOWRITE
        :type `color`: QRgb
        """
        self.curColor = color

    def setCurrentLineType(self, lineType):
        """
        TOWRITE

        :param `lineType`: TOWRITE
        :type `lineType`: QString
        """
        self.curLineType = lineType

    def setCurrentLineWeight(self, lineWeight):
        """
        TOWRITE

        :param `lineWeight`: TOWRITE
        :type `lineWeight`: QString
        """
        self.curLineWeight = lineWeight

    # Slots ------------------------------------------------------------------

    @Slot()
    def print_(self):
        """
        TOWRITE
        """
        dialog = QPrintDialog(self.printer, self)
        if dialog.exec_() == QDialog.Accepted:

            painter = QPainter(self.printer)
            if self.mainWin.getSettingsPrintingDisableBG():
                # Save current bg.
                brush = self.gview.backgroundBrush()  # QBrush
                # Save ink by not printing the bg at all.
                self.gview.setBackgroundBrush(Qt.NoBrush)
                # Print, fitting the viewport contents into a full page.
                self.gview.render(painter)
                # Restore the bg.
                self.gview.setBackgroundBrush(brush)

            else:
                # Print, fitting the viewport contents into a full page
                self.gview.render(painter)

    @Slot()
    def saveBMC(self):
        """
        .. TODO:: Save a Brother PEL image (An 8bpp, 130x113 pixel monochromatic? bitmap image) Why 8bpp when only 1bpp is needed?

        .. TODO:: Should BMC be limited to ~32KB or is this a mix up with Bitmap Cache?
        .. TODO:: Is there/should there be other embedded data in the bitmap besides the image itself?
        .. NOTE:: Can save a Singer BMC image (An 8bpp, 130x113 pixel colored bitmap image)
        """
        # TODO: figure out how to center the image, right now it just plops it to the left side.
        img = QImage(150, 150, QImage.Format_ARGB32_Premultiplied)
        img.fill(qRgb(255, 255, 255))
        extents = self.gscene.itemsBoundingRect()  # QRectF

        painter = QPainter(img)
        targetRect = QRectF(0, 0, 150, 150)
        if self.mainWin.getSettingsPrintingDisableBG():  # TODO: Make BMC background into it's own setting?
            brush = self.gscene.backgroundBrush()  # QBrush
            self.gscene.setBackgroundBrush(Qt.NoBrush)
            self.gscene.update()
            self.gscene.render(painter, targetRect, extents, Qt.KeepAspectRatio)
            self.gscene.setBackgroundBrush(brush)

        else:
            self.gscene.update()
            self.gscene.render(painter, targetRect, extents, Qt.KeepAspectRatio)

        img.convertToFormat(QImage.Format_Indexed8, Qt.ThresholdDither | Qt.AvoidDither).save("test.bmc", "BMP")

    @Slot()
    def closeEvent(self, QCloseEvent):
        """
        Handles the ``closeEvent`` for :class:`MdiWindow`.

        :param `event`: A `QCloseEvent`_ to be processed.
        """
        qDebug("MdiWindow closeEvent()")
        self.sendCloseMdiWin.emit(self)

    @Slot()
    def onWindowActivated(self):
        """
        TOWRITE
        """
        qDebug("MdiWindow onWindowActivated()")
        self.gview.getUndoStack().setActive(True)
        #TODO/PORT# self.mainWin.setUndoCleanIcon(self.fileWasLoaded)
        #TODO/PORT# self.mainWin.statusbar.statusBarSnapButton.setChecked(self.gscene.property(ENABLE_SNAP))  # .toBool()
        #TODO/PORT# self.mainWin.statusbar.statusBarGridButton.setChecked(self.gscene.property(ENABLE_GRID))  # .toBool()
        #TODO/PORT# self.mainWin.statusbar.statusBarRulerButton.setChecked(self.gscene.property(ENABLE_RULER))  # .toBool()
        #TODO/PORT# self.mainWin.statusbar.statusBarOrthoButton.setChecked(self.gscene.property(ENABLE_ORTHO))  # .toBool()
        #TODO/PORT# self.mainWin.statusbar.statusBarPolarButton.setChecked(self.gscene.property(ENABLE_POLAR))  # .toBool()
        #TODO/PORT# self.mainWin.statusbar.statusBarQSnapButton.setChecked(self.gscene.property(ENABLE_QSNAP))  # .toBool()
        #TODO/PORT# self.mainWin.statusbar.statusBarQTrackButton.setChecked(self.gscene.property(ENABLE_QTRACK))  # .toBool()
        #TODO/PORT# self.mainWin.statusbar.statusBarLwtButton.setChecked(self.gscene.property(ENABLE_LWT))  # .toBool()
        self.mainWin.prompt.setHistory(self.promptHistory)

    @Slot(str)
    def currentLayerChanged(self, layer):
        """
        TOWRITE

        :param `layer`: TOWRITE
        :type `layer`: QString
        """
        self.curLayer = layer

    @Slot(int)
    def currentColorChanged(self, color):
        """
        TOWRITE

        :param `color`: TOWRITE
        :type `color`: QRgb
        """
        self.curColor = color

    @Slot(str)
    def currentLinetypeChanged(self, type):
        """
        TOWRITE

        :param `type`: TOWRITE
        :type `type`: QString
        """
        self.curLineType = type

    @Slot(str)
    def currentLineweightChanged(self, weight):
        """
        TOWRITE

        :param `weight`: TOWRITE
        :type `weight`: QString
        """
        self.curLineWeight = weight

    @Slot()
    def updateColorLinetypeLineweight(self):
        """
        TOWRITE
        """
        pass #TODO/PORT# empty

    @Slot()
    def deletePressed(self):
        """
        TOWRITE
        """
        self.gview.deletePressed()

    @Slot()
    def escapePressed(self):
        """
        TOWRITE
        """
        self.gview.escapePressed()

    @Slot(bool)
    def showViewScrollBars(self, val):
        """
        TOWRITE

        :param `val`: TOWRITE
        :type `val`: bool
        """
        self.gview.showScrollBars(val)

    @Slot(int)
    def setViewCrossHairColor(self, color):
        """
        TOWRITE

        :param `color`: TOWRITE
        :type `color`: QRgb
        """
        self.gview.setCrossHairColor(color)

    @Slot(int)
    def setViewBackgroundColor(self, color):
        """
        TOWRITE

        :param `color`: TOWRITE
        :type `color`: QRgb
        """
        self.gview.setBackgroundColor(color)

    @Slot(int, int, int, int, int)
    def setViewSelectBoxColors(self, colorL, fillL, colorR, fillR, alpha):
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
        self.gview.setSelectBoxColors(colorL, fillL, colorR, fillR, alpha)

    @Slot(int)
    def setViewGridColor(self, color):
        """
        TOWRITE

        :param `color`: TOWRITE
        :type `color`: QRgb
        """
        self.gview.setGridColor(color)

    @Slot(int)
    def setViewRulerColor(self, color):
        """
        TOWRITE

        :param `color`: TOWRITE
        :type `color`: QRgb
        """
        self.gview.setRulerColor(color)

    @Slot(str)
    def promptHistoryAppended(self, txt):
        """
        TOWRITE

        :param `txt`: TOWRITE
        :type `txt`: QString
        """
        self.promptHistory += "<br/>" + txt

    @Slot(str)
    def logPromptInput(self, txt):
        """
        TOWRITE

        :param `txt`: TOWRITE
        :type `txt`: QString
        """
        self.promptInputList.append(txt)
        self.promptInputNum = len(self.promptInputList)  # promptInputList.size()

    @Slot()
    def promptInputPrevious(self):
        """
        TOWRITE
        """
        self.promptInputPrevNext(True)

    @Slot()
    def promptInputNext(self):
        """
        TOWRITE
        """
        self.promptInputPrevNext(False)

    # Signals ----------------------------------------------------------------

    sendCloseMdiWin = Signal(object)  # Signal(MdiWindow)



# kate: bom off; indent-mode python; indent-width 4; replace-trailing-space-save on;
