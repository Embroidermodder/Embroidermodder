/*!
 * \file script.cpp
 * \brief Lua support for Embroidermodder.
 *
 * Having scripting available for users to alter their software reflects a
 * core principle of open source software. The full build environment for
 * Embroidermodder is too complex for most users to set up, especially on
 * Windows. So the "freedom" to alter the program would be seldom used.
 * Being able to write a script in any text editor and add it to
 * the build by adding a load call to "boot.lua" is therefore a core
 * feature. We as core developers won't be using this flexibility much.
 *
 * While relying more on lua could make some core developers' work easier
 * like loading settings or writing out the state for debugging (see
 * `report_state_f`), going back and forth will be slower,
 * harder to debug and potentially will lead to invalid memory more often.
 * Having every `settings_` variable part of the global C/C++ state of the
 * program rather than as lua symbols has faster load times and easier to
 * access for over 99% of the code and run-time. So we recommend that core
 * developers make sure they know why a feature should be written as a lua
 * function before submitting a patch that requires this.
 *
 * @todo convert more lua registerables to core commands and keep cmd_f as
 *       the interface for scripts to these commands
 *
 * @todo check that the actions appropriately clear selections and rubber modes
 *
 * @todo functioning lua GUI scripting
 */

#include "embroidermodder.h"

/* Note that lua does not have interfacing outside of this file.
 * The wrappers like script_env_boot, script_env_free, etc. allow us to not
 * use the lua_State and lua_* functions outside of this file.
 */
#include "../extern/lua/src/lua.hpp"

/* Functions with C linkage which refer to lua_State and need declaration. */
extern "C" {
    void temp_name(char *name, int *err);
    int cmd_f(lua_State *L);
    int get_f(lua_State *L);
    int set_f(lua_State *L);
}

/* ---- State ------------------------------------------------------------------
 *
 * TODO: Set defaults for all state variables.
 */
lua_State *Lua;
int context_flag = CONTEXT_MAIN;

const char *temporary_name_format = "tmp_%d";
int temporary_name = 0;

void debug(const char *msg);
int unpack_args(lua_State *L, const char *function, ScriptValue *args, const char *type_string);

/* Unpacks the lua state according to the type_string where the possible
 * types are:
 *
 * | char | C++ type  | description                            |
 * |------|-----------|----------------------------------------|
 * | `r`  | `double`  | Real number: checks for NaN by default |
 * | `i`  | `int32_t` | 32-bit signed integer                  |
 * | `b`  | `bool`    | Boolean: true or false.                |
 * | `s`  | `QString` | A string TODO: check for type          |
 *
 * TODO: better type checking
 *
 * Note the the lua_to* calls add one to account for lua's indexing from 1.
 */
int
unpack_args(lua_State *L, const char *function, ScriptValue *args, const char *type_string)
{
    int expected = strlen(type_string);
    if (lua_gettop(L) < expected) {
        return 0;
    }

    for (int i=0; type_string[i]; i++) {
        switch (type_string[i]) {
        case 'r': {
            args[i].r = lua_tonumber(L, i+1);
            if (std::isnan(args[i].r)) {
                char message[100];
                sprintf(message, "ERROR: %s argument %d is not a number.",
                   function, i+1);
                debug((const char*)message);
                return 0;
            }
            break;
        }
        case 'i': {
            args[i].i = lua_tointeger(L, i+1);
            break;
        }
        case 's': {
            args[i].s = lua_tostring(L, i+1);
            break;
        }
        case 'b': {
            args[i].b = lua_toboolean(L, i+1);
            break;
        }
        default:
            debug("ERROR: invalid character passed to type_string in arg_check");
            break;
        }
    }
    return 1;
}

/* Convenience function to simplify unpacking vectors supplied as
 * neighboring arguments. Inverts the y-axis because generally that is
 * needed: if you need to override this, use the `no_y_invert` version.
 */
EmbVector
unpack_vector(ScriptValue *args, int offset)
{
    return emb_vector(args[offset].r, -args[offset+1].r);
}

EmbVector
unpack_vector_no_y_invert(ScriptValue *args, int offset)
{
    return emb_vector(args[offset].r, args[offset+1].r);
}

/* A version which doesn't produce the same error message.
 * TODO: This needs to be checked for whether it functions similarly.
 */
void
temp_name(char *name, int *err)
{
    FILE *f = 0;
    *err = 0;
    while (!f) {
        sprintf(name, temporary_name_format, temporary_name);
        f = fopen(name, "w");
        temporary_name++;
    }
    fclose(f);
}

void MainWindow::stub_testing()
{
    QMessageBox::warning(this, tr("Testing Feature"), tr("<b>This feature is in testing.</b>"));
}

/* TODO: Append QSysInfo to string where applicable. */
QString MainWindow::platformString()
{
    qDebug("Platform: %s", OS_STR);
    return OS_STR;
}

void
MainWindow::about()
{
    //TODO: QTabWidget for about dialog
    QApplication::setOverrideCursor(Qt::ArrowCursor);
    qDebug("about()");
    QString appDir = qApp->applicationDirPath();
    QString appName = QApplication::applicationName();
    QString title = "About " + appName;

    QDialog dialog(this);
    ImageWidget img(appDir + "/images/logo-small");
    QLabel text(appName + tr("\n\n") +
                          tr("http://embroidermodder.github.io") +
                          tr("\n\n") +
                          tr("Available Platforms: GNU/Linux, Windows, Mac OSX, Raspberry Pi") +
                          tr("\n\n") +
                          tr("Embroidery formats by Josh Varga.") +
                          tr("\n") +
                          tr("User Interface by Jonathan Greig.") +
                          tr("\n\n") +
                          tr("Free under the zlib/libpng license.")
                          #if defined(BUILD_GIT_HASH)
                          + tr("\n\n") +
                          tr("Build Hash: ") + qPrintable(BUILD_GIT_HASH)
                          #endif
                          );
    text.setWordWrap(true);

    QDialogButtonBox buttonbox(Qt::Horizontal, &dialog);
    QPushButton button(&dialog);
    button.setText("Oh, Yeah!");
    buttonbox.addButton(&button, QDialogButtonBox::AcceptRole);
    buttonbox.setCenterButtons(true);
    connect(&buttonbox, SIGNAL(accepted()), &dialog, SLOT(accept()));

    QVBoxLayout layout;
    layout.setAlignment(Qt::AlignCenter);
    layout.addWidget(&img);
    layout.addWidget(&text);
    layout.addWidget(&buttonbox);

    dialog.setWindowTitle(title);
    dialog.setMinimumWidth(img.minimumWidth()+30);
    dialog.setMinimumHeight(img.minimumHeight()+50);
    dialog.setLayout(&layout);
    dialog.exec();
    QApplication::restoreOverrideCursor();
}

void MainWindow::tipOfTheDay()
{
    qDebug("tipOfTheDay()");

    QString appDir = qApp->applicationDirPath();

    wizardTipOfTheDay = new QWizard(this);
    wizardTipOfTheDay->setAttribute(Qt::WA_DeleteOnClose);
    wizardTipOfTheDay->setWizardStyle(QWizard::ModernStyle);
    wizardTipOfTheDay->setMinimumSize(550, 400);

    QWizardPage* page = new QWizardPage(wizardTipOfTheDay);

    ImageWidget* imgBanner = new ImageWidget(appDir + "/images/did-you-know.png", wizardTipOfTheDay);

    StringList tip_list = string_tables["tips"];
    if (st[ST_CURRENT_TIP].i >= tip_list.size()) {
        st[ST_CURRENT_TIP].i = 0;
    }
    labelTipOfTheDay = new QLabel(tip_list[st[ST_CURRENT_TIP].i].c_str(), wizardTipOfTheDay);
    labelTipOfTheDay->setWordWrap(true);

    QCheckBox* checkBoxTipOfTheDay = new QCheckBox(tr("&Show tips on startup"), wizardTipOfTheDay);
    checkBoxTipOfTheDay->setChecked(st[ST_TIP_OF_THE_DAY].b);
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
    st[ST_TIP_OF_THE_DAY].b = checked;
}

void MainWindow::buttonTipOfTheDayClicked(int button)
{
    qDebug("buttonTipOfTheDayClicked(%d)", button);
    StringList tip_list = string_tables["tips"];
    if (button == QWizard::CustomButton1) {
        if (st[ST_CURRENT_TIP].i > 0) {
            st[ST_CURRENT_TIP].i--;
        }
        else {
            st[ST_CURRENT_TIP].i = tip_list.size() - 1;
        }
        labelTipOfTheDay->setText(tip_list[st[ST_CURRENT_TIP].i].c_str());
    }
    else if(button == QWizard::CustomButton2) {
        st[ST_CURRENT_TIP].i++;
        if (st[ST_CURRENT_TIP].i >= tip_list.size()) {
            st[ST_CURRENT_TIP].i = 0;
        }
        labelTipOfTheDay->setText(tip_list[st[ST_CURRENT_TIP].i].c_str());
    }
    else if(button == QWizard::CustomButton3) {
        wizardTipOfTheDay->close();
    }
}

// Standard Slots
bool MainWindow::isShiftPressed()
{
    return shiftKeyPressedState;
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

    st[ST_ICON_SIZE].i = iconSize;
}

void MainWindow::setUndoCleanIcon(bool opened)
{
    dockUndoEdit->updateCleanIcon(opened);
}

void
MainWindow::updateAllViewScrollBars(bool val)
{
    QList<QMdiSubWindow*> windowList = mdiArea->subWindowList();
    for (int i = 0; i < windowList.count(); i++) {
        MdiWindow* mdiWin = qobject_cast<MdiWindow*>(windowList.at(i));
        if (mdiWin) {
            mdiWin->showViewScrollBars(val);
        }
    }
}

void
MainWindow::updateAllViewCrossHairColors(QRgb color)
{
    QList<QMdiSubWindow*> windowList = mdiArea->subWindowList();
    for (int i = 0; i < windowList.count(); i++) {
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
    for(int i = 0; i < windowList.count(); ++i)
    {
        MdiWindow* mdiWin = qobject_cast<MdiWindow*>(windowList.at(i));
        if(mdiWin) { mdiWin->setViewBackgroundColor(color); }
    }
}

void MainWindow::updateAllViewSelectBoxColors(QRgb colorL, QRgb fillL, QRgb colorR, QRgb fillR, int alpha)
{
    QList<QMdiSubWindow*> windowList = mdiArea->subWindowList();
    for(int i = 0; i < windowList.count(); ++i)
    {
        MdiWindow* mdiWin = qobject_cast<MdiWindow*>(windowList.at(i));
        if(mdiWin) { mdiWin->setViewSelectBoxColors(colorL, fillL, colorR, fillR, alpha); }
    }
}

void MainWindow::updateAllViewGridColors(QRgb color)
{
    QList<QMdiSubWindow*> windowList = mdiArea->subWindowList();
    for(int i = 0; i < windowList.count(); ++i)
    {
        MdiWindow* mdiWin = qobject_cast<MdiWindow*>(windowList.at(i));
        if(mdiWin) { mdiWin->setViewGridColor(color); }
    }
}

void MainWindow::updateAllViewRulerColors(QRgb color)
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
    st[ST_SELECTION_MODE_PICKADD].b = val;
    dockPropEdit->updatePickAddModeButton(val);
}

void MainWindow::pickAddModeToggled()
{
    bool val = !st[ST_SELECTION_MODE_PICKADD].b;
    updatePickAddMode(val);
}

// Layer ToolBar
void MainWindow::makeLayerActive()
{
    qDebug("makeLayerActive()");
    qDebug("TODO: Implement makeLayerActive.");
}

void MainWindow::layerManager()
{
    qDebug("layerManager()");
    qDebug("TODO: Implement layerManager.");
    LayerManager layman(this, this);
    layman.exec();
}

void MainWindow::layerPrevious()
{
    qDebug("layerPrevious()");
    qDebug("TODO: Implement layerPrevious.");
}

void MainWindow::layerSelectorIndexChanged(int index)
{
    qDebug("layerSelectorIndexChanged(%d)", index);
}

void MainWindow::colorSelectorIndexChanged(int index)
{
    qDebug("colorSelectorIndexChanged(%d)", index);

    QComboBox* comboBox = qobject_cast<QComboBox*>(sender());
    QRgb newColor;
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
    qDebug("textFontSelectorCurrentFontChanged()");
    char command[200];
    sprintf(command, "set_text_font(%s)", qPrintable(font.family()));
    cmd(command);
}

// TODO: check that the toReal() conversion is ok
void MainWindow::textSizeSelectorIndexChanged(int index)
{
    qDebug("textSizeSelectorIndexChanged(%d)", index);
    st[ST_TEXT_SIZE].i = qFabs(textSizeSelector->itemData(index).toReal());
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

QString MainWindow::getCurrentLineType()
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

void MainWindow::deletePressed()
{
    qDebug("deletePressed()");
    QApplication::setOverrideCursor(Qt::WaitCursor);
    MdiWindow* mdiWin = qobject_cast<MdiWindow*>(mdiArea->activeSubWindow());
    if(mdiWin) { mdiWin->deletePressed(); }
    QApplication::restoreOverrideCursor();
}

void
MainWindow::escapePressed()
{
    qDebug("escapePressed()");
    QApplication::setOverrideCursor(Qt::WaitCursor);
    MdiWindow* mdiWin = qobject_cast<MdiWindow*>(mdiArea->activeSubWindow());
    if(mdiWin) { mdiWin->escapePressed(); }
    QApplication::restoreOverrideCursor();

    cmd("end");
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

void MainWindow::toggleLwt()
{
    qDebug("toggleLwt()");
    statusbar->statusBarLwtButton->toggle();
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
    if (mdiWin) {
        mdiWin->promptInputPrevious();
    }
}

void MainWindow::promptInputNext()
{
    MdiWindow* mdiWin = activeMdiWindow();
    if(mdiWin) mdiWin->promptInputNext();
}

/**
 * @brief Run the lua script in the file supplied.
 *
 * @todo warning box
 */
void
MainWindow::run(const char *filename)
{
    int status = luaL_dofile(Lua, filename);
    if (status) {
        printf("ERROR: %d\n", status);
        qDebug("Failed to boot scripting environment.");
    }
}

/**
 * @brief The callback for buttons to call `cmd` via the prompt.
 */
void
MainWindow::runCommand(void)
{
    QAction* act = qobject_cast<QAction*>(sender());
    if (act) {
        qDebug("runCommand(%s)", qPrintable(act->objectName()));
        prompt->endCommand();
        prompt->setCurrentText(act->objectName());
        prompt->processInput();
    }
}

/**
 * @brief The standard way to call a command.
 */
void
MainWindow::runCommandMain(const QString& line)
{
    qDebug("runCommandMain(%s)", qPrintable(line));
    // TODO: Uncomment this line when post-selection is available
    // if (!st[ST_SELECTION_MODE_PICKFIRST].b) {
    //     clear_selection();
    // }
    context_flag = CONTEXT_MAIN;
    cmd(qPrintable(line));
}

void
MainWindow::runCommandClick(const QString& line, qreal x, qreal y)
{
    qDebug("runCommandClick(%s, %.2f, %.2f)", qPrintable(line), x, y);
    char mouse_pos[100];
    sprintf(mouse_pos, "mouse = {%f, %f}", x, y);
    context_flag = CONTEXT_CLICK;
    cmd(qPrintable(line));
}

void
MainWindow::runCommandMove(const QString& line, qreal x, qreal y)
{
    qDebug("runCommandMove(%s, %.2f, %.2f)", qPrintable(line), x, y);
    char mouse_pos[100];
    sprintf(mouse_pos, "mouse = {%f, %f}", x, y);
    context_flag = CONTEXT_MOVE;
    cmd(qPrintable(line));
}

void
MainWindow::runCommandContext(const QString& line, const QString& str)
{
    qDebug("runCommandContext(%s, %s)", qPrintable(line), qPrintable(str));
    char context_str[100];
    sprintf(context_str, "prompt = \"%s\"", qPrintable(str));
    context_flag = CONTEXT_CONTEXT;
    cmd(qPrintable(line));
}

void
MainWindow::runCommandPrompt(const QString& line, const QString& str)
{
    qDebug("runCommandPrompt(%s, %s)", qPrintable(line), qPrintable(str));
    char prompt[100];
    sprintf(prompt, "(define prompt \"%s\")", qPrintable(str));
    QString safeStr = str;
    safeStr.replace("\\", "\\\\");
    safeStr.replace("\'", "\\\'");
    /*
    if (prompt->isRapidFireEnabled()) {
        engine->evaluate(cmd + "_prompt('" + safeStr + "')", fileName);
    }
    else {
        engine->evaluate(cmd + "_prompt('" + safeStr.toUpper() + "')", fileName);
    }
    */
    context_flag = CONTEXT_PROMPT;
    cmd(qPrintable(line));
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

/**
 * @brief Adds a EmbTextMulti to the design.
 */
void
MainWindow::add_text_multi(const QString& str, EmbVector position, qreal rot, bool fill, int rubberMode)
{
}

/**
 * @brief Adds a EmbTextSingle to the design.
 */
void
MainWindow::add_text_single(const QString& str, EmbVector position, qreal rot, bool fill, int rubberMode)
{
    View* gview = activeView();
    QGraphicsScene* gscene = gview->scene();
    QUndoStack* stack = gview->getUndoStack();
    if(gview && gscene && stack)
    {
        TextSingleObject* obj = new TextSingleObject(str, position.x, -position.y, getCurrentColor());
        obj->setObjectTextFont(st[ST_TEXT_FONT].s.c_str());
        obj->setObjectTextSize(st[ST_TEXT_SIZE].i);
        obj->setObjectTextStyle(st[ST_TEXT_BOLD].b,
                                st[ST_TEXT_ITALIC].b,
                                st[ST_TEXT_UNDERLINE].b,
                                st[ST_TEXT_STRIKEOUT].b,
                                st[ST_TEXT_OVERLINE].b);
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

/**
 * @brief Adds a EmbInfiniteLine to the design.
 */
void
MainWindow::add_infinite_line(EmbVector point1, EmbVector point2, qreal rot)
{
}

/**
 * @brief Adds a EmbRay to the design.
 */
void
MainWindow::add_ray(EmbVector start, EmbVector point, qreal rot)
{
}

/**
 * @brief Adds a EmbLine to the design.
 */
void
MainWindow::add_line(EmbVector start, EmbVector end, qreal rot, int rubberMode)
{
    View* gview = activeView();
    QGraphicsScene* gscene = gview->scene();
    QUndoStack* stack = gview->getUndoStack();
    if(gview && gscene && stack)
    {
        LineObject* obj = new LineObject(start.x, -start.y, end.x, -end.y, getCurrentColor());
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

/**
 * @brief Adds a EmbCircle to the design.
 */
void
MainWindow::add_circle(EmbVector center, double radius, bool fill, int rubberMode)
{
    View* gview = activeView();
    QGraphicsScene* gscene = gview->scene();
    QUndoStack* stack = gview->getUndoStack();
    if (gview && gscene && stack) {
        CircleObject* obj = new CircleObject(center.x, center.y, radius, getCurrentColor());
        obj->setObjectRubberMode(rubberMode);
        //TODO: circle fill
        if (rubberMode) {
            gview->addToRubberRoom(obj);
            gscene->addItem(obj);
            gscene->update();
        }
        else {
            UndoableAddCommand* cmd = new UndoableAddCommand(obj->data(OBJ_NAME).toString(), obj, gview, 0);
            stack->push(cmd);
        }
    }
}

/**
 * @brief Adds a EmbLine to the design.
 */
void
MainWindow::add_triangle(EmbVector point1, EmbVector point2, EmbVector point3,
    qreal rot, bool fill)
{
}

/**
 * @brief Adds a EmbLine to the design.
 */
void
MainWindow::add_rectangle(qreal x, qreal y, qreal w, qreal h, qreal rot,
    bool fill, int rubberMode)
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

/**
 * @brief Adds a EmbLine to the design.
 */
void
MainWindow::add_rounded_rectangle(qreal x, qreal y, qreal w, qreal h, qreal rad, qreal rot, bool fill)
{
}

/**
 * @brief Adds a EmbLine to the design.
 */
void
MainWindow::add_arc(EmbVector start, EmbVector mid, EmbVector end, int rubberMode)
{
    View* gview = activeView();
    QGraphicsScene* scene = activeScene();
    if (!(gview && scene)) {
        return;
    }

    ArcObject* arcObj = new ArcObject(start.x, -start.y, mid.x, -mid.y, end.x, -end.y,
        getCurrentColor());
    arcObj->setObjectRubberMode(rubberMode);
    if (rubberMode) {
        gview->addToRubberRoom(arcObj);
    }
    scene->addItem(arcObj);
    scene->update();
}

/**
 * @brief Adds a EmbLine to the design.
 */
void
MainWindow::add_slot(EmbVector center, qreal diameter, qreal length, qreal rot, bool fill,
    int rubberMode)
{
    //TODO: Use UndoableAddCommand for slots
    /*
    SlotObject* slotObj = new SlotObject(center.x, -center.y, diameter, length, getCurrentColor());
    slotObj->setRotation(-rot);
    slotObj->setObjectRubberMode(rubberMode);
    if(rubberMode) gview->addToRubberRoom(slotObj);
    scene->addItem(slotObj);
    //TODO: slot fill
    scene->update();
    */
}

/**
 * @brief Adds a EmbLine to the design.
 */
void
MainWindow::add_ellipse(EmbVector center, qreal width, qreal height, qreal rot, bool fill,
    int rubberMode)
{
    View* gview = activeView();
    QGraphicsScene* gscene = gview->scene();
    QUndoStack* stack = gview->getUndoStack();
    if (!(gview && gscene && stack)) {
        return;
    }

    EllipseObject* obj = new EllipseObject(center.x, -center.y, width, height, getCurrentColor());
    obj->setRotation(-rot);
    obj->setObjectRubberMode(rubberMode);
    //TODO: ellipse fill
    if (rubberMode) {
        gview->addToRubberRoom(obj);
        gscene->addItem(obj);
        gscene->update();
    }
    else {
        UndoableAddCommand* cmd = new UndoableAddCommand(obj->data(OBJ_NAME).toString(), obj,
            gview, 0);
        stack->push(cmd);
    }
}

/**
 * @brief Add an EmbPoint to the design.
 */
void
MainWindow::add_point(EmbVector position)
{
    View* gview = activeView();
    QUndoStack* stack = gview->getUndoStack();
    if (!(gview && stack)) {
        return;
    }

    PointObject* obj = new PointObject(position.x, -position.y, getCurrentColor());
    UndoableAddCommand* cmd = new UndoableAddCommand(obj->data(OBJ_NAME).toString(),
        obj, gview, 0);
    stack->push(cmd);
}

/**
 * @brief Add an EmbRegularPolygon to the design.
 */
void
MainWindow::add_regular_polygon(EmbVector center, quint16 sides, quint8 mode, qreal rad, qreal rot, bool fill)
{
}

/**
 * @brief Adds a EmbPolygon to the design.
 *
 * @note This native is different than the rest in that the Y+ is down
 *       (scripters need not worry about this).
 */
void
MainWindow::add_polygon(EmbVector start, const QPainterPath& p, int rubberMode)
{
    /*
    if (context->argumentCount() != 1)   return debug("addPolygon() requires one argument");
    if (!context->argument(0).isArray()) return debug(TypeError, "addPolygon(): first argument is not an array");

    QVariantList varList = context->argument(0).toVariant().toList();
    int varSize = varList.size();
    if (varSize < 2) return debug(TypeError, "addPolygon(): array must contain at least two elements");
    if (varSize % 2) return debug(TypeError, "addPolygon(): array cannot contain an odd number of elements");

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
            return debug(TypeError, "addPolygon(): array contains one or more invalid elements");
        }
    }

    //Close the polygon
    path.closeSubpath();

    path.translate(-startX, -startY);

    _mainWin->add_polygon(startX, startY, path, OBJ_RUBBER_OFF);
    */

    View* gview = activeView();
    QGraphicsScene* gscene = gview->scene();
    QUndoStack* stack = gview->getUndoStack();
    if(gview && gscene && stack)
    {
        PolygonObject* obj = new PolygonObject(start.x, start.y, p, getCurrentColor());
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

/**
 * @brief Adds a EmbPolyline to the design.
 *
 * @note This native is different than the rest in that the Y+ is down
 *       (scripters need not worry about this).
 */
void
MainWindow::add_polyline(EmbVector start, const QPainterPath& p, int rubberMode)
{
    /*
    if (context->argumentCount() != 1)   return debug("addPolyline() requires one argument");
    if (!context->argument(0).isArray()) return debug(TypeError, "addPolyline(): first argument is not an array");

    QVariantList varList = context->argument(0).toVariant().toList();
    int varSize = varList.size();
    if (varSize < 2) return debug(TypeError, "addPolyline(): array must contain at least two elements");
    if (varSize % 2) return debug(TypeError, "addPolyline(): array cannot contain an odd number of elements");

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
            return debug(TypeError, "addPolyline(): array contains one or more invalid elements");
        }
    }

    path.translate(-startX, -startY);

    _mainWin->add_polyline(startX, startY, path, OBJ_RUBBER_OFF);
    */

    View* gview = activeView();
    QGraphicsScene* gscene = gview->scene();
    QUndoStack* stack = gview->getUndoStack();
    if(gview && gscene && stack)
    {
        PolylineObject* obj = new PolylineObject(start.x, start.y, p, getCurrentColor());
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

/**
 * @brief Adds a EmbHorizontalDimension to the design.
 *
 * @note This native is different than the rest in that the Y+ is down
 *       (scripters need not worry about this).
 */
void
MainWindow::add_path(EmbVector start, const QPainterPath& p, int rubberMode)
{
}

/**
 * @brief Adds a EmbHorizontalDimension to the design.
 */
void
MainWindow::add_horizontal_dimension(EmbVector start, EmbVector end, qreal legHeight)
{
}

/**
 * @brief Adds a EmbVerticalDimension to the design.
 */
void
MainWindow::add_vertical_dimension(EmbVector start, EmbVector end, qreal legHeight)
{
}

/**
 * @brief Adds a EmbImage to the design.
 */
void
MainWindow::add_image(const QString& img, qreal x, qreal y, qreal w, qreal h, qreal rot)
{
}

/**
 * @brief Adds a EmbDimLeader to the design.
 */
void
MainWindow::add_dim_leader(double x1, double y1, double x2, double y2, qreal rot, int rubberMode)
{
    View* gview = activeView();
    QGraphicsScene* gscene = gview->scene();
    QUndoStack* stack = gview->getUndoStack();
    if (gview && gscene && stack) {
        DimLeaderObject* obj = new DimLeaderObject(x1, -y1, x2, -y2,
            getCurrentColor());
        obj->setRotation(-rot);
        obj->setObjectRubberMode(rubberMode);
        if (rubberMode) {
            gview->addToRubberRoom(obj);
            gscene->addItem(obj);
            gscene->update();
        }
        else {
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
        else if(shape == "dragmove")        gview->setCursor(QCursor(Qt::DragMoveCursor));
        else if(shape == "dragcopy")        gview->setCursor(QCursor(Qt::DragCopyCursor));
        else if(shape == "draglink")        gview->setCursor(QCursor(Qt::DragLinkCursor));
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
    double dx = px-x1;
    double dy = py-y1;
    norm.translate(dx, dy);
    QPointF iPoint;
    norm.intersects(line, &iPoint);
    return QLineF(px, py, iPoint.x(), iPoint.y()).length();
}

void MainWindow::nativeAddToSelection(const QPainterPath path, Qt::ItemSelectionMode mode)
{
}

void MainWindow::nativeScaleSelected(qreal x, qreal y, qreal factor)
{
    if (factor <= 0.0) {
        QMessageBox::critical(this, tr("ScaleFactor Error"),
                                tr("Hi there. If you are not a developer, report this as a bug. "
                                "If you are a developer, your code needs examined, and possibly your head too."));
    }

    View* gview = activeView();
    if(gview) { gview->scaleSelected(x, -y, factor); }
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

/* -- Basic wrappers of MainWindow functions -------------------------------- */
View*
activeView(void)
{
    MdiWindow* mdiWin = qobject_cast<MdiWindow*>(mdiArea->activeSubWindow());
    if (mdiWin) {
        View* v = mdiWin->gview;
        return v;
    }
    return NULL;
}

QGraphicsScene*
activeScene(void)
{
    MdiWindow* mdiWin = qobject_cast<MdiWindow*>(mdiArea->activeSubWindow());
    if (mdiWin) {
        QGraphicsScene* s = mdiWin->gscene;
        return s;
    }
    return 0;
}

MdiWindow*
activeMdiWindow(void)
{
    qDebug("activeMdiWindow()");
    MdiWindow* mdiWin = qobject_cast<MdiWindow*>(mdiArea->activeSubWindow());
    return mdiWin;
}

QUndoStack*
activeUndoStack(void)
{
    qDebug("activeUndoStack()");
    View* v = activeView();
    if (v) {
        QUndoStack* u = v->getUndoStack();
        return u;
    }
    return 0;
}

QRgb
getCurrentColor(void)
{
    MdiWindow* mdiWin = qobject_cast<MdiWindow*>(mdiArea->activeSubWindow());
    if (mdiWin) {
        return mdiWin->curColor;
    }
    return 0; //TODO: return color ByLayer
}

void
debug(const char *msg)
{
    qDebug(msg);
}

/* -- lua registerables --------------------------------------------------------
 *
 * Try to keep this section of the file alphabetized in the function name to
 * match the declarations above.
 *
 * All function names should end in `_f`.
 */

/* . */
int
allow_rubber_f(lua_State *L)
{
    View* gview = activeView();
    /*
    if (gview) {
        return gview->allowRubber();
    }
    return false;
    */
    return 0;
}

/* . */
int
clear_rubber_f(lua_State *L)
{
    View* gview = activeView();
    if (gview) {
        gview->clearRubberRoom();
    }
    return 0;
}

/* . */
int
delete_selected_f(lua_State *L)
{
    View* gview = activeView();
    if (gview) {
        gview->deleteSelected();
    }
    _mainWin->cmd("clear");
    return 0;
}

/* . */
int
messagebox_f(lua_State *L)
{
    debug("TODO: add_to_menu");
    /*
    if (context->argumentCount() != 3)    return debug("messageBox() requires three arguments");
    if (!context->argument(0).isString()) return debug(TypeError, "messageBox(): first argument is not a string");
    if (!context->argument(1).isString()) return debug(TypeError, "messageBox(): second argument is not a string");
    if (!context->argument(2).isString()) return debug(TypeError, "messageBox(): third argument is not a string");

    QString type = context->argument(0).toString().toLower();
    QString title = context->argument(1).toString();
    QString text = context->argument(2).toString();

    if (type != "critical" && type != "information" && type != "question" && type != "warning")
        return debug(UnknownError, "messageBox(): first argument must be \"critical\", \"information\", \"question\" or \"warning\".");

    _mainWin->nativeMessageBox(type, title, text);
    */
    return 0;
}

/* . */
int
preview_off_f(lua_State *L)
{
    debug("TODO: preview_off_f");
    View* gview = activeView();
    if (gview) {
        gview->previewOff();
    }
    return 0;
}

/* . */
int
preview_on_f(lua_State *L)
{
    ScriptValue args[5];
    if (!unpack_args(L, "preview_on_f", args, "ssrrr")) {
        return 0;
    }

    QString cloneStr = QString(args[0].s.c_str()).toUpper();
    QString modeStr = QString(args[1].s.c_str()).toUpper();
    double x = args[2].r;
    double y = args[3].r;
    double data = args[4].r;

    int clone = PREVIEW_CLONE_NULL;
    int mode = PREVIEW_MODE_NULL;
    if (cloneStr == "SELECTED") {
        clone = PREVIEW_CLONE_SELECTED;
    }
    else if (cloneStr == "RUBBER") {
        clone = PREVIEW_CLONE_RUBBER;
    }
    else {
        debug("ERROR, preview_on_f(): first argument must be \"SELECTED\" or \"RUBBER\".");
        return 0;
    }

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
        debug("ERROR preview_on_f(): second argument must be \"MOVE\", \"ROTATE\" or \"SCALE\".");
        return 0;
    }

    View* gview = activeView();
    if (gview) {
        gview->previewOn(clone, mode, x, -y, data);
    }
    return 0;
}

/* . */
int
print_area_f(lua_State *L)
{
    ScriptValue args[5];
    if (!unpack_args(L, "preview_on_f", args, "rrrr")) {
        return 0;
    }
    double x = args[0].r;
    double y = args[1].r;
    double w = args[2].r;
    double h = args[3].r;

    qDebug("print_area_f(%.2f, %.2f, %.2f, %.2f)", x, y, w, h);
    //TODO: Print Setup Stuff
    _mainWin->cmd("print");
    return 0;
}

/* . */
int
rubber_f(lua_State *L)
{
    debug("TODO: add_to_menu");
    return 0;
}

/**
 * @brief Lua interface to core commands.
 *
 * User based interaction comes via this interface so we can parse
 * the differences between interaction contexts.
 *
 * TODO: currently can't pass arguments through to core commands.
 */
int
cmd_f(lua_State *L)
{
    ScriptValue args[2];
    if (!unpack_args(L, "cmd_f", args, "s")) {
        return 0;
    }
    if (context_flag == CONTEXT_MAIN) {
        _mainWin->cmd("clear_rubber");
        /* Some selection based commands need to override this. */
        _mainWin->cmd("clear");
    }
    _mainWin->cmd(args[0].s.c_str());
    /* TODO: conditional on if the command is open ended or not. */
    _mainWin->cmd("end");
    return 0;
}

/* . */
int
text_font_f(lua_State *L)
{
    debug("TODO: text_font_f");
    /* return QScriptValue(st[ST_TEXT_FONT].s.c_str()); */
    return 0;
}

/* . */
int
vulcanize_f(lua_State *L)
{
    View* gview = activeView();
    if (gview) {
        gview->vulcanizeRubberRoom();
    }
    return 0;
}

/* . */
int
set_color_f(lua_State *L)
{
    ScriptValue args[4];
    if (!unpack_args(L, "set_color_f", args, "srrr")) {
        return 0;
    }
    QString key = args[0].s.c_str();
    double r = args[1].r;
    double g = args[2].r;
    double b = args[3].r;

    if ((r < 0) || (r > 255)) {
        debug("ERROR set_color(): r value must be in range 0-255");
        return 0;
    }
    if ((g < 0) || (g > 255)) {
        debug("ERROR set_color(): g value must be in range 0-255");
        return 0;
    }
    if ((b < 0) || (b > 255)) {
        debug("ERROR set_color(): b value must be in range 0-255");
        return 0;
    }

    if (key == "background") {
        st[ST_BG_COLOR].u = qRgb(r,g,b);
        _mainWin->updateAllViewBackgroundColors(qRgb(r,g,b));
    }
    else if (key == "crosshair") {
        st[ST_CROSSHAIR_COLOR].u = qRgb(r,g,b);
        _mainWin->updateAllViewCrossHairColors(qRgb(r,g,b));
    }
    else if (key == "grid") {
        st[ST_GRID_COLOR].u = qRgb(r,g,b);
        _mainWin->updateAllViewGridColors(qRgb(r,g,b));
    }
    else {
        debug("ERROR set_color(): key does not correspond to a color setting variable");
    }
    return 0;
}

/* . */
int
set_rubber_mode_f(lua_State *L)
{
    debug("TODO: add_to_menu");
    /*
    if (context->argumentCount() != 1)    return debug("setRubberMode() requires one argument");
    if (!context->argument(0).isString()) return debug(TypeError, "setRubberMode(): first argument is not a string");

    QString mode = context->argument(0).toString().toUpper();

    if (mode == "CIRCLE_1P_RAD") {
        _mainWin->nativeSetRubberMode(OBJ_RUBBER_CIRCLE_1P_RAD);
    }
    else if (mode == "CIRCLE_1P_DIA") {
        _mainWin->nativeSetRubberMode(OBJ_RUBBER_CIRCLE_1P_DIA);
    }
    else if (mode == "CIRCLE_2P") {
        _mainWin->nativeSetRubberMode(OBJ_RUBBER_CIRCLE_2P);
    }
    else if (mode == "CIRCLE_3P") {
        _mainWin->nativeSetRubberMode(OBJ_RUBBER_CIRCLE_3P);
    }
    else if (mode == "CIRCLE_TTR") {
        _mainWin->nativeSetRubberMode(OBJ_RUBBER_CIRCLE_TTR);
    }
    else if (mode == "CIRCLE_TTR") {
        _mainWin->nativeSetRubberMode(OBJ_RUBBER_CIRCLE_TTT);
    }
    else if (mode == "DIMLEADER_LINE") {
        _mainWin->nativeSetRubberMode(OBJ_RUBBER_DIMLEADER_LINE);
    }
    else if (mode == "ELLIPSE_LINE") {
        _mainWin->nativeSetRubberMode(OBJ_RUBBER_ELLIPSE_LINE);
    }
    else if (mode == "ELLIPSE_MAJORDIAMETER_MINORRADIUS") {
        _mainWin->nativeSetRubberMode(OBJ_RUBBER_ELLIPSE_MAJORDIAMETER_MINORRADIUS);
    }
    else if (mode == "ELLIPSE_MAJORRADIUS_MINORRADIUS") {
        _mainWin->nativeSetRubberMode(OBJ_RUBBER_ELLIPSE_MAJORRADIUS_MINORRADIUS);
    }
    else if (mode == "ELLIPSE_ROTATION") {
        _mainWin->nativeSetRubberMode(OBJ_RUBBER_ELLIPSE_ROTATION);
    }
    else if (mode == "LINE") {
        _mainWin->nativeSetRubberMode(OBJ_RUBBER_LINE);
    }
    else if (mode == "POLYGON") {
        _mainWin->nativeSetRubberMode(OBJ_RUBBER_POLYGON);
    }
    else if (mode == "POLYGON_INSCRIBE") {
        _mainWin->nativeSetRubberMode(OBJ_RUBBER_POLYGON_INSCRIBE);
    }
    else if (mode == "POLYGON_CIRCUMSCRIBE") {
        _mainWin->nativeSetRubberMode(OBJ_RUBBER_POLYGON_CIRCUMSCRIBE);
    }
    else if (mode == "POLYLINE") {
        _mainWin->nativeSetRubberMode(OBJ_RUBBER_POLYLINE);
    }
    else if (mode == "RECTANGLE") {
        _mainWin->nativeSetRubberMode(OBJ_RUBBER_RECTANGLE);
    }
    else if (mode == "TEXTSINGLE") {
        _mainWin->nativeSetRubberMode(OBJ_RUBBER_TEXTSINGLE);
    }
    else {
        return debug(UnknownError, "setRubberMode(): unknown rubberMode value");
    }
    */
    return 0;
}

/* . */
int
set_rubber_text_f(lua_State *L)
{
    ScriptValue args[2];
    if (!unpack_args(L, "set_rubber_text_f", args, "ss")) {
        return 0;
    }
    QString key = QString(args[0].s.c_str()).toUpper();
    QString txt = args[1].s.c_str();

    View* gview = activeView();
    if (gview) {
        gview->setRubberText(key, txt);
    }
    return 0;
}

/* . */
int
set_text_font_f(lua_State *L)
{
    debug("TODO: add_to_menu");
    const char *str = luaL_checkstring(L, 1);

    _mainWin->textFontSelector->setCurrentFont(QFont(str));
    st[ST_TEXT_FONT].s = str;
    return 0;
}

/* TODO: convert set_rubber_point_f. */
int
set_rubber_point_f(lua_State *L)
{
    QString key = QString(luaL_checkstring(L, 1)).toUpper();
    double x = lua_tonumber(L, 2);
    double y = lua_tonumber(L, 3);

    if (std::isnan(x)) {
        debug("TypeError, set_rubber_point_f: second argument failed isNaN check.");
        return 0;
    }
    if (std::isnan(y)) {
        debug("TypeError, set_rubber_point_f: third argument failed isNaN check.");
        return 0;
    }

    View* gview = activeView();
    if (gview) {
        gview->setRubberPoint(key, QPointF(x, -y));
    }
    return 0;
}

/* Adds the lua function (alert "EXAMPLE ALERT").
 */
int
alert_f(lua_State *L)
{
    ScriptValue args[1];
    if (!unpack_args(L, "alert_f", args, "s")) {
        return 0;
    }
    prompt->alert(args[0].s.c_str());
    return 0;
}

/*
 */
int
append_prompt_history_f(lua_State *L)
{
    ScriptValue args[1];
    if (!unpack_args(L, "append_prompt_history_f", args, "s")) {
        return 0;
    }
    prompt->appendHistory(args[0].s.c_str());
    return 0;
}

/* Adds the lua function (blink-prompt).
 */
int
blink_f(lua_State *L)
{
    prompt->startBlinking();
    return 0;
}

/* Adds the lua function:
 *     (debug "message")
 */
int
debug_f(lua_State *L)
{
    ScriptValue args[1];
    if (!unpack_args(L, "debug_f", args, "s")) {
        return 0;
    }
    qDebug("%s", args[0]);
    return 0;
}

/* Adds the lua function (error "EXAMPLE ERROR").
 */
int
error_f(lua_State *L)
{
    ScriptValue args[1];
    if (!unpack_args(L, "error_f", args, "ss")) {
        return 0;
    }

    String msg = "ERROR: (" + args[0].s + ") " + args[1].s;
    prompt->setPrefix(QString(msg.c_str()));
    prompt->appendHistory(QString());
    return 0;
}

/* Reports the entire state for debugging purposes: this means we don't have to
 * be paranoid about losing aspects of the state in lua variables because
 * we can check the contents of state before and after a given user action
 * during testing.
 *
 * TODO: finish reporting here.
 */
int
report_state_f(lua_State *L)
{
    FILE *f = fopen("state.txt", "w");
    if (!f) {
        return 0;
    }
    fprintf(f, "context_flag = %d\n", context_flag);
    fclose(f);
    return 0;
}

/* . */
int
color_selector_f(lua_State *L)
{
    return 0;
}

/* . */
int
design_details_f(lua_State *L)
{
    return 0;
}

/* . */
int
disable_move_rapid_fire_f(lua_State *L)
{
    View* gview = activeView();
    if (gview) {
        gview->disableMoveRapidFire();
    }
    return 0;
}

/* . */
int
disable_prompt_rapid_fire_f(lua_State *L)
{
    prompt->disableRapidFire();
    return 0;
}

/* . */
int
distance_f(lua_State *L)
{
    return 0;
}

/* . */
int
dolphin_f(lua_State *L)
{
    return 0;
}

/* . */
int
enable_move_rapid_fire_f(lua_State *L)
{
    View* gview = activeView();
    if (gview) {
        gview->enableMoveRapidFire();
    }
    return 0;
}

/* . */
int
enable_prompt_rapid_fire_f(lua_State *L)
{
    prompt->enableRapidFire();
    return 0;
}

/* . */
int
erase_f(lua_State *L)
{
    return 0;
}

/* . */
int
freeze_all_layers_f(lua_State *L)
{
    return 0;
}

/* . */
int
heart_f(lua_State *L)
{
    return 0;
}

/* . */
int
hide_all_layers_f(lua_State *L)
{
    return 0;
}

/* . */
int
layers_f(lua_State *L)
{
    return 0;
}

/* . */
int
layer_previous_f(lua_State *L)
{
    return 0;
}

/* . */
int
layer_selector_f(lua_State *L)
{
    return 0;
}

int
line_type_selector_f(lua_State *L)
{
    return 0;
}

int
line_weight_selector_f(lua_State *L)
{
    return 0;
}

int
locate_point_f(lua_State *L)
{
    return 0;
}

/* . */
int
lock_all_layers_f(lua_State *L)
{
    return 0;
}

/* . */
int
make_layer_current_f(lua_State *L)
{
    return 0;
}

/* . */
int
move_f(lua_State *L)
{
    return 0;
}

int
path_f(lua_State *L)
{
    return 0;
}

int
polygon_f(lua_State *L)
{
    return 0;
}

int
polyline_f(lua_State *L)
{
    return 0;
}

int
quickleader_f(lua_State *L)
{
    return 0;
}

int
rgb_f(lua_State *L)
{
    return 0;
}

int
rotate_f(lua_State *L)
{
    return 0;
}

int
selectall_f(lua_State *L)
{
    return 0;
}

int
singlelinetext_f(lua_State *L)
{
    return 0;
}

/* . */
int
print_f(lua_State *L)
{
    return 0;
}

int
scale_f(lua_State *L)
{
    return 0;
}

int
show_all_layers_f(lua_State *L)
{
    return 0;
}

int
snowflake_f(lua_State *L)
{
    return 0;
}

int
star_f(lua_State *L)
{
    return 0;
}

int
syswindows_f(lua_State *L)
{
    return 0;
}

/* . */
int
thaw_all_layers_f(lua_State *L)
{
    return 0;
}

/* . */
int
tip_of_the_day_f(lua_State *L)
{
    _mainWin->tipOfTheDay();
    return 0;
}

/* Adds the lua function:
 *     (todo "Missing feature description")
 */
int
todo_f(lua_State *L)
{
    QString strCmd(luaL_checkstring(L, 1));
    QString strTodo(luaL_checkstring(L, 2));

    prompt->alert("TODO: (" + strCmd + ") " + strTodo);
    return 0;
}

int
unlock_all_layers_f(lua_State *L)
{
    return 0;
}

/* TODO: move add_rubber to script */
int
add_rubber_f(lua_State *L)
{
    /*
    if (context->argumentCount() != 1) {
        return debug("addRubber() requires one argument");
    }
    if (!context->argument(0).isString()) {
        return debug(TypeError, "addRubber(): first argument is not a string");
    }

    QString objType = context->argument(0).toString().toUpper();

    // FIXME
    cmd("a = allow_rubber()");
    if (!a) {
        return debug(UnknownError, "addRubber(): You must use vulcanize() before you can add another rubber object.");
    }

    double mx = _mainWin->nativeMouseX();
    double my = _mainWin->nativeMouseY();

    if (objType == "ARC") {
        //TODO: handle this type
    }
    else if (objType == "BLOCK") {
        //TODO: handle this type
    }
    else if (objType == "CIRCLE") {
        _mainWin->add_circle(mx, my, 0, false, OBJ_RUBBER_ON);
    }
    else if (objType == "DIMALIGNED") {
        //TODO: handle this type
    }
    else if (objType == "DIMANGULAR") {
        //TODO: handle this type
    }
    else if (objType == "DIMARCLENGTH") {
        //TODO: handle this type
    }
    else if (objType == "DIMDIAMETER") {

} //TODO: handle this type
    else if (objType == "DIMLEADER") {
        _mainWin->add_dim_leader(mx, my, mx, my, 0, OBJ_RUBBER_ON);
    }
    else if (objType == "DIMLINEAR") {

} //TODO: handle this type
    else if (objType == "DIMORDINATE") {

} //TODO: handle this type
    else if (objType == "DIMRADIUS")   {

} //TODO: handle this type
    else if (objType == "ELLIPSE") { _mainWin->add_ellipse(mx, my, 0, 0, 0, 0, OBJ_RUBBER_ON); }
    else if (objType == "ELLIPSEARC") {

} //TODO: handle this type
    else if (objType == "HATCH")       {} //TODO: handle this type
    else if (objType == "IMAGE")       {} //TODO: handle this type
    else if (objType == "INFINITELINE") {} //TODO: handle this type
    else if (objType == "LINE")        { _mainWin->add_line(mx, my, mx, my, 0, OBJ_RUBBER_ON); }
    else if (objType == "PATH")        {} //TODO: handle this type
    else if (objType == "POINT")       {} //TODO: handle this type
    else if (objType == "POLYGON")     { _mainWin->add_polygon(mx, my, QPainterPath(), OBJ_RUBBER_ON); }
    else if (objType == "POLYLINE")    { _mainWin->add_polyline(mx, my, QPainterPath(), OBJ_RUBBER_ON); }
    else if (objType == "RAY")         {} //TODO: handle this type
    else if (objType == "RECTANGLE")   { _mainWin->add_rectangle(mx, my, mx, my, 0, 0, OBJ_RUBBER_ON); }
    else if (objType == "SPLINE")      {} //TODO: handle this type
    else if (objType == "TEXTMULTI")   {} //TODO: handle this type
    else if (objType == "TEXTSINGLE")  { _mainWin->add_text_single("", mx, my, 0, false, OBJ_RUBBER_ON); }
    */
    return 0;
}

/* . */
int
spare_rubber_f(lua_State *L)
{
    ScriptValue args[1];
    if (!unpack_args(L, "spare_rubber_f", args, "s")) {
        return 0;
    }

    QString objID = QString(args[0].s.c_str()).toUpper();

    if (objID == "PATH") {
        _mainWin->nativeSpareRubber(SPARE_RUBBER_PATH);
    }
    else if (objID == "POLYGON") {
        _mainWin->nativeSpareRubber(SPARE_RUBBER_POLYGON);
    }
    else if (objID == "POLYLINE") {
        _mainWin->nativeSpareRubber(SPARE_RUBBER_POLYLINE);
    }
    else {
        bool ok = false;
        qint64 id = objID.toLongLong(&ok);
        if (!ok) {
            debug("TypeError, spare_rubber(): error converting object ID into an int64");
            return 0;
        }
        _mainWin->nativeSpareRubber(id);
    }

    return 0;
}


int
set_cursor_shape_f(lua_State *L)
{
    ScriptValue args[1];
    if (!unpack_args(L, "set_cursor_shape_f", args, "s")) {
        return 0;
    }

    QString shape = args[0].s.c_str();
    _mainWin->nativeSetCursorShape(shape);
    return 0;
}

int
calculate_angle_f(lua_State *L)
{
    ScriptValue args[4];
    if (!unpack_args(L, "calculate_angle_f", args, "rrrr")) {
        return 0;
    }

    EmbVector start = emb_vector(args[0].r, args[1].r);
    EmbVector end = emb_vector(args[2].r, args[3].r);
    //return QScriptValue(_mainWin->nativeCalculateAngle(x1, y1, x2, y2));
    return 0;
}

/* . */
int
calculate_distance_f(lua_State *L)
{
    ScriptValue args[4];
    if (!unpack_args(L, "calculate_distance_f", args, "rrrr")) {
        return 0;
    }
    EmbVector start = emb_vector(args[0].r, args[1].r);
    EmbVector end = emb_vector(args[2].r, args[3].r);
    EmbVector line = emb_vector_subtract(end, start);

    //return QScriptValue(emb_vector_length(line));
    return 0;
}

/* . */
int
perpendicular_distance_f(lua_State *L)
{
    ScriptValue args[6];
    if (!unpack_args(L, "perpendicular_distance_f", args, "rrrrrr")) {
        return 0;
    }

//    return QScriptValue(_mainWin->nativePerpendicularDistance(px, py, x1, y1, x2, y2));
    return 0;
}

/* . */
int
add_to_selection_f(lua_State *L)
{
    //TODO: finish
    return 0;
}

/* . */
int
cut_selected_f(lua_State *L)
{
    ScriptValue args[2];
    if (!unpack_args(L, "cut_selected_f", args, "rr")) {
        return 0;
    }

    EmbVector position = unpack_vector(args, 0);
    // TODO: cut selected at position
    return 0;
}

/* . */
int
copy_selected_f(lua_State *L)
{
    ScriptValue args[2];
    if (!unpack_args(L, "copy_selected_f", args, "rr")) {
        return 0;
    }

    EmbVector position = unpack_vector(args, 0);
    // TODO: copy selected at position
    return 0;
}

/* . */
int
paste_selected_f(lua_State *L)
{
    ScriptValue args[2];
    if (!unpack_args(L, "paste_selected_f", args, "rr")) {
        return 0;
    }

    EmbVector position = unpack_vector(args, 0);
    // TODO: paste selected at position
    return 0;
}

/* . */
int
move_selected_f(lua_State *L)
{
    ScriptValue args[2];
    if (!unpack_args(L, "move_selected_f", args, "rr")) {
        return 0;
    }

    EmbVector delta = unpack_vector(args, 0);
    View* gview = activeView();
    if (gview) {
        gview->moveSelected(delta.x, delta.y);
    }
    return 0;
}

/* . */
int
scale_selected_f(lua_State *L)
{
    ScriptValue args[3];
    if (!unpack_args(L, "scale_selected_f", args, "rrr")) {
        return 0;
    }

    double x = args[0].r;
    double y = args[1].r;
    double factor = args[2].r;

    if (factor <= 0.0) {
        debug("ERROR scale_selected_f: scale factor must be greater than zero");
        return 0;
    }

    _mainWin->nativeScaleSelected(x, y, factor);
    return 0;
}

/* . */
int
rotate_selected_f(lua_State *L)
{
    ScriptValue args[3];
    if (!unpack_args(L, "rotate_selected_f", args, "rrr")) {
        return 0;
    }

    _mainWin->nativeRotateSelected(args[0].r, args[1].r, args[2].r);
    return 0;
}

/* . */
int
mirror_selected_f(lua_State *L)
{
    ScriptValue args[4];
    if (!unpack_args(L, "mirror_selected_f", args, "rrrr")) {
        return 0;
    }

    _mainWin->nativeMirrorSelected(args[0].r, args[1].r, args[2].r, args[3].r);
    return 0;
}

/**
 * @brief Set a state variable to a lua variable.
 *
 * EXAMPLE: set("text_font", "Arial")
 *
 * FIXME: this is an untested draft.
 */
int
set_f(lua_State *L)
{
    if (!lua_isstring(L, 1)) {
        debug("set: first argument not a string");
        return 0;
    }
    const char *key = lua_tostring(L, 1);
    ScriptValue value;
    value.b = false;
    if (lua_isboolean(L, 2)) {
        value.b = lua_toboolean(L, 2);
        return 0;
    }
    if (lua_isinteger(L, 2)) {
        value.i = lua_tointeger(L, 2);
        return 0;
    }
    if (lua_isstring(L, 2)) {
        value.s = lua_tostring(L, 2);
        return 0;
    }
    _mainWin->set(key, value);
    return 0;
}

/**
 * @brief Get a state variable and pass it as a lua variable.
 *
 * EXAMPLE: font = get("text_font")
 *
 * @fixme this is an untested draft.
 */
int
get_f(lua_State *L)
{
    if (!lua_isstring(L, 1)) {
        debug("get: first argument not a string");
        return 0;
    }
    ScriptValue result = _mainWin->get(lua_tostring(L, 1));
    lua_pushnumber(L, result.i);
    return 1;
}

/*!
 * \brief Load TOML data and initialise lua registerables.
 *
 * Lua in Embroidermodder 2 uses a 2 stage boot process.
 *
 * 1. Built-in style hookups to Embroidermodder2 features which are
 *    lua functions.
 * 2. Commands written in lua-only which represent the QActions used by
 *    the interface in the menus, toolbars and command line.
 *
 * Each command loaded in boot step 3 has up to 5 contexts
 * (see the list of defines in the headers: grep for "CONTEXT_") and will switch
 * depending on what context it is called in.
 */
bool
script_env_boot(void)
{
    /** Setting up Lua. */
    Lua = luaL_newstate();
    luaL_openlibs(Lua);

    /**
     * Bootstrapping function to support complex scripting.
     *
     * Allow scripts to act on the program state using the command line like this::
     *
     *     -- Convert an example file from Tajima dst to Brother pes format.
     *     cmd("open example.dst")
     *     cmd("saveas example.pes")
     */
    lua_register(Lua, "cmd", cmd_f);
    lua_register(Lua, "get", get_f);
    lua_register(Lua, "set", set_f);

    return true;
}

/**
 * @brief Free up the memory used by the scripting environment.
 *
 * This is necessary since all the script variables and headers are only
 * present in this file. Currently just the Lua state.
 */
void
script_env_free(void)
{
    lua_close(Lua);
}

/**
 * @note This has to run after script_env_boot because the command_map is
 *       populated by the script files called by it.
 *
 * @todo Set What's This Context Help to statustip for now so there is some infos there.
 * Make custom whats this context help popup with more descriptive help than just
 * the status bar/tip one liner(short but not real long) with a hyperlink in the custom popup
 * at the bottom to open full help file description. Ex: like wxPython AGW's SuperToolTip.
 *
 * @todo Finish All Commands ... <.<
 */
void
MainWindow::createAllActions()
{
    qDebug("Creating All Actions...");

    for (int i=0; i<(int)command_map.size(); i++) {
        QString tooltip = tr(qPrintable(command_map[i].tooltip));
        QString statustip = tr(qPrintable(command_map[i].statustip));
        QAction *ACTION = new QAction(createIcon(command_map[i].icon),
            tooltip, this);
        ACTION->setStatusTip(statustip);
        ACTION->setObjectName(command_map[i].icon);
        ACTION->setWhatsThis(statustip);

        if (command_map[i].shortcut != "") {
            ACTION->setShortcut(command_map[i].shortcut);
        }
        if (command_map[i].checkable != 0) {
            ACTION->setCheckable(true);
            /** @fixme connect(ACTION, SIGNAL(toggled(bool)), this, SLOT(setTextBold(bool))); */
        }

        connect(ACTION, &QAction::triggered, this,
            [=]() { cmd(qPrintable(command_map[i].command)); } );

        actionHash.insert(command_map[i].icon, ACTION);
    }

    actionHash.value("windowclose")->setEnabled(numOfDocs > 0);
    actionHash.value("designdetails")->setEnabled(numOfDocs > 0);
}

