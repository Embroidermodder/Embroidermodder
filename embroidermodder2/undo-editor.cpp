#include <QUndoGroup>
#include <QUndoStack>
#include <QUndoView>
#include <QKeyEvent>

#include "undo-editor.h"
#include "undo-commands.h"

UndoEditor::UndoEditor(const QString& iconDirectory, QWidget* widgetToFocus, QWidget* parent, Qt::WindowFlags flags) : QDockWidget(parent, flags)
{
    iconDir = iconDirectory;
    iconSize = 16;
    setMinimumSize(100,100);

    undoGroup = new QUndoGroup(this);
    undoView = new QUndoView(undoGroup, this);
    undoView->setEmptyLabel("New");
    undoView->setCleanIcon(QIcon(iconDir + "/" + "new" + ".png")); //TODO: new.png for new drawings, open.png for opened drawings, save.png for saved/cleared drawings?

    setWidget(undoView);
    setWindowTitle(tr("History"));
    setAllowedAreas(Qt::LeftDockWidgetArea | Qt::RightDockWidgetArea);

    this->setFocusProxy(widgetToFocus);
    undoView->setFocusProxy(widgetToFocus);
}

UndoEditor::~UndoEditor()
{
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

QString UndoEditor::undoText() const
{
    return undoGroup->undoText();
}

QString UndoEditor::redoText() const
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

/* kate: bom off; indent-mode cstyle; indent-width 4; replace-trailing-space-save on; */
