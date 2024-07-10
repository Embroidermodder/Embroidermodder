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

#include <QTimer>

/* C++ */
#include <string>
#include <vector>
#include <unordered_map>

#include "core.h"

#define REAL(i)             context->argument[i].r
#define INT(i)              context->argument[i].i
#define QSTR(i)    QString(context->argument[i].s)
#define STR(i)              context->argument[i].s
#define BOOL(i)             context->argument[i].b

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
        const double zValue,
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

class View : public QGraphicsView
{
    Q_OBJECT

public:
    View(MainWindow* mw, QGraphicsScene* theScene, QWidget* parent);
    ~View();

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
    QHash<int64_t, QGraphicsItem*> hashDeletedObjects;

    QList<int64_t> spareRubberList;

    QColor gridColor;
    QPainterPath gridPath;
    void createGridRect();
    void createGridPolar();
    void createGridIso();
    QPainterPath originPath;
    void createOrigin();

    bool rulerMetric;
    QColor rulerColor;
    uint8_t rulerPixelSize;
    void loadRulerSettings();

    bool willUnderflowInt32(int64_t a, int64_t b);
    bool willOverflowInt32(int64_t a, int64_t b);
    int roundToMultiple(bool roundUp, int numToRound, int multiple);
    QPainterPath createRulerTextPath(float x, float y, QString str, float height);

    QList<QGraphicsItem*> previewObjectList;
    QGraphicsItemGroup* previewObjectItemGroup;
    QPointF previewPoint;
    double previewData;
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
    uint8_t qsnapLocatorSize;
    uint8_t qsnapApertureSize;
    QRgb gripColorCool;
    QRgb gripColorHot;
    uint8_t gripSize;
    uint8_t pickBoxSize;
    QRgb crosshairColor;
    uint32_t crosshairSize;

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
    View* gview;
};

class UndoableDeleteCommand : public QUndoCommand
{
public:
    UndoableDeleteCommand(const QString& text, BaseObject* obj, View* v, QUndoCommand* parent = 0);

    void undo();
    void redo();

private:
    BaseObject* object;
    View* gview;
};

class UndoableMoveCommand : public QUndoCommand
{
public:
    UndoableMoveCommand(double deltaX, double deltaY, const QString& text, BaseObject* obj, View* v, QUndoCommand* parent = 0);

    void undo();
    void redo();

private:
    BaseObject* object;
    View* gview;
    double dx;
    double dy;
};

class UndoableRotateCommand : public QUndoCommand
{
public:
    UndoableRotateCommand(double pivotPointX, double pivotPointY, double rotAngle, const QString& text, BaseObject* obj, View* v, QUndoCommand* parent = 0);

    void undo();
    void redo();

private:
    void rotate(double x, double y, double rot);

    BaseObject* object;
    View* gview;
    double pivotX;
    double pivotY;
    double angle;
};

class UndoableScaleCommand : public QUndoCommand
{
public:
    UndoableScaleCommand(double x, double y, double scaleFactor, const QString& text, BaseObject* obj, View* v, QUndoCommand* parent = 0);

    void undo();
    void redo();

private:
    BaseObject* object;
    View* gview;
    double dx;
    double dy;
    double factor;
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
    View* gview;
    QPointF     before;
    QPointF     after;
};


class UndoableMirrorCommand : public QUndoCommand
{
public:
    UndoableMirrorCommand(double x1, double y1, double x2, double y2, const QString& text, BaseObject* obj, View* v, QUndoCommand* parent = 0);

    void undo();
    void redo();

private:
    void mirror();

    BaseObject* object;
    View* gview;
    QLineF mirrorLine;

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

inline double radians(double degree) { return (degree * embConstantPi / 180.0); }
inline double degrees(double radian) { return (radian * 180.0 / embConstantPi); }

class BaseObject : public QGraphicsPathItem
{
public:
    EmbGeometry *geometry;

    QPen objPen;
    QPen lwtPen;
    QLineF objLine;
    int objRubberMode;
    QHash<QString, QPointF> objRubberPoints;
    QHash<QString, QString> objRubberTexts;
    int64_t objID;

    BaseObject(QGraphicsItem* parent = 0);
    virtual ~BaseObject();

    enum { Type = OBJ_TYPE_BASE };
    virtual int type() const { return Type; }

//    QColor objectColor() const { return objPen.color(); }
    QRgb objectColorRGB() const { return objPen.color().rgb(); }
    Qt::PenStyle objectLineType() const { return objPen.style(); }
    double  objectLineWeight() const { return lwtPen.widthF(); }
    QPainterPath objectPath() const { return path(); }
    int objectRubberMode() const { return objRubberMode; }
    QPointF objectRubberPoint(const QString& key) const;
    QString objectRubberText(const QString& key) const;

    QRectF rect() const { return path().boundingRect(); }
    void setRect(const QRectF& r) { QPainterPath p; p.addRect(r); setPath(p); }
    void setRect(double x, double y, double w, double h) { QPainterPath p; p.addRect(x,y,w,h); setPath(p); }
    QLineF line() const { return objLine; }
    void setLine(const QLineF& li) { QPainterPath p; p.moveTo(li.p1()); p.lineTo(li.p2()); setPath(p); objLine = li; }
    void setLine(double x1, double y1, double x2, double y2) { QPainterPath p; p.moveTo(x1,y1); p.lineTo(x2,y2); setPath(p); objLine.setLine(x1,y1,x2,y2); }

    void setObjectColor(const QColor& color);
    void setObjectColorRGB(QRgb rgb);
    void setObjectLineType(Qt::PenStyle lineType);
    void setObjectLineWeight(double lineWeight);
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
    void realRender(QPainter* painter, const QPainterPath& renderPath);
};

class ArcObject : public BaseObject
{
public:
    ArcObject(EmbArc arc, QRgb rgb, QGraphicsItem* parent = 0);
    ArcObject(ArcObject* obj, QGraphicsItem* parent = 0);
    ~ArcObject();

    enum { Type = OBJ_TYPE_ARC };
    virtual int type() const { return Type; }

    QPointF objectCenter() const { return scenePos(); }
    double objectRadius() const { return rect().width()/2.0*scale(); }
    double objectStartAngle() const;
    double objectEndAngle() const;
    QPointF objectStartPoint() const;
    QPointF objectMidPoint() const;
    QPointF objectEndPoint() const;
    double objectArea() const;
    double objectArcLength() const;
    double objectChord() const;
    double objectIncludedAngle() const;
    bool objectClockwise() const;

    void setObjectCenter(EmbVector point);
    void setObjectRadius(double radius);
    void setObjectStartAngle(double angle);
    void setObjectEndAngle(double angle);
    void setObjectStartPoint(EmbVector point);
    void setObjectMidPoint(EmbVector point);
    void setObjectEndPoint(EmbVector point);

    void updateRubber(QPainter* painter = 0);
    virtual void vulcanize();
    virtual QPointF mouseSnapPoint(const QPointF& mousePoint);
    virtual QList<QPointF> allGripPoints();
    virtual void gripEdit(const QPointF& before, const QPointF& after);
protected:
    void paint(QPainter*, const QStyleOptionGraphicsItem*, QWidget*);
private:
    void init(EmbArc arc, QRgb rgb, Qt::PenStyle lineType);
    void updatePath();

    void calculateArcData(EmbArc arc);
    void updateArcRect(double radius);
};

class CircleObject : public BaseObject
{
public:
    CircleObject(double centerX, double centerY, double radius, QRgb rgb, QGraphicsItem* parent = 0);
    CircleObject(CircleObject* obj, QGraphicsItem* parent = 0);
    ~CircleObject();

    enum { Type = OBJ_TYPE_CIRCLE };
    virtual int type() const { return Type; }

    QPainterPath objectSavePath() const;

    QPointF objectCenter() const { return scenePos(); }
    double objectRadius() const { return rect().width()/2.0*scale(); }
    double objectDiameter() const { return rect().width()*scale(); }
    double objectArea() const { return embConstantPi*objectRadius()*objectRadius(); }
    double objectCircumference() const { return embConstantPi*objectDiameter(); }
    QPointF objectQuadrant0() const { return objectCenter() + QPointF(objectRadius(), 0); }
    QPointF objectQuadrant90() const { return objectCenter() + QPointF(0,-objectRadius()); }
    QPointF objectQuadrant180() const { return objectCenter() + QPointF(-objectRadius(),0); }
    QPointF objectQuadrant270() const { return objectCenter() + QPointF(0, objectRadius()); }

    void setObjectCenter(EmbVector centerY);
    void setObjectCenterX(double centerX);
    void setObjectCenterY(double centerY);
    void setObjectRadius(double radius);
    void setObjectDiameter(double diameter);
    void setObjectArea(double area);
    void setObjectCircumference(double circumference);

    void updateRubber(QPainter* painter = 0);
    virtual void vulcanize();
    virtual QPointF mouseSnapPoint(const QPointF& mousePoint);
    virtual QList<QPointF> allGripPoints();
    virtual void gripEdit(const QPointF& before, const QPointF& after);
protected:
    void paint(QPainter*, const QStyleOptionGraphicsItem*, QWidget*);
private:
    void init(double centerX, double centerY, double radius, QRgb rgb, Qt::PenStyle lineType);
    void updatePath();
};


class DimLeaderObject : public BaseObject
{
public:
    DimLeaderObject(double x1, double y1, double x2, double y2, QRgb rgb, QGraphicsItem* parent = 0);
    DimLeaderObject(DimLeaderObject* obj, QGraphicsItem* parent = 0);
    ~DimLeaderObject();

    enum ArrowStyle {
        NoArrow, //NOTE: Allow this enum to evaluate false
        Open,
        Closed,
        Dot,
        Box,
        Tick
    };

    enum lineStyle {
        NoLine, //NOTE: Allow this enum to evaluate false
        Flared,
        Fletching
    };

    enum { Type = OBJ_TYPE_DIMLEADER };
    virtual int type() const { return Type; }

    QPointF objectEndPoint1() const;
    QPointF objectEndPoint2() const;
    QPointF objectMidPoint() const;
    double objectX1() const { return objectEndPoint1().x(); }
    double objectY1() const { return objectEndPoint1().y(); }
    double objectX2() const { return objectEndPoint2().x(); }
    double objectY2() const { return objectEndPoint2().y(); }
    double objectDeltaX() const { return (objectX2() - objectX1()); }
    double objectDeltaY() const { return (objectY2() - objectY1()); }
    double objectAngle() const;
    double objectLength() const { return line().length(); }

    void setObjectEndPoint1(const QPointF& endPt1);
    void setObjectEndPoint1(double x1, double y1);
    void setObjectEndPoint2(const QPointF& endPt2);
    void setObjectEndPoint2(double x2, double y2);
    void setObjectX1(double x) { setObjectEndPoint1(x, objectY1()); }
    void setObjectY1(double y) { setObjectEndPoint1(objectX1(), y); }
    void setObjectX2(double x) { setObjectEndPoint2(x, objectY2()); }
    void setObjectY2(double y) { setObjectEndPoint2(objectX2(), y); }

    void updateRubber(QPainter* painter = 0);
    virtual void vulcanize();
    virtual QPointF mouseSnapPoint(const QPointF& mousePoint);
    virtual QList<QPointF> allGripPoints();
    virtual void gripEdit(const QPointF& before, const QPointF& after);
protected:
    void paint(QPainter*, const QStyleOptionGraphicsItem*, QWidget*);
private:
    void init(double x1, double y1, double x2, double y2, QRgb rgb, Qt::PenStyle lineType);

    bool curved;
    bool filled;
    void updateLeader();
    QPainterPath lineStylePath;
    QPainterPath arrowStylePath;
    double arrowStyleAngle;
    double arrowStyleLength;
    double lineStyleAngle;
    double lineStyleLength;
};


class EllipseObject : public BaseObject
{
public:
    EllipseObject(double centerX, double centerY, double width, double height, QRgb rgb, QGraphicsItem* parent = 0);
    EllipseObject(EllipseObject* obj, QGraphicsItem* parent = 0);
    ~EllipseObject();

    enum { Type = OBJ_TYPE_ELLIPSE };
    virtual int type() const { return Type; }

    QPainterPath objectSavePath() const;

    QPointF objectCenter() const { return scenePos(); }
    double objectCenterX() const { return scenePos().x(); }
    double objectCenterY() const { return scenePos().y(); }
    double objectRadiusMajor() const { return qMax(rect().width(), rect().height())/2.0*scale(); }
    double objectRadiusMinor() const { return qMin(rect().width(), rect().height())/2.0*scale(); }
    double objectDiameterMajor() const { return qMax(rect().width(), rect().height())*scale(); }
    double objectDiameterMinor() const { return qMin(rect().width(), rect().height())*scale(); }
    double objectWidth() const { return rect().width()*scale(); }
    double objectHeight() const { return rect().height()*scale(); }
    QPointF objectQuadrant0() const;
    QPointF objectQuadrant90() const;
    QPointF objectQuadrant180() const;
    QPointF objectQuadrant270() const;

    void setObjectSize(double width, double height);
    void setObjectCenter(const QPointF& center);
    void setObjectCenter(double centerX, double centerY);
    void setObjectCenterX(double centerX);
    void setObjectCenterY(double centerY);
    void setObjectRadiusMajor(double radius);
    void setObjectRadiusMinor(double radius);
    void setObjectDiameterMajor(double diameter);
    void setObjectDiameterMinor(double diameter);

    void updateRubber(QPainter* painter = 0);
    virtual void vulcanize();
    virtual QPointF mouseSnapPoint(const QPointF& mousePoint);
    virtual QList<QPointF> allGripPoints();
    virtual void gripEdit(const QPointF& before, const QPointF& after);
protected:
    void paint(QPainter*, const QStyleOptionGraphicsItem*, QWidget*);
private:
    void init(double centerX, double centerY, double width, double height, QRgb rgb, Qt::PenStyle lineType);
    void updatePath();
};


class ImageObject : public BaseObject
{
public:
    ImageObject(double x, double y, double w, double h, QRgb rgb, QGraphicsItem* parent = 0);
    ImageObject(ImageObject* obj, QGraphicsItem* parent = 0);
    ~ImageObject();

    enum { Type = OBJ_TYPE_IMAGE };
    virtual int type() const { return Type; }

    QPointF objectTopLeft() const;
    QPointF objectTopRight() const;
    QPointF objectBottomLeft() const;
    QPointF objectBottomRight() const;
    double objectWidth() const { return rect().width()*scale(); }
    double objectHeight() const { return rect().height()*scale(); }
    double objectArea() const { return qAbs(objectWidth()*objectHeight()); }

    void setObjectRect(double x, double y, double w, double h);

    void updateRubber(QPainter* painter = 0);
    virtual void vulcanize();
    virtual QPointF mouseSnapPoint(const QPointF& mousePoint);
    virtual QList<QPointF> allGripPoints();
    virtual void gripEdit(const QPointF& before, const QPointF& after);
protected:
    void paint(QPainter*, const QStyleOptionGraphicsItem*, QWidget*);
private:
    void init(double x, double y, double w, double h, QRgb rgb, Qt::PenStyle lineType);
    void updatePath();
};


class LineObject : public BaseObject
{
public:
    LineObject(double x1, double y1, double x2, double y2, QRgb rgb, QGraphicsItem* parent = 0);
    LineObject(LineObject* obj, QGraphicsItem* parent = 0);
    ~LineObject();

    enum { Type = OBJ_TYPE_LINE };
    virtual int type() const { return Type; }

    QPainterPath objectSavePath() const;

    QPointF objectEndPoint1() const { return scenePos(); }
    QPointF objectEndPoint2() const;
    QPointF objectMidPoint() const;
    double objectX1() const { return objectEndPoint1().x(); }
    double objectY1() const { return objectEndPoint1().y(); }
    double objectX2() const { return objectEndPoint2().x(); }
    double objectY2() const { return objectEndPoint2().y(); }
    double objectDeltaX() const { return (objectX2() - objectX1()); }
    double objectDeltaY() const { return (objectY2() - objectY1()); }
    double objectAngle() const;
    double objectLength() const { return line().length()*scale(); }

    void setObjectEndPoint1(const QPointF& endPt1);
    void setObjectEndPoint1(double x1, double y1);
    void setObjectEndPoint2(const QPointF& endPt2);
    void setObjectEndPoint2(double x2, double y2);
    void setObjectX1(double x) { setObjectEndPoint1(x, objectY1()); }
    void setObjectY1(double y) { setObjectEndPoint1(objectX1(), y); }
    void setObjectX2(double x) { setObjectEndPoint2(x, objectY2()); }
    void setObjectY2(double y) { setObjectEndPoint2(objectX2(), y); }

    void updateRubber(QPainter* painter = 0);
    virtual void vulcanize();
    virtual QPointF mouseSnapPoint(const QPointF& mousePoint);
    virtual QList<QPointF> allGripPoints();
    virtual void gripEdit(const QPointF& before, const QPointF& after);
protected:
    void paint(QPainter*, const QStyleOptionGraphicsItem*, QWidget*);
private:
    void init(double x1, double y1, double x2, double y2, QRgb rgb, Qt::PenStyle lineType);
};


class PathObject : public BaseObject
{
public:
    PathObject(double x, double y, const QPainterPath p, QRgb rgb, QGraphicsItem* parent = 0);
    PathObject(PathObject* obj, QGraphicsItem* parent = 0);
    ~PathObject();

    enum { Type = OBJ_TYPE_PATH };
    virtual int type() const { return Type; }

    QPainterPath objectCopyPath() const;
    QPainterPath objectSavePath() const;

    QPointF objectPos() const { return scenePos(); }
    double objectX() const { return scenePos().x(); }
    double objectY() const { return scenePos().y(); }

    void setObjectPos(const QPointF& point) { setPos(point.x(), point.y()); }
    void setObjectPos(double x, double y) { setPos(x, y); }
    void setObjectX(double x) { setObjectPos(x, objectY()); }
    void setObjectY(double y) { setObjectPos(objectX(), y); }

    void updateRubber(QPainter* painter = 0);
    virtual void vulcanize();
    virtual QPointF mouseSnapPoint(const QPointF& mousePoint);
    virtual QList<QPointF> allGripPoints();
    virtual void gripEdit(const QPointF& before, const QPointF& after);
protected:
    void paint(QPainter*, const QStyleOptionGraphicsItem*, QWidget*);
private:
    void init(double x, double y, const QPainterPath& p, QRgb rgb, Qt::PenStyle lineType);
    void updatePath(const QPainterPath& p);
    QPainterPath normalPath;
    //TODO: make paths similar to polylines. Review and implement any missing functions/members.
};


class PointObject : public BaseObject
{
public:
    PointObject(double x, double y, QRgb rgb, QGraphicsItem* parent = 0);
    PointObject(PointObject* obj, QGraphicsItem* parent = 0);
    ~PointObject();

    enum { Type = OBJ_TYPE_POINT };
    virtual int type() const { return Type; }

    QPainterPath objectSavePath() const;

    QPointF objectPos() const { return scenePos(); }
    double objectX() const { return scenePos().x(); }
    double objectY() const { return scenePos().y(); }

    void setObjectPos(const QPointF& point) { setPos(point.x(), point.y()); }
    void setObjectPos(double x, double y) { setPos(x, y); }
    void setObjectX(double x) { setObjectPos(x, objectY()); }
    void setObjectY(double y) { setObjectPos(objectX(), y); }

    void updateRubber(QPainter* painter = 0);
    virtual void vulcanize();
    virtual QPointF mouseSnapPoint(const QPointF& mousePoint);
    virtual QList<QPointF> allGripPoints();
    virtual void gripEdit(const QPointF& before, const QPointF& after);
protected:
    void paint(QPainter*, const QStyleOptionGraphicsItem*, QWidget*);
private:
    void init(double x, double y, QRgb rgb, Qt::PenStyle lineType);
};


class PolygonObject : public BaseObject
{
public:
    PolygonObject(double x, double y, const QPainterPath& p, QRgb rgb, QGraphicsItem* parent = 0);
    PolygonObject(PolygonObject* obj, QGraphicsItem* parent = 0);
    ~PolygonObject();

    enum { Type = OBJ_TYPE_POLYGON };
    virtual int type() const { return Type; }

    QPainterPath objectCopyPath() const;
    QPainterPath objectSavePath() const;

    QPointF objectPos() const { return scenePos(); }
    double objectX() const { return scenePos().x(); }
    double objectY() const { return scenePos().y(); }

    void setObjectPos(const QPointF& point) { setPos(point.x(), point.y()); }
    void setObjectPos(double x, double y) { setPos(x, y); }
    void setObjectX(double x) { setObjectPos(x, objectY()); }
    void setObjectY(double y) { setObjectPos(objectX(), y); }

    void updateRubber(QPainter* painter = 0);
    virtual void vulcanize();
    virtual QPointF mouseSnapPoint(const QPointF& mousePoint);
    virtual QList<QPointF> allGripPoints();
    virtual void gripEdit(const QPointF& before, const QPointF& after);
protected:
    void paint(QPainter*, const QStyleOptionGraphicsItem*, QWidget*);
private:
    void init(double x, double y, const QPainterPath& p, QRgb rgb, Qt::PenStyle lineType);
    void updatePath(const QPainterPath& p);
    QPainterPath normalPath;
    int findIndex(const QPointF& point);
    int gripIndex;
};


class PolylineObject : public BaseObject
{
public:
    PolylineObject(double x, double y, const QPainterPath& p, QRgb rgb, QGraphicsItem* parent = 0);
    PolylineObject(PolylineObject* obj, QGraphicsItem* parent = 0);
    ~PolylineObject();

    enum { Type = OBJ_TYPE_POLYLINE };
    virtual int type() const { return Type; }

    QPainterPath objectCopyPath() const;
    QPainterPath objectSavePath() const;

    QPointF objectPos() const { return scenePos(); }
    double objectX() const { return scenePos().x(); }
    double objectY() const { return scenePos().y(); }

    void setObjectPos(const QPointF& point) { setPos(point.x(), point.y()); }
    void setObjectPos(double x, double y) { setPos(x, y); }
    void setObjectX(double x) { setObjectPos(x, objectY()); }
    void setObjectY(double y) { setObjectPos(objectX(), y); }

    void updateRubber(QPainter* painter = 0);
    virtual void vulcanize();
    virtual QPointF mouseSnapPoint(const QPointF& mousePoint);
    virtual QList<QPointF> allGripPoints();
    virtual void gripEdit(const QPointF& before, const QPointF& after);
protected:
    void paint(QPainter*, const QStyleOptionGraphicsItem*, QWidget*);
private:
    void init(double x, double y, const QPainterPath& p, QRgb rgb, Qt::PenStyle lineType);
    void updatePath(const QPainterPath& p);
    QPainterPath normalPath;
    int findIndex(const QPointF& point);
    int gripIndex;
};


class RectObject : public BaseObject
{
public:
    RectObject(double x, double y, double w, double h, QRgb rgb, QGraphicsItem* parent = 0);
    RectObject(RectObject* obj, QGraphicsItem* parent = 0);
    ~RectObject();

    enum { Type = OBJ_TYPE_RECTANGLE };
    virtual int type() const { return Type; }

    QPainterPath objectSavePath() const;

    QPointF objectPos() const { return scenePos(); }

    QPointF objectTopLeft() const;
    QPointF objectTopRight() const;
    QPointF objectBottomLeft() const;
    QPointF objectBottomRight() const;
    double objectWidth() const { return rect().width()*scale(); }
    double objectHeight() const { return rect().height()*scale(); }
    double objectArea() const { return qAbs(objectWidth()*objectHeight()); }

    void setObjectRect(double x, double y, double w, double h);

    void updateRubber(QPainter* painter = 0);
    virtual void vulcanize();
    virtual QPointF mouseSnapPoint(const QPointF& mousePoint);
    virtual QList<QPointF> allGripPoints();
    virtual void gripEdit(const QPointF& before, const QPointF& after);
protected:
    void paint(QPainter*, const QStyleOptionGraphicsItem*, QWidget*);
private:
    void init(double x, double y, double w, double h, QRgb rgb, Qt::PenStyle lineType);
    void updatePath();
};


class TextSingleObject : public BaseObject
{
public:
    TextSingleObject(const QString& str, double x, double y, QRgb rgb, QGraphicsItem* parent = 0);
    TextSingleObject(TextSingleObject* obj, QGraphicsItem* parent = 0);
    ~TextSingleObject();

    enum { Type = OBJ_TYPE_TEXTSINGLE };
    virtual int type() const { return Type; }

    QList<QPainterPath> objectSavePathList() const { return subPathList(); }
    QList<QPainterPath> subPathList() const;

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
    QPointF objectPos() const { return scenePos(); }
    double objectX() const { return scenePos().x(); }
    double objectY() const { return scenePos().y(); }

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
    void setObjectPos(const QPointF& point) { setPos(point.x(), point.y()); }
    void setObjectPos(double x, double y) { setPos(x, y); }
    void setObjectX(double x) { setObjectPos(x, objectY()); }
    void setObjectY(double y) { setObjectPos(objectX(), y); }

    void updateRubber(QPainter* painter = 0);
    virtual void vulcanize();
    virtual QPointF mouseSnapPoint(const QPointF& mousePoint);
    virtual QList<QPointF> allGripPoints();
    virtual void gripEdit(const QPointF& before, const QPointF& after);
protected:
    void paint(QPainter*, const QStyleOptionGraphicsItem*, QWidget*);
private:
    void init(const QString& str, double x, double y, QRgb rgb, Qt::PenStyle lineType);
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
    void updateLineEditNumIfVaries(QLineEdit* lineEdit, double num, bool useAnglePrecision);
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

    QGroupBox*   createGroupBoxGeneral();
    QGroupBox*   createGroupBoxGeometryArc();
    QGroupBox*   createGroupBoxMiscArc();
    QGroupBox*   createGroupBoxGeometryBlock();
    QGroupBox*   createGroupBoxGeometryCircle();
    QGroupBox*   createGroupBoxGeometryDimAligned();
    QGroupBox*   createGroupBoxGeometryDimAngular();
    QGroupBox*   createGroupBoxGeometryDimArcLength();
    QGroupBox*   createGroupBoxGeometryDimDiameter();
    QGroupBox*   createGroupBoxGeometryDimLeader();
    QGroupBox*   createGroupBoxGeometryDimLinear();
    QGroupBox*   createGroupBoxGeometryDimOrdinate();
    QGroupBox*   createGroupBoxGeometryDimRadius();
    QGroupBox*   createGroupBoxGeometryEllipse();
    QGroupBox*   createGroupBoxGeometryImage();
    QGroupBox*   createGroupBoxMiscImage();
    QGroupBox*   createGroupBoxGeometryInfiniteLine();
    QGroupBox*   createGroupBoxGeometryLine();
    QGroupBox*   createGroupBoxGeometryPath();
    QGroupBox*   createGroupBoxMiscPath();
    QGroupBox*   createGroupBoxGeometryPoint();
    QGroupBox*   createGroupBoxGeometryPolygon();
    QGroupBox*   createGroupBoxGeometryPolyline();
    QGroupBox*   createGroupBoxMiscPolyline();
    QGroupBox*   createGroupBoxGeometryRay();
    QGroupBox*   createGroupBoxGeometryRectangle();
    QGroupBox*   createGroupBoxGeometryTextMulti();
    QGroupBox*   createGroupBoxTextTextSingle();
    QGroupBox*   createGroupBoxGeometryTextSingle();
    QGroupBox*   createGroupBoxMiscTextSingle();
};

class Settings_Dialog : public QDialog
{
    Q_OBJECT

public:
    Settings_Dialog(MainWindow* mw, const QString& showTab = QString(), QWidget *parent = 0);
    ~Settings_Dialog();

private:
    QTabWidget*       tabWidget;

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

    void setMouseCoord(double x, double y);
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

    virtual QSize              sizeHint() const;
    QString getShortCurrentFile();
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

class EmbDetailsDialog : public QDialog
{
    Q_OBJECT

public:
    EmbDetailsDialog(QGraphicsScene* theScene, QWidget *parent = 0);
    ~EmbDetailsDialog();
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
    CmdPromptHistory* promptHistory;
    QVBoxLayout* promptVBoxLayout;
    QFrame* promptDivider;

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

    void setUndoCleanIcon(bool opened);

    virtual void updateMenuToolbarStatusbar();

    MdiArea* mdiArea;
    CmdPrompt* prompt;
    PropertyEditor* dockPropEdit;
    UndoEditor* dockUndoEdit;
    StatusBar* statusbar;
    QTimer* testing_timer;

    QList<QGraphicsItem*> cutCopyObjectList;

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

public:
    bool shiftKeyPressedState;

    QByteArray                      layoutState;

    int numOfDocs;
    int docIndex;

private:
    QList<MdiWindow*> listMdiWin;
    QMdiSubWindow* findMdiWindow(const QString &fileName);
    QString openFilesPath;

    QAction* myFileSeparator;

    QWizard*    wizardTipOfTheDay;
    QLabel*     labelTipOfTheDay;
    QCheckBox*  checkBoxTipOfTheDay;
    QStringList listTipOfTheDay;

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

    /* Selectors */
    QComboBox* layerSelector;
    QComboBox* colorSelector;
    QComboBox* linetypeSelector;
    QComboBox* lineweightSelector;
    QFontComboBox* textFontSelector;
    QComboBox* textSizeSelector;

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
    void runCommandClick(const QString& cmd,  double x, double y);
    void runCommandMove(const QString& cmd,  double x, double y);
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

    void setTextFont(const QString& str);
    void setTextSize(double num);

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

    void nativeEnableMoveRapidFire();
    void nativeDisableMoveRapidFire();

    void messageBox(const QString& type, const QString& title, const QString& text);

    void nativeRedo();

    void nativePrintArea(double x, double y, double w, double h);

    void nativeSetBackgroundColor(uint8_t r, uint8_t g, uint8_t b);
    void nativeSetCrossHairColor(uint8_t r, uint8_t g, uint8_t b);
    void nativeSetGridColor(uint8_t r, uint8_t g, uint8_t b);

    void nativeVulcanize();
    void nativeClearRubber();
    bool nativeAllowRubber();
    void nativeSpareRubber(int64_t id);
    //TODO: void nativeSetRubberFilter(int64_t id); //TODO: This is so more than 1 rubber object can exist at one time without updating all rubber objects at once
    void nativeSetRubberMode(int mode);
    void nativeSetRubberPoint(const QString& key, double x, double y);
    void nativeSetRubberText(const QString& key, const QString& txt);

    void nativeAddTextMulti(const QString& str, double x, double y, double rot, bool fill, int rubberMode);
    void nativeAddTextSingle(const QString& str, double x, double y, double rot, bool fill, int rubberMode);

    void nativeAddInfiniteLine(double x1, double y1, double x2, double y2, double rot);
    void nativeAddRay(double x1, double y1, double x2, double y2, double rot);
    void nativeAddLine(double x1, double y1, double x2, double y2, double rot, int rubberMode);
    void nativeAddTriangle(double x1, double y1, double x2, double y2, double x3, double y3, double rot, bool fill);
    void nativeAddRectangle(double x, double y, double w, double h, double rot, bool fill, int rubberMode);
    void nativeAddRoundedRectangle(double x, double y, double w, double h, double rad, double rot, bool fill);
    void nativeAddArc(EmbArc arc, int rubberMode);
    void nativeAddCircle(double centerX, double centerY, double radius, bool fill, int rubberMode);
    void nativeAddSlot(double centerX, double centerY, double diameter, double length, double rot, bool fill, int rubberMode);
    void nativeAddEllipse(double centerX, double centerY, double width, double height, double rot, bool fill, int rubberMode);
    void nativeAddPoint(double x, double y);
    void nativeAddRegularPolygon(double centerX, double centerY, quint16 sides, uint8_t mode, double rad, double rot, bool fill);
    void nativeAddPolygon(double startX, double startY, const QPainterPath& p, int rubberMode);
    void nativeAddPolyline(double startX, double startY, const QPainterPath& p, int rubberMode);
    void nativeAddPath(double startX, double startY, const QPainterPath& p, int rubberMode);
    void nativeAddHorizontalDimension(double x1, double y1, double x2, double y2, double legHeight);
    void nativeAddVerticalDimension(double x1, double y1, double x2, double y2, double legHeight);
    void nativeAddImage(const QString& img, double x, double y, double w, double h, double rot);

    void nativeAddDimLeader(double x1, double y1, double x2, double y2, double rot, int rubberMode);

    void nativeSetCursorShape(const QString& str);
    double nativeCalculateAngle(double x1, double y1, double x2, double y2);
    double nativeCalculateDistance(double x1, double y1, double x2, double y2);
    double nativePerpendicularDistance(double px, double py, double x1, double y1, double x2, double y2);

    int nativeNumSelected();
    void nativeSelectAll();
    void nativeAddToSelection(const QPainterPath path, Qt::ItemSelectionMode mode);
    void nativeClearSelection();
    void nativeDeleteSelected();
    void nativeCutSelected(double x, double y);
    void nativeCopySelected(double x, double y);
    void nativePasteSelected(double x, double y);
    void nativeMoveSelected(double dx, double dy);
    void nativeScaleSelected(double x, double y, double factor);
    void nativeRotateSelected(double x, double y, double rot);
    void nativeMirrorSelected(double x1, double y1, double x2, double y2);

    double nativeQSnapX();
    double nativeQSnapY();
};

MdiArea* getMdiArea();
MdiWindow* activeMdiWindow();
View* activeView();
QGraphicsScene* activeScene();
QUndoStack* activeUndoStack();

QIcon create_icon(QString icon);

QAction *get_action_by_icon(const char *icon);
void add_to_menu(QMenu *menu, const char *menu_data[]);

EmbVector to_emb_vector(QPointF p);
QPointF to_qpointf(EmbVector v);
QPointF scale_and_rotate(QPointF v, double angle, double scale);
QPointF find_mouse_snap_point(QList<QPointF> snap_points, const QPointF& mouse_point);

bool pattern_save(EmbPattern *pattern, const char *fileName);

extern QHash<QString, Command> command_map;
extern QHash<QString, QString>* aliasHash;
extern QHash<int, QAction*> actionHash;
extern QHash<QString, QToolBar*> toolbarHash;
extern QHash<QString, QMenu*> menuHash;

#endif
