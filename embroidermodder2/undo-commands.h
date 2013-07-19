#ifndef UNDO_COMMANDS_H
#define UNDO_COMMANDS_H

#include <QUndoCommand>
#include <QPointF>
#include <QTransform>

class BaseObject;
class View;

class UndoableAddCommand : public QUndoCommand
{
public:
    UndoableAddCommand(const QString& text, BaseObject* obj, View* v, QUndoCommand* parent = 0);

    void undo();
    void redo();

private:
    BaseObject* object;
    View*       gview;
};

class UndoableDeleteCommand : public QUndoCommand
{
public:
    UndoableDeleteCommand(const QString& text, BaseObject* obj, View* v, QUndoCommand* parent = 0);

    void undo();
    void redo();

private:
    BaseObject* object;
    View*       gview;
};

class UndoableNavCommand : public QUndoCommand
{
public:
    UndoableNavCommand(const QString& type, View* v, QUndoCommand* parent = 0);

    int id() const { return 1234; }
    bool mergeWith(const QUndoCommand* command);
    void undo();
    void redo();

private:
    QString navType;
    QTransform fromTransform;
    QTransform toTransform;
    QPointF fromCenter;
    QPointF toCenter;
    bool    done;
    View*   gview;
};

#endif

/* kate: bom off; indent-mode cstyle; indent-width 4; replace-trailing-space-save on; */
