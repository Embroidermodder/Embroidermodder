#ifndef UNDO_COMMANDS_H
#define UNDO_COMMANDS_H

#include <QUndoCommand>
#include <QPointF>
#include <QTransform>
#include <QtCore/qmath.h>

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

class UndoableMoveCommand : public QUndoCommand
{
public:
    UndoableMoveCommand(qreal deltaX, qreal deltaY, const QString& text, BaseObject* obj, View* v, QUndoCommand* parent = 0);

    void undo();
    void redo();

private:
    BaseObject* object;
    View*       gview;
    qreal       dx;
    qreal       dy;
};

class UndoableRotateCommand : public QUndoCommand
{
public:
    UndoableRotateCommand(qreal pivotPointX, qreal pivotPointY, qreal rotAngle, const QString& text, BaseObject* obj, View* v, QUndoCommand* parent = 0);

    void undo();
    void redo();

private:
    void rotate(qreal x, qreal y, qreal rot);

    inline qreal pi() { return (qAtan(1.0)*4.0); };
    inline qreal radians(qreal degrees) { return (degrees*pi()/180.0); };

    BaseObject* object;
    View*       gview;
    qreal       pivotX;
    qreal       pivotY;
    qreal       angle;
};

class UndoableScaleCommand : public QUndoCommand
{
public:
    UndoableScaleCommand(qreal x, qreal y, qreal scaleFactor, const QString& text, BaseObject* obj, View* v, QUndoCommand* parent = 0);

    void undo();
    void redo();

private:
    BaseObject* object;
    View*       gview;
    qreal       dx;
    qreal       dy;
    qreal       factor;
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

class UndoableGripEditCommand : public QUndoCommand
{
public:
    UndoableGripEditCommand(const QPointF beforePoint, const QPointF afterPoint, const QString& text, BaseObject* obj, View* v, QUndoCommand* parent = 0);

    void undo();
    void redo();

private:
    BaseObject* object;
    View*       gview;
    QPointF     before;
    QPointF     after;
};


class UndoableMirrorCommand : public QUndoCommand
{
public:
    UndoableMirrorCommand(qreal x1, qreal y1, qreal x2, qreal y2, const QString& text, BaseObject* obj, View* v, QUndoCommand* parent = 0);

    void undo();
    void redo();

private:
    void mirror();

    BaseObject* object;
    View*       gview;
    QLineF      mirrorLine;

};

#endif

/* kate: bom off; indent-mode cstyle; indent-width 4; replace-trailing-space-save on; */
