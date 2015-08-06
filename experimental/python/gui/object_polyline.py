#!/usr/bin/env python
# -*- coding: utf-8 -*-

"""
===================================
|module_summary| object_polyline.py
===================================

TOWRITE


Classes summary:
================

============================ ============================
:class:`~PolylineObject`     TOWRITE
============================ ============================

---------------------------------------------------------

|

"""

#-Imports.---------------------------------------------------------------------
#--PySide/PyQt Imports.
if PYSIDE:
    ## from PySide import QtCore, QtGui
    # or... Improve performace with less dots...
    from PySide.QtCore import qDebug, Qt, QLineF, QPointF, QObject
    from PySide.QtGui import QGraphicsItem, QPainter, QPainterPath, QStyle, QMessageBox, QTransform
elif PYQT4:
    import sip
    sip.setapi('QString', 2)
    sip.setapi('QVariant', 2)
    ## from PyQt4 import QtCore, QtGui
    # or... Improve performace with less dots...
    from PyQt4.QtCore import qDebug, Qt, QLineF, QPointF, QObject
    from PyQt4.QtGui import QGraphicsItem, QPainter, QPainterPath, QStyle, QMessageBox, QTransform

#--Local Imports.
from hacks import overloaded, signature
from object_base import BaseObject
from object_data import (OBJ_TYPE, OBJ_TYPE_POLYLINE, OBJ_NAME, ENABLE_LWT,
    ENABLE_REAL, OBJ_RUBBER_POLYLINE, OBJ_RUBBER_GRIP, OBJ_RUBBER_OFF,
    OBJ_NAME_POLYLINE)

# C++C++C++C++C++C++C++C++C++C++C++C++C++C++C++C++C++C++C++C++C++C++C++C++C++
#include "object-polyline.h"
#include "object-data.h"

#include <QPainter>
#include <QStyleOption>
#include <QGraphicsScene>
#include <QMessageBox>
# C++C++C++C++C++C++C++C++C++C++C++C++C++C++C++C++C++C++C++C++C++C++C++C++C++


class PolylineObject(BaseObject):
    """
    Subclass of `BaseObject`_

    TOWRITE

    .. sphinx_generate_methods_summary::
       PolylineObject
    """

    Type = OBJ_TYPE_POLYLINE

    def __init__(self, x, y, p, rgb, parent=None):
        #OVERLOADED IMPL?# PolylineObject::PolylineObject(PolylineObject* obj, QGraphicsItem* parent) : BaseObject(parent)
        """
        Default class constructor.

        :param `x`: TOWRITE
        :type `x`: qreal
        :param `y`: TOWRITE
        :type `y`: qreal
        :param `p`: TOWRITE
        :type `p`: `QPainterPath`_
        :param `rgb`: TOWRITE
        :type `rgb`: QRgb
        :param `parent`: TOWRITE
        :type `parent`: `QGraphicsItem`_
        """
        super(PolylineObject, self).__init__(parent)

        qDebug("PolylineObject Constructor()")

        self.normalPath = QPainterPath()
        self.gripIndex = int()

        self.init(x, y, p, rgb, Qt.SolidLine)  # TODO: getCurrentLineType

        #OVERLOADED IMPL?# if obj:
        #OVERLOADED IMPL?#     self.init(obj.objectX(), obj.objectY(), obj.objectCopyPath(), obj.objectColorRGB(), Qt.SolidLine)  # TODO: getCurrentLineType
        #OVERLOADED IMPL?#     self.setRotation(obj.rotation())
        #OVERLOADED IMPL?#     self.setScale(obj.scale())


    def __del__(self):
        """Class destructor."""
        qDebug("PolylineObject Destructor()")

    def init(self, x, y, p, rgb, lineType):
        """
        TOWRITE

        :param `x`: TOWRITE
        :type `x`: qreal
        :param `y`: TOWRITE
        :type `y`: qreal
        :param `p`: TOWRITE
        :type `p`: `QPainterPath`_
        :param `rgb`: TOWRITE
        :type `rgb`: QRgb
        :param `lineType`: TOWRITE
        :type `lineType`: Qt.PenStyle
        """
        self.setData(OBJ_TYPE, self.type())
        self.setData(OBJ_NAME, OBJ_NAME_POLYLINE)

        # WARNING: DO NOT enable QGraphicsItem::ItemIsMovable. If it is enabled,
        # WARNING: and the item is double clicked, the scene will erratically move the item while zooming.
        # WARNING: All movement has to be handled explicitly by us, not by the scene.
        self.setFlag(QGraphicsItem.ItemIsSelectable, True)

        self.gripIndex = -1
        self.updatePath(p)
        self.setObjectPos(x,y)
        self.setObjectColorRGB(rgb)
        self.setObjectLineType(lineType)
        self.setObjectLineWeight(0.35)  # TODO: pass in proper lineweight
        self.setPen(self.objectPen())

    def updatePath(self, p):
        """
        TOWRITE

        :param `p`: TOWRITE
        :type `p`: `QPainterPath`_
        """
        self.normalPath = p
        reversePath = self.normalPath.toReversed()  # QPainterPath
        reversePath.connectPath(self.normalPath)
        self.setObjectPath(reversePath)

    def paint(self, painter, option, widget):
        """
        TOWRITE

        :param `painter`: TOWRITE
        :type `painter`: `QPainter`_
        :param `option`: TOWRITE
        :type `option`: `QStyleOptionGraphicsItem`_
        :param `widget`: TOWRITE
        :type `widget`: `QWidget`_
        """
        objScene = self.scene()  # QGraphicsScene*
        if not objScene:
            return

        paintPen = self.pen()  # QPen
        painter.setPen(paintPen)
        self.updateRubber(painter)
        if option.state & QStyle.State_Selected:
            paintPen.setStyle(Qt.DashLine)
        if objScene.property(ENABLE_LWT):  # .toBool()
            paintPen = self.lineWeightPen()
        painter.setPen(paintPen)

        painter.drawPath(self.normalPath)

        if (objScene.property(ENABLE_LWT) and objScene.property(ENABLE_REAL)):  # .toBool()
            self.realRender(painter, self.normalPath)

    def updateRubber(self, painter=None):
        """
        TOWRITE

        :param `painter`: TOWRITE
        :type `painter`: `QPainter`_
        """
        rubberMode = self.objectRubberMode()  # int
        if rubberMode == OBJ_RUBBER_POLYLINE:

            self.setObjectPos(self.objectRubberPoint("POLYLINE_POINT_0"))

            rubberLine = QLineF(self.normalPath.currentPosition(), self.mapFromScene(self.objectRubberPoint("")))
            if painter:
                self.drawRubberLine(rubberLine, painter, "VIEW_COLOR_CROSSHAIR")

            ok = False  # bool
            numStr = self.objectRubberText("POLYLINE_NUM_POINTS")  # QString
            if not numStr:
                return

            try:
                num = int(numStr)
            except ValueError:
                return

            appendStr = ''  # QString
            rubberPath = QPainterPath()
            for i in range(1, num):  # for(int i = 1; i <= num; i++)

                appendStr = "POLYLINE_POINT_" + "%s" % i  # QString().setNum(i);
                appendPoint = self.mapFromScene(self.objectRubberPoint(appendStr))  # QPointF
                rubberPath.lineTo(appendPoint)

            self.updatePath(rubberPath)

            # Ensure the path isn't updated until the number of points is changed again.
            self.setObjectRubberText("POLYLINE_NUM_POINTS", "")

        elif rubberMode == OBJ_RUBBER_GRIP:

            if painter:

                elemCount = self.normalPath.elementCount()  # int
                gripPoint = self.objectRubberPoint("GRIP_POINT")  # QPointF
                if self.gripIndex == -1:
                    self.gripIndex = self.findIndex(gripPoint)
                if self.gripIndex == -1:
                    return

                if not self.gripIndex: # First.

                    ef = self.normalPath.elementAt(1)  # QPainterPath::Element
                    efPoint = QPointF(ef.x, ef.y)  # QPointF
                    painter.drawLine(efPoint, self.mapFromScene(self.objectRubberPoint("")))

                elif self.gripIndex == elemCount - 1: # Last.

                    el = self.normalPath.elementAt(self.gripIndex - 1)  # QPainterPath::Element
                    elPoint = QPointF(el.x, el.y)  # QPointF
                    painter.drawLine(elPoint, self.mapFromScene(self.objectRubberPoint("")))

                else: # Middle.

                    em = self.normalPath.elementAt(self.gripIndex - 1)  # QPainterPath::Element
                    en = self.normalPath.elementAt(self.gripIndex + 1)  # QPainterPath::Element
                    emPoint = QPointF(em.x, em.y)  # QPointF
                    enPoint = QPointF(en.x, en.y)  # QPointF
                    painter.drawLine(emPoint, self.mapFromScene(self.objectRubberPoint("")))
                    painter.drawLine(enPoint, self.mapFromScene(self.objectRubberPoint("")))

                rubLine = QLineF(self.mapFromScene(gripPoint), self.mapFromScene(self.objectRubberPoint("")))
                self.drawRubberLine(rubLine, painter, "VIEW_COLOR_CROSSHAIR")

    def vulcanize(self):
        """
        TOWRITE
        """
        qDebug("PolylineObject vulcanize()")
        self.updateRubber()

        self.setObjectRubberMode(OBJ_RUBBER_OFF)

        if not self.normalPath.elementCount():
            QMessageBox.critical(0, QObject.tr("Empty Polyline Error"), QObject.tr("The polyline added contains no points. The command that created this object has flawed logic."))

    def mouseSnapPoint(self, mousePoint):
        """
        Returns the closest snap point to the mouse point.

        :param `mousePoint`: TOWRITE
        :type `mousePoint`: `QPointF`_
        :rtype: `QPointF`_
        """
        element = self.normalPath.elementAt(0)  # QPainterPath::Element
        closestPoint = self.mapToScene(QPointF(element.x, element.y))  # QPointF
        closestDist = QLineF(mousePoint, closestPoint).length()  # qreal
        elemCount = self.normalPath.elementCount()  # int
        for i in range(0, elemCount):  # for(int i = 0; i < elemCount; ++i)

            element = self.normalPath.elementAt(i)
            elemPoint = self.mapToScene(element.x, element.y)  # QPointF
            elemDist = QLineF(mousePoint, elemPoint).length()  # qreal
            if elemDist < closestDist:

                closestPoint = elemPoint
                closestDist = elemDist

        return closestPoint

    def allGripPoints(self):
        """
        TOWRITE

        :rtype: QList<QPointF>
        """
        ## QList<QPointF> gripPoints;
        ## QPainterPath::Element element;
        ## for(int i = 0; i < self.normalPath.elementCount(); ++i)
        ##
        ##     element = self.normalPath.elementAt(i);
        ##     gripPoints << mapToScene(element.x, element.y);
        gripPoints = []
        element = QPainterPath.Element
        for i in range(0, self.normalPath.elementCount()):
            element = self.normalPath.elementAt(i)
            gripPoints.append(self.mapToScene(element.x, element.y))

        return gripPoints

    def findIndex(self, point):
        """
        TOWRITE

        :param `point`: TOWRITE
        :type `point`: `QPointF`_
        :rtype: int
        """
        elemCount = self.normalPath.elementCount()  # int
        # NOTE: Points here are in item coordinates
        itemPoint = self.mapFromScene(point)  # QPointF
        for i in range(0, elemCount):  # for(int i = 0; i < elemCount; i++)

            e = self.normalPath.elementAt(i)  # QPainterPath::Element
            elemPoint = QPointF(e.x, e.y)  # QPointF
            if itemPoint == elemPoint:
                return i

        return -1

    def gripEdit(self, before, after):
        """
        TOWRITE

        :param `before`: TOWRITE
        :type `before`: `QPointF`_
        :param `after`: TOWRITE
        :type `after`: `QPointF`_
        """
        self.gripIndex = self.findIndex(before)
        if self.gripIndex == -1:
            return
        a = self.mapFromScene(after)  # QPointF
        self.normalPath.setElementPositionAt(self.gripIndex, a.x(), a.y())
        self.updatePath(self.normalPath)
        self.gripIndex = -1

    def objectCopyPath(self):
        """
        TOWRITE

        :rtype: `QPainterPath`_
        """
        return self.normalPath

    def objectSavePath(self):
        """
        TOWRITE

        :rtype: `QPainterPath`_
        """
        s = self.scale()  # qreal
        trans = QTransform()
        trans.rotate(self.rotation())
        trans.scale(s, s)
        return trans.map(self.normalPath)

    def objectPos(self):
        """
        TOWRITE

        :return: TOWRITE
        :rtype: `QPointF`_
        """
        return self.scenePos()

    def objectX(self):
        """
        TOWRITE

        :return: TOWRITE
        :rtype: float
        """
        return self.scenePos().x()

    def objectY(self):
        """
        TOWRITE

        :return: TOWRITE
        :rtype: float
        """
        return self.scenePos().y()

    # pythonic setObjectPos overload
    @signature(QPointF)
    def setObjectPosFromPoint(self, point):
        """
        TOWRITE

        :param `point`: TOWRITE
        :type `point`: `QPointF`_
        """
        self.setPos(point.x(), point.y())

    # pythonic setObjectPos overload
    @signature(float, float)
    def setObjectPosFromXY(self, x, y):
        """
        TOWRITE

        :param `x`: TOWRITE
        :type `x`: float
        :param y`: TOWRITE
        :type `y`: float
        """
        self.setPos(x, y)

    @overloaded(setObjectPosFromPoint, setObjectPosFromXY)
    def setObjectPos(self, *args):
        """ TOWRITE """
        pass

    def setObjectX(self, x):
        """
        TOWRITE

        :param `x`: TOWRITE
        :type `x`: float
        """
        self.setObjectPos(x, self.objectY())

    def setObjectY(self, y):
        """
        TOWRITE

        :param `y`: TOWRITE
        :type `y`: float
        """
        self.setObjectPos(self.objectX(), y)


# kate: bom off; indent-mode python; indent-width 4; replace-trailing-space-save on;
