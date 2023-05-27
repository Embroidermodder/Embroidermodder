/**
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

#include "embroidermodder.h"

/**
 * .
 */
StatusBarButton::StatusBarButton(QString buttonText, StatusBar* statbar, QWidget *parent) : QToolButton(parent)
{
    statusbar = statbar;

    this->setObjectName("StatusBarButton" + buttonText);

    this->setText(buttonText);
    this->setAutoRaise(true);
    this->setCheckable(true);

    if (objectName() == "StatusBarButtonSNAP") {
        connect(this, SIGNAL(toggled(bool)), this, SLOT(toggleSnap(bool))); 
    }
    else if (objectName() == "StatusBarButtonGRID") {
        connect(this, SIGNAL(toggled(bool)), this, SLOT(toggleGrid(bool)));
    }
    else if (objectName() == "StatusBarButtonRULER") {
        connect(this, SIGNAL(toggled(bool)), this, SLOT(toggleRuler(bool)));
    }
    else if (objectName() == "StatusBarButtonORTHO") {
        connect(this, SIGNAL(toggled(bool)), this, SLOT(toggleOrtho(bool)));
    }
    else if (objectName() == "StatusBarButtonPOLAR") {
        connect(this, SIGNAL(toggled(bool)), this, SLOT(togglePolar(bool)));
    }
    else if (objectName() == "StatusBarButtonQSNAP") {
        connect(this, SIGNAL(toggled(bool)), this, SLOT(toggleQSnap(bool)));
    }
    else if (objectName() == "StatusBarButtonQTRACK") {
        connect(this, SIGNAL(toggled(bool)), this, SLOT(toggleQTrack(bool)));
    }
    else if (objectName() == "StatusBarButtonLWT") {
        connect(this, SIGNAL(toggled(bool)), this, SLOT(toggleLwt(bool)));
    }
}

/**
 * .
 */
void
StatusBarButton::contextMenuEvent(QContextMenuEvent *event)
{
    QApplication::setOverrideCursor(Qt::ArrowCursor);
    QMenu menu(this);
    if (objectName() == "StatusBarButtonSNAP") {
        QAction* settingsSnapAction = new QAction(_mainWin->create_icon("gridsnapsettings"), "&Settings...", &menu);
        connect(settingsSnapAction, SIGNAL(triggered()), this, SLOT(settingsSnap()));
        menu.addAction(settingsSnapAction);
    }
    else if (objectName() == "StatusBarButtonGRID") {
        QAction* settingsGridAction = new QAction(_mainWin->create_icon("gridsettings"), "&Settings...", &menu);
        connect(settingsGridAction, SIGNAL(triggered()), this, SLOT(settingsGrid()));
        menu.addAction(settingsGridAction);
    }
    else if (objectName() == "StatusBarButtonRULER") {
        QAction* settingsRulerAction = new QAction(_mainWin->create_icon("rulersettings"), "&Settings...", &menu);
        connect(settingsRulerAction, SIGNAL(triggered()), this, SLOT(settingsRuler()));
        menu.addAction(settingsRulerAction);
    }
    else if (objectName() == "StatusBarButtonORTHO") {
        QAction* settingsOrthoAction = new QAction(_mainWin->create_icon("orthosettings"), "&Settings...", &menu);
        connect(settingsOrthoAction, SIGNAL(triggered()), this, SLOT(settingsOrtho()));
        menu.addAction(settingsOrthoAction);
    }
    else if (objectName() == "StatusBarButtonPOLAR") {
        QAction* settingsPolarAction = new QAction(_mainWin->create_icon("polarsettings"), "&Settings...", &menu);
        connect(settingsPolarAction, SIGNAL(triggered()), this, SLOT(settingsPolar()));
        menu.addAction(settingsPolarAction);
    }
    else if (objectName() == "StatusBarButtonQSNAP") {
        QAction* settingsQSnapAction = new QAction(_mainWin->create_icon("qsnapsettings"), "&Settings...", &menu);
        connect(settingsQSnapAction, SIGNAL(triggered()), this, SLOT(settingsQSnap()));
        menu.addAction(settingsQSnapAction);
    }
    else if (objectName() == "StatusBarButtonQTRACK") {
        QAction* settingsQTrackAction = new QAction(_mainWin->create_icon("qtracksettings"), "&Settings...", &menu);
        connect(settingsQTrackAction, SIGNAL(triggered()), this, SLOT(settingsQTrack()));
        menu.addAction(settingsQTrackAction);
    }
    else if (objectName() == "StatusBarButtonLWT") {
        View* gview = activeView();
        if (gview) {
            QAction* enableRealAction = new QAction(_mainWin->create_icon("realrender"), "&RealRender On", &menu);
            enableRealAction->setEnabled(!gview->isRealEnabled());
            connect(enableRealAction, SIGNAL(triggered()), this, SLOT(enableReal()));
            menu.addAction(enableRealAction);

            QAction* disableRealAction = new QAction(_mainWin->create_icon("realrender"), "&RealRender Off", &menu);
            disableRealAction->setEnabled(gview->isRealEnabled());
            connect(disableRealAction, SIGNAL(triggered()), this, SLOT(disableReal()));
            menu.addAction(disableRealAction);
        }

        QAction* settingsLwtAction = new QAction(_mainWin->create_icon("lineweightsettings"), "&Settings...", &menu);
        connect(settingsLwtAction, SIGNAL(triggered()), this, SLOT(settingsLwt()));
        menu.addAction(settingsLwtAction);
    }
    menu.exec(event->globalPos());
    QApplication::restoreOverrideCursor();
    statusbar->clearMessage();
}

void StatusBarButton::settingsSnap()
{
    _mainWin->settingsDialog("Snap");
}

void StatusBarButton::settingsGrid()
{
    _mainWin->settingsDialog("Grid/Ruler");
}

void StatusBarButton::settingsRuler()
{
    _mainWin->settingsDialog("Grid/Ruler");
}

void StatusBarButton::settingsOrtho()
{
    _mainWin->settingsDialog("Ortho/Polar");
}

void StatusBarButton::settingsPolar()
{
    _mainWin->settingsDialog("Ortho/Polar");
}

void StatusBarButton::settingsQSnap()
{
    _mainWin->settingsDialog("QuickSnap");
}

void StatusBarButton::settingsQTrack()
{
    _mainWin->settingsDialog("QuickTrack");
}

void StatusBarButton::settingsLwt()
{
    _mainWin->settingsDialog("LineWeight");
}

void StatusBarButton::toggleSnap(bool on)
{
    debug_message("StatusBarButton toggleSnap()");
    View* gview = activeView();
    if (gview) {
        gview->toggleSnap(on);
    }
}

void StatusBarButton::toggleGrid(bool on)
{
    debug_message("StatusBarButton toggleGrid()");
    View* gview = activeView();
    if (gview) {
        gview->toggleGrid(on);
    }
}

void StatusBarButton::toggleRuler(bool on)
{
    debug_message("StatusBarButton toggleRuler()");
    View* gview = activeView();
    if (gview) {
        gview->toggleRuler(on);
    }
}

void StatusBarButton::toggleOrtho(bool on)
{
    debug_message("StatusBarButton toggleOrtho()");
    View* gview = activeView();
    if (gview) {
        gview->toggleOrtho(on);
    }
}

void StatusBarButton::togglePolar(bool on)
{
    debug_message("StatusBarButton togglePolar()");
    View* gview = activeView();
    if (gview) {
        gview->togglePolar(on);
    }
}

void StatusBarButton::toggleQSnap(bool on)
{
    debug_message("StatusBarButton toggleQSnap()");
    View* gview = activeView();
    if (gview) {
        gview->toggleQSnap(on);
    }
}

void StatusBarButton::toggleQTrack(bool on)
{
    debug_message("StatusBarButton toggleQTrack()");
    View* gview = activeView();
    if (gview) {
        gview->toggleQTrack(on);
    }
}

void StatusBarButton::toggleLwt(bool on)
{
    debug_message("StatusBarButton toggleLwt()");
    View* gview = activeView();
    if (gview) {
        gview->toggleLwt(on);
    }
}

void StatusBarButton::enableLwt()
{
    debug_message("StatusBarButton enableLwt()");
    View* gview = activeView();
    if (gview) {
        gview->toggleLwt(true);
    }
}

void StatusBarButton::disableLwt()
{
    debug_message("StatusBarButton disableLwt()");
    View* gview = activeView();
    if (gview) {
        gview->toggleLwt(false);
    }
}

void StatusBarButton::enableReal()
{
    debug_message("StatusBarButton enableReal()");
    View* gview = activeView();
    if (gview) {
        gview->toggleReal(true);
    }
}

void StatusBarButton::disableReal()
{
    debug_message("StatusBarButton disableReal()");
    View* gview = activeView();
    if (gview) {
        gview->toggleReal(false);
    }
}
