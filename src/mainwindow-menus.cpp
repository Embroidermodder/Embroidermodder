/*
 * Embroidermodder 2.
 *
 * Copyright 2011-2024 The Embroidermodder Team
 * Embroidermodder 2 is Open Source Software, see LICENSE.md for licensing terms.
 * Visit https://www.libembroidery.org/refman for advice on altering this file,
 * or read the markdown version in embroidermodder2/docs/refman.
 *
 * MainWindow Menus
 */

#include "embroidermodder.h"
#include <QDebug>
#include <QMenu>
#include <QMenuBar>
#include <QAction>

/* */
QAction*
get_action_by_icon(const char *icon)
{
    int i;
    for (i=0; command_data[i].id != -2; i++) {
        if (!strcmp(command_data[i].icon, icon)) {
            return actionHash.value(command_data[i].id);
        }
    }
    return actionHash.value(ACTION_DO_NOTHING);
}

/* */
void
add_to_menu(QMenu *menu, string_table menu_data)
{
    for (int i=0; strcmp(menu_data[i], "END"); i++) {
        if (menu_data[i][0] == '-') {
            menu->addSeparator();
        }
        else if (menu_data[i][0] == '>') {
            menu->addMenu(menuHash[menu_data[i]+1]);
        }
        else if (menu_data[i][0] == '+') {
            QString appDir = qApp->applicationDirPath();
            QString icontheme = general_icon_theme.setting;
            QString s = appDir + "/icons/" + icontheme + "/" + QString(menu_data[i]+1) + ".png";
            menu->setIcon(QIcon(s));
        }
        else {
            menu->addAction(get_action_by_icon(menu_data[i]));
        }
    }
    menu->setTearOffEnabled(false);
}

void MainWindow::createAllMenus()
{
    qDebug("MainWindow createAllMenus()");

    QString appDir = qApp->applicationDirPath();
    QString icontheme = general_icon_theme.setting;

    menuBar()->addMenu(menuHash["File"]);
    menuHash["File"]->addAction(actionHash.value(ACTION_NEW));
    menuHash["File"]->addSeparator();
    menuHash["File"]->addAction(actionHash.value(ACTION_OPEN));

    menuHash["File"]->addMenu(menuHash["Recent"]);
    connect(menuHash["Recent"], SIGNAL(aboutToShow()), this, SLOT(recentMenuAboutToShow()));
    //Do not allow the Recent Menu to be torn off. It's a pain in the ass to maintain.
    menuHash["Recent"]->setTearOffEnabled(false);

    menuHash["File"]->addSeparator();
    menuHash["File"]->addAction(actionHash.value(ACTION_SAVE));
    menuHash["File"]->addAction(actionHash.value(ACTION_SAVE_AS));
    menuHash["File"]->addSeparator();
    menuHash["File"]->addAction(actionHash.value(ACTION_PRINT));
    menuHash["File"]->addSeparator();
    menuHash["File"]->addAction(actionHash.value(ACTION_WINDOW_CLOSE));
    menuHash["File"]->addSeparator();
    menuHash["File"]->addAction(actionHash.value(ACTION_DESIGN_DETAILS));
    menuHash["File"]->addSeparator();

    menuHash["File"]->addAction(actionHash.value(ACTION_EXIT));
    menuHash["File"]->setTearOffEnabled(false);

    menuBar()->addMenu(menuHash["Edit"]);
    add_to_menu(menuHash["Edit"], edit_menu);

    menuBar()->addMenu(menuHash["View"]);
    add_to_menu(menuHash["View"], view_menu);
    add_to_menu(menuHash["Zoom"], zoom_menu);
    add_to_menu(menuHash["Pan"], pan_menu);

    menuBar()->addMenu(menuHash["Tools"]);
    add_to_menu(menuHash["Tools"], tools_menu);

    menuBar()->addMenu(menuHash["Draw"]);
    add_to_menu(menuHash["Draw"], draw_menu);

    menuBar()->addMenu(menuHash["Dimension"]);
    add_to_menu(menuHash["Dimension"], dimension_menu);

    menuBar()->addMenu(menuHash["Modify"]);
    add_to_menu(menuHash["Modify"], modify_menu);

    menuBar()->addMenu(menuHash["Sandbox"]);
    add_to_menu(menuHash["Sandbox"], sandbox_menu);

    menuBar()->addMenu(menuHash["Settings"]);
    menuHash["Settings"]->addAction(actionHash.value(ACTION_SETTINGS_DIALOG));
    menuHash["Settings"]->addSeparator();
    menuHash["Settings"]->setTearOffEnabled(false);

    menuBar()->addMenu(menuHash["Window"]);
    connect(menuHash["Window"], SIGNAL(aboutToShow()), this, SLOT(windowMenuAboutToShow()));
    /* Do not allow the Window Menu to be torn off. It's a pain in the ass to maintain. */
    menuHash["Window"]->setTearOffEnabled(false);

    menuBar()->addMenu(menuHash["Help"]);
    add_to_menu(menuHash["Help"], help_menu);
}
