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

QToolButton* statusBarSnapButton;
QToolButton* statusBarGridButton;
QToolButton* statusBarRulerButton;
QToolButton* statusBarOrthoButton;
QToolButton* statusBarPolarButton;
QToolButton* statusBarQSnapButton;
QToolButton* statusBarQTrackButton;
QToolButton* statusBarLwtButton;
QLabel* statusBarMouseCoord;

QStringList button_list = {
    "SNAP",
    "GRID",
    "RULER",
    "ORTHO",
    "POLAR",
    "QSNAP",
    "QTRACK",
    "LWT"
};

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
        View* gview = activeView();
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
    QApplication::restoreOverrideCursor();
    statusbar->clearMessage();
}

void
statusbar_toggle(QString key, bool on)
{
    qDebug("StatusBarButton toggleSnap()");
    View* gview = activeView();
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
    qDebug("StatusBarButton enableLwt()");
    View* gview = activeView();
    if (gview) {
        if (!gview->isLwtEnabled()) {
            gview->toggleLwt(true);
        }
    }
}

void
disableLwt()
{
    qDebug("StatusBarButton disableLwt()");
    View* gview = activeView();
    if (gview) {
        if (gview->isLwtEnabled()) {
            gview->toggleLwt(false);
        }
    }
}

void
enableReal()
{
    qDebug("StatusBarButton enableReal()");
    View* gview = activeView();
    if (gview) {
        gview->toggleReal(true);
    }
}

void
disableReal()
{
    qDebug("StatusBarButton disableReal()");
    View* gview = activeView();
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

    statusBarMouseCoord->setMinimumWidth(300); // Must fit this text always
    statusBarMouseCoord->setMaximumWidth(300); // "+1.2345E+99, +1.2345E+99, +1.2345E+99"

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
