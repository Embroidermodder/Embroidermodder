/*
 * Embroidermodder 2.
 *
 * Copyright 2011-2024 The Embroidermodder Team
 * Embroidermodder 2 is Open Source Software, see LICENSE.md for licensing terms.
 * Visit https://www.libembroidery.org/refman for advice on altering this file,
 * or read the markdown version in embroidermodder2/docs/refman.
 *
 * MainWindow
 */

#include "embroidermodder.h"

QHash<int, QAction*> actionHash;
QHash<QString, QToolBar*> toolbarHash;
QHash<QString, QMenu*> menuHash;
const char *settings_file = "settings.toml";
void create_statusbar(MainWindow* mw);

QStatusBar* statusbar;

void
MainWindow::run_testing(void)
{
    int i;
    std::this_thread::sleep_for (std::chrono::milliseconds(2000));
    for (i=0; strcmp(coverage_test[i], "END"); i++) {
        QString cmd(coverage_test[i]);
        runCommandMain(cmd);
        std::this_thread::sleep_for (std::chrono::milliseconds(1000));
    }        
}

MainWindow::MainWindow() : QMainWindow(0)
{
    if (!load_data()) {
        QMessageBox::critical(this, tr("Path Error"),
            tr("Cannot locate data."));
        return;
    }
    readSettings();

    QString appDir = qApp->applicationDirPath();
    //Verify that files/directories needed are actually present.
    QFileInfo check(appDir + "/help");
    if (!check.exists())
        QMessageBox::critical(this, tr("Path Error"), tr("Cannot locate: ") + check.absoluteFilePath());
    check = QFileInfo(appDir + "/icons");
    if (!check.exists())
        QMessageBox::critical(this, tr("Path Error"), tr("Cannot locate: ") + check.absoluteFilePath());
    check = QFileInfo(appDir + "/images");
    if (!check.exists())
        QMessageBox::critical(this, tr("Path Error"), tr("Cannot locate: ") + check.absoluteFilePath());
    check = QFileInfo(appDir + "/samples");
    if (!check.exists())
        QMessageBox::critical(this, tr("Path Error"), tr("Cannot locate: ") + check.absoluteFilePath());
    check = QFileInfo(appDir + "/translations");
    if (!check.exists())
        QMessageBox::critical(this, tr("Path Error"), tr("Cannot locate: ") + check.absoluteFilePath());

    QString lang = general_language.setting;
    qDebug("language: %s", qPrintable(lang));
    if (lang == "system")
        lang = QLocale::system().languageToString(QLocale::system().language()).toLower();

    /* Load translations for the Embroidermodder 2 GUI. */
    QTranslator translatorEmb;
    translatorEmb.load(appDir + "/translations/" + lang + "/embroidermodder2_" + lang);
    qApp->installTranslator(&translatorEmb);

    /* Load translations for the commands */
    QTranslator translatorCmd;
    translatorCmd.load(appDir + "/translations/" + lang + "/commands_" + lang);
    qApp->installTranslator(&translatorCmd);

    /* Load translations provided by Qt - this covers dialog buttons and other common things. */
    QTranslator translatorQt;
    translatorQt.load("qt_" + QLocale::system().name(), QLibraryInfo::location(QLibraryInfo::TranslationsPath)); /* TODO: ensure this always loads, ship a copy of this with the app. */
    qApp->installTranslator(&translatorQt);

    /* Init */
    _main = this;
    /* Menus */
    menuHash["File"] = new QMenu(tr("&File"), this);
    menuHash["Edit"] = new QMenu(tr("&Edit"), this);
    menuHash["View"] = new QMenu(tr("&View"), this);
    menuHash["Draw"] = new QMenu(tr("&Draw"), this);
    menuHash["Tools"] = new QMenu(tr("&Tools"), this);
    menuHash["Modify"] = new QMenu(tr("&Modify"), this);
    menuHash["Sandbox"] = new QMenu(tr("S&andbox"), this);
    menuHash["Dimension"] = new QMenu(tr("&Dimension"), this);
    menuHash["Settings"] = new QMenu(tr("&Settings"), this);
    menuHash["Window"] = new QMenu(tr("&Window"), this);
    menuHash["Help"] = new QMenu(tr("&Help"), this);
    /* SubMenus */
    menuHash["Recent"] = new QMenu(tr("Open &Recent"), this);
    menuHash["Zoom"] = new QMenu(tr("&Zoom"), this);
    menuHash["Pan"] = new QMenu(tr("&Pan"), this);

    /* Toolbars */
    toolbarHash["File"] = addToolBar(tr("File"));
    toolbarHash["Edit"] = addToolBar(tr("Edit"));
    toolbarHash["View"] = addToolBar(tr("View"));
    toolbarHash["Zoom"] = addToolBar(tr("Zoom"));
    toolbarHash["Pan"] = addToolBar(tr("Pan"));
    toolbarHash["Icon"] = addToolBar(tr("Icon"));
    toolbarHash["Help"] = addToolBar(tr("Help"));
    toolbarHash["Layer"] = addToolBar(tr("Layer"));
    toolbarHash["Properties"] = addToolBar(tr("Properties"));
    toolbarHash["Text"] = addToolBar(tr("Text"));
    toolbarHash["Prompt"] = addToolBar(tr("Command Prompt"));

    toolbarHash["Draw"] = new QToolBar("toolbarDraw", this);
    toolbarHash["Modify"] = new QToolBar("toolbarModify", this);
    toolbarHash["Inquiry"] = new QToolBar("toolbarInquiry", this);
    toolbarHash["Dimension"] = new QToolBar("toolbarDimension", this);
    toolbarHash["Sandbox"] = new QToolBar("toolbarSandbox", this);

    /* Selectors */
    layerSelector = new QComboBox(this);
    colorSelector = new QComboBox(this);
    linetypeSelector = new QComboBox(this);
    lineweightSelector = new QComboBox(this);
    textFontSelector = new QFontComboBox(this);
    textSizeSelector = new QComboBox(this);

    numOfDocs = 0;
    docIndex = 0;

    shiftKeyPressedState = false;

    setWindowIcon(create_icon("app"));
    setMinimumSize(800, 480); /* Require Minimum WVGA */

    loadFormats();

    /* create the mdiArea */
    QFrame* vbox = new QFrame(this);
    QVBoxLayout* layout = new QVBoxLayout(vbox);
    //layout->setMargin(0);
    vbox->setFrameStyle(QFrame::StyledPanel | QFrame::Sunken);
    mdiArea = new MdiArea(this, vbox);
    mdiArea->useBackgroundLogo(general_mdi_bg_use_logo.setting);
    mdiArea->useBackgroundTexture(general_mdi_bg_use_texture.setting);
    mdiArea->useBackgroundColor(general_mdi_bg_use_color.setting);
    mdiArea->setBackgroundLogo(general_mdi_bg_logo.setting);
    mdiArea->setBackgroundTexture(general_mdi_bg_texture.setting);
    mdiArea->setBackgroundColor(QColor(general_mdi_bg_color.setting));
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

    prompt->setPromptTextColor(QColor(prompt_text_color.setting));
    prompt->setPromptBackgroundColor(QColor(prompt_bg_color.setting));

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

            connect(prompt, SIGNAL(shiftPressed()),     this, SLOT(setShiftPressed()));
            connect(prompt, SIGNAL(shiftReleased()),    this, SLOT(setShiftReleased()));

            connect(prompt, SIGNAL(showSettings()),     this, SLOT(settingsPrompt()));

            connect(prompt, SIGNAL(historyAppended(const QString&)), this, SLOT(promptHistoryAppended(const QString&)));

    //create the Object Property Editor
    dockPropEdit = new PropertyEditor(
        appDir + "/icons/" + general_icon_theme.setting,
        selection_mode_pickadd.setting,
        prompt,
        this);
    addDockWidget(Qt::LeftDockWidgetArea, dockPropEdit);
    connect(dockPropEdit, SIGNAL(pickAddModeToggled()), this, SLOT(pickAddModeToggled()));

    //create the Command History Undo Editor
    dockUndoEdit = new UndoEditor(
        appDir + "/icons/" + general_icon_theme.setting, prompt, this);
    addDockWidget(Qt::LeftDockWidgetArea, dockUndoEdit);

    //setDockOptions(QMainWindow::AnimatedDocks | QMainWindow::AllowTabbedDocks | QMainWindow::VerticalTabs); //TODO: Load these from settings
    //tabifyDockWidget(dockPropEdit, dockUndoEdit); //TODO: load this from settings

    create_statusbar(this);
    this->setStatusBar(statusbar);

    createAllActions();
    createAllMenus();
    createAllToolbars();

    iconResize(general_icon_size.setting);
    updateMenuToolbarStatusbar();

    //Show date in statusbar after it has been updated
    QDate date = QDate::currentDate();
    QString datestr = date.toString("MMMM d, yyyy");
    statusbar->showMessage(datestr);

    showNormal();

    toolbarHash["Prompt"]->show();

    //Load tips from external file
    QFile tipFile(appDir + "/tips.txt");
    if (tipFile.open(QFile::ReadOnly)) {
        QTextStream stream(&tipFile);
        QString tipLine;
        do {
            tipLine = stream.readLine();
            if (!tipLine.isEmpty()) {
                listTipOfTheDay << tipLine;
            }
        } while (!tipLine.isNull());
    }

    if (general_tip_of_the_day.setting && (!testing_mode)) {
        tipOfTheDay();
    }

    /* FIXME: automated interface testing
    if (testing_mode) {
        testing_timer = new QTimer();
        connect(testing_timer, &QTimer::timeout, this, run_testing);
        testing_timer->start(100);
    }
    */

    qDebug("Finished creating window.");
}

MainWindow::~MainWindow()
{
    qDebug("MainWindow::Destructor()");

    //Prevent memory leaks by deleting any unpasted objects
    qDeleteAll(cutCopyObjectList.begin(), cutCopyObjectList.end());
    cutCopyObjectList.clear();
}

void MainWindow::recentMenuAboutToShow()
{
    qDebug("MainWindow::recentMenuAboutToShow()");
    menuHash["Recent"]->clear();

    QFileInfo recentFileInfo;
    QString recentValue;
    for (int i = 0; i<MAX_FILES; ++i) {
        if (!strcmp(opensave_recent_list_of_files.setting[i], "END")) {
            break;
        }
        //If less than the max amount of entries add to menu
        if (i < opensave_recent_max_files.setting) {
            recentFileInfo = QFileInfo(opensave_recent_list_of_files.setting[i]);
            if (recentFileInfo.exists() && validFileFormat(recentFileInfo.fileName())) {
                recentValue.setNum(i+1);
                QAction* rAction;
                if (recentValue.toInt() >= 1 && recentValue.toInt() <= 9) {
                    rAction = new QAction("&" + recentValue + " " + recentFileInfo.fileName(), this);
                }
                else if (recentValue.toInt() == 10) {
                    rAction = new QAction("1&0 " + recentFileInfo.fileName(), this);
                }
                else {
                    rAction = new QAction(recentValue + " " + recentFileInfo.fileName(), this);
                }
                rAction->setCheckable(false);
                QString fname(opensave_recent_list_of_files.setting[i]);
                rAction->setData(fname);
                menuHash["Recent"]->addAction(rAction);
                connect(rAction, SIGNAL(triggered()), this, SLOT(openrecentfile()));
            }
        }
        else {
            break;
        }
    }
    /* Ensure the list only has max amount of entries. */
    if (opensave_recent_max_files.setting < MAX_FILES) {
        strcpy(opensave_recent_list_of_files.setting[opensave_recent_max_files.setting], "END");
    }
    else {
        opensave_recent_max_files.setting = MAX_FILES-1;
        strcpy(opensave_recent_list_of_files.setting[opensave_recent_max_files.setting], "END");
    }
}

void
MainWindow::debug_message(const char *msg)
{
    qDebug(msg);
}

void MainWindow::windowMenuAboutToShow()
{
    qDebug("MainWindow::windowMenuAboutToShow()");
    menuHash["Window"]->clear();
    menuHash["Window"]->addAction(actionHash.value(ACTION_WINDOW_CLOSE));
    menuHash["Window"]->addAction(actionHash.value(ACTION_WINDOW_CLOSE_ALL));
    menuHash["Window"]->addSeparator();
    menuHash["Window"]->addAction(actionHash.value(ACTION_WINDOW_CASCADE));
    menuHash["Window"]->addAction(actionHash.value(ACTION_WINDOW_TILE));
    menuHash["Window"]->addSeparator();
    menuHash["Window"]->addAction(actionHash.value(ACTION_WINDOW_NEXT));
    menuHash["Window"]->addAction(actionHash.value(ACTION_WINDOW_PREVIOUS));

    menuHash["Window"]->addSeparator();
    QList<QMdiSubWindow*> windows = mdiArea->subWindowList();
    for (int i = 0; i < windows.count(); ++i) {
        QAction* aAction = new QAction(windows.at(i)->windowTitle(), this);
        aAction->setCheckable(true);
        aAction->setData(i);
        menuHash["Window"]->addAction(aAction);
        connect(aAction, SIGNAL(toggled(bool)), this, SLOT(menuHash["Window"]Activated(bool)));
        aAction->setChecked(mdiArea->activeSubWindow() == windows.at(i));
    }
}

void MainWindow::windowMenuActivated(bool checked)
{
    qDebug("MainWindow::windowMenuActivated()");
    QAction* aSender = qobject_cast<QAction*>(sender());
    if (!aSender)
        return;
    QWidget* w = mdiArea->subWindowList().at(aSender->data().toInt());
    if (w && checked)
        w->setFocus();
}

MdiArea*
getMdiArea()
{
    qDebug("MainWindow::getMdiArea()");
    return _main->mdiArea;
}

void
MainWindow::newFile()
{
    qDebug("MainWindow::newFile()");
    docIndex++;
    numOfDocs++;
    MdiWindow* mdiWin = new MdiWindow(docIndex, _main, mdiArea, Qt::SubWindow);
    connect(mdiWin, SIGNAL(sendCloseMdiWin(MdiWindow*)), this, SLOT(onCloseMdiWin(MdiWindow*)));
    connect(mdiArea, SIGNAL(subWindowActivated(QMdiSubWindow*)), this, SLOT(onWindowActivated(QMdiSubWindow*)));

    updateMenuToolbarStatusbar();
    windowMenuAboutToShow();

    View* v = mdiWin->gview;
    if (v) {
        v->recalculateLimits();
        v->zoomExtents();
    }
}

void
MainWindow::openFile(bool recent, const QString& recentFile)
{
    qDebug("MainWindow::openFile()");

    QApplication::setOverrideCursor(Qt::ArrowCursor);

    QStringList files;
    bool preview = opensave_open_thumbnail.setting;
    openFilesPath = opensave_recent_directory.setting;

    //Check to see if this from the recent files list
    if (recent) {
        files.append(recentFile);
        openFilesSelected(files);
    }
    else if (!preview) {
        /* TODO: set getOpenFileNames' selectedFilter parameter from opensave_open_format.setting */
        files = QFileDialog::getOpenFileNames(this, tr("Open"), openFilesPath, formatFilterOpen);
        openFilesSelected(files);
    }
    else if (preview) {
        PreviewDialog* openDialog = new PreviewDialog(this, tr("Open w/Preview"), openFilesPath, formatFilterOpen);
        /* TODO: set openDialog->selectNameFilter(const QString& filter) from opensave_open_format.setting */
        connect(openDialog, SIGNAL(filesSelected(const QStringList&)), this, SLOT(openFilesSelected(const QStringList&)));
        openDialog->exec();
    }

    QApplication::restoreOverrideCursor();
}

int
string_list_contains(char list[MAX_FILES][MAX_STRING_LENGTH], const char *entry)
{
    for (int i=0; i<MAX_FILES; i++) {
        if (!strcmp(list[i], "END")) {
            break;
        }
        if (!strncmp(list[i], entry, MAX_STRING_LENGTH)) {
            return 1;
        }
    }
    return 0;
}

void
MainWindow::openFilesSelected(const QStringList& filesToOpen)
{
    qDebug("MainWindow::openFileSelected()");
    bool doOnce = true;

    if (filesToOpen.count()) {
        for (int i = 0; i < filesToOpen.count(); i++) {
            qDebug("opening %s...", qPrintable(filesToOpen[i]));

            QMdiSubWindow* existing = findMdiWindow(filesToOpen[i]);
            if (existing) {
                qDebug("File already exists, switching to it.");
                mdiArea->setActiveSubWindow(existing);
                continue;
            }

            //The docIndex doesn't need increased as it is only used for unnamed files
            numOfDocs++;
            MdiWindow* mdiWin = new MdiWindow(docIndex, _main, mdiArea, Qt::SubWindow);
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
                /* Prevent duplicate entries in the recent files list. */
                if (!string_list_contains(opensave_recent_list_of_files.setting,
                    qPrintable(filesToOpen.at(i)))) {
                    for (int j=0; j<MAX_FILES-1; j++) {
                        strcpy(opensave_recent_list_of_files.setting[j],
                            opensave_recent_list_of_files.setting[j+1]);
                    }
                    strcpy(opensave_recent_list_of_files.setting[0],
                        qPrintable(filesToOpen.at(i)));
                }
                /* Move the recent file to the top of the list */
                else {
                    strcpy(opensave_recent_list_of_files.setting[0],
                        qPrintable(filesToOpen.at(i)));
                    strcpy(opensave_recent_list_of_files.setting[1], "END");
                }
                strcpy(opensave_recent_directory.setting,
                    qPrintable(QFileInfo(filesToOpen.at(i)).absolutePath()));

                View* v = mdiWin->gview;
                if (v) {
                    v->recalculateLimits();
                    v->zoomExtents();
                }
            }
            else {
                messageBox("error", "Failed to load file", "Failed to load file.");
                qDebug("Failed to load file.");
                mdiWin->close();
            }
        }
    }

    windowMenuAboutToShow();
}

void
MainWindow::openrecentfile()
{
    qDebug("MainWindow::openrecentfile()");

    //Check to see if this from the recent files list
    QAction* recentSender = qobject_cast<QAction*>(sender());
    if (recentSender) {
        openFile(true, recentSender->data().toString());
    }
}

void
MainWindow::savefile()
{
    qDebug("MainWindow::savefile()");
}

void
MainWindow::saveasfile()
{
    qDebug("MainWindow::saveasfile()");
    // need to find the activeSubWindow before it loses focus to the FileDialog
    MdiWindow* mdiWin = qobject_cast<MdiWindow*>(mdiArea->activeSubWindow());
    if (!mdiWin) {
        return;
    }

    QString file;
    openFilesPath = opensave_recent_directory.setting;
    file = QFileDialog::getSaveFileName(this, tr("Save As"), openFilesPath, formatFilterSave);

    mdiWin->saveFile(file);
}

QMdiSubWindow* MainWindow::findMdiWindow(const QString& fileName)
{
    qDebug("MainWindow::findMdiWindow(%s)", qPrintable(fileName));
    QString canonicalFilePath = QFileInfo(fileName).canonicalFilePath();

    foreach (QMdiSubWindow* subWindow, mdiArea->subWindowList()) {
        MdiWindow* mdiWin = qobject_cast<MdiWindow*>(subWindow);
        if (mdiWin) {
            if (mdiWin->curFile == canonicalFilePath) {
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
    if (mdiWin) {
        onCloseMdiWin(mdiWin);
    }
}

void MainWindow::onCloseMdiWin(MdiWindow* theMdiWin)
{
    qDebug("MainWindow::onCloseMdiWin()");
    numOfDocs--;

    bool keepMaximized;
    if (theMdiWin) {
        keepMaximized = theMdiWin->isMaximized();
    }

    mdiArea->removeSubWindow(theMdiWin);
    theMdiWin->deleteLater();

    updateMenuToolbarStatusbar();
    windowMenuAboutToShow();

    if (keepMaximized) {
        MdiWindow* mdiWin = qobject_cast<MdiWindow*>(mdiArea->activeSubWindow());
        if (mdiWin) {
            mdiWin->showMaximized();
        }
    }
}

void MainWindow::onWindowActivated(QMdiSubWindow* w)
{
    qDebug("MainWindow::onWindowActivated()");
    MdiWindow* mdiWin = qobject_cast<MdiWindow*>(w);
    if (mdiWin) { mdiWin->onWindowActivated(); }
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

    actionHash.value(ACTION_PRINT)->setEnabled(numOfDocs > 0);
    actionHash.value(ACTION_WINDOW_CLOSE)->setEnabled(numOfDocs > 0);
    actionHash.value(ACTION_DESIGN_DETAILS)->setEnabled(numOfDocs > 0);

    if (numOfDocs) {
        /* Toolbars */
        int n = string_array_length(toolbars_when_docs);
        for (int i=0; i<n; i++) {
            QString s(toolbars_when_docs[i]);
            toolbarHash[s]->show();
        }

        /* DockWidgets */
        dockPropEdit->show();
        dockUndoEdit->show();

        /* Menus */
        menuBar()->clear();
        n = string_array_length(menubar_full_list);
        for (int i=0; i<n; i++) {
            QString s(menubar_full_list[i]);
            menuBar()->addMenu(menuHash[s]);
        }
        menuHash["Window"]->setEnabled(true);

        //Statusbar
        statusbar->clearMessage();
        statusBarMouseCoord->show();
        statusBarSnapButton->show();
        statusBarGridButton->show();
        statusBarRulerButton->show();
        statusBarOrthoButton->show();
        statusBarPolarButton->show();
        statusBarQSnapButton->show();
        statusBarQTrackButton->show();
        statusBarLwtButton->show();
    }
    else {
        /* Toolbars */
        int n = string_array_length(toolbars_when_docs);
        for (int i=0; i<n; i++) {
            QString s(toolbars_when_docs[i]);
            toolbarHash[s]->hide();
        }

        /* DockWidgets */
        dockPropEdit->hide();
        dockUndoEdit->hide();

        /* Menus */
        menuBar()->clear();
        n = string_array_length(menubar_no_docs);
        for (int i=0; i<n; i++) {
            QString s(menubar_no_docs[i]);
            menuBar()->addMenu(menuHash[s]);
        }
        menuHash["Window"]->setEnabled(false);

        //Statusbar
        statusbar->clearMessage();
        statusBarMouseCoord->hide();
        statusBarSnapButton->hide();
        statusBarGridButton->hide();
        statusBarRulerButton->hide();
        statusBarOrthoButton->hide();
        statusBarPolarButton->hide();
        statusBarQSnapButton->hide();
        statusBarQTrackButton->hide();
        statusBarLwtButton->hide();
    }
    hideUnimplemented();
}

void MainWindow::hideUnimplemented()
{
    qDebug("MainWindow::hideUnimplemented()");
}

bool MainWindow::validFileFormat(const QString& fileName)
{
    if (emb_identify_format(qPrintable(fileName)) >= 0) {
        return true;
    }
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


    for (int i=0; i<numberOfFormats; i++) {
        const char* extension = formatTable[i].extension;
        const char* description = formatTable[i].description;
        char readerState = formatTable[i].reader_state;
        char writerState = formatTable[i].writer_state;

        QString upperExt = QString(extension).toUpper();
        supportedStr = "*" + upperExt + " ";
        individualStr = upperExt.replace(".", "") + " - " + description + " (*" + extension + ");;";
        if (readerState == stable || readerState == unstable) {
            //Exclude color file formats from open dialogs
            if (upperExt != "COL" && upperExt != "EDR" && upperExt != "INF" && upperExt != "RGB") {
                supportedReaders.append(supportedStr);
                individualReaders.append(individualStr);
            }
        }
        if (writerState == stable || writerState == unstable) {
            supportedWriters.append(supportedStr);
            individualWriters.append(individualStr);
        }
    }

    supportedReaders.append(");;");
    supportedWriters.append(");;");

    formatFilterOpen = supportedReaders + individualReaders;
    formatFilterSave = supportedWriters + individualWriters;

    //TODO: Fixup custom filter
    /*
    QString custom = custom_filter.setting;
    if (custom.contains("supported", Qt::CaseInsensitive))
        custom = ""; //This will hide it
    else if (!custom.contains("*", Qt::CaseInsensitive))
        custom = ""; //This will hide it
    else
        custom = "Custom Filter(" + custom + ");;";

    return tr(qPrintable(custom + supported + all));
    */
}

void
MainWindow::closeToolBar(QAction* action)
{
    if (action->objectName() == "toolbarclose") {
        QToolBar* tb = qobject_cast<QToolBar*>(sender());
        if (tb) {
            qDebug("%s closed.", qPrintable(tb->objectName()));
            tb->hide();
        }
    }
}

void MainWindow::floatingChangedToolBar(bool isFloating)
{
    QToolBar* tb = qobject_cast<QToolBar*>(sender());
    if (tb) {
        if (isFloating) {
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
            for (int i = 0; i < actList.size(); ++i) {
                QAction* ACTION = actList.value(i);
                if (ACTION->objectName() == "toolbarclose") {
                    tb->removeAction(ACTION);
                    disconnect(tb, SIGNAL(actionTriggered(QAction*)), this, SLOT(closeToolBar(QAction*)));
                    delete ACTION;
                }
            }
        }
    }
}

/* */
QAction*
get_action_by_icon(const char *icon)
{
    int i;
    for (i=0; command_data[i].id != -2; i++) {
        if (!strcmp(command_data[i].icon, icon)) {
            return actionHash.value(command_data[i].id);
        }
    }
    return actionHash.value(ACTION_DO_NOTHING);
}

/* . */
void
add_to_menu(QMenu *menu, string_table menu_data)
{
    for (int i=0; strcmp(menu_data[i], "END"); i++) {
        if (menu_data[i][0] == '-') {
            menu->addSeparator();
        }
        else if (menu_data[i][0] == '>') {
            menu->addMenu(menuHash[menu_data[i]+1]);
        }
        else if (menu_data[i][0] == '+') {
            QString appDir = qApp->applicationDirPath();
            QString icontheme = general_icon_theme.setting;
            QString s = appDir + "/icons/" + icontheme + "/" + QString(menu_data[i]+1) + ".png";
            menu->setIcon(QIcon(s));
        }
        else {
            menu->addAction(get_action_by_icon(menu_data[i]));
        }
    }
    menu->setTearOffEnabled(false);
}

/* . */
void
MainWindow::createAllMenus()
{
    qDebug("MainWindow createAllMenus()");

    QString appDir = qApp->applicationDirPath();
    QString icontheme = general_icon_theme.setting;

    menuBar()->addMenu(menuHash["File"]);
    menuHash["File"]->addAction(actionHash.value(ACTION_NEW));
    menuHash["File"]->addSeparator();
    menuHash["File"]->addAction(actionHash.value(ACTION_OPEN));

    menuHash["File"]->addMenu(menuHash["Recent"]);
    connect(menuHash["Recent"], SIGNAL(aboutToShow()), this, SLOT(recentMenuAboutToShow()));
    //Do not allow the Recent Menu to be torn off. It's a pain in the ass to maintain.
    menuHash["Recent"]->setTearOffEnabled(false);

    menuHash["File"]->addSeparator();
    menuHash["File"]->addAction(actionHash.value(ACTION_SAVE));
    menuHash["File"]->addAction(actionHash.value(ACTION_SAVE_AS));
    menuHash["File"]->addSeparator();
    menuHash["File"]->addAction(actionHash.value(ACTION_PRINT));
    menuHash["File"]->addSeparator();
    menuHash["File"]->addAction(actionHash.value(ACTION_WINDOW_CLOSE));
    menuHash["File"]->addSeparator();
    menuHash["File"]->addAction(actionHash.value(ACTION_DESIGN_DETAILS));
    menuHash["File"]->addSeparator();

    menuHash["File"]->addAction(actionHash.value(ACTION_EXIT));
    menuHash["File"]->setTearOffEnabled(false);

    menuBar()->addMenu(menuHash["Edit"]);
    add_to_menu(menuHash["Edit"], edit_menu);

    menuBar()->addMenu(menuHash["View"]);
    add_to_menu(menuHash["View"], view_menu);
    add_to_menu(menuHash["Zoom"], zoom_menu);
    add_to_menu(menuHash["Pan"], pan_menu);

    menuBar()->addMenu(menuHash["Tools"]);
    add_to_menu(menuHash["Tools"], tools_menu);

    menuBar()->addMenu(menuHash["Draw"]);
    add_to_menu(menuHash["Draw"], draw_menu);

    menuBar()->addMenu(menuHash["Dimension"]);
    add_to_menu(menuHash["Dimension"], dimension_menu);

    menuBar()->addMenu(menuHash["Modify"]);
    add_to_menu(menuHash["Modify"], modify_menu);

    menuBar()->addMenu(menuHash["Sandbox"]);
    add_to_menu(menuHash["Sandbox"], sandbox_menu);

    menuBar()->addMenu(menuHash["Settings"]);
    menuHash["Settings"]->addAction(actionHash.value(ACTION_SETTINGS_DIALOG));
    menuHash["Settings"]->addSeparator();
    menuHash["Settings"]->setTearOffEnabled(false);

    menuBar()->addMenu(menuHash["Window"]);
    connect(menuHash["Window"], SIGNAL(aboutToShow()), this, SLOT(windowMenuAboutToShow()));
    /* Do not allow the Window Menu to be torn off. It's a pain in the ass to maintain. */
    menuHash["Window"]->setTearOffEnabled(false);

    menuBar()->addMenu(menuHash["Help"]);
    add_to_menu(menuHash["Help"], help_menu);
}

/* Note: on Unix we include the trailing separator. For Windows compatibility we
 * omit it.
 */
QString
SettingsDir()
{
#if defined(Q_OS_UNIX) || defined(Q_OS_MAC)
    return QDir::homePath() + "/.embroidermodder2/";
#else
    return "";
#endif
}

void
get_setting(QSettings *settings, const char *key, const char *value, StringSetting *s)
{
    QString k(key);
    QString v(value);
    strcpy(s->key, key);
    strcpy(s->setting, qPrintable(settings->value(k, v).toString()));
}

void
get_setting(QSettings *settings, const char *key, int value, IntSetting *i)
{
    strcpy(i->key, key);
    i->setting = settings->value(key, value).toInt();
}

void
get_setting(QSettings *settings, const char *key, bool value, BoolSetting *b)
{
    strcpy(b->key, key);
    b->setting = settings->value(key, value).toBool();
}

void
MainWindow::readSettings()
{
    qDebug("Reading Settings...");
    // This file needs to be read from the users home directory to ensure it is writable
    QString settingsDir = SettingsDir();
    QString appDir = qApp->applicationDirPath();
    //load_settings(qPrintable(appDir), qPrintable(SettingsDir()));

    QSettings settings(SettingsDir() + settings_file, QSettings::IniFormat);
    QPoint pos = settings.value("Window/Position", QPoint(0, 0)).toPoint();
    QSize size = settings.value("Window/Size", QSize(800, 600)).toSize();

    layoutState = settings.value("LayoutState").toByteArray();
    if (!restoreState(layoutState)) {
        qDebug("LayoutState NOT restored! Setting Default Layout...");
        //someToolBar->setVisible(true);
    }

    /* General */
    get_setting(&settings, "Language", "default", &general_language);
    get_setting(&settings, "IconTheme", "default", &general_icon_theme);
    get_setting(&settings, "IconSize", 16, &general_icon_size);
    get_setting(&settings, "MdiBGUseLogo", true, &general_mdi_bg_use_logo);
    get_setting(&settings, "MdiBGUseTexture", true, &general_mdi_bg_use_texture);
    get_setting(&settings, "MdiBGUseColor", true, &general_mdi_bg_use_color);
    get_setting(&settings, "MdiBGLogo", qPrintable(appDir + "/images/logo-spirals.png"), &general_mdi_bg_logo);
    get_setting(&settings, "MdiBGTexture", qPrintable(appDir + "/images/texture-spirals.png"), &general_mdi_bg_texture);
    get_setting(&settings, "MdiBGColor", qRgb(192,192,192), &general_mdi_bg_color);
    get_setting(&settings, "TipOfTheDay", true, &general_tip_of_the_day);
    get_setting(&settings, "CurrentTip", 0, &general_current_tip);
    get_setting(&settings, "SystemHelpBrowser", true, &general_system_help_browser);

    /* Display */
    get_setting(&settings, "Display/UseOpenGL", false, &display_use_opengl);
    get_setting(&settings, "Display/RenderHintAntiAlias", false, &display_renderhint_aa);
    display_renderhint_text_aa.setting = settings.value("Display/RenderHintTextAntiAlias", false).toBool();
    display_renderhint_smooth_pix.setting = settings.value("Display/RenderHintSmoothPixmap", false).toBool();
    display_renderhint_high_aa.setting = settings.value("Display/RenderHintHighQualityAntiAlias", false).toBool();
    display_renderhint_noncosmetic.setting = settings.value("Display/RenderHintNonCosmetic", false).toBool();
    display_show_scrollbars.setting = settings.value("Display/ShowScrollBars", true).toBool();
    display_scrollbar_widget_num.setting = settings.value("Display/ScrollBarWidgetNum", 0).toInt();
    display_crosshair_color.setting = settings.value("Display/CrossHairColor", qRgb(  0, 0, 0)).toInt();
    display_bg_color.setting = settings.value("Display/BackgroundColor", qRgb(235,235,235)).toInt();
    display_selectbox_left_color.setting = settings.value("Display/SelectBoxLeftColor", qRgb(  0,128, 0)).toInt();
    display_selectbox_left_fill.setting = settings.value("Display/SelectBoxLeftFill", qRgb(  0,255, 0)).toInt();
    display_selectbox_right_color.setting = settings.value("Display/SelectBoxRightColor", qRgb(  0, 0,128)).toInt();
    display_selectbox_right_fill.setting = settings.value("Display/SelectBoxRightFill", qRgb(  0, 0,255)).toInt();
    display_selectbox_alpha.setting = settings.value("Display/SelectBoxAlpha", 32).toInt();
    display_zoomscale_in.setting = settings.value("Display/ZoomScaleIn", 2.0).toFloat();
    display_zoomscale_out.setting = settings.value("Display/ZoomScaleOut", 0.5).toFloat();
    display_crosshair_percent.setting = settings.value("Display/CrossHairPercent", 5).toInt();
    strcpy(display_units.setting, qPrintable(settings.value("Display/Units", "mm").toString()));

    /* Prompt */
    prompt_text_color.setting = settings.value("Prompt/TextColor", qRgb(0, 0, 0)).toInt();
    prompt_bg_color.setting = settings.value("Prompt/BackgroundColor", qRgb(255, 255, 255)).toInt();
    strcpy(prompt_font_family.setting,
        qPrintable(settings.value("Prompt/FontFamily", "Monospace").toString()));
    strcpy(prompt_font_style.setting,
        qPrintable(settings.value("Prompt/FontStyle", "normal").toString()));
    prompt_font_size.setting = settings.value("Prompt/FontSize", 12).toInt();
    prompt_save_history.setting = settings.value("Prompt/SaveHistory", true).toBool();
    prompt_save_history_as_html.setting = settings.value("Prompt/SaveHistoryAsHtml", false).toBool();
    strcpy(prompt_save_history_filename.setting,
        qPrintable(settings.value("Prompt/SaveHistoryFilename", settingsDir + "prompt.log").toString()));

    /* OpenSave */
    strcpy(opensave_custom_filter.setting,
        qPrintable(settings.value("OpenSave/CustomFilter", "supported").toString()));
    strcpy(opensave_open_format.setting,
        qPrintable(settings.value("OpenSave/OpenFormat", "*.*").toString()));
    opensave_open_thumbnail.setting = settings.value("OpenSave/OpenThumbnail", false).toBool();
    strcpy(opensave_save_format.setting,
        qPrintable(settings.value("OpenSave/SaveFormat", "*.*").toString()));
    opensave_save_thumbnail.setting = settings.value("OpenSave/SaveThumbnail", false).toBool();

    /* Recent */
    opensave_recent_max_files.setting = settings.value("OpenSave/RecentMax", 10).toInt();
    QStringList sl = settings.value("OpenSave/RecentFiles").toStringList();
    /*
    int i;
    for (i=0; i<MAX_FILES-1; i++) {
        strncpy(opensave_recent_list_of_files.setting[i], qPrintable(sl[i]),
            MAX_STRING_LENGTH);
    }
    strncpy(opensave_recent_list_of_files.setting[i], "END", MAX_STRING_LENGTH);
    strncpy(opensave_recent_directory.setting,
        qPrintable(settings.value("OpenSave/RecentDirectory", appDir + "/samples").toString()),
        MAX_STRING_LENGTH);
    */
    strncpy(opensave_recent_list_of_files.setting[0], "END", MAX_STRING_LENGTH);

    /* Trimming */
    opensave_trim_dst_num_jumps.setting = settings.value("OpenSave/TrimDstNumJumps", 5).toInt();

    /* Printing */
    strcpy(printing_default_device.setting, qPrintable(settings.value("Printing/DefaultDevice", "").toString()));
    printing_use_last_device.setting = settings.value("Printing/UseLastDevice", false).toBool();
    printing_disable_bg.setting = settings.value("Printing/DisableBG", true).toBool();

    /* Grid */
    grid_show_on_load.setting = settings.value("Grid/ShowOnLoad", true).toBool();
    grid_show_origin.setting = settings.value("Grid/ShowOrigin", true).toBool();
    grid_color_match_crosshair.setting = settings.value("Grid/ColorMatchCrossHair", true).toBool();
    grid_color.setting = settings.value("Grid/Color", qRgb(  0, 0, 0)).toInt();
    grid_load_from_file.setting = settings.value("Grid/LoadFromFile", true).toBool();
    strcpy(grid_type.setting, qPrintable(settings.value("Grid/Type", "Rectangular").toString()));
    grid_center_on_origin.setting = settings.value("Grid/CenterOnOrigin", true).toBool();
    grid_center_x.setting = settings.value("Grid/CenterX", 0.0).toFloat();
    grid_center_y.setting = settings.value("Grid/CenterY", 0.0).toFloat();
    grid_size_x.setting = settings.value("Grid/SizeX", 100.0).toFloat();
    grid_size_y.setting = settings.value("Grid/SizeY", 100.0).toFloat();
    grid_spacing_x.setting = settings.value("Grid/SpacingX", 25.0).toFloat();
    grid_spacing_y.setting = settings.value("Grid/SpacingY", 25.0).toFloat();
    grid_size_radius.setting = settings.value("Grid/SizeRadius", 50.0).toFloat();
    grid_spacing_radius.setting = settings.value("Grid/SpacingRadius", 25.0).toFloat();
    grid_spacing_angle.setting = settings.value("Grid/SpacingAngle", 45.0).toFloat();

    /* Ruler */
    ruler_show_on_load.setting = settings.value("Ruler/ShowOnLoad", true).toBool();
    ruler_metric.setting = settings.value("Ruler/Metric", true).toBool();
    ruler_color.setting = settings.value("Ruler/Color", qRgb(210,210, 50)).toInt();
    ruler_pixel_size.setting = settings.value("Ruler/PixelSize", 20).toInt();

    /* Quick Snap */
    qsnap_enabled.setting = settings.value("QuickSnap/Enabled", true).toBool();
    qsnap_locator_color.setting = settings.value("QuickSnap/LocatorColor", qRgb(255,255, 0)).toInt();
    qsnap_locator_size.setting = settings.value("QuickSnap/LocatorSize", 4).toInt();
    qsnap_aperture_size.setting = settings.value("QuickSnap/ApertureSize", 10).toInt();
    qsnap_endpoint.setting = settings.value("QuickSnap/EndPoint", true).toBool();
    qsnap_midpoint.setting = settings.value("QuickSnap/MidPoint", true).toBool();
    qsnap_center.setting = settings.value("QuickSnap/Center", true).toBool();
    qsnap_node.setting = settings.value("QuickSnap/Node", true).toBool();
    qsnap_quadrant.setting = settings.value("QuickSnap/Quadrant", true).toBool();
    qsnap_intersection.setting = settings.value("QuickSnap/Intersection", true).toBool();
    qsnap_extension.setting = settings.value("QuickSnap/Extension", true).toBool();
    qsnap_insertion.setting = settings.value("QuickSnap/Insertion", false).toBool();
    qsnap_perpendicular.setting = settings.value("QuickSnap/Perpendicular", true).toBool();
    qsnap_tangent.setting = settings.value("QuickSnap/Tangent", true).toBool();
    qsnap_nearest.setting = settings.value("QuickSnap/Nearest", false).toBool();
    qsnap_apparent.setting = settings.value("QuickSnap/Apparent", false).toBool();
    qsnap_parallel.setting = settings.value("QuickSnap/Parallel", false).toBool();

    /* LineWeight */
    lwt_show_lwt.setting = settings.value("LineWeight/ShowLineWeight", false).toBool();
    lwt_real_render.setting = settings.value("LineWeight/RealRender", true).toBool();
    lwt_default_lwt.setting = settings.value("LineWeight/DefaultLineWeight", 0).toReal();

    /* Selection */
    selection_mode_pickfirst.setting = settings.value("Selection/PickFirst", true).toBool();
    selection_mode_pickadd.setting = settings.value("Selection/PickAdd", true).toBool();
    selection_mode_pickdrag.setting = settings.value("Selection/PickDrag", false).toBool();
    selection_coolgrip_color.setting = settings.value("Selection/CoolGripColor", qRgb(  0, 0,255)).toInt();
    selection_hotgrip_color.setting = settings.value("Selection/HotGripColor", qRgb(255, 0, 0)).toInt();
    selection_grip_size.setting = settings.value("Selection/GripSize", 4).toInt();
    selection_pickbox_size.setting = settings.value("Selection/PickBoxSize", 4).toInt();

    /* Text */
    strcpy(text_font.setting, qPrintable(settings.value("Text/Font", "Arial").toString()));
    text_size.setting = settings.value("Text/Size", 12).toReal();
    text_angle.setting = settings.value("Text/Angle", 0).toReal();
    text_style_bold.setting = settings.value("Text/StyleBold", false).toBool();
    text_style_italic.setting = settings.value("Text/StyleItalic", false).toBool();
    text_style_underline.setting = settings.value("Text/StyleUnderline", false).toBool();
    text_style_strikeout.setting = settings.value("Text/StyleStrikeOut", false).toBool();
    text_style_overline.setting = settings.value("Text/StyleOverline", false).toBool();

    move(pos);
    resize(size);
}

void
MainWindow::writeSettings()
{
    qDebug("Writing Settings...");
    save_settings("", qPrintable(SettingsDir() + settings_file));
}

void
MainWindow::settingsPrompt()
{
    settingsDialog("Prompt");
}

void
MainWindow::settingsDialog(const QString& showTab)
{
    Settings_Dialog dialog(this, showTab, this);
    dialog.exec();
}
/*
 * Embroidermodder 2.
 *
 * Copyright 2011-2024 The Embroidermodder Team
 * Embroidermodder 2 is Open Source Software, see LICENSE.md for licensing terms.
 * Visit https://www.libembroidery.org/refman for advice on altering this file,
 * or read the markdown version in embroidermodder2/docs/refman.
 *
 * Toolbars
 */

#include "embroidermodder.h"

void
add_to_toolbar(const char *toolbar_name, string_table toolbar_data)
{
    QString s(toolbar_name);
    toolbarHash[s]->setObjectName("toolbar" + s);

    int n = string_array_length(toolbar_data);
    for (int i=0; i<n; i++) {
        if (toolbar_data[i][0] == '-') {
            toolbarHash[s]->addSeparator();
        }
        else {
            QAction *action = get_action_by_icon(toolbar_data[i]);
            toolbarHash[s]->addAction(action);
        }
    }

    QAction::connect(toolbarHash[s], SIGNAL(topLevelChanged(bool)), _main,
        SLOT(floatingChangedToolBar(bool)));
}

void MainWindow::createLayerToolbar()
{
    qDebug("MainWindow createLayerToolbar()");

    toolbarHash["Layer"]->setObjectName("toolbarLayer");
    toolbarHash["Layer"]->addAction(actionHash.value(ACTION_MAKE_LAYER_CURRENT));
    toolbarHash["Layer"]->addAction(actionHash.value(ACTION_LAYERS));

    QString appDir = qApp->applicationDirPath();
    QString icontheme = general_icon_theme.setting;

    layerSelector->setFocusProxy(prompt);
    //NOTE: Qt4.7 wont load icons without an extension...
    //TODO: Create layer pixmaps by concatenating several icons
    layerSelector->addItem(create_icon("linetypebylayer"), "0");
    layerSelector->addItem(create_icon("linetypebylayer"), "1");
    layerSelector->addItem(create_icon("linetypebylayer"), "2");
    layerSelector->addItem(create_icon("linetypebylayer"), "3");
    layerSelector->addItem(create_icon("linetypebylayer"), "4");
    layerSelector->addItem(create_icon("linetypebylayer"), "5");
    layerSelector->addItem(create_icon("linetypebylayer"), "6");
    layerSelector->addItem(create_icon("linetypebylayer"), "7");
    layerSelector->addItem(create_icon("linetypebylayer"), "8");
    layerSelector->addItem(create_icon("linetypebylayer"), "9");
    toolbarHash["Layer"]->addWidget(layerSelector);
    connect(layerSelector, SIGNAL(currentIndexChanged(int)), this, SLOT(layerSelectorIndexChanged(int)));

    toolbarHash["Layer"]->addAction(actionHash.value(ACTION_LAYER_PREVIOUS));

    connect(toolbarHash["Layer"], SIGNAL(topLevelChanged(bool)), this, SLOT(floatingChangedToolBar(bool)));
}

void MainWindow::createPropertiesToolbar()
{
    qDebug("MainWindow createPropertiesToolbar()");

    toolbarHash["Properties"]->setObjectName("toolbarProperties");

    colorSelector->setFocusProxy(prompt);
    //NOTE: Qt4.7 wont load icons without an extension...
    colorSelector->addItem(create_icon("colorbylayer"), "ByLayer");
    colorSelector->addItem(create_icon("colorbyblock"), "ByBlock");
    colorSelector->addItem(create_icon("colorred"), tr("Red"), qRgb(255, 0, 0));
    colorSelector->addItem(create_icon("coloryellow"), tr("Yellow"), qRgb(255, 255, 0));
    colorSelector->addItem(create_icon("colorgreen"), tr("Green"),   qRgb(  0,255,  0));
    colorSelector->addItem(create_icon("colorcyan"), tr("Cyan"),    qRgb(  0,255,255));
    colorSelector->addItem(create_icon("colorblue"), tr("Blue"),    qRgb(  0,  0,255));
    colorSelector->addItem(create_icon("colormagenta"), tr("Magenta"), qRgb(255,  0,255));
    colorSelector->addItem(create_icon("colorwhite"), tr("White"),   qRgb(255,255,255));
    colorSelector->addItem(create_icon("colorother"), tr("Other..."));
    toolbarHash["Properties"]->addWidget(colorSelector);
    connect(colorSelector, SIGNAL(currentIndexChanged(int)), this, SLOT(colorSelectorIndexChanged(int)));

    toolbarHash["Properties"]->addSeparator();
    linetypeSelector->setFocusProxy(prompt);
    //NOTE: Qt4.7 wont load icons without an extension...
    linetypeSelector->addItem(create_icon("linetypebylayer"   ), "ByLayer");
    linetypeSelector->addItem(create_icon("linetypebyblock"   ), "ByBlock");
    linetypeSelector->addItem(create_icon("linetypecontinuous"), "Continuous");
    linetypeSelector->addItem(create_icon("linetypehidden"    ), "Hidden");
    linetypeSelector->addItem(create_icon("linetypecenter"    ), "Center");
    linetypeSelector->addItem(create_icon("linetypeother"     ), "Other...");
    toolbarHash["Properties"]->addWidget(linetypeSelector);
    connect(linetypeSelector, SIGNAL(currentIndexChanged(int)), this, SLOT(linetypeSelectorIndexChanged(int)));

    toolbarHash["Properties"]->addSeparator();
    lineweightSelector->setFocusProxy(prompt);
    //NOTE: Qt4.7 wont load icons without an extension...
    lineweightSelector->addItem(create_icon("lineweightbylayer"), "ByLayer", -2.00);
    lineweightSelector->addItem(create_icon("lineweightbyblock"), "ByBlock", -1.00);
    lineweightSelector->addItem(create_icon("lineweightdefault"), "Default",  0.00);
    //TODO: Thread weight is weird. See http://en.wikipedia.org/wiki/Thread_(yarn)#Weight
    lineweightSelector->addItem(create_icon("lineweight01"), "0.00 mm", 0.00);
    lineweightSelector->addItem(create_icon("lineweight02"), "0.05 mm", 0.05);
    lineweightSelector->addItem(create_icon("lineweight03"), "0.15 mm", 0.15);
    lineweightSelector->addItem(create_icon("lineweight04"), "0.20 mm", 0.20);
    lineweightSelector->addItem(create_icon("lineweight05"), "0.25 mm", 0.25);
    lineweightSelector->addItem(create_icon("lineweight06"), "0.30 mm", 0.30);
    lineweightSelector->addItem(create_icon("lineweight07"), "0.35 mm", 0.35);
    lineweightSelector->addItem(create_icon("lineweight08"), "0.40 mm", 0.40);
    lineweightSelector->addItem(create_icon("lineweight09"), "0.45 mm", 0.45);
    lineweightSelector->addItem(create_icon("lineweight10"), "0.50 mm", 0.50);
    lineweightSelector->addItem(create_icon("lineweight11"), "0.55 mm", 0.55);
    lineweightSelector->addItem(create_icon("lineweight12"), "0.60 mm", 0.60);
    lineweightSelector->addItem(create_icon("lineweight13"), "0.65 mm", 0.65);
    lineweightSelector->addItem(create_icon("lineweight14"), "0.70 mm", 0.70);
    lineweightSelector->addItem(create_icon("lineweight15"), "0.75 mm", 0.75);
    lineweightSelector->addItem(create_icon("lineweight16"), "0.80 mm", 0.80);
    lineweightSelector->addItem(create_icon("lineweight17"), "0.85 mm", 0.85);
    lineweightSelector->addItem(create_icon("lineweight18"), "0.90 mm", 0.90);
    lineweightSelector->addItem(create_icon("lineweight19"), "0.95 mm", 0.95);
    lineweightSelector->addItem(create_icon("lineweight20"), "1.00 mm", 1.00);
    lineweightSelector->addItem(create_icon("lineweight21"), "1.05 mm", 1.05);
    lineweightSelector->addItem(create_icon("lineweight22"), "1.10 mm", 1.10);
    lineweightSelector->addItem(create_icon("lineweight23"), "1.15 mm", 1.15);
    lineweightSelector->addItem(create_icon("lineweight24"), "1.20 mm", 1.20);
    lineweightSelector->setMinimumContentsLength(8); // Prevent dropdown text readability being squish...d.
    toolbarHash["Properties"]->addWidget(lineweightSelector);
    connect(lineweightSelector, SIGNAL(currentIndexChanged(int)), this, SLOT(lineweightSelectorIndexChanged(int)));

    connect(toolbarHash["Properties"], SIGNAL(topLevelChanged(bool)), this, SLOT(floatingChangedToolBar(bool)));
}

void MainWindow::createTextToolbar()
{
    qDebug("MainWindow createTextToolbar()");

    toolbarHash["Text"]->setObjectName("toolbarText");

    toolbarHash["Text"]->addWidget(textFontSelector);
    textFontSelector->setCurrentFont(QFont(text_font.setting));
    connect(textFontSelector, SIGNAL(currentFontChanged(const QFont&)), this, SLOT(textFontSelectorCurrentFontChanged(const QFont&)));

    toolbarHash["Text"]->addAction(actionHash.value(ACTION_TEXT_BOLD));
    actionHash.value(ACTION_TEXT_BOLD)->setChecked(text_style_bold.setting);
    toolbarHash["Text"]->addAction(actionHash.value(ACTION_TEXT_ITALIC));
    actionHash.value(ACTION_TEXT_ITALIC)->setChecked(text_style_italic.setting);
    toolbarHash["Text"]->addAction(actionHash.value(ACTION_TEXT_UNDERLINE));
    actionHash.value(ACTION_TEXT_UNDERLINE)->setChecked(text_style_underline.setting);
    toolbarHash["Text"]->addAction(actionHash.value(ACTION_TEXT_STRIKEOUT));
    actionHash.value(ACTION_TEXT_STRIKEOUT)->setChecked(text_style_strikeout.setting);
    toolbarHash["Text"]->addAction(actionHash.value(ACTION_TEXT_OVERLINE));
    actionHash.value(ACTION_TEXT_OVERLINE)->setChecked(text_style_overline.setting);

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
    setTextSize(text_size.setting);
    toolbarHash["Text"]->addWidget(textSizeSelector);
    connect(textSizeSelector, SIGNAL(currentIndexChanged(int)), this, SLOT(textSizeSelectorIndexChanged(int)));

    connect(toolbarHash["Text"], SIGNAL(topLevelChanged(bool)), this, SLOT(floatingChangedToolBar(bool)));
}

void MainWindow::createPromptToolbar()
{
    qDebug("MainWindow createPromptToolbar()");

    toolbarHash["Prompt"]->setObjectName("toolbarPrompt");
    toolbarHash["Prompt"]->addWidget(prompt);
    toolbarHash["Prompt"]->setAllowedAreas(Qt::TopToolBarArea | Qt::BottomToolBarArea);
    connect(toolbarHash["Prompt"], SIGNAL(topLevelChanged(bool)), prompt, SLOT(floatingChanged(bool)));
}

void MainWindow::createAllToolbars()
{
    qDebug("MainWindow createAllToolbars()");

    add_to_toolbar("File", file_toolbar);
    add_to_toolbar("Edit", edit_toolbar);
    add_to_toolbar("Zoom", zoom_toolbar);
    add_to_toolbar("Pan", pan_toolbar);
    add_to_toolbar("View", view_toolbar);
    add_to_toolbar("Icon", icon_toolbar);
    add_to_toolbar("Help", help_toolbar);

    createLayerToolbar();
    createPropertiesToolbar();
    createTextToolbar();
    createPromptToolbar();

    add_to_toolbar("Draw", draw_toolbar);
    add_to_toolbar("Modify", modify_toolbar);

    /* Horizontal */
    toolbarHash["View"]->setOrientation(Qt::Horizontal);
    toolbarHash["Zoom"]->setOrientation(Qt::Horizontal);
    toolbarHash["Layer"]->setOrientation(Qt::Horizontal);
    toolbarHash["Properties"]->setOrientation(Qt::Horizontal);
    toolbarHash["Text"]->setOrientation(Qt::Horizontal);
    toolbarHash["Prompt"]->setOrientation(Qt::Horizontal);

    /* Top */
    addToolBarBreak(Qt::TopToolBarArea);
    addToolBar(Qt::TopToolBarArea, toolbarHash["File"]);
    addToolBar(Qt::TopToolBarArea, toolbarHash["Edit"]);
    addToolBar(Qt::TopToolBarArea, toolbarHash["Help"]);
    addToolBar(Qt::TopToolBarArea, toolbarHash["Icon"]);
    addToolBarBreak(Qt::TopToolBarArea);
    addToolBar(Qt::TopToolBarArea, toolbarHash["Zoom"]);
    addToolBar(Qt::TopToolBarArea, toolbarHash["Pan"]);
    addToolBar(Qt::TopToolBarArea, toolbarHash["View"]);
    addToolBarBreak(Qt::TopToolBarArea);
    addToolBar(Qt::TopToolBarArea, toolbarHash["Layer"]);
    addToolBar(Qt::TopToolBarArea, toolbarHash["Properties"]);
    addToolBarBreak(Qt::TopToolBarArea);
    addToolBar(Qt::TopToolBarArea, toolbarHash["Text"]);
    /* Bottom */
    addToolBar(Qt::BottomToolBarArea, toolbarHash["Prompt"]);

    /* Left */
    addToolBar(Qt::LeftToolBarArea, toolbarHash["Draw"]);
    addToolBar(Qt::LeftToolBarArea, toolbarHash["Modify"]);
    addToolBarBreak(Qt::LeftToolBarArea);

    //zoomToolBar->setToolButtonStyle(Qt::ToolButtonTextOnly);
}
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

/* For each Command in command_data, for each alias set up a map from
 * alias to the Command. Then for a given context the call doesn't have to loop?
 *
 * NOTE: Every QScriptProgram must have a unique function name to call. If every function was called main(), then
 *       the ScriptArgs would only call the last script evaluated (which happens to be main() in another script).
 *       Thus, by adding the cmdName before main(), it becomes line_main(), circle_main(), etc...
 *       Do not change this code unless you really know what you are doing. I mean it.
 *
 * Position currently comes from the order of the command_data.
 *
 * TODO: Set What's This Context Help to statusTip for now so there is some infos there.
 *       Make custom whats this context help popup with more descriptive help than just
 *       the status bar/tip one liner(short but not real long) with a hyperlink in the custom popup
 *       at the bottom to open full help file description. Ex: like wxPython AGW's SuperToolTip.
 *
 * TODO: Finish All Commands ... <.<
 */
void
MainWindow::createAllActions()
{
    qDebug("Creating All Actions...");
    for (int i=0; command_data[i].id != -2; i++) {
        QString icon(command_data[i].icon);
        QString toolTip(command_data[i].tooltip);
        QString statusTip(command_data[i].statustip);
        QString alias_string(command_data[i].alias);
        QStringList aliases = alias_string.split(", ");

        qDebug("COMMAND: %s", qPrintable(icon));

        QAction *ACTION = new QAction(create_icon(icon), toolTip, this);
        ACTION->setStatusTip(statusTip);
        ACTION->setObjectName(icon);
        ACTION->setWhatsThis(statusTip);

        if (strcmp(command_data[i].shortcut, "")) {
            ACTION->setShortcut(QKeySequence(command_data[i].shortcut));
        }

        if (icon == "textbold" || icon == "textitalic" || icon == "textunderline"
            || icon == "textstrikeout" || icon == "textoverline") {
            ACTION->setCheckable(true);
        }

        connect(ACTION, SIGNAL(triggered()), this, SLOT(runCommand()));

        aliasHash->insert(icon.toStdString(), icon.toStdString());
        actionHash.insert(command_data[i].id, ACTION);

        foreach (QString alias, aliases) {
            prompt->addCommand(alias, icon);
        }
    }

    actionHash.value(ACTION_WINDOW_CLOSE)->setEnabled(numOfDocs > 0);
    actionHash.value(ACTION_DESIGN_DETAILS)->setEnabled(numOfDocs > 0);
}
