/* Embroidermodder 2: CAD COMMANDS
 *
 * Copyright (C) 2011-2025 The Embroidermodder Team
 * Licensed under the terms of the zlib license.
 *
 * ------------------------------------------------------------------------------------
 *
 * This is the most frequently updated part of the source: new core developers would
 * benefit most from understanding the rough approach of this file and making
 * small changes here.
 *
 * NOTE: the run_command function is the part of the source treated with the most
 * concern for optimization and bug traceability.
 *
 * Core Commands and User Interaction
 * ==================================
 *
 * All user interaction takes place via commands which can be:
 *
 * 1. Core commands defined for internal use that uses the switch table below
 *    for fast calling with logging for debugging purposes and the undo history.
 * 2. Functions written in lua scripts in the `scripts/` folder that call the
 *    wrapper run_command_f to run this switch table.
 * 3. C++ functions designed to take the same arguments called lua registerables
 *    in src/script.cpp: intended the most complex commands. These call run_command
 *    directly using the appropriate constant.
 *
 * For example, when the user issues a "day" command we have 3 internal calls to make
 * which the day command is a wrapper for: change the grid color, change the background
 * color and change the crosshair color. Each of these is issued as a core command,
 * with day being a macro encoding these as a list of core commands.
 *
 * It is likely (FIXME: decide on this) that all core commands will not be modal and
 * only the scripted commands deal with the mode variable.
 */

#include "embroidermodder.h"

int debug = 1;

/* Order matches the CMD_* ID table in "constants.h". This is compiled in since
 * it will only change with changes to the associated constants and there's a
 * potential speed improvements from the compiler if the comparisons are done
 * against static and constant data.
 */
static const char *command_names[] = {
    "null",
    "about",
    "stub",
    "cut",
    "copy",
    "paste",
    "select_all",
    "_END"
};

int
get_cmd_id(const char *cmd)
{
    for (int i=0; command_names[i][0] != '_'; i++) {
        if (!strncmp(command_names[i], cmd, 50)) {
            return i;
        }
    }
    return -1;
}

/*! It is important that there is as little as possible before the switch: this
 * overhead would be present for every core command issued and individual commands
 * may consist of thousands of core command calls.
 *
 * WARNING: note that this does not perform type checking. This is performed
 * on lua style functions, but we either assume that the runCommand* family of functions
 * has vetted the data before it gets to this point or it is the caller's
 * responsibility to get it right within a C++ function.
 */
void
MainWindow::run_command(int id, ...)
{
    if (id > MAX_COMMANDS) {
        qDebug("ERROR: unrecognised command id.");
        return;
    }
    if (debug > 0) {
        qDebug("COMMAND %d (%s)", id, command_names[id]);
    }

    switch (id) {
    /* This action intentionally does nothing. */
    case CMD_NULL:
        break;

    /* Show the about dialog. */
    case CMD_ABOUT:
        about();
        break;

    case CMD_STUB:
        QMessageBox::warning(this, tr("Testing Feature"),
            tr("<b>This feature is in testing.</b>"));
        break;

    case CMD_CUT: {
        View* gview = activeView();
        if (gview) {
            gview->cut();
        }
        break;
    }

    case CMD_COPY: {
        View* gview = activeView();
        if (gview) {
            gview->copy();
        }
        break;
    }

    case CMD_PASTE: {
        View* gview = activeView();
        if (gview) {
            gview->paste();
        }
        break;
    }

    case CMD_SELECT_ALL: {
        View* gview = activeView();
        if (gview) {
            gview->selectAll();
        }
        break;
    }

    default:
        printf("ERROR: unrecognised command id %d\n", id);
        break;
    }
}

MainWindow::MainWindow() : QMainWindow(0)
{
    _mainWin = this;

    readSettings();

    QString appDir = qApp->applicationDirPath();
    //Verify that files/directories needed are actually present.
    QFileInfo check(appDir + "/scripts");
    if (!check.exists())
        QMessageBox::critical(this, tr("Path Error"), tr("Cannot locate: ") + check.absoluteFilePath());
    check = QFileInfo(appDir + "/docs");
    if(!check.exists())
        QMessageBox::critical(this, tr("Path Error"), tr("Cannot locate: ") + check.absoluteFilePath());
    check = QFileInfo(appDir + "/icons");
    if(!check.exists())
        QMessageBox::critical(this, tr("Path Error"), tr("Cannot locate: ") + check.absoluteFilePath());
    check = QFileInfo(appDir + "/images");
    if(!check.exists())
        QMessageBox::critical(this, tr("Path Error"), tr("Cannot locate: ") + check.absoluteFilePath());
    check = QFileInfo(appDir + "/samples");
    if(!check.exists())
        QMessageBox::critical(this, tr("Path Error"), tr("Cannot locate: ") + check.absoluteFilePath());
    check = QFileInfo(appDir + "/translations");
    if(!check.exists())
        QMessageBox::critical(this, tr("Path Error"), tr("Cannot locate: ") + check.absoluteFilePath());

    QString lang = settings.general_language;
    qDebug("language: %s", qPrintable(lang));
    if(lang == "system")
        lang = QLocale::system().languageToString(QLocale::system().language()).toLower();

    //Load translations for the Embroidermodder 2 GUI
    QTranslator translatorEmb;
    translatorEmb.load(appDir + "/translations/" + lang + "/" + lang);
    qApp->installTranslator(&translatorEmb);

    //Load translations provided by Qt - this covers dialog buttons and other common things.
    QTranslator translatorQt;
    translatorQt.load("qt_" + QLocale::system().name(), QLibraryInfo::location(QLibraryInfo::TranslationsPath)); //TODO: ensure this always loads, ship a copy of this with the app
    qApp->installTranslator(&translatorQt);

    //Menus
    fileMenu     = new QMenu(tr("&File"), this);
    editMenu     = new QMenu(tr("&Edit"), this);
    viewMenu     = new QMenu(tr("&View"), this);
    windowMenu   = new QMenu(tr("&Window"), this);
    helpMenu     = new QMenu(tr("&Help"), this);
    //SubMenus
    recentMenu   = new QMenu(tr("Open &Recent"), this);
    zoomMenu     = new QMenu(tr("&Zoom"), this);
    panMenu      = new QMenu(tr("&Pan"), this);

    //Toolbars
    toolbarFile       = addToolBar(tr("File"));
    toolbarEdit       = addToolBar(tr("Edit"));
    toolbarView       = addToolBar(tr("View"));
    toolbarZoom       = addToolBar(tr("Zoom"));
    toolbarPan        = addToolBar(tr("Pan"));
    toolbarIcon       = addToolBar(tr("Icon"));
    toolbarHelp       = addToolBar(tr("Help"));
    toolbarLayer      = addToolBar(tr("Layer"));
    toolbarProperties = addToolBar(tr("Properties"));
    toolbarText       = addToolBar(tr("Text"));
    toolbarPrompt     = addToolBar(tr("Command Prompt"));
    //Selectors
    layerSelector      = new QComboBox(this);
    colorSelector      = new QComboBox(this);
    linetypeSelector   = new QComboBox(this);
    lineweightSelector = new QComboBox(this);
    textFontSelector   = new QFontComboBox(this);
    textSizeSelector   = new QComboBox(this);

    numOfDocs = 0;
    docIndex = 0;

    shiftKeyPressedState = false;

    setWindowIcon(QIcon(appDir + "/icons/" + settings.general_icon_theme + "/" + "app" + ".png"));
    setMinimumSize(800, 480); //Require Minimum WVGA

    loadFormats();

    //create the mdiArea
    QFrame* vbox = new QFrame(this);
    QVBoxLayout* layout = new QVBoxLayout(vbox);
    //layout->setMargin(0);
    vbox->setFrameStyle(QFrame::StyledPanel | QFrame::Sunken);
    mdiArea = new MdiArea(this, vbox);
    mdiArea->useBackgroundLogo(settings.general_mdi_bg_use_logo);
    mdiArea->useBackgroundTexture(settings.general_mdi_bg_use_texture);
    mdiArea->useBackgroundColor(settings.general_mdi_bg_use_color);
    mdiArea->setBackgroundLogo(settings.general_mdi_bg_logo);
    mdiArea->setBackgroundTexture(settings.general_mdi_bg_texture);
    mdiArea->setBackgroundColor(QColor(settings.general_mdi_bg_color));
    mdiArea->setViewMode(QMdiArea::TabbedView);
    mdiArea->setHorizontalScrollBarPolicy(Qt::ScrollBarAsNeeded);
    mdiArea->setVerticalScrollBarPolicy(Qt::ScrollBarAsNeeded);
    mdiArea->setActivationOrder(QMdiArea::ActivationHistoryOrder);
    layout->addWidget(mdiArea);
    setCentralWidget(vbox);

    //create the Command Prompt
    prompt = new CmdPrompt(this);
    prompt->setFocus(Qt::OtherFocusReason);
    this->setFocusProxy(prompt);
    mdiArea->setFocusProxy(prompt);

    prompt->setPromptTextColor(QColor(settings.prompt_text_color));
    prompt->setPromptBackgroundColor(QColor(settings.prompt_bg_color));

    connect(prompt, SIGNAL(startCommand(const QString&)), this, SLOT(logPromptInput(const QString&)));

    connect(prompt, SIGNAL(startCommand(const QString&)), this, SLOT(runCommandMain(const QString&)));
    connect(prompt, SIGNAL(runCommand(const QString&, const QString&)), this, SLOT(runCommandPrompt(const QString&, const QString&)));

            connect(prompt, SIGNAL(deletePressed()),    this, SLOT(deletePressed()));
    //TODO: connect(prompt, SIGNAL(tabPressed()),       this, SLOT(someUnknownSlot()));
            connect(prompt, SIGNAL(escapePressed()),    this, SLOT(escapePressed()));
            connect(prompt, SIGNAL(upPressed()),        this, SLOT(promptInputPrevious()));
            connect(prompt, SIGNAL(downPressed()),      this, SLOT(promptInputNext()));
            connect(prompt, SIGNAL(F1Pressed()),        this, SLOT(help()));
    //TODO: connect(prompt, SIGNAL(F2Pressed()),        this, SLOT(floatHistory()));
    //TODO: connect(prompt, SIGNAL(F3Pressed()),        this, SLOT(toggleQSNAP()));
            connect(prompt, SIGNAL(F4Pressed()),        this, SLOT(toggleLwt())); //TODO: typically this is toggleTablet(), make F-Keys customizable thru settings
    //TODO: connect(prompt, SIGNAL(F5Pressed()),        this, SLOT(toggleISO()));
    //TODO: connect(prompt, SIGNAL(F6Pressed()),        this, SLOT(toggleCoords()));
    connect(prompt, SIGNAL(F7Pressed()),        this, SLOT(toggleGrid()));
    //TODO: connect(prompt, SIGNAL(F8Pressed()),        this, SLOT(toggleORTHO()));
    //TODO: connect(prompt, SIGNAL(F9Pressed()),        this, SLOT(toggleSNAP()));
    //TODO: connect(prompt, SIGNAL(F10Pressed()),       this, SLOT(togglePOLAR()));
    //TODO: connect(prompt, SIGNAL(F11Pressed()),       this, SLOT(toggleQTRACK()));
    connect(prompt, SIGNAL(F12Pressed()),       this, SLOT(toggleRuler()));
    connect(prompt, SIGNAL(cutPressed()),       this, SLOT(cut()));
    connect(prompt, SIGNAL(copyPressed()),      this, SLOT(copy()));
    connect(prompt, SIGNAL(pastePressed()),     this, SLOT(paste()));
    connect(prompt, SIGNAL(selectAllPressed()), this, SLOT(selectAll()));
    connect(prompt, SIGNAL(undoPressed()),      this, SLOT(undo()));
    connect(prompt, SIGNAL(redoPressed()),      this, SLOT(redo()));

    connect(prompt, &CmdPrompt::shiftPressed, this, [=]() { shiftKeyPressedState = true; });
    connect(prompt, &CmdPrompt::shiftReleased, this, [=]() { shiftKeyPressedState = false; });

    connect(prompt, SIGNAL(showSettings()),     this, SLOT(settingsPrompt()));

    connect(prompt, SIGNAL(historyAppended(const QString&)), this, SLOT(promptHistoryAppended(const QString&)));

    //create the Object Property Editor
    dockPropEdit = new PropertyEditor(appDir + "/icons/" + settings.general_icon_theme, settings.selection_mode_pickadd, prompt, this);
    addDockWidget(Qt::LeftDockWidgetArea, dockPropEdit);
    connect(dockPropEdit, SIGNAL(pickAddModeToggled()), this, SLOT(pickAddModeToggled()));

    //create the Command History Undo Editor
    dockUndoEdit = new UndoEditor(appDir + "/icons/" + settings.general_icon_theme, prompt, this);
    addDockWidget(Qt::LeftDockWidgetArea, dockUndoEdit);

    //setDockOptions(QMainWindow::AnimatedDocks | QMainWindow::AllowTabbedDocks | QMainWindow::VerticalTabs); //TODO: Load these from settings
    //tabifyDockWidget(dockPropEdit, dockUndoEdit); //TODO: load this from settings

    // Scripting
    if (!script_env_boot()) {
        return;
    }

    statusbar = new StatusBar(this, this);
    this->setStatusBar(statusbar);

    createAllActions();
    createAllMenus();
    createAllToolbars();

    iconResize(settings.general_icon_size);
    updateMenuToolbarStatusbar();

    //Show date in statusbar after it has been updated
    QDate date = QDate::currentDate();
    QString datestr = date.toString("MMMM d, yyyy");
    statusbar->showMessage(datestr);

    showNormal();

    /* Load tips from external file */
    QFile tipFile(appDir + "/docs/tips.txt");
    if (tipFile.open(QFile::ReadOnly)) {
        QTextStream stream(&tipFile);
        QString tipLine;
        do {
            tipLine = stream.readLine();
            if (!tipLine.isEmpty()) {
                listTipOfTheDay << tipLine;
            }
        } while(!tipLine.isNull());
    }
    if (settings.general_tip_of_the_day) {
        tipOfTheDay();
    }
}

MainWindow::~MainWindow()
{
    qDebug("MainWindow::Destructor()");

    script_env_free();

    //Prevent memory leaks by deleting any unpasted objects
    qDeleteAll(cutCopyObjectList.begin(), cutCopyObjectList.end());
    cutCopyObjectList.clear();
}

void MainWindow::recentMenuAboutToShow()
{
    qDebug("MainWindow::recentMenuAboutToShow()");
    recentMenu->clear();

    QFileInfo recentFileInfo;
    QString recentValue;
    for(int i = 0; i < settings.opensave_recent_list_of_files.size(); ++i) {
        //If less than the max amount of entries add to menu
        if(i < settings.opensave_recent_max_files) {
            recentFileInfo = QFileInfo(settings.opensave_recent_list_of_files.at(i));
            if(recentFileInfo.exists() && validFileFormat(recentFileInfo.fileName())) {
                recentValue.setNum(i+1);
                QAction* rAction;
                if     (recentValue.toInt() >= 1 && recentValue.toInt() <= 9) rAction = new QAction("&" + recentValue + " " + recentFileInfo.fileName(), this);
                else if(recentValue.toInt() == 10)                            rAction = new QAction("1&0 "                  + recentFileInfo.fileName(), this);
                else                                                          rAction = new QAction(      recentValue + " " + recentFileInfo.fileName(), this);
                rAction->setCheckable(false);
                rAction->setData(settings.opensave_recent_list_of_files.at(i));
                recentMenu->addAction(rAction);
                connect(rAction, SIGNAL(triggered()), this, SLOT(openrecentfile()));
            }
        }
    }
    //Ensure the list only has max amount of entries
    while(settings.opensave_recent_list_of_files.size() > settings.opensave_recent_max_files) {
        settings.opensave_recent_list_of_files.removeLast();
    }
}

void MainWindow::windowMenuAboutToShow()
{
    qDebug("MainWindow::windowMenuAboutToShow()");
    windowMenu->clear();
    windowMenu->addAction(actionHash.value("windowclose"));
    windowMenu->addAction(actionHash.value("windowcloseall"));
    windowMenu->addSeparator();
    windowMenu->addAction(actionHash.value("windowcascade"));
    windowMenu->addAction(actionHash.value("windowtile"));
    windowMenu->addSeparator();
    windowMenu->addAction(actionHash.value("windownext"));
    windowMenu->addAction(actionHash.value("windowprevious"));

    windowMenu->addSeparator();
    QList<QMdiSubWindow*> windows = mdiArea->subWindowList();
    for(int i = 0; i < windows.count(); ++i) {
        QAction* aAction = new QAction(windows.at(i)->windowTitle(), this);
        aAction->setCheckable(true);
        aAction->setData(i);
        windowMenu->addAction(aAction);
        connect(aAction, SIGNAL(toggled(bool)), this, SLOT(windowMenuActivated(bool)));
        aAction->setChecked(mdiArea->activeSubWindow() == windows.at(i));
    }
}

void MainWindow::windowMenuActivated(bool checked)
{
    qDebug("MainWindow::windowMenuActivated()");
    QAction* aSender = qobject_cast<QAction*>(sender());
    if(!aSender)
        return;
    QWidget* w = mdiArea->subWindowList().at(aSender->data().toInt());
    if(w && checked)
        w->setFocus();
}

void MainWindow::newFile()
{
    qDebug("MainWindow::newFile()");
    docIndex++;
    numOfDocs++;
    MdiWindow* mdiWin = new MdiWindow(docIndex, _mainWin, mdiArea, Qt::SubWindow);
    connect(mdiWin, SIGNAL(sendCloseMdiWin(MdiWindow*)), this, SLOT(onCloseMdiWin(MdiWindow*)));
    connect(mdiArea, SIGNAL(subWindowActivated(QMdiSubWindow*)), this, SLOT(onWindowActivated(QMdiSubWindow*)));

    updateMenuToolbarStatusbar();
    windowMenuAboutToShow();

    View* v = mdiWin->getView();
    if(v) {
        v->recalculateLimits();
        v->zoomExtents();
    }
}

void MainWindow::openFile(bool recent, const QString& recentFile)
{
    qDebug("MainWindow::openFile()");

    QApplication::setOverrideCursor(Qt::ArrowCursor);

    QStringList files;
    bool preview = settings.opensave_open_thumbnail;
    openFilesPath = settings.opensave_recent_directory;

    //Check to see if this from the recent files list
    if(recent) {
        files.append(recentFile);
        openFilesSelected(files);
    }
    else if(!preview) {
        //TODO: set getOpenFileNames' selectedFilter parameter from settings.opensave_open_format
        files = QFileDialog::getOpenFileNames(this, tr("Open"), openFilesPath, formatFilterOpen);
        openFilesSelected(files);
    }
    else if(preview) {
        PreviewDialog* openDialog = new PreviewDialog(this, tr("Open w/Preview"), openFilesPath, formatFilterOpen);
        //TODO: set openDialog->selectNameFilter(const QString& filter) from settings.opensave_open_format
        connect(openDialog, SIGNAL(filesSelected(const QStringList&)), this, SLOT(openFilesSelected(const QStringList&)));
        openDialog->exec();
    }

    QApplication::restoreOverrideCursor();
}

void MainWindow::openFilesSelected(const QStringList& filesToOpen)
{
    bool doOnce = true;

    if(filesToOpen.count()) {
        for(int i = 0; i < filesToOpen.count(); i++) {
            if(!validFileFormat(filesToOpen[i]))
                continue;

            QMdiSubWindow* existing = findMdiWindow(filesToOpen[i]);
            if(existing) {
                mdiArea->setActiveSubWindow(existing);
                continue;
            }

            //The docIndex doesn't need increased as it is only used for unnamed files
            numOfDocs++;
            MdiWindow* mdiWin = new MdiWindow(docIndex, _mainWin, mdiArea, Qt::SubWindow);
            connect(mdiWin, SIGNAL(sendCloseMdiWin(MdiWindow*)), this, SLOT(onCloseMdiWin(MdiWindow*)));
            connect(mdiArea, SIGNAL(subWindowActivated(QMdiSubWindow*)), this, SLOT(onWindowActivated(QMdiSubWindow*)));

            //Make sure the toolbars/etc... are shown before doing their zoomExtents
            if (doOnce) {
                updateMenuToolbarStatusbar();
                doOnce = false;
            }

            if (mdiWin->loadFile(filesToOpen.at(i))) {
                statusbar->showMessage(tr("File(s) loaded"), 2000);
                mdiWin->show();
                mdiWin->showMaximized();
                //Prevent duplicate entries in the recent files list
                if(!settings.opensave_recent_list_of_files.contains(filesToOpen.at(i), Qt::CaseInsensitive)) {
                    settings.opensave_recent_list_of_files.prepend(filesToOpen.at(i));
                }
                //Move the recent file to the top of the list
                else {
                    settings.opensave_recent_list_of_files.removeAll(filesToOpen.at(i));
                    settings.opensave_recent_list_of_files.prepend(filesToOpen.at(i));
                }
                settings.opensave_recent_directory = QFileInfo(filesToOpen.at(i)).absolutePath();

                View* v = mdiWin->getView();
                if(v) {
                    v->recalculateLimits();
                    v->zoomExtents();
                }
            }
            else {
                mdiWin->close();
            }
        }
    }

    windowMenuAboutToShow();
}

void MainWindow::openrecentfile()
{
    qDebug("MainWindow::openrecentfile()");

    //Check to see if this from the recent files list
    QAction* recentSender = qobject_cast<QAction*>(sender());
    if(recentSender) {
        openFile(true, recentSender->data().toString());
    }
}

void MainWindow::savefile()
{
    qDebug("MainWindow::savefile()");
}

void MainWindow::saveasfile()
{
    qDebug("MainWindow::saveasfile()");
    // need to find the activeSubWindow before it loses focus to the FileDialog
    MdiWindow* mdiWin = qobject_cast<MdiWindow*>(mdiArea->activeSubWindow());
    if(!mdiWin)
        return;

    QString file;
    openFilesPath = settings.opensave_recent_directory;
    file = QFileDialog::getSaveFileName(this, tr("Save As"), openFilesPath, formatFilterSave);

    mdiWin->saveFile(file);
}

QMdiSubWindow* MainWindow::findMdiWindow(const QString& fileName)
{
    qDebug("MainWindow::findMdiWindow(%s)", qPrintable(fileName));
    QString canonicalFilePath = QFileInfo(fileName).canonicalFilePath();

    foreach(QMdiSubWindow* subWindow, mdiArea->subWindowList()) {
        MdiWindow* mdiWin = qobject_cast<MdiWindow*>(subWindow);
        if(mdiWin) {
            if(mdiWin->getCurrentFile() == canonicalFilePath) {
                return subWindow;
            }
        }
    }
    return 0;
}

void MainWindow::closeEvent(QCloseEvent* event)
{
    mdiArea->closeAllSubWindows();
    writeSettings();
    event->accept();
}

void MainWindow::onCloseWindow()
{
    qDebug("MainWindow::onCloseWindow()");
    MdiWindow* mdiWin = qobject_cast<MdiWindow*>(mdiArea->activeSubWindow());
    if(mdiWin) {
        onCloseMdiWin(mdiWin);
    }
}

void MainWindow::onCloseMdiWin(MdiWindow* theMdiWin)
{
    qDebug("MainWindow::onCloseMdiWin()");
    numOfDocs--;

    bool keepMaximized;
    if(theMdiWin) { keepMaximized = theMdiWin->isMaximized(); }

    mdiArea->removeSubWindow(theMdiWin);
    theMdiWin->deleteLater();

    updateMenuToolbarStatusbar();
    windowMenuAboutToShow();

    if(keepMaximized) {
        MdiWindow* mdiWin = qobject_cast<MdiWindow*>(mdiArea->activeSubWindow());
        if(mdiWin) { mdiWin->showMaximized(); }
    }
}

void MainWindow::onWindowActivated(QMdiSubWindow* w)
{
    qDebug("MainWindow::onWindowActivated()");
    MdiWindow* mdiWin = qobject_cast<MdiWindow*>(w);
    if(mdiWin) { mdiWin->onWindowActivated(); }
}

void MainWindow::resizeEvent(QResizeEvent* e)
{
    qDebug("MainWindow::resizeEvent()");
    QMainWindow::resizeEvent(e);
    statusBar()->setSizeGripEnabled(!isMaximized());
}

QAction* MainWindow::getFileSeparator()
{
    qDebug("MainWindow::getFileSeparator()");
    return myFileSeparator;
}

void MainWindow::updateMenuToolbarStatusbar()
{
    qDebug("MainWindow::updateMenuToolbarStatusbar()");

    actionHash.value("print")->setEnabled(numOfDocs > 0);
    actionHash.value("windowclose")->setEnabled(numOfDocs > 0);
    actionHash.value("designdetails")->setEnabled(numOfDocs > 0);

    if(numOfDocs) {
        //Toolbars
        toolbarView->show();
        toolbarZoom->show();
        toolbarPan->show();
        toolbarIcon->show();
        toolbarHelp->show();
        toolbarLayer->show();
        toolbarText->show();
        toolbarProperties->show();
        toolbarPrompt->show();

        foreach(QToolBar* tb, toolbarHash) {
            tb->show();
        }

        //DockWidgets
        dockPropEdit->show();
        dockUndoEdit->show();

        //Menus
        menuBar()->clear();
        menuBar()->addMenu(fileMenu);
        menuBar()->addMenu(editMenu);
        menuBar()->addMenu(viewMenu);

        foreach(QMenu* menu, menuHash) {
            menuBar()->addMenu(menu);
        }

        menuBar()->addMenu(windowMenu);
        menuBar()->addMenu(helpMenu);

        windowMenu->setEnabled(true);

        //Statusbar
        statusbar->clearMessage();
        statusbar->statusBarMouseCoord->show();
        statusbar->statusBarSnapButton->show();
        statusbar->statusBarGridButton->show();
        statusbar->statusBarRulerButton->show();
        statusbar->statusBarOrthoButton->show();
        statusbar->statusBarPolarButton->show();
        statusbar->statusBarQSnapButton->show();
        statusbar->statusBarQTrackButton->show();
        statusbar->statusBarLwtButton->show();
    }
    else {
        //Toolbars
        toolbarView->hide();
        toolbarZoom->hide();
        toolbarPan->hide();
        toolbarIcon->hide();
        toolbarHelp->hide();
        toolbarLayer->hide();
        toolbarText->hide();
        toolbarProperties->hide();
        toolbarPrompt->hide();
        foreach(QToolBar* tb, toolbarHash) {
            tb->hide();
        }

        //DockWidgets
        dockPropEdit->hide();
        dockUndoEdit->hide();

        //Menus
        menuBar()->clear();
        menuBar()->addMenu(fileMenu);
        menuBar()->addMenu(editMenu);
        menuBar()->addMenu(windowMenu);
        menuBar()->addMenu(helpMenu);

        windowMenu->setEnabled(false);

        //Statusbar
        statusbar->clearMessage();
        statusbar->statusBarMouseCoord->hide();
        statusbar->statusBarSnapButton->hide();
        statusbar->statusBarGridButton->hide();
        statusbar->statusBarRulerButton->hide();
        statusbar->statusBarOrthoButton->hide();
        statusbar->statusBarPolarButton->hide();
        statusbar->statusBarQSnapButton->hide();
        statusbar->statusBarQTrackButton->hide();
        statusbar->statusBarLwtButton->hide();
    }
    hideUnimplemented();
}

void MainWindow::hideUnimplemented()
{
    qDebug("MainWindow::hideUnimplemented()");
}

bool MainWindow::validFileFormat(const QString& fileName)
{
    /* FIXME
    if (embFormat_typeFromName(qPrintable(fileName))) {
    */
        return true;
    //}
    return false;
}

void MainWindow::loadFormats()
{
    char stable, unstable;
    QString supportedReaders  = "All Supported Files (";
    QString individualReaders = "All Files (*);;";
    QString supportedWriters  = "All Supported Files (";
    QString individualWriters = "All Files (*);;";
    QString supportedStr;
    QString individualStr;

    //TODO: Stable Only (Settings Option)
    //stable = 'S'; unstable = 'S';

    //Stable + Unstable
    stable = 'S'; unstable = 'U';

    const char* extension = 0;
    const char* description = 0;
    char readerState;
    char writerState;

    /* FIXME:
    EmbFormatList* curFormat = 0;
    EmbFormatList* formatList = embFormatList_create();
    if(!formatList) { QMessageBox::critical(this, tr("Format Loading Error"), tr("Unable to load formats from libembroidery.")); return; }
    curFormat = formatList;
    while(curFormat) {
        extension = embFormat_extension(curFormat);
        description = embFormat_description(curFormat);
        readerState = embFormat_readerState(curFormat);
        writerState = embFormat_writerState(curFormat);

        QString upperExt = QString(extension).toUpper();
        supportedStr = "*" + upperExt + " ";
        individualStr = upperExt.replace(".", "") + " - " + description + " (*" + extension + ");;";
        if(readerState == stable || readerState == unstable) {
            //Exclude color file formats from open dialogs
            if(upperExt != "COL" && upperExt != "EDR" && upperExt != "INF" && upperExt != "RGB") {
                supportedReaders.append(supportedStr);
                individualReaders.append(individualStr);
            }
        }
        if(writerState == stable || writerState == unstable) {
            supportedWriters.append(supportedStr);
            individualWriters.append(individualStr);
        }

        curFormat = curFormat->next;
    }
    embFormatList_free(formatList);
    formatList = 0;

    supportedReaders.append(");;");
    supportedWriters.append(");;");

    formatFilterOpen = supportedReaders + individualReaders;
    formatFilterSave = supportedWriters + individualWriters;

    //TODO: Fixup custom filter
    QString custom = settings.opensave_custom_filter;
    if(custom.contains("supported", Qt::CaseInsensitive))
        custom = ""; //This will hide it
    else if(!custom.contains("*", Qt::CaseInsensitive))
        custom = ""; //This will hide it
    else
        custom = "Custom Filter(" + custom + ");;";

    return tr(qPrintable(custom + supported + all));
    */
}

void MainWindow::closeToolBar(QAction* action)
{
    if(action->objectName() == "toolbarclose") {
        QToolBar* tb = qobject_cast<QToolBar*>(sender());
        if(tb) {
            qDebug("%s closed.", qPrintable(tb->objectName()));
            tb->hide();
        }
    }
}

void MainWindow::floatingChangedToolBar(bool isFloating)
{
    QToolBar* tb = qobject_cast<QToolBar*>(sender());
    if(tb) {
        if(isFloating) {
            /*
            //TODO: Determine best suited close button on various platforms.
            QStyle::SP_DockWidgetCloseButton
            QStyle::SP_TitleBarCloseButton
            QStyle::SP_DialogCloseButton
            */
            QAction *ACTION = new QAction(tb->style()->standardIcon(QStyle::SP_DialogCloseButton), "Close", this);
            ACTION->setStatusTip("Close the " + tb->windowTitle() + " Toolbar");
            ACTION->setObjectName("toolbarclose");
            tb->addAction(ACTION);
            connect(tb, SIGNAL(actionTriggered(QAction*)), this, SLOT(closeToolBar(QAction*)));
        }
        else {
            QList<QAction*> actList = tb->actions();
            for(int i = 0; i < actList.size(); ++i) {
                QAction* ACTION = actList.value(i);
                if(ACTION->objectName() == "toolbarclose") {
                    tb->removeAction(ACTION);
                    disconnect(tb, SIGNAL(actionTriggered(QAction*)), this, SLOT(closeToolBar(QAction*)));
                    delete ACTION;
                }
            }
        }
    }
}

// Do not allow the menus to be torn off. It's a pain in the ass to maintain.
void
MainWindow::addToMenu(QMenu *menu, StringList data)
{
    for (int i=0; i<data.size(); i++) {
        if (data[i] == "addmenu") {
            i += 1;
            continue;
        }
        if (data[i] == "icon") {
            i += 1;
            menu->setIcon(createIcon(data[i].c_str()));
            continue;
        }
        if (data[i] == "---") {
            menu->addSeparator();
            continue;
        }
        menu->addAction(actionHash.value(data[i].c_str()));
    }
    menu->setTearOffEnabled(false);
}

void MainWindow::createAllMenus()
{
    qDebug("MainWindow createAllMenus()");
    menuBar()->addMenu(fileMenu);
    fileMenu->addAction(actionHash.value("new"));
    fileMenu->addSeparator();
    fileMenu->addAction(actionHash.value("open"));

    fileMenu->addMenu(recentMenu);
    connect(recentMenu, SIGNAL(aboutToShow()), this, SLOT(recentMenuAboutToShow()));
    recentMenu->setTearOffEnabled(false);

    menuBar()->addMenu(editMenu);
    menuBar()->addMenu(viewMenu);
    viewMenu->addSeparator();
    viewMenu->addMenu(zoomMenu);
    viewMenu->addMenu(panMenu);
    viewMenu->addSeparator();
    viewMenu->addAction(actionHash.value( "day"));
    viewMenu->addAction(actionHash.value( "night"));
    viewMenu->addSeparator();

    menuBar()->addMenu(windowMenu);
    connect(windowMenu, SIGNAL(aboutToShow()), this, SLOT(windowMenuAboutToShow()));
    //Do not allow the Window Menu to be torn off. It's a pain in the ass to maintain.

    viewMenu->setTearOffEnabled(false);
    windowMenu->setTearOffEnabled(false);
    helpMenu->setTearOffEnabled(false);

    menuBar()->addMenu(helpMenu);

    addToMenu(fileMenu, string_tables["file_menu_list"]);
    addToMenu(editMenu, string_tables["edit_menu_list"]);
    addToMenu(zoomMenu, string_tables["zoom_menu_list"]);
    addToMenu(panMenu, string_tables["pan_menu_list"]);
    addToMenu(helpMenu, string_tables["help_menu_list"]);
}

void
MainWindow::addToToolbar(QToolBar *tb, StringList list)
{
    for (int i=0; i<list.size(); i++) {
        if (list[i] == "---") {
            tb->addSeparator();
        }
        else {
            tb->addAction(actionHash.value(list[i].c_str()));
        }
    }
}

//NOTE: Qt4.7 wont load icons without an extension...
QIcon
MainWindow::createIcon(QString label)
{
    QString appDir = qApp->applicationDirPath();
    QString icontheme = settings.general_icon_theme;
    return QIcon(appDir + "/icons/" + icontheme + "/" + label + ".png");
}

/*
 * TODO: switch prompt to some other focus proxy
 * TODO: some kind of type awareness of data column
 */
void
MainWindow::addToComboBox(QComboBox *box, StringList data)
{
    box->setFocusProxy(prompt);
    for (int i=0; i<data.size()/3; i++) {
        if (data[3*i+2].c_str() != "") {
            box->addItem(createIcon(data[3*i].c_str()),
                data[3*i+1].c_str(),
                atof(data[3*i+2].c_str()));
        }
        else {
            box->addItem(createIcon(data[3*i].c_str()), data[3*i+1].c_str());
        }
    }
}

void
MainWindow::createAllToolbars()
{
    qDebug("MainWindow createAllToolbars()");

    toolbarFile->setObjectName("toolbarFile");
    addToToolbar(toolbarFile, string_tables["file_toolbar_list"]);
    connect(toolbarFile, SIGNAL(topLevelChanged(bool)), this, SLOT(floatingChangedToolBar(bool)));

    toolbarEdit->setObjectName("toolbarEdit");
    addToToolbar(toolbarEdit, string_tables["edit_toolbar_list"]);
    connect(toolbarEdit, SIGNAL(topLevelChanged(bool)), this, SLOT(floatingChangedToolBar(bool)));

    toolbarView->setObjectName("toolbarView");
    addToToolbar(toolbarView, string_tables["view_toolbar_list"]);
    connect(toolbarView, SIGNAL(topLevelChanged(bool)), this, SLOT(floatingChangedToolBar(bool)));

    toolbarZoom->setObjectName("toolbarZoom");
    addToToolbar(toolbarZoom, string_tables["zoom_toolbar_list"]);
    connect(toolbarZoom, SIGNAL(topLevelChanged(bool)), this, SLOT(floatingChangedToolBar(bool)));

    toolbarPan->setObjectName("toolbarPan");
    addToToolbar(toolbarPan, string_tables["pan_toolbar_list"]);
    connect(toolbarPan, SIGNAL(topLevelChanged(bool)), this, SLOT(floatingChangedToolBar(bool)));

    toolbarPan->setObjectName("toolbarIcon");
    addToToolbar(toolbarIcon, string_tables["icon_toolbar_list"]);
    connect(toolbarIcon, SIGNAL(topLevelChanged(bool)), this, SLOT(floatingChangedToolBar(bool)));

    toolbarHelp->setObjectName("toolbarHelp");
    addToToolbar(toolbarHelp, string_tables["help_toolbar_list"]);
    connect(toolbarHelp, SIGNAL(topLevelChanged(bool)), this, SLOT(floatingChangedToolBar(bool)));

    toolbarLayer->setObjectName("toolbarLayer");
    toolbarLayer->addAction(actionHash.value("makelayercurrent"));
    toolbarLayer->addAction(actionHash.value("layers"));

    addToComboBox(layerSelector, string_tables["layer_selector_list"]);
    toolbarLayer->addWidget(layerSelector);
    connect(layerSelector, SIGNAL(currentIndexChanged(int)), this, SLOT(layerSelectorIndexChanged(int)));

    toolbarLayer->addAction(actionHash.value("layerprevious"));
    connect(toolbarLayer, SIGNAL(topLevelChanged(bool)), this, SLOT(floatingChangedToolBar(bool)));

    toolbarProperties->setObjectName("toolbarProperties");
    addToComboBox(colorSelector, string_tables["color_selector_list"]);
    toolbarProperties->addWidget(colorSelector);
    connect(colorSelector, SIGNAL(currentIndexChanged(int)), this, SLOT(colorSelectorIndexChanged(int)));

    toolbarProperties->addSeparator();
    addToComboBox(linetypeSelector, string_tables["linetype_selector_list"]);
    toolbarProperties->addWidget(linetypeSelector);
    connect(linetypeSelector, SIGNAL(currentIndexChanged(int)), this, SLOT(linetypeSelectorIndexChanged(int)));

    toolbarProperties->addSeparator();
    addToComboBox(lineweightSelector, string_tables["lineweight_selector_list"]);
    lineweightSelector->setMinimumContentsLength(8); // Prevent dropdown text readability being squish...d.
    toolbarProperties->addWidget(lineweightSelector);
    connect(lineweightSelector, SIGNAL(currentIndexChanged(int)), this, SLOT(lineweightSelectorIndexChanged(int)));
    connect(toolbarProperties, SIGNAL(topLevelChanged(bool)), this, SLOT(floatingChangedToolBar(bool)));

    toolbarText->setObjectName("toolbarText");
    toolbarText->addWidget(textFontSelector);
    textFontSelector->setCurrentFont(QFont(settings.text_font));
    connect(textFontSelector, SIGNAL(currentFontChanged(const QFont&)), this, SLOT(textFontSelectorCurrentFontChanged(const QFont&)));

    toolbarText->addAction(actionHash.value("textbold"));
    actionHash.value("textbold")->setChecked(settings.text_style_bold);
    toolbarText->addAction(actionHash.value("textitalic"));
    actionHash.value("textitalic")->setChecked(settings.text_style_italic);
    toolbarText->addAction(actionHash.value("textunderline"));
    actionHash.value("textunderline")->setChecked(settings.text_style_underline);
    toolbarText->addAction(actionHash.value("textstrikeout"));
    actionHash.value("textstrikeout")->setChecked(settings.text_style_strikeout);
    toolbarText->addAction(actionHash.value("textoverline"));
    actionHash.value("textoverline")->setChecked(settings.text_style_overline);

    textSizeSelector->setFocusProxy(prompt);
    textSizeSelector->addItem("6 pt",   6);
    textSizeSelector->addItem("8 pt",   8);
    textSizeSelector->addItem("9 pt",   9);
    textSizeSelector->addItem("10 pt", 10);
    textSizeSelector->addItem("11 pt", 11);
    textSizeSelector->addItem("12 pt", 12);
    textSizeSelector->addItem("14 pt", 14);
    textSizeSelector->addItem("18 pt", 18);
    textSizeSelector->addItem("24 pt", 24);
    textSizeSelector->addItem("30 pt", 30);
    textSizeSelector->addItem("36 pt", 36);
    textSizeSelector->addItem("48 pt", 48);
    textSizeSelector->addItem("60 pt", 60);
    textSizeSelector->addItem("72 pt", 72);
    char command[200];
    sprintf(command, "set_text_size(%f)", settings.text_size);
    run(command);
    toolbarText->addWidget(textSizeSelector);
    connect(textSizeSelector, SIGNAL(currentIndexChanged(int)), this, SLOT(textSizeSelectorIndexChanged(int)));

    connect(toolbarText, SIGNAL(topLevelChanged(bool)), this, SLOT(floatingChangedToolBar(bool)));

    toolbarPrompt->setObjectName("toolbarPrompt");
    toolbarPrompt->addWidget(prompt);
    toolbarPrompt->setAllowedAreas(Qt::TopToolBarArea | Qt::BottomToolBarArea);
    connect(toolbarPrompt, SIGNAL(topLevelChanged(bool)), prompt, SLOT(floatingChanged(bool)));

    // Horizontal
    toolbarView->setOrientation(Qt::Horizontal);
    toolbarZoom->setOrientation(Qt::Horizontal);
    toolbarLayer->setOrientation(Qt::Horizontal);
    toolbarProperties->setOrientation(Qt::Horizontal);
    toolbarText->setOrientation(Qt::Horizontal);
    toolbarPrompt->setOrientation(Qt::Horizontal);
    // Top
    addToolBarBreak(Qt::TopToolBarArea);
    addToolBar(Qt::TopToolBarArea, toolbarFile);
    addToolBar(Qt::TopToolBarArea, toolbarEdit);
    addToolBar(Qt::TopToolBarArea, toolbarHelp);
    addToolBar(Qt::TopToolBarArea, toolbarIcon);
    addToolBarBreak(Qt::TopToolBarArea);
    addToolBar(Qt::TopToolBarArea, toolbarZoom);
    addToolBar(Qt::TopToolBarArea, toolbarPan);
    addToolBar(Qt::TopToolBarArea, toolbarView);
    addToolBarBreak(Qt::TopToolBarArea);
    addToolBar(Qt::TopToolBarArea, toolbarLayer);
    addToolBar(Qt::TopToolBarArea, toolbarProperties);
    addToolBarBreak(Qt::TopToolBarArea);
    addToolBar(Qt::TopToolBarArea, toolbarText);
    // Bottom
    addToolBar(Qt::BottomToolBarArea, toolbarPrompt);

    //zoomToolBar->setToolButtonStyle(Qt::ToolButtonTextOnly);
}

