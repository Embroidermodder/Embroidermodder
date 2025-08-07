#ifndef UNDO_EDITOR_H
#define UNDO_EDITOR_H

#include <QDockWidget>

QT_BEGIN_NAMESPACE
class QUndoGroup;
class QUndoStack;
class QUndoView;
QT_END_NAMESPACE

class UndoEditor : public QDockWidget
{
    Q_OBJECT

public:
    UndoEditor(const QString& iconDirectory = QString(), QWidget* widgetToFocus = 0, QWidget* parent = 0); //, Qt::WindowFlags flags = 0);
    ~UndoEditor();

    void addStack(QUndoStack* stack);

    bool canUndo() const;
    bool canRedo() const;

    QString undoText() const;
    QString redoText() const;
protected:

public slots:
    void undo();
    void redo();

    void updateCleanIcon(bool opened);

private:
    QWidget*    focusWidget;

    QString     iconDir;
    int         iconSize;

    QUndoGroup* undoGroup;
    QUndoView*  undoView;
};

#endif
