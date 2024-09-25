/*
 * Embroidermodder 2.
 *
 * Copyright 2011-2024 The Embroidermodder Team
 * Embroidermodder 2 is Open Source Software, see LICENSE.md for licensing terms.
 *
 * Read the reference manual (https://www.libembroidery.org/downloads/emrm.pdf)
 * for advice on altering this file.
 *
 * Main Embroidermodder Header
 */

#ifndef EMBROIDERMODDER_H
#define EMBROIDERMODDER_H

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
#include <QTimer>
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
#include <string>
#include <vector>
#include <unordered_map>
#include <chrono>
#include <thread>

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

extern MainWindow *_main;
extern std::unordered_map<std::string, std::string> aliasHash;
extern std::unordered_map<int, QAction*> actionHash;
extern QToolBar* toolbar[N_TOOLBARS];
extern QMenu* menu[N_MENUS];
extern QToolButton* statusBarSnapButton;
extern QToolButton* statusBarGridButton;
extern QToolButton* statusBarRulerButton;
extern QToolButton* statusBarOrthoButton;
extern QToolButton* statusBarPolarButton;
extern QToolButton* statusBarQSnapButton;
extern QToolButton* statusBarQTrackButton;
extern QToolButton* statusBarLwtButton;
extern QLabel* statusBarMouseCoord;
extern QStatusBar* statusbar;
extern MdiArea* mdiArea;
extern CmdPrompt* prompt;
extern PropertyEditor* dockPropEdit;
extern UndoEditor* dockUndoEdit;
extern QTimer* testing_timer;
extern QAction* myFileSeparator;
extern QWizard* wizardTipOfTheDay;
extern QLabel* labelTipOfTheDay;
extern QCheckBox* checkBoxTipOfTheDay;
extern QComboBox* layerSelector;
extern QComboBox* colorSelector;
extern QComboBox* linetypeSelector;
extern QComboBox* lineweightSelector;
extern QFontComboBox* textFontSelector;
extern QComboBox* textSizeSelector;
extern QList<QGraphicsItem*> cutCopyObjectList;
extern QStringList objectTextJustifyList;

extern QString curText;
extern QString defaultPrefix;
extern QString prefix;
extern QString lastCmd;
extern QString curCmd;

extern QTextBrowser* promptHistory;
extern CmdPromptInput* promptInput;

extern QString formatFilterOpen;
extern QString formatFilterSave;
extern QString openFilesPath;
extern QByteArray layoutState;

extern QStringList button_list;

extern QString prompt_color_;
extern QString prompt_selection_bg_color_;
extern QString prompt_bg_color_;
extern QString prompt_selection_color_;

extern QTimer* blinkTimer;
extern bool blinkState;

extern std::unordered_map<int, int> key_map;

extern Document *documents[MAX_OPEN_FILES];
extern bool document_memory[MAX_OPEN_FILES];

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

QString getCurrentLayer();
QRgb getCurrentColor();
QString getCurrentLineType();
QString getCurrentLineWeight();

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

void obj_set_text(Object *obj, QString str);
void obj_set_text_font(Object *obj, QString font);
void obj_set_text_justify(Object *obj, QString justify);
void obj_set_text_size(Object *obj, double size);
void obj_set_text_style(Object *obj, bool bold, bool italic, bool under, bool strike, bool over);
void obj_set_text_bold(Object *obj, bool val);
void obj_set_text_italic(Object *obj, bool val);
void obj_set_text_underline(Object *obj, bool val);
void obj_set_text_strikeout(Object *obj, bool val);
void obj_set_text_overline(Object *obj, bool val);
void obj_set_text_backward(Object *obj, bool val);
void obj_set_text_upside_down(Object *obj, bool val);

/* ---------------------- Document Functions --------------------------- */

Document *create_doc(MainWindow* mw, QGraphicsScene* theScene, QWidget *parent);

void doc_set_grid_color(int32_t doc, QRgb color);
void doc_create_grid(int32_t doc, EmbString gridType);
void doc_set_ruler_color(int32_t doc, QRgb color);

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

void doc_copy_selected(int32_t doc);

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
    QColor gridColor;
    QColor qsnapLocatorColor;
    QColor crosshairColor;
    QColor rulerColor;

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
    void currentColorChanged_(int key, const QColor& color);
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
    void checkBoxLwtRealRenderStateChanged(int);

    void comboBoxLanguageCurrentIndexChanged(QString);
    void comboBoxIconThemeCurrentIndexChanged(QString);
    void comboBoxIconSizeCurrentIndexChanged(int);
    void chooseGeneralMdiBackgroundLogo();
    void chooseGeneralMdiBackgroundTexture();
    void comboBoxScrollBarWidgetCurrentIndexChanged(int);
    void spinBoxDisplaySelectBoxAlphaValueChanged(int);
    void comboBoxPromptFontFamilyCurrentIndexChanged(EmbString);
    void comboBoxPromptFontStyleCurrentIndexChanged(EmbString);
    void spinBoxPromptFontSizeValueChanged(int);
    void buttonCustomFilterSelectAllClicked();
    void buttonCustomFilterClearAllClicked();
    void spinBoxRecentMaxFilesValueChanged(int);
    void spinBoxTrimDstNumJumpsValueChanged(int);
    void comboBoxGridTypeCurrentIndexChanged(QString);
    void comboBoxRulerMetricCurrentIndexChanged(int);
    void spinBoxRulerPixelSizeValueChanged(double);
    void buttonQSnapSelectAllClicked();
    void buttonQSnapClearAllClicked();
    void comboBoxQSnapLocatorColorCurrentIndexChanged(int);
    void sliderQSnapLocatorSizeValueChanged(int);
    void sliderQSnapApertureSizeValueChanged(int);
    void comboBoxSelectionCoolGripColorCurrentIndexChanged(int);
    void comboBoxSelectionHotGripColorCurrentIndexChanged(int);
    void sliderSelectionGripSizeValueChanged(int);
    void sliderSelectionPickBoxSizeValueChanged(int);

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

    bool fileWasLoaded;

    QString promptHistory;
    QList<QString> promptInputList;
    int promptInputNum;

    QPrinter printer;

    int myIndex;

    QString curFile;
    QString curLayer;
    QRgb curColor;
    QString curLineType;
    QString curLineWeight;

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

    void showViewScrollBars(bool val);
    void setViewCrossHairColor(QRgb color);
    void setViewBackgroundColor(QRgb color);
    void setViewSelectBoxColors(QRgb colorL, QRgb fillL, QRgb colorR, QRgb fillR, int alpha);
    void setViewGridColor(QRgb color);
    void setViewRulerColor(QRgb color);

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
    void setMainWin(MainWindow* mainWin) { _main = mainWin; }
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
    void setCurrentText(QString txt)
    {
        curText = prefix + txt;
        promptInput->setText(curText);
    }
    void setPrefix(QString txt);
    void enableRapidFire() { rapidFireEnabled = true; }
    void disableRapidFire() { rapidFireEnabled = false; }
    bool isRapidFireEnabled() { return rapidFireEnabled; }

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

#endif

