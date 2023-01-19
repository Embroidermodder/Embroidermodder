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


#include "imgui.h"
#include "backends/imgui_impl_glfw.h"
#include "backends/imgui_impl_opengl2.h"

#include <iostream>

#include <GLFW/glfw3.h>

#include "embroidermodder.h"

void
MainWindow::changelog()
{
    debug_message("changelog()");

    QApplication::setOverrideCursor(Qt::ArrowCursor);
    std::ifstream file("docs/changelog.md");
    std::string message, line, title = "Changelog -- Embroidermodder 2";
    while (std::getline(file, line)) {
        message += line + "\n";
    }

    int width = 500;
    int height = 200;
    if (!glfwInit()) {
        std::cout << "ERROR: Failed to initialise GLFW." << std::endl;
        return;
    }
    GLFWwindow *window = glfwCreateWindow(width, height, title.data(), NULL, NULL);
    if (!window) {
        std::cout << "ERROR: Failed to create GLFW window." << std::endl;
        glfwTerminate();
        return;
    }
    glfwMakeContextCurrent(window);

    IMGUI_CHECKVERSION();
    ImGui::CreateContext();

    ImGui_ImplGlfw_InitForOpenGL(window, true);
    ImGui_ImplOpenGL2_Init();

    glfwGetFramebufferSize(window, &width, &height);
    ImVec2 v = {width, height};
    ImGui::SetNextWindowPos({0.0, 0.0});
    ImGui::SetNextWindowSize(v);

    while (!glfwWindowShouldClose(window)) {
        glfwPollEvents();

        ImGui_ImplOpenGL2_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();

        ImGui::ShowDemoWindow();

        ImGui::Begin(title.data());
        ImGui::Text(message.data());
        if (ImGui::Button("Close")) {
            break;
        }
        ImGui::End();
        ImGui::Render();

        glfwGetFramebufferSize(window, &width, &height);
        glViewport(0, 0, width, height);
        glClearColor(0.1, 0.1, 0.1, 0.0);
        glClear(GL_COLOR_BUFFER_BIT);

        ImGui_ImplOpenGL2_RenderDrawData(ImGui::GetDrawData());

        glfwMakeContextCurrent(window);
        glfwSwapBuffers(window);
    }

    glfwDestroyWindow(window);
    glfwTerminate();
    QApplication::restoreOverrideCursor();
}

void MainWindow::about()
{
    QApplication::setOverrideCursor(Qt::ArrowCursor);
    debug_message("about()");
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
                          tr("Embroidery formats by Josh Varga and Robin Swift.") +
                          tr("\n") +
                          tr("User Interface by Jonathan Greig and Robin Swift.") +
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

void MainWindow::help()
{
    QApplication::setOverrideCursor(Qt::ArrowCursor);
    debug_message("help()");

    QString appDir = qApp->applicationDirPath();
    QDialog dialog(this);
    std::ifstream file("docs/manual.md");
    std::string s, line;
    while (std::getline(file, line)) {
        s += line + "\n";
    }
    QPlainTextEdit text(QString::fromLocal8Bit(s));

    QDialogButtonBox buttonbox(Qt::Horizontal, &dialog);
    QPushButton close_button(&dialog);
    close_button.setText("Close");
    buttonbox.addButton(&close_button, QDialogButtonBox::AcceptRole);
    buttonbox.setCenterButtons(true);
    connect(&buttonbox, SIGNAL(accepted()), &dialog, SLOT(accept()));

    QVBoxLayout layout;
    layout.setAlignment(Qt::AlignCenter);
    layout.addWidget(&text);
    layout.addWidget(&buttonbox);

    dialog.setWindowTitle("Help -- Embroidermodder 2");
    dialog.setMinimumWidth(600);
    dialog.setMinimumHeight(500);
    dialog.setLayout(&layout);
    dialog.exec();
    QApplication::restoreOverrideCursor();
}

void MainWindow::terminal()
{
    QApplication::setOverrideCursor(Qt::ArrowCursor);
    debug_message("terminal()");

    QString appDir = qApp->applicationDirPath();
    QDialog dialog(this);
    QPlainTextEdit text("");

    QDialogButtonBox buttonbox(Qt::Horizontal, &dialog);
    QPushButton close_button(&dialog);
    close_button.setText("Close");
    buttonbox.addButton(&close_button, QDialogButtonBox::AcceptRole);
    buttonbox.setCenterButtons(true);
    connect(&buttonbox, SIGNAL(accepted()), &dialog, SLOT(accept()));

    QVBoxLayout layout;
    layout.setAlignment(Qt::AlignCenter);
    layout.addWidget(&text);
    layout.addWidget(&buttonbox);

    dialog.setWindowTitle("Help -- Embroidermodder 2");
    dialog.setMinimumWidth(600);
    dialog.setMinimumHeight(500);
    dialog.setLayout(&layout);
    dialog.exec();
    QApplication::restoreOverrideCursor();
}


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
    connect(promptInput, SIGNAL(appendHistory(const QString&, int)), promptHistory, SLOT(appendHistory(const QString&, int)));
    connect(this, SIGNAL(appendTheHistory(const QString&, int)), promptHistory, SLOT(appendHistory(const QString&, int)));

    //For use outside of command prompt
    connect(promptInput, SIGNAL(startCommand(const QString&)), this, SIGNAL(startCommand(const QString&)));
    connect(promptInput, SIGNAL(runCommand(const QString&, const QString&)), this, SIGNAL(runCommand(const QString&, const QString&)));
    connect(promptInput, SIGNAL(deletePressed()),    this, SIGNAL(deletePressed()));
    connect(promptInput, SIGNAL(tabPressed()),       this, SIGNAL(tabPressed()));
    connect(promptInput, SIGNAL(escapePressed()),    this, SIGNAL(escapePressed()));
    connect(promptInput, SIGNAL(upPressed()),        this, SIGNAL(upPressed()));
    connect(promptInput, SIGNAL(downPressed()),      this, SIGNAL(downPressed()));
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

    connect(promptInput, SIGNAL(showSettings()),     this, SIGNAL(showSettings()));

    connect(promptHistory, SIGNAL(historyAppended(const QString&)), this, SIGNAL(historyAppended(const QString&)));
}

CmdPrompt::~CmdPrompt()
{
    delete styleHash;
}

void CmdPrompt::floatingChanged(bool isFloating)
{
    debug_message("CmdPrompt floatingChanged(%d)");
    if(isFloating) promptSplitter->hide();
    else           promptSplitter->show();
}

void CmdPrompt::saveHistory(const QString& fileName, bool html)
{
    debug_message("CmdPrompt saveHistory");
    QFile file(fileName);
    if (!file.open(QIODevice::WriteOnly | QIODevice::Text))
        return;

    //TODO: save during input in case of crash
    QTextStream output(&file);
    if(html) { output << promptHistory->toHtml();      }
    else     { output << promptHistory->toPlainText(); }
}

void CmdPrompt::alert(const QString& txt)
{
    QString alertTxt = "<font color=\"red\">" + txt + "</font>"; //TODO: Make the alert color customizable
    setPrefix(alertTxt);
    appendHistory(QString());
}

void CmdPrompt::startBlinking()
{
    blinkTimer->start(750);
    promptInput->isBlinking = true;
}

void CmdPrompt::stopBlinking()
{
    blinkTimer->stop();
    promptInput->isBlinking = false;
}

void CmdPrompt::blink()
{
    blinkState = !blinkState;
    if(blinkState) {
        debug_message("CmdPrompt blink1");
    }
    else {
        debug_message("CmdPrompt blink0");
    }
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
        emit appendTheHistory(promptInput->curText, promptInput->prefix.length());
        return;
    }
    debug_message("CmdPrompt - appendHistory()");
    emit appendTheHistory(txt, promptInput->prefix.length());
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
    debug_message("CmdPromptSplitter Constructor");
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
    debug_message("CmdPromptHandle Constructor");
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

CmdPromptHistory::~CmdPromptHistory()
{
}

QString CmdPromptHistory::applyFormatting(const QString& txt, int prefixLength)
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
    if(start != -1 && stop != -1 && start < stop)
    {
        for(int i = stop; i >= start; i--)
        {
            if(prefix.at(i) == ']')
                prefix.insert(i, "</font>");
            if(prefix.at(i) == '[')
                prefix.insert(i+1, "<font color=\"#0095FF\">");
            if(prefix.at(i) == '/')
            {
                prefix.insert(i+1, "<font color=\"#0095FF\">");
                prefix.insert(i, "</font>");
            }
        }
    }

    //Default Values
    start = prefix.indexOf('{');
    stop = prefix.lastIndexOf('}');
    if(start != -1 && stop != -1 && start < stop)
    {
        for(int i = stop; i >= start; i--)
        {
            if(prefix.at(i) == '}')
                prefix.insert(i, "</font>");
            if(prefix.at(i) == '{')
                prefix.insert(i+1, "<font color=\"#00AA00\">");
        }
    }

    return prefix + usrtxt;
}

void CmdPromptHistory::appendHistory(const QString& txt, int prefixLength)
{
    QString formatStr = applyFormatting(txt, prefixLength);
    this->append(formatStr);
    emit historyAppended(formatStr);
    this->moveCursor(QTextCursor::End, QTextCursor::MoveAnchor);
}

void CmdPromptHistory::startResizeHistory(int /*y*/)
{
    tmpHeight = height();
}

void CmdPromptHistory::stopResizeHistory(int /*y*/)
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

    aliasHash = new QHash<QString, QString>;

    this->installEventFilter(this);
    this->setFocus(Qt::OtherFocusReason);

    applyFormatting();
}

CmdPromptInput::~CmdPromptInput()
{
    delete aliasHash;
}

void CmdPromptInput::addCommand(const QString& alias, const QString& cmd)
{
    aliasHash->insert(alias.toLower(), cmd.toLower());
    debug_message("Command Added: %s, %s" + alias.toStdString() + cmd.toStdString());
}

void CmdPromptInput::endCommand()
{
    debug_message("CmdPromptInput endCommand");
    lastCmd = curCmd;
    cmdActive = false;
    rapidFireEnabled = false;
    emit stopBlinking();

    prefix = defaultPrefix;
    clear();
}

void CmdPromptInput::processInput(const int rapidChar)
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
            if (rapidChar == Qt::Key_Enter || rapidChar == Qt::Key_Return) {
                emit appendHistory(curText, prefix.length());
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
        else {
            emit appendHistory(curText, prefix.length());
            emit runCommand(curCmd, cmdtxt);
        }
    }
    else {
        if (aliasHash->contains(cmdtxt)) {
            cmdActive = true;
            lastCmd = curCmd;
            curCmd = aliasHash->value(cmdtxt);
            emit appendHistory(curText, prefix.length());
            emit startCommand(curCmd);
        }
        else if (cmdtxt.isEmpty()) {
            cmdActive = true;
            emit appendHistory(curText, prefix.length());
            //Rerun the last successful command
            emit startCommand(lastCmd);
        }
        else {
            /* TODO: this crashes because it's using the mainWin pointer,
             * Which is only an issue because the actuator needs to act on
             * the data in that class.
             *
             * Since this is a dev console now we may be
             * able to capture input from the parent shell instead?
             */
            mainWin->actuator(cmdtxt.toLocal8Bit().data());
            /*
            emit appendHistory(curText + "<br/><font color=\"red\">Unknown command \"" + cmdtxt + "\". Press F1 for help.</font>", prefix.length());
            */
        }
    }

    if (!rapidFireEnabled) {
        clear();
    }
}

void CmdPromptInput::checkSelection()
{
    //debug_message("CmdPromptInput::checkSelection");
    if (this->hasSelectedText())
        this->deselect();
}

void CmdPromptInput::checkCursorPosition(int /*oldpos*/, int newpos)
{
    //debug_message("CmdPromptInput::checkCursorPosition - %d %d", oldpos, newpos);
    if(this->hasSelectedText())
        this->deselect();
    if(newpos < prefix.length())
        this->setCursorPosition(prefix.length());
}

void CmdPromptInput::changeFormatting(const QList<QTextLayout::FormatRange>& formats)
{
    QList<QInputMethodEvent::Attribute> attributes;
    foreach(const QTextLayout::FormatRange& range, formats)
    {
        QInputMethodEvent::AttributeType type = QInputMethodEvent::TextFormat;
        int start = range.start - this->cursorPosition();
        int length = range.length;
        QVariant value = range.format;
        attributes.append(QInputMethodEvent::Attribute(type, start, length, value));
    }
    QInputMethodEvent event(QString(), attributes);
    QCoreApplication::sendEvent(this, &event);
}

void CmdPromptInput::clearFormatting()
{
    changeFormatting(QList<QTextLayout::FormatRange>());
}

void CmdPromptInput::applyFormatting()
{
    int prefixLength = prefix.length();

    int start = -1;
    int stop = -1;

    QList<QTextLayout::FormatRange> formats;

    //Bold Prefix
    QTextCharFormat formatPrefix;
    formatPrefix.setFontWeight(QFont::Bold);
    QTextLayout::FormatRange rangePrefix;
    rangePrefix.start = 0;
    rangePrefix.length = prefixLength;
    rangePrefix.format = formatPrefix;
    formats.append(rangePrefix);

    //Keywords
    start = prefix.indexOf('[');
    stop = prefix.lastIndexOf(']');
    if(start != -1 && stop != -1 && start < stop)
    {
        QTextCharFormat formatKeyword;
        formatKeyword.setFontWeight(QFont::Bold);
        formatKeyword.setForeground(QColor("#0095FF"));

        int rangeStart = -1;
        int rangeStop = -1;
        for(int i = stop; i >= start; i--)
        {
            if(prefix.at(i) == ']')
            {
                rangeStop = i;
            }
            if(prefix.at(i) == '[' || prefix.at(i) == '/')
            {
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

    //Default Values
    start = prefix.indexOf('{');
    stop = prefix.lastIndexOf('}');
    if(start != -1 && stop != -1 && start < stop)
    {
        QTextCharFormat formatKeyword;
        formatKeyword.setFontWeight(QFont::Bold);
        formatKeyword.setForeground(QColor("#00AA00"));

        int rangeStart = -1;
        int rangeStop = -1;
        for(int i = stop; i >= start; i--)
        {
            if(prefix.at(i) == '}')
            {
                rangeStop = i;
            }
            if(prefix.at(i) == '{')
            {
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

void CmdPromptInput::updateCurrentText(const QString& txt)
{
    int cursorPos = cursorPosition();
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
    setCursorPosition(cursorPos);

    applyFormatting();
}

void CmdPromptInput::checkEditedText(const QString& txt)
{
    updateCurrentText(txt);

    if (rapidFireEnabled) {
        /* fixme */
        processInput(Qt::Key_Return);
    }
}

void CmdPromptInput::checkChangedText(const QString& txt)
{
    updateCurrentText(txt);
}

void CmdPromptInput::copyClip()
{
    QString copyText = curText.remove(0, prefix.length());
    qApp->clipboard()->setText(copyText);
}

void CmdPromptInput::pasteClip()
{
    paste();
}

void CmdPromptInput::contextMenuEvent(QContextMenuEvent* event)
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

bool CmdPromptInput::eventFilter(QObject* obj, QEvent* event)
{
    if (event->type() == QEvent::KeyPress) {
        QKeyEvent* pressedKey = (QKeyEvent*)event;
        int key = pressedKey->key();
        switch (key) {
        case Qt::Key_Enter:
        case Qt::Key_Return:
            pressedKey->accept();
            processInput(Qt::Key_Enter);
            return true;
        default:
            break;
        }
    }
    return false;

    if (event->type() == QEvent::KeyPress) {
        if(isBlinking) emit stopBlinking();

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
                processInput(key);
                return true;
                break;
            case Qt::Key_Delete:
                pressedKey->accept();
                del();
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
                prefix = defaultPrefix;
                clear();
                emit appendHistory(curText + tr("*Cancel*"), prefix.length());
                emit escapePressed();
                return true;
                break;
            case Qt::Key_Up:
                pressedKey->accept();
                emit upPressed();
                return true;
                break;
            case Qt::Key_Down:
                pressedKey->accept();
                emit downPressed();
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


EmbDetailsDialog::EmbDetailsDialog(QGraphicsScene* theScene, QWidget* parent) : QDialog(parent)
{
    setMinimumSize(750,550);

    getInfo();
    mainWidget = createMainWidget();

    buttonBox = new QDialogButtonBox(QDialogButtonBox::Ok);
    connect(buttonBox, SIGNAL(accepted()), this, SLOT(accept()));

    QVBoxLayout* vboxLayoutMain = new QVBoxLayout(this);
    vboxLayoutMain->addWidget(mainWidget);
    vboxLayoutMain->addWidget(buttonBox);
    setLayout(vboxLayoutMain);

    setWindowTitle(tr("Embroidery Design Details"));

    QApplication::setOverrideCursor(Qt::ArrowCursor);
}

EmbDetailsDialog::~EmbDetailsDialog()
{
    QApplication::restoreOverrideCursor();
}

void EmbDetailsDialog::getInfo()
{
    //TODO: generate a temporary pattern from the scene data.

    //TODO: grab this information from the pattern
    stitchesTotal = 5; //TODO: embStitchList_count(pattern->stitchList, TOTAL);
    stitchesReal  = 4; //TODO: embStitchList_count(pattern->stitchList, NORMAL);
    stitchesJump  = 3; //TODO: embStitchList_count(pattern->stitchList, JUMP);
    stitchesTrim  = 2; //TODO: embStitchList_count(pattern->stitchList, TRIM);
    colorTotal    = 1; //TODO: embThreadList_count(pattern->threadList, TOTAL);
    colorChanges  = 0; //TODO: embThreadList_count(pattern->threadList, CHANGES);

    boundingRect.setRect(0, 0, 50, 100); //TODO: embPattern_calcBoundingBox(pattern);
}

QWidget* EmbDetailsDialog::createMainWidget()
{
    QWidget* widget = new QWidget(this);

    //Misc
    QGroupBox* groupBoxMisc = new QGroupBox(tr("General Information"), widget);

    QLabel* labelStitchesTotal = new QLabel(tr("Total Stitches:"), this);
    QLabel* labelStitchesReal  = new QLabel(tr("Real Stitches:"),  this);
    QLabel* labelStitchesJump  = new QLabel(tr("Jump Stitches:"),  this);
    QLabel* labelStitchesTrim  = new QLabel(tr("Trim Stitches:"),  this);
    QLabel* labelColorTotal    = new QLabel(tr("Total Colors:"),   this);
    QLabel* labelColorChanges  = new QLabel(tr("Color Changes:"),  this);
    QLabel* labelRectLeft      = new QLabel(tr("Left:"),           this);
    QLabel* labelRectTop       = new QLabel(tr("Top:"),            this);
    QLabel* labelRectRight     = new QLabel(tr("Right:"),          this);
    QLabel* labelRectBottom    = new QLabel(tr("Bottom:"),         this);
    QLabel* labelRectWidth     = new QLabel(tr("Width:"),          this);
    QLabel* labelRectHeight    = new QLabel(tr("Height:"),         this);

    QLabel* fieldStitchesTotal = new QLabel(QString::number(stitchesTotal), this);
    QLabel* fieldStitchesReal  = new QLabel(QString::number(stitchesReal),  this);
    QLabel* fieldStitchesJump  = new QLabel(QString::number(stitchesJump),  this);
    QLabel* fieldStitchesTrim  = new QLabel(QString::number(stitchesTrim),  this);
    QLabel* fieldColorTotal    = new QLabel(QString::number(colorTotal),    this);
    QLabel* fieldColorChanges  = new QLabel(QString::number(colorChanges),  this);
    QLabel* fieldRectLeft      = new QLabel(QString::number(boundingRect.left())   + " mm", this);
    QLabel* fieldRectTop       = new QLabel(QString::number(boundingRect.top())    + " mm", this);
    QLabel* fieldRectRight     = new QLabel(QString::number(boundingRect.right())  + " mm", this);
    QLabel* fieldRectBottom    = new QLabel(QString::number(boundingRect.bottom()) + " mm", this);
    QLabel* fieldRectWidth     = new QLabel(QString::number(boundingRect.width())  + " mm", this);
    QLabel* fieldRectHeight    = new QLabel(QString::number(boundingRect.height()) + " mm", this);

    QGridLayout* gridLayoutMisc = new QGridLayout(groupBoxMisc);
    gridLayoutMisc->addWidget(labelStitchesTotal,  0, 0, Qt::AlignLeft);
    gridLayoutMisc->addWidget(labelStitchesReal,   1, 0, Qt::AlignLeft);
    gridLayoutMisc->addWidget(labelStitchesJump,   2, 0, Qt::AlignLeft);
    gridLayoutMisc->addWidget(labelStitchesTrim,   3, 0, Qt::AlignLeft);
    gridLayoutMisc->addWidget(labelColorTotal,     4, 0, Qt::AlignLeft);
    gridLayoutMisc->addWidget(labelColorChanges,   5, 0, Qt::AlignLeft);
    gridLayoutMisc->addWidget(labelRectLeft,       6, 0, Qt::AlignLeft);
    gridLayoutMisc->addWidget(labelRectTop,        7, 0, Qt::AlignLeft);
    gridLayoutMisc->addWidget(labelRectRight,      8, 0, Qt::AlignLeft);
    gridLayoutMisc->addWidget(labelRectBottom,     9, 0, Qt::AlignLeft);
    gridLayoutMisc->addWidget(labelRectWidth,     10, 0, Qt::AlignLeft);
    gridLayoutMisc->addWidget(labelRectHeight,    11, 0, Qt::AlignLeft);
    gridLayoutMisc->addWidget(fieldStitchesTotal,  0, 1, Qt::AlignLeft);
    gridLayoutMisc->addWidget(fieldStitchesReal,   1, 1, Qt::AlignLeft);
    gridLayoutMisc->addWidget(fieldStitchesJump,   2, 1, Qt::AlignLeft);
    gridLayoutMisc->addWidget(fieldStitchesTrim,   3, 1, Qt::AlignLeft);
    gridLayoutMisc->addWidget(fieldColorTotal,     4, 1, Qt::AlignLeft);
    gridLayoutMisc->addWidget(fieldColorChanges,   5, 1, Qt::AlignLeft);
    gridLayoutMisc->addWidget(fieldRectLeft,       6, 1, Qt::AlignLeft);
    gridLayoutMisc->addWidget(fieldRectTop,        7, 1, Qt::AlignLeft);
    gridLayoutMisc->addWidget(fieldRectRight,      8, 1, Qt::AlignLeft);
    gridLayoutMisc->addWidget(fieldRectBottom,     9, 1, Qt::AlignLeft);
    gridLayoutMisc->addWidget(fieldRectWidth,     10, 1, Qt::AlignLeft);
    gridLayoutMisc->addWidget(fieldRectHeight,    11, 1, Qt::AlignLeft);
    gridLayoutMisc->setColumnStretch(1,1);
    groupBoxMisc->setLayout(gridLayoutMisc);

    //TODO: Color Histogram

    //Stitch Distribution
    //QGroupBox* groupBoxDist = new QGroupBox(tr("Stitch Distribution"), widget);

    //TODO: Stitch Distribution Histogram

    //Widget Layout
    QVBoxLayout *vboxLayoutMain = new QVBoxLayout(widget);
    vboxLayoutMain->addWidget(groupBoxMisc);
    //vboxLayoutMain->addWidget(groupBoxDist);
    vboxLayoutMain->addStretch(1);
    widget->setLayout(vboxLayoutMain);

    QScrollArea* scrollArea = new QScrollArea(this);
    scrollArea->setWidgetResizable(true);
    scrollArea->setWidget(widget);
    return scrollArea;
}

//TODO: Move majority of this code into libembroidery
/*
void MainWindow::designDetails()
{
    QApplication::setOverrideCursor(Qt::ArrowCursor);
    debug_message("designDetails()");
    QString appName = QApplication::applicationName();
    QString title = "Design Details";

    EmbPattern* pattern = 0;

    //TODO: This is temporary. Generate actual pattern data from the scene.
    //======================================================
    //embPattern_read(pattern, "/mydata/embroidery-designs/KDE.EXP"); //TODO: This convenience function is messed up.

    EmbReaderWriter* reader = 0;
    int readSuccessful;
    QString tmpFileName = "/mydata/embroidery-designs/KDE.EXP";

    pattern = embPattern_create();
    if(!pattern) { printf("Could not allocate memory for embroidery pattern\n"); }

    readSuccessful = 0;
    reader = embReaderWriter_getByFileName(qPrintable(tmpFileName));
    if(!reader)
    {
        readSuccessful = 0;
        printf("Unsupported read file type\n");
    }
    else
    {
        readSuccessful = reader->reader(pattern, qPrintable(tmpFileName));
        if(!readSuccessful) printf("Reading file was unsuccessful\n");
    }
    free(reader);
    if(!readSuccessful)
    {
        embPattern_free(pattern);
    }
    //======================================================


    EmbRect bounds = embPattern_calcBoundingBox(pattern);

    int colors = 1;
    int num_stitches = 0;
    int real_stitches = 0;
    int jump_stitches = 0;
    int trim_stitches = 0;
    int unknown_stitches = 0;
    int num_colors = 0;
    double minx = 0.0, maxx = 0.0, miny = 0.0, maxy = 0.0;
    double min_stitchlength = 999.0;
    double max_stitchlength = 0.0;
    double total_stitchlength = 0.0;
    int number_of_minlength_stitches = 0;
    int number_of_maxlength_stitches = 0;

    double xx = 0.0, yy = 0.0;
    double dx = 0.0, dy = 0.0;
    double length = 0.0;

    num_colors = embThreadList_count(pattern->threadList);
    num_stitches = embStitchList_count(pattern->stitchList);
    if(num_stitches == 0)
    {
        QMessageBox::warning(this, tr("No Design Loaded"), tr("<b>A design needs to be loaded or created before details can be determined.</b>"));
        return;
    }
    QVector<double> stitchLengths;

    double totalColorLength = 0.0;
    for(int i = 0; i < num_stitches; i++)
    {
        dx = embStitchList_getAt(pattern->stitchList, i).xx - xx;
        dy = embStitchList_getAt(pattern->stitchList, i).yy - yy;
        xx = embStitchList_getAt(pattern->stitchList, i).xx;
        yy = embStitchList_getAt(pattern->stitchList, i).yy;
        length=sqrt(dx * dx + dy * dy);
        totalColorLength += length;
        if(i > 0 && embStitchList_getAt(pattern->stitchList, i-1).flags != NORMAL)
            length = 0.0; //can't count first normal stitch;
        if(!(embStitchList_getAt(pattern->stitchList, i).flags & (JUMP | TRIM)))
        {
            real_stitches++;
            if(length > max_stitchlength) { max_stitchlength = length; number_of_maxlength_stitches = 0; }
            if(length == max_stitchlength) number_of_maxlength_stitches++;
            if(length > 0 && length < min_stitchlength)
            {
                min_stitchlength = length;
                number_of_minlength_stitches = 0;
            }
            if(length == min_stitchlength) number_of_minlength_stitches++;
            total_stitchlength += length;
            if(xx < minx) minx = xx;
            if(xx > maxx) maxx = xx;
            if(yy < miny) miny = yy;
            if(yy > maxy) maxy = yy;
        }
        if(embStitchList_getAt(pattern->stitchList, i).flags & JUMP)
        {
            jump_stitches++;
        }
        if(embStitchList_getAt(pattern->stitchList, i).flags & TRIM)
        {
            trim_stitches++;
        }
        if(embStitchList_getAt(pattern->stitchList, i).flags & STOP)
        {
            stitchLengths.push_back(totalColorLength);
            totalColorLength = 0;
            colors++;
        }
        if(embStitchList_getAt(pattern->stitchList, i).flags & END)
        {
            stitchLengths.push_back(totalColorLength);
        }
    }

    //second pass to fill bins now that we know max stitch length
#define NUMBINS 10
    int bin[NUMBINS+1];
    for(int i = 0; i <= NUMBINS; i++)
    {
        bin[i]=0;
    }

    for(int i = 0; i < num_stitches; i++)
    {
        dx = embStitchList_getAt(pattern->stitchList, i).xx - xx;
        dy = embStitchList_getAt(pattern->stitchList, i).yy - yy;
        xx = embStitchList_getAt(pattern->stitchList, i).xx;
        yy = embStitchList_getAt(pattern->stitchList, i).yy;
        if(i > 0 && embStitchList_getAt(pattern->stitchList, i-1).flags == NORMAL && embStitchList_getAt(pattern->stitchList, i).flags == NORMAL)
        {
            length=sqrt(dx * dx + dy * dy);
            bin[int(floor(NUMBINS*length/max_stitchlength))]++;
        }
    }

    double binSize = max_stitchlength / NUMBINS;

    QString str;
    for(int i = 0; i < NUMBINS; i++)
    {
        str += QString::number(binSize * (i), 'f', 1) + " - " + QString::number(binSize * (i+1), 'f', 1) + " mm: " +  QString::number(bin[i]) + "\n\n";
    }

    QDialog dialog(this);

    QGridLayout* grid = new QGridLayout(this);
    grid->setSpacing(2);

    grid->addWidget(new QLabel(tr("Stitches:")),0,0,1,1);
    grid->addWidget(new QLabel(QString::number(num_stitches)), 0, 1, 1, 1);
    grid->addWidget(new QLabel(tr("Colors:")),1,0,1,1);
    grid->addWidget(new QLabel(QString::number(num_colors)), 1, 1, 1, 1);
    grid->addWidget(new QLabel(tr("Jumps:")),2,0,1,1);
    grid->addWidget(new QLabel(QString::number(jump_stitches)), 2, 1, 1, 1);
    grid->addWidget(new QLabel(tr("Top:")),3,0,1,1);
    grid->addWidget(new QLabel(QString::number(bounds.top) + " mm"), 3, 1, 1, 1);
    grid->addWidget(new QLabel(tr("Left:")),4,0,1,1);
    grid->addWidget(new QLabel(QString::number(bounds.left) + " mm"), 4, 1, 1, 1);
    grid->addWidget(new QLabel(tr("Bottom:")),5,0,1,1);
    grid->addWidget(new QLabel(QString::number(bounds.bottom) + " mm"), 5, 1, 1, 1);
    grid->addWidget(new QLabel(tr("Right:")),6,0,1,1);
    grid->addWidget(new QLabel(QString::number(bounds.right) + " mm"), 6, 1, 1, 1);
    grid->addWidget(new QLabel(tr("Width:")),7,0,1,1);
    grid->addWidget(new QLabel(QString::number((bounds.right - bounds.left)) + " mm"), 7, 1, 1, 1);
    grid->addWidget(new QLabel(tr("Height:")),8,0,1,1);
    grid->addWidget(new QLabel(QString::number((bounds.bottom - bounds.top)) + " mm"), 8, 1, 1, 1);
    grid->addWidget(new QLabel(tr("\nStitch Distribution: \n")),9,0,1,2);
    grid->addWidget(new QLabel(str), 10, 0, 1, 1);
    grid->addWidget(new QLabel(tr("\nThread Length By Color: \n")),11,0,1,2);
    int currentRow = 12;

    for(int i = 0; i < num_colors; i++)
    {
        QFrame *frame = new QFrame();
        frame->setGeometry(0,0,30,30);
        QPalette palette = frame->palette();
        EmbColor t = embThreadList_getAt(pattern->threadList, i).color;
        palette.setColor(backgroundRole(), QColor( t.r, t.g, t.b ) );
        frame->setPalette( palette );
        frame->setAutoFillBackground(true);
        grid->addWidget(frame, currentRow,0,1,1);
        qDebug("size: %d i: %d", stitchLengths.size(), i);
        grid->addWidget(new QLabel(QString::number(stitchLengths.at(i)) + " mm"), currentRow,1,1,1);
        currentRow++;
    }

    QDialogButtonBox buttonbox(Qt::Horizontal, &dialog);
    QPushButton button(&dialog);
    button.setText("Ok");
    buttonbox.addButton(&button, QDialogButtonBox::AcceptRole);
    buttonbox.setCenterButtons(true);
    connect(&buttonbox, SIGNAL(accepted()), &dialog, SLOT(accept()));

    grid->addWidget(&buttonbox, currentRow, 0, 1, 2);

    dialog.setWindowTitle(title);
    dialog.setMinimumWidth(100);
    dialog.setMinimumHeight(50);
    dialog.setLayout(grid);
    dialog.exec();
    QApplication::restoreOverrideCursor();

}
*/
