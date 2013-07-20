#include "mainwindow.h"
#include <QDebug>
#include <QMenu>
#include <QMenuBar>
#include <QAction>

void MainWindow::createFileMenu()
{
    qDebug("MainWindow createFileMenu()");
    menuBar()->addMenu(fileMenu);
    fileMenu->addAction(actionDict.value(ACTION_new));
    fileMenu->addSeparator();
    fileMenu->addAction(actionDict.value(ACTION_open));

    fileMenu->addMenu(recentMenu);
    connect(recentMenu, SIGNAL(aboutToShow()), this, SLOT(recentMenuAboutToShow()));
    //Do not allow the Recent Menu to be torn off. It's a pain in the ass to maintain.
    recentMenu->setTearOffEnabled(false);

    fileMenu->addSeparator();
    fileMenu->addAction(actionDict.value(ACTION_save));
    fileMenu->addAction(actionDict.value(ACTION_saveas));
    fileMenu->addSeparator();
    fileMenu->addAction(actionDict.value(ACTION_print));
    fileMenu->addSeparator();
    fileMenu->addAction(actionDict.value(ACTION_windowclose));
    fileMenu->addSeparator();
    fileMenu->addAction(actionDict.value(ACTION_designdetails));
    fileMenu->addSeparator();

    fileMenu->addAction(actionDict.value(ACTION_exit));
    fileMenu->setTearOffEnabled(false);
}

void MainWindow::createEditMenu()
{
    qDebug("MainWindow createEditMenu()");
    menuBar()->addMenu(editMenu);
    editMenu->addAction(actionDict.value(ACTION_undo));
    editMenu->addAction(actionDict.value(ACTION_redo));
    editMenu->addSeparator();
    editMenu->addAction(actionDict.value(ACTION_cut));
    editMenu->addAction(actionDict.value(ACTION_copy));
    editMenu->addAction(actionDict.value(ACTION_paste));
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
    zoomMenu->addAction(actionDict.value(ACTION_zoomrealtime));
    zoomMenu->addAction(actionDict.value(ACTION_zoomprevious));
    zoomMenu->addSeparator();
    zoomMenu->addAction(actionDict.value(ACTION_zoomwindow));
    zoomMenu->addAction(actionDict.value(ACTION_zoomdynamic));
    zoomMenu->addAction(actionDict.value(ACTION_zoomscale));
    zoomMenu->addAction(actionDict.value(ACTION_zoomcenter));
    zoomMenu->addSeparator();
    zoomMenu->addAction(actionDict.value(ACTION_zoomin));
    zoomMenu->addAction(actionDict.value(ACTION_zoomout));
    zoomMenu->addSeparator();
    zoomMenu->addAction(actionDict.value(ACTION_zoomselected));
    zoomMenu->addAction(actionDict.value(ACTION_zoomall));
    zoomMenu->addAction(actionDict.value(ACTION_zoomextents));
    viewMenu->addMenu(panMenu);
    panMenu->setIcon(QIcon("icons/" + icontheme + "/pan" + ".png"));
    panMenu->addAction(actionDict.value(ACTION_panrealtime));
    panMenu->addAction(actionDict.value(ACTION_panpoint));
    panMenu->addSeparator();
    panMenu->addAction(actionDict.value(ACTION_panleft));
    panMenu->addAction(actionDict.value(ACTION_panright));
    panMenu->addAction(actionDict.value(ACTION_panup));
    panMenu->addAction(actionDict.value(ACTION_pandown));
    viewMenu->addSeparator();

    viewMenu->setTearOffEnabled(true);
    zoomMenu->setTearOffEnabled(true);
    panMenu->setTearOffEnabled(true);
}

void MainWindow::createSettingsMenu()
{
    qDebug("MainWindow createSettingsMenu()");
    menuBar()->addMenu(settingsMenu);
    settingsMenu->addAction(actionDict.value(ACTION_settingsdialog));
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
    helpMenu->addAction(actionDict.value(ACTION_help));
    helpMenu->addSeparator();
    helpMenu->addAction(actionDict.value(ACTION_changelog));
    helpMenu->addSeparator();
    helpMenu->addAction(actionDict.value(ACTION_tipoftheday));
    helpMenu->addSeparator();
    helpMenu->addAction(actionDict.value(ACTION_about));
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
