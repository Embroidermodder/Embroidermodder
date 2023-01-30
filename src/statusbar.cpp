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

#include "imgui.h"

void status_bar(void)
{
    ImGuiIO &io = ImGui::GetIO();
    ImGui::SetNextWindowSize(ImVec2(io.DisplaySize.x, 50));
    ImGui::SetNextWindowPos(ImVec2(io.DisplaySize.x, io.DisplaySize.y - 50));
    ImGui::BeginChild("Statusbar");
    if (ImGui::Button("SNAP")) {
        views[pattern_index].snap_mode = !views[pattern_index].snap_mode;
        /* change button depressed state */
    }
    ImGui::SameLine();
    if (ImGui::Button("GRID")) {
        views[pattern_index].grid_mode = !views[pattern_index].grid_mode;
    }
    ImGui::SameLine();
    if (ImGui::Button("RULER")) {
        views[pattern_index].ruler_mode = !views[pattern_index].ruler_mode;
    }
    ImGui::SameLine();
    if (ImGui::Button("ORTHO")) {
        views[pattern_index].ortho_mode = !views[pattern_index].ortho_mode;
    }
    ImGui::SameLine();
    if (ImGui::Button("POLAR")) {
        views[pattern_index].polar_mode = !views[pattern_index].polar_mode;
    }
    ImGui::SameLine();
    if (ImGui::Button("QSNAP")) {
        views[pattern_index].qsnap_mode = !views[pattern_index].qsnap_mode;
    }
    ImGui::SameLine();
    if (ImGui::Button("QTRACK")) {
        views[pattern_index].qtrack_mode = !views[pattern_index].qtrack_mode;
    }
    ImGui::SameLine();
    if (ImGui::Button("LWT")) {
        views[pattern_index].lwt_mode = !views[pattern_index].lwt_mode;
    }
    ImGui::SameLine();
    ImGui::Text("TODO: Mouse co-ordinates here.");

    // statusBarMouseCoord->setMinimumWidth(300); // Must fit this text always
    // statusBarMouseCoord->setMaximumWidth(300); // "+1.2345E+99, +1.2345E+99, +1.2345E+99"
    ImGui::EndChild();
}


void setMouseCoord(double x, double y)
{
    //TODO: set format from settings (Architectural, Decimal, Engineering, Fractional, Scientific)

    //Decimal
    //statusBarMouseCoord->setText(std::string().setNum(x, 'F', 4) + ", " + std::string().setNum(y, 'F', 4)); //TODO: use precision from unit settings

    //Scientific
    //statusBarMouseCoord->setText(std::string().setNum(x, 'E', 4) + ", " + std::string().setNum(y, 'E', 4)); //TODO: use precision from unit settings
}

#if 0
void status_bar_context_menu(void)
{
    QMenu menu(this);
    if (objectName() == "StatusBarButtonSNAP") {
        QAction* settingsSnapAction = new QAction(mainWin->load_icon("gridsnapsettings"), "&Settings...", &menu);
        connect(settingsSnapAction, SIGNAL(triggered()), this, SLOT(settingsSnap()));
        menu.addAction(settingsSnapAction);
    }
    else if (objectName() == "StatusBarButtonGRID") {
        QAction* settingsGridAction = new QAction(mainWin->load_icon("gridsettings"), "&Settings...", &menu);
        menu.addAction(settingsGridAction);
    }
    else if (objectName() == "StatusBarButtonRULER") {
        QAction* settingsRulerAction = new QAction(mainWin->load_icon("rulersettings"), "&Settings...", &menu);
        menu.addAction(settingsRulerAction);
    }
    else if (objectName() == "StatusBarButtonORTHO") {
        QAction* settingsOrthoAction = new QAction(mainWin->load_icon("orthosettings"), "&Settings...", &menu);
        menu.addAction(settingsOrthoAction);
    }
    else if (objectName() == "StatusBarButtonPOLAR") {
        QAction* settingsPolarAction = new QAction(mainWin->load_icon("polarsettings"), "&Settings...", &menu);
        connect(settingsPolarAction, SIGNAL(triggered()), this, SLOT(settingsPolar()));
        menu.addAction(settingsPolarAction);
    }
    else if (objectName() == "StatusBarButtonQSNAP") {
        QAction* settingsQSnapAction = new QAction(mainWin->load_icon("qsnapsettings"), "&Settings...", &menu);
        connect(settingsQSnapAction, SIGNAL(triggered()), this, SLOT(settingsQSnap()));
        menu.addAction(settingsQSnapAction);
    }
    else if (objectName() == "StatusBarButtonQTRACK") {
        QAction* settingsQTrackAction = new QAction(mainWin->load_icon("qtracksettings"), "&Settings...", &menu);
        connect(settingsQTrackAction, SIGNAL(triggered()), this, SLOT(settingsQTrack()));
        menu.addAction(settingsQTrackAction);
    }
    else if (objectName() == "StatusBarButtonLWT") {
        View* gview = mainWin->activeView();
        if (gview) {
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
#endif
