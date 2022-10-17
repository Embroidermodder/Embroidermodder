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
 *
 *  Status bar management.
 */

#include "em2.h"

void
status_bar_update(int status_bar)
{

}


/*
void status_bar_context_menu_event(
    WindowTab *status_bar, SDL_Event *event)
{
    setOverrideCursor(Qt-ArrowCursor)
    menu- = QMenu(this)
    switch (object-name) {
    case SNAP:
        settingsSnapAction = Action(load-icon(gridsnapsettings-xpm), "Settings...", menu-)
        connect(settingsSnapAction, SIGNAL(triggered()), this, SLOT(settingsSnap()))
        menu-.addAction(settingsSnapAction)
        break

    case GRID:
        settingsGridAction = Action(load-icon(gridsettings-xpm), "Settings...", menu-)
        connect(settingsGridAction, SIGNAL(triggered()), this, SLOT(settingsGrid()))
        menu-.addAction(settingsGridAction)
        break

    case RULER:
        settingsRulerAction = Action( Icon("icons/rulersettings.png"), "Settings...", menu-)
        connect(settingsRulerAction, SIGNAL(triggered()), this, SLOT(settingsRuler()))
        menu-.addAction(settingsRulerAction)
        break

    case ORTHO:
        settingsOrthoAction = Action( Icon("icons/orthosettings.png"), "Settings...", menu-)
        connect(settingsOrthoAction, SIGNAL(triggered()), this, SLOT(settingsOrtho()))
        menu-.addAction(settingsOrthoAction)
        break

    case POLAR:
        settingsPolarAction = Action( Icon("icons/polarsettings.png"), "Settings...", menu-)
        connect(settingsPolarAction, SIGNAL(triggered()), this, SLOT(settingsPolar()))
        menu-.addAction(settingsPolarAction)
        break

    case QSNAP:
        settingsQSnapAction = Action( Icon("icons/qsnapsettings.png"), "Settings...", menu-)
        connect(settingsQSnapAction, SIGNAL(triggered()), this, SLOT(settingsQSnap()))
        menu-.addAction(settingsQSnapAction)

    case QTRACK:
        settingsQTrackAction = Action( Icon("icons/qtracksettings.png"), "Settings...", menu-)
        connect(settingsQTrackAction, SIGNAL(triggered()), this, SLOT(settingsQTrack()))
        menu-.addAction(settingsQTrackAction)
        break

    case LWT:
        gview = main-win.active-view()
        if (gview) {
            enable-real-action = Action(Icon("icons/realrender.png"), "RealRender On", menu-)
            enable-real-action.setEnabled(!gview.isRealEnabled())
            connect(enable-real-action, SIGNAL(triggered()), this, SLOT(enableReal()))
            menu-.addAction(enable-real-action)

            disable-real-action = Action(Icon("icons/realrender.png"), "RealRender Off", menu-)
            disable-real-action.setEnabled(gview.isRealEnabled())
            connect(disable-real-action, SIGNAL(triggered()), this, SLOT(disableReal()))
            menu-.addAction(disable-real-action)
        }

        settingsLwtAction = Action(load-icon(lineweightsettings-xpm), "Settings...", menu-)
        connect(settingsLwtAction, SIGNAL(triggered()), this, SLOT(settingsLwt()))
        menu-.addAction(settingsLwtAction)
        break

    default:
        break
    }

    menu-.exec(event.globalPos());
    restore_override_cursor();
    statusbar.clearMessage();
}
*/

/* TODO: set format from settings.
 */
void
status_bar_mouse_coord(WindowTab *tab, int x, int y)
{
    printf("%d %d\n", x, y);
    /*
    if (tab->number-mode == ARCHITECTURAL) {
        return;
    }
    if (tab->number-mode == ENGINEERING) {
        return;
    }
    if (tab->number-mode == FRACTIONAL) {
        return;
    }
    if (tab->number-mode == SCIENTIFIC) { */
        /* status_bar-mouse-coord.setText("".setNum(x, 'E', 4)
            + ", " + "".setNum(y, 'E', 4))
        # TODO: use precision from unit settings */ /*
        return;
    } */
    /* Else decimal */
    /*
     * status_bar-mouse-coord.setText("".setNum(x, 'F', 4) + ", "
     * + "".setNum(y, 'F', 4))
     */ /* TODO: use precision from unit settings */
}

/* class StatusBar() */
/*
 * StatusBar( QWidget* parent = 0)
 * def setMouseCoord(x, y)
 */
void
status_bar_init(void)
{
    /*
    (self, buttonText, mw, statbar, parent);
    statusbar = statbar;

    this.set_object_name("StatusBarButton" + buttonText);

    this.setText(buttonText);
    this.setAutoRaise(1);
    this.setCheckable(1);

    if (object-name >= 0 && object-name < N-STATUS) {
        status[object-name] = !status[object-name];
    }
    */
    /*
    int i;
    def --init--(self, mw, parent)
    this.set_object_name("StatusBar");

    for (i=0; i<N_STATUS; i++) {
        status_bar[i] = StatusBarButton(status_bar-label[i], main-win, this, this);
    }

    status_bar-mouse-coord = tk.Label(this)

    status_bar-mouse-coord.set_minimum-width(300)
    # Must fit this text always
    status_bar-mouse-coord.set_maximum-width(300)
    # "+1.2345E+99, +1.2345E+99, +1.2345E+99"

    this, add_widget(status_bar-mouse-coord)
    for i in range(N-STATUS) {
        this, add_widget(status_bar[i])
    }
    */
}

