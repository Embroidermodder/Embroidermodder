#!/usr/bin/env python
# -*- coding: utf-8 -*-

"""
========================
|module_summary| view.py
========================

TOWRITE

Classes summary:
================

============================ ============================
:class:`~View`               TOWRITE
============================ ============================

---------------------------------------------------------

|

"""

#-Imports.---------------------------------------------------------------------
#--PySide/PyQt Imports.
try:
    ## from PySide import QtCore, QtGui
    # or... Improve performace with less dots...
    from PySide.QtCore import qDebug, Qt, QDateTime, QLineF, QPoint, QPointF, QRect, QRectF, QSize # QChar, QString
    from PySide.QtGui import (qRgb, QAction, QApplication, QBrush, QColor, QFrame,
        QGL, QGLFormat, QGLWidget, QGraphicsItem, QGraphicsScene, QGraphicsView,
        QIcon, QList, QMenu, QMessageBox, QObject, QPainter, QPainterPath,
        QPen, QPixmap, QPushButton, QRubberBand,
        QTransform, QUndoStack # , QVector
        )
    PYSIDE = True
    PYQT4 = False

except ImportError:
    raise
#    ## from PyQt4 import QtCore, QtGui
#    # or... Improve performace with less dots...
#    from PyQt4.QtCore import qDebug, Qt, QDateTime, QLineF, QPoint, QPointF, QRect, QRectF, QSize # QChar, QString
#    from PyQt4.QtGui import (qRgb, QAction, QApplication, QBrush, QColor, QFrame,
#        QGL, QGLFormat, QGLWidget, QGraphicsItem, QGraphicsScene, QGraphicsView,
#        QIcon, QList, QMenu, QMessageBox, QObject, QPainter, QPainterPath,
#        QPen, QPixmap, QPushButton, QRubberBand,
#        QTransform, QUndoStack # , QVector
#        )
#    PYSIDE = False
#    PYQT4 = True


# C++C++C++C++C++C++C++C++C++C++C++C++C++C++C++C++C++C++C++C++C++C++C++C++C++
#include "view.h"
#include "property-editor.h"
#include "statusbar.h"
#include "object-data.h"
#include "object-base.h"
#include "undo-editor.h"
#include "undo-commands.h"

#include "selectbox.h"

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

#include <QtGui>
#include <QGraphicsScene>
#include <QGLWidget>
# C++C++C++C++C++C++C++C++C++C++C++C++C++C++C++C++C++C++C++C++C++C++C++C++C++


class View(QGraphicsView):
    """
    Subclass of `QGraphicsView`_

    TOWRITE

    """
    def __init__(self, mw, theScene, parent=None):
        """
        Default class constructor.

        :param `mw`: The applications main window instance.
        :type `mw`: `QMainWindow`_
        :param `theScene`: TOWRITE
        :type `theScene`: `QGraphicsScene`_
        :param `parent`: Pointer to a parent widget instance.
        :type `parent`: `QWidget`_
        """
        super(View, self).__init__(theScene, parent)

        global mainWin
        self.mainWin = mainWin = mw

        self.gscene = gscene = theScene

        #TODO/PORT# setFrameShape(QFrame.NoFrame)

        # NOTE: This has to be done before setting mouse tracking.
        # TODO: Review OpenGL for Qt5 later
        # if(mainWin->getSettingsDisplayUseOpenGL())
        # {
        #     qDebug("Using OpenGL...");
        #     setViewport(new QGLWidget(QGLFormat(QGL::DoubleBuffer)));
        # }

        # TODO: Review RenderHints later
        # setRenderHint(QPainter::Antialiasing,            mainWin->getSettingsDisplayRenderHintAA());
        # setRenderHint(QPainter::TextAntialiasing,        mainWin->getSettingsDisplayRenderHintTextAA());
        # setRenderHint(QPainter::SmoothPixmapTransform,   mainWin->getSettingsDisplayRenderHintSmoothPix());
        # setRenderHint(QPainter::HighQualityAntialiasing, mainWin->getSettingsDisplayRenderHintHighAA());
        # setRenderHint(QPainter::NonCosmeticDefaultPen,   mainWin->getSettingsDisplayRenderHintNonCosmetic());

        # NOTE: FullViewportUpdate MUST be used for both the GL and Qt renderers.
        # NOTE: Qt renderer will not draw the foreground properly if it isnt set.
        self.setViewportUpdateMode(QGraphicsView.FullViewportUpdate)

        self.panDistance = 10  # TODO: should there be a setting for this???

        self.setCursor(Qt.BlankCursor)
        self.horizontalScrollBar().setCursor(Qt.ArrowCursor)
        self.verticalScrollBar().setCursor(Qt.ArrowCursor)
        self.qsnapLocatorColor = mainWin.getSettingsQSnapLocatorColor()
        self.qsnapLocatorSize = mainWin.getSettingsQSnapLocatorSize()
        self.qsnapApertureSize = mainWin.getSettingsQSnapApertureSize()
        self.gripColorCool = mainWin.getSettingsSelectionCoolGripColor()
        self.gripColorHot = mainWin.getSettingsSelectionHotGripColor()
        self.gripSize = mainWin.getSettingsSelectionGripSize()
        self.pickBoxSize = mainWin.getSettingsSelectionPickBoxSize()
        self.setCrossHairColor(mainWin.getSettingsDisplayCrossHairColor())
        self.setCrossHairSize(mainWin.getSettingsDisplayCrossHairPercent())
        self.setGridColor(mainWin.getSettingsGridColor())

        if mainWin.getSettingsGridShowOnLoad():
            createGrid(mainWin.getSettingsGridType())
        else:
            createGrid("")

        self.toggleRuler(mainWin.getSettingsRulerShowOnLoad())
        self.toggleReal(True)  # TODO: load this from file, else settings with default being True

        self.grippingActive = False
        self.rapidMoveActive = False
        self.previewMode = PREVIEW_MODE_NULL
        self.previewData = 0
        self.previewObjectItemGroup = 0
        self.pasteObjectItemGroup = 0
        self.previewActive = False
        self.pastingActive = False
        self.movingActive = False
        self.selectingActive = False
        self.zoomWindowActive = False
        self.panningRealTimeActive = False
        self.panningPointActive = False
        self.panningActive = False
        self.qSnapActive = False
        self.qSnapToggle = False

        # Randomize the hot grip location initially so it's not located at (0,0)
        #TODO/PORT# qsrand(QDateTime.currentMSecsSinceEpoch())
        self.sceneGripPoint = QPointF(qrand() * 1000, qrand() * 1000)

        self.gripBaseObj = 0
        self.tempBaseObj = 0

        self.selectBox = selectBox = SelectBox(QRubberBand.Rectangle, self)
        selectBox.setColors(QColor(mainWin.getSettingsDisplaySelectBoxLeftColor()),
                            QColor(mainWin.getSettingsDisplaySelectBoxLeftFill()),
                            QColor(mainWin.getSettingsDisplaySelectBoxRightColor()),
                            QColor(mainWin.getSettingsDisplaySelectBoxRightFill()),
                            mainWin.getSettingsDisplaySelectBoxAlpha())

        self.showScrollBars(mainWin.getSettingsDisplayShowScrollBars())
        self.setCornerButton()

        self.undoStack = undoStack = QUndoStack(self)
        mainWin.dockUndoEdit.addStack(undoStack)

        self.installEventFilter()

        self.setMouseTracking(True)
        self.setBackgroundColor(mainWin.getSettingsDisplayBGColor())
        # TODO: wrap this with a setBackgroundPixmap() function: setBackgroundBrush(QPixmap("images/canvas.png"))

        #TODO/PORT# connect(gscene, SIGNAL(selectionChanged()), this, SLOT(selectionChanged()))


    def __del__(self):
        """Class destructor."""
        # Prevent memory leaks by deleting any objects that were removed from the scene
        qDeleteAll(hashDeletedObjects.begin(), hashDeletedObjects.end())
        hashDeletedObjects.clear()

        # Prevent memory leaks by deleting any unused instances
        qDeleteAll(previewObjectList.begin(), previewObjectList.end())
        previewObjectList.clear()

    def enterEvent(self, event):
        """
        Handles the ``enterEvent`` event for :class:`MDIArea`.

        :param `event`: A `QEvent`_ to be processed.
        """
        #TODO/PORT# QMdiSubWindow* mdiWin = qobject_cast<QMdiSubWindow*>(parent());
        if mdiWin:
            self.mainWin.getMdiArea().setActiveSubWindow(mdiWin)

    def addObject(self, obj):
        """
        TOWRITE

        :param `obj`: TOWRITE
        :type `obj`: `BaseObject`
        """
        self.gscene.addItem(obj)
        self.gscene.update()
        hashDeletedObjects.remove(obj.objectID())

    def deleteObject(self, obj):
        """
        TOWRITE

        :param `obj`: TOWRITE
        :type `obj`: `BaseObject`
        """
        # NOTE: We really just remove the objects from the scene. deletion actually occurs in the destructor.
        obj.setSelected(False)
        self.gscene.removeItem(obj)
        self.gscene.update()
        hashDeletedObjects.insert(obj.objectID(), obj)

    def previewOn(self, clone, mode, x, y, data):
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
        qDebug("View previewOn()")
        self.previewOff()  # Free the old objects before creating new ones

        self.previewMode = mode

        # Create new objects and add them to the scene in an item group.
        if clone == PREVIEW_CLONE_SELECTED:
            previewObjectList = createObjectList(self.gscene.selectedItems())
        elif clone == PREVIEW_CLONE_RUBBER:
            previewObjectList = createObjectList(self.rubberRoomList)
        else:
            return
        self.previewObjectItemGroup = self.gscene.createItemGroup(previewObjectList)

        if (previewMode == PREVIEW_MODE_MOVE or
                previewMode == PREVIEW_MODE_ROTATE or
                previewMode == PREVIEW_MODE_SCALE):

            self.previewPoint = QPointF(x, y)  # NOTE: Move: basePt; Rotate: basePt;   Scale: basePt;
            self.previewData = data            # NOTE: Move: unused; Rotate: refAngle; Scale: refFactor;
            self.previewActive = True
        else:
            self.previewMode = PREVIEW_MODE_NULL
            self.previewPoint = QPointF()
            self.previewData = 0
            self.previewActive = False

        self.gscene.update()

    def previewOff(self):
        """
        TOWRITE
        """
        # Prevent memory leaks by deleting any unused instances
        qDeleteAll(previewObjectList.begin(), previewObjectList.end())
        previewObjectList.clear()

        if previewObjectItemGroup:
            self.gscene.removeItem(previewObjectItemGroup)
            del self.previewObjectItemGroup
            self.previewObjectItemGroup = 0

        self.previewActive = False

        self.gscene.update()

    def enableMoveRapidFire(self):
        """
        TOWRITE
        """
        self.rapidMoveActive = True

    def disableMoveRapidFire(self):
        """
        TOWRITE
        """
        self.rapidMoveActive = False

    def allowRubber(self):
        """
        TOWRITE

        :rtype: bool
        """
        if not self.rubberRoomList.size():  # TODO: this check should be removed later
            return True
        return False

    def addToRubberRoom(self, item):
        """
        TOWRITE

        :param `item`: TOWRITE
        :type `item`: `QGraphicsItem`_
        """
        self.rubberRoomList.append(item)
        item.show()
        self.gscene.update()

    def vulcanizeRubberRoom(self):
        """
        TOWRITE
        """
        for item in self.rubberRoomList: # foreach(QGraphicsItem* item, rubberRoomList)
            #TODO/PORT# BaseObject* base = static_cast<BaseObject*>(item);
            if base:
                vulcanizeObject(base)

        self.rubberRoomList.clear()
        self.gscene.update()

    def vulcanizeObject(self, obj):
        """
        TOWRITE

        :param `obj`: TOWRITE
        :type `obj`: `BaseObject`
        """
        if not obj:
            return
        self.gscene.removeItem(obj)  # Prevent Qt Runtime Warning, QGraphicsScene::addItem: item has already been added to this scene
        obj.vulcanize()

        #TODO/PORT# UndoableAddCommand* cmd = new UndoableAddCommand(obj->data(OBJ_NAME).toString(), obj, this, 0);
        if cmd:
            self.undoStack.push(cmd)

    def clearRubberRoom(self):
        """
        TOWRITE
        """
        for item in self.rubberRoomList:  # foreach(QGraphicsItem* item, rubberRoomList)

            base = item # BaseObject* base = static_cast<BaseObject*>(item);
            if base:

                type = base.type() # int
                if ((type == OBJ_TYPE_PATH     and spareRubberList.contains(SPARE_RUBBER_PATH))     or
                    (type == OBJ_TYPE_POLYGON  and spareRubberList.contains(SPARE_RUBBER_POLYGON))  or
                    (type == OBJ_TYPE_POLYLINE and spareRubberList.contains(SPARE_RUBBER_POLYLINE)) or
                    (spareRubberList.contains(base.objectID()))):

                    if not base.objectPath().elementCount():

                        QMessageBox.critical(self, self.tr("Empty Rubber Object Error"),
                                              self.tr("The rubber object added contains no points. "
                                              "The command that created this object has flawed logic. "
                                              "The object will be deleted."))
                        self.gscene.removeItem(item)
                        del item

                    else:
                        self.vulcanizeObject(base)

                else:
                    self.gscene.removeItem(item)
                    del item

        self.rubberRoomList.clear()
        self.spareRubberList.clear()
        self.gscene.update()

    def spareRubber(self, id):
        """
        TOWRITE

        :param `id`: TOWRITE
        :type `id`: qint64
        """
        self.spareRubberList.append(id)

    def setRubberMode(self, mode):
        """
        TOWRITE

        :param `mode`: TOWRITE
        :type `mode`: int
        """
        ## for item in self.rubberRoomList: # foreach(QGraphicsItem* item, rubberRoomList)
        ##     base = item # BaseObject* base = static_cast<BaseObject*>(item);
        ##     if base:
        ##         base.setObjectRubberMode(mode)
        # Optimized as a python list comprehension generator.
        [item.setObjectRubberMode(mode)
            for item in self.rubberRoomList if item]

        self.gscene.update()

    def setRubberPoint(self, key, point):
        """
        TOWRITE

        :param `key`: TOWRITE
        :type `key`: QString
        :param `point`: TOWRITE
        :type `point`: `QPointF`_
        """
        ## for item in self.rubberRoomList: # foreach(QGraphicsItem* item, rubberRoomList)
        ##     base = item # BaseObject* base = static_cast<BaseObject*>(item);
        ##     if base:
        ##         base.setObjectRubberPoint(key, point)
        # Optimized as a python list comprehension generator.
        [item.setObjectRubberPoint(key, point)
            for item in self.rubberRoomList if item]

        self.gscene.update()

    def setRubberText(self, key, txt):
        """
        TOWRITE

        :param `key`: TOWRITE
        :type `key`: QString
        :param `txt`: TOWRITE
        :type `txt`: QString
        """
        ## for item in self.rubberRoomList: # foreach(QGraphicsItem* item, rubberRoomList)
        ##     base = item # BaseObject* base = static_cast<BaseObject*>(item);
        ##     if base:
        ##         base.setObjectRubberText(key, txt)
        # Optimized as a python list comprehension generator.
        [item.setObjectRubberText(key, txt)
            for item in self.rubberRoomList if item]

        self.gscene.update()

    def setGridColor(self, color):
        """
        TOWRITE

        :param `color`: TOWRITE
        :type `color`: QRgb
        """
        gridColor = QColor(color)
        self.gscene.setProperty(VIEW_COLOR_GRID, color)
        if self.gscene:
            self.gscene.update()

    def setRulerColor(self, color):
        """
        TOWRITE

        :param `color`: TOWRITE
        :type `color`: QRgb
        """
        self.rulerColor = QColor(color)
        self.gscene.update()

    def createGrid(self, gridType):
        """
        TOWRITE

        :param `gridType`: TOWRITE
        :type `gridType`: QString
        """
        if gridType == "Rectangular":
            self.createGridRect()
            self.gscene.setProperty(ENABLE_GRID, True)
        elif gridType == "Circular":
            self.createGridPolar()
            self.gscene.setProperty(ENABLE_GRID, True)
        elif gridType == "Isometric":
            self.createGridIso()
            self.gscene.setProperty(ENABLE_GRID, True)
        else:
            self.gridPath = QPainterPath()
            self.gscene.setProperty(ENABLE_GRID, False)

        self.createOrigin()

        self.gscene.update()

    def createOrigin(self): #TODO: Make Origin Customizable
        """
        TOWRITE
        """
        self.originPath = originPath = QPainterPath()

        if self.mainWin.getSettingsGridShowOrigin():

            # originPath.addEllipse(QPointF(0,0), 0.5, 0.5); //TODO: Make Origin Customizable
            rad = 0.5 # qreal
            originPath.moveTo(0.0, rad)
            originPath.arcTo(-rad, -rad, rad*2.0, rad*2.0, 90.0, 360.0)
            originPath.arcTo(-rad, -rad, rad*2.0, rad*2.0, 90.0, -360.0)
            originPath.lineTo(0.0, -rad)
            originPath.arcTo(-rad, -rad, rad*2.0, rad*2.0, 270.0, 90.0)
            originPath.lineTo(-rad, 0.0)
            originPath.arcTo(-rad, -rad, rad*2.0, rad*2.0, 180.0, -90.0)
            originPath.closeSubpath()

    def createGridRect(self):
        """
        TOWRITE
        """
        mainWin = self.mainWin
        xSpacing = mainWin.getSettingsGridSpacingX() # qreal
        ySpacing = mainWin.getSettingsGridSpacingY() # qreal

        gr = QRectF(0, 0, mainWin.getSettingsGridSizeX(), -mainWin.getSettingsGridSizeY())
        # Ensure the loop will work correctly with negative numbers
        x1 = qMin(gr.left(), gr.right()) # qreal
        y1 = qMin(gr.top(), gr.bottom()) # qreal
        x2 = qMax(gr.left(), gr.right()) # qreal
        y2 = qMax(gr.top(), gr.bottom()) # qreal

        gridPath = QPainterPath()
        gridPath.addRect(gr)

        for gx in range(x1, x2, xSpacing): # for(qreal gx = x1; gx < x2; gx += xSpacing)
            for gy in range(y1, y2, ySpacing): # for(qreal gy = y1; gy < y2; gy += ySpacing)
                gridPath.moveTo(x1, gy)
                gridPath.lineTo(x2, gy)
                gridPath.moveTo(gx, y1)
                gridPath.lineTo(gx, y2)

        # Center the Grid
        gridRect = gridPath.boundingRect()  # QRectF
        bx = gridRect.width() / 2.0             # qreal
        by = -gridRect.height() / 2.0           # qreal
        cx = mainWin.getSettingsGridCenterX()   # qreal
        cy = -mainWin.getSettingsGridCenterY()  # qreal
        dx = cx - bx                            # qreal
        dy = cy - by                            # qreal

        if self.mainWin.getSettingsGridCenterOnOrigin():
            gridPath.translate(-bx, -by)
        else:
            gridPath.translate(dx, dy)

    def createGridPolar(self):
        """
        TOWRITE
        """
        mainWin = self.mainWin
        radSpacing = mainWin.getSettingsGridSpacingRadius()  # qreal
        angSpacing = mainWin.getSettingsGridSpacingAngle()   # qreal

        rad = mainWin.getSettingsGridSizeRadius()  # qreal

        gridPath = QPainterPath()
        gridPath.addEllipse(QPointF(0,0), rad, rad);
        for r in range(0, rad, radSpacing): # for(qreal r = 0; r < rad; r += radSpacing)
            gridPath.addEllipse(QPointF(0,0), r, r);

        for ang in range(0, 360, angSpacing): # for(qreal ang = 0; ang < 360; ang += angSpacing)
            gridPath.moveTo(0, 0)
            gridPath.lineTo(QLineF.fromPolar(rad, ang).p2())

        cx = mainWin.getSettingsGridCenterX()  # qreal
        cy = mainWin.getSettingsGridCenterY()  # qreal

        if not mainWin.getSettingsGridCenterOnOrigin():
            gridPath.translate(cx, -cy)

    def createGridIso(self):
        """
        TOWRITE
        """
        mainWin = self.mainWin
        xSpacing = mainWin.getSettingsGridSpacingX()  # qreal
        ySpacing = mainWin.getSettingsGridSpacingY()  # qreal

        # Ensure the loop will work correctly with negative numbers
        isoW = qAbs(mainWin.getSettingsGridSizeX())  # qreal
        isoH = qAbs(mainWin.getSettingsGridSizeY())  # qreal

        p1 = QPointF(0, 0)                     # QPointF
        p2 = QLineF.fromPolar(isoW,  30).p2()  # QPointF
        p3 = QLineF.fromPolar(isoH, 150).p2()  # QPointF
        p4 = p2 + p3                           # QPointF

        gridPath = QPainterPath()
        gridPath.moveTo(p1)
        gridPath.lineTo(p2)
        gridPath.lineTo(p4)
        gridPath.lineTo(p3)
        gridPath.lineTo(p1)

        for x in range(0, isoW, xSpacing): # for(qreal x = 0; x < isoW; x += xSpacing)
            for y in range(0, isoH, ySpacing): # for(qreal y = 0; y < isoH; y += ySpacing)
                px = QLineF.fromPolar(x,  30).p2()  # QPointF
                py = QLineF.fromPolar(y, 150).p2()  # QPointF

                gridPath.moveTo(px)
                gridPath.lineTo(px + p3)
                gridPath.moveTo(py)
                gridPath.lineTo(py + p2)

        # Center the Grid.

        gridRect = gridPath.boundingRect()  # QRectF
        # bx is unused
        by = -gridRect.height() / 2.0          # qreal
        cx = mainWin.getSettingsGridCenterX()  # qreal
        cy = -mainWin.getSettingsGridCenterY() # qreal

        if mainWin.getSettingsGridCenterOnOrigin():
            gridPath.translate(0, -by)
        else:
            gridPath.translate(0, -by)
            gridPath.translate(cx, cy)

    def toggleSnap(self, on):
        """
        TOWRITE

        :param `on`: TOWRITE
        :type `on`: bool
        """
        qDebug("View toggleSnap()")
        QApplication.setOverrideCursor(Qt.WaitCursor)
        # TODO: finish this
        self.gscene.setProperty(ENABLE_SNAP, on)
        self.gscene.update()
        QApplication.restoreOverrideCursor()

    def toggleGrid(self, on):
        """
        TOWRITE

        :param `on`: TOWRITE
        :type `on`: bool
        """
        qDebug("View toggleGrid()")
        QApplication.setOverrideCursor(Qt.WaitCursor)
        if on:
            self.createGrid(mainWin.getSettingsGridType())
        else:
            self.createGrid("")
        QApplication.restoreOverrideCursor()

    def toggleRuler(self, on):
        """
        TOWRITE

        :param `on`: TOWRITE
        :type `on`: bool
        """
        mainWin = self.mainWin
        qDebug("View toggleRuler()")
        QApplication.setOverrideCursor(Qt.WaitCursor)
        self.gscene.setProperty(ENABLE_RULER, on)
        self.rulerMetric = mainWin.getSettingsRulerMetric()
        self.rulerColor = QColor(mainWin.getSettingsRulerColor())
        self.rulerPixelSize = mainWin.getSettingsRulerPixelSize()
        self.gscene.update()
        QApplication.restoreOverrideCursor()

    def toggleOrtho(self, on):
        """
        TOWRITE

        :param `on`: TOWRITE
        :type `on`: bool
        """
        qDebug("View toggleOrtho()")
        QApplication.setOverrideCursor(Qt.WaitCursor)
        # TODO: finish this
        self.gscene.setProperty(ENABLE_ORTHO, on)
        self.gscene.update()
        QApplication.restoreOverrideCursor()

    def togglePolar(self, on):
        """
        TOWRITE

        :param `on`: TOWRITE
        :type `on`: bool
        """
        qDebug("View togglePolar()")
        QApplication.setOverrideCursor(Qt.WaitCursor)
        # TODO: finish this
        self.gscene.setProperty(ENABLE_POLAR, on)
        self.gscene.update()
        QApplication.restoreOverrideCursor()

    def toggleQSnap(self, on):
        """
        TOWRITE

        :param `on`: TOWRITE
        :type `on`: bool
        """
        qDebug("View toggleQSnap()")
        QApplication.setOverrideCursor(Qt.WaitCursor)
        self.qSnapToggle = on
        self.gscene.setProperty(ENABLE_QSNAP, on)
        self.gscene.update()
        QApplication.restoreOverrideCursor()

    def toggleQTrack(self, on):
        """
        TOWRITE

        :param `on`: TOWRITE
        :type `on`: bool
        """
        qDebug("View toggleQTrack()")
        QApplication.setOverrideCursor(Qt.WaitCursor)
        # TODO: finish this
        self.gscene.setProperty(ENABLE_QTRACK, on)
        self.gscene.update()
        QApplication.restoreOverrideCursor()

    def toggleLwt(self, on):
        """
        TOWRITE

        :param `on`: TOWRITE
        :type `on`: bool
        """
        qDebug("View toggleLwt()");
        QApplication.setOverrideCursor(Qt.WaitCursor)
        self.gscene.setProperty(ENABLE_LWT, on)
        self.gscene.update()
        QApplication.restoreOverrideCursor()

    def toggleReal(self, on):
        """
        TOWRITE

        :param `on`: TOWRITE
        :type `on`: bool
        """
        qDebug("View toggleReal()")
        QApplication.setOverrideCursor(Qt.WaitCursor)
        self.gscene.setProperty(ENABLE_REAL, on)
        self.gscene.update()
        QApplication.restoreOverrideCursor()

    def isLwtEnabled(self):
        """
        TOWRITE

        :rtype: bool
        """
        return self.gscene.property(ENABLE_LWT).toBool()

    def isRealEnabled(self):
        """
        TOWRITE

        :rtype: bool
        """
        return self.gscene.property(ENABLE_REAL).toBool()

    def drawBackground(self, painter, rect):
        """
        TOWRITE

        :param `painter`: TOWRITE
        :type `painter`: `QPainter`_
        :param `rect`: TOWRITE
        :type `rect`: `QRectF`_
        """
        painter.fillRect(rect, backgroundBrush())  #TODO/PORT# backgroundBrush?

        if self.gscene.property(ENABLE_GRID).toBool() and rect.intersects(self.gridPath.controlPointRect()):
            gridPen = QPen(self.gridColor)
            gridPen.setJoinStyle(Qt.MiterJoin)
            gridPen.setCosmetic(True)
            painter.setPen(gridPen)
            painter.drawPath(self.gridPath)
            painter.drawPath(self.originPath)
            painter.fillPath(self.originPath, self.gridColor)

    def drawForeground(self, painter, rect):
        """
        TOWRITE

        :param `painter`: TOWRITE
        :type `painter`: `QPainter`_
        :param `rect`: TOWRITE
        :type `rect`: `QRectF`_

        .. TODO:: port this def into python.

        """
        # ==================================================
        # Draw grip points for all selected objects
        # ==================================================

        gripPen = QPen(QColor.fromRgb(self.gripColorCool))
        gripPen.setWidth(2)
        gripPen.setJoinStyle(Qt.MiterJoin)
        gripPen.setCosmetic(True)
        painter.setPen(gripPen)
        gripOffset = QPoint(gripSize, gripSize)

        selectedGripPoints = QList() # QList<QPointF> selectedGripPoints
        selectedItemList = gscene.selectedItems() # QList<QGraphicsItem*>
        if selectedItemList.size() <= 100:
            for item in selectedItemList: # foreach(QGraphicsItem* item, selectedItemList)
                if item.type() >= OBJ_TYPE_BASE:
                    tempBaseObj = item # tempBaseObj = static_cast<BaseObject*>(item);
                    if tempBaseObj:
                        selectedGripPoints = tempBaseObj.allGripPoints()

                    for ssp in selectedGripPoints: # foreach(QPointF ssp, selectedGripPoints)
                        p1 = mapFromScene(ssp) - gripOffset  # QPoint
                        q1 = mapFromScene(ssp) + gripOffset  # QPoint

                        if ssp == sceneGripPoint:
                            painter.fillRect(QRectF(self.mapToScene(p1), self.mapToScene(q1)), QColor.fromRgb(self.gripColorHot))
                        else:
                            painter.drawRect(QRectF(self.mapToScene(p1), self.mapToScene(q1)))

        # ==================================================
        # Draw the closest qsnap point
        # ==================================================

        if not selectingActive: # TODO: and findClosestSnapPoint == True

            qsnapPen = QPen(QColor.fromRgb(qsnapLocatorColor))
            qsnapPen.setWidth(2)
            qsnapPen.setJoinStyle(Qt.MiterJoin)
            qsnapPen.setCosmetic(True)
            painter.setPen(qsnapPen)
            qsnapOffset = QPoint(qsnapLocatorSize, qsnapLocatorSize)

            apertureSnapPoints = QList # QList<QPointF> apertureSnapPoints;
            apertureItemList = items(viewMousePoint.x()-qsnapApertureSize,
                                     viewMousePoint.y()-qsnapApertureSize,
                                     qsnapApertureSize*2,
                                     qsnapApertureSize*2)  # QList<QGraphicsItem *>
            for item in apertureItemList: # foreach(QGraphicsItem* item, apertureItemList)

                if item.type() >= OBJ_TYPE_BASE:

                    tempBaseObj = item # tempBaseObj = static_cast<BaseObject*>(item);
                    if tempBaseObj:
                        apertureSnapPoints << tempBaseObj.mouseSnapPoint(sceneMousePoint)

            # TODO: Check for intersection snap points and add them to the list
            for asp in apertureSnapPoints: # foreach(QPointF asp, apertureSnapPoints)
                p1 = mapFromScene(asp) - qsnapOffset  # QPoint
                q1 = mapFromScene(asp) + qsnapOffset  # QPoint
                painter.drawRect(QRectF(self.mapToScene(p1), self.mapToScene(q1)))

        # ==================================================
        # Draw horizontal and vertical rulers
        # ==================================================

        if self.gscene.property(ENABLE_RULER).toBool():

            proceed = True # bool

            vw = width()   # int # View Width
            vh = height()  # int # View Height
            origin = self.mapToScene(0, 0)                   #QPointF
            rulerHoriz = self.mapToScene(vw, rulerPixelSize) #QPointF
            rulerVert  = self.mapToScene(rulerPixelSize, vh) #QPointF

            ox = origin.x()        # qreal
            oy = origin.y()        # qreal

            rhx = rulerHoriz.x()   # qreal
            rhy = rulerHoriz.y()   # qreal
            rhw = rhx - ox         # qreal
            rhh = rhy - oy         # qreal

            rvx = rulerVert.x()    # qreal
            rvy = rulerVert.y()    # qreal
            rvw = rvx - ox         # qreal
            rvh = rvy - oy         # qreal

            # NOTE: Drawing ruler if zoomed out too far will cause an assertion failure.
            #       We will limit the maximum size the ruler can be shown at.
            maxSize = -1  # quint16 # Intentional underflow
            if (rhw >= maxSize or rvh >= maxSize):
                proceed = False

            if proceed:

                distance = self.mapToScene(rulerPixelSize*3, 0).x() - ox  # int
                distStr = QString().setNum(distance)  # QString
                distStrSize = distStr.size()  # int
                msd = distStr.at(0).digitValue()  # int # Most Significant Digit

                if msd != -1:

                    #TODO/PORT# msd++
                    if msd == 10:

                        msd = 1
                        distStr.resize(distStrSize+1)
                        #TODO/PORT# distStrSize++

                    distStr.replace(0, 1, QString().setNum(msd))

                    for i in range(1, distStrSize): # for(int i = 1; i < distStrSize; ++i)
                        distStr.replace(i, 1, '0')

                    unit = distStr.toInt() # int
                    #TODO/PORT# fraction # qreal
                    feet = True  # bool
                    if rulerMetric:

                        if unit < 10:
                            unit = 10
                        fraction = unit / 10

                    else:
                        if unit <= 1:
                            unit = 1
                            feet = False
                            #TODO/PORT# fraction = (qreal)(unit/16)

                        else:
                            unit = roundToMultiple(True, unit, 12)
                            fraction = unit/12

                    little  = 0.20                                  # qreal
                    medium = 0.40                                   # qreal
                    rhTextOffset = self.mapToScene(3, 0).x() - ox   # qreal
                    rvTextOffset = self.mapToScene(0, 3).y() - oy   # qreal
                    textHeight = rhh*medium                         # qreal

                    lines = QVector()  # QVector<QLineF> lines
                    lines.append(QLineF(ox, rhy, rhx, rhy))
                    lines.append(QLineF(rvx, oy, rvx, rvy))

                    mx = sceneMousePoint.x()  # qreal
                    my = sceneMousePoint.y()  # qreal
                    lines.append(QLineF(mx, rhy, mx, oy))
                    lines.append(QLineF(rvx, my, ox, my))

                    transform = QTransform() # QTransform transform;

                    rulerPen = QPen(QColor(0, 0, 0))
                    rulerPen.setCosmetic(True)
                    painter.setPen(rulerPen)
                    painter.fillRect(QRectF(ox, oy, rhw, rhh), rulerColor)
                    painter.fillRect(QRectF(ox, oy, rvw, rvh), rulerColor)

                    # int xFlow;
                    if willUnderflowInt32(ox, unit):
                        proceed = False
                    else:
                        xFlow = roundToMultiple(False, ox, unit)
                    # int xStart;
                    if willUnderflowInt32(xFlow, unit):
                        proceed = False
                    else:
                        xStart = xFlow - unit
                    # int yFlow;
                    if willUnderflowInt32(oy, unit):
                        proceed = False
                    else:
                        yFlow = roundToMultiple(False, oy, unit)
                    # int yStart;
                    if willUnderflowInt32(yFlow, unit):
                        proceed = False
                    else:
                        yStart = yFlow - unit

                    if proceed:

                        for x in range(xStart, rhx, unit): # for(int x = xStart; x < rhx; x += unit)

                            transform.translate(x+rhTextOffset, rhy-rhh/2)
                            rulerTextPath = QPainterPath() # QPainterPath rulerTextPath;
                            if rulerMetric:
                                rulerTextPath = transform.map(createRulerTextPath(0, 0, QString().setNum(x), textHeight))

                            else:
                                if feet:
                                    rulerTextPath = transform.map(createRulerTextPath(0, 0, QString().setNum(x/12).append('\''), textHeight))
                                else:
                                    rulerTextPath = transform.map(createRulerTextPath(0, 0, QString().setNum(x).append('\"'), textHeight))

                            transform.reset()
                            painter.drawPath(rulerTextPath)

                            lines.append(QLineF(x, rhy, x, oy))
                            if rulerMetric:

                                lines.append(QLineF(x, rhy, x, oy))
                                lines.append(QLineF(x+fraction  , rhy, x+fraction,   rhy-rhh*little))
                                lines.append(QLineF(x+fraction*2, rhy, x+fraction*2, rhy-rhh*little))
                                lines.append(QLineF(x+fraction*3, rhy, x+fraction*3, rhy-rhh*little))
                                lines.append(QLineF(x+fraction*4, rhy, x+fraction*4, rhy-rhh*little))
                                lines.append(QLineF(x+fraction*5, rhy, x+fraction*5, rhy-rhh*medium))  # Half
                                lines.append(QLineF(x+fraction*6, rhy, x+fraction*6, rhy-rhh*little))
                                lines.append(QLineF(x+fraction*7, rhy, x+fraction*7, rhy-rhh*little))
                                lines.append(QLineF(x+fraction*8, rhy, x+fraction*8, rhy-rhh*little))
                                lines.append(QLineF(x+fraction*9, rhy, x+fraction*9, rhy-rhh*little))

                            else:

                                if feet:

                                    for i in range(0, 12): # for(int i = 0; i < 12; ++i)

                                        lines.append(QLineF(x+fraction*i, rhy, x+fraction*i, rhy-rhh*medium))

                                else:

                                    lines.append(QLineF(x+fraction   , rhy, x+fraction,    rhy-rhh*little))
                                    lines.append(QLineF(x+fraction* 2, rhy, x+fraction* 2, rhy-rhh*little))
                                    lines.append(QLineF(x+fraction* 3, rhy, x+fraction* 3, rhy-rhh*little))
                                    lines.append(QLineF(x+fraction* 4, rhy, x+fraction* 4, rhy-rhh*medium))  # Quarter
                                    lines.append(QLineF(x+fraction* 5, rhy, x+fraction* 5, rhy-rhh*little))
                                    lines.append(QLineF(x+fraction* 6, rhy, x+fraction* 6, rhy-rhh*little))
                                    lines.append(QLineF(x+fraction* 7, rhy, x+fraction* 7, rhy-rhh*little))
                                    lines.append(QLineF(x+fraction* 8, rhy, x+fraction* 8, rhy-rhh*medium))  # Half
                                    lines.append(QLineF(x+fraction* 9, rhy, x+fraction* 9, rhy-rhh*little))
                                    lines.append(QLineF(x+fraction*10, rhy, x+fraction*10, rhy-rhh*little))
                                    lines.append(QLineF(x+fraction*11, rhy, x+fraction*11, rhy-rhh*little))
                                    lines.append(QLineF(x+fraction*12, rhy, x+fraction*12, rhy-rhh*medium))  # Quarter
                                    lines.append(QLineF(x+fraction*13, rhy, x+fraction*13, rhy-rhh*little))
                                    lines.append(QLineF(x+fraction*14, rhy, x+fraction*14, rhy-rhh*little))
                                    lines.append(QLineF(x+fraction*15, rhy, x+fraction*15, rhy-rhh*little))

                        for y in range(yStart, rvy, unit):# for(int y = yStart; y < rvy; y += unit)

                            transform.translate(rvx-rvw/2, y-rvTextOffset)
                            transform.rotate(-90)
                            rulerTextPath = QPainterPath() # QPainterPath rulerTextPath;
                            if rulerMetric:

                                rulerTextPath = transform.map(createRulerTextPath(0, 0, QString().setNum(-y), textHeight))

                            else:

                                if feet:
                                    rulerTextPath = transform.map(createRulerTextPath(0, 0, QString().setNum(-y/12).append('\''), textHeight))
                                else:
                                    rulerTextPath = transform.map(createRulerTextPath(0, 0, QString().setNum(-y).append('\"'), textHeight))

                            transform.reset()
                            painter.drawPath(rulerTextPath)

                            lines.append(QLineF(rvx, y, ox, y))
                            if rulerMetric:

                                lines.append(QLineF(rvx, y+fraction  , rvx-rvw*little, y+fraction))
                                lines.append(QLineF(rvx, y+fraction*2, rvx-rvw*little, y+fraction*2))
                                lines.append(QLineF(rvx, y+fraction*3, rvx-rvw*little, y+fraction*3))
                                lines.append(QLineF(rvx, y+fraction*4, rvx-rvw*little, y+fraction*4))
                                lines.append(QLineF(rvx, y+fraction*5, rvx-rvw*medium, y+fraction*5))  # Half
                                lines.append(QLineF(rvx, y+fraction*6, rvx-rvw*little, y+fraction*6))
                                lines.append(QLineF(rvx, y+fraction*7, rvx-rvw*little, y+fraction*7))
                                lines.append(QLineF(rvx, y+fraction*8, rvx-rvw*little, y+fraction*8))
                                lines.append(QLineF(rvx, y+fraction*9, rvx-rvw*little, y+fraction*9))

                            else:

                                if feet:

                                    for i in range(0, 12): # for(int i = 0; i < 12; ++i)

                                        lines.append(QLineF(rvx, y+fraction*i, rvx-rvw*medium, y+fraction*i))

                                else:

                                    lines.append(QLineF(rvx, y+fraction   , rvx-rvw*little, y+fraction))
                                    lines.append(QLineF(rvx, y+fraction* 2, rvx-rvw*little, y+fraction* 2))
                                    lines.append(QLineF(rvx, y+fraction* 3, rvx-rvw*little, y+fraction* 3))
                                    lines.append(QLineF(rvx, y+fraction* 4, rvx-rvw*medium, y+fraction* 4))  # Quarter
                                    lines.append(QLineF(rvx, y+fraction* 5, rvx-rvw*little, y+fraction* 5))
                                    lines.append(QLineF(rvx, y+fraction* 6, rvx-rvw*little, y+fraction* 6))
                                    lines.append(QLineF(rvx, y+fraction* 7, rvx-rvw*little, y+fraction* 7))
                                    lines.append(QLineF(rvx, y+fraction* 8, rvx-rvw*medium, y+fraction* 8))  # Half
                                    lines.append(QLineF(rvx, y+fraction* 9, rvx-rvw*little, y+fraction* 9))
                                    lines.append(QLineF(rvx, y+fraction*10, rvx-rvw*little, y+fraction*10))
                                    lines.append(QLineF(rvx, y+fraction*11, rvx-rvw*little, y+fraction*11))
                                    lines.append(QLineF(rvx, y+fraction*12, rvx-rvw*medium, y+fraction*12))  # Quarter
                                    lines.append(QLineF(rvx, y+fraction*13, rvx-rvw*little, y+fraction*13))
                                    lines.append(QLineF(rvx, y+fraction*14, rvx-rvw*little, y+fraction*14))
                                    lines.append(QLineF(rvx, y+fraction*15, rvx-rvw*little, y+fraction*15))



                    painter.drawLines(lines)
                    painter.fillRect(QRectF(ox, oy, rvw, rhh), rulerColor)


        # ==================================================
        # Draw the crosshair
        # ==================================================

        if not selectingActive:

            #painter.setBrush(Qt.NoBrush)
            crosshairPen = QPen(QColor.fromRgb(self.crosshairColor))
            crosshairPen.setCosmetic(True)
            painter.setPen(crosshairPen)
            painter.drawLine(QLineF(self.mapToScene(viewMousePoint.x(), viewMousePoint.y()-crosshairSize),
                                    self.mapToScene(viewMousePoint.x(), viewMousePoint.y()+crosshairSize)))
            painter.drawLine(QLineF(self.mapToScene(viewMousePoint.x()-crosshairSize, viewMousePoint.y()),
                                    self.mapToScene(viewMousePoint.x()+crosshairSize, viewMousePoint.y())))
            painter.drawRect(QRectF(self.mapToScene(viewMousePoint.x()-pickBoxSize, viewMousePoint.y()-pickBoxSize),
                                    self.mapToScene(viewMousePoint.x()+pickBoxSize, viewMousePoint.y()+pickBoxSize)))


    def willUnderflowInt32(self, a, b):
        """
        TOWRITE

        :param `a`: TOWRITE
        :type `a`: qint64
        :param `b`: TOWRITE
        :type `b`: qint64
        :rtype: bool

        .. TODO:: port this def into python.

        """
        #TODO/PORT# qint64 c
        #TODO/PORT# Q_ASSERT(LLONG_MAX>INT_MAX)
        #TODO/PORT# c = (qint64)a-b
        #TODO/PORT# if c < INT_MIN or c > INT_MAX:
        #TODO/PORT#     return True
        return False

    def willOverflowInt32(self, a, b):
        """
        TOWRITE

        :param `a`: TOWRITE
        :type `a`: qint64
        :param `b`: TOWRITE
        :type `b`: qint64
        :rtype: bool

        .. TODO:: port this def into python.

        """
        #TODO/PORT# qint64 c
        #TODO/PORT# Q_ASSERT(LLONG_MAX>INT_MAX)
        #TODO/PORT# c = (qint64)a+b
        #TODO/PORT# if c < INT_MIN or c > INT_MAX:
        #TODO/PORT#     return True
        return False

    def createRulerTextPath(self, x, y, strng, height):
        """
        TOWRITE

        :param `x`: TOWRITE
        :type `x`: float
        :param `y`: TOWRITE
        :type `y`: float
        :param `strng`: TOWRITE
        :type `strng`: QString
        :param `height`: TOWRITE
        :type `height`: float
        :rtype: `QPainterPath`_
        """
        path = QPainterPath()

        xScale = height  # qreal
        yScale = height  # qreal

        strLen = len(strng) # int len = strng.length()
        for i in range(0, strLen): # for(int i = 0; i < len; ++i)

            if (strng[i] == QChar('1')):
                path.moveTo(x+0.05*xScale, y-0.00*yScale)
                path.lineTo(x+0.45*xScale, y-0.00*yScale)
                path.moveTo(x+0.00*xScale, y-0.75*yScale)
                path.lineTo(x+0.25*xScale, y-1.00*yScale)
                path.lineTo(x+0.25*xScale, y-0.00*yScale)

            elif (strng[i] == QChar('2')):
                path.moveTo(x+0.00*xScale, y-0.75*yScale)
                path.arcTo(x+0.00*xScale, y-1.00*yScale, 0.50*xScale, 0.50*yScale, 180.00, -216.87)
                path.lineTo(x+0.00*xScale, y-0.00*yScale)
                path.lineTo(x+0.50*xScale, y-0.00*yScale)

            elif (strng[i] == QChar('3')):
                path.arcMoveTo(x+0.00*xScale, y-0.50*yScale, 0.50*xScale, 0.50*yScale, 195.00)
                path.arcTo(x+0.00*xScale, y-0.50*yScale, 0.50*xScale, 0.50*yScale, 195.00, 255.00)
                path.arcTo(x+0.00*xScale, y-1.00*yScale, 0.50*xScale, 0.50*yScale, 270.00, 255.00)

            elif (strng[i] == QChar('4')):
                path.moveTo(x+0.50*xScale, y-0.00*yScale)
                path.lineTo(x+0.50*xScale, y-1.00*yScale)
                path.lineTo(x+0.00*xScale, y-0.50*yScale)
                path.lineTo(x+0.50*xScale, y-0.50*yScale)

            elif (strng[i] == QChar('5')):
                path.moveTo(x+0.50*xScale, y-1.00*yScale)
                path.lineTo(x+0.00*xScale, y-1.00*yScale)
                path.lineTo(x+0.00*xScale, y-0.50*yScale)
                path.lineTo(x+0.25*xScale, y-0.50*yScale)
                path.arcTo(x+0.00*xScale, y-0.50*yScale, 0.50*xScale, 0.50*yScale, 90.00, -180.00)
                path.lineTo(x+0.00*xScale, y-0.00*yScale)

            elif (strng[i] == QChar('6')):
                path.addEllipse(QPointF(x+0.25*xScale, y-0.25*yScale), 0.25*xScale, 0.25*yScale)
                path.moveTo(x+0.00*xScale, y-0.25*yScale)
                path.lineTo(x+0.00*xScale, y-0.75*yScale)
                path.arcTo(x+0.00*xScale, y-1.00*yScale, 0.50*xScale, 0.50*yScale, 180.00, -140.00)

            elif (strng[i] == QChar('7')):
                path.moveTo(x+0.00*xScale, y-1.00*yScale)
                path.lineTo(x+0.50*xScale, y-1.00*yScale)
                path.lineTo(x+0.25*xScale, y-0.25*yScale)
                path.lineTo(x+0.25*xScale, y-0.00*yScale)

            elif (strng[i] == QChar('8')):
                path.addEllipse(QPointF(x+0.25*xScale, y-0.25*yScale), 0.25*xScale, 0.25*yScale)
                path.addEllipse(QPointF(x+0.25*xScale, y-0.75*yScale), 0.25*xScale, 0.25*yScale)

            elif (strng[i] == QChar('9')):
                path.addEllipse(QPointF(x+0.25*xScale, y-0.75*yScale), 0.25*xScale, 0.25*yScale)
                path.moveTo(x+0.50*xScale, y-0.75*yScale)
                path.lineTo(x+0.50*xScale, y-0.25*yScale)
                path.arcTo(x+0.00*xScale, y-0.50*yScale, 0.50*xScale, 0.50*yScale, 0.00, -140.00)

            elif (strng[i] == QChar('0')):
                #path.addEllipse(QPointF(x+0.25*xScale, y-0.50*yScale), 0.25*xScale, 0.50*yScale)

                path.moveTo(x+0.00*xScale, y-0.75*yScale)
                path.lineTo(x+0.00*xScale, y-0.25*yScale)
                path.arcTo(x+0.00*xScale, y-0.50*yScale, 0.50*xScale, 0.50*yScale, 180.00, 180.00)
                path.lineTo(x+0.50*xScale, y-0.75*yScale)
                path.arcTo(x+0.00*xScale, y-1.00*yScale, 0.50*xScale, 0.50*yScale,   0.00, 180.00)

            elif (strng[i] == QChar('-')):
                path.moveTo(x+0.00*xScale, y-0.50*yScale)
                path.lineTo(x+0.50*xScale, y-0.50*yScale)

            elif (strng[i] == QChar('\'')):
                path.moveTo(x+0.25*xScale, y-1.00*yScale)
                path.lineTo(x+0.25*xScale, y-0.75*yScale)

            elif (strng[i] == QChar('\"')):
                path.moveTo(x+0.10*xScale, y-1.00*yScale)
                path.lineTo(x+0.10*xScale, y-0.75*yScale)
                path.moveTo(x+0.40*xScale, y-1.00*yScale)
                path.lineTo(x+0.40*xScale, y-0.75*yScale)

            x += 0.75*xScale

        return path

    def roundToMultiple(self, roundUp, numToRound, multiple):
        """
        TOWRITE

        :param `roundUp`: TOWRITE
        :type `roundUp`: bool
        :param `numToRound`: TOWRITE
        :type `numToRound`: int
        :param `multiple`: TOWRITE
        :type `multiple`: int
        :rtype: int
        """
        if multiple == 0:
            return numToRound
        remainder = numToRound % multiple # int
        if remainder == 0:
            return numToRound

        if numToRound < 0 and roundUp:
            return numToRound - remainder
        if roundUp:
            return numToRound + multiple - remainder
        #else round down
        if numToRound < 0 and not roundUp:
            return numToRound - multiple - remainder
        return numToRound - remainder

    def updateMouseCoords(self, x, y):
        """
        TOWRITE

        :param `x`: TOWRITE
        :type `x`: int
        :param `y`: TOWRITE
        :type `y`: int
        """
        viewMousePoint = QPoint(x, y)
        sceneMousePoint = self.mapToScene(viewMousePoint)
        self.gscene.setProperty(SCENE_QSNAP_POINT, sceneMousePoint) # TODO: if qsnap functionality is enabled, use it rather than the mouse point
        self.gscene.setProperty(SCENE_MOUSE_POINT, sceneMousePoint)
        self.gscene.setProperty(VIEW_MOUSE_POINT, viewMousePoint)
        self.mainWin.statusbar.setMouseCoord(sceneMousePoint.x(), -sceneMousePoint.y())

    def setCrossHairSize(self, percent):
        """
        TOWRITE

        :param `percent`: TOWRITE
        :type `percent`: quint8
        """
        # NOTE: crosshairSize is in pixels and is a percentage of your screen width
        # NOTE: Example: (1280*0.05)/2 = 32, thus 32 + 1 + 32 = 65 pixel wide crosshair
        screenWidth = qApp.desktop().width() # quint32
        if percent > 0 and percent < 100:
            self.crosshairSize = (screenWidth * (percent / 100.0)) / 2
        else:
            self.crosshairSize = screenWidth

    def setCornerButton(self):
        """
        TOWRITE
        """
        num = mainWin.getSettingsDisplayScrollBarWidgetNum() # int
        if num:

            cornerButton = QPushButton(self)
            cornerButton.setFlat(True)
            act = self.mainWin.actionHash.value(num) # QAction*
            # NOTE: Prevent crashing if the action is NULL.
            if not act:
                QMessageBox.information(self, self.tr("Corner Widget Error"), self.tr("There are unused enum values in COMMAND_ACTIONS. Please report this as a bug."))
                self.setCornerWidget(0)
            else:
                cornerButton.setIcon(act.icon())
                #TODO/PORT# connect(cornerButton, SIGNAL(clicked()), this, SLOT(cornerButtonClicked()))
                self.setCornerWidget(cornerButton)
                cornerButton.setCursor(Qt.ArrowCursor)

        else:
            self.setCornerWidget(0)

    def cornerButtonClicked(self):
        """
        TOWRITE
        """
        qDebug("Corner Button Clicked.")
        self.mainWin.actionHash.value(self.mainWin.getSettingsDisplayScrollBarWidgetNum()).trigger()

    def zoomIn(self):
        """
        TOWRITE
        """
        qDebug("View zoomIn()")
        if not allowZoomIn():
            return
        QApplication.setOverrideCursor(Qt.WaitCursor)
        cntr = self.mapToScene(QPoint(width() / 2, height() / 2)) # QPointF
        s = self.mainWin.getSettingsDisplayZoomScaleIn() # qreal
        self.scale(s, s)

        self.centerOn(cntr)
        QApplication.restoreOverrideCursor()

    def zoomOut(self):
        """
        TOWRITE
        """
        qDebug("View zoomOut()")
        if not allowZoomOut():
            return
        QApplication.setOverrideCursor(Qt.WaitCursor)
        cntr = self.mapToScene(QPoint(width() / 2, height() / 2)) # QPointF
        s = self.mainWin.getSettingsDisplayZoomScaleOut() # qreal
        self.scale(s, s)

        self.centerOn(cntr)
        QApplication.restoreOverrideCursor()

    def zoomWindow(self):
        """
        TOWRITE
        """
        self.zoomWindowActive = True
        self.selectingActive = False
        self.clearSelection()

    def zoomSelected(self):
        """
        TOWRITE
        """
        QApplication.setOverrideCursor(Qt.WaitCursor);
        itemList = self.gscene.selectedItems() # QList<QGraphicsItem*>
        selectedRectPath = QPainterPath()
        for item in itemList: # foreach(QGraphicsItem* item, itemList)

            selectedRectPath.addPolygon(item.self.mapToScene(item.boundingRect()))

        selectedRect = selectedRectPath.boundingRect() # QRectF
        if selectedRect.isNull():

            QMessageBox.information(self, self.tr("ZoomSelected Preselect"), self.tr("Preselect objects before invoking the zoomSelected command."))
            # TODO: Support Post selection of objects

        self.fitInView(selectedRect, Qt.KeepAspectRatio)
        QApplication.restoreOverrideCursor()

    def zoomExtents(self):
        """
        TOWRITE
        """
        QApplication.setOverrideCursor(Qt.WaitCursor)
        extents = self.gscene.itemsBoundingRect()  # QRectF
        if extents.isNull():
            extents.setWidth(self.mainWin.getSettingsGridSizeX())
            extents.setHeight(self.mainWin.getSettingsGridSizeY())
            extents.moveCenter(QPointF(0,0))

        self.fitInView(extents, Qt.KeepAspectRatio)
        QApplication.restoreOverrideCursor()

    def panRealTime(self):
        """
        TOWRITE
        """
        self.panningRealTimeActive = True

    def panPoint(self):
        """
        TOWRITE
        """
        self.panningPointActive = True

    def panLeft(self):
        """
        TOWRITE
        """
        self.horizontalScrollBar().setValue(self.horizontalScrollBar().value() + self.panDistance)
        self.updateMouseCoords(self.viewMousePoint.x(), self.viewMousePoint.y())
        self.gscene.update()

    def panRight(self):
        """
        TOWRITE
        """
        self.horizontalScrollBar().setValue(self.horizontalScrollBar().value() - self.panDistance)
        self.updateMouseCoords(self.viewMousePoint.x(), self.viewMousePoint.y())
        self.gscene.update()

    def panUp(self):
        """
        TOWRITE
        """
        self.verticalScrollBar().setValue(self.verticalScrollBar().value() + self.panDistance)
        self.updateMouseCoords(self.viewMousePoint.x(), self.viewMousePoint.y())
        self.gscene.update()

    def panDown(self):
        """
        TOWRITE
        """
        self.verticalScrollBar().setValue(self.verticalScrollBar().value() - self.panDistance)
        self.updateMouseCoords(self.viewMousePoint.x(), self.viewMousePoint.y())
        self.gscene.update()

    def selectAll(self):
        """
        TOWRITE
        """
        allPath = QPainterPath()
        allPath.addRect(self.gscene.sceneRect())
        self.gscene.setSelectionArea(allPath, Qt.IntersectsItemShape, self.transform())

    def selectionChanged(self):
        """
        TOWRITE
        """
        if self.mainWin.dockPropEdit.isVisible():
            self.mainWin.dockPropEdit.setSelectedItems(self.gscene.selectedItems())

    def mouseDoubleClickEvent(self, event):
        """
        Handles the ``mouseDoubleClickEvent`` event for :class:`View`.

        :param `event`: A `QMouseEvent`_ to be processed.
        """
        if event.button() == Qt.LeftButton:
            item = self.gscene.itemAt(self.mapToScene(event.pos()), QTransform()) # QGraphicsItem*
            if item:
                self.mainWin.dockPropEdit.show()

    def mousePressEvent(self, event):
        """
        Handles the ``mousePressEvent`` event for :class:`View`.

        :param `event`: A `QMouseEvent`_ to be processed.
        """
        mainWin = self.mainWin
        gscene = self.gscene

        self.updateMouseCoords(event.x(), event.y())
        if event.button() == Qt.LeftButton:

            if mainWin.isCommandActive():

                cmdPoint = self.mapToScene(event.pos()) # QPointF
                mainWin.runCommandClick(mainWin.activeCommand(), cmdPoint.x(), cmdPoint.y())
                return

            path = QPainterPath()
            pickList = gscene.items(QRectF(self.mapToScene(viewMousePoint.x()-pickBoxSize, viewMousePoint.y()-pickBoxSize),
                                           self.mapToScene(viewMousePoint.x()+pickBoxSize, viewMousePoint.y()+pickBoxSize)))  # QList<QGraphicsItem*>

            itemsInPickBox = pickList.size() # bool
            if itemsInPickBox and not selectingActive and not grippingActive:

                itemsAlreadySelected = pickList.at(0).isSelected() # bool
                if not itemsAlreadySelected:

                    pickList.at(0).setSelected(True)

                else:

                    foundGrip = False
                    base = pickList.at(0)  # BaseObject* base = static_cast<BaseObject*>(pickList.at(0)); # TODO: Allow multiple objects to be gripped at once
                    if not base:
                        return

                    qsnapOffset = QPoint(qsnapLocatorSize, qsnapLocatorSize)
                    gripPoint = base.mouseSnapPoint(sceneMousePoint) # QPointF
                    p1 = self.mapFromScene(gripPoint) - qsnapOffset # QPoint
                    q1 = self.mapFromScene(gripPoint) + qsnapOffset # QPoint
                    gripRect = QRectF(self.mapToScene(p1), self.mapToScene(q1)) # QRectF
                    pickRect = QRectF(self.mapToScene(viewMousePoint.x()-pickBoxSize, viewMousePoint.y()-pickBoxSize),
                                      self.mapToScene(viewMousePoint.x()+pickBoxSize, viewMousePoint.y()+pickBoxSize)) # QRectF
                    if gripRect.intersects(pickRect):
                        foundGrip = True

                    # If the pick point is within the item's grip box, start gripping
                    if foundGrip:
                        startGripping(base)

                    else: # start moving
                        movingActive = True
                        pressPoint = event.pos()
                        scenePressPoint = self.mapToScene(pressPoint)

            elif grippingActive:
                stopGripping(True)

            elif not selectingActive:
                selectingActive = True
                pressPoint = event.pos()
                scenePressPoint = self.mapToScene(pressPoint)

                if not selectBox:
                    selectBox = SelectBox(QRubberBand.Rectangle, self)
                selectBox.setGeometry(QRect(pressPoint, pressPoint))
                selectBox.show()

            else:
                selectingActive = False
                selectBox.hide()
                releasePoint = event.pos()
                sceneReleasePoint = self.mapToScene(releasePoint)

                # Start SelectBox Code
                path.addPolygon(self.mapToScene(selectBox.geometry()))
                if sceneReleasePoint.x() > scenePressPoint.x():
                    if mainWin.getSettingsSelectionModePickAdd():

                        if mainWin.isShiftPressed():

                            itemList = gscene.items(path, Qt.ContainsItemShape) # QList<QGraphicsItem*>
                            for item in itemList: # foreach(QGraphicsItem* item, itemList)
                                item.setSelected(False)
                        else:
                            itemList = gscene.items(path, Qt.ContainsItemShape) # QList<QGraphicsItem*>
                            for item in itemList: # foreach(QGraphicsItem* item, itemList)
                                item.setSelected(True)

                    else:
                        if mainWin.isShiftPressed():

                            itemList = gscene.items(path, Qt.ContainsItemShape) # QList<QGraphicsItem*>
                            if not itemList.size():
                                clearSelection()
                            else:
                                for item in itemList: # foreach(QGraphicsItem* item, itemList)
                                    item.setSelected(not item.isSelected())  # Toggle selected

                        else:
                            clearSelection()
                            itemList = gscene.items(path, Qt.ContainsItemShape) # QList<QGraphicsItem*>
                            for item in itemList: # foreach(QGraphicsItem* item, itemList)
                                item.setSelected(True)

                else:
                    if mainWin.getSettingsSelectionModePickAdd():

                        if mainWin.isShiftPressed():
                            itemList = gscene.items(path, Qt.IntersectsItemShape) # QList<QGraphicsItem*>
                            for item in itemList: # foreach(QGraphicsItem* item, itemList)
                                item.setSelected(False)

                        else:
                            itemList = gscene.items(path, Qt.IntersectsItemShape) # QList<QGraphicsItem*>
                            for item in itemList: # foreach(QGraphicsItem* item, itemList)
                                item.setSelected(True)

                    else:
                        if self.mainWin.isShiftPressed():
                            itemList = gscene.items(path, Qt.IntersectsItemShape) # QList<QGraphicsItem*>
                            if not itemList.size():
                                clearSelection()
                            else:
                                for item in itemList: # foreach(QGraphicsItem* item, itemList)
                                    item.setSelected(not item.isSelected()) # Toggle selected

                        else:
                            clearSelection();
                            itemList = gscene.items(path, Qt.IntersectsItemShape) # QList<QGraphicsItem*>
                            for item in itemList: # foreach(QGraphicsItem* item, itemList)
                                item.setSelected(True)

                # End SelectBox Code

            if pastingActive:
                itemList = pasteObjectItemGroup.childItems() # QList<QGraphicsItem*>
                gscene.destroyItemGroup(pasteObjectItemGroup)
                for item in itemList: # foreach(QGraphicsItem* item, itemList)
                    gscene.removeItem(item) # Prevent Qt Runtime Warning, QGraphicsScene::addItem: item has already been added to this scene

                undoStack.beginMacro("Paste")
                for item in itemList: # foreach(QGraphicsItem* item, itemList)
                    base = item #cpp# BaseObject* base = static_cast<BaseObject*>(item);
                    if base:
                        #TODO/PORT# UndoableAddCommand* cmd = new UndoableAddCommand(base->data(OBJ_NAME).toString(), base, this, 0);
                        if cmd:
                            undoStack.push(cmd)

                undoStack.endMacro()

                pastingActive = False
                selectingActive = False

            if zoomWindowActive:
                self.fitInView(path.boundingRect(), Qt.KeepAspectRatio)
                clearSelection()

        if event.button() == Qt.MidButton:
            panStart(event.pos())
            # The Undo command will record the spot where the pan started.
            #TODO/PORT# UndoableNavCommand* cmd = new UndoableNavCommand("PanStart", this, 0)
            undoStack.push(cmd)
            event.accept()

        self.gscene.update()

    def panStart(self, point):
        """
        TOWRITE

        :param `point`: TOWRITE
        :type `point`: QPoint
        """
        self.recalculateLimits()

        self.alignScenePointWithViewPoint(self.mapToScene(point), point)

        self.panningActive = True
        self.panStartX = point.x()
        self.panStartY = point.y()

    def recalculateLimits(self):
        """
        TOWRITE
        """
        # NOTE: Increase the sceneRect limits if the point we want to go to lies outside of sceneRect's limits
        #       If the sceneRect limits aren't increased, you cannot pan past its limits
        viewRect = QRectF(self.mapToScene(self.rect().topLeft()), self.mapToScene(self.rect().bottomRight()))
        sceneRect = QRectF(self.gscene.sceneRect())
        newRect = viewRect.adjusted(-viewRect.width(), -viewRect.height(), viewRect.width(), viewRect.height()) # QRectF
        if (not sceneRect.contains(newRect.topLeft()) or not sceneRect.contains(newRect.bottomRight())):
            self.gscene.setSceneRect(self.sceneRect.adjusted(-viewRect.width(),
                                                             -viewRect.height(),
                                                             viewRect.width(),
                                                             viewRect.height()))

    def centerAt(self, centerPoint):
        """
        TOWRITE

        :param `centerPoint`: TOWRITE
        :type `centerPoint`: QPointF
        """
        # centerOn also updates the scrollbars, which shifts things out of wack o_O
        self.centerOn(centerPoint)
        # Reshift to the new center
        offset = centerPoint - center()  # QPointF
        newCenter = centerPoint + offset # QPointF
        self.centerOn(newCenter)

    def alignScenePointWithViewPoint(self, scenePoint, viewPoint):
        """
        TOWRITE

        :param `scenePoint`: TOWRITE
        :type `scenePoint`: QPointF
        :param `viewPoint`: TOWRITE
        :type `viewPoint`: QPoint
        """
        viewCenter = center()     # QPointF
        pointBefore = scenePoint  # QPointF
        # centerOn also updates the scrollbars, which shifts things out of wack o_O
        self.centerOn(viewCenter)
        # Reshift to the new center so the scene and view points align
        pointAfter = self.mapToScene(viewPoint)   # QPointF
        offset = pointBefore - pointAfter         # QPointF
        newCenter = viewCenter + offset           # QPointF
        self.centerOn(newCenter)

    def mouseMoveEvent(self, event):
        """
        Handles the ``mouseMoveEvent`` event for :class:`View`.

        :param `event`: A `QMouseEvent`_ to be processed.
        """
        self.updateMouseCoords(event.x(), event.y())
        movePoint = event.pos()
        sceneMovePoint = self.mapToScene(movePoint)

        mainWin = self.mainWin
        if mainWin.isCommandActive():
            if rapidMoveActive:
                mainWin.runCommandMove(mainWin.activeCommand(), sceneMovePoint.x(), sceneMovePoint.y())

        if previewActive:

            if previewMode == PREVIEW_MODE_MOVE:
                previewObjectItemGroup.setPos(sceneMousePoint - previewPoint)

            elif previewMode == PREVIEW_MODE_ROTATE:
                x = previewPoint.x()  # qreal
                y = previewPoint.y()  # qreal
                rot = previewData     # qreal

                mouseAngle = QLineF(x, y, sceneMousePoint.x(), sceneMousePoint.y()).angle() # qreal

                rad = radians(rot-mouseAngle)  # qreal
                cosRot = qCos(rad)             # qreal
                sinRot = qSin(rad)             # qreal
                px = 0                         # qreal
                py = 0                         # qreal
                px -= x
                py -= y
                rotX = px * cosRot - py * sinRot  # qreal
                rotY = px * sinRot + py * cosRot  # qreal
                rotX += x
                rotY += y

                previewObjectItemGroup.setPos(rotX, rotY)
                previewObjectItemGroup.setRotation(rot-mouseAngle)

            elif previewMode == PREVIEW_MODE_SCALE:
                x = previewPoint.x()       # qreal
                y = previewPoint.y()       # qreal
                scaleFactor = previewData  # qreal

                factor = QLineF(x, y, sceneMousePoint.x(), sceneMousePoint.y()).length()/scaleFactor  # qreal

                previewObjectItemGroup.setScale(1)
                previewObjectItemGroup.setPos(0,0)

                if scaleFactor <= 0.0:
                    QMessageBox.critical(self, QObject.tr("ScaleFactor Error"),
                                        QObject.tr("Hi there. If you are not a developer, report this as a bug. "
                                        "If you are a developer, your code needs examined, and possibly your head too."))

                else:
                    # Calculate the offset
                    oldX = 0  # qreal
                    oldY = 0  # qreal
                    scaleLine = QLineF(x, y, oldX, oldY)
                    scaleLine.setLength(scaleLine.length()*factor)
                    newX = scaleLine.x2() # qreal
                    newY = scaleLine.y2() # qreal

                    dx = newX - oldX      # qreal
                    dy = newY - oldY      # qreal

                    previewObjectItemGroup.setScale(previewObjectItemGroup.scale()*factor)
                    previewObjectItemGroup.moveBy(dx, dy)

        if pastingActive:
            pasteObjectItemGroup.setPos(sceneMousePoint - pasteDelta);

        if movingActive:
            # Ensure that the preview is only shown if the mouse has moved.
            if not previewActive:
                previewOn(PREVIEW_CLONE_SELECTED, PREVIEW_MODE_MOVE, scenePressPoint.x(), scenePressPoint.y(), 0)

        if selectingActive:
            if sceneMovePoint.x() >= scenePressPoint.x():
                selectBox.setDirection(1)
            else:
                selectBox.setDirection(0)
            selectBox.setGeometry(QRect(self.mapFromScene(scenePressPoint), event.pos()).normalized())
            event.accept()

        if panningActive:
            self.horizontalScrollBar().setValue(self.horizontalScrollBar().value() - (event.x() - panStartX))
            self.verticalScrollBar().setValue(self.verticalScrollBar().value() - (event.y() - panStartY))
            panStartX = event.x()
            panStartY = event.y()
            event.accept()

        self.gscene.update()

    def mouseReleaseEvent(self, event):
        """
        Handles the ``mouseReleaseEvent`` event for :class:`View`.

        :param `event`: A `QMouseEvent`_ to be processed.
        """
        updateMouseCoords(event.x(), event.y())
        if event.button() == Qt.LeftButton:

            if movingActive:

                self.previewOff()
                dx = sceneMousePoint.x() - scenePressPoint.x() # qreal
                dy = sceneMousePoint.y() - scenePressPoint.y() # qreal
                # Ensure that moving only happens if the mouse has moved.
                if dx or dy:
                    moveSelected(dx, dy)
                movingActive = False

            event.accept()

        if event.button() == Qt.MidButton:

            panningActive = False
            # The Undo command will record the spot where the pan completed.
            #TODO/PORT# UndoableNavCommand* cmd = new UndoableNavCommand("PanStop", this, 0)
            undoStack.push(cmd)
            event.accept()

        if event.button() == Qt.XButton1:

            qDebug("XButton1")
            mainWin.undo()  # TODO: Make this customizable
            event.accept()

        if event.button() == Qt.XButton2:

            qDebug("XButton2")
            mainWin.redo()  # TODO: Make this customizable
            event.accept()

        gscene.update()

    def allowZoomIn(self):
        """
        TOWRITE

        :rtype: bool
        """
        origin  = self.mapToScene(0, 0)                         # QPointF
        corner  = self.mapToScene(self.width(), self.height())  # QPointF
        maxWidth  = corner.x() - origin.x()  # qreal
        maxHeight = corner.y() - origin.y()  # qreal

        zoomInLimit = 0.0000000001  # qreal
        if qMin(maxWidth, maxHeight) < zoomInLimit:
            qDebug("ZoomIn limit reached. (limit=%.10f)", zoomInLimit)
            return False

        return True

    def allowZoomOut(self):
        """
        TOWRITE

        :rtype: bool
        """
        origin  = self.mapToScene(0, 0)                         # QPointF
        corner  = self.mapToScene(self.width(), self.height())  # QPointF
        maxWidth  = corner.x() - origin.x()  # qreal
        maxHeight = corner.y() - origin.y()  # qreal

        zoomOutLimit = 10000000000000.0  # qreal
        if qMax(maxWidth, maxHeight) > zoomOutLimit:
            qDebug("ZoomOut limit reached. (limit=%.1f)", zoomOutLimit)
            return False

        return True

    def wheelEvent(self, event):
        """
        Handles the ``wheelEvent`` event for :class:`View`.

        :param `event`: A `QWheelEvent`_ to be processed.
        """
        zoomDir = event.delta() # int
        mousePoint = event.pos() # QPoint

        self.updateMouseCoords(mousePoint.x(), mousePoint.y())
        if zoomDir > 0:
            #TODO/PORT# UndoableNavCommand* cmd = new UndoableNavCommand("ZoomInToPoint", this, 0)
            undoStack.push(cmd)

        else:
            #TODO/PORT# UndoableNavCommand* cmd = new UndoableNavCommand("ZoomOutToPoint", this, 0)
            undoStack.push(cmd)

    def zoomToPoint(self, mousePoint, zoomDir):
        """
        TOWRITE

        :param `mousePoint`: TOWRITE
        :type `mousePoint`: QPoint
        :param `zoomDir`: TOWRITE
        :type `zoomDir`: int
        """
        pointBeforeScale = QPointF(self.mapToScene(mousePoint))

        # Do The zoom
        # qreal s;
        if zoomDir > 0:
            if not self.allowZoomIn():
                return
            s = mainWin.getSettingsDisplayZoomScaleIn()

        else:
            if not self.allowZoomOut():
                return
            s = mainWin.getSettingsDisplayZoomScaleOut()

        self.scale(s, s)
        self.alignScenePointWithViewPoint(pointBeforeScale, mousePoint)
        self.recalculateLimits()
        self.alignScenePointWithViewPoint(pointBeforeScale, mousePoint)

        self.updateMouseCoords(mousePoint.x(), mousePoint.y())
        if pastingActive:
            pasteObjectItemGroup.setPos(sceneMousePoint - pasteDelta)

        if selectingActive:
            selectBox.setGeometry(QRect(mapFromScene(scenePressPoint), mousePoint).normalized())

        self.gscene.update()

    def contextMenuEvent(self, event):
        """
        Handles the ``contextMenuEvent`` event for :class:`View`.

        :param `event`: A `QContextMenuEvent`_ to be processed.
        """
        iconTheme = mainWin.getSettingsGeneralIconTheme() # QString

        menu = QMenu()
        itemList = gscene.selectedItems() # QList<QGraphicsItem*>
        selectionEmpty = itemList.isEmpty() # bool

        for i in range(0, itemList.size()): # for(int i = 0; i < itemList.size(); i++)
            if itemList.at(i).data(OBJ_TYPE) != OBJ_TYPE_NULL:
                selectionEmpty = False
                break

        if pastingActive:
            return

        if not mainWin.prompt.isCommandActive():
            lastCmd = mainWin.prompt.lastCommand() # QString
            repeatAction = QAction(QIcon("icons/" + iconTheme + "/" + lastCmd + ".png"), "Repeat " + lastCmd, self)
            repeatAction.setStatusTip("Repeats the previously issued command.")
            #TODO/PORT# connect(repeatAction, SIGNAL(triggered()), this, SLOT(repeatAction()))
            menu.addAction(repeatAction)

        if zoomWindowActive:
            cancelZoomWinAction = QAction("&Cancel (ZoomWindow)", self)
            cancelZoomWinAction.setStatusTip("Cancels the ZoomWindow Command.")
            #TODO/PORT# connect(cancelZoomWinAction, SIGNAL(triggered()), this, SLOT(escapePressed()));
            menu.addAction(cancelZoomWinAction);

        menu.addSeparator()
        menu.addAction(mainWin.actionHash.value(ACTION_cut))
        menu.addAction(mainWin.actionHash.value(ACTION_copy))
        menu.addAction(mainWin.actionHash.value(ACTION_paste))
        menu.addSeparator()

        if not selectionEmpty:

            deleteAction = QAction(QIcon("icons/" + iconTheme + "/" + "erase" + ".png"), "D&elete", self)
            deleteAction.setStatusTip("Removes objects from a drawing.")
            #TODO/PORT# connect(deleteAction, SIGNAL(triggered()), this, SLOT(deleteSelected()))
            menu.addAction(deleteAction)

            moveAction = QAction(QIcon("icons/" + iconTheme + "/" + "move" + ".png"), "&Move", self)
            moveAction.setStatusTip("Displaces objects a specified distance in a specified direction.")
            #TODO/PORT# connect(moveAction, SIGNAL(triggered()), this, SLOT(moveAction()))
            menu.addAction(moveAction)

            scaleAction = QAction(QIcon("icons/" + iconTheme + "/" + "scale" + ".png"), "Sca&le", self)
            scaleAction.setStatusTip("Enlarges or reduces objects proportionally in the X, Y, and Z directions.")
            #TODO/PORT# connect(scaleAction, SIGNAL(triggered()), this, SLOT(scaleAction()))
            menu.addAction(scaleAction)

            rotateAction = QAction(QIcon("icons/" + iconTheme + "/" + "rotate" + ".png"), "R&otate", self)
            rotateAction.setStatusTip("Rotates objects about a base point.")
            #TODO/PORT# connect(rotateAction, SIGNAL(triggered()), this, SLOT(rotateAction()))
            menu.addAction(rotateAction)

            menu.addSeparator()

            clearAction = QAction("Cle&ar Selection", self)
            clearAction.setStatusTip("Removes all objects from the selection set.")
            #TODO/PORT# connect(clearAction, SIGNAL(triggered()), this, SLOT(clearSelection()))
            menu.addAction(clearAction)

        menu.exec_(event.globalPos())

    def deletePressed(self):
        """
        TOWRITE
        """
        qDebug("View deletePressed()")
        if self.pastingActive:
            self.gscene.removeItem(self.pasteObjectItemGroup)
            del pasteObjectItemGroup;

        self.pastingActive = False
        self.zoomWindowActive = False
        self.selectingActive = False
        self.selectBox.hide()
        self.stopGripping(False)
        self.deleteSelected()

    def escapePressed(self):
        """
        TOWRITE
        """
        qDebug("View escapePressed()")
        if self.pastingActive:
            self.gscene.removeItem(self.pasteObjectItemGroup)
            del pasteObjectItemGroup

        self.pastingActive = False
        self.zoomWindowActive = False
        self.selectingActive = False
        self.selectBox.hide()
        if self.grippingActive:
            self.stopGripping(False)
        else:
            self.clearSelection()

    def startGripping(self, obj):
        """
        TOWRITE

        :param `obj`: TOWRITE
        :type `obj`: `BaseObject`
        """
        if not obj:
            return
        self.grippingActive = True
        self.gripBaseObj = obj
        self.sceneGripPoint = self.gripBaseObj.mouseSnapPoint(self.sceneMousePoint)
        self.gripBaseObj.setObjectRubberPoint("GRIP_POINT", self.sceneGripPoint)
        self.gripBaseObj.setObjectRubberMode(OBJ_RUBBER_GRIP)

    def stopGripping(self, accept):
        """
        TOWRITE

        :param `accept`: TOWRITE
        :type `accept`: bool
        """
        self.grippingActive = False
        if self.gripBaseObj:

            self.gripBaseObj.vulcanize()
            if accept:

                #TODO/PORT# UndoableGripEditCommand* cmd = new UndoableGripEditCommand(sceneGripPoint, sceneMousePoint, tr("Grip Edit ") + gripBaseObj->data(OBJ_NAME).toString(), gripBaseObj, this, 0);
                if cmd:
                    undoStack.push(cmd)
                self.selectionChanged()  # Update the Property Editor

            self.gripBaseObj = 0

        # Move the sceneGripPoint to a place where it will never be hot
        self.sceneGripPoint = self.sceneRect().topLeft()

    def clearSelection(self):
        """
        TOWRITE
        """
        self.gscene.clearSelection()

    def deleteSelected(self):
        """
        TOWRITE
        """
        itemList = self.gscene.selectedItems() # QList<QGraphicsItem*>
        numSelected = itemList.size() # int
        if numSelected > 1:
            undoStack.beginMacro("Delete " + QString().setNum(itemList.size()))

        for i in range(0, itemList.size()): # for(int i = 0; i < itemList.size(); i++)

            if itemList.at(i).data(OBJ_TYPE) != OBJ_TYPE_NULL:

                base = itemList.at(i) # BaseObject* base = static_cast<BaseObject*>(itemList.at(i));
                if base:

                    #TODO/PORT# UndoableDeleteCommand* cmd = new UndoableDeleteCommand(tr("Delete 1 ") + base->data(OBJ_NAME).toString(), base, this, 0);
                    if cmd:
                        undoStack.push(cmd)

        if numSelected > 1:
            undoStack.endMacro()

    def cut(self):
        """
        TOWRITE
        """
        if self.gscene.selectedItems().isEmpty():

            QMessageBox.information(self, self.tr("Cut Preselect"), self.tr("Preselect objects before invoking the cut command."))
            return # TODO: Prompt to select objects if nothing is preselected

        self.undoStack.beginMacro("Cut")
        self.copySelected()
        self.deleteSelected()
        self.undoStack.endMacro()

    def copy(self):
        """
        TOWRITE
        """
        if gscene.selectedItems().isEmpty():
            QMessageBox.information(self, self.tr("Copy Preselect"), self.tr("Preselect objects before invoking the copy command."))
            return # TODO: Prompt to select objects if nothing is preselected

        self.copySelected()
        self.clearSelection()

    def copySelected(self):
        """
        TOWRITE
        """
        selectedList = self.gscene.selectedItems() # QList<QGraphicsItem*>

        # Prevent memory leaks by deleting any unpasted instances
        qDeleteAll(mainWin.cutCopyObjectList.begin(), mainWin.cutCopyObjectList.end())
        mainWin.cutCopyObjectList.clear()

        # Create new objects but do not add them to the scene just yet.
        # By creating them now, ensures that pasting will still work
        # if the original objects are deleted before the paste occurs.
        mainWin.cutCopyObjectList = createObjectList(selectedList)

    def paste(self):
        """
        TOWRITE
        """
        if self.pastingActive:
            self.gscene.removeItem(self.pasteObjectItemGroup)
            del pasteObjectItemGroup

        self.pasteObjectItemGroup = self.gscene.createItemGroup(mainWin.cutCopyObjectList)
        self.pasteDelta = self.pasteObjectItemGroup.boundingRect().bottomLeft()
        self.pasteObjectItemGroup.setPos(self.sceneMousePoint - self.pasteDelta)
        self.pastingActive = True

        # Re-create the list in case of multiple pastes
        mainWin.cutCopyObjectList = self.createObjectList(mainWin.cutCopyObjectList)

    def createObjectList(self, list):
        """
        TOWRITE

        :param `list`: TOWRITE
        :type `list`: QList<QGraphicsItem*>
        :rtype: QList<QGraphicsItem*>
        """

        copyList = QList # QList<QGraphicsItem*> copyList;

        for i in range(0, list.size()): # for(int i = 0; i < list.size(); i++)

            item = list.at(i) # QGraphicsItem*
            if not item:
                continue

            objType = item.data(OBJ_TYPE).toInt()  # int

            if objType == OBJ_TYPE_ARC:
                arcObj = item # ArcObject* arcObj = static_cast<ArcObject*>(item);
                if arcObj:
                    copyArcObj = ArcObject(arcObj) # ArcObject*
                    copyList.append(copyArcObj)

            elif objType == OBJ_TYPE_BLOCK:
                pass # TODO: cut/copy blocks

            elif objType == OBJ_TYPE_CIRCLE:
                circObj = item # CircleObject* circObj = static_cast<CircleObject*>(item);
                if circObj:
                    copyCircObj = CircleObject(circObj) # CircleObject*
                    copyList.append(copyCircObj)

            elif objType == OBJ_TYPE_DIMALIGNED:
                pass # TODO: cut/copy aligned dimensions

            elif objType == OBJ_TYPE_DIMANGULAR:
                pass # TODO: cut/copy angular dimensions

            elif objType == OBJ_TYPE_DIMARCLENGTH:
                pass # TODO: cut/copy arclength dimensions

            elif objType == OBJ_TYPE_DIMDIAMETER:
                pass # TODO: cut/copy diameter dimensions

            elif objType == OBJ_TYPE_DIMLEADER:
                dimLeaderObj = item # DimLeaderObject* dimLeaderObj = static_cast<DimLeaderObject*>(item);
                if dimLeaderObj:
                    copyDimLeaderObj = DimLeaderObject(dimLeaderObj) # DimLeaderObject*
                    copyList.append(copyDimLeaderObj);

            elif objType == OBJ_TYPE_DIMLINEAR:
                pass # TODO: cut/copy linear dimensions

            elif objType == OBJ_TYPE_DIMORDINATE:
                pass # TODO: cut/copy ordinate dimensions

            elif objType == OBJ_TYPE_DIMRADIUS:
                pass # TODO: cut/copy radius dimensions

            elif objType == OBJ_TYPE_ELLIPSE:
                elipObj = item # EllipseObject* elipObj = static_cast<EllipseObject*>(item);
                if elipObj:
                    copyElipObj = EllipseObject(elipObj) # EllipseObject*
                    copyList.append(copyElipObj);

            elif objType == OBJ_TYPE_ELLIPSEARC:
                pass # TODO: cut/copy elliptical arcs

            elif objType == OBJ_TYPE_IMAGE:
                pass # TODO: cut/copy images

            elif objType == OBJ_TYPE_INFINITELINE:
                pass # TODO: cut/copy infinite lines

            elif objType == OBJ_TYPE_LINE:
                lineObj = item # LineObject* lineObj = static_cast<LineObject*>(item);
                if lineObj:
                    copyLineObj = LineObject(lineObj) # LineObject*
                    copyList.append(copyLineObj)

            elif objType == OBJ_TYPE_PATH:
                pathObj = item # PathObject* pathObj = static_cast<PathObject*>(item);
                if pathObj:
                    copyPathObj = PathObject(pathObj) # PathObject*
                    copyList.append(copyPathObj)

            elif objType == OBJ_TYPE_POINT:
                pointObj = item # PointObject* pointObj = static_cast<PointObject*>(item);
                if pointObj:
                    copyPointObj = PointObject(pointObj) # PointObject*
                    copyList.append(copyPointObj)

            elif objType == OBJ_TYPE_POLYGON:
                pgonObj = item # PolygonObject* pgonObj = static_cast<PolygonObject*>(item);
                if pgonObj:
                    copyPgonObj = PolygonObject(pgonObj) # PolygonObject*
                    copyList.append(copyPgonObj)

            elif objType == OBJ_TYPE_POLYLINE:
                plineObj = item # PolylineObject* plineObj = static_cast<PolylineObject*>(item);
                if plineObj:
                    copyPlineObj = PolylineObject(plineObj) # PolylineObject*
                    copyList.append(copyPlineObj)

            elif objType == OBJ_TYPE_RAY:
                pass # TODO: cut/copy rays

            elif objType == OBJ_TYPE_RECTANGLE:
                rectObj = item # RectObject* rectObj = static_cast<RectObject*>(item);
                if rectObj:
                    copyRectObj = RectObject(rectObj) # RectObject*
                    copyList.append(copyRectObj)

            elif objType == OBJ_TYPE_TEXTSINGLE:
                textObj = item # TextSingleObject* textObj = static_cast<TextSingleObject*>(item);
                if textObj:
                    copyTextObj = TextSingleObject(textObj) # TextSingleObject*
                    copyList.append(copyTextObj)

        return copyList

    def repeatAction(self):
        """
        TOWRITE
        """
        mainWin.prompt.endCommand()
        mainWin.prompt.setCurrentText(mainWin.prompt.lastCommand())
        mainWin.prompt.processInput()

    def moveAction(self):
        """
        TOWRITE
        """
        mainWin.prompt.endCommand()
        mainWin.prompt.setCurrentText("move")
        mainWin.prompt.processInput()

    def moveSelected(self, dx, dy):
        """
        TOWRITE

        :param `dx`: TOWRITE
        :type `dx`: qreal
        :param `dy`: TOWRITE
        :type `dy`: qreal
        """
        itemList = self.gscene.selectedItems() # QList<QGraphicsItem*>
        numSelected = itemList.size() # int
        if numSelected > 1:
            undoStack.beginMacro("Move " + QString().setNum(itemList.size()))

        for item in itemList: # foreach(QGraphicsItem* item, itemList)

            base = item # BaseObject* base = static_cast<BaseObject*>(item);
            if base:
                #TODO/PORT# UndoableMoveCommand* cmd = new UndoableMoveCommand(dx, dy, tr("Move 1 ") + base->data(OBJ_NAME).toString(), base, this, 0);
                if cmd:
                    undoStack.push(cmd)

        if numSelected > 1:
            undoStack.endMacro()

        # Always clear the selection after a move
        self.gscene.clearSelection()

    def rotateAction(self):
        """
        TOWRITE
        """
        mainWin.prompt.endCommand()
        mainWin.prompt.setCurrentText("rotate")
        mainWin.prompt.processInput()

    def rotateSelected(self, x, y, rot):
        """
        TOWRITE

        :param `x`: TOWRITE
        :type `x`: qreal
        :param `y`: TOWRITE
        :type `y`: qreal
        :param `rot`: TOWRITE
        :type `rot`: qreal
        """
        itemList = self.gscene.selectedItems() # QList<QGraphicsItem*>
        numSelected = itemList.size() # int
        if numSelected > 1:
            undoStack.beginMacro("Rotate " + QString().setNum(itemList.size()))

        for item in itemList: # foreach(QGraphicsItem* item, itemList)
            base = item # BaseObject* base = static_cast<BaseObject*>(item);
            if base:
                #TODO/PORT# UndoableRotateCommand* cmd = new UndoableRotateCommand(x, y, rot, tr("Rotate 1 ") + base->data(OBJ_NAME).toString(), base, this, 0);
                if cmd:
                    undoStack.push(cmd)

        if numSelected > 1:
            undoStack.endMacro()

        # Always clear the selection after a rotate
        self.gscene.clearSelection()

    def mirrorSelected(self, x1, y1, x2, y2):
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
        itemList = self.gscene.selectedItems() # QList<QGraphicsItem*>
        numSelected = itemList.size() # int
        if numSelected > 1:
            undoStack.beginMacro("Mirror " + QString().setNum(itemList.size()))

        for item in itemList: # foreach(QGraphicsItem* item, itemList)
            base = item # BaseObject* base = static_cast<BaseObject*>(item);
            if base:
                #TODO/PORT# UndoableMirrorCommand* cmd = new UndoableMirrorCommand(x1, y1, x2, y2, tr("Mirror 1 ") + base->data(OBJ_NAME).toString(), base, this, 0);
                if cmd:
                    undoStack.push(cmd)

        if numSelected > 1:
            undoStack.endMacro()

        # Always clear the selection after a mirror
        self.gscene.clearSelection()

    def scaleAction(self):
        """
        TOWRITE
        """
        mainWin.prompt.endCommand()
        mainWin.prompt.setCurrentText("scale")
        mainWin.prompt.processInput()

    def scaleSelected(self, x, y, factor):
        """
        TOWRITE

        :param `x`: TOWRITE
        :type `x`: qreal
        :param `y`: TOWRITE
        :type `y`: qreal
        :param `factor`: TOWRITE
        :type `factor`: qreal
        """
        itemList = self.gscene.selectedItems() # QList<QGraphicsItem*>
        numSelected = itemList.size() # int
        if numSelected > 1:
            undoStack.beginMacro("Scale " + QString().setNum(itemList.size()))

        for item in itemList: # foreach(QGraphicsItem* item, itemList)
            base = item # BaseObject* base = static_cast<BaseObject*>(item);
            if base:
                #TODO/PORT# UndoableScaleCommand* cmd = new UndoableScaleCommand(x, y, factor, tr("Scale 1 ") + base->data(OBJ_NAME).toString(), base, this, 0);
                if cmd:
                    undoStack.push(cmd)

        if numSelected > 1:
            undoStack.endMacro()

        # Always clear the selection after a scale
        self.gscene.clearSelection()

    def numSelected(self):
        """
        TOWRITE

        :rtype: int
        """
        return self.gscene.selectedItems().size()

    def showScrollBars(self, val):
        """
        TOWRITE

        :param `val`: TOWRITE
        :type `val`: bool
        """
        if val:
            self.setHorizontalScrollBarPolicy(Qt.ScrollBarAlwaysOn)
            self.setVerticalScrollBarPolicy(Qt.ScrollBarAlwaysOn)
        else:
            self.setHorizontalScrollBarPolicy(Qt.ScrollBarAlwaysOff)
            self.setVerticalScrollBarPolicy(Qt.ScrollBarAlwaysOff)

    def setCrossHairColor(self, color):
        """
        TOWRITE

        :param `color`: TOWRITE
        :type `color`: QRgb
        """
        self.crosshairColor = color
        self.gscene.setProperty(VIEW_COLOR_CROSSHAIR, color)
        if self.gscene:
            self.gscene.update()

    def setBackgroundColor(self, color):
        """
        TOWRITE

        :param `color`: TOWRITE
        :type `color`: QRgb
        """
        self.setBackgroundBrush(QColor(color))
        self.gscene.setProperty(VIEW_COLOR_BACKGROUND, color)
        if self.gscene:
            self.gscene.update()

    def setSelectBoxColors(self, colorL, fillL, colorR, fillR, alpha):
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
        self.selectBox.setColors(QColor(colorL), QColor(fillL), QColor(colorR), QColor(fillR), alpha)


# kate: bom off; indent-mode python; indent-width 4; replace-trailing-space-save on;
