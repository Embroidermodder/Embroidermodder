/*
 * Embroidermodder 2.
 *
 * Copyright 2011-2024 The Embroidermodder Team
 * Embroidermodder 2 is Open Source Software, see LICENSE.md for licensing terms.
 *
 * Read the reference manual (https://www.libembroidery.org/downloads/emrm.pdf)
 * for advice on altering this file.
 *
 * Undo
 */

#include "embroidermodder.h"

#define NUMBINS 10

/* . */
UndoEditor::UndoEditor(const QString& iconDirectory, QWidget* widgetToFocus, QWidget* parent) : QDockWidget(parent)
{
    iconDir = iconDirectory;
    iconSize = 16;
    setMinimumSize(100,100);

    undoGroup = new QUndoGroup(this);
    undoView = new QUndoView(undoGroup, this);
    updateCleanIcon(false);

    setWidget(undoView);
    setWindowTitle(tr("History"));
    setAllowedAreas(Qt::LeftDockWidgetArea | Qt::RightDockWidgetArea);

    this->setFocusProxy(widgetToFocus);
    undoView->setFocusProxy(widgetToFocus);
}

/* . */
UndoEditor::~UndoEditor()
{
}

/* . */
void
UndoEditor::updateCleanIcon(bool opened)
{
    if (opened) {
        undoView->setEmptyLabel(tr("Open"));
        undoView->setCleanIcon(QIcon(iconDir + "/" + "open" + ".png"));
    }
    else {
        undoView->setEmptyLabel(tr("New"));
        undoView->setCleanIcon(QIcon(iconDir + "/" + "new" + ".png"));
    }
}

/* . */
void
UndoEditor::addStack(QUndoStack* stack)
{
    undoGroup->addStack(stack);
}

/* . */
bool
UndoEditor::canUndo() const
{
    return undoGroup->canUndo();
}

/* . */
bool
UndoEditor::canRedo() const
{
    return undoGroup->canRedo();
}

/* . */
QString
UndoEditor::undoText() const
{
    return undoGroup->undoText();
}

/* . */
QString
UndoEditor::redoText() const
{
    return undoGroup->redoText();
}

/* . */
void
UndoEditor::undo()
{
    undoGroup->undo();
}

/* . */
void
UndoEditor::redo()
{
    undoGroup->redo();
}

/* . */
ImageWidget::ImageWidget(const QString &filename, QWidget* parent) : QWidget(parent)
{
    debug_message("ImageWidget Constructor");

    img.load(filename);

    setMinimumWidth(img.width());
    setMinimumHeight(img.height());
    setMaximumWidth(img.width());
    setMaximumHeight(img.height());

    this->show();
}

/* . */
bool
ImageWidget::load(const QString &fileName)
{
    img.load(fileName);
    return true;
}

/* . */
bool
ImageWidget::save(const QString &fileName)
{
    img.save(fileName, "PNG");
    return true;
}

/* . */
ImageWidget::~ImageWidget()
{
    debug_message("ImageWidget Destructor");
}

void
ImageWidget::paintEvent(QPaintEvent*)
{
    QPainter painter(this);
    painter.setViewport(0, 0, width(), height());
    painter.setWindow(0, 0, width(), height());
    painter.drawImage(0, 0, img);
}

void
contextMenuEvent(QObject* object, QContextMenuEvent *event)
{
    QApplication::setOverrideCursor(Qt::ArrowCursor);
    QMenu menu(statusbar);
    if (object->objectName() == "StatusBarButtonSNAP") {
        QAction* action = new QAction(create_icon("gridsnapsettings"), "&Settings...", &menu);
        QObject::connect(action, &QAction::trigger,
            _main, [](void) { _main->settingsDialog("Snap"); });
        menu.addAction(action);
    }
    else if (object->objectName() == "StatusBarButtonGRID") {
        QAction* action = new QAction(create_icon("gridsettings"), "&Settings...", &menu);
        QObject::connect(action, &QAction::trigger,
            _main, [](void) { _main->settingsDialog("Grid/Ruler"); });
        menu.addAction(action);
    }
    else if (object->objectName() == "StatusBarButtonRULER") {
        QAction* action = new QAction(create_icon("rulersettings"), "&Settings...", &menu);
        QObject::connect(action, &QAction::trigger,
            _main, [](void) { _main->settingsDialog("Grid/Ruler"); });
        menu.addAction(action);
    }
    else if (object->objectName() == "StatusBarButtonORTHO") {
        QAction* action = new QAction(create_icon("orthosettings"), "&Settings...", &menu);
        QObject::connect(action, &QAction::trigger,
            _main, [](void) { _main->settingsDialog("Ortho/Polar"); });
        menu.addAction(action);
    }
    else if (object->objectName() == "StatusBarButtonPOLAR") {
        QAction* action = new QAction(create_icon("polarsettings"), "&Settings...", &menu);
        QObject::connect(action, &QAction::trigger,
            _main, [](void) { _main->settingsDialog("Ortho/Polar"); });
        menu.addAction(action);
    }
    else if (object->objectName() == "StatusBarButtonQSNAP") {
        QAction* action = new QAction(create_icon("qsnapsettings"), "&Settings...", &menu);
        QObject::connect(action, &QAction::trigger,
            _main, [](void) { _main->settingsDialog("QuickSnap"); });
        menu.addAction(action);
    }
    else if (object->objectName() == "StatusBarButtonQTRACK") {
        QAction* action = new QAction(create_icon("qtracksettings"), "&Settings...", &menu);
        QObject::connect(action, &QAction::trigger,
            _main, [](void) { _main->settingsDialog("QuickTrack"); });
        menu.addAction(action);
    }
    else if (object->objectName() == "StatusBarButtonLWT") {
        Document* gview = activeDocument();
        if (gview) {
            QAction* enableRealAction = new QAction(create_icon("realrender"), "&RealRender On", &menu);
            enableRealAction->setEnabled(!gview->isRealEnabled());
            QObject::connect(enableRealAction, &QAction::triggered, _main, enableReal);
            menu.addAction(enableRealAction);

            QAction* disableRealAction = new QAction(create_icon("realrender"), "&RealRender Off", &menu);
            disableRealAction->setEnabled(gview->isRealEnabled());
            QObject::connect(disableRealAction, &QAction::triggered, _main, disableReal);
            menu.addAction(disableRealAction);
        }

        QAction* action = new QAction(create_icon("lineweightsettings"), "&Settings...", &menu);
        QObject::connect(action, &QAction::trigger,
            _main, [](void) { _main->settingsDialog("LineWeight"); });
        menu.addAction(action);
    }
    menu.exec(event->globalPos());
    restore_cursor();
    statusbar->clearMessage();
}

void
statusbar_toggle(QString key, bool on)
{
    debug_message("StatusBarButton toggleSnap()");
    Document* gview = activeDocument();
    if (gview) {
        if (key == "SNAP") {
            gview->toggleSnap(on);
        }
        else if (key == "GRID") {
            gview->toggleGrid(on);
        }
        else if (key == "RULER") {
            gview->toggleRuler(on);
        }
        else if (key == "ORTHO") {
            gview->toggleOrtho(on);
        }
        else if (key == "POLAR") {
            gview->togglePolar(on);
        }
        else if (key == "QSNAP") {
            gview->toggleQSnap(on);
        }
        else if (key == "LWT") {
            gview->toggleLwt(on);
        }
    }
}

void
enableLwt()
{
    debug_message("StatusBarButton enableLwt()");
    Document* gview = activeDocument();
    if (gview) {
        if (!gview->isLwtEnabled()) {
            gview->toggleLwt(true);
        }
    }
}

void
disableLwt()
{
    debug_message("StatusBarButton disableLwt()");
    Document* gview = activeDocument();
    if (gview) {
        if (gview->isLwtEnabled()) {
            gview->toggleLwt(false);
        }
    }
}

void
enableReal()
{
    debug_message("StatusBarButton enableReal()");
    Document* gview = activeDocument();
    if (gview) {
        gview->toggleReal(true);
    }
}

void
disableReal()
{
    debug_message("StatusBarButton disableReal()");
    Document* gview = activeDocument();
    if (gview) {
        gview->toggleReal(false);
    }
}

void
create_statusbar(MainWindow* mw)
{
    statusbar = new QStatusBar(mw);
    statusbar->setObjectName("StatusBar");

    statusBarSnapButton = new QToolButton(mw);
    statusBarGridButton = new QToolButton(mw);
    statusBarRulerButton = new QToolButton(mw);
    statusBarOrthoButton = new QToolButton(mw);
    statusBarPolarButton = new QToolButton(mw);
    statusBarQSnapButton = new QToolButton(mw);
    statusBarQTrackButton = new QToolButton(mw);
    statusBarLwtButton = new QToolButton(mw);

    QHash<QString, QToolButton*> toolbuttons;
    toolbuttons["SNAP"] = statusBarSnapButton;
    toolbuttons["GRID"] = statusBarGridButton;
    toolbuttons["RULER"] = statusBarRulerButton;
    toolbuttons["ORTHO"] = statusBarOrthoButton;
    toolbuttons["POLAR"] = statusBarPolarButton;
    toolbuttons["QSNAP"] = statusBarQSnapButton;
    toolbuttons["QTRACK"] = statusBarQTrackButton;
    toolbuttons["LWT"] = statusBarLwtButton;

    foreach (QString name, button_list) {
        toolbuttons[name]->setText(name);
        toolbuttons[name]->setObjectName("StatusBarButton" + name);
        toolbuttons[name]->setAutoRaise(true);
        toolbuttons[name]->setCheckable(true);
        QObject::connect(toolbuttons[name], &QAbstractButton::toggled,
            toolbuttons[name], [name] (bool b) { statusbar_toggle(name, b); });
    }


    statusBarMouseCoord = new QLabel(statusbar);

    statusBarMouseCoord->setMinimumWidth(300); /* Must fit this text always */
    statusBarMouseCoord->setMaximumWidth(300); /* "+1.2345E+99, +1.2345E+99, +1.2345E+99" */

    statusbar->addWidget(statusBarMouseCoord);
    statusbar->addWidget(statusBarSnapButton);
    statusbar->addWidget(statusBarGridButton);
    statusbar->addWidget(statusBarRulerButton);
    statusbar->addWidget(statusBarOrthoButton);
    statusbar->addWidget(statusBarPolarButton);
    statusbar->addWidget(statusBarQSnapButton);
    statusbar->addWidget(statusBarQTrackButton);
    statusbar->addWidget(statusBarLwtButton);
}

void
setMouseCoord(double x, double y)
{
    /* TODO: set format from settings (Architectural, Decimal, Engineering, Fractional, Scientific) */

    /* Decimal */
    statusBarMouseCoord->setText(QString().setNum(x, 'F', 4) + ", " + QString().setNum(y, 'F', 4)); /* TODO: use precision from unit settings */

    /* Scientific */
    /* statusBarMouseCoord->setText(QString().setNum(x, 'E', 4) + ", " + QString().setNum(y, 'E', 4)); */ /* TODO: use precision from unit settings */
}

/* . */
SelectBox::SelectBox(Shape s, QWidget* parent) : QRubberBand(s, parent)
{
    /* Default values */
    setColors(QColor(Qt::darkGreen), QColor(Qt::green), QColor(Qt::darkBlue), QColor(Qt::blue), 32);
}

/* . */
void
SelectBox::setDirection(int dir)
{
    if (!dir) {
        dirPen = leftPen;
        dirBrush = leftBrush;
    }
    else {
        dirPen = rightPen;
        dirBrush = rightBrush;
    }
    boxDir = dir;
}

/* . */
void
SelectBox::setColors(const QColor& colorL, const QColor& fillL, const QColor& colorR, const QColor& fillR, int newAlpha)
{
    debug_message("SelectBox setColors()");
    alpha = newAlpha;

    leftPenColor = colorL; /* TODO: allow customization */
    leftBrushColor = QColor(fillL.red(), fillL.green(), fillL.blue(), alpha);
    rightPenColor = colorR; /* TODO: allow customization */
    rightBrushColor = QColor(fillR.red(), fillR.green(), fillR.blue(), alpha);

    leftPen.setColor(leftPenColor);
    leftPen.setStyle(Qt::DashLine);
    leftBrush.setStyle(Qt::SolidPattern);
    leftBrush.setColor(leftBrushColor);

    rightPen.setColor(rightPenColor);
    rightPen.setStyle(Qt::SolidLine);
    rightBrush.setStyle(Qt::SolidPattern);
    rightBrush.setColor(rightBrushColor);

    if (!boxDir) { dirPen = leftPen;  dirBrush = leftBrush;  }
    else        { dirPen = rightPen; dirBrush = rightBrush; }

    forceRepaint();
}

/* . */
void
SelectBox::paintEvent(QPaintEvent*)
{
    QPainter painter(this);
    painter.setPen(dirPen);
    painter.fillRect(0,0,width()-1, height()-1, dirBrush);
    painter.drawRect(0,0,width()-1, height()-1);
}

/* . */
void
SelectBox::forceRepaint()
{
    /* HACK: Take that QRubberBand! */
    QSize hack = size();
    resize(hack + QSize(1,1));
    resize(hack);
}


LayerManager::LayerManager(MainWindow* mw, QWidget* parent) : QDialog(parent)
{
    layerModel = new QStandardItemModel(0, 8, this);

    layerModelSorted = new QSortFilterProxyModel;
    layerModelSorted->setDynamicSortFilter(true);
    layerModelSorted->setSourceModel(layerModel);

    treeView = new QTreeView;
    treeView->setRootIsDecorated(false);
    treeView->setAlternatingRowColors(true);
    treeView->setModel(layerModelSorted);
    treeView->setSortingEnabled(true);
    treeView->sortByColumn(0, Qt::AscendingOrder);

    QVBoxLayout *mainLayout = new QVBoxLayout;
    mainLayout->addWidget(treeView);
    setLayout(mainLayout);

    setWindowTitle(tr("Layer Manager"));
    setMinimumSize(750, 550);

    layerModel->setHeaderData(0, Qt::Horizontal, tr("Name"));
    layerModel->setHeaderData(1, Qt::Horizontal, tr("Visible"));
    layerModel->setHeaderData(2, Qt::Horizontal, tr("Frozen"));
    layerModel->setHeaderData(3, Qt::Horizontal, tr("Z Value"));
    layerModel->setHeaderData(4, Qt::Horizontal, tr("Color"));
    layerModel->setHeaderData(5, Qt::Horizontal, tr("Linetype"));
    layerModel->setHeaderData(6, Qt::Horizontal, tr("Lineweight"));
    layerModel->setHeaderData(7, Qt::Horizontal, tr("Print"));

    addLayer("0", true, false, 0.0, qRgb(0,0,0), "Continuous", "Default", true);
    addLayer("1", true, false, 1.0, qRgb(0,0,0), "Continuous", "Default", true);
    addLayer("2", true, false, 2.0, qRgb(0,0,0), "Continuous", "Default", true);
    addLayer("3", true, false, 3.0, qRgb(0,0,0), "Continuous", "Default", true);
    addLayer("4", true, false, 4.0, qRgb(0,0,0), "Continuous", "Default", true);
    addLayer("5", true, false, 5.0, qRgb(0,0,0), "Continuous", "Default", true);
    addLayer("6", true, false, 6.0, qRgb(0,0,0), "Continuous", "Default", true);
    addLayer("7", true, false, 7.0, qRgb(0,0,0), "Continuous", "Default", true);
    addLayer("8", true, false, 8.0, qRgb(0,0,0), "Continuous", "Default", true);
    addLayer("9", true, false, 9.0, qRgb(0,0,0), "Continuous", "Default", true);

    for (int i = 0; i < layerModel->columnCount(); ++i)
        treeView->resizeColumnToContents(i);

    QApplication::setOverrideCursor(Qt::ArrowCursor);
}

/* . */
LayerManager::~LayerManager()
{
    restore_cursor();
}

/* . */
void
LayerManager::addLayer(const QString& name,
                            const bool visible,
                            const bool frozen,
                            const double zValue,
                            const QRgb color,
                            const QString& lineType,
                            const QString& lineWeight,
                            const bool print)
{
    layerModel->insertRow(0);
    layerModel->setData(layerModel->index(0, 0), name);
    layerModel->setData(layerModel->index(0, 1), visible);
    layerModel->setData(layerModel->index(0, 2), frozen);
    layerModel->setData(layerModel->index(0, 3), zValue);

    QPixmap colorPix(QSize(16,16));
    colorPix.fill(QColor(color));
    layerModel->itemFromIndex(layerModel->index(0, 4))->setIcon(QIcon(colorPix));
    layerModel->setData(layerModel->index(0, 4), QColor(color));

    layerModel->setData(layerModel->index(0, 5), lineType);
    layerModel->setData(layerModel->index(0, 6), lineWeight);
    layerModel->setData(layerModel->index(0, 7), print);
}

/* TODO: Move majority of this code into libembroidery. */
void
create_details_dialog(void)
{
    QDialog *dialog = new QDialog();
    QRectF boundingRect;

    dialog->setMinimumSize(750, 550);

    debug_message("EmbDetailsDialog()");
    QApplication::setOverrideCursor(Qt::ArrowCursor);

    EmbPattern* pattern = activeMdiWindow()->pattern;
    if (!pattern) {
        printf("Pattern data is missing or failed to initialize.");
        return;
    }

    uint32_t stitchesTotal = pattern->stitch_list->count;
    uint32_t stitchesReal = 0;
    uint32_t stitchesJump = 0;
    uint32_t stitchesTrim = 0;
    uint32_t colorTotal = pattern->thread_list->count;
    uint32_t colorChanges = 0;
    double minx = 0.0, maxx = 0.0, miny = 0.0, maxy = 0.0;
    double min_stitchlength = 999.0;
    double max_stitchlength = 0.0;
    double total_stitchlength = 0.0;
    int number_of_minlength_stitches = 0;
    int number_of_maxlength_stitches = 0;

    EmbRect bounds = emb_pattern_calcBoundingBox(pattern);

    if (pattern->stitch_list->count == 0) {
        QMessageBox::warning(dialog,
            translate("No Design Loaded"),
            translate("<b>A design needs to be loaded or created before details can be determined.</b>"));
        return;
    }
    QVector<double> stitchLengths;

    EmbVector position;
    position.x = 0.0f;
    position.y = 0.0f;
    double totalColorLength = 0.0;
    for (int i = 0; i < pattern->stitch_list->count; i++) {
        EmbVector delta;
        EmbStitch st = pattern->stitch_list->stitch[i];
        delta.x = st.x - position.x;
        delta.y = st.y - position.y;
        position.x = st.x;
        position.y = st.y;
        double length = emb_vector_length(delta);
        totalColorLength += length;
        if (i > 0) {
            if (pattern->stitch_list->stitch[i-1].flags != NORMAL) {
                /* Can't count first normal stitch. */
                length = 0.0;
            }
        }
        if (!(st.flags & (JUMP | TRIM))) {
            stitchesReal++;
            if (length > max_stitchlength) {
                max_stitchlength = length;
                number_of_maxlength_stitches = 0;
            }
            if (length == max_stitchlength) {
                number_of_maxlength_stitches++;
            }
            if (length > 0 && length < min_stitchlength) {
                min_stitchlength = length;
                number_of_minlength_stitches = 0;
            }
            if (length == min_stitchlength) {
                number_of_minlength_stitches++;
            }
            total_stitchlength += length;
            if (position.x < minx) {
                minx = position.x;
            }
            if (position.x > maxx) {
                maxx = position.x;
            }
            if (position.y < miny) {
                miny = position.y;
            }
            if (position.y > maxy) {
                maxy = position.y;
            }
        }
        if (st.flags & JUMP) {
            stitchesJump++;
        }
        if (st.flags & TRIM) {
            stitchesTrim++;
        }
        if (st.flags & STOP) {
            stitchLengths.push_back(totalColorLength);
            totalColorLength = 0;
            colorChanges++;
        }
        if (st.flags & END) {
            stitchLengths.push_back(totalColorLength);
        }
    }

    /* Second pass to fill bins now that we know max stitch length. */
    int bin[NUMBINS+1];
    for (int i = 0; i <= NUMBINS; i++) {
        bin[i] = 0;
    }

    position.x = 0.0f;
    position.y = 0.0f;
    for (int i = 0; i < pattern->stitch_list->count; i++) {
        EmbVector delta;
        EmbStitch st = pattern->stitch_list->stitch[i];
        delta.x = st.x - position.x;
        delta.y = st.y - position.y;
        position.x = st.x;
        position.y = st.y;
        if (i > 0) {
            if (pattern->stitch_list->stitch[i-1].flags == NORMAL && st.flags == NORMAL) {
                double length = emb_vector_length(delta);
                int bin_number = int(floor(NUMBINS*length/max_stitchlength));
                bin[bin_number]++;
            }
        }
    }

    double binSize = max_stitchlength / NUMBINS;

    QString str;
    for (int i = 0; i < NUMBINS; i++) {
        str += QString::number(binSize * (i), 'f', 1);
        str += " - " + QString::number(binSize * (i+1), 'f', 1);
        str += " mm: " +  QString::number(bin[i]) + "\n\n";
    }

    /*
    grid->addWidget(new QLabel(translate("\nStitch Distribution: \n")), 9, 0, 1, 2);
    grid->addWidget(new QLabel(str), 10, 0, 1, 1);
    grid->addWidget(new QLabel(translate("\nThread Length By Color: \n")), 11, 0, 1, 2);
    */
    int currentRow = 12;

    for (int i = 0; i < pattern->thread_list->count; i++) {
        EmbColor t = pattern->thread_list->thread[i].color;
        /*
        QFrame *frame = new QFrame();
        frame->setGeometry(0, 0, 30, 30);
        QPalette palette = frame->palette();
        palette.setColor(backgroundRole(), QColor( t.r, t.g, t.b ) );
        frame->setPalette( palette );
        frame->setAutoFillBackground(true);
        grid->addWidget(frame, currentRow,0,1,1);
        char message[MAX_STRING_LENGTH];
        sprintf(message, "size: %d i: %d", stitchLengths.size(), i);
        debug_message(message);
        grid->addWidget(new QLabel(QString::number(stitchLengths.at(i)) + " mm"), currentRow,1,1,1);
        */
        currentRow++;
    }

    boundingRect.setRect(bounds.x, bounds.y, bounds.x + bounds.w, bounds.y + bounds.h);

    QWidget* widget = new QWidget(dialog);

    /* Misc */
    QGroupBox* groupBoxMisc = new QGroupBox(translate("General Information"), widget);

    QLabel* labelStitchesTotal = new QLabel(translate("Total Stitches:"), dialog);
    QLabel* labelStitchesReal = new QLabel(translate("Real Stitches:"), dialog);
    QLabel* labelStitchesJump = new QLabel(translate("Jump Stitches:"), dialog);
    QLabel* labelStitchesTrim = new QLabel(translate("Trim Stitches:"), dialog);
    QLabel* labelColorTotal = new QLabel(translate("Total Colors:"), dialog);
    QLabel* labelColorChanges = new QLabel(translate("Color Changes:"), dialog);
    QLabel* labelRectLeft = new QLabel(translate("Left:"), dialog);
    QLabel* labelRectTop = new QLabel(translate("Top:"), dialog);
    QLabel* labelRectRight = new QLabel(translate("Right:"), dialog);
    QLabel* labelRectBottom = new QLabel(translate("Bottom:"), dialog);
    QLabel* labelRectWidth = new QLabel(translate("Width:"), dialog);
    QLabel* labelRectHeight = new QLabel(translate("Height:"), dialog);

    QLabel* fieldStitchesTotal = new QLabel(QString::number(stitchesTotal), dialog);
    QLabel* fieldStitchesReal = new QLabel(QString::number(stitchesReal), dialog);
    QLabel* fieldStitchesJump = new QLabel(QString::number(stitchesJump), dialog);
    QLabel* fieldStitchesTrim = new QLabel(QString::number(stitchesTrim), dialog);
    QLabel* fieldColorTotal = new QLabel(QString::number(colorTotal), dialog);
    QLabel* fieldColorChanges = new QLabel(QString::number(colorChanges), dialog);
    QLabel* fieldRectLeft = new QLabel(QString::number(boundingRect.left()) + " mm", dialog);
    QLabel* fieldRectTop = new QLabel(QString::number(boundingRect.top()) + " mm", dialog);
    QLabel* fieldRectRight = new QLabel(QString::number(boundingRect.right()) + " mm", dialog);
    QLabel* fieldRectBottom = new QLabel(QString::number(boundingRect.bottom()) + " mm", dialog);
    QLabel* fieldRectWidth = new QLabel(QString::number(boundingRect.width()) + " mm", dialog);
    QLabel* fieldRectHeight = new QLabel(QString::number(boundingRect.height()) + " mm", dialog);

    QGridLayout* gridLayoutMisc = new QGridLayout(groupBoxMisc);
    gridLayoutMisc->addWidget(labelStitchesTotal, 0, 0, Qt::AlignLeft);
    gridLayoutMisc->addWidget(labelStitchesReal, 1, 0, Qt::AlignLeft);
    gridLayoutMisc->addWidget(labelStitchesJump, 2, 0, Qt::AlignLeft);
    gridLayoutMisc->addWidget(labelStitchesTrim, 3, 0, Qt::AlignLeft);
    gridLayoutMisc->addWidget(labelColorTotal, 4, 0, Qt::AlignLeft);
    gridLayoutMisc->addWidget(labelColorChanges, 5, 0, Qt::AlignLeft);
    gridLayoutMisc->addWidget(labelRectLeft, 6, 0, Qt::AlignLeft);
    gridLayoutMisc->addWidget(labelRectTop, 7, 0, Qt::AlignLeft);
    gridLayoutMisc->addWidget(labelRectRight, 8, 0, Qt::AlignLeft);
    gridLayoutMisc->addWidget(labelRectBottom, 9, 0, Qt::AlignLeft);
    gridLayoutMisc->addWidget(labelRectWidth, 10, 0, Qt::AlignLeft);
    gridLayoutMisc->addWidget(labelRectHeight, 11, 0, Qt::AlignLeft);
    gridLayoutMisc->addWidget(fieldStitchesTotal, 0, 1, Qt::AlignLeft);
    gridLayoutMisc->addWidget(fieldStitchesReal, 1, 1, Qt::AlignLeft);
    gridLayoutMisc->addWidget(fieldStitchesJump, 2, 1, Qt::AlignLeft);
    gridLayoutMisc->addWidget(fieldStitchesTrim, 3, 1, Qt::AlignLeft);
    gridLayoutMisc->addWidget(fieldColorTotal, 4, 1, Qt::AlignLeft);
    gridLayoutMisc->addWidget(fieldColorChanges, 5, 1, Qt::AlignLeft);
    gridLayoutMisc->addWidget(fieldRectLeft, 6, 1, Qt::AlignLeft);
    gridLayoutMisc->addWidget(fieldRectTop, 7, 1, Qt::AlignLeft);
    gridLayoutMisc->addWidget(fieldRectRight, 8, 1, Qt::AlignLeft);
    gridLayoutMisc->addWidget(fieldRectBottom, 9, 1, Qt::AlignLeft);
    gridLayoutMisc->addWidget(fieldRectWidth, 10, 1, Qt::AlignLeft);
    gridLayoutMisc->addWidget(fieldRectHeight, 11, 1, Qt::AlignLeft);
    gridLayoutMisc->setColumnStretch(1,1);
    groupBoxMisc->setLayout(gridLayoutMisc);

    /* TODO: Color Histogram. */

    /* Stitch Distribution */
    QGroupBox* groupBoxDist = new QGroupBox(translate("Stitch Distribution"), widget);

    /* TODO: Stitch Distribution Histogram */

    /* Widget Layout
    QScrollArea* scrollArea = new QScrollArea(dialog);
    scrollArea->setWidgetResizable(true);
    scrollArea->setWidget(widget); */

    QDialogButtonBox* buttonBox = new QDialogButtonBox(QDialogButtonBox::Ok);
    QObject::connect(buttonBox, SIGNAL(accepted()), dialog, SLOT(accept()));

    QVBoxLayout *vboxLayoutMain = new QVBoxLayout(widget);
    vboxLayoutMain->addWidget(groupBoxMisc);
    vboxLayoutMain->addWidget(groupBoxDist);
    vboxLayoutMain->addWidget(buttonBox);
    vboxLayoutMain->addStretch(1);
    widget->setLayout(vboxLayoutMain);

    dialog->setWindowTitle(translate("Embroidery Design Details"));

    dialog->exec();

    QApplication::setOverrideCursor(Qt::ArrowCursor);
}

PreviewDialog::PreviewDialog(QWidget* parent,
                             const QString& caption,
                             const QString& dir,
                             const QString& filter) : QFileDialog(parent, caption, dir, filter)
{
    debug_message("PreviewDialog Constructor");

    /* TODO: get actual thumbnail image from file, lets also use a size of 128x128 for now...
     * TODO: make thumbnail size adjustable thru settings dialog
     */
    imgWidget = new ImageWidget("icons/default/nopreview.png", this);

    QLayout* lay = layout();
    if (qobject_cast<QGridLayout*>(lay))
    {
        QGridLayout* grid = qobject_cast<QGridLayout*>(lay);
        grid->addWidget(imgWidget, 0, grid->columnCount(), grid->rowCount(), 1);
    }

    setModal(true);
    setOption(QFileDialog::DontUseNativeDialog);
    setViewMode(QFileDialog::Detail);
    setFileMode(QFileDialog::ExistingFiles);

    /* TODO: connect the currentChanged signal to update the preview imgWidget. */
}

PreviewDialog::~PreviewDialog()
{
    debug_message("PreviewDialog Destructor");
}

MdiArea::MdiArea(MainWindow* mw, QWidget *parent) : QMdiArea(parent)
{
    setTabsClosable(true);

    useLogo = false;
    useTexture = false;
    useColor = false;
}

MdiArea::~MdiArea()
{
}

void
MdiArea::useBackgroundLogo(bool use)
{
    useLogo = use;
    forceRepaint();
}

void
MdiArea::useBackgroundTexture(bool use)
{
    useTexture = use;
    forceRepaint();
}

void
MdiArea::useBackgroundColor(bool use)
{
    useColor = use;
    forceRepaint();
}

void
MdiArea::setBackgroundLogo(const QString& fileName)
{
    bgLogo.load(fileName);

    forceRepaint();
}

void
MdiArea::setBackgroundTexture(const QString& fileName)
{
    bgTexture.load(fileName);

    forceRepaint();
}

void
MdiArea::setBackgroundColor(const QColor& color)
{
    if (!color.isValid()) {
        bgColor = background().color();
    }
    else {
        bgColor = color;
    }

    forceRepaint();
}

void MdiArea::mouseDoubleClickEvent(QMouseEvent* /*e*/)
{
    _main->openFile();
}

void
MdiArea::paintEvent(QPaintEvent* /*e*/)
{
    QWidget* vport = viewport();
    QRect rect = vport->rect();

    QPainter painter(vport);
    painter.setRenderHint(QPainter::SmoothPixmapTransform);

    /* Always fill with a solid color first */
    if (useColor) {
        painter.fillRect(rect, bgColor);
    }
    else {
        painter.fillRect(rect, background());
    }

    /* Then overlay the texture */
    if (useTexture) {
        QBrush bgBrush(bgTexture);
        painter.fillRect(rect, bgBrush);
    }

    /* Overlay the logo last */
    if (useLogo) {
        /* Center the pixmap */
        int dx = (rect.width()-bgLogo.width())/2;
        int dy = (rect.height()-bgLogo.height())/2;
        painter.drawPixmap(dx, dy, bgLogo.width(), bgLogo.height(), bgLogo);
    }
}

void
MdiArea::cascade()
{
    cascadeSubWindows();
    zoomExtentsAllSubWindows();
}

void
MdiArea::tile()
{
    tileSubWindows();
    zoomExtentsAllSubWindows();
}

/* . */
void
MdiArea::zoomExtentsAllSubWindows()
{
    foreach(QMdiSubWindow* window, subWindowList()) {
        MdiWindow* mdiWin = qobject_cast<MdiWindow*>(window);
        if (mdiWin) {
            Document* v = mdiWin->gview;
            if (v) {
                v->recalculateLimits();
                v->zoomExtents();
            }
        }
    }
}

/* HACK: Take that QMdiArea! */
void
MdiArea::forceRepaint()
{
    QSize hack = size();
    resize(hack + QSize(1,1));
    resize(hack);
}

/* . */
MdiWindow::MdiWindow(const int theIndex, MainWindow* mw, QMdiArea* parent, Qt::WindowFlags wflags) : QMdiSubWindow(parent, wflags)
{
    mdiArea = parent;

    myIndex = theIndex;

    fileWasLoaded = false;

    setAttribute(Qt::WA_DeleteOnClose);

    QString aName;
    curFile = aName.asprintf("Untitled%d.dst", myIndex);
    this->setWindowTitle(curFile);

    this->setWindowIcon(create_icon("app"));

    gscene = new QGraphicsScene(0,0,0,0, this);
    gview = new Document(_main, gscene, this);

    setWidget(gview);

    /* WARNING: DO NOT SET THE QMDISUBWINDOW (this) FOCUSPROXY TO THE PROMPT
     * WARNING: AS IT WILL CAUSE THE WINDOW MENU TO NOT SWITCH WINDOWS PROPERLY!
     * WARNING: ALTHOUGH IT SEEMS THAT SETTING INTERNAL WIDGETS FOCUSPROXY IS OK.
     */
    gview->setFocusProxy(prompt);

    resize(sizeHint());

    promptHistory = "Welcome to Embroidermodder 2!<br/>Open some of our sample files. Many formats are supported.<br/>For help, press F1.";
    setHistory(promptHistory);
    promptInputList << "";
    promptInputNum = 0;

    curLayer = "0";
    curColor = 0; /* TODO: color ByLayer */
    curLineType = "ByLayer";
    curLineWeight = "ByLayer";

    /* Due to strange Qt4.2.3 feature the child window icon is not drawn
     * in the main menu if showMaximized() is called for a non-visible child window
     * Therefore calling show() first...
     */
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
    debug_message("MdiWindow Destructor()");
    emb_pattern_free(pattern);
}

bool
MdiWindow::saveFile(const QString &fileName)
{
    return pattern_save(pattern, qPrintable(fileName));
}

bool
MdiWindow::loadFile(const QString &fileName)
{
    debug_message("MdiWindow loadFile()");

    QRgb tmpColor = curColor;

    QFile file(fileName);
    if (!file.open(QFile::ReadOnly | QFile::Text)) {
        QMessageBox::warning(this, tr("Error reading file"),
            tr("Cannot read file %1:\n%2.").arg(fileName).arg(file.errorString()));
        return false;
    }

    wait_cursor();

    QString ext = fileExtension(fileName);
    debug_message("ext: ");
    debug_message(qPrintable(ext));

    /* Read */
    int format = EMB_FORMAT_CSV; /* emb_identify_format(qPrintable(fileName)); */
    if (format <= 0) {
        debug_message("Unsupported read file type: ");
        debug_message(qPrintable(fileName));
        restore_cursor();
        QMessageBox::warning(this, tr("Error reading pattern"),
            tr("Unsupported read file type: ") + qPrintable(fileName));
        return false;
    }

    int readSuccessful = emb_pattern_read(pattern, qPrintable(fileName), format);
    if (!readSuccessful) {
        debug_message("Reading file was unsuccessful:");
        debug_message(qPrintable(fileName));
        restore_cursor();
        QMessageBox::warning(this, tr("Error reading pattern"),
            tr("Reading file was unsuccessful: ") + qPrintable(fileName));
        return false;
    }

    debug_message("Read successful.\n");
    /* emb_pattern_moveStitchListToPolylines(pattern); */ /* TODO: Test more */
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
    statusbar->showMessage("File loaded.");

    if (grid_load_from_file.setting) {
        /* TODO: Josh, provide me a hoop size and/or grid spacing from the pattern. */
    }

    restore_cursor();

    /* Clear the undo stack so it is not possible to undo past this point. */
    gview->data.undoStack->clear();

    curColor = tmpColor;

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
        if (printing_disable_bg.setting) {
            /* Save current bg */
            QBrush brush = gview->backgroundBrush();
            /* Save ink by not printing the bg at all */
            gview->setBackgroundBrush(Qt::NoBrush);
            /* Print, fitting the viewport contents into a full page */
            gview->render(&painter);
            /* Restore the bg */
            gview->setBackgroundBrush(brush);
        }
        else {
            /* Print, fitting the viewport contents into a full page */
            gview->render(&painter);
        }
    }
}

/* TODO: Save a Brother PEL image (An 8bpp, 130x113 pixel monochromatic? bitmap image) Why 8bpp when only 1bpp is needed? */

/* TODO: Should BMC be limited to ~32KB or is this a mix up with Bitmap Cache? */
/* TODO: Is there/should there be other embedded data in the bitmap besides the image itself? */
/* NOTE: Can save a Singer BMC image (An 8bpp, 130x113 pixel colored bitmap image) */
void
MdiWindow::saveBMC()
{
    /* TODO: figure out how to center the image, right now it just plops it to the left side. */
    QImage img(150, 150, QImage::Format_ARGB32_Premultiplied);
    img.fill(qRgb(255,255,255));
    QRectF extents = gscene->itemsBoundingRect();

    QPainter painter(&img);
    QRectF targetRect(0,0,150,150);
    if (printing_disable_bg.setting) {
        /* TODO: Make BMC background into it's own setting? */
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
    debug_message("MdiWindow closeEvent()");
    emit sendCloseMdiWin(this);
}

void
MdiWindow::onWindowActivated()
{
    debug_message("MdiWindow onWindowActivated()");
    gview->data.undoStack->setActive(true);
    _main->setUndoCleanIcon(fileWasLoaded);
    statusBarSnapButton->setChecked(gscene->property("ENABLE_SNAP").toBool());
    statusBarGridButton->setChecked(gscene->property("ENABLE_GRID").toBool());
    statusBarRulerButton->setChecked(gscene->property("ENABLE_RULER").toBool());
    statusBarOrthoButton->setChecked(gscene->property("ENABLE_ORTHO").toBool());
    statusBarPolarButton->setChecked(gscene->property("ENABLE_POLAR").toBool());
    statusBarQSnapButton->setChecked(gscene->property("ENABLE_QSNAP").toBool());
    statusBarQTrackButton->setChecked(gscene->property("ENABLE_QTRACK").toBool());
    statusBarLwtButton->setChecked(gscene->property("ENABLE_LWT").toBool());
    setHistory(promptHistory);
}

QSize
MdiWindow::sizeHint() const
{
    debug_message("MdiWindow sizeHint()");
    return QSize(450, 300);
}

void
MdiWindow::currentLayerChanged(const QString& layer)
{
    curLayer = layer;
}

void
MdiWindow::currentColorChanged(const QRgb& color)
{
    curColor = color;
}

void
MdiWindow::currentLinetypeChanged(const QString& type)
{
    curLineType = type;
}

void
MdiWindow::currentLineweightChanged(const QString& weight)
{
    curLineWeight = weight;
}

void
MdiWindow::updateColorLinetypeLineweight()
{
}

void
MdiWindow::deletePressed()
{
    gview->deletePressed();
}

void
MdiWindow::escapePressed()
{
    gview->escapePressed();
}

void
MdiWindow::showViewScrollBars(bool val)
{
    gview->showScrollBars(val);
}

void
MdiWindow::setViewCrossHairColor(QRgb color)
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
            messagebox("critical",
                translate("Prompt Previous Error"),
                translate("The prompt input is empty! Please report this as a bug!"));
        }
        else {
            messagebox("critical",
                translate("Prompt Next Error"),
                translate("The prompt input is empty! Please report this as a bug!"));
        }
        debug_message("The prompt input is empty! Please report this as a bug!");
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
            prompt->setCurrentText("");
        }
        else if (promptInputNum >= maxNum) {
            promptInputNum = maxNum;
            prompt->setCurrentText("");
        }
        else {
            prompt->setCurrentText(promptInputList.at(promptInputNum));
        }
    }
}

void
setHistory(const QString& txt)
{
    promptHistory->setHtml(txt);
    promptHistory->moveCursor(QTextCursor::End, QTextCursor::MoveAnchor);
}

/* . */
CmdPrompt::CmdPrompt(QWidget* parent) : QWidget(parent)
{
    QFrame* promptDivider;
    QVBoxLayout* promptVBoxLayout;
    debug_message("CmdPrompt Constructor");
    setObjectName("Command Prompt");

    promptInput = new CmdPromptInput(this);
    promptHistory = new QTextBrowser();
    promptDivider = new QFrame(this);
    promptVBoxLayout = new QVBoxLayout(this);

    debug_message("CmdPromptHistory Constructor");
    setObjectName("Command Prompt History");

    int initHeight = 19*3; /* (approximately three lines of text) */

    promptHistory->setFrameStyle(QFrame::NoFrame);
    promptHistory->setMaximumHeight(initHeight);
    promptHistory->setMinimumWidth(200);
    /* TODO: use float/dock events to set minimum size
     * so when floating, it isn't smooshed. */

    promptHistory->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOn);

    this->setFocusProxy(promptInput);
    promptHistory->setFocusProxy(promptInput);

    promptDivider->setLineWidth(1);
    promptDivider->setFrameStyle(QFrame::HLine);
    promptDivider->setMaximumSize(QWIDGETSIZE_MAX, 1);

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
CmdPrompt::saveHistory(const QString& fileName, bool html)
{
    debug_message("CmdPrompt saveHistory");
    FILE *file = fopen(qPrintable(fileName), "w");
    if (html) {
        fprintf(file, "%s", qPrintable(promptHistory->toHtml()));
    }
    else {
        fprintf(file, "%s", qPrintable(promptHistory->toPlainText()));
    }
    fclose(file);
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
void
appendHistory(QString txt)
{
    if (txt.isNull()) {
        txt = curText;
    }

    promptHistory->append(txt);
    /* emit historyAppended(formatStr); */
    promptHistory->moveCursor(QTextCursor::End, QTextCursor::MoveAnchor);
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
            runCommandPrompt(curCmd);
        }
        else if (cmdtxt.isEmpty()) {
            cmdActive = true;
            appendHistory(curText);
            /* Rerun the last successful command. */
            runCommandPrompt(lastCmd);
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
    debug_message("CmdPromptInput::checkSelection");
    if (this->hasSelectedText()) {
        this->deselect();
    }
}

/* . */
void
CmdPromptInput::checkCursorPosition(int oldpos, int newpos)
{
    char message[MAX_STRING_LENGTH];
    sprintf(message, "CmdPromptInput::checkCursorPosition - %d %d", oldpos, newpos);
    debug_message(message);
    if (this->hasSelectedText()) {
        this->deselect();
    }
    if (newpos < prefix.length()) {
        this->setCursorPosition(prefix.length());
    }
}

/* . */
void
CmdPromptInput::updateCurrentText(const QString& txt)
{
    int cursorPos = cursorPosition();
    if (!txt.startsWith(prefix)) {
        if (txt.length() < prefix.length()) {
            this->setText(prefix);
        }
        else if (txt.length() != prefix.length()) {
            this->setText(prefix + txt);
        }
        else {
            this->setText(curText);
        }
    }
    else {
        /* input is okay so update curText */
        curText = txt;
        this->setText(curText);
    }
    setCursorPosition(cursorPos);
}

/* . */
void
CmdPromptInput::checkEditedText(const QString& txt)
{
    updateCurrentText(txt);

    if (rapidFireEnabled) {
        processInput();
    }
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
        int key = pressedKey->key();
        auto found = key_map.find(key);
        if (found != key_map.end()) {
            if (key != (int)Qt::Key_Shift) {
                pressedKey->accept();
            }
        }
        switch (key) {
        case Qt::Key_Enter:
        case Qt::Key_Return: {
            processInput(QChar('\n'));
            return true;
        }
        case Qt::Key_Space: {
            processInput(QChar(' '));
            return true;
        }
        case Qt::Key_Delete: {
            del();
            return true;
        }
        case Qt::Key_Escape: {
            prefix = defaultPrefix;
            clear();
            appendHistory(curText + tr("*Cancel*"));
            return true;
        }
        default: {
            pressedKey->ignore();
            break;
        }
        }
        if (found != key_map.end()) {
            if (key != (int)Qt::Key_Shift) {
                key_state[key] = true;
                return true;
            }
            else {
                /* we don't want to eat it, we just want to keep track of it */
                pressedKey->ignore();
            }
        }
    }

    if (event->type() == QEvent::KeyRelease) {
        QKeyEvent* releasedKey = (QKeyEvent*)event;
        int key = releasedKey->key();
        /* We don't want to eat it, we just want to keep track of it. */
        releasedKey->ignore();
        auto found = key_map.find(key);
        if (found != key_map.end()) {
            key_state[key] = false;
        }
    }
    return QObject::eventFilter(obj, event);
}

