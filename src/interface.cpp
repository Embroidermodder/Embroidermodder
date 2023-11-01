/*
 *  Embroidermodder 2.
 *
 *  ------------------------------------------------------------
 *
 *  Copyright 2013-2023 The Embroidermodder Team
 *  Embroidermodder 2 is Open Source Software.
 *  See LICENSE for licensing terms.
 *
 *  ------------------------------------------------------------
 *
 *  Use Python's PEP7 style guide.
 *      https://peps.python.org/pep-0007/
 *
 *  ------------------------------------------------------------
 *
 *  interface.cpp
 *  For wrappers to the Qt internals.
 *
 *  To help reduce reliance on Qt, only the functions wrap the
 *  Qt functions have a wrapper here. Ideally we could move some
 *  of the Qt headers here.
 *
 *  ------------------------------------------------------------
 *
 *  The tree walking, merging, freeing and otherwise altering functions
 *  are an attempt to create a solid foundation for the internal scripting
 *  language by having a fully abstract data model similar to an AST for
 *  the whole state of the program at run-time.
 */

#include "embroidermodder.h"

#include <time.h>

/* Make the translation function global in scope. */
QString
translate_str(const char *str)
{
    return _mainWin->tr(str);
}

/* Tokenize our std::string type using a 1 character deliminator. */
std::vector<std::string>
tokenize(std::string str, const char delim)
{
    std::vector<std::string> list;
    std::stringstream str_stream(str);
    std::string s;
    while (std::getline(str_stream, s, delim)) {
        list.push_back(s);
    }
    return list;
}

/* Convert an EmbVector to a QPointF. */
QPointF
to_QPointF(EmbVector a)
{
    QPointF result(a.x, a.y);
    return result;
}

/* Convert a QPointF to an EmbVector. */
EmbVector
to_EmbVector(QPointF a)
{
    EmbVector v;
    v.x = a.x();
    v.y = a.y();
    return v;
}

/* Debug message wrapper for qDebug.
 *
 * For debugging code running on other machines append these messages to log
 * file.
 *
 * Timestamps are added to each message so we can trace in what order things
 * happen.
 */
void
debug_message(std::string msg)
{
    struct timespec ts;
    clock_gettime(CLOCK_REALTIME, &ts);
    qDebug(msg.c_str());
    FILE *f = fopen("debug.txt", "a");
    fprintf(f, "%.2ld:%.2ld:%.2ld.%.3ld> %s\n",
        (ts.tv_sec/3600)%24, (ts.tv_sec%3600)/60, ts.tv_sec%60, ts.tv_nsec%1000, msg.c_str());
    fclose(f);
}

/* Utility function for add_to_path. */
std::vector<float>
get_n_reals(std::vector<std::string> list, int n, int *offset)
{
    std::vector<float> result;
    for (int i=1; i<n+1; i++) {
        result.push_back(std::stof(list[*offset+i]));
    }
    *offset += n;
    return result;
}

/* Render an SVG-like .
 *
 */
QPainterPath
add_to_path(QPainterPath path, EmbVector scale, std::string command)
{
    std::vector<std::string> list = tokenize(command, ' ');
    for (int i=0; i<(int)list.size(); i++) {
        command = list[i];
        if (command == "M") {
            std::vector<float> r = get_n_reals(list, 2, &i);
            path.moveTo(r[0]*scale.x, r[1]*scale.y);
        }
        else if (command == "L") {
            std::vector<float> r = get_n_reals(list, 2, &i);
            path.lineTo(r[0]*scale.x, r[1]*scale.y);
        }
        else if (command == "A") {
            std::vector<float> r = get_n_reals(list, 6, &i);
            path.arcTo(r[0]*scale.x, r[1]*scale.y, r[2]*scale.x, r[3]*scale.y, r[4], r[5]);
        }
        else if (command == "AM") {
            std::vector<float> r = get_n_reals(list, 5, &i);
            path.arcMoveTo(r[0]*scale.x, r[1]*scale.y, r[2]*scale.x, r[3]*scale.y, r[4]);
        }
        else if (command == "E") {
            std::vector<float> r = get_n_reals(list, 4, &i);
            path.addEllipse(QPointF(r[0], r[1]), r[2], r[3]);
        }
    }
    return path;
}

/* Set whether the of parent's object that has the name given by key
 * is enabled.
 *
 * TODO: error reporting.
 */
void
set_enabled(QObject* parent, const char *key, bool enabled)
{
    if (!strncmp(key, "lineEdit", 8)) {
        QLabel* label = parent->findChild<QLabel*>(key);
        if (label) {
            label->setEnabled(enabled);
        }
        return;
    }
    if (!strncmp(key, "comboBox", 8)) {
        QComboBox* comboBox = parent->findChild<QComboBox*>(key);
        if (comboBox) {
            comboBox->setEnabled(enabled);
        }
        return;
    }
    if (!strncmp(key, "checkBox", 8)) {
        QCheckBox* checkBox = parent->findChild<QCheckBox*>(key);
        if (checkBox) {
            checkBox->setEnabled(enabled);
        }
        return;
    }
    if (!strncmp(key, "button", 6)) {
        QPushButton* button = parent->findChild<QPushButton*>(key);
        if (button) {
            button->setEnabled(enabled);
        }
        return;
    }
}

/* Set visibility of parent's object that has the name given by key to
 * the boolean value in visibility.
 *
 * TODO: error reporting.
 */
void
set_visibility(QObject* parent, const char *key, bool visibility)
{
    if (!strncmp(key, "lineEdit", 8)) {
        QLabel* label = parent->findChild<QLabel*>(key);
        if (label) {
            label->setVisible(visibility);
        }
        return;
    }
    if (!strncmp(key, "spinBox", 7)) {
        QDoubleSpinBox* spinbox = parent->findChild<QDoubleSpinBox*>(key);
        if (spinbox) {
            spinbox->setVisible(visibility);
        }
        return;
    }
}

/* Turn our own markup in config.toml into the various UI elements.
 *
 * The required variable "type" is taken to choose a function to process
 * the UI element.
 *
 * This would be faster as a switch table.
 *
 * This function should take a parent object to build...
 */
void
make_ui_element(
    std::string element_type
)
{
    if (element_type == "groupBox") {

    }
    else if (element_type == "lineEdit") {

    }
    else if (element_type == "checkBox") {
        // make_checkbox(gb, description, label, icon, key);
    }
    else if (element_type == "spinBox") {

    }
    else if (element_type == "doubleSpinBox") {
        // make_spinbox(gb, description, object_name, single_step, lower, upper, key)
    }
    else if (element_type == "label") {

    }
    else if (element_type == "comboBox") {

    }
    else if (element_type == "toolButton") {

    }
    else if (element_type == "action") {

    }
    else if (element_type == "toolbar") {

    }
    else if (element_type == "menu") {

    }
}

/* . */
QCheckBox *
make_checkbox(
    QGroupBox *gb,
    std::string dictionary,
    const char *label,
    const char *icon,
    int key)
{
    QCheckBox *checkBox = new QCheckBox(translate_str(label), gb);
    checkBox->setIcon(_mainWin->create_icon(icon));
    /* checkBox->setName(); */
    if (dictionary == "settings") {
        checkBox->setChecked(settings[key].i);
        QObject::connect(checkBox, &QCheckBox::stateChanged, _mainWin,
            [=](int x) { settings[key].i = x; });
    }
    if (dictionary == "dialog") {
        checkBox->setChecked(dialog[key].i);
        QObject::connect(checkBox, &QCheckBox::stateChanged, _mainWin,
            [=](int x) { dialog[key].i = x; });
    }
/*    if (dictionary == "config") {
        checkBox->setChecked(config[key].i);
        QObject::connect(checkBox, &QCheckBox::stateChanged, _mainWin,
            [=](int x) { config[key].i = x; });
    }
*/
    return checkBox;
}

/* TODO: Make spinbox using this toml syntax:
 *
 *     [zoom_level_spinbox]
 *     type = "doubleSpinBox"
 *     object_name = "Zoom Level"
 *     single_step = 0.1
 *     lower_bound = -10
 *     upper_bound = 10
 *     key = "settings.zoom_level"
 *
 * The content (not including):
 *
 *     QDoubleSpinBox *sb = make_spinbox(gb, desc);
 */
QDoubleSpinBox *
make_spinbox(QGroupBox *gb, std::string dictionary,
    QString object_name, EmbReal single_step, EmbReal lower, EmbReal upper,
    int key)
{
    QDoubleSpinBox* spinBox = new QDoubleSpinBox(gb);
    spinBox->setObjectName(object_name);
    spinBox->setSingleStep(single_step);
    spinBox->setRange(lower, upper);
    if (dictionary == "settings") {
        spinBox->setValue(settings[key].r);
        QObject::connect(spinBox, &QDoubleSpinBox::valueChanged, _mainWin,
            [=](double x){ settings[key].r = (EmbReal)x; });
    }
    if (dictionary == "dialog") {
        spinBox->setValue(dialog[key].r);
        QObject::connect(spinBox, &QDoubleSpinBox::valueChanged, _mainWin,
            [=](double x){ dialog[key].r = (EmbReal)x; });
    }
/*
    if (dictionary == "config") {
        spinBox->setValue(config[key].r);
        QObject::connect(spinBox, &QDoubleSpinBox::valueChanged, _mainWin,
            [=](double x){ config[key].r = (EmbReal)x; });
    }
*/
    return spinBox;
}

/* Create the image widget for filename. */
ImageWidget::ImageWidget(QString filename, QWidget* parent) : QWidget(parent)
{
    debug_message("ImageWidget Constructor");

    img.load(filename);

    setMinimumWidth(img.width());
    setMinimumHeight(img.height());
    setMaximumWidth(img.width());
    setMaximumHeight(img.height());

    this->show();
}

/* Load image to widget. */
bool
ImageWidget::load(QString fileName)
{
    img.load(fileName);
    return true;
}

/* Save image from widget. */
bool
ImageWidget::save(QString fileName)
{
    img.save(fileName, "PNG");
    return true;
}

/* Destroy image widget. */
ImageWidget::~ImageWidget()
{
    debug_message("ImageWidget Destructor");
}

/* Paint the widget. */
void
ImageWidget::paintEvent(QPaintEvent*)
{
    QPainter painter(this);
    painter.setViewport(0, 0, width(), height());
    painter.setWindow(0, 0, width(), height());
    painter.drawImage(0, 0, img);
}

/* Create the Layer Manager. */
LayerManager::LayerManager(QWidget* parent) : QDialog(parent)
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

/* Destroy the Layer Manager. */
LayerManager::~LayerManager()
{
    QApplication::restoreOverrideCursor();
}

/* Add layer to Layer Manager. */
void
LayerManager::addLayer(
    QString name,
    const bool visible,
    const bool frozen,
    const EmbReal zValue,
    const QRgb color,
    QString lineType,
    QString lineWeight,
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


SelectBox::SelectBox(Shape s, QWidget* parent) : QRubberBand(s, parent)
{
    //Default values
    setColors(QColor(Qt::darkGreen), QColor(Qt::green), QColor(Qt::darkBlue), QColor(Qt::blue), 32);
}

/* . */
void
SelectBox::setDirection(int dir)
{
    if (!dir) { dirPen = leftPen;  dirBrush = leftBrush;  }
    else     { dirPen = rightPen; dirBrush = rightBrush; }
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

void SelectBox::paintEvent(QPaintEvent*)
{
    QPainter painter(this);
    painter.setPen(dirPen);
    painter.fillRect(0,0,width()-1, height()-1, dirBrush);
    painter.drawRect(0,0,width()-1, height()-1);
}

void SelectBox::forceRepaint()
{
    //HACK: Take that QRubberBand!
    QSize hack = size();
    resize(hack + QSize(1,1));
    resize(hack);
}

/*
 */
PreviewDialog::PreviewDialog(QWidget* parent, QString caption, QString dir,
                             QString filter) : QFileDialog(parent, caption, dir, filter)
{
    qDebug("PreviewDialog Constructor");

    //TODO: get actual thumbnail image from file, lets also use a size of 128x128 for now...
    //TODO: make thumbnail size adjustable thru settings dialog
    imgWidget = new ImageWidget("icons/default/nopreview.png", this);

    QLayout* lay = layout();
    if (qobject_cast<QGridLayout*>(lay)) {
        QGridLayout* grid = qobject_cast<QGridLayout*>(lay);
        grid->addWidget(imgWidget, 0, grid->columnCount(), grid->rowCount(), 1);
    }

    setModal(true);
    setOption(QFileDialog::DontUseNativeDialog);
    setViewMode(QFileDialog::Detail);
    setFileMode(QFileDialog::ExistingFiles);

    //TODO: connect the currentChanged signal to update the preview imgWidget.
}

/*
 */
PreviewDialog::~PreviewDialog()
{
    qDebug("PreviewDialog Destructor");
}


/* . */
UndoEditor::UndoEditor(QString  iconDirectory, QWidget* widgetToFocus, QWidget* parent) : QDockWidget(parent)
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

/* Destroy the undo editor, maintaining the stack. */
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
UndoEditor::canUndo()
{
    return undoGroup->canUndo();
}

/* . */
bool
UndoEditor::canRedo()
{
    return undoGroup->canRedo();
}

/* . */
QString
UndoEditor::undoText()
{
    return undoGroup->undoText();
}

/* . */
QString
UndoEditor::redoText()
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


StatusBar::StatusBar(QWidget *parent) : QStatusBar(parent)
{
    this->setObjectName("StatusBar");

    int n = string_array_length(button_list);
    for (int i=0; i<n; i++) {
        QString name = "StatusBarButton";
        QToolButton* button = new QToolButton(parent);
        button->setObjectName(name + button_list[i]);

        button->setText(QString::fromStdString(button_list[i]));
        button->setAutoRaise(true);
        button->setCheckable(true);

        connect(button, &QToolButton::toggled, this, [=](bool a) { toggle(button_list[i], a); } );
        buttons[button_list[i]] = button;
    }

    statusBarMouseCoord = new QLabel(this);

    statusBarMouseCoord->setMinimumWidth(300); // Must fit this text always
    statusBarMouseCoord->setMaximumWidth(300); // "+1.2345E+99, +1.2345E+99, +1.2345E+99"

    this->addWidget(statusBarMouseCoord);
    for (int i=0; i<n; i++) {
        this->addWidget(buttons[button_list[i]]);
    }
}

void StatusBar::setMouseCoord(EmbReal x, EmbReal y)
{
    //TODO: set format from settings (Architectural, Decimal, Engineering, Fractional, Scientific)

    //Decimal
    statusBarMouseCoord->setText(QString().setNum(x, 'F', 4) + ", " + QString().setNum(y, 'F', 4)); //TODO: use precision from unit settings

    //Scientific
    //statusBarMouseCoord->setText(QString().setNum(x, 'E', 4) + ", " + QString().setNum(y, 'E', 4)); //TODO: use precision from unit settings
}

/**
 * .
 */
void
StatusBar::context_menu_action(QToolButton *button, const char *icon, const char *label, QMenu *menu, std::string setting_page)
{
    QAction* action = new QAction(_mainWin->create_icon(icon), label, menu);
    connect(action, &QAction::triggered, button, [=](){ actuator("settingsdialog " + setting_page ); });
    menu->addAction(action);
}

/**
 * .
 */
void
StatusBar::context_menu_event(QContextMenuEvent *event, QToolButton *button)
{
    QApplication::setOverrideCursor(Qt::ArrowCursor);
    QMenu menu(button);
    QString name = button->objectName();
    if (name == "StatusBarButtonSNAP") {
        context_menu_action(button, "gridsnapsettings", "&Settings...", &menu, "Snap");
    }
    else if (name == "StatusBarButtonGRID") {
        context_menu_action(button, "gridsettings", "&Settings...", &menu, "Grid/Ruler");
    }
    else if (name == "StatusBarButtonRULER") {
        context_menu_action(button, "rulersettings", "&Settings...", &menu, "Grid/Ruler");
    }
    else if (name == "StatusBarButtonORTHO") {
        context_menu_action(button, "orthosettings", "&Settings...", &menu, "Ortho/Polar");
    }
    else if (name == "StatusBarButtonPOLAR") {
        context_menu_action(button, "polarsettings", "&Settings...", &menu, "Ortho/Polar");
    }
    else if (name == "StatusBarButtonQSNAP") {
        context_menu_action(button, "qsnapsettings", "&Settings...", &menu, "QuickSnap");
    }
    else if (name == "StatusBarButtonQTRACK") {
        context_menu_action(button, "qtracksettings", "&Settings...", &menu, "QuickTrack");
    }
    else if (name == "StatusBarButtonLWT") {
        View* gview = activeView();
        if (gview) {
            QAction* enableRealAction = new QAction(_mainWin->create_icon("realrender"), "&RealRender On", &menu);
            QAction* disableRealAction = new QAction(_mainWin->create_icon("realrender"), "&RealRender Off", &menu);

            enableRealAction->setEnabled(!(gview->state & VIEW_STATE_REAL));
            disableRealAction->setEnabled(gview->state & VIEW_STATE_REAL);

            menu.addAction(enableRealAction);
            menu.addAction(disableRealAction);

            /*
            QObject::connect(enableRealAction, &(QToolButton::triggered),
               this,  [=](){ gview->setFlag(VIEW_STATE_REAL); });

            QObject::connect(disableRealAction, &QToolButton::triggered,
                this, [=](){ gview->unsetFlag(VIEW_STATE_REAL); });
            */
        }

        context_menu_action(button, "lineweightsettings", "&Settings...", &menu, "LineWeight");
    }
    menu.exec(event->globalPos());
    QApplication::restoreOverrideCursor();
    statusbar->clearMessage();
}

void
StatusBar::toggle(std::string key, bool on)
{
    debug_message("StatusBarButton toggleSnap()");
    View* gview = activeView();
    if (!gview) {
        return;
    }
    uint64_t flag = VIEW_STATE_GRID;
    if (key == "SNAP") {
        flag = VIEW_STATE_SNAP;
    }
    else if (key == "GRID") {
        flag = VIEW_STATE_GRID;
    }
    else if (key == "RULER") {
        flag = VIEW_STATE_RULER;
    }
    else if (key == "ORTHO") {
        flag = VIEW_STATE_ORTHO;
    }
    else if (key == "POLAR") {
        flag = VIEW_STATE_POLAR;
    }
    else if (key == "QSNAP") {
        flag = VIEW_STATE_QSNAP;
    }
    else if (key == "QTRACK") {
        flag = VIEW_STATE_QTRACK;
    }
    else if (key == "LWT") {
        flag = VIEW_STATE_LWT;
    }

    if (on) {
        gview->setFlag(flag);
    }
    else {
        gview->unsetFlag(flag);
    }
}

