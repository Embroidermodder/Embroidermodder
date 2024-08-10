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

void checkBoxTipOfTheDayStateChanged(int checked);
void buttonTipOfTheDayClicked(int button);

ScriptValue previewon_command(ScriptEnv *context);
ScriptValue get_command(ScriptEnv *context);
ScriptValue set_command(ScriptEnv *context);
ScriptValue move_command(ScriptEnv *context);
ScriptValue sandbox_command(ScriptEnv *context);

/* . */
void
MainWindow::stub_implement(QString txt)
{
    qDebug("TODO: %s", qPrintable(txt));
}

/* . */
void
MainWindow::stub_testing()
{
    QMessageBox::warning(this, translate("Testing Feature"),
        translate("<b>This feature is in testing.</b>"));
}

/* . */
void
MainWindow::exit(void)
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

/* . */
void
MainWindow::checkForUpdates(void)
{
    qDebug("checkForUpdates()");
    //TODO: Check website for new versions, commands, etc...
}

/* . */
QString
platformString(void)
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

/* . */
void
MainWindow::print()
{
    qDebug("print()");
    MdiWindow* mdiWin = qobject_cast<MdiWindow*>(mdiArea->activeSubWindow());
    if (mdiWin) {
        mdiWin->print();
    }
}

/* . */
void
tipOfTheDay()
{
    qDebug("tipOfTheDay()");

    QString appDir = qApp->applicationDirPath();

    wizardTipOfTheDay = new QWizard(_main);
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

    QCheckBox* checkBoxTipOfTheDay = new QCheckBox(translate("&Show tips on startup"), wizardTipOfTheDay);
    checkBoxTipOfTheDay->setChecked(general_tip_of_the_day.setting);
    QObject::connect(checkBoxTipOfTheDay, SIGNAL(stateChanged(int)), _main, SLOT(checkBoxTipOfTheDayStateChanged(int)));

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
    QObject::connect(wizardTipOfTheDay, SIGNAL(customButtonClicked(int)), _main, SLOT(buttonTipOfTheDayClicked(int)));

    QList<QWizard::WizardButton> listTipOfTheDayButtons;
    listTipOfTheDayButtons << QWizard::Stretch << QWizard::CustomButton1
        << QWizard::CustomButton2 << QWizard::CustomButton3;
    wizardTipOfTheDay->setButtonLayout(listTipOfTheDayButtons);

    wizardTipOfTheDay->exec();
}

void
checkBoxTipOfTheDayStateChanged(int checked)
{
    general_tip_of_the_day.setting = checked;
}

/* . */
void
buttonTipOfTheDayClicked(int button)
{
    qDebug("buttonTipOfTheDayClicked(%d)", button);
    if (button == QWizard::CustomButton1) {
        if (general_current_tip.setting > 0) {
            general_current_tip.setting--;
        }
        else {
            general_current_tip.setting = listTipOfTheDay.size()-1;
        }
        labelTipOfTheDay->setText(listTipOfTheDay.value(general_current_tip.setting));
    }
    else if (button == QWizard::CustomButton2) {
        general_current_tip.setting++;
        if (general_current_tip.setting >= listTipOfTheDay.size()) {
            general_current_tip.setting = 0;
        }
        labelTipOfTheDay->setText(listTipOfTheDay.value(general_current_tip.setting));
    }
    else if (button == QWizard::CustomButton3) {
        wizardTipOfTheDay->close();
    }
}

/* . */
void
help(void)
{
    debug_message("help()");

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

/* . */
void
changelog(void)
{
    debug_message("changelog()");

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
    MdiWindow* mdiWin = qobject_cast<MdiWindow*>(mdiArea->activeSubWindow());
    return mdiWin;
}

View*
activeView()
{
    _main->debug_message("activeView()");
    MdiWindow* mdiWin = qobject_cast<MdiWindow*>(mdiArea->activeSubWindow());
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
    MdiWindow* mdiWin = qobject_cast<MdiWindow*>(mdiArea->activeSubWindow());
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
        if (mdiWin) {
            mdiWin->setViewCrossHairColor(color);
        }
    }
}

void
MainWindow::updateAllViewBackgroundColors(QRgb color)
{
    QList<QMdiSubWindow*> windowList = mdiArea->subWindowList();
    for (int i = 0; i < windowList.count(); ++i) {
        MdiWindow* mdiWin = qobject_cast<MdiWindow*>(windowList.at(i));
        if (mdiWin) {
            mdiWin->setViewBackgroundColor(color);
        }
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
        if (!ok) {
            QMessageBox::warning(this, translate("Color Selector Conversion Error"),
                translate("<b>An error has occured while changing colors.</b>"));
        }
    }
    else {
        QMessageBox::warning(this, translate("Color Selector Pointer Error"),
            translate("<b>An error has occured while changing colors.</b>"));
    }

    MdiWindow* mdiWin = qobject_cast<MdiWindow*>(mdiArea->activeSubWindow());
    if (mdiWin) {
        mdiWin->currentColorChanged(newColor);
    }
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

/* . */
void
toggleGrid(void)
{
    debug_message("toggleGrid()");
    statusBarGridButton->toggle();
}

/* . */
void
toggleRuler(void)
{
    debug_message("toggleRuler()");
    statusBarRulerButton->toggle();
}

/* . */
void
toggleLwt(void)
{
    debug_message("toggleLwt()");
    statusBarLwtButton->toggle();
}

/* . */
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
    if (mdiWin) {
        mdiWin->logPromptInput(txt);
    }
}

void
MainWindow::promptInputPrevious()
{
    MdiWindow* mdiWin = activeMdiWindow();
    if (mdiWin) {
        mdiWin->promptInputPrevious();
    }
}

void
MainWindow::promptInputNext()
{
    MdiWindow* mdiWin = activeMdiWindow();
    if (mdiWin) {
        mdiWin->promptInputNext();
    }
}

ScriptValue
MainWindow::runCommandCore(const QString& cmd, ScriptEnv *context)
{
    int id = get_command_id(qPrintable(cmd));
    View* gview = NULL;
    ScriptValue value = script_true;
    qDebug("runCommandCore(%s) %d", qPrintable(cmd), id);
    if (id < 0) {
        qDebug("ERROR: %s not found in command_data.", qPrintable(cmd));
        return script_false;
    }

    if (!argument_checks(context, id)) {
        /* TODO: error */
        return script_false;
    }

    if (!(command_data[id].flags & DONT_INITIALIZE)) {
        init_command();
    }
    if (command_data[id].flags & CLEAR_SELECTION) {
        clear_selection();
    }
    if (command_data[id].flags & REQUIRED_VIEW) {
        gview = activeView();
        if (!gview) {
            return value;
        }
    }

    qDebug("switch");
    switch (command_data[id].id) {
    case ACTION_ABOUT:
        about_dialog();
        break;
    case ACTION_ALERT:
        prompt->alert(STR(0));
        break;
    case ACTION_ANGLE:
        value = script_real(QLineF(REAL(0), -REAL(1), REAL(2), -REAL(3)).angle());
        break;
    case ACTION_CHANGELOG:
        prompt->alert("TODO: CHANGELOG");
        break;
    case ACTION_CLEAR:
        /* This is covered by the flags. */
        break;
    case ACTION_COPY: {
        View* gview = activeView();
        if (gview) {
            gview->copy();
        }
        break;
    }
    case ACTION_COPY_SELECTED: {
        // nativeCopySelected(REAL(0), REAL(1));
        break;
    }
    case ACTION_COLOR_SELECTOR:
        prompt->alert("TODO: COLORSELECTOR");
        break;
    case ACTION_CUT:
        nativeCutSelected(REAL(0), REAL(1));
        break;

    case ACTION_DEBUG:
        prompt->appendHistory(QString(STR(0)));
        break;

    case ACTION_DESIGN_DETAILS:
        create_details_dialog();
        break;

    case ACTION_DIM_LEADER: {
        nativeAddDimLeader(REAL(0), REAL(1), REAL(2), REAL(3), REAL(4), OBJ_RUBBER_OFF);
        break;
    }

    case ACTION_DISABLE: {
        QString value(STR(0));
        if (value == "MOVERAPIDFIRE") {
            gview->disableMoveRapidFire();
        }
        if (value == "PROMPTRAPIDFIRE") {
            prompt->disableRapidFire();
        }
        break;
    }

    case ACTION_DO_NOTHING:
        break;

    case ACTION_ENABLE: {
        QString value(STR(0));
        if (value == "MOVERAPIDFIRE") {
            gview->enableMoveRapidFire();
        }
        if (value == "PROMPTRAPIDFIRE") {
            prompt->enableRapidFire();
        }
        break;
    }

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
    case ACTION_MIRROR_SELECTED: {
        gview->mirrorSelected(REAL(0), -REAL(1), REAL(2), -REAL(3));
        break;
    }
    case ACTION_NEW:
        newFile();
        break;
    case ACTION_OPEN:
        openFile();
        break;

    case ACTION_PASTE: {
        gview->paste();
        break;
    }

    case ACTION_PASTE_SELECTED: {
        /* Paste with location x,y */
        // nativePasteSelected(REAL(0), REAL(1));
        break;
    }
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

    case ACTION_SETTINGS_DIALOG: {
        settingsDialog();
        break;
    }

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
        prompt->alert(s);
        break;
    }

    case ACTION_UNDO:
        undo();
        break;

    case ACTION_VULCANIZE: {
        View* gview = activeView();
        if (gview) {
            gview->vulcanizeRubberRoom();
        }
        break;
    }

    case ACTION_DAY: {
        gview->setBackgroundColor(qRgb(255,255,255)); //TODO: Make day vision color settings.
        gview->setCrossHairColor(qRgb(0,0,0));        //TODO: Make day vision color settings.
        gview->setGridColor(qRgb(0,0,0));             //TODO: Make day vision color settings.
        break;
    }
    case ACTION_NIGHT: {
        gview->setBackgroundColor(qRgb(0,0,0));      //TODO: Make night vision color settings.
        gview->setCrossHairColor(qRgb(255,255,255)); //TODO: Make night vision color settings.
        gview->setGridColor(qRgb(255,255,255));      //TODO: Make night vision color settings.
        break;
    }

    case ACTION_PRINT: {
        /* TODO: print action */
        break;
    }

    case ACTION_WHATS_THIS: {
        debug_message("whatsThisContextHelp()");
        QWhatsThis::enterWhatsThisMode();
        break;
    }

    case ACTION_MAKE_LAYER_CURRENT:
        /* makeLayerActive(); */
        break;
    case ACTION_LAYERS:
        /* layerManager(); */
        break;
    case ACTION_LAYER_SELECTOR:
        /* TODO: layer_selector */
        break;
    case ACTION_LAYER_PREVIOUS:
        /* TODO: layer_previous */
        break;
    case ACTION_LINE_TYPE_SELECTOR:
        /* TODO: line_type_selector */
        break;
    case ACTION_LINE_WEIGHT_SELECTOR:
        /* TODO: line_weight_selector */
        break;
    case ACTION_HIDE_ALL_LAYERS:
        /* TODO: hide_all_layers */
        break;
    case ACTION_SHOW_ALL_LAYERS:
        /* TODO: show_all_layers */
        break;
    case ACTION_FREEZE_ALL_LAYERS:
        /* TODO: freeze_all_layers */
        break;
    case ACTION_THAW_ALL_LAYERS:
        /* TODO: thaw_all_layers */
        break;
    case ACTION_LOCK_ALL_LAYERS:
        /* TODO: lock_all_layers */
        break;
    case ACTION_UNLOCK_ALL_LAYERS:
        /* TODO: unlock_all_layers */
        break;

    case ACTION_GET: {
        value = get_command(context);
        break;
    }

    case ACTION_SET: {
        set_command(context);
        break;
    }

    case ACTION_CIRCLE: {
        nativeAddCircle(REAL(0), REAL(1), REAL(2), BOOL(4), OBJ_RUBBER_OFF);
        break;
    }

    case ACTION_DISTANCE:
        break;

    case ACTION_DOLPHIN:
        break;

    case ACTION_ELLIPSE: {
        nativeAddEllipse(REAL(0), REAL(1), REAL(2), REAL(3), REAL(4), BOOL(5), OBJ_RUBBER_OFF);
        break;
    }

    case ACTION_ERASE: {
        if (num_selected() <= 0) {
            /* TODO: Prompt to select objects if nothing is preselected. */
            prompt->alert(
            translate("Preselect objects before invoking the delete command."));
            messageBox("information", translate("Delete Preselect"),
                translate("Preselect objects before invoking the delete command."));
        }
        else {
            gview->deleteSelected();
        }
        break;
    }

    case ACTION_ERROR: {
        std::string s = "ERROR: (";
        s += STR(0);
        s += ") ";
        s += STR(1);
        prompt->setPrefix(QString(s.c_str()));
        prompt->appendHistory("");
        break;
    }

    case ACTION_HEART:
        break;

    case ACTION_LINE: {
        nativeAddLine(REAL(0), REAL(1), REAL(2), REAL(3), REAL(4), OBJ_RUBBER_OFF);
        break;
    }

    case ACTION_LOCATE_POINT:
        break;

    case ACTION_MOVE:
        move_command(context);
        break;

    case ACTION_MOVE_SELECTED: {
        gview->moveSelected(REAL(0), -REAL(1));
        break;
    }

    case ACTION_PATH:
        break;
    case ACTION_POINT:
        break;
    case ACTION_POLYGON:
        break;
    case ACTION_POLYLINE:
        break;

    case ACTION_PREVIEW_OFF: {
        gview->previewOff();
        break;
    }

    case ACTION_PREVIEW_ON: {
        value = previewon_command(context);
        break;
    }

    case ACTION_QUICKLEADER:
        break;

    case ACTION_RECTANGLE: {
        nativeAddRectangle(REAL(0), REAL(1), REAL(2), REAL(3), REAL(4), BOOL(5), OBJ_RUBBER_OFF);
        break;
    }

    case ACTION_RGB:
        break;

    case ACTION_ROTATE: {
        gview->rotateSelected(REAL(0), -REAL(1), -REAL(2));
        break;
    }

    case ACTION_SANDBOX: {
        value = sandbox_command(context);
        break;
    }

    case ACTION_SCALE: {
        break;
    }

    case ACTION_SELECT_ALL: {
        gview->selectAll();
        break;
    }

    case ACTION_SINGLE_LINE_TEXT:
        break;

    case ACTION_SNOWFLAKE:
        break;

    case ACTION_STAR:
        break;

    case ACTION_SYSWINDOWS:
        break;

    case ACTION_ADD:
        break;

    /* ACTION_DELETE_SELECTED? */
    case ACTION_DELETE: {
        gview->deleteSelected();
        break;
    }

    case ACTION_GRIP_EDIT:
        break;

    case ACTION_NAV:
        break;

    case ACTION_MIRROR:
        break;

    case ACTION_TEST:
        break;

    case ACTION_PAN_REAL_TIME: {
        gview->panRealTime();
        break;
    }
    case ACTION_PAN_POINT: {
        gview->panPoint();
        break;
    }
    case ACTION_PAN_LEFT: {
        QUndoStack* stack = gview->getUndoStack();
        if (stack) {
            UndoableCommand* cmd = new UndoableCommand(ACTION_NAV, "PanLeft", gview, 0);
            stack->push(cmd);
        }
        break;
    }
    case ACTION_PAN_RIGHT: {
        QUndoStack* stack = gview->getUndoStack();
        if (stack) {
            UndoableCommand* cmd = new UndoableCommand(ACTION_NAV, "PanRight", gview, 0);
            stack->push(cmd);
        }
        break;
    }
    case ACTION_PAN_UP: {
        QUndoStack* stack = gview->getUndoStack();
        if (stack) {
            UndoableCommand* cmd = new UndoableCommand(ACTION_NAV, "PanUp", gview, 0);
            stack->push(cmd);
        }
        break;
    }
    case ACTION_PAN_DOWN: {
        QUndoStack* stack = gview->getUndoStack();
        if (stack) {
            UndoableCommand* cmd = new UndoableCommand(ACTION_NAV, "PanDown", gview, 0);
            stack->push(cmd);
        }
        break;
    }
    case ACTION_WINDOW_CLOSE: {
        onCloseWindow();
        break;
    }
    case ACTION_WINDOW_CLOSE_ALL: {
        mdiArea->closeAllSubWindows();
        break;
    }
    case ACTION_WINDOW_CASCADE: {
        mdiArea->cascade();
        break;
    }
    case ACTION_WINDOW_TILE: {
        mdiArea->tile();
        break;
    }
    case ACTION_WINDOW_NEXT: {
        mdiArea->activateNextSubWindow();
        break;
    }
    case ACTION_WINDOW_PREVIOUS: {
        mdiArea->activatePreviousSubWindow();
        break;
    }
    case ACTION_ZOOM_ALL: {
        stub_implement("Implement zoomAll.");
        break;
    }
    case ACTION_ZOOM_CENTER: {
        stub_implement("Implement zoomCenter.");
        break;
    }
    case ACTION_ZOOM_DYNAMIC: {
        stub_implement("Implement zoomDynamic.");
        break;
    }
    case ACTION_ZOOM_EXTENTS: {
        QUndoStack* stack = gview->getUndoStack();
        if (stack) {
            UndoableCommand* cmd = new UndoableCommand(ACTION_NAV, "ZoomExtents", gview, 0);
            stack->push(cmd);
        }
        break;
    }
    case ACTION_ZOOM_IN: {
        gview->zoomIn();
        break;
    }
    case ACTION_ZOOM_OUT: {
        gview->zoomOut();
        break;
    }
    case ACTION_ZOOM_PREVIOUS: {
        stub_implement("Implement zoomPrevious.");
        break;
    }
    case ACTION_ZOOM_REAL_TIME: {
        stub_implement("Implement zoomRealtime.");
        break;
    }
    case ACTION_ZOOM_SCALE: {
        stub_implement("Implement zoomScale.");
        break;
    }
    case ACTION_ZOOM_SELECTED: {
        QUndoStack* stack = gview->getUndoStack();
        if (stack) {
            UndoableCommand* cmd = new UndoableCommand(ACTION_NAV, "ZoomSelected", gview, 0);
            stack->push(cmd);
        }
        break;
    }
    case ACTION_ZOOM_WINDOW: {
        gview->zoomWindow();
        break;
    }
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
        clear_selection();
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
nativeSetRubberPoint(char key[MAX_STRING_LENGTH], double x, double y)
{
    View* gview = activeView();
    if (gview) {
        gview->setRubberPoint(key, QPointF(x, -y));
    }
}

/* . */
void
nativeSetRubberText(char key[MAX_STRING_LENGTH], char txt[MAX_STRING_LENGTH])
{
    View* gview = activeView();
    if (gview) {
        gview->setRubberText(key, txt);
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
            UndoableCommand* cmd = new UndoableCommand(ACTION_ADD, obj->data.OBJ_NAME, obj, gview, 0);
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
            UndoableCommand* cmd = new UndoableCommand(ACTION_ADD, obj->data.OBJ_NAME, obj, gview, 0);
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
        UndoableCommand* cmd = new UndoableCommand(ACTION_ADD, obj->data.OBJ_NAME, obj, gview, 0);
        stack->push(cmd);
    }
}

/* . */
void
nativeAddRoundedRectangle(double x, double y, double w, double h, double rad, double rot, bool fill)
{
}

void
nativeAddArc(double x1, double y1, double x2, double y2, double x3, double y3, int rubberMode)
{
    View* gview = activeView();
    QGraphicsScene* scene = activeScene();
    if (gview && scene) {
        EmbArc arc;
        arc.start.x = x1;
        arc.start.y = -y1;
        arc.mid.y = x2;
        arc.mid.y = -y2;
        arc.end.y = x3;
        arc.end.y = -y3;
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
            UndoableCommand* cmd = new UndoableCommand(ACTION_ADD, obj->data.OBJ_NAME, obj, gview, 0);
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
            UndoableCommand* cmd = new UndoableCommand(ACTION_ADD, obj->data.OBJ_NAME, obj, gview, 0);
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
        UndoableCommand* cmd = new UndoableCommand(ACTION_ADD, obj->data.OBJ_NAME, obj, gview, 0);
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
            UndoableCommand* cmd = new UndoableCommand(ACTION_ADD, obj->data.OBJ_NAME, obj, gview, 0);
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
            UndoableCommand* cmd = new UndoableCommand(ACTION_ADD, obj->data.OBJ_NAME, obj, gview, 0);
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
            UndoableCommand* cmd = new UndoableCommand(ACTION_ADD, obj->data.OBJ_NAME, obj, gview, 0);
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

void
nativeAddToSelection(const QPainterPath path, Qt::ItemSelectionMode mode)
{
}

void
nativeCutSelected(double x, double y)
{
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

/* Compatibility layer for C files */
void
init_command(void)
{
    View* gview = activeView();
    if (gview) {
        gview->clearRubberRoom();
    }
}

void
clear_selection(void)
{
    View* gview = activeView();
    if (gview) {
        gview->clearSelection();
    }
}

int
num_selected(void)
{
    View* gview = activeView();
    if (gview) {
        return gview->numSelected();
    }
    return 0;
}

void
prompt_output(const char *txt)
{
    prompt->appendHistory(QString(txt));
}

void
cut(void)
{
    View* gview = activeView();
    if (gview) {
        gview->cut();
    }
}

void
end_command(void)
{
    View* gview = activeView();
    if (gview) {
        gview->clearRubberRoom();
        gview->previewOff();
        gview->disableMoveRapidFire();
    }
    prompt->endCommand();
}

void
debug_message(const char *msg)
{
    _main->debug_message(msg);
}

/* Simple Commands (other commands, like circle_command are housed in their
 * own file with their associated functions)
 * ------------------------------------------------------------------------
 */

/* TODO: QTabWidget for about dialog
 */
void
about_dialog(void)
{
    QApplication::setOverrideCursor(Qt::ArrowCursor);
    qDebug("about()");

    QString appDir = qApp->applicationDirPath();
    QString appName = QApplication::applicationName();
    QString title = "About " + appName;

    QDialog dialog(_main);
    QLabel image_label;
    QPixmap img(appDir + "/images/logo-small.png");
    image_label.setPixmap(img);
    QLabel text(appName + "\n\n" +
        _main->tr("http://www.libembroidery.org") +
        "\n\n" +
        _main->tr("Available Platforms: GNU/Linux, Windows, Mac OSX, Raspberry Pi") +
        "\n\n" +
        _main->tr("Embroidery formats by Josh Varga.") +
        "\n" +
        _main->tr("User Interface by Jonathan Greig.") +
        "\n\n" +
        _main->tr("Free under the zlib/libpng license.")
        #if defined(BUILD_GIT_HASH)
        + "\n\n" +
        _main->tr("Build Hash: ") + qPrintable(BUILD_GIT_HASH)
        #endif
        );
    text.setWordWrap(true);

    QDialogButtonBox buttonbox(Qt::Horizontal, &dialog);
    QPushButton button(&dialog);
    button.setText("Oh, Yeah!");
    buttonbox.addButton(&button, QDialogButtonBox::AcceptRole);
    buttonbox.setCenterButtons(true);
    _main->connect(&buttonbox, SIGNAL(accepted()), &dialog, SLOT(accept()));

    QVBoxLayout layout;
    layout.setAlignment(Qt::AlignCenter);
    layout.addWidget(&image_label);
    layout.addWidget(&text);
    layout.addWidget(&buttonbox);

    dialog.setWindowTitle(title);
    dialog.setMinimumWidth(image_label.minimumWidth()+30);
    dialog.setMinimumHeight(image_label.minimumHeight()+50);
    dialog.setLayout(&layout);
    dialog.exec();

    QApplication::restoreOverrideCursor();
}

/* GET is a prompt-only Command. */
ScriptValue
get_command(ScriptEnv* context)
{
    QString value(STR(0));

    if (value == "MOUSEX") {
        QGraphicsScene* scene = activeScene();
        if (!scene) {
            return script_false;
        }
        ScriptValue r = script_real(scene->property("SCENE_MOUSE_POINT").toPointF().x());
        /* _main->qDebug("mouseY: %.50f", r.r); */
        return r;
    }
    else if (value == "MOUSEY") {
        QGraphicsScene* scene = activeScene();
        if (!scene) {
            return script_false;
        }
        ScriptValue r = script_real(-scene->property("SCENE_MOUSE_POINT").toPointF().y());
        //_main->qDebug("mouseY: %.50f", r.r);
        return r;
    }
    else if (value == "TEXTANGLE") {
        return script_real(text_angle.setting);
    }
    else if (value == "TEXTBOLD") {
        return script_bool(text_style_bold.setting);
    }
    else if (value == "TEXTITALIC") {
        return script_bool(text_style_italic.setting);
    }
    else if (value == "TEXTFONT") {
        return script_string(text_font.setting);
    }
    else if (value == "TEXTOVERLINE") {
        return script_real(text_style_overline.setting);
    }
    else if (value == "TEXTSIZE") {
        return script_real(text_size.setting);
    }
    else if (value == "TEXTSTRIKEOUT") {
        return script_real(text_style_strikeout.setting);
    }
    else if (value == "TEXTUNDERLINE") {
        return script_bool(text_style_underline.setting);
    }
    else if (value == "QSNAPX") {
        return script_bool(nativeQSnapX());
    }
    else if (value == "QSNAPY") {
        return script_bool(nativeQSnapY());
    }

    return script_null;
}

/* PREVIEWON . */
ScriptValue
previewon_command(ScriptEnv *context)
{
    QString cloneStr = QString(STR(0)).toUpper();
    int clone = PREVIEW_CLONE_NULL;
    if (cloneStr == "SELECTED") {
        clone = PREVIEW_CLONE_SELECTED;
    }
    else if (cloneStr == "RUBBER") {
        clone = PREVIEW_CLONE_RUBBER;
    }
    else {
        prompt_output("UNKNOWN_ERROR previewOn(): first argument must be \"SELECTED\" or \"RUBBER\".");
        return script_false;
    }

    QString modeStr  = QString(STR(1)).toUpper();
    int mode = PREVIEW_MODE_NULL;
    if (modeStr == "MOVE") {
        mode = PREVIEW_MODE_MOVE;
    }
    else if (modeStr == "ROTATE") {
        mode = PREVIEW_MODE_ROTATE;
    }
    else if (modeStr == "SCALE") {
        mode = PREVIEW_MODE_SCALE;
    }
    else {
        prompt_output("UNKNOWN_ERROR previewOn(): second argument must be \"MOVE\", \"ROTATE\" or \"SCALE\".");
        return script_false;
    }

    View* gview = activeView();
    if (gview) {
        gview->previewOn(clone, mode, REAL(2), -REAL(3), REAL(4));
    }
    else {
        prompt_output("Preview on requires an active view.");
    }
    return script_null;
}

/* SET is a prompt-only Command.
 *
 * We can't use the argument_checks function because the 2nd argument is a wildcard.
 * we need an override for the argument_checks call
 */
ScriptValue
set_command(ScriptEnv* context)
{
    if (context->argumentCount != 2) {
        return script_false;
    }

    QString value(STR(0));

    if (value == "TEXTANGLE") {
        if (context->argument[1].type != SCRIPT_REAL) {
            return script_false;
        }
        text_angle.setting = REAL(1);
    }
    else if (value == "TEXTBOLD") {
        if (context->argument[1].type != SCRIPT_BOOL) {
            return script_false;
        }
        text_style_bold.setting = BOOL(1);
    }
    else if (value == "TEXTITALIC") {
        if (context->argument[1].type != SCRIPT_BOOL) {
            return script_false;
        }
        text_style_italic.setting = BOOL(1);
    }
    else if (value == "TEXTFONT") {
        if (context->argument[1].type != SCRIPT_STRING) {
            return script_false;
        }
        strcpy(text_font.setting, STR(1));
    }
    else if (value == "TEXTOVERLINE") {
        if (context->argument[1].type != SCRIPT_BOOL) {
            return script_false;
        }
        text_style_overline.setting = BOOL(1);
    }
    else if (value == "TEXTSIZE") {
        if (context->argument[1].type != SCRIPT_REAL) {
            return script_false;
        }
        text_size.setting = REAL(1);
    }
    else if (value == "TEXTSTRIKEOUT") {
        if (context->argument[1].type != SCRIPT_BOOL) {
            return script_false;
        }
        text_style_strikeout.setting = BOOL(1);
    }
    else if (value == "TEXTUNDERLINE") {
        if (context->argument[1].type != SCRIPT_BOOL) {
            return script_false;
        }
        text_style_underline.setting = BOOL(1);
    }

    return script_null;
}

/* SYSWINDOWS */
ScriptValue
syswindows_command(ScriptEnv * context)
{
    prompt_output(translate("Enter an option [Cascade/Tile]: "));

    // Do nothing for click, context
    
    #if 0
    if (str == "C" || str == "CASCADE") {
        //TODO: Probably should add additional translate calls here.
        _main->windowCascade();
        end_command();
    }
    else if (str == "T" || str == "TILE") {
        //TODO: Probably should add additional translate calls here.
        _main->windowTile();
        end_command();
    }
    else {
        alert(translate("Invalid option keyword."));
        prompt_output(translate("Enter an option [Cascade/Tile]: "));
    }
    #endif
    return script_null;
}

/* --------------------------------------------------------------------------
 */

ScriptValue
blink_prompt_command(ScriptEnv* context)
{
    prompt->startBlinking();
    return script_null;
}

ScriptValue
set_prompt_prefix_command(ScriptEnv* context)
{
    prompt->setPrefix(QString(STR(0)));
    return script_null;
}

ScriptValue
append_prompt_history(ScriptEnv* context)
{
    int args = context->argumentCount;
    if (args == 0) {
        prompt_output("");
    }
    else if (args == 1) {
        prompt_output(STR(0));
    }
    else {
        prompt_output("appendPromptHistory() requires one or zero arguments");
        return script_false;
    }
    return script_null;
}

/* . */
ScriptValue
messagebox(ScriptEnv* context)
{
    std::string type(STR(0));
    std::string title(STR(1));
    std::string text(STR(2));

    if (type != "critical" && type != "information" && type != "question" && type != "warning") {
        prompt_output("UNKNOWN_ERROR messageBox(): first argument must be \"critical\", \"information\", \"question\" or \"warning\".");
        return script_false;
    }

    messageBox(type, title, text);
    return script_null;
}

/* . */
ScriptValue
is_int_command(ScriptEnv* context)
{
    return script_true;
}

ScriptValue
print_area_command(ScriptEnv* context)
{
    qDebug("nativePrintArea(%.2f, %.2f, %.2f, %.2f)", REAL(0), REAL(1), REAL(2), REAL(3));
    /* TODO: Print Setup Stuff
        nativePrintArea(REAL(0), REAL(1), REAL(2), REAL(3));
    */
    _main->print();
    return script_null;
}

ScriptValue
set_background_color_command(ScriptEnv* context)
{
    if (REAL(0) < 0 || REAL(0) > 255) {
        prompt_output("UNKNOWN_ERROR setBackgroundColor(): r value must be in range 0-255");
        return script_false;
    }
    if (REAL(1) < 0 || REAL(1) > 255) {
        prompt_output("UNKNOWN_ERROR setBackgroundColor(): g value must be in range 0-255");
        return script_false;
    }
    if (REAL(2) < 0 || REAL(2) > 255) {
        prompt_output("UNKNOWN_ERROR setBackgroundColor(): b value must be in range 0-255");
        return script_false;
    }

    nativeSetBackgroundColor(REAL(0), REAL(1), REAL(2));
    return script_null;
}

ScriptValue
set_crosshair_color_command(ScriptEnv* context)
{
    if (REAL(0) < 0 || REAL(0) > 255) {
        prompt_output("UNKNOWN_ERROR setCrossHairColor(): r value must be in range 0-255");
        return script_false;
    }
    if (REAL(1) < 0 || REAL(1) > 255) {
        prompt_output("UNKNOWN_ERROR setCrossHairColor(): g value must be in range 0-255");
        return script_false;
    }
    if (REAL(2) < 0 || REAL(2) > 255) {
        prompt_output("UNKNOWN_ERROR setCrossHairColor(): b value must be in range 0-255");
        return script_false;
    }

    nativeSetCrossHairColor(REAL(0), REAL(1), REAL(2));
    return script_null;
}

ScriptValue
set_grid_color_command(ScriptEnv* context)
{
    if (REAL(0) < 0 || REAL(0) > 255) {
        prompt_output("UNKNOWN_ERROR setGridColor(): r value must be in range 0-255");
        return script_false;
    }
    if (REAL(1) < 0 || REAL(1) > 255) {
        prompt_output("UNKNOWN_ERROR setGridColor(): g value must be in range 0-255");
        return script_false;
    }
    if (REAL(2) < 0 || REAL(2) > 255) {
        prompt_output("UNKNOWN_ERROR setGridColor(): b value must be in range 0-255");
        return script_false;
    }

    nativeSetGridColor(REAL(0), REAL(1), REAL(2));
    return script_null;
}

ScriptValue
add_text_multi_command(ScriptEnv* context)
{
    nativeAddTextMulti(std::string(STR(0)), REAL(1), REAL(2), REAL(3),
        BOOL(4), OBJ_RUBBER_OFF);
    return script_null;
}

ScriptValue
add_text_single_command(ScriptEnv* context)
{
    nativeAddTextSingle(std::string(STR(0)), REAL(1), REAL(2), REAL(3),
        BOOL(4), OBJ_RUBBER_OFF);
    return script_null;
}

ScriptValue
add_infinite_line(ScriptEnv* context)
{
    /* TODO: parameter error checking */
    prompt_output("TODO: finish addInfiniteLine command");
    return script_null;
}

ScriptValue
add_ray_command(ScriptEnv* context)
{
    /* TODO: parameter error checking */
    prompt_output("TODO: finish addRay command");
    return script_null;
}

ScriptValue
add_triangle_command(ScriptEnv* context)
{
    nativeAddTriangle(REAL(0), REAL(1), REAL(2), REAL(3), REAL(4), REAL(5), REAL(6), BOOL(7));
    return script_null;
}

ScriptValue
add_rounded_rectangle_command(ScriptEnv* context)
{
    nativeAddRoundedRectangle(REAL(0), REAL(1), REAL(2), REAL(3), REAL(4), REAL(5), BOOL(6));
    return script_null;
}

ScriptValue
add_arc_command(ScriptEnv* context)
{
    nativeAddArc(REAL(0), REAL(1), REAL(2), REAL(3), REAL(4), REAL(5), OBJ_RUBBER_OFF);
    return script_null;
}

ScriptValue
add_slot_command(ScriptEnv* context)
{
    nativeAddSlot(REAL(0), REAL(1), REAL(2), REAL(3), REAL(4), BOOL(5), OBJ_RUBBER_OFF);
    return script_null;
}

ScriptValue
add_point_command(ScriptEnv* context)
{
    nativeAddPoint(REAL(0), REAL(1));
    return script_null;
}

ScriptValue
add_regular_polygon_command(ScriptEnv* context)
{
    /* TODO: parameter error checking */
    prompt_output("TODO: finish addRegularPolygon command");
    return script_null;
}

ScriptValue
add_polygon_command(ScriptEnv* context)
{
    #if 0
    if (context->argumentCount != 1) {
        prompt_output("addPolygon() requires one argument");
        return script_false;
    }
    if (!context->argument[0].isArray()) {
        prompt_output("TYPE_ERROR, addPolygon(): first argument is not an array");
        return script_false;
    }

    QVariantList varList = context->argument[0].toVariant().toList();
    int varSize = varList.size();
    if (varSize < 2) {
        prompt_output("TYPE_ERROR, addPolygon(): array must contain at least two elements");
        return script_false;
    }
    if (varSize % 2) {
        prompt_output("TYPE_ERROR, addPolygon(): array cannot contain an odd number of elements");
        return script_false;
    }

    bool lineTo = false;
    bool xCoord = true;
    double x = 0;
    double y = 0;
    double startX = 0;
    double startY = 0;
    QPainterPath path;
    foreach (QVariant var, varList) {
        if (var.canConvert(QVariant::Double)) {
            if (xCoord) {
                xCoord = false;
                x = var.toReal();
            }
            else {
                xCoord = true;
                y = -var.toReal();

                if (lineTo) {
                    path.lineTo(x,y);
                }
                else {
                    path.moveTo(x,y);
                    lineTo = true;
                    startX = x;
                    startY = y;
                }
            }
        }
        else {
            prompt_output("TYPE_ERROR, addPolygon(): array contains one or more invalid elements");
            return script_false;
        }
    }

    //Close the polygon
    path.closeSubpath();

    path.translate(-startX, -startY);

    nativeAddPolygon(startX, startY, path, OBJ_RUBBER_OFF);
    #endif
    return script_null;
}

ScriptValue
add_polyline_command(ScriptEnv* context)
{
    #if 0
    if (context->argumentCount != 1) {
        prompt_output("addPolyline() requires one argument");
        return script_false;
    }
    if (!context->argument[0].isArray()) {
        prompt_output("TYPE_ERROR, addPolyline(): first argument is not an array");
        return script_false;
    }

    QVariantList varList = context->argument[0].toVariant().toList();
    int varSize = varList.size();
    if (varSize < 2) {
        prompt_output("TYPE_ERROR, addPolyline(): array must contain at least two elements");
        return script_false;
    }
    if (varSize % 2) {
        prompt_output("TYPE_ERROR, addPolyline(): array cannot contain an odd number of elements");
        return script_false;
    }

    bool lineTo = false;
    bool xCoord = true;
    double x = 0;
    double y = 0;
    double startX = 0;
    double startY = 0;
    QPainterPath path;
    foreach(QVariant var, varList) {
        if (var.canConvert(QVariant::Double)) {
            if (xCoord) {
                xCoord = false;
                x = var.toReal();
            }
            else {
                xCoord = true;
                y = -var.toReal();

                if (lineTo) {
                    path.lineTo(x,y);
                }
                else {
                    path.moveTo(x,y);
                    lineTo = true;
                    startX = x;
                    startY = y;
                }
            }
        }
        else {
            prompt_output("TYPE_ERROR, addPolyline(): array contains one or more invalid elements");
            return script_false;
        }
    }

    path.translate(-startX, -startY);

    nativeAddPolyline(startX, startY, path, OBJ_RUBBER_OFF);
    #endif
    return script_null;
}

ScriptValue
set_cursor_shape_command(ScriptEnv* context)
{
    nativeSetCursorShape(STR(0));
    return script_null;
}

ScriptValue
calculate_distance_command(ScriptEnv* context)
{
    double r = QLineF(REAL(0), REAL(1), REAL(2), REAL(3)).length();
    return script_real(r);
}

ScriptValue
perpendicular_distance_command(ScriptEnv* context)
{
    QLineF line(REAL(0), REAL(1), REAL(2), REAL(3));
    QLineF norm = line.normalVector();
    double dx = REAL(4) - REAL(0);
    double dy = REAL(5) - REAL(1);
    norm.translate(dx, dy);
    QPointF iPoint;
    norm.intersects(line, &iPoint);
    double r = QLineF(REAL(4), REAL(5), iPoint.x(), iPoint.y()).length();
    return script_real(r);
}

ScriptValue
num_selected_command(ScriptEnv* context)
{
    return script_int(num_selected());
}

/* TODO: finish
 */
ScriptValue
add_to_selection_command(ScriptEnv* context)
{
    return script_null;
}

ScriptValue
scale_selected_command(ScriptEnv* context)
{
    if (REAL(2) <= 0.0) {
        prompt_output("UNKNOWN_ERROR scaleSelected(): scale factor must be greater than zero");
        return script_false;
    }

    nativeScaleSelected(REAL(0), REAL(1), REAL(2));
    return script_null;
}

/*
 * Undo
 */
UndoableCommand::UndoableCommand(int type_, const QString& text, Object* obj, View* v, QUndoCommand* parent) : QUndoCommand(parent)
{
    data.type = type_;
    data.gview = v;
    data.object = obj;
    setText(text);
}

/* Move */
UndoableCommand::UndoableCommand(int type_, double deltaX, double deltaY, const QString& text, Object* obj, View* v, QUndoCommand* parent) : QUndoCommand(parent)
{
    data.type = type_;
    data.gview = v;
    data.object = obj;
    setText(text);
    data.dx = deltaX;
    data.dy = deltaY;
}

/* Rotate or scale */
UndoableCommand::UndoableCommand(int type_, double x, double y, double scaleFactor, const QString& text, Object* obj, View* v, QUndoCommand* parent) : QUndoCommand(parent)
{
    data.type = type_;
    data.gview = v;
    data.object = obj;
    setText(text);
    if (data.type == ACTION_SCALE) {
        //Prevent division by zero and other wacky behavior
        if (scaleFactor <= 0.0) {
            data.dx = 0.0;
            data.dy = 0.0;
            data.factor = 1.0;
            QMessageBox::critical(0,
                QObject::tr("ScaleFactor Error"),
                QObject::tr("Hi there. If you are not a developer, report this as a bug. "
               "If you are a developer, your code needs examined, and possibly your head too."));
        }
        else {
            //Calculate the offset
            double oldX = data.object->x();
            double oldY = data.object->y();
            QLineF scaleLine(x, y, oldX, oldY);
            scaleLine.setLength(scaleLine.length()*scaleFactor);
            double newX = scaleLine.x2();
            double newY = scaleLine.y2();

            data.dx = newX - oldX;
            data.dy = newY - oldY;
            data.factor = scaleFactor;
        }
    }
    else {
        data.pivotX = x;
        data.pivotY = y;
        data.angle = scaleFactor;
    }
}

/* Navigation */
UndoableCommand::UndoableCommand(int type_, const QString& type_name, View* v, QUndoCommand* parent) : QUndoCommand(parent)
{
    data.type = type_;
    data.gview = v;
    data.navType = type_name;
    setText(QObject::tr("Navigation"));
    data.done = false;
    data.fromTransform = data.gview->transform();
    data.fromCenter = data.gview->center();
}

/* Grip Edit */
UndoableCommand::UndoableCommand(int type_, const QPointF beforePoint, const QPointF afterPoint, const QString& text, Object* obj, View* v, QUndoCommand* parent) : QUndoCommand(parent)
{
    data.type = type_;
    data.gview = v;
    data.object = obj;
    setText(text);
    data.before = beforePoint;
    data.after = afterPoint;
}

/* Mirror */
UndoableCommand::UndoableCommand(int type_, double x1, double y1, double x2, double y2, const QString& text, Object* obj, View* v, QUndoCommand* parent) : QUndoCommand(parent)
{
    data.gview = v;
    data.object = obj;
    setText(text);
    data.mirrorLine = QLineF(x1, y1, x2, y2);
}

/* . */
void
UndoableCommand::undo()
{
    switch (data.type) {
    case ACTION_ADD:
        data.gview->deleteObject(data.object);
        break;
    case ACTION_DELETE:
        data.gview->addObject(data.object);
        break;
    case ACTION_MOVE:
        data.object->moveBy(-data.dx, -data.dy);
        break;
    case ACTION_ROTATE:
        rotate(data.pivotX, data.pivotY, -data.angle);
        break;
    case ACTION_GRIP_EDIT:
        data.object->gripEdit(data.after, data.before);
        break;
    case ACTION_SCALE:
        data.object->setScale(data.object->scale()*(1/data.factor));
        data.object->moveBy(-data.dx, -data.dy);
        break;
    case ACTION_NAV: {
        if (!data.done) {
            data.toTransform = data.gview->transform();
            data.toCenter = data.gview->center();
            data.done = true;
        }

        data.gview->setTransform(data.fromTransform);
        data.gview->centerAt(data.fromCenter);
        break;
    }
    case ACTION_MIRROR:
        mirror();
        break;
    default:
        break;
    }
}

/* . */
void
UndoableCommand::redo()
{
    switch (data.type) {
    case ACTION_ADD:
        data.gview->addObject(data.object);
        break;
    case ACTION_DELETE:
        data.gview->deleteObject(data.object);
        break;
    case ACTION_MOVE:
        data.object->moveBy(data.dx, data.dy);
        break;
    case ACTION_ROTATE:
        rotate(data.pivotX, data.pivotY, data.angle);
        break;
    case ACTION_GRIP_EDIT:
        data.object->gripEdit(data.before, data.after);
        break;
    case ACTION_SCALE:
        data.object->setScale(data.object->scale()*data.factor);
        data.object->moveBy(data.dx, data.dy);
        break;
    case ACTION_NAV: {
        if (data.done) {
            data.gview->setTransform(data.toTransform);
            data.gview->centerAt(data.toCenter);
            break;
        }
        if (data.navType == "ZoomInToPoint") {
            data.gview->zoomToPoint(data.gview->scene()->property("VIEW_MOUSE_POINT").toPoint(), +1);
        }
        else if (data.navType == "ZoomOutToPoint") {
            data.gview->zoomToPoint(data.gview->scene()->property("VIEW_MOUSE_POINT").toPoint(), -1);
        }
        else if (data.navType == "ZoomExtents") {
            data.gview->zoomExtents();
        }
        else if (data.navType == "ZoomSelected") {
            data.gview->zoomSelected();
        }
        else if (data.navType == "PanStart") {
            /* Do Nothing. We are just recording the spot where the pan started. */
        }
        else if (data.navType == "PanStop") {
            /* Do Nothing. We are just recording the spot where the pan stopped. */
        }
        else if (data.navType == "PanLeft") {
            data.gview->panLeft();
        }
        else if (data.navType == "PanRight") {
            data.gview->panRight();
        }
        else if (data.navType == "PanUp") {
            data.gview->panUp();
        }
        else if (data.navType == "PanDown") {
            data.gview->panDown();
        }
        data.toTransform = data.gview->transform();
        data.toCenter = data.gview->center();
        break;
    }
    case ACTION_MIRROR:
        mirror();
        break;
    default:
        break;
    }
}

/* Rotate */
void
UndoableCommand::rotate(double x, double y, double rot)
{
    double rad = radians(rot);
    double cosRot = cos(rad);
    double sinRot = sin(rad);
    double px = data.object->scenePos().x();
    double py = data.object->scenePos().y();
    px -= x;
    py -= y;
    double rotX = px*cosRot - py*sinRot;
    double rotY = px*sinRot + py*cosRot;
    rotX += x;
    rotY += y;

    data.object->setPos(rotX, rotY);
    data.object->setRotation(data.object->rotation() + rot);
}

/* . */
bool
UndoableCommand::mergeWith(const QUndoCommand* newest)
{
    if (newest->id() != id()) {
         // make sure other is also an UndoableNavCommand
         return false;
    }

    const UndoableCommand* cmd = static_cast<const UndoableCommand*>(newest);
    data.toTransform = cmd->data.toTransform;
    data.toCenter = cmd->data.toCenter;

    return true;
}

/* . */
void
UndoableCommand::mirror()
{
    //TODO: finish undoable mirror
}

/* LOCATEPOINT */
ScriptValue
locatepoint_command(ScriptEnv *context)
{
    switch (context->context) {
    case CONTEXT_MAIN:
        init_command();
        clear_selection();
        prompt_output(translate("Specify point: "));
        break;
    case CONTEXT_CLICK: {
        char output[200];
        float x = 0.0f;
        float y = 0.0f;
        sprintf(output, "X = %f, Y = %f", x, y);
        prompt_output(output);
        end_command();
        break;
    }
    case CONTEXT_CONTEXT:
        _main->runCommandCore("todo LOCATEPOINT context()", context);
        break;
    case CONTEXT_PROMPT:
        /*
        EmbVector v;
        if (!parse_vector(str, &v)) {
            alert(translate("Invalid point."));
            prompt_output(translate("Specify point: "));
        }
        else {
            char output[200];
            sprintf(output, "X = %f, Y = %f", v.x, v.y);
            prompt_output(output);
            end_command();
        }
        */
        break;
    default:
        break;
    }
    return script_null;
}

/* Cartesian Coordinate System reported: anticlockwise angle from x-axis. */
void
reportDistance(EmbVector a, EmbVector b)
{
    char output[200];
    EmbVector delta = emb_vector_subtract(b, a);

    EmbReal distance = emb_vector_length(delta);
    EmbReal angle = emb_vector_angle(delta);

    sprintf(output, "%s = %f, %s = %f",
        translate("Distance"), distance, translate("Angle"), angle);
    prompt_output(output);
    sprintf(output, "%s = %f, %s = %f",
        translate("Delta X"), delta.x, translate("Delta Y"), delta.y);
    prompt_output(output);
}

/* DISTANCE. */
ScriptValue
distance_command(ScriptEnv *context)
{
    switch (context->context) {
    case CONTEXT_MAIN:
        init_command();
        clear_selection();
        /*
        global.point1.x = 0.0f;
        global.point1.y = 0.0f;
        global.point2.x = 0.0f;
        global.point2.y = 0.0f;
        */
        prompt_output(translate("Specify first point: "));
        break;
    case CONTEXT_CLICK:
        /*
        if (isNaN(global.x1)) {
            global.point1 = v;
            addRubber("LINE");
            setRubberMode("LINE");
            setRubberPoint("LINE_START", global.point1.x, global.point1.y);
            prompt_output(translate("Specify second point: "));
        }
        else {
            global.point2 = v;
            reportDistance();
            end_command();
        }
        */
        break;
    case CONTEXT_CONTEXT:
        /* todo("DISTANCE", "context()"); */
        break;
    case CONTEXT_PROMPT:
        /*
        var strList = str.split(",");
        if (isNaN(global.x1)) {
            if (isNaN(strList[0]) || isNaN(strList[1])) {
                alert(translate("Requires numeric distance or two points."));
                prompt_output(translate("Specify first point: "));
            }
            else {
                global.x1 = Number(strList[0]);
                global.y1 = Number(strList[1]);
                addRubber("LINE");
                setRubberMode("LINE");
                setRubberPoint("LINE_START", global.x1, global.y1);
                prompt_output(translate("Specify second point: "));
            }
        }
        else {
            if (isNaN(strList[0]) || isNaN(strList[1])) {
                alert(translate("Requires numeric distance or two points."));
                prompt_output(translate("Specify second point: "));
            }
            else {
                global.x2 = Number(strList[0]);
                global.y2 = Number(strList[1]);
                reportDistance();
                end_command();
            }
        }
        */
        break;
    default:
        break;
    }
    return script_null;
}

/* MOVE . */
ScriptValue
move_command(ScriptEnv *context)
{
    switch (context->context) {
    case CONTEXT_MAIN:
        break;
    case CONTEXT_CLICK:
        break;
    case CONTEXT_CONTEXT:
        break;
    case CONTEXT_PROMPT:
        break;
    default:
        break;
    }
    return script_null;
}

#if 0
void
main(void)
{
    global.firstRun = true;
    global.base.x = NaN;
    global.base.y = NaN;
    global.dest.x = NaN;
    global.dest.y = NaN;
    global.delta.x = NaN;
    global.delta.y = NaN;

    if (num_selected() <= 0) {
        /* TODO: Prompt to select objects if nothing is preselected. */
        alert(translate("Preselect objects before invoking the move command."));
        end_command();
        messageBox("information", translate("Move Preselect"),
            translate("Preselect objects before invoking the move command."));
    }
    else {
        prompt_output(translate("Specify base point: "));
    }
}

void
click(EmbVector v)
{
    if (global->firstRun) {
        global->firstRun = false;
        global->base = v;
        addRubber("LINE");
        setRubberMode("LINE");
        setRubberPoint("LINE_START", global->base);
        previewOn("SELECTED", "MOVE", global->base, 0);
        prompt_output(translate("Specify destination point: "));
    }
    else {
        global.dest = v;
        global.delta = emb_vector_subtract(global.dest, global.base);
        moveSelected(global.delta);
        previewOff();
        end_command();
    }
}

void context(str)
{
    todo("MOVE", "context()");
}

void prompt(str)
{
    EmbVector v;
    if (global.firstRun) {
        if (!parse_vector(str, &v)) {
            alert(translate("Invalid point."));
            prompt_output(translate("Specify base point: "));
        }
        else {
            global.firstRun = false;
            global.base = v;
            addRubber("LINE");
            setRubberMode("LINE");
            setRubberPoint("LINE_START", global.base.x, global.base.y);
            previewOn("SELECTED", "MOVE", global.base.x, global.base.y, 0);
            prompt_output(translate("Specify destination point: "));
        }
    }
    else {
        if (!parse_vector(str, &v)) {
            alert(translate("Invalid point."));
            prompt_output(translate("Specify destination point: "));
        }
        else {
            global.dest = v;
            global.delta = global.dest - global.base;
            moveSelected(global.delta.x, global.delta.y);
            previewOff();
            end_command();
        }
    }
}
#endif

/* SCALE . */
ScriptValue
scale_command(ScriptEnv * context)
{
    switch (context->context) {
    case CONTEXT_MAIN:
        break;
    case CONTEXT_CLICK:
        break;
    case CONTEXT_CONTEXT:
        break;
    case CONTEXT_PROMPT:
        break;
    default:
        break;
    }
    return script_null;
}

#if 0
void
main(void)
{
    init_command();
    global.mode = global.mode_NORMAL;
    global.firstRun = true;
    global.baseX = NaN;
    global.baseY = NaN;
    global.destX = NaN;
    global.destY = NaN;
    global.factor = NaN;

    global.baseRX = NaN;
    global.baseRY = NaN;
    global.destRX = NaN;
    global.destRY = NaN;
    global.factorRef = NaN;
    global.factorNew = NaN;

    if (num_selected() <= 0) {
        //TODO: Prompt to select objects if nothing is preselected
        alert(translate("Preselect objects before invoking the scale command."));
        end_command();
        messageBox("information", translate("Scale Preselect"), translate("Preselect objects before invoking the scale command."));
    }
    else {
        prompt_output(translate("Specify base point: "));
    }
}

void
click(EmbVector position)
{
    if (global.mode == global.mode_NORMAL) {
        if (global.firstRun) {
            global.firstRun = false;
            global.base = position;
            addRubber("LINE");
            setRubberMode("LINE");
            setRubberPoint("LINE_START", global.baseX, global.baseY);
            previewOn("SELECTED", "SCALE", global.baseX, global.baseY, 1);
            prompt_output(translate("Specify scale factor or [Reference]: "));
        }
        else {
            global.destX = x;
            global.destY = y;
            global.factor = calculateDistance(global.baseX, global.baseY, global.destX, global.destY);
            scaleSelected(global.baseX, global.baseY, global.factor);
            previewOff();
            end_command();
        }
    }
    else if (global.mode == global.mode_REFERENCE) {
        if (isNaN(global.baseRX)) {
            global.baseRX = x;
            global.baseRY = y;
            addRubber("LINE");
            setRubberMode("LINE");
            setRubberPoint("LINE_START", global.baseRX, global.baseRY);
            prompt_output(translate("Specify second point: "));
        }
        else if (isNaN(global.destRX)) {
            global.destRX = x;
            global.destRY = y;
            global.factorRef = calculateDistance(global.baseRX, global.baseRY, global.destRX, global.destRY);
            if (global.factorRef <= 0.0) {
                global.destRX    = NaN;
                global.destRY    = NaN;
                global.factorRef = NaN;
                alert(translate("Value must be positive and nonzero."));
                prompt_output(translate("Specify second point: "));
            }
            else {
                setRubberPoint("LINE_START", global.baseX, global.baseY);
                previewOn("SELECTED", "SCALE", global.baseX, global.baseY, global.factorRef);
                prompt_output(translate("Specify new length: "));
            }
        }
        else if (isNaN(global->factorNew)) {
            global->factorNew = calculateDistance(global->base, x, y);
            if (global->factorNew <= 0.0) {
                global->factorNew = NaN;
                alert(translate("Value must be positive and nonzero."));
                prompt_output(translate("Specify new length: "));
            }
            else {
                scaleSelected(global->base, global.factorNew/global.factorRef);
                previewOff();
                end_command();
            }
        }
    }
}

void context(str)
{
    todo("SCALE", "context()");
}

void prompt(str)
{
    EmbVector v;
    switch (global->mode) {
    default:
    case SCALE_MODE_NORMAL: {
        if (global->firstRun) {
            if (!parse_vector(str, &v)) {
                alert(translate("Invalid point."));
                prompt_output(translate("Specify base point: "));
            }
            else {
                global->firstRun = false;
                global->base = v;
                addRubber("LINE");
                setRubberMode("LINE");
                setRubberPoint("LINE_START", global.baseX, global.baseY);
                previewOn("SELECTED", "SCALE", global.baseX, global.baseY, 1);
                prompt_output(translate("Specify scale factor or [Reference]: "));
            }
        }
        else {
            if (str == "R" || str == "REFERENCE") {
                //TODO: Probably should add additional translate calls here.
                global.mode = global.mode_REFERENCE;
                prompt_output(translate("Specify reference length") + " {1}: ");
                clearRubber();
                previewOff();
            }
            else {
                if (isNaN(str)) {
                    alert(translate("Requires valid numeric distance, second point, or option keyword."));
                    prompt_output(translate("Specify scale factor or [Reference]: "));
                }
                else {
                    global.factor = Number(str);
                    scaleSelected(global.baseX, global.baseY, global.factor);
                    previewOff();
                    end_command();
                }
            }
        }
        break;
    }
    case SCALE_MODE_REFERENCE: {
        if (isNaN(global.baseRX)) {
            if (isNaN(str)) {
                if (!parse_vector(str, &v)) {
                    alert(translate("Requires valid numeric distance or two points."));
                    prompt_output(translate("Specify reference length") + " {1}: ");
                }
                else {
                    global.baseR = v;
                    addRubber("LINE");
                    setRubberMode("LINE");
                    setRubberPoint("LINE_START", global.baseRX, global.baseRY);
                    prompt_output(translate("Specify second point: "));
                }
            }
            else {
                //The base and dest values are only set here to advance the command.
                global.baseRX = 0.0;
                global.baseRY = 0.0;
                global.destRX = 0.0;
                global.destRY = 0.0;
                //The reference length is what we will use later.
                global.factorRef = Number(str);
                if (global.factorRef <= 0.0) {
                    global.baseRX    = NaN;
                    global.baseRY    = NaN;
                    global.destRX    = NaN;
                    global.destRY    = NaN;
                    global.factorRef = NaN;
                    alert(translate("Value must be positive and nonzero."));
                    prompt_output(translate("Specify reference length") + " {1}: ");
                }
                else {
                    addRubber("LINE");
                    setRubberMode("LINE");
                    setRubberPoint("LINE_START", global.baseX, global.baseY);
                    previewOn("SELECTED", "SCALE", global.baseX, global.baseY, global.factorRef);
                    prompt_output(translate("Specify new length: "));
                }
            }
        }
        else if (isNaN(global.destRX)) {
            if (isNaN(str)) {
                if (!parse_vector(str, &v)) {
                    alert(translate("Requires valid numeric distance or two points."));
                    prompt_output(translate("Specify second point: "));
                }
                else {
                    global.destR = v;
                    global.factorRef = calculateDistance(global.baseRX, global.baseRY, global.destRX, global.destRY);
                    if (global.factorRef <= 0.0) {
                        global.destRX    = NaN;
                        global.destRY    = NaN;
                        global.factorRef = NaN;
                        alert(translate("Value must be positive and nonzero."));
                        prompt_output(translate("Specify second point: "));
                    }
                    else {
                        setRubberPoint("LINE_START", global.baseX, global.baseY);
                        previewOn("SELECTED", "SCALE", global.baseX, global.baseY, global.factorRef);
                        prompt_output(translate("Specify new length: "));
                    }
                }
            }
            else {
                //The base and dest values are only set here to advance the command.
                global.baseRX = 0.0;
                global.baseRY = 0.0;
                global.destRX = 0.0;
                global.destRY = 0.0;
                //The reference length is what we will use later.
                global.factorRef = Number(str);
                if (global.factorRef <= 0.0) {
                    global.destRX    = NaN;
                    global.destRY    = NaN;
                    global.factorRef = NaN;
                    alert(translate("Value must be positive and nonzero."));
                    prompt_output(translate("Specify second point: "));
                }
                else {
                    setRubberPoint("LINE_START", global.baseX, global.baseY);
                    previewOn("SELECTED", "SCALE", global.baseX, global.baseY, global.factorRef);
                    prompt_output(translate("Specify new length: "));
                }
            }
        }
        else if (isNaN(global.factorNew)) {
            if (isNaN(str)) {
                if (!parse_vector(str, &v)) {
                    alert(translate("Requires valid numeric distance or second point."));
                    prompt_output(translate("Specify new length: "));
                }
                else {
                    global.factorNew = embVector(global.base, v);
                    if (global.factorNew <= 0.0) {
                        global.factorNew = NaN;
                        alert(translate("Value must be positive and nonzero."));
                        prompt_output(translate("Specify new length: "));
                    }
                    else {
                        scaleSelected(global.baseX, global.baseY, global.factorNew/global.factorRef);
                        previewOff();
                        end_command();
                    }
                }
            }
            else {
                global.factorNew = Number(str);
                if (global.factorNew <= 0.0) {
                    global.factorNew = NaN;
                    alert(translate("Value must be positive and nonzero."));
                    prompt_output(translate("Specify new length: "));
                }
                else {
                    scaleSelected(global.baseX, global.baseY, global.factorNew/global.factorRef);
                    previewOff();
                    end_command();
                }
            }
        }
        break;
    }
    }
}
#endif

/* SANDBOX . */
ScriptValue
sandbox_command(ScriptEnv * context)
{
    switch (context->context) {
    case CONTEXT_MAIN:
        /*
        //Report number of pre-selected objects
        prompt_output("Number of Objects Selected: " + numSelected().toString());
    
        mirrorSelected(0,0,0,1);
    
        //selectAll();
        //rotateSelected(0,0,90);
    
        //Polyline & Polygon Testing
    
        var offsetX = 0.0;
        var offsetY = 0.0;
    
        var polylineArray = [];
        polylineArray.push(1.0 + offsetX);
        polylineArray.push(1.0 + offsetY);
        polylineArray.push(1.0 + offsetX);
        polylineArray.push(2.0 + offsetY);
        polylineArray.push(2.0 + offsetX);
        polylineArray.push(2.0 + offsetY);
        polylineArray.push(2.0 + offsetX);
        polylineArray.push(3.0 + offsetY);
        polylineArray.push(3.0 + offsetX);
        polylineArray.push(3.0 + offsetY);
        polylineArray.push(3.0 + offsetX);
        polylineArray.push(2.0 + offsetY);
        polylineArray.push(4.0 + offsetX);
        polylineArray.push(2.0 + offsetY);
        polylineArray.push(4.0 + offsetX);
        polylineArray.push(1.0 + offsetY);
        addPolyline(polylineArray);
    
        offsetX = 5.0;
        offsetY = 0.0;
    
        var polygonArray = [];
        polygonArray.push(1.0 + offsetX);
        polygonArray.push(1.0 + offsetY);
        polygonArray.push(1.0 + offsetX);
        polygonArray.push(2.0 + offsetY);
        polygonArray.push(2.0 + offsetX);
        polygonArray.push(2.0 + offsetY);
        polygonArray.push(2.0 + offsetX);
        polygonArray.push(3.0 + offsetY);
        polygonArray.push(3.0 + offsetX);
        polygonArray.push(3.0 + offsetY);
        polygonArray.push(3.0 + offsetX);
        polygonArray.push(2.0 + offsetY);
        polygonArray.push(4.0 + offsetX);
        polygonArray.push(2.0 + offsetY);
        polygonArray.push(4.0 + offsetX);
        polygonArray.push(1.0 + offsetY);
        addPolygon(polygonArray);
        */
        break;
    case CONTEXT_CLICK:
        break;
    case CONTEXT_CONTEXT:
        break;
    case CONTEXT_PROMPT:
        break;
    default:
        break;
    }
    return script_null;
}

/* ROTATE . */
ScriptValue
rotate_command(ScriptEnv * context)
{
    return script_null;
}

#if 0
void
main()
{
    init_command();
    global.mode = global.mode_NORMAL;
    global.firstRun = true;
    global.base = zero_vector;
    global.dest = zero_vector;
    global.angle = 0.0f;

    global.baseR = zero_vector;
    global.destR = zero_vector;
    global.angleRef = 0.0f;
    global.angleNew = 0.0f;

    if (num_selected() <= 0) {
        /* TODO: Prompt to select objects if nothing is preselected. */
        alert(translate("Preselect objects before invoking the rotate command."));
        end_command();
        messageBox("information", translate("Rotate Preselect"), translate("Preselect objects before invoking the rotate command."));
    }
    else {
        prompt_output(translate("Specify base point: "));
    }
}

void
click(EmbVector v)
{
    if (global.mode == global.mode_NORMAL) {
        if (global.firstRun) {
            global.firstRun = false;
            global.base = v;
            addRubber("LINE");
            setRubberMode("LINE");
            setRubberPoint("LINE_START", global.baseX, global.baseY);
            previewOn("SELECTED", "ROTATE", global.baseX, global.baseY, 0);
            prompt_output(translate("Specify rotation angle or [Reference]: "));
        }
        else {
            global.dest = v;
            global.angle = emb_vector_angle(global.base, global.dest);
            rotateSelected(global.baseX, global.baseY, global.angle);
            previewOff();
            end_command();
        }
    }
    else if (global.mode == global.mode_REFERENCE) {
        if (isNaN(global.baseRX)) {
            global.baseR = v;
            addRubber("LINE");
            setRubberMode("LINE");
            setRubberPoint("LINE_START", global.baseRX, global.baseRY);
            prompt_output(translate("Specify second point: "));
        }
        else if (isNaN(global.destRX)) {
            global.destR = v;
            global.angleRef = calculateAngle(global.baseRX, global.baseRY, global.destRX, global.destRY);
            setRubberPoint("LINE_START", global.baseX, global.baseY);
            previewOn("SELECTED", "ROTATE", global.baseX, global.baseY, global.angleRef);
            prompt_output(translate("Specify the new angle: "));
        }
        else if (isNaN(global.angleNew)) {
            global.angleNew = calculateAngle(global.baseX, global.baseY, x, y);
            rotateSelected(global.baseX, global.baseY, global.angleNew - global.angleRef);
            previewOff();
            end_command();
        }
    }
}

void context(str)
{
    todo("ROTATE", "context()");
}

void prompt(str)
{
    EmbVector v;
    if (global.mode == global.mode_NORMAL) {
        if (global.firstRun) {
            if (!parse_vector(str, &v)) {
                alert(translate("Invalid point."));
                prompt_output(translate("Specify base point: "));
            }
            else {
                global.firstRun = false;
                global.base = v;
                addRubber("LINE");
                setRubberMode("LINE");
                setRubberPoint("LINE_START", global.base.x, global.base.y);
                previewOn("SELECTED", "ROTATE", global.base.x, global.base.y, 0);
                prompt_output(translate("Specify rotation angle or [Reference]: "));
            }
        }
        else {
            if (str == "R" || str == "REFERENCE") {
                global.mode = global.mode_REFERENCE;
                prompt_output(translate("Specify the reference angle") + " {0.00}: ");
                clearRubber();
                previewOff();
            }
            else {
                if (isNaN(str)) {
                    alert(translate("Requires valid numeric angle, second point, or option keyword."));
                    prompt_output(translate("Specify rotation angle or [Reference]: "));
                }
                else {
                    global.angle = Number(str);
                    rotateSelected(global.baseX, global.baseY, global.angle);
                    previewOff();
                    end_command();
                }
            }
        }
    }
    else if (global.mode == global.mode_REFERENCE) {
        if (isNaN(global.baseRX)) {
            if (isNaN(str)) {
                if (!parse_vector(str, &v)) {
                    alert(translate("Requires valid numeric angle or two points."));
                    prompt_output(translate("Specify the reference angle") + " {0.00}: ");
                }
                else {
                    global.baseR = v;
                    addRubber("LINE");
                    setRubberMode("LINE");
                    setRubberPoint("LINE_START", global.baseR.x, global.baseR.y);
                    prompt_output(translate("Specify second point: "));
                }
            }
            else {
                //The base and dest values are only set here to advance the command.
                global.baseRX = 0.0;
                global.baseRY = 0.0;
                global.destRX = 0.0;
                global.destRY = 0.0;
                //The reference angle is what we will use later.
                global.angleRef = Number(str);
                addRubber("LINE");
                setRubberMode("LINE");
                setRubberPoint("LINE_START", global.base.x, global.base.y);
                previewOn("SELECTED", "ROTATE", global.baseX, global.baseY, global.angleRef);
                prompt_output(translate("Specify the new angle: "));
            }
        }
        else if (isNaN(global.destRX)) {
            if (isNaN(str)) {
                if (!parse_vector(str, &v)) {
                    alert(translate("Requires valid numeric angle or two points."));
                    prompt_output(translate("Specify second point: "));
                }
                else {
                    global.destR = v;
                    global.angleRef = calculateAngle(global.baseRX, global.baseRY, global.destRX, global.destRY);
                    previewOn("SELECTED", "ROTATE", global.baseX, global.baseY, global.angleRef);
                    setRubberPoint("LINE_START", global.baseX, global.baseY);
                    prompt_output(translate("Specify the new angle: "));
                }
            }
            else {
                //The base and dest values are only set here to advance the command.
                global.baseR = zero_vector;
                global.destR = zero_vector;
                //The reference angle is what we will use later.
                global.angleRef = Number(str);
                previewOn("SELECTED", "ROTATE", global.baseX, global.baseY, global.angleRef);
                prompt_output(translate("Specify the new angle: "));
            }
        }
        else if (isNaN(global.angleNew)) {
            if (isNaN(str)) {
                if (!parse_vector(str, &v)) {
                    alert(translate("Requires valid numeric angle or second point."));
                    prompt_output(translate("Specify the new angle: "));
                }
                else {
                    global.angleNew = emb_vector_angle(global.base, v);
                    rotateSelected(global.base.x, global.base.y,
                        global.angleNew - global.angleRef);
                    previewOff();
                    end_command();
                }
            }
            else {
                global.angleNew = Number(str);
                rotateSelected(global.baseX, global.baseY, global.angleNew - global.angleRef);
                previewOff();
                end_command();
            }
        }
    }
}
#endif

/* RGB */
ScriptValue
rgb_command(ScriptEnv *context)
{
    switch (context->context) {
    default:
    case CONTEXT_MAIN:
        init_command();
        clear_selection();
        context->mode = RGB_MODE_BACKGROUND;
        prompt_output(translate("Enter RED,GREEN,BLUE values for background or [Crosshair/Grid]: "));
        break;
    case CONTEXT_CLICK:
        /* Do Nothing, prompt only command. */
        break;
    case CONTEXT_CONTEXT:
        /* todo("RGB", "context()"); */
        break;
    }

    return script_null;
}

#if 0
void prompt(str)
{
    float v[3];
    switch (global.mode) {
    default:
    case RGB_MODE_BACKGROUND: {
        if (str == "C" || str == "CROSSHAIR") {
            /* TODO: Probably should add additional translate calls here. */
            global.mode = RGB_MODE_CROSSHAIR;
            prompt_output(translate("Specify crosshair color: "));
        }
        else if (str == "G" || str == "GRID") {
            /* TODO: Probably should add additional translate calls here. */
            global.mode = RGB_MODE_GRID;
            prompt_output(translate("Specify grid color: "));
        }
        else {
            parse_floats(str, v, 3);
            if (!validRGB(v[0], v[1], v[2])) {
                alert(translate("Invalid color. R,G,B values must be in the range of 0-255."));
                prompt_output(translate("Specify background color: "));
            }
            else {
                setBackgroundColor(v[0], v[1], v[2]);
                end_command();
            }
        }
        break;
    }
    case RGB_MODE_CROSSHAIR: {
        parse_floats(str, v, 3);
        if (!validRGB(v[0], v[1], v[2])) {
            alert(translate("Invalid color. R,G,B values must be in the range of 0-255."));
            prompt_output(translate("Specify crosshair color: "));
        }
        else {
            setCrossHairColor(v[0], v[1], v[2]);
            end_command();
        }
        break;
    }
    case RGB_MODE_GRID: {
        parse_floats(str, v, 3);
        if (!validRGB(v[0], v[1], v[2])) {
            alert(translate("Invalid color. R,G,B values must be in the range of 0-255."));
            prompt_output(translate("Specify grid color: "));
        }
        else {
            setGridColor(v[0], v[1], v[2]);
            end_command();
        }
        break;
    }
    }
}

#endif

/* . */
ScriptValue
allow_rubber_command(ScriptEnv* context)
{
    return script_bool(nativeAllowRubber());
}

/* . */
ScriptValue
set_rubber_mode_command(ScriptEnv* context)
{
    std::string mode(STR(0));

    if (mode == "CIRCLE_1P_RAD") {
        nativeSetRubberMode(OBJ_RUBBER_CIRCLE_1P_RAD);
    }
    else if (mode == "CIRCLE_1P_DIA") {
        nativeSetRubberMode(OBJ_RUBBER_CIRCLE_1P_DIA);
    }
    else if (mode == "CIRCLE_2P") {
        nativeSetRubberMode(OBJ_RUBBER_CIRCLE_2P);
    }
    else if (mode == "CIRCLE_3P") {
        nativeSetRubberMode(OBJ_RUBBER_CIRCLE_3P);
    }
    else if (mode == "CIRCLE_TTR") {
        nativeSetRubberMode(OBJ_RUBBER_CIRCLE_TTR);
    }
    else if (mode == "CIRCLE_TTT") {
        nativeSetRubberMode(OBJ_RUBBER_CIRCLE_TTT);
    }
    else if (mode == "DIMLEADER_LINE") {
        nativeSetRubberMode(OBJ_RUBBER_DIMLEADER_LINE);
    }
    else if (mode == "ELLIPSE_LINE") {
        nativeSetRubberMode(OBJ_RUBBER_ELLIPSE_LINE);
    }
    else if (mode == "ELLIPSE_MAJORDIAMETER_MINORRADIUS") {
        nativeSetRubberMode(OBJ_RUBBER_ELLIPSE_MAJORDIAMETER_MINORRADIUS);
    }
    else if (mode == "ELLIPSE_MAJORRADIUS_MINORRADIUS") {
        nativeSetRubberMode(OBJ_RUBBER_ELLIPSE_MAJORRADIUS_MINORRADIUS);
    }
    else if (mode == "ELLIPSE_ROTATION") {
        nativeSetRubberMode(OBJ_RUBBER_ELLIPSE_ROTATION);
    }
    else if (mode == "LINE") {
        nativeSetRubberMode(OBJ_RUBBER_LINE);
    }
    else if (mode == "POLYGON") {
        nativeSetRubberMode(OBJ_RUBBER_POLYGON);
    }
    else if (mode == "POLYGON_INSCRIBE") {
        nativeSetRubberMode(OBJ_RUBBER_POLYGON_INSCRIBE);
    }
    else if (mode == "POLYGON_CIRCUMSCRIBE") {
        nativeSetRubberMode(OBJ_RUBBER_POLYGON_CIRCUMSCRIBE);
    }
    else if (mode == "POLYLINE") {
        nativeSetRubberMode(OBJ_RUBBER_POLYLINE);
    }
    else if (mode == "RECTANGLE") {
        nativeSetRubberMode(OBJ_RUBBER_RECTANGLE);
    }
    else if (mode == "TEXTSINGLE") {
        nativeSetRubberMode(OBJ_RUBBER_TEXTSINGLE);
    }
    else {
        prompt_output("UNKNOWN_ERROR setRubberMode(): unknown rubberMode value");
        return script_false;
    }

    return script_null;
}

/* . */
ScriptValue
set_rubber_point_command(ScriptEnv* context)
{
    nativeSetRubberPoint(STR(0), REAL(1), REAL(2));
    return script_null;
}

/* . */
ScriptValue
set_rubber_text_command(ScriptEnv* context)
{
    nativeSetRubberText(STR(0), STR(1));
    return script_null;
}

ScriptValue
add_rubber_command(ScriptEnv* context)
{
    std::string objType(STR(0));

    if (!nativeAllowRubber()) {
        prompt_output("UNKNOWN_ERROR addRubber(): You must use vulcanize() before you can add another rubber object.");
        return script_false;
    }

    /* FIXME: ERROR CHECKING */
    double mx = _main->runCommandCore("get mousex", context).r;
    double my = _main->runCommandCore("get mousey", context).r;

    if (objType == "ARC") {
        // TODO: handle this type
    }
    else if (objType == "BLOCK") {
        // TODO: handle this type
    }
    else if (objType == "CIRCLE") {
        nativeAddCircle(mx, my, 0, false, OBJ_RUBBER_ON);
    }
    else if (objType == "DIMALIGNED") {
        // TODO: handle this type
    }
    else if (objType == "DIMANGULAR") {
        // TODO: handle this type
    }
    else if (objType == "DIMARCLENGTH") {
        // TODO: handle this type
    }
    else if (objType == "DIMDIAMETER") {
        // TODO: handle this type
    }
    else if (objType == "DIMLEADER") {
        nativeAddDimLeader(mx, my, mx, my, 0, OBJ_RUBBER_ON);
    }
    else if (objType == "DIMLINEAR") {
        // TODO: handle this type
    }
    else if (objType == "DIMORDINATE") {
        // TODO: handle this type
    }
    else if (objType == "DIMRADIUS") {
        // TODO: handle this type
    }
    else if (objType == "ELLIPSE") {
        nativeAddEllipse(mx, my, 0, 0, 0, 0, OBJ_RUBBER_ON);
    }
    else if (objType == "ELLIPSEARC") {
        // TODO: handle this type
    }
    else if (objType == "HATCH") {
        // TODO: handle this type
    }
    else if (objType == "IMAGE") {
        // TODO: handle this type
    }
    else if (objType == "INFINITELINE") {
        // TODO: handle this type
    }
    else if (objType == "LINE") {
        nativeAddLine(mx, my, mx, my, 0, OBJ_RUBBER_ON);
    }
    else if (objType == "PATH") {
        // TODO: handle this type
    }
    else if (objType == "POINT") {
        // TODO: handle this type
    }
    else if (objType == "POLYGON") {
        nativeAddPolygon(mx, my, QPainterPath(), OBJ_RUBBER_ON);
    }
    else if (objType == "POLYLINE") {
        nativeAddPolyline(mx, my, QPainterPath(), OBJ_RUBBER_ON);
    }
    else if (objType == "RAY") {
        // TODO: handle this type
    }
    else if (objType == "RECTANGLE") {
        nativeAddRectangle(mx, my, mx, my, 0, 0, OBJ_RUBBER_ON);
    }
    else if (objType == "SPLINE") {
        // TODO: handle this type
    }
    else if (objType == "TEXTMULTI") {
        // TODO: handle this type
    }
    else if (objType == "TEXTSINGLE") {
        nativeAddTextSingle("", mx, my, 0, false, OBJ_RUBBER_ON);
    }

    return script_null;
}

ScriptValue
clear_rubber_command(ScriptEnv* context)
{
    View* gview = activeView();
    if (gview) {
        gview->clearRubberRoom();
    }
    return script_null;
}

ScriptValue
spare_rubber_command(ScriptEnv* context)
{
    if (context->argumentCount != 1) {
        prompt_output("spareRubber() requires one argument");
        return script_false;
    }
    if (context->argument[0].type != SCRIPT_STRING) {
        prompt_output("TYPE_ERROR, spareRubber(): first argument is not a string");
        return script_false;
    }

    QString objID(STR(0));

    if (objID == "PATH") {
        nativeSpareRubber(SPARE_RUBBER_PATH);
    }
    else if (objID == "POLYGON") {
        nativeSpareRubber(SPARE_RUBBER_POLYGON);
    }
    else if (objID == "POLYLINE") {
        nativeSpareRubber(SPARE_RUBBER_POLYLINE);
    }
    else {
        bool ok = false;
        int64_t id = objID.toLongLong(&ok);
        if (!ok) {
            prompt_output("TYPE_ERROR, spareRubber(): error converting object ID into an int64");
            return script_false;
        }
        nativeSpareRubber(id);
    }

    return script_null;
}
