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

/**
 * @brief create_menu
 * @param menu
 * @param def
 * @param topLevel
 */
void
create_menu(std::string menu, StringList def, bool topLevel)
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
            menuHash[menu]->addMenu(subMenuHash[submenu]);
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
            if (topLevel) {
                menuHash[menu]->addAction(actionHash[def[i]]);
            }
            else {
                subMenuHash[menu]->addAction(actionHash[def[i]]);
            }
        }
    }
}

/**
 * @brief MainWindow::createAllMenus
 */
void
MainWindow::createAllMenus()
{
    debug_message("MainWindow createAllMenus()");

    /* Populate menus. */
    create_menu("file", config["file_menu"].sl, true);
    create_menu("edit", config["edit_menu"].sl, true);
    create_menu("view", config["view_menu"].sl, true);
    create_menu("settings", config["settings_menu"].sl, true);
    create_menu("window", config["window_menu"].sl, true);
    create_menu("help", config["help_menu"].sl, true);
    create_menu("draw", config["draw_menu"].sl, true);

    /* Populate submenus. */
    create_menu("zoom", config["zoom_menu"].sl, false);
    create_menu("pan", config["pan_menu"].sl, false);

    /* Connect dynamic menus. */
    connect(subMenuHash["recent"], SIGNAL(aboutToShow()), this, SLOT(recentMenuAboutToShow()));
    connect(menuHash["window"], SIGNAL(aboutToShow()), this, SLOT(windowMenuAboutToShow()));

    /* Do not allow the Menus to be torn off. It's a pain in the ass to maintain. */
    for (auto iter=menuHash.begin(); iter!=menuHash.end(); iter++) {
        iter->second->setTearOffEnabled(false);
    }
    for (auto iter=subMenuHash.begin(); iter!=subMenuHash.end(); iter++) {
        iter->second->setTearOffEnabled(false);
    }
}
