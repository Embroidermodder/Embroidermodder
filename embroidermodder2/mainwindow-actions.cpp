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

    for (int i=0; command_data[i].menu_position >= 0; i++) {
        QString icon(command_data[i].icon);
        QString menuName(command_data[i].menu_name);
        int menuPos = command_data[i].menu_position;
        QString toolbarName(command_data[i].toolbar_name);
        int toolbarPos = command_data[i].toolbar_position;
        QString toolTip(command_data[i].tooltip);
        QString statusTip(command_data[i].statustip);
        QString alias_string(command_data[i].alias);
        /* QStringList aliases = alias_string.toStringList(); */

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

        // load globals: engine->evaluate(script);

        if (toolbarName.toUpper() != "NONE") {
            //If the toolbar doesn't exist, create it.
            if (!toolbarHash.value(toolbarName)) {
                QToolBar* tb = new QToolBar(toolbarName, this);
                tb->setObjectName("toolbar" + toolbarName);
                connect(tb, SIGNAL(topLevelChanged(bool)), this,
                    SLOT(floatingChangedToolBar(bool)));
                addToolBar(Qt::LeftToolBarArea, tb);
                addToolBarBreak(Qt::LeftToolBarArea);
                toolbarHash.insert(toolbarName, tb);
            }

            //TODO: order actions position in toolbar based on .ini setting
            toolbarHash.value(toolbarName)->addAction(ACTION);
        }

        if (menuName.toUpper() != "NONE") {
            //If the menu doesn't exist, create it.
            if (!menuHash.value(menuName)) {
                QMenu* menu = new QMenu(menuName, this);
                menu->setTearOffEnabled(true);
                menuBar()->addMenu(menu);
                menuHash.insert(menuName, menu);
            }

            //TODO: order actions position in menu based on .ini setting
            menuHash.value(menuName)->addAction(ACTION);
        }

        /*
        foreach (QString alias, aliases) {
            prompt->addCommand(alias, cmdName);
        }
        */
    }

    actionHash.value(ACTION_WINDOW_CLOSE)->setEnabled(numOfDocs > 0);
    actionHash.value(ACTION_DESIGN_DETAILS)->setEnabled(numOfDocs > 0);
}
