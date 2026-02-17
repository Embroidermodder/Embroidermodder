#include <QAction>
#include <QApplication>
#include <QMdiArea>

#include "mainwindow.h"
#include "commands.h"

void
MainWindow::createAllActions()
{
    qDebug("Creating All Actions...");
    for (int i=0; command_table[i].id != ACTION_null; i++) {
        /* TODO: override the shortcuts here. */
        QAction *ACTION = createAction(command_table[i]);
        actionHash.insert(command_table[i].id, ACTION);
    }

    actionHash.value(ACTION_windowclose)->setEnabled(numOfDocs > 0);
    actionHash.value(ACTION_designdetails)->setEnabled(numOfDocs > 0);
}

QAction *
MainWindow::createAction(CommandData command, bool scripted)
{
    QString appDir = qApp->applicationDirPath();

    QString icon_path = appDir + "/icons/" + state.settings.general_icon_theme
        + "/" + QString(command.label) + ".png";
    QAction *ACTION = new QAction(QIcon(icon_path), command.tooltip, this);
    ACTION->setStatusTip(command.statustip);
    ACTION->setObjectName(command.label);
    // TODO: Set What's This Context Help to statusTip for now so there is some infos there.
    // Make custom whats this context help popup with more descriptive help than just
    // the status bar/tip one liner(short but not real long) with a hyperlink in the custom popup
    // at the bottom to open full help file description. Ex: like wxPython AGW's SuperToolTip.
    ACTION->setWhatsThis(command.statustip);
    // TODO: Finish All Commands ... <.<


    /* FIXME: detect if we should use PC or mac shortcuts here. This only supports PC. */
    if (strlen(command.shortcut) > 0) {
        ACTION->setShortcut(QKeySequence(command.shortcut));
    }
    /*
    if (strlen(command.mac_shortcut) > 0) {
        ACTION->setShortcut(QKeySequence(command.mac_shortcut));
    }
    */

    if (scripted) {
        ACTION->setIcon(QIcon(appDir + "/commands/" + QString(command.label)
            + "/" + QString(command.label) + ".png"));
        connect(ACTION, SIGNAL(triggered()), this, SLOT(runCommand()));
    }
    else {
        switch (command.type) {
        case CMD_TYPE_TOGGLE: {
            ACTION->setCheckable(true);
            connect(ACTION, &QAction::toggle, this, [=]() { call(command.label); });
            break;
        }
        default:
        case CMD_TYPE_TRIGGER:
            connect(ACTION, &QAction::triggered, this, [=]() { call(command.label); });
            break;
        }
    }
    return ACTION;
}

