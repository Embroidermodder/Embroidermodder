#include "mainwindow.h"
#include <QDebug>
#include <QMenu>
#include <QMenuBar>
#include <QAction>

void MainWindow::createFileMenu()
{
    qDebug("MainWindow createFileMenu()");
    menuBar()->addMenu(fileMenu);
    fileMenu->addAction(actionHash.value( "new"));
    fileMenu->addSeparator();
    fileMenu->addAction(actionHash.value( "open"));

    fileMenu->addMenu(recentMenu);
    connect(recentMenu, SIGNAL(aboutToShow()), this, SLOT(recentMenuAboutToShow()));
    //Do not allow the Recent Menu to be torn off. It's a pain in the ass to maintain.
    recentMenu->setTearOffEnabled(false);

    fileMenu->addSeparator();
    fileMenu->addAction(actionHash.value( "save"));
    fileMenu->addAction(actionHash.value( "saveas"));
    fileMenu->addSeparator();
    fileMenu->addAction(actionHash.value( "print"));
    fileMenu->addSeparator();
    fileMenu->addAction(actionHash.value( "windowclose"));
    fileMenu->addSeparator();
    fileMenu->addAction(actionHash.value( "designdetails"));
    fileMenu->addSeparator();

    fileMenu->addAction(actionHash.value( "exit"));
    fileMenu->setTearOffEnabled(false);
}

void MainWindow::createEditMenu()
{
    qDebug("MainWindow createEditMenu()");
    menuBar()->addMenu(editMenu);
    editMenu->addAction(actionHash.value( "undo"));
    editMenu->addAction(actionHash.value( "redo"));
    editMenu->addSeparator();
    editMenu->addAction(actionHash.value( "cut"));
    editMenu->addAction(actionHash.value( "copy"));
    editMenu->addAction(actionHash.value( "paste"));
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
    zoomMenu->addAction(actionHash.value( "zoomrealtime"));
    zoomMenu->addAction(actionHash.value( "zoomprevious"));
    zoomMenu->addSeparator();
    zoomMenu->addAction(actionHash.value( "zoomwindow"));
    zoomMenu->addAction(actionHash.value( "zoomdynamic"));
    zoomMenu->addAction(actionHash.value( "zoomscale"));
    zoomMenu->addAction(actionHash.value( "zoomcenter"));
    zoomMenu->addSeparator();
    zoomMenu->addAction(actionHash.value( "zoomin"));
    zoomMenu->addAction(actionHash.value( "zoomout"));
    zoomMenu->addSeparator();
    zoomMenu->addAction(actionHash.value( "zoomselected"));
    zoomMenu->addAction(actionHash.value( "zoomall"));
    zoomMenu->addAction(actionHash.value( "zoomextents"));
    viewMenu->addMenu(panMenu);
    panMenu->setIcon(QIcon(appDir + "/icons/" + icontheme + "/pan" + ".png"));
    panMenu->addAction(actionHash.value( "panrealtime"));
    panMenu->addAction(actionHash.value( "panpoint"));
    panMenu->addSeparator();
    panMenu->addAction(actionHash.value( "panleft"));
    panMenu->addAction(actionHash.value( "panright"));
    panMenu->addAction(actionHash.value( "panup"));
    panMenu->addAction(actionHash.value( "pandown"));
    viewMenu->addSeparator();
    viewMenu->addAction(actionHash.value( "day"));
    viewMenu->addAction(actionHash.value( "night"));
    viewMenu->addSeparator();

    viewMenu->setTearOffEnabled(true);
    zoomMenu->setTearOffEnabled(true);
    panMenu->setTearOffEnabled(true);
}

void MainWindow::createSettingsMenu()
{
    qDebug("MainWindow createSettingsMenu()");
    menuBar()->addMenu(settingsMenu);
    settingsMenu->addAction(actionHash.value("settingsdialog"));
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
    helpMenu->addAction(actionHash.value("help"));
    helpMenu->addSeparator();
    helpMenu->addAction(actionHash.value("changelog"));
    helpMenu->addSeparator();
    helpMenu->addAction(actionHash.value("tipoftheday"));
    helpMenu->addSeparator();
    helpMenu->addAction(actionHash.value("about"));
    helpMenu->addSeparator();
    helpMenu->addAction(actionHash.value("whatsthis"));
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

