#!/usr/bin/env python
# -*- coding: utf-8 -*-

"""
=====================================
|module_summary| object_textsingle.py
=====================================

TOWRITE


Classes summary:
================

============================ ============================
:class:`~TextSingleObject`   TOWRITE
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
    from PySide.QtGui import QGraphicsItem, QPainter, QPainterPath, QStyle, QTransform, QFont
    PYSIDE = True
    PYQT4 = False
except ImportError:
    raise
#    ## from PyQt4 import QtCore, QtGui
#    # or... Improve performace with less dots...
#    from PyQt4.QtCore import qDebug, Qt, QLineF, QPointF
#    from PyQt4.QtGui import QGraphicsItem, QPainter, QPainterPath, QStyle, QTransform, QFont
#    PYSIDE = False
#    PYQT4 = True

#--Local Imports.
from object_base import BaseObject

# C++C++C++C++C++C++C++C++C++C++C++C++C++C++C++C++C++C++C++C++C++C++C++C++C++
#include "object-textsingle.h"
#include "object-data.h"

#include <QPainter>
#include <QStyleOption>
#include <QGraphicsScene>
# C++C++C++C++C++C++C++C++C++C++C++C++C++C++C++C++C++C++C++C++C++C++C++C++C++


class TextSingleObject(BaseObject):
    """
    Subclass of `BaseObject`_

    TOWRITE

    """
    def __init__(self, strng, x, y, rgb, parent):
        #OVERLOADED IMPL?# TextSingleObject::TextSingleObject(TextSingleObject* obj, QGraphicsItem* parent) : BaseObject(parent)
        """
        Default class constructor.

        :param `strng`: TOWRITE
        :type `strng`: QString
        :param `x`: TOWRITE
        :type `x`: qreal
        :param `y`: TOWRITE
        :type `y`: qreal
        :param `rgb`: TOWRITE
        :type `rgb`: QRgb
        :param `parent`: TOWRITE
        :type `parent`: `QGraphicsItem`_
        """
        super(TextSingleObject, self).__init__(parent)

        qDebug("TextSingleObject Constructor()")
        self.init(str, x, y, rgb, Qt.SolidLine)  # TODO: getCurrentLineType

        #OVERLOADED IMPL?# if obj:
        #OVERLOADED IMPL?#     self.setObjectTextFont(obj.objectTextFont())
        #OVERLOADED IMPL?#     self.setObjectTextSize(obj.objectTextSize())
        #OVERLOADED IMPL?#     self.setRotation(obj.rotation())
        #OVERLOADED IMPL?#     self.setObjectTextBackward(obj.objectTextBackward())
        #OVERLOADED IMPL?#     self.setObjectTextUpsideDown(obj.objectTextUpsideDown())
        #OVERLOADED IMPL?#     self.setObjectTextStyle(obj.objectTextBold(), obj.objectTextItalic(), obj.objectTextUnderline(), obj.objectTextStrikeOut(), obj.objectTextOverline())
        #OVERLOADED IMPL?#     self.init(obj.objectText(), obj.objectX(), obj.objectY(), obj.objectColorRGB(), Qt.SolidLine)  # TODO: getCurrentLineType
        #OVERLOADED IMPL?#     self.setScale(obj.scale())


    def __del__(self):
        """Class destructor."""
        qDebug("TextSingleObject Destructor()")

    def init(self, strng, x, y, rgb, lineType):
        """
        TOWRITE

        :param `strng`: TOWRITE
        :type `strng`: QString
        :param `x`: TOWRITE
        :type `x`: qreal
        :param `y`: TOWRITE
        :type `y`: qreal
        :param `rgb`: TOWRITE
        :type `rgb`: QRgb
        :param `lineType`: TOWRITE
        :type `lineType`: Qt.PenStyle
        """
        self.setData(OBJ_TYPE, self.type())
        self.setData(OBJ_NAME, OBJ_NAME_TEXTSINGLE)

        # WARNING: DO NOT enable QGraphicsItem::ItemIsMovable. If it is enabled,
        # WARNING: and the item is double clicked, the scene will erratically move the item while zooming.
        # WARNING: All movement has to be handled explicitly by us, not by the scene.
        self.setFlag(QGraphicsItem.ItemIsSelectable, True)

        self.objTextJustify = "Left"  # TODO: set the justification properly

        self.setObjectText(strng)
        self.setObjectPos(x, y)
        self.setObjectColor(rgb)
        self.setObjectLineType(lineType)
        self.setObjectLineWeight(0.35)  # TODO: pass in proper lineweight
        self.setPen(self.objectPen())

    def objectTextJustifyList(self):
        """
        TOWRITE

        :rtype: list / QStringList
        """
        justifyList = [
            "Left", "Center", "Right", # /* TODO: << "Aligned" */ << "Middle" /* TODO: << "Fit" */ ;
            "Top Left", "Top Center", "Top Right",
            "Middle Left", "Middle Center", "Middle Right",
            "Bottom Left", "Bottom Center", "Bottom Right"
            ]
        return justifyList

    def setObjectText(self, strng):
        """
        TOWRITE

        :param `strng`: TOWRITE
        :type `strng`: QString
        """
        objText = strng
        textPath = QPainterPath()
        font = QFont()
        font.setFamily(objTextFont)
        font.setPointSizeF(objTextSize)
        font.setBold(objTextBold)
        font.setItalic(objTextItalic)
        font.setUnderline(objTextUnderline)
        font.setStrikeOut(objTextStrikeOut)
        font.setOverline(objTextOverline)
        textPath.addText(0, 0, font, strng)

        # Translate the path based on the justification.
        jRect = textPath.boundingRect()  # QRectF
        if   objTextJustify == "Left":          textPath.translate(-jRect.left(), 0)
        elif objTextJustify == "Center":        textPath.translate(-jRect.center().x(), 0)
        elif objTextJustify == "Right":         textPath.translate(-jRect.right(), 0)
        elif objTextJustify == "Aligned":       pass # TODO: TextSingleObject Aligned Justification
        elif objTextJustify == "Middle":        textPath.translate(-jRect.center())
        elif objTextJustify == "Fit":           pass # TODO: TextSingleObject Fit Justification
        elif objTextJustify == "Top Left":      textPath.translate(-jRect.topLeft())
        elif objTextJustify == "Top Center":    textPath.translate(-jRect.center().x(), -jRect.top())
        elif objTextJustify == "Top Right":     textPath.translate(-jRect.topRight())
        elif objTextJustify == "Middle Left":   textPath.translate(-jRect.left(), -jRect.top()/2.0)
        elif objTextJustify == "Middle Center": textPath.translate(-jRect.center().x(), -jRect.top()/2.0)
        elif objTextJustify == "Middle Right":  textPath.translate(-jRect.right(), -jRect.top()/2.0)
        elif objTextJustify == "Bottom Left":   textPath.translate(-jRect.bottomLeft())
        elif objTextJustify == "Bottom Center": textPath.translate(-jRect.center().x(), -jRect.bottom())
        elif objTextJustify == "Bottom Right":  textPath.translate(-jRect.bottomRight())

        # Backward or Upside Down.
        if objTextBackward or objTextUpsideDown:

            horiz = 1.0  # qreal
            vert = 1.0   # qreal
            if objTextBackward:
                horiz = -1.0
            if objTextUpsideDown:
                vert = -1.0

            flippedPath = QPainterPath()

            element = QPainterPath.Element
            P2      = QPainterPath.Element
            P3      = QPainterPath.Element
            P4      = QPainterPath.Element
            for i in range(0, textPath.elementCount()):  # for(int i = 0; i < textPath.elementCount(); ++i)

                element = textPath.elementAt(i)
                if element.isMoveTo():
                    flippedPath.moveTo(horiz * element.x, vert * element.y);

                elif element.isLineTo():
                    flippedPath.lineTo(horiz * element.x, vert * element.y);

                elif element.isCurveTo():
                                                    # start point P1 is not needed
                    P2 = textPath.elementAt(i)      # control point
                    P3 = textPath.elementAt(i + 1)  # control point
                    P4 = textPath.elementAt(i + 2)  # end point

                    flippedPath.cubicTo(horiz * P2.x, vert * P2.y,
                                        horiz * P3.x, vert * P3.y,
                                        horiz * P4.x, vert * P4.y)

            objTextPath = flippedPath

        else:
            objTextPath = textPath

        # Add the grip point to the shape path.
        gripPath = objTextPath  # QPainterPath
        gripPath.connectPath(objTextPath)
        gripPath.addRect(-0.00000001, -0.00000001, 0.00000002, 0.00000002)
        self.setObjectPath(gripPath)

    def setObjectTextFont(self, font):
        """
        TOWRITE

        :param `font`: TOWRITE
        :type `font`: QString
        """
        self.objTextFont = font
        self.setObjectText(self.objText)

    def setObjectTextJustify(self, justify):
        """
        TOWRITE

        :param `justify`: TOWRITE
        :type `justify`: QString
        """
        # Verify the string is a valid option.
        if   justify == "Left":          self.objTextJustify = justify
        elif justify == "Center":        self.objTextJustify = justify
        elif justify == "Right":         self.objTextJustify = justify
        elif justify == "Aligned":       self.objTextJustify = justify
        elif justify == "Middle":        self.objTextJustify = justify
        elif justify == "Fit":           self.objTextJustify = justify
        elif justify == "Top Left":      self.objTextJustify = justify
        elif justify == "Top Center":    self.objTextJustify = justify
        elif justify == "Top Right":     self.objTextJustify = justify
        elif justify == "Middle Left":   self.objTextJustify = justify
        elif justify == "Middle Center": self.objTextJustify = justify
        elif justify == "Middle Right":  self.objTextJustify = justify
        elif justify == "Bottom Left":   self.objTextJustify = justify
        elif justify == "Bottom Center": self.objTextJustify = justify
        elif justify == "Bottom Right":  self.objTextJustify = justify
        else:                            self.objTextJustify = "Left"  # Default
        self.setObjectText(self.objText)

    def setObjectTextSize(self, size):
        """
        TOWRITE

        :param `size`: TOWRITE
        :type `size`: qreal
        """
        self.objTextSize = size
        self.setObjectText(self.objText)

    def setObjectTextStyle(self, bold, italic, under, strike, over):
        """
        TOWRITE

        :param `bold`: TOWRITE
        :type `bold`: bool
        :param `italic`: TOWRITE
        :type `italic`: bool
        :param `under`: TOWRITE
        :type `under`: bool
        :param `strike`: TOWRITE
        :type `strike`: bool
        :param `over`: TOWRITE
        :type `over`: bool
        """
        self.objTextBold = bold
        self.objTextItalic = italic
        self.objTextUnderline = under
        self.objTextStrikeOut = strike
        self.objTextOverline = over
        self.setObjectText(self.objText)

    def setObjectTextBold(self, val):
        """
        TOWRITE

        :param `val`: TOWRITE
        :type `val`: bool
        """
        self.objTextBold = val
        self.setObjectText(self.objText)

    def setObjectTextItalic(self, val):
        """
        TOWRITE

        :param `val`: TOWRITE
        :type `val`: bool
        """
        self.objTextItalic = val
        self.setObjectText(self.objText)

    def setObjectTextUnderline(self, val):
        """
        TOWRITE

        :param `val`: TOWRITE
        :type `val`: bool
        """
        self.objTextUnderline = val
        self.setObjectText(self.objText)

    def setObjectTextStrikeOut(self, val):
        """
        TOWRITE

        :param `val`: TOWRITE
        :type `val`: bool
        """
        self.objTextStrikeOut = val
        self.setObjectText(self.objText)

    def setObjectTextOverline(self, val):
        """
        TOWRITE

        :param `val`: TOWRITE
        :type `val`: bool
        """
        self.objTextOverline = val
        self.setObjectText(self.objText)

    def setObjectTextBackward(self, val):
        """
        TOWRITE

        :param `val`: TOWRITE
        :type `val`: bool
        """
        self.objTextBackward = val
        self.setObjectText(self.objText)

    def setObjectTextUpsideDown(self, val):
        """
        TOWRITE

        :param `val`: TOWRITE
        :type `val`: bool
        """
        self.objTextUpsideDown = val
        self.setObjectText(self.objText)

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

        painter.drawPath(objTextPath)

    def updateRubber(self, painter):
        """
        TOWRITE

        :param `painter`: TOWRITE
        :type `painter`: `QPainter`_
        """
        rubberMode = self.objectRubberMode()  # int
        if rubberMode == OBJ_RUBBER_TEXTSINGLE:

            self.setObjectTextFont(self.objectRubberText("TEXT_FONT"))
            self.setObjectTextJustify(self.objectRubberText("TEXT_JUSTIFY"))
            self.setObjectPos(self.objectRubberPoint("TEXT_POINT"))
            hr = self.objectRubberPoint("TEXT_HEIGHT_ROTATION")  # QPointF
            self.setObjectTextSize(hr.x())
            self.setRotation(hr.y())
            self.setObjectText(self.objectRubberText("TEXT_RAPID"))

        elif rubberMode == OBJ_RUBBER_GRIP:

            if painter:

                gripPoint = self.objectRubberPoint("GRIP_POINT")  # QPointF
                if gripPoint == self.scenePos():

                    painter.drawPath(self.objectPath().translated(self.mapFromScene(self.objectRubberPoint("")) - self.mapFromScene(gripPoint)))

                rubLine = QLineF(self.mapFromScene(gripPoint), self.mapFromScene(self.objectRubberPoint("")))
                self.drawRubberLine(rubLine, painter, "VIEW_COLOR_CROSSHAIR")

    def vulcanize(self):
        """
        TOWRITE
        """
        qDebug("TextSingleObject vulcanize()")
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
        ## gripPoints << scenePos();
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
            delta = after - before  # QPointF
            self.moveBy(delta.x(), delta.y())

    def subPathList(self):
        """
        TOWRITE

        :rtype: QList<QPainterPath>
        """
        s = self.scale()  # qreal
        trans = QTransform()
        trans.rotate(self.rotation())
        trans.scale(s, s)

        ## QList<QPainterPath> pathList;
        pathList = []
        path = objTextPath  # QPainterPath

        element = QPainterPath.Element
        pathMoves = []  # QList<int>
        numMoves = 0  # int

        for i in range(0, path.elementCount()):  # for(int i = 0; i < path.elementCount(); i++)

            element = path.elementAt(i)
            if element.isMoveTo():

                pathMoves.append(i)  # pathMoves << i;
                numMoves += 1 # numMoves++;

        pathMoves.append(path.elementCount())  # pathMoves << path.elementCount();

        for p in range(0, pathMoves.size() - 1 and numMoves):  #  for(int p = 0; p < pathMoves.size()-1 && p < numMoves; p++)

            subPath = QPainterPath()
            for i in range(pathMoves.value(p), pathMoves.value(p + 1)):  # for(int i = pathMoves.value(p); i < pathMoves.value(p+1); i++)

                element = path.elementAt(i)
                if element.isMoveTo():
                    subPath.moveTo(element.x, element.y)

                elif element.isLineTo():
                    subPath.lineTo(element.x, element.y)

                elif element.isCurveTo():
                    subPath.cubicTo(path.elementAt(i).x, path.elementAt(i).y,          # control point 1
                                    path.elementAt(i + 1).x, path.elementAt(i + 1).y,  # control point 2
                                    path.elementAt(i + 2).x, path.elementAt(i + 2).y)  # end point

            pathList.append(trans.map(subPath))

        return pathList


# kate: bom off; indent-mode python; indent-width 4; replace-trailing-space-save on;
