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

/**
 * \file mainwindow-actions.cpp
 */

#include "embroidermodder.h"

/**
 * \todo load this table from file as part of initialisation.
 */
std::vector<Action>
action_table = {
    {
        .hash = ACTION_donothing,
        .icon = "donothing",
        .tooltip = "&Do Nothing",
        .statustip = "Does Nothing"
    },
    {
        .hash = ACTION_windowcascade,
        .icon = "windowcascade",
        .tooltip = "&Cascade",
        .statustip = "Cascade the windows."
    },
    {
        .hash = ACTION_new,
        .icon = "new",
        .tooltip = "&New",
        .statustip = "Create a new file."
    }
};

int n_actions = 69;

/**
 * @brief MainWindow::createAllActions
 */
void
MainWindow::createAllActions()
{
    qDebug("Creating All Actions...");

    for (int i=0; i<action_table.size(); i++) {
        actionHash[action_table[i].hash] =
            createAction(action_table[i].icon,
                tr(action_table[i].tooltip),
                tr(action_table[i].statustip));
    }    

    actionHash[ACTION_windowtile] = createAction("windowtile", tr("&Tile"), tr("Tile the windows."));
    actionHash[ACTION_windowclose] = createAction("windowclose", tr("Cl&ose"), tr("Close the active window."));
    actionHash[ACTION_windowcloseall] = createAction("windowcloseall", tr("Close &All"), tr("Close all the windows."));
    actionHash[ACTION_windownext] = createAction("windownext", tr("Ne&xt"), tr("Move the focus to the next window."));
    actionHash[ACTION_windowprevious] = createAction("windowprevious", tr("Pre&vious"), tr("Move the focus to the previous window."));

    actionHash[ACTION_open] = createAction("open", tr("&Open"), tr("Open an existing file."));
    actionHash[ACTION_save] = createAction("save", tr("&Save"), tr("Save the design to disk."));
    actionHash[ACTION_saveas] = createAction("saveas", tr("Save &As"), tr("Save the design under a new name."));
    actionHash[ACTION_print] = createAction("print", tr("&Print"), tr("Print the design."));
    actionHash[ACTION_designdetails] = createAction("designdetails", tr("&Details"), tr("Details of the current design."));
    actionHash[ACTION_exit] = createAction("exit", tr("E&xit"), tr("Exit the application."));

    actionHash[ACTION_cut] = createAction("cut", tr("Cu&t"), tr("Cut the current selection's contents to the clipboard."));
    actionHash[ACTION_copy] = createAction("copy", tr("&Copy"), tr("Copy the current selection's contents to the clipboard."));
    actionHash[ACTION_paste] = createAction("paste", tr("&Paste"), tr("Paste the clipboard's contents into the current selection."));

    actionHash[ACTION_help] = createAction("help", tr("&Help"), tr("Displays help."));
    actionHash[ACTION_changelog] = createAction("changelog", tr("&Changelog"), tr("Describes new features in this product."));
    actionHash[ACTION_tipoftheday] = createAction("tipoftheday", tr("&Tip Of The Day"), tr("Displays a dialog with useful tips"));
    actionHash[ACTION_about] = createAction("about", tr("&About ") + "Embroidermodder", tr("Displays information about this product."));
    actionHash[ACTION_whatsthis] = createAction("whatsthis", tr("&What's This?"), tr("What's This? Context Help!"));

    actionHash[ACTION_undo] = createAction("undo", tr("&Undo"), tr("Reverses the most recent action."));
    actionHash[ACTION_redo] = createAction("redo", tr("&Redo"), tr("Reverses the effects of the previous undo action."));

    actionHash[ACTION_icon16] = createAction("icon16", tr("Icon&16"), tr("Sets the toolbar icon size to 16x16."));
    actionHash[ACTION_icon24] = createAction("icon24", tr("Icon&24"), tr("Sets the toolbar icon size to 24x24."));
    actionHash[ACTION_icon32] = createAction("icon32", tr("Icon&32"), tr("Sets the toolbar icon size to 32x32."));
    actionHash[ACTION_icon48] = createAction("icon48", tr("Icon&48"), tr("Sets the toolbar icon size to 48x48."));
    actionHash[ACTION_icon64] = createAction("icon64", tr("Icon&64"), tr("Sets the toolbar icon size to 64x64."));
    actionHash[ACTION_icon128] = createAction("icon128", tr("Icon12&8"), tr("Sets the toolbar icon size to 128x128."));

    actionHash[ACTION_settingsdialog] = createAction("settingsdialog", tr("&Settings"), tr("Configure settings specific to this product."));

    actionHash[ACTION_makelayercurrent] = createAction("makelayercurrent", tr("&Make Layer Active"), tr("Makes the layer of a selected object the active layer"));
    actionHash[ACTION_layers] = createAction("layers", tr("&Layers"), tr("Manages layers and layer properties:  LAYER"));
    actionHash[ACTION_layerselector] = createAction("layerselector", tr("&Layer Selector"), tr("Dropdown selector for changing the current layer"));
    actionHash[ACTION_layerprevious] = createAction("layerprevious", tr("&Layer Previous"), tr("Restores the previous layer settings:  LAYERP"));
    actionHash[ACTION_colorselector] = createAction("colorselector", tr("&Color Selector"), tr("Dropdown selector for changing the current thread color"));
    actionHash[ACTION_linetypeselector] = createAction("linetypeselector", tr("&Stitchtype Selector"), tr("Dropdown selector for changing the current stitch type"));
    actionHash[ACTION_lineweightselector] = createAction("lineweightselector", tr("&Threadweight Selector"), tr("Dropdown selector for changing the current thread weight"));
    actionHash[ACTION_hidealllayers] = createAction("hidealllayers", tr("&Hide All Layers"), tr("Turns the visibility off for all layers in the current drawing:  HIDEALL"));
    actionHash[ACTION_showalllayers] = createAction("showalllayers", tr("&Show All Layers"), tr("Turns the visibility on for all layers in the current drawing:  SHOWALL"));
    actionHash[ACTION_freezealllayers] = createAction("freezealllayers", tr("&Freeze All Layers"), tr("Freezes all layers in the current drawing:  FREEZEALL"));
    actionHash[ACTION_thawalllayers] = createAction("thawalllayers", tr("&Thaw All Layers"), tr("Thaws all layers in the current drawing:  THAWALL"));
    actionHash[ACTION_lockalllayers] = createAction("lockalllayers", tr("&Lock All Layers"), tr("Locks all layers in the current drawing:  LOCKALL"));
    actionHash[ACTION_unlockalllayers] = createAction("unlockalllayers", tr("&Unlock All Layers"), tr("Unlocks all layers in the current drawing:  UNLOCKALL"));

    actionHash[ACTION_textbold] = createAction("textbold", tr("&Bold Text"), tr("Sets text to be bold."));
    actionHash[ACTION_textitalic] = createAction("textitalic", tr("&Italic Text"), tr("Sets text to be italic."));
    actionHash[ACTION_textunderline] = createAction("textunderline", tr("&Underline Text"), tr("Sets text to be underlined."));
    actionHash[ACTION_textstrikeout] = createAction("textstrikeout", tr("&StrikeOut Text"), tr("Sets text to be striked out."));
    actionHash[ACTION_textoverline] = createAction("textoverline", tr("&Overline Text"), tr("Sets text to be overlined."));

    actionHash[ACTION_zoomrealtime] = createAction("zoomrealtime", tr("Zoom &Realtime"), tr("Zooms to increase or decrease the apparent size of objects in the current viewport."));
    actionHash[ACTION_zoomprevious] = createAction("zoomprevious", tr("Zoom &Previous"), tr("Zooms to display the previous view."));
    actionHash[ACTION_zoomwindow] = createAction("zoomwindow", tr("Zoom &Window"), tr("Zooms to display an area specified by a rectangular window."));
    actionHash[ACTION_zoomdynamic] = createAction("zoomdynamic", tr("Zoom &Dynamic"), tr("Zooms to display the generated portion of the drawing."));
    actionHash[ACTION_zoomscale] = createAction("zoomscale", tr("Zoom &Scale"), tr("Zooms the display using a specified scale factor."));
    actionHash[ACTION_zoomcenter] = createAction("zoomcenter", tr("Zoom &Center"), tr("Zooms to display a view specified by a center point and magnification or height."));
    actionHash[ACTION_zoomin] = createAction("zoomin", tr("Zoom &In"), tr("Zooms to increase the apparent size of objects."));
    actionHash[ACTION_zoomout] = createAction("zoomout", tr("Zoom &Out"), tr("Zooms to decrease the apparent size of objects."));
    actionHash[ACTION_zoomselected] = createAction("zoomselected", tr("Zoom Selec&ted"), tr("Zooms to display the selected objects."));
    actionHash[ACTION_zoomall] = createAction("zoomall", tr("Zoom &All"), tr("Zooms to display the drawing extents or the grid limits."));
    actionHash[ACTION_zoomextents] = createAction("zoomextents", tr("Zoom &Extents"), tr("Zooms to display the drawing extents."));

    actionHash[ACTION_panrealtime] = createAction("panrealtime", tr("&Pan Realtime"), tr("Moves the view in the current viewport."));
    actionHash[ACTION_panpoint] = createAction("panpoint", tr("&Pan Point"), tr("Moves the view by the specified distance."));
    actionHash[ACTION_panleft] = createAction("panleft", tr("&Pan Left"), tr("Moves the view to the left."));
    actionHash[ACTION_panright] = createAction("panright", tr("&Pan Right"), tr("Moves the view to the right."));
    actionHash[ACTION_panup] = createAction("panup", tr("&Pan Up"), tr("Moves the view up."));
    actionHash[ACTION_pandown] = createAction("pandown", tr("&Pan Down"), tr("Moves the view down."));

    actionHash[ACTION_day] = createAction("day", tr("&Day"), tr("Updates the current view using day vision settings."));
    actionHash[ACTION_night] = createAction("night", tr("&Night"), tr("Updates the current view using night vision settings."));

    actionHash[ACTION_windowclose]->setEnabled(numOfDocs > 0);
    actionHash[ACTION_designdetails]->setEnabled(numOfDocs > 0);
}

/**
 * @brief MainWindow::createAction
 * @param icon
 * @param toolTip
 * @param statusTip
 * @param scripted
 * @return
 *
 * \todo Set What's This Context Help to statusTip for now so there is some infos there.
 * Make custom whats this context help popup with more descriptive help than just
 * the status bar/tip one liner(short but not real long) with a hyperlink in the custom popup
 * at the bottom to open full help file description. Ex: like wxPython AGW's SuperToolTip.
ACTION->setWhatsThis(statusTip);
 * \todo Finish All Commands ... <.<
 */
QAction *
MainWindow::createAction(const QString icon, const QString toolTip, const QString statusTip, bool scripted)
{
    QString appDir = qApp->applicationDirPath();

    QAction *ACTION = new QAction(QIcon(appDir + "/icons/" + settings_general_icon_theme + "/" + icon + ".png"), toolTip, this); ACTION->setStatusTip(statusTip);
    ACTION->setObjectName(icon);

    if (icon == "donothing") {
        connect(ACTION, SIGNAL(triggered()), this, SLOT(doNothing()));
    }
    else if (icon == "new") {
        ACTION->setShortcut(QKeySequence::New);
        connect(ACTION, &QAction::triggered, this,
            [=](){ this->actuator("new"); }
        );
    }
    else if (icon == "open") {
        ACTION->setShortcut(QKeySequence::Open);
        connect(ACTION, SIGNAL(triggered()), this, SLOT(openFile()));
    }
    else if (icon == "save") {
        ACTION->setShortcut(QKeySequence::Save);
        connect(ACTION, SIGNAL(triggered()), this, SLOT(savefile()));}
    else if (icon == "saveas") {
        ACTION->setShortcut(QKeySequence::SaveAs);
        connect(ACTION, SIGNAL(triggered()), this, SLOT(saveasfile())); 
    }
    else if (icon == "print") {
        ACTION->setShortcut(QKeySequence::Print);
        connect(ACTION, SIGNAL(triggered()), this, SLOT(print()));
    }
    else if (icon == "designdetails") {
        ACTION->setShortcut(QKeySequence("Ctrl+D"));
        connect(ACTION, SIGNAL(triggered()), this, SLOT(designDetails()));
    }
    else if (icon == "exit") {
        ACTION->setShortcut(QKeySequence("Ctrl+Q")); connect(ACTION, SIGNAL(triggered()), this, SLOT(exit()));            }

    else if (icon == "cut") { ACTION->setShortcut(QKeySequence::Cut);   connect(ACTION, SIGNAL(triggered()), this, SLOT(cut()));   }
    else if (icon == "copy") { ACTION->setShortcut(QKeySequence::Copy);  connect(ACTION, SIGNAL(triggered()), this, SLOT(copy()));  }
    else if (icon == "paste") { ACTION->setShortcut(QKeySequence::Paste); connect(ACTION, SIGNAL(triggered()), this, SLOT(paste())); }

    else if (icon == "windowcascade")              connect(ACTION, SIGNAL(triggered()), mdiArea, SLOT(cascade()));
    else if (icon == "windowtile")                 connect(ACTION, SIGNAL(triggered()), mdiArea, SLOT(tile()));
    else if (icon == "windowclose") { ACTION->setShortcut(QKeySequence::Close);    connect(ACTION, SIGNAL(triggered()), this, SLOT(onCloseWindow()));   }
    else if (icon == "windowcloseall")             connect(ACTION, SIGNAL(triggered()), mdiArea, SLOT(closeAllSubWindows()));
    else if (icon == "windownext") { ACTION->setShortcut(QKeySequence::NextChild);     connect(ACTION, SIGNAL(triggered()), mdiArea, SLOT(activateNextSubWindow()));     }
    else if (icon == "windowprevious") { ACTION->setShortcut(QKeySequence::PreviousChild); connect(ACTION, SIGNAL(triggered()), mdiArea, SLOT(activatePreviousSubWindow())); }

    else if (icon == "help")                       connect(ACTION, SIGNAL(triggered()), this, SLOT(help()));
    else if (icon == "changelog")                  connect(ACTION, SIGNAL(triggered()), this, SLOT(changelog()));
    else if (icon == "tipoftheday")                connect(ACTION, SIGNAL(triggered()), this, SLOT(tipOfTheDay()));
    else if (icon == "about")                      connect(ACTION, SIGNAL(triggered()), this, SLOT(about()));
    else if (icon == "whatsthis")                  connect(ACTION, SIGNAL(triggered()), this, SLOT(whatsThisContextHelp()));

    else if (icon == "icon16")                     connect(ACTION, SIGNAL(triggered()), this, SLOT(icon16()));
    else if (icon == "icon24")                     connect(ACTION, SIGNAL(triggered()), this, SLOT(icon24()));
    else if (icon == "icon32")                     connect(ACTION, SIGNAL(triggered()), this, SLOT(icon32()));
    else if (icon == "icon48")                     connect(ACTION, SIGNAL(triggered()), this, SLOT(icon48()));
    else if (icon == "icon64")                     connect(ACTION, SIGNAL(triggered()), this, SLOT(icon64()));
    else if (icon == "icon128")                    connect(ACTION, SIGNAL(triggered()), this, SLOT(icon128()));

    else if (icon == "settingsdialog")             connect(ACTION, SIGNAL(triggered()), this, SLOT(settingsDialog()));

    else if (icon == "undo")                       connect(ACTION, SIGNAL(triggered()), this, SLOT(undo()));
    else if (icon == "redo")                       connect(ACTION, SIGNAL(triggered()), this, SLOT(redo()));

    else if (icon == "makelayercurrent")           connect(ACTION, SIGNAL(triggered()), this, SLOT(makeLayerActive()));
    else if (icon == "layers")                     connect(ACTION, SIGNAL(triggered()), this, SLOT(layerManager()));
    else if (icon == "layerprevious")              connect(ACTION, SIGNAL(triggered()), this, SLOT(layerPrevious()));

    else if (icon == "textbold") { ACTION->setCheckable(true); connect(ACTION, SIGNAL(toggled(bool)), this, SLOT(setTextBold(bool)));   }
    else if (icon == "textitalic") { ACTION->setCheckable(true); connect(ACTION, SIGNAL(toggled(bool)), this, SLOT(setTextItalic(bool))); }
    else if (icon == "textunderline") { ACTION->setCheckable(true); connect(ACTION, SIGNAL(toggled(bool)), this, SLOT(setTextUnderline(bool))); }
    else if (icon == "textstrikeout") { ACTION->setCheckable(true); connect(ACTION, SIGNAL(toggled(bool)), this, SLOT(setTextStrikeOut(bool))); }
    else if (icon == "textoverline") { ACTION->setCheckable(true); connect(ACTION, SIGNAL(toggled(bool)), this, SLOT(setTextOverline(bool))); }

    else if (icon == "zoomrealtime")               connect(ACTION, SIGNAL(triggered()), this, SLOT(zoomRealtime()));
    else if (icon == "zoomprevious")               connect(ACTION, SIGNAL(triggered()), this, SLOT(zoomPrevious()));
    else if (icon == "zoomwindow")                 connect(ACTION, SIGNAL(triggered()), this, SLOT(zoomWindow()));
    else if (icon == "zoomdynamic")                connect(ACTION, SIGNAL(triggered()), this, SLOT(zoomDynamic()));
    else if (icon == "zoomscale")                  connect(ACTION, SIGNAL(triggered()), this, SLOT(zoomScale()));
    else if (icon == "zoomcenter")                 connect(ACTION, SIGNAL(triggered()), this, SLOT(zoomCenter()));
    else if (icon == "zoomin")                     connect(ACTION, SIGNAL(triggered()), this, SLOT(zoomIn()));
    else if (icon == "zoomout")                    connect(ACTION, SIGNAL(triggered()), this, SLOT(zoomOut()));
    else if (icon == "zoomselected")               connect(ACTION, SIGNAL(triggered()), this, SLOT(zoomSelected()));
    else if (icon == "zoomall")                    connect(ACTION, SIGNAL(triggered()), this, SLOT(zoomAll()));
    else if (icon == "zoomextents")                connect(ACTION, SIGNAL(triggered()), this, SLOT(zoomExtents()));

    else if (icon == "panrealtime")                connect(ACTION, SIGNAL(triggered()), this, SLOT(panrealtime()));
    else if (icon == "panpoint")                   connect(ACTION, SIGNAL(triggered()), this, SLOT(panpoint()));
    else if (icon == "panleft")                    connect(ACTION, SIGNAL(triggered()), this, SLOT(panLeft()));
    else if (icon == "panright") {
        connect(ACTION, SIGNAL(triggered()), this, SLOT(panRight()));
    }
    else if (icon == "panup") {
        connect(ACTION, SIGNAL(triggered()), this, SLOT(panUp()));
    }
    else if (icon == "pandown") {
        connect(ACTION, SIGNAL(triggered()), this, SLOT(panDown()));
    }
    else if (icon == "day") {
        connect(ACTION, SIGNAL(triggered()), this, SLOT(dayVision()));
    }
    else if (icon == "night") {
        connect(ACTION, SIGNAL(triggered()), this, SLOT(nightVision()));
    }
    else if (scripted) {
        ACTION->setIcon(QIcon(appDir + "/commands/" + icon + "/" + icon + ".png"));
        connect(ACTION, SIGNAL(triggered()), this, SLOT(runCommand()));
    }
    else {
        ACTION->setEnabled(false);
        connect(ACTION, SIGNAL(triggered()), this, SLOT(stub_implement()));
    }
    return ACTION;
}
