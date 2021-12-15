#include "mainwindow.h"
#include "mainwindow-actions.h"

#include "statusbar.h"
#include "statusbar-button.h"

#include "view.h"
#include "cmdprompt.h"

#include "property-editor.h"
#include "undo-editor.h"

#include "native-scripting.h"
#include "native-javascript.h"

#include "preview-dialog.h"

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

MainWindow::MainWindow() : QMainWindow(0)
{
    //Init
    mainWin = this;
    //Menus
    fileMenu     = new QMenu(tr("&File"),     this);
    editMenu     = new QMenu(tr("&Edit"),     this);
    viewMenu     = new QMenu(tr("&View"),     this);
    settingsMenu = new QMenu(tr("&Settings"), this);
    windowMenu   = new QMenu(tr("&Window"),   this);
    helpMenu     = new QMenu(tr("&Help"),     this);
    //SubMenus
    recentMenu   = new QMenu(tr("Open &Recent"), this);
    zoomMenu     = new QMenu(tr("&Zoom"),        this);
    panMenu      = new QMenu(tr("&Pan"),         this);
    //Toolbars
    toolbarFile       = addToolBar(tr("File"));
    toolbarEdit       = addToolBar(tr("Edit"));
    toolbarZoom       = addToolBar(tr("Zoom"));
    toolbarPan        = addToolBar(tr("Pan"));
    toolbarIcon       = addToolBar(tr("Icon"));
    toolbarHelp       = addToolBar(tr("Help"));
    toolbarLayer      = addToolBar(tr("Layer"));
    toolbarProperties = addToolBar(tr("Properties"));
    toolbarPrompt     = addToolBar(tr("Command Prompt"));
    //Selectors
    layerSelector      = new QComboBox(this);
    colorSelector      = new QComboBox(this);
    linetypeSelector   = new QComboBox(this);
    lineweightSelector = new QComboBox(this);

    numOfDocs = 0;
    docIndex = 0;

    readSettings();

    setWindowIcon(QIcon("icons/" + getSettingsGeneralIconTheme() + "/" + "app" + ".png"));

    //create the mdiArea
    QFrame* vbox = new QFrame(this);
    QVBoxLayout* layout = new QVBoxLayout(vbox);
    layout->setMargin(0);
    vbox->setFrameStyle(QFrame::StyledPanel | QFrame::Sunken);
    mdiArea = new MdiArea(this, vbox);
    mdiArea->useBackgroundLogo(getSettingsGeneralMdiBGUseLogo());
    mdiArea->useBackgroundTexture(getSettingsGeneralMdiBGUseTexture());
    mdiArea->useBackgroundColor(getSettingsGeneralMdiBGUseColor());
    mdiArea->setBackgroundLogo(getSettingsGeneralMdiBGLogo());
    mdiArea->setBackgroundTexture(getSettingsGeneralMdiBGTexture());
    mdiArea->setBackgroundColor(QColor(getSettingsGeneralMdiBGColor()));
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

    prompt->setPromptTextColor(QColor(getSettingsPromptTextColor()));
    prompt->setPromptBackgroundColor(QColor(getSettingsPromptBGColor()));

    connect(prompt, SIGNAL(startCommand(const QString&)), this, SLOT(runCommandMain(const QString&)));
    connect(prompt, SIGNAL(runCommand(const QString&, const QString&)), this, SLOT(runCommandPrompt(const QString&, const QString&)));

            connect(prompt, SIGNAL(deletePressed()),    this, SLOT(deletePressed()));
    //TODO: connect(prompt, SIGNAL(tabPressed()),       this, SLOT(someUnknownSlot()));
            connect(prompt, SIGNAL(escapePressed()),    this, SLOT(escapePressed()));
            connect(prompt, SIGNAL(F1Pressed()),        this, SLOT(help()));
    //TODO: connect(prompt, SIGNAL(F2Pressed()),        this, SLOT(floatHistory()));
    //TODO: connect(prompt, SIGNAL(F3Pressed()),        this, SLOT(toggleQSNAP()));
    //TODO: connect(prompt, SIGNAL(F4Pressed()),        this, SLOT(toggleTablet()));
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

    //create the Object Property Editor
    dockPropEdit = new PropertyEditor("icons/" + getSettingsGeneralIconTheme(), prompt, this);
    addDockWidget(Qt::LeftDockWidgetArea, dockPropEdit);

    //create the Command History Undo Editor
    dockUndoEdit = new UndoEditor("icons/" + getSettingsGeneralIconTheme(), prompt, this);
    addDockWidget(Qt::LeftDockWidgetArea, dockUndoEdit);

    //setDockOptions(QMainWindow::AnimatedDocks | QMainWindow::AllowTabbedDocks | QMainWindow::VerticalTabs); //TODO: Load these from settings
    //tabifyDockWidget(dockPropEdit, dockUndoEdit); //TODO: load this from settings

    //Javascript
    initMainWinPointer(this);

    engine = new QScriptEngine(this);
    debugger = new QScriptEngineDebugger(this);
    debugger->attachTo(engine);
    javaInitNatives(engine);

    //Load all commands in a loop
    QDir commandDir("commands");
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

    iconResize(getSettingsGeneralIconSize());
    updateMenuToolbarStatusbar();

    //Show date in statusbar after it has been updated
    QDate date = QDate::currentDate();
    QString datestr = date.toString("MMMM d, yyyy");
    statusbar->showMessage(datestr);

    showNormal();

    //Load tips from external file
    QFile tipFile("tips.txt");
    if(tipFile.open(QFile::ReadOnly))
    {
        QTextStream stream(&tipFile);
        QString tipLine;
        do
        {
            tipLine = stream.readLine();
            if(!tipLine.isEmpty())
                listTipOfTheDay << tipLine;
        }
        while(!tipLine.isNull());
    }
    if(getSettingsGeneralTipOfTheDay())
        tipOfTheDay();
}

MainWindow::~MainWindow()
{
    qDebug("MainWindow::Destructor()");

    //Prevent memory leaks by deleting any unpasted objects
    qDeleteAll(cutCopyObjectList.begin(), cutCopyObjectList.end());
    cutCopyObjectList.clear();
}

QAction* MainWindow::getAction(int actionEnum)
{
    return actionDict.value(actionEnum);
}

void MainWindow::recentMenuAboutToShow()
{
    qDebug("MainWindow::recentMenuAboutToShow()");
    recentMenu->clear();

    QFileInfo recentFileInfo;
    QString recentValue;
    for(int i = 0; i < settings_opensave_recent_list_of_files.size(); ++i)
    {
        //If less than the max amount of entries add to menu
        if(i < settings_opensave_recent_max_files)
        {
            recentFileInfo = QFileInfo(settings_opensave_recent_list_of_files.at(i));
            if(recentFileInfo.exists() && validFileFormat(recentFileInfo.fileName()))
            {
                recentValue.setNum(i+1);
                QAction* rAction;
                if     (recentValue.toInt() >= 1 && recentValue.toInt() <= 9) rAction = new QAction("&" + recentValue + " " + recentFileInfo.fileName(), this);
                else if(recentValue.toInt() == 10)                            rAction = new QAction("1&0 "                  + recentFileInfo.fileName(), this);
                else                                                          rAction = new QAction(      recentValue + " " + recentFileInfo.fileName(), this);
                rAction->setCheckable(false);
                rAction->setData(settings_opensave_recent_list_of_files.at(i));
                recentMenu->addAction(rAction);
                connect(rAction, SIGNAL(triggered()), this, SLOT(openrecentfile()));
            }
        }
    }
    //Ensure the list only has max amount of entries
    while(settings_opensave_recent_list_of_files.size() > settings_opensave_recent_max_files)
    {
        settings_opensave_recent_list_of_files.removeLast();
    }
}

void MainWindow::windowMenuAboutToShow()
{
    qDebug("MainWindow::windowMenuAboutToShow()");
    windowMenu->clear();
    windowMenu->addAction(actionDict.value(ACTION_windowclose));
    windowMenu->addAction(actionDict.value(ACTION_windowcloseall));
    windowMenu->addSeparator();
    windowMenu->addAction(actionDict.value(ACTION_windowcascade));
    windowMenu->addAction(actionDict.value(ACTION_windowtile));
    windowMenu->addSeparator();
    windowMenu->addAction(actionDict.value(ACTION_windownext));
    windowMenu->addAction(actionDict.value(ACTION_windowprevious));

    windowMenu->addSeparator();
    QList<QMdiSubWindow*> windows = mdiArea->subWindowList();
    for(int i = 0; i < windows.count(); ++i)
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
    if(!aSender)
        return;
    QWidget* w = mdiArea->subWindowList().at(aSender->data().toInt());
    if(w && checked)
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

void MainWindow::newfile()
{
    qDebug("MainWindow::newfile()");
    docIndex++;
    numOfDocs++;
    MDIWindow* mdiWin = new MDIWindow(docIndex, mainWin, mdiArea, Qt::SubWindow);
    connect(mdiWin, SIGNAL(sendCloseMdiWin(MDIWindow*)), this, SLOT(onCloseMdiWin(MDIWindow*)));
    connect(mdiArea, SIGNAL(subWindowActivated(QMdiSubWindow*)), this, SLOT(onWindowActivated(QMdiSubWindow*)));

    updateMenuToolbarStatusbar();
    windowMenuAboutToShow();

    View* v = mdiWin->getView();
    if(v)
    {
        v->recalculateLimits();
        v->zoomExtents();
    }
}

void MainWindow::openfile(bool recent, const QString& recentFile)
{
    qDebug("MainWindow::openfile()");

    QApplication::setOverrideCursor(Qt::ArrowCursor);

    QStringList files;
    bool preview = getSettingsOpenThumbnail();
    openFilesPath = settings_opensave_recent_directory;

    //Check to see if this from the recent files list
    if(recent)
    {
        files.append(recentFile);
        openFilesSelected(files);
    }
    else if(!preview)
    {
        //TODO: set getOpenFileNames' selectedFilter parameter from settings_opensave_open_format
        files = QFileDialog::getOpenFileNames(this, tr("Open"), openFilesPath, fileFormatFilterString());
        openFilesSelected(files);
    }
    else if(preview)
    {
        PreviewDialog* openDialog = new PreviewDialog(this, tr("Open w/Preview"), openFilesPath, fileFormatFilterString());
        //TODO: set openDialog->selectNameFilter(const QString& filter) from settings_opensave_open_format
        connect(openDialog, SIGNAL(filesSelected(const QStringList&)), this, SLOT(openFilesSelected(const QStringList&)));
        openDialog->exec();
    }

    QApplication::restoreOverrideCursor();
}

void MainWindow::openFilesSelected(const QStringList& filesToOpen)
{
    bool doOnce = true;

    if(filesToOpen.count())
    {
        for(int i = 0; i < filesToOpen.count(); i++)
        {
            int fileFormat = validFileFormat(filesToOpen[i]);
            if(fileFormat == FILEFORMAT_NULL)
                continue;

            QMdiSubWindow* existing = findMdiWindow(filesToOpen[i]);
            if(existing)
            {
                mdiArea->setActiveSubWindow(existing);
                continue;
            }

            //The docIndex doesn't need increased as it is only used for unnamed files
            numOfDocs++;
            MDIWindow* mdiWin = new MDIWindow(docIndex, mainWin, mdiArea, Qt::SubWindow);
            connect(mdiWin, SIGNAL(sendCloseMdiWin(MDIWindow*)), this, SLOT(onCloseMdiWin(MDIWindow*)));
            connect(mdiArea, SIGNAL(subWindowActivated(QMdiSubWindow*)), this, SLOT(onWindowActivated(QMdiSubWindow*)));

            //Make sure the toolbars/etc... are shown before doing their zoomExtents
            if(doOnce) { updateMenuToolbarStatusbar(); doOnce = false; }

            if(mdiWin->loadFile(filesToOpen.at(i)))
            {
                statusbar->showMessage(tr("File(s) loaded"), 2000);
                mdiWin->show();
                mdiWin->showMaximized();
                //Prevent duplicate entries in the recent files list
                if(!settings_opensave_recent_list_of_files.contains(filesToOpen.at(i), Qt::CaseInsensitive))
                {
                    settings_opensave_recent_list_of_files.prepend(filesToOpen.at(i));
                }
                //Move the recent file to the top of the list
                else
                {
                    settings_opensave_recent_list_of_files.removeAll(filesToOpen.at(i));
                    settings_opensave_recent_list_of_files.prepend(filesToOpen.at(i));
                }
                settings_opensave_recent_directory = QFileInfo(filesToOpen.at(i)).absolutePath();

                View* v = mdiWin->getView();
                if(v)
                {
                    v->recalculateLimits();
                    v->zoomExtents();
                }
            }
            else
            {
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
    if(recentSender)
    {
        openfile(true, recentSender->data().toString());
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
    MDIWindow* win;
    if(!qobject_cast<MDIWindow*>(mdiArea->activeSubWindow()))
        return;

    win = qobject_cast<MDIWindow*>(mdiArea->activeSubWindow());

    QString file;
    openFilesPath = settings_opensave_recent_directory;
    file = QFileDialog::getSaveFileName(this, tr("Save"), openFilesPath, fileFormatFilterString()); //TODO: set selectedFilter from settings_opensave_save_format

    win->saveFile(file);

}

QMdiSubWindow* MainWindow::findMdiWindow(const QString& fileName)
{
    qDebug("MainWindow::findMdiWindow(%s)", qPrintable(fileName));
    QString canonicalFilePath = QFileInfo(fileName).canonicalFilePath();

    foreach(QMdiSubWindow* window, mdiArea->subWindowList())
    {
        MDIWindow* mdiWin = qobject_cast<MDIWindow*>(window);
        if(mdiWin)
        {
            if(mdiWin->getCurrentFile() == canonicalFilePath)
            {
                return window;
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
    MDIWindow* win = qobject_cast<MDIWindow*>(mdiArea->activeSubWindow());
    if(win)
    {
        onCloseMdiWin(win);
    }
}

void MainWindow::onCloseMdiWin(MDIWindow* theMdiWin)
{
    qDebug("MainWindow::onCloseMdiWin()");
    numOfDocs--;

    bool keepMaximized;
    if(theMdiWin) { keepMaximized = theMdiWin->isMaximized(); }

    mdiArea->removeSubWindow(theMdiWin);
    theMdiWin->deleteLater();

    updateMenuToolbarStatusbar();
    windowMenuAboutToShow();

    if(keepMaximized)
    {
        MDIWindow* win = qobject_cast<MDIWindow*>(mdiArea->activeSubWindow());
        if(win) { win->showMaximized(); }
    }
}

void MainWindow::onWindowActivated(QMdiSubWindow* w)
{
    qDebug("MainWindow::onWindowActivated()");
    MDIWindow* win = qobject_cast<MDIWindow*>(w);
    if(win)
        win->onWindowActivated();
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

    actionDict.value(ACTION_print)->setEnabled(numOfDocs > 0);
    actionDict.value(ACTION_windowclose)->setEnabled(numOfDocs > 0);
    actionDict.value(ACTION_designdetails)->setEnabled(numOfDocs > 0);

    if(numOfDocs)
    {
        //Toolbars
        toolbarZoom->show();
        toolbarPan->show();
        toolbarIcon->show();
        toolbarHelp->show();
        toolbarLayer->show();
        toolbarProperties->show();
        toolbarPrompt->show();

        foreach(QToolBar* tb, toolbarHash)
        {
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

        foreach(QMenu* menu, menuHash)
        {
            menuBar()->addMenu(menu);
        }

        menuBar()->addMenu(settingsMenu);
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
    else
    {
        //Toolbars
        toolbarZoom->hide();
        toolbarPan->hide();
        toolbarIcon->hide();
        toolbarHelp->hide();
        toolbarLayer->hide();
        toolbarProperties->hide();
        toolbarPrompt->hide();
        foreach(QToolBar* tb, toolbarHash)
        {
            tb->hide();
        }

        //DockWidgets
        dockPropEdit->hide();
        dockUndoEdit->hide();

        //Menus
        menuBar()->clear();
        menuBar()->addMenu(fileMenu);
        menuBar()->addMenu(editMenu);
        menuBar()->addMenu(settingsMenu);
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

int MainWindow::validFileFormat(const QString& fileName)
{
    int type = FILEFORMAT_NULL;
    if     (fileName.toUpper().endsWith(".100")) type = FILEFORMAT_100;
    else if(fileName.toUpper().endsWith(".10O")) type = FILEFORMAT_10O;
    else if(fileName.toUpper().endsWith(".ART")) type = FILEFORMAT_ART;
    else if(fileName.toUpper().endsWith(".BMC")) type = FILEFORMAT_BMC;
    else if(fileName.toUpper().endsWith(".BRO")) type = FILEFORMAT_BRO;
    else if(fileName.toUpper().endsWith(".CND")) type = FILEFORMAT_CND;
    else if(fileName.toUpper().endsWith(".COL")) type = FILEFORMAT_COL;
    else if(fileName.toUpper().endsWith(".CSD")) type = FILEFORMAT_CSD;
    else if(fileName.toUpper().endsWith(".CSV")) type = FILEFORMAT_CSV;
    else if(fileName.toUpper().endsWith(".DAT")) type = FILEFORMAT_DAT;
    else if(fileName.toUpper().endsWith(".DEM")) type = FILEFORMAT_DEM;
    else if(fileName.toUpper().endsWith(".DSB")) type = FILEFORMAT_DSB;
    else if(fileName.toUpper().endsWith(".DST")) type = FILEFORMAT_DST;
    else if(fileName.toUpper().endsWith(".DSZ")) type = FILEFORMAT_DSZ;
    else if(fileName.toUpper().endsWith(".DXF")) type = FILEFORMAT_DXF;
    else if(fileName.toUpper().endsWith(".EDR")) type = FILEFORMAT_EDR;
    else if(fileName.toUpper().endsWith(".EMD")) type = FILEFORMAT_EMD;
    else if(fileName.toUpper().endsWith(".EXP")) type = FILEFORMAT_EXP;
    else if(fileName.toUpper().endsWith(".EXY")) type = FILEFORMAT_EXY;
    else if(fileName.toUpper().endsWith(".EYS")) type = FILEFORMAT_EYS;
    else if(fileName.toUpper().endsWith(".FXY")) type = FILEFORMAT_FXY;
    else if(fileName.toUpper().endsWith(".GNC")) type = FILEFORMAT_GNC;
    else if(fileName.toUpper().endsWith(".GT"))  type = FILEFORMAT_GT;
    else if(fileName.toUpper().endsWith(".HUS")) type = FILEFORMAT_HUS;
    else if(fileName.toUpper().endsWith(".INB")) type = FILEFORMAT_INB;
    else if(fileName.toUpper().endsWith(".JEF")) type = FILEFORMAT_JEF;
    else if(fileName.toUpper().endsWith(".KSM")) type = FILEFORMAT_KSM;
    else if(fileName.toUpper().endsWith(".PCD")) type = FILEFORMAT_PCD;
    else if(fileName.toUpper().endsWith(".PCM")) type = FILEFORMAT_PCM;
    else if(fileName.toUpper().endsWith(".PCQ")) type = FILEFORMAT_PCQ;
    else if(fileName.toUpper().endsWith(".PCS")) type = FILEFORMAT_PCS;
    else if(fileName.toUpper().endsWith(".PEC")) type = FILEFORMAT_PEC;
    else if(fileName.toUpper().endsWith(".PEL")) type = FILEFORMAT_PEL;
    else if(fileName.toUpper().endsWith(".PEM")) type = FILEFORMAT_PEM;
    else if(fileName.toUpper().endsWith(".PES")) type = FILEFORMAT_PES;
    else if(fileName.toUpper().endsWith(".PHB")) type = FILEFORMAT_PHB;
    else if(fileName.toUpper().endsWith(".PHC")) type = FILEFORMAT_PHC;
    else if(fileName.toUpper().endsWith(".RGB")) type = FILEFORMAT_RGB;
    else if(fileName.toUpper().endsWith(".SEW")) type = FILEFORMAT_SEW;
    else if(fileName.toUpper().endsWith(".SHV")) type = FILEFORMAT_SHV;
    else if(fileName.toUpper().endsWith(".STX")) type = FILEFORMAT_STX;
    else if(fileName.toUpper().endsWith(".SST")) type = FILEFORMAT_SST;
    else if(fileName.toUpper().endsWith(".SVG")) type = FILEFORMAT_SVG;
    else if(fileName.toUpper().endsWith(".T09")) type = FILEFORMAT_T09;
    else if(fileName.toUpper().endsWith(".TAP")) type = FILEFORMAT_TAP;
    else if(fileName.toUpper().endsWith(".THR")) type = FILEFORMAT_THR;
    else if(fileName.toUpper().endsWith(".TXT")) type = FILEFORMAT_TXT;
    else if(fileName.toUpper().endsWith(".U00")) type = FILEFORMAT_U00;
    else if(fileName.toUpper().endsWith(".U01")) type = FILEFORMAT_U01;
    else if(fileName.toUpper().endsWith(".VIP")) type = FILEFORMAT_VIP;
    else if(fileName.toUpper().endsWith(".VP3")) type = FILEFORMAT_VP3;
    else if(fileName.toUpper().endsWith(".XXX")) type = FILEFORMAT_XXX;
    else if(fileName.toUpper().endsWith(".ZSK")) type = FILEFORMAT_ZSK;
    return type;
}

QString MainWindow::fileFormatFilterString()
{
    QString supported = "All Supported Files (*.100 *.10o *.art *.bmc *.bro *.cnd *.col *.csd *.csv *.dem *.dsb *.dst *.dsz *.dxf *.edr *.emd *.exp *.exy *.eys *.fxy *.gnc *.gt *.hus *.inb *.jef *.ksm *.pcd *.pcm *.pcs *.pec *.pel *.pem *.pes *.phb *.phc *.rgb *.sew *.sst *.stx *.svg *.t09 *.tap *.thr *.u00 *.u01 *.vip *.vp3 *.xxx *.zsk);;";
    QString all = "All Files (*);;"   \
    "100 (*.100);;"     \
    "10o (*.10o);;"     \
    "ART (*.art);;"     \
    "BMC (*.bmc);;"     \
    "BRO (*.bro);;"     \
    "CND (*.cnd);;"     \
    "COL (*.col);;"     \
    "CSD (*.csd);;"     \
    "CSV (*.csv);;"     \
    "DAT (*.dat);;"     \
    "DEM (*.dem);;"     \
    "DSB (*.dsb);;"     \
    "DST (*.dst);;"     \
    "DSZ (*.dsz);;"     \
    "DXF (*.dxf);;"     \
    "EDR (*.edr);;"     \
    "EMD (*.emd);;"     \
    "EXP (*.exp);;"     \
    "EXY (*.exy);;"     \
    "EYS (*.eys);;"     \
    "FXY (*.fxy);;"     \
    "GNC (*.gnc);;"     \
    "GT (*.gt);;"       \
    "HUS (*.hus);;"     \
    "INB (*.inb);;"     \
    "JEF (*.jef);;"     \
    "KSM (*.ksm);;"     \
    "PCD (*.pcd);;"     \
    "PCM (*.pcm);;"     \
    "PCQ (*.pcq);;"     \
    "PCS (*.pcs);;"     \
    "PEC (*.pec);;"     \
    "PEL (*.pel);;"     \
    "PEM (*.pem);;"     \
    "PES (*.pes);;"     \
    "PHB (*.phb);;"     \
    "PHC (*.phc);;"     \
    "RGB (*.rgb);;"     \
    "SEW (*.sew);;"     \
    "SHV (*.shv);;"     \
    "SST (*.sst);;"     \
    "STX (*.stx);;"     \
    "SVG (*.svg);;"     \
    "T09 (*.t09);;"     \
    "TAP (*.tap);;"     \
    "THR (*.thr);;"     \
    "TXT (*.txt);;"     \
    "U00 (*.u00);;"     \
    "U01 (*.u01);;"     \
    "VIP (*.vip);;"     \
    "VP3 (*.vp3);;"     \
    "XXX (*.xxx);;"     \
    "ZSK (*.zsk);;";

    QString custom = getSettingsCustomFilter();
    if(custom.contains("supported", Qt::CaseInsensitive))
        custom = ""; //This will hide it
    else if(!custom.contains("*", Qt::CaseInsensitive))
        custom = ""; //This will hide it
    else
        custom = "Custom Filter(" + custom + ");;";

    return tr(qPrintable(custom + supported + all));
}

void MainWindow::closeToolBar(QAction* action)
{
    if(action->objectName() == "toolbarclose")
    {
        QToolBar* tb = qobject_cast<QToolBar*>(sender());
        if(tb)
        {
            qDebug("%s closed.", qPrintable(tb->objectName()));
            tb->hide();
        }
    }
}

void MainWindow::floatingChangedToolBar(bool isFloating)
{
    QToolBar* tb = qobject_cast<QToolBar*>(sender());
    if(tb)
    {
        if(isFloating)
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
            for(int i = 0; i < actList.size(); ++i)
            {
                QAction* ACTION = actList.value(i);
                if(ACTION->objectName() == "toolbarclose")
                {
                    tb->removeAction(ACTION);
                    disconnect(tb, SIGNAL(actionTriggered(QAction*)), this, SLOT(closeToolBar(QAction*)));
                    delete ACTION;
                }
            }
        }
    }
}

/* kate: bom off; indent-mode cstyle; indent-width 4; replace-trailing-space-save on; */
