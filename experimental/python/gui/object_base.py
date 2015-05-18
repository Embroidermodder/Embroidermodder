#!/usr/bin/env python
# -*- coding: utf-8 -*-

"""
===============================
|module_summary| object_base.py
===============================

TOWRITE

Classes summary:
================

============================ ============================
:class:`~BaseObject`         TOWRITE
============================ ============================

---------------------------------------------------------

|

.. TODO:: Python Port not finished.

"""

#-Imports.---------------------------------------------------------------------
#--PySide/PyQt Imports.
if PYSIDE:
    ## from PySide import QtCore, QtGui
    # or... Improve performace with less dots...
    from PySide.QtCore import qDebug, Qt, QDateTime, QPointF, QLineF, QObject, QRectF
    from PySide.QtGui import (QGraphicsScene, QMessageBox, QGraphicsItem,
        QGraphicsPathItem, QColor, QPen, QBrush, QPainter, QMessageBox,
        QPainterPath, QPainterPathStroker, QLinearGradient, QGradient)
elif PYQT4:
    import sip
    sip.setapi('QString', 2)
    sip.setapi('QVariant', 2)
#    ## from PyQt4 import QtCore, QtGui
#    # or... Improve performace with less dots...
    from PyQt4.QtCore import qDebug, Qt, QDateTime, QPointF, QLineF, QObject, QRectF
    from PyQt4.QtGui import (QGraphicsScene, QMessageBox, QGraphicsItem,
        QGraphicsPathItem, QColor, QPen, QBrush, QPainter, QMessageBox,
        QPainterPath, QPainterPathStroker, QLinearGradient, QGradient)

#--Local Imports.
from hacks import overloaded, signature
from object_data import OBJ_TYPE_BASE, OBJ_LWT_BYLAYER, OBJ_LWT_BYBLOCK, OBJ_RUBBER_GRIP

# C++C++C++C++C++C++C++C++C++C++C++C++C++C++C++C++C++C++C++C++C++C++C++C++C++
#include "object-base.h"

#include <QDebug>
#include <QGraphicsScene>
#include <QMessageBox>
#include <QDateTime>
#include <QPainter>
# C++C++C++C++C++C++C++C++C++C++C++C++C++C++C++C++C++C++C++C++C++C++C++C++C++


class BaseObject(QGraphicsPathItem):
    """
    Subclass of `QGraphicsPathItem`_

    TOWRITE

    """

    Type = OBJ_TYPE_BASE

    def __init__(self, parent=None):
        """
        Default class constructor.

        :param `parent`: Pointer to a parent widget instance.
        :type `parent`: `QGraphicsItem`_
        """
        super(BaseObject, self).__init__(parent)

        qDebug("BaseObject Constructor()")

        self.objPen = QPen()        # QPen objPen;
        self.lwtPen = QPen()        # QPen lwtPen;
        self.objLine = QLineF()     # QLineF objLine;
        self.objRubberMode = int()  # int objRubberMode;
        self.objRubberPoints = {}   # QHash<QString, QPointF> objRubberPoints;
        self.objRubberTexts = {}    # QHash<QString, QString> objRubberTexts;
        self.objID = int()          # qint64 objID;

        self.objPen.setCapStyle(Qt.RoundCap)
        self.objPen.setJoinStyle(Qt.RoundJoin)
        self.lwtPen.setCapStyle(Qt.RoundCap)
        self.lwtPen.setJoinStyle(Qt.RoundJoin)

        self.objID = QDateTime.currentMSecsSinceEpoch()


    def __del__(self):
        """Class destructor."""
        qDebug("BaseObject Destructor()")

    def type(self):
        """
        TOWRITE

        :return: TOWRITE
        :rtype: int
        """
        return self.Type

    def setObjectColor(self, color):
        """
        TOWRITE

        :param `color`: TOWRITE
        :type `color`: `QColor`_
        """
        self.objPen.setColor(color)
        self.lwtPen.setColor(color)

    def setObjectColorRGB(self, rgb):
        """
        TOWRITE

        :param `rgb`: TOWRITE
        :type `rgb`: `QRgb`_
        """
        self.objPen.setColor(QColor(rgb))
        self.lwtPen.setColor(QColor(rgb))

    def setObjectLineType(self, lineType):
        """
        TOWRITE

        :param `rgb`: TOWRITE
        :type `rgb`: Qt.PenStyle
        """
        self.objPen.setStyle(lineType)
        self.lwtPen.setStyle(lineType)

    def setObjectLineWeight(self, lineWeight):
        """
        TOWRITE

        :param `lineWeight`: TOWRITE
        :type `lineWeight`: qreal
        """
        self.objPen.setWidthF(0)  # NOTE: The objPen will always be cosmetic

        if lineWeight < 0:
            if lineWeight == OBJ_LWT_BYLAYER:
                self.lwtPen.setWidthF(0.35)  # TODO: getLayerLineWeight
            elif lineWeight == OBJ_LWT_BYBLOCK:
                self.lwtPen.setWidthF(0.35)  # TODO: getBlockLineWeight
            else:
                QMessageBox.warning(0, QObject.tr("Error - Negative Lineweight"),
                                       QObject.tr("Lineweight: %f" % lineWeight))
                qDebug("Lineweight cannot be negative! Inverting sign.")
                self.lwtPen.setWidthF(-lineWeight)
        else:
            self.lwtPen.setWidthF(lineWeight)

    def objectRubberPoint(self, key):
        """
        TOWRITE

        :param `key`: TOWRITE
        :type `key`: QString
        :rtype: `QPointF`_
        """
        if key in self.objRubberPoints:       # if(objRubberTexts.contains(key))
            return self.objRubberPoints[key]  #     return objRubberTexts.value(key);

        gscene = self.scene()  # QGraphicsScene* gscene = scene()
        if gscene:
            return self.scene().property("SCENE_QSNAP_POINT")  # .toPointF()
        return QPointF()

    def objectRubberText(self, key):
        """
        TOWRITE

        :param `key`: TOWRITE
        :type `key`: QString
        :rtype: QString
        """
        if key in self.objRubberTexts:       # if(objRubberTexts.contains(key))
            return self.objRubberTexts[key]  #     return objRubberTexts.value(key);
        return ""  # QString()

    def boundingRect(self):
        """
        TOWRITE

        :rtype: `QRectF`_
        """
        # If gripped, force this object to be drawn even if it is offscreen
        if self.objectRubberMode() == OBJ_RUBBER_GRIP:
            return self.scene().sceneRect()
        return self.path().boundingRect()

    def drawRubberLine(self, rubLine, painter=None, colorFromScene=''):
        """
        TOWRITE

        :param `rubLine`: TOWRITE
        :type `rubLine`: `QLineF`_
        :param `painter`: TOWRITE
        :type `painter`: `QPainter`_
        :param `colorFromScene`: TOWRITE
        :type `colorFromScene`: str
        """
        if painter:
            objScene = self.scene()  # QGraphicsScene* objScene = scene();
            if not objScene:
                return
            colorPen = self.objPen  # QPen colorPen = objPen
            colorPen.setColor(QColor(objScene.property(colorFromScene)))  # .toUInt()))
            painter.setPen(colorPen)
            painter.drawLine(rubLine)
            painter.setPen(self.objPen)

    def realRender(self, painter, renderPath):  # TODO/PORT: Still needs work.
        """
        TOWRITE

        :param `painter`: TOWRITE
        :type `painter`: `QPainter`_
        :param `renderPath`: TOWRITE
        :type `renderPath`: `QPainterPath`_
        """
        color1 = self.objectColor()  #QColor  # lighter color
        color2 = color1.darker(150)  #QColor  # darker color

        # If we have a dark color, lighten it
        darkness = color1.lightness() #int
        threshold = 32 #int   #TODO: This number may need adjusted or maybe just add it to settings.
        if darkness < threshold:
            color2 = color1
            if not darkness:
                color1 = QColor(threshold, threshold, threshold)  # lighter() does not affect pure black
            else :
                color1 = color2.lighter(100 + threshold)

        count = renderPath.elementCount()  # int
        for i in range(0, count - 1):  # for(int i = 0; i < count-1; ++i);

            elem = renderPath.elementAt(i)      # QPainterPath::Element
            next = renderPath.elementAt(i + 1)  # QPainterPath::Element

            if next.isMoveTo():
                continue

            elemPath = QPainterPath()
            elemPath.moveTo(elem.x, elem.y)
            elemPath.lineTo(next.x, next.y)

            renderPen = QPen(QColor(0, 0, 0, 0))
            renderPen.setWidthF(0)
            painter.setPen(renderPen)
            stroker = QPainterPathStroker()
            stroker.setWidth(0.35)
            stroker.setCapStyle(Qt.RoundCap)
            stroker.setJoinStyle(Qt.RoundJoin)
            realPath = stroker.createStroke(elemPath)  # QPainterPath
            painter.drawPath(realPath)

            grad = QLinearGradient(elemPath.pointAtPercent(0.5), elemPath.pointAtPercent(0.0))
            grad.setColorAt(0, color1)
            grad.setColorAt(1, color2)
            grad.setSpread(QGradient.ReflectSpread)

            painter.fillPath(realPath, QBrush(grad))

    def objectID(self):
        """
        TOWRITE

        :return: TOWRITE
        :rtype: int
        """
        return self.objID

    def objectPen(self):
        """
        TOWRITE

        :return: TOWRITE
        :rtype: `QPen`_
        """
        return self.objPen

    def objectColor(self):
        """
        TOWRITE

        :return: TOWRITE
        :rtype: `QColor`_
        """
        return self.objPen.color()

    def objectColorRGB(self):
        """
        TOWRITE

        :return: TOWRITE
        :rtype: int
        """
        return self.objPen.color().rgb()

    def objectLineType(self):
        """
        TOWRITE

        :return: TOWRITE
        :rtype: Qt.PenStyle
        """
        return self.objPen.style()

    def objectLineWeight(self):
        """
        TOWRITE

        :return: TOWRITE
        :rtype: float
        """
        return self.lwtPen.widthF()

    def objectPath(self):
        """
        TOWRITE

        :return: TOWRITE
        :rtype: `QPainterPath`_
        """
        return self.path()

    def objectRubberMode(self):
        """
        TOWRITE

        :return: TOWRITE
        :rtype: int
        """
        return self.objRubberMode

    def rect(self):
        """
        TOWRITE

        :return: TOWRITE
        :rtype: `QRectF`_
        """
        return self.path().boundingRect()

    # pythonic setRect overload
    @signature(QPointF)
    def setRectFromRect(self, r):
        """
        TOWRITE

        :param `r`: TOWRITE
        :type `r`: QPointF
        """
        p = QPainterPath()
        p.addRect(r)
        self.setPath(p)

    # pythonic setRect overload
    @signature(float, float, float, float)
    def setRectFromXYWH(self, x, y, w, h):
        """
        TOWRITE

        :param `x`: TOWRITE
        :type `x`: qreal
        :param `y`: TOWRITE
        :type `y`: qreal
        :param `w`: TOWRITE
        :type `w`: qreal
        :param `h`: TOWRITE
        :type `h`: qreal
        """
        p = QPainterPath()
        p.addRect(x, y, w, h)
        self.setPath(p)

    @overloaded(setRectFromRect, setRectFromXYWH)
    def setRect(self, *args):
        """ TOWRITE """
        pass

    def line(self):
        """
        TOWRITE

        :return: TOWRITE
        :rtype: `QLineF`_
        """
        return self.objLine

    # pythonic setLine overload
    @signature(QPointF)
    def setLineFromLine(self, li):
        """
        TOWRITE

        :param `li`: TOWRITE
        :type `li`: QPointF
        """
        p = QPainterPath()
        p.moveTo(li.p1())
        p.lineTo(li.p2())
        self.setPath(p)
        self.objLine = li

    # pythonic setLine overload
    @signature(float, float, float, float)
    def setLineFromXXYY(self, x1, y1, x2, y2):
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
        p = QPainterPath()
        p.moveTo(x1, y1)
        p.lineTo(x2, y2)
        self.setPath(p)
        self.objLine.setLine(x1, y1, x2, y2)

    @overloaded(setLineFromLine, setLineFromXXYY)
    def setLine(self, *args):
        """ TOWRITE """
        pass

    def setObjectPath(self, p):
        """
        TOWRITE

        :param `p`: TOWRITE
        :type `p`: `QPainterPath`_
        """
        self.setPath(p)

    def setObjectRubberMode(self, mode):
        """
        TOWRITE

        :param `mode`: TOWRITE
        :type `mode`: int
        """
        self.objRubberMode = mode

    def setObjectRubberPoint(self, key, point):
        """
        TOWRITE

        :param `key`: TOWRITE
        :type `key`: str
        :param `point`: TOWRITE
        :type `point`: `QPointF`_
        """
        self.objRubberPoints[key] = point  # .insert(key, point)

    def setObjectRubberText(self, key, txt):
        """
        TOWRITE

        :param `key`: TOWRITE
        :type `key`: str
        :param `txt`: TOWRITE
        :type `txt`: str
        """
        self.objRubberTexts[key] = txt  # .insert(key, txt)

    def shape(self):
        """
        TOWRITE

        :return: TOWRITE
        :rtype: `QPainterPath`_
        """
        return self.path()

    def vulcanize(self):
        """ TOWRITE """
        raise NotImplementedError

    def mouseSnapPoint(self, mousePoint):
        """
        TOWRITE

        :param `mousePoint`: TOWRITE
        :type `mousePoint`: `QPointF`_
        :return: TOWRITE
        :rtype: `QPointF`_
        """
        raise NotImplementedError

    def allGripPoints(self):
        """
        TOWRITE

        :return: TOWRITE
        :rtype: list[`QPointF`_]
        """
        raise NotImplementedError

    def gripEdit(self, before, after):
        """
        TOWRITE

        :param `before`: TOWRITE
        :type `before`: `QPointF`_
        :param `after`: TOWRITE
        :type `after`: `QPointF`_
        """
        raise NotImplementedError

    def lineWeightPen(self):
        """
        TOWRITE

        :return: TOWRITE
        :rtype: `QPen`_
        """
        return self.lwtPen

# kate: bom off; indent-mode python; indent-width 4; replace-trailing-space-save on;
