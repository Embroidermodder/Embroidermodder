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

#--PySide/PyQt Imports.
try:
    from PySide.QtCore import qDebug, Qt, QFile, QSize
    from PySide.QtGui import (QMdiSubWindow, QGraphicsScene, QGraphicsItem,
        QApplication, QBrush, QColor, QDialog,
        QIcon, QMessageBox, QPrinter, QPrintDialog)
    PYSIDE = True
    PYQT4 = False
except ImportError:
    raise
#    from PyQt4.QtCore import qDebug, Qt, QFile, QSize
#    from PyQt4.QtGui import (QMdiSubWindow, QGraphicsScene, QGraphicsItem,
#        QApplication, QBrush, QColor, QDialog,
#        QIcon, QMessageBox, QPrinter, QPrintDialog)
#    PYSIDE = False
#    PYQT4 = True


#--Local Imports.
from view import View
from object_data import *

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
        curFile = "Untitled%d.dst" % myIndex # curFile = aName.sprintf("Untitled%d.dst", myIndex);
        self.setWindowTitle(curFile)

        self.setWindowIcon(QIcon(mainWin.gIconDir + os.sep + mainWin.getSettingsGeneralIconTheme() + "/" + "app" + ".png"));

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
        qDebug("MdiWindow loadFile()");

        #TODO/PORT# QRgb tmpColor = getCurrentColor();
        #TODO/PORT#
        #TODO/PORT# QFile file(fileName);
        #TODO/PORT# if(!file.open(QFile::ReadOnly | QFile::Text))
        #TODO/PORT# {
        #TODO/PORT#     QMessageBox::warning(this, tr("Error reading file"),
        #TODO/PORT#                          tr("Cannot read file %1:\n%2.")
        #TODO/PORT#                          .arg(fileName)
        #TODO/PORT#                          .arg(file.errorString()));
        #TODO/PORT#     return false;
        #TODO/PORT# }
        #TODO/PORT#
        #TODO/PORT# QApplication::setOverrideCursor(Qt::WaitCursor);
        #TODO/PORT#
        #TODO/PORT# QString ext = fileExtension(fileName);
        #TODO/PORT# qDebug("ext: %s", qPrintable(ext));
        #TODO/PORT#
        #TODO/PORT# //Read
        #TODO/PORT# EmbPattern* p = embPattern_create();
        #TODO/PORT# if(!p) { printf("Could not allocate memory for embroidery pattern\n"); exit(1); }
        #TODO/PORT# int readSuccessful = 0;
        #TODO/PORT# QString readError;
        #TODO/PORT# EmbReaderWriter* reader = embReaderWriter_getByFileName(qPrintable(fileName));
        #TODO/PORT# if(!reader)
        #TODO/PORT# {
        #TODO/PORT#     readSuccessful = 0;
        #TODO/PORT#     readError = "Unsupported read file type: " + fileName;
        #TODO/PORT#     qDebug("Unsupported read file type: %s\n", qPrintable(fileName));
        #TODO/PORT# }
        #TODO/PORT# else
        #TODO/PORT# {
        #TODO/PORT#     readSuccessful = reader->reader(p, qPrintable(fileName));
        #TODO/PORT#     if(!readSuccessful)
        #TODO/PORT#     {
        #TODO/PORT#         readError = "Reading file was unsuccessful: " + fileName;
        #TODO/PORT#         qDebug("Reading file was unsuccessful: %s\n", qPrintable(fileName));
        #TODO/PORT#     }
        #TODO/PORT# }
        #TODO/PORT# free(reader);
        #TODO/PORT# if(!readSuccessful)
        #TODO/PORT# {
        #TODO/PORT#     QMessageBox::warning(this, tr("Error reading pattern"), tr(qPrintable(readError)));
        #TODO/PORT# }
        #TODO/PORT#
        #TODO/PORT# if(readSuccessful)
        #TODO/PORT# {
        #TODO/PORT#     embPattern_moveStitchListToPolylines(p); //TODO: Test more
        #TODO/PORT#     int stitchCount = embStitchList_count(p->stitchList);
        #TODO/PORT#     QPainterPath path;
        #TODO/PORT#
        #TODO/PORT#     if(p->circleObjList)
        #TODO/PORT#     {
        #TODO/PORT#         EmbCircleObjectList* curCircleObj = p->circleObjList;
        #TODO/PORT#         while(curCircleObj)
        #TODO/PORT#         {
        #TODO/PORT#             EmbCircle c = curCircleObj->circleObj.circle;
        #TODO/PORT#             EmbColor thisColor = curCircleObj->circleObj.color;
        #TODO/PORT#             setCurrentColor(qRgb(thisColor.r, thisColor.g, thisColor.b));
        #TODO/PORT#             //NOTE: With natives, the Y+ is up and libembroidery Y+ is up, so inverting the Y is NOT needed.
        #TODO/PORT#             mainWin->nativeAddCircle(embCircle_centerX(c), embCircle_centerY(c), embCircle_radius(c), false, OBJ_RUBBER_OFF); //TODO: fill
        #TODO/PORT#             curCircleObj = curCircleObj->next;
        #TODO/PORT#         }
        #TODO/PORT#     }
        #TODO/PORT#     if(p->ellipseObjList)
        #TODO/PORT#     {
        #TODO/PORT#         EmbEllipseObjectList* curEllipseObj = p->ellipseObjList;
        #TODO/PORT#         while(curEllipseObj)
        #TODO/PORT#         {
        #TODO/PORT#             EmbEllipse e = curEllipseObj->ellipseObj.ellipse;
        #TODO/PORT#             EmbColor thisColor = curEllipseObj->ellipseObj.color;
        #TODO/PORT#             setCurrentColor(qRgb(thisColor.r, thisColor.g, thisColor.b));
        #TODO/PORT#             //NOTE: With natives, the Y+ is up and libembroidery Y+ is up, so inverting the Y is NOT needed.
        #TODO/PORT#             mainWin->nativeAddEllipse(embEllipse_centerX(e), embEllipse_centerY(e), embEllipse_width(e), embEllipse_height(e), 0, false, OBJ_RUBBER_OFF); //TODO: rotation and fill
        #TODO/PORT#             curEllipseObj = curEllipseObj->next;
        #TODO/PORT#         }
        #TODO/PORT#     }
        #TODO/PORT#     if(p->lineObjList)
        #TODO/PORT#     {
        #TODO/PORT#         EmbLineObjectList* curLineObj = p->lineObjList;
        #TODO/PORT#         while(curLineObj)
        #TODO/PORT#         {
        #TODO/PORT#             EmbLine li = curLineObj->lineObj.line;
        #TODO/PORT#             EmbColor thisColor = curLineObj->lineObj.color;
        #TODO/PORT#             setCurrentColor(qRgb(thisColor.r, thisColor.g, thisColor.b));
        #TODO/PORT#             //NOTE: With natives, the Y+ is up and libembroidery Y+ is up, so inverting the Y is NOT needed.
        #TODO/PORT#             mainWin->nativeAddLine(embLine_x1(li), embLine_y1(li), embLine_x2(li), embLine_y2(li), 0, OBJ_RUBBER_OFF); //TODO: rotation
        #TODO/PORT#             curLineObj = curLineObj->next;
        #TODO/PORT#         }
        #TODO/PORT#     }
        #TODO/PORT#     if(p->pathObjList)
        #TODO/PORT#     {
        #TODO/PORT#         # TODO: This is unfinished. It needs more work
        #TODO/PORT#         EmbPathObjectList* curPathObjList = p->pathObjList;
        #TODO/PORT#         while(curPathObjList)
        #TODO/PORT#         {
        #TODO/PORT#             QPainterPath pathPath;
        #TODO/PORT#             EmbPointList* curPointList = curPathObjList->pathObj->pointList;
        #TODO/PORT#             EmbColor thisColor = curPathObjList->pathObj->color;
        #TODO/PORT#             if(curPointList)
        #TODO/PORT#             {
        #TODO/PORT#                 EmbPoint pp = curPointList->point;
        #TODO/PORT#                 pathPath.moveTo(embPoint_x(pp), -embPoint_y(pp)); //NOTE: Qt Y+ is down and libembroidery Y+ is up, so inverting the Y is needed.
        #TODO/PORT#                 curPointList = curPointList->next;
        #TODO/PORT#             }
        #TODO/PORT#             while(curPointList)
        #TODO/PORT#             {
        #TODO/PORT#                 EmbPoint pp = curPointList->point;
        #TODO/PORT#                 pathPath.lineTo(embPoint_x(pp), -embPoint_y(pp)); //NOTE: Qt Y+ is down and libembroidery Y+ is up, so inverting the Y is needed.
        #TODO/PORT#                 curPointList = curPointList->next;
        #TODO/PORT#             }
        #TODO/PORT#
        #TODO/PORT#             QPen loadPen(qRgb(thisColor.r, thisColor.g, thisColor.b));
        #TODO/PORT#             loadPen.setWidthF(0.35);
        #TODO/PORT#             loadPen.setCapStyle(Qt::RoundCap);
        #TODO/PORT#             loadPen.setJoinStyle(Qt::RoundJoin);
        #TODO/PORT#
        #TODO/PORT#             PathObject* obj = new PathObject(0,0, pathPath, loadPen.color().rgb());
        #TODO/PORT#             obj->setObjectRubberMode(OBJ_RUBBER_OFF);
        #TODO/PORT#             gscene->addItem(obj);
        #TODO/PORT#
        #TODO/PORT#             curPathObjList = curPathObjList->next;
        #TODO/PORT#         }
        #TODO/PORT#     }
        #TODO/PORT#     if(p->pointObjList)
        #TODO/PORT#     {
        #TODO/PORT#         EmbPointObjectList* curPointObj = p->pointObjList;
        #TODO/PORT#         while(curPointObj)
        #TODO/PORT#         {
        #TODO/PORT#             EmbPoint po = curPointObj->pointObj.point;
        #TODO/PORT#             EmbColor thisColor = curPointObj->pointObj.color;
        #TODO/PORT#             setCurrentColor(qRgb(thisColor.r, thisColor.g, thisColor.b));
        #TODO/PORT#             //NOTE: With natives, the Y+ is up and libembroidery Y+ is up, so inverting the Y is NOT needed.
        #TODO/PORT#             mainWin->nativeAddPoint(embPoint_x(po), embPoint_y(po));
        #TODO/PORT#             curPointObj = curPointObj->next;
        #TODO/PORT#         }
        #TODO/PORT#     }
        #TODO/PORT#     if(p->polygonObjList)
        #TODO/PORT#     {
        #TODO/PORT#         EmbPolygonObjectList* curPolygonObjList = p->polygonObjList;
        #TODO/PORT#         while(curPolygonObjList)
        #TODO/PORT#         {
        #TODO/PORT#             QPainterPath polygonPath;
        #TODO/PORT#             bool firstPoint = false;
        #TODO/PORT#             qreal startX = 0, startY = 0;
        #TODO/PORT#             qreal x = 0, y = 0;
        #TODO/PORT#             EmbPointList* curPointList = curPolygonObjList->polygonObj->pointList;
        #TODO/PORT#             EmbColor thisColor = curPolygonObjList->polygonObj->color;
        #TODO/PORT#             setCurrentColor(qRgb(thisColor.r, thisColor.g, thisColor.b));
        #TODO/PORT#             while(curPointList)
        #TODO/PORT#             {
        #TODO/PORT#                 EmbPoint pp = curPointList->point;
        #TODO/PORT#                 x = embPoint_x(pp);
        #TODO/PORT#                 y = -embPoint_y(pp); //NOTE: Qt Y+ is down and libembroidery Y+ is up, so inverting the Y is needed.
        #TODO/PORT#
        #TODO/PORT#                 if(firstPoint) { polygonPath.lineTo(x,y); }
        #TODO/PORT#                 else           { polygonPath.moveTo(x,y); firstPoint = true; startX = x; startY = y; }
        #TODO/PORT#
        #TODO/PORT#                 curPointList = curPointList->next;
        #TODO/PORT#             }
        #TODO/PORT#
        #TODO/PORT#             polygonPath.translate(-startX, -startY);
        #TODO/PORT#             mainWin->nativeAddPolygon(startX, startY, polygonPath, OBJ_RUBBER_OFF);
        #TODO/PORT#
        #TODO/PORT#             curPolygonObjList = curPolygonObjList->next;
        #TODO/PORT#         }
        #TODO/PORT#     }
        #TODO/PORT#     # NOTE: Polylines should only contain NORMAL stitches.
        #TODO/PORT#     if(p->polylineObjList)
        #TODO/PORT#     {
        #TODO/PORT#         EmbPolylineObjectList* curPolylineObjList = p->polylineObjList;
        #TODO/PORT#         while(curPolylineObjList)
        #TODO/PORT#         {
        #TODO/PORT#             QPainterPath polylinePath;
        #TODO/PORT#             bool firstPoint = false;
        #TODO/PORT#             qreal startX = 0, startY = 0;
        #TODO/PORT#             qreal x = 0, y = 0;
        #TODO/PORT#             EmbPointList* curPointList = curPolylineObjList->polylineObj->pointList;
        #TODO/PORT#             EmbColor thisColor = curPolylineObjList->polylineObj->color;
        #TODO/PORT#             setCurrentColor(qRgb(thisColor.r, thisColor.g, thisColor.b));
        #TODO/PORT#             while(curPointList)
        #TODO/PORT#             {
        #TODO/PORT#                 EmbPoint pp = curPointList->point;
        #TODO/PORT#                 x = embPoint_x(pp);
        #TODO/PORT#                 y = -embPoint_y(pp); # NOTE: Qt Y+ is down and libembroidery Y+ is up, so inverting the Y is needed.
        #TODO/PORT#
        #TODO/PORT#                 if(firstPoint) { polylinePath.lineTo(x,y); }
        #TODO/PORT#                 else           { polylinePath.moveTo(x,y); firstPoint = true; startX = x; startY = y; }
        #TODO/PORT#
        #TODO/PORT#                 curPointList = curPointList->next;
        #TODO/PORT#             }
        #TODO/PORT#
        #TODO/PORT#             polylinePath.translate(-startX, -startY);
        #TODO/PORT#             mainWin->nativeAddPolyline(startX, startY, polylinePath, OBJ_RUBBER_OFF);
        #TODO/PORT#
        #TODO/PORT#             curPolylineObjList = curPolylineObjList->next;
        #TODO/PORT#         }
        #TODO/PORT#     }
        #TODO/PORT#     if(p->rectObjList)
        #TODO/PORT#     {
        #TODO/PORT#         EmbRectObjectList* curRectObj = p->rectObjList;
        #TODO/PORT#         while(curRectObj)
        #TODO/PORT#         {
        #TODO/PORT#             EmbRect r = curRectObj->rectObj.rect;
        #TODO/PORT#             EmbColor thisColor = curRectObj->rectObj.color;
        #TODO/PORT#             setCurrentColor(qRgb(thisColor.r, thisColor.g, thisColor.b));
        #TODO/PORT#             # NOTE: With natives, the Y+ is up and libembroidery Y+ is up, so inverting the Y is NOT needed.
        #TODO/PORT#             mainWin->nativeAddRectangle(embRect_x(r), embRect_y(r), embRect_width(r), embRect_height(r), 0, false, OBJ_RUBBER_OFF); //TODO: rotation and fill
        #TODO/PORT#             curRectObj = curRectObj->next;
        #TODO/PORT#         }
        #TODO/PORT#     }
        #TODO/PORT#
        #TODO/PORT#     setCurrentFile(fileName);
        #TODO/PORT#     mainWin->statusbar->showMessage("File loaded.");
        #TODO/PORT#     QString stitches;
        #TODO/PORT#     stitches.setNum(stitchCount);
        #TODO/PORT#
        #TODO/PORT#     if(mainWin->getSettingsGridLoadFromFile())
        #TODO/PORT#     {
        #TODO/PORT#         # TODO: Josh, provide me a hoop size and/or grid spacing from the pattern.
        #TODO/PORT#     }
        #TODO/PORT#
        #TODO/PORT#     QApplication::restoreOverrideCursor();
        #TODO/PORT# }
        #TODO/PORT# else
        #TODO/PORT# {
        #TODO/PORT#     QApplication::restoreOverrideCursor();
        #TODO/PORT#     QMessageBox::warning(this, tr("Error reading pattern"), tr("Cannot read pattern"));
        #TODO/PORT# }
        #TODO/PORT# embPattern_free(p);
        #TODO/PORT#
        #TODO/PORT# # Clear the undo stack so it is not possible to undo past this point.
        #TODO/PORT# gview->getUndoStack()->clear();
        #TODO/PORT#
        #TODO/PORT# setCurrentColor(tmpColor);
        #TODO/PORT#
        #TODO/PORT# fileWasLoaded = true;
        #TODO/PORT# mainWin->setUndoCleanIcon(fileWasLoaded);
        #TODO/PORT# return fileWasLoaded;

    def print_(self):
        """
        TOWRITE
        """
        dialog = QPrintDialog(self.printer, self)
        if dialog.exec_() == QDialog.Accepted:

            painter = QPainter(self.printer)
            if self.mainWin.getSettingsPrintingDisableBG():
                # Save current bg.
                brush = gview.backgroundBrush()  # QBrush
                # Save ink by not printing the bg at all.
                gview.setBackgroundBrush(Qt.NoBrush)
                # Print, fitting the viewport contents into a full page.
                gview.render(painter)
                # Restore the bg.
                gview.setBackgroundBrush(brush)

            else:
                # Print, fitting the viewport contents into a full page
                gview.render(painter)

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
            self.gscene.render(painter, targetRect, extents, Qt.KeepAspectRatio);
            self.gscene.setBackgroundBrush(brush)

        else:
            self.gscene.update()
            self.gscene.render(painter, targetRect, extents, Qt.KeepAspectRatio)

        img.convertToFormat(QImage.Format_Indexed8, Qt.ThresholdDither | Qt.AvoidDither).save("test.bmc", "BMP")

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
        return QFileInfo(fileName).suffix().toLower()

    def closeEvent(self, event):
        """
        Handles the ``closeEvent`` for :class:`MdiWindow`.

        :param `event`: A `QCloseEvent`_ to be processed.
        """
        qDebug("MdiWindow closeEvent()")
        #TODO/PORT# emit sendCloseMdiWin(this)

    def onWindowActivated(self):
        """
        TOWRITE
        """
        qDebug("MdiWindow onWindowActivated()")
        self.gview.getUndoStack().setActive(True)
        self.mainWin.setUndoCleanIcon(self.fileWasLoaded)
        #TODO/PORT# self.mainWin.statusbar.statusBarSnapButton.setChecked(self.gscene.property(ENABLE_SNAP))  # .toBool()
        #TODO/PORT# self.mainWin.statusbar.statusBarGridButton.setChecked(self.gscene.property(ENABLE_GRID))  # .toBool()
        #TODO/PORT# self.mainWin.statusbar.statusBarRulerButton.setChecked(self.gscene.property(ENABLE_RULER))  # .toBool()
        #TODO/PORT# self.mainWin.statusbar.statusBarOrthoButton.setChecked(self.gscene.property(ENABLE_ORTHO))  # .toBool()
        #TODO/PORT# self.mainWin.statusbar.statusBarPolarButton.setChecked(self.gscene.property(ENABLE_POLAR))  # .toBool()
        #TODO/PORT# self.mainWin.statusbar.statusBarQSnapButton.setChecked(self.gscene.property(ENABLE_QSNAP))  # .toBool()
        #TODO/PORT# self.mainWin.statusbar.statusBarQTrackButton.setChecked(self.gscene.property(ENABLE_QTRACK))  # .toBool()
        #TODO/PORT# self.mainWin.statusbar.statusBarLwtButton.setChecked(self.gscene.property(ENABLE_LWT))  # .toBool()
        self.mainWin.prompt.setHistory(self.promptHistory)

    def sizeHint(self):
        """
        TOWRITE

        :rtype: QSize
        """
        qDebug("MdiWindow sizeHint()")
        return QSize(450, 300)

    def currentLayerChanged(self, layer):
        """
        TOWRITE

        :param `layer`: TOWRITE
        :type `layer`: QString
        """
        self.curLayer = layer

    def currentColorChanged(self, color):
        """
        TOWRITE

        :param `color`: TOWRITE
        :type `color`: QRgb
        """
        self.curColor = color

    def currentLinetypeChanged(self, type):
        """
        TOWRITE

        :param `type`: TOWRITE
        :type `type`: QString
        """
        self.curLineType = type

    def currentLineweightChanged(self, weight):
        """
        TOWRITE

        :param `weight`: TOWRITE
        :type `weight`: QString
        """
        self.curLineWeight = weight

    def updateColorLinetypeLineweight(self):
        """
        TOWRITE
        """
        pass #TODO/PORT# empty

    def deletePressed(self):
        """
        TOWRITE
        """
        self.gview.deletePressed()

    def escapePressed(self):
        """
        TOWRITE
        """
        self.gview.escapePressed()

    def showViewScrollBars(self, val):
        """
        TOWRITE

        :param `val`: TOWRITE
        :type `val`: bool
        """
        self.gview.showScrollBars(val)

    def setViewCrossHairColor(self, color):
        """
        TOWRITE

        :param `color`: TOWRITE
        :type `color`: QRgb
        """
        self.gview.setCrossHairColor(color)

    def setViewBackgroundColor(self, color):
        """
        TOWRITE

        :param `color`: TOWRITE
        :type `color`: QRgb
        """
        self.gview.setBackgroundColor(color)

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

    def setViewGridColor(self, color):
        """
        TOWRITE

        :param `color`: TOWRITE
        :type `color`: QRgb
        """
        self.gview.setGridColor(color)

    def setViewRulerColor(self, color):
        """
        TOWRITE

        :param `color`: TOWRITE
        :type `color`: QRgb
        """
        self.gview.setRulerColor(color)

    def promptHistoryAppended(self, txt):
        """
        TOWRITE

        :param `txt`: TOWRITE
        :type `txt`: QString
        """
        self.promptHistory.append("<br/>" + txt)

    def logPromptInput(self, txt):
        """
        TOWRITE

        :param `txt`: TOWRITE
        :type `txt`: QString
        """
        self.promptInputList.append(txt)
        self.promptInputNum = len(self.promptInputList)  # promptInputList.size()

    def promptInputPrevious(self):
        """
        TOWRITE
        """
        self.promptInputPrevNext(True)

    def promptInputNext(self):
        """
        TOWRITE
        """
        self.promptInputPrevNext(False)

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



# kate: bom off; indent-mode python; indent-width 4; replace-trailing-space-save on;
