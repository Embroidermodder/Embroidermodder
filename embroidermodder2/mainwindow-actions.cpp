#include "embroidermodder.h"
#include <QAction>
#include <QApplication>
#include <QMdiArea>

void MainWindow::createAllActions()
{
    qDebug("Creating All Actions...");
    QString appName = QApplication::applicationName();

    actionHash.insert(ACTION_donothing, createAction("donothing", tr("&Do Nothing"), tr("Does Nothing")));

    actionHash.insert(ACTION_windowcascade,  createAction("windowcascade",  tr("&Cascade"),   tr("Cascade the windows.")));
    actionHash.insert(ACTION_windowtile,     createAction("windowtile",     tr("&Tile"),      tr("Tile the windows.")));
    actionHash.insert(ACTION_windowclose,    createAction("windowclose",    tr("Cl&ose"),     tr("Close the active window.")));
    actionHash.insert(ACTION_windowcloseall, createAction("windowcloseall", tr("Close &All"), tr("Close all the windows.")));
    actionHash.insert(ACTION_windownext,     createAction("windownext",     tr("Ne&xt"),      tr("Move the focus to the next window.")));
    actionHash.insert(ACTION_windowprevious, createAction("windowprevious", tr("Pre&vious"),  tr("Move the focus to the previous window.")));

    actionHash.insert(ACTION_new,           createAction("new",           tr("&New"),     tr("Create a new file.")));
    actionHash.insert(ACTION_open,          createAction("open",          tr("&Open"),    tr("Open an existing file.")));
    actionHash.insert(ACTION_save,          createAction("save",          tr("&Save"),    tr("Save the design to disk.")));
    actionHash.insert(ACTION_saveas,        createAction("saveas",        tr("Save &As"), tr("Save the design under a new name.")));
    actionHash.insert(ACTION_print,         createAction("print",         tr("&Print"),   tr("Print the design.")));
    actionHash.insert(ACTION_designdetails, createAction("designdetails", tr("&Details"), tr("Details of the current design.")));
    actionHash.insert(ACTION_exit,          createAction("exit",          tr("E&xit"),    tr("Exit the application.")));

    actionHash.insert(ACTION_cut,   createAction("cut",   tr("Cu&t"),   tr("Cut the current selection's contents to the clipboard.")));
    actionHash.insert(ACTION_copy,  createAction("copy",  tr("&Copy"),  tr("Copy the current selection's contents to the clipboard.")));
    actionHash.insert(ACTION_paste, createAction("paste", tr("&Paste"), tr("Paste the clipboard's contents into the current selection.")));

    actionHash.insert(ACTION_help,        createAction("help",        tr("&Help"),             tr("Displays help.")));
    actionHash.insert(ACTION_changelog,   createAction("changelog",   tr("&Changelog"),        tr("Describes new features in this product.")));
    actionHash.insert(ACTION_tipoftheday, createAction("tipoftheday", tr("&Tip Of The Day"),   tr("Displays a dialog with useful tips")));
    actionHash.insert(ACTION_about,       createAction("about",       tr("&About ") + appName, tr("Displays information about this product.")));
    actionHash.insert(ACTION_whatsthis,   createAction("whatsthis",   tr("&What's This?"),     tr("What's This? Context Help!")));

    actionHash.insert(ACTION_undo, createAction("undo", tr("&Undo"),  tr("Reverses the most recent action.")));
    actionHash.insert(ACTION_redo, createAction("redo", tr("&Redo"),  tr("Reverses the effects of the previous undo action.")));

    actionHash.insert(ACTION_icon16,  createAction("icon16",  tr("Icon&16"),  tr("Sets the toolbar icon size to 16x16.")));
    actionHash.insert(ACTION_icon24,  createAction("icon24",  tr("Icon&24"),  tr("Sets the toolbar icon size to 24x24.")));
    actionHash.insert(ACTION_icon32,  createAction("icon32",  tr("Icon&32"),  tr("Sets the toolbar icon size to 32x32.")));
    actionHash.insert(ACTION_icon48,  createAction("icon48",  tr("Icon&48"),  tr("Sets the toolbar icon size to 48x48.")));
    actionHash.insert(ACTION_icon64,  createAction("icon64",  tr("Icon&64"),  tr("Sets the toolbar icon size to 64x64.")));
    actionHash.insert(ACTION_icon128, createAction("icon128", tr("Icon12&8"), tr("Sets the toolbar icon size to 128x128.")));

    actionHash.insert(ACTION_settingsdialog, createAction("settingsdialog", tr("&Settings"), tr("Configure settings specific to this product.")));

    actionHash.insert(ACTION_makelayercurrent,   createAction("makelayercurrent",   tr("&Make Layer Active"),    tr("Makes the layer of a selected object the active layer")));
    actionHash.insert(ACTION_layers,             createAction("layers",             tr("&Layers"),                tr("Manages layers and layer properties:  LAYER")));
    actionHash.insert(ACTION_layerselector,      createAction("layerselector",      tr("&Layer Selector"),        tr("Dropdown selector for changing the current layer")));
    actionHash.insert(ACTION_layerprevious,      createAction("layerprevious",      tr("&Layer Previous"),        tr("Restores the previous layer settings:  LAYERP")));
    actionHash.insert(ACTION_colorselector,      createAction("colorselector",      tr("&Color Selector"),        tr("Dropdown selector for changing the current thread color")));
    actionHash.insert(ACTION_linetypeselector,   createAction("linetypeselector",   tr("&Stitchtype Selector"),   tr("Dropdown selector for changing the current stitch type")));
    actionHash.insert(ACTION_lineweightselector, createAction("lineweightselector", tr("&Threadweight Selector"), tr("Dropdown selector for changing the current thread weight")));
    actionHash.insert(ACTION_hidealllayers,      createAction("hidealllayers",      tr("&Hide All Layers"),       tr("Turns the visibility off for all layers in the current drawing:  HIDEALL")));
    actionHash.insert(ACTION_showalllayers,      createAction("showalllayers",      tr("&Show All Layers"),       tr("Turns the visibility on for all layers in the current drawing:  SHOWALL")));
    actionHash.insert(ACTION_freezealllayers,    createAction("freezealllayers",    tr("&Freeze All Layers"),     tr("Freezes all layers in the current drawing:  FREEZEALL")));
    actionHash.insert(ACTION_thawalllayers,      createAction("thawalllayers",      tr("&Thaw All Layers"),       tr("Thaws all layers in the current drawing:  THAWALL")));
    actionHash.insert(ACTION_lockalllayers,      createAction("lockalllayers",      tr("&Lock All Layers"),       tr("Locks all layers in the current drawing:  LOCKALL")));
    actionHash.insert(ACTION_unlockalllayers,    createAction("unlockalllayers",    tr("&Unlock All Layers"),     tr("Unlocks all layers in the current drawing:  UNLOCKALL")));

    actionHash.insert(ACTION_textbold,      createAction("textbold",      tr("&Bold Text"),      tr("Sets text to be bold.")));
    actionHash.insert(ACTION_textitalic,    createAction("textitalic",    tr("&Italic Text"),    tr("Sets text to be italic.")));
    actionHash.insert(ACTION_textunderline, createAction("textunderline", tr("&Underline Text"), tr("Sets text to be underlined.")));
    actionHash.insert(ACTION_textstrikeout, createAction("textstrikeout", tr("&StrikeOut Text"), tr("Sets text to be striked out.")));
    actionHash.insert(ACTION_textoverline,  createAction("textoverline",  tr("&Overline Text"),  tr("Sets text to be overlined.")));

    actionHash.insert(ACTION_zoomrealtime, createAction("zoomrealtime", tr("Zoom &Realtime"), tr("Zooms to increase or decrease the apparent size of objects in the current viewport.")));
    actionHash.insert(ACTION_zoomprevious, createAction("zoomprevious", tr("Zoom &Previous"), tr("Zooms to display the previous view.")));
    actionHash.insert(ACTION_zoomwindow,   createAction("zoomwindow",   tr("Zoom &Window"),   tr("Zooms to display an area specified by a rectangular window.")));
    actionHash.insert(ACTION_zoomdynamic,  createAction("zoomdynamic",  tr("Zoom &Dynamic"),  tr("Zooms to display the generated portion of the drawing.")));
    actionHash.insert(ACTION_zoomscale,    createAction("zoomscale",    tr("Zoom &Scale"),    tr("Zooms the display using a specified scale factor.")));
    actionHash.insert(ACTION_zoomcenter,   createAction("zoomcenter",   tr("Zoom &Center"),   tr("Zooms to display a view specified by a center point and magnification or height.")));
    actionHash.insert(ACTION_zoomin,       createAction("zoomin",       tr("Zoom &In"),       tr("Zooms to increase the apparent size of objects.")));
    actionHash.insert(ACTION_zoomout,      createAction("zoomout",      tr("Zoom &Out"),      tr("Zooms to decrease the apparent size of objects.")));
    actionHash.insert(ACTION_zoomselected, createAction("zoomselected", tr("Zoom Selec&ted"), tr("Zooms to display the selected objects.")));
    actionHash.insert(ACTION_zoomall,      createAction("zoomall",      tr("Zoom &All"),      tr("Zooms to display the drawing extents or the grid limits.")));
    actionHash.insert(ACTION_zoomextents,  createAction("zoomextents",  tr("Zoom &Extents"),  tr("Zooms to display the drawing extents.")));

    actionHash.insert(ACTION_panrealtime, createAction("panrealtime", tr("&Pan Realtime"), tr("Moves the view in the current viewport.")));
    actionHash.insert(ACTION_panpoint,    createAction("panpoint",    tr("&Pan Point"),    tr("Moves the view by the specified distance.")));
    actionHash.insert(ACTION_panleft,     createAction("panleft",     tr("&Pan Left"),     tr("Moves the view to the left.")));
    actionHash.insert(ACTION_panright,    createAction("panright",    tr("&Pan Right"),    tr("Moves the view to the right.")));
    actionHash.insert(ACTION_panup,       createAction("panup",       tr("&Pan Up"),       tr("Moves the view up.")));
    actionHash.insert(ACTION_pandown,     createAction("pandown",     tr("&Pan Down"),     tr("Moves the view down.")));

    actionHash.insert(ACTION_day,   createAction("day",   tr("&Day"),   tr("Updates the current view using day vision settings.")));
    actionHash.insert(ACTION_night, createAction("night", tr("&Night"), tr("Updates the current view using night vision settings.")));

    actionHash.value(ACTION_windowclose)->setEnabled(numOfDocs > 0);
    actionHash.value(ACTION_designdetails)->setEnabled(numOfDocs > 0);
}

QAction *MainWindow::createAction(const QString icon, const QString toolTip, const QString statusTip, bool scripted)
{
    QString appDir = qApp->applicationDirPath();

    QAction *ACTION = new QAction(QIcon(appDir + "/icons/" + getSettingsGeneralIconTheme() + "/" + icon + ".png"), toolTip, this); //TODO: Qt4.7 wont load icons without an extension...
    ACTION->setStatusTip(statusTip);
    ACTION->setObjectName(icon);
    // TODO: Set What's This Context Help to statusTip for now so there is some infos there.
    // Make custom whats this context help popup with more descriptive help than just
    // the status bar/tip one liner(short but not real long) with a hyperlink in the custom popup
    // at the bottom to open full help file description. Ex: like wxPython AGW's SuperToolTip.
    ACTION->setWhatsThis(statusTip);
    // TODO: Finish All Commands ... <.<

    if     (icon == "donothing")                  connect(ACTION, SIGNAL(triggered()), this, SLOT(doNothing()));
    else if(icon == "new")                      { ACTION->setShortcut(QKeySequence::New);      connect(ACTION, SIGNAL(triggered()), this, SLOT(newFile()));         }
    else if(icon == "open")                     { ACTION->setShortcut(QKeySequence::Open);     connect(ACTION, SIGNAL(triggered()), this, SLOT(openFile()));        }
    else if(icon == "save")                     { ACTION->setShortcut(QKeySequence::Save);     connect(ACTION, SIGNAL(triggered()), this, SLOT(savefile()));        }
    else if(icon == "saveas")                   { ACTION->setShortcut(QKeySequence::SaveAs);   connect(ACTION, SIGNAL(triggered()), this, SLOT(saveasfile()));      }
    else if(icon == "print")                    { ACTION->setShortcut(QKeySequence::Print);    connect(ACTION, SIGNAL(triggered()), this, SLOT(print()));           }
    else if(icon == "designdetails")            { ACTION->setShortcut(QKeySequence("Ctrl+D")); connect(ACTION, SIGNAL(triggered()), this, SLOT(designDetails()));   }
    else if(icon == "exit")                     { ACTION->setShortcut(QKeySequence("Ctrl+Q")); connect(ACTION, SIGNAL(triggered()), this, SLOT(exit()));            }

    else if(icon == "cut")                      { ACTION->setShortcut(QKeySequence::Cut);   connect(ACTION, SIGNAL(triggered()), this, SLOT(cut()));   }
    else if(icon == "copy")                     { ACTION->setShortcut(QKeySequence::Copy);  connect(ACTION, SIGNAL(triggered()), this, SLOT(copy()));  }
    else if(icon == "paste")                    { ACTION->setShortcut(QKeySequence::Paste); connect(ACTION, SIGNAL(triggered()), this, SLOT(paste())); }

    else if(icon == "windowcascade")              connect(ACTION, SIGNAL(triggered()), mdiArea, SLOT(cascade()));
    else if(icon == "windowtile")                 connect(ACTION, SIGNAL(triggered()), mdiArea, SLOT(tile()));
    else if(icon == "windowclose")              { ACTION->setShortcut(QKeySequence::Close);    connect(ACTION, SIGNAL(triggered()), this, SLOT(onCloseWindow()));   }
    else if(icon == "windowcloseall")             connect(ACTION, SIGNAL(triggered()), mdiArea, SLOT(closeAllSubWindows()));
    else if(icon == "windownext")               { ACTION->setShortcut(QKeySequence::NextChild);     connect(ACTION, SIGNAL(triggered()), mdiArea, SLOT(activateNextSubWindow()));     }
    else if(icon == "windowprevious")           { ACTION->setShortcut(QKeySequence::PreviousChild); connect(ACTION, SIGNAL(triggered()), mdiArea, SLOT(activatePreviousSubWindow())); }

    else if(icon == "help")                       connect(ACTION, SIGNAL(triggered()), this, SLOT(help()));
    else if(icon == "changelog")                  connect(ACTION, SIGNAL(triggered()), this, SLOT(changelog()));
    else if(icon == "tipoftheday")                connect(ACTION, SIGNAL(triggered()), this, SLOT(tipOfTheDay()));
    else if(icon == "about")                      connect(ACTION, SIGNAL(triggered()), this, SLOT(about()));
    else if(icon == "whatsthis")                  connect(ACTION, SIGNAL(triggered()), this, SLOT(whatsThisContextHelp()));

    else if(icon == "icon16")                     connect(ACTION, SIGNAL(triggered()), this, SLOT(icon16()));
    else if(icon == "icon24")                     connect(ACTION, SIGNAL(triggered()), this, SLOT(icon24()));
    else if(icon == "icon32")                     connect(ACTION, SIGNAL(triggered()), this, SLOT(icon32()));
    else if(icon == "icon48")                     connect(ACTION, SIGNAL(triggered()), this, SLOT(icon48()));
    else if(icon == "icon64")                     connect(ACTION, SIGNAL(triggered()), this, SLOT(icon64()));
    else if(icon == "icon128")                    connect(ACTION, SIGNAL(triggered()), this, SLOT(icon128()));

    else if(icon == "settingsdialog")             connect(ACTION, SIGNAL(triggered()), this, SLOT(settingsDialog()));

    else if(icon == "undo")                       connect(ACTION, SIGNAL(triggered()), this, SLOT(undo()));
    else if(icon == "redo")                       connect(ACTION, SIGNAL(triggered()), this, SLOT(redo()));

    else if(icon == "makelayercurrent")           connect(ACTION, SIGNAL(triggered()), this, SLOT(makeLayerActive()));
    else if(icon == "layers")                     connect(ACTION, SIGNAL(triggered()), this, SLOT(layerManager()));
    else if(icon == "layerprevious")              connect(ACTION, SIGNAL(triggered()), this, SLOT(layerPrevious()));

    else if(icon == "textbold")                 { ACTION->setCheckable(true); connect(ACTION, SIGNAL(toggled(bool)), this, SLOT(setTextBold(bool)));   }
    else if(icon == "textitalic")               { ACTION->setCheckable(true); connect(ACTION, SIGNAL(toggled(bool)), this, SLOT(setTextItalic(bool))); }
    else if(icon == "textunderline")            { ACTION->setCheckable(true); connect(ACTION, SIGNAL(toggled(bool)), this, SLOT(setTextUnderline(bool))); }
    else if(icon == "textstrikeout")            { ACTION->setCheckable(true); connect(ACTION, SIGNAL(toggled(bool)), this, SLOT(setTextStrikeOut(bool))); }
    else if(icon == "textoverline")             { ACTION->setCheckable(true); connect(ACTION, SIGNAL(toggled(bool)), this, SLOT(setTextOverline(bool))); }

    else if(icon == "zoomrealtime")               connect(ACTION, SIGNAL(triggered()), this, SLOT(zoomRealtime()));
    else if(icon == "zoomprevious")               connect(ACTION, SIGNAL(triggered()), this, SLOT(zoomPrevious()));
    else if(icon == "zoomwindow")                 connect(ACTION, SIGNAL(triggered()), this, SLOT(zoomWindow()));
    else if(icon == "zoomdynamic")                connect(ACTION, SIGNAL(triggered()), this, SLOT(zoomDynamic()));
    else if(icon == "zoomscale")                  connect(ACTION, SIGNAL(triggered()), this, SLOT(zoomScale()));
    else if(icon == "zoomcenter")                 connect(ACTION, SIGNAL(triggered()), this, SLOT(zoomCenter()));
    else if(icon == "zoomin")                     connect(ACTION, SIGNAL(triggered()), this, SLOT(zoomIn()));
    else if(icon == "zoomout")                    connect(ACTION, SIGNAL(triggered()), this, SLOT(zoomOut()));
    else if(icon == "zoomselected")               connect(ACTION, SIGNAL(triggered()), this, SLOT(zoomSelected()));
    else if(icon == "zoomall")                    connect(ACTION, SIGNAL(triggered()), this, SLOT(zoomAll()));
    else if(icon == "zoomextents")                connect(ACTION, SIGNAL(triggered()), this, SLOT(zoomExtents()));

    else if(icon == "panrealtime")                connect(ACTION, SIGNAL(triggered()), this, SLOT(panrealtime()));
    else if(icon == "panpoint")                   connect(ACTION, SIGNAL(triggered()), this, SLOT(panpoint()));
    else if(icon == "panleft")                    connect(ACTION, SIGNAL(triggered()), this, SLOT(panLeft()));
    else if(icon == "panright")                   connect(ACTION, SIGNAL(triggered()), this, SLOT(panRight()));
    else if(icon == "panup")                      connect(ACTION, SIGNAL(triggered()), this, SLOT(panUp()));
    else if(icon == "pandown")                    connect(ACTION, SIGNAL(triggered()), this, SLOT(panDown()));

    else if(icon == "day")                        connect(ACTION, SIGNAL(triggered()), this, SLOT(dayVision()));
    else if(icon == "night")                      connect(ACTION, SIGNAL(triggered()), this, SLOT(nightVision()));

    else if(scripted)
    {
                                                  ACTION->setIcon(QIcon(appDir + "/commands/" + icon + "/" + icon + ".png"));
                                                  connect(ACTION, SIGNAL(triggered()), this, SLOT(runCommand()));
    }
    else
    {
                                                  ACTION->setEnabled(false);
                                                  connect(ACTION, SIGNAL(triggered()), this, SLOT(stub_implement()));
    }
    return ACTION;
}

/* kate: bom off; indent-mode cstyle; indent-width 4; replace-trailing-space-save on; */
