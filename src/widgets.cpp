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

UndoEditor::~UndoEditor()
{
}

void UndoEditor::updateCleanIcon(bool opened)
{
    if (opened)
    {
        undoView->setEmptyLabel(tr("Open"));
        undoView->setCleanIcon(QIcon(iconDir + "/" + "open" + ".png"));
    }
    else
    {
        undoView->setEmptyLabel(tr("New"));
        undoView->setCleanIcon(QIcon(iconDir + "/" + "new" + ".png"));
    }
}

void UndoEditor::addStack(QUndoStack* stack)
{
    undoGroup->addStack(stack);
}

bool UndoEditor::canUndo() const
{
    return undoGroup->canUndo();
}

bool UndoEditor::canRedo() const
{
    return undoGroup->canRedo();
}

QString UndoEditor::undoText() const
{
    return undoGroup->undoText();
}

QString UndoEditor::redoText() const
{
    return undoGroup->redoText();
}

void UndoEditor::undo()
{
    undoGroup->undo();
}

void UndoEditor::redo()
{
    undoGroup->redo();
}

ImageWidget::ImageWidget(const QString &filename, QWidget* parent) : QWidget(parent)
{
    qDebug("ImageWidget Constructor");

    img.load(filename);

    setMinimumWidth(img.width());
    setMinimumHeight(img.height());
    setMaximumWidth(img.width());
    setMaximumHeight(img.height());

    this->show();
}

bool ImageWidget::load(const QString &fileName)
{
    img.load(fileName);
    return true;
}

bool ImageWidget::save(const QString &fileName)
{
    img.save(fileName, "PNG");
    return true;
}

ImageWidget::~ImageWidget()
{
    qDebug("ImageWidget Destructor");
}

void ImageWidget::paintEvent(QPaintEvent*)
{
    QPainter painter(this);
    painter.setViewport(0, 0, width(), height());
    painter.setWindow(0, 0, width(), height());
    painter.drawImage(0, 0, img);
}

StatusBarButton::StatusBarButton(QString buttonText, MainWindow* mw, StatusBar* statbar, QWidget *parent) : QToolButton(parent)
{
    statusbar = statbar;

    this->setObjectName("StatusBarButton" + buttonText);

    this->setText(buttonText);
    this->setAutoRaise(true);
    this->setCheckable(true);

    if (objectName() == "StatusBarButtonSNAP") {
        connect(this, SIGNAL(toggled(bool)), this, SLOT(toggleSnap(bool)));
    }
    else if (objectName() == "StatusBarButtonGRID") {
        connect(this, SIGNAL(toggled(bool)), this, SLOT(toggleGrid(bool)));
    }
    else if (objectName() == "StatusBarButtonRULER") {
        connect(this, SIGNAL(toggled(bool)), this, SLOT(toggleRuler(bool)));
    }
    else if (objectName() == "StatusBarButtonORTHO") {
        connect(this, SIGNAL(toggled(bool)), this, SLOT(toggleOrtho(bool)));
    }
    else if (objectName() == "StatusBarButtonPOLAR") {
        connect(this, SIGNAL(toggled(bool)), this, SLOT(togglePolar(bool)));
    }
    else if (objectName() == "StatusBarButtonQSNAP") {
        connect(this, SIGNAL(toggled(bool)), this, SLOT(toggleQSnap(bool)));
    }
    else if (objectName() == "StatusBarButtonQTRACK") {
        connect(this, SIGNAL(toggled(bool)), this, SLOT(toggleQTrack(bool)));
    }
    else if (objectName() == "StatusBarButtonLWT") {
        connect(this, SIGNAL(toggled(bool)), this, SLOT(toggleLwt(bool)));
    }
}

void StatusBarButton::contextMenuEvent(QContextMenuEvent *event)
{
    QApplication::setOverrideCursor(Qt::ArrowCursor);
    QMenu menu(this);
    if (objectName() == "StatusBarButtonSNAP") {
        QAction* settingsSnapAction = new QAction(create_icon("gridsnapsettings"), "&Settings...", &menu);
        connect(settingsSnapAction, SIGNAL(triggered()), this, SLOT(settingsSnap()));
        menu.addAction(settingsSnapAction);
    }
    else if (objectName() == "StatusBarButtonGRID") {
        QAction* settingsGridAction = new QAction(create_icon("gridsettings"), "&Settings...", &menu);
        connect(settingsGridAction, SIGNAL(triggered()), this, SLOT(settingsGrid()));
        menu.addAction(settingsGridAction);
    }
    else if (objectName() == "StatusBarButtonRULER") {
        QAction* settingsRulerAction = new QAction(create_icon("rulersettings"), "&Settings...", &menu);
        connect(settingsRulerAction, SIGNAL(triggered()), this, SLOT(settingsRuler()));
        menu.addAction(settingsRulerAction);
    }
    else if (objectName() == "StatusBarButtonORTHO") {
        QAction* settingsOrthoAction = new QAction(create_icon("orthosettings"), "&Settings...", &menu);
        connect(settingsOrthoAction, SIGNAL(triggered()), this, SLOT(settingsOrtho()));
        menu.addAction(settingsOrthoAction);
    }
    else if (objectName() == "StatusBarButtonPOLAR") {
        QAction* settingsPolarAction = new QAction(create_icon("polarsettings"), "&Settings...", &menu);
        connect(settingsPolarAction, SIGNAL(triggered()), this, SLOT(settingsPolar()));
        menu.addAction(settingsPolarAction);
    }
    else if (objectName() == "StatusBarButtonQSNAP") {
        QAction* settingsQSnapAction = new QAction(create_icon("qsnapsettings"), "&Settings...", &menu);
        connect(settingsQSnapAction, SIGNAL(triggered()), this, SLOT(settingsQSnap()));
        menu.addAction(settingsQSnapAction);
    }
    else if (objectName() == "StatusBarButtonQTRACK") {
        QAction* settingsQTrackAction = new QAction(create_icon("qtracksettings"), "&Settings...", &menu);
        connect(settingsQTrackAction, SIGNAL(triggered()), this, SLOT(settingsQTrack()));
        menu.addAction(settingsQTrackAction);
    }
    else if (objectName() == "StatusBarButtonLWT") {
        View* gview = activeView();
        if (gview) {
            QAction* enableRealAction = new QAction(create_icon("realrender"), "&RealRender On", &menu);
            enableRealAction->setEnabled(!gview->isRealEnabled());
            connect(enableRealAction, SIGNAL(triggered()), this, SLOT(enableReal()));
            menu.addAction(enableRealAction);

            QAction* disableRealAction = new QAction(create_icon("realrender"), "&RealRender Off", &menu);
            disableRealAction->setEnabled(gview->isRealEnabled());
            connect(disableRealAction, SIGNAL(triggered()), this, SLOT(disableReal()));
            menu.addAction(disableRealAction);
        }

        QAction* settingsLwtAction = new QAction(create_icon("lineweightsettings"), "&Settings...", &menu);
        connect(settingsLwtAction, SIGNAL(triggered()), this, SLOT(settingsLwt()));
        menu.addAction(settingsLwtAction);
    }
    menu.exec(event->globalPos());
    QApplication::restoreOverrideCursor();
    statusbar->clearMessage();
}

void StatusBarButton::settingsSnap()
{
    _main->settingsDialog("Snap");
}

void StatusBarButton::settingsGrid()
{
    _main->settingsDialog("Grid/Ruler");
}

void StatusBarButton::settingsRuler()
{
    _main->settingsDialog("Grid/Ruler");
}

void StatusBarButton::settingsOrtho()
{
    _main->settingsDialog("Ortho/Polar");
}

void StatusBarButton::settingsPolar()
{
    _main->settingsDialog("Ortho/Polar");
}

void StatusBarButton::settingsQSnap()
{
    _main->settingsDialog("QuickSnap");
}

void StatusBarButton::settingsQTrack()
{
    _main->settingsDialog("QuickTrack");
}

void StatusBarButton::settingsLwt()
{
    _main->settingsDialog("LineWeight");
}

void StatusBarButton::toggleSnap(bool on)
{
    qDebug("StatusBarButton toggleSnap()");
    View* gview = activeView();
    if (gview) {
        gview->toggleSnap(on);
    }
}

void StatusBarButton::toggleGrid(bool on)
{
    qDebug("StatusBarButton toggleGrid()");
    View* gview = activeView();
    if (gview) {
        gview->toggleGrid(on);
    }
}

void StatusBarButton::toggleRuler(bool on)
{
    qDebug("StatusBarButton toggleRuler()");
    View* gview = activeView();
    if (gview) {
        gview->toggleRuler(on);
    }
}

void StatusBarButton::toggleOrtho(bool on)
{
    qDebug("StatusBarButton toggleOrtho()");
    View* gview = activeView();
    if (gview) {
        gview->toggleOrtho(on);
    }
}

void StatusBarButton::togglePolar(bool on)
{
    qDebug("StatusBarButton togglePolar()");
    View* gview = activeView();
    if (gview) {
        gview->togglePolar(on);
    }
}

void StatusBarButton::toggleQSnap(bool on)
{
    qDebug("StatusBarButton toggleQSnap()");
    View* gview = activeView();
    if (gview) {
        gview->toggleQSnap(on);
    }
}

void StatusBarButton::toggleQTrack(bool on)
{
    qDebug("StatusBarButton toggleQTrack()");
    View* gview = activeView();
    if (gview) {
        gview->toggleQTrack(on);
    }
}

void StatusBarButton::toggleLwt(bool on)
{
    qDebug("StatusBarButton toggleLwt()");
    View* gview = activeView();
    if (gview) {
        gview->toggleLwt(on);
    }
}

void StatusBarButton::enableLwt()
{
    qDebug("StatusBarButton enableLwt()");
    View* gview = activeView();
    if (gview) {
        if (!gview->isLwtEnabled()) {
            gview->toggleLwt(true);
        }
    }
}

void StatusBarButton::disableLwt()
{
    qDebug("StatusBarButton disableLwt()");
    View* gview = activeView();
    if (gview) {
        if (gview->isLwtEnabled()) {
            gview->toggleLwt(false);
        }
    }
}

void StatusBarButton::enableReal()
{
    qDebug("StatusBarButton enableReal()");
    View* gview = activeView();
    if (gview) {
        gview->toggleReal(true);
    }
}

void StatusBarButton::disableReal()
{
    qDebug("StatusBarButton disableReal()");
    View* gview = activeView();
    if (gview) {
        gview->toggleReal(false);
    }
}

StatusBar::StatusBar(MainWindow* mw, QWidget *parent) : QStatusBar(parent)
{
    this->setObjectName("StatusBar");

    statusBarSnapButton   = new StatusBarButton("SNAP", mw, this, this);
    statusBarGridButton   = new StatusBarButton("GRID", mw, this, this);
    statusBarRulerButton  = new StatusBarButton("RULER", mw, this, this);
    statusBarOrthoButton  = new StatusBarButton("ORTHO", mw, this, this);
    statusBarPolarButton  = new StatusBarButton("POLAR", mw, this, this);
    statusBarQSnapButton  = new StatusBarButton("QSNAP", mw, this, this);
    statusBarQTrackButton = new StatusBarButton("QTRACK", mw, this, this);
    statusBarLwtButton    = new StatusBarButton("LWT", mw, this, this);
    statusBarMouseCoord   = new QLabel(this);

    statusBarMouseCoord->setMinimumWidth(300); // Must fit this text always
    statusBarMouseCoord->setMaximumWidth(300); // "+1.2345E+99, +1.2345E+99, +1.2345E+99"

    this->addWidget(statusBarMouseCoord);
    this->addWidget(statusBarSnapButton);
    this->addWidget(statusBarGridButton);
    this->addWidget(statusBarRulerButton);
    this->addWidget(statusBarOrthoButton);
    this->addWidget(statusBarPolarButton);
    this->addWidget(statusBarQSnapButton);
    this->addWidget(statusBarQTrackButton);
    this->addWidget(statusBarLwtButton);
}

void StatusBar::setMouseCoord(double x, double y)
{
    //TODO: set format from settings (Architectural, Decimal, Engineering, Fractional, Scientific)

    //Decimal
    statusBarMouseCoord->setText(QString().setNum(x, 'F', 4) + ", " + QString().setNum(y, 'F', 4)); //TODO: use precision from unit settings

    //Scientific
    //statusBarMouseCoord->setText(QString().setNum(x, 'E', 4) + ", " + QString().setNum(y, 'E', 4)); //TODO: use precision from unit settings
}

/* . */
SelectBox::SelectBox(Shape s, QWidget* parent) : QRubberBand(s, parent)
{
    //Default values
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
    qDebug("SelectBox setColors()");
    alpha = newAlpha;

    leftPenColor = colorL; //TODO: allow customization
    leftBrushColor = QColor(fillL.red(), fillL.green(), fillL.blue(), alpha);
    rightPenColor = colorR; //TODO: allow customization
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
    //HACK: Take that QRubberBand!
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

LayerManager::~LayerManager()
{
    QApplication::restoreOverrideCursor();
}

void LayerManager::addLayer(const QString& name,
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
