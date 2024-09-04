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
extern std::unordered_map<QString, QToolBar*> toolbarHash;
extern std::unordered_map<QString, QMenu*> menuHash;
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
    QPointF previewPoint;
    double previewData;
    int previewMode;

    QPoint viewMousePoint;
    QPointF sceneMousePoint;
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
    QPointF scenePressPoint;
    QPoint pressPoint;
    QPointF sceneMovePoint;
    QPoint movePoint;
    QPointF sceneReleasePoint;
    QPoint releasePoint;
    QPointF sceneGripPoint;

    QPointF cutCopyMousePoint;
    QGraphicsItemGroup* pasteObjectItemGroup;
    QPointF pasteDelta;

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
    double dx;
    double dy;
    double pivotX;
    double pivotY;
    double angle;
    double factor;
    QString navType;
    QTransform fromTransform;
    QTransform toTransform;
    QPointF fromCenter;
    QPointF toCenter;
    bool done;
    QPointF before;
    QPointF after;
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
    QHash<QString, QPointF> objRubberPoints;
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
QPointF scale_and_rotate(QPointF v, double angle, double scale);
QPointF find_mouse_snap_point(QList<QPointF> snap_points, const QPointF& mouse_point);

void set_visibility(QObject *senderObj, const char *key, bool visibility);
void set_enabled(QObject *senderObj, const char *key, bool visibility);
void set_visibility_group(QObject *senderObj, char *key[], bool visibility);
void set_enabled_group(QObject *senderObj, char *key[], bool visibility);
QIcon create_swatch(int32_t color);
QGroupBox* create_group_box(QWidget* widget, const char *label, WidgetData data[]);
QCheckBox* create_checkbox(QGroupBox* groupbox, QString label, BoolSetting *setting, QString icon);
void preview_update(void);
QDoubleSpinBox* create_spinbox(QGroupBox* groupbox, QString label, RealSetting *setting, double single_step, double lower_bound, double upper_bound);
QSpinBox* create_int_spinbox(QGroupBox* groupbox, QString label, IntSetting *setting, int single_step, int lower_bound, int upper_bound);
QPushButton *choose_color_button(QGroupBox* groupbox, IntSetting* color_setting);

QString getCurrentLayer();
QRgb getCurrentColor();
QString getCurrentLineType();
QString getCurrentLineWeight();

void setHistory(const QString& txt);
void add_command(std::string alias, std::string cmd);

/* ------------------------ Object Functions --------------------------- */

Object *create_arc(EmbArc arc, QRgb rgb, QGraphicsItem *item=0);
Object *create_circle(EmbCircle circle, QRgb rgb, QGraphicsItem *item=0);
Object *create_ellipse(EmbEllipse ellipse, QRgb rgb, QGraphicsItem *item=0);
Object *create_polyline(EmbPath path, const QPainterPath& p, QRgb rgb, QGraphicsItem* parent=0);
Object *create_path(double x, double y, const QPainterPath p, QRgb rgb, QGraphicsItem* parent=0);
Object *create_polygon(double x, double y, const QPainterPath& p, QRgb rgb, QGraphicsItem* parent=0);
Object *create_text_single(const QString& str, double x, double y, QRgb rgb, QGraphicsItem* parent=0);
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
QPointF obj_rubber_point(Object* obj, const QString& key);
QString obj_rubber_text(Object* obj, const QString& key);

QPointF obj_end_point_1(Object *obj);
QPointF obj_end_point_2(Object *obj);
QPointF obj_start_point(Object *obj);
QPointF obj_mid_point(Object *obj);
QPointF obj_end_point(Object *obj);

QPointF obj_pos(Object *obj);
double obj_x(Object *obj);
double obj_y(Object *obj);
QPointF obj_center(Object *obj);
double obj_center_x(Object *obj);
double obj_center_y(Object *obj);
double obj_radius(Object *obj);
double obj_diameter(Object *obj);
double obj_circumference(Object *obj);
QPointF obj_delta(Object *obj);

QPointF obj_top_left(Object *obj);
QPointF obj_top_right(Object *obj);
QPointF obj_bottom_left(Object *obj);
QPointF obj_bottom_right(Object *obj);

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

QRectF obj_rect(Object *obj);
void obj_set_rect(Object *obj, QRectF r);
void obj_set_rect(Object *obj, double x, double y, double w, double h);
QLineF obj_line(Object *obj);
void obj_set_line(Object *obj, QLineF li);
void obj_set_line(Object *obj, double x1, double y1, double x2, double y2);

void obj_set_path(Object *obj, QPainterPath p);
void obj_calculate_data(Object *obj);

void obj_set_pos(Object *obj, QPointF point);
void obj_set_pos(Object *obj, double x, double y);
void obj_set_x(Object *obj, double x);
void obj_set_y(Object *obj, double y);

int obj_find_index(Object *obj, QPointF point);

void obj_set_end_point_1(Object *obj, QPointF endPt1);
void obj_set_end_point_1(Object *obj, double x1, double y1);
void obj_set_end_point_2(Object *obj, QPointF endPt2);
void obj_set_end_point_2(Object *obj, double x2, double y2);

void obj_set_color(Object *obj, const QColor& color);
void obj_set_color_rgb(Object *obj, QRgb rgb);
void obj_set_line_type(Object *obj, Qt::PenStyle lineType);

void obj_set_center(Object *obj, EmbVector point);
void obj_set_center(Object *obj, QPointF center);
void obj_set_center_x(Object *obj, double centerX);
void obj_set_center_y(Object *obj, double centerY);

void obj_set_text(Object *obj, const QString& str);
void obj_set_text_font(Object *obj, const QString& font);
void obj_set_text_justify(Object *obj, const QString& justify);
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
void doc_set_rubber_point(Document* doc, QString key, QPointF point);
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

void doc_pan_start(Document* doc, QPoint& point);

void doc_align_scene_point_with_view_point(Document* doc, QPointF scenePoint, QPoint viewPoint);

void doc_recalculate_limits(Document* doc);
void doc_zoom_to_point(Document* doc, QPoint mousePoint, int zoomDir);
void doc_center_at(Document* doc, QPointF& centerPoint);
QPointF doc_center(Document* doc);

void doc_add_object(Document* doc, Object* obj);
void doc_delete_object(Document* doc, Object* obj);
void doc_vulcanize_object(Document* doc, Object* obj);

/* ------------------------- Settings Dialog --------------------------- */

/* ------------------------------ Prompt ------------------------------- */

void setPromptTextColor(const QColor&);
void setPromptBackgroundColor(const QColor&);
void setPromptFontFamily(const QString&);
void setPromptFontStyle(const QString&);
void setPromptFontSize(int);

/* -------------------------- Main Functions --------------------------- */

void runCommandMain(const QString& cmd);
void runCommandClick(const QString& cmd, double x, double y);
void runCommandMove(const QString& cmd, double x, double y);
void runCommandContext(const QString& cmd, const QString& str);
void runCommandPrompt(const QString& cmd);

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

void setTextFont(const QString& str);
void setTextSize(double num);

void deletePressed(void);
void escapePressed(void);

void promptHistoryAppended(const QString& txt);
void logPromptInput(const QString& txt);
void promptInputPrevious();
void promptInputNext();

void print_command(void);
void undo_command(void);
void redo_command(void);

bool isShiftPressed(void);
void setShiftPressed(void);
void setShiftReleased(void);

void iconResize(int iconSize);

void openFile(bool recent = false, const QString& recentFile = "");
void openFilesSelected(const QStringList&);

void onWindowActivated(QMdiSubWindow* w);

QAction* getFileSeparator();
void loadFormats();

void settingsPrompt();

void settingsDialog(const QString& showTab = QString());
void readSettings();
void writeSettings();

void createAllMenus();
void createAllActions();
QAction* createAction(Command command);

bool validFileFormat(const QString &fileName);
QMdiSubWindow* findMdiWindow(const QString &fileName);

void onCloseWindow(void);
void onCloseMdiWin(MdiWindow*);

void setUndoCleanIcon(bool opened);

void currentLayerChanged(const QString& layer);
void currentColorChanged(const QRgb& color);
void currentLinetypeChanged(const QString& type);
void currentLineweightChanged(const QString& weight);

/* ---------------------- Class Declarations --------------------------- */

class LayerManager : public QDialog
{
    Q_OBJECT

public:
    LayerManager(MainWindow* mw, QWidget *parent = 0);
    ~LayerManager();

    void addLayer(const QString& name,
        const bool visible,
        const bool frozen,
        const double zValue,
        const QRgb color,
        const QString& lineType,
        const QString& lineWeight,
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

    void setBackgroundLogo(const QString& fileName);
    void setBackgroundTexture(const QString& fileName);
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
    UndoableCommand(int type_, const QString& text, Object* obj, Document* v, QUndoCommand* parent = 0);
    UndoableCommand(int type_, double deltaX, double deltaY, const QString& text, Object* obj, Document* v, QUndoCommand* parent = 0);
    UndoableCommand(int type_, double pivotPointX, double pivotPointY, double rotAngle, const QString& text, Object* obj, Document* v, QUndoCommand* parent = 0);
    UndoableCommand(int type_, const QString& type, Document* v, QUndoCommand* parent = 0);
    UndoableCommand(int type_, const QPointF beforePoint, const QPointF afterPoint, const QString& text, Object* obj, Document* v, QUndoCommand* parent = 0);
    UndoableCommand(int type_, double x1, double y1, double x2, double y2, const QString& text, Object* obj, Document* v, QUndoCommand* parent = 0);

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
    UndoEditor(const QString& iconDirectory = QString(), QWidget* widgetToFocus = 0, QWidget* parent = 0); /*, Qt::WindowFlags flags = 0); */
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
    QList<QPointF> allGripPoints();
    QPointF mouseSnapPoint(QPointF mousePoint);
    void gripEdit(const QPointF& before, const QPointF& after);
    QPainterPath shape() const { return path(); }

    void setObjectRubberMode(int mode) { data.objRubberMode = mode; }
    void setObjectRubberPoint(const QString& key, const QPointF& point) { data.objRubberPoints.insert(key, point); }
    void setObjectRubberText(const QString& key, const QString& txt) { data.objRubberTexts.insert(key, txt); }

    void drawRubberLine(const QLineF& rubLine, QPainter* painter = 0, const char* colorFromScene = 0);
    void realRender(QPainter* painter, const QPainterPath& renderPath);

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
        const QString& caption = "",
        const QString& directory = "",
        const QString& filter = "");
    ~PreviewDialog();

private:
    ImageWidget* imgWidget;
};

class PropertyEditor : public QDockWidget
{
    Q_OBJECT

public:
    PropertyEditor(const QString& iconDirectory = "", bool pickAddMode = true, QWidget* widgetToFocus = 0, QWidget* parent = 0); /*, Qt::WindowFlags flags = 0); */
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
    Settings_Dialog(MainWindow* mw, const QString& showTab = "", QWidget *parent = 0);
    ~Settings_Dialog();

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

private slots:
    void checkBoxCustomFilterStateChanged(int);
    void checkBoxGridColorMatchCrossHairStateChanged(int);
    void checkBoxGridLoadFromFileStateChanged(int);
    void checkBoxGridCenterOnOriginStateChanged(int);
    void checkBoxLwtShowLwtStateChanged(int);
    void checkBoxLwtRealRenderStateChanged(int);

    void comboBoxLanguageCurrentIndexChanged(const QString&);
    void comboBoxIconThemeCurrentIndexChanged(const QString&);
    void comboBoxIconSizeCurrentIndexChanged(int);
    void chooseGeneralMdiBackgroundLogo();
    void chooseGeneralMdiBackgroundTexture();
    void chooseGeneralMdiBackgroundColor();
    void currentGeneralMdiBackgroundColorChanged(const QColor&);
    void comboBoxScrollBarWidgetCurrentIndexChanged(int);
    void chooseDisplayCrossHairColor();
    void currentDisplayCrossHairColorChanged(const QColor&);
    void chooseDisplayBackgroundColor();
    void currentDisplayBackgroundColorChanged(const QColor&);
    void chooseDisplaySelectBoxLeftColor();
    void currentDisplaySelectBoxLeftColorChanged(const QColor&);
    void chooseDisplaySelectBoxLeftFill();
    void currentDisplaySelectBoxLeftFillChanged(const QColor&);
    void chooseDisplaySelectBoxRightColor();
    void currentDisplaySelectBoxRightColorChanged(const QColor&);
    void chooseDisplaySelectBoxRightFill();
    void currentDisplaySelectBoxRightFillChanged(const QColor&);
    void spinBoxDisplaySelectBoxAlphaValueChanged(int);
    void choosePromptTextColor();
    void currentPromptTextColorChanged(const QColor&);
    void choosePromptBackgroundColor();
    void currentPromptBackgroundColorChanged(const QColor&);
    void comboBoxPromptFontFamilyCurrentIndexChanged(const QString&);
    void comboBoxPromptFontStyleCurrentIndexChanged(const QString&);
    void spinBoxPromptFontSizeValueChanged(int);
    void buttonCustomFilterSelectAllClicked();
    void buttonCustomFilterClearAllClicked();
    void spinBoxRecentMaxFilesValueChanged(int);
    void spinBoxTrimDstNumJumpsValueChanged(int);
    void chooseGridColor();
    void currentGridColorChanged(const QColor&);
    void comboBoxGridTypeCurrentIndexChanged(const QString&);
    void comboBoxRulerMetricCurrentIndexChanged(int);
    void chooseRulerColor();
    void currentRulerColorChanged(const QColor&);
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

class SelectBox : public QRubberBand
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
    bool loadFile(const QString &fileName);
    bool saveFile(const QString &fileName);
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

    void promptHistoryAppended(const QString& txt);
    void logPromptInput(const QString& txt);
    void promptInputPrevious();
    void promptInputNext();

private:
    void setCurrentFile(const QString& fileName);
    QString fileExtension(const QString& fileName);

    void promptInputPrevNext(bool prev);
};

class ImageWidget : public QWidget
{
    Q_OBJECT

public:
    ImageWidget(const QString &filename, QWidget* parent = 0);
    ~ImageWidget();

    bool load(const QString &fileName);
    bool save(const QString &fileName);

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
    void updateCurrentText(const QString& txt);
    void checkEditedText(const QString& txt);
    void checkChangedText(const QString& txt);
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
    void setCurrentText(const QString& txt)
    {
        curText = prefix + txt;
        promptInput->setText(curText);
    }
    void setPrefix(const QString& txt);
    void enableRapidFire() { rapidFireEnabled = true; }
    void disableRapidFire() { rapidFireEnabled = false; }
    bool isRapidFireEnabled() { return rapidFireEnabled; }

    void alert(const QString& txt);

    void blink();

    void saveHistory(const QString& fileName, bool html);

signals:
    /* For connecting outside of command prompt. */
    void showSettings();

    void historyAppended(const QString& txt);
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

