/*!
 * \file qt.h
 *
 * Embroidermodder 2.
 *
 * Copyright 2011-2025 The Embroidermodder Team
 * Embroidermodder 2 is Open Source Software, see LICENSE.md for licensing terms.
 *
 * Read the reference manual (https://www.libembroidery.org/downloads/emrm.pdf)
 * for advice on altering this file.
 *
 * Qt wrapper
 */

#ifndef EMBROIDERMODDER_GUI_HEADER
#define EMBROIDERMODDER_GUI_HEADER

#include "core.h"

#include <QAction>
#include <QApplication>
#include <QGraphicsPathItem>
#include <QtPrintSupport>

class Object: public QGraphicsPathItem
{
public:
    ObjectCore *core;

    QGraphicsPathItem path_;
    QPen objPen;
    QPen lwtPen;
    QLineF objLine;
    QPainter* obj_painter;

    QPainterPath textPath;
    QPainterPath lineStylePath;
    QPainterPath arrowStylePath;
    QPainterPath normalPath;

    Object(int type_, QRgb rgb, Qt::PenStyle lineType, QGraphicsItem* item = 0);
    ~Object() { free_object(core); }

    void gripEdit(EmbVector before, EmbVector after);
    QPainterPath shape() const { return path(); }

    void drawRubberLine(QLineF rubLine, QPainter* painter = 0, const char* colorFromScene = 0);

    void setObjectSize(double width, EmbReal height);

    QPainterPath objectCopyPath() const;
    QPainterPath objectSavePath() const;
    QList<QPainterPath> objectSavePathList() const { return subPathList(); }
    QList<QPainterPath> subPathList() const;

protected:
    void paint(QPainter*, const QStyleOptionGraphicsItem*, QWidget*);
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

class LayerManager: public QDialog
{
    Q_OBJECT

public:
    LayerManager(QWidget *parent = 0);
    ~LayerManager() { restore_cursor(); }
    LayerData data[MAX_LAYERS];

    void addLayer(char *name, bool visible, bool frozen, EmbReal zValue,
        QRgb color, char *lineType, char *lineWeight, bool print);

    QStandardItemModel* layerModel;
    QSortFilterProxyModel* layerModelSorted;
    QTreeView* treeView;
};

class MdiArea: public QMdiArea
{
    Q_OBJECT

public:
    MdiArea(QWidget* parent = 0);
    ~MdiArea() {}

    bool useLogo;
    bool useTexture;
    bool useColor;

    QPixmap bgLogo;
    QPixmap bgTexture;
    QColor bgColor;

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
    Document(QGraphicsScene* theScene, QWidget* parent);
    ~Document() { free_doc(data->id); }

    DocumentData *data;

    QVector<int64_t> hashDeletedObjects;
    QPainterPath gridPath;
    QPainterPath originPath;

    QPainter* scene_painter;

    Object* gripBaseObj;

    QGraphicsScene* gscene;
    QUndoStack* undoStack;

    SelectBox* selectBox;
    QGraphicsItemGroup* pasteObjectItemGroup;

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

extern Document *documents[MAX_OPEN_FILES];

class UndoableCommand : public QUndoCommand
{
public:
    UndoableCommand(int type_, QString text, Object* obj, int32_t v,
        QUndoCommand* parent = 0);
    UndoableCommand(int type_, EmbVector delta, QString text, Object* obj,
        int32_t v, QUndoCommand* parent = 0);
    UndoableCommand(int type_, EmbVector pivot, EmbReal rotAngle, QString text,
        Object* obj, int32_t v, QUndoCommand* parent = 0);
    UndoableCommand(int type_, QString type, int32_t v, QUndoCommand* parent = 0);
    UndoableCommand(int type_, EmbVector start, EmbVector end, QString text,
        Object* obj, int32_t v, QUndoCommand* parent = 0);

    void undo();
    void redo();
    void rotate(double x, EmbReal y, EmbReal rot);
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
    UndoEditor(QString iconDirectory = "", QWidget* widgetToFocus = 0,
        QWidget* parent = 0); /*, Qt::WindowFlags flags = 0); */
    ~UndoEditor() {}

    void addStack(QUndoStack* stack);

    bool canUndo() const;
    bool canRedo() const;

    QString undoText() const;
    QString redoText() const;

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

class PropertyEditor: public QDockWidget
{
    Q_OBJECT

public:
    PropertyEditor(QString iconDirectory = "", bool pickAddMode = true,
        QWidget* widgetToFocus = 0, QWidget* parent = 0);
        /*, Qt::WindowFlags flags = 0); */
    ~PropertyEditor() {}

    void togglePickAddMode();

protected:
    bool eventFilter(QObject *obj, QEvent *event);

signals:
    void pick_add_mode_toggled();

public slots:
    void setSelectedItems(QList<QGraphicsItem*> itemList);
    void update_pick_add_modeButton(bool pickAddMode);
};

class MdiWindow: public QMdiSubWindow
{
    Q_OBJECT

public:
    MdiWindow(const int theIndex, QMdiArea* parent, Qt::WindowFlags wflags);
    ~MdiWindow() {
        debug_message("MdiWindow Destructor()");
        emb_pattern_free(documents[doc_index]->data->pattern);
    }

    QMdiArea* mdiArea;
    QGraphicsScene* gscene;
    int32_t doc_index;

    QPrinter printer;

    virtual QSize sizeHint() const;
    QString getShortCurrentFile();
    bool loadFile(char *fileName);
signals:
    void sendCloseMdiWin(MdiWindow*);

public slots:
    void closeEvent(QCloseEvent* e);
    void onWindowActivated();

    void print();
    void saveBMC();

private:
    void setCurrentFile(char *fileName);
};

/* On Mac, if the user drops a file on the app's Dock icon, or uses Open As,
 * then this is how the app actually opens the file.
 */
class Application : public QApplication
{
    Q_OBJECT
public:
    Application(int argc, char **argv);
protected:
    virtual bool event(QEvent *e);
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
    void checkSelection();
    void updateCurrentText(QString txt);
    void checkEditedText(QString txt);
    void checkChangedText(QString txt);
    void checkCursorPosition(int oldpos, int newpos);
private slots:
    void copyClip();
    void pasteClip();
};

class MainWindow: public QMainWindow
{
    Q_OBJECT

public:
    MainWindow();
    ~MainWindow() {
        debug_message("Destructor()");

        /* Prevent memory leaks by deleting any unpasted objects. */
        free_objects(cutCopyObjectList);
        free_id_list(cutCopyObjectList);
    }

    void add_toolbar_to_window(Qt::ToolBarArea area, int data[]);

public slots:
    void recentMenuAboutToShow();
    void windowMenuActivated(bool checked /*, int id*/);

    void closeToolBar(QAction*);
    void floatingChangedToolBar(bool);

    void runCommand();

    void colorSelectorIndexChanged(int index);

protected:
    virtual void resizeEvent(QResizeEvent*);
    void closeEvent(QCloseEvent *event);
};

#endif

