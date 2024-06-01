/*
 * Embroidermodder 2.
 *
 * Copyright 2011-2024 The Embroidermodder Team
 * Embroidermodder 2 is Open Source Software, see LICENSE.md for licensing terms.
 * Visit https://www.libembroidery.org/refman for advice on altering this file,
 * or read the markdown version in embroidermodder2/docs/refman.
 *
 * Main Embroidermodder Header
 */

#ifndef EMBROIDERMODDER_H
#define EMBROIDERMODDER_H

#include <QApplication>
#include <QBrush>
#include <QDialog>
#include <QDir>
#include <QDockWidget>
#include <QFileDialog>
#include <QGraphicsPathItem>
#include <QGraphicsScene>
#include <QGraphicsView>
#include <QGridLayout>
#include <QHash>
#include <QImage>
#include <QLineEdit>
#include <QList>
#include <QMainWindow>
#include <QMdiArea>
#include <QMdiSubWindow>
#include <QObject>
#include <QPainter>
#include <QPainterPath>
#include <QPen>
#include <QPixmap>
#include <QPointF>
#include <QRubberBand>
#include <QScrollBar>
#include <QSplitter>
#include <QStatusBar>
#include <QTextBrowser>
#include <QTextLayout>
#include <QToolButton>
#include <QTransform>
#include <QUndoCommand>
#include <QWidget>

#include <QtPrintSupport>

#include <math.h>

#include "../extern/libembroidery/embroidery.h"

#define REAL(i)             context->argument[i].r
#define INT(i)              context->argument[i].i
#define QSTR(i)    QString(context->argument[i].s)
#define BOOL(i)             context->argument[i].b

#define MAX_ARGS                      10
#define MAX_STRING_LENGTH            200

#define SCRIPT_NULL                    0
#define SCRIPT_INT                     1
#define SCRIPT_REAL                    2
#define SCRIPT_COMMAND                 3
#define SCRIPT_STRING                  4
#define SCRIPT_BOOL                    5

#define CONTEXT_MAIN                   0
#define CONTEXT_CLICK                  1
#define CONTEXT_MOVE                   2
#define CONTEXT_CONTEXT                3
#define CONTEXT_PROMPT                 4

typedef struct ScriptValue_ {
    double r;
    int i;
    bool b;
    char s[100];
    char label[100];
    struct ScriptValue_ *leaf;
    int n_leaves;
    char type;
} ScriptValue;

typedef struct ScriptEnv_ {
    ScriptValue *variable;
    int n_variables;
    int context;
    ScriptValue argument[MAX_ARGS];
    int argumentCount;
} ScriptEnv;

typedef struct Command_ {
    int id;
    ScriptValue (*main)(ScriptEnv *context);
    const char *icon;
    const char *menu_name;
    int menu_position;
    const char *toolbar_name;
    int toolbar_position;
    const char *tooltip;
    const char *statustip;
    const char *alias;
    const char *shortcut;
} Command;


enum COMMAND_ACTIONS
{
    ACTION_DO_NOTHING,

    ACTION_NEW,
    ACTION_OPEN,
    ACTION_SAVE,
    ACTION_SAVE_AS,
    ACTION_PRINT,
    ACTION_DESIGN_DETAILS,
    ACTION_EXIT,
    ACTION_CUT,
    ACTION_COPY,
    ACTION_PASTE,

    ACTION_UNDO,
    ACTION_REDO,

    /* Window Menu */
    ACTION_WINDOW_CLOSE,
    ACTION_WINDOW_CLOSE_ALL,
    ACTION_WINDOW_CASCADE,
    ACTION_WINDOW_TILE,
    ACTION_WINDOW_NEXT,
    ACTION_WINDOW_PREVIOUS,

    /* Help Menu */
    ACTION_HELP,
    ACTION_CHANGELOG,
    ACTION_TIP_OF_THE_DAY,
    ACTION_ABOUT,
    ACTION_WHATS_THIS,

    /* Icons */
    ACTION_ICON_16,
    ACTION_ICON_24,
    ACTION_ICON_32,
    ACTION_ICON_48,
    ACTION_ICON_64,
    ACTION_ICON_128,

    ACTION_SETTINGS_DIALOG,

    /* Layer ToolBar */
    ACTION_MAKE_LAYER_CURRENT,
    ACTION_LAYERS,
    ACTION_LAYER_SELECTOR,
    ACTION_LAYER_PREVIOUS,
    ACTION_COLOR_SELECTOR,
    ACTION_LINE_TYPE_SELECTOR,
    ACTION_LINE_WEIGHT_SELECTOR,
    ACTION_HIDE_ALL_LAYERS,
    ACTION_SHOW_ALL_LAYERS,
    ACTION_FREEZE_ALL_LAYERS,
    ACTION_THAW_ALL_LAYERS,
    ACTION_LOCK_ALL_LAYERS,
    ACTION_UNLOCK_ALL_LAYERS,

    /* Text ToolBar */
    ACTION_TEXT_BOLD,
    ACTION_TEXT_ITALIC,
    ACTION_TEXT_UNDERLINE,
    ACTION_TEXT_STRIKEOUT,
    ACTION_TEXT_OVERLINE,

    /* Zoom ToolBar */
    ACTION_ZOOM_REAL_TIME,
    ACTION_ZOOM_PREVIOUS,
    ACTION_ZOOM_WINDOW,
    ACTION_ZOOM_DYNAMIC,
    ACTION_ZOOM_SCALE,
    ACTION_ZOOM_CENTER,
    ACTION_ZOOM_IN,
    ACTION_ZOOM_OUT,
    ACTION_ZOOM_SELECTED,
    ACTION_ZOOM_ALL,
    ACTION_ZOOM_EXTENTS,

    /* Pan SubMenu */
    ACTION_PAN_REAL_TIME,
    ACTION_PAN_POINT,
    ACTION_PAN_LEFT,
    ACTION_PAN_RIGHT,
    ACTION_PAN_UP,
    ACTION_PAN_DOWN,

    ACTION_DAY,
    ACTION_NIGHT,

    /* Prompt. */
    ACTION_ALERT,
    ACTION_GET,
    ACTION_SET,

    /* Selection management. */
    ACTION_CLEAR,

    /* TODO: ACTION_spellcheck, */
    /* TODO: ACTION_quickselect, */

    ACTION_ANGLE,
    ACTION_CIRCLE,
    ACTION_DEBUG,
    ACTION_DISABLE,
    ACTION_DISTANCE,
    ACTION_DOLPHIN,
    ACTION_ELLIPSE,
    ACTION_ENABLE,
    ACTION_ERASE,
    ACTION_ERROR,
    ACTION_HEART,
    ACTION_LINE,
    ACTION_LOCATE_POINT,
    ACTION_MIRROR_SELECTED,
    ACTION_MOVE,
    ACTION_MOVE_SELECTED,
    ACTION_PATH,
    ACTION_PLATFORM,
    ACTION_POINT,
    ACTION_POLYGON,
    ACTION_POLYLINE,
    ACTION_PREVIEW_OFF,
    ACTION_PREVIEW_ON,
    ACTION_QUICKLEADER,
    ACTION_RECTANGLE,
    ACTION_RGB,
    ACTION_ROTATE,
    ACTION_SANDBOX,
    ACTION_SCALE,
    ACTION_SCALE_SELECTED,
    ACTION_SELECT_ALL,
    ACTION_SINGLE_LINE_TEXT,
    ACTION_SNOWFLAKE,
    ACTION_STAR,
    ACTION_SYSWINDOWS,
    ACTION_TODO,
    ACTION_VULCANIZE,

    ACTION_NULL
};

class ArcObject;
class BaseObject;
class BlockObject;
class CircleObject;
class CmdPrompt;
class DimAlignedObject;
class DimAngularObject;
class DimArcLengthObject;
class DimDiameterObject;
class DimLeaderObject;
class DimLinearObject;
class DimOrdinateObject;
class DimRadiusObject;
class EllipseArcObject;
class EllipseObject;
class HatchObject;
class ImageObject;
class ImageWidget;
class InfiniteLineObject;
class LineObject;
class MainWindow;
class MdiArea;
class MdiWindow;
class PathObject;
class PointObject;
class PolygonObject;
class PolylineObject;
class PropertyEditor;
class RayObject;
class RectObject;
class SelectBox;
class SplineObject;
class StatusBar;
class StatusBarButton;
class TextMultiObject;
class TextSingleObject;
class UndoEditor;
class View;

QT_BEGIN_NAMESPACE
class QAbstractItemModel;
class QAction;
class QCheckBox;
class QCloseEvent;
class QComboBox;
class QContextMenuEvent;
class QDialogButtonBox;
class QFileInfo;
class QFontComboBox;
class QGraphicsItem;
class QGraphicsScene;
class QGraphicsView;
class QGroupBox;
class QImage;
class QLabel;
class QLineEdit;
class QMdiArea;
class QMenu;
class QPainter;
class QScriptEngine;
class QScriptEngineDebugger;
class QScriptProgram;
class QSignalMapper;
class QSortFilterProxyModel;
class QSplitter;
class QStandardItemModel;
class QString;
class QTabWidget;
class QTextBrowser;
class QTimer;
class QToolBar;
class QToolButton;
class QTreeView;
class QUndoGroup;
class QUndoStack;
class QUndoView;
class QVBoxLayout;
QT_END_NAMESPACE

extern MainWindow *_main;

class LayerManager : public QDialog
{
    Q_OBJECT

public:
    LayerManager(MainWindow* mw, QWidget *parent = 0);
    ~LayerManager();

void addLayer(const QString& name,
              const bool visible,
              const bool frozen,
              const qreal zValue,
              const QRgb color,
              const QString& lineType,
              const QString& lineWeight,
              const bool print);

private slots:

private:
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

public slots:
    void cascade();
    void tile();
protected:
    virtual void mouseDoubleClickEvent(QMouseEvent* e);
    virtual void paintEvent(QPaintEvent* e);

private:
    bool useLogo;
    bool useTexture;
    bool useColor;

    QPixmap bgLogo;
    QPixmap bgTexture;
    QColor  bgColor;

    void zoomExtentsAllSubWindows();
    void forceRepaint();
};

class StatusBarButton : public QToolButton
{
    Q_OBJECT

public:
    StatusBarButton(QString buttonText, MainWindow* mw, StatusBar* statbar, QWidget *parent = 0);

protected:
    void contextMenuEvent(QContextMenuEvent *event = 0);

private slots:
    void settingsSnap();
    void settingsGrid();
    void settingsRuler();
    void settingsOrtho();
    void settingsPolar();
    void settingsQSnap();
    void settingsQTrack();
    void settingsLwt();
    void toggleSnap(bool on);
    void toggleGrid(bool on);
    void toggleRuler(bool on);
    void toggleOrtho(bool on);
    void togglePolar(bool on);
    void toggleQSnap(bool on);
    void toggleQTrack(bool on);
    void toggleLwt(bool on);
public slots:
    void enableLwt();
    void disableLwt();
    void enableReal();
    void disableReal();

private:
    StatusBar*  statusbar;
};

inline qreal pi() { return (atan(1.0)*4.0); };
inline qreal radians(qreal degrees) { return (degrees*pi()/180.0); };

class View : public QGraphicsView
{
    Q_OBJECT

public:
    View(MainWindow* mw, QGraphicsScene* theScene, QWidget* parent);
    ~View();
    
    EmbArray *geometry;

    bool allowZoomIn();
    bool allowZoomOut();

    void recalculateLimits();
    void zoomToPoint(const QPoint& mousePoint, int zoomDir);
    void centerAt(const QPointF& centerPoint);
    QPointF center() { return mapToScene(rect().center()); }

    QUndoStack* getUndoStack() { return undoStack; }
    void addObject(BaseObject* obj);
    void deleteObject(BaseObject* obj);
    void vulcanizeObject(BaseObject* obj);

public slots:
    void zoomIn();
    void zoomOut();
    void zoomWindow();
    void zoomSelected();
    void zoomExtents();
    void panRealTime();
    void panPoint();
    void panLeft();
    void panRight();
    void panUp();
    void panDown();
    void selectAll();
    void selectionChanged();
    void clearSelection();
    void deleteSelected();
    void moveSelected(qreal dx, qreal dy);
    void cut();
    void copy();
    void paste();
    void repeatAction();
    void moveAction();
    void scaleAction();
    void scaleSelected(qreal x, qreal y, qreal factor);
    void rotateAction();
    void rotateSelected(qreal x, qreal y, qreal rot);
    void mirrorSelected(qreal x1, qreal y1, qreal x2, qreal y2);
    int numSelected();

    void deletePressed();
    void escapePressed();

    void cornerButtonClicked();

    void showScrollBars(bool val);
    void setCornerButton();
    void setCrossHairColor(QRgb color);
    void setCrossHairSize(quint8 percent);
    void setBackgroundColor(QRgb color);
    void setSelectBoxColors(QRgb colorL, QRgb fillL, QRgb colorR, QRgb fillR, int alpha);
    void toggleSnap(bool on);
    void toggleGrid(bool on);
    void toggleRuler(bool on);
    void toggleOrtho(bool on);
    void togglePolar(bool on);
    void toggleQSnap(bool on);
    void toggleQTrack(bool on);
    void toggleLwt(bool on);
    void toggleReal(bool on);
    bool isLwtEnabled();
    bool isRealEnabled();

    void setGridColor(QRgb color);
    void createGrid(const QString& gridType);
    void setRulerColor(QRgb color);

    void previewOn(int clone, int mode, qreal x, qreal y, qreal data);
    void previewOff();

    void enableMoveRapidFire();
    void disableMoveRapidFire();

    bool allowRubber();
    void addToRubberRoom(QGraphicsItem* item);
    void vulcanizeRubberRoom();
    void clearRubberRoom();
    void spareRubber(qint64 id);
    void setRubberMode(int mode);
    void setRubberPoint(const QString& key, const QPointF& point);
    void setRubberText(const QString& key, const QString& txt);

    void drawArc(QPainter* painter, EmbArc arc);
    void drawCircle(QPainter* painter, EmbCircle circle);
    void drawEllipse(QPainter* painter, EmbEllipse ellipse);
    void drawLine(QPainter* painter, EmbLine line);
    void drawPolygon(QPainter* painter, EmbPolygon polygon);
    void drawPolyline(QPainter* painter, EmbPolyline polyline);
    void drawRect(QPainter* painter, EmbRect rect);
    void drawSpline(QPainter* painter, EmbSpline spline);

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

private:
    QHash<qint64, QGraphicsItem*> hashDeletedObjects;

    QList<qint64> spareRubberList;

    QColor gridColor;
    QPainterPath gridPath;
    void createGridRect();
    void createGridPolar();
    void createGridIso();
    QPainterPath originPath;
    void createOrigin();

    bool rulerMetric;
    QColor rulerColor;
    quint8 rulerPixelSize;
    void loadRulerSettings();

    bool willUnderflowInt32(qint64 a, qint64 b);
    bool willOverflowInt32(qint64 a, qint64 b);
    int roundToMultiple(bool roundUp, int numToRound, int multiple);
    QPainterPath createRulerTextPath(float x, float y, QString str, float height);

    QList<QGraphicsItem*> previewObjectList;
    QGraphicsItemGroup* previewObjectItemGroup;
    QPointF previewPoint;
    qreal previewData;
    int previewMode;

    QList<QGraphicsItem*> createObjectList(QList<QGraphicsItem*> list);
    QPointF cutCopyMousePoint;
    QGraphicsItemGroup* pasteObjectItemGroup;
    QPointF pasteDelta;

    QList<QGraphicsItem*> rubberRoomList;

    void copySelected();

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

    void startGripping(BaseObject* obj);
    void stopGripping(bool accept = false);

    BaseObject* gripBaseObj;
    BaseObject* tempBaseObj;

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

    void updateMouseCoords(int x, int y);
    QPoint viewMousePoint;
    QPointF sceneMousePoint;
    QRgb qsnapLocatorColor;
    quint8 qsnapLocatorSize;
    quint8 qsnapApertureSize;
    QRgb gripColorCool;
    QRgb gripColorHot;
    quint8 gripSize;
    quint8 pickBoxSize;
    QRgb crosshairColor;
    quint32 crosshairSize;

    void panStart(const QPoint& point);
    int panDistance;
    int panStartX;
    int panStartY;

    void alignScenePointWithViewPoint(const QPointF& scenePoint, const QPoint& viewPoint);
};

class UndoableAddCommand : public QUndoCommand
{
public:
    UndoableAddCommand(const QString& text, BaseObject* obj, View* v, QUndoCommand* parent = 0);

    void undo();
    void redo();

private:
    BaseObject* object;
    View*       gview;
};

class UndoableDeleteCommand : public QUndoCommand
{
public:
    UndoableDeleteCommand(const QString& text, BaseObject* obj, View* v, QUndoCommand* parent = 0);

    void undo();
    void redo();

private:
    BaseObject* object;
    View*       gview;
};

class UndoableMoveCommand : public QUndoCommand
{
public:
    UndoableMoveCommand(qreal deltaX, qreal deltaY, const QString& text, BaseObject* obj, View* v, QUndoCommand* parent = 0);

    void undo();
    void redo();

private:
    BaseObject* object;
    View*       gview;
    qreal dx;
    qreal dy;
};

class UndoableRotateCommand : public QUndoCommand
{
public:
    UndoableRotateCommand(qreal pivotPointX, qreal pivotPointY, qreal rotAngle, const QString& text, BaseObject* obj, View* v, QUndoCommand* parent = 0);

    void undo();
    void redo();

private:
    void rotate(qreal x, qreal y, qreal rot);

    inline qreal pi() { return (qAtan(1.0)*4.0); };
    inline qreal radians(qreal degrees) { return (degrees*pi()/180.0); };

    BaseObject* object;
    View*       gview;
    qreal pivotX;
    qreal pivotY;
    qreal angle;
};

class UndoableScaleCommand : public QUndoCommand
{
public:
    UndoableScaleCommand(qreal x, qreal y, qreal scaleFactor, const QString& text, BaseObject* obj, View* v, QUndoCommand* parent = 0);

    void undo();
    void redo();

private:
    BaseObject* object;
    View*       gview;
    qreal dx;
    qreal dy;
    qreal factor;
};

class UndoableNavCommand : public QUndoCommand
{
public:
    UndoableNavCommand(const QString& type, View* v, QUndoCommand* parent = 0);

    int id() const { return 1234; }
    bool mergeWith(const QUndoCommand* command);
    void undo();
    void redo();

private:
    QString navType;
    QTransform fromTransform;
    QTransform toTransform;
    QPointF fromCenter;
    QPointF toCenter;
    bool done;
    View*   gview;
};

class UndoableGripEditCommand : public QUndoCommand
{
public:
    UndoableGripEditCommand(const QPointF beforePoint, const QPointF afterPoint, const QString& text, BaseObject* obj, View* v, QUndoCommand* parent = 0);

    void undo();
    void redo();

private:
    BaseObject* object;
    View*       gview;
    QPointF     before;
    QPointF     after;
};


class UndoableMirrorCommand : public QUndoCommand
{
public:
    UndoableMirrorCommand(qreal x1, qreal y1, qreal x2, qreal y2, const QString& text, BaseObject* obj, View* v, QUndoCommand* parent = 0);

    void undo();
    void redo();

private:
    void mirror();

    BaseObject* object;
    View*       gview;
    QLineF      mirrorLine;

};

class UndoEditor : public QDockWidget
{
    Q_OBJECT

public:
    UndoEditor(const QString& iconDirectory = QString(), QWidget* widgetToFocus = 0, QWidget* parent = 0); //, Qt::WindowFlags flags = 0);
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

//Custom Data used in QGraphicsItems

//                   (     int, const QVariant)
//I.E. object.setData(OBJ_TYPE, OBJ_TYPE_LINE);
//I.E. object.setData(OBJ_LAYER, "OUTLINE");
//I.E. object.setData(OBJ_COLOR, 123);
//I.E. object.setData(OBJ_LTYPE, OBJ_LTYPE_CONT);

//Keys
enum OBJ_KEYS {
OBJ_TYPE   = 0, //value type - int: See OBJ_TYPE_VALUES
OBJ_NAME   = 1, //value type - str: See OBJ_NAME_VALUES
OBJ_LAYER  = 2, //value type - str: "USER", "DEFINED", "STRINGS", etc...
OBJ_COLOR  = 3, //value type - int: 0-255 //TODO: Use color chart in formats/format-dxf.h for this
OBJ_LTYPE  = 4, //value type - int: See OBJ_LTYPE_VALUES
OBJ_LWT    = 5, //value type - int: 0-27
OBJ_RUBBER = 6  //value type - int: See OBJ_RUBBER_VALUES
};

//Values
enum OBJ_TYPE_VALUES {
OBJ_TYPE_NULL         =      0, //NOTE: Allow this enum to evaluate false
OBJ_TYPE_BASE         = 100000, //NOTE: Values >= 65536 ensure compatibility with qgraphicsitem_cast()
OBJ_TYPE_ARC          = 100001,
OBJ_TYPE_BLOCK        = 100002,
OBJ_TYPE_CIRCLE       = 100003,
OBJ_TYPE_DIMALIGNED   = 100004,
OBJ_TYPE_DIMANGULAR   = 100005,
OBJ_TYPE_DIMARCLENGTH = 100006,
OBJ_TYPE_DIMDIAMETER  = 100007,
OBJ_TYPE_DIMLEADER    = 100008,
OBJ_TYPE_DIMLINEAR    = 100009,
OBJ_TYPE_DIMORDINATE  = 100010,
OBJ_TYPE_DIMRADIUS    = 100011,
OBJ_TYPE_ELLIPSE      = 100012,
OBJ_TYPE_ELLIPSEARC   = 100013,
OBJ_TYPE_RUBBER       = 100014,
OBJ_TYPE_GRID         = 100015,
OBJ_TYPE_HATCH        = 100016,
OBJ_TYPE_IMAGE        = 100017,
OBJ_TYPE_INFINITELINE = 100018,
OBJ_TYPE_LINE         = 100019,
OBJ_TYPE_PATH         = 100020,
OBJ_TYPE_POINT        = 100021,
OBJ_TYPE_POLYGON      = 100022,
OBJ_TYPE_POLYLINE     = 100023,
OBJ_TYPE_RAY          = 100024,
OBJ_TYPE_RECTANGLE    = 100025,
OBJ_TYPE_SLOT         = 100026,
OBJ_TYPE_SPLINE       = 100027,
OBJ_TYPE_TEXTMULTI    = 100028,
OBJ_TYPE_TEXTSINGLE   = 100029
};

//OBJ_NAME_VALUES
const char* const OBJ_NAME_NULL         = "Unknown";
const char* const OBJ_NAME_BASE         = "Base";
const char* const OBJ_NAME_ARC          = "Arc";
const char* const OBJ_NAME_BLOCK        = "Block";
const char* const OBJ_NAME_CIRCLE       = "Circle";
const char* const OBJ_NAME_DIMALIGNED   = "Aligned Dimension";
const char* const OBJ_NAME_DIMANGULAR   = "Angular Dimension";
const char* const OBJ_NAME_DIMARCLENGTH = "Arc Length Dimension";
const char* const OBJ_NAME_DIMDIAMETER  = "Diameter Dimension";
const char* const OBJ_NAME_DIMLEADER    = "Leader Dimension";
const char* const OBJ_NAME_DIMLINEAR    = "Linear Dimension";
const char* const OBJ_NAME_DIMORDINATE  = "Ordinate Dimension";
const char* const OBJ_NAME_DIMRADIUS    = "Radius Dimension";
const char* const OBJ_NAME_ELLIPSE      = "Ellipse";
const char* const OBJ_NAME_ELLIPSEARC   = "Elliptical Arc";
const char* const OBJ_NAME_RUBBER       = "Rubber";
const char* const OBJ_NAME_GRID         = "Grid";
const char* const OBJ_NAME_HATCH        = "Hatch";
const char* const OBJ_NAME_IMAGE        = "Image";
const char* const OBJ_NAME_INFINITELINE = "Infinite Line";
const char* const OBJ_NAME_LINE         = "Line";
const char* const OBJ_NAME_PATH         = "Path";
const char* const OBJ_NAME_POINT        = "Point";
const char* const OBJ_NAME_POLYGON      = "Polygon";
const char* const OBJ_NAME_POLYLINE     = "Polyline";
const char* const OBJ_NAME_RAY          = "Ray";
const char* const OBJ_NAME_RECTANGLE    = "Rectangle";
const char* const OBJ_NAME_SLOT         = "Slot";
const char* const OBJ_NAME_SPLINE       = "Spline";
const char* const OBJ_NAME_TEXTMULTI    = "Multi Line Text";
const char* const OBJ_NAME_TEXTSINGLE   = "Single Line Text";

enum OBJ_LTYPE_VALUES {
//CAD Linetypes
OBJ_LTYPE_CONT     = 0,
OBJ_LTYPE_CENTER   = 1,
OBJ_LTYPE_DOT      = 2,
OBJ_LTYPE_HIDDEN   = 3,
OBJ_LTYPE_PHANTOM  = 4,
OBJ_LTYPE_ZIGZAG   = 5,
//Embroidery Stitchtypes
OBJ_LTYPE_RUNNING  = 6, // __________
OBJ_LTYPE_SATIN    = 7, // vvvvvvvvvv
OBJ_LTYPE_FISHBONE = 8, // >>>>>>>>>>
};

enum OBJ_LWT_VALUES {
OBJ_LWT_BYLAYER    = -2,
OBJ_LWT_BYBLOCK    = -1,
OBJ_LWT_DEFAULT    =  0,
OBJ_LWT_01         =  1,
OBJ_LWT_02         =  2,
OBJ_LWT_03         =  3,
OBJ_LWT_04         =  4,
OBJ_LWT_05         =  5,
OBJ_LWT_06         =  6,
OBJ_LWT_07         =  7,
OBJ_LWT_08         =  8,
OBJ_LWT_09         =  9,
OBJ_LWT_10         = 10,
OBJ_LWT_11         = 11,
OBJ_LWT_12         = 12,
OBJ_LWT_13         = 13,
OBJ_LWT_14         = 14,
OBJ_LWT_15         = 15,
OBJ_LWT_16         = 16,
OBJ_LWT_17         = 17,
OBJ_LWT_18         = 18,
OBJ_LWT_19         = 19,
OBJ_LWT_20         = 20,
OBJ_LWT_21         = 21,
OBJ_LWT_22         = 22,
OBJ_LWT_23         = 23,
OBJ_LWT_24         = 24
};

enum OBJ_SNAP_VALUES {
OBJ_SNAP_NULL            =  0, //NOTE: Allow this enum to evaluate false
OBJ_SNAP_ENDPOINT        =  1,
OBJ_SNAP_MIDPOINT        =  2,
OBJ_SNAP_CENTER          =  3,
OBJ_SNAP_NODE            =  4,
OBJ_SNAP_QUADRANT        =  5,
OBJ_SNAP_INTERSECTION    =  6,
OBJ_SNAP_EXTENSION       =  7,
OBJ_SNAP_INSERTION       =  8,
OBJ_SNAP_PERPENDICULAR   =  9,
OBJ_SNAP_TANGENT         = 10,
OBJ_SNAP_NEAREST         = 11,
OBJ_SNAP_APPINTERSECTION = 12,
OBJ_SNAP_PARALLEL        = 13
};

enum OBJ_RUBBER_VALUES {
OBJ_RUBBER_OFF = 0,  //NOTE: Allow this enum to evaluate false
OBJ_RUBBER_ON  = 1,  //NOTE: Allow this enum to evaluate true

OBJ_RUBBER_CIRCLE_1P_RAD,
OBJ_RUBBER_CIRCLE_1P_DIA,
OBJ_RUBBER_CIRCLE_2P,
OBJ_RUBBER_CIRCLE_3P,
OBJ_RUBBER_CIRCLE_TTR,
OBJ_RUBBER_CIRCLE_TTT,

OBJ_RUBBER_DIMLEADER_LINE,

OBJ_RUBBER_ELLIPSE_LINE,
OBJ_RUBBER_ELLIPSE_MAJORDIAMETER_MINORRADIUS,
OBJ_RUBBER_ELLIPSE_MAJORRADIUS_MINORRADIUS,
OBJ_RUBBER_ELLIPSE_ROTATION,

OBJ_RUBBER_GRIP,

OBJ_RUBBER_LINE,

OBJ_RUBBER_POLYGON,
OBJ_RUBBER_POLYGON_INSCRIBE,
OBJ_RUBBER_POLYGON_CIRCUMSCRIBE,

OBJ_RUBBER_POLYLINE,

OBJ_RUBBER_IMAGE,

OBJ_RUBBER_RECTANGLE,

OBJ_RUBBER_TEXTSINGLE
};

enum SPARE_RUBBER_VALUES {
SPARE_RUBBER_OFF = 0,  //NOTE: Allow this enum to evaluate false
SPARE_RUBBER_PATH,
SPARE_RUBBER_POLYGON,
SPARE_RUBBER_POLYLINE
};

enum PREVIEW_CLONE_VALUES {
PREVIEW_CLONE_NULL = 0, //NOTE: Allow this enum to evaluate false
PREVIEW_CLONE_SELECTED,
PREVIEW_CLONE_RUBBER
};

enum PREVIEW_MODE_VALUES {
PREVIEW_MODE_NULL = 0, //NOTE: Allow this enum to evaluate false
PREVIEW_MODE_MOVE,
PREVIEW_MODE_ROTATE,
PREVIEW_MODE_SCALE
};

const char* const SCENE_QSNAP_POINT = "SCENE_QSNAP_POINT";
const char* const SCENE_MOUSE_POINT = "SCENE_MOUSE_POINT";
const char* const VIEW_MOUSE_POINT  = "VIEW_MOUSE_POINT";
const char* const RUBBER_ROOM = "RUBBER_ROOM";

const char* const VIEW_COLOR_BACKGROUND = "VIEW_COLOR_BACKGROUND";
const char* const VIEW_COLOR_CROSSHAIR  = "VIEW_COLOR_CROSSHAIR";
const char* const VIEW_COLOR_GRID       = "VIEW_COLOR_GRID";


class BaseObject : public QGraphicsPathItem
{
public:
    BaseObject(QGraphicsItem* parent = 0);
    virtual ~BaseObject();

    enum { Type = OBJ_TYPE_BASE };
    virtual int type() const { return Type; }

    qint64       objectID()                            const { return objID; }
    QPen         objectPen()                           const { return objPen; }
    QColor       objectColor()                         const { return objPen.color(); }
    QRgb objectColorRGB()                      const { return objPen.color().rgb(); }
    Qt::PenStyle objectLineType()                      const { return objPen.style(); }
    qreal  objectLineWeight()                    const { return lwtPen.widthF(); }
    QPainterPath objectPath()                          const { return path(); }
    int objectRubberMode()                    const { return objRubberMode; }
    QPointF      objectRubberPoint(const QString& key) const;
    QString objectRubberText(const QString& key) const;

    QRectF rect() const { return path().boundingRect(); }
    void setRect(const QRectF& r) { QPainterPath p; p.addRect(r); setPath(p); }
    void setRect(qreal x, qreal y, qreal w, qreal h) { QPainterPath p; p.addRect(x,y,w,h); setPath(p); }
    QLineF line() const { return objLine; }
    void setLine(const QLineF& li) { QPainterPath p; p.moveTo(li.p1()); p.lineTo(li.p2()); setPath(p); objLine = li; }
    void setLine(qreal x1, qreal y1, qreal x2, qreal y2) { QPainterPath p; p.moveTo(x1,y1); p.lineTo(x2,y2); setPath(p); objLine.setLine(x1,y1,x2,y2); }

    void setObjectColor(const QColor& color);
    void setObjectColorRGB(QRgb rgb);
    void setObjectLineType(Qt::PenStyle lineType);
    void setObjectLineWeight(qreal lineWeight);
    void setObjectPath(const QPainterPath& p) { setPath(p); }
    void setObjectRubberMode(int mode) { objRubberMode = mode; }
    void setObjectRubberPoint(const QString& key, const QPointF& point) { objRubberPoints.insert(key, point); }
    void setObjectRubberText(const QString& key, const QString& txt) { objRubberTexts.insert(key, txt); }

    virtual QRectF boundingRect() const;
    virtual QPainterPath shape() const { return path(); }

    void drawRubberLine(const QLineF& rubLine, QPainter* painter = 0, const char* colorFromScene = 0);

    virtual void vulcanize() = 0;
    virtual QPointF mouseSnapPoint(const QPointF& mousePoint) = 0;
    virtual QList<QPointF> allGripPoints() = 0;
    virtual void gripEdit(const QPointF& before, const QPointF& after) = 0;
protected:
    QPen lineWeightPen() const { return lwtPen; }
    inline qreal pi() const { return (atan(1.0)*4.0); }
    inline qreal radians(qreal degree) const { return (degree*pi()/180.0); }
    inline qreal degrees(qreal radian) const { return (radian*180.0/pi()); }
    void realRender(QPainter* painter, const QPainterPath& renderPath);
private:
    QPen objPen;
    QPen lwtPen;
    QLineF objLine;
    int objRubberMode;
    QHash<QString, QPointF> objRubberPoints;
    QHash<QString, QString> objRubberTexts;
    qint64 objID;
};

class ArcObject : public BaseObject
{
public:
    ArcObject(qreal startX, qreal startY, qreal midX, qreal midY, qreal endX, qreal endY, QRgb rgb, QGraphicsItem* parent = 0);
    ArcObject(ArcObject* obj, QGraphicsItem* parent = 0);
    ~ArcObject();

    enum { Type = OBJ_TYPE_ARC };
    virtual int type() const { return Type; }

    QPointF objectCenter()        const { return scenePos(); }
    qreal objectCenterX()       const { return scenePos().x(); }
    qreal objectCenterY()       const { return scenePos().y(); }
    qreal objectRadius()        const { return rect().width()/2.0*scale(); }
    qreal objectStartAngle()    const;
    qreal objectEndAngle()      const;
    QPointF objectStartPoint()    const;
    qreal objectStartX()        const;
    qreal objectStartY()        const;
    QPointF objectMidPoint()      const;
    qreal objectMidX()          const;
    qreal objectMidY()          const;
    QPointF objectEndPoint()      const;
    qreal objectEndX()          const;
    qreal objectEndY()          const;
    qreal objectArea()          const;
    qreal objectArcLength()     const;
    qreal objectChord()         const;
    qreal objectIncludedAngle() const;
    bool objectClockwise()     const;

    void setObjectCenter(const QPointF& point);
    void setObjectCenter(qreal pointX, qreal pointY);
    void setObjectCenterX(qreal pointX);
    void setObjectCenterY(qreal pointY);
    void setObjectRadius(qreal radius);
    void setObjectStartAngle(qreal angle);
    void setObjectEndAngle(qreal angle);
    void setObjectStartPoint(const QPointF& point);
    void setObjectStartPoint(qreal pointX, qreal pointY);
    void setObjectMidPoint(const QPointF& point);
    void setObjectMidPoint(qreal pointX, qreal pointY);
    void setObjectEndPoint(const QPointF& point);
    void setObjectEndPoint(qreal pointX, qreal pointY);

    void updateRubber(QPainter* painter = 0);
    virtual void vulcanize();
    virtual QPointF mouseSnapPoint(const QPointF& mousePoint);
    virtual QList<QPointF> allGripPoints();
    virtual void gripEdit(const QPointF& before, const QPointF& after);
protected:
    void paint(QPainter*, const QStyleOptionGraphicsItem*, QWidget*);
private:
    void init(qreal startX, qreal startY, qreal midX, qreal midY, qreal endX, qreal endY, QRgb rgb, Qt::PenStyle lineType);
    void updatePath();

    void calculateArcData(qreal startX, qreal startY, qreal midX, qreal midY, qreal endX, qreal endY);
    void updateArcRect(qreal radius);

    QPointF arcStartPoint;
    QPointF arcMidPoint;
    QPointF arcEndPoint;
};

class CircleObject : public BaseObject
{
public:
    CircleObject(qreal centerX, qreal centerY, qreal radius, QRgb rgb, QGraphicsItem* parent = 0);
    CircleObject(CircleObject* obj, QGraphicsItem* parent = 0);
    ~CircleObject();

    enum { Type = OBJ_TYPE_CIRCLE };
    virtual int type() const { return Type; }

    QPainterPath objectSavePath() const;

    QPointF objectCenter()        const { return scenePos(); }
    qreal objectCenterX()       const { return scenePos().x(); }
    qreal objectCenterY()       const { return scenePos().y(); }
    qreal objectRadius()        const { return rect().width()/2.0*scale(); }
    qreal objectDiameter()      const { return rect().width()*scale(); }
    qreal objectArea()          const { return pi()*objectRadius()*objectRadius(); }
    qreal objectCircumference() const { return pi()*objectDiameter(); }
    QPointF objectQuadrant0()     const { return objectCenter() + QPointF(objectRadius(), 0); }
    QPointF objectQuadrant90()    const { return objectCenter() + QPointF(0,-objectRadius()); }
    QPointF objectQuadrant180()   const { return objectCenter() + QPointF(-objectRadius(),0); }
    QPointF objectQuadrant270()   const { return objectCenter() + QPointF(0, objectRadius()); }

    void setObjectCenter(const QPointF& center);
    void setObjectCenter(qreal centerX, qreal centerY);
    void setObjectCenterX(qreal centerX);
    void setObjectCenterY(qreal centerY);
    void setObjectRadius(qreal radius);
    void setObjectDiameter(qreal diameter);
    void setObjectArea(qreal area);
    void setObjectCircumference(qreal circumference);

    void updateRubber(QPainter* painter = 0);
    virtual void vulcanize();
    virtual QPointF mouseSnapPoint(const QPointF& mousePoint);
    virtual QList<QPointF> allGripPoints();
    virtual void gripEdit(const QPointF& before, const QPointF& after);
protected:
    void paint(QPainter*, const QStyleOptionGraphicsItem*, QWidget*);
private:
    void init(qreal centerX, qreal centerY, qreal radius, QRgb rgb, Qt::PenStyle lineType);
    void updatePath();
};


class DimLeaderObject : public BaseObject
{
public:
    DimLeaderObject(qreal x1, qreal y1, qreal x2, qreal y2, QRgb rgb, QGraphicsItem* parent = 0);
    DimLeaderObject(DimLeaderObject* obj, QGraphicsItem* parent = 0);
    ~DimLeaderObject();

    enum ArrowStyle
    {
        NoArrow, //NOTE: Allow this enum to evaluate false
        Open,
        Closed,
        Dot,
        Box,
        Tick
    };

    enum lineStyle
    {
        NoLine, //NOTE: Allow this enum to evaluate false
        Flared,
        Fletching
    };

    enum { Type = OBJ_TYPE_DIMLEADER };
    virtual int type() const { return Type; }

    QPointF objectEndPoint1() const;
    QPointF objectEndPoint2() const;
    QPointF objectMidPoint()  const;
    qreal objectX1()        const { return objectEndPoint1().x(); }
    qreal objectY1()        const { return objectEndPoint1().y(); }
    qreal objectX2()        const { return objectEndPoint2().x(); }
    qreal objectY2()        const { return objectEndPoint2().y(); }
    qreal objectDeltaX()    const { return (objectX2() - objectX1()); }
    qreal objectDeltaY()    const { return (objectY2() - objectY1()); }
    qreal objectAngle()     const;
    qreal objectLength()    const { return line().length(); }

    void setObjectEndPoint1(const QPointF& endPt1);
    void setObjectEndPoint1(qreal x1, qreal y1);
    void setObjectEndPoint2(const QPointF& endPt2);
    void setObjectEndPoint2(qreal x2, qreal y2);
    void setObjectX1(qreal x) { setObjectEndPoint1(x, objectY1()); }
    void setObjectY1(qreal y) { setObjectEndPoint1(objectX1(), y); }
    void setObjectX2(qreal x) { setObjectEndPoint2(x, objectY2()); }
    void setObjectY2(qreal y) { setObjectEndPoint2(objectX2(), y); }

    void updateRubber(QPainter* painter = 0);
    virtual void vulcanize();
    virtual QPointF mouseSnapPoint(const QPointF& mousePoint);
    virtual QList<QPointF> allGripPoints();
    virtual void gripEdit(const QPointF& before, const QPointF& after);
protected:
    void paint(QPainter*, const QStyleOptionGraphicsItem*, QWidget*);
private:
    void init(qreal x1, qreal y1, qreal x2, qreal y2, QRgb rgb, Qt::PenStyle lineType);

    bool curved;
    bool filled;
    void updateLeader();
    QPainterPath lineStylePath;
    QPainterPath arrowStylePath;
    qreal arrowStyleAngle;
    qreal arrowStyleLength;
    qreal lineStyleAngle;
    qreal lineStyleLength;
};


class EllipseObject : public BaseObject
{
public:
    EllipseObject(qreal centerX, qreal centerY, qreal width, qreal height, QRgb rgb, QGraphicsItem* parent = 0);
    EllipseObject(EllipseObject* obj, QGraphicsItem* parent = 0);
    ~EllipseObject();

    enum { Type = OBJ_TYPE_ELLIPSE };
    virtual int type() const { return Type; }

    QPainterPath objectSavePath() const;

    QPointF objectCenter()        const { return scenePos(); }
    qreal objectCenterX()       const { return scenePos().x(); }
    qreal objectCenterY()       const { return scenePos().y(); }
    qreal objectRadiusMajor()   const { return qMax(rect().width(), rect().height())/2.0*scale(); }
    qreal objectRadiusMinor()   const { return qMin(rect().width(), rect().height())/2.0*scale(); }
    qreal objectDiameterMajor() const { return qMax(rect().width(), rect().height())*scale(); }
    qreal objectDiameterMinor() const { return qMin(rect().width(), rect().height())*scale(); }
    qreal objectWidth()         const { return rect().width()*scale(); }
    qreal objectHeight()        const { return rect().height()*scale(); }
    QPointF objectQuadrant0()     const;
    QPointF objectQuadrant90()    const;
    QPointF objectQuadrant180()   const;
    QPointF objectQuadrant270()   const;

    void setObjectSize(qreal width, qreal height);
    void setObjectCenter(const QPointF& center);
    void setObjectCenter(qreal centerX, qreal centerY);
    void setObjectCenterX(qreal centerX);
    void setObjectCenterY(qreal centerY);
    void setObjectRadiusMajor(qreal radius);
    void setObjectRadiusMinor(qreal radius);
    void setObjectDiameterMajor(qreal diameter);
    void setObjectDiameterMinor(qreal diameter);

    void updateRubber(QPainter* painter = 0);
    virtual void vulcanize();
    virtual QPointF mouseSnapPoint(const QPointF& mousePoint);
    virtual QList<QPointF> allGripPoints();
    virtual void gripEdit(const QPointF& before, const QPointF& after);
protected:
    void paint(QPainter*, const QStyleOptionGraphicsItem*, QWidget*);
private:
    void init(qreal centerX, qreal centerY, qreal width, qreal height, QRgb rgb, Qt::PenStyle lineType);
    void updatePath();
};


class ImageObject : public BaseObject
{
public:
    ImageObject(qreal x, qreal y, qreal w, qreal h, QRgb rgb, QGraphicsItem* parent = 0);
    ImageObject(ImageObject* obj, QGraphicsItem* parent = 0);
    ~ImageObject();

    enum { Type = OBJ_TYPE_IMAGE };
    virtual int type() const { return Type; }

    QPointF objectTopLeft()     const;
    QPointF objectTopRight()    const;
    QPointF objectBottomLeft()  const;
    QPointF objectBottomRight() const;
    qreal objectWidth()       const { return rect().width()*scale(); }
    qreal objectHeight()      const { return rect().height()*scale(); }
    qreal objectArea()        const { return qAbs(objectWidth()*objectHeight()); }

    void setObjectRect(qreal x, qreal y, qreal w, qreal h);

    void updateRubber(QPainter* painter = 0);
    virtual void vulcanize();
    virtual QPointF mouseSnapPoint(const QPointF& mousePoint);
    virtual QList<QPointF> allGripPoints();
    virtual void gripEdit(const QPointF& before, const QPointF& after);
protected:
    void paint(QPainter*, const QStyleOptionGraphicsItem*, QWidget*);
private:
    void init(qreal x, qreal y, qreal w, qreal h, QRgb rgb, Qt::PenStyle lineType);
    void updatePath();
};


class LineObject : public BaseObject
{
public:
    LineObject(qreal x1, qreal y1, qreal x2, qreal y2, QRgb rgb, QGraphicsItem* parent = 0);
    LineObject(LineObject* obj, QGraphicsItem* parent = 0);
    ~LineObject();

    enum { Type = OBJ_TYPE_LINE };
    virtual int type() const { return Type; }

    QPainterPath objectSavePath() const;

    QPointF objectEndPoint1() const { return scenePos(); }
    QPointF objectEndPoint2() const;
    QPointF objectMidPoint()  const;
    qreal objectX1()        const { return objectEndPoint1().x(); }
    qreal objectY1()        const { return objectEndPoint1().y(); }
    qreal objectX2()        const { return objectEndPoint2().x(); }
    qreal objectY2()        const { return objectEndPoint2().y(); }
    qreal objectDeltaX()    const { return (objectX2() - objectX1()); }
    qreal objectDeltaY()    const { return (objectY2() - objectY1()); }
    qreal objectAngle()     const;
    qreal objectLength()    const { return line().length()*scale(); }

    void setObjectEndPoint1(const QPointF& endPt1);
    void setObjectEndPoint1(qreal x1, qreal y1);
    void setObjectEndPoint2(const QPointF& endPt2);
    void setObjectEndPoint2(qreal x2, qreal y2);
    void setObjectX1(qreal x) { setObjectEndPoint1(x, objectY1()); }
    void setObjectY1(qreal y) { setObjectEndPoint1(objectX1(), y); }
    void setObjectX2(qreal x) { setObjectEndPoint2(x, objectY2()); }
    void setObjectY2(qreal y) { setObjectEndPoint2(objectX2(), y); }

    void updateRubber(QPainter* painter = 0);
    virtual void vulcanize();
    virtual QPointF mouseSnapPoint(const QPointF& mousePoint);
    virtual QList<QPointF> allGripPoints();
    virtual void gripEdit(const QPointF& before, const QPointF& after);
protected:
    void paint(QPainter*, const QStyleOptionGraphicsItem*, QWidget*);
private:
    void init(qreal x1, qreal y1, qreal x2, qreal y2, QRgb rgb, Qt::PenStyle lineType);
};


class PathObject : public BaseObject
{
public:
    PathObject(qreal x, qreal y, const QPainterPath p, QRgb rgb, QGraphicsItem* parent = 0);
    PathObject(PathObject* obj, QGraphicsItem* parent = 0);
    ~PathObject();

    enum { Type = OBJ_TYPE_PATH };
    virtual int type() const { return Type; }

    QPainterPath objectCopyPath() const;
    QPainterPath objectSavePath() const;

    QPointF objectPos() const { return scenePos(); }
    qreal objectX()   const { return scenePos().x(); }
    qreal objectY()   const { return scenePos().y(); }

    void setObjectPos(const QPointF& point) { setPos(point.x(), point.y()); }
    void setObjectPos(qreal x, qreal y) { setPos(x, y); }
    void setObjectX(qreal x) { setObjectPos(x, objectY()); }
    void setObjectY(qreal y) { setObjectPos(objectX(), y); }

    void updateRubber(QPainter* painter = 0);
    virtual void vulcanize();
    virtual QPointF mouseSnapPoint(const QPointF& mousePoint);
    virtual QList<QPointF> allGripPoints();
    virtual void gripEdit(const QPointF& before, const QPointF& after);
protected:
    void paint(QPainter*, const QStyleOptionGraphicsItem*, QWidget*);
private:
    void init(qreal x, qreal y, const QPainterPath& p, QRgb rgb, Qt::PenStyle lineType);
    void updatePath(const QPainterPath& p);
    QPainterPath normalPath;
    //TODO: make paths similar to polylines. Review and implement any missing functions/members.
};


class PointObject : public BaseObject
{
public:
    PointObject(qreal x, qreal y, QRgb rgb, QGraphicsItem* parent = 0);
    PointObject(PointObject* obj, QGraphicsItem* parent = 0);
    ~PointObject();

    enum { Type = OBJ_TYPE_POINT };
    virtual int type() const { return Type; }

    QPainterPath objectSavePath() const;

    QPointF objectPos() const { return scenePos(); }
    qreal objectX()   const { return scenePos().x(); }
    qreal objectY()   const { return scenePos().y(); }

    void setObjectPos(const QPointF& point) { setPos(point.x(), point.y()); }
    void setObjectPos(qreal x, qreal y) { setPos(x, y); }
    void setObjectX(qreal x) { setObjectPos(x, objectY()); }
    void setObjectY(qreal y) { setObjectPos(objectX(), y); }

    void updateRubber(QPainter* painter = 0);
    virtual void vulcanize();
    virtual QPointF mouseSnapPoint(const QPointF& mousePoint);
    virtual QList<QPointF> allGripPoints();
    virtual void gripEdit(const QPointF& before, const QPointF& after);
protected:
    void paint(QPainter*, const QStyleOptionGraphicsItem*, QWidget*);
private:
    void init(qreal x, qreal y, QRgb rgb, Qt::PenStyle lineType);
};


class PolygonObject : public BaseObject
{
public:
    PolygonObject(qreal x, qreal y, const QPainterPath& p, QRgb rgb, QGraphicsItem* parent = 0);
    PolygonObject(PolygonObject* obj, QGraphicsItem* parent = 0);
    ~PolygonObject();

    enum { Type = OBJ_TYPE_POLYGON };
    virtual int type() const { return Type; }

    QPainterPath objectCopyPath() const;
    QPainterPath objectSavePath() const;

    QPointF objectPos() const { return scenePos(); }
    qreal objectX()   const { return scenePos().x(); }
    qreal objectY()   const { return scenePos().y(); }

    void setObjectPos(const QPointF& point) { setPos(point.x(), point.y()); }
    void setObjectPos(qreal x, qreal y) { setPos(x, y); }
    void setObjectX(qreal x) { setObjectPos(x, objectY()); }
    void setObjectY(qreal y) { setObjectPos(objectX(), y); }

    void updateRubber(QPainter* painter = 0);
    virtual void vulcanize();
    virtual QPointF mouseSnapPoint(const QPointF& mousePoint);
    virtual QList<QPointF> allGripPoints();
    virtual void gripEdit(const QPointF& before, const QPointF& after);
protected:
    void paint(QPainter*, const QStyleOptionGraphicsItem*, QWidget*);
private:
    void init(qreal x, qreal y, const QPainterPath& p, QRgb rgb, Qt::PenStyle lineType);
    void updatePath(const QPainterPath& p);
    QPainterPath normalPath;
    int findIndex(const QPointF& point);
    int gripIndex;
};


class PolylineObject : public BaseObject
{
public:
    PolylineObject(qreal x, qreal y, const QPainterPath& p, QRgb rgb, QGraphicsItem* parent = 0);
    PolylineObject(PolylineObject* obj, QGraphicsItem* parent = 0);
    ~PolylineObject();

    enum { Type = OBJ_TYPE_POLYLINE };
    virtual int type() const { return Type; }

    QPainterPath objectCopyPath() const;
    QPainterPath objectSavePath() const;

    QPointF objectPos() const { return scenePos(); }
    qreal objectX()   const { return scenePos().x(); }
    qreal objectY()   const { return scenePos().y(); }

    void setObjectPos(const QPointF& point) { setPos(point.x(), point.y()); }
    void setObjectPos(qreal x, qreal y) { setPos(x, y); }
    void setObjectX(qreal x) { setObjectPos(x, objectY()); }
    void setObjectY(qreal y) { setObjectPos(objectX(), y); }

    void updateRubber(QPainter* painter = 0);
    virtual void vulcanize();
    virtual QPointF mouseSnapPoint(const QPointF& mousePoint);
    virtual QList<QPointF> allGripPoints();
    virtual void gripEdit(const QPointF& before, const QPointF& after);
protected:
    void paint(QPainter*, const QStyleOptionGraphicsItem*, QWidget*);
private:
    void init(qreal x, qreal y, const QPainterPath& p, QRgb rgb, Qt::PenStyle lineType);
    void updatePath(const QPainterPath& p);
    QPainterPath normalPath;
    int findIndex(const QPointF& point);
    int gripIndex;
};


class RectObject : public BaseObject
{
public:
    RectObject(qreal x, qreal y, qreal w, qreal h, QRgb rgb, QGraphicsItem* parent = 0);
    RectObject(RectObject* obj, QGraphicsItem* parent = 0);
    ~RectObject();

    enum { Type = OBJ_TYPE_RECTANGLE };
    virtual int type() const { return Type; }

    QPainterPath objectSavePath() const;

    QPointF objectPos() const { return scenePos(); }

    QPointF objectTopLeft()     const;
    QPointF objectTopRight()    const;
    QPointF objectBottomLeft()  const;
    QPointF objectBottomRight() const;
    qreal objectWidth()       const { return rect().width()*scale(); }
    qreal objectHeight()      const { return rect().height()*scale(); }
    qreal objectArea()        const { return qAbs(objectWidth()*objectHeight()); }

    void setObjectRect(qreal x, qreal y, qreal w, qreal h);

    void updateRubber(QPainter* painter = 0);
    virtual void vulcanize();
    virtual QPointF mouseSnapPoint(const QPointF& mousePoint);
    virtual QList<QPointF> allGripPoints();
    virtual void gripEdit(const QPointF& before, const QPointF& after);
protected:
    void paint(QPainter*, const QStyleOptionGraphicsItem*, QWidget*);
private:
    void init(qreal x, qreal y, qreal w, qreal h, QRgb rgb, Qt::PenStyle lineType);
    void updatePath();
};


class TextSingleObject : public BaseObject
{
public:
    TextSingleObject(const QString& str, qreal x, qreal y, QRgb rgb, QGraphicsItem* parent = 0);
    TextSingleObject(TextSingleObject* obj, QGraphicsItem* parent = 0);
    ~TextSingleObject();

    enum { Type = OBJ_TYPE_TEXTSINGLE };
    virtual int type() const { return Type; }

    QList<QPainterPath> objectSavePathList() const { return subPathList(); }
    QList<QPainterPath> subPathList() const;

    QString objectText()           const { return objText; }
    QString objectTextFont()       const { return objTextFont; }
    QString objectTextJustify()    const { return objTextJustify; }
    qreal objectTextSize()       const { return objTextSize; }
    bool objectTextBold()       const { return objTextBold; }
    bool objectTextItalic()     const { return objTextItalic; }
    bool objectTextUnderline()  const { return objTextUnderline; }
    bool objectTextStrikeOut()  const { return objTextStrikeOut; }
    bool objectTextOverline()   const { return objTextOverline; }
    bool objectTextBackward()   const { return objTextBackward; }
    bool objectTextUpsideDown() const { return objTextUpsideDown; }
    QPointF objectPos()            const { return scenePos(); }
    qreal objectX()              const { return scenePos().x(); }
    qreal objectY()              const { return scenePos().y(); }

    QStringList objectTextJustifyList() const;

    void setObjectText(const QString& str);
    void setObjectTextFont(const QString& font);
    void setObjectTextJustify(const QString& justify);
    void setObjectTextSize(qreal size);
    void setObjectTextStyle(bool bold, bool italic, bool under, bool strike, bool over);
    void setObjectTextBold(bool val);
    void setObjectTextItalic(bool val);
    void setObjectTextUnderline(bool val);
    void setObjectTextStrikeOut(bool val);
    void setObjectTextOverline(bool val);
    void setObjectTextBackward(bool val);
    void setObjectTextUpsideDown(bool val);
    void setObjectPos(const QPointF& point) { setPos(point.x(), point.y()); }
    void setObjectPos(qreal x, qreal y) { setPos(x, y); }
    void setObjectX(qreal x) { setObjectPos(x, objectY()); }
    void setObjectY(qreal y) { setObjectPos(objectX(), y); }

    void updateRubber(QPainter* painter = 0);
    virtual void vulcanize();
    virtual QPointF mouseSnapPoint(const QPointF& mousePoint);
    virtual QList<QPointF> allGripPoints();
    virtual void gripEdit(const QPointF& before, const QPointF& after);
protected:
    void paint(QPainter*, const QStyleOptionGraphicsItem*, QWidget*);
private:
    void init(const QString& str, qreal x, qreal y, QRgb rgb, Qt::PenStyle lineType);

    QString objText;
    QString objTextFont;
    QString objTextJustify;
    qreal objTextSize;
    bool objTextBold;
    bool objTextItalic;
    bool objTextUnderline;
    bool objTextStrikeOut;
    bool objTextOverline;
    bool objTextBackward;
    bool objTextUpsideDown;
    QPainterPath objTextPath;
};


class SaveObject : public QObject
{
    Q_OBJECT

public:
    SaveObject(QGraphicsScene* theScene, QObject* parent = 0);
    ~SaveObject();

    bool save(const QString &fileName);

    void addArc          (EmbPattern* pattern, QGraphicsItem* item);
    void addBlock        (EmbPattern* pattern, QGraphicsItem* item);
    void addCircle       (EmbPattern* pattern, QGraphicsItem* item);
    void addDimAligned   (EmbPattern* pattern, QGraphicsItem* item);
    void addDimAngular   (EmbPattern* pattern, QGraphicsItem* item);
    void addDimArcLength (EmbPattern* pattern, QGraphicsItem* item);
    void addDimDiameter  (EmbPattern* pattern, QGraphicsItem* item);
    void addDimLeader    (EmbPattern* pattern, QGraphicsItem* item);
    void addDimLinear    (EmbPattern* pattern, QGraphicsItem* item);
    void addDimOrdinate  (EmbPattern* pattern, QGraphicsItem* item);
    void addDimRadius    (EmbPattern* pattern, QGraphicsItem* item);
    void addEllipse      (EmbPattern* pattern, QGraphicsItem* item);
    void addEllipseArc   (EmbPattern* pattern, QGraphicsItem* item);
    void addGrid         (EmbPattern* pattern, QGraphicsItem* item);
    void addHatch        (EmbPattern* pattern, QGraphicsItem* item);
    void addImage        (EmbPattern* pattern, QGraphicsItem* item);
    void addInfiniteLine (EmbPattern* pattern, QGraphicsItem* item);
    void addLine         (EmbPattern* pattern, QGraphicsItem* item);
    void addPath         (EmbPattern* pattern, QGraphicsItem* item);
    void addPoint (EmbPattern* pattern, QGraphicsItem* item);
    void addPolygon      (EmbPattern* pattern, QGraphicsItem* item);
    void addPolyline     (EmbPattern* pattern, QGraphicsItem* item);
    void addRay          (EmbPattern* pattern, QGraphicsItem* item);
    void addRectangle    (EmbPattern* pattern, QGraphicsItem* item);
    void addSlot         (EmbPattern* pattern, QGraphicsItem* item);
    void addSpline       (EmbPattern* pattern, QGraphicsItem* item);
    void addTextMulti    (EmbPattern* pattern, QGraphicsItem* item);
    void addTextSingle   (EmbPattern* pattern, QGraphicsItem* item);

private:
    QGraphicsScene* gscene;
    int formatType;

    void toPolyline(EmbPattern* pattern, const QPointF& objPos, const QPainterPath& objPath, const QString& layer, const QColor& color, const QString& lineType, const QString& lineWeight);
};


class PreviewDialog : public QFileDialog
{
    Q_OBJECT

public:
    PreviewDialog(QWidget* parent = 0,
                  const QString& caption = QString(),
                  const QString& directory = QString(),
                  const QString& filter = QString());
    ~PreviewDialog();

private:
    ImageWidget* imgWidget;
};

class PropertyEditor : public QDockWidget
{
    Q_OBJECT

public:
    PropertyEditor(const QString& iconDirectory = QString(), bool pickAddMode = true, QWidget* widgetToFocus = 0, QWidget* parent = 0); //, Qt::WindowFlags flags = 0);
    ~PropertyEditor();

protected:
    bool eventFilter(QObject *obj, QEvent *event);

signals:
    void pickAddModeToggled();

public slots:
    void setSelectedItems(QList<QGraphicsItem*> itemList);
    void updatePickAddModeButton(bool pickAddMode);

private slots:
    void fieldEdited(QObject* fieldObj);
    void showGroups(int objType);
    void showOneType(int index);
    void hideAllGroups();
    void clearAllFields();
    void togglePickAddMode();

private:
    QWidget*     focusWidget;

    QString iconDir;
    int iconSize;
    Qt::ToolButtonStyle propertyEditorButtonStyle;

    bool pickAdd;

    QList<QGraphicsItem*> selectedItemList;

    ArcObject*          tempArcObj;
    BlockObject*        tempBlockObj;
    CircleObject*       tempCircleObj;
    DimAlignedObject*   tempDimAlignedObj;
    DimAngularObject*   tempDimAngularObj;
    DimArcLengthObject* tempDimArcLenObj;
    DimDiameterObject*  tempDimDiamObj;
    DimLeaderObject*    tempDimLeaderObj;
    DimLinearObject*    tempDimLinearObj;
    DimOrdinateObject*  tempDimOrdObj;
    DimRadiusObject*    tempDimRadiusObj;
    EllipseObject*      tempEllipseObj;
    EllipseArcObject*   tempEllipseArcObj;
    HatchObject*        tempHatchObj;
    ImageObject*        tempImageObj;
    InfiniteLineObject* tempInfLineObj;
    LineObject*         tempLineObj;
    PathObject*         tempPathObj;
    PointObject*        tempPointObj;
    PolygonObject*      tempPolygonObj;
    PolylineObject*     tempPolylineObj;
    RayObject*          tempRayObj;
    RectObject*         tempRectObj;
    SplineObject*       tempSplineObj;
    TextMultiObject*    tempTextMultiObj;
    TextSingleObject*   tempTextSingleObj;

    //Helper functions
    QToolButton*   createToolButton(const QString& iconName, const QString& txt);
    QLineEdit*     createLineEdit(const QString& validatorType = QString(), bool readOnly = false);
    QComboBox*     createComboBox(bool disable = false);
    QFontComboBox* createFontComboBox(bool disable = false);

    int precisionAngle;
    int precisionLength;

    //Used when checking if fields vary
    QString fieldOldText;
    QString fieldNewText;
    QString fieldVariesText;
    QString fieldYesText;
    QString fieldNoText;
    QString fieldOnText;
    QString fieldOffText;

    void updateLineEditStrIfVaries(QLineEdit* lineEdit, const QString& str);
    void updateLineEditNumIfVaries(QLineEdit* lineEdit, qreal num, bool useAnglePrecision);
    void updateFontComboBoxStrIfVaries(QFontComboBox* fontComboBox, const QString& str);
    void updateComboBoxStrIfVaries(QComboBox* comboBox, const QString& str, const QStringList& strList);
    void updateComboBoxBoolIfVaries(QComboBox* comboBox, bool val, bool yesOrNoText);

    QSignalMapper* signalMapper;
    void mapSignal(QObject* fieldObj, const QString& name, QVariant value);

    /* Selection */
    QComboBox*   createComboBoxSelected();
    QToolButton* createToolButtonQSelect();
    QToolButton* createToolButtonPickAdd();

    QComboBox*   comboBoxSelected;
    QToolButton* toolButtonQSelect;
    QToolButton* toolButtonPickAdd;

    //TODO: Alphabetic/Categorized TabWidget

    /* General */
    QGroupBox*   createGroupBoxGeneral();
    QGroupBox*   groupBoxGeneral;

    QToolButton* toolButtonGeneralLayer;
    QToolButton* toolButtonGeneralColor;
    QToolButton* toolButtonGeneralLineType;
    QToolButton* toolButtonGeneralLineWeight;

    QComboBox*   comboBoxGeneralLayer;
    QComboBox*   comboBoxGeneralColor;
    QComboBox*   comboBoxGeneralLineType;
    QComboBox*   comboBoxGeneralLineWeight;

    /* Geometry */

    //Arc
    QGroupBox*   createGroupBoxGeometryArc();
    QGroupBox*   groupBoxGeometryArc;

    QGroupBox*   createGroupBoxMiscArc();
    QGroupBox*   groupBoxMiscArc;

    QToolButton* toolButtonArcClockwise;

    QComboBox*   comboBoxArcClockwise;

    //Block
    QGroupBox*   createGroupBoxGeometryBlock();
    QGroupBox*   groupBoxGeometryBlock;

    QToolButton* toolButtonBlockX;
    QToolButton* toolButtonBlockY;

    QLineEdit*   lineEditBlockX;
    QLineEdit*   lineEditBlockY;

    //Circle
    QGroupBox*   createGroupBoxGeometryCircle();
    QGroupBox*   groupBoxGeometryCircle;

    QToolButton* toolButtonCircleCenterX;
    QToolButton* toolButtonCircleCenterY;
    QToolButton* toolButtonCircleRadius;
    QToolButton* toolButtonCircleDiameter;
    QToolButton* toolButtonCircleArea;
    QToolButton* toolButtonCircleCircumference;

    QLineEdit*   lineEditCircleCenterX;
    QLineEdit*   lineEditCircleCenterY;
    QLineEdit*   lineEditCircleRadius;
    QLineEdit*   lineEditCircleDiameter;
    QLineEdit*   lineEditCircleArea;
    QLineEdit*   lineEditCircleCircumference;

    //DimAligned
    QGroupBox*   createGroupBoxGeometryDimAligned();
    QGroupBox*   groupBoxGeometryDimAligned;

    //TODO: toolButtons and lineEdits for DimAligned

    //DimAngular
    QGroupBox*   createGroupBoxGeometryDimAngular();
    QGroupBox*   groupBoxGeometryDimAngular;

    //TODO: toolButtons and lineEdits for DimAngular

    //DimArcLength
    QGroupBox*   createGroupBoxGeometryDimArcLength();
    QGroupBox*   groupBoxGeometryDimArcLength;

    //TODO: toolButtons and lineEdits for DimArcLength

    //DimDiameter
    QGroupBox*   createGroupBoxGeometryDimDiameter();
    QGroupBox*   groupBoxGeometryDimDiameter;

    //TODO: toolButtons and lineEdits for DimDiameter

    //DimLeader
    QGroupBox*   createGroupBoxGeometryDimLeader();
    QGroupBox*   groupBoxGeometryDimLeader;

    //TODO: toolButtons and lineEdits for DimLeader

    //DimLinear
    QGroupBox*   createGroupBoxGeometryDimLinear();
    QGroupBox*   groupBoxGeometryDimLinear;

    //TODO: toolButtons and lineEdits for DimLinear

    //DimOrdinate
    QGroupBox*   createGroupBoxGeometryDimOrdinate();
    QGroupBox*   groupBoxGeometryDimOrdinate;

    //TODO: toolButtons and lineEdits for DimOrdinate

    //DimRadius
    QGroupBox*   createGroupBoxGeometryDimRadius();
    QGroupBox*   groupBoxGeometryDimRadius;

    //TODO: toolButtons and lineEdits for DimRadius

    //Ellipse
    QGroupBox*   createGroupBoxGeometryEllipse();
    QGroupBox*   groupBoxGeometryEllipse;

    QToolButton* toolButtonEllipseCenterX;
    QToolButton* toolButtonEllipseCenterY;
    QToolButton* toolButtonEllipseRadiusMajor;
    QToolButton* toolButtonEllipseRadiusMinor;
    QToolButton* toolButtonEllipseDiameterMajor;
    QToolButton* toolButtonEllipseDiameterMinor;

    QLineEdit*   lineEditEllipseCenterX;
    QLineEdit*   lineEditEllipseCenterY;
    QLineEdit*   lineEditEllipseRadiusMajor;
    QLineEdit*   lineEditEllipseRadiusMinor;
    QLineEdit*   lineEditEllipseDiameterMajor;
    QLineEdit*   lineEditEllipseDiameterMinor;

    //Image
    QGroupBox*   createGroupBoxGeometryImage();
    QGroupBox*   groupBoxGeometryImage;

    QToolButton* toolButtonImageX;
    QToolButton* toolButtonImageY;
    QToolButton* toolButtonImageWidth;
    QToolButton* toolButtonImageHeight;

    QLineEdit*   lineEditImageX;
    QLineEdit*   lineEditImageY;
    QLineEdit*   lineEditImageWidth;
    QLineEdit*   lineEditImageHeight;

    QGroupBox*   createGroupBoxMiscImage();
    QGroupBox*   groupBoxMiscImage;

    QToolButton* toolButtonImageName;
    QToolButton* toolButtonImagePath;

    QLineEdit*   lineEditImageName;
    QLineEdit*   lineEditImagePath;

    //Infinite Line
    QGroupBox*   createGroupBoxGeometryInfiniteLine();
    QGroupBox*   groupBoxGeometryInfiniteLine;

    QToolButton* toolButtonInfiniteLineX1;
    QToolButton* toolButtonInfiniteLineY1;
    QToolButton* toolButtonInfiniteLineX2;
    QToolButton* toolButtonInfiniteLineY2;
    QToolButton* toolButtonInfiniteLineVectorX;
    QToolButton* toolButtonInfiniteLineVectorY;

    QLineEdit*   lineEditInfiniteLineX1;
    QLineEdit*   lineEditInfiniteLineY1;
    QLineEdit*   lineEditInfiniteLineX2;
    QLineEdit*   lineEditInfiniteLineY2;
    QLineEdit*   lineEditInfiniteLineVectorX;
    QLineEdit*   lineEditInfiniteLineVectorY;

    //Line
    QGroupBox*   createGroupBoxGeometryLine();
    QGroupBox*   groupBoxGeometryLine;

    QToolButton* toolButtonLineStartX;
    QToolButton* toolButtonLineStartY;
    QToolButton* toolButtonLineEndX;
    QToolButton* toolButtonLineEndY;
    QToolButton* toolButtonLineDeltaX;
    QToolButton* toolButtonLineDeltaY;
    QToolButton* toolButtonLineAngle;
    QToolButton* toolButtonLineLength;

    QLineEdit*   lineEditLineStartX;
    QLineEdit*   lineEditLineStartY;
    QLineEdit*   lineEditLineEndX;
    QLineEdit*   lineEditLineEndY;
    QLineEdit*   lineEditLineDeltaX;
    QLineEdit*   lineEditLineDeltaY;
    QLineEdit*   lineEditLineAngle;
    QLineEdit*   lineEditLineLength;

    //Path
    QGroupBox*   createGroupBoxGeometryPath();
    QGroupBox*   groupBoxGeometryPath;

    QToolButton* toolButtonPathVertexNum;
    QToolButton* toolButtonPathVertexX;
    QToolButton* toolButtonPathVertexY;
    QToolButton* toolButtonPathArea;
    QToolButton* toolButtonPathLength;

    QComboBox*   comboBoxPathVertexNum;
    QLineEdit*   lineEditPathVertexX;
    QLineEdit*   lineEditPathVertexY;
    QLineEdit*   lineEditPathArea;
    QLineEdit*   lineEditPathLength;

    QGroupBox*   createGroupBoxMiscPath();
    QGroupBox*   groupBoxMiscPath;

    QToolButton* toolButtonPathClosed;

    QComboBox*   comboBoxPathClosed;

    //Point
    QGroupBox*   createGroupBoxGeometryPoint();
    QGroupBox*   groupBoxGeometryPoint;

    QToolButton* toolButtonPointX;
    QToolButton* toolButtonPointY;

    QLineEdit*   lineEditPointX;
    QLineEdit*   lineEditPointY;

    //Polygon
    QGroupBox*   createGroupBoxGeometryPolygon();
    QGroupBox*   groupBoxGeometryPolygon;

    QToolButton* toolButtonPolygonCenterX;
    QToolButton* toolButtonPolygonCenterY;
    QToolButton* toolButtonPolygonRadiusVertex;
    QToolButton* toolButtonPolygonRadiusSide;
    QToolButton* toolButtonPolygonDiameterVertex;
    QToolButton* toolButtonPolygonDiameterSide;
    QToolButton* toolButtonPolygonInteriorAngle;

    QLineEdit*   lineEditPolygonCenterX;
    QLineEdit*   lineEditPolygonCenterY;
    QLineEdit*   lineEditPolygonRadiusVertex;
    QLineEdit*   lineEditPolygonRadiusSide;
    QLineEdit*   lineEditPolygonDiameterVertex;
    QLineEdit*   lineEditPolygonDiameterSide;
    QLineEdit*   lineEditPolygonInteriorAngle;

    //Polyline
    QGroupBox*   createGroupBoxGeometryPolyline();
    QGroupBox*   groupBoxGeometryPolyline;

    QToolButton* toolButtonPolylineVertexNum;
    QToolButton* toolButtonPolylineVertexX;
    QToolButton* toolButtonPolylineVertexY;
    QToolButton* toolButtonPolylineArea;
    QToolButton* toolButtonPolylineLength;

    QComboBox*   comboBoxPolylineVertexNum;
    QLineEdit*   lineEditPolylineVertexX;
    QLineEdit*   lineEditPolylineVertexY;
    QLineEdit*   lineEditPolylineArea;
    QLineEdit*   lineEditPolylineLength;

    QGroupBox*   createGroupBoxMiscPolyline();
    QGroupBox*   groupBoxMiscPolyline;

    QToolButton* toolButtonPolylineClosed;

    QComboBox*   comboBoxPolylineClosed;

    //Ray
    QGroupBox*   createGroupBoxGeometryRay();
    QGroupBox*   groupBoxGeometryRay;

    QToolButton* toolButtonRayX1;
    QToolButton* toolButtonRayY1;
    QToolButton* toolButtonRayX2;
    QToolButton* toolButtonRayY2;
    QToolButton* toolButtonRayVectorX;
    QToolButton* toolButtonRayVectorY;

    QLineEdit*   lineEditRayX1;
    QLineEdit*   lineEditRayY1;
    QLineEdit*   lineEditRayX2;
    QLineEdit*   lineEditRayY2;
    QLineEdit*   lineEditRayVectorX;
    QLineEdit*   lineEditRayVectorY;

    //Rectangle
    QGroupBox*   createGroupBoxGeometryRectangle();
    QGroupBox*   groupBoxGeometryRectangle;

    QToolButton* toolButtonRectangleCorner1X;
    QToolButton* toolButtonRectangleCorner1Y;
    QToolButton* toolButtonRectangleCorner2X;
    QToolButton* toolButtonRectangleCorner2Y;
    QToolButton* toolButtonRectangleCorner3X;
    QToolButton* toolButtonRectangleCorner3Y;
    QToolButton* toolButtonRectangleCorner4X;
    QToolButton* toolButtonRectangleCorner4Y;
    QToolButton* toolButtonRectangleWidth;
    QToolButton* toolButtonRectangleHeight;
    QToolButton* toolButtonRectangleArea;

    QLineEdit*   lineEditRectangleCorner1X;
    QLineEdit*   lineEditRectangleCorner1Y;
    QLineEdit*   lineEditRectangleCorner2X;
    QLineEdit*   lineEditRectangleCorner2Y;
    QLineEdit*   lineEditRectangleCorner3X;
    QLineEdit*   lineEditRectangleCorner3Y;
    QLineEdit*   lineEditRectangleCorner4X;
    QLineEdit*   lineEditRectangleCorner4Y;
    QLineEdit*   lineEditRectangleWidth;
    QLineEdit*   lineEditRectangleHeight;
    QLineEdit*   lineEditRectangleArea;

    //Text Multi
    QGroupBox*   createGroupBoxGeometryTextMulti();
    QGroupBox*   groupBoxGeometryTextMulti;

    QToolButton* toolButtonTextMultiX;
    QToolButton* toolButtonTextMultiY;

    QLineEdit*   lineEditTextMultiX;
    QLineEdit*   lineEditTextMultiY;

    //Text Single
    QGroupBox*   createGroupBoxTextTextSingle();
    QGroupBox*   groupBoxTextTextSingle;

    QToolButton* toolButtonTextSingleContents;
    QToolButton* toolButtonTextSingleFont;
    QToolButton* toolButtonTextSingleJustify;
    QToolButton* toolButtonTextSingleHeight;
    QToolButton* toolButtonTextSingleRotation;

    QLineEdit*     lineEditTextSingleContents;
    QFontComboBox* comboBoxTextSingleFont;
    QComboBox*     comboBoxTextSingleJustify;
    QLineEdit*     lineEditTextSingleHeight;
    QLineEdit*     lineEditTextSingleRotation;

    QGroupBox*   createGroupBoxGeometryTextSingle();
    QGroupBox*   groupBoxGeometryTextSingle;

    QToolButton* toolButtonTextSingleX;
    QToolButton* toolButtonTextSingleY;

    QLineEdit*   lineEditTextSingleX;
    QLineEdit*   lineEditTextSingleY;

    QGroupBox*   createGroupBoxMiscTextSingle();
    QGroupBox*   groupBoxMiscTextSingle;

    QToolButton* toolButtonTextSingleBackward;
    QToolButton* toolButtonTextSingleUpsideDown;

    QComboBox*   comboBoxTextSingleBackward;
    QComboBox*   comboBoxTextSingleUpsideDown;
};

class Settings_Dialog : public QDialog
{
    Q_OBJECT

public:
    Settings_Dialog(MainWindow* mw, const QString& showTab = QString(), QWidget *parent = 0);
    ~Settings_Dialog();

private:
    QTabWidget*       tabWidget;

    QWidget*          createTabGeneral();
    QWidget*          createTabFilesPaths();
    QWidget*          createTabDisplay();
    QWidget*          createTabPrompt();
    QWidget*          createTabOpenSave();
    QWidget*          createTabPrinting();
    QWidget*          createTabSnap();
    QWidget*          createTabGridRuler();
    QWidget*          createTabOrthoPolar();
    QWidget*          createTabQuickSnap();
    QWidget*          createTabQuickTrack();
    QWidget*          createTabLineWeight();
    QWidget*          createTabSelection();

    QDialogButtonBox* buttonBox;

    void addColorsToComboBox(QComboBox* comboBox);

    //Temporary for instant preview
    bool preview_general_mdi_bg_use_logo;
    bool preview_general_mdi_bg_use_texture;
    bool preview_general_mdi_bg_use_color;

    QString accept_general_mdi_bg_logo;
    QString accept_general_mdi_bg_texture;
    QRgb preview_general_mdi_bg_color;
    QRgb accept_general_mdi_bg_color;

    bool preview_display_show_scrollbars;

    QRgb preview_display_crosshair_color;
    QRgb accept_display_crosshair_color;
    QRgb preview_display_bg_color;
    QRgb accept_display_bg_color;

    QRgb preview_display_selectbox_left_color;
    QRgb accept_display_selectbox_left_color;
    QRgb preview_display_selectbox_left_fill;
    QRgb accept_display_selectbox_left_fill;
    QRgb preview_display_selectbox_right_color;
    QRgb accept_display_selectbox_right_color;
    QRgb preview_display_selectbox_right_fill;
    QRgb accept_display_selectbox_right_fill;
    quint8 preview_display_selectbox_alpha;

    QRgb preview_prompt_text_color;
    QRgb accept_prompt_text_color;

    QRgb preview_prompt_bg_color;
    QRgb accept_prompt_bg_color;

    QString preview_prompt_font_family;
    QString preview_prompt_font_style;
    quint8 preview_prompt_font_size;

    QRgb preview_grid_color;
    QRgb accept_grid_color;

    QRgb preview_ruler_color;
    QRgb accept_ruler_color;

    bool preview_lwt_show_lwt;
    bool preview_lwt_real_render;

    //Temporary until changes are accepted
    QString dialog_general_language;
    QString dialog_general_icon_theme;
    int dialog_general_icon_size;
    bool dialog_general_mdi_bg_use_logo;
    bool dialog_general_mdi_bg_use_texture;
    bool dialog_general_mdi_bg_use_color;
    QString dialog_general_mdi_bg_logo;
    QString dialog_general_mdi_bg_texture;
    QRgb dialog_general_mdi_bg_color;
    bool dialog_general_tip_of_the_day;
    bool dialog_general_system_help_browser;
    bool dialog_display_use_opengl;
    bool dialog_display_renderhint_aa;
    bool dialog_display_renderhint_text_aa;
    bool dialog_display_renderhint_smooth_pix;
    bool dialog_display_renderhint_high_aa;
    bool dialog_display_renderhint_noncosmetic;
    bool dialog_display_show_scrollbars;
    int dialog_display_scrollbar_widget_num;
    QRgb dialog_display_crosshair_color;
    QRgb dialog_display_bg_color;
    QRgb dialog_display_selectbox_left_color;
    QRgb dialog_display_selectbox_left_fill;
    QRgb dialog_display_selectbox_right_color;
    QRgb dialog_display_selectbox_right_fill;
    quint8 dialog_display_selectbox_alpha;
    qreal dialog_display_zoomscale_in;
    qreal dialog_display_zoomscale_out;
    quint8 dialog_display_crosshair_percent;
    QString dialog_display_units;
    QRgb dialog_prompt_text_color;
    QRgb dialog_prompt_bg_color;
    QString dialog_prompt_font_family;
    QString dialog_prompt_font_style;
    quint8 dialog_prompt_font_size;
    bool dialog_prompt_save_history;
    bool dialog_prompt_save_history_as_html;
    QString dialog_prompt_save_history_filename;
    QString dialog_opensave_custom_filter;
    QString dialog_opensave_open_format;
    bool dialog_opensave_open_thumbnail;
    QString dialog_opensave_save_format;
    bool dialog_opensave_save_thumbnail;
    quint8 dialog_opensave_recent_max_files;
    quint8 dialog_opensave_trim_dst_num_jumps;
    QString dialog_printing_default_device;
    bool dialog_printing_use_last_device;
    bool dialog_printing_disable_bg;
    bool dialog_grid_show_on_load;
    bool dialog_grid_show_origin;
    bool dialog_grid_color_match_crosshair;
    QRgb dialog_grid_color;
    bool dialog_grid_load_from_file;
    QString dialog_grid_type;
    bool dialog_grid_center_on_origin;
    qreal dialog_grid_center_x;
    qreal dialog_grid_center_y;
    qreal dialog_grid_size_x;
    qreal dialog_grid_size_y;
    qreal dialog_grid_spacing_x;
    qreal dialog_grid_spacing_y;
    qreal dialog_grid_size_radius;
    qreal dialog_grid_spacing_radius;
    qreal dialog_grid_spacing_angle;
    bool dialog_ruler_show_on_load;
    bool dialog_ruler_metric;
    QRgb dialog_ruler_color;
    quint8 dialog_ruler_pixel_size;
    bool dialog_qsnap_enabled;
    QRgb dialog_qsnap_locator_color;
    quint8 dialog_qsnap_locator_size;
    quint8 dialog_qsnap_aperture_size;
    bool dialog_qsnap_endpoint;
    bool dialog_qsnap_midpoint;
    bool dialog_qsnap_center;
    bool dialog_qsnap_node;
    bool dialog_qsnap_quadrant;
    bool dialog_qsnap_intersection;
    bool dialog_qsnap_extension;
    bool dialog_qsnap_insertion;
    bool dialog_qsnap_perpendicular;
    bool dialog_qsnap_tangent;
    bool dialog_qsnap_nearest;
    bool dialog_qsnap_apparent;
    bool dialog_qsnap_parallel;
    bool dialog_lwt_show_lwt;
    bool dialog_lwt_real_render;
    qreal dialog_lwt_default_lwt;
    bool dialog_selection_mode_pickfirst;
    bool dialog_selection_mode_pickadd;
    bool dialog_selection_mode_pickdrag;
    QRgb dialog_selection_coolgrip_color;
    QRgb dialog_selection_hotgrip_color;
    quint8 dialog_selection_grip_size;
    quint8 dialog_selection_pickbox_size;

private slots:
    void comboBoxLanguageCurrentIndexChanged(const QString&);
    void comboBoxIconThemeCurrentIndexChanged(const QString&);
    void comboBoxIconSizeCurrentIndexChanged(int);
    void checkBoxGeneralMdiBGUseLogoStateChanged(int);
    void chooseGeneralMdiBackgroundLogo();
    void checkBoxGeneralMdiBGUseTextureStateChanged(int);
    void chooseGeneralMdiBackgroundTexture();
    void checkBoxGeneralMdiBGUseColorStateChanged(int);
    void chooseGeneralMdiBackgroundColor();
    void currentGeneralMdiBackgroundColorChanged(const QColor&);
    void checkBoxTipOfTheDayStateChanged(int);
    void checkBoxUseOpenGLStateChanged(int);
    void checkBoxRenderHintAAStateChanged(int);
    void checkBoxRenderHintTextAAStateChanged(int);
    void checkBoxRenderHintSmoothPixStateChanged(int);
    void checkBoxRenderHintHighAAStateChanged(int);
    void checkBoxRenderHintNonCosmeticStateChanged(int);
    void checkBoxShowScrollBarsStateChanged(int);
    void comboBoxScrollBarWidgetCurrentIndexChanged(int);
    void spinBoxZoomScaleInValueChanged(double);
    void spinBoxZoomScaleOutValueChanged(double);
    void checkBoxDisableBGStateChanged(int);
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
    void checkBoxPromptSaveHistoryStateChanged(int);
    void checkBoxPromptSaveHistoryAsHtmlStateChanged(int);
    void checkBoxCustomFilterStateChanged(int);
    void buttonCustomFilterSelectAllClicked();
    void buttonCustomFilterClearAllClicked();
    void spinBoxRecentMaxFilesValueChanged(int);
    void spinBoxTrimDstNumJumpsValueChanged(int);
    void checkBoxGridShowOnLoadStateChanged(int);
    void checkBoxGridShowOriginStateChanged(int);
    void checkBoxGridColorMatchCrossHairStateChanged(int);
    void chooseGridColor();
    void currentGridColorChanged(const QColor&);
    void checkBoxGridLoadFromFileStateChanged(int);
    void comboBoxGridTypeCurrentIndexChanged(const QString&);
    void checkBoxGridCenterOnOriginStateChanged(int);
    void spinBoxGridCenterXValueChanged(double);
    void spinBoxGridCenterYValueChanged(double);
    void spinBoxGridSizeXValueChanged(double);
    void spinBoxGridSizeYValueChanged(double);
    void spinBoxGridSpacingXValueChanged(double);
    void spinBoxGridSpacingYValueChanged(double);
    void spinBoxGridSizeRadiusValueChanged(double);
    void spinBoxGridSpacingRadiusValueChanged(double);
    void spinBoxGridSpacingAngleValueChanged(double);
    void checkBoxRulerShowOnLoadStateChanged(int);
    void comboBoxRulerMetricCurrentIndexChanged(int);
    void chooseRulerColor();
    void currentRulerColorChanged(const QColor&);
    void spinBoxRulerPixelSizeValueChanged(double);
    void checkBoxQSnapEndPointStateChanged(int);
    void checkBoxQSnapMidPointStateChanged(int);
    void checkBoxQSnapCenterStateChanged(int);
    void checkBoxQSnapNodeStateChanged(int);
    void checkBoxQSnapQuadrantStateChanged(int);
    void checkBoxQSnapIntersectionStateChanged(int);
    void checkBoxQSnapExtensionStateChanged(int);
    void checkBoxQSnapInsertionStateChanged(int);
    void checkBoxQSnapPerpendicularStateChanged(int);
    void checkBoxQSnapTangentStateChanged(int);
    void checkBoxQSnapNearestStateChanged(int);
    void checkBoxQSnapApparentStateChanged(int);
    void checkBoxQSnapParallelStateChanged(int);
    void buttonQSnapSelectAllClicked();
    void buttonQSnapClearAllClicked();
    void comboBoxQSnapLocatorColorCurrentIndexChanged(int);
    void sliderQSnapLocatorSizeValueChanged(int);
    void sliderQSnapApertureSizeValueChanged(int);
    void checkBoxLwtShowLwtStateChanged(int);
    void checkBoxLwtRealRenderStateChanged(int);
    void checkBoxSelectionModePickFirstStateChanged(int);
    void checkBoxSelectionModePickAddStateChanged(int);
    void checkBoxSelectionModePickDragStateChanged(int);
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

class StatusBar : public QStatusBar
{
    Q_OBJECT

public:
    StatusBar(MainWindow* mw, QWidget* parent = 0);

    StatusBarButton* statusBarSnapButton;
    StatusBarButton* statusBarGridButton;
    StatusBarButton* statusBarRulerButton;
    StatusBarButton* statusBarOrthoButton;
    StatusBarButton* statusBarPolarButton;
    StatusBarButton* statusBarQSnapButton;
    StatusBarButton* statusBarQTrackButton;
    StatusBarButton* statusBarLwtButton;
    QLabel* statusBarMouseCoord;

    void setMouseCoord(qreal x, qreal y);
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
    quint8 alpha;

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

    virtual QSize              sizeHint() const;
    QString getCurrentFile()   { return curFile; }
    QString getShortCurrentFile();
    View*                      getView() { return gview; }
    QGraphicsScene*            getScene() { return gscene; }
    QString getCurrentLayer() { return curLayer; }
    QRgb getCurrentColor() { return curColor; }
    QString getCurrentLineType() { return curLineType; }
    QString getCurrentLineWeight() { return curLineWeight; }
    void setCurrentLayer(const QString& layer) { curLayer = layer; }
    void setCurrentColor(const QRgb& color) { curColor = color; }
    void setCurrentLineType(const QString& lineType) { curLineType = lineType; }
    void setCurrentLineWeight(const QString& lineWeight) { curLineWeight = lineWeight; }
    void designDetails();
    bool loadFile(const QString &fileName);
    bool saveFile(const QString &fileName);
signals:
    void sendCloseMdiWin(MdiWindow*);

public slots:
    void closeEvent(QCloseEvent* e);
    void onWindowActivated();

    void currentLayerChanged(const QString& layer);
    void currentColorChanged(const QRgb& color);
    void currentLinetypeChanged(const QString& type);
    void currentLineweightChanged(const QString& weight);

    void updateColorLinetypeLineweight();
    void deletePressed();
    void escapePressed();

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

protected:

private:
    QMdiArea*                  mdiArea;
    QGraphicsScene*            gscene;
    View*                      gview;

    bool fileWasLoaded;

    QString promptHistory;
    QList<QString> promptInputList;
    int promptInputNum;

    QPrinter                   printer;

    QString curFile;
    void setCurrentFile(const QString& fileName);
    QString fileExtension(const QString& fileName);

    int myIndex;

    QString curLayer;
    QRgb curColor;
    QString curLineType;
    QString curLineWeight;

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

class EmbDetailsDialog : public QDialog
{
    Q_OBJECT

public:
    EmbDetailsDialog(QGraphicsScene* theScene, QWidget *parent = 0);
    ~EmbDetailsDialog();

private:
    QWidget*          mainWidget;

    void getInfo();
    QWidget*          createMainWidget();
    QWidget*          createHistogram();

    QDialogButtonBox* buttonBox;

    quint32 stitchesTotal;
    quint32 stitchesReal;
    quint32 stitchesJump;
    quint32 stitchesTrim;
    quint32 colorTotal;
    quint32 colorChanges;

    QRectF boundingRect;
};

// On Mac, if the user drops a file on the app's Dock icon, or uses Open As, then this is how the app actually opens the file.
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

extern QHash<QString, QString>* aliasHash;

class CmdPromptInput : public QLineEdit
{
    Q_OBJECT

public:
    CmdPromptInput(QWidget* parent = 0);
    ~CmdPromptInput();

    QString curText;
    QString defaultPrefix;
    QString prefix;

    QString lastCmd;
    QString curCmd;
    bool cmdActive;

    bool rapidFireEnabled;
    bool isBlinking;

protected:
    void contextMenuEvent(QContextMenuEvent *event);
    bool eventFilter(QObject *obj, QEvent *event);

signals:
    void appendHistory(const QString& txt, int prefixLength);

    //These connect to the CmdPrompt signals
    void startCommand(const QString& cmd);
    void runCommand(const QString& cmd, const QString& cmdtxt);
    void deletePressed();
    void tabPressed();
    void escapePressed();
    void upPressed();
    void downPressed();
    void F1Pressed();
    void F2Pressed();
    void F3Pressed();
    void F4Pressed();
    void F5Pressed();
    void F6Pressed();
    void F7Pressed();
    void F8Pressed();
    void F9Pressed();
    void F10Pressed();
    void F11Pressed();
    void F12Pressed();
    void cutPressed();
    void copyPressed();
    void pastePressed();
    void selectAllPressed();
    void undoPressed();
    void redoPressed();

    void shiftPressed();
    void shiftReleased();

    void showSettings();

    void stopBlinking();

public slots:
    void addCommand(const QString& alias, const QString& cmd);
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
private:

    void changeFormatting(const QList<QTextLayout::FormatRange>& formats);
    void clearFormatting();
    void applyFormatting();
};

//==========================================================================

class CmdPromptHistory : public QTextBrowser
{
    Q_OBJECT

public:
    CmdPromptHistory(QWidget* parent = 0);
    ~CmdPromptHistory();

protected:
    void contextMenuEvent(QContextMenuEvent* event);

public slots:
    void appendHistory(const QString& txt, int prefixLength);
    void startResizeHistory(int y);
    void stopResizeHistory(int y);
    void resizeHistory(int y);

signals:
    void historyAppended(const QString& txt);

private:
    int tmpHeight;
    QString applyFormatting(const QString& txt, int prefixLength);
};

//==========================================================================

class CmdPromptSplitter : public QSplitter
{
    Q_OBJECT

public:
    CmdPromptSplitter(QWidget* parent = 0);
    ~CmdPromptSplitter();

protected:
    QSplitterHandle* createHandle();

signals:
    void pressResizeHistory(int y);
    void releaseResizeHistory(int y);
    void moveResizeHistory(int y);
};

//==========================================================================

class CmdPromptHandle : public QSplitterHandle
{
    Q_OBJECT

public:
    CmdPromptHandle(Qt::Orientation orientation, QSplitter* parent);
    ~CmdPromptHandle();

protected:
    void mousePressEvent(QMouseEvent* e);
    void mouseReleaseEvent(QMouseEvent* e);
    void mouseMoveEvent(QMouseEvent* e);

signals:
    void handlePressed(int y);
    void handleReleased(int y);
    void handleMoved(int y);

private:
    int pressY;
    int releaseY;
    int moveY;
};

//==========================================================================

class CmdPrompt : public QWidget
{
    Q_OBJECT

public:
    CmdPrompt(QWidget* parent = 0);
    ~CmdPrompt();

    CmdPromptInput*    promptInput;

protected:

public slots:
    QString getHistory() { return promptHistory->toHtml(); }
    QString getPrefix() { return promptInput->prefix; }
    QString getCurrentText() { return promptInput->curText; }
    void setCurrentText(const QString& txt) { promptInput->curText = promptInput->prefix + txt; promptInput->setText(promptInput->curText); }
    void setHistory(const QString& txt) { promptHistory->setHtml(txt); promptHistory->moveCursor(QTextCursor::End, QTextCursor::MoveAnchor); }
    void setPrefix(const QString& txt);
    void appendHistory(const QString& txt);
    void startResizingTheHistory(int y) { promptHistory->startResizeHistory(y); }
    void stopResizingTheHistory(int y) { promptHistory->stopResizeHistory(y); }
    void resizeTheHistory(int y) { promptHistory->resizeHistory(y); }
    void addCommand(const QString& alias, const QString& cmd) { promptInput->addCommand(alias, cmd); }
    void endCommand() { promptInput->endCommand(); }
    bool isCommandActive() { return promptInput->cmdActive; }
    QString activeCommand() { return promptInput->curCmd; }
    QString lastCommand() { return promptInput->lastCmd; }
    void processInput() { promptInput->processInput(); }
    void enableRapidFire() { promptInput->rapidFireEnabled = true; }
    void disableRapidFire() { promptInput->rapidFireEnabled = false; }
    bool isRapidFireEnabled() { return promptInput->rapidFireEnabled; }

    void alert(const QString& txt);

    void startBlinking();
    void stopBlinking();
    void blink();

    void setPromptTextColor(const QColor&);
    void setPromptBackgroundColor(const QColor&);
    void setPromptFontFamily(const QString&);
    void setPromptFontStyle(const QString&);
    void setPromptFontSize(int);

    void floatingChanged(bool);

    void saveHistory(const QString& fileName, bool html);

private slots:

signals:
    void appendTheHistory(const QString& txt, int prefixLength);

    //For connecting outside of command prompt
    void startCommand(const QString& cmd);
    void runCommand(const QString& cmd, const QString& cmdtxt);
    void deletePressed();
    void tabPressed();
    void escapePressed();
    void upPressed();
    void downPressed();
    void F1Pressed();
    void F2Pressed();
    void F3Pressed();
    void F4Pressed();
    void F5Pressed();
    void F6Pressed();
    void F7Pressed();
    void F8Pressed();
    void F9Pressed();
    void F10Pressed();
    void F11Pressed();
    void F12Pressed();
    void cutPressed();
    void copyPressed();
    void pastePressed();
    void selectAllPressed();
    void undoPressed();
    void redoPressed();

    void shiftPressed();
    void shiftReleased();

    void showSettings();

    void historyAppended(const QString& txt);

private:
    CmdPromptHistory*  promptHistory;
    QVBoxLayout*       promptVBoxLayout;
    QFrame*            promptDivider;

    CmdPromptSplitter* promptSplitter;

    QHash<QString, QString>*  styleHash;
    void updateStyle();
    QTimer* blinkTimer;
    bool blinkState;
};

class MainWindow: public QMainWindow
{
    Q_OBJECT

public:
    MainWindow();
    ~MainWindow();

    MdiArea*                        getMdiArea();
    MainWindow*                     getApplication();
    MdiWindow*                      activeMdiWindow();

    void setUndoCleanIcon(bool opened);

    virtual void updateMenuToolbarStatusbar();

    MdiArea*        mdiArea;
    CmdPrompt*      prompt;
    PropertyEditor* dockPropEdit;
    UndoEditor*     dockUndoEdit;
    StatusBar*      statusbar;

    QList<QGraphicsItem*> cutCopyObjectList;

    bool getSettingsGeneralMdiBGUseLogo()          { return settings_general_mdi_bg_use_logo;        }
    bool getSettingsGeneralMdiBGUseTexture()       { return settings_general_mdi_bg_use_texture;     }
    bool getSettingsGeneralMdiBGUseColor()         { return settings_general_mdi_bg_use_color;       }
    QString getSettingsGeneralMdiBGLogo()             { return settings_general_mdi_bg_logo;            }
    QString getSettingsGeneralMdiBGTexture()          { return settings_general_mdi_bg_texture;         }
    QRgb getSettingsGeneralMdiBGColor()            { return settings_general_mdi_bg_color;           }
    bool getSettingsGeneralTipOfTheDay()           { return settings_general_tip_of_the_day;         }
    int getSettingsGeneralCurrentTip()            { return settings_general_current_tip;            }
    bool getSettingsGeneralSystemHelpBrowser()     { return settings_general_system_help_browser;    }
    bool getSettingsGeneralCheckForUpdates()       { return settings_general_check_for_updates;      }
    bool getSettingsDisplayUseOpenGL()             { return settings_display_use_opengl;             }
    bool getSettingsDisplayRenderHintAA()          { return settings_display_renderhint_aa;          }
    bool getSettingsDisplayRenderHintTextAA()      { return settings_display_renderhint_text_aa;     }
    bool getSettingsDisplayRenderHintSmoothPix()   { return settings_display_renderhint_smooth_pix;  }
    bool getSettingsDisplayRenderHintHighAA()      { return settings_display_renderhint_high_aa;     }
    bool getSettingsDisplayRenderHintNonCosmetic() { return settings_display_renderhint_noncosmetic; }
    bool getSettingsDisplayShowScrollBars()        { return settings_display_show_scrollbars;        }
    int getSettingsDisplayScrollBarWidgetNum()    { return settings_display_scrollbar_widget_num;   }
    QRgb getSettingsDisplayCrossHairColor()        { return settings_display_crosshair_color;        }
    QRgb getSettingsDisplayBGColor()               { return settings_display_bg_color;               }
    QRgb getSettingsDisplaySelectBoxLeftColor()    { return settings_display_selectbox_left_color;   }
    QRgb getSettingsDisplaySelectBoxLeftFill()     { return settings_display_selectbox_left_fill;    }
    QRgb getSettingsDisplaySelectBoxRightColor()   { return settings_display_selectbox_right_color;  }
    QRgb getSettingsDisplaySelectBoxRightFill()    { return settings_display_selectbox_right_fill;   }
    quint8 getSettingsDisplaySelectBoxAlpha()        { return settings_display_selectbox_alpha;        }
    qreal getSettingsDisplayZoomScaleIn()           { return settings_display_zoomscale_in;           }
    qreal getSettingsDisplayZoomScaleOut()          { return settings_display_zoomscale_out;          }
    quint8 getSettingsDisplayCrossHairPercent()      { return settings_display_crosshair_percent;      }
    QString getSettingsDisplayUnits()                 { return settings_display_units;                  }
    QRgb getSettingsPromptTextColor()              { return settings_prompt_text_color;              }
    QRgb getSettingsPromptBGColor()                { return settings_prompt_bg_color;                }
    QString getSettingsPromptFontFamily()             { return settings_prompt_font_family;             }
    QString getSettingsPromptFontStyle()              { return settings_prompt_font_style;              }
    quint8 getSettingsPromptFontSize()               { return settings_prompt_font_size;               }
    bool getSettingsPromptSaveHistory()            { return settings_prompt_save_history;            }
    bool getSettingsPromptSaveHistoryAsHtml()      { return settings_prompt_save_history_as_html;    }
    QString getSettingsPromptSaveHistoryFilename()    { return settings_prompt_save_history_filename;   }
    QString getSettingsCustomFilter()                 { return settings_opensave_custom_filter;         }
    QString getSettingsOpenFormat()                   { return settings_opensave_open_format;           }
    bool getSettingsOpenThumbnail()                { return settings_opensave_open_thumbnail;        }
    QString getSettingsSaveFormat()                   { return settings_opensave_save_format;           }
    bool getSettingsSaveThumbnail()                { return settings_opensave_save_thumbnail;        }
    quint8 getSettingsRecentMaxFiles()               { return settings_opensave_recent_max_files;      }
    quint8 getSettingsOpenSaveTrimDstNumJumps()      { return settings_opensave_trim_dst_num_jumps;    }
    QString getSettingsPrintingDefaultDevice()        { return settings_printing_default_device;        }
    bool getSettingsPrintingUseLastDevice()        { return settings_printing_use_last_device;       }
    bool getSettingsPrintingDisableBG()            { return settings_printing_disable_bg;            }
    bool getSettingsGridShowOnLoad()               { return settings_grid_show_on_load;              }
    bool getSettingsGridShowOrigin()               { return settings_grid_show_origin;               }
    bool getSettingsGridColorMatchCrossHair()      { return settings_grid_color_match_crosshair;     }
    QRgb getSettingsGridColor()                    { return settings_grid_color;                     }
    bool getSettingsGridLoadFromFile()             { return settings_grid_load_from_file;            }
    QString getSettingsGridType()                     { return settings_grid_type;                      }
    bool getSettingsGridCenterOnOrigin()           { return settings_grid_center_on_origin;          }
    qreal getSettingsGridCenterX()                  { return settings_grid_center_x;                  }
    qreal getSettingsGridCenterY()                  { return settings_grid_center_y;                  }
    qreal getSettingsGridSizeX()                    { return settings_grid_size_x;                    }
    qreal getSettingsGridSizeY()                    { return settings_grid_size_y;                    }
    qreal getSettingsGridSpacingX()                 { return settings_grid_spacing_x;                 }
    qreal getSettingsGridSpacingY()                 { return settings_grid_spacing_y;                 }
    qreal getSettingsGridSizeRadius()               { return settings_grid_size_radius;               }
    qreal getSettingsGridSpacingRadius()            { return settings_grid_spacing_radius;            }
    qreal getSettingsGridSpacingAngle()             { return settings_grid_spacing_angle;             }
    bool getSettingsRulerShowOnLoad()              { return settings_ruler_show_on_load;             }
    bool getSettingsRulerMetric()                  { return settings_ruler_metric;                   }
    QRgb getSettingsRulerColor()                   { return settings_ruler_color;                    }
    quint8 getSettingsRulerPixelSize()               { return settings_ruler_pixel_size;               }
    bool getSettingsQSnapEnabled()                 { return settings_qsnap_enabled;                  }
    QRgb getSettingsQSnapLocatorColor()            { return settings_qsnap_locator_color;            }
    quint8 getSettingsQSnapLocatorSize()             { return settings_qsnap_locator_size;             }
    quint8 getSettingsQSnapApertureSize()            { return settings_qsnap_aperture_size;            }
    bool getSettingsQSnapEndPoint()                { return settings_qsnap_endpoint;                 }
    bool getSettingsQSnapMidPoint()                { return settings_qsnap_midpoint;                 }
    bool getSettingsQSnapCenter()                  { return settings_qsnap_center;                   }
    bool getSettingsQSnapNode()                    { return settings_qsnap_node;                     }
    bool getSettingsQSnapQuadrant()                { return settings_qsnap_quadrant;                 }
    bool getSettingsQSnapIntersection()            { return settings_qsnap_intersection;             }
    bool getSettingsQSnapExtension()               { return settings_qsnap_extension;                }
    bool getSettingsQSnapInsertion()               { return settings_qsnap_insertion;                }
    bool getSettingsQSnapPerpendicular()           { return settings_qsnap_perpendicular;            }
    bool getSettingsQSnapTangent()                 { return settings_qsnap_tangent;                  }
    bool getSettingsQSnapNearest()                 { return settings_qsnap_nearest;                  }
    bool getSettingsQSnapApparent()                { return settings_qsnap_apparent;                 }
    bool getSettingsQSnapParallel()                { return settings_qsnap_parallel;                 }
    bool getSettingsLwtShowLwt()                   { return settings_lwt_show_lwt;                   }
    bool getSettingsLwtRealRender()                { return settings_lwt_real_render;                }
    qreal getSettingsLwtDefaultLwt()                { return settings_lwt_default_lwt;                }
    bool getSettingsSelectionModePickFirst()       { return settings_selection_mode_pickfirst;       }
    bool getSettingsSelectionModePickAdd()         { return settings_selection_mode_pickadd;         }
    bool getSettingsSelectionModePickDrag()        { return settings_selection_mode_pickdrag;        }
    QRgb getSettingsSelectionCoolGripColor()       { return settings_selection_coolgrip_color;       }
    QRgb getSettingsSelectionHotGripColor()        { return settings_selection_hotgrip_color;        }
    quint8 getSettingsSelectionGripSize()            { return settings_selection_grip_size;            }
    quint8 getSettingsSelectionPickBoxSize()         { return settings_selection_pickbox_size;         }
    QString getSettingsTextFont()                     { return settings_text_font;                      }
    qreal getSettingsTextSize()                     { return settings_text_size;                      }
    qreal getSettingsTextAngle()                    { return settings_text_angle;                     }
    bool getSettingsTextStyleBold()                { return settings_text_style_bold;                }
    bool getSettingsTextStyleItalic()              { return settings_text_style_italic;              }
    bool getSettingsTextStyleUnderline()           { return settings_text_style_underline;           }
    bool getSettingsTextStyleStrikeOut()           { return settings_text_style_strikeout;           }
    bool getSettingsTextStyleOverline()            { return settings_text_style_overline;            }

    void setSettingsGeneralLanguage(const QString& newValue)           { settings_general_language               = newValue; }
    void setSettingsGeneralIconTheme(const QString& newValue)          { settings_general_icon_theme             = newValue; }
    void setSettingsGeneralIconSize(int newValue)                      { settings_general_icon_size              = newValue; }
    void setSettingsGeneralMdiBGUseLogo(bool newValue)                 { settings_general_mdi_bg_use_logo        = newValue; }
    void setSettingsGeneralMdiBGUseTexture(bool newValue)              { settings_general_mdi_bg_use_texture     = newValue; }
    void setSettingsGeneralMdiBGUseColor(bool newValue)                { settings_general_mdi_bg_use_color       = newValue; }
    void setSettingsGeneralMdiBGLogo(const QString& newValue)          { settings_general_mdi_bg_logo            = newValue; }
    void setSettingsGeneralMdiBGTexture(const QString& newValue)       { settings_general_mdi_bg_texture         = newValue; }
    void setSettingsGeneralMdiBGColor(QRgb newValue)                   { settings_general_mdi_bg_color           = newValue; }
    void setSettingsGeneralTipOfTheDay(bool newValue)                  { settings_general_tip_of_the_day         = newValue; }
    void setSettingsGeneralCurrentTip(int newValue)                    { settings_general_current_tip            = newValue; }
    void setSettingsGeneralSystemHelpBrowser(bool newValue)            { settings_general_system_help_browser    = newValue; }
    void setSettingsGeneralCheckForUpdates(bool newValue)              { settings_general_check_for_updates      = newValue; }
    void setSettingsDisplayUseOpenGL(bool newValue)                    { settings_display_use_opengl             = newValue; }
    void setSettingsDisplayRenderHintAA(bool newValue)                 { settings_display_renderhint_aa          = newValue; }
    void setSettingsDisplayRenderHintTextAA(bool newValue)             { settings_display_renderhint_text_aa     = newValue; }
    void setSettingsDisplayRenderHintSmoothPix(bool newValue)          { settings_display_renderhint_smooth_pix  = newValue; }
    void setSettingsDisplayRenderHintHighAA(bool newValue)             { settings_display_renderhint_high_aa     = newValue; }
    void setSettingsDisplayRenderHintNonCosmetic(bool newValue)        { settings_display_renderhint_noncosmetic = newValue; }
    void setSettingsDisplayShowScrollBars(bool newValue)               { settings_display_show_scrollbars        = newValue; }
    void setSettingsDisplayScrollBarWidgetNum(int newValue)            { settings_display_scrollbar_widget_num   = newValue; }
    void setSettingsDisplayCrossHairColor(QRgb newValue)               { settings_display_crosshair_color        = newValue; }
    void setSettingsDisplayBGColor(QRgb newValue)                      { settings_display_bg_color               = newValue; }
    void setSettingsDisplaySelectBoxLeftColor(QRgb newValue)           { settings_display_selectbox_left_color   = newValue; }
    void setSettingsDisplaySelectBoxLeftFill(QRgb newValue)            { settings_display_selectbox_left_fill    = newValue; }
    void setSettingsDisplaySelectBoxRightColor(QRgb newValue)          { settings_display_selectbox_right_color  = newValue; }
    void setSettingsDisplaySelectBoxRightFill(QRgb newValue)           { settings_display_selectbox_right_fill   = newValue; }
    void setSettingsDisplaySelectBoxAlpha(quint8 newValue)             { settings_display_selectbox_alpha        = newValue; }
    void setSettingsDisplayZoomScaleIn(qreal newValue)                 { settings_display_zoomscale_in           = newValue; }
    void setSettingsDisplayZoomScaleOut(qreal newValue)                { settings_display_zoomscale_out          = newValue; }
    void setSettingsDisplayCrossHairPercent(quint8 newValue)           { settings_display_crosshair_percent      = newValue; }
    void setSettingsDisplayUnits(const QString& newValue)              { settings_display_units                  = newValue; }
    void setSettingsPromptTextColor(QRgb newValue)                     { settings_prompt_text_color              = newValue; }
    void setSettingsPromptBGColor(QRgb newValue)                       { settings_prompt_bg_color                = newValue; }
    void setSettingsPromptFontFamily(const QString& newValue)          { settings_prompt_font_family             = newValue; }
    void setSettingsPromptFontStyle(const QString& newValue)           { settings_prompt_font_style              = newValue; }
    void setSettingsPromptFontSize(quint8 newValue)                    { settings_prompt_font_size               = newValue; }
    void setSettingsPromptSaveHistory(bool newValue)                   { settings_prompt_save_history            = newValue; }
    void setSettingsPromptSaveHistoryAsHtml(bool newValue)             { settings_prompt_save_history_as_html    = newValue; }
    void setSettingsPromptSaveHistoryFilename(const QString& newValue) { settings_prompt_save_history_filename   = newValue; }
    void setSettingsCustomFilter(const QString& newValue)              { settings_opensave_custom_filter         = newValue; }
    void setSettingsOpenFormat(const QString& newValue)                { settings_opensave_open_format           = newValue; }
    void setSettingsOpenThumbnail(bool newValue)                       { settings_opensave_open_thumbnail        = newValue; }
    void setSettingsSaveFormat(const QString& newValue)                { settings_opensave_save_format           = newValue; }
    void setSettingsSaveThumbnail(bool newValue)                       { settings_opensave_save_thumbnail        = newValue; }
    void setSettingsRecentMaxFiles(quint8 newValue)                    { settings_opensave_recent_max_files      = newValue; }
    void setSettingsOpenSaveTrimDstNumJumps(quint8 newValue)           { settings_opensave_trim_dst_num_jumps    = newValue; }
    void setSettingsPrintingDefaultDevice(const QString& newValue)     { settings_printing_default_device        = newValue; }
    void setSettingsPrintingUseLastDevice(bool newValue)               { settings_printing_use_last_device       = newValue; }
    void setSettingsPrintingDisableBG(bool newValue)                   { settings_printing_disable_bg            = newValue; }
    void setSettingsGridShowOnLoad(bool newValue)                      { settings_grid_show_on_load              = newValue; }
    void setSettingsGridShowOrigin(bool newValue)                      { settings_grid_show_origin               = newValue; }
    void setSettingsGridColorMatchCrossHair(bool newValue)             { settings_grid_color_match_crosshair     = newValue; }
    void setSettingsGridColor(QRgb newValue)                           { settings_grid_color                     = newValue; }
    void setSettingsGridLoadFromFile(bool newValue)                    { settings_grid_load_from_file            = newValue; }
    void setSettingsGridType(const QString& newValue)                  { settings_grid_type                      = newValue; }
    void setSettingsGridCenterOnOrigin(bool newValue)                  { settings_grid_center_on_origin          = newValue; }
    void setSettingsGridCenterX(qreal newValue)                        { settings_grid_center_x                  = newValue; }
    void setSettingsGridCenterY(qreal newValue)                        { settings_grid_center_y                  = newValue; }
    void setSettingsGridSizeX(qreal newValue)                          { settings_grid_size_x                    = newValue; }
    void setSettingsGridSizeY(qreal newValue)                          { settings_grid_size_y                    = newValue; }
    void setSettingsGridSpacingX(qreal newValue)                       { settings_grid_spacing_x                 = newValue; }
    void setSettingsGridSpacingY(qreal newValue)                       { settings_grid_spacing_y                 = newValue; }
    void setSettingsGridSizeRadius(qreal newValue)                     { settings_grid_size_radius               = newValue; }
    void setSettingsGridSpacingRadius(qreal newValue)                  { settings_grid_spacing_radius            = newValue; }
    void setSettingsGridSpacingAngle(qreal newValue)                   { settings_grid_spacing_angle             = newValue; }
    void setSettingsRulerShowOnLoad(bool newValue)                     { settings_ruler_show_on_load             = newValue; }
    void setSettingsRulerMetric(bool newValue)                         { settings_ruler_metric                   = newValue; }
    void setSettingsRulerColor(QRgb newValue)                          { settings_ruler_color                    = newValue; }
    void setSettingsRulerPixelSize(quint8 newValue)                    { settings_ruler_pixel_size               = newValue; }
    void setSettingsQSnapEnabled(bool newValue)                        { settings_qsnap_enabled                  = newValue; }
    void setSettingsQSnapLocatorColor(QRgb newValue)                   { settings_qsnap_locator_color            = newValue; }
    void setSettingsQSnapLocatorSize(quint8 newValue)                  { settings_qsnap_locator_size             = newValue; }
    void setSettingsQSnapApertureSize(quint8 newValue)                 { settings_qsnap_aperture_size            = newValue; }
    void setSettingsQSnapEndPoint(bool newValue)                       { settings_qsnap_endpoint = newValue; }
    void setSettingsQSnapMidPoint(bool newValue)                       { settings_qsnap_midpoint = newValue; }
    void setSettingsQSnapCenter(bool newValue)                         { settings_qsnap_center                   = newValue; }
    void setSettingsQSnapNode(bool newValue)                           { settings_qsnap_node                     = newValue; }
    void setSettingsQSnapQuadrant(bool newValue)                       { settings_qsnap_quadrant                 = newValue; }
    void setSettingsQSnapIntersection(bool newValue)                   { settings_qsnap_intersection             = newValue; }
    void setSettingsQSnapExtension(bool newValue)                      { settings_qsnap_extension                = newValue; }
    void setSettingsQSnapInsertion(bool newValue)                      { settings_qsnap_insertion                = newValue; }
    void setSettingsQSnapPerpendicular(bool newValue)                  { settings_qsnap_perpendicular            = newValue; }
    void setSettingsQSnapTangent(bool newValue)                        { settings_qsnap_tangent                  = newValue; }
    void setSettingsQSnapNearest(bool newValue)                        { settings_qsnap_nearest                  = newValue; }
    void setSettingsQSnapApparent(bool newValue)                       { settings_qsnap_apparent                 = newValue; }
    void setSettingsQSnapParallel(bool newValue)                       { settings_qsnap_parallel                 = newValue; }
    void setSettingsLwtShowLwt(bool newValue)                          { settings_lwt_show_lwt                   = newValue; }
    void setSettingsLwtRealRender(bool newValue)                       { settings_lwt_real_render                = newValue; }
    void setSettingsLwtDefaultLwt(qreal newValue)                      { settings_lwt_default_lwt                = newValue; }
    void setSettingsSelectionModePickFirst(bool newValue)              { settings_selection_mode_pickfirst       = newValue; }
    void setSettingsSelectionModePickAdd(bool newValue)                { settings_selection_mode_pickadd         = newValue; }
    void setSettingsSelectionModePickDrag(bool newValue)               { settings_selection_mode_pickdrag        = newValue; }
    void setSettingsSelectionCoolGripColor(QRgb newValue)              { settings_selection_coolgrip_color       = newValue; }
    void setSettingsSelectionHotGripColor(QRgb newValue)               { settings_selection_hotgrip_color        = newValue; }
    void setSettingsSelectionGripSize(quint8 newValue)                 { settings_selection_grip_size            = newValue; }
    void setSettingsSelectionPickBoxSize(quint8 newValue)              { settings_selection_pickbox_size         = newValue; }
    void setSettingsTextFont(const QString& newValue)                  { settings_text_font                      = newValue; }
    void setSettingsTextSize(qreal newValue)                           { settings_text_size                      = newValue; }
    void setSettingsTextAngle(qreal newValue)                          { settings_text_angle                     = newValue; }
    void setSettingsTextStyleBold(bool newValue)                       { settings_text_style_bold                = newValue; }
    void setSettingsTextStyleItalic(bool newValue)                     { settings_text_style_italic              = newValue; }
    void setSettingsTextStyleUnderline(bool newValue)                  { settings_text_style_underline           = newValue; }
    void setSettingsTextStyleStrikeOut(bool newValue)                  { settings_text_style_strikeout           = newValue; }
    void setSettingsTextStyleOverline(bool newValue)                   { settings_text_style_overline            = newValue; }

    QHash<int, QAction*>            actionHash;
    QHash<QString, Command>         command_map;
    QHash<QString, QToolBar*>       toolbarHash;
    QHash<QString, QMenu*>          menuHash;

    QString formatFilterOpen;
    QString formatFilterSave;

    bool isCommandActive() { return prompt->isCommandActive(); }
    QString activeCommand() { return prompt->activeCommand(); }

    QString platformString();

public slots:

    void enablePromptRapidFire();
    void disablePromptRapidFire();

    void enableMoveRapidFire();
    void disableMoveRapidFire();

    void onCloseWindow();
    virtual void onCloseMdiWin(MdiWindow*);

    void recentMenuAboutToShow();

    void onWindowActivated(QMdiSubWindow* w);
    void windowMenuAboutToShow();
    void windowMenuActivated( bool checked/*int id*/ );
    QAction*                        getAction(int actionEnum);

    void updateAllViewScrollBars(bool val);
    void updateAllViewCrossHairColors(QRgb color);
    void updateAllViewBackgroundColors(QRgb color);
    void updateAllViewSelectBoxColors(QRgb colorL, QRgb fillL, QRgb colorR, QRgb fillR, int alpha);
    void updateAllViewGridColors(QRgb color);
    void updateAllViewRulerColors(QRgb color);

    void updatePickAddMode(bool val);
    void pickAddModeToggled();

    void settingsPrompt();

    void settingsDialog(const QString& showTab = QString());
    void readSettings();
    void writeSettings();

    static bool validFileFormat(const QString &fileName);

protected:
    virtual void resizeEvent(QResizeEvent*);
    void closeEvent(QCloseEvent *event);
    QAction*                        getFileSeparator();
    void loadFormats();

public:

    QString settings_general_language;
    QString settings_general_icon_theme;
    int settings_general_icon_size;
    bool settings_general_mdi_bg_use_logo;
    bool settings_general_mdi_bg_use_texture;
    bool settings_general_mdi_bg_use_color;
    QString settings_general_mdi_bg_logo;
    QString settings_general_mdi_bg_texture;
    QRgb settings_general_mdi_bg_color;
    bool settings_general_tip_of_the_day;
    quint16 settings_general_current_tip;
    bool settings_general_system_help_browser;
    bool settings_general_check_for_updates;
    bool settings_display_use_opengl;
    bool settings_display_renderhint_aa;
    bool settings_display_renderhint_text_aa;
    bool settings_display_renderhint_smooth_pix;
    bool settings_display_renderhint_high_aa;
    bool settings_display_renderhint_noncosmetic;
    bool settings_display_show_scrollbars;
    int settings_display_scrollbar_widget_num;
    QRgb settings_display_crosshair_color;
    QRgb settings_display_bg_color;
    QRgb settings_display_selectbox_left_color;
    QRgb settings_display_selectbox_left_fill;
    QRgb settings_display_selectbox_right_color;
    QRgb settings_display_selectbox_right_fill;
    quint8 settings_display_selectbox_alpha;
    qreal settings_display_zoomscale_in;
    qreal settings_display_zoomscale_out;
    quint8 settings_display_crosshair_percent;
    QString settings_display_units;
    QRgb settings_prompt_text_color;
    QRgb settings_prompt_bg_color;
    QString settings_prompt_font_family;
    QString settings_prompt_font_style;
    quint8 settings_prompt_font_size;
    bool settings_prompt_save_history;
    bool settings_prompt_save_history_as_html;
    QString settings_prompt_save_history_filename;
    QString settings_opensave_custom_filter;
    QString settings_opensave_open_format;
    bool settings_opensave_open_thumbnail;
    QString settings_opensave_save_format;
    bool settings_opensave_save_thumbnail;
    quint8 settings_opensave_recent_max_files;
    QStringList settings_opensave_recent_list_of_files;
    QString settings_opensave_recent_directory;
    quint8 settings_opensave_trim_dst_num_jumps;
    QString settings_printing_default_device;
    bool settings_printing_use_last_device;
    bool settings_printing_disable_bg;
    bool settings_grid_show_on_load;
    bool settings_grid_show_origin;
    bool settings_grid_color_match_crosshair;
    QRgb settings_grid_color;
    bool settings_grid_load_from_file;
    QString settings_grid_type;
    bool settings_grid_center_on_origin;
    qreal settings_grid_center_x;
    qreal settings_grid_center_y;
    qreal settings_grid_size_x;
    qreal settings_grid_size_y;
    qreal settings_grid_spacing_x;
    qreal settings_grid_spacing_y;
    qreal settings_grid_size_radius;
    qreal settings_grid_spacing_radius;
    qreal settings_grid_spacing_angle;
    bool settings_ruler_show_on_load;
    bool settings_ruler_metric;
    QRgb settings_ruler_color;
    quint8 settings_ruler_pixel_size;
    bool settings_qsnap_enabled;
    QRgb settings_qsnap_locator_color;
    quint8 settings_qsnap_locator_size;
    quint8 settings_qsnap_aperture_size;
    bool settings_qsnap_endpoint;
    bool settings_qsnap_midpoint;
    bool settings_qsnap_center;
    bool settings_qsnap_node;
    bool settings_qsnap_quadrant;
    bool settings_qsnap_intersection;
    bool settings_qsnap_extension;
    bool settings_qsnap_insertion;
    bool settings_qsnap_perpendicular;
    bool settings_qsnap_tangent;
    bool settings_qsnap_nearest;
    bool settings_qsnap_apparent;
    bool settings_qsnap_parallel;
    bool settings_lwt_show_lwt;
    bool settings_lwt_real_render;
    qreal settings_lwt_default_lwt;
    bool settings_selection_mode_pickfirst;
    bool settings_selection_mode_pickadd;
    bool settings_selection_mode_pickdrag;
    QRgb settings_selection_coolgrip_color;
    QRgb settings_selection_hotgrip_color;
    quint8 settings_selection_grip_size;
    quint8 settings_selection_pickbox_size;
    QString settings_text_font;
    qreal settings_text_size;
    qreal settings_text_angle;
    bool settings_text_style_bold;
    bool settings_text_style_italic;
    bool settings_text_style_underline;
    bool settings_text_style_overline;
    bool settings_text_style_strikeout;

    bool shiftKeyPressedState;

    QByteArray                      layoutState;

    int numOfDocs;
    int docIndex;

private:
    QList<MdiWindow*>               listMdiWin;
    QMdiSubWindow*                  findMdiWindow(const QString &fileName);
    QString openFilesPath;

    QAction*                        myFileSeparator;

    QWizard*    wizardTipOfTheDay;
    QLabel*     labelTipOfTheDay;
    QCheckBox*  checkBoxTipOfTheDay;
    QStringList listTipOfTheDay;

    void createAllActions();
    QAction*                        createAction(Command command);

    /* Toolbars */
    void createAllToolbars();
    void createFileToolbar();
    void createEditToolbar();
    void createViewToolbar();
    void createZoomToolbar();
    void createPanToolbar();
    void createIconToolbar();
    void createHelpToolbar();
    void createLayerToolbar();
    void createPropertiesToolbar();
    void createTextToolbar();
    void createPromptToolbar();

    QToolBar* toolbarFile;
    QToolBar* toolbarEdit;
    QToolBar* toolbarView;
    QToolBar* toolbarZoom;
    QToolBar* toolbarPan;
    QToolBar* toolbarIcon;
    QToolBar* toolbarHelp;
    QToolBar* toolbarLayer;
    QToolBar* toolbarText;
    QToolBar* toolbarProperties;
    QToolBar* toolbarPrompt;

    /* Selectors */
    QComboBox*     layerSelector;
    QComboBox*     colorSelector;
    QComboBox*     linetypeSelector;
    QComboBox*     lineweightSelector;
    QFontComboBox* textFontSelector;
    QComboBox*     textSizeSelector;
    //====================================================
    //Menus
    //====================================================
    void createAllMenus();
    void createFileMenu();
    void createEditMenu();
    void createViewMenu();
    void createSettingsMenu();
    void createWindowMenu();
    void createHelpMenu();

    QMenu* fileMenu;
    QMenu* editMenu;
    QMenu* viewMenu;
    QMenu* settingsMenu;
    QMenu* windowMenu;
    QMenu* helpMenu;
    //====================================================
    //SubMenus
    //====================================================
    QMenu* recentMenu;
    QMenu* zoomMenu;
    QMenu* panMenu;

private slots:
    void hideUnimplemented();

public slots:
    void stub_implement(QString txt);
    void stub_testing();

    void promptHistoryAppended(const QString& txt);
    void logPromptInput(const QString& txt);
    void promptInputPrevious();
    void promptInputNext();

    void runCommand();
    ScriptValue runCommandCore(const QString& cmd, ScriptEnv *context);
    void runCommandMain(const QString& cmd);
    void runCommandClick(const QString& cmd,  qreal x, qreal y);
    void runCommandMove(const QString& cmd,  qreal x, qreal y);
    void runCommandContext(const QString& cmd, const QString& str);
    void runCommandPrompt(const QString& cmd, const QString& str);

    void debug_message(const char *);

    void newFile();
    void openFile(bool recent = false, const QString& recentFile = "");
    void openFilesSelected(const QStringList&);
    void openrecentfile();
    void savefile();
    void saveasfile();
    void print();
    void designDetails();
    void exit();
    void quit();
    void checkForUpdates();

    // Help Menu
    void tipOfTheDay();
    void buttonTipOfTheDayClicked(int);
    void checkBoxTipOfTheDayStateChanged(int);
    void help();
    void changelog();
    void whatsThisContextHelp();

    void cut();
    void copy();
    void paste();
    void selectAll();

    void closeToolBar(QAction*);
    void floatingChangedToolBar(bool);

    void toggleGrid();
    void toggleRuler();
    void toggleLwt();

    // Icons
    void iconResize(int iconSize);

    //Selectors
    void layerSelectorIndexChanged(int index);
    void colorSelectorIndexChanged(int index);
    void linetypeSelectorIndexChanged(int index);
    void lineweightSelectorIndexChanged(int index);
    void textFontSelectorCurrentFontChanged(const QFont& font);
    void textSizeSelectorIndexChanged(int index);

    QString textFont();
    qreal textSize();
    qreal textAngle();
    bool textBold();
    bool textItalic();
    bool textUnderline();
    bool textStrikeOut();
    bool textOverline();

    void setTextFont(const QString& str);
    void setTextSize(qreal num);

    QString getCurrentLayer();
    QRgb getCurrentColor();
    QString getCurrentLineType();
    QString getCurrentLineWeight();

    // Standard Slots
    void undo();
    void redo();

    bool isShiftPressed();
    void setShiftPressed();
    void setShiftReleased();

    void deletePressed();
    void escapePressed();

    // Layer Toolbar
    void makeLayerActive();
    void layerManager();
    void layerPrevious();

    // Zoom Toolbar
    void zoomPrevious();
    void zoomWindow();
    void zoomDynamic();
    void zoomScale();
    void zoomCenter();
    void zoomOut();
    void zoomSelected();
    void zoomAll();
    void zoomExtents();

    // Pan SubMenu
    void panrealtime();
    void panpoint();
    void panLeft();
    void panRight();
    void panDown();

    void dayVision();
    void nightVision();

    void doNothing();

public:
    //Natives
    void nativeAlert(const QString& txt);
    void nativeBlinkPrompt();
    void nativeSetPromptPrefix(const QString& txt);
    void nativeAppendPromptHistory(const QString& txt);
    void nativeEnablePromptRapidFire();
    void nativeDisablePromptRapidFire();
    void nativeInitCommand();
    void nativeEndCommand();

    void nativeEnableMoveRapidFire();
    void nativeDisableMoveRapidFire();

    void messageBox(const QString& type, const QString& title, const QString& text);

    void nativeRedo();

    void nativePrintArea(qreal x, qreal y, qreal w, qreal h);

    void nativeSetBackgroundColor(quint8 r, quint8 g, quint8 b);
    void nativeSetCrossHairColor(quint8 r, quint8 g, quint8 b);
    void nativeSetGridColor(quint8 r, quint8 g, quint8 b);

    void nativeVulcanize();
    void nativeClearRubber();
    bool nativeAllowRubber();
    void nativeSpareRubber(qint64 id);
    //TODO: void nativeSetRubberFilter(qint64 id); //TODO: This is so more than 1 rubber object can exist at one time without updating all rubber objects at once
    void nativeSetRubberMode(int mode);
    void nativeSetRubberPoint(const QString& key, qreal x, qreal y);
    void nativeSetRubberText(const QString& key, const QString& txt);

    void nativeAddTextMulti(const QString& str, qreal x, qreal y, qreal rot, bool fill, int rubberMode);
    void nativeAddTextSingle(const QString& str, qreal x, qreal y, qreal rot, bool fill, int rubberMode);

    void nativeAddInfiniteLine(qreal x1, qreal y1, qreal x2, qreal y2, qreal rot);
    void nativeAddRay(qreal x1, qreal y1, qreal x2, qreal y2, qreal rot);
    void nativeAddLine(qreal x1, qreal y1, qreal x2, qreal y2, qreal rot, int rubberMode);
    void nativeAddTriangle(qreal x1, qreal y1, qreal x2, qreal y2, qreal x3, qreal y3, qreal rot, bool fill);
    void nativeAddRectangle(qreal x, qreal y, qreal w, qreal h, qreal rot, bool fill, int rubberMode);
    void nativeAddRoundedRectangle(qreal x, qreal y, qreal w, qreal h, qreal rad, qreal rot, bool fill);
    void nativeAddArc(qreal startX, qreal startY, qreal midX, qreal midY, qreal endX, qreal endY, int rubberMode);
    void nativeAddCircle(qreal centerX, qreal centerY, qreal radius, bool fill, int rubberMode);
    void nativeAddSlot(qreal centerX, qreal centerY, qreal diameter, qreal length, qreal rot, bool fill, int rubberMode);
    void nativeAddEllipse(qreal centerX, qreal centerY, qreal width, qreal height, qreal rot, bool fill, int rubberMode);
    void nativeAddPoint(qreal x, qreal y);
    void nativeAddRegularPolygon(qreal centerX, qreal centerY, quint16 sides, quint8 mode, qreal rad, qreal rot, bool fill);
    void nativeAddPolygon(qreal startX, qreal startY, const QPainterPath& p, int rubberMode);
    void nativeAddPolyline(qreal startX, qreal startY, const QPainterPath& p, int rubberMode);
    void nativeAddPath(qreal startX, qreal startY, const QPainterPath& p, int rubberMode);
    void nativeAddHorizontalDimension(qreal x1, qreal y1, qreal x2, qreal y2, qreal legHeight);
    void nativeAddVerticalDimension(qreal x1, qreal y1, qreal x2, qreal y2, qreal legHeight);
    void nativeAddImage(const QString& img, qreal x, qreal y, qreal w, qreal h, qreal rot);

    void nativeAddDimLeader(qreal x1, qreal y1, qreal x2, qreal y2, qreal rot, int rubberMode);

    void nativeSetCursorShape(const QString& str);
    qreal nativeCalculateAngle(qreal x1, qreal y1, qreal x2, qreal y2);
    qreal nativeCalculateDistance(qreal x1, qreal y1, qreal x2, qreal y2);
    qreal nativePerpendicularDistance(qreal px, qreal py, qreal x1, qreal y1, qreal x2, qreal y2);

    int nativeNumSelected();
    void nativeSelectAll();
    void nativeAddToSelection(const QPainterPath path, Qt::ItemSelectionMode mode);
    void nativeClearSelection();
    void nativeDeleteSelected();
    void nativeCutSelected(qreal x, qreal y);
    void nativeCopySelected(qreal x, qreal y);
    void nativePasteSelected(qreal x, qreal y);
    void nativeMoveSelected(qreal dx, qreal dy);
    void nativeScaleSelected(qreal x, qreal y, qreal factor);
    void nativeRotateSelected(qreal x, qreal y, qreal rot);
    void nativeMirrorSelected(qreal x1, qreal y1, qreal x2, qreal y2);

    qreal nativeQSnapX();
    qreal nativeQSnapY();
};

void debug_message(QString s);
View* activeView();
QGraphicsScene* activeScene();
QUndoStack* activeUndoStack();

ScriptEnv *create_script_env();
void free_script_env(ScriptEnv *);

ScriptValue script_bool(bool b);
ScriptValue script_int(int i);
ScriptValue script_real(double r);
ScriptValue script_string(const char *s);
ScriptValue do_nothing(ScriptEnv *context);
ScriptValue stub_implement(const char *function);
ScriptValue command_prompt(const char *line);

ScriptEnv *add_string_argument(ScriptEnv *context, const char *s);
ScriptEnv *add_real_argument(ScriptEnv *context, double r);
ScriptEnv *add_int_argument(ScriptEnv *context, int i);

void add_string_variable(ScriptEnv *context, const char *label, const char *s);
void add_int_variable(ScriptEnv *context, const char *label, int i);
void add_real_variable(ScriptEnv *context, const char *label, double i);

const char *script_get_string(ScriptEnv *context, const char *label);
int script_get_int(ScriptEnv *context, const char *label);
double script_get_real(ScriptEnv *context, const char *label);

int script_set_string(ScriptEnv *context, const char *label, const char *s);
int script_set_int(ScriptEnv *context, const char *label, int i);
int script_set_real(ScriptEnv *context, const char *label, double r);

QString translate(QString msg);

int argument_checks(ScriptEnv *context, QString function, const char *args);

extern Command command_data[];

extern ScriptValue script_null;
extern ScriptValue script_true;
extern ScriptValue script_false;

#endif
