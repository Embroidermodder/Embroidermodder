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

StatusBar::StatusBar(QWidget *parent) : QStatusBar(parent)
{
    this->setObjectName("StatusBar");

    int n = string_array_length(button_list);
    for (int i=0; i<n; i++) {
        QString name = "StatusBarButton";
        QToolButton* button = new QToolButton(parent);
        button->setObjectName(name + button_list[i]);

        button->setText(QString::fromStdString(button_list[i]));
        button->setAutoRaise(true);
        button->setCheckable(true);

        connect(button, &QToolButton::toggled, this, [=](bool a) { toggle(button_list[i], a); } );
        buttons[button_list[i]] = button;
    }

    statusBarMouseCoord = new QLabel(this);

    statusBarMouseCoord->setMinimumWidth(300); // Must fit this text always
    statusBarMouseCoord->setMaximumWidth(300); // "+1.2345E+99, +1.2345E+99, +1.2345E+99"

    this->addWidget(statusBarMouseCoord);
    for (int i=0; i<n; i++) {
        this->addWidget(buttons[button_list[i]]);
    }
}

void StatusBar::setMouseCoord(EmbReal x, EmbReal y)
{
    //TODO: set format from settings (Architectural, Decimal, Engineering, Fractional, Scientific)

    //Decimal
    statusBarMouseCoord->setText(QString().setNum(x, 'F', 4) + ", " + QString().setNum(y, 'F', 4)); //TODO: use precision from unit settings

    //Scientific
    //statusBarMouseCoord->setText(QString().setNum(x, 'E', 4) + ", " + QString().setNum(y, 'E', 4)); //TODO: use precision from unit settings
}

/**
 * .
 */
void
StatusBar::context_menu_action(QToolButton *button, const char *icon, const char *label, QMenu *menu, String setting_page)
{
    QAction* action = new QAction(_mainWin->create_icon(icon), label, menu);
    connect(action, &QAction::triggered, button, [=](){ actuator("settingsdialog " + setting_page ); });
    menu->addAction(action);
}

/**
 * .
 */
void
StatusBar::context_menu_event(QContextMenuEvent *event, QToolButton *button)
{
    QApplication::setOverrideCursor(Qt::ArrowCursor);
    QMenu menu(button);
    if (button->objectName() == "StatusBarButtonSNAP") {
        context_menu_action(button, "gridsnapsettings", "&Settings...", &menu, "Snap");
    }
    else if (button->objectName() == "StatusBarButtonGRID") {
        context_menu_action(button, "gridsettings", "&Settings...", &menu, "Grid/Ruler");
    }
    else if (button->objectName() == "StatusBarButtonRULER") {
        context_menu_action(button, "rulersettings", "&Settings...", &menu, "Grid/Ruler");
    }
    else if (button->objectName() == "StatusBarButtonORTHO") {
        context_menu_action(button, "orthosettings", "&Settings...", &menu, "Ortho/Polar");
    }
    else if (button->objectName() == "StatusBarButtonPOLAR") {
        context_menu_action(button, "polarsettings", "&Settings...", &menu, "Ortho/Polar");
    }
    else if (button->objectName() == "StatusBarButtonQSNAP") {
        context_menu_action(button, "qsnapsettings", "&Settings...", &menu, "QuickSnap");
    }
    else if (button->objectName() == "StatusBarButtonQTRACK") {
        context_menu_action(button, "qtracksettings", "&Settings...", &menu, "QuickTrack");
    }
    else if (button->objectName() == "StatusBarButtonLWT") {
        View* gview = activeView();
        if (gview) {
            /*
            QAction* enableRealAction = new QAction(_mainWin->create_icon("realrender"), "&RealRender On", &menu);
            enableRealAction->setEnabled(!gview->isRealEnabled());
            QAction::connect(enableRealAction, &QObject::triggered, this, [=](){ toggleReal(true) });
            menu.addAction(enableRealAction);

            QAction* disableRealAction = new QAction(_mainWin->create_icon("realrender"), "&RealRender Off", &menu);
            disableRealAction->setEnabled(gview->isRealEnabled());
            QAction::connect(disableRealAction, &QObject::triggered, this, [=](){ toggleReal(false) });
            menu.addAction(disableRealAction);
            */
        }

        context_menu_action(button, "lineweightsettings", "&Settings...", &menu, "LineWeight");
    }
    menu.exec(event->globalPos());
    QApplication::restoreOverrideCursor();
    statusbar->clearMessage();
}

void
StatusBar::toggle(String key, bool on)
{
    debug_message("StatusBarButton toggleSnap()");
    View* gview = activeView();
    if (gview) {
        if (key == "SNAP") {
            gview->toggleSnap(on);
        }
        else if (key == "GRID") {
            gview->toggleGrid(on);
        }
        else if (key == "RULER") {
            gview->toggleRuler(on);
        }
        else if (key == "ORTHO") {
            gview->toggleOrtho(on);
        }
        else if (key == "POLAR") {
            gview->togglePolar(on);
        }
        else if (key == "QSNAP") {
            gview->toggleQSnap(on);
        }
        else if (key == "QTRACK") {
            gview->toggleQTrack(on);
        }
        else if (key == "LWT") {
            gview->toggleLwt(on);
        }
    }
}

