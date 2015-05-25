#!/usr/bin/env python
# -*- coding: utf-8 -*-

"""
=================================
|module_summary| undo_commands.py
=================================

TOWRITE

Classes summary:
================

================================== ==================================
:class:`~UndoableAddCommand`       TOWRITE
:class:`~UndoableDeleteCommand`    TOWRITE
:class:`~UndoableMoveCommand`      TOWRITE
:class:`~UndoableRotateCommand`    TOWRITE
:class:`~UndoableScaleCommand`     TOWRITE
:class:`~UndoableNavCommand`       TOWRITE
:class:`~UndoableGripEditCommand`  TOWRITE
:class:`~UndoableMirrorCommand`    TOWRITE
================================== ==================================

---------------------------------------------------------------------

|

"""

#-Imports.---------------------------------------------------------------------
#--Python Imports.
from math import sin as qSin
from math import cos as qCos
from math import radians

#--PySide/PyQt Imports.
if PYSIDE:
    ## from PySide import QtCore, QtGui
    # or... Improve performace with less dots...
    from PySide.QtCore import QLineF, QObject
    from PySide.QtGui import QMessageBox, QUndoCommand
elif PYQT4:
    import sip
    sip.setapi('QString', 2)
    sip.setapi('QVariant', 2)
    ## from PyQt4 import QtCore, QtGui
    # or... Improve performace with less dots...
    from PyQt4.QtCore import QLineF, QObject
    from PyQt4.QtGui import QMessageBox, QUndoCommand


# C++C++C++C++C++C++C++C++C++C++C++C++C++C++C++C++C++C++C++C++C++C++C++C++C++
#include "undo-commands.h"

#include "object-base.h"
#include "view.h"
# C++C++C++C++C++C++C++C++C++C++C++C++C++C++C++C++C++C++C++C++C++C++C++C++C++


class UndoableAddCommand(QUndoCommand):
    """
    Subclass of `QUndoCommand`_

    TOWRITE

    Add

    """
    def __init__(self, text, obj, v, parent=None):
        """
        Default class constructor.

        :param `text`: TOWRITE
        :type `text`: QString
        :param `obj`: TOWRITE
        :type `obj`: `BaseObject`
        :param `v`: TOWRITE
        :type `v`: `View`
        :param `parent`: TOWRITE
        :type `parent`: `QUndoCommand`_
        """
        super(UndoableAddCommand, self).__init__(parent)

        self.gview = v
        self.object = obj
        self.setText(text)


    def undo(self):
        """
        TOWRITE
        """
        self.gview.deleteObject(self.object)

    def redo(self):
        """
        TOWRITE
        """
        self.gview.addObject(self.object)


class UndoableDeleteCommand(QUndoCommand):
    """
    Subclass of `QUndoCommand`_

    TOWRITE

    Delete

    """
    def __init__(self, text, obj, v, parent=None):
        """
        Default class constructor.

        :param `text`: TOWRITE
        :type `text`: QString
        :param `obj`: TOWRITE
        :type `obj`: `BaseObject`
        :param `v`: TOWRITE
        :type `v`: `View`
        :param `parent`: TOWRITE
        :type `parent`: `QUndoCommand`_
        """
        super(UndoableDeleteCommand, self).__init__(parent)

        self.gview = v
        self.object = obj
        self.setText(text)


    def undo(self):
        """
        TOWRITE
        """
        self.gview.deleteObject(self.object)

    def redo(self):
        """
        TOWRITE
        """
        self.gview.addObject(self.object)


class UndoableMoveCommand(QUndoCommand):
    """
    Subclass of `QUndoCommand`_

    TOWRITE

    Move

    """
    def __init__(self, deltaX, deltaY, text, obj, v, parent=None):
        """
        Default class constructor.

        :param `deltaX`: TOWRITE
        :type `deltaX`: qreal
        :param `deltaY`: TOWRITE
        :type `deltaY`: qreal
        :param `text`: TOWRITE
        :type `text`: QString
        :param `obj`: TOWRITE
        :type `obj`: `BaseObject`
        :param `v`: TOWRITE
        :type `v`: `View`
        :param `parent`: TOWRITE
        :type `parent`: `QUndoCommand`_
        """
        super(UndoableMoveCommand, self).__init__(parent)

        self.gview = v
        self.object = obj
        self.setText(text)
        self.dx = deltaX
        self.dy = deltaY


    def undo(self):
        """
        TOWRITE
        """
        self.object.moveBy(-self.dx, -self.dy)

    def redo(self):
        """
        TOWRITE
        """
        self.object.moveBy(self.dx, self.dy)


class UndoableRotateCommand(QUndoCommand):
    """
    Subclass of `QUndoCommand`_

    TOWRITE

    Rotate

    """
    def __init__(self, pivotPointX, pivotPointY, rotAngle, text, obj, v, parent=None):
        """
        Default class constructor.

        :param `pivotPointX`: TOWRITE
        :type `pivotPointX`: qreal
        :param `pivotPointY`: TOWRITE
        :type `pivotPointY`: qreal
        :param `rotAngle`: TOWRITE
        :type `rotAngle`: qreal
        :param `text`: TOWRITE
        :type `text`: QString
        :param `obj`: TOWRITE
        :type `obj`: `BaseObject`
        :param `v`: TOWRITE
        :type `v`: `View`
        :param `parent`: TOWRITE
        :type `parent`: `QUndoCommand`_
        """
        super(UndoableRotateCommand, self).__init__(parent)

        self.gview = v
        self.object = obj
        self.setText(text)
        self.pivotX = pivotPointX
        self.pivotY = pivotPointY
        self.angle = rotAngle


    def undo(self):
        """
        TOWRITE
        """
        self.rotate(self.pivotX, self.pivotY, -self.angle)

    def redo(self):
        """
        TOWRITE
        """
        self.rotate(self.pivotX, self.pivotY, self.angle)

    def rotate(self, x, y, rot):
        """
        TOWRITE

        :param `x`: TOWRITE
        :type `x`: qreal
        :param `y`: TOWRITE
        :type `y`: qreal
        :param `rot`: TOWRITE
        :type `rot`: qreal
        """
        rad = radians(rot)  # qreal
        cosRot = qCos(rad)  # qreal
        sinRot = qSin(rad)  # qreal
        px = self.object.scenePos().x()  # qreal
        py = self.object.scenePos().y()  # qreal
        px -= x
        py -= y
        rotX = px * cosRot - py * sinRot  # qreal
        rotY = px * sinRot + py * cosRot  # qreal
        rotX += x
        rotY += y

        self.object.setPos(rotX, rotY)
        self.object.setRotation(self.object.rotation() + rot)


class UndoableScaleCommand(QUndoCommand):
    """
    Subclass of `QUndoCommand`_

    TOWRITE

    Scale

    """
    def __init__(self, x, y, scaleFactor, text, obj, v, parent=None):
        """
        Default class constructor.

        :param `x`: TOWRITE
        :type `x`: qreal
        :param `y`: TOWRITE
        :type `y`: qreal
        :param `scaleFactor`: TOWRITE
        :type `scaleFactor`: qreal
        :param `text`: TOWRITE
        :type `text`: QString
        :param `obj`: TOWRITE
        :type `obj`: `BaseObject`
        :param `v`: TOWRITE
        :type `v`: `View`
        :param `parent`: TOWRITE
        :type `parent`: `QUndoCommand`_
        """
        super(UndoableScaleCommand, self).__init__(parent)

        self.gview = v
        self.object = obj
        self.setText(text)

        # Prevent division by zero and other wacky behavior
        if scaleFactor <= 0.0:
            self.dx = 0.0
            self.dy = 0.0
            self.factor = 1.0
            QMessageBox.critical(0, QObject.tr("ScaleFactor Error"),
                                 QObject.tr("Hi there. If you are not a developer, report this as a bug. "
                                 "If you are a developer, your code needs examined, and possibly your head too."))
        else:
            # Calculate the offset
            oldX = self.object.x()  # qreal
            oldY = self.object.y()  # qreal
            scaleLine = QLineF(x, y, oldX, oldY)
            scaleLine.setLength(scaleLine.length() * scaleFactor)
            newX = scaleLine.x2()  # qreal
            newY = scaleLine.y2()  # qreal

            self.dx = newX - oldX
            self.dy = newY - oldY
            self.factor = scaleFactor


    def undo(self):
        """
        TOWRITE
        """
        self.object.setScale(self.object.scale() * (1 / self.factor))
        self.object.moveBy(-self.dx, -self.dy)

    def redo(self):
        """
        TOWRITE
        """
        self.object.setScale(self.object.scale() * self.factor)
        self.object.moveBy(self.dx, self.dy)


class UndoableNavCommand(QUndoCommand):
    """
    Subclass of `QUndoCommand`_

    TOWRITE

    Navigation

    """
    def __init__(self, type, v, parent=None):
        """
        Default class constructor.

        :param `type`: TOWRITE
        :type `type`: QString
        :param `v`: TOWRITE
        :type `v`: `View`
        :param `parent`: TOWRITE
        :type `parent`: `QUndoCommand`_
        """
        super(UndoableNavCommand, self).__init__(parent)

        self.gview = v
        self.navType = type
        self.setText(v.tr("Navigation"))
        self.done = False
        self.fromTransform = self.gview.transform()
        self.fromCenter = self.gview.center()
        self.toTransform = None
        self.toCenter = None

    def mergeWith(self, newest):
        """
        TOWRITE

        :param `newest`: TOWRITE
        :type `newest`: QUndoCommand
        :rtype: bool
        """
        if newest.id() != self.id(): # make sure other is also an UndoableNavCommand
            return False

        cmd = newest # const UndoableNavCommand* cmd = static_cast<const UndoableNavCommand*>(newest);
        self.toTransform = cmd.toTransform
        self.toCenter = cmd.toCenter

        return True

    def undo(self):
        """
        TOWRITE
        """
        if not self.done:

            self.toTransform = self.gview.transform()
            self.toCenter = self.gview.center()

        self.done = True

        self.gview.setTransform(self.fromTransform)
        self.gview.centerAt(self.fromCenter)

    def redo(self):
        """
        TOWRITE
        """
        if not self.done:
            navType = self.navType
            if   navType == "ZoomInToPoint":  self.gview.zoomToPoint(self.gview.scene().property("VIEW_MOUSE_POINT"), +1)  # .toPoint()
            elif navType == "ZoomOutToPoint": self.gview.zoomToPoint(self.gview.scene().property("VIEW_MOUSE_POINT"), -1)  # .toPoint()
            elif navType == "ZoomExtents":    self.gview.zoomExtents()
            elif navType == "ZoomSelected":   self.gview.zoomSelected()
            elif navType == "PanStart":       pass  # { /* Do Nothing. We are just recording the spot where the pan started. */  }
            elif navType == "PanStop":        pass  # { /* Do Nothing. We are just recording the spot where the pan stopped. */  }
            elif navType == "PanLeft":        self.gview.panLeft()
            elif navType == "PanRight":       self.gview.panRight()
            elif navType == "PanUp":          self.gview.panUp()
            elif navType == "PanDown":        self.gview.panDown()
            self.toTransform = self.gview.transform()
            self.toCenter = self.gview.center()

        else:
            self.gview.setTransform(self.toTransform)
            self.gview.centerAt(self.toCenter)


class UndoableGripEditCommand(QUndoCommand):
    """
    Subclass of `QUndoCommand`_

    TOWRITE

    Grip Edit

    """
    def __init__(self, beforePoint, afterPoint, text, obj, v, parent=None):
        """
        Default class constructor.

        :param `beforePoint`: TOWRITE
        :type `beforePoint`: QPointF
        :param `afterPoint`: TOWRITE
        :type `afterPoint`: QPointF
        :param `text`: TOWRITE
        :type `text`: QString
        :param `obj`: TOWRITE
        :type `obj`: `BaseObject`
        :param `v`: TOWRITE
        :type `v`: `View`
        :param `parent`: TOWRITE
        :type `parent`: `QUndoCommand`_
        """
        super(UndoableGripEditCommand, self).__init__(parent)

        self.gview = v
        self.object = obj
        self.setText(text)
        self.before = beforePoint
        self.after = afterPoint


    def undo(self):
        """
        TOWRITE
        """
        self.object.gripEdit(self.after, self.before)

    def redo(self):
        """
        TOWRITE
        """
        self.object.gripEdit(self.before, self.after)


class UndoableMirrorCommand(QUndoCommand):
    """
    Subclass of `QUndoCommand`_

    TOWRITE

    Mirror

    """
    def __init__(self, x1, y1, x2, y2, text, obj, v, parent=None):
        """
        Default class constructor.

        :param `x1`: TOWRITE
        :type `x1`: qreal
        :param `y1`: TOWRITE
        :type `y1`: qreal
        :param `x2`: TOWRITE
        :type `x2`: qreal
        :param `y2`: TOWRITE
        :type `y2`: qreal
        :param `text`: TOWRITE
        :type `text`: QString
        :param `obj`: TOWRITE
        :type `obj`: `BaseObject`
        :param `v`: TOWRITE
        :type `v`: `View`
        :param `parent`: TOWRITE
        :type `parent`: `QUndoCommand`_
        """
        super(UndoableMirrorCommand, self).__init__(parent)

        self.gview = v
        self.object = obj
        self.setText(text)
        self.mirrorLine = QLineF(x1, y1, x2, y2)


    def undo(self):
        """
        TOWRITE
        """
        self.mirror()

    def redo(self):
        """
        TOWRITE
        """
        self.mirror()

    def mirror(self):
        """
        TOWRITE

        .. TODO:: finish undoable mirror

        """
        # TODO: finish undoable mirror



# kate: bom off; indent-mode python; indent-width 4; replace-trailing-space-save on;
