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
 * @brief MainWindow::createFileMenu
 */
void
MainWindow::createFileMenu()
{
    qDebug("MainWindow createFileMenu()");
    menuBar()->addMenu(fileMenu);
    fileMenu->addAction(actionHash[get_action_index("new")]);
    fileMenu->addSeparator();
    fileMenu->addAction(actionHash[get_action_index("open")]);

    fileMenu->addMenu(recentMenu);
    connect(recentMenu, SIGNAL(aboutToShow()), this, SLOT(recentMenuAboutToShow()));
    //Do not allow the Recent Menu to be torn off. It's a pain in the ass to maintain.
    recentMenu->setTearOffEnabled(false);

    fileMenu->addSeparator();
    fileMenu->addAction(actionHash[get_action_index("save")]);
    fileMenu->addAction(actionHash[get_action_index("saveas")]);
    fileMenu->addSeparator();
    fileMenu->addAction(actionHash[get_action_index("print")]);
    fileMenu->addSeparator();
    fileMenu->addAction(actionHash[get_action_index("windowclose")]);
    fileMenu->addSeparator();
    fileMenu->addAction(actionHash[get_action_index("designdetails")]);
    fileMenu->addSeparator();

    fileMenu->addAction(actionHash[get_action_index("exit")]);
    fileMenu->setTearOffEnabled(false);
}

void MainWindow::createEditMenu()
{
    qDebug("MainWindow createEditMenu()");
    menuBar()->addMenu(editMenu);
    editMenu->addAction(actionHash[get_action_index("undo")]);
    editMenu->addAction(actionHash[get_action_index("redo")]);
    editMenu->addSeparator();
    editMenu->addAction(actionHash[get_action_index("cut")]);
    editMenu->addAction(actionHash[get_action_index("copy")]);
    editMenu->addAction(actionHash[get_action_index("paste")]);
    editMenu->addSeparator();
    editMenu->setTearOffEnabled(true);
}

void MainWindow::createViewMenu()
{
    qDebug("MainWindow createViewMenu()");

    QString appDir = qApp->applicationDirPath();
    QString icontheme = settings_general_icon_theme;

    menuBar()->addMenu(viewMenu);
    viewMenu->addSeparator();
    viewMenu->addMenu(zoomMenu);
    zoomMenu->setIcon(QIcon(appDir + "/icons/" + icontheme + "/zoom" + ".png"));
    zoomMenu->addAction(actionHash[get_action_index("zoomrealtime")]);
    zoomMenu->addAction(actionHash[get_action_index("zoomprevious")]);
    zoomMenu->addSeparator();
    zoomMenu->addAction(actionHash[get_action_index("zoomwindow")]);
    zoomMenu->addAction(actionHash[get_action_index("zoomdynamic")]);
    zoomMenu->addAction(actionHash[get_action_index("zoomscale")]);
    zoomMenu->addAction(actionHash[get_action_index("zoomcenter")]);
    zoomMenu->addSeparator();
    zoomMenu->addAction(actionHash[get_action_index("zoomin")]);
    zoomMenu->addAction(actionHash[get_action_index("zoomout")]);
    zoomMenu->addSeparator();
    zoomMenu->addAction(actionHash[get_action_index("zoomselected")]);
    zoomMenu->addAction(actionHash[get_action_index("zoomall")]);
    zoomMenu->addAction(actionHash[get_action_index("zoomextents")]);
    viewMenu->addMenu(panMenu);
    panMenu->setIcon(QIcon(appDir + "/icons/" + icontheme + "/pan" + ".png"));
    panMenu->addAction(actionHash[get_action_index("panrealtime")]);
    panMenu->addAction(actionHash[get_action_index("panpoint")]);
    panMenu->addSeparator();
    panMenu->addAction(actionHash[get_action_index("panleft")]);
    panMenu->addAction(actionHash[get_action_index("panright")]);
    panMenu->addAction(actionHash[get_action_index("panup")]);
    panMenu->addAction(actionHash[get_action_index("pandown")]);
    viewMenu->addSeparator();
    viewMenu->addAction(actionHash[get_action_index("day")]);
    viewMenu->addAction(actionHash[get_action_index("night")]);
    viewMenu->addSeparator();

    viewMenu->setTearOffEnabled(true);
    zoomMenu->setTearOffEnabled(true);
    panMenu->setTearOffEnabled(true);
}

void MainWindow::createSettingsMenu()
{
    qDebug("MainWindow createSettingsMenu()");
    menuBar()->addMenu(settingsMenu);
    settingsMenu->addAction(actionHash[get_action_index("settingsdialog")]);
    settingsMenu->addSeparator();
    settingsMenu->setTearOffEnabled(true);
}

void MainWindow::createWindowMenu()
{
    qDebug("MainWindow createWindowMenu()");
    menuBar()->addMenu(windowMenu);
    connect(windowMenu, SIGNAL(aboutToShow()), this, SLOT(windowMenuAboutToShow()));
    //Do not allow the Window Menu to be torn off. It's a pain in the ass to maintain.
    windowMenu->setTearOffEnabled(false);

}

void MainWindow::createHelpMenu()
{
    qDebug("MainWindow createHelpMenu()");
    menuBar()->addMenu(helpMenu);
    helpMenu->addAction(actionHash[get_action_index("help")]);
    helpMenu->addSeparator();
    helpMenu->addAction(actionHash[get_action_index("changelog")]);
    helpMenu->addSeparator();
    helpMenu->addAction(actionHash[get_action_index("tipoftheday")]);
    helpMenu->addSeparator();
    helpMenu->addAction(actionHash[get_action_index("about")]);
    helpMenu->addSeparator();
    helpMenu->addAction(actionHash[get_action_index("whatsthis")]);
    helpMenu->setTearOffEnabled(true);
}

void MainWindow::createAllMenus()
{
    qDebug("MainWindow createAllMenus()");
    createFileMenu();
    createEditMenu();
    createViewMenu();
    createSettingsMenu();
    createWindowMenu();
    createHelpMenu();

}
