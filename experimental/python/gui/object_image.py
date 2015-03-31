#!/usr/bin/env python
# -*- coding: utf-8 -*-

"""
================================
|module_summary| object_image.py
================================

TOWRITE


Classes summary:
================

============================ ============================
:class:`~ImageObject`        TOWRITE
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
#include "object-image.h"
#include "object-data.h"

#include <QPainter>
#include <QStyleOption>
#include <QGraphicsScene>
# C++C++C++C++C++C++C++C++C++C++C++C++C++C++C++C++C++C++C++C++C++C++C++C++C++


class ImageObject(BaseObject):
    """
    Subclass of `BaseObject`

    TOWRITE

    .. sphinx_generate_methods_summary::
       ImageObject
    """
    def __init__(self, x, y, w, h, rgb, parent):
        #OVERLOADED IMPL?# ImageObject::ImageObject(ImageObject* obj, QGraphicsItem* parent) : BaseObject(parent)
        """
        Default class constructor.

        :param `x`: TOWRITE
        :type `x`: qreal
        :param `y`: TOWRITE
        :type `y`: qreal
        :param `w`: TOWRITE
        :type `w`: qreal
        :param `h`: TOWRITE
        :type `h`: qreal
        :param `rgb`: TOWRITE
        :type `rgb`: QRgb
        :param `parent`: TOWRITE
        :type `parent`: `QGraphicsItem`_
        """
        super(ImageObject, self).__init__(parent)

        qDebug("ImageObject Constructor()")
        self.init(x, y, w, h, rgb, Qt.SolidLine)  # TODO: getCurrentLineType

        #OVERLOADED IMPL?# if obj:
        #OVERLOADED IMPL?#     ptl = obj.objectTopLeft()  # QPointF
        #OVERLOADED IMPL?#     self.init(ptl.x(), ptl.y(), obj.objectWidth(), obj.objectHeight(), obj.objectColorRGB(), Qt.SolidLine)  # TODO: getCurrentLineType
        #OVERLOADED IMPL?#     self.setRotation(obj.rotation())


    def __del__(self):
        """Class destructor."""
        qDebug("ImageObject Destructor()")

    def init(self, x, y, w, h, rgb, lineType):
        """
        TOWRITE

        :param `x`: TOWRITE
        :type `x`: qreal
        :param y`: TOWRITE
        :type `y`: qreal
        :param `w`: TOWRITE
        :type `w`: qreal
        :param `h`: TOWRITE
        :type `h`: qreal
        :param `rgb`: TOWRITE
        :type `rgb`: QRgb
        :param `lineType`: TOWRITE
        :type `lineType`: Qt.PenStyle
        """
        self.setData(OBJ_TYPE, type())
        self.setData(OBJ_NAME, OBJ_NAME_IMAGE)

        # WARNING: DO NOT enable QGraphicsItem::ItemIsMovable. If it is enabled,
        # WARNING: and the item is double clicked, the scene will erratically move the item while zooming.
        # WARNING: All movement has to be handled explicitly by us, not by the scene.
        self.setFlag(QGraphicsItem.ItemIsSelectable, True)

        self.setObjectRect(x, y, w, h)
        self.setObjectColor(rgb)
        self.setObjectLineType(lineType)
        self.setObjectLineWeight(0.35)  # TODO: pass in proper lineweight
        self.setPen(self.objectPen())

    def setObjectRect(self, x, y, w, h):
        """
        TOWRITE

        :param `x`: TOWRITE
        :type `x`: qreal
        :param y`: TOWRITE
        :type `y`: qreal
        :param `w`: TOWRITE
        :type `w`: qreal
        :param `h`: TOWRITE
        :type `h`: qreal
        """
        self.setPos(x, y)
        self.setRect(0, 0, w, h)
        self.updatePath()

    def objectTopLeft(self):
        """
        TOWRITE

        :rtype: `QPointF`_
        """
        rot = radians(rotation())  # qreal
        cosRot = qCos(rot)         # qreal
        sinRot = qSin(rot)         # qreal

        tl = self.rect().topLeft()  # QPointF
        ptlX = tl.x() * self.scale()             # qreal
        ptlY = tl.y() * self.scale()             # qreal
        ptlXrot = ptlX * cosRot - ptlY * sinRot  # qreal
        ptlYrot = ptlX * sinRot + ptlY * cosRot  # qreal

        return (self.scenePos() + QPointF(ptlXrot, ptlYrot))

    def objectTopRight(self):
        """
        TOWRITE

        :rtype: `QPointF`_
        """
        rot = radians(rotation())  # qreal
        cosRot = qCos(rot)         # qreal
        sinRot = qSin(rot)         # qreal

        tr = self.rect().topRight()  # QPointF
        ptrX = tr.x() * self.scale()             # qreal
        ptrY = tr.y() * self.scale()             # qreal
        ptrXrot = ptrX * cosRot - ptrY * sinRot  # qreal
        ptrYrot = ptrX * sinRot + ptrY * cosRot  # qreal

        return (self.scenePos() + QPointF(ptrXrot, ptrYrot))

    def objectBottomLeft(self):
        """
        TOWRITE

        :rtype: `QPointF`_
        """
        rot = radians(rotation())  # qreal
        cosRot = qCos(rot)         # qreal
        sinRot = qSin(rot)         # qreal

        bl = self.rect().bottomLeft()  # QPointF
        pblX = bl.x() * scale()                  # qreal
        pblY = bl.y() * scale()                  # qreal
        pblXrot = pblX * cosRot - pblY * sinRot  # qreal
        pblYrot = pblX * sinRot + pblY * cosRot  # qreal

        return (self.scenePos() + QPointF(pblXrot, pblYrot))

    def objectBottomRight(self):
        """
        TOWRITE

        :rtype: `QPointF`_
        """
        rot = radians(rotation())  # qreal
        cosRot = qCos(rot)         # qreal
        sinRot = qSin(rot)         # qreal

        br = self.rect().bottomRight()  # QPointF
        pbrX = br.x() * scale()                  # qreal
        pbrY = br.y() * scale()                  # qreal
        pbrXrot = pbrX * cosRot - pbrY * sinRot  # qreal
        pbrYrot = pbrX * sinRot + pbrY * cosRot  # qreal

        return (self.scenePos() + QPointF(pbrXrot, pbrYrot))

    def updatePath(self):
        """
        TOWRITE
        """
        path = QPainterPath()
        r = self.rect()  # QRectF
        path.moveTo(r.bottomLeft())
        path.lineTo(r.bottomRight())
        path.lineTo(r.topRight())
        path.lineTo(r.topLeft())
        path.lineTo(r.bottomLeft())
        # NOTE: Reverse the path so that the inside area isn't considered part of the rectangle
        path.lineTo(r.topLeft())
        path.lineTo(r.topRight())
        path.lineTo(r.bottomRight())
        path.moveTo(r.bottomLeft())
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

        painter.drawRect(self.rect())

    def updateRubber(self, painter):
        """
        TOWRITE

        :param `painter`: TOWRITE
        :type `painter`: `QPainter`_
        """
        rubberMode = self.objectRubberMode()  # int
        if rubberMode == OBJ_RUBBER_IMAGE:
            sceneStartPoint = self.objectRubberPoint("IMAGE_START")  # QPointF
            sceneEndPoint = self.objectRubberPoint("IMAGE_END")      # QPointF
            x = sceneStartPoint.x()                      # qreal
            y = sceneStartPoint.y()                      # qreal
            w = sceneEndPoint.x() - sceneStartPoint.x()  # qreal
            h = sceneEndPoint.y() - sceneStartPoint.y()  # qreal
            self.setObjectRect(x,y,w,h)
            self.updatePath()

        elif rubberMode == OBJ_RUBBER_GRIP:
            pass # TODO: updateRubber() gripping for ImageObject

    def vulcanize(self):
        """
        TOWRITE
        """
        qDebug("ImageObject vulcanize()")
        self.updateRubber()
        self.setObjectRubberMode(OBJ_RUBBER_OFF)

    def mouseSnapPoint(self, mousePoint):
        """
        Returns the closest snap point to the mouse point.

        :param `mousePoint`: TOWRITE
        :type `mousePoint`: `QPointF`_
        :rtype: `QPointF`_
        """
        ptl = self.objectTopLeft()      # QPointF # Top Left Corner QSnap
        ptr = self.objectTopRight()     # QPointF # Top Right Corner QSnap
        pbl = self.objectBottomLeft()   # QPointF # Bottom Left Corner QSnap
        pbr = self.objectBottomRight()  # QPointF # Bottom Right Corner QSnap

        ptlDist = QLineF(mousePoint, ptl).length()  # qreal
        ptrDist = QLineF(mousePoint, ptr).length()  # qreal
        pblDist = QLineF(mousePoint, pbl).length()  # qreal
        pbrDist = QLineF(mousePoint, pbr).length()  # qreal

        minDist = qMin(qMin(ptlDist, ptrDist), qMin(pblDist, pbrDist))  # qreal

        if   minDist == ptlDist: return ptl
        elif minDist == ptrDist: return ptr
        elif minDist == pblDist: return pbl
        elif minDist == pbrDist: return pbr

        return self.scenePos()

    def allGripPoints(self):
        """
        TOWRITE

        :rtype: QList<QPointF>
        """
        ## QList<QPointF> gripPoints;
        ## gripPoints << objectTopLeft() << objectTopRight() << objectBottomLeft() << objectBottomRight();
        gripPoints = list(self.objectTopLeft() + self.objectTopRight() + self.objectBottomLeft() + self.objectBottomRight())  # TODO: Check if this would be right...
        return gripPoints

    def gripEdit(self, before, after):
        """
        TOWRITE

        :param `before`: TOWRITE
        :type `before`: `QPointF`_
        :param `after`: TOWRITE
        :type `after`: `QPointF`_

        .. TODO:: gripEdit() for ImageObject

        """
        pass # TODO: gripEdit() for ImageObject


# kate: bom off; indent-mode python; indent-width 4; replace-trailing-space-save on;
