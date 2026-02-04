#include <QAction>
#include <QApplication>
#include <QMdiArea>

#include "mainwindow.h"
#include "commands.h"

void MainWindow::createAllActions()
{
    qDebug("Creating All Actions...");
    for (int i=0; command_table[i].id != ACTION_null; i++) {
        actionHash.insert(command_table[i].id,
            createAction(command_table[i].label,
                tr(command_table[i].tooltip),
                tr(command_table[i].statustip)));
    }

    actionHash.value(ACTION_windowclose)->setEnabled(numOfDocs > 0);
    actionHash.value(ACTION_designdetails)->setEnabled(numOfDocs > 0);
}

QAction *MainWindow::createAction(const QString icon, const QString toolTip, const QString statusTip, bool scripted)
{
    QString appDir = qApp->applicationDirPath();

    QAction *ACTION = new QAction(QIcon(appDir + "/icons/" + state.settings.general_icon_theme + "/" + icon + ".png"), toolTip, this); //TODO: Qt4.7 wont load icons without an extension...
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

