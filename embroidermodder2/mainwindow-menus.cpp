#include "mainwindow.h"
#include <QDebug>
#include <QMenu>
#include <QMenuBar>
#include <QAction>

void MainWindow::createFileMenu()
{
    qDebug("MainWindow createFileMenu()");
    menuBar()->addMenu(fileMenu);
    fileMenu->addAction(actionHash.value(ACTION_NEW));
    fileMenu->addSeparator();
    fileMenu->addAction(actionHash.value(ACTION_OPEN));

    fileMenu->addMenu(recentMenu);
    connect(recentMenu, SIGNAL(aboutToShow()), this, SLOT(recentMenuAboutToShow()));
    //Do not allow the Recent Menu to be torn off. It's a pain in the ass to maintain.
    recentMenu->setTearOffEnabled(false);

    fileMenu->addSeparator();
    fileMenu->addAction(actionHash.value(ACTION_SAVE));
    fileMenu->addAction(actionHash.value(ACTION_SAVE_AS));
    fileMenu->addSeparator();
    fileMenu->addAction(actionHash.value(ACTION_PRINT));
    fileMenu->addSeparator();
    fileMenu->addAction(actionHash.value(ACTION_WINDOW_CLOSE));
    fileMenu->addSeparator();
    fileMenu->addAction(actionHash.value(ACTION_DESIGN_DETAILS));
    fileMenu->addSeparator();

    fileMenu->addAction(actionHash.value(ACTION_EXIT));
    fileMenu->setTearOffEnabled(false);
}

void MainWindow::createEditMenu()
{
    qDebug("MainWindow createEditMenu()");
    menuBar()->addMenu(editMenu);
    editMenu->addAction(actionHash.value(ACTION_UNDO));
    editMenu->addAction(actionHash.value(ACTION_REDO));
    editMenu->addSeparator();
    editMenu->addAction(actionHash.value(ACTION_CUT));
    editMenu->addAction(actionHash.value(ACTION_COPY));
    editMenu->addAction(actionHash.value(ACTION_PASTE));
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
    zoomMenu->addAction(actionHash.value(ACTION_ZOOM_REAL_TIME));
    zoomMenu->addAction(actionHash.value(ACTION_ZOOM_PREVIOUS));
    zoomMenu->addSeparator();
    zoomMenu->addAction(actionHash.value(ACTION_ZOOM_WINDOW));
    zoomMenu->addAction(actionHash.value(ACTION_ZOOM_DYNAMIC));
    zoomMenu->addAction(actionHash.value(ACTION_ZOOM_SCALE));
    zoomMenu->addAction(actionHash.value(ACTION_ZOOM_CENTER));
    zoomMenu->addSeparator();
    zoomMenu->addAction(actionHash.value(ACTION_ZOOM_IN));
    zoomMenu->addAction(actionHash.value(ACTION_ZOOM_OUT));
    zoomMenu->addSeparator();
    zoomMenu->addAction(actionHash.value(ACTION_ZOOM_SELECTED));
    zoomMenu->addAction(actionHash.value(ACTION_ZOOM_ALL));
    zoomMenu->addAction(actionHash.value(ACTION_ZOOM_EXTENTS));
    viewMenu->addMenu(panMenu);
    panMenu->setIcon(QIcon(appDir + "/icons/" + icontheme + "/pan" + ".png"));
    panMenu->addAction(actionHash.value(ACTION_PAN_REAL_TIME));
    panMenu->addAction(actionHash.value(ACTION_PAN_POINT));
    panMenu->addSeparator();
    panMenu->addAction(actionHash.value(ACTION_PAN_LEFT));
    panMenu->addAction(actionHash.value(ACTION_PAN_RIGHT));
    panMenu->addAction(actionHash.value(ACTION_PAN_UP));
    panMenu->addAction(actionHash.value(ACTION_PAN_DOWN));
    viewMenu->addSeparator();
    viewMenu->addAction(actionHash.value(ACTION_DAY));
    viewMenu->addAction(actionHash.value(ACTION_NIGHT));
    viewMenu->addSeparator();

    viewMenu->setTearOffEnabled(true);
    zoomMenu->setTearOffEnabled(true);
    panMenu->setTearOffEnabled(true);
}

void MainWindow::createSettingsMenu()
{
    qDebug("MainWindow createSettingsMenu()");
    menuBar()->addMenu(settingsMenu);
    settingsMenu->addAction(actionHash.value(ACTION_SETTINGS_DIALOG));
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
    helpMenu->addAction(actionHash.value(ACTION_HELP));
    helpMenu->addSeparator();
    helpMenu->addAction(actionHash.value(ACTION_CHANGELOG));
    helpMenu->addSeparator();
    helpMenu->addAction(actionHash.value(ACTION_TIP_OF_THE_DAY));
    helpMenu->addSeparator();
    helpMenu->addAction(actionHash.value(ACTION_ABOUT));
    helpMenu->addSeparator();
    helpMenu->addAction(actionHash.value(ACTION_WHATS_THIS));
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
