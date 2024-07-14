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

#include <stdlib.h>

#include <QDebug>
#include <QFrame>
#include <QVBoxLayout>
#include <QMenu>
#include <QMenuBar>
#include <QStatusBar>
#include <QMdiArea>
#include <QWidget>
#include <QMdiSubWindow>
#include <QMessageBox>
#include <QToolBar>
#include <QFileDialog>
#include <QApplication>
#include <QDate>
#include <QFileInfo>
#include <QLabel>
#include <QComboBox>
#include <QCloseEvent>
#include <QMetaObject>
#include <QLocale>

#include <chrono>
#include <thread>

QHash<QString, Command> command_map;
QHash<int, QAction*> actionHash;
QHash<QString, QToolBar*> toolbarHash;
QHash<QString, QMenu*> menuHash;

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

    //Load translations for the Embroidermodder 2 GUI
    QTranslator translatorEmb;
    translatorEmb.load(appDir + "/translations/" + lang + "/embroidermodder2_" + lang);
    qApp->installTranslator(&translatorEmb);

    //Load translations for the commands
    QTranslator translatorCmd;
    translatorCmd.load(appDir + "/translations/" + lang + "/commands_" + lang);
    qApp->installTranslator(&translatorCmd);

    //Load translations provided by Qt - this covers dialog buttons and other common things.
    QTranslator translatorQt;
    translatorQt.load("qt_" + QLocale::system().name(), QLibraryInfo::location(QLibraryInfo::TranslationsPath)); //TODO: ensure this always loads, ship a copy of this with the app
    qApp->installTranslator(&translatorQt);

    //Init
    _main = this;
    //Menus
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
    //SubMenus
    menuHash["Recent"] = new QMenu(tr("Open &Recent"), this);
    menuHash["Zoom"] = new QMenu(tr("&Zoom"), this);
    menuHash["Pan"] = new QMenu(tr("&Pan"), this);
    //Toolbars
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
    //Selectors
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
    setMinimumSize(800, 480); //Require Minimum WVGA

    loadFormats();

    //create the mdiArea
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

    statusbar = new StatusBar(this, this);
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
                rAction->setData(opensave_recent_list_of_files.setting[i]);
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
