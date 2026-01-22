#include "mainwindow.h"
#include "view.h"
#include "statusbar.h"
#include "statusbar-button.h"
#include "imagewidget.h"
#include "layer-manager.h"
#include "object-data.h"
#include "object-arc.h"
#include "object-circle.h"
#include "object-dimleader.h"
#include "object-ellipse.h"
#include "object-image.h"
#include "object-line.h"
#include "object-path.h"
#include "object-point.h"
#include "object-polygon.h"
#include "object-polyline.h"
#include "object-rect.h"
#include "object-textsingle.h"
#include "embroidery.h"
#include "property-editor.h"
#include "undo-editor.h"
#include "undo-commands.h"
#include "embdetails-dialog.h"

#include <QLabel>
#include <QDesktopServices>
#include <QApplication>
#include <QUrl>
#include <QProcess>
#include <QMessageBox>
#include <QDialogButtonBox>
#include <QPushButton>
#include <QMdiArea>
#include <QGraphicsScene>
#include <QComboBox>
#include <QWhatsThis>

void MainWindow::stub_implement(QString txt)
{
    qDebug("TODO: %s", qPrintable(txt));
}

void MainWindow::stub_testing()
{
    QMessageBox::warning(this, tr("Testing Feature"), tr("<b>This feature is in testing.</b>"));
}

void MainWindow::exit()
{
    qDebug("exit()");
    if (state.settings.prompt_save_history) {
        /* TODO: get filename from settings */
        prompt->saveHistory("prompt.log", state.settings.prompt_save_history_as_html);
    }
    qApp->closeAllWindows();
    this->deleteLater(); //Force the MainWindow destructor to run before exiting. Makes Valgrind "still reachable" happy :)
}

void MainWindow::quit()
{
    qDebug("quit()");
    exit();
}

void MainWindow::checkForUpdates()
{
    qDebug("checkForUpdates()");
    //TODO: Check website for new versions, commands, etc...
}

void MainWindow::cut()
{
    qDebug("cut()");
    View* gview = activeView();
    if(gview) { gview->cut(); }
}

void MainWindow::copy()
{
    qDebug("copy()");
    View* gview = activeView();
    if(gview) { gview->copy(); }
}

void MainWindow::paste()
{
    qDebug("paste()");
    View* gview = activeView();
    if(gview) { gview->paste(); }
}

void MainWindow::selectAll()
{
    qDebug("selectAll()");
    View* gview = activeView();
    if(gview) { gview->selectAll(); }
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

void MainWindow::designDetails()
{
    QGraphicsScene* scene = activeScene();
    if(scene)
    {
        EmbDetailsDialog dialog(scene, this);
        dialog.exec();
    }
}

void MainWindow::about()
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

void MainWindow::whatsThisContextHelp()
{
    qDebug("whatsThisContextHelp()");
    QWhatsThis::enterWhatsThisMode();
}

void MainWindow::print()
{
    qDebug("print()");
    MdiWindow* mdiWin = qobject_cast<MdiWindow*>(mdiArea->activeSubWindow());
    if(mdiWin) { mdiWin->print(); }
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

    if (state.settings.general_current_tip >= state.tables["tips"].size()) {
        state.settings.general_current_tip = 0;
    }
    labelTipOfTheDay = new QLabel(state.tables["tips"].value(state.settings.general_current_tip), wizardTipOfTheDay);
    labelTipOfTheDay->setWordWrap(true);

    QCheckBox* checkBoxTipOfTheDay = new QCheckBox(tr("&Show tips on startup"), wizardTipOfTheDay);
    state.settings.general_tip_of_the_day = state.settings.general_tip_of_the_day;
    checkBoxTipOfTheDay->setChecked(state.settings.general_tip_of_the_day);
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
    state.settings.general_tip_of_the_day = checked;
}

void MainWindow::buttonTipOfTheDayClicked(int button)
{
    qDebug("buttonTipOfTheDayClicked(%d)", button);
    if(button == QWizard::CustomButton1)
    {
        if(state.settings.general_current_tip > 0)
            state.settings.general_current_tip--;
        else
            state.settings.general_current_tip = state.tables["tips"].size() - 1;
        labelTipOfTheDay->setText(state.tables["tips"].value(state.settings.general_current_tip));
    }
    else if(button == QWizard::CustomButton2)
    {
        state.settings.general_current_tip++;
        if(state.settings.general_current_tip >= state.tables["tips"].size())
            state.settings.general_current_tip = 0;
        labelTipOfTheDay->setText(state.tables["tips"].value(state.settings.general_current_tip));
    }
    else if(button == QWizard::CustomButton3)
    {
        wizardTipOfTheDay->close();
    }
}

void MainWindow::help()
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

void MainWindow::changelog()
{
    qDebug("changelog()");

    QUrl changelogURL("help/changelog.html");
    QDesktopServices::openUrl(changelogURL);
}

// Standard Slots
void MainWindow::undo()
{
    qDebug("undo()");
    QString prefix = prompt->getPrefix();
    if(dockUndoEdit->canUndo())
    {
        prompt->setPrefix("Undo " + dockUndoEdit->undoText());
        prompt->appendHistory(QString());
        dockUndoEdit->undo();
        prompt->setPrefix(prefix);
    }
    else
    {
        prompt->alert("Nothing to undo");
        prompt->setPrefix(prefix);
    }
}

void MainWindow::redo()
{
    qDebug("redo()");
    QString prefix = prompt->getPrefix();
    if(dockUndoEdit->canRedo())
    {
        prompt->setPrefix("Redo " + dockUndoEdit->redoText());
        prompt->appendHistory(QString());
        dockUndoEdit->redo();
        prompt->setPrefix(prefix);
    }
    else
    {
        prompt->alert("Nothing to redo");
        prompt->setPrefix(prefix);
    }
}

bool MainWindow::isShiftPressed()
{
    return shiftKeyPressedState;
}

void MainWindow::setShiftPressed()
{
    shiftKeyPressedState = true;
}

void MainWindow::setShiftReleased()
{
    shiftKeyPressedState = false;
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

    state.settings.general_icon_size = iconSize;
}

void MainWindow::icon16()
{
    qDebug("icon16()");
    iconResize(16);
}

void MainWindow::icon24()
{
    qDebug("icon24()");
    iconResize(24);
}

void MainWindow::icon32()
{
    qDebug("icon32()");
    iconResize(32);
}

void MainWindow::icon48()
{
    qDebug("icon48()");
    iconResize(48);
}

void MainWindow::icon64()
{
    qDebug("icon64()");
    iconResize(64);;
}

void MainWindow::icon128()
{
    qDebug("icon128()");
    iconResize(128);
}

MdiWindow* MainWindow::activeMdiWindow()
{
    qDebug("activeMdiWindow()");
    MdiWindow* mdiWin = qobject_cast<MdiWindow*>(mdiArea->activeSubWindow());
    return mdiWin;
}

View* MainWindow::activeView()
{
    qDebug("activeView()");
    MdiWindow* mdiWin = qobject_cast<MdiWindow*>(mdiArea->activeSubWindow());
    if(mdiWin)
    {
        View* v = mdiWin->getView();
        return v;
    }
    return 0;
}

QGraphicsScene* MainWindow::activeScene()
{
    qDebug("activeScene()");
    MdiWindow* mdiWin = qobject_cast<MdiWindow*>(mdiArea->activeSubWindow());
    if(mdiWin)
    {
        QGraphicsScene* s = mdiWin->getScene();
        return s;
    }
    return 0;
}

QUndoStack* MainWindow::activeUndoStack()
{
    qDebug("activeUndoStack()");
    View* v = activeView();
    if(v)
    {
        QUndoStack* u = v->getUndoStack();
        return u;
    }
    return 0;
}

void MainWindow::setUndoCleanIcon(bool opened)
{
    dockUndoEdit->updateCleanIcon(opened);
}

void MainWindow::updateAllViewScrollBars(bool val)
{
    QList<QMdiSubWindow*> windowList = mdiArea->subWindowList();
    for(int i = 0; i < windowList.count(); ++i)
    {
        MdiWindow* mdiWin = qobject_cast<MdiWindow*>(windowList.at(i));
        if(mdiWin) { mdiWin->showViewScrollBars(val); }
    }
}

void MainWindow::updateAllViewCrossHairColors(QRgb color)
{
    QList<QMdiSubWindow*> windowList = mdiArea->subWindowList();
    for(int i = 0; i < windowList.count(); ++i)
    {
        MdiWindow* mdiWin = qobject_cast<MdiWindow*>(windowList.at(i));
        if(mdiWin) { mdiWin->setViewCrossHairColor(color); }
    }
}

void MainWindow::updateAllViewBackgroundColors(QRgb color)
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
    state.settings.selection_mode_pickadd = val;
    dockPropEdit->updatePickAddModeButton(val);
}

void MainWindow::pickAddModeToggled()
{
    bool val = !state.settings.selection_mode_pickadd;
    updatePickAddMode(val);
}

// Layer ToolBar
void MainWindow::makeLayerActive()
{
    qDebug("makeLayerActive()");
    stub_implement("Implement makeLayerActive.");
}

void MainWindow::layerManager()
{
    qDebug("layerManager()");
    stub_implement("Implement layerManager.");
    LayerManager layman(this, this);
    layman.exec();
}

void MainWindow::layerPrevious()
{
    qDebug("layerPrevious()");
    stub_implement("Implement layerPrevious.");
}

// Zoom ToolBar
void MainWindow::zoomRealtime()
{
    qDebug("zoomRealtime()");
    stub_implement("Implement zoomRealtime.");
}

void MainWindow::zoomPrevious()
{
    qDebug("zoomPrevious()");
    stub_implement("Implement zoomPrevious.");
}

void MainWindow::zoomWindow()
{
    qDebug("zoomWindow()");
    View* gview = activeView();
    if(gview) { gview->zoomWindow(); }
}

void MainWindow::zoomDynamic()
{
    qDebug("zoomDynamic()");
    stub_implement("Implement zoomDynamic.");
}

void MainWindow::zoomScale()
{
    qDebug("zoomScale()");
    stub_implement("Implement zoomScale.");
}

void MainWindow::zoomCenter()
{
    qDebug("zoomCenter()");
    stub_implement("Implement zoomCenter.");
}

void MainWindow::zoomIn()
{
    qDebug("zoomIn()");
    View* gview = activeView();
    if(gview) { gview->zoomIn(); }
}

void MainWindow::zoomOut()
{
    qDebug("zoomOut()");
    View* gview = activeView();
    if(gview) { gview->zoomOut(); }
}

void MainWindow::zoomSelected()
{
    qDebug("zoomSelected()");
    View* gview = activeView();
    QUndoStack* stack = gview->getUndoStack();
    if(gview && stack)
    {
        UndoableNavCommand* cmd = new UndoableNavCommand("ZoomSelected", gview, 0);
        stack->push(cmd);
    }
}

void MainWindow::zoomAll()
{
    qDebug("zoomAll()");
    stub_implement("Implement zoomAll.");
}

void MainWindow::zoomExtents()
{
    qDebug("zoomExtents()");
    View* gview = activeView();
    QUndoStack* stack = gview->getUndoStack();
    if(gview && stack)
    {
        UndoableNavCommand* cmd = new UndoableNavCommand("ZoomExtents", gview, 0);
        stack->push(cmd);
    }
}
// Pan SubMenu
void MainWindow::panrealtime()
{
    qDebug("panrealtime()");
    View* gview = activeView();
    if(gview) { gview->panRealTime(); }
}

void MainWindow::panpoint()
{
    qDebug("panpoint()");
    View* gview = activeView();
    if(gview) { gview->panPoint(); }
}

void MainWindow::panLeft()
{
    qDebug("panLeft()");
    View* gview = activeView();
    QUndoStack* stack = gview->getUndoStack();
    if(gview && stack)
    {
        UndoableNavCommand* cmd = new UndoableNavCommand("PanLeft", gview, 0);
        stack->push(cmd);
    }
}

void MainWindow::panRight()
{
    qDebug("panRight()");
    View* gview = activeView();
    QUndoStack* stack = gview->getUndoStack();
    if(gview && stack)
    {
        UndoableNavCommand* cmd = new UndoableNavCommand("PanRight", gview, 0);
        stack->push(cmd);
    }
}

void MainWindow::panUp()
{
    qDebug("panUp()");
    View* gview = activeView();
    QUndoStack* stack = gview->getUndoStack();
    if(gview && stack)
    {
        UndoableNavCommand* cmd = new UndoableNavCommand("PanUp", gview, 0);
        stack->push(cmd);
    }
}

void MainWindow::panDown()
{
    qDebug("panDown()");
    View* gview = activeView();
    QUndoStack* stack = gview->getUndoStack();
    if(gview && stack)
    {
        UndoableNavCommand* cmd = new UndoableNavCommand("PanDown", gview, 0);
        stack->push(cmd);
    }
}

void MainWindow::dayVision()
{
    View* gview = activeView();
    if(gview)
    {
        gview->setBackgroundColor(qRgb(255,255,255)); //TODO: Make day vision color settings.
        gview->setCrossHairColor(qRgb(0,0,0));        //TODO: Make day vision color settings.
        gview->setGridColor(qRgb(0,0,0));             //TODO: Make day vision color settings.
    }
}

void MainWindow::nightVision()
{
    View* gview = activeView();
    if(gview)
    {
        gview->setBackgroundColor(qRgb(0,0,0));      //TODO: Make night vision color settings.
        gview->setCrossHairColor(qRgb(255,255,255)); //TODO: Make night vision color settings.
        gview->setGridColor(qRgb(255,255,255));      //TODO: Make night vision color settings.
    }
}

void MainWindow::doNothing()
{
    //This function intentionally does nothing.
    qDebug("doNothing()");
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
    setTextFont(font.family());
}

void MainWindow::textSizeSelectorIndexChanged(int index)
{
    qDebug("textSizeSelectorIndexChanged(%d)", index);
    /* TODO: check that the toReal() conversion is ok */
    state.settings.text_size = qFabs(textSizeSelector->itemData(index).toReal());
}

QString MainWindow::textFont()
{
    return state.settings.text_font;
}

qreal MainWindow::textSize()
{
    return state.settings.text_size;
}

qreal MainWindow::textAngle()
{
    return state.settings.text_angle;
}

bool MainWindow::textBold()
{
    return state.settings.text_style_bold;
}

bool MainWindow::textItalic()
{
    return state.settings.text_style_italic;
}

bool MainWindow::textUnderline()
{
    return state.settings.text_style_underline;
}

bool MainWindow::textStrikeOut()
{
    return state.settings.text_style_strikeout;
}

bool MainWindow::textOverline()
{
    return state.settings.text_style_overline;
}

void MainWindow::setTextFont(const QString& str)
{
    textFontSelector->setCurrentFont(QFont(str));
    state.settings.text_font = str;
}

void MainWindow::setTextSize(qreal num)
{
    state.settings.text_size = qFabs(num);
    int index = textSizeSelector->findText("Custom", Qt::MatchContains);
    if(index != -1)
        textSizeSelector->removeItem(index);
    textSizeSelector->addItem("Custom " + QString().setNum(num, 'f', 2) + " pt", num);
    index = textSizeSelector->findText("Custom", Qt::MatchContains);
    if(index != -1)
        textSizeSelector->setCurrentIndex(index);
}

void MainWindow::setTextAngle(qreal num)
{
    state.settings.text_angle = num;
}

void MainWindow::setTextBold(bool val)
{
    state.settings.text_style_bold = val;
}

void MainWindow::setTextItalic(bool val)
{
    state.settings.text_style_italic = val;
}

void MainWindow::setTextUnderline(bool val)
{
    state.settings.text_style_underline = val;
}

void MainWindow::setTextStrikeOut(bool val)
{
    state.settings.text_style_strikeout = val;
}

void MainWindow::setTextOverline(bool val)
{
    state.settings.text_style_overline = val;
}

QString MainWindow::getCurrentLayer()
{
    MdiWindow* mdiWin = qobject_cast<MdiWindow*>(mdiArea->activeSubWindow());
    if(mdiWin) { return mdiWin->getCurrentLayer(); }
    return "0";
}

QRgb MainWindow::getCurrentColor()
{
    MdiWindow* mdiWin = qobject_cast<MdiWindow*>(mdiArea->activeSubWindow());
    if(mdiWin) { return mdiWin->getCurrentColor(); }
    return 0; //TODO: return color ByLayer
}

QString MainWindow::getCurrentLineType()
{
    MdiWindow* mdiWin = qobject_cast<MdiWindow*>(mdiArea->activeSubWindow());
    if(mdiWin) { return mdiWin->getCurrentLineType(); }
    return "ByLayer";
}

QString MainWindow::getCurrentLineWeight()
{
    MdiWindow* mdiWin = qobject_cast<MdiWindow*>(mdiArea->activeSubWindow());
    if(mdiWin) { return mdiWin->getCurrentLineWeight(); }
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

void MainWindow::escapePressed()
{
    qDebug("escapePressed()");
    QApplication::setOverrideCursor(Qt::WaitCursor);
    MdiWindow* mdiWin = qobject_cast<MdiWindow*>(mdiArea->activeSubWindow());
    if(mdiWin) { mdiWin->escapePressed(); }
    QApplication::restoreOverrideCursor();

    nativeEndCommand();
}

void MainWindow::toggleGrid()
{
    qDebug("toggleGrid()");
    statusbar->statusBarGridButton->toggle();
}

void MainWindow::toggleRuler()
{
    qDebug("toggleRuler()");
    statusbar->statusBarRulerButton->toggle();
}

void MainWindow::toggleLwt()
{
    qDebug("toggleLwt()");
    statusbar->statusBarLwtButton->toggle();
}

void MainWindow::enablePromptRapidFire()
{
    prompt->enableRapidFire();
}

void MainWindow::disablePromptRapidFire()
{
    prompt->disableRapidFire();
}

void MainWindow::enableMoveRapidFire()
{
    View* gview = activeView();
    if(gview) gview->enableMoveRapidFire();
}

void MainWindow::disableMoveRapidFire()
{
    View* gview = activeView();
    if(gview) gview->disableMoveRapidFire();
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
    if(mdiWin) mdiWin->promptInputPrevious();
}

void MainWindow::promptInputNext()
{
    MdiWindow* mdiWin = activeMdiWindow();
    if(mdiWin) mdiWin->promptInputNext();
}

void MainWindow::runCommand()
{
    QAction* act = qobject_cast<QAction*>(sender());
    if (act) {
        qDebug("runCommand(%s)", qPrintable(act->objectName()));
        prompt->endCommand();
        prompt->setCurrentText(act->objectName());
        prompt->processInput();
    }
}

void MainWindow::runCommandMain(const QString& cmd)
{
    qDebug("runCommandMain(%s)", qPrintable(cmd));
    QString fileName = "commands/" + cmd + "/" + cmd + ".js";
    //if(!state.settings.selection_mode_pick_first) { nativeClearSelection(); } //TODO: Uncomment this line when post-selection is available
    engine.evaluate(cmd + "_main()", fileName);
}

void MainWindow::runCommandClick(const QString& cmd, qreal x, qreal y)
{
    qDebug("runCommandClick(%s, %.2f, %.2f)", qPrintable(cmd), x, y);
    QString fileName = "commands/" + cmd + "/" + cmd + ".js";
    engine.evaluate(cmd + "_click(" + QString().setNum(x) + "," + QString().setNum(-y) + ")", fileName);
}

void MainWindow::runCommandMove(const QString& cmd, qreal x, qreal y)
{
    qDebug("runCommandMove(%s, %.2f, %.2f)", qPrintable(cmd), x, y);
    QString fileName = "commands/" + cmd + "/" + cmd + ".js";
    engine.evaluate(cmd + "_move(" + QString().setNum(x) + "," + QString().setNum(-y) + ")", fileName);
}

void MainWindow::runCommandContext(const QString& cmd, const QString& str)
{
    qDebug("runCommandContext(%s, %s)", qPrintable(cmd), qPrintable(str));
    QString fileName = "commands/" + cmd + "/" + cmd + ".js";
    engine.evaluate(cmd + "_context('" + str.toUpper() + "')", fileName);
}

void MainWindow::runCommandPrompt(const QString& cmd, const QString& str)
{
    qDebug("runCommandPrompt(%s, %s)", qPrintable(cmd), qPrintable(str));
    QString fileName = "commands/" + cmd + "/" + cmd + ".js";
    //NOTE: Replace any special characters that will cause a syntax error
    QString safeStr = str;
    safeStr.replace("\\", "\\\\");
    safeStr.replace("\'", "\\\'");
    if(prompt->isRapidFireEnabled()) { engine.evaluate(cmd + "_prompt('" + safeStr + "')", fileName); }
    else                             { engine.evaluate(cmd + "_prompt('" + safeStr.toUpper() + "')", fileName); }

}

void MainWindow::nativeAlert(const QString& txt)
{
    prompt->alert(txt);
}

void MainWindow::nativeBlinkPrompt()
{
    prompt->startBlinking();
}

void MainWindow::nativeSetPromptPrefix(const QString& txt)
{
    prompt->setPrefix(txt);
}

void MainWindow::nativeAppendPromptHistory(const QString& txt)
{
    prompt->appendHistory(txt);
}

void MainWindow::nativeEnablePromptRapidFire()
{
    enablePromptRapidFire();
}

void MainWindow::nativeDisablePromptRapidFire()
{
    disablePromptRapidFire();
}

void MainWindow::nativeEnableMoveRapidFire()
{
    enableMoveRapidFire();
}

void MainWindow::nativeDisableMoveRapidFire()
{
    disableMoveRapidFire();
}

void MainWindow::nativeInitCommand()
{
    View* gview = activeView();
    if(gview) gview->clearRubberRoom();
}

void MainWindow::nativeEndCommand()
{
    View* gview = activeView();
    if(gview)
    {
        gview->clearRubberRoom();
        gview->previewOff();
        gview->disableMoveRapidFire();
    }
    prompt->endCommand();
}

void MainWindow::nativeNewFile()
{
    newFile();
}

void MainWindow::nativeOpenFile()
{
    openFile();
}

void MainWindow::nativeExit()
{
    exit();
}

void MainWindow::nativeHelp()
{
    help();
}

void MainWindow::nativeAbout()
{
    about();
}

void MainWindow::nativeTipOfTheDay()
{
    tipOfTheDay();
}

void MainWindow::nativeWindowCascade()
{
    mdiArea->cascade();
}

void MainWindow::nativeWindowTile()
{
    mdiArea->tile();
}

void MainWindow::nativeWindowClose()
{
    onCloseWindow();
}

void MainWindow::nativeWindowCloseAll()
{
    mdiArea->closeAllSubWindows();
}

void MainWindow::nativeWindowNext()
{
    mdiArea->activateNextSubWindow();
}

void MainWindow::nativeWindowPrevious()
{
    mdiArea->activatePreviousSubWindow();
}

QString MainWindow::nativePlatformString()
{
    return platformString();
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

void MainWindow::nativeUndo()
{
    undo();
}

void MainWindow::nativeRedo()
{
    redo();
}

void MainWindow::nativeIcon16()
{
    icon16();
}

void MainWindow::nativeIcon24()
{
    icon24();
}

void MainWindow::nativeIcon32()
{
    icon32();
}

void MainWindow::nativeIcon48()
{
    icon48();
}

void MainWindow::nativeIcon64()
{
    icon64();
}

void MainWindow::nativeIcon128()
{
    icon128();
}

void MainWindow::nativePanLeft()
{
    panLeft();
}

void MainWindow::nativePanRight()
{
    panRight();
}

void MainWindow::nativePanUp()
{
    panUp();
}

void MainWindow::nativePanDown()
{
    panDown();
}

void MainWindow::nativeZoomIn()
{
    zoomIn();
}

void MainWindow::nativeZoomOut()
{
    zoomOut();
}

void MainWindow::nativeZoomExtents()
{
    zoomExtents();
}

void MainWindow::nativePrintArea(qreal x, qreal y, qreal w, qreal h)
{
    qDebug("nativePrintArea(%.2f, %.2f, %.2f, %.2f)", x, y, w, h);
    //TODO: Print Setup Stuff
    print();
}

void MainWindow::nativeDayVision()
{
    dayVision();
}

void MainWindow::nativeNightVision()
{
    nightVision();
}

void MainWindow::nativeSetBackgroundColor(quint8 r, quint8 g, quint8 b)
{
    state.settings.display_bg_color = qRgb(r,g,b);
    updateAllViewBackgroundColors(qRgb(r,g,b));
}

void MainWindow::nativeSetCrossHairColor(quint8 r, quint8 g, quint8 b)
{
    state.settings.display_crosshair_color = qRgb(r,g,b);
    updateAllViewCrossHairColors(qRgb(r,g,b));
}

void MainWindow::nativeSetGridColor(quint8 r, quint8 g, quint8 b)
{
    state.settings.grid_color = qRgb(r,g,b);
    updateAllViewGridColors(qRgb(r,g,b));
}

QString MainWindow::nativeTextFont()
{
    return textFont();
}

qreal MainWindow::nativeTextSize()
{
    return textSize();
}

qreal MainWindow::nativeTextAngle()
{
    return textAngle();
}

bool MainWindow::nativeTextBold()
{
    return textBold();
}

bool MainWindow::nativeTextItalic()
{
    return textItalic();
}

bool MainWindow::nativeTextUnderline()
{
    return textUnderline();
}

bool MainWindow::nativeTextStrikeOut()
{
    return textStrikeOut();
}

bool MainWindow::nativeTextOverline()
{
    return textOverline();
}

void MainWindow::nativeSetTextFont(const QString& str)
{
    setTextFont(str);
}

void MainWindow::nativeSetTextSize(qreal num)
{
    setTextSize(num);
}

void MainWindow::nativeSetTextAngle(qreal num)
{
    setTextAngle(num);
}

void MainWindow::nativeSetTextBold(bool val)
{
    setTextBold(val);
}

void MainWindow::nativeSetTextItalic(bool val)
{
    setTextItalic(val);
}

void MainWindow::nativeSetTextUnderline(bool val)
{
    setTextUnderline(val);
}

void MainWindow::nativeSetTextStrikeOut(bool val)
{
    setTextStrikeOut(val);
}

void MainWindow::nativeSetTextOverline(bool val)
{
    setTextOverline(val);
}

void MainWindow::nativePreviewOn(int clone, int mode, qreal x, qreal y, qreal data)
{
    View* gview = activeView();
    if(gview) gview->previewOn(clone, mode, x, -y, data);
}

void MainWindow::nativePreviewOff()
{
    View* gview = activeView();
    if(gview) gview->previewOff();
}

void MainWindow::nativeVulcanize()
{
    View* gview = activeView();
    if(gview) gview->vulcanizeRubberRoom();
}

void MainWindow::nativeClearRubber()
{
    View* gview = activeView();
    if(gview) gview->clearRubberRoom();
}

bool MainWindow::nativeAllowRubber()
{
    View* gview = activeView();
    if(gview) return gview->allowRubber();
    return false;
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

void MainWindow::nativeSetRubberPoint(const QString& key, qreal x, qreal y)
{
    View* gview = activeView();
    if(gview) gview->setRubberPoint(key, QPointF(x, -y));
}

void MainWindow::nativeSetRubberText(const QString& key, const QString& txt)
{
    View* gview = activeView();
    if(gview) gview->setRubberText(key, txt);
}

void MainWindow::nativeAddTextMulti(const QString& str, qreal x, qreal y, qreal rot, bool fill, int rubberMode)
{
}

void MainWindow::nativeAddTextSingle(const QString& str, qreal x, qreal y, qreal rot, bool fill, int rubberMode)
{
    View* gview = activeView();
    QGraphicsScene* gscene = gview->scene();
    QUndoStack* stack = gview->getUndoStack();
    if(gview && gscene && stack)
    {
        TextSingleObject* obj = new TextSingleObject(str, x, -y, getCurrentColor());
        obj->setObjectTextFont(state.settings.text_font);
        obj->setObjectTextSize(state.settings.text_size);
        obj->setObjectTextStyle(state.settings.text_style_bold,
                                state.settings.text_style_italic,
                                state.settings.text_style_underline,
                                state.settings.text_style_strikeout,
                                state.settings.text_style_overline);
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

void MainWindow::nativeAddInfiniteLine(qreal x1, qreal y1, qreal x2, qreal y2, qreal rot)
{
}

void MainWindow::nativeAddRay(qreal x1, qreal y1, qreal x2, qreal y2, qreal rot)
{
}

void MainWindow::nativeAddLine(qreal x1, qreal y1, qreal x2, qreal y2, qreal rot, int rubberMode)
{
    View* gview = activeView();
    QGraphicsScene* gscene = gview->scene();
    QUndoStack* stack = gview->getUndoStack();
    if(gview && gscene && stack)
    {
        LineObject* obj = new LineObject(x1, -y1, x2, -y2, getCurrentColor());
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

void MainWindow::nativeAddTriangle(qreal x1, qreal y1, qreal x2, qreal y2, qreal x3, qreal y3, qreal rot, bool fill)
{
}

void MainWindow::nativeAddRectangle(qreal x, qreal y, qreal w, qreal h, qreal rot, bool fill, int rubberMode)
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

void MainWindow::nativeAddRoundedRectangle(qreal x, qreal y, qreal w, qreal h, qreal rad, qreal rot, bool fill)
{
}

void MainWindow::nativeAddArc(qreal startX, qreal startY, qreal midX, qreal midY, qreal endX, qreal endY, int rubberMode)
{
    View* gview = activeView();
    QGraphicsScene* scene = activeScene();
    if(gview && scene)
    {
        ArcObject* arcObj = new ArcObject(startX, -startY, midX, -midY, endX, -endY, getCurrentColor());
        arcObj->setObjectRubberMode(rubberMode);
        if(rubberMode) gview->addToRubberRoom(arcObj);
        scene->addItem(arcObj);
        scene->update();
    }
}

void MainWindow::nativeAddCircle(qreal centerX, qreal centerY, qreal radius, bool fill, int rubberMode)
{
    View* gview = activeView();
    QGraphicsScene* gscene = gview->scene();
    QUndoStack* stack = gview->getUndoStack();
    if(gview && gscene && stack)
    {
        CircleObject* obj = new CircleObject(centerX, -centerY, radius, getCurrentColor());
        obj->setObjectRubberMode(rubberMode);
        //TODO: circle fill
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

void MainWindow::nativeAddSlot(qreal centerX, qreal centerY, qreal diameter, qreal length, qreal rot, bool fill, int rubberMode)
{
    //TODO: Use UndoableAddCommand for slots
    /*
    SlotObject* slotObj = new SlotObject(centerX, -centerY, diameter, length, getCurrentColor());
    slotObj->setRotation(-rot);
    slotObj->setObjectRubberMode(rubberMode);
    if(rubberMode) gview->addToRubberRoom(slotObj);
    scene->addItem(slotObj);
    //TODO: slot fill
    scene->update();
    */
}

void MainWindow::nativeAddEllipse(qreal centerX, qreal centerY, qreal width, qreal height, qreal rot, bool fill, int rubberMode)
{
    View* gview = activeView();
    QGraphicsScene* gscene = gview->scene();
    QUndoStack* stack = gview->getUndoStack();
    if(gview && gscene && stack)
    {
        EllipseObject* obj = new EllipseObject(centerX, -centerY, width, height, getCurrentColor());
        obj->setRotation(-rot);
        obj->setObjectRubberMode(rubberMode);
        //TODO: ellipse fill
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

void MainWindow::nativeAddPoint(qreal x, qreal y)
{
    View* gview = activeView();
    QUndoStack* stack = gview->getUndoStack();
    if(gview && stack)
    {
        PointObject* obj = new PointObject(x, -y, getCurrentColor());
        UndoableAddCommand* cmd = new UndoableAddCommand(obj->data(OBJ_NAME).toString(), obj, gview, 0);
        stack->push(cmd);
    }
}

void MainWindow::nativeAddRegularPolygon(qreal centerX, qreal centerY, quint16 sides, quint8 mode, qreal rad, qreal rot, bool fill)
{
}

//NOTE: This native is different than the rest in that the Y+ is down (scripters need not worry about this)
void MainWindow::nativeAddPolygon(qreal startX, qreal startY, const QPainterPath& p, int rubberMode)
{
    View* gview = activeView();
    QGraphicsScene* gscene = gview->scene();
    QUndoStack* stack = gview->getUndoStack();
    if(gview && gscene && stack)
    {
        PolygonObject* obj = new PolygonObject(startX, startY, p, getCurrentColor());
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

//NOTE: This native is different than the rest in that the Y+ is down (scripters need not worry about this)
void MainWindow::nativeAddPolyline(qreal startX, qreal startY, const QPainterPath& p, int rubberMode)
{
    View* gview = activeView();
    QGraphicsScene* gscene = gview->scene();
    QUndoStack* stack = gview->getUndoStack();
    if(gview && gscene && stack)
    {
        PolylineObject* obj = new PolylineObject(startX, startY, p, getCurrentColor());
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

//NOTE: This native is different than the rest in that the Y+ is down (scripters need not worry about this)
void MainWindow::nativeAddPath(qreal startX, qreal startY, const QPainterPath& p, int rubberMode)
{
}

void MainWindow::nativeAddHorizontalDimension(qreal x1, qreal y1, qreal x2, qreal y2, qreal legHeight)
{
}

void MainWindow::nativeAddVerticalDimension(qreal x1, qreal y1, qreal x2, qreal y2, qreal legHeight)
{
}

void MainWindow::nativeAddImage(const QString& img, qreal x, qreal y, qreal w, qreal h, qreal rot)
{
}

void MainWindow::nativeAddDimLeader(qreal x1, qreal y1, qreal x2, qreal y2, qreal rot, int rubberMode)
{
    View* gview = activeView();
    QGraphicsScene* gscene = gview->scene();
    QUndoStack* stack = gview->getUndoStack();
    if(gview && gscene && stack)
    {
        DimLeaderObject* obj = new DimLeaderObject(x1, -y1, x2, -y2, getCurrentColor());
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
        #if QT_VERSION >= 0x040700
        else if(shape == "dragmove")        gview->setCursor(QCursor(Qt::DragMoveCursor));
        else if(shape == "dragcopy")        gview->setCursor(QCursor(Qt::DragCopyCursor));
        else if(shape == "draglink")        gview->setCursor(QCursor(Qt::DragLinkCursor));
        #endif

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
    qreal dx = px-x1;
    qreal dy = py-y1;
    norm.translate(dx, dy);
    QPointF iPoint;
    norm.intersects(line, &iPoint);
    return QLineF(px, py, iPoint.x(), iPoint.y()).length();
}

int MainWindow::nativeNumSelected()
{
    View* gview = activeView();
    if(gview) { return gview->numSelected(); }
    return 0;
}

void MainWindow::nativeSelectAll()
{
    View* gview = activeView();
    if(gview) { gview->selectAll(); }
}

void MainWindow::nativeAddToSelection(const QPainterPath path, Qt::ItemSelectionMode mode)
{
}

void MainWindow::nativeClearSelection()
{
    View* gview = activeView();
    if(gview) { gview->clearSelection(); }
}

void MainWindow::nativeDeleteSelected()
{
    View* gview = activeView();
    if(gview) { gview->deleteSelected(); }
}

void MainWindow::nativeCutSelected(qreal x, qreal y)
{
}

void MainWindow::nativeCopySelected(qreal x, qreal y)
{
}

void MainWindow::nativePasteSelected(qreal x, qreal y)
{
}

void MainWindow::nativeMoveSelected(qreal dx, qreal dy)
{
    View* gview = activeView();
    if(gview) { gview->moveSelected(dx, -dy); }
}

void MainWindow::nativeScaleSelected(qreal x, qreal y, qreal factor)
{
    if(factor <= 0.0)
    {
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

qreal MainWindow::nativeQSnapX()
{
    QGraphicsScene* scene = activeScene();
    if(scene) return scene->property(SCENE_QSNAP_POINT).toPointF().x();
    return 0.0;
}

qreal MainWindow::nativeQSnapY()
{
    QGraphicsScene* scene = activeScene();
    if(scene) return -scene->property(SCENE_QSNAP_POINT).toPointF().y();
    return 0.0;
}

qreal MainWindow::nativeMouseX()
{
    QGraphicsScene* scene = activeScene();
    if(scene) qDebug("mouseX: %.50f", scene->property(SCENE_MOUSE_POINT).toPointF().x());
    if(scene) return scene->property(SCENE_MOUSE_POINT).toPointF().x();
    return 0.0;
}

qreal MainWindow::nativeMouseY()
{
    QGraphicsScene* scene = activeScene();
    if(scene) qDebug("mouseY: %.50f", -scene->property(SCENE_MOUSE_POINT).toPointF().y());
    if(scene) return -scene->property(SCENE_MOUSE_POINT).toPointF().y();
    return 0.0;
}

/* kate: bom off; indent-mode cstyle; indent-width 4; replace-trailing-space-save on; */
