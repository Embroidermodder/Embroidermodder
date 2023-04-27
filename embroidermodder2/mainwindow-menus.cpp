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
    fileMenu->addAction(actionHash[ACTION_new]);
    fileMenu->addSeparator();
    fileMenu->addAction(actionHash[ACTION_open]);

    fileMenu->addMenu(recentMenu);
    connect(recentMenu, SIGNAL(aboutToShow()), this, SLOT(recentMenuAboutToShow()));
    //Do not allow the Recent Menu to be torn off. It's a pain in the ass to maintain.
    recentMenu->setTearOffEnabled(false);

    fileMenu->addSeparator();
    fileMenu->addAction(actionHash[ACTION_save]);
    fileMenu->addAction(actionHash[ACTION_saveas]);
    fileMenu->addSeparator();
    fileMenu->addAction(actionHash[ACTION_print]);
    fileMenu->addSeparator();
    fileMenu->addAction(actionHash[ACTION_windowclose]);
    fileMenu->addSeparator();
    fileMenu->addAction(actionHash[ACTION_designdetails]);
    fileMenu->addSeparator();

    fileMenu->addAction(actionHash[ACTION_exit]);
    fileMenu->setTearOffEnabled(false);
}

void MainWindow::createEditMenu()
{
    qDebug("MainWindow createEditMenu()");
    menuBar()->addMenu(editMenu);
    editMenu->addAction(actionHash[ACTION_undo]);
    editMenu->addAction(actionHash[ACTION_redo]);
    editMenu->addSeparator();
    editMenu->addAction(actionHash[ACTION_cut]);
    editMenu->addAction(actionHash[ACTION_copy]);
    editMenu->addAction(actionHash[ACTION_paste]);
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
    zoomMenu->addAction(actionHash[ACTION_zoomrealtime]);
    zoomMenu->addAction(actionHash[ACTION_zoomprevious]);
    zoomMenu->addSeparator();
    zoomMenu->addAction(actionHash[ACTION_zoomwindow]);
    zoomMenu->addAction(actionHash[ACTION_zoomdynamic]);
    zoomMenu->addAction(actionHash[ACTION_zoomscale]);
    zoomMenu->addAction(actionHash[ACTION_zoomcenter]);
    zoomMenu->addSeparator();
    zoomMenu->addAction(actionHash[ACTION_zoomin]);
    zoomMenu->addAction(actionHash[ACTION_zoomout]);
    zoomMenu->addSeparator();
    zoomMenu->addAction(actionHash[ACTION_zoomselected]);
    zoomMenu->addAction(actionHash[ACTION_zoomall]);
    zoomMenu->addAction(actionHash[ACTION_zoomextents]);
    viewMenu->addMenu(panMenu);
    panMenu->setIcon(QIcon(appDir + "/icons/" + icontheme + "/pan" + ".png"));
    panMenu->addAction(actionHash[ACTION_panrealtime]);
    panMenu->addAction(actionHash[ACTION_panpoint]);
    panMenu->addSeparator();
    panMenu->addAction(actionHash[ACTION_panleft]);
    panMenu->addAction(actionHash[ACTION_panright]);
    panMenu->addAction(actionHash[ACTION_panup]);
    panMenu->addAction(actionHash[ACTION_pandown]);
    viewMenu->addSeparator();
    viewMenu->addAction(actionHash[ACTION_day]);
    viewMenu->addAction(actionHash[ACTION_night]);
    viewMenu->addSeparator();

    viewMenu->setTearOffEnabled(true);
    zoomMenu->setTearOffEnabled(true);
    panMenu->setTearOffEnabled(true);
}

void MainWindow::createSettingsMenu()
{
    qDebug("MainWindow createSettingsMenu()");
    menuBar()->addMenu(settingsMenu);
    settingsMenu->addAction(actionHash[ACTION_settingsdialog]);
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
    helpMenu->addAction(actionHash[ACTION_help]);
    helpMenu->addSeparator();
    helpMenu->addAction(actionHash[ACTION_changelog]);
    helpMenu->addSeparator();
    helpMenu->addAction(actionHash[ACTION_tipoftheday]);
    helpMenu->addSeparator();
    helpMenu->addAction(actionHash[ACTION_about]);
    helpMenu->addSeparator();
    helpMenu->addAction(actionHash[ACTION_whatsthis]);
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
