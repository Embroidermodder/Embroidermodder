#include "mdiwindow.h"
#include "view.h"
#include "statusbar.h"
#include "statusbar-button.h"
#include "object-save.h"
#include "object-data.h"

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

    connect(this, SIGNAL(message(const QString&, int)), mainWin->statusBar(), SLOT(message(const QString&, int)));
    resize(sizeHint());

    curLayer = "0";
    curColor = 0; //TODO: color ByLayer
    curLineType = "ByLayer";
    curLineWeight = "ByLayer";

    polylinePathItem = 0;

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
    /* embPattern_addPointObjectAbs(); */
    if(readSuccessful)
    {
        int stitchCount = embStitch_count(p->stitchList);
        QPainterPath path;
        int arcIndex = 0;

        if(p->circleObjList)
        {
            EmbCircleObjectList* curCircleObj = p->circleObjList;
            while(curCircleObj)
            {
                EmbCircle c = curCircleObj->circleObj.circle;
                mainWin->nativeAddCircle(embCircle_centerX(c), -embCircle_centerY(c), embCircle_radius(c), false, OBJ_RUBBER_OFF); //TODO: fill
                curCircleObj = curCircleObj->next;
            }
        }
        if(p->ellipseObjList)
        {
            EmbEllipseObjectList* curEllipseObj = p->ellipseObjList;
            while(curEllipseObj)
            {
                EmbEllipse e = curEllipseObj->ellipseObj.ellipse;
                mainWin->nativeAddEllipse(embEllipse_centerX(e), -embEllipse_centerY(e), embEllipse_width(e), embEllipse_height(e), 0, false, OBJ_RUBBER_OFF); //TODO: rotation and fill
                curEllipseObj = curEllipseObj->next;
            }
        }
        if(p->lineObjList)
        {
            EmbLineObjectList* curLineObj = p->lineObjList;
            while(curLineObj)
            {
                EmbLine li = curLineObj->lineObj.line;
                mainWin->nativeAddLine(embLine_x1(li), -embLine_y1(li), embLine_x2(li), -embLine_y2(li), 0, OBJ_RUBBER_OFF); //TODO: rotation
                curLineObj = curLineObj->next;
            }
        }
        if(p->pointObjList)
        {
            EmbPointObjectList* curPointObj = p->pointObjList;
            while(curPointObj)
            {
                EmbPoint po = curPointObj->pointObj.point;
                mainWin->nativeAddPoint(embPoint_x(po), -embPoint_y(po));
                curPointObj = curPointObj->next;
            }
        }
        if(p->rectObjList)
        {
            EmbRectObjectList* curRectObj = p->rectObjList;
            while(curRectObj)
            {
                EmbRect r = curRectObj->rectObj.rect;
                mainWin->nativeAddRectangle(embRect_x(r), -embRect_y(r), embRect_width(r), -embRect_height(r), 0, false, OBJ_RUBBER_OFF); //TODO: rotation and fill
                curRectObj = curRectObj->next;
            }
        }
        if(p->stitchList)
        {
            int previousColor = p->stitchList->stitch.color;

            EmbStitchList* curStitchItem = p->stitchList;
            while(curStitchItem)
            {
                EmbStitch tempStitch = curStitchItem->stitch;
                curStitchItem = curStitchItem->next;
                if((tempStitch.flags & STOP) || (tempStitch.flags & END))
                {
                    if(!path.isEmpty())
                    {
                        EmbColor thisColor = embThread_getAt(p->threadList, previousColor).color;
                        QPen loadPen(qRgb(thisColor.r, thisColor.g, thisColor.b));
                        loadPen.setWidthF(0.35);
                        loadPen.setCapStyle(Qt::RoundCap);
                        loadPen.setJoinStyle(Qt::RoundJoin);
                        QGraphicsPathItem* pathItem = gscene->addPath(path, loadPen);
                        previousColor = tempStitch.color;
                        pathItem->setData(OBJ_TYPE, OBJ_TYPE_POLYLINE);
                        pathItem->setFlag(QGraphicsItem::ItemIsMovable, true);
                        pathItem->setFlag(QGraphicsItem::ItemIsSelectable, true);
                        path = QPainterPath();
                    }
                    path.moveTo(tempStitch.xx, -tempStitch.yy);
                }
               // else if(tempStitch.flags==ARC)
                //{
                //    double sx        =  pattern.arclist[arcIndex].arcStartX;
                //    double sy        = -pattern.arclist[arcIndex].arcStartY;
                //    double cx        =  pattern.arclist[arcIndex].arcCenterX;
                //    double cy        = -pattern.arclist[arcIndex].arcCenterY;
                //    double rad       =  pattern.arclist[arcIndex].radius;
                 //   double incAngle  =  pattern.arclist[arcIndex].incAngle;

                    //TODO: death to rounding errors, if an arc is very close to being straight, then the arc is drawn backwards

                    //TODO: the semi-circle situation

                //    QRectF rect(QPointF(cx-rad, cy-rad), QPointF(cx+rad, cy+rad));
                //    double startAngle = QLineF(cx, cy, sx, sy).angle();
                //    path.arcTo(rect, startAngle, incAngle);
                //    arcIndex++;
                //}
                else if((tempStitch.flags & JUMP) || (tempStitch.flags & TRIM))
                {
                    //TODO: No objects should have moveTo in their path, so stop it here and add it to a BlockObject
                    path.moveTo(tempStitch.xx, -tempStitch.yy);
                }
                else
                {
                    path.lineTo(tempStitch.xx, -tempStitch.yy);
                }
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

void MDIWindow::closeEvent(QCloseEvent* e)
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

void MDIWindow::setViewGridColor(QRgb color)
{
    gview->setGridColor(color);
}

void MDIWindow::setViewRulerColor(QRgb color)
{
    gview->setRulerColor(color);
}

/* kate: bom off; indent-mode cstyle; indent-width 4; replace-trailing-space-save on; */
