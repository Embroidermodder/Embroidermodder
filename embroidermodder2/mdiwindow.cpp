#include "mdiwindow.h"
#include "view.h"
#include "statusbar.h"
#include "statusbar-button.h"
#include "object-save.h"
#include "object-data.h"
#include "object-path.h"
#include "object-polygon.h"
#include "object-polyline.h"

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

#include "emb-reader-writer.h"

MDIWindow::MDIWindow(const int theIndex, MainWindow* mw, QMdiArea* parent, Qt::WindowFlags wflags) : QMdiSubWindow(parent, wflags)
{
    mainWin = mw;
    mdiArea = parent;

    myIndex = theIndex;

    setAttribute(Qt::WA_DeleteOnClose);

    QString aName;
    curFile = aName.sprintf("Untitled%d.dst", myIndex);
    this->setWindowTitle(curFile);

    this->setWindowIcon(QIcon("icons/" + mainWin->getSettingsGeneralIconTheme() + "/" + "app" + ".png"));

    gscene = new QGraphicsScene(0,0,0,0, this);
    gview = new View(mainWin, gscene, this);

    setWidget(gview);

    //WARNING: DO NOT SET THE QMDISUBWINDOW (this) FOCUSPROXY TO THE PROMPT
    //WARNING: AS IT WILL CAUSE THE WINDOW MENU TO NOT SWITCH WINDOWS PROPERLY!
    //WARNING: ALTHOUGH IT SEEMS THAT SETTING INTERNAL WIDGETS FOCUSPROXY IS OK.
    gview->setFocusProxy(mainWin->prompt);

    resize(sizeHint());

    promptHistory = "Welcome to Embroidermodder 2!<br/>Open some of our sample files. Many formats are supported.<br/>For help, press F1.";
    mainWin->prompt->setHistory(promptHistory);
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
}

MDIWindow::~MDIWindow()
{
    qDebug("MDIWindow Destructor()");
}

bool MDIWindow::saveFile(const QString &fileName)
{
    SaveObject saveObj(gscene, this);
    return saveObj.save(fileName);
}

bool MDIWindow::loadFile(const QString &fileName)
{
    qDebug("MDIWindow loadFile()");

    QRgb tmpColor = getCurrentColor();

    QFile file(fileName);
    if(!file.open(QFile::ReadOnly | QFile::Text))
    {
        QMessageBox::warning(this, tr("Error reading file"),
                             tr("Cannot read file %1:\n%2.")
                             .arg(fileName)
                             .arg(file.errorString()));
        return false;
    }

    QApplication::setOverrideCursor(Qt::WaitCursor);

    QString ext = fileExtension(fileName);
    qDebug("ext: %s", qPrintable(ext));

    //Read
    EmbPattern* p = embPattern_create();
    if(!p) { printf("Could not allocate memory for embroidery pattern\n"); exit(1); }
    int readSuccessful = 0;
    QString readError;
    EmbReaderWriter* reader = embReaderWriter_getByFileName(qPrintable(fileName));
    if(!reader)
    {
        readSuccessful = 0;
        readError = "Unsupported read file type: " + fileName;
        qDebug("Unsupported read file type: %s\n", qPrintable(fileName));
    }
    else
    {
        readSuccessful = reader->reader(p, qPrintable(fileName));
        if(!readSuccessful)
        {
            readError = "Reading file was unsuccessful: " + fileName;
            qDebug("Reading file was unsuccessful: %s\n", qPrintable(fileName));
        }
    }
    free(reader);
    if(!readSuccessful)
    {
        QMessageBox::warning(this, tr("Error reading pattern"), tr(qPrintable(readError)));
    }

    if(readSuccessful)
    {
        embPattern_moveStitchListToPolylines(p); //TODO: Test more
        int stitchCount = embStitchList_count(p->stitchList);
        QPainterPath path;

        if(p->circleObjList)
        {
            EmbCircleObjectList* curCircleObj = p->circleObjList;
            while(curCircleObj)
            {
                EmbCircle c = curCircleObj->circleObj.circle;
                EmbColor thisColor = curCircleObj->circleObj.color;
                setCurrentColor(qRgb(thisColor.r, thisColor.g, thisColor.b));
                //NOTE: With natives, the Y+ is up and libembroidery Y+ is up, so inverting the Y is NOT needed.
                mainWin->nativeAddCircle(embCircle_centerX(c), embCircle_centerY(c), embCircle_radius(c), false, OBJ_RUBBER_OFF); //TODO: fill
                curCircleObj = curCircleObj->next;
            }
        }
        if(p->ellipseObjList)
        {
            EmbEllipseObjectList* curEllipseObj = p->ellipseObjList;
            while(curEllipseObj)
            {
                EmbEllipse e = curEllipseObj->ellipseObj.ellipse;
                EmbColor thisColor = curEllipseObj->ellipseObj.color;
                setCurrentColor(qRgb(thisColor.r, thisColor.g, thisColor.b));
                //NOTE: With natives, the Y+ is up and libembroidery Y+ is up, so inverting the Y is NOT needed.
                mainWin->nativeAddEllipse(embEllipse_centerX(e), embEllipse_centerY(e), embEllipse_width(e), embEllipse_height(e), 0, false, OBJ_RUBBER_OFF); //TODO: rotation and fill
                curEllipseObj = curEllipseObj->next;
            }
        }
        if(p->lineObjList)
        {
            EmbLineObjectList* curLineObj = p->lineObjList;
            while(curLineObj)
            {
                EmbLine li = curLineObj->lineObj.line;
                EmbColor thisColor = curLineObj->lineObj.color;
                setCurrentColor(qRgb(thisColor.r, thisColor.g, thisColor.b));
                //NOTE: With natives, the Y+ is up and libembroidery Y+ is up, so inverting the Y is NOT needed.
                mainWin->nativeAddLine(embLine_x1(li), embLine_y1(li), embLine_x2(li), embLine_y2(li), 0, OBJ_RUBBER_OFF); //TODO: rotation
                curLineObj = curLineObj->next;
            }
        }
        if(p->pathObjList)
        {
            //TODO: This is unfinished. It needs more work
            EmbPathObjectList* curPathObjList = p->pathObjList;
            while(curPathObjList)
            {
                QPainterPath pathPath;
                EmbPointList* curPointList = curPathObjList->pathObj->pointList;
                EmbColor thisColor = curPathObjList->pathObj->color;
                if(curPointList)
                {
                    EmbPoint pp = curPointList->point;
                    pathPath.moveTo(embPoint_x(pp), -embPoint_y(pp)); //NOTE: Qt Y+ is down and libembroidery Y+ is up, so inverting the Y is needed.
                    curPointList = curPointList->next;
                }
                while(curPointList)
                {
                    EmbPoint pp = curPointList->point;
                    pathPath.lineTo(embPoint_x(pp), -embPoint_y(pp)); //NOTE: Qt Y+ is down and libembroidery Y+ is up, so inverting the Y is needed.
                    curPointList = curPointList->next;
                }

                QPen loadPen(qRgb(thisColor.r, thisColor.g, thisColor.b));
                loadPen.setWidthF(0.35);
                loadPen.setCapStyle(Qt::RoundCap);
                loadPen.setJoinStyle(Qt::RoundJoin);

                PathObject* obj = new PathObject(0,0, pathPath, loadPen.color().rgb());
                obj->setObjectRubberMode(OBJ_RUBBER_OFF);
                gscene->addItem(obj);

                curPathObjList = curPathObjList->next;
            }
        }
        if(p->pointObjList)
        {
            EmbPointObjectList* curPointObj = p->pointObjList;
            while(curPointObj)
            {
                EmbPoint po = curPointObj->pointObj.point;
                EmbColor thisColor = curPointObj->pointObj.color;
                setCurrentColor(qRgb(thisColor.r, thisColor.g, thisColor.b));
                //NOTE: With natives, the Y+ is up and libembroidery Y+ is up, so inverting the Y is NOT needed.
                mainWin->nativeAddPoint(embPoint_x(po), embPoint_y(po));
                curPointObj = curPointObj->next;
            }
        }
        if(p->polygonObjList)
        {
            EmbPolygonObjectList* curPolygonObjList = p->polygonObjList;
            while(curPolygonObjList)
            {
                QPainterPath polygonPath;
                bool firstPoint = false;
                qreal startX = 0, startY = 0;
                qreal x = 0, y = 0;
                EmbPointList* curPointList = curPolygonObjList->polygonObj->pointList;
                EmbColor thisColor = curPolygonObjList->polygonObj->color;
                setCurrentColor(qRgb(thisColor.r, thisColor.g, thisColor.b));
                while(curPointList)
                {
                    EmbPoint pp = curPointList->point;
                    x = embPoint_x(pp);
                    y = -embPoint_y(pp); //NOTE: Qt Y+ is down and libembroidery Y+ is up, so inverting the Y is needed.

                    if(firstPoint) { polygonPath.lineTo(x,y); }
                    else           { polygonPath.moveTo(x,y); firstPoint = true; startX = x; startY = y; }

                    curPointList = curPointList->next;
                }

                polygonPath.translate(-startX, -startY);
                mainWin->nativeAddPolygon(startX, startY, polygonPath, OBJ_RUBBER_OFF);

                curPolygonObjList = curPolygonObjList->next;
            }
        }
        /* NOTE: Polylines should only contain NORMAL stitches. */
        if(p->polylineObjList)
        {
            EmbPolylineObjectList* curPolylineObjList = p->polylineObjList;
            while(curPolylineObjList)
            {
                QPainterPath polylinePath;
                bool firstPoint = false;
                qreal startX = 0, startY = 0;
                qreal x = 0, y = 0;
                EmbPointList* curPointList = curPolylineObjList->polylineObj->pointList;
                EmbColor thisColor = curPolylineObjList->polylineObj->color;
                setCurrentColor(qRgb(thisColor.r, thisColor.g, thisColor.b));
                while(curPointList)
                {
                    EmbPoint pp = curPointList->point;
                    x = embPoint_x(pp);
                    y = -embPoint_y(pp); //NOTE: Qt Y+ is down and libembroidery Y+ is up, so inverting the Y is needed.

                    if(firstPoint) { polylinePath.lineTo(x,y); }
                    else           { polylinePath.moveTo(x,y); firstPoint = true; startX = x; startY = y; }

                    curPointList = curPointList->next;
                }

                polylinePath.translate(-startX, -startY);
                mainWin->nativeAddPolyline(startX, startY, polylinePath, OBJ_RUBBER_OFF);

                curPolylineObjList = curPolylineObjList->next;
            }
        }
        if(p->rectObjList)
        {
            EmbRectObjectList* curRectObj = p->rectObjList;
            while(curRectObj)
            {
                EmbRect r = curRectObj->rectObj.rect;
                EmbColor thisColor = curRectObj->rectObj.color;
                setCurrentColor(qRgb(thisColor.r, thisColor.g, thisColor.b));
                //NOTE: With natives, the Y+ is up and libembroidery Y+ is up, so inverting the Y is NOT needed.
                mainWin->nativeAddRectangle(embRect_x(r), embRect_y(r), embRect_width(r), embRect_height(r), 0, false, OBJ_RUBBER_OFF); //TODO: rotation and fill
                curRectObj = curRectObj->next;
            }
        }

        setCurrentFile(fileName);
        mainWin->statusbar->showMessage("File loaded.");
        QString stitches;
        stitches.setNum(stitchCount);

        if(mainWin->getSettingsGridLoadFromFile())
        {
            //TODO: Josh, provide me a hoop size and/or grid spacing from the pattern.
        }

        QApplication::restoreOverrideCursor();
    }
    else
    {
        QApplication::restoreOverrideCursor();
        QMessageBox::warning(this, tr("Error reading pattern"), tr("Cannot read pattern"));
    }
    embPattern_free(p);

    //Clear the undo stack so it is not possible to undo past this point.
    gview->getUndoStack()->clear();

    setCurrentColor(tmpColor);

    return true;
}

void MDIWindow::print()
{
    QPrintDialog dialog(&printer, this);
    if(dialog.exec() == QDialog::Accepted)
    {
        QPainter painter(&printer);
        if(mainWin->getSettingsPrintingDisableBG())
        {
            //Save current bg
            QBrush brush = gview->backgroundBrush();
            //Save ink by not printing the bg at all
            gview->setBackgroundBrush(Qt::NoBrush);
            //Print, fitting the viewport contents into a full page
            gview->render(&painter);
            //Restore the bg
            gview->setBackgroundBrush(brush);
        }
        else
        {
            //Print, fitting the viewport contents into a full page
            gview->render(&painter);
        }
    }
}

//TODO: Save a Brother PEL image (An 8bpp, 130x113 pixel monochromatic? bitmap image) Why 8bpp when only 1bpp is needed?

//TODO: Should BMC be limited to ~32KB or is this a mix up with Bitmap Cache?
//TODO: Is there/should there be other embedded data in the bitmap besides the image itself?
//NOTE: Can save a Singer BMC image (An 8bpp, 130x113 pixel colored bitmap image)
void MDIWindow::saveBMC()
{
    //TODO: figure out how to center the image, right now it just plops it to the left side.
    QImage img(150, 150, QImage::Format_ARGB32_Premultiplied);
    img.fill(qRgb(255,255,255));
    QRectF extents = gscene->itemsBoundingRect();

    QPainter painter(&img);
    QRectF targetRect(0,0,150,150);
    if(mainWin->getSettingsPrintingDisableBG()) //TODO: Make BMC background into it's own setting?
    {
        QBrush brush = gscene->backgroundBrush();
        gscene->setBackgroundBrush(Qt::NoBrush);
        gscene->update();
        gscene->render(&painter, targetRect, extents, Qt::KeepAspectRatio);
        gscene->setBackgroundBrush(brush);
    }
    else
    {
        gscene->update();
        gscene->render(&painter, targetRect, extents, Qt::KeepAspectRatio);
    }

    img.convertToFormat(QImage::Format_Indexed8, Qt::ThresholdDither|Qt::AvoidDither).save("test.bmc", "BMP");
}

void MDIWindow::setCurrentFile(const QString &fileName)
{
    curFile = QFileInfo(fileName).canonicalFilePath();
    setWindowModified(false);
    setWindowTitle(getShortCurrentFile());
}

QString MDIWindow::getShortCurrentFile()
{
    return QFileInfo(curFile).fileName();
}

QString MDIWindow::fileExtension(const QString& fileName)
{
    return QFileInfo(fileName).suffix().toLower();
}

void MDIWindow::closeEvent(QCloseEvent* /*e*/)
{
    qDebug("MDIWindow closeEvent()");
    emit sendCloseMdiWin(this);
}

void MDIWindow::onWindowActivated()
{
    qDebug("MDIWindow onWindowActivated()");
    gview->getUndoStack()->setActive(true);
    mainWin->statusbar->statusBarSnapButton->setChecked(gscene->property(ENABLE_SNAP).toBool());
    mainWin->statusbar->statusBarGridButton->setChecked(gscene->property(ENABLE_GRID).toBool());
    mainWin->statusbar->statusBarRulerButton->setChecked(gscene->property(ENABLE_RULER).toBool());
    mainWin->statusbar->statusBarOrthoButton->setChecked(gscene->property(ENABLE_ORTHO).toBool());
    mainWin->statusbar->statusBarPolarButton->setChecked(gscene->property(ENABLE_POLAR).toBool());
    mainWin->statusbar->statusBarQSnapButton->setChecked(gscene->property(ENABLE_QSNAP).toBool());
    mainWin->statusbar->statusBarQTrackButton->setChecked(gscene->property(ENABLE_QTRACK).toBool());
    mainWin->statusbar->statusBarLwtButton->setChecked(gscene->property(ENABLE_LWT).toBool());
    mainWin->prompt->setHistory(promptHistory);
}

QSize MDIWindow::sizeHint() const
{
    qDebug("MDIWindow sizeHint()");
    return QSize(450, 300);
}

void MDIWindow::currentLayerChanged(const QString& layer)
{
    curLayer = layer;
}

void MDIWindow::currentColorChanged(const QRgb& color)
{
    curColor = color;
}

void MDIWindow::currentLinetypeChanged(const QString& type)
{
    curLineType = type;
}

void MDIWindow::currentLineweightChanged(const QString& weight)
{
    curLineWeight = weight;
}

void MDIWindow::updateColorLinetypeLineweight()
{
}

void MDIWindow::deletePressed()
{
    gview->deletePressed();
}

void MDIWindow::escapePressed()
{
    gview->escapePressed();
}

void MDIWindow::showViewScrollBars(bool val)
{
    gview->showScrollBars(val);
}

void MDIWindow::setViewCrossHairColor(QRgb color)
{
    gview->setCrossHairColor(color);
}

void MDIWindow::setViewBackgroundColor(QRgb color)
{
    gview->setBackgroundColor(color);
}

void MDIWindow::setViewSelectBoxColors(QRgb colorL, QRgb fillL, QRgb colorR, QRgb fillR, int alpha)
{
    gview->setSelectBoxColors(colorL, fillL, colorR, fillR, alpha);
}

void MDIWindow::setViewGridColor(QRgb color)
{
    gview->setGridColor(color);
}

void MDIWindow::setViewRulerColor(QRgb color)
{
    gview->setRulerColor(color);
}

void MDIWindow::promptHistoryAppended(const QString& txt)
{
    promptHistory.append("<br/>" + txt);
}

void MDIWindow::logPromptInput(const QString& txt)
{
    promptInputList << txt;
    promptInputNum = promptInputList.size();
}

void MDIWindow::promptInputPrevious()
{
    promptInputPrevNext(true);
}

void MDIWindow::promptInputNext()
{
    promptInputPrevNext(false);
}

void MDIWindow::promptInputPrevNext(bool prev)
{
    if(promptInputList.isEmpty())
    {
        if(prev) QMessageBox::critical(this, tr("Prompt Previous Error"), tr("The prompt input is empty! Please report this as a bug!"));
        else     QMessageBox::critical(this, tr("Prompt Next Error"),     tr("The prompt input is empty! Please report this as a bug!"));
        qDebug("The prompt input is empty! Please report this as a bug!");
    }
    else
    {
        if(prev) promptInputNum--;
        else     promptInputNum++;
        int maxNum = promptInputList.size();
        if     (promptInputNum < 0)       { promptInputNum = 0;      mainWin->prompt->setCurrentText(""); }
        else if(promptInputNum >= maxNum) { promptInputNum = maxNum; mainWin->prompt->setCurrentText(""); }
        else                              { mainWin->prompt->setCurrentText(promptInputList.at(promptInputNum)); }
    }
}

/* kate: bom off; indent-mode cstyle; indent-width 4; replace-trailing-space-save on; */
