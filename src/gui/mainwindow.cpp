/*
 * Embroidermodder 2 -- Main Window
 * Copyright 2011-2026 The Embroidermodder Team
 */

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
#include <QtGlobal>
#include <QtGui>
#include <QStandardPaths>
#include <QDir>
#include <QtGlobal>
#include <QPoint>
#include <QSize>
#include <QAction>
#include <QToolBar>

#include <stdio.h>
#include <string.h>
#include <errno.h>
#include <stdlib.h>

#include "mainwindow.h"
#include "settings-dialog.h"
#include "statusbar.h"
#include "statusbar-button.h"
#include "view.h"
#include "cmdprompt.h"
#include "property-editor.h"
#include "undo-editor.h"
#include "native-scripting.h"
#include "preview-dialog.h"

#include "embroidery.h"

#include "toml.h"

State state;

MainWindow::MainWindow() : QMainWindow(0)
{
    char settings_dir[2000];
    /* Needs to preceed reading settings to establish the home path etc. */
#if unix || __linux__ || __APPLE__
    QString homePath = QDir::homePath();
    /* Note: on Unix we include the trailing separator. For Windows compatibility we omit it. */
    sprintf(settings_dir, "%s/.embroidermodder2/", qPrintable(homePath));
#else
    /* FIXME */
    sprintf(settings_dir, "");
#endif
    QString appDir = qApp->applicationDirPath();
    state_create(settings_dir, (char*)qPrintable(appDir));

    readSettings();

    script_env.mainWin = this;

    //Verify that files/directories needed are actually present.
    QFileInfo check(appDir + "/commands");
    if (!check.exists())
        QMessageBox::critical(this, tr("Path Error"), tr("Cannot locate: ") + check.absoluteFilePath());
    check = QFileInfo(appDir + "/help");
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

    if (!loadData()) {
        exit();
    }

    QString lang = state.settings.general_language;
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
    mainWin = this;
    //Menus
    fileMenu     = new QMenu(tr("&File"),     this);
    editMenu     = new QMenu(tr("&Edit"),     this);
    viewMenu     = new QMenu(tr("&View"),     this);
    windowMenu   = new QMenu(tr("&Window"),   this);
    helpMenu     = new QMenu(tr("&Help"),     this);
    //SubMenus
    recentMenu   = new QMenu(tr("Open &Recent"), this);
    zoomMenu     = new QMenu(tr("&Zoom"),        this);
    panMenu      = new QMenu(tr("&Pan"),         this);
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
    toolbarPrompt->show();
    //Selectors
    layerSelector      = new QComboBox(this);
    colorSelector      = new QComboBox(this);
    linetypeSelector   = new QComboBox(this);
    lineweightSelector = new QComboBox(this);
    textFontSelector   = new QFontComboBox(this);
    textSizeSelector   = new QComboBox(this);

    setWindowIcon(QIcon(appDir + "/icons/" + state.settings.general_icon_theme + "/" + "app" + ".png"));
    setMinimumSize(800, 480); //Require Minimum WVGA

    loadFormats();

    //create the mdiArea
    QFrame* vbox = new QFrame(this);
    QVBoxLayout* layout = new QVBoxLayout(vbox);
    // FIXME: layout->setMargin(0);
    vbox->setFrameStyle(QFrame::StyledPanel | QFrame::Sunken);
    mdiArea = new MdiArea(this, vbox);
    mdiArea->useBackgroundLogo(state.settings.general_mdi_bg_use_logo);
    mdiArea->useBackgroundTexture(state.settings.general_mdi_bg_use_texture);
    mdiArea->useBackgroundColor(state.settings.general_mdi_bg_use_color);
    mdiArea->setBackgroundLogo(state.settings.general_mdi_bg_logo);
    mdiArea->setBackgroundTexture(state.settings.general_mdi_bg_texture);
    mdiArea->setBackgroundColor(QColor(state.settings.general_mdi_bg_color));
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

    promptHistory = "Welcome to Embroidermodder 2!<br/>Open some of our sample files. Many formats are supported.<br/>For help, press F1.";
    prompt->setHistory(promptHistory);
    promptInputList << "";
    promptInputNum = 0;

    prompt->setPromptTextColor(QColor(state.settings.prompt_text_color));
    prompt->setPromptBackgroundColor(QColor(state.settings.prompt_bg_color));

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
    dockPropEdit = new PropertyEditor(appDir + "/icons/" + state.settings.general_icon_theme, state.settings.selection_mode_pickadd, prompt, this);
    addDockWidget(Qt::LeftDockWidgetArea, dockPropEdit);
    connect(dockPropEdit, SIGNAL(pickAddModeToggled()), this, SLOT(pickAddModeToggled()));

    //create the Command History Undo Editor
    dockUndoEdit = new UndoEditor(appDir + "/icons/" + state.settings.general_icon_theme, prompt, this);
    addDockWidget(Qt::LeftDockWidgetArea, dockUndoEdit);

    //setDockOptions(QMainWindow::AnimatedDocks | QMainWindow::AllowTabbedDocks | QMainWindow::VerticalTabs); //TODO: Load these from settings
    //tabifyDockWidget(dockPropEdit, dockUndoEdit); //TODO: load this from settings

    // engine = new QJSEngine(this);
    // FIXME: engine->installTranslatorFunctions();
    engine.installExtensions(QJSEngine::ConsoleExtension);
    javaInitNatives();

    //Load all commands in a loop
    QDir commandDir(appDir + "/commands");
    QStringList cmdList = commandDir.entryList(QDir::Dirs | QDir::NoDotAndDotDot);
    foreach(QString cmdName, cmdList)
    {
        javaLoadCommand(cmdName);
    }

    statusbar = new StatusBar(this, this);
    this->setStatusBar(statusbar);

    createAllActions();
    createAllMenus();
    createAllToolbars();

    iconResize(state.settings.general_icon_size);
    updateMenuToolbarStatusbar();

    //Show date in statusbar after it has been updated
    QDate date = QDate::currentDate();
    QString datestr = date.toString("MMMM d, yyyy");
    statusbar->showMessage(datestr);

    showNormal();

    if (state.settings.general_tip_of_the_day) {
        tipOfTheDay();
    }
}

MainWindow::~MainWindow()
{
    qDebug("MainWindow::Destructor()");

    state_free();

    //Prevent memory leaks by deleting any unpasted objects
    qDeleteAll(cutCopyObjectList.begin(), cutCopyObjectList.end());
    cutCopyObjectList.clear();
}

void
MainWindow::createAllActions()
{
    qDebug("Creating All Actions...");
    for (int i=0; command_table[i].id != ACTION_null; i++) {
        /* TODO: override the shortcuts here. */
        QAction *ACTION = createAction(command_table[i]);
        actionHash.insert(command_table[i].id, ACTION);
    }

    actionHash.value(ACTION_windowclose)->setEnabled(state.num_docs > 0);
    actionHash.value(ACTION_designdetails)->setEnabled(state.num_docs > 0);
}

QAction *
MainWindow::createAction(CommandData command, bool scripted)
{
    QString appDir = qApp->applicationDirPath();

    QString icon_path = appDir + "/icons/" + state.settings.general_icon_theme
        + "/" + QString(command.label) + ".png";
    QAction *ACTION = new QAction(QIcon(icon_path), command.tooltip, this);
    ACTION->setStatusTip(command.statustip);
    ACTION->setObjectName(command.label);
    // TODO: Set What's This Context Help to statusTip for now so there is some infos there.
    // Make custom whats this context help popup with more descriptive help than just
    // the status bar/tip one liner(short but not real long) with a hyperlink in the custom popup
    // at the bottom to open full help file description. Ex: like wxPython AGW's SuperToolTip.
    ACTION->setWhatsThis(command.statustip);
    // TODO: Finish All Commands ... <.<


    /* FIXME: detect if we should use PC or mac shortcuts here. This only supports PC. */
    if (strlen(command.shortcut) > 0) {
        ACTION->setShortcut(QKeySequence(command.shortcut));
    }
    /*
    if (strlen(command.mac_shortcut) > 0) {
        ACTION->setShortcut(QKeySequence(command.mac_shortcut));
    }
    */

    if (scripted) {
        ACTION->setIcon(QIcon(appDir + "/commands/" + QString(command.label)
            + "/" + QString(command.label) + ".png"));
        connect(ACTION, SIGNAL(triggered()), this, SLOT(runCommand()));
    }
    else {
        switch (command.type) {
        case CMD_TYPE_TOGGLE: {
            ACTION->setCheckable(true);
            connect(ACTION, &QAction::toggle, this, [=]() { call(command.label); });
            break;
        }
        default:
        case CMD_TYPE_TRIGGER:
            connect(ACTION, &QAction::triggered, this, [=]() { call(command.label); });
            break;
        }
    }
    return ACTION;
}

void
MainWindow::readSettings(void)
{
    qDebug("Reading Settings...");
    int window_pos[2] = {0, 0};
    int window_size[2] = {800, 600};

    settings_load(&state.settings, window_pos, window_size);
    // qDebug("NOTE: settings_load failed, using defaults.");

    QPoint pos(window_pos[0], window_pos[1]);
    QSize size(window_size[0], window_size[1]);
    move(pos);
    resize(size);
}

void
MainWindow::writeSettings()
{
    qDebug("Writing Settings...");
    int window_pos[2] = {0, 0};
    int window_size[2] = {800, 600};
    settings_save(&state.settings, window_pos, window_size);
    /* FIXME: return value. */
}

void MainWindow::settingsPrompt()
{
    settingsDialog("Prompt");
}

void MainWindow::settingsDialog(const QString& showTab)
{
    Settings_Dialog dialog(this, showTab, this);
    dialog.exec();
}

/*
 * Loads a string table from the named file.
 *
 * It assumes that the sdsarray has been created already.
 */
int
load_sdsarray(QString filename, const char *key, sdsarray *arr)
{
    char errbuffer[200];
    filename = QString(state.app_dir) + "/" + filename;
    FILE *fp = fopen(qPrintable(filename), "r");
    if (!fp) {
        qDebug("ERROR: failed to open file \"%s\".", qPrintable(filename));
        return 0;
    }

    toml_table_t* table = toml_parse_file(fp, errbuffer, sizeof(errbuffer));
    if (!table) {
        qDebug("ERROR: failed to parse file \"%s\".", qPrintable(filename));
        qDebug("ERROR: %s", errbuffer);
        return 0;
    }

    /* FIXME: check key is present */

    sdsarray_empty(arr);

    toml_array_t* array = toml_array_in(table, key);
    for (int i=0; ; i++) {
        toml_datum_t str = toml_string_at(array, i);
        if (!str.ok) {
            break;
         }
        sdsarray_append(arr, str.u.s);
        free(str.u.s);
    }

    fclose(fp);
    return 1;
}

/*
 * This calls all of the other loaders: fully loading initialisation
 * outside of settings.
 */
int
MainWindow::loadData(void)
{
    if (!load_sdsarray("manifest.toml", "manifest", state.manifest)) {
        return 0;
    }
    if (!load_sdsarray("tables/tips.toml", "tips", state.tips)) {
        return 0;
    }
    return 1;
}

QAction* MainWindow::getAction(int actionEnum)
{
    return actionHash.value(actionEnum);
}

void MainWindow::recentMenuAboutToShow()
{
    qDebug("MainWindow::recentMenuAboutToShow()");
    recentMenu->clear();

    QFileInfo recentFileInfo;
    QString recentValue;
    for (int i = 0; i < state.settings.opensave_recent_list_of_files->count; ++i) {
        /* If less than the max amount of entries add to menu. */
        if (i < state.settings.opensave_recent_max_files) {
            recentFileInfo = QFileInfo(state.settings.opensave_recent_list_of_files->data[i]);
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
                rAction->setData(QString(state.settings.opensave_recent_list_of_files->data[i]));
                recentMenu->addAction(rAction);
                connect(rAction, SIGNAL(triggered()), this, SLOT(openrecentfile()));
            }
        }
    }

    /* Ensure the list only has max amount of entries */
    if (state.settings.opensave_recent_list_of_files->count > state.settings.opensave_recent_max_files) {
        state.settings.opensave_recent_list_of_files->count = state.settings.opensave_recent_max_files;
    }
}

void MainWindow::windowMenuAboutToShow()
{
    qDebug("MainWindow::windowMenuAboutToShow()");
    windowMenu->clear();
    windowMenu->addAction(actionHash.value(ACTION_windowclose));
    windowMenu->addAction(actionHash.value(ACTION_windowcloseall));
    windowMenu->addSeparator();
    windowMenu->addAction(actionHash.value(ACTION_windowcascade));
    windowMenu->addAction(actionHash.value(ACTION_windowtile));
    windowMenu->addSeparator();
    windowMenu->addAction(actionHash.value(ACTION_windownext));
    windowMenu->addAction(actionHash.value(ACTION_windowprevious));

    windowMenu->addSeparator();
    QList<QMdiSubWindow*> windows = mdiArea->subWindowList();
    for (int i = 0; i < windows.count(); ++i)
    {
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
    if (!aSender)
        return;
    QWidget* w = mdiArea->subWindowList().at(aSender->data().toInt());
    if (w && checked)
        w->setFocus();
}

MdiArea* MainWindow::getMdiArea()
{
    qDebug("MainWindow::getMdiArea()");
    return mdiArea;
}

MainWindow* MainWindow::getApplication()
{
    qDebug("MainWindow::getApplication()");
    return mainWin;
}

void MainWindow::newFile()
{
    qDebug("MainWindow::newFile()");
    state.doc_index++;
    state.num_docs++;
    MdiWindow* mdiWin = new MdiWindow(state.doc_index, mainWin, mdiArea, Qt::SubWindow);
    connect(mdiWin, SIGNAL(sendCloseMdiWin(MdiWindow*)), this, SLOT(onCloseMdiWin(MdiWindow*)));
    connect(mdiArea, SIGNAL(subWindowActivated(QMdiSubWindow*)), this, SLOT(onWindowActivated(QMdiSubWindow*)));

    updateMenuToolbarStatusbar();
    windowMenuAboutToShow();

    View* v = mdiWin->getView();
    if (v)
    {
        v->recalculateLimits();
        v->zoomExtents();
    }
}

void MainWindow::openFile(bool recent, const QString& recentFile)
{
    qDebug("MainWindow::openFile()");

    QApplication::setOverrideCursor(Qt::ArrowCursor);

    QStringList files;
    bool preview = state.settings.opensave_open_thumbnail;
    openFilesPath = state.settings.opensave_recent_directory;

    //Check to see if this from the recent files list
    if (recent)
    {
        files.append(recentFile);
        openFilesSelected(files);
    }
    else if (!preview)
    {
        //TODO: set getOpenFileNames' selectedFilter parameter from state.settings.opensave_open_format
        files = QFileDialog::getOpenFileNames(this, tr("Open"), openFilesPath, formatFilterOpen);
        openFilesSelected(files);
    }
    else if (preview)
    {
        PreviewDialog* openDialog = new PreviewDialog(this, tr("Open w/Preview"), openFilesPath, formatFilterOpen);
        //TODO: set openDialog->selectNameFilter(const QString& filter) from state.settings.opensave_open_format
        connect(openDialog, SIGNAL(filesSelected(const QStringList&)), this, SLOT(openFilesSelected(const QStringList&)));
        openDialog->exec();
    }

    QApplication::restoreOverrideCursor();
}

void MainWindow::openFilesSelected(const QStringList& filesToOpen)
{
    bool doOnce = true;

    if (filesToOpen.count()) {
        for (int i = 0; i < filesToOpen.count(); i++) {
            if (!validFileFormat(filesToOpen[i])) {
                continue;
            }

            QMdiSubWindow* existing = findMdiWindow(filesToOpen[i]);
            if (existing) {
                mdiArea->setActiveSubWindow(existing);
                continue;
            }

            /* The docIndex doesn't need increased as it is only used for unnamed files. */
            state.num_docs++;
            MdiWindow* mdiWin = new MdiWindow(state.doc_index, mainWin, mdiArea, Qt::SubWindow);
            connect(mdiWin, SIGNAL(sendCloseMdiWin(MdiWindow*)), this, SLOT(onCloseMdiWin(MdiWindow*)));
            connect(mdiArea, SIGNAL(subWindowActivated(QMdiSubWindow*)), this, SLOT(onWindowActivated(QMdiSubWindow*)));

            /* Make sure the toolbars/etc... are shown before doing their zoomExtents. */
            if (doOnce) {
                updateMenuToolbarStatusbar();
                doOnce = false;
            }

            if (mdiWin->loadFile(filesToOpen.at(i))) {
                statusbar->showMessage(tr("File(s) loaded"), 2000);
                mdiWin->show();
                mdiWin->showMaximized();
                #if 0
                /* FIXME: stronger sdsarray. */
                /* Prevent duplicate entries in the recent files list */
                if (!state.settings.opensave_recent_list_of_files.contains(filesToOpen.at(i), Qt::CaseInsensitive)) {
                    state.settings.opensave_recent_list_of_files.prepend(filesToOpen.at(i));
                }
                else {
                    /* Move the recent file to the top of the list */
                    state.settings.opensave_recent_list_of_files.removeAll(filesToOpen.at(i));
                    state.settings.opensave_recent_list_of_files.prepend(filesToOpen.at(i));
                }
                state.settings.opensave_recent_directory = QFileInfo(filesToOpen.at(i)).absolutePath();
                #endif

                View* v = mdiWin->getView();
                if (v) {
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
    if (recentSender) {
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
    if (!mdiWin)
        return;

    QString file;
    openFilesPath = state.settings.opensave_recent_directory;
    file = QFileDialog::getSaveFileName(this, tr("Save As"), openFilesPath, formatFilterSave);

    mdiWin->saveFile(file);
}

QMdiSubWindow* MainWindow::findMdiWindow(const QString& fileName)
{
    qDebug("MainWindow::findMdiWindow(%s)", qPrintable(fileName));
    QString canonicalFilePath = QFileInfo(fileName).canonicalFilePath();

    foreach(QMdiSubWindow* subWindow, mdiArea->subWindowList()) {
        MdiWindow* mdiWin = qobject_cast<MdiWindow*>(subWindow);
        if (mdiWin) {
            if (mdiWin->getCurrentFile() == canonicalFilePath) {
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
    state.num_docs--;

    bool keepMaximized;
    if (theMdiWin) { keepMaximized = theMdiWin->isMaximized(); }

    mdiArea->removeSubWindow(theMdiWin);
    theMdiWin->deleteLater();

    updateMenuToolbarStatusbar();
    windowMenuAboutToShow();

    if (keepMaximized)
    {
        MdiWindow* mdiWin = qobject_cast<MdiWindow*>(mdiArea->activeSubWindow());
        if (mdiWin) { mdiWin->showMaximized(); }
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

    actionHash.value(ACTION_print)->setEnabled(state.num_docs > 0);
    actionHash.value(ACTION_windowclose)->setEnabled(state.num_docs > 0);
    actionHash.value(ACTION_designdetails)->setEnabled(state.num_docs > 0);

    if (state.num_docs) {
        //Toolbars
        toolbarView->show();
        toolbarZoom->show();
        toolbarPan->show();
        toolbarIcon->show();
        toolbarHelp->show();
        toolbarLayer->show();
        toolbarText->show();
        toolbarProperties->show();

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
    if (embFormat_typeFromName(qPrintable(fileName)))
        return true;
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

    EmbFormatList* curFormat = 0;
    EmbFormatList* formatList = embFormatList_create();
    if (!formatList) { QMessageBox::critical(this, tr("Format Loading Error"), tr("Unable to load formats from libembroidery.")); return; }
    curFormat = formatList;
    while(curFormat)
    {
        extension = embFormat_extension(curFormat);
        description = embFormat_description(curFormat);
        readerState = embFormat_readerState(curFormat);
        writerState = embFormat_writerState(curFormat);

        QString upperExt = QString(extension).toUpper();
        supportedStr = "*" + upperExt + " ";
        individualStr = upperExt.replace(".", "") + " - " + description + " (*" + extension + ");;";
        if (readerState == stable || readerState == unstable)
        {
            //Exclude color file formats from open dialogs
            if (upperExt != "COL" && upperExt != "EDR" && upperExt != "INF" && upperExt != "RGB")
            {
                supportedReaders.append(supportedStr);
                individualReaders.append(individualStr);
            }
        }
        if (writerState == stable || writerState == unstable)
        {
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
    /*
    QString custom = state.settings.custom_filter;
    if (custom.contains("supported", Qt::CaseInsensitive))
        custom = ""; //This will hide it
    else if (!custom.contains("*", Qt::CaseInsensitive))
        custom = ""; //This will hide it
    else
        custom = "Custom Filter(" + custom + ");;";

    return tr(qPrintable(custom + supported + all));
    */
}

void MainWindow::closeToolBar(QAction* action)
{
    if (action->objectName() == "toolbarclose")
    {
        QToolBar* tb = qobject_cast<QToolBar*>(sender());
        if (tb)
        {
            qDebug("%s closed.", qPrintable(tb->objectName()));
            tb->hide();
        }
    }
}

void MainWindow::floatingChangedToolBar(bool isFloating)
{
    QToolBar* tb = qobject_cast<QToolBar*>(sender());
    if (tb)
    {
        if (isFloating)
        {
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
        else
        {
            QList<QAction*> actList = tb->actions();
            for (int i = 0; i < actList.size(); ++i)
            {
                QAction* ACTION = actList.value(i);
                if (ACTION->objectName() == "toolbarclose")
                {
                    tb->removeAction(ACTION);
                    disconnect(tb, SIGNAL(actionTriggered(QAction*)), this, SLOT(closeToolBar(QAction*)));
                    delete ACTION;
                }
            }
        }
    }
}

void MainWindow::createFileMenu()
{
    qDebug("MainWindow createFileMenu()");
    menuBar()->addMenu(fileMenu);
    fileMenu->addAction(actionHash.value(ACTION_new));
    fileMenu->addSeparator();
    fileMenu->addAction(actionHash.value(ACTION_open));

    fileMenu->addMenu(recentMenu);
    connect(recentMenu, SIGNAL(aboutToShow()), this, SLOT(recentMenuAboutToShow()));
    //Do not allow the Recent Menu to be torn off. It's a pain in the ass to maintain.
    recentMenu->setTearOffEnabled(false);

    fileMenu->addSeparator();
    fileMenu->addAction(actionHash.value(ACTION_save));
    fileMenu->addAction(actionHash.value(ACTION_saveas));
    fileMenu->addSeparator();
    fileMenu->addAction(actionHash.value(ACTION_print));
    fileMenu->addSeparator();
    fileMenu->addAction(actionHash.value(ACTION_windowclose));
    fileMenu->addSeparator();
    fileMenu->addAction(actionHash.value(ACTION_designdetails));
    fileMenu->addSeparator();

    fileMenu->addAction(actionHash.value(ACTION_exit));
    fileMenu->setTearOffEnabled(false);
}

void MainWindow::createEditMenu()
{
    qDebug("MainWindow createEditMenu()");
    menuBar()->addMenu(editMenu);
    editMenu->addAction(actionHash.value(ACTION_undo));
    editMenu->addAction(actionHash.value(ACTION_redo));
    editMenu->addSeparator();
    editMenu->addAction(actionHash.value(ACTION_cut));
    editMenu->addAction(actionHash.value(ACTION_copy));
    editMenu->addAction(actionHash.value(ACTION_paste));
    editMenu->addSeparator();
    editMenu->addAction(actionHash.value(ACTION_settingsdialog));
    editMenu->setTearOffEnabled(true);
}

void MainWindow::createViewMenu()
{
    qDebug("MainWindow createViewMenu()");

    QString appDir = qApp->applicationDirPath();
    QString icontheme = state.settings.general_icon_theme;

    menuBar()->addMenu(viewMenu);
    viewMenu->addSeparator();
    viewMenu->addMenu(zoomMenu);
    zoomMenu->setIcon(QIcon(appDir + "/icons/" + icontheme + "/zoom" + ".png"));
    zoomMenu->addAction(actionHash.value(ACTION_zoomrealtime));
    zoomMenu->addAction(actionHash.value(ACTION_zoomprevious));
    zoomMenu->addSeparator();
    zoomMenu->addAction(actionHash.value(ACTION_zoomwindow));
    zoomMenu->addAction(actionHash.value(ACTION_zoomdynamic));
    zoomMenu->addAction(actionHash.value(ACTION_zoomscale));
    zoomMenu->addAction(actionHash.value(ACTION_zoomcenter));
    zoomMenu->addSeparator();
    zoomMenu->addAction(actionHash.value(ACTION_zoomin));
    zoomMenu->addAction(actionHash.value(ACTION_zoomout));
    zoomMenu->addSeparator();
    zoomMenu->addAction(actionHash.value(ACTION_zoomselected));
    zoomMenu->addAction(actionHash.value(ACTION_zoomall));
    zoomMenu->addAction(actionHash.value(ACTION_zoomextents));
    viewMenu->addMenu(panMenu);
    panMenu->setIcon(QIcon(appDir + "/icons/" + icontheme + "/pan" + ".png"));
    panMenu->addAction(actionHash.value(ACTION_panrealtime));
    panMenu->addAction(actionHash.value(ACTION_panpoint));
    panMenu->addSeparator();
    panMenu->addAction(actionHash.value(ACTION_panleft));
    panMenu->addAction(actionHash.value(ACTION_panright));
    panMenu->addAction(actionHash.value(ACTION_panup));
    panMenu->addAction(actionHash.value(ACTION_pandown));
    viewMenu->addSeparator();
    viewMenu->addAction(actionHash.value(ACTION_day));
    viewMenu->addAction(actionHash.value(ACTION_night));
    viewMenu->addSeparator();

    viewMenu->setTearOffEnabled(true);
    zoomMenu->setTearOffEnabled(true);
    panMenu->setTearOffEnabled(true);
}

void MainWindow::createWindowMenu()
{
    qDebug("MainWindow createWindowMenu()");
    menuBar()->addMenu(windowMenu);
    connect(windowMenu, SIGNAL(aboutToShow()), this, SLOT(windowMenuAboutToShow()));
    //Do not allow the Window Menu to be torn off. It's a pain in the ass to maintain.
    windowMenu->setTearOffEnabled(false);

}

void MainWindow::createHelpMenu()
{
    qDebug("MainWindow createHelpMenu()");
    menuBar()->addMenu(helpMenu);
    helpMenu->addAction(actionHash.value(ACTION_help));
    helpMenu->addSeparator();
    helpMenu->addAction(actionHash.value(ACTION_changelog));
    helpMenu->addSeparator();
    helpMenu->addAction(actionHash.value(ACTION_tipoftheday));
    helpMenu->addSeparator();
    helpMenu->addAction(actionHash.value(ACTION_about));
    helpMenu->addSeparator();
    helpMenu->addAction(actionHash.value(ACTION_whatsthis));
    helpMenu->setTearOffEnabled(true);
}

void MainWindow::createAllMenus()
{
    qDebug("MainWindow createAllMenus()");
    createFileMenu();
    createEditMenu();
    createViewMenu();
    createWindowMenu();
    createHelpMenu();

}

void MainWindow::createFileToolbar()
{
    qDebug("MainWindow createFileToolbar()");

    toolbarFile->setObjectName("toolbarFile");
    toolbarFile->addAction(actionHash.value(ACTION_new));
    toolbarFile->addAction(actionHash.value(ACTION_open));
    toolbarFile->addAction(actionHash.value(ACTION_save));
    toolbarFile->addAction(actionHash.value(ACTION_saveas));
    toolbarFile->addAction(actionHash.value(ACTION_print));
    toolbarFile->addAction(actionHash.value(ACTION_designdetails));

    toolbarFile->addSeparator();
    toolbarFile->addAction(actionHash.value(ACTION_undo));
    toolbarFile->addAction(actionHash.value(ACTION_redo));
    toolbarFile->addSeparator();
    toolbarFile->addAction(actionHash.value(ACTION_help));

    connect(toolbarFile, SIGNAL(topLevelChanged(bool)), this, SLOT(floatingChangedToolBar(bool)));
}

void MainWindow::createEditToolbar()
{
    qDebug("MainWindow createEditToolbar()");

    toolbarEdit->setObjectName("toolbarEdit");
    toolbarEdit->addAction(actionHash.value(ACTION_cut));
    toolbarEdit->addAction(actionHash.value(ACTION_copy));
    toolbarEdit->addAction(actionHash.value(ACTION_paste));

    connect(toolbarEdit, SIGNAL(topLevelChanged(bool)), this, SLOT(floatingChangedToolBar(bool)));
}

void MainWindow::createViewToolbar()
{
    qDebug("MainWindow createViewToolbar()");

    toolbarView->setObjectName("toolbarView");
    toolbarView->addAction(actionHash.value(ACTION_day));
    toolbarView->addAction(actionHash.value(ACTION_night));

    connect(toolbarView, SIGNAL(topLevelChanged(bool)), this, SLOT(floatingChangedToolBar(bool)));
}

void MainWindow::createZoomToolbar()
{
    qDebug("MainWindow createZoomToolbar()");

    toolbarZoom->setObjectName("toolbarZoom");
    toolbarZoom->addAction(actionHash.value(ACTION_zoomwindow));
    toolbarZoom->addAction(actionHash.value(ACTION_zoomdynamic));
    toolbarZoom->addAction(actionHash.value(ACTION_zoomscale));
    toolbarZoom->addSeparator();
    toolbarZoom->addAction(actionHash.value(ACTION_zoomcenter));
    toolbarZoom->addAction(actionHash.value(ACTION_zoomin));
    toolbarZoom->addAction(actionHash.value(ACTION_zoomout));
    toolbarZoom->addSeparator();
    toolbarZoom->addAction(actionHash.value(ACTION_zoomselected));
    toolbarZoom->addAction(actionHash.value(ACTION_zoomall));
    toolbarZoom->addAction(actionHash.value(ACTION_zoomextents));

    connect(toolbarZoom, SIGNAL(topLevelChanged(bool)), this, SLOT(floatingChangedToolBar(bool)));
}

void MainWindow::createPanToolbar()
{
    qDebug("MainWindow createPanToolbar()");

    toolbarPan->setObjectName("toolbarPan");
    toolbarPan->addAction(actionHash.value(ACTION_panrealtime));
    toolbarPan->addAction(actionHash.value(ACTION_panpoint));
    toolbarPan->addSeparator();
    toolbarPan->addAction(actionHash.value(ACTION_panleft));
    toolbarPan->addAction(actionHash.value(ACTION_panright));
    toolbarPan->addAction(actionHash.value(ACTION_panup));
    toolbarPan->addAction(actionHash.value(ACTION_pandown));

    connect(toolbarPan, SIGNAL(topLevelChanged(bool)), this, SLOT(floatingChangedToolBar(bool)));
}

void MainWindow::createIconToolbar()
{
    qDebug("MainWindow createIconToolbar()");

    toolbarIcon->setObjectName("toolbarIcon");
    toolbarIcon->addAction(actionHash.value(ACTION_icon16));
    toolbarIcon->addAction(actionHash.value(ACTION_icon24));
    toolbarIcon->addAction(actionHash.value(ACTION_icon32));
    toolbarIcon->addAction(actionHash.value(ACTION_icon48));
    toolbarIcon->addAction(actionHash.value(ACTION_icon64));
    toolbarIcon->addAction(actionHash.value(ACTION_icon128));

    connect(toolbarIcon, SIGNAL(topLevelChanged(bool)), this, SLOT(floatingChangedToolBar(bool)));
}

void MainWindow::createHelpToolbar()
{
    qDebug("MainWindow createHelpToolbar()");

    toolbarHelp->setObjectName("toolbarHelp");
    toolbarHelp->addAction(actionHash.value(ACTION_help));
    toolbarHelp->addSeparator();
    toolbarHelp->addAction(actionHash.value(ACTION_changelog));
    toolbarHelp->addSeparator();
    toolbarHelp->addAction(actionHash.value(ACTION_about));
    toolbarHelp->addSeparator();
    toolbarHelp->addAction(actionHash.value(ACTION_whatsthis));

    connect(toolbarHelp, SIGNAL(topLevelChanged(bool)), this, SLOT(floatingChangedToolBar(bool)));
}

void MainWindow::createLayerToolbar()
{
    qDebug("MainWindow createLayerToolbar()");

    toolbarLayer->setObjectName("toolbarLayer");
    toolbarLayer->addAction(actionHash.value(ACTION_makelayercurrent));
    toolbarLayer->addAction(actionHash.value(ACTION_layers));

    QString appDir = qApp->applicationDirPath();
    QString icontheme = state.settings.general_icon_theme;

    layerSelector->setFocusProxy(prompt);
    //NOTE: Qt4.7 wont load icons without an extension...
    //TODO: Create layer pixmaps by concatenating several icons
    layerSelector->addItem(QIcon(appDir + "/icons/" + icontheme + "/" + "linetypebylayer" + ".png"), "0");
    layerSelector->addItem(QIcon(appDir + "/icons/" + icontheme + "/" + "linetypebylayer" + ".png"), "1");
    layerSelector->addItem(QIcon(appDir + "/icons/" + icontheme + "/" + "linetypebylayer" + ".png"), "2");
    layerSelector->addItem(QIcon(appDir + "/icons/" + icontheme + "/" + "linetypebylayer" + ".png"), "3");
    layerSelector->addItem(QIcon(appDir + "/icons/" + icontheme + "/" + "linetypebylayer" + ".png"), "4");
    layerSelector->addItem(QIcon(appDir + "/icons/" + icontheme + "/" + "linetypebylayer" + ".png"), "5");
    layerSelector->addItem(QIcon(appDir + "/icons/" + icontheme + "/" + "linetypebylayer" + ".png"), "6");
    layerSelector->addItem(QIcon(appDir + "/icons/" + icontheme + "/" + "linetypebylayer" + ".png"), "7");
    layerSelector->addItem(QIcon(appDir + "/icons/" + icontheme + "/" + "linetypebylayer" + ".png"), "8");
    layerSelector->addItem(QIcon(appDir + "/icons/" + icontheme + "/" + "linetypebylayer" + ".png"), "9");
    toolbarLayer->addWidget(layerSelector);
    connect(layerSelector, SIGNAL(currentIndexChanged(int)), this, SLOT(layerSelectorIndexChanged(int)));

    toolbarLayer->addAction(actionHash.value(ACTION_layerprevious));

    connect(toolbarLayer, SIGNAL(topLevelChanged(bool)), this, SLOT(floatingChangedToolBar(bool)));
}

void MainWindow::createPropertiesToolbar()
{
    qDebug("MainWindow createPropertiesToolbar()");

    toolbarProperties->setObjectName("toolbarProperties");

    QString appDir = qApp->applicationDirPath();
    QString icontheme = state.settings.general_icon_theme;

    colorSelector->setFocusProxy(prompt);
    //NOTE: Qt4.7 wont load icons without an extension...
    colorSelector->addItem(QIcon(appDir + "/icons/" + icontheme + "/" + "colorbylayer" + ".png"), "ByLayer");
    colorSelector->addItem(QIcon(appDir + "/icons/" + icontheme + "/" + "colorbyblock" + ".png"), "ByBlock");
    colorSelector->addItem(QIcon(appDir + "/icons/" + icontheme + "/" + "colorred"     + ".png"), tr("Red"),     qRgb(255,  0,  0));
    colorSelector->addItem(QIcon(appDir + "/icons/" + icontheme + "/" + "coloryellow"  + ".png"), tr("Yellow"),  qRgb(255,255,  0));
    colorSelector->addItem(QIcon(appDir + "/icons/" + icontheme + "/" + "colorgreen"   + ".png"), tr("Green"),   qRgb(  0,255,  0));
    colorSelector->addItem(QIcon(appDir + "/icons/" + icontheme + "/" + "colorcyan"    + ".png"), tr("Cyan"),    qRgb(  0,255,255));
    colorSelector->addItem(QIcon(appDir + "/icons/" + icontheme + "/" + "colorblue"    + ".png"), tr("Blue"),    qRgb(  0,  0,255));
    colorSelector->addItem(QIcon(appDir + "/icons/" + icontheme + "/" + "colormagenta" + ".png"), tr("Magenta"), qRgb(255,  0,255));
    colorSelector->addItem(QIcon(appDir + "/icons/" + icontheme + "/" + "colorwhite"   + ".png"), tr("White"),   qRgb(255,255,255));
    colorSelector->addItem(QIcon(appDir + "/icons/" + icontheme + "/" + "colorother"   + ".png"), tr("Other..."));
    toolbarProperties->addWidget(colorSelector);
    connect(colorSelector, SIGNAL(currentIndexChanged(int)), this, SLOT(colorSelectorIndexChanged(int)));

    toolbarProperties->addSeparator();
    linetypeSelector->setFocusProxy(prompt);
    //NOTE: Qt4.7 wont load icons without an extension...
    linetypeSelector->addItem(QIcon(appDir + "/icons/" + icontheme + "/" + "linetypebylayer"    + ".png"), "ByLayer");
    linetypeSelector->addItem(QIcon(appDir + "/icons/" + icontheme + "/" + "linetypebyblock"    + ".png"), "ByBlock");
    linetypeSelector->addItem(QIcon(appDir + "/icons/" + icontheme + "/" + "linetypecontinuous" + ".png"), "Continuous");
    linetypeSelector->addItem(QIcon(appDir + "/icons/" + icontheme + "/" + "linetypehidden"     + ".png"), "Hidden");
    linetypeSelector->addItem(QIcon(appDir + "/icons/" + icontheme + "/" + "linetypecenter"     + ".png"), "Center");
    linetypeSelector->addItem(QIcon(appDir + "/icons/" + icontheme + "/" + "linetypeother"      + ".png"), "Other...");
    toolbarProperties->addWidget(linetypeSelector);
    connect(linetypeSelector, SIGNAL(currentIndexChanged(int)), this, SLOT(linetypeSelectorIndexChanged(int)));

    toolbarProperties->addSeparator();
    lineweightSelector->setFocusProxy(prompt);
    //NOTE: Qt4.7 wont load icons without an extension...
    lineweightSelector->addItem(QIcon(appDir + "/icons/" + icontheme + "/" + "lineweightbylayer" + ".png"), "ByLayer", -2.00);
    lineweightSelector->addItem(QIcon(appDir + "/icons/" + icontheme + "/" + "lineweightbyblock" + ".png"), "ByBlock", -1.00);
    lineweightSelector->addItem(QIcon(appDir + "/icons/" + icontheme + "/" + "lineweightdefault" + ".png"), "Default",  0.00);
    //TODO: Thread weight is weird. See http://en.wikipedia.org/wiki/Thread_(yarn)#Weight
    lineweightSelector->addItem(QIcon(appDir + "/icons/" + icontheme + "/" + "lineweight01" + ".png"), "0.00 mm", 0.00);
    lineweightSelector->addItem(QIcon(appDir + "/icons/" + icontheme + "/" + "lineweight02" + ".png"), "0.05 mm", 0.05);
    lineweightSelector->addItem(QIcon(appDir + "/icons/" + icontheme + "/" + "lineweight03" + ".png"), "0.15 mm", 0.15);
    lineweightSelector->addItem(QIcon(appDir + "/icons/" + icontheme + "/" + "lineweight04" + ".png"), "0.20 mm", 0.20);
    lineweightSelector->addItem(QIcon(appDir + "/icons/" + icontheme + "/" + "lineweight05" + ".png"), "0.25 mm", 0.25);
    lineweightSelector->addItem(QIcon(appDir + "/icons/" + icontheme + "/" + "lineweight06" + ".png"), "0.30 mm", 0.30);
    lineweightSelector->addItem(QIcon(appDir + "/icons/" + icontheme + "/" + "lineweight07" + ".png"), "0.35 mm", 0.35);
    lineweightSelector->addItem(QIcon(appDir + "/icons/" + icontheme + "/" + "lineweight08" + ".png"), "0.40 mm", 0.40);
    lineweightSelector->addItem(QIcon(appDir + "/icons/" + icontheme + "/" + "lineweight09" + ".png"), "0.45 mm", 0.45);
    lineweightSelector->addItem(QIcon(appDir + "/icons/" + icontheme + "/" + "lineweight10" + ".png"), "0.50 mm", 0.50);
    lineweightSelector->addItem(QIcon(appDir + "/icons/" + icontheme + "/" + "lineweight11" + ".png"), "0.55 mm", 0.55);
    lineweightSelector->addItem(QIcon(appDir + "/icons/" + icontheme + "/" + "lineweight12" + ".png"), "0.60 mm", 0.60);
    lineweightSelector->addItem(QIcon(appDir + "/icons/" + icontheme + "/" + "lineweight13" + ".png"), "0.65 mm", 0.65);
    lineweightSelector->addItem(QIcon(appDir + "/icons/" + icontheme + "/" + "lineweight14" + ".png"), "0.70 mm", 0.70);
    lineweightSelector->addItem(QIcon(appDir + "/icons/" + icontheme + "/" + "lineweight15" + ".png"), "0.75 mm", 0.75);
    lineweightSelector->addItem(QIcon(appDir + "/icons/" + icontheme + "/" + "lineweight16" + ".png"), "0.80 mm", 0.80);
    lineweightSelector->addItem(QIcon(appDir + "/icons/" + icontheme + "/" + "lineweight17" + ".png"), "0.85 mm", 0.85);
    lineweightSelector->addItem(QIcon(appDir + "/icons/" + icontheme + "/" + "lineweight18" + ".png"), "0.90 mm", 0.90);
    lineweightSelector->addItem(QIcon(appDir + "/icons/" + icontheme + "/" + "lineweight19" + ".png"), "0.95 mm", 0.95);
    lineweightSelector->addItem(QIcon(appDir + "/icons/" + icontheme + "/" + "lineweight20" + ".png"), "1.00 mm", 1.00);
    lineweightSelector->addItem(QIcon(appDir + "/icons/" + icontheme + "/" + "lineweight21" + ".png"), "1.05 mm", 1.05);
    lineweightSelector->addItem(QIcon(appDir + "/icons/" + icontheme + "/" + "lineweight22" + ".png"), "1.10 mm", 1.10);
    lineweightSelector->addItem(QIcon(appDir + "/icons/" + icontheme + "/" + "lineweight23" + ".png"), "1.15 mm", 1.15);
    lineweightSelector->addItem(QIcon(appDir + "/icons/" + icontheme + "/" + "lineweight24" + ".png"), "1.20 mm", 1.20);
    lineweightSelector->setMinimumContentsLength(8); // Prevent dropdown text readability being squish...d.
    toolbarProperties->addWidget(lineweightSelector);
    connect(lineweightSelector, SIGNAL(currentIndexChanged(int)), this, SLOT(lineweightSelectorIndexChanged(int)));

    connect(toolbarProperties, SIGNAL(topLevelChanged(bool)), this, SLOT(floatingChangedToolBar(bool)));
}

void MainWindow::createTextToolbar()
{
    qDebug("MainWindow createTextToolbar()");

    toolbarText->setObjectName("toolbarText");

    toolbarText->addWidget(textFontSelector);
    textFontSelector->setCurrentFont(QFont(state.settings.text_font));
    connect(textFontSelector, SIGNAL(currentFontChanged(const QFont&)), this, SLOT(textFontSelectorCurrentFontChanged(const QFont&)));

    toolbarText->addAction(actionHash.value(ACTION_textbold));
    actionHash.value(ACTION_textbold)->setChecked(state.settings.text_style_bold);
    toolbarText->addAction(actionHash.value(ACTION_textitalic));
    actionHash.value(ACTION_textitalic)->setChecked(state.settings.text_style_italic);
    toolbarText->addAction(actionHash.value(ACTION_textunderline));
    actionHash.value(ACTION_textunderline)->setChecked(state.settings.text_style_underline);
    toolbarText->addAction(actionHash.value(ACTION_textstrikeout));
    actionHash.value(ACTION_textstrikeout)->setChecked(state.settings.text_style_strikeout);
    toolbarText->addAction(actionHash.value(ACTION_textoverline));
    actionHash.value(ACTION_textoverline)->setChecked(state.settings.text_style_overline);

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
    setTextSize(state.settings.text_size);
    toolbarText->addWidget(textSizeSelector);
    connect(textSizeSelector, SIGNAL(currentIndexChanged(int)), this, SLOT(textSizeSelectorIndexChanged(int)));

    connect(toolbarText, SIGNAL(topLevelChanged(bool)), this, SLOT(floatingChangedToolBar(bool)));
}

void MainWindow::createPromptToolbar()
{
    qDebug("MainWindow createPromptToolbar()");

    toolbarPrompt->setObjectName("toolbarPrompt");
    toolbarPrompt->addWidget(prompt);
    toolbarPrompt->setAllowedAreas(Qt::TopToolBarArea | Qt::BottomToolBarArea);
    connect(toolbarPrompt, SIGNAL(topLevelChanged(bool)), prompt, SLOT(floatingChanged(bool)));
}

void MainWindow::createAllToolbars()
{
    qDebug("MainWindow createAllToolbars()");

    createFileToolbar();
    createEditToolbar();
    createViewToolbar();
    createZoomToolbar();
    createPanToolbar();
    createIconToolbar();
    createHelpToolbar();
    createLayerToolbar();
    createPropertiesToolbar();
    createTextToolbar();
    createPromptToolbar();

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

