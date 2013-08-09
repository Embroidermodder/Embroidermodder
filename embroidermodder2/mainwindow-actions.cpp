#include "mainwindow.h"
#include <QAction>
#include <QApplication>
#include <QMdiArea>

void MainWindow::createAllActions()
{
    qDebug("Creating All Actions...");
    QString appName = QApplication::applicationName();

    actionDict.insert(ACTION_donothing, createAction(tr("donothing"), tr("&Do Nothing"), tr("Does Nothing")));

    actionDict.insert(ACTION_windowcascade,  createAction(tr("windowcascade"),  tr("&Cascade"),   tr("Cascade the windows.")));
    actionDict.insert(ACTION_windowtile,     createAction(tr("windowtile"),     tr("&Tile"),      tr("Tile the windows.")));
    actionDict.insert(ACTION_windowclose,    createAction(tr("windowclose"),    tr("Cl&ose"),     tr("Close the active window.")));
    actionDict.insert(ACTION_windowcloseall, createAction(tr("windowcloseall"), tr("Close &All"), tr("Close all the windows.")));
    actionDict.insert(ACTION_windownext,     createAction(tr("windownext"),     tr("Ne&xt"),      tr("Move the focus to the next window.")));
    actionDict.insert(ACTION_windowprevious, createAction(tr("windowprevious"), tr("Pre&vious"),  tr("Move the focus to the previous window.")));

    actionDict.insert(ACTION_new,           createAction(tr("new"),           tr("&New"),     tr("Create a new file.")));
    actionDict.insert(ACTION_open,          createAction(tr("open"),          tr("&Open"),    tr("Open an existing file.")));
    actionDict.insert(ACTION_save,          createAction(tr("save"),          tr("&Save"),    tr("Save the design to disk.")));
    actionDict.insert(ACTION_saveas,        createAction(tr("saveas"),        tr("Save &As"), tr("Save the design under a new name.")));
    actionDict.insert(ACTION_print,         createAction(tr("print"),         tr("&Print"),   tr("Print the design.")));
    actionDict.insert(ACTION_designdetails, createAction(tr("designdetails"), tr("&Details"), tr("Details of the current design.")));
    actionDict.insert(ACTION_exit,          createAction(tr("exit"),          tr("E&xit"),    tr("Exit the application.")));

    actionDict.insert(ACTION_cut,   createAction(tr("cut"),   tr("Cu&t"),   tr("Cut the current selection's contents to the clipboard")));
    actionDict.insert(ACTION_copy,  createAction(tr("copy"),  tr("&Copy"),  tr("Copy the current selection's contents to the clipboard")));
    actionDict.insert(ACTION_paste, createAction(tr("paste"), tr("&Paste"), tr("Paste the clipboard's contents into the current selection")));

    actionDict.insert(ACTION_help,        createAction(tr("help"),        tr("&Help"),             tr("Displays help.")));
    actionDict.insert(ACTION_changelog,   createAction(tr("changelog"),   tr("&Changelog"),        tr("Describes new features in this product.")));
    actionDict.insert(ACTION_tipoftheday, createAction(tr("tipoftheday"), tr("&Tip Of The Day"),   tr("Displays a dialog with useful tips")));
    actionDict.insert(ACTION_about,       createAction(tr("about"),       tr("&About ") + appName, tr("Displays information about this product.")));

    actionDict.insert(ACTION_undo, createAction(tr("undo"), tr("&Undo"),  tr("Reverses the most recent action.")));
    actionDict.insert(ACTION_redo, createAction(tr("redo"), tr("&Redo"),  tr("Reverses the effects of the previous undo action.")));

    actionDict.insert(ACTION_icon16,  createAction(tr("icon16"),  tr("Icon&16"),  tr("Sets the toolbar icon size to 16x16.")));
    actionDict.insert(ACTION_icon24,  createAction(tr("icon24"),  tr("Icon&24"),  tr("Sets the toolbar icon size to 24x24.")));
    actionDict.insert(ACTION_icon32,  createAction(tr("icon32"),  tr("Icon&32"),  tr("Sets the toolbar icon size to 32x32.")));
    actionDict.insert(ACTION_icon48,  createAction(tr("icon48"),  tr("Icon&48"),  tr("Sets the toolbar icon size to 48x48.")));
    actionDict.insert(ACTION_icon64,  createAction(tr("icon64"),  tr("Icon&64"),  tr("Sets the toolbar icon size to 64x64.")));
    actionDict.insert(ACTION_icon128, createAction(tr("icon128"), tr("Icon12&8"), tr("Sets the toolbar icon size to 128x128.")));

    actionDict.insert(ACTION_settingsdialog, createAction(tr("settingsdialog"), tr("&Settings"), tr("Configure settings specific to this product.")));

    actionDict.insert(ACTION_makelayercurrent,   createAction(tr("makelayercurrent"),   tr("&Make Layer Active"),    tr("Makes the layer of a selected object the active layer")));
    actionDict.insert(ACTION_layers,             createAction(tr("layers"),             tr("&Layers"),                tr("Manages layers and layer properties:  LAYER")));
    actionDict.insert(ACTION_layerselector,      createAction(tr("layerselector"),      tr("&Layer Selector"),        tr("Dropdown selector for changing the current layer")));
    actionDict.insert(ACTION_layerprevious,      createAction(tr("layerprevious"),      tr("&Layer Previous"),        tr("Restores the previous layer settings:  LAYERP")));
    actionDict.insert(ACTION_colorselector,      createAction(tr("colorselector"),      tr("&Color Selector"),        tr("Dropdown selector for changing the current thread color")));
    actionDict.insert(ACTION_linetypeselector,   createAction(tr("linetypeselector"),   tr("&Stitchtype Selector"),   tr("Dropdown selector for changing the current stitch type")));
    actionDict.insert(ACTION_lineweightselector, createAction(tr("lineweightselector"), tr("&Threadweight Selector"), tr("Dropdown selector for changing the current thread weight")));
    actionDict.insert(ACTION_hidealllayers,      createAction(tr("hidealllayers"),      tr("&Hide All Layers"),       tr("Turns the visibility off for all layers in the current drawing:  HIDEALL")));
    actionDict.insert(ACTION_showalllayers,      createAction(tr("showalllayers"),      tr("&Show All Layers"),       tr("Turns the visibility on for all layers in the current drawing:  SHOWALL")));
    actionDict.insert(ACTION_freezealllayers,    createAction(tr("freezealllayers"),    tr("&Freeze All Layers"),     tr("Freezes all layers in the current drawing:  FREEZEALL")));
    actionDict.insert(ACTION_thawalllayers,      createAction(tr("thawalllayers"),      tr("&Thaw All Layers"),       tr("Thaws all layers in the current drawing:  THAWALL")));
    actionDict.insert(ACTION_lockalllayers,      createAction(tr("lockalllayers"),      tr("&Lock All Layers"),       tr("Locks all layers in the current drawing:  LOCKALL")));
    actionDict.insert(ACTION_unlockalllayers,    createAction(tr("unlockalllayers"),    tr("&Unlock All Layers"),     tr("Unlocks all layers in the current drawing:  UNLOCKALL")));

    actionDict.insert(ACTION_textbold,      createAction(tr("textbold"),      tr("&Bold Text"),      tr("Sets text to be bold.")));
    actionDict.insert(ACTION_textitalic,    createAction(tr("textitalic"),    tr("&Italic Text"),    tr("Sets text to be italic.")));
    actionDict.insert(ACTION_textunderline, createAction(tr("textunderline"), tr("&Underline Text"), tr("Sets text to be underlined.")));
    actionDict.insert(ACTION_textstrikeout, createAction(tr("textstrikeout"), tr("&StrikeOut Text"), tr("Sets text to be striked out.")));
    actionDict.insert(ACTION_textoverline,  createAction(tr("textoverline"),  tr("&Overline Text"),  tr("Sets text to be overlined.")));

    actionDict.insert(ACTION_zoomrealtime, createAction(tr("zoomrealtime"), tr("Zoom &Realtime"), tr("Zooms to increase or decrease the apparent size of objects in the current viewport.")));
    actionDict.insert(ACTION_zoomprevious, createAction(tr("zoomprevious"), tr("Zoom &Previous"), tr("Zooms to display the previous view.")));
    actionDict.insert(ACTION_zoomwindow,   createAction(tr("zoomwindow"),   tr("Zoom &Window"),   tr("Zooms to display an area specified by a rectangular window.")));
    actionDict.insert(ACTION_zoomdynamic,  createAction(tr("zoomdynamic"),  tr("Zoom &Dynamic"),  tr("Zooms to display the generated portion of the drawing.")));
    actionDict.insert(ACTION_zoomscale,    createAction(tr("zoomscale"),    tr("Zoom &Scale"),    tr("Zooms the display using a specified scale factor.")));
    actionDict.insert(ACTION_zoomcenter,   createAction(tr("zoomcenter"),   tr("Zoom &Center"),   tr("Zooms to display a view specified by a center point and magnification or height.")));
    actionDict.insert(ACTION_zoomin,       createAction(tr("zoomin"),       tr("Zoom &In"),       tr("Zooms to increase the apparent size of objects.")));
    actionDict.insert(ACTION_zoomout,      createAction(tr("zoomout"),      tr("Zoom &Out"),      tr("Zooms to decrease the apparent size of objects.")));
    actionDict.insert(ACTION_zoomselected, createAction(tr("zoomselected"), tr("Zoom Selec&ted"), tr("Zooms to display the selected objects.")));
    actionDict.insert(ACTION_zoomall,      createAction(tr("zoomall"),      tr("Zoom &All"),      tr("Zooms to display the drawing extents or the grid limits.")));
    actionDict.insert(ACTION_zoomextents,  createAction(tr("zoomextents"),  tr("Zoom &Extents"),  tr("Zooms to display the drawing extents.")));

    actionDict.insert(ACTION_panrealtime, createAction(tr("panrealtime"), tr("&Pan Realtime"), tr("Moves the view in the current viewport.")));
    actionDict.insert(ACTION_panpoint,    createAction(tr("panpoint"),    tr("&Pan Point"),    tr("Moves the view by the specified distance.")));
    actionDict.insert(ACTION_panleft,     createAction(tr("panleft"),     tr("&Pan Left"),     tr("Moves the view to the left.")));
    actionDict.insert(ACTION_panright,    createAction(tr("panright"),    tr("&Pan Right"),    tr("Moves the view to the right.")));
    actionDict.insert(ACTION_panup,       createAction(tr("panup"),       tr("&Pan Up"),       tr("Moves the view up.")));
    actionDict.insert(ACTION_pandown,     createAction(tr("pandown"),     tr("&Pan Down"),     tr("Moves the view down.")));

    actionDict.insert(ACTION_reduce,  createAction(tr("reduce"),  tr("&Reduce"),  tr("Reduces stuff")));
    actionDict.insert(ACTION_enlarge, createAction(tr("enlarge"), tr("&Enlarge"), tr("Enlarges stuff")));

    actionDict.value(ACTION_windowclose)->setEnabled(numOfDocs > 0);
    actionDict.value(ACTION_designdetails)->setEnabled(numOfDocs > 0);
}

QAction *MainWindow::createAction(const QString icon, const QString toolTip, const QString statusTip, bool scripted)
{
    QAction *ACTION = new QAction(QIcon("icons/" + getSettingsGeneralIconTheme() + "/" + icon + ".png"), toolTip, this); //TODO: Qt4.7 wont load icons without an extension...
    ACTION->setStatusTip(statusTip);
    ACTION->setObjectName(icon);
    // TODO: Finish All Commands ... <.<

    if     (icon == "donothing")                  connect(ACTION, SIGNAL(triggered()), this, SLOT(doNothing()));
    else if(icon == "new")                      { ACTION->setShortcut(QKeySequence::New);      connect(ACTION, SIGNAL(triggered()), this, SLOT(newfile()));         }
    else if(icon == "open")                     { ACTION->setShortcut(QKeySequence::Open);     connect(ACTION, SIGNAL(triggered()), this, SLOT(openfile()));        }
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
    else if(icon == "aboutQt")                    connect(ACTION, SIGNAL(triggered()), qApp, SLOT(aboutQt()));

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

    else if(scripted)
    {
                                                  ACTION->setIcon(QIcon("commands/" + icon + "/" + icon + ".png"));
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
