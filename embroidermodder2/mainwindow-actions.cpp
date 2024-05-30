/*
 * Embroidermodder 2.
 *
 * Copyright 2011-2024 The Embroidermodder Team
 * Embroidermodder 2 is Open Source Software, see LICENSE.md for licensing terms.
 * Visit https://www.libembroidery.org/refman for advice on altering this file,
 * or read the markdown version in embroidermodder2/docs/refman.
 *
 * MainWindow Actions
 */

#include "embroidermodder.h"

#include <QAction>
#include <QApplication>
#include <QMdiArea>

/* For each Command in command_data, for each alias set up a map from
 * alias to the Command. Then for a given context the call doesn't have to loop?
 *
 * NOTE: Every QScriptProgram must have a unique function name to call. If every function was called main(), then
 *       the ScriptArgs would only call the last script evaluated (which happens to be main() in another script).
 *       Thus, by adding the cmdName before main(), it becomes line_main(), circle_main(), etc...
 *       Do not change this code unless you really know what you are doing. I mean it.
 *
 * Position currently comes from the order of the command_data.
 */
void
MainWindow::createAllActions()
{
    qDebug("Creating All Actions...");
    QString appName = QApplication::applicationName();
    QString appDir = qApp->applicationDirPath();

    for (int i=0; command_data[i].menu_position >= 0; i++) {
        QString icon(command_data[i].icon);
        QString menuName(command_data[i].menu_name);
        int menuPos = command_data[i].menu_position;
        QString toolbarName(command_data[i].toolbar_name);
        int toolbarPos = command_data[i].toolbar_position;
        QString toolTip(command_data[i].tooltip);
        QString statusTip(command_data[i].statustip);
        QString alias_string(command_data[i].alias);
        /* QStringList aliases = alias_string.toStringList(); */

        qDebug("COMMAND: %s", qPrintable(icon));

        if (!command_map.contains(icon)) {
            command_map.insert(icon, command_data[i]);
        }
        else {
            qDebug("ERROR: command_map key collision.");
        }

        Command command = command_data[i];
        QAction *ACTION = createAction(command_data[i]);
        aliasHash->insert(icon, icon);
        actionHash.insert(command_data[i].id, ACTION);

        // load globals: engine->evaluate(script);

        if (toolbarName.toUpper() != "NONE") {
            //If the toolbar doesn't exist, create it.
            if (!toolbarHash.value(toolbarName)) {
                QToolBar* tb = new QToolBar(toolbarName, this);
                tb->setObjectName("toolbar" + toolbarName);
                connect(tb, SIGNAL(topLevelChanged(bool)), this,
                    SLOT(floatingChangedToolBar(bool)));
                addToolBar(Qt::LeftToolBarArea, tb);
                addToolBarBreak(Qt::LeftToolBarArea);
                toolbarHash.insert(toolbarName, tb);
            }

            //TODO: order actions position in toolbar based on .ini setting
            toolbarHash.value(toolbarName)->addAction(ACTION);
        }

        if (menuName.toUpper() != "NONE") {
            //If the menu doesn't exist, create it.
            if (!menuHash.value(menuName)) {
                QMenu* menu = new QMenu(menuName, this);
                menu->setTearOffEnabled(true);
                menuBar()->addMenu(menu);
                menuHash.insert(menuName, menu);
            }

            //TODO: order actions position in menu based on .ini setting
            menuHash.value(menuName)->addAction(ACTION);
        }

        /*
        foreach (QString alias, aliases) {
            prompt->addCommand(alias, cmdName);
        }
        */
    }

    actionHash.value(ACTION_WINDOW_CLOSE)->setEnabled(numOfDocs > 0);
    actionHash.value(ACTION_DESIGN_DETAILS)->setEnabled(numOfDocs > 0);
}

/*
 * TODO: Set What's This Context Help to statusTip for now so there is some infos there.
 *       Make custom whats this context help popup with more descriptive help than just
 *       the status bar/tip one liner(short but not real long) with a hyperlink in the custom popup
 *       at the bottom to open full help file description. Ex: like wxPython AGW's SuperToolTip.
 *
 * TODO: Finish All Commands ... <.<
 */
QAction *
MainWindow::createAction(Command command)
{
    QString icon(command.icon);
    QString toolTip(command.tooltip);
    QString statusTip(command.statustip);
    QString appDir = qApp->applicationDirPath();

    QString iconTheme = getSettingsGeneralIconTheme();
    QAction *ACTION = new QAction(QIcon(appDir + "/icons/" + iconTheme + "/" + icon + ".png"), toolTip, this);
    ACTION->setStatusTip(statusTip);
    ACTION->setObjectName(icon);
    ACTION->setWhatsThis(statusTip);

    if (icon == "donothing") {
        connect(ACTION, SIGNAL(triggered()), this, SLOT(doNothing()));
    }
    else if (icon == "new") {
        ACTION->setShortcut(QKeySequence::New);
        connect(ACTION, SIGNAL(triggered()), this, SLOT(newFile()));
    }
    else if (icon == "open") {
        ACTION->setShortcut(QKeySequence::Open);
        connect(ACTION, SIGNAL(triggered()), this, SLOT(openFile()));
    }
    else if (icon == "save") {
        ACTION->setShortcut(QKeySequence::Save);
        connect(ACTION, SIGNAL(triggered()), this, SLOT(savefile()));
    }
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
        ACTION->setShortcut(QKeySequence("Ctrl+Q"));
        connect(ACTION, SIGNAL(triggered()), this, SLOT(exit()));
    }
    else if (icon == "cut") {
        ACTION->setShortcut(QKeySequence::Cut);
        connect(ACTION, SIGNAL(triggered()), this, SLOT(cut()));
    }
    else if (icon == "copy") {
        ACTION->setShortcut(QKeySequence::Copy);
        connect(ACTION, SIGNAL(triggered()), this, SLOT(copy()));
    }
    else if (icon == "paste") {
        ACTION->setShortcut(QKeySequence::Paste);
        connect(ACTION, SIGNAL(triggered()), this, SLOT(paste()));
    }
    /*
    else if (icon == "windowcascade") {
        connect(ACTION, SIGNAL(triggered()), mdiArea, SLOT(cascade()));
    }
    else if (icon == "windowtile") {
        connect(ACTION, SIGNAL(triggered()), mdiArea, SLOT(tile()));
    }
    else if (icon == "windowclose") {
        ACTION->setShortcut(QKeySequence::Close);
        connect(ACTION, SIGNAL(triggered()), this, SLOT(onCloseWindow()));
    }
    else if (icon == "windowcloseall") {
        connect(ACTION, SIGNAL(triggered()), mdiArea, SLOT(closeAllSubWindows()));
    }
    */
    else if (icon == "windownext") {
        ACTION->setShortcut(QKeySequence::NextChild);
        connect(ACTION, SIGNAL(triggered()), mdiArea, SLOT(activateNextSubWindow()));
    }
    else if (icon == "windowprevious") {
        ACTION->setShortcut(QKeySequence::PreviousChild);
        connect(ACTION, SIGNAL(triggered()), mdiArea, SLOT(activatePreviousSubWindow()));
    }
    /*
    else if (icon == "help") {
        connect(ACTION, SIGNAL(triggered()), this, SLOT(help()));
    }
    else if (icon == "changelog") {
        connect(ACTION, SIGNAL(triggered()), this, SLOT(changelog()));
    }
    else if (icon == "tipoftheday") {
        connect(ACTION, SIGNAL(triggered()), this, SLOT(tipOfTheDay()));
    }
    else if (icon == "whatsthis") {
        connect(ACTION, SIGNAL(triggered()), this, SLOT(whatsThisContextHelp()));
    }
    else if (icon == "settingsdialog") {
        connect(ACTION, SIGNAL(triggered()), this, SLOT(settingsDialog()));
    }
    else if (icon == "undo") {
        connect(ACTION, SIGNAL(triggered()), this, SLOT(undo()));
    }
    else if (icon == "redo")
        connect(ACTION, SIGNAL(triggered()), this, SLOT(redo()));

    else if (icon == "makelayercurrent")
        connect(ACTION, SIGNAL(triggered()), this, SLOT(makeLayerActive()));
    else if (icon == "layers")
        connect(ACTION, SIGNAL(triggered()), this, SLOT(layerManager()));
    else if (icon == "layerprevious")
        connect(ACTION, SIGNAL(triggered()), this, SLOT(layerPrevious()));
    */
    else if (icon == "textbold") {
        ACTION->setCheckable(true);
        connect(ACTION, SIGNAL(toggled(bool)), this, SLOT(setSettingsTextStyleBold(bool)));
    }
    else if (icon == "textitalic") {
        ACTION->setCheckable(true);
        connect(ACTION, SIGNAL(toggled(bool)), this, SLOT(setSettingsTextStyleItalic(bool)));
    }
    else if (icon == "textunderline") {
        ACTION->setCheckable(true);
        connect(ACTION, SIGNAL(toggled(bool)), this, SLOT(setSettingsTextStyleUnderline(bool)));
    }
    else if (icon == "textstrikeout") {
        ACTION->setCheckable(true);
        connect(ACTION, SIGNAL(toggled(bool)), this, SLOT(setSettingsTextStyleStrikeOut(bool)));
    }
    else if (icon == "textoverline") {
        ACTION->setCheckable(true);
        connect(ACTION, SIGNAL(toggled(bool)), this, SLOT(setSettingsTextStyleOverline(bool)));
    }
    /*
    else if (icon == "zoomrealtime")
        connect(ACTION, SIGNAL(triggered()), this, SLOT(zoomRealtime()));
    else if (icon == "zoomprevious")
        connect(ACTION, SIGNAL(triggered()), this, SLOT(zoomPrevious()));
    else if (icon == "zoomwindow")
        connect(ACTION, SIGNAL(triggered()), this, SLOT(zoomWindow()));
    else if (icon == "zoomdynamic")
        connect(ACTION, SIGNAL(triggered()), this, SLOT(zoomDynamic()));
    else if (icon == "zoomscale")
        connect(ACTION, SIGNAL(triggered()), this, SLOT(zoomScale()));
    else if (icon == "zoomcenter")
        connect(ACTION, SIGNAL(triggered()), this, SLOT(zoomCenter()));
    else if (icon == "zoomin")
        connect(ACTION, SIGNAL(triggered()), this, SLOT(zoomIn()));
    else if (icon == "zoomout")
        connect(ACTION, SIGNAL(triggered()), this, SLOT(zoomOut()));
    else if (icon == "zoomselected")
        connect(ACTION, SIGNAL(triggered()), this, SLOT(zoomSelected()));
    else if (icon == "zoomall")
        connect(ACTION, SIGNAL(triggered()), this, SLOT(zoomAll()));
    else if (icon == "zoomextents")
        connect(ACTION, SIGNAL(triggered()), this, SLOT(zoomExtents()));

    else if (icon == "panrealtime")
        connect(ACTION, SIGNAL(triggered()), this, SLOT(panrealtime()));
    else if (icon == "panpoint")
        connect(ACTION, SIGNAL(triggered()), this, SLOT(panpoint()));
    else if (icon == "panleft")
        connect(ACTION, SIGNAL(triggered()), this, SLOT(panLeft()));
    else if (icon == "panright")
        connect(ACTION, SIGNAL(triggered()), this, SLOT(panRight()));
    else if (icon == "panup")
        connect(ACTION, SIGNAL(triggered()), this, SLOT(panUp()));
    else if (icon == "pandown")
        connect(ACTION, SIGNAL(triggered()), this, SLOT(panDown()));
    else if (icon == "night")
        connect(ACTION, SIGNAL(triggered()), this, SLOT(nightVision()));
    */
    else {
        connect(ACTION, SIGNAL(triggered()), this, SLOT(runCommand()));
    }

    return ACTION;
}
