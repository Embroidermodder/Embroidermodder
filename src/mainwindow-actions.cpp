/*
 * Embroidermodder 2.
 *
 * Copyright 2011-2024 The Embroidermodder Team
 * Embroidermodder 2 is Open Source Software, see LICENSE.md for licensing terms.
 * Visit https://www.libembroidery.org/refman for advice on altering this file,
 * or read the markdown version in embroidermodder2/docs/refman.
 *
 * MainWindow Actions
 */

#include "embroidermodder.h"

#include <QAction>
#include <QApplication>
#include <QMdiArea>

/* For each Command in command_data, for each alias set up a map from
 * alias to the Command. Then for a given context the call doesn't have to loop?
 *
 * NOTE: Every QScriptProgram must have a unique function name to call. If every function was called main(), then
 *       the ScriptArgs would only call the last script evaluated (which happens to be main() in another script).
 *       Thus, by adding the cmdName before main(), it becomes line_main(), circle_main(), etc...
 *       Do not change this code unless you really know what you are doing. I mean it.
 *
 * Position currently comes from the order of the command_data.
 *
 * TODO: Set What's This Context Help to statusTip for now so there is some infos there.
 *       Make custom whats this context help popup with more descriptive help than just
 *       the status bar/tip one liner(short but not real long) with a hyperlink in the custom popup
 *       at the bottom to open full help file description. Ex: like wxPython AGW's SuperToolTip.
 *
 * TODO: Finish All Commands ... <.<
 */
void
MainWindow::createAllActions()
{
    qDebug("Creating All Actions...");
    QString appName = QApplication::applicationName();
    QString appDir = qApp->applicationDirPath();
    QString iconTheme = settings_general_icon_theme;

    for (int i=0; command_data[i].id != -2; i++) {
        QString icon(command_data[i].icon);
        QString toolTip(command_data[i].tooltip);
        QString statusTip(command_data[i].statustip);
        QString alias_string(command_data[i].alias);
        QStringList aliases = alias_string.split(", ");

        qDebug("COMMAND: %s", qPrintable(icon));

        if (!command_map.contains(icon)) {
            command_map.insert(icon, command_data[i]);
        }
        else {
            qDebug("ERROR: command_map key collision.");
        }

        QAction *ACTION = new QAction(QIcon(appDir + "/icons/" + iconTheme + "/" + icon + ".png"), toolTip, this);
        ACTION->setStatusTip(statusTip);
        ACTION->setObjectName(icon);
        ACTION->setWhatsThis(statusTip);

        if (strcmp(command_data[i].shortcut, "")) {
            ACTION->setShortcut(QKeySequence(command_data[i].shortcut));
        }

        if (icon == "textbold" || icon == "textitalic" || icon == "textunderline"
            || icon == "textstrikeout" || icon == "textoverline") {
            ACTION->setCheckable(true);
        }

        connect(ACTION, SIGNAL(triggered()), this, SLOT(runCommand()));

        aliasHash->insert(icon, icon);
        actionHash.insert(command_data[i].id, ACTION);

        foreach (QString alias, aliases) {
            prompt->addCommand(alias, icon);
        }
    }

    actionHash.value(ACTION_WINDOW_CLOSE)->setEnabled(numOfDocs > 0);
    actionHash.value(ACTION_DESIGN_DETAILS)->setEnabled(numOfDocs > 0);
}
