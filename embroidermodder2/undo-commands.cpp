#include "undo-commands.h"

#include "object-base.h"
#include "view.h"

//==================================================
// Add
//==================================================

UndoableAddCommand::UndoableAddCommand(const QString& text, BaseObject* obj, View* v, QUndoCommand* parent) : QUndoCommand(parent)
{
    gview = v;
    object = obj;
    setText(text);
}

void UndoableAddCommand::undo()
{
    gview->deleteObject(object);
}

void UndoableAddCommand::redo()
{
    gview->addObject(object);
}

//==================================================
// Delete
//==================================================

UndoableDeleteCommand::UndoableDeleteCommand(const QString& text, BaseObject* obj, View* v, QUndoCommand* parent) : QUndoCommand(parent)
{
    gview = v;
    object = obj;
    setText(text);
}

void UndoableDeleteCommand::undo()
{
    gview->addObject(object);
}

void UndoableDeleteCommand::redo()
{
    gview->deleteObject(object);
}

//==================================================
// Navigation
//==================================================

UndoableNavCommand::UndoableNavCommand(const QString& type, View* v, QUndoCommand* parent) : QUndoCommand(parent)
{
    gview = v;
    navType = type;
    setText("Navigation");
    done = false;
    fromTransform = gview->transform();
    fromCenter = gview->center();
}

bool UndoableNavCommand::mergeWith(const QUndoCommand* newest)
{
    if(newest->id() != id()) // make sure other is also an UndoableNavCommand
         return false;

    const UndoableNavCommand* cmd = static_cast<const UndoableNavCommand*>(newest);
    toTransform = cmd->toTransform;
    toCenter = cmd->toCenter;

    return true;
}

void UndoableNavCommand::undo()
{
    if(!done)
    {
        toTransform = gview->transform();
        toCenter = gview->center();
    }
    done = true;

    gview->setTransform(fromTransform);
    gview->centerAt(fromCenter);
}

void UndoableNavCommand::redo()
{

    if(!done)
    {
        if     (navType == "ZoomInToPoint")  { gview->zoomToPoint(gview->scene()->property(VIEW_MOUSE_POINT).toPoint(), +1); }
        else if(navType == "ZoomOutToPoint") { gview->zoomToPoint(gview->scene()->property(VIEW_MOUSE_POINT).toPoint(), -1); }
        else if(navType == "ZoomExtents")    { gview->zoomExtents(); }
        else if(navType == "ZoomSelected")   { gview->zoomSelected(); }
        else if(navType == "PanStart")       { /* Do Nothing. We are just recording the spot where the pan started. */  }
        else if(navType == "PanStop")        { /* Do Nothing. We are just recording the spot where the pan stopped. */  }
        else if(navType == "PanLeft")        { gview->panLeft();  }
        else if(navType == "PanRight")       { gview->panRight(); }
        else if(navType == "PanUp")          { gview->panUp();    }
        else if(navType == "PanDown")        { gview->panDown();  }
        toTransform = gview->transform();
        toCenter = gview->center();
    }
    else
    {
        gview->setTransform(toTransform);
        gview->centerAt(toCenter);
    }
}

/* kate: bom off; indent-mode cstyle; indent-width 4; replace-trailing-space-save on; */
