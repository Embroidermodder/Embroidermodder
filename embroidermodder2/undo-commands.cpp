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
// Move
//==================================================

UndoableMoveCommand::UndoableMoveCommand(qreal deltaX, qreal deltaY, const QString& text, BaseObject* obj, View* v, QUndoCommand* parent) : QUndoCommand(parent)
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

UndoableRotateCommand::UndoableRotateCommand(qreal pivotPointX, qreal pivotPointY, qreal rotAngle, const QString& text, BaseObject* obj, View* v, QUndoCommand* parent) : QUndoCommand(parent)
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

void UndoableRotateCommand::rotate(qreal x, qreal y, qreal rot)
{
    qreal rad = radians(rot);
    qreal cosRot = qCos(rad);
    qreal sinRot = qSin(rad);
    qreal px = object->scenePos().x();
    qreal py = object->scenePos().y();
    px -= x;
    py -= y;
    qreal rotX = px*cosRot - py*sinRot;
    qreal rotY = px*sinRot + py*cosRot;
    rotX += x;
    rotY += y;

    object->setPos(rotX, rotY);
    object->setRotation(object->rotation()+rot);
}

//==================================================
// Scale
//==================================================

UndoableScaleCommand::UndoableScaleCommand(qreal x, qreal y, qreal scaleFactor, const QString& text, BaseObject* obj, View* v, QUndoCommand* parent) : QUndoCommand(parent)
{
    gview = v;
    object = obj;
    setText(text);

    //Prevent division by zero and other wacky behavior
    if(scaleFactor <= 0.0)
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
        qreal oldX = object->x();
        qreal oldY = object->y();
        QLineF scaleLine(x, y, oldX, oldY);
        scaleLine.setLength(scaleLine.length()*scaleFactor);
        qreal newX = scaleLine.x2();
        qreal newY = scaleLine.y2();

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
