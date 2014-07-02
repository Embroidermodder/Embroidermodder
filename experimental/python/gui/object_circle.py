#!/usr/bin/env python
# -*- coding: utf-8 -*-

"""
=================================
|module_summary| object_circle.py
=================================

TOWRITE


Classes summary:
================

============================ ============================
:class:`~CircleObject`       TOWRITE
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
    from PySide.QtGui import QGraphicsItem, QPainter, QPainterPath, QStyle, QTransform
    PYSIDE = True
    PYQT4 = False
except ImportError:
    raise
#    ## from PyQt4 import QtCore, QtGui
#    # or... Improve performace with less dots...
#    from PyQt4.QtCore import qDebug, Qt, QLineF, QPointF
#    from PyQt4.QtGui import QGraphicsItem, QPainter, QPainterPath, QStyle, QTransform
#    PYSIDE = False
#    PYQT4 = True

#--Local Imports.
from object_base import BaseObject

# C++C++C++C++C++C++C++C++C++C++C++C++C++C++C++C++C++C++C++C++C++C++C++C++C++
#include "object-circle.h"
#include "object-data.h"
#include "geom-arc.h" //TODO: make a geom-circle.h that simply includes arc.h so it's more intuitive

#include <QPainter>
#include <QStyleOption>
#include <QGraphicsScene>
# C++C++C++C++C++C++C++C++C++C++C++C++C++C++C++C++C++C++C++C++C++C++C++C++C++


class CircleObject(BaseObject):
    """
    Subclass of `BaseObject`_

    TOWRITE

    """
    def __init__(self, centerX, centerY, radius, rgb, parent):
        #OVERLOADED IMPL?# CircleObject::CircleObject(CircleObject* obj, QGraphicsItem* parent) : BaseObject(parent)
        """
        Default class constructor.

        :param `centerX`: TOWRITE
        :type `centerX`: qreal
        :param `centerY`: TOWRITE
        :type `centerY`: qreal
        :param `radius`: TOWRITE
        :type `radius`: qreal
        :param `rgb`: TOWRITE
        :type `rgb`: QRgb
        :param `parent`: TOWRITE
        :type `parent`: `QGraphicsItem`_
        """
        super(CircleObject, self).__init__(parent)

        qDebug("CircleObject Constructor()")
        self.init(centerX, centerY, radius, rgb, Qt.SolidLine)  # TODO: getCurrentLineType

        #OVERLOADED IMPL?# if obj:
        #OVERLOADED IMPL?#     self.init(obj.objectCenterX(), obj.objectCenterY(), obj.objectRadius(), obj.objectColorRGB(), Qt.SolidLine) # TODO: getCurrentLineType
        #OVERLOADED IMPL?#     self.setRotation(obj.rotation())


    def __del__(self):
        """Class destructor."""
        qDebug("CircleObject Destructor()")

    def init(self, centerX, centerY, radius, rgb, lineType):
        """
        TOWRITE

        :param `centerX`: TOWRITE
        :type `centerX`: qreal
        :param centerY`: TOWRITE
        :type `centerY`: qreal
        :param `radius`: TOWRITE
        :type `radius`: qreal
        :param `rgb`: TOWRITE
        :type `rgb`: QRgb
        :param `lineType`: TOWRITE
        :type `lineType`: Qt.PenStyle
        """
        self.setData(OBJ_TYPE, type())
        self.setData(OBJ_NAME, OBJ_NAME_CIRCLE)

        # WARNING: DO NOT enable QGraphicsItem::ItemIsMovable. If it is enabled,
        # WARNING: and the item is double clicked, the scene will erratically move the item while zooming.
        # WARNING: All movement has to be handled explicitly by us, not by the scene.
        self.setFlag(QGraphicsItem.ItemIsSelectable, True)

        self.setObjectRadius(radius)
        self.setObjectCenter(centerX, centerY)
        self.setObjectColor(rgb)
        self.setObjectLineType(lineType)
        self.setObjectLineWeight(0.35)  # TODO: pass in proper lineweight
        self.setPen(objectPen())
        self.updatePath()


    def setObjectCenter(self, center):
        """
        TOWRITE

        :param `center`: TOWRITE
        :type `center`: `QPointF`_
        """
        self.setObjectCenter(center.x(), center.y())

    def setObjectCenter(self, centerX, centerY):
        """
        TOWRITE

        :param `centerX`: TOWRITE
        :type `centerX`: qreal
        :param `centerY`: TOWRITE
        :type `centerY`: qreal
        """
        self.setPos(centerX, centerY)

    def setObjectCenterX(self, centerX):
        """
        TOWRITE

        :param `centerX`: TOWRITE
        :type `centerX`: qreal
        """
        self.setX(centerX)

    def setObjectCenterY(self, centerY):
        """
        TOWRITE

        :param `centerY`: TOWRITE
        :type `centerY`: qreal
        """
        self.setY(centerY)

    def setObjectRadius(self, radius):
        """
        TOWRITE

        :param `radius`: TOWRITE
        :type `radius`: qreal
        """
        self.setObjectDiameter(radius * 2.0)

    def setObjectDiameter(self, diameter):
        """
        TOWRITE

        :param `diameter`: TOWRITE
        :type `diameter`: qreal
        """
        circRect = QRectF()
        circRect.setWidth(diameter)
        circRect.setHeight(diameter)
        circRect.moveCenter(QPointF(0, 0))
        self.setRect(circRect)
        self.updatePath()

    def setObjectArea(self, area):
        """
        TOWRITE

        :param `area`: TOWRITE
        :type `area`: qreal
        """
        radius = qSqrt(area / pi())  # qreal
        self.setObjectRadius(radius)

    def setObjectCircumference(self, circumference):
        """
        TOWRITE

        :param `circumference`: TOWRITE
        :type `circumference`: qreal
        """
        diameter = circumference / pi()  # qreal
        self.setObjectDiameter(diameter)

    def updatePath(self):
        """
        TOWRITE
        """
        path = QPainterPath()
        r = rect()  # QRectF
        # Add the center point.
        path.addRect(-0.00000001, -0.00000001, 0.00000002, 0.00000002)
        # Add the circle.
        path.arcMoveTo(r, 0)
        path.arcTo(r, 0, 360)
        # NOTE: Reverse the path so that the inside area isn't considered part of the circle.
        path.arcTo(r, 0, -360)
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
        updateRubber(painter)
        if option.state & QStyle.State_Selected:
            paintPen.setStyle(Qt.DashLine)
        if objScene.property(ENABLE_LWT).toBool():
            paintPen = self.lineWeightPen()
        painter.setPen(paintPen)

        painter.drawEllipse(self.rect())

    def updateRubber(self, painter):
        """
        TOWRITE

        :param `painter`: TOWRITE
        :type `painter`: `QPainter`_
        """
        rubberMode = self.objectRubberMode()  # int
        if rubberMode == OBJ_RUBBER_CIRCLE_1P_RAD:

            sceneCenterPoint = self.objectRubberPoint("CIRCLE_CENTER")  # QPointF
            sceneQSnapPoint = self.objectRubberPoint("CIRCLE_RADIUS")   # QPointF
            itemCenterPoint = self.mapFromScene(sceneCenterPoint)       # QPointF
            itemQSnapPoint = self.mapFromScene(sceneQSnapPoint)         # QPointF
            itemLine = QLineF(itemCenterPoint, itemQSnapPoint)
            self.setObjectCenter(sceneCenterPoint)
            sceneLine = QLineF(sceneCenterPoint, sceneQSnapPoint)
            radius = sceneLine.length()  # qreal
            self.setObjectRadius(radius)
            if painter:
                self.drawRubberLine(itemLine, painter, VIEW_COLOR_CROSSHAIR)
            self.updatePath()

        elif rubberMode == OBJ_RUBBER_CIRCLE_1P_DIA:

            sceneCenterPoint = self.objectRubberPoint("CIRCLE_CENTER")   # QPointF
            sceneQSnapPoint = self.objectRubberPoint("CIRCLE_DIAMETER")  # QPointF
            itemCenterPoint = self.mapFromScene(sceneCenterPoint)        # QPointF
            itemQSnapPoint = self.mapFromScene(sceneQSnapPoint)          # QPointF
            itemLine = QLineF(itemCenterPoint, itemQSnapPoint)
            self.setObjectCenter(sceneCenterPoint)
            sceneLine = QLineF(sceneCenterPoint, sceneQSnapPoint)
            diameter = sceneLine.length()  # qreal
            self.setObjectDiameter(diameter)
            if painter:
                self.drawRubberLine(itemLine, painter, VIEW_COLOR_CROSSHAIR)
            self.updatePath()

        elif rubberMode == OBJ_RUBBER_CIRCLE_2P:

            sceneTan1Point = self.objectRubberPoint("CIRCLE_TAN1")   # QPointF
            sceneQSnapPoint = self.objectRubberPoint("CIRCLE_TAN2")  # QPointF
            sceneLine = QLineF(sceneTan1Point, sceneQSnapPoint)
            self.setObjectCenter(sceneLine.pointAt(0.5))
            diameter = sceneLine.length()  # qreal
            self.setObjectDiameter(diameter)
            self.updatePath()

        elif rubberMode == OBJ_RUBBER_CIRCLE_3P:

            sceneTan1Point = self.objectRubberPoint("CIRCLE_TAN1")  # QPointF
            sceneTan2Point = self.objectRubberPoint("CIRCLE_TAN2")  # QPointF
            sceneTan3Point = self.objectRubberPoint("CIRCLE_TAN3")  # QPointF

            #TODO/PORT# double sceneCenterX
            #TODO/PORT# double sceneCenterY
            #TODO/PORT# getArcCenter(sceneTan1Point.x(), sceneTan1Point.y(),
            #TODO/PORT#              sceneTan2Point.x(), sceneTan2Point.y(),
            #TODO/PORT#              sceneTan3Point.x(), sceneTan3Point.y(),
            #TODO/PORT#              &sceneCenterX, &sceneCenterY)
            sceneCenterPoint = QPointF(sceneCenterX, sceneCenterY)
            sceneLine = QLineF(sceneCenterPoint, sceneTan3Point)
            self.setObjectCenter(sceneCenterPoint)
            radius = sceneLine.length()  # qreal
            self.setObjectRadius(radius)
            self.updatePath()

        elif rubberMode == OBJ_RUBBER_GRIP:

            if painter:

                gripPoint = objectRubberPoint("GRIP_POINT")  # QPointF
                if gripPoint == objectCenter():
                    painter.drawEllipse(self.rect().translated(self.mapFromScene(self.objectRubberPoint('')) - self.mapFromScene(gripPoint)))

                else:
                    gripRadius = QLineF(self.objectCenter(), self.objectRubberPoint('')).length()  # qreal
                    painter.drawEllipse(QPointF(), gripRadius, gripRadius)

                rubLine = QLineF(self.mapFromScene(gripPoint), self.mapFromScene(objectRubberPoint('')))
                self.drawRubberLine(rubLine, painter, VIEW_COLOR_CROSSHAIR)

    def vulcanize(self):
        """
        TOWRITE
        """
        qDebug("CircleObject vulcanize()")
        self.updateRubber()

        self.setObjectRubberMode(OBJ_RUBBER_OFF)

    def mouseSnapPoint(self, mousePoint):
        """
        Returns the closest snap point to the mouse point.

        :param `mousePoint`: TOWRITE
        :type `mousePoint`: `QPointF`_
        :rtype: `QPointF`_
        """
        center  = objectCenter()       # QPointF
        quad0   = objectQuadrant0()    # QPointF
        quad90  = objectQuadrant90()   # QPointF
        quad180 = objectQuadrant180()  # QPointF
        quad270 = objectQuadrant270()  # QPointF

        cntrDist = QLineF(mousePoint, center).length()   # qreal
        q0Dist   = QLineF(mousePoint, quad0).length()    # qreal
        q90Dist  = QLineF(mousePoint, quad90).length()   # qreal
        q180Dist = QLineF(mousePoint, quad180).length()  # qreal
        q270Dist = QLineF(mousePoint, quad270).length()  # qreal

        minDist = qMin(qMin(qMin(q0Dist, q90Dist), qMin(q180Dist, q270Dist)), cntrDist)  # qreal

        if   minDist == cntrDist: return center
        elif minDist == q0Dist:   return quad0
        elif minDist == q90Dist:  return quad90
        elif minDist == q180Dist: return quad180
        elif minDist == q270Dist: return quad270

        return self.scenePos()

    def allGripPoints(self):
        """
        TOWRITE

        :rtype: QList<QPointF>
        """
        # QList<QPointF> gripPoints;
        # gripPoints << objectCenter() << objectQuadrant0() << objectQuadrant90() << objectQuadrant180() << objectQuadrant270();
        gripPoints = list(self.objectCenter() + self.objectQuadrant0() + self.objectQuadrant90() + self.objectQuadrant180() + self.objectQuadrant270())  # TODO: Check if this would be right...
        return gripPoints

    def gripEdit(self, before, after):
        """
        TOWRITE

        :param `before`: TOWRITE
        :type `before`: `QPointF`_
        :param `after`: TOWRITE
        :type `after`: `QPointF`_
        """
        if before == self.objectCenter():
            delta = QPointF(after-before)
            self.moveBy(delta.x(), delta.y())
        else:
            self.setObjectRadius(QLineF(self.objectCenter(), after).length())

    def objectSavePath(self):
        """
        TOWRITE

        :rtype: `QPainterPath`_
        """
        path = QPainterPath()
        r = self.rect()  # QRectF
        path.arcMoveTo(r, 0)
        path.arcTo(r, 0, 360)

        s = self.scale()  # qreal
        trans = QTransform()
        trans.rotate(rotation())
        trans.scale(s, s)
        return trans.map(path)


# kate: bom off; indent-mode python; indent-width 4; replace-trailing-space-save on;
