#include "embroidermodder.h"

void add_polygon(qreal startX, qreal startY, const QPainterPath& p, int rubberMode);
void add_polyline(qreal startX, qreal startY, const QPainterPath& p, int rubberMode);

MdiWindow::MdiWindow(const int theIndex, MainWindow* mw, QMdiArea* parent, Qt::WindowFlags wflags) : QMdiSubWindow(parent, wflags)
{
    mainWin = mw;
    mdiArea = parent;

    myIndex = theIndex;

    fileWasLoaded = false;

    setAttribute(Qt::WA_DeleteOnClose);

    QString aName;
    curFile = aName.asprintf("Untitled%d.dst", myIndex);
    this->setWindowTitle(curFile);

    this->setWindowIcon(QIcon("icons/" + settings_general_icon_theme + "/" + "app" + ".png"));

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

MdiWindow::~MdiWindow()
{
    qDebug("MdiWindow Destructor()");
}

bool MdiWindow::saveFile(const QString &fileName)
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
                             tr("Cannot read file %1:\n%2.")
                             .arg(fileName)
                             .arg(file.errorString()));
        return false;
    }

    QApplication::setOverrideCursor(Qt::WaitCursor);

    QString ext = fileExtension(fileName);
    qDebug("ext: %s", qPrintable(ext));

    //Read
    EmbPattern* p = emb_pattern_create();
    if (!p) {
        printf("Could not allocate memory for embroidery pattern\n");
        return false;
    }
    int format = emb_identify_format(qPrintable(fileName));
    if (format == -1) {
        QApplication::restoreOverrideCursor();
        QMessageBox::warning(this, tr("Error reading pattern"), tr("Invalid format: embroidermodder doesn't support this format."));
        return false;
    }
    if (!emb_pattern_read(p, qPrintable(fileName), format)) {
        QApplication::restoreOverrideCursor();
        QMessageBox::warning(this, tr("Error reading pattern"), tr("Cannot read pattern"));
        return false;
    }

    //emb_move_stitches_to_polylines(p); //TODO: Test more
    int stitchCount = p->stitch_list->count;
    QPainterPath path;

    /* Load stitches. */
    QPainterPath polylinePath;
    bool firstPoint = true;
    qreal startX = 0, startY = 0;
    for (int i=0; i<p->stitch_list->count; i++) {
        EmbStitch st = p->stitch_list->stitch[i];
        // NOTE: Qt Y+ is down and libembroidery Y+ is up,
        // so inverting the Y is needed.
        qreal x = st.x;
        qreal y = -st.y;

        if (firstPoint) {
            polylinePath.moveTo(x, y);
            firstPoint = false;
            startX = x;
            startY = y;
        }
        else {
            polylinePath.lineTo(x, y);
        }
    }
    polylinePath.translate(-startX, -startY);
    add_polyline(startX, startY, polylinePath, OBJ_RUBBER_OFF);

    if (p->geometry) {
        for (int i=0; i<p->geometry->count; i++) {
            EmbGeometry *g = p->geometry[i].geometry;
            // FIXME
            //EmbColor thisColor = g->color;
            //setCurrentColor(qRgb(thisColor.r, thisColor.g, thisColor.b));
            setCurrentColor(qRgb(0, 0, 0));
            // NOTE: With natives, the Y+ is up and libembroidery Y+ is up,
            // so inverting the Y is NOT needed.
            switch (g->type) {
            case EMB_CIRCLE: {
                EmbCircle c = g->object.circle;
                add_circle(c.center.x, c.center.y, c.radius, false, OBJ_RUBBER_OFF);
                // TODO: fill
                break;
            }
            case EMB_ELLIPSE: {
                EmbEllipse e = g->object.ellipse;
                // add_ellipse(e.center.x, e.center.y, embEllipse_width(e), embEllipse_height(e), 0, false, OBJ_RUBBER_OFF); //TODO: rotation and fill
                break;
            }
            case EMB_LINE: {
                EmbLine li = g->object.line;
//              add_line(embLine_x1(li), embLine_y1(li), embLine_x2(li), embLine_y2(li), 0, OBJ_RUBBER_OFF);
                // TODO: rotation
                break;
            }
            case EMB_PATH: {
                //TODO: This is unfinished. It needs more work
                EmbPath path = g->object.path;
                /*
                EmbPathObjectList* curPathObjList = p->pathObjList;
                while(curPathObjList)
                {
                    QPainterPath pathPath;
                    EmbPointList* curPointList = curPathObjList->pathObj->pointList;
                    if (curPointList) {
                        EmbPoint pp = curPointList->point;
                        pathPath.moveTo(pp.xx, -pp.yy);
                        curPointList = curPointList->next;
                    }
                    while(curPointList)
                    {
                        EmbPoint pp = curPointList->point;
                        pathPath.lineTo(pp.xx, -pp.yy);
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
                */
                break;
            }
            case EMB_POINT: {
                break;
            }
        /*
            if(p->pointObjList)
            {
                EmbPointObjectList* curPointObj = p->pointObjList;
                while(curPointObj)
                {
                    EmbPoint po = curPointObj->pointObj.point;
                    EmbColor thisColor = curPointObj->pointObj.color;
                    setCurrentColor(qRgb(thisColor.r, thisColor.g, thisColor.b));
                    //NOTE: With natives, the Y+ is up and libembroidery Y+ is up, so inverting the Y is NOT needed.
                    add_point(embPoint_x(po), embPoint_y(po));
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
                    add_polygon(startX, startY, polygonPath, OBJ_RUBBER_OFF);

                    curPolygonObjList = curPolygonObjList->next;
                }
            }
            */
            case EMB_POLYLINE: {
                /* NOTE: Polylines should only contain NORMAL stitches.
                EmbPolyline polyline = g->object.polyline;
                QPainterPath polylinePath;
                bool firstPoint = true;
                qreal startX = 0, startY = 0;
                for (int i=0; i<polyline.pointList->count; i++) {
                    EmbVector v = polyline.pointList->point[i];
                    // NOTE: Qt Y+ is down and libembroidery Y+ is up,
                    // so inverting the Y is needed.
                    qreal x = v.x;
                    qreal y = -v.y;

                    if (firstPoint) {
                        polylinePath.moveTo(x, y);
                        firstPoint = false;
                        startX = x;
                        startY = y;
                    }
                    else {
                        polylinePath.lineTo(x, y);
                    }
                }
                polylinePath.translate(-startX, -startY);
                add_polyline(startX, startY, polylinePath, OBJ_RUBBER_OFF);
                 */
                break;
            }
            /*
            if(p->rectObjList)
            {
                EmbRectObjectList* curRectObj = p->rectObjList;
                while(curRectObj)
                {
                    EmbRect r = curRectObj->rectObj.rect;
                    add_rectangle(r.x, r.y, r.w, r.h, 0, false, OBJ_RUBBER_OFF); //TODO: rotation and fill
                    curRectObj = curRectObj->next;
                }
            }
            */
            default:
                break;
            }
        }
    }

    setCurrentFile(fileName);
    mainWin->statusbar->showMessage("File loaded.");
    QString stitches;
    stitches.setNum(stitchCount);

    if (settings_grid_load_from_file) {
        //TODO: Josh, provide me a hoop size and/or grid spacing from the pattern.
    }

    QApplication::restoreOverrideCursor();
    emb_pattern_free(p);

    //Clear the undo stack so it is not possible to undo past this point.
    gview->getUndoStack()->clear();

    setCurrentColor(tmpColor);

    fileWasLoaded = true;
    mainWin->setUndoCleanIcon(fileWasLoaded);
    return fileWasLoaded;
}

void MdiWindow::print()
{
    QPrintDialog dialog(&printer, this);
    if(dialog.exec() == QDialog::Accepted)
    {
        QPainter painter(&printer);
        if (settings_printing_disable_bg) {
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
void MdiWindow::saveBMC()
{
    //TODO: figure out how to center the image, right now it just plops it to the left side.
    QImage img(150, 150, QImage::Format_ARGB32_Premultiplied);
    img.fill(qRgb(255,255,255));
    QRectF extents = gscene->itemsBoundingRect();

    QPainter painter(&img);
    QRectF targetRect(0,0,150,150);
    if (settings_printing_disable_bg) //TODO: Make BMC background into it's own setting?
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

void MdiWindow::setCurrentFile(const QString &fileName)
{
    curFile = QFileInfo(fileName).canonicalFilePath();
    setWindowModified(false);
    setWindowTitle(getShortCurrentFile());
}

QString MdiWindow::getShortCurrentFile()
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
    mainWin->setUndoCleanIcon(fileWasLoaded);
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

