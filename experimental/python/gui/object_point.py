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
#include "object-point.h"
#include "object-data.h"

#include <QPainter>
#include <QStyleOption>
#include <QGraphicsScene>
# C++C++C++C++C++C++C++C++C++C++C++C++C++C++C++C++C++C++C++C++C++C++C++C++C++


class PointObject(BaseObject):
    """
    Subclass of `BaseObject`

    TOWRITE

    .. sphinx_generate_methods_summary::
       PointObject
    """
    def __init__(self, x, y, rgb, parent):
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

        qDebug("PointObject Constructor()");
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
        self.setData(OBJ_TYPE, type())
        self.setData(OBJ_NAME, OBJ_NAME_POINT)

        # WARNING: DO NOT enable QGraphicsItem::ItemIsMovable. If it is enabled,
        # WARNING: and the item is double clicked, the scene will erratically move the item while zooming.
        # WARNING: All movement has to be handled explicitly by us, not by the scene.
        self.setFlag(QGraphicsItem.ItemIsSelectable, True)

        self.setRect(-0.00000001, -0.00000001, 0.00000002, 0.00000002)
        self.setObjectPos(x, y)
        self.setObjectColor(rgb)
        self.setObjectLineType(lineType)
        self.setObjectLineWeight(0.35)  # TODO: pass in proper lineweight
        self.setPen(objectPen())

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
        updateRubber(painter)
        if option.state & QStyle.State_Selected:
            paintPen.setStyle(Qt.DashLine)
        if objScene.property(ENABLE_LWT).toBool():
            paintPen = lineWeightPen()
        painter.setPen(paintPen)

        painter.drawPoint(0, 0)

    def updateRubber(self, painter):
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
                    self.drawRubberLine(rubLine, painter, VIEW_COLOR_CROSSHAIR)

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
        gripPoints = list(self.scenePos())  # TODO: Check if this would be right...
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


# kate: bom off; indent-mode python; indent-width 4; replace-trailing-space-save on;
