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

#if 0
UndoEditor::UndoEditor(const std::string& iconDirectory, QWidget* widgetToFocus, QWidget* parent) : QDockWidget(parent)
{
    iconDir = iconDirectory;
    iconSize = 16;
    setMinimumSize(100,100);

    undoGroup = new QUndoGroup(this);
    undoView = new QUndoView(undoGroup, this);
    updateCleanIcon(false);

    setWidget(undoView);
    setWindowTitle(tr("History"));
    setAllowedAreas(Qt::LeftDockWidgetArea | Qt::RightDockWidgetArea);

    this->setFocusProxy(widgetToFocus);
    undoView->setFocusProxy(widgetToFocus);
}

UndoEditor::~UndoEditor()
{
}

void UndoEditor::updateCleanIcon(bool opened)
{
    if (opened)
    {
        undoView->setEmptyLabel(tr("Open"));
        undoView->setCleanIcon(QIcon(iconDir + "/" + "open" + ".png"));
    }
    else
    {
        undoView->setEmptyLabel(tr("New"));
        undoView->setCleanIcon(QIcon(iconDir + "/" + "new" + ".png"));
    }
}

void UndoEditor::addStack(QUndoStack* stack)
{
    undoGroup->addStack(stack);
}

bool UndoEditor::canUndo() const
{
    return undoGroup->canUndo();
}

bool UndoEditor::canRedo() const
{
    return undoGroup->canRedo();
}

std::string UndoEditor::undoText() const
{
    return undoGroup->undoText();
}

std::string UndoEditor::redoText() const
{
    return undoGroup->redoText();
}

void UndoEditor::undo()
{
    undoGroup->undo();
}

void UndoEditor::redo()
{
    undoGroup->redo();
}

#endif


#if 0
UndoableAddCommand::UndoableAddCommand(const std::string& text, BaseObject* obj, View* v, QUndoCommand* parent) : QUndoCommand(parent)
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

UndoableDeleteCommand::UndoableDeleteCommand(const std::string& text, BaseObject* obj, View* v, QUndoCommand* parent) : QUndoCommand(parent)
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
// Move
//==================================================

UndoableMoveCommand::UndoableMoveCommand(double deltaX, double deltaY, const std::string& text, BaseObject* obj, View* v, QUndoCommand* parent) : QUndoCommand(parent)
{
    gview = v;
    object = obj;
    setText(text);
    dx = deltaX;
    dy = deltaY;
}

void UndoableMoveCommand::undo()
{
    object->moveBy(-dx, -dy);
}

void UndoableMoveCommand::redo()
{
    object->moveBy(dx, dy);
}

//==================================================
// Rotate
//==================================================

UndoableRotateCommand::UndoableRotateCommand(double pivotPointX, double pivotPointY, double rotAngle, const std::string& text, BaseObject* obj, View* v, QUndoCommand* parent) : QUndoCommand(parent)
{
    gview = v;
    object = obj;
    setText(text);
    pivotX = pivotPointX;
    pivotY = pivotPointY;
    angle = rotAngle;
}

void UndoableRotateCommand::undo()
{
    rotate(pivotX, pivotY, -angle);
}

void UndoableRotateCommand::redo()
{
    rotate(pivotX, pivotY, angle);
}

void UndoableRotateCommand::rotate(double x, double y, double rot)
{
    double rad = radians(rot);
    double cosRot = cos(rad);
    double sinRot = sin(rad);
    double px = object->scenePos().x();
    double py = object->scenePos().y();
    px -= x;
    py -= y;
    double rotX = px*cosRot - py*sinRot;
    double rotY = px*sinRot + py*cosRot;
    rotX += x;
    rotY += y;

    object->setPos(rotX, rotY);
    object->setRotation(object->rotation()+rot);
}

//==================================================
// Scale
//==================================================

UndoableScaleCommand::UndoableScaleCommand(double x, double y, double scaleFactor, const std::string& text, BaseObject* obj, View* v, QUndoCommand* parent) : QUndoCommand(parent)
{
    gview = v;
    object = obj;
    setText(text);

    //Prevent division by zero and other wacky behavior
    if (scaleFactor <= 0.0)
    {
        dx = 0.0;
        dy = 0.0;
        factor = 1.0;
        QMessageBox::critical(0, QObject::tr("ScaleFactor Error"),
                              QObject::tr("Hi there. If you are not a developer, report this as a bug. "
                              "If you are a developer, your code needs examined, and possibly your head too."));
    }
    else
    {
        //Calculate the offset
        double oldX = object->x();
        double oldY = object->y();
        QLineF scaleLine(x, y, oldX, oldY);
        scaleLine.setLength(scaleLine.length()*scaleFactor);
        double newX = scaleLine.x2();
        double newY = scaleLine.y2();

        dx = newX - oldX;
        dy = newY - oldY;
        factor = scaleFactor;
    }
}

void UndoableScaleCommand::undo()
{
    object->setScale(object->scale()*(1/factor));
    object->moveBy(-dx, -dy);
}

void UndoableScaleCommand::redo()
{
    object->setScale(object->scale()*factor);
    object->moveBy(dx, dy);
}

//==================================================
// Navigation
//==================================================

UndoableNavCommand::UndoableNavCommand(const std::string& type, View* v, QUndoCommand* parent) : QUndoCommand(parent)
{
    gview = v;
    navType = type;
    setText(QObject::tr("Navigation"));
    done = false;
    fromTransform = gview->transform();
    fromCenter = gview->center();
}

bool UndoableNavCommand::mergeWith(const QUndoCommand* newest)
{
    if (newest->id() != id()) // make sure other is also an UndoableNavCommand
         return false;

    const UndoableNavCommand* cmd = static_cast<const UndoableNavCommand*>(newest);
    toTransform = cmd->toTransform;
    toCenter = cmd->toCenter;

    return true;
}

void UndoableNavCommand::undo()
{
    if (!done)
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
        else if (navType == "PanStart")       { /* Do Nothing. We are just recording the spot where the pan started. */  }
        else if (navType == "PanStop")        { /* Do Nothing. We are just recording the spot where the pan stopped. */  }
        else if (navType == "PanLeft")        {
            gview->panLeft();  }
        else if (navType == "PanRight")       { gview->panRight(); }
        else if (navType == "PanUp")          { gview->panUp();    }
        else if (navType == "PanDown")        { gview->panDown();  }
        toTransform = gview->transform();
        toCenter = gview->center();
    }
    else
    {
        gview->setTransform(toTransform);
        gview->centerAt(toCenter);
    }
}

//==================================================
// Grip Edit
//==================================================

UndoableGripEditCommand::UndoableGripEditCommand(const EmbVector beforePoint, const EmbVector afterPoint, const std::string& text, BaseObject* obj, View* v, QUndoCommand* parent) : QUndoCommand(parent)
{
    gview = v;
    object = obj;
    setText(text);
    before = beforePoint;
    after = afterPoint;
}

void UndoableGripEditCommand::undo()
{
    object->gripEdit(after, before);
}

void UndoableGripEditCommand::redo()
{
    object->gripEdit(before, after);
}

//==================================================
// Mirror
//==================================================

UndoableMirrorCommand::UndoableMirrorCommand(double x1, double y1, double x2, double y2, const std::string& text, BaseObject* obj, View* v, QUndoCommand* parent) : QUndoCommand(parent)
{
    gview = v;
    object = obj;
    setText(text);
    mirrorLine = QLineF(x1, y1, x2, y2);
}

void UndoableMirrorCommand::undo()
{
    mirror();
}

void UndoableMirrorCommand::redo()
{
    mirror();
}

void UndoableMirrorCommand::mirror()
{
    //TODO: finish undoable mirror
}
#endif
