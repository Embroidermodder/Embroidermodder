#include "mainwindow.h"
#include "view.h"
#include "statusbar.h"
#include "statusbar-button.h"
#include "imagewidget.h"
#include "layer-manager.h"
#include "object-data.h"
#include "object-arc.h"
#include "object-circle.h"
#include "object-ellipse.h"
#include "object-line.h"
#include "object-point.h"
#include "object-rect.h"
#include "object-dimleader.h"
#include "emb-rect.h"
#include "undo-editor.h"
#include "undo-commands.h"

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
#include "emb-pattern.h"
#include <vector>

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
    if(getSettingsPromptSaveHistory())
    {
        prompt->saveHistory("prompt.log", getSettingsPromptSaveHistoryAsHtml()); //TODO: get filename from settings
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

void MainWindow::designDetails()
{
//TODO: Reimplement designDetails() using the libembroidery C API
/*
    QApplication::setOverrideCursor(Qt::ArrowCursor);
    qDebug("designDetails()");
    QString appName = QApplication::applicationName();
    QString title = "Design Details";

    pattern p;
    if((MDIWindow*)mdiArea->activeSubWindow())
        p = ((MDIWindow*)mdiArea->activeSubWindow())->pattern;
    p.CalculateBoundingBox();
    embRect bounds = p.BoundingRect;

    int colors=1;
    int real_stitches=0;
    int jump_stitches=0;
    int trim_stitches=0;
    int unknown_stitches=0;
    double minx=0,maxx=0,miny=0,maxy=0;
    double min_stitchlength=999.0;
    double max_stitchlength=0.0;
    double total_stitchlength=0.0;
    int number_of_minlength_stitches=0;
    int number_of_maxlength_stitches=0;

    double xx=0,yy=0;
    double dx=0,dy=0;
    double length=0.0;

    if (p.stitchlist.size() ==0) {
        //messages.add("No design loaded\n");
        return;
    }
    std::vector<double> stitchLengths;

    int stitches = p.stitchlist.size();
    double totalColorLength = 0.0;
    for (int i=0;i<stitches;i++) {
        dx=p.stitchlist[i].xx-xx;
        dy=p.stitchlist[i].yy-yy;
        xx=p.stitchlist[i].xx;
        yy=p.stitchlist[i].yy;
        length=sqrt(dx * dx + dy * dy);
        totalColorLength += length;
        if (i>0 && p.stitchlist[i-1].flags!=NORMAL) length=0.0;	//can't count first normal stitch;
        if(!(p.stitchlist[i].flags & (JUMP | TRIM))) {
            real_stitches++;
            if(length>max_stitchlength) {max_stitchlength=length; number_of_maxlength_stitches=0;};
            if(length==max_stitchlength) number_of_maxlength_stitches++;
            if(length>0 && length<min_stitchlength)
            {
                min_stitchlength=length;
                number_of_minlength_stitches=0;
            };
            if(length==min_stitchlength) number_of_minlength_stitches++;
            total_stitchlength+=length;
            if(xx<minx) minx=xx;
            if(xx>maxx) maxx=xx;
            if(yy<miny) miny=yy;
            if(yy>maxy) maxy=yy;
        }
        if(p.stitchlist[i].flags & JUMP)
        {
            jump_stitches++;
        }
        if(p.stitchlist[i].flags & TRIM)
        {
            trim_stitches++;
        }
        if(p.stitchlist[i].flags & STOP)
        {
            stitchLengths.push_back(totalColorLength);
            totalColorLength = 0;
            colors++;
        }
        if(p.stitchlist[i].flags & END)
        {
            stitchLengths.push_back(totalColorLength);
        }
    }

    //second pass to fill bins now that we know max stitch length
#define NUMBINS 10
    int bin[NUMBINS+1];
    for (int i=0;i<=NUMBINS;i++)
    {
        bin[i]=0;
    }

    for (int i=0;i<stitches;i++) {
        dx=p.stitchlist[i].xx-xx;
        dy=p.stitchlist[i].yy-yy;
        xx=p.stitchlist[i].xx;
        yy=p.stitchlist[i].yy;
        if (i>0 && p.stitchlist[i-1].flags==NORMAL && p.stitchlist[i].flags == NORMAL) {
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

    QGridLayout *grid = new QGridLayout(this);
    grid->setSpacing(2);

    grid->addWidget(new QLabel(tr("Stitches:")),0,0,1,1);
    grid->addWidget(new QLabel(QString::number(p.stitchlist.size())), 0, 1, 1, 1);
    grid->addWidget(new QLabel(tr("Colors:")),1,0,1,1);
    grid->addWidget(new QLabel(QString::number(p.colorlist.size())), 1, 1, 1, 1);
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
    for(int i = 0; i < p.colorlist.size(); i++)
    {
        QFrame *frame = new QFrame();
        frame->setGeometry(0,0,30,30);
        QPalette palette = frame->palette();
        color t = p.colorlist[i].Color;
        palette.setColor(backgroundRole(), QColor( t.r, t.g, t.b ) );
        frame->setPalette( palette );
        frame->setAutoFillBackground(true);
        grid->addWidget(frame, currentRow,0,1,1);
        grid->addWidget(new QLabel(QString::number(stitchLengths[i]) + " mm"), currentRow,1,1,1);
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
*/
}

void MainWindow::about()
{
    //TODO: QTabWidget for about dialog
    QApplication::setOverrideCursor(Qt::ArrowCursor);
    qDebug("about()");
    QString appName = QApplication::applicationName();
    QString title = "About " + appName;

    QDialog dialog(this);
    ImageWidget img("images/logo-small");
    QLabel text(appName + tr("\n\n") +
                          tr("http://embroidermodder.sourceforge.net") +
                          tr("\n\n") +
                          tr("Available Platforms: GNU/Linux, Windows, Mac OSX, Raspberry Pi") +
                          tr("\n\n") +
                          tr("Embroidery formats by Josh Varga.") +
                          tr("\n") +
                          tr("User Interface by Jonathan Greig.") +
                          tr("\n\n") +
                          tr("Free under the zlib/libpng license."));
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

void MainWindow::print()
{
    qDebug("print()");
    if((MDIWindow*)mdiArea->activeSubWindow())
        ((MDIWindow*)mdiArea->activeSubWindow())->print();
}

void MainWindow::tipOfTheDay()
{
    qDebug("tipOfTheDay()");

    wizardTipOfTheDay = new QWizard(this);
    wizardTipOfTheDay->setAttribute(Qt::WA_DeleteOnClose);

    QWizardPage* page = new QWizardPage(wizardTipOfTheDay);
    page->setTitle("Did you know...");

    if(settings_general_current_tip >= listTipOfTheDay.size())
        settings_general_current_tip = 0;
    labelTipOfTheDay = new QLabel(listTipOfTheDay.at(settings_general_current_tip), wizardTipOfTheDay);
    labelTipOfTheDay->setWordWrap(true);

    QCheckBox* checkBoxTipOfTheDay = new QCheckBox(tr("&Show tips on startup"), wizardTipOfTheDay);
    settings_general_tip_of_the_day = mainWin->getSettingsGeneralTipOfTheDay();
    checkBoxTipOfTheDay->setChecked(settings_general_tip_of_the_day);
    connect(checkBoxTipOfTheDay, SIGNAL(stateChanged(int)), this, SLOT(checkBoxTipOfTheDayStateChanged(int)));

    QVBoxLayout* layout = new QVBoxLayout(wizardTipOfTheDay);
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
    settings_general_tip_of_the_day = checked;
}

void MainWindow::buttonTipOfTheDayClicked(int button)
{
    qDebug("buttonTipOfTheDayClicked(%d)", button);
    if(button == QWizard::CustomButton1)
    {
        if(settings_general_current_tip > 0)
            settings_general_current_tip--;
        else
            settings_general_current_tip = listTipOfTheDay.size()-1;
        labelTipOfTheDay->setText(listTipOfTheDay.at(settings_general_current_tip));
    }
    else if(button == QWizard::CustomButton2)
    {
        settings_general_current_tip++;
        if(settings_general_current_tip >= listTipOfTheDay.size())
            settings_general_current_tip = 0;
        labelTipOfTheDay->setText(listTipOfTheDay.at(settings_general_current_tip));
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
    QUrl helpURL("help/index.html");
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
    if(dockUndoEdit->canUndo())
    {
        prompt->appendHistory("Undo " + dockUndoEdit->undoText());
        dockUndoEdit->undo();
    }
    else
        prompt->appendHistory("Nothing to undo");
}

void MainWindow::redo()
{
    qDebug("redo()");
    if(dockUndoEdit->canRedo())
    {
        prompt->appendHistory("Redo " + dockUndoEdit->redoText());
        dockUndoEdit->redo();
    }
    else
        prompt->appendHistory("Nothing to redo");
}
// Icons
void MainWindow::iconResize(int iconSize)
{
    this->setIconSize(QSize(iconSize, iconSize));
    layerSelector->     setIconSize(QSize(iconSize*4, iconSize));
    colorSelector->     setIconSize(QSize(iconSize,   iconSize));
    linetypeSelector->  setIconSize(QSize(iconSize*4, iconSize));
    lineweightSelector->setIconSize(QSize(iconSize,   iconSize));
    //set the minimum combobox width so the text is always readable
    layerSelector->     setMinimumWidth(iconSize*4);
    colorSelector->     setMinimumWidth(iconSize*2);
    linetypeSelector->  setMinimumWidth(iconSize*4);
    lineweightSelector->setMinimumWidth(iconSize*2);

    //TODO: low-priority: open app with iconSize set to 128. resize the icons to a smaller size.

    setSettingsGeneralIconSize(iconSize);
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

View* MainWindow::activeView()
{
    qDebug("activeView()");
    MDIWindow* win = (MDIWindow*)mdiArea->activeSubWindow();
    if(win)
    {
        View* v = win->getView();
        return v;
    }
    return 0;
}

QGraphicsScene* MainWindow::activeScene()
{
    qDebug("activeScene()");
    MDIWindow* win = (MDIWindow*)mdiArea->activeSubWindow();
    if(win)
    {
        QGraphicsScene* s = win->getScene();
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

void MainWindow::updateAllViewScrollBars(bool val)
{
    QList<QMdiSubWindow*> windowList = mdiArea->subWindowList();
    for(int i = 0; i < windowList.count(); ++i)
    {
        if((MDIWindow*)windowList.at(i))
            ((MDIWindow*)windowList.at(i))->showViewScrollBars(val);
    }
}

void MainWindow::updateAllViewCrossHairColors(QRgb color)
{
    QList<QMdiSubWindow*> windowList = mdiArea->subWindowList();
    for(int i = 0; i < windowList.count(); ++i)
    {
        if((MDIWindow*)windowList.at(i))
            ((MDIWindow*)windowList.at(i))->setViewCrossHairColor(color);
    }
}

void MainWindow::updateAllViewBackgroundColors(QRgb color)
{
    QList<QMdiSubWindow*> windowList = mdiArea->subWindowList();
    for(int i = 0; i < windowList.count(); ++i)
    {
        if((MDIWindow*)windowList.at(i))
            ((MDIWindow*)windowList.at(i))->setViewBackgroundColor(color);
    }
}

void MainWindow::updateAllViewGridColors(QRgb color)
{
    QList<QMdiSubWindow*> windowList = mdiArea->subWindowList();
    for(int i = 0; i < windowList.count(); ++i)
    {
        if((MDIWindow*)windowList.at(i))
            ((MDIWindow*)windowList.at(i))->setViewGridColor(color);
    }
}

void MainWindow::updateAllViewRulerColors(QRgb color)
{
    QList<QMdiSubWindow*> windowList = mdiArea->subWindowList();
    for(int i = 0; i < windowList.count(); ++i)
    {
        if((MDIWindow*)windowList.at(i))
            ((MDIWindow*)windowList.at(i))->setViewRulerColor(color);
    }
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
void MainWindow::zoomrealtime()
{
    qDebug("zoomrealtime()");
    stub_implement("Implement zoomrealtime.");
}

void MainWindow::zoomprevious()
{
    qDebug("zoomprevious()");
    stub_implement("Implement zoomprevious.");
}

void MainWindow::zoomwindow()
{
    qDebug("zoomwindow()");
    View* gview = activeView();
    if(gview) { gview->zoomWindow(); }
}

void MainWindow::zoomdynamic()
{
    qDebug("zoomdynamic()");
    stub_implement("Implement zoomdynamic.");
}

void MainWindow::zoomscale()
{
    qDebug("zoomscale()");
    stub_implement("Implement zoomscale.");
}

void MainWindow::zoomcenter()
{
    qDebug("zoomcenter()");
    stub_implement("Implement zoomcenter.");
}

void MainWindow::zoomin()
{
    qDebug("zoomin()");
    View* gview = activeView();
    if(gview) { gview->zoomIn(); }
}

void MainWindow::zoomout()
{
    qDebug("zoomout()");
    View* gview = activeView();
    if(gview) { gview->zoomOut(); }
}

void MainWindow::zoomall()
{
    qDebug("zoomall()");
    stub_implement("Implement zoomall.");
}

void MainWindow::zoomextents()
{
    qDebug("zoomextents()");
    View* gview = activeView();
    if(gview) { gview->zoomExtents(); }
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

    if((MDIWindow*)mdiArea->activeSubWindow())
        ((MDIWindow*)mdiArea->activeSubWindow())->currentColorChanged(newColor);
}

void MainWindow::linetypeSelectorIndexChanged(int index)
{
    qDebug("linetypeSelectorIndexChanged(%d)", index);
}

void MainWindow::lineweightSelectorIndexChanged(int index)
{
    qDebug("lineweightSelectorIndexChanged(%d)", index);
}

QString MainWindow::getCurrentLayer()
{
    if((MDIWindow*)mdiArea->activeSubWindow())
        return ((MDIWindow*)mdiArea->activeSubWindow())->getCurrentLayer();
    return "0";
}

QRgb MainWindow::getCurrentColor()
{
    if((MDIWindow*)mdiArea->activeSubWindow())
        return ((MDIWindow*)mdiArea->activeSubWindow())->getCurrentColor();
    return 0; //TODO: return color ByLayer
}

QString MainWindow::getCurrentLineType()
{
    if((MDIWindow*)mdiArea->activeSubWindow())
        return ((MDIWindow*)mdiArea->activeSubWindow())->getCurrentLineType();
    return "ByLayer";
}

QString MainWindow::getCurrentLineWeight()
{
    if((MDIWindow*)mdiArea->activeSubWindow())
        return ((MDIWindow*)mdiArea->activeSubWindow())->getCurrentLineWeight();
    return "ByLayer";
}

void MainWindow::deletePressed()
{
    qDebug("deletePressed()");
    QApplication::setOverrideCursor(Qt::WaitCursor);
    if((MDIWindow*)mdiArea->activeSubWindow())
        ((MDIWindow*)mdiArea->activeSubWindow())->deletePressed();
    QApplication::restoreOverrideCursor();
}

void MainWindow::escapePressed()
{
    qDebug("escapePressed()");
    QApplication::setOverrideCursor(Qt::WaitCursor);
    if((MDIWindow*)mdiArea->activeSubWindow())
        ((MDIWindow*)mdiArea->activeSubWindow())->escapePressed();
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

void MainWindow::runCommand()
{
    QAction* act = qobject_cast<QAction*>(sender());
    if(act)
    {
        qDebug("runCommand(%s)", qPrintable(act->objectName()));
        prompt->endCommand();
        prompt->setCurrentText(act->objectName());
        prompt->processInput();
    }
}

void MainWindow::runCommandMain(const QString& cmd)
{
    qDebug("runCommandMain(%s)", qPrintable(cmd));
    engine->evaluate(cmd + "_main()");
}

void MainWindow::runCommandClick(const QString& cmd, qreal x, qreal y)
{
    qDebug("runCommandClick(%s, %.2f, %.2f)", qPrintable(cmd), x, y);
    engine->evaluate(cmd + "_click(" + QString().setNum(x) + "," + QString().setNum(-y) + ")");
}

void MainWindow::runCommandContext(const QString& cmd, const QString& str)
{
    qDebug("runCommandContext(%s, %s)", qPrintable(cmd), qPrintable(str));
    engine->evaluate(cmd + "_context('" + str.toUpper() + "')");
}

void MainWindow::runCommandPrompt(const QString& cmd, const QString& str)
{
    qDebug("runCommandPrompt(%s, %s)", qPrintable(cmd), qPrintable(str));
    engine->evaluate(cmd + "_prompt('" + str.toUpper() + "')");

}

void MainWindow::nativeSetPromptPrefix(const QString& txt)
{
    prompt->setPrefix(txt);
}

void MainWindow::nativeAppendPromptHistory(const QString& txt)
{
    prompt->appendHistory(txt);
}

void MainWindow::nativeEndCommand()
{
    View* gview = activeView();
    if(gview) gview->clearRubberRoom();

    prompt->endCommand();
}

void MainWindow::nativeHelp()
{
    help();
}

void MainWindow::nativeUndo()
{
    undo();
}

void MainWindow::nativeRedo()
{
    redo();
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

void MainWindow::nativePrintArea(qreal x, qreal y, qreal w, qreal h)
{
    qDebug("nativePrintArea(%.2f, %.2f, %.2f, %.2f)", x, y, w, h);
    //TODO: Print Setup Stuff
    print();
}

void MainWindow::nativeSetTextFont(const QString& str)
{
}

void MainWindow::nativeSetTextSize(quint32 num)
{
}

void MainWindow::nativeSetTextBold(bool val)
{
}

void MainWindow::nativeSetTextItalic(bool val)
{
}

void MainWindow::nativeSetTextUnderline(bool val)
{
}

void MainWindow::nativeSetTextStrikeOut(bool val)
{
}

void MainWindow::nativeSetTextOverline(bool val)
{
}

void MainWindow::nativeVulcanize()
{
    View* gview = activeView();
    if(gview) return gview->vulcanizeRubberRoom();
}

bool MainWindow::nativeAllowRubber()
{
    View* gview = activeView();
    if(gview) return gview->allowRubber();
    return false;
}

void MainWindow::nativeSetRubberMode(int mode)
{
    View* gview = activeView();
    if(gview) return gview->setRubberMode(mode);
}

void MainWindow::nativeSetRubberPoint(const QString& key, qreal x, qreal y)
{
    View* gview = activeView();
    if(gview) return gview->setRubberPoint(key, QPointF(x, -y));
}

void MainWindow::nativeAddTextMulti(const QString& str, qreal x, qreal y, bool fill)
{
}

void MainWindow::nativeAddTextSingle(const QString& str, qreal x, qreal y, bool fill)
{
    QGraphicsScene* scene = activeScene();
    if(scene)
    {
        QPainterPath textPath;
        textPath.addText(0, 0, QFont("Impact"), str);
        QGraphicsPathItem* textItem = scene->addPath(textPath);
        //QGraphicsSimpleTextItem* textItem = scene->addSimpleText(str, QFont("Impact"));
        textItem->setData(OBJ_TYPE, OBJ_TYPE_TEXTSINGLE);
        textItem->setPos(x,-y);
        textItem->setFlag(QGraphicsItem::ItemIsMovable, true);
        textItem->setFlag(QGraphicsItem::ItemIsSelectable, true);
        scene->update();
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

void MainWindow::nativeAddPolygon(qreal startX, qreal startY)
{
}

void MainWindow::nativeAddPolyline(qreal startX, qreal startY)
{
}

void MainWindow::nativeAddPath(qreal startX, qreal startY)
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
    norm.intersect(line, &iPoint);
    return QLineF(px, py, iPoint.x(), iPoint.y()).length();
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
}

void MainWindow::nativeScaleSelected(qreal x, qreal y, qreal factor)
{
}

void MainWindow::nativeRotateSelected(qreal x, qreal y, qreal rot)
{
    View* gview = activeView();
    if(gview) { gview->rotateSelected(x, y, -rot); }
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
