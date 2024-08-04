/*
 * Embroidermodder 2.
 *
 * Copyright 2011-2024 The Embroidermodder Team
 * Embroidermodder 2 is Open Source Software, see LICENSE.md for licensing terms.
 *
 * Read the reference manual (https://www.libembroidery.org/downloads/emrm.pdf)
 * for advice on altering this file.
 *
 * MainWindow Commands
 */

#include "embroidermodder.h"

void
MainWindow::stub_implement(QString txt)
{
    qDebug("TODO: %s", qPrintable(txt));
}

void
MainWindow::stub_testing()
{
    QMessageBox::warning(this, tr("Testing Feature"), tr("<b>This feature is in testing.</b>"));
}

void
MainWindow::exit()
{
    qDebug("exit()");
    if (prompt_save_history.setting) {
        prompt->saveHistory("prompt.log", prompt_save_history_as_html.setting);
        /* TODO: get filename from settings */
    }
    qApp->closeAllWindows();
    /* Force the MainWindow destructor to run before exiting.
     * Makes Valgrind "still reachable" happy :)
     */
    this->deleteLater();
}

void
MainWindow::checkForUpdates()
{
    qDebug("checkForUpdates()");
    //TODO: Check website for new versions, commands, etc...
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

void
MainWindow::designDetails()
{
    QGraphicsScene* scene = activeScene();
    if (scene) {
        EmbDetailsDialog dialog(scene, this);
        dialog.exec();
    }
}

void
MainWindow::whatsThisContextHelp()
{
    qDebug("whatsThisContextHelp()");
    QWhatsThis::enterWhatsThisMode();
}

void
MainWindow::print()
{
    qDebug("print()");
    MdiWindow* mdiWin = qobject_cast<MdiWindow*>(mdiArea->activeSubWindow());
    if (mdiWin) { mdiWin->print(); }
}

void
MainWindow::tipOfTheDay()
{
    qDebug("tipOfTheDay()");

    QString appDir = qApp->applicationDirPath();

    wizardTipOfTheDay = new QWizard(this);
    wizardTipOfTheDay->setAttribute(Qt::WA_DeleteOnClose);
    wizardTipOfTheDay->setWizardStyle(QWizard::ModernStyle);
    wizardTipOfTheDay->setMinimumSize(550, 400);

    QWizardPage* page = new QWizardPage(wizardTipOfTheDay);

    ImageWidget* imgBanner = new ImageWidget(appDir + "/images/did-you-know.png", wizardTipOfTheDay);

    if (general_current_tip.setting >= listTipOfTheDay.size()) {
        general_current_tip.setting = 0;
    }
    labelTipOfTheDay = new QLabel(listTipOfTheDay.value(general_current_tip.setting), wizardTipOfTheDay);
    labelTipOfTheDay->setWordWrap(true);

    QCheckBox* checkBoxTipOfTheDay = new QCheckBox(tr("&Show tips on startup"), wizardTipOfTheDay);
    checkBoxTipOfTheDay->setChecked(general_tip_of_the_day.setting);
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
    listTipOfTheDayButtons << QWizard::Stretch << QWizard::CustomButton1
        << QWizard::CustomButton2 << QWizard::CustomButton3;
    wizardTipOfTheDay->setButtonLayout(listTipOfTheDayButtons);

    wizardTipOfTheDay->exec();
}

void
MainWindow::checkBoxTipOfTheDayStateChanged(int checked)
{
    general_tip_of_the_day.setting = checked;
}

void
MainWindow::buttonTipOfTheDayClicked(int button)
{
    qDebug("buttonTipOfTheDayClicked(%d)", button);
    if (button == QWizard::CustomButton1) {
        if (general_current_tip.setting > 0)
            general_current_tip.setting--;
        else
            general_current_tip.setting = listTipOfTheDay.size()-1;
        labelTipOfTheDay->setText(listTipOfTheDay.value(general_current_tip.setting));
    }
    else if (button == QWizard::CustomButton2) {
        general_current_tip.setting++;
        if (general_current_tip.setting >= listTipOfTheDay.size())
            general_current_tip.setting = 0;
        labelTipOfTheDay->setText(listTipOfTheDay.value(general_current_tip.setting));
    }
    else if (button == QWizard::CustomButton3) {
        wizardTipOfTheDay->close();
    }
}

void
MainWindow::help()
{
    qDebug("help()");

    // Open the HTML Help in the default browser
    QUrl helpURL("file:///" + qApp->applicationDirPath() + "/help/doc-index.html");
    QDesktopServices::openUrl(helpURL);

    //TODO: This is how to start an external program. Use this elsewhere...
    //QString program = "firefox";
    //QStringList arguments;
    //arguments << "help/commands.html";
    //QProcess *myProcess = new QProcess(this);
    //myProcess->start(program, arguments);
}

void
MainWindow::changelog()
{
    qDebug("changelog()");

    QUrl changelogURL("help/changelog.html");
    QDesktopServices::openUrl(changelogURL);
}

// Standard Slots
void
MainWindow::undo()
{
    qDebug("undo()");
    QString prefix = prompt->getPrefix();
    if (dockUndoEdit->canUndo()) {
        prompt->setPrefix("Undo " + dockUndoEdit->undoText());
        prompt->appendHistory(QString());
        dockUndoEdit->undo();
        prompt->setPrefix(prefix);
    }
    else {
        prompt->alert("Nothing to undo");
        prompt->setPrefix(prefix);
    }
}

void
MainWindow::redo()
{
    qDebug("redo()");
    QString prefix = prompt->getPrefix();
    if (dockUndoEdit->canRedo()) {
        prompt->setPrefix("Redo " + dockUndoEdit->redoText());
        prompt->appendHistory(QString());
        dockUndoEdit->redo();
        prompt->setPrefix(prefix);
    }
    else {
        prompt->alert("Nothing to redo");
        prompt->setPrefix(prefix);
    }
}

bool MainWindow::isShiftPressed()
{
    return shiftKeyPressedState;
}

void
MainWindow::setShiftPressed()
{
    shiftKeyPressedState = true;
}

void
MainWindow::setShiftReleased()
{
    shiftKeyPressedState = false;
}

// Icons
void
MainWindow::iconResize(int iconSize)
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

    // TODO: low-priority: open app with iconSize set to 128. resize the icons to a smaller size.

    general_icon_size.setting = iconSize;
}

MdiWindow*
activeMdiWindow()
{
    qDebug("activeMdiWindow()");
    MdiWindow* mdiWin = qobject_cast<MdiWindow*>(_main->mdiArea->activeSubWindow());
    return mdiWin;
}

View*
activeView()
{
    _main->debug_message("activeView()");
    MdiWindow* mdiWin = qobject_cast<MdiWindow*>(_main->mdiArea->activeSubWindow());
    if (mdiWin) {
        View* v = mdiWin->gview;
        return v;
    }
    return 0;
}

QGraphicsScene*
activeScene()
{
    _main->debug_message("activeScene()");
    MdiWindow* mdiWin = qobject_cast<MdiWindow*>(_main->mdiArea->activeSubWindow());
    if (mdiWin) {
        QGraphicsScene* s = mdiWin->gscene;
        return s;
    }
    return 0;
}

QUndoStack*
activeUndoStack()
{
    _main->debug_message("activeUndoStack()");
    View* v = activeView();
    if (v) {
        QUndoStack* u = v->getUndoStack();
        return u;
    }
    return 0;
}

void
MainWindow::setUndoCleanIcon(bool opened)
{
    dockUndoEdit->updateCleanIcon(opened);
}

void
MainWindow::updateAllViewScrollBars(bool val)
{
    QList<QMdiSubWindow*> windowList = mdiArea->subWindowList();
    for (int i = 0; i < windowList.count(); ++i) {
        MdiWindow* mdiWin = qobject_cast<MdiWindow*>(windowList.at(i));
        if (mdiWin) { mdiWin->showViewScrollBars(val); }
    }
}

void
MainWindow::updateAllViewCrossHairColors(QRgb color)
{
    QList<QMdiSubWindow*> windowList = mdiArea->subWindowList();
    for (int i = 0; i < windowList.count(); ++i) {
        MdiWindow* mdiWin = qobject_cast<MdiWindow*>(windowList.at(i));
        if (mdiWin) { mdiWin->setViewCrossHairColor(color); }
    }
}

void
MainWindow::updateAllViewBackgroundColors(QRgb color)
{
    QList<QMdiSubWindow*> windowList = mdiArea->subWindowList();
    for (int i = 0; i < windowList.count(); ++i) {
        MdiWindow* mdiWin = qobject_cast<MdiWindow*>(windowList.at(i));
        if (mdiWin) { mdiWin->setViewBackgroundColor(color); }
    }
}

void
MainWindow::updateAllViewSelectBoxColors(QRgb colorL, QRgb fillL, QRgb colorR, QRgb fillR, int alpha)
{
    QList<QMdiSubWindow*> windowList = mdiArea->subWindowList();
    for (int i = 0; i < windowList.count(); ++i) {
        MdiWindow* mdiWin = qobject_cast<MdiWindow*>(windowList.at(i));
        if (mdiWin) {
            mdiWin->setViewSelectBoxColors(colorL, fillL, colorR, fillR, alpha);
        }
    }
}

void
MainWindow::updateAllViewGridColors(QRgb color)
{
    QList<QMdiSubWindow*> windowList = mdiArea->subWindowList();
    for (int i = 0; i < windowList.count(); ++i) {
        MdiWindow* mdiWin = qobject_cast<MdiWindow*>(windowList.at(i));
        if (mdiWin) {
            mdiWin->setViewGridColor(color);
        }
    }
}

void
MainWindow::updateAllViewRulerColors(QRgb color)
{
    QList<QMdiSubWindow*> windowList = mdiArea->subWindowList();
    for (int i = 0; i < windowList.count(); ++i) {
        MdiWindow* mdiWin = qobject_cast<MdiWindow*>(windowList.at(i));
        if (mdiWin) {
            mdiWin->setViewRulerColor(color);
        }
    }
}

void
MainWindow::updatePickAddMode(bool val)
{
    selection_mode_pickadd.setting = val;
    dockPropEdit->updatePickAddModeButton(val);
}

void
MainWindow::pickAddModeToggled()
{
    bool val = !selection_mode_pickadd.setting;
    updatePickAddMode(val);
}

// Layer ToolBar
void
MainWindow::makeLayerActive()
{
    qDebug("makeLayerActive()");
    stub_implement("Implement makeLayerActive.");
}

void
MainWindow::layerManager()
{
    qDebug("layerManager()");
    stub_implement("Implement layerManager.");
    LayerManager layman(this, this);
    layman.exec();
}

void
MainWindow::layerPrevious()
{
    qDebug("layerPrevious()");
    stub_implement("Implement layerPrevious.");
}

void
MainWindow::zoomPrevious()
{
    qDebug("zoomPrevious()");
    stub_implement("Implement zoomPrevious.");
}

void
MainWindow::zoomWindow()
{
    qDebug("zoomWindow()");
    View* gview = activeView();
    if (gview) {
        gview->zoomWindow();
    }
}

void
MainWindow::zoomDynamic()
{
    qDebug("zoomDynamic()");
    stub_implement("Implement zoomDynamic.");
}

void
MainWindow::zoomScale()
{
    qDebug("zoomScale()");
    stub_implement("Implement zoomScale.");
}

void
MainWindow::zoomCenter()
{
    qDebug("zoomCenter()");
    stub_implement("Implement zoomCenter.");
}

void
MainWindow::zoomOut()
{
    qDebug("zoomOut()");
    View* gview = activeView();
    if (gview) {
        gview->zoomOut();
    }
}

void
MainWindow::zoomSelected()
{
    qDebug("zoomSelected()");
    View* gview = activeView();
    QUndoStack* stack = gview->getUndoStack();
    if (gview && stack) {
        UndoableCommand* cmd = new UndoableCommand(ACTION_NAV, "ZoomSelected", gview, 0);
        stack->push(cmd);
    }
}

void
MainWindow::zoomAll()
{
    qDebug("zoomAll()");
    stub_implement("Implement zoomAll.");
}

void
MainWindow::zoomExtents()
{
    qDebug("zoomExtents()");
    View* gview = activeView();
    QUndoStack* stack = gview->getUndoStack();
    if (gview && stack) {
        UndoableCommand* cmd = new UndoableCommand(ACTION_NAV, "ZoomExtents", gview, 0);
        stack->push(cmd);
    }
}

void
MainWindow::doNothing()
{
    //This function intentionally does nothing.
    qDebug("doNothing()");
}

void
MainWindow::layerSelectorIndexChanged(int index)
{
    qDebug("layerSelectorIndexChanged(%d)", index);
}

void
MainWindow::colorSelectorIndexChanged(int index)
{
    qDebug("colorSelectorIndexChanged(%d)", index);

    QComboBox* comboBox = qobject_cast<QComboBox*>(sender());
    QRgb newColor;
    if (comboBox) {
        bool ok = 0;
        //TODO: Handle ByLayer and ByBlock and Other...
        newColor = comboBox->itemData(index).toUInt(&ok);
        if (!ok)
            QMessageBox::warning(this, tr("Color Selector Conversion Error"), tr("<b>An error has occured while changing colors.</b>"));
    }
    else {
        QMessageBox::warning(this, tr("Color Selector Pointer Error"), tr("<b>An error has occured while changing colors.</b>"));
    }

    MdiWindow* mdiWin = qobject_cast<MdiWindow*>(mdiArea->activeSubWindow());
    if (mdiWin) { mdiWin->currentColorChanged(newColor); }
}

void
MainWindow::linetypeSelectorIndexChanged(int index)
{
    qDebug("linetypeSelectorIndexChanged(%d)", index);
}

void
MainWindow::lineweightSelectorIndexChanged(int index)
{
    qDebug("lineweightSelectorIndexChanged(%d)", index);
}

void
MainWindow::textFontSelectorCurrentFontChanged(const QFont& font)
{
    qDebug("textFontSelectorCurrentFontChanged()");
    setTextFont(font.family());
}

void
MainWindow::textSizeSelectorIndexChanged(int index)
{
    qDebug("textSizeSelectorIndexChanged(%d)", index);
    // TODO: check that the toReal() conversion is ok
    text_size.setting = qFabs(textSizeSelector->itemData(index).toReal());
}

void
MainWindow::setTextFont(const QString& str)
{
    textFontSelector->setCurrentFont(QFont(str));
    strcpy(text_font.setting, qPrintable(str));
}

void
MainWindow::setTextSize(double num)
{
    text_size.setting = qFabs(num);
    int index = textSizeSelector->findText("Custom", Qt::MatchContains);
    if (index != -1) {
        textSizeSelector->removeItem(index);
    }
    textSizeSelector->addItem("Custom " + QString().setNum(num, 'f', 2) + " pt", num);
    index = textSizeSelector->findText("Custom", Qt::MatchContains);
    if (index != -1) {
        textSizeSelector->setCurrentIndex(index);
    }
}

QString
MainWindow::getCurrentLayer()
{
    MdiWindow* mdiWin = qobject_cast<MdiWindow*>(mdiArea->activeSubWindow());
    if (mdiWin) {
        return mdiWin->curLayer;
    }
    return "0";
}

QRgb
MainWindow::getCurrentColor()
{
    MdiWindow* mdiWin = qobject_cast<MdiWindow*>(mdiArea->activeSubWindow());
    if (mdiWin) {
        return mdiWin->curColor;
    }
    return 0; //TODO: return color ByLayer
}

QString
MainWindow::getCurrentLineType()
{
    MdiWindow* mdiWin = qobject_cast<MdiWindow*>(mdiArea->activeSubWindow());
    if (mdiWin) {
        return mdiWin->curLineType;
    }
    return "ByLayer";
}

QString MainWindow::getCurrentLineWeight()
{
    MdiWindow* mdiWin = qobject_cast<MdiWindow*>(mdiArea->activeSubWindow());
    if (mdiWin) {
        return mdiWin->curLineWeight;
    }
    return "ByLayer";
}

void
MainWindow::deletePressed()
{
    qDebug("deletePressed()");
    QApplication::setOverrideCursor(Qt::WaitCursor);
    MdiWindow* mdiWin = qobject_cast<MdiWindow*>(mdiArea->activeSubWindow());
    if (mdiWin) {
        mdiWin->deletePressed();
    }
    QApplication::restoreOverrideCursor();
}

void
MainWindow::escapePressed()
{
    qDebug("escapePressed()");
    QApplication::setOverrideCursor(Qt::WaitCursor);
    MdiWindow* mdiWin = qobject_cast<MdiWindow*>(mdiArea->activeSubWindow());
    if (mdiWin) {
        mdiWin->escapePressed();
    }
    QApplication::restoreOverrideCursor();

    end_command();
}

void
MainWindow::toggleGrid()
{
    qDebug("toggleGrid()");
    statusbar->statusBarGridButton->toggle();
}

void
MainWindow::toggleRuler()
{
    qDebug("toggleRuler()");
    statusbar->statusBarRulerButton->toggle();
}

void
MainWindow::toggleLwt()
{
    qDebug("toggleLwt()");
    statusbar->statusBarLwtButton->toggle();
}

void
MainWindow::enablePromptRapidFire()
{
    prompt->enableRapidFire();
}

void
MainWindow::disablePromptRapidFire()
{
    prompt->disableRapidFire();
}

void
MainWindow::enableMoveRapidFire()
{
    View* gview = activeView();
    if (gview) {
        gview->enableMoveRapidFire();
    }
}

void
MainWindow::disableMoveRapidFire()
{
    View* gview = activeView();
    if (gview) {
        gview->disableMoveRapidFire();
    }
}

void
MainWindow::promptHistoryAppended(const QString& txt)
{
    MdiWindow* mdiWin = activeMdiWindow();
    if (mdiWin) {
        mdiWin->promptHistoryAppended(txt);
    }
}

void
MainWindow::logPromptInput(const QString& txt)
{
    MdiWindow* mdiWin = activeMdiWindow();
    if (mdiWin) mdiWin->logPromptInput(txt);
}

void
MainWindow::promptInputPrevious()
{
    MdiWindow* mdiWin = activeMdiWindow();
    if (mdiWin) mdiWin->promptInputPrevious();
}

void
MainWindow::promptInputNext()
{
    MdiWindow* mdiWin = activeMdiWindow();
    if (mdiWin) mdiWin->promptInputNext();
}

ScriptValue
MainWindow::runCommandCore(const QString& cmd, ScriptEnv *context)
{
    int id = get_command_id(qPrintable(cmd));
    qDebug("runCommandCore(%s) %d", qPrintable(cmd), id);
    if (id < 0) {
        qDebug("ERROR: %s not found in command_data.", qPrintable(cmd));
        return script_false;
    }

    if (!argument_checks(context, command_data[id].command, command_data[id].arguments)) {
        /* TODO: error */
        return script_false;
    }

    if (!(command_data[id].flags & DONT_INITIALIZE)) {
        init_command();
    }
    if (command_data[id].flags & CLEAR_SELECTION) {
        clear_selection();
    }

    ScriptValue value = script_true;

    qDebug("switch");
    switch (command_data[id].id) {
    case ACTION_ABOUT:
        about_dialog();
        break;
    case ACTION_ALERT:
        prompt->alert(STR(0));
        break;
    case ACTION_ANGLE:
        prompt->alert("TODO: ANGLE");
        break;
    case ACTION_CHANGELOG:
        prompt->alert("TODO: CHANGELOG");
        break;
    case ACTION_CLEAR:
        /* This is covered by the flags. */
        break;
    case ACTION_COPY:
        prompt->alert("TODO: COPY");
        break;
    case ACTION_COLOR_SELECTOR:
        prompt->alert("TODO: COLORSELECTOR");
        break;
    case ACTION_CUT:
        prompt->alert("TODO: CUT");
        break;
    case ACTION_DEBUG:
        nativeAppendPromptHistory(STR(0));
        break;
    case ACTION_DESIGN_DETAILS:
        designDetails();
        break;
    case ACTION_DO_NOTHING:
        break;
    case ACTION_EXIT:
        exit();
        break;
    case ACTION_HELP:
        help();
        break;
    case ACTION_ICON_128:
        iconResize(128);
        break;
    case ACTION_ICON_16:
        iconResize(16);
        break;
    case ACTION_ICON_24:
        iconResize(24);
        break;
    case ACTION_ICON_32:
        iconResize(32);
        break;
    case ACTION_ICON_48:
        iconResize(48);
        break;
    case ACTION_ICON_64:
        iconResize(64);
        break;
    case ACTION_MIRROR_SELECTED:
        nativeMirrorSelected(REAL(0), REAL(1), REAL(2), REAL(3));
        break;
    case ACTION_NEW:
        newFile();
        break;
    case ACTION_OPEN:
        openFile();
        break;
    case ACTION_PASTE:
        break;
    case ACTION_PLATFORM:
        /* Should this display in the command prompt or just return like GET? */
        // prompt_output(translate("Platform") + " = " + _main->platformString());
        break;
    case ACTION_REDO:
        redo();
        break;
    case ACTION_SAVE:
        /* save(); */
        break;
    case ACTION_SAVE_AS:
        /* save(); */
        break;
    case ACTION_SCALE_SELECTED:
        /*  */
        break;
    case ACTION_SETTINGS_DIALOG:
        settingsDialog();
        break;
    case ACTION_TEXT_BOLD:
        text_style_bold.setting = !text_style_bold.setting;
        break;
    case ACTION_TEXT_ITALIC:
        text_style_italic.setting = !text_style_italic.setting;
        break;
    case ACTION_TEXT_UNDERLINE:
        text_style_underline.setting = !text_style_underline.setting;
        break;
    case ACTION_TEXT_STRIKEOUT:
        text_style_strikeout.setting = !text_style_strikeout.setting;
        break;
    case ACTION_TEXT_OVERLINE:
        text_style_overline.setting = !text_style_overline.setting;
        break;
    case ACTION_TIP_OF_THE_DAY:
        tipOfTheDay();
        break;
    case ACTION_TODO: {
        QString s = "TODO: (" + QString(STR(0)) + ") " + QString(STR(1));
        _main->prompt->alert(s);
        break;
    }
    case ACTION_UNDO:
        undo();
        break;
    case ACTION_VULCANIZE:
        nativeVulcanize();
        break;
    
/*
    case ACTION_DO_NOTHING:

    case ACTION_DESIGN_DETAILS:
    case ACTION_PRINT:

    case ACTION_WINDOW_CLOSE:
    case ACTION_WINDOW_CLOSE_ALL:
    case ACTION_WINDOW_CASCADE:
    case ACTION_WINDOW_TILE:
    case ACTION_WINDOW_NEXT:
    case ACTION_WINDOW_PREVIOUS:

    case ACTION_HELP:
    case ACTION_CHANGELOG:
    case ACTION_ABOUT:
    case ACTION_WHATS_THIS:

    case ACTION_SETTINGS_DIALOG:

    case ACTION_MAKE_LAYER_CURRENT:
    case ACTION_LAYERS:
    case ACTION_LAYER_SELECTOR:
    case ACTION_LAYER_PREVIOUS:
    case ACTION_COLOR_SELECTOR:
    case ACTION_LINE_TYPE_SELECTOR:
    case ACTION_LINE_WEIGHT_SELECTOR:
    case ACTION_HIDE_ALL_LAYERS:
    case ACTION_SHOW_ALL_LAYERS:
    case ACTION_FREEZE_ALL_LAYERS:
    case ACTION_THAW_ALL_LAYERS:
    case ACTION_LOCK_ALL_LAYERS:
    case ACTION_UNLOCK_ALL_LAYERS:

    case ACTION_ZOOM_REAL_TIME:
    case ACTION_ZOOM_PREVIOUS:
    case ACTION_ZOOM_WINDOW:
    case ACTION_ZOOM_DYNAMIC:
    case ACTION_ZOOM_SCALE:
    case ACTION_ZOOM_CENTER:
    case ACTION_ZOOM_IN:
    case ACTION_ZOOM_OUT:
    case ACTION_ZOOM_SELECTED:
    case ACTION_ZOOM_ALL:
    case ACTION_ZOOM_EXTENTS:

    case ACTION_PAN_REAL_TIME:
    case ACTION_PAN_POINT:
    case ACTION_PAN_LEFT:
    case ACTION_PAN_RIGHT:
    case ACTION_PAN_UP:
    case ACTION_PAN_DOWN:

    case ACTION_DAY:
    case ACTION_NIGHT:

    case ACTION_ALERT:
    case ACTION_GET:
    case ACTION_SET:

    case ACTION_CLEAR:

    case ACTION_ANGLE:
    case ACTION_CIRCLE:
    case ACTION_DEBUG:
    case ACTION_DISABLE:
    case ACTION_DISTANCE:
    case ACTION_DOLPHIN:
    case ACTION_ELLIPSE:
    case ACTION_ENABLE:
    case ACTION_ERASE:
    case ACTION_ERROR:
    case ACTION_HEART:
    case ACTION_LINE:
    case ACTION_LOCATE_POINT:
    case ACTION_MIRROR_SELECTED:
    case ACTION_MOVE:
    case ACTION_MOVE_SELECTED:
    case ACTION_PATH:
    case ACTION_PLATFORM:
    case ACTION_POINT:
    case ACTION_POLYGON:
    case ACTION_POLYLINE:
    case ACTION_PREVIEW_OFF:
    case ACTION_PREVIEW_ON:
    case ACTION_QUICKLEADER:
    case ACTION_RECTANGLE:
    case ACTION_RGB:
    case ACTION_ROTATE:
    case ACTION_SANDBOX:
    case ACTION_SCALE:
    case ACTION_SCALE_SELECTED,
    case ACTION_SELECT_ALL:
    case ACTION_SINGLE_LINE_TEXT:
    case ACTION_SNOWFLAKE:
    case ACTION_STAR:
    case ACTION_SYSWINDOWS:
    case ACTION_TODO:
    case ACTION_VULCANIZE:

    case ACTION_ADD:
    case ACTION_DELETE:
    case ACTION_GRIP_EDIT:
    case ACTION_NAV:
    case ACTION_MIRROR:

    case ACTION_TEST:

    */
    default:
        break;
    }

    if (!(command_data[id].flags & DONT_END_COMMAND)) {
        end_command();
    }
    return value;
}

void
MainWindow::runCommand()
{
    QAction* act = qobject_cast<QAction*>(sender());
    if (act) {
        qDebug("runCommand(%s)", qPrintable(act->objectName()));
        prompt->endCommand();
        prompt->setCurrentText(act->objectName());
        prompt->processInput();
    }
}

/* FIXME: reconnect to new command system.
 */
void
MainWindow::runCommandMain(const QString& cmd)
{
    ScriptEnv *context = create_script_env();
    context->context = CONTEXT_MAIN;
    qDebug("runCommandMain(%s)", qPrintable(cmd));
    // TODO: Uncomment this when post-selection is available
    /*
    if (!selection_mode_pick_first.setting) {
        nativeClearSelection();
    }
    */
    runCommandCore(cmd, context);
    free_script_env(context);
}

/* FIXME: reconnect to new command system.
 */
void
MainWindow::runCommandClick(const QString& cmd, double x, double y)
{
    ScriptEnv *context = create_script_env();
    context->context = CONTEXT_CLICK;
    qDebug("runCommandClick(%s, %.2f, %.2f)", qPrintable(cmd), x, y);
    //engine->evaluate(cmd + "_click(" + QString().setNum(x) + "," + QString().setNum(-y) + ")", fileName);
    runCommandCore(cmd, context);
    free_script_env(context);
}

/* FIXME: reconnect to new command system.
 */
void
MainWindow::runCommandMove(const QString& cmd, double x, double y)
{
    ScriptEnv *context = create_script_env();
    context->context = CONTEXT_MOVE;
    qDebug("runCommandMove(%s, %.2f, %.2f)", qPrintable(cmd), x, y);
    //engine->evaluate(cmd + "_move(" + QString().setNum(x) + "," + QString().setNum(-y) + ")", fileName);
    runCommandCore(cmd, context);
    free_script_env(context);
}

/* FIXME: reconnect to new command system.
 */
void
MainWindow::runCommandContext(const QString& cmd, const QString& str)
{
    ScriptEnv *context = create_script_env();
    context->context = CONTEXT_CONTEXT;
    qDebug("runCommandContext(%s, %s)", qPrintable(cmd), qPrintable(str));
    //engine->evaluate(cmd + "_context('" + str.toUpper() + "')", fileName);
    runCommandCore(cmd, context);
    free_script_env(context);
}

/* FIXME: reconnect to new command system.
 * NOTE: Replace any special characters that will cause a syntax error
 */
void
MainWindow::runCommandPrompt(const QString& cmd)
{
    ScriptEnv *context = create_script_env();
    qDebug("runCommandPrompt(%s)", qPrintable(cmd));
    context->context = CONTEXT_PROMPT;
    if (prompt->isRapidFireEnabled()) {
        runCommandCore(cmd, context);
    }
    else {
        /* Both branches run the same. */
        runCommandCore(cmd, context);
    }
    free_script_env(context);
}

void
nativeBlinkPrompt()
{
    _main->prompt->startBlinking();
}

void
nativeSetPromptPrefix(std::string txt)
{
    _main->prompt->setPrefix(QString(txt.c_str()));
}

void
nativeAppendPromptHistory(std::string txt)
{
    _main->prompt->appendHistory(QString(txt.c_str()));
}

void
nativeEnablePromptRapidFire()
{
    _main->enablePromptRapidFire();
}

void
nativeDisablePromptRapidFire()
{
    _main->disablePromptRapidFire();
}

void
nativeEnableMoveRapidFire()
{
    _main->enableMoveRapidFire();
}

void
nativeDisableMoveRapidFire()
{
    _main->disableMoveRapidFire();
}

void
messageBox(std::string msgType, std::string title, std::string text)
{
    if (msgType == "critical") {
        QMessageBox::critical(_main,
            translate(title.c_str()), translate(text.c_str()));
    }
    else if (msgType == "information") {
        QMessageBox::information(_main,
            translate(title.c_str()), translate(text.c_str()));
    }
    else if (msgType == "question") {
        QMessageBox::question(_main,
            translate(title.c_str()), translate(text.c_str()));
    }
    else if (msgType == "warning") {
        QMessageBox::warning(_main,
            translate(title.c_str()), translate(text.c_str()));
    }
    else {
        QMessageBox::critical(_main,
            translate(title.c_str()), translate(text.c_str()));
    }
}

void
nativePrintArea(double x, double y, double w, double h)
{
    qDebug("nativePrintArea(%.2f, %.2f, %.2f, %.2f)", x, y, w, h);
    //TODO: Print Setup Stuff
    _main->print();
}

void
nativeSetBackgroundColor(uint8_t r, uint8_t g, uint8_t b)
{
    display_bg_color.setting = qRgb(r,g,b);
    _main->updateAllViewBackgroundColors(qRgb(r,g,b));
}

void
nativeSetCrossHairColor(uint8_t r, uint8_t g, uint8_t b)
{
    display_crosshair_color.setting = qRgb(r,g,b);
    _main->updateAllViewCrossHairColors(qRgb(r,g,b));
}

void
nativeSetGridColor(uint8_t r, uint8_t g, uint8_t b)
{
    grid_color.setting = qRgb(r,g,b);
    _main->updateAllViewGridColors(qRgb(r,g,b));
}

void
nativeVulcanize()
{
    View* gview = activeView();
    if (gview) {
        gview->vulcanizeRubberRoom();
    }
}

void
nativeClearRubber()
{
    View* gview = activeView();
    if (gview) {
        gview->clearRubberRoom();
    }
}

bool
nativeAllowRubber()
{
    View* gview = activeView();
    if (gview) {
        return gview->allowRubber();
    }
    return false;
}

void
nativeSpareRubber(int64_t id)
{
    View* gview = activeView();
    if (gview) {
        gview->spareRubber(id);
    }
}

/* . */
void
nativeSetRubberMode(int mode)
{
    View* gview = activeView();
    if (gview) {
        gview->setRubberMode(mode);
    }
}

/* . */
void
nativeSetRubberPoint(std::string key, double x, double y)
{
    View* gview = activeView();
    if (gview) {
        gview->setRubberPoint(QString(key.c_str()), QPointF(x, -y));
    }
}

/* . */
void
nativeSetRubberText(std::string key, std::string txt)
{
    View* gview = activeView();
    if (gview) {
        gview->setRubberText(QString(key.c_str()), QString(txt.c_str()));
    }
}

/* . */
void
nativeAddTextMulti(std::string str, double x, double y, double rot, bool fill, int rubberMode)
{
}

/* . */
void
nativeAddTextSingle(std::string str, double x, double y, double rot, bool fill, int rubberMode)
{
    View* gview = activeView();
    QGraphicsScene* gscene = gview->scene();
    QUndoStack* stack = gview->getUndoStack();
    if (gview && gscene && stack) {
        Object* obj = new Object(QString(str.c_str()), x, -y, _main->getCurrentColor());
        obj->setObjectTextFont(text_font.setting);
        obj->setObjectTextSize(text_size.setting);
        obj->setObjectTextStyle(text_style_bold.setting,
                                text_style_italic.setting,
                                text_style_underline.setting,
                                text_style_strikeout.setting,
                                text_style_overline.setting);
        obj->setObjectTextBackward(false);
        obj->setObjectTextUpsideDown(false);
        obj->setRotation(-rot);
        //TODO: single line text fill
        obj->setObjectRubberMode(rubberMode);
        if (rubberMode) {
            gview->addToRubberRoom(obj);
            gscene->addItem(obj);
            gscene->update();
        }
        else {
            UndoableCommand* cmd = new UndoableCommand(ACTION_ADD, obj->data(OBJ_NAME).toString(), obj, gview, 0);
            stack->push(cmd);
        }
    }
}

/* . */
void
nativeAddInfiniteLine(double x1, double y1, double x2, double y2, double rot)
{
}

/* . */
void
nativeAddRay(double x1, double y1, double x2, double y2, double rot)
{
}

/* . */
void
nativeAddLine(double x1, double y1, double x2, double y2, double rot, int rubberMode)
{
    View* gview = activeView();
    QGraphicsScene* gscene = gview->scene();
    QUndoStack* stack = gview->getUndoStack();
    if (gview && gscene && stack) {
        Object* obj = new Object(x1, -y1, x2, -y2, _main->getCurrentColor());
        obj->setRotation(-rot);
        obj->setObjectRubberMode(rubberMode);
        if (rubberMode) {
            gview->addToRubberRoom(obj);
            gscene->addItem(obj);
            gscene->update();
        }
        else {
            UndoableCommand* cmd = new UndoableCommand(ACTION_ADD, obj->data(OBJ_NAME).toString(), obj, gview, 0);
            stack->push(cmd);
        }
    }
}

/* . */
void
nativeAddTriangle(double x1, double y1, double x2, double y2, double x3, double y3, double rot, bool fill)
{
}

/* . */
void
nativeAddRectangle(double x, double y, double w, double h, double rot, bool fill, int rubberMode)
{
    View* gview = activeView();
    QGraphicsScene* gscene = gview->scene();
    QUndoStack* stack = gview->getUndoStack();
    if (!(gview && gscene && stack)) {
        return;
    }
    Object* obj = new Object(x, -y, w, -h, _main->getCurrentColor());
    obj->setRotation(-rot);
    obj->setObjectRubberMode(rubberMode);
    //TODO: rect fill
    if (rubberMode) {
        gview->addToRubberRoom(obj);
        gscene->addItem(obj);
        gscene->update();
    }
    else {
        UndoableCommand* cmd = new UndoableCommand(ACTION_ADD, obj->data(OBJ_NAME).toString(), obj, gview, 0);
        stack->push(cmd);
    }
}

/* . */
void
nativeAddRoundedRectangle(double x, double y, double w, double h, double rad, double rot, bool fill)
{
}

void
nativeAddArc(EmbArc arc, int rubberMode)
{
    View* gview = activeView();
    QGraphicsScene* scene = activeScene();
    if (gview && scene) {
        arc.start.y = -arc.start.y;
        arc.mid.y = -arc.mid.y;
        arc.end.y = -arc.end.y;
        Object* arcObj = new Object(arc, _main->getCurrentColor());
        arcObj->setObjectRubberMode(rubberMode);
        if (rubberMode) {
            gview->addToRubberRoom(arcObj);
        }
        scene->addItem(arcObj);
        scene->update();
    }
}

void
nativeAddCircle(double centerX, double centerY, double radius, bool fill, int rubberMode)
{
    View* gview = activeView();
    QGraphicsScene* gscene = gview->scene();
    QUndoStack* stack = gview->getUndoStack();
    if (gview && gscene && stack) {
        EmbCircle circle;
        circle.center.x = centerX;
        circle.center.y = -centerY;
        circle.radius = radius;
        Object* obj = new Object(circle, _main->getCurrentColor());
        obj->setObjectRubberMode(rubberMode);
        //TODO: circle fill
        if (rubberMode) {
            gview->addToRubberRoom(obj);
            gscene->addItem(obj);
            gscene->update();
        }
        else {
            UndoableCommand* cmd = new UndoableCommand(ACTION_ADD, obj->data(OBJ_NAME).toString(), obj, gview, 0);
            stack->push(cmd);
        }
    }
}

void
nativeAddSlot(double centerX, double centerY, double diameter, double length, double rot, bool fill, int rubberMode)
{
    //TODO: Use UndoableCommand for slots
    /*
    Object* slotObj = new Object(centerX, -centerY, diameter, length, _main->getCurrentColor());
    slotObj->setRotation(-rot);
    slotObj->setObjectRubberMode(rubberMode);
    if (rubberMode) gview->addToRubberRoom(slotObj);
    scene->addItem(slotObj);
    //TODO: slot fill
    scene->update();
    */
}

void
nativeAddEllipse(double centerX, double centerY, double width, double height, double rot, bool fill, int rubberMode)
{
    View* gview = activeView();
    QGraphicsScene* gscene = gview->scene();
    QUndoStack* stack = gview->getUndoStack();
    if (gview && gscene && stack) {
        EmbEllipse ellipse;
        ellipse.center.x = centerX;
        ellipse.center.y = -centerY;
        ellipse.radius.x = width/2.0;
        ellipse.radius.y = height/2.0;
        Object* obj = new Object(ellipse, _main->getCurrentColor());
        obj->setRotation(-rot);
        obj->setObjectRubberMode(rubberMode);
        //TODO: ellipse fill
        if (rubberMode) {
            gview->addToRubberRoom(obj);
            gscene->addItem(obj);
            gscene->update();
        }
        else {
            UndoableCommand* cmd = new UndoableCommand(ACTION_ADD, obj->data(OBJ_NAME).toString(), obj, gview, 0);
            stack->push(cmd);
        }
    }
}

void
nativeAddPoint(double x, double y)
{
    View* gview = activeView();
    QUndoStack* stack = gview->getUndoStack();
    if (gview && stack) {
        EmbPoint point;
        point.position.x = x;
        point.position.y = -y;
        Object* obj = new Object(point, _main->getCurrentColor());
        UndoableCommand* cmd = new UndoableCommand(ACTION_ADD, obj->data(OBJ_NAME).toString(), obj, gview, 0);
        stack->push(cmd);
    }
}

void
nativeAddRegularPolygon(double centerX, double centerY, quint16 sides, uint8_t mode, double rad, double rot, bool fill)
{
}

//NOTE: This native is different than the rest in that the Y+ is down (scripters need not worry about this)
void
nativeAddPolygon(double startX, double startY, const QPainterPath& p, int rubberMode)
{
    View* gview = activeView();
    QGraphicsScene* gscene = gview->scene();
    QUndoStack* stack = gview->getUndoStack();
    if (gview && gscene && stack) {
        EmbPolygon polygon;
        Object* obj = new Object(polygon, OBJ_TYPE_POLYGON, p, _main->getCurrentColor());
        obj->setObjectRubberMode(rubberMode);
        if (rubberMode) {
            gview->addToRubberRoom(obj);
            gscene->addItem(obj);
            gscene->update();
        }
        else {
            UndoableCommand* cmd = new UndoableCommand(ACTION_ADD, obj->data(OBJ_NAME).toString(), obj, gview, 0);
            stack->push(cmd);
        }
    }
}

//NOTE: This native is different than the rest in that the Y+ is down (scripters need not worry about this)
void
nativeAddPolyline(double startX, double startY, const QPainterPath& p, int rubberMode)
{
    View* gview = activeView();
    QGraphicsScene* gscene = gview->scene();
    QUndoStack* stack = gview->getUndoStack();
    if (gview && gscene && stack) {
        EmbPath path;
        Object* obj = new Object(path, OBJ_TYPE_POLYLINE, p, _main->getCurrentColor());
        obj->setObjectRubberMode(rubberMode);
        if (rubberMode) {
            gview->addToRubberRoom(obj);
            gscene->addItem(obj);
            gscene->update();
        }
        else {
            UndoableCommand* cmd = new UndoableCommand(ACTION_ADD, obj->data(OBJ_NAME).toString(), obj, gview, 0);
            stack->push(cmd);
        }
    }
}

//NOTE: This native is different than the rest in that the Y+ is down (scripters need not worry about this)
void
nativeAddPath(double startX, double startY, const QPainterPath& p, int rubberMode)
{
}

void
nativeAddHorizontalDimension(double x1, double y1, double x2, double y2, double legHeight)
{
}

void
nativeAddVerticalDimension(double x1, double y1, double x2, double y2, double legHeight)
{
}

void
nativeAddImage(const QString& img, double x, double y, double w, double h, double rot)
{
}

void
nativeAddDimLeader(double x1, double y1, double x2, double y2, double rot, int rubberMode)
{
    View* gview = activeView();
    QGraphicsScene* gscene = gview->scene();
    QUndoStack* stack = gview->getUndoStack();
    if (gview && gscene && stack) {
        Object* obj = new Object(x1, -y1, x2, -y2, _main->getCurrentColor());
        obj->setRotation(-rot);
        obj->setObjectRubberMode(rubberMode);
        if (rubberMode) {
            gview->addToRubberRoom(obj);
            gscene->addItem(obj);
            gscene->update();
        }
        else {
            UndoableCommand* cmd = new UndoableCommand(ACTION_ADD, obj->data(OBJ_NAME).toString(), obj, gview, 0);
            stack->push(cmd);
        }
    }
}

void
nativeSetCursorShape(char shape[MAX_STRING_LENGTH])
{
    View* gview = activeView();
    if (gview) {
        if (!strcmp(shape, "arrow"))
            gview->setCursor(QCursor(Qt::ArrowCursor));
        else if (!strcmp(shape, "uparrow"))
            gview->setCursor(QCursor(Qt::UpArrowCursor));
        else if (!strcmp(shape, "cross"))
            gview->setCursor(QCursor(Qt::CrossCursor));
        else if (!strcmp(shape, "wait"))
            gview->setCursor(QCursor(Qt::WaitCursor));
        else if (!strcmp(shape, "ibeam"))
            gview->setCursor(QCursor(Qt::IBeamCursor));
        else if (!strcmp(shape, "resizevert"))
            gview->setCursor(QCursor(Qt::SizeVerCursor));
        else if (!strcmp(shape, "resizehoriz"))
            gview->setCursor(QCursor(Qt::SizeHorCursor));
        else if (!strcmp(shape, "resizediagleft"))
            gview->setCursor(QCursor(Qt::SizeBDiagCursor));
        else if (!strcmp(shape, "resizediagright"))
            gview->setCursor(QCursor(Qt::SizeFDiagCursor));
        else if (!strcmp(shape, "move"))
            gview->setCursor(QCursor(Qt::SizeAllCursor));
        else if (!strcmp(shape, "blank"))
            gview->setCursor(QCursor(Qt::BlankCursor));
        else if (!strcmp(shape, "splitvert"))
            gview->setCursor(QCursor(Qt::SplitVCursor));
        else if (!strcmp(shape, "splithoriz"))
            gview->setCursor(QCursor(Qt::SplitHCursor));
        else if (!strcmp(shape, "handpointing"))
            gview->setCursor(QCursor(Qt::PointingHandCursor));
        else if (!strcmp(shape, "forbidden"))
            gview->setCursor(QCursor(Qt::ForbiddenCursor));
        else if (!strcmp(shape, "handopen"))
            gview->setCursor(QCursor(Qt::OpenHandCursor));
        else if (!strcmp(shape, "handclosed"))
            gview->setCursor(QCursor(Qt::ClosedHandCursor));
        else if (!strcmp(shape, "whatsthis"))
            gview->setCursor(QCursor(Qt::WhatsThisCursor));
        else if (!strcmp(shape, "busy"))
            gview->setCursor(QCursor(Qt::BusyCursor));
        else if (!strcmp(shape, "dragmove"))
            gview->setCursor(QCursor(Qt::DragMoveCursor));
        else if (!strcmp(shape, "dragcopy"))
            gview->setCursor(QCursor(Qt::DragCopyCursor));
        else if (!strcmp(shape, "draglink"))
            gview->setCursor(QCursor(Qt::DragLinkCursor));
    }
}

double nativeCalculateAngle(double x1, double y1, double x2, double y2)
{
    return QLineF(x1, -y1, x2, -y2).angle();
}

double nativeCalculateDistance(double x1, double y1, double x2, double y2)
{
    return QLineF(x1, y1, x2, y2).length();
}

double nativePerpendicularDistance(double px, double py, double x1, double y1, double x2, double y2)
{
    QLineF line(x1, y1, x2, y2);
    QLineF norm = line.normalVector();
    double dx = px-x1;
    double dy = py-y1;
    norm.translate(dx, dy);
    QPointF iPoint;
    norm.intersects(line, &iPoint);
    return QLineF(px, py, iPoint.x(), iPoint.y()).length();
}

void
nativeAddToSelection(const QPainterPath path, Qt::ItemSelectionMode mode)
{
}

void
nativeDeleteSelected()
{
    View* gview = activeView();
    if (gview) {
        gview->deleteSelected();
    }
}

void
nativeCutSelected(double x, double y)
{
}

void
nativeCopySelected(double x, double y)
{
}

void
nativePasteSelected(double x, double y)
{
}

void
nativeMoveSelected(double dx, double dy)
{
    View* gview = activeView();
    if (gview) { gview->moveSelected(dx, -dy); }
}

void
nativeScaleSelected(double x, double y, double factor)
{
    if (factor <= 0.0) {
        QMessageBox::critical(_main,
            QString(translate("ScaleFactor Error")),
            QString(translate("Hi there. If you are not a developer, report this as a bug. "
            "If you are a developer, your code needs examined, and possibly your head too.")));
    }

    View* gview = activeView();
    if (gview) {
        gview->scaleSelected(x, -y, factor);
    }
}

void
nativeRotateSelected(double x, double y, double rot)
{
    View* gview = activeView();
    if (gview) {
        gview->rotateSelected(x, -y, -rot);
    }
}

void
nativeMirrorSelected(double x1, double y1, double x2, double y2)
{
    View* gview = activeView();
    if (gview) {
        gview->mirrorSelected(x1, -y1, x2, -y2);
    }
}

double
nativeQSnapX()
{
    QGraphicsScene* scene = activeScene();
    if (scene) {
        return scene->property("SCENE_QSNAP_POINT").toPointF().x();
    }
    return 0.0;
}

double
nativeQSnapY()
{
    QGraphicsScene* scene = activeScene();
    if (scene) {
        return -scene->property("SCENE_QSNAP_POINT").toPointF().y();
    }
    return 0.0;
}
