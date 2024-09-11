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
#include <QHash>
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
#include <QtGlobal>
#include <QtGui>
#include <QtPrintSupport>

#include <QOpenGLWidget>

#include <QTimer>

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
extern const char *settings_file;
extern QByteArray layoutState;

extern QStringList button_list;

extern QString prompt_color_;
extern QString prompt_selection_bg_color_;
extern QString prompt_bg_color_;
extern QString prompt_selection_color_;

extern QTimer* blinkTimer;
extern bool blinkState;

extern std::unordered_map<int, int> key_map;

void appendHistory(QString txt);

void create_statusbar(MainWindow* mw);

/* . */
typedef struct DocumentData_ {
    QHash<int64_t, QGraphicsItem*> hashDeletedObjects;

    QList<int64_t> spareRubberList;

    QColor gridColor;
    QPainterPath gridPath;
    QPainterPath originPath;

    bool grippingActive;
    bool rapidMoveActive;
    bool previewActive;
    bool pastingActive;
    bool movingActive;
    bool selectingActive;
    bool zoomWindowActive;
    bool panningRealTimeActive;
    bool panningPointActive;
    bool panningActive;
    bool qSnapActive;
    bool qSnapToggle;

    QGraphicsItemGroup* previewObjectItemGroup;
    EmbVector previewPoint;
    double previewData;
    int previewMode;

    EmbVector viewMousePoint;
    EmbVector sceneMousePoint;
    QRgb qsnapLocatorColor;
    uint8_t qsnapLocatorSize;
    uint8_t qsnapApertureSize;
    QRgb gripColorCool;
    QRgb gripColorHot;
    uint8_t gripSize;
    uint8_t pickBoxSize;
    QRgb crosshairColor;
    uint32_t crosshairSize;

    Object* gripBaseObj;
    Object* tempBaseObj;

    QGraphicsScene* gscene;
    QUndoStack* undoStack;

    SelectBox* selectBox;
    EmbVector scenePressPoint;
    EmbVector pressPoint;
    EmbVector sceneMovePoint;
    EmbVector movePoint;
    EmbVector sceneReleasePoint;
    EmbVector releasePoint;
    EmbVector sceneGripPoint;

    EmbVector cutCopyMousePoint;
    QGraphicsItemGroup* pasteObjectItemGroup;
    EmbVector pasteDelta;

    QList<QGraphicsItem*> rubberRoomList;
    int panDistance;
    int panStartX;
    int panStartY;

    QList<QGraphicsItem*> previewObjectList;
    bool rulerMetric;
    QColor rulerColor;
    uint8_t rulerPixelSize;
} DocumentData;

typedef struct UndoData_ {
    int type;
    Object* object;
    Document* gview;
    EmbVector delta;
    EmbVector pivot;
    double angle;
    double factor;
    QString navType;
    QTransform fromTransform;
    QTransform toTransform;
    EmbVector fromCenter;
    EmbVector toCenter;
    bool done;
    EmbVector before;
    EmbVector after;
    QLineF mirrorLine;
} UndoData;

typedef struct ObjectData_ {
    QGraphicsPathItem path;
    int32_t TYPE;
    QString OBJ_NAME;
    QPen objPen;
    QPen lwtPen;
    QLineF objLine;
    int objRubberMode;
    QHash<QString, EmbVector> objRubberPoints;
    QHash<QString, QString> objRubberTexts;
    int64_t objID;

    QString objText;
    QString objTextFont;
    QString objTextJustify;
    double objTextSize;
    bool objTextBold;
    bool objTextItalic;
    bool objTextUnderline;
    bool objTextStrikeOut;
    bool objTextOverline;
    bool objTextBackward;
    bool objTextUpsideDown;
    QPainterPath objTextPath;

    bool curved;
    bool filled;
    QPainterPath lineStylePath;
    QPainterPath arrowStylePath;
    double arrowStyleAngle;
    double arrowStyleLength;
    double lineStyleAngle;
    double lineStyleLength;

    QPainterPath normalPath;
    int gripIndex;
} ObjectData;

MdiWindow* activeMdiWindow();
Document* activeDocument();
QGraphicsScene* activeScene();
QUndoStack* activeUndoStack();
QString platformString();

QToolButton *create_statusbarbutton(QString buttonText, MainWindow* mw);
QIcon create_icon(QString icon);
QPixmap create_pixmap(QString icon);

void nativeAddPolygon(double startX, double startY, const QPainterPath& p, int rubberMode);
void nativeAddPolyline(double startX, double startY, const QPainterPath& p, int rubberMode);
void nativeAddPath(double startX, double startY, const QPainterPath& p, int rubberMode);

void nativeAlert(std::string txt);
void nativeAppendPromptHistory(std::string txt);

void nativeAddTextMulti(std::string str, double x, double y, double rot, bool fill, int rubberMode);
void nativeAddTextSingle(std::string str, double x, double y, double rot, bool fill, int rubberMode);

void nativeAddImage(std::string img, double x, double y, double w, double h, double rot);

void nativeAddToSelection(const QPainterPath path, Qt::ItemSelectionMode mode);

QAction *get_action_by_icon(const char *icon);

EmbVector to_emb_vector(QPointF p);
QPointF to_qpointf(EmbVector v);
EmbVector scale_and_rotate(EmbVector v, double angle, double scale);
EmbVector find_mouse_snap_point(QList<EmbVector> snap_points, EmbVector mouse_point);

void set_visibility(QObject *senderObj, const char *key, bool visibility);
void set_enabled(QObject *senderObj, const char *key, bool visibility);
void set_visibility_group(QObject *senderObj, char *key[], bool visibility);
void set_enabled_group(QObject *senderObj, char *key[], bool visibility);
QIcon create_swatch(int32_t color);
void preview_update(void);

QString getCurrentLayer();
QRgb getCurrentColor();
QString getCurrentLineType();
QString getCurrentLineWeight();

void setHistory(QString  txt);
void add_command(std::string alias, std::string cmd);

/* ------------------------ Object Functions --------------------------- */

Object *create_arc(EmbArc arc, QRgb rgb, QGraphicsItem *item=0);
Object *create_circle(EmbCircle circle, QRgb rgb, QGraphicsItem *item=0);
Object *create_ellipse(EmbEllipse ellipse, QRgb rgb, QGraphicsItem *item=0);
Object *create_polyline(EmbPath path, const QPainterPath& p, QRgb rgb, QGraphicsItem* parent=0);
Object *create_path(double x, double y, const QPainterPath p, QRgb rgb, QGraphicsItem* parent=0);
Object *create_polygon(double x, double y, const QPainterPath& p, QRgb rgb, QGraphicsItem* parent=0);
Object *create_text_single(QString  str, double x, double y, QRgb rgb, QGraphicsItem* parent=0);
Object *create_dim_leader(double x1, double y1, double x2, double y2, QRgb rgb, QGraphicsItem* parent=0);
Object *create_image(double x, double y, double w, double h, QRgb rgb, QGraphicsItem* parent=0);
Object *create_rect(double x, double y, double w, double h, QRgb rgb, QGraphicsItem* parent=0);
Object *create_line(double x1, double y1, double x2, double y2, QRgb rgb, QGraphicsItem* parent=0);
Object *create_point(EmbPoint_ point, QRgb rgb, QGraphicsItem* parent=0);

Object *copy_object(Object* obj);

QColor obj_color(Object* obj);
QRgb obj_color_rgb(Object* obj);
Qt::PenStyle obj_line_type(Object* obj);
double  obj_line_weight(Object* obj);
QPainterPath obj_path(Object* obj);
EmbVector obj_rubber_point(Object* obj, QString  key);
QString obj_rubber_text(Object* obj, QString  key);

EmbVector obj_end_point_1(Object *obj);
EmbVector obj_end_point_2(Object *obj);
EmbVector obj_start_point(Object *obj);
EmbVector obj_mid_point(Object *obj);
EmbVector obj_end_point(Object *obj);

EmbVector obj_pos(Object *obj);
double obj_x(Object *obj);
double obj_y(Object *obj);
EmbVector obj_center(Object *obj);
double obj_center_x(Object *obj);
double obj_center_y(Object *obj);
double obj_radius(Object *obj);
double obj_diameter(Object *obj);
double obj_circumference(Object *obj);
EmbVector obj_delta(Object *obj);

EmbVector obj_top_left(Object *obj);
EmbVector obj_top_right(Object *obj);
EmbVector obj_bottom_left(Object *obj);
EmbVector obj_bottom_right(Object *obj);

void obj_update_rubber(Object *obj, QPainter* painter);
void obj_update_rubber_grip(Object *obj, QPainter *painter);
void obj_update_leader(Object *obj);
void obj_update_path(Object *obj);
void obj_update_path_r(Object *obj, QPainterPath p);
void obj_update_arc_rect(Object *obj, double radius);

double obj_length(Object *obj);

void obj_set_line_weight(Object *obj, double lineWeight);

void obj_set_x1(Object *obj, double x);
void obj_set_y1(Object *obj, double y);
void obj_set_x2(Object *obj, double x);
void obj_set_y2(Object *obj, double y);

void obj_real_render(Object *obj, QPainter* painter, QPainterPath renderPath);

QRectF obj_rect(Object *obj);
void obj_set_rect(Object *obj, QRectF r);
void obj_set_rect(Object *obj, double x, double y, double w, double h);
QLineF obj_line(Object *obj);
void obj_set_line(Object *obj, QLineF li);
void obj_set_line(Object *obj, double x1, double y1, double x2, double y2);

void obj_set_path(Object *obj, QPainterPath p);
void obj_calculate_data(Object *obj);

void obj_set_pos(Object *obj, EmbVector point);
void obj_set_x(Object *obj, double x);
void obj_set_y(Object *obj, double y);

int obj_find_index(Object *obj, EmbVector point);

void obj_set_end_point_1(Object *obj, EmbVector endPt1);
void obj_set_end_point_2(Object *obj, EmbVector endPt2);

void obj_set_color(Object *obj, const QColor& color);
void obj_set_color_rgb(Object *obj, QRgb rgb);
void obj_set_line_type(Object *obj, Qt::PenStyle lineType);

void obj_set_center(Object *obj, EmbVector point);
void obj_set_center_x(Object *obj, double centerX);
void obj_set_center_y(Object *obj, double centerY);

void obj_set_text(Object *obj, QString  str);
void obj_set_text_font(Object *obj, QString  font);
void obj_set_text_justify(Object *obj, QString  justify);
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

void repeat_action(void);
void move_action(void);

bool doc_allow_zoom_in(Document* doc);
bool doc_allow_zoom_out(Document* doc);
void doc_zoom_in(Document* doc);
void doc_zoom_out(Document* doc);
void doc_zoom_window(Document* doc);
void doc_zoom_selected(Document* doc);
void doc_zoom_extents(Document* doc);
void doc_pan_real_time(Document* doc);
void doc_pan_point(Document* doc);
void doc_pan_left(Document* doc);
void doc_pan_right(Document* doc);
void doc_pan_up(Document* doc);
void doc_pan_down(Document* doc);
void doc_select_all(Document* doc);
void doc_selection_changed(Document* doc);
void doc_clear_selection(Document* doc);
void doc_delete_selected(Document* doc);
void doc_move_selected(Document* doc, double dx, double dy);
void doc_cut(Document* doc);
void doc_copy(Document* doc);
void doc_paste(Document* doc);
void doc_repeat_action(Document* doc);
void doc_move_action(Document* doc);
void doc_scale_action(Document* doc);
void doc_scale_selected(Document* doc, double x, double y, double factor);
void doc_rotate_action(Document* doc);
void doc_rotate_selected(Document* doc, double x, double y, double rot);
void doc_mirror_selected(Document* doc, double x1, double y1, double x2, double y2);
int doc_num_selected(Document* doc);

void doc_delete_pressed(Document* doc);
void doc_escape_pressed(Document* doc);

void doc_corner_button_clicked(Document* doc);

void doc_show_scroll_bars(Document* doc, bool val);
void doc_set_corner_button(Document* doc);
void doc_set_cross_hair_color(Document* doc, QRgb color);
void doc_set_cross_hair_size(Document* doc, uint8_t percent);
void doc_set_background_color(Document* doc, QRgb color);
void doc_set_select_box_colors(Document* doc, QRgb colorL, QRgb fillL, QRgb colorR, QRgb fillR, int alpha);
void doc_toggle_snap(Document* doc, bool on);
void doc_toggle_grid(Document* doc, bool on);
void doc_toggle_ruler(Document* doc, bool on);
void doc_toggle_ortho(Document* doc, bool on);
void doc_toggle_polar(Document* doc, bool on);
void doc_toggle_qsnap(Document* doc, bool on);
void doc_toggle_qtrack(Document* doc, bool on);
void doc_toggle_lwt(Document* doc, bool on);
void doc_toggle_real(Document* doc, bool on);
bool doc_is_lwt_enabled(Document* doc);
bool doc_is_real_enabled(Document* doc);

void doc_set_grid_color(Document* doc, QRgb color);
void doc_create_grid(Document* doc, QString gridType);
void doc_set_ruler_color(Document* doc, QRgb color);

void doc_preview_on(Document* doc, int clone, int mode, double x, double y, double data);
void doc_preview_off(Document* doc);

void doc_enable_move_rapid_fire(Document* doc);
void doc_disable_move_rapid_fire(Document* doc);

bool doc_allow_rubber(Document* doc);
void doc_add_to_rubber_room(Document* doc, QGraphicsItem* item);
void doc_vulcanize_rubber_room(Document* doc);
void doc_clear_rubber_room(Document* doc);
void doc_spare_rubber(Document* doc, int64_t id);
void doc_set_rubber_mode(Document* doc, int mode);
void doc_set_rubber_point(Document* doc, QString key, EmbVector point);
void doc_set_rubber_text(Document* doc, QString key, QString txt);

void draw_arc(QPainter* painter, EmbArc arc);
void draw_circle(QPainter* painter, EmbCircle circle);
void draw_ellipse(QPainter* painter, EmbEllipse ellipse);
void draw_line(QPainter* painter, EmbLine line);
void draw_polygon(QPainter* painter, EmbPolygon polygon);
void draw_polyline(QPainter* painter, EmbPolyline polyline);
void draw_rect(QPainter* painter, EmbRect rect);
void draw_spline(QPainter* painter, EmbSpline spline);

void doc_create_grid_rect(Document* doc);
void doc_create_grid_polar(Document* doc);
void doc_create_grid_iso(Document* doc);
void doc_create_origin(Document* doc);

void doc_load_ruler_settings(Document* doc);

QPainterPath doc_create_ruler_text_path(Document* doc, float x, float y, QString str, float height);

QList<QGraphicsItem*> doc_create_object_list(Document* doc, QList<QGraphicsItem*> list);

void doc_copy_selected(Document* doc);

void doc_start_gripping(Document* doc, Object* obj);
void doc_stop_gripping(Document* doc, bool accept = false);

void doc_update_mouse_coords(Document* doc, int x, int y);

void doc_pan_start(Document* doc, EmbVector point);

void doc_align_scene_point_with_view_point(Document* doc, EmbVector scenePoint, EmbVector viewPoint);

void doc_recalculate_limits(Document* doc);
void doc_zoom_to_point(Document* doc, EmbVector mousePoint, int zoomDir);
void doc_center_at(Document* doc, EmbVector centerPoint);
EmbVector doc_center(Document* doc);

void doc_add_object(Document* doc, Object* obj);
void doc_delete_object(Document* doc, Object* obj);
void doc_vulcanize_object(Document* doc, Object* obj);

/* ------------------------- Settings Dialog --------------------------- */

/* ------------------------------ Prompt ------------------------------- */

void setPromptTextColor(const QColor&);
void setPromptBackgroundColor(const QColor&);
void setPromptFontFamily(QString );
void setPromptFontStyle(QString );
void setPromptFontSize(int);

/* -------------------------- Main Functions --------------------------- */

void runCommandMain(QString  cmd);
void runCommandClick(QString  cmd, double x, double y);
void runCommandMove(QString  cmd, double x, double y);
void runCommandContext(QString  cmd, QString  str);
void runCommandPrompt(QString  cmd);

void updateAllViewScrollBars(bool val);
void updateAllViewCrossHairColors(QRgb color);
void updateAllViewBackgroundColors(QRgb color);
void updateAllViewSelectBoxColors(QRgb colorL, QRgb fillL, QRgb colorR, QRgb fillR, int alpha);
void updateAllViewGridColors(QRgb color);
void updateAllViewRulerColors(QRgb color);

void updatePickAddMode(bool val);
void pickAddModeToggled(void);

void makeLayerActive(void);
void layerManager(void);
void layerPrevious(void);

void layerSelectorIndexChanged(int index);
void linetypeSelectorIndexChanged(int index);
void lineweightSelectorIndexChanged(int index);
void textFontSelectorCurrentFontChanged(const QFont& font);
void textSizeSelectorIndexChanged(int index);

void setTextFont(QString  str);
void setTextSize(double num);

void deletePressed(void);
void escapePressed(void);

void promptHistoryAppended(QString  txt);
void logPromptInput(QString  txt);
void promptInputPrevious();
void promptInputNext();

void print_command(void);
void undo_command(void);
void redo_command(void);

bool isShiftPressed(void);
void setShiftPressed(void);
void setShiftReleased(void);

void iconResize(int iconSize);

void openFile(bool recent = false, QString  recentFile = "");
void openFilesSelected(const QStringList&);

void onWindowActivated(QMdiSubWindow* w);

QAction* getFileSeparator();
void loadFormats();

void settingsPrompt();

void settingsDialog(QString showTab = "");
void readSettings();
void writeSettings();

void createAllMenus();
void createAllActions();
QAction* createAction(Command command);

bool validFileFormat(QString fileName);
QMdiSubWindow* findMdiWindow(QString fileName);

void onCloseWindow(void);
void onCloseMdiWin(MdiWindow*);

void setUndoCleanIcon(bool opened);

void currentLayerChanged(QString  layer);
void currentColorChanged(const QRgb& color);
void currentLinetypeChanged(QString  type);
void currentLineweightChanged(QString  weight);

/* --------------------------- EmbVector ------------------------------- */

EmbVector operator+(EmbVector a, EmbVector b);
EmbVector operator-(EmbVector a, EmbVector b);

/* ---------------------- Class Declarations --------------------------- */

class LayerManager : public QDialog
{
    Q_OBJECT

public:
    LayerManager(MainWindow* mw, QWidget *parent = 0);
    ~LayerManager();

    void addLayer(QString  name,
        const bool visible,
        const bool frozen,
        const double zValue,
        const QRgb color,
        QString  lineType,
        QString  lineWeight,
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

    void setBackgroundLogo(QString  fileName);
    void setBackgroundTexture(QString  fileName);
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

protected:
    void mouseDoubleClickEvent(QMouseEvent* event);
    void mousePressEvent(QMouseEvent* event);
    void mouseMoveEvent(QMouseEvent* event);
    void mouseReleaseEvent(QMouseEvent* event);
    void wheelEvent(QWheelEvent* event);
    void contextMenuEvent(QContextMenuEvent* event);
    void drawBackground(QPainter* painter, const QRectF& rect);
    void drawForeground(QPainter* painter, const QRectF& rect);
    void enterEvent(QEvent* event);
};

class UndoableCommand : public QUndoCommand
{
public:
    UndoableCommand(int type_, QString text, Object* obj, Document* v, QUndoCommand* parent = 0);
    UndoableCommand(int type_, EmbVector delta, QString text, Object* obj, Document* v, QUndoCommand* parent = 0);
    UndoableCommand(int type_, EmbVector pivot, double rotAngle, QString text, Object* obj, Document* v, QUndoCommand* parent = 0);
    UndoableCommand(int type_, QString type, Document* v, QUndoCommand* parent = 0);
    UndoableCommand(int type_, EmbVector start, EmbVector end, QString text, Object* obj, Document* v, QUndoCommand* parent = 0);

    void undo();
    void redo();
    void rotate(double x, double y, double rot);
    int id() const { return 1234; }
    bool mergeWith(const QUndoCommand* command);
    void mirror();

    UndoData data;
};

class UndoEditor : public QDockWidget
{
    Q_OBJECT

public:
    UndoEditor(QString  iconDirectory = "", QWidget* widgetToFocus = 0, QWidget* parent = 0); /*, Qt::WindowFlags flags = 0); */
    ~UndoEditor();

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
    QWidget*    focusWidget;

    QString iconDir;
    int iconSize;

    QUndoGroup* undoGroup;
    QUndoView*  undoView;
};

class Object: public QGraphicsPathItem
{
public:
    EmbGeometry *geometry;
    ObjectData data;
    Object* obj;

    Object(int type_, QRgb rgb, Qt::PenStyle lineType, QGraphicsItem* item = 0);
    ~Object();

    void vulcanize();
    QList<EmbVector> allGripPoints();
    EmbVector mouseSnapPoint(EmbVector mousePoint);
    void gripEdit(EmbVector before, EmbVector after);
    QPainterPath shape() const { return path(); }

    void setObjectRubberMode(int mode) { data.objRubberMode = mode; }
    void setObjectRubberPoint(QString key, EmbVector point) { data.objRubberPoints.insert(key, point); }
    void setObjectRubberText(QString key, QString txt) { data.objRubberTexts.insert(key, txt); }

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
    PreviewDialog(QWidget* parent = 0,
        QString  caption = "",
        QString  directory = "",
        QString  filter = "");
    ~PreviewDialog();

private:
    ImageWidget* imgWidget;
};

class PropertyEditor : public QDockWidget
{
    Q_OBJECT

public:
    PropertyEditor(QString  iconDirectory = "", bool pickAddMode = true, QWidget* widgetToFocus = 0, QWidget* parent = 0); /*, Qt::WindowFlags flags = 0); */
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
    Settings_Dialog(MainWindow* mw, QString  showTab = "", QWidget *parent = 0);
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
    QGroupBox* create_group_box(QWidget* widget, const char *label, int data[]);
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
    void comboBoxPromptFontFamilyCurrentIndexChanged(QString);
    void comboBoxPromptFontStyleCurrentIndexChanged(QString);
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
    Document* gview;

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

    void promptHistoryAppended(QString  txt);
    void logPromptInput(QString  txt);
    void promptInputPrevious();
    void promptInputNext();

private:
    void setCurrentFile(QString  fileName);
    QString fileExtension(QString  fileName);

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
    void updateCurrentText(QString  txt);
    void checkEditedText(QString  txt);
    void checkChangedText(QString  txt);
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
    void setCurrentText(QString  txt)
    {
        curText = prefix + txt;
        promptInput->setText(curText);
    }
    void setPrefix(QString  txt);
    void enableRapidFire() { rapidFireEnabled = true; }
    void disableRapidFire() { rapidFireEnabled = false; }
    bool isRapidFireEnabled() { return rapidFireEnabled; }

    void alert(QString  txt);

    void blink();

    void saveHistory(QString  fileName, bool html);

signals:
    /* For connecting outside of command prompt. */
    void showSettings();

    void historyAppended(QString  txt);
};

class MainWindow: public QMainWindow
{
    Q_OBJECT

public:
    MainWindow();
    ~MainWindow();

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

