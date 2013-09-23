#include "cmdprompt.h"
#include <QApplication>
#include <QString>
#include <QAction>
#include <QMenu>
#include <QFile>
#include <QFrame>
#include <QVBoxLayout>
#include <QContextMenuEvent>
#include <QSplitter>
#include <QTextStream>

CmdPrompt::CmdPrompt(QWidget* parent) : QWidget(parent)
{
    qDebug("CmdPrompt Constructor");
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

    this->show();

    connect(promptInput, SIGNAL(appendHistory(const QString&)), promptHistory, SLOT(appendHistory(const QString&)));
    connect(this, SIGNAL(appendTheHistory(const QString&)), promptHistory, SLOT(appendHistory(const QString&)));

    //For use outside of command prompt
    connect(promptInput, SIGNAL(startCommand(const QString&)), this, SIGNAL(startCommand(const QString&)));
    connect(promptInput, SIGNAL(runCommand(const QString&, const QString&)), this, SIGNAL(runCommand(const QString&, const QString&)));
    connect(promptInput, SIGNAL(deletePressed()),    this, SIGNAL(deletePressed()));
    connect(promptInput, SIGNAL(tabPressed()),       this, SIGNAL(tabPressed()));
    connect(promptInput, SIGNAL(escapePressed()),    this, SIGNAL(escapePressed()));
    connect(promptInput, SIGNAL(F1Pressed()),        this, SIGNAL(F1Pressed()));
    connect(promptInput, SIGNAL(F2Pressed()),        this, SIGNAL(F2Pressed()));
    connect(promptInput, SIGNAL(F3Pressed()),        this, SIGNAL(F3Pressed()));
    connect(promptInput, SIGNAL(F4Pressed()),        this, SIGNAL(F4Pressed()));
    connect(promptInput, SIGNAL(F5Pressed()),        this, SIGNAL(F5Pressed()));
    connect(promptInput, SIGNAL(F6Pressed()),        this, SIGNAL(F6Pressed()));
    connect(promptInput, SIGNAL(F7Pressed()),        this, SIGNAL(F7Pressed()));
    connect(promptInput, SIGNAL(F8Pressed()),        this, SIGNAL(F8Pressed()));
    connect(promptInput, SIGNAL(F9Pressed()),        this, SIGNAL(F9Pressed()));
    connect(promptInput, SIGNAL(F10Pressed()),       this, SIGNAL(F10Pressed()));
    connect(promptInput, SIGNAL(F11Pressed()),       this, SIGNAL(F11Pressed()));
    connect(promptInput, SIGNAL(F12Pressed()),       this, SIGNAL(F12Pressed()));
    connect(promptInput, SIGNAL(cutPressed()),       this, SIGNAL(cutPressed()));
    connect(promptInput, SIGNAL(copyPressed()),      this, SIGNAL(copyPressed()));
    connect(promptInput, SIGNAL(pastePressed()),     this, SIGNAL(pastePressed()));
    connect(promptInput, SIGNAL(selectAllPressed()), this, SIGNAL(selectAllPressed()));
    connect(promptInput, SIGNAL(undoPressed()),      this, SIGNAL(undoPressed()));
    connect(promptInput, SIGNAL(redoPressed()),      this, SIGNAL(redoPressed()));

    connect(promptInput, SIGNAL(shiftPressed()),     this, SIGNAL(shiftPressed()));
    connect(promptInput, SIGNAL(shiftReleased()),    this, SIGNAL(shiftReleased()));
}

CmdPrompt::~CmdPrompt()
{
    delete styleHash;
}

void CmdPrompt::floatingChanged(bool isFloating)
{
    qDebug("CmdPrompt floatingChanged(%d)", isFloating);
    if(isFloating) promptSplitter->hide();
    else           promptSplitter->show();
}

void CmdPrompt::saveHistory(const QString& fileName, bool html)
{
    qDebug("CmdPrompt saveHistory");
    QFile file(fileName);
    if (!file.open(QIODevice::WriteOnly | QIODevice::Text))
        return;

    //TODO: save during input in case of crash
    QTextStream output(&file);
    if(html) { output << promptHistory->toHtml();      }
    else     { output << promptHistory->toPlainText(); }
}

void CmdPrompt::setPromptTextColor(const QColor& color)
{
    styleHash->insert("color", color.name());
    styleHash->insert("selection-background-color", color.name());
    updateStyle();
}

void CmdPrompt::setPromptBackgroundColor(const QColor& color)
{
    styleHash->insert("background-color", color.name());
    styleHash->insert("selection-color", color.name());
    updateStyle();
}


void CmdPrompt::setPromptFontFamily(const QString& family)
{
    styleHash->insert("font-family", family);
    updateStyle();
}

void CmdPrompt::setPromptFontStyle(const QString& style)
{
    styleHash->insert("font-style", style);
    updateStyle();
}

void CmdPrompt::setPromptFontSize(int size)
{
    styleHash->insert("font-size", QString().setNum(size).append("px"));
    updateStyle();
}

void CmdPrompt::updateStyle()
{
    QString style = "QTextBrowser,QLineEdit{";

    QHashIterator<QString, QString> i(*styleHash);
    while(i.hasNext())
    {
        i.next();
        style.append(i.key() + ":" + i.value() + ";");
    }
    style.append("}");

    this->setStyleSheet(style);
}

void CmdPrompt::appendHistory(const QString& txt)
{
    if(txt.isNull())
    {
        emit appendTheHistory(promptInput->curText);
        return;
    }
    qDebug("CmdPrompt - appendHistory()");
    emit appendTheHistory(txt);
}

void CmdPrompt::setPrefix(const QString& txt)
{
    promptInput->prefix = txt;
    promptInput->curText = txt;
    promptInput->setText(txt);
}

//============================================================================================================

CmdPromptSplitter::CmdPromptSplitter(QWidget* parent) : QSplitter(parent)
{
    qDebug("CmdPromptSplitter Constructor");
    setObjectName("Command Prompt Splitter");

    setOrientation(Qt::Vertical);
    //NOTE: Add two empty widgets just so we have a handle to grab
    addWidget(new QWidget(this)); addWidget(new QWidget(this));

    connect(this, SIGNAL(pressResizeHistory(int)),   parent, SLOT(startResizingTheHistory(int)));
    connect(this, SIGNAL(releaseResizeHistory(int)), parent, SLOT(stopResizingTheHistory(int)));
    connect(this, SIGNAL(moveResizeHistory(int)),    parent, SLOT(resizeTheHistory(int)));
}

CmdPromptSplitter::~CmdPromptSplitter()
{
}

QSplitterHandle* CmdPromptSplitter::createHandle()
{
    return new CmdPromptHandle(orientation(), this);
}

//============================================================================================================

CmdPromptHandle::CmdPromptHandle(Qt::Orientation orientation, QSplitter* parent) : QSplitterHandle(orientation, parent)
{
    qDebug("CmdPromptHandle Constructor");
    setObjectName("Command Prompt Handle");

    connect(this, SIGNAL(handlePressed(int)),  parent, SIGNAL(pressResizeHistory(int)));
    connect(this, SIGNAL(handleReleased(int)), parent, SIGNAL(releaseResizeHistory(int)));
    connect(this, SIGNAL(handleMoved(int)),    parent, SIGNAL(moveResizeHistory(int)));
}

CmdPromptHandle::~CmdPromptHandle()
{
}

void CmdPromptHandle::mousePressEvent(QMouseEvent* e)
{
    pressY = e->globalY();
    emit handlePressed(pressY);
}

void CmdPromptHandle::mouseReleaseEvent(QMouseEvent* e)
{
    releaseY = e->globalY();
    emit handleReleased(releaseY);
}

void CmdPromptHandle::mouseMoveEvent(QMouseEvent* e)
{
    moveY = e->globalY();
    int dY = moveY - pressY;
    emit handleMoved(dY);
}

//============================================================================================================

//TODO: Ensure history jumps to the bottom when input is processed
CmdPromptHistory::CmdPromptHistory()
{
    qDebug("CmdPromptHistory Constructor");
    setObjectName("Command Prompt History");

    int initHeight = 57; // 19*3 (approximately three lines of text)

    this->append("Welcome to Embroidermodder 2!");
    this->append("Open some of our sample files. Many formats are supported.");
    this->append("For help, press F1.");
    this->setFrameStyle(QFrame::NoFrame);
    this->setMaximumHeight(initHeight);
    this->setMinimumWidth(200); // TODO: use float/dock events to set minimum size so when floating, it isn't smooshed.

    this->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOn);
}

CmdPromptHistory::~CmdPromptHistory()
{
}

void CmdPromptHistory::appendHistory(QString txt)
{
    this->append(txt);
}

void CmdPromptHistory::startResizeHistory(int y)
{
    tmpHeight = height();
}

void CmdPromptHistory::stopResizeHistory(int y)
{
    tmpHeight = height();
}

void CmdPromptHistory::resizeHistory(int y)
{
    int newHeight = tmpHeight - y;
    if(newHeight < 0)
        newHeight = 0;
    setMaximumHeight(newHeight);
}

void CmdPromptHistory::contextMenuEvent(QContextMenuEvent* event)
{
    QMenu* menu = createStandardContextMenu();
    menu->addSeparator();
    //TODO: Extra stuff
    menu->exec(event->globalPos());
    delete menu;
}

//============================================================================================================
CmdPromptInput::CmdPromptInput(QWidget* parent) : QLineEdit(parent)
{
    qDebug("CmdPromptInput Constructor");
    setObjectName("Command Prompt Input");

    prefix = "Command: ";
    curText = prefix;

    lastCmd = "help";
    curCmd = "help";
    cmdActive = false;

    rapidFireEnabled = false;

    this->setText(prefix);
    this->setFrame(false);
    this->setMaxLength(266);
    this->setMaximumSize(5000, 25);
    this->setDragEnabled(false);

    connect(this, SIGNAL(cursorPositionChanged(int, int)), this, SLOT(checkCursorPosition(int, int)));
    connect(this, SIGNAL(textEdited(const QString&)), this, SLOT(checkEditedText(const QString&)));
    connect(this, SIGNAL(textChanged(const QString&)), this, SLOT(checkChangedText(const QString&)));
    connect(this, SIGNAL(selectionChanged()), this, SLOT(checkSelection()));

    aliasHash = new QHash<QString, QString>;

    this->installEventFilter(this);
    this->setFocus(Qt::OtherFocusReason);
}

CmdPromptInput::~CmdPromptInput()
{
    delete aliasHash;
}

void CmdPromptInput::addCommand(const QString& alias, const QString& cmd)
{
    aliasHash->insert(alias.toLower(), cmd.toLower());
    qDebug("Command Added: %s, %s", qPrintable(alias), qPrintable(cmd));
}

void CmdPromptInput::endCommand()
{
    qDebug("CmdPromptInput endCommand");
    lastCmd = curCmd;
    cmdActive = false;
    rapidFireEnabled = false;

    prefix = "Command: ";
    clear();
}

void CmdPromptInput::processInput(const QChar& rapidChar)
{
    qDebug("CmdPromptInput::processInput");

    updateCurrentText(curText);

    QString cmdtxt(curText);
    cmdtxt.replace(0, prefix.length(), "");
    if(!rapidFireEnabled) cmdtxt = cmdtxt.toLower();

    if(cmdActive)
    {
        if(rapidFireEnabled)
        {
            if(rapidChar == Qt::Key_Enter || rapidChar == Qt::Key_Return)
            {
                emit appendHistory(curText);
                emit runCommand(curCmd, "RAPID_ENTER");
                curText.clear();
                clear();
                return;
            }
            else if(rapidChar == Qt::Key_Space)
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
        }
        else
        {
            emit appendHistory(curText);
            emit runCommand(curCmd, cmdtxt);
        }
    }
    else
    {
        if(aliasHash->contains(cmdtxt))
        {
            cmdActive = true;
            lastCmd = curCmd;
            curCmd = aliasHash->value(cmdtxt);
            emit appendHistory(curText);
            emit startCommand(curCmd);
        }
        else if(cmdtxt.isEmpty())
        {
            cmdActive = true;
            emit appendHistory(curText);
            //Rerun the last successful command
            emit startCommand(lastCmd);
        }
        else
        {
            emit appendHistory(curText + " Unknown command \"" + cmdtxt + "\". Press F1 for help.");
        }
    }

    if(!rapidFireEnabled)
        clear();
}

void CmdPromptInput::checkSelection()
{
    //qDebug("CmdPromptInput::checkSelection");
    if(this->hasSelectedText())
        this->deselect();
}

void CmdPromptInput::checkCursorPosition(int oldpos, int newpos)
{
    //qDebug("CmdPromptInput::checkCursorPosition - %d %d", oldpos, newpos);
    if(this->hasSelectedText())
        this->deselect();
    if(newpos < prefix.length())
        this->setCursorPosition(prefix.length());
}

void CmdPromptInput::updateCurrentText(const QString& txt)
{
    if(!txt.startsWith(prefix))
    {
        if(txt.length() < prefix.length())
            this->setText(prefix);
        else if(txt.length() != prefix.length())
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
}

void CmdPromptInput::checkEditedText(const QString& txt)
{
    updateCurrentText(txt);

    if(rapidFireEnabled)
        processInput();
}

void CmdPromptInput::checkChangedText(const QString& txt)
{
    updateCurrentText(txt);
}

void CmdPromptInput::contextMenuEvent(QContextMenuEvent* event)
{
    QMenu menu(this);
    menu.addSeparator();
    //TODO: Extra stuff
    menu.exec(event->globalPos());
}

bool CmdPromptInput::eventFilter(QObject* obj, QEvent* event)
{
    if(event->type() == QEvent::KeyPress)
    {
        QKeyEvent* pressedKey = (QKeyEvent*)event;

        //NOTE: These shortcuts need to be caught since QLineEdit uses them
        if(pressedKey->matches(QKeySequence::Cut))
        {
            pressedKey->accept();
            emit cutPressed();
            return true;
        }
        else if(pressedKey->matches(QKeySequence::Copy))
        {
            pressedKey->accept();
            emit copyPressed();
            return true;
        }
        else if(pressedKey->matches(QKeySequence::Paste))
        {
            pressedKey->accept();
            emit pastePressed();
            return true;
        }
        else if(pressedKey->matches(QKeySequence::SelectAll))
        {
            pressedKey->accept();
            emit selectAllPressed();
            return true;
        }
        else if(pressedKey->matches(QKeySequence::Undo))
        {
            pressedKey->accept();
            emit undoPressed();
            return true;
        }
        else if(pressedKey->matches(QKeySequence::Redo))
        {
            pressedKey->accept();
            emit redoPressed();
            return true;
        }

        int key = pressedKey->key();
        switch(key)
        {
            case Qt::Key_Enter:
            case Qt::Key_Return:
            case Qt::Key_Space:
                pressedKey->accept();
                processInput(QChar(key));
                return true;
                break;
            case Qt::Key_Delete:
                pressedKey->accept();
                emit deletePressed();
                return true;
                break;
            case Qt::Key_Tab:
                pressedKey->accept();
                emit tabPressed();
                return true;
                break;
            case Qt::Key_Escape:
                pressedKey->accept();
                prefix = "Command: ";
                clear();
                emit appendHistory(curText + tr("*Cancel*"));
                emit escapePressed();
                return true;
                break;
            case Qt::Key_F1:
                pressedKey->accept();
                emit F1Pressed();
                return true;
                break;
            case Qt::Key_F2:
                pressedKey->accept();
                emit F2Pressed();
                return true;
                break;
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

    if(event->type() == QEvent::KeyRelease)
    {
        QKeyEvent* releasedKey = (QKeyEvent*)event;
        int key = releasedKey->key();
        switch(key)
        {
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

/* kate: bom off; indent-mode cstyle; indent-width 4; replace-trailing-space-save on; */
