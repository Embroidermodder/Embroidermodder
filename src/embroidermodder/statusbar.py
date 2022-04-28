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

import tkinter as tk


class StatusBarButton():
    r"""
    StatusBarButton(buttonText, mw, statbar, parent = 0)
    StatusBar*  statusbar
    def contextMenuEvent(QContextMenuEvent *event = 0)
    """
    def __init__(self, buttonText, mw, statbar, parent):
        statusbar = statbar

        this.set_object_name("StatusBarButton" + buttonText)

        this.setText(buttonText)
        this.setAutoRaise(1)
        this.setCheckable(1)

        if self.object_name == "SNAP":
            connect(this, SIGNAL(toggled(int)), this, SLOT(toggleSnap(int)))
        elif self.object_name == "GRID":
            connect(this, SIGNAL(toggled(int)), this, SLOT(toggleGrid(int)))
        elif self.object_name == "RULER":
            connect(this, SIGNAL(toggled(int)), this, SLOT(toggleRuler(int)))
        elif self.object_name == "ORTHO":
            connect(this, SIGNAL(toggled(int)), this, SLOT(toggleOrtho(int)))
        elif self.object_name == "POLAR":
            connect(this, SIGNAL(toggled(int)), this, SLOT(togglePolar(int)))
        elif self.object_name== "QSNAP":
            connect(this, SIGNAL(toggled(int)), this, SLOT(toggleQSnap(int)))
        elif self.object_name== "QTRACK":
            connect(this, SIGNAL(toggled(int)), this, SLOT(toggleQTrack(int)))
        elif self.object_name== "LWT":
            connect(this, SIGNAL(toggled(int)), this, SLOT(toggleLwt(int)))

    def contextMenuEvent(self, event):
        QApplication_setOverrideCursor(Qt_ArrowCursor)
        menu_ = QMenu(this)
        if self.object_name== "SNAP":
            settingsSnapAction = Action(loadIcon(gridsnapsettings_xpm), "Settings...", menu_)
            connect(settingsSnapAction, SIGNAL(triggered()), this, SLOT(settingsSnap()))
            menu_.addAction(settingsSnapAction)

        elif self.object_name== "GRID":
            settingsGridAction = Action(loadIcon(gridsettings_xpm), "Settings...", menu_)
            connect(settingsGridAction, SIGNAL(triggered()), this, SLOT(settingsGrid()))
            menu_.addAction(settingsGridAction)

        elif self.object_name== "RULER":
            settingsRulerAction = Action( Icon("icons/rulersettings.png"), "Settings...", menu_)
            connect(settingsRulerAction, SIGNAL(triggered()), this, SLOT(settingsRuler()))
            menu_.addAction(settingsRulerAction)

        elif self.object_name== "ORTHO":
            settingsOrthoAction = Action( Icon("icons/orthosettings.png"), "Settings...", menu_)
            connect(settingsOrthoAction, SIGNAL(triggered()), this, SLOT(settingsOrtho()))
            menu_.addAction(settingsOrthoAction)

        elif self.object_name== "POLAR":
            settingsPolarAction = Action( Icon("icons/polarsettings.png"), "Settings...", menu_)
            connect(settingsPolarAction, SIGNAL(triggered()), this, SLOT(settingsPolar()))
            menu_.addAction(settingsPolarAction)

        elif self.object_name== "QSNAP":
            settingsQSnapAction = Action( Icon("icons/qsnapsettings.png"), "Settings...", menu_)
            connect(settingsQSnapAction, SIGNAL(triggered()), this, SLOT(settingsQSnap()))
            menu_.addAction(settingsQSnapAction)

        elif self.object_name== "QTRACK":
            settingsQTrackAction = Action( Icon("icons/qtracksettings.png"), "Settings...", menu_)
            connect(settingsQTrackAction, SIGNAL(triggered()), this, SLOT(settingsQTrack()))
            menu_.addAction(settingsQTrackAction)

        elif self.object_name== "LWT":
            gview = main_win.active_view()
            if gview:
                enable_real_action = Action(Icon("icons/realrender.png"), "RealRender On", menu_)
                enable_real_action.setEnabled(!gview.isRealEnabled())
                connect(enable_real_action, SIGNAL(triggered()), this, SLOT(enableReal()))
                menu_.addAction(enable_real_action)

                disable_real_action = Action(Icon("icons/realrender.png"), "RealRender Off", menu_)
                disable_real_action.setEnabled(gview.isRealEnabled())
                connect(disable_real_action, SIGNAL(triggered()), this, SLOT(disableReal()))
                menu_.addAction(disable_real_action)

            settingsLwtAction = Action(loadIcon(lineweightsettings_xpm), "Settings...", menu_)
            connect(settingsLwtAction, SIGNAL(triggered()), this, SLOT(settingsLwt()))
            menu_.addAction(settingsLwtAction)

        menu_.exec(event.globalPos())
        restoreOverrideCursor()
        statusbar.clearMessage()

class StatusBar():
    """
    StatusBar(MainWindow* mw, QWidget* parent = 0)
    def setMouseCoord(x, y)
    """
    def __init__(self, mw, parent):
        " . "
        this.set_object_name("StatusBar")

        for i in range(N_STATUS):
            status_bar[i] = StatusBarButton(status_bar_label[i], main_win, this, this)

        status_bar_mouse_coord = tk.Label(this)

        status_bar_mouse_coord.set_minimum_width(300)
        # Must fit this text always
        status_bar_mouse_coord.set_maximum_width(300)
        # "+1.2345E+99, +1.2345E+99, +1.2345E+99"

        this.add_widget(status_bar_mouse_coord)
        for i in range(N_STATUS):
            this.add_widget(status_bar[i])

    def set_mouse_coord(x, y):
        " . "
        # TODO: set format from settings (Architectural, Decimal, Engineering, Fractional, Scientific)

        # Decimal
        status_bar_mouse_coord.setText("".setNum(x, 'F', 4) + ", "
            + "".setNum(y, 'F', 4));
            #TODO: use precision from unit settings

        # Scientific
        # status_bar_mouse_coord.setText("".setNum(x, 'E', 4)
            + ", " + "".setNum(y, 'E', 4));
        # TODO: use precision from unit settings
