#include "mainwindow.h"
#include <QDebug>
#include <QMenu>
#include <QMenuBar>
#include <QAction>

void MainWindow::createFileMenu()
{
    qDebug("MainWindow createFileMenu()");
    menuBar()->addMenu(fileMenu);
    fileMenu->addAction(actionHash.value(ACTION_new));
    fileMenu->addSeparator();
    fileMenu->addAction(actionHash.value(ACTION_open));

    fileMenu->addMenu(recentMenu);
    connect(recentMenu, SIGNAL(aboutToShow()), this, SLOT(recentMenuAboutToShow()));
    //Do not allow the Recent Menu to be torn off. It's a pain in the ass to maintain.
    recentMenu->setTearOffEnabled(false);

    fileMenu->addSeparator();
    fileMenu->addAction(actionHash.value(ACTION_save));
    fileMenu->addAction(actionHash.value(ACTION_saveas));
    fileMenu->addSeparator();
    fileMenu->addAction(actionHash.value(ACTION_print));
    fileMenu->addSeparator();
    fileMenu->addAction(actionHash.value(ACTION_windowclose));
    fileMenu->addSeparator();
    fileMenu->addAction(actionHash.value(ACTION_designdetails));
    fileMenu->addSeparator();

    fileMenu->addAction(actionHash.value(ACTION_exit));
    fileMenu->setTearOffEnabled(false);
}

void MainWindow::createEditMenu()
{
    qDebug("MainWindow createEditMenu()");
    menuBar()->addMenu(editMenu);
    editMenu->addAction(actionHash.value(ACTION_undo));
    editMenu->addAction(actionHash.value(ACTION_redo));
    editMenu->addSeparator();
    editMenu->addAction(actionHash.value(ACTION_cut));
    editMenu->addAction(actionHash.value(ACTION_copy));
    editMenu->addAction(actionHash.value(ACTION_paste));
    editMenu->addSeparator();
    editMenu->setTearOffEnabled(true);
}

void MainWindow::createViewMenu()
{
    qDebug("MainWindow createViewMenu()");

    QString icontheme = getSettingsGeneralIconTheme();

    menuBar()->addMenu(viewMenu);
    viewMenu->addSeparator();
    viewMenu->addMenu(zoomMenu);
    zoomMenu->setIcon(QIcon("icons/" + icontheme + "/zoom" + ".png"));
    zoomMenu->addAction(actionHash.value(ACTION_zoomrealtime));
    zoomMenu->addAction(actionHash.value(ACTION_zoomprevious));
    zoomMenu->addSeparator();
    zoomMenu->addAction(actionHash.value(ACTION_zoomwindow));
    zoomMenu->addAction(actionHash.value(ACTION_zoomdynamic));
    zoomMenu->addAction(actionHash.value(ACTION_zoomscale));
    zoomMenu->addAction(actionHash.value(ACTION_zoomcenter));
    zoomMenu->addSeparator();
    zoomMenu->addAction(actionHash.value(ACTION_zoomin));
    zoomMenu->addAction(actionHash.value(ACTION_zoomout));
    zoomMenu->addSeparator();
    zoomMenu->addAction(actionHash.value(ACTION_zoomselected));
    zoomMenu->addAction(actionHash.value(ACTION_zoomall));
    zoomMenu->addAction(actionHash.value(ACTION_zoomextents));
    viewMenu->addMenu(panMenu);
    panMenu->setIcon(QIcon("icons/" + icontheme + "/pan" + ".png"));
    panMenu->addAction(actionHash.value(ACTION_panrealtime));
    panMenu->addAction(actionHash.value(ACTION_panpoint));
    panMenu->addSeparator();
    panMenu->addAction(actionHash.value(ACTION_panleft));
    panMenu->addAction(actionHash.value(ACTION_panright));
    panMenu->addAction(actionHash.value(ACTION_panup));
    panMenu->addAction(actionHash.value(ACTION_pandown));
    viewMenu->addSeparator();
    viewMenu->addAction(actionHash.value(ACTION_day));
    viewMenu->addAction(actionHash.value(ACTION_night));
    viewMenu->addSeparator();

    viewMenu->setTearOffEnabled(true);
    zoomMenu->setTearOffEnabled(true);
    panMenu->setTearOffEnabled(true);
}

void MainWindow::createSettingsMenu()
{
    qDebug("MainWindow createSettingsMenu()");
    menuBar()->addMenu(settingsMenu);
    settingsMenu->addAction(actionHash.value(ACTION_settingsdialog));
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
    helpMenu->addAction(actionHash.value(ACTION_help));
    helpMenu->addSeparator();
    helpMenu->addAction(actionHash.value(ACTION_changelog));
    helpMenu->addSeparator();
    helpMenu->addAction(actionHash.value(ACTION_tipoftheday));
    helpMenu->addSeparator();
    helpMenu->addAction(actionHash.value(ACTION_about));
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

/* kate: bom off; indent-mode cstyle; indent-width 4; replace-trailing-space-save on; */
