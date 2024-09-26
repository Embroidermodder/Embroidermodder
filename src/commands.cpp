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

/* Could initialise all documents to NULL rather than having a seperate memory
 * usage array?
 */
Document *documents[MAX_OPEN_FILES];
bool document_memory[MAX_OPEN_FILES];

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
activeMdiWindow(void)
{
    debug_message("activeMdiWindow()");
    MdiWindow* mdiWin = qobject_cast<MdiWindow*>(mdiArea->activeSubWindow());
    return mdiWin;
}

/* . */
int32_t
activeDocument(void)
{
    debug_message("activeDocument()");
    MdiWindow* mdiWin = activeMdiWindow();
    if (mdiWin) {
        return mdiWin->doc_index;
    }
    return -1;
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
    //FIXME
    return NULL;
    int32_t doc_index = activeDocument();
    if (doc_index >= 0) {
        QUndoStack* u = documents[doc_index]->undoStack;
        return u;
    }
    return 0;
}

/* . */
void
whats_this_mode(void)
{
    debug_message("whatsThisContextHelp()");
    QWhatsThis::enterWhatsThisMode();
}

void
window_close_all(void)
{
    mdiArea->closeAllSubWindows();
}

void
window_cascade(void)
{
    mdiArea->cascade();
}

void
window_tile(void)
{
    mdiArea->tile();
}

void
window_next(void)
{
    mdiArea->activateNextSubWindow();
}

void
window_previous(void)
{
    mdiArea->activatePreviousSubWindow();
}

void
disable_rapid_fire(void)
{
    prompt->disableRapidFire();
}

void
enable_rapid_fire(void)
{
    prompt->enableRapidFire();
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

/* . */
void
layerManager(void)
{
    debug_message("layerManager()");
    todo("Implement layerManager.");
    LayerManager layman(_main, _main);
    layman.exec();
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
 * NOTE: Replace any special characters that will cause a syntax error
 */
void
runCommandPrompt(const char *cmd)
{
    EmbString message;
    ScriptEnv *context = create_script_env();
    sprintf(message, "runCommandPrompt(%s)", cmd);
    debug_message(message);
    context->context = CONTEXT_PROMPT;
    if (prompt->isRapidFireEnabled()) {
        run_command(cmd, context);
    }
    else {
        /* Both branches run the same. */
        run_command(cmd, context);
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
uint32_t
rgb(uint8_t r, uint8_t g, uint8_t b)
{
    return qRgb(r, g, b);
}

/* . */
void
nativeAddTextSingle(char *str, double x, double y, double rot, bool fill, int rubberMode)
{
    int32_t doc_index = activeDocument();
    QGraphicsScene* gscene = activeScene();
    QUndoStack* stack = activeUndoStack();
    /*
    char *str = STR(0);
    EmbVector v = unpack_vector(context, 1);
    double rot = REAL(3);
    bool fill = BOOL(4);
    int rubberMode = INT(5);
     */
    if ((doc_index >= 0) && gscene && stack) {
        EmbVector v = emb_vector(x, -y);
        Object* obj = create_text_single(QString(str), v, getCurrentColor());
        obj_set_text_font(obj->core, get_str(TEXT_FONT));
        obj_set_text_size(obj->core, get_real(TEXT_SIZE));
        obj_set_text_style(obj->core,
            get_bool(TEXT_STYLE_BOLD),
            get_bool(TEXT_STYLE_ITALIC),
            get_bool(TEXT_STYLE_UNDERLINE),
            get_bool(TEXT_STYLE_STRIKEOUT),
            get_bool(TEXT_STYLE_OVERLINE));
        obj_set_text_backward(obj->core, false);
        obj_set_text_upside_down(obj->core, false);
        obj->setRotation(-rot);
        /* TODO: single line text fill. */
        obj_set_rubber_mode(obj->core, rubberMode);
        if (rubberMode) {
            doc_add_to_rubber_room(doc_index, obj);
            gscene->addItem(obj);
            gscene->update();
        }
        else {
            UndoableCommand* cmd = new UndoableCommand(ACTION_ADD, obj->core->OBJ_NAME, obj, doc_index, 0);
            stack->push(cmd);
        }
    }
}

/* . */
void
nativeAddLine(double x1, double y1, double x2, double y2, double rot, int rubberMode)
{
    int32_t doc_index = activeDocument();
    QGraphicsScene* gscene = activeScene();
    QUndoStack* stack = activeUndoStack();
    if ((doc_index >= 0) && gscene && stack) {
        EmbLine line;
        line.start = emb_vector(x1, -y1);
        line.end = emb_vector(x2, -y2);
        Object* obj = create_line(line, getCurrentColor());
        obj->setRotation(-rot);
        obj_set_rubber_mode(obj->core, rubberMode);
        if (rubberMode) {
            doc_add_to_rubber_room(doc_index, obj);
            gscene->addItem(obj);
            doc_update(doc_index);
        }
        else {
            UndoableCommand* cmd = new UndoableCommand(ACTION_ADD, obj->core->OBJ_NAME, obj, doc_index, 0);
            stack->push(cmd);
        }
    }
}

/* . */
void
nativeAddRectangle(double x, double y, double w, double h, double rot, bool fill, int rubberMode)
{
    int32_t doc_index = activeDocument();
    QGraphicsScene* gscene = activeScene();
    QUndoStack* stack = activeUndoStack();
    if ((doc_index < 0) && !(gscene && stack)) {
        return;
    }
    EmbRect rect = emb_rect(x, -y, w, -h);
    Object* obj = create_rect(rect, getCurrentColor());
    obj->setRotation(-rot);
    obj_set_rubber_mode(obj->core, rubberMode);
    /* TODO: rect fill */
    if (rubberMode) {
        doc_add_to_rubber_room(doc_index, obj);
        gscene->addItem(obj);
        doc_update(doc_index);
    }
    else {
        UndoableCommand* cmd = new UndoableCommand(ACTION_ADD, obj->core->OBJ_NAME, obj, doc_index, 0);
        stack->push(cmd);
    }
}


void
nativeAddArc(double x1, double y1, double x2, double y2, double x3, double y3, int rubberMode)
{
    int32_t doc_index = activeDocument();
    QGraphicsScene* scene = activeScene();
    if ((doc_index >= 0) && scene) {
        EmbArc arc;
        arc.start = emb_vector(x1, -y1);
        arc.mid = emb_vector(x2, -y2);
        arc.end = emb_vector(x3, -y3);
        Object* arcObj = create_arc(arc, getCurrentColor());
        obj_set_rubber_mode(arcObj->core, rubberMode);
        if (rubberMode) {
            doc_add_to_rubber_room(doc_index, arcObj);
        }
        scene->addItem(arcObj);
        doc_update(doc_index);
    }
}

void
nativeAddCircle(double centerX, double centerY, double radius, bool fill, int rubberMode)
{
    int32_t doc_index = activeDocument();
    QGraphicsScene* gscene = activeScene();
    QUndoStack* stack = activeUndoStack();
    if ((doc_index >= 0) && gscene && stack) {
        EmbCircle circle;
        circle.center = emb_vector(centerX, -centerY);
        circle.radius = radius;
        Object* obj = create_circle(circle, getCurrentColor());
        obj_set_rubber_mode(obj->core, rubberMode);
        /* TODO: circle fill. */
        if (rubberMode) {
            doc_add_to_rubber_room(doc_index, obj);
            gscene->addItem(obj);
            doc_update(doc_index);
        }
        else {
            UndoableCommand* cmd = new UndoableCommand(ACTION_ADD, obj->core->OBJ_NAME, obj, doc_index, 0);
            stack->push(cmd);
        }
    }
}

void
nativeAddSlot(double centerX, double centerY, double diameter, double length, double rot, bool fill, int rubberMode)
{
    /* TODO: Use UndoableCommand for slots */
    int32_t doc_index = activeDocument();
    /*
    Object* slotObj = new Object(centerX, -centerY, diameter, length, getCurrentColor());
    slotObj->setRotation(-rot);
    obj_set_rubber_mode(slotObj->core, rubberMode);
    if (rubberMode) doc_add_to_rubber_room(doc_index, slotObj);
    scene->addItem(slotObj);
    */
    /* TODO: slot fill */
    doc_update(doc_index);
}

void
nativeAddEllipse(double centerX, double centerY, double width, double height, double rot, bool fill, int rubberMode)
{
    int32_t doc_index = activeDocument();
    QGraphicsScene* gscene = activeScene();
    QUndoStack* stack = activeUndoStack();
    if ((doc_index >= 0) && gscene && stack) {
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
            doc_add_to_rubber_room(doc_index, obj);
            gscene->addItem(obj);
            gscene->update();
        }
        else {
            UndoableCommand* cmd = new UndoableCommand(ACTION_ADD, obj->core->OBJ_NAME, obj, doc_index, 0);
            stack->push(cmd);
        }
    }
}

void
nativeAddPoint(double x, double y)
{
    int32_t doc_index = activeDocument();
    QUndoStack* stack = activeUndoStack();
    if ((doc_index >= 0) && stack) {
        EmbPoint point;
        point.position.x = x;
        point.position.y = -y;
        Object* obj = create_point(point, getCurrentColor());
        UndoableCommand* cmd = new UndoableCommand(ACTION_ADD, obj->core->OBJ_NAME, obj, doc_index, 0);
        stack->push(cmd);
    }
}

/* NOTE: This native is different than the rest in that the Y+ is down
 * (scripters need not worry about this)
 */
void
nativeAddPolygon(double startX, double startY, const QPainterPath& p, int rubberMode)
{
    int32_t doc_index = activeDocument();
    QGraphicsScene* gscene = activeScene();
    QUndoStack* stack = activeUndoStack();
    if ((doc_index >= 0) && gscene && stack) {
        EmbPolygon polygon;
        EmbVector start = emb_vector(startX, startY);
        Object* obj = create_polygon(start, p, getCurrentColor());
        obj_set_rubber_mode(obj->core, rubberMode);
        if (rubberMode) {
            doc_add_to_rubber_room(doc_index, obj);
            gscene->addItem(obj);
            doc_update(doc_index);
        }
        else {
            UndoableCommand* cmd = new UndoableCommand(ACTION_ADD, obj->core->OBJ_NAME, obj, doc_index, 0);
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
    int32_t doc_index = activeDocument();
    QGraphicsScene* gscene = activeScene();
    QUndoStack* stack = activeUndoStack();
    if ((doc_index >= 0) && gscene && stack) {
        EmbPath path;
        EmbVector start = emb_vector(startX, startY);
        Object* obj = create_polygon(start, p, getCurrentColor());
        obj_set_rubber_mode(obj->core, rubberMode);
        if (rubberMode) {
            doc_add_to_rubber_room(doc_index, obj);
            gscene->addItem(obj);
            doc_update(doc_index);
        }
        else {
            UndoableCommand* cmd = new UndoableCommand(ACTION_ADD,
                obj->core->OBJ_NAME, obj, doc_index, 0);
            stack->push(cmd);
        }
    }
}

void
nativeAddDimLeader(double x1, double y1, double x2, double y2, double rot, int rubberMode)
{
    int32_t doc_index = activeDocument();
    QGraphicsScene* gscene = activeScene();
    QUndoStack* stack = activeUndoStack();
    if ((doc_index >= 0) && gscene && stack) {
        EmbLine line;
        line.start = emb_vector(x1, -y1);
        line.end = emb_vector(x2, -y2);
        Object* obj = create_dim_leader(line, getCurrentColor());
        obj->setRotation(-rot);
        obj_set_rubber_mode(obj->core, rubberMode);
        if (rubberMode) {
            doc_add_to_rubber_room(doc_index, obj);
            gscene->addItem(obj);
            doc_update(doc_index);
        }
        else {
            UndoableCommand* cmd = new UndoableCommand(ACTION_ADD,
                obj->core->OBJ_NAME, obj, doc_index, 0);
            stack->push(cmd);
        }
    }
}

/* . */
void
nativeSetCursorShape(char shape[MAX_STRING_LENGTH])
{
    int32_t doc_index = activeDocument();
    Document *doc = documents[doc_index];
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

/* Compatibility layer for C files */
void
prompt_output(const EmbString txt)
{
    appendHistory((char*)txt);
}

/* . */
void
end_command(void)
{
    int32_t doc_index = activeDocument();
    if (doc_index >= 0) {
        doc_clear_rubber_room(doc_index);
        doc_preview_off(doc_index);
        doc_disable_move_rapid_fire(doc_index);
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

    int32_t doc_index = activeDocument();
    if (doc_index) {
        doc_preview_on(doc_index, clone, mode, REAL(2), -REAL(3), REAL(4));
    }
    else {
        prompt_output("Preview on requires an active view.");
    }
    return script_null;
}

/* . */
ScriptValue
set_prompt_prefix_command(ScriptEnv* context)
{
    prompt->setPrefix(QString(STR(0)));
    return script_null;
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

/*
 * Undo
 */
UndoableCommand::UndoableCommand(int type_, QString text, Object* obj, int32_t doc,
    QUndoCommand* parent) : QUndoCommand(parent)
{
    data.type = type_;
    data.doc = doc;
    object = obj;
    setText(text);
}

/* Move */
UndoableCommand::UndoableCommand(int type_, EmbVector delta, QString text,
    Object* obj, int32_t doc, QUndoCommand* parent) : QUndoCommand(parent)
{
    data.type = type_;
    data.doc = doc;
    object = obj;
    setText(text);
    data.delta = delta;
}

/* Rotate or scale */
UndoableCommand::UndoableCommand(int type_, EmbVector pos, double scaleFactor,
    QString text, Object* obj, int32_t doc, QUndoCommand* parent) : QUndoCommand(parent)
{
    data.type = type_;
    data.doc = doc;
    object = obj;
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
            old.x = object->x();
            old.y = object->y();
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
UndoableCommand::UndoableCommand(int type_, QString type_name, int32_t doc,
    QUndoCommand* parent) : QUndoCommand(parent)
{
    data.type = type_;
    data.doc = doc;
    string_copy(data.navType, qPrintable(type_name));
    setText(QObject::tr("Navigation"));
    data.done = false;
    // fromTransform = doc_transform(data.doc);
    data.fromCenter = doc_center(data.doc);
}

/* Grip Edit/Mirror */
UndoableCommand::UndoableCommand(int type_, EmbVector beforePoint,
    EmbVector afterPoint, QString text, Object* obj, int32_t doc,
    QUndoCommand* parent) : QUndoCommand(parent)
{
    data.type = type_;
    data.doc = doc;
    object = obj;
    setText(text);
    if (type_ == ACTION_GRIP_EDIT) {
        setText(text);
        data.before = beforePoint;
        data.after = afterPoint;
    }
    if (type_ == ACTION_MIRROR) {
        data.mirrorLine.start = beforePoint;
        data.mirrorLine.end = afterPoint;
    }
}

/* . */
void
UndoableCommand::undo()
{
    switch (data.type) {
    case ACTION_ADD:
        doc_delete_object(data.doc, object);
        break;
    case ACTION_DELETE:
        doc_add_object(data.doc, object);
        break;
    case ACTION_MOVE:
        object->moveBy(-data.delta.x, -data.delta.y);
        break;
    case ACTION_ROTATE:
        rotate(data.pivot.x, data.pivot.y, -data.angle);
        break;
    case ACTION_GRIP_EDIT:
        // FIXME: object->gripEdit(data.after, data.before);
        break;
    case ACTION_SCALE:
        object->setScale(object->scale()*(1/data.factor));
        object->moveBy(-data.delta.x, -data.delta.y);
        break;
    case ACTION_NAV: {
        if (!data.done) {
            toTransform = documents[data.doc]->transform();
            data.toCenter = doc_center(data.doc);
            data.done = true;
        }

        documents[data.doc]->setTransform(fromTransform);
        doc_center_at(data.doc, data.fromCenter);
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
        doc_add_object(data.doc, object);
        break;
    case ACTION_DELETE:
        doc_delete_object(data.doc, object);
        break;
    case ACTION_MOVE:
        object->moveBy(data.delta.x, data.delta.y);
        break;
    case ACTION_ROTATE:
        rotate(data.pivot.x, data.pivot.y, data.angle);
        break;
    case ACTION_GRIP_EDIT:
        // FIXME: object->gripEdit(data.before, data.after);
        break;
    case ACTION_SCALE:
        object->setScale(object->scale() * data.factor);
        object->moveBy(data.delta.x, data.delta.y);
        break;
    case ACTION_NAV: {
        if (data.done) {
            documents[data.doc]->setTransform(toTransform);
            doc_center_at(data.doc, data.toCenter);
            break;
        }
        DocumentData *d = doc_data(data.doc);
        if (string_equal(data.navType, "ZoomInToPoint")) {
            doc_zoom_to_point(data.doc, d->viewMousePoint, +1);
        }
        else if (string_equal(data.navType, "ZoomOutToPoint")) {
            doc_zoom_to_point(data.doc, d->viewMousePoint, -1);
        }
        else if (string_equal(data.navType, "ZoomExtents")) {
            doc_zoom_extents(data.doc);
        }
        else if (string_equal(data.navType, "ZoomSelected")) {
            doc_zoom_selected(data.doc);
        }
        else if (string_equal(data.navType, "PanStart")) {
            /* Do Nothing. We are just recording the spot where the pan started. */
        }
        else if (string_equal(data.navType, "PanStop")) {
            /* Do Nothing. We are just recording the spot where the pan stopped. */
        }
        else if (string_equal(data.navType, "PanLeft")) {
            doc_pan_left(data.doc);
        }
        else if (string_equal(data.navType, "PanRight")) {
            doc_pan_right(data.doc);
        }
        else if (string_equal(data.navType, "PanUp")) {
            doc_pan_up(data.doc);
        }
        else if (string_equal(data.navType, "PanDown")) {
            doc_pan_down(data.doc);
        }
        toTransform = documents[data.doc]->transform();
        data.toCenter = doc_center(data.doc);
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
    EmbVector rotv;
    EmbVector p = to_emb_vector(object->scenePos());
    p.x -= x;
    p.y -= y;
    rotv.x = p.x*cosRot - p.y*sinRot;
    rotv.y = p.x*sinRot + p.y*cosRot;
    rotv.x += x;
    rotv.y += y;

    object->setPos(rotv.x, rotv.y);
    object->setRotation(object->rotation() + rot);
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
    toTransform = cmd->toTransform;
    data.toCenter = cmd->data.toCenter;

    return true;
}

/* TODO: finish undoable mirror */
void
UndoableCommand::mirror()
{
}

