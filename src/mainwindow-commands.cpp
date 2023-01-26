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

#if 0
void stub_implement(std::string txt)
{
    debug_message("TODO: %s", qPrintable(txt));
}

void stub_testing()
{
    QMessageBox::warning(this, translate("Testing Feature"), translate("<b>This feature is in testing.</b>"));
}

void exit()
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

void quit()
{
    debug_message("quit()");
    exit();
}

void checkForUpdates()
{
    debug_message("checkForUpdates()");
    //TODO: Check website for new versions, commands, etc...
}

void cut()
{
    debug_message("cut()");
    View* gview = activeView();
    if (gview) { gview->cut(); }
}

void copy()
{
    debug_message("copy()");
    View* gview = activeView();
    if (gview) { gview->copy(); }
}

void paste()
{
    debug_message("paste()");
    View* gview = activeView();
    if (gview) { gview->paste(); }
}

void selectAll()
{
    debug_message("selectAll()");
    View* gview = activeView();
    if (gview) { gview->selectAll(); }
}

std::string platformString()
{
    //TODO: Append QSysInfo to string where applicable.
    std::string os;
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
    debug_message("Platform: %s", qPrintable(os));
    return os;
}

void designDetails()
{
    QGraphicsScene* scene = activeScene();
    if (scene)
    {
        EmbDetailsDialog dialog(scene, this);
        dialog.exec();
    }
}

void whatsThisContextHelp()
{
    debug_message("whatsThisContextHelp()");
    QWhatsThis::enterWhatsThisMode();
}

void print()
{
    debug_message("print()");
    MdiWindow* mdiWin = qobject_cast<MdiWindow*>(mdiArea->activeSubWindow());
    if (mdiWin) { mdiWin->print(); }
}

void tipOfTheDay()
{
    debug_message("tipOfTheDay()");

    std::string appDir = qApp->applicationDirPath();

    wizardTipOfTheDay = new QWizard(this);
    wizardTipOfTheDay->setAttribute(WA_DeleteOnClose);
    wizardTipOfTheDay->setWizardStyle(QWizard::ModernStyle);
    wizardTipOfTheDay->setMinimumSize(550, 400);

    QWizardPage* page = new QWizardPage(wizardTipOfTheDay);

    ImageWidget* imgBanner = new ImageWidget(appDir + "/images/did-you-know.png", wizardTipOfTheDay);

    if (settings_general_current_tip >= listTipOfTheDay.size())
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
    wizardTipOfTheDay->setButtonText(QWizard::CustomButton1, translate("&Previous"));
    wizardTipOfTheDay->setButtonText(QWizard::CustomButton2, translate("&Next"));
    wizardTipOfTheDay->setButtonText(QWizard::CustomButton3, translate("&Close"));
    wizardTipOfTheDay->setOption(QWizard::HaveCustomButton1, true);
    wizardTipOfTheDay->setOption(QWizard::HaveCustomButton2, true);
    wizardTipOfTheDay->setOption(QWizard::HaveCustomButton3, true);
    connect(wizardTipOfTheDay, SIGNAL(customButtonClicked(int)), this, SLOT(buttonTipOfTheDayClicked(int)));

    QList<QWizard::WizardButton> listTipOfTheDayButtons;
    listTipOfTheDayButtons << QWizard::Stretch << QWizard::CustomButton1 << QWizard::CustomButton2 << QWizard::CustomButton3;
    wizardTipOfTheDay->setButtonLayout(listTipOfTheDayButtons);

    wizardTipOfTheDay->exec();
}

void checkBoxTipOfTheDayStateChanged(int checked)
{
    settings_general_tip_of_the_day = checked;
}

void buttonTipOfTheDayClicked(int button)
{
    debug_message("buttonTipOfTheDayClicked(%d)", button);
    if (button == QWizard::CustomButton1)
    {
        if (settings_general_current_tip > 0)
            settings_general_current_tip--;
        else
            settings_general_current_tip = listTipOfTheDay.size()-1;
        labelTipOfTheDay->setText(listTipOfTheDay.value(settings_general_current_tip));
    }
    else if (button == QWizard::CustomButton2)
    {
        settings_general_current_tip++;
        if (settings_general_current_tip >= listTipOfTheDay.size())
            settings_general_current_tip = 0;
        labelTipOfTheDay->setText(listTipOfTheDay.value(settings_general_current_tip));
    }
    else if (button == QWizard::CustomButton3)
    {
        wizardTipOfTheDay->close();
    }
}

// Standard Slots
void undo()
{
    debug_message("undo()");
    std::string prefix = prompt->getPrefix();
    if (dockUndoEdit->canUndo())
    {
        prompt->setPrefix("Undo " + dockUndoEdit->undoText());
        prompt->appendHistory(std::string());
        dockUndoEdit->undo();
        prompt->setPrefix(prefix);
    }
    else
    {
        prompt->alert("Nothing to undo");
        prompt->setPrefix(prefix);
    }
}

void redo()
{
    debug_message("redo()");
    std::string prefix = prompt->getPrefix();
    if (dockUndoEdit->canRedo())
    {
        prompt->setPrefix("Redo " + dockUndoEdit->redoText());
        prompt->appendHistory(std::string());
        dockUndoEdit->redo();
        prompt->setPrefix(prefix);
    }
    else
    {
        prompt->alert("Nothing to redo");
        prompt->setPrefix(prefix);
    }
}

bool isShiftPressed()
{
    return shiftKeyPressedState;
}

void setShiftPressed()
{
    shiftKeyPressedState = true;
}

void setShiftReleased()
{
    shiftKeyPressedState = false;
}

// Icons
void iconResize(int iconSize)
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

View* activeView()
{
    debug_message("activeView()");
    MdiWindow* mdiWin = qobject_cast<MdiWindow*>(mdiArea->activeSubWindow());
    if (mdiWin) {
        View* v = mdiWin->getView();
        return v;
    }
    return 0;
}

QGraphicsScene* activeScene()
{
    debug_message("activeScene()");
    MdiWindow* mdiWin = qobject_cast<MdiWindow*>(mdiArea->activeSubWindow());
    if (mdiWin)
    {
        QGraphicsScene* s = mdiWin->getScene();
        return s;
    }
    return 0;
}

QUndoStack* activeUndoStack()
{
    debug_message("activeUndoStack()");
    View* v = activeView();
    if (v)
    {
        QUndoStack* u = v->getUndoStack();
        return u;
    }
    return 0;
}

void setUndoCleanIcon(bool opened)
{
    dockUndoEdit->updateCleanIcon(opened);
}

void updateAllViewScrollBars(bool val)
{
    QList<QMdiSubWindow*> windowList = mdiArea->subWindowList();
    for(int i = 0; i < windowList.count(); ++i)
    {
        MdiWindow* mdiWin = qobject_cast<MdiWindow*>(windowList.at(i));
        if (mdiWin) { mdiWin->showViewScrollBars(val); }
    }
}

void updateAllViewCrossHairColors(unsigned int color)
{
    QList<QMdiSubWindow*> windowList = mdiArea->subWindowList();
    for(int i = 0; i < windowList.count(); ++i)
    {
        MdiWindow* mdiWin = qobject_cast<MdiWindow*>(windowList.at(i));
        if (mdiWin) { mdiWin->setViewCrossHairColor(color); }
    }
}

void updateAllViewBackgroundColors(unsigned int color)
{
    QList<QMdiSubWindow*> windowList = mdiArea->subWindowList();
    for(int i = 0; i < windowList.count(); ++i)
    {
        MdiWindow* mdiWin = qobject_cast<MdiWindow*>(windowList.at(i));
        if (mdiWin) { mdiWin->setViewBackgroundColor(color); }
    }
}

void updateAllViewSelectBoxColors(unsigned int colorL, unsigned int fillL, unsigned int colorR, unsigned int fillR, int alpha)
{
    QList<QMdiSubWindow*> windowList = mdiArea->subWindowList();
    for(int i = 0; i < windowList.count(); ++i)
    {
        MdiWindow* mdiWin = qobject_cast<MdiWindow*>(windowList.at(i));
        if (mdiWin) { mdiWin->setViewSelectBoxColors(colorL, fillL, colorR, fillR, alpha); }
    }
}

void updateAllViewGridColors(unsigned int color)
{
    QList<QMdiSubWindow*> windowList = mdiArea->subWindowList();
    for(int i = 0; i < windowList.count(); ++i)
    {
        MdiWindow* mdiWin = qobject_cast<MdiWindow*>(windowList.at(i));
        if (mdiWin) { mdiWin->setViewGridColor(color); }
    }
}

void updateAllViewRulerColors(unsigned int color)
{
    QList<QMdiSubWindow*> windowList = mdiArea->subWindowList();
    for(int i = 0; i < windowList.count(); ++i)
    {
        MdiWindow* mdiWin = qobject_cast<MdiWindow*>(windowList.at(i));
        if (mdiWin) { mdiWin->setViewRulerColor(color); }
    }
}

void updatePickAddMode(bool val)
{
    settings_selection_mode_pickadd = val;
    dockPropEdit->updatePickAddModeButton(val);
}

void pickAddModeToggled()
{
    bool val = !settings_selection_mode_pickadd;
    updatePickAddMode(val);
}

// Layer ToolBar
void makeLayerActive()
{
    debug_message("makeLayerActive()");
    stub_implement("Implement makeLayerActive.");
}

void layerManager()
{
    debug_message("layerManager()");
    stub_implement("Implement layerManager.");
    LayerManager layman(this, this);
    layman.exec();
}

void layerPrevious()
{
    debug_message("layerPrevious()");
    stub_implement("Implement layerPrevious.");
}

// Zoom ToolBar
void zoomRealtime()
{
    debug_message("zoomRealtime()");
    stub_implement("Implement zoomRealtime.");
}

void zoomPrevious()
{
    debug_message("zoomPrevious()");
    stub_implement("Implement zoomPrevious.");
}

void zoomWindow()
{
    debug_message("zoomWindow()");
    View* gview = activeView();
    if (gview) { gview->zoomWindow(); }
}

void zoomDynamic()
{
    debug_message("zoomDynamic()");
    stub_implement("Implement zoomDynamic.");
}

void zoomScale()
{
    debug_message("zoomScale()");
    stub_implement("Implement zoomScale.");
}

void zoomCenter()
{
    debug_message("zoomCenter()");
    stub_implement("Implement zoomCenter.");
}

void zoomIn()
{
    debug_message("zoomIn()");
    View* gview = activeView();
    if (gview) { gview->zoomIn(); }
}

void zoomOut()
{
    debug_message("zoomOut()");
    View* gview = activeView();
    if (gview) { gview->zoomOut(); }
}

void zoomSelected()
{
    debug_message("zoomSelected()");
    View* gview = activeView();
    QUndoStack* stack = gview->getUndoStack();
    if (gview && stack)
    {
        UndoableNavCommand* cmd = new UndoableNavCommand("ZoomSelected", gview, 0);
        stack->push(cmd);
    }
}

void zoomAll()
{
    debug_message("zoomAll()");
    stub_implement("Implement zoomAll.");
}

void zoomExtents()
{
    debug_message("zoomExtents()");
    View* gview = activeView();
    QUndoStack* stack = gview->getUndoStack();
    if (gview && stack)
    {
        UndoableNavCommand* cmd = new UndoableNavCommand("ZoomExtents", gview, 0);
        stack->push(cmd);
    }
}
// Pan SubMenu
void panrealtime()
{
    debug_message("panrealtime()");
    View* gview = activeView();
    if (gview) { gview->panRealTime(); }
}

void panpoint()
{
    debug_message("panpoint()");
    View* gview = activeView();
    if (gview) { gview->panPoint(); }
}

void panLeft()
{
    debug_message("panLeft()");
    View* gview = activeView();
    QUndoStack* stack = gview->getUndoStack();
    if (gview && stack)
    {
        UndoableNavCommand* cmd = new UndoableNavCommand("PanLeft", gview, 0);
        stack->push(cmd);
    }
}

void panRight()
{
    debug_message("panRight()");
    View* gview = activeView();
    QUndoStack* stack = gview->getUndoStack();
    if (gview && stack)
    {
        UndoableNavCommand* cmd = new UndoableNavCommand("PanRight", gview, 0);
        stack->push(cmd);
    }
}

void panUp()
{
    debug_message("panUp()");
    View* gview = activeView();
    QUndoStack* stack = gview->getUndoStack();
    if (gview && stack)
    {
        UndoableNavCommand* cmd = new UndoableNavCommand("PanUp", gview, 0);
        stack->push(cmd);
    }
}

void panDown()
{
    debug_message("panDown()");
    View* gview = activeView();
    QUndoStack* stack = gview->getUndoStack();
    if (gview && stack)
    {
        UndoableNavCommand* cmd = new UndoableNavCommand("PanDown", gview, 0);
        stack->push(cmd);
    }
}

void dayVision()
{
    View* gview = activeView();
    if (gview)
    {
        gview->setBackgroundColor(qRgb(255,255,255)); //TODO: Make day vision color settings.
        gview->setCrossHairColor(qRgb(0,0,0));        //TODO: Make day vision color settings.
        gview->setGridColor(qRgb(0,0,0));             //TODO: Make day vision color settings.
    }
}

void nightVision()
{
    View* gview = activeView();
    if (gview)
    {
        gview->setBackgroundColor(qRgb(0,0,0));      //TODO: Make night vision color settings.
        gview->setCrossHairColor(qRgb(255,255,255)); //TODO: Make night vision color settings.
        gview->setGridColor(qRgb(255,255,255));      //TODO: Make night vision color settings.
    }
}

void doNothing()
{
    //This function intentionally does nothing.
    debug_message("doNothing()");
}

void layerSelectorIndexChanged(int index)
{
    debug_message("layerSelectorIndexChanged(%d)", index);
}

void colorSelectorIndexChanged(int index)
{
    debug_message("colorSelectorIndexChanged(%d)", index);

    QComboBox* comboBox = qobject_cast<QComboBox*>(sender());
    unsigned int newColor;
    if (comboBox)
    {
        bool ok = 0;
        //TODO: Handle ByLayer and ByBlock and Other...
        newColor = comboBox->itemData(index).toUInt(&ok);
        if (!ok)
            QMessageBox::warning(this, translate("Color Selector Conversion Error"), translate("<b>An error has occured while changing colors.</b>"));
    }
    else
        QMessageBox::warning(this, translate("Color Selector Pointer Error"), translate("<b>An error has occured while changing colors.</b>"));

    MdiWindow* mdiWin = qobject_cast<MdiWindow*>(mdiArea->activeSubWindow());
    if (mdiWin) { mdiWin->currentColorChanged(newColor); }
}

void linetypeSelectorIndexChanged(int index)
{
    debug_message("linetypeSelectorIndexChanged(%d)", index);
}

void lineweightSelectorIndexChanged(int index)
{
    debug_message("lineweightSelectorIndexChanged(%d)", index);
}

void textFontSelectorCurrentFontChanged(const QFont& font)
{
    debug_message("textFontSelectorCurrentFontChanged()");
    setTextFont(font.family());
}

void textSizeSelectorIndexChanged(int index)
{
    debug_message("textSizeSelectorIndexChanged(%d)", index);
    settings_text_size = qFabs(textSizeSelector->itemData(index).toReal()); //TODO: check that the toReal() conversion is ok
}

std::string textFont()
{
    return settings_text_font;
}

double textSize()
{
    return settings_text_size;
}

double textAngle()
{
    return settings_text_angle;
}

bool textBold()
{
    return settings_text_style_bold;
}

bool textItalic()
{
    return settings_text_style_italic;
}

bool textUnderline()
{
    return settings_text_style_underline;
}

bool textStrikeOut()
{
    return settings_text_style_strikeout;
}

bool textOverline()
{
    return settings_text_style_overline;
}

void setTextFont(const std::string& str)
{
    textFontSelector->setCurrentFont(QFont(str));
    settings_text_font = str;
}

void setTextSize(double num)
{
    settings_text_size = qFabs(num);
    int index = textSizeSelector->findText("Custom", MatchContains);
    if (index != -1)
        textSizeSelector->removeItem(index);
    textSizeSelector->addItem("Custom " + std::string().setNum(num, 'f', 2) + " pt", num);
    index = textSizeSelector->findText("Custom", MatchContains);
    if (index != -1)
        textSizeSelector->setCurrentIndex(index);
}

void setTextAngle(double num)
{
    settings_text_angle = num;
}

void setTextBold(bool val)
{
    settings_text_style_bold = val;
}

void setTextItalic(bool val)
{
    settings_text_style_italic = val;
}

void setTextUnderline(bool val)
{
    settings_text_style_underline = val;
}

void setTextStrikeOut(bool val)
{
    settings_text_style_strikeout = val;
}

void setTextOverline(bool val)
{
    settings_text_style_overline = val;
}

std::string getCurrentLayer()
{
    MdiWindow* mdiWin = qobject_cast<MdiWindow*>(mdiArea->activeSubWindow());
    if (mdiWin) { return mdiWin->getCurrentLayer(); }
    return "0";
}

unsigned int getCurrentColor()
{
    MdiWindow* mdiWin = qobject_cast<MdiWindow*>(mdiArea->activeSubWindow());
    if (mdiWin) { return mdiWin->getCurrentColor(); }
    return 0; //TODO: return color ByLayer
}

std::string getCurrentLineType()
{
    MdiWindow* mdiWin = qobject_cast<MdiWindow*>(mdiArea->activeSubWindow());
    if (mdiWin) { return mdiWin->getCurrentLineType(); }
    return "ByLayer";
}

std::string getCurrentLineWeight()
{
    MdiWindow* mdiWin = qobject_cast<MdiWindow*>(mdiArea->activeSubWindow());
    if (mdiWin) { return mdiWin->getCurrentLineWeight(); }
    return "ByLayer";
}

void deletePressed()
{
    debug_message("deletePressed()");
    QApplication::setOverrideCursor(WaitCursor);
    MdiWindow* mdiWin = qobject_cast<MdiWindow*>(mdiArea->activeSubWindow());
    if (mdiWin) { mdiWin->deletePressed(); }
    QApplication::restoreOverrideCursor();
}

void escapePressed()
{
    debug_message("escapePressed()");
    QApplication::setOverrideCursor(WaitCursor);
    MdiWindow* mdiWin = qobject_cast<MdiWindow*>(mdiArea->activeSubWindow());
    if (mdiWin) { mdiWin->escapePressed(); }
    QApplication::restoreOverrideCursor();

    nativeEndCommand();
}

void toggleGrid()
{
    debug_message("toggleGrid()");
    statusbar->statusBarGridButton->toggle();
}

void toggleRuler()
{
    debug_message("toggleRuler()");
    statusbar->statusBarRulerButton->toggle();
}

void toggleLwt()
{
    debug_message("toggleLwt()");
    statusbar->statusBarLwtButton->toggle();
}

void enablePromptRapidFire()
{
    prompt->enableRapidFire();
}

void disablePromptRapidFire()
{
    prompt->disableRapidFire();
}

void enableMoveRapidFire()
{
    View* gview = activeView();
    if (gview) gview->enableMoveRapidFire();
}

void disableMoveRapidFire()
{
    View* gview = activeView();
    if (gview) gview->disableMoveRapidFire();
}

void promptHistoryAppended(const std::string& txt)
{
    MdiWindow* mdiWin = qobject_cast<MdiWindow*>(mdiArea->activeSubWindow());
    if (mdiWin) mdiWin->promptHistoryAppended(txt);
}

void logPromptInput(const std::string& txt)
{
    MdiWindow* mdiWin = qobject_cast<MdiWindow*>(mdiArea->activeSubWindow());
    if (mdiWin) mdiWin->logPromptInput(txt);
}

void promptInputPrevious()
{
    MdiWindow* mdiWin = qobject_cast<MdiWindow*>(mdiArea->activeSubWindow());
    if (mdiWin) mdiWin->promptInputPrevious();
}

void promptInputNext()
{
    MdiWindow* mdiWin = qobject_cast<MdiWindow*>(mdiArea->activeSubWindow());
    if (mdiWin) mdiWin->promptInputNext();
}

void runCommand()
{
    /* TODO. */
    QAction* act = qobject_cast<QAction*>(sender());
    if (act) {
        debug_message("runCommand(%s)", qPrintable(act->objectName()));
        prompt->endCommand();
        prompt->setCurrentText(act->objectName());
        prompt->processInput(Key_Return);
    }
}

void runCommandMain(const std::string& cmd)
{
    debug_message("runCommandMain(%s)", qPrintable(cmd));
    std::string fileName = "commands/" + cmd + "/" + cmd + ".js";
    // if (!getSettingsSelectionModePickFirst()) { nativeClearSelection(); } //TODO: Uncomment this line when post-selection is available
    // engine->evaluate(cmd + "_main()", fileName);
}

void runCommandClick(const std::string& cmd, double x, double y)
{
    debug_message("runCommandClick(%s, %.2f, %.2f)", qPrintable(cmd), x, y);
    std::string fileName = "commands/" + cmd + "/" + cmd + ".js";
    // engine->evaluate(cmd + "_click(" + std::string().setNum(x) + "," + std::string().setNum(-y) + ")", fileName);
}

void runCommandMove(const std::string& cmd, double x, double y)
{
    debug_message("runCommandMove(%s, %.2f, %.2f)", qPrintable(cmd), x, y);
    std::string fileName = "commands/" + cmd + "/" + cmd + ".js";
    // engine->evaluate(cmd + "_move(" + std::string().setNum(x) + "," + std::string().setNum(-y) + ")", fileName);
}

void runCommandContext(const std::string& cmd, const std::string& str)
{
    debug_message("runCommandContext(%s, %s)", qPrintable(cmd), qPrintable(str));
    std::string fileName = "commands/" + cmd + "/" + cmd + ".js";
    // engine->evaluate(cmd + "_context('" + str.toUpper() + "')", fileName);
}

void runCommandPrompt(const std::string& cmd, const std::string& str)
{
    debug_message("runCommandPrompt(%s, %s)", qPrintable(cmd), qPrintable(str));
    std::string fileName = "commands/" + cmd + "/" + cmd + ".js";
    //NOTE: Replace any special characters that will cause a syntax error
    std::string safeStr = str;
    safeStr.replace("\\", "\\\\");
    safeStr.replace("\'", "\\\'");
    /*
    if (prompt->isRapidFireEnabled()) { engine->evaluate(cmd + "_prompt('" + safeStr + "')", fileName); }
    else                             { engine->evaluate(cmd + "_prompt('" + safeStr.toUpper() + "')", fileName); }
    */
}

void nativeAlert(const std::string& txt)
{
    prompt->alert(txt);
}

void nativeBlinkPrompt()
{
    prompt->startBlinking();
}

void nativeSetPromptPrefix(const std::string& txt)
{
    prompt->setPrefix(txt);
}

void nativeAppendPromptHistory(const std::string& txt)
{
    prompt->appendHistory(txt);
}

void nativeEnablePromptRapidFire()
{
    enablePromptRapidFire();
}

void nativeDisablePromptRapidFire()
{
    disablePromptRapidFire();
}

void nativeEnableMoveRapidFire()
{
    enableMoveRapidFire();
}

void nativeDisableMoveRapidFire()
{
    disableMoveRapidFire();
}

void nativeInitCommand()
{
    View* gview = activeView();
    if (gview) gview->clearRubberRoom();
}

void nativeEndCommand()
{
    View* gview = activeView();
    if (gview)
    {
        gview->clearRubberRoom();
        gview->previewOff();
        gview->disableMoveRapidFire();
    }
    prompt->endCommand();
}

void nativeExit()
{
    exit();
}

void nativeHelp()
{
    help();
}

void nativeAbout()
{
    about();
}

void nativeTipOfTheDay()
{
    tipOfTheDay();
}

void nativeWindowCascade()
{
    mdiArea->cascade();
}

void nativeWindowTile()
{
    mdiArea->tile();
}

void nativeWindowClose()
{
    onCloseWindow();
}

void nativeWindowCloseAll()
{
    mdiArea->closeAllSubWindows();
}

void nativeWindowNext()
{
    mdiArea->activateNextSubWindow();
}

void nativeWindowPrevious()
{
    mdiArea->activatePreviousSubWindow();
}

std::string nativePlatformString()
{
    return platformString();
}

void nativeMessageBox(const std::string& type, const std::string& title, const std::string& text)
{
    std::string msgType = type.toLower();
    if     (msgType == "critical")    { QMessageBox::critical   (this, translate(qPrintable(title)), translate(qPrintable(text))); }
    else if (msgType == "information") { QMessageBox::information(this, translate(qPrintable(title)), translate(qPrintable(text))); }
    else if (msgType == "question")    { QMessageBox::question   (this, translate(qPrintable(title)), translate(qPrintable(text))); }
    else if (msgType == "warning")     { QMessageBox::warning    (this, translate(qPrintable(title)), translate(qPrintable(text))); }
    else                              { QMessageBox::critical   (this, translate("Native MessageBox Error"), translate("Incorrect use of the native messageBox function.")); }
}

void nativePrintArea(double x, double y, double w, double h)
{
    debug_message("nativePrintArea(%.2f, %.2f, %.2f, %.2f)", x, y, w, h);
    //TODO: Print Setup Stuff
    print();
}

void nativeDayVision()
{
    dayVision();
}

void nativeNightVision()
{
    nightVision();
}

void nativeSetBackgroundColor(quint8 r, quint8 g, quint8 b)
{
    settings_display_bg_color = qRgb(r,g,b);
    updateAllViewBackgroundColors(qRgb(r,g,b));
}

void nativeSetCrossHairColor(quint8 r, quint8 g, quint8 b)
{
    settings_display_crosshair_color = qRgb(r,g,b);
    updateAllViewCrossHairColors(qRgb(r,g,b));
}

void nativeSetGridColor(quint8 r, quint8 g, quint8 b)
{
    settings_grid_color = qRgb(r,g,b);
    updateAllViewGridColors(qRgb(r,g,b));
}

std::string nativeTextFont()
{
    return textFont();
}

double nativeTextSize()
{
    return textSize();
}

double nativeTextAngle()
{
    return textAngle();
}

bool nativeTextBold()
{
    return textBold();
}

bool nativeTextItalic()
{
    return textItalic();
}

bool nativeTextUnderline()
{
    return textUnderline();
}

bool nativeTextStrikeOut()
{
    return textStrikeOut();
}

bool nativeTextOverline()
{
    return textOverline();
}

void nativeSetTextFont(const std::string& str)
{
    setTextFont(str);
}

void nativeSetTextSize(double num)
{
    setTextSize(num);
}

void nativeSetTextAngle(double num)
{
    setTextAngle(num);
}

void nativeSetTextBold(bool val)
{
    setTextBold(val);
}

void nativeSetTextItalic(bool val)
{
    setTextItalic(val);
}

void nativeSetTextUnderline(bool val)
{
    setTextUnderline(val);
}

void nativeSetTextStrikeOut(bool val)
{
    setTextStrikeOut(val);
}

void nativeSetTextOverline(bool val)
{
    setTextOverline(val);
}

void nativePreviewOn(int clone, int mode, double x, double y, double data)
{
    View* gview = activeView();
    if (gview) gview->previewOn(clone, mode, x, -y, data);
}

void nativePreviewOff()
{
    View* gview = activeView();
    if (gview) gview->previewOff();
}

void nativeVulcanize()
{
    View* gview = activeView();
    if (gview) gview->vulcanizeRubberRoom();
}

void nativeClearRubber()
{
    View* gview = activeView();
    if (gview) gview->clearRubberRoom();
}

bool nativeAllowRubber()
{
    View* gview = activeView();
    if (gview) return gview->allowRubber();
    return false;
}

void nativeSpareRubber(qint64 id)
{
    View* gview = activeView();
    if (gview) gview->spareRubber(id);
}

void nativeSetRubberMode(int mode)
{
    View* gview = activeView();
    if (gview) gview->setRubberMode(mode);
}

void nativeSetRubberPoint(const std::string& key, double x, double y)
{
    View* gview = activeView();
    if (gview) gview->setRubberPoint(key, EmbVector(x, -y));
}

void nativeSetRubberText(const std::string& key, const std::string& txt)
{
    View* gview = activeView();
    if (gview) gview->setRubberText(key, txt);
}

void nativeAddTextMulti(const std::string& str, double x, double y, double rot, bool fill, int rubberMode)
{
}

void nativeAddTextSingle(const std::string& str, double x, double y, double rot, bool fill, int rubberMode)
{
    View* gview = activeView();
    QGraphicsScene* gscene = gview->scene();
    QUndoStack* stack = gview->getUndoStack();
    if (gview && gscene && stack)
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
        if (rubberMode)
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

void nativeAddInfiniteLine(double x1, double y1, double x2, double y2, double rot)
{
}

void nativeAddRay(double x1, double y1, double x2, double y2, double rot)
{
}

void nativeAddLine(double x1, double y1, double x2, double y2, double rot, int rubberMode)
{
    View* gview = activeView();
    QGraphicsScene* gscene = gview->scene();
    QUndoStack* stack = gview->getUndoStack();
    if (gview && gscene && stack)
    {
        LineObject* obj = new LineObject(x1, -y1, x2, -y2, getCurrentColor());
        obj->setRotation(-rot);
        obj->setObjectRubberMode(rubberMode);
        if (rubberMode)
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

void nativeAddTriangle(double x1, double y1, double x2, double y2, double x3, double y3, double rot, bool fill)
{
}

void nativeAddRectangle(double x, double y, double w, double h, double rot, bool fill, int rubberMode)
{
    View* gview = activeView();
    QGraphicsScene* gscene = gview->scene();
    QUndoStack* stack = gview->getUndoStack();
    if (gview && gscene && stack)
    {
        RectObject* obj = new RectObject(x, -y, w, -h, getCurrentColor());
        obj->setRotation(-rot);
        obj->setObjectRubberMode(rubberMode);
        //TODO: rect fill
        if (rubberMode)
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

void nativeAddRoundedRectangle(double x, double y, double w, double h, double rad, double rot, bool fill)
{
}

void nativeAddArc(double startX, double startY, double midX, double midY, double endX, double endY, int rubberMode)
{
    View* gview = activeView();
    QGraphicsScene* scene = activeScene();
    if (gview && scene)
    {
        ArcObject* arcObj = new ArcObject(startX, -startY, midX, -midY, endX, -endY, getCurrentColor());
        arcObj->setObjectRubberMode(rubberMode);
        if (rubberMode) gview->addToRubberRoom(arcObj);
        scene->addItem(arcObj);
        scene->update();
    }
}

void nativeAddCircle(double centerX, double centerY, double radius, bool fill, int rubberMode)
{
    View* gview = activeView();
    QGraphicsScene* gscene = gview->scene();
    QUndoStack* stack = gview->getUndoStack();
    if (gview && gscene && stack)
    {
        CircleObject* obj = new CircleObject(centerX, -centerY, radius, getCurrentColor());
        obj->setObjectRubberMode(rubberMode);
        //TODO: circle fill
        if (rubberMode)
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

void nativeAddSlot(double centerX, double centerY, double diameter, double length, double rot, bool fill, int rubberMode)
{
    //TODO: Use UndoableAddCommand for slots
    /*
    SlotObject* slotObj = new SlotObject(centerX, -centerY, diameter, length, getCurrentColor());
    slotObj->setRotation(-rot);
    slotObj->setObjectRubberMode(rubberMode);
    if (rubberMode) gview->addToRubberRoom(slotObj);
    scene->addItem(slotObj);
    //TODO: slot fill
    scene->update();
    */
}

void nativeAddEllipse(double centerX, double centerY, double width, double height, double rot, bool fill, int rubberMode)
{
    View* gview = activeView();
    QGraphicsScene* gscene = gview->scene();
    QUndoStack* stack = gview->getUndoStack();
    if (gview && gscene && stack)
    {
        EllipseObject* obj = new EllipseObject(centerX, -centerY, width, height, getCurrentColor());
        obj->setRotation(-rot);
        obj->setObjectRubberMode(rubberMode);
        //TODO: ellipse fill
        if (rubberMode)
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

void nativeAddPoint(double x, double y)
{
    View* gview = activeView();
    QUndoStack* stack = gview->getUndoStack();
    if (gview && stack)
    {
        PointObject* obj = new PointObject(x, -y, getCurrentColor());
        UndoableAddCommand* cmd = new UndoableAddCommand(obj->data(OBJ_NAME).toString(), obj, gview, 0);
        stack->push(cmd);
    }
}

void nativeAddRegularPolygon(double centerX, double centerY, quint16 sides, quint8 mode, double rad, double rot, bool fill)
{
}

//NOTE: This native is different than the rest in that the Y+ is down (scripters need not worry about this)
void nativeAddPolygon(double startX, double startY, const QPainterPath& p, int rubberMode)
{
    View* gview = activeView();
    QGraphicsScene* gscene = gview->scene();
    QUndoStack* stack = gview->getUndoStack();
    if (gview && gscene && stack)
    {
        PolygonObject* obj = new PolygonObject(startX, startY, p, getCurrentColor());
        obj->setObjectRubberMode(rubberMode);
        if (rubberMode)
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
void nativeAddPolyline(double startX, double startY, const QPainterPath& p, int rubberMode)
{
    View* gview = activeView();
    QGraphicsScene* gscene = gview->scene();
    QUndoStack* stack = gview->getUndoStack();
    if (gview && gscene && stack) {
        PolylineObject* obj = new PolylineObject(startX, startY, p, getCurrentColor());
        obj->setObjectRubberMode(rubberMode);
        if (rubberMode)
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
void nativeAddPath(double startX, double startY, const QPainterPath& p, int rubberMode)
{
}

void nativeAddHorizontalDimension(double x1, double y1, double x2, double y2, double legHeight)
{
}

void nativeAddVerticalDimension(double x1, double y1, double x2, double y2, double legHeight)
{
}

void nativeAddImage(const std::string& img, double x, double y, double w, double h, double rot)
{
}

void nativeAddDimLeader(double x1, double y1, double x2, double y2, double rot, int rubberMode)
{
    View* gview = activeView();
    QGraphicsScene* gscene = gview->scene();
    QUndoStack* stack = gview->getUndoStack();
    if (gview && gscene && stack)
    {
        DimLeaderObject* obj = new DimLeaderObject(x1, -y1, x2, -y2, getCurrentColor());
        obj->setRotation(-rot);
        obj->setObjectRubberMode(rubberMode);
        if (rubberMode)
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

void nativeSetCursorShape(const std::string& str)
{
    View* gview = activeView();
    if (gview)
    {
        std::string shape = str.toLower();
        if     (shape == "arrow")           gview->setCursor(QCursor(ArrowCursor));
        else if (shape == "uparrow")         gview->setCursor(QCursor(UpArrowCursor));
        else if (shape == "cross")           gview->setCursor(QCursor(CrossCursor));
        else if (shape == "wait")            gview->setCursor(QCursor(WaitCursor));
        else if (shape == "ibeam")           gview->setCursor(QCursor(IBeamCursor));
        else if (shape == "resizevert")      gview->setCursor(QCursor(SizeVerCursor));
        else if (shape == "resizehoriz")     gview->setCursor(QCursor(SizeHorCursor));
        else if (shape == "resizediagleft")  gview->setCursor(QCursor(SizeBDiagCursor));
        else if (shape == "resizediagright") gview->setCursor(QCursor(SizeFDiagCursor));
        else if (shape == "move")            gview->setCursor(QCursor(SizeAllCursor));
        else if (shape == "blank")           gview->setCursor(QCursor(BlankCursor));
        else if (shape == "splitvert")       gview->setCursor(QCursor(SplitVCursor));
        else if (shape == "splithoriz")      gview->setCursor(QCursor(SplitHCursor));
        else if (shape == "handpointing")    gview->setCursor(QCursor(PointingHandCursor));
        else if (shape == "forbidden")       gview->setCursor(QCursor(ForbiddenCursor));
        else if (shape == "handopen")        gview->setCursor(QCursor(OpenHandCursor));
        else if (shape == "handclosed")      gview->setCursor(QCursor(ClosedHandCursor));
        else if (shape == "whatsthis")       gview->setCursor(QCursor(WhatsThisCursor));
        else if (shape == "busy")            gview->setCursor(QCursor(BusyCursor));
        #if QT_VERSION >= 0x040700
        else if (shape == "dragmove")        gview->setCursor(QCursor(DragMoveCursor));
        else if (shape == "dragcopy")        gview->setCursor(QCursor(DragCopyCursor));
        else if (shape == "draglink")        gview->setCursor(QCursor(DragLinkCursor));
        #endif

    }
}

double nativeCalculateAngle(double x1, double y1, double x2, double y2)
{
    return EmbLine(x1, -y1, x2, -y2).angle();
}

double nativeCalculateDistance(double x1, double y1, double x2, double y2)
{
    return EmbLine(x1, y1, x2, y2).length();
}

double nativePerpendicularDistance(double px, double py, double x1, double y1, double x2, double y2)
{
    EmbLine line(x1, y1, x2, y2);
    EmbLine norm = line.normalVector();
    double dx = px-x1;
    double dy = py-y1;
    norm.translate(dx, dy);
    EmbVector iPoint;
    norm.intersects(line, &iPoint);
    return EmbLine(px, py, iPoint.x(), iPoint.y()).length();
}

int nativeNumSelected()
{
    View* gview = activeView();
    if (gview) { return gview->numSelected(); }
    return 0;
}

void nativeSelectAll()
{
    View* gview = activeView();
    if (gview) {
        gview->selectAll();
    }
}

void nativeAddToSelection(const QPainterPath path, ItemSelectionMode mode)
{

}

void nativeClearSelection()
{
    View* gview = activeView();
    if (gview) {
        gview->clearSelection();
    }
}

void nativeDeleteSelected()
{
    View* gview = activeView();
    if (gview) {
        gview->deleteSelected();
    }
}

void nativeCutSelected(double x, double y)
{
}

void nativeCopySelected(double x, double y)
{
}

void nativePasteSelected(double x, double y)
{
}

void nativeMoveSelected(double dx, double dy)
{
    View* gview = activeView();
    if (gview) { gview->moveSelected(dx, -dy); }
}

void nativeScaleSelected(double x, double y, double factor)
{
    if (factor <= 0.0) {
        QMessageBox::critical(this, translate("ScaleFactor Error"),
                                translate("Hi there. If you are not a developer, report this as a bug. "
                                "If you are a developer, your code needs examined, and possibly your head too."));
    }

    View* gview = activeView();
    if (gview) {
        gview->scaleSelected(x, -y, factor);
    }
}

void nativeRotateSelected(double x, double y, double rot)
{
    View* gview = activeView();
    if (gview) { gview->rotateSelected(x, -y, -rot); }
}

void nativeMirrorSelected(double x1, double y1, double x2, double y2)
{
    View* gview = activeView();
    if (gview) { gview->mirrorSelected(x1, -y1, x2, -y2); }
}

double nativeQSnapX()
{
    QGraphicsScene* scene = activeScene();
    if (scene) return scene->property(SCENE_QSNAP_POINT).toPointF().x();
    return 0.0;
}

double nativeQSnapY()
{
    QGraphicsScene* scene = activeScene();
    if (scene) {
        return -scene->property(SCENE_QSNAP_POINT).toPointF().y();
    }
    return 0.0;
}

double nativeMouseX()
{
    QGraphicsScene* scene = activeScene();
    if (scene) {
        debug_message("mouseX: %.50f", scene->property(SCENE_MOUSE_POINT).toPointF().x());
        return scene->property(SCENE_MOUSE_POINT).toPointF().x();
    }
    return 0.0;
}

double nativeMouseY()
{
    QGraphicsScene* scene = activeScene();
    if (scene) {
        debug_message("mouseY: %.50f", -scene->property(SCENE_MOUSE_POINT).toPointF().y());
        return -scene->property(SCENE_MOUSE_POINT).toPointF().y();
    }
    return 0.0;
}
#endif
