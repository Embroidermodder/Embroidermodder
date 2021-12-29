#!/usr/bin/env python3

r"""
    Embroidermodder 2.

    -----

    Copyright 2013-2022 The Embroidermodder Team
    Embroidermodder 2 is Open Source Software.
    See LICENSE for licensing terms.

    -----

    Another attempt at a graphical user interface that runs on
    lots of machines without a complex build or fragile dependencies.

    This is a translation of some of the ideas we came up with for other
    attempts.
"""

class StatusBarButton():
    r"""
    StatusBarButton(QString buttonText, MainWindow* mw, StatusBar* statbar, QWidget *parent = 0)
    StatusBar*  statusbar
    def contextMenuEvent(QContextMenuEvent *event = 0)
    """
    def __init__(self, buttonText, mw, statbar, parent):
        statusbar = statbar

        this.setObjectName("StatusBarButton" + buttonText)

        this.setText(buttonText)
        this.setAutoRaise(1)
        this.setCheckable(1)

        if self.object_name == "StatusBarButtonSNAP":
            connect(this, SIGNAL(toggled(int)), this, SLOT(toggleSnap(int)))
        elif self.object_name == "StatusBarButtonGRID":
            connect(this, SIGNAL(toggled(int)), this, SLOT(toggleGrid(int)))
        elif self.object_name == "StatusBarButtonRULER":
            connect(this, SIGNAL(toggled(int)), this, SLOT(toggleRuler(int)))
        elif self.object_name == "StatusBarButtonORTHO":
            connect(this, SIGNAL(toggled(int)), this, SLOT(toggleOrtho(int)))
        elif self.object_name == "StatusBarButtonPOLAR":
            connect(this, SIGNAL(toggled(int)), this, SLOT(togglePolar(int)))
        elif self.object_name== "StatusBarButtonQSNAP":
            connect(this, SIGNAL(toggled(int)), this, SLOT(toggleQSnap(int)))
        elif self.object_name== "StatusBarButtonQTRACK":
            connect(this, SIGNAL(toggled(int)), this, SLOT(toggleQTrack(int)))
        elif self.object_name== "StatusBarButtonLWT":
            connect(this, SIGNAL(toggled(int)), this, SLOT(toggleLwt(int)))

    def contextMenuEvent(self, event):
        QApplication_setOverrideCursor(Qt_ArrowCursor)
        menu_ = QMenu(this)
        if self.object_name== "StatusBarButtonSNAP":
            settingsSnapAction = new QAction(loadIcon(gridsnapsettings_xpm), "&Settings...", &menu_)
            connect(settingsSnapAction, SIGNAL(triggered()), this, SLOT(settingsSnap()))
            menu_.addAction(settingsSnapAction)

        elif self.object_name== "StatusBarButtonGRID":
            settingsGridAction = new QAction(loadIcon(gridsettings_xpm), "&Settings...", &menu_)
            connect(settingsGridAction, SIGNAL(triggered()), this, SLOT(settingsGrid()))
            menu_.addAction(settingsGridAction)
        }
        elif (self.object_name== "StatusBarButtonRULER") {
            settingsRulerAction = new QAction(QIcon("icons/rulersettings.png"), "&Settings...", &menu_)
            connect(settingsRulerAction, SIGNAL(triggered()), this, SLOT(settingsRuler()))
            menu_.addAction(settingsRulerAction)
        }
        elif (self.object_name== "StatusBarButtonORTHO") {
            settingsOrthoAction = new QAction(QIcon("icons/orthosettings.png"), "&Settings...", &menu_)
            connect(settingsOrthoAction, SIGNAL(triggered()), this, SLOT(settingsOrtho()))
            menu_.addAction(settingsOrthoAction)
        }
        elif (self.object_name== "StatusBarButtonPOLAR") {
            settingsPolarAction = new QAction(QIcon("icons/polarsettings.png"), "&Settings...", &menu_)
            connect(settingsPolarAction, SIGNAL(triggered()), this, SLOT(settingsPolar()))
            menu_.addAction(settingsPolarAction)
        }
        elif(self.object_name== "StatusBarButtonQSNAP")
        {
            settingsQSnapAction = new QAction(QIcon("icons/qsnapsettings.png"), "&Settings...", &menu_)
            connect(settingsQSnapAction, SIGNAL(triggered()), this, SLOT(settingsQSnap()))
            menu_.addAction(settingsQSnapAction)

        elif self.object_name== "StatusBarButtonQTRACK":
            settingsQTrackAction = new QAction(QIcon("icons/qtracksettings.png"), "&Settings...", &menu_)
            connect(settingsQTrackAction, SIGNAL(triggered()), this, SLOT(settingsQTrack()))
            menu_.addAction(settingsQTrackAction)

        elif self.object_name== "StatusBarButtonLWT":
            gview = _mainWin.activeView()
            if gview:
                enableRealAction = new QAction(QIcon("icons/realrender.png"), "&RealRender On", &menu_)
                enableRealAction.setEnabled(!gview.isRealEnabled())
                connect(enableRealAction, SIGNAL(triggered()), this, SLOT(enableReal()))
                menu_.addAction(enableRealAction)

                disableRealAction = new QAction(QIcon("icons/realrender.png"), "&RealRender Off", &menu_)
                disableRealAction.setEnabled(gview.isRealEnabled())
                connect(disableRealAction, SIGNAL(triggered()), this, SLOT(disableReal()))
                menu_.addAction(disableRealAction)

            settingsLwtAction = new QAction(loadIcon(lineweightsettings_xpm), "&Settings...", &menu_)
            connect(settingsLwtAction, SIGNAL(triggered()), this, SLOT(settingsLwt()))
            menu_.addAction(settingsLwtAction)

        menu_.exec(event.globalPos())
        QApplication_restoreOverrideCursor()
        statusbar.clearMessage()

    def StatusBar(MainWindow* mw, QWidget *parent) : QStatusBar(parent):
        i
        this.setObjectName("StatusBar")

        for (i=0; i<N_STATUS; i++) {
            status_bar[i] = new StatusBarButton(status_bar_label[i], _mainWin, this, this)
        }
        statusBarMouseCoord = new QLabel(this)

        statusBarMouseCoord.setMinimumWidth(300); # Must fit this text always
        statusBarMouseCoord.setMaximumWidth(300); # "+1.2345E+99, +1.2345E+99, +1.2345E+99"

        this.addWidget(statusBarMouseCoord)
        for (i=0; i<N_STATUS; i++) {
            this.addWidget(status_bar[i])

    def setMouseCoord(x, y):
        # TODO: set format from settings (Architectural, Decimal, Engineering, Fractional, Scientific)

        # Decimal
        statusBarMouseCoord.setText("".setNum(x, 'F', 4) + ", "
            + "".setNum(y, 'F', 4));
            #TODO: use precision from unit settings

        # Scientific
        # statusBarMouseCoord.setText("".setNum(x, 'E', 4)
            + ", " + "".setNum(y, 'E', 4));
        # TODO: use precision from unit settings

class Status_Bar():
    """
    StatusBar(MainWindow* mw, QWidget* parent = 0)
    def setMouseCoord(x, y)
    """