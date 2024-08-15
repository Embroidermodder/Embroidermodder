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
#include <QWidget>
#include <QtGlobal>
#include <QtGui>
#include <QtPrintSupport>

/* #include <QOpenGLWidget> */

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
class View;

extern MainWindow *_main;
extern QHash<std::string, std::string>* aliasHash;
extern QHash<int, QAction*> actionHash;
extern QHash<QString, QToolBar*> toolbarHash;
extern QHash<QString, QMenu*> menuHash;
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
extern QStringList listTipOfTheDay;
extern QComboBox* layerSelector;
extern QComboBox* colorSelector;
extern QComboBox* linetypeSelector;
extern QComboBox* lineweightSelector;
extern QFontComboBox* textFontSelector;
extern QComboBox* textSizeSelector;
extern bool shiftKeyPressedState;
extern QList<QGraphicsItem*> cutCopyObjectList;

/* . */
typedef struct ViewData_ {
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
} ViewData;

typedef struct UndoData_ {
    int type;
    Object* object;
    View* gview;
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
View* activeView();
QGraphicsScene* activeScene();
QUndoStack* activeUndoStack();
QString platformString();

QToolButton *create_statusbarbutton(QString buttonText, MainWindow* mw);
QIcon create_icon(QString icon);

void nativeAddPolygon(double startX, double startY, const QPainterPath& p, int rubberMode);
void nativeAddPolyline(double startX, double startY, const QPainterPath& p, int rubberMode);
void nativeAddPath(double startX, double startY, const QPainterPath& p, int rubberMode);

void nativeAlert(std::string txt);
void nativeAppendPromptHistory(std::string txt);
void messageBox(std::string type, std::string title, std::string text);

void nativeAddTextMulti(std::string str, double x, double y, double rot, bool fill, int rubberMode);
void nativeAddTextSingle(std::string str, double x, double y, double rot, bool fill, int rubberMode);

void nativeAddImage(std::string img, double x, double y, double w, double h, double rot);

void nativeAddToSelection(const QPainterPath path, Qt::ItemSelectionMode mode);

QAction *get_action_by_icon(const char *icon);
void add_to_menu(QMenu *menu, const char *menu_data[]);

EmbVector to_emb_vector(QPointF p);
QPointF to_qpointf(EmbVector v);
QPointF scale_and_rotate(QPointF v, double angle, double scale);
QPointF find_mouse_snap_point(QList<QPointF> snap_points, const QPointF& mouse_point);

void set_visibility(QObject *senderObj, const char *key, bool visibility);
void set_enabled(QObject *senderObj, const char *key, bool visibility);
void set_visibility_group(QObject *senderObj, const char *key, bool visibility);
void set_enabled_group(QObject *senderObj, const char *key, bool visibility);
QIcon create_swatch(int32_t color);
QPushButton *choose_color_button(QGroupBox* groupbox, IntSetting* color_setting);

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

class View : public QGraphicsView
{
    Q_OBJECT

public:
    View(MainWindow* mw, QGraphicsScene* theScene, QWidget* parent);
    ~View();

    ViewData data;

    bool allowZoomIn();
    bool allowZoomOut();

    void recalculateLimits();
    void zoomToPoint(const QPoint& mousePoint, int zoomDir);
    void centerAt(const QPointF& centerPoint);
    QPointF center() { return mapToScene(rect().center()); }

    QUndoStack* getUndoStack() { return data.undoStack; }
    void addObject(Object* obj);
    void deleteObject(Object* obj);
    void vulcanizeObject(Object* obj);

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
    void moveSelected(double dx, double dy);
    void cut();
    void copy();
    void paste();
    void repeatAction();
    void moveAction();
    void scaleAction();
    void scaleSelected(double x, double y, double factor);
    void rotateAction();
    void rotateSelected(double x, double y, double rot);
    void mirrorSelected(double x1, double y1, double x2, double y2);
    int numSelected();

    void deletePressed();
    void escapePressed();

    void cornerButtonClicked();

    void showScrollBars(bool val);
    void setCornerButton();
    void setCrossHairColor(QRgb color);
    void setCrossHairSize(uint8_t percent);
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

    void previewOn(int clone, int mode, double x, double y, double data);
    void previewOff();

    void enableMoveRapidFire();
    void disableMoveRapidFire();

    bool allowRubber();
    void addToRubberRoom(QGraphicsItem* item);
    void vulcanizeRubberRoom();
    void clearRubberRoom();
    void spareRubber(int64_t id);
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
    void createGridRect();
    void createGridPolar();
    void createGridIso();
    void createOrigin();

    void loadRulerSettings();

    bool willUnderflowInt32(int64_t a, int64_t b);
    bool willOverflowInt32(int64_t a, int64_t b);
    int roundToMultiple(bool roundUp, int numToRound, int multiple);
    QPainterPath createRulerTextPath(float x, float y, QString str, float height);

    QList<QGraphicsItem*> createObjectList(QList<QGraphicsItem*> list);

    void copySelected();

    void startGripping(Object* obj);
    void stopGripping(bool accept = false);

    void updateMouseCoords(int x, int y);

    void panStart(const QPoint& point);

    void alignScenePointWithViewPoint(const QPointF& scenePoint, const QPoint& viewPoint);
};

class UndoableCommand : public QUndoCommand
{
public:
    UndoableCommand(int type_, const QString& text, Object* obj, View* v, QUndoCommand* parent = 0);
    UndoableCommand(int type_, double deltaX, double deltaY, const QString& text, Object* obj, View* v, QUndoCommand* parent = 0);
    UndoableCommand(int type_, double pivotPointX, double pivotPointY, double rotAngle, const QString& text, Object* obj, View* v, QUndoCommand* parent = 0);
    UndoableCommand(int type_, const QString& type, View* v, QUndoCommand* parent = 0);
    UndoableCommand(int type_, const QPointF beforePoint, const QPointF afterPoint, const QString& text, Object* obj, View* v, QUndoCommand* parent = 0);
    UndoableCommand(int type_, double x1, double y1, double x2, double y2, const QString& text, Object* obj, View* v, QUndoCommand* parent = 0);

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

class Object : public QGraphicsPathItem
{
public:
    EmbGeometry *geometry;
    ObjectData data;

    void init_geometry(int type_, QRgb rgb, Qt::PenStyle lineType);

    Object(EmbArc arc, QRgb rgb, QGraphicsItem* parent = 0);
    Object(EmbCircle circle, QRgb rgb, QGraphicsItem *item = 0);
    Object(EmbEllipse ellipse, QRgb rgb, QGraphicsItem *item = 0);
    Object(EmbLine line, QRgb rgb, QGraphicsItem* parent = 0);
    Object(double centerX, double centerY, double width, double height, QRgb rgb, QGraphicsItem* parent = 0);
    Object(EmbRect rect, QRgb rgb, QGraphicsItem* parent = 0);
    Object(EmbPoint point, QRgb rgb, QGraphicsItem* parent = 0);
    Object(EmbPath path, int type_, const QPainterPath& p, QRgb rgb, QGraphicsItem* parent = 0);
    Object(EmbPolygon polygon, int type_, QRgb rgb, QGraphicsItem* parent = 0);
    Object(const QString& str, double x, double y, QRgb rgb, QGraphicsItem* parent = 0);

    Object(Object* obj, QGraphicsItem* parent = 0);
    ~Object();

    /* QColor objectColor() const { return data.objPen.color(); } */
    QRgb objectColorRGB() const { return data.objPen.color().rgb(); }
    Qt::PenStyle objectLineType() const { return data.objPen.style(); }
    double  objectLineWeight() const { return data.lwtPen.widthF(); }
    QPainterPath objectPath() const { return path(); }
    int objectRubberMode() const { return data.objRubberMode; }
    QPointF objectRubberPoint(const QString& key) const;
    QString objectRubberText(const QString& key) const;

    double objectRadiusMajor() const { return EMB_MAX(rect().width(), rect().height())/2.0*scale(); }
    double objectRadiusMinor() const { return EMB_MIN(rect().width(), rect().height())/2.0*scale(); }
    double objectDiameterMajor() const { return EMB_MAX(rect().width(), rect().height())*scale(); }
    double objectDiameterMinor() const { return EMB_MIN(rect().width(), rect().height())*scale(); }

    QPointF objectPos() const { return scenePos(); }
    double objectX() const { return scenePos().x(); }
    double objectY() const { return scenePos().y(); }

    QPointF objectCenter() const { return scenePos(); }
    double objectCenterX() const { return scenePos().x(); }
    double objectCenterY() const { return scenePos().y(); }

    double objectRadius() const { return rect().width()/2.0*scale(); }
    double objectDiameter() const { return rect().width()*scale(); }
    double objectArea() const;
    double objectCircumference() const { return embConstantPi*objectDiameter(); }

    QPointF objectQuadrant0();
    QPointF objectQuadrant90();
    QPointF objectQuadrant180();
    QPointF objectQuadrant270();

    void updateRubber(void);

    QPointF objectEndPoint1() const;
    QPointF objectEndPoint2() const;
    QPointF objectStartPoint() const;
    QPointF objectMidPoint() const;
    QPointF objectEndPoint() const;
    QPointF objectDelta() const { return objectEndPoint2() - objectEndPoint1(); }

    QPointF objectTopLeft() const;
    QPointF objectTopRight() const;
    QPointF objectBottomLeft() const;
    QPointF objectBottomRight() const;
    double objectWidth() const { return rect().width()*scale(); }
    double objectHeight() const { return rect().height()*scale(); }

    void updateRubber(QPainter* painter = 0);
    void updateLeader();
    void updatePath();
    void updatePath(const QPainterPath& p);
    void updateArcRect(double radius);

    double objectAngle() const;
    double objectLength() const { return line().length()*scale(); }

    void setObjectEndPoint1(const QPointF& endPt1);
    void setObjectEndPoint1(double x1, double y1);
    void setObjectEndPoint2(const QPointF& endPt2);
    void setObjectEndPoint2(double x2, double y2);
    void setObjectX1(double x) { setObjectEndPoint1(x, objectEndPoint1().y()); }
    void setObjectY1(double y) { setObjectEndPoint1(objectEndPoint1().x(), y); }
    void setObjectX2(double x) { setObjectEndPoint2(x, objectEndPoint2().y()); }
    void setObjectY2(double y) { setObjectEndPoint2(objectEndPoint2().x(), y); }

    QRectF rect() const { return path().boundingRect(); }
    void setRect(const QRectF& r) { QPainterPath p; p.addRect(r); setPath(p); }
    void setRect(double x, double y, double w, double h) { QPainterPath p; p.addRect(x,y,w,h); setPath(p); }
    QLineF line() const { return data.objLine; }
    void setLine(const QLineF& li) { QPainterPath p; p.moveTo(li.p1()); p.lineTo(li.p2()); setPath(p); data.objLine = li; }
    void setLine(double x1, double y1, double x2, double y2) { QPainterPath p; p.moveTo(x1,y1); p.lineTo(x2,y2); setPath(p); data.objLine.setLine(x1,y1,x2,y2); }

    void setObjectPos(const QPointF& point) { setPos(point.x(), point.y()); }
    void setObjectPos(double x, double y) { setPos(x, y); }
    void setObjectX(double x) { setObjectPos(x, objectY()); }
    void setObjectY(double y) { setObjectPos(objectX(), y); }

    void setObjectRect(double x1, double y1, double x2, double y2);
    virtual void vulcanize();
    virtual QList<QPointF> allGripPoints();
    virtual QPointF mouseSnapPoint(const QPointF& mousePoint);
    virtual void gripEdit(const QPointF& before, const QPointF& after);
    virtual QRectF boundingRect() const;
    virtual QPainterPath shape() const { return path(); }

    void setObjectColor(const QColor& color);
    void setObjectColorRGB(QRgb rgb);
    void setObjectLineType(Qt::PenStyle lineType);
    void setObjectLineWeight(double lineWeight);
    void setObjectPath(const QPainterPath& p) { setPath(p); }
    void setObjectRubberMode(int mode) { data.objRubberMode = mode; }
    void setObjectRubberPoint(const QString& key, const QPointF& point) { data.objRubberPoints.insert(key, point); }
    void setObjectRubberText(const QString& key, const QString& txt) { data.objRubberTexts.insert(key, txt); }

    void drawRubberLine(const QLineF& rubLine, QPainter* painter = 0, const char* colorFromScene = 0);
    QPen lineWeightPen() const { return data.lwtPen; }
    void realRender(QPainter* painter, const QPainterPath& renderPath);
    double objectStartAngle() const;
    double objectEndAngle() const;
    double objectArcLength() const;
    double objectChord() const;
    double objectIncludedAngle() const;
    bool objectClockwise() const;

    void setObjectCenter(EmbVector point);
    void setObjectCenter(const QPointF& center);
    void setObjectCenterX(double centerX);
    void setObjectCenterY(double centerY);
    void setObjectRadius(double radius);
    void setObjectRadiusMajor(double radius);
    void setObjectRadiusMinor(double radius);
    void setObjectDiameterMajor(double diameter);
    void setObjectDiameterMinor(double diameter);
    void setObjectStartAngle(double angle);
    void setObjectEndAngle(double angle);
    void setObjectStartPoint(EmbVector point);
    void setObjectMidPoint(EmbVector point);
    void setObjectEndPoint(EmbVector point);

    void calculateArcData(void);

    void setObjectDiameter(double diameter);
    void setObjectArea(double area);
    void setObjectCircumference(double circumference);

    void setObjectSize(double width, double height);

    QPainterPath objectCopyPath() const;
    QPainterPath objectSavePath() const;
    QList<QPainterPath> objectSavePathList() const { return subPathList(); }
    QList<QPainterPath> subPathList() const;

    /* TODO: make paths similar to polylines. Review and implement any missing functions/members. */
    int findIndex(const QPointF& point);

    QStringList objectTextJustifyList() const;

    void setObjectText(const QString& str);
    void setObjectTextFont(const QString& font);
    void setObjectTextJustify(const QString& justify);
    void setObjectTextSize(double size);
    void setObjectTextStyle(bool bold, bool italic, bool under, bool strike, bool over);
    void setObjectTextBold(bool val);
    void setObjectTextItalic(bool val);
    void setObjectTextUnderline(bool val);
    void setObjectTextStrikeOut(bool val);
    void setObjectTextOverline(bool val);
    void setObjectTextBackward(bool val);
    void setObjectTextUpsideDown(bool val);
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
    /* Helper functions */
    void updateLineEditStrIfVaries(QLineEdit* lineEdit, const QString& str);
    void updateLineEditNumIfVaries(QLineEdit* lineEdit, double num, bool useAnglePrecision);
    void updateFontComboBoxStrIfVaries(QFontComboBox* fontComboBox, const QString& str);
    void updateComboBoxStrIfVaries(QComboBox* comboBox, const QString& str, const QStringList& strList);
    void updateComboBoxBoolIfVaries(QComboBox* comboBox, bool val, bool yesOrNoText);

    /* Selection */
    QComboBox* createComboBoxSelected();
    QToolButton* createToolButtonQSelect();
    QToolButton* createToolButtonPickAdd();
};

class Settings_Dialog : public QDialog
{
    Q_OBJECT

public:
    Settings_Dialog(MainWindow* mw, const QString& showTab = "", QWidget *parent = 0);
    ~Settings_Dialog();

    QCheckBox* create_checkbox(QGroupBox* groupbox, QString label, BoolSetting *setting,
        QString icon="");
    QDoubleSpinBox* create_spinbox(
        QGroupBox* groupbox,
        QString label,
        RealSetting *setting,
        double single_step,
        double lower_bound,
        double upper_bound);
    QSpinBox* create_int_spinbox(
        QGroupBox* groupbox,
        QString label,
        IntSetting *setting,
        int single_step,
        int lower_bound,
        int upper_bound);

    void preview_update(void);

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
    void comboBoxLanguageCurrentIndexChanged(const QString&);
    void comboBoxIconThemeCurrentIndexChanged(const QString&);
    void comboBoxIconSizeCurrentIndexChanged(int);
    void chooseGeneralMdiBackgroundLogo();
    void chooseGeneralMdiBackgroundTexture();
    void chooseGeneralMdiBackgroundColor();
    void currentGeneralMdiBackgroundColorChanged(const QColor&);
    void checkBoxShowScrollBarsStateChanged(int);
    void comboBoxScrollBarWidgetCurrentIndexChanged(int);
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
    void checkBoxGridColorMatchCrossHairStateChanged(int);
    void chooseGridColor();
    void currentGridColorChanged(const QColor&);
    void checkBoxGridLoadFromFileStateChanged(int);
    void comboBoxGridTypeCurrentIndexChanged(const QString&);
    void checkBoxGridCenterOnOriginStateChanged(int);
    void checkBoxRulerShowOnLoadStateChanged(int);
    void comboBoxRulerMetricCurrentIndexChanged(int);
    void chooseRulerColor();
    void currentRulerColorChanged(const QColor&);
    void spinBoxRulerPixelSizeValueChanged(double);
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
    View* gview;

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

class CmdPromptHistory : public QTextBrowser
{
    Q_OBJECT

public:
    CmdPromptHistory(QWidget* parent = 0);
    ~CmdPromptHistory() {}

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

class CmdPromptSplitter : public QSplitter
{
    Q_OBJECT

public:
    CmdPromptSplitter(QWidget* parent = 0);
    ~CmdPromptSplitter() {}

protected:
    QSplitterHandle* createHandle();

signals:
    void pressResizeHistory(int y);
    void releaseResizeHistory(int y);
    void moveResizeHistory(int y);
};

class CmdPromptHandle : public QSplitterHandle
{
    Q_OBJECT

public:
    CmdPromptHandle(Qt::Orientation orientation, QSplitter* parent);
    ~CmdPromptHandle() {}

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

class CmdPrompt : public QWidget
{
    Q_OBJECT

public:
    CmdPrompt(QWidget* parent = 0);
    ~CmdPrompt();

    CmdPromptInput*    promptInput;

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
    CmdPromptHistory* promptHistory;
    QVBoxLayout* promptVBoxLayout;
    QFrame* promptDivider;

    CmdPromptSplitter* promptSplitter;

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

    void setUndoCleanIcon(bool opened);

    virtual void updateMenuToolbarStatusbar();

    bool isCommandActive() { return prompt->isCommandActive(); }
    QString activeCommand() { return prompt->activeCommand(); }

public slots:
    void onCloseWindow();
    virtual void onCloseMdiWin(MdiWindow*);

    void recentMenuAboutToShow();

    void onWindowActivated(QMdiSubWindow* w);
    void windowMenuAboutToShow();
    void windowMenuActivated( bool checked/*int id*/ );

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
    QAction* getFileSeparator();
    void loadFormats();

private:
    QMdiSubWindow* findMdiWindow(const QString &fileName);

    void createAllActions();
    QAction* createAction(Command command);

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

    /* Menus */
    void createAllMenus();

private slots:
    void hideUnimplemented();

public slots:
    void stub_implement(QString txt);
    void stub_testing();

    void run_testing();

    void promptHistoryAppended(const QString& txt);
    void logPromptInput(const QString& txt);
    void promptInputPrevious();
    void promptInputNext();

    void runCommand();
    ScriptValue runCommandCore(const QString& cmd, ScriptEnv *context);
    void runCommandMain(const QString& cmd);
    void runCommandClick(const QString& cmd, double x, double y);
    void runCommandMove(const QString& cmd, double x, double y);
    void runCommandContext(const QString& cmd, const QString& str);
    void runCommandPrompt(const QString& cmd);

    void debug_message(const char *);

    void newFile();
    void openFile(bool recent = false, const QString& recentFile = "");
    void openFilesSelected(const QStringList&);
    void openrecentfile();
    void savefile();
    void saveasfile();
    void print();
    void exit();
    void checkForUpdates();

    void closeToolBar(QAction*);
    void floatingChangedToolBar(bool);

    /* Icons */
    void iconResize(int iconSize);

    /* Selectors */
    void layerSelectorIndexChanged(int index);
    void colorSelectorIndexChanged(int index);
    void linetypeSelectorIndexChanged(int index);
    void lineweightSelectorIndexChanged(int index);
    void textFontSelectorCurrentFontChanged(const QFont& font);
    void textSizeSelectorIndexChanged(int index);

    void setTextFont(const QString& str);
    void setTextSize(double num);

    QString getCurrentLayer();
    QRgb getCurrentColor();
    QString getCurrentLineType();
    QString getCurrentLineWeight();

    /* Standard Slots */
    void undo();
    void redo();

    bool isShiftPressed();
    void setShiftPressed();
    void setShiftReleased();

    void deletePressed();
    void escapePressed();

    /* Layer Toolbar */
    void makeLayerActive();
    void layerManager();
    void layerPrevious();
};

#endif

