#!/usr/bin/env python
# -*- coding: utf-8 -*-

"""
===============================
|module_summary| object_save.py
===============================

TOWRITE


Classes summary:
================

============================ ============================
:class:`~SaveObject`         TOWRITE
============================ ============================

---------------------------------------------------------

|

"""

#-Imports.---------------------------------------------------------------------
#--Python Imports
qPrintable = str

#--PySide/PyQt Imports.
if PYSIDE:
    ## from PySide import QtCore, QtGui
    # or... Improve performace with less dots...
    from PySide.QtCore import qDebug, Qt, QLineF, QPointF, QObject
    from PySide.QtGui import QGraphicsItem, QPainter, QPainterPath, QStyle, QTransform, QFont
elif PYQT4:
    ## from PyQt4 import QtCore, QtGui
    # or... Improve performace with less dots...
    from PyQt4.QtCore import qDebug, Qt, QLineF, QPointF, QObject
    from PyQt4.QtGui import QGraphicsItem, QPainter, QPainterPath, QStyle, QTransform, QFont

#--Local Imports.
from object_data import *
from object_base import BaseObject

#--libembroidery Imports.
from libembroidery import (embFormat_typeFromName, embPattern_create,
    embReaderWriter_getByFileName, embPattern_movePolylinesToStitchList,
    embPattern_addCircleObjectAbs, embPattern_addEllipseObjectAbs,
    embPattern_addLineObjectAbs, embPattern_addPointObjectAbs, embColor_make,
    embPattern_addRectObjectAbs, embPointList_create, embPointList_add,
    embPoint_make, embPolylineObject_create, embPattern_addPolylineObjectAbs,
    EMBFORMAT_UNSUPPORTED, EMBFORMAT_STITCHONLY)


# C++C++C++C++C++C++C++C++C++C++C++C++C++C++C++C++C++C++C++C++C++C++C++C++C++
#include "object-save.h"
#include "object-data.h"

#include "object-arc.h"
#include "object-circle.h"
#include "object-dimleader.h"
#include "object-ellipse.h"
#include "object-line.h"
#include "object-point.h"
#include "object-polygon.h"
#include "object-polyline.h"
#include "object-rect.h"
#include "object-textsingle.h"

#include "emb-color.h"
#include "emb-format.h"

#include <QGraphicsScene>
#include <QGraphicsItem>
# C++C++C++C++C++C++C++C++C++C++C++C++C++C++C++C++C++C++C++C++C++C++C++C++C++


class SaveObject(QObject):
    """
    Subclass of `QObject`_

    TOWRITE

    """
    def __init__(self, theScene, parent=None):
        """
        Default class constructor.

        :param `theScene`: TOWRITE
        :type `theScene`: `QGraphicsScene`_
        :param `parent`: TOWRITE
        :type `parent`: `QObject`_
        """
        super(SaveObject, self).__init__(parent)

        qDebug("SaveObject Constructor()")
        self.gscene = theScene
        self.formatType = EMBFORMAT_UNSUPPORTED


    def __del__(self):
        """Class destructor."""
        qDebug("SaveObject Destructor()")

    def save(self, fileName):
        """
        TOWRITE

        :param `fileName`: TOWRITE
        :type `fileName`: QString
        :rtype: bool
        """
        qDebug("SaveObject save(%s)" % qPrintable(fileName))

        # TODO: Before saving to a stitch only format, Embroidermodder needs
        #       to calculate the optimal path to minimize jump stitches. Also
        #       based upon which layer needs to be stitched first,
        #       the path to the next object needs to be hidden beneath fills
        #       that will come later. When finding the optimal path, we need
        #       to take into account the color of the thread, as we do not want
        #       to try to hide dark colored stitches beneath light colored fills.

        writeSuccessful = False  # bool
        #TODO/PORT# int i

        self.formatType = embFormat_typeFromName(qPrintable(fileName))
        if self.formatType == EMBFORMAT_UNSUPPORTED:
            return False

        ## pattern = 0  # EmbPattern*
        ## writer = 0   # EmbReaderWriter*

        pattern = embPattern_create()
        if not pattern:
            qDebug("Could not allocate memory for embroidery pattern")

        # Write.
        writer = embReaderWriter_getByFileName(qPrintable(fileName))
        if not writer:
            qDebug("Unsupported write file type: %s" % qPrintable(fileName))
        else:

            for item in self.gscene.items():  # foreach(QGraphicsItem* item, gscene->items())

                objType = item.data(OBJ_TYPE).toInt()  # int

                if   objType == OBJ_TYPE_ARC:          self.addArc(pattern, item)
                elif objType == OBJ_TYPE_BLOCK:        self.addBlock(pattern, item)
                elif objType == OBJ_TYPE_CIRCLE:       self.addCircle(pattern, item)
                elif objType == OBJ_TYPE_DIMALIGNED:   self.addDimAligned(pattern, item)
                elif objType == OBJ_TYPE_DIMANGULAR:   self.addDimAngular(pattern, item)
                elif objType == OBJ_TYPE_DIMARCLENGTH: self.addDimArcLength(pattern, item)
                elif objType == OBJ_TYPE_DIMDIAMETER:  self.addDimDiameter(pattern, item)
                elif objType == OBJ_TYPE_DIMLEADER:    self.addDimLeader(pattern, item)
                elif objType == OBJ_TYPE_DIMLINEAR:    self.addDimLinear(pattern, item)
                elif objType == OBJ_TYPE_DIMORDINATE:  self.addDimOrdinate(pattern, item)
                elif objType == OBJ_TYPE_DIMRADIUS:    self.addDimRadius(pattern, item)
                elif objType == OBJ_TYPE_ELLIPSE:      self.addEllipse(pattern, item)
                elif objType == OBJ_TYPE_ELLIPSEARC:   self.addEllipseArc(pattern, item)
                elif objType == OBJ_TYPE_GRID:         self.addGrid(pattern, item)
                elif objType == OBJ_TYPE_HATCH:        self.addHatch(pattern, item)
                elif objType == OBJ_TYPE_IMAGE:        self.addImage(pattern, item)
                elif objType == OBJ_TYPE_INFINITELINE: self.addInfiniteLine(pattern, item)
                elif objType == OBJ_TYPE_LINE:         self.addLine(pattern, item)
                elif objType == OBJ_TYPE_POINT:        self.addPoint(pattern, item)
                elif objType == OBJ_TYPE_POLYGON:      self.addPolygon(pattern, item)
                elif objType == OBJ_TYPE_POLYLINE:     self.addPolyline(pattern, item)
                elif objType == OBJ_TYPE_RAY:          self.addRay(pattern, item)
                elif objType == OBJ_TYPE_RECTANGLE:    self.addRectangle(pattern, item)
                elif objType == OBJ_TYPE_SPLINE:       self.addSpline(pattern, item)
                elif objType == OBJ_TYPE_TEXTMULTI:    self.addTextMulti(pattern, item)
                elif objType == OBJ_TYPE_TEXTSINGLE:   self.addTextSingle(pattern, item)


            # TODO: handle EMBFORMAT_STCHANDOBJ also
            if self.formatType == EMBFORMAT_STITCHONLY:
                embPattern_movePolylinesToStitchList(pattern)  # TODO: handle all objects like this

            writeSuccessful = writer.writer(pattern, qPrintable(fileName))
            if not writeSuccessful:
                qDebug("Writing file %s was unsuccessful" % qPrintable(fileName))

        # TODO: check the embLog for errors and if any exist, report them.

        ## free(writer)              #TODO/REMOVE# not needed in python
        ## embPattern_free(pattern)  #TODO/REMOVE# not needed in python

        return writeSuccessful

    def addArc(self, pattern, item):
        """
        TOWRITE

        :param `pattern`: TOWRITE
        :type `pattern`: EmbPattern
        :param `item`: TOWRITE
        :type `item`: `QGraphicsItem`_
        """
        pass #TODO/PORT# Is this supposed to be empty?

    def addBlock(self, pattern, item):
        """
        TOWRITE

        :param `pattern`: TOWRITE
        :type `pattern`: EmbPattern
        :param `item`: TOWRITE
        :type `item`: `QGraphicsItem`_
        """
        pass #TODO/PORT# Is this supposed to be empty?

    def addCircle(self, pattern, item):
        """
        TOWRITE

        :param `pattern`: TOWRITE
        :type `pattern`: EmbPattern
        :param `item`: TOWRITE
        :type `item`: `QGraphicsItem`_
        """
        obj = item  # CircleObject* obj = static_cast<CircleObject*>(item);
        if obj:
            if self.formatType == EMBFORMAT_STITCHONLY:
                path = obj.objectSavePath()  # QPainterPath
                self.toPolyline(pattern, obj.objectCenter(), path.simplified(), "0", obj.objectColor(), "CONTINUOUS", "BYLAYER") # TODO: proper layer/lineType/lineWeight # TODO: Improve precision, replace simplified
            else:
                embPattern_addCircleObjectAbs(pattern, float(obj.objectCenterX()), float(obj.objectCenterY()), float(obj.objectRadius()))

    def addDimAligned(self, pattern, item):
        """
        TOWRITE

        :param `pattern`: TOWRITE
        :type `pattern`: EmbPattern
        :param `item`: TOWRITE
        :type `item`: `QGraphicsItem`_
        """
        pass #TODO/PORT# Is this supposed to be empty?

    def addDimAngular(self, pattern, item):
        """
        TOWRITE

        :param `pattern`: TOWRITE
        :type `pattern`: EmbPattern
        :param `item`: TOWRITE
        :type `item`: `QGraphicsItem`_
        """
        pass #TODO/PORT# Is this supposed to be empty?

    def addDimArcLength(self, pattern, item):
        """
        TOWRITE

        :param `pattern`: TOWRITE
        :type `pattern`: EmbPattern
        :param `item`: TOWRITE
        :type `item`: `QGraphicsItem`_
        """
        pass #TODO/PORT# Is this supposed to be empty?

    def addDimDiameter(self, pattern, item):
        """
        TOWRITE

        :param `pattern`: TOWRITE
        :type `pattern`: EmbPattern
        :param `item`: TOWRITE
        :type `item`: `QGraphicsItem`_
        """
        pass #TODO/PORT# Is this supposed to be empty?

    def addDimLeader(self, pattern, item):
        """
        TOWRITE

        :param `pattern`: TOWRITE
        :type `pattern`: EmbPattern
        :param `item`: TOWRITE
        :type `item`: `QGraphicsItem`_
        """
        pass #TODO/PORT# Is this supposed to be empty?

    def addDimLinear(self, pattern, item):
        """
        TOWRITE

        :param `pattern`: TOWRITE
        :type `pattern`: EmbPattern
        :param `item`: TOWRITE
        :type `item`: `QGraphicsItem`_
        """
        pass #TODO/PORT# Is this supposed to be empty?

    def addDimOrdinate(self, pattern, item):
        """
        TOWRITE

        :param `pattern`: TOWRITE
        :type `pattern`: EmbPattern
        :param `item`: TOWRITE
        :type `item`: `QGraphicsItem`_
        """
        pass #TODO/PORT# Is this supposed to be empty?

    def addDimRadius(self, pattern, item):
        """
        TOWRITE

        :param `pattern`: TOWRITE
        :type `pattern`: EmbPattern
        :param `item`: TOWRITE
        :type `item`: `QGraphicsItem`_
        """
        pass #TODO/PORT# Is this supposed to be empty?

    def addEllipse(self, pattern, item):
        """
        TOWRITE

        :param `pattern`: TOWRITE
        :type `pattern`: EmbPattern
        :param `item`: TOWRITE
        :type `item`: `QGraphicsItem`_
        """
        obj = item # EllipseObject* obj = static_cast<EllipseObject*>(item);
        if obj:
            if self.formatType == EMBFORMAT_STITCHONLY:
                path = obj.objectSavePath()  # QPainterPath
                self.toPolyline(pattern, obj.objectCenter(), path.simplified(), "0", obj.objectColor(), "CONTINUOUS", "BYLAYER") # TODO: proper layer/lineType/lineWeight # TODO: Improve precision, replace simplified
            else:
                # TODO: ellipse rotation
                embPattern_addEllipseObjectAbs(pattern, float(obj.objectCenterX()), float(obj.objectCenterY()), float(obj.objectWidth()) / 2.0, float(obj.objectHeight()) / 2.0)

    def addEllipseArc(self, pattern, item):
        """
        TOWRITE

        :param `pattern`: TOWRITE
        :type `pattern`: EmbPattern
        :param `item`: TOWRITE
        :type `item`: `QGraphicsItem`_
        """
        pass #TODO/PORT# Is this supposed to be empty?

    def addGrid(self, pattern, item):
        """
        TOWRITE

        :param `pattern`: TOWRITE
        :type `pattern`: EmbPattern
        :param `item`: TOWRITE
        :type `item`: `QGraphicsItem`_
        """
        pass #TODO/PORT# Is this supposed to be empty?

    def addHatch(self, pattern, item):
        """
        TOWRITE

        :param `pattern`: TOWRITE
        :type `pattern`: EmbPattern
        :param `item`: TOWRITE
        :type `item`: `QGraphicsItem`_
        """
        pass #TODO/PORT# Is this supposed to be empty?

    def addImage(self, pattern, item):
        """
        TOWRITE

        :param `pattern`: TOWRITE
        :type `pattern`: EmbPattern
        :param `item`: TOWRITE
        :type `item`: `QGraphicsItem`_
        """
        pass #TODO/PORT# Is this supposed to be empty?

    def addInfiniteLine(self, pattern, item):
        """
        TOWRITE

        :param `pattern`: TOWRITE
        :type `pattern`: EmbPattern
        :param `item`: TOWRITE
        :type `item`: `QGraphicsItem`_
        """
        pass #TODO/PORT# Is this supposed to be empty?

    def addLine(self, pattern, item):
        """
        TOWRITE

        :param `pattern`: TOWRITE
        :type `pattern`: EmbPattern
        :param `item`: TOWRITE
        :type `item`: `QGraphicsItem`_
        """
        obj = item # LineObject* obj = static_cast<LineObject*>(item);
        if obj:
            if self.formatType == EMBFORMAT_STITCHONLY:
                self.toPolyline(pattern, obj.objectEndPoint1(), obj.objectSavePath(), "0", obj.objectColor(), "CONTINUOUS", "BYLAYER")  # TODO: proper layer/lineType/lineWeight
            else:
                embPattern_addLineObjectAbs(pattern, float(obj.objectX1()), float(obj.objectY1()), float(obj.objectX2()), float(obj.objectY2()))

    def addPath(self, pattern, item):
        """
        TOWRITE

        :param `pattern`: TOWRITE
        :type `pattern`: EmbPattern
        :param `item`: TOWRITE
        :type `item`: `QGraphicsItem`_

        .. TODO:: Reimplement addPolyline() using the libembroidery C API

        """
        ## //TODO: Reimplement addPolyline() using the libembroidery C API
        ## /*
        ## qDebug("addPolyline()");
        ## QGraphicsPathItem* polylineItem = (QGraphicsPathItem*)item;
        ## if(polylineItem)
        ## {
        ##     QPainterPath path = polylineItem->path();
        ##     QPointF pos = polylineItem->pos();
        ##     qreal startX = pos.x();
        ##     qreal startY = pos.y();
        ##
        ##     QPainterPath::Element element;
        ##     QPainterPath::Element P1;
        ##     QPainterPath::Element P2;
        ##     QPainterPath::Element P3;
        ##     QPainterPath::Element P4;
        ##
        ##     for(int i = 0; i < path.elementCount()-1; ++i)
        ##     {
        ##         element = path.elementAt(i);
        ##         if(element.isMoveTo())
        ##         {
        ##             pattern.AddStitchAbs((element.x + startX), -(element.y + startY), TRIM);
        ##         }
        ##         else if(element.isLineTo())
        ##         {
        ##             pattern.AddStitchAbs((element.x + startX), -(element.y + startY), NORMAL);
        ##         }
        ##         else if(element.isCurveTo())
        ##         {
        ##             P1 = path.elementAt(i-1); // start point
        ##             P2 = path.elementAt(i);   // control point
        ##             P3 = path.elementAt(i+1); // control point
        ##             P4 = path.elementAt(i+2); // end point
        ##
        ##             pattern.AddStitchAbs(P4.x, -P4.y, NORMAL); //TODO: This is temporary
        ##             //TODO: Curved Polyline segments are always arcs
        ##         }
        ##     }
        ##     pattern.AddStitchRel(0, 0, STOP);
        ##     QColor c= polylineItem->pen().color();
        ##     pattern.AddColor(c.red(), c.green(), c.blue(), "", "");
        ## }
        ## */

    def addPoint(self, pattern, item):
        """
        TOWRITE

        :param `pattern`: TOWRITE
        :type `pattern`: EmbPattern
        :param `item`: TOWRITE
        :type `item`: `QGraphicsItem`_
        """
        obj = item # PointObject* obj = static_cast<PointObject*>(item);
        if obj:
            if self.formatType == EMBFORMAT_STITCHONLY:
                self.toPolyline(pattern, obj.objectPos(), obj.objectSavePath(), "0", obj.objectColor(), "CONTINUOUS", "BYLAYER")  # TODO: proper layer/lineType/lineWeight
            else:
                embPattern_addPointObjectAbs(pattern, float(obj.objectX()), float(obj.objectY()))

    def addPolygon(self, pattern, item):
        """
        TOWRITE

        :param `pattern`: TOWRITE
        :type `pattern`: EmbPattern
        :param `item`: TOWRITE
        :type `item`: `QGraphicsItem`_
        """
        obj = item # PolygonObject* obj = static_cast<PolygonObject*>(item);
        if obj:
            self.toPolyline(pattern, obj.objectPos(), obj.objectSavePath(), "0", obj.objectColor(), "CONTINUOUS", "BYLAYER")  # TODO: proper layer/lineType/lineWeight

    def addPolyline(self, pattern, item):
        """
        TOWRITE

        :param `pattern`: TOWRITE
        :type `pattern`: EmbPattern
        :param `item`: TOWRITE
        :type `item`: `QGraphicsItem`_
        """
        obj = item # PolylineObject* obj = static_cast<PolylineObject*>(item);
        if obj:
            self.toPolyline(pattern, obj.objectPos(), obj.objectSavePath(), "0", obj.objectColor(), "CONTINUOUS", "BYLAYER")  # TODO: proper layer/lineType/lineWeight

    def addRay(self, pattern, item):
        """
        TOWRITE

        :param `pattern`: TOWRITE
        :type `pattern`: EmbPattern
        :param `item`: TOWRITE
        :type `item`: `QGraphicsItem`_
        """
        pass #TODO/PORT# Is this supposed to be empty?

    def addRectangle(self, pattern, item):
        """
        TOWRITE

        :param `pattern`: TOWRITE
        :type `pattern`: EmbPattern
        :param `item`: TOWRITE
        :type `item`: `QGraphicsItem`_
        """
        obj = item # RectObject* obj = static_cast<RectObject*>(item);
        if obj:
            if self.formatType == EMBFORMAT_STITCHONLY:
                self.toPolyline(pattern, obj.objectPos(), obj.objectSavePath(), "0", obj.objectColor(), "CONTINUOUS", "BYLAYER")  # TODO: proper layer/lineType/lineWeight
            else:
                # TODO: Review this at some point
                topLeft = obj.objectTopLeft()  # QPointF
                embPattern_addRectObjectAbs(pattern, float(topLeft.x()), float(topLeft.y()), float(obj.objectWidth()), float(obj.objectHeight()))

    def addSlot(self, pattern, item):
        """
        TOWRITE

        :param `pattern`: TOWRITE
        :type `pattern`: EmbPattern
        :param `item`: TOWRITE
        :type `item`: `QGraphicsItem`_
        """
        pass #TODO/PORT# Is this supposed to be empty?

    def addSpline(self, pattern, item):
        """
        TOWRITE

        :param `pattern`: TOWRITE
        :type `pattern`: EmbPattern
        :param `item`: TOWRITE
        :type `item`: `QGraphicsItem`_
        """
        pass #TODO: abstract bezier into geom-bezier... cubicBezierMagic(P1, P2, P3, P4, 0.0, 1.0, tPoints);

    def addTextMulti(self, pattern, item):
        """
        TOWRITE

        :param `pattern`: TOWRITE
        :type `pattern`: EmbPattern
        :param `item`: TOWRITE
        :type `item`: `QGraphicsItem`_
        """
        pass # TODO: saving polygons, polylines and paths must be stable before we go here.

    def addTextSingle(self, pattern, item):
        """
        TOWRITE

        :param `pattern`: TOWRITE
        :type `pattern`: EmbPattern
        :param `item`: TOWRITE
        :type `item`: `QGraphicsItem`_
        """
        # TODO: saving polygons, polylines and paths must be stable before we go here.

        # TODO: This needs to work like a path, not a polyline. Improve this
        obj = item # TextSingleObject* obj = static_cast<TextSingleObject*>(item);
        if obj:
            if self.formatType == EMBFORMAT_STITCHONLY:
                pathList = obj.objectSavePathList()  # QList<QPainterPath>
                for path in pathList:  # foreach(QPainterPath path, pathList)
                    self.toPolyline(pattern, obj.objectPos(), path.simplified(), "0", obj.objectColor(), "CONTINUOUS", "BYLAYER")  # TODO: proper layer/lineType/lineWeight # TODO: Improve precision, replace simplified
            else:
                pass #PORT# nothing?

    def toPolyline(self, pattern, objPos, objPath, layer, color, lineType, lineWeight):
        """
        TOWRITE

        .. NOTE:: This function should be used to interpret various object types and save them as polylines for stitchOnly formats.

        :param `pattern`: TOWRITE
        :type `pattern`: EmbPattern
        :param `objPos`: TOWRITE
        :type `objPos`: `QPointF`_
        :param `objPath`: TOWRITE
        :type `objPath`: `QPainterPath`_
        :param `layer`: TOWRITE
        :type `layer`: QString
        :param `color`: TOWRITE
        :type `color`: `QColor`_
        :param `lineType`: TOWRITE
        :type `lineType`: QString
        :param `lineWeight`: TOWRITE
        :type `lineWeight`: QString
        """
        startX = objPos.x()  # qreal
        startY = objPos.y()  # qreal
        pointList = 0  # EmbPointList*
        lastPoint = 0  # EmbPointList*
        element = QPainterPath.Element
        for i in range(0, objPath.elementCount()): # for(int i = 0; i < objPath.elementCount(); ++i)
            element = objPath.elementAt(i)
            if not pointList:
                pointList = lastPoint = embPointList_create(element.x + startX, -(element.y + startY))
            else:
                lastPoint = embPointList_add(lastPoint, embPoint_make(element.x + startX, -(element.y + startY)))

        polyObject = embPolylineObject_create(pointList, embColor_make(color.red(), color.green(), color.blue()), 1)  # EmbPolylineObject*  # TODO: proper lineType
        embPattern_addPolylineObjectAbs(pattern, polyObject)


# kate: bom off; indent-mode python; indent-width 4; replace-trailing-space-save on;
