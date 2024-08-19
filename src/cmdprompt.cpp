/*
 * Embroidermodder 2.
 *
 * Copyright 2011-2024 The Embroidermodder Team
 * Embroidermodder 2 is Open Source Software, see LICENSE.md for licensing terms.
 *
 * Read the reference manual (https://www.libembroidery.org/downloads/emrm.pdf)
 * for advice on altering this file.
 *
 * MainWindow
 */

#include "embroidermodder.h"

StringMap aliases[MAX_ALIASES];
std::unordered_map<std::string, std::string> aliasHash;

QString curText;
QString defaultPrefix;
QString prefix;

QString lastCmd;
QString curCmd;

QString prompt_color_;
QString prompt_selection_bg_color_;
QString prompt_bg_color_;
QString prompt_selection_color_;

int pressY;
int releaseY;
int moveY;
int tmpHeight;
CmdPromptHistory* promptHistory;
QVBoxLayout* promptVBoxLayout;
QFrame* promptDivider;

CmdPromptSplitter* promptSplitter;
CmdPromptInput* promptInput;

QTimer* blinkTimer;
bool blinkState;

void
setHistory(const QString& txt)
{
    promptHistory->setHtml(txt);
    promptHistory->moveCursor(QTextCursor::End, QTextCursor::MoveAnchor);
}

/* . */
CmdPrompt::CmdPrompt(QWidget* parent) : QWidget(parent)
{
    debug_message("CmdPrompt Constructor");
    setObjectName("Command Prompt");

    promptInput = new CmdPromptInput(this);
    promptHistory = new CmdPromptHistory();
    promptDivider = new QFrame(this);
    promptVBoxLayout = new QVBoxLayout(this);

    promptSplitter = new CmdPromptSplitter(this);

    this->setFocusProxy(promptInput);
    promptHistory->setFocusProxy(promptInput);

    promptDivider->setLineWidth(1);
    promptDivider->setFrameStyle(QFrame::HLine);
    promptDivider->setMaximumSize(QWIDGETSIZE_MAX, 1);

    promptVBoxLayout->addWidget(promptSplitter);
    promptVBoxLayout->addWidget(promptHistory);
    promptVBoxLayout->addWidget(promptDivider);
    promptVBoxLayout->addWidget(promptInput);

    promptVBoxLayout->setSpacing(0);
    promptVBoxLayout->setContentsMargins(0,0,0,0);

    this->setLayout(promptVBoxLayout);

    prompt_color_ = "#000000"; /* Match --------------------| */
    prompt_selection_bg_color_ = "#000000"; /* Match -------| */
    prompt_bg_color_ = "#FFFFFF";
    prompt_selection_color_ = "#FFFFFF";
    strcpy(prompt_font_family.setting, "Monospace");
    strcpy(prompt_font_style.setting, "normal");
    prompt_font_size.setting = 12;

    updateStyle();

    blinkState = false;
    blinkTimer = new QTimer(this);
    connect(blinkTimer, SIGNAL(timeout()), this, SLOT(blink()));

    this->show();

    connect(promptInput, SIGNAL(showSettings()), this, SIGNAL(showSettings()));
}

/* . */
void
CmdPrompt::floatingChanged(bool isFloating)
{
    qDebug("CmdPrompt floatingChanged(%d)", isFloating);
    if (isFloating) {
        promptSplitter->hide();
    }
    else {
        promptSplitter->show();
    }
}

/* . */
void
CmdPrompt::saveHistory(const QString& fileName, bool html)
{
    debug_message("CmdPrompt saveHistory");
    QFile file(fileName);
    if (!file.open(QIODevice::WriteOnly | QIODevice::Text)) {
        return;
    }

    /* TODO: save during input in case of crash. */
    QTextStream output(&file);
    if (html) {
        output << promptHistory->toHtml();
    }
    else {
        output << promptHistory->toPlainText();
    }
}

/* . */
void
CmdPrompt::alert(const QString& txt)
{
    QString alertTxt = "<font color=\"red\">" + txt + "</font>";
    /* TODO: Make the alert color customizable. */
    setPrefix(alertTxt);
    appendHistory("");
}

/* . */
void
start_blinking(void)
{
    blinkTimer->start(750);
    isBlinking = true;
}

/* . */
void
stop_blinking(void)
{
    blinkTimer->stop();
    isBlinking = false;
}

/* . */
void
CmdPrompt::blink()
{
    blinkState = !blinkState;
    if (blinkState) {
        debug_message("CmdPrompt blink1");
    }
    else {
        debug_message("CmdPrompt blink0");
    }
}

/* . */
void
CmdPrompt::setPromptTextColor(const QColor& color)
{
    prompt_color_ = color.name();
    prompt_selection_bg_color_ = color.name();
    updateStyle();
}

/* . */
void
CmdPrompt::setPromptBackgroundColor(const QColor& color)
{
    prompt_bg_color_ = color.name();
    prompt_selection_color_ = color.name();
    updateStyle();
}

/* . */
void
CmdPrompt::setPromptFontFamily(const QString& family)
{
    strcpy(prompt_font_family.setting, qPrintable(family));
    updateStyle();
}

/* . */
void
CmdPrompt::setPromptFontStyle(const QString& style)
{
    strcpy(prompt_font_style.setting, qPrintable(style));
    updateStyle();
}

/* . */
void
CmdPrompt::setPromptFontSize(int size)
{
    prompt_font_size.setting = size;
    updateStyle();
}

/* . */
void
CmdPrompt::updateStyle()
{
    char style_string[2200];
    sprintf(style_string,
        "QTextBrowser,QLineEdit{" \
        "    color:%s;" \
        "    background-color:%s;" \
        "    selection-color:%s;" \
        "    selection-background-color:%s;" \
        "    font-family:%s;" \
        "    font-style:%s;" \
        "    font-size:%dpx;" \
        "}",
        qPrintable(prompt_color_),
        qPrintable(prompt_bg_color_),
        qPrintable(prompt_selection_color_),
        qPrintable(prompt_selection_bg_color_),
        prompt_font_family.setting,
        prompt_font_style.setting,
        prompt_font_size.setting);

    this->setStyleSheet(QString(style_string));
}

/* . */
void
CmdPrompt::setPrefix(const QString& txt)
{
    prefix = txt;
    curText = txt;
    promptInput->setText(txt);
}

/* . */
CmdPromptSplitter::CmdPromptSplitter(QWidget* parent) : QSplitter(parent)
{
    debug_message("CmdPromptSplitter Constructor");
    setObjectName("Command Prompt Splitter");

    setOrientation(Qt::Vertical);
    /* NOTE: Add two empty widgets just so we have a handle to grab. */
    addWidget(new QWidget(this)); addWidget(new QWidget(this));
}

/* . */
QSplitterHandle*
CmdPromptSplitter::createHandle()
{
    return new CmdPromptHandle(orientation(), this);
}

/* . */
CmdPromptHandle::CmdPromptHandle(Qt::Orientation orientation, QSplitter* parent) : QSplitterHandle(orientation, parent)
{
    debug_message("CmdPromptHandle Constructor");
    setObjectName("Command Prompt Handle");
}

/* . */
void
CmdPromptHandle::mousePressEvent(QMouseEvent* e)
{
    pressY = e->globalPosition().y();
    tmpHeight = promptHistory->height();
}

/* . */
void
CmdPromptHandle::mouseReleaseEvent(QMouseEvent* e)
{
    releaseY = e->globalPosition().y();
    tmpHeight = promptHistory->height();
}

/* . */
void
CmdPromptHandle::mouseMoveEvent(QMouseEvent* e)
{
    moveY = e->globalPosition().y();
    int delta = moveY - pressY;
    int newHeight = tmpHeight - delta;
    if (newHeight < 0) {
        newHeight = 0;
    }
    promptHistory->setMaximumHeight(newHeight);
}

/* . */
CmdPromptHistory::CmdPromptHistory(QWidget* parent) : QTextBrowser(parent)
{
    debug_message("CmdPromptHistory Constructor");
    setObjectName("Command Prompt History");

    int initHeight = 19*3; /* (approximately three lines of text) */

    this->setFrameStyle(QFrame::NoFrame);
    this->setMaximumHeight(initHeight);
    this->setMinimumWidth(200); /* TODO: use float/dock events to set minimum size so when floating, it isn't smooshed. */

    this->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOn);
}

/* . */
QString
CmdPromptHistory::applyFormatting(const QString& txt, int prefixLength)
{
    QString prefix = txt.left(prefixLength);
    QString usrtxt = txt.right(txt.length()-prefixLength);

    int start = -1;
    int stop = -1;

    /* Bold Prefix */
    prefix.prepend("<b>");
    prefix.append("</b>");

    /* Keywords */
    start = prefix.indexOf('[');
    stop = prefix.lastIndexOf(']');
    if (start != -1 && stop != -1 && start < stop) {
        for (int i = stop; i >= start; i--) {
            if (prefix.at(i) == ']') {
                prefix.insert(i, "</font>");
            }
            if (prefix.at(i) == '[') {
                prefix.insert(i+1, "<font color=\"#0095FF\">");
            }
            if (prefix.at(i) == '/') {
                prefix.insert(i+1, "<font color=\"#0095FF\">");
                prefix.insert(i, "</font>");
            }
        }
    }

    /* Default Values */
    start = prefix.indexOf('{');
    stop = prefix.lastIndexOf('}');
    if (start != -1 && stop != -1 && start < stop) {
        for (int i = stop; i >= start; i--) {
            if (prefix.at(i) == '}') {
                prefix.insert(i, "</font>");
            }
            if (prefix.at(i) == '{') {
                prefix.insert(i+1, "<font color=\"#00AA00\">");
            }
        }
    }

    return prefix + usrtxt;
}

/* . */
void
appendHistory(QString txt)
{
    if (txt.isNull()) {
        txt = curText;
    }

    QString formatStr = promptHistory->applyFormatting(txt, (int)prefix.length());
    promptHistory->append(formatStr);
    /* emit historyAppended(formatStr); */
    promptHistory->moveCursor(QTextCursor::End, QTextCursor::MoveAnchor);
}

/* . */
void
CmdPromptHistory::contextMenuEvent(QContextMenuEvent* event)
{
    QMenu* menu = createStandardContextMenu();
    menu->addSeparator();
    /* TODO: Extra stuff */
    menu->exec(event->globalPos());
    delete menu;
}

/* . */
CmdPromptInput::CmdPromptInput(QWidget* parent) : QLineEdit(parent)
{
    debug_message("CmdPromptInput Constructor");
    setObjectName("Command Prompt Input");

    defaultPrefix = tr("Command: ");
    prefix = defaultPrefix;
    curText = prefix;

    lastCmd = "help";
    curCmd = "help";
    cmdActive = false;

    rapidFireEnabled = false;

    isBlinking = false;

    this->setText(prefix);
    this->setFrame(false);
    this->setMaxLength(266);
    this->setMaximumSize(5000, 25);
    this->setDragEnabled(false);

    connect(this, SIGNAL(cursorPositionChanged(int, int)), this, SLOT(checkCursorPosition(int, int)));
    connect(this, SIGNAL(textEdited(const QString&)), this, SLOT(checkEditedText(const QString&)));
    connect(this, SIGNAL(textChanged(const QString&)), this, SLOT(checkChangedText(const QString&)));
    connect(this, SIGNAL(selectionChanged()), this, SLOT(checkSelection()));

    this->installEventFilter(this);
    this->setFocus(Qt::OtherFocusReason);

    applyFormatting();
}

/* . */
void
CmdPromptInput::endCommand()
{
    debug_message("CmdPromptInput endCommand");
    lastCmd = curCmd;
    cmdActive = false;
    rapidFireEnabled = false;
    stop_blinking();

    prefix = defaultPrefix;
    clear();
}

/*
 */
void
CmdPromptInput::processInput(const QChar& rapidChar)
{
    debug_message("processInput");

    updateCurrentText(curText);

    QString cmdtxt(curText);
    cmdtxt.replace(0, prefix.length(), "");
    if (!rapidFireEnabled) {
        cmdtxt = cmdtxt.toLower();
    }

    if (cmdActive) {
        if (rapidFireEnabled) {
            /*
            if (rapidChar == Qt::Key_Enter || rapidChar == Qt::Key_Return) {
                appendHistory(curText);
                runCommand(curCmd, "RAPID_ENTER");
                curText.clear();
                clear();
                return;
            }
            else if (rapidChar == Qt::Key_Space) {
                updateCurrentText(curText + " ");
                runCommand(curCmd, cmdtxt + " ");
                return;
            }
            else */ {
                /* runCommand(curCmd, cmdtxt); */
                return;
            }
        }
        else {
            appendHistory(curText);
            /* runCommand(curCmd, cmdtxt); */
        }
    }
    else {
        auto found = aliasHash.find(cmdtxt.toStdString());
        if (found != aliasHash.end()) {
            cmdActive = true;
            lastCmd = curCmd;
            std::string cmd = aliasHash[cmdtxt.toStdString()];
            curCmd = QString(cmd.c_str());
            appendHistory(curText);
            _main->runCommandPrompt(curCmd);
        }
        else if (cmdtxt.isEmpty()) {
            cmdActive = true;
            appendHistory(curText);
            /* Rerun the last successful command. */
            _main->runCommandPrompt(lastCmd);
        }
        else {
            appendHistory(curText + "<br/><font color=\"red\">Unknown command \"" + cmdtxt + "\". Press F1 for help.</font>");
        }
    }

    if (!rapidFireEnabled) {
        clear();
    }
}

/* . */
void
CmdPromptInput::checkSelection()
{
    /* debug_message("CmdPromptInput::checkSelection"); */
    if (this->hasSelectedText()) {
        this->deselect();
    }
}

/* . */
void
CmdPromptInput::checkCursorPosition(int /*oldpos*/, int newpos)
{
    /* qDebug("CmdPromptInput::checkCursorPosition - %d %d", oldpos, newpos); */
    if (this->hasSelectedText()) {
        this->deselect();
    }
    if (newpos < prefix.length()) {
        this->setCursorPosition(prefix.length());
    }
}

/* . */
void
CmdPromptInput::changeFormatting(const QList<QTextLayout::FormatRange>& formats)
{
    QList<QInputMethodEvent::Attribute> attributes;
    foreach(const QTextLayout::FormatRange& range, formats) {
        QInputMethodEvent::AttributeType type = QInputMethodEvent::TextFormat;
        int start = range.start - this->cursorPosition();
        int length = range.length;
        QVariant value = range.format;
        attributes.append(QInputMethodEvent::Attribute(type, start, length, value));
    }
    QInputMethodEvent event(QString(), attributes);
    QCoreApplication::sendEvent(this, &event);
}

/* . */
void
CmdPromptInput::clearFormatting()
{
    changeFormatting(QList<QTextLayout::FormatRange>());
}

/* . */
void
CmdPromptInput::applyFormatting()
{
    int prefixLength = prefix.length();

    int start = -1;
    int stop = -1;

    QList<QTextLayout::FormatRange> formats;

    /* Bold Prefix */
    QTextCharFormat formatPrefix;
    formatPrefix.setFontWeight(QFont::Bold);
    QTextLayout::FormatRange rangePrefix;
    rangePrefix.start = 0;
    rangePrefix.length = prefixLength;
    rangePrefix.format = formatPrefix;
    formats.append(rangePrefix);

    /* Keywords */
    start = prefix.indexOf('[');
    stop = prefix.lastIndexOf(']');
    if (start != -1 && stop != -1 && start < stop) {
        QTextCharFormat formatKeyword;
        formatKeyword.setFontWeight(QFont::Bold);
        formatKeyword.setForeground(QColor("#0095FF"));

        int rangeStart = -1;
        int rangeStop = -1;
        for (int i = stop; i >= start; i--) {
            if (prefix.at(i) == ']') {
                rangeStop = i;
            }
            if (prefix.at(i) == '[' || prefix.at(i) == '/') {
                rangeStart = i;

                QTextLayout::FormatRange rangeKeyword;
                rangeKeyword.start = rangeStart+1;
                rangeKeyword.length = rangeStop-rangeStart-1;
                rangeKeyword.format = formatKeyword;
                formats.append(rangeKeyword);

                rangeStop = i;
            }
        }
    }

    /* Default Values */
    start = prefix.indexOf('{');
    stop = prefix.lastIndexOf('}');
    if (start != -1 && stop != -1 && start < stop) {
        QTextCharFormat formatKeyword;
        formatKeyword.setFontWeight(QFont::Bold);
        formatKeyword.setForeground(QColor("#00AA00"));

        int rangeStart = -1;
        int rangeStop = -1;
        for (int i = stop; i >= start; i--) {
            if (prefix.at(i) == '}') {
                rangeStop = i;
            }
            if (prefix.at(i) == '{') {
                rangeStart = i;

                QTextLayout::FormatRange rangeKeyword;
                rangeKeyword.start = rangeStart+1;
                rangeKeyword.length = rangeStop-rangeStart-1;
                rangeKeyword.format = formatKeyword;
                formats.append(rangeKeyword);

                rangeStop = i;
            }
        }
    }

    changeFormatting(formats);
}

/* . */
void
CmdPromptInput::updateCurrentText(const QString& txt)
{
    int cursorPos = cursorPosition();
    if (!txt.startsWith(prefix)) {
        if (txt.length() < prefix.length())
            this->setText(prefix);
        else if (txt.length() != prefix.length())
            this->setText(prefix + txt);
        else
            this->setText(curText);
    }
    else {
        /* input is okay so update curText */
        curText = txt;
        this->setText(curText);
    }
    setCursorPosition(cursorPos);

    applyFormatting();
}

/* . */
void
CmdPromptInput::checkEditedText(const QString& txt)
{
    updateCurrentText(txt);

    if (rapidFireEnabled)
        processInput();
}

/* . */
void
CmdPromptInput::checkChangedText(const QString& txt)
{
    updateCurrentText(txt);
}

/* . */
void
CmdPromptInput::copyClip()
{
    QString copyText = curText.remove(0, prefix.length());
    qApp->clipboard()->setText(copyText);
}

/* . */
void
CmdPromptInput::pasteClip()
{
    paste();
}

/* . */
void
CmdPromptInput::contextMenuEvent(QContextMenuEvent* event)
{
    QMenu menu(this);

    QAction* copyAction = new QAction("&Copy", this);
    copyAction->setStatusTip("Copies the command prompt text to the clipboard.");
    connect(copyAction, SIGNAL(triggered()), this, SLOT(copyClip()));
    menu.addAction(copyAction);

    QAction* pasteAction = new QAction("&Paste", this);
    pasteAction->setStatusTip("Inserts the clipboard's text into the command prompt at the cursor position.");
    connect(pasteAction, SIGNAL(triggered()), this, SLOT(pasteClip()));
    menu.addAction(pasteAction);

    menu.addSeparator();

    QAction* settingsAction = new QAction("&Settings...", this);
    settingsAction->setStatusTip("Opens settings for the command prompt.");
    connect(settingsAction, SIGNAL(triggered()), this, SIGNAL(showSettings()));
    menu.addAction(settingsAction);

    menu.exec(event->globalPos());
}

/* The key_state is set to false again by whatever uses the sequence
 * so we can control when the keypress is eaten. Otherwise, if the release happens
 * it manually sets it to false.
 */
bool
CmdPromptInput::eventFilter(QObject* obj, QEvent* event)
{
    if (event->type() == QEvent::KeyPress) {
        if (isBlinking) {
            stop_blinking();
        }

        QKeyEvent* pressedKey = (QKeyEvent*)event;

        /* NOTE: These shortcuts need to be caught since QLineEdit uses them. */
        if (pressedKey->matches(QKeySequence::Cut)) {
            pressedKey->accept();
            key_state[CUT_SEQUENCE] = true;
            return true;
        }
        else if (pressedKey->matches(QKeySequence::Copy)) {
            pressedKey->accept();
            key_state[COPY_SEQUENCE] = true;
            return true;
        }
        else if (pressedKey->matches(QKeySequence::Paste)) {
            pressedKey->accept();
            key_state[PASTE_SEQUENCE] = true;
            return true;
        }
        else if (pressedKey->matches(QKeySequence::SelectAll)) {
            pressedKey->accept();
            key_state[SELECT_ALL_SEQUENCE] = true;
            return true;
        }
        else if (pressedKey->matches(QKeySequence::Undo)) {
            pressedKey->accept();
            key_state[UNDO_SEQUENCE] = true;
            return true;
        }
        else if (pressedKey->matches(QKeySequence::Redo)) {
            pressedKey->accept();
            key_state[REDO_SEQUENCE] = true;
            return true;
        }

        int key = pressedKey->key();
        switch (key) {
            case Qt::Key_Enter:
            case Qt::Key_Return:
                pressedKey->accept();
                processInput(QChar('\n'));
                return true;
            case Qt::Key_Space:
                pressedKey->accept();
                processInput(QChar(' '));
                return true;
            case Qt::Key_Delete:
                pressedKey->accept();
                del();
                key_state[DELETE_KEY] = true;
                return true;
            case Qt::Key_Tab:
                pressedKey->accept();
                key_state[TAB_KEY] = true;
                return true;
            case Qt::Key_Escape:
                pressedKey->accept();
                prefix = defaultPrefix;
                clear();
                appendHistory(curText + tr("*Cancel*"));
                key_state[ESCAPE_KEY] = true;
                return true;
            case Qt::Key_Up:
                pressedKey->accept();
                key_state[UP_KEY] = true;
                return true;
            case Qt::Key_Down:
                pressedKey->accept();
                key_state[DOWN_KEY] = true;
                return true;
            case Qt::Key_F1:
                pressedKey->accept();
                key_state[F1_KEY] = true;
                return true;
            case Qt::Key_F2:
                pressedKey->accept();
                key_state[F2_KEY] = true;
                return true;
            case Qt::Key_F3:
                pressedKey->accept();
                key_state[F3_KEY] = true;
                return true;
            case Qt::Key_F4:
                pressedKey->accept();
                key_state[F4_KEY] = true;
                return true;
            case Qt::Key_F5:
                pressedKey->accept();
                key_state[F5_KEY] = true;
                return true;
            case Qt::Key_F6:
                pressedKey->accept();
                key_state[F6_KEY] = true;
                return true;
            case Qt::Key_F7:
                pressedKey->accept();
                key_state[F7_KEY] = true;
                return true;
            case Qt::Key_F8:
                pressedKey->accept();
                key_state[F8_KEY] = true;
                return true;
            case Qt::Key_F9:
                pressedKey->accept();
                key_state[F9_KEY] = true;
                return true;
            case Qt::Key_F10:
                pressedKey->accept();
                key_state[F10_KEY] = true;
                return true;
            case Qt::Key_F11:
                pressedKey->accept();
                key_state[F11_KEY] = true;
                return true;
            case Qt::Key_F12:
                pressedKey->accept();
                key_state[F12_KEY] = true;
                return true;
            case Qt::Key_Shift: {
                /* we don't want to eat it, we just want to keep track of it */
                pressedKey->ignore();
                key_state[SHIFT_KEY] = true;
                break;
            }
            default: {
                pressedKey->ignore();
                break;
            }
        }
    }

    if (event->type() == QEvent::KeyRelease) {
        QKeyEvent* releasedKey = (QKeyEvent*)event;
        int key = releasedKey->key();
        /* We don't want to eat it, we just want to keep track of it. */
        releasedKey->ignore();
        switch(key) {
        case Qt::Key_Delete:
            key_state[DELETE_KEY] = false;
            break;
        case Qt::Key_Tab:
            key_state[TAB_KEY] = false;
            break;
        case Qt::Key_Escape:
            key_state[ESCAPE_KEY] = false;
            break;
        case Qt::Key_Up:
            key_state[UP_KEY] = false;
            break;
        case Qt::Key_Down:
            key_state[DOWN_KEY] = false;
            break;
        case Qt::Key_F1:
            key_state[F1_KEY] = false;
            break;
        case Qt::Key_F2:
            key_state[F2_KEY] = false;
            break;
        case Qt::Key_F3:
            key_state[F3_KEY] = false;
            break;
        case Qt::Key_F4:
            key_state[F4_KEY] = false;
            break;
        case Qt::Key_F5:
            key_state[F5_KEY] = false;
            break;
        case Qt::Key_F6:
            key_state[F6_KEY] = false;
            break;
        case Qt::Key_F7:
            key_state[F7_KEY] = false;
            break;
        case Qt::Key_F8:
            key_state[F8_KEY] = false;
            break;
        case Qt::Key_F9:
            key_state[F9_KEY] = false;
            break;
        case Qt::Key_F10:
            key_state[F10_KEY] = false;
            break;
        case Qt::Key_F11:
            key_state[F11_KEY] = false;
            break;
        case Qt::Key_F12:
            key_state[F12_KEY] = false;
            break;
        case Qt::Key_Shift:
            key_state[SHIFT_KEY] = false;
            break;
        default:
            break;
        }
    }
    return QObject::eventFilter(obj, event);
}
