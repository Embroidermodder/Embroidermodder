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

/* . */
void
exit_program(void)
{
    debug_message("exit()");
    if (get_bool(PROMPT_SAVE_HISTORY)) {
        prompt->saveHistory("prompt.log", get_bool(PROMPT_SAVE_HISTORY_AS_HTML));
        /* TODO: get filename from settings */
    }
    qApp->closeAllWindows();
    /* Force the MainWindow destructor to run before exiting.
     * Makes Valgrind "still reachable" happy :)
     */
    _main->deleteLater();
}

/* . */
void
check_for_updates(void)
{
    debug_message("checkForUpdates()");
    /* TODO: Check website for new versions, commands, etc... */
}

/* . */
QString
platformString(void)
{
    char message[MAX_STRING_LENGTH];
    /* TODO: Append QSysInfo to string where applicable. */
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
    sprintf(message, "Platform: %s", qPrintable(os));
    debug_message(message);
    return os;
}

/* . */
void
print_command(void)
{
    debug_message("print_command()");
    MdiWindow* mdiWin = qobject_cast<MdiWindow*>(mdiArea->activeSubWindow());
    if (mdiWin) {
        mdiWin->print();
    }
}

/* . */
void
tipOfTheDay(void)
{
    debug_message("tipOfTheDay()");

    wizardTipOfTheDay = new QWizard(_main);
    wizardTipOfTheDay->setAttribute(Qt::WA_DeleteOnClose);
    wizardTipOfTheDay->setWizardStyle(QWizard::ModernStyle);
    wizardTipOfTheDay->setMinimumSize(550, 400);

    QWizardPage* page = new QWizardPage(wizardTipOfTheDay);

    ImageWidget* imgBanner = new ImageWidget("Did you know", wizardTipOfTheDay);
    // create_pixmap("did_you_know")

    if (get_int(GENERAL_CURRENT_TIP) >= string_array_length(tips)) {
        set_int(GENERAL_CURRENT_TIP, 0);
    }
    labelTipOfTheDay = new QLabel(tips[get_int(GENERAL_CURRENT_TIP)], wizardTipOfTheDay);
    labelTipOfTheDay->setWordWrap(true);

    QCheckBox* checkBoxTipOfTheDay = new QCheckBox(translate("&Show tips on startup"), wizardTipOfTheDay);
    checkBoxTipOfTheDay->setChecked(get_bool(GENERAL_TIP_OF_THE_DAY));
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

    /* TODO: Add icons to buttons by using wizardTipOfTheDay->setButton(QWizard::CustomButton1, buttonPrevious) */
    /* TODO: Add icons to buttons by using wizardTipOfTheDay->setButton(QWizard::CustomButton1, buttonNext) */
    /* TODO: Add icons to buttons by using wizardTipOfTheDay->setButton(QWizard::CustomButton1, buttonClose) */
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
    set_bool(GENERAL_TIP_OF_THE_DAY, checked);
}

/* . */
void
buttonTipOfTheDayClicked(int button)
{
    char message[MAX_STRING_LENGTH];
    sprintf(message, "buttonTipOfTheDayClicked(%d)", button);
    debug_message(message);
    int current = get_int(GENERAL_CURRENT_TIP);
    if (button == QWizard::CustomButton1) {
        if (current > 0) {
            current--;
        }
        else {
            current = string_array_length(tips)-1;
        }
        labelTipOfTheDay->setText(tips[current]);
        set_int(GENERAL_CURRENT_TIP, current);
    }
    else if (button == QWizard::CustomButton2) {
        current++;
        if (current >= string_array_length(tips)) {
            current = 0;
        }
        labelTipOfTheDay->setText(tips[current]);
        set_int(GENERAL_CURRENT_TIP, current);
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

    /* Open the HTML Help in the default browser. */
    QUrl helpURL("file:///" + qApp->applicationDirPath() + "/help/doc-index.html");
    QDesktopServices::openUrl(helpURL);

    /* TODO: This is how to start an external program. Use this elsewhere...
     * QString program = "firefox";
     * QStringList arguments;
     * arguments << "help/commands.html";
     * QProcess *myProcess = new QProcess(this);
     * myProcess->start(program, arguments);
     */
}

/* . */
void
changelog(void)
{
    debug_message("changelog()");

    QUrl changelogURL("help/changelog.html");
    QDesktopServices::openUrl(changelogURL);
}

/* Standard Slots */
void
undo_command(void)
{
    debug_message("undo_command()");
    if (dockUndoEdit->canUndo()) {
        prompt->setPrefix("Undo " + dockUndoEdit->undoText());
        appendHistory("");
        dockUndoEdit->undo();
        prompt->setPrefix(prefix);
    }
    else {
        prompt->alert("Nothing to undo");
        prompt->setPrefix(prefix);
    }
}

/* . */
void
redo_command(void)
{
    debug_message("redo_command()");
    if (dockUndoEdit->canRedo()) {
        prompt->setPrefix("Redo " + dockUndoEdit->redoText());
        appendHistory("");
        dockUndoEdit->redo();
        prompt->setPrefix(prefix);
    }
    else {
        prompt->alert("Nothing to redo");
        prompt->setPrefix(prefix);
    }
}

/* . */
bool
isShiftPressed(void)
{
    return shiftKeyPressedState;
}

/* . */
void
setShiftPressed(void)
{
    shiftKeyPressedState = true;
}

/* . */
void
setShiftReleased(void)
{
    shiftKeyPressedState = false;
}

/* Icons */
void
iconResize(int iconSize)
{
    _main->setIconSize(QSize(iconSize, iconSize));
    layerSelector->setIconSize(QSize(iconSize*4, iconSize));
    colorSelector->setIconSize(QSize(iconSize, iconSize));
    linetypeSelector->setIconSize(QSize(iconSize*4, iconSize));
    lineweightSelector->setIconSize(QSize(iconSize*4, iconSize));
    /* set the minimum combobox width so the text is always readable */
    layerSelector->setMinimumWidth(iconSize*4);
    colorSelector->setMinimumWidth(iconSize*2);
    linetypeSelector->setMinimumWidth(iconSize*4);
    lineweightSelector->setMinimumWidth(iconSize*4);

    /* TODO: low-priority: open app with iconSize set to 128. resize the icons to a smaller size. */

    set_int(GENERAL_ICON_SIZE, iconSize);
}

/* . */
MdiWindow*
activeMdiWindow()
{
    debug_message("activeMdiWindow()");
    MdiWindow* mdiWin = qobject_cast<MdiWindow*>(mdiArea->activeSubWindow());
    return mdiWin;
}

/* . */
Document*
activeDocument()
{
    debug_message("activeDocument()");
    MdiWindow* mdiWin = qobject_cast<MdiWindow*>(mdiArea->activeSubWindow());
    if (mdiWin) {
        Document* v = mdiWin->gview;
        return v;
    }
    return 0;
}

/* . */
QGraphicsScene*
activeScene(void)
{
    debug_message("activeScene()");
    MdiWindow* mdiWin = qobject_cast<MdiWindow*>(mdiArea->activeSubWindow());
    if (mdiWin) {
        QGraphicsScene* s = mdiWin->gscene;
        return s;
    }
    return 0;
}

/* . */
QUndoStack*
activeUndoStack(void)
{
    debug_message("activeUndoStack()");
    Document* v = activeDocument();
    if (v) {
        QUndoStack* u = v->data.undoStack;
        return u;
    }
    return 0;
}

/* . */
void
setUndoCleanIcon(bool opened)
{
    dockUndoEdit->updateCleanIcon(opened);
}

/* . */
void
updateAllViewScrollBars(bool val)
{
    QList<QMdiSubWindow*> windowList = mdiArea->subWindowList();
    for (int i = 0; i < windowList.count(); ++i) {
        MdiWindow* mdiWin = qobject_cast<MdiWindow*>(windowList.at(i));
        if (mdiWin) {
            mdiWin->showViewScrollBars(val);
        }
    }
}

/* . */
void
updateAllViewCrossHairColors(QRgb color)
{
    QList<QMdiSubWindow*> windowList = mdiArea->subWindowList();
    for (int i = 0; i < windowList.count(); ++i) {
        MdiWindow* mdiWin = qobject_cast<MdiWindow*>(windowList.at(i));
        if (mdiWin) {
            mdiWin->setViewCrossHairColor(color);
        }
    }
}

/* . */
void
updateAllViewBackgroundColors(QRgb color)
{
    QList<QMdiSubWindow*> windowList = mdiArea->subWindowList();
    for (int i = 0; i < windowList.count(); ++i) {
        MdiWindow* mdiWin = qobject_cast<MdiWindow*>(windowList.at(i));
        if (mdiWin) {
            mdiWin->setViewBackgroundColor(color);
        }
    }
}

/* . */
void
updateAllViewSelectBoxColors(QRgb colorL, QRgb fillL, QRgb colorR, QRgb fillR, int alpha)
{
    QList<QMdiSubWindow*> windowList = mdiArea->subWindowList();
    for (int i = 0; i < windowList.count(); ++i) {
        MdiWindow* mdiWin = qobject_cast<MdiWindow*>(windowList.at(i));
        if (mdiWin) {
            mdiWin->setViewSelectBoxColors(colorL, fillL, colorR, fillR, alpha);
        }
    }
}

/* . */
void
updateAllViewGridColors(QRgb color)
{
    QList<QMdiSubWindow*> windowList = mdiArea->subWindowList();
    for (int i = 0; i < windowList.count(); ++i) {
        MdiWindow* mdiWin = qobject_cast<MdiWindow*>(windowList.at(i));
        if (mdiWin) {
            mdiWin->setViewGridColor(color);
        }
    }
}

/* . */
void
updateAllViewRulerColors(QRgb color)
{
    QList<QMdiSubWindow*> windowList = mdiArea->subWindowList();
    for (int i = 0; i < windowList.count(); ++i) {
        MdiWindow* mdiWin = qobject_cast<MdiWindow*>(windowList.at(i));
        if (mdiWin) {
            mdiWin->setViewRulerColor(color);
        }
    }
}

/* . */
void
updatePickAddMode(bool val)
{
    set_bool(SELECTION_MODE_PICKADD, val);
    dockPropEdit->updatePickAddModeButton(val);
}

/* . */
void
pickAddModeToggled(void)
{
    bool val = !get_bool(SELECTION_MODE_PICKADD);
    updatePickAddMode(val);
}

/* Layer ToolBar */
void
makeLayerActive(void)
{
    debug_message("makeLayerActive()");
    todo("Implement makeLayerActive.");
}

/* . */
void
layerManager(void)
{
    debug_message("layerManager()");
    todo("Implement layerManager.");
    LayerManager layman(_main, _main);
    layman.exec();
}

/* . */
void
layerPrevious(void)
{
    debug_message("layerPrevious()");
    todo("Implement layerPrevious.");
}

/* . */
void
layerSelectorIndexChanged(int index)
{
    EmbString message;
    sprintf(message, "layerSelectorIndexChanged(%d)", index);
    debug_message(message);
}

void
MainWindow::colorSelectorIndexChanged(int index)
{
    EmbString message;
    sprintf(message, "colorSelectorIndexChanged(%d)", index);
    debug_message(message);

    QComboBox* comboBox = qobject_cast<QComboBox*>(sender());
    QRgb newColor;
    if (comboBox) {
        bool ok = 0;
        /* TODO: Handle ByLayer and ByBlock and Other... */
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
        currentColorChanged(newColor);
    }
}

/* . */
void
linetypeSelectorIndexChanged(int index)
{
    EmbString message;
    sprintf(message, "linetypeSelectorIndexChanged(%d)", index);
    debug_message(message);
}

/* . */
void
lineweightSelectorIndexChanged(int index)
{
    char message[MAX_STRING_LENGTH];
    sprintf(message, "lineweightSelectorIndexChanged(%d)", index);
    debug_message(message);
}

/* . */
void
textFontSelectorCurrentFontChanged(const QFont& font)
{
    debug_message("textFontSelectorCurrentFontChanged()");
    setTextFont((char*)qPrintable(font.family()));
}

/* . */
void
textSizeSelectorIndexChanged(int index)
{
    char message[MAX_STRING_LENGTH];
    sprintf(message, "textSizeSelectorIndexChanged(%d)", index);
    debug_message(message);
    /* TODO: check that the toReal() conversion is ok. */
    set_real(TEXT_SIZE, fabs(textSizeSelector->itemData(index).toReal()));
}

/* . */
void
setTextFont(EmbString str)
{
    textFontSelector->setCurrentFont(QFont(str));
    set_str(TEXT_FONT, (char*)qPrintable(str));
}

/* . */
void
setTextSize(double num)
{
    set_real(TEXT_SIZE, fabs(num));
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

/* . */
QString
getCurrentLayer()
{
    MdiWindow* mdiWin = qobject_cast<MdiWindow*>(mdiArea->activeSubWindow());
    if (mdiWin) {
        return mdiWin->curLayer;
    }
    return "0";
}

/* . */
QRgb
getCurrentColor()
{
    MdiWindow* mdiWin = qobject_cast<MdiWindow*>(mdiArea->activeSubWindow());
    if (mdiWin) {
        return mdiWin->curColor;
    }
    return 0; /* TODO: return color ByLayer */
}

/* . */
QString
getCurrentLineType()
{
    MdiWindow* mdiWin = qobject_cast<MdiWindow*>(mdiArea->activeSubWindow());
    if (mdiWin) {
        return mdiWin->curLineType;
    }
    return "ByLayer";
}

/* . */
QString
getCurrentLineWeight()
{
    MdiWindow* mdiWin = qobject_cast<MdiWindow*>(mdiArea->activeSubWindow());
    if (mdiWin) {
        return mdiWin->curLineWeight;
    }
    return "ByLayer";
}

/* . */
void
deletePressed(void)
{
    debug_message("deletePressed()");
    wait_cursor();
    MdiWindow* mdiWin = qobject_cast<MdiWindow*>(mdiArea->activeSubWindow());
    if (mdiWin) {
        /* TODO: change document based on selection */
    }
    restore_cursor();
}

/* . */
void
escapePressed(void)
{
    debug_message("escapePressed()");
    wait_cursor();
    MdiWindow* mdiWin = qobject_cast<MdiWindow*>(mdiArea->activeSubWindow());
    if (mdiWin) {
        /* TODO: change document based on selection */
    }
    restore_cursor();

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
promptHistoryAppended(QString txt)
{
    MdiWindow* mdiWin = activeMdiWindow();
    if (mdiWin) {
        mdiWin->promptHistoryAppended(txt);
    }
}

/* . */
void
logPromptInput(QString txt)
{
    MdiWindow* mdiWin = activeMdiWindow();
    if (mdiWin) {
        mdiWin->logPromptInput(txt);
    }
}

/* . */
void
promptInputPrevious(void)
{
    MdiWindow* mdiWin = activeMdiWindow();
    if (mdiWin) {
        mdiWin->promptInputPrevious();
    }
}

/* . */
void
promptInputNext(void)
{
    MdiWindow* mdiWin = activeMdiWindow();
    if (mdiWin) {
        mdiWin->promptInputNext();
    }
}

/*
 * BUG: pan commands broke
 */
ScriptValue
run_command(const EmbString cmd, ScriptEnv *context)
{
    char message[MAX_STRING_LENGTH];
    int id = get_command_id((char*)cmd);
    Document* doc = NULL;
    ScriptValue value = script_true;
    sprintf(message, "run_command(%s) %d", cmd, id);
    debug_message(message);

    if (id < 0) {
        sprintf(message, "ERROR: %s not found in command_data.", cmd);
        debug_message(message);
        return script_false;
    }

    if (!argument_checks(context, id)) {
        /* TODO: error */
        return script_false;
    }

    if (command_data[id].flags & REQUIRED_VIEW) {
        doc = activeDocument();
        if (!doc) {
            return value;
        }
    }
    if (!(command_data[id].flags & DONT_INITIALIZE)) {
        doc = activeDocument();
        if (doc) {
            doc_clear_rubber_room(doc);
        }
    }
    if (command_data[id].flags & CLEAR_SELECTION) {
        doc = activeDocument();
        if (doc) {
            doc_clear_selection(doc);
        }
    }

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
        doc_copy(doc);
        break;
    }
    case ACTION_COPY_SELECTED: {
        /* nativeCopySelected(REAL(0), REAL(1)); */
        break;
    }
    case ACTION_COLOR_SELECTOR:
        prompt->alert("TODO: COLORSELECTOR");
        break;
    case ACTION_CUT:
        nativeCutSelected(REAL(0), REAL(1));
        break;

    case ACTION_DEBUG:
        appendHistory(QString(STR(0)));
        break;

    case ACTION_DESIGN_DETAILS:
        create_details_dialog();
        break;

    case ACTION_DIM_LEADER: {
        nativeAddDimLeader(REAL(0), REAL(1), REAL(2), REAL(3), REAL(4), RUBBER_OFF);
        break;
    }

    case ACTION_DISABLE: {
        QString value(STR(0));
        if (value == "MOVERAPIDFIRE") {
            doc_disable_move_rapid_fire(doc);
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
            doc_enable_move_rapid_fire(doc);
        }
        if (value == "PROMPTRAPIDFIRE") {
            prompt->enableRapidFire();
        }
        break;
    }

    case ACTION_EXIT:
        exit_program();
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
        doc_mirror_selected(doc, REAL(0), -REAL(1), REAL(2), -REAL(3));
        break;
    }

    case ACTION_NEW:
        new_file();
        break;

/*
    case ACTION_NUM_SELECTED: {
        
        break;
    }
 */

    case ACTION_OPEN:
        openFile(false, "");
        break;

    case ACTION_PASTE: {
        doc_paste(doc);
        break;
    }

    case ACTION_PASTE_SELECTED: {
        /* Paste with location x,y */
        /* nativePasteSelected(REAL(0), REAL(1)); */
        break;
    }
    case ACTION_PLATFORM:
        /* Should this display in the command prompt or just return like GET? */
        /* prompt_output(translate("Platform") + " = " + _main->platformString()); */
        break;
    case ACTION_REDO:
        redo_command();
        break;

    case ACTION_SAVE:
        save_file();
        break;

    case ACTION_SAVE_AS:
        /* save(); */
        break;

    case ACTION_SCALE_SELECTED:
        /*  */
        break;

    case ACTION_SETTINGS_DIALOG: {
        settingsDialog("");
        break;
    }

    case ACTION_TEXT_BOLD:
        set_bool(TEXT_STYLE_BOLD, !get_bool(TEXT_STYLE_BOLD));
        break;

    case ACTION_TEXT_ITALIC:
        set_bool(TEXT_STYLE_ITALIC, !get_bool(TEXT_STYLE_ITALIC));
        break;

    case ACTION_TEXT_UNDERLINE:
        set_bool(TEXT_STYLE_UNDERLINE, !get_bool(TEXT_STYLE_UNDERLINE));
        break;

    case ACTION_TEXT_STRIKEOUT:
        set_bool(TEXT_STYLE_STRIKEOUT, !get_bool(TEXT_STYLE_STRIKEOUT));
        break;

    case ACTION_TEXT_OVERLINE:
        set_bool(TEXT_STYLE_OVERLINE, !get_bool(TEXT_STYLE_OVERLINE));
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
        undo_command();
        break;

    case ACTION_VULCANIZE: {
        doc_vulcanize_rubber_room(doc);
        break;
    }

    case ACTION_DAY: {
        /* TODO: Make day vision color settings. */
        doc_set_background_color(doc, qRgb(255,255,255)); 
        doc_set_cross_hair_color(doc, qRgb(0,0,0));
        doc_set_grid_color(doc, qRgb(0,0,0));
        break;
    }
    case ACTION_NIGHT: {
        /* TODO: Make night vision color settings. */
        doc_set_background_color(doc, qRgb(0,0,0));
        doc_set_cross_hair_color(doc, qRgb(255,255,255));
        doc_set_grid_color(doc, qRgb(255,255,255));
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
        nativeAddCircle(REAL(0), REAL(1), REAL(2), BOOL(4), RUBBER_OFF);
        break;
    }

    case ACTION_DISTANCE:
        break;

    case ACTION_DOLPHIN:
        break;

    case ACTION_ELLIPSE: {
        nativeAddEllipse(REAL(0), REAL(1), REAL(2), REAL(3), REAL(4), BOOL(5), RUBBER_OFF);
        break;
    }

    case ACTION_ERASE: {
        if (doc_num_selected(doc) <= 0) {
            /* TODO: Prompt to select objects if nothing is preselected. */
            prompt->alert(
            translate("Preselect objects before invoking the delete command."));
            information_box(translate("Delete Preselect"),
                translate("Preselect objects before invoking the delete command."));
        }
        else {
            doc_delete_selected(doc);
        }
        break;
    }

    case ACTION_ERROR: {
        std::string s = "ERROR: (";
        s += STR(0);
        s += ") ";
        s += STR(1);
        prompt->setPrefix(QString(s.c_str()));
        appendHistory("");
        break;
    }

    case ACTION_HEART:
        break;

    case ACTION_LINE: {
        nativeAddLine(REAL(0), REAL(1), REAL(2), REAL(3), REAL(4), RUBBER_OFF);
        break;
    }

    case ACTION_LOCATE_POINT:
        break;

    case ACTION_MOVE:
        move_command(context);
        break;

    case ACTION_MOVE_SELECTED: {
        doc_move_selected(doc, REAL(0), -REAL(1));
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
        doc_preview_off(doc);
        break;
    }

    case ACTION_PREVIEW_ON: {
        value = previewon_command(context);
        break;
    }

    case ACTION_QUICKLEADER:
        break;

    case ACTION_RECTANGLE: {
        nativeAddRectangle(REAL(0), REAL(1), REAL(2), REAL(3), REAL(4), BOOL(5), RUBBER_OFF);
        break;
    }

    case ACTION_RGB:
        break;

    case ACTION_ROTATE: {
        doc_rotate_selected(doc, REAL(0), -REAL(1), -REAL(2));
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
        doc_select_all(doc);
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
        doc_delete_selected(doc);
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
        doc_pan_real_time(doc);
        break;
    }
    case ACTION_PAN_POINT: {
        doc_pan_point(doc);
        break;
    }
    case ACTION_PAN_LEFT: {
        QUndoStack* stack = activeUndoStack();
        if (stack) {
            UndoableCommand* cmd = new UndoableCommand(ACTION_NAV, "PanLeft", doc, 0);
            stack->push(cmd);
        }
        break;
    }
    case ACTION_PAN_RIGHT: {
        QUndoStack* stack = activeUndoStack();
        if (stack) {
            UndoableCommand* cmd = new UndoableCommand(ACTION_NAV, "PanRight", doc, 0);
            stack->push(cmd);
        }
        break;
    }
    case ACTION_PAN_UP: {
        QUndoStack* stack = activeUndoStack();
        if (stack) {
            UndoableCommand* cmd = new UndoableCommand(ACTION_NAV, "PanUp", doc, 0);
            stack->push(cmd);
        }
        break;
    }
    case ACTION_PAN_DOWN: {
        QUndoStack* stack = activeUndoStack();
        if (stack) {
            UndoableCommand* cmd = new UndoableCommand(ACTION_NAV, "PanDown", doc, 0);
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
        todo("Implement zoomAll.");
        break;
    }
    case ACTION_ZOOM_CENTER: {
        todo("Implement zoomCenter.");
        break;
    }
    case ACTION_ZOOM_DYNAMIC: {
        todo("Implement zoomDynamic.");
        break;
    }
    case ACTION_ZOOM_EXTENTS: {
        QUndoStack* stack = activeUndoStack();
        if (stack) {
            UndoableCommand* cmd = new UndoableCommand(ACTION_NAV, "ZoomExtents", doc, 0);
            stack->push(cmd);
        }
        break;
    }
    case ACTION_ZOOM_IN: {
        doc_zoom_in(doc);
        break;
    }
    case ACTION_ZOOM_OUT: {
        doc_zoom_out(doc);
        break;
    }
    case ACTION_ZOOM_PREVIOUS: {
        todo("Implement zoomPrevious.");
        break;
    }
    case ACTION_ZOOM_REAL_TIME: {
        todo("Implement zoomRealtime.");
        break;
    }
    case ACTION_ZOOM_SCALE: {
        todo("Implement zoomScale.");
        break;
    }
    case ACTION_ZOOM_SELECTED: {
        QUndoStack* stack = activeUndoStack();
        if (stack) {
            UndoableCommand* cmd = new UndoableCommand(ACTION_NAV, "ZoomSelected", doc, 0);
            stack->push(cmd);
        }
        break;
    }
    case ACTION_ZOOM_WINDOW: {
        doc_zoom_window(doc);
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
        EmbString message;
        sprintf(message, "runCommand(%s)", qPrintable(act->objectName()));
        debug_message(message);
        promptInput->endCommand();
        prompt->setCurrentText(act->objectName());
        promptInput->processInput();
    }
}

/* FIXME: reconnect to new command system.
 */
void
runCommandMain(EmbString cmd)
{
    EmbString message;
    ScriptEnv *context = create_script_env();
    context->context = CONTEXT_MAIN;
    sprintf(message, "runCommandMain(%s)", qPrintable(cmd));
    debug_message(message);
    /* TODO: Uncomment this when post-selection is available. */
    /*
    if (!get_bool(SELECTION_MODE_PICKFIRST)) {
        clear_selection();
    }
    */
    run_command((char*)qPrintable(cmd), context);
    free_script_env(context);
}

/* FIXME: reconnect to new command system.
 */
void
runCommandClick(EmbString cmd, double x, double y)
{
    EmbString message;
    ScriptEnv *context = create_script_env();
    context->context = CONTEXT_CLICK;
    sprintf(message, "runCommandClick(%s, %.2f, %.2f)", qPrintable(cmd), x, y);
    debug_message(message);
    /* engine->evaluate(cmd + "_click(" + QString().setNum(x) + "," + QString().setNum(-y) + ")", fileName); */
    run_command((char*)qPrintable(cmd), context);
    free_script_env(context);
}

/* FIXME: reconnect to new command system.
 */
void
runCommandMove(EmbString cmd, double x, double y)
{
    EmbString message;
    ScriptEnv *context = create_script_env();
    context->context = CONTEXT_MOVE;
    sprintf(message, "runCommandMove(%s, %.2f, %.2f)", qPrintable(cmd), x, y);
    debug_message(message);
    /* engine->evaluate(cmd + "_move(" + QString().setNum(x) + "," + QString().setNum(-y) + ")", fileName); */
    run_command((char*)qPrintable(cmd), context);
    free_script_env(context);
}

/* FIXME: reconnect to new command system.
 */
void
runCommandContext(EmbString cmd, EmbString str)
{
    EmbString message;
    ScriptEnv *context = create_script_env();
    context->context = CONTEXT_CONTEXT;
    sprintf(message, "runCommandContext(%s, %s)", qPrintable(cmd), qPrintable(str));
    debug_message(message);
    /* engine->evaluate(cmd + "_context('" + str.toUpper() + "')", fileName); */
    run_command((char*)qPrintable(cmd), context);
    free_script_env(context);
}

/* FIXME: reconnect to new command system.
 * NOTE: Replace any special characters that will cause a syntax error
 */
void
runCommandPrompt(EmbString cmd)
{
    EmbString message;
    ScriptEnv *context = create_script_env();
    sprintf(message, "runCommandPrompt(%s)", qPrintable(cmd));
    debug_message(message);
    context->context = CONTEXT_PROMPT;
    if (prompt->isRapidFireEnabled()) {
        run_command((char*)qPrintable(cmd), context);
    }
    else {
        /* Both branches run the same. */
        run_command((char*)qPrintable(cmd), context);
    }
    free_script_env(context);
}

/* NOTE: translation is the repsonisbility of the caller, because some reports
 * include parts that aren't translated. For example:
 *
 *     char message[MAX_STRING_LENGTH];
 *     sprintf(message, "%s: x > %f", translate("Value of X is too small"), x);
 *     critical_box(translate("Out of Bounds"), message);
 */
void
critical_box(const char *title, const char *text)
{
    QMessageBox::critical(_main, title, text);
}

/* See critical_box comment. */
void
information_box(const char *title, const char *text)
{
    QMessageBox::information(_main, title, text);
}

/* See critical_box comment. */
void
question_box(const char *title, const char *text)
{
    QMessageBox::question(_main, title, text);
}

/* See critical_box comment. */
void
warning_box(const char *title, const char *text)
{
    QMessageBox::warning(_main, title, text);
}

/* . */
void
stub_testing(void)
{
    warning_box(translate("Testing Feature"),
        translate("<b>This feature is in testing.</b>"));
}

/* . */
void
nativeSetBackgroundColor(uint8_t r, uint8_t g, uint8_t b)
{
    set_int(DISPLAY_BG_COLOR, qRgb(r,g,b));
    updateAllViewBackgroundColors(qRgb(r,g,b));
}

/* . */
void
nativeSetCrossHairColor(uint8_t r, uint8_t g, uint8_t b)
{
    set_int(DISPLAY_CROSSHAIR_COLOR, qRgb(r,g,b));
    updateAllViewCrossHairColors(qRgb(r,g,b));
}

/* . */
void
nativeSetGridColor(uint8_t r, uint8_t g, uint8_t b)
{
    set_int(GRID_COLOR, qRgb(r,g,b));
    updateAllViewGridColors(qRgb(r,g,b));
}

/* . */
bool
nativeAllowRubber()
{
    Document* doc = activeDocument();
    if (doc) {
        return doc_allow_rubber(doc);
    }
    return false;
}

/* . */
void
nativeSpareRubber(int64_t id)
{
    Document* doc = activeDocument();
    if (doc) {
        doc_spare_rubber(doc, id);
    }
}

/* . */
void
nativeSetRubberMode(int mode)
{
    Document* doc = activeDocument();
    if (doc) {
        doc_set_rubber_mode(doc, mode);
    }
}

/* . */
void
nativeSetRubberPoint(EmbString key, double x, double y)
{
    Document* doc = activeDocument();
    if (doc) {
        EmbVector v = emb_vector(x, -y);
        doc_set_rubber_point(doc, key, v);
    }
}

/* . */
void
nativeSetRubberText(EmbString key, EmbString txt)
{
    Document* doc = activeDocument();
    if (doc) {
        doc_set_rubber_text(doc, key, txt);
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
    Document* doc = activeDocument();
    QGraphicsScene* gscene = activeScene();
    QUndoStack* stack = activeUndoStack();
    if (doc && gscene && stack) {
        Object* obj = create_text_single(QString(str.c_str()), x, -y, getCurrentColor());
        obj_set_text_font(obj, get_str(TEXT_FONT));
        obj_set_text_size(obj, get_real(TEXT_SIZE));
        obj_set_text_style(obj,
            get_bool(TEXT_STYLE_BOLD),
            get_bool(TEXT_STYLE_ITALIC),
            get_bool(TEXT_STYLE_UNDERLINE),
            get_bool(TEXT_STYLE_STRIKEOUT),
            get_bool(TEXT_STYLE_OVERLINE));
        obj_set_text_backward(obj, false);
        obj_set_text_upside_down(obj, false);
        obj->setRotation(-rot);
        /* TODO: single line text fill. */
        obj_set_rubber_mode(obj->core, rubberMode);
        if (rubberMode) {
            doc_add_to_rubber_room(doc, obj);
            gscene->addItem(obj);
            gscene->update();
        }
        else {
            UndoableCommand* cmd = new UndoableCommand(ACTION_ADD, obj->core->OBJ_NAME, obj, doc, 0);
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
    Document* doc = activeDocument();
    QGraphicsScene* gscene = activeScene();
    QUndoStack* stack = activeUndoStack();
    if (doc && gscene && stack) {
        Object* obj = create_line(x1, -y1, x2, -y2, getCurrentColor());
        obj->setRotation(-rot);
        obj_set_rubber_mode(obj->core, rubberMode);
        if (rubberMode) {
            doc_add_to_rubber_room(doc, obj);
            gscene->addItem(obj);
            gscene->update();
        }
        else {
            UndoableCommand* cmd = new UndoableCommand(ACTION_ADD, obj->core->OBJ_NAME, obj, doc, 0);
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
    Document* doc = activeDocument();
    QGraphicsScene* gscene = activeScene();
    QUndoStack* stack = activeUndoStack();
    if (!(doc && gscene && stack)) {
        return;
    }
    Object* obj = create_rect(x, -y, w, -h, getCurrentColor());
    obj->setRotation(-rot);
    obj_set_rubber_mode(obj->core, rubberMode);
    /* TODO: rect fill */
    if (rubberMode) {
        doc_add_to_rubber_room(doc, obj);
        gscene->addItem(obj);
        gscene->update();
    }
    else {
        UndoableCommand* cmd = new UndoableCommand(ACTION_ADD, obj->core->OBJ_NAME, obj, doc, 0);
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
    Document* doc = activeDocument();
    QGraphicsScene* scene = activeScene();
    if (doc && scene) {
        EmbArc arc;
        arc.start.x = x1;
        arc.start.y = -y1;
        arc.mid.y = x2;
        arc.mid.y = -y2;
        arc.end.y = x3;
        arc.end.y = -y3;
        Object* arcObj = create_arc(arc, getCurrentColor());
        obj_set_rubber_mode(arcObj->core, rubberMode);
        if (rubberMode) {
            doc_add_to_rubber_room(doc, arcObj);
        }
        scene->addItem(arcObj);
        scene->update();
    }
}

void
nativeAddCircle(double centerX, double centerY, double radius, bool fill, int rubberMode)
{
    Document* doc = activeDocument();
    QGraphicsScene* gscene = activeScene();
    QUndoStack* stack = activeUndoStack();
    if (doc && gscene && stack) {
        EmbCircle circle;
        circle.center.x = centerX;
        circle.center.y = -centerY;
        circle.radius = radius;
        Object* obj = create_circle(circle, getCurrentColor());
        obj_set_rubber_mode(obj->core, rubberMode);
        /* TODO: circle fill. */
        if (rubberMode) {
            doc_add_to_rubber_room(doc, obj);
            gscene->addItem(obj);
            gscene->update();
        }
        else {
            UndoableCommand* cmd = new UndoableCommand(ACTION_ADD, obj->core->OBJ_NAME, obj, doc, 0);
            stack->push(cmd);
        }
    }
}

void
nativeAddSlot(double centerX, double centerY, double diameter, double length, double rot, bool fill, int rubberMode)
{
    /* TODO: Use UndoableCommand for slots */
    /*
    Object* slotObj = new Object(centerX, -centerY, diameter, length, getCurrentColor());
    slotObj->setRotation(-rot);
    obj_set_rubber_mode(slotObj->core, rubberMode);
    if (rubberMode) doc_add_to_rubber_room(doc, slotObj);
    scene->addItem(slotObj); */
    /* TODO: slot fill */ /*
    scene->update();
    */
}

void
nativeAddEllipse(double centerX, double centerY, double width, double height, double rot, bool fill, int rubberMode)
{
    Document* doc = activeDocument();
    QGraphicsScene* gscene = activeScene();
    QUndoStack* stack = activeUndoStack();
    if (doc && gscene && stack) {
        EmbEllipse ellipse;
        ellipse.center.x = centerX;
        ellipse.center.y = -centerY;
        ellipse.radius.x = width/2.0;
        ellipse.radius.y = height/2.0;
        Object* obj = create_ellipse(ellipse, getCurrentColor());
        obj->setRotation(-rot);
        obj_set_rubber_mode(obj->core, rubberMode);
        /* TODO: ellipse fill */
        if (rubberMode) {
            doc_add_to_rubber_room(doc, obj);
            gscene->addItem(obj);
            gscene->update();
        }
        else {
            UndoableCommand* cmd = new UndoableCommand(ACTION_ADD, obj->core->OBJ_NAME, obj, doc, 0);
            stack->push(cmd);
        }
    }
}

void
nativeAddPoint(double x, double y)
{
    Document* doc = activeDocument();
    QUndoStack* stack = activeUndoStack();
    if (doc && stack) {
        EmbPoint point;
        point.position.x = x;
        point.position.y = -y;
        Object* obj = create_point(point, getCurrentColor());
        UndoableCommand* cmd = new UndoableCommand(ACTION_ADD, obj->core->OBJ_NAME, obj, doc, 0);
        stack->push(cmd);
    }
}

void
nativeAddRegularPolygon(double centerX, double centerY, quint16 sides, uint8_t mode, double rad, double rot, bool fill)
{
}

/* NOTE: This native is different than the rest in that the Y+ is down
 * (scripters need not worry about this)
 */
void
nativeAddPolygon(double startX, double startY, const QPainterPath& p, int rubberMode)
{
    Document* doc = activeDocument();
    QGraphicsScene* gscene = activeScene();
    QUndoStack* stack = activeUndoStack();
    if (doc && gscene && stack) {
        EmbPolygon polygon;
        Object* obj = create_polygon(startX, startY, p, getCurrentColor());
        obj_set_rubber_mode(obj->core, rubberMode);
        if (rubberMode) {
            doc_add_to_rubber_room(doc, obj);
            gscene->addItem(obj);
            gscene->update();
        }
        else {
            UndoableCommand* cmd = new UndoableCommand(ACTION_ADD, obj->core->OBJ_NAME, obj, doc, 0);
            stack->push(cmd);
        }
    }
}

/* NOTE: This native is different than the rest in that the Y+ is down
 * (scripters need not worry about this)
 */
void
nativeAddPolyline(double startX, double startY, const QPainterPath& p, int rubberMode)
{
    Document* doc = activeDocument();
    QGraphicsScene* gscene = activeScene();
    QUndoStack* stack = activeUndoStack();
    if (doc && gscene && stack) {
        EmbPath path;
        Object* obj = create_polygon(startX, startY, p, getCurrentColor());
        obj_set_rubber_mode(obj->core, rubberMode);
        if (rubberMode) {
            doc_add_to_rubber_room(doc, obj);
            gscene->addItem(obj);
            gscene->update();
        }
        else {
            UndoableCommand* cmd = new UndoableCommand(ACTION_ADD, obj->core->OBJ_NAME, obj, doc, 0);
            stack->push(cmd);
        }
    }
}

/* NOTE: This native is different than the rest in that the Y+ is down
 * (scripters need not worry about this)
 */
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
nativeAddImage(QString img, double x, double y, double w, double h, double rot)
{
}

void
nativeAddDimLeader(double x1, double y1, double x2, double y2, double rot, int rubberMode)
{
    Document* doc = activeDocument();
    QGraphicsScene* gscene = activeScene();
    QUndoStack* stack = activeUndoStack();
    if (doc && gscene && stack) {
        Object* obj = create_dim_leader(x1, -y1, x2, -y2, getCurrentColor());
        obj->setRotation(-rot);
        obj_set_rubber_mode(obj->core, rubberMode);
        if (rubberMode) {
            doc_add_to_rubber_room(doc, obj);
            gscene->addItem(obj);
            gscene->update();
        }
        else {
            UndoableCommand* cmd = new UndoableCommand(ACTION_ADD, obj->core->OBJ_NAME, obj, doc, 0);
            stack->push(cmd);
        }
    }
}

void
nativeSetCursorShape(char shape[MAX_STRING_LENGTH])
{
    Document* doc = activeDocument();
    if (doc) {
        if (!strcmp(shape, "arrow")) {
            doc->setCursor(QCursor(Qt::ArrowCursor));
        }
        else if (!strcmp(shape, "uparrow")) {
            doc->setCursor(QCursor(Qt::UpArrowCursor));
        }
        else if (!strcmp(shape, "cross")) {
            doc->setCursor(QCursor(Qt::CrossCursor));
        }
        else if (!strcmp(shape, "wait")) {
            doc->setCursor(QCursor(Qt::WaitCursor));
        }
        else if (!strcmp(shape, "ibeam"))
            doc->setCursor(QCursor(Qt::IBeamCursor));
        else if (!strcmp(shape, "resizevert"))
            doc->setCursor(QCursor(Qt::SizeVerCursor));
        else if (!strcmp(shape, "resizehoriz"))
            doc->setCursor(QCursor(Qt::SizeHorCursor));
        else if (!strcmp(shape, "resizediagleft"))
            doc->setCursor(QCursor(Qt::SizeBDiagCursor));
        else if (!strcmp(shape, "resizediagright"))
            doc->setCursor(QCursor(Qt::SizeFDiagCursor));
        else if (!strcmp(shape, "move"))
            doc->setCursor(QCursor(Qt::SizeAllCursor));
        else if (!strcmp(shape, "blank"))
            doc->setCursor(QCursor(Qt::BlankCursor));
        else if (!strcmp(shape, "splitvert"))
            doc->setCursor(QCursor(Qt::SplitVCursor));
        else if (!strcmp(shape, "splithoriz"))
            doc->setCursor(QCursor(Qt::SplitHCursor));
        else if (!strcmp(shape, "handpointing"))
            doc->setCursor(QCursor(Qt::PointingHandCursor));
        else if (!strcmp(shape, "forbidden"))
            doc->setCursor(QCursor(Qt::ForbiddenCursor));
        else if (!strcmp(shape, "handopen"))
            doc->setCursor(QCursor(Qt::OpenHandCursor));
        else if (!strcmp(shape, "handclosed"))
            doc->setCursor(QCursor(Qt::ClosedHandCursor));
        else if (!strcmp(shape, "whatsthis"))
            doc->setCursor(QCursor(Qt::WhatsThisCursor));
        else if (!strcmp(shape, "busy"))
            doc->setCursor(QCursor(Qt::BusyCursor));
        else if (!strcmp(shape, "dragmove"))
            doc->setCursor(QCursor(Qt::DragMoveCursor));
        else if (!strcmp(shape, "dragcopy"))
            doc->setCursor(QCursor(Qt::DragCopyCursor));
        else if (!strcmp(shape, "draglink"))
            doc->setCursor(QCursor(Qt::DragLinkCursor));
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

    Document* doc = activeDocument();
    if (doc) {
        doc_scale_selected(doc, x, -y, factor);
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
prompt_output(const EmbString txt)
{
    appendHistory(QString(txt));
}

void
cut(void)
{
    Document* doc = activeDocument();
    if (doc) {
        doc_cut(doc);
    }
}

void
end_command(void)
{
    Document* doc = activeDocument();
    if (doc) {
        doc_clear_rubber_room(doc);
        doc_preview_off(doc);
        doc_disable_move_rapid_fire(doc);
    }
    promptInput->endCommand();
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
    arrow_cursor();
    debug_message("about()");

    QString title = "About Embroidermodder 2";

    QDialog dialog(_main);
    QLabel image_label;
    QPixmap img = create_pixmap("logo_small");
    image_label.setPixmap(img);
    QString body_text = "Embroidermodder 2\n\n";
    /* add version here */
    body_text += translate("http://www.libembroidery.org");
    body_text += "\n\n";
    body_text += translate("Available Platforms: GNU/Linux, Windows, Mac OSX, Raspberry Pi");
    body_text += "\n\n";
    body_text += translate("Embroidery formats by Josh Varga.");
    body_text += "\n\n";
    body_text += translate("User Interface by Jonathan Greig.");
    body_text += "\n\n";
    body_text += translate("Free under the zlib/libpng license.");
#if defined(BUILD_GIT_HASH)
    body_text += "\n\n";
    body_text += translate("Build Hash: ");
    body_text += qPrintable(BUILD_GIT_HASH);
#endif
    QLabel text(body_text);
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

    restore_cursor();
}

/* GET is a prompt-only Command. */
ScriptValue
get_command(ScriptEnv* context)
{
    char message[MAX_STRING_LENGTH];
    QString value(STR(0));

    if (value == "MOUSEX") {
        QGraphicsScene* scene = activeScene();
        if (!scene) {
            return script_false;
        }
        ScriptValue r = script_real(scene->property("SCENE_MOUSE_POINT").toPointF().x());
        sprintf(message, "mouseY: %.50f", r.r);
        debug_message(message);
        return r;
    }
    else if (value == "MOUSEY") {
        QGraphicsScene* scene = activeScene();
        if (!scene) {
            return script_false;
        }
        ScriptValue r = script_real(-scene->property("SCENE_MOUSE_POINT").toPointF().y());
        sprintf(message, "mouseY: %.50f", r.r);
        debug_message(message);
        return r;
    }
    else if (value == "TEXTANGLE") {
        return setting[TEXT_ANGLE].setting;
    }
    else if (value == "TEXTBOLD") {
        return setting[TEXT_STYLE_BOLD].setting;
    }
    else if (value == "TEXTITALIC") {
        return setting[TEXT_STYLE_ITALIC].setting;
    }
    else if (value == "TEXTFONT") {
        return setting[TEXT_FONT].setting;
    }
    else if (value == "TEXTOVERLINE") {
        return setting[TEXT_STYLE_OVERLINE].setting;
    }
    else if (value == "TEXTSIZE") {
        return setting[TEXT_SIZE].setting;
    }
    else if (value == "TEXTSTRIKEOUT") {
        return setting[TEXT_STYLE_STRIKEOUT].setting;
    }
    else if (value == "TEXTUNDERLINE") {
        return setting[TEXT_STYLE_UNDERLINE].setting;
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
    int mode = PREVIEW_NULL;
    if (modeStr == "MOVE") {
        mode = PREVIEW_MOVE;
    }
    else if (modeStr == "ROTATE") {
        mode = PREVIEW_ROTATE;
    }
    else if (modeStr == "SCALE") {
        mode = PREVIEW_SCALE;
    }
    else {
        prompt_output("UNKNOWN_ERROR previewOn(): second argument must be \"MOVE\", \"ROTATE\" or \"SCALE\".");
        return script_false;
    }

    Document* doc = activeDocument();
    if (doc) {
        doc_preview_on(doc, clone, mode, REAL(2), -REAL(3), REAL(4));
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
        set_real(TEXT_ANGLE, REAL(1));
    }
    else if (value == "TEXTBOLD") {
        if (context->argument[1].type != SCRIPT_BOOL) {
            return script_false;
        }
        set_bool(TEXT_STYLE_BOLD, BOOL(1));
    }
    else if (value == "TEXTITALIC") {
        if (context->argument[1].type != SCRIPT_BOOL) {
            return script_false;
        }
        set_bool(TEXT_STYLE_ITALIC, BOOL(1));
    }
    else if (value == "TEXTFONT") {
        if (context->argument[1].type != SCRIPT_STRING) {
            return script_false;
        }
        set_str(TEXT_FONT, STR(1));
    }
    else if (value == "TEXTOVERLINE") {
        if (context->argument[1].type != SCRIPT_BOOL) {
            return script_false;
        }
        set_bool(TEXT_STYLE_OVERLINE, BOOL(1));
    }
    else if (value == "TEXTSIZE") {
        if (context->argument[1].type != SCRIPT_REAL) {
            return script_false;
        }
        set_real(TEXT_SIZE, REAL(1));
    }
    else if (value == "TEXTSTRIKEOUT") {
        if (context->argument[1].type != SCRIPT_BOOL) {
            return script_false;
        }
        set_bool(TEXT_STYLE_STRIKEOUT, BOOL(1));
    }
    else if (value == "TEXTUNDERLINE") {
        if (context->argument[1].type != SCRIPT_BOOL) {
            return script_false;
        }
        set_bool(TEXT_STYLE_UNDERLINE, BOOL(1));
    }

    return script_null;
}

/* SYSWINDOWS
 * Do nothing for click, context.
 */
ScriptValue
syswindows_command(ScriptEnv * context)
{
    prompt_output(translate("Enter an option [Cascade/Tile]: "));

    #if 0
    if (str == "C" || str == "CASCADE") {
        _main->windowCascade();
        end_command();
    }
    else if (str == "T" || str == "TILE") {
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
    start_blinking();
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
messagebox_command(ScriptEnv* context)
{
    std::string type(STR(0));

    if (type == "critical") {
        critical_box(STR(1), STR(2));
    }
    else if (type == "information") {
        information_box(STR(1), STR(2));
    }
    else if (type == "question") {
        question_box(STR(1), STR(2));
    }
    else if (type == "warning") {
        warning_box(STR(1), STR(2));
    }
    else {
        prompt_output("UNKNOWN_ERROR messageBox(): first argument must be \"critical\", \"information\", \"question\" or \"warning\".");
        return script_false;
    }

    return script_null;
}

/* . */
ScriptValue
is_int_command(ScriptEnv* context)
{
    return script_true;
}

/* . */
ScriptValue
print_area_command(ScriptEnv* context)
{
    char message[MAX_STRING_LENGTH];
    sprintf(message, "nativePrintArea(%.2f, %.2f, %.2f, %.2f)", REAL(0), REAL(1), REAL(2), REAL(3));
    debug_message(message);
    /* TODO: Print Setup Stuff
     * nativePrintArea(REAL(0), REAL(1), REAL(2), REAL(3));
     */
    print_command();
    return script_null;
}

/* . */
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
        BOOL(4), RUBBER_OFF);
    return script_null;
}

ScriptValue
add_text_single_command(ScriptEnv* context)
{
    nativeAddTextSingle(std::string(STR(0)), REAL(1), REAL(2), REAL(3),
        BOOL(4), RUBBER_OFF);
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
    nativeAddArc(REAL(0), REAL(1), REAL(2), REAL(3), REAL(4), REAL(5), RUBBER_OFF);
    return script_null;
}

ScriptValue
add_slot_command(ScriptEnv* context)
{
    nativeAddSlot(REAL(0), REAL(1), REAL(2), REAL(3), REAL(4), BOOL(5), RUBBER_OFF);
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
    EmbVector v = zero_vector;
    EmbVector start = zero_vector;
    QPainterPath path;
    foreach (QVariant var, varList) {
        if (var.canConvert(QVariant::Double)) {
            if (xCoord) {
                xCoord = false;
                v.x = var.toReal();
            }
            else {
                xCoord = true;
                v.y = -var.toReal();

                if (lineTo) {
                    path.lineTo(v.x, v.y);
                }
                else {
                    path.moveTo(v.x, v.y);
                    lineTo = true;
                    start = v;
                }
            }
        }
        else {
            prompt_output("TYPE_ERROR, addPolygon(): array contains one or more invalid elements");
            return script_false;
        }
    }

    /* Close the polygon. */
    path.closeSubpath();

    path.translate(-start.x, -start.y);

    nativeAddPolygon(start.x, start.y, path, RUBBER_OFF);
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

    nativeAddPolyline(startX, startY, path, RUBBER_OFF);
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
UndoableCommand::UndoableCommand(int type_, QString text, Object* obj, Document* v, QUndoCommand* parent) : QUndoCommand(parent)
{
    data.type = type_;
    data.gview = v;
    data.object = obj;
    setText(text);
}

/* Move */
UndoableCommand::UndoableCommand(int type_, EmbVector delta, QString text, Object* obj, Document* v, QUndoCommand* parent) : QUndoCommand(parent)
{
    data.type = type_;
    data.gview = v;
    data.object = obj;
    setText(text);
    data.delta = delta;
}

/* Rotate or scale */
UndoableCommand::UndoableCommand(int type_, EmbVector pos, double scaleFactor, QString text, Object* obj, Document* v, QUndoCommand* parent) : QUndoCommand(parent)
{
    data.type = type_;
    data.gview = v;
    data.object = obj;
    setText(text);
    if (data.type == ACTION_SCALE) {
        /* Prevent division by zero and other wacky behavior. */
        if (scaleFactor <= 0.0) {
            data.delta.x = 0.0;
            data.delta.y = 0.0;
            data.factor = 1.0;
            QMessageBox::critical(0,
                QObject::tr("ScaleFactor Error"),
                QObject::tr("Hi there. If you are not a developer, report this as a bug. "
               "If you are a developer, your code needs examined, and possibly your head too."));
        }
        else {
            /* Calculate the offset */
            EmbVector old, new_;
            old.x = data.object->x();
            old.y = data.object->y();
            QLineF scaleLine(pos.x, pos.y, old.x, old.y);
            scaleLine.setLength(scaleLine.length() * scaleFactor);
            new_.x = scaleLine.x2();
            new_.y = scaleLine.y2();

            data.delta = emb_vector_subtract(new_, old);
            data.factor = scaleFactor;
        }
    }
    else {
        data.pivot = pos;
        data.angle = scaleFactor;
    }
}

/* Navigation */
UndoableCommand::UndoableCommand(int type_, QString type_name, Document* doc, QUndoCommand* parent) : QUndoCommand(parent)
{
    data.type = type_;
    data.gview = doc;
    data.navType = type_name;
    setText(QObject::tr("Navigation"));
    data.done = false;
    //data.fromTransform = doc_transform(data.gview);
    data.fromCenter = doc_center(data.gview);
}

/* Grip Edit/Mirror */
UndoableCommand::UndoableCommand(int type_, EmbVector beforePoint, EmbVector afterPoint, QString text, Object* obj, Document* v, QUndoCommand* parent) : QUndoCommand(parent)
{
    data.type = type_;
    data.gview = v;
    data.object = obj;
    setText(text);
    if (type_ == ACTION_GRIP_EDIT) {
        setText(text);
        data.before = beforePoint;
        data.after = afterPoint;
    }
    if (type_ == ACTION_MIRROR) {
        data.mirrorLine = QLineF(beforePoint.x, beforePoint.y,
            afterPoint.x, afterPoint.y);
    }
}

/* . */
void
UndoableCommand::undo()
{
    switch (data.type) {
    case ACTION_ADD:
        doc_delete_object(data.gview, data.object);
        break;
    case ACTION_DELETE:
        doc_add_object(data.gview, data.object);
        break;
    case ACTION_MOVE:
        data.object->moveBy(-data.delta.x, -data.delta.y);
        break;
    case ACTION_ROTATE:
        rotate(data.pivot.x, data.pivot.y, -data.angle);
        break;
    case ACTION_GRIP_EDIT:
        // FIXME: data.object->gripEdit(data.after, data.before);
        break;
    case ACTION_SCALE:
        data.object->setScale(data.object->scale()*(1/data.factor));
        data.object->moveBy(-data.delta.x, -data.delta.y);
        break;
    case ACTION_NAV: {
        if (!data.done) {
            data.toTransform = data.gview->transform();
            data.toCenter = doc_center(data.gview);
            data.done = true;
        }

        data.gview->setTransform(data.fromTransform);
        doc_center_at(data.gview, data.fromCenter);
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
        doc_add_object(data.gview, data.object);
        break;
    case ACTION_DELETE:
        doc_delete_object(data.gview, data.object);
        break;
    case ACTION_MOVE:
        data.object->moveBy(data.delta.x, data.delta.y);
        break;
    case ACTION_ROTATE:
        rotate(data.pivot.x, data.pivot.y, data.angle);
        break;
    case ACTION_GRIP_EDIT:
        // FIXME: data.object->gripEdit(data.before, data.after);
        break;
    case ACTION_SCALE:
        data.object->setScale(data.object->scale() * data.factor);
        data.object->moveBy(data.delta.x, data.delta.y);
        break;
    case ACTION_NAV: {
        if (data.done) {
            data.gview->setTransform(data.toTransform);
            doc_center_at(data.gview, data.toCenter);
            break;
        }
        if (data.navType == "ZoomInToPoint") {
            QPoint p = activeScene()->property("VIEW_MOUSE_POINT").toPoint();
            doc_zoom_to_point(data.gview, to_emb_vector(p), +1);
        }
        else if (data.navType == "ZoomOutToPoint") {
            QPoint p = activeScene()->property("VIEW_MOUSE_POINT").toPoint();
            doc_zoom_to_point(data.gview, to_emb_vector(p), -1);
        }
        else if (data.navType == "ZoomExtents") {
            doc_zoom_extents(data.gview);
        }
        else if (data.navType == "ZoomSelected") {
            doc_zoom_selected(data.gview);
        }
        else if (data.navType == "PanStart") {
            /* Do Nothing. We are just recording the spot where the pan started. */
        }
        else if (data.navType == "PanStop") {
            /* Do Nothing. We are just recording the spot where the pan stopped. */
        }
        else if (data.navType == "PanLeft") {
            doc_pan_left(data.gview);
        }
        else if (data.navType == "PanRight") {
            doc_pan_right(data.gview);
        }
        else if (data.navType == "PanUp") {
            doc_pan_up(data.gview);
        }
        else if (data.navType == "PanDown") {
            doc_pan_down(data.gview);
        }
        data.toTransform = data.gview->transform();
        data.toCenter = doc_center(data.gview);
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
         /* make sure other is also an UndoableNavCommand */
         return false;
    }

    const UndoableCommand* cmd = static_cast<const UndoableCommand*>(newest);
    data.toTransform = cmd->data.toTransform;
    data.toCenter = cmd->data.toCenter;

    return true;
}

/* TODO: finish undoable mirror */
void
UndoableCommand::mirror()
{
}

/* LOCATEPOINT */
ScriptValue
locatepoint_command(ScriptEnv *context)
{
    switch (context->context) {
    case CONTEXT_MAIN: {
        prompt_output(translate("Specify point: "));
        break;
    }
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
        run_command("todo LOCATEPOINT context()", context);
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
        /*
        context->point1 = zero_vector;
        context->point2 = zero_vector;
        */
        prompt_output(translate("Specify first point: "));
        break;
    case CONTEXT_CLICK:
        /*
        if (isNaN(context->x1)) {
            context->point1 = v;
            addRubber("LINE");
            set_rubber_mode(doc, "LINE");
            set_rubber_point(doc, "LINE_START", context->point1.x, context->point1.y);
            prompt_output(translate("Specify second point: "));
        }
        else {
            context->point2 = v;
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
        if (isNaN(context->x1)) {
            if (isNaN(strList[0]) || isNaN(strList[1])) {
                alert(translate("Requires numeric distance or two points."));
                prompt_output(translate("Specify first point: "));
            }
            else {
                context->x1 = Number(strList[0]);
                context->y1 = Number(strList[1]);
                addRubber("LINE");
                setRubberMode("LINE");
                setRubberPoint("LINE_START", context->x1, context->y1);
                prompt_output(translate("Specify second point: "));
            }
        }
        else {
            if (isNaN(strList[0]) || isNaN(strList[1])) {
                alert(translate("Requires numeric distance or two points."));
                prompt_output(translate("Specify second point: "));
            }
            else {
                context->x2 = Number(strList[0]);
                context->y2 = Number(strList[1]);
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
    context->firstRun = true;
    context->base = zero_vector;
    context->dest = zero_vector;
    context->delta = zero_vector;

    if (doc_num_selected(doc) <= 0) {
        /* TODO: Prompt to select objects if nothing is preselected. */
        alert(translate("Preselect objects before invoking the move command."));
        end_command();
        information_box(translate("Move Preselect"),
            translate("Preselect objects before invoking the move command."));
    }
    else {
        prompt_output(translate("Specify base point: "));
    }
}

void
click(EmbVector v)
{
    if (context->firstRun) {
        context->firstRun = false;
        context->base = v;
        doc_add_rubber(doc, "LINE");
        doc_set_rubber_mode(doc, "LINE");
        doc_set_rubber_point(doc, "LINE_START", context->base);
        doc_preview_on(doc, "SELECTED", "MOVE", context->base, 0);
        prompt_output(translate("Specify destination point: "));
    }
    else {
        context->dest = v;
        context->delta = emb_vector_subtract(context->dest, context->base);
        doc_move_selected(context->delta);
        doc_preview_off();
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
    if (context->firstRun) {
        if (!parse_vector(str, &v)) {
            alert(translate("Invalid point."));
            prompt_output(translate("Specify base point: "));
        }
        else {
            context->firstRun = false;
            context->base = v;
            doc_add_rubber(doc, "LINE");
            doc_set_rubber_mode(doc, "LINE");
            doc_set_rubber_point(doc, "LINE_START", context->base);
            doc_preview_on(doc, "SELECTED", "MOVE", context->base, 0);
            prompt_output(translate("Specify destination point: "));
        }
    }
    else {
        if (!parse_vector(str, &v)) {
            alert(translate("Invalid point."));
            prompt_output(translate("Specify destination point: "));
        }
        else {
            context->dest = v;
            context->delta = context->dest - context->base;
            moveSelected(context->delta);
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
    context->mode = context->mode_NORMAL;
    context->firstRun = true;
    context->base = zero_vector;
    context->dest = zero_vector;
    context->factor = NaN;

    context->baseR = zero_vector;
    context->destR = zero_vector;
    context->factorRef = NaN;
    context->factorNew = NaN;

    if (gview->numSelected() <= 0) {
        /* TODO: Prompt to select objects if nothing is preselected */
        alert(translate("Preselect objects before invoking the scale command."));
        end_command();
        information_box(translate("Scale Preselect"), translate("Preselect objects before invoking the scale command."));
    }
    else {
        prompt_output(translate("Specify base point: "));
    }
}

void
click(EmbVector position)
{
    if (context->mode == context->mode_NORMAL) {
        if (context->firstRun) {
            context->firstRun = false;
            context->base = position;
            addRubber("LINE");
            setRubberMode("LINE");
            setRubberPoint("LINE_START", context->base);
            previewOn("SELECTED", "SCALE", context->base, 1);
            prompt_output(translate("Specify scale factor or [Reference]: "));
        }
        else {
            context->dest = position;
            context->factor = calculateDistance(context->base, context->dest);
            doc_scale_selected(doc, context->base, context->factor);
            previewOff();
            end_command();
        }
    }
    else if (context->mode == context->mode_REFERENCE) {
        if (isNaN(context->baseRX)) {
            context->baseR = position;
            addRubber("LINE");
            setRubberMode("LINE");
            setRubberPoint("LINE_START", context->baseR);
            prompt_output(translate("Specify second point: "));
        }
        else if (isNaN(context->destRX)) {
            context->destR = position;
            context->factorRef = emb_vector_distance(context->baseR, context->destR);
            if (context->factorRef <= 0.0) {
                context->destR = zero_vector;
                context->factorRef = NaN;
                alert(translate("Value must be positive and nonzero."));
                prompt_output(translate("Specify second point: "));
            }
            else {
                setRubberPoint("LINE_START", context->base);
                previewOn("SELECTED", "SCALE", context->base, context->factorRef);
                prompt_output(translate("Specify new length: "));
            }
        }
        else if (isNaN(context->factorNew)) {
            context->factorNew = calculateDistance(context->base, x, y);
            if (context->factorNew <= 0.0) {
                context->factorNew = NaN;
                alert(translate("Value must be positive and nonzero."));
                prompt_output(translate("Specify new length: "));
            }
            else {
                doc_scale_selected(doc, context->base, context->factorNew/context->factorRef);
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
    switch (context->mode) {
    default:
    case SCALE_MODE_NORMAL: {
        if (context->firstRun) {
            if (!parse_vector(str, &v)) {
                alert(translate("Invalid point."));
                prompt_output(translate("Specify base point: "));
            }
            else {
                context->firstRun = false;
                context->base = v;
                addRubber("LINE");
                setRubberMode("LINE");
                setRubberPoint("LINE_START", context->base);
                previewOn("SELECTED", "SCALE", context->base, 1);
                prompt_output(translate("Specify scale factor or [Reference]: "));
            }
        }
        else {
            if (str == "R" || str == "REFERENCE") {
                /* TODO: Probably should add additional translate calls here. */
                context->mode = context->mode_REFERENCE;
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
                    context->factor = Number(str);
                    scaleSelected(context->base, context->factor);
                    previewOff();
                    end_command();
                }
            }
        }
        break;
    }
    case SCALE_MODE_REFERENCE: {
        if (isNaN(context->baseRX)) {
            if (isNaN(str)) {
                if (!parse_vector(str, &v)) {
                    alert(translate("Requires valid numeric distance or two points."));
                    prompt_output(translate("Specify reference length") + " {1}: ");
                }
                else {
                    context->baseR = v;
                    addRubber("LINE");
                    setRubberMode("LINE");
                    setRubberPoint("LINE_START", context->baseR);
                    prompt_output(translate("Specify second point: "));
                }
            }
            else {
                /* The base and dest values are only set here to advance the command. */
                context->baseR = zero_vector;
                context->destR = zero_vector;
                /* The reference length is what we will use later. */
                context->factorRef = Number(str);
                if (context->factorRef <= 0.0) {
                    context->factorRef = NaN;
                    alert(translate("Value must be positive and nonzero."));
                    prompt_output(translate("Specify reference length") + " {1}: ");
                }
                else {
                    addRubber("LINE");
                    setRubberMode("LINE");
                    setRubberPoint("LINE_START", context->base);
                    previewOn("SELECTED", "SCALE", context->base, context->factorRef);
                    prompt_output(translate("Specify new length: "));
                }
            }
        }
        else if (isNaN(context->destRX)) {
            if (isNaN(str)) {
                if (!parse_vector(str, &v)) {
                    alert(translate("Requires valid numeric distance or two points."));
                    prompt_output(translate("Specify second point: "));
                }
                else {
                    context->destR = v;
                    context->factorRef = calculateDistance(context->baseR, context->destR);
                    if (context->factorRef <= 0.0) {
                        context->destR = zero_vector;
                        context->factorRef = NaN;
                        alert(translate("Value must be positive and nonzero."));
                        prompt_output(translate("Specify second point: "));
                    }
                    else {
                        setRubberPoint("LINE_START", context->base);
                        previewOn("SELECTED", "SCALE", context->base, context->factorRef);
                        prompt_output(translate("Specify new length: "));
                    }
                }
            }
            else {
                /* The base and dest values are only set here to advance the command. */
                context->baseR = zero_vector;
                context->destR = zero_vector;
                /* The reference length is what we will use later. */
                context->factorRef = Number(str);
                if (context->factorRef <= 0.0) {
                    context->destR = zero_vector;
                    context->factorRef = NaN;
                    alert(translate("Value must be positive and nonzero."));
                    prompt_output(translate("Specify second point: "));
                }
                else {
                    setRubberPoint("LINE_START", context->base);
                    previewOn("SELECTED", "SCALE", context->base, context->factorRef);
                    prompt_output(translate("Specify new length: "));
                }
            }
        }
        else if (isNaN(context->factorNew)) {
            if (isNaN(str)) {
                if (!parse_vector(str, &v)) {
                    alert(translate("Requires valid numeric distance or second point."));
                    prompt_output(translate("Specify new length: "));
                }
                else {
                    context->factorNew = embVector(context->base, v);
                    if (context->factorNew <= 0.0) {
                        context->factorNew = NaN;
                        alert(translate("Value must be positive and nonzero."));
                        prompt_output(translate("Specify new length: "));
                    }
                    else {
                        scaleSelected(context->base, context->factorNew/context->factorRef);
                        previewOff();
                        end_command();
                    }
                }
            }
            else {
                context->factorNew = Number(str);
                if (context->factorNew <= 0.0) {
                    context->factorNew = NaN;
                    alert(translate("Value must be positive and nonzero."));
                    prompt_output(translate("Specify new length: "));
                }
                else {
                    scaleSelected(context->base, context->factorNew/context->factorRef);
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
        /* Report number of pre-selected objects. */
        char msg[200];
        /* sprintf(msg, "Number of Objects Selected: %d", doc->numSelected()); */
        /* prompt_output(msg); */
        /* mirrorSelected(0,0,0,1); */
    
        /* doc_select_all(doc); */
        /* rotateSelected(0,0,90); */

        /* Polyline & Polygon Testing */
    
        EmbVector offset;
        offset.x = 0.0;
        offset.y = 0.0;

        /*    
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
    context->mode = context->mode_NORMAL;
    context->firstRun = true;
    context->base = zero_vector;
    context->dest = zero_vector;
    context->angle = 0.0f;

    context->baseR = zero_vector;
    context->destR = zero_vector;
    context->angleRef = 0.0f;
    context->angleNew = 0.0f;

    if (gview->numSelected() <= 0) {
        /* TODO: Prompt to select objects if nothing is preselected. */
        alert(translate("Preselect objects before invoking the rotate command."));
        end_command();
        information_box(translate("Rotate Preselect"),
            translate("Preselect objects before invoking the rotate command."));
    }
    else {
        prompt_output(translate("Specify base point: "));
    }
}

void
click(EmbVector v)
{
    if (context->mode == context->mode_NORMAL) {
        if (context->firstRun) {
            context->firstRun = false;
            context->base = v;
            addRubber("LINE");
            setRubberMode("LINE");
            setRubberPoint("LINE_START", context->base);
            previewOn("SELECTED", "ROTATE", context->base, 0);
            prompt_output(translate("Specify rotation angle or [Reference]: "));
        }
        else {
            context->dest = v;
            context->angle = emb_vector_angle(context->base, context->dest);
            rotateSelected(context->base, context->angle);
            previewOff();
            end_command();
        }
    }
    else if (context->mode == context->mode_REFERENCE) {
        if (isNaN(context->baseRX)) {
            context->baseR = v;
            addRubber("LINE");
            setRubberMode("LINE");
            setRubberPoint("LINE_START", context->baseR);
            prompt_output(translate("Specify second point: "));
        }
        else if (isNaN(context->destRX)) {
            context->destR = v;
            context->angleRef = emb_vector_angle(context->baseR, context->destR);
            setRubberPoint("LINE_START", context->base);
            previewOn("SELECTED", "ROTATE", context->base, context->angleRef);
            prompt_output(translate("Specify the new angle: "));
        }
        else if (isNaN(context->angleNew)) {
            context->angleNew = calculateAngle(context->base, v);
            rotateSelected(context->base, context->angleNew - context->angleRef);
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
    if (context->mode == context->mode_NORMAL) {
        if (context->firstRun) {
            if (!parse_vector(str, &v)) {
                alert(translate("Invalid point."));
                prompt_output(translate("Specify base point: "));
            }
            else {
                context->firstRun = false;
                context->base = v;
                addRubber("LINE");
                setRubberMode("LINE");
                setRubberPoint("LINE_START", context->base);
                previewOn("SELECTED", "ROTATE", context->base, 0);
                prompt_output(translate("Specify rotation angle or [Reference]: "));
            }
        }
        else {
            if (str == "R" || str == "REFERENCE") {
                context->mode = context->mode_REFERENCE;
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
                    context->angle = Number(str);
                    rotateSelected(context->base, context->angle);
                    previewOff();
                    end_command();
                }
            }
        }
    }
    else if (context->mode == context->mode_REFERENCE) {
        if (isNaN(context->baseRX)) {
            if (isNaN(str)) {
                if (!parse_vector(str, &v)) {
                    alert(translate("Requires valid numeric angle or two points."));
                    prompt_output(translate("Specify the reference angle") + " {0.00}: ");
                }
                else {
                    context->baseR = v;
                    addRubber("LINE");
                    setRubberMode("LINE");
                    setRubberPoint("LINE_START", context->baseR);
                    prompt_output(translate("Specify second point: "));
                }
            }
            else {
                /* The base and dest values are only set here to advance the command. */
                context->baseR = zero_vector;
                context->destR = zero_vector;
                /* The reference angle is what we will use later. */
                context->angleRef = Number(str);
                addRubber("LINE");
                setRubberMode("LINE");
                setRubberPoint("LINE_START", context->base);
                previewOn("SELECTED", "ROTATE", context->base, context->angleRef);
                prompt_output(translate("Specify the new angle: "));
            }
        }
        else if (isNaN(context->destRX)) {
            if (isNaN(str)) {
                if (!parse_vector(str, &v)) {
                    alert(translate("Requires valid numeric angle or two points."));
                    prompt_output(translate("Specify second point: "));
                }
                else {
                    context->destR = v;
                    context->angleRef = calculateAngle(context->baseR, context->destR);
                    previewOn("SELECTED", "ROTATE", context->base, context->angleRef);
                    setRubberPoint("LINE_START", context->base);
                    prompt_output(translate("Specify the new angle: "));
                }
            }
            else {
                /* The base and dest values are only set here to advance the command. */
                context->baseR = zero_vector;
                context->destR = zero_vector;
                /* The reference angle is what we will use later. */
                context->angleRef = Number(str);
                previewOn("SELECTED", "ROTATE", context->base, context->angleRef);
                prompt_output(translate("Specify the new angle: "));
            }
        }
        else if (isNaN(context->angleNew)) {
            if (isNaN(str)) {
                if (!parse_vector(str, &v)) {
                    alert(translate("Requires valid numeric angle or second point."));
                    prompt_output(translate("Specify the new angle: "));
                }
                else {
                    context->angleNew = emb_vector_angle(context->base, v);
                    rotateSelected(context->base.x, context->base.y,
                        context->angleNew - context->angleRef);
                    previewOff();
                    end_command();
                }
            }
            else {
                context->angleNew = Number(str);
                rotateSelected(context->base, context->angleNew - context->angleRef);
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
    case CONTEXT_MAIN: {
        context->mode = RGB_BACKGROUND;
        prompt_output(translate("Enter RED,GREEN,BLUE values for background or [Crosshair/Grid]: "));
        break;
    }
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
    switch (context->mode) {
    default:
    case RGB_BACKGROUND: {
        if (str == "C" || str == "CROSSHAIR") {
            context->mode = RGB_CROSSHAIR;
            prompt_output(translate("Specify crosshair color: "));
        }
        else if (str == "G" || str == "GRID") {
            context->mode = RGB_GRID;
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
    case RGB_CROSSHAIR: {
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
    case RGB_GRID: {
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
        nativeSetRubberMode(RUBBER_CIRCLE_1P_RAD);
    }
    else if (mode == "CIRCLE_1P_DIA") {
        nativeSetRubberMode(RUBBER_CIRCLE_1P_DIA);
    }
    else if (mode == "CIRCLE_2P") {
        nativeSetRubberMode(RUBBER_CIRCLE_2P);
    }
    else if (mode == "CIRCLE_3P") {
        nativeSetRubberMode(RUBBER_CIRCLE_3P);
    }
    else if (mode == "CIRCLE_TTR") {
        nativeSetRubberMode(RUBBER_CIRCLE_TTR);
    }
    else if (mode == "CIRCLE_TTT") {
        nativeSetRubberMode(RUBBER_CIRCLE_TTT);
    }
    else if (mode == "DIMLEADER_LINE") {
        nativeSetRubberMode(RUBBER_DIMLEADER_LINE);
    }
    else if (mode == "ELLIPSE_LINE") {
        nativeSetRubberMode(RUBBER_ELLIPSE_LINE);
    }
    else if (mode == "ELLIPSE_MAJDIA_MINRAD") {
        nativeSetRubberMode(RUBBER_ELLIPSE_MAJDIA_MINRAD);
    }
    else if (mode == "ELLIPSE_MAJRAD_MINRAD") {
        nativeSetRubberMode(RUBBER_ELLIPSE_MAJRAD_MINRAD);
    }
    else if (mode == "ELLIPSE_ROTATION") {
        nativeSetRubberMode(RUBBER_ELLIPSE_ROTATION);
    }
    else if (mode == "LINE") {
        nativeSetRubberMode(RUBBER_LINE);
    }
    else if (mode == "POLYGON") {
        nativeSetRubberMode(RUBBER_POLYGON);
    }
    else if (mode == "POLYGON_INSCRIBE") {
        nativeSetRubberMode(RUBBER_POLYGON_INSCRIBE);
    }
    else if (mode == "POLYGON_CIRCUMSCRIBE") {
        nativeSetRubberMode(RUBBER_POLYGON_CIRCUMSCRIBE);
    }
    else if (mode == "POLYLINE") {
        nativeSetRubberMode(RUBBER_POLYLINE);
    }
    else if (mode == "RECTANGLE") {
        nativeSetRubberMode(RUBBER_RECTANGLE);
    }
    else if (mode == "TEXTSINGLE") {
        nativeSetRubberMode(RUBBER_TEXTSINGLE);
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
    double mx = run_command("get mousex", context).r;
    double my = run_command("get mousey", context).r;

    if (objType == "ARC") {
        /* TODO: handle this type */
    }
    else if (objType == "BLOCK") {
        /* TODO: handle this type */
    }
    else if (objType == "CIRCLE") {
        nativeAddCircle(mx, my, 0, false, RUBBER_ON);
    }
    else if (objType == "DIMALIGNED") {
        /* TODO: handle this type */
    }
    else if (objType == "DIMANGULAR") {
        /* TODO: handle this type */
    }
    else if (objType == "DIMARCLENGTH") {
        /* TODO: handle this type */
    }
    else if (objType == "DIMDIAMETER") {
        /* TODO: handle this type */
    }
    else if (objType == "DIMLEADER") {
        nativeAddDimLeader(mx, my, mx, my, 0, RUBBER_ON);
    }
    else if (objType == "DIMLINEAR") {
        /* TODO: handle this type */
    }
    else if (objType == "DIMORDINATE") {
        /* TODO: handle this type */
    }
    else if (objType == "DIMRADIUS") {
        /* TODO: handle this type */
    }
    else if (objType == "ELLIPSE") {
        nativeAddEllipse(mx, my, 0, 0, 0, 0, RUBBER_ON);
    }
    else if (objType == "ELLIPSEARC") {
        /* TODO: handle this type */
    }
    else if (objType == "HATCH") {
        /* TODO: handle this type */
    }
    else if (objType == "IMAGE") {
        /* TODO: handle this type */
    }
    else if (objType == "INFINITELINE") {
        /* TODO: handle this type */
    }
    else if (objType == "LINE") {
        nativeAddLine(mx, my, mx, my, 0, RUBBER_ON);
    }
    else if (objType == "PATH") {
        /* TODO: handle this type */
    }
    else if (objType == "POINT") {
        /* TODO: handle this type */
    }
    else if (objType == "POLYGON") {
        nativeAddPolygon(mx, my, QPainterPath(), RUBBER_ON);
    }
    else if (objType == "POLYLINE") {
        nativeAddPolyline(mx, my, QPainterPath(), RUBBER_ON);
    }
    else if (objType == "RAY") {
        /* TODO: handle this type */
    }
    else if (objType == "RECTANGLE") {
        nativeAddRectangle(mx, my, mx, my, 0, 0, RUBBER_ON);
    }
    else if (objType == "SPLINE") {
        /* TODO: handle this type */
    }
    else if (objType == "TEXTMULTI") {
        /* TODO: handle this type */
    }
    else if (objType == "TEXTSINGLE") {
        nativeAddTextSingle("", mx, my, 0, false, RUBBER_ON);
    }

    return script_null;
}

ScriptValue
clear_rubber_command(ScriptEnv* context)
{
    Document* doc = activeDocument();
    if (doc) {
        doc_clear_rubber_room(doc);
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
