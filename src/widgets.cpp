/*
 * Embroidermodder 2.
 *
 * Copyright 2011-2024 The Embroidermodder Team
 * Embroidermodder 2 is Open Source Software, see LICENSE.md for licensing terms.
 *
 * Read the reference manual (https://www.libembroidery.org/downloads/emrm.pdf)
 * for advice on altering this file.
 *
 * MainWindow Commands
 */


/* Qt Headers */
#include <QAction>
#include <QApplication>
#include <QBrush>
#include <QClipboard>
#include <QCloseEvent>
#include <QColor>
#include <QComboBox>
#include <QContextMenuEvent>
#include <QDate>
#include <QDateTime>
#include <QDebug>
#include <QDesktopServices>
#include <QDialog>
#include <QDialogButtonBox>
#include <QDir>
#include <QDockWidget>
#include <QFile>
#include <QFileDialog>
#include <QFileInfo>
#include <QFontComboBox>
#include <QFormLayout>
#include <QFrame>
#include <QGraphicsItem>
#include <QGraphicsPathItem>
#include <QGraphicsScene>
#include <QGraphicsView>
#include <QGridLayout>
#include <QGroupBox>
#include <QImage>
#include <QKeyEvent>
#include <QLabel>
#include <QLineEdit>
#include <QList>
#include <QLocale>
#include <QMainWindow>
#include <QMdiArea>
#include <QMdiSubWindow>
#include <QMenu>
#include <QMenuBar>
#include <QMessageBox>
#include <QMetaObject>
#include <QObject>
#include <QPainter>
#include <QPainterPath>
#include <QPen>
#include <QPixmap>
#include <QPointF>
#include <QProcess>
#include <QPushButton>
#include <QRubberBand>
#include <QScrollArea>
#include <QScrollBar>
#include <QSettings>
#include <QSignalMapper>
#include <QSplitter>
#include <QStandardPaths>
#include <QStatusBar>
#include <QString>
#include <QStyleOption>
#include <QTextBrowser>
#include <QTextLayout>
#include <QTextStream>
#include <QToolBar>
#include <QToolButton>
#include <QTransform>
#include <QUndoCommand>
#include <QUndoGroup>
#include <QUndoStack>
#include <QUndoView>
#include <QUrl>
#include <QVBoxLayout>
#include <QWhatsThis>

#include <QtPrintSupport>

/* C++ Headers */
#include <unordered_map>
#include <string>
#include <vector>

/* C Headers */
#include "core.h"

class CmdPrompt;
class ImageWidget;
class MainWindow;
class MdiArea;
class MdiWindow;
class Object;
class PropertyEditor;
class SelectBox;
class UndoEditor;
class Document;
class CmdPromptInput;

#include <chrono>
#include <thread>

#define NUMBINS 10

/* Could initialise all documents to NULL rather than having a seperate memory
 * usage array?
 */
Document *documents[MAX_OPEN_FILES];
EmbVector obj_map_rubber(Object *obj, const char *key);
EmbVector map_from_scene(Object *obj, EmbVector v);
void to_string_table(QStringList src, EmbStringTable dest);

std::unordered_map<int, QAction*> actionHash;
QToolBar* toolbar[N_TOOLBARS];
QMenu* menu[N_MENUS];

QList<QGraphicsItem*> cutCopyObjectList;

QStatusBar* statusbar;
MdiArea* mdiArea;
CmdPrompt* prompt;
PropertyEditor* dockPropEdit;
UndoEditor* dockUndoEdit;
QTimer* testing_timer;

QList<MdiWindow*> listMdiWin;

QAction* myFileSeparator;

QWizard* wizardTipOfTheDay;
QLabel* labelTipOfTheDay;
QCheckBox* checkBoxTipOfTheDay;

/* Selectors */
QComboBox* layerSelector;
QComboBox* colorSelector;
QComboBox* linetypeSelector;
QComboBox* lineweightSelector;
QFontComboBox* textFontSelector;
QComboBox* textSizeSelector;

QByteArray layoutState;

MainWindow *_main;

StringMap aliases[MAX_ALIASES];
std::unordered_map<std::string, std::string> aliasHash;
 
QString curText;
QString defaultPrefix;
QString prefix;

QString lastCmd;
QString curCmd;

QTextBrowser* promptHistory;
CmdPromptInput* promptInput;

QTimer* blinkTimer;
bool blinkState;
/* NOTE: These shortcuts need to be caught since QLineEdit uses them. */

std::unordered_map<int, int> key_map = {
    {QKeySequence::Cut, CUT_SEQUENCE},
    {QKeySequence::Copy, COPY_SEQUENCE},
    {QKeySequence::Paste, PASTE_SEQUENCE},
    {QKeySequence::SelectAll, SELECT_ALL_SEQUENCE},
    {QKeySequence::Undo, UNDO_SEQUENCE},
    {QKeySequence::Redo, REDO_SEQUENCE},
    {Qt::Key_Delete, DELETE_KEY},
    {Qt::Key_Tab, TAB_KEY},
    {Qt::Key_Escape, ESCAPE_KEY},
    {Qt::Key_Up, UP_KEY},
    {Qt::Key_Down, DOWN_KEY},
    {Qt::Key_F1, F1_KEY},
    {Qt::Key_F2, F2_KEY},
    {Qt::Key_F3, F3_KEY},
    {Qt::Key_F4, F4_KEY},
    {Qt::Key_F5, F5_KEY},
    {Qt::Key_F6, F6_KEY},
    {Qt::Key_F7, F7_KEY},
    {Qt::Key_F8, F8_KEY},
    {Qt::Key_F9, F9_KEY},
    {Qt::Key_F10, F10_KEY},
    {Qt::Key_F11, F11_KEY},
    {Qt::Key_F12, F12_KEY},
    {Qt::Key_Shift, SHIFT_KEY}
};

QToolButton* statusBarButtons[N_SB_BUTTONS];
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
QGroupBox* createGroupBox__(const char *label, const char *name, Editor editor_data[]);

std::unordered_map<QString, QGroupBox*> group_boxes;
std::unordered_map<QString, QToolButton*> tool_buttons;
std::unordered_map<QString, QLineEdit*> line_edits;
std::unordered_map<QString, QComboBox*> combo_boxes;
QComboBox* comboBoxSelected;

QStringList group_box_list = {
    "General",
    "GeometryArc",
    "MiscArc",
    "GeometryBlock",
    "GeometryCircle",
    "GeometryDimAligned",
    "GeometryDimAngular",
    "GeometryDimArcLength",
    "GeometryDimDiameter",
    "GeometryDimLeader",
    "GeometryDimLinear",
    "GeometryDimOrdinate",
    "GeometryDimRadius",
    "GeometryEllipse",
    "GeometryImage",
    "MiscImage",
    "GeometryInfiniteLine",
    "GeometryLine",
    "GeometryPath",
    "MiscPath",
    "GeometryPoint",
    "GeometryPolygon",
    "GeometryPolyline",
    "MiscPolyline",
    "GeometryRay",
    "GeometryRectangle",
    "GeometryTextMulti",
    "TextTextSingle",
    "GeometryTextSingle",
    "MiscTextSingle"
};

QWidget* focusWidget_;

QString iconDir;
int iconSize;

bool pickAdd;

Object* tempObj;

int precisionAngle;
int precisionLength;

/* Used when checking if fields vary. */
QString fieldOldText;
QString fieldNewText;
QString fieldVariesText;
QString fieldYesText;
QString fieldNoText;
QString fieldOnText;
QString fieldOffText;

QFontComboBox* comboBoxTextSingleFont;

QSignalMapper* signalMapper;
Qt::ToolButtonStyle propertyEditorButtonStyle;

QList<QGraphicsItem*> selectedItemList;

QToolButton* toolButtonQSelect;
QToolButton* toolButtonPickAdd;

/* File-scope Functions ----------------------------------------------------- */
void mapSignal(QObject* fieldObj, QString name, QVariant value);
QToolButton* createToolButton(QString iconName, QString txt);

void updateLineEditStrIfVaries(QLineEdit* lineEdit, QString str);
void update_lineedit_num(QLineEdit* lineEdit, double num, bool useAnglePrecision);
void updateFontComboBoxStrIfVaries(QFontComboBox* fontComboBox, QString str);
void update_lineedit_str(QComboBox* comboBox, QString str, const QStringList& strList);
void update_lineedit_bool(QComboBox* comboBox, bool val, bool yesOrNoText);

void showGroups(int objType);
void fieldEdited(QObject* fieldObj);
void showOneType(int index);
void hideAllGroups();
void clearAllFields();

QComboBox* createComboBoxSelected();
QToolButton* createToolButtonQSelect();
QToolButton* createToolButtonPickAdd();

void create_statusbar(MainWindow* mw);

MdiWindow* activeMdiWindow();
QGraphicsScene* activeScene();
QUndoStack* activeUndoStack();
QString platformString();

QToolButton *create_statusbarbutton(QString buttonText, MainWindow* mw);
QIcon create_icon(QString icon);
QPixmap create_pixmap(QString icon);

void nativeAlert(std::string txt);
void nativeAppendPromptHistory(std::string txt);

void nativeAddPolygon(double startX, double startY, const QPainterPath& p, int rubberMode);
void nativeAddPolyline(double startX, double startY, const QPainterPath& p, int rubberMode);
void nativeAddPath(double startX, double startY, const QPainterPath& p, int rubberMode);

void nativeAddToSelection(const QPainterPath path, Qt::ItemSelectionMode mode);

QAction *get_action_by_icon(EmbString icon);

EmbVector to_emb_vector(QPointF p);
QPointF to_qpointf(EmbVector v);
EmbVector find_mouse_snap_point(QList<EmbVector> snap_points, EmbVector mouse_point);

void set_visibility(QObject *senderObj, EmbString key, bool visibility);
void set_enabled(QObject *senderObj, EmbString key, bool visibility);
void set_visibility_group(QObject *senderObj, EmbStringTable key, bool visibility);
void set_enabled_group(QObject *senderObj, EmbStringTable key, bool visibility);
QIcon create_swatch(int32_t color);
void preview_update(void);

void setHistory(QString txt);
void add_command(std::string alias, std::string cmd);

/* ------------------------ Object Functions --------------------------- */

Object *create_arc(EmbArc arc, QRgb rgb, QGraphicsItem *item=0);
Object *create_circle(EmbCircle circle, QRgb rgb, QGraphicsItem *item=0);
Object *create_ellipse(EmbEllipse ellipse, QRgb rgb, QGraphicsItem *item=0);
Object *create_polyline(EmbPath path, const QPainterPath& p, QRgb rgb, QGraphicsItem* parent=0);
Object *create_path(EmbVector v, const QPainterPath p, QRgb rgb, QGraphicsItem* parent=0);
Object *create_polygon(EmbVector v, const QPainterPath& p, QRgb rgb, QGraphicsItem* parent=0);
Object *create_text_single(QString str, EmbVector v, QRgb rgb, QGraphicsItem* parent=0);
Object *create_dim_leader(EmbLine line, QRgb rgb, QGraphicsItem* parent=0);
Object *create_image(EmbRect rect, QRgb rgb, QGraphicsItem* parent=0);
Object *create_rect(EmbRect rect, QRgb rgb, QGraphicsItem* parent=0);
Object *create_line(EmbLine line, QRgb rgb, QGraphicsItem* parent=0);
Object *create_point(EmbPoint_ point, QRgb rgb, QGraphicsItem* parent=0);

Object *copy_object(Object* obj);

Qt::PenStyle obj_line_type(Object* obj);
double  obj_line_weight(Object* obj);
QPainterPath obj_path(Object* obj);
EmbVector obj_rubber_point(Object* obj, QString key);
QString obj_rubber_text(Object* obj, QString key);

void obj_update_rubber(Object *obj, QPainter* painter);
void obj_update_rubber_grip(Object *obj, QPainter *painter);
void obj_update_leader(Object *obj);
void obj_update_path(Object *obj);
void obj_update_path_r(Object *obj, QPainterPath p);
void obj_update_arc_rect(Object *obj, double radius);

void obj_set_line_weight(Object *obj, double lineWeight);

void obj_real_render(Object *obj, QPainter* painter, QPainterPath renderPath);

void obj_set_rect(Object *obj, QRectF r);
void obj_set_rect(Object *obj, double x, double y, double w, double h);
QLineF obj_line(Object *obj);
void obj_set_line(Object *obj, QLineF li);
void obj_set_line(Object *obj, double x1, double y1, double x2, double y2);

void obj_set_path(Object *obj, QPainterPath p);
void obj_calculate_data(Object *obj);

int obj_find_index(Object *obj, EmbVector point);

void obj_set_color(Object *obj, const QColor& color);
void obj_set_color_rgb(Object *obj, QRgb rgb);
void obj_set_line_type(Object *obj, Qt::PenStyle lineType);

/* ---------------------- Document Functions --------------------------- */

Document *create_doc(MainWindow* mw, QGraphicsScene* theScene, QWidget *parent);

void doc_add_to_rubber_room(int32_t doc, QGraphicsItem* item);

void draw_arc(QPainter* painter, EmbArc arc);
void draw_circle(QPainter* painter, EmbCircle circle);
void draw_ellipse(QPainter* painter, EmbEllipse ellipse);
void draw_line(QPainter* painter, EmbLine line);
void draw_polygon(QPainter* painter, EmbPolygon polygon);
void draw_polyline(QPainter* painter, EmbPolyline polyline);
void draw_rect(QPainter* painter, EmbRect rect);
void draw_spline(QPainter* painter, EmbSpline spline);

QPainterPath doc_create_ruler_text_path(EmbString str, float height);

QList<QGraphicsItem*> doc_create_object_list(int32_t doc, QList<QGraphicsItem*> list);

void doc_start_gripping(int32_t doc, Object* obj);
void doc_stop_gripping(int32_t doc, bool accept = false);

void doc_add_object(int32_t doc, Object* obj);
void doc_delete_object(int32_t doc, Object* obj);
void doc_vulcanize_object(int32_t doc, Object* obj);

void textFontSelectorCurrentFontChanged(const QFont& font);

void onWindowActivated(QMdiSubWindow* w);

QAction* getFileSeparator();
QAction* createAction(Command command);
QMdiSubWindow* findMdiWindow(EmbString fileName);
void onCloseMdiWin(MdiWindow*);

/* ---------------------- Class Declarations --------------------------- */

class LayerManager : public QDialog
{
    Q_OBJECT

public:
    LayerManager(MainWindow* mw, QWidget *parent = 0);
    ~LayerManager();

    void addLayer(QString name,
        const bool visible,
        const bool frozen,
        const double zValue,
        const QRgb color,
        QString lineType,
        QString lineWeight,
        const bool print);

    QStandardItemModel*    layerModel;
    QSortFilterProxyModel* layerModelSorted;
    QTreeView*             treeView;
};

class MdiArea : public QMdiArea
{
    Q_OBJECT

public:
    MdiArea(MainWindow* mw, QWidget* parent = 0);
    ~MdiArea();

    void useBackgroundLogo(bool use);
    void useBackgroundTexture(bool use);
    void useBackgroundColor(bool use);

    void setBackgroundLogo(QString fileName);
    void setBackgroundTexture(QString fileName);
    void setBackgroundColor(const QColor& color);

    bool useLogo;
    bool useTexture;
    bool useColor;

    QPixmap bgLogo;
    QPixmap bgTexture;
    QColor  bgColor;

    void zoomExtentsAllSubWindows();
    void forceRepaint();

public slots:
    void cascade();
    void tile();
protected:
    virtual void mouseDoubleClickEvent(QMouseEvent* e);
    virtual void paintEvent(QPaintEvent* e);
};

/* . */
class Document: public QGraphicsView
{
    Q_OBJECT

public:
    Document(MainWindow* mw, QGraphicsScene* theScene, QWidget* parent);
    ~Document();

    DocumentData data;

    std::unordered_map<int64_t, QGraphicsItem*> hashDeletedObjects;
    QList<int64_t> spareRubberList;
    QPainterPath gridPath;
    QPainterPath originPath;
    QGraphicsItemGroup* previewObjectItemGroup;

    Object* gripBaseObj;
    Object* tempBaseObj;

    QGraphicsScene* gscene;
    QUndoStack* undoStack;

    SelectBox* selectBox;
    QGraphicsItemGroup* pasteObjectItemGroup;
    QList<QGraphicsItem*> rubberRoomList;
    QList<QGraphicsItem*> previewObjectList;

protected:
    void mouseDoubleClickEvent(QMouseEvent* event);
    void mousePressEvent(QMouseEvent* event);
    void mouseMoveEvent(QMouseEvent* event);
    void mouseReleaseEvent(QMouseEvent* event);
    void wheelEvent(QWheelEvent* event);
    void contextMenuEvent(QContextMenuEvent* event);
    void drawBackground(QPainter* painter, const QRectF& rect);
    void draw_rulers(QPainter* painter, const QRectF& rect);
    void drawForeground(QPainter* painter, const QRectF& rect);
    void enterEvent(QEvent* event);
};
class UndoableCommand : public QUndoCommand
{
public:
    UndoableCommand(int type_, QString text, Object* obj, int32_t v,
        QUndoCommand* parent = 0);
    UndoableCommand(int type_, EmbVector delta, QString text, Object* obj,
        int32_t v, QUndoCommand* parent = 0);
    UndoableCommand(int type_, EmbVector pivot, double rotAngle, QString text,
        Object* obj, int32_t v, QUndoCommand* parent = 0);
    UndoableCommand(int type_, QString type, int32_t v, QUndoCommand* parent = 0);
    UndoableCommand(int type_, EmbVector start, EmbVector end, QString text,
        Object* obj, int32_t v, QUndoCommand* parent = 0);

    void undo();
    void redo();
    void rotate(double x, double y, double rot);
    int id() const { return 1234; }
    bool mergeWith(const QUndoCommand* command);
    void mirror();

    UndoData data;
    Object *object;
    QTransform toTransform;
    QTransform fromTransform;
};

class UndoEditor : public QDockWidget
{
    Q_OBJECT

public:
    UndoEditor(QString iconDirectory = "", QWidget* widgetToFocus = 0, QWidget* parent = 0); /*, Qt::WindowFlags flags = 0); */
    ~UndoEditor() {}

    void addStack(QUndoStack* stack);

    bool canUndo() const;
    bool canRedo() const;

    QString undoText() const;
    QString redoText() const;
protected:

public slots:
    void undo();
    void redo();

    void updateCleanIcon(bool opened);

private:
    QWidget* focusWidget;

    QString iconDir;
    int iconSize;

    QUndoGroup* undoGroup;
    QUndoView*  undoView;
};

class Object: public QGraphicsPathItem
{
public:
    ObjectCore *core;

    QGraphicsPathItem path_;
    QPen objPen;
    QPen lwtPen;
    QLineF objLine;
    std::vector<LabelledVector> rubber_points;
    std::vector<StringMap> rubber_texts;

    QPainterPath textPath;
    QPainterPath lineStylePath;
    QPainterPath arrowStylePath;
    QPainterPath normalPath;

    Object(int type_, QRgb rgb, Qt::PenStyle lineType, QGraphicsItem* item = 0);
    ~Object();

    void vulcanize();
    QList<EmbVector> allGripPoints();
    EmbVector mouseSnapPoint(EmbVector mousePoint);
    void gripEdit(EmbVector before, EmbVector after);
    QPainterPath shape() const { return path(); }

    void setObjectRubberPoint(char key[MAX_STRING_LENGTH], EmbVector value);
    void setObjectRubberText(char key[MAX_STRING_LENGTH], char value[MAX_STRING_LENGTH]);

    void drawRubberLine(QLineF rubLine, QPainter* painter = 0, const char* colorFromScene = 0);

    void setObjectSize(double width, double height);

    QPainterPath objectCopyPath() const;
    QPainterPath objectSavePath() const;
    QList<QPainterPath> objectSavePathList() const { return subPathList(); }
    QList<QPainterPath> subPathList() const;

protected:
    void paint(QPainter*, const QStyleOptionGraphicsItem*, QWidget*);
};


class PreviewDialog : public QFileDialog
{
    Q_OBJECT

public:
    PreviewDialog(QWidget* parent = 0, QString caption = "",
        QString directory = "", QString filter = "");
    ~PreviewDialog();

private:
    ImageWidget* imgWidget;
};

class PropertyEditor : public QDockWidget
{
    Q_OBJECT

public:
    PropertyEditor(QString iconDirectory = "", bool pickAddMode = true,
        QWidget* widgetToFocus = 0, QWidget* parent = 0);
        /*, Qt::WindowFlags flags = 0); */
    ~PropertyEditor();

    void togglePickAddMode();

protected:
    bool eventFilter(QObject *obj, QEvent *event);

signals:
    void pickAddModeToggled();

public slots:
    void setSelectedItems(QList<QGraphicsItem*> itemList);
    void updatePickAddModeButton(bool pickAddMode);
};

void currentColorChanged_(int key, uint32_t color);

class Settings_Dialog : public QDialog
{
    Q_OBJECT

public:
    Settings_Dialog(MainWindow* mw, QString showTab = "", QWidget *parent = 0);
    ~Settings_Dialog();

    void color_dialog(QPushButton *button, int key);

private:
    QTabWidget* tabWidget;

    QWidget* createTabGeneral();
    QWidget* createTabFilesPaths();
    QWidget* createTabDisplay();
    QWidget* createTabPrompt();
    QWidget* createTabOpenSave();
    QWidget* createTabPrinting();
    QWidget* createTabSnap();
    QWidget* createTabGridRuler();
    QWidget* createTabOrthoPolar();
    QWidget* createTabQuickSnap();
    QWidget* createTabQuickTrack();
    QWidget* createTabLineWeight();
    QWidget* createTabSelection();

    QDialogButtonBox* buttonBox;

    void addColorsToComboBox(QComboBox* comboBox);
    void chooseColor(int key);
    QGroupBox* create_group_box(QWidget* widget, EmbString label, int data[]);
    QCheckBox* create_checkbox(QGroupBox* groupbox, int key);
    QDoubleSpinBox* create_spinbox(QGroupBox* groupbox, int key);
    QSpinBox* create_int_spinbox(QGroupBox* groupbox, int key);
    QPushButton *choose_color_button(QGroupBox* groupbox, int key);

private slots:

    void checkBoxCustomFilterStateChanged(int);
    void checkBoxGridColorMatchCrossHairStateChanged(int);
    void checkBoxGridLoadFromFileStateChanged(int);
    void checkBoxGridCenterOnOriginStateChanged(int);
    void checkBoxLwtShowLwtStateChanged(int);

    void comboBoxLanguageCurrentIndexChanged(QString);
    void comboBoxIconThemeCurrentIndexChanged(QString);
    void comboBoxIconSizeCurrentIndexChanged(int);
    void chooseGeneralMdiBackgroundLogo();
    void chooseGeneralMdiBackgroundTexture();
    void buttonCustomFilterSelectAllClicked();
    void buttonCustomFilterClearAllClicked();
    void comboBoxGridTypeCurrentIndexChanged(QString);
    void comboBoxRulerMetricCurrentIndexChanged(int);
    void buttonQSnapSelectAllClicked();
    void buttonQSnapClearAllClicked();
    void comboBoxQSnapLocatorColorCurrentIndexChanged(int);
    void comboBoxSelectionCoolGripColorCurrentIndexChanged(int);
    void comboBoxSelectionHotGripColorCurrentIndexChanged(int);

    void acceptChanges();
    void rejectChanges();

signals:
    void buttonCustomFilterSelectAll(bool);
    void buttonCustomFilterClearAll(bool);
    void buttonQSnapSelectAll(bool);
    void buttonQSnapClearAll(bool);
};

class SelectBox: public QRubberBand
{
    Q_OBJECT

public:
    SelectBox(Shape s, QWidget* parent = 0);

public slots:
    void setDirection(int dir);
    void setColors(const QColor& colorL, const QColor& fillL, const QColor& colorR, const QColor& fillR, int newAlpha);

protected:
    void paintEvent(QPaintEvent*);

private:
    QColor leftBrushColor;
    QColor rightBrushColor;
    QColor leftPenColor;
    QColor rightPenColor;
    uint8_t alpha;

    QBrush dirBrush;
    QBrush leftBrush;
    QBrush rightBrush;

    QPen dirPen;
    QPen leftPen;
    QPen rightPen;

    bool boxDir;

    void forceRepaint();
};


class MdiWindow: public QMdiSubWindow
{
    Q_OBJECT

public:
    MdiWindow(const int theIndex, MainWindow* mw, QMdiArea* parent, Qt::WindowFlags wflags);
    ~MdiWindow();

    EmbPattern* pattern;

    QMdiArea* mdiArea;
    QGraphicsScene* gscene;
    int32_t doc_index;

    QString promptHistory;
    QList<QString> promptInputList;
    int promptInputNum;

    QPrinter printer;

    virtual QSize sizeHint() const;
    QString getShortCurrentFile();
    bool loadFile(QString fileName);
    bool saveFile(QString fileName);
signals:
    void sendCloseMdiWin(MdiWindow*);

public slots:
    void closeEvent(QCloseEvent* e);
    void onWindowActivated();

    void updateColorLinetypeLineweight();

    void print();
    void saveBMC();

    void promptHistoryAppended(QString txt);
    void logPromptInput(QString txt);
    void promptInputPrevious();
    void promptInputNext();

private:
    void setCurrentFile(QString fileName);
    QString fileExtension(QString fileName);

    void promptInputPrevNext(bool prev);
};

class ImageWidget : public QWidget
{
    Q_OBJECT

public:
    ImageWidget(QString filename, QWidget* parent = 0);
    ~ImageWidget();

    bool load(QString fileName);
    bool save(QString fileName);

protected:
    void paintEvent(QPaintEvent* event);

private:
    QImage img;
};

/* On Mac, if the user drops a file on the app's Dock icon, or uses Open As,
 * then this is how the app actually opens the file.
 */
class Application : public QApplication
{
    Q_OBJECT
public:
    Application(int argc, char **argv);
    void setMainWin(MainWindow* mainWin);
protected:
    virtual bool event(QEvent *e);
private:
    MainWindow* _mainWin;
};

class CmdPromptInput: public QLineEdit
{
    Q_OBJECT

public:
    CmdPromptInput(QWidget* parent = 0);
    ~CmdPromptInput() {}

protected:
    void contextMenuEvent(QContextMenuEvent *event);
    bool eventFilter(QObject *obj, QEvent *event);

public slots:
    void endCommand();
    void processInput(const QChar& rapidChar = QChar());
    void checkSelection();
    void updateCurrentText(QString txt);
    void checkEditedText(QString txt);
    void checkChangedText(QString txt);
    void checkCursorPosition(int oldpos, int newpos);
private slots:
    void copyClip();
    void pasteClip();
};

class CmdPrompt : public QWidget
{
    Q_OBJECT

public:
    CmdPrompt(QWidget* parent = 0);
    ~CmdPrompt() {}

    void updateStyle(void);

public slots:
    void setCurrentText(QString txt);
    void setPrefix(QString txt);

    void alert(QString txt);

    void blink();

    void saveHistory(QString fileName, bool html);

signals:
    /* For connecting outside of command prompt. */
    void showSettings();

    void historyAppended(QString txt);
};

class MainWindow: public QMainWindow
{
    Q_OBJECT

public:
    MainWindow();
    ~MainWindow();

    void add_toolbar_to_window(Qt::ToolBarArea area, int data[]);

public slots:
    void recentMenuAboutToShow();
    void windowMenuActivated( bool checked/*int id*/ );

    void closeToolBar(QAction*);
    void floatingChangedToolBar(bool);

    void runCommand();

    void colorSelectorIndexChanged(int index);

protected:
    virtual void resizeEvent(QResizeEvent*);
    void closeEvent(QCloseEvent *event);
};

/* . */
void
exit_program(void)
{
    debug_message("exit()");
    if (get_bool(PROMPT_SAVE_HISTORY)) {
        prompt->saveHistory("prompt.log", get_bool(PROMPT_SAVE_HISTORY_AS_HTML));
        /* TODO: get filename from settings */
    }
    qApp->closeAllWindows();
    /* Force the MainWindow destructor to run before exiting.
     * Makes Valgrind "still reachable" happy :)
     */
    _main->deleteLater();
}

/* . */
QString
platformString(void)
{
    char message[MAX_STRING_LENGTH];
    /* TODO: Append QSysInfo to string where applicable. */
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
    sprintf(message, "Platform: %s", qPrintable(os));
    debug_message(message);
    return os;
}

/* . */
void
print_command(void)
{
    debug_message("print_command()");
    MdiWindow* mdiWin = qobject_cast<MdiWindow*>(mdiArea->activeSubWindow());
    if (mdiWin) {
        mdiWin->print();
    }
}

/* . */
void
tipOfTheDay(void)
{
    debug_message("tipOfTheDay()");

    wizardTipOfTheDay = new QWizard(_main);
    wizardTipOfTheDay->setAttribute(Qt::WA_DeleteOnClose);
    wizardTipOfTheDay->setWizardStyle(QWizard::ModernStyle);
    wizardTipOfTheDay->setMinimumSize(550, 400);

    QWizardPage* page = new QWizardPage(wizardTipOfTheDay);

    ImageWidget* imgBanner = new ImageWidget("Did you know", wizardTipOfTheDay);
    // create_pixmap("did_you_know")

    if (get_int(GENERAL_CURRENT_TIP) >= string_array_length(tips)) {
        set_int(GENERAL_CURRENT_TIP, 0);
    }
    labelTipOfTheDay = new QLabel(tips[get_int(GENERAL_CURRENT_TIP)], wizardTipOfTheDay);
    labelTipOfTheDay->setWordWrap(true);

    QCheckBox* checkBoxTipOfTheDay = new QCheckBox(translate("&Show tips on startup"), wizardTipOfTheDay);
    checkBoxTipOfTheDay->setChecked(get_bool(GENERAL_TIP_OF_THE_DAY));
    QObject::connect(checkBoxTipOfTheDay, SIGNAL(stateChanged(int)), _main, SLOT(checkBoxTipOfTheDayStateChanged(int)));

    QVBoxLayout* layout = new QVBoxLayout(wizardTipOfTheDay);
    layout->addWidget(imgBanner);
    layout->addStrut(1);
    layout->addWidget(labelTipOfTheDay);
    layout->addStretch(1);
    layout->addWidget(checkBoxTipOfTheDay);
    page->setLayout(layout);
    wizardTipOfTheDay->addPage(page);

    wizardTipOfTheDay->setWindowTitle("Tip of the Day");

    /* TODO: Add icons to buttons by using wizardTipOfTheDay->setButton(QWizard::CustomButton1, buttonPrevious) */
    /* TODO: Add icons to buttons by using wizardTipOfTheDay->setButton(QWizard::CustomButton1, buttonNext) */
    /* TODO: Add icons to buttons by using wizardTipOfTheDay->setButton(QWizard::CustomButton1, buttonClose) */
    wizardTipOfTheDay->setButtonText(QWizard::CustomButton1, translate("&Previous"));
    wizardTipOfTheDay->setButtonText(QWizard::CustomButton2, translate("&Next"));
    wizardTipOfTheDay->setButtonText(QWizard::CustomButton3, translate("&Close"));
    wizardTipOfTheDay->setOption(QWizard::HaveCustomButton1, true);
    wizardTipOfTheDay->setOption(QWizard::HaveCustomButton2, true);
    wizardTipOfTheDay->setOption(QWizard::HaveCustomButton3, true);
    QObject::connect(wizardTipOfTheDay, SIGNAL(customButtonClicked(int)), _main, SLOT(buttonTipOfTheDayClicked(int)));

    QList<QWizard::WizardButton> listTipOfTheDayButtons;
    listTipOfTheDayButtons << QWizard::Stretch << QWizard::CustomButton1
        << QWizard::CustomButton2 << QWizard::CustomButton3;
    wizardTipOfTheDay->setButtonLayout(listTipOfTheDayButtons);

    wizardTipOfTheDay->exec();
}

/* . */
void
buttonTipOfTheDayClicked(int button)
{
    char message[MAX_STRING_LENGTH];
    sprintf(message, "buttonTipOfTheDayClicked(%d)", button);
    debug_message(message);
    int current = get_int(GENERAL_CURRENT_TIP);
    if (button == QWizard::CustomButton1) {
        if (current > 0) {
            current--;
        }
        else {
            current = string_array_length(tips)-1;
        }
        labelTipOfTheDay->setText(tips[current]);
        set_int(GENERAL_CURRENT_TIP, current);
    }
    else if (button == QWizard::CustomButton2) {
        current++;
        if (current >= string_array_length(tips)) {
            current = 0;
        }
        labelTipOfTheDay->setText(tips[current]);
        set_int(GENERAL_CURRENT_TIP, current);
    }
    else if (button == QWizard::CustomButton3) {
        wizardTipOfTheDay->close();
    }
}

/* . */
void
help(void)
{
    debug_message("help()");

    /* Open the HTML Help in the default browser. */
    QUrl helpURL("file:///" + qApp->applicationDirPath() + "/help/doc-index.html");
    QDesktopServices::openUrl(helpURL);

    /* TODO: This is how to start an external program. Use this elsewhere...
     * QString program = "firefox";
     * QStringList arguments;
     * arguments << "help/commands.html";
     * QProcess *myProcess = new QProcess(this);
     * myProcess->start(program, arguments);
     */
}

/* . */
void
changelog(void)
{
    debug_message("changelog()");

    QUrl changelogURL("help/changelog.html");
    QDesktopServices::openUrl(changelogURL);
}

/* Standard Slots */
void
undo_command(void)
{
    debug_message("undo_command()");
    if (dockUndoEdit->canUndo()) {
        prompt->setPrefix("Undo " + dockUndoEdit->undoText());
        appendHistory("");
        dockUndoEdit->undo();
        prompt->setPrefix(prefix);
    }
    else {
        prompt->alert("Nothing to undo");
        prompt->setPrefix(prefix);
    }
}

/* . */
void
redo_command(void)
{
    debug_message("redo_command()");
    if (dockUndoEdit->canRedo()) {
        prompt->setPrefix("Redo " + dockUndoEdit->redoText());
        appendHistory("");
        dockUndoEdit->redo();
        prompt->setPrefix(prefix);
    }
    else {
        prompt->alert("Nothing to redo");
        prompt->setPrefix(prefix);
    }
}

/* Icons */
void
iconResize(int iconSize)
{
    _main->setIconSize(QSize(iconSize, iconSize));
    layerSelector->setIconSize(QSize(iconSize*4, iconSize));
    colorSelector->setIconSize(QSize(iconSize, iconSize));
    linetypeSelector->setIconSize(QSize(iconSize*4, iconSize));
    lineweightSelector->setIconSize(QSize(iconSize*4, iconSize));
    /* set the minimum combobox width so the text is always readable */
    layerSelector->setMinimumWidth(iconSize*4);
    colorSelector->setMinimumWidth(iconSize*2);
    linetypeSelector->setMinimumWidth(iconSize*4);
    lineweightSelector->setMinimumWidth(iconSize*4);

    /* TODO: low-priority: open app with iconSize set to 128. resize the icons to a smaller size. */

    set_int(GENERAL_ICON_SIZE, iconSize);
}

/* . */
MdiWindow*
activeMdiWindow(void)
{
    debug_message("activeMdiWindow()");
    MdiWindow* mdiWin = qobject_cast<MdiWindow*>(mdiArea->activeSubWindow());
    return mdiWin;
}

/* . */
int32_t
activeDocument(void)
{
    debug_message("activeDocument()");
    MdiWindow* mdiWin = activeMdiWindow();
    if (mdiWin) {
        return mdiWin->doc_index;
    }
    return -1;
}

/* . */
QGraphicsScene*
activeScene(void)
{
    debug_message("activeScene()");
    MdiWindow* mdiWin = qobject_cast<MdiWindow*>(mdiArea->activeSubWindow());
    if (mdiWin) {
        QGraphicsScene* s = mdiWin->gscene;
        return s;
    }
    return 0;
}

/* . */
QUndoStack*
activeUndoStack(void)
{
    debug_message("activeUndoStack()");
    //FIXME
    return NULL;
    int32_t doc_index = activeDocument();
    if (doc_index >= 0) {
        QUndoStack* u = documents[doc_index]->undoStack;
        return u;
    }
    return 0;
}

/* . */
void
whats_this_mode(void)
{
    debug_message("whatsThisContextHelp()");
    QWhatsThis::enterWhatsThisMode();
}

void
window_close_all(void)
{
    mdiArea->closeAllSubWindows();
}

void
window_cascade(void)
{
    mdiArea->cascade();
}

void
window_tile(void)
{
    mdiArea->tile();
}

void
window_next(void)
{
    mdiArea->activateNextSubWindow();
}

void
window_previous(void)
{
    mdiArea->activatePreviousSubWindow();
}

/* . */
void
setUndoCleanIcon(bool opened)
{
    dockUndoEdit->updateCleanIcon(opened);
}

/* . */
void
updatePickAddMode(bool val)
{
    set_bool(SELECTION_MODE_PICKADD, val);
    dockPropEdit->updatePickAddModeButton(val);
}

/* Layer ToolBar */

/* . */
void
layerManager(void)
{
    debug_message("layerManager()");
    todo("Implement layerManager.");
    LayerManager layman(_main, _main);
    layman.exec();
}

void
MainWindow::colorSelectorIndexChanged(int index)
{
    EmbString message;
    sprintf(message, "colorSelectorIndexChanged(%d)", index);
    debug_message(message);

    QComboBox* comboBox = qobject_cast<QComboBox*>(sender());
    uint32_t newColor;
    if (comboBox) {
        bool ok = 0;
        /* TODO: Handle ByLayer and ByBlock and Other... */
        newColor = comboBox->itemData(index).toUInt(&ok);
        if (!ok) {
            QMessageBox::warning(this, translate("Color Selector Conversion Error"),
                translate("<b>An error has occured while changing colors.</b>"));
        }
    }
    else {
        QMessageBox::warning(this, translate("Color Selector Pointer Error"),
            translate("<b>An error has occured while changing colors.</b>"));
    }

    MdiWindow* mdiWin = qobject_cast<MdiWindow*>(mdiArea->activeSubWindow());
    if (mdiWin) {
        currentColorChanged(newColor);
    }
}


/* . */
void
textFontSelectorCurrentFontChanged(const QFont& font)
{
    debug_message("textFontSelectorCurrentFontChanged()");
    setTextFont((char*)qPrintable(font.family()));
}

/* . */
void
textSizeSelectorIndexChanged(int index)
{
    char message[MAX_STRING_LENGTH];
    sprintf(message, "textSizeSelectorIndexChanged(%d)", index);
    debug_message(message);
    /* TODO: check that the toReal() conversion is ok. */
    set_real(TEXT_SIZE, fabs(textSizeSelector->itemData(index).toReal()));
}

/* . */
void
setTextFont(EmbString str)
{
    textFontSelector->setCurrentFont(QFont(str));
    set_str(TEXT_FONT, (char*)qPrintable(str));
}

/* . */
void
setTextSize(double num)
{
    set_real(TEXT_SIZE, fabs(num));
    int index = textSizeSelector->findText("Custom", Qt::MatchContains);
    if (index != -1) {
        textSizeSelector->removeItem(index);
    }
    textSizeSelector->addItem("Custom " + QString().setNum(num, 'f', 2) + " pt", num);
    index = textSizeSelector->findText("Custom", Qt::MatchContains);
    if (index != -1) {
        textSizeSelector->setCurrentIndex(index);
    }
}

/* . */
void
promptHistoryAppended(QString txt)
{
    MdiWindow* mdiWin = activeMdiWindow();
    if (mdiWin) {
        mdiWin->promptHistoryAppended(txt);
    }
}

/* . */
void
logPromptInput(QString txt)
{
    MdiWindow* mdiWin = activeMdiWindow();
    if (mdiWin) {
        mdiWin->logPromptInput(txt);
    }
}

/* . */
void
promptInputPrevious(void)
{
    MdiWindow* mdiWin = activeMdiWindow();
    if (mdiWin) {
        mdiWin->promptInputPrevious();
    }
}

/* . */
void
promptInputNext(void)
{
    MdiWindow* mdiWin = activeMdiWindow();
    if (mdiWin) {
        mdiWin->promptInputNext();
    }
}

void
MainWindow::runCommand()
{
    QAction* act = qobject_cast<QAction*>(sender());
    if (act) {
        EmbString message;
        sprintf(message, "runCommand(%s)", qPrintable(act->objectName()));
        debug_message(message);
        promptInput->endCommand();
        prompt->setCurrentText(act->objectName());
        promptInput->processInput();
    }
}

/* NOTE: translation is the repsonisbility of the caller, because some reports
 * include parts that aren't translated. For example:
 *
 *     char message[MAX_STRING_LENGTH];
 *     sprintf(message, "%s: x > %f", translate("Value of X is too small"), x);
 *     critical_box(translate("Out of Bounds"), message);
 */
void
critical_box(const char *title, const char *text)
{
    QMessageBox::critical(_main, title, text);
}

/* See critical_box comment. */
void
information_box(const char *title, const char *text)
{
    QMessageBox::information(_main, title, text);
}

/* See critical_box comment. */
void
question_box(const char *title, const char *text)
{
    QMessageBox::question(_main, title, text);
}

/* See critical_box comment. */
void
warning_box(const char *title, const char *text)
{
    QMessageBox::warning(_main, title, text);
}

/* . */
uint32_t
rgb(uint8_t r, uint8_t g, uint8_t b)
{
    return qRgb(r, g, b);
}

/* (char *str, double x, double y, double rot, bool fill, int rubberMode). */
ScriptValue
add_textsingle_command(ScriptEnv *context)
{
    int32_t doc_index = activeDocument();
    QGraphicsScene* gscene = activeScene();
    QUndoStack* stack = activeUndoStack();
    char *str = STR(0);
    EmbVector v = unpack_vector(context, 1);
    double rot = REAL(3);
    bool fill = BOOL(4);
    int rubberMode = INT(5);
    if ((doc_index >= 0) && gscene && stack) {
        Object* obj = create_text_single(QString(str), v, getCurrentColor());
        obj_set_text_font(obj->core, get_str(TEXT_FONT));
        obj_set_text_size(obj->core, get_real(TEXT_SIZE));
        obj_set_text_style(obj->core,
            get_bool(TEXT_STYLE_BOLD),
            get_bool(TEXT_STYLE_ITALIC),
            get_bool(TEXT_STYLE_UNDERLINE),
            get_bool(TEXT_STYLE_STRIKEOUT),
            get_bool(TEXT_STYLE_OVERLINE));
        obj_set_text_backward(obj->core, false);
        obj_set_text_upside_down(obj->core, false);
        obj->setRotation(-rot);
        /* TODO: single line text fill. */
        obj_set_rubber_mode(obj->core, rubberMode);
        if (rubberMode) {
            doc_add_to_rubber_room(doc_index, obj);
            gscene->addItem(obj);
            doc_update(doc_index);
        }
        else {
            UndoableCommand* cmd = new UndoableCommand(ACTION_ADD, obj->core->OBJ_NAME, obj, doc_index, 0);
            stack->push(cmd);
        }
        return script_true;
    }
    return script_false;
}

/* (double x1, double y1, double x2, double y2, double rot, int rubberMode). */
ScriptValue
add_line_command(ScriptEnv *context)
{
    int32_t doc_index = activeDocument();
    QGraphicsScene* gscene = activeScene();
    QUndoStack* stack = activeUndoStack();
    if ((doc_index >= 0) && gscene && stack) {
        EmbLine line;
        line.start = unpack_vector(context, 0);
        line.end = unpack_vector(context, 2);
        double rot = REAL(4);
        int rubberMode = INT(5);
        Object* obj = create_line(line, getCurrentColor());
        obj->setRotation(-rot);
        obj_set_rubber_mode(obj->core, rubberMode);
        if (rubberMode) {
            doc_add_to_rubber_room(doc_index, obj);
            gscene->addItem(obj);
            doc_update(doc_index);
        }
        else {
            UndoableCommand* cmd = new UndoableCommand(ACTION_ADD, obj->core->OBJ_NAME, obj, doc_index, 0);
            stack->push(cmd);
        }
        return script_true;
    }
    return script_false;
}

/* (double x, double y, double w, double h, double rot, bool fill, int rubberMode). */
ScriptValue
add_rectangle_command(ScriptEnv *context)
{
    int32_t doc_index = activeDocument();
    QGraphicsScene* gscene = activeScene();
    QUndoStack* stack = activeUndoStack();
    if ((doc_index < 0) && !(gscene && stack)) {
        return script_false;
    }
    EmbRect rect = emb_rect(REAL(0), -REAL(1), REAL(2), -REAL(3));
    double rot = REAL(4);
    bool fill = BOOL(5);
    int rubberMode = INT(6);
    Object* obj = create_rect(rect, getCurrentColor());
    obj->setRotation(-rot);
    obj_set_rubber_mode(obj->core, rubberMode);
    /* TODO: rect fill */
    if (rubberMode) {
        doc_add_to_rubber_room(doc_index, obj);
        gscene->addItem(obj);
        doc_update(doc_index);
    }
    else {
        UndoableCommand* cmd = new UndoableCommand(ACTION_ADD, obj->core->OBJ_NAME, obj, doc_index, 0);
        stack->push(cmd);
    }
    return script_false;
}

/* (double x1, double y1, double x2, double y2, double x3, double y3, int rubberMode). */
ScriptValue
add_arc_command(ScriptEnv *context)
{
    int32_t doc_index = activeDocument();
    QGraphicsScene* scene = activeScene();
    if ((doc_index >= 0) && scene) {
        EmbArc arc;
        arc.start = unpack_vector(context, 0);
        arc.mid = unpack_vector(context, 2);
        arc.end = unpack_vector(context, 4);
        int rubberMode = INT(5);
        Object* arcObj = create_arc(arc, getCurrentColor());
        obj_set_rubber_mode(arcObj->core, rubberMode);
        if (rubberMode) {
            doc_add_to_rubber_room(doc_index, arcObj);
        }
        scene->addItem(arcObj);
        doc_update(doc_index);
        return script_true;
    }
    return script_false;
}

/* (double centerX, double centerY, double radius, bool fill, int rubberMode). */
ScriptValue
add_circle_command(ScriptEnv *context)
{
    int32_t doc_index = activeDocument();
    QGraphicsScene* gscene = activeScene();
    QUndoStack* stack = activeUndoStack();
    if ((doc_index >= 0) && gscene && stack) {
        EmbCircle circle;
        circle.center = unpack_vector(context, 0);
        circle.radius = REAL(2);
        int rubberMode = INT(3);
        Object* obj = create_circle(circle, getCurrentColor());
        obj_set_rubber_mode(obj->core, rubberMode);
        /* TODO: circle fill. */
        if (rubberMode) {
            doc_add_to_rubber_room(doc_index, obj);
            gscene->addItem(obj);
            doc_update(doc_index);
        }
        else {
            UndoableCommand* cmd = new UndoableCommand(ACTION_ADD, obj->core->OBJ_NAME, obj, doc_index, 0);
            stack->push(cmd);
        }
        return script_true;
    }
    return script_false;
}

/* (double centerX, double centerY, double diameter, double length, double rot, bool fill, int rubberMode). */
ScriptValue
add_slot_command(ScriptEnv *context)
{
    /* TODO: Use UndoableCommand for slots */
    int32_t doc_index = activeDocument();
    /*
    Object* slotObj = new Object(centerX, -centerY, diameter, length, getCurrentColor());
    slotObj->setRotation(-rot);
    obj_set_rubber_mode(slotObj->core, rubberMode);
    if (rubberMode) doc_add_to_rubber_room(doc_index, slotObj);
    scene->addItem(slotObj);
    */
    /* TODO: slot fill */
    doc_update(doc_index);
    return script_false;
}

/* (double centerX, double centerY, double width, double height, double rot, bool fill, int rubberMode). */
ScriptValue
add_ellipse_command(ScriptEnv *context)
{
    int32_t doc_index = activeDocument();
    QGraphicsScene* gscene = activeScene();
    QUndoStack* stack = activeUndoStack();
    if ((doc_index >= 0) && gscene && stack) {
        double centerX = REAL(0);
        double centerY = REAL(1);
        double width = REAL(2);
        double height = REAL(3);
        double rot = REAL(4);
        bool fill = BOOL(5);
        int rubberMode = INT(6);
        EmbEllipse ellipse;
        ellipse.center.x = centerX;
        ellipse.center.y = -centerY;
        ellipse.radius.x = width/2.0;
        ellipse.radius.y = height/2.0;
        Object* obj = create_ellipse(ellipse, getCurrentColor());
        obj->setRotation(-rot);
        obj_set_rubber_mode(obj->core, rubberMode);
        /* TODO: ellipse fill */
        if (rubberMode) {
            doc_add_to_rubber_room(doc_index, obj);
            gscene->addItem(obj);
            doc_update(doc_index);
        }
        else {
            UndoableCommand* cmd = new UndoableCommand(ACTION_ADD, obj->core->OBJ_NAME, obj, doc_index, 0);
            stack->push(cmd);
        }
        return script_true;
    }
    return script_false;
}

/* double x, double y. */
ScriptValue
add_point_command(ScriptEnv *context)
{
    int32_t doc_index = activeDocument();
    QUndoStack* stack = activeUndoStack();
    if ((doc_index >= 0) && stack) {
        double x = REAL(0);
        double y = REAL(1);
        EmbPoint point;
        point.position.x = x;
        point.position.y = -y;
        Object* obj = create_point(point, getCurrentColor());
        UndoableCommand* cmd = new UndoableCommand(ACTION_ADD, obj->core->OBJ_NAME, obj, doc_index, 0);
        stack->push(cmd);
        return script_true;
    }
    return script_false;
}

/* NOTE: This native is different than the rest in that the Y+ is down
 * (scripters need not worry about this)
 */
void
add_polygon_command(double startX, double startY, const QPainterPath& p, int rubberMode)
{
    int32_t doc_index = activeDocument();
    QGraphicsScene* gscene = activeScene();
    QUndoStack* stack = activeUndoStack();
    if ((doc_index >= 0) && gscene && stack) {
        EmbPolygon polygon;
        EmbVector start = emb_vector(startX, startY);
        Object* obj = create_polygon(start, p, getCurrentColor());
        obj_set_rubber_mode(obj->core, rubberMode);
        if (rubberMode) {
            doc_add_to_rubber_room(doc_index, obj);
            gscene->addItem(obj);
            doc_update(doc_index);
        }
        else {
            UndoableCommand* cmd = new UndoableCommand(ACTION_ADD, obj->core->OBJ_NAME, obj, doc_index, 0);
            stack->push(cmd);
        }
    }
}

/* NOTE: This native is different than the rest in that the Y+ is down
 * (scripters need not worry about this)
 */
void
add_polyline_command(double startX, double startY, const QPainterPath& p, int rubberMode)
{
    int32_t doc_index = activeDocument();
    QGraphicsScene* gscene = activeScene();
    QUndoStack* stack = activeUndoStack();
    if ((doc_index >= 0) && gscene && stack) {
        EmbPath path;
        EmbVector start = emb_vector(startX, startY);
        Object* obj = create_polygon(start, p, getCurrentColor());
        obj_set_rubber_mode(obj->core, rubberMode);
        if (rubberMode) {
            doc_add_to_rubber_room(doc_index, obj);
            gscene->addItem(obj);
            doc_update(doc_index);
        }
        else {
            UndoableCommand* cmd = new UndoableCommand(ACTION_ADD,
                obj->core->OBJ_NAME, obj, doc_index, 0);
            stack->push(cmd);
        }
    }
}

/* double x1, double y1, double x2, double y2, double rot, int rubberMode */
ScriptValue
add_dimleader_command(ScriptEnv *context)
{
    int32_t doc_index = activeDocument();
    QGraphicsScene* gscene = activeScene();
    QUndoStack* stack = activeUndoStack();
    if ((doc_index >= 0) && gscene && stack) {
        EmbLine line;
        double x1 = REAL(0);
        double y1 = REAL(1);
        double x2 = REAL(2);
        double y2 = REAL(3);
        double rot = REAL(4);
        int rubberMode = INT(5);
        line.start = emb_vector(x1, -y1);
        line.end = emb_vector(x2, -y2);
        Object* obj = create_dim_leader(line, getCurrentColor());
        obj->setRotation(-rot);
        obj_set_rubber_mode(obj->core, rubberMode);
        if (rubberMode) {
            doc_add_to_rubber_room(doc_index, obj);
            gscene->addItem(obj);
            doc_update(doc_index);
        }
        else {
            UndoableCommand* cmd = new UndoableCommand(ACTION_ADD,
                obj->core->OBJ_NAME, obj, doc_index, 0);
            stack->push(cmd);
        }
        return script_true;
    }
    return script_false;
}

/* . */
void
set_CursorShape(char shape[MAX_STRING_LENGTH])
{
    int32_t doc_index = activeDocument();
    Document *doc = documents[doc_index];
    if (doc) {
        if (!strcmp(shape, "arrow")) {
            doc->setCursor(QCursor(Qt::ArrowCursor));
        }
        else if (!strcmp(shape, "uparrow")) {
            doc->setCursor(QCursor(Qt::UpArrowCursor));
        }
        else if (!strcmp(shape, "cross")) {
            doc->setCursor(QCursor(Qt::CrossCursor));
        }
        else if (!strcmp(shape, "wait")) {
            doc->setCursor(QCursor(Qt::WaitCursor));
        }
        else if (!strcmp(shape, "ibeam"))
            doc->setCursor(QCursor(Qt::IBeamCursor));
        else if (!strcmp(shape, "resizevert"))
            doc->setCursor(QCursor(Qt::SizeVerCursor));
        else if (!strcmp(shape, "resizehoriz"))
            doc->setCursor(QCursor(Qt::SizeHorCursor));
        else if (!strcmp(shape, "resizediagleft"))
            doc->setCursor(QCursor(Qt::SizeBDiagCursor));
        else if (!strcmp(shape, "resizediagright"))
            doc->setCursor(QCursor(Qt::SizeFDiagCursor));
        else if (!strcmp(shape, "move"))
            doc->setCursor(QCursor(Qt::SizeAllCursor));
        else if (!strcmp(shape, "blank"))
            doc->setCursor(QCursor(Qt::BlankCursor));
        else if (!strcmp(shape, "splitvert"))
            doc->setCursor(QCursor(Qt::SplitVCursor));
        else if (!strcmp(shape, "splithoriz"))
            doc->setCursor(QCursor(Qt::SplitHCursor));
        else if (!strcmp(shape, "handpointing"))
            doc->setCursor(QCursor(Qt::PointingHandCursor));
        else if (!strcmp(shape, "forbidden"))
            doc->setCursor(QCursor(Qt::ForbiddenCursor));
        else if (!strcmp(shape, "handopen"))
            doc->setCursor(QCursor(Qt::OpenHandCursor));
        else if (!strcmp(shape, "handclosed"))
            doc->setCursor(QCursor(Qt::ClosedHandCursor));
        else if (!strcmp(shape, "whatsthis"))
            doc->setCursor(QCursor(Qt::WhatsThisCursor));
        else if (!strcmp(shape, "busy"))
            doc->setCursor(QCursor(Qt::BusyCursor));
        else if (!strcmp(shape, "dragmove"))
            doc->setCursor(QCursor(Qt::DragMoveCursor));
        else if (!strcmp(shape, "dragcopy"))
            doc->setCursor(QCursor(Qt::DragCopyCursor));
        else if (!strcmp(shape, "draglink"))
            doc->setCursor(QCursor(Qt::DragLinkCursor));
    }
}

/* Compatibility layer for C files */
void
prompt_output(const EmbString txt)
{
    appendHistory((char*)txt);
}

/* . */
void
end_command(void)
{
    int32_t doc_index = activeDocument();
    if (doc_index >= 0) {
        doc_clear_rubber_room(doc_index);
        doc_preview_off(doc_index);
        doc_disable_move_rapid_fire(doc_index);
    }
    promptInput->endCommand();
}

/* Simple Commands (other commands, like circle_command are housed in their
 * own file with their associated functions)
 * ------------------------------------------------------------------------
 */

/* TODO: QTabWidget for about dialog
 */
void
about_dialog(void)
{
    arrow_cursor();
    debug_message("about()");

    QString title = "About Embroidermodder 2";

    QDialog dialog(_main);
    QLabel image_label;
    QPixmap img = create_pixmap("logo_small");
    image_label.setPixmap(img);
    QString body_text = "Embroidermodder 2\n\n";
    /* add version here */
    body_text += translate("http://www.libembroidery.org");
    body_text += "\n\n";
    body_text += translate("Available Platforms: GNU/Linux, Windows, Mac OSX, Raspberry Pi");
    body_text += "\n\n";
    body_text += translate("Embroidery formats by Josh Varga.");
    body_text += "\n\n";
    body_text += translate("User Interface by Jonathan Greig.");
    body_text += "\n\n";
    body_text += translate("Free under the zlib/libpng license.");
#if defined(BUILD_GIT_HASH)
    body_text += "\n\n";
    body_text += translate("Build Hash: ");
    body_text += qPrintable(BUILD_GIT_HASH);
#endif
    QLabel text(body_text);
    text.setWordWrap(true);

    QDialogButtonBox buttonbox(Qt::Horizontal, &dialog);
    QPushButton button(&dialog);
    button.setText("Oh, Yeah!");
    buttonbox.addButton(&button, QDialogButtonBox::AcceptRole);
    buttonbox.setCenterButtons(true);
    _main->connect(&buttonbox, SIGNAL(accepted()), &dialog, SLOT(accept()));

    QVBoxLayout layout;
    layout.setAlignment(Qt::AlignCenter);
    layout.addWidget(&image_label);
    layout.addWidget(&text);
    layout.addWidget(&buttonbox);

    dialog.setWindowTitle(title);
    dialog.setMinimumWidth(image_label.minimumWidth()+30);
    dialog.setMinimumHeight(image_label.minimumHeight()+50);
    dialog.setLayout(&layout);
    dialog.exec();

    restore_cursor();
}


/* PREVIEWON . */
ScriptValue
previewon_command(ScriptEnv *context)
{
    QString cloneStr = QString(STR(0)).toUpper();
    int clone = PREVIEW_CLONE_NULL;
    if (cloneStr == "SELECTED") {
        clone = PREVIEW_CLONE_SELECTED;
    }
    else if (cloneStr == "RUBBER") {
        clone = PREVIEW_CLONE_RUBBER;
    }
    else {
        prompt_output("UNKNOWN_ERROR previewOn(): first argument must be \"SELECTED\" or \"RUBBER\".");
        return script_false;
    }

    QString modeStr  = QString(STR(1)).toUpper();
    int mode = PREVIEW_NULL;
    if (modeStr == "MOVE") {
        mode = PREVIEW_MOVE;
    }
    else if (modeStr == "ROTATE") {
        mode = PREVIEW_ROTATE;
    }
    else if (modeStr == "SCALE") {
        mode = PREVIEW_SCALE;
    }
    else {
        prompt_output("UNKNOWN_ERROR previewOn(): second argument must be \"MOVE\", \"ROTATE\" or \"SCALE\".");
        return script_false;
    }

    int32_t doc_index = activeDocument();
    if (doc_index) {
        doc_preview_on(doc_index, clone, mode, REAL(2), -REAL(3), REAL(4));
    }
    else {
        prompt_output("Preview on requires an active view.");
    }
    return script_null;
}

/* . */
ScriptValue
set_prompt_prefix_command(ScriptEnv* context)
{
    prompt->setPrefix(QString(STR(0)));
    return script_null;
}

/* . */
ScriptValue
print_area_command(ScriptEnv* context)
{
    char message[MAX_STRING_LENGTH];
    sprintf(message, "nativePrintArea(%.2f, %.2f, %.2f, %.2f)", REAL(0), REAL(1), REAL(2), REAL(3));
    debug_message(message);
    /* TODO: Print Setup Stuff
     * nativePrintArea(REAL(0), REAL(1), REAL(2), REAL(3));
     */
    print_command();
    return script_null;
}

/* . */
ScriptValue
perpendicular_distance_command(ScriptEnv* context)
{
    QLineF line(REAL(0), REAL(1), REAL(2), REAL(3));
    QLineF norm = line.normalVector();
    double dx = REAL(4) - REAL(0);
    double dy = REAL(5) - REAL(1);
    norm.translate(dx, dy);
    QPointF iPoint;
    norm.intersects(line, &iPoint);
    double r = QLineF(REAL(4), REAL(5), iPoint.x(), iPoint.y()).length();
    return script_real(r);
}

/*
 * Undo
 */
UndoableCommand::UndoableCommand(int type_, QString text, Object* obj, int32_t doc,
    QUndoCommand* parent) : QUndoCommand(parent)
{
    data.type = type_;
    data.doc = doc;
    object = obj;
    setText(text);
}

/* Move */
UndoableCommand::UndoableCommand(int type_, EmbVector delta, QString text,
    Object* obj, int32_t doc, QUndoCommand* parent) : QUndoCommand(parent)
{
    data.type = type_;
    data.doc = doc;
    object = obj;
    setText(text);
    data.delta = delta;
}

/* Rotate or scale */
UndoableCommand::UndoableCommand(int type_, EmbVector pos, double scaleFactor,
    QString text, Object* obj, int32_t doc, QUndoCommand* parent) : QUndoCommand(parent)
{
    data.type = type_;
    data.doc = doc;
    object = obj;
    setText(text);
    if (data.type == ACTION_SCALE) {
        /* Prevent division by zero and other wacky behavior. */
        if (scaleFactor <= 0.0) {
            data.delta.x = 0.0;
            data.delta.y = 0.0;
            data.factor = 1.0;
            QMessageBox::critical(0,
                QObject::tr("ScaleFactor Error"),
                QObject::tr("Hi there. If you are not a developer, report this as a bug. "
               "If you are a developer, your code needs examined, and possibly your head too."));
        }
        else {
            /* Calculate the offset */
            EmbVector old, new_;
            old.x = object->x();
            old.y = object->y();
            QLineF scaleLine(pos.x, pos.y, old.x, old.y);
            scaleLine.setLength(scaleLine.length() * scaleFactor);
            new_.x = scaleLine.x2();
            new_.y = scaleLine.y2();

            data.delta = emb_vector_subtract(new_, old);
            data.factor = scaleFactor;
        }
    }
    else {
        data.pivot = pos;
        data.angle = scaleFactor;
    }
}

/* Navigation */
UndoableCommand::UndoableCommand(int type_, QString type_name, int32_t doc,
    QUndoCommand* parent) : QUndoCommand(parent)
{
    data.type = type_;
    data.doc = doc;
    string_copy(data.navType, qPrintable(type_name));
    setText(QObject::tr("Navigation"));
    data.done = false;
    // fromTransform = doc_transform(data.doc);
    data.fromCenter = doc_center(data.doc);
}

/* Grip Edit/Mirror */
UndoableCommand::UndoableCommand(int type_, EmbVector beforePoint,
    EmbVector afterPoint, QString text, Object* obj, int32_t doc,
    QUndoCommand* parent) : QUndoCommand(parent)
{
    data.type = type_;
    data.doc = doc;
    object = obj;
    setText(text);
    if (type_ == ACTION_GRIP_EDIT) {
        setText(text);
        data.before = beforePoint;
        data.after = afterPoint;
    }
    if (type_ == ACTION_MIRROR) {
        data.mirrorLine.start = beforePoint;
        data.mirrorLine.end = afterPoint;
    }
}

/* . */
void
UndoableCommand::undo()
{
    switch (data.type) {
    case ACTION_ADD:
        doc_delete_object(data.doc, object);
        break;
    case ACTION_DELETE:
        doc_add_object(data.doc, object);
        break;
    case ACTION_MOVE:
        object->moveBy(-data.delta.x, -data.delta.y);
        break;
    case ACTION_ROTATE:
        rotate(data.pivot.x, data.pivot.y, -data.angle);
        break;
    case ACTION_GRIP_EDIT:
        // FIXME: object->gripEdit(data.after, data.before);
        break;
    case ACTION_SCALE:
        object->setScale(object->scale()*(1/data.factor));
        object->moveBy(-data.delta.x, -data.delta.y);
        break;
    case ACTION_NAV: {
        if (!data.done) {
            toTransform = documents[data.doc]->transform();
            data.toCenter = doc_center(data.doc);
            data.done = true;
        }

        documents[data.doc]->setTransform(fromTransform);
        doc_center_at(data.doc, data.fromCenter);
        break;
    }
    case ACTION_MIRROR:
        mirror();
        break;
    default:
        break;
    }
}

/* . */
void
UndoableCommand::redo()
{
    switch (data.type) {
    case ACTION_ADD:
        doc_add_object(data.doc, object);
        break;
    case ACTION_DELETE:
        doc_delete_object(data.doc, object);
        break;
    case ACTION_MOVE:
        object->moveBy(data.delta.x, data.delta.y);
        break;
    case ACTION_ROTATE:
        rotate(data.pivot.x, data.pivot.y, data.angle);
        break;
    case ACTION_GRIP_EDIT:
        // FIXME: object->gripEdit(data.before, data.after);
        break;
    case ACTION_SCALE:
        object->setScale(object->scale() * data.factor);
        object->moveBy(data.delta.x, data.delta.y);
        break;
    case ACTION_NAV: {
        if (data.done) {
            documents[data.doc]->setTransform(toTransform);
            doc_center_at(data.doc, data.toCenter);
            break;
        }
        DocumentData *d = doc_data(data.doc);
        if (string_equal(data.navType, "ZoomInToPoint")) {
            doc_zoom_to_point(data.doc, d->viewMousePoint, +1);
        }
        else if (string_equal(data.navType, "ZoomOutToPoint")) {
            doc_zoom_to_point(data.doc, d->viewMousePoint, -1);
        }
        else if (string_equal(data.navType, "ZoomExtents")) {
            doc_zoom_extents(data.doc);
        }
        else if (string_equal(data.navType, "ZoomSelected")) {
            doc_zoom_selected(data.doc);
        }
        else if (string_equal(data.navType, "PanStart")) {
            /* Do Nothing. We are just recording the spot where the pan started. */
        }
        else if (string_equal(data.navType, "PanStop")) {
            /* Do Nothing. We are just recording the spot where the pan stopped. */
        }
        else if (string_equal(data.navType, "PanLeft")) {
            doc_pan_left(data.doc);
        }
        else if (string_equal(data.navType, "PanRight")) {
            doc_pan_right(data.doc);
        }
        else if (string_equal(data.navType, "PanUp")) {
            doc_pan_up(data.doc);
        }
        else if (string_equal(data.navType, "PanDown")) {
            doc_pan_down(data.doc);
        }
        toTransform = documents[data.doc]->transform();
        data.toCenter = doc_center(data.doc);
        break;
    }
    case ACTION_MIRROR:
        mirror();
        break;
    default:
        break;
    }
}

/* Rotate */
void
UndoableCommand::rotate(double x, double y, double rot)
{
    double rad = radians(rot);
    double cosRot = cos(rad);
    double sinRot = sin(rad);
    EmbVector rotv;
    EmbVector p = to_emb_vector(object->scenePos());
    p.x -= x;
    p.y -= y;
    rotv.x = p.x*cosRot - p.y*sinRot;
    rotv.y = p.x*sinRot + p.y*cosRot;
    rotv.x += x;
    rotv.y += y;

    object->setPos(rotv.x, rotv.y);
    object->setRotation(object->rotation() + rot);
}

/* . */
bool
UndoableCommand::mergeWith(const QUndoCommand* newest)
{
    if (newest->id() != id()) {
         /* make sure other is also an UndoableNavCommand */
         return false;
    }

    const UndoableCommand* cmd = static_cast<const UndoableCommand*>(newest);
    toTransform = cmd->toTransform;
    data.toCenter = cmd->data.toCenter;

    return true;
}

/* TODO: finish undoable mirror */
void
UndoableCommand::mirror()
{
}

/*
 * Object: attempting to combine the geometry objects into one that changes
 * behaviour depending on its mode.
 */

QRectF
to_qrectf(EmbRect rect)
{
    return QRectF(rect.x, rect.y, rect.w, rect.h);
}

EmbVector
map_from_scene(Object *obj, EmbVector v)
{
    return to_emb_vector(obj->mapFromScene(to_qpointf(v)));
}

/* WARNING: DO NOT enable QGraphicsItem::ItemIsMovable. If it is enabled,
 * WARNING: and the item is double clicked, the scene will erratically move the item while zooming.
 * WARNING: All movement has to be handled explicitly by us, not by the scene.
 */
Object::Object(int type_, QRgb rgb, Qt::PenStyle lineType, QGraphicsItem* item)// : QGraphicsItem(item)
{
    debug_message("BaseObject Constructor()");

    core = (ObjectCore*)malloc(sizeof(ObjectCore));

    if (type_ < 30) {
        string_copy(core->OBJ_NAME, object_names[type_]);
    }
    else {
        string_copy(core->OBJ_NAME, "Unknown");
    }

    objPen.setCapStyle(Qt::RoundCap);
    objPen.setJoinStyle(Qt::RoundJoin);
    lwtPen.setCapStyle(Qt::RoundCap);
    lwtPen.setJoinStyle(Qt::RoundJoin);

    core->objID = QDateTime::currentMSecsSinceEpoch();

    core->gripIndex = -1;
    core->curved = 0;

    setFlag(QGraphicsItem::ItemIsSelectable, true);

    obj_set_color(this, rgb);
    obj_set_line_type(this, lineType);
    obj_set_line_weight(this, 0.35);
    todo("pass in proper lineweight");
    setPen(objPen);

    core->geometry = (EmbGeometry*)malloc(sizeof(EmbGeometry));
    core->geometry->type = type_;
    core->geometry->object.color.r = qRed(rgb);
    core->geometry->object.color.g = qGreen(rgb);
    core->geometry->object.color.b = qBlue(rgb);
    core->geometry->lineType = lineType;
}

/* . */
Object::~Object()
{
    debug_message("ArcObject Destructor()");
    free(core->geometry);
    free(core);
}

/* . */
Object *
create_arc(EmbArc arc, QRgb rgb, QGraphicsItem *item)
{
    debug_message("ArcObject Constructor()");
    Object *obj = new Object(EMB_ARC, rgb, Qt::SolidLine, item);
    obj->core->geometry->object.arc = arc;
    todo("getCurrentLineType");
    obj_calculate_data(obj);
    obj_set_pos(obj->core, arc.start);
    return obj;
}

/* . */
Object *
create_circle(EmbCircle circle, QRgb rgb, QGraphicsItem *item)
{
    debug_message("CircleObject Constructor()");
    Object *obj = new Object(EMB_CIRCLE, rgb, Qt::SolidLine, item);
    todo("getCurrentLineType");
    obj->core->geometry->object.circle = circle;

    /*
    update_path();
    */
    return obj;
}

/* . */
Object *
create_ellipse(EmbEllipse ellipse, QRgb rgb, QGraphicsItem *item)
{
    debug_message("EllipseObject Constructor()");
    todo("getCurrentLineType");
    Object *obj = new Object(EMB_ELLIPSE, rgb, Qt::SolidLine, item);
    obj->core->geometry->object.ellipse = ellipse;

    /*
    setObjectSize(width, height);
    obj_update_path(obj);
    */
    return obj;
}

/* . */
Object *
create_polyline(EmbPath path, const QPainterPath& p, QRgb rgb, QGraphicsItem* parent)
{
    debug_message("PolylineObject Constructor()");
    todo("getCurrentLineType");
    Object *obj = new Object(EMB_POLYLINE, rgb, Qt::SolidLine);
    obj_update_path_r(obj, p);
    /* EmbVector v; obj_set_pos(obj->core, v); */
    return obj;
}

/* . */
Object *
create_path(EmbVector v, const QPainterPath p, QRgb rgb, QGraphicsItem* parent)
{
    debug_message("PathObject Constructor()");
    Object *obj = new Object(EMB_PATH, rgb, Qt::SolidLine);
    todo("getCurrentLineType");
    obj_update_path_r(obj, p);
    obj_set_pos(obj->core, v);
    return obj;
}

/* . */
Object *
create_polygon(EmbVector v, const QPainterPath& p, QRgb rgb, QGraphicsItem* parent)
{
    debug_message("PolygonObject Constructor()");
    Object *obj = new Object(EMB_POLYGON, rgb, Qt::SolidLine);
    todo("getCurrentLineType");
    obj_update_path_r(obj, p);
    obj_set_pos(obj->core, v);
    return obj;
}

/* . */
Object *
create_text_single(QString str, EmbVector v, QRgb rgb, QGraphicsItem* parent)
{
    debug_message("TextSingleObject Constructor()");
    todo("getCurrentLineType");
    Object *obj = new Object(EMB_TEXT_SINGLE, rgb, Qt::SolidLine);
    string_copy(obj->core->textJustify, "Left");
    /* TODO: set the justification properly */

    obj_set_text(obj->core, qPrintable(str));
    obj_set_pos(obj->core, v);
    return obj;
}

/* . */
Object *
create_dim_leader(EmbLine line, QRgb rgb, QGraphicsItem* parent)
{
    debug_message("DimLeaderObject Constructor()");
    todo("getCurrentLineType");
    Object *obj = new Object(EMB_DIM_LEADER, rgb, Qt::SolidLine);

    obj->core->curved = false;
    obj->core->filled = true;
    obj_set_end_point_1(obj->core, line.start);
    obj_set_end_point_2(obj->core, line.end);
    return obj;
}

/* . */
Object *
create_image(EmbRect rect, QRgb rgb, QGraphicsItem* parent)
{
    debug_message("ImageObject Constructor()");
    todo("getCurrentLineType");
    Object *obj = new Object(EMB_IMAGE, rgb, Qt::SolidLine);
    obj_set_rect(obj, rect.x, rect.y, rect.w, rect.h);
    return obj;
}

/* . */
Object *
create_rect(EmbRect rect, QRgb rgb, QGraphicsItem* parent)
{
    debug_message("RectObject Constructor()");
    todo("getCurrentLineType");
    Object *obj = new Object(EMB_RECT, rgb, Qt::SolidLine);
    obj_set_rect(obj, rect.x, rect.y, rect.w, rect.h);
    return obj;
}

/* . */
Object *
create_line(EmbLine line, QRgb rgb, QGraphicsItem* parent)
{
    debug_message("LineObject Constructor()");
    Object *obj = new Object(EMB_LINE, rgb, Qt::SolidLine);
    todo("getCurrentLineType");
    obj_set_end_point_1(obj->core, line.start);
    obj_set_end_point_2(obj->core, line.end);
    return obj;
}

/* . */
Object *
create_point(EmbPoint_ point, QRgb rgb, QGraphicsItem* parent)
{
    Object *obj = new Object(EMB_POINT, rgb, Qt::SolidLine);
    return obj;
}

/* . */
Object *
copy_object(Object* obj)
{
    debug_message("ArcObject Constructor()");
    if (!obj) {
        return NULL;
    }
    Object *copy = new Object(obj->core->geometry->type, obj->core->rgb, Qt::SolidLine);
    switch (obj->core->geometry->type) {
    case EMB_ARC:
        copy->core->geometry->object.arc = obj->core->geometry->object.arc;
        todo("getCurrentLineType");
        copy->setRotation(obj->core->rotation);
        break;
    case EMB_CIRCLE:
        copy->core->geometry->object.circle = obj->core->geometry->object.circle;
        todo("getCurrentLineType");
        copy->setRotation(obj->core->rotation);
        break;
    case EMB_DIM_LEADER:
        copy->core->geometry->object.line = obj->core->geometry->object.line;
        /* init(obj_x1(obj), obj_y1(obj), obj_x2(obj), obj_y2(obj), obj_color_rgb(obj->core), Qt::SolidLine); */
        todo("getCurrentLineType");
        break;
    case EMB_ELLIPSE:
        copy->core->geometry->object.ellipse = obj->core->geometry->object.ellipse;
        /* init(obj->obj_centerX(), obj->obj_centerY(), obj->objectWidth(), obj->objectHeight(), obj_color_rgb(obj), Qt::SolidLine); */
        todo("getCurrentLineType");
        copy->setRotation(obj->core->rotation);
        break;
    case EMB_IMAGE: {
        copy->core->geometry->object.ellipse = obj->core->geometry->object.ellipse;
        /* EmbVector ptl = obj_top_left(obj); */
        /* init(ptl.x, ptl.y, obj->objectWidth(), obj->objectHeight(), obj_color_rgb(obj), Qt::SolidLine); */
        todo("getCurrentLineType");
        copy->setRotation(obj->core->rotation);
        break;
    }
    case EMB_LINE: {
        copy->core->geometry->object.line = obj->core->geometry->object.line;
        /* init(obj->objectX1(), obj->objectY1(), obj->objectX2(), obj->objectY2(), obj_color_rgb(obj), Qt::SolidLine); */
        todo("getCurrentLineType");
        break;
    }
    case EMB_PATH: {
        copy->core->geometry->object.path = obj->core->geometry->object.path;
        /* init(obj->objectX(), obj->objectY(), obj->objectCopyPath(), obj_color_rgb(obj), Qt::SolidLine);
         * obj->setRotation(obj->rotation());
         * obj->setScale(obj->scale());
         */
        todo("getCurrentLineType");
        break;
    }
    case EMB_POINT: {
        copy->core->geometry->object.point = obj->core->geometry->object.point;
        /* init(obj->objectX(), obj->objectY(), obj_color_rgb(obj), Qt::SolidLine);
         */
        todo("getCurrentLineType");
        copy->setRotation(obj->core->rotation);
        break;
    }
    case EMB_POLYGON: {
        copy->core->geometry->object.polygon = obj->core->geometry->object.polygon;
        /* init(obj->objectX(), obj->objectY(), obj->objectCopyPath(), obj_color_rgb(obj), Qt::SolidLine);
         * obj->setRotation(obj->rotation());
         * obj->setScale(obj->scale());
         */
        todo("getCurrentLineType");
        break;
    }
    case EMB_POLYLINE: {
        copy->core->geometry->object.polyline = obj->core->geometry->object.polyline;
        /* init(obj->objectX(), obj->objectY(), obj->objectCopyPath(), obj_color_rgb(obj), Qt::SolidLine);
         */
        todo("getCurrentLineType");
        copy->setRotation(obj->core->rotation);
        copy->setScale(obj->core->scale);
        break;
    }
    case EMB_RECT: {
        obj->core->geometry->object.rect = obj->core->geometry->object.rect;
        EmbVector ptl = obj_top_left(obj->core);
        /* init(ptl.x, ptl.y, obj->objectWidth(), obj->objectHeight(), obj_color_rgb(obj), Qt::SolidLine);
         */
        todo("getCurrentLineType");
        copy->setRotation(obj->core->rotation);
        break;
    }
    case EMB_TEXT_SINGLE: {
        obj_set_text_font(copy->core, obj->core->textFont);
        obj_set_text_size(copy->core, obj->core->textSize);
        copy->setRotation(obj->core->rotation);
        obj_set_text_backward(copy->core, obj->core->textBackward);
        obj_set_text_upside_down(copy->core, obj->core->textUpsideDown);
        obj_set_text_style(copy->core, obj->core->textBold, obj->core->textItalic,
            obj->core->textUnderline, obj->core->textStrikeOut, obj->core->textOverline);
        /* init(obj->text, obj->objectX(), obj->objectY(), obj_color_rgb(obj), Qt::SolidLine);
         */
        todo("getCurrentLineType");
        copy->setScale(obj->core->scale);
        break;
    }
    default:
        break;
    }
    return copy;
}

Qt::PenStyle
obj_line_type(Object* obj)
{
    return obj->objPen.style();
}

double
obj_line_weight(Object* obj)
{
    return obj->lwtPen.widthF();
}

QPainterPath
obj_path(Object* obj)
{
    return obj->path();
}

/* . */
void
obj_draw_rubber_line(Object *obj, const QLineF& rubLine, QPainter* painter, const char* colorFromScene)
{
    if (painter) {
        QGraphicsScene* objScene = obj->scene();
        if (!objScene) {
            return;
        }
        QPen colorPen = obj->objPen;
        colorPen.setColor(QColor(objScene->property(colorFromScene).toUInt()));
        painter->setPen(colorPen);
        painter->drawLine(rubLine);
        painter->setPen(obj->objPen);
    }
}

/* . */
void
Object::setObjectRubberPoint(char key[MAX_STRING_LENGTH], EmbVector value)
{
    LabelledVector s;
    string_copy(s.key, key);
    s.vector = value;
    rubber_points.push_back(s);
}

/* . */
void
Object::setObjectRubberText(char key[MAX_STRING_LENGTH], char value[MAX_STRING_LENGTH])
{
    StringMap s;
    string_copy(s.key, key);
    string_copy(s.value, value);
    rubber_texts.push_back(s);
}

/* . */
void
obj_real_render(Object *obj, QPainter* painter, QPainterPath renderPath)
{
    QColor color1 = obj->objPen.color(); /* lighter color */
    QColor color2 = color1.darker(150); /* darker color */

    /* If we have a dark color, lighten it. */
    int darkness = color1.lightness();
    int threshold = 32;
    todo("This number may need adjusted or maybe just add it to settings.");
    if (darkness < threshold) {
        color2 = color1;
        if (!darkness) {
            color1 = QColor(threshold, threshold, threshold);
        } /* lighter() does not affect pure black */
        else {
            color1 = color2.lighter(100 + threshold);
        }
    }

    int count = renderPath.elementCount();
    for (int i = 0; i < count-1; ++i) {
        QPainterPath::Element elem = renderPath.elementAt(i);
        QPainterPath::Element next = renderPath.elementAt(i+1);

        if (next.isMoveTo()) {
            continue;
        }

        QPainterPath elemPath;
        elemPath.moveTo(elem.x, elem.y);
        elemPath.lineTo(next.x, next.y);

        QPen renderPen(QColor(0,0,0,0));
        renderPen.setWidthF(0);
        painter->setPen(renderPen);
        QPainterPathStroker stroker;
        stroker.setWidth(0.35);
        stroker.setCapStyle(Qt::RoundCap);
        stroker.setJoinStyle(Qt::RoundJoin);
        QPainterPath realPath = stroker.createStroke(elemPath);
        painter->drawPath(realPath);

        QLinearGradient grad(elemPath.pointAtPercent(0.5), elemPath.pointAtPercent(0.0));
        grad.setColorAt(0, color1);
        grad.setColorAt(1, color2);
        grad.setSpread(QGradient::ReflectSpread);

        painter->fillPath(realPath, QBrush(grad));
    }
}

void
Object::setObjectSize(double width, double height)
{
    QRectF elRect = to_qrectf(obj_rect(core));
    elRect.setWidth(width);
    elRect.setHeight(height);
    elRect.moveCenter(QPointF(0, 0));
    // FIXME: obj->setRect(elRect);
}


void
obj_draw_rubber_grip(Object *obj, QPainter *painter)
{
    QPointF point = to_qpointf(obj_map_rubber(obj, ""))
        - to_qpointf(obj_map_rubber(obj, "GRIP_POINT"));
    painter->drawLine(obj_line(obj).translated(point));
}

/* . */
void
obj_update_rubber_grip(Object *obj, QPainter *painter)
{
    if (!painter) {
        return;
    }
    EmbVector gripPoint = obj_rubber_point(obj, "GRIP_POINT");
    switch (obj->core->geometry->type) {
    case EMB_ARC: {
        todo("rubber grip arc");
        break;
    }
    case EMB_ELLIPSE: {
        todo("rubber grip ellipse");
        break;
    }
    case EMB_IMAGE: {
        todo("rubber grip image");
        break;
    }
    case EMB_PATH: {
        todo("rubber grip path");
        break;
    }
    case EMB_LINE: {
        QPointF p = to_qpointf(obj_map_rubber(obj, ""));
        if (emb_approx(gripPoint, obj_end_point_1(obj->core))) {
            painter->drawLine(obj_line(obj).p2(), p);
        }
        else if (emb_approx(gripPoint, obj_end_point_2(obj->core))) {
            painter->drawLine(obj_line(obj).p1(), p);
        }
        else if (emb_approx(gripPoint, obj_mid_point(obj->core))) {
            QPointF point = p - to_qpointf(obj_map_rubber(obj, "GRIP_POINT"));
            QLineF line = obj_line(obj).translated(point);
            painter->drawLine(line);
        }

        obj_draw_rubber_grip(obj, painter);
        break;
    }
    case EMB_CIRCLE: {
        if (emb_approx(gripPoint, obj_center(obj->core))) {
            QPointF point = to_qpointf(obj_map_rubber(obj, ""))
                - to_qpointf(obj_map_rubber(obj, "GRIP_POINT"));
            QRectF r = to_qrectf(obj_rect(obj->core));
            painter->drawEllipse(r.translated(point));
        }
        else {
            double gripRadius = emb_vector_distance(obj_center(obj->core), obj_rubber_point(obj, ""));
            painter->drawEllipse(QPointF(), gripRadius, gripRadius);
        }

        obj_draw_rubber_grip(obj, painter);
        break;
    }
    case EMB_DIM_LEADER: {
        QPointF p = to_qpointf(obj_map_rubber(obj, ""));
        if (emb_approx(gripPoint, obj_end_point_1(obj->core))) {
            painter->drawLine(obj_line(obj).p2(), p);
        }
        else if (emb_approx(gripPoint, obj_end_point_2(obj->core))) {
            painter->drawLine(obj_line(obj).p1(), p);
        }
        else if (emb_approx(gripPoint, obj_mid_point(obj->core))) {
            obj_draw_rubber_grip(obj, painter);
        }
        break;
    }
    case EMB_POINT: {
        if (emb_approx(gripPoint, obj_pos(obj->core))) {
            obj_draw_rubber_grip(obj, painter);
        }
        break;
    }
    case EMB_POLYGON: {
        int elemCount = obj->normalPath.elementCount();
        EmbVector gripPoint = obj_rubber_point(obj, "GRIP_POINT");
        if (obj->core->gripIndex == -1) {
            obj->core->gripIndex = obj_find_index(obj, gripPoint);
                if (obj->core->gripIndex == -1) {
                    return;
                }
            }

            int m = 0;
            int n = 0;

            if (!obj->core->gripIndex) {
                m = elemCount - 1;
                n = 1;
            }
            else if (obj->core->gripIndex == elemCount-1) {
                m = elemCount - 2;
                n = 0;
            }
            else {
                m = obj->core->gripIndex - 1;
                n = obj->core->gripIndex + 1;
            }

            QPointF p = to_qpointf(obj_map_rubber(obj, ""));
            QPainterPath::Element em = obj->normalPath.elementAt(m);
            QPainterPath::Element en = obj->normalPath.elementAt(n);
            painter->drawLine(QPointF(em.x, em.y), p);
            painter->drawLine(QPointF(en.x, en.y), p);

            obj_draw_rubber_grip(obj, painter);
            break;
        }
        case EMB_POLYLINE: {
            int elemCount = obj->normalPath.elementCount();
            EmbVector gripPoint = obj_rubber_point(obj, "GRIP_POINT");
            if (obj->core->gripIndex == -1) {
                obj->core->gripIndex = obj_find_index(obj, gripPoint);
            }
            if (obj->core->gripIndex == -1) {
                return;
            }

            QPointF p = to_qpointf(obj_map_rubber(obj, ""));
            if (!obj->core->gripIndex) {
                /* First */
                QPainterPath::Element ef = obj->normalPath.elementAt(1);
                painter->drawLine(QPointF(ef.x, ef.y), p);
            }
            else if (obj->core->gripIndex == elemCount-1) {
                /* Last */
                QPainterPath::Element el = obj->normalPath.elementAt(obj->core->gripIndex-1);
                painter->drawLine(QPointF(el.x, el.y), p);
            }
            else {
                /* Middle */
                QPainterPath::Element em = obj->normalPath.elementAt(obj->core->gripIndex-1);
                QPainterPath::Element en = obj->normalPath.elementAt(obj->core->gripIndex+1);
                painter->drawLine(QPointF(em.x, em.y), p);
                painter->drawLine(QPointF(en.x, en.y), p);
            }

        obj_draw_rubber_grip(obj, painter);
        break;
    }
    case EMB_RECT: {
        todo("Make this work with rotation & scaling.");
        EmbVector after = obj_rubber_point(obj, "");
        EmbVector delta = emb_vector_subtract(after, gripPoint);
        EmbVector tl = obj_top_left(obj->core);
        double w = emb_width(obj->core->geometry);
        double h = emb_height(obj->core->geometry);
        if (emb_approx(gripPoint, obj_top_left(obj->core))) {
            painter->drawPolygon(obj->mapFromScene(QRectF(
                after.x,
                after.y,
                w - delta.x,
                h - delta.y)));
        }
        else if (emb_approx(gripPoint, obj_top_right(obj->core))) {
            painter->drawPolygon(obj->mapFromScene(QRectF(
                tl.x,
                tl.y + delta.y,
                w + delta.x,
                h - delta.y)));
        }
        else if (emb_approx(gripPoint, obj_bottom_left(obj->core))) {
            painter->drawPolygon(obj->mapFromScene(QRectF(
                tl.x + delta.x,
                tl.y,
                w - delta.x,
                h + delta.y)));
        }
        else if (emb_approx(gripPoint, obj_bottom_right(obj->core))) {
            painter->drawPolygon(obj->mapFromScene(QRectF(
                tl.x,
                tl.y,
                w + delta.x,
                h + delta.y)));
        }

        obj_draw_rubber_grip(obj, painter);
        break;
    }
    case EMB_TEXT_SINGLE: {
        if (emb_approx(gripPoint, obj_pos(obj->core))) {
           // painter->drawPath(obj_path(obj).translated(obj->mapFromScene(obj_rubber_point(obj, ""))- obj_map_rubber(obj, "GRIP_POINT"));
        }

        obj_draw_rubber_grip(obj, painter);
        break;
    }
    default:
        break;
    }
}

/* FIXME: */
QList<QPainterPath>
Object::subPathList() const {
    QList<QPainterPath> p;
    return p;
}

/* TODO: << "Aligned" << "Fit" */
QStringList objectTextJustifyList = {
    "Left",
    "Center",
    "Right",
    "Middle",
    "Top Left",
    "Top Center",
    "Top Right",
    "Middle Left",
    "Middle Center",
    "Middle Right",
    "Bottom Left",
    "Bottom Center",
    "Bottom Right"
};

/* . */
void
obj_set_text(ObjectCore* obj, const char *str)
{
    string_copy(obj->text, str);
    QPainterPath textPath;
    QFont font;
    font.setFamily(obj->textFont);
    font.setPointSizeF(obj->textSize);
    font.setBold(obj->textBold);
    font.setItalic(obj->textItalic);
    font.setUnderline(obj->textUnderline);
    font.setStrikeOut(obj->textStrikeOut);
    font.setOverline(obj->textOverline);
    textPath.addText(0, 0, font, str);

    /* Translate the path based on the justification. */
    QRectF jRect = textPath.boundingRect();
    if (string_equal(obj->textJustify, "Left")) {
        textPath.translate(-jRect.left(), 0);
    }
    else if (string_equal(obj->textJustify, "Center")) {
        textPath.translate(-jRect.center().x(), 0);
    }
    else if (string_equal(obj->textJustify, "Right")) {
        textPath.translate(-jRect.right(), 0);
    }
    else if (string_equal(obj->textJustify, "Aligned")) {
        todo("TextSingleObject Aligned Justification.");
    }
    else if (string_equal(obj->textJustify, "Middle")) {
        textPath.translate(-jRect.center());
    }
    else if (string_equal(obj->textJustify, "Fit")) {
        todo("TextSingleObject Fit Justification.");
    }
    else if (string_equal(obj->textJustify, "Top Left")) {
        textPath.translate(-jRect.topLeft());
    }
    else if (string_equal(obj->textJustify, "Top Center")) {
        textPath.translate(-jRect.center().x(), -jRect.top());
    }
    else if (string_equal(obj->textJustify, "Top Right")) {
        textPath.translate(-jRect.topRight());
    }
    else if (string_equal(obj->textJustify, "Middle Left")) {
        textPath.translate(-jRect.left(), -jRect.top()/2.0);
    }
    else if (string_equal(obj->textJustify, "Middle Center")) {
        textPath.translate(-jRect.center().x(), -jRect.top()/2.0);
    }
    else if (string_equal(obj->textJustify, "Middle Right")) {
        textPath.translate(-jRect.right(), -jRect.top()/2.0);
    }
    else if (string_equal(obj->textJustify, "Bottom Left")) {
        textPath.translate(-jRect.bottomLeft());
    }
    else if (string_equal(obj->textJustify, "Bottom Center")) {
        textPath.translate(-jRect.center().x(), -jRect.bottom());
    }
    else if (string_equal(obj->textJustify, "Bottom Right")) {
        textPath.translate(-jRect.bottomRight());
    }

    /* Backward or Upside Down. */
    if (obj->textBackward || obj->textUpsideDown) {
        double horiz = 1.0;
        double vert = 1.0;
        if (obj->textBackward) {
            horiz = -1.0;
        }
        if (obj->textUpsideDown) {
            vert = -1.0;
        }

        QPainterPath flippedPath;

        QPainterPath::Element element;
        QPainterPath::Element P2;
        QPainterPath::Element P3;
        QPainterPath::Element P4;
        for (int i = 0; i < textPath.elementCount(); ++i) {
            element = textPath.elementAt(i);
            if (element.isMoveTo()) {
                flippedPath.moveTo(horiz * element.x, vert * element.y);
            }
            else if (element.isLineTo()) {
                flippedPath.lineTo(horiz * element.x, vert * element.y);
            }
            else if (element.isCurveTo()) {
                                              /* start point P1 is not needed. */
                P2 = textPath.elementAt(i);   /* control point. */
                P3 = textPath.elementAt(i+1); /* control point. */
                P4 = textPath.elementAt(i+2); /* end point. */

                flippedPath.cubicTo(horiz * P2.x, vert * P2.y,
                                    horiz * P3.x, vert * P3.y,
                                    horiz * P4.x, vert * P4.y);
            }
        }
        // FIXME: obj->textPath = flippedPath;
    }
    else {
        // FIXME: obj->textPath = textPath;
    }

    /* Add the grip point to the shape path. */
    /* FIXME:
    QPainterPath gripPath = obj->textPath;
    gripPath.connectPath(obj->textPath);
    gripPath.addRect(-0.00000001, -0.00000001, 0.00000002, 0.00000002);
    obj_set_path(obj, gripPath);
    */
}

/* . */
void
obj_update_path(Object *obj)
{
    QRectF r = to_qrectf(obj_rect(obj->core));
    switch (obj->core->geometry->type) {
    case EMB_ARC: {
        double startAngle = emb_start_angle(obj->core->geometry) + obj->rotation();
        double spanAngle = emb_included_angle(obj->core->geometry);

        if (emb_clockwise(obj->core->geometry)) {
            spanAngle = -spanAngle;
        }

        QPainterPath path;
        path.arcMoveTo(r, startAngle);
        path.arcTo(r, startAngle, spanAngle);
        /* Reverse the path so that the inside area isn't considered part of the arc. */
        path.arcTo(r, startAngle+spanAngle, -spanAngle);
        obj_set_path(obj, path);
        break;
    }
    case EMB_CIRCLE: {
        QPainterPath path;
        /* Add the center point. */
        path.addRect(-0.00000001, -0.00000001, 0.00000002, 0.00000002);
        /* Add the circle */
        path.arcMoveTo(r, 0);
        path.arcTo(r, 0, 360);
        /* Reverse the path so that the inside area isn't considered part of the circle. */
        path.arcTo(r, 0, -360);
        obj_set_path(obj, path);
        break;
    }
    case EMB_ELLIPSE: {
        QPainterPath path;
        path.arcMoveTo(r, 0);
        path.arcTo(r, 0, 360);
        /* Reverse the path so that the inside area isn't considered part of the ellipse. */
        path.arcTo(r, 0, -360);
        obj_set_path(obj, path);
        break;
    }
    case EMB_RECT:
    case EMB_IMAGE: {
        QPainterPath path;
        path.moveTo(r.bottomLeft());
        path.lineTo(r.bottomRight());
        path.lineTo(r.topRight());
        path.lineTo(r.topLeft());
        path.lineTo(r.bottomLeft());
        /* NOTE: Reverse the path so that the inside area isn't considered part of the rectangle. */
        path.lineTo(r.topLeft());
        path.lineTo(r.topRight());
        path.lineTo(r.bottomRight());
        path.moveTo(r.bottomLeft());
        obj_set_path(obj, path);
        break;
    }
    default:
        break;
    }
}

/* . */
void
obj_calculate_data(Object *obj)
{
    EmbVector center = emb_arc_center(*(obj->core->geometry));

    double radius = emb_vector_distance(center, obj->core->geometry->object.arc.mid);
    obj_update_arc_rect(obj, radius);
    obj_update_path(obj);
    obj->setRotation(0);
    obj->setScale(1);
}

/* . */
void
obj_update_arc_rect(Object *obj, double radius)
{
    QRectF arcRect;
    arcRect.setWidth(radius*2.0);
    arcRect.setHeight(radius*2.0);
    arcRect.moveCenter(QPointF(0, 0));
    obj_set_rect(obj, arcRect);
}

/* . */
void
obj_set_line_weight(Object *obj, double lineWeight)
{
    obj->objPen.setWidthF(0); /* NOTE: The objPen will always be cosmetic. */

    if (lineWeight < 0) {
        if (lineWeight == OBJ_LWT_BYLAYER) {
            obj->lwtPen.setWidthF(0.35);
            todo("getLayerLineWeight");
        }
        else if (lineWeight == OBJ_LWT_BYBLOCK) {
            obj->lwtPen.setWidthF(0.35);
            todo("getBlockLineWeight");
        }
        else {
            char msg[MAX_STRING_LENGTH];
            sprintf(msg, "Lineweight: %f", lineWeight);
            warning_box(translate("Error - Negative Lineweight"), msg);
            debug_message("Lineweight cannot be negative! Inverting sign.");
            obj->lwtPen.setWidthF(-lineWeight);
        }
    }
    else {
        obj->lwtPen.setWidthF(lineWeight);
    }
}

/* . */
void
obj_update_path_r(Object *obj, QPainterPath p)
{
    switch (obj->core->geometry->type) {
    case EMB_POLYGON: {
        obj->normalPath = p;
        QPainterPath closedPath = obj->normalPath;
        closedPath.closeSubpath();
        QPainterPath reversePath = closedPath.toReversed();
        reversePath.connectPath(closedPath);
        obj_set_path(obj, reversePath);
        break;
    }
    case EMB_PATH:
    case EMB_POLYLINE: {
        obj->normalPath = p;
        QPainterPath reversePath = obj->normalPath.toReversed();
        reversePath.connectPath(obj->normalPath);
        obj_set_path(obj, reversePath);
        break;
    }
    default:
        break;
    }
}

/* . */
void
obj_set_color(Object *obj, const QColor& color)
{
    obj->objPen.setColor(color);
    obj->lwtPen.setColor(color);
}

/* . */
void
obj_set_color_rgb(Object *obj, QRgb rgb)
{
    obj->objPen.setColor(QColor(rgb));
    obj->lwtPen.setColor(QColor(rgb));
}

/* . */
void
obj_set_line_type(Object *obj, Qt::PenStyle lineType)
{
    obj->objPen.setStyle(lineType);
    obj->lwtPen.setStyle(lineType);
}

/* . */
EmbVector
obj_rubber_point(Object *obj, QString key)
{
    for (int i=0; i<(int)obj->rubber_points.size(); i++) {
        if (string_equal(obj->rubber_points[i].key, qPrintable(key))) {
            return obj->rubber_points[i].vector;
        }
    }

    /* TODO: object's scene rather than current. */
    int doc = activeDocument();
    DocumentData *data = doc_data(doc);
    return data->sceneQSnapPoint;
}

/* . */
QString
obj_rubber_text(Object *obj, QString key)
{
    for (int i=0; i<(int)obj->rubber_texts.size(); i++) {
        if (string_equal(obj->rubber_texts[i].key, qPrintable(key))) {
            return obj->rubber_texts[i].value;
        }
    }
    return "";
}

/* . */
QRectF
obj_bounding_rect(ObjectCore *obj)
{
    /* If gripped, force this object to be drawn even if it is offscreen. */
    if (obj->rubber_mode == RUBBER_GRIP) {
        return documents[obj->doc]->scene()->sceneRect();
    }
    //return documents[obj->doc]->normalPath.boundingRect();
    return QRectF(0.0, 0.0, 1.0, 1.0);
}


/* . */
EmbRect
obj_rect(ObjectCore *obj)
{
    QRectF r = obj_bounding_rect(obj);
    EmbRect rect;
    rect.x = r.x();
    rect.y = r.y();
    rect.w = r.width();
    rect.h = r.height();
    return rect;
}

/* . */
void
obj_set_rect(Object *obj, QRectF r)
{
    QPainterPath p;
    p.addRect(r);
    obj->setPath(p);
}

/* . */
void
obj_set_rect(Object *obj, double x, double y, double w, double h)
{
    // obj->setPos(x, y); ?
    QPainterPath p;
    p.addRect(x,y,w,h);
    obj->setPath(p);
}

/* . */
QLineF
obj_line(Object *obj)
{
    return obj->objLine;
}

/* . */
void
obj_set_line(Object *obj, QLineF li)
{
    QPainterPath p;
    p.moveTo(li.p1());
    p.lineTo(li.p2());
    obj->setPath(p);
    obj->objLine = li;
}

/* . */
void
obj_set_line(Object *obj, double x1, double y1, double x2, double y2)
{
    QPainterPath p;
    p.moveTo(x1, y1);
    p.lineTo(x2, y2);
    obj->setPath(p);
    obj->objLine.setLine(x1, y1, x2, y2);
}

/* . */
QPainterPath
obj_shape(Object *obj)
{
    return obj->path();
}

/* . */
void
obj_set_path(Object *obj, QPainterPath p)
{
    obj->setPath(p);
}

/* . */
void
obj_set_rubber_point(Object *obj, std::string key, const EmbVector& point)
{
    LabelledVector s;
    string_copy(s.key, key.c_str());
    s.vector = point;
    obj->rubber_points.push_back(s);
}

/* . */
void
obj_set_rubber_text(Object *obj, std::string key, std::string txt)
{
    StringMap s;
    string_copy(s.key, key.c_str());
    string_copy(s.value, txt.c_str());
    obj->rubber_texts.push_back(s);
}

/* . */
QList<QPainterPath>
obj_save_path_list(Object *obj)
{
    return obj->subPathList();
}

/* . */
int
obj_find_index(Object *obj, EmbVector point)
{
    int elemCount = obj->normalPath.elementCount();
    /* NOTE: Points here are in item coordinates */
    EmbVector itemPoint = map_from_scene(obj, point);
    for (int i = 0; i < elemCount; i++) {
        QPainterPath::Element e = obj->normalPath.elementAt(i);
        EmbVector elemPoint;
        elemPoint.x = e.x;
        elemPoint.y = e.y;
        if (emb_approx(itemPoint, elemPoint)) {
            return i;
        }
    }
    return -1;
}

/* Map rubber point from scene. */
EmbVector
obj_map_rubber(Object *obj, const char *key)
{
    EmbVector point = obj_rubber_point(obj, key);
    return map_from_scene(obj, point);
}

/* . */
void
obj_update_rubber(Object *obj, QPainter* painter)
{
    todo("Arc,Path Rubber Modes");
    switch (obj->core->rubber_mode) {
    case RUBBER_CIRCLE_1P_RAD: {
        EmbVector sceneCenterPoint = obj_rubber_point(obj, "CIRCLE_CENTER");
        EmbVector sceneQSnapPoint = obj_rubber_point(obj, "CIRCLE_RADIUS");
        EmbVector itemCenterPoint = obj_map_rubber(obj, "CIRCLE_CENTER");
        EmbVector itemQSnapPoint = obj_map_rubber(obj, "CIRCLE_RADIUS");
        QLineF itemLine(to_qpointf(itemCenterPoint), to_qpointf(itemQSnapPoint));
        obj_set_center(obj->core, sceneCenterPoint);
        QLineF sceneLine(to_qpointf(sceneCenterPoint), to_qpointf(sceneQSnapPoint));
        double radius = sceneLine.length();
        emb_set_radius(obj->core->geometry, radius);
        if (painter) {
            obj_draw_rubber_line(obj, itemLine, painter, "VIEW_COLOR_CROSSHAIR");
        }
        obj_update_path(obj);
        break;
    }
    case RUBBER_CIRCLE_1P_DIA: {
        EmbVector sceneCenterPoint = obj_rubber_point(obj, "CIRCLE_CENTER");
        EmbVector sceneQSnapPoint = obj_rubber_point(obj, "CIRCLE_DIAMETER");
        EmbVector itemCenterPoint = obj_map_rubber(obj, "CIRCLE_CENTER");
        EmbVector itemQSnapPoint = obj_map_rubber(obj, "CIRCLE_DIAMETER");
        QLineF itemLine(to_qpointf(itemCenterPoint), to_qpointf(itemQSnapPoint));
        obj_set_center(obj->core, sceneCenterPoint);
        QLineF sceneLine(to_qpointf(sceneCenterPoint), to_qpointf(sceneQSnapPoint));
        double diameter = sceneLine.length();
        emb_set_diameter(obj->core->geometry, diameter);
        if (painter) {
            obj_draw_rubber_line(obj, itemLine, painter, "VIEW_COLOR_CROSSHAIR");
        }
        obj_update_path(obj);
        break;
    }
    case RUBBER_CIRCLE_2P: {
        EmbVector sceneTan1Point = obj_rubber_point(obj, "CIRCLE_TAN1");
        EmbVector sceneQSnapPoint = obj_rubber_point(obj, "CIRCLE_TAN2");
        QLineF sceneLine(to_qpointf(sceneTan1Point), to_qpointf(sceneQSnapPoint));
        obj_set_center(obj->core, to_emb_vector(sceneLine.pointAt(0.5)));
        double diameter = sceneLine.length();
        emb_set_diameter(obj->core->geometry, diameter);
        obj_update_path(obj);
        break;
    }
    case RUBBER_CIRCLE_3P: {
        EmbGeometry g;
        g.object.arc.start = obj_rubber_point(obj, "CIRCLE_TAN1");
        g.object.arc.mid = obj_rubber_point(obj, "CIRCLE_TAN2");
        g.object.arc.end = obj_rubber_point(obj, "CIRCLE_TAN3");
        g.type = EMB_ARC;
        EmbVector center = emb_arc_center(g);
        obj_set_center(obj->core, center);
        double radius = emb_vector_distance(center, g.object.arc.end);
        emb_set_radius(obj->core->geometry, radius);
        obj_update_path(obj);
        break;
    }
    case RUBBER_DIMLEADER_LINE: {
        EmbVector sceneStartPoint = obj_rubber_point(obj, "DIMLEADER_LINE_START");
        EmbVector sceneQSnapPoint = obj_rubber_point(obj, "DIMLEADER_LINE_END");

        obj_set_end_point_1(obj->core, sceneStartPoint);
        obj_set_end_point_2(obj->core, sceneQSnapPoint);
        break;
    }
    case RUBBER_ELLIPSE_LINE: {
        EmbVector itemLinePoint1  = obj_map_rubber(obj, "ELLIPSE_LINE_POINT1");
        EmbVector itemLinePoint2  = obj_map_rubber(obj, "ELLIPSE_LINE_POINT2");
        QLineF itemLine(to_qpointf(itemLinePoint1), to_qpointf(itemLinePoint2));
        if (painter) {
            obj_draw_rubber_line(obj, itemLine, painter, "VIEW_COLOR_CROSSHAIR");
        }
        obj_update_path(obj);
        break;
    }
    case RUBBER_ELLIPSE_MAJDIA_MINRAD: {
        EmbVector sceneAxis1Point1 = obj_rubber_point(obj, "ELLIPSE_AXIS1_POINT1");
        EmbVector sceneAxis1Point2 = obj_rubber_point(obj, "ELLIPSE_AXIS1_POINT2");
        EmbVector sceneCenterPoint = obj_rubber_point(obj, "ELLIPSE_CENTER");
        EmbVector sceneAxis2Point2 = obj_rubber_point(obj, "ELLIPSE_AXIS2_POINT2");
        double ellipseWidth = obj_rubber_point(obj, "ELLIPSE_WIDTH").x;
        double ellipseRot = obj_rubber_point(obj, "ELLIPSE_ROT").x;

        todo("incorporate perpendicularDistance() into libembroidery.");
        QLineF line(to_qpointf(sceneAxis1Point1), to_qpointf(sceneAxis1Point2));
        QLineF norm = line.normalVector();
        EmbVector delta = emb_vector_subtract(sceneAxis2Point2, sceneAxis1Point1);
        norm.translate(delta.x, delta.y);
        QPointF iPoint;
        norm.intersects(line, &iPoint);
        double ellipseHeight = emb_vector_distance(sceneAxis2Point2, to_emb_vector(iPoint))*2.0;

        obj_set_center(obj->core, sceneCenterPoint);
        // FIXME: obj->setSize(ellipseWidth, ellipseHeight);
        obj->setRotation(-ellipseRot);

        EmbVector itemCenterPoint = obj_map_rubber(obj, "ELLIPSE_CENTER");
        EmbVector itemAxis2Point2 = obj_map_rubber(obj, "ELLIPSE_AXIS2_POINT2");
        QLineF itemLine(to_qpointf(itemCenterPoint), to_qpointf(itemAxis2Point2));
        if (painter) {
            obj_draw_rubber_line(obj, itemLine, painter, "VIEW_COLOR_CROSSHAIR");
        }
        obj_update_path(obj);
        break;
    }
    case RUBBER_ELLIPSE_MAJRAD_MINRAD: {
        EmbVector sceneAxis1Point2 = obj_rubber_point(obj, "ELLIPSE_AXIS1_POINT2");
        EmbVector sceneCenterPoint = obj_rubber_point(obj, "ELLIPSE_CENTER");
        EmbVector sceneAxis2Point2 = obj_rubber_point(obj, "ELLIPSE_AXIS2_POINT2");
        double ellipseWidth = obj_rubber_point(obj, "ELLIPSE_WIDTH").x;
        double ellipseRot = obj_rubber_point(obj, "ELLIPSE_ROT").x;

        todo("incorporate perpendicularDistance() into libcgeometry.");
        QLineF line(to_qpointf(sceneCenterPoint), to_qpointf(sceneAxis1Point2));
        QLineF norm = line.normalVector();
        EmbVector delta = emb_vector_subtract(sceneAxis2Point2, sceneCenterPoint);
        norm.translate(delta.x, delta.y);
        QPointF iPoint;
        norm.intersects(line, &iPoint);
        double ellipseHeight = emb_vector_distance(sceneAxis2Point2, to_emb_vector(iPoint)) * 2.0;

        obj_set_center(obj->core, sceneCenterPoint);
        obj->setObjectSize(ellipseWidth, ellipseHeight);
        obj->setRotation(-ellipseRot);

        EmbVector itemCenterPoint = obj_map_rubber(obj, "ELLISPE_CENTER");
        EmbVector itemAxis2Point2 = obj_map_rubber(obj, "ELLIPSE_AXIS2_POINT2");
        QLineF itemLine(to_qpointf(itemCenterPoint), to_qpointf(itemAxis2Point2));
        if (painter) {
            obj_draw_rubber_line(obj, itemLine, painter, "VIEW_COLOR_CROSSHAIR");
        }
        obj_update_path(obj);
        break;
    }
    case RUBBER_IMAGE: {
        EmbVector start = obj_rubber_point(obj, "IMAGE_START");
        EmbVector end = obj_rubber_point(obj, "IMAGE_END");
        EmbVector delta = emb_vector_subtract(end, start);
        obj_set_rect(obj, start.x, start.y, delta.x, delta.y);
        obj_update_path(obj);
        break;
    }
    case RUBBER_LINE: {
        EmbVector sceneStartPoint = obj_rubber_point(obj, "LINE_START");
        EmbVector sceneQSnapPoint = obj_rubber_point(obj, "LINE_END");

        obj_set_end_point_1(obj->core, sceneStartPoint);
        obj_set_end_point_2(obj->core, sceneQSnapPoint);

        obj_draw_rubber_line(obj, obj_line(obj), painter, "VIEW_COLOR_CROSSHAIR");
        break;
    }
    case RUBBER_POLYGON: {
        obj_set_pos(obj->core, obj_rubber_point(obj, "POLYGON_POINT_0"));

        bool ok = false;
        QString numStr = obj_rubber_text(obj, "POLYGON_NUM_POINTS");
        if (numStr.isNull()) {
            return;
        }
        int num = numStr.toInt(&ok);
        if (!ok) {
            return;
        }

        QPainterPath rubberPath;
        rubberPath.moveTo(to_qpointf(obj_map_rubber(obj, "POLYGON_POINT_0")));
        for (int i = 1; i <= num; i++) {
            char key[MAX_STRING_LENGTH];
            sprintf(key, "POLYGON_POINT_%d", i);
            EmbVector appendPoint = obj_map_rubber(obj, key);
            rubberPath.lineTo(to_qpointf(appendPoint));
        }
        /* rubberPath.lineTo(0,0); */
        obj_update_path_r(obj, rubberPath);

        /* Ensure the path isn't updated until the number of points is changed again. */
        obj_set_rubber_text(obj, "POLYGON_NUM_POINTS", "");
        break;
    }
    case RUBBER_POLYGON_INSCRIBE: {
        obj_set_pos(obj->core, obj_rubber_point(obj, "POLYGON_CENTER"));

        quint16 numSides = obj_rubber_point(obj, "POLYGON_NUM_SIDES").x;

        EmbVector inscribePoint = obj_map_rubber(obj, "POLYGON_INSCRIBE_POINT");
        QLineF inscribeLine = QLineF(QPointF(0, 0), to_qpointf(inscribePoint));
        double inscribeAngle = inscribeLine.angle();
        double inscribeInc = 360.0/numSides;

        if (painter) {
            obj_draw_rubber_line(obj, inscribeLine, painter, "VIEW_COLOR_CROSSHAIR");
        }

        QPainterPath inscribePath;
        /* First Point */
        inscribePath.moveTo(to_qpointf(inscribePoint));
        /* Remaining Points */
        for (int i = 1; i < numSides; i++) {
            inscribeLine.setAngle(inscribeAngle + inscribeInc*i);
            inscribePath.lineTo(inscribeLine.p2());
        }
        obj_update_path_r(obj, inscribePath);
        break;
    }
    case RUBBER_POLYGON_CIRCUMSCRIBE: {
        obj_set_pos(obj->core, obj_rubber_point(obj, "POLYGON_CENTER"));

        quint16 numSides = obj_rubber_point(obj, "POLYGON_NUM_SIDES").x;

        EmbVector circumscribePoint = obj_map_rubber(obj, "POLYGON_CIRCUMSCRIBE_POINT");
        QLineF circumscribeLine = QLineF(QPointF(0, 0), to_qpointf(circumscribePoint));
        double circumscribeAngle = circumscribeLine.angle();
        double circumscribeInc = 360.0/numSides;

        if (painter) {
            obj_draw_rubber_line(obj, circumscribeLine, painter, "VIEW_COLOR_CROSSHAIR");
        }

        QPainterPath circumscribePath;
        /* First Point */
        QLineF prev(circumscribeLine.p2(), QPointF(0,0));
        prev = prev.normalVector();
        circumscribeLine.setAngle(circumscribeAngle + circumscribeInc);
        QLineF perp(circumscribeLine.p2(), QPointF(0,0));
        perp = perp.normalVector();
        QPointF iPoint;
        perp.intersects(prev, &iPoint);
        circumscribePath.moveTo(iPoint);
        /* Remaining Points */
        for (int i = 2; i <= numSides; i++) {
            prev = perp;
            circumscribeLine.setAngle(circumscribeAngle + circumscribeInc*i);
            perp = QLineF(circumscribeLine.p2(), QPointF(0,0));
            perp = perp.normalVector();
            perp.intersects(prev, &iPoint);
            circumscribePath.lineTo(iPoint);
        }
        obj_update_path_r(obj, circumscribePath);
        break;
    }
    case RUBBER_POLYLINE: {
        obj_set_pos(obj->core, obj_rubber_point(obj, "POLYLINE_POINT_0"));

        QPointF p = to_qpointf(obj_map_rubber(obj, ""));
        QLineF rubberLine(obj->normalPath.currentPosition(), p);
        if (painter) {
            obj_draw_rubber_line(obj, rubberLine, painter, "VIEW_COLOR_CROSSHAIR");
        }

        bool ok = false;
        QString numStr = obj_rubber_text(obj, "POLYLINE_NUM_POINTS");
        if (numStr.isNull()) return;
        int num = numStr.toInt(&ok);
        if (!ok) {
            return;
        }

        QPainterPath rubberPath;
        for (int i = 1; i <= num; i++) {
            char msg[MAX_STRING_LENGTH];
            sprintf(msg, "POLYLINE_POINT_%d", i);
            EmbVector appendPoint = obj_map_rubber(obj, msg);
            rubberPath.lineTo(to_qpointf(appendPoint));
        }
        obj_update_path_r(obj, rubberPath);

        /* Ensure the path isn't updated until the number of points is changed again. */
        obj_set_rubber_text(obj, "POLYLINE_NUM_POINTS", "");
        break;
    }
    case RUBBER_RECTANGLE: {
        EmbVector start = obj_rubber_point(obj, "RECTANGLE_START");
        EmbVector end = obj_rubber_point(obj, "RECTANGLE_END");
        EmbVector delta = emb_vector_subtract(end, start);
        obj_set_rect(obj, start.x, start.y, delta.x, delta.y);
        obj_update_path(obj);
        break;
    }
    case RUBBER_TEXTSINGLE: {
        obj_set_text_font(obj->core, qPrintable(obj_rubber_text(obj, "TEXT_FONT")));
        obj_set_text_justify(obj->core, qPrintable(obj_rubber_text(obj, "TEXT_JUSTIFY")));
        obj_set_pos(obj->core, obj_rubber_point(obj, "TEXT_POINT"));
        EmbVector hr = obj_rubber_point(obj, "TEXT_HEIGHT_ROTATION");
        obj_set_text_size(obj->core, hr.x);
        obj->setRotation(hr.y);
        obj_set_text(obj->core, qPrintable(obj_rubber_text(obj, "TEXT_RAPID")));
        break;
    }
    case RUBBER_GRIP: {
        obj_update_rubber_grip(obj, painter);
        break;
    }
    default:
        break;
    }
}

/* . */
void
Object::vulcanize()
{
    debug_message("vulcanize()");
    /* FIXME: obj_update_rubber(painter); */

    core->rubber_mode = RUBBER_OFF;

    switch (core->geometry->type) {
    case EMB_POLYLINE:
        if (!normalPath.elementCount()) {
            critical_box(
                translate("Empty Polyline Error"),
                translate("The polyline added contains no points. The command that created this object has flawed logic."));
        }
        break;
    case EMB_POLYGON:
        if (!normalPath.elementCount()) {
            critical_box(
                translate("Empty Polygon Error"),
                translate("The polygon added contains no points. The command that created this object has flawed logic."));
        }
        break;
    case EMB_PATH:
        if (!normalPath.elementCount()) {
            critical_box(
                translate("Empty Path Error"),
                translate("The path added contains no points. The command that created this object has flawed logic."));
        }
        break;
    default:
        break;
    }
}

/* . */
QList<EmbVector>
Object::allGripPoints()
{
    QList<EmbVector> gripPoints;
    switch (core->geometry->type) {
    case EMB_ARC: {
        gripPoints.append(obj_center(core));
        gripPoints.append(obj_start_point(core));
        gripPoints.append(obj_mid_point(core));
        gripPoints.append(obj_end_point(core));
        break;
    }
    case EMB_CIRCLE:
    case EMB_ELLIPSE: {
        gripPoints.append(obj_center(core));
        gripPoints.append(emb_quadrant(core->geometry, 0));
        gripPoints.append(emb_quadrant(core->geometry, 90));
        gripPoints.append(emb_quadrant(core->geometry, 180));
        gripPoints.append(emb_quadrant(core->geometry, 270));
        break;
    }
    case EMB_DIM_LEADER: {
        gripPoints.append(obj_end_point_1(core));
        gripPoints.append(obj_end_point_2(core));
        if (core->curved) {
            gripPoints.append(obj_mid_point(core));
        }
        break;
    }
    case EMB_IMAGE: {
        gripPoints.append(obj_top_left(core));
        gripPoints.append(obj_top_right(core));
        gripPoints.append(obj_bottom_left(core));
        gripPoints.append(obj_bottom_right(core));
        break;
    }
    case EMB_LINE: {
        gripPoints.append(obj_end_point_1(core));
        gripPoints.append(obj_end_point_2(core));
        gripPoints.append(obj_mid_point(core));
        break;
    }
    case EMB_PATH: {
        gripPoints.append(obj_pos(core));
        todo("loop thru all path Elements and return their points.");
        break;
    }
    case EMB_POLYGON:
    case EMB_POLYLINE: {
        QPainterPath::Element element;
        for (int i = 0; i < normalPath.elementCount(); ++i) {
            element = normalPath.elementAt(i);
            gripPoints.append(to_emb_vector(mapToScene(element.x, element.y)));
        }
        break;
    }
    case EMB_TEXT_SINGLE:
    case EMB_POINT:
    default:
        gripPoints.append(obj_pos(this->core));
        break;
    }
    return gripPoints;
}

/* Returns the closest snap point to the mouse point */
EmbVector
Object::mouseSnapPoint(EmbVector mousePoint)
{
    return find_mouse_snap_point(allGripPoints(), mousePoint);
}

/* . */
void
obj_grip_edit(Object *obj, EmbVector before, EmbVector after)
{
    EmbVector delta = emb_vector_subtract(after, before);
    switch (obj->core->geometry->type) {
    case EMB_ARC: {
        todo("gripEdit() for ArcObject.");
        break;
    }
    case EMB_CIRCLE: {
        if (emb_approx(before, obj_center(obj->core))) {
            obj->moveBy(delta.x, delta.y);
        }
        else {
            double length = emb_vector_distance(obj_center(obj->core), after);
            emb_set_radius(obj->core->geometry, length);
        }
        break;
    }
    case EMB_DIM_LEADER:
    case EMB_LINE: {
        if (emb_approx(before, obj_end_point_1(obj->core))) {
            obj_set_end_point_1(obj->core, after);
        }
        else if (emb_approx(before, obj_end_point_2(obj->core))) {
            obj_set_end_point_2(obj->core, after);
        }
        else if (emb_approx(before, obj_mid_point(obj->core))) {
            obj->moveBy(delta.x, delta.y);
        }
        break;
    }
    case EMB_ELLIPSE: {
        todo("gripEdit() for EllipseObject");
        break;
    }
    case EMB_IMAGE:
    case EMB_RECT: {
        double height = emb_height(obj->core->geometry);
        double width = emb_width(obj->core->geometry);
        EmbVector tl = obj_top_left(obj->core);
        if (emb_approx(before, tl)) {
            obj_set_rect(obj, after.x, after.y,
                width - delta.x, height - delta.y);
        }
        else if (emb_approx(before, obj_top_right(obj->core))) {
            obj_set_rect(obj, tl.x, tl.y+delta.y,
                width + delta.x, height - delta.y);
        }
        else if (emb_approx(before, obj_bottom_left(obj->core))) {
            obj_set_rect(obj, tl.x+delta.x, tl.y,
                width - delta.x, height + delta.y);
        }
        else if (emb_approx(before, obj_bottom_right(obj->core))) {
            obj_set_rect(obj, tl.x, tl.y,
                width + delta.x, height + delta.y);
        }
        break;
    }
    case EMB_PATH: {
        todo("gripEdit() for PathObject");
        break;
    }
    case EMB_POLYGON:
    case EMB_POLYLINE: {
        obj->core->gripIndex = obj_find_index(obj, before);
        if (obj->core->gripIndex == -1) {
            return;
        }
        EmbVector a = map_from_scene(obj, after);
        obj->normalPath.setElementPositionAt(obj->core->gripIndex, a.x, a.y);
        obj_update_path_r(obj, obj->normalPath);
        obj->core->gripIndex = -1;
        break;
    }
    case EMB_TEXT_SINGLE:
    case EMB_POINT:
    default: {
        if (emb_approx(before, obj_pos(obj->core))) {
            obj->moveBy(delta.x, delta.y);
        }
        break;
    }
    }
}

void
Object::paint(QPainter* painter, const QStyleOptionGraphicsItem *option, QWidget *)
{
    QGraphicsScene* objScene = scene();
    if (!objScene) {
        return;
    }

    QPen paintPen = pen();
    painter->setPen(paintPen);
    obj_update_rubber(this, painter);
    if (option->state & QStyle::State_Selected) {
        paintPen.setStyle(Qt::DashLine);
    }
    if (objScene->property("ENABLE_LWT").toBool()) {
        paintPen = lwtPen;
    }
    painter->setPen(paintPen);

    switch (core->geometry->type) {
    case EMB_ARC: {
        double startAngle = (emb_start_angle(core->geometry) + rotation())*16;
        double spanAngle = emb_included_angle(core->geometry) * 16;

        if (emb_clockwise(core->geometry)) {
            spanAngle = -spanAngle;
        }

        double rad = obj_radius(this->core);
        QRectF paintRect(-rad, -rad, rad*2.0, rad*2.0);
        painter->drawArc(paintRect, startAngle, spanAngle);
        break;
    }
    case EMB_CIRCLE:
    case EMB_ELLIPSE: {
        painter->drawEllipse(to_qrectf(obj_rect(core)));
        break;
    }
    case EMB_DIM_LEADER: {
        painter->drawPath(lineStylePath);
        painter->drawPath(arrowStylePath);

        if (core->filled) {
            painter->fillPath(arrowStylePath, objPen.color());
        }
        break;
    }
    case EMB_LINE: {
        if (core->rubber_mode != RUBBER_LINE) {
            painter->drawLine(obj_line(this));
        }

        if (objScene->property("ENABLE_LWT").toBool()
            && objScene->property("ENABLE_REAL").toBool()) {
            obj_real_render(this, painter, path());
        }
        break;
    }
    case EMB_IMAGE:
    case EMB_RECT: {
        painter->drawRect(to_qrectf(obj_rect(core)));
        break;
    }
    case EMB_PATH: {
        painter->drawPath(obj_path(this));
        break;
    }
    case EMB_POLYGON: {
        if (normalPath.elementCount()) {
            painter->drawPath(normalPath);
            QPainterPath::Element zero = normalPath.elementAt(0);
            QPainterPath::Element last = normalPath.elementAt(normalPath.elementCount()-1);
            painter->drawLine(QPointF(zero.x, zero.y), QPointF(last.x, last.y));
        }
        break;
    }
    case EMB_POLYLINE: {
        painter->drawPath(normalPath);

        if (objScene->property("ENABLE_LWT").toBool()
            && objScene->property("ENABLE_REAL").toBool()) {
            obj_real_render(this, painter, normalPath);
        }
        break;
    }
    case EMB_TEXT_SINGLE: {
        painter->drawPath(textPath);
        break;
    }
    default:
    case EMB_POINT: {
        painter->drawPoint(0,0);
        break;
    }
    }
}

QPainterPath
Object::objectCopyPath() const
{
    return normalPath;
}

QPainterPath
Object::objectSavePath() const
{
    QPainterPath path;
    QRectF r = to_qrectf(obj_rect(core));
    switch (core->geometry->type) {
    case EMB_CIRCLE:
    case EMB_ELLIPSE: {
        path.arcMoveTo(r, 0);
        path.arcTo(r, 0, 360);

        double s = scale();
        QTransform trans;
        trans.rotate(rotation());
        trans.scale(s,s);
        return trans.map(path);
    }
    case EMB_DIM_LEADER:
    case EMB_LINE: {
        QPainterPath path;
        EmbVector delta = obj_delta(this->core);
        path.lineTo(delta.x, delta.y);
        return path;
    }
    case EMB_POINT: {
        path.addRect(-0.00000001, -0.00000001, 0.00000002, 0.00000002);
        return path;
    }
    case EMB_RECT: {
        path.moveTo(r.bottomLeft());
        path.lineTo(r.bottomRight());
        path.lineTo(r.topRight());
        path.lineTo(r.topLeft());
        path.lineTo(r.bottomLeft());

        double s = scale();
        QTransform trans;
        trans.rotate(rotation());
        trans.scale(s,s);
        return trans.map(path);
    }
    case EMB_PATH:
    case EMB_POLYLINE: {
        double s = scale();
        QTransform trans;
        trans.rotate(rotation());
        trans.scale(s,s);
        return trans.map(normalPath);
    }
    case EMB_POLYGON: {
        QPainterPath closedPath = normalPath;
        closedPath.closeSubpath();
        double s = scale();
        QTransform trans;
        trans.rotate(rotation());
        trans.scale(s,s);
        return trans.map(closedPath);
    }
    default:
        break;
    }
    return normalPath;
}

/*
 * View
 */

Document::Document(MainWindow* mw, QGraphicsScene* theScene, QWidget* parent) : QGraphicsView(theScene, parent)
{
}

/* . */
Document::~Document()
{
    document_memory[data.id] = false;

    /* Prevent memory leaks by deleting any objects that were removed from the scene */
    for (const auto& [key, value] : hashDeletedObjects) {
        delete value;
    }
    hashDeletedObjects.clear();

    /* Prevent memory leaks by deleting any unused instances. */
    qDeleteAll(previewObjectList.begin(), previewObjectList.end());
    previewObjectList.clear();
}

/* FIXME */
Document*
create_doc(MainWindow* mw, QGraphicsScene* theScene, QWidget *parent)
{
    Document* doc = new Document(mw, theScene, parent);
    doc->data.id = numOfDocs;
    documents[doc->data.id] = doc;
    doc->gscene = theScene;
    document_memory[doc->data.id] = true;

    doc->setFrameShape(QFrame::NoFrame);

    /* NOTE: This has to be done before setting mouse tracking.
     * TODO: Review OpenGL for Qt5 later
     * if (get_bool(DISPLAY_USE_OPENGL)) {
     *     debug_message("Using OpenGL...");
     *     setViewport(new QGLWidget(QGLFormat(QGL::DoubleBuffer)));
     * }

     * TODO: Review RenderHints later
     * setRenderHint(QPainter::Antialiasing, get_bool(DISPLAY_RENDERHINT_AA));
     * setRenderHint(QPainter::TextAntialiasing, get_bool(DISPLAY_RENDERHINT_TEXT_AA));
     * setRenderHint(QPainter::SmoothPixmapTransform, get_bool(DISPLAY_RENDERHINT_SMOOTHPIX));
     * setRenderHint(QPainter::HighQualityAntialiasing, get_bool(DISPLAY_RENDERHINT_HIGH_AA));
     * setRenderHint(QPainter::NonCosmeticDefaultPen, get_bool(DISPLAY_RENDERHINT_NONCOSMETIC));

     * NOTE: FullViewportUpdate MUST be used for both the GL and Qt renderers.
     * NOTE: Qt renderer will not draw the foreground properly if it isnt set.
     */
    doc->setViewportUpdateMode(QGraphicsView::FullViewportUpdate);

    doc->data.panDistance = 10; /* TODO: should there be a setting for this??? */

    doc->setCursor(Qt::BlankCursor);
    doc->horizontalScrollBar()->setCursor(Qt::ArrowCursor);
    doc->verticalScrollBar()->setCursor(Qt::ArrowCursor);
    doc->data.qsnapLocatorColor = get_int(QSNAP_LOCATOR_COLOR);
    doc->data.qsnapLocatorSize = get_int(QSNAP_LOCATOR_SIZE);
    doc->data.qsnapApertureSize = get_int(QSNAP_APERTURE_SIZE);
    doc->data.gripColorCool = get_int(SELECTION_COOLGRIP_COLOR);
    doc->data.gripColorHot = get_int(SELECTION_HOTGRIP_COLOR);
    doc->data.gripSize = get_int(SELECTION_GRIP_SIZE);
    doc->data.pickBoxSize = get_int(SELECTION_PICKBOX_SIZE);
    doc_set_cross_hair_color(doc->data.id, get_int(DISPLAY_CROSSHAIR_COLOR));
    doc_set_cross_hair_size(doc->data.id, get_int(DISPLAY_CROSSHAIR_PERCENT));
    doc_set_grid_color(doc->data.id, get_int(GRID_COLOR));

    if (get_bool(GRID_SHOW_ON_LOAD)) {
        doc_create_grid(doc->data.id, get_str(GRID_TYPE));
    }
    else {
        doc_create_grid(doc->data.id, "");
    }

    doc_toggle_ruler(doc->data.id, get_bool(RULER_SHOW_ON_LOAD));
    doc_toggle_real(doc->data.id, true);
    /* TODO: load this from file, else settings with default being true. */

    doc->data.grippingActive = false;
    doc->data.rapidMoveActive = false;
    doc->data.previewMode = PREVIEW_NULL;
    doc->data.previewData = 0;
    doc->previewObjectItemGroup = 0;
    doc->pasteObjectItemGroup = 0;
    doc->data.previewActive = false;
    doc->data.pastingActive = false;
    doc->data.movingActive = false;
    doc->data.selectingActive = false;
    doc->data.zoomWindowActive = false;
    doc->data.panningRealTimeActive = false;
    doc->data.panningPointActive = false;
    doc->data.panningActive = false;
    doc->data.qSnapActive = false;
    doc->data.qSnapToggle = false;

    /* TODO: set up config */
    doc->data.enableRuler = true;
    doc->data.enableGrid = true;
    doc->data.enableOrtho = false;
    doc->data.enablePolar = false;
    doc->data.enableLwt = false;
    doc->data.enableRuler = true;

    /* Randomize the hot grip location initially so it's not located at (0,0). */
    srand(QDateTime::currentMSecsSinceEpoch());
    doc->data.sceneGripPoint = to_emb_vector(QPointF(rand()*1000, rand()*1000));

    doc->gripBaseObj = 0;
    doc->tempBaseObj = 0;

    doc->selectBox = new SelectBox(QRubberBand::Rectangle, doc);
    doc->selectBox->setColors(
        QColor(get_int(DISPLAY_SELECTBOX_LEFT_COLOR)),
        QColor(get_int(DISPLAY_SELECTBOX_LEFT_FILL)),
        QColor(get_int(DISPLAY_SELECTBOX_RIGHT_COLOR)),
        QColor(get_int(DISPLAY_SELECTBOX_RIGHT_FILL)),
        get_int(DISPLAY_SELECTBOX_ALPHA));

    doc_show_scroll_bars(doc->data.id, get_bool(DISPLAY_SHOW_SCROLLBARS));
    doc_set_corner_button(doc->data.id);

    doc->undoStack = new QUndoStack(doc);
    dockUndoEdit->addStack(doc->undoStack);

    doc->installEventFilter(doc);

    doc->setMouseTracking(true);
    doc_set_background_color(doc->data.id, get_int(DISPLAY_BG_COLOR));
    /* TODO: wrap this with a setBackgroundPixmap() function: setBackgroundBrush(QPixmap("images/canvas.png")); */

    // FIXME: QObject::connect(doc->gscene, SIGNAL(selectionChanged()), doc_index,
    //    [=]() { doc_selection_changed(doc); });
    return doc;
}

/* Wrapper */
void
doc_update(int doc_index)
{
    documents[doc_index]->gscene->update();
}

double
doc_width(int doc_index)
{
    return documents[doc_index]->width();
}

double
doc_height(int doc_index)
{
    return documents[doc_index]->height();
}

/* . */
EmbVector
doc_map_from_scene(int32_t doc, EmbVector v)
{
    QPointF p = documents[doc]->mapFromScene(to_qpointf(v).toPoint());
    return to_emb_vector(p);
}

/* . */
EmbVector
doc_map_to_scene(int32_t doc, EmbVector v)
{
    QPointF p = documents[doc]->mapToScene(to_qpointf(v).toPoint());
    return to_emb_vector(p);
}

/* . */
EmbVector
doc_center(int32_t doc)
{
    EmbVector a = emb_vector(0.0, 0.0);
    // FIXME: return documents[doc]->center();
    return a;
}

/* FIXME */
void
doc_center_on(int32_t doc, EmbVector p)
{
    documents[doc]->centerOn(to_qpointf(p));
}

/* . */
void
Document::enterEvent(QEvent* /*event*/)
{
    QMdiSubWindow* mdiWin = qobject_cast<QMdiSubWindow*>(parent());
    if (mdiWin) {
        if (mdiArea) {
            mdiArea->setActiveSubWindow(mdiWin);
        }
    }
}

/* . */
void
doc_add_object(int32_t doc, Object* obj)
{
    documents[doc]->gscene->addItem(obj);
    doc_update(doc);
    documents[doc]->hashDeletedObjects.erase(obj->core->objID);
}

/* NOTE: We really just remove the objects from the scene. Deletion actually
 * occurs in the destructor.
 */
void
doc_delete_object(int32_t doc, Object* obj)
{
    obj->setSelected(false);
    documents[doc]->gscene->removeItem(obj);
    doc_update(doc);
    documents[doc]->hashDeletedObjects[obj->core->objID] = obj;
}

/* . */
void
doc_preview_on(int32_t doc, int clone, int mode, double x, double y, double data_)
{
    debug_message("View previewOn()");
    DocumentData *data = doc_data(doc);
    doc_preview_off(doc); /* Free the old objects before creating new ones */

    data->previewMode = mode;

    /* Create new objects and add them to the scene in an item group. */
    if (clone == PREVIEW_CLONE_SELECTED) {
        documents[doc]->previewObjectList = doc_create_object_list(doc, documents[doc]->gscene->selectedItems());
    }
    else if (clone == PREVIEW_CLONE_RUBBER) {
        documents[doc]->previewObjectList = doc_create_object_list(doc,
            documents[doc]->rubberRoomList);
    }
    else {
        return;
    }
    documents[doc]->previewObjectItemGroup = documents[doc]->gscene->createItemGroup(documents[doc]->previewObjectList);

    if (data->previewMode == PREVIEW_MOVE ||
       data->previewMode == PREVIEW_ROTATE ||
       data->previewMode == PREVIEW_SCALE) {
        data->previewPoint.x = x;
        data->previewPoint.y = y; /* NOTE: Move: basePt; Rotate: basePt; Scale: basePt; */
        data->previewData = data_;           /* NOTE: Move: unused; Rotate: refAngle; Scale: refFactor; */
        data->previewActive = true;
    }
    else {
        data->previewMode = PREVIEW_NULL;
        data->previewPoint = emb_vector(0.0, 0.0);
        data->previewData = 0;
        data->previewActive = false;
    }

    doc_update(doc);
}

/* . */
void
doc_preview_off(int32_t doc)
{
    /* Prevent memory leaks by deleting any unused instances */
    DocumentData *data = doc_data(doc);
    qDeleteAll(documents[doc]->previewObjectList.begin(), documents[doc]->previewObjectList.end());
    documents[doc]->previewObjectList.clear();

    if (documents[doc]->previewObjectItemGroup) {
        documents[doc]->gscene->removeItem(documents[doc]->previewObjectItemGroup);
        delete documents[doc]->previewObjectItemGroup;
        documents[doc]->previewObjectItemGroup = 0;
    }

    data->previewActive = false;

    doc_update(doc);
}

/* . */
void
doc_add_to_rubber_room(int32_t doc, QGraphicsItem* item)
{
    documents[doc]->rubberRoomList.append(item);
    item->show();
    doc_update(doc);
}

/* . */
void
doc_vulcanize_rubber_room(int32_t doc)
{
    foreach(QGraphicsItem* item, documents[doc]->rubberRoomList) {
        Object* base = static_cast<Object*>(item);
        if (base) {
            doc_vulcanize_object(doc, base);
        }
    }
    documents[doc]->rubberRoomList.clear();
    doc_update(doc);
}

/* . */
void
doc_vulcanize_object(int32_t doc, Object* obj)
{
    if (!obj) {
        return;
    }
    /* Prevent Qt Runtime Warning, QGraphicsScene::addItem: item has already been added to this scene */
    documents[doc]->gscene->removeItem(obj);
    obj->vulcanize();

    UndoableCommand* cmd = new UndoableCommand(ACTION_ADD, obj->core->OBJ_NAME,
        obj, doc, 0);
    if (cmd) {
        documents[doc]->undoStack->push(cmd);
    }
}

/* . */
void
doc_clear_rubber_room(int32_t doc)
{
    // FIXME:
    return;
    DocumentData *data = doc_data(doc);
    foreach(QGraphicsItem* item, documents[doc]->rubberRoomList) {
        Object* base = static_cast<Object*>(item);
        if (base) {
            int type = base->type();
            if ((type == OBJ_PATH && documents[doc]->spareRubberList.contains(SPARE_RUBBER_PATH))
            || (type == OBJ_POLYGON  && documents[doc]->spareRubberList.contains(SPARE_RUBBER_POLYGON))
            || (type == OBJ_POLYLINE && documents[doc]->spareRubberList.contains(SPARE_RUBBER_POLYLINE))
            || (documents[doc]->spareRubberList.contains(base->core->objID))) {
                if (!obj_path(base).elementCount()) {
                    critical_box(translate("Empty Rubber Object Error"),
                        translate("The rubber object added contains no points. "
                            "The command that created this object has flawed logic. "
                            "The object will be deleted."));
                    documents[doc]->gscene->removeItem(item);
                    delete item;
                }
                else {
                    doc_vulcanize_object(doc, base);
                }
            }
            else {
                documents[doc]->gscene->removeItem(item);
                delete item;
            }
        }
    }

    documents[doc]->rubberRoomList.clear();
    documents[doc]->spareRubberList.clear();
    doc_update(doc);
}

/* . */
void
doc_spare_rubber(int32_t doc, int64_t id)
{
    documents[doc]->spareRubberList.append(id);
}

/* . */
void
doc_set_rubber_mode(int32_t doc, int mode)
{
    foreach (QGraphicsItem* item, documents[doc]->rubberRoomList) {
        Object* base = static_cast<Object*>(item);
        if (base) {
            obj_set_rubber_mode(base->core, mode);
        }
    }
    doc_update(doc);
}

/* . */
void
doc_set_rubber_point(int32_t doc, EmbString key, EmbVector point)
{
    foreach (QGraphicsItem* item, documents[doc]->rubberRoomList) {
        Object* base = static_cast<Object*>(item);
        if (base) {
            base->setObjectRubberPoint((char*)qPrintable(key), point);
        }
    }
    doc_update(doc);
}

/* . */
void
doc_set_rubber_text(int32_t doc, EmbString key, EmbString txt)
{
    foreach (QGraphicsItem* item, documents[doc]->rubberRoomList) {
        Object* base = static_cast<Object*>(item);
        if (base) {
            base->setObjectRubberText((char*)qPrintable(key), (char*)qPrintable(txt));
        }
    }
    doc_update(doc);
}

/* . */
void
doc_create_grid(int32_t doc, EmbString gridType)
{
    if (string_equal(gridType, "Rectangular")) {
        doc_create_grid_rect(doc);
        documents[doc]->gscene->setProperty("ENABLE_GRID", true);
    }
    else if (string_equal(gridType, "Circular")) {
        doc_create_grid_polar(doc);
        documents[doc]->gscene->setProperty("ENABLE_GRID", true);
    }
    else if (string_equal(gridType, "Isometric")) {
        doc_create_grid_iso(doc);
        documents[doc]->gscene->setProperty("ENABLE_GRID", true);
    }
    else {
        documents[doc]->gridPath = QPainterPath();
        documents[doc]->gscene->setProperty("ENABLE_GRID", false);
    }

    doc_create_origin(doc);

    doc_update(doc);
}

/* https://developer.mozilla.org/en-US/docs/Web/SVG/Tutorial/Paths
 * M x y, m dx dy -- move to
 *
 * TODO: error reporting for parsing.
 */
void
svg_to_painterpath(QPainterPath *path, const char *svg, EmbVector pos, EmbVector scale)
{
    char token[MAX_STRING_LENGTH];
    char *p = (char*)svg;
    p = emb_get_svg_token(p, token);
    while (p) {
        switch (token[0]) {
        case 'M':
        case 'm': {
            EmbVector v;
            p = emb_get_svg_vector(p, &v);
            if (!p) {
                break;
            }
            if (token[0] == 'M') {
                pos.x = v.x * scale.x;
                pos.y = v.y * scale.y;
            }
            else {
                pos.x += v.x * scale.x;
                pos.y += v.y * scale.y;
            }
            path->moveTo(pos.x, pos.y);
            break;
        }
        case 'L':
        case 'l': {
            EmbVector v;
            p = emb_get_svg_vector(p, &v);
            if (!p) {
                break;
            }
            if (token[0] == 'L') {
                pos.x = v.x * scale.x;
                pos.y = v.y * scale.y;
            }
            else {
                pos.x += v.x * scale.x;
                pos.y += v.y * scale.y;
            }
            path->lineTo(pos.x, pos.y);
            break;
        }
        case 'A': {
            EmbVector v1, v2, v3;
            /* Start position */
            p = emb_get_svg_vector(p, &v1);
            if (!p) {
                break;
            }
            p = emb_get_svg_vector(p, &v2);
            if (!p) {
                break;
            }
            /* flag: ignored */
            p = emb_get_svg_token(p, token);
            if (!p) {
                break;
            }
            p = emb_get_svg_vector(p, &v3);
            if (!p) {
                break;
            }
            path->arcTo(
                v1.x * scale.x, v1.y * scale.y,
                v2.x * scale.x, v2.y * scale.y,
                v3.x, v3.y);
            break;
        }
        case 'Z': {
            path->closeSubpath();
            break;
        }
        default:
            break;
        }
        p = emb_get_svg_token(p, token);
    }
}

/* TODO: Make Origin Customizable */
void
doc_create_origin(int32_t doc)
{
    documents[doc]->originPath = QPainterPath();

    if (get_bool(GRID_SHOW_ORIGIN)) {
        /* originPath.addEllipse(QPointF(0,0), 0.5, 0.5); */
        svg_to_painterpath(&(documents[doc]->originPath), circle_origin_path, emb_vector(0.0, 0.0), emb_vector(1.0, 1.0));
    }
}

/* . */
void
doc_create_grid_rect(int32_t doc)
{
    double xSpacing = get_real(GRID_SPACING_X);
    double ySpacing = get_real(GRID_SPACING_Y);

    QRectF gr(0, 0, get_real(GRID_SIZE_X), -get_real(GRID_SIZE_Y));
    /* Ensure the loop will work correctly with negative numbers */
    double x1 = EMB_MIN(gr.left(), gr.right());
    double y1 = EMB_MIN(gr.top(), gr.bottom());
    double x2 = EMB_MAX(gr.left(), gr.right());
    double y2 = EMB_MAX(gr.top(), gr.bottom());

    documents[doc]->gridPath = QPainterPath();
    documents[doc]->gridPath.addRect(gr);
    for (double gx = x1; gx < x2; gx += xSpacing) {
        for (double gy = y1; gy < y2; gy += ySpacing) {
            documents[doc]->gridPath.moveTo(x1, gy);
            documents[doc]->gridPath.lineTo(x2, gy);
            documents[doc]->gridPath.moveTo(gx, y1);
            documents[doc]->gridPath.lineTo(gx, y2);
        }
    }

    /* Center the Grid */
    QRectF gridRect = documents[doc]->gridPath.boundingRect();
    EmbVector b;
    b.x = gridRect.width()/2.0;
    b.y = -gridRect.height()/2.0;

    if (get_bool(GRID_CENTER_ON_ORIGIN)) {
        documents[doc]->gridPath.translate(-b.x, -b.y);
    }
    else {
        EmbVector c;
        c.x = get_real(GRID_CENTER_X);
        c.y = -get_real(GRID_CENTER_Y);
        EmbVector d = emb_vector_subtract(c, b);
        documents[doc]->gridPath.translate(d.x, d.y);
    }
}

/* . */
void
doc_create_grid_polar(int32_t doc)
{
    double radSpacing = get_real(GRID_SPACING_RADIUS);
    double angSpacing = get_real(GRID_SPACING_ANGLE);

    double rad = get_real(GRID_SIZE_RADIUS);

    documents[doc]->gridPath = QPainterPath();
    documents[doc]->gridPath.addEllipse(QPointF(0,0), rad, rad);
    for (double r = 0; r < rad; r += radSpacing) {
        documents[doc]->gridPath.addEllipse(QPointF(0,0), r, r);
    }
    for (double ang = 0; ang < 360; ang += angSpacing) {
        documents[doc]->gridPath.moveTo(0,0);
        documents[doc]->gridPath.lineTo(QLineF::fromPolar(rad, ang).p2());
    }

    if (!get_bool(GRID_CENTER_ON_ORIGIN)) {
        double cx = get_real(GRID_CENTER_X);
        double cy = get_real(GRID_CENTER_Y);
        documents[doc]->gridPath.translate(cx, -cy);
    }
}

/* . */
void
doc_create_grid_iso(int32_t doc)
{
    double xSpacing = get_real(GRID_SPACING_X);
    double ySpacing = get_real(GRID_SPACING_Y);

    /* Ensure the loop will work correctly with negative numbers. */
    double isoW = fabs(get_real(GRID_SIZE_X));
    double isoH = fabs(get_real(GRID_SIZE_Y));

    QPointF p1 = QPointF(0,0);
    QPointF p2 = QLineF::fromPolar(isoW, 30).p2();
    QPointF p3 = QLineF::fromPolar(isoH, 150).p2();
    QPointF p4 = p2 + p3;

    documents[doc]->gridPath = QPainterPath();
    documents[doc]->gridPath.moveTo(p1);
    documents[doc]->gridPath.lineTo(p2);
    documents[doc]->gridPath.lineTo(p4);
    documents[doc]->gridPath.lineTo(p3);
    documents[doc]->gridPath.lineTo(p1);

    for (double x = 0; x < isoW; x += xSpacing) {
        for (double y = 0; y < isoH; y += ySpacing) {
            QPointF px = QLineF::fromPolar(x, 30).p2();
            QPointF py = QLineF::fromPolar(y, 150).p2();

            documents[doc]->gridPath.moveTo(px);
            documents[doc]->gridPath.lineTo(px+p3);
            documents[doc]->gridPath.moveTo(py);
            documents[doc]->gridPath.lineTo(py+p2);
        }
    }

    /* Center the Grid */

    QRectF gridRect = documents[doc]->gridPath.boundingRect();
    /* bx is unused */
    double by = -gridRect.height()/2.0;
    double cx = get_real(GRID_CENTER_X);
    double cy = -get_real(GRID_CENTER_Y);

    if (get_real(GRID_CENTER_ON_ORIGIN)) {
        documents[doc]->gridPath.translate(0, -by);
    }
    else {
        documents[doc]->gridPath.translate(0, -by);
        documents[doc]->gridPath.translate(cx, cy);
    }
}

/* . */
DocumentData *
doc_data(int32_t doc)
{
    return &(documents[doc]->data);
}

/* . */
void
draw_arc(QPainter* painter, EmbArc arc)
{
    QPainterPath path;
}

/* . */
void
draw_circle(QPainter* painter, EmbCircle circle)
{
    QPainterPath path;
    EmbVector p = circle.center;
    double rad = circle.radius;
    path.moveTo(p.x, p.y + rad);
    path.arcTo(p.x-rad, p.y-rad, rad*2.0, rad*2.0, 90.0, 360.0);
    path.arcTo(p.x-rad, p.y-rad, rad*2.0, rad*2.0, 90.0, -360.0);
    path.closeSubpath();
    painter->drawPath(path);
}

/* . */
void
draw_ellipse(QPainter* painter, EmbEllipse ellipse)
{
    QPainterPath path;
}

/* . */
void
draw_line(QPainter* painter, EmbLine line)
{
    QPainterPath path;
}

/* . */
void
draw_polygon(QPainter* painter, EmbPolygon polygon)
{
    QPainterPath path;
}

/* . */
void
draw_polyline(QPainter* painter, EmbPolyline polyline)
{
    QPainterPath path;
    EmbGeometry *geometry = polyline.pointList->geometry;
    path.moveTo(geometry[0].object.vector.x, geometry[0].object.vector.y);
    for (int i=0; i<polyline.pointList->count; i++) {
        path.lineTo(geometry[i].object.vector.x, geometry[i].object.vector.y);
    }
    painter->drawPath(path);
}

void
draw_rect(QPainter* painter, EmbRect rect)
{
    QPainterPath path;
    path.moveTo(rect.x, rect.y);
    path.lineTo(rect.x + rect.w, rect.y + rect.h);
    path.lineTo(rect.x + rect.w, rect.y);
    path.lineTo(rect.x, rect.y);
    painter->drawPath(path);
}

/* . */
void
draw_spline(QPainter* painter, EmbSpline spline)
{

}

/* . */
void
Document::drawBackground(QPainter* painter, const QRectF& rect)
{
    int32_t doc = data.id;
    DocumentData *data = doc_data(doc);
    painter->fillRect(rect, backgroundBrush());

    if (data->enableGrid && rect.intersects(documents[doc]->gridPath.controlPointRect())) {
        QPen gridPen(QColor(data->gridColor));
        gridPen.setJoinStyle(Qt::MiterJoin);
        gridPen.setCosmetic(true);
        painter->setPen(gridPen);
        painter->drawPath(documents[doc]->gridPath);
        painter->drawPath(documents[doc]->originPath);
        painter->fillPath(documents[doc]->originPath, QColor(data->gridColor));
    }

    EmbPattern *pattern = activeMdiWindow()->pattern;
    for (int i = 0; i < pattern->geometry->count; i++) {
        EmbGeometry g = pattern->geometry->geometry[i];
        switch (g.type) {
        case EMB_ARC: {
            draw_arc(painter, g.object.arc);
            break;
        }
        case EMB_CIRCLE: {
            draw_circle(painter, g.object.circle);
            break;
        }
        case EMB_ELLIPSE: {
            draw_ellipse(painter, g.object.ellipse);
            break;
        }
        case EMB_LINE: {
            draw_line(painter, g.object.line);
            break;
        }
        case EMB_POLYGON: {
            draw_polygon(painter, g.object.polygon);
            break;
        }
        case EMB_POLYLINE: {
            draw_polyline(painter, g.object.polyline);
            break;
        }
        case EMB_RECT: {
            draw_rect(painter, g.object.rect);
            break;
        }
        case EMB_SPLINE: {
            draw_spline(painter, g.object.spline);
            break;
        }
        default:
            break;
        }
    }
}

/* . */
void
Document::draw_rulers(QPainter* painter, const QRectF& rect)
{
    int32_t doc = data.id;
    DocumentData *data = doc_data(doc);

    int vw = width(); /* View Width */
    int vh = height(); /* View Height */
    EmbVector origin = doc_map_to_scene(doc, emb_vector(0.0, 0.0));
    EmbVector rulerHoriz = doc_map_to_scene(doc, emb_vector(vw, data->rulerPixelSize));
    EmbVector rulerVert = doc_map_to_scene(doc, emb_vector(data->rulerPixelSize, vh));

    EmbRect ruler_h, ruler_v;

    ruler_h.x = rulerHoriz.x;
    ruler_h.y = rulerHoriz.y;
    ruler_h.w = rulerHoriz.x - origin.x;
    ruler_h.h = rulerHoriz.y - origin.y;

    ruler_v.x = rulerVert.x;
    ruler_v.y = rulerVert.y;
    ruler_v.w = ruler_v.x - origin.x;
    ruler_v.h = ruler_v.y - origin.y;

    /* NOTE: Drawing ruler if zoomed out too far will cause an assertion failure. */
    /* We will limit the maximum size the ruler can be shown at. */
    uint16_t maxSize = -1; /* Intentional underflow */
    if (ruler_h.w >= maxSize || ruler_v.h >= maxSize) {
        return;
    }

    int distance = documents[doc]->mapToScene(data->rulerPixelSize*3, 0).x() - origin.x;
    QString distStr = QString().setNum(distance);
    int distStrSize = distStr.size();
    int msd = distStr.at(0).digitValue(); /* Most Significant Digit */

    if (msd == -1) {
        return;
    }

    msd++;
    if (msd == 10) {
        msd = 1;
        distStr.resize(distStrSize+1);
        distStrSize++;
    }

    distStr.replace(0, 1, QString().setNum(msd));
    for (int i = 1; i < distStrSize; ++i) {
        distStr.replace(i, 1, '0');
    }
    int unit = distStr.toInt();
    double fraction;
    bool feet = true;
    if (data->rulerMetric) {
        if (unit < 10) {
            unit = 10;
        }
        fraction = unit/10;
    }
    else {
        if (unit <= 1) {
            unit = 1;
            feet = false;
            fraction = (double)(unit/16);
        }
        else {
            unit = roundToMultiple(true, unit, 12);
            fraction = unit/12;
        }
    }

    double little  = 0.20;
    double medium = 0.40;
    double rhTextOffset = documents[doc]->mapToScene(3, 0).x() - origin.x;
    double rvTextOffset = documents[doc]->mapToScene(0, 3).y() - origin.y;
    double textHeight = ruler_h.h*medium;

    std::vector<QLineF> lines;
    lines.push_back(QLineF(origin.x, ruler_h.y, ruler_h.x, ruler_h.y));
    lines.push_back(QLineF(ruler_v.x, origin.y, ruler_v.x, ruler_v.y));

    lines.push_back(QLineF(data->sceneMousePoint.x, ruler_h.y,
        data->sceneMousePoint.x, origin.y));
    lines.push_back(QLineF(ruler_v.x, data->sceneMousePoint.y,
        origin.x, data->sceneMousePoint.y));

    QTransform transform;

    QPen rulerPen(QColor(0, 0, 0));
    rulerPen.setCosmetic(true);
    painter->setPen(rulerPen);
    painter->fillRect(QRectF(origin.x, origin.y, ruler_h.w, ruler_h.h),
        documents[doc]->data.rulerColor);
    painter->fillRect(QRectF(origin.x, origin.y, ruler_v.w, ruler_v.h),
        documents[doc]->data.rulerColor);

    int xFlow, xStart, yFlow, yStart;
    if (willUnderflowInt32(origin.x, unit)) {
        return;
    }
    xFlow = roundToMultiple(false, origin.x, unit);
    if (willUnderflowInt32(xFlow, unit)) {
        return;
    }
    xStart = xFlow - unit;
    if (willUnderflowInt32(origin.y, unit)) {
        return;
    }
    yFlow = roundToMultiple(false, origin.y, unit);
    if (willUnderflowInt32(yFlow, unit)) {
        return;
    }
    yStart = yFlow - unit;

    for (int x = xStart; x < ruler_h.x; x += unit) {
        char label[MAX_STRING_LENGTH];
        transform.translate(x+rhTextOffset, ruler_h.y-ruler_h.h/2);
        QPainterPath rulerTextPath;
        if (data->rulerMetric) {
            sprintf(label, "%d", x);
            rulerTextPath = transform.map(doc_create_ruler_text_path(label, textHeight));
        }
        else {
            if (feet) {
                sprintf(label, "%d'", x/12);
                rulerTextPath = transform.map(doc_create_ruler_text_path(label, textHeight));
            }
            else {
                sprintf(label, "%d\"", x);
                rulerTextPath = transform.map(doc_create_ruler_text_path(label, textHeight));
            }
        }
        transform.reset();
        painter->drawPath(rulerTextPath);

        lines.push_back(QLineF(x, ruler_h.y, x, origin.y));
        if (data->rulerMetric) {
            for (int i=1; i<10; i++) {
                double xf = x + fraction*i;
                double tick = ruler_h.y - ruler_h.h * little;
                if (i == 5) {
                    tick = ruler_h.y - ruler_h.h * medium;
                }
                lines.push_back(QLineF(xf, ruler_h.y, xf, tick));
            }
        }
        else {
            if (feet) {
                for (int i = 0; i < 12; ++i) {
                    double xf = x + fraction*i;
                    double tick = ruler_h.y - ruler_h.h * medium;
                    lines.push_back(QLineF(xf, ruler_h.y, xf, tick));
                }
            }
            else {
                for (int i=1; i<16; i++) {
                    double xf = x + fraction*i;
                    double tick = ruler_h.y - ruler_h.h * little;
                    if (i % 4 == 0) {
                        tick = ruler_h.y - ruler_h.h * medium;
                    }
                    lines.push_back(QLineF(xf, ruler_h.y, xf, tick));
                }
            }
        }
    }
    for (int y = yStart; y < ruler_v.y; y += unit) {
        char label[MAX_STRING_LENGTH];
        transform.translate(ruler_v.x-ruler_v.w/2, y-rvTextOffset);
        transform.rotate(-90);
        QPainterPath rulerTextPath;
        if (data->rulerMetric) {
            sprintf(label, "%d", -y);
            rulerTextPath = transform.map(doc_create_ruler_text_path(label, textHeight));
        }
        else {
            if (feet) {
                sprintf(label, "%d'", -y/12);
                rulerTextPath = transform.map(doc_create_ruler_text_path(label, textHeight));
            }
            else {
                sprintf(label, "%d", -y);
                rulerTextPath = transform.map(doc_create_ruler_text_path(label, textHeight));
            }
        }
        transform.reset();
        painter->drawPath(rulerTextPath);

        lines.push_back(QLineF(ruler_v.x, y, origin.x, y));
        if (data->rulerMetric) {
            for (int i=1; i<10; i++) {
                double yf = y + fraction*i;
                double tick = ruler_v.x - ruler_v.w * little;
                if (i == 5) {
                    tick = ruler_v.x - ruler_v.w * medium;
                }
                lines.push_back(QLineF(ruler_v.x, yf, tick, yf));
            }
        }
        else {
            if (feet) {
                for (int i = 0; i < 12; ++i) {
                    lines.push_back(QLineF(ruler_v.x, y+fraction*i, ruler_v.x-ruler_v.w*medium, y+fraction*i));
                }
            }
            else {
                for (int i=1; i<16; i++) {
                    double yf = y + fraction*i;
                    double tick = ruler_v.x - ruler_v.w * little;
                    if (i % 4 == 0) {
                        tick = ruler_v.x - ruler_v.w * medium;
                    }
                    lines.push_back(QLineF(ruler_v.x, yf, tick, yf));
                }
            }
        }
    }

    QVector<QLineF> qlines;
    for (int i=0; i<lines.size(); i++) {
        qlines.append(lines[i]);
    }

    painter->drawLines(qlines);
    painter->fillRect(QRectF(origin.x, origin.y, ruler_v.w, ruler_h.h),
        documents[doc]->data.rulerColor);
}

/* . */
void
Document::drawForeground(QPainter* painter, const QRectF& rect)
{
    int32_t doc = data.id;
    DocumentData *data = doc_data(doc);
    /* Draw grip points for all selected objects */

    QPen gripPen(QColor::fromRgb(data->gripColorCool));
    gripPen.setWidth(2);
    gripPen.setJoinStyle(Qt::MiterJoin);
    gripPen.setCosmetic(true);
    painter->setPen(gripPen);
    QPointF gripOffset(data->gripSize, data->gripSize);

    QList<EmbVector> selectedGripPoints;
    QList<QGraphicsItem*> selectedItemList = documents[doc]->gscene->selectedItems();
    if (selectedItemList.size() <= 100) {
        foreach(QGraphicsItem* item, selectedItemList) {
            if (item->type() >= OBJ_BASE) {
                documents[doc]->tempBaseObj = static_cast<Object*>(item);
                if (documents[doc]->tempBaseObj) {
                    selectedGripPoints = documents[doc]->tempBaseObj->allGripPoints();
                }

                EmbVector offset = to_emb_vector(gripOffset);
                foreach(EmbVector ssp, selectedGripPoints) {
                    EmbVector p1 = emb_vector_subtract(doc_map_from_scene(doc, ssp), offset);
                    EmbVector q1 = emb_vector_add(doc_map_from_scene(doc, ssp), offset);
                    QPointF p2 = to_qpointf(doc_map_to_scene(doc, p1));
                    QPointF q2 = to_qpointf(doc_map_to_scene(doc, q1));

                    if (emb_approx(ssp, data->sceneGripPoint)) {
                        painter->fillRect(QRectF(p2, q2), QColor::fromRgb(data->gripColorHot));
                    }
                    else {
                        painter->drawRect(QRectF(p2, q2));
                    }
                }
            }
        }
    }

    /* Draw the closest qsnap point */

    /* TODO: && findClosestSnapPoint == true */
    if (!data->selectingActive) {
        QPen qsnapPen(data->qsnapLocatorColor);
        qsnapPen.setWidth(2);
        qsnapPen.setJoinStyle(Qt::MiterJoin);
        qsnapPen.setCosmetic(true);
        painter->setPen(qsnapPen);
        EmbVector qsnapOffset = emb_vector(data->qsnapLocatorSize,
            data->qsnapLocatorSize);

        double x = data->viewMousePoint.x - data->qsnapApertureSize;
        double y = data->viewMousePoint.y - data->qsnapApertureSize;
        QList<EmbVector> apertureSnapPoints;
        QList<QGraphicsItem *> apertureItemList = items(x, y,
            data->qsnapApertureSize*2,
            data->qsnapApertureSize*2);
        foreach(QGraphicsItem* item, apertureItemList) {
            if (item->type() >= OBJ_BASE) {
                documents[doc]->tempBaseObj = static_cast<Object*>(item);
                if (documents[doc]->tempBaseObj) {
                    EmbVector p = data->sceneMousePoint;
                    EmbVector q = documents[doc]->tempBaseObj->mouseSnapPoint(p);
                    apertureSnapPoints << q;
                }
            }
        }
        /* TODO: Check for intersection snap points and add them to the list. */
        foreach(EmbVector asp, apertureSnapPoints) {
            EmbVector p1 = emb_vector_subtract(doc_map_from_scene(doc, asp), qsnapOffset);
            EmbVector q1 = emb_vector_add(doc_map_from_scene(doc, asp), qsnapOffset);
            painter->drawRect(QRectF(
                to_qpointf(doc_map_to_scene(doc, p1)),
                to_qpointf(doc_map_to_scene(doc, q1))));
        }
    }

    /* Draw horizontal and vertical rulers */

    if (data->enableRuler) {
        draw_rulers(painter, rect);
    }

    /* Draw the crosshair */
    if (!data->selectingActive) {
        /* painter->setBrush(Qt::NoBrush); */
        QPen crosshairPen(data->crosshairColor);
        crosshairPen.setCosmetic(true);
        painter->setPen(crosshairPen);

        QPointF p1 = documents[doc]->mapToScene(data->viewMousePoint.x, data->viewMousePoint.y - data->crosshairSize);
        QPointF p2 = documents[doc]->mapToScene(data->viewMousePoint.x, data->viewMousePoint.y + data->crosshairSize);
        painter->drawLine(QLineF(p1, p2));

        QPointF p3 = documents[doc]->mapToScene(data->viewMousePoint.x - data->crosshairSize, data->viewMousePoint.y);
        QPointF p4 = documents[doc]->mapToScene(data->viewMousePoint.x + data->crosshairSize, data->viewMousePoint.y);
        painter->drawLine(QLineF(p3, p4));

        QPointF p5 = documents[doc]->mapToScene(data->viewMousePoint.x - data->pickBoxSize,
            data->viewMousePoint.y - data->pickBoxSize);
        QPointF p6 = documents[doc]->mapToScene(data->viewMousePoint.x + data->pickBoxSize,
            data->viewMousePoint.y + data->pickBoxSize);
        painter->drawRect(QRectF(p5, p6));
    }
}

/* Converting the the path descriptions isn't working here because it is
 * scaled before it is placed, we could reverse the scaling in x and y
 * then rescale?
 */
QPainterPath
doc_create_ruler_text_path(EmbString str, float height)
{
    QPainterPath path = QPainterPath();
    EmbVector position = emb_vector(0.0, 0.0);
    EmbVector scale = emb_vector(height, height);

    for (int i = 0; str[i]; ++i) {
        switch (str[i]) {
        case '1': {
            path.moveTo(position.x+0.05*scale.x, position.y-0.00*scale.y);
            path.lineTo(position.x+0.45*scale.x, position.y-0.00*scale.y);
            path.moveTo(position.x+0.00*scale.x, position.y-0.75*scale.y);
            path.lineTo(position.x+0.25*scale.x, position.y-1.00*scale.y);
            path.lineTo(position.x+0.25*scale.x, position.y-0.00*scale.y);
            break;
        }

        case '2': {
            path.moveTo(position.x + 0.00 * scale.x, position.y - 0.75*scale.y);
            path.arcTo(position.x + 0.00 * scale.x, position.y - 1.00*scale.y,
                0.50 * scale.x, 0.50 * scale.y, 180.00, -216.87);
            path.lineTo(position.x + 0.00*scale.x, position.y - 0.00*scale.y);
            path.lineTo(position.x + 0.50*scale.x, position.y - 0.00*scale.y);
            break;
        }

        case '3': {
            path.arcMoveTo(position.x + 0.00 * scale.x, -0.50 * scale.y,
                0.50 * scale.x, 0.50 * scale.y, 195.00);
            path.arcTo(position.x + 0.00 * scale.x, -0.50 * scale.y,
                0.50 * scale.x, 0.50 * scale.y, 195.00, 255.00);
            path.arcTo(position.x+0.00*scale.x, -1.00 * scale.y,
                0.50 * scale.x, 0.50 * scale.y, 270.00, 255.00);
            break;
        }

        case '4': {
            path.moveTo(position.x + 0.50 * scale.x, -0.00 * scale.y);
            path.lineTo(position.x + 0.50 * scale.x, -1.00 * scale.y);
            path.lineTo(position.x + 0.00 * scale.x, -0.50 * scale.y);
            path.lineTo(position.x + 0.50 * scale.x, -0.50 * scale.y);
            break;
        }

        case '5': {
            path.moveTo(position.x + 0.50 * scale.x, -1.00 * scale.y);
            path.lineTo(position.x + 0.00 * scale.x, -1.00 * scale.y);
            path.lineTo(position.x + 0.00 * scale.x, -0.50 * scale.y);
            path.lineTo(position.x + 0.25 * scale.x, -0.50 * scale.y);
            path.arcTo(position.x + 0.00 * scale.x, -0.50 * scale.y,
                0.50 * scale.x, 0.50 * scale.y, 90.00, -180.00);
            path.lineTo(position.x + 0.00 * scale.x, -0.00 * scale.y);
            break;
        }

        case '6': {
            path.addEllipse(QPointF(position.x+0.25*scale.x, -0.25*scale.y),
                0.25*scale.x, 0.25*scale.y);
            path.moveTo(position.x+0.00*scale.x, -0.25*scale.y);
            path.lineTo(position.x+0.00*scale.x, -0.75*scale.y);
            path.arcTo(position.x+0.00*scale.x, -1.00*scale.y, 0.50*scale.x, 0.50*scale.y, 180.00, -140.00);
            break;
        }

        case '7': {
            path.moveTo(position.x+0.00*scale.x, -1.00*scale.y);
            path.lineTo(position.x+0.50*scale.x, -1.00*scale.y);
            path.lineTo(position.x+0.25*scale.x, -0.25*scale.y);
            path.lineTo(position.x+0.25*scale.x, -0.00*scale.y);
            break;
        }

        case '8': {
            path.addEllipse(QPointF(position.x+0.25*scale.x, -0.25*scale.y), 0.25*scale.x, 0.25*scale.y);
            path.addEllipse(QPointF(position.x+0.25*scale.x, -0.75*scale.y), 0.25*scale.x, 0.25*scale.y);
            break;
        }

        case '9': {
            path.addEllipse(QPointF(position.x+0.25*scale.x, -0.75*scale.y), 0.25*scale.x, 0.25*scale.y);
            path.moveTo(position.x+0.50*scale.x, -0.75*scale.y);
            path.lineTo(position.x+0.50*scale.x, -0.25*scale.y);
            path.arcTo(position.x+0.00*scale.x, -0.50*scale.y, 0.50*scale.x, 0.50*scale.y, 0.00, -140.00);
            break;
        }

        case '0': {
            /* path.addEllipse(QPointF(position.x+0.25*scale.x, -0.50*scale.y), 0.25*scale.x, 0.50*scale.y); */

            path.moveTo(position.x+0.00*scale.x, -0.75*scale.y);
            path.lineTo(position.x+0.00*scale.x, -0.25*scale.y);
            path.arcTo(position.x+0.00*scale.x, -0.50*scale.y, 0.50*scale.x, 0.50*scale.y, 180.00, 180.00);
            path.lineTo(position.x+0.50*scale.x, -0.75*scale.y);
            path.arcTo(position.x+0.00*scale.x, -1.00*scale.y, 0.50*scale.x, 0.50*scale.y, 0.00, 180.00);
            break;
        }

        case '-': {
            path.moveTo(position.x+0.00*scale.x, -0.50*scale.y);
            path.lineTo(position.x+0.50*scale.x, -0.50*scale.y);
            break;
        }

        case '\'': {
            path.moveTo(position.x+0.25*scale.x, -1.00*scale.y);
            path.lineTo(position.x+0.25*scale.x, -0.75*scale.y);
            break;
        }

        case '"': {
            path.moveTo(position.x+0.10*scale.x, -1.00*scale.y);
            path.lineTo(position.x+0.10*scale.x, -0.75*scale.y);
            path.moveTo(position.x+0.40*scale.x, -1.00*scale.y);
            path.lineTo(position.x+0.40*scale.x, -0.75*scale.y);
            break;
        }

        default:
            break;
        }

        position.x += 0.75 * scale.x;
    }

    return path;
}

/* . */
void
doc_update_mouse_coords(int32_t doc, int x, int y)
{
    DocumentData *data = doc_data(doc);
    data->viewMousePoint = emb_vector(x, y);
    data->sceneMousePoint = doc_map_to_scene(doc, data->viewMousePoint);
    data->sceneQSnapPoint = data->sceneMousePoint;
    /* TODO: if qsnap functionality is enabled, use it rather than the mouse point */
    setMouseCoord(data->sceneMousePoint.x, -data->sceneMousePoint.y);
}

/*
 * NOTE: crosshairSize is in pixels and is a percentage of your screen width
 * NOTE: Example: (1280*0.05)/2 = 32, thus 32 + 1 + 32 = 65 pixel wide crosshair
 */
void
doc_set_cross_hair_size(int32_t doc, uint8_t percent)
{
    DocumentData *data = doc_data(doc);
    uint32_t screenWidth = QGuiApplication::primaryScreen()->geometry().width();
    if (percent > 0 && percent < 100) {
        data->crosshairSize = (screenWidth*(percent/100.0))/2;
    }
    else {
        data->crosshairSize = screenWidth;
    }
}

/* . */
void
doc_set_corner_button(int32_t doc)
{
    int num = get_int(DISPLAY_SCROLLBAR_WIDGET_NUM);
    if (num) {
        QPushButton* cornerButton = new QPushButton(documents[doc]);
        cornerButton->setFlat(true);
        QAction* act = actionHash[num];
        /* NOTE: Prevent crashing if the action is NULL. */
        if (!act) {
            information_box(translate("Corner Widget Error"),
                translate("There are unused enum values in COMMAND_ACTIONS. Please report this as a bug."));
            documents[doc]->setCornerWidget(0);
        }
        else {
            cornerButton->setIcon(act->icon());
            QObject::connect(cornerButton, SIGNAL(clicked()), documents[doc],
                SLOT(cornerButtonClicked()));
            documents[doc]->setCornerWidget(cornerButton);
            cornerButton->setCursor(Qt::ArrowCursor);
        }
    }
    else {
        documents[doc]->setCornerWidget(0);
    }
}

void
doc_cornerButtonClicked()
{
    debug_message("Corner Button Clicked.");
    actionHash[get_int(DISPLAY_SCROLLBAR_WIDGET_NUM)]->trigger();
}

/* . */
void
doc_zoom_in(Document *doc)
{
    debug_message("View zoomIn()");
    if (!doc_allow_zoom_in(doc->data.id)) {
        return;
    }
    wait_cursor();
    QPointF cntr = doc->mapToScene(QPoint(doc->width()/2, doc->height()/2));
    double s = get_real(DISPLAY_ZOOMSCALE_IN);
    doc->scale(s, s);

    doc_center_on(doc->data.id, to_emb_vector(cntr));
    restore_cursor();
}

/* . */
void
doc_zoom_out(Document *doc)
{
    debug_message("View zoomOut()");
    if (!doc_allow_zoom_out(doc->data.id)) {
        return;
    }
    wait_cursor();
    QPointF cntr = doc->mapToScene(QPoint(doc->width()/2, doc->height()/2));
    double s = get_real(DISPLAY_ZOOMSCALE_OUT);
    doc->scale(s, s);

    doc_center_on(doc->data.id, to_emb_vector(cntr));
    restore_cursor();
}

/* . */
void
doc_zoom_window(Document *doc)
{
    doc->data.zoomWindowActive = true;
    doc->data.selectingActive = false;
    doc_clear_selection(doc->data.id);
}

/* . */
void
doc_zoom_selected(Document *doc)
{
    QUndoStack* stack = activeUndoStack();
    if (stack) {
        UndoableCommand* cmd = new UndoableCommand(ACTION_NAV, "ZoomSelected",
            doc->data.id, 0);
        stack->push(cmd);
    }

    wait_cursor();
    QList<QGraphicsItem*> itemList = doc->gscene->selectedItems();
    QPainterPath selectedRectPath;
    foreach(QGraphicsItem* item, itemList) {
        selectedRectPath.addPolygon(item->mapToScene(item->boundingRect()));
    }
    QRectF selectedRect = selectedRectPath.boundingRect();
    if (selectedRect.isNull()) {
        information_box(translate("ZoomSelected Preselect"),
            translate("Preselect objects before invoking the zoomSelected command."));
        /* TODO: Support Post selection of objects */
    }
    doc->fitInView(selectedRect, Qt::KeepAspectRatio);
    restore_cursor();
}

/* . */
void
doc_zoom_extents(int32_t doc)
{
    QUndoStack* stack = activeUndoStack();
    if (stack) {
        UndoableCommand* cmd = new UndoableCommand(ACTION_NAV, "ZoomExtents", doc, 0);
        stack->push(cmd);
    }

    wait_cursor();
    QRectF extents = documents[doc]->gscene->itemsBoundingRect();
    if (extents.isNull()) {
        extents.setWidth(get_real(GRID_SIZE_X));
        extents.setHeight(get_real(GRID_SIZE_Y));
        extents.moveCenter(QPointF(0,0));
    }
    documents[doc]->fitInView(extents, Qt::KeepAspectRatio);
    restore_cursor();
}

/* . */
void
doc_pan_left(int32_t doc)
{
    QUndoStack* stack = activeUndoStack();
    DocumentData *data = doc_data(doc);
    if (stack) {
        UndoableCommand* cmd = new UndoableCommand(ACTION_NAV, "PanLeft", doc, 0);
        stack->push(cmd);
    }

    documents[doc]->horizontalScrollBar()->setValue(documents[doc]->horizontalScrollBar()->value() + data->panDistance);
    doc_update_mouse_coords(doc, data->viewMousePoint.x, data->viewMousePoint.y);
    doc_update(doc);
}

/* . */
void
doc_pan_right(int32_t doc)
{
    QUndoStack* stack = activeUndoStack();
    DocumentData *data = doc_data(doc);
    if (stack) {
        UndoableCommand* cmd = new UndoableCommand(ACTION_NAV, "PanRight", doc, 0);
        stack->push(cmd);
    }

    documents[doc]->horizontalScrollBar()->setValue(documents[doc]->horizontalScrollBar()->value() - data->panDistance);
    doc_update_mouse_coords(doc, data->viewMousePoint.x,
        data->viewMousePoint.y);
    doc_update(doc);
}

/* . */
void
doc_pan_up(int32_t doc)
{
    QUndoStack* stack = activeUndoStack();
    DocumentData *data = doc_data(doc);
    if (stack) {
        UndoableCommand* cmd = new UndoableCommand(ACTION_NAV, "PanUp", doc, 0);
        stack->push(cmd);
    }

    documents[doc]->verticalScrollBar()->setValue(documents[doc]->verticalScrollBar()->value() + data->panDistance);
    doc_update_mouse_coords(doc, data->viewMousePoint.x, data->viewMousePoint.y);
    doc_update(doc);
}

/* . */
void
doc_pan_down(int32_t doc)
{
    QUndoStack* stack = activeUndoStack();
    DocumentData *data = doc_data(doc);
    if (stack) {
        UndoableCommand* cmd = new UndoableCommand(ACTION_NAV, "PanDown", doc, 0);
        stack->push(cmd);
    }

    documents[doc]->verticalScrollBar()->setValue(documents[doc]->verticalScrollBar()->value() - data->panDistance);
    doc_update_mouse_coords(doc, data->viewMousePoint.x, data->viewMousePoint.y);
    doc_update(doc);
}

/* . */
void
doc_select_all(int32_t doc)
{
    QPainterPath allPath;
    allPath.addRect(documents[doc]->gscene->sceneRect());
    /*  documents[doc]->gscene->setSelectionArea(allPath, Qt::IntersectsItemShape, this->transform()); */
}

/* . */
void
doc_selection_changed(int32_t doc)
{
    if (dockPropEdit->isVisible()) {
        dockPropEdit->setSelectedItems(documents[doc]->gscene->selectedItems());
    }
}

/* . */
void
Document::mouseDoubleClickEvent(QMouseEvent* event)
{
    if (event->button() == Qt::LeftButton) {
        QGraphicsItem* item = gscene->itemAt(mapToScene(event->pos()), QTransform());
        if (item) {
            dockPropEdit->show();
        }
    }
}

/* . */
void
Document::mousePressEvent(QMouseEvent* event)
{
    int32_t doc = data.id;
    DocumentData *data = doc_data(doc);
    doc_update_mouse_coords(doc, event->position().x(), event->position().y());
    if (event->button() == Qt::LeftButton) {
        if (cmdActive) {
            QPointF cmdPoint = documents[doc]->mapToScene(event->pos());
            runCommandClick((char*)qPrintable(curCmd), cmdPoint.x(), cmdPoint.y());
            return;
        }
        QPainterPath path;
        QList<QGraphicsItem*> pickList = documents[doc]->gscene->items(QRectF(documents[doc]->mapToScene(data->viewMousePoint.x-data->pickBoxSize, data->viewMousePoint.y-data->pickBoxSize),
                                                              documents[doc]->mapToScene(data->viewMousePoint.x+data->pickBoxSize, data->viewMousePoint.y+data->pickBoxSize)));

        bool itemsInPickBox = pickList.size();
        if (itemsInPickBox && !data->selectingActive && !data->grippingActive) {
            bool itemsAlreadySelected = pickList.at(0)->isSelected();
            if (!itemsAlreadySelected) {
                pickList.at(0)->setSelected(true);
            }
            else {
                bool foundGrip = false;
                Object* base = static_cast<Object*>(pickList.at(0)); /* TODO: Allow multiple objects to be gripped at once */
                if (!base) {
                    return;
                }

                QPoint qsnapOffset(data->qsnapLocatorSize, data->qsnapLocatorSize);
                QPointF gripPoint = to_qpointf(base->mouseSnapPoint(data->sceneMousePoint));
                QPoint p1 = mapFromScene(gripPoint) - qsnapOffset;
                QPoint q1 = mapFromScene(gripPoint) + qsnapOffset;
                QRectF gripRect = QRectF(documents[doc]->mapToScene(p1), documents[doc]->mapToScene(q1));
                QRectF pickRect = QRectF(documents[doc]->mapToScene(data->viewMousePoint.x -data->pickBoxSize, data->viewMousePoint.y - data->pickBoxSize),
                                        documents[doc]->mapToScene(data->viewMousePoint.x + data->pickBoxSize, data->viewMousePoint.y+data->pickBoxSize));
                if (gripRect.intersects(pickRect)) {
                    foundGrip = true;
                }

                /* If the pick point is within the item's grip box, start gripping */
                if (foundGrip) {
                    doc_start_gripping(doc, base);
                }
                else {
                    /* start moving */
                    data->movingActive = true;
                    data->pressPoint = to_emb_vector(event->pos());
                    data->scenePressPoint = doc_map_to_scene(doc, data->pressPoint);
                }
            }
        }
        else if (data->grippingActive) {
            doc_stop_gripping(doc, true);
        }
        else if (!data->selectingActive) {
            data->selectingActive = true;
            data->pressPoint = to_emb_vector(event->pos());
            data->scenePressPoint = doc_map_to_scene(doc, data->pressPoint);

            if (!documents[doc]->selectBox) {
                documents[doc]->selectBox = new SelectBox(QRubberBand::Rectangle, documents[doc]);
            }
            QPointF p1 = to_qpointf(data->pressPoint);
            documents[doc]->selectBox->setGeometry(QRect(p1.toPoint(), p1.toPoint()));
            documents[doc]->selectBox->show();
        }
        else {
            data->selectingActive = false;
            documents[doc]->selectBox->hide();
            data->releasePoint = to_emb_vector(event->pos());
            data->sceneReleasePoint = doc_map_to_scene(doc, data->releasePoint);

            /* Start SelectBox Code */
            path.addPolygon(documents[doc]->mapToScene(documents[doc]->selectBox->geometry()));
            if (data->sceneReleasePoint.x > data->scenePressPoint.x) {
                if (get_bool(SELECTION_MODE_PICKADD)) {
                    if (isShiftPressed()) {
                        QList<QGraphicsItem*> itemList = documents[doc]->gscene->items(path, Qt::ContainsItemShape);
                        foreach(QGraphicsItem* item, itemList)
                            item->setSelected(false);
                    }
                    else {
                        QList<QGraphicsItem*> itemList = documents[doc]->gscene->items(path, Qt::ContainsItemShape);
                        foreach(QGraphicsItem* item, itemList)
                            item->setSelected(true);
                    }
                }
                else {
                    if (isShiftPressed()) {
                        QList<QGraphicsItem*> itemList = documents[doc]->gscene->items(path, Qt::ContainsItemShape);
                        if (!itemList.size())
                            doc_clear_selection(doc);
                        else {
                            foreach(QGraphicsItem* item, itemList)
                                item->setSelected(!item->isSelected()); /* Toggle selected */
                        }
                    }
                    else {
                        doc_clear_selection(doc);
                        QList<QGraphicsItem*> itemList = documents[doc]->gscene->items(path, Qt::ContainsItemShape);
                        foreach(QGraphicsItem* item, itemList)
                            item->setSelected(true);
                    }
                }
            }
            else {
                if (get_bool(SELECTION_MODE_PICKADD)) {
                    if (isShiftPressed()) {
                        QList<QGraphicsItem*> itemList = documents[doc]->gscene->items(path, Qt::IntersectsItemShape);
                        foreach(QGraphicsItem* item, itemList)
                            item->setSelected(false);
                    }
                    else {
                        QList<QGraphicsItem*> itemList = documents[doc]->gscene->items(path, Qt::IntersectsItemShape);
                        foreach(QGraphicsItem* item, itemList)
                            item->setSelected(true);
                    }
                }
                else {
                    if (isShiftPressed()) {
                        QList<QGraphicsItem*> itemList = documents[doc]->gscene->items(path, Qt::IntersectsItemShape);
                        if (!itemList.size())
                            doc_clear_selection(doc);
                        else {
                            foreach (QGraphicsItem* item, itemList)
                                item->setSelected(!item->isSelected()); /* Toggle selected */
                        }
                    }
                    else {
                        doc_clear_selection(doc);
                        QList<QGraphicsItem*> itemList = documents[doc]->gscene->items(path, Qt::IntersectsItemShape);
                        foreach(QGraphicsItem* item, itemList)
                            item->setSelected(true);
                    }
                }
            }
            /* End SelectBox Code */
        }

        if (data->pastingActive) {
            QList<QGraphicsItem*> itemList = documents[doc]->pasteObjectItemGroup->childItems();
            documents[doc]->gscene->destroyItemGroup(documents[doc]->pasteObjectItemGroup);
            foreach(QGraphicsItem* item, itemList) {
                /* Prevent Qt Runtime Warning, QGraphicsScene::addItem: item has already been added to this scene */
                documents[doc]->gscene->removeItem(item);
            }

            documents[doc]->undoStack->beginMacro("Paste");
            foreach(QGraphicsItem* item, itemList) {
                Object* base = static_cast<Object*>(item);
                if (base) {
                    UndoableCommand* cmd = new UndoableCommand(ACTION_ADD, base->core->OBJ_NAME, base, doc, 0);
                    if (cmd) {
                        documents[doc]->undoStack->push(cmd);
                    }
                }
            }
            documents[doc]->undoStack->endMacro();

            data->pastingActive = false;
            data->selectingActive = false;
        }
        if (data->zoomWindowActive) {
            fitInView(path.boundingRect(), Qt::KeepAspectRatio);
            doc_clear_selection(doc);
        }
    }
    if (event->button() == Qt::MiddleButton) {
        //FIXME: doc_pan_start(doc, event->pos());
        /* The Undo command will record the spot where the pan started. */
        UndoableCommand* cmd = new UndoableCommand(ACTION_NAV, "PanStart", doc, 0);
        documents[doc]->undoStack->push(cmd);
        event->accept();
    }
    doc_update(doc);
}

/* . */
void
doc_pan_start(int32_t doc, const QPoint& point)
{
    DocumentData *data = doc_data(doc);
    doc_recalculate_limits(doc);

    doc_align_scene_point_with_view_point(doc, doc_map_to_scene(doc, to_emb_vector(point)), to_emb_vector(point));

    data->panningActive = true;
    data->panStartX = point.x();
    data->panStartY = point.y();
}

/* NOTE: Increase the sceneRect limits if the point we want to go to lies
 * outside of sceneRect's limits.
 * If the sceneRect limits aren't increased, you cannot pan past its limits.
 */
void
doc_recalculate_limits(int32_t doc)
{
    DocumentData *data = doc_data(doc);
    EmbVector tl = to_emb_vector(documents[doc]->mapToScene(documents[doc]->rect().topLeft()));
    EmbVector br = to_emb_vector(documents[doc]->mapToScene(documents[doc]->rect().bottomRight()));
    QRectF viewRect(to_qpointf(tl), to_qpointf(br));
    QRectF sceneRect(documents[doc]->sceneRect());
    QRectF newRect = viewRect.adjusted(-viewRect.width(), -viewRect.height(), viewRect.width(), viewRect.height());
    if (!sceneRect.contains(newRect.topLeft()) || !sceneRect.contains(newRect.bottomRight())) {
        documents[doc]->gscene->setSceneRect(sceneRect.adjusted(-viewRect.width(),
                                                -viewRect.height(),
                                                viewRect.width(),
                                                viewRect.height()));
    }
}

/* . */
void
doc_align_scene_point_with_view_point(int32_t doc, EmbVector scenePoint, EmbVector viewPoint)
{
    EmbVector viewCenter = doc_center(doc);
    EmbVector pointBefore = scenePoint;
    /* centerOn also updates the scrollbars, which shifts things out of wack o_O */
    doc_center_on(doc, viewCenter);
    /* Reshift to the new center so the scene and view points align */
    EmbVector pointAfter = doc_map_to_scene(doc, viewPoint);
    EmbVector offset = emb_vector_subtract(pointBefore, pointAfter);
    EmbVector newCenter = emb_vector_add(viewCenter, offset);
    doc_center_on(doc, newCenter);
}

/* . */
void
Document::mouseMoveEvent(QMouseEvent* event)
{
    int32_t doc = data.id;
    DocumentData *data = doc_data(doc);
    doc_update_mouse_coords(doc, event->position().x(), event->position().y());
    data->movePoint = to_emb_vector(event->pos());
    data->sceneMovePoint = doc_map_to_scene(doc, data->movePoint);

    if (cmdActive) {
        if (data->rapidMoveActive) {
            runCommandMove((char*)qPrintable(curCmd), data->sceneMovePoint.x,
                data->sceneMovePoint.y);
        }
    }
    if (data->previewActive) {
        if (data->previewMode == PREVIEW_MOVE) {
            QPointF p = to_qpointf(emb_vector_subtract(data->sceneMousePoint,
                data->previewPoint));
            documents[doc]->previewObjectItemGroup->setPos(p);
        }
        else if (data->previewMode == PREVIEW_ROTATE) {
            double x = data->previewPoint.x;
            double y = data->previewPoint.y;
            double rot = data->previewData;

            double mouseAngle = QLineF(x, y, data->sceneMousePoint.x, data->sceneMousePoint.y).angle();

            double rad = radians(rot-mouseAngle);
            double cosRot = cos(rad);
            double sinRot = sin(rad);
            EmbVector p, rotv;
            p.x = -x;
            p.y = -y;
            rotv.x = p.x*cosRot - p.y*sinRot;
            rotv.y = p.x*sinRot + p.y*cosRot;
            rotv.x += x;
            rotv.y += y;

            documents[doc]->previewObjectItemGroup->setPos(rotv.x, rotv.y);
            documents[doc]->previewObjectItemGroup->setRotation(rot-mouseAngle);
        }
        else if (data->previewMode == PREVIEW_SCALE) {
            double x = data->previewPoint.x;
            double y = data->previewPoint.y;
            double scaleFactor = data->previewData;

            double factor = QLineF(x, y, data->sceneMousePoint.x, data->sceneMousePoint.y).length() / scaleFactor;

            documents[doc]->previewObjectItemGroup->setScale(1);
            documents[doc]->previewObjectItemGroup->setPos(0,0);

            if (scaleFactor <= 0.0) {
                /* FIXME: messagebox("critical",
                    tr("ScaleFactor Error"),
                    tr("Hi there. If you are not a developer, report this as a bug. "
                        "If you are a developer, your code needs examined, and possibly your head too."));
                        */
            }
            else {
                /* Calculate the offset */
                double oldX = 0;
                double oldY = 0;
                QLineF scaleLine(x, y, oldX, oldY);
                scaleLine.setLength(scaleLine.length()*factor);
                double newX = scaleLine.x2();
                double newY = scaleLine.y2();

                double dx = newX - oldX;
                double dy = newY - oldY;

                documents[doc]->previewObjectItemGroup->setScale(documents[doc]->previewObjectItemGroup->scale()*factor);
                documents[doc]->previewObjectItemGroup->moveBy(dx, dy);
            }
        }
    }
    if (data->pastingActive) {
        EmbVector p = emb_vector_subtract(data->sceneMousePoint,
            data->pasteDelta);
        documents[doc]->pasteObjectItemGroup->setPos(to_qpointf(p));
    }
    if (data->movingActive) {
        /* Ensure that the preview is only shown if the mouse has moved. */
        if (!data->previewActive)
            doc_preview_on(doc, PREVIEW_CLONE_SELECTED, PREVIEW_MOVE,
                data->scenePressPoint.x, data->scenePressPoint.y, 0);
    }
    if (data->selectingActive) {
        /* FIXME:
        if (data->sceneMovePoint >= data->scenePressPoint) {
            documents[doc]->selectBox->setDirection(1);
        }
        else {
            documents[doc]->selectBox->setDirection(0);
        }
        QPointF p = documents[doc]->mapFromScene(to_qpointf(data->scenePressPoint));
        QRect rect = QRect(p, event->pos());
        documents[doc]->selectBox->setGeometry(rect).normalized();
        event->accept();
        */
    }
    if (data->panningActive) {
        documents[doc]->horizontalScrollBar()->setValue(
            documents[doc]->horizontalScrollBar()->value() - (event->position().x() - data->panStartX));
        documents[doc]->verticalScrollBar()->setValue(
            documents[doc]->verticalScrollBar()->value() - (event->position().y() - data->panStartY));
        data->panStartX = event->position().x();
        data->panStartY = event->position().y();
        event->accept();
    }
    doc_update(doc);
}

void
Document::mouseReleaseEvent(QMouseEvent* event)
{
    doc_update_mouse_coords(data.id, event->position().x(), event->position().y());
    if (event->button() == Qt::LeftButton) {
        if (data.movingActive) {
            doc_preview_off(data.id);
            EmbVector delta = emb_vector_subtract(data.sceneMousePoint,
                data.scenePressPoint);
            /* Ensure that moving only happens if the mouse has moved. */
            if (delta.x || delta.y) {
                doc_move_selected(data.id, delta.x, delta.y);
            }
            data.movingActive = false;
        }
        event->accept();
    }
    if (event->button() == Qt::MiddleButton) {
        data.panningActive = false;
        /* The Undo command will record the spot where the pan completed. */
        UndoableCommand* cmd = new UndoableCommand(ACTION_NAV, "PanStop",
            data.id, 0);
        undoStack->push(cmd);
        event->accept();
    }
    if (event->button() == Qt::XButton1) {
        debug_message("XButton1");
        undo_command(); /* TODO: Make this customizable */
        event->accept();
    }
    if (event->button() == Qt::XButton2) {
        debug_message("XButton2");
        redo_command(); /* TODO: Make this customizable */
        event->accept();
    }
    doc_update(data.id);
}

/* . */
void
Document::wheelEvent(QWheelEvent* event)
{
    // FIXME: int zoomDir = event->delta();
    QPoint mousePoint = event->position().toPoint();

    doc_update_mouse_coords(data.id, mousePoint.x(), mousePoint.y());
    /* FIXME:
    if (zoomDir > 0) {
        UndoableNavCommand* cmd = new UndoableNavCommand("ZoomInToPoint", data.id, 0);
        documents[doc]->undoStack->push(cmd);
    }
    else {
        UndoableNavCommand* cmd = new UndoableNavCommand("ZoomOutToPoint", data.id, 0);
        documents[doc]->undoStack->push(cmd);
    }
    */
}

/* . */
void
doc_zoom_to_point(int32_t doc, EmbVector mousePoint, int zoomDir)
{
    QPointF pointBeforeScale = to_qpointf(doc_map_to_scene(doc, mousePoint));
    DocumentData *data = doc_data(doc);

    /* Do The zoom */
    double s;
    if (zoomDir > 0) {
        if (!doc_allow_zoom_in(doc)) {
            return;
        }
        s = get_real(DISPLAY_ZOOMSCALE_IN);
    }
    else {
        if (!doc_allow_zoom_out(doc)) {
            return;
        }
        s = get_real(DISPLAY_ZOOMSCALE_OUT);
    }

    documents[doc]->scale(s, s);
    doc_align_scene_point_with_view_point(doc, to_emb_vector(pointBeforeScale), mousePoint);
    doc_recalculate_limits(doc);
    doc_align_scene_point_with_view_point(doc, to_emb_vector(pointBeforeScale), mousePoint);

    doc_update_mouse_coords(doc, mousePoint.x, mousePoint.y);
    if (data->pastingActive) {
        EmbVector p = emb_vector_subtract(data->sceneMousePoint,
            data->pasteDelta);
        documents[doc]->pasteObjectItemGroup->setPos(to_qpointf(p));
    }
    if (data->selectingActive) {
        QPointF v1 = documents[doc]->mapFromScene(to_qpointf(data->scenePressPoint));
        QPointF v2 = to_qpointf(mousePoint);
        QRectF r(v1, v2);
        /* FIXME: documents[doc]->selectBox->setGeometry(r.normalized()); */
    }
    doc_update(doc);
}

void
Document::contextMenuEvent(QContextMenuEvent* event)
{
    QMenu menu;
    QList<QGraphicsItem*> itemList = gscene->selectedItems();
    bool selectionEmpty = itemList.isEmpty();

    for (int i = 0; i < itemList.size(); i++) {
        Object *obj = static_cast<Object *>(itemList.at(i));
        if (obj->core->geometry->type != OBJ_NULL) {
            selectionEmpty = false;
            break;
        }
    }

    if (data.pastingActive) {
        return;
    }
    if (!cmdActive) {
        QAction* repeatAction = new QAction(create_icon(lastCmd), "Repeat " + lastCmd, this);
        repeatAction->setStatusTip("Repeats the previously issued command.");
        connect(repeatAction, SIGNAL(triggered()), this, SLOT(repeatAction()));
        menu.addAction(repeatAction);
    }
    if (data.zoomWindowActive) {
        QAction* cancelZoomWinAction = new QAction("&Cancel (ZoomWindow)", this);
        cancelZoomWinAction->setStatusTip("Cancels the ZoomWindow Command.");
        connect(cancelZoomWinAction, SIGNAL(triggered()), this, SLOT(escapePressed()));
        menu.addAction(cancelZoomWinAction);
    }

    menu.addSeparator();
    menu.addAction(actionHash[ACTION_CUT]);
    menu.addAction(actionHash[ACTION_COPY]);
    menu.addAction(actionHash[ACTION_PASTE]);
    menu.addSeparator();

    if (!selectionEmpty) {
        /* FIXME:
        QAction* deleteAction = new QAction(create_icon("erase"), "D&elete", this);
        deleteAction->setStatusTip("Removes objects from a drawing.");
        connect(deleteAction, SIGNAL(triggered()), this,
            [=]() { doc_delete_selected(doc); });
        menu.addAction(deleteAction);

        QAction* moveAction = new QAction(create_icon("move"), "&Move", this);
        moveAction->setStatusTip("Displaces objects a specified distance in a specified direction.");
        connect(moveAction, SIGNAL(triggered()), this, SLOT(move_action()));
        menu.addAction(moveAction);

        QAction* scaleAction = new QAction(create_icon("scale"), "Sca&le", this);
        scaleAction->setStatusTip("Enlarges or reduces objects proportionally in the X, Y, and Z directions.");
        connect(scaleAction, SIGNAL(triggered()), this, SLOT(scale_action()));
        menu.addAction(scaleAction);

        QAction* rotateAction = new QAction(create_icon("rotate"), "R&otate", this);
        rotateAction->setStatusTip("Rotates objects about a base point.");
        connect(rotateAction, SIGNAL(triggered()), this, SLOT(rotate_action()));
        menu.addAction(rotateAction);

        menu.addSeparator();

        QAction* clearAction = new QAction("Cle&ar Selection", this);
        clearAction->setStatusTip("Removes all objects from the selection set.");
        connect(clearAction, SIGNAL(triggered()), this, [=]() { doc_clear_selection(this); });
        menu.addAction(clearAction);
        */
    }

    menu.exec(event->globalPos());
}

/* . */
void
doc_deletePressed(int32_t doc)
{
    debug_message("View deletePressed()");
    DocumentData *data = doc_data(doc);
    if (data->pastingActive) {
        documents[doc]->gscene->removeItem(documents[doc]->pasteObjectItemGroup);
        delete documents[doc]->pasteObjectItemGroup;
    }
    data->pastingActive = false;
    data->zoomWindowActive = false;
    data->selectingActive = false;
    documents[doc]->selectBox->hide();
    doc_stop_gripping(doc, false);
    doc_delete_selected(doc);
}

/* . */
void
doc_escapePressed(int32_t doc)
{
    debug_message("View escapePressed()");
    DocumentData *data = doc_data(doc);
    if (data->pastingActive) {
        documents[doc]->gscene->removeItem(documents[doc]->pasteObjectItemGroup);
        delete documents[doc]->pasteObjectItemGroup;
    }
    data->pastingActive = false;
    data->zoomWindowActive = false;
    data->selectingActive = false;
    documents[doc]->selectBox->hide();
    if (data->grippingActive) {
        doc_stop_gripping(doc, false);
    }
    else {
        doc_clear_selection(doc);
    }
}

/* . */
void
doc_start_gripping(int32_t doc, Object* obj)
{
    if (!obj) {
        return;
    }
    DocumentData *data = doc_data(doc);
    data->grippingActive = true;
    documents[doc]->gripBaseObj = obj;
    data->sceneGripPoint = documents[doc]->gripBaseObj->mouseSnapPoint(data->sceneMousePoint);
    documents[doc]->gripBaseObj->setObjectRubberPoint("GRIP_POINT", data->sceneGripPoint);
    obj_set_rubber_mode(documents[doc]->gripBaseObj->core, RUBBER_GRIP);
}

/* . */
void
doc_stop_gripping(int32_t doc, bool accept)
{
    DocumentData *data = doc_data(doc);
    data->grippingActive = false;
    if (documents[doc]->gripBaseObj) {
        documents[doc]->gripBaseObj->vulcanize();
        if (accept) {
            QString s = translate("Grip Edit ");
            s += documents[doc]->gripBaseObj->core->OBJ_NAME;
            UndoableCommand* cmd = new UndoableCommand(ACTION_GRIP_EDIT, data->sceneGripPoint, data->sceneMousePoint, s, documents[doc]->gripBaseObj, doc, 0);
            if (cmd) {
                documents[doc]->undoStack->push(cmd);
            }
            /* Update the Property Editor */
            doc_selection_changed(doc);
        }
        documents[doc]->gripBaseObj = 0;
    }
    /* Move the data->sceneGripPoint to a place where it will never be hot. */
    data->sceneGripPoint = to_emb_vector(documents[doc]->sceneRect().topLeft());
}

/* . */
void
doc_clear_selection(int32_t doc)
{
    // FIXME: documents[doc]->gscene->clearSelection();
}

/* . */
void
doc_delete_selected(int32_t doc)
{
    DocumentData *data = doc_data(doc);
    QList<QGraphicsItem*> itemList = documents[doc]->gscene->selectedItems();
    int numSelected = itemList.size();
    if (numSelected > 1) {
        documents[doc]->undoStack->beginMacro("Delete " + QString().setNum(itemList.size()));
    }
    for (int i = 0; i < itemList.size(); i++) {
        Object* base = static_cast<Object*>(itemList.at(i));
        if (base) {
            if (base->core->geometry->type != OBJ_NULL) {
                char label[MAX_STRING_LENGTH];
                sprintf(label, "%s%s", translate("Delete 1 "), base->core->OBJ_NAME);
                UndoableCommand* cmd = new UndoableCommand(ACTION_DELETE, label,
                    base, doc, 0);
                if (cmd) {
                    documents[doc]->undoStack->push(cmd);
                }
            }
        }
    }
    if (numSelected > 1) {
        documents[doc]->undoStack->endMacro();
    }
}

/* . */
void
doc_cut(int32_t doc)
{
    if (documents[doc]->gscene->selectedItems().isEmpty()) {
        information_box(translate("Cut Preselect"),
            translate("Preselect objects before invoking the cut command."));
        return; /* TODO: Prompt to select objects if nothing is preselected */
    }

    documents[doc]->undoStack->beginMacro("Cut");
    doc_copy_selected(doc);
    doc_delete_selected(doc);
    documents[doc]->undoStack->endMacro();
}

/* . */
void
doc_copy(int32_t doc)
{
    if (documents[doc]->gscene->selectedItems().isEmpty()) {
        information_box(translate("Copy Preselect"),
            translate("Preselect objects before invoking the copy command."));
        return; /* TODO: Prompt to select objects if nothing is preselected */
    }

    doc_copy_selected(doc);
    doc_clear_selection(doc);
}

/* . */
void
doc_copy_selected(int32_t doc)
{
    QList<QGraphicsItem*> selectedList = documents[doc]->gscene->selectedItems();

    /* Prevent memory leaks by deleting any unpasted instances */
    qDeleteAll(cutCopyObjectList.begin(), cutCopyObjectList.end());
    cutCopyObjectList.clear();

    /* Create new objects but do not add them to the scene just yet.
     * By creating them now, ensures that pasting will still work
     * if the original objects are deleted before the paste occurs.
     */
    cutCopyObjectList = doc_create_object_list(doc, selectedList);
}

/* . */
void
doc_paste(int32_t doc)
{
    DocumentData *data = doc_data(doc);
    if (data->pastingActive) {
        documents[doc]->gscene->removeItem(documents[doc]->pasteObjectItemGroup);
        delete documents[doc]->pasteObjectItemGroup;
    }

    documents[doc]->pasteObjectItemGroup = documents[doc]->gscene->createItemGroup(cutCopyObjectList);
    data->pasteDelta = to_emb_vector(documents[doc]->pasteObjectItemGroup->boundingRect().bottomLeft());
    EmbVector p = emb_vector_subtract(data->sceneMousePoint, data->pasteDelta);
    documents[doc]->pasteObjectItemGroup->setPos(to_qpointf(p));
    data->pastingActive = true;

    /* Re-create the list in case of multiple pastes. */
    cutCopyObjectList = doc_create_object_list(doc, cutCopyObjectList);
}

/* . */
QList<QGraphicsItem*>
doc_create_object_list(int32_t doc, QList<QGraphicsItem*> list)
{
    QList<QGraphicsItem*> copyList;

    for (int i = 0; i < list.size(); i++) {
        QGraphicsItem* item = list.at(i);
        if (!item) {
            continue;
        }

        Object* obj = static_cast<Object*>(item);
        if (obj) {
            Object* copyObj = copy_object(obj);
            copyList.append(copyObj);
        }
    }

    return copyList;
}

/* . */
void
repeat_action(void)
{
    promptInput->endCommand();
    prompt->setCurrentText(lastCmd);
    promptInput->processInput();
}

/* . */
void
move_action(void)
{
    promptInput->endCommand();
    prompt->setCurrentText("move");
    promptInput->processInput();
}

/* . */
void
doc_move_selected(int32_t doc, double dx, double dy)
{
    QList<QGraphicsItem*> itemList = documents[doc]->gscene->selectedItems();
    int numSelected = itemList.size();
    if (numSelected > 1) {
        documents[doc]->undoStack->beginMacro("Move " + QString().setNum(itemList.size()));
    }
    foreach(QGraphicsItem* item, itemList) {
        Object* base = static_cast<Object*>(item);
        if (base) {
            char msg[MAX_STRING_LENGTH];
            EmbVector delta = emb_vector(dx, dy);
            sprintf(msg, "%s%s", translate("Move 1 "), base->core->OBJ_NAME);
            UndoableCommand* cmd = new UndoableCommand(ACTION_MOVE, delta, msg,
                base, doc, 0);
            if (cmd) {
                documents[doc]->undoStack->push(cmd);
            }
        }
    }
    if (numSelected > 1) {
        documents[doc]->undoStack->endMacro();
    }

    /* Always clear the selection after a move. */
    documents[doc]->gscene->clearSelection();
}

/* . */
void
rotate_action(void)
{
    promptInput->endCommand();
    prompt->setCurrentText("rotate");
    promptInput->processInput();
}

/* . */
void
doc_rotate_selected(int32_t doc, double x, double y, double rot)
{
    QList<QGraphicsItem*> itemList = documents[doc]->gscene->selectedItems();
    int numSelected = itemList.size();
    if (numSelected > 1) {
        documents[doc]->undoStack->beginMacro("Rotate " + QString().setNum(itemList.size()));
    }
    foreach(QGraphicsItem* item, itemList) {
        Object* base = static_cast<Object*>(item);
        if (base) {
            QString s = translate("Rotate 1 ");
            s += base->core->OBJ_NAME;
            EmbVector v = emb_vector(x, y);
            UndoableCommand* cmd = new UndoableCommand(ACTION_ROTATE, v, rot, s,
                base, doc, 0);
            if (cmd) {
                documents[doc]->undoStack->push(cmd);
            }
        }
    }
    if (numSelected > 1) {
        documents[doc]->undoStack->endMacro();
    }

    /* Always clear the selection after a rotate. */
    documents[doc]->gscene->clearSelection();
}

/* . */
void
doc_mirror_selected(int32_t doc, double x1, double y1, double x2, double y2)
{
    QList<QGraphicsItem*> itemList = documents[doc]->gscene->selectedItems();
    int numSelected = itemList.size();
    if (numSelected > 1)
        documents[doc]->undoStack->beginMacro("Mirror " + QString().setNum(itemList.size()));
    foreach(QGraphicsItem* item, itemList) {
        Object* base = static_cast<Object*>(item);
        if (base) {
            EmbVector start, end;
            start = emb_vector(x1, y1);
            end = emb_vector(x2, y2);
            QString s = translate("Mirror 1 ");
            s += base->core->OBJ_NAME;
            UndoableCommand* cmd = new UndoableCommand(ACTION_MIRROR, start, end,
                s, base, doc, 0);
            if (cmd) {
                documents[doc]->undoStack->push(cmd);
            }
        }
    }
    if (numSelected > 1) {
        documents[doc]->undoStack->endMacro();
    }

    /* Always clear the selection after a mirror. */
    documents[doc]->gscene->clearSelection();
}

/* . */
void
doc_scaleAction()
{
    promptInput->endCommand();
    prompt->setCurrentText("scale");
    promptInput->processInput();
}

/* . */
void
doc_scale_selected(int32_t doc, double x, double y, double factor)
{
    QList<QGraphicsItem*> itemList = documents[doc]->gscene->selectedItems();
    int numSelected = itemList.size();
    if (numSelected > 1)
        documents[doc]->undoStack->beginMacro("Scale " + QString().setNum(itemList.size()));
    foreach(QGraphicsItem* item, itemList) {
        Object* base = static_cast<Object*>(item);
        if (base) {
            EmbVector v = emb_vector(x, y);
            char msg[MAX_STRING_LENGTH];
            sprintf(msg, "%s%s", translate("Scale 1 "), base->core->OBJ_NAME);
            UndoableCommand* cmd = new UndoableCommand(ACTION_SCALE, v, factor, msg,
                base, doc, 0);
            if (cmd) {
                documents[doc]->undoStack->push(cmd);
            }
        }
    }
    if (numSelected > 1) {
        documents[doc]->undoStack->endMacro();
    }

    /* Always clear the selection after a scale. */
    documents[doc]->gscene->clearSelection();
}

/* . */
int
doc_num_selected(int32_t doc)
{
    return documents[doc]->gscene->selectedItems().size();
}

/* . */
void
doc_show_scroll_bars(int32_t doc, bool val)
{
    if (val) {
        documents[doc]->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOn);
        documents[doc]->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOn);
    }
    else {
        documents[doc]->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
        documents[doc]->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    }
}

/* . */
void
doc_set_background_color(int32_t doc, uint32_t color)
{
    DocumentData *data = doc_data(doc);
    documents[doc]->setBackgroundBrush(QColor(color));
    data->backgroundColor = color;
    doc_update(doc);
}

/* . */
void
doc_set_select_box_colors(int32_t doc, QRgb colorL, QRgb fillL, QRgb colorR, QRgb fillR, int alpha)
{
    documents[doc]->selectBox->setColors(QColor(colorL), QColor(fillL), QColor(colorR), QColor(fillR), alpha);
}

/*
 * Undo
 */

/* . */
UndoEditor::UndoEditor(QString iconDirectory, QWidget* widgetToFocus, QWidget* parent) : QDockWidget(parent)
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
void
UndoEditor::updateCleanIcon(bool opened)
{
    if (opened) {
        undoView->setEmptyLabel(tr("Open"));
        undoView->setCleanIcon(create_icon("open"));
    }
    else {
        undoView->setEmptyLabel(tr("New"));
        undoView->setCleanIcon(create_icon("new"));
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

/* . */
bool
ImageWidget::load(QString fileName)
{
    img.load(fileName);
    return true;
}

/* . */
bool
ImageWidget::save(QString fileName)
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
            _main, [](void) { settingsDialog("Snap"); });
        menu.addAction(action);
    }
    else if (object->objectName() == "StatusBarButtonGRID") {
        QAction* action = new QAction(create_icon("gridsettings"), "&Settings...", &menu);
        QObject::connect(action, &QAction::trigger,
            _main, [](void) { settingsDialog("Grid/Ruler"); });
        menu.addAction(action);
    }
    else if (object->objectName() == "StatusBarButtonRULER") {
        QAction* action = new QAction(create_icon("rulersettings"), "&Settings...", &menu);
        QObject::connect(action, &QAction::trigger,
            _main, [](void) { settingsDialog("Grid/Ruler"); });
        menu.addAction(action);
    }
    else if (object->objectName() == "StatusBarButtonORTHO") {
        QAction* action = new QAction(create_icon("orthosettings"), "&Settings...", &menu);
        QObject::connect(action, &QAction::trigger,
            _main, [](void) { settingsDialog("Ortho/Polar"); });
        menu.addAction(action);
    }
    else if (object->objectName() == "StatusBarButtonPOLAR") {
        QAction* action = new QAction(create_icon("polarsettings"), "&Settings...", &menu);
        QObject::connect(action, &QAction::trigger,
            _main, [](void) { settingsDialog("Ortho/Polar"); });
        menu.addAction(action);
    }
    else if (object->objectName() == "StatusBarButtonQSNAP") {
        QAction* action = new QAction(create_icon("qsnapsettings"), "&Settings...", &menu);
        QObject::connect(action, &QAction::trigger,
            _main, [](void) { settingsDialog("QuickSnap"); });
        menu.addAction(action);
    }
    else if (object->objectName() == "StatusBarButtonQTRACK") {
        QAction* action = new QAction(create_icon("qtracksettings"), "&Settings...", &menu);
        QObject::connect(action, &QAction::trigger,
            _main, [](void) { settingsDialog("QuickTrack"); });
        menu.addAction(action);
    }
    else if (object->objectName() == "StatusBarButtonLWT") {
        int32_t doc = activeDocument();
        if (doc >= 0) {
            QAction* enableRealAction = new QAction(create_icon("realrender"), "&RealRender On", &menu);
            enableRealAction->setEnabled(!documents[doc]->data.enableReal);
            QObject::connect(enableRealAction, &QAction::triggered, _main, enableReal);
            menu.addAction(enableRealAction);

            QAction* disableRealAction = new QAction(create_icon("realrender"), "&RealRender Off", &menu);
            disableRealAction->setEnabled(documents[doc]->data.enableReal);
            QObject::connect(disableRealAction, &QAction::triggered, _main, disableReal);
            menu.addAction(disableRealAction);
        }

        QAction* action = new QAction(create_icon("lineweightsettings"), "&Settings...", &menu);
        QObject::connect(action, &QAction::trigger,
            _main, [](void) { settingsDialog("LineWeight"); });
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
    int32_t doc = activeDocument();
    if (doc >= 0) {
        if (key == "SNAP") {
            doc_toggle_snap(doc, on);
        }
        else if (key == "GRID") {
            doc_toggle_grid(doc, on);
        }
        else if (key == "RULER") {
            doc_toggle_ruler(doc, on);
        }
        else if (key == "ORTHO") {
            doc_toggle_ortho(doc, on);
        }
        else if (key == "POLAR") {
            doc_toggle_polar(doc, on);
        }
        else if (key == "QSNAP") {
            doc_toggle_qsnap(doc, on);
        }
        else if (key == "LWT") {
            doc_toggle_lwt(doc, on);
        }
    }
}

void
create_statusbar(MainWindow* mw)
{
    statusbar = new QStatusBar(mw);
    statusbar->setObjectName("StatusBar");

    statusBarMouseCoord = new QLabel(statusbar);

    statusBarMouseCoord->setMinimumWidth(300); /* Must fit this text always */
    statusBarMouseCoord->setMaximumWidth(300); /* "+1.2345E+99, +1.2345E+99, +1.2345E+99" */
    statusbar->addWidget(statusBarMouseCoord);

    for (int i=0; i<N_SB_BUTTONS; i++) {
        statusBarButtons[i] = new QToolButton(mw);
        QString name = button_list[i];
        statusBarButtons[i]->setText(name);
        statusBarButtons[i]->setObjectName("StatusBarButton" + name);
        statusBarButtons[i]->setAutoRaise(true);
        statusBarButtons[i]->setCheckable(true);
        QObject::connect(statusBarButtons[i], &QAbstractButton::toggled,
            statusBarButtons[i], [name] (bool b) { statusbar_toggle(name, b); });
        statusbar->addWidget(statusBarButtons[i]);
    }
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
LayerManager::addLayer(QString  name,
                            const bool visible,
                            const bool frozen,
                            const double zValue,
                            const QRgb color,
                            QString  lineType,
                            QString  lineWeight,
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

/* . */
QLabel *
create_tr_label(char *label, QDialog *dialog)
{
    return new QLabel(translate(label), dialog);
}

/* . */
QLabel *
create_int_label(uint32_t value, QDialog *dialog)
{
    return new QLabel(QString::number(value), dialog);
}

/* TODO: Move majority of this code into libembroidery.
 * Originally this counted number of maximum stitches but this is covered
 * by the histograms.
 */
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
    uint32_t stitchesReal = emb_pattern_real_count(pattern);
    uint32_t stitchesJump = emb_pattern_count_type(pattern, JUMP);
    uint32_t stitchesTrim = emb_pattern_count_type(pattern, TRIM);
    uint32_t colorTotal = pattern->thread_list->count;
    uint32_t colorChanges = emb_pattern_count_type(pattern, STOP);
    double min_stitchlength = emb_pattern_shortest_stitch(pattern);
    double max_stitchlength = emb_pattern_longest_stitch(pattern);
    double total_stitchlength = emb_total_thread_length(pattern);

    EmbRect bounds = emb_pattern_calcBoundingBox(pattern);

    if (pattern->stitch_list->count == 0) {
        warning_box(
            translate("No Design Loaded"),
            translate("<b>A design needs to be loaded or created before details can be determined.</b>"));
        return;
    }

    int bin[11];
    emb_length_histogram(pattern, bin);

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
        QFrame *frame = new QFrame();
        frame->setGeometry(0, 0, 30, 30);
        QPalette palette = frame->palette();
        /*
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

    QLabel* fieldRectLeft = new QLabel(QString::number(boundingRect.left()) + " mm", dialog);
    QLabel* fieldRectTop = new QLabel(QString::number(boundingRect.top()) + " mm", dialog);
    QLabel* fieldRectRight = new QLabel(QString::number(boundingRect.right()) + " mm", dialog);
    QLabel* fieldRectBottom = new QLabel(QString::number(boundingRect.bottom()) + " mm", dialog);
    QLabel* fieldRectWidth = new QLabel(QString::number(boundingRect.width()) + " mm", dialog);
    QLabel* fieldRectHeight = new QLabel(QString::number(boundingRect.height()) + " mm", dialog);

    QGridLayout* gridLayoutMisc = new QGridLayout(groupBoxMisc);
    gridLayoutMisc->addWidget(create_tr_label("Total Stitches:", dialog), 0, 0, Qt::AlignLeft);
    gridLayoutMisc->addWidget(create_int_label(stitchesTotal, dialog), 0, 1, Qt::AlignLeft);
    gridLayoutMisc->addWidget(create_tr_label("Real Stitches:", dialog), 1, 0, Qt::AlignLeft);
    gridLayoutMisc->addWidget(create_int_label(stitchesReal, dialog), 1, 1, Qt::AlignLeft);
    gridLayoutMisc->addWidget(create_tr_label("Jump Stitches:", dialog), 2, 0, Qt::AlignLeft);
    gridLayoutMisc->addWidget(create_int_label(stitchesJump, dialog), 2, 1, Qt::AlignLeft);
    gridLayoutMisc->addWidget(create_tr_label("Trim Stitches:", dialog), 3, 0, Qt::AlignLeft);
    gridLayoutMisc->addWidget(create_int_label(stitchesTrim, dialog), 3, 1, Qt::AlignLeft);
    gridLayoutMisc->addWidget(create_tr_label("Total Stitches:", dialog), 4, 0, Qt::AlignLeft);
    gridLayoutMisc->addWidget(create_int_label(colorTotal, dialog), 4, 1, Qt::AlignLeft);
    gridLayoutMisc->addWidget(create_tr_label("Color Changes:", dialog), 5, 0, Qt::AlignLeft);
    gridLayoutMisc->addWidget(create_int_label(colorChanges, dialog), 5, 1, Qt::AlignLeft);
    gridLayoutMisc->addWidget(create_tr_label("Left:", dialog), 6, 0, Qt::AlignLeft);
    gridLayoutMisc->addWidget(fieldRectLeft, 6, 1, Qt::AlignLeft);
    gridLayoutMisc->addWidget(create_tr_label("Top:", dialog), 7, 0, Qt::AlignLeft);
    gridLayoutMisc->addWidget(fieldRectTop, 7, 1, Qt::AlignLeft);
    gridLayoutMisc->addWidget(create_tr_label("Right:", dialog), 8, 0, Qt::AlignLeft);
    gridLayoutMisc->addWidget(fieldRectRight, 8, 1, Qt::AlignLeft);
    gridLayoutMisc->addWidget(create_tr_label("Bottom:", dialog), 9, 0, Qt::AlignLeft);
    gridLayoutMisc->addWidget(fieldRectBottom, 9, 1, Qt::AlignLeft);
    gridLayoutMisc->addWidget(create_tr_label("Width:", dialog), 10, 0, Qt::AlignLeft);
    gridLayoutMisc->addWidget(fieldRectWidth, 10, 1, Qt::AlignLeft);
    gridLayoutMisc->addWidget(create_tr_label("Height:", dialog), 11, 0, Qt::AlignLeft);
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
                             QString  caption,
                             QString  dir,
                             QString  filter) : QFileDialog(parent, caption, dir, filter)
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

/* . */
MdiArea::~MdiArea()
{
}

/* . */
void
MdiArea::useBackgroundLogo(bool use)
{
    useLogo = use;
    forceRepaint();
}

/* . */
void
MdiArea::useBackgroundTexture(bool use)
{
    useTexture = use;
    forceRepaint();
}

/* . */
void
MdiArea::useBackgroundColor(bool use)
{
    useColor = use;
    forceRepaint();
}

/* . */
void
MdiArea::setBackgroundLogo(QString  fileName)
{
    bgLogo.load(fileName);

    forceRepaint();
}

/* . */
void
MdiArea::setBackgroundTexture(QString  fileName)
{
    bgTexture.load(fileName);

    forceRepaint();
}

/* . */
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

/* . */
void
MdiArea::mouseDoubleClickEvent(QMouseEvent* /*e*/)
{
    openFile(false, "");
}

/* . */
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
            int32_t doc_index = mdiWin->doc_index;
            if (doc_index) {
                doc_recalculate_limits(doc_index);
                doc_zoom_extents(doc_index);
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

    setAttribute(Qt::WA_DeleteOnClose);

    this->setWindowIcon(create_icon("app"));

    gscene = new QGraphicsScene(0,0,0,0, this);
    /* FIXME:. */
    doc_index = numOfDocs;
    documents[doc_index] = create_doc(_main, gscene, this);

    documents[doc_index]->data.myIndex = theIndex;
    documents[doc_index]->data.fileWasLoaded = false;
    sprintf(documents[doc_index]->data.curFile,
        "Untitled%d.dst", theIndex);
    this->setWindowTitle(documents[doc_index]->data.curFile);

    setWidget(documents[doc_index]);

    /* WARNING: DO NOT SET THE QMDISUBWINDOW (this) FOCUSPROXY TO THE PROMPT
     * WARNING: AS IT WILL CAUSE THE WINDOW MENU TO NOT SWITCH WINDOWS PROPERLY!
     * WARNING: ALTHOUGH IT SEEMS THAT SETTING INTERNAL WIDGETS FOCUSPROXY IS OK.
     */
    documents[doc_index]->setFocusProxy(prompt);

    resize(sizeHint());

    promptHistory = "Welcome to Embroidermodder 2!<br/>Open some of our sample files. Many formats are supported.<br/>For help, press F1.";
    setHistory(promptHistory);
    promptInputList << "";
    promptInputNum = 0;

    strcpy(documents[doc_index]->data.curLayer, "0");
    documents[doc_index]->data.curColor = 0; /* TODO: color ByLayer */
    strcpy(documents[doc_index]->data.curLineType, "ByLayer");
    strcpy(documents[doc_index]->data.curLineWeight, "ByLayer");

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
MdiWindow::saveFile(QString fileName)
{
    return pattern_save(pattern, (char*)qPrintable(fileName));
}

bool
MdiWindow::loadFile(QString fileName)
{
    debug_message("MdiWindow loadFile()");

    QRgb tmpColor = documents[doc_index]->data.curColor;

    QFile file(fileName);
    if (!file.open(QFile::ReadOnly | QFile::Text)) {
        QMessageBox::warning(this, tr("Error reading file"),
            tr("Cannot read file %1:\n%2.").arg(fileName).arg(file.errorString()));
        return false;
    }

    wait_cursor();

    QString ext = fileExtension(fileName);
    debug_message("ext: ");
    debug_message((char*)qPrintable(ext));

    /* Read */
    int format = EMB_FORMAT_CSV; /* emb_identify_format(qPrintable(fileName)); */
    if (format <= 0) {
        debug_message("Unsupported read file type: ");
        debug_message((char*)qPrintable(fileName));
        restore_cursor();
        QMessageBox::warning(this, tr("Error reading pattern"),
            tr("Unsupported read file type: ") + qPrintable(fileName));
        return false;
    }

    int readSuccessful = emb_pattern_read(pattern, qPrintable(fileName), format);
    if (!readSuccessful) {
        debug_message("Reading file was unsuccessful:");
        debug_message((char*)qPrintable(fileName));
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

    if (get_bool(GRID_LOAD_FROM_FILE)) {
        /* TODO: Josh, provide me a hoop size and/or grid spacing from the pattern. */
    }

    restore_cursor();

    /* Clear the undo stack so it is not possible to undo past this point. */
    documents[doc_index]->undoStack->clear();

    documents[doc_index]->data.curColor = tmpColor;

    documents[doc_index]->data.fileWasLoaded = true;
    setUndoCleanIcon(documents[doc_index]->data.fileWasLoaded);
    return documents[doc_index]->data.fileWasLoaded;
}

void
MdiWindow::print()
{
    QPrintDialog dialog(&printer, this);
    if (dialog.exec() == QDialog::Accepted) {
        QPainter painter(&printer);
        if (get_bool(PRINTING_DISABLE_BG)) {
            /* Save current bg */
            QBrush brush = documents[doc_index]->backgroundBrush();
            /* Save ink by not printing the bg at all */
            documents[doc_index]->setBackgroundBrush(Qt::NoBrush);
            /* Print, fitting the viewport contents into a full page */
            documents[doc_index]->render(&painter);
            /* Restore the bg */
            documents[doc_index]->setBackgroundBrush(brush);
        }
        else {
            /* Print, fitting the viewport contents into a full page */
            documents[doc_index]->render(&painter);
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
    if (get_bool(PRINTING_DISABLE_BG)) {
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
MdiWindow::setCurrentFile(QString fileName)
{
    strcpy(documents[doc_index]->data.curFile,
        qPrintable(QFileInfo(fileName).canonicalFilePath()));
    setWindowModified(false);
    setWindowTitle(getShortCurrentFile());
}

QString
MdiWindow::getShortCurrentFile()
{
    return QFileInfo(documents[doc_index]->data.curFile).fileName();
}

QString MdiWindow::fileExtension(QString  fileName)
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
    // FIXME: documents[doc]->undoStack->setActive(true);
    DocumentData *data = doc_data(doc_index);
    setUndoCleanIcon(data->fileWasLoaded);
    statusBarButtons[SB_SNAP]->setChecked(data->enableSnap);
    statusBarButtons[SB_GRID]->setChecked(data->enableGrid);
    statusBarButtons[SB_RULER]->setChecked(data->enableRuler);
    statusBarButtons[SB_ORTHO]->setChecked(data->enableOrtho);
    statusBarButtons[SB_POLAR]->setChecked(data->enablePolar);
    statusBarButtons[SB_QSNAP]->setChecked(data->enableQSnap);
    statusBarButtons[SB_QTRACK]->setChecked(data->enableQTrack);
    statusBarButtons[SB_LWT]->setChecked(data->enableLwt);
    setHistory(promptHistory);
}

QSize
MdiWindow::sizeHint() const
{
    debug_message("MdiWindow sizeHint()");
    return QSize(450, 300);
}

/* . */
void
MdiWindow::updateColorLinetypeLineweight()
{
}

void
MdiWindow::promptHistoryAppended(QString  txt)
{
    promptHistory.append("<br/>" + txt);
}

void MdiWindow::logPromptInput(QString  txt)
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
            critical_box(translate("Prompt Previous Error"),
                translate("The prompt input is empty! Please report this as a bug!"));
        }
        else {
            critical_box(translate("Prompt Next Error"),
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
setHistory(QString  txt)
{
    promptHistory->setHtml(txt);
    promptHistory->moveCursor(QTextCursor::End, QTextCursor::MoveAnchor);
}

void
Application::setMainWin(MainWindow* mainWin)
{
    _main = mainWin;
}

void
CmdPrompt::setCurrentText(QString txt)
{
    curText = prefix + txt;
    promptInput->setText(curText);
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

    strcpy(prompt_color_, "#000000"); /* Match --------------------| */
    strcpy(prompt_selection_bg_color_, "#000000"); /* Match -------| */
    strcpy(prompt_bg_color_, "#FFFFFF");
    strcpy(prompt_selection_color_, "#FFFFFF");
    set_str(PROMPT_FONT_FAMILY, "Monospace");
    set_str(PROMPT_FONT_STYLE, "normal");
    set_int(PROMPT_FONT_SIZE, 12);

    updateStyle();

    blinkState = false;
    blinkTimer = new QTimer(this);
    connect(blinkTimer, SIGNAL(timeout()), this, SLOT(blink()));

    this->show();

    connect(promptInput, SIGNAL(showSettings()), this, SIGNAL(showSettings()));
}

/* . */
void
CmdPrompt::saveHistory(QString  fileName, bool html)
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
CmdPrompt::alert(QString  txt)
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
setPromptTextColor(uint32_t color)
{
    strcpy(prompt_color_, (char*)qPrintable(QColor(color).name()));
    strcpy(prompt_selection_bg_color_, (char*)qPrintable(QColor(color).name()));
    prompt->updateStyle();
}

/* . */
void
setPromptBackgroundColor(uint32_t color)
{
    strcpy(prompt_bg_color_, (char*)qPrintable(QColor(color).name()));
    strcpy(prompt_selection_color_, (char*)qPrintable(QColor(color).name()));
    prompt->updateStyle();
}

/* . */
void
setPromptFontFamily(EmbString family)
{
    set_str(PROMPT_FONT_FAMILY, (char*)qPrintable(family));
    prompt->updateStyle();
}

/* . */
void
setPromptFontStyle(EmbString style)
{
    set_str(PROMPT_FONT_STYLE, (char*)qPrintable(style));
    prompt->updateStyle();
}

/* . */
void
setPromptFontSize(int size)
{
    set_int(PROMPT_FONT_SIZE, size);
    prompt->updateStyle();
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
        get_str(PROMPT_FONT_FAMILY),
        get_str(PROMPT_FONT_STYLE),
        get_int(PROMPT_FONT_SIZE));

    this->setStyleSheet(QString(style_string));
}

/* . */
void
CmdPrompt::setPrefix(QString  txt)
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
    connect(this, SIGNAL(textEdited(QString )), this, SLOT(checkEditedText(QString )));
    connect(this, SIGNAL(textChanged(QString )), this, SLOT(checkChangedText(QString )));
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
            runCommandPrompt((char*)qPrintable(curCmd));
        }
        else if (cmdtxt.isEmpty()) {
            cmdActive = true;
            appendHistory(curText);
            /* Rerun the last successful command. */
            runCommandPrompt((char*)qPrintable(lastCmd));
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
CmdPromptInput::updateCurrentText(QString  txt)
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
CmdPromptInput::checkEditedText(QString  txt)
{
    updateCurrentText(txt);

    if (rapidFireEnabled) {
        processInput();
    }
}

/* . */
void
CmdPromptInput::checkChangedText(QString  txt)
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


/* . */
EmbVector
to_emb_vector(QPointF p)
{
    EmbVector v;
    v.x = p.x();
    v.y = p.y();
    return v;
}

/* . */
QPointF
to_qpointf(EmbVector v)
{
    QPointF p(v.x, v.y);
    return p;
}

/* . */
EmbVector
find_mouse_snap_point(QList<EmbVector> snap_points, EmbVector mouse_point)
{
    float closest = 1.0e10;
    EmbVector result = snap_points[0];
    int i;
    for (i=0; i<snap_points.count(); i++) {
        float distance = emb_vector_distance(snap_points[i], mouse_point);
        if (distance < closest) {
            closest = distance;
            result = snap_points[i];
        }
    }
    return result;
}

/* . */
EmbArc
emb_arc_set_radius(EmbArc arc, EmbReal radius)
{
    EmbGeometry geometry;
    geometry.object.arc = arc;
    radius = EMB_MAX(radius, 0.0000001);
    EmbVector center = emb_arc_center(geometry);

    EmbVector start = emb_vector_subtract(center, arc.start);
    start = emb_vector_scale(start, radius/emb_vector_length(start));
    arc.start = emb_vector_add(center, start);

    EmbVector mid = emb_vector_subtract(center, arc.mid);
    mid = emb_vector_scale(mid, radius/emb_vector_length(mid));
    arc.mid = emb_vector_add(center, mid);

    EmbVector end = emb_vector_subtract(center, arc.end);
    end = emb_vector_scale(start, radius/emb_vector_length(end));
    arc.end = emb_vector_add(center, end);

    return arc;
}

/* TODO: choose a default icon. */
QPixmap
create_pixmap(QString icon)
{
    int id = 0;
    int n = string_array_length(xpm_icon_labels);
    for (int i=0; i<n; i++) {
        if (string_equal((char*)qPrintable(icon), xpm_icon_labels[i])) {
            id = i;
            break;
        }
    }
    QPixmap pixmap(xpm_icons[id]);
    return pixmap;
}

/* . */
QIcon
create_icon(QString icon)
{
    return QIcon(create_pixmap(icon));
}

/* . */
QIcon
create_swatch(int32_t color)
{
    QPixmap pixmap(16, 16);
    pixmap.fill(QColor(color));
    return QIcon(pixmap); 
}


/* . */
void
wait_cursor(void)
{
    QApplication::setOverrideCursor(Qt::WaitCursor);
}

/* . */
void
arrow_cursor(void)
{
    QApplication::setOverrideCursor(Qt::ArrowCursor);
}

/* . */
void
restore_cursor(void)
{
    QApplication::restoreOverrideCursor();
}

/* . */
void
set_toolbar_horizontal(int data[])
{
    for (int i=0; data[i] != TERMINATOR_SYMBOL; i++) {
        toolbar[data[i]]->setOrientation(Qt::Horizontal);
    }
}

QMenuBar *menuBar()
{
    return _main->menuBar();
}

Application::Application(int argc, char **argv) : QApplication(argc, argv), _mainWin(NULL)
{
}

bool
Application::event(QEvent *event)
{
    switch (event->type()) {
    case QEvent::FileOpen:
        if (_main) {
            QStringList sl = QStringList(static_cast<QFileOpenEvent *>(event)->file());
            EmbStringTable files;
            to_string_table(sl, files);
            openFilesSelected(files);
            return true;
        }
        /* Fall through */
    default:
        return QApplication::event(event);
    }
}

int
make_application(int argc, char* argv[])
{
#if defined(Q_OS_MAC)
    Application app(argc, argv);
#else
    QApplication app(argc, argv);
#endif
    app.setApplicationName(_appName_);
    app.setApplicationVersion(_appVer_);

    EmbStringTable filesToOpen;
    for (int i=0; i<argc; i++) {
        strcpy(filesToOpen[i], argv[i]);
    }
    
    _main = new MainWindow();
#if defined(Q_OS_MAC)
    app.setMainWin(_main);
#endif

    QObject::connect(&app, SIGNAL(lastWindowClosed()), _main, SLOT(quit()));

    _main->setWindowTitle(app.applicationName() + " " + app.applicationVersion());
    _main->show();

    /* NOTE: If openFilesSelected() is called from within the _main constructor,
     * slot commands wont work and the window menu will be screwed
     */
    if (argc > 1) {
        openFilesSelected(filesToOpen);
    }

    return app.exec();
}

/* . */
void
nanosleep_(int time)
{
    std::this_thread::sleep_for(std::chrono::milliseconds(time));
}

/*
 * BUG: two layer properties dropdowns malfunctioning
 * BUG: layers button broken icon
 * BUG: text size dropdown broken
 *
 * Read the code that this replaces carefully.
 */
void
add_to_selector(QComboBox* box, EmbStringTable list, EmbString type, int use_icon)
{
    int n = string_array_length(list) / 3;
    for (int i=0; i<n; i++) {
        if (!use_icon) {
            if (!strcmp(type, "real")) {
                box->addItem(list[3*i+0], atof(list[3*i+1]));
                continue;
            }
            if (!strcmp(type, "int")) {
                box->addItem(list[3*i+0], atoi(list[3*i+1]));
            }
            continue;
        }
        if (!strcmp(type, "string")) {
            box->addItem(create_icon(list[3*i+0]), list[3*i+1]);
            continue;
        }
        if (!strcmp(type, "int")) {
            if (strlen(list[3*i+2]) > 0) {
                box->addItem(create_icon(list[3*i+0]), list[3*i+1],
                    atoi(list[3*i+2]));
            }
            else {
                box->addItem(create_icon(list[3*i+0]), list[3*i+1]);
            }
            continue;
        }
        if (!strcmp(type, "real")) {
            if (strlen(list[3*i+2]) > 0) {
                box->addItem(create_icon(list[3*i+0]), list[3*i+1],
                    atof(list[3*i+2]));
            }
            else {
                box->addItem(create_icon(list[3*i+0]), list[3*i+1]);
            }
        }
    }
}

/* . */
void
MainWindow::add_toolbar_to_window(Qt::ToolBarArea area, int data[])
{
    int i;
    for (i = 0; data[i] != TERMINATOR_SYMBOL; i++) {
        if (data[i] == TOOLBAR_BREAK) {
            addToolBarBreak(area);
        }
        else {
            addToolBar(area, toolbar[data[i]]);
        }
    }
}

MainWindow::MainWindow() : QMainWindow(0)
{
    readSettings();
    
    for (int i=0; i<MAX_OPEN_FILES; i++) {
        document_memory[i] = false;
    }

    QString lang = get_str(GENERAL_LANGUAGE);
    debug_message((char*)qPrintable("language: " + lang));
    if (lang == "system") {
        lang = QLocale::system().languageToString(QLocale::system().language()).toLower();
    }

    /* Init */
    _main = this;

    for (int i=0; i<N_MENUS; i++) {
        menu[i] = new QMenu(translate(menu_list[i]), this);
    }

    for (int i=0; i<N_TOOLBARS; i++) {
        toolbar[i] = new QToolBar(translate(toolbar_list[i]), this);
    }

    /* Selectors */
    layerSelector = new QComboBox(this);
    colorSelector = new QComboBox(this);
    linetypeSelector = new QComboBox(this);
    lineweightSelector = new QComboBox(this);
    textFontSelector = new QFontComboBox(this);
    textSizeSelector = new QComboBox(this);

    numOfDocs = 0;
    docIndex = 0;

    shiftKeyPressedState = false;

    setWindowIcon(create_icon("app"));
    setMinimumSize(800, 480); /* Require Minimum WVGA */

    loadFormats();

    /* create the mdiArea */
    QFrame* vbox = new QFrame(this);
    QVBoxLayout* layout = new QVBoxLayout(vbox);
    /* layout->setMargin(0); */
    vbox->setFrameStyle(QFrame::StyledPanel | QFrame::Sunken);
    mdiArea = new MdiArea(this, vbox);
    mdiArea->useBackgroundLogo(get_bool(GENERAL_MDI_BG_USE_LOGO));
    mdiArea->useBackgroundTexture(get_bool(GENERAL_MDI_BG_USE_TEXTURE));
    mdiArea->useBackgroundColor(get_bool(GENERAL_MDI_BG_USE_COLOR));
    mdiArea->setBackgroundLogo(get_str(GENERAL_MDI_BG_LOGO));
    mdiArea->setBackgroundTexture(get_str(GENERAL_MDI_BG_TEXTURE));
    mdiArea->setBackgroundColor(QColor(get_int(GENERAL_MDI_BG_COLOR)));
    mdiArea->setViewMode(QMdiArea::TabbedView);
    mdiArea->setHorizontalScrollBarPolicy(Qt::ScrollBarAsNeeded);
    mdiArea->setVerticalScrollBarPolicy(Qt::ScrollBarAsNeeded);
    mdiArea->setActivationOrder(QMdiArea::ActivationHistoryOrder);

    layout->addWidget(mdiArea);
    setCentralWidget(vbox);

    /* create the Command Prompt */
    prompt = new CmdPrompt(this);
    prompt->setFocus(Qt::OtherFocusReason);
    this->setFocusProxy(prompt);
    mdiArea->setFocusProxy(prompt);

    setPromptTextColor(get_int(PROMPT_TEXT_COLOR));
    setPromptBackgroundColor(get_int(PROMPT_BG_COLOR));

    connect(prompt, SIGNAL(startCommand(QString)), this, SLOT(logPromptInput(QString)));

    connect(prompt, SIGNAL(startCommand(QString)), this, SLOT(runCommandMain(char *)));
    connect(prompt, SIGNAL(runCommand(QString, QString)), this, SLOT(runCommandPrompt(QString, QString)));

    connect(prompt, SIGNAL(deletePressed()), this, SLOT(deletePressed()));
    /* TODO: connect(prompt, SIGNAL(tabPressed()), this, SLOT(someUnknownSlot())); */
    connect(prompt, SIGNAL(escapePressed()), this, SLOT(escapePressed()));
    connect(prompt, SIGNAL(upPressed()), this, SLOT(promptInputPrevious()));
    connect(prompt, SIGNAL(downPressed()), this, SLOT(promptInputNext()));
    connect(prompt, SIGNAL(F1Pressed()), this, SLOT(help()));
    /* TODO: connect(prompt, SIGNAL(F2Pressed()), this, SLOT(floatHistory())); */
    /* TODO: connect(prompt, SIGNAL(F3Pressed()), this, SLOT(toggleQSNAP())); */
    connect(prompt, SIGNAL(F4Pressed()), this, SLOT(toggleLwt())); /* TODO: typically this is toggleTablet(), make F-Keys customizable thru settings */
    /* TODO: connect(prompt, SIGNAL(F5Pressed()), this, SLOT(toggleISO())); */
    /* TODO: connect(prompt, SIGNAL(F6Pressed()), this, SLOT(toggleCoords())); */
    connect(prompt, SIGNAL(F7Pressed()), this, SLOT(toggleGrid()));
    /* TODO: connect(prompt, SIGNAL(F8Pressed()), this, SLOT(toggleORTHO())); */
    /* TODO: connect(prompt, SIGNAL(F9Pressed()), this, SLOT(toggleSNAP())); */
    /* TODO: connect(prompt, SIGNAL(F10Pressed()), this, SLOT(togglePOLAR())); */
    /* TODO: connect(prompt, SIGNAL(F11Pressed()), this, SLOT(toggleQTRACK())); */
    connect(prompt, SIGNAL(F12Pressed()), this, SLOT(toggleRuler()));
    connect(prompt, SIGNAL(cutPressed()), this, SLOT(cut()));
    connect(prompt, SIGNAL(copyPressed()), this, SLOT(copy()));
    connect(prompt, SIGNAL(pastePressed()), this, SLOT(paste()));
    connect(prompt, SIGNAL(selectAllPressed()), this, SLOT(selectAll()));
    connect(prompt, SIGNAL(undoPressed()), this, SLOT(undo()));
    connect(prompt, SIGNAL(redoPressed()), this, SLOT(redo()));

    connect(prompt, SIGNAL(shiftPressed()), this, SLOT(setShiftPressed()));
    connect(prompt, SIGNAL(shiftReleased()), this, SLOT(setShiftReleased()));

    connect(prompt, SIGNAL(showSettings()), this, SLOT(settingsPrompt()));

    connect(prompt, SIGNAL(historyAppended(QString)), this, SLOT(promptHistoryAppended(QString)));

    /* create the Object Property Editor */
    dockPropEdit = new PropertyEditor(
        get_str(GENERAL_ICON_THEME),
        get_bool(SELECTION_MODE_PICKADD),
        prompt,
        this);
    addDockWidget(Qt::LeftDockWidgetArea, dockPropEdit);
    connect(dockPropEdit, SIGNAL(pickAddModeToggled()), this, SLOT(pickAddModeToggled()));

    /* create the Command History Undo Editor */
    dockUndoEdit = new UndoEditor(get_str(GENERAL_ICON_THEME), prompt, this);
    addDockWidget(Qt::LeftDockWidgetArea, dockUndoEdit);

    /* setDockOptions(QMainWindow::AnimatedDocks | QMainWindow::AllowTabbedDocks | QMainWindow::VerticalTabs); */ /* TODO: Load these from settings */
    /* tabifyDockWidget(dockPropEdit, dockUndoEdit); */ /* TODO: load this from settings */

    create_statusbar(this);
    this->setStatusBar(statusbar);

    createAllActions();

    /* Do not allow the menus to be torn off.
     * It's a pain in the ass to maintain.
     */
    debug_message("create all menus");

    add_to_menu(MENU_FILE, file_menu);
    add_to_menu(MENU_EDIT, edit_menu);
    add_to_menu(MENU_VIEW, view_menu);
    add_to_menu(MENU_DRAW, draw_menu);
    add_to_menu(MENU_TOOLS, tools_menu);
    add_to_menu(MENU_MODIFY, modify_menu);
    add_to_menu(MENU_DIMENSION, dimension_menu);
    add_to_menu(MENU_SANDBOX, sandbox_menu);
//    add_to_menu(MENU_WINDOW, window_menu);
    add_to_menu(MENU_HELP, help_menu);
//    add_to_menu(MENU_RECENT, recent_menu);
    add_to_menu(MENU_ZOOM, zoom_menu);
    add_to_menu(MENU_PAN, pan_menu);

    for (int i=0; menubar_full_list[i] != TERMINATOR_SYMBOL; i++) {
        menuBar()->addMenu(menu[i]);
    }

    QObject::connect(menu[MENU_RECENT], SIGNAL(aboutToShow()), _main,
        SLOT(recentMenuAboutToShow()));
    QObject::connect(menu[MENU_WINDOW], SIGNAL(aboutToShow()), _main,
        SLOT(windowMenuAboutToShow()));

    menu[MENU_RECENT]->setTearOffEnabled(false);
    menu[MENU_WINDOW]->setTearOffEnabled(false);

    debug_message("create all toolbars");

    add_to_toolbar(TOOLBAR_FILE, file_toolbar);
    add_to_toolbar(TOOLBAR_EDIT, edit_toolbar);
    add_to_toolbar(TOOLBAR_ZOOM, zoom_toolbar);
    add_to_toolbar(TOOLBAR_PAN, pan_toolbar);
    add_to_toolbar(TOOLBAR_VIEW, view_toolbar);
    add_to_toolbar(TOOLBAR_ICON, icon_toolbar);
    add_to_toolbar(TOOLBAR_HELP, help_toolbar);

    debug_message("createLayerToolbar()");

    toolbar[TOOLBAR_LAYER]->setObjectName("toolbarLayer");
    toolbar[TOOLBAR_LAYER]->addAction(actionHash[ACTION_MAKE_LAYER_CURRENT]);
    toolbar[TOOLBAR_LAYER]->addAction(actionHash[ACTION_LAYERS]);

    QString icontheme = get_str(GENERAL_ICON_THEME);

    layerSelector->setFocusProxy(prompt);
    /* TODO: Create layer pixmaps by concatenating several icons. */
    add_to_selector(layerSelector, layer_list, "string", true);
    toolbar[TOOLBAR_LAYER]->addWidget(layerSelector);
    connect(layerSelector, SIGNAL(currentIndexChanged(int)), this, SLOT(layerSelectorIndexChanged(int)));

    toolbar[TOOLBAR_LAYER]->addAction(actionHash[ACTION_LAYER_PREVIOUS]);

    connect(toolbar[TOOLBAR_LAYER], SIGNAL(topLevelChanged(bool)), this, SLOT(floatingChangedToolBar(bool)));

    debug_message("createPropertiesToolbar()");

    toolbar[TOOLBAR_PROPERTIES]->setObjectName("toolbarProperties");

    colorSelector->setFocusProxy(prompt);
    colorSelector->addItem(create_icon("colorbylayer"), "ByLayer");
    colorSelector->addItem(create_icon("colorbyblock"), "ByBlock");
    colorSelector->addItem(create_icon("colorred"), translate("Red"), qRgb(255, 0, 0));
    colorSelector->addItem(create_icon("coloryellow"), translate("Yellow"), qRgb(255, 255, 0));
    colorSelector->addItem(create_icon("colorgreen"), translate("Green"), qRgb(0, 255, 0));
    colorSelector->addItem(create_icon("colorcyan"), translate("Cyan"), qRgb(  0,255,255));
    colorSelector->addItem(create_icon("colorblue"), translate("Blue"), qRgb(  0, 0,255));
    colorSelector->addItem(create_icon("colormagenta"), translate("Magenta"), qRgb(255, 0,255));
    colorSelector->addItem(create_icon("colorwhite"), translate("White"), qRgb(255,255,255));
    colorSelector->addItem(create_icon("colorother"), translate("Other..."));
    toolbar[TOOLBAR_PROPERTIES]->addWidget(colorSelector);
    connect(colorSelector, SIGNAL(currentIndexChanged(int)), this, SLOT(colorSelectorIndexChanged(int)));

    toolbar[TOOLBAR_PROPERTIES]->addSeparator();
    linetypeSelector->setFocusProxy(prompt);
    add_to_selector(linetypeSelector, line_type_list, "string", true);
    toolbar[TOOLBAR_PROPERTIES]->addWidget(linetypeSelector);
    connect(linetypeSelector, SIGNAL(currentIndexChanged(int)), this, SLOT(linetypeSelectorIndexChanged(int)));

    toolbar[TOOLBAR_PROPERTIES]->addSeparator();
    lineweightSelector->setFocusProxy(prompt);
    add_to_selector(lineweightSelector, line_weight_list, "real", true);
    /* Prevent dropdown text readability being squish...d. */
    lineweightSelector->setMinimumContentsLength(8);
    toolbar[TOOLBAR_PROPERTIES]->addWidget(lineweightSelector);
    connect(lineweightSelector, SIGNAL(currentIndexChanged(int)), this, SLOT(lineweightSelectorIndexChanged(int)));

    connect(toolbar[TOOLBAR_PROPERTIES], SIGNAL(topLevelChanged(bool)), this, SLOT(floatingChangedToolBar(bool)));

    debug_message("createTextToolbar()");

    toolbar[TOOLBAR_TEXT]->setObjectName("toolbarText");

    toolbar[TOOLBAR_TEXT]->addWidget(textFontSelector);
    textFontSelector->setCurrentFont(QFont(get_str(TEXT_FONT)));
    connect(textFontSelector, SIGNAL(currentFontChanged(const QFont&)), this, SLOT(textFontSelectorCurrentFontChanged(const QFont&)));

    toolbar[TOOLBAR_TEXT]->addAction(actionHash[ACTION_TEXT_BOLD]);
    toolbar[TOOLBAR_TEXT]->addAction(actionHash[ACTION_TEXT_ITALIC]);
    toolbar[TOOLBAR_TEXT]->addAction(actionHash[ACTION_TEXT_UNDERLINE]);
    toolbar[TOOLBAR_TEXT]->addAction(actionHash[ACTION_TEXT_STRIKEOUT]);
    toolbar[TOOLBAR_TEXT]->addAction(actionHash[ACTION_TEXT_OVERLINE]);

    actionHash[ACTION_TEXT_BOLD]->setChecked(get_bool(TEXT_STYLE_BOLD));
    actionHash[ACTION_TEXT_ITALIC]->setChecked(get_bool(TEXT_STYLE_ITALIC));
    actionHash[ACTION_TEXT_UNDERLINE]->setChecked(get_bool(TEXT_STYLE_UNDERLINE));
    actionHash[ACTION_TEXT_STRIKEOUT]->setChecked(get_bool(TEXT_STYLE_STRIKEOUT));
    actionHash[ACTION_TEXT_OVERLINE]->setChecked(get_bool(TEXT_STYLE_OVERLINE));

    textSizeSelector->setFocusProxy(prompt);
    add_to_selector(textSizeSelector, text_size_list, "int", false);
    setTextSize(get_int(TEXT_SIZE));
    toolbar[TOOLBAR_TEXT]->addWidget(textSizeSelector);
    connect(textSizeSelector, SIGNAL(currentIndexChanged(int)), this, SLOT(textSizeSelectorIndexChanged(int)));

    connect(toolbar[TOOLBAR_TEXT], SIGNAL(topLevelChanged(bool)), this, SLOT(floatingChangedToolBar(bool)));

    debug_message("createPromptToolbar()");

    toolbar[TOOLBAR_PROMPT]->setObjectName("toolbarPrompt");
    toolbar[TOOLBAR_PROMPT]->addWidget(prompt);
    toolbar[TOOLBAR_PROMPT]->setAllowedAreas(Qt::TopToolBarArea | Qt::BottomToolBarArea);
    connect(toolbar[TOOLBAR_PROMPT], SIGNAL(topLevelChanged(bool)), prompt, SLOT(floatingChanged(bool)));

    add_to_toolbar(TOOLBAR_DRAW, draw_toolbar);
    add_to_toolbar(TOOLBAR_MODIFY, modify_toolbar);

    set_toolbar_horizontal(toolbar_horizontal);

    add_toolbar_to_window(Qt::TopToolBarArea, top_toolbar);
    add_toolbar_to_window(Qt::BottomToolBarArea, bottom_toolbar);
    add_toolbar_to_window(Qt::LeftToolBarArea, left_toolbar);

    /* zoomToolBar->setToolButtonStyle(Qt::ToolButtonTextOnly); */

    iconResize(get_int(GENERAL_ICON_SIZE));
    update_interface();

    /* Show date in statusbar after it has been updated. */
    QDate date = QDate::currentDate();
    QString datestr = date.toString("d MMMM yyyy");
    statusbar->showMessage(datestr);

    showNormal();

    toolbar[TOOLBAR_PROMPT]->show();

    if (get_bool(GENERAL_TIP_OF_THE_DAY) && (!testing_mode)) {
        tipOfTheDay();
    }

    debug_message("Finished creating window.");
}

MainWindow::~MainWindow()
{
    debug_message("Destructor()");

    /* Prevent memory leaks by deleting any unpasted objects. */
    qDeleteAll(cutCopyObjectList.begin(), cutCopyObjectList.end());
    cutCopyObjectList.clear();
}

/* . */
void
MainWindow::recentMenuAboutToShow(void)
{
    debug_message("recentMenuAboutToShow()");
    menu[MENU_RECENT]->clear();

    int n = string_array_length(recent_files);
    for (int i = 0; i < n; ++i) {
        /* If less than the max amount of entries add to menu. */
        if (i < get_int(OPENSAVE_RECENT_MAX_FILES)) {
            QFileInfo recentFileInfo = QFileInfo(recent_files[i]);
            bool valid = validFileFormat((char*)qPrintable(recentFileInfo.fileName()));
            if (recentFileInfo.exists() && valid) {
                QString recentValue = QString().setNum(i+1);
                QAction* rAction;
                if (recentValue.toInt() >= 1 && recentValue.toInt() <= 9) {
                    rAction = new QAction("&" + recentValue + " " + recentFileInfo.fileName(), _main);
                }
                else if (recentValue.toInt() == 10) {
                    rAction = new QAction("1&0 " + recentFileInfo.fileName(), _main);
                }
                else {
                    rAction = new QAction(recentValue + " " + recentFileInfo.fileName(), _main);
                }
                rAction->setCheckable(false);
                QString fname(recent_files[i]);
                rAction->setData(fname);
                menu[MENU_RECENT]->addAction(rAction);
                QObject::connect(rAction, SIGNAL(triggered()), _main,
                    SLOT(openrecentfile()));
            }
        }
        else {
            break;
        }
    }
    /* Ensure the list only has max amount of entries. */
    if (get_int(OPENSAVE_RECENT_MAX_FILES) < MAX_FILES) {
        strcpy(recent_files[get_int(OPENSAVE_RECENT_MAX_FILES)], "END");
    }
    else {
        set_int(OPENSAVE_RECENT_MAX_FILES, MAX_FILES - 1);
        strcpy(recent_files[get_int(OPENSAVE_RECENT_MAX_FILES)], "END");
    }
}

/* . */
void
windowMenuAboutToShow(void)
{
    debug_message("windowMenuAboutToShow()");
    menu[MENU_WINDOW]->clear();
    menu[MENU_WINDOW]->addAction(actionHash[ACTION_WINDOW_CLOSE]);
    menu[MENU_WINDOW]->addAction(actionHash[ACTION_WINDOW_CLOSE_ALL]);
    menu[MENU_WINDOW]->addSeparator();
    menu[MENU_WINDOW]->addAction(actionHash[ACTION_WINDOW_CASCADE]);
    menu[MENU_WINDOW]->addAction(actionHash[ACTION_WINDOW_TILE]);
    menu[MENU_WINDOW]->addSeparator();
    menu[MENU_WINDOW]->addAction(actionHash[ACTION_WINDOW_NEXT]);
    menu[MENU_WINDOW]->addAction(actionHash[ACTION_WINDOW_PREVIOUS]);

    menu[MENU_WINDOW]->addSeparator();
    QList<QMdiSubWindow*> windows = mdiArea->subWindowList();
    for (int i = 0; i < windows.count(); ++i) {
        QAction* aAction = new QAction(windows.at(i)->windowTitle(), _main);
        aAction->setCheckable(true);
        aAction->setData(i);
        menu[MENU_WINDOW]->addAction(aAction);
        QObject::connect(aAction, SIGNAL(toggled(bool)), _main, SLOT(menu[MENU_WINDOW]Activated(bool)));
        aAction->setChecked(mdiArea->activeSubWindow() == windows.at(i));
    }
}

/* . */
void
MainWindow::windowMenuActivated(bool checked)
{
    debug_message("windowMenuActivated()");
    QAction* aSender = qobject_cast<QAction*>(sender());
    if (!aSender) {
        return;
    }
    QWidget* w = mdiArea->subWindowList().at(aSender->data().toInt());
    if (w && checked) {
        w->setFocus();
    }
}

/* . */
void
new_file(void)
{
    debug_message("new_file()");
    docIndex++;
    numOfDocs++;
    MdiWindow* mdiWin = new MdiWindow(docIndex, _main, mdiArea, Qt::SubWindow);
    QObject::connect(mdiWin, SIGNAL(sendCloseMdiWin(MdiWindow*)), _main,
        SLOT(onCloseMdiWin(MdiWindow*)));
    QObject::connect(mdiArea, SIGNAL(subWindowActivated(QMdiSubWindow*)), _main,
        SLOT(onWindowActivated(QMdiSubWindow*)));

    update_interface();
    windowMenuAboutToShow();

    int32_t doc_index = mdiWin->doc_index;
    if (doc_index) {
        doc_recalculate_limits(doc_index);
        doc_zoom_extents(doc_index);
    }
}

/* . */
void
to_string_table(QStringList src, EmbStringTable dest)
{
    int i;
    for (i=0; i < MAX_FILES && i < src.size(); i++) {
        strcpy(dest[i], (char*)qPrintable(src[i]));
    }
    strcpy(dest[i], end_symbol);
}

/* . */
void
openFile(bool recent, EmbString recentFile)
{
    debug_message("openFile()");

    QApplication::setOverrideCursor(Qt::ArrowCursor);

    EmbStringTable files;
    int n_files = 0;
    bool preview = get_bool(OPENSAVE_OPEN_THUMBNAIL);
    strcpy(openFilesPath, get_str(OPENSAVE_RECENT_DIRECTORY));

    /* Check to see if this from the recent files list. */
    if (recent) {
        strcpy(files[0], (char*)qPrintable(recentFile));
        strcpy(files[1], end_symbol);
        openFilesSelected(files);
    }
    else if (!preview) {
        /* TODO: set getOpenFileNames' selectedFilter parameter from opensave_open_format.setting */
        QStringList files_ = QFileDialog::getOpenFileNames(_main,
            translate("Open"), QString(openFilesPath), formatFilterOpen);
        to_string_table(files_, files);
        openFilesSelected(files);
    }
    else if (preview) {
        PreviewDialog* openDialog = new PreviewDialog(_main,
            translate("Open w/Preview"),
            QString(openFilesPath), formatFilterOpen);
        /* TODO: set openDialog->selectNameFilter(QString filter) from opensave_open_format.setting */
        QObject::connect(openDialog, SIGNAL(filesSelected(const QStringList&)), _main,
            SLOT(openFilesSelected(const QStringList&)));
        openDialog->exec();
    }

    restore_cursor();
}

/* . */
void
openFilesSelected(EmbStringTable filesToOpen)
{
    debug_message("openFileSelected()");
    bool doOnce = true;

    int n = string_array_length(filesToOpen);
    for (int i = 0; i < n; i++) {
        EmbString message;
        sprintf(message, "opening %s...", qPrintable(filesToOpen[i]));
        debug_message(message);

        QMdiSubWindow* existing = findMdiWindow((char*)qPrintable(filesToOpen[i]));
        if (existing) {
            debug_message("File already exists, switching to it.");
            mdiArea->setActiveSubWindow(existing);
            continue;
        }

        /* The docIndex doesn't need increased as it is only used for unnamed files. */
        numOfDocs++;
        MdiWindow* mdiWin = new MdiWindow(docIndex, _main, mdiArea, Qt::SubWindow);
        QObject::connect(mdiWin, SIGNAL(sendCloseMdiWin(MdiWindow*)), _main,
            SLOT(onCloseMdiWin(MdiWindow*)));
        QObject::connect(mdiArea, SIGNAL(subWindowActivated(QMdiSubWindow*)), _main,
            SLOT(onWindowActivated(QMdiSubWindow*)));

        /* Make sure the toolbars/etc... are shown before doing their zoomExtents. */
        if (doOnce) {
            update_interface();
            doOnce = false;
        }

        if (mdiWin->loadFile(filesToOpen[i])) {
            statusbar->showMessage(translate("File(s) loaded"), 2000);
            mdiWin->show();
            mdiWin->showMaximized();
            /* Prevent duplicate entries in the recent files list. */
            if (!string_list_contains(recent_files, filesToOpen[i])) {
                for (int j=0; j<MAX_FILES-1; j++) {
                    strcpy(recent_files[j], recent_files[j+1]);
                }
                string_copy(recent_files[0], filesToOpen[i]);
            }
            /* Move the recent file to the top of the list */
            else {
                string_copy(recent_files[0], filesToOpen[i]);
                string_copy(recent_files[1], end_symbol);
            }
            set_str(OPENSAVE_RECENT_DIRECTORY, (char*)qPrintable(QFileInfo(filesToOpen[i]).absolutePath()));

            int32_t doc_index = mdiWin->doc_index;
            if (doc_index) {
                doc_recalculate_limits(doc_index);
                doc_zoom_extents(doc_index);
            }
        }
        else {
            critical_box(translate("Failed to load file"),
                translate("Failed to load file."));
            debug_message("Failed to load file.");
            mdiWin->close();
        }
    }

    windowMenuAboutToShow();
}

/* . */
void
open_recent_file(void)
{
    debug_message("open_recent_file()");

    /* Check to see if this from the recent files list. */
    /* FIXME: QAction* recentSender = qobject_cast<QAction*>(sender());
    if (recentSender) {
        openFile(true, recentSender->data().toString());
    }
    */
}

/* . */
void
save_as_file(void)
{
    debug_message("save_as_file()");
    /* need to find the activeSubWindow before it loses focus to the FileDialog. */
    MdiWindow* mdiWin = qobject_cast<MdiWindow*>(mdiArea->activeSubWindow());
    if (!mdiWin) {
        return;
    }

    strcpy(openFilesPath, get_str(OPENSAVE_RECENT_DIRECTORY));
    QString file = QFileDialog::getSaveFileName(_main,
        translate("Save As"), QString(openFilesPath), formatFilterSave);

    mdiWin->saveFile(file);
}

/* . */
QMdiSubWindow*
findMdiWindow(EmbString fileName)
{
    EmbString message;
    sprintf(message, "findMdiWindow(%s)", fileName);
    debug_message(message);
    QString canonicalFilePath = QFileInfo(fileName).canonicalFilePath();

    foreach (QMdiSubWindow* subWindow, mdiArea->subWindowList()) {
        MdiWindow* mdiWin = qobject_cast<MdiWindow*>(subWindow);
        if (mdiWin) {
            if (documents[mdiWin->doc_index]->data.curFile == canonicalFilePath) {
                return subWindow;
            }
        }
    }
    return 0;
}

/* . */
void
MainWindow::closeEvent(QCloseEvent* event)
{
    mdiArea->closeAllSubWindows();
    writeSettings();
    event->accept();
}

/* . */
void
onCloseWindow(void)
{
    debug_message("onCloseWindow()");
    MdiWindow* mdiWin = qobject_cast<MdiWindow*>(mdiArea->activeSubWindow());
    if (mdiWin) {
        onCloseMdiWin(mdiWin);
    }
}

/* . */
void
onCloseMdiWin(MdiWindow* theMdiWin)
{
    debug_message("onCloseMdiWin()");
    numOfDocs--;

    bool keepMaximized;
    if (theMdiWin) {
        keepMaximized = theMdiWin->isMaximized();
    }

    mdiArea->removeSubWindow(theMdiWin);
    theMdiWin->deleteLater();

    update_interface();
    windowMenuAboutToShow();

    if (keepMaximized) {
        MdiWindow* mdiWin = qobject_cast<MdiWindow*>(mdiArea->activeSubWindow());
        if (mdiWin) {
            mdiWin->showMaximized();
        }
    }
}

/* . */
void
onWindowActivated(QMdiSubWindow* w)
{
    debug_message("onWindowActivated()");
    MdiWindow* mdiWin = qobject_cast<MdiWindow*>(w);
    if (mdiWin) {
        mdiWin->onWindowActivated();
    }
}

/* . */
void
MainWindow::resizeEvent(QResizeEvent* e)
{
    debug_message("resizeEvent()");
    QMainWindow::resizeEvent(e);
    statusBar()->setSizeGripEnabled(!isMaximized());
}

/* . */
QAction*
getFileSeparator()
{
    debug_message("getFileSeparator()");
    return myFileSeparator;
}

/* . */
void
update_interface()
{
    debug_message("update_interface()");

    actionHash[ACTION_PRINT]->setEnabled(numOfDocs > 0);
    actionHash[ACTION_WINDOW_CLOSE]->setEnabled(numOfDocs > 0);
    actionHash[ACTION_DESIGN_DETAILS]->setEnabled(numOfDocs > 0);

    if (numOfDocs) {
        /* Toolbars */
        for (int i=0; toolbars_when_docs[i] != TERMINATOR_SYMBOL; i++) {
            toolbar[toolbars_when_docs[i]]->show();
        }

        /* DockWidgets */
        dockPropEdit->show();
        dockUndoEdit->show();

        /* Menus */
        menuBar()->clear();
        for (int i=0; menubar_full_list[i] != TERMINATOR_SYMBOL; i++) {
            menuBar()->addMenu(menu[menubar_full_list[i]]);
        }
        menu[MENU_WINDOW]->setEnabled(true);

        /* Statusbar */
        statusbar->clearMessage();
        statusBarMouseCoord->show();
        for (int i=0; i<N_SB_BUTTONS; i++) {
            statusBarButtons[i]->show();
        }
    }
    else {
        /* Toolbars */
        for (int i=0; toolbars_when_docs[i] != TERMINATOR_SYMBOL; i++) {
            toolbar[toolbars_when_docs[i]]->hide();
        }

        /* DockWidgets */
        dockPropEdit->hide();
        dockUndoEdit->hide();

        /* Menus */
        menuBar()->clear();
        for (int i=0; menubar_no_docs[i] != TERMINATOR_SYMBOL; i++) {
            menuBar()->addMenu(menu[menubar_no_docs[i]]);
        }
        menu[MENU_WINDOW]->setEnabled(false);

        /* Statusbar */
        statusbar->clearMessage();
        statusBarMouseCoord->hide();
        for (int i=0; i<N_SB_BUTTONS; i++) {
            statusBarButtons[i]->hide();
        }
    }
    hide_unimplemented();
}

/* . */
void
hide_unimplemented(void)
{
    debug_message("hide_unimplemented()");
}

/* . */
bool
validFileFormat(EmbString fileName)
{
    if (emb_identify_format(fileName) >= 0) {
        return true;
    }
    return false;
}

/* . */
void
loadFormats()
{
    char stable, unstable;
    QString supportedReaders  = "";
    QString individualReaders = "";
    QString supportedWriters  = "";
    QString individualWriters = "";
    QString supportedStr;
    QString individualStr;

    /* TODO: Stable Only (Settings Option) */
    /* stable = 'S'; unstable = 'S'; */

    /* Stable + Unstable */
    stable = 'S'; unstable = 'U';

    for (int i=0; i<numberOfFormats; i++) {
        const char* extension = formatTable[i].extension;
        const char* description = formatTable[i].description;
        char readerState = formatTable[i].reader_state;
        char writerState = formatTable[i].writer_state;

        QString upperExt = QString(extension).toUpper();
        supportedStr = "*" + upperExt + " ";
        individualStr = upperExt.replace(".", "") + " - " + description + " (*" + extension + ");;";
        if (readerState == stable || readerState == unstable) {
            /* Exclude color file formats from open dialogs. */
            if (upperExt != "COL" && upperExt != "EDR" && upperExt != "INF" && upperExt != "RGB") {
                supportedReaders.append(supportedStr);
                individualReaders.append(individualStr);
            }
        }
        if (writerState == stable || writerState == unstable) {
            supportedWriters.append(supportedStr);
            individualWriters.append(individualStr);
        }
    }

    sprintf(formatFilterOpen,
        "All Supported Files (%s);;All Files (*);;%s",
        qPrintable(supportedReaders),
        qPrintable(individualReaders));
    sprintf(formatFilterSave,
        "All Supported Files (%s);;All Files (*);;%s",
        qPrintable(supportedWriters),
        qPrintable(individualWriters));

    /* TODO: Fixup custom filter. */
    /*
    QString custom = custom_filter.setting;
    if (custom.contains("supported", Qt::CaseInsensitive)) {
        custom = ""; */ /* This will hide it */ /*
    }
    else if (!custom.contains("*", Qt::CaseInsensitive)) {
        custom = ""; */ /* This will hide it */ /*
    }
    else {
        custom = "Custom Filter(" + custom + ");;";
    }

    return tr(qPrintable(custom + supported + all));
    */
}

/* . */
void
MainWindow::closeToolBar(QAction* action)
{
    if (action->objectName() == "toolbarclose") {
        QToolBar* tb = qobject_cast<QToolBar*>(sender());
        if (tb) {
            debug_message((char*)qPrintable(tb->objectName() + " closed."));
            tb->hide();
        }
    }
}

/* . */
void
MainWindow::floatingChangedToolBar(bool isFloating)
{
    QToolBar* tb = qobject_cast<QToolBar*>(sender());
    if (tb) {
        if (isFloating) {
            /* TODO: Determine best suited close button on various platforms. */
            /*
            QStyle::SP_DockWidgetCloseButton
            QStyle::SP_TitleBarCloseButton
            QStyle::SP_DialogCloseButton
            */
            QAction *ACTION = new QAction(tb->style()->standardIcon(QStyle::SP_DialogCloseButton), "Close", this);
            ACTION->setStatusTip("Close the " + tb->windowTitle() + " Toolbar");
            ACTION->setObjectName("toolbarclose");
            tb->addAction(ACTION);
            connect(tb, SIGNAL(actionTriggered(QAction*)), this, SLOT(closeToolBar(QAction*)));
        }
        else {
            QList<QAction*> actList = tb->actions();
            for (int i = 0; i < actList.size(); ++i) {
                QAction* ACTION = actList[i];
                if (ACTION->objectName() == "toolbarclose") {
                    tb->removeAction(ACTION);
                    disconnect(tb, SIGNAL(actionTriggered(QAction*)), this, SLOT(closeToolBar(QAction*)));
                    delete ACTION;
                }
            }
        }
    }
}

/* . */
QAction*
get_action_by_icon(EmbString icon)
{
    int i;
    for (i=0; command_data[i].id != -2; i++) {
        if (string_equal(command_data[i].icon, icon)) {
            return actionHash[command_data[i].id];
        }
    }
    return actionHash[ACTION_DO_NOTHING];
}

/* . */
int
get_id(EmbStringTable data, EmbString label)
{
    int id;
    int n = string_array_length(data);
    for (id=0; id<n; id++) {
        if (string_equal(data[id], label)) {
            return id;
        }
    }
    return -1;
}

/* . */
void
add_to_menu(int index, EmbStringTable menu_data)
{
    int n = string_array_length(menu_data);
    for (int i=0; i<n; i++) {
        char *s = menu_data[i];
        if (s[0] == '-') {
            menu[index]->addSeparator();
        }
        else if (s[0] == '>') {
            int id = get_id(menu_list, s+1);
            if (id < 0) {
                debug_message("Failed to identify submenu.");
                continue;
            }
            menu[index]->addMenu(menu[id]);
        }
        else if (s[0] == '+') {
            QString icontheme = get_str(GENERAL_ICON_THEME);
            menu[index]->setIcon(create_icon(s+1));
        }
        else {
            menu[index]->addAction(get_action_by_icon(s));
        }
    }
    menu[index]->setTearOffEnabled(false);
}

/* Note: on Unix we include the trailing separator. For Windows compatibility we
 * omit it.
 */
QString
SettingsDir()
{
#if defined(Q_OS_UNIX) || defined(Q_OS_MAC)
    return QDir::homePath() + "/.embroidermodder2/";
#else
    return "";
#endif
}

/* . */
void
readSettings(void)
{
    debug_message("Reading Settings...");
    /* This file needs to be read from the users home directory to ensure it is writable. */
    QString settingsDir = SettingsDir();
    /* load_settings(qPrintable(SettingsDir())); */

    QSettings settings(SettingsDir() + settings_file, QSettings::IniFormat);
    QPoint pos = settings.value("Window/Position", QPoint(0, 0)).toPoint();
    QSize size = settings.value("Window/Size", QSize(800, 600)).toSize();

    layoutState = settings.value("LayoutState").toByteArray();
    if (!_main->restoreState(layoutState)) {
        debug_message("LayoutState NOT restored! Setting Default Layout...");
        /* someToolBar->setVisible(true); */
    }

    for (int i=0; i<N_SETTINGS; i++) {
        char *label = settings_data[i].label;
        char *value = settings_data[i].value;
        switch (settings_data[i].type) {
        case SCRIPT_INT: {
            int x = settings.value(label, atoi(value)).toInt();
            printf("%d %s %d %d\n", i, label, atoi(value), x);
            set_int(i, x);
            break;
        }
        case SCRIPT_BOOL: {
            bool x = settings.value(label, (bool)atoi(value)).toBool();
            printf("%d %s %d %d\n", i, label, (bool)atoi(value), x);
            set_bool(i, x);
            break;
        }
        case SCRIPT_STRING: {
            char *x = (char *)qPrintable(settings.value(label, QString(value)).toString());
            printf("%d %s %s %s\n", i, label, value, x);
            set_str(i, x);
            break;
        }
        case SCRIPT_REAL: {
            float x = settings.value(label, atof(value)).toFloat();
            printf("%d %s %f %f\n", i, label, atof(value), x);
            set_real(i, x);
            break;
        }
        default:
            // TODO: ERROR
            break;
        }
    }

    /* FIXME:
    _main->move(pos);
    _main->resize(size);
    */
}

/* . */
void
writeSettings(void)
{
    debug_message("Writing Settings...");
    save_settings("", (char*)qPrintable(SettingsDir() + settings_file));
}

/* . */
void
settingsPrompt(void)
{
    settingsDialog("Prompt");
}

/* . */
void
settingsDialog(EmbString showTab)
{
    Settings_Dialog dialog(_main, showTab, _main);
    dialog.exec();
}

/* . */
void
add_to_toolbar(int id, EmbStringTable toolbar_data)
{
    toolbar[id]->setObjectName(QString("toolbar") + toolbar_list[id]);

    int n = string_array_length(toolbar_data);
    for (int i=0; i<n; i++) {
        if (toolbar_data[i][0] == '-') {
            toolbar[id]->addSeparator();
        }
        else {
            QAction *action = get_action_by_icon(toolbar_data[i]);
            toolbar[id]->addAction(action);
        }
    }

    QAction::connect(toolbar[id], SIGNAL(topLevelChanged(bool)), _main,
        SLOT(floatingChangedToolBar(bool)));
}

/* For each Command in command_data, for each alias set up a map from
 * alias to the Command. Then for a given context the call doesn't have to loop?
 *
 * NOTE:
 * Every QScriptProgram must have a unique function name to call. If every function was called main(), then
 * the ScriptArgs would only call the last script evaluated (which happens to be main() in another script).
 * Thus, by adding the cmdName before main(), it becomes line_main(), circle_main(), etc...
 * Do not change this code unless you really know what you are doing. I mean it.
 *
 * Position currently comes from the order of the command_data.
 *
 * TODO:
 * Set What's This Context Help to statusTip for now so there is some infos there.
 * Make custom whats this context help popup with more descriptive help than just
 * the status bar/tip one liner(short but not real long) with a hyperlink in the custom popup
 * at the bottom to open full help file description. Ex: like wxPython AGW's SuperToolTip.
 */
void
createAllActions(void)
{
    debug_message("Creating All Actions...");
    for (int i=0; command_data[i].id != -2; i++) {
        QString icon(command_data[i].icon);
        QString toolTip(command_data[i].tooltip);
        QString statusTip(command_data[i].statustip);
        QString alias_string(command_data[i].alias);
        QStringList aliases = alias_string.split(",");

        debug_message((char*)qPrintable("COMMAND: " + icon));

        QAction *ACTION = new QAction(create_icon(icon), toolTip, _main);
        ACTION->setStatusTip(statusTip);
        ACTION->setObjectName(icon);
        ACTION->setWhatsThis(statusTip);

        if (command_data[i].shortcut[0] = 0) {
            ACTION->setShortcut(QKeySequence(command_data[i].shortcut));
        }

        if (icon == "textbold" || icon == "textitalic" || icon == "textunderline"
            || icon == "textstrikeout" || icon == "textoverline") {
            ACTION->setCheckable(true);
        }

        QObject::connect(ACTION, SIGNAL(triggered()), _main, SLOT(runCommand()));

        aliasHash[icon.toStdString()] = icon.toStdString();
        actionHash[command_data[i].id] = ACTION;

        foreach (QString alias, aliases) {
            char msg[200];
            aliasHash[alias.toStdString()] = icon.toStdString();
            sprintf(msg, "Command Added: %s, %s", qPrintable(icon), qPrintable(icon));
            debug_message(msg);
        }
    }

    actionHash[ACTION_WINDOW_CLOSE]->setEnabled(numOfDocs > 0);
    actionHash[ACTION_DESIGN_DETAILS]->setEnabled(numOfDocs > 0);
}


/* TODO: Alphabetic/Categorized TabWidget */

PropertyEditor::PropertyEditor(QString iconDirectory, bool pickAddMode, QWidget* widgetToFocus, QWidget* parent) : QDockWidget(parent)
{
    iconDir = iconDirectory;
    iconSize = 16;
    propertyEditorButtonStyle = Qt::ToolButtonTextBesideIcon; /* TODO: Make customizable */
    setMinimumSize(100,100);

    pickAdd = pickAddMode;

    precisionAngle = 0; /* TODO: Load this from settings and provide function for updating from settings */
    precisionLength = 4; /* TODO: Load this from settings and provide function for updating from settings */

    signalMapper = new QSignalMapper(this);

    fieldOldText = "";
    fieldNewText = "";
    fieldVariesText = "*Varies*";
    fieldYesText = "Yes";
    fieldNoText = "No";
    fieldOnText = "On";
    fieldOffText = "Off";

    QWidget* widgetMain = new QWidget(this);

    QWidget* widgetSelection = new QWidget(this);
    QHBoxLayout* hboxLayoutSelection = new QHBoxLayout(this);
    hboxLayoutSelection->addWidget(createComboBoxSelected());
    hboxLayoutSelection->addWidget(createToolButtonQSelect());
    hboxLayoutSelection->addWidget(createToolButtonPickAdd());
    widgetSelection->setLayout(hboxLayoutSelection);

    createGroupBox__("General", "General", general_editor_data);
    createGroupBox__("GeometryArc", "Geometry", geometry_arc_editor_data);
    createGroupBox__("MiscArc", "Misc", misc_arc_editor_data);
    createGroupBox__("GeometryBlock", "Geometry", geometry_block_editor_data);
    createGroupBox__("GeometryCircle", "Geometry", geometry_circle_editor_data);
    createGroupBox__("GeometryDimAligned", "Geometry", geometry_dim_aligned_editor_data);
    createGroupBox__("GeometryDimAngular", "Geometry", geometry_dim_angular_editor_data);
    createGroupBox__("GeometryDimArcLength", "Geometry", geometry_dim_arc_length_editor_data);
    createGroupBox__("GeometryDimDiameter", "Geometry", geometry_dim_diameter_editor_data);
    createGroupBox__("GeometryDimLeader", "Geometry", geometry_dim_leader_editor_data);
    createGroupBox__("GeometryDimLinear", "Geometry", geometry_dim_linear_editor_data);
    createGroupBox__("GeometryDimOrdinate", "Geometry", geometry_dim_ordinate_editor_data);
    createGroupBox__("GeometryDimRadius", "Geometry", geometry_dim_radius_editor_data);
    createGroupBox__("GeometryEllipse", "Geometry", geometry_ellipse_editor_data);
    createGroupBox__("GeometryImage", "Geometry", geometry_image_editor_data);
    createGroupBox__("MiscImage", "Misc", misc_image_editor_data);
    createGroupBox__("GeometryInfiniteLine", "Geometry", geometry_infinite_line_editor_data);
    createGroupBox__("GeometryLine", "Geometry", geometry_line_editor_data);
    createGroupBox__("GeometryPath", "Geometry", geometry_path_editor_data);
    createGroupBox__("MiscPath", "Misc", misc_path_editor_data);
    createGroupBox__("GeometryPoint", "Geometry", geometry_point_editor_data);
    createGroupBox__("GeometryPolygon", "Geometry", geometry_polygon_editor_data);
    createGroupBox__("GeometryPolyline", "Geometry", geometry_polyline_editor_data);
    createGroupBox__("MiscPolyline", "Misc", misc_polyline_editor_data);
    createGroupBox__("GeometryRay", "Geometry", geometry_ray_editor_data);
    createGroupBox__("GeometryRectangle", "Geometry", geometry_rectangle_editor_data);
    createGroupBox__("GeometryTextMulti", "Geometry", geometry_text_multi_editor_data);
    createGroupBox__("TextTextSingle", "Text", text_text_single_editor_data);
    createGroupBox__("GeometryTextSingle", "Geometry", geometry_text_single_editor_data);
    createGroupBox__("MiscTextSingle", "Misc", misc_text_single_editor_data);

    QScrollArea* scrollProperties = new QScrollArea(this);
    QWidget* widgetProperties = new QWidget(this);
    QVBoxLayout* vboxLayoutProperties = new QVBoxLayout(this);
    foreach (QString label, group_box_list) {
        vboxLayoutProperties->addWidget(group_boxes[label]);
    }
    vboxLayoutProperties->addStretch(1);
    widgetProperties->setLayout(vboxLayoutProperties);
    scrollProperties->setWidget(widgetProperties);
    scrollProperties->setWidgetResizable(true);

    QVBoxLayout* vboxLayoutMain = new QVBoxLayout(this);
    vboxLayoutMain->addWidget(widgetSelection);
    vboxLayoutMain->addWidget(scrollProperties);
    widgetMain->setLayout(vboxLayoutMain);

    setWidget(widgetMain);
    setWindowTitle(translate("Properties"));
    setAllowedAreas(Qt::LeftDockWidgetArea | Qt::RightDockWidgetArea);

    hideAllGroups();

    QObject::connect(signalMapper, SIGNAL(mapped(QObject*)), this, SLOT(fieldEdited(QObject*)));

    focusWidget_ = widgetToFocus;
    this->installEventFilter(this);
}

PropertyEditor::~PropertyEditor()
{
}

bool PropertyEditor::eventFilter(QObject *obj, QEvent *event)
{
    if (event->type() == QEvent::KeyPress) {
        QKeyEvent* pressedKey = (QKeyEvent*)event;
        int key = pressedKey->key();
        switch(key) {
        case Qt::Key_Escape:
            if (focusWidget_) {
                focusWidget_->setFocus(Qt::OtherFocusReason);
            }
            return true;
        default: {
            pressedKey->ignore();
            break;
        }
        }
    }
    return QObject::eventFilter(obj, event);
}

/* . */
QComboBox*
createComboBoxSelected(void)
{
    comboBoxSelected = new QComboBox(dockPropEdit);
    comboBoxSelected->addItem(translate("No Selection"));
    return comboBoxSelected;
}

/* . */
QToolButton*
createToolButtonQSelect(void)
{
    toolButtonQSelect = new QToolButton(dockPropEdit);
    toolButtonQSelect->setIcon(create_icon("quickselect"));
    toolButtonQSelect->setIconSize(QSize(iconSize, iconSize));
    toolButtonQSelect->setText("QSelect");
    toolButtonQSelect->setToolTip("QSelect"); /* TODO: Better Description */
    toolButtonQSelect->setToolButtonStyle(Qt::ToolButtonIconOnly);
    return toolButtonQSelect;
}

/* . */
QToolButton*
createToolButtonPickAdd(void)
{
    /* TODO: Set as PickAdd or PickNew based on settings */
    toolButtonPickAdd = new QToolButton(dockPropEdit);
    dockPropEdit->updatePickAddModeButton(pickAdd);
    QObject::connect(toolButtonPickAdd, SIGNAL(clicked(bool)), dockPropEdit, SLOT(togglePickAddMode()));
    return toolButtonPickAdd;
}

void
PropertyEditor::updatePickAddModeButton(bool pickAddMode)
{
    pickAdd = pickAddMode;
    if (pickAdd) {
        toolButtonPickAdd->setIcon(create_icon("pickadd"));
        toolButtonPickAdd->setIconSize(QSize(iconSize, iconSize));
        toolButtonPickAdd->setText("PickAdd");
        toolButtonPickAdd->setToolTip("PickAdd Mode - Add to current selection.\nClick to switch to PickNew Mode.");
        toolButtonPickAdd->setToolButtonStyle(Qt::ToolButtonIconOnly);
    }
    else {
        toolButtonPickAdd->setIcon(create_icon("picknew"));
        toolButtonPickAdd->setIconSize(QSize(iconSize, iconSize));
        toolButtonPickAdd->setText("PickNew");
        toolButtonPickAdd->setToolTip("PickNew Mode - Replace current selection.\nClick to switch to PickAdd Mode.");
        toolButtonPickAdd->setToolButtonStyle(Qt::ToolButtonIconOnly);
    }
}

/* . */
void
PropertyEditor::togglePickAddMode()
{
    emit pickAddModeToggled();
}

/* . */
void
PropertyEditor::setSelectedItems(QList<QGraphicsItem*> itemList)
{
    selectedItemList = itemList;
    /* Hide all the groups initially, then decide which ones to show. */
    hideAllGroups();
    comboBoxSelected->clear();

    if (itemList.isEmpty()) {
        comboBoxSelected->addItem(translate("No Selection"));
        return;
    }

    QSet<int> typeSet;

    int numAll = itemList.size();
    int object_counts[50];
    for (int i=0; i<50; i++) {
        object_counts[i] = 0;
    }

    foreach (QGraphicsItem* item, itemList) {
        if (!item) {
            continue;
        }

        int objType = item->type();
        typeSet.insert(objType);

        if ((objType > OBJ_BASE) && (objType < OBJ_UNKNOWN)) {
            object_counts[objType-OBJ_ARC]++;
        }
        else {
            object_counts[OBJ_UNKNOWN-OBJ_ARC]++;
        }
    }

    int numTypes = typeSet.size();

    /* Populate the selection comboBox. */
    if (numTypes > 1) {
        QString s(translate("Varies"));
        comboBoxSelected->addItem(s + " (" + QString().setNum(numAll) + ")");
        QObject::connect(comboBoxSelected, SIGNAL(currentIndexChanged(int)), dockPropEdit, SLOT(showOneType(int)));
    }

    foreach (int objType, typeSet) {
        if ((objType > OBJ_BASE) && (objType <= OBJ_UNKNOWN)) {
            int index = objType - OBJ_ARC;
            QString comboBoxStr = translate(object_names[index]);
            comboBoxStr += " (" + QString().setNum(object_counts[index]) + ")";
            comboBoxSelected->addItem(comboBoxStr, objType);
        }
    }

    /* Load Data into the fields. */

    /* Clear fields first so if the selected data varies, the comparison is simple. */
    clearAllFields();

    foreach (QGraphicsItem* item, itemList) {
        if (!item) {
            continue;
        }

        /* TODO: load data into the General field */
        int objType = item->type();
        Object* obj = static_cast<Object*>(item);
        if (!obj) {
            continue;
        }
        EmbGeometry *g = obj->core->geometry;
        switch (objType) {
        case OBJ_ARC: {
            EmbVector center = obj_center(obj->core);
            EmbVector start = g->object.arc.start;
            EmbVector end = g->object.arc.end;
            update_lineedit_num(line_edits["ArcCenterX"], center.x, false);
            update_lineedit_num(line_edits["ArcCenterY"], -center.y, false);
            update_lineedit_num(line_edits["ArcRadius"], obj_radius(obj->core), false);
            update_lineedit_num(line_edits["ArcStartAngle"], emb_start_angle(g), true);
            update_lineedit_num(line_edits["ArcEndAngle"], emb_end_angle(g), true);
            update_lineedit_num(line_edits["ArcStartX"], start.x, false);
            update_lineedit_num(line_edits["ArcStartY"], -start.y, false);
            update_lineedit_num(line_edits["ArcEndX"], end.x, false);
            update_lineedit_num(line_edits["ArcEndY"], -end.y, false);
            update_lineedit_num(line_edits["ArcArea"], emb_area(g), false);
            update_lineedit_num(line_edits["ArcLength"], emb_arc_length(g), false);
            update_lineedit_num(line_edits["ArcChord"], emb_chord(g), false);
            update_lineedit_num(line_edits["ArcIncAngle"], emb_included_angle(g), true);
            update_lineedit_bool(combo_boxes["ArcClockwise"], emb_clockwise(g), true);
            break;
        }
        case OBJ_BLOCK: {
            /* TODO: load block data */
            break;
        }
        case OBJ_CIRCLE: {
            EmbVector center = obj_center(obj->core);
            update_lineedit_num(line_edits["CircleCenterX"], center.x, false);
            update_lineedit_num(line_edits["CircleCenterY"], -center.y, false);
            update_lineedit_num(line_edits["CircleRadius"], obj_radius(obj->core), false);
            update_lineedit_num(line_edits["CircleDiameter"], obj_diameter(obj->core), false);
            update_lineedit_num(line_edits["CircleArea"], emb_area(g), false);
            update_lineedit_num(line_edits["CircleCircumference"], obj_circumference(obj->core), false);
            break;
        }
        case OBJ_DIMALIGNED: {
            /* TODO: load aligned dimension data */
            break;
        }
        case OBJ_DIMANGULAR: {
            /* TODO: load angular dimension data */
            break;
        }
        case OBJ_DIMARCLENGTH: {
            /* TODO: load arclength dimension data */
            break;
        }
        case OBJ_DIMDIAMETER: {
            /* TODO: load diameter dimension data */
            break;
        }
        case OBJ_DIMLEADER: {
            /* TODO: load leader dimension data */
            break;
        }
        case OBJ_DIMLINEAR: {
            /* TODO: load linear dimension data */
            break;
        }
        case OBJ_DIMORDINATE: {
            /* TODO: load ordinate dimension data */
            break;
        }
        case OBJ_DIMRADIUS: {
            /* TODO: load radius dimension data */
            break;
        }
        case OBJ_ELLIPSE: {
            EmbVector center = obj_center(obj->core);
            update_lineedit_num(line_edits["EllipseCenterX"], center.x, false);
            update_lineedit_num(line_edits["EllipseCenterY"], -center.y, false);
            update_lineedit_num(line_edits["EllipseRadiusMajor"], emb_radius_major(g), false);
            update_lineedit_num(line_edits["EllipseRadiusMinor"], emb_radius_minor(g), false);
            update_lineedit_num(line_edits["EllipseDiameterMajor"], emb_diameter_major(g), false);
            update_lineedit_num(line_edits["EllipseDiameterMinor"], emb_diameter_minor(g), false);
            break;
        }
        case OBJ_IMAGE: {
            todo("load image data");
            break;
        }
        case OBJ_INFINITELINE: {
            todo("load infinite line data");
            break;
        }
        case OBJ_LINE: {
            EmbVector point1 = obj_end_point_1(obj->core);
            EmbVector point2 = obj_end_point_2(obj->core);
            EmbVector delta = emb_vector_subtract(point2, point1);
            update_lineedit_num(line_edits["LineStartX"], point1.x, false);
            update_lineedit_num(line_edits["LineStartY"], -point1.y, false);
            update_lineedit_num(line_edits["LineEndX"], point2.x, false);
            update_lineedit_num(line_edits["LineEndY"], -point2.y, false);
            update_lineedit_num(line_edits["LineDeltaX"], delta.x, false);
            update_lineedit_num(line_edits["LineDeltaY"], -delta.y, false);
            update_lineedit_num(line_edits["LineAngle"], emb_angle(g), true);
            update_lineedit_num(line_edits["LineLength"], obj_length(obj->core), false);
            break;
        }
        case OBJ_PATH: {
            todo("load path data");
            break;
        }
        case OBJ_POINT: {
            update_lineedit_num(line_edits["PointX"], obj_x(obj->core), false);
            update_lineedit_num(line_edits["PointY"], -obj_y(obj->core), false);
            break;
        }
        case OBJ_POLYGON: {
            todo("load polygon data");
            break;
        }
        case OBJ_POLYLINE: {
            todo("load polyline data");
            break;
        }
        case OBJ_RAY: {
            todo("load ray data");
            break;
        }
        case OBJ_RECTANGLE: {
            EmbVector corn1 = obj_top_left(obj->core);
            EmbVector corn2 = obj_top_right(obj->core);
            EmbVector corn3 = obj_bottom_left(obj->core);
            EmbVector corn4 = obj_bottom_right(obj->core);

            update_lineedit_num(line_edits["RectangleCorner1X"], corn1.x, false);
            update_lineedit_num(line_edits["RectangleCorner1Y"], -corn1.y, false);
            update_lineedit_num(line_edits["RectangleCorner2X"], corn2.x, false);
            update_lineedit_num(line_edits["RectangleCorner2Y"], -corn2.y, false);
            update_lineedit_num(line_edits["RectangleCorner3X"], corn3.x, false);
            update_lineedit_num(line_edits["RectangleCorner3Y"], -corn3.y, false);
            update_lineedit_num(line_edits["RectangleCorner4X"], corn4.x, false);
            update_lineedit_num(line_edits["RectangleCorner4Y"], -corn4.y, false);
            update_lineedit_num(line_edits["RectangleWidth"], emb_width(g), false);
            update_lineedit_num(line_edits["RectangleHeight"], -emb_height(g), false);
            update_lineedit_num(line_edits["RectangleArea"], emb_area(g), false);
            break;
        }
        case OBJ_TEXTMULTI: {
            todo("load multiline text data");
            break;
        }
        case OBJ_TEXTSINGLE: {
            updateLineEditStrIfVaries(line_edits["TextSingleContents"], obj->core->text);
            updateFontComboBoxStrIfVaries(comboBoxTextSingleFont, obj->core->textFont);
            update_lineedit_str(combo_boxes["TextSingleJustify"],
                QString(obj->core->textJustify), objectTextJustifyList);
            update_lineedit_num(line_edits["TextSingleHeight"],
                obj->core->textSize, false);
            update_lineedit_num(line_edits["TextSingleRotation"], -obj->rotation(), true);
            update_lineedit_num(line_edits["TextSingleX"], obj_x(obj->core), false);
            update_lineedit_num(line_edits["TextSingleY"], -obj_y(obj->core), false);
            update_lineedit_bool(combo_boxes["TextSingleBackward"],
                obj->core->textBackward, true);
            update_lineedit_bool(combo_boxes["TextSingleUpsideDown"],
                obj->core->textUpsideDown, true);
            break;
        }
        default:
            break;
        }
    }

    /* Only show fields if all objects are the same type. */
    if (numTypes == 1) {
        foreach (int objType, typeSet) {
            showGroups(objType);
        }
    }
}

/* . */
void
updateLineEditStrIfVaries(QLineEdit* lineEdit, QString str)
{
    fieldOldText = lineEdit->text();
    fieldNewText = str;

    if (fieldOldText.isEmpty()) {
        lineEdit->setText(fieldNewText);
    }
    else if (fieldOldText != fieldNewText) {
        lineEdit->setText(fieldVariesText);
    }
}

/* . */
void
update_lineedit_num(QLineEdit* lineEdit, double num, bool useAnglePrecision)
{
    int precision = 0;
    if (useAnglePrecision) {
        precision = precisionAngle;
    }
    else {
        precision = precisionLength;
    }

    fieldOldText = lineEdit->text();
    fieldNewText.setNum(num, 'f', precision);

    /* Prevent negative zero :D */
    QString negativeZero = "-0.";
    for (int i = 0; i < precision; ++i) {
        negativeZero.append('0');
    }
    if (fieldNewText == negativeZero) {
        fieldNewText = negativeZero.replace("-", "");
    }

    if (fieldOldText.isEmpty()) {
        lineEdit->setText(fieldNewText);
    }
    else if (fieldOldText != fieldNewText) {
        lineEdit->setText(fieldVariesText);
    }
}

/* . */
void
updateFontComboBoxStrIfVaries(QFontComboBox* fontComboBox, QString str)
{
    EmbString message;
    fieldOldText = fontComboBox->property("FontFamily").toString();
    fieldNewText = str;
    /*
    sprintf(message, "old: %d %s, new: %d %s",
        oldIndex, qPrintable(fontComboBox->currentText()), newIndex, qPrintable(str));
    debug_message(message);
    */
    if (fieldOldText.isEmpty()) {
        fontComboBox->setCurrentFont(QFont(fieldNewText));
        fontComboBox->setProperty("FontFamily", fieldNewText);
    }
    else if (fieldOldText != fieldNewText) {
        /* Prevent multiple entries */
        if (fontComboBox->findText(fieldVariesText) == -1) {
            fontComboBox->addItem(fieldVariesText);
        }
        fontComboBox->setCurrentIndex(fontComboBox->findText(fieldVariesText));
    }
}

/* . */
void
update_lineedit_str(QComboBox* comboBox, QString str, const QStringList& strList)
{
    fieldOldText = comboBox->currentText();
    fieldNewText = str;

    if (fieldOldText.isEmpty()) {
        foreach(QString s, strList) {
            comboBox->addItem(s, s);
        }
        comboBox->setCurrentIndex(comboBox->findText(fieldNewText));
    }
    else if (fieldOldText != fieldNewText) {
        if (comboBox->findText(fieldVariesText) == -1) /* Prevent multiple entries */
            comboBox->addItem(fieldVariesText);
        comboBox->setCurrentIndex(comboBox->findText(fieldVariesText));
    }
}

/* . */
void
update_lineedit_bool(QComboBox* comboBox, bool val, bool yesOrNoText)
{
    fieldOldText = comboBox->currentText();
    if (yesOrNoText) {
        if (val) {
            fieldNewText = fieldYesText;
        }
        else {
            fieldNewText = fieldNoText;
        }
    }
    else {
        if (val) {
            fieldNewText = fieldOnText;
        }
        else {
            fieldNewText = fieldOffText;
        }
    }

    if (fieldOldText.isEmpty()) {
        if (yesOrNoText) {
            comboBox->addItem(fieldYesText, true);
            comboBox->addItem(fieldNoText, false);
        }
        else {
            comboBox->addItem(fieldOnText, true);
            comboBox->addItem(fieldOffText, false);
        }
        comboBox->setCurrentIndex(comboBox->findText(fieldNewText));
    }
    else if (fieldOldText != fieldNewText) {
        if (comboBox->findText(fieldVariesText) == -1) /* Prevent multiple entries. */
            comboBox->addItem(fieldVariesText);
        comboBox->setCurrentIndex(comboBox->findText(fieldVariesText));
    }
}

/* . */
void
showGroups(int objType)
{
    if (objType == OBJ_ARC) {
        group_boxes["GeometryArc"]->show();
        group_boxes["MiscArc"]->show();
    }
    else if (objType == OBJ_BLOCK) {
        group_boxes["GeometryBlock"]->show();
    }
    else if (objType == OBJ_CIRCLE) {
        group_boxes["GeometryCircle"]->show();
    }
    else if (objType == OBJ_DIMALIGNED) {
        group_boxes["GeometryDimAligned"]->show();
    }
    else if (objType == OBJ_DIMANGULAR) {
        group_boxes["GeometryDimAngular"]->show();
    }
    else if (objType == OBJ_DIMARCLENGTH) {
        group_boxes["GeometryDimArcLength"]->show();
    }
    else if (objType == OBJ_DIMDIAMETER) {
        group_boxes["GeometryDimDiameter"]->show();
    }
    else if (objType == OBJ_DIMLEADER) {
        group_boxes["GeometryDimLeader"]->show();
    }
    else if (objType == OBJ_DIMLINEAR) {
        group_boxes["GeometryDimLinear"]->show();
    }
    else if (objType == OBJ_DIMORDINATE) {
        group_boxes["GeometryDimOrdinate"]->show();
    }
    else if (objType == OBJ_DIMRADIUS) {
        group_boxes["GeometryDimRadius"]->show();
    }
    else if (objType == OBJ_ELLIPSE) {
        group_boxes["GeometryEllipse"]->show();
    }
    else if (objType == OBJ_IMAGE) {
        group_boxes["GeometryImage"]->show();
        group_boxes["MiscImage"]->show();
    }
    else if (objType == OBJ_INFINITELINE) {
        group_boxes["GeometryInfiniteLine"]->show();
    }
    else if (objType == OBJ_LINE) {
        group_boxes["GeometryLine"]->show();
    }
    else if (objType == OBJ_PATH) {
        group_boxes["GeometryPath"]->show();
        group_boxes["MiscPath"]->show();
    }
    else if (objType == OBJ_POINT) {
        group_boxes["GeometryPoint"]->show();
    }
    else if (objType == OBJ_POLYGON) {
        group_boxes["GeometryPolygon"]->show();
    }
    else if (objType == OBJ_POLYLINE) {
        group_boxes["GeometryPolyline"]->show();
        group_boxes["MiscPolyline"]->show();
    }
    else if (objType == OBJ_RAY) {
        group_boxes["GeometryRay"]->show();
    }
    else if (objType == OBJ_RECTANGLE) {
        group_boxes["GeometryRectangle"]->show();
    }
    else if (objType == OBJ_TEXTMULTI) {
        group_boxes["GeometryTextMulti"]->show();
    }
    else if (objType == OBJ_TEXTSINGLE) {
        group_boxes["TextTextSingle"]->show();
        group_boxes["GeometryTextSingle"]->show();
        group_boxes["MiscTextSingle"]->show();
    }
}

/* . */
void
showOneType(int index)
{
    hideAllGroups();
    showGroups(comboBoxSelected->itemData(index).toInt());
}

/* NOTE: General group will never be hidden. */
void
hideAllGroups(void)
{
    foreach (QString label, group_box_list) {
        if (label != "General") {
            group_boxes[label]->hide();
        }
    }
}

/* . */
void
clearAllFields(void)
{
    int n = string_array_length(editor_list);
    for (int i=0; i<n; i++) {
        line_edits[editor_list[i]]->clear();
    }
    n = string_array_length(combobox_list);
    for (int i=0; i<n; i++) {
        combo_boxes[combobox_list[i]]->clear();
    }

    comboBoxTextSingleFont->removeItem(comboBoxTextSingleFont->findText(fieldVariesText)); /* NOTE: Do not clear comboBoxTextSingleFont. */
    comboBoxTextSingleFont->setProperty("FontFamily", "");
}

/* . */
void
create_editor(
    QFormLayout *layout,
    EmbString icon,
    EmbString label,
    EmbString type_label,
    EmbString signal_name,
    int obj_type)
{
    EmbString signal;
    QToolButton *toolButton = createToolButton(icon, translate((char*)label));
    QString s(signal_name);
    if (!strcmp(signal_name, "combobox")) {
        sprintf(signal, "comboBox%s", signal_name);
        combo_boxes[s] = new QComboBox(dockPropEdit);
        if (signal_name[0] == 0) {
            combo_boxes[s]->setDisabled(true);
        }
        else {
            combo_boxes[s]->setDisabled(false);
            mapSignal(combo_boxes[s], signal, obj_type);
        }
        layout->addRow(toolButton, combo_boxes[s]);
        return;
    }
    if (!strcmp(signal_name, "fontcombobox")) {
        comboBoxTextSingleFont = new QFontComboBox(dockPropEdit);
        comboBoxTextSingleFont->setDisabled(false);

        mapSignal(comboBoxTextSingleFont, "comboBoxTextSingleFont", OBJ_TEXTSINGLE);
        layout->addRow(toolButton, comboBoxTextSingleFont);
    }

    sprintf(signal, "lineEdit%s", signal_name);

    line_edits[s] = new QLineEdit(dockPropEdit);
    if (!strcmp(type_label, "int")) {
        line_edits[s]->setValidator(new QIntValidator(line_edits[s]));
    }
    else if (!strcmp(type_label, "double")) {
        line_edits[s]->setValidator(new QDoubleValidator(line_edits[s]));
    }
    else if (!strcmp(type_label, "string")) {
    }

    if (signal_name[0] != 0) {
        line_edits[s]->setReadOnly(true);
    }
    else {
        line_edits[s]->setReadOnly(false);
        mapSignal(line_edits[s], signal, obj_type);
    }

    layout->addRow(toolButton, line_edits[s]);
}

/* . */
QGroupBox*
createGroupBox__(const char *label, const char *name, Editor editor_data[])
{
    todo("Use proper icons for tool buttons.");
    group_boxes[QString(label)] = new QGroupBox(translate((char*)name), dockPropEdit);

    QFormLayout* formLayout = new QFormLayout(dockPropEdit);
    for (int i=0; strcmp(editor_data[i].icon, "END"); i++) {
        Editor editor = editor_data[i];
        create_editor(formLayout, editor.icon, editor.label, editor.data_type,
            editor.signal, editor.object);
    }
    group_boxes[QString(label)]->setLayout(formLayout);

    return group_boxes[QString(label)];
}

/* . */
QToolButton*
createToolButton(QString iconName, QString txt)
{
    QToolButton* tb = new QToolButton(dockPropEdit);
    tb->setIcon(create_icon(iconName));
    tb->setIconSize(QSize(iconSize, iconSize));
    tb->setText(txt);
    tb->setToolButtonStyle(propertyEditorButtonStyle);
    tb->setStyleSheet("border:none;");
    return tb;
}

/* . */
void
mapSignal(QObject* fieldObj, QString name, QVariant value)
{
    fieldObj->setObjectName(name);
    fieldObj->setProperty(qPrintable(name), value);

    if (name.startsWith("lineEdit")) {
        QObject::connect(fieldObj, SIGNAL(editingFinished()), signalMapper, SLOT(map()));
    }
    else if (name.startsWith("comboBox")) {
        QObject::connect(fieldObj, SIGNAL(activated(QString)), signalMapper, SLOT(map()));
    }

    signalMapper->setMapping(fieldObj, fieldObj);
}

/* . */
void
fieldEdited(QObject* fieldObj)
{
    static bool blockSignals = false;
    if (blockSignals) {
        return;
    }

    debug_message("==========Field was Edited==========");
    QString objName = fieldObj->objectName();
    int objType = fieldObj->property(qPrintable(objName)).toInt();

    foreach(QGraphicsItem* item, selectedItemList) {
        if (item->type() != objType) {
            continue;
        }

        tempObj = static_cast<Object*>(item);
        if (tempObj) {
            /* TODO: Error message. */
            break;
        }

        switch(objType) {
        case OBJ_ARC: {
            if (objName == "lineEditArcCenterX") {
                EmbVector arc = obj_center(tempObj->core);
                EmbVector center;
                center.x = line_edits["ArcCenterX"]->text().toDouble();
                center.y = arc.y;
                obj_set_center(tempObj->core, center);
            }
            if (objName == "lineEditArcCenterY") {
                EmbVector arc = obj_center(tempObj->core);
                EmbVector center;
                center.x = arc.x;
                center.y = -line_edits["ArcCenterY"]->text().toDouble();
                obj_set_center(tempObj->core, center);
            }
            if (objName == "lineEditArcRadius") {
                emb_set_radius(tempObj->core->geometry, line_edits["ArcRadius"]->text().toDouble());
            }
            if (objName == "lineEditArcStartAngle") {
                emb_set_start_angle(tempObj->core->geometry, line_edits["ArcStartAngle"]->text().toDouble());
            }
            if (objName == "lineEditArcEndAngle") {
                emb_set_end_angle(tempObj->core->geometry, line_edits["ArcEndAngle"]->text().toDouble());
            }
            break;
        }
        case OBJ_BLOCK: {
            /* TODO: field editing. */
            break;
        }
        case OBJ_CIRCLE: {
            if (objName == "lineEditCircleCenterX") {
                obj_set_center_x(tempObj->core, line_edits["CircleCenterX"]->text().toDouble());
            }
            if (objName == "lineEditCircleCenterY") {
                obj_set_center_y(tempObj->core, -line_edits["CircleCenterY"]->text().toDouble());
            }
            if (objName == "lineEditCircleRadius") {
                emb_set_radius(tempObj->core->geometry, line_edits["CircleRadius"]->text().toDouble());
            }
            if (objName == "lineEditCircleDiameter") {
                emb_set_diameter(tempObj->core->geometry, line_edits["CircleDiameter"]->text().toDouble());
            }
            if (objName == "lineEditCircleArea") {
                emb_set_area(tempObj->core->geometry, line_edits["CircleArea"]->text().toDouble());
            }
            if (objName == "lineEditCircleCircumference") {
                emb_set_circumference(tempObj->core->geometry, line_edits["CircleCircumference"]->text().toDouble());
            }
            break;
        }
        case OBJ_DIMALIGNED: {
            /* TODO: field editing. */
            break;
        }
        case OBJ_DIMANGULAR: {
            /* TODO: field editing. */
            break;
        }
        case OBJ_DIMARCLENGTH: {
            /* TODO: field editing. */
            break;
        }
        case OBJ_DIMDIAMETER: {
            /* TODO: field editing. */
            break;
        }
        case OBJ_DIMLEADER: {
            /* TODO: field editing. */
            break;
        }
        case OBJ_DIMLINEAR: {
            /* TODO: field editing. */
            break;
        }
        case OBJ_DIMORDINATE: {
            /* TODO: field editing. */
            break;
        }
        case OBJ_DIMRADIUS: {
            /* TODO: field editing. */
            break;
        }
        case OBJ_ELLIPSE: {
            if (objName == "lineEditEllipseCenterX") {
                obj_set_center_x(tempObj->core, line_edits["EllipseCenterX"]->text().toDouble());
            }
            if (objName == "lineEditEllipseCenterY") {
                obj_set_center_y(tempObj->core, -line_edits["EllipseCenterY"]->text().toDouble());
            }
            if (objName == "lineEditEllipseRadiusMajor") {
                emb_set_radius_major(tempObj->core->geometry, line_edits["EllipseRadiusMajor"]->text().toDouble());
            }
            if (objName == "lineEditEllipseRadiusMinor") {
                emb_set_radius_minor(tempObj->core->geometry, line_edits["EllipseRadiusMinor"]->text().toDouble());
            }
            if (objName == "lineEditEllipseDiameterMajor") {
                emb_set_diameter_major(tempObj->core->geometry, line_edits["EllipseDiameterMajor"]->text().toDouble());
            }
            if (objName == "lineEditEllipseDiameterMinor") {
                emb_set_diameter_minor(tempObj->core->geometry, line_edits["EllipseDiameterMinor"]->text().toDouble());
            }
            break;
        }
        case OBJ_IMAGE: {
            /* TODO: field editing. */
            break;
        }
        case OBJ_INFINITELINE: {
            /* TODO: field editing. */
            break;
        }
        case OBJ_LINE: { 
                if (objName == "lineEditLineStartX") {
                    obj_set_x1(tempObj->core, line_edits["LineStartX"]->text().toDouble());
                }
                if (objName == "lineEditLineStartY") {
                    obj_set_y1(tempObj->core, -line_edits["LineStartY"]->text().toDouble());
                }
                if (objName == "lineEditLineEndX") {
                    obj_set_x2(tempObj->core, line_edits["LineEndX"]->text().toDouble());
                }
                if (objName == "lineEditLineEndY") {
                    obj_set_y2(tempObj->core, -line_edits["LineEndY"]->text().toDouble());
                }
            break;
        }
        case OBJ_PATH: {
            /* TODO: field editing. */
            break;
        }
        case OBJ_POINT: {
            if (objName == "lineEditPointX") {
                obj_set_x(tempObj->core, line_edits["PointX"]->text().toDouble());
            }
            if (objName == "lineEditPointY") {
                obj_set_y(tempObj->core, -line_edits["PointY"]->text().toDouble());
            }
            break;
        }
        case OBJ_POLYGON: {
            /* TODO: field editing. */
            break;
        }
        case OBJ_POLYLINE: {
            /* TODO: field editing. */
            break;
        }
        case OBJ_RAY: {
            /* TODO: field editing. */
            break;
        }
        case OBJ_RECTANGLE: {
            /* TODO: field editing. */
            break;
        }
        case OBJ_TEXTMULTI: {
            /* TODO: field editing. */
            break;
        }
        case OBJ_TEXTSINGLE: {
            /* TODO: field editing */
            if (objName == "lineEditTextSingleContents") {
                obj_set_text(tempObj->core,
                    qPrintable(line_edits["TextSingleContents"]->text()));
            }
            if (objName == "comboBoxTextSingleFont") {
                if (comboBoxTextSingleFont->currentText() == fieldVariesText) {
                    break;
                }
                obj_set_text_font(tempObj->core,
                    qPrintable(comboBoxTextSingleFont->currentFont().family()));
            }
            if (objName == "comboBoxTextSingleJustify") {
                if (combo_boxes["TextSingleJustify"]->currentText() == fieldVariesText) {
                    break;
                }
                int index = combo_boxes["TextSingleJustify"]->currentIndex();
                obj_set_text_justify(tempObj->core,
                    qPrintable(combo_boxes["TextSingleJustify"]->itemData(index).toString()));
                break;
            }
            if (objName == "lineEditTextSingleHeight") {
                double height = line_edits["TextSingleHeight"]->text().toDouble();
                obj_set_text_size(tempObj->core, height);
                break;
            }
            if (objName == "lineEditTextSingleRotation") {
                tempObj->setRotation(-line_edits["TextSingleRotation"]->text().toDouble());
                break;
            }
            if (objName == "lineEditTextSingleX") {
                obj_set_x(tempObj->core, line_edits["TextSingleX"]->text().toDouble());
                break;
            }
            if (objName == "lineEditTextSingleY") {
                obj_set_y(tempObj->core, line_edits["TextSingleY"]->text().toDouble());
                break;
            }
            if (objName == "comboBoxTextSingleBackward") {
                if (combo_boxes["TextSingleBackward"]->currentText() == fieldVariesText) {
                    break;
                }
                obj_set_text_backward(tempObj->core, combo_boxes["TextSingleBackward"]->itemData(combo_boxes["TextSingleBackward"]->currentIndex()).toBool());
                break;
            }
            if (objName == "comboBoxTextSingleUpsideDown") {
                if (combo_boxes["TextSingleUpsideDown"]->currentText() == fieldVariesText) {
                    break;
                }
                obj_set_text_upside_down(tempObj->core, combo_boxes["TextSingleUpsideDown"]->itemData(combo_boxes["TextSingleUpsideDown"]->currentIndex()).toBool());
            }
            break;
        }
        default:
            break;
        }

    }

    /* Block this slot from running twice since calling setSelectedItems will trigger it. */
    blockSignals = true;

    QWidget* widget = QApplication::focusWidget();
    /* Update so all fields have fresh data. TODO: Improve this. */
    dockPropEdit->setSelectedItems(selectedItemList);
    hideAllGroups();
    showGroups(objType);

    if (widget) widget->setFocus(Qt::OtherFocusReason);

    blockSignals = false;
}

/* . */
QPushButton *
Settings_Dialog::choose_color_button(QGroupBox* groupbox, int i)
{
    QPushButton* button = new QPushButton(_main->tr("Choose"), groupbox);
    button->setIcon(create_swatch(setting[i].preview.i));
    connect(button, &QPushButton::clicked, this, [=] () { chooseColor(i); });
    return button;
}

/* . */
QCheckBox*
Settings_Dialog::create_checkbox(QGroupBox* groupbox, int key)
{
    QCheckBox* checkBox = new QCheckBox(translate(settings_data[key].label), groupbox);
    checkBox->setChecked(setting[key].dialog.b);
    QObject::connect(checkBox, &QCheckBox::stateChanged, this,
        [=](int checked) { setting[key].dialog.b = checked; preview_update(); });
    if (QString(settings_data[key].icon) != "") {
        checkBox->setIcon(create_icon(settings_data[key].icon));
    }
    return checkBox;
}

/* . */
QDoubleSpinBox*
Settings_Dialog::create_spinbox(QGroupBox* groupbox, int key)
{
    QDoubleSpinBox* spinbox = new QDoubleSpinBox(groupbox);
    QStringList data = QString(settings_data[key].editor_data).split(',');
    spinbox->setObjectName(settings_data[key].key);
    spinbox->setSingleStep(data[0].toFloat());
    spinbox->setRange(data[1].toFloat(), data[2].toFloat());
    spinbox->setValue(setting[key].dialog.r);
    QObject::connect(spinbox, &QDoubleSpinBox::valueChanged, this,
        [=](double value) { setting[key].dialog.r = value; });
    return spinbox;
}

/* . */
QSpinBox*
Settings_Dialog::create_int_spinbox(QGroupBox* groupbox, int key)
{
    QSpinBox* spinbox = new QSpinBox(groupbox);
    QStringList data = QString(settings_data[key].editor_data).split(',');
    spinbox->setObjectName(settings_data[key].key);
    spinbox->setSingleStep(data[0].toInt());
    spinbox->setRange(data[1].toInt(), data[2].toInt());
    spinbox->setValue(setting[key].dialog.i);
    QObject::connect(spinbox, &QSpinBox::valueChanged, this,
        [=](int value) { setting[key].dialog.i = value; });
    return spinbox;
}


/* . */
void
set_visibility(QObject *senderObj, EmbString key, bool visibility)
{
    QObject* parent = senderObj->parent();
    if (!parent) {
        debug_message("set_visibility called from sender without a parent object");
        return;
    }
    int error = 1;
    if (!strncmp(key, "label", 5)) {
        QLabel* label = parent->findChild<QLabel*>(key);
        if (label) {
            label->setVisible(visibility);
            error = 0;
        }
    }
    if (!strncmp(key, "spinBox", 7)) {
        QDoubleSpinBox* spinbox = parent->findChild<QDoubleSpinBox*>(key);
        if (spinbox) {
            spinbox->setVisible(visibility);
            error = 0;
        }
    }
    if (!strncmp(key, "checkBox", 8)) {
        QCheckBox* checkbox = parent->findChild<QCheckBox*>(key);
        if (checkbox) {
            checkbox->setVisible(visibility);
            error = 0;
        }
    }
    if (!strncmp(key, "comboBox", 8)) {
        QComboBox* combobox = parent->findChild<QComboBox*>(key);
        if (combobox) {
            combobox->setVisible(visibility);
            error = 0;
        }
    }
    if (error) {
        debug_message("Failed to enable/disable the variable");
        debug_message((char*)key);
    }
}

/* . */
void
set_visibility_group(QObject *senderObj, EmbStringTable keylist, bool visibility)
{
    int i;
    int n = string_array_length(keylist);
    for (i=0; i<n; i++) {
        set_visibility(senderObj, keylist[i], visibility);
    }
}

/* . */
void
set_enabled(QObject *senderObj, EmbString key, bool enabled)
{
    QObject* parent = senderObj->parent();
    if (!parent) {
        /* Error reporting. */
        return;
    }
    int error = 1;
    if (!strncmp(key, "label", 5)) {
        QLabel* label = parent->findChild<QLabel*>(key);
        if (label) {
            label->setEnabled(enabled);
            error = 0;
        }
    }
    if (!strncmp(key, "spinBox", 7)) {
        QDoubleSpinBox* spinbox = parent->findChild<QDoubleSpinBox*>(key);
        if (spinbox) {
            spinbox->setEnabled(enabled);
            error = 0;
        }
    }
    if (!strncmp(key, "checkBox", 8)) {
        QCheckBox* checkbox = parent->findChild<QCheckBox*>(key);
        if (checkbox) {
            checkbox->setEnabled(enabled);
            error = 0;
        }
    }
    if (!strncmp(key, "comboBox", 8)) {
        QComboBox* combobox = parent->findChild<QComboBox*>(key);
        if (combobox) {
            combobox->setEnabled(enabled);
            error = 0;
        }
    }
    if (error) {
        debug_message("Failed to enable/disable the variable");
        debug_message((char*)key);
    }
}

/* . */
void
set_enabled_group(QObject *senderObj, EmbStringTable keylist, bool enabled)
{
    int i;
    int n = string_array_length(keylist);
    for (i=0; i<n; i++) {
        set_enabled(senderObj, keylist[i], enabled);
    }
}

/* . */
Settings_Dialog::Settings_Dialog(MainWindow* mw, QString showTab, QWidget* parent) : QDialog(parent)
{
    setMinimumSize(750,550);

    tabWidget = new QTabWidget(this);

    for (int i=0; i<N_SETTINGS; i++) {
        copy_setting(i, SETTING_DIALOG, SETTING_SETTING);
        copy_setting(i, SETTING_PREVIEW, SETTING_SETTING);
        copy_setting(i, SETTING_ACCEPT, SETTING_SETTING);
    }

    /* TODO: Add icons to tabs */
    tabWidget->addTab(createTabGeneral(), translate("General"));
    tabWidget->addTab(createTabFilesPaths(), translate("Files/Paths"));
    tabWidget->addTab(createTabDisplay(), translate("Display"));
    tabWidget->addTab(createTabPrompt(), translate("Prompt"));
    tabWidget->addTab(createTabOpenSave(), translate("Open/Save"));
    tabWidget->addTab(createTabPrinting(), translate("Printing"));
    tabWidget->addTab(createTabSnap(), translate("Snap"));
    tabWidget->addTab(createTabGridRuler(), translate("Grid/Ruler"));
    tabWidget->addTab(createTabOrthoPolar(), translate("Ortho/Polar"));
    tabWidget->addTab(createTabQuickSnap(), translate("QuickSnap"));
    tabWidget->addTab(createTabQuickTrack(), translate("QuickTrack"));
    tabWidget->addTab(createTabLineWeight(), translate("LineWeight"));
    tabWidget->addTab(createTabSelection(), translate("Selection"));

    if (showTab == "General") {
        tabWidget->setCurrentIndex(0);
    }
    else if (showTab == "Files/Path") {
        tabWidget->setCurrentIndex(1);
    }
    else if (showTab == "Display") {
        tabWidget->setCurrentIndex(2);
    }
    else if (showTab == "Prompt") {
        tabWidget->setCurrentIndex(3);
    }
    else if (showTab == "Open/Save") {
        tabWidget->setCurrentIndex(4);
    }
    else if (showTab == "Printing") {
        tabWidget->setCurrentIndex( 5);
    }
    else if (showTab == "Snap")        tabWidget->setCurrentIndex( 6);
    else if (showTab == "Grid/Ruler")  tabWidget->setCurrentIndex( 7);
    else if (showTab == "Ortho/Polar") tabWidget->setCurrentIndex( 8);
    else if (showTab == "QuickSnap")   tabWidget->setCurrentIndex( 9);
    else if (showTab == "QuickTrack")  tabWidget->setCurrentIndex(10);
    else if (showTab == "LineWeight")  tabWidget->setCurrentIndex(11);
    else if (showTab == "Selection")   tabWidget->setCurrentIndex(12);

    buttonBox = new QDialogButtonBox(QDialogButtonBox::Ok | QDialogButtonBox::Cancel);

    connect(buttonBox, SIGNAL(accepted()), this, SLOT(acceptChanges()));
    connect(buttonBox, SIGNAL(rejected()), this, SLOT(rejectChanges()));

    QVBoxLayout* vboxLayoutMain = new QVBoxLayout(this);
    vboxLayoutMain->addWidget(tabWidget);
    vboxLayoutMain->addWidget(buttonBox);
    setLayout(vboxLayoutMain);

    setWindowTitle(translate("Settings"));

    QApplication::setOverrideCursor(Qt::ArrowCursor);
}

/* . */
Settings_Dialog::~Settings_Dialog()
{
    QApplication::restoreOverrideCursor();
}

/* . */
QWidget*
Settings_Dialog::createTabGeneral()
{
    QWidget* widget = new QWidget(this);

    /* Language */
    QGroupBox* groupBoxLanguage = new QGroupBox(translate("Language"), widget);

    QLabel* labelLanguage = new QLabel(translate("Language (Requires Restart)"), groupBoxLanguage);
    QComboBox* comboBoxLanguage = new QComboBox(groupBoxLanguage);
    strcpy(setting[GENERAL_LANGUAGE].dialog.s,
        qPrintable(QString(get_str(GENERAL_LANGUAGE)).toLower()));
    comboBoxLanguage->addItem("Default");
    comboBoxLanguage->addItem("System");
    comboBoxLanguage->insertSeparator(2);
    QDir trDir(qApp->applicationDirPath());
    trDir.cd("translations");
    foreach(QString dirName, trDir.entryList(QDir::Dirs | QDir::NoDotAndDotDot))
    {
        dirName[0] = dirName[0].toUpper();
        comboBoxLanguage->addItem(dirName);
    }
    QString current = setting[GENERAL_LANGUAGE].dialog.s;
    current[0] = current[0].toUpper();
    comboBoxLanguage->setCurrentIndex(comboBoxLanguage->findText(current));
    connect(comboBoxLanguage, SIGNAL(currentIndexChanged(QString )), this, SLOT(comboBoxLanguageCurrentIndexChanged(QString )));

    QVBoxLayout* vboxLayoutLanguage = new QVBoxLayout(groupBoxLanguage);
    vboxLayoutLanguage->addWidget(labelLanguage);
    vboxLayoutLanguage->addWidget(comboBoxLanguage);
    groupBoxLanguage->setLayout(vboxLayoutLanguage);

    /* Icons */
    QGroupBox* groupBoxIcon = new QGroupBox(translate("Icons"), widget);

    QLabel* labelIconTheme = new QLabel(translate("Icon Theme"), groupBoxIcon);
    QComboBox* comboBoxIconTheme = new QComboBox(groupBoxIcon);
    QDir dir(qApp->applicationDirPath());
    dir.cd("icons");
    foreach(QString dirName, dir.entryList(QDir::Dirs | QDir::NoDotAndDotDot)) {
        comboBoxIconTheme->addItem(QIcon("icons/" + dirName + "/theme.png"), dirName);
    }
    comboBoxIconTheme->setCurrentIndex(comboBoxIconTheme->findText(setting[GENERAL_ICON_THEME].dialog.s));
    connect(comboBoxIconTheme, SIGNAL(currentIndexChanged(QString )), this, SLOT(comboBoxIconThemeCurrentIndexChanged(QString )));

    QLabel* labelIconSize = new QLabel(translate("Icon Size"), groupBoxIcon);
    QComboBox* comboBoxIconSize = new QComboBox(groupBoxIcon);
    QString dialog_icon("icons/");
    dialog_icon += setting[GENERAL_ICON_THEME].dialog.s;
    dialog_icon += "/";
    comboBoxIconSize->addItem(QIcon(dialog_icon + "icon16.png"), "Very Small", 16);
    comboBoxIconSize->addItem(QIcon(dialog_icon + "icon24.png"), "Small", 24);
    comboBoxIconSize->addItem(QIcon(dialog_icon + "icon32.png"), "Medium", 32);
    comboBoxIconSize->addItem(QIcon(dialog_icon + "icon48.png"), "Large", 48);
    comboBoxIconSize->addItem(QIcon(dialog_icon + "icon64.png"), "Very Large", 64);
    comboBoxIconSize->addItem(QIcon(dialog_icon + "icon128.png"), "I'm Blind", 128);
    setting[GENERAL_ICON_SIZE].dialog.i = get_int(GENERAL_ICON_SIZE);
    comboBoxIconSize->setCurrentIndex(comboBoxIconSize->findData(setting[GENERAL_ICON_SIZE].dialog.i));
    connect(comboBoxIconSize, SIGNAL(currentIndexChanged(int)), this, SLOT(comboBoxIconSizeCurrentIndexChanged(int)));

    QVBoxLayout* vboxLayoutIcon = new QVBoxLayout(groupBoxIcon);
    vboxLayoutIcon->addWidget(labelIconTheme);
    vboxLayoutIcon->addWidget(comboBoxIconTheme);
    vboxLayoutIcon->addWidget(labelIconSize);
    vboxLayoutIcon->addWidget(comboBoxIconSize);
    groupBoxIcon->setLayout(vboxLayoutIcon);

    /* Mdi Background */
    QGroupBox* groupBoxMdiBG = new QGroupBox(translate("Background"), widget);

    QCheckBox* checkBoxMdiBGUseLogo = create_checkbox(groupBoxMdiBG, GENERAL_MDI_BG_USE_LOGO);
    QCheckBox* checkBoxMdiBGUseTexture = create_checkbox(groupBoxMdiBG, GENERAL_MDI_BG_USE_TEXTURE);
    QCheckBox* checkBoxMdiBGUseColor = create_checkbox(groupBoxMdiBG, GENERAL_MDI_BG_USE_COLOR);

    QPushButton* buttonMdiBGLogo = new QPushButton(translate("Choose"), groupBoxMdiBG);
    buttonMdiBGLogo->setEnabled(setting[GENERAL_MDI_BG_USE_LOGO].dialog.b);
    connect(buttonMdiBGLogo, SIGNAL(clicked()), this, SLOT(chooseGeneralMdiBackgroundLogo()));
    connect(checkBoxMdiBGUseLogo, SIGNAL(toggled(bool)), buttonMdiBGLogo, SLOT(setEnabled(bool)));

    QPushButton* buttonMdiBGTexture = new QPushButton(translate("Choose"), groupBoxMdiBG);
    buttonMdiBGTexture->setEnabled(setting[GENERAL_MDI_BG_USE_TEXTURE].dialog.b);
    connect(buttonMdiBGTexture, SIGNAL(clicked()), this, SLOT(chooseGeneralMdiBackgroundTexture()));
    connect(checkBoxMdiBGUseTexture, SIGNAL(toggled(bool)), buttonMdiBGTexture, SLOT(setEnabled(bool)));

    QPushButton* buttonMdiBGColor = choose_color_button(groupBoxMdiBG,
        GENERAL_MDI_BG_COLOR);
    buttonMdiBGColor->setEnabled(setting[GENERAL_MDI_BG_USE_COLOR].dialog.b);
    connect(checkBoxMdiBGUseColor, SIGNAL(toggled(bool)), buttonMdiBGColor, SLOT(setEnabled(bool)));

    QGridLayout* gridLayoutMdiBG = new QGridLayout(widget);
    gridLayoutMdiBG->addWidget(checkBoxMdiBGUseLogo, 0, 0, Qt::AlignLeft);
    gridLayoutMdiBG->addWidget(buttonMdiBGLogo, 0, 1, Qt::AlignRight);
    gridLayoutMdiBG->addWidget(checkBoxMdiBGUseTexture, 1, 0, Qt::AlignLeft);
    gridLayoutMdiBG->addWidget(buttonMdiBGTexture, 1, 1, Qt::AlignRight);
    gridLayoutMdiBG->addWidget(checkBoxMdiBGUseColor, 2, 0, Qt::AlignLeft);
    gridLayoutMdiBG->addWidget(buttonMdiBGColor, 2, 1, Qt::AlignRight);
    groupBoxMdiBG->setLayout(gridLayoutMdiBG);

    /* Tips */
    QGroupBox* groupBoxTips = new QGroupBox(translate("Tips"), widget);

    QCheckBox* checkBoxTipOfTheDay = create_checkbox(groupBoxTips, GENERAL_TIP_OF_THE_DAY);

    QVBoxLayout* vboxLayoutTips = new QVBoxLayout(groupBoxTips);
    vboxLayoutTips->addWidget(checkBoxTipOfTheDay);
    groupBoxTips->setLayout(vboxLayoutTips);

    /* Help Browser */
    QGroupBox* groupBoxHelpBrowser = new QGroupBox(translate("Help Browser"), widget);

    QRadioButton* radioButtonSystemHelpBrowser = new QRadioButton(translate("System"), groupBoxHelpBrowser);
    radioButtonSystemHelpBrowser->setChecked(get_bool(GENERAL_SYSTEM_HELP_BROWSER));
    QRadioButton* radioButtonCustomHelpBrowser = new QRadioButton(translate("Custom"), groupBoxHelpBrowser);
    radioButtonCustomHelpBrowser->setChecked(!get_bool(GENERAL_SYSTEM_HELP_BROWSER));
    radioButtonCustomHelpBrowser->setEnabled(false); /* TODO: finish this. */

    QVBoxLayout* vboxLayoutHelpBrowser = new QVBoxLayout(groupBoxHelpBrowser);
    vboxLayoutHelpBrowser->addWidget(radioButtonSystemHelpBrowser);
    vboxLayoutHelpBrowser->addWidget(radioButtonCustomHelpBrowser);
    groupBoxHelpBrowser->setLayout(vboxLayoutHelpBrowser);

    /* Widget Layout */
    QVBoxLayout* vboxLayoutMain = new QVBoxLayout(widget);
    vboxLayoutMain->addWidget(groupBoxLanguage);
    vboxLayoutMain->addWidget(groupBoxIcon);
    vboxLayoutMain->addWidget(groupBoxMdiBG);
    vboxLayoutMain->addWidget(groupBoxTips);
    vboxLayoutMain->addWidget(groupBoxHelpBrowser);
    vboxLayoutMain->addStretch(1);
    widget->setLayout(vboxLayoutMain);

    QScrollArea* scrollArea = new QScrollArea(this);
    scrollArea->setWidgetResizable(true);
    scrollArea->setWidget(widget);
    return scrollArea;
}

/* . */
QWidget*
Settings_Dialog::createTabFilesPaths()
{
    QWidget* widget = new QWidget(this);

    QScrollArea* scrollArea = new QScrollArea(this);
    scrollArea->setWidgetResizable(true);
    scrollArea->setWidget(widget);
    return scrollArea;
}

/* . */
QGroupBox*
Settings_Dialog::create_group_box(QWidget* widget, EmbString label, int data[])
{
    QGroupBox* groupbox = new QGroupBox(translate(label), widget);
    QVBoxLayout* vboxLayoutRender = new QVBoxLayout(groupbox);
    for (int i=0; data[i] != TERMINATOR_SYMBOL; i++) {
        QCheckBox* checkBox = create_checkbox(groupbox, settings_data[data[i]].id);
        vboxLayoutRender->addWidget(checkBox);
    }
    groupbox->setLayout(vboxLayoutRender);
    return groupbox;
}

/* . */
QWidget*
Settings_Dialog::createTabDisplay()
{
    QWidget* widget = new QWidget(this);

    /* Rendering */
    /* TODO: Review OpenGL and Rendering settings for future inclusion */
    QGroupBox* groupBoxRender = create_group_box(widget, "Rendering", render_hints);

    /* ScrollBars */
    QGroupBox* groupBoxScrollBars = new QGroupBox(translate("ScrollBars"), widget);

    QCheckBox* checkBoxShowScrollBars = create_checkbox(groupBoxScrollBars,
        DISPLAY_SHOW_SCROLLBARS);

    QLabel* labelScrollBarWidget = new QLabel(translate("Perform action when clicking corner widget"), groupBoxScrollBars);
    QComboBox* comboBoxScrollBarWidget = new QComboBox(groupBoxScrollBars);
    int numActions = actionHash.size();
    for (int i = 0; i < numActions; i++) {
        QAction* action = actionHash[i];
        if (action) {
            comboBoxScrollBarWidget->addItem(action->icon(), action->text().replace("&", ""));
        }
    }
    comboBoxScrollBarWidget->setCurrentIndex(setting[DISPLAY_SCROLLBAR_WIDGET_NUM].dialog.i);
    connect(comboBoxScrollBarWidget, SIGNAL(currentIndexChanged(int)), this, SLOT(comboBoxScrollBarWidgetCurrentIndexChanged(int)));

    QVBoxLayout* vboxLayoutScrollBars = new QVBoxLayout(groupBoxScrollBars);
    vboxLayoutScrollBars->addWidget(checkBoxShowScrollBars);
    vboxLayoutScrollBars->addWidget(labelScrollBarWidget);
    vboxLayoutScrollBars->addWidget(comboBoxScrollBarWidget);
    groupBoxScrollBars->setLayout(vboxLayoutScrollBars);

    /* Colors */
    QGroupBox* groupBoxColor = new QGroupBox(translate("Colors"), widget);

    QLabel* labelCrossHairColor = new QLabel(translate("Crosshair Color"), groupBoxColor);
    QPushButton* buttonCrossHairColor = choose_color_button(groupBoxColor,
        DISPLAY_CROSSHAIR_COLOR);

    QLabel* labelBGColor = new QLabel(translate("Background Color"), groupBoxColor);
    QPushButton* buttonBGColor = choose_color_button(groupBoxColor,
        DISPLAY_BG_COLOR);

    QLabel* labelSelectBoxLeftColor = new QLabel(translate("Selection Box Color (Crossing)"), groupBoxColor);
    QPushButton* buttonSelectBoxLeftColor = choose_color_button(groupBoxColor,
        DISPLAY_SELECTBOX_LEFT_COLOR);

    QLabel* labelSelectBoxLeftFill = new QLabel(translate("Selection Box Fill (Crossing)"), groupBoxColor);
    QPushButton* buttonSelectBoxLeftFill = choose_color_button(groupBoxColor,
        DISPLAY_SELECTBOX_LEFT_FILL);

    QLabel* labelSelectBoxRightColor = new QLabel(translate("Selection Box Color (Window)"), groupBoxColor);
    QPushButton* buttonSelectBoxRightColor = choose_color_button(groupBoxColor,
        DISPLAY_SELECTBOX_RIGHT_COLOR);

    QLabel* labelSelectBoxRightFill = new QLabel(translate("Selection Box Fill (Window)"), groupBoxColor);
    QPushButton* buttonSelectBoxRightFill = choose_color_button(groupBoxColor,
        DISPLAY_SELECTBOX_RIGHT_FILL);

    QLabel* labelSelectBoxAlpha = new QLabel(translate("Selection Box Fill Alpha"), groupBoxColor);
    QSpinBox* spinBoxSelectBoxAlpha = new QSpinBox(groupBoxColor);
    spinBoxSelectBoxAlpha->setRange(0, 255);
    spinBoxSelectBoxAlpha->setValue(setting[DISPLAY_SELECTBOX_ALPHA].preview.i);
    connect(spinBoxSelectBoxAlpha, SIGNAL(valueChanged(int)), this, SLOT(spinBoxDisplaySelectBoxAlphaValueChanged(int)));

    QGridLayout* gridLayoutColor = new QGridLayout(widget);
    gridLayoutColor->addWidget(labelCrossHairColor, 0, 0, Qt::AlignLeft);
    gridLayoutColor->addWidget(buttonCrossHairColor, 0, 1, Qt::AlignRight);
    gridLayoutColor->addWidget(labelBGColor, 1, 0, Qt::AlignLeft);
    gridLayoutColor->addWidget(buttonBGColor, 1, 1, Qt::AlignRight);
    gridLayoutColor->addWidget(labelSelectBoxLeftColor, 2, 0, Qt::AlignLeft);
    gridLayoutColor->addWidget(buttonSelectBoxLeftColor, 2, 1, Qt::AlignRight);
    gridLayoutColor->addWidget(labelSelectBoxLeftFill, 3, 0, Qt::AlignLeft);
    gridLayoutColor->addWidget(buttonSelectBoxLeftFill, 3, 1, Qt::AlignRight);
    gridLayoutColor->addWidget(labelSelectBoxRightColor, 4, 0, Qt::AlignLeft);
    gridLayoutColor->addWidget(buttonSelectBoxRightColor, 4, 1, Qt::AlignRight);
    gridLayoutColor->addWidget(labelSelectBoxRightFill, 5, 0, Qt::AlignLeft);
    gridLayoutColor->addWidget(buttonSelectBoxRightFill, 5, 1, Qt::AlignRight);
    gridLayoutColor->addWidget(labelSelectBoxAlpha, 6, 0, Qt::AlignLeft);
    gridLayoutColor->addWidget(spinBoxSelectBoxAlpha, 6, 1, Qt::AlignRight);
    groupBoxColor->setLayout(gridLayoutColor);

    /* Zoom */
    QGroupBox* groupBoxZoom = new QGroupBox(translate("Zoom"), widget);

    QLabel* labelZoomScaleIn = new QLabel(translate("Zoom In Scale"), groupBoxZoom);
    QDoubleSpinBox* spinBoxZoomScaleIn = create_spinbox(groupBoxZoom, DISPLAY_ZOOMSCALE_IN);

    QLabel* labelZoomScaleOut = new QLabel(translate("Zoom Out Scale"), groupBoxZoom);
    QDoubleSpinBox* spinBoxZoomScaleOut = create_spinbox(groupBoxZoom, DISPLAY_ZOOMSCALE_OUT);

    QGridLayout* gridLayoutZoom = new QGridLayout(groupBoxZoom);
    gridLayoutZoom->addWidget(labelZoomScaleIn, 0, 0, Qt::AlignLeft);
    gridLayoutZoom->addWidget(spinBoxZoomScaleIn, 0, 1, Qt::AlignRight);
    gridLayoutZoom->addWidget(labelZoomScaleOut, 1, 0, Qt::AlignLeft);
    gridLayoutZoom->addWidget(spinBoxZoomScaleOut, 1, 1, Qt::AlignRight);
    groupBoxZoom->setLayout(gridLayoutZoom);

    /* Widget Layout */
    QVBoxLayout *vboxLayoutMain = new QVBoxLayout(widget);
    /* TODO: Review OpenGL and Rendering settings for future inclusion. */
    vboxLayoutMain->addWidget(groupBoxRender);
    vboxLayoutMain->addWidget(groupBoxScrollBars);
    vboxLayoutMain->addWidget(groupBoxColor);
    vboxLayoutMain->addWidget(groupBoxZoom);
    vboxLayoutMain->addStretch(1);
    widget->setLayout(vboxLayoutMain);

    QScrollArea* scrollArea = new QScrollArea(this);
    scrollArea->setWidgetResizable(true);
    scrollArea->setWidget(widget);
    return scrollArea;
}

/* TODO: finish prompt options */
QWidget* Settings_Dialog::createTabPrompt()
{
    QWidget* widget = new QWidget(this);

    /* Colors */
    QGroupBox* groupBoxColor = new QGroupBox(translate("Colors"), widget);
    QLabel* labelTextColor = new QLabel(translate("Text Color"), groupBoxColor);
    QPushButton* buttonTextColor = choose_color_button(groupBoxColor, PROMPT_TEXT_COLOR);

    QLabel* labelBGColor = new QLabel(translate("Background Color"), groupBoxColor);
    QPushButton* buttonBGColor = choose_color_button(groupBoxColor, PROMPT_BG_COLOR);

     QGridLayout* gridLayoutColor = new QGridLayout(widget);
    gridLayoutColor->addWidget(labelTextColor, 0, 0, Qt::AlignLeft);
    gridLayoutColor->addWidget(buttonTextColor, 0, 1, Qt::AlignRight);
    gridLayoutColor->addWidget(labelBGColor, 1, 0, Qt::AlignLeft);
    gridLayoutColor->addWidget(buttonBGColor, 1, 1, Qt::AlignRight);
    groupBoxColor->setLayout(gridLayoutColor);

    /* TODO: Tweak the Prompt Font ComboBoxes so they work properly */
    /* Font */
    QGroupBox* groupBoxFont = new QGroupBox(translate("Font"), widget);

    QLabel* labelFontFamily = new QLabel(translate("Font Family"), groupBoxFont);
    QFontComboBox* comboBoxFontFamily = new QFontComboBox(groupBoxFont);
    comboBoxFontFamily->setCurrentFont(QFont(setting[PROMPT_FONT_FAMILY].preview.s));
    connect(comboBoxFontFamily, SIGNAL(currentIndexChanged(QString )), this, SLOT(comboBoxPromptFontFamilyCurrentIndexChanged(QString )));

    QLabel* labelFontStyle = new QLabel(translate("Font Style"), groupBoxFont);
    QComboBox* comboBoxFontStyle = new QComboBox(groupBoxFont);
    comboBoxFontStyle->addItem("Normal");
    comboBoxFontStyle->addItem("Italic");
    comboBoxFontStyle->setEditText(setting[PROMPT_FONT_STYLE].preview.s);
    connect(comboBoxFontStyle, SIGNAL(currentIndexChanged(QString )), this, SLOT(comboBoxPromptFontStyleCurrentIndexChanged(QString )));

    QLabel* labelFontSize = new QLabel(translate("Font Size"), groupBoxFont);
    QSpinBox* spinBoxFontSize = new QSpinBox(groupBoxFont);
    spinBoxFontSize->setRange(4, 64);
    spinBoxFontSize->setValue(setting[PROMPT_FONT_SIZE].preview.i);
    connect(spinBoxFontSize, SIGNAL(valueChanged(int)), this, SLOT(spinBoxPromptFontSizeValueChanged(int)));

    QGridLayout* gridLayoutFont = new QGridLayout(groupBoxFont);
    gridLayoutFont->addWidget(labelFontFamily, 0, 0, Qt::AlignLeft);
    gridLayoutFont->addWidget(comboBoxFontFamily, 0, 1, Qt::AlignRight);
    gridLayoutFont->addWidget(labelFontStyle, 1, 0, Qt::AlignLeft);
    gridLayoutFont->addWidget(comboBoxFontStyle, 1, 1, Qt::AlignRight);
    gridLayoutFont->addWidget(labelFontSize, 2, 0, Qt::AlignLeft);
    gridLayoutFont->addWidget(spinBoxFontSize, 2, 1, Qt::AlignRight);
    groupBoxFont->setLayout(gridLayoutFont);

    /* History */
    QGroupBox* groupBoxHistory = new QGroupBox(translate("History"), widget);

    QVBoxLayout* vboxLayoutHistory = new QVBoxLayout(groupBoxHistory);
    vboxLayoutHistory->addWidget(create_checkbox(groupBoxHistory, PROMPT_SAVE_HISTORY));
    vboxLayoutHistory->addWidget(create_checkbox(groupBoxHistory, PROMPT_SAVE_HISTORY_AS_HTML));
    groupBoxHistory->setLayout(vboxLayoutHistory);

    /* Widget Layout */
    QVBoxLayout *vboxLayoutMain = new QVBoxLayout(widget);
    vboxLayoutMain->addWidget(groupBoxColor);
    vboxLayoutMain->addWidget(groupBoxFont);
    vboxLayoutMain->addWidget(groupBoxHistory);
    vboxLayoutMain->addStretch(1);
    widget->setLayout(vboxLayoutMain);

    QScrollArea* scrollArea = new QScrollArea(this);
    scrollArea->setWidgetResizable(true);
    scrollArea->setWidget(widget);
    return scrollArea;
}

/* TODO: finish open/save options */
QWidget* Settings_Dialog::createTabOpenSave()
{
    QWidget* widget = new QWidget(this);

    /* Custom Filter */
    QGroupBox* groupBoxCustomFilter = new QGroupBox(translate("Custom Filter"), widget);
    groupBoxCustomFilter->setEnabled(false); /* TODO: Fixup custom filter */

    std::unordered_map<QString, QCheckBox*> custom_filter;

    QPushButton* buttonCustomFilterSelectAll = new QPushButton(translate("Select All"), widget);
    connect(buttonCustomFilterSelectAll, SIGNAL(clicked()), this,
        SLOT(buttonCustomFilterSelectAllClicked()));
    QPushButton* buttonCustomFilterClearAll = new QPushButton("Clear All", widget);
    connect(buttonCustomFilterClearAll, SIGNAL(clicked()), this,
        SLOT(buttonCustomFilterClearAllClicked()));

    int i;
    int n_extensions = string_array_length(extensions);
    for (i=0; i<n_extensions; i++) {
        const char *extension = extensions[i];
        custom_filter[extension] = new QCheckBox(extension, groupBoxCustomFilter);
        custom_filter[extension]->setChecked(QString(setting[OPENSAVE_CUSTOM_FILTER].dialog.s).contains("*." + QString(extension), Qt::CaseInsensitive));
        connect(custom_filter[extension], SIGNAL(stateChanged(int)), this,
            SLOT(checkBoxCustomFilterStateChanged(int)));

        connect(this, SIGNAL(buttonCustomFilterSelectAll(bool)),
            custom_filter[extension], SLOT(setChecked(bool)));

        connect(this, SIGNAL(buttonCustomFilterClearAll(bool)),
            custom_filter[extension], SLOT(setChecked(bool)));
    }

    QGridLayout* gridLayoutCustomFilter = new QGridLayout(groupBoxCustomFilter);
    int row = 0;
    int column = 0;
    for (i=0; i<n_extensions; i++) {
        const char *extension = extensions[i];
        gridLayoutCustomFilter->addWidget(custom_filter[extension], row, column, Qt::AlignLeft);
        row++;
        if (row == 10) {
            row = 0;
            column++;
        }
    }
    gridLayoutCustomFilter->addWidget(buttonCustomFilterClearAll);
    gridLayoutCustomFilter->addWidget(buttonCustomFilterSelectAll);
    gridLayoutCustomFilter->setColumnStretch(6,1);
    groupBoxCustomFilter->setLayout(gridLayoutCustomFilter);

    if (QString(setting[OPENSAVE_CUSTOM_FILTER].dialog.s).contains("supported", Qt::CaseInsensitive)) {
        buttonCustomFilterSelectAllClicked();
    }

    /* Opening */
    QGroupBox* groupBoxOpening = new QGroupBox(translate("File Open"), widget);

    QComboBox* comboBoxOpenFormat = new QComboBox(groupBoxOpening);

    QCheckBox* checkBoxOpenThumbnail = new QCheckBox(translate("Preview Thumbnails"), groupBoxOpening);
    checkBoxOpenThumbnail->setChecked(false);

    /* TODO: Add a button to clear the recent history. */

    QLabel* labelRecentMaxFiles = new QLabel(translate("Number of recently accessed files to show"), groupBoxOpening);
    QSpinBox* spinBoxRecentMaxFiles = new QSpinBox(groupBoxOpening);
    spinBoxRecentMaxFiles->setRange(0, 10);
    spinBoxRecentMaxFiles->setValue(setting[OPENSAVE_RECENT_MAX_FILES].dialog.b);
    connect(spinBoxRecentMaxFiles, SIGNAL(valueChanged(int)), this, SLOT(spinBoxRecentMaxFilesValueChanged(int)));

    QFrame* frameRecent = new QFrame(groupBoxOpening);
    QGridLayout* gridLayoutRecent = new QGridLayout(frameRecent);
    gridLayoutRecent->addWidget(labelRecentMaxFiles, 0, 0, Qt::AlignLeft);
    gridLayoutRecent->addWidget(spinBoxRecentMaxFiles, 0, 1, Qt::AlignRight);
    frameRecent->setLayout(gridLayoutRecent);

    QVBoxLayout* vboxLayoutOpening = new QVBoxLayout(groupBoxOpening);
    vboxLayoutOpening->addWidget(comboBoxOpenFormat);
    vboxLayoutOpening->addWidget(checkBoxOpenThumbnail);
    vboxLayoutOpening->addWidget(frameRecent);
    groupBoxOpening->setLayout(vboxLayoutOpening);

    /* Saving */
    QGroupBox* groupBoxSaving = new QGroupBox(translate("File Save"), widget);

    QComboBox* comboBoxSaveFormat = new QComboBox(groupBoxSaving);

    QCheckBox* checkBoxSaveThumbnail = new QCheckBox(translate("Save Thumbnails"), groupBoxSaving);
    checkBoxSaveThumbnail->setChecked(false);

    QCheckBox* checkBoxAutoSave = new QCheckBox(translate("AutoSave"), groupBoxSaving);
    checkBoxAutoSave->setChecked(false);

    QVBoxLayout* vboxLayoutSaving = new QVBoxLayout(groupBoxSaving);
    vboxLayoutSaving->addWidget(comboBoxSaveFormat);
    vboxLayoutSaving->addWidget(checkBoxSaveThumbnail);
    vboxLayoutSaving->addWidget(checkBoxAutoSave);
    groupBoxSaving->setLayout(vboxLayoutSaving);

    /* Trimming */
    QGroupBox* groupBoxTrim = new QGroupBox(translate("Trimming"), widget);

    QLabel* labelTrimDstNumJumps = new QLabel(translate("DST Only: Minimum number of jumps to trim"), groupBoxTrim);
    QSpinBox* spinBoxTrimDstNumJumps = new QSpinBox(groupBoxTrim);
    spinBoxTrimDstNumJumps->setRange(1, 20);
    spinBoxTrimDstNumJumps->setValue(setting[OPENSAVE_TRIM_DST_NUM_JUMPS].dialog.i);
    connect(spinBoxTrimDstNumJumps, SIGNAL(valueChanged(int)), this, SLOT(spinBoxTrimDstNumJumpsValueChanged(int)));

    QFrame* frameTrimDstNumJumps = new QFrame(groupBoxTrim);
    QGridLayout* gridLayoutTrimDstNumJumps = new QGridLayout(frameTrimDstNumJumps);
    gridLayoutTrimDstNumJumps->addWidget(labelTrimDstNumJumps, 0, 0, Qt::AlignLeft);
    gridLayoutTrimDstNumJumps->addWidget(spinBoxTrimDstNumJumps, 0, 1, Qt::AlignRight);
    frameTrimDstNumJumps->setLayout(gridLayoutTrimDstNumJumps);

    QVBoxLayout* vboxLayoutTrim = new QVBoxLayout(groupBoxTrim);
    vboxLayoutTrim->addWidget(frameTrimDstNumJumps);
    groupBoxTrim->setLayout(vboxLayoutTrim);

    /* Widget Layout */
    QVBoxLayout* vboxLayoutMain = new QVBoxLayout(widget);
    vboxLayoutMain->addWidget(groupBoxCustomFilter);
    vboxLayoutMain->addWidget(groupBoxOpening);
    vboxLayoutMain->addWidget(groupBoxSaving);
    vboxLayoutMain->addWidget(groupBoxTrim);
    vboxLayoutMain->addStretch(1);
    widget->setLayout(vboxLayoutMain);

    QScrollArea* scrollArea = new QScrollArea(this);
    scrollArea->setWidgetResizable(true);
    scrollArea->setWidget(widget);
    return scrollArea;
}

QWidget* Settings_Dialog::createTabPrinting()
{
    QWidget* widget = new QWidget(this);

    /* Default Printer */
    QGroupBox* groupBoxDefaultPrinter = new QGroupBox(translate("Default Printer"), widget);

    QRadioButton* radioButtonUseSame = new QRadioButton(translate("Use as default device"), groupBoxDefaultPrinter);
    radioButtonUseSame->setChecked(!get_bool(PRINTING_USE_LAST_DEVICE));
    QRadioButton* radioButtonUseLast = new QRadioButton(translate("Use last used device"), groupBoxDefaultPrinter);
    radioButtonUseLast->setChecked(get_bool(PRINTING_USE_LAST_DEVICE));

    QComboBox* comboBoxDefaultDevice = new QComboBox(groupBoxDefaultPrinter);
    QList<QPrinterInfo> listAvailPrinters = QPrinterInfo::availablePrinters();
    foreach (QPrinterInfo info, listAvailPrinters) {
        comboBoxDefaultDevice->addItem(create_icon("print"), info.printerName());
    }

    QVBoxLayout* vboxLayoutDefaultPrinter = new QVBoxLayout(groupBoxDefaultPrinter);
    vboxLayoutDefaultPrinter->addWidget(radioButtonUseSame);
    vboxLayoutDefaultPrinter->addWidget(comboBoxDefaultDevice);
    vboxLayoutDefaultPrinter->addWidget(radioButtonUseLast);
    groupBoxDefaultPrinter->setLayout(vboxLayoutDefaultPrinter);

    /* Save Ink */
    QGroupBox* groupBoxSaveInk = new QGroupBox(translate("Save Ink"), widget);

    QVBoxLayout* vboxLayoutSaveInk = new QVBoxLayout(groupBoxSaveInk);
    vboxLayoutSaveInk->addWidget(create_checkbox(groupBoxSaveInk, PRINTING_DISABLE_BG));
    groupBoxSaveInk->setLayout(vboxLayoutSaveInk);

    /* Widget Layout */
    QVBoxLayout* vboxLayoutMain = new QVBoxLayout(widget);
    vboxLayoutMain->addWidget(groupBoxDefaultPrinter);
    vboxLayoutMain->addWidget(groupBoxSaveInk);
    vboxLayoutMain->addStretch(1);
    widget->setLayout(vboxLayoutMain);

    QScrollArea* scrollArea = new QScrollArea(this);
    scrollArea->setWidgetResizable(true);
    scrollArea->setWidget(widget);
    return scrollArea;
}

QWidget* Settings_Dialog::createTabSnap()
{
    QWidget* widget = new QWidget(this);

    /* TODO: finish this */

    QScrollArea* scrollArea = new QScrollArea(this);
    scrollArea->setWidgetResizable(true);
    scrollArea->setWidget(widget);
    return scrollArea;
}

QWidget*
Settings_Dialog::createTabGridRuler()
{
    QWidget* widget = new QWidget(this);

    /* Grid Misc */
    QGroupBox* groupBoxGridMisc = new QGroupBox(translate("Grid Misc"), widget);

    QCheckBox* checkBoxGridShowOnLoad = create_checkbox(groupBoxGridMisc, GRID_SHOW_ON_LOAD);
    QCheckBox* checkBoxGridShowOrigin = create_checkbox(groupBoxGridMisc, GRID_SHOW_ORIGIN);

    QGridLayout* gridLayoutGridMisc = new QGridLayout(widget);
    gridLayoutGridMisc->addWidget(checkBoxGridShowOnLoad, 0, 0, Qt::AlignLeft);
    gridLayoutGridMisc->addWidget(checkBoxGridShowOrigin, 1, 0, Qt::AlignLeft);
    groupBoxGridMisc->setLayout(gridLayoutGridMisc);

    /* Grid Color */
    QGroupBox* groupBoxGridColor = new QGroupBox(translate("Grid Color"), widget);

    QCheckBox* checkBoxGridColorMatchCrossHair = new QCheckBox(
        translate("Match grid color to crosshair color"), groupBoxGridColor);
    checkBoxGridColorMatchCrossHair->setChecked(setting[GRID_COLOR_MATCH_CROSSHAIR].dialog.b);
    connect(checkBoxGridColorMatchCrossHair, SIGNAL(stateChanged(int)), this, SLOT(checkBoxGridColorMatchCrossHairStateChanged(int)));

    QLabel* labelGridColor = new QLabel(translate("Grid Color"), groupBoxGridColor);
    labelGridColor->setObjectName("labelGridColor");
    QPushButton* buttonGridColor = new QPushButton(translate("Choose"), groupBoxGridColor);
    buttonGridColor->setObjectName("buttonGridColor");
    if (setting[GRID_COLOR_MATCH_CROSSHAIR].dialog.b) {
        setting[GRID_COLOR].dialog.i = setting[DISPLAY_CROSSHAIR_COLOR].setting.i;
        setting[GRID_COLOR].preview.i = setting[GRID_COLOR].dialog.i;
        setting[GRID_COLOR].accept.i = setting[GRID_COLOR].dialog.i;
    }
    buttonGridColor->setIcon(QIcon(create_swatch(setting[GRID_COLOR].preview.i)));
    connect(buttonGridColor, &QPushButton::clicked, this,
        [=] () { chooseColor(GRID_COLOR); });

    labelGridColor->setEnabled(!setting[GRID_COLOR_MATCH_CROSSHAIR].dialog.b);
    buttonGridColor->setEnabled(!setting[GRID_COLOR_MATCH_CROSSHAIR].dialog.b);

    QGridLayout* gridLayoutGridColor = new QGridLayout(widget);
    gridLayoutGridColor->addWidget(checkBoxGridColorMatchCrossHair, 0, 0, Qt::AlignLeft);
    gridLayoutGridColor->addWidget(labelGridColor, 1, 0, Qt::AlignLeft);
    gridLayoutGridColor->addWidget(buttonGridColor, 1, 1, Qt::AlignRight);
    groupBoxGridColor->setLayout(gridLayoutGridColor);

    /* Grid Geometry */
    QGroupBox* groupBoxGridGeom = new QGroupBox(translate("Grid Geometry"), widget);

    QCheckBox* checkBoxGridLoadFromFile = new QCheckBox(translate("Set grid size from opened file"), groupBoxGridGeom);
    checkBoxGridLoadFromFile->setChecked(setting[GRID_LOAD_FROM_FILE].dialog.b);
    connect(checkBoxGridLoadFromFile, SIGNAL(stateChanged(int)), this, SLOT(checkBoxGridLoadFromFileStateChanged(int)));

    QLabel* labelGridType = new QLabel(translate("Grid Type"), groupBoxGridGeom);
    labelGridType->setObjectName("labelGridType");
    QComboBox* comboBoxGridType = new QComboBox(groupBoxGridGeom);
    comboBoxGridType->setObjectName("comboBoxGridType");
    comboBoxGridType->addItem("Rectangular");
    comboBoxGridType->addItem("Circular");
    comboBoxGridType->addItem("Isometric");
    comboBoxGridType->setCurrentIndex(comboBoxGridType->findText(setting[GRID_TYPE].dialog.s));
    connect(comboBoxGridType, SIGNAL(currentIndexChanged(QString)), this, SLOT(comboBoxGridTypeCurrentIndexChanged(QString )));

    QCheckBox* checkBoxGridCenterOnOrigin = new QCheckBox(translate("Center the grid on the origin"), groupBoxGridGeom);
    checkBoxGridCenterOnOrigin->setObjectName("checkBoxGridCenterOnOrigin");
    checkBoxGridCenterOnOrigin->setChecked(setting[GRID_CENTER_ON_ORIGIN].dialog.b);
    connect(checkBoxGridCenterOnOrigin, SIGNAL(stateChanged(int)), this, SLOT(checkBoxGridCenterOnOriginStateChanged(int)));

    QLabel* labelGridCenterX = new QLabel(translate("Grid Center X"), groupBoxGridGeom);
    labelGridCenterX->setObjectName("labelGridCenterX");
    QDoubleSpinBox* spinBoxGridCenterX = create_spinbox(groupBoxGridGeom, GRID_CENTER_X);

    QLabel* labelGridCenterY = new QLabel(translate("Grid Center Y"), groupBoxGridGeom);
    labelGridCenterY->setObjectName("labelGridCenterY");
    QDoubleSpinBox* spinBoxGridCenterY = create_spinbox(groupBoxGridGeom, GRID_CENTER_Y);

    QLabel* labelGridSizeX = new QLabel(translate("Grid Size X"), groupBoxGridGeom);
    labelGridSizeX->setObjectName("labelGridSizeX");
    QDoubleSpinBox* spinBoxGridSizeX = create_spinbox(groupBoxGridGeom, GRID_SIZE_X);

    QLabel* labelGridSizeY = new QLabel(translate("Grid Size Y"), groupBoxGridGeom);
    labelGridSizeY->setObjectName("labelGridSizeY");
    QDoubleSpinBox* spinBoxGridSizeY = create_spinbox(groupBoxGridGeom, GRID_SIZE_Y);

    QLabel* labelGridSpacingX = new QLabel(translate("Grid Spacing X"), groupBoxGridGeom);
    labelGridSpacingX->setObjectName("labelGridSpacingX");
    QDoubleSpinBox* spinBoxGridSpacingX = create_spinbox(groupBoxGridGeom, GRID_SPACING_X);

    QLabel* labelGridSpacingY = new QLabel(translate("Grid Spacing Y"), groupBoxGridGeom);
    labelGridSpacingY->setObjectName("labelGridSpacingY");
    QDoubleSpinBox* spinBoxGridSpacingY = create_spinbox(groupBoxGridGeom, GRID_SPACING_Y);

    QLabel* labelGridSizeRadius = new QLabel(translate("Grid Size Radius"), groupBoxGridGeom);
    labelGridSizeRadius->setObjectName("labelGridSizeRadius");
    QDoubleSpinBox* spinBoxGridSizeRadius = create_spinbox(groupBoxGridGeom, GRID_SIZE_RADIUS);

    QLabel* labelGridSpacingRadius = new QLabel(translate("Grid Spacing Radius"), groupBoxGridGeom);
    labelGridSpacingRadius->setObjectName("labelGridSpacingRadius");
    QDoubleSpinBox* spinBoxGridSpacingRadius = create_spinbox(groupBoxGridGeom, GRID_SPACING_RADIUS);

    QLabel* labelGridSpacingAngle = new QLabel(translate("Grid Spacing Angle"), groupBoxGridGeom);
    labelGridSpacingAngle->setObjectName("labelGridSpacingAngle");
    QDoubleSpinBox* spinBoxGridSpacingAngle = create_spinbox(groupBoxGridGeom, GRID_SPACING_ANGLE);

    bool disable = setting[GRID_LOAD_FROM_FILE].dialog.b;
    labelGridType->setEnabled(disable);
    comboBoxGridType->setEnabled(disable);
    checkBoxGridCenterOnOrigin->setEnabled(disable);
    labelGridCenterX->setEnabled(disable);
    spinBoxGridCenterX->setEnabled(disable);
    labelGridCenterY->setEnabled(disable);
    spinBoxGridCenterY->setEnabled(disable);
    labelGridSizeX->setEnabled(disable);
    spinBoxGridSizeX->setEnabled(disable);
    labelGridSizeY->setEnabled(disable);
    spinBoxGridSizeY->setEnabled(disable);
    labelGridSpacingX->setEnabled(disable);
    spinBoxGridSpacingX->setEnabled(disable);
    labelGridSpacingY->setEnabled(disable);
    spinBoxGridSpacingY->setEnabled(disable);
    labelGridSizeRadius->setEnabled(disable);
    spinBoxGridSizeRadius->setEnabled(disable);
    labelGridSpacingRadius->setEnabled(disable);
    spinBoxGridSpacingRadius->setEnabled(disable);
    labelGridSpacingAngle->setEnabled(disable);
    spinBoxGridSpacingAngle->setEnabled(disable);

    bool visibility = false;
    if (QString(setting[GRID_TYPE].dialog.s) == "Circular") {
        visibility = true;
    }
    labelGridSizeX->setVisible(!visibility);
    spinBoxGridSizeX->setVisible(!visibility);
    labelGridSizeY->setVisible(!visibility);
    spinBoxGridSizeY->setVisible(!visibility);
    labelGridSpacingX->setVisible(!visibility);
    spinBoxGridSpacingX->setVisible(!visibility);
    labelGridSpacingY->setVisible(!visibility);
    spinBoxGridSpacingY->setVisible(!visibility);
    labelGridSizeRadius->setVisible(visibility);
    spinBoxGridSizeRadius->setVisible(visibility);
    labelGridSpacingRadius->setVisible(visibility);
    spinBoxGridSpacingRadius->setVisible(visibility);
    labelGridSpacingAngle->setVisible(visibility);
    spinBoxGridSpacingAngle->setVisible(visibility);

    QGridLayout* gridLayoutGridGeom = new QGridLayout(groupBoxGridGeom);
    gridLayoutGridGeom->addWidget(checkBoxGridLoadFromFile, 0, 0, Qt::AlignLeft);
    gridLayoutGridGeom->addWidget(labelGridType, 1, 0, Qt::AlignLeft);
    gridLayoutGridGeom->addWidget(comboBoxGridType, 1, 1, Qt::AlignRight);
    gridLayoutGridGeom->addWidget(checkBoxGridCenterOnOrigin, 2, 0, Qt::AlignLeft);
    gridLayoutGridGeom->addWidget(labelGridCenterX, 3, 0, Qt::AlignLeft);
    gridLayoutGridGeom->addWidget(spinBoxGridCenterX, 3, 1, Qt::AlignRight);
    gridLayoutGridGeom->addWidget(labelGridCenterY, 4, 0, Qt::AlignLeft);
    gridLayoutGridGeom->addWidget(spinBoxGridCenterY, 4, 1, Qt::AlignRight);
    gridLayoutGridGeom->addWidget(labelGridSizeX, 5, 0, Qt::AlignLeft);
    gridLayoutGridGeom->addWidget(spinBoxGridSizeX, 5, 1, Qt::AlignRight);
    gridLayoutGridGeom->addWidget(labelGridSizeY, 6, 0, Qt::AlignLeft);
    gridLayoutGridGeom->addWidget(spinBoxGridSizeY, 6, 1, Qt::AlignRight);
    gridLayoutGridGeom->addWidget(labelGridSpacingX, 7, 0, Qt::AlignLeft);
    gridLayoutGridGeom->addWidget(spinBoxGridSpacingX, 7, 1, Qt::AlignRight);
    gridLayoutGridGeom->addWidget(labelGridSpacingY, 8, 0, Qt::AlignLeft);
    gridLayoutGridGeom->addWidget(spinBoxGridSpacingY, 8, 1, Qt::AlignRight);
    gridLayoutGridGeom->addWidget(labelGridSizeRadius, 9, 0, Qt::AlignLeft);
    gridLayoutGridGeom->addWidget(spinBoxGridSizeRadius, 9, 1, Qt::AlignRight);
    gridLayoutGridGeom->addWidget(labelGridSpacingRadius, 10, 0, Qt::AlignLeft);
    gridLayoutGridGeom->addWidget(spinBoxGridSpacingRadius, 10, 1, Qt::AlignRight);
    gridLayoutGridGeom->addWidget(labelGridSpacingAngle, 11, 0, Qt::AlignLeft);
    gridLayoutGridGeom->addWidget(spinBoxGridSpacingAngle, 11, 1, Qt::AlignRight);
    groupBoxGridGeom->setLayout(gridLayoutGridGeom);

    /* Ruler Misc */
    QGroupBox* groupBoxRulerMisc = new QGroupBox(translate("Ruler Misc"), widget);

    QCheckBox* checkBoxRulerShowOnLoad = create_checkbox(groupBoxRulerMisc,
        RULER_SHOW_ON_LOAD);

    QLabel* labelRulerMetric = new QLabel(translate("Ruler Units"), groupBoxRulerMisc);
    QComboBox* comboBoxRulerMetric = new QComboBox(groupBoxRulerMisc);
    comboBoxRulerMetric->addItem("Imperial", false);
    comboBoxRulerMetric->addItem("Metric", true);
    comboBoxRulerMetric->setCurrentIndex(comboBoxRulerMetric->findData(setting[RULER_METRIC].dialog.i));
    connect(comboBoxRulerMetric, SIGNAL(currentIndexChanged(int)), this, SLOT(comboBoxRulerMetricCurrentIndexChanged(int)));

    QGridLayout* gridLayoutRulerMisc = new QGridLayout(widget);
    gridLayoutRulerMisc->addWidget(checkBoxRulerShowOnLoad, 0, 0, Qt::AlignLeft);
    gridLayoutRulerMisc->addWidget(labelRulerMetric, 1, 0, Qt::AlignLeft);
    gridLayoutRulerMisc->addWidget(comboBoxRulerMetric, 1, 1, Qt::AlignRight);
    groupBoxRulerMisc->setLayout(gridLayoutRulerMisc);

    /* Ruler Color */
    QGroupBox* groupBoxRulerColor = new QGroupBox(translate("Ruler Color"), widget);

    QLabel* labelRulerColor = new QLabel(translate("Ruler Color"), groupBoxRulerColor);
    labelRulerColor->setObjectName("labelRulerColor");
    QPushButton* buttonRulerColor = new QPushButton(translate("Choose"), groupBoxRulerColor);
    buttonRulerColor->setObjectName("buttonRulerColor");
    buttonRulerColor->setIcon(create_swatch(setting[RULER_COLOR].preview.i));
    connect(buttonRulerColor, &QPushButton::clicked, this,
        [=] () { chooseColor(RULER_COLOR); });

    QGridLayout* gridLayoutRulerColor = new QGridLayout(widget);
    gridLayoutRulerColor->addWidget(labelRulerColor, 1, 0, Qt::AlignLeft);
    gridLayoutRulerColor->addWidget(buttonRulerColor, 1, 1, Qt::AlignRight);
    groupBoxRulerColor->setLayout(gridLayoutRulerColor);

    /* Ruler Geometry */
    QGroupBox* groupBoxRulerGeom = new QGroupBox(translate("Ruler Geometry"), widget);

    QLabel* labelRulerPixelSize = new QLabel(translate("Ruler Pixel Size"), groupBoxRulerGeom);
    labelRulerPixelSize->setObjectName("labelRulerPixelSize");
    QSpinBox* spinBoxRulerPixelSize = create_int_spinbox(groupBoxRulerGeom, RULER_PIXEL_SIZE);

    QGridLayout* gridLayoutRulerGeom = new QGridLayout(groupBoxRulerGeom);
    gridLayoutRulerGeom->addWidget(labelRulerPixelSize, 0, 0, Qt::AlignLeft);
    gridLayoutRulerGeom->addWidget(spinBoxRulerPixelSize, 0, 1, Qt::AlignRight);
    groupBoxRulerGeom->setLayout(gridLayoutRulerGeom);

    /* Widget Layout */
    QVBoxLayout *vboxLayoutMain = new QVBoxLayout(widget);
    vboxLayoutMain->addWidget(groupBoxGridMisc);
    vboxLayoutMain->addWidget(groupBoxGridColor);
    vboxLayoutMain->addWidget(groupBoxGridGeom);
    vboxLayoutMain->addWidget(groupBoxRulerMisc);
    vboxLayoutMain->addWidget(groupBoxRulerColor);
    vboxLayoutMain->addWidget(groupBoxRulerGeom);
    vboxLayoutMain->addStretch(1);
    widget->setLayout(vboxLayoutMain);

    QScrollArea* scrollArea = new QScrollArea(this);
    scrollArea->setWidgetResizable(true);
    scrollArea->setWidget(widget);
    return scrollArea;
}

QWidget* Settings_Dialog::createTabOrthoPolar()
{
    QWidget* widget = new QWidget(this);

    /* TODO: finish this */

    QScrollArea* scrollArea = new QScrollArea(this);
    scrollArea->setWidgetResizable(true);
    scrollArea->setWidget(widget);
    return scrollArea;
}

QWidget*
Settings_Dialog::createTabQuickSnap()
{
    QWidget* widget = new QWidget(this);

    /* QSnap Locators */
    QGroupBox* groupBoxQSnapLoc = new QGroupBox(translate("Locators Used"), widget);

    QCheckBox* checkBoxQSnapEndPoint = create_checkbox(groupBoxQSnapLoc, QSNAP_ENDPOINT);
    QCheckBox* checkBoxQSnapMidPoint = create_checkbox(groupBoxQSnapLoc, QSNAP_MIDPOINT);
    QCheckBox* checkBoxQSnapCenter = create_checkbox(groupBoxQSnapLoc, QSNAP_CENTER);
    QCheckBox* checkBoxQSnapNode = create_checkbox(groupBoxQSnapLoc, QSNAP_NODE);
    QCheckBox* checkBoxQSnapQuadrant = create_checkbox(groupBoxQSnapLoc, QSNAP_QUADRANT);
    QCheckBox* checkBoxQSnapIntersection = create_checkbox(groupBoxQSnapLoc, QSNAP_INTERSECTION);
    QCheckBox* checkBoxQSnapExtension = create_checkbox(groupBoxQSnapLoc, QSNAP_EXTENSION);
    QCheckBox* checkBoxQSnapInsertion = create_checkbox(groupBoxQSnapLoc, QSNAP_INSERTION);
    QCheckBox* checkBoxQSnapPerpendicular = create_checkbox(groupBoxQSnapLoc, QSNAP_PERPENDICULAR);
    QCheckBox* checkBoxQSnapTangent = create_checkbox(groupBoxQSnapLoc, QSNAP_TANGENT);
    QCheckBox* checkBoxQSnapNearest = create_checkbox(groupBoxQSnapLoc, QSNAP_NEAREST);
    QCheckBox* checkBoxQSnapApparent = create_checkbox(groupBoxQSnapLoc, QSNAP_APPARENT);
    QCheckBox* checkBoxQSnapParallel = create_checkbox(groupBoxQSnapLoc, QSNAP_PARALLEL);

    QPushButton* buttonQSnapSelectAll = new QPushButton(translate("Select All"), groupBoxQSnapLoc);
    connect(buttonQSnapSelectAll, SIGNAL(clicked()), this, SLOT(buttonQSnapSelectAllClicked()));
    connect(this, SIGNAL(buttonQSnapSelectAll(bool)), checkBoxQSnapEndPoint, SLOT(setChecked(bool)));
    connect(this, SIGNAL(buttonQSnapSelectAll(bool)), checkBoxQSnapMidPoint, SLOT(setChecked(bool)));
    connect(this, SIGNAL(buttonQSnapSelectAll(bool)), checkBoxQSnapCenter, SLOT(setChecked(bool)));
    connect(this, SIGNAL(buttonQSnapSelectAll(bool)), checkBoxQSnapNode, SLOT(setChecked(bool)));
    connect(this, SIGNAL(buttonQSnapSelectAll(bool)), checkBoxQSnapQuadrant, SLOT(setChecked(bool)));
    connect(this, SIGNAL(buttonQSnapSelectAll(bool)), checkBoxQSnapIntersection, SLOT(setChecked(bool)));
    connect(this, SIGNAL(buttonQSnapSelectAll(bool)), checkBoxQSnapExtension, SLOT(setChecked(bool)));
    connect(this, SIGNAL(buttonQSnapSelectAll(bool)), checkBoxQSnapInsertion, SLOT(setChecked(bool)));
    connect(this, SIGNAL(buttonQSnapSelectAll(bool)), checkBoxQSnapPerpendicular, SLOT(setChecked(bool)));
    connect(this, SIGNAL(buttonQSnapSelectAll(bool)), checkBoxQSnapTangent, SLOT(setChecked(bool)));
    connect(this, SIGNAL(buttonQSnapSelectAll(bool)), checkBoxQSnapNearest, SLOT(setChecked(bool)));
    connect(this, SIGNAL(buttonQSnapSelectAll(bool)), checkBoxQSnapApparent, SLOT(setChecked(bool)));
    connect(this, SIGNAL(buttonQSnapSelectAll(bool)), checkBoxQSnapParallel, SLOT(setChecked(bool)));

    QPushButton* buttonQSnapClearAll = new QPushButton(translate("Clear All"), groupBoxQSnapLoc);
    connect(buttonQSnapClearAll, SIGNAL(clicked()), this, SLOT(buttonQSnapClearAllClicked()));
    connect(this, SIGNAL(buttonQSnapClearAll(bool)), checkBoxQSnapEndPoint, SLOT(setChecked(bool)));
    connect(this, SIGNAL(buttonQSnapClearAll(bool)), checkBoxQSnapMidPoint, SLOT(setChecked(bool)));
    connect(this, SIGNAL(buttonQSnapClearAll(bool)), checkBoxQSnapCenter, SLOT(setChecked(bool)));
    connect(this, SIGNAL(buttonQSnapClearAll(bool)), checkBoxQSnapNode, SLOT(setChecked(bool)));
    connect(this, SIGNAL(buttonQSnapClearAll(bool)), checkBoxQSnapQuadrant, SLOT(setChecked(bool)));
    connect(this, SIGNAL(buttonQSnapClearAll(bool)), checkBoxQSnapIntersection, SLOT(setChecked(bool)));
    connect(this, SIGNAL(buttonQSnapClearAll(bool)), checkBoxQSnapExtension, SLOT(setChecked(bool)));
    connect(this, SIGNAL(buttonQSnapClearAll(bool)), checkBoxQSnapInsertion, SLOT(setChecked(bool)));
    connect(this, SIGNAL(buttonQSnapClearAll(bool)), checkBoxQSnapPerpendicular, SLOT(setChecked(bool)));
    connect(this, SIGNAL(buttonQSnapClearAll(bool)), checkBoxQSnapTangent, SLOT(setChecked(bool)));
    connect(this, SIGNAL(buttonQSnapClearAll(bool)), checkBoxQSnapNearest, SLOT(setChecked(bool)));
    connect(this, SIGNAL(buttonQSnapClearAll(bool)), checkBoxQSnapApparent, SLOT(setChecked(bool)));
    connect(this, SIGNAL(buttonQSnapClearAll(bool)), checkBoxQSnapParallel, SLOT(setChecked(bool)));

    QGridLayout* gridLayoutQSnap = new QGridLayout(groupBoxQSnapLoc);
    gridLayoutQSnap->addWidget(checkBoxQSnapEndPoint, 0, 0, Qt::AlignLeft);
    gridLayoutQSnap->addWidget(checkBoxQSnapMidPoint, 1, 0, Qt::AlignLeft);
    gridLayoutQSnap->addWidget(checkBoxQSnapCenter, 2, 0, Qt::AlignLeft);
    gridLayoutQSnap->addWidget(checkBoxQSnapNode, 3, 0, Qt::AlignLeft);
    gridLayoutQSnap->addWidget(checkBoxQSnapQuadrant, 4, 0, Qt::AlignLeft);
    gridLayoutQSnap->addWidget(checkBoxQSnapIntersection, 5, 0, Qt::AlignLeft);
    gridLayoutQSnap->addWidget(checkBoxQSnapExtension, 6, 0, Qt::AlignLeft);
    gridLayoutQSnap->addWidget(checkBoxQSnapInsertion, 0, 1, Qt::AlignLeft);
    gridLayoutQSnap->addWidget(checkBoxQSnapPerpendicular, 1, 1, Qt::AlignLeft);
    gridLayoutQSnap->addWidget(checkBoxQSnapTangent, 2, 1, Qt::AlignLeft);
    gridLayoutQSnap->addWidget(checkBoxQSnapNearest, 3, 1, Qt::AlignLeft);
    gridLayoutQSnap->addWidget(checkBoxQSnapApparent, 4, 1, Qt::AlignLeft);
    gridLayoutQSnap->addWidget(checkBoxQSnapParallel, 5, 1, Qt::AlignLeft);
    gridLayoutQSnap->addWidget(buttonQSnapSelectAll, 0, 2, Qt::AlignLeft);
    gridLayoutQSnap->addWidget(buttonQSnapClearAll, 1, 2, Qt::AlignLeft);
    gridLayoutQSnap->setColumnStretch(2,1);
    groupBoxQSnapLoc->setLayout(gridLayoutQSnap);

    /* QSnap Visual Config */
    QGroupBox* groupBoxQSnapVisual = new QGroupBox(translate("Visual Configuration"), widget);

    QLabel* labelQSnapLocColor = new QLabel(translate("Locator Color"), groupBoxQSnapVisual);
    QComboBox* comboBoxQSnapLocColor = new QComboBox(groupBoxQSnapVisual);
    addColorsToComboBox(comboBoxQSnapLocColor);
    comboBoxQSnapLocColor->setCurrentIndex(comboBoxQSnapLocColor->findData(setting[QSNAP_LOCATOR_COLOR].dialog.i));
    connect(comboBoxQSnapLocColor, SIGNAL(currentIndexChanged(int)), this, SLOT(comboBoxQSnapLocatorColorCurrentIndexChanged(int)));

    QLabel* labelQSnapLocSize = new QLabel(translate("Locator Size"), groupBoxQSnapVisual);
    QSlider* sliderQSnapLocSize = new QSlider(Qt::Horizontal, groupBoxQSnapVisual);
    sliderQSnapLocSize->setRange(1,20);
    sliderQSnapLocSize->setValue(setting[QSNAP_LOCATOR_SIZE].dialog.i);
    connect(sliderQSnapLocSize, SIGNAL(valueChanged(int)), this, SLOT(sliderQSnapLocatorSizeValueChanged(int)));

    QVBoxLayout* vboxLayoutQSnapVisual = new QVBoxLayout(groupBoxQSnapVisual);
    vboxLayoutQSnapVisual->addWidget(labelQSnapLocColor);
    vboxLayoutQSnapVisual->addWidget(comboBoxQSnapLocColor);
    vboxLayoutQSnapVisual->addWidget(labelQSnapLocSize);
    vboxLayoutQSnapVisual->addWidget(sliderQSnapLocSize);
    groupBoxQSnapVisual->setLayout(vboxLayoutQSnapVisual);

    /* QSnap Sensitivity Config */
    QGroupBox* groupBoxQSnapSensitivity = new QGroupBox(translate("Sensitivity"), widget);

    QLabel* labelQSnapApertureSize = new QLabel(translate("Aperture Size"), groupBoxQSnapSensitivity);
    QSlider* sliderQSnapApertureSize = new QSlider(Qt::Horizontal, groupBoxQSnapSensitivity);
    sliderQSnapApertureSize->setRange(1,20);
    sliderQSnapApertureSize->setValue(setting[QSNAP_APERTURE_SIZE].dialog.i);
    connect(sliderQSnapApertureSize, SIGNAL(valueChanged(int)), this, SLOT(sliderQSnapApertureSizeValueChanged(int)));

    QVBoxLayout* vboxLayoutQSnapSensitivity = new QVBoxLayout(groupBoxQSnapSensitivity);
    vboxLayoutQSnapSensitivity->addWidget(labelQSnapApertureSize);
    vboxLayoutQSnapSensitivity->addWidget(sliderQSnapApertureSize);
    groupBoxQSnapSensitivity->setLayout(vboxLayoutQSnapSensitivity);

    /* Widget Layout */
    QVBoxLayout *vboxLayoutMain = new QVBoxLayout(widget);
    vboxLayoutMain->addWidget(groupBoxQSnapLoc);
    vboxLayoutMain->addWidget(groupBoxQSnapVisual);
    vboxLayoutMain->addWidget(groupBoxQSnapSensitivity);
    vboxLayoutMain->addStretch(1);
    widget->setLayout(vboxLayoutMain);

    QScrollArea* scrollArea = new QScrollArea(this);
    scrollArea->setWidgetResizable(true);
    scrollArea->setWidget(widget);
    return scrollArea;
}

QWidget*
Settings_Dialog::createTabQuickTrack()
{
    QWidget* widget = new QWidget(this);

    /* TODO: finish this */

    QScrollArea* scrollArea = new QScrollArea(this);
    scrollArea->setWidgetResizable(true);
    scrollArea->setWidget(widget);
    return scrollArea;
}

QWidget*
Settings_Dialog::createTabLineWeight()
{
    QWidget* widget = new QWidget(this);

    /* TODO: finish this */

    /* Misc */
    QGroupBox* groupBoxLwtMisc = new QGroupBox(translate("LineWeight Misc"), widget);

    QGraphicsScene* s = activeScene();

    QCheckBox* checkBoxShowLwt = new QCheckBox(translate("Show LineWeight"), groupBoxLwtMisc);
    if (s) {
        setting[LWT_SHOW_LWT].dialog.b = s->property("ENABLE_LWT").toBool();
    }
    else {
        setting[LWT_SHOW_LWT].dialog.b = setting[LWT_SHOW_LWT].setting.b;
    }
    setting[LWT_SHOW_LWT].preview.b = setting[LWT_SHOW_LWT].dialog.b;
    checkBoxShowLwt->setChecked(setting[LWT_SHOW_LWT].preview.b);
    connect(checkBoxShowLwt, SIGNAL(stateChanged(int)), this, SLOT(checkBoxLwtShowLwtStateChanged(int)));

    QCheckBox* checkBoxRealRender = new QCheckBox(translate("RealRender"), groupBoxLwtMisc);
    checkBoxRealRender->setObjectName("checkBoxRealRender");
    if (s) {
        setting[LWT_REAL_RENDER].dialog.b = s->property("ENABLE_REAL").toBool();
    }
    else {
        setting[LWT_REAL_RENDER].dialog.b = setting[LWT_REAL_RENDER].setting.b;
    }
    setting[LWT_REAL_RENDER].preview.b = setting[LWT_REAL_RENDER].dialog.b;
    checkBoxRealRender->setChecked(setting[LWT_REAL_RENDER].preview.b);
    connect(checkBoxRealRender, SIGNAL(stateChanged(int)), this, SLOT(checkBoxLwtRealRenderStateChanged(int)));
    checkBoxRealRender->setEnabled(setting[LWT_SHOW_LWT].dialog.b);

    QLabel* labelDefaultLwt = new QLabel(translate("Default weight"), groupBoxLwtMisc);
    labelDefaultLwt->setEnabled(false); /* TODO: remove later. */
    QComboBox* comboBoxDefaultLwt = new QComboBox(groupBoxLwtMisc);
    /* TODO: populate the comboBox and set the initial value. */
    comboBoxDefaultLwt->addItem(QString().setNum(setting[LWT_DEFAULT_LWT].dialog.r, 'F', 2).append(" mm"), setting[LWT_DEFAULT_LWT].dialog.r);
    comboBoxDefaultLwt->setEnabled(false); /* TODO: remove later */

    QVBoxLayout* vboxLayoutLwtMisc = new QVBoxLayout(groupBoxLwtMisc);
    vboxLayoutLwtMisc->addWidget(checkBoxShowLwt);
    vboxLayoutLwtMisc->addWidget(checkBoxRealRender);
    vboxLayoutLwtMisc->addWidget(labelDefaultLwt);
    vboxLayoutLwtMisc->addWidget(comboBoxDefaultLwt);
    groupBoxLwtMisc->setLayout(vboxLayoutLwtMisc);

    /* Widget Layout. */
    QVBoxLayout *vboxLayoutMain = new QVBoxLayout(widget);
    vboxLayoutMain->addWidget(groupBoxLwtMisc);
    vboxLayoutMain->addStretch(1);
    widget->setLayout(vboxLayoutMain);

    QScrollArea* scrollArea = new QScrollArea(this);
    scrollArea->setWidgetResizable(true);
    scrollArea->setWidget(widget);
    return scrollArea;
}

QWidget* Settings_Dialog::createTabSelection()
{
    QWidget* widget = new QWidget(this);

    /* Selection Modes */
    QGroupBox* groupBoxSelectionModes = new QGroupBox(translate("Modes"), widget);

    QCheckBox* checkBoxSelectionModePickFirst = create_checkbox(groupBoxSelectionModes, SELECTION_MODE_PICKFIRST);
    checkBoxSelectionModePickFirst->setEnabled(false);
    /* TODO: Remove this line when Post-selection is available. */

    QCheckBox* checkBoxSelectionModePickAdd = create_checkbox(groupBoxSelectionModes,
        SELECTION_MODE_PICKADD);

    QCheckBox* checkBoxSelectionModePickDrag = create_checkbox(groupBoxSelectionModes,
        SELECTION_MODE_PICKDRAG);
    checkBoxSelectionModePickDrag->setEnabled(false);
    /* TODO: Remove this line when this functionality is available. */

    QVBoxLayout* vboxLayoutSelectionModes = new QVBoxLayout(groupBoxSelectionModes);
    vboxLayoutSelectionModes->addWidget(checkBoxSelectionModePickFirst);
    vboxLayoutSelectionModes->addWidget(checkBoxSelectionModePickAdd);
    vboxLayoutSelectionModes->addWidget(checkBoxSelectionModePickDrag);
    groupBoxSelectionModes->setLayout(vboxLayoutSelectionModes);

    /* Selection Colors */
    QGroupBox* groupBoxSelectionColors = new QGroupBox(translate("Colors"), widget);

    QLabel* labelCoolGripColor = new QLabel(translate("Cool Grip (Unselected)"), groupBoxSelectionColors);
    QComboBox* comboBoxCoolGripColor = new QComboBox(groupBoxSelectionColors);
    addColorsToComboBox(comboBoxCoolGripColor);
    comboBoxCoolGripColor->setCurrentIndex(comboBoxCoolGripColor->findData(setting[SELECTION_COOLGRIP_COLOR].dialog.i));
    connect(comboBoxCoolGripColor, SIGNAL(currentIndexChanged(int)), this, SLOT(comboBoxSelectionCoolGripColorCurrentIndexChanged(int)));

    QLabel* labelHotGripColor = new QLabel(translate("Hot Grip (Selected)"), groupBoxSelectionColors);
    QComboBox* comboBoxHotGripColor = new QComboBox(groupBoxSelectionColors);
    addColorsToComboBox(comboBoxHotGripColor);
    comboBoxHotGripColor->setCurrentIndex(comboBoxHotGripColor->findData(setting[SELECTION_HOTGRIP_COLOR].dialog.i));
    connect(comboBoxHotGripColor, SIGNAL(currentIndexChanged(int)), this, SLOT(comboBoxSelectionHotGripColorCurrentIndexChanged(int)));

    QVBoxLayout* vboxLayoutSelectionColors = new QVBoxLayout(groupBoxSelectionColors);
    vboxLayoutSelectionColors->addWidget(labelCoolGripColor);
    vboxLayoutSelectionColors->addWidget(comboBoxCoolGripColor);
    vboxLayoutSelectionColors->addWidget(labelHotGripColor);
    vboxLayoutSelectionColors->addWidget(comboBoxHotGripColor);
    groupBoxSelectionColors->setLayout(vboxLayoutSelectionColors);

    /* Selection Sizes */
    QGroupBox* groupBoxSelectionSizes = new QGroupBox(translate("Sizes"), widget);

    QLabel* labelSelectionGripSize = new QLabel(translate("Grip Size"), groupBoxSelectionSizes);
    QSlider* sliderSelectionGripSize = new QSlider(Qt::Horizontal, groupBoxSelectionSizes);
    sliderSelectionGripSize->setRange(1,20);
    sliderSelectionGripSize->setValue(setting[SELECTION_GRIP_SIZE].dialog.i);
    connect(sliderSelectionGripSize, SIGNAL(valueChanged(int)), this, SLOT(sliderSelectionGripSizeValueChanged(int)));

    QLabel* labelSelectionPickBoxSize = new QLabel(translate("Pickbox Size"), groupBoxSelectionSizes);
    QSlider* sliderSelectionPickBoxSize = new QSlider(Qt::Horizontal, groupBoxSelectionSizes);
    sliderSelectionPickBoxSize->setRange(1,20);
    sliderSelectionPickBoxSize->setValue(setting[SELECTION_PICKBOX_SIZE].dialog.i);
    connect(sliderSelectionPickBoxSize, SIGNAL(valueChanged(int)), this, SLOT(sliderSelectionPickBoxSizeValueChanged(int)));

    QVBoxLayout* vboxLayoutSelectionSizes = new QVBoxLayout(groupBoxSelectionSizes);
    vboxLayoutSelectionSizes->addWidget(labelSelectionGripSize);
    vboxLayoutSelectionSizes->addWidget(sliderSelectionGripSize);
    vboxLayoutSelectionSizes->addWidget(labelSelectionPickBoxSize);
    vboxLayoutSelectionSizes->addWidget(sliderSelectionPickBoxSize);
    groupBoxSelectionSizes->setLayout(vboxLayoutSelectionSizes);

    /* Widget Layout */
    QVBoxLayout *vboxLayoutMain = new QVBoxLayout(widget);
    vboxLayoutMain->addWidget(groupBoxSelectionModes);
    vboxLayoutMain->addWidget(groupBoxSelectionColors);
    vboxLayoutMain->addWidget(groupBoxSelectionSizes);
    vboxLayoutMain->addStretch(1);
    widget->setLayout(vboxLayoutMain);

    QScrollArea* scrollArea = new QScrollArea(this);
    scrollArea->setWidgetResizable(true);
    scrollArea->setWidget(widget);
    return scrollArea;
}

void
Settings_Dialog::addColorsToComboBox(QComboBox* comboBox)
{
    comboBox->addItem(create_icon("colorred"), translate("Red"), qRgb(255, 0, 0));
    comboBox->addItem(create_icon("coloryellow"), translate("Yellow"), qRgb(255, 255, 0));
    comboBox->addItem(create_icon("colorgreen"), translate("Green"), qRgb(0, 255, 0));
    comboBox->addItem(create_icon("colorcyan"), translate("Cyan"), qRgb(0, 255, 255));
    comboBox->addItem(create_icon("colorblue"), translate("Blue"), qRgb(0, 0, 255));
    comboBox->addItem(create_icon("colormagenta"), translate("Magenta"), qRgb(255, 0, 255));
    comboBox->addItem(create_icon("colorwhite"), translate("White"), qRgb(255, 255, 255));
    /* TODO: Add Other... so the user can select custom colors */
}

void
Settings_Dialog::comboBoxLanguageCurrentIndexChanged(QString lang)
{
    strcpy(setting[GENERAL_LANGUAGE].dialog.s, qPrintable(lang.toLower()));
}

void
Settings_Dialog::comboBoxIconThemeCurrentIndexChanged(QString theme)
{
    strcpy(setting[GENERAL_ICON_THEME].dialog.s, qPrintable(theme));
}

void
Settings_Dialog::comboBoxIconSizeCurrentIndexChanged(int index)
{
    QComboBox* comboBox = qobject_cast<QComboBox*>(sender());
    if (comboBox) {
        bool ok = 0;
        setting[GENERAL_ICON_SIZE].dialog.i = comboBox->itemData(index).toUInt(&ok);
        if (!ok) {
            setting[GENERAL_ICON_SIZE].dialog.i = 16;
        }
    }
    else {
        setting[GENERAL_ICON_SIZE].dialog.i = 16;
    }
}

void
preview_update(void)
{
    mdiArea->useBackgroundLogo(setting[GENERAL_MDI_BG_USE_LOGO].preview.b);
    mdiArea->useBackgroundColor(setting[GENERAL_MDI_BG_USE_COLOR].preview.b);
    mdiArea->useBackgroundTexture(setting[GENERAL_MDI_BG_USE_TEXTURE].preview.b);
    updateAllViewScrollBars(setting[DISPLAY_SHOW_SCROLLBARS].preview.b);
}

void
Settings_Dialog::chooseGeneralMdiBackgroundLogo()
{
    QPushButton* button = qobject_cast<QPushButton*>(sender());
    if (button) {
        QString selectedImage = QFileDialog::getOpenFileName(this,
            translate("Open File"),
            QStandardPaths::writableLocation(QStandardPaths::PicturesLocation),
            translate("Images (*.bmp *.png *.jpg)"));

        if (!selectedImage.isNull()) {
            strcpy(setting[GENERAL_MDI_BG_LOGO].accept.s, qPrintable(selectedImage));
        }

        /* Update immediately so it can be previewed */
        mdiArea->setBackgroundLogo(setting[GENERAL_MDI_BG_LOGO].accept.s);
    }
}

void
Settings_Dialog::chooseGeneralMdiBackgroundTexture()
{
    QPushButton* button = qobject_cast<QPushButton*>(sender());
    if (button) {
        QString selectedImage;
        selectedImage = QFileDialog::getOpenFileName(this, translate("Open File"),
            QStandardPaths::writableLocation(QStandardPaths::PicturesLocation),
            translate("Images (*.bmp *.png *.jpg)"));

        if (!selectedImage.isNull()) {
            strcpy(setting[GENERAL_MDI_BG_TEXTURE].accept.s, qPrintable(selectedImage));
        }

        /* Update immediately so it can be previewed */
        mdiArea->setBackgroundTexture(setting[GENERAL_MDI_BG_TEXTURE].accept.s);
    }
}

/* . */
void
Settings_Dialog::chooseColor(int key)
{
    QPushButton* button = qobject_cast<QPushButton*>(sender());
    if (!button) {
        return;
    }
    QColorDialog* colorDialog = new QColorDialog(QColor(setting[key].accept.i), this);
    connect(colorDialog, &QColorDialog::currentColorChanged, this,
        [=](const QColor& color) { currentColorChanged_(key, color.rgb()); });
    colorDialog->exec();

    if (colorDialog->result() == QDialog::Accepted) {
        setting[key].accept.i = colorDialog->selectedColor().rgb();
        button->setIcon(create_swatch(setting[key].accept.i));
        switch (key) {
        case DISPLAY_CROSSHAIR_COLOR:
            updateAllViewCrossHairColors(setting[key].accept.i);
            break;
        case DISPLAY_BG_COLOR:
            updateAllViewBackgroundColors(setting[key].accept.i);
            break;
        case DISPLAY_SELECTBOX_LEFT_COLOR:
        case DISPLAY_SELECTBOX_LEFT_FILL:
        case DISPLAY_SELECTBOX_RIGHT_COLOR:
        case DISPLAY_SELECTBOX_RIGHT_FILL:
            updateAllViewSelectBoxColors(
                setting[DISPLAY_SELECTBOX_LEFT_COLOR].accept.i,
                setting[DISPLAY_SELECTBOX_LEFT_FILL].accept.i,
                setting[DISPLAY_SELECTBOX_RIGHT_COLOR].accept.i,
                setting[DISPLAY_SELECTBOX_RIGHT_FILL].accept.i,
                setting[DISPLAY_SELECTBOX_ALPHA].preview.i);
            break;
        case PROMPT_TEXT_COLOR:
            setPromptTextColor(setting[key].accept.i);
            break;
        case PROMPT_BG_COLOR:
            setPromptBackgroundColor(setting[key].accept.i);
            break;
        case GRID_COLOR:
            updateAllViewGridColors(setting[GRID_COLOR].accept.i);
            break;
        case RULER_COLOR:
            updateAllViewRulerColors(setting[key].accept.i);
            break;
        case GENERAL_MDI_BG_COLOR:
            mdiArea->setBackgroundColor(QColor(setting[key].accept.i));
            break;
        default:
            break;
        }
    }
    else {
        switch (key) {
        case DISPLAY_CROSSHAIR_COLOR:
            updateAllViewCrossHairColors(setting[key].dialog.i);
            break;
        case DISPLAY_BG_COLOR:
            updateAllViewBackgroundColors(setting[key].dialog.i);
            break;
        case DISPLAY_SELECTBOX_LEFT_COLOR:
        case DISPLAY_SELECTBOX_LEFT_FILL:
        case DISPLAY_SELECTBOX_RIGHT_COLOR:
        case DISPLAY_SELECTBOX_RIGHT_FILL:
            updateAllViewSelectBoxColors(
                setting[DISPLAY_SELECTBOX_LEFT_COLOR].dialog.i,
                setting[DISPLAY_SELECTBOX_LEFT_FILL].dialog.i,
                setting[DISPLAY_SELECTBOX_RIGHT_COLOR].dialog.i,
                setting[DISPLAY_SELECTBOX_RIGHT_FILL].dialog.i,
                setting[DISPLAY_SELECTBOX_ALPHA].preview.i);
            break;
        case PROMPT_TEXT_COLOR:
            setPromptTextColor(setting[key].dialog.i);
            break;
        case PROMPT_BG_COLOR:
            setPromptBackgroundColor(setting[key].dialog.i);
            break;
        case GRID_COLOR:
            updateAllViewGridColors(setting[GRID_COLOR].dialog.i);
            break;
        case RULER_COLOR:
            updateAllViewRulerColors(setting[key].dialog.i);
            break;
        case GENERAL_MDI_BG_COLOR:
            mdiArea->setBackgroundColor(QColor(setting[key].dialog.i));
            break;
        default:
            break;
        }
    }
}

/* . */
void
currentColorChanged_(int key, uint32_t color)
{
    setting[key].preview.i = color;
    switch (key) {
    case DISPLAY_CROSSHAIR_COLOR:
        updateAllViewCrossHairColors(setting[key].preview.i);
        break;
    case DISPLAY_BG_COLOR:
        updateAllViewBackgroundColors(setting[key].preview.i);
        break;
    case DISPLAY_SELECTBOX_LEFT_COLOR:
    case DISPLAY_SELECTBOX_LEFT_FILL:
    case DISPLAY_SELECTBOX_RIGHT_COLOR:
    case DISPLAY_SELECTBOX_RIGHT_FILL:
        updateAllViewSelectBoxColors(
            setting[DISPLAY_SELECTBOX_LEFT_COLOR].preview.i,
            setting[DISPLAY_SELECTBOX_LEFT_FILL].preview.i,
            setting[DISPLAY_SELECTBOX_RIGHT_COLOR].preview.i,
            setting[DISPLAY_SELECTBOX_RIGHT_FILL].preview.i,
            setting[DISPLAY_SELECTBOX_ALPHA].preview.i);
        break;
    case PROMPT_TEXT_COLOR:
        setPromptTextColor(setting[key].preview.i);
        break;
    case PROMPT_BG_COLOR:
        setPromptBackgroundColor(setting[key].preview.i);
        break;
    case GRID_COLOR:
        updateAllViewGridColors(setting[GRID_COLOR].preview.i);
        break;
    case RULER_COLOR:
        updateAllViewRulerColors(setting[key].preview.i);
        break;
    case GENERAL_MDI_BG_COLOR:
        mdiArea->setBackgroundColor(QColor(setting[key].preview.i));
        break;
    default:
        break;
    }
}

/* . */
void
Settings_Dialog::checkBoxCustomFilterStateChanged(int checked)
{
    QCheckBox* checkBox = qobject_cast<QCheckBox*>(sender());
    if (checkBox) {
        EmbString message;
        QString format = checkBox->text();
        sprintf(message, "CustomFilter: %s %d", qPrintable(format), checked);
        debug_message(message);
        if (checked) {
            strcat(setting[OPENSAVE_CUSTOM_FILTER].dialog.s,
                qPrintable(" *." + format.toLower()));
        }
        else {
            QString s;
            s = QString(setting[OPENSAVE_CUSTOM_FILTER].dialog.s).remove("*." + format, Qt::CaseInsensitive);
            strcpy(setting[OPENSAVE_CUSTOM_FILTER].dialog.s, qPrintable(s));
        }
        /* setting[OPENSAVE_USE_CUSTOM_FILTER].dialog.s = checked; */ /* TODO */
    }
}

void
Settings_Dialog::buttonCustomFilterSelectAllClicked()
{
    emit buttonCustomFilterSelectAll(true);
    strcpy(setting[OPENSAVE_CUSTOM_FILTER].dialog.s, "supported");
}

void
Settings_Dialog::buttonCustomFilterClearAllClicked()
{
    emit buttonCustomFilterClearAll(false);
    strcpy(setting[OPENSAVE_CUSTOM_FILTER].dialog.s, "");
}

/* . */
void
Settings_Dialog::checkBoxGridColorMatchCrossHairStateChanged(int checked)
{
    setting[GRID_COLOR_MATCH_CROSSHAIR].dialog.b = checked;
    if (setting[GRID_COLOR_MATCH_CROSSHAIR].dialog.b) {
        updateAllViewGridColors(setting[DISPLAY_CROSSHAIR_PERCENT].accept.i);
    }
    else {
        updateAllViewGridColors(setting[GRID_COLOR].accept.i);
    }

    QObject* senderObj = sender();
    if (senderObj)
    {
        QObject* parent = senderObj->parent();
        if (parent)
        {
            QLabel* labelGridColor = parent->findChild<QLabel*>("labelGridColor");
            if (labelGridColor) labelGridColor->setEnabled(!setting[GRID_COLOR_MATCH_CROSSHAIR].dialog.b);
            QPushButton* buttonGridColor = parent->findChild<QPushButton*>("buttonGridColor");
            if (buttonGridColor) buttonGridColor->setEnabled(!setting[GRID_COLOR_MATCH_CROSSHAIR].dialog.b);
        }
    }
}

/* . */
void
Settings_Dialog::checkBoxGridLoadFromFileStateChanged(int checked)
{
    setting[GRID_LOAD_FROM_FILE].dialog.b = checked;

    QObject* senderObj = sender();
    if (!senderObj) {
        return;
    }

    bool dont_load = !setting[GRID_LOAD_FROM_FILE].dialog.b;
    set_enabled_group(senderObj, grid_load_from_file_group, dont_load);

    bool use_this_origin = !setting[GRID_LOAD_FROM_FILE].dialog.b && !setting[GRID_CENTER_ON_ORIGIN].dialog.b;
    set_enabled_group(senderObj, defined_origin_group, use_this_origin);
}

/* . */
void
Settings_Dialog::comboBoxGridTypeCurrentIndexChanged(QString type)
{
    strcpy(setting[GRID_TYPE].dialog.s, qPrintable(type));

    QObject* senderObj = sender();
    if (!senderObj) {
        return;
    }
    bool visibility = (type == "Circular");
    set_visibility_group(senderObj, rectangular_grid_group, !visibility);
    set_visibility_group(senderObj, circular_grid_group, visibility);
}

/* . */
void
Settings_Dialog::checkBoxGridCenterOnOriginStateChanged(int checked)
{
    setting[GRID_CENTER_ON_ORIGIN].dialog.b = checked;

    QObject* senderObj = sender();
    if (senderObj) {
        set_enabled_group(senderObj, center_on_origin_group, !setting[GRID_CENTER_ON_ORIGIN].dialog.b);
    }
}

/* . */
void
Settings_Dialog::comboBoxRulerMetricCurrentIndexChanged(int index)
{
    QComboBox* comboBox = qobject_cast<QComboBox*>(sender());
    if (comboBox) {
        setting[RULER_METRIC].dialog.i = comboBox->itemData(index).toBool();
    }
    else {
        setting[RULER_METRIC].dialog.i = true;
    }
}

/* . */
void
Settings_Dialog::buttonQSnapSelectAllClicked()
{
    emit buttonQSnapSelectAll(true);
}

/* . */
void
Settings_Dialog::buttonQSnapClearAllClicked()
{
    emit buttonQSnapClearAll(false);
}

/* TODO: Figure out how to abstract the slot in a way that it can be used for comboBoxes in general
 *      Currently comboBoxQSnapLocatorColorCurrentIndexChanged(int index)
 *                comboBoxSelectionCoolGripColorCurrentIndexChanged(int index)
 *                comboBoxSelectionHotGripColorCurrentIndexChanged(int index)
 *      are all similar except the dialog_ variable being worked on and the QVariant.
 */

void
Settings_Dialog::comboBoxQSnapLocatorColorCurrentIndexChanged(int index)
{
    /* TODO: Alert user if color matched the display bg color. */
    QComboBox* comboBox = qobject_cast<QComboBox*>(sender());
    QRgb defaultColor = qRgb(255,255,0); /* Yellow */
    if (comboBox) {
        bool ok = 0;
        setting[QSNAP_LOCATOR_COLOR].dialog.i = comboBox->itemData(index).toUInt(&ok);
        if (!ok) {
            setting[QSNAP_LOCATOR_COLOR].dialog.i = defaultColor;
        }
    }
    else {
        setting[QSNAP_LOCATOR_COLOR].dialog.i = defaultColor;
    }
}

/* . */
void
Settings_Dialog::checkBoxLwtShowLwtStateChanged(int checked)
{
    setting[LWT_SHOW_LWT].preview.b = checked;
    if (setting[LWT_SHOW_LWT].preview.b) {
        enableLwt();
    }
    else {
        disableLwt();
    }

    QObject* senderObj = sender();
    if (senderObj) {
        QObject* parent = senderObj->parent();
        if (parent) {
            QCheckBox* checkBoxRealRender = parent->findChild<QCheckBox*>("checkBoxRealRender");
            if (checkBoxRealRender)
                checkBoxRealRender->setEnabled(setting[LWT_SHOW_LWT].preview.b);
        }
    }
}

/* . */
void
Settings_Dialog::comboBoxSelectionCoolGripColorCurrentIndexChanged(int index)
{
    /* TODO: Alert user if color matched the display bg color */
    QComboBox* comboBox = qobject_cast<QComboBox*>(sender());
    QRgb defaultColor = qRgb(0,0,255); /* Blue */
    if (comboBox) {
        bool ok = 0;
        setting[SELECTION_COOLGRIP_COLOR].dialog.i = comboBox->itemData(index).toUInt(&ok);
        if (!ok) {
            setting[SELECTION_COOLGRIP_COLOR].dialog.i = defaultColor;
        }
    }
    else {
        setting[SELECTION_COOLGRIP_COLOR].dialog.i = defaultColor;
    }
}

/* . */
void
Settings_Dialog::comboBoxSelectionHotGripColorCurrentIndexChanged(int index)
{
    /* TODO: Alert user if color matched the display bg color. */
    QComboBox* comboBox = qobject_cast<QComboBox*>(sender());
    QRgb defaultColor = qRgb(255,0,0); /* Red */
    if (comboBox) {
        bool ok = 0;
        setting[SELECTION_HOTGRIP_COLOR].dialog.i = comboBox->itemData(index).toUInt(&ok);
        if (!ok) {
            setting[SELECTION_HOTGRIP_COLOR].dialog.i = defaultColor;
        }
    }
    else {
        setting[SELECTION_HOTGRIP_COLOR].dialog.i = defaultColor;
    }
}

void
update_view(void)
{
    mdiArea->useBackgroundLogo(setting[GENERAL_MDI_BG_USE_LOGO].dialog.b);
    mdiArea->useBackgroundTexture(setting[GENERAL_MDI_BG_USE_TEXTURE].dialog.b);
    mdiArea->useBackgroundColor(setting[GENERAL_MDI_BG_USE_COLOR].dialog.b);
    mdiArea->setBackgroundLogo(setting[GENERAL_MDI_BG_LOGO].dialog.s);
    mdiArea->setBackgroundTexture(setting[GENERAL_MDI_BG_TEXTURE].dialog.s);
    mdiArea->setBackgroundColor(setting[GENERAL_MDI_BG_COLOR].dialog.i);

    iconResize(setting[GENERAL_ICON_SIZE].dialog.i);

    updateAllViewScrollBars(setting[DISPLAY_SHOW_SCROLLBARS].dialog.b);
    updateAllViewCrossHairColors(setting[DISPLAY_CROSSHAIR_COLOR].dialog.i);
    updateAllViewBackgroundColors(setting[DISPLAY_BG_COLOR].dialog.i);
    updateAllViewSelectBoxColors(
        setting[DISPLAY_SELECTBOX_LEFT_COLOR].dialog.i,
        setting[DISPLAY_SELECTBOX_LEFT_FILL].dialog.i,
        setting[DISPLAY_SELECTBOX_RIGHT_COLOR].dialog.i,
        setting[DISPLAY_SELECTBOX_RIGHT_FILL].dialog.i,
        setting[DISPLAY_SELECTBOX_ALPHA].dialog.i);
    updateAllViewGridColors(setting[GRID_COLOR].dialog.i);
    updateAllViewRulerColors(setting[RULER_COLOR].dialog.i);

    setPromptTextColor(setting[PROMPT_TEXT_COLOR].dialog.i);
    setPromptBackgroundColor(setting[PROMPT_BG_COLOR].dialog.i);
    setPromptFontFamily(setting[PROMPT_FONT_FAMILY].dialog.s);
    setPromptFontStyle(setting[PROMPT_FONT_STYLE].dialog.s);
    setPromptFontSize(setting[PROMPT_FONT_SIZE].dialog.i);

    if (setting[LWT_SHOW_LWT].dialog.b) {
        enableLwt();
    }
    else {
        disableLwt();
    }
    if (setting[LWT_REAL_RENDER].dialog.b) {
        enableReal();
    }
    else {
        disableReal();
    }

    updatePickAddMode(setting[SELECTION_MODE_PICKADD].dialog.b);
}

/* . */
void
Settings_Dialog::acceptChanges()
{
    if (setting[GRID_COLOR_MATCH_CROSSHAIR].dialog.b) {
        setting[GRID_COLOR].dialog.i = setting[DISPLAY_CROSSHAIR_COLOR].accept.i;
    }

    for (int i=0; preview_to_dialog[i] != TERMINATOR_SYMBOL; i++) {
        copy_setting(preview_to_dialog[i], SETTING_DIALOG, SETTING_PREVIEW);
    }

    for (int i=0; accept_to_dialog[i] != TERMINATOR_SYMBOL; i++) {
        copy_setting(accept_to_dialog[i], SETTING_DIALOG, SETTING_ACCEPT);
    }

    for (int i=0; i < N_SETTINGS; i++) {
        copy_setting(i, SETTING_SETTING, SETTING_DIALOG);
    }

    /* Make sure the user sees the changes applied immediately. */
    update_view();

    writeSettings();
    accept();
}

/* TODO: inform the user if they have changed settings */
void
Settings_Dialog::rejectChanges()
{
    /* Update the view since the user must accept the preview */
    update_view();

    reject();
}

#include "widgets.moc"

