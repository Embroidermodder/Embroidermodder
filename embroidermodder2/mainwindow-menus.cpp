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

StringList file_menu = {

};

/**
 * @brief MainWindow::createAllMenus
 */
void
MainWindow::createAllMenus()
{
    debug_message("MainWindow createAllMenus()");

    debug_message("creating file menu");
    menuBar()->addMenu(menuHash["file"]);
    menuHash["file"]->addAction(actionHash[get_action_index("new")]);
    menuHash["file"]->addSeparator();
    menuHash["file"]->addAction(actionHash[get_action_index("open")]);

    menuHash["file"]->addMenu(subMenuHash["recent"]);
    connect(subMenuHash["recent"], SIGNAL(aboutToShow()), this, SLOT(recentMenuAboutToShow()));
    //Do not allow the Recent Menu to be torn off. It's a pain in the ass to maintain.
    subMenuHash["recent"]->setTearOffEnabled(false);

    menuHash["file"]->addSeparator();
    menuHash["file"]->addAction(actionHash[get_action_index("save")]);
    menuHash["file"]->addAction(actionHash[get_action_index("saveas")]);
    menuHash["file"]->addSeparator();
    menuHash["file"]->addAction(actionHash[get_action_index("print")]);
    menuHash["file"]->addSeparator();
    menuHash["file"]->addAction(actionHash[get_action_index("windowclose")]);
    menuHash["file"]->addSeparator();
    menuHash["file"]->addAction(actionHash[get_action_index("designdetails")]);
    menuHash["file"]->addSeparator();

    menuHash["file"]->addAction(actionHash[get_action_index("exit")]);
    menuHash["file"]->setTearOffEnabled(false);

    debug_message("MainWindow createEditMenu()");
    menuBar()->addMenu(menuHash["edit"]);
    menuHash["edit"]->addAction(actionHash[get_action_index("undo")]);
    menuHash["edit"]->addAction(actionHash[get_action_index("redo")]);
    menuHash["edit"]->addSeparator();
    menuHash["edit"]->addAction(actionHash[get_action_index("cut")]);
    menuHash["edit"]->addAction(actionHash[get_action_index("copy")]);
    menuHash["edit"]->addAction(actionHash[get_action_index("paste")]);
    menuHash["edit"]->addSeparator();
    menuHash["edit"]->setTearOffEnabled(true);

    debug_message("view menu");

    menuBar()->addMenu(menuHash["view"]);
    menuHash["view"]->addSeparator();
    menuHash["view"]->addMenu(subMenuHash["zoom"]);
    subMenuHash["zoom"]->setIcon(create_icon("zoom"));
    subMenuHash["zoom"]->addAction(actionHash[get_action_index("zoomrealtime")]);
    subMenuHash["zoom"]->addAction(actionHash[get_action_index("zoomprevious")]);
    subMenuHash["zoom"]->addSeparator();
    subMenuHash["zoom"]->addAction(actionHash[get_action_index("zoomwindow")]);
    subMenuHash["zoom"]->addAction(actionHash[get_action_index("zoomdynamic")]);
    subMenuHash["zoom"]->addAction(actionHash[get_action_index("zoomscale")]);
    subMenuHash["zoom"]->addAction(actionHash[get_action_index("zoomcenter")]);
    subMenuHash["zoom"]->addSeparator();
    subMenuHash["zoom"]->addAction(actionHash[get_action_index("zoomin")]);
    subMenuHash["zoom"]->addAction(actionHash[get_action_index("zoomout")]);
    subMenuHash["zoom"]->addSeparator();
    subMenuHash["zoom"]->addAction(actionHash[get_action_index("zoomselected")]);
    subMenuHash["zoom"]->addAction(actionHash[get_action_index("zoomall")]);
    subMenuHash["zoom"]->addAction(actionHash[get_action_index("zoomextents")]);
    menuHash["view"]->addMenu(subMenuHash["pan"]);
    subMenuHash["pan"]->setIcon(create_icon("pan"));
    subMenuHash["pan"]->addAction(actionHash[get_action_index("panrealtime")]);
    subMenuHash["pan"]->addAction(actionHash[get_action_index("panpoint")]);
    subMenuHash["pan"]->addSeparator();
    subMenuHash["pan"]->addAction(actionHash[get_action_index("panleft")]);
    subMenuHash["pan"]->addAction(actionHash[get_action_index("panright")]);
    subMenuHash["pan"]->addAction(actionHash[get_action_index("panup")]);
    subMenuHash["pan"]->addAction(actionHash[get_action_index("pandown")]);
    menuHash["view"]->addSeparator();
    menuHash["view"]->addAction(actionHash[get_action_index("day")]);
    menuHash["view"]->addAction(actionHash[get_action_index("night")]);
    menuHash["view"]->addSeparator();

    menuHash["view"]->setTearOffEnabled(true);
    subMenuHash["zoom"]->setTearOffEnabled(true);
    subMenuHash["pan"]->setTearOffEnabled(true);

    debug_message("creating settings menu");
    menuBar()->addMenu(menuHash["settings"]);
    menuHash["settings"]->addAction(actionHash[get_action_index("settingsdialog")]);
    menuHash["settings"]->addSeparator();
    menuHash["settings"]->setTearOffEnabled(true);

    debug_message("creating window menu");
    menuBar()->addMenu(menuHash["window"]);
    connect(menuHash["window"], SIGNAL(aboutToShow()), this, SLOT(windowMenuAboutToShow()));
    //Do not allow the Window Menu to be torn off. It's a pain in the ass to maintain.
    menuHash["window"]->setTearOffEnabled(false);

    debug_message("creating help menu");
    menuBar()->addMenu(menuHash["help"]);
    menuHash["help"]->addAction(actionHash[get_action_index("help")]);
    menuHash["help"]->addSeparator();
    menuHash["help"]->addAction(actionHash[get_action_index("changelog")]);
    menuHash["help"]->addSeparator();
    menuHash["help"]->addAction(actionHash[get_action_index("tipoftheday")]);
    menuHash["help"]->addSeparator();
    menuHash["help"]->addAction(actionHash[get_action_index("about")]);
    menuHash["help"]->addSeparator();
    menuHash["help"]->addAction(actionHash[get_action_index("whatsthis")]);
    menuHash["help"]->setTearOffEnabled(true);
}
