#include "embroidermodder.h"

#include <QDebug>
#include <QMenu>
#include <QMenuBar>
#include <QAction>

void MainWindow::createFileMenu()
{
    qDebug("MainWindow createFileMenu()");
    menuBar()->addMenu(fileMenu);
    fileMenu->addAction(actionHash["new"]);
    fileMenu->addSeparator();
    fileMenu->addAction(actionHash["open"]);

    fileMenu->addMenu(recentMenu);
    connect(recentMenu, SIGNAL(aboutToShow()), this, SLOT(recentMenuAboutToShow()));
    //Do not allow the Recent Menu to be torn off. It's a pain in the ass to maintain.
    recentMenu->setTearOffEnabled(false);

    fileMenu->addSeparator();
    fileMenu->addAction(actionHash["save"]);
    fileMenu->addAction(actionHash["saveas"]);
    fileMenu->addSeparator();
    fileMenu->addAction(actionHash["print"]);
    fileMenu->addSeparator();
    fileMenu->addAction(actionHash["windowclose"]);
    fileMenu->addSeparator();
    fileMenu->addAction(actionHash["designdetails"]);
    fileMenu->addSeparator();

    fileMenu->addAction(actionHash["exit"]);
    fileMenu->setTearOffEnabled(false);
}

void MainWindow::createEditMenu()
{
    qDebug("MainWindow createEditMenu()");
    menuBar()->addMenu(editMenu);
    editMenu->addAction(actionHash["undo"]);
    editMenu->addAction(actionHash["redo"]);
    editMenu->addSeparator();
    editMenu->addAction(actionHash["cut"]);
    editMenu->addAction(actionHash["copy"]);
    editMenu->addAction(actionHash["paste"]);
    editMenu->addSeparator();
    editMenu->setTearOffEnabled(true);
}

void MainWindow::createViewMenu()
{
    qDebug("MainWindow createViewMenu()");

    QString appDir = qApp->applicationDirPath();
    QString icontheme = getSettingsGeneralIconTheme();

    menuBar()->addMenu(viewMenu);
    viewMenu->addSeparator();
    viewMenu->addMenu(zoomMenu);
    zoomMenu->setIcon(QIcon(appDir + "/icons/" + icontheme + "/zoom" + ".png"));
    zoomMenu->addAction(actionHash["zoomrealtime"]);
    zoomMenu->addAction(actionHash["zoomprevious"]);
    zoomMenu->addSeparator();
    zoomMenu->addAction(actionHash["zoomwindow"]);
    zoomMenu->addAction(actionHash["zoomdynamic"]);
    zoomMenu->addAction(actionHash["zoomscale"]);
    zoomMenu->addAction(actionHash["zoomcenter"]);
    zoomMenu->addSeparator();
    zoomMenu->addAction(actionHash["zoomin"]);
    zoomMenu->addAction(actionHash["zoomout"]);
    zoomMenu->addSeparator();
    zoomMenu->addAction(actionHash["zoomselected"]);
    zoomMenu->addAction(actionHash["zoomall"]);
    zoomMenu->addAction(actionHash["zoomextents"]);
    viewMenu->addMenu(panMenu);
    panMenu->setIcon(QIcon(appDir + "/icons/" + icontheme + "/pan" + ".png"));
    panMenu->addAction(actionHash["panrealtime"]);
    panMenu->addAction(actionHash["panpoint"]);
    panMenu->addSeparator();
    panMenu->addAction(actionHash["panleft"]);
    panMenu->addAction(actionHash["panright"]);
    panMenu->addAction(actionHash["panup"]);
    panMenu->addAction(actionHash["pandown"]);
    viewMenu->addSeparator();
    viewMenu->addAction(actionHash["day"]);
    viewMenu->addAction(actionHash["night"]);
    viewMenu->addSeparator();

    viewMenu->setTearOffEnabled(true);
    zoomMenu->setTearOffEnabled(true);
    panMenu->setTearOffEnabled(true);
}

void MainWindow::createSettingsMenu()
{
    qDebug("MainWindow createSettingsMenu()");
    menuBar()->addMenu(settingsMenu);
    settingsMenu->addAction(actionHash["settingsdialog"]);
    settingsMenu->addSeparator();
    settingsMenu->setTearOffEnabled(true);
}

void MainWindow::createWindowMenu()
{
    qDebug("MainWindow createWindowMenu()");
    menuBar()->addMenu(windowMenu);
    connect(windowMenu, SIGNAL(aboutToShow()), this, SLOT(windowMenuAboutToShow()));
    // Do not allow the Window Menu to be torn off. It's a pain in the ass to maintain.
    windowMenu->setTearOffEnabled(false);

}

void MainWindow::createHelpMenu()
{
    qDebug("MainWindow createHelpMenu()");
    menuBar()->addMenu(helpMenu);
    helpMenu->addAction(actionHash["help"]);
    helpMenu->addSeparator();
    helpMenu->addAction(actionHash["changelog"]);
    helpMenu->addSeparator();
    helpMenu->addAction(actionHash["tipoftheday"]);
    helpMenu->addSeparator();
    helpMenu->addAction(actionHash["about"]);
    helpMenu->addSeparator();
    helpMenu->addAction(actionHash["whatsthis"]);
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

