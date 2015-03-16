#!/usr/bin/env python
# -*- coding: utf-8 -*-

"""
==================================
|module_summary| object_ellipse.py
==================================

TOWRITE


Classes summary:
================

============================ ============================
:class:`~EllipseObject`      TOWRITE
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
#include "object-ellipse.h"
#include "object-data.h"

#include <QPainter>
#include <QStyleOption>
#include <QGraphicsScene>
# C++C++C++C++C++C++C++C++C++C++C++C++C++C++C++C++C++C++C++C++C++C++C++C++C++


class EllipseObject(BaseObject):
    """
    Subclass of `BaseObject`_

    TOWRITE

    """
    def __init__(self, centerX, centerY, width, height, rgb, parent):
        #OVERLOADED IMPL?# EllipseObject::EllipseObject(EllipseObject* obj, QGraphicsItem* parent) : BaseObject(parent)
        """
        Default class constructor.

        :param `centerX`: TOWRITE
        :type `centerX`: qreal
        :param `centerY`: TOWRITE
        :type `centerY`: qreal
        :param `width`: TOWRITE
        :type `width`: qreal
        :param `height`: TOWRITE
        :type `height`: qreal
        :param `rgb`: TOWRITE
        :type `rgb`: QRgb
        :param `parent`: TOWRITE
        :type `parent`: `QGraphicsItem`_
        """
        super(EllipseObject, self).__init__(parent)

        qDebug("EllipseObject Constructor()")
        self.init(centerX, centerY, width, height, rgb, Qt.SolidLine)  # TODO: getCurrentLineType

        #OVERLOADED IMPL?# if obj:
        #OVERLOADED IMPL?#     self.init(obj.objectCenterX(), obj.objectCenterY(), obj.objectWidth(), obj.objectHeight(), obj.objectColorRGB(), Qt.SolidLine)  # TODO: getCurrentLineType
        #OVERLOADED IMPL?#     self.setRotation(obj.rotation())


    def __del__(self):
        """Class destructor."""
        qDebug("EllipseObject Destructor()")

    def init(self, centerX, centerY, width, height, rgb, lineType):
        """
        TOWRITE

        :param `centerX`: TOWRITE
        :type `centerX`: qreal
        :param centerY`: TOWRITE
        :type `centerY`: qreal
        :param `width`: TOWRITE
        :type `width`: qreal
        :param `height`: TOWRITE
        :type `height`: qreal
        :param `rgb`: TOWRITE
        :type `rgb`: QRgb
        :param `lineType`: TOWRITE
        :type `lineType`: Qt.PenStyle
        """
        self.setData(OBJ_TYPE, self.type())
        self.setData(OBJ_NAME, OBJ_NAME_ELLIPSE)

        # WARNING: DO NOT enable QGraphicsItem::ItemIsMovable. If it is enabled,
        # WARNING: and the item is double clicked, the scene will erratically move the item while zooming.
        # WARNING: All movement has to be handled explicitly by us, not by the scene.
        self.setFlag(QGraphicsItem.ItemIsSelectable, True)

        self.setObjectSize(width, height)
        self.setObjectCenter(centerX, centerY)
        self.setObjectColor(rgb)
        self.setObjectLineType(lineType)
        self.setObjectLineWeight(0.35)  # TODO: pass in proper lineweight
        self.setPen(objectPen())
        self.updatePath()

    def setObjectSize(self, width, height):
        """
        TOWRITE

        :param `width`: TOWRITE
        :type `width`: qreal
        :param `height`: TOWRITE
        :type `height`: qreal
        """
        elRect = self.rect()  # QRectF
        elRect.setWidth(width)
        elRect.setHeight(height)
        elRect.moveCenter(QPointF(0, 0))
        self.setRect(elRect)

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
        :param centerY`: TOWRITE
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

        :param centerY`: TOWRITE
        :type `centerY`: qreal
        """
        self.setY(centerY)

    def setObjectRadiusMajor(self, radius):
        """
        TOWRITE

        :param radius`: TOWRITE
        :type `radius`: qreal
        """
        self.setObjectDiameterMajor(radius * 2.0)

    def setObjectRadiusMinor(self, radius):
        """
        TOWRITE

        :param radius`: TOWRITE
        :type `radius`: qreal
        """
        self.setObjectDiameterMinor(radius * 2.0)

    def setObjectDiameterMajor(self, diameter):
        """
        TOWRITE

        :param diameter`: TOWRITE
        :type `diameter`: qreal
        """
        elRect = self.rect()  # QRectF
        if elRect.width() > elRect.height():
            elRect.setWidth(diameter)
        else:
            elRect.setHeight(diameter)
        elRect.moveCenter(QPointF(0, 0))
        self.setRect(elRect)

    def setObjectDiameterMinor(self, diameter):
        """
        TOWRITE

        :param diameter`: TOWRITE
        :type `diameter`: qreal
        """
        elRect = self.rect()  # QRectF
        if elRect.width() < elRect.height():
            elRect.setWidth(diameter)
        else:
            elRect.setHeight(diameter)
        elRect.moveCenter(QPointF(0, 0))
        self.setRect(elRect)

    def objectQuadrant0(self):
        """
        TOWRITE

        :rtype: `QPointF`_
        """
        halfW = self.objectWidth() / 2.0  # qreal
        rot = radians(self.rotation())    # qreal
        x = halfW * qCos(rot)             # qreal
        y = halfW * qSin(rot)             # qreal
        return self.objectCenter() + QPointF(x, y)

    def objectQuadrant90(self):
        """
        TOWRITE

        :rtype: `QPointF`_
        """
        halfH = self.objectHeight() / 2.0      # qreal
        rot = radians(self.rotation() + 90.0)  # qreal
        x = halfH * qCos(rot)                  # qreal
        y = halfH * qSin(rot)                  # qreal
        return self.objectCenter() + QPointF(x, y)

    def objectQuadrant180(self):
        """
        TOWRITE

        :rtype: `QPointF`_
        """
        halfW = self.objectWidth() / 2.0        # qreal
        rot = radians(self.rotation() + 180.0)  # qreal
        x = halfW * qCos(rot)                   # qreal
        y = halfW * qSin(rot)                   # qreal
        return self.objectCenter() + QPointF(x, y)

    def objectQuadrant270(self):
        """
        TOWRITE

        :rtype: `QPointF`_
        """
        halfH = self.objectHeight() / 2.0       # qreal
        rot = radians(self.rotation() + 270.0)  # qreal
        x = halfH * qCos(rot)                   # qreal
        y = halfH * qSin(rot)                   # qreal
        return self.objectCenter() + QPointF(x, y)

    def updatePath(self):
        """
        TOWRITE
        """
        path = QPainterPath()
        r = self.rect()  # QRectF
        path.arcMoveTo(r, 0)
        path.arcTo(r, 0, 360)
        # NOTE: Reverse the path so that the inside area isn't considered part of the ellipse
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
        self.updateRubber(painter)
        if option.state & QStyle.State_Selected:
            paintPen.setStyle(Qt.DashLine)
        if objScene.property(ENABLE_LWT):  # .toBool()
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
        if rubberMode == OBJ_RUBBER_ELLIPSE_LINE:

            sceneLinePoint1 = self.objectRubberPoint("ELLIPSE_LINE_POINT1")  # QPointF
            sceneLinePoint2 = self.objectRubberPoint("ELLIPSE_LINE_POINT2")  # QPointF
            itemLinePoint1  = self.mapFromScene(sceneLinePoint1)             # QPointF
            itemLinePoint2  = self.mapFromScene(sceneLinePoint2)             # QPointF
            itemLine = QLineF(itemLinePoint1, itemLinePoint2)
            if painter:
                self.drawRubberLine(itemLine, painter, "VIEW_COLOR_CROSSHAIR")
            self.updatePath()

        elif rubberMode == OBJ_RUBBER_ELLIPSE_MAJORDIAMETER_MINORRADIUS:

            sceneAxis1Point1 = self.objectRubberPoint("ELLIPSE_AXIS1_POINT1")  # QPointF
            sceneAxis1Point2 = self.objectRubberPoint("ELLIPSE_AXIS1_POINT2")  # QPointF
            sceneCenterPoint = self.objectRubberPoint("ELLIPSE_CENTER")        # QPointF
            sceneAxis2Point2 = self.objectRubberPoint("ELLIPSE_AXIS2_POINT2")  # QPointF
            ellipseWidth = self.objectRubberPoint("ELLIPSE_WIDTH").x()  # qreal
            ellipseRot = self.objectRubberPoint("ELLIPSE_ROT").x()      # qreal

            # TODO: incorporate perpendicularDistance() into libcgeometry
            px = sceneAxis2Point2.x()  # qreal
            py = sceneAxis2Point2.y()  # qreal
            x1 = sceneAxis1Point1.x()  # qreal
            y1 = sceneAxis1Point1.y()  # qreal
            line = QLineF(sceneAxis1Point1, sceneAxis1Point2)
            norm = line.normalVector()  # QLineF
            dx = px - x1  # qreal
            dy = py - y1  # qreal
            norm.translate(dx, dy)
            iPoint = QPointF()
            norm.intersect(line, iPoint)
            ellipseHeight = QLineF(px, py, iPoint.x(), iPoint.y()).length() * 2.0  # qreal

            self.setObjectCenter(sceneCenterPoint)
            self.setObjectSize(ellipseWidth, ellipseHeight)
            self.setRotation(-ellipseRot)

            itemCenterPoint = self.mapFromScene(sceneCenterPoint)  # QPointF
            itemAxis2Point2 = self.mapFromScene(sceneAxis2Point2)  # QPointF
            itemLine = QLineF(itemCenterPoint, itemAxis2Point2)
            if painter:
                self.drawRubberLine(itemLine, painter, "VIEW_COLOR_CROSSHAIR")
            self.updatePath()

        elif rubberMode == OBJ_RUBBER_ELLIPSE_MAJORRADIUS_MINORRADIUS:

            sceneAxis1Point2 = self.objectRubberPoint("ELLIPSE_AXIS1_POINT2")  # QPointF
            sceneCenterPoint = self.objectRubberPoint("ELLIPSE_CENTER")        # QPointF
            sceneAxis2Point2 = self.objectRubberPoint("ELLIPSE_AXIS2_POINT2")  # QPointF
            ellipseWidth = self.objectRubberPoint("ELLIPSE_WIDTH").x()  # qreal
            ellipseRot = self.objectRubberPoint("ELLIPSE_ROT").x()      # qreal

            # TODO: incorporate perpendicularDistance() into libcgeometry
            px = sceneAxis2Point2.x()  # qreal
            py = sceneAxis2Point2.y()  # qreal
            x1 = sceneCenterPoint.x()  # qreal
            y1 = sceneCenterPoint.y()  # qreal
            line = QLineF(sceneCenterPoint, sceneAxis1Point2)
            norm = line.normalVector()  # QLineF
            dx = px - x1  # qreal
            dy = py - y1  # qreal
            norm.translate(dx, dy)
            iPoint = QPointF()
            norm.intersect(line, iPoint)
            ellipseHeight = QLineF(px, py, iPoint.x(), iPoint.y()).length() * 2.0  # qreal

            self.setObjectCenter(sceneCenterPoint)
            self.setObjectSize(ellipseWidth, ellipseHeight)
            self.setRotation(-ellipseRot)

            itemCenterPoint = self.mapFromScene(sceneCenterPoint)  # QPointF
            itemAxis2Point2 = self.mapFromScene(sceneAxis2Point2)  # QPointF
            itemLine = QLineF(itemCenterPoint, itemAxis2Point2)
            if painter:
                self.drawRubberLine(itemLine, painter, "VIEW_COLOR_CROSSHAIR")
            self.updatePath()

        elif rubberMode == OBJ_RUBBER_GRIP:

            pass # TODO: updateRubber() gripping for EllipseObject

    def vulcanize(self):
        """
        TOWRITE
        """
        qDebug("EllipseObject vulcanize()")
        self.updateRubber()
        self.setObjectRubberMode(OBJ_RUBBER_OFF)

    def mouseSnapPoint(self, mousePoint):
        """
        Returns the closest snap point to the mouse point.

        :param `mousePoint`: TOWRITE
        :type `mousePoint`: `QPointF`_
        :rtype: `QPointF`_
        """
        center  = self.objectCenter()       # QPointF
        quad0   = self.objectQuadrant0()    # QPointF
        quad90  = self.objectQuadrant90()   # QPointF
        quad180 = self.objectQuadrant180()  # QPointF
        quad270 = self.objectQuadrant270()  # QPointF

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
        ## QList<QPointF> gripPoints;
        ## gripPoints << objectCenter() << objectQuadrant0() << objectQuadrant90() << objectQuadrant180() << objectQuadrant270();
        gripPoints = list(self.objectCenter() + self.objectQuadrant0() + self.objectQuadrant90() + self.objectQuadrant180() + self.objectQuadrant270())  # TODO: Check if this would be right...
        return gripPoints

    def gripEdit(self, before, after):
        """
        TOWRITE

        :param `before`: TOWRITE
        :type `before`: `QPointF`_
        :param `after`: TOWRITE
        :type `after`: `QPointF`_

        .. TODO:: gripEdit() for EllipseObject

        """
        pass # TODO: gripEdit() for EllipseObject

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
        trans.rotate(self.rotation())
        trans.scale(s, s)
        return trans.map(path)


# kate: bom off; indent-mode python; indent-width 4; replace-trailing-space-save on;
