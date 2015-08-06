#!/usr/bin/env python
# -*- coding: utf-8 -*-

"""
================================
|module_summary| object_point.py
================================

TOWRITE


Classes summary:
================

============================ ============================
:class:`~PointObject`        TOWRITE
============================ ============================

---------------------------------------------------------

|

"""

#-Imports.---------------------------------------------------------------------
#--PySide/PyQt Imports.
if PYSIDE:
    ## from PySide import QtCore, QtGui
    # or... Improve performace with less dots...
    from PySide.QtCore import qDebug, Qt, QLineF, QPointF
    from PySide.QtGui import QGraphicsItem, QPainter, QPainterPath, QStyle
elif PYQT4:
    import sip
    sip.setapi('QString', 2)
    sip.setapi('QVariant', 2)
    ## from PyQt4 import QtCore, QtGui
    # or... Improve performace with less dots...
    from PyQt4.QtCore import qDebug, Qt, QLineF, QPointF
    from PyQt4.QtGui import QGraphicsItem, QPainter, QPainterPath, QStyle

#--Local Imports.
from hacks import overloaded, signature
from object_base import BaseObject
from object_data import (OBJ_TYPE_POINT, OBJ_TYPE, OBJ_NAME, OBJ_NAME_POINT,
    ENABLE_LWT, OBJ_RUBBER_GRIP, OBJ_RUBBER_OFF)

# C++C++C++C++C++C++C++C++C++C++C++C++C++C++C++C++C++C++C++C++C++C++C++C++C++
#include "object-point.h"
#include "object-data.h"

#include <QPainter>
#include <QStyleOption>
#include <QGraphicsScene>
# C++C++C++C++C++C++C++C++C++C++C++C++C++C++C++C++C++C++C++C++C++C++C++C++C++


class PointObject(BaseObject):
    """
    Subclass of `BaseObject`_

    TOWRITE

    .. sphinx_generate_methods_summary::
       PointObject
    """

    Type = OBJ_TYPE_POINT

    def __init__(self, x, y, rgb, parent=None):
        #OVERLOADED IMPL?# PointObject::PointObject(PointObject* obj, QGraphicsItem* parent) : BaseObject(parent)
        """
        Default class constructor.

        :param `x`: TOWRITE
        :type `x`: qreal
        :param `y`: TOWRITE
        :type `y`: qreal
        :param `rgb`: TOWRITE
        :type `rgb`: QRgb
        :param `parent`: TOWRITE
        :type `parent`: `QGraphicsItem`_
        """
        super(PointObject, self).__init__(parent)

        qDebug("PointObject Constructor()")
        self.init(x, y, rgb, Qt.SolidLine)  # TODO: getCurrentLineType

        #OVERLOADED IMPL?# if obj:
        #OVERLOADED IMPL?#     self.init(obj.objectX(), obj.objectY(), obj.objectColorRGB(), Qt.SolidLine)  # TODO: getCurrentLineType
        #OVERLOADED IMPL?#     self.setRotation(obj.rotation())


    def __del__(self):
        """Class destructor."""
        qDebug("PointObject Destructor()")

    def init(self, x, y, rgb, lineType):
        """
        TOWRITE

        :param `x`: TOWRITE
        :type `x`: qreal
        :param y`: TOWRITE
        :type `y`: qreal
        :param `rgb`: TOWRITE
        :type `rgb`: QRgb
        :param `lineType`: TOWRITE
        :type `lineType`: Qt.PenStyle
        """
        self.setData(OBJ_TYPE, self.type())
        self.setData(OBJ_NAME, OBJ_NAME_POINT)

        # WARNING: DO NOT enable QGraphicsItem::ItemIsMovable. If it is enabled,
        # WARNING: and the item is double clicked, the scene will erratically move the item while zooming.
        # WARNING: All movement has to be handled explicitly by us, not by the scene.
        self.setFlag(QGraphicsItem.ItemIsSelectable, True)

        self.setRect(-0.00000001, -0.00000001, 0.00000002, 0.00000002)
        self.setObjectPos(x, y)
        self.setObjectColorRGB(rgb)
        self.setObjectLineType(lineType)
        self.setObjectLineWeight(0.35)  # TODO: pass in proper lineweight
        self.setPen(self.objectPen())

    def paint(self, painter, option, widget):
        """
        TOWRITE

        :param `painter`: TOWRITE
        :type `painter`: `QPainter`_
        :param option`: TOWRITE
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

        painter.drawPoint(0, 0)

    def updateRubber(self, painter=None):
        """
        TOWRITE

        :param `painter`: TOWRITE
        :type `painter`: `QPainter`_
        """
        rubberMode = self.objectRubberMode()  # int
        if rubberMode == OBJ_RUBBER_GRIP:
            if painter:
                gripPoint = self.objectRubberPoint("GRIP_POINT")  # QPointF
                if gripPoint == self.scenePos():
                    rubLine = QLineF(self.mapFromScene(gripPoint), self.mapFromScene(self.objectRubberPoint('')))
                    self.drawRubberLine(rubLine, painter, "VIEW_COLOR_CROSSHAIR")

    def vulcanize(self):
        """
        TOWRITE
        """
        qDebug("PointObject vulcanize()")
        self.updateRubber()

        self.setObjectRubberMode(OBJ_RUBBER_OFF)

    def mouseSnapPoint(self, mousePoint):
        """
        Returns the closest snap point to the mouse point.

        :param `mousePoint`: TOWRITE
        :type `mousePoint`: `QPointF`_
        :rtype: `QPointF`_
        """
        return self.scenePos()

    def allGripPoints(self):
        """
        TOWRITE

        :rtype: QList<QPointF>
        """
        ## QList<QPointF> gripPoints;
        ## gripPoints << scenePos()
        gripPoints = [self.scenePos()]
        return gripPoints

    def gripEdit(self, before, after):
        """
        TOWRITE

        :param `before`: TOWRITE
        :type `before`: `QPointF`_
        :param `after`: TOWRITE
        :type `after`: `QPointF`_
        """
        if before == self.scenePos():
            delta = QPointF(after-before)
            self.moveBy(delta.x(), delta.y())

    def objectSavePath(self):
        """
        TOWRITE

        :rtype: `QPainterPath`_
        """
        path = QPainterPath()
        path.addRect(-0.00000001, -0.00000001, 0.00000002, 0.00000002)
        return path

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
