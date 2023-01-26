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

#include "embroidermodder.h"

#if 0
StatusBar::StatusBar(MainWindow* mw, QWidget *parent) : QStatusBar(parent)
{
    this->setObjectName("StatusBar");

    statusBarSnapButton   = new StatusBarButton("SNAP", mw, this, this);
    statusBarGridButton   = new StatusBarButton("GRID", mw, this, this);
    statusBarRulerButton  = new StatusBarButton("RULER", mw, this, this);
    statusBarOrthoButton  = new StatusBarButton("ORTHO", mw, this, this);
    statusBarPolarButton  = new StatusBarButton("POLAR", mw, this, this);
    statusBarQSnapButton  = new StatusBarButton("QSNAP", mw, this, this);
    statusBarQTrackButton = new StatusBarButton("QTRACK", mw, this, this);
    statusBarLwtButton    = new StatusBarButton("LWT", mw, this, this);
    statusBarMouseCoord   = new QLabel(this);

    statusBarMouseCoord->setMinimumWidth(300); // Must fit this text always
    statusBarMouseCoord->setMaximumWidth(300); // "+1.2345E+99, +1.2345E+99, +1.2345E+99"

    this->addWidget(statusBarMouseCoord);
    this->addWidget(statusBarSnapButton);
    this->addWidget(statusBarGridButton);
    this->addWidget(statusBarRulerButton);
    this->addWidget(statusBarOrthoButton);
    this->addWidget(statusBarPolarButton);
    this->addWidget(statusBarQSnapButton);
    this->addWidget(statusBarQTrackButton);
    this->addWidget(statusBarLwtButton);
}

void StatusBar::setMouseCoord(double x, double y)
{
    //TODO: set format from settings (Architectural, Decimal, Engineering, Fractional, Scientific)

    //Decimal
    statusBarMouseCoord->setText(std::string().setNum(x, 'F', 4) + ", " + std::string().setNum(y, 'F', 4)); //TODO: use precision from unit settings

    //Scientific
    //statusBarMouseCoord->setText(std::string().setNum(x, 'E', 4) + ", " + std::string().setNum(y, 'E', 4)); //TODO: use precision from unit settings
}


StatusBarButton::StatusBarButton(std::string buttonText, MainWindow* mw, StatusBar* statbar, QWidget *parent) : QToolButton(parent)
{
    mainWin = mw;
    statusbar = statbar;

    this->setObjectName("StatusBarButton" + buttonText);

    this->setText(buttonText);
    this->setAutoRaise(true);
    this->setCheckable(true);

    if     (objectName() == "StatusBarButtonSNAP")   { connect(this, SIGNAL(toggled(bool)), this, SLOT(toggleSnap(bool))); }
    else if (objectName() == "StatusBarButtonGRID")   { connect(this, SIGNAL(toggled(bool)), this, SLOT(toggleGrid(bool))); }
    else if (objectName() == "StatusBarButtonRULER")  { connect(this, SIGNAL(toggled(bool)), this, SLOT(toggleRuler(bool))); }
    else if (objectName() == "StatusBarButtonORTHO")  { connect(this, SIGNAL(toggled(bool)), this, SLOT(toggleOrtho(bool))); }
    else if (objectName() == "StatusBarButtonPOLAR")  { connect(this, SIGNAL(toggled(bool)), this, SLOT(togglePolar(bool))); }
    else if (objectName() == "StatusBarButtonQSNAP")  { connect(this, SIGNAL(toggled(bool)), this, SLOT(toggleQSnap(bool))); }
    else if (objectName() == "StatusBarButtonQTRACK") { connect(this, SIGNAL(toggled(bool)), this, SLOT(toggleQTrack(bool))); }
    else if (objectName() == "StatusBarButtonLWT")    { connect(this, SIGNAL(toggled(bool)), this, SLOT(toggleLwt(bool))); }
}

void StatusBarButton::contextMenuEvent(QContextMenuEvent *event)
{
    QApplication::setOverrideCursor(Qt::ArrowCursor);
    QMenu menu(this);
    if (objectName() == "StatusBarButtonSNAP")
    {
        QAction* settingsSnapAction = new QAction(mainWin->load_icon("gridsnapsettings"), "&Settings...", &menu);
        connect(settingsSnapAction, SIGNAL(triggered()), this, SLOT(settingsSnap()));
        menu.addAction(settingsSnapAction);
    }
    else if (objectName() == "StatusBarButtonGRID")
    {
        QAction* settingsGridAction = new QAction(mainWin->load_icon("gridsettings"), "&Settings...", &menu);
        connect(settingsGridAction, SIGNAL(triggered()), this, SLOT(settingsGrid()));
        menu.addAction(settingsGridAction);
    }
    else if (objectName() == "StatusBarButtonRULER")
    {
        QAction* settingsRulerAction = new QAction(mainWin->load_icon("rulersettings"), "&Settings...", &menu);
        connect(settingsRulerAction, SIGNAL(triggered()), this, SLOT(settingsRuler()));
        menu.addAction(settingsRulerAction);
    }
    else if (objectName() == "StatusBarButtonORTHO")
    {
        QAction* settingsOrthoAction = new QAction(mainWin->load_icon("orthosettings"), "&Settings...", &menu);
        connect(settingsOrthoAction, SIGNAL(triggered()), this, SLOT(settingsOrtho()));
        menu.addAction(settingsOrthoAction);
    }
    else if (objectName() == "StatusBarButtonPOLAR")
    {
        QAction* settingsPolarAction = new QAction(mainWin->load_icon("polarsettings"), "&Settings...", &menu);
        connect(settingsPolarAction, SIGNAL(triggered()), this, SLOT(settingsPolar()));
        menu.addAction(settingsPolarAction);
    }
    else if (objectName() == "StatusBarButtonQSNAP")
    {
        QAction* settingsQSnapAction = new QAction(mainWin->load_icon("qsnapsettings"), "&Settings...", &menu);
        connect(settingsQSnapAction, SIGNAL(triggered()), this, SLOT(settingsQSnap()));
        menu.addAction(settingsQSnapAction);
    }
    else if (objectName() == "StatusBarButtonQTRACK")
    {
        QAction* settingsQTrackAction = new QAction(mainWin->load_icon("qtracksettings"), "&Settings...", &menu);
        connect(settingsQTrackAction, SIGNAL(triggered()), this, SLOT(settingsQTrack()));
        menu.addAction(settingsQTrackAction);
    }
    else if (objectName() == "StatusBarButtonLWT")
    {
        View* gview = mainWin->activeView();
        if (gview)
        {
            QAction* enableRealAction = new QAction(mainWin->load_icon("realrender"), "&RealRender On", &menu);
            enableRealAction->setEnabled(!gview->isRealEnabled());
            connect(enableRealAction, SIGNAL(triggered()), this, SLOT(enableReal()));
            menu.addAction(enableRealAction);

            QAction* disableRealAction = new QAction(mainWin->load_icon("realrender"), "&RealRender Off", &menu);
            disableRealAction->setEnabled(gview->isRealEnabled());
            connect(disableRealAction, SIGNAL(triggered()), this, SLOT(disableReal()));
            menu.addAction(disableRealAction);
        }

        QAction* settingsLwtAction = new QAction(mainWin->load_icon("lineweightsettings"), "&Settings...", &menu);
        connect(settingsLwtAction, SIGNAL(triggered()), this, SLOT(settingsLwt()));
        menu.addAction(settingsLwtAction);
    }
    menu.exec(event->globalPos());
    QApplication::restoreOverrideCursor();
    statusbar->clearMessage();
}

void StatusBarButton::settingsSnap()
{
    mainWin->settingsDialog("Snap");
}

void StatusBarButton::settingsGrid()
{
    mainWin->settingsDialog("Grid/Ruler");
}

void StatusBarButton::settingsRuler()
{
    mainWin->settingsDialog("Grid/Ruler");
}

void StatusBarButton::settingsOrtho()
{
    mainWin->settingsDialog("Ortho/Polar");
}

void StatusBarButton::settingsPolar()
{
    mainWin->settingsDialog("Ortho/Polar");
}

void StatusBarButton::settingsQSnap()
{
    mainWin->settingsDialog("QuickSnap");
}

void StatusBarButton::settingsQTrack()
{
    mainWin->settingsDialog("QuickTrack");
}

void StatusBarButton::settingsLwt()
{
    mainWin->settingsDialog("LineWeight");
}

void StatusBarButton::toggleSnap(bool on)
{
    debug_message("StatusBarButton toggleSnap()");
    View* gview = mainWin->activeView();
    if (gview) { gview->toggleSnap(on); }
}

void StatusBarButton::toggleGrid(bool on)
{
    debug_message("StatusBarButton toggleGrid()");
    View* gview = mainWin->activeView();
    if (gview) { gview->toggleGrid(on); }
}

void StatusBarButton::toggleRuler(bool on)
{
    debug_message("StatusBarButton toggleRuler()");
    View* gview = mainWin->activeView();
    if (gview) { gview->toggleRuler(on); }
}

void StatusBarButton::toggleOrtho(bool on)
{
    debug_message("StatusBarButton toggleOrtho()");
    View* gview = mainWin->activeView();
    if (gview) { gview->toggleOrtho(on); }
}

void StatusBarButton::togglePolar(bool on)
{
    debug_message("StatusBarButton togglePolar()");
    View* gview = mainWin->activeView();
    if (gview) { gview->togglePolar(on); }
}

void StatusBarButton::toggleQSnap(bool on)
{
    debug_message("StatusBarButton toggleQSnap()");
    View* gview = mainWin->activeView();
    if (gview) { gview->toggleQSnap(on); }
}

void StatusBarButton::toggleQTrack(bool on)
{
    debug_message("StatusBarButton toggleQTrack()");
    View* gview = mainWin->activeView();
    if (gview) { gview->toggleQTrack(on); }
}

void StatusBarButton::toggleLwt(bool on)
{
    debug_message("StatusBarButton toggleLwt()");
    View* gview = mainWin->activeView();
    if (gview) { gview->toggleLwt(on); }
}

void StatusBarButton::enableLwt()
{
    debug_message("StatusBarButton enableLwt()");
    View* gview = mainWin->activeView();
    if (gview)
    {
        if (!gview->isLwtEnabled())
            gview->toggleLwt(true);
    }
}

void StatusBarButton::disableLwt()
{
    debug_message("StatusBarButton disableLwt()");
    View* gview = mainWin->activeView();
    if (gview)
    {
        if (gview->isLwtEnabled())
            gview->toggleLwt(false);
    }
}

void StatusBarButton::enableReal()
{
    debug_message("StatusBarButton enableReal()");
    View* gview = mainWin->activeView();
    if (gview) {
        gview->toggleReal(true);
    }
}

void StatusBarButton::disableReal()
{
    debug_message("StatusBarButton disableReal()");
    View* gview = mainWin->activeView();
    if (gview) {
        gview->toggleReal(false);
    }
}

#endif
