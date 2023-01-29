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

/* For these transformations, we need to store the relavant data that isn't
 * in the command itself. For example if you delete an object then reference
 * to the object is passed, not the object itself. So we need to put that object
 * in a deleted objects buffer for later access. Therefore the opposite of
 * delete is a new command restore.
 */
string_matrix undo_table = {
    {"pan left", "pan right"},
    {"pan right", "pan left"},
    {"pan up", "pan down"},
    {"pan down", "pan up"},
    {"zoom in", "zoom out"},
    {"zoom out", "zoom in"},
    {"scale $1", "scale 1.0/$1"},
    {"add $@", "remove $1"},
    {"delete $1", "restore $1"},
    {"restore $1", "delete $1"},
    {"move $1 $2", "move -$1 -$2"},
    {"rotate $1 $2 $3", "rotate $1 $2 -$3"},
    {"mirror $1 $2 $3 $4", "mirror $1 $2 $3 $4"},
    {"grip-edit $1 $2 $3 $4", "grip-edit $3 $4 $1 $2"}
};

#if 0
void UndoEditor(std::string iconDirectory,
    QWidget* widgetToFocus, QWidget* parent)
{
    iconDir = iconDirectory;
    iconSize = 16;
    setMinimumSize(100,100);

    undoGroup = new QUndoGroup(this);
    undoView = new QUndoView(undoGroup, this);
    updateCleanIcon(false);

    setWidget(undoView);
    setWindowTitle(translate("History"));
    setAllowedAreas(Qt::LeftDockWidgetArea | Qt::RightDockWidgetArea);

    this->setFocusProxy(widgetToFocus);
    undoView->setFocusProxy(widgetToFocus);
}

void updateCleanIcon(bool opened)
{
    if (opened) {
        undoView->setEmptyLabel(tr("Open"));
        undoView->setCleanIcon(QIcon(iconDir + "/" + "open" + ".png"));
    }
    else {
        undoView->setEmptyLabel(tr("New"));
        undoView->setCleanIcon(QIcon(iconDir + "/" + "new" + ".png"));
    }
}

/* Actions that were Qt internals. */
addStack, canUndo, canRedo, undoText, redoText, undo, redo

void rotate(EmbVector point, double rot)
{
    double alpha = radians(rot);
    EmbVector p = embVector_subtract(object->scenePos(), point);
    EmbVector rot = embVector_rotate(p, alpha);
    rot = embVector(rot, point);

    object->setPos(rot);
    object->setRotation(object->rotation() + alpha);
}

// Scale
void scale_action(EmbVector position, double scaleFactor,
    std::string& text, BaseObject* obj, View* v)
{
    gview = v;
    object = obj;
    setText(text);

    // Prevent division by zero and other wacky behavior
    if (scaleFactor <= 0.0) {
        delta.x = 0.0;
        delta.y = 0.0;
        factor = 1.0;
        critical_message(0, translate("ScaleFactor Error"),
                        translate("Hi there. If you are not a developer, report this as a bug. "
                        "If you are a developer, your code needs examined, and possibly your head too."));
    }
    else {
        //Calculate the offset
        EmbLine scaleLine(position, object->position);
        scaleLine.setLength(scaleLine.length()*scaleFactor);

        delta = embVector_subtract(scaleLine.end, object->position.x);
        factor = scaleFactor;
    }
}

void scale_undo()
{
    object->setScale(object->scale()*(1/factor));
    object->moveBy(-dx, -dy);
}

void scale_redo()
{
    object->setScale(object->scale()*factor);
    object->moveBy(dx, dy);
}

void UndoableNavCommand(std::string type, View* v)
{
    gview = v;
    navType = type;
    setText(translate("Navigation"));
    done = false;
    fromTransform = gview->transform();
    fromCenter = gview->center();
}

bool mergeWith(QUndoCommand* newest)
{
    if (newest->id() != id()) {
        // make sure other is also an UndoableNavCommand
         return false;
    }

    UndoableNavCommand* cmd = static_cast<const UndoableNavCommand*>(newest);
    toTransform = cmd->toTransform;
    toCenter = cmd->toCenter;

    return true;
}

void undo_navigation()
{
    if (!done) {
        toTransform = gview->transform();
        toCenter = gview->center();
    }
    done = true;

    gview->setTransform(fromTransform);
    gview->centerAt(fromCenter);
}

void redo_navigation()
{
    if (!done) {
        if (navType == "ZoomInToPoint")  {
            gview->zoomToPoint(
                gview->scene()->property("VIEW_MOUSE_POINT").toPoint(),
                +1);
        }
        else if (navType == "ZoomOutToPoint") {
            gview->zoomToPoint(
                gview->scene()->property("VIEW_MOUSE_POINT").toPoint(),
                -1);
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
    else
    {
        gview->setTransform(toTransform);
        gview->centerAt(toCenter);
    }
}
#endif
