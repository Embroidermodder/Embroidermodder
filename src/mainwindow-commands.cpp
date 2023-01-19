/*
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

#include "embroidermodder.h"

void MainWindow::stub_implement(QString txt)
{
    qDebug("TODO: %s", qPrintable(txt));
}

void MainWindow::stub_testing()
{
    QMessageBox::warning(this, tr("Testing Feature"), tr("<b>This feature is in testing.</b>"));
}

void MainWindow::exit()
{
    debug_message("exit()");
    if (settings_prompt_save_history) {
        prompt->saveHistory("prompt.log", settings_prompt_save_history_as_html);
        // TODO: get filename from settings
    }
    qApp->closeAllWindows();
    this->deleteLater();
    // Force the MainWindow destructor to run before exiting. Makes Valgrind "still reachable" happy :)
}

void MainWindow::quit()
{
    debug_message("quit()");
    exit();
}

void MainWindow::checkForUpdates()
{
    debug_message("checkForUpdates()");
    //TODO: Check website for new versions, commands, etc...
}

void MainWindow::cut()
{
    debug_message("cut()");
    View* gview = activeView();
    if(gview) { gview->cut(); }
}

void MainWindow::copy()
{
    debug_message("copy()");
    View* gview = activeView();
    if(gview) { gview->copy(); }
}

void MainWindow::paste()
{
    debug_message("paste()");
    View* gview = activeView();
    if(gview) { gview->paste(); }
}

void MainWindow::selectAll()
{
    debug_message("selectAll()");
    View* gview = activeView();
    if(gview) { gview->selectAll(); }
}

QString MainWindow::platformString()
{
    //TODO: Append QSysInfo to string where applicable.
    QString os;
    #if   defined(Q_OS_AIX)
    os = "AIX";
    #elif defined(Q_OS_BSD4)
    os = "BSD 4.4";
    #elif defined(Q_OS_BSDI)
    os = "BSD/OS";
    #elif defined(Q_OS_CYGWIN)
    os = "Cygwin";
    #elif defined(Q_OS_DARWIN)
    os = "Mac OS";
    #elif defined(Q_OS_DGUX)
    os = "DG/UX";
    #elif defined(Q_OS_DYNIX)
    os = "DYNIX/ptx";
    #elif defined(Q_OS_FREEBSD)
    os = "FreeBSD";
    #elif defined(Q_OS_HPUX)
    os = "HP-UX";
    #elif defined(Q_OS_HURD)
    os = "GNU Hurd";
    #elif defined(Q_OS_IRIX)
    os = "SGI Irix";
    #elif defined(Q_OS_LINUX)
    os = "Linux";
    #elif defined(Q_OS_LYNX)
    os = "LynxOS";
    #elif defined(Q_OS_MAC)
    os = "Mac OS";
    #elif defined(Q_OS_MSDOS)
    os = "MS-DOS";
    #elif defined(Q_OS_NETBSD)
    os = "NetBSD";
    #elif defined(Q_OS_OS2)
    os = "OS/2";
    #elif defined(Q_OS_OPENBSD)
    os = "OpenBSD";
    #elif defined(Q_OS_OS2EMX)
    os = "XFree86 on OS/2";
    #elif defined(Q_OS_OSF)
    os = "HP Tru64 UNIX";
    #elif defined(Q_OS_QNX)
    os = "QNX Neutrino";
    #elif defined(Q_OS_RELIANT)
    os = "Reliant UNIX";
    #elif defined(Q_OS_SCO)
    os = "SCO OpenServer 5";
    #elif defined(Q_OS_SOLARIS)
    os = "Sun Solaris";
    #elif defined(Q_OS_SYMBIAN)
    os = "Symbian";
    #elif defined(Q_OS_ULTRIX)
    os = "DEC Ultrix";
    #elif defined(Q_OS_UNIX)
    os = "UNIX BSD/SYSV";
    #elif defined(Q_OS_UNIXWARE)
    os = "UnixWare";
    #elif defined(Q_OS_WIN32)
    os = "Windows";
    #elif defined(Q_OS_WINCE)
    os = "Windows CE";
    #endif
    qDebug("Platform: %s", qPrintable(os));
    return os;
}

void MainWindow::designDetails()
{
    QGraphicsScene* scene = activeScene();
    if(scene)
    {
        EmbDetailsDialog dialog(scene, this);
        dialog.exec();
    }
}

void MainWindow::whatsThisContextHelp()
{
    debug_message("whatsThisContextHelp()");
    QWhatsThis::enterWhatsThisMode();
}

void MainWindow::print()
{
    debug_message("print()");
    MdiWindow* mdiWin = qobject_cast<MdiWindow*>(mdiArea->activeSubWindow());
    if(mdiWin) { mdiWin->print(); }
}

void MainWindow::tipOfTheDay()
{
    debug_message("tipOfTheDay()");

    QString appDir = qApp->applicationDirPath();

    wizardTipOfTheDay = new QWizard(this);
    wizardTipOfTheDay->setAttribute(Qt::WA_DeleteOnClose);
    wizardTipOfTheDay->setWizardStyle(QWizard::ModernStyle);
    wizardTipOfTheDay->setMinimumSize(550, 400);

    QWizardPage* page = new QWizardPage(wizardTipOfTheDay);

    ImageWidget* imgBanner = new ImageWidget(appDir + "/images/did-you-know.png", wizardTipOfTheDay);

    if(settings_general_current_tip >= listTipOfTheDay.size())
        settings_general_current_tip = 0;
    labelTipOfTheDay = new QLabel(listTipOfTheDay.value(settings_general_current_tip), wizardTipOfTheDay);
    labelTipOfTheDay->setWordWrap(true);

    QCheckBox* checkBoxTipOfTheDay = new QCheckBox(tr("&Show tips on startup"), wizardTipOfTheDay);
    settings_general_tip_of_the_day = settings_general_tip_of_the_day;
    checkBoxTipOfTheDay->setChecked(settings_general_tip_of_the_day);
    connect(checkBoxTipOfTheDay, SIGNAL(stateChanged(int)), this, SLOT(checkBoxTipOfTheDayStateChanged(int)));

    QVBoxLayout* layout = new QVBoxLayout(wizardTipOfTheDay);
    layout->addWidget(imgBanner);
    layout->addStrut(1);
    layout->addWidget(labelTipOfTheDay);
    layout->addStretch(1);
    layout->addWidget(checkBoxTipOfTheDay);
    page->setLayout(layout);
    wizardTipOfTheDay->addPage(page);

    wizardTipOfTheDay->setWindowTitle("Tip of the Day");

    //TODO: Add icons to buttons by using wizardTipOfTheDay->setButton(QWizard::CustomButton1, buttonPrevious)
    //TODO: Add icons to buttons by using wizardTipOfTheDay->setButton(QWizard::CustomButton1, buttonNext)
    //TODO: Add icons to buttons by using wizardTipOfTheDay->setButton(QWizard::CustomButton1, buttonClose)
    wizardTipOfTheDay->setButtonText(QWizard::CustomButton1, tr("&Previous"));
    wizardTipOfTheDay->setButtonText(QWizard::CustomButton2, tr("&Next"));
    wizardTipOfTheDay->setButtonText(QWizard::CustomButton3, tr("&Close"));
    wizardTipOfTheDay->setOption(QWizard::HaveCustomButton1, true);
    wizardTipOfTheDay->setOption(QWizard::HaveCustomButton2, true);
    wizardTipOfTheDay->setOption(QWizard::HaveCustomButton3, true);
    connect(wizardTipOfTheDay, SIGNAL(customButtonClicked(int)), this, SLOT(buttonTipOfTheDayClicked(int)));

    QList<QWizard::WizardButton> listTipOfTheDayButtons;
    listTipOfTheDayButtons << QWizard::Stretch << QWizard::CustomButton1 << QWizard::CustomButton2 << QWizard::CustomButton3;
    wizardTipOfTheDay->setButtonLayout(listTipOfTheDayButtons);

    wizardTipOfTheDay->exec();
}

void MainWindow::checkBoxTipOfTheDayStateChanged(int checked)
{
    settings_general_tip_of_the_day = checked;
}

void MainWindow::buttonTipOfTheDayClicked(int button)
{
    qDebug("buttonTipOfTheDayClicked(%d)", button);
    if(button == QWizard::CustomButton1)
    {
        if(settings_general_current_tip > 0)
            settings_general_current_tip--;
        else
            settings_general_current_tip = listTipOfTheDay.size()-1;
        labelTipOfTheDay->setText(listTipOfTheDay.value(settings_general_current_tip));
    }
    else if(button == QWizard::CustomButton2)
    {
        settings_general_current_tip++;
        if(settings_general_current_tip >= listTipOfTheDay.size())
            settings_general_current_tip = 0;
        labelTipOfTheDay->setText(listTipOfTheDay.value(settings_general_current_tip));
    }
    else if(button == QWizard::CustomButton3)
    {
        wizardTipOfTheDay->close();
    }
}

// Standard Slots
void MainWindow::undo()
{
    debug_message("undo()");
    QString prefix = prompt->getPrefix();
    if(dockUndoEdit->canUndo())
    {
        prompt->setPrefix("Undo " + dockUndoEdit->undoText());
        prompt->appendHistory(QString());
        dockUndoEdit->undo();
        prompt->setPrefix(prefix);
    }
    else
    {
        prompt->alert("Nothing to undo");
        prompt->setPrefix(prefix);
    }
}

void MainWindow::redo()
{
    debug_message("redo()");
    QString prefix = prompt->getPrefix();
    if(dockUndoEdit->canRedo())
    {
        prompt->setPrefix("Redo " + dockUndoEdit->redoText());
        prompt->appendHistory(QString());
        dockUndoEdit->redo();
        prompt->setPrefix(prefix);
    }
    else
    {
        prompt->alert("Nothing to redo");
        prompt->setPrefix(prefix);
    }
}

bool MainWindow::isShiftPressed()
{
    return shiftKeyPressedState;
}

void MainWindow::setShiftPressed()
{
    shiftKeyPressedState = true;
}

void MainWindow::setShiftReleased()
{
    shiftKeyPressedState = false;
}

// Icons
void MainWindow::iconResize(int iconSize)
{
    this->setIconSize(QSize(iconSize, iconSize));
    layerSelector->     setIconSize(QSize(iconSize*4, iconSize));
    colorSelector->     setIconSize(QSize(iconSize,   iconSize));
    linetypeSelector->  setIconSize(QSize(iconSize*4, iconSize));
    lineweightSelector->setIconSize(QSize(iconSize*4, iconSize));
    //set the minimum combobox width so the text is always readable
    layerSelector->     setMinimumWidth(iconSize*4);
    colorSelector->     setMinimumWidth(iconSize*2);
    linetypeSelector->  setMinimumWidth(iconSize*4);
    lineweightSelector->setMinimumWidth(iconSize*4);

    //TODO: low-priority: open app with iconSize set to 128. resize the icons to a smaller size.

    settings_general_icon_size = iconSize;
}

void MainWindow::icon16()
{
    debug_message("icon16()");
    iconResize(16);
}

void MainWindow::icon24()
{
    debug_message("icon24()");
    iconResize(24);
}

void MainWindow::icon32()
{
    debug_message("icon32()");
    iconResize(32);
}

void MainWindow::icon48()
{
    debug_message("icon48()");
    iconResize(48);
}

void MainWindow::icon64()
{
    debug_message("icon64()");
    iconResize(64);;
}

void MainWindow::icon128()
{
    debug_message("icon128()");
    iconResize(128);
}

MdiWindow* MainWindow::activeMdiWindow()
{
    debug_message("activeMdiWindow()");
    MdiWindow* mdiWin = qobject_cast<MdiWindow*>(mdiArea->activeSubWindow());
    return mdiWin;
}

View* MainWindow::activeView()
{
    debug_message("activeView()");
    MdiWindow* mdiWin = qobject_cast<MdiWindow*>(mdiArea->activeSubWindow());
    if(mdiWin)
    {
        View* v = mdiWin->getView();
        return v;
    }
    return 0;
}

QGraphicsScene* MainWindow::activeScene()
{
    debug_message("activeScene()");
    MdiWindow* mdiWin = qobject_cast<MdiWindow*>(mdiArea->activeSubWindow());
    if(mdiWin)
    {
        QGraphicsScene* s = mdiWin->getScene();
        return s;
    }
    return 0;
}

QUndoStack* MainWindow::activeUndoStack()
{
    debug_message("activeUndoStack()");
    View* v = activeView();
    if(v)
    {
        QUndoStack* u = v->getUndoStack();
        return u;
    }
    return 0;
}

void MainWindow::setUndoCleanIcon(bool opened)
{
    dockUndoEdit->updateCleanIcon(opened);
}

void MainWindow::updateAllViewScrollBars(bool val)
{
    QList<QMdiSubWindow*> windowList = mdiArea->subWindowList();
    for(int i = 0; i < windowList.count(); ++i)
    {
        MdiWindow* mdiWin = qobject_cast<MdiWindow*>(windowList.at(i));
        if(mdiWin) { mdiWin->showViewScrollBars(val); }
    }
}

void MainWindow::updateAllViewCrossHairColors(unsigned int color)
{
    QList<QMdiSubWindow*> windowList = mdiArea->subWindowList();
    for(int i = 0; i < windowList.count(); ++i)
    {
        MdiWindow* mdiWin = qobject_cast<MdiWindow*>(windowList.at(i));
        if(mdiWin) { mdiWin->setViewCrossHairColor(color); }
    }
}

void MainWindow::updateAllViewBackgroundColors(unsigned int color)
{
    QList<QMdiSubWindow*> windowList = mdiArea->subWindowList();
    for(int i = 0; i < windowList.count(); ++i)
    {
        MdiWindow* mdiWin = qobject_cast<MdiWindow*>(windowList.at(i));
        if(mdiWin) { mdiWin->setViewBackgroundColor(color); }
    }
}

void MainWindow::updateAllViewSelectBoxColors(unsigned int colorL, unsigned int fillL, unsigned int colorR, unsigned int fillR, int alpha)
{
    QList<QMdiSubWindow*> windowList = mdiArea->subWindowList();
    for(int i = 0; i < windowList.count(); ++i)
    {
        MdiWindow* mdiWin = qobject_cast<MdiWindow*>(windowList.at(i));
        if(mdiWin) { mdiWin->setViewSelectBoxColors(colorL, fillL, colorR, fillR, alpha); }
    }
}

void MainWindow::updateAllViewGridColors(unsigned int color)
{
    QList<QMdiSubWindow*> windowList = mdiArea->subWindowList();
    for(int i = 0; i < windowList.count(); ++i)
    {
        MdiWindow* mdiWin = qobject_cast<MdiWindow*>(windowList.at(i));
        if(mdiWin) { mdiWin->setViewGridColor(color); }
    }
}

void MainWindow::updateAllViewRulerColors(unsigned int color)
{
    QList<QMdiSubWindow*> windowList = mdiArea->subWindowList();
    for(int i = 0; i < windowList.count(); ++i)
    {
        MdiWindow* mdiWin = qobject_cast<MdiWindow*>(windowList.at(i));
        if(mdiWin) { mdiWin->setViewRulerColor(color); }
    }
}

void MainWindow::updatePickAddMode(bool val)
{
    settings_selection_mode_pickadd = val;
    dockPropEdit->updatePickAddModeButton(val);
}

void MainWindow::pickAddModeToggled()
{
    bool val = !settings_selection_mode_pickadd;
    updatePickAddMode(val);
}

// Layer ToolBar
void MainWindow::makeLayerActive()
{
    debug_message("makeLayerActive()");
    stub_implement("Implement makeLayerActive.");
}

void MainWindow::layerManager()
{
    debug_message("layerManager()");
    stub_implement("Implement layerManager.");
    LayerManager layman(this, this);
    layman.exec();
}

void MainWindow::layerPrevious()
{
    debug_message("layerPrevious()");
    stub_implement("Implement layerPrevious.");
}

// Zoom ToolBar
void MainWindow::zoomRealtime()
{
    debug_message("zoomRealtime()");
    stub_implement("Implement zoomRealtime.");
}

void MainWindow::zoomPrevious()
{
    debug_message("zoomPrevious()");
    stub_implement("Implement zoomPrevious.");
}

void MainWindow::zoomWindow()
{
    debug_message("zoomWindow()");
    View* gview = activeView();
    if(gview) { gview->zoomWindow(); }
}

void MainWindow::zoomDynamic()
{
    debug_message("zoomDynamic()");
    stub_implement("Implement zoomDynamic.");
}

void MainWindow::zoomScale()
{
    debug_message("zoomScale()");
    stub_implement("Implement zoomScale.");
}

void MainWindow::zoomCenter()
{
    debug_message("zoomCenter()");
    stub_implement("Implement zoomCenter.");
}

void MainWindow::zoomIn()
{
    debug_message("zoomIn()");
    View* gview = activeView();
    if(gview) { gview->zoomIn(); }
}

void MainWindow::zoomOut()
{
    debug_message("zoomOut()");
    View* gview = activeView();
    if(gview) { gview->zoomOut(); }
}

void MainWindow::zoomSelected()
{
    debug_message("zoomSelected()");
    View* gview = activeView();
    QUndoStack* stack = gview->getUndoStack();
    if(gview && stack)
    {
        UndoableNavCommand* cmd = new UndoableNavCommand("ZoomSelected", gview, 0);
        stack->push(cmd);
    }
}

void MainWindow::zoomAll()
{
    debug_message("zoomAll()");
    stub_implement("Implement zoomAll.");
}

void MainWindow::zoomExtents()
{
    debug_message("zoomExtents()");
    View* gview = activeView();
    QUndoStack* stack = gview->getUndoStack();
    if(gview && stack)
    {
        UndoableNavCommand* cmd = new UndoableNavCommand("ZoomExtents", gview, 0);
        stack->push(cmd);
    }
}
// Pan SubMenu
void MainWindow::panrealtime()
{
    debug_message("panrealtime()");
    View* gview = activeView();
    if(gview) { gview->panRealTime(); }
}

void MainWindow::panpoint()
{
    debug_message("panpoint()");
    View* gview = activeView();
    if(gview) { gview->panPoint(); }
}

void MainWindow::panLeft()
{
    debug_message("panLeft()");
    View* gview = activeView();
    QUndoStack* stack = gview->getUndoStack();
    if(gview && stack)
    {
        UndoableNavCommand* cmd = new UndoableNavCommand("PanLeft", gview, 0);
        stack->push(cmd);
    }
}

void MainWindow::panRight()
{
    debug_message("panRight()");
    View* gview = activeView();
    QUndoStack* stack = gview->getUndoStack();
    if(gview && stack)
    {
        UndoableNavCommand* cmd = new UndoableNavCommand("PanRight", gview, 0);
        stack->push(cmd);
    }
}

void MainWindow::panUp()
{
    debug_message("panUp()");
    View* gview = activeView();
    QUndoStack* stack = gview->getUndoStack();
    if(gview && stack)
    {
        UndoableNavCommand* cmd = new UndoableNavCommand("PanUp", gview, 0);
        stack->push(cmd);
    }
}

void MainWindow::panDown()
{
    debug_message("panDown()");
    View* gview = activeView();
    QUndoStack* stack = gview->getUndoStack();
    if(gview && stack)
    {
        UndoableNavCommand* cmd = new UndoableNavCommand("PanDown", gview, 0);
        stack->push(cmd);
    }
}

void MainWindow::dayVision()
{
    View* gview = activeView();
    if(gview)
    {
        gview->setBackgroundColor(qRgb(255,255,255)); //TODO: Make day vision color settings.
        gview->setCrossHairColor(qRgb(0,0,0));        //TODO: Make day vision color settings.
        gview->setGridColor(qRgb(0,0,0));             //TODO: Make day vision color settings.
    }
}

void MainWindow::nightVision()
{
    View* gview = activeView();
    if(gview)
    {
        gview->setBackgroundColor(qRgb(0,0,0));      //TODO: Make night vision color settings.
        gview->setCrossHairColor(qRgb(255,255,255)); //TODO: Make night vision color settings.
        gview->setGridColor(qRgb(255,255,255));      //TODO: Make night vision color settings.
    }
}

void MainWindow::doNothing()
{
    //This function intentionally does nothing.
    debug_message("doNothing()");
}

void MainWindow::layerSelectorIndexChanged(int index)
{
    qDebug("layerSelectorIndexChanged(%d)", index);
}

void MainWindow::colorSelectorIndexChanged(int index)
{
    qDebug("colorSelectorIndexChanged(%d)", index);

    QComboBox* comboBox = qobject_cast<QComboBox*>(sender());
    unsigned int newColor;
    if(comboBox)
    {
        bool ok = 0;
        //TODO: Handle ByLayer and ByBlock and Other...
        newColor = comboBox->itemData(index).toUInt(&ok);
        if(!ok)
            QMessageBox::warning(this, tr("Color Selector Conversion Error"), tr("<b>An error has occured while changing colors.</b>"));
    }
    else
        QMessageBox::warning(this, tr("Color Selector Pointer Error"), tr("<b>An error has occured while changing colors.</b>"));

    MdiWindow* mdiWin = qobject_cast<MdiWindow*>(mdiArea->activeSubWindow());
    if(mdiWin) { mdiWin->currentColorChanged(newColor); }
}

void MainWindow::linetypeSelectorIndexChanged(int index)
{
    qDebug("linetypeSelectorIndexChanged(%d)", index);
}

void MainWindow::lineweightSelectorIndexChanged(int index)
{
    qDebug("lineweightSelectorIndexChanged(%d)", index);
}

void MainWindow::textFontSelectorCurrentFontChanged(const QFont& font)
{
    debug_message("textFontSelectorCurrentFontChanged()");
    setTextFont(font.family());
}

void MainWindow::textSizeSelectorIndexChanged(int index)
{
    qDebug("textSizeSelectorIndexChanged(%d)", index);
    settings_text_size = qFabs(textSizeSelector->itemData(index).toReal()); //TODO: check that the toReal() conversion is ok
}

QString MainWindow::textFont()
{
    return settings_text_font;
}

qreal MainWindow::textSize()
{
    return settings_text_size;
}

qreal MainWindow::textAngle()
{
    return settings_text_angle;
}

bool MainWindow::textBold()
{
    return settings_text_style_bold;
}

bool MainWindow::textItalic()
{
    return settings_text_style_italic;
}

bool MainWindow::textUnderline()
{
    return settings_text_style_underline;
}

bool MainWindow::textStrikeOut()
{
    return settings_text_style_strikeout;
}

bool MainWindow::textOverline()
{
    return settings_text_style_overline;
}

void MainWindow::setTextFont(const QString& str)
{
    textFontSelector->setCurrentFont(QFont(str));
    settings_text_font = str;
}

void MainWindow::setTextSize(qreal num)
{
    settings_text_size = qFabs(num);
    int index = textSizeSelector->findText("Custom", Qt::MatchContains);
    if(index != -1)
        textSizeSelector->removeItem(index);
    textSizeSelector->addItem("Custom " + QString().setNum(num, 'f', 2) + " pt", num);
    index = textSizeSelector->findText("Custom", Qt::MatchContains);
    if(index != -1)
        textSizeSelector->setCurrentIndex(index);
}

void MainWindow::setTextAngle(qreal num)
{
    settings_text_angle = num;
}

void MainWindow::setTextBold(bool val)
{
    settings_text_style_bold = val;
}

void MainWindow::setTextItalic(bool val)
{
    settings_text_style_italic = val;
}

void MainWindow::setTextUnderline(bool val)
{
    settings_text_style_underline = val;
}

void MainWindow::setTextStrikeOut(bool val)
{
    settings_text_style_strikeout = val;
}

void MainWindow::setTextOverline(bool val)
{
    settings_text_style_overline = val;
}

QString MainWindow::getCurrentLayer()
{
    MdiWindow* mdiWin = qobject_cast<MdiWindow*>(mdiArea->activeSubWindow());
    if(mdiWin) { return mdiWin->getCurrentLayer(); }
    return "0";
}

unsigned int MainWindow::getCurrentColor()
{
    MdiWindow* mdiWin = qobject_cast<MdiWindow*>(mdiArea->activeSubWindow());
    if(mdiWin) { return mdiWin->getCurrentColor(); }
    return 0; //TODO: return color ByLayer
}

QString MainWindow::getCurrentLineType()
{
    MdiWindow* mdiWin = qobject_cast<MdiWindow*>(mdiArea->activeSubWindow());
    if(mdiWin) { return mdiWin->getCurrentLineType(); }
    return "ByLayer";
}

QString MainWindow::getCurrentLineWeight()
{
    MdiWindow* mdiWin = qobject_cast<MdiWindow*>(mdiArea->activeSubWindow());
    if(mdiWin) { return mdiWin->getCurrentLineWeight(); }
    return "ByLayer";
}

void MainWindow::deletePressed()
{
    debug_message("deletePressed()");
    QApplication::setOverrideCursor(Qt::WaitCursor);
    MdiWindow* mdiWin = qobject_cast<MdiWindow*>(mdiArea->activeSubWindow());
    if(mdiWin) { mdiWin->deletePressed(); }
    QApplication::restoreOverrideCursor();
}

void MainWindow::escapePressed()
{
    debug_message("escapePressed()");
    QApplication::setOverrideCursor(Qt::WaitCursor);
    MdiWindow* mdiWin = qobject_cast<MdiWindow*>(mdiArea->activeSubWindow());
    if(mdiWin) { mdiWin->escapePressed(); }
    QApplication::restoreOverrideCursor();

    nativeEndCommand();
}

void MainWindow::toggleGrid()
{
    debug_message("toggleGrid()");
    statusbar->statusBarGridButton->toggle();
}

void MainWindow::toggleRuler()
{
    debug_message("toggleRuler()");
    statusbar->statusBarRulerButton->toggle();
}

void MainWindow::toggleLwt()
{
    debug_message("toggleLwt()");
    statusbar->statusBarLwtButton->toggle();
}

void MainWindow::enablePromptRapidFire()
{
    prompt->enableRapidFire();
}

void MainWindow::disablePromptRapidFire()
{
    prompt->disableRapidFire();
}

void MainWindow::enableMoveRapidFire()
{
    View* gview = activeView();
    if(gview) gview->enableMoveRapidFire();
}

void MainWindow::disableMoveRapidFire()
{
    View* gview = activeView();
    if(gview) gview->disableMoveRapidFire();
}

void MainWindow::promptHistoryAppended(const QString& txt)
{
    MdiWindow* mdiWin = activeMdiWindow();
    if(mdiWin) mdiWin->promptHistoryAppended(txt);
}

void MainWindow::logPromptInput(const QString& txt)
{
    MdiWindow* mdiWin = activeMdiWindow();
    if(mdiWin) mdiWin->logPromptInput(txt);
}

void MainWindow::promptInputPrevious()
{
    MdiWindow* mdiWin = activeMdiWindow();
    if(mdiWin) mdiWin->promptInputPrevious();
}

void MainWindow::promptInputNext()
{
    MdiWindow* mdiWin = activeMdiWindow();
    if(mdiWin) mdiWin->promptInputNext();
}

void MainWindow::runCommand()
{
    /* TODO. */
    QAction* act = qobject_cast<QAction*>(sender());
    if (act) {
        qDebug("runCommand(%s)", qPrintable(act->objectName()));
        prompt->endCommand();
        prompt->setCurrentText(act->objectName());
        prompt->processInput(Qt::Key_Return);
    }
}

void MainWindow::runCommandMain(const QString& cmd)
{
    qDebug("runCommandMain(%s)", qPrintable(cmd));
    QString fileName = "commands/" + cmd + "/" + cmd + ".js";
    // if(!getSettingsSelectionModePickFirst()) { nativeClearSelection(); } //TODO: Uncomment this line when post-selection is available
    // engine->evaluate(cmd + "_main()", fileName);
}

void MainWindow::runCommandClick(const QString& cmd, qreal x, qreal y)
{
    qDebug("runCommandClick(%s, %.2f, %.2f)", qPrintable(cmd), x, y);
    QString fileName = "commands/" + cmd + "/" + cmd + ".js";
    // engine->evaluate(cmd + "_click(" + QString().setNum(x) + "," + QString().setNum(-y) + ")", fileName);
}

void MainWindow::runCommandMove(const QString& cmd, qreal x, qreal y)
{
    qDebug("runCommandMove(%s, %.2f, %.2f)", qPrintable(cmd), x, y);
    QString fileName = "commands/" + cmd + "/" + cmd + ".js";
    // engine->evaluate(cmd + "_move(" + QString().setNum(x) + "," + QString().setNum(-y) + ")", fileName);
}

void MainWindow::runCommandContext(const QString& cmd, const QString& str)
{
    qDebug("runCommandContext(%s, %s)", qPrintable(cmd), qPrintable(str));
    QString fileName = "commands/" + cmd + "/" + cmd + ".js";
    // engine->evaluate(cmd + "_context('" + str.toUpper() + "')", fileName);
}

void MainWindow::runCommandPrompt(const QString& cmd, const QString& str)
{
    qDebug("runCommandPrompt(%s, %s)", qPrintable(cmd), qPrintable(str));
    QString fileName = "commands/" + cmd + "/" + cmd + ".js";
    //NOTE: Replace any special characters that will cause a syntax error
    QString safeStr = str;
    safeStr.replace("\\", "\\\\");
    safeStr.replace("\'", "\\\'");
    /*
    if(prompt->isRapidFireEnabled()) { engine->evaluate(cmd + "_prompt('" + safeStr + "')", fileName); }
    else                             { engine->evaluate(cmd + "_prompt('" + safeStr.toUpper() + "')", fileName); }
    */
}

void MainWindow::nativeAlert(const QString& txt)
{
    prompt->alert(txt);
}

void MainWindow::nativeBlinkPrompt()
{
    prompt->startBlinking();
}

void MainWindow::nativeSetPromptPrefix(const QString& txt)
{
    prompt->setPrefix(txt);
}

void MainWindow::nativeAppendPromptHistory(const QString& txt)
{
    prompt->appendHistory(txt);
}

void MainWindow::nativeEnablePromptRapidFire()
{
    enablePromptRapidFire();
}

void MainWindow::nativeDisablePromptRapidFire()
{
    disablePromptRapidFire();
}

void MainWindow::nativeEnableMoveRapidFire()
{
    enableMoveRapidFire();
}

void MainWindow::nativeDisableMoveRapidFire()
{
    disableMoveRapidFire();
}

void MainWindow::nativeInitCommand()
{
    View* gview = activeView();
    if(gview) gview->clearRubberRoom();
}

void MainWindow::nativeEndCommand()
{
    View* gview = activeView();
    if(gview)
    {
        gview->clearRubberRoom();
        gview->previewOff();
        gview->disableMoveRapidFire();
    }
    prompt->endCommand();
}

void MainWindow::nativeExit()
{
    exit();
}

void MainWindow::nativeHelp()
{
    help();
}

void MainWindow::nativeAbout()
{
    about();
}

void MainWindow::nativeTipOfTheDay()
{
    tipOfTheDay();
}

void MainWindow::nativeWindowCascade()
{
    mdiArea->cascade();
}

void MainWindow::nativeWindowTile()
{
    mdiArea->tile();
}

void MainWindow::nativeWindowClose()
{
    onCloseWindow();
}

void MainWindow::nativeWindowCloseAll()
{
    mdiArea->closeAllSubWindows();
}

void MainWindow::nativeWindowNext()
{
    mdiArea->activateNextSubWindow();
}

void MainWindow::nativeWindowPrevious()
{
    mdiArea->activatePreviousSubWindow();
}

QString MainWindow::nativePlatformString()
{
    return platformString();
}

void MainWindow::nativeMessageBox(const QString& type, const QString& title, const QString& text)
{
    QString msgType = type.toLower();
    if     (msgType == "critical")    { QMessageBox::critical   (this, tr(qPrintable(title)), tr(qPrintable(text))); }
    else if(msgType == "information") { QMessageBox::information(this, tr(qPrintable(title)), tr(qPrintable(text))); }
    else if(msgType == "question")    { QMessageBox::question   (this, tr(qPrintable(title)), tr(qPrintable(text))); }
    else if(msgType == "warning")     { QMessageBox::warning    (this, tr(qPrintable(title)), tr(qPrintable(text))); }
    else                              { QMessageBox::critical   (this, tr("Native MessageBox Error"), tr("Incorrect use of the native messageBox function.")); }
}

void MainWindow::nativePrintArea(qreal x, qreal y, qreal w, qreal h)
{
    qDebug("nativePrintArea(%.2f, %.2f, %.2f, %.2f)", x, y, w, h);
    //TODO: Print Setup Stuff
    print();
}

void MainWindow::nativeDayVision()
{
    dayVision();
}

void MainWindow::nativeNightVision()
{
    nightVision();
}

void MainWindow::nativeSetBackgroundColor(quint8 r, quint8 g, quint8 b)
{
    settings_display_bg_color = qRgb(r,g,b);
    updateAllViewBackgroundColors(qRgb(r,g,b));
}

void MainWindow::nativeSetCrossHairColor(quint8 r, quint8 g, quint8 b)
{
    settings_display_crosshair_color = qRgb(r,g,b);
    updateAllViewCrossHairColors(qRgb(r,g,b));
}

void MainWindow::nativeSetGridColor(quint8 r, quint8 g, quint8 b)
{
    settings_grid_color = qRgb(r,g,b);
    updateAllViewGridColors(qRgb(r,g,b));
}

QString MainWindow::nativeTextFont()
{
    return textFont();
}

qreal MainWindow::nativeTextSize()
{
    return textSize();
}

qreal MainWindow::nativeTextAngle()
{
    return textAngle();
}

bool MainWindow::nativeTextBold()
{
    return textBold();
}

bool MainWindow::nativeTextItalic()
{
    return textItalic();
}

bool MainWindow::nativeTextUnderline()
{
    return textUnderline();
}

bool MainWindow::nativeTextStrikeOut()
{
    return textStrikeOut();
}

bool MainWindow::nativeTextOverline()
{
    return textOverline();
}

void MainWindow::nativeSetTextFont(const QString& str)
{
    setTextFont(str);
}

void MainWindow::nativeSetTextSize(qreal num)
{
    setTextSize(num);
}

void MainWindow::nativeSetTextAngle(qreal num)
{
    setTextAngle(num);
}

void MainWindow::nativeSetTextBold(bool val)
{
    setTextBold(val);
}

void MainWindow::nativeSetTextItalic(bool val)
{
    setTextItalic(val);
}

void MainWindow::nativeSetTextUnderline(bool val)
{
    setTextUnderline(val);
}

void MainWindow::nativeSetTextStrikeOut(bool val)
{
    setTextStrikeOut(val);
}

void MainWindow::nativeSetTextOverline(bool val)
{
    setTextOverline(val);
}

void MainWindow::nativePreviewOn(int clone, int mode, qreal x, qreal y, qreal data)
{
    View* gview = activeView();
    if(gview) gview->previewOn(clone, mode, x, -y, data);
}

void MainWindow::nativePreviewOff()
{
    View* gview = activeView();
    if(gview) gview->previewOff();
}

void MainWindow::nativeVulcanize()
{
    View* gview = activeView();
    if(gview) gview->vulcanizeRubberRoom();
}

void MainWindow::nativeClearRubber()
{
    View* gview = activeView();
    if(gview) gview->clearRubberRoom();
}

bool MainWindow::nativeAllowRubber()
{
    View* gview = activeView();
    if(gview) return gview->allowRubber();
    return false;
}

void MainWindow::nativeSpareRubber(qint64 id)
{
    View* gview = activeView();
    if(gview) gview->spareRubber(id);
}

void MainWindow::nativeSetRubberMode(int mode)
{
    View* gview = activeView();
    if(gview) gview->setRubberMode(mode);
}

void MainWindow::nativeSetRubberPoint(const QString& key, qreal x, qreal y)
{
    View* gview = activeView();
    if(gview) gview->setRubberPoint(key, QPointF(x, -y));
}

void MainWindow::nativeSetRubberText(const QString& key, const QString& txt)
{
    View* gview = activeView();
    if(gview) gview->setRubberText(key, txt);
}

void MainWindow::nativeAddTextMulti(const QString& str, qreal x, qreal y, qreal rot, bool fill, int rubberMode)
{
}

void MainWindow::nativeAddTextSingle(const QString& str, qreal x, qreal y, qreal rot, bool fill, int rubberMode)
{
    View* gview = activeView();
    QGraphicsScene* gscene = gview->scene();
    QUndoStack* stack = gview->getUndoStack();
    if(gview && gscene && stack)
    {
        TextSingleObject* obj = new TextSingleObject(str, x, -y, getCurrentColor());
        obj->setObjectTextFont(settings_text_font);
        obj->setObjectTextSize(settings_text_size);
        obj->setObjectTextStyle(settings_text_style_bold,
                                settings_text_style_italic,
                                settings_text_style_underline,
                                settings_text_style_strikeout,
                                settings_text_style_overline);
        obj->setObjectTextBackward(false);
        obj->setObjectTextUpsideDown(false);
        obj->setRotation(-rot);
        //TODO: single line text fill
        obj->setObjectRubberMode(rubberMode);
        if(rubberMode)
        {
            gview->addToRubberRoom(obj);
            gscene->addItem(obj);
            gscene->update();
        }
        else
        {
            UndoableAddCommand* cmd = new UndoableAddCommand(obj->data(OBJ_NAME).toString(), obj, gview, 0);
            stack->push(cmd);
        }
    }
}

void MainWindow::nativeAddInfiniteLine(qreal x1, qreal y1, qreal x2, qreal y2, qreal rot)
{
}

void MainWindow::nativeAddRay(qreal x1, qreal y1, qreal x2, qreal y2, qreal rot)
{
}

void MainWindow::nativeAddLine(qreal x1, qreal y1, qreal x2, qreal y2, qreal rot, int rubberMode)
{
    View* gview = activeView();
    QGraphicsScene* gscene = gview->scene();
    QUndoStack* stack = gview->getUndoStack();
    if(gview && gscene && stack)
    {
        LineObject* obj = new LineObject(x1, -y1, x2, -y2, getCurrentColor());
        obj->setRotation(-rot);
        obj->setObjectRubberMode(rubberMode);
        if(rubberMode)
        {
            gview->addToRubberRoom(obj);
            gscene->addItem(obj);
            gscene->update();
        }
        else
        {
            UndoableAddCommand* cmd = new UndoableAddCommand(obj->data(OBJ_NAME).toString(), obj, gview, 0);
            stack->push(cmd);
        }
    }
}

void MainWindow::nativeAddTriangle(qreal x1, qreal y1, qreal x2, qreal y2, qreal x3, qreal y3, qreal rot, bool fill)
{
}

void MainWindow::nativeAddRectangle(qreal x, qreal y, qreal w, qreal h, qreal rot, bool fill, int rubberMode)
{
    View* gview = activeView();
    QGraphicsScene* gscene = gview->scene();
    QUndoStack* stack = gview->getUndoStack();
    if(gview && gscene && stack)
    {
        RectObject* obj = new RectObject(x, -y, w, -h, getCurrentColor());
        obj->setRotation(-rot);
        obj->setObjectRubberMode(rubberMode);
        //TODO: rect fill
        if(rubberMode)
        {
            gview->addToRubberRoom(obj);
            gscene->addItem(obj);
            gscene->update();
        }
        else
        {
            UndoableAddCommand* cmd = new UndoableAddCommand(obj->data(OBJ_NAME).toString(), obj, gview, 0);
            stack->push(cmd);
        }
    }
}

void MainWindow::nativeAddRoundedRectangle(qreal x, qreal y, qreal w, qreal h, qreal rad, qreal rot, bool fill)
{
}

void MainWindow::nativeAddArc(qreal startX, qreal startY, qreal midX, qreal midY, qreal endX, qreal endY, int rubberMode)
{
    View* gview = activeView();
    QGraphicsScene* scene = activeScene();
    if(gview && scene)
    {
        ArcObject* arcObj = new ArcObject(startX, -startY, midX, -midY, endX, -endY, getCurrentColor());
        arcObj->setObjectRubberMode(rubberMode);
        if(rubberMode) gview->addToRubberRoom(arcObj);
        scene->addItem(arcObj);
        scene->update();
    }
}

void MainWindow::nativeAddCircle(qreal centerX, qreal centerY, qreal radius, bool fill, int rubberMode)
{
    View* gview = activeView();
    QGraphicsScene* gscene = gview->scene();
    QUndoStack* stack = gview->getUndoStack();
    if(gview && gscene && stack)
    {
        CircleObject* obj = new CircleObject(centerX, -centerY, radius, getCurrentColor());
        obj->setObjectRubberMode(rubberMode);
        //TODO: circle fill
        if(rubberMode)
        {
            gview->addToRubberRoom(obj);
            gscene->addItem(obj);
            gscene->update();
        }
        else
        {
            UndoableAddCommand* cmd = new UndoableAddCommand(obj->data(OBJ_NAME).toString(), obj, gview, 0);
            stack->push(cmd);
        }
    }
}

void MainWindow::nativeAddSlot(qreal centerX, qreal centerY, qreal diameter, qreal length, qreal rot, bool fill, int rubberMode)
{
    //TODO: Use UndoableAddCommand for slots
    /*
    SlotObject* slotObj = new SlotObject(centerX, -centerY, diameter, length, getCurrentColor());
    slotObj->setRotation(-rot);
    slotObj->setObjectRubberMode(rubberMode);
    if(rubberMode) gview->addToRubberRoom(slotObj);
    scene->addItem(slotObj);
    //TODO: slot fill
    scene->update();
    */
}

void MainWindow::nativeAddEllipse(qreal centerX, qreal centerY, qreal width, qreal height, qreal rot, bool fill, int rubberMode)
{
    View* gview = activeView();
    QGraphicsScene* gscene = gview->scene();
    QUndoStack* stack = gview->getUndoStack();
    if(gview && gscene && stack)
    {
        EllipseObject* obj = new EllipseObject(centerX, -centerY, width, height, getCurrentColor());
        obj->setRotation(-rot);
        obj->setObjectRubberMode(rubberMode);
        //TODO: ellipse fill
        if(rubberMode)
        {
            gview->addToRubberRoom(obj);
            gscene->addItem(obj);
            gscene->update();
        }
        else
        {
            UndoableAddCommand* cmd = new UndoableAddCommand(obj->data(OBJ_NAME).toString(), obj, gview, 0);
            stack->push(cmd);
        }
    }
}

void MainWindow::nativeAddPoint(qreal x, qreal y)
{
    View* gview = activeView();
    QUndoStack* stack = gview->getUndoStack();
    if(gview && stack)
    {
        PointObject* obj = new PointObject(x, -y, getCurrentColor());
        UndoableAddCommand* cmd = new UndoableAddCommand(obj->data(OBJ_NAME).toString(), obj, gview, 0);
        stack->push(cmd);
    }
}

void MainWindow::nativeAddRegularPolygon(qreal centerX, qreal centerY, quint16 sides, quint8 mode, qreal rad, qreal rot, bool fill)
{
}

//NOTE: This native is different than the rest in that the Y+ is down (scripters need not worry about this)
void MainWindow::nativeAddPolygon(qreal startX, qreal startY, const QPainterPath& p, int rubberMode)
{
    View* gview = activeView();
    QGraphicsScene* gscene = gview->scene();
    QUndoStack* stack = gview->getUndoStack();
    if(gview && gscene && stack)
    {
        PolygonObject* obj = new PolygonObject(startX, startY, p, getCurrentColor());
        obj->setObjectRubberMode(rubberMode);
        if(rubberMode)
        {
            gview->addToRubberRoom(obj);
            gscene->addItem(obj);
            gscene->update();
        }
        else
        {
            UndoableAddCommand* cmd = new UndoableAddCommand(obj->data(OBJ_NAME).toString(), obj, gview, 0);
            stack->push(cmd);
        }
    }
}

//NOTE: This native is different than the rest in that the Y+ is down (scripters need not worry about this)
void MainWindow::nativeAddPolyline(qreal startX, qreal startY, const QPainterPath& p, int rubberMode)
{
    View* gview = activeView();
    QGraphicsScene* gscene = gview->scene();
    QUndoStack* stack = gview->getUndoStack();
    if(gview && gscene && stack)
    {
        PolylineObject* obj = new PolylineObject(startX, startY, p, getCurrentColor());
        obj->setObjectRubberMode(rubberMode);
        if(rubberMode)
        {
            gview->addToRubberRoom(obj);
            gscene->addItem(obj);
            gscene->update();
        }
        else
        {
            UndoableAddCommand* cmd = new UndoableAddCommand(obj->data(OBJ_NAME).toString(), obj, gview, 0);
            stack->push(cmd);
        }
    }
}

//NOTE: This native is different than the rest in that the Y+ is down (scripters need not worry about this)
void MainWindow::nativeAddPath(qreal startX, qreal startY, const QPainterPath& p, int rubberMode)
{
}

void MainWindow::nativeAddHorizontalDimension(qreal x1, qreal y1, qreal x2, qreal y2, qreal legHeight)
{
}

void MainWindow::nativeAddVerticalDimension(qreal x1, qreal y1, qreal x2, qreal y2, qreal legHeight)
{
}

void MainWindow::nativeAddImage(const QString& img, qreal x, qreal y, qreal w, qreal h, qreal rot)
{
}

void MainWindow::nativeAddDimLeader(qreal x1, qreal y1, qreal x2, qreal y2, qreal rot, int rubberMode)
{
    View* gview = activeView();
    QGraphicsScene* gscene = gview->scene();
    QUndoStack* stack = gview->getUndoStack();
    if(gview && gscene && stack)
    {
        DimLeaderObject* obj = new DimLeaderObject(x1, -y1, x2, -y2, getCurrentColor());
        obj->setRotation(-rot);
        obj->setObjectRubberMode(rubberMode);
        if(rubberMode)
        {
            gview->addToRubberRoom(obj);
            gscene->addItem(obj);
            gscene->update();
        }
        else
        {
            UndoableAddCommand* cmd = new UndoableAddCommand(obj->data(OBJ_NAME).toString(), obj, gview, 0);
            stack->push(cmd);
        }
    }
}

void MainWindow::nativeSetCursorShape(const QString& str)
{
    View* gview = activeView();
    if(gview)
    {
        QString shape = str.toLower();
        if     (shape == "arrow")           gview->setCursor(QCursor(Qt::ArrowCursor));
        else if(shape == "uparrow")         gview->setCursor(QCursor(Qt::UpArrowCursor));
        else if(shape == "cross")           gview->setCursor(QCursor(Qt::CrossCursor));
        else if(shape == "wait")            gview->setCursor(QCursor(Qt::WaitCursor));
        else if(shape == "ibeam")           gview->setCursor(QCursor(Qt::IBeamCursor));
        else if(shape == "resizevert")      gview->setCursor(QCursor(Qt::SizeVerCursor));
        else if(shape == "resizehoriz")     gview->setCursor(QCursor(Qt::SizeHorCursor));
        else if(shape == "resizediagleft")  gview->setCursor(QCursor(Qt::SizeBDiagCursor));
        else if(shape == "resizediagright") gview->setCursor(QCursor(Qt::SizeFDiagCursor));
        else if(shape == "move")            gview->setCursor(QCursor(Qt::SizeAllCursor));
        else if(shape == "blank")           gview->setCursor(QCursor(Qt::BlankCursor));
        else if(shape == "splitvert")       gview->setCursor(QCursor(Qt::SplitVCursor));
        else if(shape == "splithoriz")      gview->setCursor(QCursor(Qt::SplitHCursor));
        else if(shape == "handpointing")    gview->setCursor(QCursor(Qt::PointingHandCursor));
        else if(shape == "forbidden")       gview->setCursor(QCursor(Qt::ForbiddenCursor));
        else if(shape == "handopen")        gview->setCursor(QCursor(Qt::OpenHandCursor));
        else if(shape == "handclosed")      gview->setCursor(QCursor(Qt::ClosedHandCursor));
        else if(shape == "whatsthis")       gview->setCursor(QCursor(Qt::WhatsThisCursor));
        else if(shape == "busy")            gview->setCursor(QCursor(Qt::BusyCursor));
        #if QT_VERSION >= 0x040700
        else if(shape == "dragmove")        gview->setCursor(QCursor(Qt::DragMoveCursor));
        else if(shape == "dragcopy")        gview->setCursor(QCursor(Qt::DragCopyCursor));
        else if(shape == "draglink")        gview->setCursor(QCursor(Qt::DragLinkCursor));
        #endif

    }
}

qreal MainWindow::nativeCalculateAngle(qreal x1, qreal y1, qreal x2, qreal y2)
{
    return QLineF(x1, -y1, x2, -y2).angle();
}

qreal MainWindow::nativeCalculateDistance(qreal x1, qreal y1, qreal x2, qreal y2)
{
    return QLineF(x1, y1, x2, y2).length();
}

qreal MainWindow::nativePerpendicularDistance(qreal px, qreal py, qreal x1, qreal y1, qreal x2, qreal y2)
{
    QLineF line(x1, y1, x2, y2);
    QLineF norm = line.normalVector();
    qreal dx = px-x1;
    qreal dy = py-y1;
    norm.translate(dx, dy);
    QPointF iPoint;
    norm.intersects(line, &iPoint);
    return QLineF(px, py, iPoint.x(), iPoint.y()).length();
}

int MainWindow::nativeNumSelected()
{
    View* gview = activeView();
    if(gview) { return gview->numSelected(); }
    return 0;
}

void MainWindow::nativeSelectAll()
{
    View* gview = activeView();
    if (gview) {
        gview->selectAll();
    }
}

void MainWindow::nativeAddToSelection(const QPainterPath path, Qt::ItemSelectionMode mode)
{
}

void MainWindow::nativeClearSelection()
{
    View* gview = activeView();
    if (gview) {
        gview->clearSelection();
    }
}

void MainWindow::nativeDeleteSelected()
{
    View* gview = activeView();
    if (gview) {
        gview->deleteSelected();
    }
}

void MainWindow::nativeCutSelected(qreal x, qreal y)
{
}

void MainWindow::nativeCopySelected(qreal x, qreal y)
{
}

void MainWindow::nativePasteSelected(qreal x, qreal y)
{
}

void MainWindow::nativeMoveSelected(qreal dx, qreal dy)
{
    View* gview = activeView();
    if(gview) { gview->moveSelected(dx, -dy); }
}

void MainWindow::nativeScaleSelected(qreal x, qreal y, qreal factor)
{
    if (factor <= 0.0) {
        QMessageBox::critical(this, tr("ScaleFactor Error"),
                                tr("Hi there. If you are not a developer, report this as a bug. "
                                "If you are a developer, your code needs examined, and possibly your head too."));
    }

    View* gview = activeView();
    if (gview) {
        gview->scaleSelected(x, -y, factor);
    }
}

void MainWindow::nativeRotateSelected(qreal x, qreal y, qreal rot)
{
    View* gview = activeView();
    if(gview) { gview->rotateSelected(x, -y, -rot); }
}

void MainWindow::nativeMirrorSelected(qreal x1, qreal y1, qreal x2, qreal y2)
{
    View* gview = activeView();
    if(gview) { gview->mirrorSelected(x1, -y1, x2, -y2); }
}

qreal MainWindow::nativeQSnapX()
{
    QGraphicsScene* scene = activeScene();
    if(scene) return scene->property(SCENE_QSNAP_POINT).toPointF().x();
    return 0.0;
}

qreal MainWindow::nativeQSnapY()
{
    QGraphicsScene* scene = activeScene();
    if (scene) {
        return -scene->property(SCENE_QSNAP_POINT).toPointF().y();
    }
    return 0.0;
}

qreal MainWindow::nativeMouseX()
{
    QGraphicsScene* scene = activeScene();
    if (scene) {
        qDebug("mouseX: %.50f", scene->property(SCENE_MOUSE_POINT).toPointF().x());
        return scene->property(SCENE_MOUSE_POINT).toPointF().x();
    }
    return 0.0;
}

qreal MainWindow::nativeMouseY()
{
    QGraphicsScene* scene = activeScene();
    if (scene) {
        qDebug("mouseY: %.50f", -scene->property(SCENE_MOUSE_POINT).toPointF().y());
        return -scene->property(SCENE_MOUSE_POINT).toPointF().y();
    }
    return 0.0;
}