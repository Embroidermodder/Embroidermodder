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
try:
    ## from PySide import QtCore, QtGui
    # or... Improve performace with less dots...
    from PySide.QtCore import qDebug, Qt, QDateTime
    from PySide.QtGui import (QGraphicsScene, QMessageBox, QGraphicsItem,
        QGraphicsPathItem, QColor, QPen, QBrush, QPainter, QMessageBox)
    PYSIDE = True
    PYQT4 = False
except ImportError:
    raise
#    ## from PyQt4 import QtCore, QtGui
#    # or... Improve performace with less dots...
#    from PyQt4.QtCore import qDebug, Qt, QDateTime
#    from PyQt4.QtGui import (QGraphicsScene, QMessageBox, QGraphicsItem,
#        QGraphicsPathItem, QColor, QPen, QBrush, QPainter, QMessageBox)
#    PYSIDE = False
#    PYQT4 = True


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

    .. sphinx_generate_methods_summary::
       BaseObject
    """
    def __init__(self, parent=None):
        """
        Default class constructor.

        :param `parent`: Pointer to a parent widget instance.
        :type `parent`: `QGraphicsItem`_
        """
        super(BaseObject, self).__init__(parent)

        qDebug("BaseObject Constructor()")

        objPen.setCapStyle(Qt.RoundCap)
        objPen.setJoinStyle(Qt.RoundJoin)
        lwtPen.setCapStyle(Qt.RoundCap)
        lwtPen.setJoinStyle(Qt.RoundJoin)

        objID = QDateTime.currentMSecsSinceEpoch()


    def __del__(self):
        """Class destructor."""
        qDebug("BaseObject Destructor()")

    def setObjectColor(self, color):
        """
        TOWRITE

        :param `color`: TOWRITE
        :type `color`: `QColor`_
        """
        objPen.setColor(color)
        lwtPen.setColor(color)

    def setObjectColorRGB(self, rgb):
        """
        TOWRITE

        :param `rgb`: TOWRITE
        :type `rgb`: QRgb
        """
        objPen.setColor(QColor(rgb))
        lwtPen.setColor(QColor(rgb))

    def setObjectLineType(self, lineType):
        """
        TOWRITE

        :param `rgb`: TOWRITE
        :type `rgb`: Qt.PenStyle
        """
        objPen.setStyle(lineType)
        lwtPen.setStyle(lineType)

    def setObjectLineWeight(self, lineWeight):
        """
        TOWRITE

        :param `lineWeight`: TOWRITE
        :type `lineWeight`: qreal
        """
        objPen.setWidthF(0)  # NOTE: The objPen will always be cosmetic

        if lineWeight < 0:
            if lineWeight == OBJ_LWT_BYLAYER:
                lwtPen.setWidthF(0.35)  # TODO: getLayerLineWeight
            elif lineWeight == OBJ_LWT_BYBLOCK:
                lwtPen.setWidthF(0.35)  # TODO: getBlockLineWeight
            else:
                QMessageBox.warning(0, QObject.tr("Error - Negative Lineweight"),
                                       QObject.tr("Lineweight: %1")
                                       .arg(QString().setNum(lineWeight)))
                qDebug("Lineweight cannot be negative! Inverting sign.")
                lwtPen.setWidthF(-lineWeight)
        else:
            lwtPen.setWidthF(lineWeight)

    def objectRubberPoint(self, key):
        """
        TOWRITE

        :param `key`: TOWRITE
        :type `key`: QString
        :rtype: `QPointF`_
        """
        if objRubberPoints.contains(key):
            return objRubberPoints.value(key)

        gscene = scene()  # QGraphicsScene* gscene = scene()
        if gscene:
            return scene().property(SCENE_QSNAP_POINT).toPointF()
        return QPointF()

    def objectRubberText(self, key):
        """
        TOWRITE

        :param `key`: TOWRITE
        :type `key`: QString
        :rtype: QString
        """
        if objRubberTexts.contains(key):
            return objRubberTexts.value(key)
        return ''  # QString()

    def boundingRect(self):
        """
        TOWRITE

        :rtype: `QRectF`_
        """
        # If gripped, force this object to be drawn even if it is offscreen
        if objectRubberMode() == OBJ_RUBBER_GRIP:
            return scene().sceneRect()
        return path().boundingRect()

    def drawRubberLine(self, rubLine, painter, colorFromScene):
        """
        TOWRITE

        :param `rubLine`: TOWRITE
        :type `rubLine`: `QLineF`_
        :param `painter`: TOWRITE
        :type `painter`: `QPainter`_
        :param `colorFromScene`: TOWRITE
        :type `colorFromScene`: char
        """
        if painter:
            objScene = scene()  # QGraphicsScene* objScene = scene();
            if not objScene:
                return
            colorPen = objPen  # QPen colorPen = objPen
            colorPen.setColor(QColor(objScene.property(colorFromScene).toUInt()))
            painter.setPen(colorPen)
            painter.drawLine(rubLine)
            painter.setPen(objPen)

    def realRender(self, painter, renderPath):  # TODO/PORT: Still needs work.
        """
        TOWRITE

        :param `painter`: TOWRITE
        :type `painter`: `QPainter`_
        :param `renderPath`: TOWRITE
        :type `renderPath`: `QPainterPath`_
        """
        color1 = objectColor()       #QColor  # lighter color
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


# kate: bom off; indent-mode python; indent-width 4; replace-trailing-space-save on;
