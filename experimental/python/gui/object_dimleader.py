#!/usr/bin/env python
# -*- coding: utf-8 -*-

"""
====================================
|module_summary| object_dimleader.py
====================================

TOWRITE


Classes summary:
================

============================ ============================
:class:`~DimLeaderObject`    TOWRITE
============================ ============================

---------------------------------------------------------

|

"""

#-Imports.---------------------------------------------------------------------
#--Python Imports.
from math import sin as qSin
from math import cos as qCos
from math import radians
qMin = min

#--PySide/PyQt Imports.
if PYSIDE:
    ## from PySide import QtCore, QtGui
    # or... Improve performace with less dots...
    from PySide.QtCore import qDebug, Qt, QLineF, QPointF, QRectF
    from PySide.QtGui import QGraphicsItem, QPainter, QPainterPath, QStyle
elif PYQT4:
    import sip
    sip.setapi('QString', 2)
    sip.setapi('QVariant', 2)
#    ## from PyQt4 import QtCore, QtGui
#    # or... Improve performace with less dots...
    from PyQt4.QtCore import qDebug, Qt, QLineF, QPointF, QRectF
    from PyQt4.QtGui import QGraphicsItem, QPainter, QPainterPath, QStyle

#--Local Imports.
from hacks import overloaded, signature
from object_base import BaseObject
from object_data import (OBJ_RUBBER_OFF, OBJ_RUBBER_GRIP,
    OBJ_RUBBER_DIMLEADER_LINE, ENABLE_LWT, OBJ_TYPE, OBJ_NAME,
    OBJ_NAME_DIMLEADER, OBJ_TYPE_DIMLEADER)

# C++C++C++C++C++C++C++C++C++C++C++C++C++C++C++C++C++C++C++C++C++C++C++C++C++
#include "object-dimleader.h"
#include "object-data.h"

#include <QPainter>
#include <QStyleOption>
#include <QGraphicsScene>
# C++C++C++C++C++C++C++C++C++C++C++C++C++C++C++C++C++C++C++C++C++C++C++C++C++


# enum ArrowStyle
# {
NoArrow = 0  # NOTE: Allow this enum to evaluate false
Open = 1
Closed = 2
Dot = 3
Box = 4
Tick = 5
# };

# enum lineStyle
# {
NoLine = 0  # NOTE: Allow this enum to evaluate false
Flared = 1
Fletching = 2
# };


class DimLeaderObject(BaseObject):
    """
    Subclass of `BaseObject`_

    TOWRITE

    """

    Type = OBJ_TYPE_DIMLEADER

    def __init__(self, x1, y1, x2, y2, rgb, parent=None):
        #OVERLOADED IMPL?# DimLeaderObject::DimLeaderObject(DimLeaderObject* obj, QGraphicsItem* parent) : BaseObject(parent)
        """
        Default class constructor.

        :param `x1`: TOWRITE
        :type `x1`: qreal
        :param `y1`: TOWRITE
        :type `y1`: qreal
        :param `x2`: TOWRITE
        :type `x2`: qreal
        :param `y2`: TOWRITE
        :type `y2`: qreal
        :param `rgb`: TOWRITE
        :type `rgb`: QRgb
        :param `parent`: TOWRITE
        :type `parent`: `QGraphicsItem`_
        """
        super(DimLeaderObject, self).__init__(parent)

        qDebug("DimLeaderObject Constructor()")

        self.curved = bool()
        self.filled = bool()
        self.lineStylePath = QPainterPath()
        self.arrowStylePath = QPainterPath()
        self.arrowStyleAngle = float()   # qreal
        self.arrowStyleLength = float()  # qreal
        self.lineStyleAngle = float()    # qreal
        self.lineStyleLength = float()   # qreal

        self.init(x1, y1, x2, y2, rgb, Qt.SolidLine)  # TODO: getCurrentLineType

        #OVERLOADED IMPL?# if obj:
        #OVERLOADED IMPL?#     init(obj.objectX1(), obj.objectY1(), obj.objectX2(), obj.objectY2(), obj.objectColorRGB(), Qt.SolidLine)  # TODO: getCurrentLineType


    def __del__(self):
        """Class destructor."""
        qDebug("DimLeaderObject Destructor()")

    def init(self, x1, y1, x2, y2, rgb, lineType):
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
        :param `rgb`: TOWRITE
        :type `rgb`: QRgb
        :param `lineType`: TOWRITE
        :type `lineType`: Qt.PenStyle
        """
        self.setData(OBJ_TYPE, self.type())
        self.setData(OBJ_NAME, OBJ_NAME_DIMLEADER)

        # WARNING: DO NOT enable QGraphicsItem::ItemIsMovable. If it is enabled,
        # WARNING: and the item is double clicked, the scene will erratically move the item while zooming.
        # WARNING: All movement has to be handled explicitly by us, not by the scene.
        self.setFlag(QGraphicsItem.ItemIsSelectable, True)

        self.curved = False
        self.filled = True
        self.setObjectEndPoint1(x1, y1)
        self.setObjectEndPoint2(x2, y2)
        self.setObjectColorRGB(rgb)
        self.setObjectLineType(lineType)
        self.setObjectLineWeight(0.35)  # TODO: pass in proper lineweight
        self.setPen(self.objectPen())

    # pythonic setObjectEndPoint1 overload
    @signature(QPointF)
    def setObjectEndPoint1FromPoint(self, endPt1):
        """
        TOWRITE

        :param `endPt1`: TOWRITE
        :type `endPt1`: `QPointF`_
        """
        self.setObjectEndPoint1(endPt1.x(), endPt1.y())

    # pythonic setObjectEndPoint1 overload
    @signature(float, float)
    def setObjectEndPoint1FromXY(self, x1, y1):
        """
        TOWRITE

        :param `x1`: TOWRITE
        :type `x1`: qreal
        :param `y1`: TOWRITE
        :type `y1`: qreal
        """
        endPt2 = self.objectEndPoint2()  # QPointF
        x2 = endPt2.x()  # qreal
        y2 = endPt2.y()  # qreal
        dx = x2 - x1     # qreal
        dy = y2 - y1     # qreal
        self.setRotation(0)
        self.setLine(0, 0, dx, dy)
        self.setPos(x1, y1)
        self.updateLeader()

    @overloaded(setObjectEndPoint1FromPoint, setObjectEndPoint1FromXY)
    def setObjectEndPoint1(self, *args):
        """ TOWRITE """
        pass

    # pythonic setObjectEndPoint2 overload
    @signature(QPointF)
    def setObjectEndPoint2FromPoint(self, endPt2):
        """
        TOWRITE

        :param `endPt2`: TOWRITE
        :type `endPt2`: `QPointF`_
        """
        self.setObjectEndPoint2(endPt2.x(), endPt2.y())

    # pythonic setObjectEndPoint2 overload
    @signature(float, float)
    def setObjectEndPoint2FromXY(self, x2, y2):
        """
        TOWRITE

        :param `x2`: TOWRITE
        :type `x2`: qreal
        :param `y2`: TOWRITE
        :type `y2`: qreal
        """
        endPt1 = self.scenePos()  # QPointF
        x1 = endPt1.x()  # qreal
        y1 = endPt1.y()  # qreal
        dx = x2 - x1     # qreal
        dy = y2 - y1     # qreal
        self.setRotation(0)
        self.setLine(0, 0, dx, dy)
        self.setPos(x1, y1)
        self.updateLeader()

    @overloaded(setObjectEndPoint2FromPoint, setObjectEndPoint2FromXY)
    def setObjectEndPoint2(self, *args):
        """ TOWRITE """
        pass

    def objectEndPoint1(self):
        """
        TOWRITE

        :rtype: `QPointF`_
        """
        return self.scenePos()

    def objectEndPoint2(self):
        """
        TOWRITE

        :rtype: `QPointF`_
        """
        lyne = self.line()  # QLineF
        rot = radians(self.rotation())       # qreal
        cosRot = qCos(rot)                   # qreal
        sinRot = qSin(rot)                   # qreal
        x2 = lyne.x2() * self.scale()        # qreal
        y2 = lyne.y2() * self.scale()        # qreal
        rotEnd2X = x2 * cosRot - y2 * sinRot # qreal
        rotEnd2Y = x2 * sinRot + y2 * cosRot # qreal

        return (self.scenePos() + QPointF(rotEnd2X, rotEnd2Y))

    def objectMidPoint(self):
        """
        TOWRITE

        :rtype: `QPointF`_
        """
        lyne = self.line()  # QLineF
        mp = lyne.pointAt(0.5)  # QPointF
        rot = radians(self.rotation())       # qreal
        cosRot = qCos(rot)                   # qreal
        sinRot = qSin(rot)                   # qreal
        mx = mp.x() * self.scale()           # qreal
        my = mp.y() * self.scale()           # qreal
        rotMidX = mx * cosRot - my * sinRot  # qreal
        rotMidY = mx * sinRot + my * cosRot  # qreal

        return (self.scenePos() + QPointF(rotMidX, rotMidY))

    def objectAngle(self):
        """
        TOWRITE

        :rtype: qreal
        """
        angle = self.line().angle() - self.rotation()  # qreal
        while angle >= 360.0:
            angle -= 360.0
        while angle < 0.0:
            angle += 360.0
        return angle

    def updateLeader(self):
        """
        TOWRITE
        """
        arrowStyle = Closed     # int # TODO: Make this customizable
        arrowStyleAngle = 15.0  # qreal # TODO: Make this customizable
        arrowStyleLength = 1.0  # qreal # TODO: Make this customizable
        self.lineStyleAngle = 45.0   # qreal # TODO: Make this customizable
        self.lineStyleLength = 1.0   # qreal # TODO: Make this customizable

        lyne = self.line()  # QLineF
        angle = lyne.angle()  # qreal
        ap0 = lyne.p1()  # QPointF
        lp0 = lyne.p2()  # QPointF

        # Arrow
        lynePerp = QLineF(lyne.pointAt(arrowStyleLength / lyne.length()), lp0)
        lynePerp.setAngle(angle + 90)
        lyne1 = QLineF(ap0, lp0)
        lyne2 = QLineF(ap0, lp0)
        lyne1.setAngle(angle + arrowStyleAngle)
        lyne2.setAngle(angle - arrowStyleAngle)
        # ap1 = QPointF()
        # ap2 = QPointF()
        _, ap1 = lynePerp.intersect(lyne1)
        _, ap2 = lynePerp.intersect(lyne2)

        # Math Diagram
        #                  .(ap1)                     .(lp1)
        #                 /|                         /|
        #                / |                        / |
        #               /  |                       /  |
        #              /   |                      /   |
        #             /    |                     /    |
        #            /     |                    /     |
        #           /      |                   /      |
        #          /       |                  /       |
        #         /+(aSA)  |                 /+(lSA)  |
        #  (ap0)./__(aSL)__|__________(lp0)./__(lSL)__|
        #        \ -(aSA)  |                \ -(lSA)  |
        #         \        |                 \        |
        #          \       |                  \       |
        #           \      |                   \      |
        #            \     |                    \     |
        #             \    |                     \    |
        #              \   |                      \   |
        #               \  |                       \  |
        #                \ |                        \ |
        #                 \|                         \|
        #                  .(ap2)                     .(lp2)

        if arrowStyle == Open:
            arrowStylePath = QPainterPath()
            arrowStylePath.moveTo(ap1)
            arrowStylePath.lineTo(ap0)
            arrowStylePath.lineTo(ap2)
            arrowStylePath.lineTo(ap0)
            arrowStylePath.lineTo(ap1)
        elif arrowStyle == Closed:
            arrowStylePath = QPainterPath()
            arrowStylePath.moveTo(ap1)
            arrowStylePath.lineTo(ap0)
            arrowStylePath.lineTo(ap2)
            arrowStylePath.lineTo(ap1)
        elif arrowStyle == Dot:
            arrowStylePath = QPainterPath()
            arrowStylePath.addEllipse(ap0, arrowStyleLength, arrowStyleLength)
        elif arrowStyle == Box:
            arrowStylePath = QPainterPath()
            side = QLineF(ap1, ap2).length()  # qreal
            ar0 = QRectF(0, 0, side, side)
            ar0.moveCenter(ap0)
            arrowStylePath.addRect(ar0)
        elif arrowStyle == Tick:
            pass  #TODO/PORT# Is this supposed to be empty?

        lineStylePath = QPainterPath()
        lineStylePath.moveTo(ap0)
        lineStylePath.lineTo(lp0)

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

        painter.drawPath(self.lineStylePath)
        painter.drawPath(self.arrowStylePath)

        if self.filled:
            painter.fillPath(self.arrowStylePath, self.objectColor())

    def updateRubber(self, painter=None):
        """
        TOWRITE

        :param `painter`: TOWRITE
        :type `painter`: `QPainter`_
        """
        rubberMode = self.objectRubberMode()  # int
        if rubberMode == OBJ_RUBBER_DIMLEADER_LINE:
            sceneStartPoint = self.objectRubberPoint("DIMLEADER_LINE_START")  # QPointF
            sceneQSnapPoint = self.objectRubberPoint("DIMLEADER_LINE_END")    # QPointF

            self.setObjectEndPoint1(sceneStartPoint)
            self.setObjectEndPoint2(sceneQSnapPoint)

        elif rubberMode == OBJ_RUBBER_GRIP:

            if painter:

                gripPoint = self.objectRubberPoint("GRIP_POINT")  # QPointF
                if   gripPoint == self.objectEndPoint1():
                    painter.drawLine(self.line().p2(), self.mapFromScene(self.objectRubberPoint('')))
                elif gripPoint == self.objectEndPoint2():
                    painter.drawLine(self.line().p1(), self.mapFromScene(self.objectRubberPoint('')))
                elif gripPoint == self.objectMidPoint():
                    painter.drawLine(self.line().translated(self.mapFromScene(self.objectRubberPoint('')) - self.mapFromScene(gripPoint)))

    def vulcanize(self):
        """
        TOWRITE
        """
        qDebug("DimLeaderObject vulcanize()")
        self.updateRubber()
        self.setObjectRubberMode(OBJ_RUBBER_OFF)

    def mouseSnapPoint(self, mousePoint):
        """
        Returns the closest snap point to the mouse point.

        :param `mousePoint`: TOWRITE
        :type `mousePoint`: `QPointF`_
        :rtype: `QPointF`_
        """
        endPoint1 = self.objectEndPoint1()  # QPointF
        endPoint2 = self.objectEndPoint2()  # QPointF
        midPoint  = self.objectMidPoint()   # QPointF

        end1Dist = QLineF(mousePoint, endPoint1).length()  # qreal
        end2Dist = QLineF(mousePoint, endPoint2).length()  # qreal
        midDist  = QLineF(mousePoint, midPoint).length()   # qreal

        minDist = qMin(end1Dist, end2Dist)  # qreal

        if self.curved:
            minDist = qMin(minDist, midDist)

        if   minDist == end1Dist: return endPoint1
        elif minDist == end2Dist: return endPoint2
        elif minDist == midDist:  return midPoint

        return self.scenePos()

    def allGripPoints(self):
        """
        TOWRITE

        :rtype: QList<QPointF>
        """
        ## QList<QPointF> gripPoints;
        ## gripPoints << objectEndPoint1() << objectEndPoint2();
        gripPoints = [self.objectEndPoint1(), self.objectEndPoint2()]
        if self.curved:
            gripPoints.append(self.objectMidPoint())
        return gripPoints

    def gripEdit(self, before, after):
        """
        TOWRITE

        :param `before`: TOWRITE
        :type `before`: `QPointF`_
        :param `after`: TOWRITE
        :type `after`: `QPointF`_
        """
        if   before == self.objectEndPoint1():
            self.setObjectEndPoint1(after)
        elif before == self.objectEndPoint2():
            self.setObjectEndPoint2(after)
        elif before == self.objectMidPoint():
            delta = QPointF(after-before)
            self.moveBy(delta.x(), delta.y())

    def objectX1(self):
        """
        TOWRITE

        :return: TOWRITE
        :rtype: float
        """
        return self.objectEndPoint1().x()

    def objectY1(self):
        """
        TOWRITE

        :return: TOWRITE
        :rtype: float
        """
        return self.objectEndPoint1().y()

    def objectX2(self):
        """
        TOWRITE

        :return: TOWRITE
        :rtype: float
        """
        return self.objectEndPoint2().x()

    def objectY2(self):
        """
        TOWRITE

        :return: TOWRITE
        :rtype: float
        """
        return self.objectEndPoint2().y()

    def objectDeltaX(self):
        """
        TOWRITE

        :return: TOWRITE
        :rtype: float
        """
        return (self.objectX2() - self.objectX1())

    def objectDeltaY(self):
        """
        TOWRITE

        :return: TOWRITE
        :rtype: float
        """
        return (self.objectY2() - self.objectY1())

    def objectLength(self):
        """
        TOWRITE

        :return: TOWRITE
        :rtype: float
        """
        return self.line().length()

    def setObjectX1(self, x):
        """
        TOWRITE

        :param `x`: TOWRITE
        :type `x`: float
        """
        self.setObjectEndPoint1(x, self.objectY1())

    def setObjectY1(self, y):
        """
        TOWRITE

        :param `y`: TOWRITE
        :type `y`: float
        """
        self.setObjectEndPoint1(self.objectX1(), y)

    def setObjectX2(self, x):
        """
        TOWRITE

        :param `x`: TOWRITE
        :type `x`: float
        """
        self.setObjectEndPoint2(x, self.objectY2())

    def setObjectY2(self, y):
        """
        TOWRITE

        :param `y`: TOWRITE
        :type `y`: float
        """
        self.setObjectEndPoint2(self.objectX2(), y)

# kate: bom off; indent-mode python; indent-width 4; replace-trailing-space-save on;
