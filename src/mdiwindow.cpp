/*
 * Embroidermodder 2.
 *
 * Copyright 2011-2024 The Embroidermodder Team
 * Embroidermodder 2 is Open Source Software, see LICENSE.md for licensing terms.
 * Visit https://www.libembroidery.org/refman for advice on altering this file,
 * or read the markdown version in embroidermodder2/docs/refman.
 *
 * MdiWindow
 */

#include "embroidermodder.h"

#include <QFileDialog>
#include <QMessageBox>
#include <QApplication>
#include <QDir>
#include <QFile>
#include <QFileInfo>
#include <QMainWindow>
#include <QMdiArea>
#include <QMdiSubWindow>
#include <QStatusBar>
#include <QColor>
#include <QUndoStack>

#include <QGraphicsScene>
#include <QGraphicsView>
#include <QGraphicsItem>

MdiWindow::MdiWindow(const int theIndex, MainWindow* mw, QMdiArea* parent, Qt::WindowFlags wflags) : QMdiSubWindow(parent, wflags)
{
    mdiArea = parent;

    myIndex = theIndex;

    fileWasLoaded = false;

    setAttribute(Qt::WA_DeleteOnClose);

    QString aName;
    curFile = aName.asprintf("Untitled%d.dst", myIndex);
    this->setWindowTitle(curFile);

    this->setWindowIcon(QIcon("icons/" + _main->settings_general_icon_theme + "/" + "app" + ".png"));

    gscene = new QGraphicsScene(0,0,0,0, this);
    gview = new View(_main, gscene, this);

    setWidget(gview);

    //WARNING: DO NOT SET THE QMDISUBWINDOW (this) FOCUSPROXY TO THE PROMPT
    //WARNING: AS IT WILL CAUSE THE WINDOW MENU TO NOT SWITCH WINDOWS PROPERLY!
    //WARNING: ALTHOUGH IT SEEMS THAT SETTING INTERNAL WIDGETS FOCUSPROXY IS OK.
    gview->setFocusProxy(_main->prompt);

    resize(sizeHint());

    promptHistory = "Welcome to Embroidermodder 2!<br/>Open some of our sample files. Many formats are supported.<br/>For help, press F1.";
    _main->prompt->setHistory(promptHistory);
    promptInputList << "";
    promptInputNum = 0;

    curLayer = "0";
    curColor = 0; //TODO: color ByLayer
    curLineType = "ByLayer";
    curLineWeight = "ByLayer";

    // Due to strange Qt4.2.3 feature the child window icon is not drawn
    // in the main menu if showMaximized() is called for a non-visible child window
    // Therefore calling show() first...
    show();
    showMaximized();

    setFocusPolicy(Qt::WheelFocus);
    setFocus();

    onWindowActivated();

    pattern = emb_pattern_create();
    if (!pattern) {
        printf("Could not allocate memory for embroidery pattern\n");
        exit(1);
    }
}

MdiWindow::~MdiWindow()
{
    qDebug("MdiWindow Destructor()");
    emb_pattern_free(pattern);
}

bool
MdiWindow::saveFile(const QString &fileName)
{
    SaveObject saveObj(gscene, this);
    return saveObj.save(fileName);
}

bool
MdiWindow::loadFile(const QString &fileName)
{
    qDebug("MdiWindow loadFile()");

    QRgb tmpColor = getCurrentColor();

    QFile file(fileName);
    if (!file.open(QFile::ReadOnly | QFile::Text)) {
        QMessageBox::warning(this, tr("Error reading file"),
            tr("Cannot read file %1:\n%2.").arg(fileName).arg(file.errorString()));
        return false;
    }

    QApplication::setOverrideCursor(Qt::WaitCursor);

    QString ext = fileExtension(fileName);
    qDebug("ext: %s", qPrintable(ext));

    //Read
    int format = EMB_FORMAT_CSV; //emb_identify_format(qPrintable(fileName));
    if (format <= 0) {
        qDebug("Unsupported read file type: %s\n", qPrintable(fileName));
        QApplication::restoreOverrideCursor();
        QMessageBox::warning(this, tr("Error reading pattern"),
            tr("Unsupported read file type: ") + qPrintable(fileName));
        return false;
    }

    int readSuccessful = emb_pattern_read(pattern, qPrintable(fileName), format);
    if (!readSuccessful) {
        qDebug("Reading file was unsuccessful: %s\n", qPrintable(fileName));
        QApplication::restoreOverrideCursor();
        QMessageBox::warning(this, tr("Error reading pattern"),
            tr("Reading file was unsuccessful: ") + qPrintable(fileName));
        return false;
    }

    qDebug("Read successful.\n");
    //emb_pattern_moveStitchListToPolylines(pattern); //TODO: Test more
    EmbPolyline polyline;
    polyline.pointList = emb_array_create(EMB_VECTOR);
    polyline.flagList = emb_array_create(EMB_FLAG);
    polyline.lineType = 0;
    polyline.color.r = 0;
    polyline.color.g = 0;
    polyline.color.b = 0;
    for (int i=1; i<pattern->stitch_list->count; i++) {
        EmbVector v;
        v.x = pattern->stitch_list->stitch[i].x;
        v.y = pattern->stitch_list->stitch[i].y;
        emb_array_addVector(polyline.pointList, v);
    }
    emb_array_addPolyline(pattern->geometry, polyline);

    setCurrentFile(fileName);
    _main->statusbar->showMessage("File loaded.");

    if (_main->settings_grid_load_from_file) {
        //TODO: Josh, provide me a hoop size and/or grid spacing from the pattern.
    }

    QApplication::restoreOverrideCursor();

    //Clear the undo stack so it is not possible to undo past this point.
    gview->getUndoStack()->clear();

    setCurrentColor(tmpColor);

    fileWasLoaded = true;
    _main->setUndoCleanIcon(fileWasLoaded);
    return fileWasLoaded;
}

void
MdiWindow::print()
{
    QPrintDialog dialog(&printer, this);
    if (dialog.exec() == QDialog::Accepted) {
        QPainter painter(&printer);
        if (_main->settings_printing_disable_bg) {
            //Save current bg
            QBrush brush = gview->backgroundBrush();
            //Save ink by not printing the bg at all
            gview->setBackgroundBrush(Qt::NoBrush);
            //Print, fitting the viewport contents into a full page
            gview->render(&painter);
            //Restore the bg
            gview->setBackgroundBrush(brush);
        }
        else {
            //Print, fitting the viewport contents into a full page
            gview->render(&painter);
        }
    }
}

//TODO: Save a Brother PEL image (An 8bpp, 130x113 pixel monochromatic? bitmap image) Why 8bpp when only 1bpp is needed?

//TODO: Should BMC be limited to ~32KB or is this a mix up with Bitmap Cache?
//TODO: Is there/should there be other embedded data in the bitmap besides the image itself?
//NOTE: Can save a Singer BMC image (An 8bpp, 130x113 pixel colored bitmap image)
void
MdiWindow::saveBMC()
{
    //TODO: figure out how to center the image, right now it just plops it to the left side.
    QImage img(150, 150, QImage::Format_ARGB32_Premultiplied);
    img.fill(qRgb(255,255,255));
    QRectF extents = gscene->itemsBoundingRect();

    QPainter painter(&img);
    QRectF targetRect(0,0,150,150);
    if (_main->settings_printing_disable_bg) {
        //TODO: Make BMC background into it's own setting?
        QBrush brush = gscene->backgroundBrush();
        gscene->setBackgroundBrush(Qt::NoBrush);
        gscene->update();
        gscene->render(&painter, targetRect, extents, Qt::KeepAspectRatio);
        gscene->setBackgroundBrush(brush);
    }
    else {
        gscene->update();
        gscene->render(&painter, targetRect, extents, Qt::KeepAspectRatio);
    }

    img.convertToFormat(QImage::Format_Indexed8, Qt::ThresholdDither|Qt::AvoidDither).save("test.bmc", "BMP");
}

void
MdiWindow::setCurrentFile(const QString &fileName)
{
    curFile = QFileInfo(fileName).canonicalFilePath();
    setWindowModified(false);
    setWindowTitle(getShortCurrentFile());
}

QString
MdiWindow::getShortCurrentFile()
{
    return QFileInfo(curFile).fileName();
}

QString MdiWindow::fileExtension(const QString& fileName)
{
    return QFileInfo(fileName).suffix().toLower();
}

void MdiWindow::closeEvent(QCloseEvent* /*e*/)
{
    qDebug("MdiWindow closeEvent()");
    emit sendCloseMdiWin(this);
}

void MdiWindow::onWindowActivated()
{
    qDebug("MdiWindow onWindowActivated()");
    gview->getUndoStack()->setActive(true);
    _main->setUndoCleanIcon(fileWasLoaded);
    _main->statusbar->statusBarSnapButton->setChecked(gscene->property("ENABLE_SNAP").toBool());
    _main->statusbar->statusBarGridButton->setChecked(gscene->property("ENABLE_GRID").toBool());
    _main->statusbar->statusBarRulerButton->setChecked(gscene->property("ENABLE_RULER").toBool());
    _main->statusbar->statusBarOrthoButton->setChecked(gscene->property("ENABLE_ORTHO").toBool());
    _main->statusbar->statusBarPolarButton->setChecked(gscene->property("ENABLE_POLAR").toBool());
    _main->statusbar->statusBarQSnapButton->setChecked(gscene->property("ENABLE_QSNAP").toBool());
    _main->statusbar->statusBarQTrackButton->setChecked(gscene->property("ENABLE_QTRACK").toBool());
    _main->statusbar->statusBarLwtButton->setChecked(gscene->property("ENABLE_LWT").toBool());
    _main->prompt->setHistory(promptHistory);
}

QSize MdiWindow::sizeHint() const
{
    qDebug("MdiWindow sizeHint()");
    return QSize(450, 300);
}

void MdiWindow::currentLayerChanged(const QString& layer)
{
    curLayer = layer;
}

void MdiWindow::currentColorChanged(const QRgb& color)
{
    curColor = color;
}

void MdiWindow::currentLinetypeChanged(const QString& type)
{
    curLineType = type;
}

void MdiWindow::currentLineweightChanged(const QString& weight)
{
    curLineWeight = weight;
}

void MdiWindow::updateColorLinetypeLineweight()
{
}

void MdiWindow::deletePressed()
{
    gview->deletePressed();
}

void MdiWindow::escapePressed()
{
    gview->escapePressed();
}

void MdiWindow::showViewScrollBars(bool val)
{
    gview->showScrollBars(val);
}

void MdiWindow::setViewCrossHairColor(QRgb color)
{
    gview->setCrossHairColor(color);
}

void MdiWindow::setViewBackgroundColor(QRgb color)
{
    gview->setBackgroundColor(color);
}

void MdiWindow::setViewSelectBoxColors(QRgb colorL, QRgb fillL, QRgb colorR, QRgb fillR, int alpha)
{
    gview->setSelectBoxColors(colorL, fillL, colorR, fillR, alpha);
}

void MdiWindow::setViewGridColor(QRgb color)
{
    gview->setGridColor(color);
}

void MdiWindow::setViewRulerColor(QRgb color)
{
    gview->setRulerColor(color);
}

void MdiWindow::promptHistoryAppended(const QString& txt)
{
    promptHistory.append("<br/>" + txt);
}

void MdiWindow::logPromptInput(const QString& txt)
{
    promptInputList << txt;
    promptInputNum = promptInputList.size();
}

void MdiWindow::promptInputPrevious()
{
    promptInputPrevNext(true);
}

void MdiWindow::promptInputNext()
{
    promptInputPrevNext(false);
}

void MdiWindow::promptInputPrevNext(bool prev)
{
    if (promptInputList.isEmpty()) {
        if (prev) {
            QMessageBox::critical(this, tr("Prompt Previous Error"), tr("The prompt input is empty! Please report this as a bug!"));
        }
        else {
            QMessageBox::critical(this, tr("Prompt Next Error"),     tr("The prompt input is empty! Please report this as a bug!"));
        }
        qDebug("The prompt input is empty! Please report this as a bug!");
    }
    else {
        if (prev) {
            promptInputNum--;
        }
        else {
            promptInputNum++;
        }
        int maxNum = promptInputList.size();
        if (promptInputNum < 0) {
            promptInputNum = 0;
            _main->prompt->setCurrentText("");
        }
        else if (promptInputNum >= maxNum) {
            promptInputNum = maxNum;
            _main->prompt->setCurrentText("");
        }
        else {
            _main->prompt->setCurrentText(promptInputList.at(promptInputNum));
        }
    }
}
