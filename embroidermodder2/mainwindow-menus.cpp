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

/**
 * \file mainwindow-menus.cpp
 */

#include "embroidermodder.h"

StringList file_menu;
StringList edit_menu;
StringList pan_menu;
StringList zoom_menu;
StringList view_menu;
StringList settings_menu;
StringList window_menu;
StringList help_menu;

/**
 * @brief create_menu
 * @param menu
 * @param def
 * @param topLevel
 */
static void
create_menu(QString menu, StringList def, bool topLevel)
{
    if (topLevel) {
        _mainWin->menuBar()->addMenu(menuHash[menu]);
    }
    for (int i=0; i<(int)def.size(); i++) {
        if (def[i] == "---") {
            if (topLevel) {
                menuHash[menu]->addSeparator();
            }
            else {
                subMenuHash[menu]->addSeparator();
            }
        }
        else if (!strncmp(def[i].c_str(), "submenu", 7)) {
            const char *submenu_name = def[i].c_str() + 8;
            std::string submenu(submenu_name);
            menuHash[menu]->addMenu(subMenuHash[QString::fromStdString(submenu)]);
        }
        else if (!strncmp(def[i].c_str(), "icon", 4)) {
            QString icon_fname(def[i].c_str() + 5);
            if (topLevel) {
                menuHash[menu]->setIcon(_mainWin->create_icon(icon_fname));
            }
            else {
                subMenuHash[menu]->setIcon(_mainWin->create_icon(icon_fname));
            }
        }
        else {
            int action = get_action_index(def[i]);
            if (topLevel) {
                menuHash[menu]->addAction(actionHash[action]);
            }
            else {
                subMenuHash[menu]->addAction(actionHash[action]);
            }
        }
    }
}

StringList menubar_order = {
    "file",
    "edit",
    "view",
    "settings",
    "window",
    "help"
};

/**
 * @brief MainWindow::createAllMenus
 */
void
MainWindow::createAllMenus()
{
    debug_message("MainWindow createAllMenus()");

    /* Populate menus. */
    create_menu("file", file_menu, true);
    create_menu("edit", edit_menu, true);
    create_menu("view", view_menu, true);
    create_menu("settings", settings_menu, true);
    create_menu("window", window_menu, true);
    create_menu("help", help_menu, true);

    /* Populate submenus. */
    create_menu("zoom", zoom_menu, false);
    create_menu("pan", pan_menu, false);

    /* Connect dynamic menus. */
    connect(subMenuHash["recent"], SIGNAL(aboutToShow()), this, SLOT(recentMenuAboutToShow()));
    connect(menuHash["window"], SIGNAL(aboutToShow()), this, SLOT(windowMenuAboutToShow()));

    /* Do not allow the Menus to be torn off. It's a pain in the ass to maintain. */
    foreach (QMenu *menu , subMenuHash) {
        menu->setTearOffEnabled(false);
    }
    foreach (QMenu *menu , menuHash) {
        menu->setTearOffEnabled(false);
    }
}
