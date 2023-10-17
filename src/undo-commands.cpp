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

#include "embroidermodder.h"

/* . */
UndoableCommand::UndoableCommand(String command_, QString text, Geometry* obj, View* v, QUndoCommand* parent) : QUndoCommand(parent)
{
    gview = v;
    object = obj;
    command = command_;
    setText(text);
}

/* . */
UndoableCommand::UndoableCommand(EmbVector delta_, QString text, Geometry* obj, View* v, QUndoCommand* parent) : QUndoCommand(parent)
{
    gview = v;
    object = obj;
    command = "move";
    setText(text);
    delta = delta_;
}

/* . */
UndoableCommand::UndoableCommand(String command, EmbVector point, EmbReal value, QString text, Geometry* obj, View* v, QUndoCommand* parent) : QUndoCommand(parent)
{
    gview = v;
    object = obj;
    setText(text);
    command = command;
    if (command == "scale") {
        pivot = point;
        angle = value;
    }
    else {
        //Prevent division by zero and other wacky behavior
        if (value <= 0.0) {
            delta.x = 0.0;
            delta.y = 0.0;
            factor = 1.0;
            QMessageBox::critical(0,
                QObject::tr("ScaleFactor Error"),
                QObject::tr("Hi there. If you are not a developer, report this as a bug. "
            "If you are a developer, your code needs examined, and possibly your head too."));
        }
        else {
            //Calculate the offset
            EmbVector old;
            old.x = object->x();
            old.y = object->y();
            factor = value;
            QLineF scaleLine(point.x, point.y, old.x, old.y);
            scaleLine.setLength(scaleLine.length() * factor);
            EmbVector new_;
            new_.x = scaleLine.x2();
            new_.y = scaleLine.y2();

            delta = embVector_subtract(new_, old);
        }
    }
}

/* . */
void
UndoableCommand::undo()
{
    if (command == "add") {
        gview->deleteObject(object);
    }
    else if (command == "delete") {
        gview->addObject(object);
    }
    else if (command == "move") {
        object->moveBy(-delta.x, -delta.y);
    }
    else if (command == "rotate") {
        rotate(pivot, -angle);
    }
    else if (command == "scale") {
        object->setScale(object->scale()*(1/factor));
        object->moveBy(-delta.x, -delta.y);
    }
    else if (command == "gripedit") {
        object->gripEdit(after, before);
    }
    else if (command == "mirror") {
        mirror();
    }
    else if (command == "nav") {
        if (!done) {
            toTransform = gview->transform();
            toCenter = gview->center();
        }
        done = true;

        gview->setTransform(fromTransform);
        gview->centerAt(fromCenter);
    }
}

/* . */
void
UndoableCommand::redo()
{
    if (command == "add") {
        gview->addObject(object);
    }
    else if (command == "delete") {
        gview->deleteObject(object);
    }
    else if (command == "move") {
        object->moveBy(delta.x, delta.y);
    }
    else if (command == "rotate") {
        rotate(pivot, angle);
    }
    else if (command == "scale") {
        object->setScale(object->scale()*factor);
        object->moveBy(delta.x, delta.y);
    }
    else if (command == "gripedit") {
        object->gripEdit(before, after);
    }
    else if (command == "mirror") {
        mirror();
    }
    else if (command == "nav") {
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
}

/* . */
void
UndoableCommand::rotate(EmbVector pivot, EmbReal rot)
{
    EmbReal rad = radians(rot);
    EmbVector p = embVector_subtract(to_EmbVector(object->scenePos()), pivot);
    EmbVector rotv = embVector_add(rotate_vector(p, rad), pivot);

    object->setPos(rotv.x, rotv.y);
    object->setRotation(object->rotation() + rot);
}

/* . */
UndoableCommand::UndoableCommand(QString type, View* v, QUndoCommand* parent) : QUndoCommand(parent)
{
    gview = v;
    navType = type;
    setText(QObject::tr("Navigation"));
    command = "nav";
    done = false;
    fromTransform = gview->transform();
    fromCenter = gview->center();
}

/* . */
bool
UndoableCommand::mergeWith(const QUndoCommand* newest)
{
    if (newest->id() != id()) // make sure other is also an UndoableNavCommand
         return false;

    const UndoableCommand* cmd = static_cast<const UndoableCommand*>(newest);
    toTransform = cmd->toTransform;
    toCenter = cmd->toCenter;

    return true;
}

/* . */
UndoableCommand::UndoableCommand(const QPointF beforePoint, const QPointF afterPoint, QString  text, Geometry* obj, View* v, QUndoCommand* parent) : QUndoCommand(parent)
{
    gview = v;
    object = obj;
    setText(text);
    command = "gripedit";
    before = beforePoint;
    after = afterPoint;
}

/* . */
UndoableCommand::UndoableCommand(EmbReal x1, EmbReal y1, EmbReal x2, EmbReal y2, QString  text, Geometry* obj, View* v, QUndoCommand* parent) : QUndoCommand(parent)
{
    gview = v;
    object = obj;
    setText(text);
    command = "mirror";
    mirrorLine = QLineF(x1, y1, x2, y2);
}

/* . */
void
UndoableCommand::mirror()
{
    //TODO: finish undoable mirror
}
