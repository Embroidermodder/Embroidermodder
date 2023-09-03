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

/* CmdPrompt::CmdPrompt
 * parent
 */
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

    styleHash = new QHash<QString, QString>();
    styleHash->insert("color",                      "#000000"); // Match -------|
    styleHash->insert("background-color",           "#FFFFFF"); //              |
    styleHash->insert("selection-color",            "#FFFFFF"); //              |
    styleHash->insert("selection-background-color", "#000000"); // Match -------|
    styleHash->insert("font-family",              "Monospace");
    styleHash->insert("font-style",                  "normal");
    styleHash->insert("font-size",                     "12px");

    updateStyle();

    blinkState = false;
    blinkTimer = new QTimer(this);
    connect(blinkTimer, SIGNAL(timeout()), this, SLOT(blink()));

    this->show();

    connect(promptInput, SIGNAL(stopBlinking()), this, SLOT(stopBlinking()));
    connect(promptInput, SIGNAL(appendHistory(QString,int)), promptHistory, SLOT(appendHistory(QString,int)));
    connect(this, SIGNAL(appendTheHistory(QString,int)), promptHistory, SLOT(appendHistory(QString,int)));

    //For use outside of command prompt
    connect(promptInput, SIGNAL(startCommand(QString)), this, SIGNAL(startCommand(QString)));
    connect(promptInput, SIGNAL(runCommand(QString,QString)), this, SIGNAL(runCommand(QString,QString)));
    connect(promptInput, SIGNAL(deletePressed()), this, SIGNAL(deletePressed()));
    connect(promptInput, SIGNAL(tabPressed()), this, SIGNAL(tabPressed()));
    connect(promptInput, SIGNAL(escapePressed()), this, SIGNAL(escapePressed()));
    connect(promptInput, SIGNAL(upPressed()), this, SIGNAL(upPressed()));
    connect(promptInput, SIGNAL(downPressed()), this, SIGNAL(downPressed()));
    connect(promptInput, SIGNAL(F1Pressed()), this, SIGNAL(F1Pressed()));
    connect(promptInput, SIGNAL(F2Pressed()), this, SIGNAL(F2Pressed()));
    connect(promptInput, SIGNAL(F3Pressed()), this, SIGNAL(F3Pressed()));
    connect(promptInput, SIGNAL(F4Pressed()), this, SIGNAL(F4Pressed()));
    connect(promptInput, SIGNAL(F5Pressed()), this, SIGNAL(F5Pressed()));
    connect(promptInput, SIGNAL(F6Pressed()), this, SIGNAL(F6Pressed()));
    connect(promptInput, SIGNAL(F7Pressed()), this, SIGNAL(F7Pressed()));
    connect(promptInput, SIGNAL(F8Pressed()), this, SIGNAL(F8Pressed()));
    connect(promptInput, SIGNAL(F9Pressed()), this, SIGNAL(F9Pressed()));
    connect(promptInput, SIGNAL(F10Pressed()), this, SIGNAL(F10Pressed()));
    connect(promptInput, SIGNAL(F11Pressed()), this, SIGNAL(F11Pressed()));
    connect(promptInput, SIGNAL(F12Pressed()), this, SIGNAL(F12Pressed()));
    connect(promptInput, SIGNAL(cutPressed()), this, SIGNAL(cutPressed()));
    connect(promptInput, SIGNAL(copyPressed()), this, SIGNAL(copyPressed()));
    connect(promptInput, SIGNAL(pastePressed()), this, SIGNAL(pastePressed()));
    connect(promptInput, SIGNAL(selectAllPressed()), this, SIGNAL(selectAllPressed()));
    connect(promptInput, SIGNAL(undoPressed()), this, SIGNAL(undoPressed()));
    connect(promptInput, SIGNAL(redoPressed()), this, SIGNAL(redoPressed()));

    connect(promptInput, SIGNAL(shiftPressed()), this, SIGNAL(shiftPressed()));
    connect(promptInput, SIGNAL(shiftReleased()), this, SIGNAL(shiftReleased()));

    connect(promptInput, SIGNAL(showSettings()), this, SIGNAL(showSettings()));

    connect(promptHistory, SIGNAL(historyAppended(QString)), this, SIGNAL(historyAppended(QString)));
}

/* Destroy CmdPrompt object. */
CmdPrompt::~CmdPrompt()
{
    delete styleHash;
}

/* CmdPrompt::floatingChanged
 * isFloating
 */
void
CmdPrompt::floatingChanged(bool isFloating)
{
    qDebug("CmdPrompt floatingChanged(%d)", isFloating);
    if (isFloating) promptSplitter->hide();
    else           promptSplitter->show();
}

/* CmdPrompt::saveHistory
 * fileName
 * html
 */
void
CmdPrompt::saveHistory(QString  fileName, bool html)
{
    debug_message("CmdPrompt saveHistory");
    QFile file(fileName);
    if (!file.open(QIODevice::WriteOnly | QIODevice::Text))
        return;

    //TODO: save during input in case of crash
    QTextStream output(&file);
    if (html) {
        output << promptHistory->toHtml();
    }
    else {
        output << promptHistory->toPlainText();
    }
}

/* CmdPrompt::alert
 * txt
 */
void
CmdPrompt::alert(QString txt)
{
    QString alertTxt = "<font color=\"red\">" + txt + "</font>"; //TODO: Make the alert color customizable
    setPrefix(alertTxt);
    appendHistory(QString());
}

/* Start blinking the prompt cursor. */
void
CmdPrompt::startBlinking()
{
    blinkTimer->start(750);
    promptInput->isBlinking = true;
}

/* Stop blinking the prompt cursor. */
void
CmdPrompt::stopBlinking()
{
    blinkTimer->stop();
    promptInput->isBlinking = false;
}

/* Blink the prompt cursor. */
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

/* Set prompt text color. */
void
CmdPrompt::setPromptTextColor(const QColor& color)
{
    styleHash->insert("color", color.name());
    styleHash->insert("selection-background-color", color.name());
    updateStyle();
}

/* Set prompt background color. */
void
CmdPrompt::setPromptBackgroundColor(const QColor& color)
{
    styleHash->insert("background-color", color.name());
    styleHash->insert("selection-color", color.name());
    updateStyle();
}

/**
 * @brief CmdPrompt::setPromptFontFamily
 * @param family
 */
void
CmdPrompt::setPromptFontFamily(QString  family)
{
    styleHash->insert("font-family", family);
    updateStyle();
}

/**
 * @brief CmdPrompt::setPromptFontStyle
 * @param style
 */
void
CmdPrompt::setPromptFontStyle(QString  style)
{
    styleHash->insert("font-style", style);
    updateStyle();
}

/**
 * @brief CmdPrompt::setPromptFontSize
 * @param size
 */
void
CmdPrompt::setPromptFontSize(int size)
{
    styleHash->insert("font-size", QString().setNum(size).append("px"));
    updateStyle();
}

/**
 * @brief CmdPrompt::updateStyle
 */
void
CmdPrompt::updateStyle()
{
    QString style = "QTextBrowser,QLineEdit{";

    QHashIterator<QString, QString> i(*styleHash);
    while (i.hasNext()) {
        i.next();
        style.append(i.key() + ":" + i.value() + ";");
    }
    style.append("}");

    this->setStyleSheet(style);
}

/**
 * @brief CmdPrompt::appendHistory
 * @param txt
 */
void
CmdPrompt::appendHistory(QString  txt)
{
    if (txt.isNull()) {
        emit appendTheHistory(promptInput->curText, promptInput->prefix.length());
        return;
    }
    debug_message("CmdPrompt - appendHistory()");
    emit appendTheHistory(txt, promptInput->prefix.length());
}

/**
 * @brief CmdPrompt::setPrefix
 * @param txt
 */
void
CmdPrompt::setPrefix(QString txt)
{
    promptInput->prefix = txt;
    promptInput->curText = txt;
    promptInput->setText(txt);
}

/**
 * @brief CmdPromptSplitter::CmdPromptSplitter
 * @param parent
 */
CmdPromptSplitter::CmdPromptSplitter(QWidget* parent) : QSplitter(parent)
{
    debug_message("CmdPromptSplitter Constructor");
    setObjectName("Command Prompt Splitter");

    setOrientation(Qt::Vertical);
    //NOTE: Add two empty widgets just so we have a handle to grab
    addWidget(new QWidget(this)); addWidget(new QWidget(this));

    connect(this, SIGNAL(pressResizeHistory(int)),   parent, SLOT(startResizingTheHistory(int)));
    connect(this, SIGNAL(releaseResizeHistory(int)), parent, SLOT(stopResizingTheHistory(int)));
    connect(this, SIGNAL(moveResizeHistory(int)),    parent, SLOT(resizeTheHistory(int)));
}

/**
 * @brief CmdPromptSplitter::~CmdPromptSplitter
 */
CmdPromptSplitter::~CmdPromptSplitter()
{
}

/* Create handle for this command prompt. */
QSplitterHandle* CmdPromptSplitter::createHandle()
{
    return new CmdPromptHandle(orientation(), this);
}

/* Create command prompt handle object. */
CmdPromptHandle::CmdPromptHandle(Qt::Orientation orientation, QSplitter* parent) : QSplitterHandle(orientation, parent)
{
    debug_message("CmdPromptHandle Constructor");
    setObjectName("Command Prompt Handle");

    connect(this, SIGNAL(handlePressed(int)),  parent, SIGNAL(pressResizeHistory(int)));
    connect(this, SIGNAL(handleReleased(int)), parent, SIGNAL(releaseResizeHistory(int)));
    connect(this, SIGNAL(handleMoved(int)),    parent, SIGNAL(moveResizeHistory(int)));
}

/* Destroy the command prompt handle object. */
CmdPromptHandle::~CmdPromptHandle()
{
}

/* Process mouse press in the command prompt handle context. */
void
CmdPromptHandle::mousePressEvent(QMouseEvent* e)
{
    pressY = e->globalPosition().y();
    emit handlePressed(pressY);
}

/* Process mouse release in the command prompt handle context. */
void
CmdPromptHandle::mouseReleaseEvent(QMouseEvent* e)
{
    releaseY = e->globalPosition().y();
    emit handleReleased(releaseY);
}

/* Process mouse move in the command prompt handle context. */
void
CmdPromptHandle::mouseMoveEvent(QMouseEvent* e)
{
    moveY = e->globalPosition().y();
    int dY = moveY - pressY;
    emit handleMoved(dY);
}

/**
 * @brief CmdPromptHistory::CmdPromptHistory
 * @param parent The QWidget that it sits in.
 */
CmdPromptHistory::CmdPromptHistory(QWidget* parent) : QTextBrowser(parent)
{
    debug_message("CmdPromptHistory Constructor");
    setObjectName("Command Prompt History");

    int initHeight = 57; // 19*3 (approximately three lines of text)

    this->setFrameStyle(QFrame::NoFrame);
    this->setMaximumHeight(initHeight);
    this->setMinimumWidth(200); // TODO: use float/dock events to set minimum size so when floating, it isn't smooshed.

    this->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOn);
}

/* CmdPromptHistory::~CmdPromptHistory */
CmdPromptHistory::~CmdPromptHistory()
{
}

/* CmdPromptHistory::applyFormatting
 * txt
 * prefixLength
 */
QString CmdPromptHistory::applyFormatting(QString  txt, int prefixLength)
{
    QString prefix = txt.left(prefixLength);
    QString usrtxt = txt.right(txt.length()-prefixLength);

    int start = -1;
    int stop = -1;

    //Bold Prefix
    prefix.prepend("<b>");
    prefix.append("</b>");

    //Keywords
    start = prefix.indexOf('[');
    stop = prefix.lastIndexOf(']');
    if (start != -1 && stop != -1 && start < stop)
    {
        for (int i = stop; i >= start; i--)
        {
            if (prefix.at(i) == ']')
                prefix.insert(i, "</font>");
            if (prefix.at(i) == '[')
                prefix.insert(i+1, "<font color=\"#0095FF\">");
            if (prefix.at(i) == '/')
            {
                prefix.insert(i+1, "<font color=\"#0095FF\">");
                prefix.insert(i, "</font>");
            }
        }
    }

    //Default Values
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

/* CmdPromptHistory::appendHistory
 * txt
 * prefixLength
 */
void
CmdPromptHistory::appendHistory(QString txt, int prefixLength)
{
    QString formatStr = applyFormatting(txt, prefixLength);
    this->append(formatStr);
    emit historyAppended(formatStr);
    this->moveCursor(QTextCursor::End, QTextCursor::MoveAnchor);
}

/* CmdPromptHistory::startResizeHistory */
void
CmdPromptHistory::startResizeHistory(int /*y*/)
{
    tmpHeight = height();
}

/* stopResizeHistory */
void
CmdPromptHistory::stopResizeHistory(int /*y*/)
{
    tmpHeight = height();
}

/* Resize history to "y" lines. */
void
CmdPromptHistory::resizeHistory(int y)
{
    int newHeight = tmpHeight - y;
    if (newHeight < 0)
        newHeight = 0;
    setMaximumHeight(newHeight);
}

/* contextMenuEvent
 * @param event
 */
void
CmdPromptHistory::contextMenuEvent(QContextMenuEvent* event)
{
    QMenu* menu = createStandardContextMenu();
    menu->addSeparator();
    //TODO: Extra stuff
    menu->exec(event->globalPos());
    delete menu;
}

/**
 * @brief CmdPromptInput::CmdPromptInput
 * @param parent
 */
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

    connect(this, SIGNAL(cursorPositionChanged(int,int)), this, SLOT(checkCursorPosition(int,int)));
    connect(this, SIGNAL(textEdited(QString)), this, SLOT(checkEditedText(QString)));
    connect(this, SIGNAL(textChanged(QString)), this, SLOT(checkChangedText(QString)));
    connect(this, SIGNAL(selectionChanged()), this, SLOT(checkSelection()));

    this->installEventFilter(this);
    this->setFocus(Qt::OtherFocusReason);

    applyFormatting();
}

/* EndCommand */
void
CmdPromptInput::endCommand()
{
    debug_message("CmdPromptInput endCommand");
    lastCmd = curCmd;
    cmdActive = false;
    rapidFireEnabled = false;
    emit stopBlinking();

    prefix = defaultPrefix;
    clear();
}

/* ProcessInput */
void
CmdPromptInput::processInput()
{
    debug_message("CmdPromptInput::processInput");

    updateCurrentText(curText);

    QString cmdtxt(curText);
    cmdtxt.replace(0, prefix.length(), "");
    if (!rapidFireEnabled) {
        cmdtxt = cmdtxt.toLower();
    }

    if (cmdActive) {
        if (rapidFireEnabled) {
            /*
            \todo sort Qt::Return
            if (rapidChar == Qt::Key_Enter || rapidChar == Qt::Key_Return)
            {
                emit appendHistory(curText, prefix.length());
                emit runCommand(curCmd, "RAPID_ENTER");
                curText.clear();
                clear();
                return;
            }
            else if (rapidChar == Qt::Key_Space)
            {
                updateCurrentText(curText + " ");
                emit runCommand(curCmd, cmdtxt + " ");
                return;
            }
            else
            {
                emit runCommand(curCmd, cmdtxt);
                return;
            }
            */
        }
        else {
            emit appendHistory(curText, prefix.length());
            emit runCommand(curCmd, cmdtxt);
        }
    }
    else {
        QString output = QString::fromStdString(actuator(cmdtxt.toStdString()));
        emit appendHistory(curText + output, prefix.length());
    }

    if (!rapidFireEnabled) {
        clear();
    }
}

/**
 * @brief CmdPromptInput::checkSelection
 */
void
CmdPromptInput::checkSelection()
{
    debug_message("CmdPromptInput::checkSelection");
    if (this->hasSelectedText()) {
        this->deselect();
    }
}

/* checkCursorPosition
 * @param oldpos
 * @param newpos
 */
void
CmdPromptInput::checkCursorPosition(int oldpos, int newpos)
{
    qDebug("CmdPromptInput::checkCursorPosition - %d %d", oldpos, newpos);
    if (this->hasSelectedText())
        this->deselect();
    if (newpos < prefix.length())
        this->setCursorPosition(prefix.length());
}

/* changeFormatting
 * @param formats
 */
void
CmdPromptInput::changeFormatting(std::vector<QTextLayout::FormatRange> formats)
{
    QList<QInputMethodEvent::Attribute> attributes;
    for (int i=0; i<(int)(formats.size()); i++) {
        QTextLayout::FormatRange range = formats[i];
        QInputMethodEvent::AttributeType type = QInputMethodEvent::TextFormat;
        int start = range.start - this->cursorPosition();
        int length = range.length;
        QVariant value = range.format;
        attributes.append(QInputMethodEvent::Attribute(type, start, length, value));
    }
    QInputMethodEvent event(QString(), attributes);
    QCoreApplication::sendEvent(this, &event);
}

/* clearFormatting */
void
CmdPromptInput::clearFormatting()
{
    std::vector<QTextLayout::FormatRange> formatting;
    changeFormatting(formatting);
}

/* applyFormatting */
void CmdPromptInput::applyFormatting()
{
    int prefixLength = prefix.length();

    int start = -1;
    int stop = -1;

    std::vector<QTextLayout::FormatRange> formats;

    //Bold Prefix
    QTextCharFormat formatPrefix;
    formatPrefix.setFontWeight(QFont::Bold);
    QTextLayout::FormatRange rangePrefix;
    rangePrefix.start = 0;
    rangePrefix.length = prefixLength;
    rangePrefix.format = formatPrefix;
    formats.push_back(rangePrefix);

    //Keywords
    start = prefix.indexOf('[');
    stop = prefix.lastIndexOf(']');
    if (start != -1 && stop != -1 && start < stop) {
        QTextCharFormat formatKeyword;
        formatKeyword.setFontWeight(QFont::Bold);
        formatKeyword.setForeground(QColor::fromRgb(0x00, 0x95, 0xFF));

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
                formats.push_back(rangeKeyword);

                rangeStop = i;
            }
        }
    }

    //Default Values
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
                formats.push_back(rangeKeyword);

                rangeStop = i;
            }
        }
    }

    changeFormatting(formats);
}

/* Update current text to "txt". */
void
CmdPromptInput::updateCurrentText(QString txt)
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
    else
    {
        // input is okay so update curText
        curText = txt;
        this->setText(curText);
    }
    setCursorPosition(cursorPos);

    applyFormatting();
}

/**
 * @brief CmdPromptInput::checkEditedText
 * @param txt
 */
void
CmdPromptInput::checkEditedText(QString txt)
{
    updateCurrentText(txt);

    if (rapidFireEnabled) {
        processInput();
    }
}

/**
 * @brief CmdPromptInput::checkChangedText
 * @param txt
 */
void
CmdPromptInput::checkChangedText(QString  txt)
{
    updateCurrentText(txt);
}

/**
 * @brief CmdPromptInput::copyClip
 */
void
CmdPromptInput::copyClip()
{
    QString copyText = curText.remove(0, prefix.length());
    qApp->clipboard()->setText(copyText);
}

/**
 * @brief CmdPromptInput::pasteClip
 */
void CmdPromptInput::pasteClip()
{
    paste();
}

/**
 * @brief CmdPromptInput::contextMenuEvent
 * @param event
 */
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

/**
 * @brief CmdPromptInput::eventFilter
 * @param obj
 * @param event
 * @return
 */
bool
CmdPromptInput::eventFilter(QObject* obj, QEvent* event)
{
    if (event->type() == QEvent::KeyPress) {
        if (isBlinking) {
            emit stopBlinking();
        }

        QKeyEvent* pressedKey = (QKeyEvent*)event;

        //NOTE: These shortcuts need to be caught since QLineEdit uses them
        if (pressedKey->matches(QKeySequence::Cut)) {
            pressedKey->accept();
            emit cutPressed();
            return true;
        }
        else if (pressedKey->matches(QKeySequence::Copy)) {
            pressedKey->accept();
            emit copyPressed();
            return true;
        }
        else if (pressedKey->matches(QKeySequence::Paste)) {
            pressedKey->accept();
            emit pastePressed();
            return true;
        }
        else if (pressedKey->matches(QKeySequence::SelectAll)) {
            pressedKey->accept();
            emit selectAllPressed();
            return true;
        }
        else if (pressedKey->matches(QKeySequence::Undo)) {
            pressedKey->accept();
            emit undoPressed();
            return true;
        }
        else if (pressedKey->matches(QKeySequence::Redo)) {
            pressedKey->accept();
            emit redoPressed();
            return true;
        }

        int key = pressedKey->key();
        switch (key) {
            case Qt::Key_Enter:
            case Qt::Key_Return:
                pressedKey->accept();
                processInput();
                return true;
            case Qt::Key_Delete:
                pressedKey->accept();
                del();
                emit deletePressed();
                return true;
            case Qt::Key_Tab:
                pressedKey->accept();
                emit tabPressed();
                return true;
            case Qt::Key_Escape:
                pressedKey->accept();
                prefix = defaultPrefix;
                clear();
                emit appendHistory(curText + tr("*Cancel*"), prefix.length());
                emit escapePressed();
                return true;
            case Qt::Key_Up:
                pressedKey->accept();
                emit upPressed();
                return true;
            case Qt::Key_Down:
                pressedKey->accept();
                emit downPressed();
                return true;
            case Qt::Key_F1:
                pressedKey->accept();
                emit F1Pressed();
                return true;
            case Qt::Key_F2:
                pressedKey->accept();
                emit F2Pressed();
                return true;
            case Qt::Key_F3:
                pressedKey->accept();
                emit F3Pressed();
                return true;
                break;
            case Qt::Key_F4:
                pressedKey->accept();
                emit F4Pressed();
                return true;
                break;
            case Qt::Key_F5:
                emit F5Pressed();
                pressedKey->accept();
                return true;
                break;
            case Qt::Key_F6:
                pressedKey->accept();
                emit F6Pressed();
                return true;
                break;
            case Qt::Key_F7:
                pressedKey->accept();
                emit F7Pressed();
                return true;
                break;
            case Qt::Key_F8:
                pressedKey->accept();
                emit F8Pressed();
                return true;
                break;
            case Qt::Key_F9:
                pressedKey->accept();
                emit F9Pressed();
                return true;
                break;
            case Qt::Key_F10:
                pressedKey->accept();
                emit F10Pressed();
                return true;
                break;
            case Qt::Key_F11:
                pressedKey->accept();
                emit F11Pressed();
                return true;
                break;
            case Qt::Key_F12:
                pressedKey->accept();
                emit F12Pressed();
                return true;
                break;
            case Qt::Key_Shift:
                pressedKey->ignore(); //we don't want to eat it, we just want to keep track of it
                emit shiftPressed();
                break;
            default:
                pressedKey->ignore();
        }
    }

    if (event->type() == QEvent::KeyRelease) {
        QKeyEvent* releasedKey = (QKeyEvent*)event;
        int key = releasedKey->key();
        switch (key) {
            case Qt::Key_Shift:
                releasedKey->ignore(); //we don't want to eat it, we just want to keep track of it
                emit shiftReleased();
                break;
            default:
                releasedKey->ignore();
        }
    }
    return QObject::eventFilter(obj, event);
}

