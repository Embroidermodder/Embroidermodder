/*
 *  Embroidermodder 2.
 *
 *  ------------------------------------------------------------
 *
 *  Copyright 2013-2022 The Embroidermodder Team
 *  Embroidermodder 2 is Open Source Software.
 *  See LICENSE for licensing terms.
 *
 *  ------------------------------------------------------------
 *
 *  Use Python's PEP7 style guide.
 *      https://peps.python.org/pep-0007/
 */

/**
 * \file undo-commands.cpp
 */

#include "embroidermodder.h"

/**
 *
 */
UndoableAddCommand::UndoableAddCommand(const QString& text, BaseObject* obj, View* v, QUndoCommand* parent) : QUndoCommand(parent)
{
    gview = v;
    object = obj;
    setText(text);
}

/**
 *
 */
void
UndoableAddCommand::undo()
{
    gview->deleteObject(object);
}

/**
 *
 */
void UndoableAddCommand::redo()
{
    gview->addObject(object);
}

/**
 *
 */
UndoableDeleteCommand::UndoableDeleteCommand(const QString& text, BaseObject* obj, View* v, QUndoCommand* parent) : QUndoCommand(parent)
{
    gview = v;
    object = obj;
    setText(text);
}

/**
 *
 */
void
UndoableDeleteCommand::undo()
{
    gview->addObject(object);
}

/**
 *
 */
void
UndoableDeleteCommand::redo()
{
    gview->deleteObject(object);
}

/**
 *
 */
UndoableMoveCommand::UndoableMoveCommand(EmbReal deltaX, EmbReal deltaY, const QString& text, BaseObject* obj, View* v, QUndoCommand* parent) : QUndoCommand(parent)
{
    gview = v;
    object = obj;
    setText(text);
    dx = deltaX;
    dy = deltaY;
}

/**
 *
 */
void
UndoableMoveCommand::undo()
{
    object->moveBy(-dx, -dy);
}

/**
 *
 */
void
UndoableMoveCommand::redo()
{
    object->moveBy(dx, dy);
}

/**
 *
 */
UndoableRotateCommand::UndoableRotateCommand(EmbReal pivotPointX, EmbReal pivotPointY, EmbReal rotAngle, const QString& text, BaseObject* obj, View* v, QUndoCommand* parent) : QUndoCommand(parent)
{
    gview = v;
    object = obj;
    setText(text);
    pivotX = pivotPointX;
    pivotY = pivotPointY;
    angle = rotAngle;
}

/**
 *
 */
void
UndoableRotateCommand::undo()
{
    rotate(pivotX, pivotY, -angle);
}

/**
 *
 */
void
UndoableRotateCommand::redo()
{
    rotate(pivotX, pivotY, angle);
}

/**
 *
 */
void
UndoableRotateCommand::rotate(EmbReal x, EmbReal y, EmbReal rot)
{
    EmbReal rad = radians(rot);
    EmbVector p;
    p.x = object->scenePos().x();
    p.y = object->scenePos().y();
    p.x -= x;
    p.y -= y;
    EmbVector rotv = rotate_vector(p, rad);
    rotv.x += x;
    rotv.y += y;

    object->setPos(rotv.x, rotv.y);
    object->setRotation(object->rotation() + rot);
}

/**
 *
 */
UndoableScaleCommand::UndoableScaleCommand(EmbReal x, EmbReal y, EmbReal scaleFactor, const QString& text, BaseObject* obj, View* v, QUndoCommand* parent) : QUndoCommand(parent)
{
    gview = v;
    object = obj;
    setText(text);

    //Prevent division by zero and other wacky behavior
    if (scaleFactor <= 0.0) {
        dx = 0.0;
        dy = 0.0;
        factor = 1.0;
        QMessageBox::critical(0,
            QObject::tr("ScaleFactor Error"),
            QObject::tr("Hi there. If you are not a developer, report this as a bug. "
            "If you are a developer, your code needs examined, and possibly your head too."));
    }
    else {
        //Calculate the offset
        EmbReal oldX = object->x();
        EmbReal oldY = object->y();
        QLineF scaleLine(x, y, oldX, oldY);
        scaleLine.setLength(scaleLine.length()*scaleFactor);
        EmbReal newX = scaleLine.x2();
        EmbReal newY = scaleLine.y2();

        dx = newX - oldX;
        dy = newY - oldY;
        factor = scaleFactor;
    }
}

/**
 *
 */
void
UndoableScaleCommand::undo()
{
    object->setScale(object->scale()*(1/factor));
    object->moveBy(-dx, -dy);
}

/**
 *
 */
void
UndoableScaleCommand::redo()
{
    object->setScale(object->scale()*factor);
    object->moveBy(dx, dy);
}

/**
 *
 */
UndoableNavCommand::UndoableNavCommand(const QString& type, View* v, QUndoCommand* parent) : QUndoCommand(parent)
{
    gview = v;
    navType = type;
    setText(QObject::tr("Navigation"));
    done = false;
    fromTransform = gview->transform();
    fromCenter = gview->center();
}

/**
 *
 */
bool
UndoableNavCommand::mergeWith(const QUndoCommand* newest)
{
    if (newest->id() != id()) // make sure other is also an UndoableNavCommand
         return false;

    const UndoableNavCommand* cmd = static_cast<const UndoableNavCommand*>(newest);
    toTransform = cmd->toTransform;
    toCenter = cmd->toCenter;

    return true;
}

/**
 *
 */
void
UndoableNavCommand::undo()
{
    if (!done) {
        toTransform = gview->transform();
        toCenter = gview->center();
    }
    done = true;

    gview->setTransform(fromTransform);
    gview->centerAt(fromCenter);
}

/**
 *
 */
void
UndoableNavCommand::redo()
{
    if (!done) {
        if (navType == "ZoomInToPoint")  {
            gview->zoomToPoint(gview->scene()->property("VIEW_MOUSE_POINT").toPoint(), +1);
        }
        else if (navType == "ZoomOutToPoint") {
            gview->zoomToPoint(gview->scene()->property("VIEW_MOUSE_POINT").toPoint(), -1);
        }
        else if (navType == "ZoomExtents") {
            gview->zoomExtents();
        }
        else if (navType == "ZoomSelected") {
            gview->zoomSelected();
        }
        else if (navType == "PanStart") {
            /* Do Nothing. We are just recording the spot where the pan started. */
        }
        else if (navType == "PanStop") {
            /* Do Nothing. We are just recording the spot where the pan stopped. */
        }
        else if (navType == "PanLeft") {
            gview->panLeft();
        }
        else if (navType == "PanRight") {
            gview->panRight();
        }
        else if (navType == "PanUp") {
            gview->panUp();
        }
        else if (navType == "PanDown") {
            gview->panDown();
        }
        toTransform = gview->transform();
        toCenter = gview->center();
    }
    else {
        gview->setTransform(toTransform);
        gview->centerAt(toCenter);
    }
}

/**
 *
 */
UndoableGripEditCommand::UndoableGripEditCommand(const QPointF beforePoint, const QPointF afterPoint, const QString& text, BaseObject* obj, View* v, QUndoCommand* parent) : QUndoCommand(parent)
{
    gview = v;
    object = obj;
    setText(text);
    before = beforePoint;
    after = afterPoint;
}

/**
 *
 */
void
UndoableGripEditCommand::undo()
{
    object->gripEdit(after, before);
}

/**
 *
 */
void
UndoableGripEditCommand::redo()
{
    object->gripEdit(before, after);
}

/**
 *
 */
UndoableMirrorCommand::UndoableMirrorCommand(EmbReal x1, EmbReal y1, EmbReal x2, EmbReal y2, const QString& text, BaseObject* obj, View* v, QUndoCommand* parent) : QUndoCommand(parent)
{
    gview = v;
    object = obj;
    setText(text);
    mirrorLine = QLineF(x1, y1, x2, y2);
}

/**
 *
 */
void
UndoableMirrorCommand::undo()
{
    mirror();
}

/**
 *
 */
void
UndoableMirrorCommand::redo()
{
    mirror();
}

/**
 *
 */
void
UndoableMirrorCommand::mirror()
{
    //TODO: finish undoable mirror
}
