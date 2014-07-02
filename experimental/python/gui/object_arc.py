#!/usr/bin/env python
# -*- coding: utf-8 -*-

"""
==============================
|module_summary| object_arc.py
==============================

TOWRITE


Classes summary:
================

============================ ============================
:class:`~ArcObject`          TOWRITE
============================ ============================

---------------------------------------------------------

|

"""

#-Imports.---------------------------------------------------------------------
#--Python Imports.
from math import sin as qSin
from math import cos as qCos
from math import radians

#--PySide/PyQt Imports.
try:
    ## from PySide import QtCore, QtGui
    # or... Improve performace with less dots...
    from PySide.QtCore import qDebug, Qt, QLineF, QPointF
    from PySide.QtGui import QGraphicsItem, QPainter, QPainterPath, QStyle
    PYSIDE = True
    PYQT4 = False
except ImportError:
    raise
#    ## from PyQt4 import QtCore, QtGui
#    # or... Improve performace with less dots...
#    from PyQt4.QtCore import qDebug, Qt, QLineF, QPointF
#    from PyQt4.QtGui import QGraphicsItem, QPainter, QPainterPath, QStyle
#    PYSIDE = False
#    PYQT4 = True

#--Local Imports.
from object_base import BaseObject

# C++C++C++C++C++C++C++C++C++C++C++C++C++C++C++C++C++C++C++C++C++C++C++C++C++
#include "object-arc.h"
#include "object-data.h"
#include "geom-arc.h"

#include <QPainter>
#include <QStyleOption>
#include <QGraphicsScene>
# C++C++C++C++C++C++C++C++C++C++C++C++C++C++C++C++C++C++C++C++C++C++C++C++C++


class ArcObject(BaseObject):
    """
    Subclass of `BaseObject`_

    TOWRITE

    """
    def __init__(self, startX, startY, midX, midY, endX, endY, rgb, parent):
        #OVERLOADED IMPL?# ArcObject::ArcObject(ArcObject* obj, QGraphicsItem* parent) : BaseObject(parent)
        """
        Default class constructor.

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
        :param `rgb`: TOWRITE
        :type `rgb`: QRgb
        :param `parent`: TOWRITE
        :type `parent`: `QGraphicsItem`_
        """
        super(ArcObject, self).__init__(parent)

        qDebug("ArcObject Constructor()")
        self.init(startX, startY, midX, midY, endX, endY, rgb, Qt.SolidLine)  # TODO: getCurrentLineType

        #OVERLOADED IMPL?# if obj:
        #OVERLOADED IMPL?#     self.init(obj.objectStartX(), obj.objectStartY(), obj.objectMidX(), obj.objectMidY(), obj.objectEndX(), obj.objectEndY(), obj.objectColorRGB(), Qt.SolidLine)  # TODO: getCurrentLineType
        #OVERLOADED IMPL?#     self.setRotation(obj.rotation())


    def __del__(self):
        """Class destructor."""
        qDebug("ArcObject Destructor()")

    def init(self, startX, startY, midX, midY, endX, endY, rgb, lineType):
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
        :param `rgb`: TOWRITE
        :type `rgb`: QRgb
        :param `lineType`: TOWRITE
        :type `lineType`: Qt.PenStyle
        """
        self.setData(OBJ_TYPE, self.type())
        self.setData(OBJ_NAME, OBJ_NAME_ARC)

        # WARNING: DO NOT enable QGraphicsItem::ItemIsMovable. If it is enabled,
        # WARNING: and the item is double clicked, the scene will erratically move the item while zooming.
        # WARNING: All movement has to be handled explicitly by us, not by the scene.
        self.setFlag(QGraphicsItem.ItemIsSelectable, True)

        self.calculateArcData(startX, startY, midX, midY, endX, endY)

        self.setObjectColor(rgb)
        self.setObjectLineType(lineType)
        self.setObjectLineWeight(0.35)  # TODO: pass in proper lineweight
        self.setPen(self.objectPen())

    def calculateArcData(self, startX, startY, midX, midY, endX, endY):
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
        """
        #TODO/PORT# double centerX;
        #TODO/PORT# double centerY;
        #TODO/PORT# getArcCenter(startX,  startY,
        #TODO/PORT#              midX,    midY,
        #TODO/PORT#              endX,    endY,
        #TODO/PORT#              &centerX, &centerY);

        arcStartPoint = QPointF(startX - centerX, startY - centerY)
        arcMidPoint   = QPointF(midX   - centerX, midY   - centerY)
        arcEndPoint   = QPointF(endX   - centerX, endY   - centerY)

        self.setPos(centerX, centerY)

        radius = QLineF(centerX, centerY, midX, midY).length()  # qreal
        self.updateArcRect(radius)
        self.updatePath()
        self.setRotation(0)
        self.setScale(1)

    def updateArcRect(self, radius):
        """
        TOWRITE

        :param `radius`: TOWRITE
        :type `radius`: qreal
        """
        arcRect = QRectF()
        arcRect.setWidth(radius * 2.0)
        arcRect.setHeight(radius * 2.0)
        arcRect.moveCenter(QPointF(0, 0))
        self.setRect(arcRect)

    def setObjectCenter(self, point):
        """
        TOWRITE

        :param `point`: TOWRITE
        :type `point`: `QPointF`_
        """
        self.setObjectCenter(point.x(), point.y())

    def setObjectCenter(self, pointX, pointY):
        """
        TOWRITE

        :param `pointX`: TOWRITE
        :type `pointX`: qreal
        :param `pointY`: TOWRITE
        :type `pointY`: qreal
        """
        self.setPos(pointX, pointY)

    def setObjectCenterX(self, pointX):
        """
        TOWRITE

        :param `pointX`: TOWRITE
        :type `pointX`: qreal
        """
        self.setX(pointX)

    def setObjectCenterY(self, pointY):
        """
        TOWRITE

        :param `pointY`: TOWRITE
        :type `pointY`: qreal
        """
        self.setY(pointY)

    def setObjectRadius(self, radius):
        """
        TOWRITE

        :param `radius`: TOWRITE
        :type `radius`: qreal
        """
        # qreal rad;
        if radius <= 0:
            rad = 0.0000001
        else:
            rad = radius

        center = self.scenePos()  # QPointF
        startLine = QLineF(center, self.objectStartPoint())  # QLineF
        midLine   = QLineF(center, self.objectMidPoint())    # QLineF
        endLine   = QLineF(center, self.objectEndPoint())    # QLineF
        startLine.setLength(rad)
        midLine.setLength(rad)
        endLine.setLength(rad)
        arcStartPoint = startLine.p2()
        arcMidPoint = midLine.p2()
        arcEndPoint = endLine.p2()

        self.calculateArcData(arcStartPoint.x(), arcStartPoint.y(), arcMidPoint.x(), arcMidPoint.y(), arcEndPoint.x(), arcEndPoint.y())

    def setObjectStartAngle(self, angle):
        """
        TOWRITE

        :param `angle`: TOWRITE
        :type `angle`: qreal
        """
        pass # TODO: ArcObject setObjectStartAngle

    def setObjectEndAngle(self, angle):
        """
        TOWRITE

        :param `angle`: TOWRITE
        :type `angle`: qreal
        """
        pass # TODO: ArcObject setObjectEndAngle

    def setObjectStartPoint(self, point):
        """
        TOWRITE

        :param `point`: TOWRITE
        :type `point`: `QPointF`_
        """
        self.setObjectStartPoint(point.x(), point.y())

    def setObjectStartPoint(self, pointX, pointY):
        """
        TOWRITE

        :param `pointX`: TOWRITE
        :type `pointX`: qreal
        :param `pointY`: TOWRITE
        :type `pointY`: qreal
        """
        self.calculateArcData(pointX, pointY, arcMidPoint.x(), arcMidPoint.y(), arcEndPoint.x(), arcEndPoint.y())

    def setObjectMidPoint(self, point):
        """
        TOWRITE

        :param `point`: TOWRITE
        :type `point`: `QPointF`_
        """
        self.setObjectMidPoint(point.x(), point.y())

    def setObjectMidPoint(self, pointX, pointY):
        """
        TOWRITE

        :param `pointX`: TOWRITE
        :type `pointX`: qreal
        :param `pointY`: TOWRITE
        :type `pointY`: qreal
        """
        self.calculateArcData(arcStartPoint.x(), arcStartPoint.y(), pointX, pointY, arcEndPoint.x(), arcEndPoint.y())

    def setObjectEndPoint(self, point):
        """
        TOWRITE

        :param `point`: TOWRITE
        :type `point`: `QPointF`_
        """
        setObjectEndPoint(point.x(), point.y());

    def setObjectEndPoint(self, pointX, pointY):
        """
        TOWRITE

        :param `pointX`: TOWRITE
        :type `pointX`: qreal
        :param `pointY`: TOWRITE
        :type `pointY`: qreal
        """
        self.calculateArcData(arcStartPoint.x(), arcStartPoint.y(), arcMidPoint.x(), arcMidPoint.y(), pointX, pointY)

    def objectStartAngle(self):
        """
        TOWRITE

        :rtype: qreal
        """
        angle = QLineF(self.scenePos(), self.objectStartPoint()).angle()  # qreal
        while angle >= 360.0:
            angle -= 360.0
        while angle < 0.0:
            angle += 360.0
        return angle

    def objectEndAngle(self):
        """
        TOWRITE

        :rtype: qreal
        """
        angle = QLineF(self.scenePos(), self.objectEndPoint()).angle()  # qreal
        while angle >= 360.0:
            angle -= 360.0
        while angle < 0.0:
            angle += 360.0
        return angle

    def objectStartPoint(self):
        """
        TOWRITE

        :rtype: `QPointF`_
        """
        rot = radians(self.rotation())        # qreal
        cosRot = qCos(rot)                    # qreal
        sinRot = qSin(rot)                    # qreal
        x = arcStartPoint.x() * self.scale()  # qreal
        y = arcStartPoint.y() * self.scale()  # qreal
        rotX = x * cosRot - y*sinRot          # qreal
        rotY = x * sinRot + y*cosRot          # qreal

        return (self.scenePos() + QPointF(rotX, rotY))

    def objectStartX(self):
        """
        TOWRITE

        :rtype: qreal
        """
        return self.objectStartPoint().x()

    def objectStartY(self):
        """
        TOWRITE

        :rtype: qreal
        """
        return self.objectStartPoint().y();

    def objectMidPoint(self):
        """
        TOWRITE

        :rtype: `QPointF`_
        """
        rot = radians(self.rotation())      # qreal
        cosRot = qCos(rot)                  # qreal
        sinRot = qSin(rot)                  # qreal
        x = arcMidPoint.x() * self.scale()  # qreal
        y = arcMidPoint.y() * self.scale()  # qreal
        rotX = x * cosRot - y * sinRot      # qreal
        rotY = x * sinRot + y * cosRot      # qreal

        return (self.scenePos() + QPointF(rotX, rotY))

    def objectMidX(self):
        """
        TOWRITE

        :rtype: qreal
        """
        return self.objectMidPoint().x()

    def objectMidY(self):
        """
        TOWRITE

        :rtype: qreal
        """
        return self.objectMidPoint().y()

    def objectEndPoint(self):
        """
        TOWRITE

        :rtype: `QPointF`_
        """
        rot = radians(self.rotation())      # qreal
        cosRot = qCos(rot)                  # qreal
        sinRot = qSin(rot)                  # qreal
        x = arcEndPoint.x() * self.scale()  # qreal
        y = arcEndPoint.y() * self.scale()  # qreal
        rotX = x * cosRot - y * sinRot      # qreal
        rotY = x * sinRot + y * cosRot      # qreal

        return (self.scenePos() + QPointF(rotX, rotY))

    def objectEndX(self):
        """
        TOWRITE

        :rtype: qreal
        """
        return self.objectEndPoint().x()

    def objectEndY(self):
        """
        TOWRITE

        :rtype: qreal
        """
        return self.objectEndPoint().y()

    def objectArea(self):
        """
        TOWRITE

        :rtype: qreal
        """
        # Area of a circular segment.
        r = self.objectRadius()                      # qreal
        theta = radians(self.objectIncludedAngle())  # qreal
        return ((r * r) / 2) * (theta - qSin(theta))

    def objectArcLength(self):
        """
        TOWRITE

        :rtype: qreal
        """
        return radians(self.objectIncludedAngle()) * self.objectRadius()

    def objectChord(self):
        """
        TOWRITE

        :rtype: qreal
        """
        return QLineF(self.objectStartX(), self.objectStartY(), self.objectEndX(), self.objectEndY()).length()

    def objectIncludedAngle(self):
        """
        TOWRITE

        :rtype: qreal
        """
        chord = self.objectChord()  # qreal
        rad = self.objectRadius()   # qreal
        if chord <= 0 or rad <= 0:
            return 0  # Prevents division by zero and non-existant circles.

        # NOTE: Due to floating point rounding errors, we need to clamp the quotient so it is in the range [-1, 1]
        #       If the quotient is out of that range, then the result of asin() will be NaN.
        quotient = chord / (2.0 * rad)  # qreal
        if quotient > 1.0:
            quotient = 1.0
        if quotient < 0.0:
            quotient = 0.0  # NOTE: 0 rather than -1 since we are enforcing a positive chord and radius
        return degrees(2.0 * asin(quotient))  # Properties of a Circle - Get the Included Angle - Reference: ASD9

    def objectClockwise(self):
        """
        TOWRITE

        :rtype: bool
        """
        # NOTE: Y values are inverted here on purpose
        if self.isArcClockwise(self.objectStartX(), -self.objectStartY(), self.objectMidX(), -self.objectMidY(), self.objectEndX(), -self.objectEndY()):
            return True
        return False

    def updatePath(self):
        """
        TOWRITE
        """
        startAngle = (self.objectStartAngle() + self.rotation())  # qreal
        spanAngle = self.objectIncludedAngle()                    # qreal

        if self.objectClockwise():
            spanAngle = -spanAngle

        path = QPainterPath()
        path.arcMoveTo(self.rect(), startAngle)
        path.arcTo(self.rect(), startAngle, spanAngle)
        # NOTE: Reverse the path so that the inside area isn't considered part of the arc
        path.arcTo(self.rect(), startAngle + spanAngle, -spanAngle);
        self.setObjectPath(path)

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
        if objScene.property(ENABLE_LWT).toBool():
            paintPen = self.lineWeightPen()
        painter.setPen(paintPen)

        startAngle = (self.objectStartAngle() + self.rotation()) * 16  # qreal
        spanAngle = self.objectIncludedAngle() * 16                    # qreal

        if self.objectClockwise():
            spanAngle = -spanAngle

        rad = self.objectRadius()  # qreal
        paintRect = QRectF(-rad, -rad, rad * 2.0, rad * 2.0)
        painter.drawArc(paintRect, startAngle, spanAngle)

    def updateRubber(self, painter):
        """
        TOWRITE

        :param `painter`: TOWRITE
        :type `painter`: `QPainter`_
        """
        pass # TODO: Arc Rubber Modes

        pass # TODO: updateRubber() gripping for ArcObject

    def vulcanize(self):
        """
        TOWRITE
        """
        qDebug("ArcObject vulcanize()")
        self.updateRubber()
        self.setObjectRubberMode(OBJ_RUBBER_OFF)

    def mouseSnapPoint(self, mousePoint):
        """
        Returns the closest snap point to the mouse point.

        :param `mousePoint`: TOWRITE
        :type `mousePoint`: `QPointF`_
        :rtype: `QPointF`_
        """
        center = self.objectCenter()      # QPointF
        start  = self.objectStartPoint()  # QPointF
        mid    = self.objectMidPoint()    # QPointF
        end    = self.objectEndPoint()    # QPointF

        cntrDist  = QLineF(mousePoint, center).length()  # qreal
        startDist = QLineF(mousePoint, start).length()   # qreal
        midDist   = QLineF(mousePoint, mid).length()     # qreal
        endDist   = QLineF(mousePoint, end).length()     # qreal

        minDist = qMin(qMin(cntrDist, startDist), qMin(midDist, endDist))  # qreal

        if   minDist == cntrDist:  return center
        elif minDist == startDist: return start
        elif minDist == midDist:   return mid
        elif minDist == endDist:   return end

        return self.scenePos()

    def allGripPoints(self):
        """
        TOWRITE

        :rtype: QList<QPointF>
        """
        # QList<QPointF> gripPoints;
        # gripPoints << objectCenter() << objectStartPoint() << objectMidPoint() << objectEndPoint();
        gripPoints = list(self.objectCenter() + self.objectStartPoint() + self.objectMidPoint() + self.objectEndPoint())
        return gripPoints

    def gripEdit(self, before, after):
        """
        TOWRITE

        :param `before`: TOWRITE
        :type `before`: `QPointF`_
        :param `after`: TOWRITE
        :type `after`: `QPointF`_

        .. TODO:: gripEdit() for ArcObject

        """
        pass # TODO: gripEdit() for ArcObject


# kate: bom off; indent-mode python; indent-width 4; replace-trailing-space-save on;
