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
    create_menu("file", string_lists["file_menu"], true);
    create_menu("edit", string_lists["edit_menu"], true);
    create_menu("view", string_lists["view_menu"], true);
    create_menu("settings", string_lists["settings_menu"], true);
    create_menu("window", string_lists["window_menu"], true);
    create_menu("help", string_lists["help_menu"], true);
    create_menu("draw", string_lists["draw_menu"], true);

    /* Populate submenus. */
    create_menu("zoom", string_lists["zoom_menu"], false);
    create_menu("pan", string_lists["pan_menu"], false);

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
