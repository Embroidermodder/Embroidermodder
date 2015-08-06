#!/usr/bin/env python
# -*- coding: utf-8 -*-

"""
===============================
|module_summary| object_path.py
===============================

TOWRITE


Classes summary:
================

============================ ============================
:class:`~PathObject`         TOWRITE
============================ ============================

---------------------------------------------------------

|

.. TODO:: make paths similar to polylines. Review and implement any missing functions/members.
"""

#-Imports.---------------------------------------------------------------------
#--PySide/PyQt Imports.
if PYSIDE:
    ## from PySide import QtCore, QtGui
    # or... Improve performace with less dots...
    from PySide.QtCore import qDebug, Qt, QLineF, QPointF, QObject
    from PySide.QtGui import (QGraphicsItem, QPainter, QPainterPath, QStyle,
        QMessageBox, QTransform)
elif PYQT4:
    import sip
    sip.setapi('QString', 2)
    sip.setapi('QVariant', 2)
    ## from PyQt4 import QtCore, QtGui
    # or... Improve performace with less dots...
    from PyQt4.QtCore import qDebug, Qt, QLineF, QPointF, QObject
    from PyQt4.QtGui import (QGraphicsItem, QPainter, QPainterPath, QStyle,
        QMessageBox, QTransform)

#--Local Imports.
from hacks import overloaded, signature
from object_base import BaseObject
from object_data import (OBJ_TYPE, OBJ_NAME, OBJ_NAME_PATH, ENABLE_LWT,
    OBJ_RUBBER_OFF)

# C++C++C++C++C++C++C++C++C++C++C++C++C++C++C++C++C++C++C++C++C++C++C++C++C++
#include "object-point.h"
#include "object-data.h"

#include <QPainter>
#include <QStyleOption>
#include <QGraphicsScene>
# C++C++C++C++C++C++C++C++C++C++C++C++C++C++C++C++C++C++C++C++C++C++C++C++C++


class PathObject(BaseObject):
    """
    Subclass of `BaseObject`

    TOWRITE

    .. sphinx_generate_methods_summary::
       PathObject
    """
    def __init__(self, x, y, p, rgb, parent=None):
        #OVERLOADED IMPL?# PathObject::PathObject(PathObject* obj, QGraphicsItem* parent) : BaseObject(parent)
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
        super(PathObject, self).__init__(parent)

        qDebug("PathObject Constructor()")

        self.normalPath = QPainterPath()

        self.init(x, y, p, rgb, Qt.SolidLine)  # TODO: getCurrentLineType

        #OVERLOADED IMPL?# if obj:
        #OVERLOADED IMPL?#     self.init(obj.objectX(), obj.objectY(), obj.objectCopyPath(), obj.objectColorRGB(), Qt.SolidLine)  # TODO: getCurrentLineType
        #OVERLOADED IMPL?#     self.setRotation(obj.rotation())
        #OVERLOADED IMPL?#     self.setScale(obj.scale())


    def __del__(self):
        """Class destructor."""
        qDebug("PathObject Destructor()")

    def init(self, x, y, p, rgb, lineType):
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
        :param `lineType`: TOWRITE
        :type `lineType`: Qt.PenStyle
        """
        self.setData(OBJ_TYPE, self.type())
        self.setData(OBJ_NAME, OBJ_NAME_PATH)

        # WARNING: DO NOT enable QGraphicsItem::ItemIsMovable. If it is enabled,
        # WARNING: and the item is double clicked, the scene will erratically move the item while zooming.
        # WARNING: All movement has to be handled explicitly by us, not by the scene.
        self.setFlag(QGraphicsItem.ItemIsSelectable, True)

        self.updatePath(p)
        self.setObjectPos(x, y)
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
        normalPath = p
        reversePath = normalPath.toReversed()  # QPainterPath
        reversePath.connectPath(normalPath)
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

        painter.drawPath(self.objectPath())

    def updateRubber(self, painter=None):
        """
        TOWRITE

        :param `painter`: TOWRITE
        :type `painter`: `QPainter`_
        """
        # TODO: Path Rubber Modes
        # TODO: updateRubber() gripping for PathObject
        pass

    def vulcanize(self):
        """
        TOWRITE
        """
        qDebug("PathObject vulcanize()")
        self.updateRubber()

        self.setObjectRubberMode(OBJ_RUBBER_OFF)

        if not self.normalPath.elementCount():
            QMessageBox.critical(0, QObject.tr("Empty Path Error"), QObject.tr("The path added contains no points. The command that created this object has flawed logic."))

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
        ## gripPoints << scenePos()  # TODO: loop thru all path Elements and return their points
        gripPoints = [self.scenePos()]
        return gripPoints

    def gripEdit(self, before, after):
        """
        TOWRITE

        :param `before`: TOWRITE
        :type `before`: `QPointF`_
        :param `after`: TOWRITE
        :type `after`: `QPointF`_

        .. TODO:: gripEdit() for PathObject

        """
        pass # TODO: gripEdit() for PathObject

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
